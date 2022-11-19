/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       commtest.c                                   */
/*             Created:    7/28/96                                      */
/*             Modify:     7/06/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/*===========================================================================
                        EDIT HISTORY FOR MODULE


when		who		what, where, why
--------	---		----------------------------------------------------------
00.03.21    chos    IrDA Upgrade
00.03.18    chos    IrDA D1 porting

------------------------------------------------------------------------------*/

/************************************************************************

	IrCOMM Modem(DCE) / IrMS Control Program

************************************************************************/

/* $Header$ */
#include	<target.h>
#include	"irdaconf.h"
#include	<string.h>
#include	<irlap.h>
#include	<irda.h>
#include	<ircomm.h>
#include	<okmisc.h>
#include	<timer.h>
#include	"commctrl.h"
#include	"irdauty.h"
#include	"dcoldnm.h"

#include	"console.h"

#include	"siors232.h"

#define	API_BUFF_SIZE		256

#define	APP_INTERVAL		3000
#define	APP_CONNECT_TIME	10000							/* 接続要求からの待ち時間		*/
#define	APP_DISCON_WAIT1	2000							/* IrCOMM切断までの待ち時間		*/
#define	APP_DISCON_WAIT2	2000							/* IrLAP 切断までの待ち時間		*/
#define APP_DSCV_COUNT		5								/* DIscovery 回数				*/

//static	const char	MDM_NickName[]	= "SK C5ES-Board ";
//static	const char	MDM_LongName[]	= "(c) 1996 Okaya Systemware";
/* rosa 01-08-10 --> @why, what */
#if( MODEL_ID == MODEL_B2 )

	static	const char	MDM_NickName[]	= "SKY IM-3200 ";
	static	const char	MDM_LongName[]	= "(C) 2001 SK Teletech";

#elif	( MODEL_ID == MODEL_B1 )
/* <-- */

	static	const char	MDM_NickName[]	= "SKY IM-3100 ";
	static	const char	MDM_LongName[]	= "(C) 2001 SK Teletech";

#elif	( MODEL_ID == MODEL_A7 )

	static	const char	MDM_NickName[]	= "SKY IM-2000 ";
	static	const char	MDM_LongName[]	= "(C) 2000 SK Teletech";

#elif	( MODEL_ID == MODEL_A8 )

	static	const char	MDM_NickName[]	= "SKY IM-2100 ";
	static	const char	MDM_LongName[]	= "(C) 2000 SK Teletech";

#elif	( MODEL_ID == MODEL_A10 )

#ifdef A10I
    static	const char	MDM_NickName[]	= "SKY IM-2400S";
#else
	static	const char	MDM_NickName[]	= "SKY IM-2400 ";
#endif
	static	const char	MDM_LongName[]	= "(C) 2000 SK Teletech";

#elif  ( MODEL_ID == MODEL_D1)

	static	const char	MDM_NickName[]	= "SKY IM-D1  ";
	static	const char	MDM_LongName[]	= "(C) 2000 SK Teletech";

#else // #if	( MODEL_ID == MODEL_A7 )
// Modified by khekim 99/8/30
#if defined(TREX_C5_M)
static	const char	MDM_NickName[]	= "SKY IM-1200 ";
#elif defined(TREX_B5)
static	const char	MDM_NickName[]	= "SKY IM-1100 ";
#elif defined(TREX_C5)
static	const char	MDM_NickName[]	= "SKY IM-1000 ";
#else
static	const char	MDM_NickName[]	= "SKY IM-1000 ";
#endif
static	const char	MDM_LongName[]	= "(C) 1998 SK Teletech";
#endif // #if	( MODEL_ID == MODEL_A7 )

static	const char	MDM_Class[]		= "IrDA:IrCOMM";
static	const char	MDM_Attribute[] = "IrDA:TinyTP:LsapSel";

int					IrCOMM_Test_Send;						/*	IrCOMM Test Send En			*/
int					IrCOMM_Test_Recv;						/*	IrCOMM Test Recv En			*/

byte				API_Send_Buf[API_BUFF_SIZE];
byte				API_Recv_Buf[API_BUFF_SIZE];
dword				MDM_Count;


static	dword		Total_Send_Size;						/*	Total Send Size				*/
static	dword		Total_Recv_Size;						/*	Total Recv Size				*/

