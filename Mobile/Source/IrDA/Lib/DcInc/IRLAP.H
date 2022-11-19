/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-1998 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: irlap.hv  171.3  98/03/13 16:18:50  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* �T�v																		*/
/*		IrLAP�v���g�R����`													*/
/*																			*/
/* �쐬��																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irlap_h__)
#define	__irlap_h__

/*  -----  Include Files  -----  */

#include	"irda.h"


/*  -----  return status ( for callback )  -----  */

#define	IrLAP_CONNECTEN		IrDA_CONNECTEN
#define	IrLAP_CONNECTDI		IrDA_CONNECTDI


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  public functions  -----  */

void	IrDAAPI	IrLAP_Open( void ) ;				/*IrLAP�̃I�[�v��									*/
void	IrDAAPI	IrLAP_Close( void ) ;				/*IrLAP�̃N���[�Y									*/
uint	IrDAAPI	IrLAP_Status( void ) ;				/*�ڑ����⍇�킹									*/
													/*���u���̐ݒ�										*/
void	IrDAAPI	IrLAP_SetDeviceName( byte const IrDAFAR *name, uint namesz ) ;
													/*�f�B�X�J�o���[�v��								*/
int		IrDAAPI	IrLAP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;
int		IrDAAPI	IrLAP_ConnectReq( dword addr, void (IrDACB *func)( int status )) ;
int		IrDAAPI	IrLAP_DisconnectReq( void ) ;		/*�ؒf�v��											*/
													/*�f�[�^���M										*/
int		IrDAAPI	IrLAP_DataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz )) ;

#if !defined(IrLAPNOUI)
int		IrDAAPI	IrLAP_UDataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz )) ;
int		IrDAAPI	IrLAP_SetUDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size )) ;
int		IrDAAPI	IrLAP_SetUDataCnf( void (IrDACB *func)( void )) ;
int		IrDAAPI	IrLAP_QueryMaxTxUDataSize( void ) ;
int		IrDAAPI	IrLAP_QueryMaxRxUDataSize( void ) ;
#endif	/*!IrLAPNOUI*/

													/*�R�l�N�g�v���R�[���o�b�N�֐���`					*/
int		IrDAAPI	IrLAP_SetConnectInd( int (IrDACB *func)( dword addr )) ;
													/*�f�B�X�R�l�N�g�v���R�[���o�b�N�֐���`			*/
int		IrDAAPI	IrLAP_SetDisconnectInd( void (IrDACB *func)( int status )) ;
													/*�f�[�^���M���R�[���o�b�N�֐���`					*/
int		IrDAAPI	IrLAP_SetDataCnf( void (IrDACB *func)( uint freebufsz )) ;
													/*�f�[�^��M���R�[���o�b�N�֐���`					*/
int		IrDAAPI	IrLAP_SetDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size )) ;
#if !defined(IrLAPNORESET)
int		IrDAAPI	IrLAP_SetResetInd( IrDABOOL (IrDACB *func)( void )) ;
#endif	/*!IrLAPNORESET*/
													/*LocalBusy ON/OFF									*/
int		IrDAAPI	IrLAP_LocalBusy( IrDABOOL sw ) ;

uint	IrDAAPI	IrLAP_GetMaxRxSize( void ) ;		/*�ő��M�T�C�Y�擾								*/
uint	IrDAAPI	IrLAP_GetMaxTxSize( void ) ;		/*�ő呗�M�T�C�Y�擾								*/
IrDABOOL	IrDAAPI	IrLAP_QueryTxQueue( void ) ;		/*�󂫑��M�L���[����								*/
int		IrDAAPI	IrLAP_QueryVirginTxQueue( void ) ;	/*�����M�L���[���擾	*/

#if defined(IrDAPRIMARY)
# if !defined(IrLAPNORXCHG)
int		IrDAAPI	IrLAP_SetPrimaryInd( int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLAP_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
# endif	/*IrLAPNORXCHG*/
#endif	/*IrDAPRIMARY*/

#if !defined(IrLAPNOSNIFF)
int		IrDAAPI	IrLAP_SniffConnectReq( void (IrDACB *func)( int status )) ;
void	IrDAAPI	IrLAP_SetSniffInd( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void )) ;
int		IrDAAPI	IrLAP_SniffReq( void ) ;
int		IrDAAPI	IrLAP_SniffAbt( void ) ;
#endif	/*!IrLAPNOSNIFF*/


/*  -----  for indication  -----  */

void	IrDAAPI	IrLAP_SetDiscoveryPrv( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetConnectPrv2( void (IrDACB *func)( int type )) ;
void	IrDAAPI	IrLAP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


/*  -----  for Appilication  -----  */

void	IrDAAPI	IrLAP_SetSnrmPrv( void (IrDACB *func)( void ), PORTCONFIG2 IrDAFAR *pcp ) ;
void	IrDAAPI	IrLAP_SetAplConnectInd( DISCOVERYLOG IrDAFAR *peer, IrDABOOL (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetAplLinkInd( void (IrDACB *func)( void ), dword msec ) ;
void	IrDAAPI	IrLAP_SetTxEmptyPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_EnableDiscoveryRsp( void ) ;
void	IrDAAPI	IrLAP_DisableDiscoveryRsp( void ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


#endif	/*!defined(__irlap_h__)*/