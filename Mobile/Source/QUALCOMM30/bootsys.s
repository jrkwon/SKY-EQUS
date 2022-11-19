;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
;
;                  S Y S T E M   B O O T    S E R V I C E S
;
; GENERAL DESCRIPTION
;
; EXTERNALIZED FUNCTIONS
;
; INITIALIZATION AND SEQUENCING REQUIREMENTS
;
; Copyright (c) 1998,1999,2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


;*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
;
;                           EDIT HISTORY FOR FILE
;  This section contains comments describing changes made to the module.
;  Notice that changes are listed in reverse chronological order.
;
;$Header:   L:/src/asw/MSM5000/VCS/bootsys.s_v   1.8   02 Apr 2001 20:40:02   knordin  $
;
;when       who     what, where, why
;--------   ---     ----------------------------------------------------------
;04/02/01   kjn     Added IMPORT of RAM_IRQ_VECTOR & RAM_FIQ_VECTOR.
;01/16/01   nxb     Fixed #if of FFA5000
;01/12/01   nxb     Merged FFA5000.
;11/06/00   nxb     FEATURE_IRAM around export
;09/08/00   sr      Added call to IRAM test under FEATURE_VOC_IRAM_TEST
;06/16/00   jcw     Exported IRAM sysmbols
;04/28/99   ms/sk   Added function to clear sl register.
;                   Initialized sl properly in boot_error_handler, dload_entry
;                   and boot_powerdown_entry.
;04/08/99    sk     Used _cf with SPSR for ARM 2.5.
;03/25/99    ms     Added re-entrancy check in boot_abort_handler.
;03/24/99    ms     Fixed the exception vector table with bl instead of
;                   b for calling boot_default_isr. The link register is used
;                   for getting to the correct message, so it should be set.
;                   Defined a new message for re-entrant data abort.
;03/22/99    ms     Defined and exported symbol _main.
;03/17/99    ms     Incorporated changes for Virtual Boot Block.
;02/19/99    ms     Now running in Supervisor mode.
;                   'blx' renamed to 'blatox' for ARM SDT 2.5 compatibility.
;01/18/99    ms     Incorporated the macros save_exception_frame_ex and
;                   restore_exception_frame_ex from armasm.h directly into
;                   boot_abot_handler. 
;11/23/98   hcg     updated to build standalone bootblock. Added boot_delay.
;11/05/98   mdd     Reduced time for boot_ram_test by changing algorithm.
;11/03/98   mdd     Checked into SS archive -- no changes.  Version numbers
;                   restart, however.
;11/02/98   jct     Removed unneeded reference to bsp
;10/28/98   jkl     Changed Supervisor mode to System mode 
;09/20/98   hcg     Put boot_entry_table back in.
;09/14/98   hcg     Ported lite RAM test from 186 builds
;09/12/98   aj      Fixed boot_block_ver to spaces
;08/08/98   hcg     Exported boot functions for vectoring interrupts 
;                   through RAM.	
;07/29/98   hcg     Updated to allow stand-alone boot block compilation
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
#include "rexarm.h"
#include "armasm.h"
#include "target.h"
#ifdef T_MSM5000
  #include "msm50reg.h"
#else
  #include "msm3reg.h"
#endif
#include "boot_trap.h"
        
;============================================================================
;
;                             MODULE IMPORTS
;
;============================================================================

        ; Import functions referenced in the boot sequence.
        IMPORT  lcd_message

        IMPORT  boot_hw_powerdown
        IMPORT  boot_hw_ctrl_init
        IMPORT  boot_mem_ctrl_init
        IMPORT  boot_ram_init
        IMPORT  boot_rom_test
        IMPORT  boot_downloader
        IMPORT  boot_data_abort_handler
        IMPORT  Reset_Handler
#ifdef FEATURE_IRAM
        IMPORT  memtest_iram_test
#endif
        

