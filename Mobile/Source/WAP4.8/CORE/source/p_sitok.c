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

	File: P_SiTok.c

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991022    MRP   First version

	000201	  JPR	Changes to support generic WBXML-decoder

==========================================================================*/

#include "p_sitok.h"


/*========================================================================
	Definitions to support literal tags. The words MUST be sorted in 
	alphabetic order. 
==========================================================================*/


/* Tags */
const WCHAR SITag1[] = {'i','n','d','i','c','a','t','i','o','n',0};
const WCHAR SITag2[] = {'i','n','f','o',0};
const WCHAR SITag3[] = {'i','t','e','m',0};
const WCHAR SITag4[] = {'s','i',0};

const WCHAR * const ppwchSiTagTokens[SI_STAT_NBROFTAGS]=
{
	SITag1,
	SITag2,
	SITag3,
	SITag4
};

const UINT16 piSiTagBytes[SI_STAT_NBROFTAGS]=
{
	'\x6',
	'\x7',
	'\x8',
	'\x5',
};

/* Attributes */
const WCHAR SIAttr1[] = {'a','c','t','i','o','n',0};
const WCHAR SIAttr2[] = {'c','l','a','s','s',0};
const WCHAR SIAttr3[] = {'c','r','e','a','t','e','d',0};
const WCHAR SIAttr4[] = {'h','r','e','f',0};
const WCHAR SIAttr5[] = {'s','i','-','e','x','p','i','r','e','s',0};
const WCHAR SIAttr6[] = {'s','i','-','i','d',0};

const WCHAR * const ppwchSiAttrTokens[SI_STAT_NBROFATTR]=
{
	SIAttr1,
	SIAttr2,
	SIAttr3,
	SIAttr4,
	SIAttr5,
	SIAttr6

};

const UINT16 piSiAttrBytes[SI_STAT_NBROFATTR]=
{
	'\x05',
	'\x12',
	'\x0a',
	'\x0b',
	'\x10',
	'\x11',
};
