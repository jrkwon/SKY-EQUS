#ifdef SCRIPT_RSA
    #include "rsa_new.h"
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
    #include "Ecdsa.h"
#endif // SCRIPT_ECC

#include "sha.h"
#include "signtext.h"
#include "cert_manage.h"
#include "script_enum.h"

//MCommerce Porting by swjung
#include "clk.h"
#include "clnt.h"

extern unsigned long SKY_GetLocalTime(void);
extern void clk_secs_to_julian(unsigned long secs, clk_julian_type *julian_ptr);

enum { USE_CERTIFICATE = 1, USE_CERT_URL = 2 };

static int nCertInfoType = USE_CERT_URL;

#define WRITE_TO_BUF( target, source, length, offset ) \
            { memcpy(&target[offset], source, length); offset += length; }

unsigned char * signText( const char * strToSign, int nOpt, int nKeyIdType,
							const unsigned char * keyId, int * contentLength)
{
	//2001/09/28 Added by swjung
	////////////////////////////////////////////////////////
	int		keyNum, sl;
	int		keyexist=0;
	int		i;
	unsigned char temp_keyId[20];
	unsigned char * estr = NULL;
	////////////////////////////////////////////////////////

	unsigned char md[SHA_DIGEST_LENGTH];
	unsigned char sigTemplate[95];
	char timeBuf[13];
    //swjung
	unsigned long current_time;
    clk_julian_type	julian;
	int nEnum;
	int nLength=0, nOffset=0, nCertInfoLen=0, nSigLen=0, nSignerInfoLen=0;
	unsigned char * certInfo=NULL, * usrKeyId=NULL, * sig=NULL, * sigContent=NULL, *p;
#ifdef SCRIPT_RSA
	RSA * key = NULL;
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
	ECC * key = NULL;
	ECDSA_SIG * ecdsa_sig = NULL;
#endif // SCRIPT_ECC

	CLNTa_log(1,0,"전자서명시작합니다.\n");
	if(strToSign==NULL || strlen(strToSign)==0){
		goto err;
	}

	/////////////////////////////////////////////////////
	if((estr = (unsigned char *)Malloc(100))==NULL){
	   goto err;
	}
	/////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	/* find the number of keys */
	if( keyId == NULL ){
		sl = 0;

		if((nEnum = getEnumByID(nKeyIdType, keyId)) == -1){
			//return (unsigned char *)"error:noCert";
			memcpy(estr, "error:noCert", strlen("error:noCert"));
			return estr;
		}
	}
	else {
		if( nKeyIdType == 1 )
			sl = 20;
		else if( nKeyIdType == 2 )
			sl =100;

		if( sl % SHA_DIGEST_LENGTH != 0 ){
			/* invalidParameter */
			goto err;
		}
		keyNum = sl / SHA_DIGEST_LENGTH;

		memset(temp_keyId, 0, 20);
		/* get a appropreate key */
		for( i=0 ; i< keyNum && keyexist == 0 ; i++ ){
			if( memcmp(keyId + (i *SHA_DIGEST_LENGTH), temp_keyId, 20) )
				continue;

			if( (nEnum = getEnumByID( nKeyIdType,
							keyId + (i *SHA_DIGEST_LENGTH) )) == -1 ){
				continue;
			}
			keyexist = 1;
		}

		/* check if key is found */
		if( keyexist == 0 ){
			//return (unsigned char *)"error:noCert";
			memcpy(estr, "error:noCert", strlen("error:noCert"));
			return estr;
		}

	}
	/* end */
	////////////////////////////////////////////////////////////////////////
	//result가 null일 경우에는 아무거나 보내도 된다.
	//use original argment
	//To do list swjung
	if((nEnum = getEnumByID(nKeyIdType, keyId)) == -1){	// can't find appropriate
											// certificate and private key
		return NULL;
	}

	if(nOpt & INCLUDE_KEY_HASH){
		if((usrKeyId = getKeyID(nEnum))==NULL){
			goto err;
		}
	}
	
	if(nOpt & INCLUDE_CERTIFICATE){

		if(nCertInfoType==USE_CERTIFICATE){
			// include user certificate in the signer info

			if((certInfo = getCertificate(nEnum, &nCertInfoLen))==NULL){
				goto err;
			}

		}
		else{
			// include user cert. url in the signer info

			if((certInfo = (unsigned char *) getCertURL(nEnum))==NULL){
				goto err;
			}
			//swjung type casting
			//nCertInfoLen = strlen(certInfo);
			nCertInfoLen = strlen((const char *)certInfo);
		}
	}

    // get signing time
	//By swjung 2001.01.08
	//We do not support gmtime() *shrug*
	current_time = SKY_GetLocalTime();
    clk_secs_to_julian( current_time, &julian);

	sprintf(timeBuf,	"%02d%02d%02d%02d%02d%02d",
						julian.year%100,
						julian.month,
						julian.day,
						julian.hour,
						julian.minute,
						julian.second);
	// sign sinature template


	setSigTemplate(sigTemplate, timeBuf, (const unsigned char *)strToSign, strlen(strToSign));

	// n개 swjung
	if((key = getPrivateKey(nEnum))==NULL){
		goto err;
	}



	// get the message digest of the signature template
	
	SHA1(sigTemplate, sizeof(sigTemplate), md);

#ifdef SCRIPT_RSA
	nSigLen = RSA_size(key);
	if((sig = (unsigned char *) Malloc(nSigLen))==NULL){
		goto err;
	}

	if(rsaPrivateEncrypt(SHA_DIGEST_LENGTH, md, sig, key, RSA_PKCS1_PADDING)==-1){
		goto err;
	}
#endif	// SCRIPT_RSA

#ifdef SCRIPT_ECC

	//Real signing
	if((ecdsa_sig = ECDSA_do_sign(md, SHA_DIGEST_LENGTH, key))==NULL){
		goto err;
	}

	//Internal to "DER" format
	nSigLen = i2d_ECDSA_SIG(ecdsa_sig, NULL);
	if((sig = (unsigned char *) Malloc(nSigLen))==NULL){
		goto err;
	}

	//Internal to "DER" format
	p = sig;
	i2d_ECDSA_SIG(ecdsa_sig, &p);

#endif

	//-------------------------------
	// Construct WAP Signed Content
	//-------------------------------

	// calculate the length beforehand
	
	if(nOpt & (INCLUDE_KEY_HASH | INCLUDE_CERTIFICATE)){

		if(nOpt & INCLUDE_KEY_HASH){
			nSignerInfoLen += 1 + 20;	// sig_info_type + sha1_hash
		}

		if(nOpt & INCLUDE_CERTIFICATE){
			nSignerInfoLen += 1;	// sig_info_type
			if(nCertInfoType==USE_CERTIFICATE){
				// cert_len + certificate
				nSignerInfoLen += 2 + nCertInfoLen;
			}
			else{
				// cert_url_len + cert_url
				nSignerInfoLen += 1 + nCertInfoLen;
			}
		}
	}
	else{
		nSignerInfoLen = 1;	// signer_info_type(implicit);
	}
		

	nLength =	1						// version
				+ 1 + 2 + nSigLen		// signature_algo + sig_length + sig
				+ 2 + nSignerInfoLen	// signerinfo_len + signerInfo
				+ 1 + 2 + 1 + ((nOpt&INCLUDE_CONTENT)?(2+strlen(strToSign)):0)
										// contentType + content_encoding
										// + content_present + content
				+ 1 + 1 + 12;			// authAttrLen + attr_type + gmt_utc_time;

	if((sigContent = (unsigned char *)Malloc(nLength))==NULL){
		goto err;
	}

	nOffset = 0;

	//------------------
	//	1. Version
	//------------------
	
	sigContent[nOffset++] = 1;

	//------------------
	//	2. signature
	//------------------
	
#ifdef SCRIPT_RSA
	sigContent[nOffset++] = rsa_sha_pkcs1;
#endif // SCRIPT_RSA
#ifdef SCRIPT_ECC
	sigContent[nOffset++] = ecdsa_sha;
#endif

	sigContent[nOffset++] = nSigLen >> 8;
	sigContent[nOffset++] = nSigLen;
	WRITE_TO_BUF(sigContent, sig, nSigLen, nOffset);


	//------------------
	//	3. signer_infos
	//------------------

	sigContent[nOffset++] = nSignerInfoLen >> 8;
	sigContent[nOffset++] = nSignerInfoLen;
	
	if(nOpt & (INCLUDE_KEY_HASH | INCLUDE_CERTIFICATE)){

		if(nOpt & INCLUDE_KEY_HASH){
			sigContent[nOffset++] = sha_key_hash;
			WRITE_TO_BUF(sigContent, usrKeyId, 20 /* SHA_DIGEST_LENGTH */, nOffset);
		}

		if(nOpt & INCLUDE_CERTIFICATE){
			if(nCertInfoType==USE_CERTIFICATE){
				sigContent[nOffset++] = x509_certificate;
				sigContent[nOffset++] = nCertInfoLen >> 8;
				sigContent[nOffset++] = nCertInfoLen;
				WRITE_TO_BUF(sigContent, certInfo, nCertInfoLen, nOffset);
			}
			else{
				sigContent[nOffset++] = certificate_url;
				sigContent[nOffset++] = nCertInfoLen;
				WRITE_TO_BUF(sigContent, certInfo, nCertInfoLen, nOffset);
			}
		}
	}
	else{
		sigContent[nOffset++] = implicit;	// signer_info_type
	}
	
	//------------------
	//	4. content_info
	//------------------
	
	sigContent[nOffset++] = contenttype_text;	// content type
	sigContent[nOffset++] = 0x00;	// content encoding
	sigContent[nOffset++] = 0x04;
	
	if(nOpt & INCLUDE_CONTENT){
		int nContentLen = strlen(strToSign);
		sigContent[nOffset++] = bool_true;
		sigContent[nOffset++] = nContentLen >> 8;
		sigContent[nOffset++] = nContentLen;
		WRITE_TO_BUF(sigContent, strToSign, nContentLen, nOffset);
	}
	else{
		sigContent[nOffset++] = bool_false;
	}

	//------------------
	//	5. auth_attrs
	//------------------
	
	sigContent[nOffset++] = 13;			// Authenticated Attribute size
	sigContent[nOffset++] = gmt_utc_time;			// attribute_type
	WRITE_TO_BUF(sigContent, timeBuf, 12, nOffset);	// gmt_utc_time

	if(nLength!=nOffset){
		goto err;
	}

	*contentLength = nLength;

	// Clean Up :)
	// I hate memory leak... -_-;

	Free(certInfo);
	Free(usrKeyId);
	Free(sig);

	//////////////////////////////////////
	Free(estr);
	//////////////////////////////////////

#ifdef SCRIPT_RSA
	RSA_free(key);
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
	ECDSA_SIG_free(ecdsa_sig);
	ECC_free(key);
#endif

	// return WAP signed content

	return sigContent;

err:

	if(certInfo){
		Free(certInfo);
	}
	if(usrKeyId){
		Free(usrKeyId);
	}
	if(sig){
		Free(sig);
	}
	if(sigContent){
		Free(sigContent);
	}
	if(key){
#ifdef SCRIPT_RSA
		RSA_free(key);
#endif // SCRIPT_RSA
#ifdef SCRIPT_ECC
		ECC_free(key);
#endif // SCRIPT_RSA
	}

#ifdef SCRIPT_ECC
	if(ecdsa_sig){
		ECDSA_SIG_free(ecdsa_sig);
	}
#endif

	memcpy(estr, "invalid", strlen("invalid"));
	return estr;
}


	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//여기가 내가 고치는 부분입니다요
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