#if defined(FEATURE_FFA5000_1900) || defined(FEATURE_FFA5000_800)
#error code not present
#endif // FEATURE_FFA5000

        ; Import the locations of the system stacks so the stack pointers can
        ; be initialized.

        IMPORT  svc_stack
        IMPORT  abort_stack
        IMPORT  undef_stack

        ; Import the linker generated symbols that correspond to the base
        ; addresses and sizes of the boot code data areas in both ROM and RAM.

        IMPORT |Load$$BB_RAM$$Base|
        IMPORT |Image$$BB_RAM$$Base|
        IMPORT |Image$$BB_RAM$$Length|
        IMPORT |Image$$BB_RAM$$ZI$$Base|
        IMPORT |Image$$BB_RAM$$ZI$$Length|
        
#ifdef FEATURE_IRAM
        IMPORT |Load$$IRAM$$Base|
        IMPORT |Image$$IRAM$$Base|
        IMPORT |Image$$IRAM$$Length|
#endif

;============================================================================
;
;                             MODULE EXPORTS
;
;============================================================================

        ; Export the renamed linker symbols for use by the other boot modules.

        EXPORT Load__BB_RAM__Base
        EXPORT Image__BB_RAM__Base
        EXPORT Image__BB_RAM__Length
        EXPORT Image__BB_RAM__ZI__Base
        EXPORT Image__BB_RAM__ZI__Length

#ifdef FEATURE_IRAM
        EXPORT Load__IRAM__Base
        EXPORT Image__IRAM__Base
        EXPORT Image__IRAM__Length
#endif

        EXPORT boot_reset_handler
        EXPORT boot_default_isr
        
        EXPORT boot_abort_handler
        EXPORT boot_block_ver
        EXPORT boot_powerdown_entry
        EXPORT boot_mob_model
        EXPORT dload_entry
        EXPORT boot_error_handler
	EXPORT boot_delay

	EXPORT chk_security_code_ptr
	EXPORT stamp_mob_model
	EXPORT stamp_md5_hash

; insert for MODEL_D1
		EXPORT	app_image_checksum

	
        ; Export the symbols __main and _main to prevent the linker from 
        ; including the standard runtime library and startup routine.

        EXPORT __main
        EXPORT _main

        IMPORT RAM_IRQ_VECTOR
        IMPORT RAM_FIQ_VECTOR

;============================================================================
;                      PROCESSOR INTERRUPT VECTORS
;
;  ARM processor interrupt vectors are located beginning at address 00000000H
;  which is the reset entry point.
;
;  All interrupts except for reset are vectored to the application exception
;  handlers.  The address of the FIQ interrupt handler is stored in the FIQ
;  mode r8 register which allows the FIQ handler to be assigned at runtime
;  and reached with a single branch instruction.
;============================================================================

        AREA    BOOTSYS_IVT, CODE, READONLY

        CODE32                         ; 32 bit ARM instruction set.
__main
_main
        ENTRY                          ; Entry point for boot image.

        b       boot_reset_handler     ; ARM reset
        bl      boot_default_isr       ; ARM undefined instruction interrupt
        bl      boot_default_isr       ; ARM software interrupt
        bl      boot_default_isr       ; ARM prefetch abort interrupt
        b       boot_abort_handler     ; ARM data abort interrupt
        bl      boot_default_isr       ; Reserved by ARM Ltd.
        b       RAM_IRQ_VECTOR         ; ARM IRQ interrupt
        b       RAM_FIQ_VECTOR         ; ARM FIQ interrupt


;============================================================================
;                  BOOT BLOCK INFORMATIONAL  SEGMENT
;
;============================================================================

; This is used by PST to locate this informational block
boot_info_symbol
        DCB     "QCOM"

;============================================================================
; The family number for this phone
; This number MUST be defined.
;============================================================================
boot_mob_model
     #if defined MODEL_NUMBER
        DCW     MODEL_NUMBER
     #else
        #error MODEL_NUMBER not Defined.
     #endif

;============================================================================
; Customer Mobile Model Number is held here.
;
; This variable is only a placeholder for a number which the customer can
;     use to differentiate various models in the same family of phones.
;
; If you choose not to define a separate CUST_MOB_MODEL,
;     then the MODEL_NUMBER will be used here.
; 
; Important : 
;    Exactly one DCW must appear in the final code at this location.
;    Otherwise, the next symbol, boot_block_ver, will move up in the memory
;    map and lead to problems.
;============================================================================
boot_cust_mob_model
     #if defined CUST_MOB_MODEL
        DCW     CUST_MOB_MODEL
     #elif defined MODEL_NUMBER
        DCW     MODEL_NUMBER
     #else
        #error MODEL_NUMBER not Defined.
     #endif

