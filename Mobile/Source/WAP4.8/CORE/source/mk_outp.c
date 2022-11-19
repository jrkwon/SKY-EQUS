/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Modulname    : MK_OUTP.C                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This Module exports functions which represent                               |
|                                                                              |
|      - the SDL-Output-Symbol.                                                |
|      - a non SDL-like Broadcast mechanism                                    |
|                                                                              |
|  In order to scale the Cmicro Kernel, more than one C-function representing  |
|  SDL-Output is available.                                                    |
|                                                                              |
|  For scaling facilities see manual.                                          |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|    ERR_N_SEND_TO_NULLPID       Errorcheck is conditionally compiled          |
|    ERR_N_PID_INDEX             Errorcheck is conditionally compiled          |
|    ERR_N_NO_REC_AVAIL          Errorcheck is conditionally compiled          |
|    ERR_N_xOutEnv                                                             |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    ...  xmk_SendSimple ( xmk_T_SIGNAL sig, argument list depends on defines )|
|    ...  xmk_Send       ( xmk_T_SIGNAL sig, argument list depends on defines )|
|    xPID xmk_Determine_Receiver ( unsigned char proc_type )                   |
|                                                                              |
|  M3 Static functions of this module :                                        |
|  --------------------------------------------------------                    |
|    - none -                                                                  |
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

#ifndef  __MK_OUTP_C_
#define  __MK_OUTP_C_

/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typw.h"

/*--------------------  Variables    -----------------------------------------*/

#ifndef NO_GLOBAL_VARS
  /*    Modified by GBU,990518 */
  extern XCONST XPDTBL xmk_ROM_ptr xPDTBL[MAX_SDL_PROCESS_TYPES+1];

  #ifdef XMK_ADD_REALTIME_PLAY
    extern xmk_T_TIME xmk_StartupTime ;
  #endif
#endif

/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/
/*
** Have to differentiate between output for
** timer or ordinary SDL signal
*/
#define OUTPUT_IS_NOT_A_TIMER     (sig > MAX_SDL_TIMER_TYPES)
#define OUTPUT_IS_A_TIMER         (sig <= MAX_SDL_TIMER_TYPES)

/*
** Make xmk_Send functions using a return value or not
** This is only necessary for some testing purposes.
** The functions performs all possible error checks and
** calls the ErrorHandler() appropriately ...
*/
#define XMK_USE_NO_RET_VAL

#ifdef  XMK_USE_NO_RET_VAL
  /*
  ** xmk_Send returns no value
  */
  #define RETDCL       void
  #define RETURN(val)  return
#else
  /*
  ** xmk_Send returns a value
  */
  #define RETDCL       int
  #define RETURN(val)  return (val)
#endif

#ifdef XMK_ADD_PRINTF_ADDITIONAL
  #define STDOUT_WRITE_SIG(sig) \
          PRINTF (("xmk_Send()#Signal-ID  : %d\n", sig));

  #ifdef XMK_USE_SIGNAL_PRIORITIES
    #define STDOUT_WRITE_PRIO(prio) \
            PRINTF (("xmk_Send()#Signal-Prio: %d\n", prio));
  #else
    #define STDOUT_WRITE_PRIO(prio)
  #endif

  #define STDOUT_WRITE_SEND(send) \
      xxmonPID ("xmk_Send()#Sender     :", send);

  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    #define STDOUT_WRITE_REC(rec) \
        xxmonPID ("xmk_Send()#Receiver   :", rec);
  #else
    #define STDOUT_WRITE_REC(rec)
  #endif

  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
    #define STDOUT_WRITE_DATA(len,pdata) \
      PRINTF (("xmk_Send()#Signal-len : %d\n", len)); \
      if (len != 0)  \
      { \
        PRINTF (("xmk_Send()#Signal with parameters\n")); \
        xxmonhexasc (STDOUT,"Unformatted Parameterarea:\n", "", pdata, (int) len); \
      } \
      else \
      { \
        PRINTF (("xmk_Send()#Signal w/o parameters\n")); \
      }
  #else
    #define STDOUT_WRITE_DATA(len,pdata)
  #endif

#endif /* ... XMK_ADD_PRINTF_ADDITIONAL */

/*
** Automatic and manual scaling possibilities
** -- internal definitions --
*/
#ifdef XMK_USE_SIGNAL_PRIORITIES
  #define XCOND_PRIO(prio)          prio
#else
  #define XCOND_PRIO(prio)          XMK_DUMMYPRIO
