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
/* ==================================================================
	FILE: WMLToken.h
	
	WML token definitions.

	Rev history:
		980626	JPR		Created
		990310	JPR		Modified
		990322	JPR		WAP 1.1
		990426	JPR		Support for LITERALS added
		990428	JPR		Updated
		991028	JPR		WTA Extensions included
		
   ================================================================== */

#ifndef _TOKENS
#define _TOKENS

#include "CmmnRsrc.h"

/* Tag tokens				*/
#define Type_A			(unsigned char) '\x1C'
#define Type_ANCHOR		(unsigned char) '\x22'
#define Type_ACCESS		(unsigned char) '\x23'
#define Type_B			(unsigned char) '\x24'
#define Type_BIG		(unsigned char) '\x25'
#define Type_BR			(unsigned char) '\x26'
#define Type_CARD		(unsigned char) '\x27'
#define Type_DO			(unsigned char) '\x28'
#define Type_EM			(unsigned char) '\x29'
#define Type_FIELDSET	(unsigned char) '\x2A'
#define Type_GO			(unsigned char) '\x2B'
#define Type_HEAD		(unsigned char) '\x2C'
#define Type_I			(unsigned char) '\x2D'
#define Type_IMG		(unsigned char) '\x2E'
#define Type_INPUT		(unsigned char) '\x2F'
#define Type_META		(unsigned char) '\x30'
#define Type_NOOP		(unsigned char) '\x31'
#define Type_P			(unsigned char) '\x20'
#define Type_POSTFIELD	(unsigned char) '\x21'
#define Type_PREV		(unsigned char) '\x32'
#define Type_ONEVENT	(unsigned char) '\x33'
#define Type_OPTGROUP	(unsigned char) '\x34'
#define Type_OPTION		(unsigned char) '\x35'
#define Type_REFRESH	(unsigned char) '\x36'
#define Type_SELECT		(unsigned char) '\x37'
#define Type_SETVAR		(unsigned char) '\x3E'
#define Type_SMALL		(unsigned char) '\x38'
#define Type_STRONG		(unsigned char) '\x39'
#define Type_TABLE		(unsigned char) '\x1F'
#define Type_TD			(unsigned char) '\x1D'
#define Type_TEMPLATE	(unsigned char) '\x3B'
#define Type_TIMER		(unsigned char) '\x3C'
#define Type_TR			(unsigned char) '\x1E'
#define Type_U			(unsigned char) '\x3D'
#define Type_WML		(unsigned char) '\x3F'


