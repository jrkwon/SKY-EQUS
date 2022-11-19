/*+MHDR*/
/*
# clearcase: CmicroRel2.3


+------------------------------------------------------------------------------+
|  Modulname    : MK_USER.C                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|    This module contains functions, which are to be filled up by the user.    |
|                                                                              |
|    These are :                                                               |
|                                                                              |
|    1.Functions for the environment-handling                                  |
|      xInitEnv ()    - Only if the generated environment env.c is not used!   |
|      xInEnv   ()    - Only if the generated environment env.c is not used!   |
|      xOutEnv  ()    - Only if the generated environment env.c is not used!   |
|      xCloseEnv()    - Only if the generated environment env.c is not used!   |
|                                                                              |
|    2.Functions for central errorhandling                                     |
|      ErrorHandler ()                                                         |
|      xSDLOpError  ()                                                         |
|                                                                              |
|    3.Functions for the Cmicro Kernel (optional in order to optimize)         |
|      xRouteSignal ()                                                         |
|                                                                              |
|    4.Functions to trigger a hardware-Watchdog                                |
|      WatchdogTrigger ()                                                      |
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

/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typ.h"


#ifdef XSYSID 
  /* 
  ** CAUTION ! 
  ** ============ 
  ** This module cannot be used when partitioning is used. The reason 
  ** for this is that the functions of this module operate on global 
  ** variables and partitioned systems may preempt each other when 
  ** an Real Time Operating System is used. 
  ** However, users can use this module as a basis for implementing 
  ** their own environment function.  
  */ 
  #error "ERROR_in_mk_user_c_Module_cannot_be_used_for_Partitioning" 
#endif /* ... XSYSID */ 

#ifdef XMK_NO_GENERATED_ENV
  #error "It_is_recommended_to_include_the_file<systemname>.ifc_here_See_mk_user.c"
#endif

/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/

/*---+---------------------------------------------------------------
     Check automatically generated #ifdef
-------------------------------------------------------------------*/
#if defined(XMK_IFC_TIME) && defined(XMK_CFG_TIME)
  #if XMK_IFC_TIME != XMK_CFG_TIME
    /*
    ** If the following line is compiled/leads to an error during
    ** compilation, then the generated systemname.ifc file does not match with
    ** the generated sdl_cfg.h file. You should newly generate C code
    ** and be sure that each of the generated C files and the symbolfile
    ** is used appropriately
    */
    #error "ERROR_IllegalFileConfiguration_ifc_and_sdl_cfg_file_See_mk_user.c"
  #endif
#endif
 

/*--------------------  Typedefinitions     ----------------------------------*/

/*--------------------  Functions    -----------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/
#ifndef NO_GLOBAL_VARS
  #ifdef XMK_ADD_CQUERY_ERROR
    xmk_T_ERR_NUM xmk_LastStoredError = 0;
  #endif
#endif

/*============================================================================*/
/*-MGG*/


#ifdef XMK_NO_GENERATED_ENV 

#ifdef XMK_USE_xInitEnv

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xInitEnv                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This function is called by the Cmicro Kernel during initialization of the   |
|  SDL-System.                                                                 |
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
   /* ANSI - Style */
   void xInitEnv (void)
#else
   /* Kernighan-Ritchie-Style */
   void xInitEnv ()
#endif

/*-FDEF E*/
{

  #ifdef XMK_ADD_PRINTF_USER
    XMK_FUNCTION("xInitEnv");
  #endif
  
  /*
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  */

      /* ........................................... */
      /* ... Init all interfaces+the environment ... */
      /* ... here .............................. ... */
      /* ........................................... */
  xInitEnv_is_not_filled_yet = 0;

  /* 
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  */ 

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("xInitEnv");
  #endif

} /* END OF FUNCTION */

#endif /* ... XMK_USE_xInitEnv */

#endif /* ... XMK_NO_GENERATED_ENV */
 

#ifdef XMK_NO_GENERATED_ENV 

