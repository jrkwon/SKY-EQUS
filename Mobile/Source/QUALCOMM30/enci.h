#ifndef ENCI_H
#define ENCI_H
/*===========================================================================

      E N C O D E R   I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
   All the declarations and definitions necessary for the Encoder driver to
   interact with the QUALCOMM CDMA Interleaver / Encoder chip set

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999             by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/enci.h_v   1.14   14 Jul 2000 14:24:14   sanjayr  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/14/00   sr      Removed ENC_NO_GATING_V. This is replaced by 
                   ENC_NO_GATING_PATTERN in enc.h
05/18/00   sr      Changed ENC_NO_GATING_V to 0xFFFF from 0xFF
05/08/00   sr      Added support for MSM5000B
02/08/00   bgc     Added FEATURE_PLT_RF to control the BTF.
11/19/99   sr      Changes to support Supplemental Channels
11/04/99   lcc     Changed BTF values to the optimized value of 0xc80.
10/07/99   lcc     Merged from PLT 2.0.
09/23/99   lcc     Merged in MSM5000and PLT support from PLT archive.
11/05/98   dlb     Added support for MSM3000.
08/14/98   aks     Adjusted PA warmup time for QCP-1960 to 156uSec.
07/13/98   aks     Adjusted the PA warmup time for 5GP.  QCP-860 is 150uSec
                   and QCP-1960 is 299uSec.
06/30/98   aks     Shifted BTF for QCP-1960 by 0.6 uSec.
03/19/98   aks     Configured for Odie (T_O).
02/05/98   kss     Adjusted ENC_INIT_BTF_* for Module.
12/10/97   aks     Adjusted Q-800 BTF value, brought closer to 0.0us.
12/03/97   mk      Removed ENC_TEST_POINT_SEL, defined in deci.h.
11/24/97   aks     Adjusted TGP BTF value for cellular band.  Brought MSM22
                   closer to zero so that MSM23 does not exceed standards.
06/17/97   dhh     Added target T_Q for Q Phone.
06/16/97   aks     Added CRC disable.  Added ENC_TEST_POINT_SEL.  Changes are
                   related to MSM2.3.
03/14/97   jjn     Configured for Module (T_MD)
01/23/97   jah     BTF looks good, changed TGP PA_WARMUP to 200us.
11/02/96   jah     Configured for TGP (T_T), BTF still needs tuning.
02/07/96   jah     Added ENC_INIT_BTF_* for QCP-1000
01/10/96   jah     Changed ENC_INIT_BTF_PCS for QCP-1900 to 4,224 CHIPX8's
12/22/95   jah     Added ENC_INIT_BTF_PCS, for PCS BTF configuration.
10/15/95   jah     Updated Gemini PA_WARMUP to 638us to track GDAC change.
10/10/95   fkm     Minor Macro Changes to save ROM.
09/28/95   jah     Set Gemini PA_WARMUP to 600us to track GDAC change.
08/25/95   jah     Set Gemini & Beta-2 BTF values per measurement
08/24/95   jah     Set Gemini & ISS-2 PA_WARMUP to 100us
03/20/95   jah     Added support for 14400 rate set and MSM2p encoder mods.
06/28/94   jah     Added 1st guess PA_WARMUP values for ISS-2 and CSS-1
06/28/94   jah     Added PA_WARMUP values for Gemini
05/31/94   jah     Added PA_WARMUP values for ISS1.
05/05/93   jah     Added ENC_MASK_DATA_M
05/03/93   jah     Added ENC_MASK_DATA for T_MSM.
04/26/93   jah     Added changes for Beta II mobile.
04/17/93   jah     Set the mobile PA_WARMUP to 150us after Clarence/Todd/Matt's
                   measurements.  Portable set to 100us, to only allow for
                   tolerances spec'd by Mike/Ana.
03/09/93   jah     Changed ENC_PA_WARMUP_V and ENC_PCN_PA_WARMUP_V for the
                   portable/mobile/PCN RAS PA warm-up capability.
07/27/92   jah     Made enc_image[] private
01/30/92   jah     Ported module from brassboard enc.h

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "processor.h"  /* Definition of chip selects           */
#include "target.h"     /* Target specific definitions          */

