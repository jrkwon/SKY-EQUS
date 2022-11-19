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
 * connmgr.c
 *
 * Created by Anders Edenbrandt, Wed Jun 02 09:10:51 1999.
 *
 * Revision history:
 *
 *
 * This file contains functions that are used by the SDL process
 * "ConnectionManager". That process implements the handshake protocol
 * in WTLS.
 *
 */
#include "wtlsdef.h"
#include "logcodes.h"

#include "aapicrpt.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "tapimmi.h"
#include "aapimmi.h"

/*
 * Forward declarations of local functions.
 */

static INT16
wtls_cm_check_client_key_id_index (wtls_connection_t *conn,
                                   wtls_client_hello *ch,
                                   wtls_server_hello *sh);

static INT16
wtls_cm_check_cipher_suite (wtls_connection_t *conn,
                            wtls_client_hello *ch,
                            wtls_server_hello *sh);

static INT16
wtls_cm_make_cipher_suites (INT8 bearer,
                            BYTE *methods, UINT16 methodslen,
                            BYTE **cs, UINT8 *cslen);

static void
wtls_cm_set_key_refresh_rate (UINT8 bearer, UINT8 *key_refresh_rate);

static void
wtls_cm_mkaddr (AddressType *inaddr, BYTE *outaddr);


/************************************************************
 * External routines, called from SDL.
 ************************************************************/

SDL_Integer
wtls_cm_get_handshake_type (void *bufptr, SDL_Natural buflen)
{
  BYTE *buf = (BYTE *)bufptr;

  if ((buf == NULL) || (buflen < 1)) {
    return HANDSHK_ERROR;
  }

  return (SDL_Integer)buf[0];
}


/*
 * Initialize the Connection Manager process.
 */
void *
wtls_cm_init (SDL_PId cm_proc, SDL_PId rec_proc,
              AddressType src_addr, AddressType dst_addr,
              void *proxy_provider_id, SDL_Boolean allow_anon)
{
  wtls_connection_t *conn = NEWARRAY (wtls_connection_t, 1);
  INT16             i;

  if (conn == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return NULL;
  }

  wtls_main_set_connection_pointer (cm_proc, conn);

  yDef_AddressType (&(conn->client_addr));
  yDef_AddressType (&(conn->server_addr));
  yAssF_AddressType (conn->client_addr, src_addr, XASS);
  yAssF_AddressType (conn->server_addr, dst_addr, XASS);

  conn->cm_proc = cm_proc;
  conn->rec_proc = rec_proc;

  conn->h_state = NULL;
  wtls_connection_state_init (&(conn->read));
  wtls_connection_state_init (&(conn->write));

  conn->read_cksum = 0;
  for (i = 0; i < 4; i++) {
    conn->write_cksum[i] = 0;
  }
  conn->num_cksums = 0;

  conn->mask = 0;
  conn->out_records = NULL;
  conn->proxy_provider_id = (char *)proxy_provider_id;
  conn->allow_anon = (allow_anon == SDL_True) ? TRUE : FALSE;

  return conn;
}

void
wtls_cm_delete_connections_session (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  if (conn == NULL) {
    return;
  }    

  /* Do not remove the session if we are in the middle of a
   * handshake, and are negotiating a new session. */
  if ((conn->h_state != NULL) /* Doing a handshake */
      && (conn->h_state->msgs & BIT_CLIENT_HELLO) /* We sent client hello */
      && ((conn->h_state->client_hello.session_id.length == 0)/* Not reusing session */
          || ((conn->h_state->msgs & BIT_SERVER_HELLO) &&
              (conn->h_state->server_hello.session_id.length > 0) &&
              ((conn->h_state->client_hello.session_id.length !=
                conn->h_state->server_hello.session_id.length) ||
               (memcmp (conn->h_state->client_hello.session_id.id,
                        conn->h_state->server_hello.session_id.id,
                        conn->h_state->client_hello.session_id.length) != 0)))
          )
      ) {
    return;
  }

  if (conn->read.session_id.length > 0) {
    /* There exists a cached session. */
    CRYPTa_sessionInvalidate (conn->read.master_secret_id);
  }
}

/*
 * Return True if a handshake is in progress and that handshake
 * can be aborted, i.e., if the pending state has not yet been
 * installed neither on the read nor the write side.
 */
SDL_Boolean
wtls_cm_can_abort_handshake (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  if ((conn != NULL) &&
      (conn->h_state != NULL) &&
      (conn->h_state->abortable == 1)) {
    return SDL_True;
  }
  else {
    return SDL_False;
  }
}

/*
 * Abort an ongoing handshake.
 */
void
wtls_cm_abort_handshake (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  if (conn != NULL) {
    /* Delete the data that is only used during the handshake. */
    wtls_handshake_state_free (conn->h_state);
    conn->h_state = NULL;
  }

}

/*
 * The handshake protocol has completed; store necessary
 * information, and delete data that is no longer required.
 */
void
wtls_cm_handshake_finished (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_server_hello *sh = &(conn->h_state->server_hello);
  BYTE              addr[18];
  UINT8             session_options = 0;

  if (conn->h_state->msgs & BIT_SERVER_HELLO_DONE) {
    /* It was a full handshake, hence we got a new
     * master secret. Remove links to it, in case it is a
     * slot that has been reused. */
    CRYPTa_peerDeleteLinks (conn->write.master_secret_id);

    /* Cache the session. */
    if (sh->session_id.length > 0) {
      session_options |= SESSION_OPTIONS_RESUMABLE;
    }
    if (conn->h_state->msgs & BIT_SERVER_CERTIFICATE) {
      session_options |= SESSION_OPTIONS_SERVER_AUTH;
    }
    if (conn->h_state->msgs & BIT_CLIENT_CERTIFICATE_VERIFY) {
      session_options |= SESSION_OPTIONS_CLIENT_AUTH;
    }
#ifdef FEATURE_WAP
  CRYPTa_sessionUpdate (conn->write.master_secret_id,
                        session_options,
                        sh->session_id.id,
                        (UINT8)sh->session_id.length,
                        conn->write.cobj.mac_alg,
                        conn->write.cobj.bulk_cipher_alg,
                        conn->write.cobj.compression_alg,
                        (BYTE *)"\x00\x00\x00\x00",
                        CLNTa_currentTime ());
#else
  CRYPTa_sessionUpdate (conn->write.master_secret_id,
                        session_options,
                        sh->session_id.id,
                        (UINT8)sh->session_id.length,
                        conn->write.cobj.mac_alg,
                        conn->write.cobj.bulk_cipher_alg,
                        conn->write.cobj.compression_alg,
                        "\x00\x00\x00\x00",
                        CLNTa_currentTime ());
#endif //FEATURE_WAP
  }

  wtls_cm_mkaddr (&(conn->server_addr), addr);
  CRYPTa_peerLinkToSession (addr, (UINT8)(addr[1] + 2),
                            (UINT16)conn->server_addr.PortNumber,
                            conn->write.master_secret_id);
  CRYPTa_sessionActive (conn->write.master_secret_id, 1);

  CRYPTa_connectionEstablished (conn->server_addr.ViewID,
                                conn->server_addr.ChannelID,
                                conn->write.master_secret_id);

  /* Delete the data that is only used during the handshake. */
  wtls_handshake_state_free (conn->h_state);
  conn->h_state = NULL;
}

