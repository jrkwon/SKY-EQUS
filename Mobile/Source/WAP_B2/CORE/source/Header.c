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

------------------------------------------------------------------------*/

/* Private include statements */

#include "header.h"
#include "HdrUtil.h"
#include "cmmnrsrc.h"
#include "waedef.h"
#include "url.h"
#include "WAE_Cmmn.h"
#include "WAEDef.h"
#include "aapiclnt.h"
#include "capiclnt.h"
#include "confvars.h"
#include "aapimmi.h"
#include "mccdma.h"

#ifdef CONFIG_CO
	#include "WSPCapa.h"
#endif

//jjkyg 11/3 for SKTELECOM COMMAND
extern int get_position(char *result);

//extern void  CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
#define SKTSPEC
#ifdef SKTSPEC
extern BOOL makeagent;
BYTE agent[100];
WORD brow_length = 0;
static void MakeMin(BYTE* pT);
static void MakeB5_info(void);
#endif

typedef struct
{
	int					iSID;
	int					iTID;
	void*				pvReqStruct;
	int					iReload;
	int					iPrio;
	unsigned long int	uiPid;
} TRANSACTIONSTRUCT, *pTRANSACTIONSTRUCT;

typedef struct
{
	int		iConnected;
	int		iNrOfFreeTransaction;
} SESSIONSTRUCT, *pSESSIONSTRUCT;

typedef struct
{
	unsigned long int uiPid;
} PIDSTRUCT, *pPIDSTRUCT;


const INT16 arrayAcceptCharset[NR_OF_ACCEPTCHARSET] = INIT_ACCEPTCHARSET;

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
	EXTERNAL FUNCTIONS
=========================================================================*/

/*========================================================================
	urlIsValid
==========================================================================
Purpose: Validate a Url and consist of scheme, host and a path. 
Params:  pvReqStruct	pURLREQSTRUCT	casted to a VOID*
Return:  TRUE if valid, otherwise FALSE
=========================================================================*/
int urlIsValid( VOID* pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*			pbFrag = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start urlIsValid");
#endif
	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL ){
/*  Modified by GBU,0001014 */
/*
			pbFrag = B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
*/
			//EQUS
            pbFrag =  (BYTE *)strstr((const char *)pUrlReq->pbURL,"#" );
			if( pbFrag != NULL )
				*pbFrag = 0;
			if( b_UrlIsCompleteValid( pUrlReq->pbURL ) ){
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("End urlIsValid");
#endif
				return 1;
			}
		}
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End urlIsValid");
#endif

	return 0;
}  /* end of urlIsValid */

/*========================================================================
	FreeReqStruct 
==========================================================================
Purpose: Release a URLREQSTRUCT and all of it's parts
Params:  pvReqStruct	pURLREQSTRUCT*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeReqStruct( VOID** pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start FreeReqStruct");
#endif

	if( *pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)(*pvReqStruct);

		Delete_URLREQSTRUCT( &pUrlReq );
		*pvReqStruct = NULL;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End FreeReqStruct");
#endif
	return TRUE;

} /* end of FreeReqStruct */

/*========================================================================
	FreeNetStruct 
==========================================================================
Purpose: Release a NETWORKSTRUCT and all of it's parts
Params:  vsNetStruct	pNETWORKSTRUCT*	casted to a VOID**
Return:	 TRUE
=========================================================================*/
int FreeNetStruct(VOID** vsNetStruct)
{
	pNETWORKSTRUCT	pNetWorkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start FreeNetStruct");
#endif

	if( *vsNetStruct != NULL )
	{
		pNetWorkStruct = (pNETWORKSTRUCT)(*vsNetStruct);

		Delete_NETWORKSTRUCT( &pNetWorkStruct );
		*vsNetStruct = NULL;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End FreeNetStruct");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start FreeUaSettingsStruct");
#endif

	if( *vsStruct != NULL )
	{
		pUaSettings = (pUASETTINGS)(*vsStruct);

		if( pUaSettings->pbAccept != NULL )
			DEALLOC( &pUaSettings->pbAccept );
		if( pUaSettings->pbProxyCredentials != NULL )
			DEALLOC( &pUaSettings->pbProxyCredentials );
		if( pUaSettings->pbLanguage != NULL )
			DEALLOC( &pUaSettings->pbLanguage );

		DEALLOC( vsStruct );
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End FreeUaSettingsStruct");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetMethod");
#endif
	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		iReturn = pUrlReq->iMethod;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetMethod");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetUrl");
#endif
	*pvUrl = NULL;

	if( pvReqStruct != NULL )
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;

		if( pUrlReq->pbURL != NULL ){
			//EQUS
			uiUrlLength = B_STRINGLENGTH((const char *)pUrlReq->pbURL );
			pbTemp2 = NEWARRAY(BYTE,uiUrlLength+1);
			if( pbTemp2 != NULL ){
				//EQUS
				B_COPYSTRING((char *)pbTemp2, (const char *)pUrlReq->pbURL );
				pbTemp2[uiUrlLength] = 0;
				*pvUrl = (VOID*)pbTemp2;
			}
			else
				uiUrlLength = 0;
		}
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetUrl");
#endif

	return (int)uiUrlLength;
} /* end of GetUrl */

/*========================================================================
	GetHeader 
==========================================================================
Purpose: Create the Header from the URLREQSTRUCT and pUASETTINGS
Params:  pvReqStruct	URLREQSTRUCT*	casted to VOID*
		 pvUaSettings	pUASETTINGS		casted to a VOID*
		 pvHeader		BYTE**			casted to VOID**
Return:  length of pvHeader
=========================================================================*/
int GetHeader( VOID* pvReqStruct, VOID* pvUaSettings, int iConnectionMode, VOID** pvHeader )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	pUASETTINGS		pUaSet = NULL;
	pHEADBUILDSTR	pList = NULL;
	pHEADBUILDSTR	pStartList = NULL;
	BYTE*		pbString = NULL;
	BYTE*		pbTemp = NULL;
	BYTE*		pbSeparator = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbFrag = NULL;
	UINT32		uiLength = 0;
	UINT32		uiDataLength = 0;
	UINT16		i = 0;
	BYTE		tempArray[2];

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetHeader");
#endif

	if( pvReqStruct != NULL && pvUaSettings != NULL)
	{
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;
		pUaSet = (pUASETTINGS)(pvUaSettings);
		if( (iConnectionMode == 0 && pUrlReq->pbAccept != NULL) || 
/*  Modified by GBU,000114 */
/*
			(iConnectionMode == 1 && pUrlReq->pbAccept != NULL && B_COMPARESTRING(pUrlReq->pbAccept,pUaSet->pbAccept) != 0) )
*/
            (iConnectionMode == 1 && pUrlReq->pbAccept != NULL && strcmp((const char *)pUrlReq->pbAccept,(const char *)pUaSet->pbAccept) != 0) )
		{
			uiLength = B_STRINGLENGTH((const char *)pUrlReq->pbAccept );
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL )
				B_COPYSTRING((char *)pbString,(const char *)pUrlReq->pbAccept );
		}
		else if( iConnectionMode == 0 && pUaSet->pbAccept != NULL ){
			uiLength = B_STRINGLENGTH((const char *)pUaSet->pbAccept );
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL )
				B_COPYSTRING((char *)pbString,(const char *)pUaSet->pbAccept );
		}
		if( pbString != NULL ){
			RemoveSpace( pbString );
			uiLength = B_STRINGLENGTH((const char *)pbString );
#ifdef WAE_DEBUG
			URL_DEBUG_PRINT("Field_Accept");
			URL_DEBUG_PRINT(pbString);
#endif

			pbTemp = pbString;
			pbSeparator = FindSegment( pbTemp );
			while( pbSeparator != NULL )
			{
				*pbSeparator = 0;
				pbSeparator++;
				uiDataLength = B_STRINGLENGTH((const char *)pbTemp );
				WSP_CreateHeaderField( Field_Accept, pbTemp, (uiDataLength+1), &pList);
				if( pStartList == NULL )
					pStartList = pList;
				pbTemp = pbSeparator;
				pbSeparator = FindSegment( pbTemp );
			}
			uiDataLength = B_STRINGLENGTH((const char *)pbTemp );
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
#ifdef WAE_DEBUG
			URL_DEBUG_PRINT("Field_Accept_Language");
#endif
			for ( i=0; i<pUaSet->iLanguage_Length; i++ ){
				WSP_CreateHeaderField( Field_Accept_Language, &(pUaSet->pbLanguage[i]), 1, &pList);
				if( pStartList == NULL )
					pStartList = pList;
			}
		}

