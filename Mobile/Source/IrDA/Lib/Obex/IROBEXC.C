/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       Obex.c                                       */
/*             Created:    7/28/96                                      */
/*             Modify:     10/28/97                                     */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*                                                                      */
/************************************************************************/

/* $Header$ */

#include	<irdaconf.h>

#if defined(USE_MONITOR)
#undef	OBX_DEBUG
#endif

#include	<ctype.h>
#include	<string.h>

//#include	<conio.h>
//#include	<irbios.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	<irsir.h>
#include	<okmisc.h>
#include	<irlap.h>
#include	<irlmp.h>
#include	<irobex.h>
#include	<timer.h>
//#include	<irbios.h>
#include	<console.h>
#include	"obexlib.h"
#include	"filesys.h"
#include	"irdauty.h"

#define	OBX_DSCV_TIME	3000		/* ディスカバリ間隔				*/
#define	OBX_DSCV_MAX	10			/* ディスカバリ回数				*/
#define	OBX_CONEC_TIME	10000		/* 接続要求からの待ち時間		*/
#define	OBX_DISC_WAIT1	2000		/* IrOBEX切断までの待ち時間		*/
#define	OBX_DISC_WAIT2	2000		/* IrLAP 切断までの待ち時間		*/
#define	OBX_BUFF_SIZE	4096		/* OBX Buffer size				*/

static	int			OBX_Debug_Mode;		/*	for Debug 					*/

static	const char	OBX_NickName[]	= "DeepCore/IrOBEX";
static	const char	OBX_LongName[]	= "(c) 1996 Okaya Systemware";

static	const char	OBX_Class[]		= "OBEX";
static	const char	OBX_Attribute[] = "IrDA:TinyTP:LsapSel";

volatile char		OBX_Status;			/* モデムの状態遷移			*/
volatile char		OBX_ConnectConf;	/* connect conferm			*/
volatile char		OBX_IrLAPConnect;	/* IrLAPのコネクト状況		*/

static	int			OBX_Slsap;
static	int			OBX_Dlsap;
static	dword		OBX_Daddress;

static	byte		OBX_Primary;		/* プライマリ動作をする			*/
static	byte		OBX_ConnectPrimary;	/* プライマリ接続をした			*/
static	byte		OBX_Secondary;		/* セカンダリ動作をする			*/
static	dword		OBX_Interval;		/* ディスカバリ間隔の設定		*/

typedef	struct	t_FLAGS{
	IrDABOOL		AcceptResponse ;
	IrDABOOL		Connected ;
}	FLAGS ;

static	FLAGS		flags ;

typedef	struct	{
	char	*cmd ;
	void	(* const funcp)( int ac, const char *av[] ) ;
}	CMDTAB ;


static	volatile	int			rxRespCode ;
static	volatile	byte		rxHeader[256] ;	/*	256*/
static	volatile	int			rxHeaderSize ;
static	volatile	byte		rxBody[256] ;	/*	2048	*/
static	volatile	int			rxBodySize ;

static	char					fBuf[256] ;	/*	2048	*/


/*********************************************************************

	OBX IrDA/OBEX Crient Entity

**********************************************************************/

/*
**		コールバック関数の先行定義
*/

static	void	IrDACB OBX_CBDisconnectReq( int lsap, int status, const void IrDAFAR *header, int headersz );
static	void	IrDACB OBX_CBConnectRsp( int lsap, int status, const void IrDAFAR *header, int headersz );
static	void	IrDACB OBX_CBResponse( int lsap, int resp, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz );

static	void	IrDACB	IrLAP_ConnectPrv	( void ){OBX_IrLAPConnect	= TRUE;		}
static	void	IrDACB	IrLAP_DisconnectPrv	( void ){OBX_IrLAPConnect	= FALSE;	}