#ifdef T_MSM3
#include "msm.h"                /* Register alias for MSM3000. */
#endif

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* CHIPX8 delay for SYNC80M, biased +25, set by calling enc_set_btf()
** Value for the back-to-the-future counter for Init and CDMA mode
*/
#if defined(FEATURE_PLT) && !defined(FEATURE_PLT_RF)
#error code not present
#elif defined(T_MSM5000) 

// by chos for chipx8 delay, tasik 00.11.27
#if ( MODEL_ID == MODEL_A10 )
//#define ENC_INIT_BTF     ((dword)0x00000C7BL)
#define ENC_INIT_BTF     ((dword)0x00000C7FL)
#elif ( MODEL_ID == MODEL_B1 )
#define ENC_INIT_BTF     ((dword)0x00000C7CL)	// tx time offset 수정 by jnlee 00.09.26
#elif ( MODEL_ID == MODEL_B2 )
#define ENC_INIT_BTF     ((dword)0x00000C79L)	// tx time offset 수정 by jnlee 01.07.27
#else	// #if ( MODEL_ID == )
#define ENC_INIT_BTF     ((dword)0x00000C80L)
#endif	// #if ( MODEL_ID == )

#define ENC_INIT_BTF_PCS ((dword)0x00000C80L)
#elif (TG==T_G)
#define ENC_INIT_BTF     ((dword)0x00001093L) /* 01093 = 4,243 CHIPX8's    */
#define ENC_INIT_BTF_PCS ((dword)0x00001080L) /* 01080 = 4,224 CHIPX8's    */
#elif (TG==T_Q)
#error code not present
#else
#error code not present
#endif

/* Initial Transmit Frame interrupt is offset from the frame boundary by
** this offset in power control groups and walsh symbols
*/
#define ENC_INIT_PCG_T_OFF      15      /* Power control group time offset */
#define ENC_INIT_WS_T_OFF       0       /* Walsh symbol time offset        */

/* Variables for handling the time it takes to boot the emulator
*/
#define ENC_CHK_BOOT    5       /* Check if emulator booted every N ticks    */

#define ENC_BOOT_TOUT   (3000 / ENC_CHK_BOOT)   /* Timeout for emulator boot */

/* Initial I & Q PN state (set MSB of 15 bit register)
*/
#define ENC_PN_INIT_STATE       0x4000

/*---------------------------------------------------------------------------
                          Write Registers
----------------------------------------------------------------------------*/

#define ENC_U_PN_M        0x03
  /* Mask to get/set bozo bit ENC_U_PN_S5 */
#define ENC_BOZO_M        0x80
#define ENC_BOZO_V        0x80
#define ENC_NO_BOZO_V     0x00

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* Warmup delay for the chip, time turned on before data is transmitted */

/* Time the PA needs to "warm up" before transmitting a power control
** groups worth of bits.  The equation is:
**
** PA_WARMUP_V = (153.6 * DELAY) + 64
**
** for a "warm up" time of (.15 + 10%) .15 ms = 0t87 = 0x57
**                                     .10 ms = 0t80 = 0x50
*/
#if (TG==T_G)

#define ENC_PA_WARMUP_V         0xA2    /* .638 ms */
#define ENC_PCN_PA_WARMUP_V     0xA2    /* .638 ms */

#elif (TG==T_Q)
#error code not present
#elif (TG==T_O)
#error code not present
#else

#error unsupported target, need to compute PA_WARMUP time

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Specfy when WSYM_CLK will fire */

#define ENC_WSYM_WHEN_M   0xFC
  /* State of 6-bit Walsh Chip counter to fire at */
#define ENC_WSYM_DELAY_M  0x03
  /* Number of PN chips to delay before firing WSYM_CLK */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Encoder Interrupt Offset Register - specify the number of 104.2 us periods
   needed to load the encoder memory with the next frame's data from the time
   TX_FR_INT fires to when the encoder may process the data.  In other words,
   they specify the length of time that TX_FR_INT will fire before firing the
   internal SYNC_ENC signal */

