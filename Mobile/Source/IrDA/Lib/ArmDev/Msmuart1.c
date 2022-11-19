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


/************************************************************************
when       who     what, where, why
--------   ---     -----------------------------------------------------
03/24/99   Takagaki     Created file
*************************************************************************/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"                /* Target specific definitions          */
#include "comdef.h"                /* Definitions for byte, word, etc.     */

#if defined( T_MSM3)
#include "processor.h"             /* Definition of CPU registers          */
#else
#include "80186.h"                 /* Definition of CPU registers          */
#endif /* !defined T_MSM3 */

#include "queue.h"                 /* Queue declaration package            */
#include "rex.h"                   /* REX multitasking package             */
#include "sio.h"                   /* Serial I/O service definitions       */

#ifdef CUST_H
#include "customer.h"              /* Customer Specific Definitions        */
#endif /* #ifdef CUST_H */

#if defined( T_MSM3)
#include "memory.h"                /* Memory copy/set routine prototypes   */
#else
#include <memory.h>                /* Memory copy/set routine prototypes   */
#endif /* !defined T_MSM3 */

#include "bio.h"                   /* Bit I/O macros                       */
#include "err.h"                   /* Error services definitions           */
#include "tramp.h"                 /* Trampoline typedefs and prototypes   */
#include "clk.h"                   /* Clock services definitions           */
#include "crc.h"                   /* CRC definitions                      */
#include "clkregim.h"              /* Clock Regime                         */

#if defined ( T_MSM3)
#include "msm.h"                   /* MSM3000 Register Aliases             */
#include "msm3reg.h"
#endif /* T_MSM3 */

#if defined( T_MSM3)
#include "bbjmpout.h"              /* Boot Jump Out Table                  */
#endif /* T_MSM3 */

/* Target dependent include files                                          */

#ifdef FEATURE_DS
#include "ds.h"                    /* Data Services task header file       */
#include "ps.h"                    /* Data protocol task header file       */
#include "dsm.h"                   /* Memory pool management module        */
#include "dsatcopi.h"              /* DS AT command processor              */
#include "enc.h"                   /* Encoder (Interleaver) header file    */
#include "dec.h"                   /* Decoder header file                  */
#include "hw.h"                    /* Hardware Utilities                   */
#endif /* FEATURE_DS */

#include "assert.h"                /* ASSERT functionality                 */
#ifdef FEATURE_TCXO_SHUTDOWN
#include "sleep.h"                 /* Sleep Header file                    */
#endif

/*#include	<stdio.h>*/
#include	"irdaconf.h"
#include	"stdarg.h"
#include	"msmuart.h"
#include	"irsirddk.h"
#include	"ldevice.h"
/*#include	"irda.h"*/

#undef		DOWNLOAD_IRDA

#define	USE_IRDA					/*	Use IrDA Module		*/

#define UART_TXTHLEV     32       /* Setting for TX fifo interupt  */
#define UART_RXTHLEV     32       /* Setting for RX fifo interupt     */
#define	UART_CHARACTERTIMEOUT	2	/*	5Char	*/

#define	DBG_QUE_SIZE	32

void	MsmUart1Intr(void);

#if defined(USE_MONITOR)

int		MsmUart1Getc( void );
void	MsmUart1Flush( void );
void	MsmUart1Putc( int c );


void	MsmUart1Init(void);
static	void	DbgIntr(void);
void	MsmUart1DbgPutc(char c);
void	MsmUart1DbgSetBaud( long baud );
int		MsmUart1DbgGetc(void);
void	MsmUart1DbgRxRead(void);

static	void	IntrMsmUart1DbgTx(void);
static	void	IntrMsmUart1DbgRx(void);

static	void	MsmUart1Err(void);
static int		Sr_Status_OVRERR;
static	int		Sr_Status_PFERR
static	int		Tx_Status;
static	unsigned	char	DbgTxQue[DBG_QUE_SIZE];
static	unsigned	char	DbgRxQue[DBG_QUE_SIZE];

