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

	File: P_SiTok.h

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991022    MRP   First version
	991110	  JPR	Attribute value tokens added

	000201	  JPR	Changes to support generic WBXML-decoder

==========================================================================*/
#ifndef _PUSHSITOKH
#define _PUSHSITOKH

#include "cmmnrsrc.h"

/* Tag tokens				*/
#define Si_TAG_si				(unsigned char) '\x5'
#define Si_TAG_indication		(unsigned char) '\x6'
#define Si_TAG_info				(unsigned char) '\x7'
#define Si_TAG_item				(unsigned char) '\x8'


/* Attribute start tokens			*/
#define Si_ATTRST_signalnone	(unsigned char) '\x5'
#define Si_ATTRST_signallow		(unsigned char) '\x6'
#define Si_ATTRST_signalmedium	(unsigned char) '\x7'
#define Si_ATTRST_signalhigh	(unsigned char) '\x8'
#define Si_ATTRST_delete		(unsigned char) '\x9'
#define Si_ATTRST_created		(unsigned char) '\xa'
#define Si_ATTRST_href			(unsigned char) '\xb'
#define Si_ATTRST_href_http		(unsigned char) '\xc'
#define Si_ATTRST_href_www		(unsigned char) '\xd'
#define Si_ATTRST_href_https	(unsigned char) '\xe'
#define Si_ATTRST_https_www	    (unsigned char) '\xf'
#define Si_ATTRST_expires		(unsigned char) '\x10'
#define Si_ATTRST_siid			(unsigned char) '\x11'
#define Si_ATTRST_class			(unsigned char) '\x12'


/* Attribute value tokens.		*/
#define Si_ATTRVAL_com			(unsigned char) '\x85'
#define Si_ATTRVAL_edu			(unsigned char) '\x86'
#define Si_ATTRVAL_net			(unsigned char) '\x87'	
#define Si_ATTRVAL_org			(unsigned char) '\x88'	


/* Definitions to support LITERALS */
#define SI_STAT_NBROFTAGS 4
#define SI_STAT_NBROFATTR 6

extern  const WCHAR * const ppwchSiTagTokens[SI_STAT_NBROFTAGS];
extern  const WCHAR * const ppwchSiAttrTokens[SI_STAT_NBROFATTR];

extern  const UINT16 piSiTagBytes[SI_STAT_NBROFTAGS];
extern  const UINT16 piSiAttrBytes[SI_STAT_NBROFATTR];

#endif
