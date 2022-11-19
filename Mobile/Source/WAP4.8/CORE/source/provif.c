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

	File: provif.c

	Description:

	TBD

	Author: Kent Olsson, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	010313    KEOL   First version

==========================================================================*/

#include "provif.h"
#include "WAE_Cmmn.h"
#include "aapiclnt.h"
#include "URL.h"

#ifdef CONFIG_WTLS
#include "wtlsdef.h"
#endif

#define check_ProxyID			1
#define check_Name				2
#define check_PxAuthType		4
#define check_PortNbr			8
#define check_PhysicalProxyID	16
#define check_PxAddr			32
#define	check_Napid				64
#define check_NapAddr			128
#define check_AuthType			256
#define check_ClientId			512
#define check_ToNapid			1024
#define check_PxPhysical		2048


typedef struct
{
	WCHAR* pwchStrTab;
	UINT16 iStringIndex;
	UINT16 iElemCheck;
	BOOL bClientIDflag;
	BOOL bPxLogicalFlag;
	UINT16 iResult;
} PROV_HELPSTRUCT, *pPROV_HELPSTRUCT;




/*========================================================================
	Internal Functions
==========================================================================*/

/*========================================================================
	getTotalStringLen
==========================================================================*/
void getTotalStringLen(pPROV_PROVELEMENT pStartElem, UINT16* iLength)
{
	ELEMENTTYPE* pTemp = NULL;
	UINT16 iLocalLength = *iLength;

	do
	{
		pTemp = pStartElem->pContent;
		do
		{
			if ( pTemp->iType == Prov_Type_Characteristic)
			{
				getTotalStringLen ((PROV_PROVELEMENT*)pTemp, &iLocalLength);
				*iLength += (iLocalLength);
				pTemp = NULL;
			}
			else if (pTemp->iType == Prov_Type_Parm && ((PROV_PARMELEMENT*)pTemp)->pwchValue != NULL)
			{
				if (((PROV_PARMELEMENT*)pTemp)->iName == PRVNG_Parm_LITERAL  && ((PROV_PARMELEMENT*)pTemp)->pwchName != NULL)
					*iLength += ((STRINGLENGTH(((PROV_PARMELEMENT*)pTemp)->pwchName))+1);
				*iLength += ((STRINGLENGTH(((PROV_PARMELEMENT*)pTemp)->pwchValue))+1);
				pTemp = pTemp->pNextElement;
			}
			else if (pTemp->iType == Prov_Type_Parm && ((PROV_PARMELEMENT*)pTemp)->iValue == PRVNG_AUTOBAUDING)
			{
				*iLength += 12;
				pTemp = pTemp->pNextElement;
			}
			else
				pTemp = pTemp->pNextElement;
		}
		while (pTemp != NULL);
		pStartElem = (PROV_PROVELEMENT*)(pStartElem->pNextElement);
	}
	while (pStartElem != NULL);
}

/*========================================================================
	combinedLists
	Sätter ihop två listor 
==========================================================================*/
void* combinedLists(void* pTempList,void* pTempList2)
{
	void* pListElem, *pListElemContent;
	UINT8 iRootID;
	
	/* Gets the first elem in the list */
	pListElem = SDL_GetNextListElement (pTempList2, NULL);
	while (pListElem != NULL)
	{
		pListElemContent = SDL_GetListElementContent(pListElem);
		iRootID = SDL_GetListElementID (pListElem);
		SDL_AddElementLast(pTempList, iRootID, pListElemContent);
		pListElem = SDL_GetNextListElement (pTempList2, pListElem);
	}
	return pTempList;
}

/*========================================================================
	deleteListAndContent
	dealloc en lista och innehållet
==========================================================================*/

void deleteListAndContent(void* pList)
{
	void* pListElem, *pElemContent;
	DATATABSTRUCT* pTempStruct;

	if (pList != NULL)
	{
		/* Gets the first elem in the list */
		pListElem = SDL_GetNextListElement (pList, NULL);
		while(pListElem != NULL)
		{
			pElemContent = SDL_GetListElementContent(pListElem);
			pTempStruct = (DATATABSTRUCT*)pElemContent;
			DEALLOC(&pTempStruct);
			pListElem = SDL_GetNextListElement (pList, pListElem);
		}
		SDL_DeleteList(&pList);
	}
}

