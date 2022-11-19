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

	File: P_CoTok.c

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991105    MRP   First version
	000208	  JPR	Updated to support new WBXML-decoder

==========================================================================*/

#include "p_cotok.h"


/*========================================================================
	Definitions to support literal tags. The words MUST be sorted in 
	alphabetic order. 
==========================================================================*/


/* Tags */
const WCHAR COTag1[] = {'c','o',0};
const WCHAR COTag2[] = {'i','n','v','a','l','i','d','a','t','e','-','o','b','j','e','c','t',0};
const WCHAR COTag3[] = {'i','n','v','a','l','i','d','a','t','e','-','s','e','r','v','i','c','e',0};

const WCHAR * const ppwchCoTagTokens[CO_STAT_NBROFTAGS]=
{
	COTag1,
	COTag2,
	COTag3
};

const UINT16 piCoTagBytes[CO_STAT_NBROFTAGS]=
{
	'\x5',
	'\x6',
	'\x7'
};

/* Attributes */
const WCHAR COAttr1[] = {'u','r','i',0};

const WCHAR * const ppwchCoAttrTokens[CO_STAT_NBROFATTR]=
{
	COAttr1
};

const UINT16 piCoAttrBytes[CO_STAT_NBROFATTR]=
{
	'\x05'
};
