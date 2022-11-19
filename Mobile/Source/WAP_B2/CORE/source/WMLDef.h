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
	FILE: WMLDef.h

	Definition file for WML-specific parts of the WBXML-decoding. 

	Rev history:
		980708	JPR		Created
		990210	JPR		Modified
		990316	JPR		Major changes to support general WBXML-
						decoding.
		990326	JPR		WAP 1.1
		990526	JPR		Paragraph updates
		990531	JPR		Navigation variable handling updated
		990609	JPR		Trim text additions
		991028	JPR		WTA Specific addition (comment)

   ================================================================== */

#ifndef _DEFINITIONS
#define _DEFINITIONS

#include "CmmnRsrc.h"
#include "aapiclnt.h"
#include "WBXMLDef.h"
#include "WAEDef.h"

/* ========= AUTHENTICATION FUNCTIONS ==========
	Functions for handling authorization, i.e. 
	keep track of the visited realms, and the 
	appropriate passwords, etc.
   ============================================= */

/*	Authentication function structs.
	These structs are used to store information about
	realms and the appropriate user-id and passwords
	used in these realms.

	Realm struct
	------------
	BYTE *pbRealm			: The Realm (Text)
	BYTE *pchRelativeURL	: URL; Relative to Host URL
	BYTE *pbCredentials		: User-id + Password 
							  (e.g. Usr8(EOT)XcfF33F(EOT) )
	UINT8 iCredLength		: Length of User-id + Password 
							  (nbr of bytes including EOT)
	struct tREALMSTR pNext	: Next Realm					


	Host struct
	-----------
	BYTE *pchURL			: Host URL
	pREALMSTR pRealmList	: List of Realms under this host
	struct tHOSTSTR pNext	: Next Host							*/


typedef struct tREALMSTR
{
	BYTE *pbRealm;
	BYTE *pbRelativeURL;
	BYTE *pbCredentials;
	UINT8 iCredLength;
	struct tREALMSTR *pNext;
} REALMSTR, *pREALMSTR;

typedef struct tHOSTSTR
{
	BYTE *pbURL;
	pREALMSTR pRealmList;
	struct tHOSTSTR *pNext;
} HOSTSTR, *pHOSTSTR;

/* ============================================= */

/* TYPE codes */
#define DEF_TYPE_HOME				8
#define DEF_TYPE_UNKNOWN			10
#define DEF_TYPE_ONENTERFORWARD		11
#define DEF_TYPE_ONENTERBACKWARD	12
#define DEF_TYPE_ONTIMER			13	
#define DEF_TYPE_ONCLICK			14
#define DEF_TYPE_STRING				15
#define DEF_TYPE_TEXT				16
#define DEF_TYPE_PASSWORD			17
#define DEF_TYPE_PREV				18	
#define DEF_TYPE_NOEVENT			19				

/* WTA Specific 
   Other events besides onenterforward, ontenterbackward, ontimer,
   and opick are defiend in the WMLToken.h file. The numbers there
   starts at 0x20.
*/

enum MethodType {M_Post='\x60', M_Get='\x40'};
enum WrapMode {P_Wrap, P_NoWrap, P_Unknown};	

/* Trim text modes */
#define TRIMTEXT_NONE 	0
#define TRIMTEXT_LEFT	1
#define TRIMTEXT_RIGHT 	2
#define TRIMTEXT_BOTH	3

/* ------------------------------------------
   Variable table definitions.
   To keep track of the variables during run
   time.
   ------------------------------------------ */
typedef struct tVARTABELM
{
	WCHAR *pchName;
	WCHAR *pchValue;
	struct tVARTABELM *pNextVar;
} VARTABELM, *pVARTABELM;

/* ------------------------------------------
   Event Table
   On each CARD and TEMPLATE there is an event
   table. When an event occurs, this table is
   scanned for the appropriate action. If the
   event doesn't exist at the CARD level, the
   TEMPLATE table is checked. (consult
   HandleEvent in Interface.h)
   EVENTS:	ONTIMER, ONCLICK, ONENTERFORWARD, 
			ONENTERBACKWARD, WTA EVENTS, etc
			(Not DO)
   ------------------------------------------ */
