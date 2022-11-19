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
/* 概要																		*/
/*		TinyTPプロトコル定義												*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__tinytp_h__)
#define	__tinytp_h__

#include	"irda.h"
#include	"irlmp.h"


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		TinyTP接続時ユーザーデータ											*/
/*																			*/
/* 2.概要																	*/
/*		TinyTP接続時にやりとりされるユーザーデータを管理する。				*/
/*																			*/
/* 3.機能説明																*/
/*		TinyTP接続時にやりとりされるユーザーデータを管理する。				*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_TTPCONNDAT {
	byte	IrDAFAR *Data ;							/*User Data				*/
	int				DataSize ;						/*User Data Size		*/
}	TTPCONNDAT ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		コールバック関数													*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		TinyTPで各種イベント発生時に呼び出すコールバック関数を管理する。	*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
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

int		IrDAAPI	TTP_Open( void ) ;					/*初期化						*/
void	IrDAAPI	TTP_Close( void ) ;					/*終了							*/
													/*ポーリング関数設定			*/

/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	TTP_RequestLSAP( void ) ;			/*空きlsap取得					*/
													/*lsap取得						*/
int		IrDAAPI	TTP_BindLSAP( int lsap, TTPCALLBACK IrDAFAR *ttpc ) ;
void	IrDAAPI	TTP_UnbindLSAP( int lsap ) ;		/*lsap開放						*/
int		IrDAAPI	TTP_GetRealLSAP( int lsap ) ;
													/*接続要求						*/
int		IrDAAPI	TTP_ConnectReq( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd ) ;
													/*切断要求						*/
int		IrDAAPI	TTP_DisconnectReq( int lsap, void IrDAFAR *data, int datasz ) ;
													/*データ転送要求				*/
int		IrDAAPI	TTP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
int		IrDAAPI	TTP_MDataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	TTP_GetRxSize( int lsap ) ;
#if defined(WIN32)		/*MSVCがバグってる...*/
const void IrDAFAR IrDAAPI *TTP_GetDataPtr( int tsap ) ;
#else	/*WIN32*/
const void IrDAFAR* IrDAAPI TTP_GetDataPtr( int tsap ) ;
#endif	/*WIN32*/
IrDABOOL	IrDAAPI	TTP_GetMBit( int lsap ) ;
void	IrDAAPI	TTP_ReleaseRxQueue( int lsap ) ;
IrDABOOL	IrDAAPI	TTP_QueryTxQueue( int lsap ) ;		/*空き送信キュー検査			*/
uint	IrDAAPI	TTP_QeuryRxQueue( int lsap ) ;		/*受信済みキュー検査			*/
uint	IrDAAPI	TTP_GetMaxRxSize( int lsap ) ;		/*最大受信通知サイズ取得		*/
uint	IrDAAPI	TTP_GetMaxTxSize( int lsap ) ;		/*最大送信可能サイズ取得		*/


/*  -----  For Indication  -----  */

void	IrDAAPI	TTP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


/*
**	変更：
**	TTP_GetMaxDataIndSize	->	TTP_GetMaxRxSize
**	TTP_ChkTxData			->	TTP_QueryTxQueue
**		(IrDABOOL)					(uint)
**	TTP_LocalFlowReq		->	廃止（自動でフロー制御を行うようにした）
**
**	新規
**		TTP_QeuryRxQueue
**		TTP_GetRxSize
**		TTP_GetDataPtr
**		TTP_ReleaseRxQueue
**
**	受信バッファを上位層から貰わずに、自分で持つように変更（2048Bytes）
*/

#if defined(__cplusplus)
}
#endif	/*__cplusplus*/


#endif	/*__tinytp_h__*/
