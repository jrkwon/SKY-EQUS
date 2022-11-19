;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
;
;               A P P L I C A T I O N   B O O T    S E R V I C E S
;
; GENERAL DESCRIPTION
;
; EXTERNALIZED FUNCTIONS
;
; INITIALIZATION AND SEQUENCING REQUIREMENTS
;
; Copyright (c) 1998,1999 by QUALCOMM, Incorporated.  All Rights Reserved.
;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
; 
;                           EDIT HISTORY FOR FILE
;  This section contains comments describing changes made to the module.
;  Notice that changes are listed in reverse chronological order.
;
;$Header:   L:/src/asw/MSM5000/VCS/bootapp.s_v   1.4   02 Apr 2001 20:40:52   knordin  $
;   
;when       who     what, where, why
;--------   ---     ----------------------------------------------------------
;04/02/01   kjn     Added & EXPORTED defines for RAM_IRQ_VECTOR & RAM_FIQ_VECTOR.
;01/09/01   ejv     Changed function Sleep() to uP_Sleep().
;03/17/99    ms     Moved check block to bootsys.s for Virtual Boot Block.
;02/22/99   evm     Added function Sleep.
;02/19/99    ms     Now running in Supervisor mode.
;                   r13 of FIQ mode is initialized in FEATURE_INT_125_FIQ.
;10/28/98   jkl     removed system_init_1 and bsp calls
;09/13/98   hcg     Moved check block segment.  Added additional fields.
;07/30/98   hcg     Reserved additional bytes for check segment to match 186 builds.
;07/26/98   hcg     Revised for coding standard, removed unused code
;06/01/98   st      Created
;
;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


;============================================================================
;
;                           MODULE INCLUDE FILES
;
;============================================================================

#include "customer.h"
#include "armasm.h"        
#include "target.h"


;============================================================================
;
;                             MODULE IMPORTS
;
;============================================================================
                
        ; Import the linker generated symbols that correspond to the base
        ; addresses and sizes of the application code and data areas in both 
        ; ROM and RAM.

        IMPORT |Load$$APP_RAM$$Base|      
        IMPORT |Image$$APP_RAM$$Base|  
        IMPORT |Image$$APP_RAM$$Length|     
        IMPORT |Image$$APP_RAM$$ZI$$Base|
        IMPORT |Image$$APP_RAM$$ZI$$Length|

        IMPORT boot_reset_handler
        IMPORT boot_default_isr
        IMPORT boot_abort_handler
        IMPORT IRQ_Handler
        IMPORT FIQ_Handler

        IMPORT  main

        IMPORT irq_stack
        IMPORT sys_stack

#if defined FEATURE_INT_125_FIQ
        IMPORT fiq_stack
#endif

#if defined(FEATURE_STACK_CHECK)        
        IMPORT svc_stack
#endif  
;============================================================================
;
;                             MODULE EXPORTS
;
;============================================================================

        ; Export the renamed linker symbols for use by the other boot modules.
        
        EXPORT Load__APP_RAM__Base
        EXPORT Image__APP_RAM__Base
        EXPORT Image__APP_RAM__Length
        EXPORT Image__APP_RAM__ZI__Base
        EXPORT Image__APP_RAM__ZI__Length

        ; Export handles to functions defined here.

        EXPORT Reset_Handler
        EXPORT uP_Sleep
        EXPORT RAM_IRQ_VECTOR
        EXPORT RAM_FIQ_VECTOR

;============================================================================
;                        APPLICATION INTERRUPT VECTORS
;
;  The IRQ and FIQ interrupts are exported from the boot image to the
;  application image through RAM. 
;  This area must be located at the bottom of application RAM.
;============================================================================

        CODE32
        AREA    BOOTAPP_IVECT, CODE, READONLY
RAM_IRQ_VECTOR        
        b       IRQ_Handler            ; ARM IRQ interrupt
RAM_FIQ_VECTOR        
        b       FIQ_Handler            ; ARM FIQ interrupt

