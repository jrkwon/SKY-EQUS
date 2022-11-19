#ifndef DMOD_H
#define DMOD_H
/*===========================================================================

              D E M O D U L A T O R    C H I P    S E R V I C E S

                            H E A D E R    F I L E

DESCRIPTION
  This file contains declarations and definitions to support interaction
  with the QUALCOMM CDMA Demodulator Chip.

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999, 2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/dmod.h_v   1.5   11 May 2001 11:10:10   evanmell  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/11/01   ejv     Removed T_MSM31.
01/18/01   ejv     Added MSM5100 support.
12/11/00   jcw     Added DMOD_RX_DATA(2)_INT_POL values for MSM5105
12/11/00   rmd     Added bit definitions for INT_STATUS_0 and INT_STATUS_1
10/16/00   ejv     Removed T_SURF.
06/29/00   aks     Added Bluetooth, MMC Controller and QDSP2 INT1 definitions
                   for MSM3300.
02/11/00   dgy     Removed FEATURE_PA_RANGE_DIG_COMP.
01/14/00   ejv     Merged FEATURE_PA_RANGE_DIG_COMP for TigerII.
10/27/99   ajn     Added TigerII interrupt for QPCH.
10/11/99   ejv     Removed T_MSM2P checking.
09/24/99   rm      Added in macros for MAC Engine.
08/06/99   ejv     Mainlined T_MSM3 support.  Changed DMOD_LOCK_RES_V to
                   DMOD_F_UNLOCK_V.  Added preliminary Pink Panther support
                   and initial Tiger2 support.
04/16/99   ejv     Added parentheses around expressions in outp commands.
01/13/99   ejv     Removed #ifndef T_MSM3 code.
12/02/98   ejv     Replaced INT 26 value and mask with DP RX2 DATA int. 
10/29/98   ejv     jkl: Added word size mask for Interrupt Control Registers
10/16/98   ejv     Merged changes to support Panther.
03/04/98   jjn     Added DMOD_GP_I_POL2_INV_V and DMOD_GP_I_POL1_INV_V for
                   invereted polarity.
02/02/98   smo     Changed erroneous values for time-tracking gain constants,
                   TT_K1 and TT_K2, MSM2300 only.
10/07/97   smo     Added MSM2300 specific definitions.
06/20/97   thh     Added DMOD_GP_I_POL3_INV_V value for inverted polarity.
01/02/97   fkm     Added DMOD_GP_I_POL0_INV_V value for Inverted Polarity
                   of this interrupt.  Needed for TGP Dial Shuttle.
06/07/96   smo     Added Pilot filter bandwidth values for PCS/800 settings.
10/10/95   fkm     Minor Macro Changes to save ROM.
09/12/95   tdw/jah Added DMOD_Q_C_* bits in DMOD_AGC_CTL register.
08/29/95   jah     Fixed swapped DMOD_VOC_ENC/DMOD_VOC_DEC values.
07/17/95   rdh     Added I invert and pctl PUNC sym siz choices for MSM 2p.
04/26/95   rdh     Updated comments on finger time tracking settings.
02/17/95   jah     Added PDM1, PDM2, and new interrupt choices for MSM 2p.
12/22/93   jah     Added macro DMOD_STROBEM() for strobing a value
09/07/93   ptw     Added PCC register bit DMOD_PC_PMODE.
06/09/93   jca     Added DMOD_INT_STATUS_2.
05/02/93   ptw     Changed to new MSM conditionals.
03/15/93   ptw     Added target.h, Demod vs. MSM conditionals
02/18/93   ptw     Corrected PCC register TX_GAIN polarity and enable bits.
02/04/93   jai     Changed comments for DMOD_GAIN values.
12/30/92    gb     Added Tristate enables and polarity for TRK_LO.
                   Added port definitions for PA_VGG_VTL and TX_GAIN_COMP.
10/21/92   jah     Added M/N counter control for FM clock and Baud clock.
                   Added TXCO block tri-state control values.
07/27/92   jah     Changed to DMOD_KEY_READ_IN for BIO, added includes
07/16/92   ptw     Various changes for DMSS integration
07/02/91   rdb     Corrected DMOD_GAIN constants from 1,2,4,8 to 2,4,8,32
06/25/91   rdb     Added definition for DMOD_POS_LATCH port.
06/02/91   rdb     Minor revs.  Added bit to port 0x38 to enable/disable
                   puncturing.
05/06/91   rdb     Fixed a few comments.
03/28/91   rdb     Initial release.
11/05/90   rdb     First cut of file.

===========================================================================*/

#include "processor.h"
#include "comdef.h"
#include "target.h"
#include "msm.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                          Write Registers
---------------------------------------------------------------------------*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Searcher Control Register */

                                /* MSM23 specific bit */
#define DMOD_SRCH_SPEED_M  0x20 /* Mask to select searcher speed */
#define DMOD_SPEED_8X_V    0x20 /* 8X mode searcher */
#define DMOD_SPEED_1X_V    0x00 /* 1X mode searcher */

                                /* MSM23 specific bit */
#define DMOD_DMA_CTL_M     0x10 /* Mask to enable/disable DMA */
#define DMOD_DMA_DISABLE_V 0x10 /* Value to disable DMA */
#define DMOD_DMA_ENABLE_V  0x00 /* Value to enable DMA */

#define DMOD_DUMP_M   0x08      /* Mask to trigger a status dump */
#define DMOD_DUMP_V   0x08      /* Value to trigger a status dump */

                                /* Deleted in MSM23 */
#define DMOD_DMA_ALL_M 0x04     /* Mask for DMA all field */
#define DMOD_DMA_ALL_V 0x04     /* DMA all value */

#define DMOD_GAIN_M     0x03  /* Mask for Gain factor */
#define DMOD_GAIN_2_V   0x00  /* Value for Gain of 2/32 =  .0625 */
#define DMOD_GAIN_4_V   0x01  /* Value for Gain of 4/32 =  .125  */
#define DMOD_GAIN_8_V   0x02  /* Value for Gain of 8/32 =  .25   */
#define DMOD_GAIN_16_V  0x03  /* Value for Gain of 16/32 = .5    */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Searcher Coherent Accumulator Cycles Register */

