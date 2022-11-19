/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       rtcon.c	                                    */
/*             Created:    7/28/96                                      */
/*             Modify:     7/06/97                                      */
/*			   Modify:	   12/18/97										*/
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/************************************************************************

		IrMC Control Program

************************************************************************/

/* $Header$ */

#include	<string.h>
#include	"irdaconf.h"
#include	<irsir.h>
#include	<timer.h>
#include	<irlap.h>
#include	<irmc.h>
#include	"irdauty.h"
#include	"console.h"

#undef		MC_DEBUG

#define	USE_CLEAR_CONNECT									/* コネクト時にバッファをクリアする	*/
#define	USE_SERVICE_HINT									/* サービスヒントをチェックする		*/
#define	USE_REENTRANT

#define	DEF_INTERVAL		3000

#define	APP_CONNECT_TIME	10000							/* 接続要求からの待ち時間		*/
#define	APP_DISCON_WAIT1	2000							/* IrCOMM切断までの待ち時間		*/
#define	APP_DISCON_WAIT2	2000							/* IrLAP 切断までの待ち時間		*/

#define	APP_INTERVAL		3000
#define APP_DSCV_COUNT		5								/* DIscovery 回数				*/
#define	DEF_MCINT_INTERVAL	20								/*	MC Interrupt	*/
#define	DEF_INDICATION_INTERVAL	1000


#define	MC_PRILEN	80
#define	MC_AUDLEN	16


#define	CC_QUE_SIZE	32		/*	128	*/

unsigned char	ccRxQue[CC_QUE_SIZE];
int				ccRxEnq,ccRxDeq,ccRxLen;

	int DissFlag ;
extern int LinkFlag ;
extern int CallStatus ;
void atcmdreq(void) ;
/*******************************************************/

/*used for DPMP*/
int RetValue ;

/*1月9日
static	byte Buff[MC_PRILEN];
static int EmptyFlag ;

**********/

/*used to Call Control******/
typedef struct {
	int init ;
	int type ;
	int size ;
	unsigned char cmd[21] ;
}CMDTYPE ;
CMDTYPE cmdstruct ;

typedef struct {
	int len ;
	unsigned char cmd[21] ;
}CC_CMD;

CC_CMD cc_cmdreq ;

/*************/

static	const char	MC_NickName[]	= "IrMC Test";
static	const char	MC_LongName[]	= "(c) 1996 Okaya Systemware";

//static	const char	MC_Hint1		= IrLMP_SH_Modem;
//static	const char	MC_Hint2		= IrLMP_SH_Telephony;

static	const char	MC_Class[]		= "IrDA:TELECOM";
static	const char	MC_Attribute[]	= "IrDA:TinyTP:LsapSel";

static	dword		Total_Send_Size;						/*	Total Send Size				*/
static	dword		Total_Recv_Size;						/*	Total Recv Size				*/

static	byte		APP_LAPConnect;							/* ＩｒＬＡＰのコネクト状況		*/
static	dword		APP_Interval;							/* ディスカバリ間隔の設定		*/
static	byte		APP_Primary;							/* プライマリ動作をする			*/
static	byte		APP_Secondary;							/* セカンダリ動作をする			*/

static	byte		MC_Status;								/* モデムの状態遷移				*/
static	byte		MC_Request;

static	int			MC_Slsap;
static	int			MC_Dlsap;
static	dword		MC_Daddress;

static	byte		MC_ConnectPrimary;						/* プライマリ接続をした			*/
static	byte		MC_ConnectConf;							/* コネクトを待っている			*/

static	byte		MC_TxFullFlag;
static	byte		MC_RxFullFlag;

static	byte	TxBuff[MC_PRILEN];
static	byte	RxBuff[MC_PRILEN];
static	byte	NoData[MC_PRILEN];


void MsmAudIack(void);
void IrMC_SecondaryTest(void);
void IrMC_PrimaryTest(void);


/*
**		コールバック関数の先行定義
*/

