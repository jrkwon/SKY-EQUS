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
#ifndef WIPTRGT_H
#define WIPTRGT_H


/*
This is set in all the generated files but needs to be set for the non-generated files, unless it's a simulation.
*/
#if !defined XSCT_CMICRO && !defined XSCT_CBASIC
    #define XSCT_CMICRO
#endif


#if     defined TRGT_EPOC || defined __EPOC32__ || defined __SYMBIAN32__
    #ifndef TRGT_EPOC
        #define TRGT_EPOC
    #endif
#elif   defined TRGT_OSE || defined OSE || defined __IAR_SYSTEMS_ICC || defined __IAR_SYSTEMS_ICC__ || defined __ICCARM__
    #ifndef TRGT_OSE
        #define TRGT_OSE
    #endif
#elif   defined TRGT_WIN32 || defined _MSC_VER || defined __BORLANDC__ || defined __BCPLUSPLUS__
    #ifndef TRGT_WIN32
        #define TRGT_WIN32
    #endif
#endif


/*  HAS_FLOAT   */
/*  Defined, if the FLOAT32 type is mapped to a host device type. undefined otherwise.   */
/*
If this constant is defined, the ANSI C library functions pow, sqrt, ceil and floor 
are expected to be available.
*/
#if     defined TRGT_EPOC
    #define HAS_FLOAT
#elif   defined TRGT_NUCLEUS
    #define HAS_FLOAT
#elif   defined TRGT_NUC166
	#undef HAS_FLOAT
#elif     defined TRGT_SDLM
    #undef HAS_FLOAT	/* SDL-Machine does not support float  */
#elif   defined TRGT_SEOS
    #define HAS_FLOAT
#elif   defined TRGT_OS_9
    #define HAS_FLOAT
#elif   defined TRGT_OSE
    #define HAS_FLOAT
#elif	defined TRGT_OSESK
    #define HAS_FLOAT
#elif   defined TRGT_PALM
    #define HAS_FLOAT
#elif   defined TRGT_PSOS
    #define HAS_FLOAT
#elif   defined TRGT_REX
    #define HAS_FLOAT
#elif   defined TRGT_RTKEU
    #undef HAS_FLOAT
#elif   defined TRGT_RTKE
    #undef HAS_FLOAT
#elif   defined TRGT_VXWORKS
    #define HAS_FLOAT
#elif   defined TRGT_WIN32
    #define HAS_FLOAT
#elif   defined TRGT_NUCBCB
	#undef HAS_FLOAT
#elif   defined TRGT_G23
	#undef HAS_FLOAT
#else
    #error  No TRGT_... defined for HAS_FLOAT
#endif


/*
WC(x)
conversion to unicode character
*/
#if     defined(TRGT_EPOC)
    #define WC(x)   ((WCHAR)L##x)
#elif   defined(TRGT_NUCLEUS)
    #define WC(x)   ((MCHAR)x)
#elif   defined(TRGT_NUC166)
	#define WC(x)   ((WCHAR)L##x)
#elif   defined(TRGT_SDLM)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_SEOS)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_OS_9)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_OSE)
    #define WC(x)   ((WCHAR)x)
#elif	defined(TRGT_OSESK)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_PALM)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_PSOS)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_REX)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_RTKE)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_RTKE)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_G23)
    #define WC(x)   ((WCHAR)x)
#elif   defined(TRGT_VXWORKS)
    #define WC(x)   ((WCHAR)L##x)
#elif   defined(TRGT_WIN32)
    #define WC(x)   ((WCHAR)L##x)
#elif   defined(TRGT_NUCBCB)
	#define WC(x)   ((WCHAR)L##x)
#endif


/*
The setjmp function in the ANSI C standard
library of the IAR compiler for the AVR micro
processor produces erronous machine code.
If TRGT_OSE is too general, exchange it with
a compiler flag that identifies the IAR compiler.
*/
#if !defined(TRGT_OSE)
    #define HAS_SETJMP
#endif


/*
EPOC uses IMPORT_C and EXPORT_C.
*/
#if defined(TRGT_EPOC)
    #include <_ansi.h>
#else
    #define IMPORT_C
    #define EXPORT_C
#endif


#endif