/* Attribute start tokens			*/
#define ATTRST_accept_charset			(unsigned char) '\x5'
#define ATTRST_align					(unsigned char) '\x52'
#define ATTRST_align_bottom				(unsigned char) '\x6'
#define ATTRST_align_center				(unsigned char) '\x7'
#define ATTRST_align_left				(unsigned char) '\x8'
#define ATTRST_align_middle				(unsigned char) '\x9'
#define ATTRST_align_right				(unsigned char) '\xA'
#define ATTRST_align_top				(unsigned char) '\xB'
#define ATTRST_alt						(unsigned char) '\xC'
#define ATTRST_class					(unsigned char) '\x54'
#define	ATTRST_columns					(unsigned char) '\x53'
#define ATTRST_content					(unsigned char) '\xD'
#define ATTRST_content_wmlc_charset		(unsigned char) '\x5C'
#define ATTRST_domain					(unsigned char) '\xF'
#define ATTRST_emptyok_false			(unsigned char) '\x10'
#define ATTRST_emptyok_true				(unsigned char) '\x11'
#define ATTRST_format					(unsigned char) '\x12'
#define ATTRST_forua_false				(unsigned char) '\x56'
#define ATTRST_forua_true				(unsigned char) '\x57'
#define ATTRST_height					(unsigned char) '\x13'
#define ATTRST_href						(unsigned char) '\x4A'
#define ATTRST_href_http				(unsigned char) '\x4B'
#define ATTRST_href_https				(unsigned char) '\x4C'
#define ATTRST_hspace					(unsigned char) '\x14'
#define ATTRST_http_equiv				(unsigned char) '\x5A'
#define ATTRST_http_equiv_Content_Type	(unsigned char) '\x5B'
#define ATTRST_http_equiv_Expires		(unsigned char) '\x5D'
#define ATTRST_id						(unsigned char) '\x55'
#define ATTRST_ivalue					(unsigned char) '\x15'
#define ATTRST_iname					(unsigned char) '\x16'
#define ATTRST_label					(unsigned char) '\x18'
#define ATTRST_localsrc					(unsigned char) '\x19'
#define ATTRST_maxlength				(unsigned char) '\x1A'
#define ATTRST_method_get				(unsigned char) '\x1B'
#define ATTRST_method_post				(unsigned char) '\x1C'
#define ATTRST_mode_nowrap				(unsigned char) '\x1D'
#define ATTRST_mode_wrap				(unsigned char) '\x1E'
#define ATTRST_multiple_false			(unsigned char) '\x1F'
#define ATTRST_multiple_true			(unsigned char) '\x20'
#define ATTRST_name						(unsigned char) '\x21'
#define ATTRST_newcontext_false			(unsigned char) '\x22'
#define ATTRST_newcontext_true			(unsigned char) '\x23'
#define ATTRST_onenterbackward			(unsigned char) '\x25'
#define ATTRST_onenterforward			(unsigned char) '\x26'
#define ATTRST_onpick					(unsigned char) '\x24'
#define ATTRST_ontimer					(unsigned char) '\x27'
#define ATTRST_optional_false			(unsigned char) '\x28'
#define ATTRST_optional_true			(unsigned char) '\x29'
#define ATTRST_path						(unsigned char) '\x2A'
#define ATTRST_scheme					(unsigned char) '\x2E'
#define ATTRST_sendreferer_false		(unsigned char) '\x2F'
#define ATTRST_sendreferer_true			(unsigned char) '\x30'
#define ATTRST_size						(unsigned char) '\x31'
#define ATTRST_src						(unsigned char) '\x32'
#define ATTRST_src_http					(unsigned char) '\x58'
#define ATTRST_src_https				(unsigned char) '\x59'
#define ATTRST_ordered_true				(unsigned char) '\x33'
#define ATTRST_ordered_false			(unsigned char) '\x34'
#define ATTRST_tabindex					(unsigned char) '\x35'
#define ATTRST_title					(unsigned char) '\x36'
#define ATTRST_type						(unsigned char) '\x37'
#define ATTRST_type_accept				(unsigned char) '\x38'
#define ATTRST_type_delete				(unsigned char) '\x39'
#define ATTRST_type_help				(unsigned char) '\x3A'
#define ATTRST_type_password			(unsigned char) '\x3B'
#define ATTRST_type_onpick				(unsigned char) '\x3C'
#define ATTRST_type_onenterbackward		(unsigned char) '\x3D'
#define ATTRST_type_onenterforward		(unsigned char) '\x3E'
#define ATTRST_type_ontimer				(unsigned char) '\x3F'
#define ATTRST_type_options				(unsigned char) '\x45'
#define ATTRST_type_prev				(unsigned char) '\x46'
#define ATTRST_type_reset				(unsigned char) '\x47'
#define ATTRST_type_text				(unsigned char) '\x48'
#define ATTRST_type_vnd					(unsigned char) '\x49'
#define ATTRST_value					(unsigned char) '\x4D'
#define ATTRST_vspace					(unsigned char) '\x4E'
#define ATTRST_width					(unsigned char) '\x4F'
#define ATTRST_xml_lang					(unsigned char) '\x50'


