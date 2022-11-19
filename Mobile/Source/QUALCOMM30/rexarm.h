#ifndef REXARM_H
#define REXARM_H
/*==========================================================================

                     REX ARM Definitions Header File

DESCRIPTION
  Definitions for offsets into various REX and ARM data structures.  This
  is reused by REX ARM assembly files
  
Copyright (c) 1997-1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rexarm.h_v   1.2   18 Aug 2000 14:32:46   bcalder  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/18/00   bgc     Added r8-r11 to the task context for task switches.
02/19/99    ms     Redefined some offsets in REX_INTERRUPT_FRAME to match with
                   the new context frame. 
02/12/99    ms     Added REX_TCB_LED_OFFSET definition for TIMETEST.
01/18/99    ms     Moved ARM_TRAP_FRAME related #defines to boot_trap.h
12/16/98   jct     Changed this file from genarm.h to rexarm.h as there was
                   no general purpose code in this any longer
12/10/98   jkl     Included r12 in the context frame.
11/02/98   ms      Revised for new context.
09/13/98   hcg     Renamed trap frames.
07/10/98   jct     Revised for coding standard, removed unused code
01/01/98   bwj     Created

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/


/*===========================================================================
 
                             DEFINITIONS

===========================================================================*/

/* Offsets in rex_tcb_type
*/
#define REX_TCB_SIZE                    0x18
#define REX_TCB_STACK_POINTER_OFFSET    0x00
#define REX_TCB_STACK_LIMIT_OFFSET      0x04
#define REX_TCB_SLICES_OFFSET           0x08
#define REX_TCB_SIGS_OFFSET             0x0C
#define REX_TCB_WAIT_OFFSET             0x10
#define REX_TCB_PRI_OFFSET              0x14
#define REX_TCB_LED_OFFSET              0x18


/* Offsets in rex_timer_type
*/
#define REX_TIMER_SIZE                  0x24
#define REX_TIMER_CNT_OFFSET            0x10
#define REX_TIMER_TCB_PTR_OFFSET        0x18
#define REX_TIMER_SIGS_OFFSET           0x1C

/* Offsets in REX_STACK_FRAME
*/
#define REX_STACK_FRAME_SIZE            0x8
#define REX_STACK_FRAME_LR_OFFSET       0x0
#define REX_STACK_FRAME_SP_OFFSET       0x4

/* Offsets in REX_INTERRUPT_FRAME
*/
#define REX_INTERRUPT_FRAME_SIZE        0x40
#define REX_INTERRUPT_FRAME_SPSR_OFFSET 0x00
#define REX_INTERRUPT_FRAME_R_OFFSET    0x04
#define REX_INTERRUPT_FRAME_LR_OFFSET   0x38       
#define REX_INTERRUPT_FRAME_PC_OFFSET   0x3c


#endif /* END REXARM_H */
