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

	WAP PUSH Implementation Project

==========================================================================

	File: PUSHElm.c

	Description:

	Functions for handling the different elements in the PUSH speci-
	fications, i.e., Service Indication (SI), Service Loading (SL), and
	Cache Operation (CO)

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991112    JPR   First version
	991116	  JPR	Attribute functions added

	000131	  JPR	Changes to support generic WBXML-decoder
	000207	  JPR	Corrections
	000208	  JPR	Function Si_DateTime2Int added
	000211	  JPR	Correction in functions Si_SetCreated and Si_SetExpires

==========================================================================*/

#include "pushelm.h"
#include "pushdef.h"
#include "xml_conv.h"

/*========================================================================

	Private functions 

=========================================================================*/

/*========================================================================
	SI_CreateSI
==========================================================================*/
pSI_SIELEMENT Si_CreateSI (void) 
{
	pSI_SIELEMENT pElement=NEWSTRUCT(SI_SIELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Si_Type_si;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
	}
	return pElement;
}

/*========================================================================
	SI_CreateINDICATION
==========================================================================*/
pSI_INDICATIONELEMENT Si_CreateINDICATION (void) 
{
	pSI_INDICATIONELEMENT pElement=NEWSTRUCT(SI_INDICATIONELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Si_Type_indication;
		pElement->pNextElement=NULL;

		pElement->iAction=SI_SIGNAL_MEDIUM;
		pElement->iCreated=0;
		pElement->iExpires=0;
		pElement->pbHref=NULL;
		pElement->pContent=NULL;
		pElement->pwchSiId=NULL;
	}
	return pElement;
}

/*========================================================================
	SI_CreateINFO
==========================================================================*/
pSI_INFOELEMENT Si_CreateINFO (void) 
{
	pSI_INFOELEMENT pElement=NEWSTRUCT(SI_INFOELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Si_Type_info;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
	}
	return pElement;
}

/*========================================================================
	SI_CreateITEM
==========================================================================*/
pSI_ITEMELEMENT Si_CreateITEM (void) 
{
	pSI_ITEMELEMENT pElement=NEWSTRUCT(SI_ITEMELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Si_Type_item;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
		pElement->pwchClass=NULL;
	}
	return pElement;
}


/*========================================================================
	SL_CreateSL
==========================================================================*/
pSL_SLELEMENT Sl_CreateSL (void) 
{
	pSL_SLELEMENT pElement=NEWSTRUCT(SL_SLELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Sl_Type_sl;
		pElement->pNextElement=NULL;

		pElement->iAction=SL_EXECUTE_LOW;
		pElement->pbHref=NULL;
	}
	return pElement;
}


/*========================================================================
	Co_CreateCO
==========================================================================*/
pCO_COELEMENT Co_CreateCO (void) 
{
	pCO_COELEMENT pElement=NEWSTRUCT(CO_COELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Co_Type_co;
		pElement->pNextElement=NULL;

		pElement->pContent=NULL;
	}
	return pElement;
}


/*========================================================================
	Co_CreateINVOBJ
==========================================================================*/
pCO_INVOBJELEMENT Co_CreateINVOBJ (void) 
{
	pCO_INVOBJELEMENT pElement=NEWSTRUCT(CO_INVOBJELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Co_Type_invalidateobject;
		pElement->pNextElement=NULL;

		pElement->pbURI=NULL;
	}
	return pElement;
}


/*========================================================================
	Co_CreateINVSERV
==========================================================================*/
pCO_INVSERVELEMENT Co_CreateINVSERV (void) 
{
	pCO_INVSERVELEMENT pElement=NEWSTRUCT(CO_INVSERVELEMENT);

	if (pElement!=NULL) 
	{
		pElement->iType=Co_Type_invalidateservice;
		pElement->pNextElement=NULL;

		pElement->pbURI=NULL;
	}
	return pElement;
}





/*========================================================================

	Functions for handling the Service Indication (SI), Service Loading 
	(SL), and Cache Operation (CO) elements.

=========================================================================*/

