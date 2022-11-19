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
 *     $Header: ircomm.hv  170.37  98/03/06 13:42:00  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrCOMMプロトコル定義												*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__ircomm_h__)
#define	__ircomm_h__


/*  -----  Include Files  -----  */

#include	"irda.h"
#include	"tinytp.h"


/*  -----  return codes  -----  */

#define	COMMR_BUSYWAIT		TTPR_BUSYWAIT			/*今は忙しいのでリトライせよ*/
#define	COMMR_NORESOURCE	TTPR_NORESOURCE			/*要求されたリソースは無い	*/
#define	COMMR_TOOLONG		TTPR_TOOLONG			/*要求が大きすぎる			*/
#define	COMMR_BUFFULL		TTPR_BUFFULL			/*バッファフル				*/
#define	COMMR_TIMEOUT		TTPR_TIMEOUT			/*タイムアウト				*/
#define	COMMR_MEDIABUSY		TTPR_MEDIABUSY			/*メディアビジー			*/
#define	COMMR_INVALID		TTPR_INVALID			/*要求拒否					*/
#define	COMMR_ACCEPT		TTPR_ACCEPT				/*要求受理					*/
#define	COMMR_DISCOVERY		TTPR_DISCOVERY			/*ディスカバリ中			*/
#define	COMMR_CONNECTING	TTPR_CONNECTING			/*接続中					*/
#define	COMMR_ENDDISCOVERY	TTPR_ENDDISCOVERY		/*ディスカバリ終了			*/
#define	COMMR_DONE			TTPR_DONE				/*送信終了					*/


/*  -----  parameter identifire  -----  */

#define	IrCOMM_PI_SERVICETYPE		0x00	/*Service Type					*/
#define	IrCOMM_PI_PORTTYPE			0x01	/*Port Type						*/
#define	IrCOMM_PI_FLEXEDPORTNAME	0x02	/*flexed port name				*/

#define	IrCOMM_PI_DATARATE			0x10	/*data rate in bits/second		*/
#define	IrCOMM_PI_DATAFORMAT		0x11	/*data format					*/
#define	IrCOMM_PI_FLOWCONTROL		0x12	/*flow control					*/
#define	IrCOMM_PI_XONFLOWCHAR		0x13	/*XON/XOFF character			*/
#define	IrCOMM_PI_ENQFLOWCHAR		0x14	/*ENQ/ACK character				*/
#define	IrCOMM_PI_LINESTATUS		0x15	/*line status					*/
#define	IrCOMM_PI_BREAK				0x16	/*break							*/

#define	IrCOMM_PI_DTELINE			0x20	/*DTE line settings and changes	*/
#define	IrCOMM_PI_DCELINE			0x21	/*DCE line settings and changes	*/
#define	IrCOMM_PI_POLLLINESETTINGS	0x22	/*poll for line settings		*/

#define	IrCOMM_PI_STATUSQUERY		0x30	/*status query					*/
#define	IrCOMM_PI_SETBUSYTIMEOUT	0x31	/*set busy timeout				*/
#define	IrCOMM_PI_1284MODESUPORT	0x32	/*IEEE 1284 mode support		*/
#define	IrCOMM_PI_1284DEVID			0x33	/*IEEE 1284 device ID			*/
#define	IrCOMM_PI_SELECT1284MODE	0x34	/*select IEEE 1284 mode			*/
#define	IrCOMM_PI_1284TRANSFER		0x35	/*IEEE 1284 ECP/EPP data transfer*/
#define	IrCOMM_PI_STATUSQUERYRSP	0x38	/*status query response			*/
#define	IrCOMM_PI_SETBUSYTIMEOUTRSP	0x39	/*set busy timeout response		*/
#define	IrCOMM_PI_1284MODESUPORTRSP	0x3A	/*IEEE 1284 mode support response*/
#define	IrCOMM_PI_1284DEVIDRSP		0x3B	/*IEEE 1284 device ID response	*/
#define	IrCOMM_PI_SELECT1284MODERSP	0x3C	/*select IEEE 1284 mode response*/
#define	IrCOMM_PI_1284TRANSFERRSP	0x3D	/*IEEE 1284 ECP/EPP data transfer response*/


/*  -----  service type  -----
 *		IrCOMM_PI_SERVICETYPE
 */