#define ENC_POWER_ST_M    0x78            /* Units of power-control groups */
#define ENC_WALSH_ST_M    0x07            /* Units of Walsh Symbols */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define ENC_RATE_M        0x03            /* Rate mask */
#define ENC_RATE1_V       0x00            /* Full rate */
#define ENC_RATE2_V       0x01            /* 1/2 rate */
#define ENC_RATE3_V       0x02            /* 1/4 rate */
#define ENC_RATE4_V       0x03            /* 1/8 rate */

#ifdef T_MSM5000
   #define ENC_FCH_DATA_RATE_M         0x000C             /* data rate mask */
   #define ENC_RATE_SET_M              0x0010             /* rate set mask */
   #define ENC_FCH_CODE_RATE_M         0x0060             /* code rate mask */

   #define ENC_DCCH_GAIN_M             0xFF00
   #define ENC_PCH_GAIN_M              0x00FF
   #define ENC_SCH_GAIN_M              0xFF00
   #define ENC_FCH_GAIN_M              0x00FF

   #define ENC_TX_EARLY_FRAME_ADV_M    0x01FC
   #define ENC_TX_EARLY_FRAME_PRIOD_M  0x0002

   #define ENC_IS95_MODE_M             0x0002
   #define ENC_IS95_C_V                0x0002
   #define ENC_IS95_A_V                0x0000

   #define ENC_IS95_FCH_ON_M           0x0001
   #define ENC_IS95C_FCH_ON_V          0x0001
   #define ENC_IS95C_FCH_OFF_V         0x0000

   #define ENC_IS95_SCH_ON_M           0x0001
   #define ENC_IS95C_SCH_ON_V          0x0001
   #define ENC_IS95C_SCH_OFF_V         0x0000

   #define ENC_SCH_DATA_RATE_M         0x003E
   #define ENC_SCH_CODE_RATE_M         0x00C0

   #define ENC_PCH_M                   0x0100
   #define ENC_PCH_ON_V                0x0100
   #define ENC_PCH_OFF_V               0x0000
   #define ENC_NO_MOD_ROTATION_V       0x0000
   #define ENC_FCH_CTL_DIS_V           0x0000
   #define ENC_SCH_CTL_DIS_V           0x0000
   #define ENC_ZERO_GAIN_V             0x0000
   #define ENC_CLEAR_CRC_POLY_V        0x0000
   #define ENC_SCH_LTU_SIZE_ZERO_V     0x0000
   #define ENC_PUNC_PATTERN_NONE_V     0x0000

   #define ENC_TURBO_CODE_M            0x2000
   #define ENC_TURBO_CODE_V            0x2000
   #define ENC_CONV_CODE_V             0x0000
   #define ENC_LTU_ENA_M               0x4000
   #define ENC_LTU_ENA_V               0x4000
   #define ENC_LTU_DIS_V               0x0000
   #define ENC_LTU_MIN                 32
   #define ENC_LTU_MAX                 511

   #define ENC_SCH_0080_PUNC_PATT      0x0080 // 1/9 puncturing rate
   #define ENC_SCH_0800_PUNC_PATT      0x0800 // 1/5 puncturing rate
#endif

#ifdef T_MSM5000
   #define ENC_FCH_CRCLEN_M          0x0F00  /* CRC Length; 0 means disable */
   #define ENC_SCH_CRCLEN_M          0x1F00  /* CRC Length; 0 means disable */
#else
   #define ENC_CRCEN_M       0x04            /* CRC Enable */
   #define ENC_CRCEN_V       0x04            /* Generate and insert CRC     */
#endif

#define ENC_CRCDIS_V      0x00            /* Don't Generate and insert CRC */

#ifdef T_MSM2P
  #ifndef  T_MSM5000
    #define ENC_RATE_SET_M    0x08        /* Code Rate Select (1/2 or 1/3) */
  #endif
