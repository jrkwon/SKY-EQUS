/**********************************************************************************

	Obex Server Mode 

**********************************************************************************/

#include	<string.h>
#include	<stdio.h>

#include	"irsir.h"
#include	"irda.h"
#include	"irlmp.h"
#include	"irobex.h"
#include	"obexlib.h"
#include	"irdauty.h"
#include	"filesys.h"

#include	"console.h"

#define		NM_PUTIND		"obex.put"

const	int		debug  = 1;

typedef	volatile	struct	{
	int			connect ;
}	OBEXSFLAGS ;


typedef	volatile	struct	{
	int			accept ;
	FILE		*fp ;
}	OBEXPUTREQ ;


typedef	volatile	struct	{
	int			accept ;
	FILE		*fp ;
}	OBEXGETREQ ;


typedef	volatile	struct	{
	int			accept ;
}	OBEXSETPATHREQ ;

static	int				ObexLsap ;

static	OBEXSFLAGS			ObexSFlags ;
static	OBEXPUTREQ			putInd ;
static	OBEXGETREQ			getReq ;
static	OBEXSETPATHREQ		setPathReq ;

static	volatile	int		needConnectRsp ;
static				int		ObexClassId ;
static				int		ObexAttrId ;


static				IrDABOOL	rxFinalBit ;
static				byte	rxHeader[256] ;	/*	2048	*/
static				int		rxHeaderSize ;		/*	2048	*/
static				byte	rxBody[256] ;		/*	2048	*/
static				int		rxBodySize ;
static				byte	txBody[256] ;		/*	2048	*/


typedef	struct	t_FRQTAB {
	volatile int	*acceptp ;
	void			(*reqp)( void ) ;
	void			(*abortp)( void ) ;
}	REQTAB ;

static	void	do_putreq( void ) ;
static	void	do_putab( void ) ;
static	void	do_getreq( void ) ;
static	void	do_getab( void ) ;
static	void	do_setpathreq( void ) ;
static	void	do_setpathab( void ) ;

static	const	REQTAB	reqTab[]	= {
	{	&putInd.accept,		do_putreq,		do_putab		},
	{	&getReq.accept,		do_getreq,		do_getab		},
	{	&setPathReq.accept,	do_setpathreq,	do_setpathab	},
	{0,0,0}} ;


static	void				(*abortReqPtr)( void ) ;
static	volatile IrDABOOL	abortf ;

/*******************************************************************

	IrOBEX Call Back Routine

********************************************************************/


static	void	IrDACB	OBEX_CBConnectReq( int lsap, const void IrDAFAR *header, int headersz )
{
int		ret ;
	if((ret = IrOBEX_ConnectRsp( lsap, 0, 0 )) != IrDAR_ACCEPT)
		needConnectRsp		= TRUE ;
	else
		ObexSFlags.connect	= TRUE ;
}

static	void	IrDACB	OBEX_CBDisconnectReq( int lsap, int status, const void IrDAFAR *header, int headersz )
{
	ObexSFlags.connect		= FALSE ;
}

static	void	IrDACB	OBEX_CBPutReq( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz, const void IrDAFAR *body, int bodysz )
{
	rxFinalBit			= fb ;
	rxHeaderSize		= headersz ;
	if( headersz > 0 )
		IrDA_memcpy( rxHeader, header, rxHeaderSize ) ;
	rxBodySize			= bodysz ;
	if( bodysz )
		IrDA_memcpy( rxBody, body, rxBodySize ) ;

	putInd.accept		= TRUE ;
}

static	void	IrDACB	OBEX_CBGetReq( int lsap, IrDABOOL fb, const void IrDAFAR *header, int headersz )
{
	rxFinalBit			= fb ;
	rxHeaderSize		= headersz ;
	IrDA_memcpy( rxHeader, header, rxHeaderSize ) ;

	getReq.accept		= TRUE ;
}


