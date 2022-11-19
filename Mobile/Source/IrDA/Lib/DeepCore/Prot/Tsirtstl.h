


#if !defined(__tsirtstl_h__)
#define		__tsirtstl_h__


#include	<stdarg.h>
#include	<stdio.h>
#include	<string.h>
#include	"irbios.h"
#include	"irda.h"
#include	"irsir.h"
#include	"okmisc.h"
#include	"sirtest.h"
#include	"timer.h"


#define	RECVTBLSZ			8
#define	STD_DEFAULTIDLE		1000L
#define	STD_DEFAULTSPD		9600L
#define	STD_DEFAULTCA		0x43
#define	STD_DEFAULTMTT		10
#define	STD_DEFAULTBOF		10


#define	STC_ACK				0
#define	STC_NAK				1
#define	STC_DISCOVERY		2
#define	STC_CHGSPD			3
#define	STC_CHGMTT			4
#define	STC_QUERYRXSIZE		5
#define	STC_QUERYTXSIZE		6
#define	STC_TXREQ			7
#define	STC_DATA			8
#define	STC_ECHO			9
#define	STC_INVALID			10


static	uint			RxFrameSize ;
static	uint			RxWindowSize ;
static	byte	IrDAFAR	*RxBuffer ;
static	uint			TxFrameSize ;
static	uint			TxWindowSize ;
static	byte	IrDAFAR	*TxBuffer ;
volatile byte IrDAFAR	*RecvAddrTbl[RECVTBLSZ] ;	
volatile uint IrDAFAR	RecvSizeTbl[RECVTBLSZ] ;	
volatile uint IrDAFAR	RecvFrameSize ;				
static	uint			DoneFrameSize ;				


static	void			st_init( void ) ;
static	void			st_term( void ) ;
static	void			st_poweron( void ) ;
static	void			st_poweroff( void ) ;
static	void			LogPrintf( const byte *fmt, ... ) ;
static	void			LogPuts( const byte *s ) ;
static	void			LogPutc( byte c ) ;
static	void			LogDisplay( void ) ;
static	void			LogFlush( void ) ;
static	void			SetSpeed( dword bps ) ;
static	void			SendAck( void IrDACB (*cnf)( void )) ;
static	void			SendNak( void IrDACB (*cnf)( void )) ;
static	void			SendTxData( void IrDACB (*cnf)( void ), uint frmsz ) ;
static	void			SendChgMTT( void IrDACB (*cnf)( void ), word mtt ) ;
static	void	IrDACB	CB_DataInd( void ) ;
static	void	IrDACB	CB_Tick( void ) ;


static	byte			LogBuf[LOGSIZE] ;
static	MISCQUEUE			Log ;
static	int				IdleTimer ;
static	void			(**RecvEvent)( byte IrDAFAR *frame, uint framesz ) ;
static	dword			TargetSpeed ;
static	IrDABOOL			PowerMode ;
static	int				TickTimer ;
static	int				TickCount ;


static	void			DI_SendNak( byte IrDAFAR *frame, uint framesz ) ;


static	void			(*const RE_Nak[STC_INVALID+1])( byte IrDAFAR *frame, uint framesz )	= {
		DI_SendNak,			DI_SendNak,			DI_SendNak,		DI_SendNak,
		DI_SendNak,			DI_SendNak,			DI_SendNak,		DI_SendNak,
		DI_SendNak,			DI_SendNak,			DI_SendNak	} ;


#define	SF_Abort		0x01
#define	SF_Done			0x02
#define	SF_Verbose		0x04
static	uint			SystemFlag ;


void	st_setrxfrm( uint rf, uint rw, void IrDAFAR *rp )
{
	RxFrameSize		= rf - 2 ;
	RxWindowSize	= rw ;
	RxBuffer		= (byte IrDAFAR*)rp ;
}


void	st_settxfrm( uint tf, uint tw, void IrDAFAR *tp )
{
	TxFrameSize		= tf - 2 ;
	TxWindowSize	= tw ;
	TxBuffer		= (byte IrDAFAR*)tp ;
}


void	st_chktimer( void )
{
	st_init() ;
	TickCount	= 0 ;


	if(( TickTimer = StartTimer( 1000L, CB_Tick )) == -1 ) {
		LogPuts( "Can't use Timer\n" ) ;
	} else {
		for( ; st_chkabort() == FALSE ; )
			LogDisplay() ;
	}


	if( TickTimer >= 0 )
		StopTimer( TickTimer ) ;
	st_term() ;
}


static	void		st_init( void )
{
	IrSIRFRAMECONFIG	frameconfig ;


	SystemFlag			= SF_Verbose ;
	DoneFrameSize		= 0 ;
	RecvFrameSize		= 0 ;
	RecvEvent			= RE_Nak ;
	IdleTimer			= -1 ;


	MiscInitQueue( &Log, LogBuf, LOGSIZE ) ;


	st_poweron() ;
	frameconfig.RecvBuffer     = RxBuffer ;
	frameconfig.RecvBufferSize = ( RxFrameSize + 2 ) * RxWindowSize ;
	frameconfig.TblSize        = RECVTBLSZ ;
	frameconfig.RecvBufTbl     = RecvAddrTbl ;
	frameconfig.RecvSizeTbl    = RecvSizeTbl ;
	frameconfig.RecvWindow     = &RecvFrameSize ;
	IrSIR_SetFrameBuffer( &frameconfig ) ;


	IrSIR_InitConnectionParameters() ;
	SetSpeed( STD_DEFAULTSPD ) ;
	IrSIR_SetConnectionAddress( STD_DEFAULTCA ) ;
	IrSIR_SetMinimamTurnaroundTime( STD_DEFAULTMTT ) ;
	IrSIR_SetBOF( STD_DEFAULTBOF ) ;
	IrSIR_SetFrameMode() ;
	IrSIR_ResetRecvMode() ;
	IrSIR_SetDataInd( CB_DataInd ) ;
	IrSIR_SetRecvMode() ;
}


