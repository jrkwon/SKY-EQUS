#ifndef DEINT_H
#define DEINT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         M O B I L E   CHANNEL DEINTERLEAVER    S E R V I C E S

GENERAL DESCRIPTION
  Library of routines to initialize, and operate on, the Qualcomm CDMA
 Channel Deinterleaver chip

============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/deint.h_v   1.13   23 Oct 2000 11:26:06   rmalhotr  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/23/00   rm       Seperate loading of deint and decoder parameters
                    for Turbo Encoded Frames.
07/12/00   rm       Added support for Multiframe interleaving.
05/17/00   rm       Added deint_create_dummy_sch_task_list().
05/10/00   rm       Added rc parameter in deint_create_sch_task_list fn.
04/21/00   hrk      Added DEINT_RC_INVALID entry to deint_rc_type.
03/23/00   bgc      Changed yamamoto threshholds for RC1 and RC2 to 
                    [1, 1, 3, 3] for [full, half, quarter, eighth].  These
                    are the same thresholds as MSM3000 for RC1 and RC2
02/16/00   eh       Updated LTU CRC LEN for MSM5000B.
01/28/00   rm       Added in support for 1X SCH (RC3, RC4 & RC5).
01/24/00   rm       Added in support to sequence read/write parameters. The
                    write parameters are programmed one frame ahed of the read
                    parameters. This will allow us clean transiton if SCH rates
                    change or a change in RC, without loss of a frame.
11/15/99   rm       Added in support for RC5 SCH (convolutional codes).
11/09/99   rm       Added in support for RC3,RC4 & RC5 SCH (TURBO DECODER).
10/17/99   rm       Added in support for IS95B Supplemental Code Channel.
09/24/99   rm       Added in support for SCH (Rc3 & RC4).
08/11/99   rm       Added support for RC4 and RC5 for F-FCH.
07/31/99   rm       Created file. Interfaces to support channel deinterleaver.

============================================================================*/

#include "comdef.h"
#include "dec5000.h"
#include "queue.h"

#define DINT_TASK_LIST_WH HW_DINT_TASK_LIST


/*  Packet Values for  Packet A, B, C and D */
#define DINT_PKT_A_V (0x00 << 0x03)
#define DINT_PKT_B_V (0x01 << 0x03)
#define DINT_PKT_C_V (0x02 << 0x03)
#define DINT_PKT_D_V (0x03 << 0x03)
#define DINT_PKT_E_V (0x04 << 0x03)

#define DINT_TASK_LIST_PACKET_A_V  (0x00 << 0x04) 
#define DINT_TASK_LIST_PACKET_B_V  (0x01 << 0x04) 
#define DINT_TASK_LIST_PACKET_C_V  (0x02 << 0x04) 
#define DINT_TASK_LIST_PACKET_D_V  (0x03 << 0x04) 
#define DINT_TASK_LIST_PACKET_E_V  (0x04 << 0x04) 


#define DINT_PKT_INTLV_CFG_LO_V 0x00
#define DINT_PKT_INTLV_CFG_HI_V 0x01
#define DINT_PKT_BLK_SIZE_LO_V 0x02
#define DINT_PKT_BLK_SIZE_HI_V 0x03
#define DINT_PKT_PUNCT_LO_V 0x04
#define DINT_PKT_PUNCT_HI_V 0x05
#define DINT_PKT_PL_CRC_LO_V 0x06
#define DINT_PKT_PL_CRC_HI_V 0x07


#define DINT_SYNC_CHAN_80M_BND_V 0x40
#define DINT_NEXT_20M_BND_V 0x20

#define DINT_LRAM_CHAN_ALLOC_MASK 0xC0
#define DINT_MRAM_CHAN_ALLOC_MASK 0x30
#define DINT_NRAM_CHAN_ALLOC_MASK 0x0C
#define DINT_PRAM_CHAN_ALLOC_MASK 0x03


#define DINT_CH0_RAM_L_V  0x00
#define DINT_CH0_RAM_M_V  0x00
#define DINT_CH0_RAM_N_V  0x00
#define DINT_CH0_RAM_P_V  0x00

#define DINT_CH1_RAM_L_V  0x40
#define DINT_CH1_RAM_M_V  0x10
#define DINT_CH1_RAM_N_V  0x04
#define DINT_CH1_RAM_P_V  0x01

#define DINT_CH2_RAM_L_V  0x80
#define DINT_CH2_RAM_M_V  0x20
#define DINT_CH2_RAM_N_V  0x08
#define DINT_CH2_RAM_P_V  0x02


#define DINT_TASK_LIST_ACTIVE_TASK_V  (0x01 << 15)
#define DINT_TASK_LIST_SUB_CHANS_EN_V (0x01 << 14)
#define DINT_TASK_LIST_SYNC_CHAN_EN_V (0x01 << 13)
#define DINT_TASK_LIST_DEC_INTR_EN_V  (0x01  << 12)
#define DINT_TASK_LIST_CH0_V       (0x00 << 10)
#define DINT_TASK_LIST_CH1_V       (0x01 << 10)
#define DINT_TASK_LIST_CH2_V       (0x02 << 10)
#define DINT_TASK_LIST_FRM_HYP_20MS_V (0x02 << 8)
#define DINT_TASK_LIST_FRM_HYP_5MS_V (0x00 << 8)

#define DINT_FIRST_5MSM_SLOT_ADDR 0x0000
#define DINT_SECOND_5MSM_SLOT_ADDR 0x0020
#define DINT_THIRD_5MSM_SLOT_ADDR 0x0040
#define DINT_FOURTH_5MSM_SLOT_ADDR 0x0060


#define DEINT_CLK_SEL_CHIPX8 (0x0000 << 0x08)
#define DEINT_CLK_SEL_23TCXO (0x0001 << 0x08)
#define DEINT_CLK_SEL_TCXO (0x0004 << 0x08)




#define DEINT_FCH_RC_BASE 0x01
typedef enum
{
  DEINT_RC_INVALID=0x0,
  DEINT_RC1=0x01,
  DEINT_RC2,
  DEINT_RC3,
  DEINT_RC4,
  DEINT_RC5
} deint_rc_type;

#define DEINT_SCH_RC_BASE 0x03

typedef enum
{

  PRAT_9600=0,   /* Paging Channel Rate 9600 bps */
  PRAT_4800      /* paging Channel Rate 4800 bps */

} deint_pc_rate_type;


