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
 *     $Header: irobex.hv  170.35  98/03/04 12:53:10  core  Exp $
 */


/*--------------------------------------------------------------------------*/
/*																			*/
/* 概要																		*/
/*		IrOBEXプロトコル定義												*/
/*																			*/
/* 作成者																	*/
/*		nagamura@osw.co.jp													*/
/*																			*/
/*--------------------------------------------------------------------------*/


#if !defined(__irobex_h__)
#define	__irobex_h__


#include	"irda.h"


/*  -----  old name convert  -----  */

#define	IrOBEX_GetRsq	IrOBEX_GetRsp	/*I'm sorry, I mistake spelling		*/


/*  -----  OBEX operations consist  -----  */

#define	OBEX_FINALBIT				0x80

#define	OBEXOC_CONNECT				0x80	/*choose your partner, negotiate capabilities*/
#define	OBEXOC_DISCONNECT			0x81	/*signal the end of the session	*/
#define	OBEXOC_PUT					0x02	/*send an object				*/
#define	OBEXOC_GET					0x03	/*get an object					*/
#define	OBEXOC_COMMAND				0x04	/*send a respoonseless packet	*/
#define	OBEXOC_SETPATH				0x85	/*modifies the current path on the receiveing side*/
#define	OBEXOC_ABORT				0xFF	/*abort the current operation	*/


/*  -----  OBEX Resonse code  -----  */

#define	OBEXRC_CONTINUE				0x10	/*continue						*/
#define	OBEXRC_SWPROTOCOL			0x11	/*switching protocols			*/

#define	OBEXRC_OK					0x20	/*OK, Success					*/
#define	OBEXRC_CREATED				0x21	/*created						*/
#define	OBEXRC_ACCEPTED				0x22	/*accepted						*/
#define	OBEXRC_NONAUTINFO			0x23	/*non-authoritative Information	*/
#define	OBEXRC_NOCONTENT			0x24	/*no content					*/
#define	OBEXRC_RESCONTENT			0x25	/*reset content					*/
#define	OBEXRC_PARTIALCONTENT		0x26	/*partial content				*/

#define	OBEXRC_MULTICHOICE			0x30	/*multiple choices				*/
#define	OBEXRC_MVPERMANENTLY		0x31	/*moved parmanently				*/
#define	OBEXRC_MVTEMP				0x32	/*moved temprarily				*/
#define	OBEXRC_SEEOTHER				0x33	/*see other						*/
#define	OBEXRC_NOTMODIFIED			0x34	/*not modified					*/
#define	OBEXRC_USEPROXY				0x35	/*use proxy						*/

#define	OBEXRC_BADREQUEST			0x40	/*bad request					*/
#define	OBEXRC_UNAUTH				0x41	/*unauthorized					*/
#define	OBEXRC_PAYMENTREQUIRED		0x42	/*payment required				*/
#define	OBEXRC_FORBIDDEN			0x43	/*forbidden						*/
#define	OBEXRC_NOTFOUND				0x44	/*not found						*/
#define	OBEXRC_METHODNOTALLOWED		0x45	/*method not allowed			*/
#define	OBEXRC_NOTACCEPTABLE		0x46	/*not acceptable				*/
#define	OBEXRC_PROXYAUTHREQUIRED	0x47	/*proxy authentication required	*/
#define	OBEXRC_REQTO				0x48	/*request time out				*/
#define	OBEXRC_CONFLICT				0x49	/*conflict						*/
#define	OBEXRC_GONE					0x4A	/*gone							*/
#define	OBEXRC_LENGTHREQUIRED		0x4B	/*length required				*/
#define	OBEXRC_PRECONFAIL			0x4C	/*precondition failed			*/
#define	OBEXRC_REQENTITYLARGE		0x4D	/*requested entity too large	*/
#define	OBEXRC_REQURLLARGE			0x4E	/*requested URL too large		*/
#define	OBEXRC_UNSUPMEDIA			0x4F	/*unsupported media type		*/

#define	OBEXRC_INTERNALERR			0x50	/*internal server error			*/
#define	OBEXRC_NOTIMPLEMENT			0x51	/*not inplemented				*/
#define	OBEXRC_BADGW				0x52	/*bad gateway					*/
#define	OBEXRC_SERVICEUNAVAIL		0x53	/*service unavailable			*/
#define	OBEXRC_GWTO					0x54	/*gateway time out				*/
#define	OBEXRC_VERNOTSUP			0x55	/*version not supported			*/


/*  -----  OBEX header identifiers  -----	*/

