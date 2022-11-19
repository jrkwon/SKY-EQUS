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

#if defined ( T_MSM3)
#include "msm.h"                   /* MSM3000 Register Aliases             */
//#include "msm31reg.h"
#include "msm50reg.h"
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

#include	"dsi.h"
#include	"boothw.h"

#include	"irdaconf.h"
#include	"irsirddk.h"
#include	"ldevice.h"
// tasik 00.08.24
//#include	"msmuart.h"
#include "siorsreg.h"
#include	"IrDATool.h"
#include	"siors232.h"

#undef		TIMER_SETTING_ONESHOT	/*	Timer CB One Shot	*/
#undef		USE_TWO_TIMER			/*	USE CB25			*/

#ifdef	FEATURE_IRDA_UPGRADE
#define		USE_TWO_TIMER
#else	// #ifdef	FEATURE_IRDA_UPGRADE
#define		TIMER_SETTING_ONESHOT
#define		USE_TWO_TIMER
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#if defined(USE_MONITOR)
extern	volatile char	InMonitor;
static	long	ZeroCount;
static	long	SysTimer;
#endif
unsigned long	StdTimer;
unsigned long	StdTimer1;
int				TimeCount;
int				CBCount;

static	clk_cb_type              Msm_Irda_Timer_Cb_Item1;
#if defined(USE_TWO_TIMER)
static	clk_cb_type              Msm_Irda_Timer_Cb_Item25;
#endif
#if defined(USE_MONITOR)
static	clk_cb_type              Msm_Irda_Timer_Cb_Debug;
#endif	/*	USE_MONITOR	*/
static	clk_cb_type              Msm_Irda_Start_CB;


#ifdef	FEATURE_IRDA_UPGRADE

#if	MODEL_ID == MODEL_D1
//#define	MSM_TIMER_INTERVAL1		15  //5	//???20
//#define	MSM_TIMER_INTERVAL25	35  //25	//???50

static int4	MSM_TIMER_INTERVAL1	=	13;  //5	//???20
static int4	MSM_TIMER_INTERVAL25 =	40;  //25	//???50

#elif MODEL_ID == MODEL_A10
//#define	MSM_TIMER_INTERVAL1		15  //5	//???20
//#define	MSM_TIMER_INTERVAL25	35  //25	//???50

static int4	MSM_TIMER_INTERVAL1	=	13;  //5	//???20
static int4	MSM_TIMER_INTERVAL25 =	40;  //25	//???50
//youandi 01.05.22 IrDA 
#elif MODEL_ID == MODEL_B1  || (MODEL_ID == MODEL_B2)

static int4	MSM_TIMER_INTERVAL1	=	15;
static int4	MSM_TIMER_INTERVAL25 =	50;
//youandi 01.05.22 IrDA
#else

#define	MSM_TIMER_INTERVAL1		5
#define	MSM_TIMER_INTERVAL25	25

#endif // #if MODEL_ID == MODEL_D1

#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#define	MSM_TIMER_INTERVALDEBUG	20
#define	MSM_IRDA_STARTTIMER		5000

void	IrDA_Hw_Init( void);
void	IrDA_Start(void);
void	MsmTimerInit( void);
void	MsmTimerExit( void);
void	MsmTimerCb1(int4 dummy);
void	MsmTimerCb25(int4 dummy);
#if defined(USE_MONITOR)
void	MsmTimerCBDebug(int4 dummy);
#endif	/*	USE_MONITOR	*/
void	IrDA_CB_Start(int4 dummy);

extern	char ui_bIrDAExchangeMode;
void	MDM_GetSetting(int Prime,int Second);
/************************************************************************/
/*																		*/
/*	FUNCTION IrDA_Hw_Init												*/
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
void IrDA_Hw_Init( void)
{
	MsmUart1Init();
//	MsmUart2Init();
	MsmTimerInit();
}	/*	end of IrDA_Hw_Init	*/


/************************************************************************/
/*																		*/
/*	FUNCTION MsmTimerInit												*/
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
#ifndef	FEATURE_IRDA_UPGRADE
void IrExMsmTimerInit( void)
{
	StdTimer	=	0;
	StdTimer1	=	0;
	CBCount		=	0;
/*---------------------------------
   Define call back items.
  ---------------------------------*/

	clk_def( &Msm_Irda_Timer_Cb_Item1);
	clk_dereg( &Msm_Irda_Timer_Cb_Item1);
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
             MsmTimerCb1,
             (int4)MSM_TIMER_INTERVAL1,
             (int4)MSM_TIMER_INTERVAL1,
             TRUE
			);
}
#endif	// #ifndef	FEATURE_IRDA_UPGRADE

void MsmTimerInit( void)
{
#ifndef	FEATURE_IRDA_UPGRADE
// chul.add
	if(ui_bIrDAExchangeMode) 
	{
		IrExMsmTimerInit();
		return;
	}
// chul.end
#endif	// #ifndef	FEATURE_IRDA_UPGRADE

#if defined(USE_MONITOR)
	ZeroCount	= 	0;
	SysTimer	=	0;
#endif
	StdTimer	=	0;
	StdTimer1	=	0;
	CBCount		=	0;
/*---------------------------------
   Define call back items.
  ---------------------------------*/

	clk_def( &Msm_Irda_Timer_Cb_Item1);
#if defined(USE_TWO_TIMER)
	clk_def( &Msm_Irda_Timer_Cb_Item25);
#endif
#if defined(USE_MONITOR)
	clk_def( &Msm_Irda_Timer_Cb_Debug);
#endif

	clk_dereg( &Msm_Irda_Timer_Cb_Item1);
#if defined(USE_TWO_TIMER)
	clk_dereg( &Msm_Irda_Timer_Cb_Item25);
#endif
#if defined(USE_MONITOR)
	clk_dereg( &Msm_Irda_Timer_Cb_Debug);
#endif

#if !defined(TIMER_SETTING_ONESHOT)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer0CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb1,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL1,
             (int4)MSM_TIMER_INTERVAL1,
             TRUE
			);
