#ifndef DECI_H
#define DECI_H
/*===========================================================================

    D E C O D E R   ( S V D )   I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
   All the declarations and definitions necessary for the Decoder driver to
   interact with the QUALCOMM CDMA Serial Viterbi Decoder chip.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/deci.h_v   1.2   04 Oct 1999 15:23:22   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/22/99   lcc     Merged in MSM5000 support from PLT archive:
           jcw     Changes to support UART clocks for MSM5000
04/16/99   aaj     Added definition for DEC_FUND_INTR in dec_mode
11/05/98   dlb     Added support for MSM3000.
12/03/97   mk      Added DEC_TEST_POINT_SEL MSM2.3 register.
10/07/97   smo     Made dec_image[] public for clock regime control and
                   added clock regime defines for MSM2300.
10/10/95   fkm     Minor Macro Changes to save ROM.
04/03/95   jah     Added overflow bits for SER registers
03/17/95   jah     Added support for 14400 rate set and MSM2p decoder mods.
05/11/93   jah     Added definition of DEC_CENABLE_V (enable chipx8 clock)
07/27/92   jah     Made dec_image[] private, no longer used by BIO
07/02/92   jah     Added DEC_GPIO_0/1_IN and made dec_image[] public for BIO
02/06/92   jah     Ported module from brassboard dec.h

===========================================================================*/

#include "comdef.h"     /* in/outp prototypes and types: byte, etc.     */
#include "processor.h"      /* Definition of chip selects                   */
#include "target.h"     /* Target specific definitions                  */

#ifdef T_MSM3
#include "msm.h"
#endif /* T_MSM3 */
/*=============================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define DEC_CHK_BOOT    5       /* Check if emulator booted every N ticks  */

#define DEC_BOOT_TOUT   (3000 / DEC_CHK_BOOT) /* Timeout for emulator boot */

/* Quality thresholds for the 4 data rates
*/
#define DEC_QTHRESH_192         (0x01 << 4)   /* 192 bit is in high nibble */
#define DEC_QTHRESH_96          0x01
#define DEC_QTHRESH_48          (0x03 << 4)   /* 48 bit is in high nibble  */
#define DEC_QTHRESH_24          0x03

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*---------------------------------------------------------------------------
                          Write Registers
---------------------------------------------------------------------------*/

#if defined( T_MSM2P )

#define DEC_RATE_M        0x01            /* Code rate                     */
#define DEC_RATE2_V       0x01            /* Rate 1/2                      */
#define DEC_RATE3_V       0x00            /* Rate 1/3                      */

#define DEC_FRAME_M       0x02            /* Frame mode                    */
#define DEC_CONT_V        0x00            /* Continuous mode               */
#define DEC_PACKET_V      0x02            /* Packet mode                   */

#define DEC_SMTLOAD_M     0x04            /* SMT Load Control              */
#define DEC_SMT_RST_V     0x00            /* Reset load pointer            */
#define DEC_SMT_LOAD_V    0x04            /* Enable RAM load               */

#define DEC_REP_M         0x18            /* Repeats per symbol (SC, PC)   */
#define DEC_REP1_V        0x00            /* 1:1, 9600 bps                 */
#define DEC_REP2_V        0x08            /* 1:2, 4800 bps                 */
#define DEC_REP4_V        0x10            /* 1:4, 2400 bps                 */
#define DEC_REP8_V        0x18            /* 1:8, 1200 bps                 */

#define DEC_INMODE_M      0x00            /* No longer used                */
#define DEC_REPEAT_V      0x00
#define DEC_DBR_V         0x00

#define DEC_INTLV_TYP_M   0x00            /* No longer used                */
#define DEC_BLK_V         0x00
#define DEC_CONV_V        0x00

#define DEC_FUND_INTR_V   0x80            /* DEC_MODE bit to configure decoder */
                                          /* to generate two interrupts in MDR */

#else

#define DEC_RATE_M        0x01            /* Code rate                     */
#define DEC_RATE2_V       0x00            /* Rate 1/2                      */
#define DEC_RATE3_V       0x01            /* Rate 1/3                      */