/*
 * Perform necessary initializations to get a handshake started.
 */
SDL_Integer
wtls_cm_handshake_init (void *connptr, SDL_Boolean established)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  UINT32            gmt_unix_time;
  INT16             r;
  BYTE              addr[18];

  conn->h_state = wtls_handshake_state_new ();
  if (conn->h_state == NULL) {
    return -1;
  }

  wtls_cm_set_key_refresh_rate (conn->server_addr.Bearer,
                                &(conn->h_state->pending.key_refresh_rate));

  gmt_unix_time = CLNTa_currentTime ();
  if (cfg_wae_ua_current_time_is_gmt) {
    conn->h_state->pending.client_hello_random[0] =
      (BYTE)((gmt_unix_time >> 24) & 0xff);
  }
  else {
    conn->h_state->pending.client_hello_random[0] = 0;
  }
  conn->h_state->pending.client_hello_random[1] =
    (BYTE)((gmt_unix_time >> 16) & 0xff);
  conn->h_state->pending.client_hello_random[2] =
    (BYTE)((gmt_unix_time >> 8) & 0xff);
  conn->h_state->pending.client_hello_random[3] =
    (BYTE)(gmt_unix_time & 0xff);
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_generateRandom,
             "WTLS: calling CRYPTa_generateRandom");
#endif
  if ((r = CRYPTa_generateRandom (&(conn->h_state->pending.client_hello_random[4]), 12)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_GENERATE_RANDOM, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_generateRandom,
               "WTLS: error return from CRYPTa_generateRandom: %d\n", r);
#endif
    return -1;
  }

  if (!conn->allow_anon) {
    conn->h_state->pending.session_id.length = 0;
    return 1;
  }
  if (established == SDL_True) {
    conn->h_state->pending.master_secret_id = conn->read.master_secret_id;
    CRYPTa_sessionFetch ((UINT16)conn->cm_proc,
                         conn->read.master_secret_id);
  }
  else {
    wtls_cm_mkaddr (&(conn->server_addr), addr);
    CRYPTa_peerLookup ((UINT16)conn->cm_proc, addr, (UINT8)(addr[1] + 2),
                       (UINT16)conn->server_addr.PortNumber);
  }

  return RET_OK;
}

INT16
wtls_cm_session_fetch1 (void *connptr, SDL_Integer result,
                        SDL_Octet session_options,
                        void *session_id, SDL_Octet session_length,
                        SDL_Octet mac_alg, SDL_Octet cipher_alg,
                        SDL_Octet compression_alg, void *private_key_id,
                        SDL_Natural creation_time)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              addr[18];
  INT16             r = RET_OK;

  /* To get rid of warnings: */
  creation_time = creation_time;

  if (private_key_id != NULL)
    OSConnectorFree (private_key_id);

  if ((result == CRV_OK) &&
      (session_options & SESSION_OPTIONS_RESUMABLE)) {
    memcpy (conn->h_state->pending.session_id.id, session_id,
            session_length);
    conn->h_state->pending.session_id.length = session_length;
    conn->h_state->pending.mac_alg = mac_alg;
    conn->h_state->pending.bulk_cipher_alg = cipher_alg;
    conn->h_state->pending.compression_alg = compression_alg;

    r = RET_USING_CURRENT_SESSION;
  }
  else {
    wtls_cm_mkaddr (&(conn->server_addr), addr);
    CRYPTa_peerLookup ((UINT16)conn->cm_proc, addr, (UINT8)(addr[1] + 2),
                       (UINT16)conn->server_addr.PortNumber);
  }
  if (session_id != NULL)
    OSConnectorFree (session_id);

  return r;
}

INT16
wtls_cm_session_fetch2 (void *connptr, SDL_Integer result,
                        SDL_Octet master_secret_id)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  if (result == CRV_OK) {
    conn->h_state->pending.master_secret_id = master_secret_id;
    CRYPTa_sessionFetch ((UINT16)conn->cm_proc, master_secret_id);
    return RET_OK;
  }
  else if (result != CRV_OK) {
    return RET_NO_PEER;
  }
  else {
    return -1;
  }
}

INT16
wtls_cm_session_fetch3 (void *connptr, SDL_Integer result,
                        SDL_Octet session_options,
                        void *session_id, SDL_Octet session_length,
                        SDL_Octet mac_alg, SDL_Octet cipher_alg,
                        SDL_Octet compression_alg, void *private_key_id,
                        SDL_Natural creation_time)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  /* To get rid of warnings: */
  creation_time = creation_time;

  if (private_key_id != NULL)
    OSConnectorFree (private_key_id);

  if ((result == CRV_OK) &&
      (session_options & SESSION_OPTIONS_RESUMABLE)) {
    memcpy (conn->h_state->pending.session_id.id, session_id,
            session_length);
    conn->h_state->pending.session_id.length = session_length;
    conn->h_state->pending.mac_alg = mac_alg;
    conn->h_state->pending.bulk_cipher_alg = cipher_alg;
    conn->h_state->pending.compression_alg = compression_alg;
  }
  else {
    conn->h_state->pending.session_id.length = 0;
  }
  if (session_id != NULL)
    OSConnectorFree (session_id);

  return RET_OK;
}

static void
wtls_cm_push_buffer (wtls_connection_t *conn, BYTE *buf, UINT16 buflen)
{
  wtls_handshake_state *hs = conn->h_state;

  hs->hm[hs->num_handshake_msgs].buf = buf;
  hs->hm[hs->num_handshake_msgs].buflen = buflen;
  hs->num_handshake_msgs++;
}

