/************************************************************************/
/*																		*/
/*		Copyright(c) 1996 Okaya Systemware Co.,Ltd.						*/
/* 		All Rights Reserved												*/
/*																		*/
/*		THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware	*/
/*		The copyright notice above does not evidence an					*/
/*		actual or intended publication of such source code.				*/
/*																		*/
/*			File:		msmuart2.C										*/
/*			Created:		03/24/99									*/
/*			Author:		Katsuhiko Takagaki								*/
/*																		*/
/*			Version:		1.0											*/
/*			Compiler:	ARM C compile									*/
/*																		*/
/************************************************************************/


/*===========================================================================
                        EDIT HISTORY FOR MODULE


when		who		what, where, why
--------	---		----------------------------------------------------------
00.03.21    chos    IrDA Upgrade
00.03.18    chos    IrDA D1 porting
--------	---		----------------------------------------------------------
03/24/99   Takagaki     Created file
------------------------------------------------------------------------------*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include	"target.h"					/* Target specific definitions          */
#include	"comdef.h"					/* Definitions for byte, word, etc.     */

#if defined( T_MSM3)
#include	"processor.h"				/* Definition of CPU registers          */
#else
#include	"80186.h"					/* Definition of CPU registers          */
#endif /* !defined T_MSM3 */

#include	"queue.h"					/* Queue declaration package            */
#include	"rex.h"					/* REX multitasking package             */
#include	"sio.h"					/* Serial I/O service definitions       */

#ifdef CUST_H
#include	"customer.h"				/* Customer Specific Definitions        */
#endif /* #ifdef CUST_H */

#if defined( T_MSM3)
#include	"memory.h"					/* Memory copy/set routine prototypes   */
#else
#include	<memory.h>					/* Memory copy/set routine prototypes   */
#endif /* !defined T_MSM3 */

#include	"bio.h"					/* Bit I/O macros                       */
#include	"err.h"					/* Error services definitions           */
#include	"tramp.h"					/* Trampoline typedefs and prototypes   */
#include	"clk.h"					/* Clock services definitions           */
#include	"crc.h"					/* CRC definitions                      */
#include	"clkregim.h"				/* Clock Regime                         */

#if defined ( T_MSM3)
#include	"msm.h"					/* MSM3000 Register Aliases             */
#if defined (T_MSM5000)
	#include	"msm50reg.h"
#else
	#include	"msm31reg.h"
#endif /* T_MSM5000 */
#endif /* T_MSM3 */

#if defined( T_MSM3)
//#include	"bbjmpout.h"			/* Boot Jump Out Table                  */
#endif /* T_MSM3 */

/* Target dependent include files                                          */

#ifdef FEATURE_DS
#include	"ds.h"					/* Data Services task header file       */
#include	"ps.h"					/* Data protocol task header file       */
#include	"dsm.h"					/* Memory pool management module        */
#include	"dsatcopi.h"			/* DS AT command processor              */
#include	"enc.h"					/* Encoder (Interleaver) header file    */
#include	"dec.h"					/* Decoder header file                  */
#include	"hw.h"					/* Hardware Utilities                   */
#endif	/* FEATURE_DS */

#include	"assert.h"				/* ASSERT functionality                 */
#ifdef FEATURE_TCXO_SHUTDOWN
#include	"sleep.h"				/* Sleep Header file                    */
#endif

#include	"irdaconf.h"
#include	"stdarg.h"

// tasik 00.08.23
/*
	msmuart.h에 정의된 것은 모두 siorsreg.h에 있다. 
*/
//#include	"msmuart.h"
#include "siorsreg.h"

#include	"irsirddk.h"


#define	MAX_FIFO_SIZ	32			/* UART FIFOs are 32 bytes big */
									/* to change after after       */
#define	UART_FIFO_SIZ	32			/* Size of UART Fifos               */

#define	UART_TXTHLEV	32			/* Setting for DM TX fifo interupt  */
#define	UART_RXTHLEV	32			/* Setting for TX fifo interupt     */

#define	DBG_QUE_SIZE	32

