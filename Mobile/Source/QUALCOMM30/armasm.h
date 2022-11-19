#ifndef ARMASM_H
#define ARMASM_H
/*=============================================================================
  
                        ARM Assembly Language Definitions 
  
GENERAL DESCRIPTION
  This file contains assembly language macros for use with the ARM assembler.
 
Copyright (c) 1998,1999,2000 by QUALCOMM, Incorporated.  All Rights Reserved.
=============================================================================*/


/*=============================================================================
 
                            EDIT HISTORY FOR FILE
 
$Header:   L:/src/asw/MSM5000/VCS/armasm.h_v   1.3   20 Jun 2000 16:49:38   cweaver  $
    
when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/20/00   jcw     Added support for Interworking
02/02/00   jct     Added define for system stack size
04/30/99    ms     Increased abort stack size to 200 words.
04/02/99    ms     Reduced stack sizes.
02/03/99    ms     Renamed blx to blatox since blx is a keyword in Assembler
                   of ARM SDK 2.5
01/18/99    ms     Moved ARM_TRAP_FRAME related declarations to boot_trap.h.
                   Moved context frame related macros to rexarm.s.
                   Incorporated exception frame related macros directly into
                   boot_abort_handler in bootsys.s.
12/10/98   jkl     Included r12 in the context frame.
11/23/98   jkl     Clean up code. Changed save_context_from_task macro.
10/27/98   jkl     Revised for new context
09/13/98   hcg     Changed exception/trap frame names
07/10/98   jct     Revised for coding standard, removed unused code
01/01/98   bwj     Created
=============================================================================*/

#ifdef TG
   #include "target.h"
#endif
                              
/*  CPSR Control Masks         */
#define PSR_Thumb_Mask       0x20
#define PSR_Fiq_Mask         0x40
#define PSR_Irq_Mask         0x80
#define PSR_Mode_Mask        0x1f

/*  Processor mode definitions */
#define PSR_User             0x10
#define PSR_Fiq              0x11
#define PSR_Irq              0x12
#define PSR_Supervisor       0x13
#define PSR_Abort            0x17
#define PSR_Undef            0x1b
#define PSR_System           0x1f
#define PSR_Thumb            0x20

/*  Stack sizes.               */ 
#define SVC_Stack_Size       0xd0    /* => 104 words */
#define IRQ_Stack_Size       0x218   /* => 268 words */
#define FIQ_Stack_Size       0xb0    /* =>  88 words */
#define Abort_Stack_Size     0x190   /* => 200 words */
#define Undef_Stack_Size     0x100   /* => 128 words */
#define Idle_Stack_Size      0x100
#define Sys_Stack_Size       0x400


/*===========================================================================
**  GENERAL PURPOSE ASSEMBLY MACROS.
**=========================================================================*/

#if defined(_ARM_ASM_)

        GBLS    current_node_name
        GBLS    current_node_type
        GBLA    current_node_aregcount
        GBLA    current_node_vregcount

