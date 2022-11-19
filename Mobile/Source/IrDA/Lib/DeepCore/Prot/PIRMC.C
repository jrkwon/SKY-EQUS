/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IrMC.h                                       */
/*             Created:    3/25/97                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/* $Header:
*/

#define	IrDAPRIMARY
#include	"string.h"
#include	"irmc.h"


#define	IrLMP_SH_IrMC	14

#define	USE_AUTOOFF

#define	IrMCDATA		0x80	// Data    flag bit
#define	IrMCCTRLNS		0x20	// Control No Standard
#define	IrMCPNMASK		0x07	// PN Mask

#define	IrMCTTPPOS		0		// IrMC TTP  Position
#define	IrMCFLAGPOS		1		// IrMC Flag Position
#define	IrMCPCMPOS		2		// IrMC PCM  Position

#define	IrMCTTPCREDIT	1		// TTP default credit

#define	IrMCTTPLEN		1		// TTP 		body Size
#define	IrMCFLAGLEN		1		// FLAG	  	body Size

#define	IrMCPCMLEN		80		// ADPCM	body Size

#define	IrMCBEALENLEN	1		// BEARA len body Size
#define	IrMCBEAMAX		80		// BEARA max body Size

#define	IrMCCTRLLEN		6		// CONTROL	body Size
#define	IrMCPACKETLEN	86		// Packet	body Size

#define	IrMCPIAUDLEN	0x00	// PI for Audio Data length

#define	QUESIZE			256		// CONTROL 	que  Size

#define	QUEMASK			(QUESIZE-1)

typedef	struct	{
	volatile char	Mode ;
	volatile char	RealTime ;
//	volatile byte	Credit ;
	volatile char	Status ;
	volatile int	CreR,CreL;	// クレジット
	int				ClassId;
	int				LsapId;
	int				ParamId;
	int				AudLen;
	int				lsap;
	dword			Address ;
	MSCALLBACK		CallBack ;

	byte			CtrlTxQue[QUESIZE];
	volatile int	CtrlTxEnq ;
	volatile int	CtrlTxDeq ;

	byte			OCtrlTxQue[QUESIZE];
	volatile int	OCtrlTxEnq ;
	volatile int	OCtrlTxDeq ;

	byte			Data[IrMCPACKETLEN];
}	MSCONFIG ;

#define	MSM_Used		0x01
#define	MSM_Connected	0x02

#define	MS_S_IDLE		0
#define	MS_S_WAITI		1
#define	MS_S_WAITR		2
#define	MS_S_CONN		3

static	int		IrDACB	IrMC_LMConnectInd( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd ) ;
static	void	IrDACB	IrMC_LMConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status ) ;
static	void	IrDACB	IrMC_LMDisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz ) ;
static	void	IrDACB	IrMC_LMDataInd( int lsap, const void IrDAFAR *data, uint datasz ) ;

static	void	IrDACB	DmyConnectPrv( void )		{}
static	void	IrDACB	DmyDisconnectPrv( void )	{}

static	void	(IrDACB	*ConnectPrvPtr)( void ) ;
static	void	(IrDACB	*DisconnectPrvPtr)( void ) ;

static	MSCONFIG	Cf;
static	const byte	ConTTP	= IrMCTTPCREDIT;

static	const char	ClassName[]	= {IrMC_CLASS_NAME	};
static	const char	LsapName[]	= {IrMC_LSAP_NAME	};
static	const char	ParamName[]	= {IrMC_PARAM_NAME	};

static int	SendData( int dlen );

static	void	SetIASParameters( void )
{
int	cid;
	cid	= IrLMP_AddClass( ClassName,sizeof(ClassName)-1) ;
	Cf.ClassId	= 	cid;
	Cf.LsapId		=	IrLMP_AddAttribute( cid, LsapName,sizeof(LsapName)-1);
	Cf.ParamId	=	IrLMP_AddAttribute( cid, ParamName,sizeof(ParamName)-1);
}

/*
**	IrMC初期化
*/
int	IrDAAPI	IrMC_Open( void )
{
	IrDA_memset(&Cf,0,sizeof(Cf));
	SetIASParameters() ;
	ConnectPrvPtr		= DmyConnectPrv;
	DisconnectPrvPtr	= DmyDisconnectPrv;
	return( IrDAR_ACCEPT ) ;
}

/*
**	IrMC終了
*/
void	IrDAAPI	IrMC_Close( void )
{
}