#define DMOD_PASS_M   0x07  /* Mask for number of energy values acculated
                               by second acculator */

#define DMOD_1_PASS_V 0x00  /* 1 pass */
#define DMOD_2_PASS_V 0x01  /* 2 passes */
#define DMOD_3_PASS_V 0x02  /* 3 passes */
#define DMOD_4_PASS_V 0x03  /* 4 passes */
#define DMOD_5_PASS_V 0x04  /* 5 passes */
#define DMOD_6_PASS_V 0x05  /* 6 passes */
#define DMOD_7_PASS_V 0x06  /* 7 passes */
#define DMOD_8_PASS_V 0x07  /* 8 passes */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Symbol Combiner Power Control Cell Enables Registers */

/* Register for Finger 0*/

#define DMOD_PC_F0_M         0x07  /* Mask for Cell selection */
#define DMOD_PC_F0_CELL_A_V  0x01  /* Select Cell a */
#define DMOD_PC_F0_CELL_B_V  0x02  /* Select Cell b */
#define DMOD_PC_F0_CELL_C_V  0x04  /* Select Cell c */

/* Register for Finger 1 */

#define DMOD_PC_F1_M         0x07  /* Mask for Cell selection */
#define DMOD_PC_F1_CELL_A_V  0x01  /* Select Cell a */
#define DMOD_PC_F1_CELL_B_V  0x02  /* Select Cell b */
#define DMOD_PC_F1_CELL_C_V  0x04  /* Select Cell c */

/* Register for Finger 2 */

#define DMOD_PC_F2_M         0x07  /* Mask for Cell selection */
#define DMOD_PC_F2_CELL_A_V  0x01  /* Select Cell a */
#define DMOD_PC_F2_CELL_B_V  0x02  /* Select Cell b */
#define DMOD_PC_F2_CELL_C_V  0x04  /* Select Cell c */

/* Register for Finger 3 (MSM23 Only) */

#define DMOD_PC_F3_M         0x07  /* Mask for Cell selection */
#define DMOD_PC_F3_CELL_A_V  0x01  /* Select Cell a */
#define DMOD_PC_F3_CELL_B_V  0x02  /* Select Cell b */
#define DMOD_PC_F3_CELL_C_V  0x04  /* Select Cell c */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Power Control Control Register */

#define DMOD_PUNC_EN_M      0x08  /* Mask to enable puncturing */
#define DMOD_PUNC_EN_V      0x08  /* Value to enable puncturing */
          
#define DMOD_PC_EN_M        0x04  /* Mask to enable Power Control */
#define DMOD_PC_EN_V        0x04  /* Value to enable Power Control */

                                  /* Deleted for MSM23 */
#define DMOD_PC_INT_M       0x03  /* Mask to enable Power Control 
                                     interrupt rate */
#define DMOD_PC_RATE_0_V    0x00  /* Rate of 1.25 ms */
#define DMOD_PC_RATE_1_V    0x01  /* Rate of 2.5 ms */
#define DMOD_PC_RATE_2_V    0x02  /* Rate of 5 ms */
#define DMOD_PC_RATE_3_V    0x03  /* Rate of 10 ms */

                                  /* Deleted for MSM23 */
#define DMOD_TXGAIN_POL_M   0x10  /* Mask to set TX_GAIN_ADJ polarity */
#define DMOD_TXGAIN_NORM_V  0x00  /* Value for normal TX_GAIN_ADJ polarity */
#define DMOD_TXGAIN_INV_V   0x10  /* Value for inverted TX_GAIN_ADJ */

                                  /* Deleted for MSM23 */
#define DMOD_TXGAIN_EN_M    0x20  /* Mask to enable TX_GAIN_ADJ */
#define DMOD_TXGAIN_EN_V    0x20  /* Value to enable TX_GAIN_ADJ */

                                  /* Deleted for MSM23 */
#define DMOD_PC_PMODE       0x40  /* Mask for power control mode bit */
#define DMOD_PC_PMODE_FIXED 0x40  /* Value for fixed power control steps */
#define DMOD_PC_PMODE_VAR   0x00  /* Value for variable step size */

                                  /* This bit takes effect immediately for
                                     MSM23. */
#define DMOD_PUNC_SYM_M     0x80  /* Mask to control Power Control 
                                     puncturing symbol count */
#define DMOD_PUNC_SYM_V     0x80  /* Puncture one symbol (14.4 mode) */
 
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Time Tracking Gain K1 Register  (MSM23 Only)  */

#define DMOD_TT_K1_12HZ_V       0x02   /* 12.5 Hz value */
#define DMOD_TT_K1_25HZ_V       0x08   /* 25 value      */
#define DMOD_TT_K1_50HZ_V       0x20   /* 50 value      */
#define DMOD_TT_K1_100HZ_V      0x7F   /* 100Hz value   */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Time Tracking Gain K2 Register  (MSM23 Only)  */

#define DMOD_TT_K2_12HZ_V       0x10   /* 12.5 Hz value */
#define DMOD_TT_K2_25HZ_V       0x20   /* 25 Hz value   */
#define DMOD_TT_K2_50HZ_V       0x40   /* 50 Hz value   */
#define DMOD_TT_K2_100HZ_V      0x7F   /* 100Hz value   */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Control Register 2 */

#define DMOD_TT_EN_M     0x01   /* Enable Finger time tracking */
#define DMOD_TT_EN_V     0x01   /* Value to enable Finger time tracking */

                                /* Deleted MSM23 */
#define DMOD_TT_GAIN_M   0x06   /* Finger Time Tracking Bandwidth mask */
#define DMOD_GAIN0_V     0x00   /* <undefined> (3-chip set: 12.5Hz) */
#define DMOD_GAIN1_V     0x02   /* 12.5 Hz     (3-chip set: 25 Hz) */
#define DMOD_GAIN2_V     0x04   /* 25 Hz       (3-chip set: 50 Hz) */
#define DMOD_GAIN3_V     0x06   /* 50 Hz       (3-chip set: 100 Hz) */

                                /* MSM23 Only*/
