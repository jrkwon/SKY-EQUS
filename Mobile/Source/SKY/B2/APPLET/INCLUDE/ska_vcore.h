#ifndef VOC_CORE_H
#define VOC_CORE_H
/*===========================================================================

                     M S M 2   V O C O D E R   C O R E

                          H E A D E R    F I L E

DESCRIPTION
  This file contains declarations and definitions for DMSS interaction
  with the MSM 2 Vocoder Core.

REFERENCES
  MSM 2xxx Technical Specifications
  MSM 2310 Changes & Enhancements Document
  MSM 2.3 Unit Test Descriptions (Doc Ref: 80-13493-9):  This document 
    explains in detail the process by which optimal RX/TX vocoder frame 
    offsets must be established for each vocoder (8k, 13k, EVRC).  The results
    of this calibration are the values defined in this header file under the
    registers VC_ENC_FRAME_ADVANCE and VC_DEC_FRAME_ADVANCE.  

Copyright (c) 1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998,1999 by QUALCOMM, Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/MSM3000/ASW/VCS/voc_core.h_v   1.1   25 Mar 1999 13:52:04   ajoshi  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/25/99   aaj     Added FEATURE_MINIMIZE_ISR_LOADING. This uses one trasaction
                   window for RS1, RS2 and EVRC. 
12/04/98    sm     Updated vocoder tx offsets for time optimized MSM3000 code
11/09/98    st     Added register value for MSM internal flip-flop selection,
                   as requested by dhh.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
10/16/98    st     Added HFK/PCM pad byte bitmask definitions.
09/07/98    ss     Changed global variables to be in 'near' in attempt to save
                   code space.
08/18/98    ss     Added Register Definitions to support MSM2310, dynamic 
                   download, EVRC and new QDSP1+ firmware features.  New 
                   Definitions include:
                   1. VOC_RESET: Added values VC_RESET_BOOT_DSP and 
                      VC_RESET_DOWNLOAD_QDSP1P, which respectively cause the
                      vocoder to free-run (boot) or to be downloaded via the
                      FAST LOAD registers defined in the VOCDOWN module.
                   2. Tx/Rx Offsets:  For qdsp1+ vocoders, Tx & Rx offsets are
                      calibrated using the 19.68MHz TCXO vocoder clock (see 
                      references above).  
                   3. Noise Suppression Registers:  For qdsp1+ vocoders, the 
                      reg. addresses previously mapped to VOX functions are 
                      now used to control Noise Suppression Feature.  See the
                      new register definition VC_NS_CONTROL and its fields.
                   4. Automatic Gain Control:  A qdsp1+ vocoder future-feature.  
                      These registers are simply set to default value of zero.
                      See VC_FWD_AGC_MSB, VC_FWD_AGC_LSB, VC_REV_AGC_MSB and 
                      VC_REV_AGC_LSB.   
                   5. Voice Recognition Registers:  Future qdsp1+ feature.  
                      As per AGC above, the VR registers are simply set to 
                      default value of zero.  See VC_VR_CONTROL_MSB and 
                      VC_VR_CONTROL_LSB.
01/12/98    ro     Added VC_ENC_PCM_WR_MSB and VC_ENC_PCM_WR_LSB.
                   Update copyright.
10/23/97    ro     Change DOUT_VAL for T_Q, fixes CR 4189.
10/01/97   udp     Added MSM2.3 defines
05/23/97    ro     Change DOUT_VAL for HFK_PLUS.
                   Update copyright.
05/08/97    ro     Added #define VC_ENC_PCM_RD_LSB.
01/24/97    ro     Added control of VC_AUX_SYNC_VAL to VC_AUX_DEFAULT.
12/31/96   rah     Added Slot 2 value to RX and TX PCM control and re-defined
                                                                         the default value conditioned on FEATURE_ECFM for SSS Echo
                   Cancellor Needs.
12/23/96   rah     Added a DATA SERVICES value to FM deviation (10.5khz).
10/29/96   rah     Changed 13kbps RX OFFSET to reflect subtraction DC blocking
                   instead of High Pass Filter DC blocking.
10/28/96   rah     Added Diagnostic data enable/disable flags and mask to the
                   MODE_CONTROL bit definitions.
10/17/96   rah     As added screech protection for B0 13k phones, changed the
                   sense of the DC_BLOCKING bit of the TX_PCM register to the
                   correct orientation. Changed the sense of the RATE_LIMIT_TYPE
                   bit in the ENC_RATE_LIMIT register.
09/25/96   rah     Changed Rx and Tx frame offset defines for B0:
                     13k: Tx = 52, Rx = 50 (HS) 21 (CK).
                      8k: Tx = 92, Rx =  2 (HS/CK).
                   MODE_CONTROL default: 8k 1/4 rate disable.
                   VOX_CONTROL defaults:
                      CK: ESEC ON, OUT ON, NOISE ON, VOX OFF.
                      CK: ESEC ON, NO OUT, NO NOISE, VOX OFF.
08/14/96   rah     Changed B0 vox disable logical to keep ESEC enabled for
                   consistant MSM vocoder loading.
07/17/96   rah     Added multiple Rx frame offset values (by MSM version).
                   Changed default VOX enable and disable flags.
05/02/96   rah     Added new bitmap for MSM 2.2 B0 VOX/ESEC control. Fixed
                   the bit definitions for MSM 2.2 B0 DC BLOCKING. Added new
                   RX FRAME OFFSETS for 13kbps 8kbps w/13.5 MHz clock and
                   8kbps w/9.8 MHz clock, for the B0 workaround.
04/16/96   rah     Changed sense of default AUX_DOUT_VAL to HI.
04/02/96   rah     Added composite DEFAULT bit maps for RX_PCM, TX_PCM and
                   AUX ports.
03/27/96   rah     Added MSM 2.2 specific defines.
03/01/96   rah     Changed RX & TX FRAME_OFFSET's to accomodate vocoder
                   vocoder transactions from RX and TX traffic channel ISR's.
02/26/96   rah     Changed VC_TX_DEV_LIMIT_NOM_V to 0x67 and
                   VC_SAT_THRESHOLD_NOM_V to 0x19 (per P.Black).
01/04/96   rah     Added VC_DEC_PCM_WR_xSB register address and a
                   corresponding mute value. changed TX frame offset to 112
                   for 13kbps synch w/o a MICRO Frame Ref.
12/21/95   rah     Reset the Tx offset to 7.5 ms.
12/15/95   rah     Added VC_POKE_DATA_xSB for VOX threshold workaround.
11/30/95   rah     Set VC_TX_DEV_LIMIT_NOM_V back to 0x64.
11/30/95   rah     Reset VC_TX_DEV_LIMIT_NOM_V back to 0x69.
11/30/95   rah     Changed sense of VC_MODE_QTR_RATE_(ENA/DIS)_V. Changed
                   VC_TX_DEV_LIMIT_NOM_V from 0x69 to 0x64.
11/01/95   rah     Added MSM2.2 defines. Fixed VC_TEST2_VOC_BYPASS value
                   and mask.
09/21/95   rah     Changed Frame offsets to minimize R/T delay. Changed
                   VC_OUTM to regain some memory.
08/03/95   rah     Changed VC_RX_FRAME_OFFSET to 16. Changed Tx frame offset
                   to 68.
08/21/95   rah     Added PCM loopback logicals. Changed FM Deviation.
08/17/95   rah     Added BIST bit definitiions.
08/03/95   jah     Changed VC_ONES_DETECT_M to bit 7, for car-kit detect
08/03/95   rah     Changed VC_RX_FRAME_OFFSET to 120.
07/25/95   fkm/jah Correct VC_MAX_INX to account for 0th entry
07/21/95   jah     Added decoder gains for silent/unity/-6dB
07/20/95   rah     Changed VC_TX_FRAME_OFFSET to 60.
07/11/95   jah     Corrected RX/TX mute, for backwards documentation.
07/06/95   jah     Corrected definition of VOX control bits (includes muting)
05/17/95   jah     Updated Tx & Rx frame offsets to 1ms and 2ms.
03/23/95   jah     Updated from Newer documents
07/20/94   jca     Created

===========================================================================*/

