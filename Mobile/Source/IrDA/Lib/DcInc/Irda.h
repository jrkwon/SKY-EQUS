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
 *     $Header: irda.hv  171.28  99/03/12 07:24:56  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		DeepCore定義														*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


/*
**	このヘッダはwindows.hの後にインクルードする必要があります。
*/

#if !defined(__irda_h__)
#define	__irda_h__

#include "comdef.h"


/*  -----  Compile Switches  -----  */
/*
**	DMSG					Use Debug Messages
**	SEQCHK					データチェック用のシーケンサを使用
**	TxPTRCHK				送信ウィンドウの管理ポインタのチェックを行う
**	USERDBG					for user debug
**
**	MAKE_DOCUMENT			「C++工房」でドキュメントを作成する際に必須
**	IrDANOSTATIC			ユーザードキュメント作成用にローカル関数、ローカル変数をジェネレートしない。
**
**	NOTXMULTIWINDOW			送信時に複数フレームを連続して送信しない
**	IrLAPMAXINFRAMERETRYCOUNT P/F Timer, WD TimerのInFrame時の割り込み回避の為の回数（デフォルト10回）
**	IrLAPNONEWADDR			NEW ADDRESSをサポートしない。
**	AUTONEWADDRHIST			アドレス衝突処理の履歴テーブル数（デフォルト：16）
**	IrLAPNEWADDRRETRYCOUNT	アドレス衝突時の自動リトライ回数（デフォルト3回）
**
**	IrDAPRIMARY				Use Primary
**	IrDASECONDARY			Use Secondary
**	IrDANEWADDRBYDISCOVERY	ディスカバリの度にデバイスアドレスを振り直す
**	IrDASWAP				Swappable
**	IrDA_ACCEPTBEFORERNR	RR送信後にLOCALBUSYを設定された場合に、次の受信フレームがIFrameだったら受け付けるようにする
**	IrDA_DELAIEDSECONDARY	２次局接続時、送信すべきフレームがRR/RNRだった場合にある程度時間をあけてから送信出来るようにする
**	IrDANOINCLUDEDCVER		DeepCore のバージョンをIASで問い合わせられる "IrDA:OSW", "DeepCoreVersion"を入れない
**
**	TIME_SNIFFSENSE			Sniff Open動作前のIrBUS監視時間
**	TIME_SNIFF				Sniff Open時のSniff-XID-Rsp送出後のIrBUS監視時間
**	TIME_SLEEP				Sniff Open時のスリープタイマー値
**	TIME_WAITSNIFFXIDRSP	Sniff Open時のSniff-XID-Rsp待ち時間
**
**	IrLAPSTRICTMINTAT		IrLAP最小ターンアラウンドタイムで0.001ms以上1ms未満の値は1msとする。
**	IrLAPMINTATMICRO		IrLAP最小ターンアラウンドタイムで1ms未満の値をサポートする。
**
**	LSAPMAX					IrLMP LSAP数
**
**	IASNOHIDDENCONTROL		IASエントリの可視属性コントロールをサポートしない。
**	IASANSWERBUFSZ			IAS応答パケット組み立てバッファサイズ
**	IASCLASSNAMEMAX			IASに登録するクラス名の最大サイズ（デフォルト：32）
**	IASATTRNAMEMAX			IASに登録するアトリビュート名の最大サイズ（デフォルト：32）
**	IASATTRVALUEMAX			IASに登録する値の最大サイズ（デフォルト：32）
**	IASCLASSMAX				IASに登録するクラス数
**	IASATTRMAX				IASに登録するアトリビュート数
**
**	TTPMAX					TinyTP LSAP数
**	TTPRxBUF				受信FIFO用バッファサイズ。受信するI Frameよりも大きくなければならない。（デフォルト：2048）
**
**	IrCOMMNO3WIRERAW		IrCOMM 3wire rawを使用しない。
**	NOTTP					IrCOMM cooked modeを使用しない。
**	COMMMAX					IrCOMM LSAP数
**	COMMTXSDUSZ				IrCOMM 送信パケット組み立てバッファサイズ
**	CTLQUEUESZ				IrCOMM コントロールキューバッファサイズ
**
**	IrDA_LDiv				32bitの除算を行うマクロ
**	IrDA_LMul				32bitの乗算を行うマクロ
**	IrDA_LSift				32bitの左シフトを行うマクロ
**
**	IrDA_memcmp				メモリ間比較マクロ
**	IrDA_memcpy				メモリ間コピーマクロ
**	IrDA_memset				メモリセットマクロ
**
**	★任意
**	IrLAP_MORATORIUMTIME	IrLAP1次局となった場合に最後にIFrameを受信してから、RRCmdをPタイマによる遅延させて送信するようになるまでの時間（デフォルト：1000）
**	IrLAPSLOTTIME			Discovery時のスロット間隔
**	IrLAPNOUSEDISCTIMER		Primary時の切断までの時間をRRCmd送出回数で決定する
**
**	IrLAPNORESET			reset処理をサポートしない。
**	IrLAPNORXCHG			Role Exchange をサポートしない。
**	IrLAPNOSNIFF			SNIFF Openをサポートしない。
**	IrLAPNOUI				UI Dataの処理を省く
**	IrLAPFRMRDISC			FRMRを受信した場合にリセットを行わずに切断する。
**
**	IrLMPNOUI				UI Dataの処理を省く
**	IrOBEXNOULTRA			IrOBEX under ULTRAをサポートしない。
**
**	★任意（サイズ関連)
**	OBEXTXBUFSZ				IrOBEX送信パケット組み立てバッファサイズ（デフォルト：2048)
**	OBEXRXBUFSZ				IrOBEX受信パケット組み立てバッファサイズ（デフォルト：2048)
**
**	★GENOA Test System 対策
**	IrDAFORGENOASIRTEST		GENOA対策を全て含む
**	GENOA_A1120				DiscoveryCmdを取りこぼした際にすばやく対処出
**							切るようにしたチューニングが、GENOA A1120で
**							バグと報告されてしまっている。
**	GENOA_T8107				本来、TESTCmdに対するTESTRspは、受信したデー
**							タを返せるだけ返すように規定されている。その
**							為、内部コマンドバッファが64Byteなのでその分
**							だけ返しているが、GENOA では全てを返さないと
**							Failと報告されてしまう。
**
**	★パフォーマンス計測用関数マクロ
**		以下の関数マクロはすべての次のプロトタイプとする。
**			void	a( int f ) ;
**		引数fは関数呼び出し前か復帰後かを表しており、0が呼び出し前、1が復帰
**		後となっている。
**	IrDA_LAPDATACNFPRB		LAPのデータコンファーム呼び出し前後に呼ばれるマクロ。
**	IrDA_LAPDATAINDPRB		LAPのデータインディケート呼び出し前後に呼ばれるマクロ。
**	IrDA_LAPTXEMPTYPRB		LAPのTxEmptyコールバック呼び出し前後に呼ばれるマクロ。
**	IrDA_LMPDATACNFPRB		LMPのデータコンファーム呼び出し前後に呼ばれるマクロ。
**	IrDA_LMPDATAINDPRB		LMPのデータインディケーション呼び出し前後に呼ばれるマクロ。
**	IrDA_TTPDATACNFPRB		TTPのデータコンファーム呼び出し前後に呼ばれるマクロ。
**	IrDA_TTPDATAINDPRB		TTPのデータインディケーション呼び出し前後に呼ばれるマクロ。
**	IrDA_COMDATAINDPRB		COMMのデータインディケーション呼び出し前後に呼ばれるマクロ。
**
**	IrDA_LMPDATAREQPRB		LMPのデータ転送要求API実行前後で呼ばれるマクロ
**	IrDA_TTPDATAREQPRB		TTPのデータ転送要求API実行前後で呼ばれるマクロ
**	IrDA_COMMDATAREQPRB		IrCOMMのデータ転送要求API実行前後で呼ばれるマクロ
*/