#define DMOD_TT_RETARD   0x08   /* Fine retard for pn gen (internal use only)*/
#define DMOD_TT_ADVANCE  0x10   /* Fine advance for pn gen (internal use only)*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Control Register 0 (Deleted in MSM23) */

#define DMOD_SYM_SIZ_M      0x03     /* Selects Finger symbol size */
#define DMOD_SYM_32_V       0x00     /* Symbo size = 32 */
#define DMOD_SYM_64_V       0x01     /* Symbol size = 64 */
#define DMOD_SYM_128_V      0x02     /* Symbol size = 128 */
#define DMOD_SYM_256_V      0x03     /* Symbol size = 256 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Control Register 1 */

                                   /* MSM23 Only */
#define DMOD_F_DISABLE_M    0x20   /* Mask to disable a finger */
#define DMOD_F_DISABLE_V    0x20   /* Value to disable a finger */
#define DMOD_F_ENABLE_V     0x00   /* Value to enable a finger */

                                   /* Deleted in MSM23 */
#define DMOD_F_FILTER_M     0x10   /* Mask to set pilot filter */
#define DMOD_F_FILT_512_V   0x00   /* Value for pilot filter N=512 */
#define DMOD_F_FILT_256_V   0x10   /* Value for pilot filter N=256 */

#define DMOD_F_LOCK_M       0x04   /* Mask to Force Lock  */
#define DMOD_F_LOCK_V       0x04   /* Value to Force Lock */

#define DMOD_F_UNLOCK_M     0x02   /* Mask force Unlock */
#define DMOD_F_UNLOCK_V     0x02   /* Value to force Unlock (Was DMOD_LOCK_RES_V) */

#define DMOD_LOCK_ENA_M     0x01   /* Mask to Enable Lock */
#define DMOD_LOCK_SET_V     0x01   /* Value to Enable Lock */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Pilot Filter Gain Register (MSM23 Only) */

#define DMOD_PILOT_GAIN_382_HZ_V    0x10   /* 382Hz BW for 800Mhz */
#define DMOD_PILOT_GAIN_764_HZ_V    0x20   /* 764Hz BW for PCS */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Frequency Combiner Gain Register (MSM23 Only)  */

#define DMOD_FC_GAIN_ACQ_V  0x20  /* Loop Gain Value for acquisition */
#define DMOD_FC_GAIN_TRK_V  0x08  /* Loop Gain Value for tracking */

 
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Frequency Combiner Control Register */

                                 /* MSM23 Only */
#define DMOD_TRK_MODE_M  0x60    /* Mask to set TRK_LO_ADJ cross procuct */
#define DMOD_TRK_TRK_V   0x00    /* Tracking.  Compatible with MSM22 */
#define DMOD_TRK_CEL_V   0x20    /* Cellular acquisition */
#define DMOD_TRK_PCS_V   0x40    /* PCS acquisition */

#define DMOD_TRK_TSEN_M   0x10   /* Mask to enable TS freq tracking  */
#define DMOD_TRK_TSEN_V   0x10   /* Value to enable TS frequency tracking */

#define DMOD_TRK_LO_POL_M  0x08  /* Mask to specify polarity */
#define DMOD_TRK_LO_POL_V  0x08  /* Value to specify inverted polarity */

#define DMOD_TRK_ENA_M   0x04    /* Mask to enable frequency tracking  */
#define DMOD_TRK_ENA_V   0x04    /* Value to enable frequency tracking */

#define DMOD_PDM_OUT_M   0x02    /* Mask to disable inner frequency loop */
#define DMOD_PDM_OUT_V   0x02    /* Value to disabl inner frequency loop */

                                 /* Deleted MSM23 */
#define DMOD_ACK_TRK_M   0x01    /* Mask for acquisition or tracking mode */
#define DMOD_ACK_TRK_V   0x01    /* Value for mode 0 = Acquisition,
                                    1= Tracking */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Symbol Combiner Control Register 0 */

#define DMOD_SC_F_EN_M      0x1E   /* Mask to enable fingers 0..3 MSM23 
                                    * Only. Extended from 0x0E in MSM22
                                    */
#define DMOD_F0_EN_V        0x02   /* Value to enable finger 0 */
#define DMOD_F1_EN_V        0x04   /* Value to enable finger 1 */
#define DMOD_F2_EN_V        0x08   /* Value to enable finger 2 */

                                   /* MSM23 Only */
#define DMOD_F3_EN_V        0x10   /* Value to enable finger 3 */
#define DMOD_EN_TT_CNTR_M   0x20   /* Internal use bit MSM23. */
#define DMOD_EN_TT_CNTR_V   0x20   

#define DMOD_SLAM_M         0x01   /* SLAM Mask */
#define DMOD_SLAM_V         0x01   /* Slam Register */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Symbol Combiner Control Register 1 */

                                  /* MSM23 Only */
#define DMOD_SC_SR_M        0x02  /* Symbol Combiner Symbol Rate mask*/
#define DMOD_SC_SR_64_CS_V  0x00  /* Value for 64 Chips per Symbol (19.2ksps)*/
#define DMOD_SC_SR_256_CS_V 0x02  /* Value for 256 Chips per Symbol(4.8ksps) */

#define DMOD_80M_M         0x0C  /* Mask to set phase of Sync 80m */
#define DMOD_PN3_V         0x00  /* Sync 80m at third PN roll */
#define DMOD_PN2_V         0x04  /* Sync 80m at second PN roll */
#define DMOD_PNN_V         0x08  /* Sync 80m at next PN roll */
#define DMOD_NO_V          0x0C  /* Do nothing */

#define DMOD_DDS_ENA_M     0x10  /* Mask to enable DDS */
#define DMOD_DDS_ENA_V     0x10  /* Value to enable DDS */

                                 /* MSM23 bits Only */
