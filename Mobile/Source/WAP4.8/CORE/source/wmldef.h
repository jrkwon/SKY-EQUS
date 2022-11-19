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

  		991126	JPR		Redesign of Element Handling, Text Handling,
						and Error Handling.
		991202	JPR		"Unknown" element and element types added
		991223	JPR		fLastOption in OPTIONELEMENT removed
		000104	JPR		xml:lang and support for WAP 1.2 added
		000110	IPN		Moved REALMSTR and HOSTSTR to authoriz.c.
						Removed pVisitedHosts from UA.
		000110	JPR		"Generic Text" element type added
		000201	JPR		br element added
		000809	JPR		Addition of "cache-control" attribute in go
									Addition of new version (WML_VERSION_1_3)
		001011	JPR		Value of Type_UNKNOWN lowered to 0x7DDD
		010124	IPN		Added fUserLoadMode in UA-Struct.

   ================================================================== */

#ifndef _DEFINITIONS
#define _DEFINITIONS

#include "cmmnrsrc.h"
#include "aapiclnt.h"
#include "wbxmldef.h"
#include "waedef.h"

/*========================================================================

	WML Version definitions

	The appropriate value is stored automatically in the pWBXMLDECODESTR
	(attribute iVersion), based on the WSP-header and WBXML content, in
	the WML_CheckPublicID function in WMLParse.c

=========================================================================*/

/* WML version 1.0 NOT supported */

#define WML_VERSION_1_0		10
#define WML_VERSION_1_1		11
#define WML_VERSION_1_2		12
#define WML_VERSION_1_3		13

/*=======================================================================*/



/* WML Text. Type_Inline_Text, Type_Table_Text, Type_Entity, 
   or Type_Ext.
*/
#define isTypeWMLText(t)	((t==Type_Inline_Text)||(t==Type_Table_Text)||(t==Type_Entity)||(t==Type_Ext_0)||(t==Type_Ext_1)||(t==Type_Ext_2))


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

/* GO enctype */
#define GO_urlencoded	18
#define GO_formdata		36

enum MethodType {M_Post='\x60', M_Get='\x40'};
enum WrapMode {P_Wrap, P_NoWrap, P_Unknown};	
enum CacheControl {Cache_nocache, Cache_unknown};

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
    History List
   ------------------------------------------ */
typedef struct tHISTORYITEM 
{
	UINT8 inList;
	BYTE *pbURLEntry;
	WCHAR *pwchItemTitle;
	BYTE  *pbPostData;  
	UINT16 iPostDataLength;
	INT8 iMethod;       
	UINT8 iContentType;
	INT16 iContentCharSet;
	struct tHISTORYITEM *pNextEntry;
} HISTORYITEM, *pHISTORYITEM;

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
 

/*========================================================================

	WML Element definitions

=========================================================================*/

/* Used for b, big, em, i, small, strong, u - element */
typedef struct tEMPHELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%flow);* */
	WCHAR* pwchXmlLang;
} EMPHELEMENT, *pEMPHELEMENT;

/* a element */
typedef struct tAELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (#PCDATA;|br|img)* */
	pELEMENTTYPE pTitle;
	pELEMENTTYPE pHref;					/* HREF */
	WCHAR wchAccessKey;					/* WML 1.2 extension */
	WCHAR* pwchXmlLang;
} AELEMENT, *pAELEMENT;

/* anchor element */
typedef struct tANCHORELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (#PCDATA;|br|img|go|prev|refresh)* */
	pELEMENTTYPE pTitle;
	WCHAR wchAccessKey;					/* WML 1.2 extension */
	WCHAR* pwchXmlLang;
} ANCHORELEMENT, *pANCHORELEMENT;

/* access element */
typedef struct tACCESSELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	WCHAR *pchDomain;					/* CDATA */
	WCHAR *pchPath;						/* CDATA */
} ACCESSELEMENT, *pACCESSELEMENT;

/* br element */
typedef struct tBRELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	WCHAR* pwchXmlLang;					/* NOT used in Wap 1.2 or newer */
} BRELEMENT, *pBRELEMENT;

/* card element */
typedef struct tCARDELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* onevent*, timer?, (do | p | pre)*  */
	WCHAR *pchId;						/* ID */
	pELEMENTTYPE pTitle;				/* %vdata */
	BOOL fNewContext;
	BOOL fOrdered;
	pELEMENTTYPE pOnEntForw;			/* URL */
	pELEMENTTYPE pOnEntBack;			/* URL */
	pELEMENTTYPE pOnTimer;				/* URL */
	pEVENT pEventTab;					/* The possible events for the CARD */
	pTEMPLATEDOTYPE pTemplateDO;		/* Pointer to the TEMPLATE DO not
										   overridden on CARD level. */
	WCHAR* pwchXmlLang;
} CARDELEMENT, *pCARDELEMENT;	

