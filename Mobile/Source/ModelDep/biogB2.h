#ifndef BIOG_B1_H

#define BIOG_B1_H
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

$Header:   L:/src/asw/MSM5000/VCS/biog.h_v   1.1   10 Sep 1999 18:10:40   lchan  $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
00.06.14   tasik    Start for Bomber(B1) project
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

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* ----------------------------------------------------------------
                Register #0 (DMOD_GPIO_IN/OUT)

               BIT I/O MASKS FOR GPIO REGISTER 0
-------------------------------------------------------------------*/

                                       // GPIO_0
#define BIO_KYPD_17_M       0x01       /* Keypad column 0 (KYPD_17) */
#define BIO_KYPD_17_HI_V    0x01
#define BIO_KYPD_17_LO_V    0x00

                                       // GPIO_1
#define BIO_KYPD_15_M       0x02       /* Keypad column 1 (KYPD_15) */
#define BIO_KYPD_15_HI_V    0x02
#define BIO_KYPD_15_LO_V    0x00

                                       // GPIO_2
#define BIO_KYPD_13_M       0x04       /* Keypad column 2 (KYPD_13) */
#define BIO_KYPD_13_HI_V    0x04
#define BIO_KYPD_13_LO_V    0x00

                                       // GPIO_3
#define BIO_KYPD_11_M       0x08       /* Keypad column 3 (KYPD_11) */
#define BIO_KYPD_11_HI_V    0x08
#define BIO_KYPD_11_LO_V    0x00

                                       // GPIO_4
#define BIO_KYPD_9_M        0x10       /* Keypad column 4 (KYPD_9) */
#define BIO_KYPD_9_HI_V     0x10
#define BIO_KYPD_9_LO_V     0x00

                                       // GPIO_5
#define BIO_KYPD_MEMO_M     0x20       /* Keypad memo column (KYPD_MEMO) */
#define BIO_KYPD_MEMO_HI_V  0x20
#define BIO_KYPD_MEMO_LO_V  0x00


/* ----------------------------------------------------------------
                Register #1 (ENC_GPIO_0_IN/OUT)

                BIT I/O MASKS FOR GPIO REGISTER 1
-------------------------------------------------------------------*/

                                       // GPIO_6
#define BIO_PLLENABLE_M     0x01       /* PLL Enable */
#define BIO_PLLENABLE_V     0x01
#define BIO_PLLDISABLE_V    0x00

                                       // GPIO_7
#define BIO_PLLCLOCK_M      0x02       /* PLL Clock */
#define BIO_PLLCLOCK_HI_V   0x02
#define BIO_PLLCLOCK_LO_V   0x00

                                       // GPIO_8
#define BIO_PLLDATA_M       0x04       /* PLL data output */
#define BIO_PLLDATA_HI_V    0x04
#define BIO_PLLDATA_LO_V    0x00

                                       // GPIO_9
#define BIO_EXT_PWR_M		0x08       /* EXT_PWR_SENSE */
#define BIO_EXT_PWR_ON_V	0x00
#define BIO_EXT_PWR_OFF_V	0x08
#define BIO_EXT1_M		0x08			/* EXT_PWR_SENSE */
#define BIO_EXT1_ON_V	0x00
#define BIO_EXT1_OFF_V	0x08


                                       // GPIO_10
#define BIO_SYNTH_VCC_M     0x10       // Synth VCC
#define BIO_SYNTH_VCC_HI_V  0x10
#define BIO_SYNTH_VCC_LO_V  0x00

                                       // GPIO_11
#define BIO_CRDL_M          0x20       // CRDL/
#define BIO_CRDL_ON_V       0x00
#define BIO_CRDL_OFF_V      0x20
                                       // GPIO_12
#define BIO_TCXO_M          0x40       /* TCXO ON/OFF control */
#define BIO_TCXO_ON_V       0x40
#define BIO_TCXO_OFF_V      0x00


                                       // GPIO_13