#define DEC_REP_M         0x06            /* Repeats per symbol (SC, PC)   */
#define DEC_REP1_V        0x00            /* 1:1, 9600 bps                 */
#define DEC_REP2_V        0x02            /* 1:2, 4800 bps                 */
#define DEC_REP4_V        0x04            /* 1:4, 2400 bps                 */
#define DEC_REP8_V        0x06            /* 1:8, 1200 bps                 */

#define DEC_FRAME_M       0x08            /* Frame mode                    */
#define DEC_CONT_V        0x00            /* Continuous mode               */
#define DEC_PACKET_V      0x08            /* Packet mode                   */

#define DEC_INMODE_M      0x10            /* Symbol accumulation mode      */
#define DEC_REPEAT_V      0x00            /* Repeated symbols              */
#define DEC_DBR_V         0x10            /* DBRed symbols                 */

#define DEC_INTLV_TYP_M   0x20            /* Interleaver type              */
#define DEC_BLK_V         0x00            /* Block interleaver             */
#define DEC_CONV_V        0x20            /* Convolutional interleaver     */

#define DEC_SMTLOAD_M     0x80            /* SMT Load Control              */
#define DEC_SMT_RST_V     0x00            /* Reset load pointer            */
#define DEC_SMT_LOAD_V    0x80            /* Enable RAM load               */

#define DEC_FUND_INTR_V   0x80            /* DEC_MODE bit to configure decoder */
                                          /* to generate two interrupts in MDR */

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if defined( T_MSM ) || defined( T_3CHIP )

#define DEC_DIMASK_M      0x10            /* uP_INTD Interrupt control     */
#define DEC_DIMASK_V      0x10            /* Disables Interrupt            */

#define DEC_INTDINV_M     0x40            /* Level sense of DMA output     */
#define DEC_INTDLO_V      0x00            /* Active low                    */
#define DEC_INTDHI_V      0x40            /* Active high                   */

#define DEC_REQDINV_M     0x80            /* Level of DMA request output   */
#define DEC_REQDLO_V      0x00            /* Active low                    */
#define DEC_REQDHI_V      0x80            /* Active high                   */

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define DEC_Q24_M         0x0F         /* Quality thresh for 24-bit frame  */
#define DEC_Q48_M         0xF0         /* Quality thresh for 48-bit frame  */

#define DEC_Q96_M         0x0F         /* Quality thresh for 96-bit frame  */
#define DEC_Q192_M        0xF0         /* Quality thresh for 192-bit frame */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Select SYMIN micro register for input instead of serial to parallel
** input register
*/
#define DEC_INOFF_M       0x01            /* Select uProc Test Mode        */
#define DEC_INOFF_V       0x01

#define DEC_BUFOFF_M      0x02            /* Bypass input RAM buffers      */
#define DEC_BUFOFF_V      0x02

#define DEC_SMTOFF_M      0x04            /* Bypass symbol metric table    */
#define DEC_SMTOFF_V      0x04

#define DEC_BMETOFF_M     0x08            /* Bypass Branch Metric logic,   */
#define DEC_BMETOFF_V     0x08            /*   i&j to metric               */

#define DEC_ACSOFF_M      0x10            /* Bypass ACS decision output    */
#define DEC_ACSOFF_V      0x10

#define DEC_CHAINOFF_M    0x20            /* Bypass CHAINBACK logic        */
#define DEC_CHAINOFF_V    0x20

#define DEC_SIG0OFF_M     0x40            /* Bypass LSB of test signature  */
#define DEC_SIG0OFF_V     0x40

#define DEC_SIG1OFF_M     0x80            /* Bypass MSB of test signature  */
#define DEC_SIG1OFF_V     0x80

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if defined( T_MSM2P )

#define DEC_PNEN_M        0x01            /* PN Test control               */
#define DEC_PNEN_V        0x01            /* Enable start at DEC_SYNC      */

#define DEC_INMUX_M       0x02            /* Select test serial input      */
#define DEC_SYM_V         0x00            /* Use DEC_SYM                   */
#define DEC_MSB_PN_V      0x02            /* Use MSB of PN generator       */

#define DEC_MICROSTB_M    0x04            /* DEC_STB select                */
#define DEC_MICROSTB_V    0x04            /* DEC_STB from uP write         */

#define DEC_SERINV_M      0x08            /* Serial input invert           */
#define DEC_SERINV_V      0x08            /* Invert the serial input       */