/*===========================================================================
 
Name: pusha, popa
 
Description: Readable names for push and pop. 
  
MODE: Any
STATE: ARM 
  
Registers modified: sp
 
=============================================================================*/

        MACRO   
        pusha   $stack, $reg
        str     $reg, [$stack, #-4]!
        MEND

        MACRO   
        popa    $stack, $reg
        ldr     $reg, [$stack], #4
        MEND

/*===========================================================================
 
Name: Entry_Node, Entry_Node_End
 
Description: Defines an empty function prolog and epilog of a piece of assembly code. 
             These macros take care of the following:
               - Exporting the entry point label
               - Creating a stack frame
               - Software stack checking 
 
Defined Labels:
   "node_name" - The main thumb mode entry point of the function.
   "node_name"_end - A label marking the begining of the function epilog.
 
Arguments: 
  Leaf_Node
  Node_Name - The function name of the function to be defined.
  Leaf_Node_End
  None
        
MODE: Any
STATE: ARM 
 
Registers modified: None
 
=============================================================================*/

        MACRO
        ENTRY_NODE $node_name
current_node_name       SETS    "$node_name"
        EXPORT  $node_name
        ALIGN
        ROUT
$node_name
        MEND
                                        ;  The end of an entry node.
        MACRO
        ENTRY_NODE_END
$current_node_name._exit
current_node_name       SETS    ""
        MEND

/*===========================================================================
 
  Name: Leaf_Node, Leaf_Node_End
 
  Description: Defines the function prolog and epilog of a function that makes 
               no function calls. These macros take care of the following:
                 - Exporting the entry point label
                 - Creating 16 and 32 bit entry points.
                 - Creating a stack frame
                 - Software stack checking 
 
  Defined Labels:
     "node_name" - The main thumb mode entry point of the function.
     "node_name"_32 - The ARM state entry point to the function.
     "node_name"_end - A label marking the begining of the function epilog.
 
  Arguments: 
    Leaf_Node
     Node_Name - The function name of the function to be defined.
    Leaf_Node_End
     None
        
  MODE: Any
  STATE: ARM and Thumb state entry points. Code runs in ARM state.
 
  Registers modified: None
 
=============================================================================*/
        MACRO
        LEAF_NODE $node_name
current_node_type       SETS    "Leaf"
        CODE16
        ENTRY_NODE $node_name
        bx      pc
        ALIGN
        CODE32
#ifndef __APCS_INTERWORK
        orr     lr, lr, #0x01
#endif
        EXPORT  $node_name._32
$node_name._32
        MEND

        MACRO
        LEAF_NODE_END $node_name
        ASSERT  "$current_node_type" = "Leaf"
        ENTRY_NODE_END $node_name
        bx     lr
current_node_type       SETS    ""
        MEND


/*===========================================================================
 
  Name: Leaf_Node_End, Leaf_Node_End_16
 
  Description: Defines the function prolog and epilog of a Thumb mode only function that makes
               no function calls. These macros take care of the following:
                 - Exporting the entry point label
                 - Creating a 16 bit entry point.
                 - Creating a stack frame
                 - Software stack checking 
 
  Defined Labels:
     "node_name" - The main thumb mode entry point of the function.
     "node_name"_end - A label marking the begining of the function epilog.
 
  Arguments: 
    Leaf_Node_16
     Node_Name - The function name of the function to be defined.
    Leaf_Node_End_16
     None
        
  MODE: Any
  STATE: Thumb
 
  Registers modified: None
 
=============================================================================*/

        MACRO
        LEAF_NODE_16 $node_name
current_node_type       SETS    "Leaf16"
        CODE16
        ENTRY_NODE $node_name
        MEND

        MACRO
        LEAF_NODE_END_16 $node_name
        ASSERT  "$current_node_type" = "Leaf16"
        ENTRY_NODE_END $node_name
        bx     lr
current_node_type       SETS    ""
        MEND


        MACRO
        ALTERNATE_ENTRY_16 $node_name
        ASSERT  "$current_node_type" = "Leaf16"
        EXPORT $node_name
$node_name
        MEND


/*===========================================================================
 
  Name: blatox
 
  Description: Calls a function from ARM state without having to know whether that 
     that function is ARM or Thumb state code.
 
  Arguments: 
    destreg - The register that contains the address of the function to be called.
 
  Registers modified: lr
        
  MODE: Any
  STATE: ARM. Can call either ARM or Thumb state functions.
 
=============================================================================*/

        MACRO
        blatox     $destreg
        ROUT

        tst     $destreg, #0x01         /* Test for thumb mode call.  */

        ldrne   lr, =%1
        ldreq   lr, =%2
        bx      $destreg
1
        CODE16
        bx      pc
        ALIGN
        CODE32
2
        MEND

#endif   /* ARM_ASM  */

#endif   /* ARMASM_H */
