


#if !defined(__tsirtsts_h__)
#define		__tsirtsts_h__


#include	"tsirtstl.h"


#define	TIME_RESUME		(3*1000L)


static	void			ReadCapability( void ) ;
static	void			SendTxSize( void IrDACB (*cnf)( void )) ;
static	void			SendRxSize( void IrDACB (*cnf)( void )) ;
static	void			SendDataAck( void IrDACB (*cnf)( void ), uint datasz ) ;
static	void			SendEchoRsp( void IrDAFAR *data, uint datasz ) ;
static	IrDABOOL			ChkSpeed( dword speed ) ;
static	void	IrDACB	ChangeSpeed( void ) ;
static	void	IrDACB	CB_ResetResumeTimer( void ) ;
static	void	IrDACB	CB_Resume( void ) ;


#if defined(SVRKURUKURU)
static	void	IrDACB	CB_SvrTick( void ) ;
#endif	


static	void			DI_SendAck( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_ChgSpd( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_ChgMTT( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_QueryRxSize( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_QueryTxSize( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_TxReq( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_Data( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_SVR_Echo( byte IrDAFAR *frame, uint framesz ) ;


static	void			(*const RE_Server[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_SendAck,			DI_SendNak,			DI_SendNak,			DI_SVR_ChgSpd,
		DI_SVR_ChgMTT,		DI_SVR_QueryRxSize,	DI_SVR_QueryTxSize,	DI_SVR_TxReq,
		DI_SVR_Data,		DI_SVR_Echo,		DI_SendNak	} ;


static	int		TickCharPtr ;
static	PORTCONFIG2		Capability ;
#if defined(SVRKURUKURU)
static	int				TickTimer ;
#endif	


void	st_startsvr( void )
{
	TickCharPtr	= 0 ;
	st_init() ;
	ReadCapability() ;
	CB_ResetResumeTimer() ;


#if defined(SVRKURUKURU)
	TickTimer	= StartTimer( 250L, CB_SvrTick ) ;
#endif	


	RecvEvent	= RE_Server ;
	for( ; st_chkabort() == FALSE ; )
		LogDisplay() ;


#if defined(SVRKURUKURU)
	if( TickTimer >= 0 )
		StopTimer( TickTimer ) ;
#endif	
	st_term() ;
}


static	void			ReadCapability( void )
{
	const byte	IrDAFAR* ptr ;
	int				len ;
	byte			pi, pl ;
	word			dummy ;
	word			*des ;


	Capability.LineCapability		= 0xFF ;
	Capability.MaxTanaroundTime		= 0xFF ;
	Capability.FrameSize			= 0xFF ;
	Capability.WindowSize			= 0xFF ;
	Capability.NoofBOF				= 0xFF ;
	Capability.MinTurnaroundTime	= 0xFF ;
	Capability.DisconnectTime		= 0xFF ;


	len		= (int)IrSIR_GetCapability((const void IrDAFAR * IrDAFAR *)&ptr ) ;


	for( ; len > 0 ; ) {
		pi		= *ptr++ ;
		pl		= *ptr++ ;


		switch( BYTE2INT( pi )) {
		case	0x01 :								
			des		= &Capability.LineCapability ;
			break ;
		case	0x82 :								
			des		= &Capability.MaxTanaroundTime ;
			break ;
		case	0x83 :								
			des		= &Capability.FrameSize ;
			break ;
		case	0x84 :								
			des		= &Capability.WindowSize ;
			break ;
		case	0x85 :								
			des		= &Capability.NoofBOF ;
			break ;
		case	0x86 :								
			des		= &Capability.MinTurnaroundTime ;
			break ;
		case	0x08 :								
			des		= &Capability.DisconnectTime ;
			break ;
		default :
			des		= &dummy ;
		}


		*des		= *ptr ;
		if( pl > 1 )
			*des	|= ((word)ptr[1]) << 8 ;


		ptr		+= pl ;
		len		-= 2 + pl ;
	}
}


static	void			SendTxSize( void IrDACB (*cnf)( void ))
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_QUERYTXSIZE ;
	MiscSetBigEndian16( &TxBuffer[2], TxFrameSize ) ;
	MiscSetBigEndian16( &TxBuffer[4], TxWindowSize ) ;
	IrSIR_SetDataCnf( cnf ) ;
	LogPrintf( "Send TxSize\n" ) ;
	IrSIR_SendFrame( 6, TxBuffer ) ;
}


static	void			SendRxSize( void IrDACB (*cnf)( void ))
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_QUERYRXSIZE ;
	MiscSetBigEndian16( &TxBuffer[2], RxFrameSize ) ;
	MiscSetBigEndian16( &TxBuffer[4], RxWindowSize ) ;
	IrSIR_SetDataCnf( cnf ) ;
	LogPrintf( "Send RxSize\n" ) ;
	IrSIR_SendFrame( 6, TxBuffer ) ;
}


static	void			SendDataAck( void IrDACB (*cnf)( void ), uint datasz )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_DATA ;
	MiscSetBigEndian16( &TxBuffer[2], datasz ) ;
	IrSIR_SetDataCnf( cnf ) ;


	IrSIR_SendFrame( 4, TxBuffer ) ;
}


static	void			SendEchoRsp( void IrDAFAR *data, uint datasz )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_ECHO ;
	IrDA_memcpy( &TxBuffer[2], data, datasz ) ;
	IrSIR_SetDataCnf( CB_ResetResumeTimer ) ;
	IrSIR_SendFrame( 2+datasz, TxBuffer ) ;
}


static	IrDABOOL			ChkSpeed( dword speed )
{
	word			mask ;


	if( speed == 2400L )			mask	= IrLAP_BPS_2400 ;
	else if( speed == 9600L )		mask	= IrLAP_BPS_9600 ;
	else if( speed == 19200L )		mask	= IrLAP_BPS_19200 ;
	else if( speed == 38400L )		mask	= IrLAP_BPS_38400 ;
	else if( speed == 57600L )		mask	= IrLAP_BPS_57600 ;
	else if( speed == 115200L )		mask	= IrLAP_BPS_115200 ;
	else if( speed == 576000L )		mask	= IrLAP_BPS_576000 ;
	else if( speed == 1152000L )	mask	= IrLAP_BPS_1152000 ;
	else if( speed == 4000000L )	mask	= IrLAP_BPS_4000000 ;
	else							mask	= 0 ;
	LogPrintf( "ChkSpeed(): speed:%lu, mask:%04X, pv:%04X\n", speed, mask, Capability.LineCapability ) ;
	if( Capability.LineCapability & mask )
		return( FALSE ) ;
	return( TRUE ) ;
}


static	void	IrDACB	ChangeSpeed( void )
{
	LogPutc( 'c' ) ;
	SetSpeed( TargetSpeed ) ;
	CB_ResetResumeTimer() ;
}


static	void	IrDACB	CB_ResetResumeTimer( void )
{
	if( IdleTimer >= 0 )
		StopTimer( IdleTimer ) ;


	IdleTimer	= StartTimer( TIME_RESUME, CB_Resume ) ;
}


static	void	IrDACB	CB_Resume( void )
{
	IdleTimer	= -1 ;


	SetSpeed( STD_DEFAULTSPD ) ;
	IrSIR_ResetRecvMode() ;
	IrSIR_SetRecvMode() ;
}


#if defined(SVRKURUKURU)
static	void	IrDACB	CB_SvrTick( void )
{
	static	const char	TickChar[]	= "|/-\\" ;


	TickCharPtr	++ ;
	TickCharPtr	&= 3 ;


	LogPutc( TickChar[TickCharPtr] ) ;
	LogPutc( '\x08' ) ;
	TickTimer	= StartTimer( 250L, CB_SvrTick ) ;
}
#endif	


static	void			DI_SendAck( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	SendAck( CB_ResetResumeTimer ) ;
}


static	void			DI_SVR_ChgSpd( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	MiscGetBigEndian32( &frame[2], &TargetSpeed ) ;
	if( ChkSpeed( TargetSpeed )) {
		LogPrintf( "Accept ChgSpd to %lu bps, but I can't support this speed\n", TargetSpeed ) ;
		SendNak( CB_ResetResumeTimer ) ;
	} else {
		SendAck( ChangeSpeed ) ;
	}
}


static	void			DI_SVR_QueryTxSize( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	LogPrintf( "Accept QueryTxSize\n" ) ;
	SendTxSize( CB_ResetResumeTimer ) ;
}


static	void			DI_SVR_ChgMTT( byte IrDAFAR *frame, uint framesz )
{
	word	tmpw ;
	word	mtt ;


	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	MiscGetBigEndian16( &frame[2], &tmpw ) ;
	if( tmpw & (IrLAP_MinTurn_0ms|IrLAP_MinTurn_001ms|IrLAP_MinTurn_005ms|IrLAP_MinTurn_01ms|IrLAP_MinTurn_05ms))
		mtt		= 0 ;
	else if( tmpw & IrLAP_MinTurn_1ms )
		mtt		= 1 ;
	else if( tmpw & IrLAP_MinTurn_5ms )
		mtt		= 5 ;
	else
		mtt		= 10 ;
	LogPrintf( "recv ChgMTT, request %04X, me:%04X\n", mtt, Capability.MinTurnaroundTime ) ;
	IrSIR_SetMinimamTurnaroundTime( mtt ) ;
	SendChgMTT( CB_ResetResumeTimer, Capability.MinTurnaroundTime ) ;
}


static	void			DI_SVR_QueryRxSize( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	LogPrintf( "Accept QueryRxSize\n" ) ;
	SendRxSize( CB_ResetResumeTimer ) ;
}


static	void			DI_SVR_TxReq( byte IrDAFAR *frame, uint framesz )
{
	word		frmsz ;


	UNREFERENCE( framesz ) ;


	MiscGetBigEndian16( &frame[2], &frmsz ) ;


	if((uint)frmsz <= TxFrameSize ) {
		SendTxData( CB_ResetResumeTimer, (uint)frmsz ) ;
	} else {


		LogPrintf( "Accept TxRequest %u bytes, but my TxFrameSize is %u\n", frmsz, TxFrameSize ) ;
		SendNak( CB_ResetResumeTimer ) ;
	}
}


static	void			DI_SVR_Data( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	SendDataAck( CB_ResetResumeTimer, framesz - 2 ) ;
}


static	void			DI_SVR_Echo( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	SendEchoRsp( &frame[2], framesz - 2 ) ;
}


#endif	
