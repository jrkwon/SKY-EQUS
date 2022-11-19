#ifndef MSM50REG_H
#define MSM50REG_H
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                                                       _                   --
//                                                     _/ \_                 --
//                CDMA DIGITAL CELLULAR               / \_/ \                --
//                                                    \_/ \_/                --
//                                                    / \_/ \                --
//            MOBILE STATION MODEM - MSM5000          \_/ \_/                --
//                                                      \_/                  --
//                                                                           --
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//   This file defines addresses for all the registers in MSM3000
//
//
//                      EDIT HISTORY FOR FILE
//
// $Header:   L:/src/asw/MSM5000/VCS/msm50reg.h_v   1.21   14 Dec 2000 11:57:20   evanmell  $
//
//  This section contains comments describing changes made to the module.
//  Notice that changes are listed in reverse chronological order.
//
//
// when       who     what, where, why
// --------   ---     ---------------------------------------------------------
// 12/14/00   ejv     Changed Fn_OTD_CFG_WH to WB.
// 11/10/00   bgc     Fixed some misleading comments.
// 09/10/00   ejv     Added Fn_OTD_CFG_WH and masks.
// 08/11/00   rmd     replaced DEC_OB_DATA_WH with DEC_OB_DATA_RH, Hld shows 
//                    this register to be write only but it is a read only register
// 07/28/00   rmd     Corrected the masks for DINT_PKT_OFFSET.
// 07/19/00   hrk     Added definition for register DEM_REV_PWR_CONTROL_DELAY
// 03/24/00   fs      Added new bit values for AGC_CTL3 and
//                    TXC_EARLY_FRAME_CTL.
// 03/15/00   ejv     Edited the SEARCH2_POSITION macros.
// 02/24/00   eh      Added MSM_CLK_CTL1_WH__RXCX8_MICRO_BYPASS_MASK.
// 02/16/00   eh      Added new registers for MSM5000B and support for 7 SCCH.
// 02/01/00   lcc     Corrected definition of DFM_SLOT_PDM_CTL_WB to
//                    HW_DFM_SLOT_PDM_CTL.   It was incorrectly mapped to
//                    HW_DFM_SLOT_CTL_1 before.  Also added definitions for
//                    HW_DFM_SLOT_CTL_1.
// 01/17/00   ejv     Added sleep controller read register definitions.
// 10/07/99   lcc     Merged from PLT 2.0 (includes SCH support).
// 10/05/99   lcc     Corrected definition of FIQ_MASK_0_WH from FIQ_MASK_0
//                    HW_FIQ_MASK_0.
// 08/24/99   jcw     Changes to support UART clocks for MSM5000
// 08/02/99   jcw     Prepended HW_ to all labels to avoid namespace conflicts
// 07/23/99   hrk     Added masks for FPC_CTL register, and some clean-up
// 06/15/99   rmd     Updated registers for MSM5000
// 11/17/98   udp     Created from msm3rev2.h, now is a generic file for MSM3000
// 09/27/98   udp     Clean-up unused sections
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//   QUALCOMM PROPRIETARY
//       Copyright (c) 1997, 1998, 1999, 2000 Qualcomm Incorporated
//-----------------------------------------------------------------------------
#include "addrdefs.h"

#define CHIP_BASE                               HW_CHIP_BASE
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     DEMODULATOR WRITE REGISTERS
//-----------------------------------------------------------------------------
#define HW_PAD_CNTRL_WB       HW_PAD_CNTRL
#define HW_PAD_CNTRL_WB_MASK      (0x01 << 0)
#define DEMOD_RESET_WB                          HW_DEMOD_RESET
#define DEMOD_RESET_WB_MASK                     0x0ffff
#define SRCH_CTL_WB                             HW_SRCH_CTL
#define SRCH_CTL_WB_MASK                        0x03f
#define SRCH_CTL_WB__SEARCH_SPEED_MASK          0x020
#define SRCH_CTL_WB__DMA_DISABLE_MASK           0x010
#define SRCH_CTL_WB__STATUS_DUMP_MASK           0x08
#define SRCH_CTL_WB__SEARCHER_GAIN_MASK         0x03
#define SRCH_TH_ENERGY_LOW_WB                   HW_SRCH_TH_ENERGY_LOW
#define SRCH_TH_ENERGY_HIGH_WB                  HW_SRCH_TH_ENERGY_HIGH
#define SRCH_INTG_TIME_WB                       HW_SRCH_INTG_TIME
#define SRCH_TH_TIME_WB                         HW_SRCH_TH_TIME
#define SRCH_OFFSET_LOW_WB                      HW_SRCH_OFFSET_LOW
#define SRCH_OFFSET_HIGH_WB                     HW_SRCH_OFFSET_HIGH
#define SRCH_OFFSET_HIGH_WB_MASK                0x07f
#define SRCH_MASK_I_LOW_WB                      HW_SRCH_MASK_I_LOW
#define SRCH_MASK_I_HIGH_WB                     HW_SRCH_MASK_I_HIGH
#define SRCH_MASK_I_HIGH_WB_MASK                0x07f
#define SRCH_MASK_Q_LOW_WB                      HW_SRCH_MASK_Q_LOW
#define SRCH_MASK_Q_HIGH_WB                     HW_SRCH_MASK_Q_HIGH
#define SRCH_MASK_Q_HIGH_WB_MASK                0x07f
#define SRCH_SLEW_LOW_WB                        HW_SRCH_SLEW_LOW
#define SRCH_SLEW_MID_WB                        HW_SRCH_SLEW_MID
#define SRCH_SLEW_HIGH_WB                       HW_SRCH_SLEW_HIGH
#define SRCH_SLEW_HIGH_WB_MASK                  0x07
#define SRCH_ACC_PASS_WB                        HW_SRCH_ACC_PASS
#define SRCH_ACC_PASS_WB_MASK                   0x07
#define SRCH_NUM_LOW_WB                         HW_SRCH_NUM_LOW
#define SRCH_NUM_HIGH_WB                        HW_SRCH_NUM_HIGH
#define SRCH_MAX_SELECT_WB                      HW_SRCH_MAX_SELECT
#define SRCH_MAX_SELECT_WB_MASK                 0x03

//MSM5000, Demodulator, New register, Addr 0x04C, 11 bits long
#define DEM_IRAM_ADDR_WH                        HW_DEM_IRAM_ADDR
#define DEM_IRAM_ADDR_WH_MASK                   0x07ff

//MSM5000, Demodulator, New register, Addr 0x050, 16 bits long
#define DEM_IRAM_DIN_WH                         HW_DEM_IRAM_DIN
#define DEM_IRAM_DIN_WH_MASK                    0x0ffff

//MSM5000, Demodulator, New register, Addr 0x054, 16 bits long
#define DEM_DMEM_ADDR_WH                        HW_DEM_DMEM_ADDR
#define DEM_DMEM_ADDR_WH_MASK                   0x0ffff
#define DEM_DMEM_ADDR_WH__READ_MASK             0x08000
#define DEM_DMEM_ADDR_WH__RAM_AB_SEL_MASK       0x02000
#define DEM_DMEM_ADDR_WH__ADDR_MASK             0x02ff

//MSM5000, Demodulator, New register, Addr 0x058, 16 bits long
#define DEM_DMEM_DIN_WH                         HW_DEM_DMEM_DIN
#define DEM_DMEM_DIN_WH_MASK                    0x0ffff

//MSM5000, Demodulator, New register, Addr 0x05C, 1 bit long
#define DEM_DSP_RESET_WB                        HW_DEM_DSP_RESET
#define DEM_DSP_RESET_WB_MASK                   0x01


#define FFE_POWER_CTL_F0_CELLN_EN_WB            HW_FFE_POWER_CTL_F0_CELLN_EN
#define FFE_POWER_CTL_F0_CELLN_EN_WB_MASK 0x07
#define FFE_POWER_CTL_F1_CELLN_EN_WB            HW_FFE_POWER_CTL_F1_CELLN_EN
#define FFE_POWER_CTL_F1_CELLN_EN_WB_MASK               0x07
#define FFE_POWER_CTL_F2_CELLN_EN_WB            HW_FFE_POWER_CTL_F2_CELLN_EN
#define FFE_POWER_CTL_F2_CELLN_EN_WB_MASK               0x07
#define FFE_POWER_CTL_F3_CELLN_EN_WB            HW_FFE_POWER_CTL_F3_CELLN_EN
#define FFE_POWER_CTL_F3_CELLN_EN_WB_MASK               0x07
//MSM5000, demodulator, addr 0x070, change from 6 to 7 bit long.
#define FFE_FINGER_CTL1_WB                      HW_FFE_FINGER_CTL1
#define FFE_FINGER_CTL1_WB_MASK                       0x07f
#define FFE_FINGER_CTL1_WB__PN_POSITION_MODE_MASK     0x040 //MSM5000, new bit
#define FFE_FINGER_CTL1_WB__FINGER_DISABLE_MASK       0x020
#define FFE_FINGER_CTL1_WB__LOCK_SET_HYST_MASK        0x04
#define FFE_FINGER_CTL1_WB__LOCK_RES_HYST_MASK        0x02
#define FFE_FINGER_CTL1_WB__LOCK_EN_MASK              0x01
//MSM5000, demodulator, addr 0x074, change from 7 to 8 bit long.
#define FFE_FINGER_CTL2_WB                      HW_FFE_FINGER_CTL2
#define FFE_FINGER_CTL2_WB_MASK                 0x0ff
#define FFE_FINGER_CTL2_WB__FN_INIT_MODE_MASK   0x080 //MSM5000, new bit
#define FFE_FINGER_CTL2_WB__Fn_PN_HOLD_MASK     0x040
#define FFE_FINGER_CTL2_WB__Fn_NO_DITHER_MASK   0x020
#define FFE_FINGER_CTL2_WB__TT_ADVANCE_MASK     0x010
#define FFE_FINGER_CTL2_WB__TT_RETARD_MASK      0x08
#define FFE_FINGER_CTL2_WB__TT_EN_MASK  0x01
#define FFE_FINGER_PAGE_REG_WB                  HW_FFE_FINGER_PAGE_REG
#define FFE_FINGER_PAGE_REG_WB_MASK             0x0f
#define FFE_TT_ACCUM_LOWER_WB                   HW_FFE_TT_ACCUM_LOWER
#define FFE_TT_ACCUM_UPPER_WB                   HW_FFE_TT_ACCUM_UPPER
#define FFE_TT_K1_GAIN_WB                       HW_FFE_TT_K1_GAIN
#define FFE_TT_K2_GAIN_WB                       HW_FFE_TT_K2_GAIN
#define FFE_RSSI_FILT_GAIN_WB                   HW_FFE_RSSI_FILT_GAIN
#define FN_LOCK_RSSI_LOWER_RB                   HW_FFE_RSSI_FILT_GAIN
#define FFE_FN_RSSI_INIT_WB                     HW_FFE_FN_RSSI_INIT
#define FFE_FN_BINARY_OFFSET_LOWER_WB           HW_FFE_FN_BINARY_OFFSET_LOWER
#define FFE_FN_BINARY_OFFSET_UPPER_WB           HW_FFE_FN_BINARY_OFFSET_UPPER
#define FFE_FN_BINARY_OFFSET_UPPER_WB_MASK              0x07f
#define FFE_FN_PN_I_MASK_LOWER_WB               HW_FFE_FN_PN_I_MASK_LOWER
#define FFE_FN_PN_I_MASK_UPPER_WB               HW_FFE_FN_PN_I_MASK_UPPER
#define FFE_FN_PN_I_MASK_UPPER_WB_MASK          0x07f
#define FFE_FN_PN_Q_MASK_LOWER_WB               HW_FFE_FN_PN_Q_MASK_LOWER
#define FFE_FN_PN_Q_MASK_UPPER_WB               HW_FFE_FN_PN_Q_MASK_UPPER
#define FFE_FN_PN_Q_MASK_UPPER_WB_MASK          0x07f
#define FFE_FN_CODE_CHAN_ID_WB                  HW_FFE_FN_CODE_CHAN_ID
#define FFE_SLEW_VALUE_LOWER_WB                 HW_FFE_SLEW_VALUE_LOWER
#define FFE_SLEW_VALUE_MID_WB                   HW_FFE_SLEW_VALUE_MID
#define FFE_SLEW_VALUE_UPPER_WB                 HW_FFE_SLEW_VALUE_UPPER
#define FFE_SLEW_VALUE_UPPER_WB_MASK            0x07
#define FFE_PILOT_FILT_GAIN_WB                  HW_FFE_PILOT_FILT_GAIN
#define FFE_LOCK_THRESH_LOWER_WB                HW_FFE_LOCK_THRESH_LOWER
#define FFE_LOCK_THRESH_UPPER_WB                HW_FFE_LOCK_THRESH_UPPER
#define FFE_FN_PWR_THRESH_WB                    HW_FFE_FN_PWR_THRESH
#define FREQ_COMB_GAIN_WB                       HW_FREQ_COMB_GAIN
#define FFE_FN_SUP_NUM_WB                       HW_FN_CHAN_CTL  //MSM5000, Demodulator, 8 bits long
#define FFE_FN_SUP_NUM_WB_MASK          0x07
//MSM5000, replaces COMB_SUP_NUM, addr 0x0DC, 8 bits long
#define COMB_CHAN_CTL_WB                        HW_COMB_CHAN_CTL
#define COMB_CHAN_CTL_WB_MASK                   0x0ff
//MSM5000, Some of the bits change function, addr 0x0E0, 8 bits long
#define POWER_COMB_CTL_WB                       HW_POWER_COMB_CTL
#define POWER_COMB_CTL_WB_MASK                  0x0fc
#define POWER_COMB_CTL_WB__POWER_CTL_PCT_MASK   0x080 //MSM5000, Set to '1' when receiving RC 2. Clear otherwise
#define POWER_COMB_CTL_WB__PUNC_POS_SEL_MASK    0x040 //MSM5000, new bit
#define POWER_COMB_CTL_WB__PC_EVEN_MASK         0x020 //MSM5000, new bit
#define POWER_COMB_CTL_WB__ERASE_EN_MASK        0x08
#define POWER_COMB_CTL_WB__PWR_CTL_EN_MASK      0x04
//MSM5000, new bits, addr 0x0E4, 10 bits long
#define SYMB_COMB_CTL0_WH                       HW_SYMB_COMB_CTL0
#define SYMB_COMB_CTL0_WH_MASK                           0x03ff
#define SYMB_COMB_CTL0_WH__PN_ROLL_GEN_MASK              0x0200 //MSM5000, new bit
#define SYMB_COMB_CTL0_WH__PN_ROLL_CTL_MASK              0x0100 //MSM5000, new bit
#define SYMB_COMB_CTL0_WH__COMB_128_MASK                 0x080  //MSM5000, new bit
#define SYMB_COMB_CTL0_WH__ULPN_OPTION_MASK              0x040  //MSM5000, new bit
#define SYMB_COMB_CTL0_WH__POSITION_MUX_MASK             0x020
#define SYMB_COMB_CTL0_WH__SYMB_COMB_FIN_EN_n_MASK       0x01e
#define SYMB_COMB_CTL0_WH__SLAM_EN_MASK                  0x01
//MSM5000, new bits, addr 0x0E8, 12 bit long
#define SYMB_COMB_CTL1_WH                       HW_SYMB_COMB_CTL1
#define SYMB_COMB_CTL1_WH_MASK                           0x0fff
#define SYMB_COMB_CTL1_WH__BETA_OVERWRITE_MASK           0x0800 //MSM5000, new bit
#define SYMB_COMB_CTL1_WH__DESKEW_OFFSET_MASK            0x0700 //MSM5000, new bits
#define SYMB_COMB_CTL1_WH__IQ_CAPTURE_EN_MASK            0x080
#define SYMB_COMB_CTL1_WH__LOCK_RSSI_EXT_RANGE_MASK      0x040
#define SYMB_COMB_CTL1_WH__COMBINER_DISABLE_MASK         0x020
#define SYMB_COMB_CTL1_WH__EN_DDS_MASK                   0x010
#define SYMB_COMB_CTL1_WH__80MS_RESET_CTL_MASK           0x0c
#define SYMB_COMB_CTL1_WH__SYMB_RATE_MASK                0x02
#define DEM_PEEK_POKE_CTL_WB                    HW_DEM_PEEK_POKE_CTL
#define DEM_PEEK_POKE_CTL_WB_MASK               0x0f
#define DEM_PEEK_POKE_CTL_WB__DEM_POKE_MASK     0x08
#define DEM_PEEK_POKE_CTL_WB__DEM_PEEK_POKE_SEL_MASK    0x07
#define SYMB_COMB_FIFO_DEPTH_WB                 HW_SYMB_COMB_FIFO_DEPTH


#define SYMB_COMB_POS_DUMP_WB                   HW_SYMB_COMB_POS_DUMP

#define SYMB_COMB_FREQ_ADJ_LOWER_WB             HW_SYMB_COMB_FREQ_ADJ_LOWER
#define SYMB_COMB_FREQ_ADJ_UPPER_WB             HW_SYMB_COMB_FREQ_ADJ_UPPER
#define SYMB_COMB_LONG_CODE_LD_0_WB             HW_SYMB_COMB_LONG_CODE_LD_0
#define SYMB_COMB_LONG_CODE_LD_1_WB             HW_SYMB_COMB_LONG_CODE_LD_1
#define SYMB_COMB_LONG_CODE_LD_2_WB             HW_SYMB_COMB_LONG_CODE_LD_2
#define SYMB_COMB_LONG_CODE_LD_3_WB             HW_SYMB_COMB_LONG_CODE_LD_3
#define SYMB_COMB_LONG_CODE_LD_4_WB             HW_SYMB_COMB_LONG_CODE_LD_4
#define SYMB_COMB_LONG_CODE_LD_5_WB             HW_SYMB_COMB_LONG_CODE_LD_5
#define SYMB_COMB_LONG_CODE_LD_5_WB_MASK                0x03
#define SYMB_COMB_LONG_CODE_MASK_0_WB           HW_SYMB_COMB_LONG_CODE_MASK_0
#define SYMB_COMB_LONG_CODE_MASK_1_WB           HW_SYMB_COMB_LONG_CODE_MASK_1
#define SYMB_COMB_LONG_CODE_MASK_2_WB           HW_SYMB_COMB_LONG_CODE_MASK_2
#define SYMB_COMB_LONG_CODE_MASK_3_WB           HW_SYMB_COMB_LONG_CODE_MASK_3
#define SYMB_COMB_LONG_CODE_MASK_4_WB           HW_SYMB_COMB_LONG_CODE_MASK_4
#define SYMB_COMB_LONG_CODE_MASK_5_WB           HW_SYMB_COMB_LONG_CODE_MASK_5
#define SYMB_COMB_LONG_CODE_MASK_5_WB_MASK              0x03
#define EPOCH_WR_WB                             HW_EPOCH_WR
#define EPOCH_WR_WB_MASK                        0x0ffff
//MSM5000, Demodulator, addr 0x134, 7 bits long, replaces Fn_SUP_CODE1
#define FN_CODE1_WB                             HW_FFE_FN_SUP_CODE1
#define FN_CODE1_WB_MASK                        0x07f
//MSM5000, Demodulator, addr 0x138, 7 bits long, replaces Fn_SUP_CODE2
#define FN_CODE2_WB                             HW_FFE_FN_SUP_CODE2
#define FN_CODE2_WB_MASK                        0x07f
//MSM5000, Demodulator, addr 0x13C, 7 bits long, replaces Fn_SUP_CODE3
#define FN_CODE3_WB                             HW_FFE_FN_SUP_CODE3
#define FN_CODE3_WB_MASK                        0x07f

#define FN_CODE4_WB                     HW_FFE_FN_SUP_CODE4
#define FN_CODE4_WB_MASK                0x03f
#define FN_CODE5_WB                     HW_FFE_FN_SUP_CODE5
#define FN_CODE5_WB_MASK                0x03f
#define FN_CODE6_WB                     HW_FFE_FN_SUP_CODE6
#define FN_CODE6_WB_MASK                0x03f
#define FN_CODE7_WB                     HW_FFE_FN_SUP_CODE7
#define FN_CODE7_WB_MASK                0x03f

//MSM5000, Demodulator, Bit [5:0] deleted, addr 0x15C, 8 bits long
#define DEM_FRAME_OFF_WB                        HW_DEM_FRAME_CTL
#define DEM_FRAME_OFF_WB_MASK                      0x0ff
#define DEM_FRAME_OFF_WB__SET_ROLL_SYNC_MASK       0x080
#define DEM_FRAME_OFF_WB__FRAME_SYNC_BYPASS_MASK   0x040
/*MSM5000, FRAME_OFF_MASK deleted, Demod will now use FWD_OFF[3:0]
as frame offset */