/*========================================================================
	setRootElemValueIndex
	dealloc en lista och innehållet
==========================================================================*/
void setRootElemValueIndex(void* pArrayList, UINT8 iRootElemId)
{
	UINT8 i = 1;	/* if only one root element, the first one shall have ValueIndex = 0 (and already has). Therefore we starts with i = 1 */
	void* pListElem, *pNewElemContent, *pRootElem;
	UINT16 iArrayIndex = 0;

	/* gets the first elem */
	pListElem = SDL_GetNextListElement (pArrayList, NULL);
	while (i < iRootElemId)  /* dont take the last root element */
	{
		/* gets the content of the root elem we want to change */
		pNewElemContent = SDL_GetListElementContent(pListElem);
		pRootElem = pListElem;
		while(SDL_GetListElementID (pListElem) != (i+1))		/* Search until we found the root element after the one we want to change */
		{
			pListElem = SDL_GetNextListElement (pArrayList, pListElem);
			iArrayIndex ++;
		}
		((DATATABSTRUCT*)pNewElemContent)->iValueIndex = iArrayIndex;
		SDL_ChangeListElementContent (pRootElem, pNewElemContent);
		i++;
	}
}

/*========================================================================
	checkIfAlreadySet
==========================================================================*/
BOOL checkIfAlreadySet(UINT16* iElemCheck, UINT16 check_X)
{
	if (*iElemCheck & check_X)
		return FALSE;
	else
		*iElemCheck += check_X;
	return TRUE;
}

/*========================================================================
	setStringValue
==========================================================================*/
void setStringValue(WCHAR* pTheString, pPROV_HELPSTRUCT pTheStruct)
{
	UINT16 iValueStringLen = (STRINGLENGTH(pTheString));
	COPYSTRINGN((pTheStruct->pwchStrTab + pTheStruct->iStringIndex),pTheString,iValueStringLen);
	DEALLOC(&(pTheString));
	pTheStruct->pwchStrTab[pTheStruct->iStringIndex + iValueStringLen] = '\0';
	pTheStruct->iStringIndex  += (iValueStringLen + 1);
}

/*========================================================================
	createStruct
==========================================================================*/
void createStruct(INT16 iName, UINT16 iValueIndex, void* pTempList, UINT8 iRootElemID)
{
	DATATABSTRUCT* pTempStruct = NEWSTRUCT(DATATABSTRUCT);

	pTempStruct->iName = iName;
	pTempStruct->iValueIndex = iValueIndex;
	SDL_AddElementLast(pTempList, iRootElemID, (void*)pTempStruct);
}

/*========================================================================
	checkIfValidElem
	Denna funktionen kollar så att det inte redan finns en paramter av samma sort,
	om denna parametern bara får finnas 1 gång under föräldern. Finns den redan returnerar vi FALSE
	och elementet läggs inte till i listan
==========================================================================*/
BOOL checkIfValidElem(pPROV_PROVELEMENT pParentElem, pPROV_PARMELEMENT pActual, pPROV_HELPSTRUCT pTheStruct)
{	

	BOOL fResult = FALSE;
	UINT16 iLocalElemCheck = pTheStruct->iElemCheck;

	switch (pParentElem->iAttribute)
	{
	case PRVNG_PXLOGICAL:
		if (pActual->iName == PRVNG_Parm_PROXY_ID && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_ProxyID);
		else if (pActual->iName == PRVNG_Parm_NAME && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_Name);
		else
			fResult = TRUE;
		break;
	case PRVNG_PXAUTHINFO:
		if (pActual->iName == PRVNG_Parm_PXAUTH_TYPE)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_PxAuthType);
		else
			fResult = TRUE;
		break;
	case PRVNG_PORT:
		if (pActual->iName == PRVNG_Parm_PORTNBR && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_PortNbr);
		else
			fResult = TRUE;
		break;
	case PRVNG_PXPHYSICAL:
		if (pActual->iName == PRVNG_Parm_PHYSICAL_PROXY_ID && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_PhysicalProxyID);
		else if (pActual->iName ==PRVNG_Parm_PXADDR && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_PxAddr);
		else if ((pActual->iName == PRVNG_Parm_TO_NAPID && pActual->pwchValue != NULL) && 
			!(iLocalElemCheck & check_ToNapid))
		{
			iLocalElemCheck += check_ToNapid;
			fResult = TRUE;
		}
		else
			fResult = TRUE;
		break;
	case PRVNG_NAPDEF:
		if (pActual->iName == PRVNG_Parm_NAPID && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_Napid);
		else if (pActual->iName == PRVNG_Parm_NAME && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_Name);
		else if (pActual->iName == PRVNG_Parm_NAP_ADDRESS && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_NapAddr);
		else
			fResult = TRUE;
		break;
	case PRVNG_NAPAUTHINFO:
		if (pActual->iName == PRVNG_name_AUTHTYPE)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_AuthType);
		else
			fResult = TRUE;
		break;
	case PRVNG_CLIENTIDENTITY:
		if (pActual->iName == PRVNG_Parm_CLIENT_ID && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_ClientId);
		else
			fResult = TRUE;
		break;
	case PRVNG_VENDOR_CONFIG:
		if (pActual->iName == PRVNG_Parm_NAME && pActual->pwchValue != NULL)
			fResult = checkIfAlreadySet(&iLocalElemCheck, check_Name);
		else
			fResult = TRUE;
		break;
	case PRVNG_VALIDITY:
		fResult = TRUE;
		break;
	case PRVNG_BOOTSTRAP:
		fResult = TRUE;
		break;
	default:
	/* Not valid value */
		fResult = FALSE;
		break;
	}
	pTheStruct->iElemCheck = iLocalElemCheck;
	return fResult;
}

