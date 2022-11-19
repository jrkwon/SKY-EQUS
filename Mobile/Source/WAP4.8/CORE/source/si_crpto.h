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
 * si_crpto.h
 *
 * Created by Anders Edenbrandt, Mon Jan 10 10:20:36 2000.
 *
 * Revision history:
 *   000119, AED: Moved parameter "options" from ST_encodeSignedContent
 *                to ST_createDigestInfo.
 *                Changed ST_encodeSignedContent to return a
 *                null-terminated byte string.
 *
 */
#ifndef _si_crpto_h
#define _si_crpto_h

#include "confvars.h"

#ifdef CAN_SIGN_TEXT

#include "tapicmmn.h"

#define SIGALG_NULL            0
#define SIGALG_RSA_SHA_PCKS1   1
#define SIGALG_ECDSA_SHA_P1363 2

typedef struct {
  UINT8  algorithm;
  UINT16 siglen;
  BYTE   *sig;
} Signature;


#define SIGNERINFO_IMPLICIT         0
#define SIGNERINFO_SHA_KEY_HASH     1
#define SIGNERINFO_WTLS_CERTIFICATE 2
#define SIGNERINFO_X509_CERTIFICATE 3
#define SIGNERINFO_X968_CERTIFICATE 4
#define SIGNERINFO_CERTIFICATE_URL  5

typedef struct {
  UINT8  signerInfoType;
  UINT16 infoLen;
  BYTE   *info;
} SignerInfo;


#define CONTENTTYPE_TEXT  1
#define CONTENTTYPE_DATA  2

typedef struct {
  UINT8  contentType;
  UINT16 contentEncoding;
  UINT8  contentPresent;
  UINT16 contentLen;
  BYTE   *content;
} ContentInfo;


#define ATTRIBUTETYPE_GMT_UTC_TIME  1
#define ATTRIBUTETYPE_SIGNER_NONCE  2

typedef struct {
  UINT8 attributeType;
  union {
    UINT8 gmtUTCTime[12];
    UINT8 signerNonce[8];
  } _u;
} AuthenticatedAttribute;


typedef struct {
  UINT8                  version;
  Signature              signature;
  SignerInfo             signerInfo[2];
  ContentInfo            contentInfo;
  AuthenticatedAttribute authenticatedAttribute;
} SignedContent;



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
                     SignedContent *sc);

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
                        BYTE **result, UINT16 *reslen);

/*
 * ST_newSignedContent
 *
 * Allocate and initialize a new SignedContent structure.
 * Returns NULL in case of error.
 */
SignedContent *
ST_newSignedContent (void);

/*
 * ST_freeSignedContent
 *
 * Deallocate all relevant fields in the SignedContent structure,
 * as well as the SignedContent structure itself.
 */
void
ST_freeSignedContent (SignedContent *sc);

#endif
#endif