static	void		st_term( void )
{
	if( IdleTimer >= 0 )
		StopTimer( IdleTimer ) ;


	IrSIR_SetDataInd( 0 ) ;
	st_poweroff() ;
	LogFlush() ;
}


static	void		st_poweron( void )
{
	PowerMode	= IrDA_GetPowerMode() ;
	IrDA_PowerOn() ;
}


static	void		st_poweroff( void )
{
	if( PowerMode == FALSE )
		IrDA_PowerOff() ;
}


static	void			LogPrintf( const byte *fmt, ... )
{
	static	byte	prtbuf[256] ;
	va_list			varp ;


	va_start( varp, fmt ) ;


	vsprintf((char*)prtbuf, (char*)fmt, varp ) ;
	MiscSetQueue( &Log, prtbuf, strlen( prtbuf )) ;


	va_end( varp ) ;
}


static	void			LogPuts( const byte *s )
{
	MiscSetQueue( &Log, s, strlen( s )) ;
}


static	void			LogPutc( byte c )
{
	MiscSetQueue( &Log, &c, 1 ) ;
}


static	void			LogDisplay( void )
{
	byte	IrDAFAR*	ptr ;
	uint	len ;


	if(( len = MiscGetReadableQueue( &Log, &ptr )) == 0 )
		return ;


	st_put( ptr, len ) ;
	MiscSkipReadQueue( &Log, len ) ;
}


static	void			LogFlush( void )
{
	dword	timer ;


	SetTimer( &timer, 1000L ) ;
	for( ; MiscGetUsedQueueSize( &Log ) && GetTimer( timer ) ; )
		LogDisplay() ;
}


static	void			SetSpeed( dword bps )
{
	LogPrintf( "Change Speed to %lu\n", bps ) ;
	IrSIR_SetSpeed( bps ) ;
	IrSIR_SetRecvMode() ;
}


static	void			DI_SendNak( byte IrDAFAR *frame, uint framesz )
{
	UNREFERENCE( frame ) ;
	UNREFERENCE( framesz ) ;
	SendNak( 0 ) ;
}


static	void			SendAck( void IrDACB (*cnf)( void ))
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_ACK ;


	IrSIR_SetDataCnf( cnf ) ;
	IrSIR_SendFrame( 2, TxBuffer ) ;
}


static	void			SendNak( void IrDACB (*cnf)( void ))
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_NAK ;
	if( cnf )
		IrSIR_SetDataCnf( cnf ) ;
	LogPuts( "Send NAK\n" ) ;
	IrSIR_SendFrame( 2, TxBuffer ) ;
}


static	void			SendTxData( void IrDACB (*cnf)( void ), uint frmsz )
{
	int		i ;


	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_DATA ;
	for( i=0 ; i<frmsz ; i++ )
		TxBuffer[2+i]	= i ;
	IrSIR_SetDataCnf( cnf ) ;


	IrSIR_SendFrame( 2+frmsz, TxBuffer ) ;
}


static	void			SendChgMTT( void IrDACB (*cnf)( void ), word mtt )
{
	TxBuffer[0]			= 0xFF ;
	TxBuffer[1]			= STC_CHGMTT ;
	MiscSetBigEndian16( &TxBuffer[2], mtt ) ;
	IrSIR_SetDataCnf( cnf ) ;
	LogPrintf( "Send ChgMTT (%04X)\n", mtt ) ;
	IrSIR_SendFrame( 4, TxBuffer ) ;
}


static	void	IrDACB	CB_DataInd( void )
{
	int		cmd ;
	int		i ;


	for( ; DoneFrameSize < RecvFrameSize ; DoneFrameSize ++ ) {
		cmd		= RecvAddrTbl[DoneFrameSize][1] ;


		if( cmd >= STC_INVALID ) {
			LogPrintf( "Accept Invalid Cmd(%u)\n       ", cmd ) ;
			for( i=0 ; i<RecvSizeTbl[DoneFrameSize] ; i++ )
				LogPrintf( " %02X", RecvAddrTbl[DoneFrameSize][i] ) ;
			LogPrintf( "\n" ) ;
			cmd	= STC_INVALID ;
		}
#if 0
else {
for( i=0 ; i<RecvSizeTbl[DoneFrameSize] ; i++ )
	LogPrintf( " %02X", RecvAddrTbl[DoneFrameSize][i] ) ;
LogPrintf( "\n" ) ;
}
#endif


		(*RecvEvent[cmd])((byte IrDAFAR *)RecvAddrTbl[DoneFrameSize], RecvSizeTbl[DoneFrameSize] ) ;
	}


	if( RecvFrameSize == RxWindowSize ||
		( DoneFrameSize == RecvFrameSize && !IrSIR_InFrame())) {
		RecvFrameSize		= 0 ;
		DoneFrameSize		= 0 ;
		IrSIR_ResetRecvMode() ;


	}
}


static	void	IrDACB	CB_Tick( void )
{
	LogPrintf( "%u sec\n", ++ TickCount ) ;
	if(( TickTimer = StartTimer( 1000L, CB_Tick )) == -1 ) {
		LogPrintf( "Can't Hook Timer\n" ) ;
		SystemFlag	|= SF_Abort ;
	}
}


#endif	
