/************************************************************************
;*                                                                      *
;*     IrDA Driver  IrBIOS                                              *
;*                                                                      *
;*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      *
;*     All Rights Reserved                                              *
;*                                                                      *
;*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  *
;*     The copyright notice above does not evidence any                 *
;*     actual or intended publication of such source code.              *
;*                                                                      *
;*             File:       typetp.c                                     *
;*             Created:    07/14/96                                     *
;*             Author:     Shin-ichi Nagamura                           *
;*                                                                      *
;*             Version:    0.0                                          *
;*             Assembler:  Turbo Assembler  Version 2.0                 *
;*                                                                      *
;***********************************************************************/

/* $Header: TINYTP.CV  0.1  96/07/14 00:00:00  core  Exp $ */

#include	"irdaconf.h"
#include	"string.h"
#include    "memory.h"

#if defined(USE_TinyTP)

#define	TTPMAX		TinyTP_LSAPMAX
#define	SENDBUFSZ	TinyTP_SENDBUFSZ
#define	SENDBUFMAX	TinyTP_SENDBUFMAX		/*今は2以上にしないと死ぬ		*/
#define	TTPRxBUF	IrLAPRECVSZ

//#include	"ptinytp.h"		for my compiler
// this code is ptinytp.h
#if !defined(__ptinytp_h__)
#define	__ptinytp_h__


#define		NOCONNECTCNF


#include	"okmisc.h"
#include	"tinytp.h"
#include	"irlmp.h"
#include	"timer.h"
#if defined(TYPES2DEBUG)
#include	"shpio.h"
#include	"sh7000.h"
#endif	


#if !defined(TTPMAX)
#error	TTPMAXが正しくありません。最大接続可能数（1<n<256）を定義して下さい。
#endif
#if !defined(TTPRxBUF)
#define	TTPRxBUF	2048
#endif	


#define	CP_MAXSDUSIZE			0x01


#define	TTP_MOREBIT				0x80


typedef	volatile	struct	_TTPFLAGS {
	IrDABOOL		txlock ;		
}	TTPFLAGS ;


typedef	struct	_TTPNUM {
	IrDABOOL	lock ;			
	IrDABOOL	lock2 ;			
	IrDABOOL	num ;			
	IrDABOOL	num2 ;			
}	TTPNUM ;


typedef	volatile	struct	_TTPSENDCREDIT	{
	TTPNUM			minus ;			
	TTPNUM			plus ;			
}	TTPCREDIT ;


typedef	volatile	struct	_TTPCONFIG	{
	uint			Mode ;							
	TTPFLAGS		flags ;							
	int				lsap ;							
	TTPCALLBACK		CallBack ;						


	byte			RxSduData[TTPRxBUF] ;			
	uint			RxSduSize[TTPRxBUF/64] ;		
	IrDABOOL		RxSduMBit[TTPRxBUF/64] ;		
	uint			RxSduReadPtr ;					
	uint			RxSduWritePtr ;					
	uint			RxSduTblSize ;					
	uint			RxSduElmSize ;					


	int				TxMaxSduSize ;					
	int				MaxSegmentSize ;				


	TTPCREDIT		RemoteCredit ;					
	TTPCREDIT		SendCredit ;					
	TTPCREDIT		AvailCredit ;					
}	TTPCONFIG ;


#define	TTPM_Used		0x0001						
#define	TTPM_Connected	0x0002						


static	TTPCONFIG	TTPConfig[TTPMAX] ;				


static	byte	TTPTxData[SENDBUFSZ] ;				


static	int		IrDACB ttpCbConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB ttpCbConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
static	void	IrDACB ttpCbDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
static	void	IrDACB ttpCbDataInd( int lsap, const void IrDAFAR *bufp, uint size ) ;
static	void	IrDACB ttpCbDataCnf( int lsap ) ;


static	int		ttpCallConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd ) ;
#if defined(IrDAPRIMARY)
static	void	ttpCallConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd, ulong maxsdu, int status ) ;
#endif	
static	void	ttpCallDisconnectInd( int lsap, int reason ) ;
static	void	ttpCallDataInd( TTPCONFIG *tc ) ;
static	void	ttpCallDataCnf( TTPCONFIG *tc ) ;


static	int		ttpDataReq( int tsap, IrDABOOL mbit, const void IrDAFAR *bufp, int bufsz ) ;
static	void	ttpSendFlowData( TTPCONFIG *tc ) ;


static	uint	ttpReadConnectParameters( TTPCONFIG *tc, byte IrDAFAR *data, int datasz ) ;
static	int		ttpCalcAvailCredit( TTPCONFIG *tc ) ;
static	void	ttpRecalcAvailCredit( TTPCONFIG *tc, int n ) ;
static	void	ttpInitCredit( TTPCREDIT *tc, int icredit ) ;
static	void	ttpAddCredit( TTPCREDIT *tc, int delta ) ;
static	void	ttpSubCredit( TTPCREDIT *tc, int delta ) ;
static	void	ttpAdjustCredit( TTPCREDIT *tc ) ;
static	int		ttpGetCredit( TTPCREDIT *tc ) ;
static	void	ttpSetupRxSdu( int tsap ) ;