#ifdef XMK_USE_xInEnv

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xInEnv                                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This function is called by the Cmicro Kernel continuously to retrieve       |
|  signals polled from the Environment.                                        |
|                                                                              |
|  Use the Macro XMK_SEND_ENV() to send signals into the SDL system.           |
|                                                                              |
|  The use of xInEnv() is not absolutely necessary in the case, where          |
|  the Cmicro Kernel is scaled to preemption, and all external Events are      |
|  put into the SDL-System via an Interrupt Service Routine, which is to be    |
|  written by the user.                                                        |
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
   /* ANSI - Style */
   void xInEnv (void)
#else
   /* Kernighan-Ritchie-Style */
   void xInEnv ()
#endif

/*-FDEF E*/
{

  /*
  ** Define some temporary variables for XMK_SEND_ENV here ...
  */
  XMK_SEND_TMP_VARS

  #ifdef XMK_ADD_PRINTF_USER
    XMK_FUNCTION("xInEnv");
  #endif /* ... XMK_ADD_PRINTF_USER */

  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("Environment-Signale are not implemented (xInEnv).\n"));
    PRINTF (("(nothing was read from the environment)\n"));
  #endif /* ... XMK_ADD_PRINTF_USER */

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  **
  ** In the following the identifiers mean:
  ** sender-pid   : the environment use the macro ENV
  ** signal-id    : the pid of the signal. Get it from the file <systemname>.ifc
  ** priority     : in most cases the default priority xDefaultPrioSignal can be used here
  ** parameter-length  :  the length of the parameters in counted in characters
  **                      e.g. use sizeof()
  ** pointer-to-signal-parameters  :  the address of the parameters
  ** receiver-pid : the pid of the receiver process. Get it from the file sdl_cfg.h
  */
 
  if (i_got_a_signal_from_source1)
  {
     /* ........................................... */
     /* ... Set Signal-Parameters ................. */
     /* ........................................... */
     XMK_SEND_ENV(sender-pid,
                  signal-id,
                  priority,
                  parameter-length,
                  pointer-to-signal-parameters,
                  GLOBALPID(receiver-pid,0));

     #ifdef XMK_ADD_PRINTF_USER
           XMK_TRACE_EXIT("xInEnv");
     #endif

     return ;

  }
  else if (i_got_a_signal_from_source2)
  {

     /* ........................................... */
     /* ... Set Signal-Parameters ................. */
     /* ........................................... */
     XMK_SEND_ENV(sender-pid,
                  signal-id,
                  priority,
                  parameter-length,
                  pointer-to-signal-parameters,
                  GLOBALPID(receiver-pid,0));

     #ifdef XMK_ADD_PRINTF_USER
           XMK_TRACE_EXIT("xInEnv");
     #endif

     return ;
  }
  else
  {

     #ifdef XMK_ADD_PRINTF_USER
           XMK_TRACE_EXIT("xInEnv");
     #endif

     return;
  }

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E 
  */

} /* END OF FUNCTION */

#endif /* ... XMK_USE_xInEnv */

