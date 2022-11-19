#ifndef BOOT_TRAP_H
#define BOOT_TRAP_H
/*==========================================================================

                        Boot Trap Handlers Header File
                 
DESCRIPTION
  Routines to intercept trap handlers from the processor
  
  Details ARM instructions so they can be interpreted by the data abort
  handler.  These formats are detailed in the ARM documentation and in
  The ARM ARM.

Copyright (c) 1997,1998,1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/boot_trap.h_v   1.1   10 Sep 1999 18:06:20   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/22/99    ms     Included comdef.h to make the dependency explicit.
01/18/99    ms     Moved ARM_TRAP_FRAME related macros from armasm.h.
                   Moved the related #defines from rexarm.h.
09/13/98   hcg     Updated for MSM3000 archive.
08/04/98   hcg     Multiple bug fixes from original ARM port
07/26/98   hcg     Moved rex_trap.c to boot_trap.c
07/10/98   jct     Revised for coding standard, removed unused code
01/01/98   bwj     Created

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/



/*===========================================================================
 
                           DATA DECLARATIONS

===========================================================================*/

/* -----------------------------------------------------------------------
** Offsets in ARM_TRAP_FRAME. These are used in assembly files.
** ----------------------------------------------------------------------- */

#define ARM_TRAP_FRAME_SIZE             0x44
#define ARM_TRAP_FRAME_SPSR_OFFSET      0x0
#define ARM_TRAP_FRAME_A_OFFSET         0x4
#define ARM_TRAP_FRAME_V_OFFSET         0x14
#define ARM_TRAP_FRAME_FP_OFFSET        0x30
#define ARM_TRAP_FRAME_IP_OFFSET        0x34
#define ARM_TRAP_FRAME_SP_OFFSET        0x38
#define ARM_TRAP_FRAME_LR_OFFSET        0x3c
#define ARM_TRAP_FRAME_PC_OFFSET        0x40

#if !defined(_ARM_ASM_)

#include "comdef.h"

typedef void (*PFUNCTION)(void);
typedef union { word w; void *addr; PFUNCTION func; } ArmRegister;

typedef struct {
  ArmRegister Spsr;
  ArmRegister a[4];
  ArmRegister v[7];
  struct _StackFrame *fp;             /*  (r11) Saved Frame pointer */
  ArmRegister ip;                     /*  (r12) Saved ip            */
  ArmRegister sp;                     /*  (r13) Saved stack pointer */
  ArmRegister lr;                     /*  (r14) Saved Link Register */
  ArmRegister pc_not_used;            /*  (r15) Saved PC            */
} ARM_EXCEPTION_FRAME_EX, *PARM_EXCEPTION_FRAME_EX;

typedef union {
  ARM_EXCEPTION_FRAME_EX f;
  struct {
    dword Spsr;
    dword r[16];
  } l;
} ARM_TRAP_FRAME, *PARM_TRAP_FRAME;

typedef union {
#define THUMB_INST_MASK_LD_LITERAL 0xf800
#define THUMB_INST_VAL_LD_LITERAL  0x4800
  struct {
    int offset:8;
    int rd:3;
    int opcode:5;
  } ld_literal;
  
#define THUMB_INST_MASK_LD_W_B_REG 0xf200
#define THUMB_INST_VAL_LD_W_B_REG  0x5000
  struct {
    int rd:3;
    int rn:3;
    int rm:3;
    int opcode2:1;
    int b:1;
    int l:1;
    int opcode:4;
  } ld_w_b_reg;
  
#define THUMB_INST_MASK_LD_S_B_H_REG 0xf200
#define THUMB_INST_VAL_LD_S_B_H_REG  0x5200
  struct {
    int rd:3;
    int rn:3;
    int rm:3;
    int opcode2:1;
    int l:1;
    int h:1;
    int opcode:4;
  } ld_s_b_h_reg;
  
#define THUMB_INST_MASK_LD_W_B_IMMED 0xe000
#define THUMB_INST_VAL_LD_W_B_IMMED  0x6000
  struct {
    int rd:3;
    int rn:3;
    int immed:5;
    int l:1;
    int b:1;
    int opcode:3;
  } ld_w_b_immed;
  
#define THUMB_INST_MASK_LD_H_IMMED 0xf000
#define THUMB_INST_VAL_LD_H_IMMED  0x8000
  struct {
    int rd:3;
    int rn:3;
    int immed:5;
    int l:1;
    int opcode:4;
  } ld_h_immed;
  
#define THUMB_INST_MASK_LD_STACK 0xf000
#define THUMB_INST_VAL_LD_STACK  0x9000
  struct {
    int offset:8;
    int rd:3;
    int l:1;
    int opcode:4;
  } ld_stack;

#define THUMB_INST_MASK_PUSH_POP_STACK 0xf000
#define THUMB_INST_VAL_PUSH_POP_STACK  0x9000
  struct {
    int rlist:8;
    int r:1;
    int sbz:1;
    int opcode2:1;
    int l:1;
    int opcode:4;
  } push_pop_stack;
  
#define THUMB_INST_MASK_LD_MULTIPLE 0xf000
#define THUMB_INST_VAL_LD_MULTIPLE  0xc000
  struct {
    int rlist:8;
    int rn:3;
    int l:1;
    int opcode:4;
  } ld_multiple;

  int l;
} ThumbInst;

