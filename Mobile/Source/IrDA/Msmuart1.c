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
#include "target.h"                /* Target specific definitions          */
#include "comdef.h"                /* Definitions for byte, word, etc.     */

#ifdef	FEATURE_USE_IRDA

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

// tasik 00.08.20
#ifdef T_MSM5000 
#include "msm.h"					/* MSM5000 Register Aliases             */
#include "msm50reg.h"
#else
#include "msm.h"                   /* MSM3000 Register Aliases             */
#include "msm31reg.h"
#endif /* T_MSM3 */

#if defined( T_MSM3)
//#include "bbjmpout.h"              /* Boot Jump Out Table                  */
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

//#include	"msmuart.h"
#include	"irsirddk.h"
#include	"ldevice.h"
/*#include	"irda.h"*/

#include	"IrDATool.h"
#include	"dog.h"
#include    "siorsreg.h"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

#undef		DOWNLOAD_IRDA

#ifdef	FEATURE_IRDA_UPGRADE

extern byte IrDACurrentRate;

// tasik 00.08.14

#if (MODEL_ID == MODEL_A10) || (MODEL_ID == MODEL_B1)  || (MODEL_ID == MODEL_B2)

//#define UART_TXLEV	16     /* Setting for TX fifo interupt  */
//#define UART_RXLEV	64//16		  /* Setting for RX fifo interupt     */
//#define	UART_SENDSIZE	64

#ifdef FEATURE_IRDA_HIGH_RATE
int UART_FIFO_MAIN_SIZE = 512;
#endif

#define UART_TXLEV	16     /* Setting for TX fifo interupt  */
#define UART_RXLEV	64		  /* Setting for RX fifo interupt     */
#define	UART_SENDSIZE	128 


#elif MODEL_ID == MODEL_D1
 
//#define UART_TXLEV	4     /* Setting for TX fifo interupt  */
//#define UART_RXLEV	16		  /* Setting for RX fifo interupt     */
//#define	UART_SENDSIZE	32
//#define UART_FIFO_MAIN_SIZE 512

#ifdef FEATURE_IRDA_HIGH_RATE
int UART_FIFO_MAIN_SIZE = 512;
#endif

int UART_TXLEV=	16;     /* Setting for TX fifo interupt  */
int UART_RXLEV=	64;//16		  /* Setting for RX fifo interupt     */
int	UART_SENDSIZE=	64;

#else

#define UART_TXLEV	8     /* Setting for TX fifo interupt  */
#define UART_RXLEV	16		  /* Setting for RX fifo interupt     */
#define	UART_SENDSIZE	32

#endif // #if MODEL_ID == MODEL_A10

#endif	// #ifdef	FEATURE_IRDA_UPGRADE

// tasik 00.08.21
#ifdef T_MSM5000
#define	UART_LOWERBIT_CHARACTERTIMEOUT	    0x0002//0x08 		/*	2 char	*/
#define UART_UPPERBIT_CHARACTERTIMEOUT		0x0000//0x080
#else
#define UART_CHARACTERTIMEOUT	2		/* 2 char */
#endif

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

#ifdef	FEATURE_IRDA_UPGRADE
extern char	ui_bIrDAExchangeMode;
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

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

