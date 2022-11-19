
#define		_MODULE_IRDA_TOOLS

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       T O O L S    f o r    I r D A

GENERAL DESCRIPTION
	Functions for IrDA support

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

  Copyright (c) 1999 by SK Teletech INCORPORATED. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE


when		who		what, where, why
--------	---		----------------------------------------------------------
00.05.18    chos    PC ���� Phone ���� data �ִ� �ӵ� ����
                     IrDA IF buffer �� full�� �� 500 mSec����
                     IrDA IFó�� �κп��� PC�� ���� data�� �ȹް� ����
00.05.18    chos    IrDA lib�� DS task ���� IF�� command ���� signal�� �ٲ�
00.04.21    chos    RAM share�� ���� IrDAInitVariable() �߰�
00.03.21    chos    IrDA Upgrade
00.03.18    chos    IrDA D1 porting
99.07.12	chos	Created.

------------------------------------------------------------------------------*/
#include "target.h"             /* Target specific definitions          */
#include "customer.h"           /* Customer Specific Definitions        */



#include	"task.h"
#include	"sio.h"
#include	"diag.h"
#include	"diagi.h"
#include	"dsi.h"
#include	"bio.h"
#include	"dsskt.h" // NKHA 2000.10.08
#if (MODEL_ID == MODEL_D1)
#include    "uintxti.h"
#include	"uinscrn.h"
#endif
#if (MODEL_ID == MODEL_A10)
#include    "uiutxti.h"
#include	"uixscrn.h"
#endif
#include	"IrDATool.h"


#if (MODEL_ID == MODEL_D1)
    #include	"uinstate.h"
#else
    #include	"uistate.h"
#endif
#include    "msg.h"
#include    "siors232.h"
#include    "Irsirddk.h"
#include    "Timer.h"
#include    "IrDA.h"
#if (MODEL_ID == MODEL_D1)
    #include "Sisi\DataManager\DataManager.h"
#else
    #include "PC_IF\DataManager\DataManager.h"       
#endif

#ifdef	FEATURE_USE_IRDA
#include    "ds.h"

/*===========================================================================

	global variables

===========================================================================*/
extern dword	uisleep;
short			IrDA_AutoOffTimeOutCounter = 0;

boolean	_bIsIrDAMode = FALSE;

// by chos 00.05.18
// PC ���� Phone ���� data �ִ� �ӵ� ����
int		nIrDACTSCount = 0;

/*===========================================================================

	global functions

===========================================================================*/
// tasik 00.08.09
void far ui_IrDAStatusCheck_set ( int4  interval);
void IrDA_DrawIrDAStatusLine(byte Status);
extern char	ui_bIrDAExchangeMode;

// SIO���� routine�� test�� ���� �̰����� ó���ϰ� ���߿� signal�� �ٲپ� ó��
//	�̰��� �־ ���� ������ �׳� ��
//	by chos 99.08.06
void	ChangeIrDAMode ( boolean bUseIrDA )
{
	if ( bUseIrDA != bIsIrDAMode () )
	{
		_bIsIrDAMode = bUseIrDA;

		StartStopIrDA ();

		// for MMI config    ---------------------->

		if ( bIsIrDAMode () )
		// enable IrDA
		{
		}
		else
		// disable IrDA
		{
			// Disable timer
			IrDA_AutoOffTimeOutCounter = 0;
			ui_IrDAStatusCheck_set (0);
#if (MODEL_ID == MODEL_D1)
			IrDA_DrawIrDAStatusLine(0);
#else
// erase by chos 00.04.21 �� routine�� �̻����� ����Ŵ
//			IrDA_DrawIrDAStatusLine(0);
#endif

			ui_bIrDAExchangeMode = 0;

            // chojw 01.01.08 - IrDA�� �̿��Ͽ� �����͸Ŵ����� ����ϴ� ���
            //                  IrDA�� ������ Sleep�� ���� �ʴ� ����
            resetBioSW9M();
		}

		// <----------------------    for MMI config
	}
}

