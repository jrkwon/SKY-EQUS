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

	File: P_SlTok.h

	Description:

	Author: Martin Ranstorp, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991022    MRP   First version
	991110	  JPR	Attribute value tokens added

	000131	  JPR	Changes to support generic WBXML-decoder
	000208	  JPR	Corrections

==========================================================================*/
#ifndef _PUSHSLTOKH
#define _PUSHSLTOKH

#include "cmmnrsrc.h"

/* Tag tokens				*/
#define Sl_TAG_sl				(unsigned char) '\x5'

/* Attribute start tokens			*/
#define Sl_ATTRST_executelow	(unsigned char) '\x5'
#define Sl_ATTRST_executehigh	(unsigned char) '\x6'
#define Sl_ATTRST_cache			(unsigned char) '\x7'
#define Sl_ATTRST_href			(unsigned char) '\x8'
#define Sl_ATTRST_href_http		(unsigned char) '\x9'
#define Sl_ATTRST_href_www		(unsigned char) '\xa'
#define Sl_ATTRST_href_https	(unsigned char) '\xb'
#define Sl_ATTRST_https_www	    (unsigned char) '\xc'


/* Attribute value tokens.		*/
#define Sl_ATTRVAL_com			(unsigned char) '\x85'
#define Sl_ATTRVAL_edu			(unsigned char) '\x86'
#define Sl_ATTRVAL_net			(unsigned char) '\x87'	
#define Sl_ATTRVAL_org			(unsigned char) '\x88'	


/* Definitions to support LITERALS */
#define SL_STAT_NBROFTAGS 1
#define SL_STAT_NBROFATTR 2

extern  const WCHAR * const ppwchSlTagTokens[SL_STAT_NBROFTAGS];
extern  const WCHAR * const ppwchSlAttrTokens[SL_STAT_NBROFATTR];

extern  const UINT16 piSlTagBytes[SL_STAT_NBROFTAGS];
extern  const UINT16 piSlAttrBytes[SL_STAT_NBROFATTR];

#endif