#define DEC_NONSTOP_M     0x10            /* Continuous operation for test */
#define DEC_NONSTOP_V     0x10

#define DEC_STATE0_M      0x20            /* Set state input to ACS to 0   */
#define DEC_STATE0_V      0x20

#define DEC_CRC_TEST_M    0xC0            /* CRC test mode                 */
#define DEC_CRC_NORM_V    0x00            /* CRC normal operation          */

#else

#define DEC_CDISABLE_M    0x01            /* CHIPX8 control                */
#define DEC_CDISABLE_V    0x01            /* Disable                       */
#define DEC_CENABLE_V     0x00            /* Enable                        */

#define DEC_UNLOCK_M      0x02            /* Lock of Sync Strobes          */
#define DEC_UNLOCK_V      0x02            /* Don't lock out sync strobes   */

#define DEC_PNEN_M        0x04            /* PN Test control               */
#define DEC_PNEN_V        0x04            /* Enable start at DEC_SYNC      */

#define DEC_INMUX_M       0x08            /* Select test serial input      */
#define DEC_SYM_V         0x00            /* Use DEC_SYM                   */
#define DEC_MSB_PN_V      0x08            /* Use MSB of PN generator       */

#define DEC_MICROSTB_M    0x10            /* DEC_STB select                */
#define DEC_MICROSTB_V    0x10            /* DEC_STB from uP write         */

#define DEC_SERINV_M      0x20            /* Serial input invert           */
#define DEC_SERINV_V      0x20            /* Invert the serial input       */

#define DEC_NONSTOP_M     0x40            /* Continuous operation for test */
#define DEC_NONSTOP_V     0x40

#define DEC_STATE0_M      0x80            /* Set state input to ACS to 0   */
#define DEC_STATE0_V      0x80

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if defined( T_MSM2P )

#if !defined (T_MSM3)
#define DEC_CRC           DEC_IO(0x20)    /* Decoded CRC Polynomials       */
#endif /* !T_MSM3 */

#else

#define DEC_SCANBIT_M     0x01

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if defined( T_MSM2P )

#if !defined (T_MSM3)
#define DEC_TEST_RAM_SEL  DEC_IO(0x22)    /* Test RAM Select               */
#endif /* !T_MSM3 */

#define DEC_CDISABLE_M    0x01            /* CHIPX8 control                */
#define DEC_CDISABLE_V    0x01            /* Disable                       */
#define DEC_CENABLE_V     0x00            /* Enable                        */

#define DEC_CLK_M         0x02            /* CHIPX8 clock pulse            */
#define DEC_CLK_V         0x02

#define DEC_ACSSEL_M      0x0C            /* CHIPX8 clock pulse            */
#define DEC_ACSSEL_NORM_V 0x00            /* ACS Sel for normal operation  */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#if !defined (T_MSM3)
#define DEC_TEST_RAM_CS   DEC_IO(0x24)    /* Test RAM Chip Select          */
#define DEC_TEST_RAM_ADDR DEC_IO(0x26)    /* Test RAM Address              */
#define DEC_TEST_DATA     DEC_IO(0x28)    /* Test RAM Data                 */
#endif /* !T_MSM3 */

#else

#define DEC_SCANSEL_M     0x01
#define DEC_SCANSEL_V     0x00            /* Select the scan MUX           */
#define DEC_FEED_V        0x01            /* Feedback data from SCANIN     */

#define DEC_SCANFEED_M    0x06            /* Control feedback data         */
#define DEC_FEED0_V       0x00            /* 0                             */
#define DEC_FEED1_V       0x02            /* 1                             */
#define DEC_FEEDMSB_V     0x04            /* MSB of SCANIN                 */
#define DEC_FEEDINV_V     0x06            /* Inverted MSB of SCANIN        */

#define DEC_SCANTSTEN_M   0x08            /* Scan data input control       */
#define DEC_SCANTSTEN_V   0x08            /* Use scan register data        */

#define DEC_SCANPHI1_M    0x30            /* Phi1 clock control            */
#define DEC_DISPHI1_V     0x00            /* Disable Phi1                  */
#define DEC_SCANPHI1_V    0x10            /* Load SCANIN with SCAN_PHI1    */
#define DEC_REGPHI1_V     0x20            /* Load observable regs on Phi1  */
#define DEC_ALLPHI1_V     0x30            /* Shift all scan regs on Phi1   */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Select input for shift into SCANOUT register
*/