void	OBX_Install( void )
{
OBEXCALLBACK	ocb ;

#ifdef	OBX_DEBUG
	cprintf("Start Initialize\n");
#endif
	/* Internal Value Initialize	*/
	OBX_IrLAPConnect	= FALSE;
	OBX_Status			= FALSE;

	IrDA_memset( &ocb, 0, sizeof( OBEXCALLBACK )) ;
	ocb.DisconnectReq	= OBX_CBDisconnectReq ;
	ocb.Response		= OBX_CBResponse ;
	ocb.ConnectRsp		= OBX_CBConnectRsp ;

	OBX_Slsap = IrOBEX_RequestLSAP();
	IrOBEX_Open();
	IrOBEX_BindLSAP( OBX_Slsap, &ocb );

//	IrAPP_SetIASClass((char *)OBX_Class,(char *) OBX_Attribute, OBX_Slsap );
//	IrLMP_SetServiceHint(IrLMP_SH_IrOBEX);
}

void	OBX_Remove( void )
{
#ifdef	OBX_DEBUG
	cprintf("Uninitialize\n");
#endif
	IrOBEX_UnbindLSAP( OBX_Slsap ) ;
	IrOBEX_Close();
	OBX_Status	= FALSE;
}


/*
**		接続中か問いあわせる
*/
word OBX_isConnected( void )
{
	return( OBX_Status );
}

/******************************************************************************/

static	void ConnectProc(void)
{
	OBX_Status		= TRUE;
}

static void DisconnectProc(void)
{
	OBX_ConnectPrimary	= FALSE;
	OBX_Status			= FALSE;
}

static	void	IrDACB	OBX_CBConnectRsp( int lsap, int status, const void IrDAFAR *header, int headersz )
{
	rxRespCode				= status ;

	if( headersz > sizeof( rxHeader ))
		headersz	= sizeof( rxHeader ) ;
	IrDA_memcpy( (void *)rxHeader, (const void *)header, headersz ) ;
	rxHeaderSize	= headersz ;

	if( rxRespCode == 0xA0 ) {				/*Success, FInal bit set	*/
		flags.Connected		= TRUE ;
		OBX_ConnectPrimary	= TRUE;
		ConnectProc();
	}
	OBX_ConnectConf = TRUE;
}

static	void	IrDACB	OBX_CBDisconnectReq( int lsap, int status, const void IrDAFAR *header, int headersz )
{
	flags.Connected		= FALSE ;
	DisconnectProc();
}

static	void	IrDACB	OBX_CBResponse( int lsap, int resp, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	rxRespCode		= resp ;

	if( headersz > sizeof( rxHeader ))
		headersz	= sizeof( rxHeader ) ;
	IrDA_memcpy( (void *)rxHeader, (const void *)header, headersz ) ;
	rxHeaderSize	= headersz ;

	if( bodysz > sizeof( rxBody ))
		bodysz	= sizeof( rxBody ) ;
	IrDA_memcpy( (void *)rxBody, (const void *)body, bodysz ) ;
	rxBodySize	= bodysz ;

	flags.AcceptResponse		= TRUE ;
}

/**********************************************************************/
/*
**		接続要求処理
*/

int	OBX_ConnectReq(void)
{
int		ret;
dword	timer;
#ifdef	OBX_DEBUG
	cprintf("IrOBEX ConnectReq...\n");
#endif
	OBX_ConnectConf = FALSE;
	for(;;){
		ret		= IrOBEX_ConnectReq(OBX_Slsap,OBX_Daddress,OBX_Dlsap, 0,0);
		if(ret == IrDAR_ACCEPT)
			break;
		if(ret == IrDAR_BUSYWAIT)
			continue;
		if(ret == IrDAR_NORESOURCE)
			continue;
#ifdef	OBX_DEBUG
		cprintf("Error %d\n",ret);
#endif
		return	TRUE;
	}
	
	SetTimer( &timer, OBX_CONEC_TIME ) ;
	while(GetTimer( timer )){
		if(OBX_ConnectConf){
#ifdef	OBX_DEBUG
			cprintf("\n Conf\n");
#endif
			dspHeader((const byte*)rxHeader, rxHeaderSize ) ;
			goto end;
		}
	}
#ifdef	OBX_DEBUG
	cprintf("Time Out\n");
#endif
end:
	if(	OBX_Status ){
#ifdef	OBX_DEBUG
			cprintf("Connected\n");
#endif
			return	FALSE;
	}
	return	TRUE;
}