#include "comdef.h"
#include "processor.h"
#include "target.h"             /* Definition for target specific defs */

#if defined (T_MSM3)
#include "msm.h"                /* Defs for registers and MSM_* macros */
#endif /* T_MSM3 */

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

MACRO VC_IO

DESCRIPTION
  Calculate the I/O address of a vocoder register given the port index.

PARAMETERS
  inx   Index of the port to calculate the address of.

DEPENDENCIES
  None

RETURN VALUE
  The I/O address of the vocoder register

SIDE EFFECTS
  None

===========================================================================*/


/*===========================================================================

MACRO VC_INX

DESCRIPTION
  Calculate the index of a register from the address.

PARAMETERS
  io    I/O address of the register to calculate the offset of.

DEPENDENCIES
  None

RETURN VALUE
  The offset of the vocoder register relative to the Vocoder Core's
  base address.

SIDE EFFECTS
  None

===========================================================================*/
#if defined (T_MSM3)
#define VC_INX( io )             MSM_INX( io )
#endif /* T_MSM3 */

/*---------------------------------------------------------------------------
                      Vocoder Core Write Registers
---------------------------------------------------------------------------*/

/* Reset:  A write to this register causes the Vocoder to be reset.
** When reset the Vocoder will first acquire frame timing and will then
** function according to the configuration selected in VC_MODE_CONTROL and
** VC_FM_TEST_CONTROL.  Frame timing can be acquired by asserting the
** VOC_FRAME_REF pin following a write to the ENC_FRAME_ADV register.
*/

#define VC_RESET_FIRMWARE_START_M      0x01   /* FAST-LOAD RAM INTERFACE */
                                              /* SELECTION MASK. */
#define VC_RESET_FIRMWARE_DOWNLOAD     0x00   /* Enable Fast Load i/f, */
                                              /* & freeze DSP execution. */
#define VC_RESET_FIRMWARE_BOOT         0x01   /* Disable Fast Load i/f, */
                                              /* & Boot the DSP. */ 

#define VC_RESET_ROM_BANK_SEL_M        0x02   /* ROM BANK SELECTION MASK */
                                              /* (valid for MSM3x only). */
#define VC_RESET_ROM_BANK_SEL_8K13K    0x00   /* ROM bank 0 (8k/13k) */
#define VC_RESET_ROM_BANK_SEL_EVRC     0x02   /* ROM bank 1 (EVRC) */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Fast Load RAM Interface registers. */
#if defined (T_MSM3)
/* 16-bit download interface. */
#define VC_FAST_LOAD_RAM_ADDR          VOC_FL_RAM_ADDR_WH
#define VC_FAST_LOAD_RAM_DATA          VOC_FL_RAM_DATA_WH
#endif /* T_MSM3 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Encoder and Decoder Frame Advance:  Position the encoder and decoder
** frame boundaries relative to the vocoder frame reference.  See MSM Unit 
** Test Document in References above, which explains the concept of vocoder
** transaction windows and the need to align each TX and RX transaction in the
** middle of the available window.  
**
** Note: The system sampling frequency Fs is 8kHz (Ts = 0.125ms / sample).  
** Each vocoder packet, regardless of the air interface bit rate, contains
** data representing 160 PCM speech samples (160 * 0.125 = 20 milliseconds).  
** The offset values defined below are measured in units of PCM samples.  
** The comment for each value gives the corresponding time offset in ms.
** For an offset of N PCM samples, the actual time offset is simply N * Ts.
*/

#ifdef FEATURE_MINIMIZE_ISR_LOADING

/* These are the new transaction window settings for MDR ISR optimizations.
** We use only one set of frame advances which can include all the rate sets
** such as RS1, RS2 or EVRC.  This definitions are used only in vocm2.c hence 
** we did not include those in voc_core.h which has larger dependency list 
*/
#define VC_RX_OFFSET_NEW_TIMELINE_MDR      0x0 /* 24=3ms from end*/
#define VC_TX_OFFSET_NEW_TIMELINE_MDR      0x50 /* 80=10 ms from end*/

#endif /* FEATURE_MINIMIZE_ISR_LOADING */

#define VC_RX_8K_135M_FRM_OFFSET       51        /*  6.375 ms */
#define VC_RX_8K_98M_FRM_OFFSET         2        /*  0.250 ms */
#define VC_RX_8K_M2P_FRM_OFFSET        42        /*  5.250 ms */
#define VC_RX_8K_MA2_FRM_OFFSET        55        /*  6.875 ms */
#define VC_RX_B0_08K_HS_FRM_OFFSET      2        /*  0.250 ms */
#define VC_RX_B0_08K_CK_FRM_OFFSET      2        /*  0.250 ms */
#define VC_RX_B0_13K_HS_FRM_OFFSET     52        /*  6.500 ms */
#define VC_RX_B0_13K_CK_FRM_OFFSET     21        /*  5.125 ms */
#define VC_RX_MSM23_13K_HS_FRM_OFFSET  (52+13)   /*  8.125 ms */
#define VC_RX_TCXO_IS96A_OFFSET        82        /* 10.250 ms */
#define VC_RX_TCXO_13K_OFFSET          65        /*  8.125 ms */
#define VC_RX_TCXO_EVRC_OFFSET         76        /*  9.500 ms */

#define VC_TX_FRAME_OFFSET             52        /*  6.500 ms */
#define VC_TX_08K_FRAME_OFFSET         92        /* 11.500 ms */
#define VC_TX_13K_FRAME_OFFSET         52        /*  6.500 ms */

/*
** MSM3000 13k frame offset value (non-optimized ARM)
*/
#define VC_TX_MSM3_13K_FRAME_OFFSET    82        /* 10.250 ms */

#define VC_TX_TCXO_IS96A_OFFSET        40        /*  5.000 ms */
#define VC_TX_TCXO_13K_OFFSET          24        /*  3.000 ms */
#define VC_TX_TCXO_EVRC_OFFSET         72        /*  9.000 ms */

/* MSM3000 Surf now runs time optimized code, affecting offsets */
#define VC_TX_TCXO_M30_IS96A_OFFSET  0x62
#define VC_TX_TCXO_M30_13K_OFFSET    0x66
#define VC_TX_TCXO_M30_EVRC_OFFSET   0x82

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* TX PCM Control:  PCM Codec configuration parameters for the TX audio
** input.  Must be written before a software reset.
*/

#define VC_TX_PCM_DC_BLOCK_M     0x80    /* DC Bias removal from input PCM */
#define VC_TX_PCM_DC_BLOCK_ENA_V 0x00    /* Enable bias removal            */
#define VC_TX_PCM_DC_BLOCK_DIS_V 0x80    /* Disable bias removal           */

