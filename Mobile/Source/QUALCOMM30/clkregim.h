#ifndef CLKREGI_H
#define CLKREGI_H
/*===========================================================================

   C L O C K   R E G I M E I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal definitions used by the
  DMSS clock regime services.

Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/clkregim.h_v   1.6   15 Feb 2001 17:17:50   robertom  $   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/01   rmd     Included mclk_5000.h which has definitions that support dynamic
                   clock switching.
03/31/00   hrk     Moved RXDSP and UART clock related MACROS from clkregim.c
01/18/99   bgc     Fixed up #ifdef's with T_MSM5000
09/15/99   lcc     Merged in MSM5000 support from PLT archive:
                   1. Added DEC clock regime for MSM5000.
11/06/98   mk      Added MSM3000 support.
09/08/97   mk      Extracted CODEC clock from CHIPX8 mask.
04/04/97   smo     Created module.

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "target.h"     /* Target specific definitions          */
#include "mclk_5000.h"  /* micro clock switching services       */


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
/***
 * Define CLOCK regimes and their respective source.
 ***/
#if defined (T_MSM5000)
#define CLK_RGM_ENABLE    0x000
#define CLK_RGM_DISABLE   0x7FF          
#elif defined (T_MSM3)
#define CLK_RGM_ENABLE     0x000
#define CLK_RGM_DISABLE    0x3FF
#else
#define CLK_RGM_ENABLE     0x00
#define CLK_RGM_DISABLE    0xFF
#endif /* T_MSM5000 */

#if defined (T_MSM5000)
#define CLK_RGM_CLEAR    0x000
#define CLK_RGM_SET      0x7FF
#elif defined (T_MSM3)
#define CLK_RGM_CLEAR      0x000
#define CLK_RGM_SET        0x3FF
#else
#define CLK_RGM_CLEAR      0x00
#define CLK_RGM_SET        0xFF
#endif /* T_MSM5000 */

/* Masks */

#if defined (T_MSM3)
#define CLK_RGM_SBI_M        0x100 /* SBI */
#define CLK_RGM_CDMA_RXDSP_M 0x200 /* Super finger DSP */
#endif /* T_MSM3 */
#define CLK_RGM_GEN_M      0x01 /* Time-Tick INT, Ringer, PDM1-3. TCXO/4 */
#define CLK_RGM_UART_M     0x02 /* UART, TCXO/4 */
#define CLK_RGM_CDMA_TX_M  0x04 /* 1.25msINT, Modul, CODEC(CDMAmode). CHIPX8 */
#if defined (T_MSM3)
#define CLK_RGM_CDMA_RX_M    CLK_RGM_CDMA_RXDSP_M | 0x08
#else
#define CLK_RGM_CDMA_RX_M    0x08
#endif  /* T_MSM3 */           /* Demod, Decoder, Deint, CAGC. CHIPX8 */
#define CLK_RGM_DFM_M      0x10 /* 360kHz Clock Cntr DFM circuits.  TCXO/4 */
#define CLK_RGM_COD_M      0x20 /* CODEC in DFM mode.  CHIPX8 */
#define CLK_RGM_VOC_M      0x40 /* Vocoder, may be CHIPX8, XTAL_IN or GPIO28 */
#define CLK_RGM_TCXO_RST_M 0x80 /* Sleep Cntr, Dog & TCXO/4. For reset only. */

#define CLK_RGM_CHIPX8_M   CLK_RGM_CDMA_TX_M | CLK_RGM_CDMA_RX_M

#if defined(T_MSM5000)
#define CLK_RGM_DEC_M        0x400  /* DEC/DEINT Clock Regime */
#endif /* T_MSM5000 */

#if defined (T_MSM3)
/* CLK_CTL5 depends on several build options */
#undef CLK_CTL5_RXDSP_CLK_SEL_V
#undef CLK_CTL5_UART_CLK_SEL_V

/* Superfinger DSP clock selection */
#ifdef FEATURE_ENHANCED_STANDBY_II
/* Superfinger DSP clock to be TCXO */
#define CLK_CTL5_RXDSP_CLK_SEL_V         DEC_CDMA_RXDSP_CLK_SEL_TCXO_V
#else
/* Superfinger DSP clock to be ChipX8 */
#define CLK_CTL5_RXDSP_CLK_SEL_V         DEC_CDMA_RXDSP_CLK_SEL_CHIPX8_V
#endif /* FEATURE_ENHANCED_STANDBY_II */

/* UART clock selection */
#if defined(FEATURE_UART_CLK_TEST_INPUT)
#error code not present
#endif

#if !defined(FEATURE_UART_CLK_TEST_INPUT) && defined(FEATURE_UART_TCXO_CLK_FREQ)
/* UART clock is TCXO, from the TCXO pin */
#define CLK_CTL5_UART_CLK_SEL_V     DEC_UART_CLK_SEL_TCXO_V
#endif

#if !defined(FEATURE_UART_CLK_TEST_INPUT) && !defined(FEATURE_UART_TCXO_CLK_FREQ)
/* UART clock is TCXO/4, from the TCXO pin */
#define CLK_CTL5_UART_CLK_SEL_V     DEC_UART_CLK_SEL_TCXODIV4_V
#endif
#endif /* T_MSM3 */


/*===========================================================================

FUNCTION CLK_REGIME_INIT


===========================================================================*/
extern void  clk_regime_init ( void );


/*===========================================================================

FUNCTION CLK_REGIME_ENABLE


===========================================================================*/
#if defined (T_MSM3)
extern void  clk_regime_enable
(
  word regime_mask
);
#else
extern void  clk_regime_enable
(
  byte regime_mask
);
#endif /* T_MSM3 */



/*===========================================================================

FUNCTION CLK_REGIME_DISABLE


===========================================================================*/
#if defined (T_MSM3)
extern void  clk_regime_disable
(
  word regime_mask
);
#else
extern void  clk_regime_disable
(
  byte regime_mask
);
#endif /* T_MSM3 */

/*===========================================================================

FUNCTION CLK_REGIME_RESET


===========================================================================*/
#if defined (T_MSM3)
extern void  clk_regime_reset
(
  word regime_mask
);
#else
extern void  clk_regime_reset
(
  byte regime_mask
);
#endif /* T_MSM3 */

#endif /* CLKREGI_H */

