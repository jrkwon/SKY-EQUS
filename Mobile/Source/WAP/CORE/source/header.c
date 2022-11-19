/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: Header.C

	Description:

	Author: Ingmar Persson, AU-System Radio AB

	Revision history:
  	Date    Rev  Sign  Comment
  	980917  PA1  IPN    First version
	990218       IPN    Latest version
    001205       HEAD   CreateConnectHeader ignores the ev. PROFILE DIFF.
	001211       IPN    Removed CreateCapabilities and ParseCapabilities.
------------------------------------------------------------------------*/

/* Private include statements */

#include "header.h"
#include "cache.h"
#include "hdrutil.h"
#include "cmmnrsrc.h"
#include "waedef.h"
#include "url.h"
#include "wae_cmmn.h"
#include "waedef.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "confvars.h"
#include "aapimmi.h"
#include "dbfunct.h"

#ifdef FEATURE_WAP
#include "mccdma.h"
extern int get_position(char *result);
extern BOOL makeagent;
BYTE agent[100];
WORD brow_length = 0;
static void MakeMin(BYTE* pT);
static void MakeB5_info(void);
#endif//#ifdef FEATURE_WAP

#ifdef CONFIG_CO
	#include "wspcapa.h"
#endif

#ifdef CONFIG_PUSH
	#define APP_ID_SIA		1
	#define APP_ID_WML_UA	2
	#define APP_ID_WTA_UA	3
#endif

typedef struct
{
	int					iObjectID;
	int					iSID;
	void*				pvReqStruct;
	int					iOptions;
	int					iPrio;
	unsigned long int	uiPid;
	char				chWaitingForChannel;		/* Kan ev strykas om man går på SID */
} TRANSACTIONSTRUCT, *pTRANSACTIONSTRUCT;

typedef struct
{
	int					iSID;
	unsigned long int	uiPid;
	int					iConnected;
	int					iNrOfFreeTransaction;
} SESSIONSTRUCT, *pSESSIONSTRUCT;

typedef struct
{
	unsigned long int	uiSMPid;
	unsigned long int	uiTRPid;
} ACTIVETRANSSTRUCT, *pACTIVETRANSSTRUCT;

typedef struct
{
	unsigned long int uiPid;
} PIDSTRUCT, *pPIDSTRUCT;


const INT16 arrayAcceptCharset[NR_OF_ACCEPTCHARSET] = INIT_ACCEPTCHARSET;


UASETTINGS* New_UASETTINGS(void)
{
   UASETTINGS*  pUASettings;
   pUASettings = NEWSTRUCT(UASETTINGS);

   if (pUASettings)
   {
 	  pUASettings->pbAccept=NULL;                             
      pUASettings->pbProxyCredentials=NULL;
	  pUASettings->iProxyCredentialsLength=0;   
	  pUASettings->pbLanguage=NULL;
	  pUASettings->iLanguage_Length=0;
   }
   return pUASettings;
}

void Delete_UASETTINGS(UASETTINGS** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
    DEALLOC(&((*ppTheObject)->pbAccept));
    DEALLOC(&((*ppTheObject)->pbProxyCredentials));
    DEALLOC(&((*ppTheObject)->pbLanguage));
    DEALLOC(ppTheObject);
  }
}

NETWORKSTRUCT* New_NETWORKSTRUCT(void)
{
    NETWORKSTRUCT*  pNetwStruct;
    pNetwStruct = NEWSTRUCT(NETWORKSTRUCT);
	if (pNetwStruct)
    {
	  pNetwStruct->iStackMode=0;
	  pNetwStruct->iActiveBearer=-1;
	  pNetwStruct->iClientPort=-1;
	  pNetwStruct->pbUDP_CLIENT_IP=NULL;
	  pNetwStruct->pbUDP_CLIENT_IP_LEN=0;

	  pNetwStruct->pbUDP_GW_IP=NULL;
	  pNetwStruct->pbUDP_GW_IP_LEN=0;
	  
	  pNetwStruct->pbSMS_CENTER=NULL;
	  pNetwStruct->pbSMS_CENTER_LEN=0;
	  
	  pNetwStruct->pbSMS_GW=NULL;
	  pNetwStruct->pbSMS_GW_LEN=0;
	  
	  pNetwStruct->pbUSSD_CENTER=NULL;
	  pNetwStruct->pbUSSD_CENTER_LEN=0;
	  
	  pNetwStruct->iUSSD_SC_TYPE=-1;
	  pNetwStruct->pbUSSD_SC=NULL;
	  pNetwStruct->pbUSSD_SC_LEN=0;
	  pNetwStruct->iTimeOut=TIMEOUT_VALUE_INVALID;
    }
	return pNetwStruct;
}

void Delete_NETWORKSTRUCT(NETWORKSTRUCT** ppTheObject)
{
  if (*ppTheObject !=NULL)
  {
	DEALLOC(& ((*ppTheObject)->pbUDP_CLIENT_IP ));
	DEALLOC(&((*ppTheObject)->pbUDP_GW_IP));
	DEALLOC(&((*ppTheObject)->pbSMS_CENTER));
	DEALLOC(&((*ppTheObject)->pbSMS_GW));
	DEALLOC(&((*ppTheObject)->pbUSSD_CENTER));
	DEALLOC(&((*ppTheObject)->pbUSSD_SC));
    DEALLOC(ppTheObject);
  }
}


/*========================================================================
	INTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	RemoveSpace 
==========================================================================
Purpose: Removes all space characters from the src string
Params:  pchSrc BYTE*
Return: 
=========================================================================*/
VOID RemoveSpace (BYTE* pchSrc)
{
	BYTE* pchTemp = pchSrc;

	while (*pchSrc) {
		if (*pchSrc == ' ')	{
			pchSrc++;
		}
		else {
			*pchTemp++ = *pchSrc++;
		}
	}
	*pchTemp = '\0';
} /* end of RemoveSpace */

/*========================================================================
	FindSegment 
==========================================================================
Purpose: Search after a ',' character in src string
Params:  pchSrc BYTE*
Return:  A pointer where the ',' character was found
=========================================================================*/
BYTE* FindSegment( BYTE* pchSrc )
{

	while( *pchSrc != 0 ){
		if( *pchSrc == ',' )
			return pchSrc;
		pchSrc++;
	}
	return NULL;

} /* end of FindSegment */

/*========================================================================
	GetCacheControl 
==========================================================================
Purpose: Get the CacheControl value from pHeadList
Params:  pHeadList	pHEADERELEMENT
Return:  cachecontrol value.
=========================================================================*/
int GetCacheControl( pHEADERELEMENT pHeadList )
{
	pHEADERELEMENT	pHeaderElement = NULL;
	unsigned char	ucCacheControl = 0;
	BOOL			boError = FALSE;
	UINT32			uiDeltaSec = 0;
	int				iCachePrivate = cfg_wae_cc_cachePrivate;

	if( pHeadList != NULL ){
		pHeaderElement = WSP_GetHeaderWK ( 1, Field_Cache_Control, pHeadList );
		if( pHeaderElement != NULL )
		{
			ucCacheControl = WSP_GetCacheControlByte( pHeaderElement, &uiDeltaSec, &boError );
				/* If the cache-directive is Private and it is allowed to cache Privat content,
						change the directive to Public.*/
			if( ucCacheControl == 135 && iCachePrivate == 1 )
				ucCacheControl = 134; /* Public */
		}
			/* If the header field Field_Vary exists, the response should not be stored in the cache */
		pHeaderElement = WSP_GetHeaderWK ( 1, Field_Vary, pHeadList );
		if( pHeaderElement != NULL )
		{
			ucCacheControl = 0x80;
		}
	}

	return (int)ucCacheControl;

} /* end of GetCacheControl */

/*========================================================================
	CheckIfMultipart 
==========================================================================
Purpose: Check if the contentType is Multipart
Params:  pHeadList	pHEADERELEMENT
Return:  1 if Multipart.
=========================================================================*/
int CheckIfMultipart( pHEADERELEMENT pHeadList )
{
	pHEADERELEMENT	pHeaderElement = NULL;
	BOOL			boError = FALSE;
	pWSPPARAMETERS	pParameters = NULL;
	BYTE			bContentType = 0;
	int				iReturn = 0;
	

	if( pHeadList != NULL ){
		pHeaderElement = WSP_GetHeaderWK ( 1, Field_Content_Type, pHeadList );
		if( pHeaderElement != NULL ){
			bContentType = WSP_GetContentTypeByte( pHeaderElement, &boError, &pParameters );
				/* Check if ContentType is application/vnd.wap.multipart.related */
			if( !boError && bContentType == 0x33 )
				iReturn = 1;
			if( pParameters != NULL )
				WSP_DeleteParameters( &pParameters );
		}
	}
	return iReturn;

} /* end of CheckIfMultipart */

/*========================================================================
	WriteContentIntoCache 
==========================================================================
Purpose: Write Url, Header and Body into the cache
Params:  pvURL			void*
		 pvHeader		void*
		 iHeaderLength	int
		 pvBody			void*
		 iBodyLength	int
Return:
=========================================================================*/
void WriteContentIntoCache( void* pvURL, void* pvHeader, int iHeaderLength, 
						   void* pvBody, int iBodyLength, int iUaType )
{

#ifdef LOG_INTERNAL
	CLNTa_log(0, 0, "Cache-Control: Write to Cache, URL: %s\n", pvURL?pvURL:"" );
#endif
	if (iUaType==WTA_USER_AGENT)
		Cache_Write( pvURL, TRUE, pvHeader, iHeaderLength, pvBody, iBodyLength);
	else
		Cache_Write( pvURL, FALSE, pvHeader, iHeaderLength, pvBody, iBodyLength);
} /* end of WriteContentIntoCache */

/*========================================================================
	TryWriteToCache
==========================================================================
Purpose: Examine if the content may be stored into the cache referenced 
		 to the UserAgent type, CacheHeaders and if the cache is in Idle.
Params:  pvURL				void*
		 iUaType			int
		 iCacheInIdle		int
		 iCacheDirektive	int
		 pHeaderList		pHEADERDEF
		 pvHeader			void*
		 iHeaderLength		int
		 pvBody				void*
		 iBodyLength		int
Return:  
=========================================================================*/
void TryWriteToCache( void* pvURL, int iUaType, int iCacheInIdle, int iCacheDirektive,
					 pHEADERDEF	pHeaderList, void* pvHeader, int iHeaderLength, 
						   void* pvBody, int iBodyLength )
{
	int	iCacheControl = 0;

	if( iUaType != REP_UA && iCacheInIdle == 1 ){
			/* if the PushHandler made the request, this request is a CacheOperation 
			and the response will be cached irrespective what the Cache header said. */
		if( iUaType == PUSH_UA ) 
				/* cache */
			WriteContentIntoCache( pvURL, pvHeader, iHeaderLength, pvBody, iBodyLength, iUaType );
		else{
			iCacheControl = GetCacheControl( pHeaderList->pHeadList );
			if( iCacheDirektive == NOT_CACHE_CONTENT || iCacheControl == NoCache || 
					iCacheControl == NoStore || iCacheControl == Private ){
					/*No cache*/
				return;
			}
			else{
					/* cache */
				WriteContentIntoCache( pvURL, pvHeader, iHeaderLength, pvBody, iBodyLength, iUaType);
			}
		}
	}
	else
			/*No cache*/
		return;

} /* end of TryWriteToCache */


/*========================================================================
	EXTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	urlIsValid
==========================================================================
Purpose: Check if a URL is valid and has at least a scheme, host, and 
		 path component.
Params:  pvReqStruct	pURLREQSTRUCT	casted to a VOID*
Return:  TRUE if valid, otherwise FALSE
=========================================================================*/
int urlIsValid( VOID* pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*			pbFrag = NULL;

	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL ){
#ifdef FEATURE_WAP
			pbFrag =(BYTE *)B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
#else
            pbFrag = B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
#endif //FEATURE_WAP
			if( pbFrag != NULL )
				*pbFrag = 0;
			if( b_UrlIsCompleteValid( pUrlReq->pbURL ) ){
				return 1;
			}
		}
	}

	return 0;
}  /* end of urlIsValid */

/*========================================================================
	FreeReqStruct 
==========================================================================
Purpose: Release a URLREQSTRUCT and all of it's parts
Params:  pvReqStruct	pURLREQSTRUCT*	casted to a VOID**
Return:	 1
=========================================================================*/
int FreeReqStruct( VOID** pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;

	if( *pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)(*pvReqStruct);

		Delete_URLREQSTRUCT( &pUrlReq );
		*pvReqStruct = NULL;
	}

	return 1;

} /* end of FreeReqStruct */

/*========================================================================
	FreeNetStruct 
==========================================================================
Purpose: Release a NETWORKSTRUCT and all of it's parts
Params:  vsNetStruct	pNETWORKSTRUCT*	casted to a VOID**
Return:	 1
=========================================================================*/
int FreeNetStruct(VOID** vsNetStruct)
{
	pNETWORKSTRUCT	pNetWorkStruct = NULL;

	if( *vsNetStruct != NULL )
	{
		pNetWorkStruct = (pNETWORKSTRUCT)(*vsNetStruct);

		Delete_NETWORKSTRUCT( &pNetWorkStruct );
		*vsNetStruct = NULL;
	}

	return 1;

} /* end of FreeNetStruct */

/*========================================================================
	FreeUaSettingsStruct 
==========================================================================
Purpose: Release a UASETTINGSSTRUCT and all of it's parts
Params:  vsStruct	pUASETTINGS*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeUaSettingsStruct(VOID** vsStruct)
{
	pUASETTINGS	pUaSettings = NULL;

	if( *vsStruct != NULL )
	{
		pUaSettings = (pUASETTINGS)(*vsStruct);

		Delete_UASETTINGS( &pUaSettings );
		*vsStruct = NULL;
	}

	return 1;

} /* end of FreeUaSettingsStruct */
/*========================================================================
	GetMethod 
==========================================================================
Purpose: Get the Method part from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
Return:  Method
=========================================================================*/
int GetMethod( VOID* pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	int				iReturn = 0;

	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		iReturn = pUrlReq->iMethod;
	}

	return iReturn;
}  /* end of GetMethod */

