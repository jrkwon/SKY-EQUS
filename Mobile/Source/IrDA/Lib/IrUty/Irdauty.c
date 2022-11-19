/********************************************************************

	IrAPP Apprication convinience Program

*********************************************************************/

#include	<string.h>

#include	"irdaconf.h"
#include	"ldevice.h"

#include	<irsir.h>
#include	<irlap.h>
#include	<irlmp.h>
#include	<ircomm.h>
#include	"irdauty.h"
#include	"ldevice.h"

#include	<timer.h>
#include	"console.h"

/*  -----  Definition  -----  */

extern	byte IrDAFAR	achRecvBuf[IrDARECVBUFSZ];
extern	byte IrDAFAR	achSendBuf[IrDASENDBUFSZ];


#define			MAXDISCOVERY		10
#define			DiscoverySlot		6

#if !defined(DISCOVERYLOGSZ)
#define			DISCOVERYLOGSZ		1	/* 8Discovery Log entry Size	*/
#endif	/*	DISCOVERYLOGSZ	*/


#define			IASVALUEBYCLASSSZ	2	/* 4IrIAS Class Max	*/


static	int				DstLSAP ;
static	int				classid ;
static	int				attrid ;

static	volatile int	DiscoveryFounds ;	/* Max Logdata	*/
static	volatile int	DiscoveryStat;
static	volatile int	IASValueSize ;
static	volatile int	IASValueStat ;

//static	void	PortConfig( void ) ;

static	volatile		byte	IASValue[256] ;	/*	256	*/
static	DISCOVERYLOG	DiscoveryLog[DISCOVERYLOGSZ] ;

/************************************************

	Setup Class & attribute

************************************************/
void	IrAPP_SetIASClass(char *class,char *attribute,int lsap )
{
	classid = IrLMP_AddClass(class,strlen(class) ) ;
	attrid  = IrLMP_AddAttribute( classid, attribute,strlen(attribute));
	IrLMP_SetLSAPAttribute( attrid, lsap ) ;
}

#if	0
void	IrAPP_RemoveIASClass( void )
{
	IrLMP_DeleteClass( classid );
	IrLMP_DeleteAttribute( attrid );
}

#endif

/************************************************

	Discovery Request

*************************************************/
static	void	IrDACB	CBDiscoveryCnf( int status, uint logsz )
{
	DiscoveryFounds	= logsz ;
	DiscoveryStat	= 1;
}

IrDABOOL	IrAPP_DiscoveryDevice( void )
{
int		ret ;
dword	Timer ;
	DiscoveryFounds = -1;
	DiscoveryStat	= 0;
	if((ret = IrLMP_DiscoveryReq( DiscoverySlot, DiscoveryLog, DISCOVERYLOGSZ, CBDiscoveryCnf )) != IrDAR_ACCEPT ){
		switch(ret){
			case	IrDAR_MEDIABUSY:
					break;
			case	IrDAR_BUSYWAIT:
			default:
					break;
		}
		return	TRUE;
	}
	SetTimer( &Timer, 10000L ) ;
	while(DiscoveryStat == 0){
		if( GetTimer( Timer ) == 0L ){
			return( TRUE ) ;
		}
	}
	if( DiscoveryFounds ){
			return	FALSE;
	}
	return	TRUE;
}

int	IrAPP_FoundServiceHint(int hintBit)
{
int	no;
	for(no = 0 ;no < DiscoveryFounds ; no++ ){
		if( IrLMP_CheckServiceHint( DiscoveryLog[no].DeviceName, DiscoveryLog[no].DeviceNameSize, hintBit) )
			break;
	}
	if(no == DiscoveryFounds){
		return	-1;
	}
	return	no;
}

#ifdef	USE_MONITOR

void	IrAPP_DisplayDeviceName(int no )
{
uint	log_point;
	log_point = IrLMP_GetDiscoveryName(
					DiscoveryLog[no].DeviceName,
					DiscoveryLog[no].DeviceNameSize
				);

	cprintf(	"DiscoveryName = %s\n", 
			&DiscoveryLog[no].DeviceName[log_point]
	);
}
#endif	/*USE_MONITOR*/

