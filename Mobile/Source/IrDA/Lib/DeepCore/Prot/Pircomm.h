


#if !defined(__pircomm_h__)
#define	__pircomm_h__


#include	"irlap.h"
#include	"ircomm.h"
#include	"tinytp.h"
#include	"okmisc.h"


#if !defined(COMMMAX)
#error	COMMMAXが正しくありません。使用するサービス数（0x01 < n < 0x80）を定義して下さい。
#endif
#if !defined(COMMTXSDUSZ)
#error	COMMTXSDUSZが正しくありません。送信一時バッファのサイズを定義して下さい。
#endif
#if !defined(CTLQUEUESZ)
#error	CTLQUEUESZが正しくありません。LSAP毎の送信制御コードキューのサイズを定義して下さい。
#endif


typedef	struct	_COMMFLAGS	{
	IrDABOOL			used ;			
	IrDABOOL			connected ;		
	IrDABOOL			txsduused ;		
	IrDABOOL			inRecvData ;	
	IrDABOOL			inSendTxSdu ;	
	IrDABOOL			inSetTxSdu ;
	IrDABOOL			localBusy ;		
	IrDABOOL			acceptDataCnf ;
	IrDABOOL			inConnCallBack ;
}	COMMFLAGS ;


typedef	struct	_COMMCONFIG {
	volatile COMMFLAGS	flags ;					
	dword			Address ;					
	int				lsap ;						
	COMMCALLBACK	CallBack ;					
	uint			ServiceType ;				
	uint			PeerServiceType ;			
	uint			Status ;					


	uint			MaxTxSize ;					
	uint			MaxDataIndSize ;			


	uint			TxSduSize ;					
	byte			TxSdu[COMMTXSDUSZ] ;		


	byte			TxQueueBuf[TXQUEUESZ] ;		
	MISCQUEUE		TxQueue ;					


	byte			RxQueueBuf[(RXQUEUESZ)+1] ;	
	MISCQUEUE		RxQueue ;					


#if defined(CTLQUEUESZ)
	byte			CtlQueueBuf[CTLQUEUESZ] ;	
	MISCQUEUE		CtlQueue ;					
#else	
	void			(IrDACB *CntlCnf)( int hndl, int status ) ;
	void			*CntlData ;					
	uint			CntlDataSize ;				
	int				CntlHndl ;					
#endif	


	byte	IrDAFAR	*connDatPtr ;
	int				connDatLen ;
}	COMMCONFIG ;


#define	COMM_S_IDLE			0
#define	COMM_S_WAITI		1
#define	COMM_S_WAITR		2
#define	COMM_S_CONN			3


#if defined(DMSG2)
		COMMCONFIG	COMMConfig[COMMMAX] ;
		byte		COMMConfigEnd ;
#else	
static	COMMCONFIG	COMMConfig[COMMMAX] ;	
#endif	
#if	defined(SEQCHK)
byte	IrDAFAR		*irda_rxbufp ;
byte	IrDAFAR		*irda_rxreadp ;
byte	IrDAFAR		*irda_rxwritep ;
int					irda_rxbufsize ;
int					irda_rxreadsz ;
uint				irda_rxrwp ;
uint				irda_rxwrp ;
#endif	


#if !defined(NOTTP)
static	int		IrDACB	commCbTTPConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	commCbTTPConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status ) ;
static	void	IrDACB	commCbTTPDisconnectInd( int lsap, int status ) ;
static	void	IrDACB	commCbTTPDataInd( int lsap ) ;
static	void	IrDACB	commCbTTPDataCnf( int lsap ) ;
#endif	
#if !defined(IrCOMMNO3WIRERAW)
static	int		IrDACB	commCbLMConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	commCbLMConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
static	void	IrDACB	commCbLMDataInd( int lsap, const void IrDAFAR *data, uint datasz ) ;
static	void	IrDACB	commCbLMDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
static	void	IrDACB	commCbLMDataCnf( int lsap ) ;
#endif	


static	void	commFlushQueues( COMMCONFIG *cc ) ;
static	void	commFlushTxSdu( COMMCONFIG *cc ) ;
static	int		commCallConnectInd( int slsap, dword addr, int dsel, int servicetype ) ;
#if defined(IrDAPRIMARY)
static	void	commCallConnectCnf( int slsap, dword addr, int dlsap, int status ) ;
#endif	
static	void	commCallDisconnectInd( int lsap, int status ) ;
static	void	commCallDataInd( int lsap ) ;
static	void	commCallCtlInd( int lsap, const void IrDAFAR *ctl, int ctlsz ) ;


static	void	commTTP_RecvData( int lsap ) ;
static	int		commSendData( COMMCONFIG *cc ) ;
static	void	commSetTxSdu( COMMCONFIG *cc ) ;
static	void	commSendTxSdu( COMMCONFIG *cc ) ;
#if !defined(NOTTP)
static	void	commSetServiceType( byte IrDAFAR *p, int st ) ;
#endif	


static	void	commSetIASParameters( void ) ;


static	void	IrDACB	commDmyConnectPrv( void ) ;
static	void	IrDACB	commDmyDisconnectPrv( void ) ;


static	void	(IrDACB	*ConnectPrvPtr)( void ) ;
static	void	(IrDACB	*DisconnectPrvPtr)( void ) ;
static	ulong IrDAFAR	*TxSizePtr ;	
static	ulong IrDAFAR	*RxSizePtr ;	
static	ulong	DmyTxSize ;				
static	ulong	DmyRxSize ;				


int		IrDAAPI	IrCOMM_Open( void )
{
	COMMCONFIG	*cc ;
	int			i ;


	for( i=0 ; i<COMMMAX ; i++ ) {
		cc		= &COMMConfig[i] ;
		IrDA_memset( &cc->flags, 0, sizeof( COMMFLAGS )) ;
		MiscInitQueue( &cc->TxQueue, cc->TxQueueBuf, TXQUEUESZ ) ;
		MiscInitQueue( &cc->RxQueue, cc->RxQueueBuf, (RXQUEUESZ)+1 ) ;
		commFlushQueues( cc ) ;
#if defined(CTLQUEUESZ)
		MiscInitQueue( &cc->CtlQueue, cc->CtlQueueBuf, CTLQUEUESZ ) ;
#endif	
	}


	commSetIASParameters() ;


	ConnectPrvPtr		= commDmyConnectPrv ;
	DisconnectPrvPtr	= commDmyDisconnectPrv ;


	TxSizePtr			= &DmyTxSize ;
	RxSizePtr			= &DmyRxSize ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_Open(): done\n" ) ;
#endif	
#if	defined(SEQCHK)
	irda_rxbufp		= COMMConfig[0].RxQueueBuf ;
	irda_rxbufsize	= RXQUEUESZ + 1 ;
#endif	


	return( IrDAR_ACCEPT ) ;
}


