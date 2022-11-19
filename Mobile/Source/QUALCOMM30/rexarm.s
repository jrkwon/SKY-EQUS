;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
;
;         R E X  A R M   P O R T  A S S E M B L Y   R O U T I N E S 
;
; GENERAL DESCRIPTION
;   This module contains the ARM port of REX functions that needed to be 
;   coded in assembly. 
;
; EXTERNALIZED FUNCTIONS
;   rex_sched
;     If the current task is not the best task, a context switch occurs
;   rex_int_lock
;     Locks interrupts
;   rex_int_free
;     Unlocks interrupts
;
; INITIALIZATION AND SEQUENCING REQUIREMENTS
;   The bulk of the REX code is contained in rex.c. 
;
;  Copyright (c) 1998,1999,2000 by QUALCOMM, Incorporated.  All Rights Reserved.
;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

;============================================================================
;
;                      EDIT HISTORY FOR MODULE
;
; This section contains comments describing changes made to the module.
; Notice that changes are listed in reverse chronological order.
;
; $Header:   L:/src/asw/MSM5000/VCS/rexarm.s_v   1.6   18 Aug 2000 14:41:14   bcalder  $
;
; when      who    what, where, why
; --------  -----  -----------------------------------------------------------
; 08/18/00   bgc   Added r8-r11 to the task context for task switches.
; 06/20/00   jcw   Added support for interworking
; 03/27/00   tim   Added timetest support, fixed bugs with rex_int_nest_level
; 06/14/99   tac   Added support for pre-emptive interrupts.
; 05/19/99   ms    Not passing rex_curr_task to rex_report_stack_overflow,
;                     since this value is available as a global anyway.
; 04/07/99   sk    Removed warnings for ARM 2.5 SDT.
; 03/19/99   ms    Defined stack overflow handlers.
; 02/19/99   ms    Optimized IRQ_Handler to save complete context only when
;                  required. Also running in Supervisor mode.
;                  Removed macros save_context_from_interrupt,
;                  save_context_from_task and restore_context, since these are
;                  not being used any more.
;                  Modified rex_sched to conform to IRQ_Handler optimization.
; 02/12/99   ms    Added support for TIMETEST.
; 02/04/99   ms    Replaced 'blx' with 'blatox'.
;                  Use of fiq_stack and call to rex_fiq_vector were featurized
;                  inside FIQ_Handler using FEATURE_INT_125_FIQ.
; 01/18/99   ms    Moved context frame related macros from armasm.h.
; 11/30/98  jkl    Added Interrupt Vectors. Revised and cleaned up code
; 09/29/98  jkl    Adapted for new context frame
; 08/14/98  jct    Changed processing in irq to save context and restore to
;                  best_task on exit.  Changed rex_sched to save/restore same
;                  context frame as irq and to update curr_task to best_task
;                  regardless of interrupt level           
; 05/29/98  jbh    Created file
;=============================================================================

#include "arm.h"
#include "rexarm.h"
#include "armasm.h"
#include "customer.h"

#if defined TIMETEST
   #include "timetest.h"
#endif

;=============================================================================
;
;                            REGION and SIZE
;
;=============================================================================

  AREA RexRom, CODE, READONLY
  CODE32

;=============================================================================
;
;                       INCLUDE FILES FOR MODULE
;
;=============================================================================

;=============================================================================
;
;              CONSTANTS, MACROS AND STRUCTURE TYPES FOR MODULE
;
;=============================================================================

;=============================================================================
;
;                      IMPORTED FUNCTIONS AND SYMBOLS
;
;=============================================================================
  IMPORT rex_curr_task           ; These are from rex.c 
  IMPORT rex_best_task           
  IMPORT rex_int_stack  
  IMPORT rex_irq_vector
  IMPORT rex_fiq_vector

  IMPORT rex_int_nest_level
  
#if defined FEATURE_STACK_CHECK
  IMPORT rex_report_stack_overflow
#endif

#if defined FEATURE_INT_125_FIQ
  IMPORT fiq_stack               ; from bootmem.c
#endif

#if defined TIMETEST
  IMPORT rex_led_buf
#endif