unsigned char * signText_swjung( const char * strToSign, int nOpt, int nKeyIdType,
							const unsigned char * keyId, int * contentLength)
{
	unsigned char md[SHA_DIGEST_LENGTH];
	unsigned char sigTemplate[95];
	char timeBuf[13];
    //swjung
	unsigned long current_time;
    clk_julian_type	julian;
	int nEnum;
	int nLength=0, nOffset=0, nCertInfoLen=0, nSigLen=0, nSignerInfoLen=0;
	unsigned char * certInfo=NULL, * usrKeyId=NULL, * sig=NULL, * sigContent=NULL, *p;

#ifdef SCRIPT_ECC
	ECC * key = NULL;
	ECDSA_SIG * ecdsa_sig = NULL;
#endif // SCRIPT_ECC

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//여기가 내가 고치는 부분입니다요
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	CLNTa_log(1,0,"전자서명시작합니다.\n");
	if(strToSign==NULL || strlen(strToSign)==0){
		goto err;
	}

	//result가 null일 경우에는 아무거나 보내도 된다.
	//use original argment
	if((nEnum = getEnumByID(nKeyIdType, keyId)) == -1){	// can't find appropriate
											// certificate and private key
		return NULL;
	}

	if(nOpt & INCLUDE_KEY_HASH){
		if((usrKeyId = getKeyID(nEnum))==NULL){
			goto err;
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//여기가 내가 고치는 부분입니다요
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	if(nOpt & INCLUDE_CERTIFICATE){

		if(nCertInfoType==USE_CERTIFICATE){
			// include user certificate in the signer info

			if((certInfo = getCertificate(nEnum, &nCertInfoLen))==NULL){
				goto err;
			}

		}
		else{
			// include user cert. url in the signer info

			if((certInfo = (unsigned char *) getCertURL(nEnum))==NULL){
				goto err;
			}
			//swjung type casting
			//nCertInfoLen = strlen(certInfo);
			nCertInfoLen = strlen((const char *)certInfo);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//여기가 내가 고치는 부분입니다요
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
    
	current_time = SKY_GetLocalTime();
    clk_secs_to_julian( current_time, &julian);

	sprintf(timeBuf,	"%02d%02d%02d%02d%02d%02d",
						julian.year%100,
						julian.month,
						julian.day,
						julian.hour,
						julian.minute,
						julian.second);

	///////////////////////////////////////////////////////////////////////////////////////
	// sign sinature template
	setSigTemplate(sigTemplate, timeBuf, (const unsigned char *)strToSign, strlen(strToSign));

	// n개 swjung
	if((key = getPrivateKey(nEnum))==NULL){
		goto err;
	}

	// get the message digest of the signature template	
	SHA1(sigTemplate, sizeof(sigTemplate), md);

	if((ecdsa_sig = ECDSA_do_sign(md, SHA_DIGEST_LENGTH, key))==NULL){
		goto err;
	}

	nSigLen = i2d_ECDSA_SIG(ecdsa_sig, NULL);
	if((sig = (unsigned char *) Malloc(nSigLen))==NULL){
		goto err;
	}
	p = sig;
	i2d_ECDSA_SIG(ecdsa_sig, &p);


	//-------------------------------
	// Construct WAP Signed Content
	//-------------------------------

	// calculate the length beforehand
	
	if(nOpt & (INCLUDE_KEY_HASH | INCLUDE_CERTIFICATE)){

		if(nOpt & INCLUDE_KEY_HASH){
			nSignerInfoLen += 1 + 20;	// sig_info_type + sha1_hash
		}

		if(nOpt & INCLUDE_CERTIFICATE){
			nSignerInfoLen += 1;	// sig_info_type
			if(nCertInfoType==USE_CERTIFICATE){
				// cert_len + certificate
				nSignerInfoLen += 2 + nCertInfoLen;
			}
			else{
				// cert_url_len + cert_url
				nSignerInfoLen += 1 + nCertInfoLen;
			}
		}
	}
	else{
		nSignerInfoLen = 1;	// signer_info_type(implicit);
	}
		

	nLength =	1						// version
				+ 1 + 2 + nSigLen		// signature_algo + sig_length + sig
				+ 2 + nSignerInfoLen	// signerinfo_len + signerInfo
				+ 1 + 2 + 1 + ((nOpt&INCLUDE_CONTENT)?(2+strlen(strToSign)):0)
										// contentType + content_encoding
										// + content_present + content
				+ 1 + 1 + 12;			// authAttrLen + attr_type + gmt_utc_time;

	if((sigContent = (unsigned char *)Malloc(nLength))==NULL){
		goto err;
	}

	nOffset = 0;

	//------------------
	//	1. Version
	//------------------
	
	sigContent[nOffset++] = 1;

	//------------------
	//	2. signature
	//------------------
	
	sigContent[nOffset++] = ecdsa_sha;

	sigContent[nOffset++] = nSigLen >> 8;
	sigContent[nOffset++] = nSigLen;
	WRITE_TO_BUF(sigContent, sig, nSigLen, nOffset);


	//------------------
	//	3. signer_infos
	//------------------

	sigContent[nOffset++] = nSignerInfoLen >> 8;
	sigContent[nOffset++] = nSignerInfoLen;
	
	if(nOpt & (INCLUDE_KEY_HASH | INCLUDE_CERTIFICATE)){

		if(nOpt & INCLUDE_KEY_HASH){
			sigContent[nOffset++] = sha_key_hash;
			WRITE_TO_BUF(sigContent, usrKeyId, 20 /* SHA_DIGEST_LENGTH */, nOffset);
		}

		if(nOpt & INCLUDE_CERTIFICATE){
			if(nCertInfoType==USE_CERTIFICATE){
				sigContent[nOffset++] = x509_certificate;
				sigContent[nOffset++] = nCertInfoLen >> 8;
				sigContent[nOffset++] = nCertInfoLen;
				WRITE_TO_BUF(sigContent, certInfo, nCertInfoLen, nOffset);
			}
			else{
				sigContent[nOffset++] = certificate_url;
				sigContent[nOffset++] = nCertInfoLen;
				WRITE_TO_BUF(sigContent, certInfo, nCertInfoLen, nOffset);
			}
		}
	}
	else{
		sigContent[nOffset++] = implicit;	// signer_info_type
	}
	
	//------------------
	//	4. content_info
	//------------------
	
	sigContent[nOffset++] = contenttype_text;	// content type
	sigContent[nOffset++] = 0x00;	// content encoding
	sigContent[nOffset++] = 0x04;
	
	if(nOpt & INCLUDE_CONTENT){
		int nContentLen = strlen(strToSign);
		sigContent[nOffset++] = bool_true;
		sigContent[nOffset++] = nContentLen >> 8;
		sigContent[nOffset++] = nContentLen;
		WRITE_TO_BUF(sigContent, strToSign, nContentLen, nOffset);
	}
	else{
		sigContent[nOffset++] = bool_false;
	}

	//------------------
	//	5. auth_attrs
	//------------------
	
	sigContent[nOffset++] = 13;			// Authenticated Attribute size
	sigContent[nOffset++] = gmt_utc_time;			// attribute_type
	WRITE_TO_BUF(sigContent, timeBuf, 12, nOffset);	// gmt_utc_time

	if(nLength!=nOffset){
		goto err;
	}

	*contentLength = nLength;

	// Clean Up :)
	// I hate memory leak... -_-;

	Free(certInfo);
	Free(usrKeyId);
	Free(sig);

	ECDSA_SIG_free(ecdsa_sig);
	ECC_free(key);

	// return WAP signed content
	return sigContent;

err:
	if(certInfo){
		Free(certInfo);
	}
	if(usrKeyId){
		Free(usrKeyId);
	}
	if(sig){
		Free(sig);
	}
	if(sigContent){
		Free(sigContent);
	}
	if(key){
		ECC_free(key);
	}
	if(ecdsa_sig){
		ECDSA_SIG_free(ecdsa_sig);
	}
	return NULL;
}





//
//	End of signtext.c
//
//--------------------
