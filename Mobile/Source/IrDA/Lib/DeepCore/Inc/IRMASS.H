


#if !defined(__irmass_h__)
#define	__irmass_h__


#include	"irda.h"


#define		IrMASSR_OK		0x00
#define		IrMASSR_INVUNT	0x01
#define		IrMASSR_NOTRDY	0x02
#define		IrMASSR_GENERAL	0xFF


typedef	struct	{
	int		(IrDACB *ConnectInd)( int smsap, dword addr, int dmsap ) ;
	void	(IrDACB *ConnectCnf)( int smsap, dword addr, int dmsap, int status ) ;
	void	(IrDACB *DisconnectInd)( int msap, int status ) ;
	void	(IrDACB *ReadInd)( int msap, dword secno, word count ) ;
	void	(IrDACB *ReadCnf)( int msap, dword secno, word count, int status ) ;
	void	(IrDACB *TxCnf)( int msap ) ;
	void	(IrDACB *MediaChkCnf)( int msap, int status ) ;
	void	(IrDACB *MediaChkInd)( int msap ) ;


}	MASSCALLBACK ;


#if defined(__cplusplus)
extern "C" {
#endif	


int		IrDAAPI	IrMASS_Open( void ) ;
void	IrDAAPI	IrMASS_Close( void ) ;


int		IrDAAPI	IrMASS_RequestLSAP( void ) ;		
													
int		IrDAAPI	IrMASS_BindLSAP( int msap, MASSCALLBACK IrDAFAR *mcb ) ;
void	IrDAAPI	IrMASS_UnbindLSAP( int msap ) ;		
int		IrDAAPI	IrMASS_GetRealLSAP( int msap ) ;
													
int		IrDAAPI	IrMASS_ConnectReq( int smsap, dword addr, int dmsap ) ;
int		IrDAAPI	IrMASS_DisconnectReq( int msap ) ;
int		IrDAAPI	IrMASS_MediaCheckReq( int msap ) ;
int		IrDAAPI	IrMASS_MediaCheckRsp( int msap, word status ) ;
int		IrDAAPI	IrMASS_DataReq( int msap, dword sec, word count, void IrDAFAR *buf ) ;
int		IrDAAPI	IrMASS_DataRsp( int msap, dword sec, word count, const void IrDAFAR *buf, uint status ) ;


#if defined(__cplusplus)
}
#endif	


#endif	