;=============================================================================
;
;                      EXPORTED FUNCTIONS AND SYMBOLS (See Macro Leaf_Node)
;
;=============================================================================
;   EXPORT IRQ_Handler          ; Irq handler
;   EXPORT FIQ_Handler          ; Fiq handler
;   EXPORT rex_sched            ; The scheduler
;   EXPORT rex_start_task       ; Start task in Rex_init
;   EXPORT rex_int_lock         ; Disable interrupts
;   EXPORT rex_int_free         ; Enable interrupts
;   EXPORT rex_set_CPSR         ; Set CPSR
;   EXPORT DBGbreak             ; Debug Break
;=============================================================================

;=============================================================================
;
; FUNCTION IRQ_handler 
;
; DESCRIPTION
;    Entry point of the IRQ interrupt handling services
;
; FORMAL PARAMETERS
;   None
;
; DEPENDENCIES
;   tramp_init must have been initialized.
;
; RETURN VALUE
;   None
;
; SIDE EFFECTS
;   too many to describe
;=============================================================================
  EXPORT IRQ_Handler
  ALIGN
  ROUT
IRQ_Handler
    sub     lr, lr, #4     ; adjust return address (ARM requirement)

; --------------------------------------------------------------------
; Some registers will be trashed by the interrupt handler function
; which is a C routine. Save those registers on the interrupt stack.
; Save the SPSR too. That will be trashed by the subsequent nested 
; interrupt.
; --------------------------------------------------------------------  
    stmfd   sp!, {r0-r3, r10, r12, r14}
    mrs     r0, SPSR
    stmfd   sp!, {r0}

; --------------------------------------------------------------------
; Increment the nesting level counter.
; --------------------------------------------------------------------  
    ldr     r0, =rex_int_nest_level
    ldrh    r1, [r0]
    add     r1, r1, #1
    strh    r1, [r0]

; --------------------------------------------------------------------  
; Switch to System Mode and save r14 on stack.
; --------------------------------------------------------------------  
    msr     CPSR_c, #PSR_System:OR:PSR_Irq_Mask 
    stmfd   sp!, {r14}


#if defined TIMETEST
; --------------------------------------------------------------------
; TIMETEST_INT is a special value denoting interrupt.  
; Write that value 
; --------------------------------------------------------------------
    mov     r0, #TIMETEST_INT       ; interrupt id on the led
    ldr     r1, =rex_led_buf        ; load contents of leds buffer
    ldr     r2, =TIMETEST_PORT      ; write to the leds
    strh    r0, [r2]
    strh    r0, [r1]                ; save new leds contents
#endif

; --------------------------------------------------------------------
; Call the interrupt handler
; --------------------------------------------------------------------  
#if defined(FEATURE_STACK_CHECK)
    ldr     sl, =rex_int_stack
    ldr     sl, [sl]
#endif
    ldr     r3, =rex_irq_vector
    ldr     r3, [r3]
    blatox  r3

; --------------------------------------------------------------------  
; Pop r14 from stack and switch to IRQ Mode.
; --------------------------------------------------------------------  
    ldmfd   sp!, {r14}
    msr     CPSR_c, #PSR_Irq:OR:PSR_Irq_Mask 

; --------------------------------------------------------------------
; Decrement the nesting level counter.
; --------------------------------------------------------------------  
    ldr       r0, =rex_int_nest_level
    ldrh      r1, [r0]
    subs      r1, r1, #1      ; Notice it is subs. Set Z flag if r1==0
    strh      r1, [r0]

;---------------------------------------------------------------------
; Branch if rex_int_nest_level <> 0.
; The Z flag was set (or not set) by the subs instruction above.
;---------------------------------------------------------------------
    bne       task_swap_not_required

;---------------------------------------------------------------------
; Is task swap required even though rex_int_nest_level == 0?
; Not required if curr task is same as best task.
;---------------------------------------------------------------------
    ldr     r0, =rex_curr_task
    ldr     r0, [r0]
    ldr     r1, =rex_best_task
    ldr     r1, [r1]
    cmp     r0, r1
    beq     task_swap_not_required
    
