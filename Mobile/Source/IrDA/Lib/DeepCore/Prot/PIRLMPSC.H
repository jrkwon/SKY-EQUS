


#if !defined(__pirlmpsc_h__)
#define	__pirlmpsc_h__


#define	SC_Ready			0
#define	SC_Discover			1
#define	SC_ResolveAddr		2
#define	SC_ExclusivePend	3
#define	SC_Exclusive		4
#define	SC_ReadyPend		5
#define	SC_RoleExchange		6
#define	SC_Sniff			7


static	int			SCStatus ;				
static	int			SCConnectDevice ;		


#if defined(IrDAPRIMARY)
static	void	(IrDACB *SCDiscoveryCnfPtr)( int status, uint logsz ) ;
#endif	


static	void		lmpSC_Initialize( void ) ;
#if defined(IrDAPRIMARY)
static	int			lmpSC_DiscoverReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;
static	int			lmpSC_ConnectReq( dword addr ) ;
static	void	IrDACB	lmpCbSC_DiscoveryCnf( int status, uint logsz ) ;
static	void	IrDACB	lmpCbSC_ConnectCnf( int status ) ;
#endif	
static	void		lmpSC_DisconnectReq( DEVICECONFIG *dc, int reason ) ;
static	int		IrDACB	lmpCbSC_ConnectInd( dword addr ) ;
static	void	IrDACB	lmpCbSC_DisconnectInd( int status ) ;
#if !defined(IrLAPNORESET)
static	IrDABOOL	IrDACB	lmpCbSC_ResetInd( void ) ;
#endif	


#if defined(IrDAPRIMARY)
static	void			lmpCallDiscoveryCnf( int status, int logsz ) ;
#endif	


#if !defined(IrLAPNORXCHG)
static	int		lmpSC_SetPrimaryInd( int (IrDACB *indp)( void )) ;
static	int		lmpSC_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
#endif	


