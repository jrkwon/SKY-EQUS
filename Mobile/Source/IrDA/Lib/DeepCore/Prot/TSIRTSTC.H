


#if !defined(__tsirtstc_h__)
#define		__tsirtstc_h__


#define		SIRTEST_CLIENT
#include	"tsirtstl.h"


#define	CMD_RETRY		6
#define	DAT_RETRY		20
#define	TIME_RETRY		500L


typedef	struct	{
	ulong		framesz ;							
	ulong		transsz ;							
	ulong		datasz ;							
}	TRNSSIZE ;


static	void			KickNewMintTurn( void ) ;
static	void			SendChgSpd( dword bps ) ;
static	void			SendQueryTxSize( void ) ;
static	void			SendQueryRxSize( void ) ;
static	void			SendTxReq( uint frmsz, uint winsz ) ;
static	void			SendData( uint frmsz, uint winsz ) ;
static	void			SendEcho( uint datasz ) ;
static	void			DisplayTransRate( void ) ;


static	uint			TargetFrameSize ;
static	uint			TargetWindowSize ;
static	uint			RxDataSize ;
static	uint			TxDataSize ;
static	uint			DataCount ;
static	uint			CmdRetryCount ;
static	uint			InitialDataSize ;
static	uint			MinTurnTime ;


static	ulong			TestTime ;
#define	TestBaud		TargetSpeed
static	TRNSSIZE		Receive ;
static	TRNSSIZE		Transmit ;
static	ulong			TxCount ;
static	ulong			RxCount ;


