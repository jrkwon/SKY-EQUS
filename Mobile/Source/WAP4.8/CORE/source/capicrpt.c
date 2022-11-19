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
#include "ml_typw.h"
#include "wap.ifc"
#include "mk_queu.h"
#ifdef LOG_EXTERNAL
#include "aapiclnt.h"
#endif

/*
 * Deliver the result from the CRYPTa_initialise function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_initialise, and "result"
 * is the return value, as defined in aapicrpt.h.
 */
void
CRYPTc_initialiseResponse (UINT16 id, INT16 result)
{
	XMK_SEND_TMP_VARS

    yPDef_CRYPTcInitialiseResponse var;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_initialiseResponse: %d\n", result);
#endif

    var.Param1 = id;
    var.Param2 = result;

    XMK_SEND_ENV( ENV,
                  CRYPTcInitialiseResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_CRYPTcInitialiseResponse ),
                  &var,
                  GLOBALPID(XPTID_StackCtrl,0));
}

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
	XMK_SEND_TMP_VARS

  yPDef_GetMethodsResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_getMethodsResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param4 = cipherMethodLen;
  var.Param6 = keyExchangeIdLen;
  var.Param8 = trustedKeyIdLen;

  if (cipherMethods != NULL) {
    pc = (CHAR *)OSConnectorAlloc (cipherMethodLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, cipherMethods, cipherMethodLen);
    var.Param3 = pc;
  }
  else {
    var.Param3 = NULL;
  }

  if (keyExchangeIds != NULL) {
    pc = (CHAR *)OSConnectorAlloc (keyExchangeIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, keyExchangeIds, keyExchangeIdLen);
    var.Param5 = pc;
  }
  else {
    var.Param5 = NULL;
  }

  if (trustedKeyIds != NULL) {
    pc = (CHAR *)OSConnectorAlloc (trustedKeyIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, trustedKeyIds, trustedKeyIdLen);
    var.Param7 = pc;
  }
  else {
    var.Param7 = NULL;
  }

   XMK_SEND_ENV( ENV,
                  GetMethodsResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetMethodsResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_PRFResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_PRFResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param4 = buflen;
  if (buf != NULL) {
    pc = (CHAR*)OSConnectorAlloc (buflen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, buf, buflen);
    var.Param3 = pc;
  }
  else {
    var.Param3 = NULL;
  }

    XMK_SEND_ENV( ENV,
                  PRFResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PRFResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_VerifyCertificateChainResponse var;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_verifyCertificateChainResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;

    XMK_SEND_ENV( ENV,
                  VerifyCertificateChainResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_VerifyCertificateChainResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_KeyExchangeResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_keyExchangeResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2  = result;
  var.Param3 = masterSecretID;
  var.Param5 = publicValueLen;
  if (publicValue != NULL) {
    pc = (CHAR*)OSConnectorAlloc (publicValueLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, publicValue, publicValueLen);
    var.Param4 = pc;
  }
  else {
    var.Param4 = NULL;
  }

    XMK_SEND_ENV( ENV,
                  KeyExchangeResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_KeyExchangeResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_GetClientCertificateResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_getClientCertificateResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param4 = keyIdLen;
  var.Param6 = certLen;
  if (keyId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (keyIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, keyId, keyIdLen);
    var.Param3 = pc;
  }
  else {
    var.Param3 = NULL;
  }
  if (cert != NULL) {
    pc = (CHAR*)OSConnectorAlloc (certLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, cert, certLen);
    var.Param5 = pc;
  }
  else {
    var.Param5 = NULL;
  }

    XMK_SEND_ENV( ENV,
                  GetClientCertificateResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_GetClientCertificateResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_ComputeSignatureResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_computeSignatureResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param4 = sigLen;
  if (sig != NULL) {
    pc = (CHAR*)OSConnectorAlloc (sigLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, sig, sigLen);
    var.Param3 = pc;
  }
  else {
    var.Param3 = NULL;
  }

    XMK_SEND_ENV( ENV,
                  ComputeSignatureResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_ComputeSignatureResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_PeerLookupResponse var;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_peerLookupResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param3 = masterSecretIndex;

    XMK_SEND_ENV( ENV,
                  PeerLookupResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_PeerLookupResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
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
	XMK_SEND_TMP_VARS

  yPDef_SessionFetchResponse var;
  CHAR *pc;

#ifdef LOG_EXTERNAL
  CLNTa_log (0, 0, "WTLS: CRYPTc_sessionFetchResponse: %d\n", result);
#endif

  var.Param1 = id;
  var.Param2 = result;
  var.Param3 = sessionOptions;
  if (sessionId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (sessionIdLen);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, sessionId, sessionIdLen);
    var.Param4 = pc;
  }
  else {
    var.Param4 = NULL;
  }
  var.Param5 = sessionIdLen;
  var.Param6 = macAlg;
  var.Param7 = cipherAlg;
  var.Param8 = compressionAlg;
  if (privateKeyId != NULL) {
    pc = (CHAR*)OSConnectorAlloc (4);
    if (pc == NULL) {
      return;
    }
    memcpy (pc, privateKeyId, 4);
    var.Param9 = pc;
  }
  else {
    var.Param9 = NULL;
  }
  var.Param10 = creationTime;

    XMK_SEND_ENV( ENV,
                  SessionFetchResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_SessionFetchResponse ),
                  &var,
                  GLOBALPID(XPTID_WTLS_Main,0));
}