#define DEC_SCANSIG_V     0x00            /* Signature register            */
#define DEC_SCAN1_V       0x01            /* Scan chain 1                  */
#define DEC_SCAN2_V       0x02            /* Scan chain 2                  */

#endif

/*-------------------------------------------------------------------------
    CLOCK REGIME control registers (MSM23 Only):  These include:

    DEC_CLK_CTL_1 (0x1F8) absolute address
    DEC_CLK_CTL_2 (0x1FA) absolute address
    DEC_CLK_CTL_3 (0x1FC) absolute address
    DEC_CLK_CTL_4 (0x1FE) absolute address

--------------------------------------------------------------------------
 Clock Control 1-  Clock Enable/Disable of individual clock regimes via uP
---------------------------------------------------------------------------*/

#if defined  (T_MSM3)
#define DEC_CDMA_RXDSP_CLK_M      0x200  /* Super finger QDSP clock regime */
#define DEC_CDMA_RXDSP_CLK_DIS_V  0x200  /* Disable Super finger QDSP clock regime */
#define DEC_CDMA_RXDSP_CLK_ENA_V  0x000  /* Enable Super finger QDSP clock regime */

#define DEC_SBI_CLK_M             0x100  /* SBI clock regime */
#define DEC_SBI_CLK_DIS_V         0x100  /* Disable the SBI clock regime */
#define DEC_SBI_CLK_ENA_V         0x000  /* Enable the SBI clock regime */
#endif /* T_MSM3 */

#define DEC_UP_BYPASS_M        0x80      /* uP waitstate logic */
#define DEC_UP_BYPASS_DIS_V    0x80      /* Disables uP waitstate logic */
#define DEC_UP_BYPASS_ENA_V    0x00      /* Enable uP waitstate logic */

#define DEC_VOC_CLK_M          0x40      /* VOC clock regime */
#define DEC_VOC_CLK_DIS_V      0x40      /* Disable VOC clock regime */
#define DEC_VOC_CLK_ENA_V      0x00      /* Enable VOC clock regime */

#define DEC_CODEC_CLK_M        0x20      /* CODEC clock regime */
#define DEC_CODEC_CLK_DIS_V    0x20      /* Disable CODEC clock regime */
#define DEC_CODEC_CLK_ENA_V    0x00      /* Enable CODEC clock regime */

#define DEC_DFM_CLK_M          0x10      /* DFM clock regime */
#define DEC_DFM_CLK_DIS_V      0x10      /* Disable DFM clock regime */
#define DEC_DFM_CLK_ENA_V      0x00      /* Enable DFM clock regime */

#define DEC_CDMA_RX_CLK_M      0x08      /* CDMA RX clock regime */
#define DEC_CDMA_RX_CLK_DIS_V  0x08      /* Disable CDMA RX clock regime */
#define DEC_CDMA_RX_CLK_ENA_V  0x00      /* Enable CDMA RX clock regime */

#define DEC_CDMA_TX_CLK_M      0x04      /* CDMA TX clock regime */
#define DEC_CDMA_TX_CLK_DIS_V  0x04      /* Disable CDMA TX clock regime */
#define DEC_CDMA_TX_CLK_ENA_V  0x00      /* Enable CDMA TX clock regime */

#define DEC_UART_CLK_M         0x02      /* UART clock regime */
#define DEC_UART_CLK_DIS_V     0x02      /* Disable UART clock regime */
#define DEC_UART_CLK_ENA_V     0x00      /* Enable UART clock regime */

#define DEC_GEN_SYS_M          0x01      /* Ringer, PDM1-3, time-tick
                                             tcxo/4 clock regime */
#define DEC_GEN_SYS_DIS_V      0x01      /* Disable ringer, PDM1-3, time-tick*/
#define DEC_GEN_SYS_ENA_V      0x00      /* Enable ringer, PDM1-3, time-tick*/

/*--------------------------------------------------------------------------
 Clock Control 2-  Clock Override of individual clock regimes via uP
---------------------------------------------------------------------------*/

#ifdef T_MSM3
#define DEC_CDMA_RXDSP_CLK_OVRD_M  0x200   /* CDMA_RXDSP clock regime */
#define DEC_CDMA_RXDSP_CLK_OVRD_M  0x200   /* CDMA_RXDSP clock regime */

