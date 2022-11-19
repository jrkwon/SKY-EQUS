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
		990426	JPR		Created
		990428	JPR		Updated
		990514	JPR		Updated
		990616	JPR		Updated
		991028	JPR		WTA Extensions included

		991202	JPR		Changes to support new generic WBXML-decoder
		000104	JPR		Tokens; WAP 1.2 added
		000225	JPR		Tokens for xml:space added
		000308	JPR		Corrections in event handling
		000808	JPR		Updated to support WTA-WML (WTA 1.2.1)
		000810	JPR		Token for cache-control added
		010125	NKE		Token for wtawml added. Treated as <wml> when decoding.
   ================================================================== */

#include "wmltoken.h"

const WCHAR WMLTag1[] = {'a',0};
const WCHAR WMLTag2[] = {'a','c','c','e','s','s',0};
const WCHAR WMLTag3[] = {'a','n','c','h','o','r',0};
const WCHAR WMLTag4[] = {'b',0};
const WCHAR WMLTag5[] = {'b','i','g',0};
const WCHAR WMLTag6[] = {'b','r',0};
const WCHAR WMLTag7[] = {'c','a','r','d',0};
const WCHAR WMLTag8[] = {'d','o',0};
const WCHAR WMLTag9[] = {'e','m',0};
const WCHAR WMLTag10[] = {'f','i','e','l','d','s','e','t',0};
const WCHAR WMLTag11[] = {'g','o',0};
const WCHAR WMLTag12[] = {'h','e','a','d',0};
const WCHAR WMLTag13[] = {'i',0};
const WCHAR WMLTag14[] = {'i','m','g',0};
const WCHAR WMLTag15[] = {'i','n','p','u','t',0};
const WCHAR WMLTag16[] = {'m','e','t','a',0};
const WCHAR WMLTag17[] = {'n','o','o','p',0};
const WCHAR WMLTag18[] = {'o','n','e','v','e','n','t',0};
const WCHAR WMLTag19[] = {'o','p','t','g','r','o','u','p',0};
const WCHAR WMLTag20[] = {'o','p','t','i','o','n',0};
const WCHAR WMLTag21[] = {'p',0};
const WCHAR WMLTag22[] = {'p','o','s','t','f','i','e','l','d',0};
const WCHAR WMLTag23[] = {'p','r','e','v',0};
const WCHAR WMLTag24[] = {'r','e','f','r','e','s','h',0};
const WCHAR WMLTag25[] = {'s','e','l','e','c','t',0};
const WCHAR WMLTag26[] = {'s','e','t','v','a','r',0};
const WCHAR WMLTag27[] = {'s','m','a','l','l',0};
const WCHAR WMLTag28[] = {'s','t','r','o','n','g',0};
const WCHAR WMLTag29[] = {'t','a','b','l','e',0};
const WCHAR WMLTag30[] = {'t','d',0};
const WCHAR WMLTag31[] = {'t','e','m','p','l','a','t','e',0};
const WCHAR WMLTag32[] = {'t','i','m','e','r',0};
const WCHAR WMLTag33[] = {'t','r',0};
const WCHAR WMLTag34[] = {'u',0};
const WCHAR WMLTag35[] = {'w','m','l',0};
const WCHAR WMLTag36[] = {'p','r','e',0};

const WCHAR* const ppwchTagTokens[STAT_NBROFTAGS]=
{
	WMLTag1,
	WMLTag2,
	WMLTag3,
	WMLTag4,
	WMLTag5,
	WMLTag6,
	WMLTag7,
	WMLTag8,
	WMLTag9,
	WMLTag10,
	WMLTag11,
	WMLTag12,
	WMLTag13,
	WMLTag14,
	WMLTag15,
	WMLTag16,
	WMLTag17,
	WMLTag18,
	WMLTag19,
	WMLTag20,
	WMLTag21,
	WMLTag22,
	WMLTag36,
	WMLTag23,
	WMLTag24,
	WMLTag25,
	WMLTag26,
	WMLTag27,
	WMLTag28,
	WMLTag29,
	WMLTag30,
	WMLTag31,
	WMLTag32,
	WMLTag33,
	WMLTag34,
	WMLTag35
#ifdef CONFIG_WTA
	, WTAWml
#endif
};

const UINT16 piTagBytes[STAT_NBROFTAGS]=
{
	'\x1C',
	'\x23',
	'\x22',
	'\x24',
	'\x25',
	'\x26',
	'\x27',
	'\x28',
	'\x29',
	'\x2A',
	'\x2B',
	'\x2C',
	'\x2D',
	'\x2E',
	'\x2F',
	'\x30',
	'\x31',
	'\x33',
	'\x34',
	'\x35',
	'\x20',
	'\x21',
	'\x1B',
	'\x32',
	'\x36',
	'\x37',
	'\x3E',
	'\x38',
	'\x39',
	'\x1F',
	'\x1D',
	'\x3B',
	'\x3C',
	'\x1E',
	'\x3D',
	'\x3F'
#ifdef CONFIG_WTA
	, WTATAG_wtawml /* 0x13F */
#endif
};