/********************************************************************

	IAS Control

**********************************************************************/
#define	ST_IASINIT	0
#define	ST_IASOPEN	1
#define	ST_IASCONF	2
#define	ST_IASDISC	4

static	void	IrDACB	IASConnectCnf( int lsap, int status )
{
	IASValueStat |= ST_IASOPEN;
}

static	void	IrDACB	IASValueCnf( int lsap, byte IrDAFAR *value, int valuesz )
{
	if( valuesz >= sizeof( IASValue ))
		valuesz	= sizeof( IASValue ) ;
	IrDA_memcpy((void *)IASValue, (const void *)value, valuesz ) ;
	IASValueSize = valuesz ;
	IASValueStat	|= ST_IASCONF;
}

static	void	IrDACB	IASDisconnectInd( int lsap )
{
	IASValueStat &= ~ST_IASOPEN;
	IASValueStat |= ST_IASDISC;
}

int	IrAPP_QueryLSAP(int no ,char *class,char *attr ,dword *daddr,int *dlsap)
{
IASCALLBACK 	icb ;
dword			Timer ;
int				lsap,ret ;
dword			tmpdw ;

int				listsz ;
IASVALUEBYCLASS	list[IASVALUEBYCLASSSZ] ;

	icb.ConnectCnf    = IASConnectCnf ;
	icb.ValueCnf      = IASValueCnf ;
	icb.DisconnectInd = IASDisconnectInd ;

	IASValueStat	= ST_IASINIT;
	for(;;) {
		lsap = IrLMP_OpenIAS( DiscoveryLog[no].Address, &icb ) ;
		if( lsap >= 0 )
			break ;
		if( lsap != IrDAR_BUSYWAIT ){
			return( TRUE ) ;
		}
	}
	SetTimer( &Timer, 10000L ) ;
	while((IASValueStat & ST_IASOPEN) == 0){
		if( IASValueStat & ST_IASDISC )
			return( TRUE ) ;
		if( GetTimer( Timer ) == 0L ) {
			IrLMP_CloseIAS( lsap );
			return( TRUE ) ;
		}
	}
	for(;;) {
		ret	= IrLMP_GetValueByClass( lsap, class,strlen(class),attr,strlen(attr) );
		if( ret == IrDAR_ACCEPT )
			break;
#if defined(TYPEFSA)
		if( ret == IrDAR_NORESOURCE)
			continue;
		if( ret == IrDAR_BUSYWAIT)
			continue;
#endif
		if( ret != IrDAR_BUSYWAIT ) {
			IrLMP_CloseIAS( lsap ) ;
			return( TRUE ) ;
		}
		if(( IASValueStat & ST_IASOPEN) == 0){
			return( TRUE ) ;
		}
	}

	SetTimer( &Timer, 5000L ) ;
	for(;;) {
		if( GetTimer( Timer ) == 0L ) {
			IrLMP_CloseIAS( lsap ) ;
			return( TRUE ) ;
		}
		if(( IASValueStat & ST_IASOPEN) == 0){
			return( TRUE ) ;
		}
		if(( IASValueStat & ST_IASCONF) == 0)
			continue ;
		if( IASValue[0] & 0x40 ) {
			continue ;
		}
		break ;
	}
	IrLMP_CloseIAS( lsap ) ;
	ret = IrLMP_QueryReturnCode((byte IrDAFAR *)IASValue, IASValueSize ) ;
	switch( ret ) {
		case	0 :
			break ;
		case	1 :
		case	2 :
		default :
			return( TRUE ) ;
	}
	listsz = IrLMP_QueryValueByClass((byte IrDAFAR *)IASValue, IASValueSize, list, IASVALUEBYCLASSSZ );
	if( listsz < 1 ){
		return( TRUE ) ;
	}
	if( IrLMP_GetIntValue( list[0].EncodedValue, &tmpdw ) != IrDAR_ACCEPT ){
		return( TRUE ) ;
	}
	*dlsap = DstLSAP = (int)tmpdw;
	*daddr = DiscoveryLog[no].Address;
	return( FALSE ) ;
}

extern char	ui_bIrDAExchangeMode;

