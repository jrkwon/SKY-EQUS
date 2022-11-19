/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       MONITOR.C                                    */
/*             Created:    7/27/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/


/************************************************************************

	Legasic Project IrDA (IrCOMM) Testing Monitor

************************************************************************/



#include	<stdio.h>
#include	<string.h>

#include	"irdaconf.h"
#include	"ldevice.h"
#include	"console.h"
#include	"irsirddk.h"

#if defined(USE_MONITOR)
extern	void		Sample(void);
extern	void		DiscoveryTest(void);
extern	void		Api_Acitve(void);
extern	void		Api_InAcitve(void);


#define		B9600			2
#define		B115200			6
#define		IrEYEFIFOSIZE	16
#define		PRTTIME			1000

extern	void		IrDA_FrameTest(void);
extern	void		GenoaClient(void);
extern	void		IrAPP_PrimaryTest(void);
extern	void		IrCOMM_PrimaryTest(void);
extern	void		IrCOMM_SecondaryTest(void);
extern	void		IrMC_PrimaryTest(void);
extern	void		IrMC_SecondaryTest(void);
extern	void 		OBEX_Server(void);
extern	void 		OBEX_Client(void);

static	void	LoopBackTest( void ) ;

static	int	baud;
static	int	IrDADebugMode;

const	long	bltbl[]= {
		2400L,4800L,9600L,19200L,38400L,57600L,115200L,
		0L
	};

static	void	DisplayCopyright(void)
{
	cputs("\n\n");
	cputs("********************************************************\n");
	cputs("  LEGASIC(tm) High Performance IrDA Protocol Controller \n");
	cputs("  (C) 1996/07/31 Okaya Systemware All Right Reserved\n");
	cputs("********************************************************\n");
	cputs("\n");
	cputs("            Okaya Systemware   Tokyo JAPAN \n");
	cputs("                        Phone +81-3-3795-7601\n");
	cputs("                        Fax   +81-3-3795-7149\n");
	cputs("\n");
	cputs(" This System is IrDA Protocol Driver (IrCOMM)\n");
	cputs(" Include:\n");
	cputs("           IrCOMM (COM:9-Wire LPT:Centronics)\n");
	cputs("           TinyTP IrLMP IrLAP and IrSIR(V1.0)\n");
	cputs("\n");
}


static	void Title(void)	/* Monitor Title Display	*/
{
	cputs("\n\n");
	cputs("*********************************************\n");
	cputs("   LEGASIC IrDA Protocol Controller Ver 1.7\n");
	cputs("   (C)1996 Okaya Systemware\n");
	cputs("*********************************************\n");
	cputs("H  ...... display this Message\n");
	cputs("T  ..... check timer interrupt\n");
	cputs("F  ........... IrDA Frame Test\n");
	cputs("S  ............ Genoa Sir Test\n");
	cputs("1  ......IrAPP  Primary   Test\n");
	cputs("2  ......IrAPP  Secondary Test\n");
	cputs("3  ......IrCOMM Primary   Test\n");
	cputs("4  ..... IrCOMM Secondary Test\n");
	cputs("5  ...... IrMC  Primary   Test\n");
	cputs("6  .....  IrMC  Secondary Test\n");
	cputs("7  ... IrOBEX      Server Test\n");
	cputs("8  ... IrOBEX      Client Test\n");
//	cputs("9  ....API                Test\n");
//	cputs("0  ....Discovery          Test\n");
//	cputs("A  .............. API   ACTIVE\n");
//	cputs("W  .............. API INACTIVE\n");

	cputs("\n");
}

static	void SetBaud()	/* Set Current Baud Rate	*/
{
int	i;
char	buf[20];
	cputs("Change Baud Rate\n");
	for(i = 0;bltbl[i] != 0L;i++){
		cprintf("%d.\t%ldbps\n",i,bltbl[i]);
	}
	cputs("select>");
	cgets(buf,2);
	if(*buf < '0' || i+'0' <= *buf){
		cputs("Error\n");
		return;
	}
	baud = *buf - '0';
	cprintf("Change to %ldbps\n",bltbl[baud]);
	IrEyeSetBaud(bltbl[baud]);
}

extern	unsigned	long	StdTimer1;
static	void DispTimer()	/* Display Tic Timer	*/
{
unsigned long	old,new;
	old	=	0;
	new	=	0;
	cputs("Timer Test <ESC> break\n");
	for(;;){
		new = StdTimer1 / 1000L;
		if(old != new){
			old = new;
			cprintf("Tic Timer %lu\r",new);
		}
		if(cget() == 0x1b)
			break;
	}
}

static	void IrSIR_ModemTest()
{
int	c;
	cputs("IrSIR <--> Modem Test <ESC> Break!\n");
	for(;;){
		c = cget();
		if(c != -1){
			if(c == 0x1b)
				goto end;
//			IrEyePutc(c);
			if(c == '\r'){
//				IrEyePutc('\n');
			}
		}
//		c = IrEyeGetc();
		if(c != -1){
			cputc(c);
			if(c == '\r')
				cputc('\n');
		}
	}
end:
	cputs("\n\nBreak!\n");
}


