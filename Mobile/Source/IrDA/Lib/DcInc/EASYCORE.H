/********************************************************************

	Easy Core for Sinple use IrDA Protocol stack I/F

	DeepCore (tm) by Okaya Systemware

	Copyright (c) 1996 Okaya Systemware Co.,LTD

	Use EasyCore Interface (IrLPT Easy Interface)

*********************************************************************/

/* $Header: easycore.hv  170.27  98/02/20 14:58:24  core  Exp $ */

#if !defined(__easycore_h__)
#define	__easycore_h__

#include	"irda.h"
#define	IrDAVER	IrDAVER001


/*  -----  Return Code  -----  */

#define		ECR_ACCEPT		0						/*正常終了				*/
#define		ECR_NORESOURCE	-1						/*リソース不足			*/
#define		ECR_NODEVICE	-2						/*接続相手がいない		*/
#define		ECR_NOLSAP		-3						/*接続先にサービスがいない*/
#define		ECR_DISCONNECT	-4						/*切断					*/
#define		ECR_FAIL		-5


/*  -----  Public functions  -----  */

extern	int			DeepCoreIrLPT_Open(dword baud) ;
extern	int			DeepCoreIrLPT_Close(void) ;
extern	int			DeepCoreIrLPT_Flush(void) ;
extern	int			DeepCoreIrLPT_Write( char *data,int size ) ;

extern	int			DeepCoreIrCOMM_EnableBaud( dword baud ) ;
extern	int			DeepCoreIrCOMM_DisableBaud( dword baud ) ;
extern	int			DeepCoreIrCOMM_Open( dword baud ) ;
extern	int			DeepCoreIrCOMM_Open2( dword baud, uint sec ) ;
extern	int			DeepCoreIrCOMM_Close( void ) ;
extern	int			DeepCoreIrCOMM_Read( char *des, uint size ) ;
extern	int			DeepCoreIrCOMM_Write( const char *src, uint size ) ;
extern	int			DeepCoreIrCOMM_Flush( void ) ;
extern	int			DeepCoreIrCOMM_QueryRx( void ) ;
extern	int			DeepCoreIrCOMM_DTROn( void ) ;
extern	int			DeepCoreIrCOMM_DTROff( void ) ;
extern	int			DeepCoreIrCOMM_RTSOn( void ) ;
extern	int			DeepCoreIrCOMM_RTSOff( void ) ;
extern	int			DeepCoreIrCOMM_BreakOn( void ) ;
extern	int			DeepCoreIrCOMM_BreakOff( void ) ;
extern	IrDABOOL	DeepCoreIrCOMM_CheckCD( void ) ;
extern	IrDABOOL	DeepCoreIrCOMM_CheckDSR( void ) ;
extern	IrDABOOL	DeepCoreIrCOMM_CheckCTS( void ) ;
extern	IrDABOOL	DeepCoreIrCOMM_CheckRI( void ) ;
extern	IrDABOOL	DeepCoreIrCOMM_CheckBreak( void ) ;

#endif	/*!__easycore_h__*/
