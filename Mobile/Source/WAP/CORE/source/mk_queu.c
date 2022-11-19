/*+MHDR*/
/*
# clearcase: CmicroRel2.3
+------------------------------------------------------------------------------+
|  Modulname    : MK_QUEU.C                                                    |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  This module represents the mechanism of the SDL-signalqueue.                |
|  A signal is represented by the following structure :                        |
|                                                                              |
|  +------------------------------------------------------+                    |
|  | Signal (signal-instance-data)                        |                    |
|  +------------------------------------------------------+                    |
|  | Pointer to successor                                 |                    |
|  +------------------------------------------------------+                    |
|  | SAVE-state                                           |                    |
|  +------------------------------------------------------+                    |
|                                                                              |
|  Normally, signals are put at the end of the linked list, but not in         |
|  the case of "SAVE".                                                         |
|                                                                              |
|  The ordering of the signals in the queue is as follows:                     |
|  1. order by priority. The signal with the highest priority is inserted in   |
|     the queue at the first position.                                         |
|  2. order by time of arrival. If several signals with the same priority      |
|     are queued, they are inserted according to their arrival.                |
|                                                                              |
|  Using a preemptive kernel, there possibly are several queues.               |
|  The user doesn't have to take regard of this fact, because the kernel       |
|  is the only client of the queues. The Cmicro Kernel's behaviour doesn't     |
|  change because of the the existence of more than one queue from the user's  |
|  point of view.                                                              |
|                                                                              |
|  The address of a signal-element is to be the same as the address of         |
|  the signal itself. The Cmicro Kernel uses the signal address to access a    |
|  signal.                                                                     |
|                                                                              |
|  M1 Errormessages, which are routed to the ErrorHandler                      |
|  --------------------------------------------------------                    |
|    - none -                                                                  |
|                                                                              |
|  M2 Exported functions of this module :                                      |
|  --------------------------------------------------------                    |
|    void             xmk_InitQueue                  (void)                    |
|    xmk_T_MESSAGE*   xmk_FirstSignal                (void)                    |
|    xmk_T_MESSAGE*   xmk_NextSignal                 (void)                    |
|    void             xmk_InsertSignal               (xmk_T_MESSAGE)           |
|    void             xmk_RemoveCurrentSignal        (void)                    |
|    void             xmk_RemoveSignalBySignalID     (xmk_T_SIGNAL)            |
|    void             xmk_RemoveSignalsByProcessID   (xPID)                    |
|    xmk_T_MESSAGE*   xmk_AllocSignal                (void)                    |
|    void             xmk_FreeSignal                 (xmk_T_MESSAGE)           |
|    xmk_T_BOOL       xmk_TestAndSetSaveState        (xmk_T_STATE)             |
|    xmk_T_BOOL       xmk_QueueEmpty                 (void)                    |
|    void             xmk_QueryQueue               (xmk_T_CMD_QUERY_QUEUE_CNF) |
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



#ifndef __MK_QUEU_C_
#define __MK_QUEU_C_


/*+IMPORT*/
/*====================  I M P O R T  =========================================*/
#include "ml_typw.h"

/*--------------------  Variables    -----------------------------------------*/

/*
**     Pointer to <root-process-table>
*/
#ifndef NO_GLOBAL_VARS
  /*    Modified by GBU,990518 */
  extern XCONST XPDTBL xmk_ROM_ptr xPDTBL[MAX_SDL_PROCESS_TYPES+1];

  #ifdef XMK_USE_MAX_ERR_CHECK
    extern unsigned char xmk_InitState;
  #endif
#endif

/*============================================================================*/
/*-IMPORT*/


/*+MGG*/
/*====================  V A L U E S  O F  T H I S   M O D U L E  =============*/

/*--------------------  Constants,  Macros  ----------------------------------*/

/*--------------------  Typedefinitions     ----------------------------------*/


