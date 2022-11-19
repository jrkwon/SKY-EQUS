#ifndef DFM_H
#define DFM_H
/*===========================================================================

                     D I G I T A L   F M   S E R V I C E S
                            H E A D E R    F I L E

DESCRIPTION
  This file contains declarations and definitions to support interaction
  with the Digital FM section of the Qualcomm MSM2.

See Also MSM2P HLD.

Copyright (c) 1994,1995,1996,1997,1998,1999 by QUALCOMM, Incorporated.
                                                      All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/dfm.h_v   1.1   10 Sep 1999 18:10:32   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/03/98   ss      Add in values for DFM slotted mode wake-up timers (for non
                   Surf / non-SBI builds).
09/01/98   kss/ajn Added DFM_FREQ_TRK_PDM_V, DFM_Q_PDM_MSB_V, DFM_I_PDM_MSB_V
10/09/97   udp     Updated Copyright
10/08/97   smo/udp Added MSM2300 support defines.
01/16/95   jah     Added DFM_FREQ_SENSE_GAIN_0_V, zero gain FM Tx
10/10/95   fkm     Minor Macro Changes to save ROM.
06/18/94   tdw     Created.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "processor.h"
#include "target.h"             /* Definition for target specific defs */

#if defined (T_MSM3)
#include "msm.h"
#endif /* T_MSM3 */
/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                           Digital FM Registers
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                      IQ Demodulator WRITE Registers
---------------------------------------------------------------------------*/

/* Digital FM initialization Register */

#define DFM_360_M             0x01     /* Enable 4.92 two-phase generator. */
#define DFM_360_ENA_V         0x01
#define DFM_360_DIS_V         0x00

#define DFM_360_RESET_M       0x02     /* Reset the 3/41 clock divider.    */
#define DFM_360_RESET_ENA_V   0x02
#define DFM_360_RESET_DIS_V   0x00

#define DFM_360_MODE_M        0x04     /* Select clock mode mask.          */
#define DFM_360_MODE_NORM_V   0x00     /* Normal = 360Khz clock mode.      */
#define DFM_360_MODE_TEST_V   0x04     /* Test = 2.46Mhz clock mode.       */

#define DFM_RESET_M           0x08     /* Reset Digital FM ciruitry        */
#define DFM_RESET_ENA_V       0x08
#define DFM_RESET_DIS_V       0x00

#define DFM_CDMA_OR_FM_MODE_M 0x10     /* Select CDMA or FM mode mask.     */
#define DFM_FM_MODE_V         0x00
#define DFM_CDMA_MODE_V       0x10

#define DFM_TX_FM_M           0x20     /* Enable TX ciruitry mask.         */
#define DFM_TX_FM_ENA_V       0x20
#define DFM_TX_FM_DIS_V       0x00

#define DFM_IQ_FMT_M           0x40    /* Select IQ data format.           */
#define DFM_IQ_FMT_TWOS_COMP_V 0x00    /* IQ Data is twos complement.      */
#define DFM_IQ_FMT_OFF_BIN_V   0x40    /* IQ Data is offset binary.        */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This Register Sets the Gain of the I and Q DC loop */

#define DFM_I_DC_OFFSET_GAIN_M 0x07    /* I DC offset gain mask. Bits 0-2  */
#define DFM_Q_DC_OFFSET_GAIN_M 0x38    /* Q DC offset gain mask. Bits 3-5  */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Sets the Gain of the AGC loop. */

#define DFM_AGC_DECAY_M       0x0F
#define DFM_AGC_ATTACK_M      0xF0

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the Frequency Tracking loop. */

#define DFM_FREQ_TRK_GAIN_M   0x07     /* Frequency tracking loop gain mask. */

#define DFM_FREQ_DITHER_M     0x08     /* Dithering function mask.         */
#define DFM_FREQ_DITHER_ENA_V 0x08
#define DFM_FREQ_DITHER_DIS_V 0x00

#define DFM_FREQ_DITHER_CHRP_M     0x10   /* Chirp signal for dithering    */
#define DFM_FREQ_DITHER_CHRP_ENA_M 0x10   /* the loop filter output.       */
#define DFM_FREQ_DITHER_CHRP_DIS_M 0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the PDM ciruitry */

