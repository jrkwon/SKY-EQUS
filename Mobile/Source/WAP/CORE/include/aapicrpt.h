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
 * aapicrpt.h
 *
 * Created by Anders Edenbrandt, Thu Sep 09 14:01:33 1999.
 *
 * Revision history:
 *   000929, AED: added adaptor functions for session cache
 *
 */
#ifndef _aapicrpt_h
#define _aapicrpt_h

/* To define basic types, BYTE, UINT16, etc. */
#include "tapicmmn.h"


/***********************************************************************
 * Type and constant definitions
 ***********************************************************************/

/*---------------------------------------------
 * Bulk encryption
 *---------------------------------------------*/

typedef UINT8 BulkCipherAlgorithm;

/* The bulk encryption algorithms defined by WTLS. */
#define CIPHER_NULL             0
#define CIPHER_RC5_CBC_40       1
#define CIPHER_RC5_CBC_56       2
#define CIPHER_RC5_CBC          3
#define CIPHER_DES_CBC_40       4
#define CIPHER_DES_CBC          5
#define CIPHER_3DES_CBC_EDE     6
#define CIPHER_IDEA_CBC_40      7
#define CIPHER_IDEA_CBC_56      8
#define CIPHER_IDEA_CBC         9
#define CIPHER_RC5_CBC_64      10
#define CIPHER_IDEA_CBC_64     11

/* Key parameter object for bulk encryption routines. */
typedef struct {
  BYTE   *key;
  UINT16 keyLen;
  BYTE   *iv;
  UINT16 ivLen;
} KeyObject;


/*---------------------------------------------
 * Secure hash algorithms
 *---------------------------------------------*/

typedef UINT8 HashAlgorithm;

/* The secure hash algorithms that can be used in WTLS. */
#define HASH_SHA              1
#define HASH_MD5              2

typedef void *HashHandle;


/************************************************************
 * Cipher Method
 ************************************************************/

typedef struct {
  BulkCipherAlgorithm bulkCipherAlg;
  HashAlgorithm       hashAlg;
} CipherMethod;


/*---------------------------------------------
 * Key exchange
 *---------------------------------------------*/

typedef UINT8 KeyExchangeSuite;

/* The key exchange suites defined by WTLS. */
#define KEY_EXCH_NULL           0
#define KEY_EXCH_SHARED_SECRET  1
#define KEY_EXCH_DH_ANON        2
#define KEY_EXCH_DH_ANON_512    3
#define KEY_EXCH_DH_ANON_768    4
#define KEY_EXCH_RSA_ANON       5
#define KEY_EXCH_RSA_ANON_512   6
#define KEY_EXCH_RSA_ANON_768   7
#define KEY_EXCH_RSA            8
#define KEY_EXCH_RSA_512        9
#define KEY_EXCH_RSA_768        10
#define KEY_EXCH_ECDH_ANON      11
#define KEY_EXCH_ECDH_ANON_113  12
#define KEY_EXCH_ECDH_ANON_131  13
#define KEY_EXCH_ECDH_ECDSA     14


/*---------------------------------------------
 * Parameter to key agreement routines
 *---------------------------------------------*/

typedef struct {
  UINT16 expLen;
  BYTE   *exponent;
  UINT16 modLen;
  BYTE   *modulus;
} PublicKey_RSA;

typedef struct {
  UINT16 len;
  BYTE   *y;
} PublicKey_DH;

typedef struct {
  UINT8 len;
  BYTE *point;
} PublicKey_EC;

typedef struct {
  union {
    PublicKey_RSA rsa_key;
    PublicKey_DH  dh_key;
    PublicKey_EC  ec_key;
  } _u;
} PublicKey;

typedef struct {
  BYTE   parameterIndex;
  UINT16 paramLen;
  BYTE   *params;
} ParameterSpecifier;