/* do element */
typedef struct tDOELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%task) */
	WCHAR *pchType;						/* CDATA */
	pELEMENTTYPE pLabel;				/* %vdata */
	WCHAR *pchName;						/* NMTOKEN */
	BOOL fOptional;
	WCHAR* pwchXmlLang;
} DOELEMENT, *pDOELEMENT;

/* fieldset element */
typedef struct tFIELDSETELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%fields;|do)* */
	pELEMENTTYPE pTitle;				/* %vdata */
	WCHAR* pwchXmlLang;
} FIELDSETELEMENT, *pFIELDSETELEMENT;

/* go element */
typedef struct tGOELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (postfield|setvar)* */
	pELEMENTTYPE pHref;					/* HREF */
	enum CacheControl iCacheControl;			/* ("no-cache") */
	BOOL fSendRef;
	enum MethodType iMethod;
	WCHAR* pchAccChar;					/* CDATA */
	UINT8 iEnctype;						/* defined above */
} GOELEMENT, *pGOELEMENT;

/* head element */
typedef struct tHEADELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (access|meta)+ */
} HEADELEMENT, *pHEADELEMENT;

/* img element */
typedef struct tIMGELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pAlt;					/* %vdata */
	pELEMENTTYPE pSrc;					/* HREF */
	pELEMENTTYPE pLocalSrc;				/* %vdata */
	UINT8 iVSpace;						/* %length */
	UINT8 iHSpace;						/* %length */
	ImageAlignmentType iIAlign;			/* %IAlign */
	INT16 iHeight;						/* %length */
	INT16 iWidth;						/* %length */
	PercentType iIsPercent;			/* Special; NONE_IS_PERCENT as default */
	UINT8 iMMIElementID;
	WCHAR* pwchXmlLang;
} IMGELEMENT, *pIMGELEMENT;

/* input element */
typedef struct tINPUTELEMENT
{		
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	WCHAR *pchName;						/* NMTOKEN */
	INT8 iInputType;			
	pELEMENTTYPE pValue;				/* %vdata */
	WCHAR *pchFormat;					/* CDATA */
	BOOL fEmptyOK;	
	INT8 iSize;							/* %number */
	INT8 iMaxLength;					/* %number */
	UINT8 iTabIndex;					/* %number */
	pELEMENTTYPE pTitle;				/* %vdata */
	WCHAR wchAccessKey;					/* WML 1.2 extension */
	WCHAR* pwchXmlLang;
} INPUTELEMENT, *pINPUTELEMENT;

/* meta element */
typedef struct tMETAELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	WCHAR *pchHttpEquiv;				/* CDATA */
	WCHAR *pchName;						/* CDATA */
	BOOL fForua;						/* %boolean */
	WCHAR *pchContent;					/* CDATA */
	WCHAR *pchScheme;					/* CDATA */
} METAELEMENT, *pMETAELEMENT;

/* noop element - no body */

/* onevent element */
typedef struct tONEVENTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%task) */
	INT8 iEventType;
} ONEVENTELEMENT, *pONEVENTELEMENT;	

/* optgroup element */
typedef struct tOPTGROUPELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (optgroup|option)+ */
	pELEMENTTYPE pTitle;				/* %vdata */
	WCHAR* pwchXmlLang;
} OPTGROUPELEMENT, *pOPTGROUPELEMENT;

/* option element */
typedef struct tOPTIONELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (#PCDATA|onevent)* */
	pELEMENTTYPE pValue;				/* %vdata */
	pELEMENTTYPE pTitle;				/* %vdata */
	pELEMENTTYPE pOnpick;				/* URL or moved content */
	BOOL fSelected;
	WCHAR* pwchXmlLang;
} OPTIONELEMENT, *pOPTIONELEMENT;

/* p element */
typedef struct tPELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%fields|do)* */
	TextAlignmentType iAlign;			/* %TAlign */
	enum WrapMode iMode;				/* %WrapMode */
	WCHAR* pwchXmlLang;
} PELEMENT, *pPELEMENT;

/* postfield element */
typedef struct tPOSTFIELDELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pName;					/* %vdata */
	pELEMENTTYPE pValue;				/* %vdata */
} POSTFIELDELEMENT, *pPOSTFIELDELEMENT;

/* pre element */
typedef struct tPREELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* ( #PCDATA | a | br | i | b | b | em | strong |
										     input | select )*  */
	UINT8 iXmlSpace;					/* 'preserve' | 'default' */
} PREELEMENT, *pPREELEMENT;

/* prev element */
typedef struct tPREVELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (setvar)* */
} PREVELEMENT, *pPREVELEMENT;

/* refresh element */
typedef struct tREFRESHELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (setvar)* */
} REFRESHELEMENT, *pREFRESHELEMENT;