//MSM5000, Demodulator, New bits, addr 0x0154, 6 bits long
#define DEM_CTL_WB                              HW_DEM_CTL
#define DEM_CTL_WB_MASK                         0x03f
#define DEM_CTL_WB__DSP_BYPASS_MASK             0x020 //MSM5000, New bit
#define DEM_CTL_WB__DSP_SUP_OFF_MASK            0x010 //MSM5000, New bit
#define DEM_CTL_WB__MAC_EN_MASK                 0x08  //MSM5000, New bit
#define DEM_CTL_WB__Q_CHAN_OFF_MASK             0x04  //MSM5000, New bit
#define DEM_CTL_WB__I_CHAN_OFF_MASK             0x02  //MSM5000, New bit
#define DEM_CTL_WB__SYNC_BYPASS_MASK            0x01  //Unchanged

//MSM5000, Combiner, New register, Quick paging OOK control register
//addr 158, 10 bits long
#define OOK_CTL_WH                              HW_OOK_CTL
#define OOK_CTL_WH_MASK                         0x03ff
#define OOK_CTL_WH__OOK_POSITION_MASK           0x03fe
#define OOK_CTL_WH__OOK_EN_MASK                 0x0001

//MSM5000, Demodulator, addr 0x15c, 5 bits long
#define DEM_CH2_SPR_WB                          HW_DEM_CH2_SPR
#define DEM_CH2_SPR_WB__MASK                    0x01f

#define FREQ_COMB_CTL_WB                        HW_FREQ_COMB_CTL
#define FREQ_COMB_CTL_WB_MASK                   0x07c
#define FREQ_COMB_CTL_WB__CROSS_MODE_MASK       0x060
#define FREQ_COMB_CTL_WB__TRK_LO_ADJ_EN_MASK    0x010
#define FREQ_COMB_CTL_WB__TRK_LO_ADJ_POLARITY_MASK      0x08
#define FREQ_COMB_CTL_WB__CARRIER_FREQ_TRACK_EN_MASK    0x04
#define CARRIER_FREQ_ERR_WR_LSB_WB              HW_CARRIER_FREQ_ERR_WR_LSB
#define CARRIER_FREQ_ERR_WR_MSB_WB              HW_CARRIER_FREQ_ERR_WR_MSB

//MSM5000, TXAGC, new register. addr 0x16c, 9 bits long
#define TX_RATE_ADJ_WH                          HW_TX_RATE_ADJ
#define TX_RATE_ADJ_WH_MASK                             0x01ff

//MSM5000, TXAGC, new register. addr 0x170, 8 bits long
#define TX_PDM_BOOST_PERIOD_WB                  HW_TX_PDM_BOOST_PERIOD
#define TX_PDM_BOOST_PERIOD_WB_MASK             0x0ff

//MSM5000, TXAGC, new register. addr 0x20c, 6 bits long
#define PA_DELAY_WB                             HW_PA_DELAY
#define PA_DELAY_WB_MASK                        0x03f

//MSM5000, TXAGC, new register. addr 0x210, 9 bits long
#define PA_OFFSET0_WH                           HW_PA_OFFSET0
#define PA_OFFSET0_WH_MASK                      0x01ff

//MSM5000, TXAGC, new register. addr 0x214, 9 bits long
#define PA_OFFSET1_WH                           HW_PA_OFFSET1
#define PA_OFFSET1_WH_MASK                      0x01ff

//MSM5000, TXAGC, new register. addr 0x218, 9 bits long
#define PA_OFFSET2_WH                           HW_PA_OFFSET2
#define PA_OFFSET2_WH_MASK                      0x01ff

//MSM5000, TXAGC, new register. addr 0x21c, 9 bits long
#define PA_OFFSET3_WH                           HW_PA_OFFSET3
#define PA_OFFSET3_WH_MASK                      0x01ff

#define I_OFFSET_WR_WB                          HW_I_OFFSET_WR
#define Q_OFFSET_WR_WB                          HW_Q_OFFSET_WR
#define IQ_OFFSET_CTL_WB                        HW_IQ_OFFSET_CTL
#define IQ_OFFSET_CTL_WB__DC_LOOP_GAIN_MASK             0x080
#define IQ_OFFSET_CTL_WB__Q_OFFSET_POLARITY_MASK        0x040
#define IQ_OFFSET_CTL_WB__IQ_OFFSET_EN_MASK             0x020
#define IQ_OFFSET_CTL_WB__I_OFFSET_POLARITY_MASK        0x08
#define IQ_OFFSET_CTL_WB__I_OFFSET_OVERRIDE_N_MASK      0x02
#define IQ_OFFSET_CTL_WB__Q_OFFSET_OVERRIDE_N_MASK      0x01
#define RAS_RAM_DATA_MSB_WB                     HW_RAS_RAM_DATA_MSB
#define RAS_RAM_DATA_LSB_WB                     HW_RAS_RAM_DATA_LSB
#define AGC_TEST_CTL_WB                         HW_AGC_TEST_CTL
#define AGC_TEST_CTL_WB_MASK                    0x00
#define AGC_RDWR_CTL_WB                         HW_AGC_RDWR_CTL
#define AGC_RDWR_CTL_WB_MASK                    0x0f
#define AGC_RDWR_CTL_WB__AGC_VALUE_OVERRIDE_MASK         0x08
#define AGC_RDWR_CTL_WB__TX_OPEN_LOOP_OVERRIDE_MASK      0x04
#define AGC_RDWR_CTL_WB__AGC_LOW_HIGH_N_MASK             0x02
#define AGC_RDWR_CTL_WB__RAS_RAM_WR_MASK                 0x01
#define AGC_CTL_WB                              HW_AGC_CTL
#define AGC_CTL_WB__TX_GAIN_ADJ_STEP_SIZE_MASK           0x0c0
#define AGC_CTL_WB__LINEARIZER_RANGE_SEL_MASK            0x020
#define AGC_CTL_WB__TX_GAIN_CTL_LATCH_MASK               0x08
#define AGC_CTL_WB__RATCHET_MODE_MASK                    0x04
#define AGC_CTL_WB__RX_AGC_ADJ_OE_MASK                   0x02
#define AGC_CTL_WB__TX_AGC_ADJ_OE_MASK                   0x01
#define AGC_CTL2_WB                             HW_AGC_CTL2
#define AGC_CTL2_WB__RX_AGC_ADJ_POLARITY_MASK            0x080
#define AGC_CTL2_WB__TX_AGC_ADJ_POLARITY_MASK            0x040
#define AGC_CTL2_WB__LNA_RANGE_POLARITY_MASK             0x020
#define AGC_CTL2_WB__PA_R_POLARITY_MASK                  0x010
#define AGC_CTL2_WB__RX_AGC_ADJ_OVERRIDE_N_MASK          0x08
#define AGC_CTL2_WB__TX_AGC_ADJ_OVERRIDE_N_MASK          0x04
#define AGC_CTL2_WB__LNA_RANGE_OVERRIDE_N_MASK           0x02
#define AGC_CTL2_WB__PA_R_OVERRIDE_N_MASK                0x01
#define RX_AGC_ADJ_WR_WB                        HW_RX_AGC_ADJ_WR
#define TX_AGC_ADJ_WR_MSB_WB                    HW_TX_AGC_ADJ_WR_MSB
#define AGC_VALUE_WR_WB                         HW_AGC_VALUE_WR
#define AGC_VALUE_MAX_WB                        HW_AGC_VALUE_MAX
#define AGC_VALUE_MAX_WB__AGC_VALUE_MAX_EN_MASK          0x080
#define AGC_VALUE_MAX_WB__AGC_VALUE_MAX_MASK             0x07f
#define AGC_VALUE_MIN_WB                        HW_AGC_VALUE_MIN
#define AGC_VALUE_MIN_WB__AGC_VALUE_MIN_EN_MASK          0x080
#define AGC_VALUE_MIN_WB__AGC_VALUE_MIN_MASK             0x07f

//MSM5000, TXAGC, New bits, addr 0x1AC, 9 bits long
#define AGC_CTL3_WH                             HW_AGC_CTL3
#define AGC_CTL3_WH_MASK                                 0x01ff
#define AGC_CTL3_WH__TX_RAM_SIZE_MASK                    0x0100 //MSM5000, New bit
#define AGC_CTL3_WH__TX_RAM_SIZE_VAL                     0x0100 //MSM5000, New bit
#define AGC_CTL3_WH__MASK_DATA_DELAY_MASK                0x080  //MSM5000, New bit
#define AGC_CTL3_WH__PA_RANGE_UPDATE_MASK                0x040  //MSM5000, New bit
#define AGC_CTL3_WH__PA_RANGE_UPDATE_VAL                 0x040  //MSM5000, New bit
#define AGC_CTL3_WH__TX_AGC_ADJ_WR_LSB_MASK              0x020
#define AGC_CTL3_WH__LNA_RANGE_WR_MASK                   0x010
#define AGC_CTL3_WH__PA_R_WR_MASK                        0x0c
#define AGC_CTL3_WH__LNA_OFFSET_MSB_MASK                 0x03

#define AGC_CTL4_WB                             HW_AGC_CTL4
#define AGC_CTL4_WB__GAIN_CONSTANT_IM_MASK      0x0c0
#define AGC_CTL4_WB__GAIN_CONSTANT_MASK         0x03c
#define AGC_CTL4_WB__TX_GAIN_LIMIT_MSB_MASK     0x03
#define TX_GAIN_LIMIT_LSB_WB                    HW_TX_GAIN_LIMIT_LSB
#define LNA_OFFSET_LSB_WB                       HW_LNA_OFFSET_LSB
#define IM_STATE_CTL_WB                         HW_IM_STATE_CTL
#define IM_STATE_CTL_WB__NONBYPASS_TIMER_MASK   0x0e0
#define IM_STATE_CTL_WB__BYPASS_TIMER_MASK      0x01f
#define LNA_FALL_WB                             HW_LNA_FALL
#define LNA_RISE_WB                             HW_LNA_RISE
#define LNA_FILT_WR_WB                          HW_LNA_FILT_WR
#define AGC_CTL5_WB                             HW_AGC_CTL5
#define AGC_CTL5_WB__LNA_FILT_OVERRIDE_MASK              0x080
#define AGC_CTL5_WB__LNA_RANGE_FILT_SEL_MASK             0x040
#define AGC_CTL5_WB__LNA_GAIN_FILT_SEL_MASK              0x020
#define AGC_CTL5_WB__LNA_RANGE_FORCE_OVERRIDE_MASK       0x010
#define AGC_CTL5_WB__LNA_RANGE_FORCE_MASK                0x08
#define AGC_CTL5_WB__LNA_GAIN_OVERRIDE_MASK              0x04
#define AGC_CTL5_WB__LNA_GAIN_POLARITY_MASK              0x02
#define AGC_CTL5_WB__LNA_GAIN_OE_MASK                    0x01
#define AGC_CTL6_WB                             HW_AGC_CTL6
#define AGC_CTL6_WB_MASK                        0x0f
#define AGC_CTL6_WB__LNA_GAIN_SEL_MASK          0x08
#define AGC_CTL6_WB__LNA_LIN_SEL_MASK           0x04
#define AGC_CTL6_WB__LNA_FILT_BW_MASK           0x03
#define LNA_GAIN_WR_WB                          HW_LNA_GAIN_WR
#define LNA_GAIN_MIN_WB                         HW_LNA_GAIN_MIN
#define LNA_GAIN_PWR_MAX_WB                     HW_LNA_GAIN_PWR_MAX
#define TX_OPEN_LOOP_WR_WB                      HW_TX_OPEN_LOOP_WR
#define TX_GAIN_ADJ_WR_WB                       HW_TX_GAIN_ADJ_WR
#define PA_R1_RISE_WB                           HW_PA_R1_RISE
#define PA_R1_FALL_WB                           HW_PA_R1_FALL
#define PA_R2_RISE_WB                           HW_PA_R2_RISE
#define PA_R2_FALL_WB                           HW_PA_R2_FALL
#define PA_R3_RISE_WB                           HW_PA_R3_RISE
#define PA_R3_FALL_WB                           HW_PA_R3_FALL
#define PA_R_MAP_WB                             HW_PA_R_MAP
#define AGC_CTL7_WB                             HW_AGC_CTL7
#define AGC_CTL7_WB_MASK                        0x03
#define AGC_CTL7_WB__PA_R_MAP_EN_MASK           0x02
#define AGC_CTL7_WB__TEMPORAL_HYST_MASK         0x01
#define PA_R_TIMER_WB                           HW_PA_R_TIMER

//MSM5000, Combiner, New register, addr 0x820, 11 bits long
#define TIME_INT1_MASK_WH                       HW_TIME_INT1_MASK
#define TIME_INT1_MASK_WH_MASK                  0x07ff
#define TIME_INT1_MASK_WH__5MS_MASK_MASK        0x0780
#define TIME_INT1_MASK_WH__OFFSET_PCG_MASK      0x060
#define TIME_INT1_MASK_WH__OFFSET_SYMB_MASK     0x01f
//MSM5000, Combiner, New register, addr 0x824, 11 bits long
#define TIME_INT2_MASK_WH                       HW_TIME_INT2_MASK
#define TIME_INT2_MASK_WH_MASK                  0x07ff
#define TIME_INT2_MASK_WH__5MS_MASK_MASK        0x0780
#define TIME_INT2_MASK_WH__OFFSET_PCG_MASK      0x060
#define TIME_INT2_MASK_WH__OFFSET_SYMB_MASK     0x01f

//MSM5000, Demodulator, New Register, addr 0x82C, 11 bits long
#define FN_MAC_I_PILOT_WH                       HW_FN_MAC_I_PILOT
#define FN_MAC_I_PILOT_WH_MASK                  0x07ff

//MSM5000, Demodulator, New Register, addr 0x830, 11 bits long
#define FN_MAC_Q_PILOT_WH                       HW_FN_MAC_Q_PILOT
#define FN_MAC_Q_PILOT_WH_MASK                  0x07ff

//MSM5000, Demodulator, New Register, addr 0x834, 4 bits long
#define MAC_COMB_LOCK_WB                        HW_MAC_COMB_LOCK
#define MAC_COMB_LOCK_WB_MASK                   0x0f

//MSM5000, Combiner, New register, addr 0x828, 12 bits long
#define MAC_RND_WH                              HW_MAC_RND
#define MAC_RND_WH_MASK                         0x0fff
#define MAC_RND_WH__COMB_RND3_MASK              0x0f00
#define MAC_RND_WH__COMB_RND2_MASK              0x00f0
#define MAC_RND_WH__COMB_RND1_MASK              0x000f

//MSM5000, Forward power control, new register, addr 0x838, 16 bits long
#define FPC_CTL_WH                              HW_FPC_CTL
#define FPC_CTL_WH_MASK                         0xffff
#define FPC_CTL_WH__FPC_ACC_SCH_POS_MASK        0xf800
#define FPC_CTL_WH__FPC_POSITION2_MASK          0x07c0
#define FPC_CTL_WH__FPC_POSITION_MASK           0x003e
#define FPC_CTL_WH__FPC_EN_MASK                 0x0001

//MSM5000, Forward power control bit channel select, addr 0x83C, 16 bits long
#define FPC_SEL_WH                              HW_FPC_SEL
#define FPC_SEL_WH_MASK                         0x0ffff

//MSM5000, Forward power control bit filter gain, addr 0x840, 8 bits long
#define PC_FILT_GAIN_WB                         HW_PC_FILT_GAIN
#define PC_FILT_GAIN_WB_MASK                    0x0ff

//MSM5000, Forward power control bit filter gain for up decision,
// addr 0x844, 8 bits long
#define PC_FILT_GAIN_UP_WB                      HW_PC_FILT_GAIN_UP
#define PC_FILT_GAIN_UP_WB_MASK                 0x0ff

//MSM5000, Forward power control bit filter gain for down decision,
// addr 0x848, 8 bits long
#define PC_FILT_GAIN_DOWN_WB                    HW_PC_FILT_GAIN_DOWN
#define PC_FILT_GAIN_DOWN_WB_MASK               0x0ff

//MSM5000, Forward link Nt/Io filter gain, addr 0x84C, 8 bits long
#define NT_IO_FILT_GAIN_WB                      HW_NT_IO_FILT_GAIN
#define NT_IO_FILT_GAIN_WB_MASK                 0x0ff

//MSM5000, FCH Eb/No target, addr 0x850, 16 bits long
#define EB_NO_TARGET_FCH_WH                     HW_EB_NO_TARGET_FCH
#define EB_NO_TARGET_FCH_WH_MASK                0x0ffff

//MSM5000, SCH Eb/No target, addr 0x854, 16 bits long
#define EB_NO_TARGET_SCH_WH                     HW_EB_NO_TARGET_SCH
#define EB_NO_TARGET_SCH_WH_MASK                0x0ffff

//MSM5000, SCH based scaling factor for Eb/No target, addr 0x858, 8 bits long
#define WALSH_GAIN_WB                           HW_WALSH_GAIN
#define WALSH_GAIN_WB_MASK                      0x0ff

//MSM5000, FPC_MODE and beta settings, addr 0x85C, 1 bits long
#define BETA_CTL_WH                             HW_BETA_CTL
#define BETA_CTL_WH_MASK                        0x0400
#define BETA_CTL_WH__BETA_MODE_MASK             0x0400

//MSM5000, Frame based information for SCH scaling factor,
// addr 0x860, 16 bits long
#define SW_BETA_TURBO_WH                        HW_SW_BETA_TURBO
#define SW_BETA_TURBO_WH_MASK                   0x0ffff

//MSM5000, FCH beta scaling factor, addr 0x864, 8 bits long
#define SW_BETA_FCH_WB                          HW_SW_BETA_FCH
#define SW_BETA_FCH_WB_MASK                     0x0ff

//MSM5000, SCH beta scaling factor, addr 0x868, 8 bits long
#define SW_BETA_SCH_WB                          HW_SW_BETA_SCH
#define SW_BETA_SCH_WB_MASK                     0x0ff

#define Fn_CH0_QOF_SEL_WH                       HW_FN_CH0_QOF_SEL //MSM5000, New demodulator register, 9 bits long
#define Fn_CH0_QOF_SEL_WH__MASK_SEL_MASK        0x0300
#define Fn_CH0_QOF_SEL_WH__WALSH_ROT_MASK       0x0ff
#define Fn_CH1_QOF_SEL_WH                       HW_FN_CH1_QOF_SEL //MSM5000, New demodulator register, 9 bits long
#define Fn_CH1_QOF_SEL_WH__MASK_SEL_MASK        0x0300
#define Fn_CH1_QOF_SEL_WH__WALSH_ROT_MASK       0x0ff
#define Fn_CH2_QOF_SEL_WH                       HW_FN_CH2_QOF_SEL //MSM5000, New demodulator register, 9 bits long
#define Fn_CH2_QOF_SEL_WH__MASK_SEL_MASK        0x0300
#define Fn_CH2_QOF_SEL_WH__WALSH_ROT_MASK       0x0ff

//MSM5000, OTD config, new register, addr 0x890, 3 bits long
#define Fn_OTD_CFG_WB                           HW_FN_OTD_CFG
#define Fn_OTD_CFG_WB__FCH_QOF_MASK             0x04
#define Fn_OTD_CFG_WB__SCH_QOF_MASK             0x02

//MSM5000, default value for CH2 IQ accumulation, addr 0x878, 16 bits long
#define DEFAULT_IQ_ACC_WH                       HW_DEFAULT_IQ_ACC
#define DEFAULT_IQ_ACC_WH_MASK                  0x0ffff

//MSM5000, CH2 IQ accumulatoin filter gain for Beta scaling,
// addr 0x87C, 8 bits long
#define BETA_IQ_FILT_GAIN_WB                    HW_BETA_IQ_FILT_GAIN
#define BETA_IQ_FILT_GAIN_WB_MASK               0x0ff

//MSM5000, CH2 Nt/Io filter gain for DSP Beta computation,
// addr 0x880, 8 bits long
#define BETA_NT_IO_FILT_GAIN_WB                 HW_BETA_NT_IO_FILT_GAIN
#define BETA_NT_IO_FILT_GAIN_WB_MASK            0x0ff

//MSM5000, Reverse power control, new register, addr 0x888, 2 bits long
#define DEM_REV_PWR_CONTROL_DELAY_WB            HW_DEM_REV_PWR_CONTROL_DELAY
#define DEM_REV_PWR_CONTROL_DELAY_WB_MASK       0x07

#define DEM_PILOT_GATING_MASK_WB                HW_DEM_PILOT_GATING_MASK
#define DEM_CTL2_WH                             HW_DEM_CTL2


//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     DEMODULATOR READ REGISTERS
//-----------------------------------------------------------------------------
#define SRCH_DMA_ERROR_RB                       HW_SRCH_DMA_ERROR
#define SRCH_DMA_ERROR_RB_MASK                  0x00

#define SRCH_POSITION_LOW_RH                    HW_SRCH_POSITION_LOW

#define SRCH_POSITION_HIGH_RB                   HW_SRCH_POSITION_HIGH
#define SRCH_POSITION_HIGH_RB_MASK              0x03

