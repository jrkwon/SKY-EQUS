/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*
 * keyexchg.c
 *
 * Created by Anders Edenbrandt, Mon Jun 21 15:09:21 1999.
 *
 * Revision history:
 *
 *
 */
#include "wtlsdef.h"
#ifdef LOG_EXTERNAL
#include "aapiclnt.h"
#include "logcodes.h"
#endif

/************************************************************
 * Forward declarations of local functions.
 ************************************************************/

static INT16
wtls_key_exchange_extract_identifier (wtls_connection_t *conn,
                                      BYTE **id_buf, UINT16 *idlen);

static INT16
wtls_key_exchange_get_params (wtls_handshake_state *hs,
                              KeyExchangeParameters *param);


static INT16
wtls_key_exchange_get_cert_params (wtls_server_certificate *sc,
                                   KeyExchangeParameters *param);

/************************************************************
 * Public routines
 ************************************************************/

/*
 * Start the process of key exchange.
 *
 * This routine terminates by calling the adapter function
 * CRYPTa_keyExchange. When the answer from this function has
 * been delivered via a call to CRYPTc_keyExchangeResponse,
 * the following function wtls_key_exchange_continue can be called.
 */
SDL_Integer
wtls_key_exchange_init (void *connptr)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  KeyExchangeParameters param;
  BYTE                  tmprand[32];

  /* Concatenate: ClientHello.Random + ServerHello.Random */
  B_COPYSTRINGN (tmprand, conn->h_state->pending.client_hello_random, 16);
  B_COPYSTRINGN (tmprand + 16, conn->h_state->pending.server_hello_random, 16);
  
  param.keyExchangeSuite = conn->h_state->key_exch.key_exchange_suite;
  switch (param.keyExchangeSuite) {
  case KEY_EXCH_NULL:
    if (conn->h_state->msgs & BIT_SERVER_KEY_EXCHANGE) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNEXPECTED_KEY_EXCHANGE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_UNEXPECTED_MESSAGE);
      return -1;
    }
    break;

  case KEY_EXCH_SHARED_SECRET:
    if (conn->h_state->msgs & BIT_SERVER_KEY_EXCHANGE) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNEXPECTED_KEY_EXCHANGE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_UNEXPECTED_MESSAGE);
      return -1;
    }
    if (wtls_key_exchange_extract_identifier (conn,
                                              &param._u.secretKey.identifier,
                                              &param._u.secretKey.idLen) < 0) {
      return -1;
    }
    break;

  case KEY_EXCH_DH_ANON:
  case KEY_EXCH_DH_ANON_512:
  case KEY_EXCH_DH_ANON_768:
  case KEY_EXCH_RSA_ANON:
  case KEY_EXCH_RSA_ANON_512:
  case KEY_EXCH_RSA_ANON_768:
  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
    if (!(conn->h_state->msgs & BIT_SERVER_KEY_EXCHANGE)) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_MISSING_KEY_EXCHANGE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_HANDSHAKE_FAILURE);
      return -1;
    }
    wtls_key_exchange_get_params (conn->h_state, &param);
    break;

  case KEY_EXCH_RSA:
  case KEY_EXCH_RSA_512:
  case KEY_EXCH_RSA_768:
  case KEY_EXCH_ECDH_ECDSA:
    if (!(conn->h_state->msgs & BIT_SERVER_CERTIFICATE)) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_MISSING_SERVER_CERTIFICATE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_HANDSHAKE_FAILURE);
      return -1;
    }
    if (conn->h_state->msgs & BIT_SERVER_KEY_EXCHANGE) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNEXPECTED_KEY_EXCHANGE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_UNEXPECTED_MESSAGE);
      return -1;
    }
    wtls_key_exchange_get_cert_params (&(conn->h_state->server_certificate),
                                       &param);
    break;

  default:
    return -1;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_keyExchange,
             "WTLS: calling CRYPTa_keyExchange");
#endif
  CRYPTa_keyExchange ((UINT16)(conn->cm_proc), param,
                      wtls_crypto_MAC_to_hash_alg (conn->h_state->pending.mac_alg),
                      tmprand);

  if (param.keyExchangeSuite == KEY_EXCH_SHARED_SECRET) {
    DEALLOC (&param._u.secretKey.identifier);
  }

  return RET_OK;
}

/*
 * Continue and finish the process of key exchange.
 *
 * This function should be called when the result from CRYPTa_keyExchange
 * has been delivered via a call to CRYPTc_keyExchangeResponse.
 */