#if defined(XMK_ADD_PROFILE) || defined(XMK_ADD_CQUERY_QUEUE)
/*
**   In  order to be able to test the number of used signals in the queue(s)
**   xmk_max_q_cnt is updated with every change of the number of signals.
**   The maximum counter value can be investigated whenever needed.
**   This, of course, is only useful when nearly all of the critical phases
**   have been running.
*/

#ifndef NO_GLOBAL_VARS
  /*
  **   counter for the current number of signals in the queue(s)
  */
  int xmk_act_q_cnt;

  /*
  **   counter for maximum number of signals in the queues
  */
  int xmk_max_q_cnt;
#endif /* NO_GLOBAL_VARS */

#define XMK_Q_INIT_MAX \
        xmk_act_q_cnt=0; \
        xmk_max_q_cnt=0;
#define XMK_Q_SET_MAX  if (xmk_act_q_cnt > xmk_max_q_cnt ) \
                           xmk_max_q_cnt = xmk_act_q_cnt;
#define XMK_Q_INCR_ACT xmk_act_q_cnt ++;
#define XMK_Q_DECR_ACT if (xmk_act_q_cnt) xmk_act_q_cnt --;

#else

#define XMK_Q_INIT_MAX
#define XMK_Q_SET_MAX
#define XMK_Q_INCR_ACT
#define XMK_Q_DECR_ACT
#endif /* ... XMK_ADD_PROFILE || XMK_ADD_CQUERY_QUEUE */

/*+PREEMPT*/
#ifdef XMK_USE_PREEMPTIVE

  /*
  **     Internal value (priority-level of signal's receiver)
  */
#define EPRIOLEVEL xPDTBL[EPIDTYPE(p_Message->rec)]->PrioLevel
#endif
/*-PREEMPT*/

/*--------------------  Functions    -----------------------------------------*/

/*--------------------  Variables    -----------------------------------------*/

/*
**   array which represents the available signals
*/
#ifndef NO_GLOBAL_VARS
  static T_E_SIGNAL xmk_SignalArrayVar [ XMK_MAX_SIGNALS ] ;

  /*
  **   pointer to list of free signals
  */
  static T_E_SIGNAL *xmk_FreeListPointer     ;

  /*
  **   pointer to list of queued signals
  **   when using a preemptive kernel, this pointer is
  **   updated with every context-switch
  */
  T_E_SIGNAL XMK_QUEUE_PTR ;

  /*
  **   pointer to the signal that is currently worked on
  **   when using a preemptive kernel, this pointer is
  **   updated with every context-switch
  */

  T_E_SIGNAL XMK_CURRENTSIGNAL_PTR ;

#endif /* NO_GLOBAL_VARS */
/*============================================================================*/
/*-MGG*/



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_InitQueue                                                |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function initializes the signal queue. It must be called before any    |
|  other Cmicro Kernel function, e.g.before xmk_InitSDL(). All relevant        |
|  pointers are initialized. All signal-elements are put into the free-list.   |
|  The SAVE-state of all signals is set to false.                              |
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
  void xmk_InitQueue( void )
#else
  void xmk_InitQueue()
#endif

/*-FDEF E*/

