//-------------------------------
//
//  proc_cert_response.c
//
//  Process MIME Contents
//
//  2001. 1. by Byung-Jun Yoon
//
//-------------------------------

#include "stdio.h"
#include "string.h"
#include "crypto.h"
#include "script_enum.h"
#include "wap_mime.h"
#include "sky.h"

#define CUR_CERT_RESPONSE_VERSION		1

#define READ_FROM_BUF( source, target, length, offset ) \
			{ memcpy(target, &source[offset], length); offset += length; }

CERT_RESPONSE * acceptCertResponse( const unsigned char * certResponse,
									int length, int *errMsg ) {
	CERT_RESPONSE * res = NULL;
	int nOffset = 0, nVersion, nType, nTemp;
	unsigned char charSet[2];

	if((res = new_cert_response())==NULL){
		goto err1;
	}

	//----------------------------
	//	1. get version
	//----------------------------
	nVersion = certResponse[nOffset++];
	
	if(nVersion!=CUR_CERT_RESPONSE_VERSION){
		goto err2;
	}
	
	//----------------------------
	//	2. get CertRespType
	//----------------------------
	nType = certResponse[nOffset++];	

	switch(nType){
		case V_WTLS__CERT_INFO:
		case V_WTLS__CERT:
			break;

		case V_WTLS__REFERRAL:	// unsupported cert-response type
		default:
			goto err3;
	}
	
	//----------------------------
	//	3. get CertDisplayName
	//----------------------------
	READ_FROM_BUF(certResponse, charSet, 2, nOffset);

	nTemp = certResponse[nOffset++];
	if((res->name = (char *)Malloc(nTemp+1))==NULL){
		goto err1;
	}
	READ_FROM_BUF(certResponse, res->name, nTemp, nOffset);
	res->name[nTemp] = '\0';
	
	//----------------------------
	//	4. get issuer identifier
	//----------------------------
	nTemp = certResponse[nOffset++];	// Identifier Type

	if(nTemp!=idtype_key_hash_sha){
		goto err4;
	}

	READ_FROM_BUF(certResponse, res->akid, 20, nOffset);
	
	//----------------------------
	//	5. get subject identifier
	//----------------------------
	nTemp = certResponse[nOffset++];	// Identifier Type

	if(nTemp!=idtype_key_hash_sha){
		goto err5;
	}

	READ_FROM_BUF(certResponse, res->skid, 20, nOffset);
	
	//----------------------------
	//	6. get Cert/Cert URL
	//----------------------------
	res->type = nType;

	if(nType==(int)V_WTLS__CERT_INFO){
		nTemp = certResponse[nOffset++];
		if((res->cert = (unsigned char *)Malloc(nTemp+1))==NULL){
			goto err1;
		}
		
		READ_FROM_BUF(certResponse, res->cert, nTemp, nOffset);
		res->cert[nTemp] = '\0';
		res->cert_len = nTemp;
	}
	else if(nType==(int)V_WTLS__CERT){
		nTemp = length - nOffset;
		if((res->cert = (unsigned char *)Malloc(nTemp))==NULL){
			goto err1;
		}
		
		READ_FROM_BUF(certResponse, res->cert, nTemp, nOffset);
		res->cert_len = nTemp;
	}
	else{
		goto err3;
	}

	// check size

	if(nOffset != length){
		printf("size error!\n");
		goto err6;
	}

	// Alles Klar! :)

	*errMsg = 0; // Success
	return res;

err1:
	*errMsg = 10010002;	// Memory Allocation error
	goto err;
err2:
	*errMsg = 10010004;	// Invalid MIME Version
	goto err;
err3:
	*errMsg = 10010001;	// Unsupported CertRespType
	goto err;
err4:
	*errMsg = 10010005;	// issuer identifier type != KEY_HASH_SHA
	goto err;
err5:
	*errMsg = 10010005;	// subject identifier type != KEY_HASH_SHA
	goto err;
err6:
	*errMsg = 10010003;	// InValid MIME Data
	goto err;
err:
	if( res ) {
		free_cert_response(res);
	}

	return NULL;
}

CERT_RESPONSE * new_cert_response() {
	CERT_RESPONSE * res;
	
	if((res = (CERT_RESPONSE *)Malloc(sizeof(CERT_RESPONSE)))==NULL){
		return NULL;
	}

	res->name = NULL;
	res->cert = NULL;

	memset(res->akid, 0, 20);
	memset(res->skid, 0, 20);

	res->cert_len = 0;
	res->type = -1;

	return res;
}

void free_cert_response(CERT_RESPONSE * res) {
	if( res ) {
		if( res->name ) {
			Free(res->name);
		}
		if( res->cert ) {
			Free(res->cert);
		}
		Free(res);
	}
}

//
//	End of proc_cert_response.c
//
//---------------------------------