/* select element */
typedef struct tSELECTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (optgroup|option)+ */
	pELEMENTTYPE pTitle;				/* %vdata */
	WCHAR *pchName;						/* NMTOKEN */
	pELEMENTTYPE pValue;				/* %vdata */
	WCHAR *pchIName;					/* NMTOKEN */
	pELEMENTTYPE pIValue;				/* %vdata */
	BOOL fMultiple;
	UINT8 iTabIndex;					/* %number */
	WCHAR* pwchXmlLang;
} SELECTELEMENT, *pSELECTELEMENT;

/* setvar element */
typedef struct tSETVARELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pName;					/* %vdata */
	pELEMENTTYPE pValue;				/* %vdata */
} SETVARELEMENT, *pSETVARELEMENT;

/* table element */
typedef struct tTABLEELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (tr)+ */
	pELEMENTTYPE pTitle;				/* %vdata */
	WCHAR *pchAlign;					/* L, C or R */
	UINT8 iColumns;						/* %number */
	WCHAR* pwchXmlLang;
} TABLEELEMENT, *pTABLEELEMENT;

/* td element */
typedef struct tTDELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%text;|%layout|img|anchor|a)* */
	WCHAR* pwchXmlLang;
} TDELEMENT, *pTDELEMENT;

/* template element */
typedef struct tTEMPLATEELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (%navelmts;)*  */
	pELEMENTTYPE pOnEntForw;			/* URL */
	pELEMENTTYPE pOnEntBack;			/* URL */
	pELEMENTTYPE pOnTimer;				/* URL */
	pEVENT pEventTab;		/* The possible events for the TEMPLATE */
} TEMPLATEELEMENT, *pTEMPLATEELEMENT;

/* timer element */
typedef struct tTIMERELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	WCHAR *pchName;						/* NMTOKEN */
	pELEMENTTYPE pValue;				/* %vdata */
} TIMERELEMENT, *pTIMERELEMENT;

/* tr element */
typedef struct tTRELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (td)+ */
} TRELEMENT, *pTRELEMENT;

/* unknown element */
typedef struct tUNKNOWNELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* ( * ) */
} UNKNOWNELEMENT, *pUNKNOWNELEMENT;

/* wml element */
typedef struct tWMLELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (head?, template?, card+) */
	WCHAR* pwchXmlLang;
} WMLELEMENT, *pWMLELEMENT;


/* Element type definitions */
#define Type_A			(UINT16) 0x1C
#define Type_ANCHOR		(UINT16) 0x22
#define Type_ACCESS		(UINT16) 0x23
#define Type_B			(UINT16) 0x24
#define Type_BIG		(UINT16) 0x25
#define Type_BR			(UINT16) 0x26
#define Type_CARD		(UINT16) 0x27
#define Type_DO			(UINT16) 0x28
#define Type_EM			(UINT16) 0x29
#define Type_FIELDSET	(UINT16) 0x2A
#define Type_GO			(UINT16) 0x2B
#define Type_HEAD		(UINT16) 0x2C
#define Type_I			(UINT16) 0x2D
#define Type_IMG		(UINT16) 0x2E
#define Type_INPUT		(UINT16) 0x2F
#define Type_META		(UINT16) 0x30
#define Type_NOOP		(UINT16) 0x31
#define Type_P			(UINT16) 0x20
#define Type_POSTFIELD	(UINT16) 0x21
#define Type_PRE		(UINT16) 0x1B
#define Type_PREV		(UINT16) 0x32
#define Type_ONEVENT	(UINT16) 0x33
#define Type_OPTGROUP	(UINT16) 0x34
#define Type_OPTION		(UINT16) 0x35
#define Type_REFRESH	(UINT16) 0x36
#define Type_SELECT		(UINT16) 0x37
#define Type_SETVAR		(UINT16) 0x3E
#define Type_SMALL		(UINT16) 0x38
#define Type_STRONG		(UINT16) 0x39
#define Type_TABLE		(UINT16) 0x1F
#define Type_TD			(UINT16) 0x1D
#define Type_TEMPLATE	(UINT16) 0x3B
#define Type_TIMER		(UINT16) 0x3C
#define Type_TR			(UINT16) 0x1E
#define Type_U			(UINT16) 0x3D
#define Type_WML		(UINT16) 0x3F
#define Type_UNKNOWN	(UINT16) 0x7DDD
#define Type_GenericText (UINT16) 0x2000


/*========================================================================

	User-agent specific data

=========================================================================*/
typedef struct 
{
	UINT8 iUserAgentMode;				/* User agent. (e.g. WTA or WML) */
	pELEMENTTYPE pDeck;					/* Pointer to the Deck */
	pWBXMLDECODESTR pDecStr;			/* Pointer to the Decode struct */
	BYTE *pbURLBase;					/* Contains complete URL-path to current deck. */
	pHISTORYITEM pHistoryList;			/* History List */
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
	BYTE *pWMLDeckData;

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

} UA, *pUA;

#endif