{
#if XMK_MAX_SIGNALS<255
  X_REGISTER unsigned char i ;
#else
  X_REGISTER int i ;
#endif

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_InitQueue");
  #endif

  #ifdef XMK_USE_MAX_ERR_CHECK
    xmk_InitState = 2; /* Queue is initialized */
  #endif

  /*
  **   Initialize Queue memory ...
  */
  (void)memset ((void xmk_RAM_ptr) xmk_SignalArrayVar , 0, sizeof (xmk_SignalArrayVar ));

  /*
  **   Initialize all the pointers
  */
  xmk_FreeListPointer          = (T_E_SIGNAL xmk_RAM_ptr) NULL ;

  #ifdef XMK_USE_PREEMPTIVE
    XMK_QUEUE_ADR         = (T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr) NULL ;
    XMK_CURRENTSIGNAL_ADR = (T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr) NULL ;
  #else
    XMK_QUEUE             = (T_E_SIGNAL xmk_RAM_ptr) NULL ;
    XMK_CURRENTSIGNAL     = (T_E_SIGNAL xmk_RAM_ptr) NULL ;
  #endif

  XMK_Q_INIT_MAX

  /*
  **   Insert all the signals in the free-list
  */
  for( i = 0; i < XMK_MAX_SIGNALS; i++ )
  {
    xmk_FreeSignal( ( xmk_T_MESSAGE xmk_RAM_ptr )&xmk_SignalArrayVar [ i ] ) ;
  } /* END FOR */

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_InitQueue");
  #endif

} /* END OF FUNCTION */



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_FirstSignal                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  The first signal in the current queue, which is the one with the highest    |
|  priority, is copied to the pointer to currently treated signal and          |
|  returned to the caller.                                                     |
|                                                                              |
|  The functions returns a pointer to the first signal in the queue or NULL,   |
|  if there are no signals in the queue.                                       |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : pointer to first signal in queue                             |
|                 (NULL, if no signals in queue)                               |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_MESSAGE xmk_RAM_ptr xmk_FirstSignal( void )
#else
  xmk_T_MESSAGE xmk_RAM_ptr xmk_FirstSignal( )
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_FirstSignal");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  XMK_CURRENTSIGNAL = XMK_QUEUE;

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_FirstSignal");
  #endif

  return( ( xmk_T_MESSAGE xmk_RAM_ptr ) XMK_CURRENTSIGNAL ) ;

} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_NextSignal                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  The signal following the current signal is copied to the current signal.    |
|  If there are no more signals, NULL is returned.                             |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : Pointer to the next signal in the queue                      |
|                 (NULL if no more signals in the queue)                       |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_MESSAGE xmk_RAM_ptr xmk_NextSignal( void )
#else
  xmk_T_MESSAGE xmk_RAM_ptr xmk_NextSignal( )
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_NextSignal");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  if( XMK_CURRENTSIGNAL != (T_E_SIGNAL xmk_RAM_ptr) NULL )
  {
    XMK_CURRENTSIGNAL = (XMK_CURRENTSIGNAL)->next ;
  } /* END IF */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_NextSignal");
  #endif

  return( ( xmk_T_MESSAGE xmk_RAM_ptr) XMK_CURRENTSIGNAL ) ;

} /* END OF FUNCTION */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_InsertSignal                                             |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function puts a signal into the queue.                                 |
|  The position depends on the signal-priority (if existing).                  |
|                                                                              |
|  With the parameter p_Message a pointer to the signal which is to be inserted|
|  is given.                                                                   |
|                                                                              |
|  Parameter    : *p_Message - pointer to signal which is to be inserted       |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_InsertSignal( xmk_T_MESSAGE xmk_RAM_ptr p_Message )
#else
  void xmk_InsertSignal ( p_Message )
  xmk_T_MESSAGE xmk_RAM_ptr p_Message ;
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr rover ;
  X_STATIC   T_E_SIGNAL xmk_RAM_ptr element ;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_InsertSignal");
  #endif

  element = ( T_E_SIGNAL xmk_RAM_ptr )p_Message ;

  /*
  **     search for signal's position in queue
  */
  element->next = (T_E_SIGNAL xmk_RAM_ptr) NULL ;

  #ifdef XMK_USE_SIGNAL_PRIORITIES

    #ifdef XMK_USE_PREEMPTIVE
      for( rover = &(Prio_Queue[EPRIOLEVEL]); *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL;
                                          rover = &(( *rover )->next ) )
    #else
      for( rover = XMK_QUEUE_ADR; *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL; rover = &(( *rover )->next ) )
    #endif
    {
      if( p_Message->prio < ( *rover )->Signal.prio  )
      {
        /*
        **     Position found, if signal with lower priority detected
        */
        break ;
      } /* END IF */
    } /* END FOR */

  /*
  **    signal-priority not used
  */
  #else

    /*
    **   In both cases inserting signal at the queue's end
    */
    #ifdef XMK_USE_PREEMPTIVE
      for( rover = &(Prio_Queue[EPRIOLEVEL]); *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL;
                                           rover = &(( *rover )->next ) );
    #else
      for( rover = XMK_QUEUE_ADR; *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL; rover = &(( *rover )->next ) );
    #endif

  #endif

  /*
  **       Insert signal into queue
  */
  element->next = *rover ;
  *rover        = element ;
  XMK_Q_INCR_ACT
  XMK_Q_SET_MAX

  /*+PREEMPT*/
  #ifdef XMK_USE_PREEMPTIVE
    xmk_CheckIfSchedule();
  #endif
  /*-PREEMPT*/

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_InsertSignal");
  #endif

} /* END OF FUNCTION */



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RemoveCurrentSignal                                      |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  The signal currently worked on is removed from the queue and is inserted    |
|  into the list of free signals.                                              |
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
  void xmk_RemoveCurrentSignal( void )
