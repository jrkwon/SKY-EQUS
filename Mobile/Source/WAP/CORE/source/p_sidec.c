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

	File: P_SiDec.c

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000201	JPR		Updated to support generic WBXML-decoder

=========================================================================*/

#include "p_sidec.h"


/*========================================================================

	Implemented function pointers for SI

=========================================================================*/

/*========================================================================
	Si_AppSpecParseMod
==========================================================================*/
void Si_AppSpecParseMod (pDECSTR pDecStr, pELEMENTTYPE *ppElement, 
						 pELEMENTTYPE pParent)
{
	/* This function is not used in SI */
	pDecStr=pDecStr;
	ppElement=ppElement;
	pParent=pParent;
}


/*========================================================================
	Si_StoreAttributeValue
==========================================================================*/
BOOL Si_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal)
{
	BOOL fResult=TRUE;

	if (pElement!=NULL)
	{
		/* Store attribute value */
		switch (iAttribute)
		{
		case Si_ATTRST_signalnone:
			fResult=Si_SetAction(pDecStr,pElement,ppAttrVal,SI_SIGNAL_NONE);
			break;
		case Si_ATTRST_signallow:
			fResult=Si_SetAction(pDecStr,pElement,ppAttrVal,SI_SIGNAL_LOW);
			break;
		case Si_ATTRST_signalmedium:
			fResult=Si_SetAction(pDecStr,pElement,ppAttrVal,SI_SIGNAL_MEDIUM);
			break;
		case Si_ATTRST_signalhigh:
			fResult=Si_SetAction(pDecStr,pElement,ppAttrVal,SI_SIGNAL_HIGH);
			break;
		case Si_ATTRST_delete:
			fResult=Si_SetAction(pDecStr,pElement,ppAttrVal,SI_DELETE);
			break;
		case Si_ATTRST_created:
			fResult=Si_SetCreated(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_href:
			fResult=Si_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_href_http:
			XML_AddHeadString(ppAttrVal,"http://",pDecStr);
			fResult=Si_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_href_www:
			XML_AddHeadString(ppAttrVal,"http://www.",pDecStr);
			fResult=Si_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_href_https:
			XML_AddHeadString(ppAttrVal,"https://",pDecStr);
			fResult=Si_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_https_www:
			XML_AddHeadString(ppAttrVal,"https://www.",pDecStr);
			fResult=Si_SetHref(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_expires:
			fResult=Si_SetExpires(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_siid:
			fResult=Si_SetSiId(pDecStr,pElement,ppAttrVal);
			break;
		case Si_ATTRST_class:
			fResult=Si_SetClass(pDecStr,pElement,ppAttrVal);
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
	Si_GetAttributeValue
==========================================================================*/
WCHAR* Si_GetAttributeValue (UINT16 iToken)
{
	CHAR* pchString=NULL;

	switch (iToken)
	{
	case Si_ATTRVAL_com:
		pchString=".com/";
		break;
	case Si_ATTRVAL_edu:
		pchString=".edu/";
		break;
	case Si_ATTRVAL_net:
		pchString=".net/";
		break;
	case Si_ATTRVAL_org:
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
	Si_LiteralAttributeToToken
==========================================================================*/
UINT16 Si_LiteralAttributeToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchSiAttrTokens,SI_STAT_NBROFATTR);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piSiAttrBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Si_LiteralTagToToken
==========================================================================*/
UINT16 Si_LiteralTagToToken (WCHAR* pwchToken)
{
	UINT16 iToken=Type_Literal;
	INT16 iIndex=-1;

	/* Get index from string table */
	iIndex=FindStringBinarySearch (pwchToken,ppwchSiTagTokens,SI_STAT_NBROFTAGS);

	if (iIndex>=0)
	{
		/* Found, find token at location (index) */
		iToken=piSiTagBytes[iIndex];
	}

	return iToken;
}


/*========================================================================
	Si_CheckPublicID
==========================================================================*/
BOOL Si_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId)
{
	/* ASSERT: pDecStr!=NULL
	*/

	/* Use string */
	if (pwchPId!=NULL)
	{
		UINT32 iLength=0;

		/* SI 1.0 */
		WCHAR pwchPublic_ID_1_0[]={'-','/','/','W','A','P','F','O','R','U','M',
								   '/','/','D','T','D',' ','S','I',' ','1',
								   '.','0','/','/','E','N',0};

		/* Get length of pwchPId string */
		iLength=(UINT32) (w_strlen(pwchPId));

		if (CompareWideStringNoCase (pwchPId,pwchPublic_ID_1_0,iLength))
		{
			/* SI 1.0 - OK */
			*piPId=SI_VERSION_1_0;
			return TRUE;
		}

		/* Check content-type and level (0x2e is application/vnd.wap.sic) */
		if (pDecStr->bContentType=='\x2e')
		{
			/* Check level */
			if (pDecStr->iContentLevel=='\x10')
			{
				/* SI 1.0 - OK */
				*piPId=SI_VERSION_1_0;
				return TRUE;
			}
		}	
	}

	/* Use integer */
	else
	{
		/* Public ID (si 1.0) is 0x05 */

		switch (*piPId)
		{

		/* Unknown */
		case '\x01':
			/* Check content-type and level (0x2e is application/vnd.wap.sic)*/
			if (pDecStr->bContentType=='\x2e')
			{
				/* Check level */
				if (pDecStr->iContentLevel=='\x10')
				{
					/* SI 1.0 - OK */
					*piPId=SI_VERSION_1_0;
					return TRUE;
				}
			}
			return FALSE;

		/* SI 1.0 */
		case '\x05':
			*piPId=SI_VERSION_1_0;
			return TRUE;
		}
	}

	/* Other - not supported */
	return FALSE;

}