#endif

#ifdef XMK_USE_SIGNAL_TIME_STAMP
  #define XCOND_TIMESTAMP(stamp)    stamp
#else
  #define XCOND_TIMESTAMP(stamp)    0
#endif

#ifdef XMK_USED_SIGNAL_WITH_PARAMS
  #define XCOND_DATA_LEN(data_len)  data_len
  #define XCOND_DATA(p_data)        p_data
#else
  #define XCOND_DATA_LEN(data_len)  0
  #define XCOND_DATA(p_data)        (void *) NULL
#endif

#ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
  #define XCOND_RECEIVER(Receiver)  Receiver
#else
  #define XCOND_RECEIVER(Receiver)  xRouteSignal(sig)
#endif

/*
** Switch off any error check
*/
#ifdef XMK_USE_NO_ERR_CHECK
  #define ErrorHandler(errnum)
#endif 

/*--------------------  Typedefinitions     ----------------------------------*/

/*--------------------  Functions   ------------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/


/*============================================================================*/
/*-MGG*/


#ifdef XMK_USE_xmk_SendSimple

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_SendSimple                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This is a quite simple SDL-Output Function, which needs only 2 Parameters   |
|  if called. Most SDL-Systems consist of a lot of "normal" Signals without    |
|  any Parameters and no priority. In most cases it makes sense to use         |
|  this simple Function, but if signal priorities are defined, these get lost. |
|                                                                              |
|  The Signal is put into the linked list of Signals by using a default-       |
|  Priority.                                                                   |
|                                                                              |
|  Parameter    : sig          - Signal's code                                 |
|                 Receiver     - PID of the receiver-process                   |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  /*
  ** Use ANSI Style
  */
  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    void xmk_SendSimple ( xmk_T_SIGNAL sig, xPID Receiver )
  #else
    void xmk_SendSimple ( xmk_T_SIGNAL sig )
  #endif

#else
  /*
  ** Use K/R Style
  */
  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    void xmk_SendSimple ( sig, Receiver )
    xmk_T_SIGNAL          sig;
    xPID                  Receiver;
  #else
    void xmk_SendSimple ( sig )
    xmk_T_SIGNAL          sig;
  #endif
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_FUNCTION("xmk_SendSimple");
  #endif

  #ifdef XMK_USE_SIGNAL_PRIORITIES
    #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
      /*
      ** A receiver PID is stored in each signal header ...
      */
      XMK_SF (sig, xDefaultPrioSignal, 0, (void *) NULL, Receiver);
    #else
      /*
      ** No receiver PID is stored in any signal header ...
      */
      XMK_SF (sig, xDefaultPrioSignal, 0, (void *) NULL, 0);
    #endif
  #else
     #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
      /*
      ** A receiver PID is stored in each signal header ...
      */
      XMK_SF (sig, XMK_DUMMYPRIO, 0, (void *) NULL, Receiver);
    #else
      /* 
      ** No receiver PID is stored in any signal header ...
      */
      XMK_SF (sig, XMK_DUMMYPRIO, 0, (void *) NULL, 0);
    #endif 
  #endif /* ...XMK_USE_SIGNAL_PRIORITIES*/

  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_TRACE_EXIT("xmk_SendSimple");
  #endif

} /* END OF FUNCTION */

#endif /* ... XMK_USE_xmk_SendSimple */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Send                                                     |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function represents the basic SDL-Output-function with all necessary   |
|  parameters. The <signal-instance> to be created is put into the             |
|  <input-port> of the receiving process.                                      |
|  Normally, signals are transferred by using a signalheader plus an area of   |
|  fixed length containing the <signal-parameter>. If the length of parameters |
|  for a signal exceeds the length of this "default"-area, a memory block      |
|  is allocated for the signal-parameters to be transferred.                   |
|  If there is not enough memory, the ErrorHandler is called.                  |
|                                                                              |
|  Conditionally compiled error-Check :                                        |
|  If the receiver-PID is a NULL-PID, the ErrorHandler is called.              |
|                                                                              |
|  Parameter    : sig          - signal's code                                 |
|                 prio         - signal's priority                             |
|                 data_len     - number of bytes of the signal's parameter     |
|                 p_data       - pointer to dynamically allocated memory area  |
|                                containing the signal's parameter             |
|                                (NULL if parameter contained within signal)   |
|                 Receiver     - PID of the receiver-process                   |
|                                                                              |
|  Return       : 0 - no error (o.k.)                                          |
|                 1 - no more signals left free                                |
|                 2 - no more memory allocatable                               |
|                 3 - semantic error (cannot send to NULL-PID, ..)             |
|                 4 - no error - signal was sent to environment                |
|                 5 - Indexing error in receiver - PID                         |
|                 6 - System is suspended                                      |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO

         RETDCL xmk_Send(xmk_T_SIGNAL sig

                         #ifdef XMK_USE_SIGNAL_PRIORITIES
                          , xmk_T_PRIO prio
                         #endif

                         #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                          , xmk_T_MESS_LENGTH data_len,
                           void               xmk_RAM_ptr p_data
                         #endif

                         #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                          , xPID Receiver
                         #endif
                         )

