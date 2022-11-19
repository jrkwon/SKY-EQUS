


#if !defined(__pmisc_h__)
#define	__pmisc_h__


#include	"string.h"
#include	"okmisc.h"
#include	"irsir.h"


#if 0
int			_memcmp( const void *des, const void *src, word len )
{
	const byte	*csrc, *cdes ;


	csrc	= (const byte*)src ;
	cdes	= (const byte*)des ;


	for( ; len ; len --, csrc++, cdes++ )
		if( *csrc != *cdes )
			return( 1 ) ;
	return( 0 ) ;
}
#endif


void		MiscInitQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR *buf, uint len )
{
	q->Data		= buf ;
	q->DataSize	= len ;
	MiscFlushQueue( q ) ;
}


void	MiscFlushQueue( MISCQUEUE IrDAFAR *q )
{
	q->ReadPtr	= 0 ;
	q->WritePtr	= 0 ;
}


uint	MiscGetUsedQueueSize( MISCQUEUE IrDAFAR *q )
{
	uint	wp, rp ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


	if( rp <= wp )
		return( wp - rp ) ;


	return( q->DataSize - rp + wp ) ;
}


uint	MiscGetFreeQueueSize( MISCQUEUE IrDAFAR *q )
{
	uint	wp, rp ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


	if( wp < rp )
		return( rp - wp - 1 ) ;


	if( wp + 1 == q->DataSize && rp == 0 )
		return( 0 ) ;


	return( q->DataSize - wp + rp - 1 ) ;
}


uint		MiscGetReadableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR *ptr )
{
	uint	wp, rp ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


	*ptr	= &q->Data[rp] ;


	if( rp <= wp )
		return( wp - rp ) ;


	return( q->DataSize - rp ) ;
}


uint		MiscGetWritableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR  *ptr )
{
	uint	wp, rp ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


	*ptr	= &q->Data[wp] ;


	if( wp < rp )
		return( rp - wp - 1 ) ;


	if( rp == 0 )
		return( q->DataSize - wp - 1 ) ;


	return( q->DataSize - wp ) ;
}


void		MiscSkipReadQueue( MISCQUEUE IrDAFAR *q, uint len )
{
#if	defined(DMSG2)
	_DPRINTF( DL_MISCINFO, "MISC::MiscSkipReadQueue(): s=%d, r=%d, w=%d, len=%d\n", q->DataSize, q->ReadPtr, q->WritePtr, len ) ;
#endif	
	if( q->ReadPtr >= q->DataSize - len )
		q->ReadPtr -= q->DataSize - len ;
	else
		q->ReadPtr	+= len ;
}


void		MiscSkipWriteQueue( MISCQUEUE IrDAFAR *q, uint len )
{
	if( q->WritePtr >= q->DataSize - len )
		q->WritePtr	-= q->DataSize - len ;
	else
		q->WritePtr	+= len ;
}


uint	MiscGetQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len )
{
	len			= MiscSenseQueue( des, q, len ) ;
	MiscSkipReadQueue( q, len ) ;
	return( len ) ;
}


uint		MiscSenseQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len )
{
	uint	dlen, tlen ;
	byte	IrDAFAR *desp ;
	uint	wp, rp ;


	desp	= (byte IrDAFAR *)des ;
	dlen	= MiscGetUsedQueueSize( q ) ;
	if( dlen == 0 )
		return( 0 ) ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


	if( len > dlen )
		len	= dlen ;


#if	defined(DMSG2)
	if( len )
		_DPRINTF( DL_MISCINFO, "MISC::MiscGetQueue(): s=%d, r=%d, w=%d, l=%d\n", q->DataSize, rp, wp, len ) ;
#endif	


	if( rp < wp ) {
		IrDA_memcpy( desp, &q->Data[rp], len ) ;
		return( len ) ;
	}


	tlen	= q->DataSize - rp ;
	if( len < tlen ) {
		IrDA_memcpy( desp, &q->Data[rp], len ) ;
		return( len ) ;
	}


	IrDA_memcpy( desp, &q->Data[rp], tlen ) ;
	dlen	= len - tlen ;
	if( dlen )
		IrDA_memcpy( &desp[tlen], &q->Data[0], dlen ) ;


	return( len ) ;
}


