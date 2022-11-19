


#if !defined(__plapps_h__)
#define	__plapps_h__


#if !defined(IrLAP_MORATORIUMTIME)
#define		IrLAP_MORATORIUMTIME	1000L
#endif	


#if !defined(IrLAPNEWADDRRETRYCOUNT)
#define		IrLAPNEWADDRRETRYCOUNT	3
#endif	


#if !defined(IrLAPSLOTTIME)
#define		IrLAPSLOTTIME			50
#endif	


#if !defined(TIME_WAITSNIFFXIDRSP)
#define		TIME_WAITSNIFFXIDRSP	5000L
#endif	


static	DISCOVERYLOG IrDAFAR	*lapDiscLogPtr ;
static	uint	lapDiscSlotCount ;				
static	uint	lapDiscMaxSlot ;				
static	uint	lapDiscSlot ;					
static	dword	MTimer ;						
static	dword	DiscTarget ;					
static	byte	DiscFlag ;						
#if !defined(IrLAPNONEWADDR)
static	int		NewAddrRetryCount ;				
static	uint	lapDiscConflictSlot ;			
static	dword	NewAddrHist[AUTONEWADDRHIST] ;	
#endif	


static	dword			DiscTimer ;				


#if !defined(IrLAPNORXCHG)
static	int		(IrDACB *PrimaryIndPtr)( void ) ;
static	void	(IrDACB *PrimaryCnfPtr)( int status ) ;
#define	rxchgRetryCountN4	3
static	int				rxchgRetryCount ;	
#endif	


static	void	(IrDACB *ConnectCnfPtr)( int status ) ;


static	void	lapRE_SETUPUARsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SETUPDMRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_QUERYXIDRsp( const byte IrDAFAR *frame, uint framesz ) ;
#if !defined(IrLAPFRMRDISC)
static	void	lapRE_PRecvFRMRRsp( const byte IrDAFAR *frame, uint framesz ) ;
#else	
#define			lapRE_PRecvFRMRRsp		lapRE_PRecvRDRsp
#endif	
static	void	lapRE_PRecvRDRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PRecvRRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PRecvRNRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PRecvIRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PBusyWaitRRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PBusyWaitRNRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PBusyWaitIRsp( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_PBusyWaitRDRsp		lapRE_PRecvRDRsp
static	void	lapRE_PCloseDMRsp( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_PCloseUARsp	lapRE_PCloseDMRsp
#if !defined(IrLAPNORXCHG)
static	void	lapRE_PRecvRXCHGRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SRXWaitDXCHGCmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SRXWaitDISCCmd		lapRE_SRecvDISCCmd
static	void	lapRE_SRXWaitXXXCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SXWaitDISCCmd		lapRE_SRecvDISCCmd
#define			lapRE_SXWaitRDRsp		lapRE_PRecvRDRsp
static	void	lapRE_SXWaitRRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SXWaitRNRRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SXWaitIRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SXWaitRXCHGRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitDISCCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitFRMRRsp( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_PXWaitRDRsp		lapRE_PRecvRDRsp
static	void	lapRE_PXWaitRRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitXXXRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitSNRMCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitTESTCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitUICmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitXIDCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitRNRCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitREJCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitSREJCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitIFrame( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_PXWaitDXCHGCmd( const byte IrDAFAR *frame, uint framesz ) ;
#else	
#define	lapRE_PRecvRXCHGRsp			lapRE_MissStage
#define	lapRE_SRXWaitDISCCmd		lapRE_MissStage
#define	lapRE_SRXWaitXXXCmd			lapRE_MissStage
#define	lapRE_SRXWaitXCHGCmd		lapRE_MissStage
#define	lapRE_SRXWaitDXCHGCmd		lapRE_MissStage
#define	lapRE_SXWaitDISCCmd			lapRE_MissStage
#define	lapRE_SXWaitRDRsp			lapRE_MissStage
#define	lapRE_SXWaitRRRsp		lapRE_MissStage
#define	lapRE_SXWaitRNRRsp		lapRE_MissStage
#define	lapRE_SXWaitIRsp			lapRE_MissStage
#define	lapRE_SXWaitRXCHGRsp		lapRE_MissStage
#define	lapRE_SXWaitXCHGCmd		lapRE_MissStage
#define	lapRE_PXWaitDISCCmd		lapRE_MissStage
#define	lapRE_PXWaitXXXRsp		lapRE_MissStage
#define	lapRE_PXWaitFRMRRsp		lapRE_MissStage
#define	lapRE_PXWaitRDRsp		lapRE_MissStage
#define	lapRE_PXWaitSNRMCmd		lapRE_MissStage
#define	lapRE_PXWaitTESTCmd		lapRE_MissStage
#define	lapRE_PXWaitUICmd		lapRE_MissStage
#define	lapRE_PXWaitXIDCmd		lapRE_MissStage
#define	lapRE_PXWaitRRCmd		lapRE_MissStage
#define	lapRE_PXWaitRNRCmd		lapRE_MissStage
#define	lapRE_PXWaitREJCmd		lapRE_MissStage
#define	lapRE_PXWaitSREJCmd		lapRE_MissStage
#define	lapRE_PXWaitIFrame		lapRE_MissStage
#define	lapRE_PXWaitXCHGCmd		lapRE_MissStage
#define	lapRE_PXWaitDXCHGCmd		lapRE_MissStage
#endif	
#if !defined(IrLAPNOSNIFF)
static	void	lapRE_NDMXIDRsp( const byte IrDAFAR *frame, uint framesz ) ;
static	void	lapRE_SCONNXIDRsp( const byte IrDAFAR *frame, uint framesz ) ;
#define			lapRE_SSETUPUARsp	lapRE_SETUPUARsp
#define			lapRE_SSETUPDMRsp	lapRE_SETUPDMRsp
#else	
#define			lapRE_NDMXIDRsp		lapRE_MissStage
#define			lapRE_SCONNXIDRsp	lapRE_MissStage
#define			lapRE_SSETUPUARsp	lapRE_MissStage
#define			lapRE_SSETUPDMRsp	lapRE_MissStage
#endif	


static	void		lapKickDiscovery( void ) ;
static	void		lapCallDiscoveryCnf( int status ) ;
#if !defined(IrLAPNONEWADDR)
static	IrDABOOL		lapIsAddrConflict( void ) ;
static	int			lapGetNoConflictLog( int base ) ;
static	void		lapRemoveConflictLog( void ) ;
static	dword		lapGetNewDiscAddr( void ) ;
static	int			lapGetNewAddrHistIdx( dword addr ) ;
static	void		lapTouchNewAddrLRU( dword addr ) ;
static	void		lapSwapDiscLog( int log1, int log2 ) ;
#endif	


static	void		lapGenerateConnectionAddress( void ) ;


static	byte		lapGetXIDSlotFlag( word c ) ;


static	IrDABOOL		lapChkPrimaryDiscReq( void ) ;
static	void		lapRE_PXMIT( void ) ;
#if !defined(IrLAPNORXCHG)
static	void		lapCallPrimaryCnf( int status ) ;
static	int			lapCallPrimaryInd( void ) ;
static	int		IrDACB	lapDmyPrimaryInd( void ) ;
static	void	IrDACB	lapDmyPrimaryCnf( int status ) ;
#endif	


static	void			lapSendXIDCmd( int slotno ) ;	
static	void			lapSendSNRMCmd( int ca ) ;										
static	void			lapSendRRCmd( void ) ;											
static	void	IrDACB	lapSendRNRCmd( void ) ;
static	void			lapSendDISCCmd( void ) ;										
#if !defined(IrLAPNORXCHG)
static	void			lapSendRXCHGRsp( void ) ;
static	void			lapSendDXCHGCmd( void ) ;
static	void			lapSendXCHGCmd( void ) ;
#endif	


#if !defined(IrLAPNOSNIFF)
static	void			lapStartListenTimer( void ) ;
static	void			lapStopListenTimer( void ) ;
static	void	IrDACB	lapListenTimerExpired( void ) ;
#endif	


static	void	IrDACB	lapXIDCmdCnf( void ) ;
static	void	IrDACB	lapStateToPRecv( void ) ;


static	void	IrDACB	lapCbStartSlotTimer( void ) ;
static	void	IrDACB	lapCbKickFTimer( void ) ;
static	void	IrDACB	lapCbStartFTimer( void ) ;
static	void	IrDACB	lapSlotTimerExpired( void ) ;
static	void	IrDACB	lapSendNextSlot( void ) ;
static	void	IrDACB	lapFTimerExpired( void ) ;
static	void	IrDACB	lapPTimerExpired( void ) ;
static	void			lapSendSCmd( void ) ;
static	void	IrDACB	lapCBSendPLeaveWindow( void ) ;


int		IrDAAPI	IrLAP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz ))
{
	if( maxslot == 0 || logsz == 0 )
		return( IrDAR_EPARM ) ;


#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_DiscoveryReq():" ) ;
	_DPUTB( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTW( DL_LAPINFO, " IrSIR_Busy():", IrSIR_Busy()) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


#if	!defined(IrLAPNOUI)
	if( lapFlags.sendingUi )
		return( IrDAR_BUSYWAIT ) ;
#endif	


	lapFlags.refuseDiscovery	= TRUE ;
	if( IrLAP_Stage == IrLAP_S_REPLY ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_BUSYWAIT ) ;
	}


	if( IrLAP_Stage > IrLAP_S_REPLY ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_CONNECTING ) ;
	}


	if( IrSIR_Busy()) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_MEDIABUSY ) ;
	}


	if( lapFlags.sendingUARsp ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_INVALID ) ;
	}
	IrLAP_Stage					= IrLAP_S_QUERY ;
	lapFlags.refuseDiscovery	= FALSE ;