void	IrDAAPI	IrCOMM_Close( void )
{
}


int		IrDAAPI	IrCOMM_RequestLSAP( void )
{
#if 0	
	int		ret ;
	ret		= TTP_RequestLSAP() ;
	if( ret < 0 )
		return( 10 ) ;
	return( ret ) ;
#else
#if defined(NOTTP)
	return( IrLMP_RequestLSAP()) ;
#else	
	return( TTP_RequestLSAP()) ;
#endif	
#endif
}


int		IrDAAPI	IrCOMM_BindLSAP( int lsap, COMMCALLBACK IrDAFAR *ccb, int servicetype )
{
	int			ret ;
	COMMCONFIG	*cc ;


#if	defined(DMSG)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_BindLSAP():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " servicetype:", servicetype ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
#if 0	
	if( lsap >= COMMMAX )
		return( IrDAR_ACCEPT ) ;
#endif
	if( lsap >= COMMMAX )
		return( IrDAR_NORESOURCE ) ;
	cc		= &COMMConfig[lsap] ;
	if( cc->flags.used )
		return( IrDAR_INUSE ) ;


	if(( servicetype & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
#if !defined(IrCOMMNO3WIRERAW)
		LMCALLBACK	lcb ;
		IrDA_memset( &lcb, 0, sizeof( LMCALLBACK )) ;
		lcb.ConnectInd		= commCbLMConnectInd ;
		lcb.ConnectCnf		= commCbLMConnectCnf ;
		lcb.DataInd			= commCbLMDataInd ;
		lcb.DataCnf			= commCbLMDataCnf ;
		lcb.DisconnectInd	= commCbLMDisconnectInd ;
		ret		= IrLMP_BindLSAP( lsap, &lcb ) ;
		if( ret != IrDAR_ACCEPT )
			return( ret ) ;
#else	
		return( IrDAR_NOPROTOCOL ) ;
#endif	
#if !defined(NOTTP)
	} else {
		TTPCALLBACK	tcb ;
		IrDA_memset( &tcb, 0, sizeof( TTPCALLBACK )) ;
		tcb.ConnectInd		= commCbTTPConnectInd ;
		tcb.ConnectCnf		= commCbTTPConnectCnf ;
		tcb.DataInd			= commCbTTPDataInd ;
		tcb.DataCnf			= commCbTTPDataCnf ;
		tcb.DisconnectInd	= commCbTTPDisconnectInd ;
		ret		= TTP_BindLSAP( lsap, &tcb ) ;
		if( ret != IrDAR_ACCEPT )
			return( ret ) ;
#endif	
	}


	cc->ServiceType	= servicetype ;
	cc->lsap		= lsap ;
	IrDA_memset( &cc->flags, 0, sizeof( COMMFLAGS )) ;
	cc->flags.used		= TRUE ;
	cc->Status			= COMM_S_IDLE ;
	commFlushTxSdu( cc ) ;
	commFlushQueues( cc ) ;
	IrDA_memcpy( &cc->CallBack, ccb, sizeof( COMMCALLBACK )) ;


	commSetIASParameters() ;
	return( ret ) ;
}


void	IrDAAPI	IrCOMM_UnbindLSAP( int lsap )
{
	COMMCONFIG	*cc ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_UnbindLSAP():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	if( lsap >= COMMMAX )
		return ;


	cc		= &COMMConfig[lsap] ;
	if( !cc->flags.used )
		return ;


	IrDA_memset( &cc->flags, 0, sizeof( COMMFLAGS )) ;
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
#if !defined(IrCOMMNO3WIRERAW)
		IrLMP_UnbindLSAP( lsap ) ;
#endif	
#if !defined(NOTTP)
	} else {
		TTP_UnbindLSAP( lsap ) ;
#endif	
	}


	cc->flags.used	= FALSE ;
	commSetIASParameters() ;
}


int		IrDAAPI	IrCOMM_GetRealLSAP( int lsap )
{
	return( TTP_GetRealLSAP( lsap )) ;
}


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrCOMM_ConnectReq( int slsap, dword addr, int dlsap )
{
	byte		buf[4] ;
	int			ret ;
	TTPCONNDAT	tcd ;
	COMMCONFIG	*cc ;


	if( slsap >= COMMMAX )
		return( IrDAR_INVALID ) ;


	cc		= &COMMConfig[slsap] ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_ConnectReq():" ) ;
	_DPUTW( DL_COMMINFO, " COMMConfig[", slsap ) ;
	_DPUTW( DL_COMMINFO, "].Status:", COMMConfig[slsap].Status ) ;
	_DPUTW( DL_COMMINFO, ", ServiceType:", COMMConfig[slsap].ServiceType ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	if( cc->flags.connected )
		return( IrDAR_CONNECTING ) ;


	if( cc->Status != COMM_S_IDLE )
		return( IrDAR_INVALID ) ;


	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
#if !defined(IrCOMMNO3WIRERAW)
		ret		= IrLMP_ConnectReq( addr, slsap, dlsap, 0, 0 ) ;
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "COMM::IrCOMM_ConnectReq():" ) ;
		_DPUTD( DL_COMMINFO, " IrLMP_ConnectReq( ", addr ) ;
		_DPUTW( DL_COMMINFO, ", ", slsap ) ;
		_DPUTW( DL_COMMINFO, ", ", dlsap ) ;
		_DPUTW( DL_COMMINFO, ", 0, 0 )=", ret ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
		if( ret != IrDAR_ACCEPT )
			return( ret ) ;
#endif	
#if !defined(NOTTP)
	} else {
		commSetServiceType( buf, cc->ServiceType & IrCOMM_ST_TypeMask ) ;
		tcd.Data		= buf ;
		tcd.DataSize	= 4 ;
		ret		= TTP_ConnectReq( slsap, addr, dlsap, &tcd ) ;
		if( ret != IrDAR_ACCEPT )
			return( ret ) ;
#endif	
	}


	cc->Status	= COMM_S_WAITI ;
	cc->Address	= addr ;


	return( ret ) ;
}
#endif	


