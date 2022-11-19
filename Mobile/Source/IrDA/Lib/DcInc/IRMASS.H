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
 *     $Header: IRMASS.HV  170.0  97/10/09 16:04:18  core  Exp $
 */


#if !defined(__irmass_h__)
#define	__irmass_h__


#include	"irda.h"


/*  -----  response status  -----  */

#define		IrMASSR_OK		0x00
#define		IrMASSR_INVUNT	0x01
#define		IrMASSR_NOTRDY	0x02
#define		IrMASSR_GENERAL	0xFF


/*  -----  call backs  -----  */

typedef	struct	{
	int		(IrDACB *ConnectInd)( int smsap, dword addr, int dmsap ) ;
	void	(IrDACB *ConnectCnf)( int smsap, dword addr, int dmsap, int status ) ;
	void	(IrDACB *DisconnectInd)( int msap, int status ) ;
	void	(IrDACB *ReadInd)( int msap, dword secno, word count ) ;
	void	(IrDACB *ReadCnf)( int msap, dword secno, word count, int status ) ;
	void	(IrDACB *TxCnf)( int msap ) ;
	void	(IrDACB *MediaChkCnf)( int msap, int status ) ;
	void	(IrDACB *MediaChkInd)( int msap ) ;
//	void	(IrDACB *DataInd)( int msap, const void IrDAFAR *buf, int size ) ;
//	void	(IrDACB *ControlInd)( int msap, void IrDAFAR *control, int controlsz ) ;
}	MASSCALLBACK ;


#if defined(__cplusplus)
extern "C" {
#endif	/*__cplusplus*/


/*  -----  Public Functions for Device  -----  */

int		IrDAAPI	IrMASS_Open( void ) ;
void	IrDAAPI	IrMASS_Close( void ) ;


/*  -----  Public Functions for Service  -----  */

int		IrDAAPI	IrMASS_RequestLSAP( void ) ;		/*ãÛÇ´msapéÊìæ				*/
													/*msapéÊìæ					*/
int		IrDAAPI	IrMASS_BindLSAP( int msap, MASSCALLBACK IrDAFAR *mcb ) ;
void	IrDAAPI	IrMASS_UnbindLSAP( int msap ) ;		/*msapäJï˙					*/
int		IrDAAPI	IrMASS_GetRealLSAP( int msap ) ;
													/*ê⁄ë±óvãÅ					*/
int		IrDAAPI	IrMASS_ConnectReq( int smsap, dword addr, int dmsap ) ;
int		IrDAAPI	IrMASS_DisconnectReq( int msap ) ;/*êÿífóvãÅ					*/
int		IrDAAPI	IrMASS_MediaCheckReq( int msap ) ;
int		IrDAAPI	IrMASS_MediaCheckRsp( int msap, word status ) ;
int		IrDAAPI	IrMASS_DataReq( int msap, dword sec, word count, void IrDAFAR *buf ) ;
int		IrDAAPI	IrMASS_DataRsp( int msap, dword sec, word count, const void IrDAFAR *buf, uint status ) ;


#if defined(__cplusplus)
}
#endif	/*__cplusplus*/

#endif	/*!__irmass_h__*/
