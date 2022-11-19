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

	File: PUSHDef.h

	Description: Definitions needed for the WBXML-decoding of the PUSH
				 Content types.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991115    JPR   First version

	000131	  JPR	Changes to support generic WBXML-decoder

==========================================================================*/
#ifndef _PUSHDEF_H
#define _PUSHDEF_H


#include "cmmnrsrc.h"
#include "aapiclnt.h"
#include "wbxmldef.h"
#include "p_sitok.h"
#include "p_sltok.h"
#include "p_cotok.h"

/*========================================================================

	Push Definitions

==========================================================================*/

/* Element type definitions */
#define Si_Type_si					(UINT16) 0x05
#define Si_Type_indication			(UINT16) 0x06
#define Si_Type_info				(UINT16) 0x07
#define Si_Type_item				(UINT16) 0x08

#define Sl_Type_sl					(UINT16) 0x05

#define Co_Type_co					(UINT16) 0x05
#define Co_Type_invalidateobject	(UINT16) 0x06
#define Co_Type_invalidateservice	(UINT16) 0x07

#define SI_VERSION_1_0				1
#define SL_VERSION_1_0				1
#define CO_VERSION_1_0				1

/*========================================================================

	Service Indication Element definitions

==========================================================================*/

/* Action */
#define SI_SIGNAL_NONE			0
#define SI_SIGNAL_LOW			1
#define SI_SIGNAL_MEDIUM		2
#define SI_SIGNAL_HIGH			3
#define SI_DELETE				4


/* si element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (indication, info?) */
} SI_SIELEMENT, *pSI_SIELEMENT;


/* indication element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* #PCDATA */
	BYTE* pbHref;						/* URI */
	WCHAR* pwchSiId;					/* CDATA */
	UINT32 iCreated;					/* %Datetime */
	UINT32 iExpires;					/* %Datetime */
	UINT8 iAction;						/* none | low | medium | high | delete */
} SI_INDICATIONELEMENT, *pSI_INDICATIONELEMENT;


/* info element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* (item+) */
} SI_INFOELEMENT, *pSI_INFOELEMENT;


/* item element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	pELEMENTTYPE pContent;				/* #PCDATA */
	WCHAR* pwchClass;
} SI_ITEMELEMENT, *pSI_ITEMELEMENT;




/*========================================================================

	Service Loading Element definitions

==========================================================================*/

/* Action */
#define SL_CACHE				4 
#define SL_EXECUTE_LOW			1
#define SL_EXECUTE_HIGH			3


/* sl element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	
	BYTE* pbHref;				/* URI */
	UINT8 iAction;				/* low | high | cache */
} SL_SLELEMENT, *pSL_SLELEMENT;




/*========================================================================

	Cache Operation Element definitions

==========================================================================*/

/* co element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	
	pELEMENTTYPE pContent;		/* (invalidate-object | invalidate-service)+ */
} CO_COELEMENT, *pCO_COELEMENT;


/* invalidate-object element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	
	BYTE* pbURI;				/* URI */
} CO_INVOBJELEMENT, *pCO_INVOBJELEMENT;


/* invalidate-service element */
typedef struct
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	
	BYTE* pbURI;				/* URI */
} CO_INVSERVELEMENT, *pCO_INVSERVELEMENT;

/*========================================================================*/

#endif

