#ifndef BIOG_H

#define BIOG_H
/*===========================================================================

               G E M I N I   B I O   H E A D E R    F I L E

DESCRIPTION
  This file contains definitions of the bits of the Gemini hardware for
  the BIO services.  These bits are the control lines which can be read
  or written using the BIO services on the Gemini.

  Copyright (c) 1994,1995,1996 by QUALCOMM Incorporated.  All Rights Reserved.
  Copyright (c) 1997,1998,1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/biog.h_v   1.3   24 Aug 2000 10:34:50   tsummers  $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/24/00   ts      Moved the RUIM pwr enb to GPIO_INT_3 since GPIO_INT_4 is 
                   used for A20.
07/07/00   ts      Added RUIM support

00.03.01   chos     D1¿ë GPIO defineÀ» 'biogD1.h'·Î ¿Å±è
00.03.01   chos     Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
11/10/98   jjn     Modified RI/ definition to correctly reflect the fact that
                   it is an active low signal.
10/25/95   jah     Added PCS IF PLL reset
05/11/95   jah     Added TRUE_IDLE and Ringer Enable
04/04/95   jah     Added definitions for external DSP control (GPIO 9 & 10)
10/05/94   jah     Updated contents from GDAC schematic rev X2
10/05/94   jah     Updated contents from GDAC schematic rev X0
05/05/94   jah     Created file

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include	"customer.h"	// added by chos for Ori(D1) project

#if	( MODEL_ID == MODEL_D1 )	// by chos 99.03.01 for D1 GPIO

	#include	"..\ModelDep\biogD1.h"

#elif	(MODEL_ID==MODEL_A10)

	#include	"..\ModelDep\biogA10.h"

#elif	(MODEL_ID == MODEL_B1)

    #include  "..\ModelDep\biogB1.h"  // tasik 00.06.14 for B1 GPIO assignment

#elif	(MODEL_ID == MODEL_B2)

    #include  "..\ModelDep\biogB2.h"  // tasik 00.06.14 for B1 GPIO assignment

#else	// #if	MODEL_ID ==


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* ----------------------------------------------------------------
                Register #0 (DMOD_GPIO_IN/OUT)

               BIT I/O MASKS FOR GPIO REGISTER 0
-------------------------------------------------------------------*/

#define BIO_KYPD_17_M       0x01       /* Keypad column 0 (KYPD_17) */
#define BIO_KYPD_17_HI_V    0x01
#define BIO_KYPD_17_LO_V    0x00

#define BIO_KYPD_15_M       0x02       /* Keypad column 1 (KYPD_15) */
#define BIO_KYPD_15_HI_V    0x02
#define BIO_KYPD_15_LO_V    0x00

#define BIO_KYPD_13_M       0x04       /* Keypad column 2 (KYPD_13) */
#define BIO_KYPD_13_HI_V    0x04
#define BIO_KYPD_13_LO_V    0x00

#define BIO_KYPD_11_M       0x08       /* Keypad column 3 (KYPD_11) */
#define BIO_KYPD_11_HI_V    0x08
#define BIO_KYPD_11_LO_V    0x00

#define BIO_KYPD_9_M        0x10       /* Keypad column 4 (KYPD_9) */
#define BIO_KYPD_9_HI_V     0x10
#define BIO_KYPD_9_LO_V     0x00

#define BIO_KYPD_MEMO_M     0x20       /* Keypad memo column (KYPD_MEMO) */
#define BIO_KYPD_MEMO_HI_V  0x20
#define BIO_KYPD_MEMO_LO_V  0x00


/* ----------------------------------------------------------------
                Register #1 (ENC_GPIO_0_IN/OUT)

                BIT I/O MASKS FOR GPIO REGISTER 1
-------------------------------------------------------------------*/

#define BIO_PLLDATA_M       0x01       /* PLL data output */
#define BIO_PLLDATA_HI_V    0x01
#define BIO_PLLDATA_LO_V    0x00

#define BIO_PLLCLOCK_M      0x02       /* PLL Clock */
#define BIO_PLLCLOCK_HI_V   0x02
#define BIO_PLLCLOCK_LO_V   0x00

#define BIO_PLLENABLE_M     0x04       /* PLL Enable */
#define BIO_PLLENABLE_V     0x04
#define BIO_PLLDISABLE_V    0x00

#define BIO_AUX_PCM_SRC_M   0x08       /* Auxiliary PCM Source Select */
#define BIO_INT_PCM_SRC_V   0x00       /* Internal PCM source         */
#define BIO_EXT_PCM_SRC_V   0x08       /* External PCM source         */

#define BIO_DSP_SRC_SEL_M   0x10       /* DSP Source Select */
#define BIO_INT_DSP_SRC_V   0x00       /* Internal DSP      */
#define BIO_EXT_DSP_SRC_V   0x10       /* External DSP      */