/* Only in ConnectionLess Mode */
		if( iConnectionMode == 0 && NR_OF_ACCEPTCHARSET > 0 ){
#ifdef WAE_DEBUG
			URL_DEBUG_PRINT("Field_Accept_Charset");
#endif
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
/*  Modified by GBU,0001014 */
/*
			pbFrag = B_STRSTRING( pUrlReq->pbReferer , (BYTE*)"#" );
*/
            pbFrag =  (BYTE *)strstr((const char *)pUrlReq->pbReferer, "#" );

			if( pbFrag != NULL )
				uiDataLength = pbFrag-(pUrlReq->pbReferer);
			else
				uiDataLength = B_STRINGLENGTH((const char *)pUrlReq->pbReferer );

			pbString = NEWARRAY(BYTE,uiDataLength+1);
			if( pbString != NULL ){
				B_COPYSTRINGN( pbString, pUrlReq->pbReferer, uiDataLength );
				pbString[uiDataLength] = 0;
				WSP_CreateHeaderField( Field_Referer, pbString, (uiDataLength+1), &pList);
				if( pStartList == NULL )
					pStartList = pList;

#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Referer");
				URL_DEBUG_PRINT(pbString);
#endif

				DEALLOC( &pbString );
			}
			uiLength = 0;
		}

		if( pUrlReq->pbCredentials != NULL && pUrlReq->iCredentialsLength > 0 )
		{
			uiLength = pUrlReq->iCredentialsLength;
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL ){
				B_COPYSTRINGN( pbString, pUrlReq->pbCredentials, uiLength );
				pbString[uiLength] = 0;
				WSP_CreateHeaderField( Field_Authorization, pbString, uiLength, &pList);
				if( pStartList == NULL )
					pStartList = pList;

#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Authorization");
				URL_DEBUG_PRINT(pbString);
#endif

				DEALLOC( &pbString );
			}
			uiLength = 0;
		}

/* Only in ConnectionLess Mode */
		if( iConnectionMode == 0 && pUaSet->pbProxyCredentials != NULL && pUaSet->iProxyCredentialsLength > 0 )
		{
			uiLength = pUaSet->iProxyCredentialsLength;
			pbString = NEWARRAY(BYTE,uiLength+1);
			if( pbString != NULL ){
				B_COPYSTRINGN( pbString, pUaSet->pbProxyCredentials, uiLength );
				pbString[uiLength] = 0;
				WSP_CreateHeaderField( Field_Proxy_Authorization, pbString, uiLength, &pList);
				if( pStartList == NULL )
					pStartList = pList;

#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Proxy_Authorization");
				URL_DEBUG_PRINT(pbString);
#endif

				DEALLOC( &pbString );
			}
			uiLength = 0;
		}

		//jjkyg
//		if(
//			(iConnectionMode == 0)||(iConnectionMode == 1)
//		  ) 
//		{
#ifdef SKTSPEC
		{			
			int templength = 0;
			if(makeagent)
			{
				MakeB5_info();
				makeagent =FALSE;
			}
			templength = get_position((char *)agent+brow_length);

			agent[brow_length+templength] ='\x0';

			uiLength = B_STRINGLENGTH((const char *)agent);
			WSP_CreateHeaderField( Field_User_Agent, agent, (uiLength+1), &pList);			
		}
#else
			uiLength = B_STRINGLENGTH( (BYTE*)HTTP_HEADER_USER_AGENT );
			WSP_CreateHeaderField( Field_User_Agent, (BYTE*)HTTP_HEADER_USER_AGENT, (uiLength+1), &pList);
#endif			
			if( pStartList == NULL )
				pStartList = pList;
//jjkyg
	//	}

		if( pStartList != NULL )
		{
			uiLength = 0;
			pbHeader = WSP_BuildHeader( pStartList, &uiLength );
		}

	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetHeader");
#endif
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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetBody");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetBody");
#endif

	*pvBody = (VOID*)pbOctetString;
	return (int)uiUrlLength;
} /* end of GetBody */

/*========================================================================
	CreateWSPHeader
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateWSPHeader( VOID* pvHeader, int iHeadLen,
		unsigned long int uiLastMod, int iReload, unsigned char ucMethod,
		VOID* pvReqStruct, long int* pliHeaderLength )
{
	pURLREQSTRUCT	pUrlReq = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbOctetString = NULL;
	UINT16		uiLength = 0;
	UINT16		uiIndex = 0;
	UINT16		uiStart = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start CreateWSPHeader");
#endif

	*pliHeaderLength = 0;
	if( pvReqStruct != NULL )
		pUrlReq = (pURLREQSTRUCT)pvReqStruct;

	if( pvHeader != NULL ){
		pbHeader = (BYTE*)pvHeader;
		uiLength = (UINT16)iHeadLen;
	}
	
	if( uiLastMod != 0 ){
		uiLength += 6;
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
			uiLength++;
	}

	if( uiLength > 0 ){

		pbOctetString = NEWARRAY( BYTE, uiLength );

		if( pbOctetString != NULL ){
				/* Post */
			if( ucMethod == 0x60 ){
				if( pUrlReq != NULL && (pUrlReq->iContentCharSet != IANA_CHARSET_INVALID) ){
					if( pUrlReq->iContentCharSet < 256 ){
						pbOctetString[0] = 0x03; /* Content-Type Length */
						pbOctetString[1] = 0x92; /* application/x-form-urlencoded */
						pbOctetString[2] = 0x81; /* Parameter-charset */
						pbOctetString[3] = (BYTE)(pUrlReq->iContentCharSet | 0x80); 
						uiStart += 4;
						iHeadLen += 4;
					}
					else{
						pbOctetString[0] = 0x05; /* Content-Type Length */
						pbOctetString[1] = 0x92; /* application/x-form-urlencoded */
						pbOctetString[2] = 0x81; /* Parameter-charset */
						pbOctetString[3] = 0x02; /* Length uintvar */
						pbOctetString[4] = (pUrlReq->iContentCharSet >> 8) & 0xff;
						pbOctetString[5] = pUrlReq->iContentCharSet & 0xff;
						uiStart += 6;
						iHeadLen += 6;
					}
				}
				else{
					pbOctetString[0] = 0x92; /* application/x-form-urlencoded */
					uiStart++;
					iHeadLen++;
				}
			}
			if( pbHeader != NULL ){
				for ( uiIndex=uiStart; uiIndex<iHeadLen; uiIndex++ )
					pbOctetString[uiIndex] = pbHeader[uiIndex-uiStart];
			}
			if( uiLastMod != 0 ){
				pbOctetString[uiIndex] = 0x80 + Field_If_Modified_Since;
				uiIndex++;
				pbOctetString[uiIndex] = 0x04;
				uiIndex++;
				pbOctetString[uiIndex] = (BYTE)((uiLastMod >> 24) & 0xFF);
				uiIndex++;
				pbOctetString[uiIndex] = (BYTE)((uiLastMod >> 16) & 0xFF);
				uiIndex++;
				pbOctetString[uiIndex] = (BYTE)((uiLastMod >> 8) & 0xFF);
				uiIndex++;
				pbOctetString[uiIndex] = (BYTE)(uiLastMod & 0xFF);
				uiIndex++;
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_If_Modified_Since");
#endif
			}
			if( iReload ){
				pbOctetString[uiIndex] = 0x80 + Field_Cache_Control;
				uiIndex++;
					/* No-Cache */
				pbOctetString[uiIndex] = 0x80;
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Cache_Control");
#endif
			}
		}
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End CreateWSPHeader");
#endif

	*pliHeaderLength = uiLength;
	return (void*)pbOctetString;
} /* end of CreateWSPHeader */