#define DFM_DC_PDM_POL_M      0x01     /* IQ DC OFFSET PDM polarity mask.  */
#define DFM_DC_PDM_NORM_V     0x00
#define DFM_DC_PDM_INV_V      0x01

#define DFM_RX_AGC_PDM_POL_M  0x02     /* RX AGC PDM polarity mask.        */
#define DFM_RX_AGC_PDM_NORM_V 0x00
#define DFM_RX_AGC_PDM_INV_V  0x02

#define DFM_FREQ_PDM_POL_M    0x04     /* Freq. track PDM polarity mask.   */
#define DFM_FREQ_PDM_NORM_V   0x00
#define DFM_FREQ_PDM_INV_V    0x04

#define DFM_DC_PDM_OE_M       0x08     /* IQ DC OFFSET PDM output enable mask.   */
#define DFM_DC_PDM_OE_ENA_V   0x08
#define DFM_DC_PDM_OE_DIS_V   0x00

#define DFM_RX_AGC_PDM_OE_M     0x10   /* RX AGC PDM output enable mask.   */
#define DFM_RX_AGC_PDM_OE_ENA_V 0x10
#define DFM_RX_AGC_PDM_OE_DIS_V 0x00

#define DFM_FREQ_PDM_OE_M     0x20     /* Freq. track PDM output enable mask.    */
#define DFM_FREQ_PDM_OE_ENA_V 0x20
#define DFM_FREQ_PDM_OE_DIS_V 0x00

#define DFM_LOOP_STAT_SEL_M      0xC0  /* Loop status select mask.         */
#define DFM_LOOP_STAT_AGC_RSSI_V 0x00  /* AGC and RSSI status.             */
#define DFM_LOOP_STAT_FREQ_V    (0x01 << 6)     /* Freq. Track status.     */
#define DFM_LOOP_STAT_I_DC_V    (0x02 << 6)     /* I DC offset status.     */
#define DFM_LOOP_STAT_Q_DC_V    (0x03 << 6)     /* Q DC offset status.     */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register contains the I and Q DC MSB's as well as the Mode control
** bits.  The mode control bits choose either the DC loop filter or uP as
** inputs to the PDM. */

#define DFM_I_PDM_MSB_M       0x01  /* I DC PDM MSBit mask.                */
#define DFM_I_PDM_MSB_V       0x01  /* I DC PDM MSBit set value.           */

#define DFM_Q_PDM_MSB_M       0x02  /* Q DC PDM MSBit mask.                */
#define DFM_Q_PDM_MSB_V       0x02  /* Q DC PDM MSBit set value.           */

#define DFM_I_DC_INPUT_SEL_M  0x04  /* I DC PDM input select mask.         */
#define DFM_I_DC_NORM_V       0x00
#define DFM_I_DC_MICRO_V      0x04

#define DFM_Q_DC_INPUT_SEL_M  0x08  /* Q DC PDM input select mask.         */
#define DFM_Q_DC_NORM_V       0x00
#define DFM_Q_DC_MICRO_V      0x08

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register is the uP overwrite value and Mode control register for
** the RX AGC PDM.
*/

#define DFM_RX_AGC_PDM_M      0x7F     /* RX AGC PDM uP overwrite value mask. */

#define DFM_AGC_INPUT_SEL_M   0x80     /* RX AGC PDM input select mask.       */
#define DFM_AGC_NORM_V        0x00
#define DFM_AGC_MICRO_V       0x80


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register is the uP overwrite MSB and Mode control for the Frequency
** Tracking loop PDM.
*/

#define DFM_FREQ_TRK_PDM_M       0x01  /* Freq Track PDM MSBit mask.       */
#define DFM_FREQ_TRK_PDM_V       0x01  /* Freq Track PDM MSBit set value.  */

#define DFM_FREQ_TRK_INPUT_SEL_M 0x02  /* Freq Track PDM input select mask.*/
#define DFM_FREQ_TRK_NORM_V      0x00
#define DFM_FREQ_TRK_MICRO_V     0x02

#define DFM_FREQ_TRK_FREEZE_M    0x04  /* Freq Track PDM input select mask.*/
#define DFM_FREQ_TRK_FREEZE_V    0x04  /* MSM23 Only */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the Vocoder interface block. */

#define DFM_FM_REQ_SEL_M        0x01   /* 1ms or .25ms mode select mask.   */
#define DFM_FM_REQ_SEL_1MS_V    0x00
#define DFM_FM_REQ_SEL_0_25MS_V 0x01


