#ifndef	_IrDATOOL_H
#define	_IrDATOOL_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       H E A D E R    f o r    I r D A T O O L . C

GENERAL DESCRIPTION
	Functions for IrDA support

  Copyright (c) 1999 by SK Teletech INCORPORATED. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE


when		who		what, where, why
--------	---		----------------------------------------------------------
00.03.21    chos    IrDA Upgrade
00.03.18    chos    IrDA D1 porting
99.07.12	chos	Created.

------------------------------------------------------------------------------*/


/*===========================================================================

	define global constants & functions

===========================================================================*/

#include	"target.h"
#include	"comdef.h"
#if (MODEL_ID != MODEL_B1)  && (MODEL_ID != MODEL_B2)
#include	"uiCommon.h"
#endif

#ifdef	FEATURE_USE_IRDA


// predefine global variables

extern boolean	_bIsIrDAMode;
extern int		nIrDACTSCount;


// predefine global function prototypes

#define	bIsIrDAMode()				_bIsIrDAMode


void	ChangeIrDAMode ( boolean bUseIrDA );
void	IrDAInitVariable ( void );

// set MODEM line status
//	DTE mode
void	IrDA_SetCTS ( boolean bOnOff );
void	IrDA_SetDSR ( boolean bOnOff );
void	IrDA_SetCD ( boolean bOnOff );
void	IrDA_SetRI ( boolean bOnOff );

//	DTE mode
void	IrDA_SetRTS ( boolean bOnOff );
void	IrDA_SetDTR ( boolean bOnOff );

// IrDA -> DS task comand signal control
extern volatile dword	nIrDACommandSignal;
void	SetIrDACommandSignal ( dword nSignal );
void	ClearIrDACommandSignal ( dword nSignal );
void	SetIrDACommandSignalToDS ( dword nSignal );

// IrDA timer CB
void	IrDA_Timer0CB ( int4 ms_interval );
void	IrDA_Timer1CB ( int4 ms_interval );

// IrDA etc
void	IrDA_ProtocolCB ( void );
void	IrDA_GetMoreRx ( void );


// functions in irModem.c
void	MDM_InitProc ( void );
void	MDM_ExitProc ( void );


// global functions in MSMTimer.c
void	MsmTimerCb1 ( int4 dummy );
void	MsmTimerCb25 ( int4 dummy );

// global variables in IrModem.c
extern byte		APP_Primary;
extern byte		APP_Secondary;

extern int		MDM_Slsap;

extern byte		MDM_DCEMode;

extern byte		MDM_DTELine;
extern byte		MDM_DCELine;

extern byte		MDM_RTS;
extern byte		MDM_DTR;

extern byte		MDM_CTS;
extern byte		MDM_DSR;
extern byte		MDM_CD;
extern byte		MDM_RI;


#endif	// #ifdef	FEATURE_USE_IRDA

#endif	// #ifndef	_IrDATOOL_H