typedef struct {
  KeyExchangeSuite       keyExchangeSuite;
  union {
    struct {
      BYTE   *identifier;
      UINT16 idLen;
    } secretKey;
    struct {
      PublicKey          pubKey;
      ParameterSpecifier parameterSpecifier;
    } keyParam;
    struct {
      UINT16 bufLen;
      BYTE   *buf;
    } certificates;
  } _u;
} KeyExchangeParameters;

/***********************************************************************
 * Function return values
 *
 * Each function in this crypto library returns an integer
 * value. Successful return is indicated by the value CRV_OK.
 * Other values indicate some sort of failure or problem.
 * The Generic WAP client treats all failures as equal, i.e.,
 * a certain library function that fails will be handled in
 * a uniform way regardless of the actual reason for failure.
 * However, the return code is logged with CLNTc_log when
 * LOG_EXTERNAL is defined.
 * It is recommended that the following constants be used
 * as return values for the Adapter functions of this API.
 ***********************************************************************/

#define CRV_OK                                       0
#define CRV_GENERAL_ERROR                            1
#define CRV_BUFFER_TOO_SMALL                         2
#define CRV_UNSUPPORTED_METHOD                       3
#define CRV_ALREADY_INITIALISED                      4
#define CRV_INSUFFICIENT_MEMORY                      5
#define CRV_CRYPTOLIB_NOT_INITIALISED                6
#define CRV_KEY_TOO_LONG                             7
#define CRV_NOT_IMPLEMENTED                          8
#define CRV_INVALID_PARAMETER                        9
#define CRV_DATA_LENGTH                             10
#define CRV_INVALID_KEY                             11
#define CRV_INVALID_HANDLE                          12
#define CRV_KEY_LENGTH                              13
#define CRV_MISSING_KEY                             14
#define CRV_UNKNOWN_CERTIFICATE_TYPE                15
#define CRV_NO_MATCHING_ROOT_CERTIFICATE            16
#define CRV_BAD_CERTIFICATE                         17
#define CRV_CERTIFICATE_EXPIRED                     18
#define CRV_MISSING_CERTIFICATE                     19
#define CRV_CONFIG_ERROR                            20
#define CRV_NOT_FOUND                               21


/***********************************************************************
 * General functions
 ***********************************************************************/

/*
 * Perform necessary initialization chores, for example, seed
 * the random number generator.
 * This function must be called once before any other functions
 * in the crypto library are used.
 */
INT16
CRYPTa_initialise (VOID);

/*
 * Terminate the use of the crypto library. This function is called
 * when WTLS is being shut down. Current handles held by the user
 * (e.g., HashHandle and MasterSecretID) are no longer valid
 * after calling this function.
 */
INT16
CRYPTa_terminate (void);

/***********************************************************************
 * Crypto Methods
 ***********************************************************************/

/*
 * Retrieve lists of crypto parameters that the crypto library supports.
 * This includes cipher methods, key exchange methods, and trusted
 * certificates.
 * The response should be delivered by a call to the connector function
 * CRYPTc_getMethodsResponse. The parameter, "id",
 * should be passed back in this response.
 */
void
CRYPTa_getMethods (UINT16 id);


/***********************************************************************
 * Bulk encryption algorithms
 ***********************************************************************/

/*
 * Encrypt single-part data.
 * For some encryption methods, the input plaintext data has certain
 * length constraints. If these constraints are not satisfied, then
 * CRYPTa_encrypt will fail with return code CRV_DATA_LENGTH.
 * The plaintext and ciphertext can be in the same place, i.e., it is
 * OK if data and encryptedData point to the same location.
 * The ciphertext is always of the same length as the plaintext.
 */
INT16
CRYPTa_encrypt (BulkCipherAlgorithm method,
                KeyObject key,
                BYTE *data, UINT16 dataLen,
                BYTE *encryptedData);