/*========================================================================
	setParmValue
==========================================================================*/
void setParmValue(void* pTempList, pPROV_HELPSTRUCT pTheStruct, pELEMENTTYPE pActual)
{
	UINT16 iValueIndex;
	BYTE *bs = NULL;
	BOOL err = FALSE;
	WCHAR* pwchScheme;
	WCHAR pwchAutoBauding[]={'A','U','T','O','B','A','U','D','I','N','G'};

	switch (((PROV_PARMELEMENT*)pActual)->iName)
	{
		/* StringValue both in Name and Value */
		case PRVNG_Parm_LITERAL:
			if (((PROV_PARMELEMENT*)pActual)->pwchName != NULL)
			{
				iValueIndex = pTheStruct->iStringIndex;
				setStringValue(((PROV_PARMELEMENT*)pActual)->pwchName, pTheStruct);
				createStruct(5, iValueIndex, pTempList, 0);
				iValueIndex = pTheStruct->iStringIndex;
				setStringValue(((PROV_PARMELEMENT*)pActual)->pwchValue, pTheStruct);
				createStruct(0, iValueIndex, pTempList, 0);
			}
			break;
		/* String Values */
		case PRVNG_Parm_NAME:					
		case PRVNG_Parm_NAP_ADDRESS:								
		case PRVNG_Parm_VALIDUNTIL:									
		case PRVNG_Parm_AUTHNAME:				
		case PRVNG_Parm_AUTHSECRET:				
		case PRVNG_Parm_LINGER:									
		case PRVNG_Parm_NAPID:					
		case PRVNG_Parm_COUNTRY:				
		case PRVNG_Parm_NETWORK:				
		case PRVNG_Parm_INTERNET:				
		case PRVNG_Parm_PROXY_ID:				
		case PRVNG_Parm_PROXY_PROVIDER_ID:			
		case PRVNG_Parm_DOMAIN:													
		case PRVNG_Parm_PXAUTH_ID:				
		case PRVNG_Parm_PXAUTH_PW:							
		case PRVNG_Parm_BASAUTH_ID:				
		case PRVNG_Parm_BASAUTH_PW:				
		case PRVNG_Parm_PUSHENABLED:				
		case PRVNG_Parm_PXADDR:								
		case PRVNG_Parm_TO_NAPID:				
		case PRVNG_Parm_PORTNBR:												
		case PRVNG_Parm_LOCAL_ADDR:							
		case PRVNG_Parm_CONTEXT_ALLOW:				
		case PRVNG_Parm_TRUST:					
		case PRVNG_Parm_MASTER:					
		case PRVNG_Parm_SID:					
		case PRVNG_Parm_SOC:						
		case PRVNG_Parm_WSP_VERSION:				
		case PRVNG_Parm_PHYSICAL_PROXY_ID:		
		case PRVNG_Parm_CLIENT_ID:					
		case PRVNG_Parm_DELIVERY_ERR_SDU:			
		case PRVNG_Parm_DELIVERY_ORDER:				
		case PRVNG_Parm_TRAFFIC_CLASS:				
		case PRVNG_Parm_MAX_SDU_SIZE:				
		case PRVNG_Parm_MAX_BITRATE_UPLINK:			
		case PRVNG_Parm_MAX_BITRATE_DNLINK:		
		case PRVNG_Parm_RESIDUAL_BER:			
		case PRVNG_Parm_SDU_ERROR_RATIO:			
		case PRVNG_Parm_TRAFFIC_HANDL_PRIO:			
		case PRVNG_Parm_TRANSFER_DELAY:				
		case PRVNG_Parm_GUARANTEED_BITRATE_UPLINK:
		case PRVNG_Parm_GUARANTEED_BITRATE_DNLINK:
			if(((PROV_PARMELEMENT*)pActual)->pwchValue != NULL)
			{
				iValueIndex = pTheStruct->iStringIndex;
				setStringValue(((PROV_PARMELEMENT*)pActual)->pwchValue, pTheStruct);
				createStruct(((PROV_PARMELEMENT*)pActual)->iName, iValueIndex, pTempList, 0);
			}
			break;
		/* Toke Value*/
		case PRVNG_Parm_NAP_ADDRTYPE:
		case PRVNG_Parm_PXADDRTYPE:
		case PRVNG_Parm_LOCAL_ADDRTYPE:
		case PRVNG_Parm_CALLTYPE:
		case PRVNG_name_AUTHTYPE:
		case PRVNG_Parm_PXAUTH_TYPE:
		case PRVNG_Parm_BEARER:
		case PRVNG_Parm_SERVICE:
			if (((PROV_PARMELEMENT*)pActual)->iValue != 0)
				createStruct(((PROV_PARMELEMENT*)pActual)->iName, ((PROV_PARMELEMENT*)pActual)->iValue, pTempList, 0); 	
			break;
		case PRVNG_Parm_LINKSPEED:
		case PRVNG_Parm_DNLINKSPEED:
			if (((PROV_PARMELEMENT*)pActual)->iValue != PRVNG_AUTOBAUDING)
			{
				((PROV_PARMELEMENT*)pActual)->pwchValue = NEWARRAY(WCHAR,11);
				COPYSTRINGN(((PROV_PARMELEMENT*)pActual)->pwchValue,pwchAutoBauding,11);
			}
			else if (((PROV_PARMELEMENT*)pActual)->pwchValue != NULL)
			{
				iValueIndex = pTheStruct->iStringIndex;
				setStringValue(((PROV_PARMELEMENT*)pActual)->pwchValue, pTheStruct);
				createStruct(((PROV_PARMELEMENT*)pActual)->iName, iValueIndex, pTempList, 0);
			}
			break;
		case PRVNG_Parm_PROVURL:
		case PRVNG_Parm_STARTPAGE:
			/* Check that the URL is absolut, and if scheme is not included add http */
			if (((PROV_PARMELEMENT*)pActual)->pwchValue != NULL)
			{
				err = w_GetScheme ((const WCHAR*)((PROV_PARMELEMENT*)pActual)->pwchValue, &pwchScheme);
				if( pwchScheme == NULL)
				{
					bs = wip_wchar2byte (((PROV_PARMELEMENT*)pActual)->pwchValue, &err);
					if (err != TRUE) /* no overflow detected */
					{
						b_CompleteURLHeuristically (bs);
						((PROV_PARMELEMENT*)pActual)->pwchValue = wip_byte2wchar(bs);
					}
					else
						DEALLOC(&(((PROV_PARMELEMENT*)pActual)->pwchValue));
						break;
				}
				iValueIndex = pTheStruct->iStringIndex;
				setStringValue(((PROV_PARMELEMENT*)pActual)->pwchValue, pTheStruct);
				createStruct(((PROV_PARMELEMENT*)pActual)->iName, iValueIndex, pTempList, 0);
				DEALLOC(&pwchScheme);
				break;
			}
			break;
		default:
			/* Not valid value */
			break;
	}
}


