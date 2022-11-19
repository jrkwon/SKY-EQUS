/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     clntPIM.c
**
** Description: 
**     Phonebook library Function 
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-02-19 ytchoi     Created.
**
** ===========================================================================
*/
/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sky.h"
#include "ska.h"
#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif
#include "clnt.h"
#include "clntapp.h"
#include "aapicrpt.h"



/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/*EQS_API*/


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
void CRYPTa_initialise (UINT16 id)
{
	CLNTa_log(1,0,"DUMMY CRYPTa_initialise");
    CRYPTc_initialiseResponse (id, CRV_OK);    
}

INT16 CRYPTa_terminate (void)
{
	CLNTa_log(1,0,"DUMMY CRYPTa_terminate");
	return 0;
}

void CRYPTa_sessionInvalidate (UINT8 masterSecretIndex)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_sessionInvalidate");
}

void CRYPTa_peerDeleteLinks (UINT8 masterSecretIndex)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_peerDeleteLinks");
}

void CRYPTa_sessionUpdate (UINT8 masterSecretIndex,
                      UINT8 sessionOptions,
                      BYTE *sessionId, UINT8 sessionIdLen,
                      UINT8 macAlg,
                      UINT8 cipherAlg,
                      UINT8 compressionAlg,
                      BYTE *privateKeyId,
                      UINT32 creationTime)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_sessionUpdate");
}

void CRYPTa_peerLinkToSession (BYTE *address, UINT16 addrlen,
                          UINT16 portnum,
                          UINT8 masterSecretIndex)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_peerLinkToSession");
}

void CRYPTa_sessionActive (UINT8 masterSecretIndex, UINT8 isActive)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_sessionActive");
}

INT16 CRYPTa_generateRandom (BYTE *randomData, UINT16 randomLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_generateRandom");
    return 0;
}

void CRYPTa_sessionFetch (UINT16 id, UINT8 masterSecretIndex)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_sessionFetch");
}

void CRYPTa_peerLookup (UINT16 id,
                   BYTE *address, UINT16 addrlen,
                   UINT16 portnum)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_peerLookup");
}

void CRYPTa_getMethods (UINT16 id)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_getMethods");
}

void CRYPTa_getClientCertificate (UINT16 id, const BYTE *buf, UINT16 bufLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_getClientCertificate");
}

void CRYPTa_computeSignature (UINT16 id,
                         const BYTE *keyId, UINT16 keyIdLen,
                         const BYTE *buf, UINT16 bufLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_computeSignature");
}

void CRYPTa_verifyCertificateChain (UINT16 id,
                               const BYTE *buf, UINT16 bufLen,
                               const BYTE *addr, UINT16 addrLen,
                               const CHAR *domain, UINT16 domainLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_verifyCertificateChain");
}

void CRYPTa_PRF (UINT16 id, HashAlgorithm alg,
            UINT8 masterSecretID,
            BYTE *secret, UINT16 seclen,
            BYTE *label,
            BYTE *seed, UINT16 seedLen,
            UINT16 outputLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_PRF");
}

INT16 CRYPTa_encrypt (BulkCipherAlgorithm method,
                KeyObject key,
                BYTE *data, UINT16 dataLen,
                BYTE *encryptedData)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_encrypt");
    return 0;
}

INT16 CRYPTa_decrypt (BulkCipherAlgorithm method,
                KeyObject key,
                BYTE *data, UINT16 dataLen,
                BYTE *decryptedData)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_decrypt");
    return 0;
}

INT16 CRYPTa_hash (HashAlgorithm alg,
             BYTE *data, UINT16 dataLen,
             BYTE *digest)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_hash");
    return 0;
}

INT16 CRYPTa_hashInit (HashAlgorithm alg,
                 HashHandle *handleptr)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_hashInit");
    return 0;
}

INT16 CRYPTa_hashUpdate (HashHandle handle,
                   BYTE *part, UINT16 partLen)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_hashUpdate");
    return 0;
}

INT16 CRYPTa_hashFinal (HashHandle handle,
                  BYTE *digest)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_hashFinal");
    return 0;
}

void CRYPTa_keyExchange (UINT16 id, KeyExchangeParameters param,
                    HashAlgorithm alg,
                    const BYTE *randval)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_keyExchange");    
}

void CRYPTa_connectionEstablished (UINT8 objectId, UINT8 channelId,UINT8 masterSecretId)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_connectionEstablished");    
}

void CRYPTa_connectionTerminated (UINT8 objectId, UINT8 channelId)
{
    CLNTa_log(1,0,"DUMMY CRYPTa_connectionTerminated");    
}


