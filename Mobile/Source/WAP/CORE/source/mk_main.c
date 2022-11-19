/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Modulname    : MK_MAIN.C                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This module contains the functions of the toplevel, which are to be called  |
|  by the user of the Cmicro Kernel, namely                                    |
|                                                                              |
|  -void xmk_InitSDL     ( void )                                              |
|  -void xmk_RunSDL (unsigned char CountSignals)                               |
|                                                                              |
|  There is another function that must be called by the user, before           |
|  xmk_InitSDL and xmk_RunSDL can be used :                                    |
|                                                                              |
|  -void xmk_InitQueue   ( void ) from the mk_queu module.                     |
|                                                                              |
|  The last function is necessary in any case if the Cmicro Tester is to be    |
|  used. This function must be called first before the queue is initialized.   |
|                                                                              |
|  -void xmk_DebugInit   ( void ) from the mt_deb  module.                     |
|                                                                              |
|  If a user forgets to call xmk_InitQueue there might be a call to the        |
|  ErrorHandler, if the Error handling is correctly configured.                |
|                                                                              |
|  Also the users have to fill up some other modules which define environment- |
|  handling, the ErrorHandler etc. These modules reside below the "template"   |
|  directory.                                                                  |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|    ERR_N_INIT_QUEUE      The queue is not correctly initialized              |
|    ERR_N_ILLEGAL_CMD     There was an unknown command received               |
|                          from Cmicro Tester                                  |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    #ifndef XNOMAIN                                                           |
|      void            main                 ( argc, argv )                     |
|    #endif                                                                    |
|                                                                              |
|    void            xmk_InitSDL          ( void )                             |
|                                                                              |
|    if switch XMK_USE_HIGH_OPTIMIZATION is set :                              |
|    void            xmk_RunSDL           ( void )                             |
|                                                                              |
|    if switch XMK_USE_HIGH_OPTIMIZATION is not set :                          |
|    int             xmk_RunSDL (unsigned char CountSignals)                   |
|                                                                              |
|    void            xmk_MicroTesterInit( int flag )                           |
|    void            xmk_MicroTesterDeinit( void )                             |
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

#ifndef __MK_MAIN_C_
#define __MK_MAIN_C_


/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typw.h"

/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/

#ifndef XNOMAIN

  #define XMK_WAIT_ON_HOST  

#endif

/*
**  System is to be shutdowned when user
**  wants to.
**  Shutdown is implemented close the environment
**  and exit in the following ...
*/

#ifdef XMK_USE_xCloseEnv
  #define XMK_CLOSE_ENV xCloseEnv ();
#else
  #define XMK_CLOSE_ENV
#endif

#define XMK_MAIN_SHUTDOWN \
               if (xmk_SystemState & XMK_SYSTEM_SHUTDOWN) \
               { \
                 XMK_CLOSE_ENV; \
                 XMK_KERNEL_EXIT (0); \
               }


/*
** The following is the encoding function of the cod - module
** which is used for the serial interface ...
*/
#define ENCODE_NPAR(IDtag)  \
        xmk_Cod_Encode (XMK_MICRO_COMMAND,IDtag,(char*)NULL,0);

#define ENCODE(IDtag,infobuf,LEN)  \
        xmk_Cod_Encode (XMK_MICRO_COMMAND,IDtag,(char*)&infobuf,LEN);

               
/*--------------------  Typedefinitions     ----------------------------------*/

/*--------------------  Functions    -----------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/

#ifndef NO_GLOBAL_VARS
  #ifdef XMK_ADD_REALTIME_PLAY
    xmk_T_TIME xmk_StartupTime = 0;
  #endif

  #ifdef XMK_USE_SDL_MEM
    char _user_defined_Memory_Area[XMK_MAX_MALLOC_SIZE+1];
  #endif

  #ifdef XMK_USE_MAX_ERR_CHECK
    unsigned char xmk_InitState = 1;
  #endif
#endif

/*============================================================================*/
/*-MGG*/


