/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IRSIRRX.C                                    */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/* $Header$ */

#include	"irdaconf.h"
#include	<irsir.h>
#include	"irsirddk.h"

static				word	crc;
					uint	RxTableNo;
static	volatile	byte	*RxStore,*RxFree,*RxEnd;

volatile void	(IrDAAPI *SirRxFunc)(int c) ;

/*********************************************************************

	Recive Controls

**********************************************************************/
/*  -----  Rx Subfunctions  -----  */

static	void	RxIdle( int c ) ;
static	void	RxWaitHeader( int c ) ;
static	void	RxHuntHeader( int c ) ;
static	void	RxHuntEsc(int c );
static	void	RxFrame( int c ) ;
static	void	RxFrameEsc( int c ) ;

void	SIRSetRxIdle()
{
	SirRxFunc	= (volatile void(IrDAAPI *)(int))RxIdle ;
}

void	SIRSetRxBuffer()
{
	RxEnd	= SirRecvBuffer + SirRecvBufferSize;
}

void	SIRKickRx()
{
	SirInFrame	= FALSE;
	SirRxFunc 	= (volatile void(IrDAAPI *)(int))RxWaitHeader;
}

void	SIRResetRxBuff()
{
	*SirRecvWinSizePtr = RxTableNo = 0;
	RxFree	= SirRecvBuffer;
	RxEnd	= SirRecvBuffer + SirRecvBufferSize;
	SIRKickRx();
}

/*
**	受信ステートメントで何もしない
*/

static	void	RxIdle( int c )
{
}

/*
**	ヘッダーを待つ。
**	BOF	-> RxHuntHeader
**	else	-> RxWaitHeader
*/
static	void	RxWaitHeader( int c )
{
	if(c != IrSIR_BOF )
		return;
	SirRxFunc	= (volatile void(IrDAAPI *)(int))RxHuntHeader;
}

/*
**	機能：
**		ヘッダー以外を待つ
**		BOF|ESC	-> RxHuntHeader
**		else	-> RxFrame
*/

static	void	RxHuntHeader( int c )
{
int	ca;
	if( c == IrSIR_BOF ){
		return;
	}
	if( c == IrSIR_ESC ){
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxHuntEsc;
		return;
	}
	if( c == IrSIR_EOF ){
		SIRKickRx();
		return;
	}
	ca = c & IrSIR_GROB;
	if(ca == SirConnAddr || ca == IrSIR_GROB ){
		RxStore	 = RxFree;
		if( RxStore < RxEnd ) {
			crc		= (word)-1 ;
			UpdateCRC( c ) ;
			*(RxStore++)	= (byte)c ;
			SirInFrame	= TRUE ;
			SirRxFunc	= (volatile void(IrDAAPI *)(int))RxFrame ;
			return;
		}
	}
	SIRKickRx();
	return;
}

static	void	RxHuntEsc( int c )
{
int	ca;
	if( c == IrSIR_BOF ){
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxHuntHeader;
		return;
	}
	if( c == IrSIR_ESC || c == IrSIR_EOF ){
		SIRKickRx();
		return;
	}
	c ^= IrSIR_XOR;
	ca = c & IrSIR_GROB;
	if(ca == SirConnAddr || ca == IrSIR_GROB ){
		RxStore	 = RxFree;
		if( RxStore < RxEnd ) {
			crc		= (word)-1 ;
			UpdateCRC( c ) ;
			*(RxStore++)	= (byte)c ;
			SirInFrame	= TRUE ;
			SirRxFunc	= (volatile void(IrDAAPI *)(int))RxFrame ;
			return;
		}
	}
	SIRKickRx();
	return;
}

/*
**	フレーム受信。
**	BOF|EOF	-> RxHuntHeader
**	else	-> RxFrame
*/
static	void	RxFrame( int c )
{
	if( c == IrSIR_ESC ){
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxFrameEsc;
		return;
	}
	if( c == IrSIR_BOF ){
		SirInFrame	= FALSE ;
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxHuntHeader ;
		return;
	}
	if( c != IrSIR_EOF ){
		if( RxStore < RxEnd){
			UpdateCRC(c);
			*(RxStore++) = (byte)c;
			return;
		}
		SIRKickRx();
		return;
	}
	/* EOF Proc */
	SirInFrame	= FALSE;
	if( crc != GOODCRC ) {
		SIRKickRx();
		return;
	}
	RxStore -= 2;
	SirRecvBufTbl[RxTableNo]  = (byte *)RxFree;
	SirRecvSizeTbl[RxTableNo] = RxStore - RxFree;
	*SirRecvWinSizePtr = ++RxTableNo;
	SirDataFunc = SirDataIndPtr;
	if(RxTableNo < SirRecvTblSize){
		RxFree		= RxStore;
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxWaitHeader;
		return;
	}
	SirRxFunc	= (volatile void(IrDAAPI *)(int))RxIdle ;
	return;
}

static	void	RxFrameEsc( int c )
{
	if( c == IrSIR_ESC || c == IrSIR_EOF ){
		SIRKickRx();
		return;
	}
	if( c == IrSIR_BOF) {
		SirInFrame	= FALSE ;
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxHuntHeader ;
		return;
	}
	c ^= IrSIR_XOR;
	if( RxStore < RxEnd){
		UpdateCRC(c);
		*(RxStore++)	= (byte)c;
		SirRxFunc	= (volatile void(IrDAAPI *)(int))RxFrame;
		return;
	}
	SIRKickRx();
	return;
}

/* $Log$ */
