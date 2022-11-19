#ifndef REX_H
#define REX_H
/*==========================================================================

                      R E X    H E A D E R    F I L E

DESCRIPTION
   API for the REX Real Time Executive


Copyright (c) 1990-1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rex.h_v   1.2   18 Aug 2000 14:32:02   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/00   bgc     Added r8-r11 to the task context for task switches.
08/24/99   jct     Integrated Windows NT support
08/23/99   jeb     Removed cntrl-L white space character
08/16/99   jct     Added TASKLOCK/FREE
06/16/99   jct     Added inclusion of rexnt.h for T_WINNT targets
02/19/99    ms     Modified context frame to exploit running in Supervisor mode.
                   Changed interface of rex_init and rex_def_task. The stack
                   pointers in their parameter lists are void *.
                   Defined a new type rex_stack_size_type for stack sizes.
                   Added prototype for DBGBreak.
02/12/99    ms     Added support for TIMETEST port.
02/05/99    sk     Reduced kernel stack size to 40 words.
02/04/99    ms     Support for FEATURE_SI_STACK_WM.
12/16/98    ms     Removed 80186 support.
12/10/98   jkl     Include r12 in the context frame.
11/03/98   jct     Compressed some typedefs  
10/26/98   jkl     Merged T_ARM with 80186 code, cleaned up file    
09/15/98   udp     Replaced _ARM_ --> T_ARM
09/15/98   jct     Removed inclusion of types.h (was commented out anyway)
07/10/98   jct     Revised for coding standard, removed unused code
01/01/98   bwj     Created
02/01/96   rdh     Added rex_task_pri for time-critical task response speedup.
03/16/93   jah     Added extern of rex_int_level so that tasks could see the
                   current level of interrupt nesting.
02/23/93   jah/gb  Added TIMETEST code for task performance analysis using
                   an 8-bit buffered output.
04/23/92   ip      Changed signals type to rex_sigs_type (still one word).
04/22/92   ip      Initial porting of file from Brassboard to DMSS.

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"         

#ifdef CUST_H
   #include "customer.h"         
#endif

#ifdef TG
   #include "target.h"
#endif


/*===========================================================================

                           DATA DECLARATIONS

===========================================================================*/

/* ----------------------------------------------------------------------
** REX Data Types
** - These are sized based on the target
**   - Stack word, size of a word declared for stack variables
**   - Priority word, holds REX task priorities
**   - Time Count word, holds timer count downs
**   - Signal word, holds signals for a task
** ---------------------------------------------------------------------- */
typedef dword   rex_stack_word_type;
typedef dword   rex_stack_size_type;
typedef dword   rex_priority_type;
typedef dword   rex_timer_cnt_type;
typedef dword   rex_sigs_type;
typedef dword   rex_vect_type;

/* ----------------------------------------------------------------------
** Time test variable
** ---------------------------------------------------------------------- */
#ifdef TIMETEST
   extern word rex_led_buf;
#endif

/* ----------------------------------------------------------------------
** REX Task Control Block
** - Describes a REX task
** - Fields should not be accessed directly
** - Order of fields should not be altered as there are dependencies in
**   the kernel on their ordering
** ---------------------------------------------------------------------- */
typedef struct rex_tcb_struct {
   void               *sp;          /* Stack pointer   */
   void               *stack_limit; /* Stack Limit     */
   unsigned long      slices;       /* Task slices     */
   rex_sigs_type      sigs;         /* Current signals */
   rex_sigs_type      wait;         /* Wait signals    */
   rex_priority_type  pri;          /* Task's priority */
#if defined TIMETEST
   word               leds;         /* TIMETEST val    */
#endif
   struct
   {
      struct rex_tcb_struct *next_ptr;
      struct rex_tcb_struct *prev_ptr;
   } link;
#if defined FEATURE_REX_EXTENDED_CONTEXT
   void               *ext;
#endif
} rex_tcb_type;

/* ------------------------------------------------------------------------
** CPU Registers
** ------------------------------------------------------------------------ */
typedef PACKED union {
   unsigned long arg;
   unsigned long val;
   void          *addr;
   void         (*func)( void );
   void         (*task)( unsigned long );
   void         (*preamble)( void (*func)(unsigned long), unsigned long );
} rex_cpu_register_type;