/*========================================================================
	GetUrl 
==========================================================================
Purpose: Get the Url part from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvUrl			BYTE**			casted to VOID**
Return:  length of pvUrl
=========================================================================*/
int GetUrl( VOID* pvReqStruct, VOID** pvUrl )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*		pbTemp2 = NULL;
	UINT16		uiUrlLength = 0;

	*pvUrl = NULL;

	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;

		if( pUrlReq->pbURL != NULL ){
			uiUrlLength = B_STRINGLENGTH( pUrlReq->pbURL );
			pbTemp2 = NEWARRAY(BYTE,uiUrlLength+1);
			if( pbTemp2 != NULL ){
				B_COPYSTRING( pbTemp2, pUrlReq->pbURL );
				pbTemp2[uiUrlLength] = 0;
				*pvUrl = (VOID*)pbTemp2;
			}
			else
				uiUrlLength = 0;
		}
	}

	return (int)uiUrlLength;
} /* end of GetUrl */

/*========================================================================
	GetHeader 
==========================================================================
Purpose: Create a WSP-Header, pvHeader, from the input parameters.
Params:  pvReqStruct				URLREQSTRUCT*	casted to VOID*
		 pvUaSettings				UASETTINGS*		casted to VOID*
		 pNetStruct					NETWORKSTRUCT*	casted to VOID*
		 pvWAEMaStruct				WAEMAINOBJECT*	casted to VOID*
		 iConnectionMode			int (0=CL, 1=CM)
		 iProfileEnabled			int (1=UAProf is enabled)
		 vsProxyCredentials			BYTE*			casted to VOID*
		 iProxyCredentialsLength	int
		 pvHeader					BYTE**			casted to VOID**
Return:  length of pvHeader
=========================================================================*/
int GetHeader( VOID* pvReqStruct, VOID* pvUaSettings, VOID* pNetStruct, VOID* pvWAEMaStruct,
			  int iConnectionMode, int iProfileEnabled, VOID* vsProxyCredentials,
			  int iProxyCredentialsLength, VOID** pvHeader )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	pUASETTINGS		pUaSet = NULL;
	pHEADBUILDSTR	pList = NULL;
	pHEADBUILDSTR	pStartList = NULL;
	pWAEMAINOBJECT	pWAEMainStruct = NULL;
	BYTE*		pbString = NULL;
	BYTE*		pbTemp = NULL;
	BYTE*		pbSeparator = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbFrag = NULL;
	UINT32		uiLength = 0;
	UINT32		uiDataLength = 0;
	UINT16		i = 0;
	UINT8		uiLen = 0;
	BYTE		tempArray[2];

	if( pvReqStruct != NULL && pvUaSettings != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		pUaSet = (pUASETTINGS)(pvUaSettings);

		if( pvWAEMaStruct != NULL )
			pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		if( (iConnectionMode == 0 && pUrlReq->pbAccept != NULL) || 
			(iConnectionMode == 1 && pUrlReq->pbAccept != NULL && B_COMPARESTRING(pUrlReq->pbAccept,pUaSet->pbAccept) != 0) )
		{
			uiLength = B_STRINGLENGTH( pUrlReq->pbAccept );
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL )
				B_COPYSTRING( pbString, pUrlReq->pbAccept );
		}
		else if( iConnectionMode == 0 && pUaSet->pbAccept != NULL ){
			uiLength = B_STRINGLENGTH( pUaSet->pbAccept );
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL )
				B_COPYSTRING( pbString, pUaSet->pbAccept );
		}
		if( pbString != NULL ){
			RemoveSpace( pbString );
			uiLength = B_STRINGLENGTH( pbString );

			pbTemp = pbString;
			pbSeparator = FindSegment( pbTemp );
			while( pbSeparator != NULL )
			{
				*pbSeparator = 0;
				pbSeparator++;
				uiDataLength = B_STRINGLENGTH( pbTemp );
				WSP_CreateHeaderField( Field_Accept, pbTemp, (uiDataLength+1), &pList);
				if( pStartList == NULL )
					pStartList = pList;
				pbTemp = pbSeparator;
				pbSeparator = FindSegment( pbTemp );
			}
			uiDataLength = B_STRINGLENGTH( pbTemp );
			WSP_CreateHeaderField( Field_Accept, pbTemp, (uiDataLength+1), &pList);
			if( pStartList == NULL )
				pStartList = pList;

			pbTemp = NULL;
			pbSeparator = NULL;
			DEALLOC( &pbString );
		}
		uiLength = 0;

/* Only in ConnectionLess Mode */
		if( iConnectionMode == 0 && pUaSet->pbLanguage != NULL && pUaSet->iLanguage_Length > 0 ){
			for ( i=0; i<pUaSet->iLanguage_Length; i++ ){
				WSP_CreateHeaderField( Field_Accept_Language, &(pUaSet->pbLanguage[i]), 1, &pList);
				if( pStartList == NULL )
					pStartList = pList;
			}
		}

/* Only in ConnectionLess Mode */
		if( iConnectionMode == 0 && NR_OF_ACCEPTCHARSET > 0 ){
			for ( i=0; i<NR_OF_ACCEPTCHARSET; i++ ){
				if( arrayAcceptCharset[i] < 256 ){
					tempArray[0] = arrayAcceptCharset[i] & 0xff;

					WSP_CreateHeaderField( Field_Accept_Charset, tempArray, 1, &pList);
				}
				else{
					tempArray[0] = (arrayAcceptCharset[i] >> 8) & 0xff;
					tempArray[1] = arrayAcceptCharset[i] & 0xff;

					WSP_CreateHeaderField( Field_Accept_Charset, tempArray, 2, &pList);

				}
				if( pStartList == NULL )
					pStartList = pList;
			}
		}

		if( pUrlReq->pbReferer != NULL )
		{
#ifdef FEATURE_WAP
			pbFrag =(BYTE *)B_STRSTRING( pUrlReq->pbReferer , (BYTE*)"#" );
#else
            pbFrag = B_STRSTRING( pUrlReq->pbReferer , (BYTE*)"#" );
#endif //FEATURE_WAP


			if( pbFrag != NULL )
				uiDataLength = pbFrag-(pUrlReq->pbReferer);
			else
				uiDataLength = B_STRINGLENGTH( pUrlReq->pbReferer );

			pbString = NEWARRAY(BYTE,uiDataLength+1);
			if( pbString != NULL ){
				B_COPYSTRINGN( pbString, pUrlReq->pbReferer, uiDataLength );
				pbString[uiDataLength] = 0;
				WSP_CreateHeaderField( Field_Referer, pbString, (uiDataLength+1), &pList);
				if( pStartList == NULL )
					pStartList = pList;

				DEALLOC( &pbString );
			}
			uiLength = 0;
		}

		if( pUrlReq->iLastModified != 0 )
		{
			pbString = NEWARRAY(BYTE,4);
			if( pbString != NULL ){
				pbString[0] = (BYTE)((pUrlReq->iLastModified >> 24) & 0xFF);
				pbString[1] = (BYTE)((pUrlReq->iLastModified >> 16) & 0xFF);
				pbString[2] = (BYTE)((pUrlReq->iLastModified >> 8) & 0xFF);
				pbString[3] = (BYTE)(pUrlReq->iLastModified & 0xFF);

				WSP_CreateHeaderField( Field_If_Modified_Since, pbString, 4, &pList);
				if( pStartList == NULL )
					pStartList = pList;

				DEALLOC( &pbString );
			}
		}

		if( pUrlReq->pbEtag != NULL )
		{
			uiLength = B_STRINGLENGTH( pUrlReq->pbEtag );
			WSP_CreateHeaderField( Field_If_None_Match, pUrlReq->pbEtag, (uiLength+1), &pList);
			if( pStartList == NULL )
				pStartList = pList;
		}

		pbString = NULL;
		if( checkHostAuth( pUrlReq->pbURL, NULL, &pbString ) ){
			if( pbString != NULL )
			{
				uiLen = ((UINT16)*pbString)-2;
				pbString+=2;
				WSP_CreateHeaderField( Field_Authorization, pbString, uiLen, &pList);
				if( pStartList == NULL )
					pStartList = pList;
			}
		}


			/*Try new values for Proxy Authorization*/
		if( vsProxyCredentials != NULL && iProxyCredentialsLength > 0 ){
			WSP_CreateHeaderField( Field_Proxy_Authorization, (BYTE*)vsProxyCredentials, iProxyCredentialsLength, &pList);
			if( pStartList == NULL )
				pStartList = pList;
		}
/* Only in ConnectionLess Mode */
		else if( iConnectionMode == 0 )
		{
			BYTE* pbAddress = NULL;
			UINT8 iAddressLength = 0;
			BYTE* pbCredentials = NULL;
			UINT8 iCredLength = 0;
			UINT16 iPortNo = 0;

			getProxyAddress( pNetStruct, &pbAddress, &iAddressLength );
			iPortNo = getStackMode( pNetStruct );

			if(checkProxyAuth(pbAddress,iAddressLength,iPortNo,&pbCredentials)){
				if( pbCredentials != NULL ){
					iCredLength = ((UINT16)*pbCredentials)-2;
					pbCredentials+=2;
					if( pbCredentials != NULL && iCredLength > 0 ){
						WSP_CreateHeaderField( Field_Proxy_Authorization, pbCredentials, iCredLength, &pList);
						if( pStartList == NULL )
							pStartList = pList;
					}
				}
			}
		}

/* Only in ConnectionLess Mode */
		if( iConnectionMode == 0 && pWAEMainStruct != NULL && pWAEMainStruct->pbUSERAGENT != NULL ){
			uiLength = B_STRINGLENGTH( pWAEMainStruct->pbUSERAGENT );
			WSP_CreateHeaderField( Field_User_Agent, pWAEMainStruct->pbUSERAGENT, (uiLength+1), &pList);
			if( pStartList == NULL )
				pStartList = pList;
		}

/* Only in ConnectionOriented Mode */
		if( iConnectionMode == 1 && iProfileEnabled == 1 ){
			if( pWAEMainStruct != NULL ){
				if( pWAEMainStruct->pbPROFILE_DIFF != NULL && pWAEMainStruct->iPROFILE_DIFF_LEN > 0 &&
					!(*(pWAEMainStruct->pbPROFILE_DIFF) == 0 && pWAEMainStruct->iPROFILE_DIFF_LEN == 1) ){
					WSP_CreateHeaderField( Field_Profile_Diff, pWAEMainStruct->pbPROFILE_DIFF, pWAEMainStruct->iPROFILE_DIFF_LEN, &pList);
					if( pStartList == NULL )
						pStartList = pList;
				}
			}
		}

		if( pStartList != NULL ){
			uiLength = 0;
			pbHeader = WSP_BuildHeader( pStartList, &uiLength );
		}

	}

	*pvHeader = (VOID*)pbHeader;
	return (int)uiLength;

} /* end of GetHeader */

/*========================================================================
	GetBody 
==========================================================================
Purpose: Get the Body from the URLREQSTRUCT
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvBody			BYTE**			casted to VOID**
Return:  length of pvBody
=========================================================================*/
int GetBody( VOID* pvReqStruct, VOID** pvBody)
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*		pbOctetString = NULL;
	UINT32		uiUrlLength = 0;
	UINT32		uiIndex = 0;

	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		if( pUrlReq->pbBody != NULL ){
			uiUrlLength = pUrlReq->iBodyLength;
			if( uiUrlLength > 0 ){
				pbOctetString = NEWARRAY(BYTE,uiUrlLength);
				if( pbOctetString != NULL ){
					for ( uiIndex=0; uiIndex<uiUrlLength; uiIndex++ )
						pbOctetString[uiIndex] = pUrlReq->pbBody[uiIndex];
				}
				else
					uiUrlLength = 0;
			}
		}
	}

	*pvBody = (VOID*)pbOctetString;
	return (int)uiUrlLength;
} /* end of GetBody */