/*
**	IrMC接続通知関数設定
*/
void	IrDAAPI	IrMC_SetConnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		ConnectPrvPtr	= func ;
	else
		ConnectPrvPtr	= DmyConnectPrv ;
}


/*
**	IrMC切断通知関数設定
*/
void	IrDAAPI	IrMC_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		DisconnectPrvPtr	= func ;
	else
		DisconnectPrvPtr	= DmyDisconnectPrv ;
}

/*
**	空きLSAP取得
**
**	戻り値
**		IrDAR_NORESOURCE	割り当て出来ない
**		>= 0				LSAP
*/
int	IrDAAPI	IrMC_RequestLSAP( void )
{
	return( IrLMP_RequestLSAP()) ;
}


/*
**	LSAP取得
**
**	戻り値
**		IrDAR_NORESOURCE	割り当て出来ないLSAPを指定した
**		IrDAR_INVALID		既に使用中
**		IrDAR_ACCEPT		要求受理
*/
int	IrDAAPI	IrMC_BindLSAP( MSCALLBACK IrDAFAR *ccb )
{
int			ret ,lsap;
LMCALLBACK	lcb ;
	if(Cf.Mode & MSM_Used)
		return	IrDAR_INVALID;
	lsap = IrLMP_RequestLSAP() ;
	if(lsap < 0)
		return	lsap;
	IrDA_memset( &lcb, 0, sizeof( LMCALLBACK )) ;
	lcb.ConnectInd		= IrMC_LMConnectInd ;
	lcb.ConnectCnf		= IrMC_LMConnectCnf ;
	lcb.DataInd			= IrMC_LMDataInd ;
	lcb.DisconnectInd	= IrMC_LMDisconnectInd ;

	ret	= IrLMP_BindLSAP( lsap,&lcb ) ;
	if( ret == IrDAR_ACCEPT ){
		IrDA_memset(&Cf,0,sizeof(Cf));
		Cf.Mode				= MSM_Used ;
		Cf.Status			= MS_S_IDLE ;
		Cf.lsap				= lsap;
		Cf.Data[IrMCTTPPOS]	= IrMCTTPCREDIT;
		Cf.AudLen			= IrMCPCMLEN;
		IrDA_memcpy( &Cf.CallBack, ccb, sizeof( MSCALLBACK )) ;
//		SetIASParameters() ;
		return	lsap;
	}
	return ret;
}

/*
**	LSAP開放
*/
void	IrDAAPI	IrMC_UnbindLSAP()
{
	if(Cf.Mode & MSM_Used ){
		Cf.Mode	= 0;
		IrLMP_UnbindLSAP( Cf.lsap ) ;
	}
}

/*
**		本当のLSAP番号を取得する
*/
int	IrDAAPI	IrMC_GetRealLSAP()
{
	if(Cf.Mode & MSM_Used){
		return	Cf.lsap + 1;
	}
	return	IrDAR_INVALID;
}

#if defined(IrDAPRIMARY)
/*
**	接続要求
**
**	戻り値
**		IrDAR_CONNECTING	既に接続中
**		IrDAR_INVALID		接続処理中
**		IrDAR_NORESOURCE	割り当てるデバイスアドレステーブルが無い
**		IrDAR_ACCEPT		要求受理
**		IrDAR_BUSYWAIT		要求拒否（忙しいので、しばらくまってから再試行して欲しい）
*/

int	IrDAAPI	IrMC_ConnectReq( dword addr, int dlsap )
{
int			ret ;
	if(( Cf.Mode & MSM_Used ) == 0 )
		return	IrDAR_INVALID;
	if( Cf.Mode & MSM_Connected )
		return	IrDAR_CONNECTING;
	if( Cf.Status != MS_S_IDLE )
		return	IrDAR_INVALID;
	Cf.CreL	= 1;	// ローカルクレジットは１
	Cf.CreR = 0;	// リモートクレジットは０
//	ret	= IrLMP_ConnectReq( addr, Cf.lsap, dlsap, &ConTTP, sizeof(ConTTP) ) ;
	ret	= IrLMP_ConnectReq( addr, Cf.lsap, dlsap, (byte *)&ConTTP, 1 ) ;
	if( ret == IrDAR_ACCEPT ){
		Cf.Status	= MS_S_WAITI ;
		Cf.Address	= addr ;
	}
	return	ret;
}
#endif	/*IrDAPRIMARY*/

