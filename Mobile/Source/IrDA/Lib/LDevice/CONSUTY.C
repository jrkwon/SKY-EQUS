/************************************************************************/
/*                                                                      */
/*     Copyright(c) 1996 Okaya Systemware Co.,Ltd.                      */
/*     All Rights Reserved                                              */
/*                                                                      */
/*     THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Okaya Systemware  */
/*     The copyright notice above does not evidence any                 */
/*     actual or intended publication of such source code.              */
/*                                                                      */
/*             File:       CONSUTY.C                                    */
/*             Created:    4/21/96                                      */
/*             Author:     Gontaro Kitazumi                             */
/*                                                                      */
/*             Version:    1.0                                          */
/*             Compiler:   SH series C compiler for RISC of HITACH.     */
/*                                                                      */
/************************************************************************/


#include	<stdio.h>
#include	<string.h>
#include	<stdarg.h>
#include	<stdlib.h>

#include	"irdaconf.h"

#if defined(USE_MONITOR)

#include	"ldevice.h"
#include	"console.h"

int cgetc(void)
{
int	c;
	for(;;){
		c = ConsGetc();
		if(c != -1)
			break;
	}
	return	c;
}

void cputc(int c)
{
	ConsPutc(c);
}

int	cgets(char *cp,int len)
{
int	pos = 0;
int	c;
	*cp = 0;
	for(;;){
		*cp = 0;
		c = cgetc();
		if(c == 0x1b){
			cputc(13);
			cputc(10);
			return	-1;
		}
		if(c == 0x0d){
			cputc(13);
			cputc(10);
			return	pos;
		}
		if(c == 0x08){
			if(pos){
				cputc(0x08);
				cputc(' ');
				cputc(0x08);
				pos--;
				cp--;
			}
		}
		if(c >= ' ' && pos < len){
			*cp++ = c;
			cputc(c);
			pos++;
		}
	}
}

void cputs(char const *cp)
{
	while(*cp){
		if(*cp == 10)
			ConsPutc(13);
		ConsPutc(*cp++);
	}
	ConsFlush();
}


void cprintf(const char *fmt, ... )
{
char	buf[100];
	va_list	va ;
	va_start( va, fmt ) ;
	vsprintf((char*)buf, fmt, va ) ;
	cputs((const char*)buf ) ;
	va_end( va ) ;
}
#endif	/* USE_MONITOR	*/
