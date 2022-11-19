/*+MHDR*/
/*
# clearcase: CmicroRel2.3


+------------------------------------------------------------------------------+
|  Modulname    : MK_CPU.C                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|    This module is a template for functions, which are to be filled up        |
|    by the user of the Cmicro Kernel.                                         |
|                                                                              |
|    1.Character read / write on hardware / OS-level                           |
|      xmk_PutString  () Unblocking function to write a string to display.     |
|                        Called within this module only (xmk_printf).          |
|      xmk_GetChar    () Unblocking Character read function. Used by the       |
|                        Cmicro Tester in some cases.                          |
|      xmk_TermInit   () Terminal initialization                               |
|      xmk_TermDeinit () Terminal deinitialization                             |
|      xmk_printf     () Same as usual printf known from ANSI C compiler.      |
|                        Can be used if C compiler does not support printf     |
|                        or sprintf function. Used by the Cmicro Tester in     |
|                        some cases.                                           |
|                                                                              |
|      Cmicro Tester                                                           |
|           !                                                                  |
|         calls                                                                |
|           !                                                                  |
|           V                                                                  |
|       xmk_printf                                                             |
|           !                                                                  |
|         calls                                                                |
|           !                                                                  |
|           V                                                                  |
|       xmk_PutString                                                          |
|                                                                              |
|    2.Functions for dynamic memory handling.                                  |
|      This is an intermediate layer to the compiler function library          |
|      The following functions exists                                          |
|      xAlloc                () Used for xAlloc_SDL_Charstring                 |
|      xFree                 () Used for xFree_SDL_Charstring                  |
|      xAlloc_SDL_Charstring () Dynamic allocation of SDL Charstrings          |
|      xFree_SDL_Charstring  () Release of dynamic allocated Charstrings       |
|      xAlloc_SDL_Bit_String () Dynamic allocation of SDL Bitstrings           |
|      xFree_SDL_Bit_String  () Release of dynamic allocated Bitstrings        |
|                                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-MHDR*/

/*
+------------------------------------------------------------------------------+
|                                                                              |
|  Copyright by Telelogic AB 1993 - 1998                                       |
|  Copyright by S&P Media GmbH Germany 1993 - 1998                             |
|                                                                              |
|  This Program is owned by Telelogic and is protected by national             |
|  copyright laws and international copyright treaties. Telelogic              |
|  grants you the right to use this Program on one computer or in              |
|  one local computer network at any one time.                                 |
|  Under this License you may only modify the source code for the purpose      |
|  of adapting it to your environment. You must reproduce and include          |
|  any copyright and trademark notices on all copies of the source code.       |
|  You may not use, copy, merge, modify or transfer the Program except as      |
|  provided in this License.                                                   |
|  Telelogic does not warrant that the Program will meet your                  |
|  requirements or that the operation of the Program will be                   |
|  uninterrupted and error free. You are solely responsible that the           |
|  selection of the Program and the modification of the source code            |
|  will achieve your intended results and that the results are actually        |
|  obtained.                                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/

#ifndef __MK_CPU_C_
#define __MK_CPU_C_


/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typ.h"

#ifdef XMK_UNIX
  #include <unistd.h>
  #include <signal.h>
  #include <errno.h>
#endif

#if defined(XMK_UNIX) 
  #include <varargs.h>
#endif

#if defined(BORLAND_C) 
  #include <stdarg.h>
#endif

#if defined(MICROSOFT_C)
  #include <stdarg.h>
#endif

#if defined (LOG_INTERNAL)
  #include "aapiclnt.h"
#endif

#include "cmmnrsrc.h"

 
/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/
 
/*--------------------  Constants,  Macros  ----------------------------------*/

#ifdef XSYSID 
  /* 
  ** CAUTION ! 
  ** ============ 
  ** This module cannot be used when partitioning is used. The reason 
  ** for this is that the functions of this module operate on global 
  ** variables and partitioned systems may preempt each other when 
  ** an Real Time Operating System is used. 
  ** However, users can use this module as a basis for implementing 
  ** their own terminal and memory management functions.  
  */ 
  #error "ERROR_in_mk_cpu_c_Module_cannot_be_used_for_Partitioning" 
