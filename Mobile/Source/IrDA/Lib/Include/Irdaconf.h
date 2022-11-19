/************************************************************************
;*                                                                      *
;*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      *
;*     All Rights Reserved                                              *
;*                                                                      *
;*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  *
;*     The copyright notice above does not evidence any                 *
;*     actual or intended publication of such source code.              *
;*                                                                      *
;*             File:       irdaconf.h                                   *
;*             Created:    03/23/97                                     *
;*             Author:     Gontaro Kitazumi								*
;*                                                                      *
;*             Version:    1.7                                          *
;*                                                                      *
;***********************************************************************/

#if !defined(__irdaconf_h__)
#define	__irdaconf_h__

#include "target.h"                /* Target specific definitions          */

extern	int	IrDA_MoraTime;

#ifdef	FEATURE_IRDA_UPGRADE
#define	USE_SYSTEM_TASKCALLBACK		//Protocol System Running	OSW:Takagaki	2000/01/21 
#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#undef	USE_MONITOR    // HTG				

//#define		DEVICENAME			"(c) 1996 Okaya Systemware"
#define		SHORTDEVICENAME		"SK Teletech"
#define		DEVICENAME			"(C) 2000 SK Teletech"


/*	IrSIR Capability
*/
#define		IrDACAPABILITY		\
	{							\
		0x01, 0x01, 0x3F,		\
		0x82, 0x01, 0x01,		\
		0x86, 0x01, 0xFF		\
	}



#define		USE_TinyTP										/* TinyTP	を使用する			*/
#define		USE_IrCOMM										/* IrCOMM	を使用する			*/
#define		USE_IrOBEX										/* IrOBEX	を使用する			*/
#define		USE_RTCON										/* RTCON	を使用する			*/
//#define		USE_IrTranP									/* IrTranP	を使用する			*/

/* Windows DOS 版 制御	*/

//#define		IrDASWAP									/* IrBIOS で使用する				*/
#define		IrDAAPI											/*	IrDA の API 		マーカー	*/
#define		IrDACB											/*	IrDA の Callback 	マーカー	*/

/*	IrDA 互換性保証	*/

#define		IrDAFORWIN95									/*	Windows 95 バグ回避				*/
#define		IrDAPRIMARY										/*	１次局コード生成				*/

//#define		IrLAPSLOTTIME		80

#define		IrDABROKENCOMPILER01							/*	コンパイラエラー対策			*/

#ifdef	FEATURE_IRDA_UPGRADE


#if MODEL_ID == MODEL_D1
#define		IrLAPRECVSZ			555 //???2048						/*      受信フレームサイズ			*/
#define		IrLAPSENDSZ			3000//2048						/*      送信フレームサイズ          */
#elif MODEL_ID == MODEL_A10
#define		IrLAPRECVSZ			555 //???2048						/*      受信フレームサイズ			*/
#define		IrLAPSENDSZ			2500//2048						/*      送信フレームサイズ          */
#else
#define		IrLAPRECVSZ			555 //???2048						/*      受信フレームサイズ			*/
#define		IrLAPSENDSZ			2048						/*      送信フレームサイズ          */
#endif // #if MODEL_ID == MODEL_D1

#define		IrDARECVADD			4							/*	受信追加バイト数 CRC 等			*/
#define		IrDASENDADD			4							/*	送信追加バイト数 CRC 等			*/

#define		IrLAPRECVWINSIZE	1							/*	受信ウインドウサイズ			*/
#define		IrLAPSENDWINSIZE	1							/*	送信ウインドウサイズ			*/

#else	// #ifdef	FEATURE_IRDA_UPGRADE

#define		IrLAPRECVSZ			555// by chos512							/*      受信フレームサイズ			*/
#define		IrLAPSENDSZ			512							/*      送信フレームサイズ          */

#define		IrDARECVADD			4							/*	受信追加バイト数 CRC 等			*/
#define		IrDASENDADD			4							/*	送信追加バイト数 CRC 等			*/

#define		IrLAPRECVWINSIZE	1							/*	受信ウインドウサイズ			*/
#define		IrLAPSENDWINSIZE	2							/*	送信ウインドウサイズ			*/

#endif	// #ifdef	FEATURE_IRDA_UPGRADE

#define		IrDARECVSZ			(IrLAPRECVSZ+IrDARECVADD)	/*	実質の１受信フレームサイズ		*/
#define		IrDASENDSZ			(IrLAPSENDSZ+IrDASENDADD)	/*	実質の１送信フレームサイズ		*/
#define		IrDARECVBUFSZ		(IrDARECVSZ*IrLAPRECVWINSIZE)	/*	総受信バッファサイズ		*/
#define		IrDASENDBUFSZ		(IrDASENDSZ*IrLAPSENDWINSIZE)	/*	総送信バッファサイズ		*/
#define		IrLAPSTRICTMINTAT	/*	IrLAP最小ターンアラウンドタイムで0.001ms以上1ms未満の値は1msとする	*/

