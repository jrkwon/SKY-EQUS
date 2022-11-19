//---------------------------------
//
//	cert_request.c
//
//	create certificate request
//
//	2000. 12. by Byung-Jun Yoon
//
//---------------------------------

#include <string.h>
#include "sha.h"
#include "seed.h"
#include "key_hash.h"
#include "key_op.h"

#ifdef SCRIPT_RSA
	#include "rsa_new.h"
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
	#include "Ecc.h"
	#include "Ecdsa.h"
#endif  // SCRIPT_ECC

#include "script_enum.h"
#include "cert_request.h"
#include "cert_manage.h"
#include "mwdef.h"

#include "ska.h"
#include "aapiclnt.h"
#include "clntwallet.h"



#define CUR_CERTREQ_VERSION		1

#define ALGO_STR_RSA		"rsa"
#define ALGO_STR_ECC		"ecc"


#define WRITE_TO_BUF( target, source, length, offset ) \
            { memcpy(&target[offset], source, length); offset += length; }


static char * revReason[] =
{
	"AN",	// anonymous				(0)
	"KC",	// key_compromise			(1)
	"CA",	// ca_compromise			(2)
	"AC",	// affiliation_changed		(3)
	"SU",	// superseded				(4)
	"CO",	// cessation_of_operation	(5)
	NULL
};

static int checkPolicy(const char * policy, int * algo, int * keybits);

static int checkReason(const char * reason);

// added 2001.08.29
int storeTempCertInfo(const char *subject, ECC *ecc);

static unsigned char * certSigningRequest( const char * subject, int nAlgo, int nKeyLen, void ** key, int * length);

static unsigned char * certRevocationRequest( const char * subject, int reason, void ** key, int * length, int * algo);



//-------------------------------------
//
//	certRequest
//
//	create certificate request message
//
//-------------------------------------

unsigned char * certRequest( int nCommand, const char * subject,
							 const char * policy, int * reqLength)
{
	unsigned char *certReq=NULL, *certReqInfo=NULL, *sig=NULL, *p;
	unsigned char md[SHA_DIGEST_LENGTH];
	int nOffset=0, nSigLen=0, nLength=0, nReqInfoLen=0;
	int nKeyLen=0, nAlgo=0;
#ifdef SCRIPT_RSA
	RSA * key = NULL;
#endif	// SCRIPT_RSA
#ifdef SCRIPT_ECC
	ECC * key = NULL;
	ECDSA_SIG * ecdsa_sig = NULL;
#endif	// SCRIPT_ECC

	if(subject==0 || strlen(subject)==0){
		goto err;
	}
	
	if(nCommand == CMD_CERT_REQUEST){

		//-------------------------------
		//	CASE I : Create CSR Info
		//-------------------------------
		
		// get the public key algorithm
		// and the key length (int bits)

		if(checkPolicy(policy, &nAlgo, &nKeyLen)){
			goto err;
		}

		// create certSigningRequestInfo

		if((certReqInfo = certSigningRequest(subject, nAlgo, nKeyLen, (void **)&key, &nReqInfoLen))==NULL){
			goto err;
		}


	}
	else if(nCommand == CMD_CERT_REVOKE){

		int nReason;
		
		//-----------------------------------
		//	CASE II : Create Revocation Info
		//-----------------------------------

		// check the reason for revocation

		nReason = checkReason(policy);

		// create certRevocationRequestInfo

		if((certReqInfo = certRevocationRequest(subject, nReason, (void **)&key, &nReqInfoLen, &nAlgo))==NULL){
			goto err;
		}
	
		
	}
	else{
		goto err;
	}

	SHA1(certReqInfo, nReqInfoLen, md);

#ifdef SCRIPT_RSA

	nSigLen = RSA_size(key);

	if((sig = (unsigned char *)Malloc(nSigLen))==NULL){
		goto err;
	}

	if(rsaPrivateEncrypt(SHA_DIGEST_LENGTH, md, sig, key, RSA_PKCS1_PADDING)==-1){
		goto err;
	}
#endif	// SCRIPT_RSA

#ifdef SCRIPT_ECC
	
	if((ecdsa_sig = ECDSA_do_sign(md, SHA_DIGEST_LENGTH, key))==NULL){
		goto err;
	}

	nSigLen = i2d_ECDSA_SIG(ecdsa_sig, NULL);

	if((sig = (unsigned char *) Malloc(nSigLen))==NULL){
		goto err;
	}
	p = sig;
	i2d_ECDSA_SIG(ecdsa_sig, &p);

#endif	// SCRIPT_ECC

	nLength =	nReqInfoLen		// certReqInfo length
				+ 1				// signature_algorithm
				+ 2 + nSigLen;	// signature_length + signature

	if((certReq = (unsigned char *)Malloc(nLength))==NULL){
		goto err;
	}

	WRITE_TO_BUF( certReq, certReqInfo, nReqInfoLen, nOffset);

	switch(nAlgo){
		case PUBKEY_ALGO_RSA:
			certReq[nOffset++] = sigalgo_rsa_sha;
			break;
		case PUBKEY_ALGO_ECC:
			certReq[nOffset++] = sigalgo_ecdsa_sha;
			break;
		default:
			goto err;
	}
	certReq[nOffset++] = nSigLen >> 8;
	certReq[nOffset++] = nSigLen;
	WRITE_TO_BUF( certReq, sig, nSigLen, nOffset);


	if(nOffset != nLength){
		Free(certReq);
		certReq = NULL;
		goto err;
	}

	*reqLength = nLength;


err:

	if(certReqInfo){
		Free(certReqInfo);
	}
	if(sig){
		Free(sig);
	}
#ifdef SCRIPT_RSA
	if(key){
		RSA_free(key);
	}
#endif	// SCRIPT_RSA
	
#ifdef SCRIPT_ECC
	if(key){
		ECC_free(key);
	}
	if(ecdsa_sig){
		ECDSA_SIG_free(ecdsa_sig);
	}
#endif	// SCRIPT_ECC

	return certReq;

}