/*========================================================================
	GetCacheControl 
==========================================================================
Purpose: Get the CacheControl value from the Header (MBM-Buffer)
Params:  pbuHeader	buffhead*	MBM-Buffer
Return:  cachecontrol value.
=========================================================================*/
unsigned char GetCacheControl( void* pHeaderHandle )
{
	pHEADERDEF		pHeaderList = NULL;
	pHEADERELEMENT	pHeaderElement = NULL;
	unsigned char	ucCacheControl = 0;
	BOOL			boError = FALSE;
	UINT32			uiDeltaSec = 0;
	int				iCachePrivate = cfg_wae_cc_cachePrivate;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetCacheControl");
#endif

	if( pHeaderHandle != NULL )
	{
		pHeaderList = (pHEADERDEF)pHeaderHandle;
		if( pHeaderList->pHeadList != NULL )
		{
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("pHeaderList->pHeadList != NULL");
#endif
			pHeaderElement = WSP_GetHeaderWK ( 1, Field_Cache_Control, pHeaderList->pHeadList );
			if( pHeaderElement != NULL )
			{
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("pHeaderElement != NULL");
#endif
				ucCacheControl = WSP_GetCacheControlByte( pHeaderElement, &uiDeltaSec, &boError );
					/* If the cache-directive is Private and it is allowed to cache Privat content,
							change the directive to Public.*/
				if( ucCacheControl == 135 && iCachePrivate == 1 )
					ucCacheControl = 134; /* Public */
			}
				/* If the header field Field_Vary exists, the response should not be stored in the cache */
			pHeaderElement = WSP_GetHeaderWK ( 1, Field_Vary, pHeaderList->pHeadList );
			if( pHeaderElement != NULL )
			{
				ucCacheControl = 0x80;
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Vary");
#endif
			}
		}
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetCacheControl");
#endif
	return (int)ucCacheControl;

} /* end of GetCacheControl */

/*========================================================================
	GetURLfromLocation 
==========================================================================
Purpose: Get the Location value from the Header (MBM-Buffer) and store the 
		 Location in pvUrl
Params:  pbuHeader		buffhead*	MBM-Buffer
		 pvUrl			BYTE**		casted to VOID**
		 liUrlLength	Long Int*	the length of pvUrl
Return:  TRUE if success, otherwise FALSE
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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetURLfromLocation");
#endif

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
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("pHeaderList != NULL");
#endif
				if( pHeaderList->pHeadList != NULL )
				{
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("pHeaderList->pHeadList != NULL");
#endif
					pHeaderElement = WSP_GetHeaderWK ( 1, Field_Location, pHeaderList->pHeadList );
					if( pHeaderElement != NULL )
					{
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("pHeaderElement != NULL");
#endif
						pbUrlLocation = WSP_GetLocation( pHeaderElement );
						if( pbUrlLocation != NULL ){
							uiUrlLength = B_STRINGLENGTH((const char *)pbUrlLocation );
							if( uiUrlLength > 0 ){
								if( b_Resolve( (BYTE*)(*pvUrl), pbUrlLocation, &pbResolvedUrl ) ){
									if( pbResolvedUrl != NULL ){
										uiUrlLength = B_STRINGLENGTH((const char *)pbResolvedUrl );
										DEALLOC( pvUrl );

										*pvUrl = (VOID*)pbResolvedUrl;
										*liUrlLength = (long int)uiUrlLength;
										DeleteHEADERDEF( pHeaderList );
	#ifdef WAE_DEBUG
										URL_DEBUG_PRINT("End GetURLfromLocation");
										URL_DEBUG_PRINT(pbResolvedUrl);
	#endif
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
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetURLfromLocation");
#endif
	return 0;

} /* end of GetURLfromLocation */

/*========================================================================
	GetHeaderHandle
==========================================================================
Purpose: Create a HeaderHandle from a Bytestream
Params:  pvHeader	BYTE*	casted to a VOID*
		 iHeadLen	Int		length of pvHeader
Return:  pHEADERDEF casted to a VOID*.
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

#ifdef WAE_DEBUG
	BYTE		pcTemp[200];
	int i=0;
	int j=0;
	int tal=0;

	URL_DEBUG_PRINT("Start GetHeaderHandle");
#endif

	*ppHeaderHandle = NULL;
	uiHeadLen = (UINT32)iHeadLen;
	if( pvHeader != NULL )
	{
		pbHeader = (BYTE*)pvHeader;
		if( pbHeader != NULL )
		{
#ifdef WAE_DEBUG
			for(i=0; i<iHeadLen; i++){
				tal = pbHeader[i]/16;
				if( tal < 10 )
					pcTemp[j] = (BYTE)(48 + tal);
				else
					pcTemp[j] = (BYTE)(55 + tal);
				j++;
				tal = pbHeader[i]%16;
				if( tal < 10 )
					pcTemp[j] = (BYTE)(48 + tal);
				else
					pcTemp[j] = (BYTE)(55 + tal);
				j++;
			}
			pcTemp[j] = 0;
			URL_DEBUG_PRINT("Header:");
			URL_DEBUG_PRINT(pcTemp);
#endif
			pHeaderList = WSP_PreParseHeaders ( pbHeader, uiHeadLen, &boError );
			if( pHeaderList != NULL && boError == FALSE ){
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("pHeaderList != NULL");
#endif
						/* Simulate ContetBase */
				if( iRedirect != 0 && pvUrl != NULL ){
					iUrlLength = B_STRINGLENGTH( (const char *)pvUrl );
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
#ifdef WAE_DEBUG
										URL_DEBUG_PRINT("Add ContentBase");
#endif
									}
								}
							}
						}
					}
				}
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("End GetHeaderHandle");
#endif
				*ppHeaderHandle = (void*)pHeaderList;
				iReturn = 1;
			}
			else{
				if( pHeaderList != NULL )
					DeleteHEADERDEF( pHeaderList );
			}
		}
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End GetHeaderHandle");
#endif
	return iReturn;

} /* end of GetHeaderHandle */

/*========================================================================
	FreeVoidStar 
==========================================================================
Purpose: Deallocate a pointer
Params:  pvBuffer	VOID**
Return:  
=========================================================================*/
VOID FreeVoidStar( VOID** pvBuffer )
{

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start FreeVoidStar");
#endif

	DEALLOC( pvBuffer );

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End FreeVoidStar");
#endif

} /* end of FreeVoidStar */

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getAddrBearer");
#endif

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
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
Return:  SecondAddressFormat
=========================================================================*/
unsigned char getAddrSecAddrFormat( VOID* pNetStruct, int iServerSet )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getAddrSecAddrFormat");
#endif

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet && (pNetworkStruct->iActiveBearer==BEARER_USSD) )
			return (unsigned char)pNetworkStruct->iUSSD_SC_TYPE;
	}
	return 0;
} /* end of getAddrSecAddrFormat */