SDL_Integer
wtls_key_exchange_continue (void *connptr,
                            SDL_Integer result,
                            SDL_Octet master_secret_id,
                            void *public_value,
                            SDL_Natural public_valuelen)
{
  wtls_connection_t        *conn = (wtls_connection_t *)connptr;
  wtls_client_key_exchange *cke;

  if (result != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_KEY_EXCHANGE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_HANDSHAKE_FAILURE);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_keyExchange,
               "WTLS: error return from CRYPTa_keyExchange: %d", result);
#endif
    return -1;
  }
  conn->h_state->pending.master_secret_id = master_secret_id;
  CRYPTa_sessionInvalidate (conn->write.master_secret_id);

  switch (conn->h_state->key_exch.key_exchange_suite) {
  case KEY_EXCH_DH_ANON:
  case KEY_EXCH_DH_ANON_512:
  case KEY_EXCH_DH_ANON_768:
  case KEY_EXCH_RSA_ANON:
  case KEY_EXCH_RSA_ANON_512:
  case KEY_EXCH_RSA_ANON_768:
  case KEY_EXCH_RSA:
  case KEY_EXCH_RSA_512:
  case KEY_EXCH_RSA_768:
    cke = &(conn->h_state->client_key_exchange);
    cke->msg_type = HANDSHK_CLIENT_KEY_EXCHANGE;
    cke->length = (UINT16)(public_valuelen + 2);
    cke->buf = public_value;
    cke->buflen.len16 = (UINT16)public_valuelen;
    conn->h_state->need_client_key_exchange = 1;
    break;

  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
  case KEY_EXCH_ECDH_ECDSA:
    cke = &(conn->h_state->client_key_exchange);
    cke->msg_type = HANDSHK_CLIENT_KEY_EXCHANGE;
    cke->length = (UINT16)(public_valuelen + 2);
    cke->buf = public_value;
    cke->buflen.len8 = (UINT8)public_valuelen;
    conn->h_state->need_client_key_exchange = 1;
    break;

  default:
    conn->h_state->need_client_key_exchange = 0;
    DEALLOC (&public_value);
  }

  return RET_OK;
}

/************************************************************
 * Local functions
 ************************************************************/

/*
 * When the key exchange method is "shared secret", extract
 * the Identifier field from the corresponding key_exchange_id
 * in the Client Hello message.
 */
static INT16
wtls_key_exchange_extract_identifier (wtls_connection_t *conn,
                                      BYTE **id_buf, UINT16 *idlen)
{
  wtls_cvt_t cvt_obj;

  wtls_cvt_init (&cvt_obj, CVT_ENCODE_SIZE, NULL, 0);
  if (wtls_cvt_identifier (&cvt_obj, &(conn->h_state->key_exch.identifier))) {
    return -1;
  }
  *idlen = cvt_obj.pos;
  *id_buf = NEWARRAY (BYTE, *idlen);
  if (*id_buf == NULL) {
    return -1;
  }
  wtls_cvt_init (&cvt_obj, CVT_ENCODE, *id_buf, *idlen);
  if (wtls_cvt_identifier (&cvt_obj, &(conn->h_state->key_exch.identifier))) {
    return -1;
  }

  return RET_OK;
}

/*
 * Retrieve the key exchange parameters from the server
 * key exchange message.
 */
static INT16
wtls_key_exchange_get_params (wtls_handshake_state  *hs,
                              KeyExchangeParameters *param)
{
  BYTE   *buf;
  UINT16 buflen;
  UINT8  idx;

  if (hs->server_key_exchange.parameter_specifier.parameterIndex == 0) {
    buflen = hs->key_exch.parameter_specifier.paramLen;
    buf = hs->key_exch.parameter_specifier.params;
    idx = hs->key_exch.parameter_specifier.parameterIndex;
  }
  else {
    buflen = hs->server_key_exchange.parameter_specifier.paramLen;
    buf = hs->server_key_exchange.parameter_specifier.params;
    idx = hs->server_key_exchange.parameter_specifier.parameterIndex;
  }
  param->_u.keyParam.parameterSpecifier.parameterIndex = idx;
  param->_u.keyParam.parameterSpecifier.paramLen = buflen;
  param->_u.keyParam.parameterSpecifier.params = buf;
  
  param->_u.keyParam.pubKey = hs->server_key_exchange.public_key;

  return RET_OK;
}

/*
 * Retrieve the key exchange parameters from the server
 * certificate message.
 */
static INT16
wtls_key_exchange_get_cert_params (wtls_server_certificate *sc,
                                   KeyExchangeParameters   *param)
{
  param->_u.certificates.bufLen = sc->buflen;
  param->_u.certificates.buf = sc->buf;

  return RET_OK;
}