uint	MiscSetQueue( MISCQUEUE IrDAFAR *q, const void IrDAFAR *src, uint len )
{
	uint	dlen ;
	uint	tlen ;
	const byte	IrDAFAR *srcb ;
	uint	wp, rp ;


	if( len == 0 )
		return( 0 ) ;


	if(( dlen = MiscGetFreeQueueSize( q )) == 0 )
		return( 0 ) ;


	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;


#if	defined(DMSG2)
	_DPUTS( DL_MISCINFO, "MISC::MiscSetQueue()" ) ;
	_DPUTW( DL_MISCINFO, " DataSize:", q->DataSize ) ;
	_DPUTW( DL_MISCINFO, " rp:", rp ) ;
	_DPUTW( DL_MISCINFO, " ReadPtr:", q->ReadPtr ) ;
	_DPUTW( DL_MISCINFO, " wp:", wp ) ;
	_DPUTW( DL_MISCINFO, " WritePtr:", q->WritePtr ) ;
	_DPUTW( DL_MISCINFO, " dlen:", dlen ) ;
	_DPUTW( DL_MISCINFO, " len:", len ) ;
	_DPUTS( DL_MISCINFO, "\n" ) ;
#endif	
	srcb	= (const byte IrDAFAR*)src ;
	if( len > dlen )
		len	= dlen ;


	if( wp < rp ) {
		IrDA_memcpy( &q->Data[wp], srcb, len ) ;
		q->WritePtr	+= len ;
		return( len ) ;
	}


	tlen	= q->DataSize - wp ;
	if( rp == 0 )
		tlen	-- ;								
#if	defined(DMSG2)
	_DPUTS( DL_MISCINFO, "MISC::MiscSetQueue()" ) ;
	_DPUTW( DL_MISCINFO, " tlen:", tlen ) ;
	_DPUTS( DL_MISCINFO, "\n" ) ;
#endif	
	if( len <= tlen ) {
		IrDA_memcpy( &q->Data[wp], srcb, len ) ;
		q->WritePtr	+= len ;
		if( q->WritePtr == q->DataSize )
			q->WritePtr	= 0 ;
		return( len ) ;
	}


	IrDA_memcpy( &q->Data[wp], srcb, tlen ) ;
	dlen	= len - tlen ;
#if	defined(DMSG)
	_DPUTS( DL_MISCINFO, "MISC::MiscSetQueue()" ) ;
	_DPUTW( DL_MISCINFO, " len - tlen:", dlen ) ;
	_DPUTS( DL_MISCINFO, "\n" ) ;
#endif	
	if( dlen )
		IrDA_memcpy( &q->Data[0], &srcb[tlen], dlen ) ;
	q->WritePtr		= dlen ;


	return( len ) ;
}


void	MiscCopyQueue( MISCQUEUE IrDAFAR *dq, MISCQUEUE IrDAFAR *sq )
{
	int		readsz, writesz, size ;
	byte	IrDAFAR *sp ;
	byte	IrDAFAR *dp ;


	for(;;) {
		readsz	= MiscGetReadableQueue( sq, &sp ) ;
		if( readsz == 0 )
			return ;


		writesz	= MiscGetWritableQueue( dq, &dp ) ;
		if( writesz == 0 )							
			return ;


		if( readsz < writesz )
			size	= readsz ;
		else
			size	= writesz ;


		IrDA_memcpy( dp, sp, size ) ;
#if	defined(DMSG2)
{	int		i ;
	_DPRINTF( DL_MISCINFO, "MISC::MiscCopyQueue(): transfered size=%d\n", size ) ;
	_DPRINTF( DL_MISCINFO, "                 : Data=" ) ;
	for( i=0 ; i<size ; i++ )
		_DPRINTF( DL_MISCINFO, "%02x ", (byte)dp[i] ) ;
	_DPRINTF( DL_MISCINFO, "\n" ) ;
}
#endif	
		MiscSkipReadQueue( sq, size ) ;
		MiscSkipWriteQueue( dq, size ) ;
	}
}