static	int			IrDACB	MC_CBConnectInd( dword addr, int dlsap );
static	void		IrDACB	MC_CBConnectCnf( dword addr, int dlsap, int status ) ;
static	void		IrDACB	MC_CBDisconnectInd(int status ) ;
static	void		IrDACB	MC_CBAudInd( int type, const byte IrDAFAR *aud, int size );
static	void		IrDACB	MC_CBControlInd( int type, const byte IrDAFAR *cmd, int size );

static	void		IrDACB	IrLAP_ConnectPrv( void );
static	void		IrDACB	IrLAP_DisconnectPrv( void );
static	void		IrDACB	IrLAP_TxEmptyPrv( void );

static	void		IrDACB	CB_MC_INT( void );	/*	MC Interrup	*/


static	void		MC_Install( void );
static	void		MC_Remove( void );
static	IrDABOOL	MC_isConnected( void );
static	void		MC_ConnectProc(void);
static	void		MC_DisconnectProc(void);
static	int			MC_ConnectReq(void);
static	void		MC_Polling(void);
static	void		MC_DisconnectReq( void );
static	void		MC_CBPdcCmdInd( int len );
static	void		MC_Polling(void);
static	void		MC_DispStatus(void);
static	void		APP_InitProc(void);
static	void		APP_ExitProc(void);
static	IrDABOOL	MC_PrimaryProc(void);
static	IrDABOOL	MC_CheckKey(void);
static	void		IrMC_MainProc(void);
static	void		GetSetting(void);

static	void		IrDACB	IrLAP_ConnectPrv( void )
{
		APP_LAPConnect	= TRUE;
}

static	void		IrDACB	IrLAP_DisconnectPrv( void )
{
		APP_LAPConnect	= FALSE;
}


static	void	MC_Install(void)
{
MSCALLBACK		ccb ;
int				classid, attrid ;
unsigned char 	buf[32] ;
unsigned char 	*bufp ;

	APP_LAPConnect	=	FALSE;
	MC_Status		=	FALSE ;
	
	DissFlag = 0 ;
	/* IrMC をオープンする		*/
	IrMC_Open();

	memset( &ccb, 0, sizeof( ccb )) ;
	ccb.ConnectInd		= MC_CBConnectInd ;
	ccb.ConnectCnf		= MC_CBConnectCnf ;
	ccb.DisconnectInd	= MC_CBDisconnectInd ;
	ccb.AudioInd			= MC_CBAudInd ;
	ccb.ControlInd			= MC_CBControlInd ;
	RetValue = IrMC_BindLSAP( &ccb );
	IrAPP_SetIASClass( (char *)MC_Class,(char *)MC_Attribute, MC_Slsap );

/* パラメータ登録 */
	classid	= IrLMP_AddClass( "IrDA:TELECOM", 12 ) ;
	attrid	= IrLMP_AddAttribute( classid, "Parameters", 10 ) ;
	bufp	= buf ;
	*bufp++	= 0x00 ;						/*PI PhoneBook Access Support	*/
	*bufp++	= 0x01 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV					*/

	*bufp++	= 0x02 ;						/*PI PhoneBook Versiont	*/
	*bufp++	= 0x02 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV 1.0				*/
	*bufp++	= 0x00 ;						/*PV					*/

	*bufp++	= 0x20 ;						/*PI Message Access Support		*/
	*bufp++	= 0x01 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV					*/

	*bufp++	= 0x22 ;						/*PI Message Version	*/
	*bufp++	= 0x02 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV 1.0				*/
	*bufp++	= 0x00 ;						/*PV					*/
	
	*bufp++	= 0x40 ;						/*PI AudioCoding		*/
	*bufp++	= 0x01 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV 32K ADPCM			*/

	*bufp++	= 0x41 ;						/*PI Audio Version		*/
	*bufp++	= 0x02 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV 1.0				*/
	*bufp++	= 0x00 ;						/*PV					*/

	*bufp++	= 0x50 ;						/*PI CC Version			*/
	*bufp++	= 0x02 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV 1.0				*/
	*bufp++	= 0x00 ;						/*PV					*/

	*bufp++	= 0x51 ;						/*PI CC Indtificastion	*/
	*bufp++	= 0x01 ;						/*PL					*/
	*bufp++	= 0x01 ;						/*PV ME					*/
	
	IrLMP_SetOctetAttribute( attrid, buf, 28 ) ;
	/* Internal Value Initialize	*/
	MC_TxFullFlag	= FALSE;
	MC_RxFullFlag	= FALSE;
	
	memset( NoData, 0xff, sizeof(NoData));
	memset( TxBuff, 0xff, sizeof(TxBuff));
	memset( RxBuff, 0xff, sizeof(RxBuff));
}