#define DFM_INTF_LOOPBACK_M   0x02     /* Vocoder interface loopback select. */
#define DFM_INTF_NORM_V       0x00
#define DFM_INTF_LOOPBACK_V   0x02

/*---------------------------------------------------------------------------
                     IQ Demodulator READ Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* The following registers Read various loop filter accumulator statuses and
** the Receive Signal Strength Indicator. (RSSI) */
#define DFM_IQDMOD_RSSI_M      0x7F         /* mask for active RSSI bits */



/*---------------------------------------------------------------------------
                     TX Wideband Data WRITE Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* This register is the uP overwrite MSB for the TX AGC PDM.  It also contains
** the polarity and tristate enable for TX AGC PDM. */

#define DFM_TX_AGC_PDM_M      0x01

#define DFM_TX_AGC_PDM_POL_M  0x02     /* TX AGC PDM polarity select mask. */
#define DFM_TX_AGC_PDM_NORM_V 0x00
#define DFM_TX_AGC_PDM_INV_V  0x02

#define DFM_TX_AGC_PDM_OE_M      0x04  /* TX AGC PDM output enable mask.   */
#define DFM_TX_AGC_PDM_OE_ENA_V  0x04
#define DFM_TX_AGC_PDM_OE_DIS_V  0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register sets the TX FM GAIN */

#define DFM_FREQ_SENSE_GAIN_0_V   0    /* Zero gain, disable the output */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the TX FM circuitry. */

#define DFM_TX_AUDIO_MUTE_M      0x01  /* TX audio path mute mask.         */
#define DFM_TX_AUDIO_MUTE_ENA_V  0x01
#define DFM_TX_AUDIO_MUTE_DIS_V  0x00

#define DFM_TX_CTL_RESET_M       0x02  /* DFM_TXCNTRL reset mask.          */
#define DFM_TX_CTL_NORM_V        0x00
#define DFM_TX_CTL_SET_V         0x02


/*---------------------------------------------------------------------------
                      TX Wideband Data READ Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register contains Busy/Idle, PA_ON, and TX underrun status. */

#define DFM_BUSY_IDLE_M       0x01     /* Busy/IDLE mask.                  */
#define DFM_BUSY_V            0x00
#define DFM_IDLE_V            0x01

#define DFM_TX_CTL_STAT_M     0x02     /* DFM_TXCNTRL status mask. ??????? */
#define DFM_PA_ON_V           0x02
#define DFM_PA_OFF_V          0x00

#define DFM_WBD_MUTE_STAT_M   0x04     /* TX underrun status mask.         */
#define DFM_NO_UNDERRUN_V     0x00
#define DFM_UNDERRUN_V        0x04


/*---------------------------------------------------------------------------
                      RX Wideband Data WRITE Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register sets the Aquistion and Tracking Bandwidth for the timing
** recovery DPLL. */

#define DFM_TRK_BW_M          0x07           /* Tracking Bandwidth Mask    */
#define DFM_ACQ_BW_M          0x38           /* Aquisition Bandwidth Mask  */

#define DFM_DPLL_FALSLOCKEN_M 0x40           /* False Lock mode MSM23 Only */
#define DFM_DPLL_FALSLOCKEN_V 0x40           /* Set to enable, clear to dis */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the RX Wideband Data block. */

#define DFM_FOCC_FVC_SEL_M    0x01     /* Select Forword Control or Forward Voice */
#define DFM_FOCC_V            0x01
#define DFM_FVC_V             0x00

#define DFM_RX_WBD_INV_M      0x02     /* Invert data input polarity mask. */
#define DFM_RX_WBD_INV_V      0x02
#define DFM_RX_WBD_NORM_V     0x00

#define DFM_ROAM_STAT_M       0x04
#define DFM_ROAMING_V         0x04     /* Not Sure what these should be ?? */
#define DFM_NOT_ROAMING_V     0x00     /* ???? */

#define DFM_MAJ_VOTE_MODE_M   0x08
#define DFM_MAJ_VOTE_3_5_V    0x00     /* 3/5 Majority Vote             */
#define DFM_MAJ_VOTE_ALL_V    0x08     /* 2/3, 3/4 or 3/5 Majority Vote */

