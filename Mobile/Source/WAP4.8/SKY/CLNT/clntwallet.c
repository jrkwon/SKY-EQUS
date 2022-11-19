/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           Wireless Application Protocol   Client Function

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS
  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/22/01   swjung  Porting SecureSoft's Certificate handling routine(acceptCertResponse)
===========================================================================*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "customer.h"

#include "logcodes.h"
#include "errcodes.h"
#include "confvars.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "si_libs.h"

#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif

#include "GenDsp.h"
#include "mmi_data.h"
#include "mmi_func.h"

#include "customer.h"

#include "clnt.h"
#include "clntapp.h"
#include "wapconnector.h"
#include "clntwallet.h"

#include "script_enum.h"
#include "wap_mime.h"
#include "cert_manage.h"

#include "sky.h"


int clnt_check_mime(const char *mime)
{
	if(!strcmp(mime, CERT_RESPONSE_MIME))
		return	CERT_RESPONSE_TYPE;
	else if(!strcmp(mime, HASHED_CA_CERT_RESPONSE_MIME))
		return	HASHED_CA_CERT_RESPONSE_TYPE;
	else if(!strcmp(mime, SIGNED_CA_CERT_RESPONSE_MIME))
		return	SIGNED_CA_CERT_RESPONSE_TYPE;
	else
		return	UNKNOWN_TYPE;
}


//Function for WMLS lib signTextEx()
//You can get my cert index that is matched to key ID type and key ID
//By doing this, you can compare user inputted hashed PIN with EFS saved hashed PIN indexed with 
//this function's return value.
int clnt_get_cert_index(int keyIDType, const unsigned char *keyID)
{
	return getEnumByID(keyIDType, keyID);
}

///////////////////////////////////////////////////////////////////////////////////
//SKID of TCA is hashed DN string
//AKID of my CERT will be changed to hashed subject DN string by server
///////////////////////////////////////////////////////////////////////////////////
int clnt_search_skt_user_cert(char *akid, int* index)
{
	int i;
	//For debugging only
	char tmpAKID[100];

	for(i=0; i<MAX_MY_CERT; i++)
	{
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpAKID, (const char *)SKY_GET_MW_MYCERT_ISSUER_KID(i), 20);

		CLNTa_log(1,0,"[PAS:AKID][%s]",akid);

		CLNTa_log(1,0,"[EFS:AKID][%s]",tmpAKID);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(akid, (const char *)SKY_GET_MW_MYCERT_ISSUER_KID(i), 20))
		{
			*index = i;
			CLNTa_log(1,0,"[AKID Matched] Success");
			return MW_SUCCESS;
		}
	}

	CLNTa_log(1,0,"[SKID Mismatched] Failure");
	return MW_FAILURE;
}

//Search my cert list with skid
int clnt_search_my_cert(char *skid, int* index)
{
	int i;
	//For debugging only
	char tmpSKID[100];

	for(i=0; i<MAX_MY_CERT; i++)
	{
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpSKID, (const char *)SKY_GET_MW_MYCERT_SUBJECT_KID(i), 20);

		CLNTa_log(1,0,"[PAS:SKID][%s]",skid);

		CLNTa_log(1,0,"[EFS:SKID][%s]",tmpSKID);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(skid, (const char *)SKY_GET_MW_MYCERT_SUBJECT_KID(i), 20))
		{
			*index = i;
			CLNTa_log(1,0,"[SKID Matched] Success");
			return MW_SUCCESS;
		}
	}

	CLNTa_log(1,0,"[SKID Mismatched] Failure");
	return MW_FAILURE;
}


//Search temporary cert with skid for downloading my cert
int clnt_search_temp_cert(char *skid, int* index)
{
	int i;
	//For debugging only
	char tmpSKID[100];

	for(i=0; i<MAX_TEMP_CERT; i++)
	{	
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpSKID, (const char *)SKY_GET_MW_TEMPCERT_SKID(i), 20);

		CLNTa_log(1,0,"[PAS:SKID][%s]",skid);

		CLNTa_log(1,0,"[EFS:SKID][%s]",tmpSKID);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(skid, (const char *)SKY_GET_MW_TEMPCERT_SKID(i), 20))
		{
			*index = i;
			CLNTa_log(1,0,"[SKID Matched] Success");
			return MW_SUCCESS;
		}
	}

	CLNTa_log(1,0,"[SKID Mismatched] Failure");
	return MW_FAILURE;
}

//Search my cert with subject dn string and find out index of matched cert for cert revoke
int clnt_search_user_cert_dn(char *subject_dn, int* index)
{
	int i;
	int dnLength = strlen(subject_dn);
	//For debugging only
	char tmpSubjectDN[400];

	for(i=0; i<MAX_MY_CERT; i++)
	{	
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpSubjectDN, (const char *)SKY_GET_MW_MYCERT_SUBJECT_DN(i), dnLength);

		CLNTa_log(1,0,"[PAS:tmpSubjectDN:%d][%s]",subject_dn, dnLength);

		CLNTa_log(1,0,"[EFS:tmpSubjectDN][%s]",tmpSubjectDN);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(subject_dn, (const char *)SKY_GET_MW_MYCERT_SUBJECT_DN(i), dnLength))
		{
			*index = i;
			CLNTa_log(1,0,"[tmpSubjectDN Matched] Success");
			return MW_SUCCESS;
		}
	}

	CLNTa_log(1,0,"[tmpSubjectDN Mismatched] Failure");
	return MW_FAILURE;
}

