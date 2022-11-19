/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1998 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irhwif(H8S����)                              */
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
	SirBusyTimer = 0; // ���� ������ �Ѵ�. 	tasik 01.04.11
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
	{// phone-to-pc�� ��쿡�� ds task���� ó���ϵ��� �Ѵ�.
		SetIrDACommandSignalToDS ( DS_IRDA_PROTOCOL_CMD_SIG );
		return ;
	}
	//�ϴ��� phone-to-pc�� �����Ѵ�.
	InCallBackProc1 = 1;          
	
#if defined(USE_SYSTEM_TASKCALLBACK)	/*	Modify OSW:Takagaki 2000/01/21	*/

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
	if(!fIrDAExchangeMode)
#else
	if(!ui_bIrDAExchangeMode)
#endif
	{// Phone-to-PC��� 
		// tasik 01.04.11
		//pFunc�� ���� �Լ��� �뺯�ϴ� �������̴�.
		//���⼭ irda protocol callback�̶� �ǹ̴� 
		// �ᱹ ���� ���� �������� ���� sir���� 
		// �Լ������͸� �����ν� �ᱹ ��ü �������� ���� 
		// ���ʷ� ��� ����ǵ��� �Ѵٴ� �ǹ��̴�. 
		pFunc = (PFCB)SirDataFunc;

		// irda �˰����� ������ �Լ� �����Ͱ� ����Ű�� ��(pFunc)�� 
		// �����ϵ��� �Ǿ� �����Ƿ� �ϴ� �Լ� �����Ϳ�
		// ���� �Ҵ�Ǹ� �̹� �Ҵ�� �Լ� ������(��: SirDataFunc)�� null�� ����� �ξ�� 
		// ��ü �������� ���� �ϳ��� ������(pFunc)�� ���ؼ� ������ �� �� �ִ�.
		SirDataFunc = 0;

		// ������ �ᱹ ������ �Լ��� ���� ���̴�.
        // ���� ������ ������ ������ �ؾ��ϹǷ� 
		// �������� callback�� �ϴ� ���̴�.       
		IrDA_ProtocolCB() ;
	}
	else
#endif
	{//phone-to-phone�� ���
		// ���� �����Ͱ� �ִٴ� indication�� �Ͽ��ٸ�,
		if( SirDataFunc ) {
			pFunc = (PFCB)SirDataFunc;
			SirDataFunc = 0;
			/*	Interrup En	*/
			// ���������� �����϶�.
			(*pFunc)();
		}

		// phone-to-phone���� �����Ѵ�.
		InCallBackProc1 = 0;
	}
}


int IrdaTxCallBack(void)
{
	SirBusyTimer = 0; //���� �۽��� �Ѵ�.
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
		if(MinTimer <= SirTicTimer){ // 10ms turnaround time�� ������.
			MinTimer = 0;
			SIRSetTxIntr(); // �۽��Ѵ�.
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
		/*�^�C�}�̑��ݔ��肩��InCallBackProc�̌����܂ł̊ԂɃv���g�R*/
		/*��������A�^�C�}���L�����Z�������\��������ׂɁA�n���h*/
		/*���̑��݂ƃ^�C���A�E�g�i�L�����Z���A�ă��N�G�X�g����������*/
		/*�m�F�j���ēx��������B				    */
		if( SirTimerFunc[TblCnt]  &&
			SirTimerCount[TblCnt] <= SirTicTimer ) {
             // ������ �Լ��� ������ StartTimer() (= IrSIR_TimerReq())�� ���ؼ� ������
			// �ð��� ������.
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
	// �������� �۽��ϴ� �Լ��̴�. �׷��� ������ ���� �ִ�.
	// SIR�� ���忡�� raw data�� ������ ���� �ƴ϶�.
	// (IrSIR_SetBOF()�� �����ϴ� �� +1)�� BOF(Beginning of Frame)�� ������ ����
	// ������ �������� ������ �״��� 2����Ʈ�� crc�� ������ �״��� EOF(End of Frame)��
	// ������. 
	// ���⼱ ������ �����͸� ������ �Լ��̴�.
    // ����, �Ѱ��� �� ���� �� ���� �ִ�.
	// ���������� �����͸� �����ϰ� ���� IrSIR_SetMinimumTurnaroundTime()���� 
	// �����ϴ� �ð�(10ms)�̻��� ����� ����, �۽��� �����ؾ� �Ѵ�.
	SirSendBuffer		= (byte*)data;
	SirSendBufferEnd	= SirSendBuffer + size;
	if(SirMinTATime){//sir minimum turn-around time = 10�̴�.
		SIRSetNoIntr(); // �۽����� �ʴ´�.
		SirDataFunc	= 0 ;
		MinTimer = SirTicTimer + SirMinTATime;
	}else{
		SIRSetTxIntr(); // �۽��Ѵ�.
	}
}

void	IrSIR_HwInit(void)
{
/*	SIRSetNoIntr();*/
	SirDataFunc		= 0;
	SirMinTATime	= 0;
	MinTimer		= 0;
//	InCallBackProc	= 0;
	InCallBackProc1	= 0; //phone-to-phone���� ó�� �����Ѵ�. �� ������ phone-to-pc�̸� 1�� �ȴ�.
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
