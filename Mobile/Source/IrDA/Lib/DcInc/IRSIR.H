/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-97 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: irsir.hv  170.27  98/02/20 15:01:48  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* �T�v																		*/
/*		IrSIR API��`														*/
/*																			*/
/* �쐬��																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irsir_h__)
#define		__irsir_h__

#include	"irda.h"


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		IrSIR�h���C�o��M�o�b�t�@�ʒm�e�[�u��								*/
/*																			*/
/* 2.�T�v																	*/
/*		IrSIR�h���C�o�Ɏ�M�o�b�t�@����ʒm����ׂɎg�p����B				*/
/*																			*/
/* 3.�@�\����																*/
/*		IrSIR�h���C�o�Ɏ�M�o�b�t�@����ʒm����ׂɎg�p����B				*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IrSIRFRAMECONFIG {
				byte	IrDAFAR	*RecvBuffer ;			/*��M�o�b�t�@�A�h���X			*/
				uint			RecvBufferSize ;		/*��M�o�b�t�@�T�C�Y			*/
				uint			TblSize ;				/*�e�[�u����					*/
	volatile	byte	IrDAFAR	* IrDAFAR *RecvBufTbl ;	/*��M�t���[���A�h���X�e�[�u��	*/
	volatile	uint	IrDAFAR	*RecvSizeTbl ;			/*��M�T�C�Y�e�[�u��			*/
	volatile	uint	IrDAFAR	*RecvWindow ;			/*��M�t���[�����i�[�A�h���X	*/
}	IrSIRFRAMECONFIG ;


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/

/*  -----  public functions  -----  */

int		IrDAAPI	IrSIR_Open( void ) ;
void	IrDAAPI	IrSIR_Close( void ) ;
uint	IrDAAPI	IrSIR_GetCapability( const void IrDAFAR * IrDAFAR *ptr ) ;
													/*�f�[�^��M�C���f�B�P�[�g�֐���`			*/
void	IrDAAPI	IrSIR_SetDataInd( void (IrDACB *funcp)( void )) ;
													/*�f�[�^���M�R���t�@�[���֐���`			*/
void	IrDAAPI	IrSIR_SetDataCnf( void (IrDACB *funcp)( void )) ;
													/*SIR�p�����[�^�[������						*/
void	IrDAAPI	IrSIR_InitConnectionParameters( void ) ;
void	IrDAAPI	IrSIR_SetSpeed( dword spd ) ;		/*�]�����x�ݒ�								*/

IrDABOOL	IrDAAPI	IrSIR_Busy( void ) ;				/*�Ō�Ɏ�M���Ă���500ms�����Ă��邩�ǂ���	*/
IrDABOOL	IrDAAPI	IrSIR_InFrame( void ) ;				/*�t���[����M�����ǂ���					*/
													/*�t���[����M�p�A�h���X�ݒ�				*/
void	IrDAAPI	IrSIR_SetFrameBuffer( IrSIRFRAMECONFIG IrDAFAR *frameconfig ) ;
void	IrDAAPI	IrSIR_SetFrameMode( void ) ;		/*�t���[����M���[�h�ݒ�					*/
void	IrDAAPI	IrSIR_SetByteMode( void ) ;			/*�o�C�g��M���[�h�ݒ�						*/
void	IrDAAPI	IrSIR_SetRecvMode( void ) ;			/*��M���[�h�ֈȍ~							*/
void	IrDAAPI	IrSIR_ResetRecvMode( void ) ;		/*�t���[����M���[�h���Z�b�g				*/
													/*�R�l�N�V�����A�h���X�ݒ�					*/
void	IrDAAPI	IrSIR_SetConnectionAddress( uint ca ) ;

													/*�~�j�}���^�[���A���E���h�^�C���ݒ�		*/
void	IrDAAPI	IrSIR_SetMinimamTurnaroundTime( uint msec ) ;
void	IrDAAPI	IrSIR_SetBOF( uint bofsz ) ;		/*BOF���ݒ�									*/
													/*�t���[�����M								*/
void	IrDAAPI	IrSIR_SendFrame( uint datasz, void IrDAFAR *data ) ;

ulong	IrDAAPI	IrSIR_ReadTimer( void ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


#endif	/*__irsir_h__*/