//-------------------------------------
//
//	certSigningRequest
//
//	create certificate signing request
//	information
//
//-------------------------------------

static unsigned char * certSigningRequest( const char * subject, int nAlgo, int nKeyLen, void ** key, int * length)
{

	int nOffset=0, nKeyBufLen, nParamBufLen, nReqInfoLen, nTemp;
	unsigned char * pKeyBuf=NULL, * pParamBuf=NULL, * certReqInfo=NULL, *p;

#ifdef SCRIPT_RSA
	RSA * rsa = NULL;
#endif	// SCRIPT_RSA

#ifdef SCRIPT_ECC
	ECC * ecc = NULL;
#endif


#ifdef SCRIPT_RSA
	if(nAlgo!=PUBKEY_ALGO_RSA){
		goto err;
	}
#endif	// SCRIPT_RSA

#ifdef SCRIPT_ECC
	if(nAlgo!=PUBKEY_ALGO_ECC){
		goto err;
	}
#endif	// SCRIPT_ECC


#ifdef SCRIPT_RSA
	// create RSA Key

	if((rsa = rsaGenerateKey(nKeyLen, 3))==NULL){
		goto err;
	}


	// RSA Public Key

	if((nKeyBufLen = i2d_RSAPublicKey( rsa, NULL ))<=0){
		goto err;
	}

	if((pKeyBuf = (unsigned char *)Malloc(nKeyBufLen))==NULL){
		goto err;
	}

	p = pKeyBuf;

	if(i2d_RSAPublicKey( rsa, &p)!=nKeyBufLen){
		goto err;
	}

	// RSA Parameter
	
	nParamBufLen = 0;
	pParamBuf = NULL;
	
#endif	// SCRIPT_RSA

#ifdef SCRIPT_ECC

	ecc = ECC_new();
	if(!ECC_generate_key(ecc)){
		goto err;
	}

	// ECC PublicKey
	
	if((nKeyBufLen = i2d_ECCPublicKey( ecc, NULL ))<=0){
		goto err;
	}

	if((pKeyBuf = (unsigned char *)Malloc(nKeyBufLen))==NULL){
		goto err;
	}

	p = pKeyBuf;

	if(i2d_ECCPublicKey( ecc, &p)!=nKeyBufLen){
		goto err;
	}

	// ECC Parameter
	
	if((nParamBufLen = i2d_ECCparams( ecc, NULL ))<=0){
		goto err;
	}

	if((pParamBuf = (unsigned char *)Malloc(nParamBufLen))==NULL){
		goto err;
	}

	p = pParamBuf;

	if(i2d_ECCparams( ecc, &p)!=nParamBufLen){
		goto err;
	}

#endif	// SCRIPT_ECC
	
	// get certSigningInfo length
	
	nReqInfoLen = 	1		// certificate_request_type
					+ 1		// cert_request_version
					+ 1 + 2 + 1 + strlen(subject)
							// IdentifierType + char_set
							// + id_length + subject
					+ 1 + 2 + nKeyBufLen	// pub_key_type + key_len + key
						+ 2 + nParamBufLen;	// param_len + param

	if((certReqInfo = (unsigned char *)Malloc(nReqInfoLen))==NULL){
		goto err;
	}

	//--------------------------
	//	Cert Signing Info
	//--------------------------

	//--------------------------
	//	1. cert_request_type
	//--------------------------

	certReqInfo[nOffset++] = cert_signing_request;
	
	//--------------------------
	//	2. cert_request_version
	//--------------------------

	certReqInfo[nOffset++] = CUR_CERTREQ_VERSION;
		
	//--------------------------
	//	3. subject identifier
	//--------------------------

	certReqInfo[nOffset++] = idtype_text;
	certReqInfo[nOffset++] = 0x00;	// character set
	certReqInfo[nOffset++] = 0x04;
	if((nTemp = strlen(subject))>255){
		goto err;
	}
	certReqInfo[nOffset++] = nTemp;
	WRITE_TO_BUF(certReqInfo, subject, nTemp, nOffset);

	//--------------------------
	//	4. SubjectPublicKeyInfo
	//--------------------------

	switch(nAlgo){
		case PUBKEY_ALGO_RSA:
			certReqInfo[nOffset++] = pkeytype_rsa;
			break;
		case PUBKEY_ALGO_ECC:
			certReqInfo[nOffset++] = pkeytype_ecdsa;
			break;
		default:
			goto err;
	}

	certReqInfo[nOffset++] = nKeyBufLen >> 8;
	certReqInfo[nOffset++] = nKeyBufLen;
	WRITE_TO_BUF(certReqInfo, pKeyBuf, nKeyBufLen, nOffset);

	certReqInfo[nOffset++] = nParamBufLen >> 8;
	certReqInfo[nOffset++] = nParamBufLen;
	if(nParamBufLen){
		WRITE_TO_BUF(certReqInfo, pParamBuf, nParamBufLen, nOffset);
	}

	if(nOffset!=nReqInfoLen){
		goto err;
	}

	*length = nReqInfoLen;

#ifdef SCRIPT_RSA
	*key = (void *)rsa;
#endif
#ifdef SCRIPT_ECC
	*key = (void *)ecc;
#endif

	Free(pKeyBuf);
	Free(pParamBuf);

// added 2001.08.29
#ifdef SCRIPT_ECC
    if( storeTempCertInfo(subject, ecc) < 0 )
		goto err;
#endif

	return certReqInfo;

err:

	if(pParamBuf){
		Free(pParamBuf);
	}

	if(pKeyBuf){
		Free(pKeyBuf);
	}
	if(certReqInfo){
		Free(certReqInfo);
	}
#ifdef SCRIPT_RSA
	if(rsa){
		RSA_free(rsa);
	}
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
	if(ecc){
		ECC_free(ecc);
	}
#endif

	return NULL;

}



