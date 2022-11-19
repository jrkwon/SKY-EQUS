


#if !defined(__ircomm_h__)
#define	__ircomm_h__


#include	"irda.h"
#include	"tinytp.h"


#define	COMMR_BUSYWAIT		TTPR_BUSYWAIT			
#define	COMMR_NORESOURCE	TTPR_NORESOURCE			
#define	COMMR_TOOLONG		TTPR_TOOLONG			
#define	COMMR_BUFFULL		TTPR_BUFFULL			
#define	COMMR_TIMEOUT		TTPR_TIMEOUT			
#define	COMMR_MEDIABUSY		TTPR_MEDIABUSY			
#define	COMMR_INVALID		TTPR_INVALID			
#define	COMMR_ACCEPT		TTPR_ACCEPT				
#define	COMMR_DISCOVERY		TTPR_DISCOVERY			
#define	COMMR_CONNECTING	TTPR_CONNECTING			
#define	COMMR_ENDDISCOVERY	TTPR_ENDDISCOVERY		
#define	COMMR_DONE			TTPR_DONE				


#define	IrCOMM_PI_SERVICETYPE		0x00	
#define	IrCOMM_PI_PORTTYPE			0x01	
#define	IrCOMM_PI_FLEXEDPORTNAME	0x02	


#define	IrCOMM_PI_DATARATE			0x10	
#define	IrCOMM_PI_DATAFORMAT		0x11	
#define	IrCOMM_PI_FLOWCONTROL		0x12	
#define	IrCOMM_PI_XONFLOWCHAR		0x13	
#define	IrCOMM_PI_ENQFLOWCHAR		0x14	
#define	IrCOMM_PI_LINESTATUS		0x15	
#define	IrCOMM_PI_BREAK				0x16	


#define	IrCOMM_PI_DTELINE			0x20	
#define	IrCOMM_PI_DCELINE			0x21	
#define	IrCOMM_PI_POLLLINESETTINGS	0x22	


#define	IrCOMM_PI_STATUSQUERY		0x30	
#define	IrCOMM_PI_SETBUSYTIMEOUT	0x31	
#define	IrCOMM_PI_1284MODESUPORT	0x32	
#define	IrCOMM_PI_1284DEVID			0x33	
#define	IrCOMM_PI_SELECT1284MODE	0x34	
#define	IrCOMM_PI_1284TRANSFER		0x35	
#define	IrCOMM_PI_STATUSQUERYRSP	0x38	
#define	IrCOMM_PI_SETBUSYTIMEOUTRSP	0x39	
#define	IrCOMM_PI_1284MODESUPORTRSP	0x3A	
#define	IrCOMM_PI_1284DEVIDRSP		0x3B	
#define	IrCOMM_PI_SELECT1284MODERSP	0x3C	
#define	IrCOMM_PI_1284TRANSFERRSP	0x3D	


#define	IrCOMM_ST_3WireRaw			0x01	
#define	IrCOMM_ST_3Wire				0x02	
#define	IrCOMM_ST_9Wire				0x04	
#define	IrCOMM_ST_Centro			0x08	
#define	IrCOMM_ST_DEFAULT			0x00


#define	IrCOMM_PT_SERIAL			0x01	
#define	IrCOMM_PT_PARALLEL			0x02	


#define	IrCOMM_DF_CLMSK				0x03	
#define	IrCOMM_DF_CL5				0x00	
#define	IrCOMM_DF_CL6				0x01	
#define	IrCOMM_DF_CL7				0x02	
#define	IrCOMM_DF_CL8				0x03	
#define	IrCOMM_DF_SB1				0x00	
#define	IrCOMM_DF_SB2				0x04	
#define	IrCOMM_DF_NP				0x00	
#define	IrCOMM_DF_PE				0x08	
#define	IrCOMM_DF_PTMSK				0x30	
#define	IrCOMM_DF_PTODD				0x00	
#define	IrCOMM_DF_PTEVEN			0x10	
#define	IrCOMM_DF_PTMARK			0x20	
#define	IrCOMM_DF_PTSPC				0x30	


#define	IrCOMM_FC_XIN				0x01	
#define	IrCOMM_FC_XOUT				0x02	
#define	IrCOMM_FC_RTSIN				0x04	
#define	IrCOMM_FC_RTSOUT			0x08	
#define	IrCOMM_FC_DSRIN				0x10	
#define	IrCOMM_FC_DSROUT			0x20	
#define	IrCOMM_FC_ENQIN				0x40	
#define	IrCOMM_FC_ENQOUT			0x80	


#define	IrCOMM_LS_OE				0x02	
#define	IrCOMM_LS_PE				0x04	
#define	IrCOMM_LS_FE				0x08	