/*========================================================================
	getAddrAddress 
==========================================================================
Purpose: Create a OctetString with the Address part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 liLength		Long Int*		length of the OctetString
Return:  The created OctetString
=========================================================================*/
void getAddrAddress( VOID* pNetStruct, int iServerSet, DeviceAddress* pAddress )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getAddrAddress");
#endif

	if( pNetStruct != NULL && pAddress != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet ){
			switch( pNetworkStruct->iActiveBearer )
			{
				case BEARER_UDP:     
					if( pNetworkStruct->pbUDP_GW_IP != NULL ){
						devaddr_store( pAddress, pNetworkStruct->pbUDP_GW_IP, 
							pNetworkStruct->pbUDP_GW_IP_LEN );
					}
					break;
				case BEARER_USSD:    
					if( pNetworkStruct->pbUSSD_CENTER != NULL ){
						devaddr_store( pAddress, pNetworkStruct->pbUSSD_CENTER, 
							pNetworkStruct->pbUSSD_CENTER_LEN );
					}
					break;
				case BEARER_SMS:     
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
				case BEARER_UDP:     /* UDP set client IP */
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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End getAddrAddress");
#endif

} /* end of getAddrAddress */

/*========================================================================
	getAddrSecondAddress 
==========================================================================
Purpose: Create a OctetString with the SecondAddress part from the NETWORKSTRUCT
Params:  pNetStruct		NETWORKSTRUCT*	casted to VOID*,
		 iServerSet		Int				1 (Serverpart), 0 (Clientpart)
		 liLength		Long Int*		length of the OctetString
Return:  The created OctetString
=========================================================================*/
void getAddrSecondAddress( VOID* pNetStruct, int iServerSet, AuxiliaryAddress* pSecondAddress )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getAddrSecondAddress");
#endif

	if( pNetStruct != NULL && pSecondAddress != NULL  ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
		if( iServerSet ){
			switch( pNetworkStruct->iActiveBearer )
			{
				case BEARER_USSD:    
					if( pNetworkStruct->pbUSSD_SC != NULL ){
						auxaddr_store( pSecondAddress, pNetworkStruct->pbUSSD_SC, 
							pNetworkStruct->pbUSSD_SC_LEN );
					}
					break;
				case BEARER_SMS:     
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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End getAddrSecondAddress");
#endif

} /* end of getAddrSecondAddress */

/*========================================================================
	getTimerValue 
==========================================================================
Purpose: Get the TimeOut part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  TimeOut
=========================================================================*/
int getTimerValue( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getTimerValue");
#endif

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;

		return ((pNetworkStruct->iTimeOut)*10);
	}
	return cfg_wae_ua_defaultRequestTimeout * 10;
} /* end of getTimerValue */

/*========================================================================
	getStackMode 
==========================================================================
Purpose: Get the StackMode part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  StackMode
=========================================================================*/
int getStackMode( VOID* pNetStruct )
{
	NETWORKSTRUCT* pNetworkStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getStackMode");
#endif

	if( pNetStruct != NULL ){
		pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;

		return pNetworkStruct->iStackMode;
	}
	return 0;
} /* end of getStackMode */

/*========================================================================
	getOTAMode 
==========================================================================
Purpose: Get the OTAIsAllowed part from the WAEMAINOBJECT
Params:  pvWAEMaStruct	WAEMAINOBJECT*	casted to VOID*
Return:  OTAIsAllowed
=========================================================================*/
int getOTAMode( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getOTAMode");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		return pWAEMainStruct->iOTAIsAllowed;
	}
	return 1;
} /* end of getOTAMode */

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start GetFile");
#endif

	if( *pvRequestStruct != NULL ){
		pUrlReq = (pURLREQSTRUCT)(*pvRequestStruct);
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL ){
/*  Modified by GBU,0001014 */
/*
			pbFrag = B_STRSTRING( pUrlReq->pbURL , (BYTE*)"#" );
*/
            pbFrag =  (BYTE *)strstr((const char *)pUrlReq->pbURL, "#" );
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
Purpose: 
Params:  
Return:  
=========================================================================*/
int checkIfImageRequest( void* pvReqStruct )
{
	pURLREQSTRUCT	pUrlReq = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start checkIfImageRequest");
#endif

	if( pvReqStruct != NULL ){
		pUrlReq = (pURLREQSTRUCT)(pvReqStruct);
		if( pUrlReq != NULL && pUrlReq->pbAccept != NULL ){
/*  Modified by GBU,0001014 */
/*
			if( B_STRSTRING( pUrlReq->pbAccept , (BYTE*)"image" ) != NULL )
*/
            if ( strstr((const char *)pUrlReq->pbAccept, "image" ))
				return 1;
		}
	}
	return 0;
} /* end of checkIfImageRequest */

/*========================================================================
	changeSessionParam
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int changeSessionParam( VOID* pvOldNetwork, VOID* pvNewNetwork, 
	VOID* pvOldUaSettings, VOID* pvNewUaSettings )
{
	NETWORKSTRUCT*	pOldNetwork = NULL;
	NETWORKSTRUCT*	pNewNetwork = NULL;
	UASETTINGS*		pOldUaSet = NULL;
	UASETTINGS*		pNewUaSet = NULL;
	int iReturn = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start changeSessionParam");
#endif

	if( pvOldNetwork != NULL ){
		pOldNetwork = (NETWORKSTRUCT*)pvOldNetwork;
		if( pvNewNetwork != NULL ){
			pNewNetwork = (NETWORKSTRUCT*)pvNewNetwork;
			if( pNewNetwork->iStackMode != 0 && pNewNetwork->iStackMode != pOldNetwork->iStackMode ){
				pOldNetwork->iStackMode = pNewNetwork->iStackMode;
				iReturn = 1;
			}
			if( pNewNetwork->iActiveBearer != -1 && pOldNetwork->iActiveBearer != pNewNetwork->iActiveBearer ){
				pOldNetwork->iActiveBearer = pNewNetwork->iActiveBearer;
				iReturn = 1;
			}
			if( pNewNetwork->pbUDP_CLIENT_IP != NULL && 
					(pOldNetwork->pbUDP_CLIENT_IP==NULL || 
					(pNewNetwork->pbUDP_CLIENT_IP_LEN!=pOldNetwork->pbUDP_CLIENT_IP_LEN) ||
					(strncmp((const char *)pNewNetwork->pbUDP_CLIENT_IP,(const char *)pOldNetwork->pbUDP_CLIENT_IP,pNewNetwork->pbUDP_CLIENT_IP_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbUDP_CLIENT_IP) );
				pOldNetwork->pbUDP_CLIENT_IP = NEWARRAY(BYTE,pNewNetwork->pbUDP_CLIENT_IP_LEN);
				B_COPYSTRINGN( pOldNetwork->pbUDP_CLIENT_IP, pNewNetwork->pbUDP_CLIENT_IP, pNewNetwork->pbUDP_CLIENT_IP_LEN );
				pOldNetwork->pbUDP_CLIENT_IP_LEN = pNewNetwork->pbUDP_CLIENT_IP_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->pbUDP_GW_IP != NULL && 
					(pOldNetwork->pbUDP_GW_IP==NULL || 
					(pNewNetwork->pbUDP_GW_IP_LEN!=pOldNetwork->pbUDP_GW_IP_LEN) ||
					(strncmp((const char *)pNewNetwork->pbUDP_GW_IP,(const char *)pOldNetwork->pbUDP_GW_IP,pNewNetwork->pbUDP_GW_IP_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbUDP_GW_IP) );
				pOldNetwork->pbUDP_GW_IP = NEWARRAY(BYTE,pNewNetwork->pbUDP_GW_IP_LEN);
				B_COPYSTRINGN( pOldNetwork->pbUDP_GW_IP, pNewNetwork->pbUDP_GW_IP, pNewNetwork->pbUDP_GW_IP_LEN );
				pOldNetwork->pbUDP_GW_IP_LEN = pNewNetwork->pbUDP_GW_IP_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->pbSMS_CENTER != NULL && 
					(pOldNetwork->pbSMS_CENTER==NULL || 
					(pNewNetwork->pbSMS_CENTER_LEN!=pOldNetwork->pbSMS_CENTER_LEN) ||
					(strncmp((const char *)pNewNetwork->pbSMS_CENTER,(const char *)pOldNetwork->pbSMS_CENTER,pNewNetwork->pbSMS_CENTER_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbSMS_CENTER) );
				pOldNetwork->pbSMS_CENTER = NEWARRAY(BYTE,pNewNetwork->pbSMS_CENTER_LEN);
				B_COPYSTRINGN( pOldNetwork->pbSMS_CENTER, pNewNetwork->pbSMS_CENTER, pNewNetwork->pbSMS_CENTER_LEN );
				pOldNetwork->pbSMS_CENTER_LEN = pNewNetwork->pbSMS_CENTER_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->pbSMS_GW != NULL && 
					(pOldNetwork->pbSMS_GW==NULL || 
					(pNewNetwork->pbSMS_GW_LEN!=pOldNetwork->pbSMS_GW_LEN) ||
					(strncmp((const char *)pNewNetwork->pbSMS_GW,(const char *)pOldNetwork->pbSMS_GW,pNewNetwork->pbSMS_GW_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbSMS_GW) );
				pOldNetwork->pbSMS_GW = NEWARRAY(BYTE,pNewNetwork->pbSMS_GW_LEN);
				B_COPYSTRINGN( pOldNetwork->pbSMS_GW, pNewNetwork->pbSMS_GW, pNewNetwork->pbSMS_GW_LEN );
				pOldNetwork->pbSMS_GW_LEN = pNewNetwork->pbSMS_GW_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->pbUSSD_CENTER != NULL && 
					(pOldNetwork->pbUSSD_CENTER==NULL || 
					(pNewNetwork->pbUSSD_CENTER_LEN!=pOldNetwork->pbUSSD_CENTER_LEN) ||
					(strncmp((const char *)pNewNetwork->pbUSSD_CENTER,(const char *)pOldNetwork->pbUSSD_CENTER,pNewNetwork->pbUSSD_CENTER_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbUSSD_CENTER) );
				pOldNetwork->pbUSSD_CENTER = NEWARRAY(BYTE,pNewNetwork->pbUSSD_CENTER_LEN);
				B_COPYSTRINGN( pOldNetwork->pbUSSD_CENTER, pNewNetwork->pbUSSD_CENTER, pNewNetwork->pbUSSD_CENTER_LEN );
				pOldNetwork->pbUSSD_CENTER_LEN = pNewNetwork->pbUSSD_CENTER_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->iUSSD_SC_TYPE != -1 && pOldNetwork->iUSSD_SC_TYPE != pNewNetwork->iUSSD_SC_TYPE ){
				pOldNetwork->iUSSD_SC_TYPE = pNewNetwork->iUSSD_SC_TYPE;
				iReturn = 1;
			}
			if( pNewNetwork->pbUSSD_SC != NULL && 
					(pOldNetwork->pbUSSD_SC==NULL || 
					(pNewNetwork->pbUSSD_SC_LEN!=pOldNetwork->pbUSSD_SC_LEN) ||
					(strncmp((const char *)pNewNetwork->pbUSSD_SC,(const char *)pOldNetwork->pbUSSD_SC,pNewNetwork->pbUSSD_SC_LEN)!=0) ) ){
				DEALLOC( &(pOldNetwork->pbUSSD_SC) );
				pOldNetwork->pbUSSD_SC = NEWARRAY(BYTE,pNewNetwork->pbUSSD_SC_LEN);
				B_COPYSTRINGN( pOldNetwork->pbUSSD_SC, pNewNetwork->pbUSSD_SC, pNewNetwork->pbUSSD_SC_LEN );
				pOldNetwork->pbUSSD_SC_LEN = pNewNetwork->pbUSSD_SC_LEN;
				iReturn = 1;
			}
			if( pNewNetwork->iTimeOut != TIMEOUT_VALUE_INVALID )
				pOldNetwork->iTimeOut = pNewNetwork->iTimeOut;
		}
	}

	if( pvOldUaSettings != NULL ){
		pOldUaSet = (UASETTINGS*)pvOldUaSettings;
		if( pvNewUaSettings != NULL ){
			pNewUaSet = (UASETTINGS*)pvNewUaSettings;
			if( pNewUaSet->pbAccept != NULL && 
					(pOldUaSet->pbAccept==NULL || 
					(B_COMPARESTRING((const char *)pNewUaSet->pbAccept,(const char *)pOldUaSet->pbAccept)!=0) ) ){
				DEALLOC( &(pOldUaSet->pbAccept) );
				pOldUaSet->pbAccept = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pNewUaSet->pbAccept) + 1);
				B_COPYSTRING( (char *)pOldUaSet->pbAccept,(const char *)pNewUaSet->pbAccept );
				iReturn = 1;
			}
			if( pNewUaSet->pbProxyCredentials != NULL && 
					(pOldUaSet->pbProxyCredentials==NULL || 
					(pNewUaSet->iProxyCredentialsLength!=pOldUaSet->iProxyCredentialsLength) ||
					(strncmp((const char *)pNewUaSet->pbProxyCredentials,(const char *)pOldUaSet->pbProxyCredentials,pNewUaSet->iProxyCredentialsLength)!=0) ) ){
				DEALLOC( &(pOldUaSet->pbProxyCredentials) );
				pOldUaSet->pbProxyCredentials = NEWARRAY(BYTE,pNewUaSet->iProxyCredentialsLength);
				B_COPYSTRINGN( pOldUaSet->pbProxyCredentials, pNewUaSet->pbProxyCredentials, pNewUaSet->iProxyCredentialsLength );
				pOldUaSet->iProxyCredentialsLength = pNewUaSet->iProxyCredentialsLength;
				iReturn = 1;
			}
			if( pNewUaSet->pbLanguage != NULL && 
					(pOldUaSet->pbLanguage==NULL || 
					(pNewUaSet->iLanguage_Length!=pOldUaSet->iLanguage_Length) ||
					(strncmp((const char *)pNewUaSet->pbLanguage,(const char *)pOldUaSet->pbLanguage,pNewUaSet->iLanguage_Length)!=0) ) ){
				DEALLOC( &(pOldUaSet->pbLanguage) );
				pOldUaSet->pbLanguage = NEWARRAY(BYTE,pNewUaSet->iLanguage_Length);
				B_COPYSTRINGN( pOldUaSet->pbLanguage, pNewUaSet->pbLanguage, pNewUaSet->iLanguage_Length );
				pOldUaSet->iLanguage_Length = pNewUaSet->iLanguage_Length;
				iReturn = 1;
			}
		}
	}
	return iReturn;
} /* end of changeSessionParam */

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getNrOfFreeReqstruct");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start InitNrOfFreeTrProc");
#endif

	if( pvWAEMainObject != NULL )
	{
		pWaeObject = (pWAEMAINOBJECT)pvWAEMainObject;
		if( pWaeObject != NULL )
			pWaeObject->iNrOfFreeRequestProc = iMaxTrProc;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End InitNrOfFreeTrProc");
#endif
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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start decNrOfFreeReqstruct");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeRequestProc)--;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End decNrOfFreeReqstruct");
#endif

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

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start incNrOfFreeReqstruct");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeRequestProc)++;
	}
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End incNrOfFreeReqstruct");
#endif

} /* end of incNrOfFreeReqstruct */