//---------------------------------------
//
//	create certificate revocation request
//	information
//
//---------------------------------------


static unsigned char * certRevocationRequest( const char * subject, int reason, void ** key, int * length, int * algo)
{
	int nOffset=0, nReqInfoLen, nTemp, certIndex;
	unsigned char * certReqInfo = NULL;


	// get certSigningInfo length
	
	nReqInfoLen = 	1		// certificate_request_type
					+ 1		// cert_request_version
					+ 1 + 2 + 1 + strlen(subject)
							// IdentifierType + char_set
							// + id_length + subject
					+ 1;	// ReasonForRevocation

	if((certReqInfo = (unsigned char *)Malloc(nReqInfoLen))==NULL){
		goto err;
	}

	//--------------------------
	//	Cert Revocation Info
	//--------------------------

	//--------------------------
	//	1. cert_request_type
	//--------------------------

	certReqInfo[nOffset++] = cert_revocation_request;
	
	//--------------------------
	//	2. cert_request_version
	//--------------------------

	certReqInfo[nOffset++] = CUR_CERTREQ_VERSION;
		
	//--------------------------
	//	3. subject identifier
	//--------------------------

	certReqInfo[nOffset++] = idtype_text;
	certReqInfo[nOffset++] = 0x00;	// character set
	certReqInfo[nOffset++] = 0x04;
	if((nTemp = strlen(subject))>255){
		goto err;
	}
	certReqInfo[nOffset++] = nTemp;
	WRITE_TO_BUF(certReqInfo, subject, nTemp, nOffset);

	//--------------------------
	//	4. ReasonForRevocation
	//--------------------------

	certReqInfo[nOffset++] = reason;

	
	if(nOffset!=nReqInfoLen){
		goto err;
	}

	//!---------------------------!
	//!                           !
	//!	HAVE TO CHANGE THIS PART  !
	//!                           !
	//!                           !
	

	////////////////////////////////////////////////////////////////////////////
	//Search My CERT with Subject DN string, is it possible ?
	////////////////////////////////////////////////////////////////////////////
	if(!clnt_search_user_cert_dn((char *)subject, &certIndex)){
		CLNTa_log(1,0,"인증서폐지 목록없음.\n");
		goto err;
	}

	CLNTa_log(1,0,"인증서폐지 index[%d]\n", certIndex);

	*key = (void *)getPrivateKey(certIndex);
	////////////////////////////////////////////////////////////////////////////

#ifdef SCRIPT_RSA
	*algo = PUBKEY_ALGO_RSA;
#endif	// SCRIPT_RSA
#ifdef SCRIPT_ECC
	*algo = PUBKEY_ALGO_ECC;
#endif	// SCRIPT_ECC
	
	//!                           !
	//!---------------------------!


	*length = nReqInfoLen;

	return certReqInfo;

err:

	if(certReqInfo){
		Free(certReqInfo);
	}

	return NULL;
}