#ifdef XMK_USE_xOutEnv

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xOutEnv                                                      |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|                 This function is called by the Cmicro Kernel, if an SDL      |
|                 signal is to be sent to the environment.                     |
|                                                                              |
|                 The user must take care for that for each possible signal    |
|                 there is an appropriate handling. If a signal was handled,   |
|                 the function should return with XMK_TRUE, otherwise XMK_FALSE|
|                                                                              |
|                 Notes:                                                       |
|                 ------                                                       |
|               1.xOutEnv is called only, and only if a signal is to be send   |
|                 to the SDL environment. In earlier Cmicro KLernel versions,  |
|                 each SDL signal was passed through xOutEnv, even internal    |
|                 signals of the system. This was confusing and does not longer|
|                 happen.                                                      |
|               2.The user has several possibilities to send signals to the    |
|                 Environment by using #EXTSIG,#ALT or #CODE, please view      |
|                 the reference manual.                                        |
|                                                                              |
|  Parameter    :  xmk_TmpSignalID        - ID of the signal (see .ifc file)   |
|                  xmk_TmpPrio            - Priority of signal                 |
|                  xmk_TmpDataLength      - Amount of bytes in signal's        |
|                                           parameters                         |
|                 *xmk_TmpDataPtr         - Pointer to signal's parameters     |
|                  xmk_TmpReceiverPID     - PID of the receiver.               |
|                                                                              |
|  Return       : Function must return with XMK_TRUE, if Signal was sent       |
|                 to the environment, otherwise with XMK_FALSE.                |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
  xmk_OPT_INT   xOutEnv (xmk_T_SIGNAL xmk_TmpSignalID
                         #ifdef XMK_USE_SIGNAL_PRIORITIES
                          , xmk_T_PRIO xmk_TmpPrio
                         #endif
                         #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                          , xmk_T_MESS_LENGTH xmk_TmpDataLength,
                           void xmk_RAM_ptr  xmk_TmpDataPtr
                         #endif
                         #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                          , xPID xmk_TmpReceiverPID
                         #endif
                         )

#else
   /* Kernighan-Ritchie-Style */
         xmk_OPT_INT xOutEnv  (xmk_TmpSignalID
                               #ifdef XMK_USE_SIGNAL_PRIORITIES
                               ,  xmk_TmpPrio
                               #endif
                               #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                               , xmk_TmpDataLength,
                                 xmk_TmpDataPtr
                               #endif
                               #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                               , xmk_TmpReceiverPID
                               #endif
                               )
                               xmk_T_SIGNAL       xmk_TmpSignalID;
                               #ifdef XMK_USE_SIGNAL_PRIORITIES
                                 xmk_T_PRIO         xmk_TmpPrio;
                               #endif
                               #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                                 xmk_T_MESS_LENGTH  xmk_TmpDataLength;
                                 void xmk_RAM_ptr  xmk_TmpDataPtr;
                               #endif
                               #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                                 xPID               xmk_TmpReceiverPID;
                               #endif
#endif

/*-FDEF E*/
{
  xmk_OPT_INT xmk_TmpResult = XMK_FALSE;

  #ifdef XMK_ADD_PRINTF_USER
    XMK_FUNCTION("xOutEnv");
  #endif

  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("signal_id = %d\n", xmk_TmpSignalID));
    #ifdef XMK_USE_SIGNAL_PRIORITIES
      PRINTF (("prio      = %d\n", xmk_TmpPrio));
    #endif

    #ifdef XMK_USED_SIGNAL_WITH_PARAMS
      PRINTF (("data_len  = %d\n", xmk_TmpDataLength));
    #endif
  #endif /* ... XMK_ADD_PRINTF_USER */

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E
  */

  /*
  ** Please insert appropriate code here to send
  ** signals to the environment ...
  */
  switch (xmk_TmpSignalID)
  {
     case SDL_Signal1 :
        /* ........................................... */
        /* IMPORTANT NOTE                              */
        /* ===============                             */
        /* The parameters of the signal to be sent to  */
        /* the environment are TO BE COPIED !          */
        /* Do not use the pointer xmk_TmpDataPtr to transfer   */
        /* data, because the memory block, where it    */
        /* points to, is undefined after returning     */
        /* from xOutEnv () ! See generated Code and    */
        /* mk_outp for more details.                   */
        /* Include the generated file <systemname>.ifc */
        /* ........................................... */
        xmk_TmpResult = XMK_TRUE; /* to tell the caller that */
                       /* signal is consumed      */
        break ;


     case SDL_Signal2 :
        /* ........................................... */
        /* ........................................... */
        /* ........................................... */
        xmk_TmpResult = XMK_TRUE; /* to tell the caller that */
                       /* signal is consumed      */
        break ;

     default :
        /* Please do not modify the following, because */
        /* it is used for error checking.              */
        xmk_TmpResult = XMK_FALSE;
        break ;
  }

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  */


  #ifdef XMK_USE_SIGNAL_PRIORITIES
    XMK_SUPPRESS(xmk_TmpPrio);
  #endif

  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
    XMK_SUPPRESS(xmk_TmpDataLength);
    XMK_SUPPRESS(xmk_TmpDataPtr);
  #endif

  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    XMK_SUPPRESS(xmk_TmpReceiverPID);
  #endif

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("xOutEnv");
  #endif

  return (xmk_TmpResult);

} /* END OF FUNCTION */

