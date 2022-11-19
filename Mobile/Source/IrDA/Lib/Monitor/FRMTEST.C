/*=======================================**
                frmtest.c                 
        Copyright (c) 1996 by Yan Elan    
**=======================================*/

/* $Header$ */


#include	<stdarg.h>
#include	<stdio.h>
	
#include	"irdaconf.h"
#include	"frmtest.h"
#include	"irsir.h"
#include	"irsirddk.h"
#include	"console.h"

#ifdef	USE_MONITOR

/*  -----  local functions  -----  */
static	IrDACB void	CBDataInd_h( void ) ;
static	IrDACB void	CBDataCnf_h( void ) ;

static	IrDACB void	CBDataInd_v( void ) ;
static	IrDACB void	CBDataCnf_v( void ) ;

static	IrDACB void	CBDataInd_s( void ) ;
static	IrDACB void	CBDataCnf_s( void ) ;

static	void	SetSendData( byte *achBuf, int len ) ;
static	void	chg_speed( int count ) ;
static	void	dputc( int c ) ;
static	void	dprintf( const char *fmt, ... ) ;


/*  -----  local variables  -----  */

static	volatile byte IrDAFAR	*RecvAddrTbl[IrLAPRECVWINSIZE];
static	volatile uint IrDAFAR	RecvSizeTbl[IrLAPRECVWINSIZE] ;
static	volatile uint IrDAFAR	RecvFrameSize ;		/* 受信フレーム数	*/

static	 uint IrDAFAR	RecvFrameReadPtr ;
static	IrDABOOL	PowerMode ;

static	int		fSendCnf ;							/* Confirm Flag		*/
static	int		fRecvInd ;							/* Indicate Flag	*/

static	long	RxFrameCount ;
static	long	TxFrameCount ;
static	long	RxByteCount;
static	long	TxByteCount;


/*  -----  public valiables  -----  */
ulong	FramelBaudRate ;
uint	FrameMinTurn ;
uint	FrameNumBOF ;
int		FrameExamMethod ;
int		FramefVerbose ;
int		FrameState ;
int		FramefIncData ;
int		FrameBidirectionMode;					/*	Bidirection mode	*/
int		FrameBidirectionMode;					/*	Reciev Dump			*/
int		DataLen ;							/*	Set Data Length		*/
int		FrameRecievDump;					/*	Reciev Frame Dump	*/
int		TimerEventSend;						/*	Timer Event Send	*/
int		FrameHighPerformance;				/*	Performance Check	*/

void	FrameInitialize( void )
{
	IrSIRFRAMECONFIG	frameconfig ;

	PowerMode	= IrDA_GetPowerMode() ;
	IrDA_PowerOn() ;

	IrSIR_InitConnectionParameters() ;
	IrSIR_SetSpeed( FramelBaudRate ) ;
	IrSIR_SetConnectionAddress( 0x7f ) ;
	IrSIR_SetMinimamTurnaroundTime( FrameMinTurn ) ;
	IrSIR_SetBOF( FrameNumBOF ) ;
	frameconfig.RecvBuffer     = achRecvBuf ;
	frameconfig.RecvBufferSize = sizeof( achRecvBuf ) ;
	frameconfig.TblSize        = IrLAPRECVWINSIZE ;
	frameconfig.RecvBufTbl     = RecvAddrTbl ;
	frameconfig.RecvSizeTbl    = RecvSizeTbl ;
	frameconfig.RecvWindow     = &RecvFrameSize ;
	IrSIR_SetFrameBuffer( &frameconfig ) ;
	IrSIR_SetFrameMode() ;
	IrSIR_ResetRecvMode() ;
	RecvFrameReadPtr		=	0 ;
	RecvFrameSize			=	0 ;

	fRecvInd				=	0 ;
	fSendCnf				=	0 ;
}


void	FrameTerminate( void )
{
	if( PowerMode == FALSE )
		IrDA_PowerOff() ;
}


/* ----- 水平方向検査 ------------------------------------------------- */

#if defined(OSFIR_CHECK)
	extern	OsFirTxStatus;
#endif

void	FrameTest_h( void )

