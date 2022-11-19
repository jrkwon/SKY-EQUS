


#if !defined(__pirlmplsap_h__)
#define		__pirlmplsap_h__


#define	LSAP_S_Disconnected		0
#define	LSAP_S_ConnectPEnd		1
#define	LSAP_S_Connect			2
#define	LSAP_S_DTR				3
#define	LSAP_S_SetupPEnd		4
#define	LSAP_S_Setup			5
#define	LSAP_S_LockedOut		6
#define	LSAP_S_DTRLocked		7
#define	LSAP_S_DTRIdle			8


static	uint	DataCnfLSAP ;		


static	void	lmpLSAP_Initialize( void ) ;
static	int		lmpLSAP_Request( void ) ;
static	int		lmpLSAP_Bind( int lsap, LMCALLBACK IrDAFAR *lmc ) ;
static	void	lmpLSAP_InitLSAP( int no, LMCALLBACK IrDAFAR *lmc ) ;
#if defined(IrDAPRIMARY)
static	int		lmpLSAP_ConnectReq( dword addr, int slsap, int dlsap, void IrDAFAR *data, int datasz ) ;
#endif	
static	int		lmpLSAP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz ) ;
static	void	IrDACB	lmpCbLSAP_DataInd( const byte IrDAFAR *data, uint rcvsz ) ;
#if !defined(IrLMPNOUI)
static	void	IrDACB	lmpCbLSAP_UDataInd( const byte IrDAFAR *data, uint rcvsz ) ;
#endif	
static	void	IrDACB	lmpCbLSAP_DataCnf( uint freebufsz ) ;
static	int		lmpLSAP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
static	void	lmpLSAP_Disconnect( LSAPCONFIG *lc, int next, int reason ) ;