#endif /* ... XMK_USE_xOutEnv */

#endif /* ... XMK_NO_GENERATED_ENV */


#ifdef XMK_NO_GENERATED_ENV

#ifdef XMK_USE_xCloseEnv

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xCloseEnv                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This function is called by the Cmicro Kernel after termination.             |
|  It is not that useful in a Cmicro controler application, but may be so in   |
|  a simulation with the Cmicro Package.                                       |
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
   /* ANSI - Style */
   void xCloseEnv (void)
#else
   /* Kernighan-Ritchie-Style */
   void xCloseEnv ()
#endif

/*-FDEF E*/
{

  #ifdef XMK_ADD_PRINTF_USER
    XMK_FUNCTION("xCloseEnv");
  #endif

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  */

      /* ........................................... */
      /* ... Close all interfaces+the environment... */
      /* ... here .............................. ... */
      /* ........................................... */

  xCloseEnv_is_not_filled_yet = 0;

  /*
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  ** P L E A S E _ M O D I F Y _ T H E _ P R E V I O U S _ C O D E
  */

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("xCloseEnv");
  #endif

} /* END OF FUNCTION */

#endif /* ... XMK_USE_xCloseEnv */

#endif /* ... XMK_NO_GENERATED_ENV */


#if defined(XECSOP)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xSDLOpError                                                  |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This is a function which is to filled up by the user. The function is an    |
|  central error handling function for ADTs. It is compiled only if XECSOP     |
|  was defined in ml_mcf.h.                                                    |
|                                                                              |
|  Parameter    :                                                              |
|            char *xmk_String1,     - the SDL ADT operators name plus detail   |
|            char *xmk_String2,     - the reason for failure                   |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   void xSDLOpError ( char * xmk_String1, char *xmk_String2 )
#else
   /* Kernighan-Ritchie-Style */
   void xSDLOpError ( xmk_String1, xmk_String2 )
   char * xmk_String1;
   char * xmk_String2;
#endif

/*-FDEF E*/
{

  #ifdef XMK_ADD_PRINTF_USER
      XMK_FUNCTION("xSDLOpError");
  #endif

  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("ERR:IN ADT:%s\n%s\n", xmk_String1, xmk_String2));
  #endif

  ErrorHandler (ERR_N_PREDEFINED_OPERATOR_CALL);  

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("xSDLOpError");
  #endif

} /* END OF FUNCTION */

#endif /* #if defined(XECSOP) */


#ifndef XMK_USE_NO_ERR_CHECK


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : ErrorHandler                                                 |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|  This is a function which is to filled up by the user. The function is the   |
|  central ErrorHandler. The Cmicro Kernel, Cmicro Tester as well as the       |
|  SDL-Application are the main clients of this function.                      |
|                                                                              |
|  Parameter    :                                                              |
|            xmk_T_ERR_NUM xmk_TmpErrorNumber   -   given error number         |
|                                   (see headerfile *err.h)                    |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   void ErrorHandler ( xmk_T_ERR_NUM xmk_TmpErrorNumber )
#else
   /* Kernighan-Ritchie-Style */
   void ErrorHandler ( xmk_TmpErrorNumber )
   xmk_T_ERR_NUM xmk_TmpErrorNumber;
#endif