static	byte		APP_LAPConnect;							/* ＩｒＬＡＰのコネクト状況		*/
static	dword		APP_Interval;							/* ディスカバリ間隔の設定		*/
byte				APP_Primary;							/* プライマリ動作をする			*/
byte				APP_Secondary;							/* セカンダリ動作をする			*/

		int			MDM_Status;								/* モデムの状態遷移				*/
//static	
int					MDM_Slsap;
static	int			MDM_Dlsap;
static	dword		MDM_Daddress;

static	byte		MDM_ConnectPrimary;						/* プライマリ接続をした			*/																
static	byte		MDM_ConnectConf;						/* コネクトを待っている			*/


byte		MDM_DCEMode;							/* DCE として 動作をする		*/

static	dword		MDM_Speed;								/* 回線速度						*/
static	byte		MDM_Break;								/* ブレーク状態					*/
byte		MDM_DTELine;							/* ＤＣＥ側ラインステータス		*/
byte		MDM_DCELine;							/* ＤＴＥ側ラインステータス		*/

static	char		MDM_Xon,MDM_Xoff;						/* フロー制御キャラクタ			*/
static	char		MDM_Enq,MDM_Ack;						/* データ応答キャラクタ			*/

byte		MDM_RTS;
byte		MDM_DTR;

byte		MDM_CTS;
byte		MDM_DSR;
byte		MDM_CD;
byte		MDM_RI;

static	int			IrDACB	MDM_CBConnectInd( int slsap, dword addr, int dlsap, int servicetype ) ;
static	void		IrDACB	MDM_CBConnectCnf( int slsap, dword addr, int dlsap, int status ) ;
static	void		IrDACB	MDM_CBDisconnectInd( int lsap, int status ) ;
#ifndef FEATURE_USE_IRDA
static	void		IrDACB	MDM_CBDataInd( int lsap, int datasz ) ;
#endif
static	void		IrDACB	MDM_CBControlInd( int lsap, const void IrDAFAR *control, int controlsz ) ;

static	void				MDM_Polling(void);

static	void		IrDACB	IrLAP_ConnectPrv( void )
{
		APP_LAPConnect	= TRUE;
}

static	void		IrDACB	IrLAP_DisconnectPrv( void )
{
		APP_LAPConnect	= FALSE;
}

/******************************************************************************/

/*
**		モデム制御をバインドする
*/
//      modem control bind

static	int		MDM_Install( void )
{
COMMCALLBACK	ccb ;
int				i;
byte			*p;

	IrCOMM_Open();
	/* Internal Value Initialize			*/
	APP_LAPConnect		= FALSE;
	MDM_Status			= FALSE;
	MDM_DTELine		= 0;
	MDM_DCELine		= 0;

	/* Set Default			*/
	MDM_Xon		= IRCOMM_DEF_XON;   // 0x11
	MDM_Xoff	= IRCOMM_DEF_XOFF;  // 0x13
	MDM_Enq		= IRCOMM_DEF_ENQ;
	MDM_Ack		= IRCOMM_DEF_ACK;
	MDM_Break	= FALSE;


	/* Set Call Back Functions				*/

	memset( (void *)&ccb, 0, sizeof( ccb )) ;
	ccb.ConnectInd		= MDM_CBConnectInd ;
	ccb.ConnectCnf		= MDM_CBConnectCnf ;
	ccb.DisconnectInd	= MDM_CBDisconnectInd ;
// modified by chos 99.07.12
#ifdef	FEATURE_USE_IRDA
	ccb.DataInd			= IrDA_rx_isr;
#else	// #ifdef	FEATURE_USE_IRDA
	ccb.DataInd			= MDM_CBDataInd ;
#endif	// #ifdef	FEATURE_USE_IRDA
// finish chos
	ccb.ControlInd		= MDM_CBControlInd ;

	MDM_Slsap			= IrCOMM_RequestLSAP();

	if(APP_Primary)
		IrCOMM_BindLSAP( MDM_Slsap, &ccb, IrCOMM_ST_9WireDTE ); //99/6/10
	else
		IrCOMM_BindLSAP( MDM_Slsap, &ccb, IrCOMM_ST_9WireDCE ); //99/6/10
	IrAPP_SetIASClass( (char *)MDM_Class, (char *)MDM_Attribute, MDM_Slsap );
	IrLMP_SetServiceHint(IrLMP_SH_IrCOMM );
	
	/*	Send Data 	*/
	p	=	API_Send_Buf;
	for(i = 0 ; i < API_BUFF_SIZE ; i++,p++){
		*p	=	i;
	}
	return	MDM_Slsap;
}