/*========================================================================
	getNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int getNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start getNrOfFreeSession");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		return pWAEMainStruct->iNrOfFreeSessionProc;
	}
	return 0;
} /* end of getNrOfFreeSession */

/*========================================================================
	InitNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void InitNrOfFreeSession( VOID* pvWAEMainObject, int iMaxSessionProc )
{
	pWAEMAINOBJECT	pWaeObject = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start InitNrOfFreeSession");
#endif

	if( pvWAEMainObject != NULL )
	{
		pWaeObject = (pWAEMAINOBJECT)pvWAEMainObject;
		if( pWaeObject != NULL )
			pWaeObject->iNrOfFreeSessionProc = iMaxSessionProc;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End InitNrOfFreeSession");
#endif
}  /* end of InitNrOfFreeSession */

/*========================================================================
	decNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void decNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start decNrOfFreeSession");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeSessionProc)--;
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End decNrOfFreeSession");
#endif

} /* end of decNrOfFreeSession */

/*========================================================================
	InitNrOfFreeSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void incNrOfFreeSession( VOID* pvWAEMaStruct )
{
	pWAEMAINOBJECT pWAEMainStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start incNrOfFreeSession");
#endif

	if( pvWAEMaStruct != NULL ){
		pWAEMainStruct = (pWAEMAINOBJECT)pvWAEMaStruct;

		(pWAEMainStruct->iNrOfFreeSessionProc)++;
	}
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End incNrOfFreeSession");
#endif

} /* end of incNrOfFreeSession */






/*========================================================================
	pidList_AddPid
=========================================================================*/
int pidList_AddPid( void* pList, int iId, unsigned long int uiPid )
{
	PIDSTRUCT* pPidStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_AddPid");
#endif

	if( pList != NULL ){
		pPidStruct = NEWSTRUCT(PIDSTRUCT);

		if( pPidStruct != NULL ){
			pPidStruct->uiPid = uiPid;
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Add ListElement");
#endif
			return SDL_AddListElement( pList, iId, 0, (void*)pPidStruct );

		}
	}
	return 0;
	
} /* end of pidList_AddPid */

/*========================================================================
	pidList_AddPid_Resend
=========================================================================*/
int pidList_AddPid_Resend( void* pList, int iId, unsigned long int uiPid, int iResend )
{
	PIDSTRUCT*	pPidStruct = NULL;
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	int			iFound = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_AddPid_Resend");
#endif

	if( pList != NULL ){
		if( iResend ){
			pvListElement = SDL_GetNextListElement( pList, NULL );
			while( pvListElement != NULL ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
						if( pPidStruct->uiPid == uiPid ){
							iFound = 1;
#ifdef WAE_DEBUG
								URL_DEBUG_PRINT("It is a real ReSend");
#endif
							break;
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
	
} /* end of pidList_AddPid_Resend */

/*========================================================================
	pidList_SetSessionID 
=========================================================================*/
int pidList_SetSessionID( void* pList, int iId )
{
	void*		pvListElement = NULL;
#ifdef WAE_DEBUG
	int			iCounter = 1;
	char		stTemp[10];
	URL_DEBUG_PRINT("Start pidList_SetSessionID");
#endif

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) == 0 ){
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("Set Id in Entry number:");
					_itoa( iCounter, stTemp, 10 );
					URL_DEBUG_PRINT(stTemp);
#endif
				SDL_ChangeListElementID( pvListElement, iId );
				return 1;
			}
			else{
#ifdef WAE_DEBUG
				iCounter++;
#endif
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
			}
		}
	}
	return 0;
	
} /* end of pidList_SetSessionID */

/*========================================================================
	pidList_ClearSessionID 
=========================================================================*/
int pidList_ClearSessionID( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	int			iID = 0;
	PIDSTRUCT*	pPidStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_ClearSessionID");
#endif

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("Pid found");
#endif
					iID = SDL_GetListElementID( pvListElement );
					SDL_ChangeListElementID( pvListElement, 0 );
					return iID;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
} /* end of pidList_ClearSessionID */

/*========================================================================
	pidList_GetPid 
=========================================================================*/
unsigned long int pidList_GetPid( void* pList, int iId, int iRemove )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int	uiPID = 0;
	PIDSTRUCT*	pPidStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_GetPid");
