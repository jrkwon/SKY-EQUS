
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       R E X   A R M   P O R T 

GENERAL DESCRIPTION
   This is the ARM Port of REX (Real Time Executive). 
   It is a simple, efficient real-time executive for the ARM7TDMI processors. 
   It supports multi-tasking according to a priority  scheduling algorithm, 
   and provides a small set of services for task suspension and reactivation.
   All REX services follow the APCS conventions for function call/return and 
   parameter passing.

 TASKS

   REX treats tasks as independently dispatchable entities, each having
   its own stack, signal flags and priority which collectively constitute
   the context of the task. Each task has an associated data structure,
   called the task context block (TCB), which REX uses for keeping track of
   the task's context. REX maintains a list of tasks in which all task TCBs
   are linked in order of decreasing task priority. The first TCB in the 
   task list belongs to the highest priority task, while the last TCB be-
   longs to the lowest priority task (i.e., the kernel task whose priority
   is zero).

   REX allows an arbitrary number of tasks to be created dynamically at
   any time during execution. Realistically, however, REX's performance
   deteriorates slightly with each added task, so care is called for in
   keeping the number of tasks to a minimum. Ultimately, the task load
   which REX can support depends on the processor chosen, the processor
   clock speed and the interrupt characteristics of the particular system.
    
 TASK STACKS

   As mentioned above, each task has its own stack which is used when-
   ever the task is running. When the task is suspended (for example, to
   run another task or to service an interrupt), the task's registers
   are pushed onto the task's stack, and then the top of stack pointer
   is saved in the task's TCB. Later, when the task is selected for
   running again, the top of stack pointer is restored from the task's TCB,
   the task's registers are popped off it's stack, and task execution 
   resumes where it left off. The process of task switching is virtually
   invisible to tasks.

 TASK SIGNALS

   Each task has associated with it 32 general purpose signal flags which
   are kept as part of the task's context in the task's TCB. These signals
   are used to notify the task that some kind of event has occurred. A
   task's signals may be set or cleared by any task, and by any interrupt
   handler.

   A task can poll the current state of its signals at any time. Moreover,
   a task can also suspend its own execution pending the setting of one or
   more of its signals. So, for example, Task A may suspend waiting for
   signal 1 to be set. At some later time, Task B  or an interrupt handler
   can wake up Task A by setting signal 1 for A. Once awakened, Task A is
   again eligible for execution, although it will actually run only if it
   is the highest priority task that is ready to run. 

   Notice that task signals are treated as a 32 bit wide mask. This means
   that all 32 signals for a task may be set or cleared in a single op-
   eration and, similarly, a task may suspend on up to 32 different signals
   at the same time, waking up when any one of them is set. 

    The important points to keep in mind about signals are:

     1. 32 signals are defined on a per task basis. For purposes of setting,
        clearing and waiting, signals are treated as 32 bit masks.

     2. Signals are general purpose flags whose meanings may be defined
        any way the user sees fit.

     3. A task may optionally suspend execution pending the setting of
        particular signal(s), presumably by another task or an interrupt
        handler. The emphasis here should be on the word 'optionally'
        since setting a signal for a task that is not suspended on the
        signal does not directly affect the execution state of that task.


 TASK PRIORITIES AND SCHEDULING

   Each task has associated with it a priority that is stored in it's TCB.
   Priorities range from 1 to 65535, with 1 being the lowest priority and
   65535 being the highest. REX reserves priority value zero for use with
   the kernel task.

   When scheduling, REX ALWAYS chooses the highest priority ready task,
   i.e., the highest priority task that is not suspended waiting on sig-
   nal(s). REX will continue to execute this task until it voluntarily
   suspends, or until an interrupt occurs which reactivates a higher pri-
   ority task by setting signals for the task. In the case where all user
   defined tasks suspend, the kernel task, defined by REX, will execute.

   REX also provides a mechanism by which a task can raise or lower its
   own priority dynamically.  No tasks are supposed to have same priority.
   If this happens, REX will have an unpredictable behavior.

===========================================================================

  INTERRUPTS

    When an interrupt occurs, the corresponding interrupt handler must
    immediately save the register context of the interrupted entity,
    which may be a task or a lower priority interrupt.



===========================================================================

  TIMERS

    Timers are used under REX to perform software interval timing. When
    using a timer, a task specifies the length of the interval that is to 
    be timed and a signal mask which is to be set when the timer expires. 
    The task may then poll to determine when the signal mask is set, or
    it may suspend on the signal mask. Notice that timers may be used in
    conjunction with other events that set signals, making it possible
    to perform timeout detection for those events.

    REX maintains a timer list which contains all active (i.e. unexpired)
    timers. When a timer is set, it is added to the list of active timers;
    upon expiration, it is pulled out of the list.

    Under REX, an arbitrary number of timers may be created although, as
    with tasks, performance deteriorates slightly with each active (i.e.,
    unexpired) timer. There is no overhead cost for timers that have
    expired.

===========================================================================