#else

         RETDCL xmk_Send (sig

                          #ifdef XMK_USE_SIGNAL_PRIORITIES
                          ,  prio
                          #endif

                          #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                          , data_len,
                            p_data
                          #endif

                          #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                          , Receiver
                          #endif

                          )

                          xmk_T_SIGNAL       sig;

                          #ifdef XMK_USE_SIGNAL_PRIORITIES
                            xmk_T_PRIO         prio;
                          #endif

                          #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                            xmk_T_MESS_LENGTH  data_len;
                            void               xmk_RAM_ptr p_data;
                          #endif

                          #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                            xPID               Receiver;
                          #endif

#endif

/*-FDEF E*/
{
  X_REGISTER xmk_T_MESSAGE  xmk_RAM_ptr P_Signal;  /* Pointer to signal             */

  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
    X_STATIC   void   xmk_RAM_ptr P_Parameter     ;  /* Pointer to signal's parameter */
  #endif

  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    xmk_T_TIME timestamp            ;  /* set signal's timestamp        */
  #endif

  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_FUNCTION("xmk_Send");
  #endif

  #ifdef XMK_USE_DEBUGGING
    if (xmk_SystemState & XMK_SYSTEM_SUSPENDED)
    {
      #ifdef XMK_ADD_PRINTF_OUTPUT
        XMK_TRACE_EXIT("xmk_Send");
      #endif
      RETURN(6);
    }
  #endif

  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    STDOUT_WRITE_SIG(sig)              ;
    STDOUT_WRITE_PRIO(prio)            ;
    if (OUTPUT_IS_NOT_A_TIMER)
    {
      STDOUT_WRITE_SEND(xRunPID)       ;
    }
    else
    {
      STDOUT_WRITE_SEND( XCOND_RECEIVER( Receiver ) ) ;
    }
    STDOUT_WRITE_REC( XCOND_RECEIVER( Receiver) )     ;
    STDOUT_WRITE_DATA(data_len,p_data) ;
  #endif

  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    #ifdef XMK_ADD_REALTIME_PLAY
      timestamp   = xmk_NOW() - xmk_StartupTime ;
    #else
      timestamp   = xmk_NOW()         ;
    #endif  
  #endif

  /*
  **   Check if correct Receiver
  **   (unnecessary of using SDT-Analyzer)
  */
  #ifdef XMK_USE_MAX_ERR_CHECK
    if (Receiver == xNULLPID)
    {
      ErrorHandler (ERR_N_SEND_TO_NULLPID);
      #ifdef XMK_ADD_PRINTF_OUTPUT
        XMK_TRACE_EXIT("xmk_Send");
      #endif
      RETURN (3);
    }
  #endif

  #ifdef XMK_ADD_TOUTPUT
    /*
    **   Outputtrace and redirecting the
    **   signal, if necessary ....
    */
    if ( xmk_TSDL_Output ( sig                         ,
                           XCOND_PRIO      (prio)      ,
                           XCOND_TIMESTAMP (timestamp) ,
                           XCOND_DATA_LEN  (data_len)  ,
                           XCOND_DATA      (p_data)    ,
                           XCOND_RECEIVER  (Receiver)  ) != XMK_OKAY )
    {
      /*
      ** and if something was wrong with the
      ** signal (parameters), then discard the signal ...
      */
      #ifdef XMK_ADD_PRINTF_OUTPUT
        XMK_TRACE_EXIT("xmk_Send");
      #endif
      RETURN (2);
    }
  #endif /* ... XMK_ADD_TOUTPUT */

  /*
  ** Send to environment ?
  */
  if ( XCOND_RECEIVER( Receiver ) == ENV)
  {
    #ifdef XMK_USE_xOutEnv
      /*
      ** Direct signal to the environment
      */
      if ( xOutEnv (sig
                    #ifdef XMK_USE_SIGNAL_PRIORITIES
                      , prio
                    #endif
                    #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                      , data_len, p_data
                    #endif
                    #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                      , Receiver
                    #endif
                    ) == XMK_TRUE )
      {
        /* 
        ** Signal was correctly sent to the Environment 
        */
        #ifdef XMK_ADD_PRINTF_OUTPUT
          XMK_TRACE_EXIT("xmk_Send");
        #endif
        RETURN (4);
      }
      else
    #endif /*... XMK_USE_xOutEnv */
      {
        /* 
        ** Signal was not consumed in the Environment,
        ** this is an error !
        */
        ErrorHandler (ERR_N_xOutEnv);
        #ifdef XMK_ADD_PRINTF_OUTPUT
          XMK_TRACE_EXIT("xmk_Send");
        #endif
        RETURN (4);
      } /* ... if ( xOutEnv (sig ... */
  } /* ...   if ( XCOND_RECEIVER( Receiver ) == ENV) */

  /*
  ** Signal not to be sent to the environment
  */
  #ifdef XMK_USE_BROADCAST_OUTPUT
    if (sig == BROADCAST)
      xmk_DistributeSignal (sig
                            #ifdef XMK_USE_SIGNAL_PRIORITIES
                            , prio
                            #endif
                            #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                            , data_len, p_data
                            #endif
                            );
  #endif /* ... XMK_USE_BROADCAST_OUTPUT */

  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    #ifdef XMK_USE_MAX_ERR_CHECK
      if (! xmk_IfExist (Receiver))
      {
        ErrorHandler(ERR_N_PID_INDEX);
        #ifdef XMK_ADD_PRINTF_OUTPUT
          XMK_TRACE_EXIT("xmk_Send");
        #endif

        RETURN (5);
      }
    #endif /* ... XMK_USE_MAX_ERR_CHECK */
  #endif /* ... XMK_USE_RECEIVER_PID_IN_SIGNAL */

  /*
  ** Allocate signal. If there is no free signal 
  ** instance left, return with an error code
  */
  P_Signal = xmk_AllocSignal();
  if (P_Signal == NULL)
  {
        ErrorHandler (ERR_N_NO_FREE_SIGNAL);
        #ifdef XMK_ADD_PRINTF_OUTPUT
          XMK_TRACE_EXIT("xmk_Send");
        #endif

    RETURN (1);
  }

  P_Signal->signal      = sig         ; /* set signal's code                  */
  #ifdef XMK_USE_SIGNAL_PRIORITIES
    P_Signal->prio        = prio      ; /* set signal's priority              */
  #endif
  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    P_Signal->timestamp   = timestamp ; /* set signal's timestamp             */
  #endif
  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
     P_Signal->mess_length = data_len ; /* set length of signal's parameter   */
  #endif
  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    P_Signal->rec         = Receiver  ; /* set signal's receiver              */
  #endif

  #ifdef XMK_USE_SENDER_PID_IN_SIGNAL
    #if defined(XMK_USED_TIMER) && defined(XMK_USED_SENDER)
      if (OUTPUT_IS_A_TIMER)
      {
        P_Signal->send      = Receiver  ; /* set signal's sender           */
      }
      else
    #endif
    {
      P_Signal->send        = xRunPID   ; /* set signal's sender           */
    }
   #endif

  /*
  **  if number of bytes of the signal's parameter is greater than
  **  the number of bytes contained in the signal
  */
#ifdef XMK_USED_SIGNAL_WITH_PARAMS
  if (data_len > XMK_MSG_BORDER_LEN)
  {
    /*
    ** Allocate memory area
    */
    P_Parameter = (xmk_T_MESSAGE xmk_RAM_ptr ) xAlloc (data_len);

    /*
    ** if memory allocation failed, free signal and return errorcode
    */
    if (P_Parameter == NULL)
    {
      ErrorHandler (ERR_N_PARAMETER_MEM_ALLOC);
      xmk_FreeSignal (P_Signal);
          #ifdef XMK_ADD_PRINTF_OUTPUT
            XMK_TRACE_EXIT("xmk_Send");
          #endif

      RETURN (2);
    }

    /*
    ** copy parameter into the allocated memory area
    */
    (void)memcpy (P_Parameter, p_data, data_len); /* accept cast problem in cast from u_int to int. */

    /*
    **  store pointer to memory area in signal
    */
    P_Signal->ParUnion.ParPtr = P_Parameter;
  }

  /*
  ** if parameters can be stored within the signal itself
  */
  else
  {
    if( data_len > 0 )
    {
      /*
      ** copy the parameters into the signal
      */
      (void)memcpy ((void xmk_RAM_ptr ) (P_Signal->ParUnion.ParCopy), (void xmk_RAM_ptr) p_data,
                XMK_MSG_BORDER_LEN);

    } /* END IF */
  } /* END ELSE */

#endif /* ... XMK_USED_SIGNAL_WITH_PARAMS */


  /*
  ** Insert signal into queue ...
  */
  xmk_InsertSignal (P_Signal);

  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_TRACE_EXIT("xmk_Send");
  #endif

  /*
  ** RETURN (0) means o.k.
  */
  RETURN (0);

} /* END OF FUNCTION */