static volatile unsigned short		RxEnq,RxDeq;
static volatile unsigned short		TxEnq,TxDeq;

#endif	/*	USE_MONITOR	*/
static	void	(*pfunc)(void); //htg


void	SirSetBaud( long baud ) ;			/*set SIR Baud Rate		*/
void	SirRxIntOn( void ) ;
void	SirTxIntOn( void ) ;
void	SirIntOff( void ) ;
void	SirWaitTxEmpty( void ) ;


static	void	IntrIdle( void ) ;
static	void	IntrSirRx( void ) ;
static	void	IntrSirTx( void ) ;
int				SirTx( void );

/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1Init												*/
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

#define	BIO_IRDA_SD_M		  0x01 // 0x20
#define	BIO_IRDA_SD_LO_V	0x00
#define	BIO_IRDA_SD_HI_V	0x01  //0x20


void MsmUart1Init( void)
{
	byte c;
	int	i,
  /*-------------------------------------------------------------------------
   Set up Clock Regime Management and disable IRDA transceiver.
   -------------------------------------------------------------------------*/
#ifdef FEATURE_TCXO_SHUTDOWN
    sleep_uart_clock_rgm = TRUE;
#endif

  /* Enable UART clock regime. */
    clk_regime_enable( CLK_RGM_UART_M );
  //NO-Problem
  /* Disable IRDA transceiver. */
 
#if defined(USE_IRDA)
	MSMU_OUT(MSMU_IRDA, UART_IRDA_WB__IRDA_EN_MASK | UART_IRDA_WB__INVERT_IRDA_RX_MASK);
	BIO_TRISTATE(ENC_GPIO_0, BIO_IRDA_SD_M, BIO_IRDA_SD_HI_V); \
	BIO_OUT(ENC_GPIO_0, BIO_IRDA_SD_M, BIO_IRDA_SD_LO_V); 
#else
	MSMU_OUT(MSMU_IRDA, MSMU_IRDA_DISABLE);
#endif		/*	USE_IRDA	*/
  /*-------------------------------------------------------------------------
   Program M, N, and D registers to the proper rates.
   -------------------------------------------------------------------------*/
	//NO-Problem
  
	MSMU_OUT( MSMU_MVR,  0xC0);
	MSMU_OUT( MSMU_NVR,  0xAF);
	MSMU_OUT( MSMU_DVR,  0x80);
	MSMU_OUT( MSMU_MNDR, 0x19);
	//NO-Problem 

	MSMU_OUT( MSMU_MR1, 0);
	MSMU_OUT( MSMU_MR2, ( MSMU_MR2_8BPC | MSMU_MR2_1SB ));	/*	8bit,NonParity,Stopbit:1	*/
	MSMU_OUT( MSMU_CSR, MSMU_CSR_9600_BPS);					/*	baud:115200bps				*/
	//NO-Problem
	MSMU_OUT( MSMU_IMR, 0);					/*	IMR	ALL MASK	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_DIS_TX);			/*	TX:Disable		*/
	MSMU_OUT( MSMU_CR, MSMU_CR_DIS_RX);			/*	RX:Disable		*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_ERR);	

	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);		/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);		/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_TX);		/*	Reset TxFifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_TX);		/*	Reset TxFifo	*/
	MSMU_OUT( MSMU_CR,(MSMU_CR_ENA_TX | MSMU_CR_ENA_RX));			/*	Tx,Rx:Enable		*/
	//NO-Problem
  
	MSMU_OUT( MSMU_TFWR, UART_TXTHLEV);			/*	Tx:Enable		*/
	MSMU_OUT( MSMU_RFWR, UART_RXTHLEV);			/*	Rx:Enable		*/
	//NO-Problem

  
	c	=	inp(MSMU_SR);					/*	Status Register Clear	*/

#if defined(USE_MONITOR)
	RxEnq	=	0;
	RxDeq	=	0;
	TxEnq	=	0;
	TxDeq	=	0;
	
	Sr_Status_OVRERR	=	0;
	Sr_Status_PFERR		=	0;
	Tx_Status			=	0;
#endif	/*	USE_MONITOR	*/


#if defined(USE_MONITOR)
	  pfunc	=	DbgIntr;				/*	Dbg Intr	*/
#else
	  pfunc	=	IntrIdle;				/*	Sir Intr	*/
#endif	/*	USE_MONITOR	*/
	//NO-Problem


  /*	RX Tx InrEnable	*/
	MSMU_OUT( MSMU_IPR, (MSMU_IPR_NEW_RXSTALE | UART_CHARACTERTIMEOUT));			/*	Rx Character Timeout		*/
	//NO-Problem


  /*-------------------------------------------------------------------------
   Install MSM UART interrupt handler
  -------------------------------------------------------------------------*/

	tramp_set_isr(TRAMP_UART_ISR, NULL);
	tramp_set_isr(TRAMP_UART_ISR, MsmUart1Intr);

//	MSMU_OUT( MSMU_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE | MSMU_IMR_TXLEV));	/*	IntOn	*/
} /* end of MsmUart1Init */


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1Intr												*/
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
void MsmUart1Intr( void)
{
		MSMU_OUT( MSMU2_TF, 'I');	/*	after delete	*/
		(*pfunc)();
}	/*	end of MsmUart1Intr	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1Getc												*/
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
#if defined(USE_MONITOR)
int		MsmUart1Getc( void )
{
}	/*	end of MsmUart1Getc	*/
#endif	/*USE_MONITOR*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1Flush												*/
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
void	MsmUart1Flush( void )
{
	for( ; ( inp(MSMU_SR) & MSMU_SR_TXEMT ) == 0 ; ) ;
}	/*	end of MsmUart1Flush	*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1Putc												*/
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
#if defined(USE_MONITOR)
void	MsmUart1Putc( int c )
{
	MsmUart1Flush();
	MSMU_OUT( MSMU_TF, c);
}	/*	end of MsmUart1Putc	*/
#endif	/*	USE_MONITOR	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirSetBaud													*/
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
void	SirSetBaud( long baud ){
	switch(baud){
		case 115200:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_1152K_BPS);
			break;
		case 57600:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_576K_BPS);
			break;
		case 38400:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_384K_BPS);
			break;
		case 19200:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_192K_BPS);
			break;
		case 9600:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_9600_BPS);
			break;
		case 2400:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_2400_BPS);
			break;
	}
}	/*	end of SirSetBaud	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirRxIntOn													*/
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
void	SirRxIntOn( void ){
	MSMU_OUT( MSMU_IMR, 0x00);									/*	All Mask		*/
	pfunc	=	IntrSirRx;										/*	Sir RxIntr On	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);						/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);						/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR,(MSMU_CR_ENA_RX));						/*	Rx:Enable		*/
	MSMU_OUT( MSMU_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE));	/*	RxIntOn	*		*/
}	/*	end of SirRxIntOn	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirTxIntOn													*/
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
void	SirTxIntOn( void ){
	MSMU_OUT( MSMU_IMR, 0x00);									/*	All Mask		*/
	pfunc	=	IntrSirTx;										/*	Sir TxIntr On	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_TX);						/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR,(MSMU_CR_ENA_TX));						/*	Rx:Enable		*/
	MSMU_OUT( MSMU_IMR, MSMU_IMR_TXLEV);						/*	RxIntOn	*		*/
}	/*	end of SirTxIntOn	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirIntOff													*/
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
void	SirIntOff( void ){
	pfunc	=	IntrIdle;										/*	Sir Intr Off	*/
	MSMU_OUT( MSMU_IMR, 0x00);									/*	All Mask		*/
}	/*	end of SirIntOff	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirWaitTxEmpty												*/
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
void	SirWaitTxEmpty( void ){
	for( ; ( inp(MSMU_SR) & MSMU_SR_TXEMT ) == 0 ; ) ;
}	/*	end of SirWaitTxEmpty	*/