/*========================================================================
	Si_CreateElement
	Sl_CreateElement
	Co_CreateElement
==========================================================================*/
pELEMENTTYPE Si_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pvNewElm=NULL;

	pDecStr=pDecStr;

	/* Create specific element */
	switch (iType)
	{
	case Si_Type_si:
		pvNewElm=Si_CreateSI();
		break;
	case Si_Type_indication:
		pvNewElm=Si_CreateINDICATION();
		break;
	case Si_Type_info:
		pvNewElm=Si_CreateINFO();
		break;
	case Si_Type_item:
		pvNewElm=Si_CreateITEM();
		break;
	}

	return pvNewElm;
}

pELEMENTTYPE Sl_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pvNewElm=NULL;

	pDecStr=pDecStr;
	switch (iType) 
	{
	case Sl_Type_sl:
		pvNewElm=Sl_CreateSL();
		break;
	}

	return pvNewElm;
}

pELEMENTTYPE Co_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pvNewElm=NULL;

	pDecStr=pDecStr;
	switch (iType) 
	{
	case Co_Type_co:
		pvNewElm=Co_CreateCO();
		break;
	case Co_Type_invalidateobject:
		pvNewElm=Co_CreateINVOBJ();
		break;
	case Co_Type_invalidateservice:
		pvNewElm=Co_CreateINVSERV();
		break;
	}

	return pvNewElm;
}


/*========================================================================
	Si_DeleteElement
	Sl_DeleteElement
	Co_DeleteElement
==========================================================================*/
void Si_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Si_Type_si:
			XML_DeleteElementList(&(((SI_SIELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Si_Type_indication:
			DEALLOC(&(((SI_INDICATIONELEMENT*)(*ppElement))->pbHref));
			DEALLOC(&(((SI_INDICATIONELEMENT*)(*ppElement))->pwchSiId));
			XML_DeleteElementList(&(((SI_INDICATIONELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Si_Type_info:
			XML_DeleteElementList(&(((SI_INFOELEMENT *)(*ppElement))->pContent),pDecStr);
			break;
		case Si_Type_item:
			DEALLOC(&(((SI_ITEMELEMENT *)(*ppElement))->pwchClass));
			XML_DeleteElementList(&(((SI_ITEMELEMENT *)(*ppElement))->pContent),pDecStr);
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}

void Sl_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	pDecStr=pDecStr;
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Sl_Type_sl:
			DEALLOC(&(((SL_SLELEMENT *)(*ppElement))->pbHref));
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}
	
void Co_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Co_Type_co:
			XML_DeleteElementList(&(((CO_COELEMENT *)(*ppElement))->pContent),pDecStr);
			break;
		case Co_Type_invalidateobject:
			DEALLOC(&(((CO_INVOBJELEMENT *)(*ppElement))->pbURI));
			break;
		case Co_Type_invalidateservice:
			DEALLOC(&(((CO_INVSERVELEMENT *)(*ppElement))->pbURI));
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}


/*========================================================================
	Si_AddContent
	Sl_AddContent
	Co_AddContent
==========================================================================*/
BOOL Si_AddContent (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Si_Type_si:
		case Si_Type_indication:
		case Si_Type_info:
		case Si_Type_item:

			/* Add content using generic function */
			XML_AddContent(pElement,ppContent);

			return TRUE;
		}
	}

	/* Content is NOT stored */
	return FALSE;
}


BOOL Sl_AddContent (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Sl_Type_sl:

			/* Add content using generic function */
			XML_AddContent(pElement,ppContent);

			return TRUE;
		}
	}

	/* Content is NOT stored */
	return FALSE;

}


BOOL Co_AddContent (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Co_Type_co:

			/* Add content using generic function */
			XML_AddContent(pElement,ppContent);

			return TRUE;
		}
	}

	/* Content is NOT stored */
	return FALSE;

}


/*========================================================================
	Si_GetContent
	Sl_GetContent
	Co_GetContent
==========================================================================*/
pELEMENTTYPE Si_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Si_Type_si:
		case Si_Type_indication:
		case Si_Type_info:
		case Si_Type_item:

			/* Return content using generic function */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}

pELEMENTTYPE Sl_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Sl_Type_sl:

			/* Return content using generic function */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}

pELEMENTTYPE Co_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Co_Type_co:

			/* Return content using generic function */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}

