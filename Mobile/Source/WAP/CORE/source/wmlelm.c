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
	FILE: WMLElm.c
  
	Source for WML-specific element handler.

	Rev history:
		980706	JPR		Created
		981223	JPR		Event concept changed	
		990303	JPR		Corrections		
		990316	JPR		Major changes. Handles the WML-specific 
						elements. Variable handling moved to 
						WMLIF.c
		990317	JPR		Corrections
		990325	JPR		WAP 1.1
		990412	JPR		Uses Url.h instead of Url_lib.h
		990421	JPR		Corr. in WML_CreateINPUT and WML_SetFormat
		990428	JPR		Support for LITERALS added
		990526	JPR		Paragraph updates
		990610	JPR		Removal of unreferenced variables
		991026	JPR		WTA specific parts added

		991202	JPR		Changes to support new generic WBXML-decoder
		991208	JPR		Minor corrections
		991223	JPR		fLastOption in OPTIONELEMENT removed
		000104	JPR		xml:lang, xml:space and pre element added	
		000113	JPR		Correction in WML_CreateP
		000131	JPR		Functions WML_AddContent and WML_GetContent 
						updated.
		000201	JPR		Corrections in Set-functions for attributes and
						in WML_CreateINPUT, WML_CreateBR created
		000210	JPR		Correction in function WML_CreateElement
		000225	JPR		Function WML_SetXmlSpace updated
		000308	JPR		Corrections in WTA Event handling 
		000419	JPR		Functions WML_SetWidth and WML_SetHeight corrected
						Default value "unknown" added for attribute "type" 
						in do element
		000809	JPR		Addition of "cache-control" attribute in go

==========================================================================*/


#include "url.h"
#include "cmmnrsrc.h"
#include "wmldef.h"
#include "wmltoken.h"
#include "wmlevent.h"
#include "wmlelm.h" 
#include "waectype.h"
#include "wmlparse.h"
#include "wbxmlelm.h"
#include "wipprdct.h"



/*========================================================================
	WML_ConvertEventToInt
==========================================================================*/
INT8 WML_ConvertEventToInt (WCHAR** ppchType)
{
	const WCHAR pchConst_OnEnterForward[]={'o','n','e','n','t','e','r','f','o','r','w','a','r','d',0};
	const WCHAR pchConst_OnEnterBackward[]={'o','n','e','n','t','e','r','b','a','c','k','w','a','r','d',0};
	const WCHAR pchConst_OnTimer[]={'o','n','t','i','m','e','r',0};
	const WCHAR pchConst_OnClick[]={'o','n','p','i','c','k',0};

	if (*ppchType!=NULL) 
	{
		if (COMPARESTRING(*ppchType,pchConst_OnEnterForward)==0) 
		{
			/* DEALLOC must be on pchType, not on &pchType since
			   the parameter is **pchType. */
			DEALLOC(ppchType);
			return DEF_TYPE_ONENTERFORWARD;
		} 
		else if (COMPARESTRING(*ppchType,pchConst_OnEnterBackward)==0) 
		{
			DEALLOC(ppchType);
			return DEF_TYPE_ONENTERBACKWARD;
		} 
		else if (COMPARESTRING(*ppchType,pchConst_OnTimer)==0) 
		{
			DEALLOC(ppchType);
			return DEF_TYPE_ONTIMER;
		} 
		else if (COMPARESTRING(*ppchType,pchConst_OnClick)==0) 
		{
			DEALLOC(ppchType);
			return DEF_TYPE_ONCLICK;
		}
		else
		{
			DEALLOC(ppchType);
		}
	}

	return DEF_TYPE_UNKNOWN;
}


/*========================================================================
	WML_ConvertEventToInt
==========================================================================*/
INT8 WML_ConvertInputToInt (WCHAR** ppchType)
{
	const WCHAR pchConst_Text[]={'t','e','x','t',0};
	const WCHAR pchConst_Password[]={'p','a','s','s','w','o','r','d',0};

	if (*ppchType!=NULL) 
	{
		if (COMPARESTRING(*ppchType,pchConst_Text)==0) 
		{
			DEALLOC(ppchType);
			return DEF_TYPE_TEXT;
		} 
		else if (COMPARESTRING(*ppchType,pchConst_Password)==0) 
		{
			DEALLOC(ppchType);
			return DEF_TYPE_PASSWORD;
		} 
		else
		{
			DEALLOC(ppchType);
		}
	}

	return DEF_TYPE_UNKNOWN;
}


/* WTA specific */
#ifdef CONFIG_WTA
/*========================================================================
	WTA_ConvertEventToInt
==========================================================================*/
INT8 WTA_ConvertEventToInt (WCHAR *pwchType)
{
	INT8 iResult=DEF_TYPE_UNKNOWN;
	INT16 iIndex=-1;
	UINT16 iLength=0;
	WCHAR *pwchTemp=pwchType;
	const WCHAR pwchWtaev[]={'w','t','a','e','v','-',0};

	if (pwchType!=NULL) 
	{
		/* Remove 'wtaev-' from type */

		/* Get length of pwchPId string */
		iLength=(UINT16) (w_strlen(pwchType));

		/* Length must be greater than 6 */
		if ( iLength > 6 )
		{
			if (COMPARESTRINGN(pwchWtaev,pwchType,6)==0)
			{
				/* String starts with 'wtaev-' -> step 6 WCHARs */
				pwchTemp+=6;
			}

			/* Compare (Network common) */
			iIndex=FindStringBinarySearch(pwchTemp,ppwchWTAEvents,STAT_NBROFWTAEVENTS);

			if (iIndex>=0)
			{
				iResult=pbWTAEventBytes[iIndex];
			}


			/* Compare (Network GSM) */
			#ifdef CONFIG_WTAI_GSM

			iIndex=FindStringBinarySearch(pwchTemp,ppwchGSMWTAEvents,STAT_NBROFGSMEVENTS);

			if (iIndex>=0)
			{
				iResult=pbGSMWTAEventBytes[iIndex];
			}

			#endif

			
			/* No events defined for CONFIG_WTAI_PDC */
		

			/* Compare (Network IS 136) */
			#ifdef CONFIG_WTAI_IS_136

			iIndex=FindStringBinarySearch(pwchTemp,ppwchIS136WTAEvents,STAT_NBROFIS136EVENTS);

			if (iIndex>=0)
			{
				iResult=pbIS136WTAEventBytes[iIndex];
			}

			#endif	
		}
	}

	return iResult;
}
#endif


/*========================================================================
	WML_ConvertAlign
==========================================================================*/
INT8 WML_ConvertAlign (WCHAR **ppchAlign)
{
	const WCHAR pchConst_left[]={'l','e','f','t',0};
	const WCHAR pchConst_center[]={'c','e','n','t','e','r',0};
	const WCHAR pchConst_right[]={'r','i','g','h','t',0};
	const WCHAR pchConst_bottom[]={'b','o','t','t','o','m',0};
	const WCHAR pchConst_middle[]={'m','i','d','d','l','e',0};
	const WCHAR pchConst_top[]={'t','o','p',0};

	if (*ppchAlign!=NULL) 
	{
		if (COMPARESTRING(*ppchAlign,pchConst_left)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_LEFT;
		} 
		else if (COMPARESTRING(*ppchAlign,pchConst_center)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_CENTER;
		} 
		else if (COMPARESTRING(*ppchAlign,pchConst_right)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_RIGHT;
		} 
		else if (COMPARESTRING(*ppchAlign,pchConst_bottom)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_BOTTOM;
		} 
		else if (COMPARESTRING(*ppchAlign,pchConst_middle)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_MIDDLE;
		} 
		else if (COMPARESTRING(*ppchAlign,pchConst_top)==0) 
		{
			DEALLOC(ppchAlign);
			return ALIGN_TOP;
		} 

		DEALLOC(ppchAlign);
	}
	return DEF_TYPE_UNKNOWN;
}

