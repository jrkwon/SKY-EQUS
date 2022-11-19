/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IRSIRTX.C                                    */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/*
	SIR 층의 데이터 처리는 다음과 같다.



      ------							 ----------
     | Idle | ---(transmit.request)---> | BOF wait | 
      ------							 ----------		 
		\								      |
		\									  |  BOF(0xC0) 받음
		\								  ----------
		\								 | Data Wait|
		\								  ----------	
		\								     \    |
		\					    BOF(0xC0) 줌 \    | BOF(0xC0) 이외의 모든 값
		\									 \    |
		\									 \    |
		\								  -------------					 ----------
		\								 |    Data     | ---> ESC(0x7D) | Received |
		\								 |   Receive   | <---			|    ESC   |
		\								  -------------	                 ---------- 
		\								        |
		\									    |
		\	  --------------					|
		---- | EOF Receiving|	<---------------
			  --------------	    EOF(0xC1)			

  | : 아래 방향
  \ : 윗 방향 

*/






/* $Header$ */

#include	"irdaconf.h"
#include	<irda.h>
#include	<irsir.h>
#include	"irsirddk.h"
#include	"console.h"

static	byte	cback;
static	int		TxBOFCount;
static	word	crc;
byte	*SirSendBuffer;
byte	*SirSendBufferEnd;

volatile int	(IrDAAPI *SirTxFunc)(void ) ;


/********************************************************************

	Transmit Control

*********************************************************************/
/*  -----  Tx Subfunctions  -----  */

static	int	TxIdle( void ) ;
static	int	TxBOF( void ) ;
static	int	TxFrame( void ) ;
static	int	TxFrameESC( void ) ;
static	int	TxCRC0( void ) ;
static	int	TxCRC0ESC( void ) ;
static	int	TxCRC1( void ) ;
static	int	TxCRC1ESC( void ) ;
static	int	TxEOF( void ) ;
#ifndef TX_NOWAIT_CONFIRM
static	int	TxCONF( void ) ;
#endif
/*
**	Tx를 하지 못하도록 설정한다.
*/

void	SIRSetTxIdle()
{
	SirTxFunc	= (volatile int	(IrDAAPI *)(void))TxIdle ;
}

/*
**	Tx를 시작하기 위한 설정
*/

void	SIRKickTx(void)
{
	crc		= (word)-1 ;
	TxBOFCount	= SirNoOfBOF ;
	SirTxFunc	= (volatile int	(IrDAAPI *)(void))TxBOF ;
}

/*
**	Tx를 할 수 없음을 알림.
*/
static	int	TxIdle( void )
{
	return	-1;
}


/*
	Tx를 유지한다. 그러나 실제로 tx는 txframe에서 한다.
*/

static	int	TxBOF( void )
{
	if(!(--TxBOFCount)){
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxFrame ;
	}
	return( IrSIR_BOF ) ; // 프레임의 시작을 알림
}

/*
	실제로 Tx 프레임을 가져온다.
*/

static	int	TxFrame( void )
{
byte	c ;
	c = *SirSendBuffer++;
	UpdateCRC( c );
	if(isESCCode( c )) {
		cback   = c;
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxFrameESC;
		return	IrSIR_ESC; // esc코드가 들어왔음을 알림.
	}
	if(SirSendBuffer >= SirSendBufferEnd)
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC0;
	return	c;
}

/*
**	ESC코드가 들어왔을 때의 처리를 정의한다.
**	만약 전송버퍼를 넘어섰다면 	-> TxCRC0
**	그렇지 않으면 다시			-> TxFrame
*/
static	int	TxFrameESC( void )
{
	if(SirSendBuffer >= SirSendBufferEnd)
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC0;
	else
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxFrame;
	return	cback ^ IrSIR_XOR;
}

/*
**	crc의 역도 esc코드이면 -> TxCRC0ESC
    그렇지 않으면          -> TxCRC1
*/
static	int	TxCRC0( void )
{
byte	c ;
	crc	= ~crc;
	c	= (byte)crc;
	if(isESCCode( c )){
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC0ESC ;
		return	IrSIR_ESC;
	}
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC1;
	return	c;
}

/*
**	crc가 esc코드이면 항상 	-> TxCRC1
*/
static	int	TxCRC0ESC( void )
{
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC1 ;
	return	crc ^ IrSIR_XOR;
}

/*
**	crc코드의 역이 esc코드가 아닌 경우이면서, 
	CRC를 8자리 오른쪽으로 옮겼을 때 그 값이
**	ESC코드이면 	-> TxCRC1ESC
**	그렇지 않으면	-> TxEOF
*/
static	int	TxCRC1( void )
{
byte	c ;
	c = crc >> 8;
	if(isESCCode( c )){
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC1ESC ;
		return	IrSIR_ESC;
	}
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxEOF ;
	return	c;
}


/*
**	crc코드의 역이 esc코드가 아니고, crc를 
오른쪽으로 8비트 옮겼을 때 그값도 esc 코드가 아니면 항상
  -> TxEOF (Tx End of Frame, 전송끝)
*/
static	int	TxCRC1ESC( void )
{
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxEOF ;
	return	(( crc >> 8 ) ^ IrSIR_XOR);
}

/*
**	Send EOF
**	all --> Txidle
	
    더이상 tx를 하지 않고, rx를 준비한다.
	그리고 end of frame 신호를 리턴한다.

*/
static	int	TxEOF( void )
{
#if defined(TX_NOWAIT_CONFIRM)	/*	Modify OSW:Takagaki 2000/01/20	*/
	SirDataFunc		=	SirDataCnfPtr;
	SirDataCnfPtr	=	0;
	SirTxFunc		=	(volatile int	(IrDAAPI *)(void))TxIdle;
	SIRSetRxIntr();
#else
	SirTxFunc		=	(volatile int	(IrDAAPI *)(void))TxCONF;
#endif	/*	TX_NOWAIT_CONFIRM	*/
	return	IrSIR_EOF;
}

#ifndef TX_NOWAIT_CONFIRM
static	int	TxCONF( void )
{
	SirDataFunc		=	SirDataCnfPtr;
	SirDataCnfPtr	=	0;
	SirTxFunc		=	(volatile int	(IrDAAPI *)(void))TxIdle;
	SIRSetRxIntr();
	return	-1;
}
#endif

/* $Log$ */