/*========================================================================

	Push specific convertion functions for the attributes.

==========================================================================*/
BOOL PUSH_ConvertSiAction (WCHAR** ppwchAction, UINT8* piResult)
{
	WCHAR pwchPushAction_none[]={'s','i','g','n','a','l','-','n','o','n','e',0};
	WCHAR pwchPushAction_low[]={'s','i','g','n','a','l','-','l','o','w',0};
	WCHAR pwchPushAction_medium[]={'s','i','g','n','a','l','-','m','e','d','i','u','m',0};
	WCHAR pwchPushAction_high[]={'s','i','g','n','a','l','-','h','i','g','h',0};
	WCHAR pwchPushAction_delete[]={'d','e','l','e','t','e',0};
	BOOL fResult=FALSE;

	/* ASSERT: ppwchAction!=NULL
	*/

	if (*ppwchAction!=NULL)
	{
		if (COMPARESTRING(*ppwchAction,pwchPushAction_none)==0) 
		{
			*piResult=SI_SIGNAL_NONE;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_low)==0) 
		{
			*piResult=SI_SIGNAL_LOW;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_medium)==0) 
		{
			*piResult=SI_SIGNAL_MEDIUM;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_high)==0) 
		{
			*piResult=SI_SIGNAL_HIGH;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_delete)==0) 
		{
			*piResult=SI_DELETE;
			fResult=TRUE;
		}
	}

	DEALLOC(ppwchAction);
	return fResult;
}


BOOL PUSH_ConvertSlAction (WCHAR** ppwchAction, UINT8* piResult)
{
	WCHAR pwchPushAction_low[]={'e','x','e','c','u','t','e','-','l','o','w',0};
	WCHAR pwchPushAction_high[]={'e','x','e','c','u','t','e','-','h','i','g','h',0};
	WCHAR pwchPushAction_cache[]={'c','a','c','h','e',0};
	BOOL fResult=FALSE;

	/* ASSERT: ppwchAction!=NULL
	*/

	if (*ppwchAction!=NULL)
	{
		if (COMPARESTRING(*ppwchAction,pwchPushAction_low)==0) 
		{
			*piResult=SL_EXECUTE_LOW;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_high)==0) 
		{
			*piResult=SL_EXECUTE_HIGH;
			fResult=TRUE;
		} 
		else if (COMPARESTRING(*ppwchAction,pwchPushAction_cache)==0) 
		{
			*piResult=SL_CACHE;
			fResult=TRUE;
		} 
	}

	DEALLOC(ppwchAction);
	return fResult;
}

/*========================================================================

	Attribute handling functions for the PUSH elements.

==========================================================================*/

/*========================================================================
	Si_DateTime2Int
==========================================================================
Purpose: Convert from Date Time format to an UINT32.
Params:  pbDateTime, iDateTimeLen
Return:  UINT32 in seconds counting from 1/1 1970.
=========================================================================*/
UINT32 Si_DateTime2Int (BYTE* pbDateTime,UINT32 iDateTimeLen)
{

	UINT32 iYears=0;
	UINT32 iMonths=0;
	UINT32 iDays=0;
	UINT32 iHours=0;
	UINT32 iMinutes=0;
	UINT32 iSeconds=0;
	BYTE*  pbArr;
	UINT8  idx;

	pbArr=NEWARRAY(BYTE,8);
	if (pbArr==NULL)
	{
		return 0;
	}
	for (idx=0;idx<8;idx++)
	{
		pbArr[idx]=0;
	}
	for (idx=0;idx < iDateTimeLen;idx++)
	{
		pbArr[idx]=pbDateTime[idx];
	}

	/* Retrieve year */
	iYears=( ((pbArr[0]&0xf0)/16)*10 + (pbArr[0]&0x0f) )*100 + 
		((pbArr[1]&0xf0)/16)*10 + (pbArr[1]&0x0f) - 1970;
	/* Retrieve month */
	iMonths=((pbArr[2]&0xf0)/16)*10 + (pbArr[2]&0x0f);
	/* Retrieve day */
	iDays=((pbArr[3]&0xf0)/16)*10 + (pbArr[3]&0x0f);
	/* Retrieve hours */
	iHours=((pbArr[4]&0xf0)/16)*10 + (pbArr[4]&0x0f);
	/* Retrieve minutes */
	iMinutes=((pbArr[5]&0xf0)/16)*10 + (pbArr[5]&0x0f);
	/* Retrieve seconds */
	iSeconds=((pbArr[6]&0xf0)/16)*10 + (pbArr[6]&0x0f);

	DEALLOC(&pbArr);

	/* Calculate in seconds from 1/1 1970 */

	switch (iMonths)
	{
	case 1:iMonths=0;
		break;
	case 2:iMonths=2678400;   /* 31*24*60*60 */ 
		break;
	case 3:iMonths=5097600;   /* (31+28)*24*60*60 */
		break;
	case 4:iMonths=7776000;   /* (31+28+31)*24*60*60 */
		break;
	case 5:iMonths=10368000;  /* (31+28+31+30)*24*60*60 */
		break;
	case 6:iMonths=13046400;  /* (31+28+31+30+31)*24*60*60 */
		break;
	case 7:iMonths=15638400;  /* (31+28+31+30+31+30)*24*60*60 */
		break;
	case 8:iMonths=18316800;  /* (31+28+31+30+31+30+31)*24*60*60 */
		break;
	case 9:iMonths=20995200;  /* (31+28+31+30+31+30+31+31)*24*60*60 */
		break;
	case 10:iMonths=23587200; /* (31+28+31+30+31+30+31+31+30)*24*60*60 */
		break;
	case 11:iMonths=26265600; /* (31+28+31+30+31+30+31+31+30+31)*24*60*60 */
		break;
	case 12:iMonths=28857600; /* (31+28+31+30+31+30+31+31+30+31+30)*24*60*60 */
		break;
	}
	
	return iYears*(365*86400+20040) + iMonths + ((iDays-1)*86400) + iHours*3600 + iMinutes*60 + iSeconds;

} /* end of ME_Push_DateTime2Int */