#else
  void xmk_RemoveCurrentSignal( )
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr rover ;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_RemoveCurrentSignal");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  /*
  **  If the first signal in the queue is the one to be removed
  */
  if( XMK_QUEUE == XMK_CURRENTSIGNAL )
  {
    /*
    **   move the pointer to the signal's successor
    */
    XMK_QUEUE = (XMK_QUEUE)->next ;
  } /* END IF */

  else
  {
    /*
    **   for all the other signals
    */
    for( rover = XMK_QUEUE; rover != (T_E_SIGNAL xmk_RAM_ptr) NULL; rover = rover->next )
    {
      /*
      **     if signal to be removed is found
      */
      if( rover->next == XMK_CURRENTSIGNAL )
      {
        rover->next = rover->next->next ;
        /*
        **   move the pointer to the signal following to the found one
        */
        break;
      } /* END IF */
    } /* END OF FUNCTION */
  } /* END ELSE */

  /*
  **  remove the signal from the queue and insert it in the free list
  */
  xmk_FreeSignal( ( xmk_T_MESSAGE xmk_RAM_ptr )XMK_CURRENTSIGNAL ) ;

  /*
  **    Set current signal to NULL
  */
  XMK_CURRENTSIGNAL = (T_E_SIGNAL xmk_RAM_ptr) NULL ;

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_RemoveCurrentSignal");
  #endif

} /* END OF FUNCTION */



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RemoveSignalBySignalID                                   |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  Signals with a given signal-code directed towards the current process       |
|  are removed from the queue and inserted in the list of free signals.        |
|                                                                              |
|  The signal currently worked on mustn't be removed, because it is            |
|  necessary for the current actions. It is anyhow removed after having        |
|  finished its treatment.                                                     |
|                                                                              |
|                                                                              |
|  Parameter    : SignalID - signal ID of the signals to be removed            |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_RemoveSignalBySignalID( xmk_T_SIGNAL SignalId )
#else
  void xmk_RemoveSignalBySignalID ( SignalId )
  xmk_T_SIGNAL SignalId ;
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr rover ;

  #ifndef XMK_USE_RECEIVER_PID_IN_SIGNAL
    xPID     OwnerProcessPID ;   /* temp.variable                        */
  #endif

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_RemoveSignalBySignalID");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  /*
  **    for all the signals in the queue
  */
  rover = XMK_QUEUE_ADR;
  while ( *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL)
  {
    /*
    **    does the current signal have the wanted signalID ?
    */
    #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
      if( ( *rover                    != XMK_CURRENTSIGNAL ) &&
        ( ( *rover )->Signal.rec      == xRunPID      ) &&
        ( ( *rover )->Signal.signal   == SignalId     ))
    #else
      OwnerProcessPID = xRouteSignal ( SignalId ) ;

      if( ( *rover                    != XMK_CURRENTSIGNAL ) &&
        ( OwnerProcessPID             == xRunPID      ) &&
        ( ( *rover )->Signal.signal   == SignalId     ))

    #endif
    {

      T_E_SIGNAL xmk_RAM_ptr f ;

      /*
      **   save pointer to that signal
      */
      f = *rover ;

      /*
      **   remove that signal from the queue
      */
      *rover = ( *rover )->next ;

      /*
      **   insert the removed signal into the free list
      */
      xmk_FreeSignal( ( xmk_T_MESSAGE xmk_RAM_ptr)f ) ;

    } 
    else
    {

      /*
      **  Signal pointer may have been affected already in if-loop above !!
      */
      rover = &( ( *rover )->next );
    } /* END IF */
  } /* END WHILE */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_RemoveSignalBySignalID");
  #endif

} /* END OF FUNCTION */