// by chos 00.04.20
//	IrDA���� ��� �Ǵ� �������� ���⼭ �ʱ�ȭ
//	�ٸ� module�� RAM�� ���� �ϹǷ� IrDA start�� ���� �ʱ�ȭ �ʿ�
void	IrDAInitVariable ( void )
{
#if (MODEL_ID == MODEL_A10)
	extern byte	Load$$APP_SHARE_RAM$$Base[];
	extern byte	Image$$APP_SHARE_RAM$$Base[];
	extern byte	Image$$APP_SHARE_RAM$$Length[];
	extern byte	Image$$APP_SHARE_RAM$$ZI$$Base[];
	extern byte	Image$$APP_SHARE_RAM$$ZI$$Length[];
	dword	*pSrc;
	dword	*pDest;
	dword	*pStop;

	pStop = (dword*)( Image$$APP_SHARE_RAM$$Base +
		(dword)Image$$APP_SHARE_RAM$$Length );
	for ( pSrc = (dword*)Load$$APP_SHARE_RAM$$Base,
			pDest = (dword*)Image$$APP_SHARE_RAM$$Base;
		pDest < pStop;
		pSrc ++, pDest ++ ) 
	{
		*pDest = *pSrc;
	}

	pStop = (dword*)( Image$$APP_SHARE_RAM$$ZI$$Base +
		(dword)Image$$APP_SHARE_RAM$$ZI$$Length );
	for ( pDest = (dword*)Image$$APP_SHARE_RAM$$ZI$$Base; 
		pDest < pStop;
		pDest ++ ) 
	{
		*pDest = 0;
	}
#endif //#if (MODEL_ID == MODEL_A10)
}


/*****************************************************************************
 *
 *		set MODEM line status
 *
 *				by chos 99.08.16
 *
 *****************************************************************************/

#include	<commctrl.h>
#include	<Ircomm.h>

void	IrDA_SendLineStatus ( void )
{
	extern int	MDM_Slsap;
	byte	szBuffer[ 3 ];
	byte	nLineStatus = 0;
	byte	nDelta;

	if ( MDM_DCEMode )		// DCE mode
	{
		if ( MDM_CTS )			// set CTS
			nLineStatus |= IRCOMM_CTS;
		if ( MDM_DSR )			// set DSR
			nLineStatus |= IRCOMM_DSR;
		if ( MDM_RI )			// set RI
			nLineStatus |= IRCOMM_RI;
		if ( MDM_CD )			// set CD
			nLineStatus |= IRCOMM_CD;

		if ( MDM_DCELine != nLineStatus ) 
		{
			szBuffer[ 0 ] = 0x21;	// DCE line setting
			szBuffer[ 1 ] = 1;
			szBuffer[ 2 ] = nLineStatus;

			// Set Delta Line Status
			nDelta = MDM_DCELine ^ nLineStatus;
			if ( nDelta & IRCOMM_CTS )
				szBuffer[ 2 ] |= IRCOMM_DLT_CTS;
			if ( nDelta & IRCOMM_DSR )
				szBuffer[ 2 ] |= IRCOMM_DLT_DSR;
			if ( nDelta & IRCOMM_RI )
				szBuffer[ 2 ] |= IRCOMM_DLT_RI;
			if ( nDelta & IRCOMM_CD )
				szBuffer[ 2 ] |= IRCOMM_DLT_CD;

			if ( IrCOMM_ControlReq ( MDM_Slsap, szBuffer, 3 ) >= 3 )
				MDM_DCELine = nLineStatus;
		}
	}
	else					// DTE mode
	{
		if ( MDM_RTS )			// set RTS
			nLineStatus |= IRCOMM_RTS;
		if ( MDM_DTR )			// set DTR
			nLineStatus |= IRCOMM_DTR;

		if ( MDM_DTELine != nLineStatus ) 
		{
			szBuffer[ 0 ] = 0x20;	// DCE line setting
			szBuffer[ 1 ] = 1;
			szBuffer[ 2 ] = nLineStatus;

			// Set Delta Line Status
			nDelta = MDM_DTELine ^ nLineStatus;
			if ( nDelta & IRCOMM_RTS )
				szBuffer[ 2 ] |= IRCOMM_DLT_RTS;
			if ( nDelta & IRCOMM_DTR )
				szBuffer[ 2 ] |= IRCOMM_DLT_DTR;

			if ( IrCOMM_ControlReq ( MDM_Slsap, szBuffer, 3 ) >= 3 )
				MDM_DTELine = nLineStatus;
		}
	}
}


