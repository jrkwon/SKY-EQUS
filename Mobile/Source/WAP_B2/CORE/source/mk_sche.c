/*+MHDR*/
/*
# clearcase: CmicroPatchRel2.3.3
+------------------------------------------------------------------------------+
|  Modulname    : MK_SCHE.C                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|    This module implements the functionality of the SDL-                      |
|    scheduler of the Cmicro Kernel.                                           |
|                                                                              |
|    Callers of the functions contained in this module should                  |
|    provide the following requirements :                                      |
|                                                                              |
|    Requirements :                                                            |
|    -----------------------                                                   |
|    The imported variable is to be initialized correctly :                    |
|    <Root-process-table>                                                      |
|    extern XCONST XPDTBL xmk_ROM_ptr xPDTBL [MAX_SDL_PROCESS_TYPES+1];        |
|                                                                              |
|    Exported variables of the SDL-scheduler (this module)                     |
|    (read only by SDL-Application)                                            |
|    -----------------------------------------------                           |
|      xmk_T_MESSAGE *xmk_SignalInstPtr Pointer to current SDL-signal          |
|                                       to work on                             |
|      xPID           xRunPID      stores the value of SELF                    |
|      #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)             |
|      xPIDTable     *pRunPIDTable stores    SDL_PARENT,SDL_OFFSPRING          |
|      #endif                                                                  |
|      xINSTD        *pRunData     Pointer to <instance-data-struct>           |
|                                  of currently running process.               |
|      #ifdef XMK_USE_PREEMPTIVE                                               |
|      T_E_SIGNAL    *PrioQueue[]  Pointer to different prio-level queues      |
|      #endif                                                                  |
|                                                                              |
|      #ifndef XMK_USE_PREEMPTIVE                                              |
|      xSYMBOLNR xmk_SymbolNrVar   Global variable to select the appropriate   |
|                                  transition in the yPAD function ...         |
|    #endif                                                                    |
|                                                                              |
|                                                                              |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|    ERR_N_NO_COR_PRIO           Errorcheck is conditionally compiled          |
|                                      If preemption is selected, this error   |
|                                      can come up, if the priority levels     |
|                                      are not set appropriately.              |
|    ERR_N_NO_CONT_PRIO          Errorcheck is conditionally compiled          |
|                                      If preemption is selected, this error   |
|                                      can come up, if the priority levels     |
|                                      are not set appropriately.              |
|    ERR_N_xRouteSignal                Errorcheck is conditionally compiled    |
|                                      If a signal has no defined receiver-    |
|                                      process. Note : This errorcheck is      |
|                                      used only when a signal route function  |
|                                      is defined by the user.                 |
|    ERR_N_SDL_DISCARD           Errorcheck is conditionally compiled          |
|                                      Error is detected, if a signal is sent  |
|                                      to a non existent process (a sleeping   |
|                                      one), or of the process is in a state,  |
|                                      where this signal neither can be        |
|                                      consumed not saved.                     |
|    ERR_N_TRANS_TIME            Errorcheck is conditionally compiled          |
|                                      Maximum timeout for the execution of    |
|                                      a transition exceeded.                  |
|    ERR_N_CREATE_NO_MEM         Errorcheck is conditionally compiled          |
|                                      It wasn't possible to allocate the      |
|                                      SDL-createsignal for process creation.  |
|    ERR_N_CREATE_INSTANCE       Errorcheck is conditionally compiled          |
|                                      It wasn't possible to create a process  |
|    ERR_N_SYSTEM_SIGNAL                 Errorcheck is conditionally compiled  |
|                                      A Signal was treated as an systemsignal |
|                                      but is not defined in the list of       |
|                                      systemsignals.                          |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
l    void              *SI*StartProcesses         (void)                       |
|    void              *SI*ProcessSignal          (void)                       |
|    xmk_T_INSTANCE    *SI*CreateProcess          (xmk_T_PROCESS)    *1        |
|    xmk_T_BOOL        *SI*IsAnyProcessAlive      (void)             *1        |
|    void              xmk_InitPreemptionVars     (void)             *1 *2     |
|    void              xmk_DisablePreemption      (void)             *1 *2     |
|    void              xmk_EnablePreemption       (void)             *1 *2     |
|    xmk_T_PRIOLEVEL   xmk_FetchHighestPrioLevel  (void)             *1 *2     |
|    void              xmk_CheckIfSchedule        (void)             *1 *2     |
|    void              xmk_SwitchPrioLevel        (xmk_T_PRIOLEVEL)  *1 *2     |
|    xmk_T_STATE       *SI*GetProcessState        (void)             *1    *3  |
|    xmk_T_BOOL        *SI*KillProcess            (xPID)             *1    *3  |
|                                                                              |
|    The marker *SI* above means "SystemID". For each partitioned system there |
|    is a unique name which is used as a prefix for making it possible to      |
|    compile/link several Cmicro Kernel plus system into one executable pro-   |
|    gram. The functions xmk_ above are not available in partitioned systems,  |
|    because these can be used to preemptive configuration of Cmicro Kernel    |
|    only.                                                                     |
|                                                                              |
|    Those functions marked with *1 are compiled conditionally. Functions      |
|    marked with *2 are compiled only for a so scaled preemptive Cmicro Kernel.|
|    Functions marked with *3 are compiled only for the Cmicro Tester.         |
|                                                                              |
|                                                                              |
|  M3 Static functions of this module :                                        |
|  --------------------------------------------------------                    |
|    ....              *SI*HandleSystemSignal     (...)                        |
|    ....              *SI*CheckSignal            (...)                        |
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


#ifndef __MK_SCHE_C_
#define __MK_SCHE_C_


/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typ.h"
#include "CmmnRsrc.h"


/*--------------------  Variables    -----------------------------------------*/

#ifndef NO_GLOBAL_VARS
  /*    Modified by GBU,990518 */
  extern XCONST XPDTBL xmk_ROM_ptr xPDTBL[MAX_SDL_PROCESS_TYPES+1];  /* <root-process-table>          */
#endif

/*============================================================================*/
/*-IMPORT*/


/*+EXPORT*/
/*====================  E X P O R T  =========================================*/
#ifndef NO_GLOBAL_VARS
  xPID                     xRunPID;    /* PID of currently active SDL-Process */
                                       /* to retrieve SDL_SELF                */


  #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
    xPIDTable xmk_RAM_ptr  pRunPIDTable; /* Pointer to PID-table of currently*/
                                         /* running process; this is to      */
                                         /* enable access to SDL_OFFSPRING   */
                                         /* and SDL_PARENT.                  */
  #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */


    xINSTD xmk_RAM_ptr       pRunData;      /* Pointer to currently running   */
                                            /* processinstance - data         */

    xmk_T_MESSAGE xmk_RAM_ptr xmk_SignalInstPtr; /* Pointer to current SDL-signal */
                                                 /* to work on                    */

  /*+PREEMPT*/
  #ifdef XMK_USE_PREEMPTIVE
    T_E_SIGNAL    xmk_RAM_ptr Prio_Queue[MAX_PRIO_LEVELS];
                                              /* Used to store the queue-     */
                                              /* pointers of the different    */
                                              /* priority-levels              */
  #endif
  /*-PREEMPT*/

  #ifndef XMK_USE_PREEMPTIVE
    xSYMBOLNR xmk_SymbolNrVar ;                /* Select appropriate Transition */                                               /* in yPAD function ...          */
  #endif

  unsigned char xmk_InterruptsDisabled;        /* To store whether the hardware */
                                               /* interrupts are disabled or not*/

  #if defined(XMK_ADD_MICRO_TESTER) && defined(XMK_ADD_CQUERY_PROCESS)
    xmk_T_PRS_PROF xmk_ProcessProfiles [MAX_SDL_PROCESS_TYPES];
  #endif

#endif /*NO_GLOBAL_VARS*/

/*
** Switch off any error check
*/
#ifdef XMK_USE_NO_ERR_CHECK
  #define ErrorHandler(errnum)
#endif

/*============================================================================*/
/*-EXPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/

#ifdef  ANY_STATE
#undef  ANY_STATE
#endif
#define ANY_STATE       0

/*
** Check Signal Results
*/
#ifdef  SAVE_NOT_TRACE
#undef  SAVE_NOT_TRACE
#endif
#define SAVE_NOT_TRACE       -1
#ifdef  SAVE
#undef  SAVE
#endif
#define SAVE                 -2
#ifdef  DISCARD
#undef  DISCARD
#endif
#define DISCARD              -3
#ifdef  IMPLICIT_CONSUMPTION
#undef  IMPLICIT_CONSUMPTION
#endif
#define IMPLICIT_CONSUMPTION -4

/*
**  Internal Value(running proc-type)
*/
#ifdef  RUN_PROC
#undef  RUN_PROC
#endif
#define RUN_PROC        EPIDTYPE(xRunPID)

/*
**  Internal Value(running proc-inst)
*/
#ifdef  RUN_INST
#undef  RUN_INST
#endif
#define RUN_INST        EPIDINST(xRunPID)

/*+PREEMPT*/

#ifdef XMK_USE_PREEMPTIVE
  /*
  **  Internal value (priority-level of currently running process)
  */
  #undef  EPRIOLEVEL
  #define EPRIOLEVEL xPDTBL[EPIDTYPE(xRunPID)]->PrioLevel
#endif

/*-PREEMPT*/

/*--------------------  Typedefinitions     ----------------------------------*/

/*--------------------  Functions    -----------------------------------------*/
/*static*/ int  xmk_CheckSignal XPP (( xmk_T_SIGNAL, xmk_T_STATE, XCONST XPDTBL xmk_ROM_ptr));

#ifdef XMK_USED_DYNAMIC_CREATE
  /*static*/ void xmk_HandleSystemSignal XPP (( xmk_T_MESSAGE xmk_RAM_ptr));
#endif


/*+PREEMPT*/
#ifdef XMK_USE_PREEMPTIVE
  void      xmk_CheckIfSchedule    XPP (( void ));
#endif
/*-PREEMPT*/

/*--------------------  Variables   ------------------------------------------*/