#define VC_TX_PCM_FORMAT_M       0x60    /* PCM Format select              */
#define VC_TX_PCM_MU_LAW_V       0x00    /* mu-law decompanding            */
#define VC_TX_PCM_A_LAW_V        0x20    /* a-law decompanding             */
#define VC_TX_PCM_LINEAR_V       0x40    /* Linear                         */

#define VC_TX_PCM_SLOT_M         0x1F    /* PCM Slot value                 */
#define VC_TX_PCM_SLOT_0_V       0x00    /* Slot #0                        */
#define VC_TX_PCM_SLOT_2_V       0x02    /* Slot #2                        */

/* Default Encoder PCM DC Block / Slot mask and value */
#define VC_TX_PCM_DEFAULT_M (VC_TX_PCM_DC_BLOCK_M      | VC_TX_PCM_SLOT_M)

#ifdef FEATURE_ECFM
#error code not present
#else
#define VC_TX_PCM_DEFAULT_V (VC_TX_PCM_DC_BLOCK_ENA_V  | VC_TX_PCM_SLOT_0_V)
#endif /* FEATURE_ECFM */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* RX PCM Control:  PCM Codec configuration parameters for the RX audio
** input.  Must be written before a software reset.
*/

#define VC_RX_PCM_DOUT_EN_M      0x80    /* PCM_DOUT enable                */
#define VC_RX_PCM_DOUT_ENA_V     0x80    /* Enable                         */
#define VC_RX_PCM_DOUT_DIS_V     0x00    /* Disable                        */

/* Note:  PAD_COMPANDED (from the PCM_PAD_CONTROL register) controls MODE1 & 2
**
**       if PAD_COMPANDED is 0, both MODE1 and MODE2 are Linear
**       if PAD_COMPANDED is 1, MODE1 is mu-law with padding
**                              MODE2 is a-law with padding
*/
#define VC_RX_PCM_FORMAT_M       0x60    /* PCM Format select              */
#define VC_RX_PCM_MU_LAW_V       0x00    /* mu-law companding w/out pad    */
#define VC_RX_PCM_A_LAW_V        0x20    /* a-law companding w/out pad     */
#define VC_RX_PCM_MODE1_V        0x40    /* See Note above                 */
#define VC_RX_PCM_MODE2_V        0x60    /* See Note above                 */

#define VC_RX_PCM_SLOT_M         0x1F    /* PCM Slot value                 */
#define VC_RX_PCM_SLOT_0_V       0x00    /* Slot #0                        */
#define VC_RX_PCM_SLOT_2_V       0x02    /* Slot #2                        */

/* Default Decoder PCM Data Output Enable  / Slot mask and value */
#define VC_RX_PCM_DEFAULT_M (VC_RX_PCM_DOUT_EN_M       | VC_RX_PCM_SLOT_M)

#ifdef FEATURE_ECFM
#error code not present
#else
#define VC_RX_PCM_DEFAULT_V (VC_RX_PCM_DOUT_ENA_V      | VC_RX_PCM_SLOT_0_V)
#endif /* FEATURE_ECFM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* RX PCM Write Data Register:  Initialize CODEC data through these registers.
*/

#define VC_DEC_PCM_MUTE_VAL        0x00  /* Car Kit vocoder/CODEC mute MSB*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Test Control #1:
*/

#define VC_TEST1_PCM_CLK_SENSE_M 0x40    /* PCM Clock edge to sample on    */
#define VC_TEST1_PCM_CLK_RISE_V  0x40    /* Sample on rising edge          */
#define VC_TEST1_PCM_CLK_FALL_V  0x00    /* Sample on falling edge         */

#define VC_TEST1_BIST_MODE_M     0x20    /* Built In Self Test Mode */
#define VC_TEST1_BIST_MODE_ENA_V 0x20    /* Enable BIST */
#define VC_TEST1_BIST_MODE_DIS_V 0x00    /* Disable BIST */

#define VC_TEST1_PCM_ENC_W_EN_M  0x10    /* PCM sample override            */
#define VC_TEST1_PCM_ENC_W_ENA_V 0x10    /* Get samples from ENC_PCM_WR    */
#define VC_TEST1_PCM_ENC_W_DIS_V 0x00    /* Get samples from PCM interface */

#define VC_TEST1_PCM_DEC_W_EN_M  0x08    /* PCM sample override            */
#define VC_TEST1_PCM_DEC_W_ENA_V 0x08    /* Get samples from DEC_PCM_WR    */
#define VC_TEST1_PCM_DEC_W_DIS_V 0x00    /* Get samples from PCM interface */

#define VC_TEST1_MICRO_FRM_REF_M 0x04    /* Frame Reference input          */
#define VC_TEST1_FRM_REF_HIGH_V  0x04    /* OR a 1 with VOC_FR_REF         */
#define VC_TEST1_FRM_REF_NORM_V  0x00    /* VOC_FR_REF operates normally   */

#define VC_TEST1_PCM_LOOPBK_M    0x02    /* PCM Clock edge to sample on    */
#define VC_TEST1_LOOPBK_ENA_V    0x02    /* Sample on rising edge          */
#define VC_TEST1_LOOPBK_DIS_V    0x00    /* Sample on falling edge         */

/* MSM2.3 MICRO_CLK_EN has been deleted, use clk_regime modules() */
#define VC_TEST1_MICRO_CLK_EN_M  0x01    /* Internal clock control         */
#define VC_TEST1_MICRO_WRITE_V   0x01    /* MICRO_CLK drives the clock     */
#define VC_TEST1_CHIPX8_V        0x00    /* Chipx8 drives the clock        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Test Control #2:
*/

#define VC_TEST2_VOC_BYPASS_M    0x02    /* Vocoder By-pass control        */
#define VC_TEST2_VOC_BYPASS_V    0x02    /* By-pass the internal vocoder   */
#define VC_TEST2_VOC_CORE_V      0x00    /* Use the internal vocoder       */

/* MSM2.2 ONLY */
#define VC_OSC_EN_M              0x08    /* XTAL IN/OUT control            */
#define VC_OSC_MICRO_CTL_V       0x00    /* Allow microprocessor control   */
#define VC_OSC_ALWAYS_ENA_V      0x08    /* Always enable                  */

#define VC_PLL_CLK_SEL_M         0x80    /* Clock Select for PLL           */
#define VC_PLL_CLK_CHIPX8_V      0x00    /* Use Chipx8                     */
#define VC_PLL_CLK_TCXO_4_V      0x80    /* Use TCXO/4                     */

#define VC_CLK_SEL_M             0x70    /* Clock Select for Vocoder       */
#define VC_CLK_CHIPX8_V          0x00    /* Use Chipx8                     */
#define VC_CLK_PLL_V             0x10    /* Use Internal PLL               */
#define VC_CLK_XTAL_IN_DIV2_V    0x20    /* Use XTAL IN/2                  */
#define VC_CLK_GPIO_28_V         0x40    /* Use GPIO 28 INPUT              */
/* END MSM2.2 ONLY */

/* MSM2.3 ONLY */
#define VC_CLK_XTAL_IN_DIV3_V    0x30    /* Use XTAL IN/3  MSM2.3          */
#define VC_CLK_GPIO_23_DIV2_V    0x60    /* Use GPIO 28 INPUT/2            */
/* END MSM2.3 ONLY */

