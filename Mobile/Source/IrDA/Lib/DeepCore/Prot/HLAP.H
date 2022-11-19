


#if !defined(__hlap_h__)
#define		__hlap_h__


#if !defined(GENOA_T8107)
#define	IrLAP_CMDBUFSZ	(64+2)
#else	
#define	IrLAP_CMDBUFSZ	(2048+2)
#endif	
#if !defined(IrLAPRECVTBLSIZE)
#define	IrLAPRECVTBLSIZE	7						
#endif	
#if !defined(IrLAPMAXTXWINSIZE)
#define	IrLAPMAXTXWINSIZE	7						
#endif	


#define	IrLAP_PF		0x10						
#define	IrLAP_BroadCast	0x7F
#define	IrLAP_CA		0xFE
#define	IrLAP_CR		0x01						


#define		IrLAP_SNRMCmd	0x83
#define		IrLAP_DISCCmd	0x43
#define		IrLAP_UICmd		0x03
#define		IrLAP_XIDCmd	0x2F
#define		IrLAP_TESTCmd	0xE3


#define		IrLAP_RNRMRsp	0x83
#define		IrLAP_UARsp		0x63					
#define		IrLAP_FRMRRsp	0x87
#define		IrLAP_DMRsp		0x0F					
#define		IrLAP_RDRsp		0x43
#define		IrLAP_UIRsp		0x03
#define		IrLAP_XIDRsp	0xAF
#define		IrLAP_TESTRsp	0xE3
#define		IrLAP_XCHGCmd	0x4F	
#define		IrLAP_DXCHGCmd	0xCF	
#define		IrLAP_RXCHGRsp	0xCF	


#define		IrLAP_RRCmd		0x01
#define		IrLAP_RNRCmd	0x05
#define		IrLAP_REJCmd	0x09
#define		IrLAP_SREJCmd	0x0D
#define		IrLAP_RRRsp		0x01
#define		IrLAP_RNRRsp	0x05
#define		IrLAP_REJRsp	0x09
#define		IrLAP_SREJRsp	0x0D


#define		XID_FI					2
#define		XID_SrcAddress			3
#define		XID_DesAddress			7
#define		XID_DiscoveryFlag		11
#define		XID_SlotNo				12
#define		XID_Version				13
#define		XID_Info				14


#define		XID_AddressConfilict	0x04


#define		UA_SrcAddress			2
#define		UA_DesAddress			6
#define		UA_NegotiationParm		10


#define		TEST_SrcAddress			2
#define		TEST_DesAddress			6
#define		TEST_Information		10


#define		SNRM_RetryCount			10


#define		SNRM_SrcAddress			2
#define		SNRM_DesAddress			6
#define		SNRM_ConnectAddress		10
#define		SNRM_NegotiationParm	11


#define	FRMR_RejectFrameCtrlField	2
#define	FRMR_Field1					3
#define	FRMR_Field2					4


#define	FRMR_FuterNotAvilable		0x01
#define	FRMR_IgnIField				0x02
#define	FRMR_OvrNRCount				0x04
#define	FRMR_InvalidNr				0x08
#define	FRMR_InvalidNs				0x0F


typedef	struct	_LAPSENDCTL	{
	uint			vs ;		
	byte IrDAFAR	*ptr ;		
	uint			size ;		
	IrDABOOL			xmit ;		
}	LAPSENDCTL ;


#if !defined(MAKE_DOCUMENT)
extern	byte	IrDAFAR	LAP_SendCmdBuffer[IrLAP_CMDBUFSZ] ;		
extern	volatile IrDAFAR uint	RecvSizeTbl[IrLAPRECVTBLSIZE] ;	
extern	volatile IrDAFAR byte IrDAFAR	* IrDAFAR RecvAddrTbl[IrLAPRECVTBLSIZE] ;
																
extern	volatile IrDAFAR uint	RecvFrameSize ;					
#else	
extern	byte				LAP_SendCmdBuffer[IrLAP_CMDBUFSZ] ;	
extern	volatile	uint	RecvSizeTbl[IrLAPRECVTBLSIZE] ;		
extern	volatile	byte	*RecvAddrTbl[IrLAPRECVTBLSIZE] ;	
extern	volatile	uint	RecvFrameSize ;						
#endif	