EXTERNALIZED FUNCTIONS

  rex_clr_sigs
     Clears a specified mask of signals for a specified task.

  rex_set_sigs
    Sets a specified mask of signals for a specified task. If the specified
    task is waiting on any of the signals set, it will be again be eligible
    for execution.

  rex_get_sigs
    Returns the state of the signals for a specified task.

  rex_def_timer
    Initializes a specified timer block for use as an interval timer. Sets
    up which task to notify when the timer expires, and which signal mask
    to set.

  rex_clr_timer
    Sets the counter value for a specified timer to zero. This, in effect,
    stops the timer.

  rex_set_timer
    Sets the counter value for a specified timer to a specified value and
    starts the count down. The signal mask specified by the timer is 
    cleared for the task associated with the timer, and later will be set
    when the timer expires.

  rex_get_timer
    Reads out the current counter value for a specified timer.

  rex_tick
    Decrements the counter values for all active timers by the specified
    argument value. As each timer expires (i.e., its count value goes to
    less than 1), the appropriate signal mask is set for the appropriate
    task. This procedure is generally called from a periodic interrupt.

  rex_wait
    Suspends the calling task until at least one signal of a specified
    signal mask is set for the task. If any signal of the signal mask is
    already set prior to calling rex_wait, an immediate return is made
    from the function and the calling task continues to execute.

  rex_timed_wait
    Sets the counter value of a specified timer to a specified value, and
    then suspends the calling task until at least one signal of a specified
    signal mask is set for the task. Typically, the timer, when it expires,
    will set one of the signals of the specified signal mask. Calling this
    function is equivalent to calling rex_set_timer followed by rex_wait.

  rex_def_task
    Creates a new task using a specified TCB, stack, priority, entry point
    and task parameter. Task will start running only if it is the highest
    priority ready task.

  rex_self
    Returns a pointer to the TCB of the calling task. This allows a task
    to obtain the address of its own TCB.

  rex_set_pri
    Changes the priority of the calling task to a specified priority.
  
  rex_task_pri
    Sets the priority of the task chosen to a determined value.

  rex_get_pri
    Returns the current priority of the calling task.

  rex_init
    Initializes REX and fires up a user startup task using a specified TCB,
    stack, priority, entry point and startup parameter.

  rex_set_interrupt_vector
    Given the id v of an installable vector and a pointer to the routine to
    be called when the interrupts occurs, this function installs the routine.

  rex_enable_interrupt
    Enables the interrupt associated with id v.
    
  rex_disable_interrupt
    Disables the interrupt associated with id v.
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  rex_init must be called to initialize the operating system.

Copyright (c) 1998,1999,2000 by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

 $Header:   L:/src/asw/MSM5000/VCS/rex.c_v   1.4   27 Jul 2000 10:48:14   tcarlson  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/08/00   rmd     Added MSM5000_IRAM_FWD #ifndefs around the functions
                   that will be copy into Internal RAM.
02/02/00   jct     Integrated preemptive interrupt changes
07/07/99   jct     Integrated Windows NT support
05/19/99    ms     Changes in rex_report_stack_overflow.
04/09/99    ms     Lint cleanup.
03/31/99    ms     Fixed rex_set_sigs to correctly compare priorities.
03/19/99    ms     Defined a new function rex_report_stack_overflow.
02/19/99    ms     Now running in Supervisor mode.
                   Changed interface of rex_init and rex_def_task. The stack
                   pointers in their parameter lists are void *.
                   Using a new type rex_stack_size_type for stack sizes.
02/12/99    ms     Added support for TIMETEST port.
02/05/99    sk     Reduced Kernel Task stack size to 40 words.
02/04/99    ms     Support for FEATURE_SI_STACK_WM.
12/02/98    ms     rex_task_list and rex_timer_list changed to headnodes.
11/30/98   jkl     Added new functions. Revised for code standards.
11/04/98   jkl     Revised for code standard (rex types)
09/10/98   jkl     Adapted for new context frame (Delta Rex)
05/27/98   jbh     Created module based on rex.asm 1.7
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "rex.h"

#ifdef T_WINNT
#error code not present
#else
  #include "armasm.h"
  #include "arm.h"
#endif

#ifdef CUST_H
  #include "customer.h"
#endif


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/*----------------------------------------------------------------------
** Kernel stack size
**--------------------------------------------------------------------*/
#if !defined FEATURE_SI_STACK_WM
#define REX_KERNEL_STACK_SIZE     40
#endif

/*----------------------------------------------------------------------
** The interrupt stack pointer (not static - the scheduler is not
**                              in this module)
**--------------------------------------------------------------------*/
rex_stack_word_type *  rex_int_stack;

/*-----------------------------------------------------------------------
** The current task pointer (not static - the scheduler is not
**                              in this module)
**---------------------------------------------------------------------*/
rex_tcb_type  * rex_curr_task;

/*-----------------------------------------------------------------------
** The highest priority ready task (not static - the scheduler is not
**                              in this module)
**---------------------------------------------------------------------*/
rex_tcb_type  * rex_best_task; 