int		IrDAAPI	IrCOMM_DisconnectReq( int lsap )
{
	int			ret ;
	COMMCONFIG	*cc ;


	cc		= &COMMConfig[lsap] ;
	if( lsap > COMMMAX || !cc->flags.used )
		return( IrDAR_INVALID ) ;


	if( !cc->flags.connected )
		return( IrDAR_INVALID ) ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_DisconnectReq():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " Status:", cc->Status ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	switch( cc->Status ) {
	case	COMM_S_IDLE :
	case	COMM_S_WAITI :
	case	COMM_S_WAITR :
		return( IrDAR_INVALID ) ;
	case	COMM_S_CONN :
		if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
#if !defined(IrCOMMNO3WIRERAW)
			ret		= IrLMP_DisconnectReq( lsap, IrLMP_DR_UserRequest, 0, 0 ) ;
#endif	
#if !defined(NOTTP)
		} else {
			ret		= TTP_DisconnectReq( lsap, 0, 0 ) ;
#endif	
		}


		if( ret == IrDAR_ACCEPT ) {
			MiscFlushQueue( &cc->TxQueue ) ;
			commFlushTxSdu( cc ) ;
			cc->Status			= COMM_S_IDLE ;
			cc->flags.connected	= FALSE ;
			(*DisconnectPrvPtr)() ;
		}


		return( ret ) ;
	}


	return( IrDAR_INVALID ) ;
}


int		IrDAAPI	IrCOMM_SendData( int lsap, const void IrDAFAR *buf, uint bufsz )
{
	int			ret ;
	int			size ;
	COMMCONFIG	*cc ;
#if	defined(DMSG2)
	uint		wp1, wp2, rp1, rp2 ;
#endif	


#if defined(IrDA_COMMDATAREQPRB)
	IrDA_COMMDATAREQPRB( 0 ) ;
#endif	
	cc		= &COMMConfig[lsap] ;
	if( lsap >= COMMMAX || !cc->flags.used ) {
#if defined(IrDA_COMMDATAREQPRB)
		IrDA_COMMDATAREQPRB( 1 ) ;
#endif	
		return( IrDAR_INVALID ) ;
	}


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_SendData():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " bufsz:", bufsz ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	for( size=0 ; bufsz > 0 ;) {
#if	defined(DMSG2)
		wp1		= cc->TxQueue.WritePtr ;
		rp1		= cc->TxQueue.ReadPtr ;
#endif	
		ret		= MiscSetQueue( &cc->TxQueue, &((byte IrDAFAR*)buf)[size], bufsz ) ;
#if	defined(DMSG2)
		wp2		= cc->TxQueue.WritePtr ;
		rp2		= cc->TxQueue.ReadPtr ;
		if( wp1 != wp2 || rp1 != rp2 ) {
			_DPUTS( DL_COMMINFO, "COMM::IrCOMM_SendData():" ) ;
			_DPUTW( DL_COMMINFO, " rp:", rp1 ) ;
			_DPUTW( DL_COMMINFO, "->", rp2 ) ;
			_DPUTW( DL_COMMINFO, " wp:", wp1 ) ;
			_DPUTW( DL_COMMINFO, "->", wp2 ) ;
			_DPUTW( DL_COMMINFO, " QueueSize:", cc->TxQueue.DataSize ) ;
			_DPUTS( DL_COMMINFO, "\n" ) ;
		}
#endif	
		if( ret <= 0 )
			break ;
#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_SendData():" ) ;
	_DPUTW( DL_COMMINFO, " MiscSetQueue()=", ret ) ;
	_DPUTS( DL_COMMINFO, " data" ) ;
	_DPUTB( DL_COMMDAT, "=", ((byte IrDAFAR*)buf)[size] ) ;
	for( i=1 ; i<ret ; i++ )
		_DPUTB( DL_COMMDAT, " ", ((byte IrDAFAR*)buf)[size+i] ) ;
	_DPUTS( DL_COMMDAT, "\n" ) ;
}
#endif	
		size	+= ret ;
		bufsz	-= ret ;
	}


	commSendData( cc ) ;


	*TxSizePtr	+= (ulong)size ;


#if defined(IrDA_COMMDATAREQPRB)
	IrDA_COMMDATAREQPRB( 1 ) ;
#endif	
	return( size ) ;
}


int		IrDAAPI	IrCOMM_RecvData( int lsap, void IrDAFAR *buf, int bufsz )
{
	int			ret ;
	COMMCONFIG	*cc ;
#if	defined(DMSG2)
	uint	wp1, wp2, rp1, rp2 ;
#endif	


	cc		= &COMMConfig[lsap] ;
	if( lsap >= COMMMAX || !cc->flags.used )
		return( IrDAR_INVALID ) ;


	commSendData( cc ) ;
#if	defined(DMSG2)
	wp1		= cc->RxQueue.WritePtr ;
	rp1		= cc->RxQueue.ReadPtr ;
#endif	
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) != IrCOMM_ST_3WireRaw )
		commTTP_RecvData( lsap ) ;
	ret		= MiscGetQueue( buf, &cc->RxQueue, bufsz ) ;
#if defined(DMSG)
	if( ret > 0 ) {
		_DPUTS( DL_COMMINFO, "COMM::IrCOMM_RecvData():" ) ;
		_DPUTW( DL_COMMINFO, " MiscGetQueue():", ret ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
	}
#endif	
#if	defined(DMSG2)
	wp2		= cc->RxQueue.WritePtr ;
	rp2		= cc->RxQueue.ReadPtr ;
	if( wp1 != wp2 || rp1 != rp2 ) {
		_DPUTS( DL_COMMINFO, "COMM::IrCOMM_RecvData():" ) ;
		_DPUTW( DL_COMMINFO, " rp:", rp1 ) ;
		_DPUTW( DL_COMMINFO, "->", rp2 ) ;
		_DPUTW( DL_COMMINFO, " wp:", wp1 ) ;
		_DPUTW( DL_COMMINFO, "->", wp2 ) ;
		_DPUTW( DL_COMMINFO, " QueueSize:", cc->RxQueue.DataSize ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
	}
#endif	
#if	defined(DMSG2)
	if( ret ) {
		int		i ;
		_DPUTS( DL_COMMINFO, "COMM::IrCOMM_RecvData():" ) ;
		_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
		_DPUTW( DL_COMMINFO, " bufsz:", bufsz ) ;
		_DPUTW( DL_COMMINFO, " transfered-size:", ret ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;


		_DPUTS( DL_COMMDAT, "                       : Data" ) ;
		_DPUTB( DL_COMMDAT, ".", ((byte IrDAFAR*)buf)[0] ) ;
		for( i=1 ; i<ret ; i++ )
			_DPUTB( DL_COMMDAT, " ", ((byte IrDAFAR*)buf)[i] ) ;
		_DPUTS( DL_COMMDAT, "\n" ) ;
	}
#endif	


	if( ret > 0 ) {
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "COMM::IrCOMM_RecvData():" ) ;
		_DPUTW( DL_COMMINFO, " MiscGetFreeQueueSize()=", MiscGetFreeQueueSize( &cc->RxQueue )) ;
		_DPUTW( DL_COMMINFO, " MaxDataIndSize=", cc->MaxDataIndSize ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
#if !defined(IrCOMMNO3WIRERAW)
		if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
			if( cc->flags.localBusy ) {
				if( MiscGetFreeQueueSize( &cc->RxQueue ) >= cc->MaxDataIndSize ) {


					IrLAP_LocalBusy( OFF ) ;
					cc->flags.localBusy	= FALSE ;
				}
			}
		}
#endif	


		*RxSizePtr	+= (ulong)ret ;
	}


	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) != IrCOMM_ST_3WireRaw )
		commTTP_RecvData( lsap ) ;


	return( ret ) ;
}


