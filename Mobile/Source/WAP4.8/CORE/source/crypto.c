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
 * Crypto.c
 *
 * Created by Anders Edenbrandt, Fri Jun 02 07:59:31 2000.
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"
#ifdef LOG_EXTERNAL
#include "aapiclnt.h"
#include "logcodes.h"
#endif

/* Information below is from WTLS spec. Appendix A */

#define NUM_CIPHER_ALGS  12

static const struct {
  UINT8 bulk_cipher_alg;
  UINT8 is_exportable;
  UINT8 type;
  UINT8 key_material_length;
  UINT8 key_size;
  UINT8 iv_size;
  UINT8 block_size;
} bulk_cipher_alg_info[NUM_CIPHER_ALGS] = {
  {CIPHER_NULL, TRUE, CIPHER_TYPE_STREAM, 0, 0, 0, 0},
  {CIPHER_RC5_CBC_40, TRUE, CIPHER_TYPE_BLOCK, 5, 16, 8, 8},
  {CIPHER_RC5_CBC_56, TRUE, CIPHER_TYPE_BLOCK, 7, 16, 8, 8},
  {CIPHER_RC5_CBC, FALSE, CIPHER_TYPE_BLOCK, 16, 16, 8, 8},
  {CIPHER_DES_CBC_40, TRUE, CIPHER_TYPE_BLOCK, 5, 8, 8, 8},
  {CIPHER_DES_CBC, FALSE, CIPHER_TYPE_BLOCK, 8, 8, 8, 8},
  {CIPHER_3DES_CBC_EDE, FALSE, CIPHER_TYPE_BLOCK, 24, 24, 8, 8},
  {CIPHER_IDEA_CBC_40, TRUE, CIPHER_TYPE_BLOCK, 5, 16, 8, 8},
  {CIPHER_IDEA_CBC_56, TRUE, CIPHER_TYPE_BLOCK, 7, 16, 8, 8},
  {CIPHER_IDEA_CBC, FALSE, CIPHER_TYPE_BLOCK, 16, 16, 8, 8},
  {CIPHER_RC5_CBC_64, TRUE, CIPHER_TYPE_BLOCK, 8, 16, 8, 8},
  {CIPHER_IDEA_CBC_64, TRUE, CIPHER_TYPE_BLOCK, 8, 16, 8, 8}
};

#define NUM_MAC_ALGS 8

static const struct {
  UINT8 mac_alg;
  UINT8 mac_key_size;
  UINT8 mac_size;
  UINT8 full_mac_size;
  UINT8 mac_block_size;
} MAC_alg_info[NUM_MAC_ALGS] = {
  {MAC_SHA_0, 0, 0, 20, 64},
  {MAC_SHA_40, 20, 5, 20, 64},
  {MAC_SHA_80, 20, 10, 20, 64},
  {MAC_SHA, 20, 20, 20, 64},
  {MAC_SHA_XOR_40, 0, 5, 20, 5},
  {MAC_MD5_40, 16, 5, 16, 64},
  {MAC_MD5_80, 16, 10, 16, 64},
  {MAC_MD5, 16, 16, 16, 64}
};


/*
 * For a given WTLS MAC algorithm, return the underlying
 * secure hash algorithm.
 */
HashAlgorithm
wtls_crypto_MAC_to_hash_alg (wtls_MAC_algorithm alg)
{
  switch (alg) {
  case MAC_SHA_0:
  case MAC_SHA_40:
  case MAC_SHA_80:
  case MAC_SHA:
  case MAC_SHA_XOR_40:
    return HASH_SHA;

  case MAC_MD5_40:
  case MAC_MD5_80:
  case MAC_MD5:
    return HASH_MD5;

  default:
    return 0;
  }
}

/*
 * Initialize a cryptographic object according to the given parameters.
 */
