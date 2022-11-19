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
 * si_crpto.c
 *
 * Created by Anders Edenbrandt, Mon Jan 10 10:27:04 2000.
 *
 * Revision history:
 *   000119, AED: Moved parameter "options" from ST_encodeSignedContent
 *                to ST_createDigestInfo.
 *                Changed ST_encodeSignedContent to return a
 *                null-terminated byte string.
 *   000809, AED: Removed the dependency on files from the WTLS
 *                implementation. Still needs the crypto API, though.
 *   010119, AED: Use wapcvt instead of private conversion routines.
 *                Removed obsolete parameter from CRYPTa_hash.
 *   010212, AED: Added conditional fix for the bug correction to the spec.
 * 
 */
#include "confvars.h"

#ifdef CAN_SIGN_TEXT

#include "cmmnrsrc.h"
#include "aapiclnt.h"
#include "aapicrpt.h"
#include "wapcvt.h"
#include "si_crpto.h"

/************************************************************
 * Base-64 encoding
 ************************************************************/

/* The "alphabet" of base-64 encoding: */
static const BYTE b64[64] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * Given data of arbitrary length, create a base-64 encoding
 * of said data, and place it in a newly allocated buffer.
 * The created string will be null-terminated.
 *
 * NOTE: it is the caller's responsibility to deallocate
 * the buffer holding the result.
 *
 * Returns 0 if OK, and -1 on error.
 */
static INT16
Base64Encoding (BYTE *inbuf, UINT16 inlen, BYTE **outbuf, UINT16 *outlen)
{
  UINT16 len = (UINT16)(((inlen + 2) / 3) * 4);
  BYTE   *p;
  UINT16 i, j;

  /* Add space for line breaks. */
  len += 2 * (len / 76);

  *outbuf = NEWARRAY (BYTE, len + 1);
  if (*outbuf == NULL) {
    *outlen = 0;
    return -1;
  }
  *outlen = len;

  for (i = 0, j = 0, p = *outbuf; i + 2 < inlen; i += 3, p += 4, j += 4) {
    if (j >= 76) {
      p[0] = '\r';
      p[1] = '\n';
      p += 2;
      j = 0;
    }
    p[0] = b64[(inbuf[i] >> 2) & 0x3f];
    p[1] = b64[((inbuf[i] << 4) & 0x30) | ((inbuf[i + 1] >> 4) & 0x0f)];
    p[2] = b64[((inbuf[i + 1] << 2) & 0x3c) | ((inbuf[i + 2] >> 6) & 0x03)];
    p[3] = b64[inbuf[i + 2] & 0x3f];
  }

  if (i < inlen) {
    if (j >= 76) {
      p[0] = '\r';
      p[1] = '\n';
      p += 2;
    }
    p[0] = b64[(inbuf[i] >> 2) & 0x3f];
    if (i + 1 < inlen) {
      p[1] = b64[((inbuf[i] << 4) & 0x30) | ((inbuf[i + 1] >> 4) & 0x0f)];
      p[2] = b64[((inbuf[i + 1] << 2) & 0x3c)];
    }
    else {
      p[1] = b64[((inbuf[i] << 4) & 0x30)];
      p[2] = '=';
    }
    p[3] = '=';
  }
  (*outbuf)[len] = '\0';

  return 0;
}


/**********************************************************************
 * Formatting of a Unix 32-bit time value (seconds since 1/1 1970)
 * as a string in the format YYMMDDHHMMSS.
 **********************************************************************/