//Search tca cert with skid
int clnt_search_tca_cert(char *skid)
{
	int i;
	//For debugging only
	char tmpSKID[100];

	for(i=0; i<MAX_TCA_CERT; i++)
	{
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpSKID, (const char *)SKY_GET_MW_TCACERT_SUBJECT_KID(i), 20);

		CLNTa_log(1,0,"[PAS:SKID][%s]",skid);

		CLNTa_log(1,0,"[EFS:SKID][%s]",tmpSKID);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(skid, (const char *)SKY_GET_MW_TCACERT_SUBJECT_KID(i), 20))
		{
			CLNTa_log(1,0,"[SKID Matched] Success");
			return MW_SUCCESS;
		}
	}

	CLNTa_log(1,0,"[SKID Mismatched] Failure");
	return MW_FAILURE;
}


char *clnt_get_tca_cert(char *skid)
{
	int i;
	//For debugging only
	char tmpSKID[100];
	char *pCertResult=NULL;

	for(i=0; i<MAX_TCA_CERT; i++)
	{
		///////////////////////////////////////////////////////////////////////////
		//For debugging only
		memcpy(tmpSKID, (const char *)SKY_GET_MW_TCACERT_SUBJECT_KID(i), 20);

		CLNTa_log(1,0,"[PAS:SKID][%s]",skid);

		CLNTa_log(1,0,"[EFS:SKID][%s]",tmpSKID);
		///////////////////////////////////////////////////////////////////////////

		if(!strncmp(skid, (const char *)SKY_GET_MW_TCACERT_SUBJECT_KID(i), 20))
		{
			pCertResult = (char *)WIPAlloc(MAX_TCA_CERT_SIZE+2);

			memset(pCertResult, 0, MAX_TCA_CERT_SIZE+2);
			memcpy(pCertResult, SKY_GET_MW_TCACERT_CERT(i), MAX_TCA_CERT_SIZE);

			CLNTa_log(1,0,"[SKID Matched] Success[length]");
			return pCertResult;
		}
		else if(!strncmp(skid, (const char *)SKY_GET_MW_TCACERT_ISSUER_KID(i), 20))
		{
			pCertResult = (char *)WIPAlloc(MAX_TCA_CERT_SIZE+2);

			memset(pCertResult, 0, MAX_TCA_CERT_SIZE+2);
			memcpy(pCertResult, SKY_GET_MW_TCACERT_CERT(i), MAX_TCA_CERT_SIZE);

			CLNTa_log(1,0,"[AKID Matched] Success[length]");
			return pCertResult;
		}
	}

	CLNTa_log(1,0,"[SKID/AKID Mismatched] Failure");
	return NULL;
}


//Search Temp Cert skid matched with downloaded cert
int clnt_write_user_cert(CERT_RESPONSE *cert, int efs_pos,int log_pos)
{
	int tmpCertIndex=0;
	
	//Temporary SKID search failure case
	if(clnt_search_temp_cert((char *)cert->skid, &tmpCertIndex))
	{
		CLNTa_log(1,0,"[SKID][%s]",cert->skid);

		SKY_SET_MW_MYCERT_PIN_HASH(efs_pos,SKY_GET_MW_TEMPCERT_PIN_HASH(tmpCertIndex),20);		    
		SKY_SET_MW_MYCERT_PRV_KEY(efs_pos,SKY_GET_MW_TEMPCERT_PRV_KEY(tmpCertIndex),96);
		SKY_SET_MW_MYCERT_SUBJECT_DN(efs_pos,SKY_GET_MW_TEMPCERT_SUBJECT_DN(tmpCertIndex),400);

		SKY_SET_MW_MYCERT_DISPLAY(efs_pos,(unsigned char *)cert->name,strlen(cert->name));

		SKY_SET_MW_MYCERT_SUBJECT_KID(efs_pos,cert->skid,strlen((const char *)cert->skid));
		SKY_SET_MW_MYCERT_ISSUER_KID(efs_pos,cert->akid,strlen((const char *)cert->akid));
		SKY_SET_MW_MYCERT_CERT_URL(efs_pos,cert->cert,strlen((const char *)cert->cert));

		SKY_SET_MW_MYCERT(efs_pos);
		//SKY_SET_MW_MYCERT_CHECKFIELD(efs_pos,s,len);			
		//SKY_SET_MW_MYCERT_LENGTH(efs_pos,s,len)		
		//Temporarily blocked for compilation by swjung

		SKY_SET_MW_MYCERT_DATA_SAVED(efs_pos,log_pos);

		// 해당 tempcert 삭제
		InitMWTempCert(tmpCertIndex);
		SKY_SET_MW_TEMPCERT_DATA_SAVED(tmpCertIndex,0);

		SKY_SET_MW_WALLET();

		return 1;
	}
	
	return 0;
}

CERT_RESPONSE * clnt_process_user_cert(const char *data, int length, int* error)
{
	return acceptCertResponse((const unsigned char *)data, length, error);
}
