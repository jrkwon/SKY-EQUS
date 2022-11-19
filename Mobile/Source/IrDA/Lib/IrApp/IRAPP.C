/********************************************************************

	IrAPP Apprication convinience Program

*********************************************************************/

#include	<string.h>
#include	<irdaconf.h>
#include	<irlap.h>
#include	<irlmp.h>
#include	<irapp.h>
#include	<timer.h>

#undef		APP_DEBUG

#ifdef	APP_DEBUG
#include	<console.h>
#endif

/*  -----  Definition  -----  */

#define		BUSYTIMER		100		/*	ビジー状態の待ち時間		*/
#define		BUSYRETRY		20		/*	ビジー状態のリトライ回数	*/

/********************************

	Auto Discovery

*********************************/

static	volatile	int	DiscBusy;
static	volatile	int	DiscCount;
static				int	DiscInterval;
static				int	DiscSlot;
static				int DiscRetry;
static				int	hDiscTimer;
static DISCOVERYLOG		*pDiscLog;
static				int	*pDiscResult;
static				int (*pDiscConfFunc)(int result);
static				void(*pDiscIndFunc)(int sw);

static	void RekickDiscovery(void);

static	void	IrDACB	CBDiscCnf( int status, uint logsz )
{
int	ret;
	*pDiscResult	= logsz;
	if(pDiscIndFunc){
		(*pDiscIndFunc)(FALSE);
	}
	if(pDiscConfFunc){					/*	コールバックするなら				*/
		if((*pDiscConfFunc)(logsz)){	/* もし、終了なら終わる					*/
			DiscBusy = FALSE;
			return;
		}
	}else{
		*pDiscResult	= logsz;
		DiscBusy = FALSE;
		return;
	}
	DiscRetry	= BUSYRETRY;
	if(DiscCount){						/* リトライするなら						*/
		if(DiscBusy){
			if(DiscCount != -1)			/* 無限にリトライしないなら				*/
				DiscCount--;			/* カウントを減らして					*/
			if(DiscInterval){			/* 時間待ちをする						*/
				hDiscTimer = StartTimer(DiscInterval,RekickDiscovery);
				if(hDiscTimer >= 0)
					return;
				/* タイマがないならエラー				*/
				ret	= -1;
			}else{						/* すぐにリトライ						*/
				RekickDiscovery();
				return;
			}
		}else{
			ret	= -1;
		}
	}else{
		ret	=-1;
	}
	DiscBusy = FALSE;
	if(pDiscConfFunc){
		(*pDiscConfFunc)(ret);
	}
}

static	void RekickDiscovery(void)
{
int	ret;
	hDiscTimer	= -1;
	if(DiscBusy){
		ret = IrLMP_DiscoveryReq( DiscSlot, pDiscLog, DiscSlot, CBDiscCnf );
		switch(ret){
		case	IrDAR_ACCEPT:
				if(pDiscIndFunc){
					(*pDiscIndFunc)(TRUE);
				}
				return;
		case	IrDAR_BUSYWAIT:	case	IrDAR_MEDIABUSY:	case	IrDAR_NORESOURCE:
				if(!DiscBusy){
					ret	= -1;
					break;
				}
				if(DiscRetry){
					hDiscTimer = StartTimer(DiscInterval,RekickDiscovery);
					if(hDiscTimer != -1)		/* タイマがないならエラー				*/
						return;
					ret	= -1;
				}
				break;
		default:
				break;
		}
	}else{
		ret	= -1;
	}
	DiscBusy = FALSE;
	if(pDiscConfFunc){
		(*pDiscConfFunc)(ret);
	}
}

int	IrAPP_GetAutoDiscoveryStat(void)
{
	return	DiscBusy;
}

int	IrAPP_AbortAutoDiscovery(int result)
{
#ifdef	APP_DEBUG
	cputs("[IrAPP]IrAPP_AbortAutoDiscovery\n");
#endif
	if(!DiscBusy)
		return	TRUE;
	if(hDiscTimer != -1){
		StopTimer(hDiscTimer);
		if(pDiscConfFunc){
			(*pDiscConfFunc)(result);
		}
	}
	DiscBusy	= FALSE;
	return	FALSE;
}

int	IrAPP_SetAutoDiscovery(AUTODISCOVERY *p)
{
#ifdef	APP_DEBUG
	cputs("[IrAPP]IrAPP_SetAutoDiscovery\n");
#endif
	if(DiscBusy)
		return	IrDAR_INUSE;

	DiscBusy		= TRUE;
	DiscRetry		= BUSYRETRY;
	DiscCount		= p->count - 1;
	DiscSlot		= p->slot;
	DiscInterval	= p->interval;
	pDiscConfFunc	= p->pConfFunc;
	pDiscIndFunc	= p->pIndFunc;
	pDiscLog		= p->pLog;
	pDiscResult		= p->pResult;
	*pDiscResult	= -1;
	DiscRetry		= BUSYRETRY;
	RekickDiscovery();

	return	IrDAR_ACCEPT;
}

