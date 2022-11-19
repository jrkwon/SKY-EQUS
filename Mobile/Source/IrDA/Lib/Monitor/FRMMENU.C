/*=======================================**
                frmtest.c                 
        Copyright (c) 1996 by core        
**=======================================*/

/* $Header$ */


#include	<ctype.h>
#include	<stdarg.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	"irdaconf.h"
#include	"frmtest.h"
#include	"console.h"
/*#include	"irda.h"*/
#include	"irsir.h"
#if	defined(DOS)
#include	<stdlib.h>
#endif	/*DOS*/

#ifdef	USE_MONITOR

#undef	USE_FIR

/*  -----  public functions  -----  */
void	FrameMenu( void ) ;


/*  -----  statis functions  -----  */
static	void	Initialize( void ) ;
static	void	Terminate( void ) ;
static	void	DispMenu( void ) ;
static	void	DispLineLong( const char *msg, long val, char sym ) ;
static	void	DispLineExam( const char *msg, int val, char sym ) ;
static	void	DispLineString( const char *msg, char sym ) ;
static	void	DispLineSwitch( const char *msg, int val, char sym ) ;
static	void	DispDot( uint len, char sym ) ;
static	int		ReadCommand( void ) ;
static	long	ReadLong( const char *prompt ) ;
static	int		SelectBaudRate( void ) ;

/*  -----  local variables  -----  */



int	IrDA_FrameTest( void )
{
	Initialize() ;
	FrameMenu() ;
	Terminate() ;
	return( 0 ) ;
}


static	void	Initialize( void )
{
int	ret ;

	FrameMinTurn	=	0 ;
	FrameNumBOF		=	0 ;
	FramelBaudRate	=	9600L ;
	FrameExamMethod	=	EM_HORIZON ;
	FramefVerbose	=	0 ;
	
	FrameState				=	S_IDLE ;
	FramefIncData			=	FALSE;
	FrameBidirectionMode	=	TRUE;	
	FrameRecievDump			=	FALSE;
	TimerEventSend			=	FALSE;
	FrameHighPerformance	=	FALSE;
	DataLen					=	1 ;
	
	ret		= IrSIR_Open() ;
#if	defined(DOS)
	if( ret != IrDAR_ACCEPT ) {
		printf( "IrSIR_Open()=%d\n", ret ) ;
		exit( 1 ) ;
	}
	freopen( "CON", "rb", stdin ) ;
	setvbuf( stdin, 0, _IONBF, 0 ) ;
#endif	/*DOS*/
}


static	void	Terminate( void )
{
	IrSIR_Close() ;
}


void	FrameMenu( void )
{
int	c ;
	DispMenu() ;
	for(;;) {
		c = ReadCommand() ;
		if( c == 'Q' ){
			break ;
		}
		if( c != 'T' && c != 'X' ){
			continue ;
		}

		FrameInitialize() ;
		if( c == 'T' ){
			FrameState = S_SENDFRAME ;
		}
		if( c == 'X' ){
			FrameState = S_RECVFRAME ;
		}

		switch ( FrameExamMethod ) {
		case EM_HORIZON :
/*			cputs( "Method : Horizonal\n" ) ;*/
			FrameTest_h() ;
			break ;
		case EM_VERTICAL :
/*			cputs( "Method : Vertical\n" ) ;*/
			FrameTest_v() ;
			break ;
		case EM_SPEED :
/*			cputs( "Method : Speed\n" ) ;*/
			FrameTest_s() ;
			break ;
		} /* switch */
		FrameTerminate() ;
	}
}

static	void	DispSetting(void)
{
	cprintf("Setting: MinTurn[%dms] BOF[%d] Baud[%ldbps]\n",
		FrameMinTurn,
		FrameNumBOF,
		FramelBaudRate
	);
}

static	void	DispMenu( void )
{
	cputs("*********************************************************\n");
	cputs("    OSW DeepCore(tm) IrSIR Frame Test Monitor Ver 1.0\n");
	cputs("*********************************************************\n");
	DispLineLong("Minimum Turnaround time",(long)FrameMinTurn,	'M' ) ;
	DispLineLong("Additional BOF", (long)FrameNumBOF,	 	'A' ) ;
	DispLineLong("Baud Rate", FramelBaudRate,			'B' ) ;
	DispLineLong("Start Datalength",(long)DataLen,	'L' ) ;
	DispLineExam("Exam Method", FrameExamMethod,			'E' ) ;
	DispLineSwitch("Verbose", FramefVerbose,			'V' ) ;
	DispLineSwitch("IncData", FramefIncData,			'I' ) ;
	DispLineSwitch("Bidirection", FrameBidirectionMode,			'P' ) ;
	DispLineSwitch("Reciev Dump", FrameRecievDump,			'D' ) ;
	DispLineSwitch("Timer Event Send ", TimerEventSend,			'S' ) ;
	DispLineSwitch("Performance Check ", FrameHighPerformance,			'C' ) ;
	DispLineString("Start TEST TxMode",				'T' ) ;
	DispLineString("Start TEST RxMode",				'X' ) ;
	DispLineString("Display This Menu",				'H' ) ;
	DispLineString("QUIT",						'Q' ) ;
}

static	void	DispLineLong( const char *msg, long val, char sym )
{
char	buf[80] ;

	cputs( msg ) ;
	sprintf( buf, " (%ld) ", val ) ;
	cputs( buf ) ;
	DispDot( strlen( msg ) + strlen( buf ), sym ) ;
}