#if defined(XMK_USED_DYNAMIC_STOP) || defined(XMK_USE_CMD_KILL)

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_RemoveSignalsByProcessID                                 |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|  All signals addressed to a specific process are removed by calling this     |
|  Function.                                                                   |
|                                                                              |
|  Parameter    : ProcessId  - PID of Process                                  |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_RemoveSignalsByProcessID( xPID ProcessID )
#else
  void xmk_RemoveSignalsByProcessID( ProcessID )
  xPID           ProcessID;
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr rover;

  #ifndef XMK_USE_RECEIVER_PID_IN_SIGNAL
    xPID OwnerProcessPID ;
  #endif

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_RemoveSignalsByProcessID");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  #ifdef XMK_USE_PREEMPTIVE
    rover = &Prio_Queue[EPIDTYPE(ProcessID)];
  #else
    rover = &xmk_Queue;
  #endif

  /*
  **     for all the signals in the queue
  */
  while (*rover != (T_E_SIGNAL xmk_RAM_ptr) NULL)
  {
    #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
      if ( ( *rover               != XMK_CURRENTSIGNAL) &&
           ( (*rover)->Signal.rec == ProcessID) )
    #else
      OwnerProcessPID = xRouteSignal ((*rover)->Signal.signal) ;
      if ( ( *rover               != XMK_CURRENTSIGNAL) &&
           ( OwnerProcessPID == ProcessID ) )

    #endif
    {

      /*
      ** The element, which must be unchained, is found.
      */
      T_E_SIGNAL xmk_RAM_ptr f;

      f= *rover;
      *rover = (*rover)->next;
      xmk_FreeSignal(( xmk_T_MESSAGE xmk_RAM_ptr )f);
    }  
    else
    {
      /*
      ** load next element
      */
      rover = &( (*rover)->next );
    }  /* END IF */
  } /* END WHILE */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_RemoveSignalsByProcessID");
  #endif

} /* END OF FUNCTION */

#endif /* ... XMK_USED_DYNAMIC_STOP */


/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_AllocSignal                                              |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  An initialized signal from the beginning of the list of free signals        |
|  is returned, as far as there still remains one. If not NULL is returned.    |
|                                                                              |
|  A pointer to the signal that was allocated is returned usually, or NULL, if |
|  there is no space left to allocate one more signal.                         |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : Pointer to returned signal                                   |
|                 (NULL if there is none)                                      |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_MESSAGE xmk_RAM_ptr xmk_AllocSignal( void )
#else
  xmk_T_MESSAGE xmk_RAM_ptr xmk_AllocSignal( )
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr AllocatedMessage ;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_AllocSignal");
  #endif

  /*
  **  Return-value: first signal in free list
  */
  AllocatedMessage = xmk_FreeListPointer ;

  /*
  **  if that signal isn't NULL
  */
  if( AllocatedMessage != (T_E_SIGNAL xmk_RAM_ptr) NULL )
  {
    /*
    **    set pointer to first signal in free list to the next one
    */
    xmk_FreeListPointer = xmk_FreeListPointer->next ;

  } /* END IF */

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_AllocSignal");
  #endif

  return( (xmk_T_MESSAGE xmk_RAM_ptr)AllocatedMessage ) ;

} /* END OF FUNCTION */