typedef enum
{
  _20MS,
  _5MS,
  _10MS,
  _80MS
} frame_len_type;



typedef struct
{
  word pkt_intlv_cfg_lo;
  /* row_width | chainback_depth | ym_thresh | smt_scalar */
  word pkt_intlv_cfg_hi;                   
  /* intlv_cols | intlv_rows */
  word pkt_blk_size_lo;                    
  /* Block size */
  word pkt_blk_size_hi;    
  /* punct_length | symbol_repeat | code_rate */
  word pkt_punct_lo;                      
  /* lo_punct_pattern | rdcnt_en_sel | num_ltu_frm */
  word pkt_punct_hi;  
  /* punct_pattern_hi */
  word pkt_pl_crc_lo;                      
  /* Physical Layer CRC Polynomial */
  word pkt_pl_crc_hi;                  
  /* LTU CRC Polynomial */

}deint_pkt_params_type;


typedef struct
{
  deint_pkt_params_type fch_full_rate_param;
  deint_pkt_params_type fch_half_rate_param;
  deint_pkt_params_type fch_quarter_rate_param;
  deint_pkt_params_type fch_eighth_rate_param;

} deint_fch_pkt_cfg_type;

typedef struct
{
  deint_pkt_params_type sc_param;
} deint_sc_pkt_cfg_type;

typedef deint_pkt_params_type deint_sch_pkt_cfg_type;


/*=============================================================
   DEINTRELEAVER & TURBO DECODER Params for Turbo Codes
================================================================*/
typedef struct
{
  word td_block_size;
  /* Deinterleaver TD block size. Indicates the block size, N
     for de-interleaving. Programmed value should be N-1. */
  word td_intlv_cfg_lo;
  /* bits 13:15 indicate Row_width for the DeInterleaver(m=logM) 
     bits 0:12   reserved */
  word td_intlv_cfg_hi;
  /* bits 8:15 indicate number of columns for deinterleaver.
     bits 0:7 indicate number of rows (M = 2 ^ (m-1) )  */
  word td_min_llr;
  /* Indicates the Minimum Log-Likelihood ratio used for
     early termination check */
  word td_punct_lo;
  /* Punct_Pattern | Punct_Length | TD_DEC_INT_EN | Code_Rate */
  word td_punct_hi;
  /* bits 0:15 indicate the 16_upper bits of the puncturing pattern */
  word td_intlv_size_lo;
  /* bits 0:14 indicate the Turbo decoder interleaver length 
     bit 15 indicates if Early Termination is enabled or not */
  word td_intlv_size_hi;
  /* bits 0:3 reserved 
     bits 4:9 indicate Minimum number of iterations to be run before
     checking for early termination word td_intlv_size_hi;
     bits 10:15 indicate Maximum number of iterations to be run */
  word td_params_lo;
  /* Indicate the length of Sliding Window, Beta Scalinf factor */
  word td_params_hi;
  /* Indicate the minimum number of CRC checks (pass) and the number
    of windows for the Turbo Decoder */
} deint_td_params_type;  
   

/*=======================================================================

   Data Structures for Deinterleaver/Decoder Sequencing Commands to
   Program Read Parameters.

========================================================================*/
typedef enum
{
  DEINT_FCH_READ_PARAMS,
  DEINT_SCCH_READ_PARAMS,
  DEINT_SCH_READ_PARAMS
}  deint_sequencing_cmd_name_type;


typedef struct
{
  q_link_type link;   /* Queue Link */
  q_type *done_q_ptr;
  deint_sequencing_cmd_name_type command;
}  deint_hdr_type;

typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type fch_rc;
}  deint_fch_read_params_cmd_type;


typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type sch_rc;
  dec_coding_type dec_coding;
  dec_sch_rate_type  dec_sch_rate;
}  deint_sch_read_params_cmd_type;

typedef struct
{
  deint_hdr_type hdr;
  deint_rc_type scch_rc;
  byte num_sup;
}  deint_scch_read_params_cmd_type;


/* Union of all the sequencing commands */
typedef union
{
  deint_hdr_type   hdr;
  deint_fch_read_params_cmd_type fch_cmd;
  deint_scch_read_params_cmd_type scch_cmd;
  deint_sch_read_params_cmd_type sch_cmd;
}  deint_sequencing_cmd_type;


  

/*===============================================================
              RC CONVERSION MACROS 
================================================================*/
#define DEINT_FCH_RC_INDEX(fch_rc)\
  (fch_rc - DEINT_FCH_RC_BASE)

#define DEINT_SCH_RC_INDEX(sch_rc)\
  (sch_rc - DEINT_SCH_RC_BASE)


/*=========================================================================

    DEINTERLEAVER PCAKET PARAMETERS FOR SYNC CHANNEL

==========================================================================*/


#define DINT_PKT_SC_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SC_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_SC_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SC_SMT_SCALAR_V (0xb5)
#define DINT_PKT_SC_INTLV_COLS_V (0x01 << 8)
#define DINT_PKT_SC_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SC_BLOCK_SIZE_V 0x0180
#define DINT_PKT_SC_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SC_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_SC_CODE_RATE_V (0x00)
#define DINT_PKT_SC_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SC_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_SC_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SC_PL_CRC_V (0x0000)
#define DINT_PKT_SC_LTU_CRC_V (0x0000)
#define DINT_PKT_SC_NUM_LTU_FRM_V 0x00

/*========================================================================

       DEINTERLEAVER PACKET PARAMETERS FOR FCH FOR RC1 -RC5

==========================================================================*/

