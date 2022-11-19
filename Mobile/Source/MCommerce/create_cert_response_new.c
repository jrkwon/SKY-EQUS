//-------------------------------
//
//  create_cert_response.c
//
//  Create MIME Contents
//
//  2001. 1. by Byung-Jun Yoon
//
//-------------------------------

#include <string.h>
#include "crypto.h"
#include "wap_mime.h"
#include "script_enum.h"

#define	CUR_CERT_RESPONSE_VERSION		1

unsigned char *createCertResponse( const char * strCertDisplayName,
						const unsigned char * issuerKeyId,
						const unsigned char * subjectKeyId,
						const unsigned char * cert,
						int nCertLen, int nCertRespType,
						int * length, int *errMsg ) {

	int nOffset, nLength, nTemp;
	unsigned char * res = NULL;

	// check cert response type
	
	switch(nCertRespType){
		case V_WTLS_CERT_INFO:
		case V_WTLS_CERT:
			break;

		case V_WTLS_REFERRAL:	// unsupported cert-response type
		default:
			goto err1;
	}

	// caculate the expected mime size

	nLength = 1 + 1		// version + CertRespType
			+ 2 + 1 + strlen(strCertDisplayName)
						// char_set + name_length + CertDisplayName
			+ 1 + 20	// Issuer Identifier = id_type + key_hash
			+ 1 + 20	// Subject Identifier = id_type + key_hash
			+ ((nCertRespType==(int)V_WTLS_CERT_INFO)?(nCertLen+1):nCertLen);
						// cert url : url_len + url
						// cert     : cert_len

	if((res = (unsigned char *)Malloc(nLength))==NULL){
		goto err2;
	}

	nOffset = 0;
	
	//----------------------------
	//	1. version
	//----------------------------
	res[nOffset++] = CUR_CERT_RESPONSE_VERSION;	
	
	//----------------------------
	//	2. Cert Response Type
	//----------------------------
	res[nOffset++] = nCertRespType;
	
	//----------------------------
	//	3. CertDisplayName
	//----------------------------
	res[nOffset++] = '\x00'; // set character set
	res[nOffset++] = '\x04';

	nTemp = strlen(strCertDisplayName); // write display_name
	res[nOffset++] = nTemp;
	WRITE_TO_BUF(res, strCertDisplayName, nTemp, nOffset);
	
	//----------------------------
	//	4. ca_domain ( Issuer Identifier )
	//----------------------------
	res[nOffset++] = idtype_key_hash_sha;
	WRITE_TO_BUF(res, issuerKeyId, 20, nOffset);
	
	//----------------------------
	//	5. subject ( Subject Identifier )
	//----------------------------
	res[nOffset++] = idtype_key_hash_sha;
	WRITE_TO_BUF(res, subjectKeyId, 20, nOffset);

	//----------------------------
	//	6. Cert / Cert URL
	//----------------------------
	if(nCertRespType==(int)V_WTLS_CERT_INFO){
		res[nOffset++] = nCertLen;
	}

	WRITE_TO_BUF(res, cert, nCertLen, nOffset);

//	printf(">> expected size:%d\n", nLength);
//	printf(">> actual size  :%d\n", nOffset);

	// check mime size
	
	if(nLength!=nOffset){
//		printf("size error!\n");
		goto err3;
	}

	*errMsg = 0;
	*length = nOffset;

	// Alles klar~ :)

	return res;

err1:
	*errMsg = 201;	// CertRespType Error
	goto err;
err2:
	*errMsg = 202;	// Memory Allocation Error
	goto err;
err3: 
	*errMsg = 203;	// Invalid MIME
	goto err;
err:
	if( res ) {
		Free(res);
	}

	return NULL;
}

//
//	End of create_cert_response.c
//
//---------------------------------