static	void	MC_Remove( void )
{
	IrMC_UnbindLSAP() ;
	MC_Status	= FALSE;
	IrMC_Close();
}

/*
**		接続中か問いあわせる
*/
static	IrDABOOL MC_isConnected( void )
{
	return( MC_Status );
}

/**************************************************************/


/*
**		接続時の初期化
*/
static	void MC_ConnectProc(void)
{
	MC_Status	= TRUE;
	Total_Send_Size	=	0;
	Total_Recv_Size	=	0;
	IrMC_SetRealTime(1);
}

static void MC_DisconnectProc(void)
{
	MC_Status	= FALSE ;
	IrMC_SetRealTime(0);
}

/*
**		IrMC ２次局接続時の処理
*/
static	int	IrDACB	MC_CBConnectInd( dword daddr, int dlsap)
{
#if defined(MC_DEBUG)
	cputs("[CB]MC_CBConnectInd\n");
#endif
	MC_Daddress	= daddr;
	MC_Dlsap	= dlsap;

	if(!APP_Secondary)		/* ２次局モードでなければつながらない	*/
		goto NoConnect;
	MC_ConnectPrimary	= FALSE;
	MC_ConnectProc();
	return( IrDA_CONNECTEN ) ;
NoConnect:
	return( IrDA_CONNECTDI ) ;
}

/*
**		IrMC １次局接続時の処理
*/
static	void IrDACB	MC_CBConnectCnf( dword daddr, int dlsap, int status )
{
#if defined(MC_DEBUG)
	cputs("[CB]MC_CBConnectCnf\n");
#endif
	if(status == IrDAR_CONNECTING){
		MC_ConnectPrimary	= TRUE;
		MC_ConnectProc();
	}
	MC_ConnectConf = TRUE;
}

/*
**		切断時の処理
*/
static	void	IrDACB	MC_CBDisconnectInd(int status )
{
#if defined(MC_DEBUG)
	cputs("\n[CB]MC_CBDisconnectInd\n");
#endif
	MC_DisconnectProc();
	DissFlag = 1 ;
}

static	int	MC_ConnectReq(void)
{
dword	timer;
int		ret;
	MC_ConnectConf = FALSE;
	for(;;){
		ret = IrMC_ConnectReq(MC_Daddress,MC_Dlsap);
		if( ret == IrDAR_ACCEPT )
			break;
		if( ret != IrDAR_BUSYWAIT && ret != IrDAR_NORESOURCE ) {
			return	TRUE;
		}
		SetTimer( &timer, 50L );
		while( GetTimer( timer ))
			;
	}
	SetTimer(&timer,APP_CONNECT_TIME);
	while(GetTimer(timer)){
		if(MC_ConnectConf){
			if(MC_isConnected()){
					return	FALSE;
			}else{
					goto wait_disc;
			}
		}
	}
	return	TRUE;
wait_disc:
	SetTimer(&timer,APP_CONNECT_TIME);
	while(GetTimer(timer)){
		if(!APP_LAPConnect)
			return	TRUE;
	}
	return	TRUE;
}

/*
**		切断要求発行
*/

static	void	MC_DisconnectReq( void )
{
dword	timer;
	SetTimer(&timer,APP_DISCON_WAIT1);
	while(GetTimer(timer)){
			MC_Polling();
	}
	IrMC_DisconnectReq();
	SetTimer(&timer,APP_DISCON_WAIT2);
	while(GetTimer(timer)){
			MC_Polling();
	}
	MC_DisconnectProc();
}