#define SRCH_DMA_DATA_RB                        HW_SRCH_DMA_DATA

#define SRCH_MAX_ENERGY_RH                      HW_SRCH_MAX_ENERGY
#define SRCH_MAX_INDEX_RH                       HW_SRCH_MAX_INDEX

//MS5000, Demodulator, new register, addr 0x05C, 16 bits long
#define DEM_DSP_INT_WORD_RH                     HW_DEM_DSP_INT_WORD
#define DEM_DSP_INT_WORD_RH_MASK                0x0ffff

#define FN_LOCK_RSSI_RH                         HW_FN_LOCK_RSSI

#define FN_POSITION_LOWER_RH                    HW_FN_POSITION_LOWER
#define FN_POSITION_UPPER_RB                    HW_FN_POSITION_UPPER

//MSM5000, Demodulator, new register, addr 0x0BC, 16 bits long
#define EPOCH_TIMER_LOWER_RH                    HW_EPOCH_TIMER_LOWER
#define EPOCH_TIMER_LOWER_RH_MASK               0x0ffff

//MSM5000, Demodulator, new register, addr 0x0C0, 2 bits long
#define EPOCH_TIMER_UPPER_RB                    HW_EPOCH_TIMER_UPPER
#define EPOCH_TIMER_UPPER_RB_MASK               0x03


#define FN_POSITION_UPPER_RB_MASK               0x03
//MSM5000, Combiner, new bits, addr 0x0E4, 7 bits long
#define SYMB_COMB_STATUS0_RB                    HW_SYMB_COMB_STATUS0
#define SYMB_COMB_STATUS0_RB_MASK                       0x07f  //MSM5000, 1:0, new bits
#define SYMB_COMB_STATUS0_RB__LOCK_STATE_F3_MASK        0x040
#define SYMB_COMB_STATUS0_RB__LOCK_STATE_F2_MASK        0x020
#define SYMB_COMB_STATUS0_RB__LOCK_STATE_F1_MASK        0x010
#define SYMB_COMB_STATUS0_RB__LOCK_STATE_F0_MASK        0x08
#define SYMB_COMB_POSITION2_LOWER_RB            HW_SYMB_COMB_POSITION2_LOWER
#define SYMB_COMB_POSITION2_UPPER_RB            HW_SYMB_COMB_POSITION2_UPPER
#define SYMB_COMB_LONG_CODE_RD_0_RB             HW_SYMB_COMB_LONG_CODE_RD_0
#define SYMB_COMB_LONG_CODE_RD_1_RB             HW_SYMB_COMB_LONG_CODE_RD_1
#define SYMB_COMB_LONG_CODE_RD_2_RB             HW_SYMB_COMB_LONG_CODE_RD_2
#define SYMB_COMB_LONG_CODE_RD_3_RB             HW_SYMB_COMB_LONG_CODE_RD_3
#define SYMB_COMB_LONG_CODE_RD_4_RB             HW_SYMB_COMB_LONG_CODE_RD_4
#define SYMB_COMB_LONG_CODE_RD_5_RB             HW_SYMB_COMB_LONG_CODE_RD_5
#define SYMB_COMB_LONG_CODE_RD_5_RB_MASK                0x03

//MSM5000, Demodulator, New register, addr 0x120, 11 bits long
#define QP_I_RD_RH                              HW_QP_I_RD
#define QP_I_RD_RH_MASK                         0x07ff

//MSM5000, Demodulator, New register, addr 0x128, 11 bits long
#define QP_Q_RD_RH                              HW_QP_Q_RD
#define QP_Q_RD_RH_MASK                         0x07ff

//MSM5000, Demodulator, New register, addr 0x124, 11 bits long
#define QP_I_TH1_RH                             HW_QP_TH1_RD
#define QP_I_TH1_RH_MASK                        0x07ff

//MSM5000, Demodulator, New register, addr 0x12C, 11 bits long
#define QP_I_TH2_RH                             HW_QP_TH2_RD
#define QP_I_TH2_RH_MASK                        0x07ff

//MSM5000, combiner, New register, addr 0x0130, 8 bits long
#define TIME_INT_PHASE_RB                       HW_TIME_INT_PHASE
#define TIME_INT_PHASE_RB_MASK                  0x0ff
#define TIME_INT_PHASE_RB__TIME_INT2_PHASE_MASK 0x0f0
#define TIME_INT_PHASE_RB__TIME_INT1_PHASE_MASK 0x00f

//MSM5000, Forward power control, new register, addr 0x140, 16 bits long
#define PC_ACC_RH                               HW_PC_ACC
#define PC_ACC_RH_MASK                          0x0ffff

//MSM5000, Forward power control, new register, addr 0x144, 16 bits long
#define NT_IO_ACC_RH                            HW_NT_IO_ACC
#define NT_IO_ACC_RH_MASK                       0x0ffff

//MSM5000, Forward power control, new register, addr 0x148, 16 bits long
#define FPC_HISTORY_RH                          HW_FPC_HISTORY
#define FPC_HISTORY_RH_MASK                     0x0ffff

//MSM5000, Forward power control, new register, addr 0x14C, 16 bits long
#define RPC_HISTORY_RH                          HW_RPC_HISTORY
#define RPC_HISTORY_RH_MASK                     0x0ffff

#define CARRIER_FREQ_ERR_RD_LSB_RB              HW_CARRIER_FREQ_ERR_RD_LSB
#define CARRIER_FREQ_ERR_RD_MSB_RB              HW_CARRIER_FREQ_ERR_RD_MSB
#define I_OFFSET_RD_RB                          HW_I_OFFSET_RD
#define Q_OFFSET_RD_RB                          HW_Q_OFFSET_RD
#define RATCHET_BIT_DIS_RB                      HW_RATCHET_BIT_DIS
#define RATCHET_BIT_DIS_RB_MASK                 0x00
#define RX_AGC_ADJ_RD_RB                        HW_RX_AGC_ADJ_RD
#define TX_AGC_ADJ_RD_RB                        HW_TX_AGC_ADJ_RD
#define AGC_VALUE_RD_RB                         HW_AGC_VALUE_RD
#define LNA_PA_RD_RB                            HW_LNA_PA_RD
#define LNA_PA_RD_RB_MASK                       0x07
#define LNA_PA_RD_RB__PA_R1_RD_MASK             0x04
#define LNA_PA_RD_RB__PA_R0_RD_MASK             0x02
#define LNA_PA_RD_RB__LNA_RANGE_RD_MASK         0x01
#define LNA_FILT_RD_RB                          HW_LNA_FILT_RD
#define LNA_GAIN_RD_RB                          HW_LNA_GAIN_RD
#define TX_OPEN_LOOP_RD_RB                      HW_TX_OPEN_LOOP_RD
#define TX_GAIN_ADJ_RD_RB                       HW_TX_GAIN_ADJ_RD
#define TX_GAIN_CTL_MSB_RB                      HW_TX_GAIN_CTL_MSB
#define TX_GAIN_CTL_MSB_RB_MASK                 0x03
#define TX_GAIN_CTL_LSB_RB                      HW_TX_GAIN_CTL_LSB
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     MODULATOR WRITE REGISTERS
//-----------------------------------------------------------------------------
#define MOD_RESET_WB                            HW_MOD_RESET
#define MOD_RESET_WB_MASK                       0x0ffff

#define MOD_PCH_GAIN_WB                         HW_MOD_PCH_GAIN
#define MOD_PCH_GAIN_WB_MASK                    0x0ff

//MSM5000, New register, addr 0x228, 16 bits long
#define MOD_SCH_FCH_GAIN_WH                     HW_MOD_SCH_FCH_GAIN
#define MOD_SCH_FCH_GAIN_WH_MASK                0x0ffff

#define MOD_FCH_PUNC_PATTERN_1_WH               HW_FCH_PUNC_PATTERN_1
#define MOD_FCH_PUNC_PATTERN_1_WH_MASK          0x0ffff

#define MOD_FCH_PUNC_PATTERN_0_WH               HW_FCH_PUNC_PATTERN_0
#define MOD_FCH_PUNC_PATTERN_0_WH_MASK          0x0ff00

#define MOD_SCH_PUNC_PATTERN_1_WH               HW_SCH_PUNC_PATTERN_1
#define MOD_SCH_PUNC_PATTERN_1_WH_MASK          0x0ff80

#define I_PN_STATE_0_WB                         HW_I_PN_STATE_0
#define I_PN_STATE_1_WB                         HW_I_PN_STATE_1
#define I_PN_STATE_1_WB_MASK                    0x07f
#define Q_PN_STATE_0_WB                         HW_Q_PN_STATE_0
#define Q_PN_STATE_1_WB                         HW_Q_PN_STATE_1
#define Q_PN_STATE_1_WB_MASK                    0x07f
#define U_PN_STATE_0_WB                         HW_U_PN_STATE_0
#define U_PN_STATE_1_WB                         HW_U_PN_STATE_1
#define U_PN_STATE_2_WB                         HW_U_PN_STATE_2
#define U_PN_STATE_3_WB                         HW_U_PN_STATE_3
#define U_PN_STATE_4_WB                         HW_U_PN_STATE_4
#define U_PN_STATE_5_WB                         HW_U_PN_STATE_5
#define U_PN_STATE_5_WB__BOZO_MASK              0x080
#define U_PN_STATE_5_WB__U_PN_STATE_41_40_MASK  0x03
#define U_PN_MASK_0_WB                          HW_U_PN_MASK_0
#define U_PN_MASK_1_WB                          HW_U_PN_MASK_1
#define U_PN_MASK_2_WB                          HW_U_PN_MASK_2
#define U_PN_MASK_3_WB                          HW_U_PN_MASK_3
#define U_PN_MASK_4_WB                          HW_U_PN_MASK_4
#define U_PN_MASK_5_WB                          HW_U_PN_MASK_5
#define U_PN_MASK_5_WB_MASK                     0x03
#define PA_WARMUP_WB                            HW_PA_WARMUP

#define WSYM_STATE_WB                           HW_WSYM_STATE
#define WSYM_STATE_WB__WSYM_CLK_WCHIP_MASK      0x0fc
#define WSYM_STATE_WB__WSYM_CLK_PN_MASK         0x03
#define TXSYNC_ST_0_WB                          HW_TXSYNC_ST_0
#define TXSYNC_ST_1_WB                          HW_TXSYNC_ST_1
#define TXSYNC_ST_1_WB_MASK                     0x03f
#define SYSFR_STATE_WB                          HW_SYSFR_STATE
#define SYSFR_STATE_WB_MASK                     0x03f
#define ENC_INT_ST_WB                           HW_ENC_INT_ST
#define ENC_INT_ST_WB_MASK                      0x07f
#define ENC_INT_ST_WB__ENC_INT_PCG_MASK         0x078
#define ENC_INT_ST_WB__ENC_INT_WSYM_MASK        0x07

#define MOD_FCH_ENC_DATA_WH                     HW_FCH_ENC_DATA
#define MOD_FCH_ENC_DATA_WH_MASK                0x0ffff

//MSM5000, New bits, Modulator, Addr 0x29c, 15 bits long.
#define MOD_FCH_CTL_WH                          HW_FCH_CTL
#define MOD_FCH_CTL_WH_MASK                     0x08f7f
#define MOD_FCH_CTL_WH__TX_TONE_EN_MASK         0x08000
#define MOD_FCH_CTL_WH__FCH_CRC_LENGTH_MASK     0x0f00
#define MOD_FCH_CTL_WH__FCH_CODE_RATE_MASK      0x060
#define MOD_FCH_CTL_WH__FCH_ENC_RATE_SET_MASK   0x010
#define MOD_FCH_CTL_WH__FCH_ENC_RATE_MASK       0x0C
#define MOD_FCH_CTL_WH__IS_95_C_MASK            0x02
#define MOD_FCH_CTL_WH__IS_95C_FCH_EN_MASK      0x01

#define MOD_SCH_ENC_DATA_WH                     HW_SCH_ENC_DATA
#define MOD_SCH_ENC_DATA_WH_MASK                0x0ffff

//MSM5000, New bits, Modulator, Addr 0x2bc, 16 bits long.
#define MOD_SCH_CTL_WH                          HW_SCH_CTL
#define MOD_SCH_CTL_WH_MASK                     0x07fff
#define MOD_SCH_CTL_WH__SCH_LTU_EN_MASK         0x08000
#define MOD_SCH_CTL_WH__TURBO_ENCODE_EN_MASK    0x04000
#define MOD_SCH_CTL_WH__SCH_CRC_LENGTH_MASK     0x01f00
#define MOD_SCH_CTL_WH__SCH_CODE_RATE_MASK      0x0c0
#define MOD_SCH_CTL_WH__SCH_ENC_RATE_MASK       0x037
#define MOD_SCH_CTL_WH__IS_95C_SCH_EN_MASK      0x01

//MSM5000, Modulator, New bits, addr 0x2a0, 16 bits long
#define FCH_CRC_POLY_WH                         HW_FCH_CRC_POLY
#define FCH_CRC_POLY_WH_MASK                    0x0ffff
//MSM5000, Modulator, New bits, addr 0x2a4, 16 bits long
#define SCH_CRC_POLY_WH                         HW_SCH_CRC_POLY
#define SCH_CRC_POLY_WH_MASK                    0x0ffff

//MSM5000, Modulator, New bits, addr 0x2a8, 9 bits long
#define MOD_CLK_CTL_WH                          HW_MOD_CLK_CTL
#define MOD_CLK_CTL_WB_MASK                     0x01f8
#define MOD_CLK_CTL_WB__TX_CLK_INVERT_MASK      0x0100
#define MOD_CLK_CTL_WB__TX_CLKS_DISABLE_MASK    0x080
#define MOD_CLK_CTL_WB__VOC_REF_DISABLE_MASK    0x040
#define MOD_CLK_CTL_WB__CODEC_CTL_MASK          0x038

//MSM5000, Modulator, New bits, addr 0x2aC, 9 bits long
#define MOD_MISC_CTL_WH                         HW_MOD_MISC_CTL
#define MOD_MISC_CTL_WH_MASK                       0x1fe3
#define MOD_MISC_CTL_WH__PILOT_GATE_ENA_MASK       0x1000
#define MOD_MISC_CTL_WH__FCH_EIGHTH_GATE_ENA_MASK  0x0800
#define MOD_MISC_CTL_WH__HHO_PRMBL_MASK            0x0400
#define MOD_MISC_CTL_WH__DCCH_EN_MASK              0x0200 
#define MOD_MISC_CTL_WH__IS95C_PCH_EN_MASK         0x0100
#define MOD_MISC_CTL_WH__IS95C_PCBIT_REG_EN_MASK   0x080
#define MOD_MISC_CTL_WH__TX_DATA_FMT_MASK          0x040
#define MOD_MISC_CTL_WH__IS_95A_ACCESS_CH_MASK     0x020
#define MOD_MISC_CTL_WH__PA_CTL_MASK               0x03

#define FRAME_OFF_WB                            HW_FRAME_OFF
#define FRAME_OFF_WB__FWD_OFF_MASK              0x0f0
#define FRAME_OFF_WB__REV_OFF_MASK              0x0f

#define BTF_CTL_WB                              HW_BTF_CTL
#define BTF_CTL_WB_MASK                         0x00
#define BTF_CTL_WB__BTF_LOAD_MODE_MASK          0x01

//MSM5000, Modulator, New bits, addr 0x2C0, 10 bits long
#define MOD_TEST_CTL_WH                         HW_MOD_TEST_CTL
#define MOD_TEST_CTL_WH_MASK                          0x03fe
#define MOD_TEST_CTL_WH__PA_STATE_TEST_DATA_MASK      0x0300 //New bit
#define MOD_TEST_CTL_WH__PA_STATE_TEST_EN_MASK        0x080  //New bit
#define MOD_TEST_CTL_WH__TX_SPECTRAL_INVERSION_MASK   0x040
#define MOD_TEST_CTL_WH__RX_SPECTRAL_INVERSION_MASK   0x020
#define MOD_TEST_CTL_WH__TX_DATA_CTL_MASK             0x010
#define MOD_TEST_CTL_WH__MASK_CTL_MASK                0x0c
#define MOD_TEST_CTL_WH__SET_IQ_HIGH_MASK             0x02

#define TX_I_CLK_WB                             HW_TX_I_CLK
#define TX_I_CLK_WB_MASK                        0x0ffff
#define TX_DATA_TEST_WB                         HW_TX_DATA_TEST

//MSM5000, Modulator, New register, addr 0x2CC, 16 bits long
#define MOD_PCBIT_TEST_DATA_WH                  HW_PCH_PCBIT_DATA
#define MOD_PCBIT_TEST_DATA_WH_MASK             0x0ffff

//MSM5000, Modulator, New register, addr 0x23C, 16 bits long
#define TX_EARLY_FRAME_CTL_WH                   HW_TX_EARLY_FRAME_CTL
#define TX_EARLY_FRAME_CTL_WH_MASK                       0x01ff
#define TX_EARLY_FRAME_CTL_WH__EARLY_FRAME_ADV_MASK      0x01fc
#define TX_EARLY_FRAME_CTL_WH__EARLY_FRAME_PERIOD_MASK   0x03
#define TX_EARLY_FRAME_CTL_WH__EARLY_FRAME_PERIOD_VAL    0x03

//MSM5000, Modulator, New register, addr 0x2D0, 9 bits long
#define MOD_SCH_LTU_CTL_WH                      HW_MOD_SCH_LTU_CTL
#define MOD_SCH_LTU_CTL_WH_MASK                 0x01ff
#define MOD_SCH_LTU_CTL_WH__SCH_LTU_SIZE_MASK   0x01ff

//MSM5000, Modulator, New register, addr 0x2CC, 8 bits long
#define MOD_ROTATOR_MAP_WB                      HW_MOD_ROTATOR_MAP
#define MOD_ROTATOR_MAP_WB_MASK                 0x0ff
#define MOD_ROTATOR_MAP_WB__PASTATE_00_MAP_MASK 0x0c0
#define MOD_ROTATOR_MAP_WB__PASTATE_01_MAP_MASK 0x030
#define MOD_ROTATOR_MAP_WB__PASTATE_10_MAP_MASK 0x0c
#define MOD_ROTATOR_MAP_WB__PASTATE_11_MAP_MASK 0x03
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     MODULATOR READ REGISTERS
//-----------------------------------------------------------------------------
#define MASK_DATA_RB                            HW_MASK_DATA
#define MASK_DATA_RB_MASK                       0x00

//MSM5000, Change bits function, Modulator, addr 0x224, 4 bits long
#define MOD_STATUS_RB                           HW_MOD_STATUS
#define MOD_STATUS_RB_MASK                      0x0f
#define MOD_STATUS_RB__SCH_ENCODING_MASK        0x08
#define MOD_STATUS_RB__SCH_ENC_ERROR_MASK       0x04
#define MOD_STATUS_RB__FCH_ENCODING_MASK        0x02
#define MOD_STATUS_RB__FCH_ENC_ERROR_MASK       0x01

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     4X SERIAL VITERBI DECODER WRITE REGISTERS
//-----------------------------------------------------------------------------
//MSM5000, New decoder registers, No attempt was made to be
//backwards compatible.

#define VD_RESET_WB                       HW_VD_RESET

#define VD_MODE_WB                        HW_VD_MODE
#define VD_MODE_WB_MASK                   0x0f
#define VD_MODE_WB__INTMODE_MASK          0x0e
#define VD_MODE_WB__PACKET_MASK           0x01

#define VDPOLY2IJ_WH                      HW_VDPOLY2IJ
#define VDPOLY2IJ_WH_MASK                 0x03fff

#define VDPOLY3IJ_WH                      HW_VDPOLY3IJ
#define VDPOLY3IJ_WH_MASK                 0x03fff

#define VDPOLY3K_WB                       HW_VDPOLY3K
#define VDPOLY3K_WB_MASK                  0x07f

#define VDPOLY4IJ_WH                      HW_VDPOLY4IJ
#define VDPOLY4IJ_WH_MASK                 0x03fff

#define VDPOLY4KL_WH                      HW_VDPOLY4KL
#define VDPOLY4KL_WH_MASK                 0x03fff