/* MSM2310 ONLY */
#define VC_CLK_XTAL_IN_V         0x50    /* Use XTAL IN (27MHz).           */
/* Note: VC_CLK_PLL_V defined above has no meaning for MSM2300.  The new 
 * meaning for the field-value 0x10 is expressed below. 
 */
#define VC_CLK_TCX0_V            0x10    /* Use TXCO (19.68MHz).           */
/* END MSM2310 ONLY */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Mode Control:  control the operating mode
*/

#define VC_MODE_QTR_RATE_M       0x40    /* Quarter rate frame generation  */
#define VC_MODE_QTR_RATE_ENA_V   0x00    /* Generate quarter rate frames   */
#define VC_MODE_QTR_RATE_DIS_V   0x40    /* No quarter rate frames         */

#define VC_MODE_IS96_VER_M       0x20    /* IS-96 version                  */
#define VC_MODE_IS96A_V          0x20    /* IS-96a                         */
#define VC_MODE_IS96_V           0x00    /* IS-96                          */

#define VC_MODE_EXTPC_LPBK_M     0x10    /* PCM loopback               */
#define VC_MODE_EXTPC_LPBK_ENA_V 0x10    /* Enable                     */
#define VC_MODE_EXTPC_LPBK_DIS_V 0x00    /* Disable                    */

#define VC_MODE_PKT_LOOPBK_M     0x08    /* Packet loopback                */
#define VC_MODE_PKT_LOOPBK_ENA_V 0x08    /* Enable                         */
#define VC_MODE_PKT_LOOPBK_DIS_V 0x00    /* Disable                        */

#define VC_MODE_DIAG_EN_M        0x02    /* Diagnostic data                */
#define VC_MODE_DIAG_EN_ENA_V    0x02    /* Enable                         */
#define VC_MODE_DIAG_EN_DIS_V    0x00    /* Disable                        */

/* MSM2.2 ONLY */
#define VC_SELECT_VOC_M          0x80    /* Select Vocoder     */
#define VC_13K_V                 0x80    /* Use 13K vocoder    */
#define VC_8K_V                  0x00    /* Use 8K vocoder     */

#define VC_DTMF_DETECT_M         0x01    /* Enable/Disable DTMF detection */
#define VC_DTMF_DETECT_ENA_V     0x01    /* Enable Note: Disable QCELP encoding */
#define VC_DTMF_DETECT_DIS_V     0x00    /* Disable */

/* NOTE: This bit definition overlaps QTR_RATE_M */
#define VC_MODE_B0_DC_BLOCK_M    0x40    /* MSM 2.2 B0 DC blocking mode*/
#define VC_MODE_SUB_DC_BLOCK_V   0x40    /* DC subtracting */
#define VC_MODE_HP_DC_BLOCK_V    0x00    /* High pass filter */

/* NOTE: BIST bits overlap control bits */
#define VC_MODE_ALU_RAM_ROM_V    0x01    /* BIST ALU, RAM, ROM test */
#define VC_MODE_MP_PITCH_1_V     0x02    /* BIST Pitch #1 test */
#define VC_MODE_MP_PITCH_2_V     0x04    /* BIST Pitch #2 test */
#define VC_MODE_MP_PITCH_3_V     0x08    /* BIST Pitch #3 test */
#define VC_MODE_MP_CODEBK_1_V    0x10    /* BIST Codebook #1 test */
#define VC_MODE_MP_CODEBK_2_V    0x20    /* BIST Codebook #2 test */
#define VC_MODE_MP_FILTER_MODE_V 0x40    /* BIST Filter (MAC) mode test */
#define VC_MODE_MP_L_S_P_V       0x80    /* BIST LOOP/STACK PORT test */

/* END MSM2.2 ONLY */

#define VC_MODE_CONTROL_M     ( VC_MODE_QTR_RATE_M    | VC_MODE_IS96_VER_M \
                              | VC_MODE_PKT_LOOPBK_M  | VC_SELECT_VOC_M )
#define VC_MODE_CONTROL_V     ( VC_MODE_QTR_RATE_DIS_V| VC_MODE_IS96A_V \
                              | VC_MODE_PKT_LOOPBK_DIS_V | VC_8K_V )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* VOX Control: these bits are read by the vocoder during a 1 ms period
** beginning at the encoder frame boundary.
*/

/* MSM 2.2 B0 ONLY */
#define VC_ESEC_DIAG_M           0x80    /* ESEC Diagnostics              */
#define VC_ESEC_DIAG_ENA_V       0x80    /* Enable                        */
#define VC_ESEC_DIAG_DIS_V       0x00    /* Disable                       */

#define VC_ESEC_GEAR_M           0x40    /* ESEC Auto Adaptation Step Size*/
#define VC_ESEC_GEAR_ENA_V       0x00    /* Enable                        */
#define VC_ESEC_GEAR_DIS_V       0x40    /* Disable                       */

#define VC_ESEC_NOISE_M          0x20    /* ESEC Comfort Noise            */
#define VC_ESEC_NOISE_ENA_V      0x00    /* Enable                        */
#define VC_ESEC_NOISE_DIS_V      0x20    /* Disable                       */

#define VC_ESEC_NO_OUT_M         0x10    /* ESEC Synthisized Echo Subtract*/
#define VC_ESEC_NO_OUT_ENA_V     0x00    /* Enable                        */
#define VC_ESEC_NO_OUT_DIS_V     0x10    /* Disable                       */

#define VC_ESEC_ENABLE_M         0x08    /* ESEC Enable                   */
#define VC_ESEC_ENABLE_ENA_V     0x08    /* Enable                        */
#define VC_ESEC_ENABLE_DIS_V     0x00    /* Disable                       */

/* END MSM 2.2 B0 ONLY */

#define VC_VOX_TX_MUTE_M         0x04    /* Tx Mute (called RX_MUTE)       */
#define VC_VOX_TX_MUTE_ENA_V     0x04    /* Mute                           */
#define VC_VOX_TX_MUTE_DIS_V     0x00    /* Un-mute                        */

#define VC_VOX_RX_MUTE_M         0x02    /* Rx Mute (called TX_MUTE)       */
#define VC_VOX_RX_MUTE_ENA_V     0x02    /* Mute                           */
#define VC_VOX_RX_MUTE_DIS_V     0x00    /* Un-mute                        */

#define VC_VOX_CTL_M             0x01    /* VOX control                    */
#define VC_VOX_ENA_V             0x01    /* Enable                         */
#define VC_VOX_DIS_V             0x00    /* Disable                        */

#define VC_VOX_B0_VOX_M     ( VC_ESEC_ENABLE_M      | VC_ESEC_NO_OUT_M \
                            | VC_ESEC_NOISE_M       | VC_VOX_CTL_M )

#define VC_VOX_B0_CK_V      ( VC_ESEC_ENABLE_ENA_V  | VC_ESEC_NO_OUT_DIS_V \
                            | VC_ESEC_NOISE_DIS_V   | VC_VOX_DIS_V )

#define VC_VOX_B0_HS_V      ( VC_ESEC_ENABLE_ENA_V  | VC_ESEC_NO_OUT_ENA_V \
                            | VC_ESEC_NOISE_ENA_V   | VC_VOX_DIS_V )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Peek/Poke Control:  Request that a 16 byte peek or a 2 byte poke.
*/

#define VC_PEEK_POKE_EN_M        0x40    /* Peek/Poke enable               */
#define VC_PEEK_POKE_ENA_V       0x40    /* Enable                         */
#define VC_PEEK_POKE_DIS_V       0x00    /* Disable                        */

