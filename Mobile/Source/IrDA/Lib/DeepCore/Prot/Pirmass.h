


#if !defined(__plrmass_h__)
#define	__plrmass_h__


#include	"irda.h"
#include	"irmass.h"
#include	"tinytp.h"
#include	"okmisc.h"


#define		MAXSECTORSIZE	2048
#define		SECTORSIZE		2048


#define		MASSM_Used			0x0001
#define		MASSM_Connected		0x0002
#define		MASSM_RxRecvLock	0x0004
#define		MASSM_WaitHeader	0x0008


#define		MASS_CMD		0
#define		MASS_UN			1
#define		MASS_READSEC	2
#define		MASS_READCNT	6


#define		MASS_READSTAT	8
#define		MASS_READDATA	9


#define		MASS_CMDB		0x80
#define		MASS_FINALB		0x40
#define		MASS_CMDMASK	0x3F


#define		CMD_INIT		0x00
#define		CMD_READ		0x01
#define		CMD_MDACHK		0x02


typedef	struct	{
	volatile uint	Mode ;
	volatile uint	State ;
	int				tsap ;


	uint			MaxTxSize ;						
	uint			MaxDataIndSize ;				


	MASSCALLBACK	CallBack ;


	byte	IrDAFAR	*RxBufPtr ;
	uint			RxReqSize ;
	uint			RxReqCount ;


	dword			WaitSector ;
	word			WaitCount ;


	byte			TxBuf[MAXSECTORSIZE] ;
	const byte IrDAFAR *TxBufPtr ;
	uint			TxRspSize ;
}	MASSCONFIG ;


static	int		CallConnectInd( int smsap, dword addr, int dlsap ) ;
static	void	CallConnectCnf( int smsap, dword addr, int dmsap, int status ) ;
static	void	CallDisconnectInd( int msap, int status ) ;
static	void	CallMediaChkInd( int msap ) ;
static	void	CallMediaChkCnf( int msap, int status ) ;
static	void	CallReadInd( int msap, dword secno, word count ) ;
static	void	CallReadCnf( int msap, dword secno, word count, int status ) ;
static	void	CallTxCnf( int msap ) ;
static	int		GetMSAP( int tsap ) ;
static	int		SendDataReq( int msap, dword secno, word count ) ;
static	int		SendFirstDataRsp( int msap, dword secno, word count, const void IrDAFAR *buf, uint status ) ;
static	void	RecvReadRsp( int msap, const byte IrDAFAR *buf, uint bufsz ) ;
static	void	TTP_RecvData( int msap ) ;