#endif /* ... XSYSID */ 


#ifdef P
#undef P
#endif

/*--------------------  Typedefinitions     ----------------------------------*/

#define xmk_exit exit
/*--------------------  Functions    -----------------------------------------*/
#ifdef XMK_UNIX
  static void sig_catch(int);
#endif

/*--------------------  Variables    -----------------------------------------*/

#ifdef XMK_UNIX
  struct termios tty_buf, xmk_OldTTYSettings;
#endif

/*============================================================================*/
/*-MGG*/

#ifndef XMK_USE_NO_ERR_CHECK
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_PutString                                                |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for how to print out a character string, terminated with 0.      |
|                                                                              |
|  CAUTION                                                                     |
|  ==========                                                                  |
|  The function must return immediately, i.e.may not be blocking on the        |
|  output device. If the user does not care about this restriction, the correct|
|  function of other program parts cannot be guaranteed.                       |
|                                                                              |
|  Parameter    : char * Param1 Pointer to string, terminated with 0           |
|                 int    length string length                                  |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_PutString ( char * Param1 )
#else
  void xmk_PutString ( Param1 )
  char  *Param1;
#endif

/*-FDEF E*/
{
/*  Added by GBU,990516,    to get rid of IAR warning */
    Param1 = Param1;

  /*
  ** NEVER CALL xmk_printf here !!!!!
  ** NEITHER USE XMK_PRINTF macros !!!!!
  */
  #if defined(XMK_UNIX)

    (void)fprintf (stdout,"%s", Param1);

  #elif defined(BORLAND_C)

    #ifdef XMK_WINDOWS
      (void)fprintf (stdout,"%s", Param1);
    #else
      /* Non Console applications do not support fprintf in Windows !!! */
      xmk_PutString_is_not_filled_yet = 0; 
    #endif

  #elif defined(MICROSOFT_C)
    #ifdef XMK_WINDOWS
      (void)fprintf (stdout,"%s", Param1);
    #else
      /* Non Console applications do not support fprintf in Windows !!! */
      xmk_PutString_is_not_filled_yet = 0; 
    #endif
  #else
	#if defined (LOG_INTERNAL)
      CLNTa_log(0, 0, "SDL kernel: %s\n", Param1);
	#endif
  #endif

  return ;
}
#endif


#ifndef XMK_USE_NO_ERR_CHECK
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_GetChar                                                  |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template to ask the OS, if the user has typed any key and which one.      |
|  If the user has pressed no key, the function returns with EOF.              |
|  If any key was pressed, the function returns with the key value.            |
|                                                                              |
|  CAUTION                                                                     |
|  ==========                                                                  |
|  The function must return immediately, i.e.may not be blocking on the        |
|  input device. If the user does not care about this restriction, the correct |
|  function of other program parts cannot be guaranteed.                       |
|                                                                              |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : int    - character or XMK_EOF                                |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/
 
/*+FDEF E*/

#ifndef XNOPROTO
  int xmk_GetChar ( void )
#else
  int xmk_GetChar ( )
#endif

/*-FDEF E*/
{
  #ifdef XMK_UNIX
    int cnt;
    static char c;
  #endif

  #if defined(BORLAND_C) || defined(MICROSOFT_C)
    int c;
  #endif

  #ifdef XMK_ADD_PRINTF_CPU
    XMK_FUNCTION("xmk_GetChar");
  #endif

  #if defined(XMK_UNIX)

    cnt = read ( STDIN_FILENO , (void *) &c, 1 );

    if (cnt < 0)
    {
      (void)fprintf (STDOUT," cnt is : %d", cnt);
    }

    if (cnt>0)
    {
      return (((int) c) & 0xff);
    }

    return ((int) XMK_EOF);

  #elif defined(BORLAND_C) || defined(MICROSOFT_C)

    if (! kbhit () ) return (XMK_EOF);
 
    c = _getch ( );

    if (XMK_IS_PRINTABLE_CHAR(c))
      (void)fprintf (STDOUT, "%c", (char) c);
  
    return (c);

  #else

    #ifdef XMK_ADD_PRINTF_CPU
      XMK_TRACE_EXIT("xmk_GetChar");
    #endif

    return (XMK_FALSE);

  #endif
}
#endif