/*----------------------------------------------------------------------
** The task_list head node.
**--------------------------------------------------------------------*/
static rex_tcb_type rex_task_list;  

/*----------------------------------------------------------------------
** Null timer which is the last member on the timer_list
**--------------------------------------------------------------------*/
static rex_timer_type rex_null_timer;  

/*----------------------------------------------------------------------
** The timer_list head node
**--------------------------------------------------------------------*/
static rex_timer_type rex_timer_list;  

/*----------------------------------------------------------------------
** The kernel task which is the last member on the task list
**--------------------------------------------------------------------*/
static rex_tcb_type rex_kernel_tcb;  
word                rex_kernel_stack[ REX_KERNEL_STACK_SIZE ];

/*----------------------------------------------------------------------
** The vectors that contain entrypoints to user defined ISR handlers
**--------------------------------------------------------------------*/
void   (* rex_irq_vector) (void);
void   (* rex_fiq_vector) (void);

/*---------------------------------------------------------------------
** External declaration
**--------------------------------------------------------------------*/
extern void *rex_set_cpsr( dword );

/*---------------------------------------------------------------------
** The interrupt nesting level - 0 means a task is being executed, 1
** means a single interrupt, 2 means a nested interrupt, etc.
**--------------------------------------------------------------------*/
word rex_int_nest_level;


/*---------------------------------------------------------------------
** A rex_tick counter for debugging 
**--------------------------------------------------------------------*/

#ifdef REX_DEBUG
#error code not present
#endif

/*---------------------------------------------------------------------
** Shadow of value last written to Timetest port.
**--------------------------------------------------------------------*/

#if defined TIMETEST
word rex_led_buf  =  0;
#endif


#ifndef MSM5000_IRAM_FWD
/*===========================================================================

FUNCTION REX_CLR_SIGS

DESCRIPTION
  Clears the p_sigs from the tcb passed in.

DEPENDENCIES
  p_tcb is a valid task control block.  

RETURN VALUE
  The previous signal mask.

SIDE EFFECTS
  The specified signals will be cleared for the specified task

===========================================================================*/
rex_sigs_type rex_clr_sigs
( 
  rex_tcb_type   *p_tcb,        /* tcb for which the signals will be cleared */
  rex_sigs_type   p_sigs        /* which signals to clear                    */
) 
{
  rex_sigs_type prev_sigs = 0;  /* holds the signals prior to the clear */
 
  /*-------------------------------------------------------
  ** Save current signals for the return value and then
  ** clear the signals listed in p_sigs
  **-----------------------------------------------------*/
  INTLOCK();

  p_sigs = ~p_sigs;

  prev_sigs  = p_tcb->sigs;
  p_tcb->sigs = p_tcb->sigs & p_sigs;

  INTFREE();

  return prev_sigs;
} /* END rex_clr_sigs */
#endif /*MSM5000_IRAM_FWD*/

#ifndef MSM5000_IRAM_FWD
/*===========================================================================

FUNCTION REX_SET_SIGS

DESCRIPTION
  Sets the signals for a task. If the task is waiting for one or more of the
  set signals and it is of higher priority then the current task, a task
  swap is induced.

DEPENDENCIES
   p_tcb must be a valid task control block.

RETURN VALUE
  The sigs prior to the set action.

SIDE EFFECTS
  Changes a tasks signals which may induce a task swap.

===========================================================================*/
rex_sigs_type rex_set_sigs
(
  rex_tcb_type   *p_tcb,          /* tcb for which the sigs will be set */
  rex_sigs_type   p_sigs          /* the sigs to set                    */
)
{
  rex_sigs_type prev_sigs = 0;    /* the sigs before setting */

  INTLOCK();

  prev_sigs = p_tcb->sigs;

  p_tcb->sigs = p_tcb->sigs | p_sigs;

  /*-------------------------------------------------------
  ** Check and see if the p_tcb task was waiting for the 
  ** sigs. If so, clear out the wait sigs and reschedule
  ** if the task is the new best task
  **-----------------------------------------------------*/

  if((p_tcb->wait & p_sigs) != 0)
  {
    p_tcb->wait = 0;
   
    if(p_tcb->pri > rex_best_task->pri)
    {
      rex_best_task = p_tcb;
      rex_sched();
    }
  }

  INTFREE();

  return prev_sigs;  
} /* END rex_set_sigs */
#endif /*MSM5000_IRAM_FWD*/

#ifndef MSM5000_IRAM_FWD
/*===========================================================================

FUNCTION REX_GET_SIGS

DESCRIPTION
  Returns the current signatures for a task.

DEPENDENCIES
  p_tcb is a valid task control block

RETURN VALUE
  The current sigs for a task.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type rex_get_sigs
(
  rex_tcb_type *p_tcb           /* tcb for which sigs will be returned */
)
{
  return p_tcb->sigs;
} /* END rex_get_sigs */
#endif /*MSM5000_IRAM_FWD*/