static void
wtls_cm_copy_buffer (wtls_connection_t *conn, BYTE *buf, UINT16 buflen)
{
  wtls_handshake_state *hs = conn->h_state;
  BYTE                 *cbuf;

  cbuf = NEWARRAY (BYTE, buflen);
  B_COPYSTRINGN (cbuf, buf, buflen);
  hs->hm[hs->num_handshake_msgs].buf = cbuf;
  hs->hm[hs->num_handshake_msgs].buflen = buflen;
  hs->num_handshake_msgs++;
}

SDL_Integer
wtls_cm_create_client_hello (void *connptr)
{
  wtls_connection_t    *conn = (wtls_connection_t *)connptr;
  wtls_handshake_state *hs = conn->h_state;
  wtls_client_hello    *ch = &(hs->client_hello);

  ch->client_version = WTLS_PROTOCOL_VERSION;
  B_COPYSTRINGN (ch->random, hs->pending.client_hello_random, 16);

  ch->session_id = hs->pending.session_id;

  /* Finally, sequence number and key refresh */
  ch->seqnum_mode = SEQNUMMODE_EXPLICIT;
  ch->key_refresh_rate = hs->pending.key_refresh_rate;

  /* The following values are fetched from Cryptcfg.h */
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_getMethods, "WTLS: calling CRYPTa_getMethods");
#endif
  CRYPTa_getMethods ((UINT16)conn->cm_proc);

  return RET_OK;
}

SDL_Integer
wtls_cm_continue_create_client_hello (void *connptr, SDL_Integer result,
                                      void *cmptr, SDL_Natural cmlen,
                                      void *kxptr, SDL_Natural kxlen,
                                      void *tkptr, SDL_Natural tklen,
                                      void **pbufptr, SDL_Natural *pbuflen)
{
  wtls_connection_t    *conn = (wtls_connection_t *)connptr;
  wtls_handshake_state *hs = conn->h_state;
  wtls_client_hello    *ch = &(hs->client_hello);
  wap_cvt_t            cvt_obj;
  BYTE                 *buf;
  UINT16               buflen;

  if (result != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_GET_METHODS,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&cmptr);
    DEALLOC (&kxptr);
    DEALLOC (&tkptr);
    return -1;
  }
  if (!conn->allow_anon) {
    /* Go through all the key exchange methods and remove the ANON ones. */
    /* Sigh... */
    wap_cvt_t            decode_obj, encode_obj;
    wtls_key_exchange_id kid;

    wap_cvt_init (&decode_obj, WAP_CVT_DECODE_STATIC, kxptr, kxlen);
    wap_cvt_init (&encode_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
    while (decode_obj.pos < decode_obj.length) {
      if (!wtls_cvt_key_exchange_id (&decode_obj, &kid))
        break;
      if ((kid.key_exchange_suite == KEY_EXCH_RSA) ||
          (kid.key_exchange_suite == KEY_EXCH_RSA_512) ||
          (kid.key_exchange_suite == KEY_EXCH_RSA_768) ||
          (kid.key_exchange_suite == KEY_EXCH_ECDH_ECDSA)) {
        wtls_cvt_key_exchange_id (&encode_obj, &kid);
      }
    }
    if (encode_obj.pos == 0) {
      /* No non-anonymous methods available */
      wtls_err_set (ERR_CRYPTLIB, ERR_GET_METHODS,
                    0, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
      DEALLOC (&cmptr);
      DEALLOC (&kxptr);
      DEALLOC (&tkptr);
      return -1;
    }
    if (encode_obj.pos < kxlen) {
      /* Some methods were excluded */
      UINT16  len = (UINT16)encode_obj.pos;
      BYTE   *buf = OSConnectorAlloc (len);

      wap_cvt_init (&decode_obj, WAP_CVT_DECODE_STATIC, kxptr, kxlen);
      wap_cvt_init (&encode_obj, WAP_CVT_ENCODE, buf, len);
      while (decode_obj.pos < decode_obj.length) {
        if (!wtls_cvt_key_exchange_id (&decode_obj, &kid))
          break;
        if ((kid.key_exchange_suite == KEY_EXCH_RSA) ||
            (kid.key_exchange_suite == KEY_EXCH_RSA_512) ||
            (kid.key_exchange_suite == KEY_EXCH_RSA_768) ||
            (kid.key_exchange_suite == KEY_EXCH_ECDH_ECDSA)) {
          wtls_cvt_key_exchange_id (&encode_obj, &kid);
        }
      }
      OSConnectorFree (kxptr);
      kxptr = buf;
      kxlen = len;
    }
  }

  wtls_cm_make_cipher_suites (conn->server_addr.Bearer,
                              (BYTE *)cmptr, (UINT16)cmlen,
                              &(ch->cipher_suites), &(ch->cipher_suites_len));
  DEALLOC (&cmptr);

  ch->client_key_ids = (BYTE *)kxptr;
  ch->client_key_id_len = (UINT16)kxlen;

  ch->trusted_key_ids = (BYTE *)tkptr;
  ch->trusted_key_id_len = (UINT16)tklen;

  ch->compression_methods = NEWARRAY (BYTE, 1);
  ch->compression_methods[0] = COMPRESS_NULL;
  ch->num_compression_methods = 1;

  /* Convert to external format */
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  wtls_cvt_client_hello (&cvt_obj, ch);
  buflen = (UINT16)cvt_obj.pos;
  ch->msg_type = HANDSHK_CLIENT_HELLO;
  ch->length = (UINT16)(buflen - 3);
  buf = NEWARRAY (BYTE, buflen);
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  wtls_cvt_client_hello (&cvt_obj, ch);

  conn->h_state->msgs |= BIT_CLIENT_HELLO;
  wtls_cm_copy_buffer (conn, buf, buflen);

  *pbufptr = buf;
  *pbuflen = buflen;

  return RET_OK;
}

/*
 * Check the contents of the Server Hello message,
 * and copy parameters to the pending state.
 */
SDL_Integer
wtls_cm_process_server_hello (void *connptr, void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              *buf = (BYTE *)bufptr;
  wtls_server_hello *sh;
  wtls_client_hello *ch;
  INT16             i;
  INT16             resuming = FALSE;
  wap_cvt_t         cvt_obj;

  ch = &(conn->h_state->client_hello);
  sh = &(conn->h_state->server_hello);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_server_hello (&cvt_obj, sh)) {
    wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
    DEALLOC (&buf);
    return -1;
  }

  /* Protocol version */
  if (ch->client_version != sh->server_version) {
    DEALLOC (&buf);
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_MESSAGE_PARAMETERS,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
    return -1;
  }

  /* Random value */
  B_COPYSTRINGN (conn->h_state->pending.server_hello_random, sh->random, 16);

  /* Session Id */
  if ((ch->session_id.length > 0) &&
      (ch->session_id.length == sh->session_id.length) &&
      (memcmp (ch->session_id.id, sh->session_id.id,
               ch->session_id.length) == 0)) {
    resuming = TRUE;
  }
  else {
    conn->h_state->pending.session_id = sh->session_id;
  }

  /* Client key ID */
  if (!resuming) {
    if (wtls_cm_check_client_key_id_index (conn, ch, sh) != RET_OK) {
      DEALLOC (&buf);
      return -1;
    }
  }
  else {
    conn->h_state->key_exch.key_exchange_suite = 0;
  }

  /* Check selected cipher suite */
  if (!resuming) {
    if (wtls_cm_check_cipher_suite (conn, ch, sh) != RET_OK) {
      DEALLOC (&buf);
      return -1;
    }
  }
  else {
    if ((conn->h_state->pending.bulk_cipher_alg !=
         sh->cipher_suite.bulk_cipher_alg) ||
        (conn->h_state->pending.mac_alg != sh->cipher_suite.mac_alg)) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_CIPHER_SUITE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
      DEALLOC (&buf);
      return -1;
    }
  }

  /* Check selected compression method */
  if (!resuming) {
    for (i = 0; i < ch->num_compression_methods; i++) {
      if (ch->compression_methods[i] == sh->compression_method)
        break;
    }
    if (i == ch->num_compression_methods) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_COMPRESSION_ALG,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
      DEALLOC (&buf);
      return -1;
    }
    conn->h_state->pending.compression_alg = sh->compression_method;
  }
  else {
    if (conn->h_state->pending.compression_alg != sh->compression_method) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_COMPRESSION_ALG,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
      DEALLOC (&buf);
      return -1;
    }
  }

  /* Check sequence number mode */
  if (sh->seqnum_mode != SEQNUMMODE_EXPLICIT) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_SEQNUM_MODE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->pending.seqnum_mode = sh->seqnum_mode;

  /* Check key refresh rate */
  if (sh->key_refresh_rate > ch->key_refresh_rate) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_KEY_REFRESH,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->pending.key_refresh_rate = sh->key_refresh_rate;

  conn->h_state->msgs |= BIT_SERVER_HELLO;
  wtls_cm_push_buffer (conn, buf, (UINT16)buflen);

  if (resuming) {
    return RET_ABBR;
  }
  else if ((conn->h_state->key_exch.key_exchange_suite ==
            KEY_EXCH_SHARED_SECRET) ||
           (conn->h_state->key_exch.key_exchange_suite ==
            KEY_EXCH_NULL)) {
    return RET_SHARED_SECRET;
  }
  else {
    return RET_FULL;
  }
}