#ifndef XMK_USED_ONLY_X_1

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_Determine_Receiver                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is used in the context of SDL output in generated C code.     |
|  This function determines an instance of the given type to be available as   |
|  a signal's receiver. If no instance can be found, it returns xNULLPID.      |
|                                                                              |
|  Parameter    : proc_type    - process type to be checked for a receiver     |
|                                                                              |
|  Return       : global PID of a found receiver, xNULLPID if none is found    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xPID xmk_Determine_Receiver ( xmk_T_PROCESS proc_type )
#else
  xPID xmk_Determine_Receiver ( proc_type )
  xmk_T_PROCESS proc_type;
#endif

/*-FDEF E*/

{
  X_REGISTER unsigned char i;

  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_FUNCTION("xmk_Determine_Receiver");
  #endif

  /*
  **   for all the instances of the given processtype
  */
  for (i=0; i < xPDTBL[proc_type]->MaxInstances; i++)
  {
    /*
    **    if the instance isn't in state XDORMANT, i.e. active
    */
    if ((xPDTBL[proc_type])->ProcessStateTable[i] != XDORMANT)
    {
      #ifdef XMK_ADD_PRINTF_OUTPUT
        XMK_TRACE_EXIT("xmk_Determine_Receiver");
      #endif

      /*
      **     return global PID of this instance
      */
      return(GLOBALPID(proc_type, i));
    }
  }

  /*
  **    fatal error: no instance can receive a signal
  */
  ErrorHandler(ERR_N_NO_REC_AVAIL);

  #ifdef XMK_ADD_PRINTF_OUTPUT
    XMK_TRACE_EXIT("xmk_Determine_Receiver");
  #endif

  /*
  **   return xNULLPID to enable caller to detect there is no receiver
  */
  return(xNULLPID);
}