#define DFM_WORD_SYNC_SEL_M   0x10     /* Word sync detection mask      */
#define DFM_DOT_BI_WS_V       0x00     /* Sync on dotting5 + bi1 + ws11 */
#define DFM_WS_V              0x10     /* Sync on ws11 only             */

#define DFM_MY_DATA_INT_M     0x20     /* My Data detect interrupt mask.*/
#define DFM_MY_DATA_INT_ENA_V 0x20
#define DFM_MY_DATA_INT_DIS_V 0x00

#define DFM_ANY_DATA_INT_M     0x40    /* Any Data detect interrupt mask. */
#define DFM_ANY_DATA_INT_ENA_V 0x40
#define DFM_ANY_DATA_INT_DIS_V 0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* This register Configures the RX Wideband Data block (MSM23 Only). */

#define DFM_NRZDATA_WR_M      0x02
#define DFM_NRZDATA_WR_V      0x02

#define DFM_NRZDATA_WR_MODE_M 0x01
#define DFM_NRZDATA_WR_MODE_V 0x01


/*---------------------------------------------------------------------------
                     RX Wideband Data READ Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#if defined (T_MSM3)

/* DFM Slotted Sleep Control */

#define DFM_SLOT_CTL_WB__SLEEP_EN_ENA                   1
#define DFM_SLOT_CTL_WB__SLEEP_EN_DIS                   0

#define DFM_SLOT_CTL_WB__MAJORITY_EN_ENA                (1 << 1)
#define DFM_SLOT_CTL_WB__MAJORITY_EN_DIS                (0 << 1)

#define DFM_SLOT_CTL_WB__CRC_EN_ENA                     (1 << 2)
#define DFM_SLOT_CTL_WB__CRC_EN_DIS                     (0 << 2)

#define DFM_SLOT_CTL_WB__FILLER_EN_ENA                  (1 << 3)
#define DFM_SLOT_CTL_WB__FILLER_EN_DIS                  (0 << 3)

#define DFM_SLOT_CTL_WB__SYNC_ERASE_EN_ENA              (1 << 4)
#define DFM_SLOT_CTL_WB__SYNC_ERASE_EN_DIS              (0 << 4)

#define DFM_SLOT_CTL_WB__STREAM_SLOT_EN_ENA             (1 << 5)
#define DFM_SLOT_CTL_WB__STREAM_SLOT_EN_DIS             (0 << 5)

#define DFM_SLOT_CTL_WB__SLEEP_OVERRIDE_N_ENA           (1 << 6)
#define DFM_SLOT_CTL_WB__SLEEP_OVERRIDE_N_DIS           (0 << 6)

#define DFM_SLOT_CTL_WB__SLEEP_FORCE_N_ENA              (1 << 7)
#define DFM_SLOT_CTL_WB__SLEEP_FORCE_N_DIS              (0 << 7)

/* DFM slotted mode wakeup timers */
#if defined (T_SURF) || defined (FEATURE_SBI)

#define DFM_DPLL_WU_TIMER_VAL                           0x09
#define DFM_DATA_WU_TIMER_STREAM_A_VAL                  0xE1  /* stream A */
#define DFM_DATA_WU_TIMER_STREAM_B_VAL                  0x10  /* stream B */
#define DFM_SYNC_WU_TIMER_VAL                           0xD6
#define DFM_STREAM_SLOT_TIMER_VAL                       0x21

#else

/* These timers need to calculated based on specific targets */

#define DFM_DPLL_WU_TIMER_VAL                           0x09  
#define DFM_DATA_WU_TIMER_STREAM_A_VAL                  0xE1  
#define DFM_DATA_WU_TIMER_STREAM_B_VAL                  0x10  
#define DFM_SYNC_WU_TIMER_VAL                           0xD6  
#define DFM_STREAM_SLOT_TIMER_VAL                       0x21  

#endif /* T_SURF || FEATURE_SBI */

/* DFM Slotted Sleep PDM Control */

#define DFM_SLOT_PDM_CTL_WB__PDM_LOW_PWR_EN_ENA         1
#define DFM_SLOT_PDM_CTL_WB__PDM_LOW_PWR_EN_DIS         0

#define DFM_SLOT_PDM_CTL_WB__FREQ_TRACK_TRI_EN_ENA      (1 << 1)
#define DFM_SLOT_PDM_CTL_WB__FREQ_TRACK_TRI_EN_DIS      (0 << 1)

