/*=======================================**
                frmtest.h                 
        Copyright (c) 1996 by core        
**=======================================*/

/* $Header$ */


#if !defined(__frmtest_h__)
#define		__frmtest_h__


#if defined(DOS)
#define		IrDAFAR		__far
#define		IrDACB		__far __loadds
#define		cputc(c)	fputc( c, stdout )
#define		cputs(s)	fputs( s, stdout )
#define		cgetc()		fgetc( stdin )
#endif

#define		EM_HORIZON		0
#define		EM_VERTICAL		1
#define		EM_VERTICAL2	2
#define		EM_VERTICAL3	3
#define		EM_SPEED		4
#define		EM_PARA			-1
#define		EM_SEND			-2
#define		EM_RECV			-3
#define		EM_VERBOSE		-4
#define		EM_ABORT		-5


#define		S_IDLE		0		/* 初期状態		*/
#define		S_SENDFRAME	1		/* データ送信		*/
#define		S_WAITCNF	2		/* データ送信完了待ち	*/
#define		S_RECVFRAME	3		/* データ受信		*/
#define		S_WAITIND	4		/* データ受信完了待ち	*/
#define		S_CHKDATA	5		/* 受信データチェック	*/


extern	void	FrameInitialize( void ) ;
extern	void	FrameTerminate( void ) ;
extern	void	FrameTest_h( void ) ;
extern	void	FrameTest_v( void ) ;
extern	void	FrameTest_s( void ) ;

extern	int	FrameState ;
extern	ulong	FramelBaudRate ;
extern	uint	FrameMinTurn ;
extern	uint	FrameNumBOF ;
extern	int	FrameExamMethod ;
extern	int	FramefVerbose ;
extern  int IrDA_FrameTest(void) ;
extern	int	FramefIncData ;
extern	int	DataLen ;
extern	int	FrameBidirectionMode;
extern	int	FrameRecievDump;
extern	int	TimerEventSend;
extern	int	FrameHighPerformance;
#endif	/*!__frmtest_h__*/

/* $Log$
*/