/*===========================================================================

FUNCTION REX_DEF_TIMER

DESCRIPTION
  Initializes a new timer.

DEPENDENCIES
  p_timer must be a valid timer
  p_tcb must be a valid task control block

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rex_def_timer
(
  rex_timer_type  *p_timer,     /* pointer to a valid timer structure */
  rex_tcb_type    *p_tcb,       /* tcb to associate with the timer    */ 
  rex_sigs_type    p_sigs       /* sigs to set upon timer expiration  */
)
{  

  p_timer->link.next_ptr = NULL;
  p_timer->link.prev_ptr = NULL;
  p_timer->cnt           = 0;
  p_timer->tcb_ptr       = p_tcb;
  p_timer->sigs          = p_sigs;
  
  return;
} /* END rex_def_timer */

#ifndef MSM5000_IRAM_FWD
/*===========================================================================

FUNCTION REX_CLR_TIMER

DESCRIPTION
  Clears a timer and if it is in the current timer list, it is removed.

DEPENDENCIES
  p_timer must be a valid timer.

RETURN VALUE
  The count before the timer is cleared.  

SIDE EFFECTS
  None

===========================================================================*/
rex_timer_cnt_type rex_clr_timer
( 
  rex_timer_type *p_timer       /* timer to clear */
)
{
  rex_timer_cnt_type  prev_value = 0;    /* previous count */
  
  INTLOCK();
 
  prev_value = p_timer->cnt;
  p_timer->cnt = 0;
  
  /*-------------------------------------------------------
  ** If it was in the active timers list, remove it. 
  **-----------------------------------------------------*/
  if(prev_value != 0)
  {
    p_timer->link.prev_ptr->link.next_ptr = p_timer->link.next_ptr;
    p_timer->link.next_ptr->link.prev_ptr = p_timer->link.prev_ptr;
  }

  INTFREE();

  return prev_value;
} /* END rex_clr_timer */
#endif /*MSM5000_IRAM_FWD*/
 
/*===========================================================================

FUNCTION REX_SET_TIMER

DESCRIPTION
  Sets a timer for expiration after a specific amount of time (in milliseconds). 
  If the timer is already in the active timer list, the old timer values are 
  overwritten by the new timer values. If a p_cnt of 0 is passed in, the 
  signals for the task are immediatly set.

DEPENDENCIES
  p_timer must point to an initialized timer.  

RETURN VALUE
  The timers value before setting it to the new value.

SIDE EFFECTS
  Could cause a call to set sigs and thus induce a task swap.

===========================================================================*/
rex_timer_cnt_type rex_set_timer
(
  rex_timer_type *p_timer,      /* pointer to timer to set */
  rex_timer_cnt_type  p_cnt     /* value to set the timer   */
)
{
  rex_timer_cnt_type prev_value = 0; /* the value before the set */
 
  INTLOCK();
 
  /*-------------------------------------------------------
  ** We need to stick the timer on the front of the timer
  ** list if it is not in the list already
  **-----------------------------------------------------*/
  if(p_timer->cnt == 0)
  {
    p_timer->link.prev_ptr = &rex_timer_list;
    p_timer->link.next_ptr = rex_timer_list.link.next_ptr;
    rex_timer_list.link.next_ptr->link.prev_ptr = p_timer;
    rex_timer_list.link.next_ptr = p_timer;
  }

  /*-------------------------------------------------------
  ** Save the old timer count for returning 
  **-----------------------------------------------------*/
  prev_value = p_timer->cnt;
  p_timer->cnt = p_cnt;

  /*-------------------------------------------------------
  ** If the user passed in 0, pull from the timer list
  ** and set the signals for the task
  **-----------------------------------------------------*/
  if(p_cnt == 0)
  { 
    p_timer->link.prev_ptr->link.next_ptr = p_timer->link.next_ptr;
    p_timer->link.next_ptr->link.prev_ptr = p_timer->link.prev_ptr;
  
    (void) rex_set_sigs( p_timer->tcb_ptr , p_timer->sigs);  
  }
  else
  {
    (void) rex_clr_sigs( p_timer->tcb_ptr, p_timer->sigs);
  }
 
  INTFREE();
 
  return prev_value;
} /* END rex_set_timer */

/*===========================================================================

FUNCTION REX_GET_TIMER
 
DESCRIPTION
  Returns the current remaining count of a timer.
 
DEPENDENCIES
  p_timer must be valid timer
  
RETURN VALUE
  The count remaining in the timer.
  
SIDE EFFECTS
  None.  

===========================================================================*/
rex_timer_cnt_type rex_get_timer
(
  rex_timer_type *p_timer       /* pointer to the timer to get */
)
{  
  return p_timer->cnt;
} /* END rex_get_timer */