; This tells us what version of the informational block we have got
boot_block_ver
        ; This is the current version number
        DCW     1
        ; This is the minimum version number for backwards compatability
        DCW     1

;=========================================================================
;                       APPLICATION BOOT CHECK SEGMENT
; 
;  This segment reserves 32 bytes of space for the check values written in 
;  after the build has completed.  They are initialized to 0xFF so values 
;  can be written directly into the flash part, like the security code.
; 
;  This area must be located at the top of application ROM space.
;=========================================================================
     
stamp_mob_model        
#if defined MODEL_NUMBER
        DCW     MODEL_NUMBER           ; Space for the mobile model number
#else
#error MODEL_NUMBER not Defined.
#endif

stamp_md5_hash
        DCW     &FFFF
        DCD     &FFFFFFFF              ; and the MD5 hash value which is used 
        DCD     &FFFFFFFF              ; as a checksum across the boot image.
        DCD     &FFFFFFFF
        DCW     &FFFF
        
reserved
        DCW     &FFFF
        DCD     &FFFFFFFF
        
chk_security_code_ptr                  ; Space for the 64 bit security code.
                                       ; The label is exported so the security
        DCD     &F1E2D3C4
        DCD     &1234CDEF              ; code can be used for access control
                                       ; of the serial link.
; insert for MODEL_D1
app_image_checksum
		DCW		0x0000