#define DFM_SLOT_PDM_CTL_WB__RXAGC_TRI_EN_ENA           (1 << 2)
#define DFM_SLOT_PDM_CTL_WB__RXAGC_TRI_EN_DIS           (0 << 2)

#define DFM_SLOT_PDM_CTL_WB__IQ_OFFSET_TRI_EN_ENA       (1 << 3)
#define DFM_SLOT_PDM_CTL_WB__IQ_OFFSET_TRI_EN_DIS       (0 << 3)

#define DFM_DC_PDM_2_WB__DC_PDM_RANGE_SEL_ENA            0x40
#define DFM_DC_PDM_2_WB__DC_PDM_RANGE_SEL_DIS            0x00

#define DFM_DC_PDM_2_WB__Q_DC_OFFSET_FREEZE_ENA          0x20
#define DFM_DC_PDM_2_WB__Q_DC_OFFSET_FREEZE_DIS          0x00

#define DFM_DC_PDM_2_WB__I_DC_OFFSET_FREEZE_ENA          0x10
#define DFM_DC_PDM_2_WB__I_DC_OFFSET_FREEZE_DIS          0x00

#define DFM_DC_PDM_2_WB__Q_DC_IN_SEL_ENA                 0x08
#define DFM_DC_PDM_2_WB__Q_DC_IN_SEL_DIS                 0x00

#define DFM_DC_PDM_2_WB__I_DC_IN_SEL_ENA                 0x04
#define DFM_DC_PDM_2_WB__I_DC_IN_SEL_DIS                 0x00

#define DFM_RXAGC_PDM_1_WB__RXAGC_PDM_FREEZE_ENA         0x01
#define DFM_RXAGC_PDM_1_WB__RXAGC_PDM_FREEZE_DIS         0x00

#define DFM_RXWBD_CONFIG_1_WB__AUDIO_LOW_PWR_EN_ENA      0x02
#define DFM_RXWBD_CONFIG_1_WB__AUDIO_LOW_PWR_EN_DIS      0x00

#define DFM_TXFM_CONFIG_WB__TXAGC_TRI_EN_ENA             0x04
#define DFM_TXFM_CONFIG_WB__TXAGC_TRI_EN_DIS             0x00

#define DFM_TXFM_CONFIG_WB__TX_AUDIO_MUTE_ENA            0x01
#define DFM_TXFM_CONFIG_WB__TX_AUDIO_MUTE_DIS            0x00

#define DFM_RXWBD_CONFIG_0_WB__BCH_BYPASS_ENA            0x80
#define DFM_RXWBD_CONFIG_0_WB__BCH_BYPASS_DIS            0x00

#define DFM_RXWBD_CONFIG_1_WB__FOCC_MVOTE_MODE_ENA       0x01
#define DFM_RXWBD_CONFIG_1_WB__FOCC_MVOTE_MODE_DIS       0x00

#define DFM_TXFM_CONFIG_WB__DFM_TXCTL_EN_ENA             0x02
#define DFM_TXFM_CONFIG_WB__DFM_TXCTL_EN_DIS             0x00

#endif /* T_MSM3 */
/*---------------------------------------------------------------------------
                 END of Digital FM Register Definitions
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                           MACRO DFM_OUT

This macro outputs a specified value to a specified port. All 8 bits of the
port are written (i.e., the port is not buffered by software).
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define DFM_OUT( io, val )  MSM_OUT( io, val )
#endif /* T_MSM3 */


/*---------------------------------------------------------------------------
                           MACRO DFM_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which
is then combined with the new value.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define DFM_OUTM( io, mask, val) MSM_OUTM_IL( io, mask, val)
#endif /* T_MSM3 */

/*---------------------------------------------------------------------------
                           MACRO DFM_IN
This macro reads and returns the value from a specified port. NO mask is
applied to the read value.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define DFM_IN( io )  MSM_IN( io )
#endif /* T_MSM3 */

/*---------------------------------------------------------------------------
                           MACRO DFM_INM
This macro reads and returns the value from a specified port. The mask is
applied to the read value. The bits that do not correspond to the mask are
returned as zero.
---------------------------------------------------------------------------*/
#if defined (T_MSM3)
#define DFM_INM( io, mask ) MSM_INM( io, mask )
#endif /* T_MSM3 */

#endif /* DFM_H */