#define DEC_SBI_CLK_OVRD_M         0x100   /* SBI clock regime */
#define DEC_SBI_CLK_OVRD_M         0x100   /* SBI clock regime */
#endif /* T_MSM3 */

#define DEC_PIN_TCXO_OVRD_M        0x80    /* Sleep cntr, WDOG regime */
#define DEC_PIN_TCXO_OVRD_SET_V    0x80    /* Sleep cntr, WDOG regime */

#define DEC_VOC_CLK_OVRD_M         0x40    /* VOC clock regime */
#define DEC_VOC_CLK_OVRD_SET_V     0x40    /* VOC clock regime */

#define DEC_CODEC_CLK_OVRD_M       0x20    /* CODEC clock regime */
#define DEC_CODEC_CLK_OVRD_SET_V   0x20    /* CODEC clock regime */

#define DEC_DFM_CLK_OVRD_M         0x10    /* DFM clock regime */
#define DEC_DFM_CLK_OVRD_SET_V     0x10    /* DFM clock regime */

#define DEC_CDMA_RX_CLK_OVRD_M     0x08    /* CDMA RX clock regime */
#define DEC_CDMA_RX_CLK_OVRD_SET_V 0x08    /* CDMA RX clock regime */

#define DEC_CDMA_TX_CLK_OVRD_M     0x04    /* CDMA TX clock regime */
#define DEC_CDMA_TX_CLK_OVRD_SET_V 0x04    /* CDMA TX clock regime */

#define DEC_UART_CLK_OVRD_M        0x02    /* UART clock regime */
#define DEC_UART_CLK_OVRD_SET_V    0x02    /* UART clock regime */

#define DEC_GEN_SYS_OVRD_M         0x01    /* ringer, PDM1-3, time-tick */
#define DEC_GEN_SYS_OVRD_SET_V     0x01    /* ringer, PDM1-3, time-tick
                                               tcxo/4 clock regime */
/*--------------------------------------------------------------------------
 Clock Control 3-  Resetting of individual clock regimes 
---------------------------------------------------------------------------*/
#ifdef T_MSM3
#define DEC_CDMA_RXDSP_RES_M   0x200     /* Reset CDMA_RXDSP clock regime */
#define DEC_SBI_RES_M          0x100     /* Reset SBI clock regime */
#endif /* T_MSM3 */

#define DEC_PIN_TCXO_RES_M     0x80      /* Reset Sleep counter, WDOG regime */
#define DEC_VOC_CLK_RES_M      0x40      /* Reset VOC clock regime */
#define DEC_CODEC_CLK_RES_M    0x20      /* Reset CODEC clock regime */
#define DEC_DFM_CLK_RES_M      0x10      /* Reset DFM clock regime */
#define DEC_CDMA_RX_CLK_RES_M  0x08      /* Reset CDMA RX clock regime */
#define DEC_CDMA_TX_CLK_RES_M  0x04      /* Reset CDMA TX clock regime */
#define DEC_UART_CLK_RES_M     0x02      /* Reset UART clock regime */
#define DEC_GEN_SYS_RES_M      0x01      /* Reset ringer, PDM1-3, time-tick
                                             tcxo/4 clock regime */

/*--------------------------------------------------------------------------
 Clock Control 4- uP clock select, PDTMR duration, TCXO div2. 
---------------------------------------------------------------------------*/

#ifdef T_MSM3
#define DEC_SLEEP_OSC_GAIN_M            0x0E00    /* configure gain of internal oscillator circuit */
#define DEC_SLEEP_OSC_GAIN_000          0x0000

#define DEC_SLEEP_OSC_R_BYPASS_M        0x0100    /* bypass the resistance in the internal oscillator circuit */
#define DEC_SLEEP_OSC_R_BYPASS_ENA_V    0x0100
#define DEC_SLEEP_OSC_R_BYPASS_DIS_V    0x0000

#define DEC_SLEEP_XTAL_EN_M             0x80      /* Select Sleep XTAL_Enable */
#define DEC_SLEEP_XTAL_EN_V             0x80      /* Select Sleep XTAL_Enable */
#define DEC_SLEEP_XTAL_DIS              0x00      /* Select Sleep XTAL_Disable */