/*		IrLAP 制御		*/

//#define		IrLAPNORXCHG								/*	LAP ロールイクステェンジをサポートしない	*/
#define		IrLAPNOSNIFF								/*	LAP スニフをサポートしない					*/

//#define		IrLAP_MORATORIUMTIME	0				/*	LAP 1次局ですぐに RRを返し続けるの時間		*/

/*		IrLMP 制御		*/

#define		IrLMP_LSAPMAX			5				/*	LMP 登録可能 LSAP + IAS Client数	*/
#define		IrLMP_IDLETIMER			1000			/*	LMP で未接続時にLAPを切る時間		*/

#define		IrLMP_IASCLASSMAX		5				/*	LMP 登録可能クラス数				*/
#define		IrLMP_IASATTRMAX		5				/*	LMP 登録可能アトリビュート数		*/
#define		IASATTRVALUEMAX			32				/*	LMP 値の最大長						*/

/*		TinyTP 制御		*/

#if defined(USE_TinyTP)
#define		TinyTP_LSAPMAX			3				/*	TinyTP 登録可能 LSAP 数				*/
#define		TinyTP_SENDBUFSZ		IrLAPSENDSZ		/*	TinyTP 送信バッファサイズ			*/
#define		TinyTP_SENDBUFMAX		2				/*	TinyTP 送信ＱＵＥサイズ 2以上にする	*/
#endif	/*	USE_TinyTP	*/

#if defined(USE_IrCOMM)
#define		IrCOMM_LSAPMAX			1				/*	IrCOMM	登録可能 LSAP 数			*/
#define		IrCOMM_RXSDUSZ			IrLAPRECVSZ		/*	IrCOMM	受信 SDU サイズ				*/
#define		IrCOMM_TXSDUSZ			IrLAPSENDSZ		/*	IrCOMM	送信 SDU サイズ				*/

// tasik 00.10.18
#ifdef FEATURE_IRDA_HIGH_RATE
#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*6)		/*	IrCOMM	受信 FIFO サイズ			*/
#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*6)		/*	IrCOMM	送信 FIFO サイズ			*/
#else
// chul.edit
#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*4)		/*	IrCOMM	受信 FIFO サイズ			*/
#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*4)		/*	IrCOMM	送信 FIFO サイズ			*/

//#define		IrCOMM_RXQUESZ		(IrLAPRECVSZ*2)		/*	IrCOMM	受信 FIFO サイズ			*/
//#define		IrCOMM_TXQUESZ		(IrLAPSENDSZ*2)		/*	IrCOMM	送信 FIFO サイズ			*/
// chul.end
#endif
// tasik end

#define		IrCOMM_CTLQUESZ		64					/*	IrCOMM	制御 FIFO サイズ			*/
#endif	/*	USE_IrCOMM	*/

#if defined(USE_IrOBEX)
#define		IrOBEX_LSAPMAX			2				/*	IrOBEX	登録可能 LSAP 数			*/
#define		IrOBEX_TXBUFSZ			IrLAPSENDSZ		/*	IrOBEX	送信バッファサイズ			*/
#define		IrOBEX_RXBUFSZ			IrLAPSENDSZ		/*	IrOBEX	受信バッファサイズ			*/
#endif	/*	USE_IrOBEX	*/

#if !defined(IrDA_LDiv)
#define		IrDA_LDiv(a,b)			(a/b)			/*	割り算マクロ						*/
#endif	/*!IrDA_LDiv*/
#define		StartTimer				IrSIR_TimerReq	/*	タイマーセット関数					*/
#define		StopTimer				IrSIR_TimerRem	/*	タイマーリセット関数				*/

#include	<irda.h>

#if	0
#define	IrDA_LAPTXEMPTYPRB(f)		P14.1 = !f	//	LAPのTxEmptyコールバック呼び出し前後に呼ばれるマクロ。
#define	IrDA_LMPDATAINDPRB(f)		P14.2 = !f	//	LMPのデータインディケーション呼び出し前後に呼ばれるマクロ。
#define	IrDA_TTPDATAINDPRB(f)		P14.3 = !f	//	TTPのデータインディケーション呼び出し前後に呼ばれるマクロ。
#define	IrDA_COMDATAINDPRB(f)		P14.4 = !f	//	COMMのデータインディケーション呼び出し前後に呼ばれるマクロ。
#define	IrDA_LMPDATAREQPRB(f)		P14.5 = !f	//	LMPのデータ転送要求API実行前後で呼ばれるマクロ
#define	IrDA_TTPDATAREQPRB(f)		P14.6 = !f	//	TTPのデータ転送要求API実行前後で呼ばれるマクロ
#define	IrDA_COMMDATAREQPRB(f)		P14.7 = !f	//	IrCOMMのデータ転送要求API実行前後で呼ばれるマクロ
#endif

#endif	/*!__irdaconf_h__*/

