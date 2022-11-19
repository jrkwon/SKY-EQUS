//---------------------------------
//
//	rsa_verify.c
//
//	by Byung-Jun Yoon
//
//---------------------------------


#include "rsa.h"
#include "md5.h"
#include "sha.h"

int RSA_sign(int type, unsigned char *m, unsigned int m_len,
		unsigned char *sigret, unsigned int *siglen, RSA *rsa)
{
	int nRet;

	switch(type){
	case HASH_ALGO_MD5:
		if(m_len != MD5_DIGEST_LENGTH){
			return 0;
		}
		break;
	case HASH_ALGO_SHA1:
		if(m_len != SHA_DIGEST_LENGTH){
			return 0;
		}
		break;

	default:	// unknown hash algorithm
		return 0;
	}

	// encrypt message digest
	// with the private key

	nRet = RSA_eay_private_encrypt(m_len, m, sigret, rsa, RSA_PKCS1_OAEP_PADDING );

	*siglen = nRet;

	if( nRet == -1 ){
		return 0;
	}
	else{
		return 1;
	}
}


int RSA_verify(int type, unsigned char *m, unsigned int m_len,
		unsigned char *sigbuf, unsigned int siglen, RSA *rsa)
{
	int nRet;
	unsigned char plainSig[256];

	// check Message Digest Algorithm

	switch(type){
	case HASH_ALGO_MD5:
		if(m_len != MD5_DIGEST_LENGTH){
			return 0;
		}
		break;
	case HASH_ALGO_SHA1:
		if(m_len != SHA_DIGEST_LENGTH){
			return 0;
		}
		break;

	default:	// unknown hash algorithm
		return 0;
	}

	// decrypt signature with the publice key

	nRet = RSA_eay_public_decrypt(siglen, sigbuf, plainSig, rsa, RSA_PKCS1_OAEP_PADDING);

	if( nRet < 0){
		return 0;
	}

	if( nRet!= m_len ){	// decrypted signature size is
						// different from the message-digest
		return 0;
	}

	// compare decrypted signature with
	// the message digest

	if( memcmp( m, plainSig, m_len ) != 0 ){
		return 0;
	}

	return 1;
}


int RSA_sign_data(int type, unsigned char *m, unsigned int m_len,
		unsigned char *sigret, unsigned int *siglen, RSA *rsa)

{
//	int nRet;
	unsigned char digest[20];
	unsigned int  digest_len;

	switch(type){
	case HASH_ALGO_MD5:
		MD5(m, m_len, digest);
		digest_len = MD5_DIGEST_LENGTH;
		break;
	case HASH_ALGO_SHA1:
		SHA1(m, m_len, digest);
		digest_len = SHA_DIGEST_LENGTH;
		break;
	default:	// unknown hash algorithm
		break;
	}

	return RSA_sign(type, digest, digest_len, sigret, siglen, rsa);

}

int RSA_verify_data(int type, unsigned char *m, unsigned int m_len,
		unsigned char *sigret, unsigned int siglen, RSA *rsa)

{
	unsigned char digest[20];
	unsigned int  digest_len=0;

	switch(type){
	case HASH_ALGO_MD5:
		MD5(m, m_len, digest);
		digest_len = MD5_DIGEST_LENGTH;
		break;
	case HASH_ALGO_SHA1:
		SHA1(m, m_len, digest);
		digest_len = SHA_DIGEST_LENGTH;
		break;
	default:	// unknown hash algorithm
		break;
	}

	return RSA_verify(type, digest, digest_len, sigret, siglen, rsa);

}


//
// End of rsa_verify.c
//
//------------------------------