static	void	IrDACB	ttpCbDmyConnectPrv( void ) ;
static	void	IrDACB	ttpCbDmyDisconnectPrv( void ) ;


static	void	(IrDACB	*ConnectPrvPtr)( void ) ;
static	void	(IrDACB	*DisconnectPrvPtr)( void ) ;
static	ulong IrDAFAR	*TxSizePtr ;	
static	ulong IrDAFAR	*RxSizePtr ;	
static	ulong	DmyTxSize ;				
static	ulong	DmyRxSize ;				


int		IrDAAPI	TTP_Open( void )
{
	int		i ;


	
	for( i=0 ; i<TTPMAX ; i++ ) {
		TTPConfig[i].Mode	= 0 ;
		IrDA_memset( (void*)&TTPConfig[i].flags, 0, sizeof( TTPFLAGS )) ;
	}


	ConnectPrvPtr		= ttpCbDmyConnectPrv ;
	DisconnectPrvPtr	= ttpCbDmyDisconnectPrv ;


	TxSizePtr			= &DmyTxSize ;
	RxSizePtr			= &DmyRxSize ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_Open(): done\n" ) ;
#endif	


	return( IrDAR_ACCEPT ) ;
}


void	IrDAAPI	TTP_Close( void )
{
}


int		IrDAAPI	TTP_RequestLSAP( void )
{
	return( IrLMP_RequestLSAP()) ;
}


