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
;*             File:       irlmp.c                                      *
;*             Created:    07/14/96                                     *
;*             Author:     Shin-ichi Nagamura                           *
;*                                                                      *
;*             Version:    0.0                                          *
;*             Assembler:  Turbo Assembler  Version 2.0                 *
;*                                                                      *
;***********************************************************************/

/* $Header: IRLMP.CV  0.1  96/07/14 00:00:00  core  Exp $ */


#include	"irdaconf.h"
#include	"string.h"
#include    "memory.h"

#define		DEVICECONFIGMAX		1
#define		LSAPMAX				IrLMP_LSAPMAX

#define		TIME_IdleTimer		IrLMP_IDLETIMER

#define		IASCLASSMAX			IrLMP_IASCLASSMAX
#define		IASATTRMAX			IrLMP_IASATTRMAX
#define		IASCLIENTMAX		1

//#include	"pirlmp.h"		for my compiler
// this code is pirlmp.h
#if !defined(__pirlmp_h__)
#define	__pirlmp_h__


#if	defined(DMSG2)
#include	<stdio.h>
#endif	
#include	"irlap.h"
#include	"irlmp.h"
#include	"okmisc.h"
#include	"timer.h"


#if defined(DEVICECONFIGMAX)
#undef	DEVICECONFIGMAX
#endif
#define	DEVICECONFIGMAX		1			
#if !defined(LSAPMAX)
#error	LSAPMAXが正しくありません。最大接続ポイント数（1<n<0x80）を定義して下さい。
#endif
#if !defined(TIME_IdleTimer)
#error	TIME_IdleTimerが正しくありません。LSAPコネクションが切断されてからIrLAPコネクションを切断するまでの時間をmscで定義して下さい。
#endif


#if !defined(IrLMP_SERVICEHINTSZ)
#define	IrLMP_SERVICEHINTSZ	2						
#endif	
#define	IrLMP_SHORTDEVNMSZ	23						
#define	CDATAMAX			32


typedef	struct	_DEVICECONFIG	{
	dword	Address ;						
	int		ICCStatus ;						
	uint	Mode ;							
	int		LSAPCount ;						
	int		IdleTimerHndl ;					
}	DEVICECONFIG ;


#define	DCM_Used			0x0001
#define	DCM_Connected		0x0002


static	DEVICECONFIG	DeviceConfig[DEVICECONFIGMAX] ;


typedef	struct	_LSAPCONFIG	{
	uint			Mode ;						
	DEVICECONFIG	*Device ;					
	int				Status ;					
	LMCALLBACK		CallBack ;					


	int				dLSAP ;						
	int				lmpConnectDataSize ;		
	byte			lmpConnectData[CDATAMAX] ;	


}	LSAPCONFIG ;


#define	LCM_Used			0x01
#define	LCM_Connected		0x02
#define	LCM_RsvConnectRsp	0x04


static	LSAPCONFIG	LSAPConfig[LSAPMAX+1] ;		


static	IrDABOOL	disableIdleTimer ;
static	byte	ServiceHint[IrLMP_SERVICEHINTSZ] ;	
static	byte	ShortDeviceName[IrLMP_SHORTDEVNMSZ] ;
static	byte	IrLAPDeviceName[IrLAP_NNMSZ] ;		


#define	LMPDU_Confirm		0x80
#define	LMPDU_Connect		1
#define	LMPDU_Disconnect	2


static	void	(IrDACB	*PrvConnect)( void ) ;
static	void	(IrDACB	*PrvDisconnect)( void ) ;
static	ulong IrDAFAR	*TxSizePtr ;	
static	ulong IrDAFAR	*RxSizePtr ;	
static	ulong	DmyTxSize ;				
static	ulong	DmyRxSize ;				


static	void	(IrDACB *CLessDataIndPtr)( const void IrDAFAR *data, int datasz ) ;
static	void	IrDACB	DmyCLessDataInd( const void IrDAFAR *data, int datasz ) ;