/*-FDEF E*/
{
/*  Added by GBU,990516,    to get rid of IAR warning */
    xmk_TmpErrorNumber = xmk_TmpErrorNumber;

   #ifdef XMK_ADD_PRINTF_USER
      XMK_FUNCTION("ErrorHandler");
   #endif

   #ifdef XMK_ADD_CQUERY_ERROR
     /* Variable stores the last errornumber for the command interface ... */
     xmk_LastStoredError = xmk_TmpErrorNumber;
   #endif

  #ifdef XMK_ADD_TSYS_ERROR
     /*
     ** Trace of Errors via communication interface ...
     ** Note : Not all error situations can be traced
     **        (i.e.an error on the communication interface)
     */
     if(xmk_TmpErrorNumber !=  ERR_N_RING_WRITE_LENGTH)
       xmk_tsys_error ( xmk_TmpErrorNumber );
  #endif

  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("ERR:xmk_TmpErrorNumber=%d\n", xmk_TmpErrorNumber));

    #if defined(XMK_ADD_PRINTF_ERROR)
        xmk_err_text(STDOUT, xmk_TmpErrorNumber);
    #endif

    PRINTF (("ERR:xmk_TmpErrorNumber=%d\n", xmk_TmpErrorNumber));
  #else

    #ifdef USE_THE_FOLLOWING_EXAMPLE
     /* I.E.implement the 3 functions ECERROR, ECWARNING and ECINFORMATION */
     /* The mapping between different error situations and the reaction on */
     /* it strongly depends on the target hardware architecture, i.e.if it */
     /* has any output device or not.                                      */
     switch (xmk_TmpErrorNumber)
     {
        case ERR_N_UNDEFINED_ERROR          : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_CREATE_NO_MEM            : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_CREATE_INSTANCE          : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_SDL_DISCARD              : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_DIVIDE_BY_ZERO           : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_EMPTY_CHARSTRING         : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_ERROR_CHARSTRING         : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_FATAL_CHARSTRING         : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_NO_FREE_SIGNAL           : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_PARAMETER_MEM_ALLOC      : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_INDEX_SIGNAL             : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_SYSTEM_SIGNAL            : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_NO_CONT_PRIO             : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_NO_COR_PRIO              : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_xRouteSignal             : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_NO_REC_AVAIL             : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_NO_FREE_TIMER            : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_PID_INDEX                : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_SEND_TO_NULLPID          : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_TRANS_TIME               : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_xOutEnv                  : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_ILLEGAL_CMD              : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_TESTER_MESSAGE           : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_CHKSUM              : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_DEST_BUFFER         : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_ENC_MEMORY          : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_ENC_LENGTH          : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_NOT_IMPL            : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_LINK_SYNC                : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_DATA_LINK                : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_DECODE_METHOD            : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_RING_OVERFLOW            : ECWARNING (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_INIT_QUEUE               : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_PREDEFINED_OPERATOR_CALL : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_INIT_SDL_MEM             : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_MEM_PARAM                : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_MEM_NO_FREE              : ECERROR   (xmk_TmpErrorNumber);
                                              break;
        case ERR_N_MEM_ILLMBLOCK            : ECERROR   (xmk_TmpErrorNumber);
                                              break;

        default                             : ECERROR   (ERR_N_UNKNOWN);
                                              break;
      }
    #endif /* ... USE_THE_FOLLOWING_EXAMPLE */

  #endif

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("ErrorHandler");
  #endif

} /* END OF FUNCTION */


#endif /* ...  ! XMK_USE_NO_ERR_CHECK */