static void IrSIR_OutputTest()
{
int	c;
	cputs("Sending Ir !\n");
	for(c = ' ';c < 'z';c++){
//		IrEyePutc(c);
	}
	cputs("Sned End\n");
}



static	void SwitchDebugMode()
{
#if 0
	IrDADebugMode = !IrDADebugMode;
	if(IrDADebugMode)
		_SETDEBUGMODE(-1);
	else
		_SETDEBUGMODE(0);
#endif
}

static	void IrSIR_FrameTest()
{
	IrDA_FrameTest();
	IrEyeSetBaud(bltbl[baud]);
}

static	void IrSIR_GenoaTest()
{
	GenoaClient();
	IrEyeSetBaud(bltbl[baud]);
}

static	void MonInit(void)
{
	baud = B9600;
	IrEyeSetBaud(bltbl[baud]);
	cbaud(bltbl[baud]);
/*	cDTROn();*/
/*	cRTSOn();*/
	IrDADebugMode = 0;
/*	_SETDEBUGMODE(0);*/
	Title();
}

void Monitor(void)
{
int	flg;
char	cmd[10];
	MonInit();
	
	flg = 1;
//	for(;;)
//	{
//		IrCOMM_PrimaryTest();
//	}

	for(;;){
		if(flg)
			cputs("MON>");
		flg = 1;
		if(cgets(cmd,5) == -1)
			continue;
		switch(*cmd){
			case	'H':	/* Help 	*/
			case	'h':
					Title();
					break;

			case	'B':	/* Baud rate 	*/
			case	'b':
					SetBaud();
					cputs("MON>");
					cbaud(bltbl[baud]);
					flg = 0;
					break;
			case	'T':	/* Timer	*/
			case	't':
					DispTimer();
					break;
			case	'I':	/* IrSIR <--> Modem Test	*/
			case	'i':
					IrSIR_ModemTest();
					break;
			case	'O' :	/* Loop Back Test		*/
			case	'o' :
					LoopBackTest() ;
					break ;
			case	'Z':	/* IrSIR OutputTest		*/
			case	'z':
					IrSIR_OutputTest();
					break;
			case	'C':	/* Display Copyright		*/
			case	'c':
					DisplayCopyright();
					break;
			case	'F':	/* IrDA Frame Test */
			case	'f':
					IrSIR_FrameTest();
					break;
			case	'S':	/* IrDA Genoa Test */
			case	's':
					IrSIR_GenoaTest();
					break;

			case	'D':	/* Debugmode 	*/
			case	'd':
					SwitchDebugMode();
					break;
			case	'1':
					IrAPP_PrimaryTest();
					break;
			case	'2':
/*					IrAPP_SecondaryTest();*/
					break;
			case	'3':
					IrCOMM_PrimaryTest();
					break;
			case	'4':
					IrCOMM_SecondaryTest();
			case	'5':
					IrMC_PrimaryTest();
					break;
			case	'6':
					IrMC_SecondaryTest();
					break;
			case	'7':
					OBEX_Server();
					break;
			case	'8':
					OBEX_Client();
					break;
			case	'9':
//					Sample();
					break;
			case	'0':
//					DiscoveryTest();
					break;
			case	'A':
//					Api_Acitve();
					break;
			case	'W':
			case	'w':
//					Api_InAcitve();
					break;
			default:
				Title();
				break;
		}
	}
}


static	void	LoopBackTest( void )
{
	int		c ;
	int		i ;
	int		testcnt ;
	int		fifosz ;
	int		rxc, txc ;

	cputs( "FIFO SIZE\n" ) ;
	cputs( "1  ............................... 1\n");
	cputs( "F  ...............................16\n");
	for( ; ( c = cget()) == -1 ; ) ;
	if( c == '1' )
		fifosz	= 1 ;
	else if( c == 'F' || c == 'f' )
		fifosz	= 16 ;
	else
		fifosz	= IrEYEFIFOSIZE ;
	cprintf( "Set FIFO Size %d\n", fifosz ) ;

	rxc		= 0 ;
	txc		= 0 ;
	for( testcnt=0 ;; testcnt++ ) {
		cputs( "T\x08" ) ;
		for( i=0 ; i<fifosz ; i++ ) {
			if( cget() != -1 ) {
				cputs( "User BREAK\n" ) ;
				return ;
			}
//			IrEyePutc( txc ) ;
			txc		= ( txc + 1 ) & 0xFF ;
		}

		cputs( "R\x08" ) ;
		for(;;) {
			if( cget() != -1 ) {
				cputs( "User BREAK\n" ) ;
				return ;
			}

//			if(( c = IrEyeGetc()) == -1 )
//				break ;

			if( rxc != c )
				cprintf( " %02x:%02xR\x08", rxc, c ) ;
			rxc		= ( rxc + 1 ) & 0xFF ;
		}

		cputs( "." ) ;
	}
}


unsigned char InMonitor = 1;

void	SetInMonitor(char c)
{
	InMonitor = c;
}


#endif
