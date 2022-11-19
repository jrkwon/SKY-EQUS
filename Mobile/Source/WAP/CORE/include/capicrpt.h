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
 * capicrpt.h
 *
 * Created by Anders Edenbrandt, Tue Mar 07 15:14:09 2000.
 *
 * Revision history:
 *   000929, AED: added connector functions for session cache
 *
 */
#ifndef _capicrpt_h
#define _capicrpt_h

/* To define basic types, BYTE, UINT16, etc. */
#include "tapicmmn.h"

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
                           UINT16 trustedKeyIdLen);

/*
 * Deliver the result from the CRYPTa_PRF function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_PRF, and "result"
 * is the return value, as defined in aapicrpt.h.
 * The parameter "buf" holds the computed value.
 */
void
CRYPTc_PRFResponse (UINT16 id, INT16 result,
                    const BYTE *buf, UINT16 buflen);

/*
 * Deliver the result from the CRYPTa_verifyCertificateChain function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_verifyCertificateChain, and "result"
 * is the return value, as defined in aapicrpt.h.
 */
void
CRYPTc_verifyCertificateChainResponse (UINT16 id, INT16 result);

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
CRYPTc_keyExchangeResponse (UINT16 id, INT16 result,
                            UINT8 masterSecretID,
                            const BYTE *publicValue,
                            UINT16 publicValueLen);

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
                                     const BYTE *cert, UINT16 certLen);

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
                                 const BYTE *sig, UINT16 sigLen);


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
                           UINT8 masterSecretIndex);

/*
 * Deliver the result from the CRYPTa_sessionFetch function.
 * The first parameter, "id", should hold the same value that
 * was passed in the call to CRYPTa_sessionFetch, and "result"
 * is the return value, as defined in aapicrpt.h.
 * If the requested session slot was not found (or empty),
 * the result should be set to "CRV_NOT_FOUND". Otherwise, the values
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
                             UINT32 creationTime);

#endif