#define BIO_GAS_GUAGE_M      0x80      // Gas Guage Data Line
#define BIO_GAS_GUAGE_HI_V   0x80
#define BIO_GAS_GUAGE_LO_V   0x00
										// by jnlee 01.09.05
#define BIO_JACK_SEND_M      0x80      // earmic send/end key로 사용함
#define BIO_JACK_SEND_HI_V   0x80		
#define BIO_JACK_SEND_LO_V   0x00		//low 인 경우가 눌린 경우임 



/* ----------------------------------------------------------------
                Register #2 (ENC_GPIO_1_IN/OUT)

            BIT I/O MASKS FOR GPIO REGISTER 2
-------------------------------------------------------------------*/

                                       // GPIO_14
#define BIO_PS_HOLD_M       0x01       /* Power Source Holding */
#define BIO_PS_HOLD_ON_V    0x01
#define BIO_PS_HOLD_OFF_V   0x00

                                       // GPIO_15
#define BIO_MEMO_M         0x02			// memo key
#define BIO_MEMO_HI_V      0x02
#define BIO_MEMO_LO_V      0x00

                                       // GPIO_16
#define BIO_EL_M       0x04				// EL
#define BIO_EL_HI_V    0x04
#define BIO_EL_LO_V    0x00

//asdf 64k color LCD를 위해				 // GPIO_17
#define BIO_64KCOLOR_LCD_M		0x08     //65KCOLOR LCD  
#define BIO_64KCOLOR_LCD_HI_V	0x08	 
#define BIO_64KCOLOR_LCD_LO_V   0x00



/* ----------------------------------------------------------------
                Register #3 (DEC_GPIO_0_IN/OUT)

            BIT I/O MASKS FOR GPIO WRITE REGISTER 3
-------------------------------------------------------------------*/

                                       // GPIO_18
#define BIO_MOD_EN_M     0x01       // mod_en for Camera
#define BIO_MOD_EN_HI_V  0x01
#define BIO_MOD_EN_LO_V  0x00

                                       // GPIO_19
#define BIO_LCD_LED_M      0x02      /* LCD LED on/off */
#define BIO_LCD_LED_HI_V   0x02
#define BIO_LCD_LED_LO_V   0x00

                                       // GPIO_20
#define BIO_FLIP_M           0x04      /* FLIP OPEN/CLOSE */          
#define BIO_FLIP_OPEN_V      0x04
#define BIO_FLIP_CLOSE_V     0x00

                                       // GPIO_21
#define BIO_CODEC_DEN_M     0x08       /* CODEC data enable control */
#define BIO_CODEC_DEN_HI_V  0x08
#define BIO_CODEC_DEN_LO_V  0x00


                                       // GPIO_22 
#define BIO_CODEC_CLK_M     0x10       /* CODEC clock control */
#define BIO_CODEC_CLK_HI_V  0x10
#define BIO_CODEC_CLK_LO_V  0x00

                                       // GPIO_23
#define BIO_CODEC_DATA_M    0x20       /* CODEC data control */
#define BIO_CODEC_DATA_HI_V 0x20
#define BIO_CODEC_DATA_LO_V 0x00

                                       // GPIO_24 
#define BIO_AUDIO_EN_M     0x40        /* Audio pwr on/off */
#define BIO_AUDIO_EN_ON_V  0x40
#define BIO_AUDIO_EN_OFF_V  0x00

                                       // GPIO_25
#define BIO_MUX_SEL_M		0x80		// mux_sel for B2
#define	BIO_MUX_SEL_HI_V	0x80
#define	BIO_MUX_SEL_LO_V	0x00



/* ----------------------------------------------------------------
                Register #4 (DEC_GPIO_1_IN/OUT)

               BIT I/O MASKS FOR GPIO REGISTER 4
-------------------------------------------------------------------*/

                                       // GPIO_26 tasik 00.04.24
#define BIO_DCD_M           0x01       /* DCD/, select on/off */
#define BIO_DCD_ON_V        0x00
#define BIO_DCD_OFF_V       0x01

                                       // GPIO_27
