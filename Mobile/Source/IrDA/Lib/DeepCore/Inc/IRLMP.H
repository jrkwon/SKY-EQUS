


#if !defined(__irlmp_h__)
#define	__irlmp_h__


#define		FUTURE


#include	"irda.h"


#define	LMPR_BUSYWAIT		IrDAR_BUSYWAIT			
#define	LMPR_NORESOURCE		IrDAR_NORESOURCE		
#define	LMPR_TOOLONG		IrDAR_TOOLONG			
#define	LMPR_BUFFULL		IrDAR_BUFFULL			
#define	LMPR_TIMEOUT		IrDAR_TIMEOUT			
#define	LMPR_MEDIABUSY		IrDAR_MEDIABUSY			
#define	LMPR_INVALID		IrDAR_INVALID			
#define	LMPR_ACCEPT			IrDAR_ACCEPT			
#define	LMPR_DISCOVERY		IrDAR_DISCOVERY			
#define	LMPR_CONNECTING		IrDAR_CONNECTING		
#define	LMPR_ENDDISCOVERY	IrDAR_ENDDISCOVERY		
#define	LMPR_DONE			IrDAR_DONE				


#define	IrLMP_SH_PnPCompatible	0					
#define	IrLMP_SH_PDA			1					
#define	IrLMP_SH_Computer		2					
#define	IrLMP_SH_Printer		3					
#define	IrLMP_SH_Modem			4					
#define	IrLMP_SH_Fax			5					
#define	IrLMP_SH_LAN			6					
#define	IrLMP_SH_Telephony		(8+0)				
#define	IrLMP_SH_FileServer		(8+1)				
#define	IrLMP_SH_IrCOMM			(8+2)				
#define	IrLMP_SH_IrOBEX			(8+5)				


#define	IrLMP_CS_ASCII		0
#define	IrLMP_CS_ISO8859_1	1
#define	IrLMP_CS_ISO8859_2	2
#define	IrLMP_CS_ISO8859_3	3
#define	IrLMP_CS_ISO8859_4	4
#define	IrLMP_CS_ISO8859_5	5
#define	IrLMP_CS_ISO8859_6	6
#define	IrLMP_CS_ISO8859_7	7
#define	IrLMP_CS_ISO8859_8	8
#define	IrLMP_CS_ISO8859_9	9
#define	IrLMP_CS_UNICODE	255


typedef	struct	_LMCONNDAT	{
	byte	IrDAFAR *Data ;							
	int				DataSize ;						
}	LMCONNDAT ;