;---------------------------------------------------------------------
; Restore the SPSR from stack.
;---------------------------------------------------------------------
    ldmfd     sp!, {r0}
    msr       SPSR_f, r0
    msr       SPSR_c, r0

;---------------------------------------------------------------------
; Restore the registers from IRQ mode stack.
;---------------------------------------------------------------------
    ldmfd     sp!, {r0-r3, r10, r12, r14}

;---------------------------------------------------------------------
; Save context of outgoing task.
;---------------------------------------------------------------------

    msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask 

    sub     sp, sp, #4             ; Space for Return address (pc)             
    stmfd   sp!, {r0-r12,lr}    ; Other registers
    sub     sp, sp, #4             ; Space for SPSR 
    mov     r1, sp                          

    ldr     r0, =rex_curr_task         ; Save sp of outgoing task into TCB.
    ldr     r2, [r0]                   ; 
    str     r1, [r2, #REX_TCB_STACK_POINTER_OFFSET]

    msr     CPSR_c, #PSR_Irq:OR:PSR_Irq_Mask  ; change to IRQ mode
                                              ; save Return address into context frame.
    str     r14,  [r1, #REX_INTERRUPT_FRAME_PC_OFFSET]
    mrs     r3, SPSR                          ; save SPSR into context frame.
    str     r3,  [r1, #REX_INTERRUPT_FRAME_SPSR_OFFSET]

                                              ; Back to Supervisor
    msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask

;---------------------------------------------------------------------
; Restore context of incoming task.
;---------------------------------------------------------------------
; Reminder ... r0 = address of rex_curr_task variable.
;---------------------------------------------------------------------

    ldr     r1, =rex_best_task
    ldr     r1, [r1]
    str     r1, [r0]           ; curr_task <- best_task

#if defined TIMETEST
;---------------------------------------------------------------------
; TIMETEST - writing current task id on leds port
;---------------------------------------------------------------------
    mov r0, r1          ; r1 contains rex-best-task
    bl func_timetest
    mov r1, r0
#endif
        
#if defined(FEATURE_STACK_CHECK)
    ldr     sl, [r1, #REX_TCB_STACK_LIMIT_OFFSET]      ; Stack Limit
#endif

    ldr     sp, [r1, #REX_TCB_STACK_POINTER_OFFSET]    ; Stack pointer
    ldmfd   sp!, {r0}                                  ; Get SPSR
    msr     SPSR_f, r0
    msr     SPSR_c, r0                                 ; Restore SPSR.
    mov     a2, sp
    add     sp, sp, #&3c                  ; adjust {r0-r7, r12, r14, pc}
    ldmfd   a2, {r0-r12, r14, pc}^    ; Load and return 
                                          ; This will also restore CPSR.
        
;---------------------------------------------------------------------
; Control comes here when a task swap is not required.
;---------------------------------------------------------------------
task_swap_not_required
    ;---------------------------------------------------------------------
    ; Restore the SPSR from stack.
    ;---------------------------------------------------------------------
    ldmfd     sp!, {r0}
    msr       SPSR_f, r0
    msr       SPSR_c, r0

#if defined TIMETEST
;---------------------------------------------------------------------
; TIMETEST - writing current task id on leds port
;---------------------------------------------------------------------
    ldr     r0, =rex_best_task
    ldr     r0, [r0]
    mov r12, lr          ; cannot destroy lr
    bl func_timetest
    mov lr, r12
#endif

    ;-----------------------------------------------------------------
    ; Load saved registers and return
    ; This will also restore CPSR.
    ;-----------------------------------------------------------------
    ldmfd   sp!, {r0-r3, r10, r12, pc}^

IRQ_Handler_end  


;=============================================================================
;
; FUNCTION FIQ_Handler 
;
; DESCRIPTION
;   Entry point of the FIQ interrupt handling services.
;   For future implementation.
;
; FORMAL PARAMETERS
;   None
;
; DEPENDENCIES
;   No task switch should happen in this ISR
;
; RETURN VALUE
;   None
;
; SIDE EFFECTS
;   None
;
;=============================================================================
  EXPORT FIQ_Handler
  ALIGN
  ROUT

FIQ_Handler
    sub   lr, lr, #4        ; adjust return address (ARM requirement)
    stmfd sp!, {a1-a4,lr}

#if defined TIMETEST
; --------------------------------------------------------------------
; TIMETEST_INT is a special value denoting Interrupt.
; Write that value to the TIMETEST port.
; -------------------------------------------------------------------- 
    mov     a1, #TIMETEST_INT       ; Get that special value.
    ldr     a2, =rex_led_buf
    ldr     a3, =TIMETEST_PORT
    strh    a1, [a3]                ; Write it to TIMETEST port.
    strh    a1, [a2]                ; Shadow this val into rex_led_buf.
#endif

#if defined(FEATURE_STACK_CHECK)
#if defined FEATURE_INT_125_FIQ
    ldr   sl, =fiq_stack
    ldr   sl, [sl]
#endif
#endif        

; --------------------------------------------------------------------
; Call the interrupt handler
; --------------------------------------------------------------------  

#if defined FEATURE_INT_125_FIQ
    ldr     a4, =rex_fiq_vector
    ldr     a4, [a4]
    blatox  a4
#endif
    
#if defined TIMETEST
; --------------------------------------------------------------------
; Each task is assigned a unique value which is stored in 'leds' field 
; in TCB. Write the special value for this task onto TIMETEST port.
; -------------------------------------------------------------------- 
    ldr     a1, = rex_curr_task
    ldr     a1, [a1]
    bl      func_timetest
#endif
    
    ldmfd    sp!, {a1-a4,pc}^
    
FIQ_Handler_end  


;=============================================================================
;
; FUNCTION rex_sched
;
; DESCRIPTION
;   This function does the actual task swapping. It should only be called by
;   other rex functions and **NEVER** by the user.
;
;   Typically, rex_sched is called immediately after a REX service function
;   changes the best task pointer. This occurs, for example, when a task
;   of higher priority than the current task is reactivated. Rex_sched deter-
;   mines whether the current task pointer and best task pointer still point
;   to the same task. If so, then the current task is still the best task and
;   no further scheduling action is required. Rex_sched merely returns. 
;
;   Otherwise, the current task is no longer the best task. Rex_sched first
;   sets the current task pointer equal to the best task pointer. It then
;   determines whether it has been called from task level or interrupt level.
;   If called from task level, rex_sched saves the context of the 'old'
;   current task and restores the context of the 'new' current task. If called
;   from interrupt level, rex_sched does not perform a context switch; the
;   switch will be deferred until a return is made to task level.
;
; FORMAL PARAMETERS
;   None.
;
; DEPENDENCIES
;   Rex_sched assumes that the following global variables have reasonable
;   values: rex_curr_task, rex_best_task.
;
; RETURN VALUE
;   None.
;
; SIDE EFFECTS
;   Rex_sched may set the current task pointer (rex_curr_task) equal to
;   the best task pointer (rex_best_task) if they are not already the same.
;   Also, rex_sched may cause a task swap to occur from the old current
;   task to the new current task.
;
;=============================================================================
 LEAF_NODE rex_sched

    mrs   a3, CPSR               ; Save the CPSR for later. 
  
    orr   a1, a3, #PSR_Irq_Mask  ; locks interrupts
    msr   CPSR_c, a1

;---------------------------------------------------------------------------
; If we are in interrupt level, just return
;---------------------------------------------------------------------------
    and   a1, a3, #PSR_Mode_Mask
    cmp   a1, #PSR_Supervisor    ; If not in Supervisor mode, do not swap
    bne   rex_sched_exit_1       ; until we revert back to task level 
 
;---------------------------------------------------------------------------
; Make sure we need to task swap
;---------------------------------------------------------------------------
    ldr   a2, =rex_best_task       ; load the best task into a2
    ldr   a2, [a2]                 ; dereference best task        
    ldr   a4, =rex_curr_task       ; load the current task into a4
    ldr   a1, [a4]                 ; dereference current task
    cmp   a2, a1                   ; if current task == best task just return
    beq   rex_sched_exit_1 
 
;---------------------------------------------------------------------------
; Set the curr_task to the new value
;---------------------------------------------------------------------------
    str a2, [a4]                 ; set rex_curr_task=rex_best_task
    mov a4, a1                   ; a4 points now to the last (former current) task

;---------------------------------------------------------------------------
; Increment the slice count.
;---------------------------------------------------------------------------
    ldr   a1, [a2, #REX_TCB_SLICES_OFFSET]       ; load up the slice count
    add   a1, a1, #1                             ; increment it
    str   a1, [a2, #REX_TCB_SLICES_OFFSET]       ; store it
 
; --------------------------------------------------------------------
; Save volatile context of CPU
; --------------------------------------------------------------------  
    stmfd   sp!, {lr}               ; Return address. 
    sub     sp, sp, #8              ; no need to store r12,r14 in task context.
    stmfd   sp!, {r4-r11}
    sub     sp, sp, #16             ; Subtract a1-a4 location
#ifdef __APCS_INTERWORK
    tst     lr, #1                  ; Test for thumb return address
    orrne   a3, a3, #PSR_Thumb_Mask ; Return in Thumb mode   
#else
    orr     a3, a3, #PSR_Thumb_Mask ; Return in Thumb mode
#endif    
    stmfd   sp!, {a3}               ; First line on rex_sched saves CPSR in a3!!!

;---------------------------------------------------------------------------
; Save the context on stack
;---------------------------------------------------------------------------
    str     sp, [a4,  #REX_TCB_STACK_POINTER_OFFSET] 
    mov     a1, a2                  ; a1 = the current task

;---------------------------------------------------------------------------
; rex_start_task_1 is an alternate entry point.
; void rex_start_task(rex_tcb_type *);
; This implies that a1 = current task tcb pointer.
;---------------------------------------------------------------------------
rex_start_task_1
  
#if defined TIMETEST
; --------------------------------------------------------------------
; TIMETEST - writing current task leds on timetest port.
; -------------------------------------------------------------------- 
    bl      func_timetest
#endif
  
; --------------------------------------------------------------------
; Restore the user state, note this may not be the state saved above
; since the call the rex_sched may have changed which stack the handler
; is working on. Note, a context switch will happen here.
; --------------------------------------------------------------------

#if defined(FEATURE_STACK_CHECK)
    ldr     sl, [a1, #REX_TCB_STACK_LIMIT_OFFSET]      ; Stack Limit.
#endif
    ldr     sp, [a1, #REX_TCB_STACK_POINTER_OFFSET]    ; Load the stack pointer
    ldmfd   sp!, {a1}                                  ; Restore SPSR (in a1)
    msr     SPSR_f, a1                         ; Load SPSR
    msr     SPSR_c, a1                         ; Load SPSR
    mov     a1, sp                           ; Load sp in a1.
    add     sp, sp, #&3c                     ; adjust {r0-r7, r12, lr, pc}
    ldmfd   a1, {r0-r12,lr,pc}^        ; Load and return, sp already adjusted. 
  
; --------------------------------------------------------------------
; If no context swap occurred, the execution exits thru here
; --------------------------------------------------------------------

rex_sched_exit_1
    msr   CPSR_f, a3               ; Restore interrupts as prior to rex_sched
    msr   CPSR_c, a3               ; Restore interrupts as prior to rex_sched

 LEAF_NODE_END

; END rex_sched

;=============================================================================
;
;  FUNCTION rex_start_task
;
;  DESCRIPTION
;    Spawn the task without saving any context. rex_start_task_1 is an entry
;    point in rex_sched, when context is re-installed. Used only by rex_init
;    to initialize user first task. Alternate_entries is an entry_node macro
;    that creates Thumb mode entry points to the scheduler. 
;    This function should **NEVER** be called by user.
;
;  DEPENDENCIES
;    rex_curr_task must point to user first task tcb
;
;  RETURN VALUE
;    None
;
; SIDE EFFECTS
;    The user-first-task will be spawned
;
;=============================================================================
 
 ENTRY_NODE Alternate_entries        
        CODE16
        EXPORT  rex_start_task
rex_start_task
        ldr     a4, =rex_start_task_1
        bx      a4

 ENTRY_NODE_END
 
;=============================================================================
;
;  FUNCTION rex_int_lock
;
;  DESCRIPTION
;    Disable the IRQ and FIQ exceptions.
;
;  DEPENDENCIES
;    None
;    
;  PROTOTYPE
;    dword rex_int_lock(void)
;
;  RETURN VALUE
;    The previous state of the IRQ and FIQ flags.
;
;  SIDE EFFECTS
;    None
;
;=============================================================================
 LEAF_NODE rex_int_lock
       
        mrs     a1, CPSR
        orr     a2, a1, #PSR_Irq_Mask:OR:PSR_Fiq_Mask
        msr     CPSR_c, a2
        and     a1, a1, #PSR_Irq_Mask:OR:PSR_Fiq_Mask

 LEAF_NODE_END


;=============================================================================
;
;  FUNCTION rex_int_free
;
;  DESCRIPTION
;    Reenable the IRQ and FIQ exceptions.
;
;  DEPENDENCIES
;    None
;    
;  PROTOTYPE
;    void rex_int_free(void)
;
;  RETURN VALUE
;    The previous state of the IRQ and FIQ flags.
;
;  SIDE EFFECTS
;    None
;
;=============================================================================
 LEAF_NODE rex_int_free
        
        mrs     a1, CPSR
        bic     a2, a1, #PSR_Irq_Mask:OR:PSR_Fiq_Mask
        msr     CPSR_c, a2
        and     a1, a1, #PSR_Irq_Mask:OR:PSR_Fiq_Mask
       
 LEAF_NODE_END

;===========================================================================
;
;   FUNCTION REX_ENABLE_INTERRUPT
;
;   DESCRIPTION
;    Enables the interrupt associated with id v rex function.
;
;   DEPENDENCIES
;    None.
;
;  PROTOTYPE
;    rex_enable_interrupt( rex_vect_type v )
;  
;  RETURN VALUE
;    None.  
;
;  SIDE EFFECTS
;    None.
;  
;===========================================================================

 LEAF_NODE rex_enable_interrupt
           
        mrs    r1, CPSR 
        cmp    r0, #P_IRQ_VECT
        biceq  r1, r1, #PSR_Irq_Mask
        bicne  r1, r1, #PSR_Fiq_Mask
        msr    CPSR_c, r1
 
 LEAF_NODE_END

;===========================================================================
;
;   FUNCTION REX_DISABLE_INTERRUPT
;
;   DESCRIPTION
;    Disables the interrupt associated with id v rex function.
;
;   DEPENDENCIES
;    None.
;
;  PROTOTYPE
;    rex_disable_interrupt( rex_vect_type v )
;  
;  RETURN VALUE
;    None.  
;
;  SIDE EFFECTS
;    None.
;  
;===========================================================================

 LEAF_NODE rex_disable_interrupt
        
        mrs    r1, CPSR 
        cmp    r0, #P_IRQ_VECT
        orreq  r1, r1, #PSR_Irq_Mask
        orrne  r1, r1, #PSR_Fiq_Mask
        msr    CPSR_c, r1
 
 LEAF_NODE_END

;=============================================================================
;
;  ENTRY rex_set_cpsr
;
;  DESCRIPTION 
;    Set CPSR to user specified value
;
;  DEPENDENCIES
;    Must be done as an ARM instruction so making this a leaf node causes
;    the compiler to handle getting in and out of ARM/Thumb mode
;    
;  REGISTERS USED: 
;    CPSR, a1, a2, a3,lr
;
;  SIDE EFFECTS
;    None 
;
;=============================================================================

 LEAF_NODE rex_set_cpsr
        mrs     a3, CPSR       ; Save the value of the old msr
        mov     a2, lr         ; Save off the shadowed link register.
        msr     CPSR_f, a1
        msr     CPSR_c, a1
        mov     a1, a3         ; Return the old value of the msr
        mov     lr, a2
 LEAF_NODE_END
 
;=============================================================================
; 
;   FUNCTION DBGBreak
; 
;   DESCRIPTION - 
;     This function does nothing and its only purpose is as a place to set a 
;     breakpoint for runtime code warnings.
; 
;   DEPENDENCIES
;     None
;     
;   RETURN VALUE
;     None
; 
;   SIDE EFFECTS
;     None
; 
;=============================================================================

 LEAF_NODE      DBGBreak
         b      DBGBreak_32
 LEAF_NODE_END


#if defined TIMETEST
;=============================================================================
; 
;   FUNCTION FUNC_TIMETEST
; 
;   DESCRIPTION - 
;     Read the leds field from the current task TCB.
;     Each task is assigned a unique value.
;     Write that value to TIMETEST_PORT and also shadow it in
;     the global variable rex_led_buf.
; 
;   FORMAL PARAMETERS
;     a1 - current TCB pointer
;
;   DEPENDENCIES
;     Only called in ARM mode
;     
;   RETURN VALUE
;     None
; 
;   SIDE EFFECTS
;     Registers destroyed:   a2, a3, a4, lr
;   
;=============================================================================
func_timetest
      
    ldrh    a2, [a1, #REX_TCB_LED_OFFSET] ; Load Task id (0-31)
    ldr     a4, =rex_led_buf
    ldrh    a3, [a4]                      ; read rex_led_buf
    bic     a3, a3, #&1f                  ; mask lower 5 bits
    orr     a2, a2, a3
    strh    a2, [a4]                      ; Save back in leds buffer
    ldr     a4, =TIMETEST_PORT            ; Write to the Led port
    strh    a2, [a4]
    mov     pc, lr                        ; Return

;=============================================================================
; TIMETEST endif
;=============================================================================
#endif

#if defined FEATURE_STACK_CHECK
;=============================================================================
;  
;  FUNCTION __16__rt_stkovf_split_small
;           __16__rt_stkovf_split_big
;                   
;  DESCRIPTION
;    These functions are stack overflow handlers which are called when a stack
;    over flow is detected. 
;  
;    __16__rt_stkovf_split_small  is called if the frame is less than 256 bytes.
;    __16__rt_stkovf_split_big    is called if the frame is more than 256 bytes.
;  
;    We do not differntiate between small/big frames.
;
;    This function calls a C routine which calls ERR_FATAL.
;
;    It clears the stack limit register before calling the C routine since
;    we do not want the stack check to fire anymore.
;    This routine had to be written in assembly because the ARM tcc compiler
;    does not allow modification of stack limit register in inline assembly
;    code (__asm).
;
;    The C function, rex_report_stack_overflow is required because 
;    ERR_FATAL macro is available only in C files.
;
;    Stack checking code is inserted in the beginning of each function by the
;    compiler when code is compiled with swstackcheck option.
;  
;  DEPENDENCIES
;    The macros LEAF_NODE and LEAF_NODE_END are being used to define the 
;    entry and exit points. Although these macros are meant to be used in
;    functions which do not make function calls, it is acceptable to use
;    them here since this is a suicidal path anyway (ERR_FATAL to be called)
;    and does not need to preserve the stack frame.
;    
;  RETURN VALUE
;    None. This function branches to a C routine which never returns.
;  
;  SIDE EFFECTS
;    None.
;  
;=============================================================================

  LEAF_NODE  __16__rt_stkovf_split_small

        mov     r0, r13     ; First  paramter for rex_report_stack_overflow.
        mov     r1, r14     ; Second paramter for rex_report_stack_overflow.
        mrs     r2, CPSR    ; Third parameter for rex_report_stack_overflow.

        eor     r10, r10, r10    ; Clear the stack limit register.

        ; Call  void rex_report_stack_overflow(dword sp, dword lr, dword mode);

        ldr     r3, =rex_report_stack_overflow
        blatox  r3

inf_loop
        b       inf_loop    ;  Should never reach this line. 
                            ;  If it does then the infinite loop will trigger
                            ;  the watchdog.

  LEAF_NODE_END


  LEAF_NODE __16__rt_stkovf_split_big

        ;------------------------------------------------------------------
        ; Simply branch to the routine above.
        ; Do not link because lr should not change.
        ;------------------------------------------------------------------
        b    __16__rt_stkovf_split_small_32

  LEAF_NODE_END

;=============================================================================
; FEATURE_STACK_CHECK endif
;=============================================================================
#endif  

; Termination Token for ARM Assembly files
  END 