static	void	IrDACB	OBEX_CBSetPathReq( int lsap, byte flag, const void IrDAFAR *header, int headersz )
{
	rxHeaderSize		= headersz ;
	IrDA_memcpy( rxHeader, header, rxHeaderSize ) ;
	setPathReq.accept	= TRUE ;
}


static	void	IrDACB	OBEX_CBAbortReq( int lsap, const void IrDAFAR *header, int headersz )
{
	rxFinalBit			= TRUE ;
	rxHeaderSize		= headersz ;
	IrDA_memcpy( rxHeader, header, rxHeaderSize ) ;

	abortf				= TRUE ;
}



/***************************************************************************

	Bind / Unbind

***************************************************************************/


static	IrDABOOL	BindLsap( void )
{
	OBEXCALLBACK	ocb ;
	int				ret ;
	IrAPP_OpenStack();
	IrOBEX_Open();

	ObexLsap	= IrOBEX_RequestLSAP() ;
	if( debug )	cprintf( "BindLsap::IrOBEX_RequestLSAP(): ObexLsap=%d\n", ObexLsap ) ;
	if( ObexLsap < 0 ) {
		return( TRUE ) ;
	}

	memset( &ocb, 0, sizeof( OBEXCALLBACK )) ;
	ocb.ConnectReq		= OBEX_CBConnectReq ;
	ocb.DisconnectReq	= OBEX_CBDisconnectReq ;
	ocb.PutReq			= OBEX_CBPutReq ;
	ocb.GetReq			= OBEX_CBGetReq ;
	ocb.SetPathReq		= OBEX_CBSetPathReq ;
	ocb.AbortReq		= OBEX_CBAbortReq ;
	if(( ret = IrOBEX_BindLSAP( ObexLsap, &ocb )) != IrDAR_ACCEPT ) {
		return( TRUE ) ;
	}
	if( debug )	cprintf( "BindLsap::IrOBEX_BindLSAP(): Success\n" ) ;

	IrLMP_SetServiceHint( IrLMP_SH_IrOBEX ) ;
	ObexClassId		= IrLMP_AddClass( "OBEX", 4 ) ;
	ObexAttrId		= IrLMP_AddAttribute( ObexClassId, "IrDA:TinyTP:LsapSel", 19 ) ;
	IrLMP_SetLSAPAttribute( ObexAttrId, ObexLsap ) ;

	return( FALSE ) ;
}


static	void	UnbindLsap( void )
{
	IrLMP_ResetServiceHint( IrLMP_SH_IrOBEX ) ;
	IrLMP_DeleteAttribute( ObexAttrId ) ;
	IrLMP_DeleteClass( ObexClassId ) ;

	if( ObexLsap >= 0 ) {
		IrOBEX_UnbindLSAP( ObexLsap ) ;
		if( debug )	cprintf( "BindLsap::IrOBEX_UnbindLSAP(): Success\n" ) ;
	}
	IrOBEX_Close();
	IrAPP_CloseStack();
}

/****************************************************************************************/

static	IrDABOOL	getNameOption( char *des, int dessz, byte *opt, word optsz )
{
long	size ;
word	namesz ,sz;

	*des	= 0 ;
	for( size = (long)optsz ; size > 0 ; ) {
		if( *opt != 0x01 ) {		/*not name options ?					*/
			sz	= obexGetHeaderSize( opt ) ;
			opt		+= sz ;
			size	-= sz ;
			continue ;
		}

		opt			++ ;
		namesz		= ((word)(*opt++)) << 8 ;
		namesz		+= *opt++ ;

		for( ; dessz > 1 && namesz > 0 && ( opt[0] || opt[1] ) ; ) {
			opt		++ ;
			*des++	= *opt++ ;
			namesz	-= 2 ;
			dessz	-- ;
		}

		*des	= 0 ;
		return( FALSE ) ;
	}

	return( TRUE ) ;
}



static	void	do_putab( void )
{
	cprintf( "put opration is aborted\n" ) ;
	if( putInd.fp ) {
		fclose( putInd.fp ) ;
		putInd.fp	= 0 ;
	}
	IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
}