/************************************************************************/
/*																		*/
/*	FUNCTION IntrIdle													*/
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
void	IntrIdle( void ){
}	/*	end of IntrIdle	*/

/************************************************************************/
/*																		*/
/*	FUNCTION IntrSirTx													*/
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
static	void	IntrSirTx( void ){
	SirTx();
#if 0
int	c;
	for( ; (inp(MSMU_SR) & MSMU_SR_TXRDY) ; ){
		c	=	IrdaTxCallBack();
		if(c == -1){
			break;
		}
		MSMU_OUT( MSMU_TF, c);
		if(c == 0xc1){
			SirWaitTxEmpty();
			(*SirTxFunc)();
		}
		
	}
	IrdaTxEOI() ;
#endif
}	/*	end of 	IntrSirTx	*/

/************************************************************************/
/*																		*/
/*	FUNCTION SirTx														*/
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
int	SirTx( void )
{
	int	c;
	int flag = 0;
	for( ; (inp(MSMU_SR) & MSMU_SR_TXRDY) ; )
	{
		c	=	IrdaTxCallBack();
		if(c == -1)
		{
			flag	=	1;
			break;
		}
		MSMU_OUT( MSMU_TF, c);
		if(c == 0xc1)
		{
			SirWaitTxEmpty();
			(*SirTxFunc)();
			flag	=	1;
		}else{
			flag	=	0;
		}
	}
	IrdaTxEOI() ;
	return	flag;
}	/*	end of 	SirTx	*/


