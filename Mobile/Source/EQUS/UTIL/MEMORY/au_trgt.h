/*
 * Copyright (C) 1999, AU-System Radio AB. All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System Radio AB, and may be used and copied
 * only in accordance with the terms of this agreement.
 *
 * AU-System Radio AB assumes no responsibility
 * or liability for any errors or inaccuracies in this software,
 * or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 *
 */
#ifndef AU_TRGT_H
#define AU_TRGT_H


/*
This is set in all the generated files but needs to be set for the non-generated files, unless it's a simulation.
*/
#if !defined XSCT_CMICRO && !defined XSCT_CBASIC
	#define XSCT_CMICRO
#endif


#if     defined __EPOC32__ || defined __SYMBIAN32__
    #ifndef TRGT_EPOC
        #define TRGT_EPOC
    #endif
    #ifndef EPOC
        #define EPOC
    #endif
    #if defined TRGT_OSE || defined TRGT_WIN32
        #error Multiple targets
    #endif
#elif   defined TRGT_OSE || defined OSE || defined __IAR_SYSTEMS_ICC
    #ifndef TRGT_OSE
        #define TRGT_OSE
    #endif
    #ifndef OSE
        #define OSE
    #endif
    #if defined TRGT_EPOC || defined TRGT_WIN32
        #error Multiple targets
    #endif
#elif   defined TRGT_WIN32 || defined WIN32 || defined _WINDOWS || defined _MSC_VER || defined __BORLANDC__ || defined __BCPLUSPLUS__
    #ifndef TRGT_WIN32
        #define TRGT_WIN32
    #endif
    #if defined TRGT_EPOC || defined TRGT_OSE
        #error Multiple targets
    #endif
#elif   defined TRGT_REX
    #if defined TRGT_EPOC || defined TRGT_OSE || defined TRGT_WIN32
        #error Multiple targets
    #endif
#elif   defined TRGT_PALM
    #if defined TRGT_EPOC || defined TRGT_OSE || defined TRGT_WIN32 || defined TRGT_REX
        #error Multiple targets
    #endif
#else
//    #error  No TRGT_... defined  FEATURE_NUI_A10
#endif



#if defined(TRGT_EPOC)

	/* conversion to unicode character */
	#define WC(x)   ((WCHAR)L##x)

#elif defined(TRGT_OSE)

	/* conversion to unicode character */
	#define WC(x)   ((WCHAR)x)

#elif defined(TRGT_WIN32)

	/* conversion to unicode character */
	#define WC(x)   ((WCHAR)L##x)

#elif defined(TRGT_REX)

	/* conversion to unicode character */
	#define WC(x)   ((WCHAR)x)

#elif defined(TRGT_PALM)

	/* conversion to unicode character */
	#define WC(x)   ((WCHAR)x)

#endif


/* The setjmp function in the ANSI C standard
   library of the IAR compiler for the AVR micro
   processor produces erronous machine code.
   If TRGT_OSE is too general, exchange it with
   a compiler flag that identifies the IAR compiler. */
#if !defined(TRGT_OSE)
	#define HAS_SETJMP
#endif


#if !defined(TRGT_EPOC)
	#define IMPORT_C 
	#define EXPORT_C
#else
	#include <_ansi.h>
#endif


/* The MC218 target must have this flag defined! */
/* #define NO_GLOBAL_VARS */


#endif
