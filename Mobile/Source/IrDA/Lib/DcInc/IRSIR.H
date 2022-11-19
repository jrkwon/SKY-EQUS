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
/* 概要																		*/
/*		IrSIR API定義														*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irsir_h__)
#define		__irsir_h__

#include	"irda.h"


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		IrSIRドライバ受信バッファ通知テーブル								*/
/*																			*/
/* 2.概要																	*/
/*		IrSIRドライバに受信バッファ等を通知する為に使用する。				*/
/*																			*/
/* 3.機能説明																*/
/*		IrSIRドライバに受信バッファ等を通知する為に使用する。				*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IrSIRFRAMECONFIG {
				byte	IrDAFAR	*RecvBuffer ;			/*受信バッファアドレス			*/
				uint			RecvBufferSize ;		/*受信バッファサイズ			*/
				uint			TblSize ;				/*テーブル数					*/
	volatile	byte	IrDAFAR	* IrDAFAR *RecvBufTbl ;	/*受信フレームアドレステーブル	*/
	volatile	uint	IrDAFAR	*RecvSizeTbl ;			/*受信サイズテーブル			*/
	volatile	uint	IrDAFAR	*RecvWindow ;			/*受信フレーム数格納アドレス	*/
}	IrSIRFRAMECONFIG ;


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/

/*  -----  public functions  -----  */

int		IrDAAPI	IrSIR_Open( void ) ;
void	IrDAAPI	IrSIR_Close( void ) ;
uint	IrDAAPI	IrSIR_GetCapability( const void IrDAFAR * IrDAFAR *ptr ) ;
													/*データ受信インディケート関数定義			*/
void	IrDAAPI	IrSIR_SetDataInd( void (IrDACB *funcp)( void )) ;
													/*データ送信コンファーム関数定義			*/
void	IrDAAPI	IrSIR_SetDataCnf( void (IrDACB *funcp)( void )) ;
													/*SIRパラメーター初期化						*/
void	IrDAAPI	IrSIR_InitConnectionParameters( void ) ;
void	IrDAAPI	IrSIR_SetSpeed( dword spd ) ;		/*転送速度設定								*/

IrDABOOL	IrDAAPI	IrSIR_Busy( void ) ;				/*最後に受信してから500msたっているかどうか	*/
IrDABOOL	IrDAAPI	IrSIR_InFrame( void ) ;				/*フレーム受信中かどうか					*/
													/*フレーム受信用アドレス設定				*/
void	IrDAAPI	IrSIR_SetFrameBuffer( IrSIRFRAMECONFIG IrDAFAR *frameconfig ) ;
void	IrDAAPI	IrSIR_SetFrameMode( void ) ;		/*フレーム受信モード設定					*/
void	IrDAAPI	IrSIR_SetByteMode( void ) ;			/*バイト受信モード設定						*/
void	IrDAAPI	IrSIR_SetRecvMode( void ) ;			/*受信モードへ以降							*/
void	IrDAAPI	IrSIR_ResetRecvMode( void ) ;		/*フレーム受信モードリセット				*/
													/*コネクションアドレス設定					*/
void	IrDAAPI	IrSIR_SetConnectionAddress( uint ca ) ;

													/*ミニマムターンアラウンドタイム設定		*/
void	IrDAAPI	IrSIR_SetMinimamTurnaroundTime( uint msec ) ;
void	IrDAAPI	IrSIR_SetBOF( uint bofsz ) ;		/*BOF数設定									*/
													/*フレーム送信								*/
void	IrDAAPI	IrSIR_SendFrame( uint datasz, void IrDAFAR *data ) ;

ulong	IrDAAPI	IrSIR_ReadTimer( void ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


#endif	/*__irsir_h__*/
