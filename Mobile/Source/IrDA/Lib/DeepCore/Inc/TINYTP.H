


#if !defined(__tinytp_h__)
#define	__tinytp_h__


#include	"irda.h"
#include	"irlmp.h"


typedef	struct	_TTPCONNDAT {
	byte	IrDAFAR *Data ;							
	int				DataSize ;						
}	TTPCONNDAT ;


typedef	struct	_TTPCALLBACK	{
	int		(IrDACB *ConnectInd)( int stsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd ) ;
	void	(IrDACB *ConnectCnf)( int stsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *lcd, ulong maxsdu, int status ) ;
	void	(IrDACB *DataInd)( int lsap ) ;
	void	(IrDACB *DataCnf)( int lsap ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int reason ) ;
#if 0
	void	(IrDACB *MDataInd)( int lsap ) ;
	void	(IrDACB *UDataInd)( int lsap, void IrDAFAR *buf, uint size ) ;
	void	(IrDACB *UDataCnf)( int lsap ) ;
#endif
}	TTPCALLBACK ;


#if defined(__cplusplus)
extern "C" {
#endif	


int		IrDAAPI	TTP_Open( void ) ;					
void	IrDAAPI	TTP_Close( void ) ;					
													


int		IrDAAPI	TTP_RequestLSAP( void ) ;			
													
int		IrDAAPI	TTP_BindLSAP( int lsap, TTPCALLBACK IrDAFAR *ttpc ) ;
void	IrDAAPI	TTP_UnbindLSAP( int lsap ) ;		
int		IrDAAPI	TTP_GetRealLSAP( int lsap ) ;
													
int		IrDAAPI	TTP_ConnectReq( int slsap, dword addr, int dlsap, TTPCONNDAT IrDAFAR *tcd ) ;
													
int		IrDAAPI	TTP_DisconnectReq( int lsap, void IrDAFAR *data, int datasz ) ;
													
int		IrDAAPI	TTP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
int		IrDAAPI	TTP_MDataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	TTP_GetRxSize( int lsap ) ;
#if defined(WIN32)		
const void IrDAFAR IrDAAPI *TTP_GetDataPtr( int tsap ) ;
#else	
const void IrDAFAR* IrDAAPI TTP_GetDataPtr( int tsap ) ;
#endif	
IrDABOOL	IrDAAPI	TTP_GetMBit( int lsap ) ;
void	IrDAAPI	TTP_ReleaseRxQueue( int lsap ) ;
IrDABOOL	IrDAAPI	TTP_QueryTxQueue( int lsap ) ;		
uint	IrDAAPI	TTP_QeuryRxQueue( int lsap ) ;		
uint	IrDAAPI	TTP_GetMaxRxSize( int lsap ) ;		
uint	IrDAAPI	TTP_GetMaxTxSize( int lsap ) ;		


void	IrDAAPI	TTP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	TTP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	


#endif	