#if 0
dword		GetDWord( const void *ptr )
{
	dword	ret ;
	word	h, l ;
	const byte	*p ;


	p		= (const byte*)ptr ;
	l		 = (word)p[0] ;
	l		+= ((word)p[1]) << 8 ;
	h		 = (word)p[2] ;
	h		+= ((word)p[3]) << 8 ;
	ret		= (((dword)h ) << 16 ) + l ;


	return( ret ) ;
}


void		SetDWord( void *ptr, dword data )
{
	word	h, l ;
	byte	*p ;


	p		= (byte*)ptr ;
	h		= (word)( data >> 16 ) ;
	l		= (word)( data & 0xFFFF ) ;
	p[0]	= (byte)(( l      ) & 0x00FF ) ;
	p[1]	= (byte)(( l >> 8 ) & 0x00FF ) ;
	p[2]	= (byte)(( h      ) & 0x00FF ) ;
	p[3]	= (byte)(( h >> 8 ) & 0x00FF ) ;
}
#endif


uint	IrDAMISCAPI	MiscSetBigEndian16( void IrDAFAR *ptr, word var )
{
	((byte IrDAFAR *)ptr)[0]	= (byte)(( var >> 8 ) & 0xFF ) ;
	((byte IrDAFAR *)ptr)[1]	= (byte)(( var      ) & 0xFF ) ;


	return( 2 ) ;
}


uint	IrDAMISCAPI	MiscGetBigEndian16( const void IrDAFAR *ptr, word IrDAFAR *varp )
{
	*varp	 = ((word)((const byte IrDAFAR *)ptr)[0]) << 8 ;
	*varp	+= ((word)((const byte IrDAFAR *)ptr)[1]) ;


	return( 2 ) ;
}


uint	IrDAMISCAPI	MiscSetBigEndian32( void IrDAFAR *ptr, dword var )
{
	word	h, l ;
	byte	IrDAFAR *p ;


	p		= (byte IrDAFAR *)ptr ;
	h		= (word)(( var >> 16 ) & 0xFFFF ) ;
	l		= (word)(( var       ) & 0xFFFF ) ;
	p[0]	= (byte)(( h >> 8 ) & 0x00FF ) ;
	p[1]	= (byte)(( h      ) & 0x00FF ) ;
	p[2]	= (byte)(( l >> 8 ) & 0x00FF ) ;
	p[3]	= (byte)(( l      ) & 0x00FF ) ;


	return( 4 ) ;
}


uint	IrDAMISCAPI	MiscGetBigEndian32( const void IrDAFAR *ptr, dword IrDAFAR *varp )
{
	word	h, l ;
	const byte	IrDAFAR *p ;


	p		= (const byte IrDAFAR*)ptr ;
	h		 = ((word)p[0]) << 8 ;
	h		+= (word)p[1] ;
	l		 = ((word)p[2]) << 8 ;
	l		+= (word)p[3] ;
	*varp	= (((dword)h ) << 16 ) + (dword)l ;
#if	defined(DMSG2)
	_DPUTS( DL_MISCINFO, "MISC::MiscGetBigEndian32()" ) ;
	_DPUTS( DL_MISCINFO, " ptr" ) ;
	_DPUTB( DL_MISCINFO, "=", p[0] ) ;
	_DPUTB( DL_MISCINFO, ".", p[1] ) ;
	_DPUTB( DL_MISCINFO, ".", p[2] ) ;
	_DPUTB( DL_MISCINFO, ".", p[3] ) ;
	_DPUTD( DL_MISCINFO, " *varp:", *varp ) ;
	_DPUTS( DL_MISCINFO, "\n" ) ;
#endif	


	return( 4 ) ;
}


