/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-97 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: TIMER.HV  170.12  97/12/17 17:18:24  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* äTóv																		*/
/*		Timerä÷êîåQíËã`														*/
/*																			*/
/* çÏê¨é“																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__timer_h__)
#define		__timer_h__

#include	"irda.h"


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  Public Functiones  -----  */

int		IrDAAPI	StartTimer( dword msec, void (IrDACB *func)( void )) ;
void	IrDAAPI	StopTimer( int hndl ) ;
void	IrDAAPI	SetTimer( dword IrDAFAR *timer, dword msec ) ;
dword	IrDAAPI	GetTimer( dword timer ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/

#endif	/*__timer_h__*/