typedef union {

  struct {
    int cond:4;
    int opcode:3;
    int operation:4;
    int s:1;
    int rn:4;
    int rd:4;
    int rotate:4;
    int immediate:8;
  } dp_immed;

  struct {
    int cond:4;
    int opcode:3;
    int operation:4;
    int s:1;
    int rn:4;
    int rd:4;
    int shift_immed:5;
    int shift:2;
    int opcode2:1;
    int rm:4;
  } dp_immed_shift;

  struct {
    int cond:4;
    int opcode:3;
    int operation:4;
    int s:1;
    int rn:4;
    int rd:4;
    int rs:4;
    int opcode2:1;
    int shift:2;
    int rm:4;
  } dp_reg_shift;

  struct {
    int cond:4;
    int opcode:6;
    int a:1;
    int s:1;
    int rd:4;
    int rn:4;
    int rs:4;
    int opcode2:4;
    int rm:4;
  } multiply;

  struct {
    int cond:4;
    int opcode:5;
    int u:1;
    int a:1;
    int s:1;
    int rdhi:4;
    int rdlo:4;
    int rs:4;
    int opcode2:4;
    int rm:4;
  } multiply_long;

  struct {
    int cond:4;
    int opcode:5;
    int r:1;
    int opcode2:2;
    int mask:4;
    int sbo:4;
    int rotate:4;
    int immediate:8;
  } msr_immediate;

  struct {
    int cond:4;
    int opcode:5;
    int r:1;
    int opcode2:2;
    int mask:4;
    int sbo:4;
    int sbz:7;
    int opcode3:1;
    int rm:4;
  } msr;

  struct {
    int cond:4;
    int opcode:5;
    int r:1;
    int opcode2:2;
    int mask:4;
    int sbo:4;
    int sbz:12;
  } mrs;

  struct {
    int cond:4;
    int opcode:7;
    int sbo:4;
    int sbo2:4;
    int sbo3:4;
    int rn:4;
    int opcode2:4;
    int rm:4;
  } bx;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int b:1;
    int w:1;
    int l:1;
    int rn:4;
    int rd:4;
    int immediate:12;
  } ld_st_immediate_offset;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int b:1;
    int w:1;
    int l:1;
    int rn:4;
    int rd:4;
    int shifted_immediate:6;
    int shift:3;
    int opcode2:1;
    int rm:4;
  } ld_st_register_offset;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int opcode2:1;
    int w:1;
    int l:1;
    int rn:4;
    int rd:4;
    int hi_offset:4;
    int opcode3:1;
    int s:1;
    int h:1;
    int opcode4:1;
    int lo_offset:4;
  } ld_st_half_byte;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int opcode2:1;
    int w:1;
    int l:1;
    int rn:4;
    int rd:4;
    int sbz:4;
    int opcode3:1;
    int s:1;
    int h:1;
    int opcode4:1;
    int rm:4;
  } ld_st_half_byte_2;

  struct {
    int cond:4;
    int opcode:5;
    int b:1;
    int opcode2:2;
    int rn:4;
    int rd:4;
    int sbz:4;
    int opcode3:4;
    int rm:4;
  } swap;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int s:1;
    int w:1;
    int l:1;
    int rn:4;
    int rlist:16;
  } ld_st_multiple;

  struct {
    int cond:4;
    int opcode:4;
    int op:4;
    int crn:4;
    int crd:4;
    int cp_num:4;
    int op2:3;
    int opcode2:1;
    int crm:4;
  } coprocessor_data;

  struct {
    int cond:4;
    int opcode:4;
    int op:3;
    int l:1;
    int crn:4;
    int rd:4;
    int cp_num:4;
    int op2:3;
    int opcode2:1;
    int crm:4;
  } coprocessor_register;

  struct {
    int cond:4;
    int opcode:3;
    int p:1;
    int u:1;
    int n:1;
    int w:1;
    int l:1;

    int rn:4;
    int crd:4;
    int cp_num:4;
    int offset:8;
  } coprocessor_ld_st;

  struct {
    int cond:4;
    int opcode:3;
    int l:1;
    int offset:24;
  } b_bl;

  struct {
    int cond:4;
    int opcode:4;
    int swi_num:24;
  } swi;

  struct {
    int cond:4;
    int opcode:3;
    int op:20;
    int opcode2:1;
    int op2:4;
  } undef;

  dword l;

} ArmInstr;


/* -----------------------------------------------------------------------
** ARM Instruction Values
** ----------------------------------------------------------------------- */
#define ARM_INST_VAL_LD_ST_IMMEDIATE_OFFSET 0x04000000
#define ARM_INST_VAL_LD_ST_REGISTER_OFFSET  0x05000000
#define ARM_INST_VAL_LD_ST_HALF_BYTE        0x00400090
#define ARM_INST_VAL_LD_ST_HALF_BYTE_2      0x00000090
#define ARM_INST_VAL_SWAP                   0x01000090
#define ARM_INST_VAL_LD_ST_MULTIPLE         0x08000000

/* -----------------------------------------------------------------------
** ARM Instruction Masks
** ----------------------------------------------------------------------- */
#define ARM_INST_MASK_LD_ST_IMMEDIATE_OFFSET 0x0e000000
#define ARM_INST_MASK_LD_ST_REGISTER_OFFSET  0x0e000010
#define ARM_INST_MASK_LD_ST_HALF_BYTE        0x0e400050
#define ARM_INST_MASK_LD_ST_HALF_BYTE_2      0x0e400050
#define ARM_INST_MASK_SWAP                   0x0fb000f0
#define ARM_INST_MASK_LD_ST_MULTIPLE         0x0e000000

#endif /* _ARM_ASM_ */

#endif /* END BOOT_TRAP_H */