static	void	lmpREventDisconnected( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#if 0
static	void	lmpREventConnectPEnd( dword addr, const byte IrDAFAR *data, uint datasz ) ;
static	void	lmpREventConnect( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#endif
static	void	lmpREventDTR( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#if 0
static	void	lmpREventSetupPEnd( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#endif
static	void	lmpREventSetup( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#if 0
static	void	lmpREventLockedOut( dword addr, const byte IrDAFAR *data, uint datasz ) ;
static	void	lmpREventDTRLocked( dword addr, const byte IrDAFAR *data, uint datasz ) ;
static	void	lmpREventDTRIdle( dword addr, const byte IrDAFAR *data, uint datasz ) ;
#endif
#define	lmpREventConnectPEnd	lmpREventMissing
#define	lmpREventConnect		lmpREventMissing
#define	lmpREventSetupPEnd		lmpREventMissing
#define	lmpREventLockedOut		lmpREventMissing
#define	lmpREventDTRLocked		lmpREventMissing
#define	lmpREventDTRIdle		lmpREventMissing
static	void	lmpREventMissing( dword addr, const byte IrDAFAR *data, uint datasz ) ;


static	int		lmpCallConnectInd( dword addr, int dlsap, int slsap, LMCONNDAT IrDAFAR *lcd ) ;
#if defined(IrDAPRIMARY)
static	void	lmpCallConnectCnf( dword addr, int slsap, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
#endif	
static	void	lmpCallDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
static	void	lmpCallDataInd( int lsap, const void IrDAFAR *data, int datasz ) ;


static	void	(*const RecvEventFunc[])( dword addr, const byte IrDAFAR *data, uint datasz ) = {
	lmpREventDisconnected,	lmpREventConnectPEnd,	lmpREventConnect,
	lmpREventDTR,			lmpREventSetupPEnd,	lmpREventSetup,
	lmpREventLockedOut,	lmpREventDTRLocked,	lmpREventDTRIdle,
} ;


#if !defined(IrDANOSTATIC)


static	void	lmpLSAP_Initialize( void )
{
	int		i ;


	for( i=0 ; i<LSAPMAX+1 ; i++ )
		LSAPConfig[i].Mode	= 0 ;


	DataCnfLSAP		= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpLSAP_Request( void )
{
	int		i ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_Request():" ) ;
	_DPUTW( DL_LMPINFO, " LSAPConfig[].Mode:", LSAPConfig[1].Mode ) ;
	for( i=2 ; i<LSAPMAX+1 ; i++ )
		_DPUTW( DL_LMPINFO, ".", LSAPConfig[i].Mode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	for( i=1 ; i<LSAPMAX+1 ; i++ )
		if(( LSAPConfig[i].Mode & LCM_Used ) == 0 )
			return( i ) ;


	return( IrDAR_NORESOURCE ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpLSAP_Bind( int lsap, LMCALLBACK IrDAFAR *lmc )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_Bind():" ) ;
	_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTB( DL_LMPINFO, " LSAPMAX:", LSAPMAX ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( lsap > LSAPMAX )
		return( IrDAR_NORESOURCE ) ;


#if	defined(DMSG)
	if( LSAPConfig[lsap].Mode & LCM_Used ) {
		_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_Bind():" ) ;
		_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
		_DPUTS( DL_LMPINFO, " is allready in use" ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
	}
#endif	
	if( LSAPConfig[lsap].Mode & LCM_Used )
		return( IrDAR_INUSE ) ;


	lmpLSAP_InitLSAP( lsap, lmc ) ;
	LSAPConfig[lsap].Mode	= LCM_Used ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_Bind():" ) ;
	_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTS( DL_LMPINFO, " bind!!" ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpLSAP_Unbind( int lsap )
{
	LSAPCONFIG		*lc ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_Unbind():" ) ;
	_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( lsap > LSAPMAX )
		return ;


	lc		= &LSAPConfig[lsap] ;
	if(( lc->Mode & LCM_Used ) == 0 )
		return ;


	switch( lc->Status ) {
	case	LSAP_S_Setup :
	case	LSAP_S_DTR :
		lmpSendPDUDisconnect( lsap, IrLMP_DR_UserRequest, 0, 0 ) ;
		lmpLSAP_Disconnect( lc, LSAP_S_Disconnected, IrLMP_DR_UserRequest ) ;
	}


	lc->Mode	= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpLSAP_InitLSAP( int no, LMCALLBACK IrDAFAR *lmc )
{
	LSAPCONFIG	*lc ;


	lc		= &LSAPConfig[no] ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_InitLSAP():" ) ;
	_DPUTB( DL_LMPINFO, " lsap:", no ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	lc->Device			= 0 ;
	lc->Mode			= 0 ;
	lc->Status			= LSAP_S_Disconnected ;
	IrDA_memcpy( &lc->CallBack, lmc, sizeof( LMCALLBACK )) ;


	lc->dLSAP			= -1 ;
	lc->lmpConnectDataSize	= 0 ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	int		lmpLSAP_ConnectReq( dword addr, int slsap, int dlsap, void IrDAFAR *data, int datasz )
{
	int			ret ;
	LSAPCONFIG	*lc ;


	lc		= &LSAPConfig[slsap] ;
	if( lc->Mode & LCM_Connected )
		return( IrDAR_CONNECTING ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectReq():" ) ;
	_DPUTW( DL_LMPINFO, " Status:", lc->Status ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( lc->Status ) {
	case	LSAP_S_Disconnected :
		lc->Mode		&= ~LCM_RsvConnectRsp ;
		if( datasz > CDATAMAX )
			datasz	= CDATAMAX ;
		lc->lmpConnectDataSize	= datasz ;
		IrDA_memcpy( lc->lmpConnectData, data, datasz ) ;
		ret		= lmpSC_ConnectReq( addr ) ;
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectReq():" ) ;
		_DPUTW( DL_LMPINFO, " lmpSC_ConnectReq()=", ret ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		if( ret == IrDAR_ACCEPT || ret == IrDAR_CONNECTING ) {
			lc->Device		= lmpGetDeviceConfig( addr ) ;
			lc->Status		= LSAP_S_SetupPEnd ;
			lc->dLSAP		= dlsap ;
			lmpICC_StopIdleTimer( lc->Device  ) ;
			lc->Device->LSAPCount	++ ;
#if	defined(DMSG)
			_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectReq():" ) ;
			_DPUTW( DL_LMPINFO, " lsap:", slsap ) ;
			_DPUTW( DL_LMPINFO, " LSAPCount:", lc->Device->LSAPCount ) ;
			_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
			if( ret == IrDAR_CONNECTING ) {
				if( IrLAP_QueryTxQueue()) {
					lmpLSAP_ConnectCnf( slsap, IrDAR_DONE ) ;
					ret	= IrDAR_ACCEPT ;
				} else {
					lc->Status	= LSAP_S_Disconnected ;
					lc->Device->LSAPCount	-- ;
#if	defined(DMSG)
					_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectReq():" ) ;
					_DPUTW( DL_LMPINFO, " LSAPCount:", lc->Device->LSAPCount ) ;
					_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
					ret	= IrDAR_BUSYWAIT ;
				}
			} else {
				ret	= IrDAR_ACCEPT ;
			}
		}
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectReq():" ) ;
		_DPUTW( DL_LMPINFO, " ret:", ret ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		return( ret ) ;
	case	LSAP_S_ConnectPEnd :
	case	LSAP_S_Connect :
	case	LSAP_S_DTR :
	case	LSAP_S_SetupPEnd :
	case	LSAP_S_Setup :
	case	LSAP_S_LockedOut :
	case	LSAP_S_DTRLocked :
	case	LSAP_S_DTRIdle :
		break ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpLSAP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz )
{
	int			ret ;
	LSAPCONFIG	*lc ;


	lc		= &LSAPConfig[lsap] ;
	if(( lc->Mode & LCM_Used ) == 0 ||
	   ( lc->Mode & LCM_Connected ) == 0 )
		return( IrDAR_INVALID ) ;


#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_DisconnectReq():" ) ;
	_DPUTW( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_LMPINFO, " reason:", reason ) ;
	_DPUTW( DL_LMPINFO, " Status:", lc->Status ) ;
	_DPUTW( DL_LMPINFO, " datasz:", datasz ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	


#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_LMPINFO, "                         : data=" ) ;
	for( i=0 ; i<datasz ; i++ ) {
		if( ((byte*)data)[i] < ' ' ) {
			_DPUTB( DL_LMPINFO, "[%02x]", ((byte*)data)[i] ) ;
		} else {
			_DPUTC( DL_LMPINFO, "%c", ((byte*)data)[i] ) ;
		}
	}
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
	switch( lc->Status ) {
	case	LSAP_S_Disconnected :					
	case	LSAP_S_ConnectPEnd :
	case	LSAP_S_SetupPEnd :
	case	LSAP_S_Setup :
	case	LSAP_S_LockedOut :
		break ;
	case	LSAP_S_Connect :
	case	LSAP_S_DTR :
	case	LSAP_S_DTRIdle :
		if(( ret = lmpSendPDUDisconnect( lsap, reason, data, datasz )) != IrDAR_ACCEPT )
			return( ret ) ;
		lmpLSAP_Disconnect( lc, LSAP_S_Disconnected, reason ) ;
		return( IrDAR_ACCEPT ) ;
	case	LSAP_S_DTRLocked :
		if(( ret = lmpSendPDUDisconnect( lsap, reason, data, datasz )) != IrDAR_ACCEPT )
			return( ret ) ;
		lmpLSAP_Disconnect( lc, LSAP_S_LockedOut, reason ) ;
		return( IrDAR_ACCEPT ) ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	lmpLSAP_ConnectCnf( int lsap, int status )
{
	LSAPCONFIG	*lc ;
	LMCONNDAT	lcd ;


	lc		= &LSAPConfig[lsap] ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_ConnectCnf():" ) ;
	_DPUTW( DL_LMPINFO, " LSAPConfig[", lsap ) ;
	_DPUTW( DL_LMPINFO, "].Status:", LSAPConfig[lsap].Status ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( lc->Status ) {
	case	LSAP_S_Disconnected :
	case	LSAP_S_ConnectPEnd :
	case	LSAP_S_Connect :
	case	LSAP_S_DTR :
		break ;
	case	LSAP_S_SetupPEnd :
		if( status == IrDAR_DONE ) {
			lmpSendPDUConnect( lsap, 0, lc->lmpConnectData, lc->lmpConnectDataSize ) ;


			lc->Status		= LSAP_S_Setup ;
		} else {							
			lcd.Data		= 0 ;
			lcd.DataSize	= 0 ;
			lc->Status		= LSAP_S_Disconnected ;
			lmpCallConnectCnf( lc->Device->Address, lsap, lc->dLSAP, &lcd, status ) ;
		}
		return ;
	case	LSAP_S_Setup :
	case	LSAP_S_LockedOut :
	case	LSAP_S_DTRLocked :
	case	LSAP_S_DTRIdle :
		break ;
	}
	return ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpLSAP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz )
{
	return( lmpSendPDUData( lsap, bufp, bufsz )) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpLSAP_Disconnect( LSAPCONFIG *lc, int next, int reason )
{
	lc->Mode				&= ~LCM_Connected ;
	lc->Status				= next ;
	lc->Device->LSAPCount	-- ;
	lmpSC_DisconnectReq( lc->Device, reason ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbLSAP_DataInd( const byte IrDAFAR *data, uint rcvsz )
{
	int		lsap ;


	dword			addr ;
	addr	= DeviceConfig[0].Address ;


	if( rcvsz < 2 )									
		return ;


	lsap		= data[0] & 0x7F ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbLSAP_DataInd():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", lsap ) ;
	_DPUTW( DL_LMPINFO, "].Mode:", LSAPConfig[lsap].Mode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
													
	if( lsap > LSAPMAX || ( LSAPConfig[lsap].Mode & LCM_Used ) == 0 ) {
		lmpSendPDUDisconnect2( lsap, data[1] & 0x7F, IrLMP_DR_NoAvailableLMClient ) ;
		return ;
	}


	
	if( !lmpGetDeviceConfig( addr ))
		return ;


#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_LMPINFO, " LMP::lmpCbLSAP_DataInd():" ) ;
	_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_LMPINFO, " datasz:", rcvsz ) ;
	_DPUTW( DL_LMPINFO, " Status:", LSAPConfig[lsap].Status ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;


	if( rcvsz > 0 ) {
		_DPUTB( DL_LMPDAT, "                    : Data:", data[0] ) ;
		for( i=1 ; i<rcvsz ; i++ )
			_DPUTB( DL_LMPDAT, ".", data[i] ) ;
		_DPUTS( DL_LMPDAT, "\n" ) ;
	}
}
#endif	


	(*RecvEventFunc[LSAPConfig[lsap].Status])( addr, data, rcvsz ) ;
}
#endif	


#if !defined(IrLMPNOUI)
# if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbLSAP_UDataInd( const byte IrDAFAR *data, uint rcvsz )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCbLSAP_UDataInd():" ) ;
	_DPUTW( DL_LMPINFO, " rcvsz:", rcvsz ) ;
	_DDUMP( DL_LMPINFO, data, rcvsz, 384 ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( rcvsz < 3 || data[0] != CLESSLSAP || data[1] != CLESSLSAP )
		return ;


	(*CLessDataIndPtr)( data + 2, rcvsz - 2 ) ;
}
# endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpCbLSAP_DataCnf( uint freebufsz )
{
	int			i ;
	uint		mode ;
	LSAPCONFIG	*lc ;


	UNREFERENCE( freebufsz ) ;


	for( i=0 ; i<LSAPMAX+1 ; i++ ) {
		lc		= &LSAPConfig[DataCnfLSAP] ;
		mode	= lc->Mode ;
		if(( mode & LCM_Used ) && ( mode & LCM_Connected )) {
			if( mode & LCM_RsvConnectRsp ) {
				if( lmpSendPDUConnect( DataCnfLSAP, LMPDU_Confirm, lc->lmpConnectData, lc->lmpConnectDataSize ) == IrDAR_ACCEPT )
					lc->Mode	&= ~LCM_RsvConnectRsp ;
			} else if( lc->CallBack.DataCnf ) {
#if	defined(DMSG)
				_DPUTS( DL_LMPINFO, " LMP::lmpCbLSAP_DataCnf():" ) ;
				_DPUTB( DL_LMPINFO, " DataCnfLSAP:", DataCnfLSAP ) ;
				_DPUTD( DL_LMPINFO, " DataCnf:", (dword)lc->CallBack.DataCnf ) ;
				_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
#if defined(IrDA_LMPDATACNFPRB)
				IrDA_LMPDATACNFPRB( 0 ) ;
#endif	
				if( DataCnfLSAP == 0 )
					(*lc->CallBack.DataCnf)( DataCnfLSAP ) ;
				else
					(*lc->CallBack.DataCnf)( DataCnfLSAP - 1 ) ;
#if defined(IrDA_LMPDATACNFPRB)
				IrDA_LMPDATACNFPRB( 1 ) ;
#endif	
			}
		}


		DataCnfLSAP	++ ;
		if( DataCnfLSAP >= LSAPMAX+1 )
			DataCnfLSAP	= 0 ;


		if( !IrLAP_QueryTxQueue())
			break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpLSAP_DisconnectInd( int lsap, int reason )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpLSAP_DisconnectInd():" ) ;
	_DPUTW( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_LMPINFO, " reason:", reason ) ;
	_DPUTW( DL_LMPINFO, " LSAPConfig[].Status:", LSAPConfig[lsap].Status ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( LSAPConfig[lsap].Status ) {
	case	LSAP_S_Disconnected :					
	case	LSAP_S_LockedOut :						
		break ;
	case	LSAP_S_ConnectPEnd :
	case	LSAP_S_Connect :
	case	LSAP_S_DTR :
	case	LSAP_S_SetupPEnd :
	case	LSAP_S_Setup :
	case	LSAP_S_DTRLocked :
	case	LSAP_S_DTRIdle :
		lmpCallDisconnectInd( lsap, reason, 0, 0 ) ;
		LSAPConfig[lsap].Status	= LSAP_S_Disconnected ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpREventDisconnected( dword addr, const byte IrDAFAR *data, uint datasz )
{
	int			lsap ;
	int			cdatasz ;
	int			ret ;
	LMCONNDAT	lcd ;
	LSAPCONFIG	*lc ;


	lsap	= data[0] & 0x7F ;
	lc		= &LSAPConfig[lsap] ;


	if(( data[0] & 0x80 ) == 0 )					
		return ;


	switch( data[2] & 0x7F ) {
	case	LMPDU_Connect :							
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpREventDisconnected():" ) ;
		_DPUTB( DL_LMPINFO, " received LMP Connect Req to ", lsap ) ;
		_DPUTW( DL_LMPINFO, " datasz:", datasz ) ;
		_DPUTB( DL_LMPINFO, " data[3]=", (byte)data[3] ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
#if	defined(DMSG)
		if( lsap > LSAPMAX || ( lc->Mode & LCM_Used ) == 0 ) {
			_DPUTS( DL_LMPINFO, " LMP::lmpREventDisconnected():" ) ;
			_DPUTB( DL_LMPINFO, " lsap:", lsap ) ;
			_DPUTS( DL_LMPINFO, " is not used!!" ) ;
			_DPUTS( DL_LMPINFO, "\n" ) ;
		}
#endif	
		if( data[2] & 0x80 ) {					
			return ;
		}


		cdatasz		= datasz - 4 ;
		if( cdatasz >= CDATAMAX )
			cdatasz	= CDATAMAX ;
		lc->lmpConnectDataSize	= cdatasz ;
		IrDA_memcpy( lc->lmpConnectData, &data[4], cdatasz ) ;
		lcd.Data		= lc->lmpConnectData ;
		lcd.DataSize	= lc->lmpConnectDataSize ;


		lc->Mode		|= LCM_Connected ;
		lc->Mode		&= ~LCM_RsvConnectRsp ;
		lc->Device		= lmpGetDeviceConfig( addr ) ;
		lc->dLSAP		= data[1] & 0x7F ;
		lc->Status		= LSAP_S_DTR ;


#if	defined(DMSG)
{	int		i ;
		_DPUTS( DL_LMPINFO, " LMP::lmpREventDisconnected():" ) ;
		_DPUTW( DL_LMPINFO, " lcd.DataSize:", lcd.DataSize ) ;
		if( lcd.DataSize > 0 ) {
			_DPUTB( DL_LMPINFO, " lcd.Data:", lcd.Data[0] ) ;
			for( i=1 ; i<lcd.DataSize ; i++ )
				_DPUTB( DL_LMPINFO, ".", lcd.Data[i] ) ;
		}
		_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
		ret				= lmpCallConnectInd( addr, data[1] & 0x7F, lsap, &lcd ) ;
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpREventDisconnected():" ) ;
		_DPUTW( DL_LMPINFO, " lmpCallConnectInd()=", ret ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		if( ret == IrDA_CONNECTEN ) {
			lmpICC_StopIdleTimer( lc->Device  ) ;
			lc->Device->LSAPCount	++ ;
			ret		= lmpSendPDUConnect( lsap, LMPDU_Confirm, lcd.Data, lcd.DataSize ) ;
#if	defined(DMSG)
			_DPUTS( DL_LMPINFO, " LMP::lmpREventDisconnected():" ) ;
			_DPUTW( DL_LMPINFO, " LSAPCount:", lc->Device->LSAPCount ) ;
			_DPUTW( DL_LMPINFO, " lmpSendPDUConnect():", ret ) ;
			_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
			if( ret == IrDAR_NORESOURCE ) {
				IrDA_memcpy( lc->lmpConnectData, lcd.Data, CDATAMAX ) ;
				lc->lmpConnectDataSize	= lcd.DataSize ;
				lc->Mode			|= LCM_RsvConnectRsp ;
			}
			return ;
		}
		lmpSendPDUDisconnect( lsap, IrLMP_DR_LMInitiatedDisconnect, 0, 0 ) ;
		lc->Mode		&= ~LCM_Connected ;
		lc->Status		= LSAP_S_Disconnected ;
		return ;
	case	LMPDU_Disconnect :						
	case	3 :										
		return ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventConnectPEnd( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventConnect( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpREventDTR( dword addr, const byte IrDAFAR *data, uint datasz )
{
	int			lsap ;
	LSAPCONFIG	*lc ;


	addr	= addr ;								
	lsap	= data[0] & 0x7F ;
	lc		= &LSAPConfig[lsap] ;


	if(( data[0] & 0x80 ) == 0 ) {					
													
		if(( lc->Mode & LCM_Used ) == 0 )
			return ;


		lmpCallDataInd( lsap, &data[2], datasz - 2 ) ;
		return ;
	}


	switch( data[2] & 0x7F ) {
	case	LMPDU_Disconnect :
		lmpLSAP_Disconnect( lc, LSAP_S_Disconnected, data[3] ) ;
		lmpCallDisconnectInd( lsap, data[3], &data[4], datasz - 4 ) ;
		return ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventSetupPEnd( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpREventSetup( dword addr, const byte IrDAFAR *data, uint datasz )
{
	int			slsap ;
	LSAPCONFIG	*lc ;
#if defined(IrDAPRIMARY)
	LMCONNDAT	lcd ;
#endif	


	slsap	= data[0] & 0x7F ;
	lc		= &LSAPConfig[slsap] ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpREventSetup():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", slsap ) ;
	_DPUTW( DL_LMPINFO, "].Mode:", lc->Mode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if(( data[0] & 0x80 ) == 0 )					
		return ;


	switch( data[2] & 0x7F ) {
#if defined(IrDAPRIMARY)
	case	1 :										
		if( data[2] & 0x80 ) {						
			lcd.Data		= (byte IrDAFAR *)&data[4] ;
			lcd.DataSize	= datasz - 4 ;
			lc->Mode		|= LCM_Connected ;
			lc->Status		= LSAP_S_DTR ;
			lmpCallConnectCnf( addr, slsap, data[1] & 0x7F, &lcd, IrDAR_CONNECTING ) ;
			return ;
		}
		lmpSendPDUDisconnect( slsap, IrLMP_DR_ConnectionHalfOpen, 0, 0 ) ;
		lmpLSAP_Disconnect( lc, LSAP_S_Disconnected, IrLMP_DR_ConnectionHalfOpen ) ;
		lcd.Data		= 0 ;
		lcd.DataSize	= 0 ;
		lmpCallConnectCnf( addr, slsap, data[1] & 0x7F, &lcd, IrDAR_DISCONNECT ) ;
		return ;
		
	case	2 :										
		lmpLSAP_Disconnect( lc, LSAP_S_Disconnected, IrLMP_DR_NoAvailableLMClient ) ;
		lcd.Data		= (byte IrDAFAR *)&data[4] ;
		lcd.DataSize	= datasz - 4 ;
		lmpCallConnectCnf( addr, slsap, data[1] & 0x7F, &lcd, IrDAR_DISCONNECT ) ;
		return ;
#endif	
	case	3 :										
		return ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventLockedOut( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventDTRLocked( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


#if 0
static	void	lmpREventDTRIdle( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpREventMissing( dword addr, const byte IrDAFAR *data, uint datasz )
{
	UNREFERENCE( addr ) ;
	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpCallConnectInd( dword addr, int dlsap, int slsap, LMCONNDAT IrDAFAR *lcd )
{
	if( PrvConnect )
		(*PrvConnect)() ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCallConnectInd():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", slsap ) ;
	_DPUTD( DL_LMPINFO, "].CallBack.ConnectInd:", (dword)LSAPConfig[slsap].CallBack.ConnectInd ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( LSAPConfig[slsap].CallBack.ConnectInd == 0L )
		return( IrDA_CONNECTDI ) ;


	return((*LSAPConfig[slsap].CallBack.ConnectInd)( slsap-1, addr, dlsap, lcd )) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	lmpCallConnectCnf( dword addr, int slsap, int dlsap, LMCONNDAT IrDAFAR *lcd, int status )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCallConnectCnf():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", slsap ) ;
	_DPUTD( DL_LMPINFO, "].CallBack.ConnectCnf:", (dword)LSAPConfig[slsap].CallBack.ConnectCnf ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( PrvConnect )
		(*PrvConnect)() ;


	if( LSAPConfig[slsap].CallBack.ConnectCnf )
		(*LSAPConfig[slsap].CallBack.ConnectCnf)( slsap-1, addr, dlsap, lcd, status ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpCallDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz )
{
	int		tmp ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCallDisconnectInd():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", lsap ) ;
	_DPUTD( DL_LMPINFO, "].CallBack.DisconnectInd:", (dword)LSAPConfig[lsap].CallBack.DisconnectInd ) ;
	_DPUTW( DL_LMPINFO, " reason:", reason ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( LSAPConfig[lsap].CallBack.DisconnectInd ) {
		tmp		= lsap ;
		if( tmp )
			tmp	-- ;
		(*LSAPConfig[lsap].CallBack.DisconnectInd)( tmp, reason, data, datasz ) ;
	}


	if( PrvDisconnect )
		(*PrvDisconnect)() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	lmpCallDataInd( int lsap, const void IrDAFAR *data, int datasz )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpCallDataInd():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", lsap ) ;
	_DPUTD( DL_LMPINFO, "].CallBack.DataInd:", (dword)LSAPConfig[lsap].CallBack.DataInd ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	*RxSizePtr	+= (ulong)datasz ;
	if( LSAPConfig[lsap].CallBack.DataInd ) {
#if defined(IrDA_LMPDATAINDPRB)
		IrDA_LMPDATAINDPRB( 0 ) ;
#endif	
		(*LSAPConfig[lsap].CallBack.DataInd)( lsap-1, data, datasz ) ;
#if defined(IrDA_LMPDATAINDPRB)
		IrDA_LMPDATAINDPRB( 1 ) ;
#endif	
	}
}
#endif	


#endif	
