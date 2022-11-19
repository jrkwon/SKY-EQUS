


#if !defined(__irda_h__)
#define	__irda_h__


#define	IrDAFORTRANXIT01


#if defined(IrDAFORGENOASIRTEST)
#undef	GENOA_A1120
#define	GENOA_A1120
#undef	GENOA_T8107
#define	GENOA_T8107
#endif	


#define	IrDAVER000	200


#if defined(IrDASWAP)
#  if !defined(IrDAAPI)
#    define	IrDAAPI		__far __cdecl
#  endif	
#  if !defined(IrDAFAR)
#    define	IrDAFAR		__far
#  endif	
#  if !defined(IrDACB)
#    define	IrDACB		__far __cdecl
#  endif	
#endif	


#if 0
#if defined(__cplusplus)
extern	"C"	{
#endif	
extern	void	IrDACB	IrDA_SwapIn( void ) ;
extern	void	IrDACB	IrDA_SwapOut( void ) ;
#if defined(__cplusplus)
}
#endif	
#endif


#if !defined(MAKE_DOCUMENT)
#if !defined(IrDAAPI)
#define		IrDAAPI
#endif	
#if !defined(IrDAFAR)
#define		IrDAFAR
#endif	
#if !defined(IrDACB)
#define		IrDACB
#endif	
#endif	
#if !defined(IrDAMISCAPI)
#define		IrDAMISCAPI
#endif	


#if !defined(__byte_defined)
#define	__byte_defined
typedef	unsigned char		byte ;
#endif	
#if !defined(__irdabool_defined__)
#define	__irdabool_defined__
typedef	unsigned int		IrDABOOL ;
#endif	
#if !defined(__word_defined__)
#define	__word_defined__
typedef	unsigned short		word ;					
#endif	
#if !defined(__dword_defined__)
#define	__dword_defined__
typedef	unsigned long		dword ;
#endif	
#if !defined(__uint_defined__)
#define	__uint_defined__
typedef	unsigned int		uint ;
#endif	
#if !defined(__ulong_defined__)
#define	__ulong_defined__
typedef	unsigned long		ulong ;
#endif	


typedef	void (IrDACB *IrDACBP)( void ) ;


#define	IrDAR_DONE			1					
#define	IrDAR_ACCEPT		0					
#define	IrDAR_NOCONNECT		-1					
#define	IrDAR_NORESPONSE	-2					
#define	IrDAR_DISCONNECT	-3					
#define	IrDAR_CONNECTING	-4					
#define	IrDAR_MEDIABUSY		-5					
#define	IrDAR_EPARM			-6					
#define	IrDAR_INVALIDVER	-7
#define	IrDAR_NOPROTOCOL	-8
#define	IrDAR_INUSE			-9					
#define	IrDAR_PRIMARY		-10					
#define	IrDAR_DENY			-11					


#if 1
#define	IrDAR_BUSYWAIT		-17					
#define	IrDAR_NORESOURCE	-16					
#define	IrDAR_TOOLONG		-15					
#define	IrDAR_BUFFULL		-14					
#define	IrDAR_TIMEOUT		-13					
#define	IrDAR_INVALID		-12					
#define	IrDAR_DISCOVERY		11					
#endif


#define	MAKER_SII		0x00
#define	MAC_BP			0x00
#define	OEMID_SIIBP		((MAKER_SII<<8)+MAC_BP)
#define	OEMID_MITS10	0x0200
#define	OEMID_PUBLIC	0xFFFF


#define	NO					0
#define	YES					1
#define	FALSE				0
#define	TRUE				1
#define	OFF					0
#define	ON					1
#define	IrLAP_NNMSZ			32
#define	CLESSLSAP			0x70


typedef	struct	_PORTCONFIG	{
	byte			LineCapability ;				
	byte			MaxTanaroundTime ;				
	byte			FrameSize ;						
	byte			WindowSize ;					
	byte			NoofBOF ;						
	byte			MinTurnaroundTime ;				
	byte			DisconnectTime ;				
}	PORTCONFIG ;


typedef	struct	_PORTCONFIG2 {
	word			LineCapability ;				
	word			MaxTanaroundTime ;				
	word			FrameSize ;						
	word			WindowSize ;					
	word			NoofBOF ;						
	word			MinTurnaroundTime ;				
	word			DisconnectTime ;				
}	PORTCONFIG2 ;