#define DEC_SLEEP_CLK_SEL_M             0x40      /* Select Sleep clock source */
#define DEC_SLEEP_CLK_SEL_1P92M_V       0x40      /* Select Sleep clock source */
#define DEC_SLEEP_CLK_SEL_32K_V         0x00      /* Select Sleep clock source */

#define DEC_TCXO_DIV4_M        0x20      /* Select  TCXO/4 clock */
#define DEC_TCXO_DIV4_V        0x20      /* Enable  TCXO/4 clock enable for 19.68MHz input */
#define DEC_TCXO_DIV4_DIS      0x00      /* Disable TCXO/4 clock disable for 19.68MHz input */

#endif  /* T_MSM3 */

#define DEC_TCXO_DIV2_M        0x01      /* Divide TCXO/4 pin by 2 */
#define DEC_TCXO_DIV2_V        0x01      /* Enable TCXO/2 clock enable for 9.84MHz input */
#define DEC_TCXO_DIV2_DIS      0x00      /* Disable TCXO/2 clock disable for 9.84MHz input */


/*--------------------------------------------------------------------------
 Clock Control 5- Configures clock source for MSM circuits
---------------------------------------------------------------------------*/

#ifdef T_MSM3
#define DEC_VC_CLK_SEL_M                  0x70  /* Clock Select for Vocoder */
#define DEC_VC_CLK_CHIPX8_V               0x00  /* Use Chipx8               */
#define DEC_VC_CLK_TCX0_V                 0x10  /* USE TCXO                 */
#define DEC_VC_CLK_XTAL_IN2_V             0x20  /* Use XTAL_IN DIV 2        */
#define DEC_VC_CLK_XTAL_IN3_V             0x30  /* Use XTAL_IN DIV 3        */
#define DEC_VC_CLK_GPIO_28_V              0x40  /* Use GPIO 28 INPUT        */
#define DEC_VC_CLK_XTAL_IN_V              0x50  /* Use XTAL_IN              */
#define DEC_VC_CLK_GPIO_28_2_V            0x60  /* Use GPIO28 Pin DIV 2     */
#define DEC_VC_CLK_GPIO_28_3_V            0x70  /* Use GPIO 28 Pin DIV 3    */

#define DEC_VC_OSC_EN_M                   0x08  /* XTAL IN/OUT control            */
#define DEC_VC_OSC_MICRO_CTL_V            0x00  /* Allow microprocessor control   */
#define DEC_VC_OSC_ALWAYS_ENA_V           0x08  /* Always enable                  */

#ifdef T_MSM5000
/* 16-bit register with two extra choices */
#define DEC_UART_CLK_SEL_M                MSM_CLK_CTL5_WH__UART_CLK_SEL_MASK  /* Select the UART clock */
#define DEC_UART_CLK_SEL_TCXODIV4_V       0x0000  /* clock source is TCXO/4 */
#define DEC_UART_CLK_SEL_SLEEP_XTAL_V     0x0004  /* clock source is SLEEP_XTAL_IN PIN */
#define DEC_UART_CLK_SEL_TCXO_V           0x0800  /* clock source is TCXO */
#define DEC_UART_CLK_SEL_TESTPIN_V        0x0804  /* clock source is FM_RX_IDATA */
#else
#define DEC_UART_CLK_SEL_M                0x04  /* Select the UART clock */
#define DEC_UART_CLK_SEL_TCXODIV4_V       0x00  /* clock source is TCXO/4 */
#define DEC_UART_CLK_SEL_SLEEP_XTAL_V     0x04  /* clock source is SLEEP_XTAL_IN PIN */
#endif /* T_MSM5000 */
                
