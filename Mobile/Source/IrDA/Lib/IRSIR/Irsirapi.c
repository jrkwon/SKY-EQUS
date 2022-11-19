/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IRSIRAPI.C                                   */
/*             Created:    7/25/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/* $Header$ */

#include	"irdaconf.h"
#include	"irsir.h"
#include	"irsirddk.h"

/********************************************************************

	Common Data Strage

********************************************************************/

/*** Recive Buffser resource ***/

byte	*SirRecvBuffer ;
uint	 SirRecvBufferSize ;

uint	 SirRecvTblSize ;
uint	*SirRecvWinSizePtr ;

uint	*SirRecvSizeTbl ;
byte	**SirRecvBufTbl ;

IrDABOOL	 SirInFrame;

/*** Callback Pointers	***/

volatile void	(IrDACB *SirDataIndPtr)( void ) ;
volatile void	(IrDACB *SirDataCnfPtr)( void ) ;
volatile void	(IrDACB *SirDataIndc)( void ) ;
volatile void	(IrDACB *SirDataConf)( void ) ;

/*** Misc Data	***/

int	 SirConnAddr;	/* Connection Address		*/
int	 SirNoOfBOF;	/* BOF lengs			*/
uint	 SirMinTATime;	/* Minimam Turnaround Time	*/

/*** Timer resource ***/

volatile	dword	SirBusyTimer;
volatile	dword	SirTicTimer;

extern	int	OsFirFlag;


/********************************************************************

	IrSIR Common Function Entrys

*********************************************************************/

#if defined(IrDAPRIMARY)
IrDABOOL	IrDAAPI	IrSIR_Busy( void )
{
	// ÇöÀç media°¡ »ç¿ëÁßÀÎ°¡ ¾Æ´Ñ°¡¸¦ È®ÀÎÇÏ´Â °ÍÀÌ´Ù.
	// ¸¸¾à °ú°Å 500msÀÌ³»¿¡ ¼Û¼ö½ÅÀÌ ÀÖ¾ú´Ù¸é 
	// true°¡ ¸®ÅÏµÉ °ÍÀÌ¸ç ¾ø¾ú´Ù¸é false°¡ ¸®ÅÏµÉ °ÍÀÌ´Ù.
	if(SirBusyTimer >= 500L )
		return( FALSE ) ;
	return( TRUE ) ;
}
#endif	/*IrDAPRIMARY*/

IrDABOOL	IrDAAPI	IrSIR_InFrame( void )
{
		return( SirInFrame ) ;
}

void	IrDAAPI	IrSIR_SetFrameBuffer( IrSIRFRAMECONFIG IrDAFAR *frameconfig )
{
	SirRecvBufferSize	= frameconfig->RecvBufferSize ;
	SirRecvBuffer		= frameconfig->RecvBuffer ;

	SirRecvTblSize		= frameconfig->TblSize ;
	SirRecvBufTbl		= (byte **)frameconfig->RecvBufTbl ;
	SirRecvSizeTbl		= (uint *)frameconfig->RecvSizeTbl ;
	SirRecvWinSizePtr	= (uint *)frameconfig->RecvWindow ;
	SIRSetRxBuffer();
}

void	IrDAAPI	IrSIR_SetIrDAMode( void )
{
	IrSIR_ResetRecvMode() ;
	SIRSetRxIntr() ;
}

void	IrDAAPI	IrSIR_SetConnectionAddress( uint ca )
{
	SirConnAddr = (ca << 1) & IrSIR_GROB;
}

void	IrDAAPI	IrSIR_SetBOF( uint bofsz )
{
	SirNoOfBOF = bofsz + 1 ;
}

void	IrDAAPI	IrSIR_SetMinimamTurnaroundTime( uint msec )
{
	SirMinTATime = msec ;
}

void	IrDAAPI	IrSIR_SetDataInd( void (IrDACB *funcp)( void ))
{
	SirDataIndPtr	= (volatile void (IrDACB *)(void))funcp ;
}

void	IrDAAPI	IrSIR_SetDataCnf( void (IrDACB *funcp)( void ))
{
	SirDataCnfPtr	= (volatile void (IrDACB *)(void))funcp ;
}

void	IrDAAPI	IrSIR_ResetRecvMode( void )
{
	/* ƒf[ƒ^Ši”[ƒAƒhƒŒƒX‚ðæ“ª‚É–ß‚· */
	SIRResetRxBuff();
}

