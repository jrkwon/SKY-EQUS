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
 * convert.c
 *
 * Created by Anders Edenbrandt, Wed Oct 06 14:42:51 1999.
 *
 *
 * Revision history:
 *
 */
#include "wapcvt.h"
#include "wtlsdef.h"


/************************************************************
 * Global routines
 ************************************************************/

/*
 * Convert an element of the type wtls_record_t.
 */
INT16
wtls_cvt_record (wap_cvt_t *obj, wtls_record_t *p)
{
  if (!wap_cvt_uint8 (obj, &(p->rec_type))) {
    return FALSE;
  }
  if ((p->rec_type & RECTYPE_SEQNUM) &&
      !wap_cvt_uint16 (obj, &(p->seqnum))) {
    return FALSE;
  }
  if (p->rec_type & RECTYPE_LENGTH_FIELD) {
    if (!wap_cvt_uint16 (obj, &(p->length))) {
      return FALSE;
    }
  }
  else if ((obj->operation == WAP_CVT_DECODE) ||
           (obj->operation == WAP_CVT_DECODE_STATIC)) {
    p->length = (UINT16)(obj->length - obj->pos);
  }
  if (!wap_cvt_bytevector (obj, p->length, &(p->fragment))) {
    return FALSE;
  }
  
  return TRUE;
}

/*
 * Convert an element of the type wtls_alert_t.
 */