/*
**		切断要求発行
*/
int	OBX_DisconnectReq( void )
{
dword	timer;
int		ret;
#ifdef	OBX_DEBUG
	cprintf("Disconnect..");
#endif
	SetTimer(&timer,OBX_DISC_WAIT1);
	while(GetTimer(timer)){
		ret = IrOBEX_DisconnectReq( OBX_Slsap, 0, 0 );
		if(ret == IrDAR_ACCEPT)
			goto next;
	}
#ifdef	OBX_DEBUG
	cprintf("Time Out\n");
#endif
	DisconnectProc();
	return	TRUE;
next:
#ifdef	OBX_DEBUG
	cprintf("OK\n");
#endif
	SetTimer(&timer,OBX_DISC_WAIT2);
	while(GetTimer(timer))
		;
	DisconnectProc();
	return FALSE;
}

/********************************************


*****************************************/

static	int		waitResponse( void )
{
dword	timer ;

	for( SetTimer( &timer, 10000L ) ; GetTimer( timer ) != 0L ; ) {
		if( !flags.AcceptResponse )
			continue ;

#ifdef	OBX_DEBUG
		cprintf( "waitResponse(): accept response, response code is %s(0x%02X)\n", obexRespCode( rxRespCode ), rxRespCode ) ;
		cprintf( "waitResponse(): header size is %d, body size is %d\n", rxHeaderSize, rxBodySize ) ;
#endif
		dspHeader((const byte*)rxHeader, rxHeaderSize ) ;

		return( rxRespCode ) ;
	}

#ifdef	OBX_DEBUG
	cprintf( "waitResponse(): time out\n" ) ;
#endif
	return( -1 ) ;
}


static	int	setName( char *des, int bufsz, const char *name )
{
int		sz ;
int		nmsz ;

	sz		= 0 ;

	/* 本当は日本語ファイル名だったら Unicode に変換しなきゃいけないんだが... */

	nmsz		= 3 + (( strlen( name ) + 1 ) * 2 );

	des[sz++]	= 0x01 ;					/*HI for Name					*/
	des[sz++]	= ( nmsz >> 8 ) & 0xFF ;
	des[sz++]	= nmsz & 0xFF ;
	for( ; *name ; name ++ ) {
		des[sz++]	= 0 ;
		des[sz++]	= *name ;
	}
	des[sz++]	= 0 ;
	des[sz++]	= 0 ;
	return( sz ) ;
}


static	int	setSize( char *des, int bufsz, ulong size )
{
int		sz ;

	sz			= 0 ;
	des[sz++]	= 0xC3 ;
	sz			+= MiscSetBigEndian32( &des[sz], size ) ;
	return( sz ) ;
}

static	int	mkGetReqHeader( char *des, int bufsz, const char *name )
{
	int		sz ;

	sz		= 0 ;
	sz		+= setName( &des[sz], bufsz, name ) ;

	return( sz ) ;
}

static	int	mkPutReqHeader( char *des, int bufsz, const char *name, ulong size )
{
	int		sz ;

	sz		= 0 ;
	sz		+= setName( &des[sz], bufsz - sz, name ) ;
	sz		+= setSize( &des[sz], bufsz - sz, size ) ;

	return( sz ) ;
}

static	void	do_abort( int ac, const char *av[] )
{
	int		ret ;

#ifdef	OBX_DEBUG
	cprintf( "abort\n" ) ;
#endif
	if(( ret = IrOBEX_AbortReq( OBX_Slsap, 0, 0 )) != IrDAR_ACCEPT ) {
#ifdef	OBX_DEBUG
		cprintf( "do_abort::IrOBEX_AbortReq()", ret ) ;
#endif
		return ;
	}

	waitResponse() ;
}