/*  -----  for other bugs  -----  */
/*	IrDAFORTRANXIT01	TRANXIT(IBMIR.DLL)の速度に関するネゴシエーショ
**						ンパラメータのバグを回避。
**	IrDABROKENCOMPILER01	switch文の中で byte から int への符号拡張が
**							正しく行えないコンパイラ用。以下、必要なコン
**							パイラ。
**							V850 Family Optimize C Compiler V1.10 [13 Aug 1996]
*/
#define	IrDAFORTRANXIT01


/*  -----  for GENOA Test Suites  -----  */
#if defined(IrDAFORGENOASIRTEST)
#undef	GENOA_A1120
#define	GENOA_A1120
#undef	GENOA_T8107
#define	GENOA_T8107
#endif	/*IrDAFORGENOASIRTEST*/

/*	-----  Version Control  -----  */
/*
**	バージョン間の差を吸収する為に、IrDAVERに現在のバージョン番号を定
**	義する必要があります。
*/
#define	IrDAVER000	200
/*
**		現在のバージョン
*/
/*#define	IrDAVER	IrDAVER200*/


/*  -----  for Swappable module  -----  */

#if defined(IrDASWAP)
#  if !defined(IrDAAPI)
#    define	IrDAAPI		__far __cdecl
#  endif	/*!IrDAAPI*/
#  if !defined(IrDAFAR)
#    define	IrDAFAR		__far
#  endif	/*!IrDAFAR*/
#  if !defined(IrDACB)
#    define	IrDACB		__far __cdecl
#  endif	/*!IrDACB*/
#endif	/*IrDASWAP*/