/*========================================================================
	CreateWSPHeader
==========================================================================
Purpose: Add if required Post and Reload headers to an existing WSP-header. 
Params:  pvHeader			BYTE*			casted to VOID*
		 iHeadLen			int				length of pvHeader
		 iReload			int				1=Reload
		 ucMethod			unsigned char	Get or Post
		 pvReqStruct		URLREQSTRUCT*	casted to VOID*
		 pliHeaderLength	long int*		length of the hole WSP-header
Return:  pointer to the WSP-header
=========================================================================*/
void* CreateWSPHeader( VOID* pvHeader, int iHeadLen,
		int iReload, unsigned char ucMethod,
		VOID* pvReqStruct, long int* pliHeaderLength )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbOctetString = NULL;
	UINT16		uiLength = 0;
	UINT16		uiIndex = 0;
	UINT16		uiStart = 0;
	UINT16		iContentTypeLength = 0;

	*pliHeaderLength = 0;
	if( pvReqStruct != NULL )
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;

	if( pvHeader != NULL ){
		pbHeader = (BYTE*)pvHeader;
		uiLength = (UINT16)iHeadLen;
	}
	
	if( iReload )
		uiLength += 2;
		/* Post */
	if( ucMethod == 0x60 ){
		if( pUrlReq != NULL && (pUrlReq->iContentCharSet != IANA_CHARSET_INVALID) ){
			if( pUrlReq->iContentCharSet < 256 )
				uiLength += 4;
			else
				uiLength += 6;
		}
		else
			if( pUrlReq->iContentType != CONTENTTYPE_INVALID || 
					(pUrlReq->iContentType == CONTENTTYPE_INVALID  && pUrlReq->pbContentType == NULL)){
				uiLength++;
			}
			else{
				iContentTypeLength = B_STRINGLENGTH( pUrlReq->pbContentType ) + 1;
				uiLength += iContentTypeLength;
			}
	}

	if( uiLength > 0 ){

		pbOctetString = NEWARRAY( BYTE, uiLength );

		if( pbOctetString != NULL ){
				/* Post */
			if( ucMethod == 0x60 ){
				if( pUrlReq != NULL && (pUrlReq->iContentCharSet != IANA_CHARSET_INVALID) ){
					if( pUrlReq->iContentCharSet < 256 ){
						pbOctetString[0] = 0x03; /* Content-Type Length */
						pbOctetString[1] = 0x80 + pUrlReq->iContentType;
						pbOctetString[2] = 0x81; /* Parameter-charset */
						pbOctetString[3] = (BYTE)(pUrlReq->iContentCharSet | 0x80); 
						uiStart += 4;
						iHeadLen += 4;
					}
					else{
						pbOctetString[0] = 0x05; /* Content-Type Length */
						pbOctetString[1] = 0x80 + pUrlReq->iContentType;
						pbOctetString[2] = 0x81; /* Parameter-charset */
						pbOctetString[3] = 0x02; /* Length uintvar */
						pbOctetString[4] = (pUrlReq->iContentCharSet >> 8) & 0xff;
						pbOctetString[5] = pUrlReq->iContentCharSet & 0xff;
						uiStart += 6;
						iHeadLen += 6;
					}
				}
				else{		/* This code is at least used by CLNTc_PostContent */
					if( pUrlReq->iContentType != CONTENTTYPE_INVALID || 
							(pUrlReq->iContentType == CONTENTTYPE_INVALID  && pUrlReq->pbContentType == NULL)){
						pbOctetString[0] = 0x80 + pUrlReq->iContentType;
						uiStart++;
						iHeadLen++;
					}
					else{
						B_COPYSTRING (pbOctetString,pUrlReq->pbContentType);
						uiStart += iContentTypeLength;
						iHeadLen += iContentTypeLength;
					}
				}
			}
			if( pbHeader != NULL ){
				for ( uiIndex=uiStart; uiIndex<iHeadLen; uiIndex++ )
					pbOctetString[uiIndex] = pbHeader[uiIndex-uiStart];
			}
			if( iReload ){
				pbOctetString[uiIndex] = 0x80 + Field_Cache_Control;
				uiIndex++;
					/* No-Cache */
				pbOctetString[uiIndex] = 0x80;
			}
		}
	}

	*pliHeaderLength = uiLength;
	return (void*)pbOctetString;
} /* end of CreateWSPHeader */

/*========================================================================
	ExamineCacheInfo 
==========================================================================
Purpose: Examine the content. If it is Multipart this function is invoked 
		 recursive, stored every multipart part in the cache, returns the 
		 first content part to the UserAgent.
Params:  pHeaderHandle			pHEADERDEF*
		 iUaType				int
		 iCacheInIdle			int
		 iCacheDirektive		int
		 pvReqStruct			URLREQSTRUCT*		casted to void*
		 pvHeader				BYTE**				casted to void**
		 pliHeadLen				long int*
		 pvBody					BYTE**				casted to void**
		 pliBodyLen				long int*
		 iFirstWrite			int*
		 iMultipart				int
Return:  1, if the content is a Multipart and there are some errors in the 
		 Multpart, 0 is returned.
=========================================================================*/
int ExamineCacheInfo( pHEADERDEF *pHeaderHandle, int iUaType, int iCacheInIdle,
                      int iCacheDirektive, void* pvReqStruct, void** pvHeader,
                      long int* pliHeadLen, void** pvBody, long int* pliBodyLen,
                      int* iFirstWrite, int iMultipart )
{
	pHEADERDEF		pHeaderList = NULL;
	int				iReturn = 1;

	if( *pHeaderHandle != NULL )
	{
        pHeaderList = *pHeaderHandle;
		if( pHeaderList->pHeadList != NULL )
		{		/* Check if multipart/related */
			if( CheckIfMultipart( pHeaderList->pHeadList ) ){
				pMULTIPARTSTR	pMultiPartList = NULL;
				pMULTIPARTSTR	pMultiPartListStart = NULL;
				pHEADERDEF		pMultiHeaderList = NULL;
				BOOL			boError = FALSE;
				int				iTemp = 0;

				pHEADERDEF		pTempHeaderList = NULL;
				void*			pvTempBody = NULL;
				int				iTempBodyLength = 0;

                pMultiPartList = WSP_PreParseMultipart( (BYTE*)(*pvBody), *pliBodyLen );
				if( pMultiPartList != NULL ){
					pMultiPartListStart = pMultiPartList;
					while( pMultiPartList != NULL ){
						if( pMultiPartList->pbHdrStart != NULL && pMultiPartList->iHdrLen > 0 ){
							boError = FALSE;
							pMultiHeaderList = WSP_PreParseHeaders ( pMultiPartList->pbHdrStart, pMultiPartList->iHdrLen, &boError );
							if( pMultiHeaderList != NULL && boError == FALSE ){
								if( *iFirstWrite == 0 )
									iTemp = 1;
/*								iReturn = ExamineCacheInfo( &((void*)pMultiHeaderList),iUaType,iCacheInIdle,iCacheDirektive,
											pvReqStruct,&(pMultiPartList->pbHdrStart),&(pMultiPartList->iHdrLen),
											&(pMultiPartList->pbBodyStart),&(pMultiPartList->iBodyLen),iFirstWrite,1);
                                */{
                                    long int liHdrLen = (long int)pMultiPartList->iHdrLen;
                                    long int liBodyLen = (long int)pMultiPartList->iBodyLen;
#ifdef FEATURE_WAP
                                    iReturn = ExamineCacheInfo( &pMultiHeaderList, iUaType, iCacheInIdle, iCacheDirektive,
                                                                pvReqStruct, (void **)&(pMultiPartList->pbHdrStart), &liHdrLen,
                                                                (void**)&(pMultiPartList->pbBodyStart), &liBodyLen, iFirstWrite, 1);
#else
                                    iReturn = ExamineCacheInfo( &pMultiHeaderList, iUaType, iCacheInIdle, iCacheDirektive,
                                                                pvReqStruct, &(pMultiPartList->pbHdrStart), &liHdrLen,
                                                                (void**)&(pMultiPartList->pbBodyStart), &liBodyLen, iFirstWrite, 1);
#endif //FEATURE_WAP

                                    pMultiPartList->iHdrLen = (UINT32)liHdrLen;
                                    pMultiPartList->iBodyLen = (UINT32)liBodyLen;
                                }
								if( *iFirstWrite == 0 && iReturn == 0 ){
									DeleteHEADERDEF( pMultiHeaderList );
									pMultiHeaderList = NULL;
									break;
								}
									/* Om första sätt tempvariabler */
								if( *iFirstWrite == 1 && iTemp == 1 ){
									pTempHeaderList = pMultiHeaderList;
									pMultiHeaderList = NULL;
									pvTempBody = pMultiPartList->pbBodyStart;
									iTempBodyLength = pMultiPartList->iBodyLen;
									iTemp = 0;
								}
							}
							else if( *iFirstWrite == 0 ){
								iReturn = 0;
								if( pMultiHeaderList != NULL ){
									DeleteHEADERDEF( pMultiHeaderList );
									pMultiHeaderList = NULL;
								}
								break;
							}
							if( pMultiHeaderList != NULL ){
								DeleteHEADERDEF( pMultiHeaderList );
								pMultiHeaderList = NULL;
							}
						}
						pMultiPartList = pMultiPartList->pNext;
					}
						/* Om tempvariabler satta sätt om utgående. */
					if( pTempHeaderList != NULL && pvTempBody != NULL && iTempBodyLength > 0 ){
						DeleteHEADERDEF( (pHEADERDEF)(*pHeaderHandle) );
                        *pHeaderHandle = pTempHeaderList;
						if( iMultipart == 0 ){
							DEALLOC(pvHeader);
							*pvHeader = NULL;
							*pliHeadLen = 0;
							DEALLOC(pvBody);
							*pvBody = NEWARRAY(BYTE,iTempBodyLength);
							B_COPYSTRINGN(*pvBody,pvTempBody,iTempBodyLength);
						}
						else
							*pvBody = pvTempBody;
						*pliBodyLen = iTempBodyLength;
					}
					WSP_DeleteMultipartList( &pMultiPartListStart );
				}
				else
					iReturn = 0;
			}
			else{
				if( !*iFirstWrite && iMultipart == 0 ){
					*iFirstWrite = 1;
					TryWriteToCache(((pURLREQSTRUCT)pvReqStruct)->pbURL,iUaType,iCacheInIdle,iCacheDirektive,pHeaderList,
										*pvHeader, *pliHeadLen, *pvBody, *pliBodyLen);
				}
				else{
					BYTE*			pbURL = NULL;
					BYTE*			pbContentLocation = NULL;
					pHEADERELEMENT	pHeaderElement = WSP_GetHeaderWK ( 1, Field_Content_Location, pHeaderList->pHeadList );
					
					if( pHeaderElement != NULL ){
						pbContentLocation = WSP_GetContentLocation( pHeaderElement );
						if( pbContentLocation != NULL ){
							b_Resolve( ((pURLREQSTRUCT)pvReqStruct)->pbURL, pbContentLocation, &pbURL );
							TryWriteToCache( pbURL,iUaType,iCacheInIdle,iCacheDirektive,pHeaderList,
												*pvHeader, *pliHeadLen, *pvBody, *pliBodyLen);
							DEALLOC(&pbURL);
							if( !*iFirstWrite )
								*iFirstWrite = 1;
							return iReturn;
						}
					}
					if( !*iFirstWrite ){
						*iFirstWrite = 1;
						TryWriteToCache(((pURLREQSTRUCT)pvReqStruct)->pbURL,iUaType,iCacheInIdle,iCacheDirektive,pHeaderList,
											*pvHeader, *pliHeadLen, *pvBody, *pliBodyLen);
					}
				}
			}
		}
	}

	return iReturn;
} /* end of ExamineCacheInfo */

/*========================================================================
	GetURLfromLocation 
==========================================================================
Purpose: Get the Location value from the pvHeader and store the Location in pvUrl
Params:  pvHeader		BYTE*		casted to void*
		 iHeaderLength	int
		 pvUrl			BYTE**		casted to VOID**
		 liUrlLength	long int*
Return:  1 if success, otherwise 0
=========================================================================*/
int GetURLfromLocation( void* pvHeader, int iHeaderLength, VOID** pvUrl, long int* liUrlLength )
{
	BYTE*			pbHeader = NULL;
	UINT32			uiHeadLen = 0;
	pHEADERDEF		pHeaderList = NULL;
	pHEADERELEMENT	pHeaderElement = NULL;
	BYTE*			pbUrlLocation = NULL;
	BYTE*			pbResolvedUrl = NULL;
	UINT16			uiUrlLength = 0;
	BOOL			boError = FALSE;

	*liUrlLength = 0;
	if( pvHeader != NULL && iHeaderLength > 0 )
	{
		pbHeader = (BYTE*)pvHeader;
		uiHeadLen = iHeaderLength;
		if( pbHeader != NULL )
		{
			pHeaderList = WSP_PreParseHeaders ( pbHeader, uiHeadLen, &boError );
			if( pHeaderList != NULL && boError == FALSE )
			{
				if( pHeaderList->pHeadList != NULL )
				{
					pHeaderElement = WSP_GetHeaderWK ( 1, Field_Location, pHeaderList->pHeadList );
					if( pHeaderElement != NULL )
					{
						pbUrlLocation = WSP_GetLocation( pHeaderElement );
						if( pbUrlLocation != NULL ){
							uiUrlLength = B_STRINGLENGTH( pbUrlLocation );
							if( uiUrlLength > 0 ){
								if( b_Resolve( (BYTE*)(*pvUrl), pbUrlLocation, &pbResolvedUrl ) ){
									if( pbResolvedUrl != NULL ){
										uiUrlLength = B_STRINGLENGTH( pbResolvedUrl );
										DEALLOC( pvUrl );

										*pvUrl = (VOID*)pbResolvedUrl;
										*liUrlLength = (long int)uiUrlLength;
										DeleteHEADERDEF( pHeaderList );
										return 1;
									}
								}
							}
						}
					}
				}
				DeleteHEADERDEF( pHeaderList );
			}
		}
	}
	return 0;

} /* end of GetURLfromLocation */

/*========================================================================
	GetHeaderHandle
==========================================================================
Purpose: Create a HeaderHandle from a Bytestream
Params:  ppHeaderHandle	pHEADERDEF*	casted to void**
		 pvHeader		BYTE*		casted to a VOID*
		 iHeadLen		int
		 iRedirect		int
		 pvUrl			BYTE*		casted to VOID*
Return:  1 if success, otherwise 0
=========================================================================*/
int GetHeaderHandle( void** ppHeaderHandle, VOID* pvHeader, int iHeadLen, 
					int iRedirect, VOID* pvUrl  )
{
	BYTE*			pbHeader = NULL;
	BYTE*			pbContentBase = NULL;
	pHEADERDEF		pHeaderList = NULL;
	pHEADERELEMENT	pHead = NULL;
	UINT32			uiHeadLen = 0;
	BOOL			boError = FALSE;
	int				iUrlLength = 0;
	int				iReturn = 0;

	*ppHeaderHandle = NULL;
	uiHeadLen = (UINT32)iHeadLen;
	if( pvHeader != NULL )
	{
		pbHeader = (BYTE*)pvHeader;
		if( pbHeader != NULL )
		{
			pHeaderList = WSP_PreParseHeaders ( pbHeader, uiHeadLen, &boError );
			if( pHeaderList != NULL && boError == FALSE ){
						/* Simulate ContetBase if the request has been redirected. */
				if( iRedirect != 0 && pvUrl != NULL ){
					iUrlLength = B_STRINGLENGTH( (BYTE*)pvUrl );
					if( iUrlLength > 0 ){
						if( pHeaderList->pHeadList != NULL ){
							if( (WSP_GetHeaderWK( 1, Field_Content_Base, pHeaderList->pHeadList )==NULL) &&
									(WSP_GetHeaderWK( 1, Field_Content_Location, pHeaderList->pHeadList )==NULL) ){
								pbContentBase = NEWARRAY( BYTE, iUrlLength+1 );
								if( pbContentBase != NULL ){
									B_COPYSTRINGN( pbContentBase, (BYTE*)pvUrl, iUrlLength );
									pbContentBase[iUrlLength] = 0;
									pHead = CreateHeaderElement( 1, Field_Content_Base | 0x80, NULL, pbContentBase, 
											(pbContentBase + iUrlLength + 1) );
									if( pHead != NULL ){
										AddHeaderElement( pHeaderList, pHead);
											/* Add ContentBase */
									}
								}
							}
						}
					}
				}
				*ppHeaderHandle = (void*)pHeaderList;
				iReturn = 1;
			}
			else{
				if( pHeaderList != NULL )
					DeleteHEADERDEF( pHeaderList );
			}
		}
	}

	return iReturn;

} /* end of GetHeaderHandle */

