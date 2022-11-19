#ifndef RFIGPIO_H
#define RFIGPIO_H

/* EJECT  */
/*===========================================================================

                       R F  G P I O   R E G I S T E R S

DESCRIPTION
  This header file contains definitions for all GPIO registers that are used
  by the RF unit.  This header file is internal to the RF unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/rfigpio.h_v   1.1   10 Sep 1999 18:12:34   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/99   ychan   Modified for targets NGP, SURF, Q-1900.
01/06/99   ychan   Updated Copyright date.
09/10/98   kss     Renamed Q RF_GPIO_IDLE definitions to match change in 
                   BIOQ.H.
05/18/98   thh     For TGP, define PCS band select as DEC_GPIO_0.
05/06/98   kss     In the Module, FM/ and PCS_BAND_SELECT/ map to the same
                   GPIO pin.  This is now reflected in the appropriate bio
                   file rather than in this file.
03/25/98   kss     Corrected the PCS band select GPIO definition for MD.
02/24/98   jjn     Removed support for MSM2P Rev 3
02/23/98   kss     Added support for 1900MHz Module.
02/20/98   jjn     Defined RF_GPIO_TRUE_IDLE_M, RF_GPIO_TRUE_IDLE_DIS_V and
                   RF_GPIO_TRUE_IDLE_ENA_V for WLL targets.
02/09/98   kss     Added GPIO definitions for Q target.
02/03/98   kss     Added GPIO definitions for Module
12/11/97   jjn     Defined _TS_BUF, _BUF, _OUT_OP, _TSEN, and _OUT for all
                   of the generic GPIO register names so that the generic
                   names could be used along with BIO_OUT, etc, in rfmsm.c
11/12/97   jjn     Added GPIO definitions for ISS2
09/04/97   thh     Initial version

===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/

#include "rficap.h"     /* RF capability       */
#include "bio.h"        /* Basic I/O interface */

/* EJECT  */
/*===========================================================================

                        PUBLIC DATA DECLARATIONS

===========================================================================*/

#if ((TG==T_O) || (TG==T_PC))
#error code not present
#elif (TG==T_Q)
#error code not present
#elif (TG==T_G)                               

/* --------------------------------------------------------------------------
                        SYNTHESIZER CONTROL SIGNALS
-------------------------------------------------------------------------- */
#define RF_GPIO_PLL_R           ENC_GPIO_0

#define RF_GPIO_PLL_R_TS_BUF    ENC_GPIO_0_TS_BUF
#define RF_GPIO_PLL_R_BUF       ENC_GPIO_0_BUF
#define RF_GPIO_PLL_R_OUT_OP    ENC_GPIO_0_OUT_OP
#define RF_GPIO_PLL_R_TSEN      ENC_GPIO_0_TSEN
#define RF_GPIO_PLL_R_OUT       ENC_GPIO_0_OUT

#define RF_GPIO_PLLDATA_M       BIO_PLLDATA_M       /* PLL data output     */
#define RF_GPIO_PLLDATA_HI_V    BIO_PLLDATA_HI_V
#define RF_GPIO_PLLDATA_LO_V    BIO_PLLDATA_LO_V

#define RF_GPIO_PLLCLOCK_M      BIO_PLLCLOCK_M      /* PLL Clock           */
#define RF_GPIO_PLLCLOCK_HI_V   BIO_PLLCLOCK_HI_V
#define RF_GPIO_PLLCLOCK_LO_V   BIO_PLLCLOCK_LO_V

#define RF_GPIO_PLLENABLE_M     BIO_PLLENABLE_M     /* PLL Enable / Latch  */
#define RF_GPIO_PLLENABLE_V     BIO_PLLENABLE_V
#define RF_GPIO_PLLDISABLE_V    BIO_PLLDISABLE_V

/* --------------------------------------------------------------------------
                             ADC CONTROL SIGNAL
-------------------------------------------------------------------------- */
#define RF_GPIO_ADC_MUX_R       ENC_GPIO_0

#define RF_GPIO_ADC_MUX_R_TS_BUF ENC_GPIO_0_TS_BUF
#define RF_GPIO_ADC_MUX_R_BUF    ENC_GPIO_0_BUF
#define RF_GPIO_ADC_MUX_R_OUT_OP ENC_GPIO_0_OUT_OP
#define RF_GPIO_ADC_MUX_R_TSEN   ENC_GPIO_0_TSEN
#define RF_GPIO_ADC_MUX_R_OUT    ENC_GPIO_0_OUT

#define RF_GPIO_ADC_MUX_M       BIO_ADC_MUX_M
#define RF_GPIO_ADC_MUX_0_V     BIO_ADC_MUX_0_V
#define RF_GPIO_ADC_MUX_1_V     BIO_ADC_MUX_1_V
#define RF_GPIO_ADC_MUX_2_V     BIO_ADC_MUX_2_V
#define RF_GPIO_ADC_MUX_3_V     BIO_ADC_MUX_3_V
#define RF_GPIO_ADC_MUX_4_V     BIO_ADC_MUX_4_V
#define RF_GPIO_ADC_MUX_5_V     BIO_ADC_MUX_5_V
#define RF_GPIO_ADC_MUX_6_V     BIO_ADC_MUX_6_V
#define RF_GPIO_ADC_MUX_7_V     BIO_ADC_MUX_7_V

