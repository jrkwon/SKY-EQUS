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

	File: wtachelm.c

	Description:

	Author: Jens Pommer, AU-System AB

	Revision history:
	Date	Sign	Comment
	990813	JPR		First version
	000204	JPR		Updated to support new WBXML-decoder
	000211	JPR		Etag changed from WCHAR* to BYTE*
	000630	JPR		Updated for WAP 1.2.1
	001011	ASPN	Lower-case filenames
	010119	NKE		Replaced Ch_AddContent with Ch_ValidContent
					for the new decoder in wbxmldec.c.

==========================================================================*/

#include "wtachelm.h"
#include "wtachdef.h"
#include "wmlelm.h"
#include "waectype.h"

/*========================================================================

	Private Functions

=========================================================================*/

/*========================================================================
	Ch_CreateCHANNEL
==========================================================================*/
pCH_CHANNELELEMENT Ch_CreateCHANNEL (void) 
{
	pCH_CHANNELELEMENT pElement;

	pElement=NEWSTRUCT(CH_CHANNELELEMENT);
	if (pElement!=NULL) 
	{
		pElement->iType=Ch_Type_channel;
		pElement->pNextElement=NULL;

		pElement->iMaxSpace=0;
		pElement->pbBase=NULL;
		pElement->pwchEventID=NULL;
		pElement->pwchChannelID=NULL;
		pElement->pbSuccess=NULL;
		pElement->pbFailure=NULL;
		pElement->pContent=NULL;
		pElement->fUserAccessible=FALSE;
	}
	return pElement;
}

/*========================================================================
	Ch_CreateTITLE
==========================================================================*/
pCH_TITLEELEMENT Ch_CreateTITLE (void) 
{
	pCH_TITLEELEMENT pElement;

	pElement=NEWSTRUCT(CH_TITLEELEMENT);
	if (pElement!=NULL) 
	{
		pElement->iType=Ch_Type_title;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
	}
	return pElement;
}

/*========================================================================
	Ch_CreateABSTRACT
==========================================================================*/
pCH_ABSTRACTELEMENT Ch_CreateABSTRACT (void) 
{
	pCH_ABSTRACTELEMENT pElement;

	pElement=NEWSTRUCT(CH_ABSTRACTELEMENT);
	if (pElement!=NULL) 
	{
		pElement->iType=Ch_Type_abstract;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
	}
	return pElement;
}

/*========================================================================
	Ch_CreateRESOURCE
==========================================================================*/
pCH_RESOURCEELEMENT Ch_CreateRESOURCE (void) 
{
	pCH_RESOURCEELEMENT pElement;

	pElement=NEWSTRUCT(CH_RESOURCEELEMENT);
	if (pElement!=NULL) 
	{
		pElement->iType=Ch_Type_resource;
		pElement->pNextElement=NULL;

		pElement->pbHref=NULL;
		pElement->iLastMod=NULL;
		pElement->pbEtag=NULL;
		pElement->pwchMd5=NULL;
	}
	return pElement;
}


/*========================================================================

	Implemented function pointers for Channel elements

=========================================================================*/

/*========================================================================
	Ch_CreateElement
==========================================================================*/
pELEMENTTYPE Ch_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pvNewElm=NULL;

	pDecStr=pDecStr;

	/* Create specific element */
	switch (iType) 
	{
	case Ch_Type_channel:
		pvNewElm=Ch_CreateCHANNEL();
		break;
	case Ch_Type_title:
		pvNewElm=Ch_CreateTITLE();
		break;
	case Ch_Type_abstract:
		pvNewElm=Ch_CreateABSTRACT();
		break;
	case Ch_Type_resource:
		pvNewElm=Ch_CreateRESOURCE();
		break;
	}

	return pvNewElm;
}