#if !defined(IrLAPNOSNIFF)
static	DISCOVERYLOG IrDAFAR	*SniffLogPtr ;		
static	int		(IrDACB *SniffIndPtr)( void ) ;
static	DISCOVERYLOG 			SniffLog ;			
static	int				lmpSC_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void )) ;
static	void	IrDACB	lmpSC_SniffInd( void ) ;
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpSC_Initialize( void )
{
	SCStatus			= SC_Ready ;
	SCConnectDevice		= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	int			lmpSC_DiscoverReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz ))
{
	int		ret ;


	switch( SCStatus ) {
	case	SC_Ready :
		if( SCConnectDevice )
			return( IrDAR_CONNECTING ) ;
		SCDiscoveryCnfPtr	= func ;
		ret		= IrLAP_DiscoveryReq( maxslot, log, logsz, lmpCbSC_DiscoveryCnf ) ;
		if( ret == IrDAR_ACCEPT )
			SCStatus			= SC_Discover ;
		return( ret ) ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
	case	SC_Sniff :
		break ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	IrDACB	lmpCbSC_DiscoveryCnf( int status, uint logsz )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_DiscoveryCnf():" ) ;
	_DPUTW( DL_LMPINFO, " status:", status ) ;
	_DPUTW( DL_LMPINFO, " logsz:", logsz ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Discover :
		SCStatus	= SC_Ready ;
		lmpCallDiscoveryCnf( status, logsz ) ;
		return ;
	case	SC_Ready :
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
	case	SC_Sniff :
		break ;
	}


	
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	int			lmpSC_ConnectReq( dword addr )
{
	DEVICECONFIG	*dc ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpSC_ConnectReq():" ) ;
	_DPUTD( DL_LMPINFO, " addr:", addr ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
		if(( dc = lmpMakeDeviceConfig( addr )) == (DEVICECONFIG*)0 )
			return( IrDAR_NORESOURCE ) ;
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpSC_ConnectReq():" ) ;
		_DPUTW( DL_LMPINFO, " dc->Mode=", dc->Mode ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		if( dc->Mode & DCM_Connected )
			return( IrDAR_CONNECTING ) ;
		return( lmpICC_ConnectReq( dc )) ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
	case	SC_Sniff :
		break ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	int		IrDACB	lmpCbSC_ConnectInd( dword addr )
{
	DEVICECONFIG	*dc ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_ConnectInd():" ) ;
	_DPUTD( DL_LMPINFO, " addr:", addr ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
		if(( dc = lmpMakeDeviceConfig( addr )) == (DEVICECONFIG*)0 )
			return( IrDA_CONNECTDI ) ;
		return( lmpICC_ConnectInd( dc )) ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
	case	SC_Sniff :
		break ;
	}


	return( IrDA_CONNECTDI ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	IrDACB	lmpCbSC_ConnectCnf( int status )
{
	DEVICECONFIG	*dc ;
	int				ret ;


	dword			addr ;
	addr	= DeviceConfig[0].Address ;


	if( status == IrDAR_MEDIABUSY ) {
		ret		= IrLAP_ConnectReq( addr, lmpCbSC_ConnectCnf ) ;
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_ConnectCnf():" ) ;
		_DPUTD( DL_LMPINFO, " IrLAP_ConnectReq( ", addr ) ;
		_DPUTW( DL_LMPINFO, " , lmpCbSC_ConnectCnf )=", ret ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		if( ret == IrDAR_ACCEPT )
			return ;
	}


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_ConnectCnf():" ) ;
	_DPUTD( DL_LMPINFO, " addr:", addr ) ;
	_DPUTW( DL_LMPINFO, " status:", status ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
		if(( dc = lmpMakeDeviceConfig( addr )) == (DEVICECONFIG*)0 )
			return ;
		lmpICC_ConnectCnf( dc, status ) ;
		return ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
	case	SC_Sniff :
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpSC_DisconnectReq( DEVICECONFIG *dc, int reason )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpSC_DisconnectReq():" ) ;
	_DPUTW( DL_LMPINFO, " reason:", reason ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
	case	SC_Exclusive :
	case	SC_RoleExchange :
		lmpICC_DisconnectReq( dc, reason ) ;
		break ;
	case	SC_Discover :							
	case	SC_ResolveAddr :
	case	SC_ExclusivePend :						
	case	SC_ReadyPend :							
	case	SC_Sniff :								
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbSC_DisconnectInd( int status )
{
	DEVICECONFIG	*dc ;


	dword			addr ;
	addr	= DeviceConfig[0].Address ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_DisconnectInd():" ) ;
	_DPUTW( DL_LMPINFO, " status:", status ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
		dc		= lmpGetDeviceConfig( addr ) ;				
		if( dc )
			lmpICC_DisconnectInd( dc, status ) ;
		break ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_Sniff :
		break ;
	}
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_DisconnectInd(): done\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORESET)


static	IrDABOOL	IrDACB	lmpCbSC_ResetInd( void )
{
	DEVICECONFIG	*dc ;


	dword			addr ;
	addr	= DeviceConfig[0].Address ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_ResetInd():" ) ;
	_DPUTW( DL_LMPINFO, " SCStatus:", SCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( SCStatus ) {
	case	SC_Ready :
	case	SC_ExclusivePend :
	case	SC_Exclusive :
	case	SC_ReadyPend :
	case	SC_RoleExchange :
		dc		= lmpGetDeviceConfig( addr ) ;				
		if( dc )
			return( lmpICC_ResetInd( dc )) ;
		break ;
	case	SC_Discover :
	case	SC_ResolveAddr :
	case	SC_Sniff :
		break ;
	}
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbSC_ResetInd(): done\n" ) ;
#endif	


	return( FALSE ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	int		lmpSC_SetPrimaryInd( int (IrDACB *indp)( void ))
{
	return( IrLAP_SetPrimaryInd( indp )) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORXCHG)


static	int		lmpSC_PrimaryReq( void (IrDACB *cnfp)( int status ))
{
	return( IrLAP_PrimaryReq( cnfp )) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void		lmpCallDiscoveryCnf( int status, int logsz )
{
	void	(IrDACB *func)( int status, uint logsz ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCallDiscoveryCnf():" ) ;
	_DPUTD( DL_LMPINFO, " SCDiscoveryCnfPtr:", (dword)SCDiscoveryCnfPtr ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( SCDiscoveryCnfPtr == 0 )
		return ;


	func				= SCDiscoveryCnfPtr ;
	SCDiscoveryCnfPtr	= 0 ;
	(*func)( status, logsz ) ;
}
#endif	
#endif	


#if !defined(IrLAPNOSNIFF)
#if !defined(IrDANOSTATIC)


static	int				lmpSC_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void ))
{
	SniffIndPtr	= indp ;
	SniffLogPtr	= logp ;
	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpSC_SniffInd( void )
{
	if( SniffIndPtr ) {
		if( SniffLogPtr )
			IrDA_memcpy( SniffLogPtr, &SniffLog, sizeof( DISCOVERYLOG )) ;


		if((*SniffIndPtr)() == IrDA_CONNECTEN )
			IrLAP_SniffConnectReq( lmpCbSC_ConnectCnf ) ;
		else
			IrLAP_SniffConnectReq( lmpCbSC_ConnectCnf ) ;
	}
}
#endif	
#endif	


#endif	