#endif

	if( pList != NULL ){
		pvListElement = SDL_SearchListElement( pList, iId, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL ){
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("Id found");
#endif
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
unsigned long int pidList_GetPid_Resend( void* pList, int iId )
{
	void*		pvListElement = NULL;
	void*		pvPrevListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int uiPid = 0;
	PIDSTRUCT*	pPidStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_GetPid_Resend");
#endif

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
#ifdef WAE_DEBUG
								URL_DEBUG_PRINT("Delete Extra PidStruct");
#endif
							pvPrevListElement = pvListElement;
						}
					}
				}
				pvListElement = SDL_GetNextListElement( pList, pvListElement );
				if( pvPrevListElement != NULL ){
					SDL_DeleteListElement( pList, pvPrevListElement );
					pvPrevListElement = NULL;
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("Delete Extra ListElement");
#endif
				}
			}
		}
	}
	return uiPid;
	
} /* end of pidList_GetPid_Resend */


/*========================================================================
	pidList_RemovePid 
=========================================================================*/
int pidList_RemovePid( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_RemovePid");
#endif

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (PIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("Pid found");
#endif
					DEALLOC( &pPidStruct );
					SDL_DeleteListElement( pList, pvListElement );
					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
} /* end of pidList_RemovePid */

/*========================================================================
	pidList_GetFirstPid 
=========================================================================*/
unsigned long int pidList_GetFirstPid( void* pList )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_GetFirstPid");
#endif

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) != 0 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
#ifdef WAE_DEBUG
							URL_DEBUG_PRINT("Id found");
#endif
						return pPidStruct->uiPid;
					}
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
} /* end of pidList_GetFirstPid */

/*========================================================================
	pidList_GetPidFromElement 
=========================================================================*/
unsigned long int pidList_GetPidFromElement( void* pList, void** ppListElement )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	PIDSTRUCT*	pPidStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_GetPidFromElement");
#endif

	if( pList != NULL ){
		pvListElement = *ppListElement;
		pvListElement = SDL_GetNextListElement( pList, pvListElement );
		while( pvListElement != NULL ){
			if( SDL_GetListElementID( pvListElement ) != 0 ){
				pvElementContent = SDL_GetListElementContent( pvListElement );
				if( pvElementContent != NULL ){
					pPidStruct = (PIDSTRUCT*)pvElementContent;
					if( pPidStruct != NULL ){
#ifdef WAE_DEBUG
						URL_DEBUG_PRINT("Id found");
#endif
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
	
} /* end of pidList_GetPidFromElement */

/*========================================================================
	pidList_Delete 
=========================================================================*/
int pidList_Delete( void** pList )
{
	PIDSTRUCT*			pPidStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start pidList_Delete");
#endif

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
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End pidList_Delete");
#endif

	return 1;
} /* end of pidList_Delete */

/*========================================================================
	releaseExtraTids 
=========================================================================*/
void releaseExtraTids( void* pvWAEMaStruct, void* pList )
{
	void*		pvListElement = NULL;
	int			iTID = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start releaseExtraTids");
#endif

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

} /* end of releaseExtraTids */

/*========================================================================
	TR_List_Init 
==========================================================================
Purpose: Initialize the Transaction-queue
Params:  iSize	Int		the max number of element in queue, if NULL, no limit.
Return:  pointer to the queue
=========================================================================*/
void* TR_List_Init( int iSize )
{
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_Init");
#endif

	return SDL_NewList( iSize );
} /* end of TR_List_Init */

/*========================================================================
	TR_List_Count 
==========================================================================
Purpose: Count how many element there is in the Transaction-queue
Params:  pvTRList	void*	pointer to the queue
Return:  number of elements
=========================================================================*/
int TR_List_Count( void* pvTRList )
{
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_Count");
#endif

	return SDL_GetCardinal( pvTRList );
} /* end of TR_List_Count */

/*========================================================================
	TR_List_Add 
==========================================================================
Purpose: Insert (priority, highest first) a new element in the Transaction-queue
Params:  pvTransList	void*	pointer to the queue
		 iTID			Int
		 pvReqStruct	void*	pointer to a URLREQSTRUCT
		 iReload		Int
		 iPrio			Int		Priority
		 pvNetStruct	void*	pointer to a NETWORKSTRUCT
		 iStackMode		Int
		 iPid			Int		index mapped to a SDL-Pid
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_Add( void* pvTransList, int iSID, int iTID, void* pvReqStruct, 
			int iReload, int iPrio, unsigned long int uiPid )
{
	TRANSACTIONSTRUCT* pTransStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_Add");
#endif

	if( pvTransList != NULL ){
		pTransStruct = NEWSTRUCT(TRANSACTIONSTRUCT);

		if( pTransStruct != NULL ){
			pTransStruct->iSID = iSID;
			pTransStruct->iTID = iTID;
			pTransStruct->pvReqStruct = pvReqStruct;
			pTransStruct->iReload = iReload;
			pTransStruct->iPrio = iPrio;
			pTransStruct->uiPid = uiPid;

			return SDL_AddListElement( pvTransList, iTID, iPrio, (void*)pTransStruct );

		}
	}
	return 0;
	
} /* end of TR_List_Add */

/*========================================================================
	TR_List_GetNext 
==========================================================================
Purpose: Get the next element from the Transaction-queue and delete this
			element from the queue.
Params:  pvTransList	void*		pointer to the queue
		 liTID			Long Int*
		 pvReqStruct	void**		pointer to a URLREQSTRUCT
		 liReload		Long Int*
		 liPrio			Long Int*	Priority
		 pvNetStruct	void**		pointer to a NETWORKSTRUCT
		 liStackMode	Long Int*
		 liPid			Long Int*	index mapped to a SDL-Pid
Return:  TRUE if success, otherwise FALSE
=========================================================================*/
int TR_List_GetNext( void* pvTransList, long int* liSID, long int* liTID, 
	void** pvReqStruct, long int* liReload,	long int* liPrio, unsigned long int* uiPid )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_GetNext");
#endif

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					*liSID = (long int)pTransStruct->iSID;
					*liTID = (long int)pTransStruct->iTID;
					*pvReqStruct = pTransStruct->pvReqStruct;
					*liReload = (long int)pTransStruct->iReload;
					*liPrio = (long int)pTransStruct->iPrio;
					*uiPid = pTransStruct->uiPid;

					SDL_DeleteListElement( pvTransList, pvListElement );
					DEALLOC( &pTransStruct );

					return 1;
				}
			}
			SDL_DeleteListElement( pvTransList, pvListElement );
		}
	}
	return 0;
} /* end of TR_List_GetNext */

/*========================================================================
	TR_List_GetNextSIDTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int TR_List_GetNextSIDTrans( void* pvTransList, int iSID, long int* liTID, 
	void** pvReqStruct, long int* liReload,	long int* liPrio, unsigned long int* uiPid )
{
	TRANSACTIONSTRUCT*	pTransStruct = NULL;
	void*				pvPrevElement = NULL;
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_GetNextSIDTrans");
#endif

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iSID == iSID ){
						*liTID = (long int)pTransStruct->iTID;
						*pvReqStruct = pTransStruct->pvReqStruct;
						*liReload = (long int)pTransStruct->iReload;
						*liPrio = (long int)pTransStruct->iPrio;
						*uiPid = pTransStruct->uiPid;

						SDL_DeleteListElement( pvTransList, pvListElement );
						DEALLOC( &pTransStruct );
						return 1;
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
	return 0;
} /* end of TR_List_GetNextSIDTrans */

/*========================================================================
	TR_List_DeleteTransactions
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int TR_List_DeleteTransactions( void* pvTransList, int iSID )
{
	TRANSACTIONSTRUCT* pTransStruct = NULL;
	void*				pvListElement = NULL;
	void*				pvPrevElement = NULL;
	void*				pvElementContenet = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_DeleteTransactions");
#endif

	if( pvTransList != NULL ){
		pvListElement = SDL_GetNextListElement( pvTransList, pvPrevElement );
		while( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pTransStruct = (TRANSACTIONSTRUCT*)pvElementContenet;
				if( pTransStruct != NULL ){
					if( pTransStruct->iSID == iSID ){
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
	
} /* end of TR_List_DeleteTransactions */

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
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start TR_List_Delete");
#endif

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
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End TR_List_Delete");
#endif

	return 1;
} /* end of TR_List_Delete */

