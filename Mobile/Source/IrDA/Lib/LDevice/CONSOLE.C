/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       CONSOLE.C                                    */
/*             Created:    4/21/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"irdaconf.h"
#include	"console.h"
#include	"ldevice.h"

#if defined(USE_MONITOR)
int cget(void)
{
	return	ConsGetc();
}

#if 0
int cput(int c)
{
	ConsPutc(c);
}
#endif


void cFlush()
{
	ConsFlush();
}

void	cbaud(long baud)
{
	ConsSetBaud(baud);
}
#endif
