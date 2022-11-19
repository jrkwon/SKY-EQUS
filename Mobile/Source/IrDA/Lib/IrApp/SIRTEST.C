/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       sirtest.c                                    */
/*             Created:    8/6/96                                       */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/
/*=======================================**
                  main.c                  
        Copyright (c) 1996 by core        
**=======================================*/

/* $Header$ */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	"irdaconf.h"
#include	"irlmp.h"
#include	"irsir.h"
#include	"console.h"
#include	"ldevice.h"

extern	byte	achSendBuf[],achRecvBuf[];

static	IrDABOOL	Initialize( void ) ;
static	void	Terminate( void ) ;
static	void	OpenStack( void ) ;
static	void	SetupPortConfig( void ) ;
static	IrDABOOL	BindLSAP( void ) ;
static	void	UnbindLSAP( void ) ;

static	int		IrDACB ConnectInd( 	int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB DataInd( 	int lsap,const void IrDAFAR *buf, uint size );

static	ulong	MaxDeviceSpeed ;
static	uint	LinkDisconnectTime ;
static	int	TargetLSAP ;
static	uint	Mode ;

#define	MD_NeedIRsp	0x0001

void	GenoaClient(void)
{
	if( !Initialize()) {
#ifdef	USE_MONITOR
#if 0
		cputs( "**** SIR Genoa Test Mode ****\n") ;
		cputs( "\nstart ...\n" ) ;
#endif
#endif
		for(;;){
#if 0
			if(ConsGetc() == 0x1b)
				break;
#endif
			if( Mode & MD_NeedIRsp ) {
				Mode	&= ~MD_NeedIRsp ;
				IrLMP_DataReq( TargetLSAP, "OK", 2 ) ;
			}
		}
#ifdef	USE_MONITOR
#if 0
		cputs( "\ndone\n" ) ;
#endif
#endif
	}
	Terminate() ;
}

static	IrDABOOL	Initialize(void)
{
	OpenStack();
	if( BindLSAP())
		return( TRUE ) ;

	IrLMP_SetShortDeviceName(
			IrLMP_CS_ASCII,
			(byte IrDAFAR *)"SK Teletech Test",16
	);
	MaxDeviceSpeed		=	115200L;
	LinkDisconnectTime	=	3 ;
	TargetLSAP			=	0 ;
	Mode				=	0 ;
	SetupPortConfig() ;
	return( FALSE ) ;
}

static	void	Terminate( void )
{
	UnbindLSAP() ;
	IrLMP_Close() ;
	IrSIR_Close();
}

static	void	OpenStack( void )
{
	IrSIR_Open();
	IrLMP_Open() ;
	IrDA_SetTxFrame( achSendBuf,IrDASENDSZ,IrLAPSENDWINSIZE);
	IrDA_SetRxFrame( achRecvBuf,IrDARECVSZ,IrLAPRECVWINSIZE);
}

static	void	SetupPortConfig( void )
{
PORTCONFIG	pc ;
	IrDA_GetPortConfig( &pc ) ;
	if( MaxDeviceSpeed >= 115200L )
		pc.LineCapability	= IrLAP_BPS_2400|IrLAP_BPS_9600|IrLAP_BPS_19200|IrLAP_BPS_38400|IrLAP_BPS_57600|IrLAP_BPS_115200 ;
	else if( MaxDeviceSpeed >= 57600L )
		pc.LineCapability	= IrLAP_BPS_2400|IrLAP_BPS_9600|IrLAP_BPS_19200|IrLAP_BPS_38400|IrLAP_BPS_57600 ;
	else if( MaxDeviceSpeed >= 38400L )
		pc.LineCapability	= IrLAP_BPS_2400|IrLAP_BPS_9600|IrLAP_BPS_19200|IrLAP_BPS_38400 ;
	else if( MaxDeviceSpeed >= 19200L )
		pc.LineCapability	= IrLAP_BPS_2400|IrLAP_BPS_9600|IrLAP_BPS_19200 ;
	else if( MaxDeviceSpeed >= 9600L )
		pc.LineCapability	= IrLAP_BPS_2400|IrLAP_BPS_9600 ;

	if( LinkDisconnectTime >= 40 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s|IrLAP_SL_16s|IrLAP_SL_20s|IrLAP_SL_25s|IrLAP_SL_30s|IrLAP_SL_40s ;
	else if( LinkDisconnectTime >= 30 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s|IrLAP_SL_16s|IrLAP_SL_20s|IrLAP_SL_25s|IrLAP_SL_30s ;
	else if( LinkDisconnectTime >= 25 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s|IrLAP_SL_16s|IrLAP_SL_20s|IrLAP_SL_25s ;
	else if( LinkDisconnectTime >= 20 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s|IrLAP_SL_16s|IrLAP_SL_20s ;
	else if( LinkDisconnectTime >= 16 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s|IrLAP_SL_16s ;
	else if( LinkDisconnectTime >= 12 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s|IrLAP_SL_12s ;
	else if( LinkDisconnectTime >= 8 )
		pc.DisconnectTime	= IrLAP_SL_3s|IrLAP_SL_8s ;
	else if( LinkDisconnectTime >= 3 )
		pc.DisconnectTime	= IrLAP_SL_3s ;

	pc.NoofBOF		= IrLAP_BOF_0;
	pc.MinTurnaroundTime	= IrLAP_MinTurn_0ms;
	IrDA_SetPortConfig( &pc ) ;
}

static	IrDABOOL	BindLSAP( void )
{
	int		ret ;
	LMCALLBACK	lmc ;

	if( TargetLSAP == 0 ) {
		if(( TargetLSAP = IrLMP_RequestLSAP()) < 0 ) {
#ifdef	USE_MONITOR
/*			cprintf("not enough free lsap (%d)\n", TargetLSAP ) ;*/
#endif
			return( TRUE ) ;
		}
	}

	memset( &lmc, 0, sizeof( LMCALLBACK )) ;
	lmc.ConnectInd	= ConnectInd ;
	lmc.DataInd		= DataInd ;
	if(( ret = IrLMP_BindLSAP( TargetLSAP, &lmc )) < 0 ) {
#ifdef	USE_MONITOR
/*		cprintf("bind error (%d)\n", ret ) ;*/
#endif
		return( TRUE ) ;
	}
#ifdef	USE_MONITOR
/*	cprintf( "Bind to lsap %d\n", TargetLSAP ) ;*/
#endif
	return( FALSE ) ;
}


static	void	UnbindLSAP( void )
{
	if( TargetLSAP >= 0 ) {
#ifdef	USE_MONITOR
		cprintf( "Unbind lsap %d\n", TargetLSAP ) ;
#endif
		IrLMP_UnbindLSAP( TargetLSAP ) ;
	}
}

static	int	IrDACB ConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd )
{
	Mode	&= ~MD_NeedIRsp ;
	return( IrLMP_CONNECTEN ) ;
}

static	void	IrDACB DataInd( int lsap,const void IrDAFAR *buf, uint size )
{
byte IrDAFAR	*ptr ;
	ptr = (byte IrDAFAR *)buf ;
	if( ptr[0] == 0x1B && ptr[1] == 0x25 && ptr[2] == 0x2D )
		Mode	|= MD_NeedIRsp ;
}


/* $Log$
*/