enum WrapMode WML_ConvertMode (WCHAR *pchMode)
{
	const WCHAR pchConst_wrap[]={'w','r','a','p',0};
	const WCHAR pchConst_Nowrap[]={'n','o','w','r','a','p',0};
	
	if (pchMode!=NULL) 
	{
		if (COMPARESTRING(pchMode,pchConst_wrap)==0) 
		{
			DEALLOC(&pchMode);
			return P_Wrap;
		} 
		else if (COMPARESTRING(pchMode,pchConst_Nowrap)==0) 
		{
			DEALLOC(&pchMode);
			return P_NoWrap;
		} 
		DEALLOC(&pchMode);
	}
	return P_Unknown;
}
	
enum MethodType WML_ConvertMethod (WCHAR *pchMethod)
{
	const WCHAR pchConst_post[]={'p','o','s','t',0};
	
	if (pchMethod!=NULL) 
	{
		if (COMPARESTRING(pchMethod,pchConst_post)==0) 
		{
			DEALLOC(&pchMethod);
			return M_Post;
		} 
		DEALLOC(&pchMethod);
	}
	return M_Get;
}

enum CacheControl WML_ConvertCacheControl (WCHAR **ppwchCache)
{
	const WCHAR pwchConst_nocache[]={'n','o','-','c','a','c','h','e',0};
	
	if (*ppwchCache!=NULL) 
	{
		if (COMPARESTRING(*ppwchCache,pwchConst_nocache)==0) 
		{
			DEALLOC(ppwchCache);
			return Cache_nocache;
		} 
		DEALLOC(ppwchCache);
	}
	return Cache_unknown;
}

UINT8 WML_ConvertEnctype (WCHAR **ppwchEnctype)
{
	const WCHAR pchConst_formdata[]={'m','u','l','t','i','p','a','r','t',
							   '/','f','o','r','m','-','d','a','t','a',0};

	if (*ppwchEnctype!=NULL) 
	{
		if (COMPARESTRING(*ppwchEnctype,pchConst_formdata)==0) 
		{
			DEALLOC(ppwchEnctype);
			return GO_formdata;
		} 
		DEALLOC(ppwchEnctype);
	}
	return GO_urlencoded;
}

/* --------------------------------------------- */



/* ---------------------------------------------------
    Functions for handling the WML specific elements.
   --------------------------------------------------- */


