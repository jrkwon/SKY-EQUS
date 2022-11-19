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

	File: P_CoTok.h

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991105    MRP   First version
	991115	  JPR	Attribute value tokens added

	000131	  JPR	Changes to support generic WBXML-decoder

==========================================================================*/
#ifndef _PUSHCOTOKH
#define _PUSHCOTOKH

#include "cmmnrsrc.h"

/* Tag tokens				*/
#define Co_TAG_co					(unsigned char) '\x5'
#define Co_TAG_invalidateobject		(unsigned char) '\x6'
#define Co_TAG_invalidateservice	(unsigned char) '\x7'


/* Attribute start tokens			*/
#define Co_ATTRST_href			(unsigned char) '\x5'
#define Co_ATTRST_href_http		(unsigned char) '\x6'
#define Co_ATTRST_href_www		(unsigned char) '\x7'
#define Co_ATTRST_href_https	(unsigned char) '\x8'
#define Co_ATTRST_https_www		(unsigned char) '\x9'


/* Attribute value tokens.		*/
#define Co_ATTRVAL_com			(unsigned char) '\x85'
#define Co_ATTRVAL_edu			(unsigned char) '\x86'
#define Co_ATTRVAL_net			(unsigned char) '\x87'	
#define Co_ATTRVAL_org			(unsigned char) '\x88'	


/* Definitions to support LITERALS */
#define CO_STAT_NBROFTAGS 3
#define CO_STAT_NBROFATTR 1

extern  const WCHAR * const ppwchCoTagTokens[CO_STAT_NBROFTAGS];
extern  const WCHAR * const ppwchCoAttrTokens[CO_STAT_NBROFATTR];

extern  const UINT16 piCoTagBytes[CO_STAT_NBROFTAGS];
extern  const UINT16 piCoAttrBytes[CO_STAT_NBROFATTR];

#endif