/*========================================================================
	getAddrBearer 
==========================================================================
Purpose: Get the Bearer part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  Bearer
=========================================================================*/
unsigned char getAddrBearer( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		return (unsigned char)pNetworkStruct->iActiveBearer;
	}
	return 0;
} /* end of getAddrBearer */

/*========================================================================
	getAddrSecAddrFormat 
==========================================================================
Purpose: Get the SecondAddressFormat part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*
		 iServerSet		int				1 (Serverpart), 0 (Clientpart)
Return:  SecondAddressFormat
=========================================================================*/
unsigned char getAddrSecAddrFormat( VOID* pNetStruct, int iServerSet )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet && (pNetworkStruct->iActiveBearer==BEARER_GSM_USSD) )
			return (unsigned char)pNetworkStruct->iUSSD_SC_TYPE;
	}
	return 0;
} /* end of getAddrSecAddrFormat */

/*========================================================================
	getAddrAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in DeviceAddress
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 pAddress		DeviceAddress*
Return:  
=========================================================================*/
void getAddrAddress( VOID* pNetStruct, int iServerSet, DeviceAddress* pAddress )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL && pAddress != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet ){
			switch( pNetworkStruct->iActiveBearer )
			{
				case BEARER_ANY_UDP:
				case BEARER_GSM_CSD:
				case BEARER_GSM_GPRS:
				case BEARER_BT:
					if( pNetworkStruct->pbUDP_GW_IP != NULL ){
						devaddr_store( pAddress, pNetworkStruct->pbUDP_GW_IP, 
							pNetworkStruct->pbUDP_GW_IP_LEN );
					}
					break;
				case BEARER_GSM_USSD:
					if( pNetworkStruct->iUSSD_SC_TYPE != 0xFF ){
						if( pNetworkStruct->pbUSSD_SC != NULL ){
							devaddr_store( pAddress, pNetworkStruct->pbUSSD_SC, 
								pNetworkStruct->pbUSSD_SC_LEN );
						}
						else
							pAddress->length = 0;
					}
					else{
						if( pNetworkStruct->pbUSSD_CENTER != NULL ){
							devaddr_store( pAddress, pNetworkStruct->pbUSSD_CENTER, 
								pNetworkStruct->pbUSSD_CENTER_LEN );
						}
						else
							pAddress->length = 0;
					}
					break;
				case BEARER_GSM_SMS:
					if( pNetworkStruct->pbSMS_GW != NULL ){
						devaddr_store( pAddress, pNetworkStruct->pbSMS_GW, 
							pNetworkStruct->pbSMS_GW_LEN );
					}
					break;
				default :
					pAddress->length = 0;
					break;
			}
		}
		else{
			switch( pNetworkStruct->iActiveBearer )
			{
				case BEARER_ANY_UDP:     /* UDP set client IP */
				case BEARER_GSM_CSD:
				case BEARER_GSM_GPRS:
				case BEARER_BT:
					if( pNetworkStruct->pbUDP_CLIENT_IP != NULL ){
						devaddr_store( pAddress, pNetworkStruct->pbUDP_CLIENT_IP, 
							pNetworkStruct->pbUDP_CLIENT_IP_LEN );
					}
					break;
				default :     /* USSD, SMS = no client addr, assign a dummy addr */
					pAddress->data[0] = 0;
					pAddress->length = 1;
					break;
			}
		}
	}

} /* end of getAddrAddress */

/*========================================================================
	getProxyAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in pAddress
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 pAddress		BYTE**			
		 iAddressLength	UINT8*
Return:  
=========================================================================*/
void getProxyAddress( VOID* pNetStruct, BYTE** pAddress, UINT8* iAddressLength )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;
	int	iBearer = 0;

	*pAddress = NULL;
	*iAddressLength = 0;
	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		iBearer = (int)getAddrBearer( pNetStruct );

		switch( iBearer )
		{
			case BEARER_ANY_UDP:
			case BEARER_GSM_CSD:
			case BEARER_GSM_GPRS:
			case BEARER_BT:
				if( pNetworkStruct->pbUDP_GW_IP != NULL ){
					*pAddress = pNetworkStruct->pbUDP_GW_IP;
					*iAddressLength = pNetworkStruct->pbUDP_GW_IP_LEN;
				}
				break;
			case BEARER_GSM_USSD:
				if( pNetworkStruct->iUSSD_SC_TYPE != 0xFF ){
					if( pNetworkStruct->pbUSSD_SC != NULL ){
						*pAddress = pNetworkStruct->pbUSSD_SC;
						*iAddressLength = pNetworkStruct->pbUSSD_SC_LEN;
					}
				}
				else{
					if( pNetworkStruct->pbUSSD_CENTER != NULL ){
						*pAddress = pNetworkStruct->pbUSSD_CENTER;
						*iAddressLength = pNetworkStruct->pbUSSD_CENTER_LEN;
					}
				}
				break;
			case BEARER_GSM_SMS:
				if( pNetworkStruct->pbSMS_GW != NULL ){
					*pAddress = pNetworkStruct->pbSMS_GW;
					*iAddressLength = pNetworkStruct->pbSMS_GW_LEN;
				}
				break;
		}
	}
}


/*========================================================================
	getAddrSecondAddress 
==========================================================================
Purpose: Take the Address from the NETWORKSTRUCT and stored it in AuxiliaryAddress
Params:  pNetStruct			NETWORKSTRUCT*		casted to VOID*,
		 iServerSet			Int					1 (Serverpart), 0 (Clientpart)
		 pSecondAddress		AuxiliaryAddress*	
Return: 
=========================================================================*/
void getAddrSecondAddress( VOID* pNetStruct, int iServerSet, AuxiliaryAddress* pSecondAddress )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL && pSecondAddress != NULL  ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet ){
			switch( pNetworkStruct->iActiveBearer )
			{
				case BEARER_GSM_USSD:
					if( pNetworkStruct->iUSSD_SC_TYPE != 0xFF ){
						if( pNetworkStruct->pbUSSD_CENTER != NULL ){
							auxaddr_store( pSecondAddress, pNetworkStruct->pbUSSD_CENTER, 
								pNetworkStruct->pbUSSD_CENTER_LEN );
						}
						else
							pSecondAddress->length = 0;
					}
					else
						pSecondAddress->length = 0;
					break;
				case BEARER_GSM_SMS:     
					if( pNetworkStruct->pbSMS_CENTER != NULL ){
						auxaddr_store( pSecondAddress, pNetworkStruct->pbSMS_CENTER, 
							pNetworkStruct->pbSMS_CENTER_LEN );
					}
					break;
				default :
					pSecondAddress->length = 0;
					break;
			}
		}
		else
			pSecondAddress->length = 0;
	}

} /* end of getAddrSecondAddress */

/*========================================================================
	getTimerValue 
==========================================================================
Purpose: Get the TimeOut part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  TimeOut value in 1/10 of a second.
=========================================================================*/
int getTimerValue( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;

		return ((pNetworkStruct->iTimeOut)*10);
	}
	return 864000;
} /* end of getTimerValue */

/*========================================================================
	getStackMode 
==========================================================================
Purpose: Get the StackMode part from the NETWORKSTRUCT
Params:  pNetStruct	NETWORKSTRUCT*	casted to VOID*
Return:  StackMode
=========================================================================*/
int getStackMode( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;

		return pNetworkStruct->iStackMode;
	}
	return 0;
} /* end of getStackMode */

int getClientPort( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;

		return pNetworkStruct->iClientPort;
	}
	return -1;
} /* end of getClientPort */

/*========================================================================
	GetFile 
==========================================================================
Purpose: Invokes the adapter function CLNTa_getFile with a URL from 
		 pvRequestStruct and also dealloc the pvRequestStruct
Params:  iTID				Int
		 pvRequestStruct	pURLREQSTRUCT*	casted to VOID**
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int GetFile( int iTID, VOID** pvRequestStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*	pbFrag = NULL;

	if( *pvRequestStruct != NULL ){
		pUrlReq = (pURLREQSTRUCT)(*pvRequestStruct);
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL ){
#ifdef FEATURE_WAP
			pbFrag = (BYTE *)B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
#else
            pbFrag = B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
#endif //FEATURE_WAP
			if( pbFrag != NULL )
				*pbFrag = 0;
			CLNTa_getFile( (UINT8)iTID, (CHAR*)(pUrlReq->pbURL) );

			Delete_URLREQSTRUCT( &pUrlReq );
			return 1;
		}

		Delete_URLREQSTRUCT( &pUrlReq );
	}
	return 0;
} /* end of GetFile */

/*========================================================================
	checkIfImageRequest 
==========================================================================
Purpose: Check in the pvReqStruct if the request expect an image in the response.
Params:  pvReqStruct	URLREQSTRUCT*	casted to void*
Return:  1 if an image, otherwise 0
=========================================================================*/
int checkIfImageRequest( void* pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;

	if( pvReqStruct != NULL ){
		pUrlReq = (pURLREQSTRUCT)(pvReqStruct);
		if( pUrlReq != NULL && pUrlReq->pbAccept != NULL ){
			if( B_STRSTRING( pUrlReq->pbAccept , (BYTE*)"image" ) != NULL )
				return 1;
		}
	}
	return 0;
} /* end of checkIfImageRequest */



void*  getMyPtrFromMem( DB_ref record, const BYTE* key, BOOL del, UINT8* errorCode, UINT16* uiLength )
{
	BYTE*	bDummy;

	*uiLength = 0;
	bDummy = (BYTE*)getPtr( record, key, del, errorCode );
	if( bDummy != NULL ){

		*uiLength = *((UINT16*)bDummy)-2;
		bDummy+=2;
		return (void*)bDummy;
	}
	return NULL;
}