int		IrDAAPI	IrCOMM_ControlReq( int lsap, const void IrDAFAR *control, int controlsz )
{
	byte IrDAFAR	*ptr ;
	int			qsz ;
	int			i ;
	int			pl ;
	COMMCONFIG	*cc ;


	cc		= &COMMConfig[lsap] ;
	if( lsap >= COMMMAX || !cc->flags.used )
		return( IrDAR_INVALID ) ;
	if( !cc->flags.connected )
		return( IrDAR_INVALID ) ;
#if !defined(IrCOMMNO3WIRERAW)
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw )
		return( IrDAR_INVALID ) ;
#endif	


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_ControlReq():" ) ;
	_DPUTW( DL_COMMINFO, " controlsz:", controlsz ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	qsz		= MiscGetFreeQueueSize( &cc->CtlQueue ) ;
	ptr		= (byte IrDAFAR*)control ;
	for( i=0 ; i<controlsz && i<qsz ; i+=pl+2 ) {


		pl	= ptr[i+1] ;
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "                         :" ) ;
		_DPUTW( DL_COMMINFO, " i:", i ) ;
		_DPUTW( DL_COMMINFO, " pl:", pl ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	}
	if( i < 1 )
		return( IrDAR_INVALID ) ;


	qsz	= MiscSetQueue( &cc->CtlQueue, control, i ) ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::IrCOMM_ControlReq():" ) ;
	_DPUTW( DL_COMMINFO, " TxSduSize:", cc->TxSduSize ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	commSendData( cc ) ;
	return( qsz ) ;
}


int		IrDAAPI	IrCOMM_QueryConnectUserData( int lsap, byte IrDAFAR * IrDAFAR *ptr )
{
	COMMCONFIG	*cc ;


	if( lsap < 0 || COMMMAX <= lsap )
		return( IrDAR_EPARM ) ;
	cc	= &COMMConfig[lsap] ;
	if( !cc->flags.used )
		return( IrDAR_EPARM ) ;


	if( !cc->flags.inConnCallBack )
		return( IrDAR_INVALID ) ;


	*ptr	= cc->connDatPtr ;
	return( cc->connDatLen ) ;
}


int		IrDAAPI	IrCOMM_QueryTxQueue( int lsap )
{
	if( lsap >= COMMMAX || !COMMConfig[lsap].flags.used )
		return( IrDAR_INVALID ) ;


	commSendData( &COMMConfig[lsap] ) ;
	return( MiscGetFreeQueueSize( &COMMConfig[lsap].TxQueue )) ;
}


int		IrDAAPI	IrCOMM_QueryRxQueue( int lsap )
{
	if( lsap >= COMMMAX || !COMMConfig[lsap].flags.used )
		return( IrDAR_INVALID ) ;


	commSendData( &COMMConfig[lsap] ) ;
	return( MiscGetFreeQueueSize( &COMMConfig[lsap].RxQueue )) ;
}


int		IrDAAPI	IrCOMM_QueryTxData( int lsap )
{
	int		size ;
#if defined(NOTTP)
	int		ret ;
#endif	


	if( lsap >= COMMMAX || !COMMConfig[lsap].flags.used )
		return( IrDAR_INVALID ) ;


	commSendData( &COMMConfig[lsap] ) ;
	size	= MiscGetUsedQueueSize( &COMMConfig[lsap].TxQueue ) ;


#if 0
	if(( servicetype & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
		if(( ret = IrLMP_QueryTxData( lsap )) >= 0 )
			size	+= ret ;
#if !defined(NOTTP)
	} else {
		if(( ret = TTP_QueryTxData( lsap )) >= 0 )
			size	+= ret ;
#endif	
	}
#endif


	return( size ) ;
}


int		IrDAAPI	IrCOMM_QueryRxData( int lsap )
{
	int			size ;
	COMMCONFIG	*cc ;
#if defined(NOTTP)
	int			ret ;
#endif	


	cc		= &COMMConfig[lsap] ;
	if( lsap >= COMMMAX || !cc->flags.used )
		return( IrDAR_INVALID ) ;


	commSendData( cc ) ;
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) != IrCOMM_ST_3WireRaw )
		commTTP_RecvData( lsap ) ;
	size	= MiscGetUsedQueueSize( &cc->RxQueue ) ;
#if 0
	if(( servicetype & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
		if(( ret = IrLMP_QueryRxData( lsap )) >= 0 )
			size	+= ret ;
#if !defined(NOTTP)
	} else {
		if(( ret = TTP_QueryRxData( lsap )) >= 0 )
			size	+= ret ;
#endif	
	}
#endif
	return( size ) ;
}


void	IrDAAPI	IrCOMM_SetConnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		ConnectPrvPtr	= func ;
	else
		ConnectPrvPtr	= commDmyConnectPrv ;
}


void	IrDAAPI	IrCOMM_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		DisconnectPrvPtr	= func ;
	else
		DisconnectPrvPtr	= commDmyDisconnectPrv ;
}


void	IrDAAPI	IrCOMM_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp )
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
#if !defined(NOTTP)