#define ENC_RSET_9600_V   0x00            /*  9600, rate=1/3               */
#define ENC_RSET_14400_V  0x08            /* 14400, rate=1/2               */
#endif

#define ENC_NUM_FRAC_RATES 4

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* Baud Rate Mask - 000 is 307.2 KHz, 111 is 2400 */
#define ENC_BAUD_SEL_M    0x07
#define ENC_BAUD_307_2K   0x00            /* 307.2 KHz          */
#define ENC_BAUD_153_6K   0x01            /* 153.6 KHz          */
#define ENC_BAUD_76_8K    0x02            /*  76.8 KHz          */
#define ENC_BAUD_38_4K    0x03            /*  38.4 KHz          */
#define ENC_BAUD_19_2K    0x04            /*  19.2 KHz          */
#define ENC_BAUD_9600     0x05            /*   9.6 KHz          */
#define ENC_BAUD_4800     0x06            /*   4.8 KHz          */
#define ENC_BAUD_DISABLE  0x07            /* Disable            */

#define ENC_CODEC_CTL_M   0x38            /* Codec Control */
#define ENC_CODEC_LO_V    0x00            /* CODEC_CLK and CODEC_SYNC low    */
#define ENC_CODEC_LOHI_V  0x08            /* CODEC_CLK low, CODEC_SYNC hi    */
#define ENC_CODEC_HILO_V  0x10            /* CODEC_CLK hi, CODEC_SYNC low    */
#define ENC_CODEC_HI_V    0x18            /* CODEC_CLK and CODEC_SYNC high   */
#define ENC_CODEC_NORM_V  0x20            /* CODEC_CLK and CODEC_SYNC normal */

/* Vocoder frame reference disable - if set, SET VOC_FR_REF, VOC_CS_B,
   VOC_WR_B, VOC_OE_B low */
#define ENC_VOC_REF_DIS_M 0x40
#define ENC_VOC_REF_DIS_V 0x40
#define ENC_VOC_REF_ENA_V 0x00

#define ENC_TX_CLKS_DIS_M 0x80            /* Transmit clocks disable     */
#define ENC_TX_CLKS_DIS_V 0x80            /* Transmit clocks disable on  */
#define ENC_TX_CLKS_ENA_V 0x00            /* Transmit clocks disable off */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define ENC_TXP_CTL_M     0x03            /* Power amplifier control    */
#define ENC_TXP_OFF_V     0x00            /* TX_PUNCT off (low)         */
#define ENC_TXP_MASK_V    0x01            /* TX_PUNCT follows MASK_DATA */
#define ENC_TXP_ON_V      0x02            /* TX_PUNCT on (high)         */

#define ENC_INT_BYP_M     0x04            /* Interleaver bypass        */
#define ENC_INT_BYP_V     0x04            /* To bypass interleaver     */
#define ENC_INT_V         0x00            /* To NOT bypass interleaver */

#define ENC_DEINT_BYP_M   0x08            /* Interleaver bypass        */
#define ENC_DEINT_BYP_V   0x08            /* To bypass interleaver     */
#define ENC_DEINT_V       0x00            /* To NOT bypass interleaver */

#define ENC_SYNCCH_M      0x10            /* Sync channel on forward link   */
#define ENC_SYNCCH_V      0x10            /* Sync channel is being recieved */
#define ENC_NO_SYNCCH_V   0x00            /* Sync channel is not being rx'd */

#define ENC_ACCESS_M      0x20            /* Access channel transmitted */
#define ENC_ACCESS_V      0x20            /*   yes */
#define ENC_NO_ACCESS_V   0x00            /*   no  */

#ifdef T_MSM5000

#define ENC_IS95C_PILOT_M      0x0100          /* IS95 C Pilot transmitted */
#define ENC_IS95C_PILOT_V      0x0100          /*   yes */
#define ENC_NO_IS95C_PILOT_V   0x0000          /*   no  */

