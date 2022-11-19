/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1998 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irhwif(H8SŒü‚¯)                              */
/*             Created:    7/28/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

#include	<string.h>
#include	"irdaconf.h"
#include	<irda.h>
#include	<irsir.h>

#include	"ldevice.h"
#include	"irsirddk.h"
#include	"console.h"

#if defined(MMI_TREX)
#include	"chul\irdaex\IrDAEx.h"
#elif (MMI_TYPE == MMI_A10)
#include    "irda\IrDAEx.h"
#endif
//#include	"IrDAEx.h"
#include	"ds.h"
#include	"IrDATool.h"
//char ui_bIrDAExchangeMode = 0; //irdaui temp

#define	TMR1_MASK	0x3

#define	USE_MINTURN

#if (MODEL_ID == MODEL_B1)  || (MODEL_ID == MODEL_B2)
extern boolean fIrDAExchangeMode;
#endif

static	PFCB	pFunc ;

byte	achRecvBuf[IrDARECVBUFSZ];
byte	achSendBuf[IrDASENDBUFSZ];

volatile void		(IrDACB *SirDataFunc)( void ) ;
static	dword		MinTimer;
static	int			TblCnt;
//static	volatile	char	InCallBackProc;
static	volatile	char	InCallBackProc1;
static	volatile	char	InCallBackProc2;


void	IrDAAPI	IrSIR_SetFrameMode( void )
{
	IrSIR_SetIrDAMode() ;
}

void	IrDAAPI	IrSIR_SetSpeed( dword spd )
{
	IrEyeSetBaud(spd);
}

void	IrDAAPI	IrSIR_SetRecvMode( void )
{
	IrdaRxIntOn();
}

void	SIRSetRxIntr( void )
{
	IrdaIntOff();
	SIRKickRx();
	IrdaRxIntOn();
}

void	SIRSetTxIntr( void )
{
	IrdaIntOff();
	SIRKickTx();
	IrdaTxIntOn();
}

void	SIRSetNoIntr(void)
{
	IrdaIntOff();
}

void IrdaRxCallBack(int data)
{
	SirBusyTimer = 0; // Áö±Ý ¼ö½ÅÀ» ÇÑ´Ù. 	tasik 01.04.11
	(*SirRxFunc)(data);
}


void IrdaRxEOI(void)
{
#if !defined(USE_SYSTEM_TASKCALLBACK)	/*	Modify OSW:Takagaki 2000/01/21	*/
	PFCB	pFunc;
#endif

	if( !SirDataFunc )
		return ;

	if(InCallBackProc1)
	{// phone-to-pcÀÇ °æ¿ì¿¡´Â ds task¿¡¼­ Ã³¸®ÇÏµµ·Ï ÇÑ´Ù.
		SetIrDACommandSignalToDS ( DS_IRDA_PROTOCOL_CMD_SIG );
		return ;
	}
	//ÀÏ´ÜÀº phone-to-pc·Î °£ÁÖÇÑ´Ù.
	InCallBackProc1 = 1;          
	
#if defined(USE_SYSTEM_TASKCALLBACK)	/*	Modify OSW:Takagaki 2000/01/21	*/

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
	if(!fIrDAExchangeMode)
#else
	if(!ui_bIrDAExchangeMode)
#endif
	{// Phone-to-PC°æ¿ì 
		// tasik 01.04.11
		//pFunc´Â °¢Á¾ ÇÔ¼ö¸¦ ´ëº¯ÇÏ´Â Æ÷ÀÎÅÍÀÌ´Ù.
		//¿©±â¼­ irda protocol callbackÀÌ¶õ ÀÇ¹Ì´Â 
		// °á±¹ °¡Àå ³·Àº ÇÁ·ÎÅäÄÝ ÃþÀÎ sirÃþ¿¡ 
		// ÇÔ¼öÆ÷ÀÎÅÍ¸¦ µÒÀ¸·Î½á °á±¹ ÀüÃ¼ ÇÁ·ÎÅäÄÝ ÃþÀÌ 
		// Â÷·Ê·Î ¸ðµÎ ½ÇÇàµÇµµ·Ï ÇÑ´Ù´Â ÀÇ¹ÌÀÌ´Ù. 
		pFunc = (PFCB)SirDataFunc;

		// irda ¾Ë°í¸®µëÀº ÇöÀçÀÇ ÇÔ¼ö Æ÷ÀÎÅÍ°¡ °¡¸®Å°´Â °÷(pFunc)À» 
		// ½ÇÇàÇÏµµ·Ï µÇ¾î ÀÖÀ¸¹Ç·Î ÀÏ´Ü ÇÔ¼ö Æ÷ÀÎÅÍ¿¡
		// °ªÀÌ ÇÒ´çµÇ¸é ÀÌ¹Ì ÇÒ´çµÈ ÇÔ¼ö Æ÷ÀÎÅÍ(¿¹: SirDataFunc)´Â null·Î ¸¸µé¾î µÎ¾î¾ß 
		// ÀüÃ¼ ÇÁ·ÎÅäÄÝ ÃþÀº ÇÏ³ªÀÇ Æ÷ÀÎÅÍ(pFunc)¿¡ ÀÇÇØ¼­ ½ÇÇàÀÌ µÉ ¼ö ÀÖ´Ù.
		SirDataFunc = 0;

		// À§¿¡¼± °á±¹ ½ÇÇàÇÒ ÇÔ¼ö¸¸ Á¤ÇÑ °ÍÀÌ´Ù.
        // µû¶ó¼­ ´ÙÀ½¿£ ½ÇÁ¦·Î ½ÇÇàÀ» ÇØ¾ßÇÏ¹Ç·Î 
		// ÇÁ·ÎÅäÄÝ callbackÀ» ÇÏ´Â °ÍÀÌ´Ù.       
		IrDA_ProtocolCB() ;
	}
	else
