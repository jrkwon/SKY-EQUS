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
 *     $Header: tinytp.hv  170.27  98/02/20 15:01:56  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* �T�v																		*/
/*		TinyTP�v���g�R����`												*/
/*																			*/
/* �쐬��																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__tinytp_h__)
#define	__tinytp_h__

#include	"irda.h"
#include	"irlmp.h"


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		TinyTP�ڑ������[�U�[�f�[�^											*/
/*																			*/
/* 2.�T�v																	*/
/*		TinyTP�ڑ����ɂ��Ƃ肳��郆�[�U�[�f�[�^���Ǘ�����B				*/
/*																			*/
/* 3.�@�\����																*/
/*		TinyTP�ڑ����ɂ��Ƃ肳��郆�[�U�[�f�[�^���Ǘ�����B				*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_TTPCONNDAT {
	byte	IrDAFAR *Data ;							/*User Data				*/
	int				DataSize ;						/*User Data Size		*/
}	TTPCONNDAT ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.�\���̖�																*/
/*		�R�[���o�b�N�֐�													*/
/*																			*/
/* 2.�T�v																	*/
/*		�e��C�x���g�������ɌĂяo���R�[���o�b�N�֐����Ǘ�����B			*/
/*																			*/
/* 3.�@�\����																*/
/*		TinyTP�Ŋe��C�x���g�������ɌĂяo���R�[���o�b�N�֐����Ǘ�����B	*/
/*																			*/
/* 4.���l																	*/
/*		����																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_TTPCALLBACK	{
	int		(IrDACB *ConnectInd)( int stsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd ) ;
	void	(IrDACB *ConnectCnf)( int stsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status ) ;
	void	(IrDACB *DataInd)( int lsap ) ;
	void	(IrDACB *DataCnf)( int lsap ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int reason ) ;
#if 0
	void	(IrDACB *MDataInd)( int lsap ) ;
	void	(IrDACB *UDataInd)( int lsap, void IrDAFAR *buf, uint size ) ;
	void	(IrDACB *UDataCnf)( int lsap ) ;
#endif
}	TTPCALLBACK ;


#if defined(__cplusplus)
extern "C" {
#endif	/*__cplusplus*/


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	TTP_Open( void ) ;					/*������						*/
void	IrDAAPI	TTP_Close( void ) ;					/*�I��							*/
													/*�|�[�����O�֐��ݒ�			*/

/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	TTP_RequestLSAP( void ) ;			/*��lsap�擾					*/
													/*lsap�擾						*/
int		IrDAAPI	TTP_BindLSAP( int lsap, TTPCALLBACK IrDAFAR *ttpc ) ;
void	IrDAAPI	TTP_UnbindLSAP( int lsap ) ;		/*lsap�J��						*/
int		IrDAAPI	TTP_GetRealLSAP( int lsap ) ;
													/*�ڑ��v��						*/
int		IrDAAPI	TTP_ConnectReq( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd ) ;
													/*�ؒf�v��						*/
int		IrDAAPI	TTP_DisconnectReq( int lsap, void IrDAFAR *data, int datasz ) ;
													/*�f�[�^�]���v��				*/
int		IrDAAPI	TTP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
int		IrDAAPI	TTP_MDataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	TTP_GetRxSize( int lsap ) ;
#if defined(WIN32)		/*MSVC���o�O���Ă�...*/
const void IrDAFAR IrDAAPI *TTP_GetDataPtr( int tsap ) ;
#else	/*WIN32*/
const void IrDAFAR* IrDAAPI TTP_GetDataPtr( int tsap ) ;
#endif	/*WIN32*/
IrDABOOL	IrDAAPI	TTP_GetMBit( int lsap ) ;
void	IrDAAPI	TTP_ReleaseRxQueue( int lsap ) ;
IrDABOOL	IrDAAPI	TTP_QueryTxQueue( int lsap ) ;		/*�󂫑��M�L���[����			*/
uint	IrDAAPI	TTP_QeuryRxQueue( int lsap ) ;		/*��M�ς݃L���[����			*/
uint	IrDAAPI	TTP_GetMaxRxSize( int lsap ) ;		/*�ő��M�ʒm�T�C�Y�擾		*/
uint	IrDAAPI	TTP_GetMaxTxSize( int lsap ) ;		/*�ő呗�M�\�T�C�Y�擾		*/


/*  -----  For Indication  -----  */

void	IrDAAPI	TTP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


/*
**	�ύX�F
**	TTP_GetMaxDataIndSize	->	TTP_GetMaxRxSize
**	TTP_ChkTxData			->	TTP_QueryTxQueue
**		(IrDABOOL)					(uint)
**	TTP_LocalFlowReq		->	�p�~�i�����Ńt���[������s���悤�ɂ����j
**
**	�V�K
**		TTP_QeuryRxQueue
**		TTP_GetRxSize
**		TTP_GetDataPtr
**		TTP_ReleaseRxQueue
**
**	��M�o�b�t�@����ʑw�����킸�ɁA�����Ŏ��悤�ɕύX�i2048Bytes�j
*/

#if defined(__cplusplus)
}
#endif	/*__cplusplus*/


#endif	/*__tinytp_h__*/