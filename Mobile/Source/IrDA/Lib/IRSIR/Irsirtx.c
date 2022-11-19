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
	SIR ���� ������ ó���� ������ ����.



      ------							 ----------
     | Idle | ---(transmit.request)---> | BOF wait | 
      ------							 ----------		 
		\								      |
		\									  |  BOF(0xC0) ����
		\								  ----------
		\								 | Data Wait|
		\								  ----------	
		\								     \    |
		\					    BOF(0xC0) �� \    | BOF(0xC0) �̿��� ��� ��
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

  | : �Ʒ� ����
  \ : �� ���� 

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
**	Tx�� ���� ���ϵ��� �����Ѵ�.
*/

void	SIRSetTxIdle()
{
	SirTxFunc	= (volatile int	(IrDAAPI *)(void))TxIdle ;
}

/*
**	Tx�� �����ϱ� ���� ����
*/

void	SIRKickTx(void)
{
	crc		= (word)-1 ;
	TxBOFCount	= SirNoOfBOF ;
	SirTxFunc	= (volatile int	(IrDAAPI *)(void))TxBOF ;
}

/*
**	Tx�� �� �� ������ �˸�.
*/
static	int	TxIdle( void )
{
	return	-1;
}


/*
	Tx�� �����Ѵ�. �׷��� ������ tx�� txframe���� �Ѵ�.
*/

static	int	TxBOF( void )
{
	if(!(--TxBOFCount)){
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxFrame ;
	}
	return( IrSIR_BOF ) ; // �������� ������ �˸�
}

/*
	������ Tx �������� �����´�.
*/

static	int	TxFrame( void )
{
byte	c ;
	c = *SirSendBuffer++;
	UpdateCRC( c );
	if(isESCCode( c )) {
		cback   = c;
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxFrameESC;
		return	IrSIR_ESC; // esc�ڵ尡 �������� �˸�.
	}
	if(SirSendBuffer >= SirSendBufferEnd)
		SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC0;
	return	c;
}

/*
**	ESC�ڵ尡 ������ ���� ó���� �����Ѵ�.
**	���� ���۹��۸� �Ѿ�ٸ� 	-> TxCRC0
**	�׷��� ������ �ٽ�			-> TxFrame
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
**	crc�� ���� esc�ڵ��̸� -> TxCRC0ESC
    �׷��� ������          -> TxCRC1
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
**	crc�� esc�ڵ��̸� �׻� 	-> TxCRC1
*/
static	int	TxCRC0ESC( void )
{
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxCRC1 ;
	return	crc ^ IrSIR_XOR;
}

/*
**	crc�ڵ��� ���� esc�ڵ尡 �ƴ� ����̸鼭, 
	CRC�� 8�ڸ� ���������� �Ű��� �� �� ����
**	ESC�ڵ��̸� 	-> TxCRC1ESC
**	�׷��� ������	-> TxEOF
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
**	crc�ڵ��� ���� esc�ڵ尡 �ƴϰ�, crc�� 
���������� 8��Ʈ �Ű��� �� �װ��� esc �ڵ尡 �ƴϸ� �׻�
  -> TxEOF (Tx End of Frame, ���۳�)
*/
static	int	TxCRC1ESC( void )
{
	SirTxFunc = (volatile int	(IrDAAPI *)(void))TxEOF ;
	return	(( crc >> 8 ) ^ IrSIR_XOR);
}

/*
**	Send EOF
**	all --> Txidle
	
    ���̻� tx�� ���� �ʰ�, rx�� �غ��Ѵ�.
	�׸��� end of frame ��ȣ�� �����Ѵ�.

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
