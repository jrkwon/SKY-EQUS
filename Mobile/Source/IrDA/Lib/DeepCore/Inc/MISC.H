


#if	!defined(__misc_h__)
#define		__misc_h__


#include	"irda.h"
#include	"dcoldnm.h"


typedef	struct	_MISCQUEUE	{
	byte	IrDAFAR *Data ;		
	uint		DataSize ;		
	uint		ReadPtr ;		
	uint		WritePtr ;		
}	MISCQUEUE ;


#if defined(__cplusplus)
extern	"C"	{
#endif	


void		MiscInitRandomize( void ) ;
byte		MiscRandom8( byte max ) ;
word		MiscRandom16( word max ) ;
dword		MiscRandom32( dword max ) ;
#if defined(NOSTDLIB)
void		*memcpy( void *, const void *, word ) ;
int			_memcmp( const void *des, const void *src, word len ) ;
void		memset( void *, byte c, word len ) ;
#endif	


void		MiscInitQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR *buf, uint len ) ;
void		MiscFlushQueue( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetUsedQueueSize( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetFreeQueueSize( MISCQUEUE IrDAFAR *q ) ;
uint		MiscGetReadableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR *ptr ) ;
uint		MiscGetWritableQueue( MISCQUEUE IrDAFAR *q, byte IrDAFAR * IrDAFAR *ptr ) ;
void		MiscSkipReadQueue( MISCQUEUE IrDAFAR *q, uint len ) ;
void		MiscSkipWriteQueue( MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscGetQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscSenseQueue( void IrDAFAR *des, MISCQUEUE IrDAFAR *q, uint len ) ;
uint		MiscSetQueue( MISCQUEUE IrDAFAR *q, const void IrDAFAR *src, uint len ) ;
void		MiscCopyQueue( MISCQUEUE IrDAFAR *dq, MISCQUEUE IrDAFAR *sq ) ;


uint	IrDAMISCAPI	MiscSetBigEndian16( void IrDAFAR *ptr, word var ) ;
uint	IrDAMISCAPI	MiscGetBigEndian16( const void IrDAFAR *ptr, word IrDAFAR *varp ) ;
uint	IrDAMISCAPI	MiscSetBigEndian32( void IrDAFAR *ptr, dword var ) ;
uint	IrDAMISCAPI	MiscGetBigEndian32( const void IrDAFAR *ptr, dword IrDAFAR *varp ) ;


#if defined(SEQCHK)
#define		SEQHNDLSIZE		20
void		InitSequencer( void ) ;
IrDABOOL		CheckDataSequence( int hndl, byte IrDAFAR *ptr, uint size ) ;
IrDABOOL		CheckQueue( int hndl, MISCQUEUE IrDAFAR *q ) ;
#endif	


#if defined(__cplusplus)
}
#endif	


#endif		