#define VC_PEEK_POKE_SEL_M       0x20    /* Select peek OR poke            */
#define VC_PEEK_V                0x20    /* Peek                           */
#define VC_POKE_V                0x00    /* Poke                           */

#define VC_POKE_RAM_M            0x18    /* RAM area select for poke       */
#define VC_POKE_RAM_A_V          0x00    /* RAM A                          */
#define VC_POKE_RAM_B_V          0x08    /* RAM B                          */
#define VC_POKE_RAM_C_LSW_V      0x10    /* RAM C, LSW                     */
#define VC_POKE_RAM_C_MSW_V      0x18    /* RAM C, MSW                     */

#define VC_PEEK_POKE_ADDR_10_8_M 0x07    /* Peek/Poke address bits 10:8    */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Encoder Rate Control:  this register indicates the minimum and maximum
** rates for the next encoded frame.  These bits must be written within 2ms
** of the encoder interrupt, or 19ms before the encoder frame boundary.
*/

#define VC_INIT_BG_NOISE_M       0x80    /* (re-)Init the background noise */
#define VC_INIT_BG_NOISE_V       0x80    /* (re-)Init                      */
#define VC_INIT_BG_DONE_V        0x00    /* Done re-initializing           */

#define VC_RATE_LIMIT_TYPE_M     0x40    /* Rate limit type                */
#define VC_RATE_LIMIT_1_S_V      0x40    /* 1/S                            */
#define VC_RATE_LIMIT_S_V        0x00    /* S                              */

#define VC_RATE_LIMIT_EN_M       0x20    /* Rate limit enable              */
#define VC_RATE_LIMIT_ENA_V      0x20    /* Enable                         */
#define VC_RATE_LIMIT_DIS_V      0x00    /* Disable                        */

#define VC_BLANK_FRAME_M         0x10    /* Blank a frame                  */
#define VC_BLANK_V               0x10    /* Blank                          */
#define VC_BLANK_DONE_V          0x00    /* Done blanking                  */

#define VC_MAX_RATE_M            0x0C    /* Max encode rate                */
#define VC_MAX_FULL_V            0x00    /* Full                           */
#define VC_MAX_HALF_V            0x04    /* Half                           */
#define VC_MAX_QTR_V             0x08    /* Quarter                        */
#define VC_MAX_EIGHTH_V          0x0C    /* Eighth                         */

#define VC_MIN_RATE_M            0x03    /* Min encode rate                */
#define VC_MIN_FULL_V            0x00    /* Full                           */
#define VC_MIN_HALF_V            0x01    /* Half                           */
#define VC_MIN_QTR_V             0x02    /* Quarter                        */
#define VC_MIN_EIGHTH_V          0x03    /* Eighth                         */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM Audio Configuration:  Configures the FM audio processing paths.
** Takes effect within 3ms.
*/

#define VC_FM_VOX_EN_M           0x80    /* FM VOX Enable                  */
#define VC_FM_VOX_ENA_V          0x80    /* Enable FM VOX                  */
#define VC_FM_VOX_DIS_V          0x00    /* Disable FM VOX                 */

#define VC_AUTO_MUTE_WBD_M       0x20    /* RX Auto-Mute WBD Enable        */
#define VC_AUTO_MUTE_WBD_ENA_V   0x20    /* Enable RX auto-mute            */
#define VC_AUTO_MUTE_WBD_DIS_V   0x00    /* Disable RX auto-mute           */

#define VC_TX_MUTE_M             0x08    /* TX Audio Mute                  */
#define VC_TX_MUTE_ENA_V         0x08    /* Mute TX audio output           */
#define VC_TX_MUTE_DIS_V         0x00    /* Enable TX audio output         */

#define VC_RX_DTMF_EN_M          0x04    /* RX DTMF Enable                 */
#define VC_RX_DTMF_ENA_V         0x04    /* Enable DTMFs to RX path        */
#define VC_RX_DTMF_DIS_V         0x00    /* Disable DTMFs to Rx path       */

#define VC_TX_DTMF_EN_M          0x02    /* TX DTMF Enable                 */
#define VC_TX_DTMF_ENA_V         0x02    /* Enable DTMFs to Tx path        */
#define VC_TX_DTMF_DIS_V         0x00    /* Disable DTMFs to Tx path       */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Automatic Gain Control (QDSP1+ only)
*/    

#define VC_AGC_NOT_DEFINED       0x00

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM Test Control:  Configures the FM audio path for test.  Must be
** set to 0x00 for operation.  Must be written before a software reset.
*/

#define VC_PACKET_VALID_M        0x80    /* FM Packet Valid                */
#define VC_PACKET_LOAD_V         0x80    /* Load VOX and Comp. values      */
#define VC_PACKET_RESET_V        0x00    /* Reset 'load' latch             */

#define VC_COMPRESSOR_M          0x20    /* Audio Compressor Disable       */
#define VC_COMPRESSOR_BYPASS_V   0x20    /* Bypass compressor              */
#define VC_COMPRESSOR_ENA_V      0x00    /* Enable compressor              */

#define VC_FM_PREEMPH_M          0x10    /* FM Pre-emphasis                */
#define VC_FM_PREEMPH_ENA_V      0x00    /* Enable FM pre-emphasis         */
#define VC_FM_PREEMPH_DIS_V      0x10    /* Disable FM pre-emphasis        */

#define VC_FM_LOOPBACK_M         0x08    /* FM Loopback                    */
#define VC_FM_LOOPBACK_ENA_V     0x08    /* Enable FM loopback             */
#define VC_FM_LOOPBACK_DIS_V     0x00    /* Disable FM loopback            */

#define VC_EXPANDER_M            0x02    /* Audio Expander Disable         */
#define VC_EXPANDER_BYPASS_V     0x02    /* Bypass expander                */
#define VC_EXPANDER_ENA_V        0x00    /* Enable expander                */

#define VC_SAT_TRANS_M           0x01    /* SAT Transpond Disable          */
#define VC_SAT_TRANS_DIS_V       0x01    /* Disable SAT transpond          */
#define VC_SAT_TRANS_ENA_V       0x00    /* Enable SAT transpond           */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* SAT Configuration
*/

#define VC_SAT_INT_EN_M          0x04    /* SAT Valid [change] interrupt   */
#define VC_SAT_INT_DIS_V         0x04    /* Disable the interrupt          */
#define VC_SAT_INT_ENA_V         0x00    /* Enable the interrupt           */

#define VC_SAT_ECC_M             0x03    /* SAT Expected Color Code        */
#define VC_SAT_ECC_5970_V        0x00    /* 5970 kHZ SAT Tone              */
#define VC_SAT_ECC_6000_V        0x01    /* 6000 kHZ SAT Tone              */
#define VC_SAT_ECC_6030_V        0x02    /* 6030 kHZ SAT Tone              */
#define VC_SAT_ECC_RSVD_V        0x03    /* Reserved                       */

/* MSM2.2 ONLY */
#define VC_DTMF_OVRWRT_M         0x08    /* DTMF overwrite control         */
#define VC_DTMF_OVRWRT_ENA_V     0x08    /* Enable overwrites of DTMF      */
#define VC_DTMF_OVRWRT_DIS_V     0x00    /* Disable overwrites of DTMF     */

/* MSM2.2 ONLY */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* SAT Level for transponding SAT
** -6dB..6dB in ~.07dB steps.  This must be written before soft reset.
*/