int setNetworkParameters( VOID* pvWAEMaStruct, VOID* pvNetwork, VOID* pvUaSettings, int iObjectID, int iChannelRef, int iConnectionOriented )
{
	pWAEMAINOBJECT	pWAEMainStruct = NULL;
	NETWORKSTRUCT*	pNetwork = NULL;
	UASETTINGS*		pUaSet = NULL;
	DB_ref			dbChannelRef = 0;
	DB_ref			myDB_Root = 0;
	UINT16			iDummy = 0;
	BYTE*			bDummy = NULL;
	void*			vDummy = NULL;
	UINT8			uiErrorCode = 0;
	int				iReturn = 0;

	if( pvNetwork != NULL ){
		pNetwork = (NETWORKSTRUCT*)pvNetwork;
		dbChannelRef = (DB_ref)iChannelRef;
#ifdef FEATURE_WAP
		myDB_Root = getRef(DB_root, (const BYTE* )"globl", &uiErrorCode);
#else
        myDB_Root = getRef(DB_root, "globl", &uiErrorCode);
#endif //FEATURE_WAP

#ifdef FEATURE_WAP
		iDummy = (int)getInt( dbChannelRef, (const BYTE*)"stckm", FALSE, &uiErrorCode );
#else
        iDummy = (int)getInt( dbChannelRef, "stckm", FALSE, &uiErrorCode );
#endif //FEATURE_WAP
		if( !uiErrorCode && pNetwork->iStackMode != iDummy ){
			pNetwork->iStackMode = iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		iDummy = (int)getInt( dbChannelRef, (const BYTE*)"accTp", FALSE, &uiErrorCode );
#else
        iDummy = (int)getInt( dbChannelRef, "accTp", FALSE, &uiErrorCode );
#endif //FEATURE_WAP
		if( !uiErrorCode && pNetwork->iActiveBearer != iDummy ){
			pNetwork->iActiveBearer = iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		iDummy = (int)getInt( dbChannelRef, (const BYTE*)"cltLP", FALSE, &uiErrorCode );
#else
        iDummy = (int)getInt( dbChannelRef, "cltLP", FALSE, &uiErrorCode );
#endif //FEATURE_WAP

		if( !uiErrorCode && pNetwork->iClientPort != iDummy ){
			pNetwork->iClientPort = iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"udpIS", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "udpIS", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP
		if( vDummy != NULL && 
				(pNetwork->pbUDP_CLIENT_IP==NULL || 
				(iDummy != pNetwork->pbUDP_CLIENT_IP_LEN) ||
#ifdef FEATURE_WAP
				(strncmp( vDummy, (char *)pNetwork->pbUDP_CLIENT_IP, iDummy) !=0) ) ){
#else
                (strncmp( vDummy, pNetwork->pbUDP_CLIENT_IP, iDummy) !=0) ) ){
#endif //FEATURE_WAP
			DEALLOC( &(pNetwork->pbUDP_CLIENT_IP) );
			pNetwork->pbUDP_CLIENT_IP = NEWARRAY(BYTE, iDummy );
			B_COPYSTRINGN( pNetwork->pbUDP_CLIENT_IP, vDummy, iDummy );
			pNetwork->pbUDP_CLIENT_IP_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"udpIG", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "udpIG", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP
		if( vDummy != NULL && 
				(pNetwork->pbUDP_GW_IP==NULL || 
				(iDummy!=pNetwork->pbUDP_GW_IP_LEN) ||
#ifdef FEATURE_WAP
				(strncmp(vDummy,(char *)pNetwork->pbUDP_GW_IP,iDummy)!=0) ) ){
#else
                (strncmp(vDummy,pNetwork->pbUDP_GW_IP,iDummy)!=0) ) ){
#endif //FEATURE_WAP

			DEALLOC( &(pNetwork->pbUDP_GW_IP) );
			pNetwork->pbUDP_GW_IP = NEWARRAY(BYTE,iDummy);
			B_COPYSTRINGN( pNetwork->pbUDP_GW_IP, vDummy, iDummy );
			pNetwork->pbUDP_GW_IP_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"smsC", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "smsC", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP
		if( vDummy != NULL && 
				(pNetwork->pbSMS_CENTER==NULL || 
				(iDummy!=pNetwork->pbSMS_CENTER_LEN) ||
#ifdef FEATURE_WAP
				(strncmp(vDummy,(char *)pNetwork->pbSMS_CENTER,iDummy)!=0) ) ){
#else
                (strncmp(vDummy,pNetwork->pbSMS_CENTER,iDummy)!=0) ) ){
#endif //#ifdef FEATURE_WAP

			DEALLOC( &(pNetwork->pbSMS_CENTER) );
			pNetwork->pbSMS_CENTER = NEWARRAY(BYTE,iDummy);
			B_COPYSTRINGN( pNetwork->pbSMS_CENTER, vDummy, iDummy );
			pNetwork->pbSMS_CENTER_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"smsGW", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "smsGW", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

		if( vDummy != NULL && 
				(pNetwork->pbSMS_GW==NULL || 
				(iDummy!=pNetwork->pbSMS_GW_LEN) ||
#ifdef FEATURE_WAP
				(strncmp(vDummy,(char *)pNetwork->pbSMS_GW,iDummy)!=0) ) ){
#else
                (strncmp(vDummy,pNetwork->pbSMS_GW,iDummy)!=0) ) ){
#endif //FEATURE_WAP

			DEALLOC( &(pNetwork->pbSMS_GW) );
			pNetwork->pbSMS_GW = NEWARRAY(BYTE,iDummy);
			B_COPYSTRINGN( pNetwork->pbSMS_GW, vDummy, iDummy );
			pNetwork->pbSMS_GW_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"usdC", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "usdC", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

		if( vDummy != NULL && 
				(pNetwork->pbUSSD_CENTER==NULL || 
				(iDummy!=pNetwork->pbUSSD_CENTER_LEN) ||
#ifdef FEATURE_WAP
				(strncmp(vDummy,(char *)pNetwork->pbUSSD_CENTER,iDummy)!=0) ) ){
#else
                (strncmp(vDummy,pNetwork->pbUSSD_CENTER,iDummy)!=0) ) ){
#endif //FEATURE_WAP
			DEALLOC( &(pNetwork->pbUSSD_CENTER) );
			pNetwork->pbUSSD_CENTER = NEWARRAY(BYTE,iDummy);
			B_COPYSTRINGN( pNetwork->pbUSSD_CENTER, vDummy, iDummy );
			pNetwork->pbUSSD_CENTER_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		iDummy = (int)getInt( dbChannelRef, (const BYTE*)"usdGT", FALSE, &uiErrorCode );
#else
        iDummy = (int)getInt( dbChannelRef, "usdGT", FALSE, &uiErrorCode );
#endif //FEATURE_WAP
		if( !uiErrorCode && pNetwork->iUSSD_SC_TYPE != iDummy ){
			pNetwork->iUSSD_SC_TYPE = iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"usdGW", FALSE, &uiErrorCode, &iDummy );
#else
        vDummy = getMyPtrFromMem( dbChannelRef, "usdGW", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

		if( vDummy != NULL && 
				(pNetwork->pbUSSD_SC==NULL || 
				(iDummy!=pNetwork->pbUSSD_SC_LEN) ||
#ifdef FEATURE_WAP
				(strncmp(vDummy,(char *)pNetwork->pbUSSD_SC,iDummy)!=0) ) ){
#else
                (strncmp(vDummy,pNetwork->pbUSSD_SC,iDummy)!=0) ) ){
#endif //FEATURE_WAP

			DEALLOC( &(pNetwork->pbUSSD_SC) );
			pNetwork->pbUSSD_SC = NEWARRAY(BYTE,iDummy);
			B_COPYSTRINGN( pNetwork->pbUSSD_SC, vDummy, iDummy );
			pNetwork->pbUSSD_SC_LEN = (int)iDummy;
			iReturn = 1;
		}
#ifdef FEATURE_WAP
		iDummy = (int)getInt( dbChannelRef, (const BYTE*)"tmout", FALSE, &uiErrorCode );
#else
        iDummy = (int)getInt( dbChannelRef, "tmout", FALSE, &uiErrorCode );
#endif //FEATURE_WAP

		if( !uiErrorCode )
			pNetwork->iTimeOut = iDummy;


		if( pvUaSettings != NULL ){
			pUaSet = (UASETTINGS*)pvUaSettings;

			if( pUaSet->pbAccept == NULL ){
				if( iObjectID == CONTENT_USER_AGENT ){
					pUaSet->pbAccept = NEWARRAY(BYTE,B_STRINGLENGTH(ACCEPT_ALL) + 1);
					B_COPYSTRING( pUaSet->pbAccept, ACCEPT_ALL );
				}
				else{
					pUaSet->pbAccept = NEWARRAY(BYTE,B_STRINGLENGTH(ACCEPT_DEFAULT) + 1);
					B_COPYSTRING( pUaSet->pbAccept, ACCEPT_DEFAULT );
				}
				if( iConnectionOriented )
					iReturn = 1;
			}
#ifdef FEATURE_WAP
			vDummy = getMyPtrFromMem( dbChannelRef, (const BYTE*)"cred", FALSE, &uiErrorCode, &iDummy );
#else
            vDummy = getMyPtrFromMem( dbChannelRef, "cred", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

			if( vDummy != NULL && 
					(pUaSet->pbProxyCredentials==NULL || 
					(iDummy!=pUaSet->iProxyCredentialsLength) ||
#ifdef FEATURE_WAP
					(strncmp(vDummy,(char *)pUaSet->pbProxyCredentials,iDummy)!=0) ) ){
#else
                    (strncmp(vDummy,pUaSet->pbProxyCredentials,iDummy)!=0) ) ){
#endif //FEATURE_WAP

				DEALLOC( &(pUaSet->pbProxyCredentials) );
				pUaSet->pbProxyCredentials = NEWARRAY(BYTE,iDummy);
				B_COPYSTRINGN( pUaSet->pbProxyCredentials, vDummy, iDummy );
				pUaSet->iProxyCredentialsLength = (int)iDummy;
				if( iConnectionOriented )
					iReturn = 1;
			}
#ifdef FEATURE_WAP
			vDummy = getMyPtrFromMem( myDB_Root, (const BYTE*)"wsp l", FALSE, &uiErrorCode, &iDummy );
#else
            vDummy = getMyPtrFromMem( myDB_Root, "wsp l", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

			if( vDummy != NULL && 
					(pUaSet->pbLanguage==NULL || 
					(iDummy!=pUaSet->iLanguage_Length) ||
#ifdef FEATURE_WAP
					(strncmp(vDummy,(char *)pUaSet->pbLanguage,iDummy)!=0) ) ){
#else
                    (strncmp(vDummy,pUaSet->pbLanguage,iDummy)!=0) ) ){
#endif //FEATURE_WAP

				DEALLOC( &(pUaSet->pbLanguage) );
				pUaSet->pbLanguage = NEWARRAY(BYTE,iDummy);
				B_COPYSTRINGN( pUaSet->pbLanguage, vDummy, iDummy );
				pUaSet->iLanguage_Length = (int)iDummy;
				if( iConnectionOriented )
					iReturn = 1;
			}
		}
		if( pvWAEMaStruct != NULL ){
			pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;
#ifdef FEATURE_WAP
			bDummy = (BYTE*)getPtr( myDB_Root, (const BYTE*)"ua", FALSE, &uiErrorCode );
#else
            bDummy = (BYTE*)getPtr( myDB_Root, "ua", FALSE, &uiErrorCode );
#endif //FEATURE_WAP

			if( bDummy != NULL ){
				if( pWAEMainStruct->pbUSERAGENT == NULL || B_COMPARESTRING(bDummy,pWAEMainStruct->pbUSERAGENT) != 0 ){
					DEALLOC( &(pWAEMainStruct->pbUSERAGENT) );
					pWAEMainStruct->pbUSERAGENT = NEWARRAY(BYTE,B_STRINGLENGTH(bDummy) + 1);
					B_COPYSTRING( pWAEMainStruct->pbUSERAGENT, bDummy );
					if( iConnectionOriented )
						iReturn = 1;
				}
			}
			else
				DEALLOC( &(pWAEMainStruct->pbUSERAGENT) );
#ifdef FEATURE_WAP
			bDummy = (BYTE*)getPtr( myDB_Root, (const BYTE*)"profl", FALSE, &uiErrorCode );
#else
            bDummy = (BYTE*)getPtr( myDB_Root, "profl", FALSE, &uiErrorCode );
#endif //FEATURE_WAP

			if( bDummy != NULL ){
				if( pWAEMainStruct->pbPROFILE == NULL || B_COMPARESTRING(bDummy,pWAEMainStruct->pbPROFILE) != 0 ){
					DEALLOC( &(pWAEMainStruct->pbPROFILE) );
					pWAEMainStruct->pbPROFILE = NEWARRAY(BYTE,B_STRINGLENGTH(bDummy) + 1);
					B_COPYSTRING( pWAEMainStruct->pbPROFILE, bDummy );
					if( iConnectionOriented )
						iReturn = 1;
				}
			}
			else
				DEALLOC( &(pWAEMainStruct->pbPROFILE) );
#ifdef FEATURE_WAP
			vDummy = getMyPtrFromMem( myDB_Root, (const BYTE*)"pDiff", FALSE, &uiErrorCode, &iDummy );
#else
            vDummy = getMyPtrFromMem( myDB_Root, "pDiff", FALSE, &uiErrorCode, &iDummy );
#endif //FEATURE_WAP

			if( vDummy != NULL && 
					(pWAEMainStruct->pbPROFILE_DIFF==NULL || 
					(iDummy!=pWAEMainStruct->iPROFILE_DIFF_LEN) ||
#ifdef FEATURE_WAP
					(strncmp(vDummy,(char *)pWAEMainStruct->pbPROFILE_DIFF,iDummy)!=0) ) ){
#else
                    (strncmp(vDummy,pWAEMainStruct->pbPROFILE_DIFF,iDummy)!=0) ) ){
#endif //FEATURE_WAP

				DEALLOC( &(pWAEMainStruct->pbPROFILE_DIFF) );
				pWAEMainStruct->pbPROFILE_DIFF = NEWARRAY(BYTE,iDummy);
				B_COPYSTRINGN( pWAEMainStruct->pbPROFILE_DIFF, vDummy, iDummy );
				pWAEMainStruct->iPROFILE_DIFF_LEN = (int)iDummy;
				if( iConnectionOriented )
					iReturn = 1;
			}
#ifdef FEATURE_WAP
			iDummy = (int)getInt( dbChannelRef, (const BYTE*)"tmout", FALSE, &uiErrorCode );
#else
            iDummy = (int)getInt( dbChannelRef, "tmout", FALSE, &uiErrorCode );
#endif //FEATURE_WAP			

			if( !uiErrorCode && pWAEMainStruct->iDefaultExpireTime != iDummy ){
				pWAEMainStruct->iDefaultExpireTime = iDummy;
			}
		}
	}
	return iReturn;
} /* end of setNetworkParameters */

/*========================================================================
	GetRealm
==========================================================================
Purpose: Get the Realm value from the pvHeader.
Params:  pvHeader		BYTE*		casted to void*
		 iHeaderLength	int
Return:  Pointer to a Realm
=========================================================================*/
void* GetRealm( void* pvHeader, int iHeadLen )
{
	BYTE*			pbHeader = NULL;
	BYTE*			pbRealm = NULL;
	BYTE*			pbResult = NULL;
	pHEADERDEF		pHeaderList = NULL;
	pHEADERELEMENT	pHead = NULL;
	UINT32			uiHeadLen = 0;
	BOOL			boError = FALSE;


	uiHeadLen = (UINT32)iHeadLen;
	if( pvHeader != NULL )
	{
		pbHeader = (BYTE*)pvHeader;
		if( pbHeader != NULL )
		{
			pHeaderList = WSP_PreParseHeaders ( pbHeader, uiHeadLen, &boError );
			if( pHeaderList != NULL && boError == FALSE ){
				pHead = WSP_GetHeaderWK ( 1, Field_WWW_Authenticate, pHeaderList->pHeadList );
				if( pHead != NULL ){
					pbRealm= WSP_GetWWWAuthenticateRealm (pHead);
					if( pbRealm != NULL ){
						pbResult = NEWARRAY(BYTE,B_STRINGLENGTH(pbRealm)+1);
						if( pbResult != NULL )
							B_COPYSTRING(pbResult,pbRealm);
					}
				}
			}
		}
	}

	if( pHeaderList != NULL )
		DeleteHEADERDEF( pHeaderList );

	if( pbResult == NULL ){
		pbResult = NEWARRAY( BYTE, 1 );
		pbResult[0] = 0;
	}

	return (void*)pbResult;

} /* end of GetRealm */

/*========================================================================
	getNrOfFreeReqstruct 
==========================================================================
Purpose: Get the iNrOfFreeRequestProc parameter from WAEMAINOBJECT.
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  iNrOfFreeRequestProc
=========================================================================*/
int getNrOfFreeReqstruct( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		return pWAEMainStruct->iNrOfFreeRequestProc;
	}
	return 0;
} /* end of getNrOfFreeReqstruct */

/*========================================================================
	InitNrOfFreeTrProc
==========================================================================
Purpose: Set the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMainObject	pWAEMAINOBJECT	casted to a VOID*
		 iMaxTrProc			Int				MaxNumberOfRequests
Return:  
=========================================================================*/
void InitNrOfFreeTrProc( VOID* pvWAEMainObject, int iMaxTrProc )
{
	pWAEMAINOBJECT	pWaeObject = NULL;

	if( pvWAEMainObject != NULL )
	{
		pWaeObject = (pWAEMAINOBJECT)pvWAEMainObject;
		if( pWaeObject != NULL )
			pWaeObject->iNrOfFreeRequestProc = iMaxTrProc;
	}

}  /* end of InitNrOfFreeTrProc */

/*========================================================================
	decNrOfFreeReqstruct 
==========================================================================
Purpose: Decrease the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void decNrOfFreeReqstruct( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeRequestProc)--;
	}

} /* end of decNrOfFreeReqstruct */

/*========================================================================
	incNrOfFreeReqstruct 
==========================================================================
Purpose: Increase the iNrOfFreeRequestProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void incNrOfFreeReqstruct( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeRequestProc)++;
	}

} /* end of incNrOfFreeReqstruct */

/*========================================================================
	getNrOfFreeSession
==========================================================================
Purpose: Get the iNrOfFreeSessionProc parameter from WAEMAINOBJECT.
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  iNrOfFreeSessionProc
=========================================================================*/
int getNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		return pWAEMainStruct->iNrOfFreeSessionProc;
	}
	return 0;
} /* end of getNrOfFreeSession */