#define VD_TESTCON_WB                     HW_VD_TESTCON
#define VD_TESTCON_WB_MASK                0x03

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     4X SERIAL VITERBI DECODER READ REGISTERS
//-----------------------------------------------------------------------------
#define VD_TESTOUT_RH                     HW_VD_TESTOUT
#define VD_TESTOUT_RH_MASK                0x0ffff

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     UART2 WRITE REGISTERS
//-----------------------------------------------------------------------------
#define UART2_MR1_WB                            HW_UART2_MR1
#define UART2_MR1_WB__RX_RDY_CTL_MASK   0x080
#define UART2_MR1_WB__CTS_CTL_MASK      0x040
#define UART2_MR1_WB__AUTO_RFR_LEVEL_MASK       0x03f
#define UART2_MR2_WB                            HW_UART2_MR2
#define UART2_MR2_WB_MASK               0x07f
#define UART2_MR2_WB__LOOPBACK_MASK     0x080
#define UART2_MR2_WB__ERROR_MODE_MASK   0x040
#define UART2_MR2_WB__BITS_PER_CHAR_MASK        0x030
#define UART2_MR2_WB__STOP_BIT_LEN_MASK         0x0c
#define UART2_MR2_WB__PARITY_MODE_MASK  0x03
#define UART2_CSR_WB                            HW_UART2_CSR
#define UART2_CSR_WB__UART_RX_CLK_SEL_MASK      0x0f0
#define UART2_CSR_WB__UART_TX_CLK_SEL_MASK      0x0f
#define UART2_TF_WB                             HW_UART2_TF
#define UART2_CR_WB                             HW_UART2_CR
#define UART2_CR_WB__CHANNEL_COMMAND_MASK       0x0f0
#define UART2_CR_WB__UART_TX_DISABLE_MASK       0x08
#define UART2_CR_WB__UART_TX_EN_MASK    0x04
#define UART2_CR_WB__UART_RX_DISABLE_MASK       0x02
#define UART2_CR_WB__UART_RX_EN_MASK    0x01
#define UART2_IMR_WB                            HW_UART2_IMR
#define UART2_IMR_WB_MASK               0x07f
#define UART2_IMR_WB__CURRENT_CTS_MASK  0x040
#define UART2_IMR_WB__DELTA_CTS_MASK    0x020
#define UART2_IMR_WB__RXLEV_MASK        0x010
#define UART2_IMR_WB__RXSTALE_MASK      0x08
#define UART2_IMR_WB__RXBREAK_MASK      0x04
#define UART2_IMR_WB__RXHUNT_MASK       0x02
#define UART2_IMR_WB__TXLEV_MASK        0x01
#define UART2_IPR_WB                            HW_UART2_IPR
#define UART2_IPR_WB__STALE_TIMEOUT_MSB_MASK    0x080
#define UART2_IPR_WB__SAMPLE_DATA_MASK  0x040
#define UART2_IPR_WB__RXSTALE_LAST_MASK         0x020
#define UART2_IPR_WB__STALE_TIMEOUT_LSB_MASK    0x01f
#define UART2_TFWR_WB                           HW_UART2_TFWR
#define UART2_TFWR_WB_MASK              0x03f
#define UART2_TFWR_WB__TFW_MASK         0x03f
#define UART2_RFWR_WB                           HW_UART2_RFWR
#define UART2_RFWR_WB_MASK              0x03f
#define UART2_RFWR_WB__RFW_MASK         0x03f
#define UART2_HCR_WB                            HW_UART2_HCR
#define UART2_MREG_WB                           HW_UART2_MREG
#define UART2_NREG_WB                           HW_UART2_NREG
#define UART2_DREG_WB                           HW_UART2_DREG
#define UART2_MNDREG_WB                         HW_UART2_MNDREG
#define UART2_MNDREG_WB_MASK            0x03f
#define UART2_MNDREG_WB__MREG_LSB_MASK  0x020
#define UART2_MNDREG_WB__NREG_LSB_MASK  0x01c
#define UART2_MNDREG_WB__DREG_LSB_MASK  0x03
#define UART2_IRDA_WB                           HW_UART2_IRDA
#define UART2_IRDA_WB_MASK              0x0f
#define UART2_IRDA_WB__IRDA_LOOPBACK_MASK       0x08
#define UART2_IRDA_WB__INVERT_IRDA_TX_MASK      0x04
#define UART2_IRDA_WB__INVERT_IRDA_RX_MASK      0x02
#define UART2_IRDA_WB__IRDA_EN_MASK     0x01
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     UART2 READ REGISTERS
//-----------------------------------------------------------------------------
#define UART2_SR_RB                             HW_UART2_SR
#define UART2_SR_RB__HUNT_CHAR_MASK     0x080
#define UART2_SR_RB__RX_BREAK_MASK      0x040
#define UART2_SR_RB__PAR_FRAME_ERR_MASK         0x020
#define UART2_SR_RB__UART2_OVERRUN_MASK         0x010
#define UART2_SR_RB__TXEMT_MASK         0x08
#define UART2_SR_RB__TXRDY_MASK         0x04
#define UART2_SR_RB__RXFULL_MASK        0x02
#define UART2_SR_RB__RXRDY_MASK         0x01
#define UART2_RF_RB                             HW_UART2_RF
#define UART2_MISR_RB                           HW_UART2_MISR
#define UART2_ISR_RB                            HW_UART2_ISR
#define UART2_ISR_RB_MASK               0x07f
#define UART2_ISR_RB__CURRENT_CTS_MASK  0x040
#define UART2_ISR_RB__DELTA_CTS_MASK    0x020
#define UART2_ISR_RB__RXLEV_MASK        0x010
#define UART2_ISR_RB__RXSTALE_MASK      0x08
#define UART2_ISR_RB__RXBREAK_MASK      0x04
#define UART2_ISR_RB__RXHUNT_MASK       0x02
#define UART2_ISR_RB__TXLEV_MASK        0x01
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     CDMA VOCODER WRITE REGISTERS
//-----------------------------------------------------------------------------
#define VOC_RESET_WB                            HW_VOC_RESET
#define VOC_RESET_WB_MASK               0x03
#define VOC_RESET_WB__VROM_BANK_SEL_MASK        0x02
#define VOC_RESET_WB__FIRMWARE_START_MASK       0x01
#define ENC_FRAME_ADVANCE_WB                    HW_ENC_FRAME_ADVANCE
#define ENC_ADJUST_WB                           HW_ENC_ADJUST
#define ENC_ADJUST_WB_MASK              0x00
#define DEC_FRAME_ADVANCE_WB                    HW_DEC_FRAME_ADVANCE
#define DEC_ADJUST_WB                           HW_DEC_ADJUST
#define DEC_ADJUST_WB_MASK              0x00
#define DEC_INT_ADVANCE_WB                      HW_DEC_INT_ADVANCE
#define ENC_PCM_CONTROL_WB                      HW_ENC_PCM_CONTROL
#define ENC_PCM_CONTROL_WB__VOC_ENC_DC_BLOCK_DISABLE_MASK       0x080
#define ENC_PCM_CONTROL_WB__VOC_ENC_PCM_FORMAT_MASK     0x060
#define ENC_PCM_CONTROL_WB__VOC_ENC_SLOT_MASK   0x01f
#define DEC_PCM_CONTROL_WB                      HW_DEC_PCM_CONTROL
#define DEC_PCM_CONTROL_WB__VOC_DEC_PCM_OE_MASK         0x080
#define DEC_PCM_CONTROL_WB__VOC_DEC_PCM_FORMAT_MASK     0x060
#define DEC_PCM_CONTROL_WB__VOC_DEC_SLOT_MASK   0x01f
#define ENC_PCM_WR_MSB_WB                       HW_ENC_PCM_WR_MSB
#define ENC_PCM_WR_LSB_WB                       HW_ENC_PCM_WR_LSB
#define DEC_PCM_WR_MSB_WB                       HW_DEC_PCM_WR_MSB
#define DEC_PCM_WR_LSB_WB                       HW_DEC_PCM_WR_LSB
#define TEST_CONTROL_1_WB                       HW_TEST_CONTROL_1
#define TEST_CONTROL_1_WB_MASK          0x07e
#define TEST_CONTROL_1_WB__PCM_CLK_SENSE_MASK   0x040
#define TEST_CONTROL_1_WB__BIST_MODE_MASK       0x020
#define TEST_CONTROL_1_WB__VOC_ENC_PCM_WR_EN_MASK       0x010
#define TEST_CONTROL_1_WB__VOC_DEC_PCM_WR_EN_MASK       0x08
#define TEST_CONTROL_1_WB__MICRO_VOC_FR_REF_MASK        0x04
#define TEST_CONTROL_1_WB__PCM_LOOPBACK_MASK    0x02
#define MODE_CONTROL_WB                         HW_MODE_CONTROL
#define MODE_CONTROL_WB__13K_MODE_MASK  0x080
#define MODE_CONTROL_WB__INCL_OMIT_QTR_RTE_MASK         0x040
#define MODE_CONTROL_WB__DC_BLOCK_MODE_MASK     0x040
#define MODE_CONTROL_WB__IS96A_MODE_MASK        0x020
#define MODE_CONTROL_WB__EXT_PCM_LOOPBACK_MASK  0x010
#define MODE_CONTROL_WB__PACKET_LOOPBACK_MASK   0x08
#define MODE_CONTROL_WB__PCM_TEST_MASK  0x04
#define MODE_CONTROL_WB__DTMF_DETECT_MASK       0x01
#define VOX_CONTROL_WB                          HW_VOX_CONTROL
#define VOX_CONTROL_WB__ESEC_DIAG_MASK  0x080
#define VOX_CONTROL_WB__ESEC_GEAR_DISABLE_MASK  0x040
#define VOX_CONTROL_WB__ESEC_EN_MASK    0x08
#define VOX_CONTROL_WB__ENC_MUTE_MASK   0x04
#define VOX_CONTROL_WB__DEC_MUTE_MASK   0x02
#define VOX_CONTROL_WB__VOX_EN_MASK     0x01
#define PEEK_POKE_CONTROL_WB                    HW_PEEK_POKE_CONTROL
#define PEEK_POKE_CONTROL_WB_MASK               0x07f
#define PEEK_POKE_CONTROL_WB__PEEK_POKE_EN_MASK         0x040
#define PEEK_POKE_CONTROL_WB__PEEK_POKE_MASK    0x020
#define PEEK_POKE_CONTROL_WB__PEEK_POKE_ARM_MASK        0x018
#define PEEK_POKE_CONTROL_WB__PEEK_POKE_ADDR_10_8_MASK  0x07
#define PEEK_POKE_ADDR_LSB_WB                   HW_PEEK_POKE_ADDR_LSB
#define POKE_DATA_MSB_WB                        HW_POKE_DATA_MSB
#define POKE_DATA_LSB_WB                        HW_POKE_DATA_LSB
#define ENC_RATE_CONTROL_WB                     HW_ENC_RATE_CONTROL
#define ENC_RATE_CONTROL_WB__VOC_ENC_INIT_BKG_NOISE_MASK        0x080
#define ENC_RATE_CONTROL_WB__VOC_ENC_RATE_LIMIT_TYPE_MASK       0x040
#define ENC_RATE_CONTROL_WB__VOC_ENC_RATE_LIMIT_EN_MASK         0x020
#define ENC_RATE_CONTROL_WB__REDUCED_RATE_MODE_MASK     0x020
#define ENC_RATE_CONTROL_WB__VOC_ENC_BLANK_FRAME_MASK   0x010
#define ENC_RATE_CONTROL_WB__VOC_ENC_MAX_RATE_MASK      0x0c
#define ENC_RATE_CONTROL_WB__VOC_ENC_MIN_RATE_MASK      0x03
#define ENC_RATE_LIMIT_FACTOR_WB                HW_ENC_RATE_LIMIT_FACTOR
#define RRM_LEVEL_WB                            HW_RRM_LEVEL
#define ENERGY_DECAY_FAC_MSB_WB                 HW_ENERGY_DECAY_FAC_MSB
#define ENERGY_DECAY_FAC_LSB_WB                 HW_ENERGY_DECAY_FAC_LSB
#define BGN_DECAY_FAC_MSB_WB                    HW_BGN_DECAY_FAC_MSB
#define BGN_DECAY_FAC_LSB_WB                    HW_BGN_DECAY_FAC_LSB
#define DEC_VOX_THRESHOLD_MSB_WB                HW_DEC_VOX_THRESHOLD_MSB
#define DEC_VOX_THRESHOLD_LSB_WB                HW_DEC_VOX_THRESHOLD_LSB
#define ENC_WEIGHTING_FAC_MSB_WB                HW_ENC_WEIGHTING_FAC_MSB
#define ENC_WEIGHTING_FAC_LSB_WB                HW_ENC_WEIGHTING_FAC_LSB
#define DEC_BGN_FLOOR_MSB_WB                    HW_DEC_BGN_FLOOR_MSB
#define DEC_BGN_FLOOR_LSB_WB                    HW_DEC_BGN_FLOOR_LSB
#define DTMF_DURATION_MSB_WB                    HW_DTMF_DURATION_MSB
#define DTMF_DURATION_LSB_WB                    HW_DTMF_DURATION_LSB
#define DTMF_HIGH_MSB_WB                        HW_DTMF_HIGH_MSB
#define DTMF_HIGH_LSB_WB                        HW_DTMF_HIGH_LSB
#define DTMF_LOW_MSB_WB                         HW_DTMF_LOW_MSB
#define DTMF_LOW_LSB_WB                         HW_DTMF_LOW_LSB
#define DTMF_VOLUME_MSB_WB                      HW_DTMF_VOLUME_MSB
#define DTMF_VOLUME_LSB_WB                      HW_DTMF_VOLUME_LSB
#define DEC_VOLUME_MSB_WB                       HW_DEC_VOLUME_MSB
#define DEC_VOLUME_LSB_WB                       HW_DEC_VOLUME_LSB
#define PCM_PAD_CONTROL_WB                      HW_PCM_PAD_CONTROL
#define PCM_PAD_CONTROL_WB_MASK                 0x03f
#define PCM_PAD_CONTROL_WB__ENC_PCM_COMPANDED_MASK      0x020
#define PCM_PAD_CONTROL_WB__PAD_COMPANDED_MASK  0x010
#define PCM_PAD_CONTROL_WB__PAD_LINEAR_MASK     0x08
#define PCM_PAD_CONTROL_WB__PAD_BITS_MASK       0x07
#define PCM_CONTROL_WORD_MSB_WB                 HW_PCM_CONTROL_WORD_MSB
#define PCM_CONTROL_WORD_LSB_WB                 HW_PCM_CONTROL_WORD_LSB
#define IO_CONTROL_WB                           HW_IO_CONTROL
#define IO_CONTROL_WB__VOC_ENC_DRQ_EN_MASK      0x080
#define IO_CONTROL_WB__VOC_ENC_INT_EN_MASK      0x040
#define IO_CONTROL_WB__VOC_DEC_DRQ_EN_MASK      0x020
#define IO_CONTROL_WB__VOC_DEC_INT_EN_MASK      0x010
#define IO_CONTROL_WB__VOC_ENC_DRQ_SENSE_MASK   0x08
#define IO_CONTROL_WB__VOC_ENC_INT_SENSE_MASK   0x04
#define IO_CONTROL_WB__VOC_DEC_DRQ_SENSE_MASK   0x02
#define IO_CONTROL_WB__VOC_DEC_INT_SENSE_MASK   0x01
#define CLEAR_ENC_INT_WB                        HW_CLEAR_ENC_INT
#define CLEAR_ENC_INT_WB_MASK           0x00
#define CLEAR_DEC_INT_WB                        HW_CLEAR_DEC_INT
#define CLEAR_DEC_INT_WB_MASK           0x00
#define DEC_PACKET_WB                           HW_DEC_PACKET
#define DEC_PACKET_WB__13K_PACKET_DATA_MASK     0x080
#define DEC_PACKET_WB__LOAD_FIR_MASK    0x020
#define DEC_PACKET_WB__VOC_DEC_BAD_RX_DATA_MASK         0x010
#define DEC_PACKET_WB__VOC_DEC_ERASURE_MASK     0x08
#define DEC_PACKET_WB__VOC_DEC_BLANK_MASK       0x04
#define DEC_PACKET_WB__VOC_DEC_RATE_MASK        0x03
#define AUX_IN_PORT_MSB_WB                      HW_AUX_IN_PORT_MSB
#define AUX_IN_PORT_LSB_WB                      HW_AUX_IN_PORT_LSB
#define VOC_FL_RAM_ADDR_WH                      HW_VOC_FL_RAM_ADDR
#define VOC_FL_RAM_ADDR_WH_MASK                 0x0ffff
#define VOC_FL_RAM_DATA_WH                      HW_VOC_FL_RAM_DATA
#define VOC_FL_RAM_DATA_WH_MASK                 0x0ffff
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     CDMA VOCODER READ REGISTERS
//-----------------------------------------------------------------------------
#define ENC_PCM_RD_MSB_RB                       HW_ENC_PCM_RD_MSB
#define ENC_PCM_RD_LSB_RB                       HW_ENC_PCM_RD_LSB
#define DEC_PCM_RD_MSB_RB                       HW_DEC_PCM_RD_MSB
#define DEC_PCM_RD_LSB_RB                       HW_DEC_PCM_RD_LSB
#define VOC_STATUS_RB                           HW_VOC_STATUS
#define VOC_STATUS_RB__ONES_DETECT_MASK         0x080
#define VOC_STATUS_RB__BIST_COMPLETE_MASK       0x040
#define VOC_STATUS_RB__VOC_ENC_PCM_INT_MASK     0x020
#define VOC_STATUS_RB__VOC_ENC_DRQ_MASK         0x010
#define VOC_STATUS_RB__VOC_ENC_INT_MASK         0x08
#define VOC_STATUS_RB__VOC_DEC_PCM_INT_MASK     0x04
#define VOC_STATUS_RB__VOC_DEC_DRQ_MASK         0x02
#define VOC_STATUS_RB__VOC_DEC_INT_MASK         0x01
#define AUX_STATUS1_RB                          HW_AUX_STATUS1
#define AUX_STATUS2_RB                          HW_AUX_STATUS2
#define AUX_STATUS3_RB                          HW_AUX_STATUS3
#define TEST_STATUS_RB                          HW_TEST_STATUS
#define ENC_PACKET_RB                           HW_ENC_PACKET
#define ENC_PACKET_RB__VOC_ENC_PACKET_FORMAT_MASK       0x0ff
#define ENC_PACKET_RB__13K_PACKET_FORMAT_MASK   0x080
#define ENC_PACKET_RB__DTMF_STATUS_EN_MASK      0x020
#define ENC_PACKET_RB__VOC_ENC_BLANK_MASK       0x04
#define ENC_PACKET_RB__VOC_ENC_RATE_MASK        0x03
#define VOC_ENC_SAMP_CNT_RB                     HW_VOC_ENC_SAMP_CNT
#define VOC_DEC_SAMP_CNT_RB                     HW_VOC_DEC_SAMP_CNT
#define DTMF_STATUS0_RB                         HW_DTMF_STATUS0
#define DTMF_STATUS0_RB__DIGIT_DETECTED_MASK    0x080
#define DTMF_STATUS0_RB__DTMF_TEST_STATUS_MASK  0x070
#define DTMF_STATUS0_RB__DTMF_ROW_MASK  0x0c
#define DTMF_STATUS0_RB__DTMF_COLUMN_MASK       0x03
#define DTMF_STATUS1_RB                         HW_DTMF_STATUS1
#define DTMF_STATUS2_RB                         HW_DTMF_STATUS2
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     FM VOCODER WRITE REGISTERS
//-----------------------------------------------------------------------------
#define DTMF_STATUS2_RB__VROM_BANK_SEL_MASK     0x02
#define DTMF_STATUS2_RB__FIRMWARE_START_MASK    0x01
#define TX_PCM_CONTROL_WB                       HW_TX_PCM_CONTROL
#define TX_PCM_CONTROL_WB__VOC_ENC_DC_BLOCK_DISABLE_MASK        0x080
#define TX_PCM_CONTROL_WB__VOC_ENC_PCM_FORMAT_MASK      0x060
#define TX_PCM_CONTROL_WB__VOC_ENC_SLOT_MASK    0x01f
#define RX_PCM_CONTROL_WB                       HW_RX_PCM_CONTROL
#define RX_PCM_CONTROL_WB__VOC_DEC_PCM_OE_MASK  0x080
#define RX_PCM_CONTROL_WB__VOC_DEC_PCM_FORMAT_MASK      0x060
#define RX_PCM_CONTROL_WB__VOC_DEC_SLOT_MASK    0x01f
#define RX_PCM_CONTROL_WB__PCM_CLK_SENSE_MASK   0x040
#define RX_PCM_CONTROL_WB__PCM_LOOPBACK_MASK    0x02
#define RX_PCM_CONTROL_WB__EXT_PCM_LOOPBACK_MASK        0x010
#define FM_AUDIO_CONFIG_WB                      HW_FM_AUDIO_CONFIG
#define FM_AUDIO_CONFIG_WB_MASK                 0x0fe
#define FM_AUDIO_CONFIG_WB__FM_VOX_EN_MASK      0x080
#define FM_AUDIO_CONFIG_WB__AUTO_MUTE_WBD_MASK  0x020
#define FM_AUDIO_CONFIG_WB__TX_MUTE_MASK        0x08
#define FM_AUDIO_CONFIG_WB__RX_DTMF_EN_MASK     0x04
#define FM_AUDIO_CONFIG_WB__TX_DTMF_EN_MASK     0x02
#define FM_TEST_CTL_WB                          HW_FM_TEST_CTL
#define FM_TEST_CTL_WB__FM_PACKET_VALID_MASK    0x080
#define FM_TEST_CTL_WB__COMP_DISABLE_MASK       0x020
#define FM_TEST_CTL_WB__FM_LOOPBACK_MASK        0x08
#define FM_TEST_CTL_WB__EXPAND_DISABLE_MASK     0x02
#define FM_TEST_CTL_WB__SAT_TRANS_DISABLE_MASK  0x01
#define VOC_FM_CONFIG_WB                        HW_VOC_FM_CONFIG
#define VOC_FM_CONFIG_WB_MASK           0x0f
#define VOC_FM_CONFIG_WB__FM_DTMF_OVRWRT_EN_MASK        0x08
#define VOC_FM_CONFIG_WB__SAT_CHANGED_INT_MASK_MASK     0x04
#define VOC_FM_CONFIG_WB__SAT_ECC_MASK  0x03
#define SAT_LEVEL_WB                            HW_SAT_LEVEL
#define SAT_THRESHOLD_WB                        HW_SAT_THRESHOLD
#define TX_DEVIATION_LIMIT_WB                   HW_TX_DEVIATION_LIMIT
#define FM_TX_GAIN_WB                           HW_FM_TX_GAIN
#define FM_RX_GAIN_WB                           HW_FM_RX_GAIN
#define DTMF_FM_RX_GAIN_WB                      HW_DTMF_FM_RX_GAIN
#define DTMF_FM_TX_GAIN_WB                      HW_DTMF_FM_TX_GAIN
#define DTMF_FM_TX_GAIN_WB__ENC_PCM_COMPANDED_MASK      0x020
#define DTMF_FM_TX_GAIN_WB__PAD_COMPANDED_MASK  0x010
#define DTMF_FM_TX_GAIN_WB__PAD_LINEAR_MASK     0x08
#define DTMF_FM_TX_GAIN_WB__PAD_BITS_MASK       0x07
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     FM VOCODER READ REGISTERS
//-----------------------------------------------------------------------------
#define DTMF_FM_TX_GAIN_WB__ONES_DETECT_MASK    0x080
#define FM_STATUS_RB                            HW_FM_STATUS
#define FM_STATUS_RB_MASK               0x0fe
#define FM_STATUS_RB__PEEK_EN_MASK      0x080
#define FM_STATUS_RB__DIAG_EN_MASK      0x040
#define FM_STATUS_RB__SAT_DCC_MASK      0x0c
#define FM_STATUS_RB__MUTE_WBD_MASK     0x02
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     DFM WRITE REGISTERS
//-----------------------------------------------------------------------------
#define DFM_INITIAL_WB                          HW_DFM_INITIAL
#define DFM_INITIAL_WB_MASK             0x07e
#define DFM_INITIAL_WB__IQ_FORMAT_MASK  0x040
#define DFM_INITIAL_WB__EN_DFM_TX_MASK  0x020
#define DFM_INITIAL_WB__CDMA_FM_SEL_MASK        0x010
#define DFM_INITIAL_WB__RES_DFM_uP_MASK         0x08
#define DFM_INITIAL_WB__MODE_DFM_360_MASK       0x04
#define DFM_INITIAL_WB__RES_DFM_360_MASK        0x02
#define DFM_DC_OFFSET_GAIN_WB                   HW_DFM_DC_OFFSET_GAIN
#define DFM_DC_OFFSET_GAIN_WB_MASK              0x03f
#define DFM_DC_OFFSET_GAIN_WB__Q_OFFSET_GAIN_MASK       0x038
#define DFM_DC_OFFSET_GAIN_WB__I_OFFSET_GAIN_MASK       0x07
#define DFM_AGC_REF_WB                          HW_DFM_AGC_REF
#define DFM_AGC_REF_WB_MASK             0x01f
#define DFM_AGC_ACC_MIN_WB                      HW_DFM_AGC_ACC_MIN
#define DFM_AGC_ACC_MIN_WB_MASK                 0x03f
#define DFM_AGC_ACC_MAX_WB                      HW_DFM_AGC_ACC_MAX
#define DFM_AGC_ACC_MAX_WB_MASK                 0x03f
#define DFM_AGC_GAIN_WB                         HW_DFM_AGC_GAIN
#define DFM_AGC_GAIN_WB__AGC_LOOP_ATC_MASK      0x0f0
#define DFM_AGC_GAIN_WB__AGC_LOOP_DTC_MASK      0x0f
#define DFM_FREQ_LOOP_CONFIG_WB                 HW_DFM_FREQ_LOOP_CONFIG
#define DFM_FREQ_LOOP_CONFIG_WB_MASK            0x01f
#define DFM_FREQ_LOOP_CONFIG_WB__FREQ_CHIRP_EN_MASK     0x010
#define DFM_FREQ_LOOP_CONFIG_WB__FREQ_DITHER_EN_MASK    0x08
#define DFM_FREQ_LOOP_CONFIG_WB__FREQ_TRACK_GAIN_MASK   0x07
#define DFM_PDM_CONFIG_WB                       HW_DFM_PDM_CONFIG
#define DFM_PDM_CONFIG_WB_MASK          0x03f
#define DFM_PDM_CONFIG_WB__LOOP_STAT_SEL_MASK   0x0c0
#define DFM_PDM_CONFIG_WB__FREQ_PDM_OE_MASK     0x020
#define DFM_PDM_CONFIG_WB__RXAGC_PDM_OE_MASK    0x010
#define DFM_PDM_CONFIG_WB__IQ_PDM_OE_MASK       0x08
#define DFM_PDM_CONFIG_WB__FREQ_PDM_POLARITY_MASK       0x04
#define DFM_PDM_CONFIG_WB__RXAGC_POLARITY_MASK  0x02
#define DFM_PDM_CONFIG_WB__DC_POLARITY_MASK     0x01
#define DFM_DC_PDM_0_WB                         HW_DFM_DC_PDM_0
#define DFM_DC_PDM_1_WB                         HW_DFM_DC_PDM_1
#define DFM_DC_PDM_2_WB                         HW_DFM_DC_PDM_2
#define DFM_DC_PDM_2_WB_MASK            0x07f
#define DFM_DC_PDM_2_WB__DC_PDM_RANGE_SEL_MASK  0x040
#define DFM_DC_PDM_2_WB__Q_DC_OFFSET_FREEZE_MASK        0x020
#define DFM_DC_PDM_2_WB__I_DC_OFFSET_FREEZE_MASK        0x010
#define DFM_DC_PDM_2_WB__Q_DC_IN_SEL_MASK       0x08
#define DFM_DC_PDM_2_WB__I_DC_IN_SEL_MASK       0x04
#define DFM_DC_PDM_2_WB__Q_DC_OFFSET_MSB_MASK   0x02
#define DFM_DC_PDM_2_WB__I_DC_OFFSET_MSB_MASK   0x01
#define DFM_RXAGC_PDM_0_WB                      HW_DFM_RXAGC_PDM_0
#define DFM_RXAGC_PDM_0_WB__RXAGC_IN_SEL_MASK   0x080
#define DFM_RXAGC_PDM_0_WB__RXAGC_PDM_OVERRIDE_MASK     0x07f
#define DFM_FREQ_PDM_0_WB                       HW_DFM_FREQ_PDM_0
#define DFM_FREQ_PDM_1_WB                       HW_DFM_FREQ_PDM_1
#define DFM_FREQ_PDM_1_WB_MASK          0x07
#define DFM_FREQ_PDM_1_WB__FREQ_TRACK_FREEZE_MASK       0x04
#define DFM_FREQ_PDM_1_WB__FREQ_IN_SEL_MASK     0x02
#define DFM_FREQ_PDM_1_WB__FREQ_IN_MSB_MASK     0x01
#define DFM_VOC_INTF_CONFIG_WB                  HW_DFM_VOC_INTF_CONFIG
#define DFM_VOC_INTF_CONFIG_WB_MASK             0x07
#define DFM_VOC_INTF_CONFIG_WB__DFM_LOOPBACK_MASK       0x04
#define DFM_VOC_INTF_CONFIG_WB__VOC_LOOPBACK_MASK       0x02
#define DFM_VOC_INTF_CONFIG_WB__FM_REQ_SEL_MASK         0x01
#define DFM_TXWBD_INTF_0_WB                     HW_DFM_TXWBD_INTF_0
#define DFM_TXWBD_INTF_1_WB                     HW_DFM_TXWBD_INTF_1
#define DFM_MAX_TX_PWR_0_WB                     HW_DFM_MAX_TX_PWR_0
#define DFM_MAX_TX_PWR_1_WB                     HW_DFM_MAX_TX_PWR_1
#define DFM_MAX_TX_PWR_1_WB_MASK                0x07
#define DFM_MAX_TX_PWR_1_WB__TXAGC_PDM_OE_MASK  0x04
#define DFM_MAX_TX_PWR_1_WB__TXAGC_POLARITY_MASK        0x02
#define DFM_MAX_TX_PWR_1_WB__TXAGC_PDM_MSB_MASK         0x01
#define DFM_FREQ_SENS_GAIN_WB                   HW_DFM_FREQ_SENS_GAIN
#define DFM_FREQ_SENS_GAIN_WB_MASK              0x03f
#define DFM_TXFM_CONFIG_WB                      HW_DFM_TXFM_CONFIG
#define DFM_TXFM_CONFIG_WB_MASK                 0x07
#define DFM_TXFM_CONFIG_WB__TXAGC_TRI_EN_MASK   0x04
#define DFM_TXFM_CONFIG_WB__DFM_TXCTL_EN_MASK   0x02
#define DFM_TXFM_CONFIG_WB__TX_AUDIO_MUTE_MASK  0x01
#define DFM_MIN1_BYTE_0_WB                      HW_DFM_MIN1_BYTE_0
#define DFM_MIN1_BYTE_1_WB                      HW_DFM_MIN1_BYTE_1
#define DFM_MIN1_BYTE_2_WB                      HW_DFM_MIN1_BYTE_2
#define DFM_RXWBD_BANDWIDTH_WB                  HW_DFM_RXWBD_BANDWIDTH
#define DFM_RXWBD_BANDWIDTH_WB_MASK             0x07f
#define DFM_RXWBD_BANDWIDTH_WB__DPLL_FALSELOCK_EN_MASK  0x040
#define DFM_RXWBD_BANDWIDTH_WB__WBD_ACQ_BW_MASK         0x038
#define DFM_RXWBD_BANDWIDTH_WB__WBD_TRK_BW_MASK         0x07
#define DFM_RXWBD_CONFIG_0_WB                   HW_DFM_RXWBD_CONFIG_0
#define DFM_RXWBD_CONFIG_0_WB__BCH_BYPASS_MASK  0x080
#define DFM_RXWBD_CONFIG_0_WB__ANY_DATA_INT_EN_MASK     0x040
#define DFM_RXWBD_CONFIG_0_WB__MY_DATA_INT_EN_MASK      0x020
#define DFM_RXWBD_CONFIG_0_WB__WS_SEL_MASK      0x010
#define DFM_RXWBD_CONFIG_0_WB__FVC_MVOTE_MODE_MASK      0x08
#define DFM_RXWBD_CONFIG_0_WB__ROAM_STAT_MASK   0x04
#define DFM_RXWBD_CONFIG_0_WB__WBD_RX_INV_MASK  0x02
#define DFM_RXWBD_CONFIG_0_WB__FOCC_FVC_SEL_MASK        0x01
#define DFM_RXWBD_WR_WB                         HW_DFM_RXWBD_WR
#define DFM_RXWBD_WR_WB_MASK            0x03
#define DFM_RXWBD_WR_WB__NRZDATA_WR_MASK        0x02
#define DFM_RXWBD_WR_WB__NRZDATA_WR_MODE_MASK   0x01
#define DFM_RXAGC_PDM_1_WB                      HW_DFM_RXAGC_PDM_1
#define DFM_RXAGC_PDM_1_WB_MASK                 0x01
#define DFM_RXAGC_PDM_1_WB__RXAGC_PDM_FREEZE_MASK       0x01
#define DFM_RXWBD_CONFIG_1_WB                   HW_DFM_RXWBD_CONFIG_1
#define DFM_RXWBD_CONFIG_1_WB_MASK              0x03
#define DFM_RXWBD_CONFIG_1_WB__AUDIO_LOW_PWR_EN_MASK    0x02
#define DFM_RXWBD_CONFIG_1_WB__FOCC_MVOTE_MODE_MASK     0x01
#define DFM_SLOT_PDM_CTL_WB                     HW_DFM_SLOT_PDM_CTL
#define DFM_SLOT_PDM_CTL_WB_MASK                0x0f
#define DFM_SLOT_PDM_CTL_WB__IQ_OFFSET_TRI_EN_MASK      0x08
#define DFM_SLOT_PDM_CTL_WB__RXAGC_TRI_EN_MASK  0x04
#define DFM_SLOT_PDM_CTL_WB__FREQ_TRACK_TRI_EN_MASK     0x02
#define DFM_SLOT_PDM_CTL_WB__PDM_LOW_PWR_EN_MASK        0x01
#define DFM_SLOT_CTL_WB                         HW_DFM_SLOT_CTL
#define DFM_SLOT_CTL_WB__SLEEP_FORCE_N_MASK     0x080
#define DFM_SLOT_CTL_WB__SLEEP_OVERRIDE_N_MASK  0x040
#define DFM_SLOT_CTL_WB__STREAM_SLOT_EN_MASK    0x020
#define DFM_SLOT_CTL_WB__SYNC_ERASE_EN_MASK     0x010
#define DFM_SLOT_CTL_WB__FILLER_EN_MASK         0x08
#define DFM_SLOT_CTL_WB__CRC_EN_MASK    0x04
#define DFM_SLOT_CTL_WB__MAJORITY_EN_MASK       0x02
#define DFM_SLOT_CTL_WB__SLEEP_EN_MASK  0x01
#define DFM_SLOT_CTL_1_WB                       HW_DFM_SLOT_CTL_1
#define DFM_SLOT_CTL_1_WB__RF_SLEEP_MVOTE_MODE_MASK 0x01
#define DFM_DPLL_WU_TIMER_WB                    HW_DFM_DPLL_WU_TIMER
#define DFM_SYNC_WU_TIMER_WB                    HW_DFM_SYNC_WU_TIMER
#define DFM_DATA_WU_TIMER_WB                    HW_DFM_DATA_WU_TIMER
#define DFM_STREAM_SLOT_TIMER_WB                HW_DFM_STREAM_SLOT_TIMER
#define DFM_STREAM_SLOT_TIMER_WB_MASK           0x03f
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     DFM READ REGISTERS
//-----------------------------------------------------------------------------
#define DFM_RXIQ_STATUS_0_RB                    HW_DFM_RXIQ_STATUS_0
#define DFM_RX_AGC_FILTER_RB                    HW_DFM_RX_AGC_FILTER
#define DFM_RX_AGC_RSSI_RB                      HW_DFM_RX_AGC_RSSI
#define DFM_RX_AGC_RSSI_RB_MASK                 0x07f
#define DFM_TXWBD_STATUS_RB                     HW_DFM_TXWBD_STATUS
#define DFM_TXWBD_STATUS_RB_MASK                0x07
#define DFM_TXWBD_STATUS_RB__TXFM_WBD_MUTE_MASK         0x04
#define DFM_TXWBD_STATUS_RB__DFM_TXCTL_MASK     0x02
#define DFM_TXWBD_STATUS_RB__IDLE_BUSY_N_MASK   0x01
#define DFM_RXWBD_STAT_4_RB                     HW_DFM_RXWBD_STAT_4
#define DFM_RXWBD_STAT_4_RB_MASK                0x0f
#define DFM_RXWBD_STAT_4_RB__BCH_PARITY_11_8_MASK       0x0f
#define DFM_RXWBD_STAT_5_RB                     HW_DFM_RXWBD_STAT_5
#define DFM_RXWBD_STAT_5_RB__BCH_PARITY_7_0_MASK        0x0ff
#define DFM_RXWBD_STAT_0_RB                     HW_DFM_RXWBD_STAT_0
#define DFM_RXWBD_STAT_0_RB__BCH_DATA_7_0_MASK  0x0ff
#define DFM_RXWBD_STAT_1_RB                     HW_DFM_RXWBD_STAT_1
#define DFM_RXWBD_STAT_1_RB__BCH_DATA_15_8_MASK         0x0ff
#define DFM_RXWBD_STAT_2_RB                     HW_DFM_RXWBD_STAT_2
#define DFM_RXWBD_STAT_2_RB__BCH_DATA_23_16_MASK        0x0ff
#define DFM_RXWBD_STAT_3_RB                     HW_DFM_RXWBD_STAT_3
#define DFM_RXWBD_STAT_3_RB__BCH_STATUS_MASK    0x0c0
#define DFM_RXWBD_STAT_3_RB__BCH_DATA_27_24_MASK        0x0f
#define DFM_WORD_SYNC_COUNT_RB                  HW_DFM_WORD_SYNC_COUNT
#define DFM_RXWBD_RD_RB                         HW_DFM_RXWBD_RD
#define DFM_RXWBD_RD_RB_MASK            0x03
#define DFM_RXWBD_RD_RB__NRZDATA_RD_MASK        0x02
#define DFM_RXWBD_RD_RB__NRZDATA_STATUS_MASK    0x01
#define DFM_SLOT_STATUS_RB                      HW_DFM_SLOT_STATUS
#define DFM_SLOT_STATUS_RB_MASK                 0x0f
#define DFM_SLOT_STATUS_RB__SYNC_ERASED_MASK    0x08
#define DFM_SLOT_STATUS_RB__SLOT_COUNT_MASK     0x07
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     WEB CORE WRITE REGISTERS
//-----------------------------------------------------------------------------
//  SECTION     CDMA Sleep Control
#define SLEEP_CTL_WB                            HW_SLEEP_CTL
#define SLEEP_CTL_WB_MASK               0x03f
#define SLEEP_CTL_WB__BBA_POWER_DOWN_MASK       0x020
#define SLEEP_CTL_WB__AUTO_KICK_ARM_MASK        0x010
#define SLEEP_CTL_WB__RF_SLEEP_N_MASK_N_MASK    0x08
#define SLEEP_CTL_WB__ASIC_SLEEP_ARM_MASK       0x04
#define SLEEP_CTL_WB__WAKE_UP_EN_MASK   0x02
#define SLEEP_CTL_WB__WATCH_DOG_MASK    0x01
#define POWER_DOWN_SLEEP_INTERVAL_0_WH          HW_POWER_DOWN_SLEEP_INTERVAL_0
#define POWER_DOWN_SLEEP_INTERVAL_0_WH_MASK             0x0ffff
#define POWER_DOWN_SLEEP_INTERVAL_1_WH          HW_POWER_DOWN_SLEEP_INTERVAL_1
#define POWER_DOWN_SLEEP_INTERVAL_1_WH_MASK             0x0ffff
#define POWER_DOWN_WU_TIME_WH                   HW_POWER_DOWN_WU_TIME
#define POWER_DOWN_WU_TIME_WH_MASK              0x0ffff
#define POWER_DOWN_CHIPX8_SLEEP_TIME_WH         HW_POWER_DOWN_CHIPX8_SLEEP_TIME
#define POWER_DOWN_CHIPX8_SLEEP_TIME_WH_MASK            0x03ff
//  SECTION     Interrupt Controller
#define INT_CLEAR_0_WH                          HW_INT_CLEAR_0
#define INT_CLEAR_0_WH_MASK                     0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define INT_CLEAR_1_WH                          HW_INT_CLEAR_1
#define INT_CLEAR_1_WH_MASK                     0x0efff
#define IRQ_MASK_0_WH                           HW_IRQ_MASK_0
#define IRQ_MASK_0_WH_MASK                      0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define IRQ_MASK_1_WH                           HW_IRQ_MASK_1
#define IRQ_MASK_1_WH_MASK                      0x0efff
#define FIQ_MASK_0_WH                           HW_FIQ_MASK_0
#define FIQ_MASK_0_WH_MASK                      0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define FIQ_MASK_1_WH                           HW_FIQ_MASK_1
#define FIQ_MASK_1_WH_MASK                      0x0efff
#define INT_POLARITY_WH                         HW_INT_POLARITY
#define INT_POLARITY_WH_MASK                            0x07f
#define INT_POLARITY_WH__DP_RX_DATA2_POLARITY_MASK      0x040
#define INT_POLARITY_WH__DP_RX_DATA_POLARITY_MASK       0x020
#define INT_POLARITY_WH__GPIO_INT_POLARITY_MASK         0x01f