void			MsmUart2Init(void);
void			MsmUart2Intr(void);
void			MsmUart2Putc(char c);
void			MsmUart2SetBaud( long baud );
int				MsmUart2Getc(void);
void			MsmUart2Flush( void );

static	void	IntrMsmUart2Tx(void);
static	void	IntrMsmUart2Rx(void);

static	unsigned	char	DbgTxQue[DBG_QUE_SIZE];
static	unsigned	char	DbgRxQue[DBG_QUE_SIZE];

static volatile unsigned short		Rx2Enq,Rx2Deq;
static volatile unsigned short		Tx2Enq,Tx2Deq;


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2Init												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
void MsmUart2Init( void)
{
byte c;
	/*-------------------------------------------------------------------------
		Set up Clock Regime Management and disable IRDA transceiver.
	-------------------------------------------------------------------------*/
#ifdef FEATURE_TCXO_SHUTDOWN
	sleep_uart_clock_rgm=TRUE;
#endif

	/* Enable UART clock regime. */
	clk_regime_enable( CLK_RGM_UART_M );
	MSM_OUTM(DEC_CLK_CTL_1,MSM_CLK_CTL1_WH__CODEC_CLK_EN_N_MASK,0xFF);

	/* Disable IRDA transceiver. */
	MSMU_OUT(MSMU2_IRDA, MSMU_IRDA_DISABLE);

#if defined (T_MSM31)
	MSMU_OUT(CODEC_CTL_WH, CODEC_CTL_WH__UART2_SEL_MASK);			/*	UART2_SEL	*/
#else
    MSM_OUTM(CODEC_CTL2_WB, CODEC_CTL2_WB__UART2_SEL_MASK, 0xFF);
#endif

	/*-------------------------------------------------------------------------
		Program M, N, and D registers to the proper rates.
	-------------------------------------------------------------------------*/
	MSMU_OUT( MSMU2_MVR,  0xC0);
	MSMU_OUT( MSMU2_NVR,  0xAF);
	MSMU_OUT( MSMU2_DVR,  0x80);
	MSMU_OUT( MSMU2_MNDR, 0x19);

	MSMU_OUT( MSMU2_MR1, 0);
	MSMU_OUT( MSMU2_MR2, ( MSMU_MR2_8BPC | MSMU_MR2_1SB ));		/*	8bit,NonParity,Stopbit:1	*/
	MSMU_OUT( MSMU2_CSR, MSMU_CSR_192K_BPS);					/*	baud:19200bps				*/

	MSMU_OUT( MSMU2_IMR, 0);										/*	IMR	ALL MASK				*/
	MSMU_OUT( MSMU2_CR, MSMU_CR_DIS_TX);							/*	TX:Disable					*/
	MSMU_OUT( MSMU2_CR, MSMU_CR_DIS_RX);							/*	RX:Disable					*/
	MSMU_OUT( MSMU2_CR, MSMU_CR_RESET_ERR);	
	MSMU_OUT( MSMU2_CR, MSMU_CR_RESET_RX);						/*	Reset Rxfifo				*/
	MSMU_OUT( MSMU2_CR, MSMU_CR_RESET_TX);						/*	Reset TxFifo				*/
	MSMU_OUT( MSMU2_CR,(MSMU_CR_ENA_TX | MSMU_CR_ENA_RX));		/*	Tx,Rx:Enable				*/
	MSMU_OUT( MSMU2_TFWR, UART_TXTHLEV);						/*	Tx:Enable					*/
	MSMU_OUT( MSMU2_RFWR, UART_RXTHLEV);						/*	Rx:Enable					*/

	c	=	inp(MSMU2_SR);										/*	Status Register Clear		*/

	Rx2Enq	=	0;
	Rx2Deq	=	0;
	Tx2Enq	=	0;
	Tx2Deq	=	0;

	/*	RX Tx InrEnable	*/


	/*-------------------------------------------------------------------------
		Install MSM UART interrupt handler
	-------------------------------------------------------------------------*/
//	tramp_set_isr(TRAMP_UART2_ISR, MsmUart2Intr);
//	MSMU_OUT( MSMU2_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE | MSMU_IMR_TXLEV));	/*	IntOn	*/

} /* end of MsmUart2Init */

