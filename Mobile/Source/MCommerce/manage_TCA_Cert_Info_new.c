//-------------------------------
//
//  manageTCACertInfo.c
//
//  - manage and handle the TCACertInfo in the Phone
//
//  2001. 6. created by Ji-Hyun Sohn
//
//-------------------------------

#include "crypto.h"
#include "stdio.h"
#include "wap_mime.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#include "sky.h"
#include "clntwallet.h"
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// 단말기에서 해당하는 CA 인증서가 존재하는 지의 여부를 판별한다.
//
// parameter
// 		- keyIDType : SKID(1) 또는 AKID(2) 인지 여부
// 		- keyID     : SKID 또는 AKID의 값
// 		- keyIDLen  : keyID의 길이
// 		- certLen   : 인증서의 길이
//		- errMsg	: 0(존재할 경우), 1001(존재하지 않을 경우)
//
// return value
// 		- WTLS 인증서


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//At this point, we do not consider that keyIdType is not SKID.. swjung

int searchCACert( int keyIDType, unsigned char *keyID) 
{
	return clnt_search_tca_cert((char *)keyID);
}

unsigned char *getCACert(int keyIDType, unsigned char *keyID, int *certLen)
{
	return (unsigned char *)clnt_get_tca_cert((char *)keyID);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


// 단말기의 ROM 저장영역에 해당하는 CA의 정보를 저장한다.
//
// return value
// 		- 0   : 저장에 성공할 경우
// 		- 1101 : 저장에 실패할 경우

int storeNewCAInfo( unsigned char *skid, int skidLen, char *displayName,
		unsigned char *cert, int certLen, unsigned char *akid, int akidLen, 
		int cert_type, unsigned long not_after ) {

#ifdef BLOCK
	//SKY_SET_MW_TCACERT_LENGTH(certIndex,BYTE *s,int len);		
	//SKY_SET_MW_TCACERT_CHECKFIELD(certIndex,BYTE *s,int len);		
	SKY_SET_MW_TCACERT_DISPLAY(certIndex,pHashedResult->displayName,strlen(pHashedResult->displayName));		
	//SKY_SET_MW_TCACERT_SUBJECT_KID(certIndex,BYTE *s,int len);		
	//SKY_SET_MW_TCACERT_ISSUER_KID(certIndex,BYTE *s,int len);
	SKY_SET_MW_TCACERT_CERT(certIndex,pHashedResult->trustedCACert,strlen(pHashedResult->trustedCACert));		
	SKY_SET_MW_TCACERT_CA_INFO_URL(certIndex,pHashedResult->cainfo_url,strlen(pHashedResult->cainfo_url));		
	//SKY_SET_MW_TCACERT_SUBJECT_DN(certIndex,BYTE *s,int len);		
	//SKY_SET_MW_TCACERT_NOT_AFTER(certIndex,BYTE *s,int len);		
	SKY_SET_MW_TCACERT(certIndex);								
#endif //#ifdef BLOCK

	return 0;
}