#if defined(SEQCHK)
#define		CHAR_SYNC		0xC0
typedef	struct	_SEQHNDL {
	int		SeqPtr ;
	int		SyncCnt ;
	IrDABOOL	(*SeqFunc)( struct _SEQHNDL *seqhndlp, int c ) ;
}	SEQHNDL ;
static	SEQHNDL	seqhndl[SEQHNDLSIZE] ;
static	IrDABOOL	SeqWaitSyncChar( SEQHNDL *seqhndlp, int c ) ;
static	IrDABOOL	SeqHuntSync( SEQHNDL *seqhndlp, int c ) ;
static	IrDABOOL	SeqCheck( SEQHNDL *seqhndlp, int c ) ;
void		InitSequencer( void )
{
	int		i ;


	for( i=0 ; i<SEQHNDLSIZE ; i++ ) {
#if 0
		seqhndl[i].SeqFunc	= SeqWaitSyncChar ;
#else
		seqhndl[i].SeqFunc	= SeqCheck ;
		seqhndl[i].SeqPtr	= '0' ;
#endif
	}
}


static	IrDABOOL	SeqWaitSyncChar( SEQHNDL *seqhndlp, int c )
{
	if( c != CHAR_SYNC )
		return( FALSE ) ;
	seqhndlp->SeqFunc	= SeqHuntSync ;
	seqhndlp->SyncCnt	= 0 ;
	return( FALSE ) ;
}


static	IrDABOOL	SeqHuntSync( SEQHNDL *seqhndlp, int c )
{
	if( c == CHAR_SYNC ) {
		seqhndlp->SyncCnt	++ ;
		return( FALSE ) ;
	}


	if( seqhndlp->SyncCnt < 4 ) {
		seqhndlp->SeqFunc	= SeqWaitSyncChar ;
		return( FALSE ) ;
	}


	seqhndlp->SeqFunc	= SeqCheck ;
#if 0
	seqhndlp->SeqPtr	= 0 ;
#else
	seqhndlp->SeqPtr	= '0' ;
#endif
	return( FALSE ) ;
}


static	IrDABOOL	SeqCheck( SEQHNDL *seqhndlp, int c )
{
	if( c != seqhndlp->SeqPtr )
		return( TRUE ) ;


#if 0
	seqhndlp->SeqPtr	= ( seqhndlp->SeqPtr + 1 ) & 0xFF ;
#else
	seqhndlp->SeqPtr	++ ;
	if( seqhndlp->SeqPtr > '9' )
		seqhndlp->SeqPtr	= '0' ;
#endif
	return( FALSE ) ;
}


void		irda_errorInd( int hndl, byte IrDAFAR *ptr, uint size ) ;
IrDABOOL		CheckDataSequence( int hndl, byte IrDAFAR *ptr, uint size )
{
	int		i ;


	for( i=0 ; i < size ; i++ ) {
		if((*seqhndl[hndl].SeqFunc)( &seqhndl[hndl], ptr[i] )) {
			irda_errorInd( hndl, ptr, size ) ;
			return( TRUE ) ;
		}
	}


	return( FALSE ) ;
}


IrDABOOL		CheckQueue( int hndl, MISCQUEUE IrDAFAR *q )
{
	uint	dlen, tlen ;
	byte	IrDAFAR *desp ;
	uint	wp, rp ;
	byte	chk ;


return( FALSE ) ;
	wp		= q->WritePtr ;
	rp		= q->ReadPtr ;
	if( wp == rp )
		return( FALSE ) ;


	chk		= q->Data[rp] ;
	for(;;) {
		rp	++ ;
		if( rp >= q->DataSize )
			rp	= 0 ;
		if( rp == wp )
			return( FALSE ) ;


		chk	++ ;
		if( chk > '9' )
			chk	= '0' ;


		if( chk != q->Data[rp] )
			irda_errorInd( hndl, 0, 0 ) ;
	}
}


#endif	


#endif	