/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_FreeSignal                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  A signal is initialized and inserted into the free-list at the first        |
|  position.                                                                   |
|                                                                              |
|  The parameter p_Message must point to the signal that is to be initialized. |
|                                                                              |
|  Parameter    : *p_Message - Pointer to signal to be initialized             |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_FreeSignal( xmk_T_MESSAGE xmk_RAM_ptr p_Message )
#else
  void xmk_FreeSignal( p_Message )
  xmk_T_MESSAGE xmk_RAM_ptr p_Message;
#endif

/*-FDEF E*/

{
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr element ;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_FreeSignal");
  #endif

  element = (T_E_SIGNAL xmk_RAM_ptr )p_Message ;

  /*
  **     if signal's parameter are in an allocated memory area
  */
  #ifdef XMK_USED_SIGNAL_WITH_PARAMS
    if (element->Signal.mess_length > XMK_MSG_BORDER_LEN)
    {
      /*
      **     free the allocated memory area
      */
      xFree (&element->Signal.ParUnion.ParPtr);
    }
  #endif

  #ifdef XMK_USED_SAVE
    /*
    **    Reset signal's SaveState 
    */
    element->SaveState = 0;
  #endif

  #ifndef XMK_USE_HIGH_OPTIMIZATION
    /*
    **    set all the information in the signal to zero (initialize signal)
    */
    (void)memset( (void xmk_RAM_ptr) element, 0, sizeof( T_E_SIGNAL) ) ;
  #endif

  /*
  **   Insert signal in front of the free-list and set
  **   pointer to free list to the new signal
  */
  element->next = xmk_FreeListPointer ;
  xmk_FreeListPointer = element ;
  XMK_Q_DECR_ACT

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_FreeSignal");
  #endif

} /* END OF FUNCTION */



#ifdef XMK_USED_SAVE

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_TestAndSetSaveState                                      |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function tests whether the current signal's SAVE-state is set or not.  |
|  In testing, the SAVE-state is set.                                          |
|                                                                              |
|  Parameter    : State - SAVE-state                                           |
|                                                                              |
|  Return       : XMK_TRUE  - the given SAVE-state equals the signal's         |
|                             SAVE state                                       |
|                 XMK_FALSE - the given SAVE-sate differs from the signal's    |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_BOOL xmk_TestAndSetSaveState( xmk_T_STATE State )
#else
  xmk_T_BOOL xmk_TestAndSetSaveState( State )
  xmk_T_STATE State ;
#endif

/*-FDEF E*/

{
  X_REGISTER xmk_T_BOOL match = XMK_FALSE ;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_TestAndSetSaveState");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  /*
  **   If there is a signal in the queue
  */
  if( XMK_CURRENTSIGNAL != (T_E_SIGNAL *) NULL )
  {

    /*
    **     Test if signal's save state equals parameter State
    */
    match = ( (XMK_CURRENTSIGNAL)->SaveState == State ) ;

    /*
    **     assign parameter State to signal's state
    */
    (XMK_CURRENTSIGNAL)->SaveState = State ;
  } /* END IF */

  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_TestAndSetSaveState");
  #endif

  return( match ) ;

} /* END OF FUNCTION */

#endif /* XMK_USED_SAVE */

#if defined ( XMK_USED_SAVE ) && defined ( XMK_USE_EXPANDED_KERNEL )

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_SaveSignalsOnly                                          |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function tests whether there only are SAVE signals contained in        |
|  the queue. This can be used in integrations with operating systems.         |
|                                                                              |
|  The function returns XMK_TRUE, if there are SAVE signals only in the        |
|  queue, otherwise it returns XMK_FALSE.                                      |
|                                                                              |
|  Parameter    : -                                                            |
|                                                                              |
|  Return       : XMK_TRUE  - SAVE-signals in the queue only                   |
|                                                                              |
|                 XMK_FALSE - at least one not SAVE signal is in the queue     |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
 xmk_T_BOOL xmk_SaveSignalsOnly ( void )
#else
 xmk_T_BOOL xmk_SaveSignalsOnly (  )
#endif

/*-FDEF E*/