/*============================================================================*/
/*-MGG*/


#ifndef XNOMAIN

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : main                                                         |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|    This is a simple implementation of the main function. If the functionality|
|    doesn't fulfill the users needs, the define XNOMAIN is to be set and the  |
|    user should implement an own function in a separate module.               |
|    For target applications the define XMK_USE_OS_ENVIRONMENT is to be unset. |
|    If the Cmicro Tester is conditionally compiled (XMK_ADD_MICRO_TESTER),    |
|    the initialization is performed via the host.                             |
|    The trace is fully initialized, e.g.all events will be traced.            |
|    If the xmk_Malloc/xmk_Free functions of Cmicro are to be used             |
|    (XMK_USE_SDL_MEM), then the user must supply a variable named             | 
|    #ifdef XMK_USE_SDL_MEM                                                    |
|    _user_defined_Memory_Area[XMK_MAX_MALLOC_SIZE-1]);                        |
|    #endif                                                                    |
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
  #ifdef XMK_USE_OS_ENVIRONMENT
    int main ( int argc, char* argv[] )
  #else
    int main()
  #endif
#else
  #ifdef XMK_USE_OS_ENVIRONMENT
    int main ( argc, argv[] )
    int argc;
    char* argv[];
  #else
    int main()
  #endif
#endif


/*-FDEF E*/

{

  int                result  = XMK_OKAY ;
  
  #ifdef XMK_USE_SDL_MEM
    /*
    **  You have to spend a memory area (size: XMK_MAX_MALLOC_SIZE t.b.d. in ml_mcf.h) ,
    **  which will be managed by the module ml_mem.c
    */
    xmk_MemInit(&_user_defined_Memory_Area[0], &_user_defined_Memory_Area[XMK_MAX_MALLOC_SIZE-1]);
  #endif

  #ifdef XMK_USE_OS_ENVIRONMENT

    #if defined(_GCC_)
      /*
      ** Try to initialize the terminal
      ** i.e.to enable a non blocking Character read function.
      */
      result = xmk_TermInit ();
    #endif

    XMK_SUPPRESS(argc);
    XMK_SUPPRESS(argv);

  #endif /* ... XMK_USE_OS_ENVIRONMENT */

  #ifdef XMK_USE_DEBUGGING
    /*
    ** Initialization of debugging takes place here
    */
    xmk_DebugInit();
  #endif

  /*
  **   initialize the signal queue
  */
  xmk_InitQueue() ;


  #ifdef XMK_ADD_MICRO_TESTER
    /*
    ** Set all options ON
    */
    result = xmk_OptionsSymbol (xNULLPID, 0xffffffffL);
    if (result != XMK_OKAY) exit (-1);
    /*
    ** Initialize Cmicro Tester ...
    */
    xmk_MicroTesterInit( );
  #endif


  /* C o n f i r m  c u r r e n t        */
  /* S D L - S Y S T E M - S C A L I N G */
  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    xxmonconfig ();
  #endif

  /*
  ** I n i t i a l i z e   C m i c r o   K e r n e l ...
  */
  xmk_InitSDL ();
  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("Init done\n"));
  #endif


  /*
  ** R u n   C m i c r o   K e r n e l  ...
  */

  /*
  ** the 0xff below tells the Cmicro Kernel to infinitely loop
  ** in the queue ...
  */
  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("Starting .... done\n"));
  #endif
  #ifdef XMK_USE_HIGH_OPTIMIZATION
    xmk_RunSDL ();
  #else
    while (xmk_RunSDL (0xff) != XMK_STOP);
  #endif


  #ifdef XMK_USE_xCloseEnv
    /*
    ** Close Interfaces at the environment
    */
    xCloseEnv ();
  #endif

  #ifdef XMK_USE_OS_ENVIRONMENT
    #if defined(_GCC_)
      /*
      ** Deinitialize the terminal
      */
      result = xmk_TermDeinit ();
    #endif
  #endif

  /*
  ** Quit the program ...
  */
  #ifdef XMK_ADD_PRINTF_ADDITIONAL
    PRINTF (("Bye bye...\n"));
  #endif

  #ifdef XMK_ADD_MICRO_TESTER
    xmk_MicroTesterDeinit();
  #endif