/*
**		モデム制御をアンバインドする
*/

// modem control unbind
static	void	MDM_Remove( void )
{
	IrCOMM_UnbindLSAP( MDM_Slsap ) ;
	MDM_Status	= FALSE;
	IrCOMM_Close();
}


/*
**		接続中か問いあわせる
*/
static	IrDABOOL	MDM_isConnected( void )
{
	return( MDM_Status );
}

/******************************************************************************/

/*
**		接続時の初期化
*/
static	void MDM_ConnectProc(void)
{
	MDM_Status		=	TRUE;
	Total_Send_Size	=	0;
	Total_Recv_Size	=	0;
}

/*
**		２次局接続時の処理
*/
static	int	IrDACB	MDM_CBConnectInd( int slsap, dword daddr, int dlsap, int servicetype )
{
	MDM_Daddress	= daddr;
	MDM_Dlsap		= dlsap;

	if(APP_Secondary){		/* ２次局モードでなければつながらない	*/
		MDM_ConnectPrimary	= FALSE;
		MDM_ConnectProc();
		return( IrDA_CONNECTEN ) ;
	}else{
		return( IrDA_CONNECTDI ) ;
	}
}

/*
**		１次局接続時の処理
*/
static	void IrDACB	MDM_CBConnectCnf( int slsap, dword daddr, int dlsap, int status )
{
	if(status == IrDAR_CONNECTING){
		MDM_ConnectPrimary	= TRUE;
		MDM_ConnectProc();
	}
	MDM_ConnectConf = TRUE;
}

static void MDM_DisconnectProc(void)
{
	MDM_ConnectPrimary	= FALSE;
	MDM_Status			= FALSE;
}

/*
**		切断時の処理
*/
static	void	IrDACB	MDM_CBDisconnectInd( int lsap, int status )
{
	MDM_DisconnectProc();
}

/******************************************************************************/

/*
**		カテゴリ別コントロールデータ受信時の処理
*/

/*	Category 0x10	Set Baud Rate		*/
static	void	IrCOMM_Ctrl10H(byte *pv)
{
dword	baud ;
	GetBigEndian32( pv, &baud ) ;
	MDM_Speed	= baud;
}

/*	Category 0x11	Change Data Format	*/
static	void	IrCOMM_Ctrl11H(byte fmt)
{
}

/*	Category 0x12	Flow control		*/
static	void	IrCOMM_Ctrl12H(byte fmt)
{
}

/*	Category 0x13	XON/XOFF charcters	*/
static	void	IrCOMM_Ctrl13H(byte *pd)
{
	MDM_Xon  = *pd++;
	MDM_Xoff = *pd;
}

/*	Category 0x14	ENQ/ACK charcters	*/
static	void	IrCOMM_Ctrl14H(byte *pd)
{
	MDM_Enq = *pd++;
	MDM_Ack = *pd;
}

/*	Category 0x15	Line Status		*/
static	void	IrCOMM_Ctrl15H(byte stat)
{
}

/*	Category 0x16	Send Break		*/
static	void	IrCOMM_Ctrl16H(int flag)
{
	if(flag & 0x1){
		MDM_Break = TRUE;
	}else{
		MDM_Break = FALSE;
	}
}

/*	Category 0x20	DTE line Setting & Change	*/
static	void	IrCOMM_Ctrl20H(byte cline)
{
	if(MDM_DCEMode){
		if(cline & IRCOMM_DTR)
			MDM_DTR	= TRUE;
		else
			MDM_DTR = FALSE;

		if(cline & IRCOMM_RTS)
			MDM_RTS	= TRUE;
		else
			MDM_RTS	= FALSE;
		MDM_DTELine = cline;

		// added by chos 99.08.18 for DTR event
		{
			if ( ( cline & IRCOMM_DLT_DTR ) &&		// check DTR change
				( siors_dtr_func_ptr != NULL ) )		// check DTR event CB function
			{
				extern void siors_dtr_event_isr(void);
				siors_dtr_event_isr ();
			}
		}
		// finish chos
	}else{
	MDM_DTELine = cline;
	}
}

