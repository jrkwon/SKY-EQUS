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

	File: P_SlDec.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000207	JPR		Updated to support new WBXML-decoder
	010126	NKE		New definition of Sl_AppSpecParseMod

=========================================================================*/

#include "p_sldec.h"
#include "p_sltok.h"

/*========================================================================

	Implemented function pointers for SL

=========================================================================*/

/*========================================================================
	Sl_AppSpecParseMod
==========================================================================*/
void Sl_AppSpecParseMod (pDECSTR pDecStr)
{
	/* This function is not used in SL */
	pDecStr=pDecStr;
}


/*========================================================================
	Sl_StoreAttributeValue
==========================================================================*/
BOOL Sl_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;

	if (pElement!=NULL)
	{
		/* Store attribute value */
		switch (iAttribute)
		{
		case Sl_ATTRST_executelow:
			fResult=Sl_SetAction(pDecStr,pElement,ppAttrVal,SL_EXECUTE_LOW);
			break;
		case Sl_ATTRST_executehigh:
			fResult=Sl_SetAction(pDecStr,pElement,ppAttrVal,SL_EXECUTE_HIGH);
			break;
		case Sl_ATTRST_cache:
			fResult=Sl_SetAction(pDecStr,pElement,ppAttrVal,SL_CACHE);
			break;
		case Sl_ATTRST_href:
			fResult=Sl_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Sl_ATTRST_href_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Sl_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Sl_ATTRST_href_www:
			XML_AddHeadString(ppAttrVal,"http://www.",pDecStr);
			fResult=Sl_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Sl_ATTRST_href_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Sl_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Sl_ATTRST_https_www:
			XML_AddHeadString(ppAttrVal,"https://www.",pDecStr);
			fResult=Sl_SetHref(pDecStr,pElement,ppAttrVal);
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
	Sl_GetAttributeValue
==========================================================================*/
WCHAR* Sl_GetAttributeValue (UINT16 iToken)
{
	CHAR* pchString=NULL;

	switch (iToken)
	{
	case Sl_ATTRVAL_com:
		pchString=".com/";
		break;
	case Sl_ATTRVAL_edu:
		pchString=".edu/";
		break;
	case Sl_ATTRVAL_net:
		pchString=".net/";
		break;
	case Sl_ATTRVAL_org:
		pchString=".org/";
		break;
	}

	if (pchString!=NULL)
	{
		/* Copy string and return */
		return (w_cstr2wstr(pchString));
	}

	return NULL;
}


/*========================================================================
	Sl_LiteralAttributeToToken
==========================================================================*/
UINT16 Sl_LiteralAttributeToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchSlAttrTokens,SL_STAT_NBROFATTR);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piSlAttrBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Sl_LiteralTagToToken
==========================================================================*/
UINT16 Sl_LiteralTagToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchSlTagTokens,SL_STAT_NBROFTAGS);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piSlTagBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Sl_CheckPublicID
==========================================================================*/
BOOL Sl_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* SL 1.0 */
		WCHAR pwchPublic_ID_1_0[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','S','L',' ','1',
								   '.','0','/','/','E','N',0};

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,pwchPublic_ID_1_0,iLength))
		{
			/* SL 1.0 - OK */
			*piPId=SL_VERSION_1_0;
			return TRUE;
		}

		/* Check content-type and level (0x30 is application/vnd.wap.slc) */
		if (pDecStr->bContentType=='\x30')
		{
			/* Check level */
			if (pDecStr->iContentLevel=='\x10')
			{
				/* SL 1.0 - OK */
				*piPId=SL_VERSION_1_0;
				return TRUE;
			}
		}	
	}

	/* Use integer */
	else
	{
		/* Public ID (sl 1.0) is 0x06 */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x30 is application/vnd.wap.slc)*/
			if (pDecStr->bContentType=='\x30')
			{
				/* Check level */
				if (pDecStr->iContentLevel=='\x10')
				{
					/* SL 1.0 - OK */
					*piPId=SL_VERSION_1_0;
					return TRUE;
				}
			}
			return FALSE;

		/* SL 1.0 */
		case '\x06':
			*piPId=SL_VERSION_1_0;
			return TRUE;
		}
	}

	/* Other - not supported */
	return FALSE;
}