{
	dword	starttime,endtime;
	long	baud;
	
	starttime = SirTicTimer;
	IrSIR_SetDataInd( CBDataInd_h ) ;

	if( FrameState == S_RECVFRAME )
		IrSIR_SetRecvMode() ;

	for ( ;; ) {
		switch ( FrameState ) {

			case S_SENDFRAME :
				cputs("Start...\n") ;
				RxFrameCount	=	0;
				TxFrameCount	=	0;
				RxByteCount		=	0;
				TxByteCount		=	0;
				SetSendData( achSendBuf, DataLen ) ;
				IrSIR_SetDataCnf( CBDataCnf_h ) ;
				FrameState = S_WAITCNF ;
				IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
				dputc('s');
				break ;
			case S_RECVFRAME :
				IrSIR_SetRecvMode() ;
				RxFrameCount	=	0;
				TxFrameCount	=	0;
				RxByteCount		=	0;
				TxByteCount		=	0;
				FrameState = S_WAITCNF ;
				break;
			
			default :
				break ;
		} /* switch */
#if 0
		if( cget() != -1 )
			FrameState = S_IDLE ;
#endif
		if ( FrameState == S_IDLE ) {
			cputs( "done \n" ) ;
			endtime = SirTicTimer;
			/*	Recieved	*/
			cprintf( "Received Frame : %d\n", RxFrameCount ) ;
			cprintf( "Received Byte  : %d bytes\n", RxByteCount ) ;
			/*	Send		*/
			cprintf( "Send     Frame : %d\n", TxFrameCount ) ;
			cprintf( "Send     Byte  : %d bytes\n", TxByteCount ) ;
			
			baud = 8L*(TxByteCount + RxByteCount)/(endtime - starttime);
			cprintf( "BaudRate       : %d kbps\n", baud ) ;
			cprintf( "Time           : %d sec\n", (endtime-starttime)/1000L ) ;
			break ;
		}
	}
}

