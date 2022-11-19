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
 * capicrpt.c
 *
 * Created by Anders Edenbrandt, Tue Mar 14 09:51:25 2000.
 *
 * Revision history:
 *
 */
#include "wiptrgt.h"

#include "aapicrpt.h"
#include "capicrpt.h"

#include "cmmnrsrc.h"
#include "msgqueue.h"
#include "ml_typw.h"
#include "mk_queu.h"
#ifdef LOG_EXTERNAL
#include "aapiclnt.h"
#endif

/*
 * Deliver the result from the CRYPTa_getMethods function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_getMethods, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The rest of the parameters are "cipherMethods", which holds
 * pairs of values, <BulkCipherAlgorithm, HashAlgorithm>;
 * "keyExchangeIds", which holds preferred key exchange methods;
 * and "trustedKeyIds", which holds trusted certificates.
 */
void
CRYPTc_getMethodsResponse (UINT16 id, INT16 result,
                           const BYTE *cipherMethods,
                           UINT16 cipherMethodLen,
                           const BYTE *keyExchangeIds,
                           UINT16 keyExchangeIdLen,
                           const BYTE *trustedKeyIds,
                           UINT16 trustedKeyIdLen)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_getMethodsResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT16[1] = cipherMethodLen;
  pMsg->vUINT16[2] = keyExchangeIdLen;
  pMsg->vUINT16[3] = trustedKeyIdLen;

  if (cipherMethods != NULL) {
    pc = (CHAR *)OSConnectorAlloc (cipherMethodLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, cipherMethods, cipherMethodLen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }

  if (keyExchangeIds != NULL) {
    pc = (CHAR *)OSConnectorAlloc (keyExchangeIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, keyExchangeIds, keyExchangeIdLen);
    pMsg->pCHAR[1] = pc;
  }
  else {
    pMsg->pCHAR[1] = NULL;
  }

  if (trustedKeyIds != NULL) {
    pc = (CHAR *)OSConnectorAlloc (trustedKeyIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, trustedKeyIds, trustedKeyIdLen);
    pMsg->pCHAR[2] = pc;
  }
  else {
    pMsg->pCHAR[2] = NULL;
  }

  pMsg->Id = eCRYPTc_getMethodsResponse;
  bSendMsg = SendMsg (pMsg);
}


/*
 * Deliver the result from the CRYPTa_PRF function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_PRF, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The parameter "buf" holds the computed value.
 */
void
CRYPTc_PRFResponse (UINT16 id, INT16 result,
                    const BYTE *buf, UINT16 buflen)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_PRFResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT16[1] = buflen;
  if (buf != NULL) {
    pc = (CHAR*)OSConnectorAlloc (buflen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, buf, buflen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }

  pMsg->Id = eCRYPTc_PRFResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_verifyCertificateChain function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_verifyCertificateChain, and "result"
 * is the return value, as defined in aapicrpt.h.
 */
void
CRYPTc_verifyCertificateChainResponse (UINT16 id, INT16 result)
{
  Msg *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int bSendMsg;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_verifyCertificateChainResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;

  pMsg->Id = eCRYPTc_verifyCertificateChainResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_keyExchange function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_keyExchange, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The other parameters hold the master secret ID, and possibly
 * a public value to be sent to the server as part of the key
 * key exchange process.
 */
void
CRYPTc_keyExchangeResponse (UINT16 id, INT16 result, UINT8 masterSecretID,
                            const BYTE *publicValue, UINT16 publicValueLen)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_keyExchangeResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT8[0] = masterSecretID;
  pMsg->vUINT16[1] = publicValueLen;
  if (publicValue != NULL) {
    pc = (CHAR*)OSConnectorAlloc (publicValueLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, publicValue, publicValueLen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }

  pMsg->Id = eCRYPTc_keyExchangeResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_getClientCertificate function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_getClientCertificate, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The parameter "keyId" should hold a byte-encoded Identifier value
 * that can be used to identify the client private key associated with
 * the certificate(s) contained in the buffer "cert".
 * If no suitable certificate was available, "result" should
 * be set to CRV_MISSING_CERTIFICATE.
 */
void
CRYPTc_getClientCertificateResponse (UINT16 id, INT16 result,
                                     const BYTE *keyId, UINT16 keyIdLen,
                                     const BYTE *cert, UINT16 certLen)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_getClientCertificateResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT16[1] = keyIdLen;
  pMsg->vUINT16[2] = certLen;
  if (keyId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (keyIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, keyId, keyIdLen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }
  if (cert != NULL) {
    pc = (CHAR*)OSConnectorAlloc (certLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, cert, certLen);
    pMsg->pCHAR[1] = pc;
  }
  else {
    pMsg->pCHAR[1] = NULL;
  }

  pMsg->Id = eCRYPTc_getClientCertificateResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_computeSignature function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_computeSignature, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The computed signature is in the buffer "sig".
 * If the requested private key was not available, the result
 * should be set to "CRV_MISSING_KEY".
 */
void
CRYPTc_computeSignatureResponse (UINT16 id, INT16 result,
                                 const BYTE *sig, UINT16 sigLen)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_computeSignatureResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT16[2] = sigLen;
  if (sig != NULL) {
    pc = (CHAR*)OSConnectorAlloc (sigLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, sig, sigLen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }

  pMsg->Id = eCRYPTc_computeSignatureResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_peerLookup function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_peerLookup, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The result is a master secret index. If the requested
 * peer was not found the result should be set to "CRV_NOT_FOUND".
 */
void
CRYPTc_peerLookupResponse (UINT16 id, INT16 result,
                           UINT8 masterSecretIndex)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_peerLookupResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT8[0] = masterSecretIndex;

  pMsg->Id = eCRYPTc_peerLookupResponse;
  bSendMsg = SendMsg (pMsg);
}

/*
 * Deliver the result from the CRYPTa_sessionFetch function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_sessionFetch, and "result"
 * is the return value, as defined in aapicrpt.h.
 * If the requested session slot was not found, the result
 * should be set to "CRV_NOT_FOUND". Otherwise, the values
 * found in the session slot are passed in the remaining parameters.
 */
void
CRYPTc_sessionFetchResponse (UINT16 id, INT16 result,
                             UINT8 sessionOptions,
                             BYTE *sessionId, UINT8 sessionIdLen,
                             UINT8 macAlg,
                             UINT8 cipherAlg,
                             UINT8 compressionAlg,
                             BYTE *privateKeyId,
                             UINT32 creationTime)
{
  Msg  *pMsg = (Msg*)OSConnectorAlloc (sizeof (Msg));
  int  bSendMsg;
  CHAR *pc;

  if (pMsg == NULL) {
    return;
  }

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_sessionFetchResponse: %d\n", result);
#endif

  pMsg->vUINT16[0] = id;
  pMsg->vINT16 = result;
  pMsg->vUINT8[0] = sessionOptions;
  if (sessionId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (sessionIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, sessionId, sessionIdLen);
    pMsg->pCHAR[0] = pc;
  }
  else {
    pMsg->pCHAR[0] = NULL;
  }
  pMsg->vUINT8[1] = sessionIdLen;
  pMsg->vUINT8[2] = macAlg;
  pMsg->vUINT8[3] = cipherAlg;
  pMsg->vUINT8[4] = compressionAlg;
  if (privateKeyId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (4);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, privateKeyId, 4);
    pMsg->pCHAR[1] = pc;
  }
  else {
    pMsg->pCHAR[1] = NULL;
  }
  pMsg->vUINT32[0] = creationTime;

  pMsg->Id = eCRYPTc_sessionFetchResponse;
  bSendMsg = SendMsg (pMsg);
}
