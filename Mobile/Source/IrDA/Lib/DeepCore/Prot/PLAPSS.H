


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