/************************************************************************/
/*																		*/
/*	FUNCTION IntrSirRx													*/
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
static	void  IntrSirRx( void ){
int	c;
#if defined(USE_MONITOR)
	char	status;
	status	=	inp(MSMU_SR);
	if(status	& MSMU_SR_PF_ERR)
		Sr_Status_PFERR++;
	if(status	& MSMU_SR_OVR_ERR)
		Sr_Status_OVRERR++;
#endif	/*	USE_MONITOR	*/
	for( ; ( inp(MSMU_SR) & MSMU_SR_RXRDY ) ; ){
		c	=	inp(MSMU_RF);
		IrdaRxCallBack(c);
		MSMU_OUT( MSMU2_TF, c);
	}
	SirBusyTimer = 0;
	IrdaRxEOI() ;
}	/*	end of IntrSirRx	*/


/************************************************************************/
/*																		*/
/*	FUNCTION DbgIntr													*/
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
#if defined(USE_MONITOR)
void DbgIntr( void)
{
unsigned int	status;
	MSMU_OUT( MSMU_IMR, 0);
	status	=	inp(MSMU_ISR);
	if(inp(MSMU_SR) & MSMU_SR_RXRDY ){
//	if(status & (MSMU_ISR_RXLEV | MSMU_ISR_RXSTALE)){
		IntrMsmUart1DbgRx();
	}
	if(status & MSMU_ISR_TXLEV){
		IntrMsmUart1DbgTx();
	}
}	/*	end of DbgIntr	*/
#endif	/*	USE_MONITOR	*/


