//---------------------------------------
//
//	signtext.c
//
//	implementation of the WAP WMLScript
//	Crypto Library "signText"
//
//	2000. 10
//
//	by Byung-Jun Yoon
//
//----------------------------------------

#ifdef SCRIPT_ECC
#undef SCRIPT_ECC

#define SCRIPT_RSA

#ifdef SCRIPT_RSA
    #include "rsa_new.h"
#endif // SCRIPT_RSA

#ifdef SCRIPT_ECC
    #include "Ecdsa.h"
#endif // SCRIPT_ECC

#include "sha.h"
#include "time.h"
#include "signtext.h"
//swjung temporarily blocked 
//#include "cert_manage_rsa.h"
//swjung add header
#include "cert_manage.h"
#include "script_enum.h"

//MCommerce Porting by swjung
#include "clk.h"

//by swjung for timer porting
//extern unsigned long ui_get_local_time(void);
extern unsigned long SKY_GetLocalTime(void);
extern void clk_secs_to_julian(unsigned long secs, clk_julian_type *julian_ptr);

enum { USE_CERTIFICATE = 1, USE_CERT_URL = 2 };

//static int nCertInfoType = USE_CERTIFICATE;
static int nCertInfoType = USE_CERT_URL;



#define WRITE_TO_BUF( target, source, length, offset ) \
            { memcpy(&target[offset], source, length); offset += length; }


unsigned char * signText_rsa( const char * strToSign, int nOpt, int nKeyIdType,
							const unsigned char * keyId, int * contentLength)
{
	unsigned char md[SHA_DIGEST_LENGTH];
	unsigned char sigTemplate[95];
	char timeBuf[13];

//swjung
	unsigned long current_time;
    clk_julian_type	julian;

#ifdef BLOCK
	time_t t = time(NULL);
	struct tm * pTime;
#endif //#ifdef BLOCK

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

#ifdef BLOCK
	pTime = gmtime(&t);
	sprintf(timeBuf,	"%02d%02d%02d%02d%02d%02d",
						pTime->tm_year%100,
						pTime->tm_mon+1,
						pTime->tm_mday,
						pTime->tm_hour,
						pTime->tm_min,
						pTime->tm_sec );
#endif //#ifdef BLOCK


	// sign sinature template

	setSigTemplate(sigTemplate, timeBuf, (const unsigned char *)strToSign, strlen(strToSign));

	if((key = getPrivateKey(nEnum))==NULL){
		goto err;
	}

	// get the message digest of the signature template
	
	SHA1(sigTemplate, sizeof(sigTemplate), md);

#if 0
	{	
		int l=0;
		for(l=0; l<sizeof(md); ++l){
			printf("(%02X)", md[l]);
		}
		printf("\n");
	}
#endif


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

	if((ecdsa_sig = ecdsa_do_sign(md, SHA_DIGEST_LENGTH, key))==NULL){
		goto err;
	}

	nSigLen = i2d_ECDSA_SIG(ecdsa_sig, NULL);
	if((sig = (unsigned char *) Malloc(nSigLen))==NULL){
		goto err;
	}
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
		//swjung
		//sigContent[nOffset++] = true;
		sigContent[nOffset++] = 1;
		sigContent[nOffset++] = nContentLen >> 8;
		sigContent[nOffset++] = nContentLen;
		WRITE_TO_BUF(sigContent, strToSign, nContentLen, nOffset);
	}
	else{
		//swjung
		//sigContent[nOffset++] = false;
		sigContent[nOffset++] = 0;
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

	return NULL;
}





//
//	End of signtext.c
//
//--------------------
#endif //#ifdef SCRIPT_ECC