const WCHAR WMLAttr1[] = {'a','c','c','e','p','t','-','c','h','a','r','s','e','t',0};
const WCHAR WMLAttr2[] = {'a','l','i','g','n',0};
const WCHAR WMLAttr3[] = {'a','l','t',0};
const WCHAR WMLAttr4[] = {'c','l','a','s','s',0};
const WCHAR WMLAttr5[] = {'c','o','l','u','m','n','s',0};
const WCHAR WMLAttr6[] = {'c','o','n','t','e','n','t',0};
const WCHAR WMLAttr7[] = {'d','o','m','a','i','n',0};
const WCHAR WMLAttr8[] = {'e','m','p','t','y','o','k',0};
const WCHAR WMLAttr9[] = {'f','o','r','m','a','t',0};
const WCHAR WMLAttr10[] = {'f','o','r','u','a',0};
const WCHAR WMLAttr11[] = {'h','e','i','g','h','t',0};
const WCHAR WMLAttr12[] = {'h','r','e','f',0};
const WCHAR WMLAttr13[] = {'h','s','p','a','c','e',0};
const WCHAR WMLAttr14[] = {'h','t','t','p','-','e','q','u','i','v',0};
const WCHAR WMLAttr15[] = {'i','d',0};
const WCHAR WMLAttr16[] = {'i','n','a','m','e',0};
const WCHAR WMLAttr17[] = {'i','v','a','l','u','e',0};
const WCHAR WMLAttr18[] = {'l','a','b','e','l',0};
const WCHAR WMLAttr19[] = {'l','o','c','a','l','s','r','c',0};
const WCHAR WMLAttr20[] = {'m','a','x','l','e','n','g','t','h',0};
const WCHAR WMLAttr21[] = {'m','e','t','h','o','d',0};
const WCHAR WMLAttr22[] = {'m','o','d','e',0};
const WCHAR WMLAttr23[] = {'m','u','l','t','i','p','l','e',0};
const WCHAR WMLAttr24[] = {'n','a','m','e',0};
const WCHAR WMLAttr25[] = {'n','e','w','c','o','n','t','e','x','t',0};
const WCHAR WMLAttr26[] = {'o','n','e','n','t','e','r','b','a','c','k','w','a','r','d',0};
const WCHAR WMLAttr27[] = {'o','n','e','n','t','e','r','f','o','r','w','a','r','d',0};
const WCHAR WMLAttr28[] = {'o','n','p','i','c','k',0};
const WCHAR WMLAttr29[] = {'o','n','t','i','m','e','r',0};
const WCHAR WMLAttr30[] = {'o','p','t','i','o','n','a','l',0};
const WCHAR WMLAttr31[] = {'o','r','d','e','r','e','d',0};
const WCHAR WMLAttr32[] = {'p','a','t','h',0};
const WCHAR WMLAttr33[] = {'s','c','h','e','m','e',0};
const WCHAR WMLAttr34[] = {'s','e','n','d','r','e','f','e','r','e','r',0};
const WCHAR WMLAttr35[] = {'s','i','z','e',0};
const WCHAR WMLAttr36[] = {'s','r','c',0};
const WCHAR WMLAttr37[] = {'t','a','b','i','n','d','e','x',0};
const WCHAR WMLAttr38[] = {'t','i','t','l','e',0};
const WCHAR WMLAttr39[] = {'t','y','p','e',0};
const WCHAR WMLAttr40[] = {'v','a','l','u','e',0};
const WCHAR WMLAttr41[] = {'v','s','p','a','c','e',0};
const WCHAR WMLAttr42[] = {'w','i','d','t','h',0};
const WCHAR WMLAttr43[] = {'x','m','l',':','l','a','n','g',0};
const WCHAR WMLAttr44[] = {'a','c','c','e','s','s','k','e','y',0};
const WCHAR WMLAttr45[] = {'e','n','c','t','y','p','e',0};
const WCHAR WMLAttr46[] = {'x','m','l',':','s','p','a','c','e',0};
const WCHAR WMLAttr47[] = {'c','a','c','h','e','-','c','o','n','t','r','o','l',0};


