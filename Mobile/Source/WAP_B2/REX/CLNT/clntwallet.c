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

#ifdef WAP_NEO
	#include "wiptrgt.h"
#else //#ifdef WAP_NEO
	#include "waptarget.h"
#endif //#ifdef WAP_NEO

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
#include "waptask.h"

#include "script_enum.h"
#include "wap_mime.h"
#include "clntwallet.h"

#include "sky.h"

#define CUR_CERT_RESPONSE_VERSION		1

#define READ_FROM_BUF( source, target, length, offset ) \
			{ memcpy(target, &source[offset], length); offset += length; }

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

#define SEARCH_FAILURE 0
#define SEARCH_SUCCESS 1
#define MAX_TEMP_CERT 2

int clnt_search_user_cert(char *skid, int* index)
{
	int i;

	for(i=0; i<MAX_TEMP_CERT; i++)
	{
		if(!strcmp(skid, (const char *)SKY_GET_MW_TEMPCERT_SKID(i)))
		{
			*index = i;
			return SEARCH_SUCCESS;
		}
	}
	return SEARCH_FAILURE;
}

void clnt_free_user_cert(CERT_RESPONSE *cert)
{	
	free_cert_response(cert);
}

/*
typedef struct _cert_response {
	char * name;
	unsigned char akid[20];		// authority(issuer) key identifier
	unsigned char skid[20];		// subject key identifier
	unsigned char * cert;
	int cert_len;
	int type;					// CertRespType
} CERT_RESPONSE;
*/
#define TEMP_CERT 2
void clnt_write_user_cert(CERT_RESPONSE *cert, int certIndex)
{
	int i=certIndex;
	int tmpCertIndex;
	int saved=1;

	//Temporary SKID search success case
	if(clnt_search_user_cert((char *)cert->skid, &tmpCertIndex))
	{
		SKY_SET_MW_MYCERT_PIN_HASH(i,SKY_GET_MW_TEMPCERT_PIN_HASH(tmpCertIndex),20);		    
		SKY_SET_MW_MYCERT_PRV_KEY(i,SKY_GET_MW_TEMPCERT_PRV_KEY(tmpCertIndex),96);		    

		SKY_SET_MW_MYCERT_DISPLAY(i,(unsigned char *)cert->name,strlen(cert->name));

		SKY_SET_MW_MYCERT_SUBJECT_KID(i,cert->skid,strlen((const char *)cert->skid));
		SKY_SET_MW_MYCERT_ISSUER_KID(i,cert->akid,strlen((const char *)cert->akid));
		SKY_SET_MW_MYCERT_CERT_URL(i,cert->cert,strlen((const char *)cert->cert));
		SKY_SET_MW_MYCERT_SUBJECT_DN(i,(unsigned char *)cert->cert_len,strlen((const char *)cert->cert_len));

		//SKY_SET_MW_MYCERT_CHECKFIELD(i,s,len);			
		//SKY_SET_MW_MYCERT_LENGTH(i,s,len)		
		//Temporarily blocked for compilation by swjung

		SKY_SET_MW_MYCERT_DATA_SAVED(i,saved);


		//Reset temp cert
		//sInitMW(TEMP_CERT);

		//Show user cert info
	}
	else
	{
		//Show user CSR failure
	}

	clnt_free_user_cert(cert);
}


CERT_RESPONSE * clnt_process_user_cert(const char *data, int length, int* error)
{
	return acceptCertResponse((const unsigned char *)data, length, error);
}

TBHTrustedCAInfo* clnt_process_hashed_cert(const char *data, int length, int *error)
{
	return parseHashedCertificate((unsigned char *)data, length, error);
}

SignedTrustedCAInfo* clnt_process_signed_cert(const char *data, int length, int *error)
{
	return parseSignedCertificate((unsigned char *)data, length, error);
}