#define DINT_PKT_FCH_RC1_ALL_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_FCH_RC1_ALL_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_FCH_RC1_FULL_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC1_HALF_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_YM_THRESH_V (0x03 << 8)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_YM_THRESH_V (0x03 << 8)
#define DINT_PKT_FCH_RC1_FULL_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_FCH_RC1_HALF_RATE_SMT_SCALAR_V (0xb5)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_SMT_SCALAR_V (0x80)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_SMT_SCALAR_V (0x5a)
#define DINT_PKT_FCH_RC1_ALL_RATE_INTLV_COLS_V (0x06 << 8)
#define DINT_PKT_FCH_RC1_ALL_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_FCH_RC1_ALL_RATE_BLOCK_SIZE_V 0x0180
#define DINT_PKT_FCH_RC1_ALL_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_FCH_RC1_FULL_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_FCH_RC1_HALF_RATE_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_SYMBOL_REPEAT_V (0x02 << 2)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_SYMBOL_REPEAT_V (0x03 << 2)
#define DINT_PKT_FCH_RC1_ALL_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_FCH_RC1_ALL_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_FCH_RC1_ALL_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC1_ALL_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_FCH_RC1_FULL_RATE_PL_CRC_V (0xf130)
#define DINT_PKT_FCH_RC1_HALF_RATE_PL_CRC_V (0x9b00)
#define DINT_PKT_FCH_RC1_QUARTER_RATE_PL_CRC_V (0x0000)
#define DINT_PKT_FCH_RC1_EIGHTH_RATE_PL_CRC_V (0x0000)
#define DINT_PKT_FCH_RC1_ALL_RATE_LTU_CRC_V (0x0000)
#define DINT_PKT_FCH_RC1_ALL_RATE_NUM_LTU_FRM_V 0x00


#define DINT_PKT_FCH_RC2_ALL_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_FCH_RC2_ALL_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_FCH_RC2_FULL_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC2_HALF_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_YM_THRESH_V (0x03 << 8)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_YM_THRESH_V (0x03 << 8)
#define DINT_PKT_FCH_RC2_FULL_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_FCH_RC2_HALF_RATE_SMT_SCALAR_V (0xb5)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_SMT_SCALAR_V (0x80)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_SMT_SCALAR_V (0x5a)
#define DINT_PKT_FCH_RC2_ALL_RATE_INTLV_COLS_V (0x06 << 8)
#define DINT_PKT_FCH_RC2_ALL_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_FCH_RC2_ALL_RATE_BLOCK_SIZE_V 0x0180
#define DINT_PKT_FCH_RC2_ALL_RATE_PUNCT_LENGTH_V (0x05 << 4)
#define DINT_PKT_FCH_RC2_FULL_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_FCH_RC2_HALF_RATE_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_SYMBOL_REPEAT_V (0x02 << 2)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_SYMBOL_REPEAT_V (0x03 << 2)
#define DINT_PKT_FCH_RC2_ALL_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_FCH_RC2_ALL_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_FCH_RC2_ALL_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC2_ALL_RATE_HI_PUNCT_PATTERN_V (0xd7ff)
#define DINT_PKT_FCH_RC2_FULL_RATE_PL_CRC_V (0xf130)
#define DINT_PKT_FCH_RC2_HALF_RATE_PL_CRC_V (0xf640)
#define DINT_PKT_FCH_RC2_QUARTER_RATE_PL_CRC_V (0x9b00)
#define DINT_PKT_FCH_RC2_EIGHTH_RATE_PL_CRC_V (0x1c00)
#define DINT_PKT_FCH_RC2_ALL_RATE_LTU_CRC_V (0x0000)
#define DINT_PKT_FCH_RC2_ALL_RATE_NUM_LTU_FRM_V 0x00


#define DINT_PKT_FCH_RC3_ALL_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_FCH_RC3_ALL_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_FCH_RC3_ALL_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC3_FULL_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_FCH_RC3_HALF_RATE_SMT_SCALAR_V (0xb5)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_SMT_SCALAR_V (0x80)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_SMT_SCALAR_V (0x5a)
#define DINT_PKT_FCH_RC3_ALL_RATE_INTLV_COLS_V (0x0c << 8)
#define DINT_PKT_FCH_RC3_ALL_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_FCH_RC3_ALL_RATE_BLOCK_SIZE_V 0x0300
#define DINT_PKT_FCH_RC3_FULL_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_FCH_RC3_HALF_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_PUNCT_LENGTH_V (0x08 << 4)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_PUNCT_LENGTH_V (0x04 << 4)
#define DINT_PKT_FCH_RC3_FULL_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_FCH_RC3_HALF_RATE_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_SYMBOL_REPEAT_V (0x02 << 2)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_SYMBOL_REPEAT_V (0x03 << 2)
#define DINT_PKT_FCH_RC3_ALL_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_FCH_RC3_ALL_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_FCH_RC3_FULL_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC3_HALF_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_FCH_RC3_FULL_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_FCH_RC3_HALF_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_HI_PUNCT_PATTERN_V (0xff7f)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_HI_PUNCT_PATTERN_V (0xf7ff)
#define DINT_PKT_FCH_RC3_FULL_RATE_PL_CRC_V (0xf130)
#define DINT_PKT_FCH_RC3_HALF_RATE_PL_CRC_V (0x9b00)
#define DINT_PKT_FCH_RC3_QUARTER_RATE_PL_CRC_V (0x9c00)
#define DINT_PKT_FCH_RC3_EIGHTH_RATE_PL_CRC_V (0x9c00)
#define DINT_PKT_FCH_RC3_ALL_RATE_LTU_CRC_V (0x0000)
#define DINT_PKT_FCH_RC3_ALL_RATE_NUM_LTU_FRM_V 0x00

#define DINT_PKT_FCH_RC4_ALL_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_FCH_RC4_ALL_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_FCH_RC4_ALL_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC4_FULL_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_FCH_RC4_HALF_RATE_SMT_SCALAR_V (0xb5)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_SMT_SCALAR_V (0x80)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_SMT_SCALAR_V (0x5a)
#define DINT_PKT_FCH_RC4_ALL_RATE_INTLV_COLS_V (0x06 << 8)
#define DINT_PKT_FCH_RC4_ALL_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_FCH_RC4_ALL_RATE_BLOCK_SIZE_V 0x0180
#define DINT_PKT_FCH_RC4_FULL_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_FCH_RC4_HALF_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_PUNCT_LENGTH_V (0x08 << 4)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_PUNCT_LENGTH_V (0x04 << 4)
#define DINT_PKT_FCH_RC4_FULL_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_FCH_RC4_HALF_RATE_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_SYMBOL_REPEAT_V (0x02 << 2)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_SYMBOL_REPEAT_V (0x03 << 2)
#define DINT_PKT_FCH_RC4_ALL_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_FCH_RC4_ALL_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_FCH_RC4_ALL_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC4_FULL_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_FCH_RC4_HALF_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_HI_PUNCT_PATTERN_V (0xff7f)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_HI_PUNCT_PATTERN_V (0xf7ff)
#define DINT_PKT_FCH_RC4_FULL_RATE_PL_CRC_V (0xf130)
#define DINT_PKT_FCH_RC4_HALF_RATE_PL_CRC_V (0x9b00)
#define DINT_PKT_FCH_RC4_QUARTER_RATE_PL_CRC_V (0x9c00)
#define DINT_PKT_FCH_RC4_EIGHTH_RATE_PL_CRC_V (0x9c00)
#define DINT_PKT_FCH_RC4_ALL_RATE_LTU_CRC_V (0x0000)
#define DINT_PKT_FCH_RC4_ALL_RATE_NUM_LTU_FRM_V 0x00