static	void	do_get( char *filename )
{
FILE	*fp ;
int		ret ;
char	opthdr[256] ;
int		opthdrsz ;
int		wlen ;
char	*argc[2] ;
int		rc ;

#ifdef	OBX_DEBUG
	cprintf( "get %s \n",filename);
#endif
	if(( fp = fopen( filename, "wb" )) == NULL ) {
#ifdef	OBX_DEBUG
		cprintf( "do_get(): write object\n" ) ;
#endif
		return ;
	}
	opthdrsz	= mkGetReqHeader( opthdr, sizeof( opthdr ), filename ) ;
	for(;;) {
#ifdef	OBX_DEBUG
		cprintf( "do_get(): optional header size is %d bytes\n", opthdrsz ) ;
#endif
		flags.AcceptResponse	= FALSE ;
		if(( ret = IrOBEX_GetReq( OBX_Slsap, opthdr, opthdrsz )) != IrDAR_ACCEPT ) {
#ifdef	OBX_DEBUG
			cprintf( "do_get::IrOBEX_PutReq() %d\n", ret ) ;
#endif
			fclose( fp ) ;
			return ;
		}

		switch(( rc = waitResponse()) & 0x7F ) {
			case	0x10 :
			case	0x20 :
				break ;
			default :
#ifdef	OBX_DEBUG
				cprintf( "do_get(): received unknown response code(0x%02X)\n", rc ) ;
#endif
				fclose( fp ) ;
				return ;
		}
#ifdef	OBX_DEBUG
		cprintf( "do_get(): final:%s header:%d bytes, body:%d bytes\n", (rxRespCode&0x80)?"TRUE":"FALSE", rxHeaderSize, rxBodySize ) ;
#endif
		if(( wlen = fwrite((void *)rxBody, 1, rxBodySize, fp )) != rxBodySize ) {
			if( wlen < 0 )
#ifdef	OBX_DEBUG
				cprintf( "do_get(): fwrite\n" ) ;
#endif
			else
#ifdef	OBX_DEBUG
				cprintf( "do_get(): may be disk full\n" ) ;
#endif
			do_abort( 1, (const char **)argc ) ;
			fclose( fp ) ;
			return ;
		}

		if( rxRespCode & OBEXRC_OK ) {
			fclose( fp ) ;
			break ;
		}

		opthdrsz	= 0 ;
	}
}

static	void	do_put( char *filename)
{
FILE	*fp ;
ulong	filesz, fileptr ;
int		bodysz ;
char	opthdr[256] ;
int		opthdrsz ;
int		len ;
IrDABOOL	fb ;
int		ret ;
dword	timer ;
int		rc ;

#ifdef	OBX_DEBUG
	cprintf( "put %s\n",filename);
#endif
	if(( fp = fopen(filename, "rb" )) == NULL ) {
#ifdef	OBX_DEBUG
		cprintf( "do_put(): read object" ) ;
#endif
		return ;
	}
	filesz	= fsize(fp);
#ifdef	OBX_DEBUG
	cprintf( "file size = %lu\n",filesz);
#endif
	fileptr		= 0 ;

	opthdrsz	= mkPutReqHeader( opthdr, sizeof( opthdr ), filename, filesz ) ;

	for(;;) {
#ifdef	OBX_DEBUG
		cprintf( "do_put(): optional header size is %d bytes\n", opthdrsz ) ;
#endif
		if(( bodysz = IrOBEX_GetMaxTxBodySize( OBX_Slsap, opthdrsz )) < 0 ) {
#ifdef	OBX_DEBUG
			cprintf( "do_put(): IrOBEX_GetMaxTxBodySize()%d\n", bodysz ) ;
#endif
			fclose( fp ) ;
			return ;
		}
#ifdef	OBX_DEBUG
		cprintf( "do_put(): max body size is 0x%04X(%d)\n", bodysz, bodysz ) ;
#endif
		fb		= FALSE ;							/*final bit false		*/
		len		= 0 ;
		if( bodysz > 0 && opthdrsz == 0 ) {
			if( bodysz > sizeof( fBuf ))			/*adjust read size		*/
				bodysz	= sizeof( fBuf ) ;
			if(( len = fread( fBuf, 1, bodysz, fp )) < 0 ) {
#ifdef	OBX_DEBUG
				cprintf( "do_put(): read object" ) ;
#endif
				fclose( fp ) ;
				return ;
			}
			fileptr		+= len ;
#ifdef	OBX_DEBUG
			cprintf( "do_put(): %lu/%lu\n", fileptr, filesz ) ;
#endif
			if( len < bodysz )
				fb	= TRUE ;
		}

		flags.AcceptResponse	= FALSE ;
#ifdef	OBX_DEBUG
		cprintf( "do_put(): final:%s header:%d bytes, body:0x%08lX, %d bytes\n", fb?"TRUE":"FALSE",opthdrsz,fBuf, len ) ;
#endif
		for(;;) {
			if(( ret = IrOBEX_PutReq( OBX_Slsap, fb, opthdr, opthdrsz, fBuf, len )) == IrDAR_ACCEPT )
				break ;
#ifdef	OBX_DEBUG
			cprintf( "do_put(): IrOBEX_PutReq()%d\n", ret ) ;
#endif
			if( ret != IrDAR_BUSYWAIT ) {
				fclose( fp ) ;
				return ;
			}

			for( SetTimer( &timer, 100L ) ; GetTimer( timer ) != 0L ; ) ;
		}

		switch(( rc = waitResponse()) & 0x7F ) {
			case	0x10 :
			case	0x20 :
				break ;
			default :
#ifdef	OBX_DEBUG
				cprintf( "do_put(): received unknown response code(0x%02X)\n", rc ) ;
#endif
				fclose( fp ) ;
				return ;
		}

		opthdrsz	= 0 ;

		if( fb ) {
			fclose( fp ) ;
			break ;
		}
	}
}