SDL_Integer
wtls_cm_process_server_certificate (void *connptr,
                                    void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t       *conn = (wtls_connection_t *)connptr;
  BYTE                    *buf = (BYTE *)bufptr;
  wtls_server_certificate *sc;
  wap_cvt_t               cvt_obj;

  sc = &(conn->h_state->server_certificate);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_server_certificate (&cvt_obj, sc)) {
    wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->msgs |= BIT_SERVER_CERTIFICATE;
  wtls_cm_push_buffer (conn, buf, (UINT16)buflen);

  return RET_OK;
}

SDL_Integer
wtls_cm_process_server_key_exchange (void *connptr,
                                     void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t        *conn = (wtls_connection_t *)connptr;
  BYTE                     *buf = (BYTE *)bufptr;
  wtls_server_key_exchange *ske;
  wap_cvt_t                 cvt_obj;

  ske = &(conn->h_state->server_key_exchange);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_server_key_exchange (&cvt_obj,
                                    conn->h_state->key_exch.key_exchange_suite, ske)) {
    wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->msgs |= BIT_SERVER_KEY_EXCHANGE;
  wtls_cm_push_buffer (conn, buf, (UINT16)buflen);

  return RET_OK;
}

SDL_Integer
wtls_cm_process_certificate_request (void *connptr,
                                     void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t        *conn = (wtls_connection_t *)connptr;
  BYTE                     *buf = (BYTE *)bufptr;
  wtls_certificate_request *cr;
  wap_cvt_t                 cvt_obj;

  cr = &(conn->h_state->certificate_request);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_certificate_request (&cvt_obj, cr)) {
    wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->msgs |= BIT_SERVER_CERTIFICATE_REQUEST;
  wtls_cm_push_buffer (conn, buf,(UINT16) buflen);

  return RET_OK;
}

INT16
wtls_cm_process_server_hello_done (void *connptr,
                                   void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t      *conn = (wtls_connection_t *)connptr;
  BYTE                   *buf = (BYTE *)bufptr;
  wtls_server_hello_done *shd;
  wap_cvt_t               cvt_obj;

  shd = &(conn->h_state->server_hello_done);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_server_hello_done (&cvt_obj, shd)) {
    wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->msgs |= BIT_SERVER_HELLO_DONE;
  wtls_cm_push_buffer (conn, buf, (UINT16)buflen);

  return RET_OK;
}

SDL_Integer
wtls_cm_create_client_certificate_msg (void *connptr)
{
  wtls_connection_t        *conn = (wtls_connection_t *)connptr;
  wtls_certificate_request *cr = &(conn->h_state->certificate_request);

  /* If the server has requested a client certificate, then we
   * must send a Certificate message. */
  if (conn->h_state->msgs & BIT_SERVER_CERTIFICATE_REQUEST) {
    if (conn->h_state->key_exch.identifier.identifier_type != IDENTIFIER_NULL) {
      return RET_SEND_MSG;
    }
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_getClientCertificate,
               "WTLS: calling CRYPTa_getClientCertificate");
#endif
    CRYPTa_getClientCertificate ((UINT16)conn->cm_proc,
                                 cr->buf, cr->buflen);
    return RET_GET_CERT;
  }

  return RET_OK;
}

