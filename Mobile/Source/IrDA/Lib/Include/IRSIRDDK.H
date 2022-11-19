#ifndef	IRSIRDDK_H
#define	IRSIRDDK_H
/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irsirddk.h                                     */
/*             Created:    8/04/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

#include "target.h"                /* Target specific definitions          */

/*** IrDA Control Codes	***/
#include	"irda.h"
#include	"irdaconf.h"

#ifdef	FEATURE_IRDA_UPGRADE
// tasik 01.05.16, PDA∏¶ ø¨∞·«œ±‚ ¿ß«ÿº≠ ªÏ∑¡µ“.
//youandi 01.05.22 IrDA
//#define	TX_NOWAIT_CONFIRM	/*	Modify OSW:Takagaki 2000/01/20	*/
//youandi 01.05.22 IrDA
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#define	IrSIR_BOF	0xC0
#define	IrSIR_EOF	0xC1
#define	IrSIR_ESC	0x7D
#define	IrSIR_XOR	0x20
#define	IrSIR_GROB	0xfe

#define	isESCCode(c)	((IrSIR_BOF == c) || (IrSIR_EOF == c) || (IrSIR_ESC == c))

/*** IrDA CRC Calcs	***/

#define	GOODCRC		0xF0B8
#define	UpdateCRC(c)	crc = ( crc >> 8 ) ^ SIRcrc[( crc ^((word) c )) & 0xff ]
extern	const word	SIRcrc[];

typedef	void (IrDACB *PFCB)( void ) ;

/*** Recive Buffser resource ***/

extern	byte	*SirRecvBuffer ;
extern	uint	 SirRecvBufferSize ;
extern	byte	*SirRecvBufferEnd ;
extern	uint	 SirRecvTblSize ;

extern	uint	*SirRecvWinSizePtr ;
extern	uint	*SirRecvSizeTbl ;

extern	byte	**SirRecvBufTbl ;
extern	IrDABOOL	 SirInFrame;

extern	void	SIRResetRxBuff(void);
extern	void	SIRKickRx(void);

extern	void	SIRSetRxIdle(void);
extern	void	SIRSetRxBuffer(void);

extern	volatile void	(IrDAAPI *SirRxFunc)(int c) ;

/*** Send Buffser resource ***/

extern	byte	*SirSendBuffer;
extern	byte	*SirSendBufferEnd;

extern	void	SIRKickTx(void);
extern	void	SIRSetTxIdle(void);

extern	volatile int	(IrDAAPI *SirTxFunc)(void ) ;

/*** Callback Pointers	***/

extern	volatile void	(IrDACB *SirDataIndPtr)( void ) ;
extern	volatile void	(IrDACB *SirDataCnfPtr)( void ) ;
extern	volatile void	(IrDACB *SirDataFunc)( void ) ;

/*** Misc Data	***/

extern	int	 SirConnAddr;	/* Connection Address		*/
extern	int	 SirNoOfBOF;	/* BOF lengs			*/
extern	uint	 SirMinTATime;	/* Minimam Turnaround Time	*/

/*** Timer resource ***/

#define		TBL_CNT		4		/* ÉeÅ[ÉuÉãêî		*/
#define		TBL_MASK		0x3

extern	PFCB	SirTimerFunc [ TBL_CNT ] ;	/* äÑÇËçûÇ›èàóùä÷êî	*/
extern	dword	SirTimerCount[ TBL_CNT ] ;	/* É^ÉCÉ}èâä˙íl		*/
extern volatile char SirTimerSem[TBL_CNT] ;

extern	volatile	dword	SirBusyTimer;
extern	volatile	dword	SirTicTimer;

/*** Interrupt Enabler ***/

extern	void	SIRSetRxIntr(void);
extern	void	SIRSetTxIntr(void);
extern	void	SIRSetNoIntr(void);


extern	void	IrdaIntOff(void);
extern	void	IrdaRxIntOn(void);
extern	void	IrdaTxIntOn(void);
extern	void	IrdaSirSetBaud(long);

extern	void	IrdaRxCallBack(int data);
extern	void	IrdaRxEOI(void);
extern	int		IrdaTxCallBack(void);
extern	void	IrdaTxEOI( void );
extern	void	IrdaTic0CallBack(int interval);
extern	void	IrdaTic1CallBack(void);
extern	void	IrSIR_HwInit(void);

void			IrDAAPI	IrSIR_SetIrDAMode( void );
int				IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) ) ;
void			IrDAAPI IrSIR_TimerRem( int hndl ) ;
ulong			readtimer( void ) ;

extern	byte IrDAFAR	achRecvBuf[IrDARECVBUFSZ];
extern	byte IrDAFAR	achSendBuf[IrDASENDBUFSZ];

extern	void	IrDA_Hw_Init( void);
extern	void	MsmTimerInit( void);
extern	void	MsmUart1Init(void);
extern	void	MsmUart2Init(void);
extern	void	IrDA_Start(void);

#endif	//	#ifndef	IRSIRDDK_H