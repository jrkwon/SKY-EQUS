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
#include "wtlsdef.h"

/************************************************************
 * Conversion functions
 *
 * All decoding and encoding functions take a pointer to a
 * conversion object as the first argument. Usually, the second
 * argument is a pointer to an element of the type to be
 * converted.
 *
 * There are six different conversion operations:
 *   CVT_DECODE         convert from external to internal representation
 *   CVT_DECODE_STATIC  as above, but don't allocate any memory
 *   CVT_ENCODE         convert from internal to external representation
 *   CVT_DECODE_SIZE    calculate the buffer size that would be used
 *                        if one were to do a decode operation
 *   CVT_ENCODE_SIZE    calculate the buffer size required to do an
 *                        encoding operation
 *   CVT_FREE           deallocate any vectors that were allocated as
 *                        part of a previous decoding operation
 *
 * Each function returns an integer value != 0 on error,
 * and 0 otherwise.
 *
 ************************************************************/

/************************************************************
 * Local routines
 ************************************************************/

/*
 * Get and return one byte from the stream.
 */
static BYTE
stream_fetch_byte (wtls_cvt_t *obj)
{
  BYTE tmp = obj->data[obj->pos];

  obj->pos += 1;

  return tmp;
}

/*
 * Place one byte into the stream.
 */
#define STREAM_PUT_BYTE(obj, b) do { (obj)->data[(obj)->pos] = (b); \
                                      (obj)->pos += 1; } while (0)

/*
 * Return the number of bytes remaining in the buffer.
 */
#define STREAM_REMAINING_LENGTH(str) ((str)->length - (str)->pos)


/************************************************************
 * Global routines
 ************************************************************/

/*
 * Prepare a conversion object for a specific operation,
 * using the indicated buffer of given length.
 */
void
wtls_cvt_init (wtls_cvt_t *obj, UINT8 operation, BYTE *buf, UINT16 length)
{
  obj->operation = operation;
  obj->data = buf;
  obj->length = length;
  obj->pos = 0;
}

/*
 * Convert a 1-byte unsigned integer.
 */