/*========================================================================
	InitNrOfFreeSession
==========================================================================
Purpose: Set the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMainObject	pWAEMAINOBJECT	casted to a VOID*
		 iMaxSessionProc	Int				MaxNumberOfSessions
Return:  
=========================================================================*/
void InitNrOfFreeSession( VOID* pvWAEMainObject, int iMaxSessionProc )
{
	pWAEMAINOBJECT	pWaeObject = NULL;

	if( pvWAEMainObject != NULL )
	{
		pWaeObject = (pWAEMAINOBJECT)pvWAEMainObject;
		if( pWaeObject != NULL )
			pWaeObject->iNrOfFreeSessionProc = iMaxSessionProc;
	}

}  /* end of InitNrOfFreeSession */

/*========================================================================
	decNrOfFreeSession
==========================================================================
Purpose: Decrease the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void decNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeSessionProc)--;
	}

} /* end of decNrOfFreeSession */

/*========================================================================
	incNrOfFreeSession
==========================================================================
Purpose: Increase the iNrOfFreeSessionProc parameter in WAEMainObject
Params:  pvWAEMaStruct	pWAEMAINOBJECT	casted to a VOID*
Return:  
=========================================================================*/
void incNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeSessionProc)++;
	}

} /* end of incNrOfFreeSession */



/*========================================================================
	Functions to handeling PID for SDL-Processes
=========================================================================*/

/*========================================================================
	pidList_AddPid
=========================================================================*/
int pidList_AddPid( void* pList, int iId, unsigned long int uiPid )
{
	PIDSTRUCT* pPidStruct = NULL;

	if( pList != NULL ){
		pPidStruct = NEWSTRUCT(PIDSTRUCT);

		if( pPidStruct != NULL ){
			pPidStruct->uiPid = uiPid;
			return SDL_AddListElement( pList, iId, 0, (void*)pPidStruct );
		}
	}
	return 0;
	
} /* end of pidList_AddPid */

/*========================================================================
	pidList_AddPid_Resend
=========================================================================*/
/*int pidList_AddPid_Resend( void* pList, int iId, unsigned long int uiPid, int iResend )
{
	PIDSTRUCT*	pPidStruct = NULL;
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	int			iFound = 0;

	if( pList != NULL ){
		if( iResend ){
			pvListElement = SDL_GetNextListElement( pList, NULL );
			while( pvListElement != NULL ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
						if( pPidStruct->uiPid == uiPid ){
							iFound = 1;*/
								/* It is a real ReSend */
/*							break;
						}
					}
				}
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
			}
		}

		if( !iResend || (iResend && iFound) )
			return pidList_AddPid( pList, iId, uiPid );
	}
	return 0;
	
}*/ /* end of pidList_AddPid_Resend */

/*========================================================================
	pidList_SetSessionID 
=========================================================================*/
/*int pidList_SetSessionID( void* pList, int iId )
{
	void*		pvListElement = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == 0 ){
				SDL_ChangeListElementID( pvListElement, iId );
				return 1;
			}
			else{
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
			}
		}
	}
	return 0;
	
}*/ /* end of pidList_SetSessionID */

/*========================================================================
	pidList_ClearSessionID 
=========================================================================*/
/*int pidList_ClearSessionID( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	int			iID = 0;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
					iID = SDL_GetListElementID( pvListElement );
					SDL_ChangeListElementID( pvListElement, 0 );
					return iID;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
}*/ /* end of pidList_ClearSessionID */

/*========================================================================
	pidList_GetPid 
=========================================================================*/
unsigned long int pidList_GetPid( void* pList, int iId, int iRemove )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int	uiPID = 0;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_SearchListElement( pList, iId, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL ){
					if( iRemove ){
						uiPID = pPidStruct->uiPid;
						DEALLOC( &pPidStruct );
						SDL_DeleteListElement( pList, pvListElement );
						return uiPID;
					}
					else
						return pPidStruct->uiPid;
				}
			}
		}
	}
	return 0;
	
} /* end of pidList_GetPid */


/*========================================================================
	pidList_GetPid_Resend 
=========================================================================*/
/*unsigned long int pidList_GetPid_Resend( void* pList, int iId )
{
	void*		pvListElement = NULL;
	void*		pvPrevListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int uiPid = 0;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		uiPid = pidList_GetPid( pList, iId, 1 );

		if( uiPid != 0 ){
			pvListElement = SDL_GetNextListElement( pList, NULL );
			while( pvListElement != NULL ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
						if( pPidStruct->uiPid == uiPid ){
							DEALLOC( &pPidStruct );
*/								/*Delete Extra PidStruct*/
/*							pvPrevListElement = pvListElement;
						}
					}
				}
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
				if( pvPrevListElement != NULL ){
					SDL_DeleteListElement( pList, pvPrevListElement );
					pvPrevListElement = NULL;
*/						/*Delete Extra ListElement*/
/*				}
			}
		}
	}
	return uiPid;
	
}*/ /* end of pidList_GetPid_Resend */


/*========================================================================
	pidList_RemovePid 
=========================================================================*/
/*int pidList_RemovePid( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
					DEALLOC( &pPidStruct );
					SDL_DeleteListElement( pList, pvListElement );
					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
}*/ /* end of pidList_RemovePid */

/*========================================================================
	pidList_GetFirstPid 
=========================================================================*/
/*unsigned long int pidList_GetFirstPid( void* pList )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) != 0 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
						return pPidStruct->uiPid;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
}*/ /* end of pidList_GetFirstPid */

/*========================================================================
	pidList_GetPidFromElement 
=========================================================================*/
/*unsigned long int pidList_GetPidFromElement( void* pList, void** ppListElement )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = *ppListElement;
		pvListElement = SDL_GetNextListElement( pList, pvListElement );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) != 0 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
						*ppListElement = pvListElement;
						return pPidStruct->uiPid;
					}
				}
			}
			else
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
}*/ /* end of pidList_GetPidFromElement */

/*========================================================================
	pidList_Delete 
=========================================================================*/
int pidList_Delete( void** pList )
{
	PIDSTRUCT*			pPidStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( *pList != NULL ){
		pvListElement = SDL_GetNextListElement( *pList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContenet;
				if( pPidStruct != NULL ){
					DEALLOC( &pPidStruct );
				}
			}
			SDL_DeleteListElement( *pList, pvListElement );
			pvListElement = SDL_GetNextListElement( *pList, NULL );
		}

		SDL_DeleteList( pList );
	}

	return 1;
} /* end of pidList_Delete */

/*========================================================================
	releaseExtraTids 
=========================================================================*/
/*void releaseExtraTids( void* pvWAEMaStruct, void* pList )
{
	void*		pvListElement = NULL;
	int			iTID = 0;

	if( pList != NULL && pvWAEMaStruct != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			iTID = SDL_GetListElementID( pvListElement );

			SDL_ReleaseGlobalTID( pvWAEMaStruct, iTID );		
			
			SDL_DeleteListElement( pList, pvListElement );
			pvListElement = SDL_GetNextListElement( pList, NULL );
		}
		SDL_DeleteList( &pList );
	}

}*/ /* end of releaseExtraTids */

/*========================================================================
	TR_List_Add 
==========================================================================
Purpose: Insert (priority, highest first) a new element in the Transaction-queue
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
		 iTID			Int
		 pvReqStruct	void*					pointer to a URLREQSTRUCT
		 iOptions		Int
		 iPrio			Int						Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_Add( void* pvTransList, int iObjectID, int iSID, int iTID, void* pvReqStruct, 
			int iOptions, int iPrio, unsigned long int uiPid, char chWaitingForChannel )
{
	TRANSACTIONSTRUCT* pTransStruct = NULL;

	if( pvTransList != NULL ){
		pTransStruct = NEWSTRUCT(TRANSACTIONSTRUCT);

		if( pTransStruct != NULL ){
			pTransStruct->iObjectID = iObjectID;
			pTransStruct->iSID = iSID;
			pTransStruct->pvReqStruct = pvReqStruct;
			pTransStruct->iOptions = iOptions;
			pTransStruct->iPrio = iPrio;
			pTransStruct->uiPid = uiPid;
			pTransStruct->chWaitingForChannel = chWaitingForChannel;

			return SDL_AddListElement( pvTransList, iTID, iPrio, (void*)pTransStruct );

		}
	}
	return 0;
	
} /* end of TR_List_Add */

/*========================================================================
	TR_List_CheckForWaitingForConn 
==========================================================================
Purpose: 
Params:  
Return: 
=========================================================================*/
int TR_List_CheckForWaitingForConn( void* pvTransList, int iObjectID )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvPrevElement = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;


	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->chWaitingForChannel ){
						return 1;
					}
				}
			}
			pvPrevElement = pvListElement;
			pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		}
	}
	return 0;
} /* end of TR_List_CheckForWaitingForConn */

/*========================================================================
	TR_List_ClearWaitingForConn 
==========================================================================
Purpose: 
Params:  
Return: 
=========================================================================*/
int TR_List_ClearWaitingForConn( void* pvTransList, int iObjectID )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvPrevElement = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	int					iReturn = 1;
	UINT8				iError = 0;
	int					iSID = 0;


	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->chWaitingForChannel ){
						iSID = getDataChannel( (UINT8)iObjectID, ((pURLREQSTRUCT)(pTransStruct->pvReqStruct))->pbURL, &iError );
						if( iError == DB_err_success ){
							pTransStruct->iSID = iSID;
							pTransStruct->chWaitingForChannel = 0;
						}
						else
							iReturn = 0;
					}
				}
			}
			pvPrevElement = pvListElement;
			pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		}
	}

	return iReturn;
} /* end of TR_List_ClearWaitingForConn */

/*========================================================================
	TR_List_GetNextObjectTrans
==========================================================================
Purpose:
Params:
Return:
=========================================================================*/
int TR_List_GetNextObjectTrans( void* pvTransList, int iObjectID, long int* liTID, 
	void** pvReqStruct, unsigned long int* uiPid )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->chWaitingForChannel ){
						*liTID = (long int)SDL_GetListElementID( pvListElement );
						*pvReqStruct = pTransStruct->pvReqStruct;
						*uiPid = pTransStruct->uiPid;

						SDL_DeleteListElement( pvTransList, pvListElement );
						DEALLOC( &pTransStruct );
						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvTransList, pvListElement );
		}
	}
	return 0;
} /* end of TR_List_GetNextObjectTrans */

/*========================================================================
	TR_List_GetNext 
==========================================================================
Purpose: Get the next element from the Transaction-queue and delete this
			element from the queue.
Params:  pvTransList	void*					pointer to the queue
		 liSID			Long Int*
		 liTID			Long Int*
		 pvReqStruct	void**					pointer to a URLREQSTRUCT
		 liOptions		Long Int*
		 liPrio			Long Int*				Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNext( void* pvTransList, long int* liObjectID, long int* liSID, long int* liTID, 
	void** pvReqStruct, long int* liOptions,	long int* liPrio, unsigned long int* uiPid )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL && !(pTransStruct->chWaitingForChannel) ){
					*liObjectID = (long int)pTransStruct->iObjectID;
					*liSID = (long int)pTransStruct->iSID;
					*liTID = (long int)SDL_GetListElementID( pvListElement );
					*pvReqStruct = pTransStruct->pvReqStruct;
					*liOptions = (long int)pTransStruct->iOptions;
					*liPrio = (long int)pTransStruct->iPrio;
					*uiPid = pTransStruct->uiPid;

					SDL_DeleteListElement( pvTransList, pvListElement );
					DEALLOC( &pTransStruct );

					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pvTransList, pvListElement );
		}
	}
	return 0;
} /* end of TR_List_GetNext */

/*========================================================================
	TR_List_GetNextSIDTrans
==========================================================================
Purpose: Get the next element from the Transaction-queue with a specific iSID 
			and delete this	element from the queue.
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
		 liTID			Long Int*
		 pvReqStruct	void**					pointer to a URLREQSTRUCT
		 liOptions		Long Int*
		 liPrio			Long Int*				Priority
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNextSIDTrans( void* pvTransList, int iObjectID, int iSID, long int* liTID, 
	void** pvReqStruct, long int* liOptions,	long int* liPrio, unsigned long int* uiPid )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->iSID == iSID && !(pTransStruct->chWaitingForChannel) ){
						*liTID = (long int)SDL_GetListElementID( pvListElement );
						*pvReqStruct = pTransStruct->pvReqStruct;
						*liOptions = (long int)pTransStruct->iOptions;
						*liPrio = (long int)pTransStruct->iPrio;
						*uiPid = pTransStruct->uiPid;

						SDL_DeleteListElement( pvTransList, pvListElement );
						DEALLOC( &pTransStruct );
						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvTransList, pvListElement );
		}
	}
	return 0;
} /* end of TR_List_GetNextSIDTrans */

int TR_List_CheckForSIDTrans( void* pvTransList, int iObjectID, int iSID )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->iSID == iSID && !(pTransStruct->chWaitingForChannel) ){
						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvTransList, pvListElement );
		}
	}
	return 0;
} /* end of TR_List_GetNextSIDTrans */