#define BIO_RINGER_EN_M     0x80       /* Ringer driver enable */
#define BIO_RINGER_ENA_V    0x80       /* Enable               */
#define BIO_RINGER_DIS_V    0x00       /* Disable              */

#define BIO_ADC_MUX_M       0x60       /* ADC_MUX0, 1 */
#define BIO_ADC_MUX_0_V     0x00
#define BIO_ADC_MUX_1_V     0x20
#define BIO_ADC_MUX_2_V     0x40
#define BIO_ADC_MUX_3_V     0x60


/* ----------------------------------------------------------------
                Register #2 (ENC_GPIO_1_IN/OUT)

            BIT I/O MASKS FOR GPIO REGISTER 2
-------------------------------------------------------------------*/

#define BIO_CALL_LED_M      0x01       /* Call LED, select on/off */
#define BIO_CALL_LED_ON_V   0x01
#define BIO_CALL_LED_OFF_V  0x00

#define BIO_RUIM_RST_M      0x02      /* R-UIM Reset */ /* GPIO 15 */
#define BIO_RUIM_RST_LOW_V  0x00
#define BIO_RUIM_RST_HIGH_V 0x02

#define BIO_DSP_RESET_M     0x02       /* External DSP reset */
#define BIO_DSP_RESET_V     0x00
#define BIO_DSP_NOT_RESET_V 0x02

#define BIO_DCD_M           0x04       /* DCD/, select on/off */
#define BIO_DCD_ON_V        0x00
#define BIO_DCD_OFF_V       0x04

#define BIO_PS_HOLD_M       0x08       /* Power supply hold/off control */
#define BIO_PS_HOLD_ON_V    0x08
#define BIO_PS_HOLD_OFF_V   0x00


/* ----------------------------------------------------------------
                Register #3 (DEC_GPIO_0_IN/OUT)

            BIT I/O MASKS FOR GPIO WRITE REGISTER 3
-------------------------------------------------------------------*/

#define BIO_LCD_DATA_M      0x01       /* LCD_SER_DATA, data high/low */
#define BIO_LCD_DATA_HI_V   0x01
#define BIO_LCD_DATA_LO_V   0x00

#define BIO_LCD_CLK_M       0x02       /* LCD_SER_CLK, clock high/low */
#define BIO_LCD_CLK_HI_V    0x02
#define BIO_LCD_CLK_LO_V    0x00

#define BIO_LCD_A0_M        0x04       /* LCD_SER_A0, A0 high/low */
#define BIO_LCD_A0_HI_V     0x04
#define BIO_LCD_A0_LO_V     0x00

#define BIO_LCD_SELECT_M    0x08       /* LCD_SER_CS/, select/deselected */
#define BIO_LCD_SELECT_V    0x00
#define BIO_LCD_DESELECT_V  0x08

#define BIO_EEPROM_DATA_M    0x10      /* EEPROM_SER_DATA, data high/low */
#define BIO_EEPROM_DATA_HI_V 0x10
#define BIO_EEPROM_DATA_LO_V 0x00

#define BIO_EEPROM_CLK_M    0x20       /* EEPROM_SER_CLK, clock high/low */
#define BIO_EEPROM_CLK_HI_V 0x20
#define BIO_EEPROM_CLK_LO_V 0x00

#define BIO_PCS_IF_PLL_RST_M 0x40      /* PCS IF PLL reset line          */
#define BIO_PCS_IF_PLL_RST_V 0x40
#define BIO_PCS_IF_PLL_ENA_V 0x00

#define BIO_RI_O_25_M       0x80       /* RI/, output on/off */
#define BIO_RI_ON_V         0x00
#define BIO_RI_OFF_V        0x80


/* ----------------------------------------------------------------
                Register #4 (DEC_GPIO_1_IN/OUT)

               BIT I/O MASKS FOR GPIO REGISTER 4
-------------------------------------------------------------------*/

#define BIO_EAR_MUTE_M      0x01       /* EAR_MUTE/, select mute/un-mute */
#define BIO_EAR_MUTE_V      0x00
#define BIO_EAR_UNMUTE_V    0x01

#define BIO_FM_M            0x02       /* FM/, select FM or CDMA */
#define BIO_FM_ENA_V        0x00
#define BIO_FM_DIS_V        0x02

#define BIO_BB2_IDLE_M      0x04       /* BB2_IDLE/, select BB2 idle/active */
#define BIO_BB2_IDLE_DIS_V  0x04
#define BIO_BB2_IDLE_ENA_V  0x00

#define BIO_BACKLIGHT_M     0x08       /* Backlight, select on/off */
#define BIO_BACKLIGHT_ON_V  0x08
#define BIO_BACKLIGHT_OFF_V 0x00

