//---------------------------------
//
//	crypto_script.c
//
//	WMLScript Crypto API
//	implementation
//
//	2000. 12. by Byung-Jun Yoon
//
//---------------------------------

#include <string.h>
#include "base64.h"
#include "crypto.h"

#include "cert_request.h"
#include "signtext.h"
#include "encrypttext.h"
#include "crypto_size.h"
#include "wcert.h"

/////////////////////////////////////////////////////////////////////////////////
#include "clnt.h"
#include "ska.h"
#include "sky.h"
/////////////////////////////////////////////////////////////////////////////////

char * script_signText( const char * strToSign, int nOption, int nKeyIdType,
						const unsigned char * keyId, int errorMMI)
{
	unsigned char * sig = NULL;
	char * encSig = NULL;
	int nLength, nEncLen;
	//2001.10.04
	///////////////////////////////////////////////////////
	char *estr = NULL;
	///////////////////////////////////////////////////////

	if((estr = (char *)Malloc(100))==NULL){
		return NULL;
	}	
	///////////////////////////////////////////////////////////////////////
	//Codes for SKY MMI triggered error handling
	///////////////////////////////////////////////////////////////////////
	if(errorMMI != MMI_NO_ERROR)
	{
		strcpy((char *)estr, (const char *)getErrorMsg());
		goto err;
	}

	if((sig=signText(strToSign, nOption, nKeyIdType, keyId, &nLength))==NULL){
		//2001.10.04
		///////////////////////////////////////////////////////
		goto err;
	}

	//Return type casting added by swjung 09/28
	if( !memcmp(sig, "error:", 6) || !memcmp(sig, "invalid", 7) ) {
		return (char *)sig;
	}

	nEncLen = base64_enc_len(nLength);

	if((encSig = (char *) Malloc(nEncLen+1))==NULL){
		goto err;
	}

	//swjung type casting
	if(encodeBase64((const unsigned char *)sig, nLength, (unsigned char *)encSig)!=nEncLen){
		goto err;
	}

	Free(sig);

	/////////////////////////////
	//2001.10.04
	/////////////////////////////
	if(estr) Free(estr);
	/////////////////////////////

	return encSig;

err:
	if(sig){
		Free(sig);
	}
	if(encSig){
		Free(encSig);
	}

	//////////////////////////////////////////////////////////////////////
	if(errorMMI == MMI_NO_ERROR)
		memcpy(estr, "invalid", strlen("invalid"));

	return estr;
	//////////////////////////////////////////////////////////////////////
}


/* 2001.09.27
 *	- encryptTextEx 9/27 Spec 적용
 * 	- prototype 변경
 */