#if defined(IrDAPRIMARY)
static	int			lmpICC_ConnectReq( DEVICECONFIG *dc ) ;
static	void		lmpICC_ConnectCnf( DEVICECONFIG *dc, int status ) ;
#endif	
static	int			lmpICC_ConnectInd( DEVICECONFIG *dc ) ;
static	void		lmpICC_DisconnectReq( DEVICECONFIG *dc, int reason ) ;
static	void		lmpICC_DisconnectInd( DEVICECONFIG *dc, int status ) ;
#if !defined(IrLAPNORESET)
static	IrDABOOL	lmpICC_ResetInd( DEVICECONFIG *dc ) ;
#endif	
static	void		lmpICC_StopIdleTimer( DEVICECONFIG *dc ) ;
static	void	IrDACB	lmpICC_IdleTimerExpired( void ) ;


static	void		lmpLSAP_Unbind( int lsap ) ;
#if defined(IrDAPRIMARY)
static	void		lmpLSAP_ConnectCnf( int lsap, int status ) ;
#endif	
static	void		lmpLSAP_DisconnectInd( int lsap, int reason ) ;


#if defined(IrDAPRIMARY)
static	int			lmpSetMissing( void IrDAFAR *des ) ;
#endif	
static	int			lmpSetInteger( void *des, int val ) ;
static	int			lmpSetOctetSequence( void *des, const void IrDAFAR *src, uint size ) ;
static	int			lmpSetUserString( void *des, uint charset, const void IrDAFAR *src, uint size ) ;
static	DEVICECONFIG *lmpInitDeviceConfig( int no ) ;
static	DEVICECONFIG *lmpGetDeviceConfig( dword addr ) ;
static	DEVICECONFIG *lmpMakeDeviceConfig( dword addr ) ;
static	void		lmpFreeDeviceConfig( DEVICECONFIG *dc ) ;


static	LSAPCONFIG	*lmpGetLSAPConfig( int lsap ) ;


static	int			lmpSendPDUConnect( int slsap, byte cmd, byte IrDAFAR *data, int datasz ) ;
static	int			lmpSendPDUDisconnect( int lsap, int reason, void IrDAFAR *data, int datasz ) ;
static	int			lmpSendPDUDisconnect2( int slsap, int dlsap, int reason ) ;
static	int				lmpSendPDUData( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
static	uint	IrDACB	lmpCbSetPDUConnect( byte IrDAFAR *buf, uint bufsz ) ;
static	uint	IrDACB	lmpCbSetPDUDisconnect( byte IrDAFAR *buf, uint bufsz ) ;
static	uint	IrDACB	lmpCbSetPDUDisconnect2( byte IrDAFAR *buf, uint bufsz ) ;
static	uint	IrDACB	lmpCbSetPDUData( byte IrDAFAR *buf, uint bufsz ) ;


#if !defined(IrLMPNOUI)
static	uint	IrDACB	lmpCbSetCLessData( byte IrDAFAR *buf, uint bufsz ) ;
static	void	IrDACB	lmpDmyCLessDataCnf( void ) ;
static	void	(IrDACB	*lmpCLessDataCnfPtr)( void ) ;
#endif	


static	void		lmpSetShortDeviceName( void ) ;


#if	defined(CMSG)
extern	void	StoreCMSG( const byte * ) ;
extern	void	StoreCMSGC( byte ) ;
extern	void	StoreCMSGB( byte ) ;
extern	void	StoreCMSGW( word ) ;
extern	void	StoreCMSGDW( dword ) ;
extern	void	__dspword( byte, dword ) ;
extern	void	__dspdword( byte, dword ) ;
#endif	


#include	"pirlmpsc.h"
#include	"pirlmpcc.h"
#include	"pirlmpls.h"
#include	"pirlmpia.h"


int		IrDAAPI	IrLMP_Open( void )
{
	int		i ;


	
	for( i=0 ; i<IrLMP_SERVICEHINTSZ ; i++ )
		ServiceHint[i]	= 0 ;


	
	IrDA_memset( ShortDeviceName, 0, sizeof( ShortDeviceName )) ;


	
	for( i=0 ; i<DEVICECONFIGMAX ; i++ )
		lmpInitDeviceConfig( i ) ;


	
	disableIdleTimer		= FALSE ;


	
	lmpSC_Initialize() ;


	
	lmpLSAP_Initialize() ;


#if !defined(IrLMPNOUI)
	
	lmpCLessDataCnfPtr		= lmpDmyCLessDataCnf ;
#endif	


	
	IrLAP_Open() ;
	IrLAP_SetConnectInd( lmpCbSC_ConnectInd ) ;
	IrLAP_SetDataInd( lmpCbLSAP_DataInd ) ;
#if !defined(IrLMPNOUI)
	IrLAP_SetUDataInd( lmpCbLSAP_UDataInd ) ;
#endif	
	IrLAP_SetDataCnf( lmpCbLSAP_DataCnf ) ;
	IrLAP_SetDisconnectInd( lmpCbSC_DisconnectInd ) ;
#if !defined(IrLAPNORESET)
	IrLAP_SetResetInd( lmpCbSC_ResetInd ) ;
#endif	
#if !defined(IrLAPNOSNIFF)
	IrLAP_SetSniffInd( &SniffLog, lmpSC_SniffInd ) ;
#endif	


	
	PrvConnect		= 0 ;
	PrvDisconnect	= 0 ;
	CLessDataIndPtr	= DmyCLessDataInd ;
	TxSizePtr		= &DmyTxSize ;
	RxSizePtr		= &DmyRxSize ;


	
	lmpIAS_Open() ;
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::IrLMP_Open(): done\n" ) ;
#endif	


	return( IrDAR_ACCEPT ) ;
}


void	IrDAAPI	IrLMP_Close( void )
{
	
	IrLAP_Close() ;
}


#if !defined(IrLAPNORXCHG)


int		IrDAAPI	IrLMP_SetPrimaryInd( int (IrDACB *indp)( void ))
{
	return( lmpSC_SetPrimaryInd( indp )) ;
}


int		IrDAAPI	IrLMP_PrimaryReq( void (IrDACB *cnfp)( int status ))
{
	return( lmpSC_PrimaryReq( cnfp )) ;
}
#endif	


#if !defined(IrLAPNOSNIFF)


int		IrDAAPI	IrLMP_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void ))
{
	return( lmpSC_SetSniffInd( logp, indp )) ;
}