#define DINT_PKT_FCH_RC5_ALL_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_FCH_RC5_ALL_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_FCH_RC5_ALL_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_FCH_RC5_FULL_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_FCH_RC5_HALF_RATE_SMT_SCALAR_V (0xb5)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_SMT_SCALAR_V (0x80)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_SMT_SCALAR_V (0x5a)
#define DINT_PKT_FCH_RC5_ALL_RATE_INTLV_COLS_V (0x0c << 8)
#define DINT_PKT_FCH_RC5_ALL_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_FCH_RC5_ALL_RATE_BLOCK_SIZE_V 0x0300
#define DINT_PKT_FCH_RC5_ALL_RATE_PUNCT_LENGTH_V (0x0b << 4)
#define DINT_PKT_FCH_RC5_FULL_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_FCH_RC5_HALF_RATE_SYMBOL_REPEAT_V (0x01 << 2)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_SYMBOL_REPEAT_V (0x02 << 2)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_SYMBOL_REPEAT_V (0x03 << 2)
#define DINT_PKT_FCH_RC5_ALL_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_FCH_RC5_ALL_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_FCH_RC5_FULL_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC5_HALF_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_FCH_RC5_ALL_RATE_HI_PUNCT_PATTERN_V (0xd9bf)
#define DINT_PKT_FCH_RC5_FULL_RATE_PL_CRC_V (0xf130)
#define DINT_PKT_FCH_RC5_HALF_RATE_PL_CRC_V (0xf640)
#define DINT_PKT_FCH_RC5_QUARTER_RATE_PL_CRC_V (0x9b00)
#define DINT_PKT_FCH_RC5_EIGHTH_RATE_PL_CRC_V (0x9c00)
#define DINT_PKT_FCH_RC5_ALL_RATE_LTU_CRC_V (0x0000)
#define DINT_PKT_FCH_RC5_ALL_RATE_NUM_LTU_FRM_V 0x00


#define DINT_PKT_SCH_RC3_2X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC3_2X_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_SCH_RC3_2X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC3_2X_RATE_INTLV_COLS_V (0x18 << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC3_2X_RATE_BLOCK_SIZE_V 0x0600
#define DINT_PKT_SCH_RC3_2X_RATE_LTU_LEN_V (0x00 << 9)
#define DINT_PKT_SCH_RC3_2X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC3_2X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC3_2X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC3_2X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC3_2X_RATE_NUM_LTU_FRM_V 0x00
#define DINT_PKT_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC3_2X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC3_2X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC3_4X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC3_4X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC3_4X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC3_4X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC3_4X_RATE_BLOCK_SIZE_V 0x0c00
#define DINT_PKT_SCH_RC3_4X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC3_4X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC3_4X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC3_4X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC3_4X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC3_4X_RATE_NUM_LTU_FRM_V 0x02
#define DINT_PKT_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC3_4X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC3_4X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC3_8X_RATE_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SCH_RC3_8X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC3_8X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC3_8X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC3_8X_RATE_BLOCK_SIZE_V 0x1800
#define DINT_PKT_SCH_RC3_8X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC3_8X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC3_8X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC3_8X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC3_8X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC3_8X_RATE_NUM_LTU_FRM_V 0x04
#define DINT_PKT_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC3_8X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC3_8X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC3_16X_RATE_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SCH_RC3_16X_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_SCH_RC3_16X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC3_16X_RATE_INTLV_COLS_V (0x60 << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC3_16X_RATE_BLOCK_SIZE_V 0x3000
#define DINT_PKT_SCH_RC3_16X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC3_16X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC3_16X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC3_16X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC3_16X_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_SCH_RC3_16X_RATE_NUM_LTU_FRM_V 0x08
#define DINT_PKT_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC3_16X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC3_16X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC4_2X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC4_2X_RATE_CHAINBACK_DEPTH_V (0x00 << 12)
#define DINT_PKT_SCH_RC4_2X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC4_2X_RATE_INTLV_COLS_V (0x0c << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC4_2X_RATE_BLOCK_SIZE_V 0x0300
#define DINT_PKT_SCH_RC4_2X_RATE_LTU_LEN_V (0x00 << 9)
#define DINT_PKT_SCH_RC4_2X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC4_2X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC4_2X_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC4_2X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC4_2X_RATE_NUM_LTU_FRM_V 0x00
#define DINT_PKT_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC4_2X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC4_2X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC4_4X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC4_4X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC4_4X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC4_4X_RATE_INTLV_COLS_V (0x18 << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC4_4X_RATE_BLOCK_SIZE_V 0x0600
#define DINT_PKT_SCH_RC4_4X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC4_4X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC4_4X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC4_4X_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC4_4X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC4_4X_RATE_NUM_LTU_FRM_V 0x02
#define DINT_PKT_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC4_4X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC4_4X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC4_8X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC4_8X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC4_8X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC4_8X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC4_8X_RATE_BLOCK_SIZE_V 0x0c00
#define DINT_PKT_SCH_RC4_8X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC4_8X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC4_8X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC4_8X_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC4_8X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC4_8X_RATE_NUM_LTU_FRM_V 0x04
#define DINT_PKT_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC4_8X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC4_8X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC4_16X_RATE_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SCH_RC4_16X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC4_16X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC4_16X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC4_16X_RATE_BLOCK_SIZE_V 0x1800
#define DINT_PKT_SCH_RC4_16X_RATE_LTU_LEN_V (0x2e << 9)
#define DINT_PKT_SCH_RC4_16X_RATE_PUNCT_LENGTH_V (0x00 << 4)
#define DINT_PKT_SCH_RC4_16X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC4_16X_RATE_CODE_RATE_V (0x00)
#define DINT_PKT_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC4_16X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC4_16X_RATE_NUM_LTU_FRM_V 0x08
#define DINT_PKT_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_PKT_SCH_RC4_16X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC4_16X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC5_2X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC5_2X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC5_2X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC5_2X_RATE_INTLV_COLS_V (0x18 << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC5_2X_RATE_BLOCK_SIZE_V 0x0600
#define DINT_PKT_SCH_RC5_2X_RATE_LTU_LEN_V (0x00 << 9) 
#define DINT_PKT_SCH_RC5_2X_RATE_PUNCT_LENGTH_V (0x0B << 4)
#define DINT_PKT_SCH_RC5_2X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC5_2X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC5_2X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC5_2X_RATE_NUM_LTU_FRM_V 0x00 
#define DINT_PKT_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V (0xd9bf)
#define DINT_PKT_SCH_RC5_2X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC5_2X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC5_4X_RATE_ROW_WIDTH_V (0x03 << 13)
#define DINT_PKT_SCH_RC5_4X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC5_4X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC5_4X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_INTLV_ROWS_V (0x40 << 0)
#define DINT_PKT_SCH_RC5_4X_RATE_BLOCK_SIZE_V 0x0c00
#define DINT_PKT_SCH_RC5_4X_RATE_LTU_LEN_V (0x46 << 9) 
#define DINT_PKT_SCH_RC5_4X_RATE_PUNCT_LENGTH_V (0x0B << 4)
#define DINT_PKT_SCH_RC5_4X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC5_4X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC5_4X_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_SCH_RC5_4X_RATE_NUM_LTU_FRM_V 0x02 
#define DINT_PKT_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V (0xd9bf)
#define DINT_PKT_SCH_RC5_4X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC5_4X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC5_8X_RATE_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SCH_RC5_8X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC5_8X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC5_8X_RATE_INTLV_COLS_V (0x30 << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC5_8X_RATE_BLOCK_SIZE_V 0x1800
#define DINT_PKT_SCH_RC5_8X_RATE_LTU_LEN_V (0x46 << 9)
#define DINT_PKT_SCH_RC5_8X_RATE_PUNCT_LENGTH_V (0x0B << 4)
#define DINT_PKT_SCH_RC5_8X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC5_8X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC5_8X_RATE_RDCNT_EN_SEL_V (0x01 << 4)
#define DINT_PKT_SCH_RC5_8X_RATE_NUM_LTU_FRM_V 0x04 
#define DINT_PKT_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V (0xd9bf)
#define DINT_PKT_SCH_RC5_8X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC5_8X_RATE_LTU_CRC_V (0xc867)