typedef struct tEVENT 
{
	INT8 iEvent;
	pELEMENTTYPE pAction;
	struct tEVENT *pNextEvent;
} EVENT, *pEVENT;


/* ------------------------------------------
   Used on CARD level.
   The DO elements in the TEMPLATE element 
   which are not overridden on the CARD level.
   The pDO is a pointer to the TEMPLATE DO -
   DO NOT DEALLOCATE!
   ------------------------------------------ */
typedef struct tTEMPLATEDOTYPE
{
	pELEMENTTYPE pDO;
	struct tTEMPLATEDOTYPE *pNext;
} TEMPLATEDOTYPE, *pTEMPLATEDOTYPE;
 
/* ------------------------------------------
    History List
   ------------------------------------------ */
typedef struct tHISTORY 
{
	BYTE *pbEntry;
	WCHAR *pchTitle;
	struct tHISTORY *pNextEntry;
} HISTORY, *pHISTORY;

/* body for b, big, em, i, small, strong, u - element */
typedef struct
{
	pELEMENTTYPE pContent;				/* (%flow);* */
} EMPHELEMENT, *pEMPHELEMENT;

/* a element */
typedef struct
{
	pTEXTELEMENT pTitle;
	pTEXTELEMENT pHref;					/* HREF */
	pELEMENTTYPE pContent;				/* (#PCDATA;|br|img)* */
} AELEMENT, *pAELEMENT;

/* anchor element */
typedef struct
{
	pTEXTELEMENT pTitle;
	pELEMENTTYPE pContent;				/* (#PCDATA;|br|img|go|prev|refresh)* */
} ANCHORELEMENT, *pANCHORELEMENT;

/* access element */
typedef struct 
{
	WCHAR *pchDomain;					/* CDATA */
	WCHAR *pchPath;						/* CDATA */
} ACCESSELEMENT, *pACCESSELEMENT;

/* br element - no body */

/* card element */
typedef struct 
{
	WCHAR *pchId;						/* ID */
	pTEXTELEMENT pTitle;				/* %vdata */
	BOOL fNewContext;
	BOOL fOrdered;
	pELEMENTTYPE pOnEntForw;			/* URL */
	pELEMENTTYPE pOnEntBack;			/* URL */
	pELEMENTTYPE pOnTimer;				/* URL */
	pELEMENTTYPE pContent;				/* (%fields;|%navelmts;|TIMER)* */
	pEVENT pEventTab;					/* The possible events for the CARD */
	pTEMPLATEDOTYPE pTemplateDO;		/* Pointer to the TEMPLATE DO not
										   overridden on CARD level. */
} CARDELEMENT, *pCARDELEMENT;	

/* do element */
typedef struct 
{
	WCHAR *pchType;						/* CDATA */
	pTEXTELEMENT pLabel;				/* %vdata */
	WCHAR *pchName;						/* NMTOKEN */
	BOOL fOptional;
	pELEMENTTYPE pContent;				/* (%task) */
} DOELEMENT, *pDOELEMENT;

/* fieldset element */
typedef struct 
{
	pTEXTELEMENT pTitle;				/* %vdata */
	pELEMENTTYPE pContent;				/* (%fields;|do)* */
} FIELDSETELEMENT, *pFIELDSETELEMENT;

/* go element */
typedef struct 
{
	pTEXTELEMENT pHref;					/* HREF */
	BOOL fSendRef;
	enum MethodType iMethod;
	WCHAR *pchAccChar;					/* CDATA */
	pELEMENTTYPE pContent;				/* (postfield|setvar)* */
} GOELEMENT, *pGOELEMENT;

/* head element */
typedef struct 
{
	pELEMENTTYPE pContent;				/* (access|meta)+ */
} HEADELEMENT, *pHEADELEMENT;