INT16
wtls_crypto_init (wtls_crypto_t *cobj,
                  BulkCipherAlgorithm bulk_cipher_alg,
                  wtls_MAC_algorithm mac_alg,
                  UINT8 compression_alg)
{
  /* First, the bulk encryption algorithm. */
  if (bulk_cipher_alg >= NUM_CIPHER_ALGS) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  /* Consistency check */
  if (bulk_cipher_alg !=
      bulk_cipher_alg_info[bulk_cipher_alg].bulk_cipher_alg) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  cobj->bulk_cipher_alg = bulk_cipher_alg;
  cobj->is_exportable = bulk_cipher_alg_info[bulk_cipher_alg].is_exportable;
  cobj->type = bulk_cipher_alg_info[bulk_cipher_alg].type;
  cobj->key_material_length =
    bulk_cipher_alg_info[bulk_cipher_alg].key_material_length;
  cobj->key_size = bulk_cipher_alg_info[bulk_cipher_alg].key_size;
  cobj->iv_size = bulk_cipher_alg_info[bulk_cipher_alg].iv_size;
  cobj->block_size = bulk_cipher_alg_info[bulk_cipher_alg].block_size;

  /* Next, the cryptographic hash algorithm. */
  if (mac_alg >= NUM_MAC_ALGS) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  /* Consistency check */
  if (mac_alg != MAC_alg_info[mac_alg].mac_alg) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  cobj->mac_alg = mac_alg;
  cobj->mac_key_size = MAC_alg_info[mac_alg].mac_key_size;
  cobj->mac_size = MAC_alg_info[mac_alg].mac_size;
  cobj->full_mac_size = MAC_alg_info[mac_alg].full_mac_size;
  cobj->mac_block_size = MAC_alg_info[mac_alg].mac_block_size;
  cobj->hash_handle = NULL;
  cobj->hash_alg = wtls_crypto_MAC_to_hash_alg (mac_alg);

  /* Finally, the compression algorithm. */
  if (compression_alg != COMPRESS_NULL) {
    wtls_err_set (ERR_INTERNAL, ERR_PROGRAMMING_ERROR,
                  1, ALERT_LEVEL_FATAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }
  cobj->compression_alg = COMPRESS_NULL;

  return RET_OK;
}

/*
 * Bulk encryption.
 */
INT16
wtls_crypto_encrypt (wtls_crypto_t *cobj,
                     BYTE *key, BYTE *iv,
                     BYTE *buf, UINT16 buflen,
                     BYTE *outbuf)
{
  KeyObject keyobj;
  INT16     r;

  keyobj.key = key;
  keyobj.keyLen = cobj->key_size;
  keyobj.iv = iv;
  keyobj.ivLen = cobj->iv_size;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_encrypt, "WTLS: calling CRYPTa_encrypt");
#endif
  if ((r = CRYPTa_encrypt (cobj->bulk_cipher_alg, keyobj,
                           buf, buflen, outbuf)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_ENCRYPT,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_encrypt,
               "WTLS: error return from CRYPTa_encrypt: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}

/*
 * Bulk decryption.
 */
INT16
wtls_crypto_decrypt (wtls_crypto_t *cobj,
                     BYTE *key, BYTE *iv,
                     BYTE *buf, UINT16 buflen,
                     BYTE *outbuf)
{
  KeyObject keyobj;
  INT16     r;

  keyobj.key = key;
  keyobj.keyLen = cobj->key_size;
  keyobj.iv = iv;
  keyobj.ivLen = cobj->iv_size;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_decrypt, "WTLS: calling CRYPTa_decrypt");
#endif
  if ((r = CRYPTa_decrypt (cobj->bulk_cipher_alg,
                           keyobj, buf, buflen,
                           outbuf)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_DECRYPT,
                  1, ALERT_LEVEL_WARNING, ALERT_DESC_DECRYPTION_FAILED);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_decrypt,
               "WTLS: error return from CRYPTa_decrypt: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}

/*
 * Secure hash algorithms.
 */
INT16
wtls_crypto_hash (wtls_crypto_t *cobj, BYTE *buf, UINT16 buflen, BYTE *digest)
{
  INT16  r;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_hash, "WTLS: calling CRYPTa_hash");
#endif
  if ((r = CRYPTa_hash (cobj->hash_alg,
                        buf, buflen, digest)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_HASH,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_hash,
               "WTLS: error return from CRYPTa_hash: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}

INT16
wtls_crypto_hash_init (wtls_crypto_t *cobj)
{
  INT16 r;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_hashInit, "WTLS: calling CRYPTa_hashInit");
#endif
  if ((r = CRYPTa_hashInit (cobj->hash_alg,
                            &(cobj->hash_handle))) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_HASH_INIT,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_hashInit,
               "WTLS: error return from CRYPTa_hashInit: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}

INT16
wtls_crypto_hash_update (wtls_crypto_t *cobj, BYTE *buf, UINT16 buflen)
{
  INT16 r;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_hashUpdate, "WTLS: calling CRYPTa_hashUpdate");
#endif
  if ((r = CRYPTa_hashUpdate (cobj->hash_handle, buf, buflen)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_HASH_UPDATE,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_hashUpdate,
               "WTLS: error return from CRYPTa_hashUpdate: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}

INT16
wtls_crypto_hash_final (wtls_crypto_t *cobj, BYTE *digest)
{
  INT16  r;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_hashFinal, "WTLS: calling CRYPTa_hashFinal");
#endif
  if ((r = CRYPTa_hashFinal (cobj->hash_handle, digest)) != CRV_OK) {
    wtls_err_set (ERR_CRYPTLIB, ERR_HASH_FINAL,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
#ifdef LOG_EXTERNAL
    CLNTa_log (0, log_wtls_CRYPTa_hashFinal,
               "WTLS: error return from CRYPTa_hashFinal: %d\n", r);
#endif
    return -1;
  }

  return RET_OK;
}


#if 0
/*
 * The NULL compression algorithm, which does not do any compression.
 * The WTLS spec requires that we support the NULL compression algorithm,
 * and, in fact does not presently define any alternative method.
 */
INT16
wtls_crypto_compress (wtls_crypto_t *cobj,
                      BYTE *buf, UINT16 buflen,
                      BYTE *outbuf, UINT16 *outbuflen)
{
  cobj = cobj;

  if (outbuf == NULL) {
    *outbuflen = buflen;
    return RET_OK;
  }
  if (*outbuflen < buflen) {
    *outbuflen = buflen;
    return -1;
  }
  *outbuflen = buflen;
  B_COPYSTRINGN (outbuf, buf, buflen);

  return RET_OK;
}

INT16
wtls_crypto_decompress (wtls_crypto_t *cobj,
                        BYTE *buf, UINT16 buflen,
                        BYTE *outbuf, UINT16 *outbuflen)
{
  cobj = cobj;

  if (outbuf == NULL) {
    *outbuflen = buflen;
    return RET_OK;
  }
  if (*outbuflen < buflen) {
    *outbuflen = buflen;
    return -1;
  }
  *outbuflen = buflen;
  B_COPYSTRINGN (outbuf, buf, buflen);

  return RET_OK;
}
#endif

INT16
wtls_crypto_HMAC_init (wtls_crypto_t *cobj, BYTE *key, UINT16 keylen)
{
  BYTE   buf1[MAX_HASH_BLOCK_SIZE];
  UINT16 i = 0;

  if (keylen > cobj->mac_block_size) {
    wtls_err_set (ERR_CRYPTLIB, ERR_HASH_KEY_TOO_LONG,
                  1, ALERT_LEVEL_CRITICAL, ALERT_DESC_INTERNAL_ERROR);
    return -1;
  }

  /* Step 0: Copy the key material */
  /* Step 1: Append zeros to a length of B bytes */
  /* Step 2: XOR with ipad (0x36) */
  for (i = 0; i < keylen; i++) {
    buf1[i] = key[i] ^ 0x36;
  }
  for (i = keylen; i < cobj->mac_block_size; i++) {
    buf1[i] = 0x36;
  }

  /* Step 3: Append the data */
  /* Step 4: Apply H to the data generated in step 3. */
  if ((wtls_crypto_hash_init (cobj) < 0) ||
      (wtls_crypto_hash_update (cobj, buf1, cobj->mac_block_size) < 0)) {
    return -1;
  }

  return RET_OK;
}

INT16
wtls_crypto_HMAC_update (wtls_crypto_t *cobj, BYTE *data, UINT16 datalen)
{
  if (wtls_crypto_hash_update (cobj, data, datalen) < 0) {
    return -1;
  }

  return RET_OK;
}

INT16
wtls_crypto_HMAC_final (wtls_crypto_t *cobj,
                        BYTE *key, UINT16 keylen, BYTE *result)
{
  BYTE  buf1[MAX_HASH_BLOCK_SIZE];
  BYTE  md[MAX_HASH_MAC_SIZE];
  INT16 i;

  if (wtls_crypto_hash_final (cobj, md) < 0) {
    return -1;
  }

  /* Step 5: XOR the byte string computed in step 1 with opad (0x5c) */
  for (i = 0; i < keylen; i++) {
    buf1[i] = key[i] ^ 0x5c;
  }
  for (i = keylen; i < cobj->mac_block_size; i++) {
    buf1[i] = 0x5c;
  }

  /* Step 6: Append the result from step 4 to the string from step 5 */
  /* Step 7: Apply H to the data generated in step 6 */
  if ((wtls_crypto_hash_init (cobj) < 0) ||
      (wtls_crypto_hash_update (cobj, buf1, cobj->mac_block_size) < 0) ||
      (wtls_crypto_hash_update (cobj, md, cobj->full_mac_size) < 0) ||
      (wtls_crypto_hash_final (cobj, result) < 0)) {
    return -1;
  }

  return RET_OK;
}


/*
 * Compute the Message Authentication Code as described in sections 9.2.3.2
 * and 11.3.1 of the WTLS specification.
 * The result is placed at the location indicated by 'mac'.
 */
INT16
wtls_crypto_MAC (wtls_crypto_t *cobj, BYTE *mac_secret,
                 BYTE *buf, UINT16 buflen,
                 UINT16 seqnum, UINT8 rec_type,
                 BYTE *mac)
{
  BYTE   tmpbuf[5];

  /* Here we use WTLS spec section 9.2.3.2.
   * Concatenate: seqnum + rec_type + buflen + data */
  tmpbuf[0] = (seqnum >> 8) & 0xff;
  tmpbuf[1] = seqnum & 0xff;
  tmpbuf[2] = rec_type;
  tmpbuf[3] = (buflen >> 8) & 0xff;
  tmpbuf[4] = buflen & 0xff;

  if ((wtls_crypto_HMAC_init (cobj, mac_secret, cobj->mac_key_size) < 0) ||
      (wtls_crypto_HMAC_update (cobj, tmpbuf, 5) < 0) ||
      (wtls_crypto_HMAC_update (cobj, buf, buflen) < 0) ||
      (wtls_crypto_HMAC_final (cobj, mac_secret,
                               cobj->mac_key_size, mac) < 0)) {
    return -1;
  }

  return RET_OK;
}


/*
 * Return the length of the key block to be generated as part
 * of key generation.
 */
static UINT16
wtls_crypto_key_block_size (wtls_crypto_t *cobj)
{
  return  cobj->mac_key_size + cobj->key_material_length + cobj->iv_size;
}

/*
 * Refresh the read keys for this connection state, using the given
 * label and sequence number.
 */
INT16
wtls_crypto_key_refresh1 (void *connptr, SDL_Natural state)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_connection_state *cstate;
  CHAR                  *label;
  UINT16                seqnum;
  UINT16                key_block_size;
  BYTE                  p[34];
  BYTE                  tmplabel[20];

  if (state == CONSTATE_READ) {
    cstate = &(conn->read);
    label = "server";
  }
  else {
    cstate = &(conn->write);
    label = "client";
  }
  seqnum = cstate->last_refresh;
  key_block_size = wtls_crypto_key_block_size (&(cstate->cobj));

  /* Concatenate: seqnum + ServerHello.random + ClientHello.random */
  p[0] = (seqnum >> 8) & 0xff;
  p[1] = seqnum & 0xff;
  B_COPYSTRINGN (p + 2, cstate->server_hello_random, 16);
  B_COPYSTRINGN (p + 18, cstate->client_hello_random, 16);

#ifdef FEATURE_WAP
  sprintf ((char *)tmplabel, "%s expansion", label);
#else
  sprintf (tmplabel, "%s expansion", label);
#endif //FEATURE_WAP
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_PRF, "WTLS: calling CRYPTa_PRF");
#endif
  CRYPTa_PRF ((UINT16)conn->rec_proc,
              cstate->cobj.hash_alg,
              cstate->master_secret_id, NULL, 0,
              tmplabel, p, 34, key_block_size);

  return RET_OK;
}

INT16
wtls_crypto_key_refresh2 (void *connptr, SDL_Natural state,
                          void *buf, SDL_Natural buflen)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_connection_state *cstate;

  buflen = buflen;

  if (state == CONSTATE_READ) {
    cstate = &(conn->read);
  }
  else {
    cstate = &(conn->write);
  }

  B_COPYSTRINGN (cstate->mac_secret, buf, cstate->cobj.mac_key_size);
  B_COPYSTRINGN (cstate->encryption_key,
                 (BYTE *)buf + cstate->cobj.mac_key_size,
                 cstate->cobj.key_material_length);
  B_COPYSTRINGN (cstate->iv, (BYTE *)buf + cstate->cobj.mac_key_size +
                 cstate->cobj.key_material_length, cstate->cobj.iv_size);
  DEALLOC (&buf);

  if (cstate->cobj.is_exportable) {
    return RET_EXPORTABLE;
  }
  else {
    return RET_OK;
  }
}

INT16
wtls_crypto_key_refresh3 (void *connptr, SDL_Natural state)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_connection_state *cstate;
  CHAR                  *label;
  BYTE                  p[34];
  BYTE                  tmplabel[20];

  if (state == CONSTATE_READ) {
    cstate = &(conn->read);
    label = "server";
  }
  else {
    cstate = &(conn->write);
    label = "client";
  }

  /* Exportable algorithms have a shorter keyMaterialLength than keyLength,
   * and hence need a further expansion to bring the key length up
   * to the required length. */
  B_COPYSTRINGN (p, cstate->client_hello_random, 16);
  B_COPYSTRINGN (p + 16, cstate->server_hello_random, 16);
#ifdef FEATURE_WAP
  sprintf ((char *)tmplabel, "%s write key", label);
#else
  sprintf (tmplabel, "%s write key", label);
#endif //FEATURE_WAP
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_PRF, "WTLS: calling CRYPTa_PRF");
#endif
  CRYPTa_PRF ((UINT16)conn->rec_proc,
              cstate->cobj.hash_alg,
              0, cstate->encryption_key,
              cstate->cobj.key_material_length,
              tmplabel, p, 32, cstate->cobj.key_size);

  return RET_OK;
}

INT16
wtls_crypto_key_refresh4 (void *connptr, SDL_Natural state, void *buf,
                          SDL_Natural buflen)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_connection_state *cstate;
  CHAR                  *label;
  UINT16                seqnum;
  BYTE                  p[34];
  BYTE                  tmplabel[20];

  buflen = buflen;

  if (state == CONSTATE_READ) {
    cstate = &(conn->read);
    label = "server";
  }
  else {
    cstate = &(conn->write);
    label = "client";
  }
  seqnum = cstate->last_refresh;

  B_COPYSTRINGN (cstate->encryption_key, buf, cstate->cobj.key_size);
  DEALLOC (&buf);

  p[0] = (seqnum >> 8) & 0xff;
  p[1] = seqnum & 0xff;
  B_COPYSTRINGN (p + 2, cstate->client_hello_random, 16);
  B_COPYSTRINGN (p + 18, cstate->server_hello_random, 16);
#ifdef FEATURE_WAP
  sprintf ((char *)tmplabel, "%s write IV", label);
#else
  sprintf (tmplabel, "%s write IV", label);
#endif //#ifdef FEATURE_WAP
#ifdef LOG_EXTERNAL
  CLNTa_log (0, log_wtls_CRYPTa_PRF, "WTLS: calling CRYPTa_PRF");
#endif
#ifdef FEATURE_WAP
  CRYPTa_PRF ((UINT16)conn->rec_proc,
              cstate->cobj.hash_alg,
              0, (BYTE *)"", 0, tmplabel,
              p, 34, cstate->cobj.iv_size);
#else
  CRYPTa_PRF ((UINT16)conn->rec_proc,
              cstate->cobj.hash_alg,
              0, "", 0, tmplabel,
              p, 34, cstate->cobj.iv_size);
#endif //FEATURE_WAP

  return RET_OK;
}

INT16
wtls_crypto_key_refresh5 (void *connptr, SDL_Natural state,
                          void *buf, SDL_Natural buflen)
{
  wtls_connection_t     *conn = (wtls_connection_t *)connptr;
  wtls_connection_state *cstate;

  buflen = buflen;

  if (state == CONSTATE_READ) {
    cstate = &(conn->read);
  }
  else {
    cstate = &(conn->write);
  }
  B_COPYSTRINGN (cstate->iv, buf, cstate->cobj.iv_size);
  DEALLOC (&buf);

  return RET_OK;
}
