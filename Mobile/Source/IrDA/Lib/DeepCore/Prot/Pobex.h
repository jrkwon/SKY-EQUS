


#if !defined(__pobex_h__)
#define	__pobex_h__


#include	"irda.h"
#include	"irobex.h"
#include	"tinytp.h"
#include	"okmisc.h"


#if !defined(OBEXTXBUFSZ)
# define	OBEXTXBUFSZ		2046
#endif	
#if !defined(OBEXRXBUFSZ)
# define	OBEXRXBUFSZ		2046
#endif	


typedef	struct	_OBEXFLAGS	{
	IrDABOOL			Used ;		
	IrDABOOL			Connected ;	
}	OBEXFLAGS ;


typedef	struct	{
	byte				buf[OBEXRXBUFSZ] ;		
	word				packetLength ;			
	word				usedSize ;				
	byte				sar ;
}	OBEXRXSDU ;


typedef	struct	{
	IrDABOOL			lock ;					
	byte				buf[OBEXTXBUFSZ] ;		
	word				maxPacketLength ;		
	word				usedSize ;				
	byte				sar ;
}	OBEXTXSDU ;


typedef	struct	_OBEXBCONFIG {
	int					lsap ;					
	OBEXRXSDU			rx ;
	OBEXTXSDU			tx ;
	IrDABOOL			WaitResp ;	
	void				(*RspCallBack)( struct _OBEXBCONFIG *ob ) ;
}	OBEXBCONFIG ;


typedef	struct	_OBEXCONFIG	{
	OBEXBCONFIG			b ;
	uint				Mode ;					
	volatile OBEXFLAGS	of ;					
	OBEXCALLBACK		CallBack ;				
	byte				PeerObexVer ;			
}	OBEXCONFIG ;


#define	OBEX_WAITTTPCONNRSP	0x0004
#define	OBEX_WAITCONNREQ	0x0020


