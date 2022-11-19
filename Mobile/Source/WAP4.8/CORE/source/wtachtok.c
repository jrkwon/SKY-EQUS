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

	File: WTAChTok.c

	Description:

	Author: Jens Pommer, AU-System AB

	Revision history:

  	Date    Sign	Comment
  	990810  JPR		First version
		000204	JPR		Updated to support new WBXML-decoder
		000207	JPR		Correction eventid -> EventId
		000630	JPR		Updated for WAP 1.2.1
		001011	ASPN	Lower-case filenames

==========================================================================*/

#include "wtachtok.h"


/*========================================================================
	Definitions to support literal tags. The words MUST be sorted in 
	alphabetic order. 
==========================================================================*/


/* Tags */
const WCHAR CHTag1[] = {'a','b','s','t','r','a','c','t',0};
const WCHAR CHTag2[] = {'c','h','a','n','n','e','l',0};
const WCHAR CHTag3[] = {'r','e','s','o','u','r','c','e',0};
const WCHAR CHTag4[] = {'t','i','t','l','e',0};

/* Sorted in alphabetic order */
const WCHAR * const ppwchChTagTokens[CH_STAT_NBROFTAGS]=
{
	CHTag1,
	CHTag2,
	CHTag3,
	CHTag4
};

const UINT16 piChTagBytes[CH_STAT_NBROFTAGS]=
{
	'\x7',
	'\x5',
	'\x8',
	'\x6'
};

/* Attributes */
const WCHAR CHAttr1[] = {'e','v','e','n','t','i','d',0};
const WCHAR CHAttr2[] = {'b','a','s','e',0};
const WCHAR CHAttr3[] = {'e','t','a','g',0};
const WCHAR CHAttr4[] = {'f','a','i','l','u','r','e',0};
const WCHAR CHAttr5[] = {'h','r','e','f',0};
const WCHAR CHAttr6[] = {'l','a','s','t','m','o','d',0};
const WCHAR CHAttr7[] = {'m','a','x','s','p','a','c','e',0};
const WCHAR CHAttr8[] = {'m','d','5',0};
const WCHAR CHAttr9[] = {'s','u','c','c','e','s','s',0};
const WCHAR CHAttr10[] = {'c','h','a','n','n','e','l','i','d',0};
const WCHAR CHAttr11[] = {'u','s','e','r','a','c','c','e','s','s','i','b','l','e',0};

/* Sorted in alphabetic order */
const WCHAR * const ppwchChAttrTokens[CH_STAT_NBROFATTR]=
{
	CHAttr2,
	CHAttr10,
	CHAttr3,
	CHAttr1,
	CHAttr4,
	CHAttr5,
	CHAttr6,
	CHAttr7,
	CHAttr8,
	CHAttr9,
	CHAttr11
};

const UINT16 piChAttrBytes[CH_STAT_NBROFATTR]=
{
	'\x06',
	'\x15',
	'\x0b',
	'\x13',
	'\x10',
	'\x07',
	'\x0a',
	'\x05',
	'\x0c',
	'\x0d',
	'\x16'
};