#ifdef T_MSM5000
/* 16-bit register with an extra choice */
#define DEC_CDMA_RXDSP_CLK_SEL_M          MSM_CLK_CTL5_WH__CDMA_RXDSP_CLK_SEL_MASK  /* Select the Superfinger DSP clock */
#define DEC_CDMA_RXDSP_CLK_SEL_CHIPX8_V   0x00  /* clock source CHIPX8 */
#define DEC_CDMA_RXDSP_CLK_SEL_TCX023_V   0x01  /* clock source TCXO 2/3 */
#define DEC_CDMA_RXDSP_CLK_SEL_XTALIND2_V 0x02  /* clock source XTAL_IN div 2 */
#define DEC_CDMA_RXDSP_CLK_SEL_XTALIN_V   0x03  /* clock source XTAL_IN */
#define DEC_CDMA_RXDSP_CLK_SEL_TCXO_V     0x80  /* clock source TCXO */
#else
#define DEC_CDMA_RXDSP_CLK_SEL_M          0x03  /* Select the Superfinger DSP clock */
#define DEC_CDMA_RXDSP_CLK_SEL_CHIPX8_V   0x00  /* clock source CHIPX8 */
#define DEC_CDMA_RXDSP_CLK_SEL_TCX023_V   0x01  /* clock source TCXO 2/3 */
#define DEC_CDMA_RXDSP_CLK_SEL_XTALIND2_V 0x02  /* clock source XTAL_IN div 2 */
#define DEC_CDMA_RXDSP_CLK_SEL_XTALIN_V   0x03  /* clock source XTAL_IN */
#endif /* T_MSM5000 */

#endif  /* T_MSM3 */

/*---------------------------------------------------------------------------
                          Read Registers
---------------------------------------------------------------------------*/

#if defined( T_MSM ) || defined( T_3CHIP )

#define DEC_READY_M       0x80            /* 1 if SVD is ready             */
#define DEC_READY_V       0x80

#define DEC_EMUL_M        0x40            /* Set if this is an emulator    */
#define DEC_EMUL_V        0x40

#define DEC_VERSION_M     0x3F            /* Chip version number           */

#endif

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Symbol error counts -- count of IJ input symbols which do not agree with
** symbols from re-encoded output data bits. Scaled to be the equivalent for
** all error rates.
*/

#define DEC_SER2_OFLOW_M  0x01            /* Overflow bits for 1/2 rate    */
#define DEC_SER4_OFLOW_M  0x03            /* Overflow bits for 1/4 rate    */
#define DEC_SER8_OFLOW_M  0x07            /* Overflow bits for 1/8 rate    */

#define DEC_SER_MAX_V     0xFF            /* Max SER value, for saturation */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define DEC_SMOTST_M      0x3F            /* State RAM output              */

#define DEC_ODDCLK_M      0x80            /* State RAM even/odd select     */

/*===========================================================================

                      MACRO FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

MACRO DEC_OUT

DESCRIPTION
  This macro outputs a specified value to a specified port. All 8 bits of the
  port are written (i.e., the port is not buffered by software).

PARAMETERS
  io    Decoder register defined by this header via DEC_IO()
  val   Value to write the Decoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define  DEC_OUT( io, val )  MSM_OUT( io, val )


/*===========================================================================

MACRO DEC_OUTM

DESCRIPTION
  This macro outputs a specified value, qualified by a specified mask, to
  a specified port address. Only the bits corresponding to the mask are
  actually affected; other bits retain their previous values. To do this,
  an image is maintained of the previous value written to the port which
  is then combined with the new value.

PARAMETERS
  io    Decoder register defined by this header via DEC_IO()
  mask  Mask of bits in Decoder register to write
  val   Value to write the Decoder register

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef T_MSM3
        #define  DEC_OUTM( io, mask, val ) MSM_OUTM( io, mask, val)
#endif /* T_MSM3 */


/*===========================================================================

MACRO DEC_IN

DESCRIPTION
  This macro reads and returns the value from a specified port. No mask is
  applied to the read value.

PARAMETERS
  io    Decoder register defined by this header via DEC_IO()

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the Decoder register.

SIDE EFFECTS
  None

===========================================================================*/

#define  DEC_IN( io )  MSM_IN( io )


/*===========================================================================

MACRO DEC_INM

DESCRIPTION
  This macro reads the value from a specified I/O port address and then
  applies a specified mask to the value. Bits not corresponding to the
  mask are returned as zero.

PARAMETERS
  io    Decoder register defined by this header via DEC_IO()
  mask  Mask of bits in Decoder register to read

DEPENDENCIES
  Address activated registers will not activate if CHIPX8 is off.

RETURN VALUE
  byte read from the Decoder register.

SIDE EFFECTS
  None

===========================================================================*/

#define  DEC_INM( io, mask )  MSM_INM( io, mask )

#endif  /* DECI_H */