//  SECTION     GPIO

#define GPIO_OUT_0_WB                           HW_GPIO_OUT_0
#define GPIO_OUT_0_WB_MASK              0x03f
#define GPIO_OUT_1_WB                           HW_GPIO_OUT_1
#define GPIO_OUT_2_WB                           HW_GPIO_OUT_2
#define GPIO_OUT_2_WB_MASK              0x0f
#define GPIO_OUT_3_WB                           HW_GPIO_OUT_3
#define GPIO_OUT_4_WB                           HW_GPIO_OUT_4
#define GPIO_OUT_4_WB_MASK              0x03f
#define GPIO_TSEN_0_WB                          HW_GPIO_TSEN_0
#define GPIO_TSEN_0_WB_MASK             0x03f
#define GPIO_TSEN_1_WB                          HW_GPIO_TSEN_1
#define GPIO_TSEN_2_WB                          HW_GPIO_TSEN_2
#define GPIO_TSEN_2_WB_MASK             0x01f
#define GPIO_TSEN_2_WB__PA_ON_WR_MASK   0x010
#define GPIO_TSEN_2_WB__GPIO_TSEN_2_MASK        0x0f
#define GPIO_TSEN_3_WB                          HW_GPIO_TSEN_3
#define GPIO_TSEN_4_WB                          HW_GPIO_TSEN_4
#define GPIO_TSEN_4_WB_MASK             0x03f
#define GPIO_INT_OUT_WB                         HW_GPIO_INT_OUT
#define GPIO_INT_OUT_WB_MASK            0x01f
#define GPIO_INT_TSEN_WB                        HW_GPIO_INT_TSEN
#define GPIO_INT_TSEN_WB_MASK           0x01f
#define GPIO_FUNCTION_SEL_WH                    HW_GPIO_FUNCTION_SEL
#define GPIO_FUNCTION_SEL_WH_MASK               0x1ff
#define GPIO_FUNCTION_SEL_WH__CLOCK_TEST_SEL_MASK       0x0c0
#define GPIO_FUNCTION_SEL_WH__GPIO26_SEL_MASK   0x020
#define GPIO_FUNCTION_SEL_WH__GPIO_INT1_SEL_MASK        0x010
#define GPIO_FUNCTION_SEL_WH__GPIO30_SEL_2_0_MASK       0x0e
#define GPIO_FUNCTION_SEL_WH__GPIO29_SEL_MASK   0x01
#define PA_ON_CTL_WB                            HW_PA_ON_CTL
#define PA_ON_CTL_WB_MASK               0x00
#define PA_ON_CTL_WB__DUAL_PA_ON_MASK   0x01
//MSM5000
#define PAD_CNTRL_WB                            HW_PAD_CNTRL
#define PAD_CNTRL_HIGH_DRIVE_MASK               0x01
//  SECTION     YAMN1 and RINGER M/N Counters
#define YAMN1_CLK_MDIV_MSB_WB                   HW_YAMN1_CLK_MDIV_MSB
#define YAMN1_CLK_MDIV_MSB_WB_MASK              0x00
#define YAMN1_CLK_MDIV_LSB_WB                   HW_YAMN1_CLK_MDIV_LSB
#define YAMN1_CLK_NDIV_MSB_WB                   HW_YAMN1_CLK_NDIV_MSB
#define YAMN1_CLK_NDIV_MSB_WB_MASK              0x01f
#define YAMN1_CLK_NDIV_LSB_WB                   HW_YAMN1_CLK_NDIV_LSB
#define YAMN1_CLK_DUTY_MSB_WB                   HW_YAMN1_CLK_DUTY_MSB
#define YAMN1_CLK_DUTY_MSB_WB_MASK              0x01f
#define YAMN1_CLK_DUTY_LSB_WB                   HW_YAMN1_CLK_DUTY_LSB
#define RINGER_MN_A_DUTY_LSB_WB                 HW_RINGER_MN_A_DUTY_LSB
#define RINGER_MN_A_DUTY_MSB_WB                 HW_RINGER_MN_A_DUTY_MSB
#define RINGER_MN_A_DUTY_MSB_WB_MASK            0x0f
#define RINGER_MN_A_NDIV_LSB_WB                 HW_RINGER_MN_A_NDIV_LSB
#define RINGER_MN_A_NDIV_MSB_WB                 HW_RINGER_MN_A_NDIV_MSB
#define RINGER_MN_A_NDIV_MSB_WB_MASK            0x01f
#define RINGER_MN_A_MDIV_WB                     HW_RINGER_MN_A_MDIV
#define RINGER_MN_A_MDIV_WB_MASK                0x07f
#define RINGER_MN_A_MDIV_WB__RINGER_MN_A_EN_MASK        0x040
#define RINGER_MN_A_MDIV_WB__RINGER_MN_A_MDIV_MASK      0x03f
#define RINGER_MN_B_DUTY_LSB_WB                 HW_RINGER_MN_B_DUTY_LSB
#define RINGER_MN_B_DUTY_MSB_WB                 HW_RINGER_MN_B_DUTY_MSB
#define RINGER_MN_B_DUTY_MSB_WB_MASK            0x0f
#define RINGER_MN_B_NDIV_LSB_WB                 HW_RINGER_MN_B_NDIV_LSB
#define RINGER_MN_B_NDIV_MSB_WB                 HW_RINGER_MN_B_NDIV_MSB
#define RINGER_MN_B_NDIV_MSB_WB_MASK            0x01f
#define RINGER_MN_B_MDIV_WB                     HW_RINGER_MN_B_MDIV
#define RINGER_MN_B_MDIV_WB_MASK                0x07f
#define RINGER_MN_B_MDIV_WB__RINGER_MN_B_EN_MASK        0x040
#define RINGER_MN_B_MDIV_WB__RINGER_MN_B_MDIV_MASK      0x03f
//  SECTION     Time Tick Interrupt and General Purpose Timer
#define TIME_TICK_CTL_WB                        HW_TIME_TICK_CTL
#define TIME_TICK_CTL_WB_MASK           0x07
#define TIME_TICK_CTL_WB__SYNC_MASK     0x04
#define TIME_TICK_CTL_WB__RATE_SEL_MASK         0x03
#define GPTIMER_COUNT_WB                        HW_GPTIMER_COUNT
#define GPTIMER_CTL_WB                          HW_GPTIMER_CTL
#define GPTIMER_CTL_WB_MASK             0x07
#define GPTIMER_CTL_WB__GPTIMER_AUTOKICK_EN_MASK        0x04
#define GPTIMER_CTL_WB__GPTIMER_KICK_EN_MASK    0x02
#define GPTIMER_CTL_WB__GPTIMER_EN_MASK         0x01
//  SECTION     PDM1 and PDM2
#define TCXO_PDM_CTL_WB                         HW_TCXO_PDM_CTL
#define TCXO_PDM_CTL_WB_MASK            0x03f
#define TCXO_PDM_CTL_WB__PDM1_POLARITY_MASK     0x020
#define TCXO_PDM_CTL_WB__PDM2_POLARITY_MASK     0x010
#define TCXO_PDM_CTL_WB__PDM1_EN_MASK   0x02
#define TCXO_PDM_CTL_WB__PDM2_EN_MASK   0x01
#define PDM2_CTL_WB                             HW_PDM2_CTL
#define PDM1_CTL_WB                             HW_PDM1_CTL
//  SECTION     ADC Interface
#define ADC_RESET_WB                            HW_ADC_RESET
#define ADC_RESET_WB_MASK               0x0ffff
#define ADC_DATA_WR_WB                          HW_ADC_DATA_WR
#define ADC_DATA_WR_WB_MASK             0x0ffff
//  SECTION     Auxillary Codec
#define CODEC_CTL1_WB                           HW_CODEC_CTL1
#define CODEC_CTL1_WB__SYNC_SEL_MASK    0x080
#define CODEC_CTL1_WB__AUX_CLK_VAL_MASK         0x040
#define CODEC_CTL1_WB__AUX_SYNC_VAL_MASK        0x020
#define CODEC_CTL1_WB__AUX_DOUT_VAL_MASK        0x010
#define CODEC_CTL1_WB__AUX_CODEC_SEL_MASK       0x08
#define CODEC_CTL1_WB__AUX_INVERT_MASK  0x04
#define CODEC_CTL1_WB__AUX_CLK_EN_MASK  0x02
#define CODEC_CTL1_WB__SCS_SEL_MASK     0x01
#define CODEC_CTL2_WB                           HW_CODEC_CTL2
#define CODEC_CTL2_WB_MASK              0x03f
#define CODEC_CTL2_WB__WDOGSTB_SCS_SEL_MASK     0x080
#define CODEC_CTL2_WB__RESERVED_MASK    0x040
#define CODEC_CTL2_WB__UART2_SEL_MASK   0x020
#define CODEC_CTL2_WB__CLK_SYNC_DOUT_LOOP_MASK  0x010
#define CODEC_CTL2_WB__DIN_LOOP_MASK    0x08
#define CODEC_CTL2_WB__PCM_CLK_DIR_MASK         0x04
#define CODEC_CTL2_WB__PCM_SYNC_DIR_MASK        0x02
#define CODEC_CTL2_WB__ONES_POLARITY_MASK       0x01
//  SECTION     MSM Clocks Control
//MSM5000, New bit, addr 0x740, 10 bits long
#define MSM_CLK_CTL1_WH                         HW_MSM_CLK_CTL1
#define MSM_CLK_CTL1_WH_MASK                          0xffff
#define MSM_CLK_CTL1_WH__RXCX8_MICRO_BYPASS_MASK      0x8000
#define MSM_CLK_CTL1_WH__DEC_CLK_EN_N_MASK            0x0400   //MSM5000, new bit
#define MSM_CLK_CTL1_WH__CDMA_RXDSP_CLK_EN_N_MASK     0x0200
#define MSM_CLK_CTL1_WH__SBI_CLK_EN_N_MASK            0x0100
#define MSM_CLK_CTL1_WH__MICRO_INTF_BYPASS_MASK       0x080
#define MSM_CLK_CTL1_WH__VOC_CLK_EN_N_MASK            0x040
#define MSM_CLK_CTL1_WH__CODEC_CLK_EN_N_MASK          0x020
#define MSM_CLK_CTL1_WH__DFM_CLK_EN_N_MASK            0x010
#define MSM_CLK_CTL1_WH__CDMA_RX_CLK_EN_N_MASK        0x08
#define MSM_CLK_CTL1_WH__CDMA_TX_CLK_EN_N_MASK        0x04
#define MSM_CLK_CTL1_WH__UART_CLK_EN_N_MASK           0x02
#define MSM_CLK_CTL1_WH__GENERAL_CLK_EN_N_MASK        0x01
#define MSM_CLK_CTL2_WH                         HW_MSM_CLK_CTL2
#define MSM_CLK_CTL2_WH_MASK                          0x07ff
//MSM5000, New bit, addr 0x744, 10 bits long
#define MSM_CLK_CTL2_WH__DEC_CLK_MASK                 0x0400   //MSM5000, new bit
#define MSM_CLK_CTL2_WH__CDMA_RXDSP_CLK_MASK          0x0200
#define MSM_CLK_CTL2_WH__SBI_CLK_MASK                 0x0100
#define MSM_CLK_CTL2_WH__RESERVED_MASK                0x080
#define MSM_CLK_CTL2_WH__VOC_CLK_MASK                 0x040
#define MSM_CLK_CTL2_WH__CODEC_CLK_MASK               0x020
#define MSM_CLK_CTL2_WH__DFM_CLK_MASK                 0x010
#define MSM_CLK_CTL2_WH__CDMA_RX_CLK_MASK             0x08
#define MSM_CLK_CTL2_WH__CDMA_TX_CLK_MASK             0x04
#define MSM_CLK_CTL2_WH__UART_CLK_MASK                0x02
#define MSM_CLK_CTL2_WH__GENERAL_CLK_MASK             0x01
#define MSM_CLK_CTL3_WH                         HW_MSM_CLK_CTL3
#define MSM_CLK_CTL3_WH_MASK                          0x07ff
//MSM5000, New bit, addr 0x748, 10 bits long
#define MSM_CLK_CTL3_WH__DEC_SYS_RESET_MASK           0x0400   //MSM5000, new bit
#define MSM_CLK_CTL3_WH__CDMA_RXDSP_SYS_RESET_MASK    0x0200
#define MSM_CLK_CTL3_WH__SBI_SYS_RESET_MASK           0x0100
#define MSM_CLK_CTL3_WH__PIN_TCXO_SYS_RESET_MASK      0x080
#define MSM_CLK_CTL3_WH__VOC_SYS_RESET_MASK           0x040
#define MSM_CLK_CTL3_WH__CODEC_SYS_RESET_MASK         0x020
#define MSM_CLK_CTL3_WH__DFM_SYS_RESET_MASK           0x010
#define MSM_CLK_CTL3_WH__CDMA_RX_SYS_RESET_MASK       0x08
#define MSM_CLK_CTL3_WH__CDMA_TX_SYS_RESET_MASK       0x04
#define MSM_CLK_CTL3_WH__UART_SYS_RESET_MASK          0x02
#define MSM_CLK_CTL3_WH__GENERAL_SYS_RESET_MASK       0x01
#define MSM_CLK_CTL4_WH                         HW_MSM_CLK_CTL4
//MSM5000, New bit, addr 0x74C, 13 bits long
#define MSM_CLK_CTL4_WH__UP_OSC_GAIN_MASK             0x01000
#define MSM_CLK_CTL4_WH__SLEEP_OSC_GAIN_MASK          0x0c00   //MSM5000, new bit
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RF_BYPASS_MASK     0x0200
#define MSM_CLK_CTL4_WH__SLEEP_OSC_RD_BYPASS_MASK     0x0100
#define MSM_CLK_CTL4_WH__SLEEP_XTAL_EN_MASK           0x080
#define MSM_CLK_CTL4_WH__SLEEP_CLK_SEL_MASK           0x040
#define MSM_CLK_CTL4_WH__TCXO_DIV4_MASK               0x020
#define MSM_CLK_CTL4_WH__TCXO_DIV2_MASK               0x01
#define MSM_CLK_CTL5_WH                         HW_MSM_CLK_CTL5
//MSM5000, New bit, addr 0x754, 13 bits long
#define MSM_CLK_CTL5_WH_MASK                          0x0fff
#define MSM_CLK_CTL5_WH__DEC_CLK_SEL_MASK             0x0700   //MSM5000, new bit
#define MSM_CLK_CTL5_WH__VOC_CLK_SEL_MASK             0x0070
#define MSM_CLK_CTL5_WH__VOC_OSC_EN_MASK              0x0008
#define MSM_CLK_CTL5_WH__UART_CLK_SEL_MASK            0x0804   //MSM5000, new bit
#define MSM_CLK_CTL5_WH__CDMA_RXDSP_CLK_SEL_MASK      0x0083   //MSM5000, new bit
//  SECTION     Test Registers
#define TEST_POINT_SEL_WH                       HW_TEST_POINT_SEL
#define TEST_POINT_SEL_WH_MASK          0x0ffff
#define TEST_POINT_DIN_WH                       HW_TEST_POINT_DIN
#define TEST_POINT_DIN_WH_MASK          0x0ffff
#define TEST_MEM_SEL_WH                         HW_TEST_MEM_SEL
#define TEST_MEM_ADDR_WH                        HW_TEST_MEM_ADDR
#define TEST_MEM_WRITE_WH                       HW_TEST_MEM_WRITE
#define TEST_MEM_WRITE_WH_MASK          0x0ffff
#define TEST_MEM_DIN_WH                         HW_TEST_MEM_DIN
#define TEST_MEM_DIN_WH_MASK            0x0ffff
#define ISDS_CLK_HALT_WB                        HW_ISDS_CLK_HALT
#define ISDS_CLK_HALT_WB_MASK           0x00
#define ISDS_CLK_HALT_WB__ISDS_DBG_EN_MASK      0x01
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     WEB CORE READ REGISTERS
//-----------------------------------------------------------------------------
//  SECTION     CDMA Sleep Control
#define SLEEP_STATUS_RB                         HW_SLEEP_STATUS
#define SLEEP_STATUS_RB_MASK            0x00
#define SLEEP_STATUS_RB__SLEEP_N_STATUS_MASK    0x01
#define SLEEP_XTAL_FREQ_ERR_RH                  HW_SLEEP_XTAL_FREQ_ERR
#define SLEEP_XTAL_FREQ_ERR_RH_MASK             0x0ffff
#define SLEEP_COUNT_LSHW_RH                     HW_SLEEP_COUNT_LSHW
#define SLEEP_COUNT_LSHW_RH_MASK                0x0ffff
#define SLEEP_COUNT_MSHW_RH                     HW_SLEEP_COUNT_MSHW
#define SLEEP_COUNT_MSHW_RH_MASK                0x0ffff
#define POWER_DOWN_CHIPX8_COUNT_RH              HW_POWER_DOWN_CHIPX8_COUNT
#define POWER_DOWN_CHIPX8_COUNT_RH_MASK                 0x03ff
//  SECTION     Interrupt Controller
#define INT_STATUS_0_RH                         HW_INT_STATUS_0
#define INT_STATUS_0_RH_MASK            0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define INT_STATUS_1_RH                         HW_INT_STATUS_1
#define INT_STATUS_1_RH_MASK            0x0efff
#define IRQ_MASK_RD_0_RH                        HW_IRQ_MASK_RD_0
#define IRQ_MASK_RD_0_RH_MASK           0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define IRQ_MASK_RD_1_RH                        HW_IRQ_MASK_RD_1
#define IRQ_MASK_RD_1_RH_MASK           0x0efff
#define FIQ_MASK_RD_0_RH                        HW_FIQ_MASK_RD_0
#define FIQ_MASK_RD_0_RH_MASK           0x0ffff
//MSM5000, new interrupts (bits: 15,14,13,11)
#define FIQ_MASK_RD_1_RH                        HW_FIQ_MASK_RD_1
#define FIQ_MASK_RD_1_RH_MASK           0x0efff
//  SECTION     GPIO
#define GPIO_IN_0_RB                            HW_GPIO_IN_0
#define GPIO_IN_0_RB_MASK               0x03f
#define GPIO_IN_1_RB                            HW_GPIO_IN_1
#define GPIO_IN_2_RB                            HW_GPIO_IN_2
#define GPIO_IN_2_RB_MASK               0x01f
#define GPIO_IN_2_RB__PA_ON_RD_MASK     0x010
#define GPIO_IN_2_RB__GPIO_IN_2_MASK    0x0f
#define GPIO_IN_3_RB                            HW_GPIO_IN_3
#define GPIO_IN_4_RB                            HW_GPIO_IN_4
#define GPIO_IN_4_RB_MASK               0x03f
#define GPIO_INT_IN_RB                          HW_GPIO_INT_IN
#define GPIO_INT_IN_RB_MASK             0x01f
#define KEYSENSE_RD_RB                          HW_KEYSENSE_RD
#define KEYSENSE_RD_RB_MASK             0x01f
#define PA_ON_STATUS_RB                         HW_PA_ON_STATUS
#define PA_ON_STATUS_RB__PA_ON2_STATUS_MASK     0x080
#define PA_ON_STATUS_RB__DEBUG_STATE_MASK       0x040
#define PA_ON_STATUS_RB__PA_ON_STATUS_MASK      0x020
#define PA_ON_STATUS_RB__DFM_TXCTL_STATUS_MASK  0x08
#define PA_ON_STATUS_RB__CDMA_FM_SEL_STATUS_MASK        0x04
#define PA_ON_STATUS_RB__TX_PUNCT_STATUS_MASK   0x02
#define PA_ON_STATUS_RB__SYNTH_LOCK_STATUS_MASK         0x01
//  SECTION     Time Tick Interrupt and General Purpose Timer
#define TIME_TICK_INT_MSB_RB                    HW_TIME_TICK_INT_MSB
//  SECTION     ADC Interface
#define ADC_STAT_RB                             HW_ADC_STAT
#define ADC_STAT_RB_MASK                0x03f
#define ADC_STAT_RB__ADC_CLK_RD_MASK    0x020
#define ADC_STAT_RB__ADC_DATA_RD_MASK   0x010
#define ADC_STAT_RB__ADC_ENABLE_RD_MASK         0x08
#define ADC_STAT_RB__UNDERRUN_STATUS_MASK       0x02
#define ADC_STAT_RB__ADC_EOC_MASK       0x01
#define ADC_DATA_RD_RB                          HW_ADC_DATA_RD
//  SECTION     Version Register
//MSM5000, Hardware revision, new bits, addr 0x75C, 12 bits long
#define HW_REVISION_NUMBER_RH                   HW_HW_REVISION_NUMBER
#define HW_REVISION_NUMBER_RH_MASK              0x0fff //MSM5000, New bits
#define HW_REVISION_NUMBER_RH__PROCESS_ID_MASK  0x0f00
#define HW_REVISION_NUMBER_RH__MSM_ID_MASK      0x0f0
#define HW_REVISION_NUMBER_RH__VERSION_ID_MASK  0x0f
//  SECTION     Test Registers
#define TEST_POINT_READ0_RH                     HW_TEST_POINT_READ0
#define TEST_POINT_READ0_RH_MASK                0x0ffff
#define TEST_POINT_READ1_RH                     HW_TEST_POINT_READ1
#define TEST_POINT_READ1_RH_MASK                0x0ffff
#define TEST_MEM_READ0_RH                       HW_TEST_MEM_READ0
#define TEST_MEM_READ0_RH_MASK          0x0ffff
#define TEST_MEM_READ1_RH                       HW_TEST_MEM_READ1
#define TEST_MEM_READ1_RH_MASK          0x0ffff
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     SBI WRITE REGISTERS
//-----------------------------------------------------------------------------
#define SBI_CLK_CTL_WH                          HW_SBI_CLK_CTL
#define SBI_CLK_CTL_WH__MICRO_RESET_MASK        0x080
#define SBI_CLK_CTL_WH__CNT_EN_MASK     0x040
#define SBI_CLK_CTL_WH__CLKCTL_MASK     0x03f
#define SBI_CTL_WH                              HW_SBI_CTL
#define SBI_CTL_WH_MASK                 0x0fff
#define SBI_CTL_WH__MSM_SBI_EN_MASK     0x0800
#define SBI_CTL_WH__OVR_MODE_MASK       0x0400
#define SBI_CTL_WH__DIAG_MODE_MASK      0x0200
#define SBI_CTL_WH__LAST_WORD_MASK      0x0100
#define SBI_CTL_WH__I3Q_MODE_MASK       0x0c0
#define SBI_CTL_WH__SLAVE_ID_MASK       0x03f
#define SBI_BYPASS_WH                           HW_SBI_BYPASS
#define SBI_BYPASS_WH_MASK              0x07
#define SBI_BYPASS_WH__OVR_SBCK_MASK    0x04
#define SBI_BYPASS_WH__OVR_SBST_MASK    0x02
#define SBI_BYPASS_WH__OVR_SBDT_MASK    0x01
#define SBI_WR_WH                               HW_SBI_WR
#define SBI_WR_WH_MASK          0x0ffff
#define SBI_WR_WH__RD_WR_N_MASK         0x08000
#define SBI_WR_WH__REG_ADDR_MASK        0x07f00
#define SBI_WR_WH__REG_DATA_MASK        0x0ff
#define SBI_START_CTL_WH                        HW_SBI_START_CTL
#define SBI_START_CTL_WH_MASK           0x00
#define SBI_START_CTL_WH__START_FLAG_MASK       0x01
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     SBI READ REGISTERS
//-----------------------------------------------------------------------------
#define SBI_STATUS_RH                           HW_SBI_STATUS
#define SBI_STATUS_RH_MASK              0x0ffff
#define SBI_STATUS_RH__I3Q_MODE_RD_MASK         0x0c000
#define SBI_STATUS_RH__SLAVE_ID_RD_MASK         0x03f00
#define SBI_STATUS_RH__OVR_MODE_RD_MASK         0x080
#define SBI_STATUS_RH__DIAG_MODE_RD_MASK        0x040
#define SBI_STATUS_RH__MSM_SBI_EN_RD_MASK       0x020
#define SBI_STATUS_RH__MCHN_STATUS_MASK         0x018
#define SBI_STATUS_RH__MCHN_BUSY_MASK   0x04
#define SBI_STATUS_RH__RDBUF_FULL_MASK  0x02
#define SBI_STATUS_RH__WRBUF_FULL_MASK  0x01
#define SBI_BYPASS_RD_RH                        HW_SBI_BYPASS_RD
#define SBI_BYPASS_RD_RH_MASK           0x01f
#define SBI_BYPASS_RD_RH__SBCK_PIN_STATUS_MASK  0x010
#define SBI_BYPASS_RD_RH__OVR_SBCK_RD_MASK      0x08
#define SBI_BYPASS_RD_RH__OVR_SBST_RD_MASK      0x04
#define SBI_BYPASS_RD_RH__OVR_SBDT_RD_MASK      0x02
#define SBI_BYPASS_RD_RH__SBDT_PIN_STATUS_MASK  0x01
#define SBI_RD_RH                               HW_SBI_RD
#define SBI_RD_RH_MASK          0x0ffff
#define SBI_RD_RH__REG_ADDR_MASK        0x0ff00
#define SBI_RD_RH__REG_DATA_MASK        0x0ff

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     UART WRITE REGISTERS
//-----------------------------------------------------------------------------
//MSM5000, Increase in RFR level.
#define UART_MR1_WH                             HW_UART_MR1
#define UART_MR1_WH__RX_RDY_CTL_MASK            0x080
#define UART_MR1_WH__CTS_CTL_MASK               0x040
#define UART_MR1_WH__AUTO_RFR_LEVEL_MASK        0x073f //MSM5000, new bits
#define UART_MR2_WB                             HW_UART_MR2
#define UART_MR2_WB_MASK                        0x07f
#define UART_MR2_WB__LOOPBACK_MASK              0x080
#define UART_MR2_WB__ERROR_MODE_MASK            0x040
#define UART_MR2_WB__BITS_PER_CHAR_MASK         0x030
#define UART_MR2_WB__STOP_BIT_LEN_MASK          0x0c
#define UART_MR2_WB__PARITY_MODE_MASK           0x03
#define UART_CSR_WB                             HW_UART_CSR
#define UART_CSR_WB__UART_RX_CLK_SEL_MASK       0x0f0
#define UART_CSR_WB__UART_TX_CLK_SEL_MASK       0x0f
#define UART_TF_WB                              HW_UART_TF
#define UART_CR_WB                              HW_UART_CR
#define UART_CR_WB__CHANNEL_COMMAND_MASK        0x0f0
#define UART_CR_WB__UART_TX_DISBALE_MASK        0x08
#define UART_CR_WB__UART_TX_EN_MASK             0x04
#define UART_CR_WB__UART_RX_DISABLE_MASK        0x02
#define UART_CR_WB__UART_RX_EN_MASK             0x01
#define UART_IMR_WB                             HW_UART_IMR
#define UART_IMR_WB_MASK                        0x07f
#define UART_IMR_WB__CURRENT_CTS_MASK           0x040
#define UART_IMR_WB__DELTA_CTS_MASK             0x020
#define UART_IMR_WB__RXLEV_MASK                 0x010
#define UART_IMR_WB__RXSTALE_MASK               0x08
#define UART_IMR_WB__RXBREAK_MASK               0x04
#define UART_IMR_WB__RXHUNT_MASK                0x02
#define UART_IMR_WB__TXLEV_MASK                 0x01
//MSM5000, Increase in Stale time out as result of the increase in FIFO size.
#define UART_IPR_WH                             HW_UART_IPR
#define UART_IPR_WH__STALE_TIMEOUT_MSB_MASK     0xf80       //MSM5000, New bits
#define UART_IPR_WH__SAMPLE_DATA_MASK           0x040
#define UART_IPR_WH__RXSTALE_LAST_MASK          0x020
#define UART_IPR_WH__STALE_TIMEOUT_LSB_MASK     0x01f
//MSM5000, Increase in TX FIFO size
#define UART_TFWR_WH                            HW_UART_TFWR
#define UART_TFWR_WH_MASK                       0x03f
#define UART_TFWR_WH__TFW_MASK                  0x03f
//MSM5000, Increase in RX FIFO SIZE
#define UART_RFWR_WH                            HW_UART_RFWR
#define UART_RFWR_WH_MASK                       0x1ff
#define UART_RFWR_WH__RFW_MASK                  0x1ff
#define UART_HCR_WB                             HW_UART_HCR
#define UART_MREG_WB                            HW_UART_MREG
#define UART_NREG_WB                            HW_UART_NREG
#define UART_DREG_WB                            HW_UART_DREG
#define UART_MNDREG_WB                          HW_UART_MNDREG
#define UART_MNDREG_WB_MASK                     0x03f
#define UART_MNDREG_WB__MREG_LSB_MASK           0x020
#define UART_MNDREG_WB__NREG_LSB_MASK           0x01c
#define UART_MNDREG_WB__DREG_LSB_MASK           0x03
#define UART_IRDA_WB                            HW_UART_IRDA
#define UART_IRDA_WB_MASK                       0x0f
#define UART_IRDA_WB__IRDA_LOOPBACK_MASK        0x08
#define UART_IRDA_WB__INVERT_IRDA_TX_MASK       0x04
#define UART_IRDA_WB__INVERT_IRDA_RX_MASK       0x02
#define UART_IRDA_WB__IRDA_EN_MASK      0x01
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     UART READ REGISTERS
//-----------------------------------------------------------------------------
#define UART_SR_RB                              HW_UART_SR
#define UART_SR_RB__HUNT_CHAR_MASK              0x080
#define UART_SR_RB__RX_BREAK_MASK               0x040
#define UART_SR_RB__PAR_FRAME_ERR_MASK          0x020
#define UART_SR_RB__UART_OVERRUN_MASK           0x010
#define UART_SR_RB__TXEMT_MASK                  0x08
#define UART_SR_RB__TXRDY_MASK                  0x04
#define UART_SR_RB__RXFULL_MASK                 0x02
#define UART_SR_RB__RXRDY_MASK                  0x01
#define UART_RF_RB                              HW_UART_RF
#define UART_MISR_RB                            HW_UART_MISR
#define UART_ISR_RB                             HW_UART_ISR
#define UART_ISR_RB_MASK                        0x07f
#define UART_ISR_RB__CURRENT_CTS_MASK           0x040
#define UART_ISR_RB__DELTA_CTS_MASK             0x020
#define UART_ISR_RB__RXLEV_MASK                 0x010
#define UART_ISR_RB__RXSTALE_MASK               0x08
#define UART_ISR_RB__RXBREAK_MASK               0x04
#define UART_ISR_RB__RXHUNT_MASK                0x02
#define UART_ISR_RB__TXLEV_MASK                 0x01

