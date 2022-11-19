


#if !defined(__irobex_h__)
#define	__irobex_h__


#include	"irda.h"


#define	IrOBEX_GetRsq	IrOBEX_GetRsp	


#define	OBEX_FINALBIT				0x80


#define	OBEXOC_CONNECT				0x80	
#define	OBEXOC_DISCONNECT			0x81	
#define	OBEXOC_PUT					0x02	
#define	OBEXOC_GET					0x03	
#define	OBEXOC_COMMAND				0x04	
#define	OBEXOC_SETPATH				0x85	
#define	OBEXOC_ABORT				0xFF	


#define	OBEXRC_CONTINUE				0x10	
#define	OBEXRC_SWPROTOCOL			0x11	


#define	OBEXRC_OK					0x20	
#define	OBEXRC_CREATED				0x21	
#define	OBEXRC_ACCEPTED				0x22	
#define	OBEXRC_NONAUTINFO			0x23	
#define	OBEXRC_NOCONTENT			0x24	
#define	OBEXRC_RESCONTENT			0x25	
#define	OBEXRC_PARTIALCONTENT		0x26	


#define	OBEXRC_MULTICHOICE			0x30	
#define	OBEXRC_MVPERMANENTLY		0x31	
#define	OBEXRC_MVTEMP				0x32	
#define	OBEXRC_SEEOTHER				0x33	
#define	OBEXRC_NOTMODIFIED			0x34	
#define	OBEXRC_USEPROXY				0x35	


#define	OBEXRC_BADREQUEST			0x40	
#define	OBEXRC_UNAUTH				0x41	
#define	OBEXRC_PAYMENTREQUIRED		0x42	
#define	OBEXRC_FORBIDDEN			0x43	
#define	OBEXRC_NOTFOUND				0x44	
#define	OBEXRC_METHODNOTALLOWED		0x45	
#define	OBEXRC_NOTACCEPTABLE		0x46	
#define	OBEXRC_PROXYAUTHREQUIRED	0x47	
#define	OBEXRC_REQTO				0x48	
#define	OBEXRC_CONFLICT				0x49	
#define	OBEXRC_GONE					0x4A	
#define	OBEXRC_LENGTHREQUIRED		0x4B	
#define	OBEXRC_PRECONFAIL			0x4C	
#define	OBEXRC_REQENTITYLARGE		0x4D	
#define	OBEXRC_REQURLLARGE			0x4E	
#define	OBEXRC_UNSUPMEDIA			0x4F	


#define	OBEXRC_INTERNALERR			0x50	
#define	OBEXRC_NOTIMPLEMENT			0x51	
#define	OBEXRC_BADGW				0x52	
#define	OBEXRC_SERVICEUNAVAIL		0x53	
#define	OBEXRC_GWTO					0x54	
#define	OBEXRC_VERNOTSUP			0x55	


#define	OBEXHI_NAME					0x01	
#define	OBEXHI_TYPE					0x42	
#define	OBEXHI_LENGTH				0xC3	
#define	OBEXHI_TIMEISO				0x44	
#define	OBEXHI_TIMEOLD				0xC4	
#define	OBEXHI_DESC					0x05	
#define	OBEXHI_TARGET				0x46	
#define	OBEXHI_HTTP					0x47	
#define	OBEXHI_BODY					0x48	
#define	OBEXHI_ENDBODY				0x49	
#define	OBEXHI_WHO					0x4A	


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


int		IrDAAPI	IrOBEX_Open( void ) ;
int		IrDAAPI	IrOBEX_Close( void ) ;


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
#endif	


#endif	