#ifndef KEIL_C51
  exit (0);
#endif

  return (0);

} /* END OF FUNCTION */


#endif /* ... XNOMAIN */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_InitSDL                                                  |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This Function is to be called by the user before calling xmk_RunSDL ()      |
|  and implements the initialization of the whole SDL-System, namely           |
|  Timer, Queue, Processes.                                                    |
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
void xmk_InitSDL( void )
#else
void xmk_InitSDL( )
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_MAIN
    XMK_FUNCTION("xmk_InitSDL");
  #endif

  xmk_InterruptsDisabled = 0;

  #if defined(XMK_ADD_CREINIT)
    /* 
    ** Make a backup of the XPDTBL for allowing reinitialization later on
    ** (Reinitialization can restrictively be used for some test only)
    */ 
    xmk_BackupXPDTBL ();
  #endif /* ... XMK_ADD_CREINIT */

  #ifdef XMK_USE_MAX_ERR_CHECK
    /*
    ** Check that user called xmk_InitQueue before
    */
    if (xmk_InitState == (unsigned char) 1)
    {
      #ifdef XMK_ADD_PRINTF_ADDITIONAL
        PRINTF(("\nxmk_InitSDL: Sorry, the queue was not initialized correctly with xmk_InitQueue ().\n\n"));
      #endif
      ErrorHandler (ERR_N_INIT_QUEUE);
      return;
    }
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  /*
  **   Initialize Systemtime
  */
/*  GBU 990730  Removed since GWC use the target timer.
  xmk_InitSystime() ;
*/

  /*
  **   Initialize Timer 
  */
  #ifdef XMK_USED_TIMER
    xmk_InitTimer() ;
  #endif

  #ifdef XMK_ADD_REALTIME_PLAY
    /* 
    ** The following variable is used to generate the timestamps for
    ** signals, which is used to enable the replay of events in realtime 
    ** in the Cmicro Recorders play mode
    */
    xmk_StartupTime    = xmk_NOW () ;
  #endif /* ... XMK_ADD_REALTIME_PLAY */

  #ifdef XMK_USE_KERNEL_WDTRIGGER
    /*
    **   Initialize Watchdog (this is a user function)
    */
    xmk_InitWatchdog() ;
  #endif

  /*+PREEMPT*/
  #ifdef XMK_USE_PREEMPTIVE
    /*
    **   Initialize variables used for preemption
    */
    xmk_InitPreemptionVars();

    /*
    **  Disable preemptive scheduling during system startup
    */
    xmk_DisablePreemption();
  #endif
  /*-PREEMPT*/

  #ifdef XMK_USE_xInitEnv
    /*
    **   Initialize Environment 
    */
    xInitEnv ();
  #endif

  #ifdef XMK_ADD_MICRO_TESTER

    #ifdef XMK_ADD_PSC_TRACE
      /*
      ** Initialize PSC trace
      */
      xmk_TSDL_PSC_CreateAll();
    #endif


    #if defined(XMK_ADD_MICRO_RECORDER)
      xmk_InitRecordMode ();

      XMK_END_CRITICAL_PATH;
      xmk_InitPlayMode ();
      XMK_BEGIN_CRITICAL_PATH;

    #endif /* ... XMK_ADD_MICRO_RECORDER */
  #endif /* ... XMK_ADD_MICRO_TESTER */

  /*
  **   Execute SDL Starttransitions
  */
  xmk_StartProcesses() ;

  /*+PREEMPT*/
  #ifdef XMK_USE_PREEMPTIVE
    /*
    **  Enable preemptive scheduling
    */
    xmk_EnablePreemption();
  #endif
  /*-PREEMPT*/

  /*
  **  critical path ends here, because system start phase ends
  */
  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_MAIN
    XMK_TRACE_EXIT("xmk_InitSDL");
  #endif

} /* END OF FUNCTION */



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RunSDL                                                   |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This functions processes either a number of signals or processes forever,   |
|  if CountSignals is given with 0xff.                                         |
|  Before processing signals, SDL-timeouts are checked and a the function      |
|  "xInEnv", which must be filled out by the user, is called.                  |
|                                                                              |
|  Parameter    : CountSignals  - amount of signals, which is to be worked on  |
|                                 in this function call, where                 |
|                                  0xff means endless.                         |
|                                                                              |
|  Return       : XMK_NORMALRETURN  - SDL-System not to be stopped. There are  |
|                                 processes living or the queue is not empty.  |
|                                                                              |
|                 XMK_STOP          - SDL-System is to be stopped, because     |
|                                 there are no more processinstances living and|
|                                 no signal to be worked on.                   |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifdef XMK_USE_HIGH_OPTIMIZATION
  #ifndef XNOPROTO
    void xmk_RunSDL (void)
  #else
    void xmk_RunSDL ()
  #endif