typedef	struct	_LMCALLBACK	{
	int		(IrDACB *ConnectInd)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
	void	(IrDACB *ConnectCnf)( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
	void	(IrDACB *DataInd)( int lsap, const void IrDAFAR *buf, uint size ) ;
	void	(IrDACB	*DataCnf)( int lsap ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int reason, const void IrDAFAR *data, int datasz ) ;
	void	(IrDACB *UDataInd)( int lsap, void IrDAFAR *buf, uint size ) ;
	void	(IrDACB *UDataCnf)( int lsap ) ;
}	LMCALLBACK ;


#define	IrLMP_CONNECTEN		IrDA_CONNECTEN
#define	IrLMP_CONNECTDI		IrDA_CONNECTDI


#define	IrLMP_DR_UserRequest						0x01
#define	IrLMP_DR_UnexpectedIrLAPDisconnect			0x02
#define	IrLMP_DR_FailedToEstablishIrLAPConnection	0x03
#define	IrLMP_DR_IrLAPReset							0x04
#define	IrLMP_DR_LMInitiatedDisconnect				0x05
#define	IrLMP_DR_LSAPConnection						0x06
#define	IrLMP_DR_NonResponsiveLMClient				0x07
#define	IrLMP_DR_NoAvailableLMClient				0x08
#define	IrLMP_DR_ConnectionHalfOpen					0x09
#define	IrLMP_DR_IllegalSourceAddress				0x0A
#define	IrLMP_DR_UnSpecifiedDisconnectReason		0xFF


#if defined(__cplusplus)
extern	"C"	{
#endif	


int		IrDAAPI	IrLMP_Open( void ) ;
void	IrDAAPI	IrLMP_Close( void ) ;
													
int		IrDAAPI	IrLMP_DiscoveryReq( uint maxslot, DISCOVERYLOG IrDAFAR *log, uint logsz, void (IrDACB *func)( int status, uint logsz )) ;


													
int		IrDAAPI	IrLMP_SetShortDeviceName( uint charset, const byte IrDAFAR *name, uint namesz ) ;
													
int		IrDAAPI	IrLMP_SetServiceHint( uint servicehint ) ;
													
int		IrDAAPI	IrLMP_ResetServiceHint( uint servicehint ) ;
													
IrDABOOL	IrDAAPI	IrLMP_CheckServiceHint( void IrDAFAR *ptr, uint len, uint servicehint ) ;
uint	IrDAAPI	IrLMP_GetDiscoveryName( void IrDAFAR *ptr, uint len ) ;


#if !defined(IrLAPNORXCHG)
int		IrDAAPI	IrLMP_SetPrimaryInd( int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_PrimaryReq( void (IrDACB *cnfp)( int status )) ;
#endif	
#if !defined(IrLAPNOSNIFF)
int		IrDAAPI	IrLMP_SetSniffInd( DISCOVERYLOG IrDAFAR *logp, int (IrDACB *indp)( void )) ;
int		IrDAAPI	IrLMP_SniffReq( void ) ;
int		IrDAAPI	IrLMP_SniffAbt( void ) ;
#endif	


int		IrDAAPI	IrLMP_RequestLSAP( void ) ;			
													
int		IrDAAPI	IrLMP_BindLSAP( int lsap, LMCALLBACK IrDAFAR *lmc ) ;
void	IrDAAPI	IrLMP_UnbindLSAP( int lsap ) ;		
int		IrDAAPI	IrLMP_GetRealLSAP( int lsap ) ;
													
int		IrDAAPI	IrLMP_ConnectReq( dword addr, int ssel, int dsel, void IrDAFAR *data, int datasz ) ;
													
int		IrDAAPI	IrLMP_DisconnectReq( int lsap, int reason, void IrDAFAR *data, int datasz ) ;
int		IrDAAPI	IrLMP_DataReq( int lsap, const void IrDAFAR *bufp, int bufsz ) ;
uint	IrDAAPI	IrLMP_GetMaxTxSize( int lsap ) ;	
uint	IrDAAPI	IrLMP_GetMaxRxSize( int lsap ) ;	
IrDABOOL	IrDAAPI	IrLMP_QueryTxQueue( void ) ;		
int		IrDAAPI	IrLMP_QueryVirginTxQueue( void ) ;	


int		IrDAAPI	IrLMP_QueryConnectedLsap( void ) ;
int		IrDAAPI	IrLMP_DisconnectLink( void ) ;
int		IrDAAPI	IrLMP_DisableIdleTimer( void ) ;
int		IrDAAPI	IrLMP_EnableIdleTimer( void ) ;


#if !defined(IrLMPNOUI)
int		IrDAAPI	IrLMP_CLessDataReq( const void IrDAFAR *bufp, int len ) ;
int		IrDAAPI	IrLMP_SetCLessDataCnf( void (IrDACB* func)( void )) ;
int		IrDAAPI	IrLMP_SetCLessDataInd( void (IrDACB* func)( const void IrDAFAR* data, int datasz )) ;
int		IrDAAPI	IrLMP_QueryMaxCLessTxDataSize( void ) ;
int		IrDAAPI	IrLMP_QueryMaxCLessRxDataSize( void ) ;
#endif	


typedef	struct	_IASVALUEBYCLASS	{
	uint			ObjectIdentifier ;	
	byte	IrDAFAR	*EncodedValue ;		
}	IASVALUEBYCLASS ;


typedef	struct	_IASCALLBACK	{
	void	(IrDACB *ConnectCnf)( int lsap, int status ) ;
	void	(IrDACB *ValueCnf)( int lsap, byte IrDAFAR *value, int valuesz ) ;
	void	(IrDACB *DisconnectInd)( int lsap ) ;
}	IASCALLBACK ;


int		IrDAAPI	IrLMP_AddClass( const void IrDAFAR *classnm, uint classsz ) ;
int		IrDAAPI	IrLMP_DeleteClass( int classid ) ;
int		IrDAAPI	IrLMP_AddAttribute( int classid, const void IrDAFAR *attrnm, uint attrsz ) ;
int		IrDAAPI	IrLMP_DeleteAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_SetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetIntAttribute( int attrid, int val ) ;
int		IrDAAPI	IrLMP_SetOctetAttribute( int attrid, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetUserStringAttribute( int attrid, uint charset, const void IrDAFAR *value, uint valuesz ) ;
int		IrDAAPI	IrLMP_SetLSAPAttribute( int attrid, int lsap ) ;
#if !defined(IASNOHIDDENCONTROL)
int		IrDAAPI	IrLMP_ShowAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_HiddenAttribute( int attrid ) ;
int		IrDAAPI	IrLMP_ShowClass( int classid ) ;
int		IrDAAPI	IrLMP_HiddenClass( int classid ) ;
#endif	


int		IrDAAPI	IrLMP_OpenIAS( dword addr, IASCALLBACK IrDAFAR *icb ) ;
int		IrDAAPI	IrLMP_CloseIAS( int lsap ) ;
int		IrDAAPI	IrLMP_GetValueByClass( int lsap, const void IrDAFAR *classnm, uint classsz, const void IrDAFAR *attr, uint attrsz ) ;
int		IrDAAPI	IrLMP_QueryReturnCode( byte IrDAFAR *value, int valuesz ) ;
int		IrDAAPI	IrLMP_QueryValueByClass( byte IrDAFAR *value, int valuesz, IASVALUEBYCLASS IrDAFAR *listp, uint listsz ) ;
int		IrDAAPI	IrLMP_GetIntValue( byte IrDAFAR *value, dword IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetOctetValue( byte IrDAFAR *value, word IrDAFAR *valsz, byte IrDAFAR * IrDAFAR *valp ) ;
int		IrDAAPI	IrLMP_GetUserStringValue( byte IrDAFAR *value, int IrDAFAR *charset, byte IrDAFAR * IrDAFAR *valp ) ;


void	IrDAAPI	IrLMP_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrLMP_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	


#endif	