#define DMOD_COMB_DIS_M    0x20  /* Mask to disable symbol combining */
#define DMOD_COMB_DIS_V    0x20  /* Value to disable symbol combining */
#define DMOD_RSSI_EXT_M    0x40  /* Mask to enable RSSI extended range */
#define DMOD_RSSI_EXT_V    0x40  /* Value to enable RSSI extended range */
#define DMOD_IQCAP_ENA_M   0x80  /* Mask to enable IQ symbol capture */
#define DMOD_IQCAP_ENA_V   0x80  /* Value to enable IQ symbol capture */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Demod Control Register */

#ifdef T_MSM_5100
#error code not present
#endif /* T_MSM_5100 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* AGC Reference PDM DAC Data register */

#define DMOD_AGC_PDM_FM_V     0x7F    /* Set AGC to minimum for FM mode */
#define DMOD_AGC_PDM_CDMA_V   0x00    /* Preset AGC to 0 at start of CDMA */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* AGC Data Control Register */

                                   /* Deleted in MSM23 */
#define DMOD_DC_PDM_IPOL_M   0x04  /* Mask to change polarity of I_OFFSET */
#define DMOD_DC_PDM_IPOL_V   0x04  /* Value for inverted polarity */

#define DMOD_I_OFF_EN_M      0x02  /* Mask for "I OFF Acc EN" */
#define DMOD_I_OFF_EN_V      0x02  /* Value for "I OFF Acc EN" */

#define DMOD_Q_OFF_EN_M      0x01  /* Mask for "Q Offset Acc EN" */
#define DMOD_Q_OFF_EN_V      0x01  /* Value for "Q Offset Acc EN" */

#define DMOD_DC_PDM_POL_M    0x08  /* Mask to change polarity of I_OFFSET
                                      and Q_OFFSET PDM DACs */
#define DMOD_DC_PDM_NORM_V   0x00  /* Value for normal polarity */
#define DMOD_DC_PDM_INV_V    0x08  /* Value for inverted polarity */

                                   /* Deleted in MSM23 */
#define DMOD_AGCREF_POL_M    0x10  /* Mask to change polarity of AGC
                                      reference PDM DAC */
#define DMOD_AGCREF_NORM_V   0x00  /* Value for normal polarity */
#define DMOD_AGCREF_INV_V    0x10  /* Value for inverted polarity */

#define DMOD_AGC_EN_M        0xA0  /* Mask to enable/disable AGC PDM DACs */
#define DMOD_AGC_EN_V        0xA0  /* Value to enable AGC PDM DACs */
/* The offset gain in the IFR3000 has been increased by 4, therefore
   set bit 7 to enable the 1/4 gain factor. */

#define DMOD_Q_C_OFF_POL_M   0x40  /* Mask for Q offset inversion */
#define DMOD_Q_C_NORM_V      0x00  /* Value for normal polarity   */
#define DMOD_Q_C_INV_V       0x40  /* Value for inverted polarity */

                                   /* MSM23 Only */
#define DMOD_DC_LOOP_G_M     0x80  /* Mask for DC loop gain of I, Q loops */
#define DMOD_DC_G_NORM_V     0x00  /* Gain constant compatible with MSM 2.2 */
#define DMOD_DC_G_LESS_V     0x80  /* Gain constant 1/4 that of MSM 2.2 */

/*--------------------------------------------------------------------------
 Clock Control: TCXO Block control, PDM1/2 enable and polarity.  PDM1 and 
 PDM2 enable bits are unchanged for MSM2.3, PDM1 and PDM2 polarity are new.
---------------------------------------------------------------------------*/
/* New bits (4 and 5) for MSM2.3. 7-6 are reserved*/
#define DMOD_PDM1_POL_M         0x20    /* PDM1 Polarity */
#define DMOD_PDM1_POL_INV_V     0x20    /* Inverted */
#define DMOD_PDM1_POL_NOR_V     0x00    /* Normal */

#define DMOD_PDM2_POL_M         0x10    /* PDM2 Polarity */
#define DMOD_PDM2_POL_INV_V     0x10    /* Inverted */
#define DMOD_PDM2_POL_NOR_V     0x00    /* Normal */

/* Bits 2 and 3 below are deleted from MSM2.3 0x02, 0x04 */
/* TCXO block tri-state enable */

#define DMOD_BAUD_CLK_EN_M      0x08    /* Baud Clock       */
#define DMOD_BAUD_CLK_EN_V      0x08
#define DMOD_BAUD_CLK_DIS_V     0x00

#define DMOD_FM_CLK_EN_M        0x04    /* FM Clock         */
#define DMOD_FM_CLK_EN_V        0x04
#define DMOD_FM_CLK_DIS_V       0x00

#define DMOD_PDM1_EN_M          0x02    /* PDM1 */
#define DMOD_PDM1_EN_V          0x02
#define DMOD_PDM1_DIS_V         0x00

#define DMOD_PDM2_EN_M          0x01    /* PDM2 */
#define DMOD_PDM2_EN_V          0x01
#define DMOD_PDM2_DIS_V         0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Selection Register */

/* NOTE:  Usually these masks WOULD NOT be used because when writing a
   value to this register you WOULD NOT want to save the previous bits
   that were set (ie continue to select the previous finger while
   selecting a new figure).  SEE DMOD_F_MOUT and DMOD_F_OUT macros */
   
   
#define DMOD_F0_SEL_M         0x01  /* Mask to enable finger 0 control
                                       registers */
#define DMOD_F0_SEL_V         0x01  /* Value to enable finger 0 control
                                       registers */

#define DMOD_F1_SEL_M         0x02  /* Mask to enable finger 1 control
                                       registers */
#define DMOD_F1_SEL_V         0x02  /* Value to enable finger 1 control
                                       registers */

#define DMOD_F2_SEL_M         0x04  /* Mask to enable finger 2 control
                                       registers */
#define DMOD_F2_SEL_V         0x04  /* Value to enable finger 2 control
                                       registers */
#define DMOD_F3_SEL_M         0x08  /* Mask to enable finger 3 control
                                       registers (MSM23 Only) */