/*========================================================================
	SE_List_Init
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* SE_List_Init( int iSize )
{
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_Init");
#endif

	return SDL_NewList( iSize );
} /* end of SE_List_Init */

/*========================================================================
	SE_List_Add
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_Add( void* pvSessionList, int iSID, int iMaxNrOfTrans )
{
	SESSIONSTRUCT* pSessionStruct = NULL;
#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_Add");
#endif

	if( pvSessionList != NULL ){
		pSessionStruct = NEWSTRUCT(SESSIONSTRUCT);

		if( pSessionStruct != NULL ){
			pSessionStruct->iConnected = 0;
			pSessionStruct->iNrOfFreeTransaction = iMaxNrOfTrans;

			return SDL_AddElementLast(pvSessionList, iSID, (void*)pSessionStruct );
		}
	}
	return 0;
	
} /* end of SE_List_Add */

/*========================================================================
	SE_List_CheckConnection
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_CheckConnection( void* pvSessionList, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_CheckConnection");
#endif

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				if( pSessionStruct->iConnected == 1 )
					return 1;
				else
					return 0;
			}
		}
	}
	return 0;
	
} /* end of SE_List_CheckConnection */

/*========================================================================
	SE_List_CheckNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_CheckNrOfFreeTrans( void* pvSessionList, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_CheckNrOfFreeTrans");
#endif

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				if( pSessionStruct->iNrOfFreeTransaction > 0 )
					return 1;
				else
					return 0;
			}
		}
	}
	return 0;
	
} /* end of SE_List_CheckNrOfFreeTrans */

/*========================================================================
	SE_List_ChangeConnection
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_ChangeConnection( void* pvSessionList, int iSID, int iConnected )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_ChangeConnection");
#endif

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				pSessionStruct->iConnected = iConnected;

				return 1;
			}
		}
	}
	return 0;
	
} /* end of SE_List_ChangeConnection */

/*========================================================================
	SE_List_ChangeNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_ChangeNrOfFreeTrans( void* pvSessionList, int iSID, int iIncDec )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_ChangeNrOfFreeTrans");
#endif

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				pSessionStruct->iNrOfFreeTransaction = pSessionStruct->iNrOfFreeTransaction + iIncDec;

				return 1;
			}
		}
	}
	return 0;
	
} /* end of SE_List_ChangeNrOfFreeTrans */

/*========================================================================
	SE_List_SetNrOfFreeTrans
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_SetNrOfFreeTrans( void* pvSessionList, int iSID, int iMaxNrOfTrans )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_SetNrOfFreeTrans");
#endif

	if( pvSessionList != NULL ){
		pvListElement = SDL_SearchListElement( pvSessionList, iSID, NULL );
		if( pvListElement != NULL ){
			pvElementContenet = SDL_GetListElementContent( pvListElement );
			if( pvElementContenet != NULL ){
				pSessionStruct = (SESSIONSTRUCT*)pvElementContenet;
				pSessionStruct->iNrOfFreeTransaction = iMaxNrOfTrans;

				return 1;
			}
		}
	}
	return 0;
	
} /* end of SE_List_SetNrOfFreeTrans */

/*========================================================================
	SE_List_DeleteSession
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_DeleteSession( void* pvSessionList, int iSID )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_DeleteSession");
#endif

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
	
} /* end of SE_List_DeleteSession */

/*========================================================================
	SE_List_Delete
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int SE_List_Delete( void** pvSessionList )
{
	void*				pvListElement = NULL;
	void*				pvElementContenet = NULL;
	SESSIONSTRUCT*		pSessionStruct = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start SE_List_Delete");
#endif

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






/*========================================================================
				FUNCTIONS ONLY USED IN CONNECTIONORIENTED
=========================================================================*/

#ifdef CONFIG_CO

/*========================================================================
	CreateConnectHeader
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateConnectHeader( VOID* pvStruct, long int* pliHeaderLength )
{
	pUASETTINGS	pUaSettings = NULL;
	pHEADBUILDSTR	pList = NULL;
	pHEADBUILDSTR	pStartList = NULL;
	BYTE*		pbHeader = NULL;
	BYTE*		pbString = NULL;
	BYTE*		pbTemp = NULL;
	BYTE*		pbSeparator = NULL;
	UINT32		uiLength = 0;
	UINT16		i = 0;
	BYTE		tempArray[2];

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start CreateConnectHeader");
#endif

	*pliHeaderLength = 0;

	if( pvStruct != NULL ){
		pUaSettings = (pUASETTINGS)pvStruct;
		if( pUaSettings != NULL ){
			if( pUaSettings->pbAccept != NULL ){
				uiLength = B_STRINGLENGTH((const char *)pUaSettings->pbAccept );
				pbString = NEWARRAY(BYTE,uiLength+1);
				if( pbString != NULL ){
					B_COPYSTRING((char *)pbString,(const char *)pUaSettings->pbAccept );
					RemoveSpace( pbString );
					uiLength = B_STRINGLENGTH((const char *)pbString );
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("Field_Accept");
					URL_DEBUG_PRINT(pbString);
#endif

					pbTemp = pbString;
					pbSeparator = FindSegment( pbTemp );
					while( pbSeparator != NULL )
					{
						*pbSeparator = 0;
						pbSeparator++;
						uiLength = B_STRINGLENGTH((const char *)pbTemp );
						WSP_CreateHeaderField( Field_Accept, pbTemp, (uiLength+1), &pList);
						if( pStartList == NULL )
							pStartList = pList;
						pbTemp = pbSeparator;
						pbSeparator = FindSegment( pbTemp );
					}
					uiLength = B_STRINGLENGTH((const char *)pbTemp );
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
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("Field_Proxy_Authorization");
					URL_DEBUG_PRINT(pbString);
#endif
					DEALLOC( &pbString );
				}
			}

			if( pUaSettings->pbLanguage != NULL && pUaSettings->iLanguage_Length > 0 ){
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("Field_Accept_Language");
#endif
				for ( i=0; i<pUaSettings->iLanguage_Length; i++ ){
					WSP_CreateHeaderField( Field_Accept_Language, &(pUaSettings->pbLanguage[i]), 1, &pList);
					if( pStartList == NULL )
						pStartList = pList;
				}
			}
		}
	}

	if( NR_OF_ACCEPTCHARSET > 0 ){
#ifdef WAE_DEBUG
		URL_DEBUG_PRINT("Field_Accept_Charset");
#endif
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

#ifdef SKTSPEC
	{
		int templength = 0;
		MakeB5_info();
		templength = get_position((char *)agent+brow_length);
		agent[brow_length+templength] ='\x0';

		uiLength = B_STRINGLENGTH((const char *)agent);
	}
#else
	uiLength = B_STRINGLENGTH((const char *)HTTP_HEADER_USER_AGENT );	
#endif		
	
	if( uiLength > 0 )
	{
#ifdef SKTSPEC
		WSP_CreateHeaderField( Field_User_Agent, agent, (uiLength+1), &pList);
#else
		WSP_CreateHeaderField( Field_User_Agent, (BYTE*)HTTP_HEADER_USER_AGENT, (uiLength+1), &pList);
#endif
		if( pStartList == NULL )
			pStartList = pList;

	#ifdef WAE_DEBUG
			URL_DEBUG_PRINT("Field_User_Agent");
			URL_DEBUG_PRINT( HTTP_HEADER_USER_AGENT );
	#endif
	}

	if( pStartList != NULL ){
		uiLength = 0;
		pbHeader = WSP_BuildHeader( pStartList, &uiLength );
	}

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("End CreateConnectHeader");
#endif

	*pliHeaderLength = uiLength;
	return (void*)pbHeader;
} /* end of CreateConnectHeader */


/*========================================================================
	CreateCapabilities
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
void* CreateCapabilities( int iMaxNrOfTrans, long int* pliCapabilLen )
{
	BYTE*	pbCapabilities = NULL;

	/* Temp fix */
	BYTE* pbTemp = NEWARRAY(BYTE, 10);
	BYTE iLen = 0;
	BYTE iLen2 = 0;
	BYTE iIndex = 0;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start CreateCapabilities");
#endif
	
	*pliCapabilLen = 0;
	/*pbCapabilities = NEWARRAY( BYTE, 9 );*/

	
	/* Temp fix */
	iLen = (BYTE)IntToUintvar( MaxPDUsize, pbTemp );
	iLen2 = 7+(2*iLen);

#ifdef CONFIG_PUSH
	iLen2 += 6;