/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2Flush												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
void	MsmUart2Flush( void )
{
	for( ; ( inp(MSMU2_SR) & MSMU_SR_TXEMT ) == 0 ; ) ;
}/*	end of MsmUart2Flush	*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2Intr												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
void MsmUart2Intr( void)
{
unsigned int	status;
	status	=	inp(MSMU2_ISR);
	if(status & (MSMU_ISR_RXLEV | MSMU_ISR_RXSTALE)){
		IntrMsmUart2Rx();
	}
	if(status & MSMU_ISR_TXLEV){
		IntrMsmUart2Tx();
	}
}	/*	end of MsmUart2Intr	*/


/************************************************************************/
/*																		*/
/*	FUNCTION IntrMsmUart2Rx												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
static	void	IntrMsmUart2Rx( void)
{
unsigned short	n ;
unsigned char	c ;
	for( ; ( inp(MSMU2_SR) & MSMU_SR_RXRDY ) ; ) {
		c	= inp(MSMU2_RF) ;
		n	= ( Rx2Enq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
		if( n != Rx2Deq ){
			DbgRxQue[Rx2Enq]	= c ;
			Rx2Enq	= n ;
		}
	}
}	/*	end of IntrMsmUart2Rx	*/


/************************************************************************/
/*																		*/
/*	FUNCTION IntrMsmUart2Tx												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
static	void	IntrMsmUart2Tx( void)
{
	for( ; (inp(MSMU2_SR) & MSMU_SR_TXRDY) ; ) {
		if( Tx2Deq == Tx2Enq )
			break ;
#if defined (T_MSM31)
		MSMU_OUT(CODEC_CTL_WH, CODEC_CTL_WH__UART2_SEL_MASK);			/*	UART2_SEL	*/
#else
	    MSM_OUTM(CODEC_CTL2_WB, CODEC_CTL2_WB__UART2_SEL_MASK, 0xFF);
#endif
		MSMU_OUT( MSMU2_TF, DbgTxQue[Tx2Deq]);
		Tx2Deq		= ( Tx2Deq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
	}

	if( Tx2Deq == Tx2Enq )
		MSMU_OUT( MSMU2_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE));
}	/*	end of IntrMsmUart2Tx	*/




/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2Puts												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
void	MsmUart2Putc(char c)
{
unsigned short	n ;
		n	= ( Tx2Enq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
		DbgTxQue[Tx2Enq]	= c ;
		Tx2Enq	= n ;

	if( Tx2Enq != Tx2Deq )
		MSMU_OUT( MSMU2_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE | MSMU_IMR_TXLEV));	/*	TxIntOn	*/
}	/*	end of MsmUart2Puts	*/



/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2Getc												*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
int	MsmUart2Getc(void)
{
int	c;
	if(Rx2Deq == Rx2Enq)
		return	-1;
	c = DbgRxQue[Rx2Deq];
	Rx2Deq = (Rx2Deq + 1) & (DBG_QUE_SIZE - 1);
	return	c;
}	/*	end of MsmUart2Getc	*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart2SetBaud											*/
/*																		*/
/*	DESCRIPTION															*/
/*	Initialize the MSM2P UART serial interface for operation.			*/
/*																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*	Aborts any transmit or receive which is currently in progress.		*/
/*																		*/
/************************************************************************/
void	MsmUart2SetBaud( long baud )
{
	switch(baud){
		case 115200:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_1152K_BPS);
			break;
		case 57600:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_576K_BPS);
			break;
		case 38400:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_384K_BPS);
			break;
		case 19200:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_192K_BPS);
			break;
		case 9600:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_9600_BPS);
			break;
		case 2400:
			MSMU_OUT( MSMU2_CSR, MSMU_CSR_2400_BPS);
			break;
	}
}	/*	end of MsmUart2SetBaud	*/

void	MsmCheckWrite( char c )
{
	MSMU_OUT( MSMU2_TF, c);
}	/*	end of MsmCheckWrite	*/