INT16
wtls_cvt_alert (wap_cvt_t *obj, wtls_alert_t *p)
{
  if (!wap_cvt_uint8 (obj, &(p->level)) ||
      !wap_cvt_uint8 (obj, &(p->description)) ||
      !wap_cvt_uint32 (obj, &(p->checksum))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_session_id.
 */
INT16
wtls_cvt_session_id (wap_cvt_t *obj, wtls_session_id *p)
{
  if (!wap_cvt_uint8 (obj, &(p->length)) ||
      !wap_cvt_static_bytevector (obj, p->length, p->id)) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_cipher_suite.
 */
INT16
wtls_cvt_cipher_suite (wap_cvt_t *obj, wtls_cipher_suite *p)
{
  if (!wap_cvt_uint8 (obj, &(p->bulk_cipher_alg)) ||
      !wap_cvt_uint8 (obj, &(p->mac_alg))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_client_hello.
 */
INT16
wtls_cvt_client_hello (wap_cvt_t *obj, wtls_client_hello *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint8 (obj, &(p->client_version)) ||
      !wap_cvt_static_bytevector (obj, 16, p->random) ||
      !wtls_cvt_session_id (obj, &(p->session_id)) ||
      !wap_cvt_uint16 (obj, &(p->client_key_id_len)) ||
      !wap_cvt_bytevector (obj, p->client_key_id_len,
                           &(p->client_key_ids)) ||
      !wap_cvt_uint16 (obj, &(p->trusted_key_id_len)) ||
      !wap_cvt_bytevector (obj, p->trusted_key_id_len,
                           &(p->trusted_key_ids)) ||
      !wap_cvt_uint8 (obj, &(p->cipher_suites_len)) ||
      !wap_cvt_bytevector (obj, p->cipher_suites_len,
                           &(p->cipher_suites)) ||
      !wap_cvt_uint8(obj, &(p->num_compression_methods)) ||
      !wap_cvt_bytevector (obj, p->num_compression_methods,
                           &(p->compression_methods)) ||
      !wap_cvt_uint8 (obj, &(p->seqnum_mode)) ||
      !wap_cvt_uint8 (obj, &(p->key_refresh_rate))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_server_hello.
 */
INT16
wtls_cvt_server_hello (wap_cvt_t *obj, wtls_server_hello *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint8 (obj, &(p->server_version)) ||
      !wap_cvt_static_bytevector (obj, 16, p->random) ||
      !wtls_cvt_session_id (obj, &(p->session_id)) ||
      !wap_cvt_uint8 (obj, &(p->client_key_id)) ||
      !wtls_cvt_cipher_suite (obj, &(p->cipher_suite)) ||
      !wap_cvt_uint8 (obj, &(p->compression_method)) ||
      !wap_cvt_uint8 (obj, &(p->seqnum_mode)) ||
      !wap_cvt_uint8 (obj, &(p->key_refresh_rate))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_server_certificate.
 */
INT16
wtls_cvt_server_certificate (wap_cvt_t *obj,
                             wtls_server_certificate *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint16 (obj, &(p->buflen)) ||
      !wap_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_server_key_exchange.
 */
INT16
wtls_cvt_server_key_exchange (wap_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_server_key_exchange *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wtls_cvt_parameter_specifier (obj, &(p->parameter_specifier)) ||
      !wtls_cvt_public_key (obj, kes, &(p->public_key))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_certificate_request.
 */
INT16
wtls_cvt_certificate_request (wap_cvt_t *obj,
                              wtls_certificate_request *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint16 (obj, &(p->buflen)) ||
      !wap_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_server_hello_done.
 */
INT16
wtls_cvt_server_hello_done (wap_cvt_t *obj, wtls_server_hello_done *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_client_certificate.
 */
INT16
wtls_cvt_client_certificate (wap_cvt_t *obj, wtls_client_certificate *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint16 (obj, &(p->buflen)) ||
      !wap_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_client_key_exchange.
 */
INT16
wtls_cvt_client_key_exchange (wap_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_client_key_exchange *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length))) {
    return FALSE;
  }

  switch (kes) {
  case KEY_EXCH_DH_ANON:
  case KEY_EXCH_DH_ANON_512:
  case KEY_EXCH_DH_ANON_768:
  case KEY_EXCH_RSA_ANON:
  case KEY_EXCH_RSA_ANON_512:
  case KEY_EXCH_RSA_ANON_768:
  case KEY_EXCH_RSA:
  case KEY_EXCH_RSA_512:
  case KEY_EXCH_RSA_768:
    if (!wap_cvt_uint16 (obj, &(p->buflen.len16)) ||
        !wap_cvt_bytevector (obj, p->buflen.len16, &(p->buf))) {
      return FALSE;
    }
    break;

  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
  case KEY_EXCH_ECDH_ECDSA:
    if (!wap_cvt_uint8 (obj, &(p->buflen.len8)) ||
        !wap_cvt_bytevector (obj, p->buflen.len8, &(p->buf))) {
      return FALSE;
    }
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_certificate_verify.
 */
INT16
wtls_cvt_certificate_verify (wap_cvt_t *obj, wtls_certificate_verify *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_uint16 (obj, &(p->siglen)) ||
      !wap_cvt_bytevector (obj, p->siglen, &(p->signature))) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_finished.
 */
INT16
wtls_cvt_finished (wap_cvt_t *obj, wtls_finished *p)
{
  if (!wap_cvt_uint8 (obj, &(p->msg_type)) ||
      !wap_cvt_uint16 (obj, &(p->length)) ||
      !wap_cvt_static_bytevector (obj, 12, p->verify_data)) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_identifier_t.
 */
INT16
wtls_cvt_identifier (wap_cvt_t *obj, wtls_identifier_t *p)
{
  if (!wap_cvt_uint8 (obj, &(p->identifier_type))) {
    return FALSE;
  }
  switch (p->identifier_type) {
  case IDENTIFIER_NULL:
    break;

  case IDENTIFIER_TEXT:
    if (!wap_cvt_uint16 (obj, &(p->_u.text.character_set)) ||
        !wap_cvt_uint8 (obj, &(p->_u.text.namelen)) ||
        !wap_cvt_bytevector (obj, p->_u.text.namelen,
                             &(p->_u.text.name))) {
      return FALSE;
    }
    break;

  case IDENTIFIER_BINARY:
    if (!wap_cvt_uint8 (obj, &(p->_u.binary.idlen)) ||
        !wap_cvt_bytevector (obj, p->_u.binary.idlen,
                             &(p->_u.binary.identifier))) {
      return FALSE;
    }
    break;

  case IDENTIFIER_KEY_HASH_SHA:
    if (!wap_cvt_bytevector (obj, 20, &(p->_u.key_hash_SHA.key_hash))) {
      return FALSE;
    }
    break;

  case IDENTIFIER_X509_NAME:
    if (!wap_cvt_uint8 (obj, &(p->_u.x509_name.namelen)) ||
        !wap_cvt_bytevector (obj, p->_u.x509_name.namelen,
                             &(p->_u.x509_name.distinguished_name))) {
      return FALSE;
    }
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

/*
 * Convert an element of the type PublicKey.
 */
INT16
wtls_cvt_public_key (wap_cvt_t *obj,
                     KeyExchangeSuite kes, PublicKey *p)
{
  switch (kes) {
  case KEY_EXCH_DH_ANON:
  case KEY_EXCH_DH_ANON_512:
  case KEY_EXCH_DH_ANON_768:
    if (!wap_cvt_uint16 (obj, &(p->_u.dh_key.len)) ||
        !wap_cvt_bytevector (obj, p->_u.dh_key.len,
                             &(p->_u.dh_key.y))) {
      return FALSE;
    }
    break;

  case KEY_EXCH_RSA_ANON:
  case KEY_EXCH_RSA_ANON_512:
  case KEY_EXCH_RSA_ANON_768:
  case KEY_EXCH_RSA:
  case KEY_EXCH_RSA_512:
  case KEY_EXCH_RSA_768:
    if (!wap_cvt_uint16 (obj, &(p->_u.rsa_key.expLen)) ||
        !wap_cvt_bytevector (obj, p->_u.rsa_key.expLen,
                             &(p->_u.rsa_key.exponent)) ||
        !wap_cvt_uint16 (obj, &(p->_u.rsa_key.modLen)) ||
        !wap_cvt_bytevector (obj, p->_u.rsa_key.modLen,
                             &(p->_u.rsa_key.modulus))) {
      return FALSE;
    }
    break;

  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
    if (!wap_cvt_uint8 (obj, &(p->_u.ec_key.len)) ||
        !wap_cvt_bytevector (obj, p->_u.ec_key.len,
                             &(p->_u.ec_key.point))) {
      return FALSE;
    }
    break;

  default:
    return FALSE;
  }
  
  return TRUE;
}

/*
 * Convert an element of the type ParameterSpecifier.
 */
INT16
wtls_cvt_parameter_specifier (wap_cvt_t *obj, ParameterSpecifier *p)
{
  if (!wap_cvt_uint8 (obj, &(p->parameterIndex))) {
    return FALSE;
  }
  if (p->parameterIndex == 255) {
    if (!wap_cvt_uint16 (obj, &(p->paramLen)) ||
        !wap_cvt_bytevector (obj, p->paramLen, &(p->params))) {
      return FALSE;
    }
  }

  return TRUE;
}

/*
 * Convert an element of the type wtls_key_exchange_id.
 */
INT16
wtls_cvt_key_exchange_id (wap_cvt_t *obj, wtls_key_exchange_id *p)
{
  if (!wap_cvt_uint8 (obj, &(p->key_exchange_suite)) ||
      !wtls_cvt_parameter_specifier (obj, &(p->parameter_specifier)) ||
      !wtls_cvt_identifier (obj, &(p->identifier))) {
    return FALSE;
  }

  return TRUE;
}