/********************************

	Check Ias Entry

*********************************/

static	IASCALLBACK 	IasCb;

static	int				hIasTimer;
static	int				IasBusy;
static	int				IasRetry;

static	int				IasLsap;
static	dword			IasAddr;

static	const	char	*IasClass;
static	int				IasClassLen;
static	const	char	*IasAttribute;
static	int				IasAttributeLen;

static	void			*IasData;
static	int				*IasDataSize;
static	int				IasDataOrgSize;

static	IASVALUEBYCLASS	*IasList;
static	int				*IasListSize;
static	int				IasListOrgSize;

static	void			(*IasConf)(int result );


static	void	IrDACB	CBIasDiscInd(int lsap)
{
#ifdef	APP_DEBUG
//	cprintf("IAS:IasDiscInd\n");
	cputs("[CB]CBIasDiscInd\n");
#endif
	if(hIasTimer != -1){
		StopTimer(hIasTimer);
		hIasTimer	= -1;
	}
	IasBusy	= FALSE;
	if(IasConf)
		(*IasConf)(-1);
}

static	void	IrDACB	CBIasValCnf( int lsap, byte IrDAFAR *value, int size )
{
int	ret;
	ret = IrLMP_QueryReturnCode(value,size);
	if(ret != 0){
#ifdef	APP_DEBUG
//	cprintf("IAS:QC(%d)\n",ret);
	cputs("[REQ]IAS:QC\n");
#endif
	}else{
		if(size > IasDataOrgSize){
			size = IasDataOrgSize;
		}
		IrDA_memcpy( IasData, value, size ) ;
		*IasDataSize	= size;
#ifdef	APP_DEBUG
//	cprintf("IAS:L[%d]",IasListOrgSize);
	cputs("IAS:L");
#endif
		ret = IrLMP_QueryValueByClass((byte IrDAFAR *)IasData,size,IasList,IasListOrgSize);
#ifdef	APP_DEBUG
//	cprintf("IAS:QVC(%d)\n",ret);
	cputs("IAS:QVC\n");
#endif
		if(ret > 0){
			ret	= 0;
			*IasListSize = ret;
		}else{
			ret	= -1;
		}
	}
	IrLMP_CloseIAS( IasLsap );
	IasBusy	= FALSE;
#ifdef	APP_DEBUG
//	cprintf("IAS:VCf(%d,%d)\n",size,ret);
	cputs("IAS:VCf\n");
#endif
	if(IasConf)
		(*IasConf)(ret);
}

static	void IasRegetValue(void)
{
int	ret;
	hIasTimer	= -1;
	ret	= IrLMP_GetValueByClass( IasLsap, IasClass,IasClassLen,IasAttribute,IasAttributeLen);
	switch(ret){
	case	IrDAR_ACCEPT:
		IasLsap = ret;
#ifdef	APP_DEBUG
//		cprintf("IAS:VOK(%d)\n",IasLsap);
		cputs("IAS:VOK\n");
#endif
		return;

	case	IrDAR_BUSYWAIT:	case	IrDAR_MEDIABUSY:	case	IrDAR_NORESOURCE:
		if(IasRetry){
#ifdef	APP_DEBUG
//			cprintf("IAS:VWB(%d)\n",ret);
			cputs("IAS:VWB\n");
#endif
			hIasTimer = StartTimer(BUSYTIMER,IasRegetValue);
			if(hIasTimer != -1){
				break;
			}
			IasRetry--;
			return;
		}
		ret	= -1;
	default:
		break;
	}
	IrLMP_CloseIAS( IasLsap ) ;
#ifdef	APP_DEBUG
//	cprintf("IAS:VER(%d)\n",ret);
	cputs("IAS:VER\n");
#endif
	IasBusy	= FALSE;
	if(IasConf)
		(*IasConf)(ret);
}

static	void	IrDACB	CBIasConnCnf( int lsap, int status )
{
#ifdef	APP_DEBUG
//	cprintf("IAS:OC(%d)\n",status);
	cputs("[CB]IasConnCnf\n");
#endif
	IasRetry = BUSYRETRY;
	IasRegetValue();
}

static	void IasReopen(void)
{
int	ret;
	hIasTimer	= -1;
	ret = IrLMP_OpenIAS( IasAddr,&IasCb );
	if(ret >= 0){
		IasLsap = ret;
#ifdef	APP_DEBUG
//		cprintf("IAS:O(%d)\n",IasLsap);
		cputs("[]IAS:Open\n");
#endif
		return;
	}
	switch(ret){
	case	IrDAR_BUSYWAIT:
	case	IrDAR_MEDIABUSY:
	case	IrDAR_NORESOURCE:
		if(IasRetry){
			hIasTimer = StartTimer(BUSYTIMER,IasReopen);
			if(hIasTimer != -1)
				break;
			IasRetry--;
			return;
		}
	default:
		break;
	}
#ifdef	APP_DEBUG
//	cprintf("IAS:OER(%d)\n",ret);
	cputs("IAS:OER\n");
#endif
	IasBusy	= FALSE;
	if(IasConf)
		(*IasConf)(-1);
}