#else
  #ifndef XNOPROTO
    int  xmk_RunSDL (unsigned char CountSignals)
  #else
    int  xmk_RunSDL (CountSignals)
    unsigned char CountSignals;
  #endif
#endif /* ... XMK_USE_HIGH_OPTIMIZATION */

/*-FDEF E*/

{
  #ifdef XMK_ADD_MICRO_TESTER
    char * p_data;
    char buffer [256];
    int  result;
    int  MessageClass;
    int  MessageTag;
    int  MessageLength;
  #endif /* ... XMK_ADD_MICRO_TESTER */

  #ifdef XMK_ADD_PRINTF_MAIN
    XMK_FUNCTION("xmk_RunSDL");
  #endif

  do
  {

    #ifdef XMK_ADD_MICRO_RECORDER
      if (xmk_RecorderMode == XMK_RECORDER_PLAY)
      {
        #if defined(XMK_UNIX) || defined(BORLAND_C) || defined(MICROSOFT_C)
          int c;
          c = xmk_GetChar ();
          if (XMK_TERMINATION_CHAR(c))
          {
            /* possibility to terminate the program when target          */
            /* is a PC. Attention ! Files and Communications Link are not*/
            /* closed or disabled. This is only the "emergency exit"     */
            /* of the program, which is currently running in play mode.  */
            PRINTF (( "**WARNING: User wants to quit->Using emergency exit\n"));
            XMK_KERNEL_EXIT (0);
          }
        #endif /* ... defined(XMK_UNIX) || defined(BORLAND_C) || defined(MICROSOFT_C) */
      } /* ... if (xmk_RecorderMode == ... PLAY) */
      else
      {
        /*
        ** No activities here from the Cmicro Recorder
        ** when recording or recorder off
        ** ==> Ordinary Environment handling
        */
        #ifdef XMK_USE_xInEnv
          xInEnv ();
        #endif /* ... XMK_USE_xInEnv */

        /*
        **   Check if any timer is expired ...
        */
        #ifdef XMK_USED_TIMER
          #ifdef XMK_USE_DEBUGGING

            #if defined(XMK_ADD_CSHUTDOWN)
              XMK_MAIN_SHUTDOWN;
            #endif /* ... XMK_ADD_CSHUTDOWN */

            #if defined(XMK_ADD_CREINIT)
              /*
              **  System is to be reinitialized on user requests; 
              **  This may lead to unexpected results
              **  because the hardware and physical layer initialization
              **  may fail for some reasons. Also, the buffers of the
              **  communications link will be removed here and the trace
              **  may probably not work.
              **  Reinitialization can restrictively be used for some test only.
              */
              if (xmk_SystemState & XMK_REINIT_SYSTEM)
              {
                xmk_RestoreXPDTBL ();
                xmk_InitSDL ();
              }
            #endif /* ... XMK_ADD_CREINIT */

            /*
            **  Timer expired check is only to be done
            **  if timers are not disabled
            */
            if (!(xmk_SystemState & XMK_TIMER_DISABLED))
          #endif /* ... XMK_USE_DEBUGGING */
                 xmk_ChkTimer ();
        #endif /* ... XMK_USED_TIMER */
      } /* ... if (xmk_RecorderMode == ...) */
    #else
      /*
      **   If the Cmicro Recorder is not compiled, then
      **   read in signals from the environment ...
      */
      #ifdef XMK_USE_xInEnv
        xInEnv ();

        #if defined(XMK_ADD_CSHUTDOWN)
          XMK_MAIN_SHUTDOWN;
        #endif /* ... XMK_ADD_CSHUTDOWN */

        #if defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CREINIT)
          if (xmk_SystemState & XMK_REINIT_SYSTEM)
          {
            xmk_RestoreXPDTBL ();
            xmk_InitSDL ();
          }
        #endif
      #endif /* ... XMK_USE_xInEnv */

      /*
      **   Check if any timer is expired ...
      */
      #ifdef XMK_USED_TIMER
        #ifdef XMK_USE_DEBUGGING
          /*
          **  Timer expired check is only to be done
          **  if timers aren't disabled
          */
          if (!(xmk_SystemState & XMK_TIMER_DISABLED))
        #endif /* ... XMK_USE_DEBUGGING */
            xmk_ChkTimer ();
      #endif /* XMK_USED_TIMER */
    #endif /* XMK_ADD_MICRO_RECORDER */


    #ifdef XMK_ADD_MICRO_TESTER
      p_data = xmk_DLRead () ;
      if (p_data != (char *) NULL)
      {
         result = xmk_Cod_Decode (p_data,
                                  &MessageClass,
                                  &MessageTag,
                                  &MessageLength,
                                  buffer,
                                  sizeof (buffer));

       #ifdef XMK_ADD_MICRO_COMMAND
         if((MessageClass == XMK_MICRO_COMMAND)
         #ifdef XMK_ADD_MICRO_ENVIRONMENT
             ||(MessageClass == XMK_MICRO_ENVIRONMENT))
         #else
             )
         #endif
         {
           result = xmk_HandleHostCmd ( MessageTag, (xmk_U_CMDS*) buffer );
           if (result != XMK_OKAY)
             ErrorHandler (ERR_N_ILLEGAL_CMD);
         }
       #endif

       }
    #endif/* ...  XMK_ADD_MICRO_TESTER */

    /*
    **   Process the signal (if some exists) ...
    */
    #ifdef XMK_USE_PREEMPTIVE
       /*
       ** Actions for preemption ...
       */
       XMK_BEGIN_CRITICAL_PATH;

       if ((HighestPrioLevel = xmk_FetchHighestPrioLevel())
            != MAX_PRIO_LEVELS)
       {
         xmk_Queue = &Prio_Queue[HighestPrioLevel];
         xmk_CurrentSignal = &Prio_CurrentSignal[HighestPrioLevel];
         XMK_END_CRITICAL_PATH;
         xmk_ProcessSignal ();
       }
       else
       {
         XMK_END_CRITICAL_PATH;
       }
    #else
       /*
       ** Actions for non preemption ...
       */
       xmk_ProcessSignal ();
    #endif  /* ... XMK_USE_PREEMPTIVE */


    #ifdef XMK_USE_KERNEL_WDTRIGGER
      /*
      **   Watchdogtrigger
      */
      XMK_WDTRIGGER_FUNCTION;
    #endif /* ... XMK_USE_KERNEL_WDTRIGGER */

    /*
    ** If dynamic SDL System stop was recognized ....
    */
    #ifdef XMK_USE_SDL_SYSTEM_STOP
      if (xmk_QueueEmpty () && ! xmk_IsAnyProcessAlive () )
      {
          #ifdef XMK_ADD_PRINTF_MAIN
            XMK_TRACE_EXIT("xmk_RunSDL");
          #endif

          #ifdef XMK_USE_HIGH_OPTIMIZATION
            return ;
          #else
            return (XMK_STOP);
          #endif
      }
    #endif /* ...XMK_USE_SDL_SYSTEM_STOP */

#ifndef XMK_USE_HIGH_OPTIMIZATION
    /*
    **   If an upper limit of signals is to be worked on here ....
    */
    if (CountSignals != 0xff)
    {
      /*
      **   Count the signals that have been processed ...
      */
      CountSignals --;
    }
  }
  while (CountSignals > 0);

  #ifdef XMK_ADD_PRINTF_MAIN
    XMK_TRACE_EXIT("xmk_RunSDL");
  #endif

  return (XMK_NORMALRETURN) ;