char * script_encryptText( const char *userPrompt, const char *serverNonce,
						const char * strRecipientCert, int nKeyAgreementAlgo,
						const char * strEncAlgo, int symmetricKeyId, int errorMMI)
{
	unsigned char * envelope = NULL, * cert = NULL;
	char * encData = NULL, * strToEncrypt;
	int nLength, nEncLen, nCertLen;
	WTLSCert wcert;
	/////////////////////////////////////////////////////////
	char *estr = NULL;
	PlainContent *plainContent = NULL;
	unsigned char *subContentList = NULL;
	int nSubContentListLen, nStrLen;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	if((estr = (char *)Malloc(100))==NULL){
		return NULL;
	}

	///////////////////////////////////////////////////////////////////////
	//Codes for SKY MMI triggered error handling
	///////////////////////////////////////////////////////////////////////
	if(errorMMI != MMI_NO_ERROR)
	{
		strcpy((char *)estr, (const char *)getErrorMsg());
		goto err;
	}

	/* strToEncrypt
 	 *	- userPrompt를 파싱하여 Spec에 의하여 User Input을 받은 후
	 *		Length + String + Length + String + ...
	 *		의 형태로 인코딩하여 subContentList에 Assign한다.
	 *		이 때 Length의 크기는 1 byte이고, 값은 다음에 올 String의 길이가 들어간다.
	 *	- 바로 아래의 "subContentList= (char *)userPrompt;" 와
	 *		"nSubContentListLen = strlen( userPrompt );" 부분은 테스트를 위한
	 *		코드이다. 위의 룰대로 인코딩하여 subContentList와 nSubContentListLen에 
	 *		할당한 후  아래의 두 라인은 삭제한다.
 	 */
	subContentList = (unsigned char *)userPrompt; // For Test
	nSubContentListLen = PromptLen; // For Test

	if( (nCertLen = parseWTLSCert( (unsigned char *)strRecipientCert, 0, &wcert)) == 1 ) {
		goto err;
	}

	if(serverNonce == NULL) {
		goto err;
	}

	// Encode PlainContent
	if( PlainContent_set( &plainContent, 0, (unsigned char *)serverNonce, 
			V_WTLS__CT_TEXT, (unsigned char *)subContentList, 
			nSubContentListLen ) == NULL ) {
		goto err;
	}

	if (( nStrLen = i2d_PlainContent( plainContent, (unsigned char **)&strToEncrypt ) ) <= 0) {
	    goto err;
	}

	PlainContent_free( plainContent );

	if((envelope = encryptText((unsigned char *)strToEncrypt, nStrLen, (unsigned char *)strRecipientCert, nCertLen, nKeyAgreementAlgo, strEncAlgo, &nLength))==NULL){
		goto err;
	}

	if( !memcmp(envelope, "error:", 6) || !memcmp(envelope, "invalid", 7) ) {
		return (char *)envelope;
	}

	nEncLen = base64_enc_len(nLength);

	if((encData = (char *) Malloc(nEncLen+1))==NULL){
		goto err;
	}

	if(encodeBase64(envelope, nLength, (unsigned char *)encData)!=nEncLen){
		goto err;
	}

	Free(envelope);
	Free(cert);

	if( strToEncrypt ) Free( strToEncrypt );

	return encData;

err:
	if(envelope){
		Free(envelope);
	}
	if(cert){
		Free(cert);
	}
	if(encData){
		Free(encData);
	}

	if( strToEncrypt )
		Free( strToEncrypt );

	//////////////////////////////////////////////////////////////////////
	if(errorMMI == MMI_NO_ERROR)
		memcpy(estr, "invalid", strlen("invalid"));
	
	return estr;
	/////////////////////////////////////////////////////////////////////
}


char * script_certRequest( int nCommand, const char * strSubject,
							const char * strPolicy, int errorMMI)
{
	unsigned char * req = NULL;
	char * encReq = NULL;
	int nLength, nEncLen;

	/////////////////////////////////////////////////////////
	char *estr = NULL;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	if((estr = (char *)Malloc(100))==NULL){
		return NULL;
	}

	///////////////////////////////////////////////////////////////////////
	//Codes for SKY MMI triggered error handling
	///////////////////////////////////////////////////////////////////////
	if(errorMMI != MMI_NO_ERROR)
	{
		strcpy((char *)estr, (const char *)getErrorMsg());
		goto err;
	}



	if((req=certRequest(nCommand, strSubject, strPolicy, &nLength))==NULL){
		goto err;
	}

	nEncLen = base64_enc_len(nLength);

	if((encReq = (char *) Malloc(nEncLen+1))==NULL){
		goto err;
	}

	//swjung type casting
	if(encodeBase64(req, nLength, (unsigned char *)encReq)!=nEncLen){
		goto err;
	}

	Free(req);

	return encReq;

err:
	if(req){
		Free(req);
	}
	if(encReq){
		Free(encReq);
	}

	if((estr = (char *)Malloc(100))==NULL){
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////
	if(errorMMI == MMI_NO_ERROR)
		memcpy(estr, "invalid", strlen("invalid"));
	
	return estr;
	/////////////////////////////////////////////////////////////////////

}
//
//	END of crypto_script.c
//
//---------------------------------