/*========================================================================
	checkIfElemOk
	Denna funktion kollar så allt som måste finnas under ett element verkligen finns,
	saknas något returneras FALSE och hela char elemenet med alla sina barn kommer INTE
	att läggas till i arrayen. Kollar också att inte char element (clientidentity) som bara får finnas en
	gång kommer igen.
==========================================================================*/
BOOL checkIfElemOk(pPROV_PROVELEMENT pCharElem, pPROV_HELPSTRUCT pTheStruct)
	{	
	BOOL fResult = FALSE;

	switch(pCharElem->iAttribute)
	{
	case PRVNG_PXLOGICAL:
		/* Must exists one Proxy-Id and one Name */
		if ( (pTheStruct->iElemCheck & check_Name) && (pTheStruct->iElemCheck & check_ProxyID))
		{
			if (pTheStruct->iElemCheck & check_PxPhysical)
			{
				 pTheStruct->bPxLogicalFlag = TRUE;
				 fResult = TRUE;
			}
			else
				pTheStruct->bPxLogicalFlag = FALSE;
		}
		break;
	case PRVNG_PXAUTHINFO:
		if (pTheStruct->iElemCheck & check_PxAuthType) 
			fResult = TRUE;
		break;
	case PRVNG_PORT:
		if (pTheStruct->iElemCheck & check_PortNbr) 
			fResult = TRUE;
		break;
	case PRVNG_PXPHYSICAL:
		if ((pTheStruct->iElemCheck & check_PhysicalProxyID) && (pTheStruct->iElemCheck & check_PxAddr)
			&& (pTheStruct->iElemCheck & check_ToNapid))
			fResult = TRUE;
		break;
	case PRVNG_NAPDEF:
		if ((pTheStruct->iElemCheck & check_Napid) && (pTheStruct->iElemCheck & check_Name) && (pTheStruct->iElemCheck & check_NapAddr))
			fResult = TRUE;
		break;
	case PRVNG_NAPAUTHINFO:
		if (pTheStruct->iElemCheck & check_AuthType) 
			fResult = TRUE;
		break;
	case PRVNG_CLIENTIDENTITY:
		if ((!pTheStruct->bClientIDflag) && (pTheStruct->iElemCheck & check_ClientId))
		{
			pTheStruct->bClientIDflag = TRUE;
			fResult = TRUE;
		}
		break;
	case PRVNG_VENDOR_CONFIG:
		if (pTheStruct->iElemCheck & check_Name) 
			fResult = TRUE;
		break;
	case PRVNG_BOOTSTRAP: 	
		fResult = TRUE;
		break;
	case PRVNG_VALIDITY: 	
		fResult = TRUE;
		break;
	default:
		fResult = FALSE;
		break;
	}
	return fResult;
}