;============================================================================   
; 
;  FUNCTION Reset_Handler
;
;  DESCRIPTION - The system initialization code for REX. Causes the 
;    following to happen:
;    1. system_init_0 - Turn on and initialize dram controller, copies 
;       initialized data from the rom to ram, and zeros the rest of dram. 
;       Notice that there is no stack at this point. system_init_0 cannot 
;       make any function calls.
;
;    2. Initialize stacks for the various ARM modes.
;
;    3. system_init_1 - Initializes rex subsystems timer, task, swi, interrupt, event_log.
;
;    4. main - Calls main which will only do a rex_init.
;
;  DEPENDENCIES
;    None
;    
;  RETURN VALUE
;    None
;
;  SIDE EFFECTS
;    None
;
;============================================================================   
        CODE32
        AREA    APP_RESET, CODE, READONLY
        
 ENTRY_NODE     Reset_Handler
// Supervisor Mode
                                        // --- Turn Ram on and initialise the memory system
#if defined(FEATURE_STACK_CHECK)
        eor     sl, sl, sl              // For stack limit checking.
#endif

// IRQ Mode
                                        // Enter IRQ mode and set up the IRQ stack pointer
        msr     CPSR_c, #PSR_Irq:OR:PSR_Fiq_Mask:OR:PSR_Irq_Mask
        ldr     r13, =irq_stack+IRQ_Stack_Size

// System Mode
                                        // Enter System mode and set up the System stack pointer
        msr     CPSR_c, #PSR_System:OR:PSR_Fiq_Mask:OR:PSR_Irq_Mask
        ldr     r13, =sys_stack+Sys_Stack_Size

#if defined(FIQ_Stack_Size)     
// FIQ Mode
                                        // Enter FIQ mode and set up the FIQ stack pointer
        msr     CPSR_c, #PSR_Fiq:OR:PSR_Fiq_Mask:OR:PSR_Irq_Mask
        eor     r13, r13, r13
#if defined FEATURE_INT_125_FIQ
        ldr     r13, =fiq_stack+FIQ_Stack_Size
#endif
#endif

        // go back into Supervisor mode
        msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask:OR:PSR_Fiq_Mask       

#if defined(FEATURE_STACK_CHECK)
        ldr     sl, =svc_stack          // For stack checking.
#endif

        ldr     a3, =main
        bx      a3                      // never returns

 ENTRY_NODE_END
                                               
;============================================================================
;                         APPLICATION DATA LOCATIONS 
;
;  Locations and sizes of data areas in ROM and RAM are imported from the 
;  linker and stored as data items that are used at runtime by the application 
;  initialization routines.
;============================================================================
                
        ; The $$ convention used by the linker is replaced to avoid the need 
        ; for the -pcc option required by the ARM compiler when symbols that
        ; include $$ are used in C code.
           
Load__APP_RAM__Base 
        DCD |Load$$APP_RAM$$Base|
                    
Image__APP_RAM__Base 
        DCD |Image$$APP_RAM$$Base|
               
Image__APP_RAM__Length 
        DCD |Image$$APP_RAM$$Length|
                    
Image__APP_RAM__ZI__Base 
        DCD |Image$$APP_RAM$$ZI$$Base| 
                    
Image__APP_RAM__ZI__Length 
        DCD |Image$$APP_RAM$$ZI$$Length|

;==========================================================================
; The following section is for Assembler routines callable from C
;==========================================================================

        CODE16
;==========================================================================
;
; FUNCTION  uP_Sleep
;
;       int uP_Sleep(int Address, int Ck1, int Ck2)
; 
; DESCRIPTION
;
; The purpose of "uP_Sleep" is to put ARM in Sleep mode with all external
; Chip selects high.  This is accomplished with the STMIA instruction
; which does a multiple register store to UP_CLK_CTL1 and UP_CLK_CTL2.
;
; PROCEDURE INPUTS
;                   R0 = Address of UP_CLK_CTL1
;                   R1 = initial value to store to UP_CLK_CTL1
;                   R2 = Value to store in UP_CLK_CTL2
; 
; DEPENDIENCIES
; 
; RETURN  VALUE           
;                   R0 = Address given as the input
;
; SIDE EFFECTS
;    Additional Registers Used
;                   R3 = Used as a mask to zero the LSB for second 
;                        UP_CLK_CTL2 store.
;==========================================================================

uP_Sleep
        mov   r3, #1            ; Load a one for manipulating the LSB
        orr   r1, r3            ; Toggle on the LSB
        str   r1, [r0]          ; First write to UP_CLK_CTL1
        bic   r1, r3            ; Mask off the LSB
        stmia r0!, {r1,r2}      ; Sleep will occur during this write
        mov   pc, lr            ; Return to calling procedure
                                           
        END
        
