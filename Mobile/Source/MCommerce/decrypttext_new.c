//--------------------------------
//
//	decrypttext.c
//
//	decrypt WapEnvelopedData
//
//	2000. 12 by Byung-Jun Yoon
//
//--------------------------------

#include <string.h>
#include "rsa.h"
#include "seed.h"
#include "des.h"

#ifdef BLOCK
#include "nx_key.h"
#endif //#ifdef BLOCK


#include "script_enum.h"
#include "encrypttext.h"


#define READ_FROM_BUF( source, target, length, offset ) \
            { memcpy(target, &source[offset], length); offset += length; }


RSA * getServerPrivateKey(unsigned char * keyId, int nKeyIdLen)
{
#ifdef BLOCK
	RSA * rsa = NULL;
	FILE	* fp=NULL;
	char	* pass=NULL;
	unsigned char * buf = nx_key;

	if(!d2i_RSAPrivateKey(&rsa, &buf, sizeof(nx_key))){
		return NULL;
	}

	return rsa;
#endif //#ifdef BLOCK
}

#ifdef BLOCK
unsigned char * decryptText( const unsigned char * envelope, int nEnvLen, int * length)
{
	RSA * rsa = NULL;
	unsigned char * str = NULL;
	int nVersion, nKeyAlgo, nEncAlgo;
	int nTemp, nOffset, nKeyIdLen, nEncKeyLen, nEncDataLen;
	unsigned char ivBuf[16];
	unsigned char *keyId=NULL, *encKey=NULL, *encData=NULL;

	if(envelope==NULL || nEnvLen==0){
		goto err;
	}

	//----------------------------
	//  1. get version
	//----------------------------

	nOffset = 0;
	nVersion = envelope[nOffset++];
	
	//----------------------------
	//  2. get key agreement algo
	//----------------------------

	nKeyAlgo = envelope[nOffset++];
	
	if(nKeyAlgo!=ka_algo_rsa){
		goto err;
	}
	
	//----------------------------
	//  3. get content encryption
	//     algorithm info
	//----------------------------

	nEncAlgo = envelope[nOffset++];		// ContentEncryptAlgorithm
	nTemp = envelope[nOffset++];		// IV length
	if(nTemp!=DES_BLOCK_SIZE && nTemp!=SEED_BLOCK_SIZE){
		goto err;
	}
	READ_FROM_BUF(envelope, ivBuf, nTemp, nOffset);


	//----------------------------
	//  4. get encrypted key info
	//----------------------------

	nKeyIdLen = envelope[nOffset++];

	if((keyId = (unsigned char *) Malloc(nKeyIdLen+1))==NULL){
		goto err;
	}
	READ_FROM_BUF(envelope, keyId, nKeyIdLen, nOffset);
	keyId[nKeyIdLen] = '\0';
/*
	printf("Key ID:%d bytes\n", nKeyIdLen);
	PrintData(keyId, nKeyIdLen);
*/

#if 0
	keyId = &envelope[nOffset];		// remember the position of the
									// recipient key identifier
	nOffset += nKeyIdLen;
#endif

	nEncKeyLen = envelope[nOffset++];
	encKey = (unsigned char *)&envelope[nOffset];	// remember the position
													// of the encrypted key
	nOffset += nEncKeyLen;
	
	//----------------------------
	//  5. get encrypted content
	//----------------------------

	nEncDataLen = envelope[nOffset++];
	nEncDataLen = nEncDataLen << 8;
	nEncDataLen |= envelope[nOffset++];

	encData = (unsigned char *)&envelope[nOffset];	// remember the position
													// of the encrypted data
	nOffset += nEncDataLen;

	if(nOffset!=nEnvLen){
		goto err;
	}

	// allocate memory for the decrypted string

	if((str = (char *)Malloc(nEncDataLen))==NULL){
		goto err;
	}

	//!------------------------------------!
	//! TODO:                              !
	//!                                    !
	//!  get server private key from file  !
	//!                                    !
	//!------------------------------------!

	if((rsa = getServerPrivateKey(keyId, nKeyIdLen))==NULL){
		goto err;
	}

	//-----------------------------
	//
	//	Decrypt Key and Data
	//
	//-----------------------------

	if(nEncAlgo==des_ede3_cbc){
		DES_CBLOCK key1, key2, key3, iv;
		DES_KEY_SCHEDULE ks1, ks2, ks3;
		unsigned char keyBuf[3*DES_BLOCK_SIZE];

		memcpy(iv, ivBuf, DES_BLOCK_SIZE);
		
		if((nTemp=rsaPrivateDecrypt(nEncKeyLen, encKey, keyBuf, rsa, RSA_PKCS1_PADDING))==-1){
			goto err;
		}
		if(nTemp!=sizeof(keyBuf)){
			goto err;
		}

		memcpy(key1, keyBuf, 8);
		memcpy(key2, &keyBuf[8], 8);
		memcpy(key3, &keyBuf[16], 8);

		desSetKey(&key1, ks1);
		desSetKey(&key2, ks2);
		desSetKey(&key3, ks3);

		if((nTemp=desDecryptData(encData, str, nEncDataLen, ks1, ks2, ks3, &iv))==-1){
			goto err;
		}
		str[nTemp] = '\0';
		*length = nTemp;
	
	}
	else{	// seed_cbc
		SEED_CBLOCK	key, iv;
		SEED_KEY_SCHEDULE ks;

		memcpy(iv, ivBuf, SEED_BLOCK_SIZE);

		if((nTemp=rsaPrivateDecrypt(nEncKeyLen, encKey, key, rsa, RSA_PKCS1_PADDING))==-1){
			goto err;
		}
		if(nTemp!=SEED_BLOCK_SIZE){
			goto err;
		}

		seedSetKey(&key, ks);

		if((nTemp=seedDecryptCBC(encData, str, nEncDataLen, ks, iv))==-1){
			goto err;
		}
		str[nTemp] = '\0';
		*length = nTemp;
	}


	// clean up :)


	RSA_free(rsa);
	Free(keyId);


	return str;

err:

	if(str){
		Free(str);
	}
	if(rsa){
		RSA_free(rsa);
	}
	if(keyId){
		Free(keyId);
	}
	
	return NULL;

}


#endif //#ifdef BLOCK









//
//	END of decrypttext.c
//
//-----------------------------