/* img element */
typedef struct 
{
	pTEXTELEMENT pAlt;					/* %vdata */
	pTEXTELEMENT pSrc;					/* HREF */
	pTEXTELEMENT pLocalSrc;				/* %vdata */
	UINT8 iVSpace;						/* %length */
	UINT8 iHSpace;						/* %length */
	ImageAlignmentType iIAlign;			/* %IAlign */
	INT16 iHeight;						/* %length */
	INT16 iWidth;						/* %length */
	PercentType iIsPercent;			/* Special; NONE_IS_PERCENT as default */
	UINT8 iMMIElementID;
} IMGELEMENT, *pIMGELEMENT;

/* input element */
typedef struct 
{		
	WCHAR *pchName;						/* NMTOKEN */
	INT8 iType;			
	pTEXTELEMENT pValue;				/* %vdata */
	WCHAR *pchFormat;					/* CDATA */
	BOOL fEmptyOK;	
	INT8 iSize;							/* %number */
	INT8 iMaxLength;					/* %number */
	UINT8 iTabIndex;					/* %number */
	pTEXTELEMENT pTitle;				/* %vdata */
} INPUTELEMENT, *pINPUTELEMENT;

/* meta element */
typedef struct
{
	WCHAR *pchHttpEquiv;				/* CDATA */
	WCHAR *pchName;						/* CDATA */
	BOOL fForua;						/* %boolean */
	WCHAR *pchContent;					/* CDATA */
	WCHAR *pchScheme;					/* CDATA */
} METAELEMENT, *pMETAELEMENT;

/* noop element - no body */

/* onevent element */
typedef struct 
{
	INT8 iType;
	pELEMENTTYPE pContent;				/* (%task) */
} ONEVENTELEMENT, *pONEVENTELEMENT;	

/* optgroup element */
typedef struct 
{
	pTEXTELEMENT pTitle;				/* %vdata */
	pELEMENTTYPE pContent;				/* (optgroup|option)+ */
} OPTGROUPELEMENT, *pOPTGROUPELEMENT;

/* option element */
typedef struct 
{
	pTEXTELEMENT pValue;				/* %vdata */
	pTEXTELEMENT pTitle;				/* %vdata */
	pELEMENTTYPE pOnpick;				/* URL or moved content */
	pELEMENTTYPE pContent;				/* (#PCDATA|onevent)* */
	BOOL fSelected;
	BOOL fLastOption;
} OPTIONELEMENT, *pOPTIONELEMENT;

/* p element */
typedef struct 
{
	TextAlignmentType iAlign;			/* %TAlign */
	enum WrapMode iMode;				/* %WrapMode */
	pELEMENTTYPE pContent;				/* (%fields|do)* */
} PELEMENT, *pPELEMENT;

/* postfield element */
typedef struct 
{
	pTEXTELEMENT pName;					/* %vdata */
	pTEXTELEMENT pValue;				/* %vdata */
} POSTFIELDELEMENT, *pPOSTFIELDELEMENT;

/* prev element */
typedef struct 
{
	pELEMENTTYPE pContent;				/* (setvar)* */
} PREVELEMENT, *pPREVELEMENT;

/* refresh element */
typedef struct 
{
	pELEMENTTYPE pContent;				/* (setvar)* */
} REFRESHELEMENT, *pREFRESHELEMENT;

/* select element */
typedef struct 
{
	pTEXTELEMENT pTitle;				/* %vdata */
	WCHAR *pchName;						/* NMTOKEN */
	pTEXTELEMENT pValue;				/* %vdata */
	WCHAR *pchIName;					/* NMTOKEN */
	pTEXTELEMENT pIValue;				/* %vdata */
	BOOL fMultiple;
	UINT8 iTabIndex;					/* %number */
	pELEMENTTYPE pContent;				/* (optgroup|option)+ */
} SELECTELEMENT, *pSELECTELEMENT;

/* setvar element */
typedef struct 
{
	pTEXTELEMENT pName;					/* %vdata */
	pTEXTELEMENT pValue;				/* %vdata */
} SETVARELEMENT, *pSETVARELEMENT;