static	void IrDACB	MC_CBAudInd( int type ,const byte IrDAFAR *aud,int size)
{
int	ret,i;
	/*****		RecvData	->	Audio 		******/
	Total_Recv_Size	+=	size;
	
	/*****		Secondary Tx Send			*******/
	if(MC_ConnectPrimary){	/*	Primary		*/
		/*	RecvData -> Audio	*/
	}else{				/*	Sencondary	*/
		/*	RecvData -> Audio	*/
		/*	TxData	->	IrMC_Datareq	*/
		for(i = 0 ; i < 5 ; i++){
			ret = IrMC_AudioReq( IrMC_TYPE_STD,TxBuff,MC_PRILEN );
			switch(ret){
				case	IrDAR_ACCEPT:
					Total_Send_Size	+=	MC_PRILEN;
					return;
				default:
					continue;
			}
		}
	}
}

static	void	IrDACB	IrLAP_TxEmptyPrv( void )
{
}

/******************************************************************

	Command Call Back

*******************************************************************/
static	void	IrDACB	MC_CBControlInd( int type ,const byte IrDAFAR *cmd,int size)
{
int i ;
int RxEnqo,RxEnqn;
#if defined(MC_DEBUG)
	cputs("[CB]MC_CBControlInd -> ");
#endif
	for(i=0; i<size; i++){
		RxEnqo = ccRxEnq;
		RxEnqn = RxEnqo + 1;
		if(RxEnqn == CC_QUE_SIZE)
				RxEnqn = 0;
		if(RxEnqn != ccRxDeq){
			ccRxQue[RxEnqo] = *cmd ;
			cmd++ ;
			ccRxEnq = RxEnqn;
			ccRxLen++;
		}
	}
}

static	void			MC_CBPdcCmdInd( int len )
{
/*
	IrMC_CmdReq( MC_Slsap,PdcCmdRxBuffer,len);
*/
}

static	old,new;
static	void MC_DispStatus(void)
{
	new = MC_isConnected();
	if(new != old){
		old = new;
		if(new){
#if defined(USE_MONITOR)
			cprintf("(Connect)\n");
#endif
		}else{
#if defined(USE_MONITOR)
			cprintf("(Disconnect)\n");
#endif
		}
	}
}

static	void	MC_Polling(void)
{
	MC_DispStatus();
}


static	void	APP_InitProc(void)
{
	IrAPP_OpenStack();
	IrLAP_SetConnectPrv( IrLAP_ConnectPrv ) ;
	IrLAP_SetDisconnectPrv( IrLAP_DisconnectPrv ) ;
//	IrLAP_SetTxEmptyPrv( IrLAP_TxEmptyPrv );
	IrAPP_SetDeviceName((char *)MC_NickName,(char *)MC_LongName);
	MC_Install();
	MC_ConnectPrimary	= 0;
}

static	void	APP_ExitProc(void)
{
	MC_Remove();
	IrAPP_Pause(2000);
	IrAPP_CloseStack();
}


/******************************************************************************/

static	IrDABOOL	MC_PrimaryProc(void)
{
dword	timer;
int		no,ret;
	for(;;){
		if(!APP_Primary)
			return FALSE;
		if(APP_LAPConnect)
			return FALSE;
#if defined(USE_MONITOR)
		cprintf("Silence Time (%dmS)\n",APP_Interval);
#endif
		SetTimer(&timer,APP_Interval);
		while(GetTimer(timer)){
			if(MC_CheckKey())
				return	TRUE;
			if(APP_LAPConnect)
				return FALSE;
			if(MC_isConnected()){
#if defined(USE_MONITOR)
				cprintf("Connected(Other)\n");
#endif
				return FALSE;
			}
		}
#if defined(USE_MONITOR)
		cprintf("Discovery..");
#endif
		ret = IrAPP_DiscoveryDevice();
		if(ret){
#if defined(USE_MONITOR)
			cprintf("(Device Not Found)\n");
#endif
			continue;
		}
		IrAPP_DisplayDeviceName(ret);
		
		no = 0;
//		cprintf("\nCheck Sirvice Hint(IrCOMM)..");
//		no = IrAPP_FoundServiceHint(IrLMP_SH_IrCOMM);
		if(no < 0){
#if defined(USE_MONITOR)
			cprintf("(Not Found)\n");
#endif
			continue;
		}
#if defined(USE_MONITOR)
		cprintf("(Found (%d)\n",no);
		cprintf("Query LSAP..");
#endif
		if(IrAPP_QueryLSAP( no ,(char*)MC_Class,(char *)MC_Attribute, &MC_Daddress,&MC_Dlsap)){
#if defined(USE_MONITOR)
			cprintf("Not Found\n");
#endif
			continue;
		}
#if defined(USE_MONITOR)
		cprintf("Found(LSAP=%d)\n",MC_Dlsap);
#endif
		MC_ConnectReq();
			return FALSE;
	}
	return TRUE;
}


