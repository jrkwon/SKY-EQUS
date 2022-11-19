/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       irtmrapi.h                                   */
/*             Created:    7/28/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/* -------------------------------------------------------------------- **
		�h���r�h�q�@�^�C�}�֐�

	�O���Ɍ��J���Ă���֐�

	int	IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) ) ;
	void	IrDAAPI IrSIR_TimerRem( int hndl ) ;
	ulong	readtimer( void ) ;

** -------------------------------------------------------------------- */

#include	"irdaconf.h"
#include	<irda.h>
#include	<irsir.h>

#include	"irsirddk.h"

/* ----- ���荞�ݏ����p�f�[�^�e�[�u�� ------------------------------- */
PFCB	SirTimerFunc [ TBL_CNT ] ;	/* ���荞�ݏ����֐�	*/
dword	SirTimerCount[ TBL_CNT ] ;	/* �^�C�}�����l		*/
volatile char	SirTimerSem[TBL_CNT] ;		/* �Z�}�t�H		*/

/* ----- ���荞�݊֐��̓o�^ ------------------------------------------- */
int IrDAAPI IrSIR_TimerReq( dword msec, void (IrDACB *func)(void) )
{
	// tasik 01.04.11
	// ������ �ð�(msec)�� ������ ���� callback�� �˷��ش�.
	// �� ���⼭ ���� �����ؼ� msec�� ������ �׶����� 
	// callback �Լ����� ������ �Ѵ�.
int	no ;
	for (no = 0 ;no < TBL_CNT ;no++ ) {
		if(SirTimerSem[no])
			continue ;
		if( SirTimerFunc[no] ) {
			SirTimerSem[no]	= 0 ;
			continue ;
		}
		SirTimerCount[no] = SirTicTimer + msec ;
		SirTimerFunc[no] = func ;
		SirTimerSem[no]	= 0 ;
		return(no) ;
	}
	return( -1 ) ;
}

/* ----- ���荞�݊֐��̍폜 ------------------------------------------- */
void IrDAAPI IrSIR_TimerRem( int no )
{
	if(no < 0 || TBL_CNT <= no)
		return;
	SirTimerCount[no] 	= (dword)0;
	SirTimerFunc[no]	= (PFCB)0;
	SirTimerSem[no]		= 0 ;
}

/* ----- �o�ߎ��Ԃ̎擾 ----------------------------------------------- */
ulong	IrDAAPI	IrSIR_ReadTimer( void )
{
	// tasik 01.04.11
	// ������ �ð��� �˷��ִ� ���̴�. 
	// ���� ������ ��Ȯ�� �ð��� �˷��ִ� ���� �ƴ϶�.
	// �ý����� �����ǰ� ���� �����ϰ� ī��Ʈ�� �� ���̴�.
	return( SirTicTimer ) ;
}