static	IrDACB void	CBDataInd_h( void )
{
	int	i,j,count ;
	int	size ;
	int	data ;

	dputc('i');
	RxFrameCount++;
	if ( RecvAddrTbl[0] != achRecvBuf ) {
		dprintf( "Address Table Error !\n" ) ;
		dprintf( "RecvAddrTbl[0]=%08lX, achRecvBuf=%08lX\n",
		(ulong)RecvAddrTbl[0],
		(ulong)achRecvBuf ) ;
		FrameState = S_IDLE ;
	}
				
	/* データサイズ、内容チェック */
	size = RecvSizeTbl[0] ;
	RxByteCount += size ;
	data = size - 1 ;
	
	if(FrameRecievDump){		/*	Reciev Dump Mode -> Dump	*/
		count = 0;
		for( i = 0 ; i < size ; i++){
    		dprintf("0x%04x ",(i*16));
			for( j = 0 ; j < 16 ; j++,count++ ){
				if(count == size){
					break;
				}else{
					dprintf("%02X",RecvAddrTbl[0][count]);
				}
				dprintf("\n");
			}
		}
	}
	
#if !defined(FRMTEST_PAFORMANCE_CHECK)
	for ( i = 1 ; i < size ; i++ ) {
		if ( RecvAddrTbl[0][i] != (byte)data ) {
			cprintf( "RecvAddrTbl[0][%d] = 0x%02x:0x%02x\n", i, RecvAddrTbl[0][i], data ) ;
			FrameState = S_IDLE ;
		}
	}
#endif	/*	FRMTEST_PAFORMANCE_CHECK	*/

	/* 次回のデータ長計算（受信サイズ－１が実際のデータ長） */
/*cprintf( "size:%d IrLAPRECVSZ:%d\n", size, IrLAPRECVSZ ) ;*/
#if 1
	if ( (size - 1) >= IrLAPRECVSZ ) {
#endif
#if 0
	if ( size -1   >= 20 ){
#endif
/*		dprintf( "ReceiveFramesize = %d\n", size ) ;*/
		DataLen = 1 ;
	} else {
		DataLen = (size - 1 + 1) ;
	}

	if(FrameHighPerformance)
//		DataLen = IrLAPRECVSZ - 3;
		DataLen	=	( size -1 );

	/* １フレームしか使用しないのでここでリセット */
	RecvFrameSize	= 0 ;
	IrSIR_ResetRecvMode() ;

	if(FrameState == S_IDLE )
		return;
	if(FrameBidirectionMode){			/*	Bidirectin Mode	*/
		SetSendData( achSendBuf, DataLen ) ;
		IrSIR_SetDataCnf( CBDataCnf_h ) ;
		FrameState = S_WAITCNF ;
		TxByteCount += (1 + DataLen);
		IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
		dputc('s');
	}else{								/*	Only Way Mode	*/
	}
}


static	IrDACB void	CBDataCnf_h( void )
{
	RecvFrameSize	= 0 ;
	TxFrameCount++;
	dputc('i');
#if 1
	if(FrameBidirectionMode){	/*	Bidirection	Mode	*/
		dputc('r');
	}else{						/*	Only Way Mode		*/
		SetSendData( achSendBuf, ++DataLen ) ;
		IrSIR_SetDataCnf( CBDataCnf_h ) ;
		FrameState = S_WAITCNF ;
		TxByteCount += (1 + DataLen);
		IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
		dputc('s');
	}
#endif
}


void	Debug_SendFrame(void)
{
	if(FrameBidirectionMode)
		return;
	if(TimerEventSend){
		FrameInitialize();
		if ( DataLen	>=	IrLAPRECVSZ ) {
			DataLen++;
		}
		SetSendData( achSendBuf, ++DataLen ) ;
		IrSIR_SetDataCnf( CBDataCnf_h ) ;
		FrameState = S_WAITCNF ;
		IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
	}
}



/* ----- 垂直方向検査１ ----------------------------------------------- */
void	FrameTest_v( void )
{
	int	i, j ;
	int	size ;
	int	data ;
	int	seq_count ;
	int	frame_count ;

	IrSIR_SetDataInd( CBDataInd_v ) ;

	if( FrameState == S_RECVFRAME )
		IrSIR_SetRecvMode() ;

	DataLen = IrLAPRECVSZ - 1 ;
	frame_count = 0 ;			/* 連続して送信したフレームの数 */
	for ( seq_count = 0 ; seq_count < 10 ; ) {
		switch ( FrameState ) {
			case S_SENDFRAME :
				dprintf( "send..." ) ;
				SetSendData( achSendBuf, DataLen ) ;
				IrSIR_SetDataCnf( CBDataCnf_v ) ;
				IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
				FrameState = S_WAITCNF ;
				break ;
			case S_WAITCNF :
				if ( fSendCnf != 0 ) {
/*					dprintf( "OK\n" ) ;*/
					dputc('i');
					fSendCnf = 0 ;
					if ( frame_count < IrLAPRECVSZ-1) {
						frame_count++ ;
						FrameState = S_SENDFRAME ;
					} else {
						FrameState = S_RECVFRAME ;
						frame_count = 0 ;
						seq_count++ ;
					}
				}
				break ;
			case S_RECVFRAME :
				dprintf( "receive..." ) ;
				FrameState = S_WAITIND ;
				break ;
			case S_WAITIND :
				if ( fRecvInd != 0 ) {
					dprintf( "OK, RecvFrameSize = %d\n", RecvFrameSize ) ;
					fRecvInd = 0 ;
					if ( RecvFrameSize >= IrLAPRECVWINSIZE ) {
						seq_count++ ;
						FrameState = S_CHKDATA ;
					} else {
						FrameState = S_RECVFRAME ;
					}
				}
				break ;
			case S_CHKDATA :
				dprintf( "check..." ) ;
				FrameState = S_SENDFRAME ;
				for ( i = 0 ; i < IrLAPRECVWINSIZE ; i++ ) {
					size = RecvSizeTbl[i] ;
					data = size - 1 ;
					/* データサイズ、内容チェック */
					for ( j = 1 ; j < size ; j++ ) {
						if ( RecvAddrTbl[0][j] != (byte)data ) {
							cputs( "Data Error !\n") ;
							cprintf( "RecvAddrTbl[0][j] = [%d][%d](%02x)\n", i, RecvAddrTbl[0][j], data ) ;
							FrameState = S_IDLE ;
						}
					}
				}
				if ( FrameState != S_IDLE )
					dprintf( "OK !\n" ) ;
				/* フレームのリセット */
				RecvFrameSize = 0 ;
				IrSIR_ResetRecvMode() ;
				break ;
			default :
				break ;
		} /* switch */
		if( cget() != -1 )
			break ;
		if ( FrameState == S_IDLE ) {
			cputs( "done \n" ) ;
			break ;
		}
	}
}

static	IrDACB void	CBDataInd_v( void )
{
	fRecvInd = 1 ;
}


static	IrDACB void	CBDataCnf_v( void )
{
	fSendCnf = 1 ;
}

/* ----- 速度切替検査 ------------------------------------------------- */
void	FrameTest_s( void )
{
	int	i ;
	int	size ;
	int	data ;
	int	seq_count ;
	
	IrSIR_SetDataInd( CBDataInd_s ) ;
	
	if( FrameState == S_RECVFRAME )
		IrSIR_SetRecvMode() ;

	DataLen = IrLAPRECVSZ - 1 ;
	for ( seq_count = 1 ; seq_count < 10 ; ) {
		switch ( FrameState ) {
			case S_SENDFRAME :
				dprintf( "send%02d...", seq_count ) ;
				SetSendData( achSendBuf, DataLen ) ;
				IrSIR_SetDataCnf( CBDataCnf_s ) ;
				IrSIR_SendFrame( 1 + DataLen, achSendBuf ) ;
				FrameState = S_WAITCNF ;
				break ;
			case S_WAITCNF :
				if ( fSendCnf != 0 ) {
					dprintf( "OK\n" ) ;
					fSendCnf = 0 ;
					chg_speed( seq_count ) ;
					seq_count++ ;
					FrameState = S_RECVFRAME ;
				}
				break ;
			case S_RECVFRAME :
				dprintf( "receive%02d...", seq_count ) ;
				FrameState = S_WAITIND ;
				break ;
			case S_WAITIND :
				if ( fRecvInd != 0 ) {
					dprintf( "OK\n" ) ;
					fRecvInd = 0 ;
					chg_speed( seq_count ) ;
					seq_count++ ;
					FrameState = S_CHKDATA ;
				}
				break ;
			case S_CHKDATA :
				dprintf( "check..." ) ;
				FrameState = S_SENDFRAME ;
				/* アドレステーブルチェック */
				if ( RecvAddrTbl[0] != achRecvBuf ) {
					cputs( "Address Table Error !\n" ) ;
					cprintf( "RecvAddrTbl[0]=%08x, achRecvBuf=%08x\n",
								(ulong)RecvAddrTbl[0],
								(ulong)achRecvBuf ) ;
					FrameState = S_IDLE ;
				}
				/* データサイズ、内容チェック */
				size = RecvSizeTbl[0] ;
				data = size - 1 ;
				for ( i = 1 ; i < size ; i++ ) {
					if ( RecvAddrTbl[0][i] != (byte)data ) {
						cputs( "Data Error !\n" ) ;
						cprintf( "RecvAddrTbl[0][i] = [%d][%d]\n", i, RecvAddrTbl[0][i] ) ;
						FrameState = S_IDLE ;
					}
				}
				if ( FrameState != S_IDLE ) {
					dprintf( "OK !\n" ) ;
				}
				/* １フレームしか使用しないのでここでリセット */
				RecvFrameSize = 0 ;
				IrSIR_ResetRecvMode() ;
				break ;
			default :
				break ;
		} /* switch */
		if( cget() != -1 )
			break ;
		if ( FrameState == S_IDLE ) {
			cputs( "done \n" ) ;
			break ;
		}
	}
}


static	IrDACB void	CBDataInd_s( void )
{
	fRecvInd = 1 ;
}


static	IrDACB void	CBDataCnf_s( void )
{
	fSendCnf = 1 ;
}




static	void	SetSendData( byte IrDAFAR *achBuf, int len )
{
	uint	i;

	achBuf[0] = 0xFE ;
	if(FramefIncData){
	/*	Sendデータはlenの長さだけ0-lenのデータをセット*/	
		for(i = 1 ; i < len + 1 ; i++ ){
/*
			achBuf[i] = i-1 ;
*/
			achBuf[i] = i ;
		}
	}else{
		/* 	Sendデータはlenの長さだけデータlenをセット*/
		for ( i = 1 ; i < len + 1 ; i++ ) {
			achBuf[i] = len ;
		}
	}
}


static	void	chg_speed( int count )
{
	if ( count & 0x01 ) {
		/* 奇数フレームを送受信した後 */
		cprintf( "Change Speed to %ld\n", FramelBaudRate ) ;
		IrSIR_SetSpeed( FramelBaudRate ) ;
	} else {
		dprintf( "Change Speed to 9600\n" ) ;
		IrSIR_SetSpeed( 9600L ) ;
	}
}


static	void	dputc( int c )
{
	if( FramefVerbose )
		cputc( c ) ;
}


static	void	dprintf( const char *fmt, ... )
{
	char	buf[100];
	va_list		vlist ;

	if( FramefVerbose ) {
		va_start( vlist, fmt ) ;
		vsprintf( buf, fmt, vlist ) ;
		cputs( buf ) ;
		va_end( vlist ) ;
	}
}

#endif	/* USE_MONITOR	*/

/* $Log$
*/