INT16
wtls_cvt_uint8 (wtls_cvt_t *obj, UINT8 *p)
{
  switch (obj->operation) {
  case CVT_DECODE:
  case CVT_DECODE_SIZE:
  case CVT_DECODE_STATIC:
    if (STREAM_REMAINING_LENGTH (obj) < 1) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    *p = stream_fetch_byte (obj);
    break;

  case CVT_ENCODE:
    if (STREAM_REMAINING_LENGTH (obj) < 1) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    STREAM_PUT_BYTE (obj, *p);
    break;

  case CVT_ENCODE_SIZE:
    obj->pos += 1;
    break;

  case CVT_FREE:
    break;

  default:
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  return 0;
}


/*
 * Convert a 2-byte unsigned integer in big-endian format.
 */
INT16
wtls_cvt_uint16 (wtls_cvt_t *obj, UINT16 *p)
{
  BYTE a;

  switch (obj->operation) {
  case CVT_DECODE:
  case CVT_DECODE_SIZE:
  case CVT_DECODE_STATIC:
    if (STREAM_REMAINING_LENGTH (obj) < 2) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    a = stream_fetch_byte (obj);
    *p = (a << 8) | stream_fetch_byte (obj);
    break;

  case CVT_ENCODE:
    if (STREAM_REMAINING_LENGTH (obj) < 2) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    STREAM_PUT_BYTE (obj, (UINT8)((*p >> 8) & 0xff));
    STREAM_PUT_BYTE (obj, (UINT8)(*p & 0xff));
    break;

  case CVT_ENCODE_SIZE:
    obj->pos += 2;
    break;

  case CVT_FREE:
    break;

  default:
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  return 0;
}


/*
 * Convert a 4-byte unsigned format in big-endian format.
 */
INT16
wtls_cvt_uint32 (wtls_cvt_t *obj, UINT32 *p)
{
  BYTE a, b, c;

  switch (obj->operation) {
  case CVT_DECODE:
  case CVT_DECODE_SIZE:
  case CVT_DECODE_STATIC:
    if (STREAM_REMAINING_LENGTH (obj) < 4) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    a = stream_fetch_byte (obj);
    b = stream_fetch_byte (obj);
    c = stream_fetch_byte (obj);
    *p = (a << 24) | (b << 16) | (c << 8) | stream_fetch_byte (obj);
    break;

  case CVT_ENCODE:
    if (STREAM_REMAINING_LENGTH (obj) < 4) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    STREAM_PUT_BYTE (obj, (UINT8)((*p >> 24) & 0xff));
    STREAM_PUT_BYTE (obj, (UINT8)((*p >> 16) & 0xff));
    STREAM_PUT_BYTE (obj, (UINT8)((*p >> 8) & 0xff));
    STREAM_PUT_BYTE (obj, (UINT8)(*p & 0xff));
    break;

  case CVT_ENCODE_SIZE:
    obj->pos += 4;
    break;

  case CVT_FREE:
    break;

  default:
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  return 0;
}

/*
 * Convert a static bytevector, i.e., a vector whose internal
 * representation is a static vector (e.g., BYTE v[12];).
 */
INT16
wtls_cvt_static_bytevector (wtls_cvt_t *obj, UINT16 length, BYTE *p)
{
  switch (obj->operation) {
  case CVT_DECODE:
  case CVT_DECODE_STATIC:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    B_COPYSTRINGN (p, obj->data + obj->pos, length);
    obj->pos += length;
    break;

  case CVT_ENCODE:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    B_COPYSTRINGN (obj->data + obj->pos, p, length);
    obj->pos += length;
    break;

  case CVT_DECODE_SIZE:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    obj->pos += length;
    break;

  case CVT_ENCODE_SIZE:
    obj->pos += length;
    break;

  case CVT_FREE:
    break;

  default:
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  return 0;
}

/*
 * Convert a byte vector, i.e., a vector whose internal representation
 * is a pointer to a byte (BYTE *v;).
 */
INT16
wtls_cvt_bytevector (wtls_cvt_t *obj, UINT16 length, BYTE **p)
{
  switch (obj->operation) {
  case CVT_DECODE:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      *p = NULL;
      return -1;
    }
    *p = NEWARRAY (BYTE, length);
    B_COPYSTRINGN (*p, obj->data + obj->pos, length);
    obj->pos += length;
    break;

  case CVT_DECODE_STATIC:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      *p = NULL;
      return -1;
    }
    *p = obj->data + obj->pos;
    obj->pos += length;
    break;

  case CVT_ENCODE:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
      return -1;
    }
    B_COPYSTRINGN (obj->data + obj->pos, *p, length);
    obj->pos += length;
    break;

  case CVT_DECODE_SIZE:
    if (STREAM_REMAINING_LENGTH (obj) < length) {
      wtls_err_set (ERR_GENERAL, ERR_DECODING, 1,
                    ALERT_LEVEL_CRITICAL, ALERT_DESC_DECODE_ERROR);
      return -1;
    }
    obj->pos += length;
    break;

  case CVT_ENCODE_SIZE:
    obj->pos += length;
    break;

  case CVT_FREE:
    DEALLOC (p);
    break;

  default:
    wtls_err_set (ERR_INTERNAL, ERR_ENCODING, 1,
                  ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_record_t.
 */
INT16
wtls_cvt_record (wtls_cvt_t *obj, wtls_record_t *p)
{
  if (wtls_cvt_uint8 (obj, &(p->rec_type))) {
    return -1;
  }
  if ((p->rec_type & RECTYPE_SEQNUM) &&
      wtls_cvt_uint16 (obj, &(p->seqnum))) {
    return -1;
  }
  if (p->rec_type & RECTYPE_LENGTH_FIELD) {
    if (wtls_cvt_uint16 (obj, &(p->length))) {
      return -1;
    }
  }
  else if ((obj->operation == CVT_DECODE) ||
           (obj->operation == CVT_DECODE_SIZE) ||
           (obj->operation == CVT_DECODE_STATIC)) {
    p->length = obj->length - obj->pos;
  }
  if (wtls_cvt_bytevector (obj, p->length, &(p->fragment))) {
    return -1;
  }
  
  return 0;
}

/*
 * Convert an element of the type wtls_alert_t.
 */
INT16
wtls_cvt_alert (wtls_cvt_t *obj, wtls_alert_t *p)
{
  if (wtls_cvt_uint8 (obj, &(p->level)) ||
      wtls_cvt_uint8 (obj, &(p->description)) ||
      wtls_cvt_uint32 (obj, &(p->checksum))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_session_id.
 */
INT16
wtls_cvt_session_id (wtls_cvt_t *obj, wtls_session_id *p)
{
  if (wtls_cvt_uint8 (obj, &(p->length)) ||
      wtls_cvt_static_bytevector (obj, p->length, p->id)) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_cipher_suite.
 */
INT16
wtls_cvt_cipher_suite (wtls_cvt_t *obj, wtls_cipher_suite *p)
{
  if (wtls_cvt_uint8 (obj, &(p->bulk_cipher_alg)) ||
      wtls_cvt_uint8 (obj, &(p->mac_alg))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_client_hello.
 */
INT16
wtls_cvt_client_hello (wtls_cvt_t *obj, wtls_client_hello *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint8 (obj, &(p->client_version)) ||
      wtls_cvt_static_bytevector (obj, 16, p->random) ||
      wtls_cvt_session_id (obj, &(p->session_id)) ||
      wtls_cvt_uint16 (obj, &(p->client_key_id_len)) ||
      wtls_cvt_bytevector (obj, p->client_key_id_len,
                           &(p->client_key_ids)) ||
      wtls_cvt_uint16 (obj, &(p->trusted_key_id_len)) ||
      wtls_cvt_bytevector (obj, p->trusted_key_id_len,
                           &(p->trusted_key_ids)) ||
      wtls_cvt_uint8 (obj, &(p->cipher_suites_len)) ||
      wtls_cvt_bytevector (obj, p->cipher_suites_len,
                           &(p->cipher_suites)) ||
      wtls_cvt_uint8(obj, &(p->num_compression_methods)) ||
      wtls_cvt_bytevector (obj, p->num_compression_methods,
                           &(p->compression_methods)) ||
      wtls_cvt_uint8 (obj, &(p->seqnum_mode)) ||
      wtls_cvt_uint8 (obj, &(p->key_refresh_rate))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_server_hello.
 */
INT16
wtls_cvt_server_hello (wtls_cvt_t *obj, wtls_server_hello *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint8 (obj, &(p->server_version)) ||
      wtls_cvt_static_bytevector (obj, 16, p->random) ||
      wtls_cvt_session_id (obj, &(p->session_id)) ||
      wtls_cvt_uint8 (obj, &(p->client_key_id)) ||
      wtls_cvt_cipher_suite (obj, &(p->cipher_suite)) ||
      wtls_cvt_uint8 (obj, &(p->compression_method)) ||
      wtls_cvt_uint8 (obj, &(p->seqnum_mode)) ||
      wtls_cvt_uint8 (obj, &(p->key_refresh_rate))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_server_certificate.
 */
INT16
wtls_cvt_server_certificate (wtls_cvt_t *obj,
                             wtls_server_certificate *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint16 (obj, &(p->buflen)) ||
      wtls_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_server_key_exchange.
 */
INT16
wtls_cvt_server_key_exchange (wtls_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_server_key_exchange *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_parameter_specifier (obj, &(p->parameter_specifier)) ||
      wtls_cvt_public_key (obj, kes, &(p->public_key))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_certificate_request.
 */
INT16
wtls_cvt_certificate_request (wtls_cvt_t *obj,
                              wtls_certificate_request *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint16 (obj, &(p->buflen)) ||
      wtls_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_server_hello_done.
 */
INT16
wtls_cvt_server_hello_done (wtls_cvt_t *obj, wtls_server_hello_done *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_client_certificate.
 */
INT16
wtls_cvt_client_certificate (wtls_cvt_t *obj, wtls_client_certificate *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint16 (obj, &(p->buflen)) ||
      wtls_cvt_bytevector (obj, p->buflen, &(p->buf))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_client_key_exchange.
 */
INT16
wtls_cvt_client_key_exchange (wtls_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_client_key_exchange *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length))) {
    return -1;
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
    if (wtls_cvt_uint16 (obj, &(p->buflen.len16)) ||
        wtls_cvt_bytevector (obj, p->buflen.len16, &(p->buf))) {
      return -1;
    }
    break;

  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
  case KEY_EXCH_ECDH_ECDSA:
    if (wtls_cvt_uint8 (obj, &(p->buflen.len8)) ||
        wtls_cvt_bytevector (obj, p->buflen.len8, &(p->buf))) {
      return -1;
    }
    break;

  default:
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_certificate_verify.
 */
INT16
wtls_cvt_certificate_verify (wtls_cvt_t *obj, wtls_certificate_verify *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_uint16 (obj, &(p->siglen)) ||
      wtls_cvt_bytevector (obj, p->siglen, &(p->signature))) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_finished.
 */
INT16
wtls_cvt_finished (wtls_cvt_t *obj, wtls_finished *p)
{
  if (wtls_cvt_uint8 (obj, &(p->msg_type)) ||
      wtls_cvt_uint16 (obj, &(p->length)) ||
      wtls_cvt_static_bytevector (obj, 12, p->verify_data)) {
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type wtls_identifier_t.
 */
INT16
wtls_cvt_identifier (wtls_cvt_t *obj, wtls_identifier_t *p)
{
  if (wtls_cvt_uint8 (obj, &(p->identifier_type))) {
    return -1;
  }
  switch (p->identifier_type) {
  case IDENTIFIER_NULL:
    break;

  case IDENTIFIER_TEXT:
    if (wtls_cvt_uint16 (obj, &(p->_u.text.character_set)) ||
        wtls_cvt_uint8 (obj, &(p->_u.text.namelen)) ||
        wtls_cvt_bytevector (obj, p->_u.text.namelen,
                             &(p->_u.text.name))) {
      return -1;
    }
    break;

  case IDENTIFIER_BINARY:
    if (wtls_cvt_uint8 (obj, &(p->_u.binary.idlen)) ||
        wtls_cvt_bytevector (obj, p->_u.binary.idlen,
                             &(p->_u.binary.identifier))) {
      return -1;
    }
    break;

  case IDENTIFIER_KEY_HASH_SHA:
    if (wtls_cvt_bytevector (obj, 20, &(p->_u.key_hash_SHA.key_hash))) {
      return -1;
    }
    break;

  case IDENTIFIER_X509_NAME:
    if (wtls_cvt_uint8 (obj, &(p->_u.x509_name.namelen)) ||
        wtls_cvt_bytevector (obj, p->_u.x509_name.namelen,
                             &(p->_u.x509_name.distinguished_name))) {
      return -1;
    }
    break;

  default:
    return -1;
  }

  return 0;
}

/*
 * Convert an element of the type PublicKey.
 */
INT16
wtls_cvt_public_key (wtls_cvt_t *obj,
                     KeyExchangeSuite kes, PublicKey *p)
{
  switch (kes) {
  case KEY_EXCH_DH_ANON:
  case KEY_EXCH_DH_ANON_512:
  case KEY_EXCH_DH_ANON_768:
    if (wtls_cvt_uint16 (obj, &(p->_u.dh_key.len)) ||
        wtls_cvt_bytevector (obj, p->_u.dh_key.len,
                             &(p->_u.dh_key.y))) {
      return -1;
    }
    break;

  case KEY_EXCH_RSA_ANON:
  case KEY_EXCH_RSA_ANON_512:
  case KEY_EXCH_RSA_ANON_768:
  case KEY_EXCH_RSA:
  case KEY_EXCH_RSA_512:
  case KEY_EXCH_RSA_768:
    if (wtls_cvt_uint16 (obj, &(p->_u.rsa_key.expLen)) ||
        wtls_cvt_bytevector (obj, p->_u.rsa_key.expLen,
                             &(p->_u.rsa_key.exponent)) ||
        wtls_cvt_uint16 (obj, &(p->_u.rsa_key.modLen)) ||
        wtls_cvt_bytevector (obj, p->_u.rsa_key.modLen,
                             &(p->_u.rsa_key.modulus))) {
      return -1;
    }
    break;

  case KEY_EXCH_ECDH_ANON:
  case KEY_EXCH_ECDH_ANON_113:
  case KEY_EXCH_ECDH_ANON_131:
    if (wtls_cvt_uint8 (obj, &(p->_u.ec_key.len)) ||
        wtls_cvt_bytevector (obj, p->_u.ec_key.len,
                             &(p->_u.ec_key.point))) {
      return -1;
    }
    break;

  default:
    return -1;
  }
  
  return 0;
}

/*
 * Convert an element of the type ParameterSpecifier.
 */
INT16
wtls_cvt_parameter_specifier (wtls_cvt_t *obj, ParameterSpecifier *p)
{
  if (wtls_cvt_uint8 (obj, &(p->parameterIndex))) {
    return -1;
  }
  if (p->parameterIndex == 255) {
    if (wtls_cvt_uint16 (obj, &(p->paramLen)) ||
        wtls_cvt_bytevector (obj, p->paramLen, &(p->params))) {
      return -1;
    }
  }

  return 0;
}

/*
 * Convert an element of the type wtls_key_exchange_id.
 */
INT16
wtls_cvt_key_exchange_id (wtls_cvt_t *obj, wtls_key_exchange_id *p)
{
  if (wtls_cvt_uint8 (obj, &(p->key_exchange_suite)) ||
      wtls_cvt_parameter_specifier (obj, &(p->parameter_specifier)) ||
      wtls_cvt_identifier (obj, &(p->identifier))) {
    return -1;
  }

  return 0;
}