#endif /* ... XMK_USED_ONLY_X_1 */

#ifdef XMK_USE_EXPANDED_KERNEL

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_EnvSend                                                  |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function is an alternative to the standard-function.                   |
|                                                                              |
|  The main difference is that it is impossible to direct a signal to the      |
|  xOutEnv function.                                                           |
|                                                                              |
|  This function represents the basic SDL-Output-function with all necessary   |
|  parameters. The <signal-instance> to be created is put into the             |
|  <input-port> of the receiving process.                                      |
|                                                                              |
|  Normally, signals are transferred by using a signalheader plus an area of   |
|  fixed length containing the <signal-parameter>. If the length of parameters |
|  for a signal exceeds the length of this "default"-area, a memory block      |
|  is allocated for the signal-parameters to be transferred.                   |
|  If there is not enough memory, the ErrorHandler is called.                  |
|                                                                              |
|  CAUTION !!!!!                                                               |
|  ================                                                            |
|      This function must only be used if the compiler in use does not support |
|      reentrant functions.                                                    |
|      It is the user's duty to set all the function's parameters correctly,   |
|      There is no errorcheck performed.                                       |
|                                                                              |
|  Parameter    : sig          - signal's code                                 |
|                 prio         - signal's priority                             |
|                 data_len     - number of bytes of the signal's parameter     |
|                 p_data       - pointer to dynamically allocated memory area  |
|                                containing the signal's parameter             |
|                                (NULL if parameter contained within signal)   |
|                 Receiver     - PID of the receiver-process                   |
|                                                                              |
|  Return       : 0 - no error (o.k.)                                          |
|                 1 - no more signals left free                                |
|                 2 - no more memory allocatable                               |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO

         RETDCL xmk_EnvSend(xmk_T_SIGNAL sig

                         #ifdef XMK_USE_SIGNAL_PRIORITIES
                          , xmk_T_PRIO prio
                         #endif

                         #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                          , xmk_T_MESS_LENGTH data_len,
                           void               xmk_RAM_ptr p_data
                         #endif

                         #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                          , xPID Receiver
                         #endif
                         )