;============================================================================
; BOOT ABORT HANDLER
;
;  DESCRIPTION - Called to handle all memory aborts. The only current cause is
;    mis-aligned data access. Here we put an exception_frame on the stack, call
;    into C to handle the abort, and then restore the exception frame to return.
;============================================================================
                
        AREA BOOTSYS_BOOT_CODE, CODE
        CODE32                  ; 32 bit ARM instruction set

 ENTRY_NODE boot_abort_handler

        sub     lr, lr, #8

        ; save exception frame.
        sub     sp, sp, #ARM_TRAP_FRAME_SIZE
        str     lr, [sp, #ARM_TRAP_FRAME_PC_OFFSET]   ;  Save supervisor mode lr as pc.
        mrs     lr, SPSR
        str     lr, [sp, #ARM_TRAP_FRAME_SPSR_OFFSET] ;  Save SPSR
        stmib   sp, {r0-r14}^                         ;  Return a pointer to the frame in a1.

        mov     a1, sp

#if defined(FEATURE_STACK_CHECK)
        pusha   sp, sl
        ldr     sl, =abort_stack
#endif

        ; We calculate where the stack pointer should be right now  so
        ; we can check to see if this is a re-entrant call.
        ldr     a4, =abort_stack+Abort_Stack_Size
        sub     a4, a4, #ARM_TRAP_FRAME_SIZE
#if defined FEATURE_STACK_CHECK
        sub     a4, a4, #4
#endif
        cmp     a4, sp
        ldrne   r0, =re_entrant_abort_msg
        bne     boot_error_handler

        ldr     a4, =boot_data_abort_handler
        blatox  a4                          ; Call C function

#if defined(FEATURE_STACK_CHECK)
        popa    sp, sl
#endif

        ; restore exception frame.
        ldr     r0, [sp, #ARM_TRAP_FRAME_SPSR_OFFSET]    ;  Reload  SPSR
        msr     SPSR_f, r0
        msr     SPSR_c, r0
        ldmib   sp, {r0-r14}^
        nop
        ldr     lr, [sp, #ARM_TRAP_FRAME_PC_OFFSET]      ;  Use pc as supervisor mode lr.
        add     sp, sp, #ARM_TRAP_FRAME_SIZE

        movs    pc, lr          ; return after restoring exception frame.

 ENTRY_NODE_END

;============================================================================
; BOOT_CLEAR_SL
;
;  DESCRIPTION -  
;     C prototype - void boot_clear_sl(void);
;     This function clears the SL register (R10).
;============================================================================
 LEAF_NODE boot_clear_sl
        eor     r10, r10, r10
 LEAF_NODE_END

;============================================================================
; ERROR FATAL
;
;  DESCRIPTION - Called when a fatal error occurs.  Function just spins on
;    until the watchdog timeout occurs, which will reset the system.
;============================================================================
 LEAF_NODE Error_Fatal
l1      b l1
 LEAF_NODE_END

;============================================================================
;
; BOOT RESET HANDLER
;
; DESCRIPTION
;
;   The boot sequence consists of the following steps:
;
;   * Initialize the stack pointer to be used by 'C'
;   * Latch the hardware power supply contacts if required
;   * Initialize the MSM memory and peripheral controller
;   * Test the target RAM hardware
;   * Verify the target application ROM image
;   * Transfer control to the target application if possible, otherwise
;     enter the boot kernel.
;
; FORMAL ARGUMENTS
;   None
;
; DEPENDENCIES
;   None
;
; RETURN VALUE
;   None
;
; SIDE EFFECTS
;   No RAM is used until the target RAM has been tested.
;
;   While performing the boot sequence operations, it is necessary to reset
;   the watchdog timer periodically in order to avoid a processor reset.
;
;============================================================================

boot_reset_handler
#if defined(FEATURE_FFA5000_1900) || defined(FEATURE_FFA5000_800)
#error code not present
#endif // FEATURE_FFA5000

        ; Initialize MSM memory and peripheral control hardware to enable
        ; access to the entire target ROM and RAM address space.

        ldr     r4, =boot_mem_ctrl_init
        blatox  r4

#if defined(FEATURE_FFA5000_1900) || defined(FEATURE_FFA5000_800)
#error code not present
#endif // FEATURE_FFA5000

        ; Initialize target specific hardware as required (e.g. latch power
        ; before PWR button is released).

        ldr     r4, =boot_hw_ctrl_init
        blatox  r4

        ; Verify proper functionality of the target RAM hardware. In the event
        ; of a failure, the RAM test function will return a non-zero value
        ; causing execution to transfer to the boot error handler which will
        ; display an error message on the LCD and then power down.

	ldr	r4, =boot_ram_test
	blatox  r4		; returns error message on failure,
        cmp     r0, #0		; or 0x0 if test passed.
        bne     boot_error_handler         

        ; Set up the system stacks for the ARM processor Supervisor Mode,
        ; Abort Mode and Undefined Instruction Mode.  The FIQ and IRQ
        ; interrupts are exported to the application which will set up
        ; the stacks for the corresponding processor modes as part of the
        ; application initialization.

#if defined(FEATURE_STACK_CHECK)
        eor     sl, sl, sl              // For stack limit checking.
#endif

        msr     CPSR_c, #PSR_Abort:OR:PSR_Fiq_Mask:OR:PSR_Irq_Mask
        ldr     r13, =abort_stack+Abort_Stack_Size

        msr     CPSR_c, #PSR_Undef:OR:PSR_Fiq_Mask:OR:PSR_Irq_Mask
        ldr     r13,=undef_stack+Undef_Stack_Size

        msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask:OR:PSR_Fiq_Mask       
        ldr     r13, =svc_stack+SVC_Stack_Size

#ifdef FEATURE_IRAM
#ifdef FEATURE_VOC_IRAM_TEST
        ; Test IRAM here because the call to boot_ram_init() next will
        ; initialize IRAM

        ldr     r4, =memtest_iram_test
        blatox  r4                         ; Returns pointer to error message,
        cmp     r0, #0                     ; or 0h if test passed.
        bne     boot_error_handler
#endif        
#endif

        ; Zero out the entire target RAM to satisfy the 'C' language
        ; requirement that all uninitialized variables be set to zero, and
        ; then copy the boot image initialized data to target RAM.

        ldr     r4, =boot_ram_init
        blatox  r4

#if defined(FEATURE_STACK_CHECK)
        ldr     sl, =svc_stack          // For stack checking.
#endif
        ; Verify that the target ROM contains a valid application image. In
        ; the event of a failure, the ROM test function will return a non-zero
        ; value causing execution to transfer to the boot error handler.

        ldr     r4, =boot_rom_test
        blatox  r4                         ; Returns pointer to error message,
        cmp     r0, #0                     ; or 0h if test passed.
        bne     boot_error_handler

        ; If the system boot sequence is successful, transfer control to the
        ; application reset handler and never return to this point since the
        ; target application will re-initialize all memory prior to beginning
        ; execution.  The application may transfer control back to the boot
        ; code via the boot export table.

        b       Reset_Handler

;============================================================================
; BOOT ERROR HANDLER
;
; DESCRIPTION
;   The boot error handler will is entered if either an error occurs during
;   the system boot sequence.  An LCD error message is displayed, followed
;   by a jump into the download sequence.  Note that pointers to error messages
;   will have already been placed in r0, and is sent as a parameter to the
;   LCD message driver.
;============================================================================

boot_error_handler

        ; This is temporary and should be removed once the LCD drivers have been
        ; re-written as either leaf nodes or in assembly.  Our LCD message
        ; routines currently need a stack, something which is not desirable.
        ; This call will be removed as part of the re-write.
        msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask:OR:PSR_Fiq_Mask
        ldr     r13, =svc_stack+SVC_Stack_Size

#if defined FEATURE_STACK_CHECK
        ldr     sl, =svc_stack
#endif

        ; Initialize the LCD and display the error message that is located at
        ; the address currently in register r0.
        ldr     r1, =lcd_message           ; Display LCD Message.
        blatox  r1
        
;----------------------------------------------------------------------------
; DLOAD ENTRY
;
; DESCRIPTION
;   The boot kernel is entered if an error condition has occurred, or
;   if the application code transfers control back to the boot block to
;   download a new application image.
;----------------------------------------------------------------------------

 LEAF_NODE dload_entry

        ; Restore supervisor mode stack for use by boot image in the event
        ; that the boot image has been entered from the application.  Also
        ; disable IRQ and FIQ interrupts in the event that the boot image
        ; has been entered from the application with interrupts enabled.

        msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask:OR:PSR_Fiq_Mask
        ldr     r13, =svc_stack+SVC_Stack_Size

#if defined FEATURE_STACK_CHECK
        ldr     sl, =svc_stack
#endif

        ; Enter the boot kernel, upon return from the boot kernel the only
        ; action left to take is to powerdown.

        ldr     r4, =boot_downloader
        blatox  r4

 LEAF_NODE_END

;----------------------------------------------------------------------------
; BOOT POWERDOWN ENTRY POINT
;
; DESCRIPTION
;   The boot powerdown routine is entered in the event of a return from the
;   boot kernel.
;----------------------------------------------------------------------------

 LEAF_NODE boot_powerdown_entry

        ; Disable IRQ and FIQ interrupts in the event that the boot image
        ; has been entered from the application with interrupts enabled.
        msr     CPSR_c, #PSR_Supervisor:OR:PSR_Irq_Mask:OR:PSR_Fiq_Mask

#if defined FEATURE_STACK_CHECK
        eor     sl, sl, sl
#endif

        ; Enter the boot powerdown routine.  There is no return from this
        ; routine.

        ldr     r4, =boot_hw_powerdown
        bx      r4
 LEAF_NODE_END

;============================================================================
;
; BOOT DEFAULT ISR
;
; DESCRIPTION
;   The default ISR prints an error message and then spins until power is
;   removed by the user.  The interrupt is identified by the return address
;   contained in the link register upon entry to this ISR.  The return address
;   is the address of the instruction following the one that was fetched from
;   the interrupt vector to service the interrupt.  Since there is no plan to
;   return from this ISR, the address of the instruction to return to after
;   completing the ISR is overwritten in the link register.
;
; FORMAL ARGUMENTS
;   None
;
; DEPENDENCIES
;   None
;
; RETURN VALUE
;   None
;
; SIDE EFFECTS
;   None
;
;============================================================================

boot_default_isr

        ; Get offset into interrupt vector table from link register (by using
        ; bl instruction in interrupt vector table) and use offset to select
        ; corresponding error message for display.

        ldr     r1, =isr_msg_table_base    ; Load message table base.

        ; Adjust lr by subtracting #8. This maps the Undef Instr message to
        ; to offset 0  and the reserved interrupt to offset 20 in the 
        ; message table.
        sub     r2, lr, #8
        ldr     r0, [r1, r2]

        ; Enter the boot error handler to display the error message.
        b       boot_error_handler         ; Will not return.

;----------------------------------------------------------------------------
;  BOOT ISR MESSAGE TABLE
;
;  The boot ISR message table contains the messages to be displayed on the
;  LCD in the event of an unexpected interrupt.
;----------------------------------------------------------------------------

isr_msg_table_base

        DCD     undef_inst_msg
        DCD     swi_msg
        DCD     pref_abort_msg
        DCD     data_abort_msg
        DCD     reserved_msg

undef_inst_msg
        DCB     " Undef Inst  Exception  ",0
;;;              123456789AB+123456789AB+
        
swi_msg
        DCB     "    SWI     Exception   ",0
;;;              123456789AB+123456789AB+
        
pref_abort_msg
        DCB     " Pref Abort  Exception  ",0
;;;              123456789AB+123456789AB+
        
data_abort_msg
        DCB     " Data Abort  Exception  ",0
;;;              123456789AB+123456789AB+
        
reserved_msg
        DCB     "  Reserved   Exception  ",0
;;;              123456789AB+123456789AB+
        
ram_error_msg
        DCB     "  RAM test    Failure   ",0
;;;              123456789AB+123456789AB+

re_entrant_abort_msg
        DCB     " Re-entrant  Data Abort ",0
;;;              123456789AB+123456789AB+

;============================================================================
;                       BOOT BLOCK DATA LOCATIONS
;
;  Locations and sizes of data areas in ROM and RAM are imported from the
;  linker and stored as data items that are used at runtime by the boot
;  kernel routines.
;============================================================================

        AREA    BOOTSYS_DATA, DATA, READONLY

        ; The $$ convention used by the linker is replaced to avoid the need
        ; for the -pcc option required by the ARM compiler when symbols that
        ; include $$ are used in 'C' code.

Load__BB_RAM__Base
        DCD |Load$$BB_RAM$$Base|

Image__BB_RAM__Base
        DCD |Image$$BB_RAM$$Base|

Image__BB_RAM__Length
        DCD |Image$$BB_RAM$$Length|

Image__BB_RAM__ZI__Base
        DCD |Image$$BB_RAM$$ZI$$Base|

Image__BB_RAM__ZI__Length
        DCD |Image$$BB_RAM$$ZI$$Length|

#ifdef FEATURE_IRAM
Load__IRAM__Base
        DCD |Load$$IRAM$$Base|

Image__IRAM__Base
        DCD |Image$$IRAM$$Base|

Image__IRAM__Length
        DCD |Image$$IRAM$$Length|
#endif

;============================================================================
;                      BOOT BLOCK COPYRIGHT SEGMENT
;
;  This segment contains the copyright notice which is contained in the
;  Boot Block.
;============================================================================
        AREA    BOOTSYS_COPYRIGHT, DATA, READONLY

        DCB     "Copyright (c) 1998"
        DCB     "by QUALCOMM, Incorporated."
        DCB     "All rights reserved."


;============================================================================
;			    BOOT_RAM_TEST
;
; NOTE: All variables are in registers to avoid using RAM. Also, no 
; instructions which use the stack are used.
;
; This function tests the RAM with a method which is designed to catch
; a variety of problems, including:
; 
;	data pins shorted high, low, or to each other
;	address pins shorted high, low, or to each other 
;	improper selection of byte/halfword/word on reading or writting
;	stuck/shorted chip selects 
;	disconnected address or data pins
; 
; This is accomplished by testing with a series of selected patterns
; (both for addresses and data) which are designed to bring such faults
; to light.  The basic algorithm is as follows:	
; 
; First, the data pins are checked.  The memory locations 0x0 and
; ram_test_offset are used (this must be added to ram_base to form the correct
; MSM3000 address).  A number is written to location 0x0 of the form
; 0x1 << n where n is a number from 0 to 15.  This is designed to test
; each individual pin.  After that number is written, a zero is written to
; ram_test_offset to clear the data pins and their buffers (in the case 
; where the pins are disconnected).  The number is read back, and checked
; for correctness.  If the data pins are shorted, stuck, or floating, an
; incorrect value will be fetched.
; 
; Next, address pins are tested.  Address of the form 0x1 << n where n is a
; number from 0 to some limit (defined by the size of RAM) are used.  This
; tests every address pin individually.  The data written to those locations 
; are the logical negation of the address (except for locations 0x0-0x2,
; which are too close together to store a 32 bit address).  Once all written,
; these memory locations are checked in turn for the correct values.  If the
; address pins are shorted, stuck, or floating, then two writes will select
; the same location, causing the read/test on one of them to fail.
; 
; The final test performed tests the selection of byte/word/halfword on reads
; and writes.  Storing individual bytes has allready been accomplished in the
; setup for the address pin test.  Therefore, we simply must read from 
; addresses 0x0-0x3 in bytes and halfwords to verify the correct results.
; 
; NOTE:	The constants coded here are designed for a little-endian processor.
; They will have to be changed for a big-endian processor.
; 
; In the event a test fails, the address of the error string is placed in r0.
; In the event all the tests pass, the value 0x0 is loaded into r0.  In
; either case, program execution returns to the calling function.
; 
;============================================================================
        AREA    BOOT_RAM_TEST, CODE, READONLY 

        CODE32

	;; Needed Constants
	;; ----------------
	;; These need to be in the flash for us to use since they are too
	;; big to be encoded into the instruction 
	;;(or could grow to be too big)
ram_base
	DCD	RAM_BASE
ram_size
	DCD	RAM_SIZE
ram_test_offset
	DCD	0x10

val_0055aa0f
	DCD	0x0055AA0F
val_aa0f
	DCD	0xAA0F
val_0055
	DCD	0x0055
	
	; watchdog register
watch_dog                   
        DCD     SLEEP_CTL_WB

 ENTRY_NODE boot_ram_test

	;; Check for data bus failures
	;; ---------------------------

	;; loop over all the data pins, testing each one
	ldr	r0, =ram_base	; the base address of RAM
	ldr	r0, [r0]
	mov	r1, #1		; the test value (a single 1 bit)
	mov	r2, #0		; a zero (used to clear data pins)
	ldr	r3, =ram_test_offset
	ldr     r3, [r3]	; a RAM location to test with
	add	r4, r3, r0	;   offset by the base addr of RAM 

data_test_loop
	str	r1, [r0]	; store the test value 
	str	r2, [r4]	; store a zero to clear data pins
	ldr	r3, [r0]	; retrieve the test value
	cmp	r3, r1		; check test value for correctness
	bne	ram_error	;   if incorrect, go to error routine
	
	mov	r1, r1, LSL #1	; shift test bit left one position
	cmp	r1, #0x8000	; check that we are still in the test range
	ble	data_test_loop	;   if we are, continue tests
	
	;; Kick the watchdog timer
	;; -----------------------
        mov     r2, #SLEEP_CTL_WB__WATCH_DOG_MASK  
	ldr     r3, =watch_dog        ; get offset for SLEEP_CTL
        ldr     r3, [r3]              
        strb    r2, [r3]              ; write to port
        mov     r2, #0                ; clear out R2
        strb    r2, [r3]              ; write to port

	;; Check for address bus failures
	;; ------------------------------

	;; first we store data into RAM -- later we will test it all
	;; treat addresses 0x0-0x2 specially
	;; at this point, r0 = base ram address
	mov	r1, #0x0F	; store 0x0f in location 0
	strb	r1, [r0]
	mov	r1, #0xAA	; store 0xaa in location 1
	strb	r1, [r0, #1]
	mov	r1, #0x55	; store 0x55 in location 2
	strb	r1, [r0, #2]
	
	;; store the highest address in the last location in ram
	ldr	r1, =ram_size	; fetch the size of ram
	ldr	r1, [r1]
	add	r1, r1, r0	; calculate where the last location is
	str	r1, [r1, #-3]	; store the last ram address there

	;; loop over all the address pins, storing with each one
	;; at this point, r0 = base ram address and r1 = last ram address
	mov	r2, #0x4	; start with testing the 3rd pin
	ldr	r3, =ram_size	; fetch the size of ram
	ldr	r3, [r3]

address_test_write_loop
	mvn	r4, r2		; r4 = bit inversion of address
	str	r4, [r2, r0]	; store the data into the test location
	mov	r2, r2, LSL #1	; shift test bit left one position
	cmp	r2, r3		; check that we are in the test range
	blt	address_test_write_loop	;   if we are, continue tests
				
	;; start checking the stored data for correctness
	;; treat addresses 0x0-0x2 specially
	;; at this point, 
	;;	r0 = base ram address 
	;; 	r1 = last ram address
	;;	r3 = size of ram
	ldrb	r2, [r0]	; compare location 0 to 0x0F
	cmp	r2, #0x0F	
	bne	ram_error	; if they do not match, error
	ldrb	r2, [r0, #1]	; compare location 1 to 0xAA
	cmp	r2, #0xAA	
	bne	ram_error	; if they do not match, error
	ldrb	r2, [r0, #2]	; compare location 2 to 0x55
	cmp	r2, #0x55	
	bne	ram_error	; if they do not match, error

	;; now we loop over our previously stored data and check it all
	;; at this point, 
	;;	r0 = base ram address 
	;; 	r1 = last ram address
	;;	r3 = size of ram
	mov	r2, #0x4	; start with testing the 3rd pin

address_test_read_loop
	mvn	r4, r2		; r4 = bit inversion of address
	ldr	r5, [r2, r0]	; store the data into the test location
	cmp	r5, r4		; compare retrieved data with correct data
	bne	ram_error	; if they do not match, error
		
	mov	r2, r2, LSL #1	; shift test bit left one position
	cmp	r2, r3			; check that we are in the test range
	blt	address_test_read_loop	;   if we are, continue tests
	
	;; now we check the last address
	ldr	r2, [r1, #-3]	; load from the last address
	cmp	r2, r1		; if the value there != the last address
	bne	ram_error	;   the we have an error
	
	;; Check for byte/word/halfword access failures
	;; --------------------------------------------

	;; at this point, 
	;;	r0 = base ram address 
	;; 	r1 = last ram address
	;;	r3 = size of ram

	;; store a 0 in the 3rd byte of RAM (so we know what the 
	;;  entire first word should be
	mov	r3, #0
	strb	r3, [r0, #3]

	;; check that the word at 0x0 is 0x0FAA5500
	ldr	r2, =val_0055aa0f
	ldr	r2, [r2]
	ldr	r4, [r0]
	cmp	r2, r4
	bne	ram_error

	;; check that the halfword at 0x0 is 0xAA0f
	ldr	r2, =val_aa0f
	ldr	r2, [r2]
	ldrh	r4, [r0]
	cmp	r2, r4
	bne	ram_error

	;; check that the halfword at 0x2 is 0x0055
	ldr	r2, =val_0055
	ldr	r2, [r2]
	ldrh	r4, [r0, #2]
	cmp	r2, r4
	bne	ram_error

	;; check that the byte at 0x1 is 0xAA
	mov	r2, #0xAA
	ldrb	r4, [r0, #1]
	cmp	r2, r4
	bne	ram_error

	; nothing more to do -- we must have passed all tests
        mov   r0, #0                 ; no errors - return 0
        mov   pc, r14                ; continue with boot sequence
			
ram_error
        ldr   r0, =ram_error_msg     ; load up RAM error message
        mov   pc, r14                ; jump to error handler

 ENTRY_NODE_END


;============================================================================
; BOOT DELAY
;
;  DESCRIPTION - Delays the system for the given number of microseconds
;
;============================================================================
        CODE16                             ; 32 bit ARM instruction set.
               
 ENTRY_NODE boot_delay
 
boot_delay_loop
        orr     r1, r1
        orr     r1, r1
        orr     r1, r1
        sub     r0, r0, #1
        bgt     boot_delay_loop
        mov     pc, lr

 ENTRY_NODE_END

        END