//-----------------------------------------------------------------------------
//  ASB MEMORY SPACE ASB_BASE to (ASB_BASE + 0x0EC
//
//  ARM Clock/Power Control
//  ARM Reset/Pause Control
//  ARM Memory Map Decoder
//  Memory Bus Sizer
//  MSM Peripheral Bridge
//-----------------------------------------------------------------------------
#define ASB_BASE                        HW_ASB_BASE
//-----------------------------------------------------------------------------
//  SECTION     ASB PERIPHERAL WRITE REGISTERS
//-----------------------------------------------------------------------------
//  SECTION     ARM Clock/Power Controller
#define UP_CLK_CTL1_WH                          HW_UP_CLK_CTL1
#define UP_CLK_CTL1_WH_MASK             0x01f
#define UP_CLK_CTL1_WH__UP_CLK_SRC_SEL_MASK     0x030
#define UP_CLK_CTL1_WH__XTAL_WU_DURATION_MASK   0x0e
#define UP_CLK_CTL1_WH__POWER_DOWN_MASK         0x01
#define UP_CLK_CTL2_WH                          HW_UP_CLK_CTL2
#define UP_CLK_CTL2_WH_MASK             0x07
#define UP_CLK_CTL2_WH__MCLK_RATE_MASK  0x07
//  SECTION     ARM Reset/Pause Controller
#define PAUSE_TIMER_WH                          HW_PAUSE_TIMER
#define PAUSE_TIMER_WH_MASK             0x03ff
#define ASB_DECODE_CTL_WH                       HW_ASB_DECODE_CTL
#define ASB_DECODE_CTL_WH_MASK          0x00
#define ASB_DECODE_CTL_WH__DECODE_CYCLE_EN_MASK         0x01
//  SECTION     ARM Memory Map Decoder and Bus Sizer
#define MEMORY_WAIT1_WH                         HW_MEMORY_WAIT1
#define MEMORY_WAIT1_WH_MASK            0x03fff
#define MEMORY_WAIT1_WH__EEPROM_BASE_MASK       0x03f00
#define MEMORY_WAIT1_WH__ROM_HWORD_WAIT_MASK    0x0f0
#define MEMORY_WAIT1_WH__EEPROM_HWORD_WAIT_MASK         0x0f
#define MEMORY_WAIT2_WH                         HW_MEMORY_WAIT2
#define MEMORY_WAIT2_WH_MASK            0x03fff
#define MEMORY_WAIT2_WH__PCS6_BASE_MASK         0x03f00
#define MEMORY_WAIT2_WH__RAM_MIN_WAIT_MASK      0x0f0
#define MEMORY_WAIT2_WH__PCS6_MIN_WAIT_MASK     0x0f
#define GP_CS_N_WAIT_WH                         HW_GP_CS_N_WAIT
#define GP_CS_N_WAIT_WH_MASK            0x0f
#define GP_CS_N_WAIT_WH__GP_WAIT_MASK   0x01f
#define MSM_WAIT_WH                             HW_MSM_WAIT
#define MSM_WAIT_WH_MASK                0x0f
#define MSM_WAIT_WH__MSM_WAIT_MASK      0x0f
#define CS_CTL_WH                               HW_CS_CTL
#define CS_CTL_WH_MASK          0x07f
#define CS_CTL_WH__DEAD_CYCLE_DISABLE_MASK      0x080
#define CS_CTL_WH__NATIVE_BUS_OE_MASK   0x040
#define CS_CTL_WH__GP_CS_EN_MASK        0x020
#define CS_CTL_WH__LCD_E_EN_MASK        0x010
#define CS_CTL_WH__LCD_CS_EN_MASK       0x08
#define CS_CTL_WH__PCS6_EN_MASK         0x04
#define CS_CTL_WH__RAM_CS_EN_MASK       0x02
#define CS_CTL_WH__EEPROM_CS_EN_MASK    0x01
#define BSIZER_CTL1_WH                          HW_BSIZER_CTL1
#define BSIZER_CTL1_WH_MASK             0x0fff
#define BSIZER_CTL1_WH__ROM_WR_CNT_MASK         0x0e00
#define BSIZER_CTL1_WH__ROM_RD_CNT_MASK         0x01c0
#define BSIZER_CTL1_WH__EEPROM_WR_CNT_MASK      0x038
#define BSIZER_CTL1_WH__EEPROM_RD_CNT_MASK      0x07
#define BSIZER_CTL2_WH                          HW_BSIZER_CTL2
#define BSIZER_CTL2_WH_MASK             0x03fff
#define BSIZER_CTL2_WH__RAM_BSIZE_EN_MASK       0x02000
#define BSIZER_CTL2_WH__PCS6_BSIZE_EN_MASK      0x01000
#define BSIZER_CTL2_WH__RAM_WR_CNT_MASK         0x0e00
#define BSIZER_CTL2_WH__RAM_RD_CNT_MASK         0x01c0
#define BSIZER_CTL2_WH__PCS6_WR_CNT_MASK        0x038
#define BSIZER_CTL2_WH__PCS6_RD_CNT_MASK        0x07
#define LCD_CTL_WH                              HW_LCD_CTL
#define LCD_CTL_WH_MASK                 0x01fff
#define LCD_CTL_WH__LCD_WAIT_MASK       0x01f00
#define LCD_CTL_WH__LCD_E_SETUP_MASK    0x0f0
#define LCD_CTL_WH__LCD_E_HIGH_MASK     0x0f
#define GPIO_INT_ADDR_SEL_WH                    HW_GPIO_INT_ADDR_SEL
#define GPIO_INT_ADDR_SEL_WH_MASK               0x07
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_22_EN_MASK        0x04
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_21_EN_MASK        0x02
#define GPIO_INT_ADDR_SEL_WH__ADDRESS_20_EN_MASK        0x01
//-----------------------------------------------------------------------------
//  SECTION     ASB PERIPHERAL READ REGISTERS
//-----------------------------------------------------------------------------
//  SECTION     ARM Reset/Pause Controller
#define RESET_STATUS_RH                 HW_RESET_STATUS
#define RESET_STATUS_RH_MASK            0x00

