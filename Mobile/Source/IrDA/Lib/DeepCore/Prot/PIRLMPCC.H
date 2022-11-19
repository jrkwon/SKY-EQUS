


#if !defined(__prilmpcc_h__)
#define		__prilmpcc_h__


#include	"irlmp.h"


#define	ICC_STANDBY				0
#define	ICC_UCONNECT			1
#define	ICC_ACTIVE				2


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	int		lmpICC_ConnectReq( DEVICECONFIG *dc )
{
	int		ret ;


	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :
		if(( ret = IrLAP_ConnectReq( dc->Address, lmpCbSC_ConnectCnf )) == IrDAR_ACCEPT )
			dc->ICCStatus	= ICC_UCONNECT ;
		return( ret ) ;
	case	ICC_UCONNECT :
	case	ICC_ACTIVE :
		return( IrDAR_CONNECTING ) ;
	}


	return( IrDAR_INVALID ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	int		lmpICC_ConnectInd( DEVICECONFIG *dc )
{
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_ConnectInd():" ) ;
	_DPUTW( DL_LMPINFO, " dc->ICCStatus=", dc->ICCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :
		dc->Mode		|= DCM_Connected ;
		dc->ICCStatus	= ICC_ACTIVE ;
		return( IrDA_CONNECTEN ) ;					
	case	ICC_UCONNECT :
		return( IrDA_CONNECTEN ) ;					
	case	ICC_ACTIVE :
		break ;
	}


	return( IrLAP_CONNECTDI ) ;						
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	lmpICC_ConnectCnf( DEVICECONFIG *dc, int status )
{
	int			i ;
	LSAPCONFIG	*lc ;
	dword		addr ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_ConnectCnf():" ) ;
	_DPUTW( DL_LMPINFO, " status:", status ) ;
	_DPUTW( DL_LMPINFO, " dc->ICCStatus:", dc->ICCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :
		return ;									
	case	ICC_UCONNECT :
		addr	= dc->Address ;
		if( status == IrDAR_DONE ) {
			dc->Mode		|= DCM_Connected ;
			dc->ICCStatus	= ICC_ACTIVE ;
		} else {
			dc->ICCStatus	= ICC_STANDBY ;
			lmpFreeDeviceConfig( dc ) ;
		}


		for( i=0 ; i<LSAPMAX+1 ; i++ ) {
#if	defined(DMSG)
			_DPUTS( DL_LMPINFO, "\n LMP::lmpICC_ConnectCnf():" ) ;
			_DPUTB( DL_LMPINFO, " LSAPConfig[", (byte)i ) ;
			_DPUTS( DL_LMPINFO, "]" ) ;
#endif	
			lc		= &LSAPConfig[i] ;
			if(( lc->Mode & LCM_Used ) == 0 		||
				lc->Device == 0						||
				lc->Device->Address != addr )
				continue ;
			lmpLSAP_ConnectCnf( i, status ) ;
		}
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		return ;
	case	ICC_ACTIVE :
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpICC_DisconnectReq( DEVICECONFIG *dc, int reason )
{
	UNREFERENCE( reason ) ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectReq():" ) ;
	_DPUTW( DL_LMPINFO, " lsap:", lsap ) ;
	_DPUTW( DL_LMPINFO, " reason:", reason ) ;
	_DPUTW( DL_LMPINFO, " ICCStatus:", dc->ICCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;


	_DPUTS( DL_LMPINFO, "                         :" ) ;
	_DPUTW( DL_LMPINFO, " LSAPCount:", dc->LSAPCount ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :							
		break ;
	case	ICC_UCONNECT :
		if( dc->LSAPCount > 1 ) {
#if	defined(DMSG)
			_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectReq():" ) ;
			_DPUTW( DL_LMPINFO, " lsap:", lsap ) ;
			_DPUTW( DL_LMPINFO, " LSAPCount:", dc->LSAPCount ) ;
			_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
			break ;
		}
		IrLAP_DisconnectReq() ;
		dc->LSAPCount	= 0 ;
		dc->Mode		&= ~DCM_Connected ;
		dc->ICCStatus	= ICC_STANDBY ;
#if	defined(DMSG)
			_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectReq():" ) ;
			_DPUTW( DL_LMPINFO, " lsap:", lsap ) ;
			_DPUTW( DL_LMPINFO, " LSAPCount:", dc->LSAPCount ) ;
			_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		lmpFreeDeviceConfig( dc ) ;
		break ;
	case	ICC_ACTIVE :
		if( dc->LSAPCount <= 0 && !disableIdleTimer )
			dc->IdleTimerHndl	= StartTimer( TIME_IdleTimer, lmpICC_IdleTimerExpired ) ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpICC_DisconnectInd( DEVICECONFIG *dc, int status )
{
	int			i ;
	LSAPCONFIG	*lc ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectInd():" ) ;
	_DPUTW( DL_LMPINFO, " dc->ICCStatus:", dc->ICCStatus ) ;
	_DPUTW( DL_LMPINFO, " status:", status ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :							
		break ;
	case	ICC_UCONNECT :
	case	ICC_ACTIVE :
		dc->Mode	&= ~DCM_Connected ;
		for( i=0 ; i<LSAPMAX+1 ; i++ ) {
			lc		= &LSAPConfig[i] ;
			if(( lc->Mode & LCM_Used ) == 0 )
				continue ;


			if( lc->Device != dc )
				continue ;
			lc->Mode	&= ~LCM_Connected ;
			lmpLSAP_DisconnectInd( i, status ) ;
		}
		dc->LSAPCount	= 0 ;
#if	defined(DMSG)
		_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectInd():" ) ;
		_DPUTW( DL_LMPINFO, " LSAPCount:", dc->LSAPCount ) ;
		_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		dc->ICCStatus	= ICC_STANDBY ;
		lmpFreeDeviceConfig( dc ) ;
		break ;
	}
#if	defined(DMSG2)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_DisconnectInd(): done\n" ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrLAPNORESET)


static	IrDABOOL		lmpICC_ResetInd( DEVICECONFIG *dc )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_ResetInd():" ) ;
	_DPUTW( DL_LMPINFO, " dc->ICCStatus:", dc->ICCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :							
	case	ICC_UCONNECT :							
		break ;
	case	ICC_ACTIVE :
		return( FALSE ) ;
	}


	return( TRUE ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void		lmpICC_StopIdleTimer( DEVICECONFIG *dc )
{
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_StopIdleTimer():" ) ;
	_DPUTW( DL_LMPINFO, " IdleTimerHndl:", dc->IdleTimerHndl ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	if( dc->IdleTimerHndl >= 0 ) {
		StopTimer( dc->IdleTimerHndl ) ;
		dc->IdleTimerHndl	= -1 ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	lmpICC_IdleTimerExpired( void )
{
	int			i ;
	LSAPCONFIG	*lc ;


	DEVICECONFIG	*dc ;
	dc		= &DeviceConfig[0] ;


#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, "TMR<<<\n" ) ;
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_IdleTimerExpired():" ) ;
	_DPUTW( DL_LMPINFO, " ICCStatus:", dc->ICCStatus ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
	switch( dc->ICCStatus ) {
	case	ICC_STANDBY :
	case	ICC_UCONNECT :
		break ;
	case	ICC_ACTIVE :
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_IdleTimerExpired():" ) ;
	_DPUTW( DL_LMPINFO, " dc->LSAPCount:", dc->LSAPCount ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
		if( dc->LSAPCount != 0 )
			break ;


		for( i=0 ; i<LSAPMAX+1 ; i++ ) {
			lc		= &LSAPConfig[i] ;
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, " LMP::lmpICC_IdleTimerExpired():" ) ;
	_DPUTB( DL_LMPINFO, " LSAPConfig[", i ) ;
	_DPUTW( DL_LMPINFO, "].Mode:", LSAPConfig[i].Mode ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
#endif	
			if(( lc->Mode & LCM_Used ) == 0			||
			   ( lc->Mode & LCM_Connected ) == 0	||
				 lc->Device != dc )
				continue ;
			lmpLSAP_DisconnectInd( i, IrLMP_DR_UnexpectedIrLAPDisconnect ) ;
		}
		IrLAP_DisconnectReq() ;
		dc->Mode		&= ~DCM_Connected ;
		dc->ICCStatus	= ICC_STANDBY ;
		lmpFreeDeviceConfig( dc ) ;
		break ;
	}
#if	defined(DMSG)
	_DPUTS( DL_LMPINFO, "TMR>>>\n" ) ;
#endif	
}
#endif	


#endif	
