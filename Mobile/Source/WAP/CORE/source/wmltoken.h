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

		991202	JPR		Changes to support new generic WBXML-decoder
		000104	JPR		Tokens; WAP 1.2 added
		000225	JPR		Tokens for xml:space added
		000308	JPR		Corrections in event handling
		000808	JPR		Updated to support WTA-WML (WTA 1.2.1)
		000810	JPR		Token for cache-control added
		
   ================================================================== */

#ifndef _TOKENS
#define _TOKENS

#include "cmmnrsrc.h"

/* Tag tokens				*/
#define WMLTAG_A			(unsigned char) '\x1C'
#define WMLTAG_ANCHOR		(unsigned char) '\x22'
#define WMLTAG_ACCESS		(unsigned char) '\x23'
#define WMLTAG_B			(unsigned char) '\x24'
#define WMLTAG_BIG			(unsigned char) '\x25'
#define WMLTAG_BR			(unsigned char) '\x26'
#define WMLTAG_CARD			(unsigned char) '\x27'
#define WMLTAG_DO			(unsigned char) '\x28'
#define WMLTAG_EM			(unsigned char) '\x29'
#define WMLTAG_FIELDSET		(unsigned char) '\x2A'
#define WMLTAG_GO			(unsigned char) '\x2B'
#define WMLTAG_HEAD			(unsigned char) '\x2C'
#define WMLTAG_I			(unsigned char) '\x2D'
#define WMLTAG_IMG			(unsigned char) '\x2E'
#define WMLTAG_INPUT		(unsigned char) '\x2F'
#define WMLTAG_META			(unsigned char) '\x30'
#define WMLTAG_NOOP			(unsigned char) '\x31'
#define WMLTAG_P			(unsigned char) '\x20'
#define WMLTAG_POSTFIELD	(unsigned char) '\x21'
#define WMLTAG_PRE			(unsigned char) '\x1B'
#define WMLTAG_PREV			(unsigned char) '\x32'
#define WMLTAG_ONEVENT		(unsigned char) '\x33'
#define WMLTAG_OPTGROUP		(unsigned char) '\x34'
#define WMLTAG_OPTION		(unsigned char) '\x35'
#define WMLTAG_REFRESH		(unsigned char) '\x36'
#define WMLTAG_SELECT		(unsigned char) '\x37'
#define WMLTAG_SETVAR		(unsigned char) '\x3E'
#define WMLTAG_SMALL		(unsigned char) '\x38'
#define WMLTAG_STRONG		(unsigned char) '\x39'
#define WMLTAG_TABLE		(unsigned char) '\x1F'
#define WMLTAG_TD			(unsigned char) '\x1D'
#define WMLTAG_TEMPLATE		(unsigned char) '\x3B'
#define WMLTAG_TIMER		(unsigned char) '\x3C'
#define WMLTAG_TR			(unsigned char) '\x1E'
#define WMLTAG_U			(unsigned char) '\x3D'
#define WMLTAG_WML			(unsigned char) '\x3F'


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
#define ATTRST_accesskey				(unsigned char) '\x5E'
#define ATTRST_enctype					(unsigned char) '\x5F'
#define ATTRST_application				(unsigned char) '\x60'
#define ATTRST_multipart				(unsigned char) '\x61'
#define ATTRST_xml_space_preserve		(unsigned char) '\x62'
#define ATTRST_xml_space_default		(unsigned char) '\x63'
#define ATTRST_cache_control		(unsigned char) '\x64'


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
#define STAT_NBROFTAGS 36
#define STAT_NBROFATTR 47

extern  const WCHAR* const ppwchTagTokens[STAT_NBROFTAGS];
extern  const WCHAR* const ppwchAttrTokens[STAT_NBROFATTR];

extern  const UINT16 piTagBytes[STAT_NBROFTAGS];
extern  const UINT16 piAttrBytes[STAT_NBROFATTR];


/* Definitions to support WTA Events */
#ifdef CONFIG_WTA

/* Specific tokens for WTA-WML */

extern const WCHAR WTAWml[8];

/* Tag tokens (tag code page 1 (0x100-0x1ff)) */
#define WTATAG_wtawml									(UINT16) 0x13f

/* Attribute start tokens (attribute code page 1 (0x100-0x1ff)) */
#define WTAATTRST_href_wtai						(UINT16) 0x105
#define WTAATTRST_href_wtai_wp_mc			(UINT16) 0x106
#define WTAATTRST_href_wtai_wp_sd			(UINT16) 0x107
#define WTAATTRST_href_wtai_wp_ap			(UINT16) 0x108
#define WTAATTRST_href_wtai_ms_ec			(UINT16) 0x109
#define WTAATTRST_type_wtaev					(UINT16) 0x110
#define WTAATTRST_type_wtaev_cc				(UINT16) 0x111
#define WTAATTRST_type_wtaev_cc_ic		(UINT16) 0x112
#define WTAATTRST_type_wtaev_cc_cl		(UINT16) 0x113
#define WTAATTRST_type_wtaev_cc_co		(UINT16) 0x114
#define WTAATTRST_type_wtaev_cc_oc		(UINT16) 0x115
#define WTAATTRST_type_wtaev_cc_cc		(UINT16) 0x116
#define WTAATTRST_type_wtaev_cc_dtmf	(UINT16) 0x117
#define WTAATTRST_type_wtaev_nt				(UINT16) 0x120
#define WTAATTRST_type_wtaev_nt_it		(UINT16) 0x121
#define WTAATTRST_type_wtaev_nt_st		(UINT16) 0x122
#define WTAATTRST_type_wtaev_pb				(UINT16) 0x130
#define WTAATTRST_type_wtaev_lg				(UINT16) 0x138
#define WTAATTRST_type_wtaev_ms				(UINT16) 0x150
#define WTAATTRST_type_wtaev_ms_ns		(UINT16) 0x151
#define WTAATTRST_type_wtaev_gsm			(UINT16) 0x158
#define WTAATTRST_type_wtaev_gsm_ru		(UINT16) 0x159
#define WTAATTRST_type_wtaev_gsm_ch		(UINT16) 0x15a
#define WTAATTRST_type_wtaev_gsm_ca		(UINT16) 0x15b
#define WTAATTRST_type_wtaev_pdc			(UINT16) 0x160
#define WTAATTRST_type_wtaev_is136		(UINT16) 0x168
#define WTAATTRST_type_wtaev_is136_ia	(UINT16) 0x169
#define WTAATTRST_type_wtaev_is136_if	(UINT16) 0x16a
#define WTAATTRST_type_wtaev_cdma			(UINT16) 0x170


/* Attribute value tokens */
/* None defined */


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

extern  const WCHAR * const ppwchGSMWTAEvents[STAT_NBROFGSMEVENTS];
extern  const BYTE pbGSMWTAEventBytes[STAT_NBROFGSMEVENTS];

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

extern  const WCHAR * const ppwchIS136WTAEvents[STAT_NBROFIS136EVENTS];
extern  const BYTE pbIS136WTAEventBytes[STAT_NBROFIS136EVENTS];

#endif


#endif

#endif