#if 0
#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/
extern	void	IrDACB	IrDA_SwapIn( void ) ;
extern	void	IrDACB	IrDA_SwapOut( void ) ;
#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/
#endif


#if !defined(MAKE_DOCUMENT)
#if !defined(IrDAAPI)
#define		IrDAAPI
#endif	/*!IrDAAPI*/
#if !defined(IrDAFAR)
#define		IrDAFAR
#endif	/*!IrDAFAR*/
#if !defined(IrDACB)
#define		IrDACB
#endif	/*!IrDACB*/
#endif	/*!MAKE_DOCUMENT*/
#if !defined(IrDAMISCAPI)
#define		IrDAMISCAPI
#endif	/*!IrDAMISCAPI*/


/*  -----  typedefs  -----  */

#if !defined(__byte_defined)
#define	__byte_defined
//htgtypedef	unsigned char		byte ;
#endif	/*!defined(__byte_defined)*/
#if !defined(__irdabool_defined__)
#define	__irdabool_defined__
typedef	unsigned int		IrDABOOL ;
#endif	/*defined(__irdabool_defined__)*/
#if !defined(__word_defined__)
#define	__word_defined__
//htg typedef	unsigned short		word ;					/*2byte 以上のunsigned	*/
#endif	/*defined(__word_defined__)*/
#if !defined(__dword_defined__)
#define	__dword_defined__
//htgtypedef	unsigned long		dword ;
#endif	/*defined(__dword_defined__)*/
#if !defined(__uint_defined__)
#define	__uint_defined__
typedef	unsigned int		uint ;
#endif	/*defined(__uint_defined__)*/
#if !defined(__ulong_defined__)
#define	__ulong_defined__
typedef	unsigned long		ulong ;
#endif	/*defined(__ulong_defined__)*/

typedef	void (IrDACB *IrDACBP)( void ) ;


/*  -----  return status  -----  */

#define	IrDAR_DONE			1					/*要求終了					*/
#define	IrDAR_ACCEPT		0					/*Accept Request			*/
#define	IrDAR_NOCONNECT		-1					/*not connecting			*/
#define	IrDAR_NORESPONSE	-2					/*no response				*/
#define	IrDAR_DISCONNECT	-3					/*cancel by DISCONNECT		*/
#define	IrDAR_CONNECTING	-4					/*接続中					*/
#define	IrDAR_MEDIABUSY		-5					/*メディアビジー			*/
#define	IrDAR_EPARM			-6					/*Error Parameter			*/
#define	IrDAR_INVALIDVER	-7
#define	IrDAR_NOPROTOCOL	-8
#define	IrDAR_INUSE			-9					/*In Used					*/
#define	IrDAR_PRIMARY		-10					/*プライマリに移行した		*/
#define	IrDAR_DENY			-11					/*失敗						*/

#if 1
#define	IrDAR_BUSYWAIT		-17					/*今は忙しいのでリトライせよ*/
#define	IrDAR_NORESOURCE	-16					/*要求されたリソースが無い	*/
#define	IrDAR_TOOLONG		-15					/*要求が大きすぎる			*/
#define	IrDAR_BUFFULL		-14					/*バッファフル				*/
#define	IrDAR_TIMEOUT		-13					/*タイムアウト				*/
#define	IrDAR_INVALID		-12					/*要求拒否					*/
#define	IrDAR_DISCOVERY		11					/*ディスカバリ中			*/
#endif


/*  -----  OEM ID  -----  */

#define	MAKER_SII		0x00
#define	MAC_BP			0x00
#define	OEMID_SIIBP		((MAKER_SII<<8)+MAC_BP)
#define	OEMID_MITS10	0x0200
#define	OEMID_PUBLIC	0xFFFF


/*  -----  misc  -----  */

#ifndef YES
    #define	NO					0
    #define	YES					1
#endif
#ifndef TRUE
    #define	FALSE				0
    #define	TRUE				1