static	void	DispLineExam( const char *msg, int val, char sym )
{
	char	buf[80] ;

	cputs( msg ) ;
	switch( val ) {
	case EM_HORIZON :
		strcpy( buf, " : Horizonal " ) ;
		break ;
	case EM_VERTICAL :
		sprintf( buf, " : Vertical " ) ;
		break ;
	case EM_SPEED :
		sprintf( buf, " : Speed " ) ;
		break ;
	default :
		sprintf( buf, " : IGNORE " ) ;
	}
	cputs( buf ) ;
	DispDot( strlen( msg ) + strlen( buf ), sym ) ;
}

static	void	DispLineString( const char *msg, char sym )
{
	cputs( msg ) ;
	cputc( ' ' ) ;
	DispDot( strlen( msg ) + 1, sym ) ;
}


static	void	DispLineSwitch( const char *msg, int val, char sym )
{
	char	buf[80] ;

	cputs( msg ) ;
	if( val ){
		strcpy( buf, " : ON " ) ;
	}else{
		strcpy( buf, " : OFF " ) ;
	}
	cputs( buf ) ;
	DispDot( strlen( msg ) + strlen( buf ), sym ) ;
}


static	void	DispDot( uint len, char sym )
{
	int		sz ;

	sz		= 58 - 2 - len ;
	for( ; sz > 0 ; sz -- )
		cputc( '.' ) ;
	cputc( ' ' ) ;
	cputc( sym ) ;
	cputc( '\n' );
	cputc( '\r' );
}

static	int		ReadCommand( void )
{
int	c ;
long	tmpl ;
char	buf[10];
	for(;;) {
		cputs("FRM>");
		if(cgets(buf,2) < 1){
			continue;
		}
		c = toupper(buf[0]) ;
		switch( c ) {
		case	'A' :								/*Additional BOF		*/
			tmpl	= ReadLong( "Additional BOF>" ) ;
			if( tmpl >= 0 ){
				FrameNumBOF	= (uint)tmpl ;
				DispSetting();
			}
			break ;
		case	'B' :
			if(SelectBaudRate()){
				DispSetting();
			}
			break ;
		case	'C' :
			FrameHighPerformance		=	!FrameHighPerformance;
			break;
		case	'D' :
			FrameRecievDump		=	!FrameRecievDump;
			break;
		case	'S' :
			TimerEventSend		=	!TimerEventSend;
			break;
		case	'E' :
			switch( FrameExamMethod ) {
			case	EM_HORIZON :
				FrameExamMethod	= EM_VERTICAL ;
				break ;
			case	EM_VERTICAL :
				FrameExamMethod	= EM_SPEED ;
				break ;
			case	EM_SPEED :
				FrameExamMethod	= EM_HORIZON ;
				break ;
			}
			break ;
		case	'M' :
			tmpl	= ReadLong( "Minimum Turnaround time>" ) ;
			if( tmpl >= 0 ){
				FrameMinTurn	= (uint)tmpl ;
				DispSetting();
			}
			break ;
		case	'L' :
			tmpl	= ReadLong( "Start DataLen>" ) ;
			if( tmpl >= 0 ){
				DataLen	= (uint)tmpl ;
				DispSetting();
			}
			break ;
		case	'V' :
			FramefVerbose	= !FramefVerbose ;
			break ;
		case	'I' :
			FramefIncData	= !FramefIncData ;
			break ;
		case	'P' :
			FrameBidirectionMode	=	!FrameBidirectionMode;
			break;
		case	'R' :
		case	'T' :
		case	'X' :
		case	'Q' :
			break ;
		default :
			DispMenu();
			continue ;
		}
		return( c ) ;
	}
}


static	long	ReadLong( const char *prompt )
{
int	ret ;
char	buf[20] ;
	cputs( prompt ) ;
	ret	= cgets( buf, sizeof( buf ) - 1) ;
	if( ret < 1 ){
		return( -1 ) ;
	}
	return( atol( buf )) ;
}

static	int	SelectBaudRate( void )
{
char	buf[10];
	cputs( "Change Baud Rate\n");
	DispLineString( "2400",		'0' ) ;
	DispLineString( "4800",		'1' ) ;
	DispLineString( "9600",		'2' ) ;
	DispLineString( "19200",	'3' ) ;
	DispLineString( "38400",	'4' ) ;
	DispLineString( "57600",	'5' ) ;
	DispLineString( "115200",	'6' ) ;
#if defined(USE_FIR)
	DispLineString( "576000",	'7' ) ;
	DispLineString( "1152000",	'8' ) ;
	DispLineString( "4000000",	'9' ) ;
#endif	/*USE_FIR*/
	cputs("select>");
	if(cgets(buf,1) < 1){
		goto err;
	}
	switch( buf[0] - '0' ) {
		case	0 :
			FramelBaudRate	= 2400L ;
			break ;
		case	1 :
			FramelBaudRate	= 4800L ;
			break ;
		case	2 :
			FramelBaudRate	= 9600L ;
			break ;
		case	3 :
			FramelBaudRate	= 19200L ;
			break ;
		case	4 :
			FramelBaudRate	= 38400L ;
			break ;
		case	5 :
			FramelBaudRate	= 57600L ;
			break ;
		case	6 :
			FramelBaudRate	= 115200L ;
			break ;
		default:
			goto err;
	}
	return	1;
err:
	cputs("Error\n");
	return	0;
}

#endif	/* USE_MONITOR	*/


/* $Log$
*/
