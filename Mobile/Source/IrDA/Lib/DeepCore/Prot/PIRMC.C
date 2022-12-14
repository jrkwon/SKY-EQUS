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
	volatile int	CreR,CreL;	// ?N???W?b?g
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
**	IrMC??????
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
**	IrMC?I??
*/
void	IrDAAPI	IrMC_Close( void )
{
}

/*
**	IrMC???????m????????
*/
void	IrDAAPI	IrMC_SetConnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		ConnectPrvPtr	= func ;
	else
		ConnectPrvPtr	= DmyConnectPrv ;
}


/*
**	IrMC???f???m????????
*/
void	IrDAAPI	IrMC_SetDisconnectPrv( void (IrDACB *func)( void ))
{
	if( func )
		DisconnectPrvPtr	= func ;
	else
		DisconnectPrvPtr	= DmyDisconnectPrv ;
}

/*
**	????LSAP????
**
**	?????l
**		IrDAR_NORESOURCE	?????????o??????
**		>= 0				LSAP
*/
int	IrDAAPI	IrMC_RequestLSAP( void )
{
	return( IrLMP_RequestLSAP()) ;
}


/*
**	LSAP????
**
**	?????l
**		IrDAR_NORESOURCE	?????????o??????LSAP???w??????
**		IrDAR_INVALID		?????g?p??
**		IrDAR_ACCEPT		?v??????
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
**	LSAP?J??
*/
void	IrDAAPI	IrMC_UnbindLSAP()
{
	if(Cf.Mode & MSM_Used ){
		Cf.Mode	= 0;
		IrLMP_UnbindLSAP( Cf.lsap ) ;
	}
}