static	OBEXCONFIG		ObexConfig[OBEXMAX] ;	
#if !defined(IrOBEXNOULTRA)
static	OBEXBCONFIG		ultraConfig ;
static	void	(IrDACB* obexPutReqIndPtr)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
static	void	(IrDACB* obexGetReqIndPtr)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz ) ;
static	void	(IrDACB* obexRspIndPtr)( byte sar, int rescd, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
#endif	


static	OBEXCONFIG		*obexMakeObexConfigPtr( int lsap ) ;
static	int				obexTasTxBuf( OBEXBCONFIG *ob ) ;
static	word			obexScanBody( const byte IrDAFAR *ptr, word size ) ;
static	word			obexGetHeaderSize( const byte IrDAFAR *header ) ;
static	void			obexGetMaxTxLength( OBEXCONFIG *oc, const byte IrDAFAR *src ) ;
static	void			obexSetMaxRxLength( OBEXCONFIG *oc, byte IrDAFAR *des ) ;
static	void			obexBuildPacket( OBEXBCONFIG *ob, const byte IrDAFAR *ptr, int len ) ;
static	void			obexParsePacket( OBEXBCONFIG *ob ) ;
static	void			obexReadConnectReq( OBEXBCONFIG *ob ) ;
static	void			obexReadDiscReq( OBEXBCONFIG *ob ) ;
static	void			obexReadPutReq( OBEXBCONFIG *ob ) ;
static	void			obexReadGetReq( OBEXBCONFIG *ob ) ;
static	void			obexReadAbortReq( OBEXBCONFIG *ob ) ;
static	void			obexReadSetPathReq( OBEXBCONFIG *ob ) ;
static	void			obexCallConnectReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallDisconnectReq( OBEXCONFIG *oc, int status, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallPutReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
static	void			obexCallGetReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallAbortReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallSetPathReq( OBEXCONFIG *oc, byte flag, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallConnectRsp( OBEXCONFIG *oc, int status, const void IrDAFAR *header, int headersz ) ;
static	void			obexCallResponse( OBEXBCONFIG *ob, int resp, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
static	int				obexSendConnectReq( OBEXCONFIG *oc ) ;
static	void			obexMakeConnectReq( OBEXCONFIG *oc ) ;
static	void			obexMakeGetReq( OBEXBCONFIG *ob, const void IrDAFAR *header, int headersz ) ;
static	void			obexMakeConnectRsp( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz ) ;
#if defined(USEOBEXDISCREQ)
static	void			obexMakeDisconnectReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz ) ;
#endif	
static	void			obexMakePutReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
static	void			obexMakeSetPathReq( OBEXCONFIG *oc, byte flag, const void IrDAFAR *header, int headersz ) ;
static	void			obexMakeAbortReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz ) ;
static	void			obexMakeGetRsp( OBEXBCONFIG *ob, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
static	void			obexSendBadReqRsp( OBEXBCONFIG *ob ) ;
static	int				obexSendPacket( OBEXBCONFIG *ob ) ;
static	void			obexCbConnectRsp( OBEXBCONFIG *ob ) ;
static	void			obexCbResponse( OBEXBCONFIG *ob ) ;
static	int		IrDACB	obexCbTTPConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	obexCbTTPConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status ) ;
static	void	IrDACB	obexCbTTPDisconnectInd( int lsap, int status ) ;
static	void	IrDACB	obexCbTTPDataInd( int lsap ) ;
static	void	IrDACB	obexCbTTPDataCnf( int lsap ) ;
#if !defined(IrOBEXNOULTRA)
static	void	IrDACB	obexCbLMDataInd( const void IrDAFAR* data, int datasz ) ;
static	void	IrDACB	obexCbLMDataCnf( void ) ;
#endif	


int		IrDAAPI	IrOBEX_Open( void )
{
	int		i ;
#if !defined(IrOBEXNOULTRA)
	int		ret ;
#endif	


	for( i=0 ; i<OBEXMAX ; i++ )
		IrDA_memset( &ObexConfig[i], 0, sizeof( OBEXCONFIG )) ;
#if !defined(IrOBEXNOULTRA)
	IrDA_memset( &ultraConfig, 0, sizeof( ultraConfig )) ;
	ultraConfig.lsap	= CLESSLSAP ;
	obexPutReqIndPtr	= 0 ;
	obexGetReqIndPtr	= 0 ;
	obexRspIndPtr		= 0 ;
	if(( ret = IrLMP_SetCLessDataCnf( obexCbLMDataCnf )) != IrDAR_ACCEPT )
		return( ret ) ;
	if(( ret = IrLMP_SetCLessDataInd( obexCbLMDataInd )) != IrDAR_ACCEPT ) {
		IrLMP_SetCLessDataCnf( 0 ) ;
		return( ret ) ;
	}
#endif	


	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrOBEX_Close( void )
{
#if !defined(IrOBEXNOULTRA)
	IrLMP_SetCLessDataCnf( 0 ) ;
	IrLMP_SetCLessDataInd( 0 ) ;
#endif	
	return( IrDAR_ACCEPT ) ;
}


int		IrDAAPI	IrOBEX_RequestLSAP( void )
{
	return( TTP_RequestLSAP()) ;
}


int		IrDAAPI	IrOBEX_BindLSAP( int lsap, const OBEXCALLBACK IrDAFAR *ocb )
{
	TTPCALLBACK	tcb ;
	OBEXCONFIG	*oc ;
	int			ret ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_NORESOURCE ) ;


	if( oc->of.Used )
		return( IrDAR_INUSE ) ;


	IrDA_memset( &tcb, 0, sizeof( TTPCALLBACK )) ;
	tcb.ConnectInd		= obexCbTTPConnectInd ;
	tcb.ConnectCnf		= obexCbTTPConnectCnf ;
	tcb.DataInd			= obexCbTTPDataInd ;
	tcb.DataCnf			= obexCbTTPDataCnf ;
	tcb.DisconnectInd	= obexCbTTPDisconnectInd ;
	if(( ret = TTP_BindLSAP( lsap, &tcb )) != IrDAR_ACCEPT )
		return( ret ) ;


	oc->Mode			= 0 ;
	oc->b.lsap			= lsap ;
	oc->of.Used			= TRUE ;
	oc->b.tx.lock		= FALSE ;
	oc->b.tx.usedSize	= 0 ;
	IrDA_memcpy( &oc->CallBack, ocb, sizeof( OBEXCALLBACK )) ;
	return( ret ) ;
}


void	IrDAAPI	IrOBEX_UnbindLSAP( int lsap )
{
	OBEXCONFIG	*oc ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return ;


	if( !oc->of.Used )
		return ;


	oc->Mode	= 0 ;
	oc->of.Used	= FALSE ;
	TTP_UnbindLSAP( lsap ) ;
}


int		IrDAAPI	IrOBEX_GetRealLSAP( int lsap )
{
	return( TTP_GetRealLSAP( lsap )) ;
}


int		IrDAAPI	IrOBEX_GetMaxTxBodySize( int lsap, int headersz )
{
	OBEXCONFIG	*oc ;
	int			len ;
	word		maxsz ;


	if( lsap == CLESSLSAP ) {
		maxsz	= 60 ;
	} else {
		if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
			return( IrDAR_EPARM ) ;
		if( !oc->of.Used )
			return( IrDAR_EPARM ) ;
		if( !oc->of.Connected )
			return( IrDAR_NOCONNECT ) ;


#if	defined(DMSG)
		_DPUTS( DL_USERINFO, "OBEX::IrOBEX_GetMaxTxBodySize():" ) ;
		_DPUTW( DL_USERINFO, " lsap:", lsap ) ;
		_DPUTW( DL_USERINFO, " MaxTxPacketLength:", oc->b.tx.maxPacketLength ) ;
		_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
		_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
		if(( maxsz = oc->b.tx.maxPacketLength ) > OBEXTXBUFSZ )
			maxsz	= OBEXTXBUFSZ ;
	}


	if(( len = maxsz - 3 - headersz - 3 ) < 0 )
		return( 0 ) ;
	return( len ) ;
}


#if defined(IrDAPRIMARY)


int		IrDAAPI	IrOBEX_ConnectReq( int slsap, dword addr, int dlsap, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG	*oc ;
	TTPCONNDAT	tcd ;
	int			ret ;


	if(( oc = obexMakeObexConfigPtr( slsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if( !oc->of.Used )
		return( IrDAR_EPARM ) ;
	if( oc->of.Connected )
		return( IrDAR_CONNECTING ) ;
	if( oc->Mode & OBEX_WAITTTPCONNRSP )
		return( IrDAR_BUSYWAIT ) ;


	if( headersz >= OBEXTXBUFSZ - 7 )
		headersz	= OBEXTXBUFSZ - 7 ;
	IrDA_memcpy( &oc->b.tx.buf[7], header, headersz ) ;
	oc->b.tx.usedSize	= headersz + 7 ;


	tcd.Data		= 0 ;
	tcd.DataSize	= 0 ;
	oc->Mode	|= OBEX_WAITTTPCONNRSP ;
	if(( ret = TTP_ConnectReq( slsap, addr, dlsap, &tcd )) != IrDAR_ACCEPT ) {
		oc->Mode	&= ~OBEX_WAITTTPCONNRSP ;
		return( ret ) ;
	}


	return( IrDAR_ACCEPT ) ;
}
#endif	


int		IrDAAPI	IrOBEX_ConnectRsp( int lsap, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;


	obexMakeConnectRsp( oc, header, headersz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_DisconnectReq( int lsap, const void IrDAFAR *header, int headersz )
{
	
	
	
	
	


	


#if defined(USEOBEXDISCREQ)
	OBEXCONFIG		*oc ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;


	obexMakeDisconnectReq( oc, header, headersz ) ;
	return( obexSendPacket( oc )) ;
#else	
	int				ret ;


	UNREFERENCE( header ) ;
	UNREFERENCE( headersz ) ;


	if(( ret = TTP_DisconnectReq( lsap, 0, 0 )) == IrDAR_ACCEPT )
		ObexConfig[lsap].of.Connected	= FALSE ;


	return( ret ) ;
#endif	
}


int		IrDAAPI	IrOBEX_SendResponse( int lsap, int rescd, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_USERINFO, "OBEX::IrOBEX_SendResponse():" ) ;
	_DPUTW( DL_USERINFO, " lsap:", lsap ) ;
	_DPUTW( DL_USERINFO, " rescd:", rescd ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	if( headersz > 0 ) {
		_DPUTB( DL_USERINFO, " header:", ((const byte IrDAFAR*)header)[0] ) ;
		for( i=1 ; i<headersz ; i++ )
			_DPUTB( DL_USERINFO, ".", ((const byte IrDAFAR*)header)[0] ) ;
	}
	_DPUTS( DL_USERINFO, "\n" ) ;
}
#endif	
	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;


	oc->b.tx.usedSize	= 3 + headersz ;
	oc->b.tx.buf[0]	= rescd ;
	MiscSetBigEndian16( &oc->b.tx.buf[1], oc->b.tx.usedSize ) ;
	IrDA_memcpy( &oc->b.tx.buf[3], header, headersz ) ;
	oc->b.tx.lock	= FALSE ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_GetReq( int lsap, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;


	obexMakeGetReq((OBEXBCONFIG*)oc, header, headersz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_GetRsp( int lsap, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;


	obexMakeGetRsp((OBEXBCONFIG*)oc, rescd, fb, header, headersz, body, bodysz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_PutReq( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_USERINFO, "OBEX::IrOBEX_PutReq():" ) ;
	_DPUTW( DL_USERINFO, " lsap:", lsap ) ;
	_DPUTW( DL_USERINFO, " fb:", fb ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	if( headersz > 0 ) {
		_DPUTB( DL_USERINFO, " header:", ((const byte IrDAFAR*)header)[0] ) ;
		for( i=1 ; i<headersz ; i++ )
			_DPUTB( DL_USERINFO, ".", ((const byte IrDAFAR*)header)[i] ) ;
	}
	_DPUTW( DL_USERINFO, " bodysz:", bodysz ) ;
	_DPUTD( DL_USERINFO, " body:", (dword)((const void __far*)body )) ;
	if( bodysz > 0 ) {
		_DPUTB( DL_USERINFO, ":", ((const byte IrDAFAR*)body)[0] ) ;
		for( i=1 ; i<bodysz ; i++ )
			_DPUTB( DL_USERINFO, ".", ((const byte IrDAFAR*)body)[i] ) ;
	}
	_DPUTS( DL_USERINFO, "\n" ) ;
}
#endif	
	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;
	obexMakePutReq((OBEXBCONFIG*)oc, fb, header, headersz, body, bodysz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_SetPathReq( int lsap, byte flag, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_USERINFO, "OBEX::IrOBEX_SetPathReq():" ) ;
	_DPUTW( DL_USERINFO, " lsap:", lsap ) ;
	_DPUTB( DL_USERINFO, " flag:", flag ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	if( headersz > 0 ) {
		_DPUTB( DL_USERINFO, " header:", ((const byte IrDAFAR*)header)[0] ) ;
		for( i=1 ; i<headersz ; i++ )
			_DPUTB( DL_USERINFO, ".", ((const byte IrDAFAR*)header)[i] ) ;
	}
	_DPUTS( DL_USERINFO, "\n" ) ;
}
#endif	
	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;
	obexMakeSetPathReq( oc, flag, header, headersz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


int		IrDAAPI	IrOBEX_AbortReq( int lsap, const void IrDAFAR *header, int headersz )
{
	OBEXCONFIG		*oc ;
	int				ret ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return( IrDAR_EPARM ) ;
	if(( ret = obexTasTxBuf((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		return( ret ) ;
	obexMakeAbortReq( oc, header, headersz ) ;
	return( obexSendPacket((OBEXBCONFIG*)oc )) ;
}


#if !defined(IrDANOSTATIC)
static	OBEXCONFIG		*obexMakeObexConfigPtr( int lsap )
{
	if( 0 <= lsap && lsap < OBEXMAX )
		return( &ObexConfig[lsap] ) ;


	return((OBEXCONFIG*)0 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int				obexTasTxBuf( OBEXBCONFIG *ob )
{
	if( ob->lsap != CLESSLSAP ) {
		if( !((OBEXCONFIG*)ob)->of.Used )
			return( IrDAR_EPARM ) ;
	}
	if( ob->tx.lock || ob->tx.usedSize )
		return( IrDAR_BUSYWAIT ) ;
	ob->tx.lock	= TRUE ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	word			obexScanBody( const byte IrDAFAR *ptr, word size )
{
	long		lsize, bodyp ;
	word		sz ;


	lsize		= (long)size ;
	bodyp		= 0 ;
	for( bodyp = 0 ; bodyp < lsize ; ) {
		if(( *ptr & 0xFE ) == 0x48 )
			return((word)bodyp ) ;


		sz		= obexGetHeaderSize( ptr ) ;
		if( sz <= 2 ) {								
#if	defined(DMSG)
		_DPUTS( DL_USERINFO, "OBEX::obexScanBody():" ) ;
		_DPUTS( DL_USERINFO, " BROKEN PACKET!!" ) ;
		_DPUTW( DL_USERINFO, " HL:", sz ) ;
		_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
			break ;
		}
		bodyp	+= sz ;
#if	defined(DMSG)
		_DPUTS( DL_USERINFO, "OBEX::obexScanBody():" ) ;
		_DPUTD( DL_USERINFO, " total:", (dword)bodyp ) ;
		_DPUTB( DL_USERINFO, " hi:", ptr[0] ) ;
		_DPUTW( DL_USERINFO, " len:", sz ) ;
		_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
		ptr		+= sz ;
	}


	return( size ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	word			obexGetHeaderSize( const byte IrDAFAR *header )
{
	word	tmpw ;


	switch(((int)( header[0] )) & 0xC0 ) {
	case	0x00 :	
		MiscGetBigEndian16( &header[1], &tmpw ) ;
		return( tmpw ) ;
	case	0x40 :	
		MiscGetBigEndian16( &header[1], &tmpw ) ;
		return( tmpw ) ;
	case	0x80 :	
		return( 1 + 1 ) ;
	case	0xC0 :	
		return( 1 + 4 ) ;
	}


	
	return( 0x3FFF ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexGetMaxTxLength( OBEXCONFIG *oc, const byte IrDAFAR *src )
{
	uint		maxTxLen ;


	MiscGetBigEndian16( src, (word IrDAFAR *)&oc->b.tx.maxPacketLength ) ;
	maxTxLen	= TTP_GetMaxTxSize( oc->b.lsap ) ;


	if( maxTxLen < oc->b.tx.maxPacketLength )
		oc->b.tx.maxPacketLength	= maxTxLen ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexSetMaxRxLength( OBEXCONFIG *oc, byte IrDAFAR *des )
{
	word	maxRxLen ;


	maxRxLen	= (word)TTP_GetMaxRxSize( oc->b.lsap ) ;
	MiscSetBigEndian16( des, maxRxLen ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexBuildPacket( OBEXBCONFIG *ob, const byte IrDAFAR *ptr, int len )
{
	uint		packetsz ;


	for( ; len > 0 ; ) {
		if( ob->rx.packetLength == 0 ) {
			ob->rx.usedSize	= 0 ;


			MiscGetBigEndian16( &ptr[1], (word IrDAFAR*)&ob->rx.packetLength ) ;
#if	defined(DMSG)
			_DPUTS( DL_USERINFO, "OBEX::obexCbTTPDataInd(): " ) ;
			_DPUTW( DL_USERINFO, " RxPacketLength:", ob->rx.packetLength ) ;
			_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
			if( ob->rx.packetLength > OBEXRXBUFSZ || ob->rx.packetLength == 0 ) {
				ob->rx.packetLength	= 0 ;
				break ;
			}
		}


		if( ob->rx.usedSize + len > ob->rx.packetLength )
			packetsz	= ob->rx.packetLength - ob->rx.usedSize ;
		else
			packetsz	= len ;


		IrDA_memcpy( &ob->rx.buf[ob->rx.usedSize], ptr, packetsz ) ;
		ob->rx.usedSize	+= packetsz ;


		if( ob->rx.usedSize == ob->rx.packetLength ) {
			ob->rx.packetLength	= 0 ;
			obexParsePacket( ob ) ;
		}


		ptr		+= packetsz ;
		len		-= packetsz ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexParsePacket( OBEXBCONFIG *ob )
{
	int					cmd ;


#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexParsePacket(): " ) ;
#endif	
	
	if( ob->WaitResp ) {
		ob->WaitResp	= FALSE ;
#if	defined(DMSG)
		_DPUTB( DL_USERINFO, " respcd:", ob->rx.buf[0] ) ;
		_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
		(*ob->RspCallBack)( ob ) ;
		return ;
	}


	cmd		= (int)( ob->rx.buf[0] ) & 0xFF ;
#if	defined(DMSG)
	_DPUTW( DL_USERINFO, " cmd:", cmd ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	switch( cmd & 0x7F ) {
	case	0x00 :						
		obexReadConnectReq( ob ) ;
		break ;
	case	0x01 :
		obexReadDiscReq( ob ) ;			
		break ;
	case	0x02 :						
		obexReadPutReq( ob ) ;
		break ;
	case	0x03 :						
		obexReadGetReq( ob ) ;
		break ;
	case	0x05 :						
		obexReadSetPathReq( ob ) ;
		break ;
	case	0x7F :						
		obexReadAbortReq( ob ) ;
		break ;
	default :
		obexSendBadReqRsp( ob ) ;
		break ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexReadConnectReq( OBEXBCONFIG *ob )
{
	OBEXCONFIG	*oc ;
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP )
		return ;
#endif	


	oc			= (OBEXCONFIG*)ob ;
	oc->b.rx.packetLength	= 0 ;
	oc->of.Connected		= TRUE ;
	oc->PeerObexVer			= oc->b.rx.buf[2] ;
	obexGetMaxTxLength( oc, &oc->b.rx.buf[5] ) ;
	obexCallConnectReq( oc, &oc->b.rx.buf[7], oc->b.rx.usedSize - 7 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexReadDiscReq( OBEXBCONFIG *ob )
{
	OBEXCONFIG *oc ;
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP )
		return ;
#endif	


	oc			= (OBEXCONFIG*)ob ;
	if( oc->of.Connected ) {
		oc->b.rx.packetLength	= 0 ;
		oc->of.Connected		= FALSE ;
		IrOBEX_SendResponse( ob->lsap, OBEXRC_OK|OBEX_FINALBIT, 0, 0 ) ;
		obexCallDisconnectReq( oc, IrLMP_DR_UserRequest, 0, 0 ) ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexReadPutReq( OBEXBCONFIG *ob )
{
	byte		*ptr ;
	word		len ;
	word		headersz ;
	word		bodysz ;
	IrDABOOL	fb ;


#if	defined(DMSG)
{ 	int		i ;
	word	plen ;
	_DPUTS( DL_USERINFO, "OBEX::obexReadPutReq():" ) ;
	_DPUTW( DL_USERINFO, " datasz:", ob->rx.usedSize ) ;
	_DPUTB( DL_USERINFO, " cmd:", ob->rx.buf[0] ) ;
	MiscGetBigEndian16( &ob->rx.buf[1], &plen ) ;
	_DPUTW( DL_USERINFO, " packet-len:", plen ) ;
	if( ob->rx.usedSize > 3 ) {
		_DPUTB( DL_USERINFO, " data:", ob->rx.buf[3] ) ;
		for( i=4 ; i<ob->rx.usedSize && i<2048 ; i++ )
			_DPUTB( DL_USERINFO, ".", ob->rx.buf[i] ) ;
	}
	_DPUTS( DL_USERINFO, "\n" ) ;
}
#endif	
	ptr			= ob->rx.buf ;
	len			= ob->rx.usedSize ;


	if( len <= 3 )
		return ;


	fb			= *ptr & 0x80 ;
	len			-= 3 ;
	ptr			+= 3 ;
	headersz	= obexScanBody( ptr, len ) ;
	bodysz		= 0 ;
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexReadPutReq():" ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( len > headersz ) {
		MiscGetBigEndian16( &ptr[headersz+1], &bodysz ) ;
		bodysz	-= 3 ;
	}


#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexReadPutReq():" ) ;
	_DPUTW( DL_USERINFO, " bodysz:", bodysz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	obexCallPutReq( ob, fb, ptr, headersz, &ptr[headersz+3], bodysz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexReadGetReq( OBEXBCONFIG *ob )
{
	word	headersz ;


	headersz	= obexScanBody( &ob->rx.buf[3], ob->rx.usedSize - 3 ) ;
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexReadGetReq():" ) ;
	_DPUTW( DL_USERINFO, " datasz:", ob->rx.usedSize ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	


	obexCallGetReq( ob, ob->rx.buf[0] & 0x80, &ob->rx.buf[3], headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexReadAbortReq( OBEXBCONFIG *ob )
{
	word	headersz ;


#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP )
		return ;
#endif	


	headersz	= obexScanBody( &ob->rx.buf[3], ob->rx.usedSize - 3 ) ;
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexReadAbortReq():" ) ;
	_DPUTW( DL_USERINFO, " datasz:", ob->rx.usedSize ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	


	obexCallAbortReq((OBEXCONFIG*)ob, &ob->rx.buf[3], headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexReadSetPathReq( OBEXBCONFIG *ob )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexReadSetPathReq():" ) ;
	_DPUTW( DL_USERINFO, " datasz:", ob->rx.usedSize ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	


	if( ob->rx.usedSize < 5 )
		return ;


#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP )
		return ;
#endif	


	obexCallSetPathReq((OBEXCONFIG*)ob, ob->rx.buf[3], &ob->rx.buf[5], ob->rx.usedSize - 5 ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallConnectRsp( OBEXCONFIG *oc, int status, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallConnectRsp(): " ) ;
	_DPUTW( DL_USERINFO, " status:", status ) ;
	_DPUTD( DL_USERINFO, " oc->CallBack.ConnectRsp:", (dword)oc->CallBack.ConnectRsp ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( oc->CallBack.ConnectRsp )
		(*oc->CallBack.ConnectRsp)( oc->b.lsap, status, header, headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallConnectReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallConnectReq(): " ) ;
	_DPUTD( DL_USERINFO, " oc->CallBack.ConnectReq:", (dword)oc->CallBack.ConnectReq ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( oc->CallBack.ConnectReq )
		(*oc->CallBack.ConnectReq)( oc->b.lsap, header, headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallDisconnectReq( OBEXCONFIG *oc, int status, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallDisconnectReq(): " ) ;
	_DPUTD( DL_USERINFO, " oc->CallBack.DisconnectReq:", (dword)oc->CallBack.DisconnectReq ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( oc->CallBack.DisconnectReq )
		(*oc->CallBack.DisconnectReq)( oc->b.lsap, status, header, headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallPutReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallPutReq(): " ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTW( DL_USERINFO, " bodysz:", bodysz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		if( obexPutReqIndPtr )
			(*obexPutReqIndPtr)( ob->rx.sar, fb, header, headersz, body, bodysz ) ;
		return ;
	}
#endif	
	if(((OBEXCONFIG*)ob)->CallBack.PutReq )
		(*((OBEXCONFIG*)ob)->CallBack.PutReq)( ob->lsap, fb, header, headersz, body, bodysz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallGetReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallGetReq(): " ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		if( obexGetReqIndPtr )
			(*obexGetReqIndPtr)( ob->rx.sar, fb, header, headersz ) ;
		return ;
	}
#endif	


	if(((OBEXCONFIG*)ob)->CallBack.GetReq )
		(*((OBEXCONFIG*)ob)->CallBack.GetReq)( ob->lsap, fb, header, headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallAbortReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallAbortReq(): " ) ;
	_DPUTD( DL_USERINFO, " oc->CallBack.AbortReq:", (dword)oc->CallBack.AbortReq ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( oc->CallBack.AbortReq )
		(*oc->CallBack.AbortReq)( oc->b.lsap, header, headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexCallSetPathReq( OBEXCONFIG *oc, byte flag, const void IrDAFAR *header, int headersz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallSetPathReq(): " ) ;
	_DPUTD( DL_USERINFO, " oc->CallBack.SetPathReq:", (dword)oc->CallBack.SetPathReq ) ;
	_DPUTB( DL_USERINFO, " flag:", flag ) ;
	_DPUTW( DL_USERINFO, " headersz:", headersz ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	if( oc->CallBack.SetPathReq )
		(*oc->CallBack.SetPathReq)( oc->b.lsap, flag, header, headersz ) ;
	else
		obexSendBadReqRsp((OBEXBCONFIG*)oc ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCallResponse( OBEXBCONFIG *ob, int resp, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCallResponse(): " ) ;
	if( ob->lsap != CLESSLSAP )
		_DPUTD( DL_USERINFO, " ((OBEXCONFIG*)ob)->CallBack.Response:", (dword)((OBEXCONFIG*)ob)->CallBack.Response ) ;
	_DPUTW( DL_USERINFO, " resp:", resp ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		if( obexRspIndPtr )
			(*obexRspIndPtr)( ob->rx.sar, resp, header, headersz, body, bodysz ) ;
		return ;
	}
#endif	
	if(((OBEXCONFIG*)ob)->CallBack.Response )
		(*((OBEXCONFIG*)ob)->CallBack.Response)( ob->lsap, resp, header, headersz, body, bodysz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int				obexSendConnectReq( OBEXCONFIG *oc )
{
	int		ret ;


	oc->Mode			&= ~OBEX_WAITCONNREQ ;
	oc->b.WaitResp		= TRUE ;
	oc->b.RspCallBack	= obexCbConnectRsp ;


	if(( ret = obexSendPacket((OBEXBCONFIG*)oc )) != IrDAR_ACCEPT )
		oc->Mode	|= OBEX_WAITCONNREQ ;
	return( ret ) ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexMakeConnectReq( OBEXCONFIG *oc )
{
	oc->b.tx.lock		= TRUE ;
	oc->b.tx.buf[0]		= 0x80 ;				
												
	MiscSetBigEndian16( &oc->b.tx.buf[1], oc->b.tx.usedSize ) ;
	oc->b.tx.buf[3]		= 0x10 ;				
	oc->b.tx.buf[4]		= 0 ;					
												
	obexSetMaxRxLength( oc, (byte IrDAFAR*)&oc->b.tx.buf[5] ) ;
	oc->b.tx.lock		= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexMakeGetReq( OBEXBCONFIG *ob, const void IrDAFAR *header, int headersz )
{
	byte	*des ;
	word	len ;


	ob->tx.usedSize		= 3 + headersz ;
	len					= ob->tx.usedSize ;
	des					= (byte*)ob->tx.buf ;
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		ob->tx.usedSize	+= 2 ;
		*des++			= 0x01 ;					
		*des++			= ob->tx.sar ;				
	}
#endif	


	*des++	= OBEXOC_GET | OBEX_FINALBIT ;
	MiscSetBigEndian16( des, len ) ;
	des					+= 2 ;
	IrDA_memcpy( des, header, headersz ) ;
	ob->WaitResp		= TRUE ;
	ob->RspCallBack		= obexCbResponse ;
	ob->tx.lock			= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexMakeConnectRsp( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz )
{
	oc->b.tx.usedSize		= 7 + headersz ;
	IrDA_memcpy( &oc->b.tx.buf[7], header, headersz ) ;
	oc->b.tx.buf[0]		= 0xA0 ;				
												
	MiscSetBigEndian16( &oc->b.tx.buf[1], oc->b.tx.usedSize ) ;


	oc->b.tx.buf[3]		= 0x10 ;				
	oc->b.tx.buf[4]		= 0 ;					
												
	obexSetMaxRxLength( oc, (byte IrDAFAR*)&oc->b.tx.buf[5] ) ;
	oc->b.tx.lock		= FALSE ;
}
#endif	


#if defined(USEOBEXDISCREQ)
#if !defined(IrDANOSTATIC)


static	void			obexMakeDisconnectReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz )
{
	oc->b.tx.usedSize		= 3 + headersz ;
	oc->b.tx.buf[0]		= 0x81 ;				
												
	MiscSetBigEndian16( &oc->b.tx.buf[1], oc->b.tx.usedSize ) ;
	IrDA_memcpy( &oc->b.tx.buf[3], header, headersz ) ;
	oc->b.tx.lock	= FALSE ;
}
#endif	
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexMakePutReq( OBEXBCONFIG *ob, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	byte	*des ;
	word	len ;


	ob->tx.usedSize		= 3 + headersz + 3 + bodysz ;
	len					= ob->tx.usedSize ;
	des					= (byte*)ob->tx.buf ;
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		*des++	= 0x01 ;						
		*des++	= ob->tx.sar ;					
		ob->tx.usedSize	+= 2 ;
	}
#endif	
	if( fb )
		*des++	= OBEXOC_PUT | OBEX_FINALBIT ;
	else
		*des++	= OBEXOC_PUT ;
	MiscSetBigEndian16( des, len ) ;
	des			+= 2 ;
	IrDA_memcpy( des, header, headersz ) ;
	des			+= headersz ;
	if( fb )
		*des++		= OBEXHI_ENDBODY ;			
	else
		*des++		= OBEXHI_BODY ;				
	MiscSetBigEndian16( des, bodysz + 3 ) ;
	des			+= 2 ;
	IrDA_memcpy( des, body, bodysz ) ;
	ob->WaitResp		= TRUE ;
	ob->RspCallBack		= obexCbResponse ;
	ob->tx.lock			= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexMakeSetPathReq( OBEXCONFIG *oc, byte flag, const void IrDAFAR *header, int headersz )
{
	byte	*des ;


	oc->b.tx.usedSize		= 5 + headersz ;
	des			= (byte*)oc->b.tx.buf ;
	*des++		= 0x85 ;						
												
	des			+= MiscSetBigEndian16( des, oc->b.tx.usedSize ) ;
	*des++		= flag ;						
	*des++		= 0 ;							
	IrDA_memcpy( des, header, headersz ) ;
	des			+= headersz ;
	oc->b.WaitResp		= TRUE ;
	oc->b.RspCallBack	= obexCbResponse ;
	oc->b.tx.lock		= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexMakeAbortReq( OBEXCONFIG *oc, const void IrDAFAR *header, int headersz )
{
	byte	*des ;


	UNREFERENCE( header ) ;


	oc->b.tx.usedSize		= 3 + headersz ;
	des					= (byte*)oc->b.tx.buf ;
	*des++				= 0xFF ;				
	MiscSetBigEndian16( des, oc->b.tx.usedSize ) ;
	oc->b.WaitResp		= TRUE ;
	oc->b.RspCallBack	= obexCbResponse ;
	oc->b.tx.lock		= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)
static	void			obexMakeGetRsp( OBEXBCONFIG *ob, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	byte	*des ;
	word	len ;


	ob->tx.usedSize	= 3 + headersz + 3 + bodysz ;
	len				= ob->tx.usedSize ;
	des				= ob->tx.buf ;
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		*des++		= 0x01 ;			
		*des++		= ob->tx.sar ;		
		ob->tx.usedSize	+= 2 ;
	}
#endif	


	
#if 0
	if( fb )
		rescd	|= OBEX_FINALBIT ;		
#else
	rescd	|= OBEX_FINALBIT ;		
#endif
	*des++		= rescd ;
	MiscSetBigEndian16( des, len ) ;
	des			+= 2 ;
	IrDA_memcpy( des, header, headersz ) ;
	des			+= headersz ;


	if( fb )
		*des++		= OBEXHI_ENDBODY ;
	else
		*des++		= OBEXHI_BODY ;
	MiscSetBigEndian16( des, bodysz + 3 ) ;
	des		+= 2 ;
	IrDA_memcpy( des, body, bodysz ) ;
	ob->tx.lock	= FALSE ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexSendBadReqRsp( OBEXBCONFIG *ob )
{
#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexSendBadReqRsp():" ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP )
		return ;
#endif	


	ob->tx.lock			= TRUE ;
	ob->tx.usedSize		= 3 ;
	ob->tx.buf[0]		= OBEXRC_BADREQUEST ;
	MiscSetBigEndian16( &ob->tx.buf[1], 3 ) ;
	ob->tx.lock			= FALSE ;
	obexSendPacket( ob ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int				obexSendPacket( OBEXBCONFIG *ob )
{
	if( ob->tx.usedSize == 0 )
		return( IrDAR_ACCEPT ) ;


#if	defined(DMSG)
{	int		i ;
	_DPUTS( DL_USERINFO, "OBEX::obexSendPacket():" ) ;
	_DPUTW( DL_USERINFO, " size:", ob->tx.usedSize ) ;
	if( ob->tx.usedSize > 0 ) {
		_DPUTB( DL_USERINFO, " data::", ob->tx.buf[0] ) ;
		for( i=1 ; i<ob->tx.usedSize ; i++ )
			_DPUTB( DL_USERINFO, ".", ob->tx.buf[i] ) ;
	}
	_DPUTS( DL_USERINFO, "\n" ) ;
}
#endif	
#if !defined(IrOBEXNOULTRA)
	if( ob->lsap == CLESSLSAP ) {
		int		ret ;
		ret		= IrLMP_CLessDataReq( ob->tx.buf, ob->tx.usedSize ) ;
		ob->tx.usedSize	= 0 ;
		return( ret ) ;
	}
#endif	


	if( TTP_DataReq( ob->lsap, ob->tx.buf, ob->tx.usedSize ) == IrDAR_ACCEPT )
		ob->tx.usedSize		= 0 ;


	return( IrDAR_ACCEPT ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCbConnectRsp( OBEXBCONFIG *ob )
{
	uint		rescd ;
	int			headersz ;
	OBEXCONFIG	*oc ;


#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCbConnectRsp():" ) ;
	_DPUTW( DL_USERINFO, " ob->rx.usedSize:", ob->rx.usedSize ) ;
	_DPUTS( DL_USERINFO, " data:" ) ;
	_DDUMP( DL_USERINFO, ob->rx.buf, ob->rx.usedSize, 0 ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
#if !defined(IrOBEXNOULTRA)
	
	if( ob->lsap == CLESSLSAP )	return ;
#endif	
	oc			= (OBEXCONFIG*)ob ;
	rescd			= (uint)oc->b.rx.buf[0] ;	
#if 0	
												
	MiscGetBigEndian16( &oc->b.rx.buf[1], &packetlen ) ;
#endif
	oc->PeerObexVer	= oc->b.rx.buf[3] ;			
	obexGetMaxTxLength( oc, (byte IrDAFAR*)&oc->b.rx.buf[5] ) ;


	if( rescd == (OBEXRC_OK+OBEX_FINALBIT)) {
		oc->b.rx.packetLength	= 0 ;
		oc->of.Connected		= TRUE ;
	}


	if(( headersz = oc->b.rx.usedSize - 7 ) < 0 )
		headersz	= 0 ;


#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCbConnectRsp(): " ) ;
	_DPUTW( DL_USERINFO, " lsap:", oc->b.lsap ) ;
	_DPUTW( DL_USERINFO, " rescd:", rescd ) ;
	_DPUTW( DL_USERINFO, " max:", oc->b.tx.maxPacketLength ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	obexCallConnectRsp( oc, rescd, &oc->b.rx.buf[7], headersz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void			obexCbResponse( OBEXBCONFIG *ob )
{
	uint		rescd ;
	int			headersz ;
	word		bodysz ;


#if	defined(DMSG)
	_DPUTS( DL_USERINFO, "OBEX::obexCbResponse():" ) ;
	_DPUTW( DL_USERINFO, " lsap:", ob->lsap ) ;
	_DPUTW( DL_USERINFO, " size:", ob->rx.usedSize ) ;
	_DPUTS( DL_USERINFO, " data:" ) ;
	_DDUMP( DL_USERINFO, ob->rx.buf, ob->rx.usedSize, 0 ) ;
	_DPUTS( DL_USERINFO, "\n" ) ;
#endif	
	rescd		= ob->rx.buf[0] ;
	headersz	= obexScanBody( &ob->rx.buf[3], ob->rx.usedSize - 3 ) ;
	bodysz		= 0 ;


	if( ob->rx.usedSize - 3 > headersz ) {
		MiscGetBigEndian16( &ob->rx.buf[3+headersz+1], &bodysz ) ;
		bodysz	-= 3 ;
	}


	obexCallResponse( ob, rescd, &ob->rx.buf[3], headersz, &ob->rx.buf[3+headersz+3], bodysz ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	int		IrDACB	obexCbTTPConnectInd( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd )
{
	OBEXCONFIG	*oc ;


	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;


	if(( oc = obexMakeObexConfigPtr( slsap )) == 0 )
		return( IrDA_CONNECTDI ) ;
	if( !oc->of.Used )
		return( IrDA_CONNECTDI ) ;
	return( IrDA_CONNECTEN ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	obexCbTTPConnectCnf( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status )
{
	OBEXCONFIG	*oc ;


	UNREFERENCE( addr ) ;
	UNREFERENCE( dlsap ) ;
	UNREFERENCE( lcd ) ;
	UNREFERENCE( maxsdu ) ;


	if(( oc = obexMakeObexConfigPtr( slsap )) == 0 )
		return ;
	if( !oc->of.Used )
		return ;


	if(( oc->Mode & OBEX_WAITTTPCONNRSP ) == 0 )
		return ;
	oc->Mode	&= ~OBEX_WAITTTPCONNRSP ;


	if( status != IrDAR_CONNECTING ) {
		obexCallConnectRsp( oc, status, 0, 0 ) ;
		return ;
	}
	obexMakeConnectReq( oc ) ;
	obexSendConnectReq( oc ) ;
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	obexCbTTPDisconnectInd( int lsap, int reason )
{
	OBEXCONFIG	*oc ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return ;
	if( !oc->of.Used || !oc->of.Connected )
		return ;


	if( oc->of.Connected ) {
		oc->of.Connected	= FALSE ;
		obexCallDisconnectReq( oc, reason, 0, 0 ) ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	obexCbTTPDataInd( int lsap )
{
	OBEXCONFIG			*oc ;
	const byte	IrDAFAR *ptr ;
	uint				datasz ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return ;
	if( !oc->of.Used )
		return ;


	for(;;) {
		if(( datasz = TTP_GetRxSize( lsap )) == 0 )
			break ;
		ptr		= TTP_GetDataPtr( lsap ) ;
#if	defined(DMSG)
		_DPUTS( DL_USERINFO, "OBEX::obexCbTTPDataInd(): " ) ;
		_DPUTW( DL_USERINFO, " read from TTP: size:", datasz ) ;
		_DPUTS( DL_USERINFO, "\n" ) ;
#endif	


		obexBuildPacket((OBEXBCONFIG*)oc, ptr, datasz ) ;


		TTP_ReleaseRxQueue( lsap ) ;
	}
}
#endif	


#if !defined(IrDANOSTATIC)


static	void	IrDACB	obexCbTTPDataCnf( int lsap )
{
	OBEXCONFIG	*oc ;


	if(( oc = obexMakeObexConfigPtr( lsap )) == 0 )
		return ;
	if( !oc->of.Used || oc->b.tx.lock )
		return ;


	if( oc->Mode & OBEX_WAITCONNREQ ) {
		obexSendConnectReq( oc ) ;
		return ;
	}


	if( oc->b.tx.usedSize > 0 )
		obexSendPacket((OBEXBCONFIG*)oc ) ;
}
#endif	


#if !defined(IrOBEXNOULTRA)
# if !defined(IrDANOSTATIC)
static	void	IrDACB	obexCbLMDataInd( const void IrDAFAR* data, int datasz )
{
	const byte IrDAFAR	*p ;


	p		= data ;
	if(( *p & 0x01 ) == 0 )
		return ;


	for( ; ( *p & 0x80 ) && datasz > 0 ; p++, datasz -- ) ;
	p	++ ;
	datasz	-- ;
	if( datasz <= 1 )
		return ;


	ultraConfig.rx.sar	= *p++ ;
	datasz				-- ;
	obexBuildPacket( &ultraConfig, p, datasz ) ;
}
# endif	
#endif	


#if !defined(IrOBEXNOULTRA)
# if !defined(IrDANOSTATIC)
static	void	IrDACB	obexCbLMDataCnf( void )
{
}
# endif	
#endif	


#if !defined(IrOBEXNOULTRA)


void	IrDAAPI	IrUOBEX_SetPutReqInd( void (IrDACB *func)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ))
{
	obexPutReqIndPtr	= func ;
}
#endif	


#if !defined(IrOBEXNOULTRA)


void	IrDAAPI	IrUOBEX_SetGetReqInd( void (IrDACB *func)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz ))
{
	obexGetReqIndPtr	= func ;
}
#endif	


#if !defined(IrOBEXNOULTRA)


int		IrDAAPI	IrUOBEX_SendGetRsp( byte sar, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	int				ret ;


	if(( ret = obexTasTxBuf( &ultraConfig )) != IrDAR_ACCEPT )
		return( ret ) ;


	ultraConfig.tx.sar		= sar ;
	obexMakeGetRsp( &ultraConfig, rescd, fb, header, headersz, body, bodysz ) ;
	return( obexSendPacket( &ultraConfig )) ;
}
#endif	


#if !defined(IrOBEXNOULTRA)


void	IrDAAPI	IrUOBEX_SetRspInd( void (IrDACB *func)( byte sar, int rescd, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ))
{
	obexRspIndPtr	= func ;
}
#endif	


#if !defined(IrOBEXNOULTRA)


int		IrDAAPI	IrUOBEX_PutReq( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	int		ret ;


	if(( ret = obexTasTxBuf( &ultraConfig )) != IrDAR_ACCEPT )
		return( ret ) ;


	ultraConfig.tx.sar	= sar ;
	obexMakePutReq( &ultraConfig, fb, header, headersz, body, bodysz ) ;
	return( obexSendPacket( &ultraConfig )) ;
}
#endif	


#if !defined(IrOBEXNOULTRA)


int		IrDAAPI	IrUOBEX_GetReq( byte sar, const void IrDAFAR *header, int headersz )
{
	int		ret ;


	if(( ret = obexTasTxBuf( &ultraConfig )) != IrDAR_ACCEPT )
		return( ret ) ;


	ultraConfig.tx.sar	= sar ;
	obexMakeGetReq( &ultraConfig, header, headersz ) ;
	return( obexSendPacket( &ultraConfig )) ;
}
#endif	


#endif	