/*===========================================================================

FUNCTION REX_TICK

DESCRIPTION
  Decrements the timer value for all valid timers in the active timer list  

DEPENDENCIES
  Interrupts must be disabled.

RETURN VALUE
  None.  

SIDE EFFECTS
  If a timer expires , rex_set_sigs is called and a task swap may be induced  

===========================================================================*/
void rex_tick
(  
  rex_timer_cnt_type p_ticks    /* number of rex ticks to decrement everything by */
)
{
  rex_timer_type *ptr;        /* temp pointer */

#ifdef REX_DEBUG
#error code not present
#endif

  INTLOCK();

  /*-------------------------------------------------------
  ** Cycle through the timer list decrementing p_ticks from
  ** each timer. If the timer expired, remove it from the 
  ** timer list and set the sigs for the timer's tcb.
  **-----------------------------------------------------*/
  for (ptr = rex_timer_list.link.next_ptr; ptr != &rex_null_timer ; ptr = ptr->link.next_ptr)
  {
    if( ptr->cnt <= p_ticks )
    {
      ptr->cnt = 0;

      ptr->link.prev_ptr->link.next_ptr = ptr->link.next_ptr;
      ptr->link.next_ptr->link.prev_ptr = ptr->link.prev_ptr;

      /*---------------------------------------------------
      ** set the sigs for this task. This may cause a 
      ** reschedule if the task whose timer just expired is
      ** the new best task
      **--------------------------------------------------*/
      (void) rex_set_sigs(ptr->tcb_ptr, ptr->sigs);      
    }
    else
    {
      /*--------------------------------------------------- 
      ** Subtract the number of rex_ticks this call is 
      ** servicing from the timer
      **-------------------------------------------------*/ 
      ptr->cnt -= p_ticks;
    }
  }

  INTFREE();

  return;
} /* END rex_tick */

/*===========================================================================

FUNCTION REX_WAIT

DESCRIPTION
  Suspends a task until the one or more of the signals in the mask are set for
  the current task.

DEPENDENCIES
  None.

RETURN VALUE
  The signals that were set for the task.

SIDE EFFECTS
  Will cause a task swap if the signals for the task are not already set at
  the time of the call.

===========================================================================*/
rex_sigs_type rex_wait
(
  rex_sigs_type p_sigs          /* sigs to wait on */
)
{
  rex_sigs_type sigs = 0;

  INTLOCK( );
  /*------------------------------------------------------- 
  ** if no signals are set,  wait, and reschedule
  **-----------------------------------------------------*/
  if( (rex_curr_task->sigs & p_sigs) == 0 )
  {  
     /*-------------------------------------------------------
     ** Set the sigs the task should wait upon
     **-----------------------------------------------------*/ 
     rex_curr_task->wait = p_sigs;
     /*-------------------------------------------------------
     ** Find the next best task and schedule it. Task list is 
     ** sorted in priority order (highest to lowest)
     **-----------------------------------------------------*/
     rex_best_task = rex_task_list.link.next_ptr;

     while( rex_best_task->wait != 0 )
     {
        rex_best_task = rex_best_task->link.next_ptr;    
     }    
     rex_sched();
  }
  /*-------------------------------------------------------
  ** return with the signal mask of the current task 
  **-----------------------------------------------------*/
  sigs = rex_curr_task->sigs;
  INTFREE( );
  return sigs;

} /* END rex_wait */

/*===========================================================================

FUNCTION REX_TIMED_WAIT

DESCRIPTION
  Suspends a task and sets a timer. When either the timer or one of the 
  signals is set and the task is the highest ready task, the task resumes.

DEPENDENCIES
  p_timer must point to an initialized timer.  

RETURN VALUE
  The signal mask for the task.

SIDE EFFECTS
  Causes a task swap if the signals are not already set for the calling task

===========================================================================*/
rex_timer_cnt_type rex_timed_wait
(
  rex_sigs_type   p_sigs,       /* sigs to wait on          */
  rex_timer_type *p_timer,      /* timer to set and wait on */
  rex_timer_cnt_type  p_cnt     /* timer to wait            */
)
{
  /*------------------------------------------------------- 
  ** A call to rex_set_timer and rex_wait will perform a 
  ** timed wait
  **-----------------------------------------------------*/
  (void) rex_set_timer(p_timer, p_cnt);
  
  return rex_wait(p_sigs);
} /* END rex_timed_wait */


/*===========================================================================

FUNCTION REX_IDLE_TASK

DESCRIPTION
  The lowest possible task priority (0) and runs only when no other
  task is ready to run.

DEPENDENCIES
  rex_def_task should spawn this task

RETURN VALUE
  None

SIDE EFFECTS
  this task never suspends, waits or returns.
  
===========================================================================*/ 
/*lint -e715 arg not referenced */
/*lint -e527 Unreachable */

void rex_idle_task( 
   dword arg 
)
{
#ifdef T_WINNT
#error code not present
#else
   for(;;){}
#endif
   return;
}

/*lint +e527 Unreachable */
/*lint +e715 arg not referenced */