/*+PREEMPT*/
#ifndef NO_GLOBAL_VARS
  #ifdef XMK_USE_PREEMPTIVE
    static unsigned char xmk_PreemptionDisabled;   /* Used to store       */
                                                   /* whether preemption  */
                                                   /* is disabled or not  */
    xmk_T_PRIOLEVEL    CurrentPrioLevel,           /* current prio-level  */
                       HighestPrioLevel;           /* highest prio-level  */
                                                   /* where there is a    */
                                                   /* signal              */
    T_E_SIGNAL    xmk_RAM_ptr Prio_CurrentSignal[MAX_PRIO_LEVELS];
                                                 /* storing the signal  */
                                                 /* currently worked on */
    xPID          Prio_xRunPID[MAX_PRIO_LEVELS];
                                                 /* storing current PID */

    #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)

      xPIDTable  xmk_ROM_ptr Prio_pRunPIDTable[MAX_PRIO_LEVELS];
                                                 /* Used to store the   */
                                                 /* current processes   */
                                                 /* PARENT &  OFFSPRING */
                                                 /* information         */
    #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

    xINSTD    xmk_RAM_ptr Prio_pRunData[MAX_PRIO_LEVELS];
                                                 /* storing local data  */
                                                 /* of current process  */
  #endif          /* XMK_USE_PREEMPTIVE */
  /*-PREEMPT*/

  #if defined(XMK_USED_SENDER) && defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_USE_SAFE_ADDRESSING)
    /*
    ** Usually SDL_SENDER is evaluated with xmk_SignalInstPtr->send
    ** but this is not possible in start transition and when
    ** a processinstance is dynamically allocated because
    ** xmk_SignalInstPtr does not point to a valid signal.
    ** Therefore, a dummy message must be defined.
    */
    static xmk_T_MESSAGE xmk_EmptySignalVar;
  #endif /* defined(XMK_USED_SENDER) && defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_USE_SAFE_ADDRESSING) */

#endif /* NO_GLOBAL_VARS */