#define VC_SAT_LEVEL_NOM_V       0xDA    /* Nominal value                  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* SAT Threshold for valid SAT
** This must be written before soft reset.
*/

#define VC_SAT_THRESHOLD_NOM_V   0x19    /* Nominal value                  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Noise Suppressor Control (QDSP1+ only)
*/

#define VC_NS_DIAG_M             0x10    /* NS Diagnostics mode            */
#define VC_NS_DIAG_ENA_V         0x10    /* Enable NS Diagnostics          */
#define VC_NS_DIAG_DIS_V         0x00    /* Disable NS Diagnostics         */

#define VC_NS_MIN_GAIN_M         0x0E    /* NS Minimum Gain Value          */
#define VC_NS_13DB_MIN_GAIN_V    0x00    /* -13dB (default)                */
#define VC_NS_12DB_MIN_GAIN_V    0x02    /* -12dB                          */
#define VC_NS_11DB_MIN_GAIN_V    0x04    /* -11dB                          */
#define VC_NS_10DB_MIN_GAIN_V    0x06    /* -10dB                          */
#define VC_NS_9DB_MIN_GAIN_V     0x08    /*  -9dB                          */
#define VC_NS_8DB_MIN_GAIN_V     0x0A    /*  -8dB                          */  
#define VC_NS_7DB_MIN_GAIN_V     0x0C    /*  -7dB                          */
#define VC_NS_6DB_MIN_GAIN_V     0x0E    /*  -6dB                          */

#define VC_NS_MODE_M             0x01    /* NS Enable/Disable switch.      */
#define VC_NS_MODE_ENA_V         0x01    /* NS On                          */
#define VC_NS_MODE_DIS_V         0x00    /* NS Off                         */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Tx Deviation Limit:  Sets the value, lambda, to be used as the deviation
** limit by the deviation limiter.
**              Actual_deviation (kHz) = (DEVIATION_LIMIT / 256) * 20.
*/

#define VC_TX_DEV_LIMIT_NOM_V    0x67    /* Nominal value, ~8kHz           */
#define VC_TX_DEV_LIMIT_DATA_V   0x86    /* DATA SERVICES value, ~10.5kHz  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM Transmit Audio Gain:  Gain adjust for TX audio input.
** 8 bits unsigned, normalized (0,2).  -6dB, +6dB range, ~0.07dB steps.
** Takes effect within 3ms.
*/

#define VC_FM_TX_UNITY_GAIN_V    0x40    /* Nominal value, unity gain      */
#define VC_FM_TX_0_GAIN_V        0x00    /* Zero gain, muted               */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Voice Recognition Control (QDSP1+ only)
*/

#define VC_VR_NOT_DEFINED        0x00

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM Receive Audio Gain:  Gain adjust for RX audio input.
** 8 bits unsigned, normalized (0,2).  Takes effect within 3 ms.
*/

#define VC_FM_RX_UNITY_GAIN_V    0x40    /* Nominal value, unity gain      */
#define VC_FM_RX_0_GAIN_V        0x00    /* Zero gain, muted               */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* DTMF generation has 2 frequency values, a volume, and a duration.
** Writing the LSB of the duration causes the DTMF generation to start.
** If a DTMF with the same frequency values is currently being generated,
** it is continued with no change in phase.
*/

/* DTMF Duration:  negated 16-bit signed value in units of 5 milliseconds.
**
** Example: 40 milliseconds = 0xFFF8 (-8 decimal)
**
** DTMF generation is started by writing the LSB of the DTMF duration, and
** starts relative to the decoder subframe boundary.  DTMF commands which
** arrive 2-7ms before the subframe boundary are started on that subframe
** boundary.
*/

/* DTMF High Tone:  1 tone of DTMF, can be higher/lower than "low" tone.
**
** Vocoder tone = 32768.0 * cos( 2.0 * Pi * frequency / 8000.0 ) + 0.5
**
** 2*Pi radians is 1 cycle, 8000 is the sample rate of the Vocoder,
** 32768 is for scaling the number for fixed point arithmatic, the
** 0.5 is for rounding.  All values wrap to 0 at 65536.
**
** Example:  770 Hz = 26957 (decimal)
*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM DTMF Rx Gain
*/

#define VC_FM_DTMF_RX_NOM_V      0x17    /* Nominal value                  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM DTMF Tx Gain
*/

#define VC_FM_DTMF_TX_NOM_V      0x73    /* Nominal value                  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Decoder Volume
*/

#define VC_DEC_VOLUME_M6DB_V     0x0080  /* The volume for -6 dB gain      */
#define VC_DEC_VOLUME_UNITY_V    0x0100  /* The volume for unity gain      */
#define VC_DEC_VOLUME_MUTE_V     0x0000  /* The volume for silence/muted   */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* PCM Pad Control:  Define padding for linear CODECs
*/

#define VC_PAD_COMPANDED_M       0x10    /* Companded mode                 */
#define VC_PAD_COMPANDED_ENA_V   0x10    /* Enable Companded mode          */
#define VC_PAD_COMPANDED_DIS_V   0x00    /* Disable Companded mode         */

#define VC_PAD_LINEAR_M          0x08    /* Linear mode                    */
#define VC_PAD_LINEAR_ENA_V      0x08    /* Enable Linear mode             */
#define VC_PAD_LINEAR_DIS_V      0x00    /* Disable Linear mode            */

#define VC_PAD_BITS_M            0x07    /* PAD LSBs of linear decode PCM  */
#define VC_PAD_BITS_7_V          0x07    /* PAD = 7                        */
#define VC_PAD_BITS_6_V          0x06    /* PAD = 6                        */
#define VC_PAD_BITS_5_V          0x05    /* PAD = 5                        */
#define VC_PAD_BITS_4_V          0x04    /* PAD = 4                        */
#define VC_PAD_BITS_3_V          0x03    /* PAD = 3                        */
#define VC_PAD_BITS_2_V          0x02    /* PAD = 2                        */
#define VC_PAD_BITS_1_V          0x01    /* PAD = 1                        */
#define VC_PAD_BITS_0_V          0x00    /* PAD = 0                        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* PCM Pad Byte\HKF bitmask for NGP+ protocol.
*/
#define VC_HFK_MATCH_M           0x01    /* Bit to control sequence match  */
#define VC_HFK_UNUSED_M          0x02    /* Unused                         */
#define VC_HFK_PHS_MODE_M        0x04    /* PHS Override/Normal bit        */
#define VC_HFK_STEREO_MUTE_M     0x08    /* Stereo Mute/Unmute bit         */
#define VC_HFK_AEC_MODE_M        0x10    /* AEC CDMA/FM bit                */
#define VC_HFK_VOLUME_M          0xE0    /* HFK Volume bits                */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Input/Output Control
*/

#define VC_ENC_INT_EN_M          0x40    /* Encoder interrupt control      */
#define VC_ENC_INT_ENA_V         0x40    /* Enable                         */
#define VC_ENC_INT_DIS_V         0x00    /* Disable                        */

#define VC_DEC_INT_EN_M          0x10    /* Decoder interrupt control      */
#define VC_DEC_INT_ENA_V         0x10    /* Enable                         */
#define VC_DEC_INT_DIS_V         0x00    /* Disable                        */

#define VC_ENC_INT_POL_M         0x04    /* Encoder interrupt polarity     */
#define VC_ENC_INT_HI_V          0x04    /* Active High                    */
#define VC_ENC_INT_LO_V          0x00    /* Active Low                     */