void	IrDA_SetCTS ( boolean bOnOff )
{
	MDM_CTS = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}

void	IrDA_SetDSR ( boolean bOnOff )
{
	MDM_DSR = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}

void	IrDA_SetRI ( boolean bOnOff )
{
	MDM_RI = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}

void	IrDA_SetCD ( boolean bOnOff )
{
	MDM_CD = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}


void	IrDA_SetRTS ( boolean bOnOff )
{
	MDM_RTS = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}

void	IrDA_SetDTR ( boolean bOnOff )
{
	MDM_DTR = bOnOff ? ON : OFF;

	IrDA_SendLineStatus ();
}


/*****************************************************************
 *
 *		IrDA ���� timer CB�� IRQ ISR�� ���� �ʰ� 
 *		  DS task���� ó�� �ϰ� ����
 *
 *				by chos 99.08.23
 *
 *****************************************************************/


#ifdef	FEATURE_IRDA_UPGRADE

// by chos 00.05.18
//	IrDA lib�� DS task ���� IF�� command ���� signal�� �ٲ�

volatile dword	nIrDACommandSignal = 0;

void	SetIrDACommandSignal ( dword nSignal )
{
	INTLOCK ();

	nIrDACommandSignal |= nSignal;

	INTFREE ();
}

void	ClearIrDACommandSignal ( dword nSignal )
{
	INTLOCK ();

	nIrDACommandSignal &= ~ nSignal;

	INTFREE ();
}

void	SetIrDACommandSignalToDS ( dword nSignal )
{
	SetIrDACommandSignal ( nSignal );

	rex_set_sigs ( &ds_tcb, DS_IRDA_SIG );
}

#endif	//#ifdef	FEATURE_IRDA_UPGRADE


void	IrDA_Timer0CB ( int4 ms_interval )
{
#ifdef	FEATURE_IRDA_UPGRADE
	if ( ! APP_Primary )
		SetIrDACommandSignalToDS ( DS_IRDA_TIMER_0_TIMEOUT_CMD_SIG );
	else
		MsmTimerCb1 ( 0 );
#else
	ds_cmd_type		ds_cmd_item;

	ds_cmd_item.cmd_name = DS_IRDA_TIMER_0_TIMEOUT_CMD;
	if ( ! ds_cmd ( &ds_cmd_item ) )
	{
		MsmTimerCb1 ( 0 );
	}
#endif	//#ifdef	FEATURE_IRDA_UPGRADE

	SirBusyTimer += ms_interval;
	SirTicTimer  += ms_interval;
}

void	IrDA_Timer1CB ( int4 ms_interval )
{
// by chos 00.05.18
// PC ���� Phone ���� data �ִ� �ӵ� ����
	if ( ( nIrDACTSCount > 0 ) && ( ( -- nIrDACTSCount ) == 0 ) )
	{
		ds_set_inbound_flow ( DS_FLOW_IRDA, TRUE );
	}

#ifdef	FEATURE_IRDA_UPGRADE
	if ( ! APP_Primary )
		SetIrDACommandSignalToDS ( DS_IRDA_TIMER_1_TIMEOUT_CMD_SIG );
	else
		MsmTimerCb25 ( 0 );
#else
	ds_cmd_type		ds_cmd_item;

	ds_cmd_item.cmd_name = DS_IRDA_TIMER_1_TIMEOUT_CMD;
	if ( ! ds_cmd ( &ds_cmd_item ) )
	{
		MsmTimerCb25 ( 0 );
	}
#endif	//#ifdef	FEATURE_IRDA_UPGRADE
}