#define DMOD_F3_SEL_V         0x08  /* Value to enable finger 3 control
                                       registers (MSM23 Only) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Interrupt Output Polarity register */

#define DMOD_GP_I_POL0_M       0x01  /* Mask for GPIO interrupt 0 polarity */
#define DMOD_GP_I_POL0_V       0x01  /* Value for GPIO interrupt 0 polarity */
#define DMOD_GP_I_POL0_INV_V   0x00  /* Value for GPIO interrupt 0 Inverted
                                     ** polarity */

#define DMOD_GP_I_POL1_M       0x02  /* Mask for GPIO interrupt 1 polarity */
#define DMOD_GP_I_POL1_V       0x02  /* Mask for GPIO interrupt 1 polarity */
#define DMOD_GP_I_POL1_INV_V   0x00  /* Value for GPIO interrupt 1 invered  
                                     ** polarity */
                                     
#define DMOD_GP_I_POL2_M       0x04  /* Mask for GPIO interrupt 2 polarity */
#define DMOD_GP_I_POL2_V       0x04  /* Value for GPIO interrupt 2 polarity */
#define DMOD_GP_I_POL2_INV_V   0x00  /* Value for GPIO interrupt 2 invered  
                                     ** polarity */

#define DMOD_GP_I_POL3_M       0x08  /* Mask for GPIO interrupt 3 polarity */
#define DMOD_GP_I_POL3_V       0x08  /* Value for GPIO interrupt 3 polarity */
#define DMOD_GP_I_POL3_INV_V   0x00  /* Value for GPIO interrupt 3 invered  
                                     ** polarity */
                                     
#define DMOD_GP_I_POL4_M       0x10  /* Mask for GPIO interrupt 4 polarity */
#define DMOD_GP_I_POL4_V       0x10  /* Value for GPIO interrupt 4 polarity */
#define DMOD_GP_I_POL4_INV_V   0x00  /* Value for GPIO interrupt 4 invered  
                                     ** polarity */
                                     
#define DMOD_I_OUT_POL_M       0x20  /* Mask for Interrupt output polarity */
#define DMOD_I_OUT_POL_V       0x20  /* Value for Interrupt output polarity */

#define DMOD_SLP_I_OUT_POL_M   0x40  /* ??? */
#define DMOD_SLP_I_OUT_POL_V   0x40  /* ??? */

#if defined(T_MSM5105)
#error code not present
#else
#define DMOD_RX_DATA_INT_POL_M      0x20  /* Mask for RX Data Interrupt 
                                             polarity */
#define DMOD_RX_DATA_INT_POL_V      0x20  /* Value for RX Data Interrupt 
                                             polarity*/
#define DMOD_RX_DATA_INT_POL_INV_V  0x00  /* Value for RX Data Interrupt 
                                             inverted polarity */
                                           
#define DMOD_RX_DATA2_INT_POL_M     0x40  /* Mask for RX Data2 Interrupt 
                                             polarity */
#define DMOD_RX_DATA2_INT_POL_V     0x40  /* Value for RX Data2 Interrupt 
                                             polarity */
#define DMOD_RX_DATA2_INT_POL_INV_V 0x00  /* Value for RX Data2 Interrupt
                                             inverted polarity */
#endif
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  Definitions for trampm3.c */

/* 1st interrupt register */

#define DMOD_GP_TIMER_M       0x0001  /* Mask for GP TIMER interrupt */
#define DMOD_GP_TIMER_V       0x0001  /* Value for GP TIMER interrupt */

#define DMOD_WAKE_M           0x0002  /* Mask for Wake Up interrupt */
#define DMOD_WAKE_V           0x0002  /* Value for Wake Up interrupt */

#define DMOD_125MS_INT_M      0x0004  /* Mask for fixed 1.25ms interrupt */
#define DMOD_125MS_INT_V      0x0004  /* Value for fixed 1.25ms interrupt */

#define DMOD_INTL_M           0x0008  /* Mask for Interleaver interrupt */
#define DMOD_INTL_V           0x0008  /* Value for Interleaver interrupt */

#define DMOD_DEC_M            0x0010  /* Mask for Decoder interrupt */
#define DMOD_DEC_V            0x0010  /* Value for Decoder interrupt */

#if defined(T_MSM5105)
#error code not present
#elif defined(T_MSM33)
#error code not present
#else

#define DMOD_VOC_DEC_M        0x0020  /* Mask for Vocoder decode interrupt  */
#define DMOD_VOC_DEC_V        0x0020  /* Value for Vocoder decode interrupt */

#define DMOD_VOC_ENC_M        0x0040  /* Mask for Vocoder encode interrupt  */
#define DMOD_VOC_ENC_V        0x0040  /* Value for Vocoder encode interrupt */
#endif

#define DMOD_SRCH_DONE_M      0x0080  /* Mask for Search Done interrupt */
#define DMOD_SRCH_DONE_V      0x0080  /* Value for Search Done interrupt*/

#define DMOD_26MS_CLR_M       0x0100  /* Mask for 26 ms interrupt */
#define DMOD_26MS_CLR_V       0x0100  /* Value for 26 ms interrupt */

#define DMOD_2_N_M            0x0200  /* Mask for 2^n * 1.25 msinterrupt TICK*/
#define DMOD_2_N_V            0x0200  /* Value for 2^n * 1.25 ms interrupt TICK*/

#define DMOD_UART_M           0x0400  /* Mask for UART int */
#define DMOD_UART_V           0x0400  /* Value for UART int */

#define DMOD_RX_WBD_M         0x0800  /* Mask FM Rx wideband data int */
#define DMOD_RX_WBD_V         0x0800  /* Value FM Rx wideband data int */

#define DMOD_TX_WBD_M         0x1000  /* Mask FM Tx wideband data int */
#define DMOD_TX_WBD_V         0x1000  /* Value FM Tx wideband data int */

#define DMOD_KEY_M            0x2000  /* Mask for Keyscan interrupt */
#define DMOD_KEY_V            0x2000  /* Value for Keyscan interrupt */