/*========================================================================
	addElementInList
==========================================================================*/
void* addElementInList(pPROV_PROVELEMENT pCharElem, pPROV_HELPSTRUCT pTheStruct, UINT8 iRootID)
{
	UINT16 iLocalStringIndex, iLocalElemCheck = 0;
	void* pTempList;
	void* pTempList2;
	ELEMENTTYPE* pActual, *pTemp;
	
	pTempList = SDL_NewList( 0 );

	createStruct(pCharElem->iAttribute, 0, pTempList, iRootID);
	pTheStruct->iElemCheck = (pTheStruct->iElemCheck & check_PxPhysical);

	pActual = pCharElem->pContent;
	while (pActual != NULL)
	{
		pTemp = pActual;
		if (pActual->iType == Prov_Type_Parm)
		{
			if (checkIfValidElem(pCharElem, (PROV_PARMELEMENT*)pActual, pTheStruct))
			{
				setParmValue(pTempList, pTheStruct, pActual);						
				pActual=((PROV_PARMELEMENT*)pActual)->pNextElement;
			}
			else
				pActual=((PROV_PARMELEMENT*)pActual)->pNextElement;
		}
		else if (pActual->iType == Prov_Type_Characteristic)
		{	
			iLocalStringIndex = pTheStruct->iStringIndex;
			iLocalElemCheck = pTheStruct->iElemCheck;
			pTempList2 = addElementInList((PROV_PROVELEMENT*)pActual, pTheStruct, 0 );
			if (checkIfElemOk((PROV_PROVELEMENT*)pActual, pTheStruct))	/* the char element is ok, add it to the list */
			{
				if ((((PROV_PROVELEMENT*)pActual)->iAttribute == PRVNG_PXPHYSICAL) && 
					(pCharElem->iAttribute == PRVNG_PXLOGICAL) && ((pTheStruct->iElemCheck & check_PxPhysical) == 0))
					iLocalElemCheck += check_PxPhysical; 
				pTempList = combinedLists(pTempList, pTempList2);
				SDL_DeleteList(&pTempList2);
			}
			else /* char element not ok, delete the list and change stringindex to point before those strings that belongs to the invalid elements  */
			{
				deleteListAndContent(pTempList2);
				pTheStruct->iResult = pTheStruct->iResult | Warning_IncompleteAttributeRemoved;
				pTheStruct->iStringIndex = iLocalStringIndex;
			}
				pTheStruct->iElemCheck = iLocalElemCheck;
			pActual=((PROV_PARMELEMENT*)pActual)->pNextElement;
		}
		else
			pActual=((PROV_PARMELEMENT*)pActual)->pNextElement;
		DEALLOC(&pTemp);
	}
	/*set -1 in the name field to indicate no more children for this parent */
	createStruct(-1, 0, pTempList, 0);
	return pTempList;
}

