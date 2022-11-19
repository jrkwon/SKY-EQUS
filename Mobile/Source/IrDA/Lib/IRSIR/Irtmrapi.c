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
		‚h‚’‚r‚h‚q@ƒ^ƒCƒ}ŠÖ”

	ŠO•”‚ÉŒöŠJ‚µ‚Ä‚¢‚éŠÖ”

	int	IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) ) ;
	void	IrDAAPI IrSIR_TimerRem( int hndl ) ;
	ulong	readtimer( void ) ;

** -------------------------------------------------------------------- */

#include	"irdaconf.h"
#include	<irda.h>
#include	<irsir.h>

#include	"irsirddk.h"

/* ----- Š„‚è‚İˆ——pƒf[ƒ^ƒe[ƒuƒ‹ ------------------------------- */
PFCB	SirTimerFunc [ TBL_CNT ] ;	/* Š„‚è‚İˆ—ŠÖ”	*/
dword	SirTimerCount[ TBL_CNT ] ;	/* ƒ^ƒCƒ}‰Šú’l		*/
volatile char	SirTimerSem[TBL_CNT] ;		/* ƒZƒ}ƒtƒH		*/

/* ----- Š„‚è‚İŠÖ”‚Ì“o˜^ ------------------------------------------- */
int IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) )
{
	// tasik 01.04.11
	// ÁöÁ¤µÈ ½Ã°£(msec)ÀÌ Áö³ª°í ³ª¼­ callback¿¡ ¾Ë·ÁÁØ´Ù.
	// Áï ¿©±â¼­ ºÎÅÍ ½ÃÀÛÇØ¼­ msecÀÌ Áö³ª¸é ±×¶§¼­¾ß 
	// callback ÇÔ¼öµéÀÌ µ¿ÀÛÀ» ÇÑ´Ù.
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

/* ----- Š„‚è‚İŠÖ”‚Ìíœ ------------------------------------------- */
void IrDAAPI IrSIR_TimerRem( int no )
{
	if(no < 0 || TBL_CNT <= no)
		return;
	SirTimerCount[no] 	= (dword)0;
	SirTimerFunc[no]	= (PFCB)0;
	SirTimerSem[no]		= 0 ;
}

/* ----- Œo‰ßŠÔ‚Ìæ“¾ ----------------------------------------------- */
ulong	IrDAAPI	IrSIR_ReadTimer( void )
{
	// tasik 01.04.11
	// ÇöÀçÀÇ ½Ã°£À» ¾Ë·ÁÁÖ´Â °ÍÀÌ´Ù. 
	// ¹°·Ğ ÇöÀçÀÇ Á¤È®ÇÑ ½Ã°¢À» ¾Ë·ÁÁÖ´Â °ÍÀÌ ¾Æ´Ï¶ó.
	// ½Ã½ºÅÛÀÌ °¡µ¿µÇ°í ³ª¼­ ÀÏÁ¤ÇÏ°Ô Ä«¿îÆ®°¡ µÈ °ªÀÌ´Ù.
	return( SirTicTimer ) ;
}