/* --------------------------------------------------------------------------
                             FM CONTROL SIGNAL
-------------------------------------------------------------------------- */
#define RF_GPIO_FM_R            DEC_GPIO_1

#define RF_GPIO_FM_R_TS_BUF     DEC_GPIO_1_TS_BUF
#define RF_GPIO_FM_R_BUF        DEC_GPIO_1_BUF
#define RF_GPIO_FM_R_OUT_OP     DEC_GPIO_1_OUT_OP
#define RF_GPIO_FM_R_TSEN       DEC_GPIO_1_TSEN
#define RF_GPIO_FM_R_OUT        DEC_GPIO_1_OUT

#define RF_GPIO_FM_M            BIO_FM_M
#define RF_GPIO_FM_ENA_V        BIO_FM_ENA_V
#define RF_GPIO_FM_DIS_V        BIO_FM_DIS_V


/* --------------------------------------------------------------------------
                          BB2 IDLE CONTROL SIGNAL
-------------------------------------------------------------------------- */
#define RF_GPIO_BB2_IDLE_R      DEC_GPIO_1

#define RF_GPIO_BB2_IDLE_R_TS_BUF DEC_GPIO_1_TS_BUF
#define RF_GPIO_BB2_IDLE_R_BUF    DEC_GPIO_1_BUF
#define RF_GPIO_BB2_IDLE_R_OUT_OP DEC_GPIO_1_OUT_OP
#define RF_GPIO_BB2_IDLE_R_TSEN   DEC_GPIO_1_TSEN
#define RF_GPIO_BB2_IDLE_R_OUT    DEC_GPIO_1_OUT

#define RF_GPIO_BB2_IDLE_M      BIO_BB2_IDLE_M
#define RF_GPIO_BB2_IDLE_DIS_V  BIO_BB2_IDLE_DIS_V
#define RF_GPIO_BB2_IDLE_ENA_V  BIO_BB2_IDLE_ENA_V

/* --------------------------------------------------------------------------
                            FM CONTROL SIGNAL
-------------------------------------------------------------------------- */
#define RF_GPIO_IDLE_R          DEC_GPIO_1

#define RF_GPIO_IDLE_R_TS_BUF   DEC_GPIO_1_TS_BUF
#define RF_GPIO_IDLE_R_BUF      DEC_GPIO_1_BUF
#define RF_GPIO_IDLE_R_OUT_OP   DEC_GPIO_1_OUT_OP
#define RF_GPIO_IDLE_R_TSEN     DEC_GPIO_1_TSEN
#define RF_GPIO_IDLE_R_OUT      DEC_GPIO_1_OUT

#define RF_GPIO_IDLE_M          BIO_IDLE_M
#define RF_GPIO_IDLE_DIS_V      BIO_IDLE_DIS_V
#define RF_GPIO_IDLE_ENA_V      BIO_IDLE_ENA_V

/* --------------------------------------------------------------------------
                       PCS BAND SELECT CONTROL SIGNAL
-------------------------------------------------------------------------- */
#define RF_GPIO_PCS_BAND_SEL_R  DEC_GPIO_1

#define RF_GPIO_PCS_BAND_SEL_R_TS_BUF  DEC_GPIO_0_TS_BUF
#define RF_GPIO_PCS_BAND_SEL_R_BUF     DEC_GPIO_0_BUF
#define RF_GPIO_PCS_BAND_SEL_R_OUT_OP  DEC_GPIO_0_OUT_OP
#define RF_GPIO_PCS_BAND_SEL_R_TSEN    DEC_GPIO_0_TSEN
#define RF_GPIO_PCS_BAND_SEL_R_OUT     DEC_GPIO_0_OUT

#define RF_GPIO_BAND_SEL_M      BIO_FM_M
#define RF_GPIO_BAND_SEL_HI_V   BIO_FM_ENA_V
#define RF_GPIO_BAND_SEL_LO_V   BIO_FM_DIS_V

/* ----------------------------------------------------------------------
** PCS control bit definitions for rf_pcs_ctl byte.  Each bit represents
** one control bit in a shift/latch register.  SYNTH_DATA and SYNTH_CLOCK
** are used to clock data into the register.  FM/ is strobed to transfer
** the data to the output latch glitchlessly.
** ---------------------------------------------------------------------- */
#define RF_GPIO_PCS_BAND_SEL_M    0x01 /* PCS band select mask */
#define RF_GPIO_PCS_BAND_SEL_HI_V 0x01 /* PCS high band select */
#define RF_GPIO_PCS_BAND_SEL_LO_V 0x00 /* PCS low band select */

#define RF_GPIO_PCS_CTL_MSB       0x80 /* PCS control byte MSB */

#define RF_GPIO_TOTAL_PCS_CTL_BITS   8 /* size of shift register */

#elif (TG==T_T)
#error code not present
#elif (TG==T_MD)
#error code not present
#elif (TG==T_I2)
#error code not present
#else
#error code not present
#endif

#endif  /* RFIGPIO_H */