{
  X_REGISTER xmk_T_BOOL match = XMK_TRUE ;
  X_REGISTER T_E_SIGNAL xmk_RAM_ptr xmk_RAM_ptr rover ;
  xmk_T_STATE State;
  
  #ifdef XMK_USE_PREEMPTIVE
    X_REGISTER xmk_OPT_INT level;
  #endif
    

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_SaveSignalsOnly");
  #endif

  XMK_BEGIN_CRITICAL_PATH;

  #ifdef XMK_USE_PREEMPTIVE
    
    /*
    **   For all the priority levels
    */
    for (level = 0; level < MAX_PRIO_LEVELS; level++)
    {
      match = XMK_TRUE;
      rover = &Prio_Queue[level];
      
  #else
    rover = &xmk_Queue;
  
    if ( *rover == (T_E_SIGNAL *) NULL )
    {
      match = XMK_FALSE;
    }

  #endif

  /*
  **   While there are signals in the queue
  **   and all of them are saved
  */
  while( ( *rover != (T_E_SIGNAL xmk_RAM_ptr) NULL ) && ( match ) )
  {
    /*
    **     Get signal receiver's state
    */
    #ifdef XMK_USE_RECEIVER_PID_IN_SIGNAL
      State = xmk_GetProcessState( (*rover)->Signal.rec);
    #else
      State = xmk_GetProcessState(xRouteSignal( (*rover)->Signal.signal ));
    #endif

    if ( State != XNOTEXISTENT)
    {
      /*
      **     Test if signal's save state equals receiver's State
      */
      match = ( (*rover)->SaveState == State ) ;
  
      /*
      **     Set signal pointer to next signal
      */
      rover = &((*rover)->next);

    }
    else
    {
      /*
      **    A signal not being saved is found
      **    Here, the environment is receiver of the signal or
      **    an invalid receiver ID is given.
      */
      match = XMK_FALSE;
    } /* END IF */
  } /* END WHILE */

  #ifdef XMK_USE_PREEMPTIVE
  
    if (! match)
      break;

    /*
    **   next priority level
    */
    }
  #endif
  
  XMK_END_CRITICAL_PATH;

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_SaveSignalsOnly");
  #endif

  return( match ) ;

} /* END OF FUNCTION */

#endif /* XMK_USED_SAVE && XMK_USE_EXPANDED_KERNEL */


#ifdef XMK_USE_SDL_SYSTEM_STOP

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_QueueEmpty                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|  Description  :                                                              |
|  This function tests whether there is a signal remaining in the queue(s)     |
|  or not.                                                                     |
|                                                                              |
|  The function returns XMK_TRUE, if there are no signals in the queue, but    |
|  returns XMK_FALSE if there is at least one signal in the queue. It does not |
|  matter if the signal is a saved signal or not. When the Cmicro Kernel is    |
|  configured for preemption, all the queues of the different priority levels  |
|  are checked.                                                                |
|                                                                              |
|  Parameter    : void                                                         |
|                                                                              |
|  Return       : XMK_TRUE  - There is no signal in any queue                  |
|                 XMK_FALSE - At least one signal is in any queue              |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  xmk_T_BOOL xmk_QueueEmpty ( void )
#else
  xmk_T_BOOL xmk_QueueEmpty ( )
#endif

/*-FDEF E*/

{

  #ifdef XMK_USE_PREEMPTIVE

    X_REGISTER xmk_OPT_INT i;
    #ifdef XMK_ADD_PRINTF_QUEUE
      XMK_FUNCTION("xmk_QueueEmpty");
    #endif

    /*
    **  if there are no more signals on any prio-level
    */
    for (i=0; i < MAX_PRIO_LEVELS; i++)
    {
      if (Prio_Queue[i] != (T_E_SIGNAL xmk_RAM_ptr) NULL)
      {
            #ifdef XMK_ADD_PRINTF_QUEUE
                  XMK_TRACE_EXIT("xmk_QueueEmpty");
                #endif

        return ( XMK_FALSE );
      }
    }

  #else

    #ifdef XMK_ADD_PRINTF_QUEUE
      XMK_FUNCTION("xmk_QueueEmpty");
    #endif
    /*
    **     If the queue doesn't equal NULL there is a signal
    */
    if (xmk_Queue != (T_E_SIGNAL xmk_RAM_ptr) NULL)
    {
          #ifdef XMK_ADD_PRINTF_QUEUE
           XMK_TRACE_EXIT("xmk_QueueEmpty");
          #endif

      return( XMK_FALSE );
    }

  #endif

  /*
  **    No signal in any queue
  */
  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_QueueEmpty");
  #endif

  return ( XMK_TRUE );

} /* END OF FUNCTION */