#define		IrLAP_BPS_2400			0x01
#define		IrLAP_BPS_9600			0x02
#define		IrLAP_BPS_19200			0x04
#define		IrLAP_BPS_38400			0x08
#define		IrLAP_BPS_57600			0x10
#define		IrLAP_BPS_115200		0x20
#define		IrLAP_BPS_576000		0x40
#define		IrLAP_BPS_1152000		0x80
#define		IrLAP_BPS_4000000		0x0100
#define		IrLAP_MaxTurn_500ms		0x01
#define		IrLAP_MaxTurn_250ms		0x02
#define		IrLAP_MaxTurn_100ms		0x04
#define		IrLAP_MaxTurn_50ms		0x08
#define		IrLAP_MaxTurn_25ms		0x10
#define		IrLAP_MaxTurn_10ms		0x20
#define		IrLAP_MaxTurn_5ms		0x40
#define		IrLAP_FS_64				0x01
#define		IrLAP_FS_128			0x02
#define		IrLAP_FS_256			0x04
#define		IrLAP_FS_512			0x08
#define		IrLAP_FS_1024			0x10
#define		IrLAP_FS_2048			0x20
#define		IrLAP_WS_1				0x01
#define		IrLAP_WS_2				0x02
#define		IrLAP_WS_3				0x04
#define		IrLAP_WS_4				0x08
#define		IrLAP_WS_5				0x10
#define		IrLAP_WS_6				0x20
#define		IrLAP_WS_7				0x40
#define		IrLAP_BOF_48			0x01
#define		IrLAP_BOF_24			0x02
#define		IrLAP_BOF_12			0x04
#define		IrLAP_BOF_5				0x08
#define		IrLAP_BOF_3				0x10
#define		IrLAP_BOF_2				0x20
#define		IrLAP_BOF_1				0x40
#define		IrLAP_BOF_0				0x80
#define		IrLAP_MinTurn_10ms		0x01
#define		IrLAP_MinTurn_5ms		0x02
#define		IrLAP_MinTurn_1ms		0x04
#define		IrLAP_MinTurn_05ms		0x08
#define		IrLAP_MinTurn_01ms		0x10
#define		IrLAP_MinTurn_005ms		0x20
#define		IrLAP_MinTurn_001ms		0x40
#define		IrLAP_MinTurn_0ms		0x80
#define		IrLAP_SL_3s				0x01
#define		IrLAP_SL_8s				0x02
#define		IrLAP_SL_12s			0x04
#define		IrLAP_SL_16s			0x08
#define		IrLAP_SL_20s			0x10
#define		IrLAP_SL_25s			0x20
#define		IrLAP_SL_30s			0x40
#define		IrLAP_SL_40s			0x80


typedef	struct	_DISCOVERYLOG {
	dword		Address ;							
	uint		DeviceNameSize ;					
	byte		DeviceName[IrLAP_NNMSZ] ;			
}	DISCOVERYLOG ;


#define		IrDA_CONNECTDI		-1
#define		IrDA_CONNECTEN		0
#define		IrDA_RESETDI		-1
#define		IrDA_RESETEN		0
#define		IrDA_DENY			-1
#define		IrDA_ACCEPT			0


#if !defined(IrDA_memcpy)
#define	IrDA_memcpy				memcpy
#endif	
#if !defined(IrDA_memcmp)
#define	IrDA_memcmp				memcmp
#endif	
#if !defined(IrDA_memset)
#define	IrDA_memset				memset
#endif	
#if !defined(IrDA_LDiv)
#define	IrDA_LDiv( src, des )	((src)/(des))
#endif	
#if !defined(IrDA_LMul)
#define	IrDA_LMul( src, des )	((src)*(des))
#endif	
#if !defined(IrDA_LSift)
#define	IrDA_LSift( src, cnt )	((src)<<(cnt))
#endif	


#if !defined(UNREFERENCE)
# define	UNREFERENCE(i)	i=i
#endif	