//-----------------------------------------------------------------------------
//  SECTION     INTERNAL MEMORY
//-----------------------------------------------------------------------------
//MSM5000, new register, addr 0x0a4, 3 bits long
#define IMEM_CTL_WB                             HW_IMEM_CTL
#define IMEM_CTL_WB__IMEM_EN_MASK               0x01
#define IMEM_CTL_WB__IMEM_CNT_MASK              0x06

#define IMEM_CTL_RB                             HW_IMEM_CTL
#define IMEM_CTL_RB__IMEM_EN_MASK               0x01

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW SEARCHER WRITE REGISTERS
//-----------------------------------------------------------------------------
#define SEARCH2_CTL_WB                          HW_SRCH2_CTL
#define SEARCH2_CTL_WB_MASK                     0x01f
#define SEARCH2_CTL_WB__ALL_ENERGIES_MASK       0x010
#define SEARCH2_CTL_WB__RESET_QUEUE_PTRS_MASK   0x08
#define SEARCH2_CTL_WB__RESET_RESULTS_PTRS_MASK 0x04
#define SEARCH2_CTL_WB__SEARCH_ABORT_MASK       0x02
#define SEARCH2_CTL_WB__STATUS_DUMP_MASK        0x01

#define SEARCH2_QUEUE_WH                        HW_SRCH2_QUEUE
#define SEARCH2_QUEUE_WH_MASK                   0x0ffff

#define SEARCH_STATUS_DUMP_CTL_WB               HW_SRCH_STATUS_DUMP_CTL
#define SEARCH_STATUS_DUMP_CTL_WB_MASK                      0x03
#define SEARCH_STATUS_DUMP_CTL_WB__SEARCH2_DUMP_EN_MASK     0x02
#define SEARCH_STATUS_DUMP_CTL_WB__SEARCH1_DUMP_EN_MASK     0x01

#define SEARCH_WALSH_NUM_WB                     HW_SRCH_WALSH_NUM

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW SEARCHER READ REGISTERS
//-----------------------------------------------------------------------------
#define SEARCH2_RESULTS_RH                      HW_SRCH2_RESULTS
#define SEARCH2_RESULTS_RH_MASK                          0x0ffff

#define SEARCH2_QUEUE_PTRS_RH                   HW_SRCH2_QUEUE_PTRS
#define SEARCH2_QUEUE_PTRS_RH_MASK                       0x0ffff
#define SEARCH2_QUEUE_PTRS_RH__QUEUE_WRITE_PTR_MASK      0x0f000
#define SEARCH2_QUEUE_PTRS_RH__QUEUE_WRITE_SUB_PTR_MASK  0x0f00
#define SEARCH2_QUEUE_PTRS_RH__QUEUE_READ_PTR_MASK       0x0f0
#define SEARCH2_QUEUE_PTRS_RH__QUEUE__READ_SUB_PTR_MASK  0x0f

#define SEARCH2_RESULTS_PTRS_RH                 HW_SRCH2_RESULTS_PTRS
#define SEARCH2_RESULTS_PTRS_RH_MASK                         0x0ffff
#define SEARCH2_RESULTS_PTRS_RH__RESULTS_WRITE_PTR_MASK      0x0f000
#define SEARCH2_RESULTS_PTRS_RH__RESULTS_WRITE_SUB_PTR_MASK  0x0f00
#define SEARCH2_RESULTS_PTRS_RH__RESULTS_READ_PTR_MASK       0x0f0
#define SEARCH2_RESULTS_PTRS_RH__RESULTS_READ_SUB_PTR_MASK   0x0f

#define SEARCH2_POSITION_HIGH_RH                HW_SRCH2_POSITION_HIGH
#define SEARCH2_POSITION_HIGH_RH_MASK           0x03

#define SEARCH2_POSITION_LOW_RH                 HW_SRCH2_POSITION_LOW
#define SEARCH2_POSITION_LOW_RH_MASK            0x0ffff

#define SEARCH_STATUS_DUMP_STATUS_RB            HW_SRCH_STATUS_DUMP_STATUS
#define SEARCH_STATUS_DUMP_STATUS_RB_MASK                      0x03
#define SEARCH_STATUS_DUMP_STATUS_RB__SEARCH2_DUMP_EN_MASK     0x02
#define SEARCH_STATUS_DUMP_STATUS_RB__SEARCH1_DUMP_EN_MASK     0x01

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW DEINTERLEAVER (MSM5000) WRITE REGISTERS
//-----------------------------------------------------------------------------
#define DINT_RESET_WH                           HW_DINT_RESET
#define DINT_RESET_WH_MASK                      0x0ffff

#define DINT_CFG_WH                             HW_DINT_CFG
#define DINT_CFG_WH_MASK                        0x0ffff
#define DINT_CFG_WH__L_UPPER_RAM_SEL_MASK       0x08000
#define DINT_CFG_WH__L_ODD_RAM_PAGE_SEL_MASK    0x04000
#define DINT_CFG_WH__M_UPPER_RAM_SEL_MASK       0x02000
#define DINT_CFG_WH__M_ODD_RAM_PAGE_SEL_MASK    0x01000
#define DINT_CFG_WH__N_UPPER_RAM_SEL_MASK       0x0800
#define DINT_CFG_WH__N_ODD_RAM_PAGE_SEL_MASK    0x0400
#define DINT_CFG_WH__P_UPPER_RAM_SEL_MASK       0x0200
#define DINT_CFG_WH__P_ODD_RAM_PAGE_SEL_MASK    0x0100
#define DINT_CFG_WH__CHAN_RAM_ALLOC_MASK        0x0ff