#define	IrCOMM_B_MSK				0x01	
#define	IrCOMM_B_CLEAR				0x00	
#define	IrCOMM_B_SET				0x01	


#define	IRCOMM_DL_DDTR				0x01	
#define	IRCOMM_DL_DRTS				0x02	
#define	IRCOMM_DL_DTR				0x04	
#define	IRCOMM_DL_RTS				0x08	


#define	IRCOMM_DL_DCTS				0x01	
#define	IRCOMM_DL_DDSR				0x02	
#define	IRCOMM_DL_DRI				0x04	
#define	IRCOMM_DL_DCD				0x08	
#define	IRCOMM_DL_CTS				0x10	
#define	IRCOMM_DL_DSR				0x20	
#define	IRCOMM_DL_RI				0x40	
#define	IRCOMM_DL_CD				0x80	


#define		IrCOMM_ST_TypeMask		0x7F
#define		IrCOMM_ST_DTE			0x80


#define		IrCOMM_ST_DEFAULT		0x00
#define		IrCOMM_ST_3WireRawDCE	IrCOMM_ST_3WireRaw
#define		IrCOMM_ST_3WireRawDTE	(IrCOMM_ST_DTE|IrCOMM_ST_3WireRaw)
#define		IrCOMM_ST_3WireDCE		IrCOMM_ST_3Wire
#define		IrCOMM_ST_3WireDTE		(IrCOMM_ST_DTE|IrCOMM_ST_3Wire)
#define		IrCOMM_ST_9WireDCE		IrCOMM_ST_9Wire
#define		IrCOMM_ST_9WireDTE		(IrCOMM_ST_DTE|IrCOMM_ST_9Wire)
#define		IrCOMM_ST_CentroDCE		IrCOMM_ST_Centro
#define		IrCOMM_ST_CentroDTE		(IrCOMM_ST_DTE|IrCOMM_ST_Centro)


#define	IrCOMM_CONNECTEN		IrDA_CONNECTEN
#define	IrCOMM_CONNECTDI		IrDA_CONNECTDI


typedef	struct	_COMMCALLBACK	{
	int		(IrDACB *ConnectInd)( int slsap, dword addr, int dlsap, int servicetype ) ;
	void	(IrDACB *ConnectCnf)( int slsap, dword addr, int dlsap, int status ) ;
	void	(IrDACB *DisconnectInd)( int lsap, int status ) ;
	void	(IrDACB *DataInd)( int lsap, int datasz ) ;
	void	(IrDACB *ControlInd)( int lsap, const void IrDAFAR *control, int controlsz ) ;
}	COMMCALLBACK ;


#define	COMCTL_DataRate		0x10


#if defined(__cplusplus)
extern "C" {
#endif	


int		IrDAAPI	IrCOMM_Open( void ) ;				
void	IrDAAPI	IrCOMM_Close( void ) ;				


int		IrDAAPI	IrCOMM_RequestLSAP( void ) ;		
													
int		IrDAAPI	IrCOMM_BindLSAP( int lsap, COMMCALLBACK IrDAFAR *ccb, int servicetype ) ;
void	IrDAAPI	IrCOMM_UnbindLSAP( int lsap ) ;		
int		IrDAAPI	IrCOMM_GetRealLSAP( int lsap ) ;
													
int		IrDAAPI	IrCOMM_ConnectReq( int slsap, dword addr, int dlsap ) ;
int		IrDAAPI	IrCOMM_DisconnectReq( int lsap ) ;	
													
int		IrDAAPI	IrCOMM_SendData( int lsap, const void IrDAFAR *buf, uint bufsz ) ;
													
int		IrDAAPI	IrCOMM_RecvData( int lsap, void IrDAFAR *buf, int bufsz ) ;
													
int		IrDAAPI	IrCOMM_ControlReq( int lsap, const void IrDAFAR *control, int controlsz ) ;
int		IrDAAPI	IrCOMM_QueryConnectUserData( int lsap, byte IrDAFAR * IrDAFAR *ptr ) ;
int		IrDAAPI	IrCOMM_QueryTxQueue( int lsap ) ;	
int		IrDAAPI	IrCOMM_QueryRxQueue( int lsap ) ;	
int		IrDAAPI	IrCOMM_QueryTxData( int lsap ) ;	
int		IrDAAPI	IrCOMM_QueryRxData( int lsap ) ;	


void	IrDAAPI	IrCOMM_SetConnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrCOMM_SetDisconnectPrv( void (IrDACB *func)( void )) ;
void	IrDAAPI	IrCOMM_SetDataSizePtr( ulong IrDAFAR *txszp, ulong IrDAFAR *rxszp ) ;


#if defined(__cplusplus)
}
#endif	


#endif	