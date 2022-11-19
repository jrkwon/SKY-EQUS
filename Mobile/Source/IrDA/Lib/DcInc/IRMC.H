/************************************************************************/
/*                                                                      */
/*     IrDA Driver DeepCore                                             */
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IrMC.h                                       */
/*             Created:    03/29/97                                     */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/************************************************************************/

/* $Header:	*/

#if !defined(__IrMC_h__)
#define	__IrMC_h__

/*  -----  Include Files  -----  */

#include	"irda.h"
#include	"irlmp.h"

#define	IrMC_CLASS_NAME		"IrDA:TELECOM"
#define	IrMC_LSAP_NAME		"IrDA:TinyTP:LsapSel"
#define	IrMC_PARAM_NAME		"Parameters"

#define	IrMC_TYPE_STD		FALSE
#define	IrMC_TYPE_OTHER		TRUE
#define	IrMC_STD_AUDIOLEN	80

/*  -----  call backs  -----  */

typedef	struct	{
	int		(IrDACB *ConnectInd)(	dword addr, int dlsap ) ;
	void	(IrDACB *ConnectCnf)(	dword addr, int dlsap, int status ) ;
	void	(IrDACB *DisconnectInd)( int status ) ;
	void	(IrDACB *AudioInd)(		int type, const byte IrDAFAR *aud, int size );
	void	(IrDACB *ControlInd)(	int type, const byte IrDAFAR *cmd, int size );
}	MSCALLBACK ;


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	IrMC_Open( void ) ;				/*IrMCオープン		*/
void	IrDAAPI	IrMC_Close( void ) ;			/*IrMCクローズ		*/

/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	IrMC_BindLSAP	( MSCALLBACK IrDAFAR *ccb);					/*	LSAP取得			*/
void	IrDAAPI	IrMC_UnbindLSAP	( void	) ;									/*	LSAP開放			*/
int		IrDAAPI	IrMC_GetRealLSAP( void	) ;									/*	物理LSAP取得		*/

int		IrDAAPI	IrMC_ConnectReq( dword addr, int dlsap ) ;					/*	接続要求			*/
int		IrDAAPI	IrMC_DisconnectReq( void ) ;								/*	切断要求			*/

int		IrDAAPI	IrMC_AudioReq(	int type,const byte IrDAFAR *aud, int size);	/*	ボイス送信要求	*/
int		IrDAAPI	IrMC_ControlReq(int type,const byte IrDAFAR *cmd, int size);	/*	コール送信要求	*/

int		IrDAAPI	IrMC_QueryTxFree( int type );								/* 送信キューの空き領域を取得	*/

int		IrDAAPI	IrMC_QueryRealTime( void );									/* リアルタイム状態の取得	*/


void	IrDAAPI	IrMC_SetRealTime( int sw );							/*  リアルタイムON/OFF	*/

/*  -----  Public Functions for Indication  -----  */

void	IrDAAPI	IrMC_SetConnectPrv( 	void (IrDACB *func)( void )) ;
void	IrDAAPI	IrMC_SetDisconnectPrv( 	void (IrDACB *func)( void )) ;


#endif	/*__IrMC_h__*/

/* $Log: 
*/