static	IrDABOOL	MC_CheckKey(void)
{
#if defined(USE_MONITOR)
	if(cget() == 0x1b)
		return	TRUE;
#endif
	return	FALSE;
}


static	void IrMC_MainProc(void)
{
dword	count;
	APP_InitProc();
	count	=	0;
	for(;;){
		if(APP_Primary){
			if(MC_PrimaryProc())
				goto end;
		}
		while(MC_isConnected()){
			count++;
			if(count > DEF_INDICATION_INTERVAL){
				count	=	0;
#if defined(USE_MONITOR)
				cprintf("[SendSize :%10lu bytes][RecvSize :%10lu bytes]\r",Total_Send_Size,Total_Recv_Size);
#endif
			}
			if(MC_CheckKey())
				goto end;
			MC_Polling();
		}
	}
end:
#if defined(USE_MONITOR)
	cprintf("Abort Test\n");
#endif
	if(MC_isConnected()){
		MC_DisconnectReq();
	}
	APP_ExitProc();
}

static	void	GetSetting(void)
{
	/*	Set default value	*/
	APP_Interval	= DEF_INTERVAL;
	APP_Primary		= FALSE;
	APP_Secondary	= FALSE;
}

void IrMC_SecondaryTest(void)
{
	GetSetting();

	APP_Primary 	= FALSE;
	APP_Secondary 	= TRUE;
	IrMC_MainProc();
}

void IrMC_PrimaryTest(void)
{
	GetSetting();
	APP_Primary 	= TRUE;
	APP_Secondary 	= TRUE;
	MC_Request		= TRUE;
	IrMC_MainProc();
}

void atcmdreq(void)
{
int i;
dword timer ;

	i = 0 ;
	while(i != cc_cmdreq.len)
		i = IrMC_ControlReq(IrMC_TYPE_STD,cc_cmdreq.cmd,cc_cmdreq.len);
		SetTimer(&timer,75);
		while(GetTimer(timer)){
		}
	}


void	DPMP_APP_ExitProc(void)
{
dword timer_buf ;

	MC_Remove();
	/*IrAPP_Pause(2000); 最適化オプションでこける？*/
	SetTimer(&timer_buf, 2000L);
	for(;;){
		if(GetTimer(timer_buf) == 0L){
			break ;
		}
	}	
}

static	void		IrDACB	CB_MC_INT( void ){
	MsmAudIack();
}

/************************************************************************/
/*		MSM Audio Intrrupt Handler											*/
/************************************************************************/
void MsmAudIack(void)
{
byte	*pbuf;
int		i;
	pbuf	=	&TxBuff[0];
	for(i = 0 ; i < MC_PRILEN ; i++,pbuf++){
		*pbuf	=	i;
	}
	if(MC_isConnected()){
		if(MC_ConnectPrimary){	/*	Primary 	*/
			/*****	Audio -> TxBuf -> IrMC_DataReq	*/
			IrMC_AudioReq( IrMC_TYPE_STD,TxBuff,MC_PRILEN );
			Total_Send_Size	+=	MC_PRILEN;
		}else{				/*	Secondary	*/
			/*****	DataHold	*****/
		}
	}
}


/* 
$Log$
*/