static	void	do_getab( void )
{
	cprintf( "put opration is aborted\n" ) ;
	if( getReq.fp ) {
		fclose( getReq.fp ) ;
		getReq.fp	= 0 ;
	}
	IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
}


static	void	do_setpathreq( void )
{
	cprintf( "accept setpath request\n" ) ;
	cprintf( "  final bit: %s, header: %d bytes\n", (rxFinalBit)?"TRUE":"FALSE", rxHeaderSize ) ;

	dspHeader( rxHeader, rxHeaderSize ) ;
	IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
}


static	void	do_setpathab( void )
{
	cprintf( "setpath opration is aborted\n" ) ;
	IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
}


static	void	rcvignabort( void )
{
	cprintf( "RECEIVED ABORT, BUT CURRENT OPRATION IS NOTHING\n" ) ;
	IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
}

static	void	do_putreq( void )
{
	int		len ;
	char	fName[256] ;

	cprintf( "accept put request\n" ) ;
	cprintf( "  final bit: %s, header: %d bytes, body: %d bytes\n", (rxFinalBit)?"TRUE":"FALSE", rxHeaderSize, rxBodySize ) ;

	dspHeader( rxHeader, rxHeaderSize ) ;

	if( !putInd.fp ) {
		if( getNameOption( fName, sizeof( fName ), rxHeader, rxHeaderSize ))
			strcpy( fName, NM_PUTIND ) ;

		cprintf( "do_putreq(): open \"%s\"\n", fName ) ;
		if(( putInd.fp = fopen( fName, "wb" )) == NULL ) {
			perror( "do_putreq(): fopen()" ) ;
			IrOBEX_SendResponse( ObexLsap, 0xC6, 0, 0 ) ;
			return ;
		}
	}

	if( rxBodySize > 0 ) {
		if(( len = fwrite( rxBody, 1, rxBodySize, putInd.fp )) != rxBodySize ) {
			if( len < 0 )
				perror( "do_putreq(): fwrite()" ) ;
			else
				cprintf( "do_putreq(): disk full %d/%d\n", len, rxBodySize ) ;
			return ;
		}
	}

	if( rxFinalBit ) {
		fclose( putInd.fp ) ;
		putInd.fp		= 0 ;
		cprintf( "do_putreq(): closed\n" ) ;
		IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
	} else {
		IrOBEX_SendResponse( ObexLsap, 0x90, 0, 0 ) ;
	}
}


static	void	do_getreq( void )
{
	char	fName[256] ;
	int		flen, txlen ;

	cprintf( "accept get request\n" ) ;
	cprintf( "  final bit: %s, header: %d bytes\n", (rxFinalBit)?"TRUE":"FALSE", rxHeaderSize ) ;

	dspHeader( rxHeader, rxHeaderSize ) ;

	if( !getReq.fp ) {
		if( getNameOption( fName, sizeof( fName ), rxHeader, rxHeaderSize ))
			strcpy( fName, NM_PUTIND ) ;

		cprintf( "do_getreq(): open \"%s\"\n", fName ) ;
		if(( getReq.fp = fopen( fName, "rb" )) == NULL ) {
			perror( "do_getreq(): fopen()" ) ;
			IrOBEX_SendResponse( ObexLsap, 0xC6, 0, 0 ) ;
			return ;
		}
	}

	/*送信可能な最大サイズを問い合わせ*/
	if(( txlen = IrOBEX_GetMaxTxBodySize( ObexLsap, 0 )) < 0 ) {
		cprintf( "do_getreq::IrOBEX_GetMaxTxBodySize() %d\n", txlen ) ;
		if( getReq.fp )
			fclose( getReq.fp ) ;
		return ;
	}
	if( txlen > sizeof( txBody ))
		txlen	= sizeof( txBody ) ;

//	if(( flen = fread( txBody, 1, txlen, getReq.fp )) != txlen ) {
	flen = fread( txBody, 1, txlen, getReq.fp );
	if(flen != txlen ) {
		if( flen < 0 ) {
			perror( "do_getreq(): fread" ) ;
			IrOBEX_SendResponse( ObexLsap, 0x46, 0, 0 ) ;
		}else if( flen == 0 ) {
			cprintf( "do_getreq(): send \"Success\" with final bit\n" ) ;
			IrOBEX_SendResponse( ObexLsap, 0xA0, 0, 0 ) ;
		}else{
			cprintf( "do_getreq(): send \"Continue\" with final bit and body(%d bytes)\n", flen ) ;
			IrOBEX_GetRsq( ObexLsap, 0xA0, TRUE, 0, 0, txBody, flen ) ;
		}
		fclose( getReq.fp ) ;
		getReq.fp	= NULL ;
		return ;
	}

	cprintf( "do_getreq(): send \"Continue\" with body(%d bytes)\n", flen ) ;
	IrOBEX_GetRsq( ObexLsap, 0x90, FALSE, 0, 0, txBody, flen ) ;
}