/* ------------------------------------------------------------------------
** Context Frame
** ------------------------------------------------------------------------ */
typedef PACKED struct {
   rex_cpu_register_type spsr;
   rex_cpu_register_type r[13];  /* r0-r7,r8-r12 */
   rex_cpu_register_type lr;    /* r14        */
   rex_cpu_register_type pc;    /* r15        */
} rex_context_frame_type;


/* ------------------------------------------------------------------------
** REX Timers
** - Describes timers used by REX
** - Fields should not be accessed directly
** ------------------------------------------------------------------------ */
typedef struct rex_timer_struct
{
   struct
   {
      struct rex_timer_struct *next_ptr;        
      struct rex_timer_struct *prev_ptr;        
   } link;
   rex_timer_cnt_type   cnt;        /* Current timer countdown           */
   rex_tcb_type         *tcb_ptr;   /* Pointer to the tcb to be signaled */
   rex_sigs_type        sigs;       /* Signals to set                    */
} rex_timer_type;

/* ------------------------------------------------------------------------
** REX errors
** ------------------------------------------------------------------------ */
#define  REX_ERROR_MISALIGNED_DATA                          0x01
#define  REX_ERROR_DATA_ABORT                               0x02
#define  REX_ERROR_PREFETCH_ABORT                           0x03
#define  REX_ERROR_DOUBLE_ABORT_EXCEPTION                   0x04
#define  REX_ERROR_UNDEFINED_INSTR                          0x05
#define  REX_ERROR_INVALID_PARAMETER                        0x06
#define  REX_ERROR_USER_DEVICE_ACCESS                       0x07

/* ------------------------------------------------------------------------
** REX priorities
** ------------------------------------------------------------------------ */
#define REX_MAX_PRIORITY 65535

/* ------------------------------------------------------------------------
** Stack Watermark checking support.
** Make the definition of REX_KERNEL_STACK_SIZE available in header file.
** Externalize the declaration of rex_kernel_stack[].
** ------------------------------------------------------------------------ */
#if defined FEATURE_SI_STACK_WM
#error code not present
#endif



/*===========================================================================

                            MACRO DECLARATIONS

===========================================================================*/


/*===========================================================================
MACRO TASKLOCK/TASKFREE

DESCRIPTION
  TASKLOCK prevents context switching from the point at which it is invoked
  until a matching TASKFREE is invoked.  While task locked, the current task
  is guaranteed to not be replaced by a different task, even if the other
  task has a higher priority.  However, TASKLOCK does not block interrupts
  from occurring.  An ISR will pre-empt the task, but even if the ISR causes
  a higher priority task to become ready, control will still resume at the
  interrupted task.

  TASKFREE re-enables context switching.
===========================================================================*/
#define TASKLOCK() { long pri = rex_set_pri(REX_MAX_PRIORITY);

#define TASKFREE() rex_set_pri(pri); }


/*===========================================================================

                           FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION rex_init

DESCRIPTION
  Initializes the REX subsystem

DEPENDENCIES
  p_tcb must be a valid task control block.
  p_task must be a valid function pointer.  

RETURN VALUE
  None

SIDE EFFECTS
  When the initialization is done, this thread will wait until
  the exit event is triggered at which point it will clean up 
  and return.

===========================================================================*/
extern void rex_init(
  void *                 p_istack,              /* interrupt stack      */
  rex_stack_size_type    p_istksiz,             /* interrupt stack size */ 
  rex_tcb_type           *p_tcb,                /* task control block   */
  void *                 p_stack,               /* stack                */ 
  rex_stack_size_type    p_stksiz,              /* stack size           */
  rex_priority_type      p_pri,                 /* task priority        */
  void                   (*p_task)( dword ),    /* task function        */
  dword                  p_param                /* task parameter       */
);


/*===========================================================================

FUNCTION rex_tick

DESCRIPTION
  Called by external clock service to tell REX time has elapsed

DEPENDENCIES
  Interrupts must be disabled.

RETURN VALUE
  None

SIDE EFFECTS
  May expire timers in REX and schedule tasks

===========================================================================*/
extern void rex_tick(
  rex_timer_cnt_type msecs      /* number of milliseconds elapsed */
);



/*===========================================================================

FUNCTION rex_def_timer

DESCRIPTION
  Initializes a REX timer

DEPENDENCIES
  p_timer must be a valid timer
  tcb_ptr must be a valid task control block

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rex_def_timer(
   rex_timer_type *p_timer,      /* pointer to a valid timer structure */
   rex_tcb_type   *tcb_ptr,      /* tcb to associate with the timer    */
   rex_sigs_type  sigs           /* sigs to set upon timer expiration  */
);