static const BYTE m[12] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static const BYTE m4[12] = {
  31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * Format a Unix 32-bit time value (seconds since 1/1 1970)
 * as a string in the format YYMMDDHHMMSS.
 * Parameter "t" is the given time, "result" is where the result
 * should be stored.
 */
static void
Time2String (UINT32 t, CHAR *result)
{
  INT16      secs, mins, hours, days, months, years;
  INT16      i, nly;
  const BYTE *days_in_month;

  /* First, divide into days, hours, minutes and seconds. */
  days = (INT16)(t / 86400);
  t = t % 86400;

  hours = (INT16)(t / 3600);
  t = t % 3600;

  mins = (INT16)(t / 60);
  secs = (INT16)(t % 60);

  /* Now the number of days has to be divided into years and months.
   * Start by approximating each year to be 365 days.
   * This approximation will be at most one off.
   * Compensate if necessary. */
  years = (INT16)(days / 365);
  days = (INT16)(days % 365);
  /* In this interval (1970 - 2037), every fourth year is a leap year,
   * without exceptions, starting with 1972. */
  nly = (INT16)((years + 1) / 4);
  if (days < nly) {
    years--;
    days = (INT16)(days + 365 - nly);
  }
  else {
    days -= nly;
  }

  /* To determine the month we simply do a linear search through
   * an array holding the number of days of each month.
   * First we have to select the correct array though,
   * there is one for leap years and one for non-leap years. */
  if (((years + 2) % 4) == 0)
    days_in_month = m4;
  else
    days_in_month = m;

  for (i = 0; i < 12; i++) {
    if (days < days_in_month[i])
      break;
    days -= days_in_month[i];
  }
  months = i;

  /* We are done. The variable "years" holds the number of complete
   * years that have passed since 1970; "months" holds the number
   * of complete months that have passed since the beginning of
   * the present year; and similarly for the other variables. */
  sprintf (result, "%02d%02d%02d%02d%02d%02d\n",
           (years + 1970) % 100, months + 1, days + 1, hours, mins, secs);
}


/************************************************************
 * Routines to do encoding of a SignedContent structure.
 ************************************************************/

static INT16
wap_encode_signature (wap_cvt_t *obj, Signature *p)
{
  if (!wap_cvt_uint8 (obj, &p->algorithm)) {
    return FALSE;
  }
  if (p->algorithm != SIGALG_NULL) {
    if (!wap_cvt_uint16 (obj, &p->siglen) ||
        !wap_cvt_bytevector (obj, p->siglen, &p->sig)) {
      return FALSE;
    }
  }

  return TRUE;
}

static INT16
wap_encode_signer_info (wap_cvt_t *obj, SignerInfo *p)
{
  if (!wap_cvt_uint8 (obj, &p->signerInfoType)) {
    return FALSE;
  }
  switch (p->signerInfoType) {
  case SIGNERINFO_IMPLICIT:
    break;

  case SIGNERINFO_SHA_KEY_HASH:
    if (!wap_cvt_bytevector (obj, 20, &p->info)) {
      return FALSE;
    }
    break;

  case SIGNERINFO_WTLS_CERTIFICATE:
    if (!wap_cvt_bytevector (obj, p->infoLen, &p->info)) {
      return FALSE;
    }
    break;

  case SIGNERINFO_X509_CERTIFICATE:
  case SIGNERINFO_X968_CERTIFICATE:
    if (!wap_cvt_uint16 (obj, &p->infoLen) ||
        !wap_cvt_bytevector (obj, p->infoLen, &p->info)) {
      return FALSE;
    }
    break;

  case SIGNERINFO_CERTIFICATE_URL:
    {
      UINT8 tmp = (UINT8)p->infoLen;

      if (!wap_cvt_uint8 (obj, &tmp) ||
          !wap_cvt_bytevector (obj, tmp, &p->info))
        return FALSE;
    }
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

static INT16
wap_encode_signer_infos (wap_cvt_t *obj, SignerInfo *p)
{
  wap_cvt_t tmp_cvt = *obj;
  UINT16    len;

  obj->pos += 2;
  if (p[0].signerInfoType != SIGNERINFO_IMPLICIT) {
    if (!wap_encode_signer_info (obj, &p[0])) {
      return FALSE;
    }
  }
  if (p[1].signerInfoType != SIGNERINFO_IMPLICIT) {
    if (!wap_encode_signer_info (obj, &p[1])) {
      return FALSE;
    }
  }

  len = (UINT16)(obj->pos - tmp_cvt.pos - 2);
  if (!wap_cvt_uint16 (&tmp_cvt, &len))
    return FALSE;

  return TRUE;
}

static INT16
wap_encode_content_info (wap_cvt_t *obj, ContentInfo *p)
{
  if (!wap_cvt_uint8 (obj, &p->contentType) ||
      !wap_cvt_uint16 (obj, &p->contentEncoding) ||
      !wap_cvt_uint8 (obj, &p->contentPresent)) {
    return FALSE;
  }
  if (p->contentPresent) {
    if (!wap_cvt_uint16 (obj, &p->contentLen) ||
        !wap_cvt_bytevector (obj, p->contentLen, &p->content)) {
      return FALSE;
    }
  }

  return TRUE;
}

static INT16
wap_encode_authenticated_attribute (wap_cvt_t *obj, AuthenticatedAttribute *p)
{
  UINT8 tmp;

  switch (p->attributeType) {
  case ATTRIBUTETYPE_GMT_UTC_TIME:
    tmp = 13;
    if (!wap_cvt_uint8 (obj, &tmp) ||
        !wap_cvt_uint8 (obj, &p->attributeType) ||
        !wap_cvt_static_bytevector (obj, 12, p->_u.gmtUTCTime)) {
      return FALSE;
    }
    break;

  case ATTRIBUTETYPE_SIGNER_NONCE:
    tmp = 9;
    if (!wap_cvt_uint8 (obj, &tmp) ||
        !wap_cvt_uint8 (obj, &p->attributeType) ||
        !wap_cvt_static_bytevector (obj, 8, p->_u.signerNonce)) {
      return FALSE;
    }
    break;

  default:
    return FALSE;
  }

  return TRUE;
}

static INT16
wap_encode_signed_content (wap_cvt_t *obj, SignedContent *p)
{
  if (!wap_cvt_uint8 (obj, &p->version) ||
      !wap_encode_signature (obj, &(p->signature)) ||
      !wap_encode_signer_infos (obj, p->signerInfo) ||
      !wap_encode_content_info (obj, &(p->contentInfo)) ||
      !wap_encode_authenticated_attribute (obj,
                                           &(p->authenticatedAttribute))) {
    return FALSE;
  }

  return TRUE;
}


/************************************************************
 * The public routines of this module:
 *   ST_createDigestInfo
 *   ST_encodeSignedContent
 *   ST_newSignedContent
 *   ST_freeSignedContent
 *
 ************************************************************/

#ifdef SIGN_TEXT_FIXED

#define HASHBUF_UMT_LENGTH          95
#define UMT_LENGTH                  13
#define UMT_START_POS               45

static BYTE hashbuf_umt[HASHBUF_UMT_LENGTH] = {
  0x31, 0x5d,
  0x30, 0x18,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
  0x31, 0x0b,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
  0x30, 0x1c,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x05,
  0x31, 0x0f,
  0x17, 0x0d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0x30, 0x23,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
  0x31, 0x16,
  0x04, 0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


#define HASHBUF_NONCE_LENGTH        91
#define NONCE_LENGTH                 8
#define NONCE_START_POS             46

static BYTE hashbuf_nonce[HASHBUF_NONCE_LENGTH] = {
  0x31, 0x59,
  0x30, 0x18,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
  0x31, 0x0b,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
  0x30, 0x18,
  0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x19, 0x03,
  0x31, 0x0a,
  0x04, 0x08, 0, 0, 0, 0, 0, 0, 0, 0,
  0x30, 0x23,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
  0x31, 0x16,
  0x04, 0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#else
#define HASHBUF_UMT_LENGTH          89
#define UMT_LENGTH                  13
#define UMT_START_POS               41

static BYTE hashbuf_umt[HASHBUF_UMT_LENGTH] = {
  0x31, 0x57,
  0x30, 0x16,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
  0x30, 0x1a,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x05,
  0x17, 0x0d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0x30, 0x21,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
  0x04, 0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


#define HASHBUF_NONCE_LENGTH        85
#define NONCE_LENGTH                 8
#define NONCE_START_POS             42

static BYTE hashbuf_nonce[HASHBUF_NONCE_LENGTH] = {
  0x31, 0x57,
  0x30, 0x16,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x03,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01,
  0x30, 0x16,
  0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x19, 0x03,
  0x04, 0x08, 0, 0, 0, 0, 0, 0, 0, 0,
  0x30, 0x21,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x04,
  0x04, 0x14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#endif

const static BYTE digestinfo_prefix[15] = {
  0x30, 0x21,
  0x30, 0x09,
  0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a,
  0x05, 0x00,
  0x04, 0x14
};

/*
 * ST_createDigestInfo
 *
 * Prepare the value to be encrypted given a buffer of data.
 * The hash is formatted in a DER-encoded structure compatible
 * with PKCS#7. See WMLScript Crypto Library Specification, section 6.2.
 *
 * Parameters:
 *   buf        the data to be signed, encoded as UTF-8
 *   buflen     length of data
 *   digestinfo where the result is placed (usually 35 bytes)
 *   diglen     on input, the length of the digestinfo buffer; on output,
 *              the number of bytes placed in digestinfo
 *   options    as received from the signText function
 *   sc         a pointer to a structure of type SignedContent;
 *              this routine will store values in this structure,
 *              to be passed to the routine ST_packResult later.
 *
 * NOTE: caller should NOT deallocate the buffer "buf" explicitly.
 * It should be released by a call to ST_freeSignedContent after
 * ST_encodeSignedContent has used it.
 *
 * Return value: 0 if OK, -1 if an error occurred.
 */
INT16
ST_createDigestInfo (BYTE *buf, UINT16 buflen,
                     BYTE *digestinfo, UINT16 *diglen,
                     UINT8 options,
                     SignedContent *sc)
{
  UINT16 len;
  BYTE   *hashbuf;

  if ((buf == NULL) || (digestinfo == NULL) ||
      (*diglen < 35) || (sc == NULL)) {
    return -1;
  }

  if (cfg_wae_ua_current_time_is_gmt) {
    len = HASHBUF_UMT_LENGTH;
    hashbuf = hashbuf_umt;
    /* Write UTC time into hashbuf[41..53] */
    Time2String (CLNTa_currentTime (), (CHAR *)(hashbuf_umt + UMT_START_POS));
    hashbuf_umt[UMT_START_POS + UMT_LENGTH - 1] = 'Z';
    sc->authenticatedAttribute.attributeType = ATTRIBUTETYPE_GMT_UTC_TIME;
    B_COPYSTRINGN (sc->authenticatedAttribute._u.gmtUTCTime,
                   hashbuf_umt + UMT_START_POS, UMT_LENGTH - 1);
  }
  else {
    len = HASHBUF_NONCE_LENGTH;
    hashbuf = hashbuf_nonce;
    /* Write random number into hashbuf[42..49] */
    CRYPTa_generateRandom (hashbuf_nonce + NONCE_START_POS, NONCE_LENGTH);
    sc->authenticatedAttribute.attributeType = ATTRIBUTETYPE_SIGNER_NONCE;
    B_COPYSTRINGN (sc->authenticatedAttribute._u.signerNonce,
                   hashbuf_nonce + NONCE_START_POS, NONCE_LENGTH);
  }

  /* Store content */
  sc->contentInfo.contentType = CONTENTTYPE_TEXT;
  sc->contentInfo.contentEncoding = IANA_CHARSET_UTF8;
  if (options & 0x01) {
    sc->contentInfo.contentPresent = 1;
  }
  else {
    sc->contentInfo.contentPresent = 0;
  }    
  sc->contentInfo.contentLen = buflen;
  sc->contentInfo.content = buf;

  /* Compute SHA-1 hash of given data *
   * Write computed hash into hashbuf */
  *diglen = 20;
  if (CRYPTa_hash (HASH_SHA, buf, buflen, hashbuf + len - 20) != CRV_OK) {
    *diglen = 0;
    sc->contentInfo.content = NULL;
    return -1;
  }

  /* Calculate SHA-1 hash of the hashbuf */
  B_COPYSTRINGN (digestinfo, digestinfo_prefix, 15);
  *diglen = 20;
  if (CRYPTa_hash (HASH_SHA, hashbuf, len, digestinfo + 15) != CRV_OK) {
    *diglen = 15;
    sc->contentInfo.content = NULL;
    return -1;
  }
  
  *diglen = 35;
  return 0;
}


/*
 * ST_encodeSignedContent
 *
 * Given the contents of SignedContent,
 * computed the base-64-encoding of that structure.
 *
 * Parameters:
 *   sc        the SignedContent structure
 *   result    a pointer to a newly allocated string will be placed here,
 *             holding the result: a null-terminated byte string
 *   reslen    the length of the result string
 *
 * NOTE: it is the caller's responsibility to deallocate the result string.
 *
 * Return value: 0 if OK, -1 if an error occurred.
 */
INT16
ST_encodeSignedContent (SignedContent *sc,
                        BYTE **result, UINT16 *reslen)
{
  wap_cvt_t cvt_obj;
  BYTE     *buf;
  UINT16    buflen;

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE_SIZE, NULL, 0);
  if (!wap_encode_signed_content (&cvt_obj, sc))
    return -1;
  buflen = (UINT16)cvt_obj.pos;
  buf = OSConnectorAlloc (buflen);
  if (buf == NULL) {
    return -1;
  }

  wap_cvt_init (&cvt_obj, WAP_CVT_ENCODE, buf, buflen);
  if (!wap_encode_signed_content (&cvt_obj, sc)) {
    OSConnectorFree (buf);
    return -1;
  }

  if (Base64Encoding (buf, buflen, result, reslen) < 0) {
    OSConnectorFree (buf);
    return -1;
  }

  OSConnectorFree (buf);

  return 0;
}

/*
 * ST_newSignedContent
 *
 * Allocate and initialize a new SignedContent structure.
 * Returns NULL in case of error.
 */
SignedContent *
ST_newSignedContent (void)
{
  SignedContent *sc = NEWARRAY (SignedContent, 1);

  if (sc == NULL) {
    return NULL;
  }

  sc->version = 1;

  sc->signature.algorithm = SIGALG_NULL;
  sc->signature.siglen = 0;
  sc->signature.sig = NULL;

  sc->signerInfo[0].signerInfoType = SIGNERINFO_IMPLICIT;
  sc->signerInfo[0].infoLen = 0;
  sc->signerInfo[0].info = NULL;
  sc->signerInfo[1].signerInfoType = SIGNERINFO_IMPLICIT;
  sc->signerInfo[1].infoLen = 0;
  sc->signerInfo[1].info = NULL;

  sc->contentInfo.contentType = 0;
  sc->contentInfo.contentEncoding = 0;
  sc->contentInfo.contentPresent = 0;
  sc->contentInfo.contentLen = 0;
  sc->contentInfo.content = NULL;

  sc->authenticatedAttribute.attributeType = 0;

  return sc;
}


/*
 * ST_freeSignedContent
 *
 * Deallocate all relevant fields in the SignedContent structure,
 * as well as the SignedContent structure itself.
 */
void
ST_freeSignedContent (SignedContent *sc)
{
  DEALLOC (&(sc->signature.sig));

  if (sc->signerInfo[0].signerInfoType != SIGNERINFO_IMPLICIT) {
    DEALLOC (&(sc->signerInfo[0].info));
  }
  if (sc->signerInfo[1].signerInfoType != SIGNERINFO_IMPLICIT) {
    DEALLOC (&(sc->signerInfo[1].info));
  }

  DEALLOC (&(sc->contentInfo.content));

  DEALLOC (&sc);
}

#endif