#define DINT_OTD_CFG_WH                         HW_DINT_OTD_CFG
#define DINT_OTD_CFG_WH_MASK                    0x00003
#define DINT_OTD_CFG_WH__OTD_SEL_MASK           0x02
#define DINT_OTD_CFG_WH__CH2_CODE_TYPE_MASK     0x01

#define DINT_CH0_CFG_WH                         HW_DINT_CH0_CFG
#define DINT_CH0_CFG_WH_MASK                    0x0ffff
#define DINT_CH0_CFG_WH__SYNC_CHAN_80M_BND_MASK 0x040
#define DINT_CH0_CFG_WH__NEXT_20M_BND_MASK      0x020
#define DINT_CH0_CFG_WH__SEQUENCE_REP_MASK      0x018
#define DINT_CH0_CFG_WH__NUM_SUB_CHANS_MASK     0x07

#define DINT_CH1_CFG_WH                         HW_DINT_CH1_CFG
#define DINT_CH1_CFG_WH_MASK                    0x0ffff
#define DINT_CH1_CFG_WH__SYNC_CHAN_80M_BND_MASK 0x040
#define DINT_CH1_CFG_WH__NEXT_20M_BND_MASK      0x020
#define DINT_CH1_CFG_WH__SEQUENCE_REP_MASK      0x018
#define DINT_CH1_CFG_WH__NUM_SUB_CHANS_MASK     0x07

#define DINT_CH2_CFG_WH                         HW_DINT_CH2_CFG
#define DINT_CH2_CFG_WH_MASK                    0x0ffff
#define DINT_CH2_CFG_WH__SYNC_CHAN_80M_BND_MASK 0x040
#define DINT_CH2_CFG_WH__NEXT_20M_BND_MASK      0x020
#define DINT_CH2_CFG_WH__SEQUENCE_REP_MASK      0x018
#define DINT_CH2_CFG_WH__NUM_SUB_CHANS_MASK     0x07

#define TD_BLOCK_SIZE_WH                        HW_TD_BLOCK_SIZE
#define TD_BLOCK_SIZE_WH_MASK                   0x0ffff

#define TD_INTLV_CFG_LO_WH                      HW_TD_INTLV_CFG_LO
#define TD_INTLV_CFG_LO_WH_MASK                 0x0e000
#define TD_INTLV_CFG_LO_WH__ROW_WIDTH_MASK      0x0e000

#define TD_INTLV_CFG_HI_WH                      HW_TD_INTLV_CFG_HI
#define TD_INTLV_CFG_HI_WH_MASK                 0x0ffff
#define TD_INTLV_CFG_HI_WH__INTLV_COLS_MASK     0x0ff00
#define TD_INTLV_CFG_HI_WH__INTLV_ROWS_MASK     0x0ff

#define TD_PUNCT_LO_WH                          HW_TD_PUNCT_LO
#define TD_PUNCT_LO_WH_MASK                     0x0ffff
#define TD_PUNCT_LO_WH__PUNCT_PATTERN_MASK      0x0ff00
#define TD_PUNCT_LO_WH__PUNCT_LENGTH_MASK       0x0f8
#define TD_PUNCT_LO_WH__TD_DEC_INT_EN_MASK      0x04
#define TD_PUNCT_LO_WH__CODE_RATE_MASK          0x03

#define TD_PUNCT_HI_WH                          HW_TD_PUNCT_HI
#define TD_PUNCT_HI_WH_MASK                     0x0ffff
#define TD_PUNCT_HI_WH__PUNCT_PATTERN_MASK      0x0ffff

#define TD_INTLV_SIZE_LO_WH                     HW_TD_INTLV_SIZE_LO
#define TD_INTLV_SIZE_LO_WH_MASK                         0x0ffff
#define TD_INTLV_SIZE_LO_WH__EARLY_TERMINATION_EN_MASK   0x08000
#define TD_INTLV_SIZE_LO_WH__TD_INTLV_LEN_MASK           0x07fff

#define TD_INTLV_SIZE_HI_WH                     HW_TD_INTLV_SIZE_HI
#define TD_INTLV_SIZE_HI_WH_MASK                0x0ffff
#define TD_INTLV_SIZE_HI_WH__MAX_ITER_NUM_MASK  0x0fc00
#define TD_INTLV_SIZE_HI_WH__MIN_ITER_NUM_MASK  0x03ff

#define TD_PARAMS_LO_WH                         HW_TD_PARAMS_LO
#define TD_PARAMS_LO_WH_MASK                       0x0ffff
#define TD_PARAMS_LO_WH__FRM_20M_RATE_HYP_MASK     0x0c000
#define TD_PARAMS_LO_WH__SLIDE_WIN_LEN_MASK        0x03e00
#define TD_PARAMS_LO_WH__PARTIAL_WIN_LENGTH_MASK   0x01f0
#define TD_PARAMS_LO_WH__BETA_SELECT_MASK          0x0f

#define TD_PARAMS_HI_WH                         HW_TD_PARAMS_HI
#define TD_PARAMS_HI_WH_MASK                    0x0fc7f
#define TD_PARAMS_HI_WH__MIN_NUM_CRC_PASS_MASK  0x0fc00
#define TD_PARAMS_HI_WH__NUM_WINDOWS_MASK       0x07f

#define DINT_PKT_OFFSET_WH                      HW_DINT_PKT_OFFSET
#define DINT_PKT_OFFSET_WH_MASK                 0x007f
#define DINT_PKT_OFFSET_WH__PACKET_OFFSET_MASK  0x078
#define DINT_PKT_OFFSET_WH__PACKET_TYPE_MASK    0x007

#define DINT_PKT_CFG_WH                         HW_DINT_PKT_CFG
/*DINT_PKT_CFG_WH: 0000 PK_INTLV_CFG_LO*/
#define DINT_PKT_CFG_WH_MASK                    0x0ffff
#define DINT_PKT_CFG_WH__ROW_WIDTH_MASK         0x0e000
#define DINT_PKT_CFG_WH__CHAINBACK_DEPTH_MASK   0x01000
#define DINT_PKT_CFG_WH__YM_THRESH_MASK         0x0f00
#define DINT_PKT_CFG_WH__SMT_SCALAR_MASK        0x0ff

/*DINT_PKT_CFG_WH: 0001 PK_INTLV_CFG_HI*/
#define DINT_PKT_CFG_WH__INTLV_COLS_MASK        0x0ff00

/*DINT_PKT_CFG_WH: 0010 PK_BLK_SIZE_LO*/
#define DINT_PKT_CFG_WH__FRAME_OR_PAGE_SIZE_MASK   0x0ffff

/*DINT_PKT_CFG_WH: 0011 PK_BLK_SIZE_HI*/
#define DINT_PKT_CFG_WH__LTU_LEN_MASK           0x0fe00
#define DINT_PKT_CFG_WH__PUNCT_LENGTH_MASK      0x01f0
#define DINT_PKT_CFG_WH__SYMBOL_REPEAT_MASK     0x0c
#define DINT_PKT_CFG_WH__CODE_RATE_MASK         0x03

/*DINT_PKT_CFG_WH: 0100 PK_PKT_PUNCT_LO*/
#define DINT_PKT_CFG_WH__LO_PUNCT_PATTERN_MASK     0x0ff00
#define DINT_PKT_CFG_WH__RDCNT_EN_SEL_MASK      0x010
#define DINT_PKT_CFG_WH__NUM_LTU_FRM_MASK       0x0f

/*DINT_PKT_CFG_WH: 0101 PK_PKT_PUNCT_HI*/
#define DINT_PKT_CFG_WH__HI_PUNCT_PATTERN_MASK     0x0ffff

/*DINT_PKT_CFG_WH: 0110 PK_PL_CRC_LO*/
#define DINT_PKT_CFG_WH__PL_CRC_MASK            0x0ffff

/*DINT_PKT_CFG_WH: 0111 PK_PL_CRC_HI*/
#define DINT_PKT_CFG_WH__LTU_CRC_MASK           0x0ffff

#define DINT_TASK_OFFSET_WH                     HW_DINT_TASK_OFFSET
#define DINT_TASK_OFFSET_WH_MASK                0x0ffff

#define TASK_LIST_LO_WH                         HW_DINT_TASK_LIST
#define TASK_LIST_LO_WH_MASK                    0x0fff7
#define TASK_LIST_LO_WH__ACTIVE_TASK_MASK       0x08000
#define TASK_LIST_LO_WH__SUB_CHANS_MASK         0x04000
#define TASK_LIST_LO_WH__SYNC_CHAN_MASK         0x02000
#define TASK_LIST_LO_WH__DEC_INTR_EN_MASK       0x01000
#define TASK_LIST_LO_WH__CHAN_ID_MASK           0x00c00
#define TASK_LIST_LO_WH__FRM_HYP_MASK           0x00300
#define TASK_LIST_LO_WH__PACKET_OFFSET_MASK     0x000f0
#define TASK_LIST_LO_WH__BIT_SHIFT_MASK         0x00007


#define TASK_LIST_HI_WH                         HW_DINT_TASK_LIST
#define TASK_LIST_HI_WH_MASK                          0x0fff
#define TASK_LIST_HI_WH__VD_OUTBUF_ADDR_OFFSET_MASK   0x0fff

#define TST_SYNC_DINT_WH                        HW_TST_SYNC_DINT
#define TST_SYNC_DINT_WH_MASK                   0x0ffff

#define TST_CH0_SYMS_WH                         HW_TST_CH0_SYMS
#define TST_CH0_SYMS_WH_MASK                    0x0ffff

#define TST_CH1_SYMS_WH                         HW_TST_CH1_SYMS
#define TST_CH1_SYMS_WH_MASK                    0x0ffff

#define TST_CH2_SYMS_WH                         HW_TST_CH2_SYMS
#define TST_CH2_SYMS_WH_MASK                    0x0ffff

#define TST_SEL_DINT_WH                         HW_TST_SEL_DINT
#define TST_SEL_DINT_WH_MASK                    0x0ffff

#define TD_MIN_LLR_WH                           HW_TD_MIN_LLR_THRESH
#define TD_MIN_LLR_WH_MASK                      0x00fff
#define TD_MIN_LLR_WH__MIN_LLR_THRESH_MASK      0x0fff

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW DEINTERLEAVER (MSM5000) READ REGISTERS
//-----------------------------------------------------------------------------
#define DEINT_TASK_STATUS_RH                    HW_DEINT_TASK_STATUS
#define DEINT_TASK_STATUS_RH_MASK               0x0003f
#define DEINT_TASK_STATUS_RH__TASK_ID_MASK      0x03f

#define DEINT_FRMHYP_STATUS_RH                  HW_DEINT_FRMHYP_STATUS
#define DEINT_FRMHYP_STATUS_RH_MASK                   0x0fff
#define DEINT_FRMHYP_STATUS_RH__CH2_SYMBOL_COUNT_MASK 0x0f00
#define DEINT_FRMHYP_STATUS_RH__CH1_SYMBOL_COUNT_MASK 0x00f0
#define DEINT_FRMHYP_STATUS_RH__CH0_SYMBOL_COUNT_MASK 0x000f

#define VD_DONE_STATUS_RH                       HW_VD_DONE_STATUS
#define VD_DONE_STATUS_RH_MASK                  0x0ffff

#define TD_DONE_STATUS_RH                       HW_TD_DONE_STATUS
#define TD_DONE_STATUS_RH_MASK                  0x00001

#define DEINT_RAM_STATUS_RH                     HW_DEINT_RAM_STATUS
#define DEINT_RAM_STATUS_RH_MASK                0x0ffff

#define DEINT_VD_SYM0_ST_RH                     HW_DINT_VD_SYM0_STATUS
#define DEINT_VD_SYM0_ST_RH_MASK                0x03fff

#define DEINT_VD_SYM1_ST_RH                     HW_DINT_VD_SYM1_STATUS
#define DEINT_VD_SYM1_ST_RH_MASK                0x03fff

#define DEINT_VD_SYM2_ST_RH                     HW_DINT_VD_SYM2_STATUS
#define DEINT_VD_SYM2_ST_RH_MASK                0x03fff

#define DEINT_VD_SYM3_ST_RH                     HW_DINT_VD_SYM3_STATUS
#define DEINT_VD_SYM3_ST_RH_MASK                0x03fff

#define DINT_TD_SYMS_STATUS_RH                  HW_DINT_TD_SYMS_STATUS
#define DINT_TD_SYMS_STATUS_RH_MASK             0x00fff

#define DINT_VD_RD_ADDR_RH                      HW_DINT_VD_RD_ADDR
#define DINT_VD_RD_ADDR_RH_MASK                 0x0ffff

#define DINT_VD_PKT_PARAMS_RH                   HW_DINT_VD_PKT_PARAMS
#define DINT_VD_PKT_PARAMS_RH_MASK              0x0ffff

#define DINT_TD_RD_ADDR_RH                      HW_DINT_TD_RD_ADDR
#define DINT_TD_RD_ADDR_RH_MASK                 0x0ffff

// #define DINT_TD_WR_ADDR_RH                      DINT_TD_WR_ADDR
#define DINT_TD_WR_ADDR_WH                      HW_DINT_TD_WR_ADDR // --SR
#define DINT_TD_WR_ADDR_RH_MASK                 0x0ffff

#define DINT_MISC_STATUS_RH                     HW_DINT_MISC_STATUS
#define DINT_MISC_STATUS_RH_MASK                0x0ffff

#define IS95C_TX_PATN_WH                        HW_IS95C_TX_PATN

//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW 1X TURBO DECODER (MSM5000) WRITE REGISTERS
//-----------------------------------------------------------------------------

#define TD_RESET_WB                             HW_TD_RESET
#define TD_RESET_WB_MASK                                 0x001

#define TD_TEST_MODE_CFG_MSB_WW                 HW_TD_TEST_MODE_CFG_MSB
#define TD_TEST_MODE_CFG_MSB_WW_MASK                     0x0ff
#define TD_TEST_MODE_CFG_MSB_WW__TEST_DOUT_MUX_SEL_MASK  0x0fc
#define TD_TEST_MODE_CFG_MSB_WW__SINGLE_STEP_EN_MASK     0x02
#define TD_TEST_MODE_CFG_MSB_WW__TEST_MODE_EN_MASK       0x01

#define TD_TEST_MODE_CFG_LSB_WW                 HW_TD_TEST_MODE_CFG_LSB
#define TD_TEST_MODE_CFG_LSB_WW_MASK                     0x0ff
#define TD_TEST_MODE_CFG_LSB_WW__TEST_DOUT_MUX_SEL_MASK  0x0fc
#define TD_TEST_MODE_CFG_LSB_WW__SINGLE_STEP_EN_MASK     0x02
#define TD_TEST_MODE_CFG_LSB_WW__TEST_MODE_EN_MASK       0x01

#define TD_TEST_START_WB                        HW_TD_TEST_START
#define TD_TEST_START_WB_MASK                            0x01

#define TD_TEST_SYM_LOAD_WW                     HW_TD_TEST_SYM_LOAD
#define TD_TEST_SYM_LOAD_WW_MASK                         0x0fff
#define TD_TEST_SYM_LOAD_WW__SYMBOL1_MASK                0x0cf0
#define TD_TEST_SYM_LOAD_WW__SYMBOL0_MASK                0x03f

//#define TD_TEST_SYM_LOAD_WW                     HW_TD_TEST_SYM_LOAD
//#define TD_TEST_SYM_LOAD_WW_MASK                         0x0fff
//#define TD_TEST_SYM_LOAD_WW__SYMBOL1_MASK                0x0cf0
//#define TD_TEST_SYM_LOAD_WW__SYMBOL0_MASK                0x03f


//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW 1X TURBO DECODER (MSM5000) READ REGISTERS
//-----------------------------------------------------------------------------
#define TD_ENERGY_METRIC_MSB_RW                 HW_TD_ENERGY_METRIC_MSB
#define TD_ENERGY_METRIC_MSB_RW_MASK                  0x03ffff
#define TD_ENERGY_METRIC_MSB_RW__ENERGY_METRIC_MASK   0x03ffff

#define TD_ENERGY_METRIC_LSB_RW                 HW_TD_ENERGY_METRIC_LSB
#define TD_ENERGY_METRIC_LSB_RW_MASK                  0x03ffff
#define TD_ENERGY_METRIC_LSB_RW__ENERGY_METRIC_MASK   0x03ffff

#define TD_STATUS_MSB_RW                        HW_TD_STATUS_MSB
#define TD_STATUS_MSB_RW_MASK                         0x03ffff
#define TD_STATUS_MSB_RW__MIN_LLR_MASK                0x03ff00
#define TD_STATUS_MSB_RW__ITERATIONS_RUN_MASK         0x0fc
#define TD_STATUS_MSB_RW__CRC_PASS_MASK               0x02
#define TD_STATUS_MSB_RW_DONE_MASK                    0x01

#define TD_TEST_MODE_CFG_MSB_RW                 HW_TD_TEST_MODE_CFG_MSB
#define TD_TEST_MODE_CFG_MSB_RW_MASK                     0x0ff
#define TD_TEST_MODE_CFG_MSB_RW__TEST_DOUT_MUX_SEL_MASK  0x0fc
#define TD_TEST_MODE_CFG_MSB_RW__SINGLE_STEP_EN_MASK     0x02
#define TD_TEST_MODE_CFG_MSB_RW__TEST_MODE_EN_MASK       0x01

#define TD_TEST_MODE_CFG_LSB_RW                 HW_TD_TEST_MODE_CFG_LSB
#define TD_TEST_MODE_CFG_LSB_RW_MASK                     0x0ff
#define TD_TEST_MODE_CFG_LSB_RW__TEST_DOUT_MUX_SEL_MASK  0x0fc
#define TD_TEST_MODE_CFG_LSB_RW__SINGLE_STEP_EN_MASK     0x02
#define TD_TEST_MODE_CFG_LSB_RW__TEST_MODE_EN_MASK       0x01

#define TD_TEST_STATUS_RH                       HW_TD_TEST_STATUS
#define TD_TEST_STATUS_RH_MASK                              0x01fff
#define TD_TEST_STATUS_RH__APP_WRITE_VALID_MASK             0x01000
#define TD_TEST_STATUS_RH__APP_READ_VALID_MASK              0x0800
#define TD_TEST_STATUS_RH__APP_SYMBOL_VALID_MASK            0x0400
#define TD_TEST_STATUS_RH__LOG_LIKELIHOOD_RATIO_VALID_MASK  0x0200
#define TD_TEST_STATUS_RH__RSMC1_STATE_METRICS_VALID_MASK   0x0100
#define TD_TEST_STATUS_RH__RSMC1_BRANCH_METRICS_VALID_MASK  0x080
#define TD_TEST_STATUS_RH__RSMC1_SYMBOLS_VALID_MASK         0x040
#define TD_TEST_STATUS_RH__RSMC0_STATE_METRICS_VALID_MASK   0x020
#define TD_TEST_STATUS_RH__RSMC0_BRANCH_METRICS_VALID_MASK  0x010
#define TD_TEST_STATUS_RH__RSMC0_SYMBOLS_VALID_MASK         0x08
#define TD_TEST_STATUS_RH__FSMC_STATE_METRICS_VALID_MASK    0x04
#define TD_TEST_STATUS_RH__FSMC_BRANCH_METRICS_VALID_MASK   0x02
#define TD_TEST_STATUS_RH__FSMC_SYMBOLS_VALID_MASK          0x01

#define TD_TEST_DOUT_MSB_RW                     HW_TD_TEST_DOUT_MSB
#define TD_TEST_DOUT_MSB_RW_MASK                   0x0ffffffff
#define TD_TEST_DOUT_MSB_RW__TEST_MUX_DOUT_MASK    0x0ffffffff

#define TD_TEST_DOUT_LSB_RW                     HW_TD_TEST_DOUT_LSB
#define TD_TEST_DOUT_LSB_RW_MASK                   0x0ffffffff
#define TD_TEST_DOUT_LSB_RW__TEST_MUX_DOUT_MASK    0x0ffffffff


//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW DECODER OUTPUT BUFFER (MSM5000) WRITE REGISTERS
//-----------------------------------------------------------------------------
#define DEC_OB_ADDRESS_WH                       HW_DEC_OB_ADDRESS
#define DEC_OB_ADDRESS_WH_MASK                  0x0ffff
//-----------------------------------------------------------------------------
//  MSM MEMORY SPACE CHIP_BASE to (CHIP_BASE + 0xAB0)
//
//  SECTION     NEW DECODER OUTPUT BUFFER (MSM5000) READ REGISTERS
//-----------------------------------------------------------------------------
#define DEC_OB_ADDRESS_RH                       HW_DEC_OB_ADDRESS
#define DEC_OB_ADDRESS_RH_MASK                  0x0ffff

#define DEC_OB_DATA_RH                          HW_DEC_OB_DATA
#define DEC_OB_DATA_RH_MASK                     0x0ffff

#endif /* MSM50REG_H */
