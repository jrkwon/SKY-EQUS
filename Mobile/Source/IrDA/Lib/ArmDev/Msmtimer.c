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

#include	"boothw.h"

#include	"irdaconf.h"
#include	"irsirddk.h"
#include	"ldevice.h"
#include	"msmuart.h"


#undef		TIMER_SETTING_ONESHOT	/*	Timer CB One Shot	*/
#undef		USE_TWO_TIMER			/*	USE CB25			*/

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


#define	MSM_TIMER_INTERVAL1		5
#define	MSM_TIMER_INTERVAL25	25
#define	MSM_TIMER_INTERVALDEBUG	20
#define	MSM_IRDA_STARTTIMER		5000

void	IrDA_Hw_Init( void);
void	IrDA_Start(void);
void	MsmTimerInit( void);
void	MsmTimerCb1(int4 dummy);
void	MsmTimerCb25(int4 dummy);
#if defined(USE_MONITOR)
void	MsmTimerCBDebug(int4 dummy);
#endif	/*	USE_MONITOR	*/
void	IrDA_CB_Start(int4 dummy);

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
void MsmTimerInit( void)
{
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
             MsmTimerCb1,
             (int4)MSM_TIMER_INTERVAL1,
             (int4)MSM_TIMER_INTERVAL1,
             TRUE
			);
#if defined(USE_TWO_TIMER)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item25,
             MsmTimerCb25,
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
#else
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
             MsmTimerCb1,
             (int4)MSM_TIMER_INTERVAL1,
             (int4)0,
             FALSE
			);
#if defined(USE_TWO_TIMER)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item25,
             MsmTimerCb25,
             (int4)MSM_TIMER_INTERVAL25,
             (int4)0,
             FALSE
			);
#endif
#endif	/*	TIMER_SETTING_ONESHOT	*/
}	/*	end of MsmTimerInit	*/

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
	CBCount++;
	if(CBCount==5)
	{
		CBCount=0;
		MSMU_OUT( MSMU2_TF, 'T'); //99/6/10
//		KICK_WATCHDOG();
		IrdaTic1CallBack();
	}
#if defined(TIMER_SETTING_ONESHOT)
	clk_reg(
            &Msm_Irda_Timer_Cb_Item1,
             MsmTimerCb1,
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
	MSMU_OUT( MSMU2_TF, 'b');
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
             MsmTimerCb25,
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
MsmUart2Init();
//	IrAPP_OpenStack();
	MDM_InitProc();
}