int		IrDAAPI	TTP_BindLSAP( int tsap, TTPCALLBACK IrDAFAR *ttpc )
{
	int				ret ;
	LMCALLBACK		lmc ;


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_BindLSAP():" ) ;
	_DPUTW( DL_TTPINFO, " tsap:", tsap ) ;
	_DPUTW( DL_TTPINFO, " Mode:", TTPConfig[tsap].Mode ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( tsap < 0 || TTPMAX <= tsap )
		return( IrDAR_NORESOURCE ) ;
	if( TTPConfig[tsap].Mode & TTPM_Used )
		return( IrDAR_INUSE ) ;


	IrDA_memset( &lmc, 0, sizeof( LMCALLBACK )) ;
	lmc.ConnectInd		= ttpCbConnectInd ;
	lmc.ConnectCnf		= ttpCbConnectCnf ;
	lmc.DataInd			= ttpCbDataInd ;
	lmc.DataCnf			= ttpCbDataCnf ;
	lmc.DisconnectInd	= ttpCbDisconnectInd ;


	ret		= IrLMP_BindLSAP( tsap, &lmc ) ;
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_BindLSAP():" ) ;
	_DPUTW( DL_TTPINFO, " IrLMP_BindLSAP( ", tsap ) ;
	_DPUTW( DL_TTPINFO, ", )=", ret ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( ret != LMPR_ACCEPT )
		return( ret ) ;


	TTPConfig[tsap].lsap	= tsap ;
	TTPConfig[tsap].Mode	= TTPM_Used ;
	IrDA_memcpy( (void*)&TTPConfig[tsap].CallBack, ttpc, sizeof( TTPCALLBACK )) ;


	return( ret ) ;
}


void	IrDAAPI	TTP_UnbindLSAP( int lsap )
{
#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_UnbindLSAP():" ) ;
	_DPUTW( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( lsap >= TTPMAX )
		return ;


	TTPConfig[lsap].Mode	= 0 ;
	IrLMP_UnbindLSAP( lsap ) ;
}


int		IrDAAPI	TTP_GetRealLSAP( int lsap )
{
	return( IrLMP_GetRealLSAP( lsap )) ;
}


#if defined(TYPES2DEBUG)
extern	int		ShPiocLGet( void ) ;
extern	void	ShPioaHSet( int m ) ;
extern	void	ShPioaHRes( int m ) ;
extern	void	breakpoint( void ) ;
static	IrDABOOL	chkAvailCredit ;
static	const	int	indicator1[]	= { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF } ;
static	const	int	indicator2[]	= { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF } ;
static	int		makeMap1( int val )
{
	if( val > 8 )
		val	= 8 ;
	return( indicator1[val] ) ;
}
static	int		makeMap2( int val )
{
	if( val > 8 )
		val	= 8 ;
	return( indicator2[val] ) ;
}
static	void	dspSendCredit( TTPCONFIG *tc )
{
	if( ShPiocLGet() & PCL_DSW8 )
		return ;
	SH_Pio.pbdrl	= makeMap1( ttpGetCredit( &tc->SendCredit )) ;
}
void	dspCredit( void )
{
	if(( ShPiocLGet() & PCL_DSW8 ) == 0 ) {
		dspSendCredit( &TTPConfig[0] ) ;
		return ;
	}
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &TTPConfig[0].AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
	SH_Pio.pbdrl	= makeMap1( ttpGetCredit( &TTPConfig[0].RemoteCredit )) |
					  makeMap2( ttpGetCredit( &TTPConfig[0].AvailCredit )) ;
}
#endif	


#if defined(IrDAPRIMARY)


int		IrDAAPI	TTP_ConnectReq( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd )
{
	int		ret ;
	int		ttppdusz ;
	TTPCONFIG	*tc ;


	tc		= &TTPConfig[slsap] ;
	if( tc->Mode & TTPM_Connected )
		return( IrDAR_CONNECTING ) ;


	tc->Mode			&= TTPM_Used ;
	ttpInitCredit( &tc->AvailCredit, 0 ) ;
	ttpInitCredit( &tc->SendCredit, 0 ) ;
	ttpInitCredit( &tc->RemoteCredit, 1 ) ;
#if defined(TYPES2DEBUG)
	dspSendCredit( tc ) ;
#endif	


	ttppdusz				= 0 ;
	TTPTxData[ttppdusz++]	= ttpGetCredit( &tc->RemoteCredit ) ;


	if( tcd->DataSize > SENDBUFSZ + 1 - ttppdusz )
		return( IrDAR_TOOLONG ) ;


#if	defined(DMSG)
{	int	i ;
	_DPUTS( DL_TTPINFO, " TTP::TTP_ConnectReq():" ) ;
	_DPUTW( DL_TTPINFO, " slsap:", slsap ) ;
	_DPUTW( DL_TTPINFO, " datasz:", tcd->DataSize ) ;
	if( tcd->DataSize > 0 ) {
		_DPUTB( DL_TTPINFO, " data:", tcd->Data[0] ) ;
		for( i=1 ; i<tcd->DataSize ; i++ )
			_DPUTB( DL_TTPINFO, " data:", tcd->Data[i] ) ;
	}
	_DPUTS( DL_TTPINFO, "\n" ) ;
}
#endif	
	IrDA_memcpy( &TTPTxData[ttppdusz], tcd->Data, tcd->DataSize ) ;
	ttppdusz		+= tcd->DataSize ;


	ret		= IrLMP_ConnectReq( addr, slsap, dlsap, TTPTxData, ttppdusz ) ;
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_ConnectReq():" ) ;
	_DPUTW( DL_TTPINFO, " IrLMP_ConnectReq():", ret ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	return( ret ) ;
}
#endif	


int		IrDAAPI	TTP_DisconnectReq( int lsap, void IrDAFAR *data, int datasz )
{
	int		ret ;


	ret		= IrLMP_DisconnectReq( lsap, IrLMP_DR_UserRequest, data, datasz ) ;
	if( ret == LMPR_ACCEPT ) {
		TTPConfig[lsap].Mode	&= ~TTPM_Connected ;
		(*DisconnectPrvPtr)() ;
	}


	return( ret ) ;
}


int		IrDAAPI	TTP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz )
{
	int		ret ;
#if defined(IrDA_TTPDATAREQPRB)
	IrDA_TTPDATAREQPRB( 0 ) ;
#endif	
	ret		= ttpDataReq( lsap, 0, bufp, bufsz ) ;
#if defined(IrDA_TTPDATAREQPRB)
	IrDA_TTPDATAREQPRB( 1 ) ;
#endif	
	return( ret ) ;
}


int		IrDAAPI	TTP_MDataReq( int lsap, const void IrDAFAR *bufp, int bufsz )
{
	int		ret ;
#if defined(IrDA_TTPDATAREQPRB)
	IrDA_TTPDATAREQPRB( 0 ) ;
#endif	
	ret		= ttpDataReq( lsap, TTP_MOREBIT, bufp, bufsz ) ;
#if defined(IrDA_TTPDATAREQPRB)
	IrDA_TTPDATAREQPRB( 1 ) ;
#endif	
	return( ret ) ;
}


uint	IrDAAPI	TTP_GetRxSize( int lsap )
{
	TTPCONFIG	*tc ;


	if( lsap < 0 || TTPMAX <= lsap )
		return( 0 ) ;


	tc		= &TTPConfig[lsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return( 0 ) ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;
	if( ttpGetCredit( &tc->RemoteCredit ) == (int)tc->RxSduTblSize )
		return( 0 ) ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_GetRxSize():" ) ;
	_DPUTW( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_TTPINFO, " return:", tc->RxSduSize[tc->RxSduReadPtr] ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	return( tc->RxSduSize[tc->RxSduReadPtr] ) ;
}


IrDABOOL	IrDAAPI	TTP_GetMBit( int tsap )
{
	TTPCONFIG	*tc ;


	if( tsap < 0 || TTPMAX <= tsap )
		return( 0 ) ;


	tc		= &TTPConfig[tsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return( 0 ) ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;
	if( ttpGetCredit( &tc->RemoteCredit ) == (int)tc->RxSduTblSize )
		return( 0 ) ;


	return( tc->RxSduMBit[tc->RxSduReadPtr] ) ;
}


#if defined(WIN32)		
const void IrDAFAR IrDAAPI *TTP_GetDataPtr( int tsap )
#else	
const void IrDAFAR* IrDAAPI TTP_GetDataPtr( int tsap )
#endif	
{
	TTPCONFIG	*tc ;


	if( tsap < 0 || TTPMAX <= tsap )
		return( 0 ) ;


	tc		= &TTPConfig[tsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return( 0 ) ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;
	if( ttpGetCredit( &tc->RemoteCredit ) == (int)tc->RxSduTblSize )
		return( 0 ) ;


	return( (void IrDAFAR*)&tc->RxSduData[tc->RxSduReadPtr*tc->RxSduElmSize] ) ;
}


void	IrDAAPI	TTP_ReleaseRxQueue( int lsap )
{
	TTPCONFIG	*tc ;


	if( lsap < 0 || TTPMAX <= lsap )
		return ;


	tc		= &TTPConfig[lsap] ;
#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_ReleaseRxQueue():" ) ;
	_DPUTW( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_TTPINFO, " tc->Mode:", tc->Mode ) ;
	_DPUTW( DL_TTPINFO, " tc->RemoteCredit:", ttpGetCredit( &tc->RemoteCredit )) ;
	_DPUTW( DL_TTPINFO, " tc->RxSduTblSize:", tc->RxSduTblSize ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if(( tc->Mode & TTPM_Used ) == 0 )
		return ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return ;
	if( ttpGetCredit( &tc->RemoteCredit ) == (int)tc->RxSduTblSize )
		return ;


	*RxSizePtr	+= (ulong)tc->RxSduSize[tc->RxSduReadPtr] ;


	if( ++tc->RxSduReadPtr == tc->RxSduTblSize )
		tc->RxSduReadPtr	= 0 ;
#if	defined(TYPES2DEBUG)
	chkAvailCredit	= FALSE ;
#endif	
	ttpAddCredit( &tc->RemoteCredit, 1 ) ;
	ttpAddCredit( &tc->AvailCredit, 1 ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
#if	defined(TYPES2DEBUG)
	if( chkAvailCredit )
		ShPioaHSet( PAH_LDR ) ;
	dspCredit() ;
#endif	


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_ReleaseRxQueue():" ) ;
	_DPUTW( DL_TTPINFO, " tc->RemoteCredit:", ttpGetCredit( &tc->RemoteCredit )) ;
	_DPUTW( DL_TTPINFO, " tc->AvailCredit:", ttpGetCredit( &tc->AvailCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	


	if( ttpGetCredit( &tc->RemoteCredit ) == ttpGetCredit( &tc->AvailCredit ))
		ttpSendFlowData( tc ) ;


	return ;
}


IrDABOOL	IrDAAPI	TTP_QueryTxQueue( int lsap )
{
#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_QueryTxQueue():" ) ;
	_DPUTB( DL_TTPINFO, " TTPConfig[", lsap ) ;
	_DPUTW( DL_TTPINFO, "].SendCredit:", ttpGetCredit( &TTPConfig[lsap].SendCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( ttpGetCredit( &TTPConfig[lsap].SendCredit ) < 1 )
		return( FALSE ) ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_QueryTxQueue(): CheckSendBuf() is OK\n" ) ;
#endif	
	return( IrLMP_QueryTxQueue()) ;
}


uint	IrDAAPI	TTP_QeuryRxQueue( int lsap )
{
	TTPCONFIG	*tc ;


	if( lsap < 0 || TTPMAX <= lsap )
		return( 0 ) ;


	tc		= &TTPConfig[lsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return( 0 ) ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;


	return( tc->RxSduTblSize - ttpGetCredit( &tc->RemoteCredit )) ;
}


uint	IrDAAPI	TTP_GetMaxRxSize( int tsap )
{
	int		ret ;


	if( tsap < 0 || TTPMAX <= tsap )
		return( 0 ) ;
	if(( TTPConfig[tsap].Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;


	if(( ret = IrLMP_GetMaxRxSize( tsap )) < 1 )
		return( 0 ) ;
	return( ret - 1 ) ;
}


uint	IrDAAPI	TTP_GetMaxTxSize( int lsap )
{
	int		ret ;


	if( lsap < 0 || TTPMAX <= lsap )
		return( 0 ) ;
	if(( TTPConfig[lsap].Mode & TTPM_Connected ) == 0 )
		return( 0 ) ;


	if(( ret = IrLMP_GetMaxTxSize( lsap )) < 1 )
		return( 0 ) ;
	if( ret > SENDBUFSZ )
		ret	= SENDBUFSZ ;
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_GetMaxTxSize():" ) ;
	_DPUTW( DL_TTPINFO, " ret:", ret - 1 ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	return( ret - 1 ) ;
}


void	IrDAAPI	TTP_SetConnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		ConnectPrvPtr	= func ;
	else
		ConnectPrvPtr	= ttpCbDmyConnectPrv ;
}


void	IrDAAPI	TTP_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		DisconnectPrvPtr	= func ;
	else
		DisconnectPrvPtr	= ttpCbDmyDisconnectPrv ;
}


void	IrDAAPI	TTP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp )
{
	if( txszp )
		TxSizePtr			= txszp ;
	else
		TxSizePtr			= &DmyTxSize ;


	if( rxszp )
		RxSizePtr			= rxszp ;
	else
		RxSizePtr			= &DmyRxSize ;
}


#if !defined(IrDANOSTATIC)


static	int		IrDACB ttpCbConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd )
{
	TTPCONFIG	*tc ;
	TTPCONNDAT	tcd ;
	int			ttppdusz ;
	int			ret ;


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbConnectInd():" ) ;
	_DPUTB( DL_TTPINFO, " slsap:", slsap ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	tc		= &TTPConfig[slsap] ;
	if( tc == 0 )
		return( IrDA_CONNECTDI ) ;
#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_TTPDAT, " TTP::ttpCbConnectInd():" ) ;
	if( lcd->DataSize > 0 ) {
		_DPUTB( DL_TTPDAT, " data=", lcd->Data[0] ) ;
		for( i=1 ; i<lcd->DataSize ; i++ )
			_DPUTB( DL_TTPDAT, ".", lcd->Data[i] ) ;
	}
	_DPUTS( DL_TTPDAT, "\n" ) ;
}
#endif	


	tc->Mode			|= TTPM_Connected ;
	IrDA_memset( (void*)&tc->flags, 0, sizeof( TTPFLAGS )) ;
	tc->TxMaxSduSize	= 0 ;
	ttpInitCredit( &tc->AvailCredit, 0 ) ;
	ttpInitCredit( &tc->SendCredit, lcd->Data[0] & 0x7F ) ;
#if defined(TYPES2DEBUG)
	dspSendCredit( tc ) ;
	ShPioaHRes( PAH_LDR ) ;
#endif	
	tc->MaxSegmentSize	= IrLMP_GetMaxTxSize( slsap ) - 1 ;
	ttpSetupRxSdu( slsap ) ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbConnectInd():" ) ;
	_DPUTW( DL_TTPINFO, " SendCredit:", ttpGetCredit( &tc->SendCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	


	ttppdusz			= 0 ;
	if( lcd->Data[ttppdusz++] & 0x80 )
		ttppdusz		+= ttpReadConnectParameters( tc, &lcd->Data[ttppdusz], lcd->DataSize - ttppdusz ) ;


	tcd.Data		= &lcd->Data[ttppdusz] ;
	tcd.DataSize	= lcd->DataSize - ttppdusz ;
	if(( ret = ttpCallConnectInd( slsap, addr, dlsap, &tcd )) != IrDA_CONNECTEN )
		tc->Mode	&= ~TTPM_Connected ;


	ttppdusz	= 0 ;
	TTPTxData[ttppdusz++]	= ttpGetCredit( &tc->RemoteCredit ) ;
	ttpInitCredit( &tc->AvailCredit, 0 ) ;


	if( tcd.DataSize > SENDBUFSZ + 1 - ttppdusz )
		tcd.DataSize	= SENDBUFSZ + 1 - ttppdusz ;
	IrDA_memcpy( &TTPTxData[ttppdusz], tcd.Data, tcd.DataSize ) ;


	lcd->Data		= TTPTxData ;
	lcd->DataSize	= ttppdusz + tcd.DataSize ;


	return( ret ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB ttpCbConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status )
{
#if defined(IrDAPRIMARY)
	TTPCONFIG	*tc ;
	TTPCONNDAT	tcd ;
	int			ttppdusz ;


	if( slsap < 0 || TTPMAX <= slsap )				
		return ;
	tc		= &TTPConfig[slsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return ;


	IrDA_memset( (void*)&tc->flags, 0, sizeof( TTPFLAGS )) ;
	tc->TxMaxSduSize	= 0 ;
	ttpInitCredit( &tc->AvailCredit, 0 ) ;
	ttpInitCredit( &tc->SendCredit, lcd->Data[0] & 0x7F ) ;
#if defined(TYPES2DEBUG)
	dspSendCredit( tc ) ;
	ShPioaHRes( PAH_LDR ) ;
#endif	
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbConnectCnf():" ) ;
	_DPUTW( DL_TTPINFO, " SendCredit:", ttpGetCredit( &tc->SendCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	tc->MaxSegmentSize	= IrLMP_GetMaxTxSize( slsap ) - 1 ;
	ttpSetupRxSdu( slsap ) ;


	ttppdusz			= 0 ;
	if( lcd->Data[ttppdusz++] & 0x80 )
		ttppdusz		+= ttpReadConnectParameters( tc, &lcd->Data[1], lcd->DataSize - 1 ) ;


	if( status == LMPR_CONNECTING )
		tc->Mode	|= TTPM_Connected ;


	tcd.Data		= &lcd->Data[ttppdusz] ;
	tcd.DataSize	= lcd->DataSize - ttppdusz ;


	ttpCallConnectCnf( slsap, addr, dlsap, &tcd, tc->TxMaxSduSize, status ) ;
#else	
	UNREFERENCE( slsap ) ;
	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;
	UNREFERENCE( status ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB ttpCbDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz )
{
	data	= data ;
	datasz	= datasz ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbDisconnectInd():" ) ;
	_DPUTW( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_TTPINFO, " reason:", reason ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if(( TTPConfig[lsap].Mode & TTPM_Used ) == 0 )
		return ;


	TTPConfig[lsap].Mode	&= ~TTPM_Connected ;
	ttpCallDisconnectInd( lsap, reason ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB ttpCbDataInd( int lsap, const void IrDAFAR *bufp, uint size )
{
	TTPCONFIG	*tc ;
	int			delta, m ;
	const byte IrDAFAR	*data ;


	if( size < 1 )									
		return ;


	tc		= &TTPConfig[lsap] ;
	data	= (const byte IrDAFAR *)bufp ;
	delta	= *data & 0x7F ;						
	m		= ( *data & TTP_MOREBIT ) >> 7 ;
	data	++ ;
	size	-- ;
	ttpAddCredit( &tc->SendCredit, delta ) ;
#if defined(TYPES2DEBUG)
	chkAvailCredit	= TRUE ;
	dspSendCredit( tc ) ;
#endif	
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbDataInd():" ) ;
	_DPUTB( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTB( DL_TTPINFO, " m:", m ) ;
	_DPUTB( DL_TTPINFO, " delta:", delta ) ;
	_DPUTW( DL_TTPINFO, " size:", size ) ;
	_DPUTW( DL_TTPINFO, " tc->RemoteCredit:", ttpGetCredit( &tc->RemoteCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_TTPDAT, " TTP::ttpCbDataInd():" ) ;
	_DPUTB( DL_TTPDAT, " data=", data[0] ) ;
	for( i=1 ; i<size ; i++ )
		_DPUTB( DL_TTPDAT, ".", data[i] ) ;
}
	_DPUTS( DL_TTPDAT, "\n" ) ;
#endif	


													
	if( size > 0 && ttpGetCredit( &tc->RemoteCredit ) > 0 ) {
		ttpSubCredit( &tc->RemoteCredit, 1 ) ;			
#if	defined(TYPES2DEBUG)
		dspCredit() ;
#endif	
		if( size >= tc->RxSduElmSize )				
			size	= tc->RxSduElmSize ;
		IrDA_memcpy( (void*)&tc->RxSduData[tc->RxSduWritePtr*tc->RxSduElmSize], data, size ) ;
		tc->RxSduSize[tc->RxSduWritePtr]	= size ;
		tc->RxSduMBit[tc->RxSduWritePtr]	= m ;
		if( ++tc->RxSduWritePtr == tc->RxSduTblSize )
			tc->RxSduWritePtr	= 0 ;
		ttpCallDataInd( tc ) ;
	}


	ttpCallDataCnf( tc ) ;
	ttpSendFlowData( tc ) ;
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbDataInd():" ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit => ", ttpGetCredit( &tc->AvailCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	ttpAdjustCredit( &tc->SendCredit ) ;
	ttpAdjustCredit( &tc->RemoteCredit ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
	ttpAdjustCredit( &tc->AvailCredit ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB ttpCbDataCnf( int lsap )
{
	TTPCONFIG	*tc ;


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCbDataCnf():" ) ;
	_DPUTB( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit:", ttpGetCredit( &TTPConfig[lsap].AvailCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( lsap < 0 || TTPMAX <= lsap )				
		return ;
	tc		= &TTPConfig[lsap] ;
	if(( tc->Mode & TTPM_Used ) == 0 )
		return ;
	if(( tc->Mode & TTPM_Connected ) == 0 )
		return ;


	ttpCallDataCnf( tc ) ;
	ttpSendFlowData( tc ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	ttpCbDmyConnectPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	ttpCbDmyDisconnectPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		ttpCallConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd )
{
	if( tcd->DataSize < 0 )
		tcd->DataSize	= 0 ;


	(*ConnectPrvPtr)() ;


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCallConnectInd():" ) ;
	_DPUTB( DL_TTPINFO, " TTPConfig[", slsap ) ;
	_DPUTD( DL_TTPINFO, "].CallBack.ConnectInd:", (dword)TTPConfig[slsap].CallBack.ConnectInd ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( TTPConfig[slsap].CallBack.ConnectInd == 0 )
		return( IrDA_CONNECTDI ) ;


	return((*TTPConfig[slsap].CallBack.ConnectInd)( slsap, addr, dlsap, tcd )) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	ttpCallConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd, ulong maxsdu, int status )
{
	if( tcd->DataSize < 0 )
		tcd->DataSize	= 0 ;


	(*ConnectPrvPtr)() ;


	if( TTPConfig[slsap].CallBack.ConnectCnf )
		(*TTPConfig[slsap].CallBack.ConnectCnf)( slsap, addr, dlsap, tcd, maxsdu, status ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpCallDisconnectInd( int lsap, int reason )
{
	if( TTPConfig[lsap].CallBack.DisconnectInd )
		(*TTPConfig[lsap].CallBack.DisconnectInd)( lsap, reason ) ;


	(*DisconnectPrvPtr)() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpCallDataInd( TTPCONFIG *tc )
{
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCallDataInd()" ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
#if defined(IrDA_TTPDATAINDPRB)
		IrDA_TTPDATAINDPRB( 0 ) ;
#endif	
	if( tc->CallBack.DataInd )
		(*tc->CallBack.DataInd)( tc->lsap ) ;
#if defined(IrDA_TTPDATAINDPRB)
		IrDA_TTPDATAINDPRB( 1 ) ;
#endif	


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCallDataInd()" ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit:", ttpGetCredit( &TTPConfig[tc->lsap].AvailCredit )) ;
	_DPUTS( DL_TTPINFO, " IrLMP_QueryTxQueue()=" ) ;
	_DPUTS( DL_TTPINFO, IrLMP_QueryTxQueue() ? "TRUE" : "FALSE" ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpCallDataCnf( TTPCONFIG *tc )
{
	if( ttpGetCredit( &tc->SendCredit ) > 0 && IrLMP_QueryTxQueue()) {
		if( tc->CallBack.DataCnf ) {
#if defined(IrDA_TTPDATACNFPRB)
			IrDA_TTPDATACNFPRB( 0 ) ;
#endif	
			( *tc->CallBack.DataCnf )( tc->lsap ) ;
#if defined(IrDA_TTPDATACNFPRB)
			IrDA_TTPDATACNFPRB( 1 ) ;
#endif	
		}
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		ttpDataReq( int lsap, IrDABOOL mbit, const void IrDAFAR *bufp, int bufsz )
{
	int		ret ;
	int		n ;
	TTPCONFIG	*tc ;


	tc		= &TTPConfig[lsap] ;
	if( bufsz > tc->MaxSegmentSize )
		return( IrDAR_TOOLONG ) ;
	if( !IrLMP_QueryTxQueue())
		return( IrDAR_BUFFULL ) ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::TTP_DataReq():" ) ;
	_DPUTW( DL_TTPINFO, " tc->SendCredit:", ttpGetCredit( &tc->SendCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( ttpGetCredit( &tc->SendCredit ) <= 0 )
		return( IrDAR_BUFFULL ) ;


	if( tc->flags.txlock )
		return( IrDAR_BUSYWAIT ) ;
	tc->flags.txlock	= TRUE ;
	n		= ttpCalcAvailCredit( tc ) ;


	TTPTxData[0]	= (byte)( mbit | n ) ;
	IrDA_memcpy( &TTPTxData[1], bufp, bufsz ) ;
	ret		= IrLMP_DataReq( lsap, TTPTxData, bufsz+1 ) ;
	tc->flags.txlock	= FALSE ;


	if( ret == LMPR_ACCEPT ) {						
		ttpSubCredit( &tc->SendCredit, 1 ) ;
		*TxSizePtr	+= (ulong)bufsz ;
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::TTP_DataReq():" ) ;
	_DPUTW( DL_TTPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_TTPINFO, " bufsz", bufsz ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
#if defined(DMSG)
{	int		i ;
	_DPUTS( DL_TTPDAT, "                   : data" ) ;
	_DPUTB( DL_TTPDAT, "=", ((byte IrDAFAR*)bufp)[0] ) ;
	for( i=1 ; i<bufsz ; i++ )
		_DPUTB( DL_TTPDAT, ".", ((byte IrDAFAR*)bufp)[i] ) ;
	_DPUTS( DL_TTPDAT, "\n" ) ;
}
#endif	
	} else {
		ttpRecalcAvailCredit( tc, n ) ;
	}


	return( ret ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpSendFlowData( TTPCONFIG *tc )
{
	int			ret ;
	int			n ;
	byte		flowbuf[1] ;


	if( ttpGetCredit( &tc->AvailCredit ) == 0 )		


													


		return ;


#if	defined(DMSG2)
	_DPUTS( DL_TTPINFO, " TTP::ttpSendFlowData():" ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
	if( !IrLMP_QueryTxQueue())
		return ;
	if( tc->flags.txlock )
		return ;
	tc->flags.txlock	= TRUE ;
	n		= ttpCalcAvailCredit( tc ) ;


#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpSendFlowData():" ) ;
	_DPUTW( DL_TTPINFO, " DeltaCredit:", n ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit:", ttpGetCredit( &tc->AvailCredit )) ;
	_DPUTW( DL_TTPINFO, " RemoteCredit:", ttpGetCredit( &tc->RemoteCredit )) ;
	_DPUTW( DL_TTPINFO, " SendCredit:", ttpGetCredit( &tc->SendCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	


	flowbuf[0]	= (byte)n ;
	ret		= IrLMP_DataReq( tc->lsap, flowbuf, 1 ) ;
	tc->flags.txlock	= FALSE ;


	if( ret != LMPR_ACCEPT ) {
#if	defined(DMSG)
		_DPUTS( DL_TTPINFO, " TTP:ttpSendFlowData():" ) ;
		_DPUTW( DL_TTPINFO, " IrLMP_DataReq()=", ret ) ;
		_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
		ttpRecalcAvailCredit( tc, n ) ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	ttpReadConnectParameters( TTPCONFIG *tc, byte IrDAFAR *data, int datasz )
{
	int		i, j ;
	int		len ;
	int		cmd, len2 ;


	len		= *data++ ;
	datasz	-- ;


	for( i=0 ; i<datasz && i<len ; ) {
		cmd		= data[i++] ;
		len2	= data[i++] ;


		switch( cmd ) {
		case	CP_MAXSDUSIZE :
			tc->TxMaxSduSize	= 0 ;
			j		= len2 ;
			if( j > 4 )
				j	= 4 ;
			for( ; j ; j-- )
				tc->TxMaxSduSize	|= ((int)data[i+4-j]) << ( j - 1 ) ;
			break ;
		}


		i	+= len2 ;
	}


	if( datasz < i + 1 )
		return( datasz ) ;


	return( i + 1 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		ttpCalcAvailCredit( TTPCONFIG *tc )
{
	int		n ;


	if( ttpGetCredit( &tc->AvailCredit ) > 127 )
		n		= 127 ;
	else
		n		= ttpGetCredit( &tc->AvailCredit ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
	ttpSubCredit( &tc->AvailCredit, n ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
#if	defined(TYPES2DEBUG)
	dspCredit() ;
#endif	
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpCalcAvailCredit():" ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit:", n ) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	


	return( n ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpRecalcAvailCredit( TTPCONFIG *tc, int n )
{
	ttpAddCredit( &tc->AvailCredit, n ) ;
#if	defined(TYPES2DEBUG)
	if( ttpGetCredit( &tc->AvailCredit ) >= 127 )
		breakpoint() ;
#endif	
#if	defined(TYPES2DEBUG)
	dspCredit() ;
#endif	
#if	defined(DMSG)
	_DPUTS( DL_TTPINFO, " TTP::ttpRecalcAvailCredit():" ) ;
	_DPUTW( DL_TTPINFO, " AvailCredit+=", n ) ;
	_DPUTW( DL_TTPINFO, " => ", ttpGetCredit( &tc->AvailCredit )) ;
	_DPUTS( DL_TTPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpInitCredit( TTPCREDIT *tc, int icredit )
{
	tc->plus.lock	= FALSE ;
	tc->plus.lock2	= FALSE ;
	tc->plus.num	= icredit ;
	tc->plus.num2	= 0 ;
	tc->minus.lock	= FALSE ;
	tc->minus.lock2	= FALSE ;
	tc->minus.num	= 0 ;
	tc->minus.num2	= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpAddCredit( TTPCREDIT *tc, int delta )
{
	if( !tc->plus.lock ) {
		tc->plus.lock	= TRUE ;
		tc->plus.num	+= delta ;
		tc->plus.lock	= FALSE ;
#if defined(TYPES2DEBUG)
	} else if( tc->plus.lock2 ) {
		breakpoint() ;
#endif	
	} else {
		tc->plus.lock2	= TRUE ;		
		tc->plus.num2	+= delta ;
		tc->plus.lock2	= FALSE ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpSubCredit( TTPCREDIT *tc, int delta )
{
	if( !tc->minus.lock ) {
		tc->minus.lock	= TRUE ;
		tc->minus.num	+= delta ;
		tc->minus.lock	= FALSE ;
#if defined(TYPES2DEBUG)
	} else if( tc->minus.lock2 ) {
		breakpoint() ;
#endif	
	} else {
		tc->minus.lock2	= TRUE ;		
		tc->minus.num2	+= delta ;
		tc->minus.lock2	= FALSE ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpAdjustCredit( TTPCREDIT *tc )
{
	int		lockflag ;


	lockflag	= FALSE ;


	if( tc->plus.lock || tc->plus.lock2 ) {
		lockflag	= TRUE ;
	} else {
		tc->plus.num	+= tc->plus.num2 ;
		tc->plus.num2	= 0 ;
	}


	if( tc->minus.lock || tc->minus.lock2 ) {
		lockflag	= TRUE ;
	} else {
		tc->minus.num	+= tc->minus.num2 ;
		tc->minus.num2	= 0 ;
	}


	if( lockflag )
		return ;


	tc->plus.num	-= tc->minus.num ;
	tc->minus.num	= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		ttpGetCredit( TTPCREDIT *tc )
{
	int			credit ;


	for(;;) {
		
		credit		= tc->plus.num + tc->plus.num2 -
						tc->minus.num - tc->minus.num2 ;
		if( credit == (int)( tc->plus.num + tc->plus.num2 -
					tc->minus.num - tc->minus.num2 ) )
			break ;
	}


	return( credit ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	ttpSetupRxSdu( int tsap )
{
	TTPCONFIG	*tc ;


	tc		= &TTPConfig[tsap] ;
	tc->RxSduReadPtr	= 0 ;
	tc->RxSduWritePtr	= 0 ;
	tc->RxSduElmSize	= IrLMP_GetMaxRxSize( tsap ) ;
	tc->RxSduTblSize	= TTPRxBUF / tc->RxSduElmSize ;
	ttpInitCredit( &tc->RemoteCredit, tc->RxSduTblSize ) ;
	ttpAddCredit( &tc->AvailCredit, tc->RxSduTblSize - 1 ) ;
}
#endif	


#endif	

#endif

/* $Log: rcs/TINYTP.CV $
 * Revision 0.1  96/07/14 00:00:00  core
 * Initial revision
 * 
*/