void	IrDA_ProtocolCB ( void )
{
#ifdef	FEATURE_IRDA_UPGRADE
	if ( ! APP_Primary )
		SetIrDACommandSignalToDS ( DS_IRDA_PROTOCOL_CMD_SIG );
	else
		IrDA_ProtocolCallBack() ;
#else
	ds_cmd_type		ds_cmd_item;

	ds_cmd_item.cmd_name = DS_IRDA_PROTOCOL_CMD;
	if ( ! ds_cmd ( &ds_cmd_item ) )
	{
		IrdaRxEOI ();
	}
#endif	//#ifdef	FEATURE_IRDA_UPGRADE
}

void	IrDA_GetMoreRx ( void )
{
#ifdef	FEATURE_IRDA_UPGRADE
//	if ( ! APP_Primary )
	if ( ! ui_bIrDAExchangeMode )
		SetIrDACommandSignalToDS ( DS_IRDA_GET_MORE_RX_CMD_SIG );
	else
		IrDA_rx_Process ();
#else
	ds_cmd_type		ds_cmd_item;

	ds_cmd_item.cmd_name = DS_IRDA_GET_MORE_RX_CMD;
	if ( ! ds_cmd ( &ds_cmd_item ) )
	{
		IrDA_rx_isr ( 0 , 0 );
	}
#endif	//#ifdef	FEATURE_IRDA_UPGRADE
}

void		irda_errorInd( int hndl, byte IrDAFAR *ptr, uint size )
{
	MSG_HIGH ( "IrDA err : %x %x %x", hndl, ptr, size );
}


/*
INPUT
	Status
		0 : Disable status display
		1 : Display not linked icon
		2 : Display linked icon
*/
void IrDA_DrawIrDAStatusLine(byte Status)
{
	if(ui.call)
//	if(FALSE)
	{
//		uiscrn_deact(UI_MODEM_F);  
	}
	else
	{
//		void	displayDataBanner(boolean);
//		boolean	check_dtr(void);

		switch(Status)
		{
		case 0:
		default:

//			uiscrn_deact(UI_MODEM_F);
			displayDataBanner(check_dtr());		
			break;
		case 1:
			uiscrn_bind_act(UI_MODEM_F, "      \x88\xb9        ");
			break;
		case 2:
			uiscrn_bind_act(UI_MODEM_F, "     \x88\xba\x88\xbb\x88\xbc     ");
			break;
		}
	}
	uiscrn_refresh();
}
/*
boolean check_dtr( void);
boolean				IrDA_bIrDAPortEnabled(void);
extern const char	*IrDA_Phone2PCIcon;


char *IrDA_GetBannerText(char *pText)
{
	static char BannerTxt[UI_WIDE+1];

	if(_bIsIrDAMode)
	{
		strcpy(BannerTxt, IrDA_Phone2PCIcon);
		strcat(BannerTxt, pText);
		// Reverse character display attribute
		BannerTxt[UI_WIDE-1] = '1';
		BannerTxt[UI_WIDE] = '\0';
	}
	else
	{
		return NULL;
		strcpy(BannerTxt, IrDA_Phone2PCIcon);
		strcat(BannerTxt, IrDA_Phone2PCIcon);
		// Reverse character display attribute
		BannerTxt[UI_WIDE-1] = '1';
		BannerTxt[UI_WIDE] = '\0';
	}
	return BannerTxt;
}
*/
#endif	// #ifdef	FEATURE_USE_IRDA