#define ENC_IS95C_PCBIT_M      0x0080          /* Test IS95 C PC bits transmitted */
#define ENC_IS95C_PCBIT_V      0x0080          /*   yes */
#define ENC_NO_IS95C_PCBIT_V   0x0000          /*   no  */
#define ENC_ZERO_PCBIT_V       0x0000          /*   no  */

#endif

#define ENC_TX_FMT_M      0x40            /* TX in offset-bin or 2'c comp */
#define ENC_TX_2S_V       0x40            /* TX in 2's comp               */
#define ENC_TX_OFF_V      0x00            /* TX offset-binary             */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* specify number of power-control groups that TRAFFIC frames are offset */
/* from a system frame */

#define ENC_FWD_OFF_M     0xF0            /* Forward Link */
#define ENC_REV_OFF_M     0x0F            /* Reverse Link */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* General purpose I/O writes for pins ENC_GP_IO12 thru ENC_GP_IO0 */

#define ENC_GPIO0_M       0x01            /* Pin GP_IO_0 */
#define ENC_GPIO1_M       0x02            /* Pin GP_IO_1 */
#define ENC_GPIO2_M       0x04            /* Pin GP_IO_2 */
#define ENC_GPIO3_M       0x08            /* Pin GP_IO_3 */
#define ENC_GPIO4_M       0x10            /* Pin GP_IO_4 */
#define ENC_GPIO5_M       0x20            /* Pin GP_IO_5 */
#define ENC_GPIO6_M       0x40            /* Pin GP_IO_6 */
#define ENC_GPIO7_M       0x80            /* Pin GP_IO_7 */
#define ENC_GPIO8_M       0x01            /* Pin GP_IO_8 */
#define ENC_GPIO9_M       0x02            /* Pin GP_IO_9 */
#define ENC_GPIO10_M      0x04            /* Pin GP_IO_10 */
#define ENC_GPIO11_M      0x08            /* Pin GP_IO_11 */
#define ENC_GPIO12_M      0x10            /* Pin GP_IO_12 */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* General purpose open-drain I/O writes for pins ENC_GP_IO12 thru ENC_GP_IO0 */

#define ENC_PA_M          0x10
#if defined( T_MSM2P )
/* PA_ON is inverted from MSM 1.0 */
#define ENC_PA_OFF_V      0x00
#define ENC_PA_ON_V       0x10
#else
#define ENC_PA_OFF_V      0x10
#define ENC_PA_ON_V       0x00
#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                             TEST REGISTERS                              */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Address of RAM word being written or read */

#define ENC_ADDR1_M       0x03            /* mask 2 address bits from ENC_ADDR1 */

#define ENC_ENC_RAM_M     0x04            /* Encoder RAM select */
#define ENC_ENC_RAM_V     0x04
#define ENC_INT_RAM_M     0x08            /* Interleaver RAM select */
#define ENC_INT_RAM_V     0x08
#define ENC_DEI_RAM_M     0x10            /* Deinterleaver RAM select */
#define ENC_DEI_RAM_V     0x10

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* RAM control signals used during RAM testing */

#define ENC_CHIP_SEL_M    0x01            /* RAM chip select (active low) */
#define ENC_WRITE_M       0x02            /* RAM write enable (active low) */
#define ENC_OE_M          0x04            /* RAM output enable (active low) */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define ENC_SET_IQ_M      0x02            /* Set IQ Phase Waveform         */
#define ENC_SET_IQ_V      0x02            /* I & Q high after spreading    */
#define ENC_NO_SET_IQ_V   0x00            /* Don't force I & Q             */

#define ENC_MASK_CTL_M    0x0C            /* Internal Master Mask Control  */
#define ENC_MASK_LO_V     0x00            /* Set MASK low                  */
#define ENC_MASK_V        0x04            /* MASK functions normally       */
#define ENC_MASK_HI_V     0x0C            /* Set MASK high                 */

#define ENC_TX_DAT_CTL_M  0x10            /* Transmit data control         */
#define ENC_TX_FIR_V      0x00            /* source = FIR filter           */
#define ENC_TX_TEST_V     0x10            /* source = TX_DATA_TEST         */