SDL_Integer
wtls_cm_continue_create_client_certificate_msg (void *connptr)
{
  wtls_connection_t       *conn = (wtls_connection_t *)connptr;
  wtls_client_certificate *cc = &(conn->h_state->client_certificate);
  wap_cvt_t                cvt_obj;
  BYTE                    *buf;
  UINT16                   buflen;

  cc->msg_type = HANDSHK_CERTIFICATE;
  cc->buflen = 0;
  cc->buf = NULL;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wtls_cvt_client_certificate (&cvt_obj, cc)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  buflen = (UINT16)cvt_obj.pos;

  cc->length = buflen - 3;
  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
  }
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  if (!wtls_cvt_client_certificate (&cvt_obj, cc)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  wtls_cm_copy_buffer (conn, buf, buflen);
  conn->h_state->msgs |= BIT_CLIENT_CERTIFICATE;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  wtls_cvt_identifier (&cvt_obj, &(conn->h_state->key_exch.identifier));
  buflen = (UINT16)cvt_obj.pos;
  buf = NEWARRAY (BYTE, buflen);
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  if (!wtls_cvt_identifier (&cvt_obj, &(conn->h_state->key_exch.identifier))) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->key_id = buf;
  conn->h_state->key_idlen = buflen;

  return RET_OK;
}


SDL_Integer
wtls_cm_process_client_certificate_response (void *connptr,
                                             SDL_Natural result,
                                             void *key_id,
                                             SDL_Natural key_idlen,
                                             void **pbufptr,
                                             SDL_Natural *pbuflen)
{
  wtls_connection_t       *conn = (wtls_connection_t *)connptr;
  wtls_client_certificate *cc = &(conn->h_state->client_certificate);
  wap_cvt_t                cvt_obj;
  BYTE                    *buf;
  UINT16                   buflen;

  if (result == CRV_MISSING_CERTIFICATE) {
    cc->buflen = 0;
    cc->buf = NULL;
  }
  else if (result == CRV_OK) {
    cc->buflen = (UINT16)*pbuflen;
    cc->buf = (BYTE *)*pbufptr;
  }
  else {
    wtls_err_set (ERR_CRYPTLIB, ERR_GET_CLIENT_CERTIFICATE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  cc->length = 0;
  cc->msg_type = HANDSHK_CERTIFICATE;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wtls_cvt_client_certificate (&cvt_obj, cc)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  buflen = (UINT16)cvt_obj.pos;

  cc->length = buflen - 3;
  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
  }
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  if (!wtls_cvt_client_certificate (&cvt_obj, cc)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  wtls_cm_copy_buffer (conn, buf, buflen);
  conn->h_state->msgs |= BIT_CLIENT_CERTIFICATE;
  conn->h_state->key_id = key_id;
  conn->h_state->key_idlen = (UINT16)key_idlen;

  *pbufptr = buf;
  *pbuflen = buflen;

  return RET_OK;
}

SDL_Integer
wtls_cm_create_client_key_exchange (void *connptr,
                                    void **pbufptr, SDL_Natural *pbuflen)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wap_cvt_t          cvt_obj;

  /* In case we need to send a Client Key Exchange message */
  if (conn->h_state->need_client_key_exchange) {
    wtls_client_key_exchange *cke = &(conn->h_state->client_key_exchange);
    BYTE                     *buf;
    UINT16                   buflen;
    KeyExchangeSuite         kes = conn->h_state->key_exch.key_exchange_suite;

    cke->msg_type = HANDSHK_CLIENT_KEY_EXCHANGE;
    cke->length = 0;

    wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
    if (!wtls_cvt_client_key_exchange (&cvt_obj, kes, cke)) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    buflen = (UINT16)cvt_obj.pos;

    buf = NEWARRAY (BYTE, buflen);
    if (buf == NULL) {
      wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    }
    wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
    cke->length = buflen - 3;
    if (!wtls_cvt_client_key_exchange (&cvt_obj, kes, cke)) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    wtls_cm_copy_buffer (conn, buf, buflen);
    conn->h_state->msgs |= BIT_CLIENT_KEY_EXCHANGE;

    *pbufptr = buf;
    *pbuflen = buflen;

    return RET_SEND_MSG;
  }

  return RET_OK;
}

SDL_Integer
wtls_cm_create_certificate_verify_msg (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  wtls_crypto_t     cobj;
  BYTE              md[MAX_HASH_MAC_SIZE];
  INT16             i;

  if ((conn->h_state->msgs & BIT_CLIENT_CERTIFICATE) &&
      (conn->h_state->client_certificate.buflen != 0)) {
    /* Apply the current hash function to all previous handshake
     * messages. */
    wtls_crypto_init (&cobj,
                      conn->h_state->pending.bulk_cipher_alg,
                      conn->h_state->pending.mac_alg,
                      conn->h_state->pending.compression_alg);
    if (wtls_crypto_hash_init (&cobj) < 0) {
      return -1;
    }
    for (i = 0; i < conn->h_state->num_handshake_msgs; i++) {
      if (wtls_crypto_hash_update (&cobj,
                                   conn->h_state->hm[i].buf,
                                   conn->h_state->hm[i].buflen) < 0) {
        return -1;
      }
    }
    if (wtls_crypto_hash_final (&cobj, md) < 0) {
      return -1;
    }
    
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_computeSignature,
               "WTLS: calling CRYPTa_computeSignature");
#endif
    CRYPTa_computeSignature ((UINT16)conn->cm_proc,
                             conn->h_state->key_id, conn->h_state->key_idlen,
                             md, cobj.full_mac_size);

    return RET_SEND_MSG;
  }

  return RET_OK;
}

SDL_Integer
wtls_cm_process_compute_signature_response (void *connptr,
                                            SDL_Natural result,
                                            void **pbufptr,
                                            SDL_Natural *pbuflen)
{
  wtls_connection_t       *conn = (wtls_connection_t *)connptr;
  wtls_certificate_verify *cv = &(conn->h_state->certificate_verify);
  wap_cvt_t                cvt_obj;
  BYTE                    *buf;
  UINT16                   buflen;

  if (result != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_COMPUTE_SIGNATURE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  cv->msg_type = HANDSHK_CERTIFICATE_VERIFY;
  cv->siglen = (UINT16)*pbuflen;
  cv->signature = *pbufptr;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wtls_cvt_certificate_verify (&cvt_obj, cv)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  buflen = (UINT16)cvt_obj.pos;

  buf = NEWARRAY (BYTE, buflen);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
  }
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  cv->length = buflen - 3;
  if (!wtls_cvt_certificate_verify (&cvt_obj, cv)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  wtls_cm_copy_buffer (conn, buf, buflen);
  conn->h_state->msgs |= BIT_CLIENT_CERTIFICATE_VERIFY;

  *pbufptr = buf;
  *pbuflen = buflen;

  return RET_OK;
}

SDL_Integer
wtls_cm_verify_cert (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE               addr[18];
  UINT16             len = 0;

  /* If server sent a certificate, then verify it. */
  if (conn->h_state->msgs & BIT_SERVER_CERTIFICATE) {
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_verifyCertificateChain,
               "WTLS: calling CRYPTa_verifyCertificateChain");
#endif
    wtls_cm_mkaddr (&(conn->server_addr), addr);
    if (conn->proxy_provider_id != NULL)
      len = strlen (conn->proxy_provider_id);
    CRYPTa_verifyCertificateChain ((UINT16)conn->cm_proc,
                                   conn->h_state->server_certificate.buf,
                                   conn->h_state->server_certificate.buflen,
                                   addr, (UINT16)(addr[1] + 2),
                                   conn->proxy_provider_id, len);
    return RET_VERIFY_CERT;
  }

  return RET_OK;
}

