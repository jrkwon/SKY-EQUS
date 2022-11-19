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
 * 
 */
#ifdef CAN_SIGN_TEXT

#include "cmmnrsrc.h"
#include "confvars.h"
#include "aapiclnt.h"
#include "aapicrpt.h"

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
SignerInfoSize (SignerInfo *sip)
{
  switch (sip->signerInfoType) {
  case SIGNERINFO_IMPLICIT:
    return 0;

  case SIGNERINFO_SHA_KEY_HASH:
    return 21;

  case SIGNERINFO_WTLS_CERTIFICATE:
    return (INT16)(1 + sip->infoLen);

  case SIGNERINFO_X509_CERTIFICATE:
  case SIGNERINFO_X968_CERTIFICATE:
    return (INT16)(3 + sip->infoLen);

  case SIGNERINFO_CERTIFICATE_URL:
    return (INT16)(2 + sip->infoLen);

  default:
    return -1;
  }
}

static INT16
SignedContentSize (SignedContent *scp)
{
  INT16 s = (INT16)(1 +
                    3 + scp->signature.siglen);
    
  s += 2 + SignerInfoSize (&(scp->signerInfo[0])) +
    SignerInfoSize (&(scp->signerInfo[1]));

  s += 4;
  if (scp->contentInfo.contentPresent) {
    s += 2 + scp->contentInfo.contentLen;
  }

  s += 3;
  if (scp->authenticatedAttribute.attributeType ==
      ATTRIBUTETYPE_GMT_UTC_TIME) {
    s += 12;
  }
  else {
    s += 8;
  }

  return s;
}

/*
 * The stream object used for translating to and from
 * byte stream representation.
 */
typedef struct {
  UINT16 pos;
  UINT16 length;
  BYTE   *data;
} ByteStream;

#define STREAM_REMAINING_LENGTH(str) ((str)->length - (str)->pos)

void
InitStream (ByteStream *str, BYTE *buf, UINT16 len)
{
  str->data = buf;
  str->length = len;
  str->pos = 0;
}

static INT16
StreamPutByte (ByteStream *str, BYTE b)
{
  str->data[str->pos] = b;
  str->pos += 1;

  return 0;
}

static INT16
EncodeUInt8 (ByteStream *str, UINT8 data)
{
  if (STREAM_REMAINING_LENGTH (str) < 1) {
    return -1;
  }
  return StreamPutByte (str, data);
}

static INT16
EncodeUInt16 (ByteStream *str, UINT16 data)
{
  if (STREAM_REMAINING_LENGTH (str) < 2) {
    return -1;
  }
  if (StreamPutByte (str, (UINT8)((data >> 8) & 0xff)) ||
      StreamPutByte (str, (UINT8)(data & 0xff))) {
    return -1;
  }

  return 0;
}

/*
 * Copy 'len' bytes from the buffer pointed to by 'data'
 * into the given stream.
 */
static INT16
EncodeByteVector (ByteStream *str, UINT16 len, BYTE *data)
{
  if (STREAM_REMAINING_LENGTH (str) < len) {
    return -1;
  }
  B_COPYSTRINGN (str->data + str->pos, data, len);
  str->pos += len;

  return 0;
}

static INT16
EncodeSignature (ByteStream *str, Signature *data)
{
  if (EncodeUInt8 (str, data->algorithm)) {
    return -1;
  }
  if (data->algorithm != SIGALG_NULL) {
    if (EncodeUInt16 (str, data->siglen) ||
        EncodeByteVector (str, data->siglen, data->sig)) {
      return -1;
    }
  }

  return 0;
}

static INT16
EncodeSignerInfo (ByteStream *str, SignerInfo *data)
{
  if (EncodeUInt8 (str, data->signerInfoType)) {
    return -1;
  }
  switch (data->signerInfoType) {
  case SIGNERINFO_IMPLICIT:
    break;

  case SIGNERINFO_SHA_KEY_HASH:
    if (EncodeByteVector (str, 20, data->info)) {
      return -1;
    }
    break;

  case SIGNERINFO_WTLS_CERTIFICATE:
    if (EncodeByteVector (str, data->infoLen, data->info)) {
      return -1;
    }
    break;

  case SIGNERINFO_X509_CERTIFICATE:
  case SIGNERINFO_X968_CERTIFICATE:
    if (EncodeUInt16 (str, data->infoLen) ||
        EncodeByteVector (str, data->infoLen, data->info)) {
      return -1;
    }
    break;

  case SIGNERINFO_CERTIFICATE_URL:
    if (EncodeUInt8 (str, (UINT8)(data->infoLen)) ||
        EncodeByteVector (str, data->infoLen, data->info)) {
      return -1;
    }
    break;

  default:
    return -1;
  }

  return 0;
}