#endif
	{//phone-to-phoneÀÇ °æ¿ì
		// ÇöÀç µ¥ÀÌÅÍ°¡ ÀÖ´Ù´Â indicationÀ» ÇÏ¿´´Ù¸é,
		if( SirDataFunc ) {
			pFunc = (PFCB)SirDataFunc;
			SirDataFunc = 0;
			/*	Interrup En	*/
			// ÇÁ·ÎÅäÄÝÀ» ½ÇÇàÇÏ¶ó.
			(*pFunc)();
		}

		// phone-to-phoneÀ¸·Î °£ÁÖÇÑ´Ù.
		InCallBackProc1 = 0;
	}
}


int IrdaTxCallBack(void)
{
	SirBusyTimer = 0; //Áö±Ý ¼Û½ÅÀ» ÇÑ´Ù.
	return	(*SirTxFunc)() ;
}


void IrdaTxEOI( void )
{
	IrdaRxEOI() ;
}

unsigned long tictimerread(void)
{
	return	SirTicTimer;

}

void IrdaTic0CallBack(int interval)
{
	IrdaRxEOI();
// modified by chos 00.05.16
//	move to IrDATool.c IrDA_Timer0CB ()
/*
	SirBusyTimer += interval;
	SirTicTimer  += interval;
*/
	if(MinTimer){
		if(MinTimer <= SirTicTimer){ // 10ms turnaround timeÀÌ Áö³µ´Ù.
			MinTimer = 0;
			SIRSetTxIntr(); // ¼Û½ÅÇÑ´Ù.
		}
	}
}


void IrdaTic1CallBack(void)
{
	PFCB	pFunc ;

	if( !SirTimerFunc[TblCnt] || SirTimerSem[TblCnt] )
		goto EndTbl ;

	if( SirTimerCount[TblCnt] <= SirTicTimer ) {
		if(InCallBackProc2)
			return ;
		InCallBackProc2 = 1;

		/*----------  modified by nagamura@osw.co.jp 1997.11.03	    */
		/*ƒ^ƒCƒ}‚Ì‘¶Ý”»’è‚©‚çInCallBackProc‚ÌŒŸ¸‚Ü‚Å‚ÌŠÔ‚ÉƒvƒƒgƒR*/
		/*ƒ‹‚ª‘–‚èAƒ^ƒCƒ}‚ðƒLƒƒƒ“ƒZƒ‹‚³‚ê‚é‰Â”\«‚ª‚ ‚éˆ×‚ÉAƒnƒ“ƒh*/
		/*ƒ‹‚Ì‘¶Ý‚Æƒ^ƒCƒ€ƒAƒEƒgiƒLƒƒƒ“ƒZƒ‹AÄƒŠƒNƒGƒXƒg‚ª–³‚¢Ž–‚Ì*/
		/*Šm”Fj‚ðÄ“xŒŸ¸‚·‚éB				    */
		if( SirTimerFunc[TblCnt]  &&
			SirTimerCount[TblCnt] <= SirTicTimer ) {
             // ½ÇÇàÇÒ ÇÔ¼ö°¡ ÀÖÀ¸¸ç StartTimer() (= IrSIR_TimerReq())¿¡ ÀÇÇØ¼­ ¼³Á¤µÈ
			// ½Ã°£ÀÌ Áö³µ´Ù.
			pFunc = (PFCB)SirTimerFunc[TblCnt];
			SirTimerFunc[TblCnt] = 0;
/*	Intrrupt En	*/
			(*pFunc)();
		}
		InCallBackProc2	= 0 ;
	}


EndTbl:
	TblCnt = (TblCnt + 1) & TBL_MASK ;
}