/*===========================================================================

FUNCTION REX_TASK_PREAMBLE

DESCRIPTION
  Special Note: rex_task_preamble is an internal function that should be called
  only from rex service functions. It should not be called directly by user
  software.
  rex_task_preamble is a helper function which is the first thing to execute in
  every newly created task. Upon receiving control, rex_task_preamble merely
  calls the user-defined entry point for the new task, passing it the user-
  specified parameter for the new task.

DEPENDENCIES
  assumes that function rex_def_task has set up the correct
  context frame on the stack for the new task.
RETURN VALUE
  None. This function does not return.********
SIDE EFFECTS
  None
===========================================================================*/ 
static void rex_task_preamble( 
   void (*func_ptr)( dword arg ),
   dword arg
)
{
   func_ptr( arg );
   for (;;){}               /* Tasks should never return. If it returns, this
                              infinite loop will trigger the watchdog. */
}

/*===========================================================================

FUNCTION REX_DEF_TASK

DESCRIPTION
  Define a new rex task. 

DEPENDENCIES
  p_tcb must be a valid tcb block.
  p_task must be a valid function pointer

RETURN VALUE
  None.

SIDE EFFECTS
  Defines a new rex task that will be placed in the task list.
  Creates a new thread and an event for the new task.
  The task starts running only if it is the highest priority ready task.

===========================================================================*/
void rex_def_task( 
  rex_tcb_type         *p_tcb,             /* valid tcb for new task       */
  void *               p_stack,            /* stack for new task           */
  rex_stack_size_type  p_stksiz,           /* stack size for new task      */
  rex_priority_type    p_pri,              /* priority for new task        */
  void                 (*p_task)( dword ), /* task startup function        */
  dword                p_param             /* parameter for new task       */
)
{
  byte                   *stack_ptr = NULL;  
  rex_tcb_type           *tcb_ptr   = NULL;    
  rex_context_frame_type *cf_ptr    = NULL;

  /*-------------------------------------------------------
  ** Task stack pointer points to the bottom of allocated 
  ** stack memory. p_stksiz is the number of 16-bit words.
  **-----------------------------------------------------*/
  stack_ptr = (byte *)((rex_stack_word_type *)p_stack + (p_stksiz*sizeof(word)/sizeof(rex_stack_word_type)) - 1);

  /*-------------------------------------------------------
  ** Creates room for the context. 
  ** sp points to the top of the context frame.  
  **-----------------------------------------------------*/
  stack_ptr        -= sizeof( rex_context_frame_type );

  #ifdef FEATURE_SI_STACK_WM
#error code not present
  #endif /* FEATURE_SI_STACK_WM */

  /*-------------------------------------------------------
  ** Defines the initial context. 
  **-----------------------------------------------------*/
#ifdef T_WINNT
#error code not present
#else
  cf_ptr              = (rex_context_frame_type*)stack_ptr;
  cf_ptr->spsr.val    = PSR_Supervisor | PSR_Thumb;
  cf_ptr->r[0].task   = p_task;
  cf_ptr->r[1].arg    = p_param;
  cf_ptr->lr.preamble = rex_task_preamble;
  cf_ptr->pc.preamble = rex_task_preamble;
#endif

  /* ------------------------------------------------------
  ** Initialize the task control block (TCB)
  ** ------------------------------------------------------ */
  p_tcb->sp          = stack_ptr;
  p_tcb->stack_limit = p_stack;
  p_tcb->slices      = 0;
  p_tcb->sigs        = 0;
  p_tcb->wait        = 0;
  p_tcb->pri         = p_pri;

  /*-------------------------------------------------------
  ** We are done if this is the idle task (kernel task) defined. 
  **-------------------------------------------------------*/
  if (p_pri == 0) {
#ifdef T_WINNT
#error code not present
#endif
     return;
  }

  /*-------------------------------------------------------
  ** Find the first task whose priority is lower than the  
  ** new task and insert the new task in front of the
  ** lower priority task.
  ** rex_idle_task (the kernel task) is at the end of the list
  ** with priority 0 and therefore no need to check for null ptrs.
  **-----------------------------------------------------*/
  INTLOCK();
  tcb_ptr = rex_task_list.link.next_ptr;
  while (tcb_ptr->pri > p_pri) {
     tcb_ptr =  tcb_ptr->link.next_ptr;
  }

  p_tcb->link.prev_ptr = tcb_ptr->link.prev_ptr;
  p_tcb->link.next_ptr = tcb_ptr;
  tcb_ptr->link.prev_ptr->link.next_ptr = p_tcb;
  tcb_ptr->link.prev_ptr = p_tcb;

  /*---------------------------------------------------
  ** Make this task the best task if it is higher 
  ** priority than the present best task. 
  **---------------------------------------------------*/
  if (p_pri > rex_best_task->pri) {
     rex_best_task = p_tcb;
  }
  INTFREE();

  /* ------------------------------------------------------
  ** Schedule the task  
  ** ------------------------------------------------------ */
  rex_sched();

  return;
} /* END rex_def_task */

