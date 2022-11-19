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
/* ==================================================================
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
		000418	JPR		Default value "unknown" added for attribute 
						"type" in do element
		000419	JPR		Corrections in ConvertNumber functions.

   ================================================================== */

#ifndef _ELMHANDLER
#define _ELMHANDLER

#include "Url.h"
#include "CmmnRsrc.h"
#include "WMLDef.h"
#include "WMLToken.h"
#include "WMLEvent.h"
#include "WMLElm.h" 
#include "WAEctype.h"
#include "WMLParse.h"

/* Returns WCHAR '0' to '9' as an UINT8. */
UINT8 WML_CharToInt (WCHAR chNumber)
{
	return (UINT8)(chNumber-(WC('0')));
}

/* Converts from text to integer and removes the text. */
UINT8 WML_ConvertNumber (WCHAR *pchNumber, BOOL *pfIsPercent) 
{
	UINT8 iResult=0;
	UINT8 iCount=0;
	WCHAR *pchNumberTemp=pchNumber;

	*pfIsPercent=FALSE;
	if (pchNumberTemp!=NULL) 
	{
		while (pchNumberTemp[iCount]!=W_EndOfText) 
		{
			/* Check if between '0' and '9' */
			if ((pchNumberTemp[iCount] < 0x100) && wae_isdigit(pchNumberTemp[iCount]))
			{
				UINT8 iDigit=WML_CharToInt(pchNumberTemp[iCount]);

				/* Check if result will overflow */
				if ((iResult>25)|| ( (iResult==25)&&(iDigit>5)) )
				{
					DEALLOC(&pchNumber);

					/* return max */
					return (UINT8)(255);
				}

				iResult=(UINT8)((iResult*10)+iDigit);
			}
			else if (pchNumberTemp[iCount]==(WC('%')))
			{
				*pfIsPercent=TRUE;
				DEALLOC(&pchNumber);
				return iResult;
			}
			else 
			{
				DEALLOC(&pchNumber);
				return 0;
			}
			iCount++;
		}
		DEALLOC(&pchNumber);
	}
	return iResult;
}

/* Converts from text to integer (UINT16) and removes the text. */
UINT16 WML_ConvertNumber16 (WCHAR *pchNumber, BOOL *pfIsPercent) 
{
	UINT16 iResult=0;
	UINT16 iCount=0;
	WCHAR *pchNumberTemp=pchNumber;

	*pfIsPercent=FALSE;
	if (pchNumberTemp!=NULL) 
	{
		while (pchNumberTemp[iCount]!=W_EndOfText) 
		{
			/* Check if between '0' and '9' */
			if ((pchNumberTemp[iCount] < 0x100) && wae_isdigit(pchNumberTemp[iCount]))
			{
				UINT8 iDigit=WML_CharToInt(pchNumberTemp[iCount]);

				/* Check if result will overflow */
				if ((iResult>6553)|| ( (iResult==6553)&&(iDigit>5)) )
				{
					DEALLOC(&pchNumber);

					/* return max */
					return UINT16_MAX;
				}

				iResult=(UINT16)((iResult*10)+iDigit);
			}
			else if (pchNumberTemp[iCount]==(WC('%')))
			{
				*pfIsPercent=TRUE;
				DEALLOC(&pchNumber);
				return iResult;
			}
			else 
			{
				DEALLOC(&pchNumber);
				return 0;
			}
			iCount++;
		}
		DEALLOC(&pchNumber);
	}
	return iResult;
}

/* Converts from text to integer and removes the text. */
UINT32 WML_ConvertNumber32 (WCHAR *pchNumber) 
{
	UINT32 iResult=0;
	UINT32 iCount=0;
	WCHAR *pchNumberTemp=pchNumber;

	if (pchNumberTemp!=NULL) 
	{
		while (pchNumberTemp[iCount]!=W_EndOfText) 
		{
			/* Check if between '0' and '9' */
			if ((pchNumberTemp[iCount] < 0x100) && wae_isdigit(pchNumberTemp[iCount]))
			{
				UINT8 iDigit=WML_CharToInt(pchNumberTemp[iCount]);

				/* Check if result will overflow */
				if ((iResult>(UINT32)(429496729UL))|| ( (iResult==(UINT32)(429496729UL))&&(iDigit>5)) )
				{
					DEALLOC(&pchNumber);

					/* return max */
					return UINT32_MAX;
				}

				iResult=(UINT32)((iResult*10)+iDigit);
			}
			else 
			{
				DEALLOC(&pchNumber);
				return 0;
			}
			iCount++;
		}
		DEALLOC(&pchNumber);
	}
	return iResult;
}