#define	IrCOMM_ST_3WireRaw			0x01	/*3-wire raw					*/
#define	IrCOMM_ST_3Wire				0x02	/*3-wire						*/
#define	IrCOMM_ST_9Wire				0x04	/*9-wire						*/
#define	IrCOMM_ST_Centro			0x08	/*centro						*/
#define	IrCOMM_ST_DEFAULT			0x00

/*  -----  port type  -----
 *		IrCOMM_PI_PORTTYPE
 */
#define	IrCOMM_PT_SERIAL			0x01	/*serial						*/
#define	IrCOMM_PT_PARALLEL			0x02	/*parallel						*/


/*  -----  data format  -----
 *		IrCOMM_PI_DATAFORMAT
 */
#define	IrCOMM_DF_CLMSK				0x03	/*character length bit mask		*/
#define	IrCOMM_DF_CL5				0x00	/*5bits							*/
#define	IrCOMM_DF_CL6				0x01	/*6bits							*/
#define	IrCOMM_DF_CL7				0x02	/*7bits							*/
#define	IrCOMM_DF_CL8				0x03	/*8bits							*/
#define	IrCOMM_DF_SB1				0x00	/*1stop bit						*/
#define	IrCOMM_DF_SB2				0x04	/*2stop bits					*/
#define	IrCOMM_DF_NP				0x00	/*no parity						*/
#define	IrCOMM_DF_PE				0x08	/*parity enabled				*/
#define	IrCOMM_DF_PTMSK				0x30	/*parity bit mask				*/
#define	IrCOMM_DF_PTODD				0x00	/*parity odd					*/
#define	IrCOMM_DF_PTEVEN			0x10	/*parity even					*/
#define	IrCOMM_DF_PTMARK			0x20	/*parity mark					*/
#define	IrCOMM_DF_PTSPC				0x30	/*parity space					*/


/*  -----  flow control  -----
 *		IrCOMM_PI_FLOWCONTROL
 */
#define	IrCOMM_FC_XIN				0x01	/*XON/XOFF on input				*/
#define	IrCOMM_FC_XOUT				0x02	/*XON/XOFF on output			*/
#define	IrCOMM_FC_RTSIN				0x04	/*RTS/CTS on input				*/
#define	IrCOMM_FC_RTSOUT			0x08	/*RTS/CTS on output				*/
#define	IrCOMM_FC_DSRIN				0x10	/*DSR/DTR on input				*/
#define	IrCOMM_FC_DSROUT			0x20	/*DSR/DTR on output				*/
#define	IrCOMM_FC_ENQIN				0x40	/*ENQ/ACK on input				*/
#define	IrCOMM_FC_ENQOUT			0x80	/*ENQ/ACK on output				*/


/*  -----  line status  -----
 *		IrCOMM_PI_LINESTATUS
 */
#define	IrCOMM_LS_OE				0x02	/*overrun error					*/
#define	IrCOMM_LS_PE				0x04	/*parity error					*/
#define	IrCOMM_LS_FE				0x08	/*framing error					*/


/*  -----  break  -----
 *		IrCOMM_PI_BREAK
 */
#define	IrCOMM_B_MSK				0x01	/*bit mask						*/
#define	IrCOMM_B_CLEAR				0x00	/*clear break					*/
#define	IrCOMM_B_SET				0x01	/*set break						*/


/*	-----  DTE line settings and changes  -----
 *		IrCOMM_PI_DTELINE
 */
#define	IRCOMM_DL_DDTR				0x01	/*delta DTR						*/
#define	IRCOMM_DL_DRTS				0x02	/*delta RTS						*/
#define	IRCOMM_DL_DTR				0x04	/*DTR is high					*/
#define	IRCOMM_DL_RTS				0x08	/*RTS is high					*/


/*  -----  DCE line settings and changes  -----
 *		IrCOMM_PI_DCELINE
 */
#define	IRCOMM_DL_DCTS				0x01	/*delta CTS						*/
#define	IRCOMM_DL_DDSR				0x02	/*delta DSR						*/
#define	IRCOMM_DL_DRI				0x04	/*delta RI						*/
#define	IRCOMM_DL_DCD				0x08	/*delta CD						*/
#define	IRCOMM_DL_CTS				0x10	/*CTS is high					*/
#define	IRCOMM_DL_DSR				0x20	/*DSR is high					*/
#define	IRCOMM_DL_RI				0x40	/*RI is high					*/
#define	IRCOMM_DL_CD				0x80	/*CD is high					*/