#endif
#ifndef ON
    #define	OFF					0
    #define	ON					1
#endif
#define	IrLAP_NNMSZ			32
#define	CLESSLSAP			0x70


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		局能力管理テーブル													*/
/*																			*/
/* 2.概要																	*/
/*		IrLAPの局能力を管理する。											*/
/*																			*/
/* 3.機能説明																*/
/*		IrLAPの局能力を管理する。											*/
/*																			*/
/* 4.備考																	*/
/*		各メンバはIrLAP1.1で定義されているビットマップで管理されている。	*/
/*		その為、参照、変更を行う際には各メンバ毎に以下の定義を使用する		*/
/*		事。																*/
/*																			*/
/*			LineCapability		IrLAP_BPS_*									*/
/*			MaxTanaroundTime	IrLAP_MaxTurn_*								*/
/*			FrameSize			IrLAP_FS_*									*/
/*			WindowSize			IrLAP_WS_*									*/
/*			NoofBOF				IrLAP_BOF_*									*/
/*			MinTurnaroundTime	IrLAP_MinTurn_*								*/
/*			DisconnectTime		IrLAP_SL_*									*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_PORTCONFIG	{
	byte			LineCapability ;				/*0x01:速度				*/
	byte			MaxTanaroundTime ;				/*0x82:最大ターンアラウンドタイム					*/
	byte			FrameSize ;						/*0x83:データサイズ		*/
	byte			WindowSize ;					/*0x84:ウィンドウ数		*/
	byte			NoofBOF ;						/*0x85:追加BOF数		*/
	byte			MinTurnaroundTime ;				/*0x86:最少ターンアラウンドタイム*/
	byte			DisconnectTime ;				/*0x08:切断時間			*/
}	PORTCONFIG ;


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		局能力管理テーブル(16bit版)											*/
/*																			*/
/* 2.概要																	*/
/*		IrLAPの局能力を管理する。											*/
/*																			*/
/* 3.機能説明																*/
/*		IrLAPの局能力を管理する。											*/
/*																			*/
/*		当構造体は、PORTCONFIG の16bit版である。その為、各メンバが16bit		*/
/*		に拡張されている事を除けば、PORTCONFIG と同様である。				*/
/*																			*/
/* 4.備考																	*/
/*		各メンバはIrLAP1.1で定義されているビットマップで管理されている。	*/
/*		その為、参照、変更を行う際には各メンバ毎に以下の定義を使用する		*/
/*		事。																*/
/*																			*/
/*			LineCapability		IrLAP_BPS_*									*/
/*			MaxTanaroundTime	IrLAP_MaxTurn_*								*/
/*			FrameSize			IrLAP_FS_*									*/
/*			WindowSize			IrLAP_WS_*									*/
/*			NoofBOF				IrLAP_BOF_*									*/
/*			MinTurnaroundTime	IrLAP_MinTurn_*								*/
/*			DisconnectTime		IrLAP_SL_*									*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_PORTCONFIG2 {
	word			LineCapability ;				/*0x01:速度				*/
	word			MaxTanaroundTime ;				/*0x82:最大ターンアラウンドタイム					*/
	word			FrameSize ;						/*0x83:データサイズ		*/
	word			WindowSize ;					/*0x84:ウィンドウ数		*/
	word			NoofBOF ;						/*0x85:追加BOF数		*/
	word			MinTurnaroundTime ;				/*0x86:最少ターンアラウンドタイム*/
	word			DisconnectTime ;				/*0x08:切断時間			*/
}	PORTCONFIG2 ;

