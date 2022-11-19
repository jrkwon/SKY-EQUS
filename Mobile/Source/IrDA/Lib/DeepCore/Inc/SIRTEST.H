


#if !defined(__sirtest_h__)
#define	__sirtest_h__


#include	"irda.h"


#define	STR_DONE	0			
#define	STR_NOSVR	-1			


#define	SIRTESTVER	0x00


void	st_setrxfrm( uint rf, uint rw, void IrDAFAR *rp ) ;
void	st_settxfrm( uint tf, uint tw, void IrDAFAR *tp ) ;


void	st_startsvr( void ) ;
void	st_chktimer( void ) ;
int		st_chkrxframe( dword bps, uint datasz ) ;
int		st_chktxframe( dword bps, uint datasz ) ;
int		st_chkmtt( dword bps, uint datasz ) ;
int		st_rxrate( dword bps, uint datasz, uint sec ) ;
int		st_txrate( dword bps, uint datasz, uint sec ) ;
int		st_echorate( dword bps, uint datasz, uint sec ) ;


void	st_putc( int c ) ;
void	st_puts( char IrDAFAR *msg ) ;
void	st_put( char IrDAFAR *msg, uint len ) ;
int		st_chkabort( void ) ;


#endif	