/*
 * Decrypt single-part data.
 * For some decryption methods, the input ciphertext has certain
 * length constraints. If these constraints are not satisfied, then
 * CRYPTa_decrypt will fail with return code CRV_DATA_LENGTH.
 * The ciphertext and plaintext can be in the same place, i.e., it is
 * OK if data and decryptedData point to the same location.
 * The ciphertext is always of the same length as the plaintext.
 */
INT16
CRYPTa_decrypt (BulkCipherAlgorithm method,
                KeyObject key,
                BYTE *data, UINT16 dataLen,
                BYTE *decryptedData);


/***********************************************************************
 * Secure hash algorithms
 ***********************************************************************/

/*
 * Compute a hash digest of given single-part data.
 * The input data and digest output can be in the same place, i.e.,
 * it is OK if "data" and "digest" point to the same location.
 * CRYPTa_hash is equivalent to a call to CRYPTa_hashInit, followed
 * by a sequence of CRYPTa_hashUpdate operations, and terminated
 * by a call to CRYPTa_hashFinal.
 */
INT16
CRYPTa_hash (HashAlgorithm alg,
             BYTE *data, UINT16 dataLen,
             BYTE *digest);

/*
 * Initialize a hash operation.
 * "alg" is the hash algorithm to use, and on return "handleptr"
 * will point to a new handle to be used in subsequent operations.
 * After calling CRYPTa_hashInit, one calls CRYPTa_hashUpdate zero or
 * more times, followed by CRYPTa_hashFinal, to digest data in
 * multiple parts. The hash operation is active until one makes
 * a call to CRYPTa_hashFinal to actually obtain
 * the final piece of ciphertext. To process additional data
 * (in single or multiple parts), one must call CRYPTa_hashInit again.
 */
INT16
CRYPTa_hashInit (HashAlgorithm alg,
                 HashHandle *handleptr);

/*
 * Continue a multiple-part hash operation, processing another data part.
 * "handle" is the handle of the hash operation, "part" points to the
 * data part, and "partLen" is the length of the data part.
 * The hash operation must have been initialized with CRYPTa_hashInit.
 * A call to CRYPTa_hashUpdate which results in an error terminates
 * the current hash operation.
 */
INT16
CRYPTa_hashUpdate (HashHandle handle,
                   BYTE *part, UINT16 partLen);

/*
 * Finish a multiple-part hash operation.
 * "handle" is the handle of the hash operation, and "digest" is the location
 * that receives the computed message digest.
 * The hash operation must have been initialized with CRYPTa_hashInit.
 * A call to CRYPTa_hashFinal always terminates the active hash operation.
 */
INT16
CRYPTa_hashFinal (HashHandle handle,
                  BYTE *digest);


/***********************************************************************
 * Key exchange
 ***********************************************************************/

/*
 * Perform key exchange, using one of the methods previously
 * reported to be available.
 * Derives the master secret using the pre-master secret produced
 * by the key exchange algorithm, and the 32 random bytes
 * (ClientHello.random + ServerHello.random) supplied
 * as a parameter to this call. The master secret is kept internally in
 * the crypto library, to be used in subsequent operations.
 * The parameter "alg" is the secure hash algorithm to be used.
 * The response should be delivered by a call to the connector function
 * CRYPTc_keyExchangeResponse. The first parameter, "id",
 * should be passed back in this response.
 *
 * RSA:
 * The public key to use is either given explicitly in the parameters,
 * or must be retrieved from a certificate passed to this routine.
 * This function generates a 20-byte value consisting of the given
 * "additionalData" and 19 random bytes. It encrypts the value with
 * the given public key, and stores the result in publicValue.
 * The pre-master secret is the 20-byte value appended with the
 * given public key.
 *
 * Diffie-Hellman: calculate a pre-master secret and a public value
 * to be sent to the server side.
 * The public key to use is given explicitly in the parameters.
 * This function performs a DH calculation based on the given
 * public key and a private key kept in the crypto library.
 *
 * ECDH: calculate a pre-master secret and a public value
 * to be sent to the server side.
 * The public key to use is either given explicitly in the parameters,
 * or must be retrieved from a certificate passed to this routine.
 * This function performs an ECDH calculation based on the given
 * public key and a private key kept in the crypto library.
 *
 * Shared secret: set the pre-master secret to be a pre-defined secret key.
 * Note, that if the library attempts to support the Shared Secret
 * key exchange method, it is required that the routine
 * CRYPTa_getClientKeyExchangeIds pair the method indicator
 * (KEY_EXCH_SHARED_SECRET) with a suitable identifier. That is,
 * an identifier that the gateway can use to retrieve the correct
 * secret key. "identifier" is a copy of that same identifying string.
 * The Crypto Library uses this parameter to select the appropriate
 * secret key.
 *
 * Null: set the pre-master secret to be the empty string.
 *
 */
