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

	File: P_CoDec.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000207	JPR		Updated to support new WBXML-decoder

=========================================================================*/

#include "p_codec.h"
#include "p_cotok.h"
#include "pushelm.h"
#include "pushdef.h"

/*========================================================================

	Implemented function pointers for CO

=========================================================================*/

/*========================================================================
	Co_AppSpecParseMod
==========================================================================*/
void Co_AppSpecParseMod (pDECSTR pDecStr, pELEMENTTYPE *ppElement, 
						 pELEMENTTYPE pParent)
{
	/* This function is not used in CO */
	pDecStr=pDecStr;
	ppElement=ppElement;
	pParent=pParent;
}


/*========================================================================
	Co_StoreAttributeValue
==========================================================================*/
BOOL Co_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;

	if (pElement!=NULL)
	{
		/* Store attribute value */
		switch (iAttribute)
		{
		case Co_ATTRST_href:
			fResult=Co_SetURI(pDecStr,pElement,ppAttrVal);
			break;
		case Co_ATTRST_href_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Co_SetURI(pDecStr,pElement,ppAttrVal);
			break;
		case Co_ATTRST_href_www:
			XML_AddHeadString(ppAttrVal,"http://www.",pDecStr);
			fResult=Co_SetURI(pDecStr,pElement,ppAttrVal);
			break;
		case Co_ATTRST_href_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Co_SetURI(pDecStr,pElement,ppAttrVal);
			break;
		case Co_ATTRST_https_www:
			XML_AddHeadString(ppAttrVal,"https://www.",pDecStr);
			fResult=Co_SetURI(pDecStr,pElement,ppAttrVal);
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
	Co_GetAttributeValue
==========================================================================*/
WCHAR* Co_GetAttributeValue (UINT16 iToken)
{
	CHAR* pchString=NULL;

	switch (iToken)
	{
	case Co_ATTRVAL_com:
		pchString=".com/";
		break;
	case Co_ATTRVAL_edu:
		pchString=".edu/";
		break;
	case Co_ATTRVAL_net:
		pchString=".net/";
		break;
	case Co_ATTRVAL_org:
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
	Co_LiteralAttributeToToken
==========================================================================*/
UINT16 Co_LiteralAttributeToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchCoAttrTokens,CO_STAT_NBROFATTR);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piCoAttrBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Co_LiteralTagToToken
==========================================================================*/
UINT16 Co_LiteralTagToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchCoTagTokens,CO_STAT_NBROFTAGS);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piCoTagBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Co_CheckPublicID
==========================================================================*/
BOOL Co_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* CO 1.0 */
		WCHAR pwchPublic_ID_1_0[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','C','O',' ','1',
								   '.','0','/','/','E','N',0};

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,pwchPublic_ID_1_0,iLength))
		{
			/* CO 1.0 - OK */
			*piPId=CO_VERSION_1_0;
			return TRUE;
		}

		/* Check content-type and level (0x32 is application/vnd.wap.coc) */
		if (pDecStr->bContentType=='\x32')
		{
			/* Check level */
			if (pDecStr->iContentLevel=='\x10')
			{
				/* CO 1.0 - OK */
				*piPId=CO_VERSION_1_0;
				return TRUE;
			}
		}	
	}

	/* Use integer */
	else
	{
		/* Public ID (co 1.0) is 0x07 */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x32 is application/vnd.wap.coc)*/
			if (pDecStr->bContentType=='\x32')
			{
				/* Check level */
				if (pDecStr->iContentLevel=='\x10')
				{
					/* CO 1.0 - OK */
					*piPId=CO_VERSION_1_0;
					return TRUE;
				}
			}
			return FALSE;

		/* CO 1.0 */
		case '\x07':
			*piPId=CO_VERSION_1_0;
			return TRUE;
		}
	}

	/* Other - not supported */
	return FALSE;
}