/*========================================================================
	TR_List_DeleteTID
==========================================================================
Purpose: Delete an element from the Transaction-queue with a specific iTID.
Params:  pvTransList	void*					pointer to the queue
		 iTID			Int
		 uiPid			unsigned long int		SDL-Pid-Index
Return:  TRUE if the element was found, otherwise FALSE
=========================================================================*/
int TR_List_DeleteTID( void* pvTransList, int iTID, unsigned long int* uiPid,
					  long int* liObjectID, long int* liSID )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;


	if( pvTransList != NULL ){
		pvListElement = SDL_SearchListElement( pvTransList, iTID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					*uiPid = pTransStruct->uiPid;
					*liObjectID = (long int)pTransStruct->iObjectID;
					*liSID = (long int)pTransStruct->iSID;
					FreeReqStruct( &(pTransStruct->pvReqStruct) );
					SDL_DeleteListElement( pvTransList, pvListElement );
					DEALLOC( &pTransStruct );
					return 1;
				}
			}
		}
	}
	return 0;
} /* end of TR_List_DeleteTID */

/*========================================================================
	TR_List_DeleteTransactions
==========================================================================
Purpose: Delete elements from the Transaction-queue with a specific iSID.
Params:  pvTransList	void*					pointer to the queue
		 iSID			Int
Return:  TRUE
=========================================================================*/
/*int TR_List_DeleteTransactions( void* pvTransList, int iObjectID, int iSID )
{
	TRANSACTIONSTRUCT* pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvPrevElement = NULL;
	void*				pvElementContenet = NULL;

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iObjectID == iObjectID && pTransStruct->iSID == iSID ){
						FreeReqStruct( &(pTransStruct->pvReqStruct) );
						DEALLOC( &pTransStruct );
						SDL_DeleteListElement( pvTransList, pvListElement );
					}
					else
						pvPrevElement = pvListElement;
				}
				else
					pvPrevElement = pvListElement;
			}
			else
				pvPrevElement = pvListElement;
			pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		}
	}
	return 1;
	
}*/ /* end of TR_List_DeleteTransactions */

/*========================================================================
	TR_List_Delete 
==========================================================================
Purpose: Delete the hole Transaction-queue and each elements datastructure
Params:  pvTransList	void**		pointer to the queue
Return:  TRUE
=========================================================================*/
int TR_List_Delete( void** pvTransList )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;

	if( *pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( *pvTransList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					FreeReqStruct( &(pTransStruct->pvReqStruct) );
					
					DEALLOC( &pTransStruct );
				}
			}
			SDL_DeleteListElement( *pvTransList, pvListElement );
			pvListElement = SDL_GetNextListElement( *pvTransList, NULL );
		}

		SDL_DeleteList( pvTransList );
	}

	return 1;
} /* end of TR_List_Delete */

/*========================================================================
	SE_List_Add
==========================================================================
Purpose: Add a SID to the sessionList, iMaxNrOfTrans defines how many 
			transactions a session can simultaneous handle.
=========================================================================*/
int SE_List_Add( void* pvSessionList, unsigned long int uiPid, int iObjectID, int iSID, int iMaxNrOfTrans )
{
	SESSIONSTRUCT* pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pSessionStruct = NEWSTRUCT(SESSIONSTRUCT);

		if( pSessionStruct != NULL ){
			pSessionStruct->iSID = iSID;
			pSessionStruct->iConnected = 0;
			pSessionStruct->uiPid = uiPid;
			pSessionStruct->iNrOfFreeTransaction = iMaxNrOfTrans;

			return SDL_AddElementLast(pvSessionList, iObjectID, (void*)pSessionStruct );
		}
	}
	return 0;
	
} /* end of SE_List_Add */
/*
int SE_List_SetID( void* pvSessionList, int iObjectID, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == -1 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					SDL_ChangeListElementID( pvListElement, iObjectID );
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					pSessionStruct->iSID = iSID;
					return 1;
				}
			}
			else{
				pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
			}
		}
	}
	return 0;
	
}*/ /* end of SE_List_SetID */

unsigned long int SE_List_GetFirstPid( void* pvSessionList )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) != -1 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID != -1){
						return pSessionStruct->uiPid;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_GetFirstPid */

int SE_List_DeleteSession( void* pvSessionList, unsigned long int uiPid, long int* iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	int					iObjectID = 0;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	*iSID = 0;
	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
				if( pSessionStruct != NULL && pSessionStruct->uiPid == uiPid ){
					iObjectID = SDL_GetListElementID( pvListElement );
					*iSID = pSessionStruct->iSID;
					DEALLOC( &pSessionStruct );
					SDL_DeleteListElement( pvSessionList, pvListElement );
					return iObjectID;
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_DeleteSession */

/*========================================================================
	SE_List_GetPid 
=========================================================================*/
unsigned long int SE_List_GetPid( void* pvSessionList, int iObjectID, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						return pSessionStruct->uiPid;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_GetPid */

/*========================================================================
	SE_List_CheckConnection
==========================================================================
Purpose: Check if a specific Session is connected to the server.
Return:  Return 1 if connected, otherwise 0
=========================================================================*/
int SE_List_CheckConnection( void* pvSessionList, int iObjectID, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						if( pSessionStruct->iConnected == 1 )
							return 1;
						else
							return 0;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_CheckConnection */

/*========================================================================
	SE_List_CheckNrOfFreeTrans
==========================================================================
Purpose: Check if a session can handle a new transaction, retuns 1 if OK, 
			otherwise 0.
=========================================================================*/
int SE_List_CheckNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						if( pSessionStruct->iNrOfFreeTransaction > 0 )
							return 1;
						else
							return 0;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_CheckNrOfFreeTrans */

/*========================================================================
	SE_List_ChangeConnection
==========================================================================
Purpose: Change the connection state to a specific session.
=========================================================================*/
int SE_List_ChangeConnection( void* pvSessionList, int iObjectID, int iSID, int iConnected )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						pSessionStruct->iConnected = iConnected;
						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_ChangeConnection */

/*========================================================================
	SE_List_ChangeNrOfFreeTrans
==========================================================================
Purpose: Change the number of Free transactions to a specific session.
=========================================================================*/
int SE_List_ChangeNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID, int iIncDec )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						pSessionStruct->iNrOfFreeTransaction = pSessionStruct->iNrOfFreeTransaction + iIncDec;

						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_ChangeNrOfFreeTrans */

/*========================================================================
	SE_List_SetNrOfFreeTrans
==========================================================================
Purpose: Set the maximum number of transactions a session can simultaneous
			handle.
=========================================================================*/
int SE_List_SetNrOfFreeTrans( void* pvSessionList, int iObjectID, int iSID, int iMaxNrOfTrans )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( pvSessionList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == iObjectID ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pSessionStruct = (SESSIONSTRUCT*)pvElementContent;
					if( pSessionStruct != NULL && pSessionStruct->iSID == iSID ){
						pSessionStruct->iNrOfFreeTransaction = iMaxNrOfTrans;
						return 1;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pvSessionList, pvListElement );
		}
	}
	return 0;
	
} /* end of SE_List_SetNrOfFreeTrans */

/*========================================================================
	SE_List_DeleteSession
==========================================================================
Purpose: Deletes a specific session struct from the list.
=========================================================================*/
/*int SE_List_DeleteSession( void* pvSessionList, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				DEALLOC( &pSessionStruct );
			}
			SDL_DeleteListElement( pvSessionList, pvListElement );
			return 1;
		}
	}
	return 0;
	
}*/ /* end of SE_List_DeleteSession */

/*========================================================================
	SE_List_Delete
==========================================================================
Purpose: Deletes the hole list.
=========================================================================*/
int SE_List_Delete( void** pvSessionList )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

	if( *pvSessionList != NULL ){
		pvListElement = SDL_GetNextListElement( *pvSessionList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				if( pSessionStruct != NULL )
					DEALLOC( &pSessionStruct );
			}
			SDL_DeleteListElement( *pvSessionList, pvListElement );
			pvListElement = SDL_GetNextListElement( *pvSessionList, NULL );
		}
		SDL_DeleteList( pvSessionList );
	}

	return 1;
} /* end of SE_List_Delete */

int ACT_TR_List_Add( void* pvList, int iTID, unsigned long int uiSM_Pid )
{
	ACTIVETRANSSTRUCT* pActTransStruct = NULL;

	if( pvList != NULL ){
		pActTransStruct = NEWSTRUCT(ACTIVETRANSSTRUCT);

		if( pActTransStruct != NULL ){
			pActTransStruct->uiSMPid = uiSM_Pid;
			pActTransStruct->uiTRPid = 0;

			return SDL_AddElementLast(pvList, iTID, (void*)pActTransStruct );
		}
	}
	return 0;
	
} /* end of ACT_TR_List_Add */

int ACT_TR_List_SetTR_Pid( void* pvList, int iTID, unsigned long int uiTR_Pid )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	ACTIVETRANSSTRUCT*	pActTransStruct = NULL;

	if( pvList != NULL ){
		pvListElement = SDL_SearchListElement( pvList, iTID, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pActTransStruct = (ACTIVETRANSSTRUCT*)pvElementContent;
				pActTransStruct->uiTRPid = uiTR_Pid;
				return 1;
			}
		}
	}
	return 0;
	
} /* end of ACT_TR_List_SetTR_Pid */

unsigned long int ACT_TR_List_GetPid( void* pvList, int iTID, int iSM_TR_pid )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	ACTIVETRANSSTRUCT*	pActivTransStruct = NULL;

	if( pvList != NULL ){
		pvListElement = SDL_SearchListElement( pvList, iTID, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pActivTransStruct = (ACTIVETRANSSTRUCT*)pvElementContent;
				if( iSM_TR_pid == 0 )
					return pActivTransStruct->uiSMPid;
				else
					return pActivTransStruct->uiTRPid;
			}
		}
	}
	return 0;
} /* end of ACT_TR_List_GetPid */


int ACT_TR_List_DeleteTID( void* pvList, int iTID )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;

	if( pvList != NULL ){
		pvListElement = SDL_SearchListElement( pvList, iTID, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL )
				DEALLOC( &pvElementContent );
			SDL_DeleteListElement( pvList, pvListElement );
		}
	}
	return 1;
	
} /* end of ACT_TR_List_DeleteTID */

unsigned long int ACT_TR_List_GetPidFromElement( void* pvList, unsigned long int uiSM_Pid,
												void** ppListElement )
{
	void*				pvListElement = NULL;
	void*				pvElementContent = NULL;
	ACTIVETRANSSTRUCT*	pActivTransStruct = NULL;

	if( pvList != NULL ){
		pvListElement = *ppListElement;
		pvListElement = SDL_GetNextListElement( pvList, pvListElement );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pActivTransStruct = (ACTIVETRANSSTRUCT*)pvElementContent;
				if( pActivTransStruct != NULL && pActivTransStruct->uiSMPid == uiSM_Pid ){
					*ppListElement = pvListElement;
					return pActivTransStruct->uiTRPid;
				}
			}
			pvListElement = SDL_GetNextListElement( pvList, pvListElement );
		}
	}
	return 0;
} /* end of ACT_TR_List_GetPidFromElement */

int ACT_TR_List_Reomve_Pid( void* pvList, unsigned long int uiTR_Pid )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	ACTIVETRANSSTRUCT*	pActivTransStruct = NULL;

	if( pvList != NULL ){
		pvListElement = SDL_GetNextListElement( pvList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pActivTransStruct = (ACTIVETRANSSTRUCT*)pvElementContenet;
				if( pActivTransStruct != NULL && pActivTransStruct->uiTRPid == uiTR_Pid ){
					DEALLOC( &pActivTransStruct );
					SDL_DeleteListElement( pvList, pvListElement );
					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pvList, pvListElement );
		}
	}
	return 0;
} /* end of ACT_TR_List_Reomve_Pid */

int ACT_TR_List_Delete( void** pvList )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	ACTIVETRANSSTRUCT*	pActivTransStruct = NULL;

	if( *pvList != NULL ){
		pvListElement = SDL_GetNextListElement( *pvList, NULL );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pActivTransStruct = (ACTIVETRANSSTRUCT*)pvElementContenet;
				if( pActivTransStruct != NULL )
					DEALLOC( &pActivTransStruct );
			}
			SDL_DeleteListElement( *pvList, pvListElement );
			pvListElement = SDL_GetNextListElement( *pvList, NULL );
		}
		SDL_DeleteList( pvList );
	}

	return 1;
} /* end of ACT_TR_List_Delete */






/*========================================================================
				FUNCTIONS ONLY USED IN CONNECTIONORIENTED
=========================================================================*/

#ifdef CONFIG_CO