/*
**		?{????LSAP??????????????
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
**	?????v??
**
**	?????l
**		IrDAR_CONNECTING	??????????
**		IrDAR_INVALID		??????????
**		IrDAR_NORESOURCE	???????????f?o?C?X?A?h???X?e?[?u????????
**		IrDAR_ACCEPT		?v??????
**		IrDAR_BUSYWAIT		?v???????i?Z?????????A???????????????????????s?????~?????j
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
	Cf.CreL	= 1;	// ???[?J???N???W?b?g???P
	Cf.CreR = 0;	// ?????[?g?N???W?b?g???O
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
**	???f?v??
**
**	?????l
**		IrDAR_INVALID		?????????????????A???????????X?e?[?g
**		IrDAR_NORESOURCE	?v???????i?o?b?t?@?t???j
**		IrDAR_ACCEPT		?v??????
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
		Cf.CreL			= 1;					// ???[?J???N???W?b?g???P
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
	ptr	= (byte *)data;					// ?f?[?^??????
	Cf.CreR		+= (*ptr ++) & 0x7f;	// ?s?s?o???N???W?b?g??????????
	if(Cf.CreL == 0){	// ???[?J???N???W?b?g???O????????????
		return;
	}
	if(size == 1)	// ?f?[?^???X?t???[?f?[?^
		return;
	Cf.CreL--;		// ???[?J???N???W?b?g????????
	if( size < 3)	// ???????????????p?P?b?g??????????
		return ;
	len	= size - 2;						// size - TTPCREDIT - FLAG
	flg			= *ptr ++;				// ?t???O??????????
	pn			= flg & IrMCPNMASK;		// PN ?t?B?[???h??????

	if(flg & IrMCDATA){					// ?{?C?X???????????f?t?H???g???`?c?o?b?l
//		Cf.RealTime	= TRUE;				// ?{?C?X?????????????A???^?C????????????
		blen	= Cf.AudLen;			// ???????f?[?^??
		type	= IrMC_TYPE_STD;
	}else{
//		Cf.RealTime	= FALSE;			// ?{?C?X?????????????A???^?C????????????
		blen		= 0;
	}
	for(;;){
		if(pn == 0)
			break;
		pi = *ptr++;					// ?p?????[?^?h?c
		pl = *ptr++;					// ?p?????[?^??
		if(pi == IrMCPIAUDLEN){			// ?{?C?X?f?[?^???????w???????????A?`?c?o?b?l???????????f????
			blen = *ptr;				// ?{?C?X?f?[?^????????????
			type = IrMC_TYPE_OTHER;		// ?X?^???_?[?h???????????f????
		}
		ptr += pl;		// ?????p?????[?^??????????
		len -= pl + 2;	// ?p?????[?^???X?L?b?v????
		pn--;			// ?p?????[?^??????????????
	}

	if(blen){							// ?{?C?X?f?[?^?????????AIndicate ????
		if( Cf.CallBack.AudioInd )
			(*Cf.CallBack.AudioInd)(type,ptr,blen);
		ptr += blen;	// ?{?C?X???|?C???^?[???i????
		len -= blen;	// ???????{?C?X????????
	}

	if(len > 0){				// ?c?????A?R?[???R???g???[??
		if(flg & IrMCCTRLNS)
			type = IrMC_TYPE_OTHER;
		else
			type = IrMC_TYPE_STD;
		if( Cf.CallBack.ControlInd )
			(*Cf.CallBack.ControlInd)(type,ptr,len);
		if((blen == 0) && Cf.RealTime){	// ???A???^?C???????R???g???[????????????????????
			SendData(0);				// ???[?J???N???W?b?g????????????
		}
	}
}

static int	SendData( int dlen )
{
register	byte	*dp,*que,*data,flag;
byte		cre;
register	int		no,len,olen,cmax;
register	int		enq,deq,ret;
	if(Cf.CreL == 0){	// ???????N???W?b?g???O???????[????
		cre	= 1;
	}else{
		cre	= 0;
	}
	if(Cf.CreR == 0){	// ?????????W?b?g???O???????M????????
		if(cre){		// ?f?[?^???X?t???[?f?[?^?????M
			if(IrLMP_DataReq(Cf.lsap,&cre,1) == IrDAR_ACCEPT){
				Cf.CreL += cre;
			}
		}
		return	IrDAR_BUSYWAIT;
	}
	data = Cf.Data;
	data[IrMCTTPPOS] = cre;
	if(dlen){
		// ?????A?{?C?X?f?[?^???????????R???g???[?????U????
		len	= IrMCTTPLEN + IrMCFLAGLEN + dlen;
		cmax	= IrMCCTRLLEN;
	}else{
		// ???????O???A?????f?[?^???p??????
		len	= IrMCTTPLEN + IrMCFLAGLEN;
		cmax	= IrMCPACKETLEN;
	}
	dp	= data + len;
	enq	= Cf.CtrlTxEnq;
	deq = Cf.CtrlTxDeq;
	if(enq != deq){			// ?W???R???g???[????????????
		que = Cf.CtrlTxQue;
		for(no = 0;no < cmax;no++){		// ?R?[???R???g???[???f?[?^???R?s?[????
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
	if(enq != deq){			// ???W???R???g???[????????????
		que = Cf.OCtrlTxQue;
		// ?R?[???R???g???[???f?[?^???R?s?[????
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
	if(dlen){	// ?{?C?X?f?[?^?P??
		ret = IrLMP_DataReq(Cf.lsap,data,len);
		if(ret == IrDAR_ACCEPT){
			Cf.CreL	+= cre;		// ???????N???W?b?g????????
			Cf.CreR	--;			// ???????N???W?b?g????????
		}
	}else{
		if(cre){	// ?N???W?b?g????????
			// ?f?[?^???X?t???[?f?[?^?????M
			ret = IrLMP_DataReq(Cf.lsap,&cre,1);
			if(ret == IrDAR_ACCEPT){
				Cf.CreL	+= cre;	// ???????N???W?b?g????????
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
		// ???W???{?C?X?f?[?^
		if(size > IrMCPCMLEN)	// ???????A?W?O?o?C?g????
			size	= IrMCPCMLEN;
		*dp++ = IrMCDATA | 1;	// voice flag + (PN = 1)
		*dp++ = IrMCPIAUDLEN;	// PI = AUDIO LENGTH
		*dp++ = 1;				// PL = 1
		*dp++ = size;			// PV = data size
		IrDA_memcpy(dp ,Aud,size);
		size += 3;
	}else{
		// ?W???{?C?X?f?[?^
		*dp++ = IrMCDATA;		// voice flag
//		size  = IrMCPCMLEN;		// ???????A?W?O?o?C?g????
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
			// ???W???R???g???[??
			deq = Cf.OCtrlTxDeq;
			enq	= Cf.OCtrlTxEnq;
			que = Cf.OCtrlTxQue;
		}else{
			// ?W???R???g???[??
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

int		IrDAAPI	IrMC_QueryTxFree( int type )								/* ???M?L???[????????????????	*/
{
int		enq,deq,len;
	if(!(Cf.Mode & MSM_Connected))
		return( IrDAR_INVALID ) ;
	if(type){
			// ???W???R???g???[??
			deq = Cf.OCtrlTxDeq;
			enq	= Cf.OCtrlTxEnq;
	}else{
			// ?W???R???g???[??
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