/*========================================================================
	createNewHELPSTRUCT
==========================================================================*/
PROV_HELPSTRUCT* createNewHELPSTRUCT(UINT16 iLength, UINT16 iError)
{
	PROV_HELPSTRUCT* pTheStruct = NEWSTRUCT(PROV_HELPSTRUCT);

	pTheStruct->pwchStrTab = NEWARRAY(WCHAR,iLength);
	pTheStruct->iStringIndex = 0;
	pTheStruct->bClientIDflag = FALSE;
	pTheStruct->bPxLogicalFlag = FALSE;
	pTheStruct->iResult = iError;
	pTheStruct->iElemCheck = 0;

	return pTheStruct;
}

/*========================================================================
	deleteHELPSTRUCT
==========================================================================*/
void deleteHELPSTRUCT(pPROV_HELPSTRUCT pTheStruct)
{
	DEALLOC(&(pTheStruct->pwchStrTab));
	DEALLOC(&pTheStruct);
}

/*========================================================================
	External functions
==========================================================================*/

/*========================================================================
	parseProvData
==========================================================================*/
void* parseProvData (BYTE *pbData, UINT32 pbDataLen, int id, UINT16 *iError)
{
	WBXMLDECODESTR* pDecStr=NULL;
	ELEMENTTYPE* pStructure=NULL;
	BOOL finished;
	BYTE* pbEnd = pbData + pbDataLen;
	INT16 iCharset = 0;

	*iError = 0;

	/* Create tWBXMLDECODESTR and init the attributes */
	pDecStr=WBXML_CreateWBXMLDecStr();

	if (pDecStr!=NULL)
	{
		/* Set the function pointers */
		pDecStr->DTD_CreateElement = Prov_CreateElement;
		pDecStr->DTD_DeleteElement = Prov_DeleteElement;
		pDecStr->DTD_ValidContent = Prov_ValidContent;
		pDecStr->DTD_GetContent = Prov_GetContent;
		pDecStr->DTD_AppSpecParseMod = Prov_AppSpecParseMod;
		pDecStr->DTD_StoreAttributeValue = Prov_StoreAttributeValue;
		pDecStr->DTD_GetAttributeValue = Prov_GetAttributeValue;
		pDecStr->DTD_LiteralAttributeToToken = Prov_LiteralAttributeToToken;
		pDecStr->DTD_LiteralTagToToken = Prov_LiteralTagToToken;
		pDecStr->DTD_CheckPublicID = PROV_CheckPublicID;

		/* Store data in the decode struct */
		pDecStr->bContentType='\x36';	/* application/vnd.wap.connectivity-wbxml*/
		pDecStr->iCharset=iCharset;
		if (id == -1 || id == -2)
			pDecStr->iContentLevel=0;			/* Use this as a flag for e2esec */
		else									
			pDecStr->iContentLevel=1;
		pDecStr->pbCurByte=pbData;
		pDecStr->pbEnd=pbEnd;
		pDecStr->iDecodeResult=0;		
		pDecStr->pAppSpec=NULL;
		
		/* Parse prolog */
		if (WBXML_DecodeProlog(pDecStr))
		{
			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);

			/* Parse channel */
			WBXML_InitDecode(pDecStr, FALSE);
			pStructure = WBXML_Decode(pDecStr, &finished);

			/* Step past all PI:s */
			WBXML_StepPastAllPIs (pDecStr);
		}

		/* Check if any (fatal) errors */
		if ( pDecStr->iDecodeResult < 127)
		{
			/* Store the structure under the application specific pointer
			   in the decode struct. */
			pDecStr->pAppSpec=pStructure;
		}
		else
		{
			/* Error - delete */
			Prov_DeleteElement(pDecStr,&pStructure);

			/* Delete decode struct */
			WBXML_DeleteWBXMLDecStr(&pDecStr);
			*iError = Error_InDecoding;
		}
	}

	/* Return the decode struct */
	return (void*)pDecStr;
}