#ifndef XMK_USE_RECEIVER_PID_IN_SIGNAL

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xRouteSignal                                                 |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|                 This function is called by the Cmicro Kernel, if SDL signals |
|                 have no receiver.                                            |
|                 (undef  XMK_USE_RECEIVER_PID_IN_SIGNAL)                      |
|                 This might be useful in very small systems in order to spare |
|                 some RAM memory.                                             |
|                 The following restrictions apply :                           |
|                   * The SDL System must not contain anything other than the  |
|                     following process declaration. (x,1)                     |
|                   * For each Signal in the SDL System, there is to be only   |
|                     one Receiver process (no signal may be sent to more than |
|                     one processtype).                                        |
|                   * no dynamic process creation is used                      |
|                     (Create-Symbol is not used)                              |
|                                                                              |
|                 Note, timers are represented as signals, and that this       |
|                 xRouteSignal - function also has to map timers to Receiver   |
|                 Processes.                                                   |
|                                                                              |
|                 Important Rule : Each Signal- and Timer is represented by    |
|                 a systemwide unique integer number.                          |
|                                                                              |
|  Parameter    : xmk_T_SIGNAL  - xmk_TmpSignalID                              |
|                                                                              |
|  Return       : xPID  Process - PID or xNULLPID, if no receiver is defined   |
|                                     for the given signal.                    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO

   /* ANSI - Style */
   xPID xRouteSignal  ( xmk_T_SIGNAL xmk_TmpSignalID )

#else

   /* Kernighan-Ritchie-Style */
   xPID xRouteSignal ( xmk_TmpSignalID )
   xmk_T_SIGNAL       xmk_TmpSignalID;

#endif

/*-FDEF E*/
{


  /*
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  ** P L E A S E _ M O D I F Y _ T H E _ F O L L O W I N G _ C O D E 
  */

   /*
   ** Please insert appropriate code here to map Signal ID's
   ** to Process - Type - ID's (XPTID_ProcessName in generated code).
   ** Keep in mind that this function might be called from within
   ** a critical path.
   ** Include <systemname>.ifc to get signal names and process' XPTID
   */
   switch (xmk_TmpSignalID)
   {
      /*
      **  S D L   T i m e r s ...
      */
      case SDL_Timer1  : return (XPTID_ProcessName)         break;
      case SDL_Timer2  : return (XPTID_ProcessName)         break;
      case SDL_Timer3  : return (XPTID_ProcessName)         break;
      case SDL_TimerN  : return (XPTID_ProcessName)         break;

      /*
      **  O r d i n a r y  S D L   S i g n a l s ...
      */
      case SDL_Signal1 : return (XPTID_ProcessName)         break;
      case SDL_Signal2 : return (XPTID_ProcessName)         break;
      case SDL_Signal3 : return (XPTID_ProcessName)         break;
             ......
      case SDL_SignalN : return (XPTID_ProcessName)         break;

      default          : ErrorHandler (ERR_N_NO_RCV); 
                         return (xNULLPID)                  break;
   }


} /* END OF FUNCTION */

#endif /* ... XMK_USE_RECEIVER_PID_IN_SIGNAL */




#ifdef XMK_USE_KERNEL_WDTRIGGER

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : WatchdogTrigger                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description :                                                               |
|                 If scaled so, this function is called by the Cmicro Kernel   |
|                 each time an SDL-transition is executed.                     |
|                                                                              |
|                 ** CAUTION **                                                |
|                 -------------------                                          |
|                 Be sure, that the timeout used for the Watchdog is longer    |
|                 than the longest SDL-Transition (in the case of non preemp-  |
|                 tive Cmicro Kernel). If the Cmicro Kernel is scaled to       |
|                 work preemptive, then the Watchdog Trigger should not be     |
|                 used, because the execution time of transitions can never    |
|                 be calculated.                                               |
|                                                                              |
|                                                                              |
|                                                                              |
|  Parameter    :                                                              |
|                 void                                                         |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
   /* ANSI - Style */
   void WatchdogTrigger ( void )
#else
   /* Kernighan-Ritchie-Style */
   void WatchdogTrigger ( )
#endif

/*-FDEF E*/
{

  #ifdef XMK_ADD_PRINTF_USER
    XMK_FUNCTION("WatchdogTrigger");
  #endif /* ... XMK_ADD_PRINTF_USER */

   /* ........................................... */
   /* ........................................... */
   /* ........................................... */

  #ifdef XMK_ADD_PRINTF_USER
    XMK_TRACE_EXIT("WatchdogTrigger");
  #endif /* ... XMK_ADD_PRINTF_USER */


} /* END OF FUNCTION */

#endif /* XMK_USE_KERNEL_WDTRIGGER */