#define	IrLAP_S_NDM			0
#define	IrLAP_S_REPLY		1
#define	IrLAP_S_SRecv		2
#define	IrLAP_S_SError		3
#define	IrLAP_S_SReset		4
#undef	IrLAP_S_SBusy
#define	IrLAP_S_SBusyWait	5
#define	IrLAP_S_SClose		6
#define	IrLAP_S_SRXWait		7
#define	IrLAP_S_SXWait		8
#define	IrLAP_S_POUT		9
#define	IrLAP_S_SNIFF		10
#undef	IrLAP_S_SLEEP								
#define	IrLAP_S_SETUP		11
#define	IrLAP_S_QUERY		12
#define	IrLAP_S_PRecv		13
#undef	IrLAP_S_PResetWait
#define	IrLAP_S_PReset		14
#undef	IrLAP_S_PBusy
#define	IrLAP_S_PBusyWait	15
#define	IrLAP_S_PClose		16
#define	IrLAP_S_PXWait		17
#define	IrLAP_S_SCONN		18
#define	IrLAP_S_SSETUP		19
#define	IrLAP_S_MAX			20
#undef	IrLAP_S_CONN								
#undef	IrLAP_S_SXMIT								
#undef	IrLAP_S_SResetCheck							


#define	IrLAP_IsConnected1	( IrLAP_S_PRecv <= IrLAP_Stage && IrLAP_Stage <= IrLAP_S_PClose )
#define	IrLAP_IsConnected2	( IrLAP_S_SRecv <= IrLAP_Stage && IrLAP_Stage <= IrLAP_S_SClose )
#define	IrLAP_IsConnected	( IrLAP_IsConnected1 || IrLAP_IsConnected2 )
#define	IrLAP_IsSniffing	( IrLAP_S_POUT <= IrLAP_Stage && IrLAP_Stage <= IrLAP_S_SNIFF )


#define	IrLAP_RE_Unknown		0	
#define	IrLAP_RE_DISCCmd		1
#define	IrLAP_RE_DMRsp			2
#define	IrLAP_RE_FRMRRsp		3
#define	IrLAP_RE_RDRsp			4
#define	IrLAP_RE_RNRMRsp		5
#define	IrLAP_RE_SNRMCmd		6
#define	IrLAP_RE_TESTCmd		7
#define	IrLAP_RE_TESTRsp		8
#define	IrLAP_RE_UARsp			9
#define	IrLAP_RE_UICmd			10
#define	IrLAP_RE_UIRsp			11
#define	IrLAP_RE_XIDCmd			12
#define	IrLAP_RE_XIDRsp			13
#define	IrLAP_RE_RRCmd			14
#define	IrLAP_RE_RRRsp			15
#define	IrLAP_RE_RNRCmd			16
#define	IrLAP_RE_RNRRsp			17
#define	IrLAP_RE_REJCmd			18
#define	IrLAP_RE_REJRsp			19
#define	IrLAP_RE_SREJCmd		20
#define	IrLAP_RE_SREJRsp		21
#define	IrLAP_RE_IFrame			22	
#define	IrLAP_RE_RXCHGRsp		23	
#define	IrLAP_RE_XCHGCmd		24	
#define	IrLAP_RE_DXCHGCmd		25	
#define	IrLAP_RE_MAX			(IrLAP_RE_DXCHGCmd+1)


typedef	struct	_LAPSNRMPRV	{
	void		(IrDACB *funcp)( void ) ;
	PORTCONFIG2 IrDAFAR	*pcp ;				
}	LAPSNRMPRV ;


typedef	struct	_LAPFLAGS	{
	IrDABOOL	noAddress ;				
	IrDABOOL	setNewAddr ;			
	IrDABOOL	localBusyDetected ;		
	IrDABOOL	localBusyCleared ;		
	IrDABOOL	inDataReq ;				
#if !defined(IrLAPNOUI)
	IrDABOOL	inUDataReq ;			
	IrDABOOL	sendingUi ;				
#endif	
	IrDABOOL	sendingUARsp ;			
	IrDABOOL	refuseDiscovery ;		
	IrDABOOL	disconnectReq ;			
	IrDABOOL	primaryReq ;			
	IrDABOOL	rstRxBuffer ;			
	IrDABOOL	rsvDataCnf ;			
#if !defined(IrLAPNOSNIFF)
	IrDABOOL	sniffing ;				
#endif	
}	LAPFLAGS ;


#endif	