/*========================================================================
	CreateConnectHeader
==========================================================================
Purpose: Create a ConnectHeader that you can send to the Gateway.
Params:  pvStruct			UASETTINGS*		casted to void*
		 pNetStruct			NETWORKSTRUCT*	casted to void*
		 pvWAEMaStruct		WAEMAINOBJECT*	casted to VOID*
		 pliHeaderLength	long int*
Return:  Pointer to the ConnectHeader
=========================================================================*/
void* CreateConnectHeader( VOID* pvStruct, VOID* pNetStruct, 
						  VOID* pvWAEMaStruct, long int* pliHeaderLength )
{
	pUASETTINGS	pUaSettings = NULL;
/*  Added by GBU,000224 */
#ifdef CONFIG_PUSH
	NETWORKSTRUCT* pNetworkStruct = NULL;
#endif
	pHEADBUILDSTR	pList = NULL;
	pHEADBUILDSTR	pStartList = NULL;
	pWAEMAINOBJECT pWAEMainStruct = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbString = NULL;
	BYTE*		pbTemp = NULL;
	BYTE*		pbSeparator = NULL;
	UINT32		uiLength = 0;
	UINT16		i = 0;
	BYTE		tempArray[2];

	*pliHeaderLength = 0;

	if( pvStruct != NULL ){
		pUaSettings = (pUASETTINGS)pvStruct;
		if( pUaSettings != NULL ){
			if( pUaSettings->pbAccept != NULL ){
				uiLength = B_STRINGLENGTH( pUaSettings->pbAccept );
				pbString = NEWARRAY(BYTE,uiLength+1);
				if( pbString != NULL ){
					B_COPYSTRING( pbString, pUaSettings->pbAccept );
					RemoveSpace( pbString );
					uiLength = B_STRINGLENGTH( pbString );

					pbTemp = pbString;
					pbSeparator = FindSegment( pbTemp );
					while( pbSeparator != NULL )
					{
						*pbSeparator = 0;
						pbSeparator++;
						uiLength = B_STRINGLENGTH( pbTemp );
						WSP_CreateHeaderField( Field_Accept, pbTemp, (uiLength+1), &pList);
						if( pStartList == NULL )
							pStartList = pList;
						pbTemp = pbSeparator;
						pbSeparator = FindSegment( pbTemp );
					}
					uiLength = B_STRINGLENGTH( pbTemp );
					WSP_CreateHeaderField( Field_Accept, pbTemp, (uiLength+1), &pList);
					if( pStartList == NULL )
						pStartList = pList;

					pbTemp = NULL;
					pbSeparator = NULL;
					DEALLOC( &pbString );
				}
				uiLength = 0;
			}
			
			if( pUaSettings->pbProxyCredentials != NULL && pUaSettings->iProxyCredentialsLength > 0 ){
				pbString = NEWARRAY(BYTE,pUaSettings->iProxyCredentialsLength+1);
				if( pbString != NULL ){
					B_COPYSTRINGN( pbString, pUaSettings->pbProxyCredentials, pUaSettings->iProxyCredentialsLength );
					pbString[pUaSettings->iProxyCredentialsLength] = 0;
					WSP_CreateHeaderField( Field_Proxy_Authorization, pbString, pUaSettings->iProxyCredentialsLength, &pList);
					if( pStartList == NULL )
						pStartList = pList;

					DEALLOC( &pbString );
				}
			}

			if( pUaSettings->pbLanguage != NULL && pUaSettings->iLanguage_Length > 0 ){
				for ( i=0; i<pUaSettings->iLanguage_Length; i++ ){
					WSP_CreateHeaderField( Field_Accept_Language, &(pUaSettings->pbLanguage[i]), 1, &pList);
					if( pStartList == NULL )
						pStartList = pList;
				}
			}
		}
	}

	if( NR_OF_ACCEPTCHARSET > 0 ){
		for ( i=0; i<NR_OF_ACCEPTCHARSET; i++ ){
			if( arrayAcceptCharset[i] < 256 ){
				tempArray[0] = arrayAcceptCharset[i] & 0xff;

				WSP_CreateHeaderField( Field_Accept_Charset, tempArray, 1, &pList);
			}
			else{
				tempArray[0] = (arrayAcceptCharset[i] >> 8) & 0xff;
				tempArray[1] = arrayAcceptCharset[i] & 0xff;

				WSP_CreateHeaderField( Field_Accept_Charset, tempArray, 2, &pList);

			}
			if( pStartList == NULL )
				pStartList = pList;
		}
	}

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		if( pWAEMainStruct->pbPROFILE != NULL ){
			uiLength = B_STRINGLENGTH( pWAEMainStruct->pbPROFILE );
			if( uiLength > 0 ){
				WSP_CreateHeaderField( Field_Profile, pWAEMainStruct->pbPROFILE, (uiLength+1), &pList);
				if( pStartList == NULL )
					pStartList = pList;
			}
		}

#ifdef FEATURE_WAP
	{
		int templength = 0;
		MakeB5_info();
		templength = get_position((char *)agent+brow_length);
		agent[brow_length+templength] ='\x0';

		uiLength = B_STRINGLENGTH((const char *)agent);
	}
#endif//#ifdef FEATURE_WAP

		if( pWAEMainStruct->pbUSERAGENT != NULL ){
#ifndef FEATURE_WAP
			uiLength = B_STRINGLENGTH( pWAEMainStruct->pbUSERAGENT );
#endif //#ifndef FEATURE_WAP
			if( uiLength > 0 ){
#ifdef FEATURE_WAP
		        WSP_CreateHeaderField( Field_User_Agent, agent, (uiLength+1), &pList);
#else
				WSP_CreateHeaderField( Field_User_Agent, pWAEMainStruct->pbUSERAGENT, (uiLength+1), &pList);
#endif //#ifdef FEATURE_WAP
				if( pStartList == NULL )
					pStartList = pList;
			}
		}

				/* PUSH - Headers */
#ifdef CONFIG_PUSH

		if( pNetStruct != NULL ){
			pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
			tempArray[0] = pNetworkStruct->iActiveBearer;
			WSP_CreateHeaderField( Field_Bearer_Indication, tempArray, 1, &pList);
			if( pStartList == NULL )
				pStartList = pList;
		}
		tempArray[0] = APP_ID_SIA;
		WSP_CreateHeaderField( Field_Accept_Application, tempArray, 1, &pList);
		if( pStartList == NULL )
			pStartList = pList;

		tempArray[0] = APP_ID_WML_UA;
		WSP_CreateHeaderField( Field_Accept_Application, tempArray, 1, &pList);
		if( pStartList == NULL )
			pStartList = pList;

	#ifdef CONFIG_WTA
		tempArray[0] = APP_ID_WTA_UA;
		WSP_CreateHeaderField( Field_Accept_Application, tempArray, 1, &pList);
		if( pStartList == NULL )
			pStartList = pList;
	#endif

#else
			/*  Added by GBU,000228 to get rid of warning for unused variable */
		pNetStruct = pNetStruct;
#endif


	}

	if( pStartList != NULL ){
		uiLength = 0;
		pbHeader = WSP_BuildHeader( pStartList, &uiLength );
	}

	*pliHeaderLength = uiLength;
	return (void*)pbHeader;
} /* end of CreateConnectHeader */

/*========================================================================
	CheckForProfileInfo
==========================================================================
Purpose: Check if the gateway supports UAProf features.
Params:  pvServerHeaders	BYTE*	casted to void*
		 iLen				int
Return:  1 if the Gateway support UAProf, otherwise 0.
=========================================================================*/
int CheckForProfileInfo( VOID* pvServerHeaders, int iLen)
{
	BYTE*			pbHeaders = NULL;
	pHEADERDEF		pHeaderList = NULL;
	pHEADERELEMENT	pHeaderElement = NULL;
	BOOL			boError = FALSE;
	int				iReturn = 0;

	if( pvServerHeaders != NULL && iLen > 0 ){
			/* Parsa byteströmmen */
		/*pbHeaders = (BYTE*)pvServerHeaders;*/
		
			/* Temporary fix start */
		pbHeaders = NEWARRAY( BYTE, iLen+1 );
		pbHeaders[0] = 0;
		B_COPYSTRINGN( &(pbHeaders[1]),(BYTE*)pvServerHeaders, iLen );
			/* Temporary fix end */

		/*pHeaderList = WSP_PreParseHeaders ( pbHeaders, iLen, &boError );*/

			/* Temporary fix */
		pHeaderList = WSP_PreParseHeaders ( pbHeaders, iLen+1, &boError );
		if( pHeaderList != NULL && boError == FALSE )
		{
			if( pHeaderList->pHeadList != NULL )
			{
				pHeaderElement = WSP_GetHeaderWK ( 1, Field_Profile_Warning, pHeaderList->pHeadList );
				if( pHeaderElement != NULL )
				{				/* Check if Profile-Warning is OK (100, 0x10)*/
					if( WSP_GetProfileWarning( pHeaderElement ) == 0x10 ){
							/*Profile enabled*/
						iReturn = 1;
					}
				}
			}
		}
		if( pHeaderList != NULL )
			DeleteHEADERDEF( pHeaderList );
			/* Temporary fix */
		DEALLOC(&pbHeaders);
	}

	return iReturn;
} /* end of CheckForProfileInfo */

/*========================================================================
	ParseRedirectAddress
==========================================================================
Purpose: Parse the Redirect Address and stores the new Address in the 
			Network struct
Params:  pvRedirectAddress	BYTE*				casted to void*
		 iAddressLength		int
		 pNetStruct			NETWORKSTRUCT*		casted to void*
		 uiPort				unsigned long int*
Return:  1 if success, otherwise 0.
=========================================================================*/
int ParseRedirectAddress( /*void* pvRedirectAddress, int iAddressLength,
						 void* pNetStruct, unsigned long int* uiPort, int iChannelRef*/
						 void* pvRedirectAddress, int iAddressLength,
						  void** pvAddressList )
{
	BYTE*			pbAddress = NULL;
	ADDRESS*		pAddressList = NULL;

	if( *pvAddressList != NULL ){
		WSP_DeleteAddress( (ADDRESS**)pvAddressList );
		*pvAddressList = NULL;
	}
	if( pvRedirectAddress != NULL && iAddressLength > 0 ){
			/* Parsa byteströmmen */
		pbAddress = (BYTE*)pvRedirectAddress;

		if( WSP_ParseAddress( pbAddress, iAddressLength, &pAddressList ) ){
			if( pAddressList != NULL ){
				*pvAddressList = (void*)pAddressList;
				return 1;
			}
		}
	}

	return 0;

/*	
	BYTE*			pbAddress = NULL;
	NETWORKSTRUCT*	pNetworkStruct = NULL;
	ADDRESS*		pAddressList = NULL;
	DB_ref			dbChannelRef = 0;

	*uiPort = 0;
	dbChannelRef = (DB_ref)iChannelRef;
	if( pvRedirectAddress != NULL && iAddressLength > 0 && pNetStruct != NULL ){

		pbAddress = (BYTE*)pvRedirectAddress;

		if( WSP_ParseAddress( pbAddress, iAddressLength, &pAddressList ) ){
			if( pAddressList != NULL ){
				pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
				
				if( pAddressList->iPortNumber != -1 )
					*uiPort = pAddressList->iPortNumber;

				if( pAddressList->iBearerType != -1 ){
					pNetworkStruct->iActiveBearer = pAddressList->iBearerType;
					setInt(dbChannelRef, "AccessType", pNetworkStruct->iActiveBearer );
				}
				
				if( pAddressList->pbAddress != NULL && pAddressList->uiAddressLength > 0 ){
					switch( pNetworkStruct->iActiveBearer )
					{
						case BEARER_ANY_UDP:
						case BEARER_GSM_CSD:
						case BEARER_GSM_GPRS:
							{
								if( pNetworkStruct->pbUDP_GW_IP != NULL )
									DEALLOC( &pNetworkStruct->pbUDP_GW_IP );
								pNetworkStruct->pbUDP_GW_IP = NEWARRAY( BYTE, pAddressList->uiAddressLength );
								if( pNetworkStruct->pbUDP_GW_IP != NULL ){
									B_COPYSTRINGN( pNetworkStruct->pbUDP_GW_IP, pAddressList->pbAddress, pAddressList->uiAddressLength );
									pNetworkStruct->pbUDP_GW_IP_LEN = pAddressList->uiAddressLength;
*/								/*	setMem(dbChannelRef, "UDP_GW_IP", BYTE* value);*/
/*								}
							}
							break;
						case BEARER_GSM_USSD:
							{
								if( pNetworkStruct->pbUSSD_CENTER != NULL )
									DEALLOC( &pNetworkStruct->pbUSSD_CENTER );
								pNetworkStruct->pbUSSD_CENTER = NEWARRAY( BYTE, pAddressList->uiAddressLength );
								if( pNetworkStruct->pbUSSD_CENTER != NULL ){
									B_COPYSTRINGN( pNetworkStruct->pbUSSD_CENTER, pAddressList->pbAddress, pAddressList->uiAddressLength );
									pNetworkStruct->pbUSSD_CENTER_LEN = pAddressList->uiAddressLength;
								}
							}
							break;
						case BEARER_GSM_SMS:
							{
								if( pNetworkStruct->pbSMS_GW != NULL )
									DEALLOC( &pNetworkStruct->pbSMS_GW );
								pNetworkStruct->pbSMS_GW = NEWARRAY( BYTE, pAddressList->uiAddressLength );
								if( pNetworkStruct->pbSMS_GW != NULL ){
									B_COPYSTRINGN( pNetworkStruct->pbSMS_GW, pAddressList->pbAddress, pAddressList->uiAddressLength );
									pNetworkStruct->pbSMS_GW_LEN = pAddressList->uiAddressLength;
								}
							}
							break;
					}
				}

				WSP_DeleteAddress( &pAddressList );

				return 1;
			}
		}
	}

	return 0;*/
} /* end of ParseRedirectAddress */

#ifdef FEATURE_WAP
extern BYTE SaveSelfMin[];
static void MakeMin(BYTE* pT)
{
   strncpy((char *)pT,(const char *)SaveSelfMin,8); 
}

static void MakeB5_info(void)
{
	BYTE* pAgent = agent;

	brow_length = 0;

	strncpy((char *)pAgent,SKT,3);
	pAgent += 3;
	brow_length+=3;

	if(cdma.p_rev_in_use >=6) 
		*pAgent++ = SKT95C;
	else
		*pAgent++ = S_WAP;
	brow_length++;


	*pAgent++ = CELLULAR_95C;
	brow_length++;

	strncpy((char *)pAgent,(const char *)GEGO,2);
	pAgent+=2;
	brow_length+=2;

	strncpy((char *)pAgent,(const char *)IM_3100,2);
	pAgent+=2;	
	brow_length+=2;
	
	strncpy((char *)pAgent,(const char *)AU_SYSTEM,2);
	pAgent+=2;	
	brow_length+=2;
	
	strncpy((char *)pAgent,(const char *)WAP_VER,2);
	pAgent+=2;	
	brow_length+=2;

	strncpy((char *)pAgent,(const char *)LCDPIXCEL_B1,6);
	pAgent+=6;	
	brow_length+=6;
	
	strncpy((char *)pAgent,(const char *)LCDSIZE_B1,4);
	pAgent+=4;	
	brow_length+=4;
	
	strncpy((char *)pAgent,(const char *)LCDCOLOR_256,2);
	pAgent+=2;	
	brow_length+=2;
	
	MakeMin(pAgent);
	brow_length+=8;	
}
#endif //#ifdef FEATURE_WAP
#endif