/**********************************************************************************


*********************************************************************************/


static	IrDABOOL	ObexServerInit( void )
{
	ObexLsap			= -1 ;
	ObexSFlags.connect	= FALSE ;
	needConnectRsp		= FALSE ;
	memset( (void *)&putInd, 0, sizeof( putInd )) ;
	memset( (void *)&getReq, 0, sizeof( getReq )) ;
	memset( (void *)&setPathReq, 0, sizeof( setPathReq )) ;
	abortf				= FALSE ;
	abortReqPtr			= rcvignabort ;

	if( BindLsap())
		return( TRUE ) ;

	return( FALSE ) ;
}


static	void	ObexServerTerm( void )
{
	UnbindLsap() ;
}


static	void	ObexServerProc( void )
{
	const 	char	wing[]	= "|/-/" ;
	int		wingp ;
	int		oconnect ;
	int		ret ;
	REQTAB	const *reqtabp ;
	dword	count;
	oconnect		= ObexSFlags.connect ;

	cprintf( "Start IrOBEX Server. Hit any key to terminate.\n" ) ;

	wingp	= 0 ;
	count	=	0;
	for(;;) {
		count++;
		if(cget() == 0x1b)
			break;
		if(count > 10000){
			count	=	0;
			cprintf( "%c\x08", wing[(wingp=(wingp+1)&0x03)] ) ;
		}
		if( oconnect != ObexSFlags.connect ) {
			if( ObexSFlags.connect ) {
				memset( (void *)&putInd, 0, sizeof( putInd )) ;
				cprintf( "connected\n" ) ;
			} else {
				if( putInd.fp ) {
					fclose( putInd.fp ) ;
					putInd.fp	= NULL ;
					getReq.fp	= NULL ;
				}
				cprintf( "disconnected\n" ) ;
			}
			oconnect	= ObexSFlags.connect ;
		}
		if( needConnectRsp ) {
			if( debug )	cprintf( "need connect response\n" ) ;
			if(( ret = IrOBEX_ConnectRsp( ObexLsap, 0, 0 )) == IrDAR_ACCEPT ) {
				cprintf( "ObexServerProc::IrOBEX_ConnectRsp()%d\n", ret ) ;
				needConnectRsp		= FALSE ;
				ObexSFlags.connect	= TRUE ;
			}
			if( debug )	cprintf( "send connect response\n" ) ;
		}

		for( reqtabp	=	&reqTab[0] ; reqtabp->acceptp ; reqtabp++ ) {
			if( *reqtabp->acceptp ) {
				*reqtabp->acceptp	= FALSE ;
				abortReqPtr			= reqtabp->abortp ;
				(*reqtabp->reqp)() ;
				break ;
			}
		}

		if( abortf ) {
			abortf		= FALSE ;
			(*abortReqPtr)() ;
			abortReqPtr	= rcvignabort ;
		}
	}
}

void	OBEX_Server( void )
{
	if( ObexServerInit())
		return ;
	ObexServerProc() ;
	ObexServerTerm() ;
}