#if defined(USE_TWO_TIMER)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item25,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer1CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb25,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL25,
             (int4)MSM_TIMER_INTERVAL25,
             TRUE
			);
#endif
#if defined(USE_MONITOR)
	clk_reg(
            &Msm_Irda_Timer_Cb_Debug,
             MsmTimerCBDebug,
             (int4)MSM_TIMER_INTERVALDEBUG,
             (int4)MSM_TIMER_INTERVALDEBUG,
             TRUE
			);
#endif
#else //#if !defined(TIMER_SETTING_ONESHOT)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer0CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb1,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL1,
             (int4)0,
             FALSE
			);
#if defined(USE_TWO_TIMER)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item25,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer1CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb25,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL25,
             (int4)0,
             FALSE
			);
#endif
#endif	/*	TIMER_SETTING_ONESHOT	*/
}	/*	end of MsmTimerInit	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmTimerExit												*/
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
void MsmTimerExit( void)
{
/*---------------------------------
   Define call back items.
  ---------------------------------*/
	clk_dereg( &Msm_Irda_Timer_Cb_Item1);
#if defined(USE_TWO_TIMER)
	clk_dereg( &Msm_Irda_Timer_Cb_Item25);
#endif
#if defined(USE_MONITOR)
	clk_dereg( &Msm_Irda_Timer_Cb_Debug);
#endif

}	/*	end of MsmTimerExit	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmTimerCb1												*/
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
void MsmTimerCb1( 
	 int4 dummy   /* dummy argument */
)
{
//h	StdTimer1 += MSM_TIMER_INTERVAL1; // used for test.
	IrdaTic0CallBack(MSM_TIMER_INTERVAL1);

#ifndef	FEATURE_IRDA_UPGRADE
// chul.add
	if(ui_bIrDAExchangeMode) 
	{
		CBCount++;
		if(CBCount==5)
		{
			CBCount=0;
	// erased by chos  MSMU_OUT( MSMU2_TF, 'T'); //99/6/10
	//		KICK_WATCHDOG();
			IrdaTic1CallBack();
		}
		return;
	}
// chul.end
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#if defined(TIMER_SETTING_ONESHOT)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer0CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb1,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL1,
             (int4)0,
             FALSE
			);
#endif	/*	TIMER_SETTING_ONESHOT	*/
}	/*	end of MsmTimerCb1	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmTimerCb25												*/
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
void MsmTimerCb25(
	 int4 dummy   /* dummy argument */
)
{
//	KICK_WATCHDOG();
// erased by chos	MSMU_OUT( MSMU2_TF, 'b');
//h	StdTimer += MSM_TIMER_INTERVAL25;
//h#if defined(USE_MONITOR)
//h	if(ZeroCount)
//h		ZeroCount--;
//h	SysTimer += MSM_TIMER_INTERVAL25;
//h#endif

	IrdaTic1CallBack();
#if defined(TIMER_SETTING_ONESHOT)
#if defined(USE_TWO_TIMER)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item25,
#ifdef	FEATURE_IRDA_UPGRADE
			 IrDA_Timer1CB,
#else	// #ifdef	FEATURE_IRDA_UPGRADE
             MsmTimerCb25,
#endif	// #ifdef	FEATURE_IRDA_UPGRADE
             (int4)MSM_TIMER_INTERVAL25,
             (int4)0,
             FALSE
			);
#endif
#endif	/*	TIMER_SETTING_ONESHOT	*/
}	/*	end of MsmTimerCb25	*/

/************************************************************************/
/*																		*/
/*	FUNCTION MsmTimerCBDebug											*/
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
extern	void	MsmAudIack(void);
extern	void	Debug_SendFrame(void);
//extern	void	MDM_Check(void);
void MsmTimerCBDebug(
	 int4 dummy   /* dummy argument */
)
{
	MsmAudIack();
//	MDM_Check();
//	Debug_SendFrame();
}	/*	end of MsmTimerCBDebug	*/
#endif	/*	USE_MONITOR	*/

void IrDA_Start(void){
	clk_def( &Msm_Irda_Start_CB);
	clk_dereg( &Msm_Irda_Start_CB);
	clk_reg(
            &Msm_Irda_Start_CB,
             IrDA_CB_Start,
             (int4)MSM_IRDA_STARTTIMER,
             0,
             FALSE
			);
}	/*	end of IrDA_Start	*/


void	IrDA_CB_Start(
	 int4 dummy   /* dummy argument */
){
//chos MsmUart2Init();
//	IrAPP_OpenStack();
//	MDM_InitProc();
	MDM_GetSetting(FALSE,TRUE); // Set to secondary
	ChangeIrDAMode ((boolean)1);
}

#endif	// #ifdef	FEATURE_USE_IRDA
