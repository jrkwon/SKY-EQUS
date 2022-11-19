/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#ifndef ANSILIBS_H
#define ANSILIBS_H


#include "wiptrgt.h"


#if defined TRGT_EPOC

    #include <limits.h>
	#include <stddef.h>

#endif


/* Generic includes  */

#if !defined _WIN32_WCE

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <setjmp.h>
    #ifdef HAS_FLOAT
        #include <math.h>
        #include <errno.h>
    #endif

#elif   defined _WIN32_WCE

    #include <stdlib.h>
    #ifdef VOID
        #undef VOID
        #define DEFINE_VOID
    #endif
    #include <string.h>
    #ifdef DEFINE_VOID
        #undef DEFINE_VOID
        #define VOID void
    #endif
    /* stdio.h does not exist in Windows CE */
    int sprintf(char *,const char *, ...);
    /* strtod does not exist in Windows CE */
    double strtod(const char *, char **);
    #include <setjmp.h>
    #ifdef HAS_FLOAT
        /* errno is defined in stdlib.h! */
        #include <math.h>
    #endif

#endif

/* Speceific redefinitions of standard ANSI functions  */ 

#if   defined TRGT_SEOS

    #define srand(x) srand( (int)(x) )
	#define malloc(x) malloc( (unsigned int)(x) )
	#define pow(x,y) pow( (float)(x), (float)(y) )
	#define sqrt(x) sqrt( (float)(x) )
	#define ceil(x) ceil( (float)(x) )
	#define floor(x) floor( (float)(x) )
	#define memset(x, y ,z) memset(x, (char)(y), (int)(z))
	#define memcpy(x, y, z) memcpy(x, y, (int)(z))
	#define memcmp(x, y, z) memcmp(x, y, (int)(z))

#endif

#if		defined TRGT_RTKE

		#define setjmp	MC_RTK_SET_JUMP
		#define longjmp	MC_RTK_LONGJUMP 
		
#endif

#if	defined TRGT_PALM

		#include <MemoryMgr.h>
		#include <StringMgr.h>
		#include <sys_types.h>
		#define IGNORE_STDIO_STUBS
		#include <unix_stdio.h>
		#include "unix_stdlib.h"
		#include "unix_string.h"
		#include "math.h"
		#include "errno.h"
		#include "setjmp.h"

#endif

#endif