void	IrAPP_SetPortConfig( void )
{
	PORTCONFIG2	pc ;

	IrDA_GetPortConfig2( &pc ) ;

// chul.edit.1999.8.16
	pc.LineCapability		=	IrLAP_BPS_9600  | 
								IrLAP_BPS_19200 ;
							//	IrLAP_BPS_38400 ;
							//	| IrLAP_BPS_57600;
//	if(!ui_bIrDAExchangeMode)
		pc.LineCapability	|=	IrLAP_BPS_38400|
								IrLAP_BPS_57600|
								IrLAP_BPS_115200;
//	pc.LineCapability		=	IrLAP_BPS_9600|IrLAP_BPS_19200|
//								IrLAP_BPS_38400|IrLAP_BPS_57600|
//								IrLAP_BPS_115200;
// chul.end

	pc.DisconnectTime		=	IrLAP_SL_12s;
	pc.NoofBOF				=	IrLAP_BOF_0;
/*	pc.MinTurnaroundTime	= 	IrLAP_MinTurn_1ms ;*/
	IrDA_SetPortConfig2( &pc ) ;
}


void	IrAPP_OpenStack( void )
{
	// 프로토콜의 순서에 맞게 하위 계층부터 차례로 open을 한다.
	IrSIR_Open();
	IrLMP_Open();
	TTP_Open();
	IrDA_SetTxFrame( achSendBuf,IrDASENDSZ,IrLAPSENDWINSIZE);
	IrDA_SetRxFrame( achRecvBuf,IrDARECVSZ,IrLAPRECVWINSIZE);
	IrAPP_SetPortConfig();
}

void	IrAPP_CloseStack( void )
{
	// 프롵콜의 순서에 맞게 상위 계층부터 차례로 close한다.
	TTP_Close();
	IrLMP_Close();
	IrSIR_Close();
}


void	IrAPP_SetDeviceName( const char *name, const char *lname )
{
	int		classid, attrid ;

	IrLMP_SetShortDeviceName( IrLMP_CS_ASCII,(byte*)name, strlen( name )) ;
	classid	= IrLMP_AddClass( "Device", 6 ) ;
	attrid	= IrLMP_AddAttribute( classid, "DeviceName", 10 ) ;
	IrLMP_SetUserStringAttribute( attrid, IrLMP_CS_ASCII, lname, strlen( lname )+1 ) ;
}


void IrAPP_Pause(dword tout)
{
dword	Timer;
	SetTimer( &Timer, tout ) ;
	while(GetTimer( Timer ))
		;
}

void IrAPP_VersionEntry(const char *version)
{
int		classid, attrid ;
	classid = IrLMP_AddClass("IrDA:OSW",8);
	attrid	= IrLMP_AddAttribute(classid,"LegasicVersion",14);
	IrLMP_SetUserStringAttribute(attrid,IrLMP_CS_ASCII,version,strlen(version));
}

#ifdef	USE_MONITOR
void	IrAPP_PrimaryTest(void)
{
int		no,lsap;
dword	address;
	cprintf("IrAPP Primary Test\n");
	IrAPP_OpenStack();

	IrAPP_SetDeviceName( "Legasic Test","IrAPP Primary Test" ) ;
	IrLMP_SetServiceHint( IrLMP_SH_PDA ) ;
	IrLMP_SetServiceHint( IrLMP_SH_IrCOMM ) ;

	if( IrAPP_DiscoveryDevice())
		goto err;

	for( no = 0 ; no < DiscoveryFounds ; no++ )
		IrAPP_DisplayDeviceName(no);

	no = IrAPP_FoundServiceHint(IrLMP_SH_IrCOMM);
	if( no >= 0 ) {
		if( IrAPP_QueryLSAP(no ,"IrDA:IrCOMM","IrDA:TinyTP:LsapSel",&address,&lsap) ){
			cprintf("Not Have LSAP\n");
		}else{
			cprintf("LSAP = %d,ADDR = %ld",lsap,address);
		}
	}
err:
	IrAPP_Pause( 3000 ) ;
	IrAPP_CloseStack() ;
}

void	IrAPP_SecondaryTest(void)
{
}

#endif	/*USE_MONITOR*/