#if defined( T_MSM2P )

#define ENC_RX_POL_M      0x20            /* Rx spectral inversion         */
#define ENC_RX_INV_V      0x20            /* Invert                        */
#define ENC_RX_NORM_V     0x00            /* Normal                        */

#define ENC_TX_POL_M      0x40            /* Tx spectral inversion         */
#define ENC_TX_INV_V      0x40            /* Invert                        */
#define ENC_TX_NORM_V     0x00            /* Normal                        */

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if defined( T_MSM2P )

#ifndef T_MSM3
#define ENC_CRC_UB        ENC_IO(0x4e)    /* CRC Polynomial Upper Bits     */
#endif /* not T_MSM3 */

#define ENC_CRC_UB_12_V   0xCF            /* 12-bit CRC                    */
#define ENC_CRC_UB_10_V   0xAF            /* 10-bit CRC                    */
#define ENC_CRC_UB_08_V   0x89            /*  8-bit CRC                    */
#define ENC_CRC_UB_06_V   0x61            /*  6-bit CRC                    */
#define ENC_CRC_UB_DIS_V  0x00            /* CRC disabled                  */

#ifndef T_MSM3
#define ENC_CRC_LB        ENC_IO(0x50)    /* CRC Polynomial Lower Bits     */
#endif /* not T_MSM3 */

#define ENC_CRC_LB_12_V   0x13            /* 12-bit CRC                    */
#define ENC_CRC_LB_10_V   0x64            /* 10-bit CRC                    */
#define ENC_CRC_LB_08_V   0xB0            /*  8-bit CRC                    */
#define ENC_CRC_LB_06_V   0xC0            /*  6-bit CRC                    */
#define ENC_CRC_LB_DIS_V  0x00            /* CRC disabled                  */

#ifndef T_MSM3
#define ENC_TEST_POINT_SEL ENC_IO(0xF0)   /* Selects a test point to
                                             observe. */
#endif /* not T_MSM3 */

#else

/* select source of data to scan back into scan controller shift reg. */
#define ENC_SCANBACK_M    0x01
#define ENC_SCANPATH_V    0x00            /* shift a scan path back */
#define ENC_SCANLOOP_V    0x01            /* loop back as per ENC_LOOPBACK_M */

#define ENC_LOOPBACK_M    0x06            /* Loopback control */
#define ENC_LOOP0_V       0x00            /* Loop back 0's */
#define ENC_LOOP1_V       0x02            /* Loop back 1's */
#define ENC_SCANDATA_V    0x04            /* Loop back scan register */
#define ENC_SCANINV_V     0x06            /* Loop back inverted scan register */

#define ENC_TEST_EN_M     0x08            /* Test enable */
#define ENC_TEST_EN_V     0x08            /* Test is enabled */
#define ENC_TEST_DIS_V    0x00            /* Test is disabled */

#define ENC_SCANCTL_M     0x60            /* Scan control */
#define ENC_NOSCAN_V      0x00            /* No scan operation */
#define ENC_SCANREG_V     0x20            /* Load SCAN_IN shift register */
#define ENC_SCANLOAD_V    0x40            /* Load all scan paths */
#define ENC_SCANSHIFT_V   0x80            /* shift all scan paths */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define ENC_NODES_V       0x00            /* select Internal_Nodes scan path */
#define ENC_PADS_V        0x01            /* select Pads scan path */

#endif

#ifdef T_MSM5000

#define ENC_FCH_RATE_POS      2
#define ENC_SCH_RATE_POS      1
#define ENC_RATE_SET_POS      4 
#define ENC_FCH_CODE_RATE_POS 5
#define ENC_FCH_CRCLEN_POS    8
#define ENC_SCH_CODE_RATE_POS 6
#define ENC_SCH_CRCLEN_POS    8
#define ENC_SCH_GAIN_POS      8


#define ENC_GATING_MODE_POS   10
#define ENC_GATING_MODE_M     0x1C00
#define ENC_DCCH_EN_M         0x0200
#define ENC_DCCH_EN_V         0x0200
#define ENC_DCCH_DIS_V        0x0000