#define DINT_PKT_SCH_RC5_16X_RATE_ROW_WIDTH_V (0x04 << 13)
#define DINT_PKT_SCH_RC5_16X_RATE_CHAINBACK_DEPTH_V (0x01 << 12)
#define DINT_PKT_SCH_RC5_16X_RATE_YM_THRESH_V (0x01 << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_SMT_SCALAR_V (0xff)
#define DINT_PKT_SCH_RC5_16X_RATE_INTLV_COLS_V (0x60 << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_INTLV_ROWS_V (0x80 << 0)
#define DINT_PKT_SCH_RC5_16X_RATE_BLOCK_SIZE_V 0x3000
#define DINT_PKT_SCH_RC5_16X_RATE_LTU_LEN_V (0x46 << 9)
#define DINT_PKT_SCH_RC5_16X_RATE_PUNCT_LENGTH_V (0x0B << 4)
#define DINT_PKT_SCH_RC5_16X_RATE_SYMBOL_REPEAT_V (0x00 << 2)
#define DINT_PKT_SCH_RC5_16X_RATE_CODE_RATE_V (0x02)
#define DINT_PKT_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_PKT_SCH_RC5_16X_RATE_RDCNT_EN_SEL_V (0x00 << 4)
#define DINT_PKT_SCH_RC5_16X_RATE_NUM_LTU_FRM_V 0x08
#define DINT_PKT_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V (0xd9bf)
#define DINT_PKT_SCH_RC5_16X_RATE_PL_CRC_V (0xc867)
#define DINT_PKT_SCH_RC5_16X_RATE_LTU_CRC_V (0xc867)




/*==================================================================
   TURBO DECODER  PARAMTER CONSTANT
===================================================================*/

#define DINT_TD_EARLY_TERM_EN (0x01 << 15)
/* Enables Early Termination Checking for the Turbo Decoder */
#define DINT_TD_EARLY_TERM_DIS (0x01 << 15)
/* Disables Early Termination Checking for the Turbo Decoder */
#define DINT_TD_MAX_ITER_NUM (0x09 << 10)
/* Maximum number of iterations for all rates = 10 */
#define DINT_TD_MIN_ITER_NUM (0x00 << 4)
/* Minimum of 1 iteration for all rates */
#define DINT_TD_MIN_NUM_CRC_PASS (0x01 << 10)
/* Minimum number of consective CRC passes = 2*/
#define DINT_TD_DEC_INT_EN (0x01 << 2)
/* Enable Turbo Decoder Interrupt after the frame is decoded */
#define DINT_TD_DEC_INT_DIS (0x00 << 2)
/* Do Not Enable Turbo Decoder Interrupt after the frame is decoded */

#define DINT_TD_SCH_RC3_2X_RATE_BLOCK_SIZE_V (0x5ff)
#define DINT_TD_SCH_RC3_2X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC3_2X_RATE_INTLV_COLS_V (0x17 << 8)
#define DINT_TD_SCH_RC3_2X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC3_2X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC3_2X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC3_2X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC3_2X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC3_2X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC3_2X_RATE_TD_INTLV_LEN_V (0x179)
#define DINT_TD_SCH_RC3_2X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC3_2X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC3_2X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC3_2X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC3_2X_RATE_NUM_WINDOWS_V (0x0a)


#define DINT_TD_SCH_RC3_4X_RATE_BLOCK_SIZE_V (0xbff)
#define DINT_TD_SCH_RC3_4X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC3_4X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC3_4X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC3_4X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC3_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC3_4X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC3_4X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC3_4X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC3_4X_RATE_TD_INTLV_LEN_V (0x2f9)
#define DINT_TD_SCH_RC3_4X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC3_4X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC3_4X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC3_4X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC3_4X_RATE_NUM_WINDOWS_V (0x16)