#if !defined(IrLAPNONEWADDR)
	NewAddrRetryCount	= 0 ;
#endif	
	if( func )
		DiscoveryCnfPtr		= func ;
	else
		DiscoveryCnfPtr		= lapDmyDiscoveryCnf ;
	lapDiscLogPtr	= log ;
	lapDiscSlot		= 0 ;						
	lapDiscMaxSlot	= maxslot ;					


	lapSetupMyDeviceAddress( OFF ) ;


	DiscFlag			= 0 ;
	DiscTarget			= (dword)0xFFFFFFFFL ;		
	lapKickDiscovery() ;
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_ConnectReq( dword addr, void (IrDACB *func)( int status ))
{
#if	defined(DMSG33)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_ConnectReq():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( IrLAP_IsConnected )
		return( IrDAR_CONNECTING ) ;


	if( IrLAP_Stage != IrLAP_S_NDM )
		return( IrDAR_BUSYWAIT ) ;


#if	!defined(IrLAPNOUI)
	if( lapFlags.sendingUi )
		return( IrDAR_BUSYWAIT ) ;
#endif	
	if( lapFlags.sendingUARsp )
		return( IrDAR_BUSYWAIT ) ;


	lapStopFTimer() ;								
	ConnectCnfPtr			= func ;
	lapGenerateConnectionAddress() ;				
	TargetAddress			= addr ;				
	IrLAP_ConnRetryCount	= 0 ;					
	ConnRetryCountN3		= 3 ;
#if	defined(DMSG33)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_ConnectReq()" ) ;
	_DPUTW( DL_LAPINFO, " FTimerHndl:", FTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	IrLAP_Stage	= IrLAP_S_SETUP ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;			
	lapSendSNRMCmd( IrLAP_BroadCast ) ;				
	return( IrDAR_ACCEPT ) ;
}


#if !defined(IrLAPNOSNIFF)


int		IrDAAPI	IrLAP_SniffConnectReq( void (IrDACB *func)( int status ))
{
	lapFlags.refuseDiscovery	= TRUE ;
	if( IrLAP_IsConnected ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_CONNECTING ) ;
	}


	if( IrLAP_Stage != IrLAP_S_NDM ) {
		lapFlags.refuseDiscovery	= FALSE ;
		return( IrDAR_BUSYWAIT ) ;
	}


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::IrLAP_SniffConnectReq():" ) ;
	_DPUTS( DL_LAPINFO, " accept" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	ConnectCnfPtr				= func ;
	IrLAP_Stage					= IrLAP_S_SCONN ;
	lapFlags.refuseDiscovery	= FALSE ;


	
	
	
	


	lapStartListenTimer() ;
	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrLAPNORXCHG)


int		IrDAAPI	IrLAP_SetPrimaryInd( int (IrDACB *indp)( void ))
{
	if( indp )
		PrimaryIndPtr	= indp ;
	else
		PrimaryIndPtr	= lapDmyPrimaryInd ;


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrLAP_PrimaryReq( void (IrDACB *cnfp)( int status ))
{
	if( !IrLAP_IsConnected )			return( IrDAR_NOCONNECT ) ;
	if( IrLAP_IsConnected1 )			return( IrDAR_PRIMARY ) ;
	if( IrLAP_Stage != IrLAP_S_SRecv )	return( IrDAR_BUSYWAIT ) ;


	if( cnfp )
		PrimaryCnfPtr	= cnfp ;
	else
		PrimaryCnfPtr	= lapDmyPrimaryCnf ;
	lapFlags.primaryReq	= TRUE ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapGenerateConnectionAddress( void )
{
	IrLAP_ConnectionAddress		= (uint)MiscRandom8( 0x7D ) + 1 ;
	IrSIR_SetConnectionAddress( IrLAP_ConnectionAddress ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	byte		lapGetXIDSlotFlag( word c )
{
	if( c >= 16 )	return( 0x03 ) ;
	if( c >= 8 )	return( 0x02 ) ;
	if( c >= 6 )	return( 0x01 ) ;
	return( 0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapKickDiscovery( void )
{
	lapDiscSlotCount	= 0 ;					
	lapStopSlotTimer() ;						
	IrSIR_SetDataCnf( lapCbStartSlotTimer ) ;
	lapSendXIDCmd( lapDiscSlotCount ) ;			
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	IrDABOOL		lapIsAddrConflict( void )
{
	int		i, j ;
	dword		chkaddr ;


#if defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapIsAddrConflict():" ) ;
	_DPUTW( DL_LAPINFO, " lapDiscSlot:", lapDiscSlot ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	for( i=0 ; i<(int)lapDiscSlot - 1 ; i++ ) {
		chkaddr		= lapDiscLogPtr[i].Address ;
		for( j=i+1 ; j<(int)lapDiscSlot ; j++ ) {
			if( lapDiscLogPtr[j].Address == chkaddr ) {
#if defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapIsAddrConflict():" ) ;
	_DPUTD( DL_LAPINFO, " conflict!! ", chkaddr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
				return( TRUE ) ;
			}
		}
	}


#if defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapIsAddrConflict():" ) ;
	_DPUTS( DL_LAPINFO, " no conflict" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	return( FALSE ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	void		lapRemoveConflictLog( void )
{
	int			i, j ;


	lapDiscConflictSlot	= lapDiscSlot ;


	for( i=0 ; i<(int)lapDiscSlot ; i++ ) {
		if(( j = lapGetNoConflictLog( i )) == -1 ) {
			lapDiscConflictSlot	= lapDiscSlot ;
			lapDiscSlot			= i ;
			break ;
		}


		lapSwapDiscLog( i, j ) ;
	}


	DiscTarget	= lapGetNewDiscAddr() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	int			lapGetNoConflictLog( int base )
{
	int		i, j ;
	dword	chkaddr ;
	IrDABOOL	conflict ;


	for( i=base ; i<(int)lapDiscSlot - 1; i++ ) {
		chkaddr		= lapDiscLogPtr[i].Address ;
		conflict	= FALSE ;
		for( j=i+1 ; j<(int)lapDiscSlot ; j++ ) {
			if( lapDiscLogPtr[j].Address == chkaddr )
				conflict	= TRUE ;
		}
		if( !conflict )
			return( i ) ;
	}


	return( -1 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	dword		lapGetNewDiscAddr( void )
{
	int		i ;
	int		tmp, histtop ;
	dword	hit ;


	histtop		= AUTONEWADDRHIST ;
	hit			= (dword)0xFFFFFFFFL ;
	for( i=lapDiscSlot ; i<(int)lapDiscConflictSlot ; i++ ) {
		tmp		= lapGetNewAddrHistIdx( lapDiscLogPtr[i].Address ) ;
		if( tmp < histtop ) {
			histtop	= tmp ;
			hit		= lapDiscLogPtr[i].Address ;
		}
	}


	lapTouchNewAddrLRU( hit ) ;
	return( hit ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	int			lapGetNewAddrHistIdx( dword addr )
{
	int		i ;


	for( i=0 ; i<AUTONEWADDRHIST ; i++ )
		if( NewAddrHist[i] == addr )
			return( i ) ;


	return( -1 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	void		lapTouchNewAddrLRU( dword addr )
{
	int		i ;
	int		start ;


	if(( start = lapGetNewAddrHistIdx( addr )) == -1 )
		start	= AUTONEWADDRHIST - 1 ;


	for( i=start ; i>0 ; i-- )
		NewAddrHist[i]	= NewAddrHist[i-1] ;
	NewAddrHist[0]	= addr ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNONEWADDR)


static	void		lapSwapDiscLog( int log1, int log2 )
{
	DISCOVERYLOG	tmplog ;


	if( log1 == log2 )
		return ;


	memcpy( &tmplog, &lapDiscLogPtr[log1], sizeof( DISCOVERYLOG )) ;
	memcpy( &lapDiscLogPtr[log1], &lapDiscLogPtr[log2], sizeof( DISCOVERYLOG )) ;
	memcpy( &lapDiscLogPtr[log2], &tmplog, sizeof( DISCOVERYLOG )) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapCallDiscoveryCnf( int status )
{
	void	(IrDACB *func)( int status, uint logsz ) ;


	func				= DiscoveryCnfPtr ;
	DiscoveryCnfPtr		= 0 ;
	(*func)( status, lapDiscSlot ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_QUERYXIDRsp( const byte IrDAFAR *frame, uint framesz )
{
	lapDiscLogPtr[lapDiscSlot].Address		= SrcAddress ;
	lapDiscLogPtr[lapDiscSlot].DeviceNameSize	= framesz - XID_Info ;
	IrDA_memcpy( lapDiscLogPtr[lapDiscSlot].DeviceName, &frame[XID_Info], framesz - XID_Info ) ;
	lapDiscSlot	++ ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_QUERYXIDRsp():" ) ;
	_DPUTW( DL_LAPINFO, " lapDiscSlot:", lapDiscSlot ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapRE_NDMXIDRsp( const byte IrDAFAR *frame, uint framesz )
{
	if( DesAddress != 0xFFFFFFFFL )				
		return ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_NDMXIDRsp():" ) ;
	_DPUTS( DL_LAPINFO, " received Sniff-XID-Rsp" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapGetDiscoveryInfo( SniffDiscLog, frame, framesz ) ;
	(*SniffIndPtr)() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	lapRE_SCONNXIDRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	if( DesAddress != 0xFFFFFFFFL )				
		return ;
	lapStopListenTimer() ;


	lapGenerateConnectionAddress() ;				
	TargetAddress		= DesAddress ;			


	
	


	IrLAP_Stage			= IrLAP_S_SSETUP ;
	IrSIR_SetDataCnf( lapCbStartFTimer ) ;			
	lapSendSNRMCmd( IrLAP_BroadCast ) ;			
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_SETUPUARsp( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;									
	SetTimer( &MTimer, IrLAP_MORATORIUMTIME ) ;
													
													
	lapGetCapacityParam( &TargetPortConfig, &frame[UA_NegotiationParm], framesz - UA_NegotiationParm ) ;
	lapNegotiateConnectionParameters( &TargetPortConfig, &MyPortConfig ) ;
	lapApplyConnectionParameters() ;				
	lapCbInitConnState() ;
	IrSIR_SetDataCnf( lapStateToPRecv ) ;				
	lapSendRRCmd() ;									
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SETUPUARsp()" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_SETUPDMRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;
	lapStateToNDM( IrDAR_DISCONNECT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	IrDABOOL	lapChkPrimaryDiscReq( void )
{
	if( !lapFlags.disconnectReq )
		return( FALSE ) ;


	lapStopFTimer() ;									
	lapFlags.disconnectReq	= FALSE ;
	lapReleaseBufferedData() ;							
	IrLAP_ConnRetryCount	= 0 ;					
	IrLAP_Stage				= IrLAP_S_PClose ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;
	lapSendDISCCmd() ;									
	return( TRUE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PXMIT( void )
{
#if	defined(DMSG4)
		_DPUTS( DL_LAPINFO, " LAP::lapRE_PXMIT():\n" ) ;
#endif	
	if( lapChkPrimaryDiscReq())
		return ;


	if( lapChkDataRequest()) {
#if	defined(DMSG)
		_DPUTS( DL_LAPINFO, " LAP::lapRE_PXMIT(): send data\n" ) ;
#endif	
		lapStopFTimer() ;							
		SetTimer( &MTimer, IrLAP_MORATORIUMTIME ) ;
		IrLAP_ConnRetryCount	= 0 ;				
#if	defined(NOTXMULTIWINDOW)
		LAP_TxLeaveWindow		= 1 ;
#else	
		LAP_TxLeaveWindow		= ConnSendWindowSize ;
#endif	
		lapCBSendPLeaveWindow() ;
		return ;
	}


#if	defined(DMSG4)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PXMIT():" ) ;
	_DPUTD( DL_LAPINFO, " MTimer:", GetTimer( MTimer )) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( GetTimer( MTimer ) != 0L ) {
		lapSendSCmd() ;
		return ;
	}


	lapStopPTimer() ;									
	PTimerHndl	= StartTimer( IrLAP_ConnMaxTAT, lapPTimerExpired ) ;
#if	defined(DMSG4)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PXMIT():" ) ;
	_DPUTW( DL_LAPINFO, "PTimerHndl:", PTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)
# if !defined(IrLAPFRMRDISC)


static	void	lapRE_PRecvFRMRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;
	lapSendSCmd() ;
}
# endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PRecvRDRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;
	lapReleaseBufferedData() ;							
	IrLAP_ConnRetryCount	= 0 ;					
	IrLAP_Stage				= IrLAP_S_PClose ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;
	lapSendDISCCmd() ;									
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_PRecvRRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;									
	IrLAP_ConnRetryCount	= 0 ;					
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PRecvRRRsp():" ) ;
	_DPUTB( DL_LAPINFO, " Vs:", IrLAP_Vs ) ;
	_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	IrLAP_ConnRemoteBusy	= FALSE ;				
	lapUpdateNrReceived() ;							
	if( !lapChkNr()) {								
		IrSIR_SetDataCnf( lapCbKickFTimer ) ;
		lapResendIFrame() ;							
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PRecvRRRsp()" ) ;
	_DPUTW( DL_LAPINFO, " FTimerHndl:", FTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		return ;
	}


	lapRE_PXMIT() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_PRecvRNRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopFTimer() ;
	IrLAP_ConnRetryCount	= 0 ;					
	IrLAP_ConnRemoteBusy	= TRUE ;				
	lapUpdateNrReceived() ;							


	if( lapChkPrimaryDiscReq())
		return ;


	IrSIR_SetDataCnf( lapCbKickFTimer ) ;
	lapSendRRCmd() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapRE_PRecvIRsp( const byte IrDAFAR *frame, uint framesz )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PRecvIRsp():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTB( DL_LAPINFO, " Ns:", IrLAP_Ns ) ;
	_DPUTB( DL_LAPINFO, " Vs:", IrLAP_Vs ) ;
	_DPUTB( DL_LAPINFO, " Nr:", IrLAP_Nr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


	SetTimer( &MTimer, IrLAP_MORATORIUMTIME ) ;


	if( lapChkNs() && lapChkNr()) {
		if( frame[1] & IrLAP_PF ) {
			lapStopFTimer() ;							
			IrLAP_ConnRetryCount	= 0 ;			
		}
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
		lapUpdateNrReceived() ;						
		lapCallDataInd( frame, framesz ) ;				
		if( frame[1] & IrLAP_PF )
			lapRE_PXMIT() ;
		return ;
	}


	if( lapChkNs() && !lapChkNr()) {
		if( frame[1] & IrLAP_PF ) {
			lapStopFTimer() ;							
			IrLAP_ConnRetryCount	= 0 ;			
		}
		IrLAP_Vr	= ( IrLAP_Vr + 1 ) & 7 ;		
		lapUpdateNrReceived() ;						
		lapCallDataInd( frame, framesz ) ;				
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbKickFTimer ) ;
			lapResendIFrame() ;						
		}
		return ;
	}


	if( !lapChkNs() && lapChkNr()) {
		if( frame[1] & IrLAP_PF ) {
			lapStopFTimer() ;							
			IrLAP_ConnRetryCount	= 0 ;			
		}
		lapUpdateNrReceived() ;						
		if( frame[1] & IrLAP_PF ) {
			IrSIR_SetDataCnf( lapCbKickFTimer ) ;		
			lapSendRRCmd() ;							
		}
		return ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PBusyWaitRRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopFTimer() ;								
	lapUpdateNrReceived() ;							
	IrLAP_ConnRemoteBusy	= FALSE ;				
	if( !lapChkNr()) {								
		IrSIR_SetDataCnf( lapCbKickFTimer ) ;
		lapResendIFrame() ;							
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PRecvRRRsp()" ) ;
	_DPUTW( DL_LAPINFO, " FTimerHndl:", FTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		return ;
	}
	lapRE_PXMIT() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PBusyWaitRNRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopFTimer() ;									
	lapUpdateNrReceived() ;							
	IrLAP_ConnRemoteBusy	= TRUE ;				
	lapRE_PXMIT() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PBusyWaitIRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopFTimer() ;								
	lapUpdateNrReceived() ;							
	if( !lapChkNr()) {								
		IrSIR_SetDataCnf( lapCbKickFTimer ) ;
		lapResendIFrame() ;							
		return ;
	}
	lapRE_PXMIT() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lapRE_PCloseDMRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_PCloseDMRsp()" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapStateToNDM( IrDAR_DISCONNECT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	IrDABOOL		lapChkPrimaryReq( void )
{
	if( !lapFlags.primaryReq )
		return( FALSE ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapChkPrimaryReq():" ) ;
	_DPUTS( DL_LAPINFO, " change to primary" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapFlags.primaryReq	= FALSE ;
	rxchgRetryCount		= 0 ;
	IrLAP_Stage			= IrLAP_S_SRXWait ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
	lapSendRXCHGRsp() ;
	return( TRUE ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void		lapCallPrimaryCnf( int status )
{
	(*PrimaryCnfPtr)( status ) ;
	if( status == IrDAR_PRIMARY )
		(*ConnectPrvPtr2)( 1 ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	int			lapCallPrimaryInd( void )
{
	return((*PrimaryIndPtr)()) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	int		IrDACB	lapDmyPrimaryInd( void )
{
	return( IrDA_DENY ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	IrDACB	lapDmyPrimaryCnf( int status )
{
	UNREFERENCE( status ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendXIDCmd( int slotno )
{
	uint		len ;


#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapSendXIDCmd():" ) ;
	_DPUTB( DL_LAPINFO, " slot:", slotno ) ;
	_DPUTD( DL_LAPINFO, " target:", DiscTarget ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]		= (( IrLAP_BroadCast & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]		= IrLAP_XIDCmd | IrLAP_PF ;
	LAP_SendCmdBuffer[XID_FI]	= 0x01 ;
	lapSetAddress( &LAP_SendCmdBuffer[XID_SrcAddress], MyAddress ) ;
	lapSetAddress( &LAP_SendCmdBuffer[XID_DesAddress], DiscTarget ) ;
	LAP_SendCmdBuffer[XID_DiscoveryFlag]	= lapGetXIDSlotFlag( (byte)lapDiscMaxSlot ) | DiscFlag ;
	LAP_SendCmdBuffer[XID_SlotNo]			= (byte)slotno ;
	LAP_SendCmdBuffer[XID_Version]			= 0 ;


	len							= XID_Info ;
	if( slotno == 0xFF ) {
		IrDA_memcpy( &LAP_SendCmdBuffer[XID_Info], LAP_DiscoveryName, LAP_DiscoveryNameSize ) ;
		len	+= LAP_DiscoveryNameSize ;
	}


	lapResetRxBuffer() ;
	IrSIR_SendFrame( len, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lapSendSNRMCmd( int ca )
{
	uint		len ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendSNRMCmd():" ) ;
	_DPUTB( DL_LAPINFO, " ca:", ca ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= (( ca & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_SNRMCmd | IrLAP_PF ;
	lapSetAddress( &LAP_SendCmdBuffer[SNRM_SrcAddress], MyAddress ) ;
	lapSetAddress( &LAP_SendCmdBuffer[SNRM_DesAddress], TargetAddress ) ;
	LAP_SendCmdBuffer[SNRM_ConnectAddress]	= IrLAP_ConnectionAddress << 1 ;
#if defined(IrDAFORTRANXIT01)
	IrDA_memset( &TargetPortConfig, 0xFF, sizeof( PORTCONFIG )) ;
#endif	
	len		= SNRM_NegotiationParm + lapSetCapacityParam( &LAP_SendCmdBuffer[SNRM_NegotiationParm], &MyPortConfig ) ;


	lapResetRxBuffer() ;
	IrSIR_SendFrame( len, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			lapSendRRCmd( void )
{
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapSendRRCmd():" ) ;
	_DPUTB( DL_LAPINFO, " Vr:", IrLAP_Vr ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= (( IrLAP_ConnectionAddress & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_RRCmd | IrLAP_PF | ( IrLAP_Vr << 5 ) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapSendRNRCmd( void )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapSendRNRCmd():" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;
	LAP_SendCmdBuffer[0]	= (( IrLAP_ConnectionAddress & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_RNRCmd | IrLAP_PF | ( IrLAP_Vr << 5 ) ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			lapSendDISCCmd( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendDISCCmd():" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= ( IrLAP_ConnectionAddress << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_DISCCmd | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SRXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRXWaitXCHGCmd(): change state to SXWait" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	lapStopWDTimer() ;
	IrLAP_Stage		= IrLAP_S_SXWait ;
	lapCallPrimaryCnf( IrDAR_PRIMARY ) ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;				
	lapSendRRCmd() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SRXWaitDXCHGCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopWDTimer() ;
	lapStateToSRecv() ;
	lapCallPrimaryCnf( IrDAR_DENY ) ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;		
	lapSendRRRsp() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SRXWaitXXXCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopWDTimer() ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRXWaitXXXCmd():" ) ;
	_DPUTW( DL_LAPINFO, " rxchgRetryCount:", rxchgRetryCount ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	if( rxchgRetryCount < rxchgRetryCountN4 ) {
		rxchgRetryCount	++ ;
		IrSIR_SetDataCnf( lapCbStartWDTimer ) ;
		lapSendRXCHGRsp() ;
		return ;
	}


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapRE_SRXWaitXXXCmd():" ) ;
	_DPUTS( DL_LAPINFO, " retry over" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	rxchgRetryCount	= 0 ;
	lapStateToSRecv() ;
	lapCallPrimaryCnf( IrDAR_DENY ) ;
	IrSIR_SetDataCnf( lapCbStartWDTimer ) ;		
	lapSendRRRsp() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;
	rxchgRetryCount	= 0 ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;				
	lapSendRRCmd() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SXWaitRRRsp( const byte IrDAFAR *frame, uint framesz )
{
	IrLAP_Stage		= IrLAP_S_PRecv ;
	lapRE_PRecvRRRsp( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SXWaitRNRRsp( const byte IrDAFAR *frame, uint framesz )
{
	IrLAP_Stage		= IrLAP_S_PRecv ;
	lapRE_PRecvRNRRsp( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SXWaitIRsp( const byte IrDAFAR *frame, uint framesz )
{
	IrLAP_Stage		= IrLAP_S_PRecv ;
	lapRE_PRecvIRsp( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_SXWaitRXCHGRsp( const byte IrDAFAR *frame, uint framesz )
{
	IrLAP_Stage		= IrLAP_S_PRecv ;
	lapRE_PRecvRXCHGRsp( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitDISCCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapRE_SRecvDISCCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitFRMRRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;
	IrLAP_Stage		= IrLAP_S_PRecv ;
	lapSendSCmd() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitRRCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvRRCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitXXXRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	if(( frame[1] & IrLAP_PF ) == 0 )
		return ;


	lapStopFTimer() ;
	
	rxchgRetryCount	= 0 ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;				
	lapSendXCHGCmd() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitSNRMCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvSNRMCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitTESTCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvTESTCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitUICmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvUICmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitXIDCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_MissStage( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitRNRCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvRNRCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitREJCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvREJCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitSREJCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvSREJCmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitIFrame( const byte IrDAFAR *frame, uint framesz )
{
	
	if(( frame[0] & IrLAP_CR ) == 0 ) {
		lapRE_PXWaitXXXRsp( frame, framesz ) ;
		return ;
	}


	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_SRecvICmd( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitXCHGCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_MissStage( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PXWaitDXCHGCmd( const byte IrDAFAR *frame, uint framesz )
{
	lapStopFTimer() ;
	lapStateToSRecv() ;
	lapRE_MissStage( frame, framesz ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void	lapRE_PRecvRXCHGRsp( const byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	lapStopFTimer() ;									
	rxchgRetryCount	= 0 ;
	IrSIR_SetDataCnf( lapCbKickFTimer ) ;				
	if( lapCallPrimaryInd()) {
		IrLAP_Stage		= IrLAP_S_PRecv ; ;
		lapSendDXCHGCmd() ;
		return ;
	}


	IrLAP_Stage		= IrLAP_S_PXWait ;
	lapSendXCHGCmd() ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void		lapSendRXCHGRsp( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapSendRXCHGRsp():\n" ) ;
#endif	
	LAP_SendCmdBuffer[0]	= (( IrLAP_ConnectionAddress & 0x7F ) << 1 ) ;
	LAP_SendCmdBuffer[1]	= IrLAP_RXCHGRsp | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void			lapSendDXCHGCmd( void )
{
	LAP_SendCmdBuffer[0]	= (( IrLAP_ConnectionAddress & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_DXCHGCmd | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	void			lapSendXCHGCmd( void )
{
	LAP_SendCmdBuffer[0]	= (( IrLAP_ConnectionAddress & 0x7F ) << 1 ) | IrLAP_CR ;
	LAP_SendCmdBuffer[1]	= IrLAP_XCHGCmd | IrLAP_PF ;
	lapResetRxBuffer() ;
	IrSIR_SendFrame( 2, LAP_SendCmdBuffer ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbStartSlotTimer( void )
{
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, "CNF<<<\n" ) ;
#endif	
	SlotTimerHndl	= StartTimer( IrLAPSLOTTIME, lapSlotTimerExpired ) ;
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapCbStartSlotTimer()" ) ;
	_DPUTW( DL_LAPINFO, " SlotTimerHndl:", SlotTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTS( DL_LAPINFO, "CNF>>>\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbKickFTimer( void )
{
#if defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapCbKickFTimer():\n" ) ;
#endif	
	SetTimer( &DiscTimer, ConnDisconnectMSec ) ;
	lapCbStartFTimer() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCbStartFTimer( void )
{
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, "CNF<<<\n" ) ;
#endif	
	lapStopFTimer() ;
													
	InFrameRetryCounter	= 0 ;
	FTimerHndl	= StartTimer( IrLAP_ConnMaxTAT, lapFTimerExpired ) ;
	lapStartLTimer() ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapCbStartFTimer()" ) ;
	_DPUTW( DL_LAPINFO, " FTimerHndl:", FTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
	_DPUTS( DL_LAPINFO, "CNF>>>\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB		lapSlotTimerExpired( void )
{
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapSlotTimerExpired()" ) ;
	_DPUTD( DL_LAPINFO, " now:", IrSIR_ReadTimer()) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	SlotTimerHndl	= -1 ;


	if( IrSIR_InFrame()) {
		SlotTimerHndl	= StartTimer( IrLAPSLOTTIME, lapSendNextSlot ) ;
	} else {
		lapSendNextSlot() ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	IrDACB		lapSendNextSlot( void )
{
	if( lapDiscSlotCount < lapDiscMaxSlot - 1 ) {
		lapDiscSlotCount	++ ;					
		IrSIR_SetDataCnf( lapCbStartSlotTimer ) ;
		lapSendXIDCmd( lapDiscSlotCount ) ;			
		return ;
	}


	IrSIR_SetDataCnf( lapXIDCmdCnf ) ;
	lapSendXIDCmd( 0xFF ) ;							
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapStartListenTimer( void )
{
	lapStopListenTimer() ;
	ListenTimerHndl	= StartTimer( TIME_WAITSNIFFXIDRSP, lapListenTimerExpired ) ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapStartListenTimer():" ) ;
	_DPUTW( DL_LAPINFO, " hndl:", ListenTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void			lapStopListenTimer( void )
{
	if( ListenTimerHndl < 0 )
		return ;
#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapStopListenTimer(): stop\n" ) ;
#endif	
	StopTimer( ListenTimerHndl ) ;
	ListenTimerHndl	= -1 ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNOSNIFF)


static	void	IrDACB	lapListenTimerExpired( void )
{
	ListenTimerHndl	= -1 ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapListenTimerExpired():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	switch( IrLAP_Stage ) {
	case	IrLAP_S_SCONN :
		lapStateToNDM( IrDAR_DISCONNECT ) ;
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapXIDCmdCnf( void )
{
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, "CNF<<<\n" ) ;
	_DPUTS( DL_LAPINFO, " LAP::lapXIDCmdCnf()" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	


#if !defined(IrLAPNONEWADDR)
	if( lapIsAddrConflict()) {
		lapRemoveConflictLog() ;
		NewAddrRetryCount	++ ;
		if( NewAddrRetryCount <= IrLAPNEWADDRRETRYCOUNT ) {
			DiscFlag	= XID_AddressConfilict ;
			lapKickDiscovery() ;
#if	defined(DMSG32)
			_DPUTS( DL_LAPINFO, "CNF>>>\n" ) ;
#endif	
			return ;
		}
	}
#endif	


	IrLAP_Stage	= IrLAP_S_NDM ;
	lapCallDiscoveryCnf( IrDAR_DONE ) ;			
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, "CNF>>>\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapStateToPRecv( void )
{
	lapCbKickFTimer() ;
	IrLAP_Stage	= IrLAP_S_PRecv ;


	(*ConnectPrvPtr)() ;
	(*ConnectPrvPtr2)( 1 ) ;
	(*ConnectCnfPtr)( IrDAR_DONE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void 	IrDACB	lapFTimerExpired( void )
{
	FTimerHndl	= -1 ;


#if	defined(DMSG3)
	_DPUTS( DL_LAPINFO, " LAP::lapFTimerExpired()" ) ;
	_DPUTW( DL_LAPINFO, " Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	switch( IrLAP_Stage ) {
	case	IrLAP_S_SETUP :
		if( IrLAP_ConnRetryCount < ConnRetryCountN3 ) {
			IrLAP_ConnRetryCount	++ ;			
													
			IrSIR_SetDataCnf( lapCbKickFTimer ) ;		
			lapSendSNRMCmd( IrLAP_BroadCast ) ;		
#if	defined(DMSG32)
			_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
			return ;
		}
		lapStateToNDM( IrDAR_DISCONNECT ) ;			
#if	defined(DMSG32)
		_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
		return ;
	case	IrLAP_S_PRecv :
	case	IrLAP_S_PBusyWait :
		if( IrSIR_InFrame()) {
#if	defined(DMSG3)
			_DPUTS( DL_LAPINFO, " LAP::lapFTimerExpired()" ) ;
			_DPUTS( DL_LAPINFO, " IrSIR_InFrame() is TRUE" ) ;
			_DPUTW( DL_LAPINFO, " InFrameRetryCounter:", InFrameRetryCounter ) ;
			_DPUTW( DL_LAPINFO, " MaxInFrameRetryCount:", MaxInFrameRetryCount ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			InFrameRetryCounter	++ ;
			FTimerHndl	= StartTimer( IrLAP_ConnMaxTAT, lapFTimerExpired ) ;
			if( InFrameRetryCounter <= MaxInFrameRetryCount ) {
#if	defined(DMSG32)
				_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
				return ;
			}
		}


#if defined(IrLAPNOUSEDISCTIMER)
		if( IrLAP_ConnRetryCount < ConnRetryCountN2 ) {
#else	
		if( IrLAP_ConnRetryCount < ConnRetryCountN2 && GetTimer( DiscTimer ) != (dword)0L ) {
#endif	
			IrLAP_ConnRetryCount	++ ;			
			IrSIR_SetDataCnf( lapCbStartFTimer ) ;		
#if	defined(DMSG3)
			_DPUTS( DL_LAPINFO, " LAP::lapFTimerExpired(): retry again" ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			if( IrLAP_Stage == IrLAP_S_PRecv )
				lapSendRRCmd() ;						
			else
				lapSendRNRCmd() ;
#if	defined(DMSG32)
			_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
			return ;
		}
#if	defined(DMSG32)
		_DPUTS( DL_LAPINFO, " LAP::lapFTimerExpired()" ) ;
		_DPUTS( DL_LAPINFO, " lapApplyDefaultConnParam()" ) ;
		_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
		lapStateToNDM( IrDAR_NORESPONSE ) ;
#if	defined(DMSG32)
		_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
		return ;
	case	IrLAP_S_PClose :
		if( IrLAP_ConnRetryCount < ConnRetryCountN3 ) {
			IrLAP_ConnRetryCount	++ ;			
			IrSIR_SetDataCnf( lapCbKickFTimer ) ;		
#if	defined(DMSG32)
	_DPUTS( DL_LAPINFO, " LAP::lapFTimerExpired()" ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			lapSendDISCCmd() ;							
#if	defined(DMSG32)
			_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
			return ;
		}


		lapStateToNDM( IrDAR_DISCONNECT ) ;
#if	defined(DMSG32)
		_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
		return ;
#if !defined(IrLAPNORXCHG)
	case	IrLAP_S_SXWait :
		if( rxchgRetryCount < ConnRetryCountN2 ) {
			rxchgRetryCount	++ ;					
			IrSIR_SetDataCnf( lapCbStartFTimer ) ;		
			lapSendRRCmd() ;
			return ;
		}
		lapStateToNDM( IrDAR_DISCONNECT ) ;
		return ;
	case	IrLAP_S_PXWait :
		if( rxchgRetryCount < ConnRetryCountN2 ) {
			rxchgRetryCount	++ ;
			IrSIR_SetDataCnf( lapCbStartFTimer ) ;
			lapSendXCHGCmd() ;
			return ;
		}
		lapStateToNDM( IrDAR_DISCONNECT ) ;
		return ;
#endif	
#if !defined(IrLAPNOSNIFF)
	case	IrLAP_S_SCONN :
	case	IrLAP_S_SSETUP :
		lapStateToNDM( IrDAR_DISCONNECT ) ;
		return ;
#endif	
	}
#if	defined(DMSG32)
		_DPUTS( DL_LAPINFO, "TMR>>>\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void 	IrDACB	lapPTimerExpired( void )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapPTimerExpired():" ) ;
	_DPUTW( DL_LAPINFO, " PTimerHndl:", PTimerHndl ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	
	if( PTimerHndl == -1 )
		return ;


	PTimerHndl	= -1 ;
	switch( IrLAP_Stage ) {
	case	IrLAP_S_PRecv :
	case	IrLAP_S_PBusyWait :
		if( lapFlags.inDataReq ) {
			
			PTimerHndl	= StartTimer( IrLAP_ConnMaxTAT, lapPTimerExpired ) ;
#if	defined(DMSG)
			_DPUTS( DL_LAPINFO, " LAP::lapPTimerExpired()" ) ;
			_DPUTW( DL_LAPINFO, " PTimerHndl:", PTimerHndl ) ;
			_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
			return ;
		}
		lapSendSCmd() ;
		break ;
	}
}


static	void			lapSendSCmd( void )
{
#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapSendSCmd():" ) ;
	_DPUTW( DL_LAPINFO, " IrLAP_Stage:", IrLAP_Stage ) ;
	_DPUTS( DL_LAPINFO, "\n" ) ;
#endif	
	switch( IrLAP_Stage ) {
	case	IrLAP_S_PRecv :
		lapStopFTimer() ;								
		IrSIR_SetDataCnf( lapCbKickFTimer ) ;			
		lapSendRRCmd() ;								
		break ;
	case	IrLAP_S_PBusyWait :
		lapStopFTimer() ;								
		IrSIR_SetDataCnf( lapCbKickFTimer ) ;			
		lapSendRNRCmd() ;								
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lapCBSendPLeaveWindow( void )
{
	uint	nextSendPtr ;


#if	defined(DMSG)
	_DPUTS( DL_LAPINFO, " LAP::lapCBSendPLeaveWindow(): send data\n" ) ;
#endif	
	LAP_TxLeaveWindow	-- ;
	nextSendPtr		= lapNextSendWindowPtr( LAP_SendWinSendPtr ) ;
	if( LAP_SendCtl[nextSendPtr].size == 0 ||
		LAP_TxLeaveWindow == 0 ) {
		switch( IrLAP_Stage ) {
		case	IrLAP_S_PRecv :
		default :
			IrSIR_SetDataCnf( lapCbKickFTimer ) ;
			lapSendIFrame( IrLAP_PF, IrLAP_CR ) ;
			break ;
		case	IrLAP_S_PBusyWait :
			IrSIR_SetDataCnf( lapSendRNRCmd ) ;
			lapSendIFrame( 0, IrLAP_CR ) ;
			break ;
		}
		return ;
	}


	IrSIR_SetDataCnf( lapCBSendPLeaveWindow ) ;
	lapSendIFrame( 0, IrLAP_CR ) ;
}
#endif	


#endif	