/*
	Category 0x21 DCE line Setting & Change
*/

static	void	IrCOMM_Ctrl21H(byte cline)
{
	if(MDM_DCEMode)
		return;
	if(cline & IRCOMM_CTS)
		MDM_CTS	= TRUE;
	else
		MDM_CTS	= FALSE;

	if(cline & IRCOMM_DSR)
		MDM_DSR	= TRUE;
	else
		MDM_DSR	= FALSE;

	if(cline & IRCOMM_RI)
		MDM_RI	= TRUE;
	else
		MDM_RI	= FALSE;

	if(cline & IRCOMM_CD)
		MDM_CD	= TRUE;
	else
		MDM_CD	= FALSE;
	MDM_DCELine = cline;
}

static	void	IrCOMM_Ctrl22H(void)
{
byte	cline,delta;
byte	buf[3];
	if(MDM_DCEMode){
		buf[0]	= 0x21;
		buf[1]	= 1;

		/* Set Current Line Staus	*/
		cline	= 0;
		if( MDM_CTS )
			cline	|= IRCOMM_CTS ;
		if( MDM_DSR )
			cline	|= IRCOMM_DSR;
		if( MDM_RI )
			cline	|= IRCOMM_RI;
		if( MDM_CD )
			cline	|= IRCOMM_CD;

		if(cline == MDM_DCELine)
			return;

		buf[2] = cline;
		/* Set Delta Line Status	*/
		delta	= MDM_DCELine ^ cline;
		if( delta & IRCOMM_CTS )
			buf[2]	|= IRCOMM_DLT_CTS;
		if( delta & IRCOMM_DSR )
			buf[2]	|= IRCOMM_DLT_DSR;
		if( delta & IRCOMM_RI )
			buf[2]	|= IRCOMM_DLT_RI;
		if( delta & IRCOMM_CD )
			buf[2]	|= IRCOMM_DLT_CD;

		if(IrCOMM_ControlReq( MDM_Slsap, buf, 3 ) >= 3)
			MDM_DCELine = cline;
	}else{
		buf[0]	= 0x20;
		buf[1]	= 1;

		/* Set Current Line Staus	*/
		cline	= 0;
		if( MDM_DTR )
			cline	|= IRCOMM_DTR ;
		if( MDM_RTS )
			cline	|= IRCOMM_RTS;

		if(cline == MDM_DTELine)
			return;

		buf[2] = cline;
		/* Set Delta Line Status	*/
		delta	= MDM_DTELine ^ cline;
		if( delta & IRCOMM_DTR )
			buf[2]	|= IRCOMM_DLT_DTR;
		if( delta & IRCOMM_RTS )
			buf[2]	|= IRCOMM_DLT_RTS;
		if(IrCOMM_ControlReq( MDM_Slsap, buf, 3 ) >= 3)
			MDM_DTELine = cline;
	}

}

/******************************************************************************/

/*
**		コントロールデータ受信時の処理
*/
static	void	IrDACB	MDM_CBControlInd( int lsap, const void IrDAFAR *control, int controlsz)
{
int	len ;
int	pi,pl;
byte	*pv ;
	for( len = 0 ; len < controlsz ; len +=pl+2 ) {
		pi	= ((byte*)control) [len  ] ;
		pl	= ((byte*)control) [len+1] ;
		pv	= &((byte*)control)[len+2] ;
		switch( pi ) {

			/* 3-Wire Control	*/

			case	0x10:	/* Data Rate	*/
				IrCOMM_Ctrl10H(pv);
				break ;

			case	0x11:	/* Data Format	*/
				IrCOMM_Ctrl11H(*pv);
				break ;

			case	0x12:	/* Flow Control	*/
				IrCOMM_Ctrl12H(*pv);
				break;

			case	0x13:	/* Set XON/XOFF Char	*/
				IrCOMM_Ctrl13H(pv);
				break;

			case	0x14:	/* Set ENQ/ACK Char	*/
				IrCOMM_Ctrl14H(pv);
				break;

			case	0x15:	/* Line Status Error	*/
				IrCOMM_Ctrl15H(*pv);
				break;

			case	0x16:	/* Send Break	*/
				IrCOMM_Ctrl16H(*pv);
				break;

			/* 9-Wire Control	*/

			case	0x20:	/* DTE Line Setting & Change */
				IrCOMM_Ctrl20H(*pv);
				break;

			case	0x21:	/* DCE Line Setting & Change */
				IrCOMM_Ctrl21H(*pv);
				break;

			case	0x22 :	/* Poll Line Setting */
				IrCOMM_Ctrl22H();
				break ;
		}
	}
}