/*========================================================================
	Ch_DeleteElement
==========================================================================*/
void Ch_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Ch_Type_channel:
			DEALLOC(&(((CH_CHANNELELEMENT *)(*ppElement))->pbBase));
			DEALLOC(&(((CH_CHANNELELEMENT *)(*ppElement))->pwchEventID));
			DEALLOC(&(((CH_CHANNELELEMENT *)(*ppElement))->pwchChannelID));
			DEALLOC(&(((CH_CHANNELELEMENT *)(*ppElement))->pbSuccess));
			DEALLOC(&(((CH_CHANNELELEMENT *)(*ppElement))->pbFailure));
			XML_DeleteElementList(&(((CH_CHANNELELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Ch_Type_title:
			XML_DeleteElementList(&(((CH_TITLEELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Ch_Type_abstract:
			XML_DeleteElementList(&(((CH_ABSTRACTELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Ch_Type_resource:
			DEALLOC(&(((CH_RESOURCEELEMENT*)(*ppElement))->pbHref));
			DEALLOC(&(((CH_RESOURCEELEMENT*)(*ppElement))->pbEtag));
			DEALLOC(&(((CH_RESOURCEELEMENT*)(*ppElement))->pwchMd5));
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}


/*========================================================================
	Ch_ValidContent
==========================================================================*/
BOOL Ch_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Ch_Type_channel:
		case Ch_Type_title:
		case Ch_Type_abstract:

			return TRUE;
		}
	}

	return FALSE;
}


/*========================================================================
	Ch_GetContent
==========================================================================*/
pELEMENTTYPE Ch_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Ch_Type_channel:
		case Ch_Type_title:
		case Ch_Type_abstract:

			/* Return content using generic function */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}


/*========================================================================

	Attribute handling functions for the channel elements.

==========================================================================*/

/* Converts from text to integer and removes the text. */
BOOL Ch_ConvertNumber32 (WCHAR** ppwchNumber, INT32* piResult) 
{
	INT32 iResult=0;
	UINT32 iCount=0;
	WCHAR *pchNumberTemp=*ppwchNumber;
	BOOL fNeg=FALSE;

	if (pchNumberTemp!=NULL) 
	{
		/* Check if first character is '-' */
		if (pchNumberTemp[iCount]==(WCHAR)('-'))
		{
			fNeg=TRUE;
			iCount++;
		}

		/* Get the integer */
		while (pchNumberTemp[iCount]!=(WCHAR)0) 
		{
			/* Check if between '0' and '9' */
			if ((pchNumberTemp[iCount] < 0x100) && wae_isdigit(pchNumberTemp[iCount]))
			{
				iResult=(iResult*10)+(UINT8)(pchNumberTemp[iCount]-(WC('0')));
			}
			else 
			{
				DEALLOC(&ppwchNumber);
				return FALSE;
			}
			iCount++;
		}

		DEALLOC(ppwchNumber);

		/* Finish */
		if (fNeg)
		{
			iResult=(-iResult);
		}

		*piResult=iResult;

		return TRUE;
	}
	return FALSE;
}


BOOL Ch_SetMaxspace (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppMaxspace)
{
	INT32 iMaxspace=0;

	/* Convert maxspace (text) to INT32. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT32 function. */
	WCHAR* pwchMaxspace=XML_ConvertToString(ppMaxspace,pDecStr);

	if (Ch_ConvertNumber32(&pwchMaxspace,&iMaxspace))
	{
		if (pElementObj->iType==Ch_Type_channel) 
		{
			((CH_CHANNELELEMENT *)(pElementObj))->iMaxSpace=iMaxspace;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL Ch_SetBase	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppBase)
{
	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (((CH_CHANNELELEMENT *)(pElementObj))->pbBase==NULL)
		{
			/* Get Text in WCHAR */
			WCHAR* pwchText=XML_ConvertToString(ppBase,pDecStr);
			BOOL fOverflow=FALSE;

			if (pwchText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((CH_CHANNELELEMENT *)(pElementObj))->pbBase=
								(wip_wchar2byte(pwchText,&fOverflow));

				DEALLOC(&pwchText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}


BOOL Ch_SetEventId (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppEventId)
{
	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (((CH_CHANNELELEMENT *)(pElementObj))->pwchEventID==NULL) 
		{
			((CH_CHANNELELEMENT *)(pElementObj))->
							pwchEventID=XML_ConvertToString(ppEventId,pDecStr);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Ch_SetChannelID (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppChannelId)
{
	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (((CH_CHANNELELEMENT *)(pElementObj))->pwchChannelID==NULL) 
		{
			((CH_CHANNELELEMENT *)(pElementObj))->
							pwchChannelID=XML_ConvertToString(ppChannelId,pDecStr);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Ch_SetHref (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppHref)
{
	if (pElementObj->iType==Ch_Type_resource) 
	{
		if (((CH_RESOURCEELEMENT *)(pElementObj))->pbHref==NULL)
		{
			/* Get Text in WCHAR */
			WCHAR* pwchText=XML_ConvertToString(ppHref,pDecStr);
			BOOL fOverflow=FALSE;

			if (pwchText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((CH_RESOURCEELEMENT *)(pElementObj))->pbHref=
								(wip_wchar2byte(pwchText,&fOverflow));

				DEALLOC(&pwchText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}


BOOL Ch_SetLastmod (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppLastmod)
{
	UINT32 iLastmod=0;

	/* Convert lastmod (text) to UINT32. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT32 function. */
	WCHAR* pwchLastmod=XML_ConvertToString(ppLastmod,pDecStr);

	if (Convert_StringToUINT32(&pwchLastmod,&iLastmod))
	{
		if (pElementObj->iType==Ch_Type_resource) 
		{
			((CH_RESOURCEELEMENT*)(pElementObj))->iLastMod=iLastmod;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Ch_SetEtag (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppEtag)
{
	if (pElementObj->iType==Ch_Type_resource) 
	{
		if (((CH_RESOURCEELEMENT *)(pElementObj))->pbEtag==NULL) 
		{
			/* Get Text in WCHAR */
			WCHAR* pwchText=XML_ConvertToString(ppEtag,pDecStr);
			BOOL fOverflow=FALSE;

			if (pwchText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((CH_RESOURCEELEMENT *)(pElementObj))->pbEtag=
								(wip_wchar2byte(pwchText,&fOverflow));

				DEALLOC(&pwchText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}


BOOL Ch_SetMd5 (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppMd5)
{
	if (pElementObj->iType==Ch_Type_resource) 
	{
		if (((CH_RESOURCEELEMENT *)(pElementObj))->pwchMd5==NULL) 
		{
			((CH_RESOURCEELEMENT *)(pElementObj))->
								pwchMd5=XML_ConvertToString(ppMd5,pDecStr);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Ch_SetSuccess (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppSuccess)
{
	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (((CH_CHANNELELEMENT *)(pElementObj))->pbSuccess==NULL)
		{
			/* Get Text in WCHAR */
			WCHAR* pwchText=XML_ConvertToString(ppSuccess,pDecStr);
			BOOL fOverflow=FALSE;

			if (pwchText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((CH_CHANNELELEMENT *)(pElementObj))->pbSuccess=
								(wip_wchar2byte(pwchText,&fOverflow));

				DEALLOC(&pwchText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}


BOOL Ch_SetFailure (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppFailure)
{
	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (((CH_CHANNELELEMENT *)(pElementObj))->pbFailure==NULL)
		{
			/* Get Text in WCHAR */
			WCHAR* pwchText=XML_ConvertToString(ppFailure,pDecStr);
			BOOL fOverflow=FALSE;

			if (pwchText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((CH_CHANNELELEMENT *)(pElementObj))->pbFailure=
								(wip_wchar2byte(pwchText,&fOverflow));

				DEALLOC(&pwchText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}


BOOL Ch_SetUserAccessible	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppUserAcc)
{
	BOOL fResult=FALSE;

	if (pElementObj->iType==Ch_Type_channel) 
	{
		if (*ppUserAcc!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppUserAcc,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((CH_CHANNELELEMENT *)(pElementObj))->fUserAccessible=fResult;
		return TRUE;
	} 

	return FALSE;
}