INT8 WML_ConvertType (WCHAR **ppchType) 
{
	const WCHAR pchConst_OnEnterForward[]={'o','n','e','n','t','e','r','f','o','r','w','a','r','d',0};
	const WCHAR pchConst_OnEnterBackward[]={'o','n','e','n','t','e','r','b','a','c','k','w','a','r','d',0};
	const WCHAR pchConst_OnTimer[]={'o','n','t','i','m','e','r',0};
	const WCHAR pchConst_OnClick[]={'o','n','p','i','c','k',0};
	const WCHAR pchConst_Text[]={'t','e','x','t',0};
	const WCHAR pchConst_Password[]={'p','a','s','s','w','o','r','d',0};

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
		else if (COMPARESTRING(*ppchType,pchConst_Text)==0) 
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
INT8 WTA_ConvertEventToInt (WCHAR *pwchType)
{
	INT8 iResult=DEF_TYPE_UNKNOWN;

	if (pwchType!=NULL) 
	{
		/* Compare (Network common) */
		iResult=(INT8)(WML_ConvertTokenToByte (pwchType, 
					ppwchWTAEvents, pbWTAEventBytes, STAT_NBROFWTAEVENTS));

		/* Compare (Network GSM) */

		/* ... */
	
	}

	/* Not WTA event */
	return iResult;
}
#endif


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

BOOL WML_ConvertBool (WCHAR *pchBool)
{
	const WCHAR pchConst_true[]={'t','r','u','e',0};
	
	if (pchBool!=NULL) 
	{
		if (COMPARESTRING(pchBool,pchConst_true)==0) 
		{
			DEALLOC(&pchBool);
			return TRUE;
		}
		DEALLOC(&pchBool);
	}
	return FALSE;
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
/* --------------------------------------------- */



/* ---------------------------------------------------
    Functions for handling the WML specific elements.
   --------------------------------------------------- */

/* Used in all b, big, em, i, small, strong and u-elements */
pEMPHELEMENT WML_CreateEMPH (void) 
{
	pEMPHELEMENT pEMPH;

	pEMPH=NEWSTRUCT(EMPHELEMENT);
	if (pEMPH!=NULL) 
	{
		pEMPH->pContent=NULL;
	}
	return pEMPH;
}

/* Allocate memory for the desired type. If not enough
   memory - return NULL. Set the attribute values to
   their default values. */
pAELEMENT WML_CreateA (void) 
{
	pAELEMENT pA;

	pA=NEWSTRUCT(AELEMENT);
	if (pA!=NULL) 
	{
		pA->pTitle=NULL;
		pA->pHref=NULL;
		pA->pContent=NULL;
	}
	return pA;
}

pANCHORELEMENT WML_CreateANCHOR (void) 
{
	pANCHORELEMENT pANCHOR;

	pANCHOR=NEWSTRUCT(ANCHORELEMENT);
	if (pANCHOR!=NULL) 
	{
		pANCHOR->pTitle=NULL;
		pANCHOR->pContent=NULL;
	}
	return pANCHOR;
}

pACCESSELEMENT WML_CreateACCESS (void) 
{
	pACCESSELEMENT pACCESS;

	pACCESS=NEWSTRUCT(ACCESSELEMENT);
	if (pACCESS!=NULL) 
	{
		pACCESS->pchDomain=NULL;
		pACCESS->pchPath=NULL;
	}
	return pACCESS;
}

pCARDELEMENT WML_CreateCARD (void) 
{
	pCARDELEMENT pCARD;

	pCARD=NEWSTRUCT(CARDELEMENT);
	if (pCARD!=NULL) 
	{
		pCARD->pchId=NULL;
		pCARD->pTitle=NULL;
		pCARD->fNewContext=FALSE;
		pCARD->fOrdered=TRUE;
		pCARD->pOnEntForw=NULL;
		pCARD->pOnEntBack=NULL;
		pCARD->pOnTimer=NULL;
		pCARD->pContent=NULL;
		pCARD->pEventTab=NULL;
		pCARD->pTemplateDO=NULL;
	}
	return pCARD;
}

pDOELEMENT WML_CreateDO (void) 
{
	pDOELEMENT pDO;

	pDO=NEWSTRUCT(DOELEMENT);
	if (pDO!=NULL) 
	{
		/* Default format is "unknown" */
		pDO->pchType=wip_byte2wchar((BYTE *)"unknown");
		pDO->pLabel=NULL;
		pDO->pchName=NULL;
		pDO->fOptional=FALSE;
		pDO->pContent=NULL;
	}
	return pDO;
}

pFIELDSETELEMENT WML_CreateFIELDSET (void) 
{
	pFIELDSETELEMENT pFIELDSET;

	pFIELDSET=NEWSTRUCT(FIELDSETELEMENT);
	if (pFIELDSET!=NULL) 
	{
		pFIELDSET->pTitle=NULL;
		pFIELDSET->pContent=NULL;
	}
	return pFIELDSET;
}

pGOELEMENT WML_CreateGO (void) 
{
	pGOELEMENT pGO;

	pGO=NEWSTRUCT(GOELEMENT);
	if (pGO!=NULL) 
	{
		pGO->pHref=NULL;
		pGO->fSendRef=FALSE;
		pGO->iMethod=M_Get;
		pGO->pchAccChar=NULL;
		pGO->pContent=NULL;
	}
	return pGO;
}

pHEADELEMENT WML_CreateHEAD (void) 
{
	pHEADELEMENT pHEAD;

	pHEAD=NEWSTRUCT(HEADELEMENT);
	if (pHEAD!=NULL)
	{
		pHEAD->pContent=NULL;
	}
	return pHEAD;
}

pIMGELEMENT WML_CreateIMG (void) 
{
	pIMGELEMENT pIMG;

	pIMG=NEWSTRUCT(IMGELEMENT);
	if (pIMG!=NULL) 
	{
		pIMG->pAlt=NULL;
		pIMG->pSrc=NULL;
		pIMG->pLocalSrc=NULL;
		pIMG->iVSpace=0;
		pIMG->iHSpace=0;
		pIMG->iIAlign=ALIGN_BOTTOM;
		pIMG->iHeight=0;
		pIMG->iWidth=0;
		pIMG->iIsPercent=NONE_IS_PERCENT;
	    pIMG->iMMIElementID=0;
	}
	return pIMG;
}

pINPUTELEMENT WML_CreateINPUT (void) 
{
	pINPUTELEMENT pINPUT;

	pINPUT=NEWSTRUCT(INPUTELEMENT);
	if (pINPUT!=NULL) 
	{
		pINPUT->pchName=NULL;
		pINPUT->iType=DEF_TYPE_TEXT;
		pINPUT->pValue=NULL;
		
		/* Default format is *M */
		pINPUT->pchFormat=wip_byte2wchar((BYTE *)"*M");
		pINPUT->fEmptyOK=FALSE;
		pINPUT->iSize=-1;
		pINPUT->iMaxLength=-1;
		pINPUT->iTabIndex=0;
		pINPUT->pTitle=NULL;
	}
	return pINPUT;
}

pMETAELEMENT WML_CreateMETA (void) 
{
	pMETAELEMENT pMETA;

	pMETA=NEWSTRUCT(METAELEMENT);
	if (pMETA!=NULL) 
	{
		pMETA->pchHttpEquiv=NULL;
		pMETA->pchName=NULL;
		pMETA->fForua=FALSE;
		pMETA->pchContent=NULL;
		pMETA->pchScheme=NULL;
	}
	return pMETA;
}

pONEVENTELEMENT WML_CreateONEVENT (void) 
{
	pONEVENTELEMENT pONEV;

	pONEV=NEWSTRUCT(ONEVENTELEMENT);
	if (pONEV!=NULL) 
	{
		pONEV->iType=0;
		pONEV->pContent=NULL;
	}
	return pONEV;
}

pOPTGROUPELEMENT WML_CreateOPTGROUP (void) 
{
	pOPTGROUPELEMENT pOPTGRP;

	pOPTGRP=NEWSTRUCT(OPTGROUPELEMENT);
	if (pOPTGRP!=NULL) 
	{
		pOPTGRP->pTitle=NULL;
		pOPTGRP->pContent=NULL;
	}
	return pOPTGRP;
}

pOPTIONELEMENT WML_CreateOPTION (void) 
{
	pOPTIONELEMENT pOPTION;

	pOPTION=NEWSTRUCT(OPTIONELEMENT);
	if (pOPTION!=NULL) 
	{
		pOPTION->pValue=NULL;
		pOPTION->pTitle=NULL;
		pOPTION->pOnpick=NULL;
		pOPTION->pContent=NULL;
		pOPTION->fSelected=FALSE;
		pOPTION->fLastOption=FALSE;
	}
	return pOPTION;
}

pPELEMENT WML_CreateP (void) 
{
	pPELEMENT pP;

	pP=NEWSTRUCT(PELEMENT);
	if (pP!=NULL)
	{
		pP->iAlign=ALIGN_LEFT;
		pP->iMode=P_Unknown;
		pP->pContent=NULL;
	}
	return pP;
}

pPOSTFIELDELEMENT WML_CreatePOSTFIELD (void) 
{
	pPOSTFIELDELEMENT pPOSTFIELD;

	pPOSTFIELD=NEWSTRUCT(POSTFIELDELEMENT);
	if (pPOSTFIELD!=NULL)
	{
		pPOSTFIELD->pName=NULL;
		pPOSTFIELD->pValue=NULL;
	}
	return pPOSTFIELD;
}

pPREVELEMENT WML_CreatePREV (void) 
{
	pPREVELEMENT pPREV;

	pPREV=NEWSTRUCT(PREVELEMENT);
	if (pPREV!=NULL)
	{
		pPREV->pContent=NULL;
	}
	return pPREV;
}

pREFRESHELEMENT WML_CreateREFRESH (void) 
{
	pREFRESHELEMENT pREFRESH;

	pREFRESH=NEWSTRUCT(REFRESHELEMENT);
	if (pREFRESH!=NULL)
	{
		pREFRESH->pContent=NULL;
	}
	return pREFRESH;
}

pSELECTELEMENT WML_CreateSELECT (void) 
{
	pSELECTELEMENT pSELECT;

	pSELECT=NEWSTRUCT(SELECTELEMENT);
	if (pSELECT!=NULL) 
	{
		pSELECT->pTitle=NULL;
		pSELECT->pchName=NULL;
		pSELECT->pValue=NULL;
		pSELECT->pchIName=NULL;
		pSELECT->pIValue=NULL;
		pSELECT->fMultiple=FALSE;	
		pSELECT->iTabIndex=0;
		pSELECT->pContent=NULL;
	}
	return pSELECT;
}

pSETVARELEMENT WML_CreateSETVAR (void) 
{
	pSETVARELEMENT pSETVAR;

	pSETVAR=NEWSTRUCT(SETVARELEMENT);
	if (pSETVAR!=NULL) 
	{
		pSETVAR->pName=NULL;
		pSETVAR->pValue=NULL;
	}
	return pSETVAR;
}

pTABLEELEMENT WML_CreateTABLE (void) 
{
	pTABLEELEMENT pTABLE;

	pTABLE=NEWSTRUCT(TABLEELEMENT);
	if (pTABLE!=NULL) 
	{
		pTABLE->pTitle=NULL;
		pTABLE->pchAlign=NULL;
		pTABLE->iColumns=0;
		pTABLE->pContent=NULL;
	}
	return pTABLE;
}

pTDELEMENT WML_CreateTD (void) 
{
	pTDELEMENT pTD;

	pTD=NEWSTRUCT(TDELEMENT);
	if (pTD!=NULL) 
	{
		pTD->pContent=NULL;
	}
	return pTD;
}

pTEMPLATEELEMENT WML_CreateTEMPLATE (void) 
{
	pTEMPLATEELEMENT pTEMPLATE;

	pTEMPLATE=NEWSTRUCT(TEMPLATEELEMENT);
	if (pTEMPLATE!=NULL)
	{
		pTEMPLATE->pOnEntForw=NULL;
		pTEMPLATE->pOnEntBack=NULL;
		pTEMPLATE->pOnTimer=NULL;
		pTEMPLATE->pContent=NULL;
		pTEMPLATE->pEventTab=NULL;
	}
	return pTEMPLATE;
}

pTIMERELEMENT WML_CreateTIMER (void) 
{
	pTIMERELEMENT pTIMER;

	pTIMER=NEWSTRUCT(TIMERELEMENT);
	if (pTIMER!=NULL) 
	{
		pTIMER->pchName=NULL;
		pTIMER->pValue=NULL;
	}
	return pTIMER;
}

pTRELEMENT WML_CreateTR (void) 
{
	pTRELEMENT pTR;

	pTR=NEWSTRUCT(TRELEMENT);
	if (pTR!=NULL) 
	{
		pTR->pContent=NULL;
	}
	return pTR;
}

pWMLELEMENT WML_CreateWML (void) 
{
	pWMLELEMENT pWML;

	pWML=NEWSTRUCT(WMLELEMENT);
	if (pWML!=NULL)
	{
		pWML->pchXmllang=NULL;
		pWML->pContent=NULL;
	}
	return pWML;
}

/* Create super element of the desired type. Call the
   specific CreateX function to create the "body" of
   the element. If the CreateX returns NULL, remove 
   the super element.								*/
pELEMENTTYPE WML_CreateElement (UINT8 iType, UINT8 iViewID)
{
	pELEMENTTYPE pElmEnv;
	void *pElmEnvElm=NULL;

	pElmEnv=NEWSTRUCT(ELEMENTTYPE);
	if (pElmEnv!=NULL) 
	{
		pElmEnv->pNextElement=NULL;
		pElmEnv->iType=iType;
		pElmEnv->pElm=NULL;
		
		switch (iType) 
		{
		case Type_A:
			pElmEnvElm=WML_CreateA();
			break;
		case Type_ANCHOR:
			pElmEnvElm=WML_CreateANCHOR();
			break;
		case Type_ACCESS:
			pElmEnvElm=WML_CreateACCESS();
			break;
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
			pElmEnvElm=WML_CreateEMPH();
			break;
		case Type_BR: 
			pElmEnvElm=NULL;
			return pElmEnv;
		case Type_CARD: 
			pElmEnvElm=WML_CreateCARD();
			break;
		case Type_DO:
			pElmEnvElm=WML_CreateDO();
			break;
		case Type_FIELDSET:
			pElmEnvElm=WML_CreateFIELDSET();
			break;
		case Type_GO:
			pElmEnvElm=WML_CreateGO();
			break;
		case Type_HEAD:
			pElmEnvElm=WML_CreateHEAD();
			break;
		case Type_IMG:
			pElmEnvElm=WML_CreateIMG();
			break;
		case Type_INPUT:
			pElmEnvElm=WML_CreateINPUT();
			break;
		case Type_META:
			pElmEnvElm=WML_CreateMETA();
			break;
		case Type_NOOP: 
			/* No need for "body" of element. Return the
			   super element. */
			return pElmEnv;
		case Type_ONEVENT:
			pElmEnvElm=WML_CreateONEVENT();
			break;
		case Type_OPTGROUP:
			pElmEnvElm=WML_CreateOPTGROUP();
			break;
		case Type_OPTION:
			pElmEnvElm=WML_CreateOPTION();
			break;
		case Type_P:
			pElmEnvElm=WML_CreateP();
			break;
		case Type_POSTFIELD:
			pElmEnvElm=WML_CreatePOSTFIELD();
			break;
		case Type_PREV: 
			pElmEnvElm=WML_CreatePREV();
			break;
		case Type_REFRESH:
			pElmEnvElm=WML_CreateREFRESH();
			break;
		case Type_SELECT: 
			pElmEnvElm=WML_CreateSELECT();
			break;
		case Type_SETVAR:
			pElmEnvElm=WML_CreateSETVAR();
			break;
		case Type_TABLE:
			pElmEnvElm=WML_CreateTABLE();
			break;
		case Type_TD:
			pElmEnvElm=WML_CreateTD();
			break;
		case Type_TEMPLATE:
			pElmEnvElm=WML_CreateTEMPLATE();
			break;
		case Type_TIMER:
			pElmEnvElm=WML_CreateTIMER();
			break;
		case Type_TR:
			pElmEnvElm=WML_CreateTR();
			break;
		case Type_WML:
			pElmEnvElm=WML_CreateWML();
			break;
		default:
			/* Unknown token - send warning to the application. */
			CLNTa_error (iViewID,ERR_WAE_WML_UNKNOWN_TOKEN,
					ERRTYPE_INFORMATION);
			break;
		}

		/* If the element is created add it to the
		   super element, otherwise, delete the 
		   super element */
		if (pElmEnvElm!=NULL)
		{
			pElmEnv->pElm=pElmEnvElm;
		}
		else 
		{
			DEALLOC(&pElmEnv);
			pElmEnv=NULL;
		}
	}
	return pElmEnv;
}

/* Delete the content of an element */
void WML_DeleteContent (pELEMENTTYPE pElementObj) 
{
	if (pElementObj!=NULL) 
	{
		if (pElementObj->pNextElement!=NULL)
		{
			WML_DeleteContent(pElementObj->pNextElement);
		}
		WML_DeleteElement(pElementObj);
	}
}

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

	pCardElm=(CARDELEMENT *)(pCard->pElm);

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

/* Deletes the specified element, including the content */
void WML_DeleteElement (pELEMENTTYPE pElementObj) 
{
	if (pElementObj!=NULL) 
	{
		if (pElementObj->pElm!=NULL) 
		{
			switch (pElementObj->iType) 
			{
			case Type_A:
				Text_DeleteText(&((AELEMENT *)(pElementObj->pElm))->pTitle);
				Text_DeleteText(&((AELEMENT *)(pElementObj->pElm))->pHref);
				WML_DeleteContent(((AELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_ANCHOR:
				Text_DeleteText(&((ANCHORELEMENT *)(pElementObj->pElm))->pTitle);
				WML_DeleteContent(((ANCHORELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_ACCESS:
				DEALLOC(&(((ACCESSELEMENT *)(pElementObj->pElm))->pchDomain));
				DEALLOC(&(((ACCESSELEMENT *)(pElementObj->pElm))->pchPath));
				break;
			case Type_B:
			case Type_BIG:
			case Type_EM:
			case Type_I:
			case Type_SMALL:
			case Type_STRONG:
			case Type_U:
				WML_DeleteContent(((EMPHELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_BR:
				/* No attributes to delete */
				break;
			case Type_CARD: 
				DEALLOC(&(((CARDELEMENT *)(pElementObj->pElm))->pchId));
				Text_DeleteText(&((CARDELEMENT *)(pElementObj->pElm))->pTitle);
				WML_DeleteElement(((CARDELEMENT *)(pElementObj->pElm))->pOnEntForw);
				WML_DeleteElement(((CARDELEMENT *)(pElementObj->pElm))->pOnEntBack);
				WML_DeleteElement(((CARDELEMENT *)(pElementObj->pElm))->pOnTimer);
				WML_DeleteContent(((CARDELEMENT *)(pElementObj->pElm))->pContent);
				WML_DeleteEventTab(((CARDELEMENT *)(pElementObj->pElm))->pEventTab);
				WML_DeleteTemplateDO(((CARDELEMENT *)(pElementObj->pElm))->pTemplateDO);
				break;
			case Type_DO:
				DEALLOC(&(((DOELEMENT *)(pElementObj->pElm))->pchType));
				Text_DeleteText(&((DOELEMENT *)(pElementObj->pElm))->pLabel);
				DEALLOC(&(((DOELEMENT *)(pElementObj->pElm))->pchName));
				WML_DeleteContent(((DOELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_FIELDSET:
				Text_DeleteText(&((FIELDSETELEMENT *)(pElementObj->pElm))->pTitle);
				WML_DeleteContent(((FIELDSETELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_GO:
				Text_DeleteText(&((GOELEMENT *)(pElementObj->pElm))->pHref);
				DEALLOC(&(((GOELEMENT *)(pElementObj->pElm))->pchAccChar));
				WML_DeleteContent(((GOELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_HEAD:
				WML_DeleteContent(((HEADELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_IMG:
				Text_DeleteText(&((IMGELEMENT *)(pElementObj->pElm))->pAlt);
				Text_DeleteText(&((IMGELEMENT *)(pElementObj->pElm))->pSrc);
				Text_DeleteText(&((IMGELEMENT *)(pElementObj->pElm))->pLocalSrc);
				break;
			case Type_INPUT:
				DEALLOC(&(((INPUTELEMENT *)(pElementObj->pElm))->pchName));
				Text_DeleteText(&((INPUTELEMENT *)(pElementObj->pElm))->pValue);
				DEALLOC(&(((INPUTELEMENT *)(pElementObj->pElm))->pchFormat));
				Text_DeleteText(&((INPUTELEMENT *)(pElementObj->pElm))->pTitle);
				break;
			case Type_META:
				DEALLOC(&(((METAELEMENT *)(pElementObj->pElm))->pchHttpEquiv));
				DEALLOC(&(((METAELEMENT *)(pElementObj->pElm))->pchName));
				DEALLOC(&(((METAELEMENT *)(pElementObj->pElm))->pchContent));
				DEALLOC(&(((METAELEMENT *)(pElementObj->pElm))->pchScheme));
				break;
			case Type_NOOP: 
				/* No attributes to delete */
				break;
			case Type_ONEVENT:
				WML_DeleteContent(((ONEVENTELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_OPTGROUP:
				Text_DeleteText(&((OPTGROUPELEMENT *)(pElementObj->pElm))->pTitle);
				WML_DeleteContent(((OPTGROUPELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_OPTION:
				Text_DeleteText(&((OPTIONELEMENT *)(pElementObj->pElm))->pValue);
				Text_DeleteText(&((OPTIONELEMENT *)(pElementObj->pElm))->pTitle);
				WML_DeleteElement(((OPTIONELEMENT *)(pElementObj->pElm))->pOnpick);
				WML_DeleteContent(((OPTIONELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_P: 
				WML_DeleteContent(((PELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_POSTFIELD: 
				Text_DeleteText(&((POSTFIELDELEMENT *)(pElementObj->pElm))->pName);
				Text_DeleteText(&((POSTFIELDELEMENT *)(pElementObj->pElm))->pValue);
				break;
			case Type_PREV: 
				WML_DeleteContent(((PREVELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_REFRESH:
				WML_DeleteContent(((REFRESHELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_SELECT: 
				Text_DeleteText(&((SELECTELEMENT *)(pElementObj->pElm))->pTitle);
				DEALLOC(&(((SELECTELEMENT *)(pElementObj->pElm))->pchName));
				Text_DeleteText(&((SELECTELEMENT *)(pElementObj->pElm))->pValue);
				DEALLOC(&(((SELECTELEMENT *)(pElementObj->pElm))->pchIName));
				Text_DeleteText(&((SELECTELEMENT *)(pElementObj->pElm))->pIValue);
				WML_DeleteContent(((SELECTELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_SETVAR:
				Text_DeleteText(&((SETVARELEMENT *)(pElementObj->pElm))->pName);
				Text_DeleteText(&((SETVARELEMENT *)(pElementObj->pElm))->pValue);
				break;
			case Type_TABLE:
				Text_DeleteText(&((TABLEELEMENT *)(pElementObj->pElm))->pTitle);
				DEALLOC(&(((TABLEELEMENT *)(pElementObj->pElm))->pchAlign));
				WML_DeleteContent(((TABLEELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_TD:
				WML_DeleteContent(((TDELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_TEMPLATE:
				WML_DeleteElement(((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntForw);
				WML_DeleteElement(((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntBack);
				WML_DeleteElement(((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnTimer);
				WML_DeleteContent(((TEMPLATEELEMENT *)(pElementObj->pElm))->pContent);
				WML_DeleteEventTab(((TEMPLATEELEMENT *)(pElementObj->pElm))->pEventTab);
				break;
			case Type_Text:
				Text_DeleteText((TEXTELEMENT **)&(pElementObj->pElm));
				DEALLOC(&pElementObj);
				return;
			case Type_TIMER:	
				DEALLOC(&(((TIMERELEMENT *)(pElementObj->pElm))->pchName));
				Text_DeleteText(&((TIMERELEMENT *)(pElementObj->pElm))->pValue);
				break;
			case Type_TR:
				WML_DeleteContent(((TRELEMENT *)(pElementObj->pElm))->pContent);
				break;
			case Type_WML:
				DEALLOC(&(((WMLELEMENT *)(pElementObj->pElm))->pchXmllang));
				WML_DeleteContent(((WMLELEMENT *)(pElementObj->pElm))->pContent);
				break;
			}

			/* Delete element "body" */
			DEALLOC(&pElementObj->pElm);		
		}
		DEALLOC(&pElementObj);
	}
}


/* ======================================================= */
/* ======================================================= */
/*	  Attribute handling functions for the WML-elements.   */
/* ======================================================= */
/* ======================================================= */

void WML_SetAccChar (pELEMENTTYPE pElementObj, pTEXTELEMENT pAccChar)
{		
	if (pElementObj->iType==Type_GO) 
	{
		if (((GOELEMENT *)(pElementObj->pElm))->pchAccChar==NULL) 
		{
			((GOELEMENT *)(pElementObj->pElm))->pchAccChar=Text_ToString(&pAccChar);
			return;
		}
	}

	/* Delete string pAccChar */
	Text_DeleteText(&pAccChar);
}

void WML_SetAlign (pELEMENTTYPE pElementObj, pTEXTELEMENT pAlign, INT8 iAlign)
{
	/* Table - WCHAR* */
	if (pElementObj->iType==Type_TABLE) 
	{
		((TABLEELEMENT *)(pElementObj->pElm))->pchAlign=Text_ToString(&pAlign);
		return;
	}

	/* img and p - INT8 */
	if (iAlign==DEF_TYPE_STRING)
	{
		WCHAR *pchTemp=Text_ToString(&pAlign);

		/* Convert type to INT8 */
		iAlign=WML_ConvertAlign(&pchTemp);
	}
	else 
	{
		/* Delete string pAlign */
		Text_DeleteText(&pAlign);
	}

	/* Store */
	if (pElementObj->iType==Type_IMG) 
	{
		((IMGELEMENT *)(pElementObj->pElm))->iIAlign=iAlign;
	}
	else if (pElementObj->iType==Type_P) 
	{
		((PELEMENT *)(pElementObj->pElm))->iAlign=iAlign;
	}
}

void WML_SetAlt (pELEMENTTYPE pElementObj, pTEXTELEMENT pAlt)
{
	if (pElementObj->iType==Type_IMG) 
	{
		if (((IMGELEMENT *)(pElementObj->pElm))->pAlt==NULL) 
		{
			((IMGELEMENT *)(pElementObj->pElm))->pAlt=pAlt;
			return;
		}
	}

	/* Delete element pAlt */
	Text_DeleteText(&pAlt);
}

void WML_SetColumns (pELEMENTTYPE pElementObj, pTEXTELEMENT pColumns)
{
	UINT8 iCol=0;
	BOOL fIsPercent=FALSE;

	/* Convert pColumns (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iCol=WML_ConvertNumber(Text_ToString(&pColumns),&fIsPercent);

	if (pElementObj->iType==Type_TABLE) 
	{
		((TABLEELEMENT *)(pElementObj->pElm))->iColumns=iCol;
	} 
}

void WML_SetContent (pELEMENTTYPE pElementObj, pTEXTELEMENT pContent)
{
	if (pElementObj->iType==Type_META) 
	{
		if (((METAELEMENT *)(pElementObj->pElm))->pchContent==NULL) 
		{
			((METAELEMENT *)(pElementObj->pElm))->pchContent=Text_ToString(&pContent);
			return;
		}
	}

	/* Delete string pchContent */
	Text_DeleteText(&pContent);
}

void WML_SetDomain (pELEMENTTYPE pElementObj, pTEXTELEMENT pDomain)
{
	if (pElementObj->iType==Type_ACCESS) 
	{
		if (((ACCESSELEMENT *)(pElementObj->pElm))->pchDomain==NULL) 
		{
			((ACCESSELEMENT *)(pElementObj->pElm))->pchDomain=Text_ToString(&pDomain);
			return;
		}
	}

	/* Delete string pDomain */
	Text_DeleteText(&pDomain);
}

void WML_SetEmptyOK (pELEMENTTYPE pElementObj, BOOL fEmptyOK, pTEXTELEMENT pEmptyOK)
{
	BOOL fResult=fEmptyOK;

	if (pElementObj->iType==Type_INPUT) 
	{
		if (pEmptyOK!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pEmptyOK));
		}

		((INPUTELEMENT *)(pElementObj->pElm))->fEmptyOK=fResult;
		return;
	} 

	/* Delete string pEmptyOK */
	Text_DeleteText(&pEmptyOK);
}

void WML_SetFormat (pELEMENTTYPE pElementObj, pTEXTELEMENT pFormat)
{
	if (pElementObj->iType==Type_INPUT) 
	{
		/* Remove old input format */
		if (((INPUTELEMENT *)(pElementObj->pElm))->pchFormat!=NULL) 
		{
			DEALLOC(&(((INPUTELEMENT *)(pElementObj->pElm))->pchFormat)) 
		}

		((INPUTELEMENT *)(pElementObj->pElm))->pchFormat=Text_ToString(&pFormat);
		return;
	} 

	/* Delete string pFormat */
	Text_DeleteText(&pFormat);
}

void WML_SetForua (pELEMENTTYPE pElementObj, BOOL fForua, pTEXTELEMENT pForua)
{
	BOOL fResult=fForua;

	if (pElementObj->iType==Type_META) 
	{
		if (pForua!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pForua));
		}

		((METAELEMENT *)(pElementObj->pElm))->fForua=fResult;
		return;
	} 

	/* Delete string pForua */
	Text_DeleteText(&pForua);
}

void WML_SetHeight (pELEMENTTYPE pElementObj, pTEXTELEMENT pHeight)
{
	UINT16 iHeight=0;
	BOOL fIsPercent=FALSE;

	/* Convert height (text) to UINT16. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iHeight=WML_ConvertNumber16(Text_ToString(&pHeight),&fIsPercent);

	if (pElementObj->iType==Type_IMG)
	{	
		if (fIsPercent)
		{
			((IMGELEMENT *)(pElementObj->pElm))->iIsPercent|=HEIGHT_IS_PERCENT;
		}

		((IMGELEMENT *)(pElementObj->pElm))->iHeight=iHeight;
	} 
}

void WML_SetHref(pELEMENTTYPE pElementObj, pTEXTELEMENT pHref)
{
	if (pElementObj->iType==Type_A) 
	{
		if (((AELEMENT *)(pElementObj->pElm))->pHref==NULL) 
		{
			((AELEMENT *)(pElementObj->pElm))->pHref=pHref;
			return;
		}
	} 
	else if (pElementObj->iType==Type_GO) 
	{
		if (((GOELEMENT *)(pElementObj->pElm))->pHref==NULL) 
		{
			((GOELEMENT *)(pElementObj->pElm))->pHref=pHref;
			return;
		}
	} 

	/* Delete string pHref */
	Text_DeleteText(&pHref);
}

void WML_SetHSpace (pELEMENTTYPE pElementObj, pTEXTELEMENT pHSpace)
{
	UINT8 iHSpace=0;
	BOOL fIsPercent=FALSE;

	/* Convert HSpace (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iHSpace=WML_ConvertNumber(Text_ToString(&pHSpace),&fIsPercent);

	if (pElementObj->iType==Type_IMG) 
	{
		if (fIsPercent)
		{
			((IMGELEMENT *)(pElementObj->pElm))->iIsPercent|=HSPACE_IS_PERCENT;
		}

		((IMGELEMENT *)(pElementObj->pElm))->iHSpace=iHSpace;
	} 
}

void WML_SetHttpEq(pELEMENTTYPE pElementObj, pTEXTELEMENT pHttpEq)
{
	if (pElementObj->iType==Type_META) 
	{
		if (((METAELEMENT *)(pElementObj->pElm))->pchHttpEquiv==NULL) 
		{
			((METAELEMENT *)(pElementObj->pElm))->pchHttpEquiv=Text_ToString(&pHttpEq);
			return;
		}
	} 

	/* Delete string pFormat */
	Text_DeleteText(&pHttpEq);
}

void WML_SetId (pELEMENTTYPE pElementObj, pTEXTELEMENT pId)
{
	if (pElementObj->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElementObj->pElm))->pchId==NULL) 
		{
			((CARDELEMENT *)(pElementObj->pElm))->pchId=Text_ToString(&pId);
			return;
		}
	} 

	/* Delete string pFormat */
	Text_DeleteText(&pId);
}

void WML_SetIValue (pELEMENTTYPE pElementObj, pTEXTELEMENT pIValue)
{
	if (pElementObj->iType==Type_SELECT) 
	{
		if (((SELECTELEMENT *)(pElementObj->pElm))->pIValue==NULL) 
		{
			((SELECTELEMENT *)(pElementObj->pElm))->pIValue=pIValue;
			return;
		}
	}

	/* Delete element pIValue */
	Text_DeleteText(&pIValue);
}

void WML_SetIName (pELEMENTTYPE pElementObj, pTEXTELEMENT pIName)
{
	if (pElementObj->iType==Type_SELECT) 
	{
		if (((SELECTELEMENT *)(pElementObj->pElm))->pchIName==NULL) 
		{
			((SELECTELEMENT *)(pElementObj->pElm))->pchIName=Text_ToString(&pIName);
			return;
		}
	}

	/* Delete element pIName */
	Text_DeleteText(&pIName);
}

void WML_SetLabel (pELEMENTTYPE pElementObj, pTEXTELEMENT pLabel)
{
	if (pElementObj->iType==Type_DO) 
	{
		if (((DOELEMENT *)(pElementObj->pElm))->pLabel==NULL) 
		{
			((DOELEMENT *)(pElementObj->pElm))->pLabel=pLabel;
			return;
		}
	}

	/* Delete element pLabel */
	Text_DeleteText(&pLabel);
}

void WML_SetLocalSrc (pELEMENTTYPE pElementObj, pTEXTELEMENT pLocalSrc)
{
	if (pElementObj->iType==Type_IMG) 
	{
		if (((IMGELEMENT *)(pElementObj->pElm))->pLocalSrc==NULL) 
		{
			((IMGELEMENT *)(pElementObj->pElm))->pLocalSrc=pLocalSrc;
			return;
		}
	} 

	/* Delete element pLocalSrc */
	Text_DeleteText(&pLocalSrc);
}

void WML_SetMaxLength (pELEMENTTYPE pElementObj, pTEXTELEMENT pLength)
{
	UINT32 iLength=0;

	/* Convert length (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iLength=WML_ConvertNumber32(Text_ToString(&pLength));

	if (pElementObj->iType==Type_INPUT) 
	{
		if (iLength>127) 
		{
			((INPUTELEMENT *)(pElementObj->pElm))->iMaxLength=-1;
		}
		else
		{
			((INPUTELEMENT *)(pElementObj->pElm))->iMaxLength=(INT8)iLength;
		}
	} 
}

void WML_SetMethod (pELEMENTTYPE pElementObj, enum MethodType iMethod, pTEXTELEMENT pMethod)
{
	enum MethodType iResult=iMethod;

	if (pElementObj->iType==Type_GO) 
	{
		if (pMethod!=NULL)
		{
			iResult=WML_ConvertMethod(Text_ToString(&pMethod));
		}

		((GOELEMENT *)(pElementObj->pElm))->iMethod=iResult;
		return;
	} 

	/* Delete string pMethod */
	Text_DeleteText(&pMethod);
}

void WML_SetMode (pELEMENTTYPE pElementObj, enum WrapMode iMode, pTEXTELEMENT pMode)
{
	enum WrapMode iResult=iMode;

	if (pElementObj->iType==Type_P) 
	{
		if (pMode!=NULL)
		{
			iResult=WML_ConvertMode(Text_ToString(&pMode));
		}

		((PELEMENT *)(pElementObj->pElm))->iMode=iResult;
		return;
	} 

	/* Delete string pMode */
	Text_DeleteText(&pMode);
}

void WML_SetMultiple (pELEMENTTYPE pElementObj, BOOL fMultiple, pTEXTELEMENT pMultiple)
{
	BOOL fResult=fMultiple;

	if (pElementObj->iType==Type_SELECT) 
	{
		if (pMultiple!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pMultiple));
		}

		((SELECTELEMENT *)(pElementObj->pElm))->fMultiple=fResult;
		return;
	} 

	/* Delete string pMultiple */
	Text_DeleteText(&pMultiple);
}

void WML_SetName (pELEMENTTYPE pElementObj, pTEXTELEMENT pName)
{
	switch (pElementObj->iType)
	{
	case Type_DO:
		if (((DOELEMENT *)(pElementObj->pElm))->pchName==NULL) 
		{
			((DOELEMENT *)(pElementObj->pElm))->pchName=Text_ToString(&pName);
			return;
		}
		break;
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElementObj->pElm))->pchName==NULL) 
		{
			((INPUTELEMENT *)(pElementObj->pElm))->pchName=Text_ToString(&pName);
			return;
		}
		break;
	case Type_META:
		if (((METAELEMENT *)(pElementObj->pElm))->pchName==NULL) 
		{
			((METAELEMENT *)(pElementObj->pElm))->pchName=Text_ToString(&pName);
			return;
		}
		break;
	case Type_POSTFIELD:
		if (((POSTFIELDELEMENT *)(pElementObj->pElm))->pName==NULL) 
		{
			((POSTFIELDELEMENT *)(pElementObj->pElm))->pName=pName;
			return;
		}
		break;
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElementObj->pElm))->pchName==NULL) 
		{
			((SELECTELEMENT *)(pElementObj->pElm))->pchName=Text_ToString(&pName);
			return;
		}
		break;
	case Type_SETVAR:
		if (((SETVARELEMENT *)(pElementObj->pElm))->pName==NULL) 
		{
			((SETVARELEMENT *)(pElementObj->pElm))->pName=pName;
			return;
		}
		break;
	case Type_TIMER:
		if (((TIMERELEMENT *)(pElementObj->pElm))->pchName==NULL) 
		{
			((TIMERELEMENT *)(pElementObj->pElm))->pchName=Text_ToString(&pName);
			return;
		}
		break;
	}
	/* Delete string pName */
	Text_DeleteText(&pName);
}

void WML_SetNewContext (pELEMENTTYPE pElementObj, BOOL fContext, pTEXTELEMENT pNewContext)
{
	BOOL fResult=fContext;

	if (pElementObj->iType==Type_CARD) 
	{
		if (pNewContext!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pNewContext));
		}

		((CARDELEMENT *)(pElementObj->pElm))->fNewContext=fResult;
		return;
	} 

	/* Delete string pNewContext */
	Text_DeleteText(&pNewContext);
}

void WML_SetOnEntBack (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL)
{
	if (pElementObj->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElementObj->pElm))->pOnEntBack==NULL) 
		{
			((CARDELEMENT *)(pElementObj->pElm))->pOnEntBack=*ppURL;
			return;
		}
	} 
	else if (pElementObj->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntBack==NULL) 
		{
			((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntBack=*ppURL;
			return;
		}
	}

	/* Delete element *ppURL */
	WML_DeleteElement(*ppURL);
	*ppURL=NULL;
}

void WML_SetOnEntForw (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL)
{
	if (pElementObj->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElementObj->pElm))->pOnEntForw==NULL) 
		{
			((CARDELEMENT *)(pElementObj->pElm))->pOnEntForw=*ppURL;
			return;
		}
	} 
	else if (pElementObj->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntForw==NULL) 
		{
			((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnEntForw=*ppURL;
			return;
		}
	}

	/* Delete element *ppURL */
	WML_DeleteElement(*ppURL);
	*ppURL=NULL;
}

void WML_SetOnPick (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL)
{
	if (pElementObj->iType==Type_OPTION) 
	{
		if (((OPTIONELEMENT *)(pElementObj->pElm))->pOnpick==NULL) 
		{
			((OPTIONELEMENT *)(pElementObj->pElm))->pOnpick=*ppURL;
			return;
		}
	}

	/* Delete element pURL */
	WML_DeleteElement(*ppURL);
	*ppURL=NULL;
}

void WML_SetOnTimer (pELEMENTTYPE pElementObj, pELEMENTTYPE *ppURL)
{
	if (pElementObj->iType==Type_CARD) 
	{
		if (((CARDELEMENT *)(pElementObj->pElm))->pOnTimer==NULL) 
		{
			((CARDELEMENT *)(pElementObj->pElm))->pOnTimer=*ppURL;
			return;
		}
	} 
	else if (pElementObj->iType==Type_TEMPLATE) 
	{
		if (((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnTimer==NULL) 
		{
			((TEMPLATEELEMENT *)(pElementObj->pElm))->pOnTimer=*ppURL;
			return;
		}
	} 

	/* Delete element *pURL */
	WML_DeleteElement(*ppURL);
	*ppURL=NULL;
}

void WML_SetOptional (pELEMENTTYPE pElementObj, BOOL fOptional, pTEXTELEMENT pOptional)
{
	BOOL fResult=fOptional;

	if (pElementObj->iType==Type_DO) 
	{
		if (pOptional!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pOptional));
		}

		((DOELEMENT *)(pElementObj->pElm))->fOptional=fResult;
		return;
	} 

	/* Delete string pOptional */
	Text_DeleteText(&pOptional);
}

void WML_SetPath (pELEMENTTYPE pElementObj, pTEXTELEMENT pPath)
{
	if (pElementObj->iType==Type_ACCESS) 
	{
		if (((ACCESSELEMENT *)(pElementObj->pElm))->pchPath==NULL) 
		{
			((ACCESSELEMENT *)(pElementObj->pElm))->pchPath=Text_ToString(&pPath);
			return;
		}
	}

	/* Delete string pPath */
	Text_DeleteText(&pPath);
}

void WML_SetScheme (pELEMENTTYPE pElementObj, pTEXTELEMENT pScheme)
{
	if (pElementObj->iType==Type_META) 
	{
		if (((METAELEMENT *)(pElementObj->pElm))->pchScheme==NULL) 
		{
			((METAELEMENT *)(pElementObj->pElm))->pchScheme=Text_ToString(&pScheme);
			return;
		}
	}

	/* Delete string pchScheme */
	Text_DeleteText(&pScheme);	
}

void WML_SetSendReferer (pELEMENTTYPE pElementObj, BOOL fSendRef, pTEXTELEMENT pSendRef)
{
	BOOL fResult=fSendRef;

	if (pElementObj->iType==Type_GO) 
	{
		if (pSendRef!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pSendRef));
		}

		((GOELEMENT *)(pElementObj->pElm))->fSendRef=fResult;
		return;
	} 

	/* Delete string pSendRef */
	Text_DeleteText(&pSendRef);
}

void WML_SetSize (pELEMENTTYPE pElementObj, pTEXTELEMENT pSize)
{
	UINT32 iSize=0;

	/* Convert size (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iSize=WML_ConvertNumber32(Text_ToString(&pSize));
	
	if (pElementObj->iType==Type_INPUT) 
	{

		if (iSize>127) 
		{
			((INPUTELEMENT *)(pElementObj->pElm))->iSize=-1;
		}
		else
		{
			((INPUTELEMENT *)(pElementObj->pElm))->iSize=(INT8)iSize;
		}
	}
}

void WML_SetSrc (pELEMENTTYPE pElementObj, pTEXTELEMENT pHref)
{
	if (pElementObj->iType==Type_IMG) 
	{
		if (((IMGELEMENT *)(pElementObj->pElm))->pSrc==NULL) 
		{
			((IMGELEMENT *)(pElementObj->pElm))->pSrc=pHref;
			return;
		}
	} 

	/* Delete element pHref */
	Text_DeleteText(&pHref);
}

void WML_SetOrdered (pELEMENTTYPE pElementObj, BOOL fOrdered, pTEXTELEMENT pOrdered)
{
	BOOL fResult=fOrdered;

	if (pElementObj->iType==Type_CARD) 
	{
		if (pOrdered!=NULL)
		{
			fResult=WML_ConvertBool(Text_ToString(&pOrdered));
		}

		((CARDELEMENT *)(pElementObj->pElm))->fOrdered=fResult;
		return;
	} 

	/* Delete string pOrdered */
	Text_DeleteText(&pOrdered);
}

void WML_SetTabIndex (pELEMENTTYPE pElementObj, pTEXTELEMENT pTabIndex)
{		
	UINT8 iTabIndex=0;
	BOOL fIsPercent=FALSE;

	/* Convert tabindex (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iTabIndex=WML_ConvertNumber(Text_ToString(&pTabIndex),&fIsPercent);

	if (pElementObj->iType==Type_INPUT) 
	{
		((INPUTELEMENT *)(pElementObj->pElm))->iTabIndex=iTabIndex;
	} 
	else if (pElementObj->iType==Type_SELECT) 
	{
		((SELECTELEMENT *)(pElementObj->pElm))->iTabIndex=iTabIndex;
	}
}

void WML_SetTitle (pELEMENTTYPE pElementObj, pTEXTELEMENT pTitle)
{
	switch (pElementObj->iType)
	{
	case Type_A:
	case Type_ANCHOR:
		if (((AELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((AELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_CARD:
		if (((CARDELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((CARDELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_FIELDSET:
		if (((FIELDSETELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((FIELDSETELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((INPUTELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_OPTGROUP:
		if (((OPTGROUPELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((OPTGROUPELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_OPTION:
		if (((OPTIONELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((OPTIONELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((SELECTELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	case Type_TABLE:
		if (((TABLEELEMENT *)(pElementObj->pElm))->pTitle==NULL) 
		{
			((TABLEELEMENT *)(pElementObj->pElm))->pTitle=pTitle;
			return;
		}
		break;
	}

	/* Delete element pTitle */
	Text_DeleteText(&pTitle);
}

void WML_SetType (pELEMENTTYPE pElementObj, INT8 iType, WCHAR *pchType, BOOL fWTA)
{
	switch (pElementObj->iType)
	{
	case Type_DO:
		if ((iType==DEF_TYPE_STRING)&&(pchType!=NULL))
		{
			/* Remove old type (default value) */
			if (((DOELEMENT *)(pElementObj->pElm))->pchType!=NULL) 
			{
				DEALLOC(&((DOELEMENT *)(pElementObj->pElm))->pchType);
			}

			((DOELEMENT *)(pElementObj->pElm))->pchType=pchType;
			return;
		} 
		break;
	case Type_INPUT:
		/* Only predefined events -> convert to integer */
		if (iType==DEF_TYPE_STRING)
		{
			iType=WML_ConvertType(&pchType);
		}

		((INPUTELEMENT *)(pElementObj->pElm))->iType=iType;
		return;
	case Type_ONEVENT:

		/* Check WTA events if WTA enabled AND the user agent
		   is a WTA user agent */
		#ifdef CONFIG_WTA
		if ((iType==DEF_TYPE_STRING)&&(fWTA))
		{
			/* Convert WTA event */
			iType=WTA_ConvertEventToInt(pchType);

			/* The function WTA_ConvertEventToInt returns 
			   DEF_TYPE_UNKNOWN if the string was not a WTA event.
			   In this case the WML_ConvertType should be called.
			   Otherwise the string should be deleted. */
			if (iType!=DEF_TYPE_UNKNOWN)
			{
				/* Delete string */
				DEALLOC(&pchType);
			}
			else
			{
				/* Set the type to DEF_TYPE_STRING which makes
				   the code below handle the conversion as if the 
				   WTA was not enabled. */
				iType=DEF_TYPE_STRING;
			}
		}
        #else
            /* Added by GBU,991214 */
            /*  To get rid of warnings for unused variables */
            fWTA = fWTA;
		#endif

		/* Only predefined events -> convert to integer */
		if (iType==DEF_TYPE_STRING)
		{
			iType=WML_ConvertType(&pchType);
		}

		((ONEVENTELEMENT *)(pElementObj->pElm))->iType=iType;
		return;
	}

	/* Delete string pchType */
	DEALLOC(&pchType);
}

void WML_SetValue (pELEMENTTYPE pElementObj, pTEXTELEMENT pValue)
{
	switch (pElementObj->iType)
	{
	case Type_INPUT:
		if (((INPUTELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((INPUTELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	case Type_OPTION:
		if (((OPTIONELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((OPTIONELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	case Type_POSTFIELD:
		if (((POSTFIELDELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((POSTFIELDELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	case Type_SELECT:
		if (((SELECTELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((SELECTELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	case Type_SETVAR:
		if (((SETVARELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((SETVARELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	case Type_TIMER:
		if (((TIMERELEMENT *)(pElementObj->pElm))->pValue==NULL) 
		{
			((TIMERELEMENT *)(pElementObj->pElm))->pValue=pValue;
			return;
		}
	}

	/* Delete element pValue */
	Text_DeleteText(&pValue);
}

void WML_SetVSpace (pELEMENTTYPE pElementObj, pTEXTELEMENT pVSpace)
{
	UINT8 iVSpace=0;
	BOOL fIsPercent=FALSE;

	/* Convert VSpace (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iVSpace=WML_ConvertNumber(Text_ToString(&pVSpace),&fIsPercent);

	if (pElementObj->iType==Type_IMG) 
	{
		if (fIsPercent)
		{
			((IMGELEMENT *)(pElementObj->pElm))->iIsPercent|=VSPACE_IS_PERCENT;
		}

		((IMGELEMENT *)(pElementObj->pElm))->iVSpace=iVSpace;
		return;
	} 
}

void WML_SetWidth (pELEMENTTYPE pElementObj, pTEXTELEMENT pWidth)
{
	UINT16 iWidth=0;
	BOOL fIsPercent=FALSE;

	/* Convert VSpace (text) to UINT8. The TEXTELEMENT is removed
	   in the Text_ToString function and the string is removed
	   in the ConvertNumber function. */
	iWidth=WML_ConvertNumber16(Text_ToString(&pWidth),&fIsPercent);

	if (pElementObj->iType==Type_IMG) 
	{
		if (fIsPercent)
		{
			((IMGELEMENT *)(pElementObj->pElm))->iIsPercent|=WIDTH_IS_PERCENT;
		}
		
		((IMGELEMENT *)(pElementObj->pElm))->iWidth=iWidth;
		return;
	}
}

void WML_SetXmllang (pELEMENTTYPE pElementObj, pTEXTELEMENT pXmllang)
{
	if (pElementObj->iType==Type_WML) 
	{
		((WMLELEMENT *)(pElementObj->pElm))->pchXmllang=Text_ToString(&pXmllang);
		return;
	}

	/* Delete element pXmllang */
	Text_DeleteText(&pXmllang);
}

/* ----------------------------------------------------------
	Functions for handling the contents in the WML-elements.
   ---------------------------------------------------------- */

/* Help function for WML_AddContent. */
void WML_AddContentToEnd (pELEMENTTYPE *ppContentPtr, pELEMENTTYPE pContent)
{
	pELEMENTTYPE pTemp=NULL;

	if (*ppContentPtr==NULL)
	{
		*ppContentPtr=pContent;
	}
	else
	{
		pTemp=*ppContentPtr;

		while ((pTemp->pNextElement)!=NULL)
		{
			pTemp=pTemp->pNextElement;
		}
		pTemp->pNextElement=pContent;
	}
}

void WML_AddContent (pELEMENTTYPE pElementObj, pELEMENTTYPE pContent) 
{
	if (pElementObj!=NULL) 
	{
		switch (pElementObj->iType) 
		{
		case Type_A:
			WML_AddContentToEnd(&((AELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_ANCHOR:
			WML_AddContentToEnd(&((ANCHORELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_B:
		case Type_BIG:
		case Type_EM:
		case Type_I:
		case Type_SMALL:
		case Type_STRONG:
		case Type_U:
			WML_AddContentToEnd(&((EMPHELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_CARD:
			WML_AddContentToEnd(&((CARDELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_DO:
			WML_AddContentToEnd(&((DOELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_FIELDSET:
			WML_AddContentToEnd(&((FIELDSETELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_GO:
			WML_AddContentToEnd(&((GOELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_HEAD:
			WML_AddContentToEnd(&((HEADELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_ONEVENT:
			WML_AddContentToEnd(&((ONEVENTELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_OPTGROUP:
			WML_AddContentToEnd(&((OPTGROUPELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_OPTION:
			WML_AddContentToEnd(&((OPTIONELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_P:
			WML_AddContentToEnd(&((PELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_PREV:
			WML_AddContentToEnd(&((PREVELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_REFRESH:
			WML_AddContentToEnd(&((REFRESHELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_SELECT:
			WML_AddContentToEnd(&((SELECTELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_TABLE:
			WML_AddContentToEnd(&((TABLEELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_TD:
			WML_AddContentToEnd(&((TDELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_TEMPLATE:
			WML_AddContentToEnd(&((TEMPLATEELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_TR:
			WML_AddContentToEnd(&((TRELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		case Type_WML:
			WML_AddContentToEnd(&((WMLELEMENT *)(pElementObj->pElm))->pContent,pContent);
			return;
		}
	}
	/* Wrong type - delete content. */
	WML_DeleteContent(pContent);
}

pELEMENTTYPE WML_GetContent (pELEMENTTYPE pElementObj) 
{
	if (pElementObj!=NULL) 
	{
		if (pElementObj->pElm!=NULL)
		{
			switch (pElementObj->iType) 
			{
			case Type_A:
				return ((AELEMENT *)(pElementObj->pElm))->pContent;
			case Type_ANCHOR:
				return ((ANCHORELEMENT *)(pElementObj->pElm))->pContent;
			case Type_B:
			case Type_BIG:
			case Type_EM:
			case Type_I:
			case Type_SMALL:
			case Type_STRONG:
			case Type_U:
				return ((EMPHELEMENT *)(pElementObj->pElm))->pContent;
			case Type_CARD:
				return ((CARDELEMENT *)(pElementObj->pElm))->pContent;
			case Type_DO:
				return ((DOELEMENT *)(pElementObj->pElm))->pContent;
			case Type_FIELDSET:
				return ((FIELDSETELEMENT *)(pElementObj->pElm))->pContent;
			case Type_GO:
				return ((GOELEMENT *)(pElementObj->pElm))->pContent;
			case Type_HEAD:
				return ((HEADELEMENT *)(pElementObj->pElm))->pContent;
			case Type_ONEVENT:
				return ((ONEVENTELEMENT *)(pElementObj->pElm))->pContent;
			case Type_OPTGROUP:
				return ((OPTGROUPELEMENT *)(pElementObj->pElm))->pContent;
			case Type_OPTION:
				return ((OPTIONELEMENT *)(pElementObj->pElm))->pContent;
			case Type_P:
				return ((PELEMENT *)(pElementObj->pElm))->pContent;
			case Type_PREV:
				return ((PREVELEMENT *)(pElementObj->pElm))->pContent;
			case Type_REFRESH:
				return ((REFRESHELEMENT *)(pElementObj->pElm))->pContent;
			case Type_SELECT:
				return ((SELECTELEMENT *)(pElementObj->pElm))->pContent;
			case Type_TABLE:
				return ((TABLEELEMENT *)(pElementObj->pElm))->pContent;
			case Type_TD:
				return ((TDELEMENT *)(pElementObj->pElm))->pContent;
			case Type_TEMPLATE:
				return ((TEMPLATEELEMENT *)(pElementObj->pElm))->pContent;
			case Type_TR:
				return ((TRELEMENT *)(pElementObj->pElm))->pContent;
			case Type_WML:
				return ((WMLELEMENT *)(pElementObj->pElm))->pContent;
			}
		}
	}
	return NULL;
}

/* --------------------------------------------- */

#endif