//----------------------------
//
//	check the reason for
//	certificate revocation
//
//----------------------------


static int checkReason(const char * reason)
{
	int i;
	int nReason = reason_anonymous;

	for(i=0; revReason[i]!=NULL; ++i){
		if(!strcmp(reason, revReason[i])){
			nReason = i;
			break;
		}
	}

	return nReason;
}


static int checkPolicy(const char * policy, int * algo, int * keybits)
{

	char * p;
	
	if(policy==NULL){
		return -1;
	}

	if((p=strchr(policy, ':'))==NULL){
		return -1;
	}

#ifdef SCRIPT_RSA
	if(!strncmp(policy, ALGO_STR_RSA, p-policy)){
		*algo = PUBKEY_ALGO_RSA;
	}
	else if(!strncmp(policy, ALGO_STR_ECC, p-policy)){
		return -1;
	}
#endif	// SCRIPT_RSA
#ifdef SCRIPT_ECC
	if(!strncmp(policy, ALGO_STR_RSA, p-policy)){
		return -1;
	}
	else if(!strncmp(policy, ALGO_STR_ECC, p-policy)){
		*algo = PUBKEY_ALGO_ECC;
	}
#endif	// SCRIPT_ECC
	else{
		return -1;
	}

	// get the key length (int bits);

	if((*keybits = atoi(++p))<=0){
		return -1;
	}

	return 0;

}

// added 2001.08.29
#ifdef SCRIPT_ECC

int setTempCertInfo(unsigned char *pin_hash, unsigned char *enc_prv_key, 
		unsigned char *skid, long req_date, const char *dn, int key_usage) {

	//////////////////////////////////////////////////
	int index = 0;
	//////////////////////////////////////////////////

	// check available space
	if(SKY_GET_MW_TEMPCERT_DATA_SAVED(0) == 0) index = 0;
	else if(SKY_GET_MW_TEMPCERT_DATA_SAVED(1) == 0) index = 1;
	else ERR_FATAL("Can't be here",0,0,0);

	SKY_SET_MW_TEMPCERT_PIN_HASH(index, pin_hash, 20);
	SKY_SET_MW_TEMPCERT_PRV_KEY(index, (unsigned char *)enc_prv_key, 96);
	SKY_SET_MW_TEMPCERT_SKID(index, skid, 20);
	SKY_SET_MW_TEMPCERT_REQUESTDATE(index, (unsigned long)req_date);
	SKY_SET_MW_TEMPCERT_SUBJECT_DN(index, (unsigned char *)dn, 400);
	SKY_SET_MW_TEMPCERT_KEY_USAGE(index, key_usage);

	SKY_SET_MW_TEMPCERT(index);
	SKY_SET_MW_TEMPCERT_DATA_SAVED(index, 1);
	SKY_SET_MW_WALLET();

	return 0;
}


int storeTempCertInfo(const char *subject, ECC *ecc) {
	char *pin = NULL;
	unsigned char pin_hash[20], skid[20];
	int nPrivateKeyBufLen;
	unsigned char *pPrivateKeyBuf=NULL, *p;
	unsigned char *encPrvKey=NULL;
	int nEncPrvKey;
	long req_date;

	pin = certPIN;

	SHA1((unsigned char *)pin, 8, pin_hash);

	req_date = (long)CLNTa_currentTime();

	ECC_KeyHash(skid, ecc);

	// 단말기 저장 필드 정의

    if((nPrivateKeyBufLen = i2d_ECCPrivateKey( ecc, NULL ))<=0){
		goto err;
    }
	if((pPrivateKeyBuf = (unsigned char *)Malloc(nPrivateKeyBufLen))==NULL){
	    goto err;
	}
	p = pPrivateKeyBuf;
	if(i2d_ECCPrivateKey( ecc, &p ) != nPrivateKeyBufLen){
	    goto err;
	}

	encPrvKey = key_encrypt(pPrivateKeyBuf, nPrivateKeyBufLen, pin_hash, &nEncPrvKey);

	setTempCertInfo(pin_hash, encPrvKey, skid, req_date, subject, V_WTLS__DIGITAL_SIGNATURE);

err:
	if(pPrivateKeyBuf){
	    Free(pPrivateKeyBuf);
	}

	if(encPrvKey){
	    Free(encPrvKey);
	}

	return 0;
}

#endif  // SCRIPT_ECC


//
//	End of cert_request.c
//
//---------------------------------