SDL_Integer
wtls_cm_process_verify_cert_result (SDL_Integer res)
{
  SDL_Integer retval = -1;

  if (res == CRV_UNKNOWN_CERTIFICATE_TYPE) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNSUPPORTED_CERTIFICATE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_UNSUPPORTED_CERTIFICATE);
  }
  else if (res == CRV_NO_MATCHING_ROOT_CERTIFICATE) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNKNOWN_CA,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_UNKNOWN_CA);
  }
  else if ((res == CRV_BAD_CERTIFICATE) || (res == CRV_INVALID_COMMON_NAME)) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_BAD_CERTIFICATE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_BAD_CERTIFICATE);
  }
  else if (res == CRV_CERTIFICATE_EXPIRED) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_CERTIFICATE_EXPIRED,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_CERTIFICATE_EXPIRED);
  }
  else {
    retval = RET_OK;
  }

  return retval;
}


/*
 * Create a client Finished message to be sent to the server.
 * This is defined in WTLS spec section 10.5.9.
 */
SDL_Integer
wtls_cm_create_verify_data (void *connptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              md[MAX_HASH_MAC_SIZE];
  INT16             i;
  wtls_crypto_t     cobj;

  /* Apply the current hash function to all previous handshake
   * messages. */
  wtls_crypto_init (&cobj,
                    conn->h_state->pending.bulk_cipher_alg,
                    conn->h_state->pending.mac_alg,
                    conn->h_state->pending.compression_alg);
  if (wtls_crypto_hash_init (&cobj) < 0) {
    return -1;
  }
  for (i = 0; i < conn->h_state->num_handshake_msgs; i++) {
    if (wtls_crypto_hash_update (&cobj,
                                 conn->h_state->hm[i].buf,
                                 conn->h_state->hm[i].buflen) < 0) {
      return -1;
    }
  }
  if (wtls_crypto_hash_final (&cobj, md) < 0) {
    return -1;
  }

  /* PRF (master_secret, finished_label, H(handshake_messages))[0..11]; */
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_PRF, "WTLS: calling CRYPTa_PRF");
#endif

#ifdef FEATURE_WAP
  CRYPTa_PRF ((UINT16)conn->cm_proc,
              cobj.hash_alg,
              conn->h_state->pending.master_secret_id, NULL, 0,
              (BYTE *)"client finished",
              md, cobj.full_mac_size, 12);
#else
  CRYPTa_PRF ((UINT16)conn->cm_proc,
              cobj.hash_alg,
              conn->h_state->pending.master_secret_id, NULL, 0,
              "client finished",
              md, cobj.full_mac_size, 12);
#endif

  return RET_OK;
}

SDL_Integer
wtls_cm_create_finished (void *connptr, void **pbufptr, SDL_Natural *pbuflen)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              *buf = (BYTE *)*pbufptr;
  wtls_finished     *cf = &(conn->h_state->client_finished);
  wap_cvt_t          cvt_obj;

  if (*pbuflen != 12) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  cf->msg_type = HANDSHK_FINISHED;
  cf->length = 12;
  B_COPYSTRINGN (cf->verify_data, buf, 12);
  DEALLOC (&buf);

  buf = NEWARRAY (BYTE, 15);
  if (buf == NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_INSUFFICIENT_MEMORY,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
  }
  
  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, 15);
  if (!wtls_cvt_finished (&cvt_obj, cf)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&buf);
    return -1;
  }

  wtls_cm_copy_buffer (conn, buf, 15);
  conn->h_state->msgs |= BIT_CLIENT_FINISHED;

  *pbufptr = buf;
  *pbuflen = 15;

  return RET_OK;
}

SDL_Integer
wtls_cm_process_finished (void *connptr, void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              *buf = (BYTE *)bufptr;
  wtls_finished     *sf;
  wap_cvt_t          cvt_obj;
  wtls_crypto_t     *cobj = &(conn->read.cobj);
  BYTE               md[MAX_HASH_MAC_SIZE];
  INT16              i;

  /* Apply the current hash function to all previous handshake
   * messages. */
  if (wtls_crypto_hash_init (cobj) < 0) {
    DEALLOC (&buf);
    return -1;
  }
  for (i = 0; i < conn->h_state->num_handshake_msgs; i++) {
    if (wtls_crypto_hash_update (cobj,
                                 conn->h_state->hm[i].buf,
                                 conn->h_state->hm[i].buflen) < 0) {
      DEALLOC (&buf);
      return -1;
    }
  }
  if (wtls_crypto_hash_final (cobj, md) < 0) {
    DEALLOC (&buf);
    return -1;
  }

  sf = &(conn->h_state->server_finished);

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
  if (!wtls_cvt_finished (&cvt_obj, sf)) {
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    DEALLOC (&buf);
    return -1;
  }
  conn->h_state->msgs |= BIT_SERVER_FINISHED;
  wtls_cm_push_buffer (conn, buf, (UINT16)buflen);

  /* PRF (master_secret, finished_label, H(handshake_messages))[0..11]; */
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_PRF, "WTLS: calling CRYPTa_PRF");
#endif

#ifdef FEATURE_WAP
  CRYPTa_PRF ((UINT16)conn->cm_proc,
              cobj->hash_alg,
              conn->read.master_secret_id, NULL, 0,
              (BYTE *)"server finished",
              md, cobj->full_mac_size, 12);