#else

         RETDCL xmk_EnvSend (sig

                          #ifdef XMK_USE_SIGNAL_PRIORITIES
                          ,  prio
                          #endif

                          #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                          , data_len,
                            p_data
                          #endif

                          #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                          , Receiver
                          #endif

                          )

                          xmk_T_SIGNAL       sig;

                          #ifdef XMK_USE_SIGNAL_PRIORITIES
                            xmk_T_PRIO         prio;
                          #endif

                          #ifdef XMK_USED_SIGNAL_WITH_PARAMS
                            xmk_T_MESS_LENGTH  data_len;
                            void               xmk_RAM_ptr p_data;
                          #endif

                          #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
                            xPID               Receiver;
                          #endif

#endif

/*-FDEF E*/
{
  X_REGISTER xmk_T_MESSAGE xmk_RAM_ptr P_Signal; /* Pointer to signal        */
  X_STATIC   void xmk_RAM_ptr P_Parameter;  /* Pointer to signal's parameter */
  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    xmk_T_TIME timestamp            ;       /* set signal's timestamp        */
  #endif

  #ifdef XMK_USE_DEBUGGING
    if (xmk_SystemState & XMK_SYSTEM_SUSPENDED)
    {
      RETURN(6);
    }
  #endif

  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    timestamp   = xmk_NOW()         ;
  #endif

  /*
  ** Allocate signal. If there is no free signal 
  ** instance left, return with an error code
  */
  P_Signal = xmk_AllocSignal();
  if (P_Signal == NULL)
  {
    ErrorHandler (ERR_N_NO_FREE_SIGNAL);
    RETURN (1);
  }

  P_Signal->signal      = sig         ; /* set signal's code                  */
  #ifdef XMK_USE_SIGNAL_PRIORITIES
    P_Signal->prio        = prio      ; /* set signal's priority              */
  #endif
  #ifdef XMK_USE_SIGNAL_TIME_STAMP
    P_Signal->timestamp   = timestamp ; /* set signal's timestamp             */
  #endif
  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
     P_Signal->mess_length = data_len ; /* set length of signal's parameter   */
  #endif
  #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
    P_Signal->rec         = Receiver  ; /* set signal's receiver              */
  #endif
  #ifdef XMK_USE_SENDER_PID_IN_SIGNAL
    P_Signal->send        = xRunPID   ; /* set signal's sender                */
  #endif

  /*
  ** if number of bytes of the signal's parameter is greater than
  ** the number of bytes contained in the signal
  */
  #ifdef XMK_USED_SIGNAL_WITH_PARAMS

  if (data_len > XMK_MSG_BORDER_LEN)
  {
    /*
    ** Allocate memory area
    */
    P_Parameter = (xmk_T_MESSAGE xmk_RAM_ptr ) xAlloc (data_len);

    /*
    ** if memory allocation failed, free signal and return errorcode
    */
    if (P_Parameter == NULL)
    {
      ErrorHandler (ERR_N_PARAMETER_MEM_ALLOC);
      xmk_FreeSignal (P_Signal);

      RETURN (2);
    }

    /*
    ** copy parameter into the allocated memory area
    */
    (void)memcpy (P_Parameter, p_data, data_len);

    /*
    **  store pointer to memory area in signal
    */
    P_Signal->ParUnion.ParPtr = P_Parameter;
  }

  /*
  ** if parameters can be stored within the signal itself
  */
  else
  {
    if( data_len > 0 )
    {
      /*
      ** copy the parameters into the signal
      */
      (void)memcpy ((void xmk_RAM_ptr ) (P_Signal->ParUnion.ParCopy), (void xmk_RAM_ptr ) p_data,
                XMK_MSG_BORDER_LEN);

    } /* END IF */
  } /* END ELSE */

  #endif /* ... XMK_USED_SIGNAL_WITH_PARAMS */



  /*
  ** insert signal in queue
  */
  xmk_InsertSignal (P_Signal);

  /*
  **   RETURN (0) means o.k.
  */
  RETURN (0);

} /* END OF FUNCTION */

#endif /*  ...XMK_USE_EXPANDED_KERNEL...  */

#endif /* __MK_OUTP_C_  */