/*
**	切断要求
**
**	戻り値
**		IrDAR_INVALID		接続していないか、正しくないステート
**		IrDAR_NORESOURCE	要求拒否（バッファフル）
**		IrDAR_ACCEPT		要求受理
*/
int	IrDAAPI	IrMC_DisconnectReq(void)
{
int			ret ;
	if(( Cf.Mode & MSM_Used ) == 0 )
		return	IrDAR_INVALID;
	if(( Cf.Mode & MSM_Connected ) == 0 )
		return	IrDAR_INVALID;

	switch( Cf.Status ) {
		case	MS_S_CONN :
			ret	= IrLMP_DisconnectReq( Cf.lsap, IrLMP_DR_UserRequest, 0, 0 ) ;
			if( ret == IrDAR_ACCEPT ) {
				Cf.Status	= MS_S_IDLE ;
				Cf.Mode	&= ~MSM_Connected ;
				(*DisconnectPrvPtr)() ;
			}
			return	ret;
		default:
			break;
	}
	return	IrDAR_INVALID;
}

static	int	IrDACB	IrMC_LMConnectInd(int slsap,dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd )
{
int			ret ;
byte		ttp;
	Cf.Status			= MS_S_WAITR ;
	if( Cf.CallBack.ConnectInd )
		ret = (*Cf.CallBack.ConnectInd)(addr, dlsap);
	else
		ret	= IrDA_CONNECTEN;

	if( ret == IrDA_CONNECTEN ){
		(*ConnectPrvPtr)() ;
		Cf.Status		= MS_S_CONN ;
		Cf.Mode			|= MSM_Connected ;
		Cf.Address		= addr ;
		Cf.CreR  		= lcd->Data[0] & 0x7f;	// TTP Credit
		Cf.CreL			= 1;					// ローカルクレジットは１
		Cf.AudLen		= IrMCPCMLEN;
	}else{
		Cf.Status	= MS_S_IDLE ;
	}
	// Initial TTP Data
	lcd->Data		= (byte *)&ConTTP;
//	lcd->DataSize	= sizeof(ConTTP);
	lcd->DataSize	= 1;
	return ret;
}

static	void	IrDACB	IrMC_LMConnectCnf( int slsap, dword addr, int dlsap, LMCONNDAT IrDAFAR *lcd, int status )
{
#if defined(IrDAPRIMARY)
	if(status == IrDAR_CONNECTING){
		Cf.Status	= MS_S_CONN ;
		Cf.Mode		|= MSM_Connected ;
		Cf.CreR		= lcd->Data[0] & 0x7f;	// TTP Credit
		Cf.AudLen	= IrMCPCMLEN;
	}else{
		Cf.Status	= MS_S_IDLE ;
	}
	(*ConnectPrvPtr)() ;
	if( Cf.CallBack.ConnectCnf){
		(Cf.CallBack.ConnectCnf)( addr, dlsap ,status);
	}
#endif	/*IrDAPRIMARY*/
}

static	void	IrDACB	IrMC_LMDisconnectInd( int lsap, int status, const void IrDAFAR *data, int datasz )
{
	if(( Cf.Mode & MSM_Used ) == 0 )
		return ;

	switch( Cf.Status ) {
		case	MS_S_IDLE :
				break ;
		case	MS_S_WAITI :
		case	MS_S_WAITR :
		case	MS_S_CONN :
			Cf.Status	= MS_S_IDLE ;
			Cf.Mode	&= ~MSM_Connected ;
			if( Cf.CallBack.DisconnectInd )
				(*Cf.CallBack.DisconnectInd)( status ) ;
			(*DisconnectPrvPtr)() ;
			break ;
	}
}