#else
  CRYPTa_PRF ((UINT16)conn->cm_proc,
              cobj->hash_alg,
              conn->read.master_secret_id, NULL, 0,
              "server finished",
              md, cobj->full_mac_size, 12);
#endif //

  return RET_OK;
}

/*
 * Check a server Finished message that we received.
 * This is defined in WTLS spec section 10.5.9.
 */
SDL_Integer
wtls_cm_check_server_finished (void *connptr, SDL_Integer result,
                               void *bufptr, SDL_Natural buflen)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              *buf = (BYTE *)bufptr;
  wtls_finished     *sf = &(conn->h_state->server_finished);
  INT16             i;
  SDL_Integer       retval = RET_OK;

  if (result != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_SERVER_FINISHED,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_DECRYPT_ERROR);
    return -1;
  }

  if (buflen < 12) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_SERVER_FINISHED,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_DECRYPT_ERROR);
    DEALLOC (&buf);
    return -1;
  }

  /* Is it the same value as in the Finished message? */
#ifndef TEST_WTLS
  for (i = 0; i < 12; i++) {
    if (buf[i] != sf->verify_data[i]) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_SERVER_FINISHED,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_DECRYPT_ERROR);
      retval = -1;
      break;
    }
  }
#endif
  DEALLOC (&buf);

  return retval;
}

SDL_Integer
wtls_cm_process_data_msg (void *connptr,
                          void *bufptr, SDL_Natural buflen,
                          TDUnitdataIndType *data_ind)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  yAssF_AddressType (data_ind->DestinationAddress, conn->client_addr, XASS);
  yAssF_AddressType (data_ind->SourceAddress, conn->server_addr, XASS);
  data_ind->UserData = pdubuf_newFromData ((UINT16)buflen, (BYTE *)bufptr);

  return RET_OK;
}

/*
 * Check incoming messages.
 * If we see a data message, then we have confirmation and go to
 * state OPEN. If we see an alert, then check what type of alert.
 */
SDL_Integer
wtls_cm_opening_msg_ind (void *connptr, SDL_Natural msg_type,
                         void *bufptr, SDL_Natural buflen,
                         TDUnitdataIndType *data_ind)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  BYTE              *buf = (BYTE *)bufptr;
  wap_cvt_t          cvt_obj;
  wtls_alert_t       alert;

  switch (msg_type) {
  case CONTENT_TYPE_ALERT:
    wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, buf, (UINT16)buflen);
    if (!wtls_cvt_alert (&cvt_obj, &alert)) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }

    if (alert.level == ALERT_LEVEL_WARNING) {
      if (alert.description == ALERT_DESC_DUPLICATE_FINISHED_RECEIVED) {
        return RET_DUPLICATE_FINISHED;
      }
    }
    wtls_err_set (ERR_RECEIVED_ALERT, ERR_RECEIVED_ALERT,
                  0, alert.level, alert.description);
    return -1;

  case CONTENT_TYPE_DATA:
    yAssF_AddressType (data_ind->DestinationAddress, conn->client_addr, XASS);
    yAssF_AddressType (data_ind->SourceAddress, conn->server_addr, XASS);
    data_ind->UserData = pdubuf_newFromData ((UINT16)buflen, buf);
    return RET_OK;

  case CONTENT_TYPE_HANDSHAKE:
    if (wtls_cm_get_handshake_type (buf, buflen) == HANDSHK_HELLO_REQUEST) {
      DEALLOC (&buf);
      return RET_HELLO_REQUEST;
    }
    DEALLOC (&buf);

    default:
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_UNEXPECTED_MESSAGE,
                    1, ALERT_LEVEL_CRITICAL, ALERT_DESC_UNEXPECTED_MESSAGE);
      return -1;
  }
}

SDL_Integer
wtls_cm_get_next_outbound_message (void *connptr,
                                   void **precptr)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;
  
  *precptr = wtls_main_pop_outbound_message (conn->cm_proc);
  if (*precptr == NULL) {
    return RET_QUEUE_EMPTY;
  }

  return RET_OK;
}

void
wtls_cm_terminate (void *connptr, SDL_Boolean established)
{
  wtls_connection_t *conn = (wtls_connection_t *)connptr;

  if (established == SDL_True) {
    CRYPTa_connectionTerminated (conn->server_addr.ViewID,
                                 conn->server_addr.ChannelID);
    CRYPTa_sessionActive (conn->write.master_secret_id, 0);
  }

  wtls_connection_free (conn);
}

/************************************************************
 * Local functions
 ************************************************************/

static INT16
wtls_cm_check_client_key_id_index (wtls_connection_t *conn,
                                   wtls_client_hello *ch,
                                   wtls_server_hello *sh)
{
  INT16                 i;
  wtls_key_exchange_id *key_exchange_id = &(conn->h_state->key_exch);
  wap_cvt_t             cvt_obj;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, ch->client_key_ids,
                ch->client_key_id_len);

  if (sh->client_key_id == 0) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_KEY_EXCHANGE_IDX,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
    return -1;
  }

  for (i = sh->client_key_id; i > 0; i--) {
    if (!wtls_cvt_key_exchange_id (&cvt_obj, key_exchange_id)) {
      wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_KEY_EXCHANGE_DECODE,
                    1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
      return -1;
    }
  }

  return RET_OK;
}

static INT16
wtls_cm_check_cipher_suite (wtls_connection_t *conn,
                            wtls_client_hello *ch,
                            wtls_server_hello *sh)
{
  INT16 i;

  for (i = 0; i < ch->cipher_suites_len; i += 2) {
    if ((ch->cipher_suites[i] == sh->cipher_suite.bulk_cipher_alg)
        && (ch->cipher_suites[i + 1] == sh->cipher_suite.mac_alg))
      break;
  }
  if (i >= ch->cipher_suites_len) {
    wtls_err_set (ERR_HANDSHAKE_FAILURE, ERR_CIPHER_SUITE,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_ILLEGAL_PARAMETER);
    return -1;
  }
  conn->h_state->pending.bulk_cipher_alg = sh->cipher_suite.bulk_cipher_alg;
  conn->h_state->pending.mac_alg = sh->cipher_suite.mac_alg;
    
  return RET_OK;
}


#define NUM_MAC_METHODS 3