#define DINT_TD_SCH_RC3_8X_RATE_BLOCK_SIZE_V (0x17ff)
#define DINT_TD_SCH_RC3_8X_RATE_ROW_WIDTH_V  (0x04 << 13)
#define DINT_TD_SCH_RC3_8X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC3_8X_RATE_INTLV_ROWS_V (0x7f)
#define DINT_TD_SCH_RC3_8X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC3_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC3_8X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC3_8X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC3_8X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC3_8X_RATE_TD_INTLV_LEN_V (0x5f9)
#define DINT_TD_SCH_RC3_8X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC3_8X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC3_8X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC3_8X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC3_8X_RATE_NUM_WINDOWS_V (0x2e)


#define DINT_TD_SCH_RC3_16X_RATE_BLOCK_SIZE_V (0x2fff)
#define DINT_TD_SCH_RC3_16X_RATE_ROW_WIDTH_V  (0x04 << 13)
#define DINT_TD_SCH_RC3_16X_RATE_INTLV_COLS_V (0x5f << 8)
#define DINT_TD_SCH_RC3_16X_RATE_INTLV_ROWS_V (0x7f)
#define DINT_TD_SCH_RC3_16X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC3_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC3_16X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC3_16X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC3_16X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC3_16X_RATE_TD_INTLV_LEN_V (0xbf9)
#define DINT_TD_SCH_RC3_16X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC3_16X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC3_16X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC3_16X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC3_16X_RATE_NUM_WINDOWS_V (0x5e)



#define DINT_TD_SCH_RC4_2X_RATE_BLOCK_SIZE_V (0x2ff)
#define DINT_TD_SCH_RC4_2X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC4_2X_RATE_INTLV_COLS_V (0x0b << 8)
#define DINT_TD_SCH_RC4_2X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC4_2X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC4_2X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC4_2X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC4_2X_RATE_CODE_RATE_V (0x00)
#define DINT_TD_SCH_RC4_2X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC4_2X_RATE_TD_INTLV_LEN_V (0x179)
#define DINT_TD_SCH_RC4_2X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC4_2X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC4_2X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC4_2X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC4_2X_RATE_NUM_WINDOWS_V (0x0a)


#define DINT_TD_SCH_RC4_4X_RATE_BLOCK_SIZE_V (0x5ff)
#define DINT_TD_SCH_RC4_4X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC4_4X_RATE_INTLV_COLS_V (0x17 << 8)
#define DINT_TD_SCH_RC4_4X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC4_4X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC4_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC4_4X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC4_4X_RATE_CODE_RATE_V (0x00)
#define DINT_TD_SCH_RC4_4X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC4_4X_RATE_TD_INTLV_LEN_V (0x2f9)
#define DINT_TD_SCH_RC4_4X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC4_4X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC4_4X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC4_4X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC4_4X_RATE_NUM_WINDOWS_V (0x16)


#define DINT_TD_SCH_RC4_8X_RATE_BLOCK_SIZE_V (0xbff)
#define DINT_TD_SCH_RC4_8X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC4_8X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC4_8X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC4_8X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC4_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC4_8X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC4_8X_RATE_CODE_RATE_V (0x00)
#define DINT_TD_SCH_RC4_8X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC4_8X_RATE_TD_INTLV_LEN_V (0x5f9)
#define DINT_TD_SCH_RC4_8X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC4_8X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC4_8X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC4_8X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC4_8X_RATE_NUM_WINDOWS_V (0x2e)


#define DINT_TD_SCH_RC4_16X_RATE_BLOCK_SIZE_V (0x17ff)
#define DINT_TD_SCH_RC4_16X_RATE_ROW_WIDTH_V  (0x04 << 13)
#define DINT_TD_SCH_RC4_16X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC4_16X_RATE_INTLV_ROWS_V (0x7f)
#define DINT_TD_SCH_RC4_16X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC4_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC4_16X_RATE_PUNCT_LENGTH_V (0x17 << 3)
#define DINT_TD_SCH_RC4_16X_RATE_CODE_RATE_V (0x00)
#define DINT_TD_SCH_RC4_16X_RATE_HI_PUNCT_PATTERN_V (0xffff)
#define DINT_TD_SCH_RC4_16X_RATE_TD_INTLV_LEN_V (0xbf9)
#define DINT_TD_SCH_RC4_16X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC4_16X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC4_16X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC4_16X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC4_16X_RATE_NUM_WINDOWS_V (0x5e)


#define DINT_TD_SCH_RC5_2X_RATE_BLOCK_SIZE_V (0x5ff)
#define DINT_TD_SCH_RC5_2X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC5_2X_RATE_INTLV_COLS_V (0x17 << 8)
#define DINT_TD_SCH_RC5_2X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC5_2X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC5_2X_RATE_LO_PUNCT_PATTERN_V (0x00 << 8)
#define DINT_TD_SCH_RC5_2X_RATE_PUNCT_LENGTH_V (0x0b << 3)
#define DINT_TD_SCH_RC5_2X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC5_2X_RATE_HI_PUNCT_PATTERN_V (0xdda0)
#define DINT_TD_SCH_RC5_2X_RATE_TD_INTLV_LEN_V (0x239)
#define DINT_TD_SCH_RC5_2X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC5_2X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC5_2X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC5_2X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC5_2X_RATE_NUM_WINDOWS_V (0x10)


#define DINT_TD_SCH_RC5_4X_RATE_BLOCK_SIZE_V (0xbff)
#define DINT_TD_SCH_RC5_4X_RATE_ROW_WIDTH_V  (0x03 << 13)
#define DINT_TD_SCH_RC5_4X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC5_4X_RATE_INTLV_ROWS_V (0x3f)
#define DINT_TD_SCH_RC5_4X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC5_4X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC5_4X_RATE_PUNCT_LENGTH_V (0x0b << 3)
#define DINT_TD_SCH_RC5_4X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC5_4X_RATE_HI_PUNCT_PATTERN_V (0xddaf)
#define DINT_TD_SCH_RC5_4X_RATE_TD_INTLV_LEN_V (0x479)
#define DINT_TD_SCH_RC5_4X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC5_4X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC5_4X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC5_4X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC5_4X_RATE_NUM_WINDOWS_V (0x22)