#define		IrLAP_BPS_2400			0x01
#define		IrLAP_BPS_9600			0x02
#define		IrLAP_BPS_19200			0x04
#define		IrLAP_BPS_38400			0x08
#define		IrLAP_BPS_57600			0x10
#define		IrLAP_BPS_115200		0x20
#define		IrLAP_BPS_576000		0x40
#define		IrLAP_BPS_1152000		0x80
#define		IrLAP_BPS_4000000		0x0100
#define		IrLAP_MaxTurn_500ms		0x01
#define		IrLAP_MaxTurn_250ms		0x02
#define		IrLAP_MaxTurn_100ms		0x04
#define		IrLAP_MaxTurn_50ms		0x08
#define		IrLAP_MaxTurn_25ms		0x10
#define		IrLAP_MaxTurn_10ms		0x20
#define		IrLAP_MaxTurn_5ms		0x40
#define		IrLAP_FS_64				0x01
#define		IrLAP_FS_128			0x02
#define		IrLAP_FS_256			0x04
#define		IrLAP_FS_512			0x08
#define		IrLAP_FS_1024			0x10
#define		IrLAP_FS_2048			0x20
#define		IrLAP_WS_1				0x01
#define		IrLAP_WS_2				0x02
#define		IrLAP_WS_3				0x04
#define		IrLAP_WS_4				0x08
#define		IrLAP_WS_5				0x10
#define		IrLAP_WS_6				0x20
#define		IrLAP_WS_7				0x40
#define		IrLAP_BOF_48			0x01
#define		IrLAP_BOF_24			0x02
#define		IrLAP_BOF_12			0x04
#define		IrLAP_BOF_5				0x08
#define		IrLAP_BOF_3				0x10
#define		IrLAP_BOF_2				0x20
#define		IrLAP_BOF_1				0x40
#define		IrLAP_BOF_0				0x80
#define		IrLAP_MinTurn_10ms		0x01
#define		IrLAP_MinTurn_5ms		0x02
#define		IrLAP_MinTurn_1ms		0x04
#define		IrLAP_MinTurn_05ms		0x08
#define		IrLAP_MinTurn_01ms		0x10
#define		IrLAP_MinTurn_005ms		0x20
#define		IrLAP_MinTurn_001ms		0x40
#define		IrLAP_MinTurn_0ms		0x80
#define		IrLAP_SL_3s				0x01
#define		IrLAP_SL_8s				0x02
#define		IrLAP_SL_12s			0x04
#define		IrLAP_SL_16s			0x08
#define		IrLAP_SL_20s			0x10
#define		IrLAP_SL_25s			0x20
#define		IrLAP_SL_30s			0x40
#define		IrLAP_SL_40s			0x80


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		局発見ログ															*/
/*																			*/
/* 2.概要																	*/
/*		ディスカバリの結果を格納するログ									*/
/*																			*/
/* 3.機能説明																*/
/*		ディスカバリの結果を格納するログ									*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_DISCOVERYLOG {
	dword		Address ;							/*装置アドレス			*/
	uint		DeviceNameSize ;					/*装置名サイズ			*/
	byte		DeviceName[IrLAP_NNMSZ] ;			/*装置名				*/
}	DISCOVERYLOG ;


/*  -----  Response return code  -----  */

#define		IrDA_CONNECTDI		-1
#define		IrDA_CONNECTEN		0
#define		IrDA_RESETDI		-1
#define		IrDA_RESETEN		0
#define		IrDA_DENY			-1
#define		IrDA_ACCEPT			0


/*  -----  functiones  -----  */

#if !defined(IrDA_memcpy)
#define	IrDA_memcpy				memcpy
#endif	/*!IrDA_memcpy*/
#if !defined(IrDA_memcmp)
#define	IrDA_memcmp				memcmp
#endif	/*!IrDA_memcmp*/
#if !defined(IrDA_memset)
#define	IrDA_memset				memset
#endif	/*!IrDA_memset*/
#if !defined(IrDA_LDiv)
#define	IrDA_LDiv( src, des )	((src)/(des))
#endif	/*!IrDA_LDiv*/
#if !defined(IrDA_LMul)
#define	IrDA_LMul( src, des )	((src)*(des))
#endif	/*!IrDA_LMul*/
#if !defined(IrDA_LSift)
#define	IrDA_LSift( src, cnt )	((src)<<(cnt))
#endif	/*!IrDA_LSift*/


/*  -----  macros  -----  */

#if !defined(UNREFERENCE)
# define	UNREFERENCE(i)	i=i
#endif	/*!UNREFERENCE*/


#if defined(__cplusplus)
extern	"C"	{
#endif	/*defined(__cplusplus)*/


/*  -----  public functions  -----  */

void		IrDAAPI	IrDA_SetRandomSeed( dword seed ) ;
uint		IrDAAPI	IrDA_GetOEMID( void ) ;
													/*送信バッファ設定									*/
void		IrDAAPI	IrDA_SetTxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													/*受信バッファ設定									*/
void		IrDAAPI	IrDA_SetRxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													/*局能力取得										*/
void		IrDAAPI	IrDA_GetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*局能力設定										*/
void		IrDAAPI	IrDA_SetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*接続状態取得										*/
int			IrDAAPI	IrDA_GetConnPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													/*局能力取得										*/
void		IrDAAPI	IrDA_GetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													/*局能力設定										*/
void		IrDAAPI	IrDA_SetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													/*接続状態取得										*/
int			IrDAAPI	IrDA_GetConnPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
IrDABOOL	IrDAAPI	IrDA_GetPowerMode( void ) ;
void		IrDAAPI	IrDA_PowerOn( void ) ;
void		IrDAAPI	IrDA_PowerOff( void ) ;
void		IrDAAPI	IrDA_SetPowerLevel( uint level ) ;
uint		IrDAAPI	IrDA_GetPowerLevel( void ) ;
void		IrDA_ProtocolCallBack( void ) ;

#if defined(__cplusplus)
}
#endif	/*defined(__cplusplus)*/