/*========================================================================
	Setting push attributes (attribute name)
==========================================================================
	The following functions sets the attribute values in the elements. If
	the attribute field and the attribute value is valid for the element 
	it is and stored and TRUE is returned. Otherwise, FALSE is returned. 

	NOTE! It is the callers responsibility to deallocate all parts NOT 
	used by the functions. For example, if ppAction is not used in the 
	function Si_SetAction, this memory must be deallocated.
	
	Input: Element (NOT NULL) and attribute value.						
	Output: BOOL
==========================================================================*/

/*========================================================================
	Service Indication Attribute Functions
==========================================================================*/
BOOL Si_SetAction (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppAction, UINT8 iAction)
{
	if (pElementObj->iType==Si_Type_indication)
	{
		if (*ppAction!=NULL)
		{
			UINT8 iTempAction;

			/* Convert to string */
			WCHAR* pwchTemp=XML_ConvertToString(ppAction,pDecStr);

			/* Convert string to action */
			if (PUSH_ConvertSiAction (&pwchTemp,&iTempAction))
			{
				iAction=iTempAction;
			}
			else
			{
				/* Error in convertion */
				return FALSE;
			}
		}

		((SI_INDICATIONELEMENT *)(pElementObj))->iAction=iAction;
		return TRUE;
	}
	return FALSE;
}