/*  -----  Service Types  -----  */

#define		IrCOMM_ST_TypeMask		0x7F
#define		IrCOMM_ST_DTE			0x80

#define		IrCOMM_ST_DEFAULT		0x00
#define		IrCOMM_ST_3WireRawDCE	IrCOMM_ST_3WireRaw
#define		IrCOMM_ST_3WireRawDTE	(IrCOMM_ST_DTE|IrCOMM_ST_3WireRaw)
#define		IrCOMM_ST_3WireDCE		IrCOMM_ST_3Wire
#define		IrCOMM_ST_3WireDTE		(IrCOMM_ST_DTE|IrCOMM_ST_3Wire)
#define		IrCOMM_ST_9WireDCE		IrCOMM_ST_9Wire
#define		IrCOMM_ST_9WireDTE		(IrCOMM_ST_DTE|IrCOMM_ST_9Wire)
#define		IrCOMM_ST_CentroDCE		IrCOMM_ST_Centro
#define		IrCOMM_ST_CentroDTE		(IrCOMM_ST_DTE|IrCOMM_ST_Centro)


/*  -----  return status ( for callback )  -----  */

#define	IrCOMM_CONNECTEN		IrDA_CONNECTEN
#define	IrCOMM_CONNECTDI		IrDA_CONNECTDI


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		コールバック関数													*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		IrCOMMで各種イベント発生時に呼び出すコールバック関数を管理する。	*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_COMMCALLBACK	{
	int		(IrDACB *ConnectInd)( int slsap, dword addr, int dlsap, int servicetype ) ;
	void	(IrDACB *ConnectCnf)( int slsap, dword addr, int dlsap, int status ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int status ) ;
	void	(IrDACB *DataInd)( int lsap, int datasz ) ;
	void	(IrDACB *ControlInd)( int lsap, const void IrDAFAR *control, int controlsz ) ;
}	COMMCALLBACK ;


/*  -----  Control Frame  -----  */

#define	COMCTL_DataRate		0x10


#if defined(__cplusplus)
extern "C" {
#endif	/*__cplusplus*/


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	IrCOMM_Open( void ) ;				/*IrCOMMオープン							*/
void	IrDAAPI	IrCOMM_Close( void ) ;				/*IrCOMMクローズ							*/

/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	IrCOMM_RequestLSAP( void ) ;		/*空きLSAP取得				*/
													/*LSAP取得					*/
int		IrDAAPI	IrCOMM_BindLSAP( int lsap, COMMCALLBACK IrDAFAR *ccb, int servicetype ) ;
void	IrDAAPI	IrCOMM_UnbindLSAP( int lsap ) ;		/*LSAP開放					*/
int		IrDAAPI	IrCOMM_GetRealLSAP( int lsap ) ;
													/*接続要求					*/
int		IrDAAPI	IrCOMM_ConnectReq( int slsap, dword addr, int dlsap ) ;
int		IrDAAPI	IrCOMM_DisconnectReq( int lsap ) ;	/*切断要求					*/
													/*データ送信要求			*/
int		IrDAAPI	IrCOMM_SendData( int lsap, const void IrDAFAR *buf, uint bufsz ) ;
													/*データ転送要求			*/
int		IrDAAPI	IrCOMM_RecvData( int lsap, void IrDAFAR *buf, int bufsz ) ;
													/*ステータス送信要求		*/
int		IrDAAPI	IrCOMM_ControlReq( int lsap, const void IrDAFAR *control, int controlsz ) ;
int		IrDAAPI	IrCOMM_QueryConnectUserData( int lsap, byte IrDAFAR * IrDAFAR *ptr ) ;
int		IrDAAPI	IrCOMM_QueryTxQueue( int lsap ) ;	/*送信可能サイズ取得		*/
int		IrDAAPI	IrCOMM_QueryRxQueue( int lsap ) ;	/*受信可能サイズ取得		*/
int		IrDAAPI	IrCOMM_QueryTxData( int lsap ) ;	/*送信バッファ内データ数取得*/
int		IrDAAPI	IrCOMM_QueryRxData( int lsap ) ;	/*受信バッファ内データ数取得*/


/*  -----  Public Functions for Indication  -----  */

void	IrDAAPI	IrCOMM_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrCOMM_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrCOMM_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	/*__cplusplus*/


#endif	/*__ircomm_h__*/