#define DINT_TD_SCH_RC5_8X_RATE_BLOCK_SIZE_V (0x17ff)
#define DINT_TD_SCH_RC5_8X_RATE_ROW_WIDTH_V  (0x04 << 13)
#define DINT_TD_SCH_RC5_8X_RATE_INTLV_COLS_V (0x2f << 8)
#define DINT_TD_SCH_RC5_8X_RATE_INTLV_ROWS_V (0x7f)
#define DINT_TD_SCH_RC5_8X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC5_8X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC5_8X_RATE_PUNCT_LENGTH_V (0x0b << 3)
#define DINT_TD_SCH_RC5_8X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC5_8X_RATE_HI_PUNCT_PATTERN_V (0xddaf)
#define DINT_TD_SCH_RC5_8X_RATE_TD_INTLV_LEN_V (0x8f9)
#define DINT_TD_SCH_RC5_8X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC5_8X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC5_8X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC5_8X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC5_8X_RATE_NUM_WINDOWS_V (0x46)


#define DINT_TD_SCH_RC5_16X_RATE_BLOCK_SIZE_V (0x2fff)
#define DINT_TD_SCH_RC5_16X_RATE_ROW_WIDTH_V  (0x04 << 13)
#define DINT_TD_SCH_RC5_16X_RATE_INTLV_COLS_V (0x5f << 8)
#define DINT_TD_SCH_RC5_16X_RATE_INTLV_ROWS_V (0x7f)
#define DINT_TD_SCH_RC5_16X_RATE_MIN_LLR_V    (0x00)
#define DINT_TD_SCH_RC5_16X_RATE_LO_PUNCT_PATTERN_V (0xff << 8)
#define DINT_TD_SCH_RC5_16X_RATE_PUNCT_LENGTH_V (0x0b << 3)
#define DINT_TD_SCH_RC5_16X_RATE_CODE_RATE_V (0x02)
#define DINT_TD_SCH_RC5_16X_RATE_HI_PUNCT_PATTERN_V (0xddaf)
#define DINT_TD_SCH_RC5_16X_RATE_TD_INTLV_LEN_V (0xbf9)
#define DINT_TD_SCH_RC5_16X_RATE__FRAME_20M_RATE_HYP_V (0x00 << 14)
#define DINT_TD_SCH_RC5_16X_RATE__SLIDE_WIN_LEN_V (0x1f << 9)
#define DINT_TD_SCH_RC5_16X_RATE__PARTIAL_WIN_LEN_V (0x1c << 4)
#define DINT_TD_SCH_RC5_16X_RATE_BETA_V (0x04)
#define DINT_TD_SCH_RC5_16X_RATE_NUM_WINDOWS_V (0x5e)



/*===========================================================================
                      FUNCTION DECLARATIONS

=============================================================================*/

/*============================================================================

FUNCTION DEINT_INIT

DESCRIPTION
  Enables the DEC clock regime and selects CHIPX8 clock.
  Calls dec_init to initialize the SVD Polynomials.

DEPENDENCIES
  DEC Clock regime should have been enabled.

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/

extern void deint_init
(
  void
);

/*===========================================================================

FUNCTION DEINT_ALLOCATE_CH0_RAM

DESCRIPTION
  F-FCH, SYNC and Paging channel symbols are received on Channel 0. This
  fucntion programs the RAM usage for channel 0.
  We allocate (static) Channel 0 to RAM L (This function is called from
  deint_init()). Size of RAM L is 768*6 bits. So RAM L
  can hold symbols for SYNC, PAGING and F-FCH (Radio Config 1-5).
  NOTE**** This is a private function, it should be called only from
  deint_init()

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
   The values programmes in the DINT_CFG resgister effect the deinterleaver
   write control.
=============================================================================*/
extern void deint_allocate_CH0_RAM
(
  void
);


/*===========================================================================

FUNCTION DEINT_ALLOCATE_CH2_RAM_SCCH

DESCRIPTION
 SCCH symbols are received on Channel 2. This fucntion programs the RAM usage
  for channel 2 for SCCH.
  We allocate (static) Channel  to RAM N . Size of RAM N is 6144*6. Maximum
  number of symbols for RC1 and RC2 for SCCH
  = 384*7=2688 symbols.So we select RAM N and allocate channel 2 to RAM N.
  NOTE**** This is a private function, it should be called only from deint_init


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The values programmes in the DINT_CFG resgister effect the deinterleaver
  write control.
============================================================================*/
extern void deint_allocate_CH1_RAM_SCCH
(
  void
);

/*===========================================================================

FUNCTION DEINT_ALLOCATE_CH2_RAM_SCH

DESCRIPTION
 SCH symbols are received on Channel 2. This function programs the RAM usage
 for channel 2 for SCH. CH2 is allocated to Ram N and RAM P.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
============================================================================*/
extern void deint_allocate_CH2_RAM_SCH
(
  dec_coding_type dec_coding
);

/*===========================================================================

FUNCTION DEINT_SET_SC_MODE

DESCRIPTION
    This function calls dec_sc_init() which puts the decoder in continous mode
    and resets the SVD.  This function loads the deinterleaver packet parameter
    for SYNC Channel.

DEPENDENCIES
    deint_init() should have been called

RETURN VALUE
  None

SIDE EFFECTS
   The deinterleaver packet parameters for SYNC Channel are loaded in PACKET
   RAM.
=============================================================================*/
extern void deint_set_sc_mode
(
  void
);

/*==========================================================================
FUNCTION DEINT_SET_PC_MODE

DESCRIPTION
  This function calls dec_pc_init() which puts the decoder in continous
  mode and resets the SVD. This function loads the deinterleaver packet
  parameters for PAGING Channel.

DEPENDENCIES
 deint_init() should have been called.

RETURN VALUE
  None

SIDE EFFECTS
   The deinterleaver packet parameters for PAGING Channel are loaded in PACKET
   RAM.
=============================================================================*/
extern void deint_set_pc_mode
( 
  deint_pc_rate_type pc_rate
);