static	void	IrDACB	IrMC_LMDataInd( int lsap, const void IrDAFAR *data, uint size )
{
register	int	 	len,blen,type;
register	byte	flg;
register	byte	*ptr;
byte		pn,pi,pl;
	ptr	= (byte *)data;					// データの先頭
	Cf.CreR		+= (*ptr ++) & 0x7f;	// ＴＴＰのクレジットを読み取る
	if(Cf.CreL == 0){	// ローカルクレジットが０なら無視する
		return;
	}
	if(size == 1)	// データレスフローデータ
		return;
	Cf.CreL--;		// ローカルクレジットを減らす
	if( size < 3)	// 長さが変なのでパケットは無視する
		return ;
	len	= size - 2;						// size - TTPCREDIT - FLAG
	flg			= *ptr ++;				// フラグを読み取る
	pn			= flg & IrMCPNMASK;		// PN フィールドを得る

	if(flg & IrMCDATA){					// ボイスがあるならデフォルトはＡＤＰＣＭ
//		Cf.RealTime	= TRUE;				// ボイスがないならリアルタイムでなくなる
		blen	= Cf.AudLen;			// 現在のデータ長
		type	= IrMC_TYPE_STD;
	}else{
//		Cf.RealTime	= FALSE;			// ボイスがないならリアルタイムでなくなる
		blen		= 0;
	}
	for(;;){
		if(pn == 0)
			break;
		pi = *ptr++;					// パラメータＩＤ
		pl = *ptr++;					// パラメータ長
		if(pi == IrMCPIAUDLEN){			// ボイスデータの長さ指定があれば、ＡＤＰＣＭでないと判断する
			blen = *ptr;				// ボイスデータの長さを得る
			type = IrMC_TYPE_OTHER;		// スタンダードでないと判断する
		}
		ptr += pl;		// 次のパラメータをさし直す
		len -= pl + 2;	// パラメータ分スキップする
		pn--;			// パラメータ数を減少させる
	}

	if(blen){							// ボイスデータがあれば、Indicate する
		if( Cf.CallBack.AudioInd )
			(*Cf.CallBack.AudioInd)(type,ptr,blen);
		ptr += blen;	// ボイス分ポインターを進める
		len -= blen;	// 長さをボイス分減らす
	}

	if(len > 0){				// 残りは、コールコントロール
		if(flg & IrMCCTRLNS)
			type = IrMC_TYPE_OTHER;
		else
			type = IrMC_TYPE_STD;
		if( Cf.CallBack.ControlInd )
			(*Cf.CallBack.ControlInd)(type,ptr,len);
		if((blen == 0) && Cf.RealTime){	// リアルタイム時にコントロールだけ飛んできた場合
			SendData(0);				// ローカルクレジットをあげておく
		}
	}
}

static int	SendData( int dlen )
{
register	byte	*dp,*que,*data,flag;
byte		cre;
register	int		no,len,olen,cmax;
register	int		enq,deq,ret;
	if(Cf.CreL == 0){	// 自分のクレジットが０なら補充する
		cre	= 1;
	}else{
		cre	= 0;
	}
	if(Cf.CreR == 0){	// 相手のレジットが０なら送信できない
		if(cre){		// データレスフローデータを送信
			if(IrLMP_DataReq(Cf.lsap,&cre,1) == IrDAR_ACCEPT){
				Cf.CreL += cre;
			}
		}
		return	IrDAR_BUSYWAIT;
	}
	data = Cf.Data;
	data[IrMCTTPPOS] = cre;
	if(dlen){
		// もし、ボイスデータありならばコントロールは６以下
		len	= IrMCTTPLEN + IrMCFLAGLEN + dlen;
		cmax	= IrMCCTRLLEN;
	}else{
		// それ以外は、最大データ分用意する
		len	= IrMCTTPLEN + IrMCFLAGLEN;
		cmax	= IrMCPACKETLEN;
	}
	dp	= data + len;
	enq	= Cf.CtrlTxEnq;
	deq = Cf.CtrlTxDeq;
	if(enq != deq){			// 標準コントロールがあるなら
		que = Cf.CtrlTxQue;
		for(no = 0;no < cmax;no++){		// コールコントロールデータをコピーする
			if(enq == deq)
				break;
			*dp++ = que[deq];
			deq = (deq + 1) & QUEMASK;
			len++;
		}
		ret = IrLMP_DataReq(Cf.lsap,data,len);
		if(ret == IrDAR_ACCEPT){
			Cf.CreL	+= cre;
			Cf.CreR	--;
			Cf.CtrlTxDeq		= deq;
			data[IrMCFLAGPOS]	= 0;
		}
		return	ret;
	}
	enq	= Cf.OCtrlTxEnq;
	deq = Cf.OCtrlTxDeq;
	if(enq != deq){			// 非標準コントロールがあるなら
		que = Cf.OCtrlTxQue;
		// コールコントロールデータをコピーする
		for(no = 0;no < cmax;no++){
			if(enq == deq)
				break;
			*dp++ = que[deq];
			deq = (deq + 1) & QUEMASK;
			len++;
		}
		data[IrMCFLAGPOS] |= IrMCCTRLNS;
		ret = IrLMP_DataReq(Cf.lsap,data,len);
		if(ret == IrDAR_ACCEPT){
			Cf.CreL	+= cre;
			Cf.CreR	--;
			Cf.OCtrlTxDeq		= deq;
			data[IrMCFLAGPOS]	= 0;
		}
		return	ret;
	}
	if(dlen){	// ボイスデータ単独
		ret = IrLMP_DataReq(Cf.lsap,data,len);
		if(ret == IrDAR_ACCEPT){
			Cf.CreL	+= cre;		// 自分のクレジットを上げる
			Cf.CreR	--;			// 相手のクレジットを下げる
		}
	}else{
		if(cre){	// クレジットを上げる
			// データレスフローデータを送信
			ret = IrLMP_DataReq(Cf.lsap,&cre,1);
			if(ret == IrDAR_ACCEPT){
				Cf.CreL	+= cre;	// 自分のクレジットを上げる
			}
		}
	}
	return IrDAR_ACCEPT;
}