#endif /* XMK_USE_SDL_SYSTEM_STOP */


#if defined(XMK_USE_EXPANDED_KERNEL) || (defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CQUERY_QUEUE))

/*+FHDR E*/
/*
+------------------------------------------------------------------------------+
|  Functionname : xmk_QueryQueue                                               |
|  Author       : S&P Media GmbH Germany                                       |
+------------------------------------------------------------------------------+
|                                                                              |
|                                                                              |
|  Description  :                                                              |
|     The function evaluates the current SDL-Queue-State and returns some      |
|     information to the caller :                                              |
|     * information about the size of the Q (maximum amount of entries)        |
|     * information about traffic load, measured till now.                     |
|       Users can directly use this to scale the Q. This information           |
|       is valuable in the case, where the maximum traffic load is reached     |
|       one time during execution) .                                           |
|     * Information about : How many entries are currently in the Q ?          |
|     * A pointer to the physical Address of the Q.                            |
|                                                                              |
|  Parameter    : xmk_Q_STATE    *qinfo                                        |
|                                                                              |
|  Return       : -                                                            |
|                                                                              |
+------------------------------------------------------------------------------+
*/
/*-FHDR E*/

/*+FDEF E*/

#ifndef XNOPROTO
  void xmk_QueryQueue ( xmk_T_CMD_QUERY_QUEUE_CNF xmk_RAM_ptr qinfo )
#else
  void xmk_QueryQueue ( qinfo )
  xmk_T_CMD_QUERY_QUEUE_CNF xmk_RAM_ptr qinfo;
#endif

/*-FDEF E*/

{
  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_FUNCTION("xmk_QueryQueue");
  #endif


  qinfo->max_allowed_entries = XMK_MAX_SIGNALS ;

  #if defined(XMK_ADD_PROFILE) || defined(XMK_ADD_CQUERY_QUEUE)
    qinfo->max_counter         = xmk_max_q_cnt ;
    qinfo->howmany             = xmk_act_q_cnt ;
  #else
    qinfo->max_counter         = -1 ;
    qinfo->howmany             = -1 ;
  #endif

  #if defined(IARC51) || defined(KEIL_C51)
    qinfo->address             = (long)(int) xmk_SignalArrayVar  ;
  #else
    qinfo->address             = (long) xmk_SignalArrayVar  ;
  #endif 

  #ifdef XMK_ADD_PRINTF
    PRINTF (("Q-STATE:max_allowed_entries=%d\n", qinfo->max_allowed_entries));
    PRINTF (("Q-STATE:max_counter        =%d\n", qinfo->max_counter        ));
    PRINTF (("Q-STATE:howmany            =%d\n", qinfo->howmany            ));
    PRINTF (("Q-STATE:address (hex)      =%x\n", qinfo->address            ));
  #endif

  #ifdef XMK_ADD_PRINTF_QUEUE
    XMK_TRACE_EXIT("xmk_QueryQueue");
  #endif
}

/*
+------------------------------------------------------------------------------+
+---------------------------- End of function ---------------------------------+
+------------------------------------------------------------------------------+
*/

#endif /* ...defined(XMK_USE_EXPANDED_KERNEL) || (defined(XMK_USE_DEBUGGING) && defined(XMK_ADD_CQUERY_QUEUE)) */

#endif /* __ML_QUEU_C_ */