#if 0
static	void	do_setpath( char *pathname)
{
	int		ret ;
	char	opthdr[256] ;
	int		opthdrsz ;

#ifdef	OBX_DEBUG
	cprintf( "setpath\n" ) ;
	cprintf( "do_setpath(): change remote directory to \"%s\"\n", pathname ) ;
#endif
	opthdrsz	= setName( opthdr, sizeof( opthdr ), pathname ) ;
	if(( ret = IrOBEX_SetPathReq( OBX_Slsap, 0, opthdr, opthdrsz )) != IrDAR_ACCEPT ) {
#ifdef	OBX_DEBUG
		cprintf( "do_setpath::IrOBEX_SetPathReq() %d\n", ret ) ;
#endif
		return ;
	}
	waitResponse() ;
}
#endif

/******************************************************************************/

static	int	OBX_PrimaryProc(void)
{
int		ret,no,seq;
dword	timer;

	for(seq = 0;seq < OBX_DSCV_MAX;seq++){
		SetTimer(&timer,OBX_DSCV_TIME);
		while(GetTimer(timer))
			;
#ifdef	OBX_DEBUG
		cprintf("Discovering..\n");
#endif
		ret = IrAPP_DiscoveryDevice();
		if(ret){
#ifdef	OBX_DEBUG
			cputs("(Device Not Found)\n");
#endif
			continue;
		}
		IrAPP_DisplayDeviceName(ret);
#ifdef	OBX_DEBUG
		cprintf("Check IrOBEX Service Hint\n");
#endif
		no = 0;
		no = IrAPP_FoundServiceHint(IrLMP_SH_IrOBEX);
		if(no < 0){
#ifdef	OBX_DEBUG
			cprintf("Not Found\n");
#endif
			continue;
		}
#ifdef	OBX_DEBUG
		cprintf("Check IAS (IrOBEX)\n");
#endif
		IrAPP_Pause(100);
		if(IrAPP_QueryLSAP( no,(char *)OBX_Class,(char *)OBX_Attribute,&OBX_Daddress,&OBX_Dlsap)){
#ifdef	OBX_DEBUG
		cprintf("Not Found LSAP\n",OBX_Dlsap);
#endif
			IrAPP_Pause(2000);
			continue;
		}
#ifdef	OBX_DEBUG
		cprintf("Found LSAP (%d)\n",OBX_Dlsap);
		cprintf("ConnectReq\n");
#endif
		if(!OBX_ConnectReq()){		/*初期接続時*/
#ifdef	OBX_DEBUG
			cprintf("Connected Now!\n");
#endif
			return FALSE;
		}
	}
	return	TRUE;
}

/***********************************************************************

	SCEP_BindOBX
	
***********************************************************************/