const WCHAR* const ppwchAttrTokens[STAT_NBROFATTR]=
{
	WMLAttr1,
	WMLAttr44,
	WMLAttr2,
	WMLAttr3,
	WMLAttr47,
	WMLAttr4,
	WMLAttr5,
	WMLAttr6,
	WMLAttr7,
	WMLAttr8,
	WMLAttr45,
	WMLAttr9,
	WMLAttr10,
	WMLAttr11,
	WMLAttr12,
	WMLAttr13,
	WMLAttr14,
	WMLAttr15,
	WMLAttr16,
	WMLAttr17,
	WMLAttr18,
	WMLAttr19,
	WMLAttr20,
	WMLAttr21,
	WMLAttr22,
	WMLAttr23,
	WMLAttr24,
	WMLAttr25,
	WMLAttr26,
	WMLAttr27,
	WMLAttr28,
	WMLAttr29,
	WMLAttr30,
	WMLAttr31,
	WMLAttr32,
	WMLAttr33,
	WMLAttr34,
	WMLAttr35,
	WMLAttr36,
	WMLAttr37,
	WMLAttr38,
	WMLAttr39,
	WMLAttr40,
	WMLAttr41,
	WMLAttr42,
	WMLAttr43,
	WMLAttr46
};

const UINT16 piAttrBytes[STAT_NBROFATTR]=
{
	'\x05',
	'\x5E',
	'\x52',
	'\x0C',
	'\x64',
	'\x54',
	'\x53',
	'\x0D',
	'\x0F',
	'\x10',
	'\x5F',
	'\x12',
	'\x56',
	'\x13',
	'\x4A',
	'\x14',
	'\x5A',
	'\x55',
	'\x16',
	'\x15',
	'\x18',
	'\x19',
	'\x1A',
	'\x1B',
	'\x1D',
	'\x1F',
	'\x21',
	'\x22',
	'\x25',
	'\x26',
	'\x24',
	'\x27',
	'\x28',
	'\x33',
	'\x2A',
	'\x2E',
	'\x2F',
	'\x31',
	'\x32',
	'\x35',
	'\x36',
	'\x37',
	'\x4D',
	'\x4E',
	'\x4F',
	'\x50',
	'\x62'
};


/* WTA Specific */
#ifdef CONFIG_WTA

const WCHAR WTAEvent1[] = {'c','c','/','c','c',0};
const WCHAR WTAEvent2[] = {'c','c','/','c','l',0};
const WCHAR WTAEvent3[] = {'c','c','/','c','o',0};
const WCHAR WTAEvent4[] = {'c','c','/','d','t','m','f',0};
const WCHAR WTAEvent5[] = {'c','c','/','i','c',0};
const WCHAR WTAEvent6[] = {'c','c','/','o','c',0};
const WCHAR WTAEvent7[] = {'m','s','/','n','s',0};
const WCHAR WTAEvent8[] = {'n','t','/','i','t',0};
const WCHAR WTAEvent9[] = {'n','t','/','s','t',0};
const WCHAR WTAWml[]={'w','t','a','-','w','m','l',0};

const WCHAR * const ppwchWTAEvents[STAT_NBROFWTAEVENTS]=
{
	WTAEvent1,
	WTAEvent2,
	WTAEvent3,
	WTAEvent4,
	WTAEvent5,
	WTAEvent6,
	WTAEvent7,
	WTAEvent8,
	WTAEvent9
};

const BYTE pbWTAEventBytes[STAT_NBROFWTAEVENTS]=
{
	WTAEvent_cc_cc,
	WTAEvent_cc_cl,
	WTAEvent_cc_co,
	WTAEvent_cc_dtmf,
	WTAEvent_cc_ic,
	WTAEvent_cc_oc,
	WTAEvent_ms_ns,
	WTAEvent_nt_it,
	WTAEvent_nt_st	
};

/* GSM Specific */
#ifdef CONFIG_WTAI_GSM
const WCHAR GSMEvent1[] = {'g','s','m','/','c','a',0};
const WCHAR GSMEvent2[] = {'g','s','m','/','c','h',0};
const WCHAR GSMEvent3[] = {'g','s','m','/','r','u',0};

const WCHAR * const ppwchGSMWTAEvents[STAT_NBROFGSMEVENTS]=
{
	GSMEvent1,
	GSMEvent2,
	GSMEvent3
};

const BYTE pbGSMWTAEventBytes[STAT_NBROFGSMEVENTS]=
{
	WTAEvent_gsm_ca,
	WTAEvent_gsm_ch,
	WTAEvent_gsm_ru
};
#endif

/* IS-136 Specific */
#ifdef CONFIG_WTAI_IS_136
const WCHAR IS136Event1[] = {'i','s','1','3','6','/','i','a',0};
const WCHAR IS136Event2[] = {'i','s','1','3','6','/','i','f',0};

const WCHAR * const ppwchIS136WTAEvents[STAT_NBROFIS136EVENTS]=
{
	IS136Event1,
	IS136Event2
};

const BYTE pbIS136WTAEventBytes[STAT_NBROFIS136EVENTS]=
{
	WTAEvent_is136_ia,
	WTAEvent_is136_if
};
#endif



#endif