#define VC_DEC_INT_POL_M         0x01    /* Decoder interrupt polarity     */
#define VC_DEC_INT_HI_V          0x01    /* Active High                    */
#define VC_DEC_INT_LO_V          0x00    /* Active Low                     */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Clear Encoder Interrupt
*/

#define VC_CLEAR_ENC_INT_M       0x01    /* Clear Encoder interrupt        */
#define VC_CLEAR_ENC_INT_ON_RD_V 0x01    /* Clear on packet read           */
#define VC_CLEAR_ENC_INT_V       0x00    /* Clear current interrupt        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Clear Decoder Interrupt
*/

#define VC_CLEAR_DEC_INT_M       0x01    /* Clear Decoder interrupt        */
#define VC_CLEAR_DEC_INT_ON_RD_V 0x01    /* Clear on packet write          */
#define VC_CLEAR_DEC_INT_V       0x00    /* Clear current interrupt        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Decoder Packet
*/

#define VC_DEC_LOAD_FIR_M        0x20    /* Packet had filter coefficients */
#define VC_DEC_LOAD_FIR_V        0x20    /* Coefficients present           */
#define VC_DEC_NO_FIR_V          0x00    /* Coefficients not present       */

#define VC_DEC_FULL_LIKELY_M     0x10    /* Flag full-rate-likely frames   */
#define VC_DEC_FULL_LIKELY_V     0x10    /* Frame is a full-rate-likely    */
#define VC_DEC_FRAME_OK_V        0x00    /* Frame is OK                    */

#define VC_DEC_ERASE_M           0x08    /* Flag erased frame              */
#define VC_DEC_ERASE_V           0x08    /* Frame is an erasure            */
#define VC_DEC_NOT_ERASED_V      0x00    /* Frame is OK                    */

#define VC_DEC_BLANK_M           0x04    /* Flag blanked frame             */
#define VC_DEC_BLANK_V           0x04    /* Frame is blanked               */
#define VC_DEC_NOT_BLANK_V       0x00    /* Frame is OK                    */

#define VC_DEC_RATE_M            0x03    /* Decode rate                    */
#define VC_DEC_FULL_V            0x00    /* Full                           */
#define VC_DEC_HALF_V            0x01    /* Half                           */
#define VC_DEC_QTR_V             0x02    /* Quarter                        */
#define VC_DEC_EIGHTH_V          0x03    /* Eighth                         */

/* MSM2.2 ONLY */

#define VC_DEC_PACKET_FMT_M      0x80
#define VC_DEC_8K_V              0x00
#define VC_DEC_13K_V             0x80

/* END MSM2.2 ONLY */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* PCM Control Word MSByte:  pad byte for mu-law or a-law CODEC.
Note: For MSM2.3 this has been renamed to CODEC_CTL1
*/

#define VC_AUX_CLK_VAL_M         0x40    /* Aux PCM Clock value            */
#define VC_AUX_CLK_VAL_HI_V      0x40    /* High                           */
#define VC_AUX_CLK_VAL_LO_V      0x00    /* Low                            */

#define VC_AUX_SYNC_VAL_M        0x20    /* Aux PCM Sync value             */
#define VC_AUX_SYNC_VAL_HI_V     0x20    /* High                           */
#define VC_AUX_SYNC_VAL_LO_V     0x00    /* Low                            */

#define VC_AUX_DOUT_VAL_M        0x10    /* Aux DOUT value                 */
#define VC_AUX_DOUT_VAL_HI_V     0x10    /* High                           */
#define VC_AUX_DOUT_VAL_LO_V     0x00    /* Low                            */

#define VC_AUX_CODEC_SEL_M       0x08    /* PCM Clock source               */
#define VC_AUX_CODEC_NORM_V      0x00    /* Normal (Aux DOUT=AUX_DOUT_VAL) */
#define VC_AUX_CODEC_AUX_V       0x08    /* Auxiliary output               */

#define VC_AUX_POLARITY_M        0x04    /* PCM Clock source               */
#define VC_AUX_POL_INVERTED_V    0x04    /* Modulator                      */
#define VC_AUX_POL_NORMAL_V      0x00    /* PCM_CLK pin                    */

#define VC_AUX_CLK_CONTROL_M     0x02    /* Aux PCM Clock / Sync control   */
#define VC_AUX_CLK_ENA_V         0x02    /* Enable                         */
#define VC_AUX_CLK_DIS_V         0x00    /* Disable (use AUX_CLK/SYNC_VAL) */

#define VC_SCS_SEL_M             0x01    /* SCS Select                     */
#define VC_SCS_VOC_V             0x01    /* SCS from Vocoder               */
#define VC_SCS_LONG_AUX_SYNC_V   0x00    /* SCS from Long Aux CODEC sync   */

#define VC_AUX_DEFAULT_M  ( VC_AUX_POLARITY_M       | VC_AUX_CLK_VAL_M  \
                          | VC_AUX_SYNC_VAL_M       | VC_AUX_CODEC_SEL_M \
                          | VC_AUX_CLK_CONTROL_M    | VC_SCS_SEL_M \
                          | VC_AUX_DOUT_VAL_M )

#if (defined(FEATURE_HFK_PLUS) || (TG==T_Q))
#error code not present
#else
#define VC_AUX_DEFAULT_V  ( VC_AUX_POL_INVERTED_V   | VC_AUX_CLK_VAL_LO_V \
                          | VC_AUX_SYNC_VAL_HI_V    | VC_AUX_CODEC_NORM_V \
                          | VC_AUX_CLK_DIS_V        | VC_SCS_LONG_AUX_SYNC_V \
                          | VC_AUX_DOUT_VAL_HI_V )

#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Configure GPIO 29 and GPIO 30 as GPIOs or MASK_DATA and WSYM_CLK
*/

#define VC_GPIO_29_CTL_M         0x01    /* GPIO #29 MUX control           */
#define VC_GPIO_29_ENA_V         0x00    /* GPIO #29 as a GPIO             */
#define VC_GPIO_29_DIS_V         0x01    /* GPIO #29 as Walsh Symbol Clock */

#define VC_GPIO_30_CTL_M         0x02    /* GPIO #30 MUX control           */
#define VC_GPIO_30_ENA_V         0x00    /* GPIO #30 as a GPIO             */
#define VC_GPIO_30_DIS_V         0x02    /* GPIO #30 as Mask Data          */

#define VC_GPIO_INT1_SEL_M       0x10    /* GPIO INT1 control              */
#define VC_GPIO_INT1_SEL_V       0x10    /* Select internal flip-flop      */
#define VC_GPIO_INT1_DESEL_V     0x00    /* Deselect internal flip-flop    */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* One's Polarity:  PCM direction and polarity of the 1's detect sensor.
*/

/* MSM 2.3 additions */
#define VC_U_186_PUNCT_M         0x40    /* UART Puncture PCM (1)          */
#define VC_U_186_PUNCT_ENA_V     0x00    /* UART Puncture PCM Enable (1)   */
#define VC_U_186_PUNCT_DIS_V     0x40    /* UART Puncture PCM Disable (1)  */

#define VC_PCM_DOUT_N_EN_M       0x20    /* UART Puncture PCM (2)          */
#define VC_PCM_DOUT_N_EN_ENA_V   0x20    /* UART Puncture PCM Enable (2)   */
#define VC_PCM_DOUT_N_EN_DIS_V   0x00    /* UART Puncture PCM Disable (2)  */