/* table element */
typedef struct 
{
	pTEXTELEMENT pTitle;				/* %vdata */
	WCHAR *pchAlign;					/* L, C or R */
	UINT8 iColumns;						/* %number */
	pELEMENTTYPE pContent;				/* (tr)+ */

} TABLEELEMENT, *pTABLEELEMENT;

/* td element */
typedef struct 
{
	pELEMENTTYPE pContent;				/* (%text;|%layout|img|anchor|a)* */

} TDELEMENT, *pTDELEMENT;

/* template element */
typedef struct 
{
	pELEMENTTYPE pOnEntForw;			/* URL */
	pELEMENTTYPE pOnEntBack;			/* URL */
	pELEMENTTYPE pOnTimer;				/* URL */
	pELEMENTTYPE pContent;				/* (%navelmts;)*  */
	pEVENT pEventTab;		/* The possible events for the TEMPLATE */
} TEMPLATEELEMENT, *pTEMPLATEELEMENT;

/* timer element */
typedef struct 
{
	WCHAR *pchName;						/* NMTOKEN */
	pTEXTELEMENT pValue;				/* %vdata */
} TIMERELEMENT, *pTIMERELEMENT;

/* tr element */
typedef struct 
{
	pELEMENTTYPE pContent;				/* (td)+ */

} TRELEMENT, *pTRELEMENT;

/* wml element */
typedef struct 
{
	WCHAR *pchXmllang;					/* NMTOKEN */
	pELEMENTTYPE pContent;				/* (head?, template?, card+) */
} WMLELEMENT, *pWMLELEMENT;

/* User-agent specific data */
typedef struct 
{
	UINT8 iUserAgentMode;				/* User agent. (e.g. WTA or WML) */
	pELEMENTTYPE pDeck;					/* Pointer to the Deck */
	pWBXMLDECODESTR pDecStr;			/* Pointer to the Decode struct */
	BYTE *pbURLBase;					/* Contains complete URL-path to current deck. */
	pHISTORY pHistoryList;				/* History List */
	UINT16 iElmInHistory;
	UINT16 iMaxElmInHistory;
	BOOL fLockMMI;
	BOOL fUserLoadMode;
	pVARTABELM pEventVariables;			/* Variables to be set after a successful
										   navigation */
	BOOL fUserInputMode;
	pEVENT pTemplateEvents;				/* Pointer to the TEMPLATE Event Table */
	UINT8 iViewID;
	pELEMENTTYPE pCurrentCard;			/* Pointer to the current card */
	enum WrapMode iCurrentWrapMode;		/* Wrap-mode on the current card */
	pVARTABELM pVARTABLE;				/* Pointer to the Variable Table */
	INT16 iTextEncodFormat;				/* Text encoding format. */
	pHOSTSTR pVisitedHosts;				/* Visited hosts with authorization */
	BYTE *pWMLDeckData;
	
	/* Old context */
	#ifdef PARALLELL_PARSE_ENABLED
		pELEMENTTYPE pOldDeck;				/* Pointer to the old Deck */
		pEVENT pOldTemplateEvents;			/* Pointer to the old TEMPLATE Event Table */
		pELEMENTTYPE pOldCurrentCard;		/* Pointer to the old current card */
    #endif
    
    /* previus raw data */
    #ifdef TASKFAILURE_REPARSE_RAWDATA
		UINT8 iOldPIVersion;
		UINT8 iOldContentTypeByte;
		INT16 iOldCharset;
		UINT32 iOldRawWMLDeckDataLen;
		BYTE* pOldRawWMLDeckData;

		UINT8 iTempPIVersion;
		UINT8 iTempContentTypeByte;
		INT16 iTempCharset;
		UINT32 iTempRawWMLDeckDataLen;
		BYTE* pTempRawWMLDeckData;

    #endif

	UAPARAM* pUAParam;
} UA, *pUA;

#endif