#endif

	pbCapabilities = NEWARRAY( BYTE, iLen2 );
	
	if( pbCapabilities != NULL ){
		pbCapabilities[0] = 0x02;					/* Capabilities length */
		pbCapabilities[1] = 0x83;					/* Capabilities field: Method-MOR */
		pbCapabilities[2] = (BYTE)iMaxNrOfTrans;	/* Capabilities value */

		
		/*pbCapabilities[3] = 0x02;*/					/* Capabilities length */
		pbCapabilities[3] = iLen+1;
		pbCapabilities[4] = 0x80;					/* Capabilities field: Client-SDU-Size */

		B_COPYSTRINGN( &pbCapabilities[5], pbTemp, iLen );
		
		/*pbCapabilities[5] = 0x00;*/					/* Capabilities value: No Limit */
		/*pbCapabilities[6] = 0x02;*/					/* Capabilities length */
		iIndex = 5 + iLen;
		pbCapabilities[iIndex] = iLen+1;
		iIndex++;
		/*pbCapabilities[7] = 0x81;*/					/* Capabilities field: Server-SDU-Size */
		pbCapabilities[iIndex] = 0x81;
		iIndex++;
		/*pbCapabilities[8] = 0x00;*/					/* Capabilities value: No Limit */
		B_COPYSTRINGN( &pbCapabilities[iIndex], pbTemp, iLen );

#ifdef CONFIG_PUSH
		iIndex += iLen;

		pbCapabilities[iIndex] = 0x02;					/* Capabilities length */
		iIndex++;
		pbCapabilities[iIndex] = 0x84;					/* Capabilities field: Method-PUSH */
		iIndex++;
		pbCapabilities[iIndex] = (BYTE)iMaxNrOfTrans;	/* Capabilities value */
		iIndex++;
		pbCapabilities[iIndex] = 0x02;					/* Capabilities length */
		iIndex++;
		pbCapabilities[iIndex] = 0x82;					/* Capabilities field: Protocol Options */
		iIndex++;
		pbCapabilities[iIndex] = (0x80 | 0x40);			/* Capabilities value: Confirmed Push, Push */
#endif

		/**pliCapabilLen = 9;*/

		*pliCapabilLen = iLen2;
	}

	DEALLOC(&pbTemp);

	return (void*)pbCapabilities;

} /* end of CreateCapabilities */

/*========================================================================
	ParseCapabilities
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int ParseCapabilities( VOID* pvNegotiatedCapabilities, int iLen)
{
	BYTE*				pbCapabil = NULL;
	pCAPABILITIESDEF	pCapabilList = NULL;
	BOOL				boError = FALSE;
	UINT32				uiMethodMor = 0;
	int					iReturn = -1;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ParseCapabilities");
#endif

	if( pvNegotiatedCapabilities != NULL && iLen > 0 ){
			/* Parsa bytestrˆmmen */
		pbCapabil = (BYTE*)pvNegotiatedCapabilities;
		pCapabilList = WSP_PreParseCapabilities( pbCapabil, iLen, &boError );
		if( pCapabilList != NULL && boError == FALSE )
		{
#ifdef WAE_DEBUG
			URL_DEBUG_PRINT("pCapabilList != NULL");
#endif
			if( pCapabilList->pList != NULL )
			{
#ifdef WAE_DEBUG
				URL_DEBUG_PRINT("pCapabilList->pList != NULL");
#endif
				uiMethodMor = WSP_GetCapabilityWK( METHOD_MOR, pCapabilList->pList, &boError );
				if( boError == FALSE )
				{
#ifdef WAE_DEBUG
					URL_DEBUG_PRINT("pHeaderElement != NULL");
#endif
					iReturn = (int)uiMethodMor;
				}
			}
		}
		if( pCapabilList != NULL ){
			WSP_DeleteCapabilitiesDef( &pCapabilList );
		}
		else{
			DEALLOC( &pvNegotiatedCapabilities );
		}
	}

	return iReturn;
} /* end of ParseCapabilities */


/*========================================================================
	ParseRedirectAddress
==========================================================================
Purpose: 
Params:  
Return:  
=========================================================================*/
int ParseRedirectAddress( void* pvRedirectAddress, int iAddressLength,
						 void* pNetStruct, unsigned long int* uiPort )
{
	BYTE*			pbAddress = NULL;
	NETWORKSTRUCT*	pNetworkStruct = NULL;
	ADDRESS*		pAddressList = NULL;

#ifdef WAE_DEBUG
	URL_DEBUG_PRINT("Start ParseRedirectAddress");
#endif

	*uiPort = 0;
	if( pvRedirectAddress != NULL && iAddressLength > 0 && pNetStruct != NULL ){
			/* Parsa bytestrˆmmen */
		pbAddress = (BYTE*)pvRedirectAddress;

		if( WSP_ParseAddress( pbAddress, iAddressLength, &pAddressList ) ){
			if( pAddressList != NULL ){
				pNetworkStruct = (NETWORKSTRUCT*)pNetStruct;
				
				if( pAddressList->iPortNumber != -1 )
					*uiPort = pAddressList->iPortNumber;

				if( pAddressList->iBearerType != -1 )
					pNetworkStruct->iActiveBearer = pAddressList->iBearerType;
				
				if( pAddressList->pbAddress != NULL && pAddressList->uiAddressLength > 0 ){
					switch( pNetworkStruct->iActiveBearer )
					{
						case BEARER_UDP:
							{
								if( pNetworkStruct->pbUDP_GW_IP != NULL )
									DEALLOC( &pNetworkStruct->pbUDP_GW_IP );
								pNetworkStruct->pbUDP_GW_IP = NEWARRAY( BYTE, pAddressList->uiAddressLength );
								if( pNetworkStruct->pbUDP_GW_IP != NULL ){
									B_COPYSTRINGN( pNetworkStruct->pbUDP_GW_IP, pAddressList->pbAddress, pAddressList->uiAddressLength );
									pNetworkStruct->pbUDP_GW_IP_LEN = pAddressList->uiAddressLength;
								}
							}
							break;
						case BEARER_USSD:
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
						case BEARER_SMS:
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

	return 0;
} /* end of ParseRedirectAddress */
#endif

/*========================================================================
				∏¡¿∏∑Œ ∫∏≥æ ¥‹∏ª±‚ µ•¿Ã≈∏∏¶ ±∏º∫«—¥Ÿ.
=========================================================================*/
#define TEMP_TEST   
extern BYTE SaveSelfMin[];
static void MakeMin(BYTE* pT)
{
   strncpy((char *)pT,(const char *)SaveSelfMin,8); 
}

#ifdef TEMP_TEST
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
		*pAgent++ = WAP;
	brow_length++;


	*pAgent++ = CELLULAR;
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
#else
static void MakeB5_info(void)
{
	BYTE* pAgent = agent;

	brow_length = 0;

//Carrier

	*pAgent++ = '0';
	brow_length++;

	strncpy(pAgent,CARRIER,2);
	pAgent += 2;
	brow_length+=2;

/*----------------------------------------------------------------
	P_REVø° µ˚∂Û Agent¿« ∞™¿Ã πŸ≤Ò(jjkyg 09/21) --> service type
 ----------------------------------------------------------------*/
	if(cdma.p_rev_in_use >=6)
		*pAgent++ = SKT95C;
	else
		*pAgent++ = WAP;

	brow_length++;
/*----------------------------------------------------------*/

//¥‹∏ª±‚ type
	*pAgent++ = CELLULAR;
	brow_length++;

//¡¶¡∂ªÁ
	*pAgent++ = GEGO;
	brow_length+=1;

//model num
	*pAgent++ = A10;
	brow_length+=1;

/*===============================================
			Service »Æ¿Â « µÂ
 ================================================*/
	*pAgent++= Phonehw;
	brow_length++;

	*pAgent++= WAPSPEC;
	brow_length++;

	*pAgent++= VSI;
	brow_length++;

	*pAgent++= HSI1;
	brow_length++;

	*pAgent++= HSI2;
	brow_length++;

	*pAgent++= HSI3;
	brow_length++;

/*================================================*/	
//lcd pixel
	strncpy(pAgent,(BYTE*)LCDPIXCEL,6);
	pAgent+=6;	
	brow_length+=6;

//lcd size	
	strncpy(pAgent,(BYTE*)LCDSIZE,4);
	pAgent+=4;	
	brow_length+=4;

//sound poly 	
	*pAgent++= SNDPOLY;
	brow_length++;

//Lcd color	
	*pAgent++= LCDCOLOR;
	brow_length+=1;

//Min	
	MakeMin(pAgent);
	brow_length+=8;	
}
#endif//#ifdef TEMP_TEST