void
CRYPTa_keyExchange (UINT16 id, KeyExchangeParameters param,
                    HashAlgorithm alg,
                    const BYTE *randval);

/***********************************************************************
 * Key generation
 ***********************************************************************/

/*
 * Calculate the Pseudo-Random Function defined in section
 * 11.3.2 in the WTLS spec.
 * If the master secret is to be used as first parameter,
 * then "secret" is NULL, and "masterSecretID" indicates which
 * master secret to use. Otherwise, "secret" is provided explicitly.
 * The parameter "label" is a null-terminated character string.
 * The parameter "alg" is the secure hash algorithm to be used,
 * and "outputLen" is the number of bytes of output to produce.
 * The response should be delivered by a call to the connector function
 * CRYPTc_PRFResponse. The first parameter, "id", should be
 * passed back in this response.
 */
void
CRYPTa_PRF (UINT16 id, HashAlgorithm alg,
            UINT8 masterSecretID,
            BYTE *secret, UINT16 seclen,
            BYTE *label,
            BYTE *seed, UINT16 seedLen,
            UINT16 outputLen);


/***********************************************************************
 * Certificates
 ***********************************************************************/

/*
 * Verify a chain of certificates.
 * The buffer, "buf", contains a chain of certificates. See section
 * 10.5.2 in the WTLS specification.
 * The response should be delivered by a call to the connector function
 * CRYPTc_verifyCertificateChainResponse. The first parameter, "id",
 * should be passed back in this response.
 */
void
CRYPTa_verifyCertificateChain (UINT16 id, const BYTE *buf, UINT16 bufLen);

/*
 * Retrieve a client certificate.
 * The buffer, "buf", contains a list of acceptable certificate
 * authorities, as byte encoded KeyExchangeId:s. See section 10.5.4
 * in the WTLS specification.
 * The response should be delivered by a call to the connector function
 * CRYPTc_getClientCertificateResponse. The first parameter, "id",
 * should be passed back in this response.
 */
void
CRYPTa_getClientCertificate (UINT16 id, const BYTE *buf, UINT16 bufLen);

/*
 * Compute a digital signature.
 * The buffer, "buf", contains the data to be signed, and the key
 * identifier, "keyId", contains a byte-encoded Identifier.
 * This latter value is either fetched from the Key Exchange Ids
 * passed back in CRYPTc_getMethodsResponse, or is the value
 * passed back in CRYPTc_getClientCertificateResponse (provided that
 * this function has been used). The keyId value indicates
 * which key must be used for signing.
 * The response should be delivered by a call to the connector function
 * CRYPTc_computeSignatureResponse. The first parameter, "id",
 * should be passed back in this response.
 */
void
CRYPTa_computeSignature (UINT16 id,
                         const BYTE *keyId, UINT16 keyIdLen,
                         const BYTE *buf, UINT16 bufLen);

/***********************************************************************
 * Random number generation
 ***********************************************************************/

/*
 * Generate random (or pseudo-random) data. "randomData" points to
 * the location that receives the data and "randomLen" is the number
 * of bytes of data to be generated.
 */