int	IrAPP_GetValueByClass(IASGETVALUE *p)
{
#ifdef	APP_DEBUG
	cputs("[IrAPP]IrAPP_GetValueByClass\n");
#endif
	if(IasBusy)
		return	IrDAR_INUSE;

	IasBusy				= TRUE;

	IasCb.ConnectCnf    = CBIasConnCnf ;
	IasCb.ValueCnf      = CBIasValCnf ;
	IasCb.DisconnectInd = CBIasDiscInd ;

	IasAddr				= p->Addr;					/* 接続先アドレス	*/
	IasClass			= p->pClass;				/* クラス名			*/
	IasClassLen			= strlen(IasClass);
	IasAttribute		= p->pAttribute;			/* アトリビュート名	*/
	IasAttributeLen		= strlen(IasAttribute);

	IasData				= p->pData;					/* データの場所		*/
	IasDataSize			= p->pDataSize;				/* データサイズ		*/
	IasDataOrgSize		= *IasDataSize;

	IasList				= p->pList;					/* リストの場所		*/
	IasListSize			= p->pListSize;				/* リストのサイズ	*/
	IasListOrgSize		= *IasListSize;

	IasConf				= p->pConfFunc;				/* 通知関数			*/
	IasRetry			= BUSYRETRY;
	IasReopen();
	return	IrDAR_ACCEPT;
}

/********************************

	Open Application

*********************************/

static	int		hAppTimer;
static	int		AppBusy;
static	int		AppRetry;

static	dword	AppDAddr;
static	int		AppDLsap;

static	int		(*AppOpen)(dword daddr,int dlsap);
static	void	(*AppConf)(int res);

static	void AppReopen(void)
{
int	ret;
	hAppTimer	= -1;
	ret = (*AppOpen)(AppDAddr,AppDLsap);
	switch(ret){
	case	IrDAR_ACCEPT:
#ifdef	APP_DEBUG
//		cprintf("APP:Open[OK]\n");
		cputs("APP:Open[OK]\n");
#endif
		AppBusy	= FALSE;
		if(AppConf)
			(*AppConf)(0);
		return;
	case	IrDAR_BUSYWAIT:	case	IrDAR_MEDIABUSY:	case	IrDAR_NORESOURCE:
		if(AppRetry){
			hAppTimer = StartTimer(BUSYTIMER,AppReopen);
			if(hAppTimer != -1)
				break;
			AppRetry--;
			return;
		}
	default:
		break;
	}
#ifdef	APP_DEBUG
//	cprintf("APP:Open[NG]\n");
	cputs("APP:Open[NG]\n");
#endif
	AppBusy	= FALSE;
	if(AppConf)
		(*AppConf)(ret);
}

int	IrAPP_AppOpenReq(	dword daddr,int dlsap,
						int		(*OpenFunc)(dword daddr,int dlsap),
						void	(*ConfFunc)(int res)
	)
{
	if(AppBusy)
		return	IrDAR_INUSE;
	AppBusy		= TRUE;
	AppRetry	= BUSYRETRY;
	AppDAddr	= daddr;
	AppDLsap	= dlsap;
	AppOpen		= OpenFunc;
	AppConf		= ConfFunc;
	AppReopen();
	return	IrDAR_ACCEPT;
}

void IrAPP_Initialize(void)
{
#ifdef	APP_DEBUG
	cputs("[IrAPP]IrAPP_Initialize\n");
#endif
	DiscBusy		= 0;
	DiscCount		= 0;
	DiscInterval	= 0;
	DiscSlot		= 0;
	DiscRetry		= 0;
	hDiscTimer		= 0;
	pDiscLog		= 0;
	pDiscResult		= 0;
	pDiscConfFunc	= 0;
	pDiscIndFunc	= 0;
	memset(&IasCb,0,sizeof(IasCb));
	hIasTimer		= -1;
	IasBusy			= 0;
	IasRetry		= 0;
	IasLsap			= 0;
	IasAddr			= 0;
	IasClass		= 0;
	IasClassLen		= 0;
	IasAttribute	= 0;
	IasAttributeLen	= 0;

	IasData			= 0;
	IasDataSize		= 0;
	IasDataOrgSize	= 0;

	IasList			= 0;
	IasListSize		= 0;
	IasListOrgSize	= 0;
	IasConf			= 0;

	hAppTimer		= -1;
	AppBusy			= 0;
	AppRetry		= 0;

	AppDAddr		= 0;
	AppDLsap		= 0;

	AppOpen			= 0;
	AppConf			= 0;
}