#define DMOD_AUX_PCM_M        0x4000  /* Mask for Aux PCM DIN  interrupt */
#define DMOD_AUX_PCM_V        0x4000  /* Value for AUX PCM DIN interrupt */

#if defined(T_MSM5105)
#error code not present
#elif defined(T_MSM33)
#error code not present
#else

#define DMOD_SAT_LOCK_M       0x8000  /* Mask for SAT lock interrupt  */
#define DMOD_SAT_LOCK_V       0x8000  /* Value for SAT lock interrupt */
#endif

#define DMOD_FIFO_FT_M        0x20  /* Mask for FIFO fault interrupt */
#define DMOD_FIFO_FT_V        0x20  /* Value for FIFO fault interrupt */

/*-------------------------------------------------------------------------*/
/* 2nd interrupt register */

#define DMOD_SBI_INT_M        0x0001  /* Mask for SBI interrupt */
#define DMOD_SBI_INT_V        0x0001  /* Value for SBI interrupt */

#ifdef T_MSM5105
#error code not present
#else

#define DMOD_GPIO_I0_M        0x0002  /* Mask for GPIO 0 interrupt */
#define DMOD_GPIO_I0_V        0x0002  /* Value for GPIO 0 interrupt */

#define DMOD_GPIO_I1_M        0x0004  /* Mask for GPIO 1 interrupt */
#define DMOD_GPIO_I1_V        0x0004  /* Value for GPIO 1 interrupt */

#define DMOD_GPIO_I2_M        0x0008  /* Mask for GPIO 2 interrupt */
#define DMOD_GPIO_I2_V        0x0008  /* Value for GPIO 2 interrupt */

#define DMOD_GPIO_I3_M        0x0010  /* Mask for GPIO 3 interrupt */
#define DMOD_GPIO_I3_V        0x0010  /* Value for GPIO 3 interrupt */

#define DMOD_GPIO_I4_M        0x0020  /* Mask for GPIO 4 interrupt */
#define DMOD_GPIO_I4_V        0x0020  /* Value for GPIO 4 interrupt */

#endif

#define DMOD_SLEEP_FEE_M      0x0040  /* Mask for Sleep_fee interrupt */
#define DMOD_SLEEP_FEE_V      0x0040  /* Value for Sleep_fee interrupt */
      
#define DMOD_SEARCH_DMA_M     0x0080  /* Mask for DMA Search Request interrupt */
#define DMOD_SEARCH_DMA_V     0x0080  /* Value for DMA Search Request interrupt */

#define DMOD_DP_RX_M          0x0100  /* Mask for DP RX DATA interrupt */
#define DMOD_DP_RX_V          0x0100  /* Value for DP RX DATA interrupt */

#define DMOD_UART2_M          0x0200  /* Mask for UART2 int */
#define DMOD_UART2_V          0x0200  /* Value for UART2 int */

#define DMOD_DP_RX2_M         0x0400  /* Mask for DP RX2 DATA interrupt */ 
#define DMOD_DP_RXC_V         0x0400  /* Value for DP RX2 DATA interrupt */

#if defined(T_MSM33) || defined(T_MSM5105)
#error code not present
#elif defined(T_MSM5000)

#define DMOD_SYSTIME_INT1_M   0x2000  /* Mask  for SYS TIME 1 interrupt */
#define DMOD_SYSTIME_INT1_V   0x2000  /* Value for SYS TIME 1 interrupt */

#define DMOD_RX_QP_M          0x4000  /* Mask for QPCH interrupt */
#define DMOD_RX_QP_V          0x4000  /* Value for QPCH interrupt */

#define DMOD_SYSTIME_INT2_M   0x4000  /* Mask  for SYS TIME 2 interrupt */
#define DMOD_SYSTIME_INT2_V   0x4000  /* Value for SYS TIME 2 interrupt */

#else

#define DMOD_INT_27_M         0x0800  /* Mask for INT 27 interrupt */
#define DMOD_INT_27_V         0x0800  /* Value for INT 27 interrupt */

#define DMOD_INT_28_M         0x1000  /* Mask for INT 28 interrupt */
#define DMOD_INT_28_V         0x1000  /* Value for INT 28 interrupt */

#define DMOD_INT_29_M         0x2000  /* Mask for INT 29 interrupt */
#define DMOD_INT_29_V         0x2000  /* Value for INT 29 interrupt */

#define DMOD_INT_30_M         0x4000  /* Mask for INT 30 interrupt */
#define DMOD_INT_30_V         0x4000  /* Value for INT 30 interrupt */

#define DMOD_INT_31_M         0x8000  /* Mask for INT 31 interrupt */
#define DMOD_INT_31_V         0x8000  /* Value for INT 31 interrupt */

#endif 

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Sleep Control Register */

#define DMOD_DOG_M             0x01  /* Mask to kick watch dog */
#define DMOD_DOG_V             0x01  /* Value to kick watch dog */

#define DMOD_WAKE_EN_M         0x02  /* Mask to force immediate wakeup */
#define DMOD_WAKE_EN_V         0x02  /* Value to force immediate wakeup */

#define DMOD_ASIC_SLEEP_M      0x04  /* Mask to enable sleep */
#define DMOD_ASIC_SLEEP_V      0x04  /* Value to enable sleep */

#define DMOD_RF_SLEEP_MASK_M   0x08  /* Mask to change RF_SLEEP/_MASK/ */
#define DMOD_RF_SLEEP_MASK_V   0x08  /* Value to raise RF_SLEEP/_MASK/ */

#define DMOD_AUTOKICK_ARM_M    0x10  /* Mask to enable watchdog autokicker */
#define DMOD_AUTOKICK_ARM_V    0x10  /* Value to enable watchdog autokicker */

#define DMOD_BBA_POWER_DOWN_M    0x20  /* Mask to gate all MSM output pins to BBA chip*/
#define DMOD_BBA_POWER_DOWN_V    0x20  /* Value to gate all MSM output pins to BBA chip */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Scan Control Register 0 (Deleted in MSM2.3) */

#define DMOD_LOOP_M            0x01  /* Mask to enable loop back */
#define DMOD_LOOP_V            0x01  /* Value to enable loop back */