static INT16
EncodeSignerInfos (ByteStream *str, SignerInfo *data)
{
  if (EncodeUInt16 (str, (UINT16)(SignerInfoSize (&data[0]) +
                                  SignerInfoSize (&data[1])))) {
    return -1;
  }
  if (data[0].signerInfoType != SIGNERINFO_IMPLICIT) {
    if (EncodeSignerInfo (str, &data[0])) {
      return -1;
    }
  }
  if (data[1].signerInfoType != SIGNERINFO_IMPLICIT) {
    if (EncodeSignerInfo (str, &data[1])) {
      return -1;
    }
  }
  
  return 0;
}

static INT16
EncodeContentInfo (ByteStream *str, ContentInfo *data)
{
  if (EncodeUInt8 (str, data->contentType) ||
      EncodeUInt16 (str, data->contentEncoding) ||
      EncodeUInt8 (str, data->contentPresent)) {
    return -1;
  }
  if (data->contentPresent) {
    if (EncodeUInt16 (str, data->contentLen) ||
        EncodeByteVector (str, data->contentLen, data->content)) {
      return -1;
    }
  }

  return 0;
}

static INT16
EncodeAuthenticatedAttribute (ByteStream *str, AuthenticatedAttribute *data)
{
  switch (data->attributeType) {
  case ATTRIBUTETYPE_GMT_UTC_TIME:
    if (EncodeUInt8 (str, 13) ||
        EncodeUInt8 (str, data->attributeType) ||
        EncodeByteVector (str, 12, data->_u.gmtUTCTime)) {
      return -1;
    }
    break;

  case ATTRIBUTETYPE_SIGNER_NONCE:
    if (EncodeUInt8 (str, 9) ||
        EncodeUInt8 (str, data->attributeType) ||
        EncodeByteVector (str, 8, data->_u.signerNonce)) {
      return -1;
    }
    break;

  default:
    return -1;
  }

  return 0;
}

static INT16
EncodeSignedContent (ByteStream *str, SignedContent *data)
{
  if (EncodeUInt8 (str, data->version) ||
      EncodeSignature (str, &(data->signature)) ||
      EncodeSignerInfos (str, data->signerInfo) ||
      EncodeContentInfo (str, &(data->contentInfo)) ||
      EncodeAuthenticatedAttribute (str, &(data->authenticatedAttribute))) {
    return -1;
  }

  return 0;
}


/************************************************************
 * The public routines of this module:
 *   ST_createDigestInfo
 *   ST_encodeSignedContent
 *   ST_newSignedContent
 *   ST_freeSignedContent
 *
 ************************************************************/

static BYTE hashbuf_umt[89] = {
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

static BYTE hashbuf_nonce[85] = {
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
    len = 89;
    hashbuf = hashbuf_umt;
    /* Write UTC time into hashbuf[41..53] */
    Time2String (CLNTa_currentTime (), (CHAR *)(hashbuf_umt + 41));
    hashbuf_umt[53] = 'Z';
    sc->authenticatedAttribute.attributeType = ATTRIBUTETYPE_GMT_UTC_TIME;
    B_COPYSTRINGN (sc->authenticatedAttribute._u.gmtUTCTime,
                   hashbuf_umt + 41, 12);
  }
  else {
    len = 85;
    hashbuf = hashbuf_nonce;
    /* Write random number into hashbuf[42..49] */
    CRYPTa_generateRandom (hashbuf_nonce + 42, 8);
    sc->authenticatedAttribute.attributeType = ATTRIBUTETYPE_SIGNER_NONCE;
    B_COPYSTRINGN (sc->authenticatedAttribute._u.signerNonce,
                   hashbuf_nonce + 42, 8);
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
  if (CRYPTa_hash (HASH_SHA, buf, buflen,
                   hashbuf + len - 20, diglen) != CRV_OK) {
    *diglen = 0;
    return -1;
  }

  /* Calculate SHA-1 hash of the hashbuf */
  B_COPYSTRINGN (digestinfo, digestinfo_prefix, 15);
  *diglen = 20;
  if (CRYPTa_hash (HASH_SHA, hashbuf, len,
                   digestinfo + 15, diglen) != CRV_OK) {
    *diglen = 15;
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
  ByteStream str;
  BYTE       *buf;
  UINT16     len;

  len = SignedContentSize (sc);
  buf = NEWARRAY (BYTE, len);
  if (buf == NULL) {
    return -1;
  }
  InitStream (&str, buf, len);
  if (EncodeSignedContent (&str, sc)) {
    DEALLOC (&buf);
    return -1;
  }

  if (Base64Encoding (buf, str.pos, result, reslen) < 0) {
    DEALLOC (&buf);
    return -1;
  }

  DEALLOC (&buf);

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