/* Attribute value tokens.		*/
#define ATTRVAL_com					(unsigned char) '\x85'
#define ATTRVAL_edu					(unsigned char) '\x86'
#define ATTRVAL_net					(unsigned char) '\x87'	
#define ATTRVAL_org					(unsigned char) '\x88'	
#define ATTRVAL_accept				(unsigned char) '\x89'
#define ATTRVAL_bottom				(unsigned char) '\x8A'
#define ATTRVAL_clear				(unsigned char) '\x8B'
#define ATTRVAL_delete				(unsigned char) '\x8C'
#define ATTRVAL_help				(unsigned char) '\x8D'
#define ATTRVAL_http				(unsigned char) '\x8E'
#define ATTRVAL_http_www			(unsigned char) '\x8F'
#define ATTRVAL_https				(unsigned char) '\x90'
#define ATTRVAL_https_www			(unsigned char) '\x91'
#define ATTRVAL_middle				(unsigned char) '\x93'
#define ATTRVAL_nowrap				(unsigned char) '\x94'
#define ATTRVAL_onpick				(unsigned char) '\x95'
#define ATTRVAL_onenterbackward		(unsigned char) '\x96'
#define ATTRVAL_onenterforward		(unsigned char) '\x97'
#define ATTRVAL_ontimer				(unsigned char) '\x98'
#define ATTRVAL_options				(unsigned char) '\x99'
#define ATTRVAL_password			(unsigned char) '\x9A'
#define ATTRVAL_reset				(unsigned char) '\x9B'
#define ATTRVAL_text				(unsigned char) '\x9D'
#define ATTRVAL_top					(unsigned char) '\x9E'
#define ATTRVAL_unknown				(unsigned char) '\x9F'
#define ATTRVAL_wrap				(unsigned char) '\xA0'
#define ATTRVAL_www					(unsigned char) '\xA1'


/* Definitions to support LITERALS */
#define STAT_NBROFTAGS 35
#define STAT_NBROFATTR 43

extern  const WCHAR * const ppchTagTokens[STAT_NBROFTAGS];
extern  const WCHAR * const ppchAttrTokens[STAT_NBROFATTR];

extern  const BYTE pbTagBytes[STAT_NBROFTAGS];
extern  const BYTE pbAttrBytes[STAT_NBROFATTR];


/* Definitions to support WTA Events */
#ifdef CONFIG_WTA

/* WTA Events */

/* NOTE! If the order of the events is changed, the function
   WML_ModifyParse in the file WMLParse.c MUST be changed to
   reflect this. */

/* Network common */
#define WTAEvent_cc_ic		(unsigned char) '\x20'
#define WTAEvent_cc_cl		(unsigned char) '\x21'
#define WTAEvent_cc_co		(unsigned char) '\x22'
#define WTAEvent_cc_oc		(unsigned char) '\x23'
#define WTAEvent_cc_cc		(unsigned char) '\x24'
#define WTAEvent_cc_dtmf	(unsigned char) '\x25'
#define WTAEvent_nt_it		(unsigned char) '\x26'
#define WTAEvent_nt_st		(unsigned char) '\x27'
#define WTAEvent_ms_ns		(unsigned char) '\x28'

#define STAT_NBROFWTAEVENTS 9

extern  const WCHAR * const ppwchWTAEvents[STAT_NBROFWTAEVENTS];
extern  const BYTE pbWTAEventBytes[STAT_NBROFWTAEVENTS];



/* Network GSM */
#ifdef CONFIG_WTAI_GSM

#define WTAEvent_gsm_ru		(unsigned char) '\x30'
#define WTAEvent_gsm_ch		(unsigned char) '\x31'
#define WTAEvent_gsm_ca		(unsigned char) '\x32'

#define STAT_NBROFGSMEVENTS 3

extern  const WCHAR * const ppwchWTAEvents[STAT_NBROFGSMEVENTS];
extern  const BYTE pbWTAEventBytes[STAT_NBROFGSMEVENTS];

#endif


/* Network PDC */
#ifdef CONFIG_WTAI_PDC

	/* No events defined */
	
#endif


/* Network IS-136 */
#ifdef CONFIG_WTAI_IS_136

#define WTAEvent_is136_ia	(unsigned char) '\x50'
#define WTAEvent_is136_if	(unsigned char) '\x51'

#define STAT_NBROFIS136EVENTS 2

extern  const WCHAR * const ppwchWTAEvents[STAT_NBROFIS136EVENTS];
extern  const BYTE pbWTAEventBytes[STAT_NBROFIS136EVENTS];

#endif


#endif

#endif