#if defined(__cplusplus)
extern	"C"	{
#endif	


void		IrDAAPI	IrDA_SetRandomSeed( dword seed ) ;
uint		IrDAAPI	IrDA_GetOEMID( void ) ;
													
void		IrDAAPI	IrDA_SetTxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													
void		IrDAAPI	IrDA_SetRxFrame( byte IrDAFAR *buf, uint framesz, uint winsz ) ;
													
void		IrDAAPI	IrDA_GetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													
void		IrDAAPI	IrDA_SetPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													
int			IrDAAPI	IrDA_GetConnPortConfig( PORTCONFIG IrDAFAR *configp ) ;
													
void		IrDAAPI	IrDA_GetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													
void		IrDAAPI	IrDA_SetPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
													
int			IrDAAPI	IrDA_GetConnPortConfig2( PORTCONFIG2 IrDAFAR *configp ) ;
IrDABOOL	IrDAAPI	IrDA_GetPowerMode( void ) ;
void		IrDAAPI	IrDA_PowerOn( void ) ;
void		IrDAAPI	IrDA_PowerOff( void ) ;
void		IrDAAPI	IrDA_SetPowerLevel( uint level ) ;
uint		IrDAAPI	IrDA_GetPowerLevel( void ) ;


#if defined(__cplusplus)
}
#endif	


#if	defined(DMSG)
#define	DL_TYPE		0xFF00
#define	DL_MODE		0x00FF
#define	DL_LAP		0x0100
#define	DL_LMP		0x0200
#define	DL_TTP		0x0400
#define	DL_COMM		0x0800
#define	DL_MASS		0x1000
#define	DL_MISC		0x2000
#define	DL_SIR		0x3000
#define	DL_USER		0x4000
#define	DL_DANGER	0x0001
#define	DL_WARN		0x0002
#define	DL_INFO		0x0004
#define	DL_DAT		0x0008


#define	DL_SIRDANGER	(DL_SIR|DL_DANGER)
#define	DL_SIRDAT		(DL_SIR|DL_DAT)
#define	DL_SIRWARN		(DL_SIR|DL_WARN)
#define	DL_SIRINFO		(DL_SIR|DL_INFO)
#define	DL_LAPDANGER	(DL_LAP|DL_DANGER)
#define	DL_LAPDAT		(DL_LAP|DL_DAT)
#define	DL_LAPWARN		(DL_LAP|DL_WARN)
#define	DL_LAPINFO		(DL_LAP|DL_INFO)
#define	DL_LMPDANGER	(DL_LMP|DL_DANGER)
#define	DL_LMPDAT		(DL_LMP|DL_DAT)
#define	DL_LMPWARN		(DL_LMP|DL_WARN)
#define	DL_LMPINFO		(DL_LMP|DL_INFO)
#define	DL_TTPDANGER	(DL_TTP|DL_DANGER)
#define	DL_TTPDAT		(DL_TTP|DL_DAT)
#define	DL_TTPWARN		(DL_TTP|DL_WARN)
#define	DL_TTPINFO		(DL_TTP|DL_INFO)
#define	DL_COMMDANGER	(DL_COMM|DL_DANGER)
#define	DL_COMMDAT		(DL_COMM|DL_DAT)
#define	DL_COMMWARN		(DL_COMM|DL_WARN)
#define	DL_COMMINFO		(DL_COMM|DL_INFO)
#define	DL_MASSDANGER	(DL_MASS|DL_DANGER)
#define	DL_MASSDAT		(DL_MASS|DL_DAT)
#define	DL_MASSWARN		(DL_MASS|DL_WARN)
#define	DL_MASSINFO		(DL_MASS|DL_INFO)
#define	DL_MISCDANGER	(DL_MISC|DL_DANGER)
#define	DL_MISCDAT		(DL_MISC|DL_DAT)
#define	DL_MISCWARN		(DL_MISC|DL_WARN)
#define	DL_MISCINFO		(DL_MISC|DL_INFO)
#define	DL_USERDANGER	(DL_USER|DL_DANGER)
#define	DL_USERDAT		(DL_USER|DL_DAT)
#define	DL_USERWARN		(DL_USER|DL_WARN)
#define	DL_USERINFO		(DL_USER|DL_INFO)


#if defined(__cplusplus)
extern "C" {
#endif	


int		IrDAAPI	IrDBG_Open( void ) ;
void	IrDAAPI	IrDBG_Close( void ) ;
void	IrDAAPI	_DPUTS( int level, const char IrDAFAR *msg ) ;
void	IrDAAPI	_DPUTC( int level, char data ) ;
void	IrDAAPI	_DPUTB( int level, const char IrDAFAR *msg, byte data ) ;
void	IrDAAPI	_DPUTW( int level, const char IrDAFAR *msg, word data ) ;
void	IrDAAPI	_DPUTD( int level, const char IrDAFAR *msg, dword data ) ;
void	IrDAAPI	_DDUMP( int level, const void IrDAFAR *des, word len, word maxLen ) ;


#if defined(__cplusplus)
}
#endif	
#endif	


#endif	