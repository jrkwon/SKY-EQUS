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

// �ܸ��⿡�� �ش��ϴ� CA �������� �����ϴ� ���� ���θ� �Ǻ��Ѵ�.
//
// parameter
// 		- keyIDType : SKID(1) �Ǵ� AKID(2) ���� ����
// 		- keyID     : SKID �Ǵ� AKID�� ��
// 		- keyIDLen  : keyID�� ����
// 		- certLen   : �������� ����
//		- errMsg	: 0(������ ���), 1001(�������� ���� ���)
//
// return value
// 		- WTLS ������


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


// �ܸ����� ROM ���念���� �ش��ϴ� CA�� ������ �����Ѵ�.
//
// return value
// 		- 0   : ���忡 ������ ���
// 		- 1101 : ���忡 ������ ���

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