/*===========================================================================

FUNCTION rex_clr_timer

DESCRIPTION
  Clears a running timer

DEPENDENCIES
  None

RETURN VALUE
  The countdown of the timer (in ms).

SIDE EFFECTS
  None

===========================================================================*/
extern rex_timer_cnt_type rex_clr_timer(
  rex_timer_type *p_timer    /* pointer to timer to clear */
);



/*===========================================================================

FUNCTION rex_set_timer

DESCRIPTION
  Sets a REX timer to expire after the specified interval (in ms).

DEPENDENCIES
  None

RETURN VALUE
  The last countdown of the timer

SIDE EFFECTS
  Could cause a call to set sigs and thus induce a task swap.

===========================================================================*/
extern rex_timer_cnt_type rex_set_timer(
  rex_timer_type       *p_timer,  /* pointer to timer to set */
  rex_timer_cnt_type   msecs      /* value in milliseconds   */
);



/*===========================================================================

FUNCTION rex_get_timer

DESCRIPTION
  Get the current countdown of a REX timer

DEPENDENCIES
  p_timer must be a valid timer.

RETURN VALUE
  The countdown remaining (in ms).

SIDE EFFECTS
  None

===========================================================================*/
extern rex_timer_cnt_type rex_get_timer(
   rex_timer_type *p_timer   /* pointer to the timer to get */
);



/*===========================================================================

FUNCTION rex_def_task

DESCRIPTION
  Creates a REX Task

DEPENDENCIES
  p_tcb must be a valid tcb block.
  p_task must be a valid function pointer.

RETURN VALUE
  None

SIDE EFFECTS
  Defines a new rex task that will be placed in the task list.
  Creates a new thread and an event for the new task.
  The task starts running only if it is the highest priority ready task.

===========================================================================*/
extern void rex_def_task(
  rex_tcb_type         *p_tcb,                 /* valid tcb for new task   */
  void *               p_stack,                /* stack for new task       */
  rex_stack_size_type  p_stksiz,               /* stack size for new task  */
  rex_priority_type    p_pri,                  /* priority for new task    */
  void                 (*p_task)( dword ),     /* task startup function    */
  dword                p_param                 /* parameter for new task   */
);



/*===========================================================================

FUNCTION rex_task_pri

DESCRIPTION
  Set the priority of the specified task to the given priority

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The priority change will cause a change in the whole scheduling order for 
  all of the tasks. This could result in an immediate task swap.

===========================================================================*/
extern rex_priority_type rex_task_pri(
  rex_tcb_type       *p_tcb,     /* tcb to set priority on */
  rex_priority_type  p_pri       /* the new priority       */
);



/*===========================================================================

FUNCTION rex_set_pri

DESCRIPTION
  Sets the current task's (i.e. the calling task's) priority to the specified
  value.

DEPENDENCIES
  None

RETURN VALUE
  Previous priority

SIDE EFFECTS
  Affects schedulability of system

===========================================================================*/
extern  rex_priority_type rex_set_pri(
   rex_priority_type p_pri       /* the new priority */
);



/*===========================================================================

FUNCTION rex_get_pri

DESCRIPTION
  Get the priority of this task

DEPENDENCIES
  None

RETURN VALUE
  Current priority

SIDE EFFECTS
  None

===========================================================================*/
extern rex_priority_type rex_get_pri( void );


/*===========================================================================

FUNCTION rex_self

DESCRIPTION
  Gets a pointer to the TCB of the currently running task

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern rex_tcb_type* rex_self ( void );



/*===========================================================================

FUNCTION rex_wait

DESCRIPTION
  Waits on specified signals

DEPENDENCIES
  None

RETURN VALUE
  Signals asserted

SIDE EFFECTS
  Will cause a task swap if the signals for the task are not already set at
  the time of the call.

===========================================================================*/
extern rex_sigs_type rex_wait(
  rex_sigs_type p_sigs     /* sigs to wait on */
);



/*===========================================================================

FUNCTION rex_clr_sigs

DESCRIPTION
  Clears the specified signals for the specified TCB

DEPENDENCIES
  None

RETURN VALUE
  Signal mask after clearing

SIDE EFFECTS
  Affects schedulability of specified TCB

===========================================================================*/
extern rex_sigs_type rex_clr_sigs(
  rex_tcb_type *tcb_ptr,     /* tcb for which the signals will be cleared */
  rex_sigs_type sigs         /* which signals to clear                    */
);