/*========================================================================
	WML_AddTemplateDO
==========================================================================*/
BOOL WML_AddTemplateDO (pELEMENTTYPE pCard, pELEMENTTYPE pDO)
{
	pTEMPLATEDOTYPE pTemp=NULL;
	pTEMPLATEDOTYPE pLast=NULL;
	pTEMPLATEDOTYPE pNew=NULL; 
	pCARDELEMENT pCardElm=NULL;
	BOOL fCont=TRUE;

	/* ASSERT:	pCard!=NULL
				pDO!=NULL
	*/

	pNew=NEWSTRUCT(TEMPLATEDOTYPE);

	pCardElm=(CARDELEMENT*)(pCard);

	if (pCardElm!=NULL)
	{
		if (pNew!=NULL)
		{
			pNew->pDO=pDO;
			pNew->pNext=NULL;

			/* Insert at the end of the list */
			pTemp=pCardElm->pTemplateDO;

			if (pTemp==NULL)
			{
				/* List empty */
				pCardElm->pTemplateDO=pNew;
			}
			else
			{
				while ((pTemp!=NULL)&&(fCont))
				{
					pLast=pTemp;

					if (pLast->pNext==NULL)
					{
						/* Last element */
						pLast->pNext=pNew;
						fCont=FALSE;
					}
					pTemp=pTemp->pNext;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

/* Deletes the TemplateDO list on the card */
void WML_DeleteTemplateDO (pTEMPLATEDOTYPE pTemplateDO)
{
	pTEMPLATEDOTYPE pTemp;

	while (pTemplateDO!=NULL)
	{
		pTemp=pTemplateDO;
		pTemplateDO=pTemplateDO->pNext;
		DEALLOC(&pTemp);
	}
}


/* Used in all b, big, em, i, small, strong and u-elements */
pEMPHELEMENT WML_CreateEMPH (UINT16 iType) 
{
	pEMPHELEMENT pElement=NEWSTRUCT(EMPHELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=iType;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pAELEMENT WML_CreateA (void) 
{
	pAELEMENT pElement=NEWSTRUCT(AELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_A;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pHref=NULL;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;
		pElement->wchAccessKey=0;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pANCHORELEMENT WML_CreateANCHOR (void) 
{
	pANCHORELEMENT pElement=NEWSTRUCT(ANCHORELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_ANCHOR;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;
		pElement->wchAccessKey=0;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pACCESSELEMENT WML_CreateACCESS (void) 
{
	pACCESSELEMENT pElement=NEWSTRUCT(ACCESSELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_ACCESS;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pchDomain=NULL;
		pElement->pchPath=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pBRELEMENT WML_CreateBR (void) 
{
	pBRELEMENT pElement=NEWSTRUCT(BRELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_BR;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pCARDELEMENT WML_CreateCARD (void) 
{
	pCARDELEMENT pElement=NEWSTRUCT(CARDELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_CARD;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pchId=NULL;
		pElement->pTitle=NULL;
		pElement->fNewContext=FALSE;
		pElement->fOrdered=TRUE;
		pElement->pOnEntForw=NULL;
		pElement->pOnEntBack=NULL;
		pElement->pOnTimer=NULL;
		pElement->pContent=NULL;
		pElement->pEventTab=NULL;
		pElement->pTemplateDO=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pDOELEMENT WML_CreateDO (void) 
{
	pDOELEMENT pElement=NEWSTRUCT(DOELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_DO;
		pElement->pNextElement=NULL;
	
		/* Element specific */

		/* Default format is "unknown" */
		pElement->pchType=wip_byte2wchar((BYTE *)"unknown");
		pElement->pLabel=NULL;
		pElement->pchName=NULL;
		pElement->fOptional=FALSE;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;
	
	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pFIELDSETELEMENT WML_CreateFIELDSET (void) 
{
	pFIELDSETELEMENT pElement=NEWSTRUCT(FIELDSETELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_FIELDSET;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pGOELEMENT WML_CreateGO (void) 
{
	pGOELEMENT pElement=NEWSTRUCT(GOELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_GO;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pHref=NULL;
		pElement->fSendRef=FALSE;
		pElement->iMethod=M_Get;
		pElement->pchAccChar=NULL;
		pElement->pContent=NULL;
		pElement->iEnctype=GO_urlencoded;
		pElement->iCacheControl=Cache_unknown;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pHEADELEMENT WML_CreateHEAD (void) 
{
	pHEADELEMENT pElement=NEWSTRUCT(HEADELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_HEAD;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pIMGELEMENT WML_CreateIMG (void) 
{
	pIMGELEMENT pElement=NEWSTRUCT(IMGELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_IMG;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pAlt=NULL;
		pElement->pSrc=NULL;
		pElement->pLocalSrc=NULL;
		pElement->iVSpace=0;
		pElement->iHSpace=0;
		pElement->iIAlign=ALIGN_BOTTOM;
		pElement->iHeight=0;
		pElement->iWidth=0;
		pElement->iIsPercent=NONE_IS_PERCENT;
	    pElement->iMMIElementID=0;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pINPUTELEMENT WML_CreateINPUT (void) 
{
	pINPUTELEMENT pElement=NEWSTRUCT(INPUTELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_INPUT;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pchName=NULL;
		pElement->iInputType=DEF_TYPE_TEXT;
		pElement->pValue=NULL;
		
		/* Default format is *M */
		pElement->pchFormat=wip_byte2wchar((BYTE *)"*M");
		pElement->fEmptyOK=FALSE;
		pElement->iSize=-1;
		pElement->iMaxLength=-1;
		pElement->iTabIndex=0;
		pElement->pTitle=NULL;
		pElement->pwchXmlLang=NULL;
		pElement->wchAccessKey=0;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pMETAELEMENT WML_CreateMETA (void) 
{
	pMETAELEMENT pElement=NEWSTRUCT(METAELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_META;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pchHttpEquiv=NULL;
		pElement->pchName=NULL;
		pElement->fForua=FALSE;
		pElement->pchContent=NULL;
		pElement->pchScheme=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pONEVENTELEMENT WML_CreateONEVENT (void) 
{
	pONEVENTELEMENT pElement=NEWSTRUCT(ONEVENTELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_ONEVENT;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->iEventType=0;
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pOPTGROUPELEMENT WML_CreateOPTGROUP (void) 
{
	pOPTGROUPELEMENT pElement=NEWSTRUCT(OPTGROUPELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_OPTGROUP;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pOPTIONELEMENT WML_CreateOPTION (void) 
{
	pOPTIONELEMENT pElement=NEWSTRUCT(OPTIONELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_OPTION;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pValue=NULL;
		pElement->pTitle=NULL;
		pElement->pOnpick=NULL;
		pElement->pContent=NULL;
		pElement->fSelected=FALSE;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pPELEMENT WML_CreateP (void) 
{
	pPELEMENT pElement=NEWSTRUCT(PELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_P;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->iAlign=ALIGN_LEFT;
		pElement->iMode=P_Unknown;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pPOSTFIELDELEMENT WML_CreatePOSTFIELD (void) 
{
	pPOSTFIELDELEMENT pElement=NEWSTRUCT(POSTFIELDELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_POSTFIELD;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pName=NULL;
		pElement->pValue=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pPREELEMENT WML_CreatePRE (void) 
{
	pPREELEMENT pElement=NEWSTRUCT(PREELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_PRE;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;
		pElement->iXmlSpace=XML_SPACE_PRESERVE;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pPREVELEMENT WML_CreatePREV (void) 
{
	pPREVELEMENT pElement=NEWSTRUCT(PREVELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_PREV;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pREFRESHELEMENT WML_CreateREFRESH (void) 
{
	pREFRESHELEMENT pElement=NEWSTRUCT(REFRESHELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_REFRESH;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pSELECTELEMENT WML_CreateSELECT (void) 
{
	pSELECTELEMENT pElement=NEWSTRUCT(SELECTELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_SELECT;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pchName=NULL;
		pElement->pValue=NULL;
		pElement->pchIName=NULL;
		pElement->pIValue=NULL;
		pElement->fMultiple=FALSE;	
		pElement->iTabIndex=0;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pSETVARELEMENT WML_CreateSETVAR (void) 
{
	pSETVARELEMENT pElement=NEWSTRUCT(SETVARELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_SETVAR;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pName=NULL;
		pElement->pValue=NULL;
	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pTABLEELEMENT WML_CreateTABLE (void) 
{
	pTABLEELEMENT pElement=NEWSTRUCT(TABLEELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_TABLE;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pTitle=NULL;
		pElement->pchAlign=NULL;
		pElement->iColumns=0;
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pTDELEMENT WML_CreateTD (void) 
{
	pTDELEMENT pElement=NEWSTRUCT(TDELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_TD;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;
		pElement->pwchXmlLang=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pTEMPLATEELEMENT WML_CreateTEMPLATE (void) 
{
	pTEMPLATEELEMENT pElement=NEWSTRUCT(TEMPLATEELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_TEMPLATE;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pOnEntForw=NULL;
		pElement->pOnEntBack=NULL;
		pElement->pOnTimer=NULL;
		pElement->pContent=NULL;
		pElement->pEventTab=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pTIMERELEMENT WML_CreateTIMER (void) 
{
	pTIMERELEMENT pElement=NEWSTRUCT(TIMERELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_TIMER;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pchName=NULL;
		pElement->pValue=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pTRELEMENT WML_CreateTR (void) 
{
	pTRELEMENT pElement=NEWSTRUCT(TRELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_TR;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pUNKNOWNELEMENT WML_CreateUNKNOWN (void) 
{
	pUNKNOWNELEMENT pElement=NEWSTRUCT(UNKNOWNELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_UNKNOWN;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

pWMLELEMENT WML_CreateWML (void) 
{
	pWMLELEMENT pElement=NEWSTRUCT(WMLELEMENT);

	#ifndef HAS_SETJMP
	if (pElement!=NULL) 
	{
	#endif

		pElement->iType=Type_WML;
		pElement->pNextElement=NULL;

		/* Element specific */
		pElement->pwchXmlLang=NULL;
		pElement->pContent=NULL;

	#ifndef HAS_SETJMP
	}
	#endif

	return pElement;
}

/*========================================================================

	Implemented function pointers for WML

=========================================================================*/

/*========================================================================
	WML_CreateElement
==========================================================================
	WML-specific implementation of DTD_CreateElementFunc.
==========================================================================*/
pELEMENTTYPE WML_CreateElement (pDECSTR pDecStr, UINT16 iType)
{
	void *pNewElm=NULL;
		
	/* Create specific element */
	switch (iType)
	{
	case Type_A:
		pNewElm=WML_CreateA();
		break;
	case Type_ANCHOR:
		pNewElm=WML_CreateANCHOR();
		break;
	case Type_ACCESS:
		pNewElm=WML_CreateACCESS();
		break;
	case Type_B:
	case Type_BIG:
	case Type_EM:
	case Type_I:
	case Type_SMALL:
	case Type_STRONG:
	case Type_U:
		pNewElm=WML_CreateEMPH(iType);
		break;
	case Type_BR: 
		pNewElm=WML_CreateBR();
		break;
	case Type_CARD: 
		pNewElm=WML_CreateCARD();
		break;
	case Type_DO:
		pNewElm=WML_CreateDO();
		break;
	case Type_FIELDSET:
		pNewElm=WML_CreateFIELDSET();
		break;
	case Type_GO:
		pNewElm=WML_CreateGO();
		break;
	case Type_HEAD:
		pNewElm=WML_CreateHEAD();
		break;
	case Type_IMG:
		pNewElm=WML_CreateIMG();
		break;
	case Type_INPUT:
		pNewElm=WML_CreateINPUT();
		break;
	case Type_META:
		pNewElm=WML_CreateMETA();
		break;
	case Type_NOOP: 
		/* Empty */
		pNewElm=XML_CreateEmptyElement(iType);
		break;
	case Type_ONEVENT:
		pNewElm=WML_CreateONEVENT();
		break;
	case Type_OPTGROUP:
		pNewElm=WML_CreateOPTGROUP();
		break;
	case Type_OPTION:
		pNewElm=WML_CreateOPTION();
		break;
	case Type_P:
		pNewElm=WML_CreateP();
		break;
	case Type_POSTFIELD:
		pNewElm=WML_CreatePOSTFIELD();
		break;
	case Type_PRE:
		/* This element is only allowed if the document is 1.2 or newer */
		if (pDecStr->iVersion>=WML_VERSION_1_2)
		{
			pNewElm=WML_CreatePRE();
		}
		else
		{
			pNewElm=WML_CreateUNKNOWN();
		}
		break;
	case Type_PREV: 
		pNewElm=WML_CreatePREV();
		break;
	case Type_REFRESH:
		pNewElm=WML_CreateREFRESH();
		break;
	case Type_SELECT: 
		pNewElm=WML_CreateSELECT();
		break;
	case Type_SETVAR:
		pNewElm=WML_CreateSETVAR();
		break;
	case Type_TABLE:
		pNewElm=WML_CreateTABLE();
		break;
	case Type_TD:
		pNewElm=WML_CreateTD();
		break;
	case Type_TEMPLATE:
		pNewElm=WML_CreateTEMPLATE();
		break;
	case Type_TIMER:
		pNewElm=WML_CreateTIMER();
		break;
	case Type_TR:
		pNewElm=WML_CreateTR();
		break;
	case Type_WML:
		pNewElm=WML_CreateWML();
		break;
	default:
		/* The type is unknown. Return an element of the type 
		   Type_Unknown. This element can contain content. 
		   In the function WML_ModifyParse in WMLParse.c the 
		   element is removed and its contents are moved 
		   "one level up" to take its place. This to support
		   paragraph 12.4 in the WML specification.			*/
		pNewElm=WML_CreateUNKNOWN();
		break;
	}

	return ((ELEMENTTYPE*) pNewElm);
}


/*========================================================================
	WML_DeleteElement
==========================================================================
	WML-specific implementation of DTD_DeleteElementFunc.
==========================================================================*/
void WML_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement)
{
	if (ppElement==NULL)
	{
		return;
	}

	if (*ppElement!=NULL) 
	{
		switch ((*ppElement)->iType) 
		{
		case Type_A:
			XML_DeleteElementList(&(((AELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((AELEMENT*)(*ppElement))->pHref),pDecStr);
			XML_DeleteElementList(&(((AELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((AELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_ANCHOR:
			XML_DeleteElementList(&(((ANCHORELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((ANCHORELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((ANCHORELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_ACCESS:
			DEALLOC(&(((ACCESSELEMENT*)(*ppElement))->pchDomain));
			DEALLOC(&(((ACCESSELEMENT*)(*ppElement))->pchPath));
			break;
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
			XML_DeleteElementList(&(((EMPHELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((EMPHELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_BR:
			DEALLOC(&(((BRELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_CARD: 
			DEALLOC(&(((CARDELEMENT*)(*ppElement))->pchId));
			XML_DeleteElementList(&(((CARDELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((CARDELEMENT*)(*ppElement))->pOnEntForw),pDecStr);
			XML_DeleteElementList(&(((CARDELEMENT*)(*ppElement))->pOnEntBack),pDecStr);
			XML_DeleteElementList(&(((CARDELEMENT*)(*ppElement))->pOnTimer),pDecStr);
			XML_DeleteElementList(&(((CARDELEMENT*)(*ppElement))->pContent),pDecStr);
			WML_DeleteEventTab(((CARDELEMENT*)(*ppElement))->pEventTab);
			WML_DeleteTemplateDO(((CARDELEMENT*)(*ppElement))->pTemplateDO);
			DEALLOC(&(((CARDELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_DO:
			DEALLOC(&(((DOELEMENT*)(*ppElement))->pchType));
			DEALLOC(&(((DOELEMENT*)(*ppElement))->pchName));
			XML_DeleteElementList(&(((DOELEMENT*)(*ppElement))->pLabel),pDecStr);
			XML_DeleteElementList(&(((DOELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((DOELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_FIELDSET:
			XML_DeleteElementList(&(((FIELDSETELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((FIELDSETELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((FIELDSETELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_GO:
			DEALLOC(&(((GOELEMENT*)(*ppElement))->pchAccChar));
			XML_DeleteElementList(&(((GOELEMENT*)(*ppElement))->pHref),pDecStr);
			XML_DeleteElementList(&(((GOELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_HEAD:
			XML_DeleteElementList(&(((HEADELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_IMG:
			XML_DeleteElementList(&(((IMGELEMENT*)(*ppElement))->pAlt),pDecStr);
			XML_DeleteElementList(&(((IMGELEMENT*)(*ppElement))->pSrc),pDecStr);
			XML_DeleteElementList(&(((IMGELEMENT*)(*ppElement))->pLocalSrc),pDecStr);
			DEALLOC(&(((IMGELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_INPUT:
			DEALLOC(&(((INPUTELEMENT*)(*ppElement))->pchName));
			DEALLOC(&(((INPUTELEMENT*)(*ppElement))->pchFormat));
			XML_DeleteElementList(&(((INPUTELEMENT*)(*ppElement))->pValue),pDecStr);
			XML_DeleteElementList(&(((INPUTELEMENT*)(*ppElement))->pTitle),pDecStr);
			DEALLOC(&(((INPUTELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_META:
			DEALLOC(&(((METAELEMENT*)(*ppElement))->pchHttpEquiv));
			DEALLOC(&(((METAELEMENT*)(*ppElement))->pchName));
			DEALLOC(&(((METAELEMENT*)(*ppElement))->pchContent));
			DEALLOC(&(((METAELEMENT*)(*ppElement))->pchScheme));
			break;
		case Type_NOOP: 
			/* No attributes to delete */
			break;
		case Type_ONEVENT:
			XML_DeleteElementList(&(((ONEVENTELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_OPTGROUP:
			XML_DeleteElementList(&(((OPTGROUPELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((OPTGROUPELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((OPTGROUPELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_OPTION:
			XML_DeleteElementList(&(((OPTIONELEMENT*)(*ppElement))->pValue),pDecStr);
			XML_DeleteElementList(&(((OPTIONELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((OPTIONELEMENT*)(*ppElement))->pOnpick),pDecStr);
			XML_DeleteElementList(&(((OPTIONELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((OPTIONELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_P: 
			XML_DeleteElementList(&(((PELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((PELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_POSTFIELD: 
			XML_DeleteElementList(&(((POSTFIELDELEMENT*)(*ppElement))->pName),pDecStr);
			XML_DeleteElementList(&(((POSTFIELDELEMENT*)(*ppElement))->pValue),pDecStr);
			break;
		case Type_PRE: 
			XML_DeleteElementList(&(((PREELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_PREV: 
			XML_DeleteElementList(&(((PREVELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_REFRESH:
			XML_DeleteElementList(&(((REFRESHELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_SELECT: 
			DEALLOC(&(((SELECTELEMENT*)(*ppElement))->pchName));
			DEALLOC(&(((SELECTELEMENT*)(*ppElement))->pchIName));
			XML_DeleteElementList(&(((SELECTELEMENT*)(*ppElement))->pValue),pDecStr);
			XML_DeleteElementList(&(((SELECTELEMENT*)(*ppElement))->pIValue),pDecStr);
			XML_DeleteElementList(&(((SELECTELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((SELECTELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((SELECTELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_SETVAR:
			XML_DeleteElementList(&(((SETVARELEMENT*)(*ppElement))->pName),pDecStr);
			XML_DeleteElementList(&(((SETVARELEMENT*)(*ppElement))->pValue),pDecStr);
			break;
		case Type_TABLE:
			DEALLOC(&(((TABLEELEMENT*)(*ppElement))->pchAlign));
			XML_DeleteElementList(&(((TABLEELEMENT*)(*ppElement))->pTitle),pDecStr);
			XML_DeleteElementList(&(((TABLEELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((TABLEELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_TD:
			XML_DeleteElementList(&(((TDELEMENT*)(*ppElement))->pContent),pDecStr);
			DEALLOC(&(((TDELEMENT*)(*ppElement))->pwchXmlLang));
			break;
		case Type_TEMPLATE:
			XML_DeleteElementList(&(((TEMPLATEELEMENT*)(*ppElement))->pOnEntForw),pDecStr);
			XML_DeleteElementList(&(((TEMPLATEELEMENT*)(*ppElement))->pOnEntBack),pDecStr);
			XML_DeleteElementList(&(((TEMPLATEELEMENT*)(*ppElement))->pOnTimer),pDecStr);
			XML_DeleteElementList(&(((TEMPLATEELEMENT*)(*ppElement))->pContent),pDecStr);
			WML_DeleteEventTab(((TEMPLATEELEMENT*)(*ppElement))->pEventTab);
			break;
		case Type_TIMER:	
			DEALLOC(&(((TIMERELEMENT*)(*ppElement))->pchName));
			XML_DeleteElementList(&(((TIMERELEMENT*)(*ppElement))->pValue),pDecStr);
			break;
		case Type_TR:
			XML_DeleteElementList(&(((TRELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_UNKNOWN:
			XML_DeleteElementList(&(((UNKNOWNELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		case Type_WML:
			DEALLOC(&(((WMLELEMENT*)(*ppElement))->pwchXmlLang));
			XML_DeleteElementList(&(((WMLELEMENT*)(*ppElement))->pContent),pDecStr);
			break;
		}

		/* Delete element */
		DEALLOC(ppElement);		
	}
}


/*========================================================================
	WML_AddContent
==========================================================================
	WML-specific implementation of DTD_AddContentFunc.
==========================================================================*/
BOOL WML_AddContent (pDECSTR pDecStr, pELEMENTTYPE pElement, 
					 pELEMENTTYPE* ppContent)
{
	pDecStr=pDecStr;

	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Type_A:
		case Type_ANCHOR:
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
		case Type_CARD:
		case Type_DO:
		case Type_FIELDSET:
		case Type_GO:
		case Type_HEAD:
		case Type_ONEVENT:
		case Type_OPTGROUP:
		case Type_OPTION:
		case Type_P:
		case Type_PRE:
		case Type_PREV:
		case Type_REFRESH:
		case Type_SELECT:
		case Type_TABLE:
		case Type_TD:
		case Type_TEMPLATE:
		case Type_TR:
		case Type_UNKNOWN:
		case Type_WML:

			/* Add content */
			XML_AddContent(pElement,ppContent);

			/* Content is stored */
			return TRUE;
		}
	}

	/* Content is NOT stored */
	return FALSE;
}


/*========================================================================
	WML_GetContent
==========================================================================
	WML-specific implementation of DTD_GetContentFunc.
==========================================================================*/
pELEMENTTYPE WML_GetContent (pELEMENTTYPE pElement)
{
	if (pElement!=NULL) 
	{
		switch (pElement->iType) 
		{
		case Type_A:
		case Type_ANCHOR:
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
		case Type_CARD:
		case Type_DO:
		case Type_FIELDSET:
		case Type_GO:
		case Type_HEAD:
		case Type_ONEVENT:
		case Type_OPTGROUP:
		case Type_OPTION:
		case Type_P:
		case Type_PRE:
		case Type_PREV:
		case Type_REFRESH:
		case Type_SELECT:
		case Type_TABLE:
		case Type_TD:
		case Type_TEMPLATE:
		case Type_TR:
		case Type_UNKNOWN:
		case Type_WML:

			/* Get content */
			return (XML_GetContent(pElement));
		}
	}
	return NULL;
}



/*========================================================================

	Attribute functions

=========================================================================*/

/*========================================================================
	WML_Set...
==========================================================================
	The following functions sets the attribute values in the elements. If
	the attribute field and the attribute value is valid for the element 
	it is and stored and TRUE is returned. Otherwise, FALSE is returned. 

	NOTE! It is the callers responsibility to deallocate all parts NOT 
	used by the functions. For example, if ppAlign is not used in the 
	function WML_SetAlign, this memory must be deallocated.
	
	Input: Element (NOT NULL) and attribute value.						
	Output: BOOL
==========================================================================*/
BOOL WML_SetAccChar	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAccChar)
{		
	if (pElement->iType==Type_GO) 
	{
		if (((GOELEMENT*)(pElement))->pchAccChar==NULL) 
		{
			((GOELEMENT*)(pElement))->pchAccChar=XML_ConvertToString(ppAccChar,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetAccesskey (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAccesskey)
{
	BOOL fResult=FALSE;

	/* The function returns FALSE if the document is not 1.2 or newer (greater) */
	if (pDecStr->iVersion>=WML_VERSION_1_2)
	{
		WCHAR* pwchAccess=XML_ConvertToString(ppAccesskey,pDecStr);

		/* Check if accesskey is exactly one character */
		if ((pwchAccess!=NULL)&&(STRINGLENGTH(pwchAccess)==1))
		{
			/* Store character */
			switch (pElement->iType)
			{
			case Type_A:
				((AELEMENT *)(pElement))->wchAccessKey=*pwchAccess;
				fResult=TRUE;
				break;
			case Type_ANCHOR:
				((ANCHORELEMENT *)(pElement))->wchAccessKey=*pwchAccess;
				fResult=TRUE;
				break;
			case Type_INPUT:
				((INPUTELEMENT *)(pElement))->wchAccessKey=*pwchAccess;
				fResult=TRUE;
				break;
			}
		}

		/* Delete string */
		DEALLOC(&pwchAccess);
	}

	return fResult;
}


BOOL WML_SetAlign (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAlign, INT8 iAlign)
{
	BOOL fResult=FALSE;
	WCHAR* pwchAlign=XML_ConvertToString(ppAlign,pDecStr);
	
	/* Table - WCHAR* */
	if (pElement->iType==Type_TABLE) 
	{
		((TABLEELEMENT*)(pElement))->pchAlign=pwchAlign;
		return TRUE;
	}

	/* img and p - INT8 */
	else if (iAlign==DEF_TYPE_STRING)
	{
		/* Convert type to INT8 */
		iAlign=WML_ConvertAlign(&pwchAlign);
	}
	else
	{
		DEALLOC(&pwchAlign);
	}

	/* Store */
	if (pElement->iType==Type_IMG)
	{
		((IMGELEMENT*)(pElement))->iIAlign=iAlign;
		fResult=TRUE;
	}
	else if (pElement->iType==Type_P) 
	{
		((PELEMENT*)(pElement))->iAlign=iAlign;
		fResult=TRUE;
	}

	return fResult;
}


BOOL WML_SetAlt	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAlt)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_IMG) 
	{
		if (((IMGELEMENT*)(pElement))->pAlt==NULL) 
		{
			((IMGELEMENT*)(pElement))->pAlt=*ppAlt;
			*ppAlt=NULL;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetCacheControl (pDECSTR pDecStr, pELEMENTTYPE pElement, 
						  pELEMENTTYPE* ppCacheControl, enum CacheControl iCache)
{
	/* The function returns FALSE if the document is not 1.3 or newer (greater) */
	if (pDecStr->iVersion>=WML_VERSION_1_3)
	{
		/* Convert ppCacheControl (text) to enum CacheControl. The ELEMENTTYPE is removed
			 in the XML_ConvertToString function and the string is removed in the 
			 ConvertCacheControl function. */
		WCHAR* pwchCache=XML_ConvertToString(ppCacheControl,pDecStr);

		if (pElement->iType==Type_GO) 
		{
			if (pwchCache!=NULL)
			{
				((GOELEMENT*)(pElement))->iCacheControl=WML_ConvertCacheControl(&pwchCache);
			}
			else
			{
				((GOELEMENT*)(pElement))->iCacheControl=iCache;
			}
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetColumns	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppColumns)
{
	UINT8 iCol=0;

	/* Convert pColumns (text) to UINT8. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the ConvertNumber function. */
	WCHAR* pwchColumns=XML_ConvertToString(ppColumns,pDecStr);

	if (Convert_StringToUINT8(&pwchColumns,&iCol))
	{
		if (pElement->iType==Type_TABLE) 
		{
			((TABLEELEMENT*)(pElement))->iColumns=iCol;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetContent	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent)
{
	if (pElement->iType==Type_META) 
	{
		if (((METAELEMENT*)(pElement))->pchContent==NULL) 
		{
			((METAELEMENT*)(pElement))->pchContent=XML_ConvertToString(ppContent,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetDomain (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppDomain)
{
	if (pElement->iType==Type_ACCESS) 
	{
		if (((ACCESSELEMENT*)(pElement))->pchDomain==NULL) 
		{
			((ACCESSELEMENT*)(pElement))->pchDomain=XML_ConvertToString(ppDomain,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetEmptyOK	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppEmptyOK, BOOL fEmptyOK)
{
	BOOL fResult=fEmptyOK;

	if (pElement->iType==Type_INPUT) 
	{
		if (*ppEmptyOK!=NULL)
		{	
			WCHAR* pwchTemp=XML_ConvertToString(ppEmptyOK,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in converion */
				return FALSE;
			}
		}

		((INPUTELEMENT*)(pElement))->fEmptyOK=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetEnctype	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppEnctype)
{
	/* The function returns FALSE if the document is not 1.2 or newer (greater) */
	if ((pElement->iType==Type_GO)&&(pDecStr->iVersion>=WML_VERSION_1_2))
	{
		WCHAR* pwchTemp=XML_ConvertToString(ppEnctype,pDecStr);

		((GOELEMENT*)(pElement))->iEnctype=WML_ConvertEnctype(&pwchTemp);
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetFormat (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppFormat)
{
	if (pElement->iType==Type_INPUT) 
	{
		/* Remove old input format */
		if (((INPUTELEMENT*)(pElement))->pchFormat!=NULL) 
		{
			DEALLOC(&(((INPUTELEMENT *)(pElement))->pchFormat));
		}

		((INPUTELEMENT*)(pElement))->pchFormat=XML_ConvertToString(ppFormat,pDecStr);
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetForua (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppForua, BOOL fForua)
{
	BOOL fResult=fForua;

	if (pElement->iType==Type_META) 
	{
		if (*ppForua!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppForua,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((METAELEMENT *)(pElement))->fForua=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetHeight (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHeight)
{
	UINT16 iHeight=0;
	BOOL fIsPercent=FALSE;

	/* Convert height (text) to UINT16. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT16percent function. */
	WCHAR* pwchHeight=XML_ConvertToString(ppHeight,pDecStr);

	if (Convert_StringToUINT16percent(&pwchHeight,&iHeight,&fIsPercent))
	{
		if (pElement->iType==Type_IMG)
		{	
			if (fIsPercent)
			{
				((IMGELEMENT *)(pElement))->iIsPercent|=HEIGHT_IS_PERCENT;
			}

			((IMGELEMENT *)(pElement))->iHeight=iHeight;
			return TRUE;
		} 
	}

	return FALSE;
}


BOOL WML_SetHSpace (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHSpace)
{
	UINT8 iHSpace=0;
	BOOL fIsPercent=FALSE;

	/* Convert hSpace (text) to UINT8. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT8percent function. */
	WCHAR* pwchHSpace=XML_ConvertToString(ppHSpace,pDecStr);

	if (Convert_StringToUINT8percent(&pwchHSpace,&iHSpace,&fIsPercent))
	{
		if (pElement->iType==Type_IMG)
		{	
			if (fIsPercent)
			{
				((IMGELEMENT *)(pElement))->iIsPercent|=HSPACE_IS_PERCENT;
			}

			((IMGELEMENT *)(pElement))->iHSpace=iHSpace;
			return TRUE;
		} 
	}

	return FALSE;
}


BOOL WML_SetHref (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHref)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_A) 
	{
		if (((AELEMENT*)(pElement))->pHref==NULL) 
		{
			((AELEMENT*)(pElement))->pHref=*ppHref;
			*ppHref=NULL;
			return TRUE;
		}
	} 
	else if (pElement->iType==Type_GO) 
	{
		if (((GOELEMENT*)(pElement))->pHref==NULL) 
		{
			((GOELEMENT*)(pElement))->pHref=*ppHref;
			*ppHref=NULL;
			return TRUE;
		}
	} 

	return FALSE;
}


BOOL WML_SetHttpEq (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHttpEq)
{
	if (pElement->iType==Type_META) 
	{
		if (((METAELEMENT*)(pElement))->pchHttpEquiv==NULL) 
		{
			((METAELEMENT*)(pElement))->pchHttpEquiv=XML_ConvertToString(ppHttpEq,pDecStr);
			return TRUE;
		}
	} 

	return FALSE;
}

BOOL WML_SetId (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppId)
{
	if (pElement->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElement))->pchId==NULL) 
		{
			((CARDELEMENT *)(pElement))->pchId=XML_ConvertToString(ppId,pDecStr);
			return TRUE;
		}
	} 

	/* The attribute id is not used in other elements - delete */
	XML_DeleteElementList(ppId,pDecStr);
	return TRUE;
}


BOOL WML_SetIValue (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppIValue)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_SELECT) 
	{
		if (((SELECTELEMENT *)(pElement))->pIValue==NULL) 
		{
			((SELECTELEMENT *)(pElement))->pIValue=*ppIValue;
			*ppIValue=NULL;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetIName (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppIName)
{
	if (pElement->iType==Type_SELECT) 
	{
		if (((SELECTELEMENT *)(pElement))->pchIName==NULL) 
		{
			((SELECTELEMENT *)(pElement))->pchIName=XML_ConvertToString(ppIName,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetLabel (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLabel)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_DO) 
	{
		if (((DOELEMENT *)(pElement))->pLabel==NULL) 
		{
			((DOELEMENT *)(pElement))->pLabel=*ppLabel;
			*ppLabel=NULL;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetLocalSrc (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLocalSrc)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_IMG) 
	{
		if (((IMGELEMENT *)(pElement))->pLocalSrc==NULL) 
		{
			((IMGELEMENT *)(pElement))->pLocalSrc=*ppLocalSrc;
			*ppLocalSrc=NULL;
			return TRUE;
		}
	} 

	return FALSE;
}


BOOL WML_SetMaxLength (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLength)
{
	UINT32 iLength=0;

	/* Convert length (text) to UINT32. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT32 function. */
	WCHAR* pwchLength=XML_ConvertToString(ppLength,pDecStr);

	if (Convert_StringToUINT32(&pwchLength,&iLength))
	{
		if (pElement->iType==Type_INPUT) 
		{
			if (iLength>127) 
			{
				((INPUTELEMENT *)(pElement))->iMaxLength=-1;
			}
			else
			{
				((INPUTELEMENT *)(pElement))->iMaxLength=(INT8)iLength;
			}
			return TRUE;
		} 
	}
	return FALSE;
}


BOOL WML_SetMethod (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMethod, enum MethodType iMethod)
{
	enum MethodType iResult=iMethod;

	if (pElement->iType==Type_GO) 
	{
		if (*ppMethod!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppMethod,pDecStr);

			iResult=WML_ConvertMethod(pwchTemp);
		}

		((GOELEMENT *)(pElement))->iMethod=iResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetMode (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMode, enum WrapMode iMode)
{
	enum WrapMode iResult=iMode;

	if (pElement->iType==Type_P) 
	{
		if (*ppMode!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppMode,pDecStr);

			iResult=WML_ConvertMode(pwchTemp);
		}

		((PELEMENT *)(pElement))->iMode=iResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetMultiple (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMultiple, BOOL fMultiple)
{
	BOOL fResult=fMultiple;

	if (pElement->iType==Type_SELECT) 
	{
		if (*ppMultiple!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppMultiple,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((SELECTELEMENT *)(pElement))->fMultiple=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetName (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppName)
{
	switch (pElement->iType)
	{
	case Type_DO:
		if (((DOELEMENT *)(pElement))->pchName==NULL) 
		{
			((DOELEMENT *)(pElement))->pchName=XML_ConvertToString(ppName,pDecStr);
			return TRUE;
		}
		break;
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElement))->pchName==NULL) 
		{
			((INPUTELEMENT *)(pElement))->pchName=XML_ConvertToString(ppName,pDecStr);
			return TRUE;
		}
		break;
	case Type_META:
		if (((METAELEMENT *)(pElement))->pchName==NULL) 
		{
			((METAELEMENT *)(pElement))->pchName=XML_ConvertToString(ppName,pDecStr);
			return TRUE;
		}
		break;
	case Type_POSTFIELD:
		if (((POSTFIELDELEMENT *)(pElement))->pName==NULL) 
		{
			((POSTFIELDELEMENT *)(pElement))->pName=*ppName;
			*ppName=NULL;
			return TRUE;
		}
		break;
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElement))->pchName==NULL) 
		{
			((SELECTELEMENT *)(pElement))->pchName=XML_ConvertToString(ppName,pDecStr);
			return TRUE;
		}
		break;
	case Type_SETVAR:
		if (((SETVARELEMENT *)(pElement))->pName==NULL) 
		{
			((SETVARELEMENT *)(pElement))->pName=*ppName;
			*ppName=NULL;
			return TRUE;
		}
		break;
	case Type_TIMER:
		if (((TIMERELEMENT *)(pElement))->pchName==NULL) 
		{
			((TIMERELEMENT *)(pElement))->pchName=XML_ConvertToString(ppName,pDecStr);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


BOOL WML_SetNewContext (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppNewContext, BOOL fContext)
{
	BOOL fResult=fContext;

	if (pElement->iType==Type_CARD) 
	{
		if (*ppNewContext!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppNewContext,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((CARDELEMENT *)(pElement))->fNewContext=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetOnEntBack (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElement))->pOnEntBack==NULL) 
		{
			((CARDELEMENT *)(pElement))->pOnEntBack=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	} 
	else if (pElement->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElement))->pOnEntBack==NULL) 
		{
			((TEMPLATEELEMENT *)(pElement))->pOnEntBack=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetOnEntForw (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElement))->pOnEntForw==NULL) 
		{
			((CARDELEMENT *)(pElement))->pOnEntForw=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	} 
	else if (pElement->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElement))->pOnEntForw==NULL) 
		{
			((TEMPLATEELEMENT *)(pElement))->pOnEntForw=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetOnPick (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_OPTION) 
	{
		if (((OPTIONELEMENT *)(pElement))->pOnpick==NULL) 
		{
			((OPTIONELEMENT *)(pElement))->pOnpick=*ppURL;
			*ppURL=NULL;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetOnTimer (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElement))->pOnTimer==NULL) 
		{
			((CARDELEMENT *)(pElement))->pOnTimer=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	} 
	else if (pElement->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElement))->pOnTimer==NULL) 
		{
			((TEMPLATEELEMENT *)(pElement))->pOnTimer=*ppURL;

			/* ppURL MUST NOT be set to NULL here. The pointer is also 
			   used in the calling function. */
			return TRUE;
		}
	} 

	return FALSE;
}


BOOL WML_SetOptional (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppOptional, BOOL fOptional)
{
	BOOL fResult=fOptional;

	if (pElement->iType==Type_DO) 
	{
		if (*ppOptional!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppOptional,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((DOELEMENT *)(pElement))->fOptional=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetPath (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppPath)
{
	if (pElement->iType==Type_ACCESS) 
	{
		if (((ACCESSELEMENT *)(pElement))->pchPath==NULL) 
		{
			((ACCESSELEMENT *)(pElement))->pchPath=XML_ConvertToString(ppPath,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetScheme (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppScheme)
{
	if (pElement->iType==Type_META) 
	{
		if (((METAELEMENT *)(pElement))->pchScheme==NULL) 
		{
			((METAELEMENT *)(pElement))->pchScheme=XML_ConvertToString(ppScheme,pDecStr);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL WML_SetSendReferer (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppSendRef, BOOL fSendRef)
{
	BOOL fResult=fSendRef;

	if (pElement->iType==Type_GO) 
	{
		if (*ppSendRef!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppSendRef,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((GOELEMENT *)(pElement))->fSendRef=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetSize (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppSize)
{
	UINT32 iSize=0;

	/* Convert size (text) to UINT32. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT32 function. */
	WCHAR* pwchSize=XML_ConvertToString(ppSize,pDecStr);

	if (Convert_StringToUINT32(&pwchSize,&iSize))
	{
		if (pElement->iType==Type_INPUT) 
		{
			if (iSize>127) 
			{
				((INPUTELEMENT *)(pElement))->iSize=-1;
			}
			else
			{
				((INPUTELEMENT *)(pElement))->iSize=(INT8)iSize;
			}
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WML_SetSrc	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHref)
{
	pDecStr=pDecStr;

	if (pElement->iType==Type_IMG) 
	{
		if (((IMGELEMENT *)(pElement))->pSrc==NULL) 
		{
			((IMGELEMENT *)(pElement))->pSrc=*ppHref;
			*ppHref=NULL;
			return TRUE;
		}
	} 

	return FALSE;
}


BOOL WML_SetOrdered	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppOrdered, BOOL fOrdered)
{
	BOOL fResult=fOrdered;

	if (pElement->iType==Type_CARD) 
	{
		if (*ppOrdered!=NULL)
		{
			WCHAR* pwchTemp=XML_ConvertToString(ppOrdered,pDecStr);

			if (!Convert_StringToBool(&pwchTemp,&fResult))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((CARDELEMENT *)(pElement))->fOrdered=fResult;
		return TRUE;
	} 

	return FALSE;
}


BOOL WML_SetTabIndex (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppTabIndex)
{		
	UINT8 iTabIndex=0;

	/* Convert tabindex (text) to UINT8. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the ConvertNumber function. */
	WCHAR* pwchTabIndex=XML_ConvertToString(ppTabIndex,pDecStr);

	if (Convert_StringToUINT8(&pwchTabIndex,&iTabIndex))
	{
		if (pElement->iType==Type_INPUT) 
		{
			((INPUTELEMENT *)(pElement))->iTabIndex=iTabIndex;
			return TRUE;
		} 
		else if (pElement->iType==Type_SELECT) 
		{
			((SELECTELEMENT *)(pElement))->iTabIndex=iTabIndex;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WML_SetTitle (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppTitle)
{
	pDecStr=pDecStr;

	switch (pElement->iType)
	{
	case Type_A:
	case Type_ANCHOR:
		if (((AELEMENT *)(pElement))->pTitle==NULL) 
		{
			((AELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_CARD:
		if (((CARDELEMENT *)(pElement))->pTitle==NULL) 
		{
			((CARDELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_FIELDSET:
		if (((FIELDSETELEMENT *)(pElement))->pTitle==NULL) 
		{
			((FIELDSETELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElement))->pTitle==NULL) 
		{
			((INPUTELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_OPTGROUP:
		if (((OPTGROUPELEMENT *)(pElement))->pTitle==NULL) 
		{
			((OPTGROUPELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_OPTION:
		if (((OPTIONELEMENT *)(pElement))->pTitle==NULL) 
		{
			((OPTIONELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElement))->pTitle==NULL) 
		{
			((SELECTELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	case Type_TABLE:
		if (((TABLEELEMENT *)(pElement))->pTitle==NULL) 
		{
			((TABLEELEMENT *)(pElement))->pTitle=*ppTitle;
			*ppTitle=NULL;
			return TRUE;
		}
		break;
	}

	return FALSE;
}


BOOL WML_SetType (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppType, BOOL fWTA)
{
	INT8 iType=DEF_TYPE_UNKNOWN;
	WCHAR* pwchType=XML_ConvertToString(ppType,pDecStr);

	fWTA=fWTA;

	switch (pElement->iType)
	{
	case Type_DO:
		if (pwchType!=NULL)
		{
			/* Remove old type (default value) */
			DEALLOC(&((DOELEMENT*)(pElement))->pchType);

			((DOELEMENT*)(pElement))->pchType=pwchType;
			return TRUE;
		} 
		break;

	case Type_INPUT:
		/* Only predefined events -> convert to integer */
		iType=WML_ConvertInputToInt(&pwchType);

		if (iType!=DEF_TYPE_UNKNOWN)
		{
			((INPUTELEMENT*)(pElement))->iInputType=iType;
			return TRUE;
		}
		break;
	case Type_ONEVENT:
		/* Check WTA events if WTA enabled AND the user agent
		   is a WTA user agent */
		#ifdef CONFIG_WTA
		if (fWTA)
		{
			/* Convert WTA event */
			iType=WTA_ConvertEventToInt(pwchType);

			/* The function WTA_ConvertEventToInt returns 
			   DEF_TYPE_UNKNOWN if the string was not a WTA event.
			   In this case the WML_ConvertTypeToInt should be called.
			   Otherwise the string should be deleted. */
		}
		#endif

		/* Only predefined events -> convert to integer */
		if (iType==DEF_TYPE_UNKNOWN)
		{
			iType=WML_ConvertEventToInt(&pwchType);
		}

		if (iType!=DEF_TYPE_UNKNOWN)
		{
			((ONEVENTELEMENT *)(pElement))->iEventType=iType;
			DEALLOC(&pwchType);
			return TRUE;
		}
	}

	/* Delete string pchType */
	DEALLOC(&pwchType);

	return FALSE;
}


BOOL WML_SetValue (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppValue)
{
	pDecStr=pDecStr;

	switch (pElement->iType)
	{
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElement))->pValue==NULL) 
		{
			((INPUTELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	case Type_OPTION:
		if (((OPTIONELEMENT *)(pElement))->pValue==NULL) 
		{
			((OPTIONELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	case Type_POSTFIELD:
		if (((POSTFIELDELEMENT *)(pElement))->pValue==NULL) 
		{
			((POSTFIELDELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElement))->pValue==NULL) 
		{
			((SELECTELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	case Type_SETVAR:
		if (((SETVARELEMENT *)(pElement))->pValue==NULL) 
		{
			((SETVARELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	case Type_TIMER:
		if (((TIMERELEMENT *)(pElement))->pValue==NULL) 
		{
			((TIMERELEMENT *)(pElement))->pValue=*ppValue;
			*ppValue=NULL;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WML_SetVSpace (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppVSpace)
{
	UINT8 iVSpace=0;
	BOOL fIsPercent=FALSE;

	/* Convert VSpace (text) to UINT8. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT8percent function. */
	WCHAR* pwchVSpace=XML_ConvertToString(ppVSpace,pDecStr);

	if (Convert_StringToUINT8percent(&pwchVSpace,&iVSpace,&fIsPercent))
	{
		if (pElement->iType==Type_IMG)
		{
			if (fIsPercent)
			{
				((IMGELEMENT *)(pElement))->iIsPercent|=VSPACE_IS_PERCENT;
			}

			((IMGELEMENT *)(pElement))->iVSpace=iVSpace;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WML_SetWidth (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppWidth)
{
	UINT16 iWidth=0;
	BOOL fIsPercent=FALSE;

	/* Convert Width (text) to UINT16. The ELEMENTTYPE is removed
	   in the XML_ConvertToString function and the string is removed
	   in the Convert_StringToUINT16percent function. */
	WCHAR* pwchWidth=XML_ConvertToString(ppWidth,pDecStr);

	if (Convert_StringToUINT16percent(&pwchWidth,&iWidth,&fIsPercent))
	{
		if (pElement->iType==Type_IMG) 
		{
			if (fIsPercent)
			{
				((IMGELEMENT *)(pElement))->iIsPercent|=WIDTH_IS_PERCENT;
			}
			
			((IMGELEMENT *)(pElement))->iWidth=iWidth;
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WML_SetXmllang (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppXmllang)
{
	WCHAR* pwchLang=XML_ConvertToString(ppXmllang,pDecStr);

	switch (pElement->iType)
	{
	case Type_WML:
		if (((WMLELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((WMLELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
	case Type_A:
		if (((AELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((AELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_ANCHOR:
		if (((ANCHORELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((ANCHORELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_BR:
		/* This attribute is only only allowed in this element
	       if the document is 1.1 or older. */
		if (pDecStr->iVersion<=WML_VERSION_1_1)
		{
			if (((BRELEMENT*)(pElement))->pwchXmlLang==NULL) 
			{
				((BRELEMENT*)(pElement))->pwchXmlLang=pwchLang;
				return TRUE;
			}
		}
		break;
	case Type_B:
	case Type_BIG:
	case Type_EM:
	case Type_I:
	case Type_SMALL:
	case Type_STRONG:
	case Type_U:
		if (((EMPHELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((EMPHELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_CARD: 
		if (((CARDELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((CARDELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_DO:
		if (((DOELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((DOELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_FIELDSET:
		if (((FIELDSETELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((FIELDSETELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_IMG:
		if (((IMGELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((IMGELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_INPUT:
		if (((INPUTELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((INPUTELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_OPTGROUP:
		if (((OPTGROUPELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((OPTGROUPELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_OPTION:
		if (((OPTIONELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((OPTIONELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_P: 
		if (((PELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((PELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_SELECT: 
		if (((SELECTELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((SELECTELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_TABLE:
		if (((TABLEELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((TABLEELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	case Type_TD:
		if (((TDELEMENT*)(pElement))->pwchXmlLang==NULL) 
		{
			((TDELEMENT*)(pElement))->pwchXmlLang=pwchLang;
			return TRUE;
		}
		break;
	}

	/* Delete Lang string */
	DEALLOC(&pwchLang);
	return FALSE;
}


BOOL WML_SetXmlSpace (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppXmlSpace, UINT8 iSpace)
{
	if (pElement->iType==Type_PRE) 
	{
		if (*ppXmlSpace!=NULL)
		{
			/* Convert XmlSpace (text) to UINT8. The ELEMENTTYPE is removed
			   in the XML_ConvertToString function and the string is removed
			   in the Convert_XmlSpaceToUINT8 function. */
			WCHAR* pwchSpace=XML_ConvertToString(ppXmlSpace,pDecStr);

			if (!Convert_XmlSpaceToUINT8(&pwchSpace,&iSpace))
			{
				/* Error in conversion */
				return FALSE;
			}
		}

		((PREELEMENT*)(pElement))->iXmlSpace=iSpace;
		return TRUE;
	} 

	return FALSE;
}
