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

#define	HUNI_CHO_K			0		// ぁ
#define	HUNI_CHO_KK			1		// あ
#define	HUNI_CHO_N			2		// い
#define	HUNI_CHO_T			3		// ぇ
#define	HUNI_CHO_TT			4		// え
#define	HUNI_CHO_L			5		// ぉ
#define	HUNI_CHO_M			6		// け
#define	HUNI_CHO_P			7		// げ
#define	HUNI_CHO_PP			8		// こ
#define	HUNI_CHO_S			9		// さ
#define	HUNI_CHO_SS			10		// ざ
#define	HUNI_CHO_NG			11		// し
#define	HUNI_CHO_C			12		// じ
#define	HUNI_CHO_CC			13		// す
#define	HUNI_CHO_CH			14		// ず
#define	HUNI_CHO_KH			15		// せ
#define	HUNI_CHO_TH			16		// ぜ
#define	HUNI_CHO_PH			17		// そ
#define	HUNI_CHO_H			18		// ぞ

#define	HUNI_JUNG_A			0		// た
#define	HUNI_JUNG_AY		1		// だ
#define	HUNI_JUNG_YA		2		// ち
#define	HUNI_JUNG_YAY		3		// ぢ
#define	HUNI_JUNG_E			4		// っ
#define	HUNI_JUNG_EY		5		// つ
#define	HUNI_JUNG_YE		6		// づ
#define	HUNI_JUNG_YEY		7		// て
#define	HUNI_JUNG_O			8		// で
#define	HUNI_JUNG_WA		9		// と
#define	HUNI_JUNG_WAY		10		// ど
#define	HUNI_JUNG_OY		11		// な
#define	HUNI_JUNG_YO		12		// に
#define	HUNI_JUNG_WU		13		// ぬ
#define	HUNI_JUNG_WE		14		// ね
#define	HUNI_JUNG_WEY		15		// の
#define	HUNI_JUNG_WI		16		// は
#define	HUNI_JUNG_YU		17		// ば
#define	HUNI_JUNG_U			18		// ぱ
#define	HUNI_JUNG_UY		19		// ひ
#define	HUNI_JUNG_I			20		// び

#define	HUNI_JONG_FILL		0		// Filler
#define	HUNI_JONG_K			1		// ぁ
#define	HUNI_JONG_KK		2		// あ
#define	HUNI_JONG_KS		3		// ぃ
#define	HUNI_JONG_N			4		// い
#define	HUNI_JONG_NC		5		// ぅ
#define	HUNI_JONG_NH		6		// う
#define	HUNI_JONG_T			7		// し
#define	HUNI_JONG_L			8		// ぉ
#define	HUNI_JONG_LK		9		// お
#define	HUNI_JONG_LM		10		// か
#define	HUNI_JONG_LP		11		// が
#define	HUNI_JONG_LS		12		// き
#define	HUNI_JONG_LTH		13		// ぎ
#define	HUNI_JONG_LPH		14		// く
#define	HUNI_JONG_LH		15		// ぐ
#define	HUNI_JONG_M			16		// け
#define	HUNI_JONG_P			17		// げ
#define	HUNI_JONG_PS		18		// ご
#define	HUNI_JONG_S			19		// さ
#define	HUNI_JONG_SS		20		// ざ
#define	HUNI_JONG_NG		21		// し
#define	HUNI_JONG_C			22		// じ
#define	HUNI_JONG_CH		23		// ず
#define	HUNI_JONG_KH		24		// せ
#define	HUNI_JONG_TH		25		// ぜ
#define	HUNI_JONG_PH		26		// そ
#define	HUNI_JONG_H			27		// ぞ


//--------------------------------------------------------
//
// KSSM Code JOHAB Definition
//
//--------------------------------------------------------


#define	KSSM_CHO_FILL		1		// Filler
#define	KSSM_CHO_K			2		// ぁ
#define	KSSM_CHO_KK			3		// あ
#define	KSSM_CHO_N			4		// い
#define	KSSM_CHO_T			5		// ぇ
#define	KSSM_CHO_TT			6		// え
#define	KSSM_CHO_L			7		// ぉ
#define	KSSM_CHO_M			8		// け
#define	KSSM_CHO_P			9		// げ
#define	KSSM_CHO_PP			10		// こ
#define	KSSM_CHO_S			11		// さ
#define	KSSM_CHO_SS			12		// ざ
#define	KSSM_CHO_NG			13		// し
#define	KSSM_CHO_C			14		// じ
#define	KSSM_CHO_CC			15		// す
#define	KSSM_CHO_CH			16		// ず
#define	KSSM_CHO_KH			17		// せ
#define	KSSM_CHO_TH			18		// ぜ
#define	KSSM_CHO_PH			19		// そ
#define	KSSM_CHO_H			20		// ぞ

#define	KSSM_JUNG_FILL		2
#define	KSSM_JUNG_A			3		// た
#define	KSSM_JUNG_AY		4		// だ
#define	KSSM_JUNG_YA		5		// ち
#define	KSSM_JUNG_YAY		6		// ぢ
#define	KSSM_JUNG_E			7		// っ
#define	KSSM_JUNG_EY		10		// つ
#define	KSSM_JUNG_YE		11		// づ
#define	KSSM_JUNG_YEY		12		// て
#define	KSSM_JUNG_O			13		// で
#define	KSSM_JUNG_WA		14		// と
#define	KSSM_JUNG_WAY		15		// ど
#define	KSSM_JUNG_OY		18		// な
#define	KSSM_JUNG_YO		19		// に
#define	KSSM_JUNG_WU		20		// ぬ
#define	KSSM_JUNG_WE		21		// ね
#define	KSSM_JUNG_WEY		22		// の
#define	KSSM_JUNG_WI		23		// は
#define	KSSM_JUNG_YU		26		// ば
#define	KSSM_JUNG_U			27		// ぱ
#define	KSSM_JUNG_UY		28		// ひ
#define	KSSM_JUNG_I			29		// び

#define	KSSM_JONG_FILL		1		// Filler
#define	KSSM_JONG_K			2		// ぁ
#define	KSSM_JONG_KK		3		// あ
#define	KSSM_JONG_KS		4		// ぃ
#define	KSSM_JONG_N			5		// い
#define	KSSM_JONG_NC		6		// ぅ
#define	KSSM_JONG_NH		7		// う
#define	KSSM_JONG_T			8		// し
#define	KSSM_JONG_L			9		// ぉ
#define	KSSM_JONG_LK		10		// お
#define	KSSM_JONG_LM		11		// か
#define	KSSM_JONG_LP		12		// が
#define	KSSM_JONG_LS		13		// き
#define	KSSM_JONG_LTH		14		// ぎ
#define	KSSM_JONG_LPH		15		// く
#define	KSSM_JONG_LH		16		// ぐ
#define	KSSM_JONG_M			17		// け
#define	KSSM_JONG_P			19		// げ
#define	KSSM_JONG_PS		20		// ご
#define	KSSM_JONG_S			21		// さ
#define	KSSM_JONG_SS		22		// ざ
#define	KSSM_JONG_NG		23		// し
#define	KSSM_JONG_C			24		// じ
#define	KSSM_JONG_CH		25		// ず
#define	KSSM_JONG_KH		26		// せ
#define	KSSM_JONG_TH		27		// ぜ
#define	KSSM_JONG_PH		28		// そ
#define	KSSM_JONG_H			29		// ぞ

#endif