static	int		IrDACB	commCbTTPConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd )
{
	byte		buf[4] ;
	int			ret ;
	COMMCONFIG	*cc ;


	cc		= &COMMConfig[slsap] ;
#if	defined(DMSG2)
{	int		i ;
	_DPUTS( DL_COMMINFO, "COMM::commCbTTPConnectInd():" ) ;
	if( lcd->DataSize > 0 ) {
		_DPUTB( DL_COMMINFO, " data=", lcd->Data[0] ) ;
		for( i=1 ; i<lcd->DataSize ; i++ )
			_DPUTB( DL_COMMINFO, ".", lcd->Data[i] ) ;
	}
	_DPUTS( DL_COMMINFO, "\n" ) ;
}
#endif	
	cc->connDatPtr		= lcd->Data ;
	cc->connDatLen		= lcd->DataSize ;
	commSetServiceType( buf, cc->ServiceType & IrCOMM_ST_TypeMask ) ;
	lcd->Data		= buf ;
	lcd->DataSize	= 4 ;


	cc->Status			= COMM_S_WAITR ;
	cc->MaxDataIndSize	= (word)TTP_GetMaxRxSize( slsap ) ;
	cc->MaxTxSize		= TTP_GetMaxTxSize( slsap ) ;
	if( cc->MaxTxSize > COMMTXSDUSZ )
		cc->MaxTxSize	= COMMTXSDUSZ ;
	MiscFlushQueue( &cc->RxQueue ) ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbTTPConnectInd(): slsap=%d, MaxTxSize=%d, COMMTXSDUSZ=%d\n", slsap, cc->MaxTxSize, COMMTXSDUSZ ) ;
	_DPUTS( DL_COMMINFO, "                         : TTP_GetMaxTxSize( %d )=%d\n", slsap, TTP_GetMaxTxSize( slsap )) ;
#endif	


	ret		= commCallConnectInd( slsap, addr, dlsap, cc->PeerServiceType ) ;
	if( ret == IrDA_CONNECTEN ) {
		cc->Status			= COMM_S_CONN ;
		cc->flags.connected	= TRUE ;
	} else {
		cc->Status			= COMM_S_IDLE ;
	}


	return( ret ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(NOTTP)


static	void	IrDACB	commCbTTPConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status )
{
#if defined(IrDAPRIMARY)
	COMMCONFIG	*cc ;


	UNREFERENCE( maxsdu ) ;


	cc			= &COMMConfig[slsap] ;
	if( status == IrDAR_CONNECTING ) {
		cc->connDatPtr		= lcd->Data ;
		cc->connDatLen		= lcd->DataSize ;


		cc->Status			= COMM_S_CONN ;
		cc->flags.connected	= TRUE ;
		cc->MaxDataIndSize	= (word)TTP_GetMaxRxSize( slsap ) ;
		cc->MaxTxSize		= TTP_GetMaxTxSize( slsap ) ;
		if( cc->MaxTxSize > COMMTXSDUSZ )
			cc->MaxTxSize	= COMMTXSDUSZ ;
		MiscFlushQueue( &cc->RxQueue ) ;
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "COMM::commCbTTPConnectCnf():" ) ;
		_DPUTW( DL_COMMINFO, " slsap:", slsap ) ;
		_DPUTD( DL_COMMINFO, " addr:", addr ) ;
		_DPUTW( DL_COMMINFO, " dlsap:", dlsap ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	} else {
		cc->Status			= COMM_S_IDLE ;
	}


	commCallConnectCnf( slsap, addr, dlsap, status ) ;
#else	
	UNREFERENCE( slsap ) ;
	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;
	UNREFERENCE( maxsdu ) ;
	UNREFERENCE( status ) ;
#endif	
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	commCbTTPDisconnectInd( int lsap, int status )
{
	COMMCONFIG	*cc ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbTTPDisconnectInd():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " status:", status ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	


	cc		= &COMMConfig[lsap] ;
	switch( cc->Status ) {
	case	COMM_S_IDLE :
		break ;
	case	COMM_S_WAITI :
	case	COMM_S_WAITR :
	case	COMM_S_CONN :
		cc->Status			= COMM_S_IDLE ;
		cc->flags.connected	= FALSE ;
		MiscFlushQueue( &cc->TxQueue ) ;
		commFlushTxSdu( cc ) ;
		commCallDisconnectInd( lsap, status ) ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(NOTTP)


static	void	IrDACB	commCbTTPDataInd( int lsap )
{
	commTTP_RecvData( lsap ) ;
	commCallDataInd( lsap ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(NOTTP)


static	void	IrDACB	commCbTTPDataCnf( int lsap )
{
	COMMCONFIG	*cc ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbTTPDataCnf" ) ;
	_DPUTW( DL_COMMINFO, " TxSdu is ", COMMConfig[lsap].flags.txsduused ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	


	cc		= &COMMConfig[lsap] ;
	cc->flags.acceptDataCnf		= TRUE ;
	commSendData( cc ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrCOMMNO3WIRERAW)


static	int		IrDACB	commCbLMConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd )
{
	int			ret ;
	COMMCONFIG	*cc ;


	cc				= &COMMConfig[slsap] ;
	cc->connDatPtr		= lcd->Data ;
	cc->connDatLen		= lcd->DataSize ;
	lcd->Data		= 0 ;
	lcd->DataSize	= 0 ;


	cc->Status			= COMM_S_WAITR ;
	cc->MaxDataIndSize	= IrLMP_GetMaxRxSize( slsap ) ;
	cc->MaxTxSize		= IrLMP_GetMaxTxSize( slsap ) ;
	if( cc->MaxTxSize > COMMTXSDUSZ )
		cc->MaxTxSize	= COMMTXSDUSZ ;
	MiscFlushQueue( &cc->RxQueue ) ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbLMConnectInd():" ) ;
	_DPUTW( DL_COMMINFO, " slsap:", slsap ) ;
	_DPUTW( DL_COMMINFO, " dlsap:", dlsap ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	


	ret		= commCallConnectInd( slsap, addr, dlsap, cc->PeerServiceType ) ;
	if( ret == IrDA_CONNECTEN ) {
		cc->Status			= COMM_S_CONN ;
		cc->flags.connected	= TRUE ;
	} else {
		cc->Status			= COMM_S_IDLE ;
	}


	return( ret ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrCOMMNO3WIRERAW)


static	void	IrDACB	commCbLMConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status )
{
#if defined(IrDAPRIMARY)
	COMMCONFIG	*cc ;
	UNREFERENCE( lcd ) ;


	cc					= &COMMConfig[slsap] ;
	if( status == IrDAR_CONNECTING ) {
		cc->connDatPtr		= lcd->Data ;
		cc->connDatLen		= lcd->DataSize ;


		cc->Status			= COMM_S_CONN ;
		cc->flags.connected	= TRUE ;
		cc->MaxDataIndSize	= IrLMP_GetMaxRxSize( slsap ) ;
		cc->MaxTxSize		= IrLMP_GetMaxTxSize( slsap ) ;
		if( cc->MaxTxSize > COMMTXSDUSZ )
			cc->MaxTxSize	= COMMTXSDUSZ ;
		MiscFlushQueue( &cc->RxQueue ) ;
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "COMM::commCbTTPConnectCnf():" ) ;
		_DPUTW( DL_COMMINFO, " slsap:", slsap ) ;
		_DPUTW( DL_COMMINFO, " MaxTxSize:", cc->MaxTxSize ) ;
		_DPUTW( DL_COMMINFO, " COMMTXSDUSZ:", COMMTXSDUSZ ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
		_DPUTS( DL_COMMINFO, "                         :" ) ;
		_DPUTW( DL_COMMINFO, " IrLMP_GetMaxTxSize( ", slsap ) ;
		_DPUTW( DL_COMMINFO, "  )=", IrLMP_GetMaxTxSize( slsap )) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	} else {
		cc->Status			= COMM_S_IDLE ;
	}


	commCallConnectCnf( slsap, addr, dlsap, status ) ;
#else	
	UNREFERENCE( slsap ) ;
	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;
	UNREFERENCE( status ) ;
#endif	
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrCOMMNO3WIRERAW)


static	void	IrDACB	commCbLMDisconnectInd( int lsap, int reason, const void IrDAFAR *data, int datasz )
{
	COMMCONFIG	*cc ;


	UNREFERENCE( data ) ;
	UNREFERENCE( datasz ) ;


	cc		= &COMMConfig[lsap] ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbLMDisconnectInd():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " status:", status ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	


	if( lsap < 0 || lsap >= COMMMAX )
		return ;
	if( !cc->flags.used )
		return ;


	switch( cc->Status ) {
	case	COMM_S_IDLE :
		break ;
	case	COMM_S_WAITI :
	case	COMM_S_WAITR :
	case	COMM_S_CONN :
		cc->Status			= COMM_S_IDLE ;
		cc->flags.connected	= FALSE ;
		MiscFlushQueue( &cc->TxQueue ) ;
		commFlushTxSdu( cc ) ;
		commCallDisconnectInd( lsap, reason ) ;
		break ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrCOMMNO3WIRERAW)


static	void	IrDACB	commCbLMDataInd( int lsap, const void IrDAFAR *data, uint datasz )
{
	if( datasz < 1 )
		return ;


#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCbLMDataInd():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " datasz:", datasz ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
	{	int		i ;
		_DPUTS( DL_COMMDAT, "                        : dat=" ) ;
		for( i=0 ; i<datasz ; i++ )
			_DPUTB( DL_COMMDAT, " ", ((byte IrDAFAR*)data)[i] ) ;
		_DPUTS( DL_COMMDAT, "\n" ) ;
	}
#endif	
#if	defined(SEQCHK)
{	uint	fsz ;
	if(( fsz = MiscGetFreeQueueSize( &COMMConfig[lsap].RxQueue )) < datasz ) {
		irda_rxrwp	= COMMConfig[lsap].RxQueue.ReadPtr ;
		irda_rxwrp	= COMMConfig[lsap].RxQueue.WritePtr ;
		irda_errorInd( 0xFF01, (byte IrDAFAR*)fsz, datasz ) ;
	}
}
	CheckDataSequence( 2, (byte IrDAFAR*)data, datasz ) ;
	if( MiscSetQueue( &COMMConfig[lsap].RxQueue, data, datasz ) != datasz )
		irda_errorInd( 0xFF02, 0, 0 ) ;
	CheckQueue( 0x0103, &COMMConfig[lsap].RxQueue ) ;
#else	
	MiscSetQueue( &COMMConfig[lsap].RxQueue, data, datasz ) ;
#endif	
	if( !COMMConfig[lsap].flags.localBusy ) {
		if( MiscGetFreeQueueSize( &COMMConfig[lsap].RxQueue ) < COMMConfig[lsap].MaxDataIndSize ) {
			IrLAP_LocalBusy( ON ) ;
			COMMConfig[lsap].flags.localBusy	= TRUE ;
		}
	}


#if	defined(SEQCHK)
	CheckQueue( 0x0104, &COMMConfig[lsap].RxQueue ) ;
#endif	
	commCallDataInd( lsap ) ;
#if	defined(SEQCHK)
	CheckQueue( 0x0105, &COMMConfig[lsap].RxQueue ) ;
#endif	
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(IrCOMMNO3WIRERAW)


static	void	IrDACB	commCbLMDataCnf( int lsap )
{
	COMMCONFIG	*cc ;


	cc		= &COMMConfig[lsap] ;
	cc->flags.acceptDataCnf		= TRUE ;
	commSendData( cc ) ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	commDmyConnectPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	commDmyDisconnectPrv( void )
{
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			commFlushQueues( COMMCONFIG *cc )
{
	MiscFlushQueue( &cc->TxQueue ) ;
	MiscFlushQueue( &cc->RxQueue ) ;
#if defined(CTLQUEUESZ)
	MiscFlushQueue( &cc->CtlQueue ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			commFlushTxSdu( COMMCONFIG *cc )
{
	cc->TxSduSize		= 0 ;
	cc->flags.txsduused	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		commCallConnectInd( int slsap, dword addr, int dsel, int servicetype )
{
	(*ConnectPrvPtr)() ;


	COMMConfig[slsap].flags.inConnCallBack	= TRUE ;
	if( COMMConfig[slsap].CallBack.ConnectInd )
		return((*COMMConfig[slsap].CallBack.ConnectInd)( slsap, addr, dsel, servicetype )) ;
	COMMConfig[slsap].flags.inConnCallBack	= FALSE ;


	return( IrDA_CONNECTDI ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if defined(IrDAPRIMARY)


static	void	commCallConnectCnf( int slsap, dword addr, int dlsap, int status )
{
	(*ConnectPrvPtr)() ;


	COMMConfig[slsap].flags.inConnCallBack	= TRUE ;
	if( COMMConfig[slsap].CallBack.ConnectCnf )
		(*COMMConfig[slsap].CallBack.ConnectCnf)( slsap, addr, dlsap, status ) ;
	COMMConfig[slsap].flags.inConnCallBack	= FALSE ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	commCallDisconnectInd( int lsap, int status )
{
	if( COMMConfig[lsap].CallBack.DisconnectInd )
		(*COMMConfig[lsap].CallBack.DisconnectInd)( lsap, status ) ;


	(*DisconnectPrvPtr)() ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	commCallDataInd( int lsap )
{
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commCallDataInd():" ) ;
	_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
	_DPUTW( DL_COMMINFO, " dlen:", MiscGetUsedQueueSize( &COMMConfig[lsap].RxQueue )) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
#if defined(IrDA_COMDATAINDPRB)
		IrDA_COMDATAINDPRB( 0 ) ;
#endif	
	if( COMMConfig[lsap].CallBack.DataInd )
		(*COMMConfig[lsap].CallBack.DataInd)( lsap, MiscGetUsedQueueSize( &COMMConfig[lsap].RxQueue )) ;
#if defined(IrDA_COMDATAINDPRB)
		IrDA_COMDATAINDPRB( 1 ) ;
#endif	
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	commCallCtlInd( int lsap, const void IrDAFAR *ctl, int ctlsz )
{
	if( COMMConfig[lsap].CallBack.ControlInd )
		(*COMMConfig[lsap].CallBack.ControlInd)( lsap, ctl, ctlsz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	commTTP_RecvData( int lsap )
{
	COMMCONFIG	*cc ;
	uint	len ;
	int		ctlsz, datsz ;
	const byte	IrDAFAR *ptr ;
	const byte	IrDAFAR *ctlp ;
	const byte	IrDAFAR *datp ;


	cc		= &COMMConfig[lsap] ;
#if	defined(DMSG2)
	_DPUTS( DL_COMMINFO, "COMM::commTTP_RecvData():" ) ;
	_DPUTB( DL_COMMINFO, " lsap:", lsap ) ;
	if( cc->flags.inRecvData )
		_DPUTS( DL_COMMINFO, " RECVLOCK!!\n" ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	if( cc->flags.inRecvData )
		return ;
	cc->flags.inRecvData	= TRUE ;


	for(;;) {
		if(( len = TTP_GetRxSize( lsap )) == 0 ) {
#if	defined(DMSG2)
		_DPUTS( DL_COMMINFO, "COMM::commTTP_RecvData():" ) ;
		_DPUTS( DL_COMMINFO, " TTP_GetRxSize() is 0" ) ;
		_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
			break ;
		}


		if( len > MiscGetFreeQueueSize( &cc->RxQueue )) {
#if	defined(DMSG)
			_DPUTS( DL_COMMINFO, "COMM::commTTP_RecvData():" ) ;
			_DPUTW( DL_COMMINFO, " MiscGetFreeQueueSize() is under ", len ) ;
			_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
			break ;
		}
		ptr		= TTP_GetDataPtr( lsap ) ;
		if( len > 1 ) {
			ctlsz	= *ptr ;
			datsz	= len - 1 - ctlsz ;
			ctlp	= &ptr[1] ;
			datp	= &ptr[1+ctlsz] ;


#if	defined(DMSG)
			_DPUTS( DL_COMMINFO, "COMM::commTTP_RecvData():" ) ;
			_DPUTW( DL_COMMINFO, " lsap:", lsap ) ;
			_DPUTW( DL_COMMINFO, " len:", len ) ;
			_DPUTW( DL_COMMINFO, " ctlsz:", ctlsz ) ;
			_DPUTW( DL_COMMINFO, " datsz:", datsz ) ;
			_DPUTS( DL_COMMINFO, "\n" ) ;
#if	defined(DMSG)
#endif	
			{	int		i ;
				if( ctlsz > 0 ) {
					_DPUTS( DL_COMMDAT, "                      :" ) ;
					_DPUTB( DL_COMMDAT, " ctl=", ctlp[0] ) ;
					for( i=1 ; i<ctlsz ; i++ )
						_DPUTB( DL_COMMDAT, ".", ctlp[i] ) ;
					_DPUTS( DL_COMMDAT, "\n" ) ;
				}
				if( datsz > 0 ) {
					_DPUTS( DL_COMMDAT, "                      :" ) ;
					_DPUTB( DL_COMMDAT, " dat=", datp[0] ) ;
					for( i=1 ; i<datsz ; i++ )
						_DPUTB( DL_COMMDAT, ".", datp[i] ) ;
					_DPUTS( DL_COMMDAT, "\n" ) ;
				}
			}
#endif	
			if( ctlsz > 0 )
				commCallCtlInd( lsap, ctlp, ctlsz ) ;


			if( datsz > 0 ) {
				MiscSetQueue( &cc->RxQueue, datp, datsz ) ;
				if( cc->MaxDataIndSize == 0 )
					cc->MaxDataIndSize	= (word)TTP_GetMaxRxSize( lsap ) ;
			}
		}


		TTP_ReleaseRxQueue( lsap ) ;
	}


	cc->flags.inRecvData	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	int		commSendData( COMMCONFIG *cc )
{
	int		i ;


	for( i=0 ;; i++ ) {
		commSetTxSdu( cc ) ;
		commSendTxSdu( cc ) ;


		if( cc->flags.inSetTxSdu || cc->flags.inSendTxSdu ||
			cc->flags.txsduused || cc->TxSduSize > 0 ||
			cc->TxQueue.ReadPtr == cc->TxQueue.WritePtr )
			break ;
	}


	return( i ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void	commSetTxSdu( COMMCONFIG *cc )
{
	uint		dlen ;
	uint		minlen ;
	uint		ctlsz ;


	if( cc->flags.txsduused )
		return ;


	if( cc->flags.inSetTxSdu )
		return ;


	cc->flags.inSetTxSdu	= TRUE ;
	if( cc->TxSduSize > 0 ) {
		cc->flags.inSetTxSdu	= FALSE ;
		return ;
	}


#if defined(CTLQUEUESZ)
	if( cc->TxQueue.ReadPtr == cc->TxQueue.WritePtr ) {
#if !defined(IrCOMMNO3WIRERAW)
		if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ||
			 cc->CtlQueue.ReadPtr == cc->CtlQueue.WritePtr ) {
			cc->flags.inSetTxSdu	= FALSE ;
			return ;
		}
#else	
		if(  cc->CtlQueue.ReadPtr == cc->CtlQueue.WritePtr ) {
			cc->flags.inSetTxSdu	= FALSE ;
			return ;
		}
#endif	
	}
#else	
	if( cc->TxQueue.ReadPtr == cc->TxQueue.WritePtr ) {
#if !defined(IrCOMMNO3WIRERAW)
		if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ||
			 cc->CntlDataSize == 0 ) {
			cc->flags.inSetTxSdu	= FALSE ;
			return ;
		}
#else	
		if( cc->CntlDataSize == 0 ) {
			cc->flags.inSetTxSdu	= FALSE ;
			return ;
		}
#endif	
	}
#endif	


	cc->flags.txsduused	= TRUE ;


#if !defined(IrCOMMNO3WIRERAW)
	dlen		= 0 ;
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) != IrCOMM_ST_3WireRaw ) {
#endif	
		dlen			= 1 ;
#if defined(CTLQUEUESZ)
		for( ; cc->CtlQueue.ReadPtr != cc->CtlQueue.WritePtr ; ) {
													
			MiscSenseQueue( &cc->TxSdu[dlen], &cc->CtlQueue, 2 ) ;
			ctlsz	= cc->TxSdu[dlen+1] ;
			if( dlen + 2 + ctlsz > cc->MaxTxSize )
				break ;
			MiscSkipReadQueue( &cc->CtlQueue, 2 ) ;
			MiscGetQueue( &cc->TxSdu[dlen+2], &cc->CtlQueue, ctlsz ) ;
			dlen	+= ctlsz + 2 ;
		}
#else	
		IrDA_memcpy( &cc->TxSdu[1], cc->CntlData, cc->CntlDataSize ) ;
		dlen	+= cc->CntlDataSize ;
#endif	
		cc->TxSdu[0]	= (byte)( dlen - 1 ) ;		
#if !defined(IrCOMMNO3WIRERAW)
	}
#endif	


	
	dlen	+= MiscGetQueue( &cc->TxSdu[dlen], &cc->TxQueue, cc->MaxTxSize - dlen ) ;
	
#if !defined(IrCOMMNO3WIRERAW)
	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw )
		minlen		= 0 ;
	else
#endif	
		minlen		= 1 ;
	if( dlen == minlen ) {
		cc->flags.txsduused		= FALSE ;
		cc->flags.inSetTxSdu	= FALSE ;
		return ;
	}
	cc->TxSduSize			= dlen ;
	cc->flags.inSetTxSdu	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	commSendTxSdu( COMMCONFIG *cc )
{
	if( !cc->flags.txsduused || cc->TxSduSize == 0 )
		return ;


	if( cc->flags.inSendTxSdu )
		return ;
	cc->flags.inSendTxSdu	= TRUE ;


	if( !cc->flags.txsduused ) {
		cc->flags.inSendTxSdu	= FALSE ;
		return ;
	}


	if(( cc->ServiceType & IrCOMM_ST_TypeMask ) == IrCOMM_ST_3WireRaw ) {
#if !defined(IrCOMMNO3WIRERAW)
		if( IrLMP_DataReq( cc->lsap, cc->TxSdu, cc->TxSduSize ) == IrDAR_ACCEPT ) {
			cc->TxSduSize		= 0 ;
			cc->flags.txsduused	= FALSE ;
		}
#endif	
	} else {
#if !defined(NOTTP)
		if( TTP_DataReq( cc->lsap, cc->TxSdu, cc->TxSduSize ) == IrDAR_ACCEPT ) {
			cc->TxSduSize		= 0 ;
			cc->flags.txsduused	= FALSE ;
		}
#endif	
	}


	cc->flags.inSendTxSdu	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)
#if !defined(NOTTP)


static	void	commSetServiceType( byte IrDAFAR *p, int st )
{
#if defined(DMSG)
	_DPUTS( DL_COMMINFO, "COMM:commSetServiceType():" ) ;
	_DPUTW( DL_COMMINFO, " st:", (word)st ) ;
	_DPUTS( DL_COMMINFO, "\n" ) ;
#endif	
	*p++			= 3 ;							
	*p++			= 0x00 ;						
	*p++			= 1 ;							
	switch( st ) {									
	case	IrCOMM_ST_3Wire :	*p	= 2 ;	break ;
	case	IrCOMM_ST_9Wire :	*p	= 4 ;	break ;
	case	IrCOMM_ST_Centro :	*p	= 8 ;	break ;
	default :					*p	= 0 ;
	}
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void	commSetIASParameters( void )
{
	int		i ;
	int		classid, attrid ;
	byte	buf[9] ;
	byte IrDAFAR	*bufp ;
	uint	st, pt ;
	uint	stmask ;
	COMMCONFIG	*cc ;


	st		= 0 ;
	pt		= 0 ;									
	for( i=0 ; i<COMMMAX ; i++ ) {
		cc			= &COMMConfig[i] ;
		if( !cc->flags.used )
			continue ;
		stmask		= cc->ServiceType & IrCOMM_ST_TypeMask ;
		switch( stmask ) {
#if !defined(IrCOMMNO3WIRERAW)
		case	IrCOMM_ST_3WireRaw :	st	|= 0x01 ;	pt	|= 2 ;	break ;
#endif	
		case	IrCOMM_ST_3Wire :		st	|= 0x02 ;	pt	|= 1 ;	break ;
		case	IrCOMM_ST_9Wire :		st	|= 0x04 ;	pt	|= 1 ;	break ;
		case	IrCOMM_ST_Centro :		st	|= 0x08 ;	pt	|= 1 ;	break ;
		}
	}


	if( st == 0x01 ) {
		IrLMP_ResetServiceHint( IrLMP_SH_IrCOMM ) ;
	} else if( st ) {
		IrLMP_SetServiceHint( IrLMP_SH_IrCOMM ) ;


		classid	= IrLMP_AddClass( "IrDA:IrCOMM", 11 ) ;
		attrid	= IrLMP_AddAttribute( classid, "Parameters", 10 ) ;
		bufp	= buf ;
		*bufp++	= 0x00 ;						
		*bufp++	= 0x01 ;						
		*bufp++	= (byte)st ;					
		*bufp++	= 0x01 ;						
		*bufp++	= 0x01 ;						
		*bufp++	= (byte)pt ;					


#if defined(DMSG2)
{	int	i ;
	_DPUTS( DL_LMPINFO, "COMM::commSetIASParameters():" ) ;
	_DPUTD( DL_LMPINFO, " buf:", (dword)((byte IrDAFAR*)buf )) ;
	_DPUTD( DL_LMPINFO, " bufp:", (dword)((byte IrDAFAR*)bufp )) ;
	_DPUTB( DL_LMPINFO, " value:", buf[0] ) ;
	for( i=1 ; i<6 ; i++ )
		_DPUTB( DL_LMPINFO, ".", buf[i] ) ;
	_DPUTS( DL_LMPINFO, "\n" ) ;
}
#endif	
		IrLMP_SetOctetAttribute( attrid, buf, 6 ) ;
	}
}
#endif	


#endif	
