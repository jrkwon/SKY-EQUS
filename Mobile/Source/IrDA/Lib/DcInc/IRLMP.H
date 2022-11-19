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
 *     $Header: irlmp.hv  171.8  98/05/13 14:28:34  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrLMPプロトコル定義													*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irlmp_h__)
#define	__irlmp_h__


#define		FUTURE


#include	"irda.h"


/*  -----  return codes  -----  */

#define	LMPR_BUSYWAIT		IrDAR_BUSYWAIT			/*今は忙しいのでリトライせよ*/
#define	LMPR_NORESOURCE		IrDAR_NORESOURCE		/*要求されたリソースは無い	*/
#define	LMPR_TOOLONG		IrDAR_TOOLONG			/*要求が大きすぎる			*/
#define	LMPR_BUFFULL		IrDAR_BUFFULL			/*バッファフル				*/
#define	LMPR_TIMEOUT		IrDAR_TIMEOUT			/*タイムアウト				*/
#define	LMPR_MEDIABUSY		IrDAR_MEDIABUSY			/*メディアビジー			*/
#define	LMPR_INVALID		IrDAR_INVALID			/*要求拒否					*/
#define	LMPR_ACCEPT			IrDAR_ACCEPT			/*要求受理					*/
#define	LMPR_DISCOVERY		IrDAR_DISCOVERY			/*ディスカバリ中			*/
#define	LMPR_CONNECTING		IrDAR_CONNECTING		/*接続中					*/
#define	LMPR_ENDDISCOVERY	IrDAR_ENDDISCOVERY		/*ディスカバリ終了			*/
#define	LMPR_DONE			IrDAR_DONE				/*送信終了					*/


/*  -----  Service hints  -----  */

#define	IrLMP_SH_PnPCompatible	0					/*PnP Compatible		*/
#define	IrLMP_SH_PDA			1					/*PDA/Palmtop			*/
#define	IrLMP_SH_Computer		2					/*Computer				*/
#define	IrLMP_SH_Printer		3					/*Printer				*/
#define	IrLMP_SH_Modem			4					/*Modem					*/
#define	IrLMP_SH_Fax			5					/*Fax					*/
#define	IrLMP_SH_LAN			6					/*LAN Access			*/
#define	IrLMP_SH_Telephony		(8+0)				/*Telephony				*/
#define	IrLMP_SH_FileServer		(8+1)				/*File Server			*/
#define	IrLMP_SH_IrCOMM			(8+2)				/*IrCOMM				*/
#define	IrLMP_SH_IrOBEX			(8+5)				/*IrOBEX				*/


/*  -----  Character set  -----  */