/*===========================================================================

FUNCTION REX_SELF

DESCRIPTION
  Returns the task control block for the current task

DEPENDENCIES
  None.

RETURN VALUE
  The task control block for the current task

SIDE EFFECTS
  None.

===========================================================================*/
rex_tcb_type *rex_self( void )
{
  /*-------------------------------------------------------
  ** The currently running task is in rex_curr_task
  **-----------------------------------------------------*/
  return rex_curr_task;
} /* END rex_tcb_type */

/*===========================================================================

FUNCTION REX_SET_PRI

DESCRIPTION
  Sets the priority of the calling task. 

DEPENDENCIES
  None. 

RETURN VALUE
  The priority before it was changed.

SIDE EFFECTS
  The priority change will cause a change in the whole scheduling order for 
  all of the tasks. This could result in an imediate task swap.

===========================================================================*/
rex_priority_type rex_set_pri
(
  rex_priority_type p_pri       /* the new priority */
)
{
  /*-------------------------------------------------------
  ** A wrapper function that just calls rex_task_pri with
  ** the current task
  **-----------------------------------------------------*/
  return rex_task_pri(rex_curr_task, p_pri);
} /* END rex_set_pri */

/*===========================================================================

FUNCTION REX_TASK_PRI

DESCRIPTION
  Sets the priority of the passed in task.

DEPENDENCIES
  p_tcb must be a valid task control block

RETURN VALUE
  The priority before it was changed.

SIDE EFFECTS
  The priority change will cause a change in the whole scheduling order for 
  all of the tasks. This could result in an imediate task swap.

===========================================================================*/
rex_priority_type rex_task_pri
(
  rex_tcb_type       *p_tcb,    /* tcb to set priority on      */
  rex_priority_type  p_pri      /* the new priority            */
)
{
  rex_priority_type prev_pri   = p_tcb->pri;   /* the priority before the set */
  rex_tcb_type  *search_ptr;                   /* temp pointer                */
  boolean comp = FALSE;                        /* Comparator */

  INTLOCK();
  comp = (p_pri == p_tcb->pri);
  INTFREE();

  /* just return if the priority is the same */
  if( comp )
  {
    return prev_pri;
  }
  
  /*-------------------------------------------------------
  ** Remove from the list and re insert in a new position
  ** based upon the new priority
  **-----------------------------------------------------*/
  INTLOCK();
  p_tcb->link.next_ptr->link.prev_ptr = p_tcb->link.prev_ptr;
  p_tcb->link.prev_ptr->link.next_ptr = p_tcb->link.next_ptr;

  p_tcb->pri = p_pri;     /* Assign new priority.  */

  /*-------------------------------------------------------
  ** Find the task with the priority <= the new priority. 
  ** Insert in front of this task
  ** rex_idle_task (the kernel task) is at the end of the list
  ** with priority 0 and therefore no need to check for null ptrs.
  **-----------------------------------------------------*/
  search_ptr = rex_task_list.link.next_ptr;
  while(search_ptr->pri > p_pri) {
    search_ptr = search_ptr->link.next_ptr;
  }  

  p_tcb->link.prev_ptr = search_ptr->link.prev_ptr;
  p_tcb->link.next_ptr = search_ptr;
  search_ptr->link.prev_ptr->link.next_ptr = p_tcb;
  search_ptr->link.prev_ptr = p_tcb;

  /*-------------------------------------------------------
  ** Find the highest priority waiting task
  ** rex_idle_task (the kernel task) is at the end of the list
  ** with priority 0 and always ready.
  **-----------------------------------------------------*/
  search_ptr = rex_task_list.link.next_ptr;
  while(search_ptr->wait != 0)
  {
    search_ptr = search_ptr->link.next_ptr;
  }

  /*------------------------------------------------------
  ** if the new best task is not the current best task
  ** reschedule
  **----------------------------------------------------*/
  if(rex_best_task != search_ptr)
  {
    rex_best_task = search_ptr;
    rex_sched();
  }
  
  INTFREE();
  return prev_pri;

} /* END rex_task_pri */

/*===========================================================================

FUNCTION REX_GET_PRI

DESCRIPTION
  Return the priority of the current task.  

DEPENDENCIES
  None.

RETURN VALUE
  The priority of the current task.

SIDE EFFECTS
  None.  

===========================================================================*/
rex_priority_type rex_get_pri( void )
{
  /*-------------------------------------------------------
  ** Just return the priority field of the current task
  **-----------------------------------------------------*/
  return rex_curr_task->pri;
} /* END rex_get_pri */