#ifdef XMK_UNIX

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : sig_catch                                                    |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template to react on any UNIX process signal. This function represents    |
|  the signal handler, that is called by UNIX in the case if it was defined    |
|  correctly with the "signal()" C function. The initialization of all signals |
|  and their handling is done within the xmk_TermInit () C function within this|
|  module.                                                                     |
|                                                                              |
|  Parameter    : int signo - Signal's number                                  |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/
 
/*+FDEF E*/

#ifndef XNOPROTO
  static void sig_catch(int signo)
#else
  static void sig_catch(signo)
  int signo;
#endif

/*-FDEF E*/
{
  static int sema = 0;
  int i = 6; /* Wati 6 seconds before exit */

  XMK_SUPPRESS(SIGNO);

  /*
  ** Only one signal is handled here
  */
  if (sema) return;
  sema = 1;

  (void)fprintf(stderr, "**ERR:sdtmt:Using emergency exit due to system signal.\n");

  while (i--)
  {
    (void)fprintf(stderr, ".");
    (void)sleep (1);
  }

  xmk_exit(0);
}

#endif /* ... XMK_UNIX */

#ifdef XMK_USE_OS_ENVIRONMENT
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_TermInit                                                 |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template to initialize the user terminal.                                 |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  int        xmk_TermInit ( void )
#else
  int        xmk_TermInit ( )
#endif

