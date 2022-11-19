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
;*             File:       irlapm.c                                     *
;*             Created:    07/14/96                                     *
;*             Author:     Shin-ichi Nagamura                           *
;*                                                                      *
;*             Version:    0.0                                          *
;*             Assembler:  Turbo Assembler  Version 2.0                 *
;*                                                                      *
;***********************************************************************/

/* $Header: IRLAPM.CV  0.1  96/07/14 00:00:00  core  Exp $ */


#include	"irdaconf.h"
#include	"string.h"
#include    "memory.h"


#include	"plapbm.h"
//#include	"plapbs.h"		for my compiler
// this code is plapbs.h



#if !defined(__plapbs_h__)
#define	__plapbs_h__


#define		NOISEND				1
#if  !defined(IrLAPNONEWADDR)
# if !defined(AUTONEWADDRHIST)
#  define		AUTONEWADDRHIST		16
# endif	
#endif	


#include	"irda.h"
#include	"irlap.h"
#include	"irsir.h"
#include	"hlap.h"
#include	"okmisc.h"
#include	"timer.h"
#if	defined(VMSG)
#include	"dos.h"
#define	VRAMP(x,y)	(MK_FP( 0xB800, ((80*(y))+(x))*2))
#define	VRAMD(c,a)	((c) + ((a)<<8))
#endif	


#if !defined(IrLAPMAXINFRAMERETRYCOUNT)
# define	IrLAPMAXINFRAMERETRYCOUNT		10
#endif	
#if !defined(TIME_SNIFFSENSE)
# define	TIME_SNIFFSENSE					200L
#endif	
#if !defined(TIME_SNIFF)
# define	TIME_SNIFF						200L
#endif	
#if !defined(TIME_SLEEP)
# define	TIME_SLEEP						2000L
#endif	


#if defined(_INC_WINDOWS)
#define	__IrLAP_CatchDataInd		IrLAP_WINCatchDataInd
#define	lapCbInitConnState		IrLAP_WINInitConnState
#else	
#define	__IrLAP_CatchDataInd		lapCbCatchDataInd
#define	lapCbInitConnState		lapCbInitConnState
#endif	


static	uint				LAP_TxFrameSize ;				
static	uint				LAP_TxWindowSize ;				
		LAPSENDCTL	IrDAFAR	LAP_SendCtl[IrLAPMAXTXWINSIZE] ;	


		uint		IrDAFAR	LAP_SendWinReadPtr ;			
		uint		IrDAFAR	LAP_SendWinSendPtr ;			
		uint		IrDAFAR	LAP_SendWinWritePtr ;			
volatile uint		IrDAFAR	LAP_SendWinSendSize ;			
static	uint				LAP_TxLeaveWindow ;				


static	uint				LAP_RxWIndowSize ;		
static	uint				LAP_RxFrameSize ;		
		uint		IrDAFAR	RecvFrameReadPtr ;		
#if	defined(DMSG)
static	ulong				FrameNo ;
#endif	


static	PORTCONFIG2		MyPortConfig ;				
static	dword			MyAddress ;					


static	PORTCONFIG2		TargetPortConfig ;			


static	dword			TargetAddress ;				
static	uint			ConnSendFrameSize ;			
static	uint			ConnSendWindowSize ;		
static	dword			ConnDisconnectMSec ;		
static	int				ConnRetryCountN1 ;			
static	int				ConnRetryCountN2 ;			
static	int				ConnRetryCountN3 ;			
		dword	IrDAFAR	IrLAP_ConnSpeed ;			
static	IrDABOOL	IrDAFAR	IrLAP_ConnMaxTAT ;			
		IrDABOOL	IrDAFAR	IrLAP_ConnRemoteBusy ;		
		int		IrDAFAR	IrLAP_ConnRetryCount ;		
		uint	IrDAFAR	IrLAP_ConnectionAddress ;	
		uint	IrDAFAR	IrLAP_ConnBOFSize ;			
		uint	IrDAFAR	IrLAP_ConnMinTurnAround ;	
static	int				InFrameRetryCounter ;		
static	int				MaxInFrameRetryCount ;		


static	volatile	LAPFLAGS	lapFlags ;			
static	volatile	LAPSNRMPRV	lapSnrmPrv ;		
		uint	IrLAP_Stage ;						
static	int		IrLAP_OriginalPowerMode ;			


#define	TIME_QUERYTIMER		500L


#if !defined(TIME_PTIMER)
#define	TIME_PTIMER			400L
#endif	
static	int				SlotTimerHndl ;			
static	int				QueryTimerHndl ;		
static	int				FTimerHndl ;			
static	int				PTimerHndl ;			
static	int				WDTimerHndl ;			
static	int				LTimerHndl ;			
#if	defined(IrDA_DELAIEDSECONDARY)
static	int				DTimerHndl ;			
static	dword			DelayTimer ;			
#endif	


static	IrDABOOL		EnableDiscoveryAnswer ;		
static	const byte	IrDAFAR	*LAP_DiscoveryName ;	
static	uint			LAP_DiscoveryNameSize ;		
static	uint			DiscoverySlot ;				
static	uint			DiscoveryMySlot ;			
static	IrDABOOL		DiscoveryFrameSent ;		
static	dword			NewAddress ;				
static	DISCOVERYLOG IrDAFAR	*PrvDiscLog ;		
static	DISCOVERYLOG IrDAFAR	*AplDiscLog ;		


static	dword				SrcAddress ;			
static	dword				DesAddress ;			


		uint	IrDAFAR	IrLAP_Nr ;					
		uint	IrDAFAR	IrLAP_Ns ;					
		uint	IrDAFAR	IrLAP_Vr ;					
		uint	IrDAFAR	IrLAP_Vs ;					


static	void	(IrDACB *DiscoveryCnfPtr)( int status, uint logsz ) ;
static	int		(IrDACB *ConnectIndPtr)( dword addr ) ;
static	void	(IrDACB *DisconnectIndPtr)( int status ) ;
static	void	(IrDACB *DataCnfPtr)( uint freebufsz ) ;
static	void	(IrDACB *DataIndPtr)( const byte IrDAFAR *data, uint sz ) ;
#if !defined(IrLAPNORESET)
static	IrDABOOL	(IrDACB *ResetIndPtr)( void ) ;
#endif	
static	IrDABOOL	(IrDACB *AplConnectIndPtr)( void ) ;


#if !defined(IrLAPNOUI)
static	void	(IrDACB *UDataIndPtr)( const byte IrDAFAR *data, uint sz ) ;
static	void	(IrDACB *UDataCnfPtr)( void ) ;
static	void	IrDACB	lapCbUIDataCnf( void ) ;
static	void	IrDACB	lapDmyUDataCnf( void ) ;
static	void	IrDACB	lapDmyUDataInd( const byte IrDAFAR *data, uint sz ) ;
#endif	


static	void	(IrDACB *DiscoveryPrvPtr)( void ) ;
static	void	(IrDACB	*ConnectPrvPtr)( void ) ;
static	void	(IrDACB	*ConnectPrvPtr2)( int type ) ;
static	void	(IrDACB	*DisconnectPrvPtr)( void ) ;
static	ulong IrDAFAR	*TxSizePtr ;	
static	ulong IrDAFAR	*RxSizePtr ;	
static	ulong	DmyTxSize ;				
static	ulong	DmyRxSize ;				


static	void	(IrDACB *AplLinkIndPtr)( void ) ;
static	dword	AplLinkIndTime ;		
static	void	(IrDACB *TxEmptyPrvPtr)( void ) ;


#if !defined(IrLAPNOSNIFF)
# if defined(IrDAPRIMARY)
static	int				ListenTimerHndl ;				
# endif	
static	void			(IrDACB *SniffIndPtr)( void ) ;
static	DISCOVERYLOG IrDAFAR	*SniffDiscLog ;			
static	int				SenseTimerHndl ;				
static	int				SniffTimerHndl ;				
static	int				SleepTimerHndl ;				
static	void			lapStartSenseTimer( void ) ;
static	void			lapStopSenseTimer( void ) ;
static	void	IrDACB	lapCbStartSniffTimer( void ) ;
static	void			lapStopSniffTimer( void ) ;
static	void	IrDACB	lapSniffTimerExpired( void ) ;
static	void	IrDACB	lapCbSenseTimerExpired( void ) ;
static	void			lapStartSleepTimer( void ) ;
static	void			lapStopSleepTimer( void ) ;
static	void	IrDACB	lapSleepTimerExpired( void ) ;
static	void	IrDACB	lapDmySniffInd( void ) ;
#endif	


static	void	IrDACB	lapDmyDiscoveryCnf( int status, uint logsz ) ;
static	int		IrDACB	lapDmyConnectInd( dword addr ) ;
static	void	IrDACB	lapDmyDisconnectInd( int state ) ;
static	void	IrDACB	lapDmyDataCnf( uint freebufsz ) ;
static	void	IrDACB	lapDmyDataInd( const byte IrDAFAR *data, uint sz ) ;
#if !defined(IrLAPNORESET)
static	IrDABOOL IrDACB	lapDmyResetInd( void ) ;
#endif	


static	void	IrDACB	lapDmyDiscoveryPrv( void ) ;
static	void	IrDACB	lapDmyConnectPrv( void ) ;
static	void	IrDACB	lapDmyConnectPrv2( int type ) ;
static	void	IrDACB	lapDmyDisconnectPrv( void ) ;


static	IrDABOOL	IrDACB	lapDmyAplConnectInd( void ) ;
static	void	IrDACB	lapDmyAplLinkInd( void ) ;
static	void	IrDACB	lapDmyTxEmptyPrv( void ) ;
static	void	IrDACB	lapDmySnrmPrv( void ) ;


static	void	IrDACB	lapCbCatchDataInd( void ) ;
static	void	IrDACB	lapCbInitConnState( void ) ;
static	void	IrDACB	lapCbDefaultConnState( void ) ;
static	void	IrDACB	lapCbSetRecvMode( void ) ;
static	void	IrDACB	lapCbSlowDownPower( void ) ;
static	void	IrDACB	lapCbSendRNRRsp( void ) ;
static	void	IrDACB	lapCbStartWDTimer( void ) ;
static	void			lapStartLTimer( void ) ;
static	void	IrDACB	lapCbQueryTimerExpired( void ) ;
static	void	IrDACB	lapCbWDTimerExpired( void ) ;
static	void	IrDACB	lapCbLTimerExpired( void ) ;
static	void	IrDACB	lapSendSLeaveWindow( void ) ;
#if !defined(IrLAPNORESET)
static	void	IrDACB	lapSendUARspCnf( void ) ;
#endif	
#if defined(IrDA_DELAIEDSECONDARY)
static	void	IrDACB	lapDTimerExpired( void ) ;
#endif	


static	void		lapInitCallBackFunctions( void ) ;


static	void		lapResetRxBuffer( void ) ;
static	void		lapStopAllTimers( void ) ;


static	void		lapApplyDefaultConnParam( void ) ;
static	void		lapReleaseBufferedData( void ) ;


static	int			lapGetRecvEvent( const byte IrDAFAR *frame, uint framesz ) ;
static	uint		lapGetUFrameType( const byte IrDAFAR *frame, uint framesz ) ;
static	uint		lapGetSFrameType( const byte IrDAFAR *frame, uint framesz ) ;


static	void		lapStateToNDM( int status ) ;
static	void		lapStateToSRecv( void ) ;


static	int			lapCallConnectInd( void ) ;
static	void		lapCallDataInd( const byte IrDAFAR *frame, uint framesz ) ;
#if !defined(IrLAPNOUI)
static	void		lapCallUDataInd( const byte IrDAFAR *frame, uint framesz ) ;
#endif	
static	void		lapCallDataCnf( void ) ;
#if !defined(IrLAPNORESET)
static	IrDABOOL	lapCallResetInd( void ) ;
#endif	
static	void		lapCallDisconnectInd( int status ) ;


static	dword		lapGetAddress( const byte IrDAFAR *bufp ) ;
static	dword		lapSetAddress( byte IrDAFAR *bufp, dword addr ) ;
static	IrDABOOL	lapIsRecvAddress( dword addr ) ;
static	void		lapSetupMyDeviceAddress( IrDABOOL useslot ) ;
static	dword		lapMakeDeviceAddressBySlot( void ) ;
static	void		lapGetDiscoveryInfo( DISCOVERYLOG IrDAFAR *des, const byte IrDAFAR *frame, uint framesz ) ;


static	void		lapInitCapacityParam( PORTCONFIG2 *cap ) ;
static	void		lapGetCapacityParam( PORTCONFIG2 IrDAFAR *cap, const byte IrDAFAR *src, uint len ) ;
static	word		lapSetCapacityParam( byte IrDAFAR *des, const PORTCONFIG2 *cap ) ;
static	void		lapNegotiateConnectionParameters( PORTCONFIG2 *des, const PORTCONFIG2 *src ) ;
static	word		lapMakeBestBit( word *des, word src ) ;
static	word		lapSetCapacity( byte IrDAFAR *des, byte sig, word data ) ;
static	void		lapApplyConnectionParameters( void ) ;
static	dword		lapGetLineCapability( word c ) ;
static	word		lapGetMaxTanaroundTime( word c ) ;
static	uint		lapGetBOFSize( word c ) ;
static	dword		lapGetDisconnectMSec( word c ) ;
static	uint		lapGetFrameSize( word c ) ;
static	uint		lapGetWindowSize( word c ) ;
static	uint		lapGetMinTurnaroundTime( word c ) ;


static	uint		lapGetXIDSlotSize( byte c ) ;


static	void		lapSendTESTRsp( const byte IrDAFAR *frame, uint framesz ) ;
#if defined(IrDA_DELAIEDSECONDARY)
static	void		lapDelaySendRRRsp( void ) ;
#endif	
static	void		lapSendRNRRsp( void ) ;


static	void		lapSendIFrame( byte pf, byte crf ) ;
static	void		lapResendIFrame( void ) ;


static	void		lapUpdateNrReceived( void ) ;
static	uint		lapNextSendWindowPtr( uint ptr ) ;
static	IrDABOOL	lapChkNs( void ) ;
static	IrDABOOL	lapChkNr( void ) ;
static	IrDABOOL	lapInvalidNr( void ) ;
static	IrDABOOL	lapInvalidNs( void ) ;
static	IrDABOOL	lapChkDataRequest( void ) ;


static	void		lapStopSlotTimer( void ) ;
static	void		lapStopQueryTimer( void ) ;
static	void		lapStopFTimer( void ) ;
static	void		lapStopPTimer( void ) ;
static	void		lapStopWDTimer( void ) ;
static	void		lapStopLTimer( void ) ;
#if defined(IrDA_DELAIEDSECONDARY)
static	void		lapStopDTimer( void ) ;
#endif	


