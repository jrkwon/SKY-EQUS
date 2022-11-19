#ifndef SCRIPT__H
#define SCRIPT__H

#ifdef  __cplusplus
extern "C" {
#endif

//----------------------------
//
//	script.h
//
//	by Byung-Jun Yoon
//
//----------------------------


#define	WPKI_CRYPT_ALGO_DES			0	
#define WPKI_CRYPT_ALGO_SEED		1

#define WPKI_HASH_ALGO_MD5			0
#define WPKI_HASH_ALGO_SHA1			1

#define WPKI_CRYPT_ENCRYPT			0
#define WPKI_CRYPT_DECRYPT			1

#define WPKI_CERT_GET_CERT			0
#define WPKI_CERT_GET_URL			1

#define WPKI_MAX_SKEY_ENC_LEN		150
#define WPKI_MAX_PKEY_ENC_LEN		192
#define WPKI_MAX_CSR_BUF_LEN		192
#define WPKI_MAX_PUBKEY_BUF_LEN		160
#define WPKI_MAX_PRIVKEY_BUF_LEN	620

char * pkEncryptData(const char * input, const char * cert);

char * pkSignData(const char * input, int nAlgo);

char * createCSR(const char * dn);


#ifdef  __cplusplus
}
#endif

#endif	//	SCRIPT__H

