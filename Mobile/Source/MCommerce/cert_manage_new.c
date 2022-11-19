//------------------------------------
//
//  cert_manage.c
//
//  management of certificates in the
//  mobile terminal
//
//  2000. 11. by Byung-Jun Yoon
//
//------------------------------------

#include "cert_manage.h"

#ifdef BLOCK
#include "ecc_cert.h"
#include "ecc_key.h"
#include "rsa_key.h"
#include "rsa_cert.h"
#endif //#ifdef BLOCK

#include "key_op.h"

//Inserted by swjung for EQUS
#include "sky.h"
#include "ska.h"

#include "clntwallet.h"

//swjung
#define DEFAULT_MY_CERT		0
#define SKID_MY_CERT		1
#define AKID_MY_CERT		2
#define NO_MATCHED_CERT		-1


// get enum value by Key ID
// Key ID can be the subject key id or
// the trusted ca key id

int getEnumByID(int nIdType, const unsigned char * keyID)
{
	int certIndex = NO_MATCHED_CERT; //Initialize index with cert not found

	//It is impossible to search my cert certified by SKT.
	//Because my cert's AKID, hashed public key, is different with SKID,
	//hashed SUBJECT DN.
	//So now we do not consider default keyIdType case.
	if(nIdType == DEFAULT_MY_CERT)
		clnt_search_skt_user_cert((char *)SKY_GET_MW_TCACERT_SUBJECT_KID(0), &certIndex);
	else if(nIdType == AKID_MY_CERT)
		clnt_search_skt_user_cert((char *)keyID, &certIndex);
	else if(nIdType == SKID_MY_CERT)
		clnt_search_my_cert((char *)keyID, &certIndex);
	
	return certIndex;
}


// get the certificate url
char * getCertURL(int nEnum)
{
	unsigned char *pCertURL;
	unsigned char *pLenCertURL;
	int certLength = 0;
 
	pLenCertURL = SKY_GET_MW_MYCERT_CERT_URL(nEnum);
	certLength = strlen((const char *)pLenCertURL);

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	if((pCertURL = (unsigned char *)Malloc(certLength))==NULL){
		return NULL;
	}
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	memcpy(pCertURL, SKY_GET_MW_MYCERT_CERT_URL(nEnum), certLength);
	
	return (char *)pCertURL;
}



// get the private key

ECC * getPrivateKey(int nEnum)
{
    ECC * key = NULL;
    unsigned char *cipherKey = NULL;
    int nCipherKey;
    char *pin = NULL;
    unsigned char pin_hash[20];
    unsigned char *plainKey = NULL;
    int nPlainKey;
 
    cipherKey = SKY_GET_MW_MYCERT_PRV_KEY(nEnum);
    nCipherKey = 96; // ´Ü¸»±â
 
    pin = certPIN;
    SHA1((unsigned char *)pin, 8, pin_hash);
 
    plainKey = key_decrypt( cipherKey, nCipherKey, pin_hash, &nPlainKey );
 
    if(!d2i_ECCPrivateKey(&key, &plainKey, nPlainKey)){
        return NULL;
    }
	return key;
}

//Done
unsigned char * getCertificate(int nEnum, int * nLength)
{
	unsigned char * cert;
	unsigned char * pLenCertURL;
	int certLength = 0;
 

	pLenCertURL = SKY_GET_MW_MYCERT_CERT_URL(nEnum);
	certLength = strlen((const char *)pLenCertURL);

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	if((cert = (unsigned char *)Malloc(certLength))==NULL){
		return NULL;
	}
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	memcpy(cert, SKY_GET_MW_MYCERT_CERT_URL(nEnum), certLength);
	*nLength = certLength;

	return cert;
}
	
//Done.
unsigned char * getKeyID(int nEnum)
{
	unsigned char * keyHash;

	if((keyHash = (unsigned char *)Malloc(20))==NULL){
		return NULL;
	}

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	memcpy(keyHash, (const char *)SKY_GET_MW_MYCERT_SUBJECT_KID(nEnum), 20);
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	return keyHash;
}