/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irtmrapi.h                                   */
/*             Created:    7/28/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/* -------------------------------------------------------------------- **
		ＩｒＳＩＲ　タイマ関数

	外部に公開している関数

	int	IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) ) ;
	void	IrDAAPI IrSIR_TimerRem( int hndl ) ;
	ulong	readtimer( void ) ;

** -------------------------------------------------------------------- */

#include	"irdaconf.h"
#include	<irda.h>
#include	<irsir.h>

#include	"irsirddk.h"

/* ----- 割り込み処理用データテーブル ------------------------------- */
PFCB	SirTimerFunc [ TBL_CNT ] ;	/* 割り込み処理関数	*/
dword	SirTimerCount[ TBL_CNT ] ;	/* タイマ初期値		*/
volatile char	SirTimerSem[TBL_CNT] ;		/* セマフォ		*/

/* ----- 割り込み関数の登録 ------------------------------------------- */
int IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) )
{
	// tasik 01.04.11
	// ﾁ?ﾁ､ｵﾈ ｽﾃｰ｣(msec)ﾀﾌ ﾁ?ｳｪｰ? ｳｪｼｭ callbackｿ｡ ｾﾋｷﾁﾁﾘｴﾙ.
	// ﾁ? ｿｩｱ箴ｭ ｺﾎﾅﾍ ｽﾃﾀﾛﾇﾘｼｭ msecﾀﾌ ﾁ?ｳｪｸ? ｱﾗｶｧｼｭｾﾟ 
	// callback ﾇﾔｼ?ｵ鯊ﾌ ｵｿﾀﾛﾀｻ ﾇﾑｴﾙ.
int	no ;
	for (no = 0 ;no < TBL_CNT ;no++ ) {
		if(SirTimerSem[no])
			continue ;
		if( SirTimerFunc[no] ) {
			SirTimerSem[no]	= 0 ;
			continue ;
		}
		SirTimerCount[no] = SirTicTimer + msec ;
		SirTimerFunc[no] = func ;
		SirTimerSem[no]	= 0 ;
		return(no) ;
	}
	return( -1 ) ;
}

/* ----- 割り込み関数の削除 ------------------------------------------- */
void IrDAAPI IrSIR_TimerRem( int no )
{
	if(no < 0 || TBL_CNT <= no)
		return;
	SirTimerCount[no] 	= (dword)0;
	SirTimerFunc[no]	= (PFCB)0;
	SirTimerSem[no]		= 0 ;
}

/* ----- 経過時間の取得 ----------------------------------------------- */
ulong	IrDAAPI	IrSIR_ReadTimer( void )
{
	// tasik 01.04.11
	// ﾇ?ﾀ鄲ﾇ ｽﾃｰ｣ﾀｻ ｾﾋｷﾁﾁﾖｴﾂ ｰﾍﾀﾌｴﾙ. 
	// ｹｰｷﾐ ﾇ?ﾀ鄲ﾇ ﾁ､ﾈｮﾇﾑ ｽﾃｰ｢ﾀｻ ｾﾋｷﾁﾁﾖｴﾂ ｰﾍﾀﾌ ｾﾆｴﾏｶ?.
	// ｽﾃｽｺﾅﾛﾀﾌ ｰ｡ｵｿｵﾇｰ? ｳｪｼｭ ﾀﾏﾁ､ﾇﾏｰﾔ ﾄｫｿ?ﾆｮｰ｡ ｵﾈ ｰｪﾀﾌｴﾙ.
	return( SirTicTimer ) ;
}

