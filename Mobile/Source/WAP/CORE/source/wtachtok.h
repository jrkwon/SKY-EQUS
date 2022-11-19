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

	File: WTAChTok.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:

  	Date    Sign	Comment
  	990811  JPR		First version
		000204	JPR		Updated to support new WBXML-decoder
		000630	JPR		Updated for WAP 1.2.1
		001011	ASPN	Lower-case filenames

==========================================================================*/
#ifndef _WTACHTOKH
#define _WTACHTOKH

#include "cmmnrsrc.h"

/* Tag tokens				*/
#define Ch_TAG_channel			(unsigned char) '\x5'
#define Ch_TAG_title				(unsigned char) '\x6'
#define Ch_TAG_abstract			(unsigned char) '\x7'
#define Ch_TAG_resource			(unsigned char) '\x8'


/* Attribute start tokens			*/
#define Ch_ATTRST_maxspace				(unsigned char) '\x5'
#define Ch_ATTRST_base						(unsigned char) '\x6'
#define Ch_ATTRST_href						(unsigned char) '\x7'
#define Ch_ATTRST_href_http				(unsigned char) '\x8'
#define Ch_ATTRST_href_https			(unsigned char) '\x9'
#define Ch_ATTRST_lastmod					(unsigned char) '\xa'
#define Ch_ATTRST_etag						(unsigned char) '\xb'
#define Ch_ATTRST_md5							(unsigned char) '\xc'
#define Ch_ATTRST_success					(unsigned char) '\xd'
#define Ch_ATTRST_success_http		(unsigned char) '\xe'
#define Ch_ATTRST_success_https		(unsigned char) '\xf'
#define Ch_ATTRST_failure					(unsigned char) '\x10'
#define Ch_ATTRST_failure_http		(unsigned char) '\x11'
#define Ch_ATTRST_failure_https		(unsigned char) '\x12'
#define Ch_ATTRST_eventid					(unsigned char) '\x13'
#define Ch_ATTRST_eventid_wtaev		(unsigned char) '\x14'
#define Ch_ATTRST_channelid				(unsigned char) '\x15'
#define Ch_ATTRST_useraccessible	(unsigned char) '\x16'


/* Definitions to support LITERALS */
#define CH_STAT_NBROFTAGS 4
#define CH_STAT_NBROFATTR 11

extern  const WCHAR * const ppwchChTagTokens[CH_STAT_NBROFTAGS];
extern  const WCHAR * const ppwchChAttrTokens[CH_STAT_NBROFATTR];

extern  const UINT16 piChTagBytes[CH_STAT_NBROFTAGS];
extern  const UINT16 piChAttrBytes[CH_STAT_NBROFATTR];


#endif