#define	IrLMP_CS_ASCII		0
#define	IrLMP_CS_ISO8859_1	1
#define	IrLMP_CS_ISO8859_2	2
#define	IrLMP_CS_ISO8859_3	3
#define	IrLMP_CS_ISO8859_4	4
#define	IrLMP_CS_ISO8859_5	5
#define	IrLMP_CS_ISO8859_6	6
#define	IrLMP_CS_ISO8859_7	7
#define	IrLMP_CS_ISO8859_8	8
#define	IrLMP_CS_ISO8859_9	9
#define	IrLMP_CS_UNICODE	255


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		IrLMP接続時ユーザーデータ											*/
/*																			*/
/* 2.概要																	*/
/*		IrLMP接続時にやりとりされるユーザーデータを管理する。				*/
/*																			*/
/* 3.機能説明																*/
/*		IrLMP接続時にやりとりされるユーザーデータを管理する。				*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_LMCONNDAT	{
	byte	IrDAFAR *Data ;							/*User Data											*/
	int				DataSize ;						/*User Data Size									*/
}	LMCONNDAT ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		IrLMPコールバック関数												*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		IrLMPで各種イベント発生時に呼び出すコールバック関数を管理する。		*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_LMCALLBACK	{
	int		(IrDACB *ConnectInd)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
	void	(IrDACB *ConnectCnf)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
	void	(IrDACB *DataInd)( int lsap, const void IrDAFAR *buf, uint size ) ;
	void	(IrDACB	*DataCnf)( int lsap ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
	void	(IrDACB *UDataInd)( int lsap, void IrDAFAR *buf, uint size ) ;
	void	(IrDACB *UDataCnf)( int lsap ) ;
}	LMCALLBACK ;


/*  -----  return status ( for callback )  -----  */

#define	IrLMP_CONNECTEN		IrDA_CONNECTEN
#define	IrLMP_CONNECTDI		IrDA_CONNECTDI


/*  -----  Disconnect Reason  -----  */

#define	IrLMP_DR_UserRequest						0x01
#define	IrLMP_DR_UnexpectedIrLAPDisconnect			0x02
#define	IrLMP_DR_FailedToEstablishIrLAPConnection	0x03
#define	IrLMP_DR_IrLAPReset							0x04
#define	IrLMP_DR_LMInitiatedDisconnect				0x05
#define	IrLMP_DR_LSAPConnection						0x06
#define	IrLMP_DR_NonResponsiveLMClient				0x07
#define	IrLMP_DR_NoAvailableLMClient				0x08
#define	IrLMP_DR_ConnectionHalfOpen					0x09
#define	IrLMP_DR_IllegalSourceAddress				0x0A
#define	IrLMP_DR_UnSpecifiedDisconnectReason		0xFF


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	IrLMP_Open( void ) ;
void	IrDAAPI	IrLMP_Close( void ) ;
													/*ディスカバリー要求								*/
int		IrDAAPI	IrLMP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;

													/*自局装置名設定									*/
int		IrDAAPI	IrLMP_SetShortDeviceName( uint charset, const byte IrDAFAR *name, uint namesz ) ;
													/*自局サービスヒント設定							*/
int		IrDAAPI	IrLMP_SetServiceHint( uint servicehint ) ;
													/*自局サービスヒント解除							*/
int		IrDAAPI	IrLMP_ResetServiceHint( uint servicehint ) ;
													/*サービスヒント検査								*/
IrDABOOL	IrDAAPI	IrLMP_CheckServiceHint( void IrDAFAR *ptr, uint len, uint servicehint ) ;
uint	IrDAAPI	IrLMP_GetDiscoveryName( void IrDAFAR *ptr, uint len ) ;

#if !defined(IrLAPNORXCHG)
int		IrDAAPI	IrLMP_SetPrimaryInd( int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
#endif	/*!IrLAPNORXCHG*/
#if !defined(IrLAPNOSNIFF)
int		IrDAAPI	IrLMP_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_SniffReq( void ) ;
int		IrDAAPI	IrLMP_SniffAbt( void ) ;
#endif	/*!IrLAPNOSNIFF*/


/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	IrLMP_RequestLSAP( void ) ;			/*空きLSAP取得										*/
													/*LSAP割り当て										*/
int		IrDAAPI	IrLMP_BindLSAP( int lsap, LMCALLBACK IrDAFAR *lmc ) ;
void	IrDAAPI	IrLMP_UnbindLSAP( int lsap ) ;		/*LSAP開放											*/
int		IrDAAPI	IrLMP_GetRealLSAP( int lsap ) ;
													/*LSAP接続要求										*/
int		IrDAAPI	IrLMP_ConnectReq( dword addr, int ssel, int dsel, void IrDAFAR *data, int datasz ) ;
													/*LSAP切断要求										*/
int		IrDAAPI	IrLMP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz ) ;
int		IrDAAPI	IrLMP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	IrLMP_GetMaxTxSize( int lsap ) ;	/*最大送信サイズ取得								*/
uint	IrDAAPI	IrLMP_GetMaxRxSize( int lsap ) ;	/*最大受信サイズ取得								*/
IrDABOOL	IrDAAPI	IrLMP_QueryTxQueue( void ) ;		/*空き送信キュー検査								*/
int		IrDAAPI	IrLMP_QueryVirginTxQueue( void ) ;	/*未送信キュー数取得	*/

int		IrDAAPI	IrLMP_QueryConnectedLsap( void ) ;
int		IrDAAPI	IrLMP_DisconnectLink( void ) ;
int		IrDAAPI	IrLMP_DisableIdleTimer( void ) ;
int		IrDAAPI	IrLMP_EnableIdleTimer( void ) ;

#if !defined(IrLMPNOUI)
int		IrDAAPI	IrLMP_CLessDataReq( const void IrDAFAR *bufp, int len ) ;
int		IrDAAPI	IrLMP_SetCLessDataCnf( void (IrDACB* func)( void )) ;
int		IrDAAPI	IrLMP_SetCLessDataInd( void (IrDACB* func)( const void IrDAFAR* data, int datasz )) ;
int		IrDAAPI	IrLMP_QueryMaxCLessTxDataSize( void ) ;
int		IrDAAPI	IrLMP_QueryMaxCLessRxDataSize( void ) ;
#endif	/*!IrLMPNOUI*/


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		IAS ValueByClass応答データ管理テーブル								*/
/*																			*/
/* 2.概要																	*/
/*		IAS ValueByClass応答データ解析情報を管理する。						*/
/*																			*/
/* 3.機能説明																*/
/*		IAS ValueByClass応答データ解析情報を管理する。						*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IASVALUEBYCLASS	{
	uint			ObjectIdentifier ;	/*オブジェクトID					*/
	byte	IrDAFAR	*EncodedValue ;		/*解析した応答データのアドレス		*/
}	IASVALUEBYCLASS ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		IASクライアントコールバック関数										*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		IASクライアントで各種イベント発生時に呼び出すコールバック関数		*/
/*		を管理する。														*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_IASCALLBACK	{
	void	(IrDACB *ConnectCnf)( int lsap, int status ) ;
	void	(IrDACB *ValueCnf)( int lsap, byte IrDAFAR *value, int valuesz ) ;
	void	(IrDACB *DisconnectInd)( int lsap ) ;
}	IASCALLBACK ;

int		IrDAAPI	IrLMP_AddClass( const void IrDAFAR *classnm, uint classsz ) ;
int		IrDAAPI	IrLMP_DeleteClass( int classid ) ;
int		IrDAAPI	IrLMP_AddAttribute( int classid, const void IrDAFAR *attrnm, uint attrsz ) ;
int		IrDAAPI	IrLMP_DeleteAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_SetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetIntAttribute( int attrid, int val ) ;
int		IrDAAPI	IrLMP_SetOctetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetUserStringAttribute( int attrid, uint charset, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetLSAPAttribute( int attrid, int lsap ) ;
#if !defined(IASNOHIDDENCONTROL)
int		IrDAAPI	IrLMP_ShowAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_HiddenAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_ShowClass( int classid ) ;
int		IrDAAPI	IrLMP_HiddenClass( int classid ) ;
#endif	/*!IASNOHIDDENCONTROL*/

int		IrDAAPI	IrLMP_OpenIAS( dword addr, IASCALLBACK IrDAFAR *icb ) ;
int		IrDAAPI	IrLMP_CloseIAS( int lsap ) ;
int		IrDAAPI	IrLMP_GetValueByClass( int lsap, const void IrDAFAR *classnm, uint classsz, const void IrDAFAR *attr, uint attrsz ) ;
int		IrDAAPI	IrLMP_QueryReturnCode( byte IrDAFAR *value, int valuesz ) ;
int		IrDAAPI	IrLMP_QueryValueByClass( byte IrDAFAR *value, int valuesz, IASVALUEBYCLASS IrDAFAR *listp, uint listsz ) ;
int		IrDAAPI	IrLMP_GetIntValue( byte IrDAFAR *value, dword IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetOctetValue( byte IrDAFAR *value, word IrDAFAR *valsz, byte IrDAFAR * IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetUserStringValue( byte IrDAFAR *value, int IrDAFAR *charset, byte IrDAFAR * IrDAFAR *valp ) ;


/*  -----  for indication  -----  */

void	IrDAAPI	IrLMP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


#endif	/*__irlmp_h__*/