BOOL Si_SetCreated (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppCreated)
{
	/* %Datetime (OPAQUE data) */
	if ((*ppCreated!=NULL)&&(*ppCreated)->iType==Type_Opaque)
	{
		/* Convert to UINT32 */
		UINT32 iCreated=Si_DateTime2Int (((pXMLOPAQUE)
				(*ppCreated))->pbData,((pXMLOPAQUE)(*ppCreated))->iLength);

		/* Delete attribute */
		XML_DeleteElementList (ppCreated,pDecStr);

		if (pElementObj->iType==Si_Type_indication) 
		{
			((SI_INDICATIONELEMENT *)(pElementObj))->iCreated=iCreated;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Si_SetHref (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppHref)
{
	if (pElementObj->iType==Si_Type_indication) 
	{
		if (((SI_INDICATIONELEMENT *)(pElementObj))->pbHref==NULL) 
		{
			BOOL fOverflow=FALSE;
			WCHAR *pchTempText=XML_ConvertToString(ppHref,pDecStr);

			if (pchTempText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((SI_INDICATIONELEMENT *)(pElementObj))->pbHref=
								(wip_wchar2byte(pchTempText,&fOverflow));

				DEALLOC(&pchTempText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}

BOOL Si_SetExpires (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppExpires)
{
	/* %Datetime (OPAQUE data) */
	if ((*ppExpires!=NULL)&&(*ppExpires)->iType==Type_Opaque)
	{
		/* Convert to UINT32 */
		UINT32 iExpires=Si_DateTime2Int (((pXMLOPAQUE)
				(*ppExpires))->pbData,((pXMLOPAQUE)(*ppExpires))->iLength);

		/* Delete attribute */
		XML_DeleteElementList (ppExpires,pDecStr);

		if (pElementObj->iType==Si_Type_indication) 
		{
			((SI_INDICATIONELEMENT *)(pElementObj))->iExpires=iExpires;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Si_SetSiId (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppSiId)
{
	if (pElementObj->iType==Si_Type_indication) 
	{
		if (((SI_INDICATIONELEMENT *)(pElementObj))->pwchSiId==NULL) 
		{
			/* Get Text in WCHAR */
			((SI_INDICATIONELEMENT *)(pElementObj))
						->pwchSiId=XML_ConvertToString(ppSiId,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL Si_SetClass (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppClass)
{
	if (pElementObj->iType==Si_Type_item) 
	{
		if (((SI_ITEMELEMENT *)(pElementObj))->pwchClass==NULL) 
		{
			/* Get Text in WCHAR */
			((SI_ITEMELEMENT *)(pElementObj))
						->pwchClass=XML_ConvertToString(ppClass,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}



/*========================================================================
	Service Loading Attribute Functions
==========================================================================*/

BOOL Sl_SetAction (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppAction, UINT8 iAction)
{
	if (pElementObj->iType==Sl_Type_sl)
	{
		if (*ppAction!=NULL)
		{
			UINT8 iTempAction;

			/* Convert to string */
			WCHAR* pwchTemp=XML_ConvertToString(ppAction,pDecStr);

			/* Convert string to action */
			if (PUSH_ConvertSlAction (&pwchTemp,&iTempAction))
			{
				iAction=iTempAction;
			}
			else
			{
				/* Error in convertion */
				return FALSE;
			}
		}

		((SL_SLELEMENT*)(pElementObj))->iAction=iAction;
		return TRUE;
	}
	return FALSE;
}

BOOL Sl_SetHref	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppHref)
{
	if (pElementObj->iType==Sl_Type_sl) 
	{
		if (((SL_SLELEMENT*)(pElementObj))->pbHref==NULL) 
		{
			BOOL fOverflow=FALSE;
			WCHAR *pchTempText=XML_ConvertToString(ppHref,pDecStr);

			if (pchTempText!=NULL)
			{
				/* Convert WCHAR to BYTE */
				((SL_SLELEMENT*)(pElementObj))->pbHref=
								(wip_wchar2byte(pchTempText,&fOverflow));

				DEALLOC(&pchTempText);

				return (!fOverflow);
			}
		}
	}
	return FALSE;
}



/*========================================================================
	Cache Operation Attribute Functions
==========================================================================*/

BOOL Co_SetURI (pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppURI)
{
	BOOL fOverflow=FALSE;
	BYTE* pbTemp=NULL;
	WCHAR *pchTempText=XML_ConvertToString(ppURI,pDecStr);

	if (pchTempText!=NULL)
	{
		/* Convert WCHAR to BYTE */
		pbTemp=wip_wchar2byte(pchTempText,&fOverflow);
		DEALLOC(&pchTempText);

		if ((pbTemp!=NULL)&&(!fOverflow))
		{
			/* Store in element */
			if (pElementObj->iType==Co_Type_invalidateobject) 
			{
				if (((CO_INVOBJELEMENT *)(pElementObj))->pbURI==NULL) 
				{
					((CO_INVOBJELEMENT *)(pElementObj))->pbURI=pbTemp;
					return TRUE;
				}
			}
			else if (pElementObj->iType==Co_Type_invalidateservice) 
			{
				if (((CO_INVSERVELEMENT *)(pElementObj))->pbURI==NULL) 
				{
					((CO_INVSERVELEMENT *)(pElementObj))->pbURI=pbTemp;
					return TRUE;
				}
			}

			DEALLOC(&pbTemp);
		}
	}
	return FALSE;
}