#define DMOD_WRAP_M            0x06  /* Mask for SCAN Wrap Control */
#define DMOD_WRAP_M1_V         0x00  /* Value for Wrap Mode with 0 =>lsb */
#define DMOD_WRAP_M2_V         0x02  /* Value for Wrap Mode with 1 =>lsb */
#define DMOD_WRAP_M3_V         0x04  /* Value for Wrap mode with msb=>lsb */
#define DMOD_WRAP_M4_V         0x06  /* Value for Wrap mode with msb/=>lsb*/

#define DMOD_USE_MIC_CLCK_M    0x08  /* Mask for Use micro-clock bit */
#define DMOD_USE_MIC_CLCK_V    0x08  /* Value for Use micro-clock bit */

#define DMOD_MIC_CLCK_M        0x10  /* Mask for micro-clock bit */
#define DMOD_MIC_CLCK_V        0x10  /* Value for micro-clock bit */

#define DMOD_MIC_CLK_TCXO_M    0x20  /* Mask for micro-clock TCXO bit */
#define DMOD_MIC_CLK_TCXO_V    0x20  /* Value for micro-clock TCXO bit */
 
#define DMOD_CLOCK_M           0xC0  /* Mask for SCAN Clock Action */
#define DMOD_CLOCK_1_V         0x00  /* Value for SCAN ph 1 Disabled */
#define DMOD_CLOCK_2_V         0x40  /* Value for Load SCAN Latches */
#define DMOD_CLOCK_3_V         0x80  /* Value for Load Shift Register */
#define DMOD_CLOCK_4_V         0xC0  /* Value for Shift SCAN latches */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Scan Control Register 1 (Deleted in MSM2.3) */

#define DMOD_SCAN_MUX_M        0x0F  /* Mask for Scan MUX */
#define DMOD_SCAN_MUX_0_V      0x00  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_1_V      0x01  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_2_V      0x02  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_3_V      0x03  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_4_V      0x04  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_5_V      0x05  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_6_V      0x06  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_7_V      0x07  /* Value for scan chain 0 */
#define DMOD_SCAN_MUX_8_V      0x08  /* Value for scan chain 0 */

#define DMOD_USE_MICRO_TCXO_M  0x10  /* Mask for Use Micro Clock TCXO */
#define DMOD_USE_MICRO_TCXO_V  0x10  /* Value to use the TCXO micro clock */

#define DMOD_USE_MICRO_INLV_M  0x20  /* Mask for Use Micro Clock Interleaver */
#define DMOD_USE_MICRO_INLV_V  0x20  /* Value for Interleaver micro clock */

/*---------------------------------------------------------------------------
                          Read Registers
---------------------------------------------------------------------------*/

/* Version and Status Register */

#define DMOD_VER_M             0x7F  /* Mask for version number */

#define DMOD_EMU_RDY_M         0x80  /* Mask for Emulator Ready bit */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*  Searcher Control Register */

#define DMOD_DMA_ERR_M         0x01  /* Mask to detect DMA channel error */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Searcher Maximum Energy Index Register (MSM23 Only) */

#define DMOD_SRCH_SEL_BEST1_V  0x00
#define DMOD_SRCH_SEL_BEST2_V  0x01
#define DMOD_SRCH_SEL_BEST3_V  0x02
#define DMOD_SRCH_SEL_BEST4_V  0x03
 
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Finger Lock RSSI Register (MSM23 Only) */

#define DMOD_RSSI_GAIN_3MS_V   0x08            /* 3.3ms time constant */
#define DMOD_RSSI_GAIN_6MS_V   0x04            /* 6.6ms time constant */
#define DMOD_RSSI_GAIN_13MS_V  0x02            /* 13.2ms time constant */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Symbol Combiner Control Register 0 */

                                  /* Deleted in MSM23 */
#define DMOD_FAULT_F0_M     0x01  /* Mask for FIFO Fault Finger 0 */
#define DMOD_FAULT_F1_M     0x02  /* Mask for FIFO Fault Finger 1 */
#define DMOD_FAULT_F2_M     0x04  /* Mask for FIFO Fault Finger 2 */

#define DMOD_LOCK_F0_M      0x08  /* Mask for Lock state of Finger 0 */
#define DMOD_LOCK_F1_M      0x10  /* Mask for Lock state of Finger 1 */
#define DMOD_LOCK_F2_M      0x20  /* Mask for Lock state of Finger 2 */
#define DMOD_LOCK_F3_M      0x40  /* Mask for Lock state of Finger 3
                                     (MSM23 Only) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* General Purpose Input/Output Input Register */

#define DMOD_I_GPIO_0_M          0x01  /* Mask for GPIO 0 */
#define DMOD_I_GPIO_1_M          0x02  /* Mask for GPIO 1 */
#define DMOD_I_GPIO_2_M          0x04  /* Mask for GPIO 2 */
#define DMOD_I_GPIO_3_M          0x08  /* Mask for GPIO 3 */
#define DMOD_I_GPIO_4_M          0x10  /* Mask for GPIO 4 */
#define DMOD_I_GPIO_5_M          0x20  /* Mask for GPIO 5 */

/* Note - for Masks see General Purpose Input/Output Interrupt Out Masks */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Keysense Read Register */

#define DMOD_KEY_0_M             0x01  /* Mask for Key 0 */
#define DMOD_KEY_1_M             0x02  /* Mask for Key 1 */
#define DMOD_KEY_2_M             0x04  /* Mask for Key 2 */
#define DMOD_KEY_3_M             0x08  /* Mask for Key 3 */
#define DMOD_KEY_4_M             0x10  /* Mask for Key 4 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#ifdef T_MSM5000

/*  MAC Engine Registers */

/* Rounding Factor for SCH Symbol Combining */   /* Spreading rate  */
#define  DMOD_COMB_RND1_128_64_CHIPS_V  0x08     /* 128 or 64 chips */
#define  DMOD_COMB_RND1_32_16_CHIPS_V   0x07     /* 32  or 16 chips */
#define  DMOD_COMB_RND1_8_4_CHIPS_V     0x06     /* 8   or 4  chips */

