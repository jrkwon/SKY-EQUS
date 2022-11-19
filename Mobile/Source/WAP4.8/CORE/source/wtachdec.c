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

	WAP WTA Implementation Project

==========================================================================

	File: WTAChDec.c

	Description:

	Author: Jens Pommer, AU-System AB

	Revision history:
  	Date	Sign	Comment
  	990813  JPR		First version
	991115	JPR		Channel content version updated to 1.1 (WAP 1.2)
	000207	JPR		Updated to support new WBXML-decoder
	000630	JPR		Updated for WAP 1.2.1
	001011	ASPN	Lower-case filenames
    010124  JPR     Correction in function Ch_CheckPublicID. Now supports
                    channel version 1.2 (PuclicId 0x0D)
    010126  NKE     New definition of Ch_AppSpecParseMod
==========================================================================*/

#include "wtachdec.h"
#include "wtachdef.h"
#include "wtachtok.h"


/*========================================================================

	Implemented function pointers for channels

=========================================================================*/

/*========================================================================
	Ch_AppSpecParseMod
==========================================================================*/
void Ch_AppSpecParseMod(pDECSTR pDecStr)
{
	/* Not needed for channel decoder. */
	pDecStr=pDecStr;
}


/*========================================================================
	Ch_StoreAttributeValue
==========================================================================*/
BOOL Ch_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement, 
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;

	if (pElement!=NULL)
	{
		/* Store attribute value */
		switch (iAttribute)
		{
		case Ch_ATTRST_maxspace:
			fResult=Ch_SetMaxspace(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_base:
			fResult=Ch_SetBase(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_href:
			fResult=Ch_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_href_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Ch_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_href_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Ch_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_lastmod:
			fResult=Ch_SetLastmod(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_etag:
			fResult=Ch_SetEtag(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_md5:
			fResult=Ch_SetMd5(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_success:
			fResult=Ch_SetSuccess(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_success_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Ch_SetSuccess(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_success_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Ch_SetSuccess(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_failure:
			fResult=Ch_SetFailure(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_failure_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Ch_SetFailure(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_failure_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Ch_SetFailure(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_eventid:
			fResult=Ch_SetEventId(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_eventid_wtaev:
			XML_AddHeadString(ppAttrVal,"wtaev-",pDecStr);
			fResult=Ch_SetEventId(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_channelid:
			fResult=Ch_SetChannelID(pDecStr,pElement,ppAttrVal);
			break;
		case Ch_ATTRST_useraccessible:
			fResult=Ch_SetUserAccessible(pDecStr,pElement,ppAttrVal);
			break;
		default:
			/* Not valid attribute start token. */
			fResult=FALSE;
			break;
		}
	}

	/* Check if all elements in the attribute value were used in the 
	   store function. If not all elements were user, return FALSE. 
	   This will result in a warning. Also, delete the unused element
	   to avoid memory leak. */
	if (*ppAttrVal!=NULL)
	{
		fResult=FALSE;
		XML_DeleteElementList(ppAttrVal,pDecStr);
	}	

	return fResult;
}


/*========================================================================
	Ch_GetAttributeValue
==========================================================================*/
WCHAR* Ch_GetAttributeValue (UINT16 iToken)
{
	iToken=iToken;

	/* None defined */
	return NULL;
}


/*========================================================================
	Ch_LiteralAttributeToToken
==========================================================================*/
UINT16 Ch_LiteralAttributeToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchChAttrTokens,CH_STAT_NBROFATTR);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piChAttrBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Ch_LiteralTagToToken
==========================================================================*/
UINT16 Ch_LiteralTagToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchChTagTokens,CH_STAT_NBROFTAGS);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piChTagBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Ch_CheckPublicID
==========================================================================*/
BOOL Ch_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* Channel 1.1 */
		WCHAR pwchPublic_ID_1_1[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','C','H','A','N','N',
								   'E','L',' ','1','.','1','/','/','E','N',0};

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,pwchPublic_ID_1_1,iLength))
		{
			/* Channel 1.1 - OK */
			*piPId=CH_VERSION_1_1;
			return TRUE;
		}

		/* Check content-type and level (0x16 is application/vnd.wap.channelc) */
		if (pDecStr->bContentType=='\x16')
		{
			/* Check level */
			if (pDecStr->iContentLevel=='\x11')
			{
				/* Channel 1.1 - OK */
				*piPId=CH_VERSION_1_1;
				return TRUE;
			}
		}	
	}

	/* Use integer */
	else
	{
		/* Public ID (channel 1.1) is 0x08 */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x16 is application/vnd.wap.channelc) */
			if (pDecStr->bContentType=='\x16')
			{
				/* Check level */
				if (pDecStr->iContentLevel=='\x11')
				{
					/* Channel 1.1 - OK */
					*piPId=CH_VERSION_1_1;
					return TRUE;
				}
			}	
			return FALSE;

		/* Channel 1.1 - OK */
		case '\x08':
			*piPId=CH_VERSION_1_1;
			return TRUE;
            
		/* Channel 1.2 - OK */
		case '\x0d':
			*piPId=CH_VERSION_1_2;
			return TRUE;
		}
	}

	/* Other - not supported */
	return FALSE;
}