#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*---------------------------------------------------------------------------
                             Read Registers
---------------------------------------------------------------------------*/

#if defined( T_MSM ) || defined( T_MSM2P )

#define ENC_MASK_DATA_M   0x01

#else

#define ENC_INIT_MORE_M   0x80            /* Chip needs extra time to   */
#define ENC_INIT_MORE_V   0x80            /*   initialize               */

#define ENC_EMUL_M        0x40            /* Set if this is an emulator */
#define ENC_EMUL_V        0x40            /*   instead of an ASIC       */

#define ENC_VERSION_M     0x3F            /* Chip version number        */

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* State of chip when last UPDT_STAT fired */
#define ENC_RDWR_ERR_M    0x01            /* if 1, encoder started reading
                                              before uP done writing buffer */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

  /* Word of RAM read during an interleaver and/or deinterleaver RAM test */
#define ENC_INT_TEST_M    0x10            /* The interleaver word */
#define ENC_DINT_TEST_M   0x0F            /* The deinterleaver word */

/*===========================================================================

                      TYPE DECLARATIONS

===========================================================================*/
typedef struct
{
   byte length;
   word poly;
} enc_crc_info_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/*-------------------------------------------------------------------------*/

/*===========================================================================

                      MACRO FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

MACRO ENC_OUT

DESCRIPTION
  This macro outputs a specified value to a specified port. All 8 bits of the
  port are written (i.e., the port is not buffered by software).

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()
  val   Value to write the Encoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_OUT( io, val )  MSM_OUT( io, val )


/*===========================================================================

MACRO ENC_OUTM

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask, to
  a specified port address. Only the bits corresponding to the mask are
  actually affected; other bits retain their previous values. To do this,
  an image is maintained of the previous value written to the port which
  is then combined with the new value.

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()
  mask  Mask of bits in Encoder register to write
  val   Value to write the Encoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef T_MSM3
#define ENC_OUTM(io, mask, val)  MSM_OUTM(io, mask, val)
#endif /* T_MSM3 */

//

/*===========================================================================

MACRO ENC_OUTH

DESCRIPTION
  This macro outputs a specified value to a specified port. All 16 bits of the
  port are written (i.e., the port is not buffered by software).

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()
  val   Value to write the Encoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_OUTH( io, val )  MSM_OUTH( io, val )


/*===========================================================================

MACRO ENC_OUTHM

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask, to
  a specified port address. Only the bits corresponding to the mask are
  actually affected; other bits retain their previous values. To do this,
  an image is maintained of the previous value written to the port which
  is then combined with the new value.

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()
  mask  Mask of bits in Encoder register to write
  val   Value to write the Encoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define ENC_OUTHM(io, mask, val)  MSM_OUTHM(io, mask, val)


/*===========================================================================

MACRO ENC_IN

DESCRIPTION
  This macro reads and returns the value from a specified port. No mask is
  applied to the read value.

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the Encoder register.

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_IN( io )  MSM_IN( io )


/*===========================================================================

MACRO ENC_INM

DESCRIPTION
  This macro reads the value from a specified I/O port address and then
  applies a specified mask to the value. Bits not corresponding to the
  mask are returned as zero.

PARAMETERS
  io    Encoder register defined by this header via ENC_IO()
  mask  Mask of bits in Encoder register to read

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the Encoder register.

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_INM( io, mask )  MSM_INM( io, mask )


/*===========================================================================

MACRO ENC_FCH_RC_INX

DESCRIPTION
   This converts the 1-based enum to a 0 based index for array
   subscripting

PARAMETERS

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_FCH_RC_INX( rc )  ( ( rc ) - 1 )


/*===========================================================================

MACRO ENC_FCH_RATE_INX

DESCRIPTION
   This converts the old defintions of fractional rates to what is
   understood by MSM5000.

PARAMETERS

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/

#define  ENC_FCH_RATE_INX( rate )  ( 3 - ( rate ) )

#endif  /* ENCI_H */