/*-FDEF E*/
{
  #ifdef XMK_ADD_PRINTF_CPU
    XMK_FUNCTION("xmk_TermInit");
  #endif

  #ifdef XMK_UNIX
    if (signal(SIGHUP, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGHUP (problem ignored).\n");

    if (signal(SIGSEGV, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGSEGV (problem ignored).\n");

    if (signal(SIGPIPE, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGPIPE (problem ignored).\n");

    if (signal(SIGINT, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGINT (problem ignored).\n");

    if (signal(SIGQUIT, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGQUIT (problem ignored).\n");

    if (signal(SIGTERM, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGTERM (problem ignored).\n");
 
    if (signal(SIGCLD, sig_catch) == SIG_ERR)
      (void)fprintf(stderr, "Warning: Could not set signalhandler on UNIX signal SIGCLD (problem ignored).\n");

    if (tcflush(STDIN_FILENO, TCIOFLUSH) == -1)
    {
      (void)fprintf(stderr, "Warning: Could not flush terminal (problem ignored).\n");
    }

    /*
    ** Read the current tty settings ...
    */
    if (tcgetattr(STDIN_FILENO, &tty_buf) == -1)
    {
      (void)fprintf(stderr, "Warning: Could not get current terminal settings (problem ignored).\n");
    }

    /*
    ** Save the current tty settings
    */
    (void)memcpy ((void*) &xmk_OldTTYSettings,
                  (void*) &tty_buf,
                  sizeof (xmk_OldTTYSettings));

    /*
    ** Modify the current tty settings to canonical without echo ...
    ** (fgetc must return immediately when no key was pressed)
    */
    tty_buf.c_lflag &= ~ICANON;
    tty_buf.c_cc[VMIN]  = 0;
    tty_buf.c_cc[VTIME] = 0;
    tty_buf.c_cc[VERASE] = 8;

    #ifdef ECHOCTL_OFF
      tty_buf.c_lflag &= ~ECHOCTL;
    #endif

    /*
    ** Write the new tty settings ...
    */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty_buf) == -1)
    {
      (void)fprintf(stderr, "Warning: Could not initialize terminal (problem ignored).\n");
    }
  #endif

  #ifdef XMK_ADD_PRINTF_CPU
    XMK_TRACE_EXIT("xmk_TermInit");
  #endif

  return (XMK_OKAY);
}
#endif

#ifdef XMK_USE_OS_ENVIRONMENT
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_TermDeinit                                               |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template to initialize the user terminal.                                 |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/
 
#ifndef XNOPROTO
  int        xmk_TermDeinit ( void )
#else
  int        xmk_TermDeinit ( )
#endif

/*-FDEF E*/
{
  #ifdef XMK_ADD_PRINTF_CPU
    XMK_FUNCTION("xmk_TermDeinit");
  #endif

  #ifdef XMK_UNIX

    /*
    ** Restore the old terminal settings ...
    */
    if (tcsetattr(STDIN_FILENO, TCSANOW, &xmk_OldTTYSettings) == -1)
    {
      (void)fprintf(stderr, "Warning: Could not reset terminal (problem ignored).\n");
    }
  #endif

  #ifdef XMK_ADD_PRINTF_CPU
    XMK_TRACE_EXIT("xmk_TermDeinit");
  #endif

  return (XMK_OKAY);
}
#endif

#ifdef XMK_ADD_STDIO
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_printf                                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template to print a string by using var args.                             |
|                                                                              |
|  This function is called from any place in the Cmicro Library, Cmicro Kernel |
|  or generated C code, if the printf functionality is compiled with at least  |
|  one of the XMK_ADD_PRINTF* defines.                                         |
|  The function must return immediately, i.e.may not be blocking on the output |  
|  device. If the user does not care about this restriction, the correct       |   
|  function of other program parts cannot be guaranteed.                       |
|  The function can be used for ANSI C compilers only, because optional        |
|  argument lists are used like in the printf function of the standard C       |
|  library.                                                                    |
|                                                                              |
|  Parameter    : char* format        Usual printf convention                  |
|                 ...                 var args                                 |
|                                                                              |
|  Return       : int                 without any meaning, because this is     |
|                                     is used to let declaration look like     |
|                                     declaration for printf.                  |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  int        xmk_printf ( char* format, ... )
#else
  #error "Sorry_No_Support_for_optional_Arguments_in_KR_Compilers"
#endif

/*-FDEF E*/
{

  #if defined(XMK_UNIX) || defined(BORLAND_C) || defined(MICROSOFT_C)
    char* strg = (char*) malloc(512 * sizeof (char));
    va_list params;

    #ifdef XMK_WINDOWS
      va_start (params, format); 
    #elif defined(XMK_UNIX)
      va_start (params); 
    #endif

    vsprintf (strg, format, params);

    XMK_BEGIN_CRITICAL_PATH 
      xmk_PutString (strg);
    XMK_END_CRITICAL_PATH 

    va_end (params);
    free (strg);
  #else
	  #if defined (LOG_INTERNAL)
		char* strg = (char*) malloc(512 * sizeof (char));
		va_list params;

		#ifdef XMK_WINDOWS
		  va_start (params, format); 
		#elif defined(XMK_UNIX)
		  va_start (params); 
		#endif

		vsprintf (strg, format, params);

		XMK_BEGIN_CRITICAL_PATH 
		  xmk_PutString (strg);
		XMK_END_CRITICAL_PATH 

		va_end (params);
		free (strg);
	  #endif
  #endif
  return (0);

}

#endif /* XMK_ADD_STDIO*/
/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xAlloc                                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for memory allocation.                                           |
|  The function is called from any place in the Cmicro Library, Cmicro Kernel  |
|  Cmicro Tester or generated C code, if memory is to be allocated dynamically |
|  The user may choose between the dynamic memory allocation functions from the|
|  C compiler or operating system or the dynamic memory allocation functions   |
|  from Cmicro.                                                                |
|                                                                              |
|  Parameter    : unsigned int Size     requested memory area block - size     |
|                                                                              |
|  Return       : SDL_Charstring        Pointer to allocated area or NULL,     |
|                                       if no dynamic allocation was possible. |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_RAM_ptr xAlloc( xptrint  Size )
#else
  void xmk_RAM_ptr xAlloc( Size )
  xptrint  Size;
#endif

/*-FDEF E*/
{
  void *P;

  P = (char *)OSConnectorAlloc((unsigned)Size);
  (void)memset(P, 0, (int)Size);

  return (P);

}


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xFree                                                        |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This is the counter part of xAlloc. The function is called when a memory    |
|  block that was allocated with xAlloc before can be deallocated again.       |
|                                                                              |
|  Parameter    : unsigned int Size     requested memory area block - size     |
|                                                                              |
|  Return       : SDL_Charstring        Pointer to allocated area or NULL,     |
|                                       if no dynamic allocation was possible. |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xFree( void xmk_RAM_ptr xmk_RAM_ptr  P )
#else
  void xFree( P )
  void xmk_RAM_ptr xmk_RAM_ptr  P;
#endif

/*-FDEF E*/
{

  if (! *P) 
  {
    return ;
  }

  #if defined(_GCC_)
   free((char xmk_RAM_ptr)*P);
  #else
   OSConnectorFree(*P);
  #endif

  *P = (void xmk_RAM_ptr)0;

}

#if !defined(XNOUSEOFCHARSTRING) && !defined(XRESTUSEOFCHARSTRING)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xAlloc_SDL_Charstring                                        |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for the implementation of the allocation of SDL Charstrings.     |
|  It is compiled only, if XNOUSEOFCHARSTRING is undefined.                    |
|                                                                              |
|  Parameter    : unsigned int Size     requested memory area block - size     |
|                                                                              |
|  Return       : SDL_Charstring        Pointer to allocated area or NULL,     |
|                                       if no dynamic allocation was possible. |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  SDL_Charstring xAlloc_SDL_Charstring (xptrint  Size)
#else
  SDL_Charstring xAlloc_SDL_Charstring ( Size )
  xptrint  Size;
#endif

/*-FDEF E*/
{
  return (SDL_Charstring)xAlloc(Size);
}


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xFree_SDL_Charstring                                         |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for the implementation of the allocation of SDL Charstrings.     |
|  It is compiled only, if XNOUSEOFCHARSTRING is undefined.                    |
|                                                                              |
|  Parameter    : void **C              pointer to mem block to be released    |
|                                                                              |
|  Return       : void                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xFree_SDL_Charstring (void xmk_RAM_ptr xmk_RAM_ptr C)
#else
  void xFree_SDL_Charstring ( C )
  void xmk_RAM_ptr xmk_RAM_ptr C;
#endif

/*-FDEF E*/
{
  xFree(C);
}

#endif /* ... !defined(XNOUSEOFCHARSTRING) && !defined(XRESTUSEOFCHARSTRING) */

#if !defined(XNOUSEOFASN1) && !defined(XNOUSEOFOCTETBITSTRING)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xAlloc_SDL_Bit_String                                        |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for the implementation of the allocation of SDL Charstrings.     |
|  It is compiled only, if XNOUSEOFCHARSTRING is undefined.                    |
|                                                                              |
|  Parameter    : unsigned int Size     requested memory area block - size     |
|                                                                              |
|  Return       : unsigned char *       Pointer to allocated area or NULL,     |
|                                       if no dynamic allocation was possible. |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/
 
/*+FDEF E*/
 
#ifndef XNOPROTO
  unsigned char * xAlloc_SDL_Bit_String ( xptrint  Size )
#else
  unsigned char * xAlloc_SDL_Bit_String ( Size )
  xptrint  Size;
#endif

/*-FDEF E*/
{
  return ( unsigned char *) xAlloc(Size);
}


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xFree_SDL_Bit_String                                         |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  A template for the implementation of the allocation of SDL Charstrings.     |
|  It is compiled only, if XNOUSEOFCHARSTRING is undefined.                    |
|                                                                              |
|  Parameter    : void **C              pointer to mem block to be released    |
|                                                                              |
|  Return       : void                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xFree_SDL_Bit_String (void xmk_RAM_ptr xmk_RAM_ptr B)
#else
  void xFree_SDL_Bit_String ( B )
  void xmk_RAM_ptr xmk_RAM_ptr B;
#endif

/*-FDEF E*/
{
  if ( ((SDL_Bit_String *)B)->Length > 0)
  /* GBN 31.07.98 */
  {
#ifdef XMBM_COPIED
    if ( ((SDL_Bit_String *)B)->IsAssigned != XMBM_COPIED )
#endif
      xFree( (void**)&(((SDL_Bit_String *)B)->Bits) );
  }
  /* GBN 31.07.98 */

}
#endif /* ... !defined(XNOUSEOFASN1) && !defined(XNOUSEOFOCTETBITSTRING) */

#endif /* ... __MK_CPU_C_*/