/*========================================================================
	buildDataArray
==========================================================================*/
void buildDataArray( void* vsParsedData, int iObjectID, int iTID, UINT32 iExpireDate, UINT16 iError)
{
	WBXMLDECODESTR* pDecStr;
	PROV_PROVELEMENT* pStartElem;
	ELEMENTTYPE* pTemp,*pTemp2;
	DATATABSTRUCT* pDataTab, *pTempDataTab;
	PROV_HELPSTRUCT* pTheStruct; 
	void* pListElem, *pArrayList = NULL, *pTempList;
	UINT16 iLocalStringIndex, iArrayIndex = 0;
	UINT16 iLength = 0;
	UINT8 iRootElemID = 0;
	
	if ((iError & Error_InDecoding) == 0)
	{
		pDecStr = (WBXMLDECODESTR*) vsParsedData;
		pStartElem = (PROV_PROVELEMENT*)(pDecStr->pAppSpec);

		/* Get total length of all string values */
		getTotalStringLen(pStartElem, &iLength);

		pTheStruct = createNewHELPSTRUCT(iLength, iError);
		
		/* A temp list used to store the data that shall be stored in the array,
			we can't store it in the array direcly since we don't know the size of the array */
		pArrayList = SDL_NewList( 0 );

		/* The first elemenet is the wap-provdoc, we don't want to store this in the array. Therefor we get the next */
		pTemp = pStartElem->pContent;
		DEALLOC(&pStartElem);
		
		/* Every time a new root element, jump into this while loop */
		while (pTemp != NULL)
		{
			if (pTemp->iType == Prov_Type_Characteristic)
			{
				pTheStruct->iElemCheck = 0;
				switch(((PROV_PROVELEMENT*)pTemp)->iAttribute)
				{
				/* A valid root element */
				case PRVNG_PXLOGICAL:
				case PRVNG_NAPDEF:
				case PRVNG_BOOTSTRAP:
				case PRVNG_VENDOR_CONFIG:
				case PRVNG_CLIENTIDENTITY:
					iRootElemID ++;
					iLocalStringIndex = pTheStruct->iStringIndex;
					pTempList = addElementInList((PROV_PROVELEMENT*)pTemp, pTheStruct, iRootElemID );					
					if ((pTempList != NULL) && (checkIfElemOk((PROV_PROVELEMENT*)pTemp, pTheStruct)))	/* the char element is ok, add it to the list */
					{
						pArrayList = combinedLists(pArrayList, pTempList);
						SDL_DeleteList(&pTempList);
					}
					else /* char element not ok, delete the list */
					{
						deleteListAndContent(pTempList);
						pTheStruct->iResult = pTheStruct->iResult | Warning_IncompleteAttributeRemoved;
						pTheStruct->iStringIndex = iLocalStringIndex;
						iRootElemID --;
					}
					break;
				default:
					/* icke root element satt som root doc, dock ett char doc. ignorera elementet */
					pTheStruct->iResult = pTheStruct->iResult | Warning_IllegalPlacedAttributeRemoved;
					break;
				}
			}
			/* if the root element is not a CHAR elem it's not allowed - don't add it to the array */
			pTemp2 = pTemp;
			pTemp = pTemp->pNextElement;
			DEALLOC(&pTemp2);
		}
		/* No more elements */	
		/* Sets the indexvalues for the root elements, points to next rootelement */	
		if (iRootElemID != 1)
			setRootElemValueIndex(pArrayList, iRootElemID);

		/* Copy the information from the list to the array */
		iLength = SDL_GetCardinal(pArrayList);
		pDataTab = NEWARRAY(DATATABSTRUCT, iLength);
		if (iLength > 0)
		{
			/* Get the first elem in the list */
			pListElem = SDL_GetNextListElement(pArrayList,NULL);
			iArrayIndex = 0;
			while (pListElem != NULL)
			{
				pTempDataTab= (DATATABSTRUCT*)SDL_GetListElementContent(pListElem);
				pDataTab[iArrayIndex].iName = pTempDataTab->iName;
				pDataTab[iArrayIndex].iValueIndex = pTempDataTab->iValueIndex;
				iArrayIndex++;
				pListElem = SDL_GetNextListElement(pArrayList,pListElem);
			}
			deleteListAndContent(pArrayList);
		}
		else
			SDL_DeleteList(&pArrayList);
		if (iTID == -2 || iTID == -1)
		CLNTa_resultDecodeProvisioning((const DATATABSTRUCT*)pDataTab, iLength, pTheStruct->pwchStrTab, 
										pTheStruct->iStringIndex, pTheStruct->iResult);
		else 
			CLNTa_resultDecodeE2Esecurity((UINT8)iObjectID, (UINT8)iTID, iExpireDate, (const DATATABSTRUCT*)pDataTab, iLength, pTheStruct->pwchStrTab, 
										pTheStruct->iStringIndex, pTheStruct->iResult);
		deleteHELPSTRUCT(pTheStruct);
		DEALLOC(&pDataTab);
	}
	else if (iTID == -2 || iTID == -1)
		CLNTa_resultDecodeProvisioning(NULL, 0, NULL, 0, iError);
	else 
		CLNTa_resultDecodeE2Esecurity((UINT8)iObjectID, (UINT8)iTID, iExpireDate, NULL,0, NULL, 0, iError);
}