int		IrDAAPI	IrLMP_SniffReq( void )
{
	return( IrLAP_SniffReq()) ;
}


int		IrDAAPI	IrLMP_SniffAbt( void )
{
	return( IrLAP_SniffAbt()) ;
}
#endif	


void	IrDAAPI	IrLMP_SetConnectPrv( void (IrDACB *func)( void ))
{
	PrvConnect		= func ;
}


void	IrDAAPI	IrLMP_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	PrvDisconnect	= func ;
}


void	IrDAAPI	IrLMP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp )
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


int		IrDAAPI	IrLMP_RequestLSAP( void )
{
	int		ret ;


	ret		= lmpLSAP_Request() ;
	if( ret < 0 )
		return( ret ) ;
	return( ret - 1 ) ;
}


int		IrDAAPI	IrLMP_BindLSAP( int lsap, LMCALLBACK IrDAFAR *lmc )
{
	return( lmpLSAP_Bind( lsap+1, lmc )) ;
}


void	IrDAAPI	IrLMP_UnbindLSAP( int lsap )
{
	lmpLSAP_Unbind( lsap + 1 ) ;
}


int		IrDAAPI	IrLMP_GetRealLSAP( int lsap )
{
	if( lsap <= 0 )
		return( lsap ) ;
	return( lsap - 1 ) ;
}


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrLMP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz ))
{
	return( lmpSC_DiscoverReq( maxslot, log, logsz, func )) ;
}
#endif	