/*
**		データ受信時の処理
*/
//     data receive processing
#ifndef FEATURE_USE_IRDA
static	void	IrDACB	MDM_CBDataInd( int lsap, int datasz )
{
int		len;
//	if(datasz >= 1){
		for(;IrCOMM_QueryRxData(MDM_Slsap) > 0;){
			len =	IrCOMM_RecvData( MDM_Slsap, API_Recv_Buf,sizeof(API_Recv_Buf));
			len	=	IrCOMM_SendData( MDM_Slsap,	API_Recv_Buf,	len );
//		API_Recv_Buf[0]	=	'B';
//		len	=	IrCOMM_SendData( MDM_Slsap,	API_Recv_Buf,	1 );
		}
}
#endif

/*
**		ポーリング方式でのデータ処理
*/

//     Polling BANGSIK data CHEO RI
static	void	MDM_Echo( void )
{
#if 0
int	size,len;
	if(MDM_isConnected()){
		/*	Recieve Only	*/
		if(IrCOMM_Test_Recv){
			len = IrCOMM_QueryRxData(MDM_Slsap);
			if(len > 0){
				if(len > 256)
					len = 256;
				len	= IrCOMM_RecvData( MDM_Slsap, API_Recv_Buf, len );
				Total_Send_Size	=	0;
				Total_Recv_Size	+=	len;
#if defined(USE_MONITOR)
/*				cprintf("[%d]",Total_Recv_Size);*/
#endif
			}
		}

		/*	Send Only	*/
		/*	256byte Unit Send	*/
		if(IrCOMM_Test_Send){
			size	= IrCOMM_QueryTxQueue( MDM_Slsap );
			if(size >= 256 ){
				len = IrCOMM_SendData( MDM_Slsap,API_Send_Buf,256 );
				Total_Send_Size	+=	len;
			}
		}
	}
#endif
}


/******************************************************************************/

/*
**		接続要求処理
*/