/*============================================================================*/
/*-MGG*/

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*StartProcesses                                           |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|  This function implements the STARTUP-Phase of the SDL-System. All static    |
|  process-instances are created.                                              |
|  This means executing the start-transition of all process-instances to       |
|  be created. For each created process-instance, the first state is set.      |
|  If configured right, the values SDL_SELF, SDL_PARENT and SDL_OFFSPRING      |
|  are correctly initialized (only necessary if no semantic check was          |
|  performed, i.e.if someone doesn't use the SDT-Analyzer).                    |
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
  void xmk_StartProcesses ( void )
#else
  void xmk_StartProcesses ( )
#endif

/*-FDEF E*/

{
  unsigned char        Process  ;

  #ifndef XMK_USED_ONLY_X_1
    /*
    ** Loop over instances of a type only if more than 1 instance ...
    */
    unsigned char      Instance ;
  #endif

/*+PREEMPT*/
  #ifdef XMK_USE_PREEMPTIVE
    unsigned char      priority;
    #ifdef XMK_USE_MAX_ERR_CHECK
      unsigned char    PrioLevelNumbering;
    #endif
  #endif
/*-PREEMPT*/

  XCONST    XPDTBL      xmk_ROM_ptr pProcessType ;
  xmk_T_TRANS_ADDRESS   TransitionFunction ;

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*StartProcesses");
  #endif
      PRINTF(("vorwhile_1 ")); 
  #if defined(XMK_USED_SENDER) && defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_USE_SAFE_ADDRESSING)
    /*
    ** For evaluation of SDL_SENDER expression :
    */
    xmk_EmptySignalVar.send = xNULLPID;
      PRINTF(("vorwhile_2 ")); 
  #endif

  #ifdef XMK_USED_INITFUNC
    /*
    ** Initialization used only for synonyms, which cannot
    ** become an C define, but must be an variable ...
    */
      PRINTF(("vorwhile_3 ")); 
    yInit (); 
   
  #endif

  #ifndef XMK_USE_PREEMPTIVE
    xmk_SymbolNrVar = 0;
  #endif

  #ifndef XMK_USE_PREEMPTIVE
    #ifdef XMK_ADD_CQUERY_PROCESS
      memset ((void*) &xmk_ProcessProfiles, 0, sizeof (xmk_ProcessProfiles));
    #endif
  #endif

  #ifdef XMK_USE_PREEMPTIVE
    #ifdef XMK_USE_MAX_ERR_CHECK
      priority = 0;
      for (Process = 0; Process < MAX_SDL_PROCESS_TYPES; Process++)
      {
        if (xPDTBL[Process]->PrioLevel > priority)
          priority = xPDTBL[Process]->PrioLevel;
      }
      priority++;
      if (priority != MAX_PRIO_LEVELS)
      {
          ErrorHandler(ERR_N_NO_COR_PRIO);
      }

      PrioLevelNumbering = 0;
      for (priority = 0; priority < MAX_PRIO_LEVELS; priority ++)
      {
        Process = 0;
        while ( xPDTBL [Process] != X_END_ROOT_TABLE )
        {
          if ( xPDTBL[Process]->PrioLevel == priority )
          {
            PrioLevelNumbering++;
            Process++;
            break;
          }
          Process++;
        }
      }
      if (PrioLevelNumbering != MAX_PRIO_LEVELS)
      {
        ErrorHandler(ERR_N_NO_CONT_PRIO);
      }
    #endif
    /*
    **       search for the processtype with the highest priority
    **       to create all the instances of this type the SDL-system
    **       defined as to be created at system startup and go on
    **       with decreasing priorities
    */
    for (priority = 0; priority < MAX_PRIO_LEVELS; priority ++)
        {
          for (Process = 0;
               ((pProcessType = xPDTBL[Process]) != X_END_ROOT_TABLE);
               Process ++)
          {
            if (xPDTBL[Process]->PrioLevel == priority)
                {
  #else
    Process = 0;
    /*
    **  Loop in <root-process-table>
    */
  
    while ( (pProcessType = xPDTBL [Process]) != X_END_ROOT_TABLE )
    {
  #endif
          PRINTF(("while ")); 
          TransitionFunction = pProcessType->yPAD_Function;
          #ifndef XMK_USED_ONLY_X_1
            Instance = 0 ;

            /*
            **  Loop in <process-state-table> for all instances of this processtype
            */
            while( ( pProcessType->ProcessStateTable[ Instance ] == XSTARTUP ) &&
                   ( Instance < pProcessType->MaxInstances                     )
                 )
          #else
            if ( pProcessType->ProcessStateTable[0] == XSTARTUP )
          #endif
            {
              #ifdef XMK_ADD_PRINTF_ADDITIONAL
                xxmonPID ("Starting Process :", GLOBALPID(Process,Instance));
              #endif /* ... XMK_ADD_PRINTF_ADDITIONAL */

              /*
              **   load global variable with current instances identification
              */
              xRunPID = GLOBALPID(Process,Instance);

              /*
              ** Load pointer to instancedata to enable access in start-transition
              */

              /*
               * Dynamic instance data:
               * Here we allocate memory for the data for
               * those process instances that exist on startup.
               */
#ifndef XMK_USED_ONLY_X_1
              ((unsigned char **)(pProcessType->pInstanceData))[RUN_INST] =
                OSConnectorAlloc (pProcessType->DataLength);
              pRunData = ((unsigned char **)(pProcessType->pInstanceData))[RUN_INST];
              memset (pRunData, 0, pProcessType->DataLength);  
#else
              pProcessType->pInstanceData =
                OSConnectorAlloc (pProcessType->DataLength);
              pRunData = pProcessType->pInstanceData;
              memset (pRunData, 0, pProcessType->DataLength);  
#endif

              /*
              ** Setting SDL_PARENT, SDL_OFFSPRING and SDL_SENDER to NULL ...
              */
              #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
                pRunPIDTable = &( pProcessType->pPIDTable [RUN_INST]) ;

                #ifdef XMK_USED_PARENT
                  pRunPIDTable->Parent    = xNULLPID;
                #endif

                #ifdef XMK_USED_OFFSPRING
                  pRunPIDTable->Offspring = xNULLPID;
                #endif
              #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

              #if defined(XMK_USED_SENDER) && defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_USE_SAFE_ADDRESSING)
                /*
                ** For evaluation of SDL_SENDER expression :
                */
                xmk_SignalInstPtr= &xmk_EmptySignalVar;
              #endif

              #ifdef XMK_USE_KERNEL_INIT
                memset (pRunData, 0, pProcessType->DataLength);  
              #endif

              /*+PREEMPT*/
              #ifdef XMK_USE_PREEMPTIVE
                XMK_QUEUE_ADR = &(Prio_Queue[priority]);
                XMK_CURRENTSIGNAL_ADR = &(Prio_CurrentSignal[priority]);
                #ifdef XMK_ADD_TSYS_SHOWPRIO
                  /*
                  **    Show Prio Level on MSC if wanted ...
                  */
                  xmk_tsys_ShowPrioLevel (priority);
                #endif /* ... XMK_ADD_TSYS_SHOWPRIO */
              #endif /* ... XMK_USE_PREEMPTIVE */
              /*-PREEMPT*/

              #ifdef XMK_ADD_TSTATIC_CREATE
                /*
                ** All other Traces ...
                */
                xmk_TSDL_StaticCreate (xRunPID);
              #endif

              #ifdef XMK_ADD_PRINTF_ADDITIONAL
                xxmonPID ("Starttransition of :", GLOBALPID(Process,Instance));
              #endif /* ... XMK_ADD_PRINTF_ADDITIONAL */

              #ifdef XMK_USE_BANKSW
                XMK_SWITCH_BANK(EPIDTYPE(xRunPID));
              #endif

              /*
              ** Call the start-transition  ...
              */
              #ifdef XMK_USE_PREEMPTIVE
                pProcessType->ProcessStateTable[ RUN_INST ] = TransitionFunction (XSTART_TRANSITION);
              #else
                /*
                ** xmk_SymbolNrVar set appropriate in function entry ...
                */
                pProcessType->ProcessStateTable[ RUN_INST ] = TransitionFunction ();
              #endif

              #ifdef XMK_ADD_TSTATE
                 xmk_TSDL_State ( pProcessType->ProcessStateTable[ RUN_INST ] );
              #endif

              /*
              ** Set xRunPID no longer active, relevant only for
              ** debug, trace and command interface.
              ** It is not necessary to update the current prio-level,
              ** because preemption is disabled during system startup phase.
              */
              xRunPID = xNULLPID;

          #ifndef XMK_USED_ONLY_X_1
              Instance++ ;
          #endif
            } /* testing if processinstances are to be statically created */

  #ifdef XMK_USE_PREEMPTIVE
        } /* if xPDTBL...*/
      }   /* for Process = ... */
    }     /* for priority = ... */
    
    /*
    **  necessary to prevent from scheduling before actually starting
    **  the SDL-system itself
    */
    CurrentPrioLevel = 0;    
    
  #else
    Process++ ;
  } /* END WHILE (testing processtypes) */
  #endif

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT ("*SI*StartProcesses");
  #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*ProcessSignal                                            |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|  This function processes an SDL-signal and remains in an internal loop,      |
|  until a signal was processed, or until no signal is remaining in any        |
|  input-port in the SDL-system.                                               |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#define WAP_USE_START

#ifndef XNOPROTO
  void xmk_ProcessSignal ( void )
#else
  void xmk_ProcessSignal ( )
#endif

/*-FDEF E*/

{
  XCONST XPDTBL              xmk_ROM_ptr p_ReceiverProcess  ;
         xmk_T_MESSAGE       xmk_RAM_ptr p_Message          ;
         xmk_T_TRANS_ADDRESS TransitionFunction             ;

  xmk_T_STATE            State            ;

  #define CurrentState   State
  #define NextState      State

  #ifdef XMK_USE_PREEMPTIVE
    xSYMBOLNR            TmpSymbolNr ;
  #endif

  #if defined(XMK_ADD_MICRO_TESTER) && defined(XMK_ADD_PSC_TRACE)    
    xmk_T_CMD_TPSC_TRACE   PSCTrace; 
    xmk_T_CMD_TPSC_TIME    PSCTime;
  #endif

  int                  TestResult ;

  #if defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_MICRO_COMMAND)
    #if defined(XMK_ADD_CBREAK_LOGIC)
      xmk_T_CMD_BREAK_HIT_IND BreakHitDescr ;
    #endif
  #endif

  #ifndef XMK_USE_PREEMPTIVE
    #if defined(XMK_USE_CHECK_TRANS_TIME) || defined(XMK_ADD_CQUERY_PROCESS)
    xmk_T_TIME           begin_time, end_time ;
    #endif
  #endif

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*ProcessSignal");
  #endif

  #if defined(XMK_USE_DEBUGGING) && (defined(XMK_ADD_CREINIT) || defined(XMK_ADD_CSHUTDOWN))
    if (xmk_SystemState & (XMK_REINIT_SYSTEM | XMK_SYSTEM_SHUTDOWN) )
    {
      XMK_SUPPRESS(xmk_SystemState);
      return ;
    }
  #endif /* ... XMK_ADD_CREINIT */

  #ifdef XMK_USE_DEBUGGING
    if (!(xmk_SystemState & XMK_BREAKPOINT_HIT))
    {
  #endif

      /*
      **   load pointer to signal with first signal in the queue
      */
      p_Message = xmk_FirstSignal () ;

#ifdef WAP_USE_START	
	  if(p_Message->signal == 188)
	  {
		  extern BOOL checkbrowserstart;
		  extern BOOL browserTerComp;
		  if(browserTerComp == FALSE)
		  {
			checkbrowserstart = TRUE;
			CLNTa_log(1,0,"Wait Here please");
		  }
	  }
#endif//#ifdef WAP_USE_START

      /*
      **   while there is a signal to be worked on
      */
      #if defined(XMK_ADD_MICRO_TESTER) && defined(XMK_ADD_PSC_TRACE)
        /*
        ** Send Systemtime to PSC
        */
        PSCTime.time = xmk_NOW();
        if(( PSCTime.time & 0x000000ffL) == 0x00000080 )
        {
          xmk_Cod_Encode (XMK_MICRO_TRACER, CMD_TPSC_TIME, (char*)&PSCTime, sizeof( xmk_T_CMD_TPSC_TIME) );
        }
      #endif
  
      while( p_Message != (xmk_T_MESSAGE xmk_RAM_ptr) NULL )
      {
        /*
        ** Use a Routing function, if the signal in the queue contains
        ** no receiver-PID. The user has to implement the function
        ** anywhere; a template is given in mk_user - module.
        */
        #ifndef XMK_USE_RECEIVER_PID_IN_SIGNAL
           /*
           **   Set new active process-PID
           */
           XMK_BEGIN_CRITICAL_PATH;
           xRunPID = xRouteSignal (p_Message->signal);
           #ifdef XMK_USE_PREEMPTIVE
             /*
             ** store the priority-level working on
             */
             CurrentPrioLevel = EPRIOLEVEL;
           #endif
           XMK_END_CRITICAL_PATH;

           #ifdef XMK_USE_MAX_ERR_CHECK
              if (xRunPID == xNULLPID)
              {
                 ErrorHandler (ERR_N_xRouteSignal);
                 xmk_RemoveCurrentSignal ();
                 /*
                 **   Leave signal-handling ( leave while-loop )
                 */
                 p_Message = NULL;
                 continue;
              }
           #endif
        #else
           /*
           **   Set new active process-PID
           */
           XMK_BEGIN_CRITICAL_PATH;
           xRunPID = p_Message->rec;
           #ifdef XMK_USE_PREEMPTIVE
             CurrentPrioLevel = EPRIOLEVEL;
           #endif
           XMK_END_CRITICAL_PATH;
        #endif /* ... ifndef XMK_USE_RECEIVER_PID_IN_SIGNAL */

        #ifdef XMK_USE_DEBUGGING
          /*
          **  Check whether the system is in single-stepping
          */
          if (xmk_SystemState & XMK_SINGLE_STEPPING)
          {
            if (xmk_SystemState & XMK_SYSTEM_NEXT_STEP)
            {
              XMK_BEGIN_CRITICAL_PATH;
                xmk_SystemState &= ~XMK_SYSTEM_NEXT_STEP;
              XMK_END_CRITICAL_PATH;
            }
            else
            {
              return;
            }
          }

          /*
          **  Check whether the system is suspended or not
          */
          if (xmk_SystemState & XMK_SYSTEM_SUSPENDED)
          {
            #ifdef XMK_ADD_PRINTF_ADDITIONAL
              PRINTF(("\n...Sorry, suspended\n\n"));
            #endif
            return;
          }

          #ifdef XMK_ADD_CBREAK_LOGIC
            /*
            **  Check whether the current system state is a breakpoint
            */
            if (xmk_CheckIfBreakpointTrue(xRunPID, p_Message->signal,
                                          XMK_NO_STATE,&BreakHitDescr))
            {
              XMK_BEGIN_CRITICAL_PATH;
                xmk_SystemState |= XMK_BREAKPOINT_HIT;
              XMK_END_CRITICAL_PATH;
              #ifdef XMK_ADD_PRINTF_ADDITIONAL
                PRINTF(("\n\n Breakpoint Signal was hit \n\n"));
              #endif
              #if defined(XMK_ADD_MICRO_COMMAND)
                (void) xmk_HandleHostCmd (CMD_BREAK_HIT_IND,
                                          (xmk_U_CMDS *) &BreakHitDescr);
              #endif /* ... defined(XMK_ADD_MICRO_COMMAND) */
              return;
            }
          #endif /* ... XMK_ADD_CBREAK_LOGIC */
        #endif  /* ... XMK_USE_DEBUGGING */

        #ifdef XMK_ADD_PRINTF_ADDITIONAL
          XMK_TRACE_SCHE("*SI*ProcessSignal(): WORKING ON A SIGNAL");
          XMK_TRACE_SCHE("----------------------------------------");
          PRINTF (("*SI*ProcessSignal():Signal-ID:%d\n", p_Message->signal));
        #endif /* ... XMK_ADD_PRINTF_ADDITIONAL */

        #ifdef XMK_USED_DYNAMIC_CREATE
          /*
          **   If current signal is a system-signal
          */
          if( p_Message->signal == XMK_CREATE_SIGNALID)
          {
            /*
            **   work on it
            */
            xmk_HandleSystemSignal  ( p_Message );

            /*
            **   Leave signal-handling ( leave while-loop )
            */
            p_Message = NULL;
          } /* END IF */

          /*
          **   if signal isn't system-signal
          */
          else
        #endif /* ... XMK_USED_DYNAMIC_CREATE */
        {

          /*
          **   load pointer with process-description-table of current processtype
          */
          p_ReceiverProcess = xPDTBL [ RUN_PROC ] ;
          #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
            XMK_BEGIN_CRITICAL_PATH;
            pRunPIDTable = &( p_ReceiverProcess->pPIDTable [RUN_INST]) ;
            XMK_END_CRITICAL_PATH;
          #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

          /*
          **   Get current state of the receiver-process-instance from <process-state-table>
          */
          CurrentState       = p_ReceiverProcess->ProcessStateTable [ RUN_INST ] ;

          /*
          **   Receiving processinstance remains in a state,
          **   where the signal to work on is defined as "SAVE"
          */
          #ifdef XMK_USED_SAVE
            if( xmk_TestAndSetSaveState( CurrentState ) )
            {
              /*
              **   define handling of signal
              */
              TestResult = SAVE_NOT_TRACE ;
            } /* END IF */
            else
          #endif /* XMK_USED_SAVE */
          {
            /*
            **   Get action for signals not yet saved ...
            */
            TestResult =
              xmk_CheckSignal ( p_Message->signal, CurrentState, p_ReceiverProcess );
           } /* END ELSE */

          /*
          **   Perform action ...
          */
          switch( TestResult )
          {
            #ifdef XMK_USED_SAVE
              case SAVE_NOT_TRACE :
              {
                /*
                **   No action for this signal, work on next Signal
                */
                p_Message = xmk_NextSignal () ;
                break ;
              } /* END CASE */

              case SAVE :
              {
                #ifdef XMK_ADD_TSAVE
                  xmk_TSDL_Save (p_Message);
                #endif
                /*
                **   No action for signals to be saved, work on next signal
                */
                p_Message = xmk_NextSignal () ;
                break ;
              } /* END CASE */
            #endif /* XMK_USED_SAVE */

            case IMPLICIT_CONSUMPTION :
            {
              /*
              **   INTERPRETATION ERROR : Signal has been consumed 
              */ 
              ErrorHandler (ERR_N_SDL_IMPLICIT_CONSUMPTION);

              #ifndef XMK_USE_NO_ERR_CHECK
              {
                #ifdef XMK_ADD_TIMPLICIT_CONSUMPTION
                  xmk_TSDL_ImplicitConsumption (p_Message, CurrentState);
                #endif
  
                /*
                **   Remove signal from queue
                */
                xmk_RemoveCurrentSignal () ;
  
                /*
                **   ... and work on next signal
                */
                p_Message = xmk_NextSignal () ;
                break ;
              } 
              #endif /* ... XMK_USE_NO_ERR_CHECK */
            } /* END CASE */

            /*
            **  ------------V
            ** Fall into case DISCARD, if no error check is selected...
            **              ------------V
            */
            case DISCARD              :
            {
              /*
              **   INTERPRETATION ERROR : SDL-discard -> process doesn't exist's
              */
              ErrorHandler (ERR_N_SDL_DISCARD);

              #ifdef XMK_ADD_TDISCARD
                xmk_TSDL_Discard (p_Message, CurrentState);
              #endif

              /*
              **   Remove signal from queue
              */
              xmk_RemoveCurrentSignal () ;

              /*
              **   ... and work on next signal
              */
              p_Message = xmk_NextSignal () ;
              break ;
            } /* END CASE */

            default :
            {

              XMK_BEGIN_CRITICAL_PATH;

              /*
              **   NORMAL CASE :
              **   Load global pointer before calling the transitionfunction
              */
              xmk_SignalInstPtr = p_Message ;

              /*
              **   Load global pointer to enable access instancedata for the
              **   transitionfunction
              */
              /*
               * Dynamic instance data:
               * Since it is the pointers that are stored in the array
               * pInstanceData, we just copy a pointer value.
               */
#ifndef XMK_USED_ONLY_X_1
              pRunData = ((unsigned char **)(p_ReceiverProcess->pInstanceData))[RUN_INST];
#else
              pRunData = p_ReceiverProcess->pInstanceData;
#endif

              XMK_END_CRITICAL_PATH;

              #ifdef XMK_ADD_TINPUT
                xmk_TSDL_Input (p_Message, CurrentState);
              #endif

              /*
              **   load symbolnumber and address of the transitionfunction to execute ...
              */
              #ifdef XMK_USE_PREEMPTIVE
                TmpSymbolNr         = p_ReceiverProcess->TransitionTable [ TestResult ].SymbolNr;
              #else
                XMK_BEGIN_CRITICAL_PATH;

                xmk_SymbolNrVar  = p_ReceiverProcess->TransitionTable [ TestResult ].SymbolNr;

                XMK_END_CRITICAL_PATH;
              #endif

              TransitionFunction = p_ReceiverProcess->yPAD_Function ;

              #ifndef XMK_USE_PREEMPTIVE
                #if defined(XMK_USE_CHECK_TRANS_TIME) || defined(XMK_ADD_CQUERY_PROCESS)

                  /*
                  **   Get systemtime to administer execution time of the transition to be performed
                  */
                  begin_time = xmk_NOW();
                #endif
                #ifdef XMK_ADD_CQUERY_PROCESS
                  xmk_ProcessProfiles [ RUN_PROC ].LastTime = 0 ;
                #endif
              #endif

              #ifdef XMK_USE_PREEMPTIVE
                #ifdef XMK_ADD_TSYS_SHOWPRIO
                  /*
                  **    Show Prio Level on MSC if wanted ...
                  */
                  xmk_tsys_ShowPrioLevel (CurrentPrioLevel);
                #endif /* ... XMK_ADD_TSYS_SHOWPRIO */
              #endif /* ... XMK_USE_PREEMPTIVE */

              #ifdef XMK_USE_BANKSW
                /*
                **   Bank-switching
                */
                XMK_SWITCH_BANK(EPIDTYPE(xRunPID));
              #endif

              #if defined(XMK_ADD_MICRO_TESTER) && defined(XMK_ADD_PSC_TRACE)    
                /*
                ** Fill the PSC Info Structure and send it to the host
                */
                PSCTrace.time    = xmk_NOW();
                PSCTrace.process = xRunPID;
                PSCTrace.flag    = XMK_TRUE;
 
                xmk_Cod_Encode (XMK_MICRO_TRACER, CMD_TPSC_TRACE, (char*)&PSCTrace, sizeof( xmk_T_CMD_TPSC_TRACE) );
 
              #endif

              /*
              **   Execute transitionfunction
              */
              #ifdef XMK_USE_PREEMPTIVE
                NextState = TransitionFunction (TmpSymbolNr) ;
              #else
                /*
                ** xmk_SymbolNrVar set appropriate in function entry ...
                */
                NextState = TransitionFunction ( ) ;
              #endif

              #if defined(XMK_ADD_MICRO_TESTER) && defined(XMK_ADD_PSC_TRACE)
                /* 
                ** Fill the PSC Info Structure and send it to the host 
                */ 
                PSCTrace.time    = xmk_NOW();
                PSCTrace.process = xRunPID; 
                PSCTrace.flag    = XMK_FALSE;
                xmk_Cod_Encode (XMK_MICRO_TRACER, CMD_TPSC_TRACE, (char*)&PSCTrace, sizeof( xmk_T_CMD_TPSC_TRACE) );
              #endif

              #ifndef XMK_USE_PREEMPTIVE
                #if defined(XMK_USE_CHECK_TRANS_TIME) || defined(XMK_ADD_CQUERY_PROCESS)
                  /*
                  **   get current system-time
                  */
                  end_time = xmk_NOW();
                #endif

                #ifdef  XMK_USE_CHECK_TRANS_TIME
                  /*
                  **   compare duration of transition with predefined value and call
                  **   error-handling routine if transition duration was too long
                  */
                  if (end_time - begin_time > XMK_TRANS_TIME)
                        ErrorHandler (ERR_N_TRANS_TIME);
                #endif
                 
                #ifdef XMK_ADD_CQUERY_PROCESS
                  xmk_ProcessProfiles[ RUN_PROC ].LastTime = end_time - begin_time ;
                  if (xmk_ProcessProfiles[ RUN_PROC ].LastTime >
                      xmk_ProcessProfiles[ RUN_PROC ].MaxTime)
                  {
                    xmk_ProcessProfiles[ RUN_PROC ].MaxTime = xmk_ProcessProfiles[ RUN_PROC ].LastTime ;
                  }
                #endif
              #endif

              /*
              **   if process-state hasn't changed
              */
              if( NextState != XDASHSTATE )
              {
                XMK_BEGIN_CRITICAL_PATH;
                /*
                **   save nextstate
                */
                p_ReceiverProcess->ProcessStateTable [EPIDINST(xRunPID)] = NextState;
                XMK_END_CRITICAL_PATH;
              }

              /*
              **   remove current signal from the queue
              */
              xmk_RemoveCurrentSignal () ;

              #ifdef XMK_USED_DYNAMIC_STOP
                /*
                **   if receiving instance has performed an SDL-STOP
                */
                if( NextState == XDORMANT )
                {
                  #ifdef XMK_USED_TIMER
                    /*
                    **
                    */
                    xmk_ResetAllTimer( xRunPID) ;
                  #endif
                  /*
                  **   remove all the signals from the queue, that were
                  **   directed to the stopped instance
                  */
                  xmk_RemoveSignalsByProcessID ( xRunPID ) ;

                  /*
                   * Dynamic instance data:
                   * A process instance is being deleted, so we free
                   * the instance data that was allocated to it,
                   * and set the pointer to NULL.
                   */
#ifndef XMK_USED_ONLY_X_1
                  OSConnectorFree (((unsigned char **)(p_ReceiverProcess->pInstanceData))[EPIDINST (xRunPID)]);
                  ((unsigned char **)p_ReceiverProcess->pInstanceData)[EPIDINST (xRunPID)] = 0;
#else
                  OSConnectorFree (p_ReceiverProcess->pInstanceData);
                  p_ReceiverProcess->pInstanceData = 0;
#endif
                } /* END IF */
              #endif /* ... XMK_USED_DYNAMIC_STOP */

              #ifdef XMK_ADD_TSTATE
                /*
                ** Trace the next state operation ...
                */
                xmk_TSDL_State (NextState);
              #endif

              #ifdef XMK_USE_DEBUGGING
                #ifdef XMK_ADD_CBREAK_LOGIC
                  if (NextState == XDASHSTATE)
                    NextState = p_ReceiverProcess->ProcessStateTable [EPIDINST(xRunPID)];

                  /*
                  **  Check whether the current system state is a breakpoint
                  */
                  if (xmk_CheckIfBreakpointTrue(xRunPID, XMK_NO_SIGNAL,
                                                NextState, &BreakHitDescr))
                  {
                    XMK_BEGIN_CRITICAL_PATH;
                      xmk_SystemState |= XMK_BREAKPOINT_HIT;
                    XMK_END_CRITICAL_PATH;

                    #ifdef XMK_ADD_PRINTF_ADDITIONAL
                      PRINTF(("\n\n Breakpoint NextState was hit \n\n"));
                    #endif

                    #ifdef XMK_ADD_MICRO_COMMAND
                      (void) xmk_HandleHostCmd (CMD_BREAK_HIT_IND,
                                                (xmk_U_CMDS *) &BreakHitDescr);
                    #endif
                    return;
                  }
                #endif /* ... XMK_ADD_CBREAK_LOGIC */
              #endif /* ... XMK_USE_DEBUGGING */

              #if defined(XMK_USE_DEBUGGING) || defined(XMK_USE_MICRO_COMMAND) || defined(LAUTERBACH)
                /*
                ** Set xRunPID no longer active, relevant only for
                ** debug, trace and command interface
                */
                xRunPID = xNULLPID;
                #ifdef XMK_USE_PREEMPTIVE
                  CurrentPrioLevel = MAX_PRIO_LEVELS;
                #endif
              #endif

              #ifdef XMK_USE_PREEMPTIVE
                #ifdef XMK_USED_TIMER
                  #ifdef XMK_USE_DEBUGGING
                    /*
                    **  Timer expired check is only to be done
                    **  if timers aren't disabled
                    */
                    if (!(xmk_SystemState & XMK_TIMER_DISABLED))
                  #endif
                      xmk_ChkTimer();
                #endif

                #ifdef XMK_USE_xInEnv
                  xInEnv();
                  #if defined(XMK_USE_DEBUGGING) && (defined(XMK_ADD_CREINIT) || defined(XMK_ADD_CSHUTDOWN))
                    if (xmk_SystemState & (XMK_REINIT_SYSTEM | XMK_SYSTEM_SHUTDOWN) )
                    {
                      XMK_SUPPRESS(xmk_SystemState);
                      return ;
                    }
                  #endif /* ... XMK_ADD_CREINIT */
                #endif
                p_Message = xmk_FirstSignal ();
              #else
                /*
                **   leave signal-handling (leave while - loop)
                */
                p_Message = NULL ;
                break ;
              #endif
            } /* END DEFAULT */
          } /* END SWITCH */
        } /* END IF */
      } /* END WHILE */

  #ifdef XMK_USE_DEBUGGING
    }
  #endif

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*ProcessSignal");
  #endif

  #undef CurrentState
  #undef NextState

} /* END OF FUNCTION */

#if defined(XMK_USED_DYNAMIC_CREATE) || defined(XMK_ADD_CCREATE_PROCESS)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*CreateProcess                                            |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|  This function tries to create an instance of the given process-type.        |
|  This can fail, either if there is no allocatable <create-signal>, or        |
|  if there is no free <process-instance> of that type (no instance is         |
|  "DORMANT").                                                                 |
|                                                                              |
|  Parameter    : ProcessID - ID of the processtype                            |
|                                                                              |
|  Return       : ID of the newly created instance (0 .... NMax - 1 )          |
|                 xNULLINST                                                    |
|                    it wasn't possible to create a new instance               |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_INSTANCE xmk_CreateProcess ( xmk_T_PROCESS ProcessTypeID )
#else
  xmk_T_INSTANCE xmk_CreateProcess ( ProcessTypeID )
  xmk_T_PROCESS ProcessTypeID ;
#endif

/*-FDEF E*/

{
  /*
  **   Defines
  */
  xmk_T_MESSAGE      xmk_RAM_ptr p_CreMsg;     
                                     /* Create-message                      */
  xmk_T_INSTANCE      InstanceID;    /* ID of the created instance          */
  XCONST XPDTBL       xmk_ROM_ptr pProcessType;  
                                     /* pointer to global process           */
                                     /* description table                   */
  xmk_T_BOOL          found;         /* internal value for local search     */
  #ifdef XMK_USED_OFFSPRING
    xPIDTable        xmk_RAM_ptr pCurrentProcessPIDTable ; 
                                                /* Ptr to xPID-table of the */
                                                /* Creating process         */
  #endif

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*CreateProcess");
  #endif

  #if defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_ADD_CCREATE_PROCESS)
    /*
    ** This error check is unnecessary for generated C code,
    ** but might be necessary if users want to create processes in
    ** their C code.
    **
    ** CAUTION !
    ** ==========
    ** The use of the possibility to create processes in C code is 
    ** not recommended.
    */
    if (! xmk_IfExist ( ProcessTypeID ))
    {  
      ErrorHandler(ERR_N_PID_INDEX);
      return (xNULLINST);
    }  
  #endif /* ... XMK_USE_MAX_ERR_CHECK && XMK_ADD_CCREATE_PROCESS */

  /*
  **   load pointer with process-description-table of current processtype
  */
  pProcessType = xPDTBL [ ProcessTypeID ] ;

  /*
  **   search for a DORMANT-instance
  */
  found = XMK_FALSE;
  for( InstanceID = 0; InstanceID < pProcessType->MaxInstances; InstanceID++ )
  {
    if( pProcessType->ProcessStateTable [ InstanceID ] == XDORMANT )
    {
      found = XMK_TRUE;
      break ;
    } /* END IF */
  } /* END FOR */

  /*
  **   if there is a DORMANT-instance
  */
  if( found )
  {
    /*
    **   try to allocate a create-signal
    */
    p_CreMsg = xmk_AllocSignal () ;

    /*
    **   if allocation failed
    */
    if( p_CreMsg == NULL )
    {
      /*
      **   call error-handling routine
      */
      ErrorHandler (ERR_N_CREATE_NO_MEM);

      /*
      **   prepare return-value to indicate that creation failed
      */
      InstanceID = xNULLINST ; /* == SDL_NULL */

      /* 
      ** Setting SDL_OFFSPRING in the creating Process ... 
      */  
      #ifdef XMK_USED_OFFSPRING 
         /* 
         ** Kernel already knows the parent; parent can be
         ** evaluated by using the xRunPID information, which 
         ** stores the PID of the currently active process instance. 
         */ 
         pCurrentProcessPIDTable = xPDTBL [ RUN_PROC ]->pPIDTable; 
         pCurrentProcessPIDTable [RUN_INST].Offspring = xNULLPID;
      #endif 

    } /* END IF */
    else
    {
      XMK_BEGIN_CRITICAL_PATH;

      /*
      **   if allocation succeeded, set the state of
      **   the instance to XCREATION
      */
      pProcessType->ProcessStateTable [ InstanceID ] = XCREATION;

      /*
       * Dynamic instance data:
       * A new process instance is being created, so we need to
       * allocate space for its data.
       */
#ifndef XMK_USED_ONLY_X_1
      ((unsigned char **)(pProcessType->pInstanceData))[InstanceID] =
        OSConnectorAlloc (pProcessType->DataLength);
      memset (((unsigned char **)(pProcessType->pInstanceData))[InstanceID],
              0, pProcessType->DataLength);  
#else
      pProcessType->pInstanceData =
        OSConnectorAlloc (pProcessType->DataLength);
      memset (pProcessType->pInstanceData, 0, pProcessType->DataLength);  
#endif

      XMK_END_CRITICAL_PATH;
      /*
      **   register parameters
      */
      p_CreMsg->rec         = GLOBALPID(ProcessTypeID,InstanceID) ;
      p_CreMsg->signal      = XMK_CREATE_SIGNALID ;

      #ifdef XMK_USE_SIGNAL_PRIORITIES
        p_CreMsg->prio      = XMK_CREATE_PRIO     ;
      #endif

      /*
      **   Sender is currently active instance
      */
      #ifdef XMK_USE_SENDER_PID_IN_SIGNAL
         p_CreMsg->send     = xRunPID             ;
      #endif

      #ifdef XMK_USED_SIGNAL_WITH_PARAMS
         p_CreMsg->mess_length = 0                ;
      #endif

      #ifdef XMK_ADD_TCREATE
        xmk_TSDL_Create (p_CreMsg);
      #endif

      /*
      ** Setting SDL_OFFSPRING in the creating Process ...
      */
      #ifdef XMK_USED_OFFSPRING
         /* 
         ** Kernel already knows the parent; parent can be 
         ** evaluated by using the xRunPID information, which 
         ** stores the PID of the currently active process instance.
         */ 
         pCurrentProcessPIDTable = xPDTBL [ RUN_PROC ]->pPIDTable;
         pCurrentProcessPIDTable [RUN_INST].Offspring = GLOBALPID(ProcessTypeID,InstanceID) ;
      #endif

      /*
      **   insert signal into queue
      */
      xmk_InsertSignal ( p_CreMsg ) ;
    } /* END ELSE */
  } /* END IF */
  else
  {
    /*
    **   if no instance found XDORMANT
    **   prepare return-value to indicate that creation failed
    */
    InstanceID = xNULLINST ; /* == SDL_NULL */

    /*
    **   and call error-handling routine
    */
    ErrorHandler (ERR_N_CREATE_INSTANCE);

    /*
    ** Setting SDL_OFFSPRING in the creating Process ...
    */ 
    #ifdef XMK_USED_OFFSPRING
       /* 
       ** Kernel already knows the parent; parent can be 
       ** evaluated by using the xRunPID information, which 
       ** stores the PID of the currently active process instance.
       */ 
       pCurrentProcessPIDTable = xPDTBL [ RUN_PROC ]->pPIDTable;
       pCurrentProcessPIDTable [RUN_INST].Offspring = xNULLPID;
    #endif

  } /* END ELSE */

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*CreateProcess");
  #endif

  /*
  **   return ID of the created instance respectively error notification
  */
  return( InstanceID ) ;
} /* END OF FUNCTION */

#endif /* XMK_USED_DYNAMIC_CREATE */

#ifdef XMK_USED_DYNAMIC_CREATE

/*+FHDR S*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_HandleSystemSignal                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|  This function handles systems signals. At the moment, there is only         |
|  one systemsignal defined.                                                   |
|  Create-Signal: The <start-transition> of the <process-instance> to be       |
|  created will be executed. SDL_PARENT has to be stored in the PID-table of   |
|  the created process-instance.                                               |
|                                                                              |
|  Parameter    : p_Message - Pointer to systemsignal                          |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR S*/

/*+FDEF S*/

#ifndef XNOPROTO
  /*static*/ void xmk_HandleSystemSignal ( xmk_T_MESSAGE xmk_RAM_ptr p_Message )
#else
  static void xmk_HandleSystemSignal ( p_Message )
  xmk_T_MESSAGE xmk_RAM_ptr p_Message;
#endif

/*-FDEF S*/
{
  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*HandleSystemSignal");
  #endif

  /*
  **   examine signal's code
  */
  switch( p_Message->signal )
  {
    #ifdef XMK_USED_DYNAMIC_CREATE
      /*
      **  if signal is create-signal
      */
      case XMK_CREATE_SIGNALID:
      {
        XCONST XPDTBL        xmk_ROM_ptr p_ReceiverProcess  ;
        xmk_T_TRANS_ADDRESS  TransitionFunction ;

        #if defined(XMK_USED_SENDER) && defined(XMK_USE_MAX_ERR_CHECK) && defined(XMK_USE_SAFE_ADDRESSING)
          /*
          ** For evaluation of SDL_SENDER expression :
          */
          xmk_SignalInstPtr= &xmk_EmptySignalVar;
        #endif

        /*
        **   load pointer with process-description-table of current processtype
        */
        p_ReceiverProcess = xPDTBL [ RUN_PROC ] ;

        /*
        **   if the state of the instance is XCREATION
        */
        if( p_ReceiverProcess->ProcessStateTable [ RUN_INST ] == XCREATION )
        {
          XMK_BEGIN_CRITICAL_PATH;

          /*
          **   load instance-data
          */
          /*
           * Dynamic instance data:
           * The instance data was allocated already when this process
           * was set up for creation, so here we just need to set
           * the pRunData pointer.
           */
#ifndef XMK_USED_ONLY_X_1
          pRunData = ((unsigned char **)(p_ReceiverProcess->pInstanceData))[RUN_INST];
#else
          pRunData = p_ReceiverProcess->pInstanceData;
#endif

          #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
            /*
            ** Evaluate pointer to table with parent/offspring value
            */
            pRunPIDTable = &xPDTBL[ RUN_PROC ]->pPIDTable [RUN_INST];

            /*
            ** Setting SDL_PARENT and SDL_OFFSPRING
            */
            #ifdef XMK_USED_PARENT
              pRunPIDTable->Parent = p_Message->send;
            #endif

            #ifdef XMK_USED_OFFSPRING
             pRunPIDTable->Offspring = xNULLPID;
            #endif
          #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

          XMK_END_CRITICAL_PATH;

          #ifdef XMK_USE_KERNEL_INIT
            memset (pRunData, 0, p_ReceiverProcess->DataLength);
          #endif

          /*
          **   load transitionfunction
          */
          TransitionFunction = xPDTBL [ RUN_PROC ]->yPAD_Function ;

          #ifdef XMK_ADD_TDYNAMIC_CREATE
            xmk_TSDL_DynamicCreate (p_Message->send, xRunPID);
          #endif

          #ifdef XMK_USE_BANKSW
            XMK_SWITCH_BANK(EPIDTYPE(xRunPID));
          #endif

          /*
          **   execute transitionfunction and store nextstate
          */
          #ifdef XMK_USE_PREEMPTIVE
            p_ReceiverProcess->ProcessStateTable [ RUN_INST ] =
                                        TransitionFunction(XSTART_TRANSITION) ;
          #else
            xmk_SymbolNrVar  = 0;
            p_ReceiverProcess->ProcessStateTable [ RUN_INST ] =
                                        TransitionFunction() ;
          #endif

          #ifdef XMK_ADD_TSTATE
            /*
            ** Trace the next state operation ...
            */
            xmk_TSDL_State (p_ReceiverProcess->ProcessStateTable [ RUN_INST ]);
          #endif

        } /* END IF */

        else
        {
          /*
          **   The state of the instance differs from XCREATION
          **   call error handling routine
          */
          ErrorHandler (ERR_N_CREATE_INSTANCE);

        } /* END ELSE */


        /*
        **    remove create-signal from the queue
        */
        xmk_RemoveCurrentSignal ();

        #if defined(XMK_USE_DEBUGGING) || defined(XMK_USE_MICRO_COMMAND)
          /*
          ** Set xRunPID no longer active, relevant only for
          ** debug, trace and command interface
          */
          xRunPID = xNULLPID;
          
          #ifdef XMK_USE_PREEMPTIVE
            
            CurrentPrioLevel = MAX_PRIO_LEVELS;
          #endif
          
        #endif

        break ;
      } /* END CASE */

    #endif /* XMK_USED_DYNAMIC_CREATE */

    default :

      #ifdef XMK_USE_NO_ERR_CHECK
        /*
        ** really NO error check in this case
        */
      #else
        /*
        **   signal wasn't a create-signal
        */
        ErrorHandler (ERR_N_SYSTEM_SIGNAL);

        /*
        **   remove create-signal from queue
        */
        xmk_RemoveCurrentSignal () ;
      #endif

      break ;
  } /* END SWITCH */

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*HandleSystemSignal");
  #endif

} /* END OF FUNCTION */

#endif /* XMK_USED_DYNAMIC_CREATE */

/*+FHDR S*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_CheckSignal                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|  The function checks, if the signal to work on is defined either in          |
|  the current state or in the ANY state of the process.                       |
|                                                                              |
|  The function normally returns an index to the entry found in the            |
|  <transition-table>.                                                         |
|                                                                              |
|  If the signal is to be saved, the function returns with "SAVE".             |
|                                                                              |
|  If the signal is not defined for the current state of the process,          |
|  the function returns DISCARD.                                               |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : SAVE    - Signal has to be saved                             |
|                 DISCARD - Signal isn't expected neither in current state     |
|                           nor in an ANY-state of the receiver.               |
|                           Thus, the signal has to be ignored and an          |
|                           error-handling has to be invoked                   |
|                 IMPLICIT_CONSUMPTION Signal was implicitely consumed, or     |
|                           signal is not defined in valid input signal set.   |
|                 else    - Index to transition to be performed                |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR S*/

/*+FDEF S*/

#ifndef XNOPROTO
  /*static*/ int xmk_CheckSignal ( xmk_T_SIGNAL SignalID, xmk_T_STATE CurrentState,
                               XCONST XPDTBL xmk_ROM_ptr p_CurrentProcess )
#else
  static int xmk_CheckSignal ( SignalID, CurrentState, p_CurrentProcess )
  xmk_T_SIGNAL        SignalID;
  xmk_T_STATE         CurrentState;
  XCONST XPDTBL       xmk_ROM_ptr p_CurrentProcess;
#endif

/*-FDEF S*/
{
  /*
  **   Defines
  */
  XCONST  xSITBL  xmk_ROM_ptr StateIndexTable;  
                                        /* Pointer to <state-index-table> of  */
                                        /* .. receiving processtype           */
  XCONST  xTRTBL  xmk_ROM_ptr TransitionTable;  
                                        /* Pointer to <transition-table>      */
                                        /* .. receiving processtype           */
  xmk_T_OFFSET        IndexToFirstEvent;
                                        /* Index to first expected signal in  */
                                        /* .. event-table                     */
  xmk_T_OFFSET        IndexToLastEvent; 
                                        /* Index to last expected signal in   */
                                        /* .. event-table                     */
  int                 IndexOfExplicitElse = XMK_NO_ASTERISK;  
                                        /* Used to store whether there is     */
                                        /* .. an ANY-state or not             */

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*CheckSignal");
  #endif

  /*
  **   if receiving process-instance is in state XDORMANT
  */
  if( CurrentState == XDORMANT )
  {
    #ifdef XMK_ADD_PRINTF_SCHEDULER
      XMK_TRACE_EXIT("*SI*CheckSignal");
    #endif
    /*
    **   return value to invoke error handling
    */
    return( DISCARD ) ;
  }


  /*
  **   load pointer to <state-index-table>
  */
  StateIndexTable = p_CurrentProcess->StateIndexTable ;

  /*
  **   load pointer to <transition-table>
  */
  TransitionTable = p_CurrentProcess->TransitionTable ;

  /*
  **   If there is a signal expected in current state
  */
  if( StateIndexTable[CurrentState-1].Offset
      != StateIndexTable[CurrentState].Offset)
  {
    /*
    **   load index of first expected signal in current state
    **   from state-index-table
    */
    IndexToFirstEvent = StateIndexTable[CurrentState-1].Offset;

    /*
    **   calculate index of last expected signal in current state
    **   from state-index-table
    */
    IndexToLastEvent  = StateIndexTable[CurrentState].Offset - 1 ;

    /*
    **   if last entry in transition-table equals XASTERISK
    */
    if( TransitionTable [ IndexToLastEvent ].SignalID == XASTERISK )
    {
      /*
      **   store this entry
      */
      IndexOfExplicitElse = IndexToLastEvent ;

      /*
      **  decrement index to last event
      **  Do not decrement this, if there is no other event than 
      **  XASTERISK in the TR TBL.
      */
      if (IndexToLastEvent != IndexToFirstEvent)
        IndexToLastEvent-- ;
    }

    else
    {
      /*
      **   no XASTERISK entry in transition-table
      **   Corrected marker from 0 to -1 
      */
      IndexOfExplicitElse = XMK_NO_ASTERISK ;
    }

    /*
    **   for all the other signals expected
    */
    for( ; IndexToFirstEvent <= IndexToLastEvent ; IndexToFirstEvent++ )
    {
      /*
      **   if an expected signal is found
      */
      if( SignalID == TransitionTable[ IndexToFirstEvent ].SignalID )
      {

        #ifdef XMK_USED_SAVE
          /*
          **   if signal is to be saved
          */
          if( TransitionTable [ IndexToFirstEvent ].SymbolNr == XSAVEID )
          {
            #ifdef XMK_ADD_PRINTF_SCHEDULER
              XMK_TRACE_EXIT("*SI*CheckSignal");
            #endif
            /*
            **   return SAVE to inform the caller
            */
            return( SAVE ) ;
          }
          else
        #endif /* XMK_USED_SAVE */
        /*
        **   return index
        */
        {
          #ifdef XMK_ADD_PRINTF_SCHEDULER
            XMK_TRACE_EXIT("*SI*CheckSignal");
          #endif
          return( IndexToFirstEvent ) ;
        } /* END IF */
      } /* END IF */
    } /* END FOR */
  } /* END IF */

  /*
  **   Up to now only the current state of the instance has been treated
  */

  /*
  **   if an ASTERISK-input exists for this process
  **   Corrected marker from 0 to -1 
  */
  if( IndexOfExplicitElse != XMK_NO_ASTERISK )
  {
    /*
    **   If ASTERISK input says to save the signal
    */
    #ifdef XMK_USED_SAVE
      if( TransitionTable [ IndexOfExplicitElse ].SymbolNr == XSAVEID )
      {
        #ifdef XMK_ADD_PRINTF_SCHEDULER
          XMK_TRACE_EXIT("*SI*CheckSignal");
        #endif
        /*
        **   return SAVE to inform the caller
        */
        return( SAVE ) ;
      }

      else
    #endif /* XMK_USED_SAVE */
    /*
    **   return index
    */
    {
      #ifdef XMK_ADD_PRINTF_SCHEDULER
        XMK_TRACE_EXIT("*SI*CheckSignal");
      #endif
      return( IndexOfExplicitElse ) ;
    } /* END IF */
  } /* END IF */

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*CheckSignal");
  #endif
  /*
  **   signal cannot be treated
  **   so return implicit consumption.
  **   this includes also addressing errors, e.g.
  **   if a process never can consume the received
  **   signal, because it isn't in its valid input
  **   signal set.
  */
  return( IMPLICIT_CONSUMPTION ) ;

} /* END OF FUNCTION */

#ifdef XMK_USE_SDL_SYSTEM_STOP

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_IsAnyProcessAlive                                        |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|    This function checks for any active instance by searching for             |
|    one not being in state XDORMANT.                                          |
|                                                                              |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : XMK_TRUE  - There is an active instance                      |
|                 XMK_FALSE - No active instances throughout the system        |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_BOOL xmk_IsAnyProcessAlive ( void )
#else
  xmk_T_BOOL xmk_IsAnyProcessAlive ( )
#endif

/*-FDEF E*/

{
  unsigned char        Process  ;
  unsigned char        Instance ;

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*IsAnyProcessAlive");
  #endif

  Process  = 0;

  /*
  **   Check for all process-types
  */

  while ( xPDTBL[Process] != X_END_ROOT_TABLE )
  {
    Instance = 0;
    /*
    **   for all the instances of this type
    */
    while ( Instance < xPDTBL[Process]->MaxInstances )
    {
      if ( xPDTBL[Process]->ProcessStateTable[Instance] != XDORMANT )
      {

        #ifdef XMK_ADD_PRINTF_SCHEDULER
          XMK_TRACE_EXIT("*SI*IsAnyProcessAlive");
                #endif

        /*
        **    there is a process still alive -> return XMK_TRUE
        */

        return ( XMK_TRUE );
      }
      Instance++;
    }
    Process++;
  }

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*IsAnyProcessAlive");
  #endif

  /*
  **    there is no process in the system alive
  */
  return ( XMK_FALSE );

} /* END OF FUNCTION */

#endif /* ... XMK_USE_SDL_SYSTEM_STOP */

#ifdef XMK_USE_MAX_ERR_CHECK

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*IfExist                                                  |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 This function checks if the given PID is correct or not,     |
|                 which means if the given PID is an existing value.           |
|                                                                              |
|  Parameter    :   ProcessID -  value to be checked                           |
|                                                                              |
|  Return       :   XMK_TRUE  if value is correct                              |
|                   XMK_FALSE if instance with this ID can never exist         |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_BOOL xmk_IfExist ( xPID ProcessID  )
#else
  xmk_T_BOOL xmk_IfExist ( ProcessID )
  xPID ProcessID;
#endif

/*-FDEF E*/

{
  xmk_T_PROCESS  Process;
  xmk_T_INSTANCE Instance;

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*IfExist");
  #endif

  Process  = EPIDTYPE (ProcessID);
  Instance = EPIDINST (ProcessID);

  if (Process < MAX_SDL_PROCESS_TYPES)
  {
    if ( Instance < xPDTBL[Process]->MaxInstances )
    {
      #ifdef XMK_ADD_PRINTF_SCHEDULER
        XMK_TRACE_EXIT("*SI*IfExist");
      #endif
      return (XMK_TRUE);
    }
  }
  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*IfExist");
  #endif
  return (XMK_FALSE);

} /* END OF FUNCTION */

#endif /* ... XMK_USE_MAX_ERR_CHECK */

#ifdef XMK_USE_MAX_ERR_CHECK

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*CheckNullPointerValue                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 This function checks whether there is a pointer value in     |
|                 the SDL system that is used but has no value. The            |
|                 ErrorHandler is called with the right error message then.    |
|                 The error must be caught in the user's ErrorHandler to       |
|                 implement the right reaction on this fatal situation.        |
|                                                                              |
|  Parameter    :   void * Ptr-  value to be checked                           |
|                                                                              |
|  Return       :   void                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_CheckNullPointerValue ( void * Ptr  )
#else
  void xmk_CheckNullPointerValuet ( Ptr )
  void * Ptr;
#endif

/*-FDEF E*/

{
 
  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*CheckNullPointerValue");
  #endif

  if ( ! Ptr )
  {
      ErrorHandler (ERR_N_NULL_POINTER_VALUE_USED);
  }

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*CheckNullPointerValue");
  #endif

  return ;

} /* END OF FUNCTION */

#endif /* ... XMK_USE_MAX_ERR_CHECK */

/*+PREEMPT*/
#ifdef XMK_USE_PREEMPTIVE

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_InitPreemptionVars                                       |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 The variables used in preemption are initialized.            |
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
  void xmk_InitPreemptionVars ( void )
#else
  void xmk_InitPreemptionVars ()
#endif

/*-FDEF E*/

{
   xmk_OPT_INT i;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("xmk_InitPreemptionVars");
   #endif

   /*
   **   Set all the variables used in preemption to a defined value
   */
   xmk_PreemptionDisabled = 0;
   CurrentPrioLevel       = 0;
   HighestPrioLevel       = MAX_PRIO_LEVELS;

   for (i=0; i<MAX_PRIO_LEVELS; i++)
   {
     Prio_Queue[i] = NULL;
     Prio_CurrentSignal[i] = NULL;
     Prio_xRunPID[i] = xNULLPID;
     #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
       Prio_pRunPIDTable[i] = NULL;
     #endif /* ... defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */
     Prio_pRunData[i] = NULL;
   }
   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("xmk_InitPreemptionVars");
   #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_DisablePreemption                                        |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 The variable which stores the ability to preempt is          |
|                 incremented. A value greater than zero means it isn't        |
|                 allowed to perform a context-switch at the moment.           |
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
  void xmk_DisablePreemption ( void )
#else
  void xmk_DisablePreemption ()
#endif

/*-FDEF E*/

{

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("xmk_DisablePreemption");
   #endif

   XMK_BEGIN_CRITICAL_PATH;

   /*
   **   A value greater than zero means context-switch isn't allowed
   */

   xmk_PreemptionDisabled++;

   XMK_END_CRITICAL_PATH;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("xmk_DisablePreemption");
   #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_EnablePreemption                                         |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 The variable which stores the ability to preempt is          |
|                 decremented if preemption was disabled. If the variable's    |
|                 value equals zero afterwards, the function                   |
|                 xmk_CheckIfSchedule is called.                               |
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
  void xmk_EnablePreemption( void )
#else
  void xmk_EnablePreemption()
#endif

/*-FDEF E*/

{                                /* Beginning of Function */

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("xmk_EnablePreemption");
   #endif

   /*
   **    Is context-switch currently disabled ??
   */

   if (xmk_PreemptionDisabled)
   {                             /* 1st Then-branch */

      XMK_BEGIN_CRITICAL_PATH;

      /*
      **   decrement the counter
      */

      xmk_PreemptionDisabled--;

      XMK_END_CRITICAL_PATH;

      /*
      **   If context-switch is permitted now
      */

      if (!xmk_PreemptionDisabled)
      {                          /* 2nd Then-branch */
         /*
         **   perform check if necessary to switch to another prio-level
         */

         xmk_CheckIfSchedule();
      }                          /* 2nd Then-branch */
   }                             /* 1st Then-branch */
   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("xmk_EnablePreemption");
   #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_FetchHighestPrioLevel                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 This function searches for signals in the queues. This is    |
|                 done with decreasing priority in order to find the highest   |
|                 priority level where there is a signal to work on.           |
|                 The return value is the highest priority level containing a  |
|                 signal to work on.                                           |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : xmk_T_PRIOLEVEL highest prio-level, where there is a signal  |
|                 to be dealt with                                             |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_PRIOLEVEL xmk_FetchHighestPrioLevel ( void )
#else
  xmk_T_PRIOLEVEL xmk_FetchHighestPrioLevel ()
#endif

/*-FDEF E*/

{                               /* Beginning of function */
   X_REGISTER unsigned char i;


   #ifdef XMK_USED_SAVE

     xmk_T_PROCESS Process  ;
     xmk_T_PROCESS Instance ;
     int        TestResult;
     T_E_SIGNAL xmk_RAM_ptr rover ;

     i = 0;

     #ifdef XMK_ADD_PRINTF_SCHEDULER
       XMK_FUNCTION("xmk_FetchHighestPrioLevel");
     #endif

     /*
     **    for all the priority levels
     */
     while ( i < MAX_PRIO_LEVELS )
     {

       /*
       **    if there is a signal
       */
       if (Prio_Queue[i] != NULL)
           {

             /*
             **   search for a signal in the queue which doesn't carry
             **   the Save-ID in the current state of the process, i.e.
             **   which has to be treated now
             */
             rover = Prio_Queue[i];
                 while (rover != NULL)
                 {
                   /*
                   **    If there is such a signal
                   */
                   Process  = EPIDTYPE(rover->Signal.rec);
                   Instance = EPIDINST(rover->Signal.rec);

                   if ( ((xPDTBL[Process]->ProcessStateTable[Instance])
                          != rover->SaveState)
                      || (TestResult =
                           xmk_CheckSignal ((rover->Signal.signal),
                           (xPDTBL[Process]->ProcessStateTable[Instance]),
                           (xPDTBL[Process])) != SAVE)
                             )
                   {
                     #ifdef XMK_ADD_PRINTF_SCHEDULER
                       XMK_TRACE_EXIT("xmk_FetchHighestPrioLevel");
                     #endif
                     /*
                     **   return index of highest prio-level to work on
                     */
                     return (i);
                    }
                    rover = rover->next;
                 }
           }
           /*
           **   next priority level
           */
           i++;
     }
     #ifdef XMK_ADD_PRINTF_SCHEDULER
       XMK_TRACE_EXIT("xmk_FetchHighestPrioLevel");
     #endif

     return(MAX_PRIO_LEVELS);

   #else

     #ifdef XMK_ADD_PRINTF_SCHEDULER
       XMK_FUNCTION("xmk_FetchHighestPrioLevel");
     #endif

     /*
     **   search for a signal in all the queues
     */
     for (i=0; ((i < MAX_PRIO_LEVELS) && (Prio_Queue[i] == NULL)); i++);

     /*
     **   no signals in any queue
     */
     if (Prio_Queue[i] == NULL)
     {
       #ifdef XMK_ADD_PRINTF_SCHEDULER
         XMK_TRACE_EXIT("xmk_FetchHighestPrioLevel");
           #endif

       /*
       **   return index to enable caller to detect
       **   there is no signal in any queue
       */
       return(MAX_PRIO_LEVELS);
     }
     else
     {
           #ifdef XMK_ADD_PRINTF_SCHEDULER
         XMK_TRACE_EXIT("xmk_FetchHighestPrioLevel");
           #endif
       /*
       **   return index to highest prio-level where
       **   there is a signal to work on
       */
       return (i);
     }
   #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_CheckIfSchedule                                          |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 It is checked whether a context switch is admissible.        |
|                 If true the current priority-level a signal is worked on     |
|                 is compared with the highest priority-level where there is a |
|                 signal to work on. Supposing the highest level is higher     |
|                 than the current, a context-switch is performed using        |
|                 xmk_SwitchPrioLevel.                                         |
|                 This is repeated until the current priority-level is the     |
|                 highest level to work on.                                    |
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
  void xmk_CheckIfSchedule( void )
#else
  void xmk_CheckIfSchedule()
#endif

/*-FDEF E*/

{           /* Beginning of function */

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("xmk_CheckIfSchedule");
   #endif

   /*
   **   Is context-switch allowed at all ??
   */

   if (!xmk_PreemptionDisabled)
   {        /* Then-branch */

      XMK_BEGIN_CRITICAL_PATH;

      /*
      **   Save current prio-level and check for highest prio-level
      */

      HighestPrioLevel = xmk_FetchHighestPrioLevel();

      XMK_END_CRITICAL_PATH;

      /*
      **   work on signals on other prio-levels until
      **   the current level is the highest one again
      */

      while (HighestPrioLevel < CurrentPrioLevel)
      {     /* while - loop */
            xmk_SwitchPrioLevel(HighestPrioLevel);

            XMK_BEGIN_CRITICAL_PATH;

            HighestPrioLevel = xmk_FetchHighestPrioLevel();

            XMK_END_CRITICAL_PATH;

      }     /* while - loop */

   }        /* Then-branch */

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("xmk_CheckIfSchedule");
   #endif

} /* END OF FUNCTION */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_SwitchPrioLevel                                          |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 The global variables for the current priority-level are      |
|                 stored. Afterwards, the function xmk_ProcessSignal is called |
|                 in order to deal with the signals for this higher level.     |
|                 After returning from this function call the variables for    |
|                 the current priority-level are restored.                     |
|                                                                              |
|  Parameter    : xmk_T_PRIOLEVEL NewPrioLevel - next prio-level to deal with  |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_SwitchPrioLevel( xmk_T_PRIOLEVEL NewPrioLevel )
#else
  void xmk_SwitchPrioLevel( NewPrioLevel )
  xmk_T_PRIOLEVEL NewPrioLevel;
#endif

/*-FDEF E*/

{                               /* Beginning of function */
   xmk_T_PRIOLEVEL PrioLevel;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("xmk_SwitchPrioLevel");
   #endif

   XMK_BEGIN_CRITICAL_PATH;

   /*
   **   Save all the data of the currently running
   **   process (depending on the prio-level)
   */

   PrioLevel = EPRIOLEVEL;
   Prio_xRunPID[PrioLevel] = xRunPID;


   #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
     Prio_pRunPIDTable[PrioLevel] = pRunPIDTable;
   #endif /* defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

   Prio_pRunData[PrioLevel] = pRunData;

   /*
   **    Retrieve settings of next prio-level
   */
   xmk_Queue = &Prio_Queue[NewPrioLevel];
   xmk_CurrentSignal = &Prio_CurrentSignal[NewPrioLevel];

   XMK_END_CRITICAL_PATH;

   /*
   **    Work on next prio-level by calling the function
   */
   xmk_ProcessSignal ();

   XMK_BEGIN_CRITICAL_PATH;

   /*
   **    Retrieve data of the currently running process
   */
   xRunPID = Prio_xRunPID[PrioLevel];

   #if defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING)
     pRunPIDTable = Prio_pRunPIDTable[PrioLevel];
   #endif /* defined(XMK_USED_PARENT) || defined(XMK_USED_OFFSPRING) */

   pRunData = Prio_pRunData[PrioLevel];

   xmk_Queue = &Prio_Queue[PrioLevel];
   xmk_CurrentSignal = &Prio_CurrentSignal[PrioLevel];
   xmk_SignalInstPtr = (xmk_T_MESSAGE xmk_RAM_ptr) ( *xmk_CurrentSignal);
   CurrentPrioLevel = PrioLevel;

   XMK_END_CRITICAL_PATH;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("xmk_SwitchPrioLevel");
   #endif

} /* END OF FUNCTION */

#endif  /* XMK_USE_PREEMPTIVE */
/*-PREEMPT*/

/* compare with dependence in queue module !*/

#if defined(XMK_USE_CMD_KILL) || defined(XMK_ADD_CKILL_PROCESS)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*KillProcess                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 This function resets a process (more precise: an instance)   |
|                 into the state XDORMANT, removes all it's signals from the   |
|                 queue and resets the instance-local data.                    |
|                                                                              |
|  Parameter    :   ProcessID -  process to be killed                          |
|                                                                              |
|  Return       :   XMK_TRUE  - given process was killed                       |
|                   XMK_FALSE - given process wasn't killed either because it  |
|                               is non-existent or it is currently running, or |
|                               it was already in state XDORMANT               |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_BOOL xmk_KillProcess ( xPID ProcessID  )
#else
  xmk_T_BOOL xmk_KillProcess ( ProcessID )
  xPID ProcessID;
#endif

/*-FDEF E*/

{
  XCONST XPDTBL  xmk_ROM_ptr p_Process;
  xmk_T_PROCESS  Process;
  xmk_T_INSTANCE Instance;

  #ifdef XMK_USE_PREEMPTIVE
    unsigned char priority;
  #endif

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*KillProcess");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  Process  = EPIDTYPE (ProcessID);
  Instance = EPIDINST (ProcessID);

  /*
  **  check whether the given process-type-ID is valid
  */
  if (Process < MAX_SDL_PROCESS_TYPES)
  {
    /*
    **  load process-description-table
    */
    p_Process = xPDTBL[Process];
    /*
    **   check whether the given process-instance-ID is valid
    */
    if ( Instance < p_Process->MaxInstances )
    {
      /*
      **  global process-ID is valid,
      **  check whether the process is active
      */

      if (p_Process->ProcessStateTable[Instance] != XDORMANT)
      {
        /*
        **  If the process is currently running,
        **  killing it is impossible
        */
        #ifndef XMK_USE_PREEMPTIVE
          if (ProcessID != xRunPID)
        #else
          /*
          ** check for all prio-levels if the instance is running
          */
          for (priority = 0; ((priority < MAX_PRIO_LEVELS)
                             && (Prio_xRunPID[priority] != ProcessID)); priority++);
          if (priority == MAX_PRIO_LEVELS)

          /*
          **   instance isn't currently running
          */
        #endif

          {
            /*
            **   remove signals from the queue
            */
            xmk_RemoveSignalsByProcessID (ProcessID);

            /*
            **  reset the instance data
            */
            /*
             * Dynamic instance data:
             * I don't think we really need this, because this function
             * is not normally included; but just in case...
             */
#ifndef XMK_USED_ONLY_X_1
            OSConnectorFree (((unsigned char **)(p_Process->pInstanceData))[Instance]);
#else
            OSConnectorFree (p_Process->pInstanceData);
#endif


            /*
            **  set the state of the instance to XDORMANT
            */
            p_Process->ProcessStateTable[Instance] = XDORMANT;

            XMK_END_CRITICAL_PATH;

            #ifdef XMK_ADD_PRINTF_SCHEDULER
              XMK_TRACE_EXIT("*SI*KillProcess");
            #endif
            return(XMK_TRUE);
          }
      }
    }
  }

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*KillProcess");
  #endif

  return(XMK_FALSE);
} /* END OF FUNCTION */

#endif /* ... XMK_USE_CMD_KILL */

#if defined(XMK_USE_EXPANDED_KERNEL) || (defined(XMK_ADD_CQUERY_PROCESS) && defined(XMK_USE_DEBUGGING)) 

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*GetProcessState                                          |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|     The function evaluates the current SDL-state of the SDL-process with     |
|     the given process-PID and returns it to the caller.                      |
|     During SDL-systemstartup, one possible return value might be XSTARTUP.   |
|     This means, that a process will be started during systemstartup.         |
|     During normal execution and systemstartup, the returnvalue XDORMANT      |
|     may be returned. This means, that an SDL-processinstance is either       |
|     stopped or has never been created dynamically.                           |
|                                                                              |
|  Parameter    : xPID  Process-PID                                            |
|                                                                              |
|  Return       : xmk_T_STATE     state-Value                                  |
|                                 (may also be XSTARTUP / XDORMANT )           |
|                 XNOTEXISTENT    if process doesn't exists                    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_STATE xmk_GetProcessState ( xPID pid)
#else
  xmk_T_STATE xmk_GetProcessState ( pid )
  xPID pid;
#endif

/*-FDEF E*/

{
   XCONST XPDTBL  xmk_ROM_ptr p_Process  ;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("*SI*GetProcessState");
   #endif

   #ifdef XMK_USE_MAX_ERR_CHECK
     /*
     ** Check if process exists
     */
     if (! xmk_IfExist (pid))  return (XNOTEXISTENT);
   #endif

   /*
   **   load process-description-table of given instance
   */
   p_Process = xPDTBL [ EPIDTYPE (pid) ] ;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("*SI*GetProcessState");
   #endif
   /*
   **   return current state of process instance
   */
   return (p_Process->ProcessStateTable [ EPIDINST (pid) ]) ;
} /* END OF FUNCTION */

#endif /* ... defined(XMK_USE_EXPANDED_KERNEL) ||
    (defined(XMK_ADD_CQUERY_PROCESS) && || defined(XMK_USE_DEBUGGING)) */

#if defined(XMK_USE_EXPANDED_KERNEL) || (defined(XMK_ADD_CNEXTSTATE) && defined(XMK_USE_DEBUGGING))

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_SetProcessState                                          |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|     The function sets the SDL-state of the process that is addressed with    |
|     the first parameter to the given value within the second parameter.      |
|     Note, that the value is NOT CHECKED, because Cmicro does not store any   |
|     information about this in the generated transition tables.               |
|                                                                              |
|  Parameter    : xPID  Process-PID                                            |
|                                                                              |
|  Return       : xmk_T_STATE     state-Value                                  |
|                                 (may also be XSTARTUP / XDORMANT )           |
|                 XNOTEXISTENT    if process doesn't exists                    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_OPT_INT xmk_SetProcessState ( xPID pid, xmk_T_STATE state )
#else
  xmk_OPT_INT xmk_SetProcessState ( pid, state )
  xPID        pid;
  xmk_T_STATE state;
#endif

/*-FDEF E*/

{
   XCONST XPDTBL xmk_ROM_ptr p_Process  ;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("*SI*SetProcessState");
   #endif

   #ifdef XMK_USE_MAX_ERR_CHECK
     /*
     ** Check if process exists
     */
     if (! xmk_IfExist (pid))  return (XMK_ERROR);
   #endif

   /*
   **   load process-description-table of given instance
   */
   p_Process = xPDTBL [ EPIDTYPE (pid) ] ;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_TRACE_EXIT("*SI*SetProcessState");
   #endif
   /*
   **   set the new state of the process instance
   */
   p_Process->ProcessStateTable [ EPIDINST (pid) ] = state ;
   return (XMK_OKAY);
} /* END OF FUNCTION */

#endif /* ... defined(XMK_USE_EXPANDED_KERNEL) ||
    (defined(XMK_ADD_CNEXTSTATE) && || defined(XMK_USE_DEBUGGING)) */

#if defined(XMK_USE_EXPANDED_KERNEL)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_GetProcessInstanceData                                   |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                                                                              |
|     The function returns the address of the processinstancedata of the       |
|     given process-PID, or XNOTEXISTENT, if the pid is not existent.          |
|                                                                              |
|  Parameter    : xPID  Process-PID                                            |
|                                                                              |
|  Return       : void *                                                       |
|                 (void*) NULL    if process doesn't exists                    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void *      xmk_GetProcessInstanceData ( xPID pid )
#else
  void *      xmk_GetProcessInstanceData ( pid )
  xPID        pid;
#endif

/*-FDEF E*/

{
   XCONST XPDTBL xmk_ROM_ptr pProcessType;

   #ifdef XMK_ADD_PRINTF_SCHEDULER
     XMK_FUNCTION("*SI*GetProcessInstanceData");
   #endif

   #ifdef XMK_USE_MAX_ERR_CHECK
     /*
     ** Check if process exists
     */
     if (! xmk_IfExist (pid))  return ((void*) NULL);
   #endif

   /*
   **   load process-description-table of given instance
   */
   pProcessType = xPDTBL [ EPIDTYPE (pid) ] ;

   #ifndef XMK_USED_ONLY_X_1
     #ifdef XMK_ADD_PRINTF_SCHEDULER
       XMK_TRACE_EXIT("*SI*GetProcessInstanceData");
     #endif
     /*
     **   return the address of the data of the process instance 
     */
       /*
        * Dynamic instance data:
        * Just return the pointer value.
        */
       return ((void*) (((unsigned char **)( pProcessType->pInstanceData))[EPIDINST (pid)]));
   #else
     #ifdef XMK_ADD_PRINTF_SCHEDULER
       XMK_TRACE_EXIT("*SI*GetProcessInstanceData");
     #endif
     /*
     **   return the address of the data of the process instance 
     */
     return ((void*) pProcessType->pInstanceData);
   #endif
} /* END OF FUNCTION */

#endif /* ... defined(XMK_USE_EXPANDED_KERNEL) */

#if defined(SDLPRTST) && defined(tst_stop_all_prs)
  /*
  ** For Cmicro Kernel tests only
  */

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : *SI*StopAllProcesses                                         |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|                 This function sends a signal called "TST_STOP_ALL_PRS"       |
|                 to all process instances that are active.                    |
|                 This function is to be called as the last in a transition    |
|                 and the process calling it might not stop. Otherwise it      |
|                 will come to errormessages.                                  |
|                                                                              |
|  Parameter    :   void                                                       |
|                                                                              |
|  Return       :   void                                                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_StopAllProcesses ( void )
#else
  void xmk_StopAllProcesses (  )
#endif

/*-FDEF E*/

{
  unsigned char      Process  ;
  unsigned char      Instance ;
  xPID               LocalPID ;
  xPID               xtmpPID  ;
  XCONST XPDTBL xmk_ROM_ptr pProcessType;

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_FUNCTION("*SI*StopAllProcesses");
  #endif

  Process = 0;
  /*
  **  Loop in <root-process-table>
  */
  while ( (pProcessType = xPDTBL [Process]) != X_END_ROOT_TABLE )
  {
    Instance = 0 ;
    /*
    **  Loop in <process-state-table> for all instances of this processtype
    */
    while( Instance < pProcessType->MaxInstances )
    {
      if (( pProcessType->ProcessStateTable[Instance] == XSTARTUP ) ||
          ( pProcessType->ProcessStateTable[Instance] != XDORMANT ))
      {
        #ifdef XMK_ADD_PRINTF_ADDITIONAL
          xxmonPID ("Sending STOP SIGNAL TO Process :", GLOBALPID(Process,Instance));
        #endif /* ... XMK_ADD_PRINTF_ADDITIONAL */

        /*
        ** Create a TST_STOP_ALL_PRS Signal here for this process
        ** addressed with GLOBALPID(Process,Instance)
        ** Sender of the signal is treated as the environment
        */
        LocalPID = GLOBALPID(Process,Instance);
        XMK_SEND_ENV (ENV,tst_stop_all_prs,xDefaultPrioSignal,0,(void*) 0,LocalPID);
      } /* end if ... */
      Instance++ ;
    }
    Process++ ;
  } /* END WHILE (testing processtypes) */

  #ifdef XMK_ADD_PRINTF_SCHEDULER
    XMK_TRACE_EXIT("*SI*StopAllProcesses");
  #endif

} /* END OF FUNCTION */

#endif /* ... SDLPRTST */

#endif  /* __MK_SCHE_C_ */