int		IrDAAPI	IrLMP_SetShortDeviceName( uint charset, const byte IrDAFAR *name, uint namesz )
{
	if( namesz > IrLMP_SHORTDEVNMSZ-3 )
		return( IrDAR_TOOLONG ) ;


	lmpSetUserString( ShortDeviceName, charset, name, namesz ) ;
	lmpSetShortDeviceName() ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_SetServiceHint( uint servicehint )
{
	int		i ;
	byte	bit ;


	for( i=0 ;; i++ ) {
		if( i >= IrLMP_SERVICEHINTSZ )
			return( IrDAR_INVALID ) ;
		if( servicehint < 8 )
			break ;
		servicehint	-= 8 ;
	}


	bit		= 1 << servicehint ;
	ServiceHint[i]	|= bit ;
	lmpSetShortDeviceName() ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_ResetServiceHint( uint servicehint )
{
	int		i ;
	byte	bit ;


	for( i=0 ;; i++ ) {
		if( i >= IrLMP_SERVICEHINTSZ )
			return( IrDAR_INVALID ) ;
		if( servicehint < 8 )
			break ;
		servicehint	-= 8 ;
	}


	bit		= 1 << servicehint ;
	ServiceHint[i]	&= ~bit ;
	lmpSetShortDeviceName() ;
	return( IrDAR_ACCEPT ) ;
}


#if defined(IrDAPRIMARY)


IrDABOOL	IrDAAPI	IrLMP_CheckServiceHint( void IrDAFAR *ptr, uint len, uint servicehint )
{
	uint	i, p ;
	int		s ;


	p		= servicehint >> 3 ;
	s		= ((int)1) << ( servicehint & 7 ) ;


	if( p >= len )
		return( FALSE ) ;


	for( i=0 ; i<p ; i++ )
		if((((byte IrDAFAR*)ptr)[i] & 0x80 ) == 0 )
			return( FALSE ) ;


	return(((byte IrDAFAR*)ptr)[p] & s ) ;
}
#endif	


#if defined(IrDAPRIMARY)


uint	IrDAAPI	IrLMP_GetDiscoveryName( void IrDAFAR *ptr, uint len )
{
	uint			i ;
	byte IrDAFAR	*p ;


	p		= (byte IrDAFAR *)ptr ;
	for( i=0 ; i<len ; i++, p++ ) {
		if(( *p ) & 0x80 )
			continue ;
		p	+= 2 ;
		if( i + 2 < len )
			return( i + 2 ) ;
		break ;
	}


	return( len ) ;
}
#endif	


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrLMP_ConnectReq( dword addr, int slsap, int dlsap, void IrDAFAR *data, int datasz )
{
	return( lmpLSAP_ConnectReq( addr, slsap+1, dlsap, data, datasz )) ;
}
#endif	


int		IrDAAPI	IrLMP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz )
{
	return( lmpLSAP_DisconnectReq( lsap+1, reason, data, datasz )) ;
}


int		IrDAAPI	IrLMP_QueryConnectedLsap( void )
{
	return( DeviceConfig[0].LSAPCount ) ;
}


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrLMP_DisconnectLink( void )
{
	int		ret ;


	if(( DeviceConfig[0].Mode & DCM_Used ) == 0 )
		return( IrDAR_NOCONNECT ) ;


#if 0
	if( DeviceConfig[0].LSAPCount > 0 )
		return( IrDAR_CONNECTING ) ;
#endif


	if(( ret = IrLAP_DisconnectReq()) != IrDAR_ACCEPT )
		return( ret ) ;


	lmpCbSC_DisconnectInd( IrDAR_DISCONNECT ) ;
	return( IrDAR_ACCEPT ) ;
}
#endif	


int		IrDAAPI	IrLMP_DisableIdleTimer( void )
{
	disableIdleTimer		= TRUE ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_EnableIdleTimer( void )
{
	disableIdleTimer		= FALSE ;


	if( DeviceConfig[0].Mode & DCM_Used )
		lmpSC_DisconnectReq( &DeviceConfig[0], IrLMP_DR_UnexpectedIrLAPDisconnect ) ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLMP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz )
{
	int		ret ;


#if defined(IrDA_LMPDATAREQPRB)
	IrDA_LMPDATAREQPRB( 0 ) ;
#endif	
	ret		= lmpLSAP_DataReq( lsap+1, bufp, bufsz ) ;
#if defined(IrDA_LMPDATAREQPRB)
	IrDA_LMPDATAREQPRB( 1 ) ;
#endif	
	return( ret ) ;
}


#if !defined(IrLMPNOUI)


int		IrDAAPI	IrLMP_SetCLessDataCnf( void (IrDACB* func)( void ))
{
	return( IrLAP_SetUDataCnf( func )) ;
}
#endif	


#if !defined(IrLMPNOUI)


int		IrDAAPI	IrLMP_SetCLessDataInd( void (IrDACB *func)( const void IrDAFAR* data, int datasz ))
{
	if( func )
		CLessDataIndPtr		= func ;
	else
		CLessDataIndPtr		= DmyCLessDataInd ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrLMPNOUI)
static	const void IrDAFAR *lmpCLessDataPtr ;	
static	uint				lmpCLessDataSize ;	


int		IrDAAPI	IrLMP_CLessDataReq( const void IrDAFAR *bufp, int len )
{
	lmpCLessDataPtr		= bufp ;
	lmpCLessDataSize	= (uint)len ;


	return( IrLAP_UDataReq( lmpCbSetCLessData )) ;
}
#endif	


#if !defined(IrLMPNOUI)


int		IrDAAPI	IrLMP_QueryMaxCLessTxDataSize( void )
{
	return( IrLAP_QueryMaxTxUDataSize() - 2 ) ;
}
#endif	


#if !defined(IrLMPNOUI)


int		IrDAAPI	IrLMP_QueryMaxCLessRxDataSize( void )
{
	return( IrLAP_QueryMaxRxUDataSize() - 2 ) ;
}
#endif	


#if !defined(IrLMPNOUI)
# if !defined(IrDANOSTATIC)
static	uint	IrDACB	lmpCbSetCLessData( byte IrDAFAR *buf, uint bufsz )
{
	if( bufsz > lmpCLessDataSize + 2 )
		bufsz	= lmpCLessDataSize + 2 ;
	buf[0]		= CLESSLSAP ;
	buf[1]		= CLESSLSAP ;
	IrDA_memcpy( &buf[2], lmpCLessDataPtr, bufsz ) ;


	return( 2 + lmpCLessDataSize ) ;
}
# endif	
#endif	


#if !defined(IrLMPNOUI)
# if !defined(IrDANOSTATIC)
static	void	IrDACB	lmpDmyCLessDataCnf( void )
{
}
# endif	
#endif	


uint	IrDAAPI	IrLMP_GetMaxRxSize( int lsap )
{
	int			ret ;


	if( lmpGetLSAPConfig( lsap + 1 ) == 0 )
		return( 0 ) ;


	if(( ret = IrLAP_GetMaxRxSize()) < 0 )
		return( ret ) ;


	if( ret < 2 )
		return( 0 ) ;


	return( ret - 2 ) ;
}


uint	IrDAAPI	IrLMP_GetMaxTxSize( int lsap )
{
	int			ret ;


	if( lmpGetLSAPConfig( lsap + 1 ) == 0 )
		return( 0 ) ;


	if(( ret = IrLAP_GetMaxTxSize()) < 0 )
		return( ret ) ;


	if( ret < 2 )
		return( 0 ) ;


	return( ret - 2 ) ;
}


IrDABOOL	IrDAAPI	IrLMP_QueryTxQueue( void )
{
	return( IrLAP_QueryTxQueue()) ;
}


int		IrDAAPI	IrLMP_QueryVirginTxQueue( void )
{
	return( IrLAP_QueryVirginTxQueue()) ;
}


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	int			lmpSetMissing( void IrDAFAR *des )
{
	((byte IrDAFAR*)des)[0]		= 0x00 ;					
	return( 1 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	int			lmpSetInteger( void *des, int val )
{
	byte	*cdes ;


	cdes	= (byte*)des ;


	cdes[0]	= 0x01 ;								
	MiscSetBigEndian32( &cdes[1], (dword)val ) ;


	return( 1+4 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int			lmpSetOctetSequence( void *des, const void IrDAFAR *src, uint size )
{
	byte	*cdes ;
	int		i ;


	cdes	= (byte*)des ;


	i			= 0 ;
	cdes[i++]	= 2 ;								
	i	+= MiscSetBigEndian16( &cdes[i], (word)size ) ;	
	IrDA_memcpy( &cdes[i], src, size ) ;			
	i	+= size ;


	return( i ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int			lmpSetUserString( void *des, uint charset, const void IrDAFAR *src, uint size )
{
	byte	*cdes ;


	cdes	= (byte*)des ;


	cdes[0]		= 3 ;								
	cdes[1]		= charset ;							
	cdes[2]		= size ;							
	IrDA_memcpy( &cdes[3], src, size ) ;			


	return( 3+size ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	DEVICECONFIG *lmpInitDeviceConfig( int no )
{
	DEVICECONFIG	*dc ;


	dc		= &DeviceConfig[no] ;
	dc->Mode			= 0 ;
	dc->Address			= 0 ;
	dc->ICCStatus		= ICC_STANDBY ;
	dc->LSAPCount		= 0 ;
	dc->IdleTimerHndl	= -1 ;
	return( dc ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	DEVICECONFIG *lmpGetDeviceConfig( dword addr )
{
	int				i ;
	DEVICECONFIG	*dc ;


	for( i=0 ; i<DEVICECONFIGMAX ; i++ ) {
		dc		= &DeviceConfig[i] ;
		if(( dc->Mode & DCM_Used ) == 0 )
			continue ;
		if( addr == (dword)0xFFFFFFFFL || dc->Address == addr )
			return( dc ) ;
	}


	return( 0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	DEVICECONFIG *lmpMakeDeviceConfig( dword addr )
{
	int				i ;
	DEVICECONFIG	*dc ;


	if(( dc = lmpGetDeviceConfig( addr )) != (DEVICECONFIG*)0 )
		return( dc ) ;


	for( i=0 ; i<DEVICECONFIGMAX ; i++ ) {
		dc		= &DeviceConfig[i] ;
		if( dc->Mode & DCM_Used )
			continue ;
		dc->Address			= addr ;
		dc->ICCStatus		= ICC_STANDBY ;
		dc->Mode			|= DCM_Used ;
		dc->LSAPCount		= 0 ;
		dc->IdleTimerHndl	= -1 ;
		return( dc ) ;
	}


	return( 0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpFreeDeviceConfig( DEVICECONFIG *dc )
{
	dc->Mode	&= ~DCM_Used ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	int			lmpConnectLsap ;		
static	byte		lmpConnectCmd ;			
static	byte IrDAFAR *lmpConnectData ;		
static	int			lmpConnectDataSize ;	


static	int			lmpSendPDUConnect( int slsap, byte cmd, byte IrDAFAR *data, int datasz )
{
	lmpConnectLsap		= slsap ;
	lmpConnectCmd		= cmd ;
	lmpConnectData		= data ;
	lmpConnectDataSize	= datasz ;
	return( IrLAP_DataReq( lmpCbSetPDUConnect )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	IrDACB	lmpCbSetPDUConnect( byte IrDAFAR *buf, uint bufsz )
{
	UNREFERENCE( bufsz ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSetPDUConnect():" ) ;
	_DPUTW( DL_LMPINFO, " lmpConnectDataSize:", lmpConnectDataSize ) ;
	_DPUTW( DL_LMPINFO, " bufsz:", bufsz ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( lmpConnectDataSize > (int)( bufsz - 4 ) )
		lmpConnectDataSize	= (int)( bufsz - 4 );


	buf[0]		= LSAPConfig[lmpConnectLsap].dLSAP | 0x80 ;
	buf[1]		= lmpConnectLsap & 0x7F ;
	buf[2]		= LMPDU_Connect | lmpConnectCmd ;
	buf[3]		= 0 ;
	IrDA_memcpy( &buf[4], lmpConnectData, lmpConnectDataSize ) ;
	return( 4 + lmpConnectDataSize ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	int			lmpDiscLsap ;		
static	int			lmpDiscReason ;		
static	void IrDAFAR *lmpDiscData ;		
static	int			lmpDiscDataSize ;	


static	int			lmpSendPDUDisconnect( int lsap, int reason, void IrDAFAR *data, int datasz )
{
	lmpDiscLsap		= lsap ;
	lmpDiscReason	= reason ;
	lmpDiscData		= data ;
	lmpDiscDataSize	= datasz ;
	return( IrLAP_DataReq( lmpCbSetPDUDisconnect )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	IrDACB	lmpCbSetPDUDisconnect( byte IrDAFAR *buf, uint bufsz )
{
	UNREFERENCE( bufsz ) ;


	buf[0]		= LSAPConfig[lmpDiscLsap].dLSAP | 0x80 ;
	buf[1]		= lmpDiscLsap & 0x7F ;
	buf[2]		= LMPDU_Disconnect ;
	buf[3]		= (byte)lmpDiscReason ;
	IrDA_memcpy( &buf[4], lmpDiscData, lmpDiscDataSize ) ;
	return( 4 + lmpDiscDataSize ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	int			lmpDisc2SLsap ;		
static	int			lmpDisc2DLsap ;		
static	int			lmpDisc2Reason ;	


static	int			lmpSendPDUDisconnect2( int slsap, int dlsap, int reason )
{
	lmpDisc2SLsap		= slsap ;
	lmpDisc2DLsap		= dlsap ;
	lmpDisc2Reason		= reason ;
	return( IrLAP_DataReq( lmpCbSetPDUDisconnect2 )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	IrDACB	lmpCbSetPDUDisconnect2( byte IrDAFAR *buf, uint bufsz )
{
	UNREFERENCE( bufsz ) ;


	*buf++		= lmpDisc2DLsap | 0x80 ;
	*buf++		= lmpDisc2SLsap & 0x7F ;
	*buf++		= LMPDU_Disconnect ;
	*buf++		= (byte)lmpDisc2Reason ;
	return( 4 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	int					lmpDataLsap ;	
static	const void IrDAFAR *lmpDataPtr ;	
static	uint				lmpDataSize ;	


static	int			lmpSendPDUData( int lsap, const void IrDAFAR *bufp, int bufsz )
{
	lmpDataLsap	= lsap ;
	lmpDataPtr	= bufp ;
	lmpDataSize	= (uint)bufsz ;


	return( IrLAP_DataReq( lmpCbSetPDUData )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint	IrDACB lmpCbSetPDUData( byte IrDAFAR *buf, uint bufsz )
{
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSetPDUData():" ) ;
	_DPUTW( DL_LMPINFO, " lmpDataSize:", lmpDataSize ) ;
	_DPUTW( DL_LMPINFO, " bufsz:", bufsz ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( bufsz > lmpDataSize )
		bufsz	= lmpDataSize ;
	buf[0]		= LSAPConfig[lmpDataLsap].dLSAP & 0x7F ;
	buf[1]		= lmpDataLsap & 0x7F ;
	IrDA_memcpy( &buf[2], lmpDataPtr, bufsz ) ;


	*TxSizePtr	+= bufsz ;
	return( 2 + lmpDataSize ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	LSAPCONFIG	*lmpGetLSAPConfig( int lsap )
{
	LSAPCONFIG	*lc ;


	if( lsap > LSAPMAX )
		return( 0 ) ;


	lc		= &LSAPConfig[lsap] ;
	if(( lc->Mode & LCM_Used ) == 0 )
		return( 0 ) ;


	return( lc ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpSetShortDeviceName( void )
{
	int				i ;
	int				sz ;


	for( sz=0 ; sz<IrLMP_SERVICEHINTSZ && sz<IrLAP_NNMSZ ; sz++ ) {
		IrLAPDeviceName[sz]		= ServiceHint[sz] ;
		for( i=sz+1 ; i<IrLMP_SERVICEHINTSZ ; i++ ) {
			if( ServiceHint[i] )
				break ;
		}
		if( i < IrLMP_SERVICEHINTSZ ) {
			IrLAPDeviceName[sz]	|= 0x80 ;
		} else {
			sz	++ ;
			break ;
		}
	}
	IrLAPDeviceName[sz++]		= ShortDeviceName[1] ;
	IrDA_memcpy( &IrLAPDeviceName[sz], &ShortDeviceName[3], ShortDeviceName[2] ) ;
	sz		+= ShortDeviceName[2] ;
#if	defined(DMSG2)
	_DPUTS( DL_LMPWARN, " LMP::lmpSetShortDeviceName(): len=%d\n", sz ) ;
	_DPUTS( DL_LMPWARN, "                          : dat=" ) ;
	for( i=0 ; i<sz ; i++ )
		_DPUTS( DL_LMPWARN, "%02x ", IrLAPDeviceName[i] ) ;
	_DPUTS( DL_LMPWARN, "\n" ) ;
#endif	
	IrLAP_SetDeviceName( IrLAPDeviceName, sz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	IrDACB	DmyCLessDataInd( const void IrDAFAR *data, int datasz )
{
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif	


#endif	


/* $Log: rcs/IRLMP.CV $
 * Revision 0.1  96/07/14 00:00:00  core
 * 
*/