/* A bug in the MSM 2p causes IDLE/ to need to be an input
** and the Gemini digital design works around this by hooking
** up GPIO #30 to drive the signal.  At reset, IDLE/ is made
** into an input, so it needs no explicit initialization.
*/
#define BIO_IDLE_M          0x10       /* IDLE/ input, select idle/active */
#define BIO_IDLE_ENA_V      0x00
#define BIO_IDLE_DIS_V      0x10

#define BIO_TRUE_IDLE_M     0x20       /* IDLE/, select idle/active */
#define BIO_TRUE_IDLE_ENA_V 0x00  
#define BIO_TRUE_IDLE_DIS_V 0x20  

#define BIO_CTS_M           0x40       /* CTS/, select on/off */
#define BIO_CTS_ON_V        0x00
#define BIO_CTS_OFF_V       0x40

#define BIO_RFR_M           0x80       /* RFR/, select on/off */
#define BIO_RFR_ON_V        0x00
#define BIO_RFR_OFF_V       0x80


/* --------------------------------------------------------------------------
               BIT I/O Masks for GPIO Register #5

                    ( DMOD_GPIO_INT_IN/ OUT )
------------------------------------------------------------------------- */
#if defined (PRODUCTION_SURF_REV1)

#define BIO_EAR_SW_M        0x02       /* IGN_SENSE/, ignition sense on/off */
#define BIO_EAR_SW_UP_V     0x00       /* we have external power            */
#define BIO_EAR_SW_DOWN_V   0x02       /* we do not have external power     */

#define BIO_EXT_PWR_M       0x01       /* EAR_SWITCH/, sense up/down */
#define BIO_EXT_PWR_ON_V    0x00
#define BIO_EXT_PWR_OFF_V   0x01

#define BIO_DTR_M           0x04       /* DTR, input on/off */
#define BIO_DTR_ON_V        0x04
#define BIO_DTR_OFF_V       0x00

#define BIO_RUIM_PWR_M      0x08       /* R-UIM Power Enable */
#define BIO_RUIM_PWR_OFF_V  0x08
#define BIO_RUIM_PWR_ON_V   0x00

#define BIO_GPIO_INT_4_M    0x10       /* GPIO_INT_4 */
#define BIO_GPIO_INT_4_HI_V 0x00       /* This cannot be used in Tiger since */
#define BIO_GPIO_INT_4_LO_V 0x10       /* the code uses this line for A20 */

#else /* PRODUCTION_SURF_REV0 is being used */

#define BIO_RUIM_PWR_M      0x01      /* R-UIM Power Enable */
#define BIO_RUIM_PWR_OFF_V  0x01
#define BIO_RUIM_PWR_ON_V   0x00

#define BIO_GPIO_INT_1_M    0x02       /* GPIO_INT_1 */
#define BIO_GPIO_INT_1_HI_V 0x02
#define BIO_GPIO_INT_1_LO_V 0x00

#define BIO_DTR_M           0x04       /* DTR, input on/off */
#define BIO_DTR_ON_V        0x04
#define BIO_DTR_OFF_V       0x00

#define BIO_EXT_PWR_M       0x08       /* IGN_SENSE/, ignition sense on/off */
#define BIO_EXT_PWR_ON_V    0x00       /* we have external power            */
#define BIO_EXT_PWR_OFF_V   0x08       /* we do not have external power     */

#define BIO_EAR_SW_M        0x10       /* EAR_SWITCH/, sense up/down */
#define BIO_EAR_SW_UP_V     0x00
#define BIO_EAR_SW_DOWN_V   0x10

#endif /* PRODUCTION_SURF_REV1 */


/* --------------------------------------------------------------------------
               BIT I/O Masks for GPIO Register #6

                    ( DMOD_KEY_READ_IN )
-------------------------------------------------------------------------- */

#define BIO_KYPD_1_M        0x01       /* Keypad row 0 (KYPD_7) */
#define BIO_KYPD_1_HI_V     0x01
#define BIO_KYPD_1_LO_V     0x00

#define BIO_KYPD_3_M        0x02       /* Keypad row 1 (KYPD_5) */
#define BIO_KYPD_3_HI_V     0x02
#define BIO_KYPD_3_LO_V     0x00

#define BIO_KYPD_5_M        0x04       /* Keypad row 2 (KYPD_3) */
#define BIO_KYPD_5_HI_V     0x04
#define BIO_KYPD_5_LO_V     0x00

#define BIO_KYPD_7_M        0x08       /* Keypad row 3 (KYPD_1) */
#define BIO_KYPD_7_HI_V     0x08
#define BIO_KYPD_7_LO_V     0x00

#define BIO_ON_SW_SENSE_M   0x10       /* On-switch sense */
#define BIO_ON_SW_ON_V      0x00
#define BIO_ON_SW_OFF_V     0x10

#endif	// #if	MODEL_ID ==
#endif /* BIOG_H */