#define VC_CLK_SYNC_LB_M         0x10    /* PCM SYNC and CLOCK LOOPBACK    */
#define VC_CLK_SYNC_LB_ENA_V     0x10    /* PCM SYNC & CLOCK LPBK Enable   */
#define VC_CLK_SYNC_LB_DIS_V     0x00    /* PCM SYNC & CLOCK LPBK Disable  */

#define VC_DIN_LOOP_M            0x08    /* PCM DIN LOOPBACK               */
#define VC_DIN_LOOP_ENA_V        0x08    /* PCM DIN LOOPBACK Enable        */
#define VC_DIN_LOOP_DIS_V        0x00    /* PCM DIN LOOPBACK Disable       */
/* End MSM 2.3 additions */

#define VC_PCM_CLK_DIR_M         0x04    /* PCM Clock source               */
#define VC_PCM_CLK_DIR_MOD_V     0x04    /* Modulator                      */
#define VC_PCM_CLK_DIR_EXT_V     0x00    /* PCM_CLK pin                    */

#define VC_PCM_SYNC_DIR_M        0x02    /* PCM Sync source                */
#define VC_PCM_SYNC_DIR_MOD_V    0x02    /* Modulator                      */
#define VC_PCM_SYNC_DIR_EXT_V    0x00    /* PCM_SYNC pin                   */

#define VC_ONES_POLARITY_M       0x01    /* Polarity of the one's detect   */
#define VC_DETECT_ONES_V         0x00    /* Detect 1's                     */
#define VC_DETECT_ZEROES_V       0x01    /* Detect 0's                     */

/*---------------------------------------------------------------------------
                      Vocoder Core Read Registers
---------------------------------------------------------------------------*/
/* Vocoder Status (non-FM status)
*/

#define VC_ONES_DETECT_M         0x80    /* One's detector status          */
#define VC_ONES_TRIGGERED_V      0x80    /* DIN == ONES_POLARITY triggered */
#define VC_ONES_IDLE_V           0x00    /* Detector has not triggered     */

#define VC_BIST_COMPLETE_M       0x40    /* BIST completion bit            */
#define VC_BIST_COMPLETE_V       0x40    /* BIST is complete               */
#define VC_BIST_INCOMPLETE_V     0x00    /* BIST is incomplete             */

#define VC_ENC_PCM_INT_M         0x20    /* ENC_PCM interrupt status       */
#define VC_ENC_PCM_INT_V         0x20    /* Interrupt active               */
#define VC_ENC_PCM_NO_INT_V      0x00    /* No interrupt active            */

#define VC_ENC_DRQ_M             0x10    /* Encoder DMA Request            */
#define VC_ENC_DRQ_ACTIVE_V      0x10    /* DMA request active             */
#define VC_ENC_DRQ_NOT_ACTIVE_V  0x00    /* DMA Request NOT active         */

#define VC_ENC_INT_M             0x08    /* Encoder interrupt status       */
#define VC_ENC_INT_V             0x08    /* Interrupt active               */
#define VC_ENC_NO_INT_V          0x00    /* No interrupt active            */

#define VC_DEC_PCM_INT_M         0x04    /* DEC_PCM interrupt status       */
#define VC_DEC_PCM_INT_V         0x04    /* Interrupt active               */
#define VC_DEC_PCM_NO_INT_V      0x00    /* No interrupt active            */

#define VC_DEC_DRQ_M             0x02    /* Decoder DMA Request            */
#define VC_DEC_DRQ_ACTIVE_V      0x02    /* DMA request active             */
#define VC_DEC_DRQ_NOT_ACTIVE_V  0x00    /* DMA Request NOT active         */

#define VC_DEC_INT_M             0x01    /* Decoder interrupt status       */
#define VC_DEC_INT_V             0x01    /* Interrupt active               */
#define VC_DEC_NO_INT_V          0x00    /* No interrupt active            */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* FM Status
*/

#define VC_SAT_DCC_M             0x0C    /* SAT Detected Color Code        */
#define VC_SAT_DCC_5970_V        0x00    /* 5970 HZ SAT Tone               */
#define VC_SAT_DCC_6000_V        0x04    /* 6000 HZ SAT Tone               */
#define VC_SAT_DCC_6030_V        0x08    /* 6030 HZ SAT Tone               */
#define VC_SAT_DCC_INVALID_V     0x0C    /* SAT tone not valid             */

#define VC_MUTE_WBD_M            0x02    /* Dotting Detect                 */
#define VC_MUTE_ENA_V            0x02    /* Dotting detected               */
#define VC_MUTE_DIS_V            0x00    /* Dotting not detected           */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Encoder Packet
*/

#define VC_ENC_FMT_PEEK_M        0x10    /* Peek data available            */
#define VC_ENC_PEEK_V            0x10    /* Available                      */
#define VC_ENC_NO_PEEK_V         0x00    /* No peek data                   */

#define VC_ENC_FMT_DIAG_M        0x08    /* Diag data available            */
#define VC_ENC_DIAG_V            0x08    /* Available                      */
#define VC_ENC_NO_DIAG_V         0x00    /* No diag data                   */

#define VC_ENC_FMT_BLANK_M       0x04    /* Blanked frame                  */
#define VC_ENC_BLANK_V           0x04    /* Blanked                        */
#define VC_ENC_NOT_BLANK_V       0x00    /* Not blanked                    */

#define VC_ENC_FMT_RATE_M        0x03    /* Frame rate                     */
#define VC_ENC_FULL_RATE_V       0x00    /* Full rate                      */
#define VC_ENC_HALF_RATE_V       0x01    /* Half rate                      */
#define VC_ENC_QTR_RATE_V        0x02    /* Quarter rate                   */
#define VC_ENC_8TH_RATE_V        0x03    /* Eighth rate                    */

/* MSM2.2 ONLY */

#define VC_ENC_PACKET_FMT_M      0x80    /* Decoder Packet format          */
#define VC_ENC_8K_V              0x00    /* 8k packet format  */
#define VC_ENC_13K_V             0x80    /* 13k packet format */

#define VC_DTMF_STAT_M           0x20    /* DTMF Status  */
#define VC_DTMF_STAT_NORM_V      0x00    /* Contains Normal packet data  */
#define VC_DTMF_STAT_DET_V       0x20    /* Contains DTMF detection data */


/* END MSM2.2 ONLY */

/*---------------------------------------------------------------------------
                           MACRO VC_OUT

This macro outputs a specified value to a specified port. All 8 bits of the
port are written (i.e., the port is not buffered by software).
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define VC_OUT( io, val )        MSM_OUT( io, val )
#endif /* T_MSM3 */
/*---------------------------------------------------------------------------
                           MACRO VC_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define VC_OUTM( io, mask, val ) MSM_OUTM( io, mask, val)
#endif /* T_MSM3 */

/*---------------------------------------------------------------------------
                           MACRO VC_IN

This macro reads and returns the value from a specified port.  NO mask is
applied to the read value.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define VC_IN( io )              MSM_IN( io )
#endif /* T_MSM3 */

/*---------------------------------------------------------------------------
                           MACRO VC_INM

This macro reads and returns the value from a specified port.  The mask is
applied to the read value.  The bits that do not correspond to the mask are
returned as zero.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define VC_INM( io, mask )       MSM_INM( io, mask )
#endif /* T_MSM3 */

#endif /* VOC_CORE_H */