INT16
CRYPTa_generateRandom (BYTE *randomData, UINT16 randomLen);


/***********************************************************************
 * Session cache
 ***********************************************************************/

/*
 * General assumptions about the handling of the session store:
 *
 * The master secret store should be maintained in parallell with
 * the session store. This means, that the same indexes or slot
 * numbers are used both in the master secret store and the session store.
 * New slots in the master secret store are selected by the
 * key exchange routine. If no empty slots are available, the
 * old slot to be evicted should be selected using the following
 * strategy: Select a slot where the session is
 *   1) NON-resumable and NOT active
 *   2) resumable and NOT active
 *   3) NON-resumable and active
 *   4) resumable and active
 *
 * Preferably, the size of the store should be large enough that
 * cases 3 and 4 are never used. This can be accomplished by giving
 * the store one slot more than the maximum number of simultaneous
 * WTLS connections. If there are two or more candidates from
 * the same category, they should be ranked according to creation
 * time. That is, the older slot will be reused first.
 *
 * After the key exchange routine has delivered a new master secret
 * index (mid) to WTLS, it is the responsibility of WTLS to call
 * the routines
 *     CRYPTa_sessionInvalidate (mid);
 *     CRYPTa_peerDeleteLinks (mid);
 *
 */


#define SESSION_OPTIONS_RESUMABLE       0x80
#define SESSION_OPTIONS_SERVER_AUTH     0x20
#define SESSION_OPTIONS_CLIENT_AUTH     0x10


/*
 * Initialize the session cache.
 */
void
CRYPTa_sessionInit (void);

/*
 * Close the session cache.
 */
void
CRYPTa_sessionClose (void);

/************************************************************
 * 'Peers' methods
 ************************************************************/

/*
 * Delete all peer entries that link to the indicated
 * master secret.
 */
void
CRYPTa_peerDeleteLinks (UINT8 masterSecretIndex);

/*
 * Add a peer entry that links to the given master secret.
 * If such an entry already exists, overwite it.
 */
void
CRYPTa_peerLinkToSession (BYTE *address, UINT16 addrlen,
                          UINT16 portnum,
                          UINT8 masterSecretIndex);
/*
 * Find a peer with matching address and port number.
 * If none exists, try matching just the address.
 * The result, a master secret index, is returned in
 * CRYPTc_peerLookupResponse.
 */
void
CRYPTa_peerLookup (UINT16 id,
                   BYTE *address, UINT16 addrlen,
                   UINT16 portnum);

/************************************************************
 * 'Sessions' methods
 ************************************************************/

/*
 * If "isActive" != 0, then mark the indicated session slot
 * as being "active". Otherwise, mark it as not active.
 * A session slot that is "active", SHOULD NOT be reused.
 * WTLS will keep a session marked as "active" as long as
 * any WTLS connection that is based upon that session is active.
 */
void
CRYPTa_sessionActive (UINT8 masterSecretIndex, UINT8 isActive);

/*
 * Mark a session entry as non-resumable.
 */
void
CRYPTa_sessionInvalidate (UINT8 masterSecretIndex);

/*
 * Mark all entries in 'Sessions' as non-resumable.
 */
void
CRYPTa_sessionClear (void);

/*
 * Fetch the contents of the indicated session entry.
 * The result is returned in CRYPTc_sessionFetchResponse.
 */
void
CRYPTa_sessionFetch (UINT16 id, UINT8 masterSecretIndex);

/*
 * Store new values for a session entry.
 */
void
CRYPTa_sessionUpdate (UINT8 masterSecretIndex,
                      UINT8 sessionOptions,
                      BYTE *sessionId, UINT8 sessionIdLen,
                      UINT8 macAlg,
                      UINT8 cipherAlg,
                      UINT8 compressionAlg,
                      BYTE *privateKeyId,
                      UINT32 creationTime);

#endif
