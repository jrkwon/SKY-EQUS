//
// Copyright (C) 1999-  Parole Science, Inc. All rights reserved.
//*******************************************************************
// 
// File Name: HCodeDef.h
//
// Hangul JOHAB Code Definition
//
// - Written by Choi, Woonho. Parole Science, Inc. R&D Center
//
// @ Creation Date: 1999. 2. 12. (KST)
//
//
//
//
// Work List 
//   1. Unicode JOHAB Definition
//   2. KSSM JOHAB Definition
//
//*******************************************************************



#ifndef	__HCODEDEF_H_
#define	__HCODEDEF_H_


//-------------------------------------------------------------------
//
// Unicode Ver 2.1 JOHAB Code Definition
//
//-------------------------------------------------------------------

#define	HUNI_CHO_K			0		// ��
#define	HUNI_CHO_KK			1		// ��
#define	HUNI_CHO_N			2		// ��
#define	HUNI_CHO_T			3		// ��
#define	HUNI_CHO_TT			4		// ��
#define	HUNI_CHO_L			5		// ��
#define	HUNI_CHO_M			6		// ��
#define	HUNI_CHO_P			7		// ��
#define	HUNI_CHO_PP			8		// ��
#define	HUNI_CHO_S			9		// ��
#define	HUNI_CHO_SS			10		// ��
#define	HUNI_CHO_NG			11		// ��
#define	HUNI_CHO_C			12		// ��
#define	HUNI_CHO_CC			13		// ��
#define	HUNI_CHO_CH			14		// ��
#define	HUNI_CHO_KH			15		// ��
#define	HUNI_CHO_TH			16		// ��
#define	HUNI_CHO_PH			17		// ��
#define	HUNI_CHO_H			18		// ��

#define	HUNI_JUNG_A			0		// ��
#define	HUNI_JUNG_AY		1		// ��
#define	HUNI_JUNG_YA		2		// ��
#define	HUNI_JUNG_YAY		3		// ��
#define	HUNI_JUNG_E			4		// ��
#define	HUNI_JUNG_EY		5		// ��
#define	HUNI_JUNG_YE		6		// ��
#define	HUNI_JUNG_YEY		7		// ��
#define	HUNI_JUNG_O			8		// ��
#define	HUNI_JUNG_WA		9		// ��
#define	HUNI_JUNG_WAY		10		// ��
#define	HUNI_JUNG_OY		11		// ��
#define	HUNI_JUNG_YO		12		// ��
#define	HUNI_JUNG_WU		13		// ��
#define	HUNI_JUNG_WE		14		// ��
#define	HUNI_JUNG_WEY		15		// ��
#define	HUNI_JUNG_WI		16		// ��
#define	HUNI_JUNG_YU		17		// ��
#define	HUNI_JUNG_U			18		// ��
#define	HUNI_JUNG_UY		19		// ��
#define	HUNI_JUNG_I			20		// ��

#define	HUNI_JONG_FILL		0		// Filler
#define	HUNI_JONG_K			1		// ��
#define	HUNI_JONG_KK		2		// ��
#define	HUNI_JONG_KS		3		// ��
#define	HUNI_JONG_N			4		// ��
#define	HUNI_JONG_NC		5		// ��
#define	HUNI_JONG_NH		6		// ��
#define	HUNI_JONG_T			7		// ��
#define	HUNI_JONG_L			8		// ��
#define	HUNI_JONG_LK		9		// ��
#define	HUNI_JONG_LM		10		// ��
#define	HUNI_JONG_LP		11		// ��
#define	HUNI_JONG_LS		12		// ��
#define	HUNI_JONG_LTH		13		// ��
#define	HUNI_JONG_LPH		14		// ��
#define	HUNI_JONG_LH		15		// ��
#define	HUNI_JONG_M			16		// ��
#define	HUNI_JONG_P			17		// ��
#define	HUNI_JONG_PS		18		// ��
#define	HUNI_JONG_S			19		// ��
#define	HUNI_JONG_SS		20		// ��
#define	HUNI_JONG_NG		21		// ��
#define	HUNI_JONG_C			22		// ��
#define	HUNI_JONG_CH		23		// ��
#define	HUNI_JONG_KH		24		// ��
#define	HUNI_JONG_TH		25		// ��
#define	HUNI_JONG_PH		26		// ��
#define	HUNI_JONG_H			27		// ��


//--------------------------------------------------------
//
// KSSM Code JOHAB Definition
//
//--------------------------------------------------------


#define	KSSM_CHO_FILL		1		// Filler
#define	KSSM_CHO_K			2		// ��
#define	KSSM_CHO_KK			3		// ��
#define	KSSM_CHO_N			4		// ��
#define	KSSM_CHO_T			5		// ��
#define	KSSM_CHO_TT			6		// ��
#define	KSSM_CHO_L			7		// ��
#define	KSSM_CHO_M			8		// ��
#define	KSSM_CHO_P			9		// ��
#define	KSSM_CHO_PP			10		// ��
#define	KSSM_CHO_S			11		// ��
#define	KSSM_CHO_SS			12		// ��
#define	KSSM_CHO_NG			13		// ��
#define	KSSM_CHO_C			14		// ��
#define	KSSM_CHO_CC			15		// ��
#define	KSSM_CHO_CH			16		// ��
#define	KSSM_CHO_KH			17		// ��
#define	KSSM_CHO_TH			18		// ��
#define	KSSM_CHO_PH			19		// ��
#define	KSSM_CHO_H			20		// ��

#define	KSSM_JUNG_FILL		2
#define	KSSM_JUNG_A			3		// ��
#define	KSSM_JUNG_AY		4		// ��
#define	KSSM_JUNG_YA		5		// ��
#define	KSSM_JUNG_YAY		6		// ��
#define	KSSM_JUNG_E			7		// ��
#define	KSSM_JUNG_EY		10		// ��
#define	KSSM_JUNG_YE		11		// ��
#define	KSSM_JUNG_YEY		12		// ��
#define	KSSM_JUNG_O			13		// ��
#define	KSSM_JUNG_WA		14		// ��
#define	KSSM_JUNG_WAY		15		// ��
#define	KSSM_JUNG_OY		18		// ��
#define	KSSM_JUNG_YO		19		// ��
#define	KSSM_JUNG_WU		20		// ��
#define	KSSM_JUNG_WE		21		// ��
#define	KSSM_JUNG_WEY		22		// ��
#define	KSSM_JUNG_WI		23		// ��
#define	KSSM_JUNG_YU		26		// ��
#define	KSSM_JUNG_U			27		// ��
#define	KSSM_JUNG_UY		28		// ��
#define	KSSM_JUNG_I			29		// ��

#define	KSSM_JONG_FILL		1		// Filler
#define	KSSM_JONG_K			2		// ��
#define	KSSM_JONG_KK		3		// ��
#define	KSSM_JONG_KS		4		// ��
#define	KSSM_JONG_N			5		// ��
#define	KSSM_JONG_NC		6		// ��
#define	KSSM_JONG_NH		7		// ��
#define	KSSM_JONG_T			8		// ��
#define	KSSM_JONG_L			9		// ��
#define	KSSM_JONG_LK		10		// ��
#define	KSSM_JONG_LM		11		// ��
#define	KSSM_JONG_LP		12		// ��
#define	KSSM_JONG_LS		13		// ��
#define	KSSM_JONG_LTH		14		// ��
#define	KSSM_JONG_LPH		15		// ��
#define	KSSM_JONG_LH		16		// ��
#define	KSSM_JONG_M			17		// ��
#define	KSSM_JONG_P			19		// ��
#define	KSSM_JONG_PS		20		// ��
#define	KSSM_JONG_S			21		// ��
#define	KSSM_JONG_SS		22		// ��
#define	KSSM_JONG_NG		23		// ��
#define	KSSM_JONG_C			24		// ��
#define	KSSM_JONG_CH		25		// ��
#define	KSSM_JONG_KH		26		// ��
#define	KSSM_JONG_TH		27		// ��
#define	KSSM_JONG_PH		28		// ��
#define	KSSM_JONG_H			29		// ��

#endif