/*===========================================================================

FUNCTION REX_INIT

DESCRIPTION
  Initializes REX. It initializes the kernel task and calls the p_task main
  rex function.

DEPENDENCIES
  p_tcb must be a valid task control block.
  p_task must be a valid function pointer.  

RETURN VALUE
  None.  

SIDE EFFECTS
  When the initialization is done, this thread will wait until
  the exit event is triggered at which point it will clean up 
  and return.
  
===========================================================================*/
void rex_init
(
  void *              p_istack,           /* interrupt stack      */
  rex_stack_size_type p_istksiz,          /* interrupt stack size */
  rex_tcb_type        *p_tcb,             /* task control block   */ 
  void *              p_stack,            /* stack                */
  rex_stack_size_type p_stksiz,           /* stack size           */
  rex_priority_type   p_pri,              /* task priority        */ 
  void                (*p_task)( dword ), /* task function        */
  dword               p_param             /* task parameter       */
)
{
  /*-------------------------------------------------------
  ** Change to Supervisor mode
  **-----------------------------------------------------*/
#ifndef T_WINNT
  (void) rex_set_cpsr( PSR_Supervisor | PSR_Irq_Mask | PSR_Fiq_Mask);
#endif

  /*-------------------------------------------------------
  ** Setup the interrupt stack. 
  **-----------------------------------------------------*/

  INTLOCK();
  /* Point to the top of the stack */
  
  rex_int_stack = (rex_stack_word_type *) p_istack;
  
  /*-------------------------------------------------------
  ** Initialize the interrupt nest level at zero
  **------------------------------------------------------*/
  rex_int_nest_level  = 0;

  /*-------------------------------------------------------
  ** Initialize the timer list with the null timer
  **-----------------------------------------------------*/
  rex_null_timer.cnt               = 0;
  rex_null_timer.tcb_ptr           = NULL;
  rex_null_timer.sigs              = 0x0;

  rex_null_timer.link.next_ptr     = NULL;
  rex_null_timer.link.prev_ptr     = &rex_timer_list;
  rex_timer_list.link.next_ptr     = &rex_null_timer;
  rex_timer_list.link.prev_ptr     = NULL;
  
  /*-------------------------------------------------------
  ** Initialize the task list. 
  **-----------------------------------------------------*/
  rex_task_list.link.next_ptr = &rex_kernel_tcb;
  rex_task_list.link.prev_ptr = NULL;
  rex_kernel_tcb.link.next_ptr = NULL;
  rex_kernel_tcb.link.prev_ptr = &rex_task_list;

  /*-------------------------------------------------------
  ** rex_curr_task must be equal to rex_best_task to avoid
  ** scheduling at this moment
  **-----------------------------------------------------*/
  rex_curr_task = &rex_kernel_tcb;  
  rex_best_task = &rex_kernel_tcb;  

  rex_def_task(
    &rex_kernel_tcb,                      /* tcb        */
    (void * ) &rex_kernel_stack[0],       /* stack      */
    REX_KERNEL_STACK_SIZE,                /* stack size */
    0,                                    /* priority   */
    rex_idle_task,                        /* function   */
    0                                     /* arguments  */
  );

  /*-------------------------------------------------------
  ** rex_curr_task must be equal to rex_best_task to avoid
  ** scheduling at this moment
  **-----------------------------------------------------*/
  rex_curr_task = p_tcb;
  rex_best_task = p_tcb;  
  
  /*-------------------------------------------------------
  ** Define the user first task for the main control task
  **------------------------------------------------------*/
  rex_def_task(
    p_tcb,                                   /* tcb        */
    p_stack,                                 /* stack      */
    p_stksiz,                                /* stack size */
    p_pri,                                   /* priority   */
    p_task,                                  /* function   */
    p_param                                  /* arguments  */
  );
  
  INTFREE( );
  
  rex_start_task( p_tcb );     /* Start restoring task context */

} /* END rex_init */

#ifndef T_WINNT
/*===========================================================================

FUNCTION REX_SET_INTERRUPT_VECTOR

DESCRIPTION
  Given the id v of an installable vector and a pointer to the routine
  to be called when the interrupts occurs, this function installs the routine

DEPENDENCIES
  None.
  
RETURN VALUE
  None.  

SIDE EFFECTS
  None.
  
===========================================================================*/
void rex_set_interrupt_vector 
(
   rex_vect_type v,                /* Vector                    */
   void (*fnc_ptr)( void )         /* *function to be installed */
)
{
   if (v == P_IRQ_VECT) 
   {
     rex_irq_vector =  fnc_ptr;
   }
   else
   {
     rex_fiq_vector =  fnc_ptr; 
   }
} /* END rex_set_interrupt_vector */
#endif /* END T_WINNT */



#if defined FEATURE_STACK_CHECK
#include "err.h"

/*===========================================================================

FUNCTION rex_report_stack_overflow
                 
DESCRIPTION
  This function is called by the stack overflow handlers.
  Those handlers are in assembly, whereas ERR_FATAL is available only in C.

  The ERR_FATAL message prints the stack pointer, link register, processor 
  mode (LSW) and current task pointer (LSW).

DEPENDENCIES
  None.
  
RETURN VALUE
  None. The application will stop here.

SIDE EFFECTS
  None.

===========================================================================*/

void 
rex_report_stack_overflow(dword sp, dword lr, dword mode)
{  
   ERR_FATAL( "STACK OVERFLOW. %lx | %lx | %lx.", sp, lr, ((mode << 16)|((dword)rex_curr_task & 0xffff)) );
}

#endif /* FEATURE_STACK_CHECK */