/*  -----  For Debugging  -----  */

#if	defined(DMSG)
#define	DL_TYPE		0xFF00
#define	DL_MODE		0x00FF
#define	DL_LAP		0x0100
#define	DL_LMP		0x0200
#define	DL_TTP		0x0400
#define	DL_COMM		0x0800
#define	DL_MASS		0x1000
#define	DL_MISC		0x2000
#define	DL_SIR		0x3000
#define	DL_USER		0x4000
#define	DL_DANGER	0x0001
#define	DL_WARN		0x0002
#define	DL_INFO		0x0004
#define	DL_DAT		0x0008

#define	DL_SIRDANGER	(DL_SIR|DL_DANGER)
#define	DL_SIRDAT		(DL_SIR|DL_DAT)
#define	DL_SIRWARN		(DL_SIR|DL_WARN)
#define	DL_SIRINFO		(DL_SIR|DL_INFO)
#define	DL_LAPDANGER	(DL_LAP|DL_DANGER)
#define	DL_LAPDAT		(DL_LAP|DL_DAT)
#define	DL_LAPWARN		(DL_LAP|DL_WARN)
#define	DL_LAPINFO		(DL_LAP|DL_INFO)
#define	DL_LMPDANGER	(DL_LMP|DL_DANGER)
#define	DL_LMPDAT		(DL_LMP|DL_DAT)
#define	DL_LMPWARN		(DL_LMP|DL_WARN)
#define	DL_LMPINFO		(DL_LMP|DL_INFO)
#define	DL_TTPDANGER	(DL_TTP|DL_DANGER)
#define	DL_TTPDAT		(DL_TTP|DL_DAT)
#define	DL_TTPWARN		(DL_TTP|DL_WARN)
#define	DL_TTPINFO		(DL_TTP|DL_INFO)
#define	DL_COMMDANGER	(DL_COMM|DL_DANGER)
#define	DL_COMMDAT		(DL_COMM|DL_DAT)
#define	DL_COMMWARN		(DL_COMM|DL_WARN)
#define	DL_COMMINFO		(DL_COMM|DL_INFO)
#define	DL_MASSDANGER	(DL_MASS|DL_DANGER)
#define	DL_MASSDAT		(DL_MASS|DL_DAT)
#define	DL_MASSWARN		(DL_MASS|DL_WARN)
#define	DL_MASSINFO		(DL_MASS|DL_INFO)
#define	DL_MISCDANGER	(DL_MISC|DL_DANGER)
#define	DL_MISCDAT		(DL_MISC|DL_DAT)
#define	DL_MISCWARN		(DL_MISC|DL_WARN)
#define	DL_MISCINFO		(DL_MISC|DL_INFO)
#define	DL_USERDANGER	(DL_USER|DL_DANGER)
#define	DL_USERDAT		(DL_USER|DL_DAT)
#define	DL_USERWARN		(DL_USER|DL_WARN)
#define	DL_USERINFO		(DL_USER|DL_INFO)


#if defined(__cplusplus)
extern "C" {
#endif	/*__cplusplus*/

int		IrDAAPI	IrDBG_Open( void ) ;
void	IrDAAPI	IrDBG_Close( void ) ;
void	IrDAAPI	_DPUTS( int level, const char IrDAFAR *msg ) ;
void	IrDAAPI	_DPUTC( int level, char data ) ;
void	IrDAAPI	_DPUTB( int level, const char IrDAFAR *msg, byte data ) ;
void	IrDAAPI	_DPUTW( int level, const char IrDAFAR *msg, word data ) ;
void	IrDAAPI	_DPUTD( int level, const char IrDAFAR *msg, dword data ) ;
void	IrDAAPI	_DDUMP( int level, const void IrDAFAR *des, word len, word maxLen ) ;

#if defined(__cplusplus)
}
#endif	/*__cplusplus*/
#endif	/*DMSG*/


#endif	/*__irda_h__*/