static	int	MDM_ConnectReq(void)
{
dword	timer;
int		ret;
	MDM_ConnectConf = FALSE;
	for(;;){
		ret = IrCOMM_ConnectReq(MDM_Slsap,MDM_Daddress,MDM_Dlsap);
		if( ret == IrDAR_NORESOURCE )
			continue;
		if( ret == IrDAR_BUSYWAIT )
			continue;
		if( ret == IrDAR_ACCEPT )
			break;

		if(MDM_isConnected()){
				return	FALSE;
		}
		SetTimer( &timer, 50L );
		while( GetTimer( timer ))
			;
	}

	SetTimer(&timer,APP_CONNECT_TIME);
	while(GetTimer(timer)){
		if(MDM_ConnectConf){
			if(MDM_isConnected()){
					return	FALSE;
			}
			goto wait_disc;
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

static	void	MDM_DisconnectReq( void )
{
dword	timer;
int	ret;
	SetTimer(&timer,APP_DISCON_WAIT1);
	while(GetTimer(timer)){
			MDM_Polling();
	}
	for(;;){
		ret = IrCOMM_DisconnectReq( MDM_Slsap );
		if( ret == IrDAR_ACCEPT)
			break;
		if( ret == IrDAR_NORESOURCE )
				continue;
		if( ret == IrDAR_BUSYWAIT )
			continue;
		break;
	}
	SetTimer(&timer,APP_DISCON_WAIT2);
	while(GetTimer(timer)){
		if(!APP_LAPConnect)
			return;
		MDM_Polling();
	}
	MDM_DisconnectProc();
}

/*
**		通信速度変更処理
*/
#if 0
static	void	MDM_SendSpeed(dword speed)
{
byte	param[6];

	param[0] = 0x10;
	param[1] = 0x04;
	SetBigEndian32( param + 2, speed);
	IrCOMM_ControlReq( MDM_Slsap, param, 6);

}

static	void	MDM_GetLineStatus(void)
{
byte	param[2];
	param[0] = 0x22;
	param[1] = 0x00;
	IrCOMM_ControlReq( MDM_Slsap, param, 2);
}
#endif
/*******************************************************************************


*******************************************************************************/

static	MDM_CheckKey(void)
{
#if defined(USE_MONITOR)
	if(cget() == 0x1b)  // not commented by htg.
		return	TRUE;   // not commented by htg.
#endif
	return	FALSE;
}

static	MDM_PrimaryProc(void)
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
			if(MDM_CheckKey())
				return	TRUE;
			if(APP_LAPConnect)
				return FALSE;
			if(MDM_isConnected()){
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
		no = 0;
#if defined(USE_MONITOR)
		cprintf("\nCheck Sirvice Hint(IrCOMM)..");
#endif
		no = IrAPP_FoundServiceHint(IrLMP_SH_IrCOMM);
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
		if(IrAPP_QueryLSAP( no ,(char*)MDM_Class,(char *)MDM_Attribute, &MDM_Daddress,&MDM_Dlsap)){
#if defined(USE_MONITOR)
			cprintf("Not Found\n");
#endif
			continue;
		}
#if defined(USE_MONITOR)
		cprintf("Found(LSAP=%d)\n",MDM_Dlsap);
#endif
		MDM_ConnectReq();
			return FALSE;
	}
	return	TRUE;
}


//static because called in the rs232c.
	
void	MDM_InitProc(void)
{
	IrAPP_OpenStack();     // protocol intialization.
	IrLAP_SetConnectPrv(IrLAP_ConnectPrv ) ;
	IrLAP_SetDisconnectPrv( IrLAP_DisconnectPrv ) ;
	IrAPP_SetDeviceName(MDM_NickName,MDM_LongName );
	MDM_Install();
	MDM_ConnectPrimary	= 0;
}

void	MDM_Disconnect(void);

void	MDM_ExitProc(void)
{
// added by chul
//	MDM_Disconnect();
// end of chul

	extern void	MsmTimerExit ( void );

	MsmTimerExit();		// added by chos 99.08.13

	MDM_Remove();
//	IrAPP_Pause(2000);
	IrAPP_CloseStack();
}


static	old,new;
static	void MDM_DispStatus(void)
{
	new = MDM_isConnected();
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

static	void	MDM_Polling(void)
{
	MDM_DispStatus();
	MDM_Echo();
}

static	void	MDM_Title(void){
#if defined(USE_MONITOR)
	cprintf("*****************************************\n");
	cprintf(" IrCOMM Loop Back Test\n");
	cprintf("     Okaya Systemware Co,.Ltd\n");
	cprintf("     Date:%s %s\n",__DATE__,__TIME__);
	cprintf(" Operation Mode:");
#endif
	if(APP_Primary){
#if defined(USE_MONITOR)
		cputs(" [Primary]\n");
#endif
	}else{
#if defined(USE_MONITOR)
		cputs(" [Secondary]\n");
#endif
	}
	if(IrCOMM_Test_Send){
#if defined(USE_MONITOR)
		cputs(" [Send:Enable]\n");
#endif
	}else{
#if defined(USE_MONITOR)
		cputs(" [Send:Disable]\n");
#endif
	}
	if(IrCOMM_Test_Recv){
#if defined(USE_MONITOR)
		cputs(" [Recv:Enable]\n");
#endif
	}else{
#if defined(USE_MONITOR)
		cputs(" [Recv:Disable]\n");
#endif
	}
#if defined(USE_MONITOR)
	cprintf("**************************************\n");
	cprintf("Send En.....S\n");
	cprintf("Recv En.....R\n");
	cprintf("Go..........G\n");
#endif
}

static	void	MDM_MainProc()
{
#if defined(USE_MONITOR)
char	cmd[5];
#endif
int		status;
	status	=	0;

//#ifdef OMIT	
	MDM_Title();   // Output title through UART
	for(;;){
		if(status)
			break;

#if defined(USE_MONITOR)
		cputs("Mode>");
#endif
#if defined(USE_MONITOR)
		if(cgets(cmd,5) == -1)
			continue;
		switch(*cmd){
			case	'S':	/* Send En 	*/
				IrCOMM_Test_Send	=	!IrCOMM_Test_Send;
				break;
			case	'R':	/* Recv En 	*/
				IrCOMM_Test_Recv	=	!IrCOMM_Test_Recv;
				break;
			case	'G':	/* Start 	*/
				status	=	1;
				break;
			default:
				break;
		}
#endif
		MDM_Title();
	}
#if defined(USE_MONITOR)
	cprintf("Abort :<ESC> key\n");
	cputs("Start .....\n");
#endif
//endif
	MDM_InitProc();
	old	= !MDM_isConnected();
	MDM_Count	= 0;
	for(;;){
		if(APP_Primary){
			if(MDM_PrimaryProc())
				goto end;
		}
		while(MDM_isConnected()){
			if(MDM_CheckKey())
				goto end;
			MDM_Polling();
		}
	}
end:
#if defined(USE_MONITOR)
	cprintf("Abort Test\n");
#endif
	if(MDM_isConnected()){
		MDM_DisconnectReq();
	}
	MDM_ExitProc();
}

void	MDM_GetSetting(int Prime,int Second)
{
	/*	Set default value	*/
	APP_Interval		=	APP_INTERVAL; // Period of discovery.
	APP_Primary			=	Prime;
	APP_Secondary		=	Second;
	if(APP_Primary)
		MDM_DCEMode			=	FALSE;         // DCE emulation 
	else
		MDM_DCEMode			=	TRUE;         // DCE emulation 
	IrCOMM_Test_Send	=	1;            // Send Enable
	IrCOMM_Test_Recv	=	1;            // Recv Enable.
}

/******************************************************************************/

void IrCOMM_PrimaryTest(void)
{
	MDM_GetSetting(TRUE,FALSE);
	MDM_MainProc();
}

void IrCOMM_SecondaryTest(void)
{
	MDM_GetSetting(FALSE,TRUE);
	MDM_MainProc();
}

// chul.add for primary IrDA device operation
/*

*/
short	MDM_CheckLAPConnection(void)
{
	return APP_LAPConnect;
}

void	MDM_Disconnect(void)
{
	dword	timer;
	int	ret, len;

	if(MDM_isConnected()){
//		SetTimer(&timer, 500); //APP_DISCON_WAIT1);
//		while(GetTimer(timer)){
//			MDM_Polling();
//		}
		SetTimer(&timer, 200); //APP_DISCON_WAIT1);
		while(GetTimer(timer)){
			len = IrCOMM_QueryRxData(MDM_Slsap);
			if(len > 0){
				if(len > 256)
					len = 256;
				len	= IrCOMM_RecvData( MDM_Slsap, API_Recv_Buf, len );
			}
		}
		for(;;){
			ret = IrCOMM_DisconnectReq( MDM_Slsap );
			if( ret == IrDAR_ACCEPT)
				break;
			if( ret == IrDAR_NORESOURCE )
			{
// modified by chos 99.08.30
//	disconnect ｽﾃ ｹｫﾇﾑ loopﾀﾏ ｰ豼�ｰ｡ ﾀﾖｴﾙ
//				continue;
				break;
// finish chos
			}
			if( ret == IrDAR_BUSYWAIT )
				continue;
			break;
		}
//		SetTimer(&timer, 500); //APP_DISCON_WAIT2);
//		while(GetTimer(timer)){
//			if(!APP_LAPConnect)
//				return;
//			MDM_Polling();
//		}
		SetTimer(&timer, 200); //APP_DISCON_WAIT1);
		while(GetTimer(timer)){
			len = IrCOMM_QueryRxData(MDM_Slsap);
			if(len > 0){
				if(len > 256)
					len = 256;
				len	= IrCOMM_RecvData( MDM_Slsap, API_Recv_Buf, len );
			}
		}
		MDM_DisconnectProc();
	}
}

/*
RETURN 
	0	: Search finished
	1~  : o.w.  

*/
short  MDM_SearchIrDADevice(void)
{
	int no;

	if(IrAPP_DiscoveryDevice()){
		return 1;
	}
	no = IrAPP_FoundServiceHint(IrLMP_SH_IrCOMM);
	if( no < 0)
	{
		return 2;
	}
	if(IrAPP_QueryLSAP( no ,(char*)MDM_Class,(char *)MDM_Attribute, &MDM_Daddress,&MDM_Dlsap)){
		return 3;
	}
	MDM_ConnectReq();
	return 0;
}
