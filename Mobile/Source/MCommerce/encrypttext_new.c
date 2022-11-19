//-------------------------------
//
//	encrypttext.c
//
//	encrypt text and create
//	WapEnvelopedData
//
//	2000. 12 by Byung-Jun Yoon
//
//-------------------------------

#include <string.h>
#include <sha.h>
#include <rsa.h>
#include <wcert.h>
#include <seed.h>
#include <des.h>
#include <key_hash.h>
#include <crypto.h>
#include <crypto_size.h>
#include "script_enum.h"
#include "encrypttext.h"


#define CUR_ENCTEXT_VERSION		1


#define WRITE_TO_BUF( target, source, length, offset ) \
            { memcpy(&target[offset], source, length); offset += length; }

/* 2001.09.27
 *  - encryptTextEx 9/27 Spec 적용
 *  - prototype 변경
 */

unsigned char * encryptText( const unsigned char * strToEnc, int nStrLen,
				const unsigned char * cert, int nCertLen, int nPubKeyAlgo,
				const char * strEncAlgo, int * length)
{
	unsigned char * envelope=NULL, * encData=NULL, * encKey=NULL;
	unsigned char ivBuf[16], keyId[20];
	WTLSCert wcert;
	int nEnvLen, nEncDataLen, nEncKeyLen, nIVLen;
	int nOffset=0;
	int nEncAlgo=-1; 
	char * p;
	RSA * rsa = NULL;
	int		ret, certFmt=1;

	// 2001.09.28
	int  defined_errors_n = 5;
	char defined_errors[5][64] = { 
			"error:unsupportedKeyManagementAlgorithm",
			"error:upsupportedContentEncryptionAlgorithm",
			"error:rootCertNotPresent",
			"error:certChainVerificationFailure",
			"error:cancelledByUser",
	};
	char *estr = NULL;
	int i;

	if(strToEnc==NULL || nStrLen==0){
		goto err;
	}

	for(p = (char *)strEncAlgo; *p!='\0'; ++p){
		if(*p==des_ede3_cbc || *p==seed_cbc){
			nEncAlgo = *p;
			break;
		}
	}
	
	if((estr = (char *)Malloc(strlen("error:upsupportedContentEncryptionAlgorithm"+1)))==NULL){
		goto err;
	}

	if(nEncAlgo==-1){
		memcpy(estr, "error:upsupportedContentEncryptionAlgorithm", strlen("error:upsupportedContentEncryptionAlgorithm"));
		goto err;
	}

	if(nPubKeyAlgo!=ka_algo_rsa){
		memcpy(estr, "error:unsupportedKeyManagementAlgorithm", strlen("error:unsupportedKeyManagementAlgorithm"));
		goto err;
	}

	if( (ret = verifyCert(certFmt, (unsigned char *)cert, nCertLen)) != 0 ){
		memcpy(estr, "error:certChainVerificationFailure", strlen("error:certChainVerificationFailure"));
		goto err;
	}

	// Get Recipient Public Key

	if(parseWTLSCert((unsigned char *)cert, nCertLen, &wcert)){
		goto err;
	}

	if((rsa = getRSAPubKey(&wcert))==NULL){
		memcpy(estr, "error:upsupportedPublicKey", strlen("error:upsupportedPublicKey"));
		goto err;
	}

	RSA_KeyHash(keyId, rsa);	// calculate Recipient Key Identifier

	nEncKeyLen = RSA_size(rsa);
	if((encKey = (unsigned char *)Malloc(nEncKeyLen))==NULL){
		goto err;
	}

	//---------------------------------
	//
	//	Encrypt Key and Data
	//
	//---------------------------------

	if(nEncAlgo==des_ede3_cbc){
		DES_CBLOCK key1, key2, key3, iv;
		DES_KEY_SCHEDULE ks1, ks2, ks3;
		unsigned char keyBuf[3*DES_BLOCK_SIZE];

		nEncDataLen = des_enc_block_size_with_padding(nStrLen);
		if((encData = (unsigned char *)Malloc(nEncDataLen))==NULL){
			goto err;
		}
		
		// create random key

		desRandomKey(&key1);
		desSetKey(&key1, ks1);
		desRandomKey(&key2);
		desSetKey(&key2, ks2);
		desRandomKey(&key3);
		desSetKey(&key3, ks3);
		desRandomKey(&iv);
		
		nIVLen = DES_BLOCK_SIZE;
		memcpy(ivBuf, iv, nIVLen);


		// copy key to buffer

		memcpy(keyBuf, key1, 8);
		memcpy(&keyBuf[8], key2, 8);
		memcpy(&keyBuf[16], key3, 8);

		// calculate overall length

		nEnvLen =	1		// version
					+ 1		// KeyAgreementAlgo
					+ 1 + 1 + DES_BLOCK_SIZE	// ContentEncAlgo + iv_len + iv
//					+ 1 + strlen(wcert.subject)	// recipient_key_id_len + id
					+ 1 + SHA_DIGEST_LENGTH		// recipient_key_id_len + id
					+ 1 + nEncKeyLen	// encrypted_key_len + enc_key
					+ 2 + nEncDataLen;	// encrypted_content_len + enc_content

		// encrypt content encryption key

		if(rsaPublicEncrypt(3*DES_BLOCK_SIZE, keyBuf, encKey, rsa, RSA_PKCS1_PADDING)!=nEncKeyLen){
			goto err;
		}

		// encrypt content

		if(desEncryptData(	(unsigned char *)strToEnc, encData, nStrLen,
							ks1, ks2, ks3, &iv) != nEncDataLen){
			goto err;
		}

	}
	else{	// seed_cbc
		SEED_CBLOCK key, iv;
		SEED_KEY_SCHEDULE ks;

		nEncDataLen = seed_enc_block_size_with_padding(nStrLen);
		if((encData = (unsigned char *)Malloc(nEncDataLen))==NULL){
			goto err;
		}
		
		// create random key

		seedRandomKey(&key);
		seedSetKey(&key, ks);
		seedRandomKey(&iv);
		
		nIVLen = SEED_BLOCK_SIZE;
		memcpy(ivBuf, iv, nIVLen);


		// calculate overall length

		nEnvLen =	1		// version
					+ 1		// KeyAgreementAlgo
					+ 1 + 1 + SEED_BLOCK_SIZE	// ContentEncAlgo + iv_len + iv
//					+ 1 + strlen(wcert.subject)	// recipient_key_id_len + id
					+ 1 + SHA_DIGEST_LENGTH		// recipient_key_id_len + id
					+ 1 + nEncKeyLen	// encrypted_key_len + enc_key
					+ 2 + nEncDataLen;	// encrypted_content_len + enc_content

		// encrypt content encryption key

		if(rsaPublicEncrypt(SEED_BLOCK_SIZE, key, encKey, rsa, RSA_PKCS1_PADDING)!=nEncKeyLen){
			goto err;
		}

		// encrypt content

		if(seedEncryptCBC(	(unsigned char *)strToEnc, encData,
							nStrLen, ks, iv) != nEncDataLen ){
			goto err;
		}

	}

	// Make WapEnvelopedData

	if((envelope = (unsigned char *)Malloc(nEnvLen))==NULL){
		goto err;
	}

	//----------------------------
	//	create WapEnvelopedData
	//----------------------------

	nOffset = 0;

	//----------------------------
	//	1. version
	//----------------------------

	envelope[nOffset++] = CUR_ENCTEXT_VERSION;
	
	//----------------------------
	//	2. KeyAgreementAlg
	//----------------------------

	envelope[nOffset++] = ka_algo_rsa;
	
	//----------------------------
	//	3. ContentEncryptAlgoInfo
	//----------------------------

	envelope[nOffset++] = nEncAlgo;
	envelope[nOffset++] = nIVLen;

	WRITE_TO_BUF(envelope, ivBuf, nIVLen, nOffset);
	
	//----------------------------
	//	4. EncryptedKeyInfo
	//----------------------------

	/*
	if((nTemp = strlen(wcert.subject))>255){
		goto err;
	}
	envelope[nOffset++] = nTemp;
	WRITE_TO_BUF(envelope, wcert.subject, nTemp, nOffset);
	*/
	
	envelope[nOffset++] = SHA_DIGEST_LENGTH;
	WRITE_TO_BUF(envelope, keyId, SHA_DIGEST_LENGTH, nOffset);

	envelope[nOffset++] = nEncKeyLen;
	WRITE_TO_BUF(envelope, encKey, nEncKeyLen, nOffset);
	
	//----------------------------
	//	5. Encrypted Content
	//----------------------------

	envelope[nOffset++] = nEncDataLen >> 8;
	envelope[nOffset++] = nEncDataLen;

	WRITE_TO_BUF(envelope, encData, nEncDataLen, nOffset);


	//----------------------------
	//	WapEnvelopedData Created
	//----------------------------


	if(nOffset!=nEnvLen){
		goto err;
	}


	// clean up :)

	Free(encData);
	Free(encKey);
	RSA_free(rsa);
	
	*length = nOffset;

	return envelope;

err:

	if(envelope){
		Free(envelope);
	}
	if(encData){
		Free(encData);
	}
	if(encKey){
		Free(encKey);
	}
	if(rsa){
		RSA_free(rsa);
	}

	for( i=0 ; i<defined_errors_n ; i++ ){
        if( !memcmp( estr, defined_errors[i], strlen(defined_errors[i])) )
            return (unsigned char *)estr;
    }
    memcpy( estr, "invalid", strlen("invalid")+1);

    return (unsigned char *)estr;
}

//
//	End of encrypttext.c
//
//-------------------------