void	IrDAAPI	IrSIR_SendFrame( uint size, void IrDAFAR *data)
{
	// tasik 01.04.11
	// ÇÁ·¹ÀÓÀ» ¼Û½ÅÇÏ´Â ÇÔ¼öÀÌ´Ù. ±×·¯³ª ÁÖÀÇÇÒ °ÍÀÌ ÀÖ´Ù.
	// SIRÀÇ ÀÔÀå¿¡¼± raw data¸¸ º¸³»´Â °ÍÀÌ ¾Æ´Ï¶ó.
	// (IrSIR_SetBOF()°¡ ÁöÁ¤ÇÏ´Â ¼ö +1)ÀÇ BOF(Beginning of Frame)¸¦ º¸³»°í ³ª¼­
	// µ¥ÀÌÅÍ ÇÁ·¹ÀÓÀ» º¸³»°í ±×´ÙÀ½ 2¹ÙÀÌÆ®ÀÌ crc¸¦ º¸³»°í ±×´ÙÀ½ EOF(End of Frame)À»
	// º¸³½´Ù. 
	// ¿©±â¼± ½ÇÁ¦ÀÇ µ¥ÀÌÅÍ¸¸ º¸³»´Â ÇÔ¼öÀÌ´Ù.
    // ¶ÇÇÑ, ÇÑ°¡Áö ´õ ÁÖÀÇ ÇÒ °ÍÀÌ ÀÖ´Ù.
	// ¸¶Áö¸·À¸·Î µ¥ÀÌÅÍ¸¦ ¼ö½ÅÇÏ°í ³ª¼­ IrSIR_SetMinimumTurnaroundTime()¿¡¼­ 
	// ¼³Á¤ÇÏ´Â ½Ã°£(10ms)ÀÌ»óÀ» °æ°úÇÑ ´ÙÀ½, ¼Û½ÅÀ» ½ÃÀÛÇØ¾ß ÇÑ´Ù.
	SirSendBuffer		= (byte*)data;
	SirSendBufferEnd	= SirSendBuffer + size;
	if(SirMinTATime){//sir minimum turn-around time = 10ÀÌ´Ù.
		SIRSetNoIntr(); // ¼Û½ÅÇÏÁö ¾Ê´Â´Ù.
		SirDataFunc	= 0 ;
		MinTimer = SirTicTimer + SirMinTATime;
	}else{
		SIRSetTxIntr(); // ¼Û½ÅÇÑ´Ù.
	}
}

void	IrSIR_HwInit(void)
{
/*	SIRSetNoIntr();*/
	SirDataFunc		= 0;
	SirMinTATime	= 0;
	MinTimer		= 0;
//	InCallBackProc	= 0;
	InCallBackProc1	= 0; //phone-to-phoneÀ¸·Î Ã³À½ ¼ÂÆÃÇÑ´Ù. ÀÌ º¯¼ö´Â phone-to-pcÀÌ¸é 1·Î µÈ´Ù.
	InCallBackProc2	= 0;
	TblCnt			= 0;
	SirTicTimer		= 0;
	memset(SirTimerCount,0,sizeof(SirTimerCount));
	memset(SirTimerFunc,0,sizeof(SirTimerFunc));
	memset((void *)SirTimerSem, 0, sizeof( SirTimerSem )) ;
	SIRSetRxIdle();
	SIRSetTxIdle();
/*	IrSIR_SetSpeed( (dword)9600 ) ;*/	/* Set Speed */
}

#if defined(USE_MONITOR)
void	SetInMonitor(char c);
#endif

int	IrDAAPI	IrSIR_Open(void)
{
	IrSIR_HwInit();
	MsmUart1Init();
	MsmTimerInit();
#ifdef	USE_MONITOR
	SetInMonitor(0);
#endif
	return	0;
}

void	IrDAAPI	IrSIR_Close( void )
{
	IrSIR_HwInit();
#ifdef	USE_MONITOR
	SetInMonitor(1);
#endif
	IrdaRxIntOn();
}

static	const byte	capability[]	= IrDACAPABILITY ;

uint	IrDAAPI	IrSIR_GetCapability(const void IrDAFAR * IrDAFAR *p )
{
	*p	= (const void IrDAFAR* IrDAFAR*)capability ;
	return( sizeof(capability) );
}

void	IrDAAPI	IrSIR_InitConnectionParameters( void )
{
}

/*
	Power Management Mode
*/

IrDABOOL IrDAAPI	IrDA_GetPowerMode( void ){	return 1;	}
void	IrDAAPI	IrDA_PowerOn( void ){	}
void	IrDAAPI	IrDA_PowerOff( void ){	}
void	IrDAAPI	IrDA_SetPowerLevel( uint level ){	}
uint	IrDAAPI	IrDA_GetPowerLevel( void ){	return 1;	}

void	IrDA_ProtocolCallBack( void )
{
	(*pFunc)();
	InCallBackProc1 = 0;
}