static	void			DI_Null( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_Abort( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_TxFrmChgSpeedEn( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_TxFrmChgSpeedDi( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_TxFrmQueryTxSize( byte IrDAFAR *frame, uint framesz ) ;
static	void			DI_TxFrmData( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_Null[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Null,			DI_Null,				DI_Null,		DI_Null,
		DI_Null,			DI_Null,				DI_Null,		DI_Null,
		DI_Null,			DI_Null,				DI_Null		} ;
static	void			(*const RE_TxFrmSpd[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_TxFrmChgSpeedEn,	DI_TxFrmChgSpeedDi,		DI_Abort,		DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,		DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			(*const RE_TxFrmTxSz[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,				DI_Abort,
		DI_Abort,			DI_Abort,				DI_TxFrmQueryTxSize,	DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			(*const RE_TxFrmTxReq[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,		DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,		DI_Abort,
		DI_TxFrmData,		DI_Abort,				DI_Abort	} ;
static	void			DI_RxFrmChgSpeedEn( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxFrmSpd[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_RxFrmChgSpeedEn,	DI_TxFrmChgSpeedDi,		DI_Abort,		DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,		DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_RxFrmQueryRxSize( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxFrmRxSz[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_RxFrmQueryRxSize,	DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_RxFrmData( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxFrmData[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_RxFrmData,		DI_Abort,				DI_Abort	} ;
static	void			DI_MTTChgSpeedEn( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_MTTSpd[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_MTTChgSpeedEn,	DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_MTTChgMTT( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_MTTAck[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_MTTChgMTT,		DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_MTTEchoRsp( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_MTTEcho[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_MTTEchoRsp,			DI_Abort	} ;
static	void			DI_RxRChgSpeedEn( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxRSpd[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_RxRChgSpeedEn,	DI_TxFrmChgSpeedDi,		DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_RxRQueryRxSize( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxRRxSz[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_RxRQueryRxSize,		DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_RxRChgMTT( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxRChgMTT[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_RxRChgMTT,		DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort	} ;
static	void			DI_RxRData( byte IrDAFAR *frame, uint framesz ) ;
static	void			(*const RE_RxRData[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_Abort,			DI_Abort,				DI_Abort,			DI_Abort,
		DI_RxRData,			DI_Abort,				DI_Abort	} ;


static	void	IrDACB	StartRetryTimer( void ) ;
static	void			StopRetryTimer( void ) ;
static	void	IrDACB	RetryFunc( void ) ;
static	void			(*RetrySubFunc)( void ) ;
static	void			RT_TxFrm_ChgSpd( void ) ;
static	void			RT_TxFrm_TxSize( void ) ;
static	void			RT_TxFrm_TxReq( void ) ;
#define	RT_RxFrm_ChgSpd	RT_TxFrm_ChgSpd
static	void			RT_RxFrm_RxSize( void ) ;
static	void			RT_RxFrm_Data( void ) ;
#define	RT_MTT_ChgSpd	RT_TxFrm_ChgSpd
static	void			RT_MTT_Ack( void ) ;
static	void			RT_MTT_Echo( void ) ;
#define	RT_RxR_ChgSpd	RT_TxFrm_ChgSpd
#define	RT_RxR_RxSize	RT_RxFrm_RxSize
#define	RT_RxR_Data		RT_RxFrm_Data ;
static	void			RT_RxR_ChgMTT( void ) ;


static	int				TestTimerHndl ;
static	void	IrDACB	TestTimerExpired( void ) ;
static	int				CountDownTime ;
static	int				CountDownTimerHndl ;
static	void	IrDACB	CountDownTimeOut( void ) ;


int		st_chktxframe( dword bps, uint datasz )
{
	st_init() ;
	TargetSpeed		= bps ;
	InitialDataSize	= datasz ;
	RecvEvent		= RE_TxFrmSpd ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_TxFrm_ChgSpd ;
	SendChgSpd( TargetSpeed ) ;


	for( ; st_chkabort() == FALSE && ( SystemFlag & SF_Abort ) == 0 ; )
		LogDisplay() ;


	st_term() ;
	return( STR_DONE ) ;
}


int		st_chkrxframe( dword bps, uint datasz )
{
	st_init() ;
	TargetSpeed		= bps ;
	InitialDataSize	= datasz ;
	RecvEvent		= RE_RxFrmSpd ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_RxFrm_ChgSpd ;
	SendChgSpd( TargetSpeed ) ;


	for( ; st_chkabort() == FALSE && ( SystemFlag & SF_Abort ) == 0 ; )
		LogDisplay() ;
	st_term() ;
	return( STR_DONE ) ;
}


int		st_chkmtt( dword bps, uint datasz )
{
	st_init() ;
	TargetSpeed		= bps ;
	InitialDataSize	= datasz ;
	RecvEvent		= RE_MTTSpd ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_MTT_ChgSpd ;
	SendChgSpd( TargetSpeed ) ;


	for( ; st_chkabort() == FALSE && ( SystemFlag & ( SF_Abort | SF_Done )) == 0 ; )
		LogDisplay() ;
	st_term() ;
	return( STR_DONE ) ;
}


int		st_rxrate( dword bps, uint datasz, uint sec )
{
	st_init() ;
	SystemFlag		&= ~SF_Verbose ;
	CountDownTimerHndl	= -1 ;
	TestTimerHndl		= -1 ;
	TargetSpeed		= bps ;
	InitialDataSize	= datasz ;
	CountDownTime	= sec ;
	TestTime		= (ulong)sec ;
	RecvEvent		= RE_RxRSpd ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_RxR_ChgSpd ;
	SendChgSpd( TargetSpeed ) ;


	LogPrintf( "RxRATE(): TIME:%u\n", sec ) ;
	for( ; st_chkabort() == FALSE && ( SystemFlag & ( SF_Abort | SF_Done )) == 0 ; )
		LogDisplay() ;


	if( TestTimerHndl >= 0 )
		StopTimer( TestTimerHndl ) ;
	if( CountDownTimerHndl >= 0 )
		StopTimer( CountDownTimerHndl ) ;


	if( SystemFlag & SF_Done )
		DisplayTransRate() ;


	st_term() ;
	return( STR_DONE ) ;
}


int		st_txrate( dword bps, uint datasz, uint sec )
{
	UNREFERENCE( bps ) ;
	UNREFERENCE( datasz ) ;
	UNREFERENCE( sec ) ;


	return( STR_DONE ) ;
}


int		st_echorate( dword bps, uint datasz, uint sec )
{
	return( STR_DONE ) ;
}


static	void			KickNewMintTurn( void )
{
	RecvEvent		= RE_MTTAck ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_MTT_Ack ;
	SendAck( StartRetryTimer ) ;
}


static	void			SendChgSpd( dword bps )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_CHGSPD ;
	MiscSetBigEndian32( &TxBuffer[2], bps ) ;
	StopRetryTimer() ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	LogPrintf( "Send CHGSPD\n" ) ;
	IrSIR_SendFrame( 6, TxBuffer ) ;
}


static	void			SendQueryTxSize( void )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_QUERYTXSIZE ;
	StopRetryTimer() ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	LogPrintf( "Send QUERYTXFRM\n" ) ;
	IrSIR_SendFrame( 2, TxBuffer ) ;
}


static	void			SendQueryRxSize( void )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_QUERYRXSIZE ;
	StopRetryTimer() ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	LogPrintf( "Send QUERYRXFRM\n" ) ;
	IrSIR_SendFrame( 2, TxBuffer ) ;
}


static	void			SendTxReq( uint frmsz, uint winsz )
{
	StopRetryTimer() ;
	if( DataCount == 0 && CmdRetryCount == 0 )
		LogPrintf( "\rSend TXREQ (%u/%u) ", frmsz, winsz ) ;


	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_TXREQ ;
	MiscSetBigEndian16( &TxBuffer[2], frmsz ) ;
	MiscSetBigEndian16( &TxBuffer[4], winsz ) ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	IrSIR_SendFrame( 6, TxBuffer ) ;
}


static	void			SendData( uint frmsz, uint winsz )
{
	int		i ;


	StopRetryTimer() ;
	if( DataCount == 0 && CmdRetryCount == 0 && ( SystemFlag & SF_Verbose ))
		LogPrintf( "Send DATA (%u/%u)\n", frmsz, winsz ) ;


	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_DATA ;
	for( i=0 ; i<frmsz ; i++ )
		TxBuffer[2+i]	= i ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	IrSIR_SendFrame( 2+frmsz, TxBuffer ) ;
}


static	void			SendEcho( uint datasz )
{
	int		i ;


	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_ECHO ;
	for( i=0 ; i<datasz ; i++ )
		TxBuffer[2+i]	= i & 0xFF ;
	IrSIR_SetDataCnf( StartRetryTimer ) ;
	IrSIR_SendFrame( 2+datasz, TxBuffer ) ;
}


static	void			DI_Null( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
}


static	void			DI_Abort( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	LogPrintf( "Received Ignore Command (0x%02X)\n", frame[1] ) ;
	SystemFlag	|= SF_Abort ;
}


static	void			DI_TxFrmChgSpeedEn( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	StopRetryTimer() ;
	RecvEvent		= RE_TxFrmTxSz ;
	SetSpeed( TargetSpeed ) ;
	CmdRetryCount		= 0 ;
	RetrySubFunc	= RT_TxFrm_TxSize ;
	SendQueryTxSize() ;
}


static	void			DI_TxFrmChgSpeedDi( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	LogPrintf( "Server is not supported %ldbps\n", TargetSpeed ) ;
	SystemFlag	|= SF_Abort ;
}


static	void			DI_TxFrmQueryTxSize( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	MiscGetBigEndian16( &frame[2], (word IrDAFAR *)&TargetFrameSize ) ;
	MiscGetBigEndian16( &frame[4], (word IrDAFAR *)&TargetWindowSize ) ;
	LogPrintf( "Server Tx %d/%d\n", TargetFrameSize, TargetWindowSize ) ;


	if( InitialDataSize > TargetFrameSize )
		InitialDataSize	= TargetFrameSize ;


	RecvEvent		= RE_TxFrmTxReq ;
	RxDataSize		= InitialDataSize ;
	CmdRetryCount	= 0 ;
	DataCount		= 0 ;
	RetrySubFunc	= RT_TxFrm_TxReq ;
	SendTxReq( RxDataSize, 1 ) ;
}


static	void			DI_TxFrmData( byte IrDAFAR *frame, uint framesz )
{
	int		i ;


	UNREFERENCE( frame ) ;


	if( framesz - 2 != RxDataSize ) {
		LogPrintf( "Requested %d bytes, but received %d bytes\n", RxDataSize, framesz ) ;
		LogPuts( "Last received data :\n" ) ;
		for( i=0 ; i<framesz ; i++ )
			LogPrintf( " %02X", frame[i] ) ;
		LogPuts( "\n" ) ;
		SystemFlag	|= SF_Abort ;
		return ;
	}


	DataCount	++ ;
	if( DataCount > DAT_RETRY ) {
		DataCount		= 0 ;
		RxDataSize		++ ;
		if( RxDataSize > RxFrameSize || RxDataSize > TargetFrameSize )
			RxDataSize	= 0 ;
	}
	CmdRetryCount		= 0 ;
	RetrySubFunc	= RT_TxFrm_TxReq ;
	SendTxReq( RxDataSize, 1 ) ;
}


static	void			DI_RxFrmChgSpeedEn( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;


	StopRetryTimer() ;
	RecvEvent		= RE_RxFrmRxSz ;
	CmdRetryCount		= 0 ;
	RetrySubFunc	= RT_RxFrm_RxSize ;
	SetSpeed( TargetSpeed ) ;
	SendQueryRxSize() ;
}


static	void			DI_RxFrmQueryRxSize( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( framesz ) ;


	MiscGetBigEndian16( &frame[2], (word IrDAFAR *)&TargetFrameSize ) ;
	MiscGetBigEndian16( &frame[4], (word IrDAFAR *)&TargetWindowSize ) ;
	LogPrintf( "Server MaxRx %d/%d\n", TargetFrameSize, TargetWindowSize ) ;


	if( InitialDataSize > TargetFrameSize )
		InitialDataSize	= TargetFrameSize ;


	RecvEvent		= RE_RxFrmData ;
	TxDataSize		= InitialDataSize ;
	CmdRetryCount	= 0 ;
	DataCount		= 0 ;
	RetrySubFunc	= RT_RxFrm_Data ;
	SendData( TxDataSize, 1 ) ;
}


static	void			DI_RxFrmData( byte IrDAFAR *frame, uint framesz )
{
	uint		recvsz ;


	MiscGetBigEndian16( &frame[2], (word IrDAFAR *)&recvsz ) ;
	if( recvsz != TxDataSize ) {
		LogPrintf( "Send %d bytes, but received %d bytes\n", TxDataSize, recvsz ) ;
		SystemFlag	|= SF_Abort ;
		return ;
	}


	if( DataCount )
		LogPutc( '.' ) ;


	DataCount	++ ;
	if( DataCount > DAT_RETRY ) {
		DataCount		= 0 ;
		TxDataSize		++ ;
		if( TxDataSize > TxFrameSize || TxDataSize > TargetFrameSize )
			TxDataSize	= 1 ;
	}
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_RxFrm_Data ;
	SendData( TxDataSize, 1 ) ;
}


static	void			DI_MTTChgSpeedEn( byte IrDAFAR *frame, uint framesz )
{
	StopRetryTimer() ;
	SetSpeed( TargetSpeed ) ;
	MinTurnTime		= 20 ;
	KickNewMintTurn() ;
}


static	void			DI_MTTChgMTT( byte IrDAFAR *frame, uint framesz )
{
	StopRetryTimer() ;
	IrSIR_SetMinimamTurnaroundTime( MinTurnTime ) ;
	LogPrintf( "Set MinimumTurnaroundTime %umsec\n", MinTurnTime ) ;
	RecvEvent		= RE_MTTEcho ;
	CmdRetryCount	= 0 ;
	DataCount		= 0 ;
	RetrySubFunc	= RT_MTT_Echo ;
	SendEcho( InitialDataSize ) ;
}


static	void			DI_MTTEchoRsp( byte IrDAFAR *frame, uint framesz )
{
	StopRetryTimer() ;
	DataCount	++ ;
	if( DataCount > DAT_RETRY ) {
		if( MinTurnTime == 0 ) {
			SystemFlag	|= SF_Done ;
			return ;
		}


		MinTurnTime		-- ;
		KickNewMintTurn() ;
		return ;
	}
	CmdRetryCount	= 0 ;
	SendEcho( InitialDataSize ) ;
}


static	void			DI_RxRChgSpeedEn( byte IrDAFAR *frame, uint framesz )
{
	StopRetryTimer() ;
	RecvEvent		= RE_RxRRxSz ;
	CmdRetryCount		= 0 ;
	RetrySubFunc	= RT_RxR_RxSize ;
	SetSpeed( TargetSpeed ) ;
	SendQueryRxSize() ;
}


static	void			DI_RxRQueryRxSize( byte IrDAFAR *frame, uint framesz )
{
	StopRetryTimer() ;
	MiscGetBigEndian16( &frame[2], (word IrDAFAR *)&TargetFrameSize ) ;
	MiscGetBigEndian16( &frame[4], (word IrDAFAR *)&TargetWindowSize ) ;
	LogPrintf( "Server Rx %d/%d", TargetFrameSize, TargetWindowSize ) ;
	LogPrintf( "  Client Tx %d/%d", TxFrameSize, TxWindowSize ) ;


	if( TargetFrameSize > TxFrameSize )
		TargetFrameSize	= TxFrameSize ;


	if( TargetWindowSize > TxWindowSize )
		TargetWindowSize	= TxWindowSize ;


	if( InitialDataSize > TargetFrameSize )
		InitialDataSize	= TargetFrameSize ;


	LogPrintf( "  Test %d/%d\n", InitialDataSize, TargetWindowSize ) ;


	RecvEvent		= RE_RxRChgMTT ;
	CmdRetryCount		= 0 ;
	RetrySubFunc	= RT_RxR_ChgMTT ;
	SendChgMTT( StartRetryTimer, IrLAP_MinTurn_0ms ) ;
}


static	void			DI_RxRChgMTT( byte IrDAFAR *frame, uint framesz )
{
	word		tmpw ;
	word		mtt ;


	UNREFERENCE( framesz ) ;


	StopRetryTimer() ;
	MiscGetBigEndian16( &frame[2], &tmpw ) ;
	if( tmpw & (IrLAP_MinTurn_0ms|IrLAP_MinTurn_001ms|IrLAP_MinTurn_005ms|IrLAP_MinTurn_01ms|IrLAP_MinTurn_05ms))
		mtt		= 0 ;
	else if( tmpw & IrLAP_MinTurn_1ms )
		mtt		= 1 ;
	else if( tmpw & IrLAP_MinTurn_5ms )
		mtt		= 5 ;
	else
		mtt		= 10 ;
	LogPrintf( "recv ChgMTT, request %ums\n", mtt ) ;
	IrSIR_SetMinimamTurnaroundTime( mtt ) ;


	Receive.framesz		= 0 ;
	Receive.transsz		= 0 ;
	Receive.datasz		= 0 ;
	Transmit.framesz	= 0 ;
	Transmit.transsz	= 0 ;
	Transmit.datasz		= 0 ;
	RecvEvent		= RE_RxRData ;
	TxDataSize		= InitialDataSize ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_RxR_Data ;
	TestTimerHndl		= StartTimer( TestTime * 1000L, TestTimerExpired ) ;
	CountDownTimerHndl	= StartTimer( 1000L, CountDownTimeOut ) ;
	SendData( TxDataSize, 1 ) ;
}


static	void			DI_RxRData( byte IrDAFAR *frame, uint framesz )
{
	uint		recvsz ;


	MiscGetBigEndian16( &frame[2], (word IrDAFAR *)&recvsz ) ;
	if( recvsz != TxDataSize ) {
		LogPrintf( "Send %d bytes, but received %d bytes\n", TxDataSize, recvsz ) ;
		SystemFlag	|= SF_Abort ;
		return ;
	}


	Transmit.transsz += recvsz ;
	Receive.transsz	+= framesz ;
	CmdRetryCount	= 0 ;
	RetrySubFunc	= RT_RxR_Data ;
	SendData( TxDataSize, 1 ) ;
}


static	void			StopRetryTimer( void )
{
	if( IdleTimer >= 0 ) {
		StopTimer( IdleTimer ) ;
		IdleTimer	= -1 ;
	}
}


static	void	IrDACB	StartRetryTimer( void )
{


	if( IdleTimer >= 0 )
		StopTimer( IdleTimer ) ;


	IdleTimer	= StartTimer( TIME_RETRY, RetryFunc ) ;
}


static	void	IrDACB	RetryFunc( void )
{
	IdleTimer	= -1 ;
	if( CmdRetryCount >= CMD_RETRY ) {
		LogPrintf( "Command Retry Over\n" ) ;
		SystemFlag	|= SF_Abort ;
		return ;
	}


	CmdRetryCount	++ ;
	(*RetrySubFunc)() ;
}


static	void			RT_TxFrm_ChgSpd( void )
{
	LogPrintf( "retry change speed command.\n" ) ;
	SendChgSpd( TargetSpeed ) ;
}


static	void			RT_TxFrm_TxSize( void )
{
	LogPrintf( "retry query tx size command.\n" ) ;
	SendQueryTxSize() ;
}


static	void			RT_TxFrm_TxReq( void )
{
	LogPrintf( "retry tx data command.\n" ) ;
	SendTxReq( RxDataSize, 1 ) ;
}


static	void			RT_RxFrm_RxSize( void )
{
	LogPrintf( "retry query rx size command.\n" ) ;
	SendQueryRxSize() ;
}


static	void			RT_RxFrm_Data( void )
{
	LogPrintf( "retry data.\n" ) ;
	SendData( TxDataSize, 1 ) ;
}


static	void	IrDACB	TestTimerExpired( void )
{
	TestTimerHndl	= -1 ;
	SystemFlag		|= SF_Done ;
	RecvEvent		= RE_Null ;
	StopRetryTimer() ;
	LogPrintf( "Expired\n" ) ;
}


static	void	IrDACB	CountDownTimeOut( void )
{
	CountDownTime	-- ;
	LogPrintf( "%5u\r", CountDownTime ) ;
	if( CountDownTime > 0 ) {
		CountDownTimerHndl	= StartTimer( 1000L, CountDownTimeOut ) ;
		return ;
	}
	LogPrintf( "Expired\n" ) ;
	CountDownTimerHndl	= -1 ;
}


static	void			RT_MTT_Ack( void )
{
	LogPrintf( "retry Ack.\n" ) ;
	SendAck( StartRetryTimer ) ;
}


static	void			RT_MTT_Echo( void )
{
	LogPrintf( "retry Echo.\n" ) ;
	SendEcho( InitialDataSize ) ;
}


static	void			RT_RxR_ChgMTT( void )
{
	LogPrintf( "retry ChgMTT" ) ;
	SendChgMTT( StartRetryTimer, IrLAP_MinTurn_0ms ) ;
}


static	void			DisplayTransRate( void )
{
	ulong		ratecps, ratebps ;


	ratecps		= Receive.transsz / TestTime ;
	if( TestBaud <= 115200 )
		ratebps	= ratecps * 10L ;
	else
		ratebps	= ratecps * 8L ;
	LogPrintf( "Transmited: %7luframe %10luBytes, %7lucps(%7lubps) (%2lu.%02lu%%)\n", Receive.framesz, Receive.transsz, ratecps, ratebps, (ratebps * 100L) / TestBaud, (ratebps * 100L * 100L ) / TestBaud % 100L ) ;


	ratecps		= (ulong)Transmit.transsz / TestTime ;
	if( TestBaud <= 115200 )
		ratebps	= ratecps * 10L ;
	else
		ratebps	= ratecps * 8L ;
	LogPrintf( "Received:   %7luframe %10luBytes, %7lucps(%7lubps) (%2lu.%02lu%%)\n", Transmit.framesz, Transmit.transsz, ratecps, ratebps, (ratebps * 100L) / TestBaud, (ratebps * 100L * 100L ) / TestBaud % 100L ) ;


	ratecps		= (ulong)( Receive.transsz + Transmit.transsz ) / TestTime ;
	if( TestBaud <= 115200 )
		ratebps	= ratecps * 10L ;
	else
		ratebps	= ratecps * 8L ;
	LogPrintf( "Transfer:   %7luframe %10luBytes, %7lucps(%7lubps) (%2lu.%02lu%%)\n", Receive.framesz + Transmit.framesz, Receive.transsz + Transmit.transsz, ratecps, ratebps, (ratebps * 100L) / TestBaud, (ratebps * 100L * 100L ) / TestBaud % 100L ) ;
}


#endif	