static	void	OBX_Mode(int mode)
{
	switch(mode){
		case	1:
			OBX_Primary 	= TRUE;
			OBX_Secondary 	= TRUE;
			break;
		case	2:
			OBX_Primary 	= FALSE;
			OBX_Secondary 	= TRUE;
			break;
		default:
			OBX_Primary 	= FALSE;
			OBX_Secondary 	= FALSE;
			break;
	}
}

void	OBX_Enter(void)
{
	OBX_Interval	= OBX_DSCV_TIME;
	OBX_Mode(0);
	IrAPP_OpenStack();
	IrLAP_SetConnectPrv( IrLAP_ConnectPrv ) ;
	IrLAP_SetDisconnectPrv( IrLAP_DisconnectPrv ) ;
	IrAPP_SetDeviceName(OBX_NickName,OBX_LongName );
	OBX_Install();
	OBX_ConnectPrimary = FALSE;
}

static	void	OBX_Exit(void)
{
	if(OBX_isConnected())
		OBX_DisconnectReq();
	OBX_Remove();
	IrAPP_CloseStack();
}

void	OBX_Dump(byte *dp,int len)
{
int	c;
#ifdef	OBX_DEBUG
	cprintf("Recv(%dbytes):[",len);
#endif
	while(len--){
		c = *dp++;
		if((' ' <= c) && ( c < 0x7f)){
#ifdef	OBX_DEBUG
			cprintf("%02x(%c).",c,c);
#endif
		}else{
#ifdef	OBX_DEBUG
			cprintf("%02x.",c);
#endif
		}
	}
#ifdef	OBX_DEBUG
	cprintf("]\n");
#endif
}

static	int old_con;

#if 0
static void OBX_DispStat(void)
{
int	now;
	now = OBX_isConnected();
	if(now == old_con)
		return;
	old_con = now;
	if(now){
#ifdef	OBX_DEBUG
		cprintf("OBX:(Connect)\n");
#endif
	}else{
#ifdef	OBX_DEBUG
		cprintf("OBX:(Disonnect)\n");
#endif
	}
}
#endif

static	void do_obex(int cmd,char *name)
{
	if(OBX_PrimaryProc())
		return;
	switch(cmd){
		case	0:
			do_put(name);
			break;
		case	1:
			do_get(name);
			break;
	}
	OBX_DisconnectReq();
}

static	char fname[100];

void OBEX_Client()
{
char cmd[20];
#ifdef	OBX_DEBUG
	cprintf("***************************************\n");
	cprintf(" IrOBEX Client Program (DeepCore v1.7)\n");
	cprintf(" Date: %s Time : %s\n",__DATE__,__TIME__);
	cprintf("***************************************\n");
#endif
	OBX_Debug_Mode	=	0;	/*	PUT	*/
#ifdef	OBX_DEBUG
	for(;;){
		cprintf("1..... put(1)\n");
		cprintf("2..... get(2)\n");
		cprintf("3.....  Go(G)\n");
		cputs("OBEX>");
		if(cgets(cmd,5) == -1)
			break;
		switch(*cmd){
			case	'1':
				OBX_Debug_Mode	=	0;
				break;
			case	'2':
				OBX_Debug_Mode	=	1;
				break;
			case	'G':
			case	'g':
				goto	start;
				break;
		}
	}
#endif
start:
	OBX_Enter();
	old_con = !OBX_isConnected();
	OBX_Mode(2);
	
	for(;;){
		do_obex(OBX_Debug_Mode,"Sample");
		OBX_Debug_Mode	=	!OBX_Debug_Mode;
	}
#if 0
	for(;;){
#ifdef	OBX_DEBUG
		cprintf("put(1),get(2),end(9)\n");
#endif
		cputs("OBEX>");
		if(cgets(cmd,5) == -1)
			break;
		switch(*cmd){
			case	'1':
				cputs("Put:FileName?>");
				if(cgets(fname,50) < 1)
					continue;
				do_obex(0,fname);
				break;
			case	'2':
				cputs("Get:FileName?>");
				if(cgets(fname,50) < 1)
					continue;
				do_obex(1,fname);
				break;
			case	'9':
				return;
		}
	}
#endif
	OBX_Exit();
}