#define BIO_IRDA_M          0x02       /* IrDA_SD control */
#define BIO_IRDA_ENA_V      0x00
#define BIO_IRDA_DIS_V      0x02       /* Shut Down */

                                       // GPIO_28 // tasik 00.04.24
#define BIO_RI_O_25_M       0x04       /* RI/, output on/off */
#define BIO_RI_ON_V         0x00
#define BIO_RI_OFF_V        0x04

                                       // GPIO_29
#define BIO_EAR_JACK_SENSE_M     0x08  // ear jack sense
#define BIO_EAR_JACK_SENSE_ON_V  0x00
#define BIO_EAR_JACK_SENSE_OFF_V 0x08

										// GPIO_30 
#define BIO_DC_ON_M     0x10       // DC on for B2	 by jnlee 01.05.25
#define BIO_DC_ON_HI_V  0x10
#define BIO_DC_ON_LO_V 0x00
                                       // GPIO_31
									   // IDLE/, select idle/active
#define BIO_IDLE_M          0x20
#define BIO_IDLE_ENA_V      0x00
#define BIO_IDLE_DIS_V      0x20
#define BIO_TRUE_IDLE_M     0x20
#define BIO_TRUE_IDLE_ENA_V 0x00  
#define BIO_TRUE_IDLE_DIS_V 0x20  


/* --------------------------------------------------------------------------
               BIT I/O Masks for GPIO Register #5

                    ( DMOD_GPIO_INT_IN/ OUT )
------------------------------------------------------------------------- */
                                       // GPIO_INT_0
#define BIO_YMU759_IRQ_M	 0x01
#define BIO_YMU759_IRQ_HI_V  0x01
#define BIO_YMU759_IRQ_LO_V  0x00
                                       // GPIO_INT_1 

                                       // GPIO_INT_1
#define BIO_DTR_M           0x02       // DTR, input on/off
#define BIO_DTR_ON_V        0x02
#define BIO_DTR_OFF_V       0x00
			
									   // GPIO_INT_3  
/* A21  is used for memory addressing instead of GPIO_INT_3 */

									   // GPIO_INT_4  
/* A20 is used for memory addressing instead of GPIO_INT_4 */

/* --------------------------------------------------------------------------
               BIT I/O Masks for GPIO Register #6

                    ( DMOD_KEY_READ_IN )
-------------------------------------------------------------------------- */

                                       // KEY_SENSE_0
#define BIO_KYPD_1_M        0x01       /* Keypad row 0 (KYPD_7) */
#define BIO_KYPD_1_HI_V     0x01
#define BIO_KYPD_1_LO_V     0x00

                                       // KEY_SENSE_1
#define BIO_KYPD_3_M        0x02       /* Keypad row 1 (KYPD_5) */
#define BIO_KYPD_3_HI_V     0x02
#define BIO_KYPD_3_LO_V     0x00

                                       // KEY_SENSE_2
#define BIO_KYPD_5_M        0x04       /* Keypad row 2 (KYPD_3) */
#define BIO_KYPD_5_HI_V     0x04
#define BIO_KYPD_5_LO_V     0x00

                                       // KEY_SENSE_3
#define BIO_KYPD_7_M        0x08       /* Keypad row 3 (KYPD_1) */
#define BIO_KYPD_7_HI_V     0x08
#define BIO_KYPD_7_LO_V     0x00

                                       // KEY_SENSE_4
#define BIO_ON_SW_SENSE_M   0x10       /* On-switch sense */
#define BIO_ON_SW_ON_V      0x00
#define BIO_ON_SW_OFF_V     0x10


// use IFT
//#define BIO_ADC_MUX_M       0x60       /* ADC_MUX0, 1 */
#define BIO_ADC_MUX_0_V     0x00
#define BIO_ADC_MUX_1_V     0x20
#define BIO_ADC_MUX_2_V     0x40
#define BIO_ADC_MUX_3_V     0x60

#endif //#ifndef BIOG_B1_H         /* BIOG_B1_H */