void MsmUart1Init( void)
{
	byte c;
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
 
#ifdef FEATURE_USE_IRDA
	MSMU_OUT(MSMU_IRDA, UART_IRDA_WB__IRDA_EN_MASK | UART_IRDA_WB__INVERT_IRDA_RX_MASK);
#if MODEL_ID == MODEL_B1  || (MODEL_ID == MODEL_B2)
	BIO_TRISTATE(DEC_GPIO_1, BIO_IRDA_M, BIO_OUTPUT_ENA_V);
	BIO_OUT(DEC_GPIO_1, BIO_IRDA_M, BIO_IRDA_ENA_V); 
#else
	BIO_TRISTATE(ENC_GPIO_0, BIO_IRDA_M, BIO_OUTPUT_ENA_V);
	BIO_OUT(ENC_GPIO_0, BIO_IRDA_M, BIO_IRDA_ENA_V); 
#endif
#else
	MSMU_OUT(MSMU_IRDA, MSMU_IRDA_DISABLE);
#endif		/*	USE_IRDA	*/
  /*-------------------------------------------------------------------------
   Program M, N, and D registers to the proper rates.
   -------------------------------------------------------------------------*/
	//NO-Problem
  
	MSMU_OUT( MSMU_MVR,  MSMU_MVR_DEFAULT);
	MSMU_OUT( MSMU_NVR,  MSMU_NVR_DEFAULT);
	MSMU_OUT( MSMU_DVR,  MSMU_DVR_DEFAULT);
	MSMU_OUT( MSMU_MNDR, MSMU_MNDR_DEFAULT);
	//NO-Problem 

	// added by chos 99.08.16
	DMOD_OUTM( DMOD_POLARITY, DMOD_RX_DATA_INT_POL_M, 0xff);
	tramp_set_isr( TRAMP_DP_RX_DATA_ISR, NULL);
	// finish chos

// tasik 00.08.14
/*	
	MSMU_MR1은 5000D에서는 word단위로 정의된다.
*/

#ifdef T_MSM5000
	MSMUW_OUT( MSMU_MR1, 0x0000);
#else
	MSMU_OUT(MSMU_MR1,0x00);
#endif //#ifdef T_MSM5000

	MSMU_OUT( MSMU_MR2, ( MSMU_MR2_8BPC | MSMU_MR2_1SB ));	/*	8bit,NonParity,Stopbit:1	*/
	MSMU_OUT( MSMU_CSR, MSMU_CSR_9600_BPS);					/*	baud:115200bps				*/

	MSMU_OUT( MSMU_IMR, 0);					/*	IMR	ALL MASK	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_DIS_TX);			/*	TX:Disable		*/
	MSMU_OUT( MSMU_CR, MSMU_CR_DIS_RX);			/*	RX:Disable		*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_ERR);	

	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);		/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_RX);		/*	Reset Rxfifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_TX);		/*	Reset TxFifo	*/
	MSMU_OUT( MSMU_CR, MSMU_CR_RESET_TX);		/*	Reset TxFifo	*/

// tasik 00.08.14, 
/*
	MSMU_TFWR, MSMU_RFWR은 5000에서는 word단위로 정의된다.
*/
#ifdef T_MSM5000 
	MSMUW_OUT( MSMU_TFWR, UART_TXLEV);			/*	Tx Watermarking		*/
	MSMUW_OUT( MSMU_RFWR, UART_RXLEV);			/*	Rx Watermarking		*/
#else
	MSMU_OUT( MSMU_TFWR, UART_TXLEV);			/*	Tx Watermarking		*/
	MSMU_OUT( MSMU_RFWR, UART_RXLEV);			/*	Rx Watermarking		*/
#endif //#ifdef T_MSM5000 

	//NO-Problem
	
	MSMU_OUT( MSMU_CR,(MSMU_CR_ENA_TX | MSMU_CR_ENA_RX));			/*	Tx,Rx:Enable		*/

 
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
// tasik 00.08.16
#ifdef T_MSM5000
	MSMUW_OUT( MSMU_IPR, (MSMU_IPR_NEW_RXSTALE | UART_LOWERBIT_CHARACTERTIMEOUT | UART_UPPERBIT_CHARACTERTIMEOUT));			/*	Rx Character Timeout		*/
#else
	MSMU_OUT(MSMU_IPR, (MSMU_IPR_NEW_RXSTALE | UART_CHARACTERTIMEOUT));
#endif
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
// erased by chos  		MSMU_OUT( MSMU2_TF, 'I');	/*	after delete	*/
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

#ifdef	FEATURE_IRDA_UPGRADE
	SirWaitTxEmpty() ;
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

	switch(baud){
		// tasik 00.08.22
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
	
	// tasik 00.10.18
#ifdef FEATURE_IRDA_HIGH_RATE

	switch(baud){
		case 115200:
			IrDACurrentRate = MSMU_CSR_1152K_BPS;
			break;
		case 57600:
			IrDACurrentRate = MSMU_CSR_576K_BPS;
			break;
		case 38400:
			IrDACurrentRate = MSMU_CSR_384K_BPS;
			break;
		case 19200:
			IrDACurrentRate = MSMU_CSR_192K_BPS;
			break;
		case 9600:
			IrDACurrentRate = MSMU_CSR_9600_BPS;
			break;
		case 2400:
			IrDACurrentRate = MSMU_CSR_2400_BPS;
			break;
	}
#endif
	// tasik end
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
	MSMU_OUT( MSMU_CR,(MSMU_CR_ENA_TX));						/*	Tx:Enable		*/
	MSMU_OUT( MSMU_IMR, MSMU_IMR_TXLEV);						/*	TxIntOn	*		*/
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
	for( ; ( inp(MSMU_SR) & MSMU_SR_TXEMT ) == 0 ; )
		dog_kick ();
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
/*	가장 하위층인 SIR 층에서 실제로 데이터를 UART FIFO에 써넣는다.      	
	기존의 루딘에서 feature_irda_high_rate로 정의된 곳을 보강하였다.

	보강된 루틴은 Data Service (DS)에서 취한 방식과 비슷하다. 
	먼저 UART_TXLEV의 인터럽트가 발생하게 된다면 (fifo_size - txlev )
	만큼의 쓸 공간이 생겼다는 것을 의미한다. 따라서 이만큼의 공간에 
	대해서는 비어 있는지를 확인할 필요가 없다. 따라서 -1이 나올 때까지
	비어진 공간에 그냥 써넣는다. 이렇게 하면 기존에 포트를 읽어 들여서
	쓸 수 있는 지를 검사할 때 발생하는 지연을 단순한 논리적인 지연시간으로 
	대체할 수 있다. 따라서 속도를 증가시킬 수 있다.

    만약 인터럽트에 의해서 본 함수가 불리워 진 것이 아니라면 IrDA의 
	타이머에 의해서 불리워 진 것이다. 이때가 되면 두가지 경우가 있다.

    1) 타이머에 의해 불리워 진 경우 + 인터럽트도 뜬 경우
	2) 타이머에 의해서만 불리워진 경우

	두번째의 경우는 아래에서 두번째 루프만 수행하게 된다.
																		*/
/*	DEPENDENCIES														*/
/*	None																*/
/*																		*/
/*	RETURN VALUE														*/
/*	None																*/
/*																		*/
/*	SIDE EFFECTS														*/
/*			*/
/*																		*/
/************************************************************************/

extern boolean g_fIrDAPortForPC; /* defined in ska_irpc.c */

static	void	IntrSirTx( void ){

#ifdef	FEATURE_IRDA_UPGRADE

	int	c , i ;
	
  for(i = 0 ; ( (i < UART_SENDSIZE ) && (inp(MSMU_SR) & MSMU_SR_TXRDY) ) ; i++)
  {
		c	=	IrdaTxCallBack();
		if(c == -1)
		{
			break ;
		}
		MSMU_OUT( MSMU_TF, c);
#if !defined(TX_NOWAIT_CONFIRM)
		if(c == 0xc1){
			SirWaitTxEmpty();
			(*SirTxFunc)();	/*	Modify OSW:Takagaki 2000/01/20	*/
		}
#endif	/*	TX_NOWAIT_CONFIRM	*/
	}

	IrdaTxEOI() ;

#else	// #ifdef	FEATURE_IRDA_UPGRADE
	SirTx();
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

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


/***********************************************************************

	FUNCTION IntrSirRx											
																
	DESCRIPTION											
	가장 하위층인 SIR 층에서 실제로 데이터를 UART FIFO에서 읽어 온다.
	기존의 루딘에서 feature_irda_high_rate로 정의된 곳을 보강하였다.

	보강된 루틴은 Data Service (DS)에서 취한 방식과 비슷하다. 
	먼저 UART_RXLEV의 인터럽트가 발생하게 된다면 RXLEV
	만큼의 쓸 공간이 생겼다는 것을 의미한다. 따라서 이만큼의 공간에 
	대해서는 비어 있는지를 확인할 필요가 없다. 비어진 공간에 그냥 써넣는다. 
	이렇게 하면 기존에 포트를 읽어 들여서 쓸 수 있는 지를 검사할 때 
	발생하는 지연을 단순한 논리적인 지연시간으로 대체할 수 있다. 
	따라서 속도를 증가시킬 수 있다

  
    만약 인터럽트에 의해서 본 함수가 불리워 진 것이 아니라면 IrDA의 
	타이머에 의해서 불리워 진 것이다. 이때가 되면 두가지 경우가 있다.

    1) 타이머에 의해 불리워 진 경우 + 인터럽트도 뜬 경우
	2) 타이머에 의해서만 불리워진 경우

	두번째의 경우는 아래에서 두번째 루프만 수행하게 된다.

	DEPENDENCIES														
	None																
																		
	RETURN VALUE														
	None																
																		
	SIDE EFFECTS														
																		
***********************************************************************/
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

	// tasik 00.10.18 잠시 막아 놓는다.
/*
#ifdef FEATURE_IRDA_HIGH_RATE
	int DataNumberWithoutCheck;
	int i;

    if ( inp( MSMU_ISR) & MSMU_ISR_RXLEV)
	{
		DataNumberWithoutCheck = (int) UART_RXLEV;
	
		for(i=0; i < DataNumberWithoutCheck; i++)
		{
			c = inp(MSMU_RF);
			IrdaRxCallBack(c);
		}
	}
#endif
	// tasik end
*/
	for( ; ( inp(MSMU_SR) & MSMU_SR_RXRDY ) ; ){
		c	=	inp(MSMU_RF);
		IrdaRxCallBack(c);
// erased by chos  		MSMU_OUT( MSMU2_TF, c);
	}


#ifdef	FEATURE_IRDA_UPGRADE
	IrdaRxEOI() ;
#else	// #ifdef	FEATURE_IRDA_UPGRADE
	SirBusyTimer = 0;
	if(ui_bIrDAExchangeMode)
	{
		IrdaRxEOI() ;
	}
	else
	{
		IrDA_ProtocolCB ();
	}
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

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
				// tasik 00.08.22
#ifdef T_MSM5000
		case 230400:
			MSMU_OUT( MSMU_CSR, MSMU_CSR_2304K_BPS);
			break;
#endif // #ifdef T_MSM5000

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

#endif	// #ifdef	FEATURE_USE_IRDA
