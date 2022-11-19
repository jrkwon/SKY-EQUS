/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       IREYE.C                                      */
/*             Created:    5/18/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

/*#include	<machine.h>*/
#include	"irdaconf.h"
#include	"ldevice.h"

#if defined(USE_MONITOR)


int	IrEyeGetc(void)
{
	return	IrdaGetc();
}



void	IrEyeFlush(void)
{
	IrdaFlush();
}

void	IrEyePutc(int c)
{
	IrdaPutc(c);
}

#if defined(USE_MONITOR)

void	IrEyePuts( char *cp )
{
	char	c;

	while( c = *cp ) {
		if( c == '\n' )
			IrEyePutc( '\r' ) ;
		IrEyePutc( c ) ;
		cp	++ ;
	}
	IrEyeFlush() ;
}
#endif
#endif