/*===========================================================================

FUNCTION rex_set_sigs

DESCRIPTION
  Sets signals for specified TCB, causes REX to reschedule system

DEPENDENCIES
  None

RETURN VALUE
  Signals mask after setting, note, that if the set causes a higher priority
  task to run, and it clears the sigs, the mask returned may indicate
  that the signal was never set (meaning, it was set and processed)

SIDE EFFECTS
  A task swap might happen.

===========================================================================*/
extern rex_sigs_type rex_set_sigs(
  rex_tcb_type *tcb_ptr,     /* tcb for which the sigs will be set */
  rex_sigs_type sigs         /* the sigs to set                    */
);



/*===========================================================================

FUNCTION rex_get_sigs

DESCRIPTION
  Gets the currently asserted signals for the specified task

DEPENDENCIES
  None

RETURN VALUE
  Signal mask

SIDE EFFECTS
  None

===========================================================================*/
extern rex_sigs_type rex_get_sigs(
  rex_tcb_type *p_tcb        /* tcb for which sigs will be returned */
);



/*===========================================================================

FUNCTION rex_timed_wait

DESCRIPTION
  Performs a rex_set_timer followed by a rex_wait.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes a task swap if the signals are not already set for the calling task.

===========================================================================*/
extern rex_timer_cnt_type rex_timed_wait(
  rex_sigs_type       p_sigs,         /* sigs to wait on          */
  rex_timer_type      *p_timer,       /* timer to set and wait on */
  rex_timer_cnt_type  p_cnt           /* timer to wait            */
);


/*===========================================================================

FUNCTION rex_int_lock 

DESCRIPTION
  Lock interrupt

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Affects schedulability of system and ability to service hardware

===========================================================================*/
dword rex_int_lock(void);


/*===========================================================================

FUNCTION  rex_int_free

DESCRIPTION
  free interrupts

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Affects schedulability of system and ability to service hardware

===========================================================================*/
dword rex_int_free(void);


/*===========================================================================

FUNCTION  rex_set_interrupt_vector

DESCRIPTION
  Installs interrupt vector call

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Affects schedulability of system and ability to service hardware

===========================================================================*/
void rex_set_interrupt_vector( rex_vect_type v, void (*fnc_ptr)( void ) );


/*===========================================================================

FUNCTION  rex_enable_interrupt

DESCRIPTION
  free interrupts

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void rex_enable_interrupt( rex_vect_type v );


/*===========================================================================

FUNCTION  rex_disable_interrupt

DESCRIPTION
  Locks interrupts

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Affects schedulability of system and ability to service hardware

===========================================================================*/
void rex_disable_interrupt( rex_vect_type v );


/*===========================================================================

FUNCTION REX_SCHED

DESCRIPTION
  Does the task swap. Is called within rex and should not be called from
  outside of rex. Rex_start_task is an entry point from rex_sched to restore 
  context (called only by rex_init).

DEPENDENCIES
  Rex_sched assumes that the following global variables have reasonable
  values: rex_curr_task, rex_best_task.

RETURN VALUE
  None.  

SIDE EFFECTS
  Rex_sched may set the current task pointer (rex_curr_task) equal to
  the best task pointer (rex_best_task) if they are not already the same.
  Also, rex_sched may cause a task swap to occur from the old current
  task to the new current task.
  
===========================================================================*/
void rex_sched( void );


/*===========================================================================

FUNCTION REX_START_TASK

DESCRIPTION
  Does the task initialization.It is called within REX and should not be
  called from outside of rex. 

DEPENDENCIES
  None.

RETURN VALUE
  None.  

SIDE EFFECTS
  Starts the task by restoring its context
  
===========================================================================*/
void rex_start_task( rex_tcb_type *p_tcb );

/*=============================================================================
 
   FUNCTION DBGBreak
 
   DESCRIPTION - 
     This function does nothing and its only purpose is as a place to set a 
     breakpoint for runtime code warnings.
 
   DEPENDENCIES
     None
     
   RETURN VALUE
     None
 
   SIDE EFFECTS
     None
 
=============================================================================*/

void DBGBreak(void *);

#endif /* END REX_H */

