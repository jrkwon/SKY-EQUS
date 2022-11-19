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

	File: P_SlTok.c

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991022    MRP   First version
	000208	  JPR	Updated to support new WBXML-decoder

==========================================================================*/

#include "p_sltok.h"


/*========================================================================
	Definitions to support literal tags. The words MUST be sorted in 
	alphabetic order. 
==========================================================================*/


/* Tags */
const WCHAR SLTag1[] = {'s','l',0};

const WCHAR * const ppwchSlTagTokens[SL_STAT_NBROFTAGS]=
{
	SLTag1
};

const UINT16 piSlTagBytes[SL_STAT_NBROFTAGS]=
{
	'\x5'
};

/* Attributes */
const WCHAR SLAttr1[] = {'a','c','t','i','o','n',0};
const WCHAR SLAttr2[] = {'h','r','e','f',0};

const WCHAR * const ppwchSlAttrTokens[SL_STAT_NBROFATTR]=
{
	SLAttr1,
	SLAttr2

};

const UINT16 piSlAttrBytes[SL_STAT_NBROFATTR]=
{
	'\x05',
	'\x08'
};
