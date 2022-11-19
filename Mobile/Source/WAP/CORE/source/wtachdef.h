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

	File: WTAChDef.h

	Description:

	Author: Jens Pommer, AU-System AB

	Revision history:

  	Date    Sign	Comment
  	990813  JPR		First version
		000204	JPR		Updated to support new WBXML-decoder
		000211	JPR		Etag changed from WCHAR* to BYTE*
		000630	JPR		Updated for WAP 1.2.1
		001011	ASPN	Lower-case filenames

=========================================================================*/

#ifndef _WTACHDEF_H
#define _WTACHDEF_H


#include "cmmnrsrc.h"
#include "aapiclnt.h"
#include "wbxmldef.h"
#include "wtachtok.h"

/*========================================================================

	Channel Definitions

==========================================================================*/

/* Element type definitions */
#define Ch_Type_channel			(UINT16) 0x05
#define Ch_Type_title				(UINT16) 0x06
#define Ch_Type_abstract		(UINT16) 0x07
#define Ch_Type_resource		(UINT16) 0x08

/* Channel versions */
#define CH_VERSION_1_1				1
#define CH_VERSION_1_2				2


/*========================================================================

	Channel Element definitions

==========================================================================*/

/* channel element */
typedef struct tCH_CHANNELELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	pELEMENTTYPE pContent;				/* title, abstract?, resource+ */	

	INT32 iMaxSpace;					/* %Number */
	BYTE *pbBase;							/* %URI */
	WCHAR *pwchEventID;				/* CDATA */
	WCHAR *pwchChannelID;			/* CDATA */
	BYTE *pbSuccess;					/* %URI */
	BYTE *pbFailure;					/* %URI */
	BOOL fUserAccessible;			/* true|false */
} CH_CHANNELELEMENT, *pCH_CHANNELELEMENT;


/* title element */
typedef struct tCH_TITLEELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	pELEMENTTYPE pContent;				/* #PCDATA */
} CH_TITLEELEMENT, *pCH_TITLEELEMENT;


/* abstract element */
typedef struct tCH_ABSTRACTELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;
	pELEMENTTYPE pContent;				/* #PCDATA */
} CH_ABSTRACTELEMENT, *pCH_ABSTRACTELEMENT;


/* resource element */
typedef struct tCH_RESOURCEELEMENT
{
	UINT16 iType;
	struct tELEMENTTYPE* pNextElement;

	BYTE *pbHref;						/* %URI */
	UINT32 iLastMod;				/* %Number */
	BYTE *pbEtag;						/* NMTOKEN */
	WCHAR *pwchMd5;					/* NMTOKEN */
} CH_RESOURCEELEMENT, *pCH_RESOURCEELEMENT;

/*========================================================================*/

#endif