#define	OBEXHI_COUNT				0xC0	/*number of objects				*/
#define	OBEXHI_NAME					0x01	/*name of the object			*/
#define	OBEXHI_TYPE					0x42	/*type of object				*/
#define	OBEXHI_LENGTH				0xC3	/*the length of the object in bytes*/
#define	OBEXHI_TIMEISO				0x44	/*date/time stamp - ISO 8601	*/
#define	OBEXHI_TIMEOLD				0xC4	/*date/time stamp - 4byte version*/
#define	OBEXHI_DESC					0x05	/*text description of the object*/
#define	OBEXHI_TARGET				0x46	/*name of service that operation is tageted to*/
#define	OBEXHI_HTTP					0x47	/*an HTTP 1.x header			*/
#define	OBEXHI_BODY					0x48	/*a chunk of the object body	*/
#define	OBEXHI_ENDBODY				0x49	/*the final chunk of the object body*/
#define	OBEXHI_WHO					0x4A	/*identifies the application using OBEX*/


/*--------------------------------------------------------------------------*/
/*																			*/
/* 1.構造体名																*/
/*		コールバック関数													*/
/*																			*/
/* 2.概要																	*/
/*		各種イベント発生時に呼び出すコールバック関数を管理する。			*/
/*																			*/
/* 3.機能説明																*/
/*		IrOBEXで各種イベント発生時に呼び出すコールバック関数を管理する。	*/
/*																			*/
/* 4.備考																	*/
/*		無し																*/
/*																			*/
/*--------------------------------------------------------------------------*/
typedef	struct	_OBEXCALLBACK	{
	void	(IrDACB *ConnectReq)( int lsap, const void IrDAFAR *header, int headersz ) ;
	void	(IrDACB *DisconnectReq)( int lsap, int reason, const void IrDAFAR *header, int headersz ) ;
	void	(IrDACB *PutReq)( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
	void	(IrDACB *GetReq)( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz ) ;
	void	(IrDACB *SetPathReq)( int lsap, byte flag, const void IrDAFAR *header, int headersz ) ;
	void	(IrDACB *AbortReq)( int lsap, const void IrDAFAR *header, int headersz ) ;

	void	(IrDACB *ConnectRsp)( int lsap, int status, const void IrDAFAR *header, int headersz ) ;
	void	(IrDACB *Response)( int lsap, int rsp, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;

	int		(IrDACB *CommandInd)( int lsap, const void IrDAFAR *header, int headersz ) ;
}	OBEXCALLBACK ;


/*  -----  public functions for Device  -----  */

int		IrDAAPI	IrOBEX_Open( void ) ;
int		IrDAAPI	IrOBEX_Close( void ) ;


/*  -----  public functions for LSAP  -----  */

int		IrDAAPI	IrOBEX_RequestLSAP( void ) ;
int		IrDAAPI	IrOBEX_BindLSAP( int lsap, const OBEXCALLBACK IrDAFAR *ocb ) ;
void	IrDAAPI	IrOBEX_UnbindLSAP( int lsap ) ;
int		IrDAAPI	IrOBEX_GetRealLSAP( int lsap ) ;

int		IrDAAPI	IrOBEX_GetMaxTxBodySize( int lsap, int headersz ) ;

int		IrDAAPI	IrOBEX_ConnectReq( int slsap, dword addr, int dlsap, const void IrDAFAR *header, int headersz ) ;
int		IrDAAPI	IrOBEX_ConnectRsp( int lsap, const void IrDAFAR *header, int headersz ) ;
int		IrDAAPI	IrOBEX_DisconnectReq( int lsap, const void IrDAFAR *header, int headersz ) ;

int		IrDAAPI	IrOBEX_SendResponse( int lsap, int rescd, const void IrDAFAR *header, int headersz ) ;

int		IrDAAPI	IrOBEX_GetReq( int lsap, const void IrDAFAR *header, int headersz ) ;
int		IrDAAPI	IrOBEX_GetRsp( int lsap, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;

int		IrDAAPI	IrOBEX_PutReq( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;

int		IrDAAPI	IrOBEX_CommandReq( int lsap, const void IrDAFAR *header, int headersz ) ;
int		IrDAAPI	IrOBEX_SetPathReq( int lsap, byte flag, const void IrDAFAR *header, int headersz ) ;
int		IrDAAPI	IrOBEX_AbortReq( int lsap, const void IrDAFAR *header, int headersz ) ;


#if !defined(IrOBEXNOULTRA)
void	IrDAAPI	IrUOBEX_SetPutReqInd( void (IrDACB *func)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )) ;
void	IrDAAPI	IrUOBEX_SetGetReqInd( void (IrDACB *func)( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz )) ;
int		IrDAAPI	IrUOBEX_SendGetRsp( byte sar, int rescd, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;

void	IrDAAPI	IrUOBEX_SetRspInd( void (IrDACB *func)( byte sar, int rescd, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )) ;
int		IrDAAPI	IrUOBEX_PutReq( byte sar, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz ) ;
int		IrDAAPI	IrUOBEX_GetReq( byte sar, const void IrDAFAR *header, int headersz ) ;
#endif	/*!IrOBEXNOULTRA*/


#endif	/*!__irobex_h__*/