static	int		IrDACB	CBConnectInd( int stsap, dword addr, int dtsap, TTPCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	CBConnectCnf( int smsap, dword addr, int dmsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status ) ;
static	void	IrDACB	CBDisconnectInd( int tsap, int status ) ;
static	void	IrDACB	CBDataInd( int tsap ) ;
static	void	IrDACB	CBDataCnf( int tsap ) ;


static	MASSCONFIG		MASSConfig[MASSMAX] ;


int		IrDAAPI	IrMASS_Open( void )
{
	int		i ;


	for( i=0 ; i<MASSMAX ; i++ )
		MASSConfig[i].Mode	= 0 ;


	return( IrDAR_ACCEPT ) ;
}


void	IrDAAPI	IrMASS_Close( void )
{
}


int		IrDAAPI	IrMASS_RequestLSAP( void )
{
	int		i ;
	int		ret ;


	for( i=0 ; i<MASSMAX ; i++ )
		if(( MASSConfig[i].Mode & MASSM_Used ) == 0 )
			break ;


	if( i >= MASSMAX )
		return( IrDAR_NORESOURCE ) ;


	if(( ret = TTP_RequestLSAP()) < 0 )
		return( ret ) ;


	return( i ) ;
}


int		IrDAAPI	IrMASS_BindLSAP( int msap, MASSCALLBACK IrDAFAR *mcb )
{
	int			ret ;
	TTPCALLBACK	tcb ;


	if( msap < 0 || MASSMAX <= msap )
		return( IrDAR_NORESOURCE ) ;
	if( MASSConfig[msap].Mode & MASSM_Used )
		return( IrDAR_INUSE ) ;


	MASSConfig[msap].tsap	= TTP_RequestLSAP() ;
	if( MASSConfig[msap].tsap < 0 )
		return( MASSConfig[msap].tsap ) ;


	IrDA_memset( &tcb, 0, sizeof( TTPCALLBACK )) ;
	tcb.ConnectInd		= CBConnectInd ;
	tcb.ConnectCnf		= CBConnectCnf ;
	tcb.DataInd			= CBDataInd ;
	tcb.DataCnf			= CBDataCnf ;
	tcb.DisconnectInd	= CBDisconnectInd ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::IrMASS_BindLSAP():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " tsap:", MASSConfig[msap].tsap ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	ret		= TTP_BindLSAP( MASSConfig[msap].tsap, &tcb ) ;
	if( ret != IrDAR_ACCEPT )
		return( ret ) ;


	MASSConfig[msap].Mode		= MASSM_Used ;
	MASSConfig[msap].RxBufPtr	= 0 ;
	IrDA_memcpy( &MASSConfig[msap].CallBack, mcb, sizeof( MASSCALLBACK )) ;


	return( ret ) ;
}


void	IrDAAPI	IrMASS_UnbindLSAP( int msap )
{
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::IrMASS_UnbindLSAP():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " Mode:", MASSConfig[msap].Mode ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( msap < 0 || MASSMAX <= msap )
		return ;
	if(( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return ;


	TTP_UnbindLSAP( MASSConfig[msap].tsap ) ;
	MASSConfig[msap].Mode		= 0 ;
}


int		IrDAAPI	IrMASS_GetRealLSAP( int msap )
{
	if( msap < 0 || MASSMAX <= msap )
		return( IrDAR_NORESOURCE ) ;
	if(( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return( IrDAR_NORESOURCE ) ;


	return( TTP_GetRealLSAP( MASSConfig[msap].tsap )) ;
}


int		IrDAAPI	IrMASS_ConnectReq( int smsap, dword addr, int dmsap )
{
	int			ret ;
	TTPCONNDAT	tcd ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::IrMASS_ConnectReq():" ) ;
	_DPUTW( DL_MASSINFO, " src-msap", smsap ) ;
	_DPUTW( DL_MASSINFO, ", Mode:", MASSConfig[smsap].Mode ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( MASSConfig[smsap].Mode & MASSM_Connected )
		return( IrDAR_CONNECTING ) ;


	tcd.Data		= 0 ;
	tcd.DataSize	= 0 ;
	ret		= TTP_ConnectReq( smsap, addr, dmsap, &tcd ) ;
	if( ret != IrDAR_ACCEPT )
		return( ret ) ;


	return( ret ) ;
}


int		IrDAAPI	IrMASS_DisconnectReq( int msap )
{
	if( msap < 0 || MASSMAX <= msap || ( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


	if(( MASSConfig[msap].Mode & MASSM_Connected ) == 0 )
		return( IrDAR_INVALID ) ;


#if	defined(DMSG2)
	_DPUTS( DL_MASSINFO, "MASS::IrMASS_DisconnectReq():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	return( TTP_DisconnectReq( MASSConfig[msap].tsap, 0, 0 )) ;
}


int		IrDAAPI	IrMASS_MediaCheckReq( int msap )
{
	if( msap < 0 || MASSMAX <= msap || ( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


	MASSConfig[msap].TxBuf[MASS_CMD]	= MASS_CMDB + MASS_FINALB + CMD_MDACHK ;
	return( TTP_DataReq( MASSConfig[msap].tsap, MASSConfig[msap].TxBuf, 1 )) ;
}


int		IrDAAPI	IrMASS_MediaCheckRsp( int msap, word status )
{
	MASSConfig[msap].TxBuf[MASS_CMD]	= MASS_FINALB + CMD_MDACHK ;
	MASSConfig[msap].TxBuf[MASS_CMD+1]	= status ;
	return( TTP_DataReq( MASSConfig[msap].tsap, MASSConfig[msap].TxBuf, 2 )) ;
}


int		IrDAAPI	IrMASS_DataReq( int msap, dword sec, word count, void IrDAFAR *buf )
{
	int		ret ;


	if( msap < 0 || MASSMAX <= msap || ( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::IrMASS_DataReq():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTD( DL_MASSINFO, " secno:", sec ) ;
	_DPUTW( DL_MASSINFO, " count:", count ) ;
	_DPUTD( DL_MASSINFO, " buf:", (dword)buf ) ;
	_DPUTD( DL_MASSINFO, "-", ((dword)buf ) + SECTORSIZE - 1 ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if(( ret = SendDataReq( msap, sec, count )) == IrDAR_ACCEPT ) {
		MASSConfig[msap].Mode		|= MASSM_WaitHeader ;
		MASSConfig[msap].RxBufPtr	= (byte IrDAFAR *)buf ;
		MASSConfig[msap].RxReqSize	= count * SECTORSIZE ;
		MASSConfig[msap].RxReqCount	= count ;
	}
	return( ret ) ;
}


int		IrDAAPI	IrMASS_DataRsp( int msap, dword sec, word count, const void IrDAFAR *buf, uint status )
{
	if( msap < 0 || MASSMAX <= msap || ( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return( IrDAR_INVALID ) ;


	if( MASSConfig[msap].TxRspSize )
		return( IrDAR_NORESPONSE ) ;


#if	defined(DMSG2)
	_DPUTS( DL_MASSINFO, "MASS:IrMASS_DataRsp():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTD( DL_MASSINFO, " sec:", sec ) ;
	_DPUTW( DL_MASSINFO, " count:", count ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	return( SendFirstDataRsp( msap, sec, count, buf, status )) ;
}


static	int		CallConnectInd( int smsap, dword addr, int dlsap )
{
	if( MASSConfig[smsap].CallBack.ConnectInd )
		return((*MASSConfig[smsap].CallBack.ConnectInd)( smsap, addr, dlsap )) ;


	return( IrDA_CONNECTDI ) ;
}


static	void	CallConnectCnf( int smsap, dword addr, int dmsap, int status )
{
	if( MASSConfig[smsap].CallBack.ConnectCnf )
		(*MASSConfig[smsap].CallBack.ConnectCnf)( smsap, addr, dmsap, status ) ;
}


static	void	CallDisconnectInd( int msap, int status )
{
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::CallDisconnectInd():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " status:", status ) ;
	_DPUTD( DL_MASSINFO, " CallBack.DisconnectInd:", (dword)MASSConfig[msap].CallBack.DisconnectInd ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( MASSConfig[msap].CallBack.DisconnectInd )
		(*MASSConfig[msap].CallBack.DisconnectInd)( msap, status ) ;
}


static	void	CallMediaChkInd( int msap )
{
	if( MASSConfig[msap].CallBack.MediaChkInd )
		(*MASSConfig[msap].CallBack.MediaChkInd)( msap ) ;
}


static	void	CallMediaChkCnf( int msap, int status )
{
	if( MASSConfig[msap].CallBack.MediaChkCnf )
		(*MASSConfig[msap].CallBack.MediaChkCnf)( msap, status ) ;
}


static	void	CallReadInd( int msap, dword secno, word count )
{
	if( MASSConfig[msap].CallBack.ReadInd )
		(*MASSConfig[msap].CallBack.ReadInd)( msap, secno, count ) ;
}


static	void	CallReadCnf( int msap, dword secno, word count, int status )
{
	if( MASSConfig[msap].CallBack.ReadCnf )
		(*MASSConfig[msap].CallBack.ReadCnf)( msap, secno, count, status ) ;
}


static	void	CallTxCnf( int msap )
{
	if( MASSConfig[msap].CallBack.TxCnf )
		(*MASSConfig[msap].CallBack.TxCnf)( msap ) ;
}


static	int		GetMSAP( int tsap )
{
	int		i ;


	for( i=0 ; i<MASSMAX ; i++ )
		if( MASSConfig[i].tsap == tsap )
			return( i ) ;


	return( -1 ) ;
}


static	int		SendDataReq( int msap, dword secno, word count )
{
	MASSConfig[msap].TxBuf[MASS_CMD]	= MASS_CMDB + MASS_FINALB + CMD_READ ;
	MASSConfig[msap].TxBuf[MASS_UN]		= 0 ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::SendDataReq():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTD( DL_MASSINFO, " secno:", secno ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	MiscSetBigEndian32( &MASSConfig[msap].TxBuf[MASS_READSEC], secno ) ;
	MiscSetBigEndian16( &MASSConfig[msap].TxBuf[MASS_READCNT], count ) ;


	return( TTP_DataReq( MASSConfig[msap].tsap, MASSConfig[msap].TxBuf, MASS_READCNT+ sizeof( word ))) ;
}


static	int		SendFirstDataRsp( int msap, dword secno, word count, const void IrDAFAR *buf, uint status )
{
	uint		len ;
	int			ret ;
	const byte IrDAFAR *bufp ;


	MASSConfig[msap].TxBuf[MASS_CMD]	= CMD_READ ;
	MASSConfig[msap].TxBuf[MASS_UN]		= 0 ;
	MiscSetBigEndian32( &MASSConfig[msap].TxBuf[MASS_READSEC], (dword)secno ) ;
	MiscSetBigEndian16( &MASSConfig[msap].TxBuf[MASS_READCNT], (word)count ) ;
	MASSConfig[msap].TxBuf[MASS_READSTAT]= status ;


#if	defined(DMSG2)
	_DPUTS( DL_MASSINFO, "MASS::SendFirstDataRsp():" ) ;
	_DPUTW( DL_MASSINFO, " status:", status ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( status != IrMASSR_OK ) {
		MASSConfig[msap].TxBuf[MASS_CMD]	|= MASS_FINALB ;
		return( TTP_DataReq( MASSConfig[msap].tsap, MASSConfig[msap].TxBuf, MASS_READDATA )) ;
	}


	len			= MASSConfig[msap].MaxTxSize - MASS_READDATA ;
#if	defined(DMSG2)
	_DPUTS( DL_MASSINFO, "MASS::SendFirstDataRsp():" ) ;
	_DPUTB( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " len:", len ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	IrDA_memcpy( &MASSConfig[msap].TxBuf[MASS_READDATA], buf, len ) ;
	if(( ret = TTP_DataReq( MASSConfig[msap].tsap, MASSConfig[msap].TxBuf, MASS_READDATA + len )) != IrDAR_ACCEPT )
		return( ret ) ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::SendFirstDataRsp():" ) ;
	_DPUTW( DL_MASSINFO, " txsize:", len ) ;
	_DPUTW( DL_MASSINFO, " TTP_DataReq():", ret ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	


	
	bufp		= buf ;
	MASSConfig[msap].TxRspSize	= ( SECTORSIZE * count ) - len ;
	MASSConfig[msap].TxBufPtr	= &bufp[len] ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::SendFirstDataRsp():" ) ;
	_DPUTS( DL_MASSINFO, " done" ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	return( ret ) ;
}


static	void	RecvReadRsp( int msap, const byte IrDAFAR *buf, uint bufsz )
{
	int			status ;
	MASSCONFIG	*mc ;
	const byte IrDAFAR *data ;
	uint		datasz ;


	mc		= &MASSConfig[msap] ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS:RecvReadRsp():" ) ;
	_DPUTD( DL_MASSINFO, " mc->RxReqSize:", mc->RxReqSize ) ;
	_DPUTD( DL_MASSINFO, " mc->RxReqCount:", mc->RxReqCount ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( mc->RxReqSize == 0 )
		return ;


	if( mc->Mode & MASSM_WaitHeader ) {				
		mc->Mode &= ~MASSM_WaitHeader ;
		status	= buf[MASS_READSTAT] ;
		MiscGetBigEndian32( &buf[MASS_READSEC], &mc->WaitSector ) ;
		MiscGetBigEndian16( &buf[MASS_READCNT], &mc->WaitCount ) ;
		mc->RxReqCount	-= mc->WaitCount ;


#if 0
		if( status != IrMASSR_OK ) {
			mc->RxReqSize	= 0 ;
			CallReadCnf( msap, mc->WaitSector, mc->WaitCount - mc->RxReqCount, status ) ;
			return ;
		}
#endif


		data	= &buf[MASS_READDATA] ;
		datasz	= bufsz - MASS_READDATA ;
	} else {
		data	= &buf[MASS_READSEC] ;
		datasz	= bufsz - MASS_READSEC ;
	}


	if( datasz >= mc->RxReqSize )				
		datasz	= mc->RxReqSize ;
	if( datasz > 0 )
		IrDA_memcpy( mc->RxBufPtr, data, datasz ) ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS:RecvReadRsp():" ) ;
	_DPUTD( DL_MASSINFO, " stored:", (dword)mc->RxBufPtr ) ;
	_DPUTD( DL_MASSINFO, "-", ((dword)mc->RxBufPtr ) + datasz - 1 ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	mc->RxBufPtr	+= datasz ;
	mc->RxReqSize	-= datasz ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS:RecvReadRsp():" ) ;
	_DPUTS( DL_MASSINFO, " final:" ) ;
	_DPUTS( DL_MASSINFO, ( buf[MASS_CMD] & MASS_FINALB ) ? "ON" : "OFF" ) ;
	_DPUTW( DL_MASSINFO, " mc->RxReqSize:", mc->RxReqSize ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if(( buf[MASS_CMD] & MASS_FINALB ) || mc->RxReqSize == 0 ) {
		mc->Mode		|= MASSM_WaitHeader ;
		CallReadCnf( msap, mc->WaitSector, mc->WaitCount, status ) ;
	}
}


static	void	TTP_RecvData( int msap )
{
	MASSCONFIG	*mc ;
	const byte	IrDAFAR *ptr ;
	uint	len ;
	int		cr, final, cmd ;
	dword	secno ;
	word	count ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::TTP_RecvData():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " mc->Mode:", mc->Mode ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	mc		= &MASSConfig[msap] ;
	if( mc->Mode & MASSM_RxRecvLock )
		return ;


	mc->Mode	|= MASSM_RxRecvLock ;
	for(;;) {
#if	defined(DMSG)
		len		= TTP_GetRxSize( mc->tsap ) ;
		_DPUTS( DL_MASSINFO, "MASS::TTP_RecvData():" ) ;
		_DPUTW( DL_MASSINFO, " TTP_GetRxSize():", len ) ;
		_DPUTS( DL_MASSINFO, "\n" ) ;
		if( len == 0 )
			break ;
#else	
		if(( len = TTP_GetRxSize( mc->tsap )) == 0 )
			break ;
#endif	


		ptr		= TTP_GetDataPtr( mc->tsap ) ;
		if( len >= 1 ) {
			cr		= ptr[0] & MASS_CMDB ;
			final	= ptr[0] & MASS_FINALB ;
			cmd		= ptr[0] & MASS_CMDMASK ;


#if	defined(DMSG)
			_DPUTS( DL_MASSINFO, "MASS::TTP_RecvData():" ) ;
			_DPUTW( DL_MASSINFO, " msap:", msap ) ;
			_DPUTW( DL_MASSINFO, " len:", len ) ;
			_DPUTS( DL_MASSINFO, " cr:" ) ;
			_DPUTS( DL_MASSINFO, cr ? "ON" : "OFF" ) ;
			_DPUTS( DL_MASSINFO, " final:" ) ;
			_DPUTS( DL_MASSINFO, final ? "ON" : "OFF" ) ;
			_DPUTB( DL_MASSINFO, " cmd:", cmd ) ;
			_DPUTS( DL_MASSINFO, "\n" ) ;
			{	int		i ;
				if( len > 0 ) {
					_DPUTS( DL_MASSDAT, "                      :" ) ;
					_DPUTB( DL_MASSDAT, " dat=", ptr[0] ) ;
					for( i=1 ; i<len ; i++ )
						_DPUTB( DL_MASSDAT, ".", ptr[i] ) ;
					_DPUTS( DL_MASSDAT, "\n" ) ;
				}
			}
#endif	
			switch( cmd ) {
			case	CMD_INIT :
				break ;
			case	CMD_READ :
				if( cr && len == MASS_READCNT+ sizeof( word )) {
					MiscGetBigEndian32( &ptr[MASS_READSEC], &secno ) ;
					MiscGetBigEndian16( &ptr[MASS_READCNT], &count ) ;
					CallReadInd( msap, secno, count ) ;
					break ;
				}
				if( cr == FALSE )
					RecvReadRsp( msap, ptr, len ) ;
				break ;
			case	CMD_MDACHK :
				if( cr )
					CallMediaChkInd( msap ) ;
				else
					CallMediaChkCnf( msap, ptr[1] ) ;
				break ;
			}
		}


		TTP_ReleaseRxQueue( mc->tsap ) ;
	}


	mc->Mode	&= ~MASSM_RxRecvLock ;
}


static	int		IrDACB	CBConnectInd( int stsap, dword addr, int dtsap, TTPCONNDAT IrDAFAR *lcd )
{
	int		ret ;
	int		smsap ;


	smsap	= GetMSAP( stsap ) ;
#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_MASSINFO, "MASS::CBConnectInd():" ) ;
	_DPUTW( DL_MASSINFO, " smsap:", smsap ) ;
	if( lcd->DataSize > 0 ) {
		_DPUTB( DL_MASSINFO, " data=", lcd->Data[0] ) ;
		for( i=1 ; i<lcd->DataSize ; i++ )
			_DPUTB( DL_MASSINFO, ".", lcd->Data[i] ) ;
	}
	_DPUTS( DL_MASSINFO, "\n" ) ;
}
#endif	
	if( smsap < 0 )
		return( IrDA_CONNECTDI ) ;
	if(( MASSConfig[smsap].Mode & MASSM_Used ) == 0 )
		return( IrDA_CONNECTDI ) ;


	MASSConfig[smsap].MaxDataIndSize	= (word)TTP_GetMaxRxSize( MASSConfig[smsap].tsap ) ;
	MASSConfig[smsap].MaxTxSize			= TTP_GetMaxTxSize( stsap ) ;
	MASSConfig[smsap].RxReqSize			= 0 ;
	MASSConfig[smsap].TxRspSize			= 0 ;


	ret		= CallConnectInd( smsap, addr, dtsap ) ;
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::CBConnectInd():" ) ;
	_DPUTW( DL_MASSINFO, " MASSConfig[smsap].MaxTxSize:", MASSConfig[smsap].MaxTxSize ) ;
	_DPUTW( DL_MASSINFO, " CallConnectInd( ", smsap ) ;
	_DPUTD( DL_MASSINFO, ", ", addr ) ;
	_DPUTW( DL_MASSINFO, ", ", dtsap ) ;
	_DPUTW( DL_MASSINFO, " )=", ret ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	if( ret == IrDA_CONNECTEN )
		MASSConfig[smsap].Mode		|= MASSM_Connected ;


	return( ret ) ;
}


static	void	IrDACB	CBConnectCnf( int stsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status )
{
	int		smsap ;


	UNREFERENCE( lcd ) ;
	UNREFERENCE( maxsdu ) ;


	smsap	= GetMSAP( stsap ) ;
	if( smsap < 0 )
		return ;
	if(( MASSConfig[smsap].Mode & MASSM_Used ) == 0 )
		return ;


	MASSConfig[smsap].Mode				|= MASSM_Connected ;
	MASSConfig[smsap].MaxDataIndSize	= (word)TTP_GetMaxRxSize( smsap ) ;
	MASSConfig[smsap].MaxTxSize			= TTP_GetMaxTxSize( stsap ) ;
	MASSConfig[smsap].RxReqSize			= 0 ;
	MASSConfig[smsap].TxRspSize			= 0 ;


	CallConnectCnf( smsap, addr, dlsap, status ) ;
}


static	void	IrDACB	CBDisconnectInd( int tsap, int status )
{
	int		msap ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::CBDisconnectInd():" ) ;
	_DPUTW( DL_MASSINFO, " tsap:", tsap ) ;
	_DPUTW( DL_MASSINFO, " status:", status ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	
	msap	= GetMSAP( tsap ) ;
	if( msap < 0 )
		return ;
	if(( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return ;


	MASSConfig[msap].Mode	&= ~MASSM_Connected ;
	CallDisconnectInd( msap, status ) ;
}


static	void	IrDACB	CBDataInd( int tsap )
{
	int		msap ;


	msap	= GetMSAP( tsap ) ;
	if( msap < 0 || MASSMAX <= msap )
		return ;
	if(( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return ;


	TTP_RecvData( msap ) ;
}


static	void	IrDACB	CBDataCnf( int tsap )
{
	int		msap ;
	uint	len ;
	int		finalb ;
	byte IrDAFAR	*txp ;
#if	defined(DMSG)
	int		ret ;
#endif	


	msap	= GetMSAP( tsap ) ;
	if( msap < 0 )
		return ;
	if(( MASSConfig[msap].Mode & MASSM_Used ) == 0 )
		return ;


#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::CBDataCnf():" ) ;
	_DPUTW( DL_MASSINFO, " msap:", msap ) ;
	_DPUTW( DL_MASSINFO, " TxRspSize:", MASSConfig[msap].TxRspSize ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	


	if( MASSConfig[msap].TxRspSize == 0 )
		return ;


	if( MASSConfig[msap].TxRspSize + 2 > MASSConfig[msap].MaxTxSize ) {
		len			= MASSConfig[msap].MaxTxSize - 2 ;
		finalb		= 0 ;
	} else {
		len			= MASSConfig[msap].TxRspSize ;
		finalb		= MASS_FINALB ;
	}


	txp		= MASSConfig[msap].TxBuf ;
	txp[MASS_CMD]	= finalb | CMD_READ ;
	txp[MASS_UN]	= 0 ;
	IrDA_memcpy( &txp[2], MASSConfig[msap].TxBufPtr, len ) ;


#if	defined(DMSG)
	if(( ret = TTP_DataReq( MASSConfig[msap].tsap, txp, 2+len )) == IrDAR_ACCEPT ) {
#else	
	if( TTP_DataReq( MASSConfig[msap].tsap, txp, 2+len ) == IrDAR_ACCEPT ) {
#endif	
		MASSConfig[msap].TxRspSize	-= len ;
		MASSConfig[msap].TxBufPtr	+= len ;
	}
#if	defined(DMSG)
	_DPUTS( DL_MASSINFO, "MASS::CBDataCnf():" ) ;
	_DPUTB( DL_MASSINFO, " TTP_DataReq( ", MASSConfig[msap].tsap ) ;
	_DPUTW( DL_MASSINFO, ", ", 2+len ) ;
	_DPUTW( DL_MASSINFO, " )=", ret ) ;
	_DPUTS( DL_MASSINFO, "\n" ) ;
#endif	


	if( MASSConfig[msap].TxRspSize == 0 )
		CallTxCnf( msap ) ;
}


#endif	
