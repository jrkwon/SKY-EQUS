


#if !defined(__irlap_h__)
#define	__irlap_h__


#include	"irda.h"


#define	IrLAP_CONNECTEN		IrDA_CONNECTEN
#define	IrLAP_CONNECTDI		IrDA_CONNECTDI


#if defined(__cplusplus)
extern	"C"	{
#endif	


void	IrDAAPI	IrLAP_Open( void ) ;				
void	IrDAAPI	IrLAP_Close( void ) ;				
uint	IrDAAPI	IrLAP_Status( void ) ;				
													
void	IrDAAPI	IrLAP_SetDeviceName( byte const IrDAFAR *name, uint namesz ) ;
													
int		IrDAAPI	IrLAP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;
int		IrDAAPI	IrLAP_ConnectReq( dword addr, void (IrDACB *func)( int status )) ;
int		IrDAAPI	IrLAP_DisconnectReq( void ) ;		
													
int		IrDAAPI	IrLAP_DataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz )) ;


#if !defined(IrLAPNOUI)
int		IrDAAPI	IrLAP_UDataReq( uint (IrDACB *func)( byte IrDAFAR *buf, uint bufsz )) ;
int		IrDAAPI	IrLAP_SetUDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size )) ;
int		IrDAAPI	IrLAP_SetUDataCnf( void (IrDACB *func)( void )) ;
int		IrDAAPI	IrLAP_QueryMaxTxUDataSize( void ) ;
int		IrDAAPI	IrLAP_QueryMaxRxUDataSize( void ) ;
#endif	


													
int		IrDAAPI	IrLAP_SetConnectInd( int (IrDACB *func)( dword addr )) ;
													
int		IrDAAPI	IrLAP_SetDisconnectInd( void (IrDACB *func)( int status )) ;
													
int		IrDAAPI	IrLAP_SetDataCnf( void (IrDACB *func)( uint freebufsz )) ;
													
int		IrDAAPI	IrLAP_SetDataInd( void (IrDACB *func)( const byte IrDAFAR *data, uint size )) ;
#if !defined(IrLAPNORESET)
int		IrDAAPI	IrLAP_SetResetInd( IrDABOOL (IrDACB *func)( void )) ;
#endif	
													
int		IrDAAPI	IrLAP_LocalBusy( IrDABOOL sw ) ;


uint	IrDAAPI	IrLAP_GetMaxRxSize( void ) ;		
uint	IrDAAPI	IrLAP_GetMaxTxSize( void ) ;		
IrDABOOL	IrDAAPI	IrLAP_QueryTxQueue( void ) ;		
int		IrDAAPI	IrLAP_QueryVirginTxQueue( void ) ;	


#if defined(IrDAPRIMARY)
# if !defined(IrLAPNORXCHG)
int		IrDAAPI	IrLAP_SetPrimaryInd( int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLAP_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
# endif	
#endif	


#if !defined(IrLAPNOSNIFF)
int		IrDAAPI	IrLAP_SniffConnectReq( void (IrDACB *func)( int status )) ;
void	IrDAAPI	IrLAP_SetSniffInd( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void )) ;
int		IrDAAPI	IrLAP_SniffReq( void ) ;
int		IrDAAPI	IrLAP_SniffAbt( void ) ;
#endif	


void	IrDAAPI	IrLAP_SetDiscoveryPrv( DISCOVERYLOG IrDAFAR *log, void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetConnectPrv2( void (IrDACB *func)( int type )) ;
void	IrDAAPI	IrLAP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


void	IrDAAPI	IrLAP_SetSnrmPrv( void (IrDACB *func)( void ), PORTCONFIG2 IrDAFAR *pcp ) ;
void	IrDAAPI	IrLAP_SetAplConnectInd( DISCOVERYLOG IrDAFAR *peer, IrDABOOL (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_SetAplLinkInd( void (IrDACB *func)( void ), dword msec ) ;
void	IrDAAPI	IrLAP_SetTxEmptyPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLAP_EnableDiscoveryRsp( void ) ;
void	IrDAAPI	IrLAP_DisableDiscoveryRsp( void ) ;


#if defined(__cplusplus)
}
#endif	


#endif	