/*==========================================================================
FUNCTION DEINT_SET_FCH_MODE

DESCRIPTION
    This function calls dec_tc_init() which puts the decoder in packet
    mode and resets the SVD. This function loads the deinterleaver packet
    parameters for FCH of a given Radio Configuration.

DPENEDENCIES
    deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
extern void deint_set_fch_mode
(
  deint_rc_type fch_rc
);

/*==========================================================================
FUNCTION DEINT_SET_SCCH_MODE

DESCRIPTION
    This function will initialize the deinterleaver for Supplemental Channels.
    This function loads the deinterleaver packet
    parameters for SCCH of a given Radio Configuration.

DPENEDENCIES
    deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
extern void deint_set_scch_mode
(
  deint_rc_type scch_rc, 
  byte num_sup
);

/*==========================================================================
FUNCTION DEINT_SET_SCH_MODE

DESCRIPTION
    This function calls dec_tc_init() which puts the decoder in packet
    mode and resets the SVD. This function loads the deinterleaver packet
    parameters for FCH of a given Radio Configuration.

DPENEDENCIES
    deint_init should have been called.

RETURN VALUE
  None

SIDE EFFECTS


=============================================================================*/
#ifdef FEATURE_MULTIFRAME_INTERLEAVING
#error code not present
#else
extern void deint_set_sch_mode
(
  deint_rc_type sch_rc, 
  dec_coding_type
  dec_coding, 
  dec_sch_rate_type sch_rate
);
#endif  /* FEATURE_MULTIFRAME_INTERLEAVING */

/*==========================================================================
FUNCTION DEINT_LOAD_SC_PKT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for SYNC Channel.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
extern void deint_load_sc_pkt_params
(
  void
);

/*===========================================================================
FUNCTION DEINT_LOAD_PC_PKT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for PAGING Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFCTS
 None

============================================================================*/
extern void deint_load_pc_pkt_params
( 
  deint_pc_rate_type pc_rate
);

/*===========================================================================
FUNCTION DEINT_LOAD_FCH_PKT_PARAMS

DESCRIPTION
  This function programs 4 packets corresponding to each RC for FCH.
  The four packets correspond to FULL RATE, HALF RATE,QUARTER RATE & 8TH
  HYPOTHESIS.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None
===========================================================================*/
extern boolean deint_load_fch_pkt_params
(
  deint_rc_type fch_rc
);


/*===========================================================================
FUNCTION DEINT_LOAD_SCCH_PKT_PARAMS

DESCRIPTION
  This function programs 4 packets corresponding to each RC for FCH.
  The four packets correspond to FULL RATE, HALF RATE,QUARTER RATE & 8TH
  HYPOTHESIS.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None
===========================================================================*/

extern boolean deint_load_scch_pkt_params
(
  deint_rc_type scch_rc
);

/*==========================================================================
FUNCTION DEINT_LOAD_TD_DEINT_PARAMS

DESCRIPTION
  This function loads the deinterleaver packet parameters for Turbo Encoded
  Supplemental Channel.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void deint_load_td_deint_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
);

/*==========================================================================
FUNCTION DEINT_LOAD_TD_DEC_PARAMS

DESCRIPTION
  This function loads the decoder packet parameters for Turbo Encoded
  Supplemental Channel.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
   None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void deint_load_td_dec_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
);

/*===========================================================================
FUNCTION DEINT_LOAD_SCH_PKT_PARAMS

DESCRIPTION
  This function programs 4 packets corresponding to each RC for FCH.
  The four packets correspond to FULL RATE, HALF RATE,QUARTER RATE & 8TH
  HYPOTHESIS.
  The values programmed into the registers take effect at the next SYNC20.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 None
===========================================================================*/

extern void deint_load_sch_pkt_params
(
  deint_rc_type sch_rc,
  dec_sch_rate_type sch_rate
);


/*==========================================================================
FUNCTION DEINT_CREATE_SC_TASK_LIST()

DESCRIPTION
  Create Task1 corresponding to SYNC CHANNEL HYPOTHESIS. Packet A
  contains the configuration  parameters for SYNC CHANNEL HYPOTHESIS.

DEPENDENCIES

RETURN VALUE
 None

=============================================================================*/
extern void deint_create_sc_task_list
(
  void
);

/*===========================================================================
FUNCTION DEINT_CREATE_PC_TASK_LIST()

DESCRIPTION
  This function creates a task for decoding paging channel message in the
  first 5ms slot.

DEPENDENCIES
  deint_load_pc_pjt_params should have been called.

RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
extern void deint_create_pc_task_list
(
  void
);

/*=========================================================================
FUNCTION DEINT_CREATE_FCH_TASK_LIST

DESCRIPTION
  This function creates 4 tasks in the first 5ms slot of task RAM. The 4 tasks
  correspond to FULL RATE, HALF RATE, QUARTER RATE and 8TH RATE Hypothesis.
  Decoder Interrupt is enabled after the last task is completed.

DEPENDENCIES
  deint_load_fch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
extern void deint_create_fch_task_list
(
  deint_rc_type fch_rc, 
  boolean int_enable
);

/*=========================================================================
FUNCTION DEINT_CREATE_SCCH_TASK_LIST

DESCRIPTION
  This function creates a task to decode SCCH in the second 5ms slot of task
  RAM.
  Decoder Interrupt is enabled after the  task is completed.

DEPENDENCIES
  deint_load_scch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/

void deint_create_scch_task_list
(
  deint_rc_type scch_rc
);

/*=========================================================================
FUNCTION DEINT_CREATE_SCH_TASK_LIST

DESCRIPTION
  This function creates a task to decode SCH in the second 5ms slot of task
  RAM.
  Decoder Interrupt is enabled after the  task is completed.

DEPENDENCIES
  deint_load_sch_pkt_params should have been called.

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/

extern void deint_create_sch_task_list
(
  deint_rc_type sch_rc,
  dec_sch_rate_type dec_sch_rate
/* Supplemental Channel Data Rate (1X, 2X, 4X, 8X, 16X) */
);

/*=========================================================================
FUNCTION DEINT_DELETE_SUPPLEMENTAL_TASK_LIST

DESCRIPTION
    This function deletes the sch/scch task from the deinterleaver task ram.
    The SCh/SCCH task is the first task of the second 5ms slot.

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/

extern void deint_delete_supplemental_task_list
(
  void
);

/*=========================================================================
FUNCTION DEINT_CREATE_DUMMY_SCH_TASK_LIST

DESCRIPTION
    This function creates a dummy task in the second 5ms slot. 
    This function should be called only when turbo sch is enabled.

DEPENDENCIES
 

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/

void deint_create_dummy_sch_task_list
(
  void
);

/*=========================================================================
FUNCTION DEINT_PROCESS_SEQUENCING_CMD

DESCRIPTION
  This function checks the deint cmd_q to see if there are any pending
commands to program the read parameters corresponding to FCH/SCH/SCCH.

DEPENDENCIES

RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/

void deint_process_sequencing_cmd
(
  void
);




#ifdef COSIM_TEST
#error code not present
#endif  /* COSIM_TEST */


#endif









