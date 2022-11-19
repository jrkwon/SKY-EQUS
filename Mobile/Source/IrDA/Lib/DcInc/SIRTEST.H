/*
 *     IrDA Driver DeepCore(tm) Version 1
 *
 *     Copyright(c) 1996-97 Okaya Systemware Co.,Ltd.
 *     All Rights Reserved
 *
 *     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware
 *     The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *     $Header: SIRTEST.HV  170.0  97/10/09 16:03:52  core  Exp $
 */


#if !defined(__sirtest_h__)
#define	__sirtest_h__


#include	"irda.h"


/*
**	return code
*/
#define	STR_DONE	0			/* normal end								*/
#define	STR_NOSVR	-1			/* not found server							*/


/*
**	Misc
*/
#define	SIRTESTVER	0x00


/*
**	Extern Functions
*/
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


#endif	/*__sirtest_h__*/