/*========================================================================
	checkHMAC
==========================================================================*/
#ifdef CONFIG_WTLS
BOOL checkHMAC(PROV_MACELEMENT* pMacStr, BYTE* vsSharedSecret, UINT16 iLenSharedSecret, BYTE* pbResult)
{
	wtls_crypto_t cobj;
	INT16 fResult;

	fResult = wtls_crypto_init (&cobj, CIPHER_NULL, MAC_SHA, COMPRESS_NULL);
	if (fResult != -1)
	{
		fResult = wtls_crypto_HMAC_init (&cobj, vsSharedSecret, iLenSharedSecret);
		if (fResult != -1)
		{
			wtls_crypto_HMAC_update (&cobj, (BYTE*) pMacStr->vsBody, (UINT16)pMacStr->iBodyLen);
			if (fResult != -1)
			{ 
				fResult = wtls_crypto_HMAC_final (&cobj, vsSharedSecret, iLenSharedSecret, pbResult);
				if (fResult != -1)
					return TRUE;				
			}
		}
	}
	return FALSE;
}
#endif
/*========================================================================
	calculateMAC
==========================================================================*/
BOOL calculateMAC(PROV_MACELEMENT* pMacStr, void* vsSharedSecret, UINT16 iLenSharedSecret)
{
	BYTE* pbKey = NULL, *pbEncodedRes = NULL, *ipbEncodedRes = NULL;
	UINT16 i=0;
	BOOL fResult = FALSE;
	UINT32 iMacLength = 20;  /* the size of the result from SHA (used by HMAC) is always 20 bytes */
	BYTE* pbResult = NEWARRAY(BYTE,iMacLength);

#ifdef CONFIG_WTLS
    BYTE* temp;
	if (pMacStr->iSEC == USERPINMAC)
	{ 
		if(iLenSharedSecret%2)
			iLenSharedSecret--;
		B_COPYSTRINGN(pbKey,(BYTE*)vsSharedSecret,(iLenSharedSecret/2));
		if( checkHMAC(pMacStr, pbKey, (iLenSharedSecret/2), pbResult))
		{
			while (i < (iLenSharedSecret/2))
			{
				pbResult[i] = (pbResult[i] % 10) + 48;
				i++;
			}
			if (ByteStringNCmpNoCase (pbResult, (BYTE*) vsSharedSecret + (iLenSharedSecret/2), (UINT32)((iLenSharedSecret/2)-1)) == 0)
				fResult = TRUE;
		}
	}
	else if ((pMacStr->iSEC == NETWPIN || pMacStr->iSEC == USERPIN || pMacStr->iSEC ==USERNETWPIN) &&
			checkHMAC(pMacStr, (BYTE*)vsSharedSecret, iLenSharedSecret, pbResult))
	{
		ipbEncodedRes = pbEncodedRes = NEWARRAY(BYTE, iMacLength*2);

	    for (i = 0; i < iMacLength; i++)
		{
			ByteToHex(pbResult[i], ipbEncodedRes);
			ipbEncodedRes += 2;
		}
        temp = (BYTE*)pMacStr->vsMAC;
        if (temp[0] == '"')
        {
          if ( ByteStringNCmpNoCase (pbEncodedRes, temp + 1, (iMacLength*2)-1) == 0)
            fResult = TRUE;
        }
		else
        {
          if ( ByteStringNCmpNoCase (pbEncodedRes, temp, (iMacLength*2)-1) == 0)
            fResult = TRUE;
        }
	}
	else
		fResult = FALSE;
#else
    pMacStr = pMacStr;
    vsSharedSecret = vsSharedSecret;
    iLenSharedSecret = iLenSharedSecret;
#endif
	DEALLOC(&pbResult);
	DEALLOC(&(pbEncodedRes));
	return fResult;
}