static INT16
wtls_cm_make_cipher_suites (INT8 bearer,
                            BYTE *methods, UINT16 methodslen,
                            BYTE **cs, UINT8 *cslen)
{
  BYTE  *p;
  UINT8 SHA_methods[NUM_MAC_METHODS];
  UINT8 MD5_methods[NUM_MAC_METHODS];
  INT16 i, j;
  BYTE  *cipher_suites;

  /*
   * Our current policy: for each encryption algorithm make cipher
   * suites with MAC-algorithms in an order that depends on
   * the bearer. The faster the bearer, the longer the MAC.
   */
  cipher_suites = NEWARRAY (BYTE, methodslen * NUM_MAC_METHODS);
  if (cipher_suites == NULL) {
    return -1;
  }

  switch (bearer) {
  case BEARER_ANY_UDP:
  case BEARER_GSM_CSD:
  case BEARER_GSM_GPRS:
  case BEARER_BT:
    SHA_methods[0] = WTLS_MAC(SHA, UDP, 1);
    SHA_methods[1] = WTLS_MAC(SHA, UDP, 2);
    SHA_methods[2] = WTLS_MAC(SHA, UDP, 3);

    MD5_methods[0] = WTLS_MAC(MD5, UDP, 1);
    MD5_methods[1] = WTLS_MAC(MD5, UDP, 2);
    MD5_methods[2] = WTLS_MAC(MD5, UDP, 3);
    break;

  case BEARER_GSM_USSD:
    SHA_methods[0] = WTLS_MAC(SHA, USSD, 1);
    SHA_methods[1] = WTLS_MAC(SHA, USSD, 2);
    SHA_methods[2] = WTLS_MAC(SHA, USSD, 3);

    MD5_methods[0] = WTLS_MAC(MD5, USSD, 1);
    MD5_methods[1] = WTLS_MAC(MD5, USSD, 2);
    MD5_methods[2] = WTLS_MAC(MD5, USSD, 3);
    break;

  case BEARER_GSM_SMS:
  default:
    SHA_methods[0] = WTLS_MAC(SHA, SMS, 1);
    SHA_methods[1] = WTLS_MAC(SHA, SMS, 2);
    SHA_methods[2] = WTLS_MAC(SHA, SMS, 3);

    MD5_methods[0] = WTLS_MAC(MD5, SMS, 1);
    MD5_methods[1] = WTLS_MAC(MD5, SMS, 2);
    MD5_methods[2] = WTLS_MAC(MD5, SMS, 3);
    break;
  }

  p = cipher_suites;
  for (i = 0; i < methodslen; i += 2) {
    if (methods[i + 1] == HASH_SHA) {
      for (j = 0; j < NUM_MAC_METHODS; j++) {
        p[j * 2] = methods[i];
        p[j * 2 + 1] = SHA_methods[j];
      }
      p += (NUM_MAC_METHODS * 2);
    }
    else if (methods[i + 1] == HASH_MD5) {
      for (j = 0; j < NUM_MAC_METHODS; j++) {
        p[j * 2] = methods[i];
        p[j * 2 + 1] = MD5_methods[j];
      }
      p += (NUM_MAC_METHODS * 2);
    }
  }

  *cs = cipher_suites;
  *cslen = MIN ((p - cipher_suites), 254);;

  return RET_OK;
}

/*
 * Fetch values for connection parameters.
 *
 * When starting a new connection, certain parameters have to
 * be given values. These values may depend on what type
 * of bearer the connection uses. The different parameters are:
 *
 * Max number of retransmissions:
 * The maximum number of times we are willing to resend a handshake
 * message without getting an answer.
 *
 * Retry interval:
 * The maximum time to wait for an answer to a handshake message,
 * before sending it again. Time unit is 1/10 second.
 *
 * Timeout interval:
 * A connection is closed down if no messages are exchanged during
 * a time interval of this size. Time unit is 1/10 second.
 */
void
wtls_cm_get_connection_parameters (SDL_Octet bearer,
                                   SDL_Integer *maxRetransmissions,
                                   SDL_Integer *retryInterval)
{
  switch (bearer) {
  case BEARER_ANY_UDP:
  case BEARER_GSM_CSD:
  case BEARER_GSM_GPRS:
  case BEARER_BT:
    *maxRetransmissions = WTLS_MAX_RETRANSMISSIONS_UDP;
    *retryInterval = WTLS_RETRANSMISSION_INTERVAL_UDP;
    break;

  case BEARER_GSM_USSD:
    *maxRetransmissions = WTLS_MAX_RETRANSMISSIONS_USSD;
    *retryInterval = WTLS_RETRANSMISSION_INTERVAL_USSD;
    break;

  case BEARER_GSM_SMS:
    *maxRetransmissions = WTLS_MAX_RETRANSMISSIONS_SMS;
    *retryInterval = WTLS_RETRANSMISSION_INTERVAL_SMS;
    break;
  }
}

/*
 * Key refresh rate:
 * How often do we want to refresh the encryption keys?
 * A value n here means that we refresh the keys every 2^n messages.
 * For example, if n = 3, we refresh after 8, 16, 24, etc. messages.
 * The slower the bearer, the more often we do key refresh; the goal
 * being to have the same time interval between key refreshes for
 * all bearers.
 */
void
wtls_cm_set_key_refresh_rate (UINT8 bearer, UINT8 *key_refresh_rate)
{
  switch (bearer) {
  case BEARER_ANY_UDP:
  case BEARER_GSM_CSD:
  case BEARER_GSM_GPRS:
  case BEARER_BT:
    *key_refresh_rate = WTLS_KEY_REFRESH_RATE_UDP;
    break;

  case BEARER_GSM_USSD:
    *key_refresh_rate = WTLS_KEY_REFRESH_RATE_USSD;
    break;

  case BEARER_GSM_SMS:
    *key_refresh_rate = WTLS_KEY_REFRESH_RATE_SMS;
    break;
  }
}

static void
wtls_cm_mkaddr (AddressType *inaddr, BYTE *outaddr)
{
  switch (inaddr->Bearer) {
  case BEARER_ANY_UDP:
  case BEARER_GSM_CSD:
  case BEARER_GSM_GPRS:
  case BEARER_BT:
    outaddr[0] = BEARER_ANY_UDP;
    break;

  case BEARER_GSM_USSD:
    outaddr[0] = BEARER_GSM_USSD;
    break;

  case BEARER_GSM_SMS:
    outaddr[0] = BEARER_GSM_SMS;
    break;

  default:
    outaddr[0] = BEARER_ANY_UDP;
    break;
  }
  outaddr[1] = inaddr->Address.length;
  memcpy (outaddr + 2, inaddr->Address.data, inaddr->Address.length);
}