/************************************************************************/
/*																		*/
/*	FUNCTION IntrMsmUart1DbgRx											*/
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
#if defined(USE_MONITOR)
static	void	IntrMsmUart1DbgRx( void)
{
unsigned short	n ;
unsigned char	c ;
	if(Tx_Status){
		for( ; ( inp(MSMU_SR) & MSMU_SR_RXRDY ) ; )
			c	= inp(MSMU_RF) ;
	}else{
		for( ; ( inp(MSMU_SR) & MSMU_SR_RXRDY ) ; ) {
			c	= inp(MSMU_RF) ;
			n	= ( RxEnq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
			if( n != RxDeq ){
				DbgRxQue[RxEnq]	= c ;
				RxEnq	= n ;
			}
		}
	}
}	/*	end of IntrMsmUart1DbgRx	*/
#endif	/*	USE_MONITOR	*/


/************************************************************************/
/*																		*/
/*	FUNCTION IntrMsmUart1DbgTx											*/
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
#if defined(USE_MONITOR)
static	void	IntrMsmUart1DbgTx( void)
{
int	i;
//	for( ; (inp(MSMU_SR) & MSMU_SR_TXRDY) ; ) {
	for(i = 0 ; i < 1 ; i++) {
		if( TxDeq == TxEnq ){
			Tx_Status	=	0;
			MsmUart1DbgFlush();
			MsmUart1DbgRxRead();
			break ;
		}

		MSMU_OUT( MSMU_TF, DbgTxQue[TxDeq]);
		MsmUart1DbgFlush();
		MsmUart1DbgRxRead();
		TxDeq		= ( TxDeq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
		MsmUart1DbgFlush();
		MsmUart1DbgRxRead();
	}
	if( TxDeq == TxEnq )
		MSMU_OUT( MSMU_IMR, MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE);
}	/*	end of IntrMsmUart1DbgTx	*/
#endif	/*	USE_MONITOR	*/




/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1DbgPuts											*/
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
#if defined(USE_MONITOR)
void	MsmUart1DbgPutc(char c)
{
unsigned short	n ;
		n	= ( TxEnq + 1 ) & ( DBG_QUE_SIZE - 1 ) ;
		DbgTxQue[TxEnq]	= c ;
		TxEnq	= n ;

	if( TxEnq != TxDeq ){
	  	if(pfunc	==	DbgIntr){
			MSMU_OUT( MSMU_IMR, (MSMU_IMR_RXLEV | MSMU_IMR_RXSTALE | MSMU_IMR_TXLEV));	/*	TxIntOn	*/
			Tx_Status	=	1;
		}
	}
}	/*	end of MsmUart1DbgPutc	*/
#endif	/*	USE_MONITOR	*/



/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1DbgGetc											*/
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
#if defined(USE_MONITOR)
int	MsmUart1DbgGetc(void)
{
int	c;
	if(RxDeq == RxEnq)
		return	-1;
	c = DbgRxQue[RxDeq];
	RxDeq = (RxDeq + 1) & (DBG_QUE_SIZE - 1);
	return	c;
}	/*	end of MsmUart1DbgGetc	*/
#endif	/*	USE_MONITOR	*/
/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1DbgRxRead											*/
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
#if defined(USE_MONITOR)
void	MsmUart1DbgRxRead(void)
{
char	c;
	for( ; ( inp(MSMU_SR) & MSMU_SR_RXRDY ) ; )
		c	=	inp(MSMU_RF) ;
}	/*	end of MsmUart1DbgRxRead	*/
#endif	/*	USE_MONITOR	*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1DbgFlush											*/
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
#if defined(USE_MONITOR)
void	MsmUart1DbgFlush(void)
{
	for( ; ( inp(MSMU_SR) & MSMU_SR_TXEMT ) == 0 ; ) ;
}	/*	end of MsmUart1DbgFlush	*/
#endif	/*	USE_MONITOR	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmUart1DbgSetBaud											*/
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
#if defined(USE_MONITOR)
void	MsmUart1DbgSetBaud( long baud )
{
	switch(baud){
		case 115200:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_1152K_BPS);
			break;
		case 57600:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_576K_BPS);
			break;
		case 38400:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_384K_BPS);
			break;
		case 19200:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_192K_BPS);
			break;
		case 9600:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_9600_BPS);
			break;
		case 2400:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_2400_BPS);
			break;
	}
}	/*	end of MsmUart1DbgSetBaud	*/
#endif	/*	USE_MONITOR	*/