int	IrDAAPI	IrMC_AudioReq(int type, const byte *Aud,int size)
{
register	byte	*dp;
	if(!(Cf.Mode & MSM_Connected))
		return( IrDAR_INVALID ) ;

	if(!Cf.RealTime)
		return( IrDAR_INVALID );

	dp = Cf.Data + IrMCFLAGPOS;
	if(type){
		// 非標準ボイスデータ
		if(size > IrMCPCMLEN)	// 長さは、８０バイト以下
			size	= IrMCPCMLEN;
		*dp++ = IrMCDATA | 1;	// voice flag + (PN = 1)
		*dp++ = IrMCPIAUDLEN;	// PI = AUDIO LENGTH
		*dp++ = 1;				// PL = 1
		*dp++ = size;			// PV = data size
		IrDA_memcpy(dp ,Aud,size);
		size += 3;
	}else{
		// 標準ボイスデータ
		*dp++ = IrMCDATA;		// voice flag
//		size  = IrMCPCMLEN;		// 長さは、８０バイト固定
		IrDA_memcpy(dp ,Aud,size);
	}
	return	SendData( size );
}

int	IrDAAPI	IrMC_ControlReq(int type, const byte *ctrl,int size )
{
int		nenq,enq,deq,len;
byte	*que;
	if(!(Cf.Mode & MSM_Connected))
		return( IrDAR_INVALID ) ;
	len = size;
	if(size != 0){
		if(type){
			// 非標準コントロール
			deq = Cf.OCtrlTxDeq;
			enq	= Cf.OCtrlTxEnq;
			que = Cf.OCtrlTxQue;
		}else{
			// 標準コントロール
			deq = Cf.CtrlTxDeq;
			enq	= Cf.CtrlTxEnq;
			que = Cf.CtrlTxQue;
		}
		while(len){
			nenq = (enq + 1) & QUEMASK;
			if(nenq == deq)
				break;
			que[enq] = *ctrl++;
			enq = nenq;
			len --;
		}
		if(type){
			Cf.OCtrlTxEnq = enq;
		}else{
			Cf.CtrlTxEnq = enq;
		}
	}
	if(!Cf.RealTime){
		Cf.Data[IrMCFLAGPOS] = 0x00;
		SendData( 0 );
	}
	return	size - len;
}

int		IrDAAPI	IrMC_QueryTxFree( int type )								/* 送信キューの空き領域を取得	*/
{
int		enq,deq,len;
	if(!(Cf.Mode & MSM_Connected))
		return( IrDAR_INVALID ) ;
	if(type){
			// 非標準コントロール
			deq = Cf.OCtrlTxDeq;
			enq	= Cf.OCtrlTxEnq;
	}else{
			// 標準コントロール
			deq = Cf.CtrlTxDeq;
			enq	= Cf.CtrlTxEnq;
	}
	for(len = 0;;len++){
		enq = (enq + 1) & QUEMASK;
		if(enq == deq)
			break;
	}
	return	len;
}

void	IrMC_SetRealTime(int sw)
{
	Cf.RealTime = sw;
}

int		IrMC_QueryRealTime(void)
{
	return	Cf.RealTime;
}

void	IrMC_SetAudioLen(int len)
{
	Cf.AudLen = len;
}


/* $Log:
 * 
 */
