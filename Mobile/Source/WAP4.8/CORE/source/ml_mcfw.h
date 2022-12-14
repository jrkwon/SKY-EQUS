/*
 * Copyright (C) Ericsson Mobile Communications AB, 2001.
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
/*
** This file ml_mcf.h has been generated by the Bodybuilder - Version 1.30
** Date: 18.07.01
*/
#ifndef __ML_MCF_H_ 
#define __ML_MCF_H_ 
/***********************************************/
/**** Generic Section :               KERNEL   */
/***********************************************/
/*** Subsection :                 PREEMPTION   */


/*** Subsection :           SIGNALPRIORITIES   */


/*** Subsection :              KERNELSCALING   */

#define XMK_USE_MORE_THAN_250_SIGNALS        
#define XMK_USE_PAR_GREATER_THAN_250         
#define XMK_USE_PROCEDURE_INIT               
#define XNOMAIN                              
#define XMK_MAX_SIGNALS                     45//30 //yjahn96 01.10.31 - for input_format

/*** Subsection :               TIMERSCALING   */

#define XMK_TIMERPRIO                       100 

/* BEGIN User Code */

#include "wiptrgt.h"

#define XMK_USE_EXPANDED_KERNEL
#define XMBM_COPIED 2

#ifndef XNOMAIN
    #define XNOMAIN
#endif

#ifdef XMK_USE_SDL_MEM
    #undef XMK_USE_SDL_MEM
#endif

/*  GBU: If we want internal debug-info., we also want external. */
#ifdef LOG_INTERNAL
    #ifndef LOG_EXTERNAL
        #define LOG_EXTERNAL
    #endif
#endif

/*   END User Code */

/***********************************************/
/**** Generic Section :              LIBRARY   */
/***********************************************/
/*** Subsection :             SIGNALHANDLING   */

#define XMK_USE_RECEIVER_PID_IN_SIGNAL       
#define XMK_MSG_BORDER_LEN                  4 
#define XMK_USE_xmk_SendSimple               
#define XMK_USE_SENDER_PID_IN_SIGNAL         

/*** Subsection :                 ERRORCHECK   */


/* BEGIN User Code */
/*  Added by GBU,000103 */
#ifdef XMK_USE_MON
    #undef XMK_USE_MON
#endif
#ifdef XMK_ADD_PRINTF
    #undef XMK_ADD_PRINTF
#endif
#ifdef XMK_ADD_STDIO
    #undef XMK_ADD_STDIO
#endif
#ifdef XMK_USE_CHECK_TRANS_TIME
    #undef XMK_USE_CHECK_TRANS_TIME
#endif

/*  Added by GBU,000103 optimization: disable all tests */
#ifdef XMK_USE_MIN_ERR_CHECK
    #undef XMK_USE_MIN_ERR_CHECK
#endif
#ifdef LOG_INTERNAL
    #ifdef XMK_USE_NO_ERR_CHECK
        #undef XMK_USE_NO_ERR_CHECK
    #endif
    #ifndef XMK_USE_MAX_ERR_CHECK
        #define XMK_USE_MAX_ERR_CHECK
    #endif
#else
    #ifdef XMK_USE_MAX_ERR_CHECK
        #undef XMK_USE_MAX_ERR_CHECK
    #endif
    #ifndef XMK_USE_NO_ERR_CHECK
        #define XMK_USE_NO_ERR_CHECK
    #endif
#endif
/*   END User Code */

/***********************************************/
/**** Generic Section :               TESTER   */
/***********************************************/
/*** Subsection :                 TESTERINIT   */


/*** Subsection :                     TRACER   */

#define XMK_MAX_PRINT_STRING                40 

/*** Subsection :                   RECORDER   */


/* BEGIN User Code */
/*  Added by GBU,000103 */
#ifdef XMK_ADD_MICRO_TESTER
    #error XMK_ADD_MICRO_TESTER_is_not_used_in_GenericWAPClient
#endif
/*   END User Code */

/***********************************************/
/**** Generic Section :          SDL_SUPPORT   */
/***********************************************/
/*** Subsection :          PRE_DEFINED_SORTS   */

#define XERANGE                              
#define XTESTF                               
#define XEFIXOF                              
#define XEINDEX                              
#define XEINTDIV                             
#define XEREALDIV                            
#define XMK_MAX_SDL_CHARSTRING              50 

/*** Subsection :                   VAR_SIZE   */

#define X_COMPACT_BOOL                       
#define X_LONG_INT                           

/*** Subsection :                     MEMORY   */


/* BEGIN User Code */
/* ONJ: this will force all real numbers to be long integers. */
#define XNOUSEOFREAL
#define SDL_Real long
#define xFloat_SDL_Integer(I)   ((long)I)
#define yAssF_SDL_Real(V,E,A)  (V = E)
#define xDef_SDL_Real(V)       *(V) = 0

/*  Added by GBU,000103 optimization: disable all tests */
#ifndef LOG_INTERNAL
    #ifdef XECSOP
        #undef XECSOP
    #endif
    #ifdef XEFIXOF
        #undef XEFIXOF
    #endif
    #ifdef XEINDEX
        #undef XEINDEX
    #endif
    #ifdef XEINTDIV
        #undef XEINTDIV
    #endif
    #ifdef XERANGE
        #undef XERANGE
    #endif
    #ifdef XEREALDIV
        #undef XEREALDIV
    #endif
    #ifdef XTESTF
        #undef XTESTF
    #endif
#endif
/*   END User Code */

/***********************************************/
/**** Generic Section :              SDL_ENV   */
/***********************************************/
/*** Subsection :                ENVIRONMENT   */


/* BEGIN User Code */
/*  GBU */
/*  (This is the last user code area.) */

/*   END User Code */

/***********************************************/
/*** Subsection : Communications Link and Compiler Defines*/
/***********************************************/
#endif /* ... __ML_MCF_H_ */