static	void	lapRE_MissStage( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_NDMXXXCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_NDMXIDCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_NDMSNRMCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_NDMUICmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_NDMUIRsp		lapRE_NDMUICmd
static	void	lapRE_REPLYXIDCmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_REPLYSNRMCmd	lapRE_NDMSNRMCmd
static	void	lapRE_SBusyWaitDISCCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SBusyWaitUICmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SBusyWaitXIDCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SBusyWaitRRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SBusyWaitRNRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SBusyWaitICmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SCloseDISCCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SCloseDMRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SCloseXXX( const byte IrDAFAR *frame, uint framesz ) ;
#if !defined(IrDAPRIMARY)
#define			lapRE_NDMXIDRsp		lapRE_MissStage
#endif	
#if !defined(IrLAPNOSNIFF)
static	void	lapRE_PoutXIDCmd( const byte IrDAFAR *frame, uint framesz ) ;
# define		lapRE_SniffXIDCmd	lapRE_PoutXIDCmd
static	void	lapRE_SniffSNRMCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SniffXXX( const byte IrDAFAR *frame, uint framesz ) ;
#else	
# define		lapRE_PoutXIDCmd	lapRE_MissStage
# define		lapRE_SniffXIDCmd	lapRE_MissStage
# define		lapRE_SniffXXX		lapRE_MissStage
# define		lapRE_SniffSNRMCmd	lapRE_MissStage
#endif	
#if !defined(IrLAPNORXCHG)
static	IrDABOOL	lapChkPrimaryReq( void ) ;
#endif	


//#if defined(IrDASECONDARY)
//#include	"plapss.h"
//#else	
//#include	"plapss.h"
//#endif								for my compiler
// this code is plapss.h



#if !defined(__plapss_h__)
#define	__plapss_h__


static	void	lapRE_SRecvDISCCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRecvSNRMCmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SRecvTESTCmd	lapSendTESTRsp
static	void	lapRE_SRecvUICmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRecvRRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRecvRNRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRecvREJCmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SRecvSREJCmd	lapRE_SRecvREJCmd
static	void	lapRE_SRecvICmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRecvUnknownCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SErrorMiss( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SErrorDISCCmd	lapRE_SRecvDISCCmd
static	void	lapRE_SErrorDMRsp( const byte IrDAFAR *frame, uint framesz ) ;


static	void		lapKickTxIFrame2( void ) ;
static	IrDABOOL	lapChkSecondaryDiscReq( void ) ;


static	void		lapSendXIDRsp( dword des, byte discoveryflag, int slotno ) ;
#if !defined(IrLAPNOSNIFF)
static	void		lapSendSniffXIDRsp( void ) ;
#endif	
static	void		lapSendRRRsp( void ) ;											
static	void		lapSendRDRsp( void ) ;											
static	void		lapSendUARsp( IrDABOOL flag ) ;									
static	void		lapSendDMRsp( uint ca ) ;										
static	void		lapSendFRMRRsp( byte type ) ;


#if !defined(IrDANOSTATIC)


static	void		lapRE_SRecvDISCCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
	IrSIR_SetDataCnf( lapCbDefaultConnState ) ;
	lapSendUARsp( OFF ) ;								


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRecvDISCCmd(): Link Shutdown\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SRecvSNRMCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopAllTimers() ;								
#if !defined(IrLAPNORESET)
	if( lapCallResetInd()) {
		IrSIR_SetDataCnf( lapSendUARspCnf ) ;
		lapSendUARsp( ON ) ;							
	} else {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRDRsp() ;
	}
#else	
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRDRsp() ;
#endif	
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRecvSNRMCmd():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SRecvUICmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopWDTimer() ;									


#if !defined(IrLAPNOUI)
	lapCallUDataInd( frame, framesz ) ;				
#endif	
	if( frame[1] & IrLAP_PF ) {
		if( lapChkDataRequest())
#if !defined(NOISEND)
			IrLAP_Stage			= IrLAP_S_NDM ;
#else
			;
#endif	
		else
			lapSendRRRsp() ;							
	}


	lapCbStartWDTimer() ;							
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRecvUICmd():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SRecvRRCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;								


	IrLAP_ConnRemoteBusy	= FALSE ;				
	lapUpdateNrReceived() ;							
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	if( !lapChkNr()) {								
		if( LAP_SendWinSendSize > 0 ) {
			lapResendIFrame() ;						
			return ;
		}


		lapSendFRMRRsp( FRMR_InvalidNr ) ;
		return ;
	}


	if( lapChkDataRequest()) {
		lapKickTxIFrame2() ;
		return ;
	}
	if( lapChkSecondaryDiscReq())
		return ;
#if !defined(IrLAPNORXCHG)
	if( lapChkPrimaryReq())
		return ;
#endif	
	lapSendRRRsp() ;									
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SRecvRNRCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;								
	IrLAP_ConnRemoteBusy	= TRUE ;				


	lapUpdateNrReceived() ;							
	if(( frame[1] & IrLAP_PF ) == 0 ) {
#if	defined(DMSG3)
		_DPUTS( DL_LAPDANGER, " LAP::lapRE_SRecvRNRCmd():" ) ;
		_DPUTS( DL_LAPDANGER, " RECEIVED NON P/F RRCmd" ) ;
		_DPUTS( DL_LAPDANGER, "\n" ) ;
#endif	
													
		lapCbStartWDTimer() ;						
		return ;
	}


	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	if( lapChkSecondaryDiscReq())
		return ;


	if( !lapChkNr() && LAP_SendWinSendSize == 0 ) {	
		lapSendFRMRRsp( FRMR_InvalidNr ) ;
		return ;
	}


#if !defined(IrLAPNORXCHG)
	if( lapChkPrimaryReq())
		return ;
#endif	
	lapSendRRRsp() ;									
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_SRecvREJCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									


	lapUpdateNrReceived() ;							
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	if( IrLAP_ConnRemoteBusy ) {
		lapSendRRRsp() ;
		return ;
	}


	if( LAP_SendWinSendSize > 0 ) {
		lapResendIFrame() ;							
		return ;
	}


	lapSendFRMRRsp( FRMR_InvalidNr ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_SRecvICmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopWDTimer() ;									
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRecvICmd():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTB( DL_LAPINFO, " Vs:", IrLAP_Vs ) ;
	_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
	_DPUTB( DL_LAPINFO, " Ns:", IrLAP_Ns ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	if( frame[1] & IrLAP_PF ) {
		if( lapChkSecondaryDiscReq())
			return ;
	}


	if( lapInvalidNr()) {
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendFRMRRsp( FRMR_InvalidNr ) ;
			return ;
		}
		IrLAP_Stage		= IrLAP_S_SError ;
		lapCbStartWDTimer() ;
		return ;
	}


	if( lapInvalidNs()) {
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendFRMRRsp( FRMR_InvalidNs ) ;
			return ;
		}
		IrLAP_Stage		= IrLAP_S_SError ;
		lapCbStartWDTimer() ;
		return ;
	}


	if( lapChkNs() && lapChkNr()) {
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
		lapUpdateNrReceived() ;						
		lapCallDataInd( frame, framesz ) ;				
		if( frame[1] & IrLAP_PF ) {
			if( lapChkDataRequest()) {
				lapKickTxIFrame2() ;
				return ;
			}
#if !defined(IrLAPNORXCHG)
			if( lapChkPrimaryReq())
				return ;
#endif	
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendRRRsp() ;							
			return ;
		}


		lapCbStartWDTimer() ;
		return ;
	}


	if( !lapChkNs() && lapChkNr()) {
		lapUpdateNrReceived() ;						
		if( frame[1] & IrLAP_PF ) {
#if !defined(IrLAPNORXCHG)
			if( lapChkPrimaryReq())
				return ;
#endif	
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendRRRsp() ;							
			return ;
		}
		lapCbStartWDTimer() ;
		return ;
	}


	if( lapChkNs() && !lapChkNr()) {
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
		lapUpdateNrReceived() ;						
		lapCallDataInd( frame, framesz ) ;				
		if(( frame[1] & IrLAP_PF ) == 0 ) {
			lapCbStartWDTimer() ;
			return ;
		}


		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		if( IrLAP_ConnRemoteBusy ) {
#if !defined(IrLAPNORXCHG)
			if( lapChkPrimaryReq())
				return ;
#endif	
			lapSendRRRsp() ;
			return ;
		}


		if( LAP_SendWinSendSize > 0 ) {
			lapResendIFrame() ;						
			return ;
		}


		lapSendFRMRRsp( FRMR_InvalidNr ) ;
		return ;
	}


	if( frame[1] & IrLAP_PF ) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		if( IrLAP_ConnRemoteBusy ) {
#if !defined(IrLAPNORXCHG)
			if( lapChkPrimaryReq())
				return ;
#endif	
			lapSendRRRsp() ;
			return ;
		}


		if( LAP_SendWinSendSize > 0 ) {
			lapResendIFrame() ;						
			return ;
		}


		lapSendFRMRRsp( FRMR_InvalidNs ) ;
		return ;
	}


	lapCbStartWDTimer() ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	lapRE_SRecvUnknownCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
#if	defined(DMSG3)
	{	int		i ;
		_DPUTS( DL_LAPDANGER, " LAP::lapRE_SRecvUnknownCmd():" ) ;
		_DPUTW( DL_LAPDANGER, " size:", framesz ) ;
		_DPUTS( DL_LAPDANGER, " data=" ) ;
		for( i=0 ; i<framesz ; i++ )
			_DPUTB( DL_LAPDANGER, " ", frame[i] ) ;
		_DPUTS( DL_LAPDANGER, "\n" ) ;
	}
#endif	
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;
		return ;
	}


	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendFRMRRsp( FRMR_FuterNotAvilable ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	lapRE_SErrorMiss( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;
		return ;
	}


	IrLAP_Stage		= IrLAP_S_SRecv ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendFRMRRsp( FRMR_InvalidNs ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	lapRE_SErrorDMRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;
		return ;
	}


	lapStateToNDM( IrDAR_DISCONNECT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapKickTxIFrame2( void )
{
#if	defined(NOTXMULTIWINDOW)
	LAP_TxLeaveWindow		= 1 ;
#else	
	LAP_TxLeaveWindow		= ConnSendWindowSize ;
#endif	
	lapSendSLeaveWindow() ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	IrDABOOL	lapChkSecondaryDiscReq( void )
{
	if( !lapFlags.disconnectReq )
		return( FALSE ) ;


	lapFlags.disconnectReq	= FALSE ;
	lapReleaseBufferedData() ;						
	IrLAP_ConnRetryCount	= 0 ;					
	IrLAP_Stage				= IrLAP_S_SClose ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRDRsp() ;								
	return( TRUE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendXIDRsp( dword des, byte discoveryflag, int slotno )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendXIDRsp():" ) ;
	_DPUTD( DL_LAPINFO, " des:", des ) ;
	_DPUTB( DL_LAPINFO, " discflag:", discoveryflag ) ;
	_DPUTB( DL_LAPINFO, " slotno:", slotno ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]					= IrLAP_BroadCast << 1 ;
	LAP_SendCmdBuffer[1]					= IrLAP_XIDRsp | IrLAP_PF ;
	LAP_SendCmdBuffer[XID_FI]				= 0x01 ;
	lapSetAddress( &LAP_SendCmdBuffer[XID_SrcAddress], NewAddress ) ;
	lapSetAddress( &LAP_SendCmdBuffer[XID_DesAddress], des ) ;
	LAP_SendCmdBuffer[XID_DiscoveryFlag]	= discoveryflag ;
	LAP_SendCmdBuffer[XID_SlotNo]			= (byte)slotno ;
	LAP_SendCmdBuffer[XID_Version]			= 0 ;
	IrDA_memcpy( &LAP_SendCmdBuffer[XID_Info], LAP_DiscoveryName, LAP_DiscoveryNameSize ) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( XID_Info + LAP_DiscoveryNameSize, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendRRRsp( void )
{
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapSendRRRsp():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= IrLAP_ConnectionAddress << 1 ;
	LAP_SendCmdBuffer[1]	= IrLAP_RRRsp | IrLAP_PF | ( IrLAP_Vr << 5 ) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendRDRsp( void )
{
	LAP_SendCmdBuffer[0]	= IrLAP_ConnectionAddress << 1 ;
	LAP_SendCmdBuffer[1]	= IrLAP_RDRsp | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			lapSendUARsp( IrDABOOL flag )
{
	uint		len ;


	LAP_SendCmdBuffer[0]					= IrLAP_ConnectionAddress << 1 ;
	LAP_SendCmdBuffer[1]					= IrLAP_UARsp | IrLAP_PF ;
	lapSetAddress( &LAP_SendCmdBuffer[UA_SrcAddress], MyAddress ) ;
	lapSetAddress( &LAP_SendCmdBuffer[UA_DesAddress], TargetAddress ) ;


	len		= UA_NegotiationParm ;
	if( flag )
		len	+= lapSetCapacityParam( &LAP_SendCmdBuffer[UA_NegotiationParm], &MyPortConfig ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendUARsp():\n" ) ;
#endif	
	lapResetRxBuffer() ;
	IrSIR_SendFrame( len, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendDMRsp( uint ca )
{
	LAP_SendCmdBuffer[0]	= ca << 1 ;
	LAP_SendCmdBuffer[1]	= IrLAP_DMRsp | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendFRMRRsp( byte type )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendFRMRRsp():" ) ;
	_DPUTB( DL_LAPINFO, " type:", type ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= IrLAP_ConnectionAddress << 1 ;
	LAP_SendCmdBuffer[1]	= IrLAP_FRMRRsp | IrLAP_PF ;
	LAP_SendCmdBuffer[FRMR_RejectFrameCtrlField]	= 0 ;
	LAP_SendCmdBuffer[FRMR_Field1]	= ( IrLAP_Nr << 5 ) | IrLAP_CR | ( IrLAP_Ns << 1 ) ;
	LAP_SendCmdBuffer[FRMR_Field2]	= type ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( FRMR_Field2+1, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapSendSniffXIDRsp( void )
{
	NewAddress			= MyAddress ;
	lapFlags.setNewAddr	= FALSE ;
	lapSendXIDRsp( 0xFFFFFFFFL, 0, 0 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapDmySniffInd( void )
{
}
#endif	
#endif	


#endif	

#if defined(IrDAPRIMARY)
#include	"plapps.h"
#endif	


static	void	(* const IrLAP_RecvEventFunc[IrLAP_S_MAX][IrLAP_RE_MAX])( const byte IrDAFAR *frame, uint framesz )	= {
	
	{	lapRE_MissStage, lapRE_NDMXXXCmd, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_NDMSNRMCmd, lapRE_SRecvTESTCmd,
		lapRE_MissStage, lapRE_MissStage, lapRE_NDMUICmd, lapRE_NDMUIRsp,
		lapRE_NDMXIDCmd, lapRE_NDMXIDRsp, lapRE_NDMXXXCmd, lapRE_MissStage,
		lapRE_NDMXXXCmd, lapRE_MissStage, lapRE_NDMXXXCmd, lapRE_MissStage,
		lapRE_NDMXXXCmd, lapRE_MissStage, lapRE_NDMXXXCmd, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_REPLYSNRMCmd, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_REPLYXIDCmd, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_SRecvDISCCmd, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_SRecvSNRMCmd, lapRE_SRecvTESTCmd,
		lapRE_MissStage, lapRE_MissStage, lapRE_SRecvUICmd, lapRE_MissStage,
		lapRE_SRecvUnknownCmd, lapRE_MissStage, lapRE_SRecvRRCmd, lapRE_MissStage,
		lapRE_SRecvRNRCmd, lapRE_MissStage, lapRE_SRecvREJCmd, lapRE_MissStage,
		lapRE_SRecvSREJCmd, lapRE_MissStage, lapRE_SRecvICmd, lapRE_MissStage,
		lapRE_SRecvUnknownCmd, lapRE_SRecvUnknownCmd },
	
	{	lapRE_SErrorMiss, lapRE_SErrorDISCCmd, lapRE_SErrorDMRsp, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss, lapRE_SErrorMiss,
		lapRE_SErrorMiss, lapRE_SErrorMiss },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_SBusyWaitDISCCmd, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapSendTESTRsp,
		lapRE_MissStage, lapRE_MissStage, lapRE_SBusyWaitUICmd, lapRE_MissStage,
		lapRE_SBusyWaitXIDCmd, lapRE_MissStage, lapRE_SBusyWaitRRCmd, lapRE_MissStage,
		lapRE_SBusyWaitRNRCmd, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_SBusyWaitICmd, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_SCloseDISCCmd, lapRE_SCloseDMRsp, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX, lapRE_SCloseXXX,
		lapRE_SCloseXXX, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_SRXWaitDISCCmd, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_SRXWaitXXXCmd, lapRE_SRXWaitXXXCmd,
		lapRE_MissStage, lapRE_MissStage, lapRE_SRXWaitXXXCmd, lapRE_MissStage,
		lapRE_SRXWaitXXXCmd, lapRE_MissStage, lapRE_SRXWaitXXXCmd, lapRE_MissStage,
		lapRE_SRXWaitXXXCmd, lapRE_MissStage, lapRE_SRXWaitXXXCmd, lapRE_MissStage,
		lapRE_SRXWaitXXXCmd, lapRE_MissStage, lapRE_SRXWaitXXXCmd, lapRE_MissStage,
		lapRE_SRXWaitXCHGCmd, lapRE_SRXWaitDXCHGCmd },
	
	{	lapRE_MissStage, lapRE_SXWaitDISCCmd, lapRE_MissStage, lapRE_MissStage,
		lapRE_SXWaitRDRsp, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_SXWaitRRRsp,
		lapRE_MissStage, lapRE_SXWaitRNRRsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_SXWaitIRsp, lapRE_SXWaitRXCHGRsp,
		lapRE_SXWaitXCHGCmd, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_PoutXIDCmd, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX,
		lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffSNRMCmd, lapRE_SniffXXX,
		lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX,
		lapRE_SniffXIDCmd, lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX,
		lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX,
		lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX, lapRE_SniffXXX,
		lapRE_SniffXXX, lapRE_SniffXXX },
#if defined(IrDAPRIMARY)
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_SETUPDMRsp, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_SETUPUARsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_QUERYXIDRsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_PRecvFRMRRsp,
		lapRE_PRecvRDRsp, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_PRecvRRRsp,
		lapRE_MissStage, lapRE_PRecvRNRRsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_PRecvIRsp, lapRE_PRecvRXCHGRsp,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_PBusyWaitRDRsp, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_PBusyWaitRRRsp,
		lapRE_MissStage, lapRE_PBusyWaitRNRRsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_PBusyWaitIRsp, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_PCloseDMRsp, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_PCloseUARsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_PXWaitDISCCmd, lapRE_PXWaitXXXRsp, lapRE_PXWaitFRMRRsp,
		lapRE_PXWaitRDRsp, lapRE_PXWaitXXXRsp, lapRE_PXWaitSNRMCmd, lapRE_PXWaitTESTCmd,
		lapRE_PXWaitXXXRsp, lapRE_PXWaitXXXRsp, lapRE_PXWaitUICmd, lapRE_PXWaitXXXRsp,
		lapRE_PXWaitXIDCmd, lapRE_PXWaitXXXRsp, lapRE_PXWaitRRCmd, lapRE_PXWaitXXXRsp,
		lapRE_PXWaitRNRCmd, lapRE_PXWaitXXXRsp, lapRE_PXWaitREJCmd, lapRE_PXWaitXXXRsp,
		lapRE_PXWaitSREJCmd, lapRE_PXWaitXXXRsp, lapRE_PXWaitIFrame, lapRE_PXWaitXXXRsp,
		lapRE_PXWaitXCHGCmd, lapRE_PXWaitDXCHGCmd },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_SCONNXIDRsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
	
	{	lapRE_MissStage, lapRE_MissStage, lapRE_SSETUPDMRsp, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_SSETUPUARsp, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage, lapRE_MissStage, lapRE_MissStage,
		lapRE_MissStage, lapRE_MissStage },
#endif	
} ;


#if defined(TxPTRCHK)
IrDABOOL	ChkTxWIndowPtr( void ) ;
#endif	


void	IrDAAPI	IrLAP_Open( void )
{
	const	byte	IrDAFAR *pc ;
	uint	pcsz ;
#if defined(IrDAPRIMARY)
#if !defined(IrLAPNONEWADDR)
	int		i ;
#endif	
#endif	


#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_Open(): Start!!\n" ) ;
#endif	
	
	MyPortConfig.LineCapability		= IrLAP_BPS_9600 | IrLAP_BPS_19200 | IrLAP_BPS_38400 | IrLAP_BPS_57600 | IrLAP_BPS_115200 ;
	MyPortConfig.MaxTanaroundTime	= IrLAP_MaxTurn_500ms | IrLAP_MaxTurn_250ms | IrLAP_MaxTurn_100ms | IrLAP_MaxTurn_50ms | IrLAP_MaxTurn_25ms | IrLAP_MaxTurn_10ms | IrLAP_MaxTurn_5ms ;
	MyPortConfig.FrameSize			= IrLAP_FS_64 ;
	MyPortConfig.WindowSize			= IrLAP_WS_1 ;
	MyPortConfig.NoofBOF			= IrLAP_BOF_48 | IrLAP_BOF_24 | IrLAP_BOF_12 | IrLAP_BOF_5 | IrLAP_BOF_3 | IrLAP_BOF_2 | IrLAP_BOF_1 | IrLAP_BOF_0 ;
	MyPortConfig.MinTurnaroundTime	= IrLAP_MinTurn_0ms ;
	MyPortConfig.DisconnectTime		= IrLAP_SL_3s | IrLAP_SL_8s | IrLAP_SL_12s | IrLAP_SL_16s | IrLAP_SL_20s | IrLAP_SL_25s | IrLAP_SL_30s | IrLAP_SL_40s ;
	pcsz	= IrSIR_GetCapability((const void IrDAFAR* IrDAFAR*)&pc ) ;
	lapGetCapacityParam( &MyPortConfig, pc, pcsz ) ;
	MyAddress				= (dword)(&MyAddress) ;
	IrDA_memset( (void*)&lapFlags, 0, sizeof( LAPFLAGS )) ;
	lapFlags.noAddress		= TRUE ;
	IrLAP_Stage				= IrLAP_S_NDM ;


	


	
	SlotTimerHndl		= -1 ;
	QueryTimerHndl		= -1 ;
	PTimerHndl			= -1 ;
#if defined(DMSG)	
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_Open(): stop ptimer\n" ) ;
#endif	
	FTimerHndl			= -1 ;
	WDTimerHndl			= -1 ;
	LTimerHndl			= -1 ;
#if	defined(IrDA_DELAIEDSECONDARY)
	DTimerHndl			= -1 ;
	DelayTimer			= 300L ;
printf( "IrLAP_Open(): DelayTimer:%08X\r\n", DelayTimer ) ;
#endif	
#if !defined(IrLAPNOSNIFF)
	ListenTimerHndl		= -1 ;
	SenseTimerHndl		= -1 ;
	SniffTimerHndl		= -1 ;
	SleepTimerHndl		= -1 ;
#endif	


	
	LAP_TxFrameSize		= 0 ;
	LAP_TxWindowSize	= 0 ;


	
	EnableDiscoveryAnswer	= TRUE ;
	LAP_DiscoveryName		= 0 ;
	LAP_DiscoveryNameSize	= 0 ;
#if !defined(IrLAPNOSNIFF)
	SniffDiscLog				= (DISCOVERYLOG IrDAFAR*)0 ;
#endif	
	PrvDiscLog				= (DISCOVERYLOG IrDAFAR*)0 ;
	AplDiscLog				= (DISCOVERYLOG IrDAFAR*)0 ;
#if defined(IrDAPRIMARY)
# if !defined(IrLAPNONEWADDR)
	for( i=0 ; i<AUTONEWADDRHIST ; i++ )
		NewAddrHist[i]		= (dword)0L ;
# endif	
#endif	


	
	lapInitCallBackFunctions() ;


	


	AplLinkIndTime		= 0 ;


	TxSizePtr			= &DmyTxSize ;
	RxSizePtr			= &DmyRxSize ;


	MiscInitRandomize() ;


	
	IrSIR_SetDataInd( __IrLAP_CatchDataInd ) ;
	IrSIR_InitConnectionParameters() ;
	lapApplyDefaultConnParam() ;
	lapCbInitConnState() ;


#if	defined(SEQCHK)
	InitSequencer() ;
#endif	
}


void	IrDAAPI	IrLAP_Close( void )
{
}


#if !defined(IrLAPNOSNIFF)


void	IrDAAPI	IrLAP_SetSniffInd( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void ))
{
	if( func ) {
		SniffIndPtr		= func ;
		SniffDiscLog	= log ;
	} else {
		SniffIndPtr		= lapDmySniffInd ;
		SniffDiscLog	= 0 ;
	}
}


int		IrDAAPI	IrLAP_SniffReq( void )
{
	lapFlags.sniffing			= TRUE ;
	lapFlags.refuseDiscovery	= TRUE ;
	if( IrLAP_IsConnected ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_CONNECTING ) ;
	}


	if( IrLAP_Stage != IrLAP_S_NDM ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_BUSYWAIT ) ;
	}


	IrLAP_Stage					= IrLAP_S_POUT ;
	lapFlags.refuseDiscovery	= FALSE ;


	
	
	IrDA_PowerOn() ;
	lapStartSenseTimer() ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_SniffAbt( void )
{
	lapFlags.sniffing			= FALSE ;


	if( !IrLAP_IsSniffing )
		return( IrDAR_INVALID ) ;


	IrDA_PowerOff() ;
	lapStopSenseTimer() ;
	lapStopSniffTimer() ;
	lapStopSleepTimer() ;
	lapStateToNDM( 0 ) ;
	return( IrDAR_ACCEPT ) ;
}
#endif	


uint	IrDAAPI	IrLAP_Status( void )
{
	if( IrLAP_IsConnected )
		return( 1 ) ;


	return( 0 ) ;
}


void	IrDAAPI	IrLAP_SetDeviceName( const byte IrDAFAR *name, uint namesz )
{
	LAP_DiscoveryName		= name ;
	LAP_DiscoveryNameSize	= namesz ;
}


void	IrDAAPI	IrDA_GetPortConfig( PORTCONFIG IrDAFAR *configp )
{
	configp->LineCapability		= (byte)MyPortConfig.LineCapability ;	
	configp->MaxTanaroundTime	= (byte)MyPortConfig.MaxTanaroundTime ;	
	configp->FrameSize			= (byte)MyPortConfig.FrameSize ;		
	configp->WindowSize			= (byte)MyPortConfig.WindowSize ;		
	configp->NoofBOF			= (byte)MyPortConfig.NoofBOF ;			
	configp->MinTurnaroundTime	= (byte)MyPortConfig.MinTurnaroundTime ;
	configp->DisconnectTime		= (byte)MyPortConfig.DisconnectTime ;	
}


void	IrDAAPI	IrDA_SetPortConfig( PORTCONFIG IrDAFAR *configp )
{
	PORTCONFIG2	tmp ;


	tmp.LineCapability		= (word)configp->LineCapability ;	
	tmp.MaxTanaroundTime	= (word)configp->MaxTanaroundTime ;	
	tmp.FrameSize			= (word)configp->FrameSize ;		
	tmp.WindowSize			= (word)configp->WindowSize ;		
	tmp.NoofBOF				= (word)configp->NoofBOF ;			
	tmp.MinTurnaroundTime	= (word)configp->MinTurnaroundTime ;
	tmp.DisconnectTime		= (word)configp->DisconnectTime ;	
	IrDA_SetPortConfig2( &tmp ) ;
}


int		IrDAAPI	IrDA_GetConnPortConfig( PORTCONFIG IrDAFAR *configp )
{
	PORTCONFIG2	tmp ;
	int			ret ;


	ret		= IrDA_GetConnPortConfig2( &tmp ) ;
	configp->LineCapability		= (byte)tmp.LineCapability ;	
	configp->MaxTanaroundTime	= (byte)tmp.MaxTanaroundTime ;	
	configp->FrameSize			= (byte)tmp.FrameSize ;			
	configp->WindowSize			= (byte)tmp.WindowSize ;		
	configp->NoofBOF			= (byte)tmp.NoofBOF ;			
	configp->MinTurnaroundTime	= (byte)tmp.MinTurnaroundTime ;	
	configp->DisconnectTime		= (byte)tmp.DisconnectTime ;	
	return( ret ) ;
}


void	IrDAAPI	IrDA_GetPortConfig2( PORTCONFIG2 IrDAFAR *configp )
{
	IrDA_memcpy( configp, &MyPortConfig, sizeof( PORTCONFIG2 )) ;
}


void	IrDAAPI	IrDA_SetPortConfig2( PORTCONFIG2 IrDAFAR *configp )
{
	PORTCONFIG2			tmp ;
	const void	IrDAFAR *pc ;
	uint				pcsz ;


	pcsz	= IrSIR_GetCapability((const void IrDAFAR* IrDAFAR*)&pc ) ;
	IrDA_memset( &tmp, 0xFF, sizeof( PORTCONFIG2 )) ;
	lapGetCapacityParam(( PORTCONFIG2 IrDAFAR *)&tmp, (const byte IrDAFAR *)pc, pcsz ) ;


	configp->LineCapability		|= IrLAP_BPS_9600 ;
	configp->MaxTanaroundTime	|= IrLAP_MaxTurn_500ms ;
	configp->NoofBOF			|= IrLAP_BOF_48 ;
	configp->MinTurnaroundTime	|= IrLAP_MinTurn_10ms ;
	configp->DisconnectTime		|= IrLAP_SL_3s ;


#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_SetPortConfig2():" ) ;
	_DPUTW( DL_LAPINFO, " MyPortConfig.LineCapability:", MyPortConfig.LineCapability ) ;
	_DPUTS( DL_LAPINFO, "\r\n" ) ;
#endif	
	MyPortConfig.LineCapability		= configp->LineCapability & tmp.LineCapability ;
	if( MyPortConfig.LineCapability == 0 )
		MyPortConfig.LineCapability	= tmp.LineCapability ;
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_SetPortConfig2():" ) ;
	_DPUTW( DL_LAPINFO, " MyPortConfig.LineCapability:", MyPortConfig.LineCapability ) ;
	_DPUTS( DL_LAPINFO, "\r\n" ) ;
#endif	
	MyPortConfig.MaxTanaroundTime	= configp->MaxTanaroundTime & tmp.MaxTanaroundTime ;
	if( MyPortConfig.MaxTanaroundTime == 0 )
		MyPortConfig.MaxTanaroundTime	= tmp.MaxTanaroundTime ;


	MyPortConfig.NoofBOF			= configp->NoofBOF & tmp.NoofBOF ;
	if( MyPortConfig.NoofBOF == 0 )
		MyPortConfig.NoofBOF	= tmp.NoofBOF ;
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_SetPortConfig2():" ) ;
	_DPUTW( DL_LAPINFO, " configp->MinTurnaroundTime:", configp->MinTurnaroundTime ) ;
	_DPUTW( DL_LAPINFO, " tmp.MinTurnaroundTime:", tmp.MinTurnaroundTime ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	MyPortConfig.MinTurnaroundTime	= configp->MinTurnaroundTime & tmp.MinTurnaroundTime ;
	if( MyPortConfig.MinTurnaroundTime == 0 )
		MyPortConfig.MinTurnaroundTime	= tmp.MinTurnaroundTime ;
#if	defined(DMSG2)
	_DPUTB( DL_LAPINFO, " MyPortConfig.MinTurnaroundTime:", MyPortConfig.MinTurnaroundTime ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	MyPortConfig.DisconnectTime		= configp->DisconnectTime & tmp.DisconnectTime ;
	if( MyPortConfig.DisconnectTime == 0 )
		MyPortConfig.DisconnectTime	= tmp.DisconnectTime ;
}


int		IrDAAPI	IrDA_GetConnPortConfig2( PORTCONFIG2 IrDAFAR *configp )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_GetConnPortConfig():" ) ;
	_DPUTB( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( !IrLAP_IsConnected )
		return( IrDAR_NOCONNECT ) ;


	IrDA_memcpy( configp, &TargetPortConfig, sizeof( PORTCONFIG2 )) ;
	return( IrDAR_ACCEPT ) ;
}


void	IrDAAPI	IrDA_SetTxFrame( byte IrDAFAR *buf, uint framesz, uint winsz )
{
	uint		i ;


	LAP_TxFrameSize		= framesz ;
	LAP_TxWindowSize	= winsz ;


	if( LAP_TxWindowSize > IrLAPMAXTXWINSIZE )
		LAP_TxWindowSize	= IrLAPMAXTXWINSIZE ;


	for( i=0 ; i<LAP_TxWindowSize ; i++ )
		LAP_SendCtl[i].ptr	= &buf[i*LAP_TxFrameSize] ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_SetTxFrame():" ) ;
	_DPUTW( DL_LAPINFO, " framesz:", framesz ) ;
	_DPUTW( DL_LAPINFO, " winsz:", winsz ) ;
	_DPUTD( DL_LAPINFO, " buf:", (dword)buf ) ;
	_DPUTD( DL_LAPINFO, " SendCtl[].ptr=", (dword)LAP_SendCtl[0].ptr ) ;
	for( i=1 ; i<LAP_TxWindowSize ; i++ )
		_DPUTD( DL_LAPINFO, ",", (dword)LAP_SendCtl[i].ptr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapReleaseBufferedData() ;
}


void	IrDAAPI	IrDA_SetRxFrame( byte IrDAFAR *buf, uint framesz, uint winsz )
{
	IrSIRFRAMECONFIG	frameconfig ;


	framesz	-= 2 ;									


	LAP_RxFrameSize		= framesz ;
	LAP_RxWIndowSize	= winsz ;
	if( winsz > IrLAPRECVTBLSIZE )
		LAP_RxWIndowSize	= IrLAPRECVTBLSIZE ;


	MyPortConfig.FrameSize	= IrLAP_FS_64 ;
	if( framesz >= 2048 )	MyPortConfig.FrameSize	|= IrLAP_FS_2048 ;
	if( framesz >= 1024 )	MyPortConfig.FrameSize	|= IrLAP_FS_1024 ;
	if( framesz >= 512 )	MyPortConfig.FrameSize	|= IrLAP_FS_512 ;
	if( framesz >= 256 )	MyPortConfig.FrameSize	|= IrLAP_FS_256 ;
	if( framesz >= 128 )	MyPortConfig.FrameSize	|= IrLAP_FS_128 ;


	MyPortConfig.WindowSize	= IrLAP_WS_1 ;
	if( winsz >= 7 )		MyPortConfig.WindowSize	|= IrLAP_WS_7 ;
	if( winsz >= 6 )		MyPortConfig.WindowSize	|= IrLAP_WS_6 ;
	if( winsz >= 5 )		MyPortConfig.WindowSize	|= IrLAP_WS_5 ;
	if( winsz >= 4 )		MyPortConfig.WindowSize	|= IrLAP_WS_4 ;
	if( winsz >= 3 )		MyPortConfig.WindowSize	|= IrLAP_WS_3 ;
	if( winsz >= 2 )		MyPortConfig.WindowSize	|= IrLAP_WS_2 ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::IrDA_SetRxFrame():" ) ;
	_DPUTW( DL_LAPINFO, " framesz :", framesz ) ;
	_DPUTW( DL_LAPINFO, " windowsz:", winsz ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	IrSIR_InitConnectionParameters() ;
	frameconfig.RecvBuffer		= buf ;
// chos ???	frameconfig.RecvBufferSize	= ( framesz + 4 ) * winsz ;
	frameconfig.RecvBufferSize	= framesz * winsz ;
	frameconfig.TblSize			= IrLAPRECVTBLSIZE ;
	frameconfig.RecvBufTbl		= RecvAddrTbl ;
	frameconfig.RecvSizeTbl		= RecvSizeTbl ;
	frameconfig.RecvWindow		= &RecvFrameSize ;
	IrSIR_SetFrameBuffer( &frameconfig ) ;
	IrSIR_SetFrameMode() ;
	RecvFrameReadPtr			= 0 ;
	IrSIR_ResetRecvMode() ;
	IrSIR_SetRecvMode() ;
#if	defined(DMSG)
	FrameNo		= 0 ;
#endif	
}


int		IrDAAPI	IrLAP_DisconnectReq( void )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_DisconnectReq():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( !IrLAP_IsConnected )
		return( IrDAR_NOCONNECT ) ;


	lapFlags.disconnectReq	= TRUE ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_DataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz ))
{
	uint	len ;


#if	defined(DMSG)
	_DPUTB( DL_LAPINFO, " LAP::IrLAP_DataReq(): IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTB( DL_LAPINFO, " LAP_TxWindowSize:", LAP_TxWindowSize ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( !IrLAP_IsConnected )
		return( IrDAR_NOCONNECT ) ;


	if( lapFlags.inDataReq )
		return( IrDAR_NORESOURCE ) ;
	lapFlags.inDataReq		= TRUE ;


	if( LAP_SendCtl[LAP_SendWinWritePtr].size > 0 ) {
		lapFlags.inDataReq	= FALSE ;
		return( IrDAR_NORESOURCE ) ;
	}


	len		= (*func)( &LAP_SendCtl[LAP_SendWinWritePtr].ptr[2], ConnSendFrameSize ) ;
	if( len ) {
		if( len > ConnSendFrameSize )
			len	= ConnSendFrameSize ;
		LAP_SendCtl[LAP_SendWinWritePtr].xmit	= FALSE ;
		LAP_SendCtl[LAP_SendWinWritePtr].size	= len + 2 ;
		LAP_SendWinWritePtr	= lapNextSendWindowPtr( LAP_SendWinWritePtr ) ;
	}


#if defined(IrDAPRIMARY)
	if( IrLAP_Stage == IrLAP_S_PRecv || IrLAP_Stage == IrLAP_S_PBusyWait ) {
#if defined(DMSG)	
		_DPUTS( DL_LAPINFO, " LAP::IrLAP_DataReq():" ) ;
		_DPUTW( DL_LAPINFO, " PTimerHndl:", PTimerHndl ) ;
		_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		if( PTimerHndl != -1 ) {					
			lapStopPTimer() ;
#if	defined(NOTXMULTIWINDOW)
			LAP_TxLeaveWindow		= 1 ;
#else	
			LAP_TxLeaveWindow		= ConnSendWindowSize ;
#endif	
#if defined(DMSG)
			_DPUTS( DL_LAPINFO, " LAP::IrLAP_DataReq(): call lapCBSendPLeaveWindow()" ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			lapCBSendPLeaveWindow() ;
		}
	}
#endif	


	lapFlags.inDataReq		= FALSE ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_SetConnectInd( int (IrDACB *func)( dword addr ))
{
	if( func == 0 )
		ConnectIndPtr	= lapDmyConnectInd ;
	else
		ConnectIndPtr	= func ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_SetDisconnectInd( void (IrDACB *func)( int status ))
{
	if( func == 0 )
		DisconnectIndPtr	= lapDmyDisconnectInd ;
	else
		DisconnectIndPtr	= func ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_SetDataCnf( void (IrDACB *func)( uint freebufsz ))
{
	if( func == 0 )
		DataCnfPtr			= lapDmyDataCnf ;
	else
		DataCnfPtr			= func ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_SetDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size ))
{
	if( func == 0 )
		DataIndPtr			= lapDmyDataInd ;
	else
		DataIndPtr			= func ;


	return( IrDAR_ACCEPT ) ;
}


#if !defined(IrLAPNOUI)


int		IrDAAPI	IrLAP_UDataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz ))
{
	int		ret ;
	uint	len ;


	if( IrLAP_IsConnected )				return( IrDAR_CONNECTING ) ;
	if( IrLAP_Stage != IrLAP_S_NDM )	return( IrDAR_BUSYWAIT ) ;


	if( lapFlags.inUDataReq )
		return( IrDAR_BUSYWAIT ) ;
	lapFlags.inUDataReq	= TRUE ;


	ret		= IrDAR_MEDIABUSY ;
	if( !IrSIR_Busy()) {
       // 과거 500ms이내에 송수신이 없었다.
		ret		= IrDAR_BUSYWAIT ;
		if( LAP_SendCtl[0].size == 0 && !lapFlags.sendingUi ) {
			lapFlags.sendingUi	= TRUE ;

			if(( len = (*func)( &LAP_SendCtl[0].ptr[2], IrLAP_QueryMaxTxUDataSize())) == 0 ) {
				lapFlags.sendingUi	= FALSE ;
			} else {
				//데이터를 송신한다.
				LAP_SendCtl[0].size		= 2 + len ;
				LAP_SendCtl[0].ptr[0]	= ( IrLAP_BroadCast << 1 ) | IrLAP_CR ;
				LAP_SendCtl[0].ptr[1]	= IrLAP_UICmd | IrLAP_PF ;
				IrSIR_SetDataCnf( lapCbUIDataCnf ) ;
				IrSIR_SendFrame( LAP_SendCtl[0].size, LAP_SendCtl[0].ptr ) ;
				ret		= IrDAR_ACCEPT ;
			}
		}
	}
	lapFlags.inUDataReq	= FALSE ;
	return( ret ) ;
}
#endif	


#if !defined(IrLAPNOUI)


int		IrDAAPI	IrLAP_SetUDataCnf( void (IrDACB *func)( void ))
{
	if( func == 0 )
		UDataCnfPtr			= lapDmyUDataCnf ;
	else
		UDataCnfPtr			= func ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrLAPNOUI)


int		IrDAAPI	IrLAP_SetUDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size ))
{
	if( func == 0 )
		UDataIndPtr			= lapDmyUDataInd ;
	else
		UDataIndPtr			= func ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrLAPNOUI)


int		IrDAAPI	IrLAP_QueryMaxTxUDataSize( void )
{
	if( LAP_TxFrameSize - 2 > 384 )
		return( 384 ) ;
	return( LAP_TxFrameSize - 2 ) ;
}
#endif	


#if !defined(IrLAPNOUI)


int		IrDAAPI	IrLAP_QueryMaxRxUDataSize( void )
{
	if( LAP_RxFrameSize > 384 )
		return( 384 ) ;
	return( LAP_RxFrameSize ) ;
}
#endif	


#if !defined(IrLAPNORESET)


int		IrDAAPI	IrLAP_SetResetInd( IrDABOOL (IrDACB *func)( void ))
{
	if( func == 0 )
		ResetIndPtr			= lapDmyResetInd ;
	else
		ResetIndPtr			= func ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


int		IrDAAPI	IrLAP_LocalBusy( IrDABOOL sw )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_LocalBusy():" ) ;
	_DPUTS( DL_LAPINFO, " sw:" ) ;
	if( sw )
		_DPUTS( DL_LAPINFO, "ON" ) ;
	else
		_DPUTS( DL_LAPINFO, "OFF" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapFlags.localBusyDetected	= FALSE ;
	lapFlags.localBusyCleared	= FALSE ;
	if( sw )
		lapFlags.localBusyDetected	= TRUE ;
	else
		lapFlags.localBusyCleared	= TRUE ;


	return( IrDAR_ACCEPT ) ;
}


uint	IrDAAPI	IrLAP_GetMaxRxSize( void )
{
	if( !IrLAP_IsConnected )
		return( 0 ) ;


	return( lapGetFrameSize( MyPortConfig.FrameSize )) ;
}


uint	IrDAAPI	IrLAP_GetMaxTxSize( void )
{
	if( !IrLAP_IsConnected )
		return( 0 ) ;


	return( ConnSendFrameSize ) ;
}


IrDABOOL	IrDAAPI	IrLAP_QueryTxQueue( void )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_QueryTxQueue():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTW( DL_LAPINFO, " LAP_TxWindowSize:", LAP_TxWindowSize ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( !IrLAP_IsConnected )
		return( FALSE ) ;


	if( LAP_SendCtl[LAP_SendWinWritePtr].size > 0 )
		return( FALSE ) ;


#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_QueryTxQueue():" ) ;
	_DPUTS( DL_LAPINFO, " TxQueue is Free!!" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	return( TRUE ) ;
}


int		IrDAAPI	IrLAP_QueryVirginTxQueue( void )
{
	uint	i ;
	int		sz ;


	if( !IrLAP_IsConnected )
		return( 0 ) ;


	sz		= 0 ;
	for( i=0 ; i < LAP_TxWindowSize ; i++ ) {
		if( LAP_SendCtl[i].size > 0 && !LAP_SendCtl[i].xmit )
			sz	++ ;
	}


	return( sz ) ;
}


void	IrDAAPI	IrLAP_SetDiscoveryPrv( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void ))
{
	if( func ) {
		DiscoveryPrvPtr	= func ;
		PrvDiscLog		= log ;
	} else {
		DiscoveryPrvPtr	= lapDmyDiscoveryPrv ;
		PrvDiscLog		= 0 ;
	}
}


void	IrDAAPI	IrLAP_SetConnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		ConnectPrvPtr	= func ;
	else
		ConnectPrvPtr	= lapDmyConnectPrv ;
}


void	IrDAAPI	IrLAP_SetConnectPrv2( void (IrDACB *func)( int type ))
{
	if( func )
		ConnectPrvPtr2	= func ;
	else
		ConnectPrvPtr2	= lapDmyConnectPrv2 ;
}


void	IrDAAPI	IrLAP_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		DisconnectPrvPtr	= func ;
	else
		DisconnectPrvPtr	= lapDmyDisconnectPrv ;
}


void	IrDAAPI	IrLAP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp )
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


static	void	IrDACB	lapDmyDiscoveryCnf( int status, uint logsz )
{
	UNREFERENCE( status ) ;
	UNREFERENCE( logsz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		IrDACB	lapDmyConnectInd( dword addr )
{
	UNREFERENCE( addr ) ;


	return( IrDA_CONNECTDI ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyDisconnectInd( int state )
{
	UNREFERENCE( state ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyDataCnf( uint freebufsz )
{
	UNREFERENCE( freebufsz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyDataInd( const byte IrDAFAR *data, uint sz )
{
	UNREFERENCE( data ) ;
	UNREFERENCE( sz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
# if !defined(IrLAPNOUI)
static	void	IrDACB	lapCbUIDataCnf( void )
{
	LAP_SendCtl[0].size	= 0 ;
	lapFlags.sendingUi	= FALSE ;
	(*UDataCnfPtr)() ;
}
# endif	
#endif	


#if !defined(IrDANOSTATIC)
# if !defined(IrLAPNOUI)


static	void	IrDACB	lapDmyUDataInd( const byte IrDAFAR *data, uint sz )
{
	UNREFERENCE( data ) ;
	UNREFERENCE( sz ) ;
}
# endif	
#endif	


#if !defined(IrDANOSTATIC)
# if !defined(IrLAPNOUI)


static	void	IrDACB	lapDmyUDataCnf( void )
{
}
# endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORESET)


static	IrDABOOL	IrDACB	lapDmyResetInd( void )
{
	return( IrDA_CONNECTEN ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyDiscoveryPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyConnectPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyConnectPrv2( int type )
{
	UNREFERENCE( type ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyDisconnectPrv( void )	{}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	IrDACB	lapDmyAplConnectInd( void )
{
	return( IrDA_CONNECTEN ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyAplLinkInd( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmyTxEmptyPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapDmySnrmPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)
#if	defined(VMSG)
static	void	DispWORD( uint __far *vramp, word w )
{
	int		i ;
	int		c ;


	for( i=0 ; i<4 ; i++ ) {
		c	= ( w >> 12 ) & 0x0F ;
		if( c < 10 )
			*vramp	= VRAMD( c + '0', 0x3F ) ;
		else
			*vramp	= VRAMD( c + 'A' - 10, 0x3F ) ;


		vramp	++ ;
		w		<<= 4 ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(VMSG)
static	uint	__far *vram ;
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbCatchDataInd( void )
{
	int		event ;
	const byte IrDAFAR	*frame ;
	uint				framesz ;


#if	defined(VMSG)
	vram	= VRAMP(40,0) ;
	*vram	= 0x1F20 ;
	vram	= VRAMP(40,1) ;
	{	int	i ;
		for( i=0 ; i<40 ; i++ )
			vram[i]	= 0x1F20 ;
	}
	vram[0]	= VRAMD( 'I', 0x3F ) ;
#endif	


#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, "IND<<<\n" ) ;
	if( RecvFrameReadPtr == RecvFrameSize ) {
		_DPUTS( DL_LAPINFO, " LAP::lapCbCatchDataInd():" ) ;
		_DPUTW( DL_LAPINFO, " RecvFrameReadPtr:", RecvFrameReadPtr ) ;
		_DPUTW( DL_LAPINFO, " RecvFrameSize:", RecvFrameSize ) ;
		_DPUTS( DL_LAPINFO, "\n" ) ;
	}
#endif	
#if defined(TxPTRCHK)
	if( ChkTxWIndowPtr())
		;
#endif	
	lapFlags.rstRxBuffer	= FALSE ;
#if defined(VMSG)
	vram[1]	= VRAMD( 'c', 0x1F ) ;
#endif	
	lapCallDataCnf() ;
#if defined(VMSG)
	vram[1]	= VRAMD( 'c', 0x1F ) ;
#endif	


#if defined(VMSG)
	vram[2]	= VRAMD( 'L', 0x3F ) ;
#endif	
	for( ; RecvFrameReadPtr < RecvFrameSize ; RecvFrameReadPtr++ ) {
		frame	= (const byte IrDAFAR *)RecvAddrTbl[RecvFrameReadPtr] ;
		framesz	= (uint)RecvSizeTbl[RecvFrameReadPtr] ;
		if( framesz > LAP_RxFrameSize + 2 )
			continue ;
		event	= lapGetRecvEvent( frame, framesz ) ;


		if( lapFlags.localBusyDetected ) {
			lapFlags.localBusyDetected	= FALSE ;
#if	defined(DMSG)
			_DPUTS( DL_LAPINFO, " LAP::lapCbCatchDataInd():" ) ;
			_DPUTS( DL_LAPINFO, " LocalBusy:On" ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			switch( IrLAP_Stage ) {
#if defined(IrDAPRIMARY)
			case	IrLAP_S_PRecv :
				IrLAP_Stage		= IrLAP_S_PBusyWait ;
				break ;
#endif	
			case	IrLAP_S_SRecv :
				IrLAP_Stage		= IrLAP_S_SBusyWait ;
				break ;
			}
		}


		if( lapFlags.localBusyCleared ) {
			lapFlags.localBusyCleared	= FALSE ;
#if	defined(DMSG)
			_DPUTS( DL_LAPINFO, " LAP::lapCbCatchDataInd():" ) ;
			_DPUTS( DL_LAPINFO, " LocalBusy:Off" ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			switch( IrLAP_Stage ) {
#if defined(IrDAPRIMARY)
			case	IrLAP_S_PBusyWait :
				IrLAP_Stage		= IrLAP_S_PRecv ;
				break ;
#endif	
			case	IrLAP_S_SBusyWait :
				lapStateToSRecv() ;
				break ;
			}
		}


#if	0
		FrameNo	++ ;
# if 1
		if( event == IrLAP_RE_RRCmd || event == IrLAP_RE_RRRsp || event == IrLAP_RE_RNRCmd || event == IrLAP_RE_RNRRsp ) {
#  if 0
			if( event == lapRE_RRCmd )
				_DPUTS( DL_LAPINFO, "RRCmd" ) ;
			else if( event == lapRE_RRRsp )
				_DPUTS( DL_LAPINFO, "RRRsp" ) ;
			else if( event == lapRE_RNRCmd )
				_DPUTS( DL_LAPINFO, "RNRCmd" ) ;
			else if( event == lapRE_RNRRsp )
				_DPUTS( DL_LAPINFO, "RNRRsp" ) ;
			_DPUTD( DL_LAPINFO, " FrameSize:", framesz ) ;
			_DPUTD( DL_LAPINFO, " FrameNo:", FrameNo ) ;
			_DPUTD( DL_LAPINFO, " Time:", IrSIR_ReadTimer()) ;
			_DPUTW( DL_LAPINFO, " stage:", (word)IrLAP_Stage ) ;
			_DPUTW( DL_LAPINFO, " event:", (word)event ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#  endif
		} else
# endif
		{
			_DPUTS( DL_LAPINFO, " LAP::lapCbCatchDataInd():" ) ;


			_DPUTD( DL_LAPINFO, " TIME:", IrSIR_ReadTimer()) ;
			_DPUTW( DL_LAPINFO, " RecvFrameReadPtr:", (word)RecvFrameReadPtr ) ;
			_DPUTW( DL_LAPINFO, " stage:", (word)IrLAP_Stage ) ;
			_DPUTW( DL_LAPINFO, " event:", (word)event ) ;
			_DPUTW( DL_LAPINFO, " framesz:", (word)framesz ) ;
			_DPUTD( DL_LAPINFO, " frame:", (dword)frame ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
			_DPUTS( DL_LAPDAT, "                          : data=" ) ;
			_DDUMP( DL_LAPDAT, frame, framesz, 0 ) ;
			_DPUTS( DL_LAPDAT, "\n" ) ;
		}
#endif	
#if defined(VMSG)
		vram[3]	= VRAMD( 'E', 0x3F ) ;
		DispWORD( &vram[4], IrLAP_Stage ) ;
		vram[8]	= VRAMD( ' ', 0x1F ) ;
		DispWORD( &vram[9], event ) ;
		vram[13]	= VRAMD( ' ', 0x1F ) ;
# if 0
		DispWORD( &vram[14], FP_SEG(IrLAP_RecvEventFunc[IrLAP_Stage][event])) ;
		DispWORD( &vram[18], FP_OFF(IrLAP_RecvEventFunc[IrLAP_Stage][event])) ;
# else
		DispWORD( &vram[14], (word)IrLAP_RecvEventFunc[IrLAP_Stage][event]) ;
# endif
#endif	


#if defined(DMSG)||defined(VMSG)
		if( IrLAP_RecvEventFunc[IrLAP_Stage][event] == 0 ) {
# if defined(DMSG)
			_DPUTS( DL_LAPDANGER, " LAP::lapCbCatchDataInd():" ) ;
			_DPUTS( DL_LAPDANGER, " RecvEvent Table is BROKEN!!!!!!!!!!" ) ;
			_DPUTS( DL_LAPDANGER, "\n" ) ;
# endif	
# if defined(VMSG)
			vram[18]	= VRAMD( '!', 0xCF ) ;
# endif	
		} else
#endif	
		(*IrLAP_RecvEventFunc[IrLAP_Stage][event])( frame, framesz ) ;
#if	defined(VMSG)
		vram[3]	= VRAMD( 'E', 0x1F ) ;
#endif	
	}
#if defined(VMSG)
	vram[2]	= VRAMD( 'L', 0x1F ) ;
#endif	


	if( RecvFrameReadPtr > 0 && (
		(( RecvAddrTbl[RecvFrameReadPtr-1][1] & IrLAP_PF ) && !IrSIR_InFrame()) ||
		RecvFrameReadPtr >= LAP_RxWIndowSize )) {
#if defined(VMSG)
		vram[23]	= VRAMD( 'R', 0x3F ) ;
#endif	
		lapResetRxBuffer() ;
#if defined(VMSG)
		vram[23]	= VRAMD( 'R', 0x1F ) ;
#endif	
	}
	if( lapFlags.rstRxBuffer ) {
		RecvFrameReadPtr	= 0 ;
		RecvFrameSize		= 0 ;
	}
#if	defined(VMSG)
	vram[0]	= VRAMD( 'I', 0x1F ) ;
#endif	
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, "IND>>>\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbInitConnState( void )
{
	IrLAP_ConnRemoteBusy	= FALSE ;
	IrLAP_ConnRetryCount	= 0 ;					


	
	IrLAP_Vr	= 0 ;
	IrLAP_Vs	= 0 ;


	
	lapReleaseBufferedData() ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbInitConnState():" ) ;
	_DPUTD( DL_LAPINFO, " SetSpeed              :", IrLAP_ConnSpeed ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTS( DL_LAPINFO, "                           :" ) ;
	_DPUTW( DL_LAPINFO, " MinmimamTurnAroundTime:", IrLAP_ConnMinTurnAround ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTS( DL_LAPINFO, "                           :" ) ;
	_DPUTW( DL_LAPINFO, " BOF                   :", IrLAP_ConnBOFSize + 1 ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	IrSIR_SetSpeed( IrLAP_ConnSpeed ) ;
	IrSIR_SetConnectionAddress( IrLAP_ConnectionAddress ) ;
	IrSIR_SetMinimamTurnaroundTime( IrLAP_ConnMinTurnAround ) ;
	IrSIR_SetBOF( IrLAP_ConnBOFSize ) ;
}
#endif	


void	IrDAAPI	IrLAP_SetAplConnectInd( DISCOVERYLOG IrDAFAR *peer, IrDABOOL (IrDACB *func)( void ))
{
	if( func == 0 ) {
		AplConnectIndPtr	= lapDmyAplConnectInd ;
		AplDiscLog			= 0 ;
	} else {
		AplConnectIndPtr	= func ;
		AplDiscLog			= peer ;
	}
}


void	IrDAAPI	IrLAP_SetSnrmPrv( void (IrDACB *func)( void ), PORTCONFIG2 IrDAFAR *pcp )
{
	if( func == 0 ) {
		lapSnrmPrv.funcp	= lapDmySnrmPrv ;
		lapSnrmPrv.pcp		= 0 ;
	} else {
		lapSnrmPrv.funcp	= func ;
		lapSnrmPrv.pcp		= pcp ;
	}
}


void	IrDAAPI	IrLAP_SetAplLinkInd( void (IrDACB *func)( void ), dword msec )
{
	if( func == 0 ) {
		AplLinkIndPtr	= lapDmyAplLinkInd ;
		AplLinkIndTime	= 0 ;
	} else {
		AplLinkIndPtr	= func ;
		if( msec > 40000L )
			msec	= 40000L ;
		AplLinkIndTime	= msec ;
	}
}


void	IrDAAPI	IrLAP_SetTxEmptyPrv( void (IrDACB *func)( void ))
{
	if( func == 0 )
		TxEmptyPrvPtr	= lapDmyTxEmptyPrv ;
	else
		TxEmptyPrvPtr	= func ;
}


void	IrDAAPI	IrLAP_EnableDiscoveryRsp( void )
{
	EnableDiscoveryAnswer	= TRUE ;
#if defined(DMSG)
	_DPUTS( DL_LAPINFO, "IrLAP_EnableDiscoveryRsp():\n" ) ;
#endif	
}


void	IrDAAPI	IrLAP_DisableDiscoveryRsp( void )
{
	EnableDiscoveryAnswer	= FALSE ;
#if defined(DMSG)
	_DPUTS( DL_LAPINFO, "IrLAP_DisableDiscoveryRsp():\n" ) ;
#endif	
}


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbDefaultConnState( void )
{
#if	defined(DMSG4)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRecvDISCCmd(): Link Shutdown\n" ) ;
#endif	
	lapStateToNDM( IrDAR_DISCONNECT ) ;			
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbSetRecvMode( void )
{
	lapCbInitConnState() ;


	IrSIR_SetRecvMode() ;
	InFrameRetryCounter	= 0 ;
	lapCbStartWDTimer() ;
	lapFlags.sendingUARsp	= FALSE ;
	lapStateToSRecv() ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbSetRecvMode():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbSlowDownPower( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbSlowDownPower():" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	IrDA_SetPowerLevel( 2 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbSendRNRRsp( void )
{
	lapStopWDTimer() ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRNRRsp() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbStartWDTimer( void )
{
	lapStopWDTimer() ;


	InFrameRetryCounter	= 0 ;
	WDTimerHndl	= StartTimer( ConnDisconnectMSec, lapCbWDTimerExpired ) ;
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapCbStartWDTimer():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapStartLTimer() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			lapStartLTimer( void )
{
	lapStopLTimer() ;
	if( AplLinkIndTime )
		LTimerHndl	= StartTimer( AplLinkIndTime, lapCbLTimerExpired ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbQueryTimerExpired( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbQueryTimerExpired():" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	QueryTimerHndl	= -1 ;


	lapStopAllTimers() ;
	IrLAP_Stage	= IrLAP_S_NDM ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbWDTimerExpired( void )
{
	WDTimerHndl		= -1 ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbWDTimerExpired():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, " IrSIR_InFrame():" ) ;
	_DPUTS( DL_LAPINFO, IrSIR_InFrame() ? "IN" : "OUT" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	if( IrSIR_InFrame()) {
		//tasik 01.04.11 현재 데이터 수신중이다.
		InFrameRetryCounter	++ ;
		if( InFrameRetryCounter <= MaxInFrameRetryCount ) {
			WDTimerHndl	= StartTimer( ConnDisconnectMSec, lapCbWDTimerExpired ) ;
			return ;
		}
	}


	switch( IrLAP_Stage ) {
	case	IrLAP_S_SRecv :
	case	IrLAP_S_SBusyWait :
	case	IrLAP_S_SClose :
	case	IrLAP_S_SRXWait :
		lapStateToNDM( IrDAR_DISCONNECT ) ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbLTimerExpired( void )
{
	LTimerHndl	= -1 ;
#if defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbLTimerExpired():\n" ) ;
#endif	
	(*AplLinkIndPtr)() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapSendSLeaveWindow( void )
{
	uint	nextSendPtr ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendSLeaveWindow():" ) ;
	_DPUTW( DL_LAPINFO, " LAP_TxLeaveWindow:", LAP_TxLeaveWindow ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_TxLeaveWindow	-- ;
	nextSendPtr		= lapNextSendWindowPtr( LAP_SendWinSendPtr ) ;
	if( LAP_SendCtl[nextSendPtr].size == 0 ||
		LAP_TxLeaveWindow == 0 ) {
		switch( IrLAP_Stage ) {
		case	IrLAP_S_SRecv :
		default :
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendIFrame( IrLAP_PF, 0 ) ;
			break ;
		case	IrLAP_S_SBusyWait :
			IrSIR_SetDataCnf( lapCbSendRNRRsp ) ;
			lapSendIFrame( 0, 0 ) ;
			break ;
		}
		return ;
	}


	IrSIR_SetDataCnf( lapSendSLeaveWindow ) ;
	lapSendIFrame( 0, 0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORESET)


static	void	IrDACB	lapSendUARspCnf( void )
{
	lapCbInitConnState() ;							
	lapCbStartWDTimer() ;							
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDA_DELAIEDSECONDARY)
static	void	IrDACB	lapDTimerExpired( void )
{
	DTimerHndl	= -1 ;


	switch( IrLAP_Stage ) {
	case	IrLAP_S_SRecv :
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRRRsp() ;
		break ;
	}
}
#endif	
#endif	


#if	defined(TYPES2DEBUG)
extern	int		ShPiocLGet( void ) ;
#endif	


#if !defined(IrDANOSTATIC)


static	int			lapGetRecvEvent( const byte IrDAFAR *frame, uint framesz )
{
	byte	cmd ;


	cmd		= frame[1] ;


	
	if(( cmd & 0x03 ) == 3 )
		return( lapGetUFrameType( frame, framesz )) ;


	
	if(( cmd & 0x03 ) == 1 )
		return( lapGetSFrameType( frame, framesz )) ;


	
	IrLAP_Nr	= ( cmd >> 5 ) & 0x07 ;
	IrLAP_Ns	= ( cmd >> 1 ) & 0x07 ;


	return( IrLAP_RE_IFrame ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapApplyDefaultConnParam( void )
{
	IrLAP_ConnSpeed			= 9600 ;
	IrLAP_ConnMaxTAT		= 500 ;
#if defined(IrLAPMINTATMICRO)
	IrLAP_ConnMinTurnAround	= 10*100 ;
#else	
	IrLAP_ConnMinTurnAround	= 10 ;
#endif	
	IrLAP_ConnectionAddress	= IrLAP_BroadCast ;
	IrLAP_ConnBOFSize		= 10 ;
	MaxInFrameRetryCount	= IrLAPMAXINFRAMERETRYCOUNT ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapReleaseBufferedData( void )
{
	uint	i ;


	LAP_SendWinReadPtr	= 0 ;
	LAP_SendWinSendPtr	= 0 ;
	LAP_SendWinWritePtr	= 0 ;
	LAP_SendWinSendSize	= 0 ;


	for( i=0 ; i<LAP_TxWindowSize ; i++ )
		LAP_SendCtl[i].size	= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetUFrameType( const byte IrDAFAR *frame, uint framesz )
{
	int		ca ;


#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapGetUFrameType():" ) ;
	_DPUTS( DL_LAPINFO, " cr:" ) ;	_DPUTS( DL_LAPINFO, (frame[1]&IrLAP_CR)?"ON":"OFF" ) ;
	_DPUTB( DL_LAPINFO, " cmd:", frame[1] & (~IrLAP_PF )) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( frame[0] & IrLAP_CR ) {
#if defined(IrDABROKENCOMPILER01)
		int		cmd ;
		cmd		= ((int)( frame[1] & ( ~IrLAP_PF ))) & 0xFF ;
		switch( cmd ) {
#else	
		switch( frame[1] & (~IrLAP_PF)) {
#endif	
		case	IrLAP_SNRMCmd :
			if(( frame[1] & IrLAP_PF ) == 0 )
				break ;
			if( framesz == 2 )
				return( IrLAP_RE_SNRMCmd ) ;
			ca		= frame[SNRM_ConnectAddress] & ~IrLAP_CR ;
			if( ca == 0 || ca == 0xFE )
				break ;
			SrcAddress	= lapGetAddress( &frame[SNRM_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[SNRM_DesAddress] ) ;
			if( SrcAddress == 0 || DesAddress == 0 )
				break ;
			if( lapIsRecvAddress( DesAddress ))
				return( IrLAP_RE_SNRMCmd ) ;
			break ;
		case	IrLAP_DISCCmd :
			return( IrLAP_RE_DISCCmd ) ;
		case	IrLAP_UICmd :
			return( IrLAP_RE_UICmd ) ;
		case	IrLAP_XIDCmd :
			SrcAddress	= lapGetAddress( &frame[XID_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[XID_DesAddress] ) ;
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapGetUFrameType():" ) ;
	_DPUTS( DL_LAPINFO, " received XIDCmd" ) ;
{	int	i ;
	_DPUTS( DL_LAPINFO, " frame" ) ;
	_DPUTB( DL_LAPINFO, "=", frame[0] ) ;
	for( i=1 ; i<framesz ; i++ )
		_DPUTB( DL_LAPINFO, ".", frame[i] ) ;
}
	_DPUTD( DL_LAPINFO, " src:", SrcAddress ) ;
	_DPUTD( DL_LAPINFO, " des:", DesAddress ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			if( SrcAddress == 0 || DesAddress == 0 )
				break ;
			if( SrcAddress == 0xFFFFFFFFL ||
				!lapIsRecvAddress( DesAddress ) ||
				frame[XID_FI] != 0x01 )
				break ;
			return( IrLAP_RE_XIDCmd ) ;
		case	IrLAP_TESTCmd :
			SrcAddress	= lapGetAddress( &frame[TEST_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[TEST_DesAddress] ) ;
			if(( frame[0] & IrLAP_CA ) == IrLAP_CA ) {
				if( SrcAddress == 0 || DesAddress == 0 )
					break ;
				if( SrcAddress == 0xFFFFFFFFL ||
					!lapIsRecvAddress( DesAddress ))
					break ;
			}
			return( IrLAP_RE_TESTCmd ) ;
#if !defined(IrLAPNORXCHG)
		case	IrLAP_XCHGCmd :
			return( IrLAP_RE_XCHGCmd ) ;
		case	IrLAP_DXCHGCmd :
			return( IrLAP_RE_DXCHGCmd ) ;
#endif	
		}
	} else {
#if defined(IrDABROKENCOMPILER01)
		int		cmd ;
		cmd		= ((((int)( frame[1] )) & (~IrLAP_PF)) & 0xFF ) ;
		switch( cmd ) {
#else	
		switch( frame[1] & (~IrLAP_PF)) {
#endif	
		case	IrLAP_RNRMRsp :
			return( IrLAP_RE_RNRMRsp ) ;
		case	IrLAP_RDRsp :
			return( IrLAP_RE_RDRsp ) ;
		case	IrLAP_TESTRsp :
			SrcAddress	= lapGetAddress( &frame[TEST_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[TEST_DesAddress] ) ;
			if( SrcAddress == 0 || DesAddress == 0 )
				break ;
			if( SrcAddress == 0xFFFFFFFFL ||
				!lapIsRecvAddress( DesAddress ))
				break ;
			return( IrLAP_RE_TESTRsp ) ;
		case	IrLAP_UARsp :
			SrcAddress	= lapGetAddress( &frame[UA_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[UA_DesAddress] ) ;
			if( SrcAddress == 0 || DesAddress == 0 )
				break ;
			if( lapIsRecvAddress( DesAddress ))
				return( IrLAP_RE_UARsp ) ;
			break ;
		case	IrLAP_UIRsp :
			return( IrLAP_RE_UIRsp ) ;
		case	IrLAP_FRMRRsp :
			return( IrLAP_RE_FRMRRsp ) ;
		case	IrLAP_DMRsp :
			return( IrLAP_RE_DMRsp ) ;
		case	IrLAP_XIDRsp :
			SrcAddress	= lapGetAddress( &frame[XID_SrcAddress] ) ;
			DesAddress	= lapGetAddress( &frame[XID_DesAddress] ) ;
			if( SrcAddress == 0 || DesAddress == 0 )
				break ;
			if( SrcAddress == 0xFFFFFFFFL	||
				!lapIsRecvAddress( DesAddress )	||
				frame[XID_FI] != 0x01 )
				break ;
			return( IrLAP_RE_XIDRsp ) ;
#if !defined(IrLAPNORXCHG)
		case	IrLAP_RXCHGRsp :
			return( IrLAP_RE_RXCHGRsp ) ;
#endif	
		}
	}
	return( IrLAP_RE_Unknown ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetSFrameType( const byte IrDAFAR *frame, uint framesz )
{
	byte 	cmd ;
	int		cr ;


	cr		= frame[0] & IrLAP_CR ;
	if( framesz >= 2 ) {


		
		cmd 	= frame[1];
		IrLAP_Nr	= ( cmd >> 5 ) & 0x07 ;


		switch( cmd & 0x0F ) {
		case	IrLAP_RRCmd :
			if( cr )
				return( IrLAP_RE_RRCmd ) ;
			return( IrLAP_RE_RRRsp ) ;
		case	IrLAP_RNRCmd :
			if( cr )
				return( IrLAP_RE_RNRCmd ) ;
			return( IrLAP_RE_RNRRsp ) ;
		case	IrLAP_REJCmd :
			if( cr )
				return( IrLAP_RE_REJCmd ) ;
			return( IrLAP_RE_REJRsp ) ;
		case	IrLAP_SREJCmd :
			if( cr )
				return( IrLAP_RE_SREJCmd ) ;
			return( IrLAP_RE_SREJRsp ) ;
		}
	}


	return( IrLAP_RE_Unknown ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStateToNDM( int status )
{
	int		oldstate ;
	int		connf ;


	lapStopAllTimers() ;
	lapReleaseBufferedData() ;							
	lapApplyDefaultConnParam() ;						
	lapCbInitConnState() ;


	connf					= IrLAP_IsConnected ;
	oldstate				= IrLAP_Stage ;
	IrLAP_Stage				= IrLAP_S_NDM ;
	lapFlags.disconnectReq	= FALSE ;


	switch( oldstate ) {
	case	IrLAP_S_SETUP :
		(*ConnectCnfPtr)( IrDAR_DISCONNECT ) ;
		break ;
	default :
		if( !connf )
			break ;
	case	IrLAP_S_SCONN :
	case	IrLAP_S_SSETUP :
		lapCallDisconnectInd( status ) ;				
	}


#if !defined(IrLAPNOSNIFF)
	if( lapFlags.sniffing ) {
		IrLAP_Stage	= IrLAP_S_SNIFF ;
		IrDA_PowerOff() ;
		lapStartSleepTimer() ;
	}
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStateToSRecv( void )
{
	int		oldState ;


	oldState	= IrLAP_Stage ;
	IrLAP_Stage	= IrLAP_S_SRecv ;


	switch( oldState ) {
	case	IrLAP_S_PXWait :
	case	IrLAP_S_NDM :
		(*ConnectPrvPtr2)( 2 ) ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	int			lapCallConnectInd( void )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapCallConnectInd():" ) ;
	_DPUTD( DL_LAPINFO, " ConnectPrvPtr:", (dword)ConnectPrvPtr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if((*AplConnectIndPtr)() != IrDA_CONNECTEN )
		return( IrDA_CONNECTDI ) ;
	(*ConnectPrvPtr)() ;
	return((*ConnectIndPtr)( TargetAddress )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapCallDisconnectInd( int status )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCallDisconnectInd()" ) ;
	_DPUTW( DL_LAPINFO, " status:", status ) ;
	_DPUTD( DL_LAPINFO, " DisconnectIndPtr:", (dword)lapCallDisconnectInd ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	(*DisconnectIndPtr)( status ) ;
	(*DisconnectPrvPtr)() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapCallDataInd( const byte IrDAFAR *frame, uint framesz )
{
	uint	isz ;


	if( framesz <= 2 )
		return ;


	isz			= framesz - 2 ;
	*RxSizePtr	+= (ulong)isz ;
#if defined(IrDA_LAPDATAINDPRB)
		IrDA_LAPDATAINDPRB( 0 ) ;
#endif	
	(*DataIndPtr)( &frame[2], isz ) ;
#if defined(IrDA_LAPDATAINDPRB)
		IrDA_LAPDATAINDPRB( 1 ) ;
#endif	
}
#endif	


#if !defined(IrLAPNOUI)
#if !defined(IrDANOSTATIC)


static	void		lapCallUDataInd( const byte IrDAFAR *frame, uint framesz )
{
	(*UDataIndPtr)( &frame[2], framesz - 2 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapCallDataCnf( void )
{
	int		wp, freeq ;


	if( !lapFlags.rsvDataCnf || lapFlags.inDataReq )
		return ;


	if( LAP_SendWinWritePtr == LAP_SendWinReadPtr &&
		LAP_SendCtl[LAP_SendWinWritePtr].size == 0 ) {
		freeq	= LAP_TxWindowSize ;
	} else {
		freeq	= 0 ;
		for( wp=LAP_SendWinWritePtr ;; wp=lapNextSendWindowPtr(wp)) {
			if( LAP_SendCtl[wp].size > 0 )
				break ;
			freeq	++ ;
		}
	}
#if defined(IrDA_LAPDATACNFPRB)
		IrDA_LAPDATACNFPRB( 0 ) ;
#endif	
	(*DataCnfPtr)( freeq ) ;
#if defined(IrDA_LAPDATACNFPRB)
		IrDA_LAPDATACNFPRB( 1 ) ;
#endif	
	lapFlags.rsvDataCnf	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORESET)


static	IrDABOOL	lapCallResetInd( void )
{
	return((*ResetIndPtr)()) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	dword		lapGetAddress( const byte IrDAFAR *bufp )
{
	dword	addr ;


	MiscGetBigEndian32( bufp, &addr ) ;
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapGetAddress()" ) ;
	_DPUTS( DL_LAPINFO, " bufp" ) ;
	_DPUTB( DL_LAPINFO, "=", bufp[0] ) ;
	_DPUTB( DL_LAPINFO, ".", bufp[1] ) ;
	_DPUTB( DL_LAPINFO, ".", bufp[2] ) ;
	_DPUTB( DL_LAPINFO, ".", bufp[3] ) ;
	_DPUTD( DL_LAPINFO, " addr:", addr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	return( addr ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	dword		lapSetAddress( byte IrDAFAR *bufp, dword addr )
{
	MiscSetBigEndian32( bufp, addr ) ;
	return( addr ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapIsRecvAddress( dword addr )
{
	if( addr == 0xFFFFFFFFL || addr == MyAddress )
		return( TRUE ) ;
	return( FALSE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSetupMyDeviceAddress( IrDABOOL useslot )
{
	if( !lapFlags.noAddress )
		return ;


	lapFlags.noAddress	= FALSE ;
	if( useslot )
		MyAddress			= lapMakeDeviceAddressBySlot() ;
	else
		MyAddress			= MiscRandom32( 0xFFFFFFFDL ) + 1 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	dword		lapMakeDeviceAddressBySlot( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapMakeDeviceAddressBySlot(): setup NewAddr\n" ) ;
#endif	
	return((( MiscRandom32( 0xFFFFFFFDL ) + 1 ) & 0xFFFFFFF0L ) + DiscoveryMySlot ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapGetDiscoveryInfo( DISCOVERYLOG IrDAFAR *des, const byte IrDAFAR *frame, uint framesz )
{
	if( des == 0 )
		return ;


	des->Address			= SrcAddress ;
	des->DeviceNameSize		= framesz - XID_Info ;
	if( des->DeviceNameSize > IrLAP_NNMSZ )
		des->DeviceNameSize	= IrLAP_NNMSZ ;
	IrDA_memcpy( des->DeviceName, &frame[XID_Info], des->DeviceNameSize ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapInitCallBackFunctions( void )
{
	lapSnrmPrv.funcp	= lapDmySnrmPrv ;
	lapSnrmPrv.pcp		= 0 ;


	DiscoveryCnfPtr		= lapDmyDiscoveryCnf ;
	ConnectIndPtr		= lapDmyConnectInd ;
	DisconnectIndPtr	= lapDmyDisconnectInd ;
	DataCnfPtr			= lapDmyDataCnf ;
	DataIndPtr			= lapDmyDataInd ;
#if !defined(IrLAPNOUI)
	UDataIndPtr			= lapDmyUDataInd ;
	UDataCnfPtr			= lapDmyUDataCnf ;
#endif	
#if !defined(IrLAPNORESET)
	ResetIndPtr			= lapDmyResetInd ;
#endif	
	DiscoveryPrvPtr		= lapDmyDiscoveryPrv ;
	ConnectPrvPtr		= lapDmyConnectPrv ;
	ConnectPrvPtr2		= lapDmyConnectPrv2 ;
	DisconnectPrvPtr	= lapDmyDisconnectPrv ;
	AplConnectIndPtr	= lapDmyAplConnectInd ;
	AplLinkIndPtr		= lapDmyAplLinkInd ;
	TxEmptyPrvPtr		= lapDmyTxEmptyPrv ;
#if !defined(IrLAPNORXCHG)
	PrimaryIndPtr		= lapDmyPrimaryInd ;
	PrimaryCnfPtr		= lapDmyPrimaryCnf ;
#endif	
#if !defined(IrLAPNOSNIFF)
	SniffIndPtr			= lapDmySniffInd ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapResetRxBuffer( void )
{
	if( lapFlags.rstRxBuffer )
		return ;
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapResetRxBuffer():" ) ;
	_DPUTS( DL_LAPINFO, " call IrSIR_ResetRecvMode()" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapFlags.rstRxBuffer	= TRUE ;
	RecvFrameReadPtr		= 0 ;
	RecvFrameSize			= 0 ;
	IrSIR_ResetRecvMode() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopAllTimers( void )
{
	lapStopSlotTimer() ;
	lapStopQueryTimer() ;
#if defined(IrDAPRIMARY)
	lapStopFTimer() ;
	lapStopPTimer() ;
#endif	
	lapStopWDTimer() ;
#if !defined(IrLAPNOSNIFF)
	lapStopListenTimer() ;
	lapStopSenseTimer() ;
	lapStopSniffTimer() ;
	lapStopSleepTimer() ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapInitCapacityParam( PORTCONFIG2 *cap )
{
	cap->LineCapability		= IrLAP_BPS_9600 ;
	cap->MaxTanaroundTime	= IrLAP_MaxTurn_500ms ;
	cap->FrameSize			= IrLAP_FS_64 ;
	cap->WindowSize			= IrLAP_WS_1 ;
	cap->NoofBOF			= IrLAP_BOF_48 ;
	cap->MinTurnaroundTime	= IrLAP_MinTurn_10ms ;
	cap->DisconnectTime		= IrLAP_SL_3s ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapGetCapacityParam( PORTCONFIG2 IrDAFAR *cap, const byte IrDAFAR *src, uint len )
{
	int		i ;
#if defined(IrDABROKENCOMPILER01)
	int		pi ;
	byte	pl, pv ;
#else	
	byte	pi, pl, pv ;
#endif	


	for( i=0 ; i < (int)len ; ) {
		pi	= *src++ ;
		pl	= *src++ ;
		pv	= *src ;


#if	defined(DMSG2)
			_DPUTS( DL_LAPINFO, " LAP::lapGetCapacityParam():" ) ;
			_DPUTB( DL_LAPINFO, " pi:", pi ) ;
			_DPUTB( DL_LAPINFO, " pl:", pl ) ;
			_DPUTB( DL_LAPINFO, " pv:", pv ) ;
{	int	j ;
			for( j=1 ; j<pl ; j++ )
				_DPUTB( DL_LAPINFO, ".", src[j] ) ;
}
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		switch( pi ) {
		case	0x01 :								
			cap->LineCapability		= pv | IrLAP_BPS_9600 ;
			if( pl > 1 )
				cap->LineCapability	|= ((word)src[1]) << 8 ;
			break ;
		case	0x82 :								
			cap->MaxTanaroundTime	= pv | IrLAP_MaxTurn_500ms ;
			break ;
		case	0x83 :								
			cap->FrameSize			= pv | IrLAP_FS_64 ;
			break ;
		case	0x84 :								
			cap->WindowSize			= pv | IrLAP_WS_1 ;
			break ;
		case	0x85 :								
			cap->NoofBOF			= pv | IrLAP_BOF_48 ;
			break ;
		case	0x86 :								
			cap->MinTurnaroundTime	= pv | IrLAP_MinTurn_10ms ;
			break ;
		case	0x08 :								
			cap->DisconnectTime		= pv | IrLAP_SL_3s ;
			break ;
		}


		i	+= 1 + pl + 1 ;
		src	+= pl ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	word		lapSetCapacityParam( byte IrDAFAR *des, const PORTCONFIG2 *cap )
{
	word	len ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSetCapacityParam():" ) ;
	_DPUTW( DL_LAPINFO, " LineCapability:", cap->LineCapability ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	len		= 0 ;
#if defined(IrDAFORTRANXIT01)
{	word	LineCapa ;
	LineCapa	= cap->LineCapability & TargetPortConfig.LineCapability ;
	len		+= lapSetCapacity( &des[len], 0x01, LineCapa ) ;
}
#else	
	len		+= lapSetCapacity( &des[len], 0x01, cap->LineCapability ) ;
#endif	
	len		+= lapSetCapacity( &des[len], 0x82, cap->MaxTanaroundTime ) ;
	len		+= lapSetCapacity( &des[len], 0x83, cap->FrameSize ) ;
	len		+= lapSetCapacity( &des[len], 0x84, cap->WindowSize ) ;
	len		+= lapSetCapacity( &des[len], 0x85, cap->NoofBOF ) ;
	len		+= lapSetCapacity( &des[len], 0x86, cap->MinTurnaroundTime ) ;
	len		+= lapSetCapacity( &des[len], 0x08, cap->DisconnectTime ) ;


	return( len ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapNegotiateConnectionParameters( PORTCONFIG2 *des, const PORTCONFIG2 *src )
{
	lapMakeBestBit( &des->LineCapability, src->LineCapability ) ;


	lapMakeBestBit( &des->DisconnectTime, src->DisconnectTime ) ;


}
#endif	


#if !defined(IrDANOSTATIC)


static	word		lapMakeBestBit( word *des, word src )
{
	*des	&= src ;
	if( *des == 0 )
		*des	|= src ;


	return( *des ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	word		lapSetCapacity( byte IrDAFAR *des, byte sig, word data )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSetCapacity():" ) ;
	_DPUTB( DL_LAPINFO, " pi:", sig ) ;
	_DPUTB( DL_LAPINFO, " pl:", (data & 0xFF00)? 2 : 1 ) ;
	_DPUTB( DL_LAPINFO, " pv:", (byte)( data & 0xFF )) ;
	if( data & 0xFF00 )
		_DPUTB( DL_LAPINFO, " ", (byte)(( data >> 8 ) & 0xFF )) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	*des++	= sig ;
	des[0]	= 1 ;
	des[1]	= (byte)( data & 0xFF ) ;
	if( data & 0xFF00 ) {
		des[2]	= (byte)( data >> 8 ) ;
		des[0]	++ ;
		return( 4 ) ;
	}


	return( 3 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapApplyConnectionParameters( void )
{
	dword			maxFrameLen ;
	dword			maxFrameTime ;
	dword			bitRate ;
	dword			myMinTAT ;
	dword			maxTxSz ;


	IrLAP_ConnSpeed		= lapGetLineCapability( TargetPortConfig.LineCapability ) ;


	IrLAP_ConnMaxTAT	= lapGetMaxTanaroundTime( TargetPortConfig.MaxTanaroundTime ) ;
	myMinTAT			= lapGetMinTurnaroundTime( MyPortConfig.MinTurnaroundTime ) ;
	if( IrLAP_ConnMaxTAT < myMinTAT )
		IrLAP_ConnMaxTAT	= myMinTAT ;


	if( IrLAP_ConnSpeed < 115200L && IrLAP_ConnMaxTAT < 25 )
		IrLAP_ConnMaxTAT	= 25 ;
	if( IrLAP_ConnMaxTAT == 500 )
		IrLAP_ConnMaxTAT	= 400 ;


	if( IrLAP_ConnSpeed <= 115200L )
		IrLAP_ConnBOFSize	= lapGetBOFSize( TargetPortConfig.NoofBOF ) / ((word)IrDA_LDiv( 115200L, IrLAP_ConnSpeed )) ;
	else
		IrLAP_ConnBOFSize	= 0 ;


	ConnDisconnectMSec	= lapGetDisconnectMSec( TargetPortConfig.DisconnectTime ) ;
	ConnSendFrameSize	= lapGetFrameSize( TargetPortConfig.FrameSize ) ;
	ConnSendWindowSize	= lapGetWindowSize( TargetPortConfig.WindowSize ) ;
	if( ConnSendWindowSize > LAP_TxWindowSize )
		ConnSendWindowSize	= LAP_TxWindowSize ;
	IrLAP_ConnMinTurnAround
						= lapGetMinTurnaroundTime( TargetPortConfig.MinTurnaroundTime ) ;


	ConnRetryCountN2	= (word)IrDA_LDiv( ConnDisconnectMSec, IrLAP_ConnMaxTAT ) ;
	ConnRetryCountN1	= ConnRetryCountN2 >> 1 ;


	if( ConnSendFrameSize > LAP_TxFrameSize - 2 )
		ConnSendFrameSize	= LAP_TxFrameSize - 2 ;


	
	maxTxSz	= IrDA_LDiv( IrDA_LMul( IrDA_LDiv( IrLAP_ConnSpeed, 10 ), IrLAP_ConnMaxTAT ), 1000 ) ;
	if( maxTxSz < 64 )
		maxTxSz	= 64 ;
	if( maxTxSz < ConnSendFrameSize * ConnSendWindowSize ) {
		if( maxTxSz < ConnSendFrameSize ) {
			ConnSendFrameSize	= maxTxSz ;
			ConnSendWindowSize	= 1 ;
		} else {
			ConnSendWindowSize	= IrDA_LDiv( maxTxSz, ConnSendFrameSize ) ;
		}
	}


#if	defined(DMSG2)
	{word	w ;
	w	= 0 ;
	if( ConnDisconnectMSec != 0L )
		w	= (word)IrDA_LDiv( ConnDisconnectMSec, 1000L ) ;
	_DPUTD( DL_LAPINFO, " LAP::lapApplyConnectionParameters(): Connect Speed    :", IrLAP_ConnSpeed ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTW( DL_LAPINFO, "                                 : DisconnectMSec   :", w ) ;
	_DPUTS( DL_LAPINFO, " msec\n" ) ;
	_DPUTD( DL_LAPINFO, "                                 : ConnSendFrameSize:", ConnSendFrameSize ) ;
	_DPUTS( DL_LAPINFO, " Bytes\n" ) ;
	}
#endif	
	
	
	
	
	
	bitRate			= IrDA_LDiv( IrLAP_ConnSpeed, 10L ) ;
	bitRate			= IrDA_LDiv( 1000000L, bitRate ) ;
	maxFrameLen		= ( lapGetFrameSize( MyPortConfig.FrameSize ) * 2 ) + 17 ;


	maxFrameTime	= IrDA_LDiv( IrDA_LMul( bitRate, maxFrameLen ), 1000L ) + myMinTAT ;
	MaxInFrameRetryCount	= (int)( IrDA_LDiv( maxFrameTime, IrLAP_ConnMaxTAT )) + 1 ;


#if	defined(DMSG3)
	_DPUTD( DL_LAPINFO, "                                 : bitRate:", bitRate ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTD( DL_LAPINFO, "                                 : maxFrameLen:", maxFrameLen ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTD( DL_LAPINFO, "                                 : myMinTAT:", myMinTAT ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTD( DL_LAPINFO, "                                 : maxFrameTime:", maxFrameTime ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTW( DL_LAPINFO, "                                 : MaxInFrameRetryCount:", MaxInFrameRetryCount ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	dword		lapGetLineCapability( word c )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapGetLineCapability():" ) ;
	_DPUTW( DL_LAPINFO, " c:", c ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( c & IrLAP_BPS_4000000 )		return( 4000000L ) ;
	if( c & IrLAP_BPS_1152000 )		return( 1152000L ) ;
	if( c & IrLAP_BPS_576000 )		return( 576000L ) ;
	if( c & IrLAP_BPS_115200 )		return( 115200L ) ;
	if( c & IrLAP_BPS_57600 )		return( 57600L ) ;
	if( c & IrLAP_BPS_38400 )		return( 38400L ) ;
	if( c & IrLAP_BPS_19200 )		return( 19200L ) ;
	if( c & IrLAP_BPS_9600 )		return( 9600L ) ;
	if( c & IrLAP_BPS_2400 )		return( 2400L ) ;
	return( 9600L ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	word		lapGetMaxTanaroundTime( word c )
{
	if( c & IrLAP_MaxTurn_5ms )			return( 5 ) ;
	if( c & IrLAP_MaxTurn_10ms )		return( 10 ) ;
	if( c & IrLAP_MaxTurn_25ms )		return( 25 ) ;
	if( c & IrLAP_MaxTurn_50ms )		return( 50 ) ;
	if( c & IrLAP_MaxTurn_100ms )		return( 100 ) ;
	if( c & IrLAP_MaxTurn_250ms )		return( 250 ) ;
	return( 500 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetBOFSize( word c )
{
	if( c & IrLAP_BOF_0 )			return( 0 ) ;
	if( c & IrLAP_BOF_1 )			return( 1 ) ;
	if( c & IrLAP_BOF_2 )			return( 2 ) ;
	if( c & IrLAP_BOF_3 )			return( 3 ) ;
	if( c & IrLAP_BOF_5 )			return( 5 ) ;
	if( c & IrLAP_BOF_12 )			return( 12 ) ;
	if( c & IrLAP_BOF_24 )			return( 24 ) ;
	if( c & IrLAP_BOF_48 )			return( 48 ) ;
	return( 48 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	dword		lapGetDisconnectMSec( word c )
{
	if( c & IrLAP_SL_40s )			return( 40000L ) ;
	if( c & IrLAP_SL_30s )			return( 30000L ) ;
	if( c & IrLAP_SL_25s )			return( 25000L ) ;
	if( c & IrLAP_SL_20s )			return( 20000L ) ;
	if( c & IrLAP_SL_16s )			return( 16000L ) ;
	if( c & IrLAP_SL_12s )			return( 12000L ) ;
	if( c & IrLAP_SL_8s )			return( 8000L ) ;
	if( c & IrLAP_SL_3s )			return( 3000L ) ;
	return( 40000L ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetFrameSize( word c )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapGetFrameSize():" ) ;
	_DPUTW( DL_LAPINFO, " c:", c ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( c & IrLAP_FS_2048 )		return( 2048 ) ;
	if( c & IrLAP_FS_1024 )		return( 1024 ) ;
	if( c & IrLAP_FS_512 )		return( 512 ) ;
	if( c & IrLAP_FS_256 )		return( 256 ) ;
	if( c & IrLAP_FS_128 )		return( 128 ) ;
	if( c & IrLAP_FS_64 )		return( 64 ) ;
	return( 64 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetWindowSize( word c )
{
	if( c & IrLAP_WS_7 )	return( 7 ) ;
	if( c & IrLAP_WS_6 )	return( 6 ) ;
	if( c & IrLAP_WS_5 )	return( 5 ) ;
	if( c & IrLAP_WS_4 )	return( 4 ) ;
	if( c & IrLAP_WS_3 )	return( 3 ) ;
	if( c & IrLAP_WS_2 )	return( 2 ) ;
	if( c & IrLAP_WS_1 )	return( 1 ) ;
	return( 1 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetMinTurnaroundTime( word c )
{
#if defined(IrLAPMINTATMICRO)
	if( c & IrLAP_MinTurn_0ms )		return( 0 ) ;
	if( c & IrLAP_MinTurn_001ms )	return( 1 ) ;
	if( c & IrLAP_MinTurn_005ms )	return( 5 ) ;
	if( c & IrLAP_MinTurn_01ms )	return( 10 ) ;
	if( c & IrLAP_MinTurn_05ms )	return( 50 ) ;
	if( c & IrLAP_MinTurn_1ms )		return( 100 ) ;
	if( c & IrLAP_MinTurn_5ms )		return( 500 ) ;
	if( c & IrLAP_MinTurn_10ms )	return( 1000 ) ;
	return( 10 ) ;
#else	
	if( c & IrLAP_MinTurn_0ms )		return( 0 ) ;
# if defined(IrLAPSTRICTMINTAT)
	if( c & IrLAP_MinTurn_001ms )	return( 1 ) ;
	if( c & IrLAP_MinTurn_005ms )	return( 1 ) ;
	if( c & IrLAP_MinTurn_01ms )	return( 1 ) ;
	if( c & IrLAP_MinTurn_05ms )	return( 1 ) ;
# else	
	if( c & IrLAP_MinTurn_001ms )	return( 0 ) ;
	if( c & IrLAP_MinTurn_005ms )	return( 0 ) ;
	if( c & IrLAP_MinTurn_01ms )	return( 0 ) ;
	if( c & IrLAP_MinTurn_05ms )	return( 0 ) ;
# endif	
	if( c & IrLAP_MinTurn_1ms )		return( 1 ) ;
	if( c & IrLAP_MinTurn_5ms )		return( 5 ) ;
	if( c & IrLAP_MinTurn_10ms )	return( 10 ) ;
	return( 10 ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapGetXIDSlotSize( byte c )
{
static	const word tbl[] = { 1,6,8,16 };


	return( tbl[ c & 0x03 ] );
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendTESTRsp( const byte IrDAFAR *frame, uint framesz )
{
	if( framesz > IrLAP_CMDBUFSZ )
		framesz	= IrLAP_CMDBUFSZ ;


	IrDA_memcpy( LAP_SendCmdBuffer, frame, framesz ) ;
	LAP_SendCmdBuffer[0]	&= ~IrLAP_CR ;
	lapSetAddress( &LAP_SendCmdBuffer[TEST_SrcAddress], lapGetAddress( &frame[TEST_DesAddress] )) ;
	lapSetAddress( &LAP_SendCmdBuffer[TEST_DesAddress], lapGetAddress( &frame[TEST_SrcAddress] )) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( framesz, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDA_DELAIEDSECONDARY)
static	void		lapDelaySendRRRsp( void )
{
	lapStopDTimer() ;
	if( DelayTimer == 0L ) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRRRsp() ;
		return ;
	}


	if(( DTimerHndl = StartTimer( DelayTimer, lapDTimerExpired )) == -1 ) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRRRsp() ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendRNRRsp( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendRNRRsp():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= IrLAP_ConnectionAddress << 1 ;
	LAP_SendCmdBuffer[1]	= IrLAP_RNRRsp | IrLAP_PF | ( IrLAP_Vr << 5 ) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendIFrame( byte pf, byte crf )
{
	byte IrDAFAR	*txptr ;
	uint			txsize ;


	txptr	= LAP_SendCtl[LAP_SendWinSendPtr].ptr ;
	txsize	= LAP_SendCtl[LAP_SendWinSendPtr].size ;
	LAP_SendCtl[LAP_SendWinSendPtr].vs		= IrLAP_Vs ;
	txptr[0]	= ( IrLAP_ConnectionAddress << 1 ) | crf ;
	txptr[1]	= pf | (( IrLAP_Vs & 7 ) << 1 ) | (( IrLAP_Vr & 7 ) << 5 ) ;
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapSendIFrame():" ) ;
	_DPUTB( DL_LAPINFO, " Vs:", ( LAP_SendCtl[LAP_SendWinSendPtr].ptr[1] >> 1 ) & 7 ) ;
	_DPUTB( DL_LAPINFO, " Vr:", ( LAP_SendCtl[LAP_SendWinSendPtr].ptr[1] >> 5 ) & 7 ) ;
	_DPUTB( DL_LAPINFO, " PF:", pf ) ;
	_DPUTB( DL_LAPINFO, " SendWinSendPtr:", LAP_SendWinSendPtr ) ;
	_DPUTS( DL_LAPINFO, "\n" ); 
	_DPUTS( DL_LAPDAT, "                  :" ) ;
	_DPUTS( DL_LAPDAT, " data:" ) ;
	_DDUMP( DL_LAPDAT, LAP_SendCtl[LAP_SendWinSendPtr].ptr, LAP_SendCtl[LAP_SendWinSendPtr].size, 0 ) ;
	_DPUTS( DL_LAPDAT, "\n" ); 
#endif	
	LAP_SendWinSendSize	++ ;
	LAP_SendCtl[LAP_SendWinSendPtr].xmit	= TRUE ;
	LAP_SendWinSendPtr	= lapNextSendWindowPtr( LAP_SendWinSendPtr ) ;
	IrLAP_Vs		= ( IrLAP_Vs + 1 ) & 7 ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( txsize, txptr ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapResendIFrame( void )
{
	LAP_SendCtl[LAP_SendWinReadPtr].ptr[1]	&= ~( 7 << 5 ) ;
	LAP_SendCtl[LAP_SendWinReadPtr].ptr[1]	|= IrLAP_PF + (( IrLAP_Vr & 7 ) << 5 ) ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapResendIFrame():" ) ;
	_DPUTB( DL_LAPINFO, " Vs:", ( LAP_SendCtl[LAP_SendWinReadPtr].ptr[1] >> 1 ) & 7 ) ;
	_DPUTB( DL_LAPINFO, " Vr:", ( LAP_SendCtl[LAP_SendWinReadPtr].ptr[1] >> 5 ) & 7 ) ;
	_DPUTB( DL_LAPINFO, " SendWinReadPtr:", LAP_SendWinReadPtr ) ;
	_DPUTS( DL_LAPINFO, "\n" ); 
#endif	
#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_LAPDAT, "                    :" ) ;
	_DPUTB( DL_LAPDAT, " data:", LAP_SendCtl[LAP_SendWinReadPtr].ptr[0] ) ;
	for( i=1 ; i<LAP_SendCtl[LAP_SendWinReadPtr].size ; i++ )
		_DPUTB( DL_LAPDAT, ".", LAP_SendCtl[LAP_SendWinReadPtr].ptr[i] ) ;
	_DPUTS( DL_LAPDAT, "\n" ); 
}
#endif	
	lapResetRxBuffer() ;
	IrSIR_SendFrame( LAP_SendCtl[LAP_SendWinReadPtr].size, LAP_SendCtl[LAP_SendWinReadPtr].ptr ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapUpdateNrReceived( void )
{
	uint		txsz ;


	for( txsz = 0 ; LAP_SendWinSendSize > 0 ; ) {
		if( LAP_SendCtl[LAP_SendWinReadPtr].vs == IrLAP_Nr )
			break ;
		if( LAP_SendCtl[LAP_SendWinReadPtr].size == 0 )
			break ;


#if	defined(DMSG2)
		_DPUTS( DL_LAPINFO, " LAP::lapUpdateNrReceived():" ) ;
		_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
		_DPUTB( DL_LAPINFO, " Rp:", LAP_SendWinReadPtr ) ;
		_DPUTB( DL_LAPINFO, " Vs:", LAP_SendCtl[LAP_SendWinReadPtr].vs ) ;
		_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		txsz	+= LAP_SendCtl[LAP_SendWinReadPtr].size - 2 ;
		LAP_SendCtl[LAP_SendWinReadPtr].size	= 0 ;


		LAP_SendWinReadPtr	= lapNextSendWindowPtr( LAP_SendWinReadPtr ) ;
		LAP_SendWinSendSize	-- ;
		lapFlags.rsvDataCnf	= TRUE ;
	}


	*TxSizePtr	+= (ulong)txsz ;
	lapCallDataCnf() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	uint		lapNextSendWindowPtr( uint ptr )
{
#if defined(TYPES2DEBUG2)
	if( (( LAP_SendWinReadPtr == LAP_SendWinWritePtr ) &&
		( LAP_SendWinUsedSize != 0 && LAP_SendWinUsedSize != LAP_TxWindowSize ))	||
		( LAP_SendWinReadPtr < LAP_SendWinWritePtr &&
			LAP_SendWinReadPtr + LAP_SendWinUsedSize != LAP_SendWinWritePtr )		||
		( LAP_SendWinWritePtr < LAP_SendWinReadPtr &&
			LAP_SendWinReadPtr + LAP_SendWinUsedSize - LAP_TxWindowSize != LAP_SendWinWritePtr ))
		breakpoint() ;
#endif	
	if( ++ptr < LAP_TxWindowSize )
		return( ptr ) ;
	return( 0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapChkNs( void )
{
	return( IrLAP_Vr == IrLAP_Ns ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapChkNr( void )
{
	return( IrLAP_Nr == IrLAP_Vs ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapInvalidNr( void )
{
	int		diff ;


	if( lapChkNr())
		return( FALSE ) ;


	diff	= ( IrLAP_Nr + 8 - LAP_SendCtl[LAP_SendWinReadPtr].vs ) & 7 ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapInvalidNr():" ) ;
	_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
	_DPUTB( DL_LAPINFO, " LAP_SendCtl[", LAP_SendWinReadPtr ) ;
	_DPUTB( DL_LAPINFO, "].vs:", LAP_SendCtl[LAP_SendWinReadPtr].vs ) ;
	_DPUTB( DL_LAPINFO, " LAP_SendWinSendSize:", LAP_SendWinSendSize ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( diff <= (int)LAP_SendWinSendSize )
		return( FALSE ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapInvalidNr(): INVALID!!\n" ) ;
#endif	
	return( TRUE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapInvalidNs( void )
{
	int		diff ;


	if( lapChkNs())
		return( FALSE ) ;


	diff	= ( IrLAP_Ns + 8 - IrLAP_Vr ) & 7 ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapInvalidNs():" ) ;
	_DPUTB( DL_LAPINFO, " Ns:", IrLAP_Ns ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTW( DL_LAPINFO, " diff:", diff ) ;
	_DPUTB( DL_LAPINFO, " LAP_RxWIndowSize:", LAP_RxWIndowSize ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( diff < (int)LAP_RxWIndowSize )
		return( FALSE ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapInvalidNs(): INVALID!!\n" ) ;
#endif	
	return( TRUE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapChkDataRequest( void )
{
	IrDABOOL	done ;


	if( IrLAP_ConnRemoteBusy )
		return( FALSE ) ;


	for( done=FALSE ;; done=TRUE ) {
		if( LAP_SendWinSendPtr != LAP_SendWinWritePtr )
			return( TRUE ) ;
		if( LAP_SendCtl[LAP_SendWinSendPtr].size > 0 &&
			!LAP_SendCtl[LAP_SendWinSendPtr].xmit )
			return( TRUE ) ;


		if( done )
			break ;
#if defined(IrDA_LAPTXEMPTYPRB)
		IrDA_LAPTXEMPTYPRB( 0 ) ;
#endif	
		(*TxEmptyPrvPtr)() ;
#if defined(IrDA_LAPTXEMPTYPRB)
		IrDA_LAPTXEMPTYPRB( 1 ) ;
#endif	
	}


	return( FALSE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopSlotTimer( void )
{
	if( SlotTimerHndl >= 0 ) {
		StopTimer( SlotTimerHndl ) ;
		SlotTimerHndl	= -1 ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopQueryTimer( void )
{
	if( QueryTimerHndl >= 0 ) {
		StopTimer( QueryTimerHndl ) ;
		QueryTimerHndl	= -1 ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopFTimer( void )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapStopFTimer()" ) ;
	_DPUTW( DL_LAPINFO, " FTimerHndl:", FTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( FTimerHndl >= 0 ) {
		StopTimer( FTimerHndl ) ;
		FTimerHndl		= -1 ;
	}
	lapStopLTimer() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopPTimer( void )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapStopPTimer()" ) ;
	_DPUTW( DL_LAPINFO, " PTimerHndl:", PTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( PTimerHndl >= 0 ) {
		StopTimer( PTimerHndl ) ;
		PTimerHndl		= -1 ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopWDTimer( void )
{
	if( WDTimerHndl >= 0 ) {
		StopTimer( WDTimerHndl ) ;
		WDTimerHndl		= -1 ;
	}
	lapStopLTimer() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapStopLTimer( void )
{
	if( LTimerHndl >= 0 ) {
		StopTimer( LTimerHndl ) ;
		LTimerHndl		= -1 ;
	}
}


#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDA_DELAIEDSECONDARY)


static	void		lapStopDTimer( void )
{
	if( DTimerHndl < 0 )
		return ;
	StopTimer( DTimerHndl ) ;
	DTimerHndl		= -1 ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_MissStage( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
#if	defined(DMSG3)
	{	int		i ;
		_DPUTS( DL_LAPDANGER, " LAP::lapRE_MissStage(): Miss frame/stage!!" ) ;
		_DPUTW( DL_LAPDANGER, " stage:", IrLAP_Stage ) ;
		_DPUTW( DL_LAPDANGER, " size:", framesz ) ;
		_DPUTS( DL_LAPDANGER, " data=" ) ;
		for( i=0 ; i<framesz ; i++ )
			_DPUTB( DL_LAPDANGER, " ", frame[i] ) ;
		_DPUTS( DL_LAPDANGER, "\n" ) ;
	}
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_NDMXXXCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if( frame[1] & IrLAP_PF )
		lapSendDMRsp( IrLAP_BroadCast ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_NDMXIDCmd( const byte IrDAFAR *frame, uint framesz )
{
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_NDMXIDCmd():" ) ;
	_DPUTW( DL_LAPINFO, " MyPortConfig.LineCapability:", MyPortConfig.LineCapability ) ;
	_DPUTS( DL_LAPINFO, "\r\n" ) ;
#endif	
	if( lapFlags.refuseDiscovery )					
		return ;


	if( frame[XID_SlotNo] == 0xFF )					
		return ;
	IrLAP_OriginalPowerMode	= IrDA_GetPowerLevel() ;
	IrDA_SetPowerLevel( 2 ) ;
													
	DiscoveryMySlot	= (uint)MiscRandom8( (byte)( lapGetXIDSlotSize( frame[XID_DiscoveryFlag] ) - 1 ) ) ;
	if( DiscoveryMySlot == 0 )
		IrDA_SetPowerLevel( 3 ) ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_NDMXIDCmd():" ) ;
	_DPUTB( DL_LAPINFO, " MySlot:", (byte)DiscoveryMySlot ) ;
	_DPUTS( DL_LAPINFO, "\r\n" ) ;
#endif	
	
#if defined(IrDANEWADDRBYDISCOVERY)
	if(( frame[XID_DiscoveryFlag] & XID_AddressConfilict ) == 0 )
		MyAddress	= lapMakeDeviceAddressBySlot() ;
#else	
	lapSetupMyDeviceAddress( ON ) ;
#endif	
	if( MyAddress == SrcAddress ) {
		for( MyAddress = lapMakeDeviceAddressBySlot() ;
			MyAddress == SrcAddress ;
			MyAddress = lapMakeDeviceAddressBySlot()) ;
	}


	
	NewAddress			= MyAddress ;
	lapFlags.setNewAddr	= TRUE ;
	if( frame[XID_DiscoveryFlag] & XID_AddressConfilict ) {
		for( NewAddress = lapMakeDeviceAddressBySlot() ;
			NewAddress == SrcAddress || NewAddress == DesAddress ;
			NewAddress = lapMakeDeviceAddressBySlot()) ;
	}


												
#if	defined(DMSG3)
	_DPUTS( DL_LAPDANGER, " LAP::lapRE_NDMXIDCmd(): start query timer\r\n" ) ;
#endif	


	DiscoverySlot		= 0xFF ;
	DiscoveryFrameSent	= FALSE ;				
	IrLAP_Stage			= IrLAP_S_REPLY ;
	lapRE_REPLYXIDCmd( frame, framesz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_NDMSNRMCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopWDTimer() ;
	lapStopQueryTimer() ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_NDMSNRMCmd():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;


	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	TargetAddress			= SrcAddress ;
	IrLAP_ConnectionAddress	= frame[SNRM_ConnectAddress] >> 1 ;


													
	lapInitCapacityParam( &TargetPortConfig ) ;
	lapGetCapacityParam( &TargetPortConfig, &frame[SNRM_NegotiationParm], framesz - SNRM_NegotiationParm ) ;


	if( lapCallConnectInd() == IrDA_CONNECTDI ) {
		lapSendDMRsp( IrLAP_ConnectionAddress ) ;		
		return ;
	}


	if( lapSnrmPrv.pcp )
		IrDA_memcpy( lapSnrmPrv.pcp, &TargetPortConfig, sizeof( PORTCONFIG2 )) ;
	lapFlags.sendingUARsp	= TRUE ;
	(*lapSnrmPrv.funcp)() ;
													
	lapNegotiateConnectionParameters( &TargetPortConfig, &MyPortConfig ) ;
	lapApplyConnectionParameters() ;				
	IrSIR_SetDataCnf( lapCbSetRecvMode ) ;
	lapSendUARsp( ON ) ;							
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_NDMSNRMCmd():" ) ;
	_DPUTD( DL_LAPINFO, " Start-WD-timer:", ConnDisconnectMSec ) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_NDMUICmd( const byte IrDAFAR *frame, uint framesz )
{
#if !defined(IrLAPNOUI)
	lapCallUDataInd( frame, framesz ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_REPLYXIDCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopQueryTimer() ;								


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_REPLYXIDCmd():" ) ;
	_DPUTW( DL_LAPINFO, " MyPortConfig.LineCapability:", MyPortConfig.LineCapability ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
#if !defined(GENOA_A1120)
	if( DiscoverySlot != 0xFF && DiscoverySlot >= frame[XID_SlotNo] ) {
		lapRE_NDMXIDCmd( frame, framesz ) ;
		return ;
	}
#endif	


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_REPLYXIDCmd():" ) ;
	_DPUTB( DL_LAPINFO, " ", frame[XID_SlotNo] ) ;
	_DPUTB( DL_LAPINFO, "/", lapGetXIDSlotSize( frame[XID_DiscoveryFlag] )) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	
	if( frame[XID_SlotNo] == 0xFF ) {
		IrDA_SetPowerLevel( IrLAP_OriginalPowerMode ) ;
		IrLAP_Stage			= IrLAP_S_NDM ;
		if( lapFlags.setNewAddr ) {
			lapFlags.setNewAddr	= FALSE ;
			MyAddress			= NewAddress ;
		}
		lapGetDiscoveryInfo( PrvDiscLog, frame, framesz ) ;
		lapGetDiscoveryInfo( AplDiscLog, frame, framesz ) ;
		(*DiscoveryPrvPtr)() ;
		return ;
	}


	
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_REPLYXIDCmd():" ) ;
	_DPUTB( DL_LAPINFO, " DiscoveryFrameSent:", DiscoveryFrameSent ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	DiscoverySlot	= frame[XID_SlotNo] ;
	if( frame[XID_SlotNo] < DiscoveryMySlot || DiscoveryFrameSent ) {
													
		QueryTimerHndl	= StartTimer( TIME_QUERYTIMER, lapCbQueryTimerExpired ) ;
		return ;
	}


	
	IrDA_SetPowerLevel( 3 ) ;
	DiscoveryFrameSent	= TRUE ;					
	IrSIR_SetDataCnf( lapCbSlowDownPower ) ;
#if defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_REPLYXIDCmd(): " ) ;
	_DPUTW( DL_LAPINFO, " EnableDiscoveryAnswer:", EnableDiscoveryAnswer ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( EnableDiscoveryAnswer )
		lapSendXIDRsp( SrcAddress, frame[XID_DiscoveryFlag], DiscoveryMySlot ) ;


													
	QueryTimerHndl	= StartTimer( TIME_QUERYTIMER, lapCbQueryTimerExpired ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SBusyWaitDISCCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SBusyWaitDISCCmd(): Link Shutdown\n" ) ;
#endif	
	lapStopWDTimer() ;
	IrSIR_SetDataCnf( lapCbDefaultConnState ) ;
	lapSendUARsp( OFF ) ;								
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SBusyWaitUICmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;						
		return ;
	}


	if( !lapChkDataRequest()) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRNRRsp() ;
		return ;
	}


	lapKickTxIFrame2() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SBusyWaitXIDCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRNRRsp() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SBusyWaitRRCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;
		return ;
	}


	IrLAP_ConnRemoteBusy	= FALSE ;				
	lapUpdateNrReceived() ;							
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SBusyWaitRRCmd():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_ConnRemoteBusy:", IrLAP_ConnRemoteBusy ) ;
	_DPUTW( DL_LAPINFO, " LAP_SendWinSendSize:", LAP_SendWinSendSize ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	if( !lapChkNr()) {								
		if( LAP_SendWinSendSize > 0 ) {
			lapResendIFrame() ;						
			return ;
		}


		lapSendFRMRRsp( FRMR_InvalidNr ) ;
		return ;
	}
	if( lapChkDataRequest()) {
		lapKickTxIFrame2() ;
		return ;
	}


	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRNRRsp() ;							
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SBusyWaitRNRCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
	if(( frame[1] & IrLAP_PF ) == 0 ) {
		lapCbStartWDTimer() ;
		return ;
	}


	lapUpdateNrReceived() ;							
	IrLAP_ConnRemoteBusy	= TRUE ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRNRRsp() ;							
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SBusyWaitRNRCmd():" ) ;
	_DPUTD( DL_LAPINFO, " NOW:", IrSIR_ReadTimer()) ;
	_DPUTW( DL_LAPINFO, " WDTimerHndl:", WDTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)
#if	defined(IrDA_ACCEPTBEFORERNR)
const char	irda_acceptbeforernr[]	= "IrDA_ACCEPTBEFORERNR" ;
#endif	


static	void		lapRE_SBusyWaitICmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
#if	defined(DMSG2)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SBusyWaitICmd():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTB( DL_LAPINFO, " Vs:", IrLAP_Vs ) ;
	_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
	_DPUTB( DL_LAPINFO, " Ns:", IrLAP_Ns ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	if( lapChkNs() && lapChkNr()) {
#if defined(IrDA_ACCEPTBEFORERNR)
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
#endif	
		lapUpdateNrReceived() ;						
#if defined(IrDA_ACCEPTBEFORERNR)
		lapCallDataInd( frame, framesz ) ;				
#endif	
		if(( frame[1] & IrLAP_PF ) == 0 ) {
			lapCbStartWDTimer() ;
			return ;
		}


		if( lapChkDataRequest()) {
			lapKickTxIFrame2() ;
			return ;
		}


		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRNRRsp() ;							
		return ;
	}


	if( !lapChkNs() && lapChkNr()) {
		lapUpdateNrReceived() ;						
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			lapSendRNRRsp() ;							
			return ;
		}


		lapCbStartWDTimer() ;						
		return ;
	}


	if( lapChkNs() && !lapChkNr()) {
#if defined(IrDA_ACCEPTBEFORERNR)
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
#endif	
		lapUpdateNrReceived() ;						
#if defined(IrDA_ACCEPTBEFORERNR)
		lapCallDataInd( frame, framesz ) ;				
#endif	
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
			if( !IrLAP_ConnRemoteBusy )
				lapResendIFrame() ;					
			else
				lapSendRNRRsp() ;
			return ;
		}
		lapCbStartWDTimer() ;						
		return ;
	}


	if( frame[1] & IrLAP_PF ) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		if( !IrLAP_ConnRemoteBusy )
			lapResendIFrame() ;						
		else
			lapSendRNRRsp() ;
		return ;
	}


	lapCbStartWDTimer() ;							
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SCloseDISCCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
	IrSIR_SetDataCnf( lapCbDefaultConnState ) ;
	lapSendUARsp( OFF ) ;								
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SCloseDMRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;								
	lapStateToNDM( IrDAR_DISCONNECT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SCloseXXX( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;									
	if( frame[1] & IrLAP_PF ) {
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRDRsp() ;								
		return ;
	}
	lapCbStartWDTimer() ;							
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapRE_PoutXIDCmd( const byte IrDAFAR *frame, uint framesz )
{
	if( lapFlags.refuseDiscovery ||					
		frame[XID_SlotNo] == 0xFF )					
		return ;


	
	
	
	
	lapStopSenseTimer() ;
	lapStopSniffTimer() ;


	lapRE_NDMXIDCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapRE_SniffSNRMCmd( const byte IrDAFAR *frame, uint framesz )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SniffSNRMCmd():\n" ) ;
#endif	
	
	
	lapStopSniffTimer() ;
	lapRE_NDMSNRMCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapRE_SniffXXX( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	
	
	lapStopSniffTimer() ;
	IrDA_PowerOff() ;
	lapStartSleepTimer() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapStartSenseTimer( void )
{
	lapStopSenseTimer() ;
	SenseTimerHndl	= StartTimer( TIME_SNIFFSENSE, lapCbSenseTimerExpired ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapStopSenseTimer( void )
{
	if( SenseTimerHndl < 0 )
		return ;
	StopTimer( SenseTimerHndl ) ;
	SenseTimerHndl	= -1 ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapCbSenseTimerExpired( void )
{
	SenseTimerHndl	= -1 ;


	switch( IrLAP_Stage ) {
	case	IrLAP_S_POUT :
		if( IrSIR_Busy()) {
			IrDA_PowerOff() ;
			lapStartSleepTimer() ;
		} else {
			IrLAP_Stage		= IrLAP_S_SNIFF ;
			IrSIR_SetDataCnf( lapCbStartSniffTimer ) ;
			lapSendSniffXIDRsp() ;
		}
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapCbStartSniffTimer( void )
{
	lapStopSniffTimer() ;
	SniffTimerHndl	= StartTimer( TIME_SNIFF, lapSniffTimerExpired ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapStopSniffTimer( void )
{
	if( SniffTimerHndl < 0 )
		return ;
	StopTimer( SniffTimerHndl ) ;
	SniffTimerHndl	= -1 ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapSniffTimerExpired( void )
{
	SniffTimerHndl	= -1 ;


	switch( IrLAP_Stage ) {
	case	IrLAP_S_SNIFF :
		IrDA_PowerOff() ;
		lapStartSleepTimer() ;
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapStartSleepTimer( void )
{
	lapStopSleepTimer() ;
	SleepTimerHndl	= StartTimer( TIME_SLEEP, lapSleepTimerExpired ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapStopSleepTimer( void )
{
	if( SleepTimerHndl < 0 )
		return ;
	StopTimer( SleepTimerHndl ) ;
	SleepTimerHndl	= -1 ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapSleepTimerExpired( void )
{
	SleepTimerHndl	= -1 ;


	switch( IrLAP_Stage ) {
	case	IrLAP_S_SNIFF :
		IrLAP_Stage		= IrLAP_S_POUT ;
	case	IrLAP_S_POUT :
		IrDA_PowerOn() ;
		lapStartSenseTimer() ;
		break ;
	}
}
#endif	
#endif	


#endif	

/* $Log: rcs/IRLAPM.CV $
 * Revision 0.1  96/07/14 00:00:00  core
 * 
*/