#else
  }
  while (1);
#endif /* ...XMK_USE_HIGH_OPTIMIZATION */

} /* END OF FUNCTION */

#ifdef XMK_ADD_MICRO_TESTER

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_MicroTesterInit                                          |
|  Author       : S&P Media GmbH Germany                                       | 
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This Function calls the host for the Cmicro Tester's configuration          | 
|                                                                              |
|  Parameter    : int flag = 0   Static Init only                              |
|                          = 1-N Init from host                                |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/
 
/*+FDEF E*/

#ifndef XNOPROTO
void xmk_MicroTesterInit( void )
#else
void xmk_MicroTesterInit( )
#endif

/*-FDEF E*/

{
  char                *p_data;
  char                buffer [256];
  int                 result;
  int                 MessageClass;
  int                 MessageTag;
  int                 MessageLength;

  xmk_DLInit();

  xmk_InitOptions ( );

  #ifdef XMK_ADD_MICRO_RECORDER
    xmk_SetRecorderMode( XMK_RECORDER_OFF );
  #endif

  xmk_TracerActive = XMK_TRUE;

  /*
  ** Get target's configuration and send it to host as start message
  */
  xmk_Encode_CMD_GET_CONFIG( CMD_GET_CONFIG_IND );


  #ifdef XMK_WAIT_ON_HOST 

    /*
    ** Indicate to host, that waiting here
    */
    ENCODE_NPAR ( CMD_START_SDL_IND );

    while (1)
    {
      p_data = xmk_DLRead () ;
      if (p_data != (char *) NULL)
      {  
        result = xmk_Cod_Decode (p_data,
                                 &MessageClass,
                                 &MessageTag,
                                 &MessageLength,
                                 buffer,
                                 sizeof (buffer));
 
        #ifdef XMK_ADD_MICRO_COMMAND
 
          if (MessageClass == XMK_MICRO_COMMAND)
          {
  
            result = xmk_HandleHostCmd ( MessageTag, (xmk_U_CMDS*) buffer );
            if (result != XMK_OKAY)
                ErrorHandler (ERR_N_ILLEGAL_CMD);

            if (MessageTag == CMD_START_SDL_REQ) break;
  
          } /* else : other message will be lost here */

        #endif
      }  
    }
  #endif  /* ... XMK_WAIT_ON_HOST */

} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_MicroTesterDeinit                                        |
|  Author       : S&P Media GmbH Germany                                       | 
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|    This Function deinitializes the Cmicro Tester (the data link of it)       | 
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
  void xmk_MicroTesterDeinit( void )
#else
  void xmk_MicroTesterDeinit( )
#endif

/*-FDEF E*/

{
  xmk_DLDeinit();
}

#endif /* ... XMK_ADD_MICRO_TESTER */

#endif /* ... __MK_MAIN_C_ */