/* Rounding Factor for SCH Energy Estimate */    /* Spreading rate  */
#define  DMOD_COMB_RND2_128_64_CHIPS_V  0x00     /* 128 or 64 chips */
#define  DMOD_COMB_RND2_32_16_CHIPS_V   0x10     /* 32  or 16 chips */
#define  DMOD_COMB_RND2_8_4_CHIPS_V     0x20     /* 8   or 4  chips */

/* Rounding Factor for SCH Soft Decision */
#define DMOD_COMB_RND_CONVOLUTIONAL     0x0800  /* Convolutional Coding */
#define DMOD_COMB_RND_TURBO             0x0700  /* Turbo Coding */

/* Control for the Spreading Rate for Demodulator Channel 2 */
#define SPR_128_CHIP_V   0x1f     /* Spreading rate = 128 chips */
#define SPR_64_CHIP_V    0x0f     /* Spreading rate = 64  chips */
#define SPR_32_CHIP_V    0x07     /* Spreading rate = 32  chips */
#define SPR_16_CHIP_V    0x03     /* Spreading rate = 16  chips */
#define SPR_8_CHIP_V     0x01     /* Spreading rate = 8   chips */
#define SPR_4_CHIP_V     0x00     /* Spreading rate = 4   chips */

#endif /* T_MSM5000 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/


/*---------------------------------------------------------------------------
                            MACRO DMOD_OUT

This macro outputs a specified value to a specified port. All 8 bits of the
port are written (i.e., the port is not buffered by software). This macro
does not set the finger page port, it MUST ONLY BE USED FOR PORTS WHICH ARE
NOT RELATED TO FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_OUT( io, val ) MSM_OUT( io, val )

/*---------------------------------------------------------------------------
                      MACRO DMOD_F_OUT

This macro sets up the finger page and then outputs a specified value to a 
specified port. All 8 bits of the port are written (i.e., the port is not 
buffered by software). Since this macro sets the finger page port, it 
MUST ONLY BE USED FOR PORTS WHICH ARE RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define DMOD_F_OUT( finger, io, val ) \
    INTLOCK( ); \
    (void) outp( DMOD_F_PAGE, (1<<(finger)) );\
    (void) outp( io, (int) val); \
    INTFREE( )


/*---------------------------------------------------------------------------
                           MACRO DMOD_STROBEM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. The current
mask is not updated, so the next write will not be effected by the value
'val'.  To do this, an image is maintained of the previous value written
to the port which is then combined with the new value.  This macro uses
the image for registers which are not related to the fingers so IT MUST
ONLY BE USED FOR WRITING TO PORTS WHICH ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_STROBEM( io, mask, val ) MSM_STROBEM( io, mask, val)

/*---------------------------------------------------------------------------
                           MACRO DMOD_STROBEM_H

This macro outputs a specified 16-bits value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. The current
mask is not updated, so the next write will not be effected by the value
'val'.  To do this, an image is maintained of the previous value written
to the port which is then combined with the new value.  This macro uses
the image for registers which are not related to the fingers so IT MUST
ONLY BE USED FOR WRITING TO PORTS WHICH ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#ifdef T_MSM5000
#define  DMOD_STROBEM_H( io, mask, val ) MSM_STROBEM_H( io, mask, val)
#endif /* T_MSM5000 */

/*---------------------------------------------------------------------------
                           MACRO DMOD_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which 
is then combined with the new value.  This macro uses the image for 
registers which are not related to the fingers so IT MUST ONLY BE USED FOR 
WRITING TO PORTS WHICH ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define DMOD_OUTM( io, mask, val ) MSM_OUTM( io, mask, val)

/*---------------------------------------------------------------------------
                           MACRO DMOD_F_OUTM

This macro outputs a specified value, qualified by a specified mask, to
a specified port address. Only the bits corresponding to the mask are
actually affected; other bits retain their previous values. To do this,
an image is maintained of the previous value written to the port which 
is then combined with the new value.  This macro uses the image for ports
which are related to the fingers and sets up the finger page port, so 
IT MUST ONLY BE USED FOR WRITING TO PORTS WHICH ARE RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define DMOD_F_OUTM( finger, io, mask, val ) MSM_F_OUTM( finger, io, mask, val )


/*---------------------------------------------------------------------------
                            MACRO DMOD_IN

This macro reads and returns the value from a specified port. No mask is
applied to the read value. This macro MUST ONLY BE USED FOR PORTS WHICH
ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_IN( io )  MSM_IN( io )

/*---------------------------------------------------------------------------
                            MACRO DMOD_F_IN

This macro reads and returns the value from a specified port. No mask is
applied to the read value. This macro will set up the finger page port before
reading the specified port, so IT MUST ONLY BE USED FOR PORTS WHICH ARE
RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_F_IN( var, finger, io )\
  INTLOCK( );\
  (void) outp( DMOD_F_PAGE, (1<<(finger)) );\
  var = (byte) inp( io );\
  INTFREE( )


/*---------------------------------------------------------------------------
                            MACRO DMOD_INM

This macro reads the value from a specified I/O port address and then
applies a specified mask to the value. Bits not corresponding to the
mask are returned as zero.  This macro MUST ONLY BE USED FOR PORTS WHICH
ARE NOT RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_INM( io, mask )  MSM_INM( io, mask )

/*---------------------------------------------------------------------------
                         MACRO DMOD_F_INM

This macro reads the value from a specified I/O port address and then
applies a specified mask to the value. Bits not corresponding to the
mask are returned as zero.  This macro sets up the finger page port so IT
MUST ONLY BE USED FOR PORTS RELATED TO THE FINGERS.
---------------------------------------------------------------------------*/

#define  DMOD_F_INM( finger, io, mask ) \
    INTLOCK( ),\
    outp( DMOD_F_PAGE, (1<<(finger)) ),\
    ( inp( io ) & (mask) ),\
    INTFREE( );


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#endif /* DMOD_H */

