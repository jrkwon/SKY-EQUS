#include <customer.h>
#ifdef MMI_TREX
	#include <hsig.h>
#else //#ifdef MMI_TREX
#ifndef HSIG_H
#define HSIG_H
/*===========================================================================

  G E M I N I   H A N D S E T   I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  Internal definitions specific to the handset module, which include
  communications with the LCD controller and status registers for Gemini.

REFERENCES
  Epson LCD Controller SED1230 Spec Sheet

Copyright (c) 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/hsig.h_v   1.5   12 Oct 2000 09:39:56   tsummers  $

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/04/00   ts      Removed the unneccesary semi-colons in the HS_ON_HOOK macro 
                   definitions.
09/26/00   ts      Changed HS_ON_HOOK to disable use of the handset when RUIM
                   is using the second UART.
04/11/00   jwh     Changed HS_ON_HOOK() MACRO to support 
                   FEATURE_SDEVMAP_SHARE_HFK.
--------   ---     ----------------------------------------------------------
00.03.04   chos     Audio를 살리기 위해 임시로 ear jack 처리 routine 추가
00.03.02   chos     GPIO 변경 부분 수정
00.03.02   chos     Start for Ori(D1) project
--------   ---     ----------------------------------------------------------
03/08/99   jwh     Removed the dependency of "FEATURE_SECOND_UART" on
                   "#ifdef T_SURF".
01/15/98   ak      Made compile time conditional on HS_ON_HOOK macro more
                   stringent, with addition of FEATURE_NEW_SIO.
01/12/98   ak      Added compile conditional code on HS_ON_HOOK macro.
02/05/97    ro     Added HS_FM_ONES_TIME and HS_FM_ONES_PERIOD for counting
                   polling periods between CHIPx8 turn on.
01/04/97   fkm     Added HS_NV_DONE_SIG
08/08/96   jah     Changed default contrast to 78 to match expected mean.
05/29/96   fkm     Updated value of HS_ONES_DET_DELAY
08/04/95   jah     Added hook sensing macro, changed default contrast to 7A
05/10/94   jah     Ported from hsip.h

===========================================================================*/

#include "customer.h"   // jwpark
#include "target.h"     // by chos
#include "comdef.h"     /* Definitions for byte, word, etc.        */
#include "bio.h"        /* Bit I/O macros                          */
#include "voc_core.h"   /* Vocoder Core prototypes and definitions */
#ifdef T_SLEEP
#include "sleep.h"      /* Sleep task signaling                    */
#endif


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------

  Task specific macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Handset Task Signals
*/
#if (MODEL_ID == MODEL_A10)
#define HS_SNI_BLINK_SIG        0x0200
#endif //#if (MODEL_ID == MODEL_A10)

#define HS_NV_DONE_SIG          0x0100  /* NV command "done" signal       */
#define HS_GPIO_TIMER_SIG       0x0080  /* GPIO 'key' poll timer signal   */
#if (MMI_TYPE == MMI_A10)
#define HS_ANI_TIMER_SIG		0x0010  /* Animation timer                */
#endif // #if (MMI_TYPE == MMI_A10)
#define HS_BLINK_TIMER_SIG      0x0008  /* Blink on/off timer             */
#define HS_CMD_Q_SIG            0x0004  /* Queued commands pending        */
#define HS_RPT_TIMER_SIG        0x0001  /* Time to report to the watchdog */

/* How often (in milliseconds) to poll the GPIO 'keys' for their state.
*/
#define HS_GPIO_POLL_TIME       100

/* How long to delay (in microseconds) when resetting the one's detector
*/
#define HS_ONES_DET_DELAY       200

/* Time (in milliseconds) between turning on chipx8 in FM
*/
#define HS_FM_ONES_TIME         1200

/* How many polling periods between turning on chipx8 in FM
*/
#define HS_FM_ONES_PERIOD       HS_FM_ONES_TIME/HS_GPIO_POLL_TIME


#if (MMI_TYPE != MMI_A10) // jwpark 00.08.21
#if (MODEL_ID != MODEL_B1) // girin. 00.10.19 temporary_for_compile
/*---------------------------------------------------------------------------

  LCD Controller macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Mask of GPIO bits for the LCD controller serial interface
*/
#define HS_LCD_MASK     ( BIO_LCD_DATA_M   \
                        | BIO_LCD_CLK_M    \
                        | BIO_LCD_A0_M     \
                        | BIO_LCD_SELECT_M )

/* A0 bit of an LCD command
*/
#define HS_LCD_A0       0x0100

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Epson SED1230 Instructions
*/

/* Cursor Home:  clear the address counter and move the cursor to
** the left edge of the first line.
*/
#define HS_LCD_HOME_I           0x0010

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Static Display Control:  control COMS1 and SEGS1 on/off/blink
** Set as All display segments ON
*/
#define HS_LCD_STATIC_I         0x0023

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Display on/off control:  on/off control for the display and cursors
** Set the display on/off, with no cursors.
*/
#define HS_LCD_DISPLAY_ON_I     0x0031
#define HS_LCD_DISPLAY_OFF_I    0x0030

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Power Save control:  setup or cancel powersave mode
** Powersave with the oscillator on, or non-power-save w/oscillator
*/
#define HS_LCD_POWERSAVE_OFF_I  0x0042
#define HS_LCD_POWERSAVE_ON_I   0x0043

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Power supply control:  control the internal power supply circuit.
** Turn on all of the supplies
*/
#define HS_LCD_POW_SUPPLY_I     0x005F

#define HS_LCD_POW_SUPPLY(p)    (0x0050 | (p))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* RAM control:  configure the RAM shape and CGRAM on/off
** 4 lines, use CGRAM
*/
#define HS_LCD_RAM_CTL_I        0x0069

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Contrast control:  electronic contrast control
** Set the contrast to midscale
*/
#define HS_LCD_CONTRAST_I       0x0078

#define HS_LCD_CONTRAST(c)      (0x0070 | (c))

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Address select:  set the DDRAM, CGRAM, symbol address
*/
#define HS_LCD_ADDR_I(addr)     ( 0x0080 | (addr) )

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Write Data:  load data into the DDRAM, CGRAM, or symbol register
*/
#define HS_LCD_DATA_I(data)     ( HS_LCD_A0 | (data) )

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Number of rows in a character (characters are 5x7)
*/
#define HS_LCD_FONT_ROWS        7

/* Size to step to next character
*/
#define HS_LCD_FONT_CSTEP       (7+1)

/* Number of characters in the user-defined font
*/
#define HS_LCD_FONT_CHARS       4

/* Size of the entire user-defined font
*/
#define HS_LCD_FONT_SIZE        (HS_LCD_FONT_CSTEP * HS_LCD_FONT_CHARS)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#define HS_LCD_MAX_LINES        4       /* Number of lines on the display    */
#define HS_LCD_MAX_CHARS        12      /* Number of characters per line     */

#define HS_LCD_LINE0_ADDR       0x0030  /* Start address of display line 0   */
#define HS_LCD_LINE1_ADDR       0x0040  /* Start address of display line 1   */
#define HS_LCD_LINE2_ADDR       0x0050  /* Start address of display line 2   */
#define HS_LCD_LINE3_ADDR       0x0060  /* Start address of display line 3   */

#define HS_LCD_SYMB_ADDR        0x0070  /* Start address of display symbols  */

#define HS_LCD_CGRAM_ADDR       0x0000  /* Start address of RAM font         */

/* Useful values for specifying line numbers
*/
#define HS_LCD_LINE0            0
#define HS_LCD_LINE1            1
#define HS_LCD_LINE2            2
#define HS_LCD_LINE3            3

#endif // (MODEL_ID != MODEL_B1) // girin. 00.10.19 temporary_for_compile
#endif // #if (MMI_TYPE != MMI_A10) // jwpark 00.08.21 end


/*---------------------------------------------------------------------------

  Keypad polling macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Number of scan columns on the keypad
*/
#define HS_KEY_COLUMNS          6

/* Number of scan rows on the keypad
*/
#if (MODEL_ID == MODEL_A10)||(MODEL_ID == MODEL_B1)
#define HS_KEY_ROWS             5
#else
#define HS_KEY_ROWS             4
#endif

/* Size of the row decode array (2^HS_KEY_ROWS) 2 raised to HS_KEY_ROWS.
*/
#define HS_2_TO_THE_KEY_ROWS    32

/* Mask of all keypad columns
*/
#define HS_KEYPAD_COL_M         ( BIO_KYPD_17_M | BIO_KYPD_15_M | \
                                  BIO_KYPD_13_M | BIO_KYPD_11_M | \
                                  BIO_KYPD_9_M  | BIO_KYPD_MEMO_M  )

#if (MODEL_ID==MODEL_A10) 
/* Mask of all keypad rows
*/
#define HS_KEYPAD_ROW_M         ( BIO_KYPD_1_M | BIO_KYPD_3_M | \
                                  BIO_KYPD_5_M | BIO_KYPD_7_M | \
                                  BIO_KYPD_8_M )

/* Value meaning that no rows (keys) on this keypad column are down.
*/
#define HS_KEY_NO_ROW_V         ( BIO_KYPD_1_HI_V | \
                                  BIO_KYPD_3_HI_V | \
                                  BIO_KYPD_5_HI_V | \
                                  BIO_KYPD_7_HI_V | \
                                  BIO_KYPD_8_HI_V )
#else
/* Mask of all keypad rows
*/
#define HS_KEYPAD_ROW_M         ( BIO_KYPD_1_M | BIO_KYPD_3_M | \
                                  BIO_KYPD_5_M | BIO_KYPD_7_M )

/* Value meaning that no rows (keys) on this keypad column are down.
*/
#define HS_KEY_NO_ROW_V         ( BIO_KYPD_1_HI_V | \
                                  BIO_KYPD_3_HI_V | \
                                  BIO_KYPD_5_HI_V | \
                                  BIO_KYPD_7_HI_V )
#endif

/* Generate a mask for setting the column bits in the keyscan column outputs.
*/
#define HS_KEY_COL_MASK(i)      ((byte) (1 << (i)))

/* Generate a mask for reading the row bits in the keyscan row inputs.
*/
#define HS_KEY_ROW_MASK(i)      ((byte) (1 << (i)))

/* Initialize the keypad column drivers, and set them to 0's
** The MSM pulls up the rows through 100 Kohms to +5.  This
** is a max loss of ~(.05ma * 4) while keys are pressed, but
** this allows for a 0 setup transition to sleep/key_sense_int.
*/
#define HS_KEYPAD_INIT() \
  BIO_TRISTATE( DMOD_GPIO, HS_KEYPAD_COL_M, 0xFF); \
  BIO_OUT( DMOD_GPIO, HS_KEYPAD_COL_M, 0x00)


/*---------------------------------------------------------------------------

  BIO accessed bits which are taken as keys

---------------------------------------------------------------------------*/


/*===========================================================================

MACRO HS_ON_HOOK

DESCRIPTION
  Return the on-hook-in-hands-free-adapter status.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if the portable is in a hands-free adapter
  False is the portable is not in a hands-free adapter

SIDE EFFECTS
  None

===========================================================================*/
/* On the MSM3000, we have two UART's to use.  If using a second UART for DM, 
 * then the 2nd UART's RX line causes changes to AUX_PCM_DIN, which is read
 * by this MACRO.  If AUX_PCM_DIN is read with a particular value 2x in a 
 * row, then the code assumes it is in a hands-free kit.  That causes the 
 * SURF's handset receiver to be shut-off.  We don't want that, in the case
 * where we are using the second UART for DM.
 */

#if (MODEL_ID == MODEL_A10)
#if (defined(FEATURE_NEW_SIO) && defined(FEATURE_SECOND_UART) && \
     defined(FEATURE_SDEVMAP_SHARE_HFK) && defined(FEATURE_SECOND_UART))

#define HS_ON_HOOK()											\
			((hfk_pins_enabled == TRUE) &&						\
			(BIO_INM(DEC_GPIO_1, BIO_CRDL_M) == BIO_CRDL_ON_V))
#else
    #define HS_ON_HOOK() FALSE
#endif

#else // #if (MODEL_ID == MODEL_A10)

#if (defined (FEATURE_NEW_SIO)     &&    \
     defined (FEATURE_SECOND_UART))

  /* FEATURE_SDEVMAP_SHARE_HFK allows the HFK to be enabled and disabled 
   * dynamically.
   */
  #if defined(FEATURE_SDEVMAP_SHARE_HFK) && defined(FEATURE_SECOND_UART)
    #define HS_ON_HOOK()                              \
       (((hfk_pins_enabled) == TRUE) &&               \
        (VC_INM( VC_STATUS, VC_ONES_DETECT_M ) != 0)) 
  #else  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */
    #define HS_ON_HOOK() FALSE;
  #endif  /* FEATURE_SDEVMAP_SHARE_HFK && FEATURE_SECOND_UART */

#elif defined (FEATURE_RUIM_DRIVER)
  #define HS_ON_HOOK() FALSE;
#else  /* (FEATURE_NEW_SIO) && (FEATURE_SECOND_UART) */

  #define HS_ON_HOOK() \
    (VC_INM( VC_STATUS, VC_ONES_DETECT_M ) != 0)

#endif  /* (FEATURE_NEW_SIO) && (FEATURE_SECOND_UART) */

#endif // #if (MODEL_ID == MODEL_A10)


/*===========================================================================

MACRO HS_EXT_PWR_ON

DESCRIPTION
  Return the ignition on/off status when in the hands-free car kit.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if external power is supplied
  False is external power is not supplied

SIDE EFFECTS
  None

===========================================================================*/

#if (MODEL_ID == MODEL_D1)
#define HS_EXT_PWR_ON() \
	  (BIO_INM( DEC_GPIO_1, BIO_EXT_PWR_M ) == BIO_EXT_PWR_ON_V)

#elif (MODEL_ID == MODEL_A10) || (MODEL_ID == MODEL_B1)
#define HS_EXT_PWR_ON() \
	  (BIO_INM( ENC_GPIO_0, BIO_EXT_PWR_M ) == BIO_EXT_PWR_ON_V)
#endif

/*===========================================================================

MACRO HS_EAR_UP

DESCRIPTION
  Return the earpiece up/down status.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if the earpiece is up
  False if earpiece is down

SIDE EFFECTS
  None

===========================================================================*/
// tasik 00.04.25, The code for FLIP is necessary.

#if (MODEL_ID == MODEL_D1) ||(MODEL_ID==MODEL_A10)
#define HS_EAR_INIT() \
  BIO_TRISTATE( DEC_GPIO_0, BIO_FLIP_M, BIO_OUTPUT_DIS_V)

#define HS_EAR_UP() \
	( BIO_INM( DEC_GPIO_0, BIO_FLIP_M ) == BIO_FLIP_OPEN_V )

#elif (MODEL_ID == MODEL_B1) // tasik 00.06.14 for B1
#define HS_EAR_INIT() \
  BIO_TRISTATE( DEC_GPIO_0, BIO_FLIP_M, BIO_OUTPUT_DIS_V)

#define HS_EAR_UP() \
	( BIO_INM( DEC_GPIO_0, BIO_FLIP_M ) == BIO_FLIP_OPEN_V )
#else
#define HS_EAR_UP() \
  ( BIO_INM( DMOD_GPIO_INT, BIO_EAR_SW_M ) == BIO_EAR_SW_UP_V )
#endif


///// Add by happykid in 2000.6.27
///// Add by happykid in 2000.1.6
//		Ear phone인식을 위한 매크로
/*===========================================================================

MACRO HS_JACK_SENSE / HS_JACK_SENSE_INIT

DESCRIPTION
  Return the earphone jack in/out status and initializing GPIO pin

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  True if earphone is in
  False if earphone is out

SIDE EFFECTS
  None

===========================================================================*/
#if ( MODEL_ID == MODEL_D1 )
	#define HS_JACK_SENSE_INIT() \
	BIO_TRISTATE( DEC_GPIO_0, BIO_EAR_JACK_SENSE_M, BIO_OUTPUT_DIS_V);\
	BIO_TRISTATE( DMOD_GPIO_INT, BIO_EXT_SW_M, BIO_OUTPUT_DIS_V)

	#define HS_JACK_SENSE() \
	( BIO_INM (DEC_GPIO_0, BIO_EAR_JACK_SENSE_M ) == BIO_EAR_JACK_SENSE_ON_V )

	#define HS_EAR_JACK_KEY() \
		( BIO_INM(DMOD_GPIO_INT, BIO_EXT_SW_M ) == BIO_EXT_SW_ON_V )

#elif ( MODEL_ID==MODEL_A10)
	#define HS_JACK_SENSE_INIT() \
	BIO_TRISTATE( DEC_GPIO_0, BIO_EAR_JACK_SENSE_M, BIO_OUTPUT_DIS_V);\
	BIO_TRISTATE( DEC_GPIO_1, BIO_JACK_SEND_M, BIO_OUTPUT_DIS_V)

	#define HS_JACK_SENSE() \
	( BIO_INM (DEC_GPIO_0, BIO_EAR_JACK_SENSE_M ) == BIO_EAR_JACK_SENSE_ON_V )

	#define HS_EAR_JACK_KEY() \
		( BIO_INM(DEC_GPIO_1, BIO_JACK_SEND_M ) == BIO_JACK_SEND_ON_V )

#elif ( MODEL_ID == MODEL_B1 )// tasik 00.06.15
	#define HS_JACK_SENSE_INIT() \
		BIO_TRISTATE( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M, BIO_OUTPUT_DIS_V);\
		BIO_TRISTATE( DEC_GPIO_1, BIO_JACK_SEND_M, BIO_OUTPUT_DIS_V)

	#define HS_JACK_SENSE() \
	( BIO_INM ( DEC_GPIO_1, BIO_EAR_JACK_SENSE_M ) == BIO_EAR_JACK_SENSE_ON_V )

	#define HS_EAR_JACK_KEY() \
		( BIO_INM(DEC_GPIO_1, BIO_JACK_SEND_M ) == BIO_JACK_SEND_ON_V )

#endif	//#if ( MODEL_ID == MODEL_D1 )||~

///// End of happykid

/*---------------------------------------------------------------------------

  Backlight initialization and control

---------------------------------------------------------------------------*/


/*===========================================================================

MACRO HS_BACKLIGHT_INIT

DESCRIPTION
  Initialize the GPIO output to drive the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 )	// by chos 99.03.02 for D1 GPIO

#define HS_BACKLIGHT_INIT() \
	{ \
		BIO_TRISTATE( DEC_GPIO_0, BIO_BACKLIGHT_M, 0xFF); \
		BIO_TRISTATE( DEC_GPIO_0, BIO_BACKLIGHT2_M, 0xFF); \
	}
#elif (MODEL_ID==MODEL_A10)
#define HS_BACKLIGHT_INIT()			 							  \
	{															  \
		BIO_TRISTATE(ENC_GPIO_1, BIO_LED_KEYPD_M, 0xFF);          \
		BIO_TRISTATE(DEC_GPIO_0, BIO_LED_BLUE_M,  0xFF);          \
		BIO_TRISTATE(DEC_GPIO_0, BIO_LED_RED_M,   0xFF);          \
		BIO_TRISTATE(DEC_GPIO_1, BIO_LED_GREEN_M, 0xFF);          \
		BIO_OUT(ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V); \
		BIO_OUT(DEC_GPIO_0, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V);  \
		BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M,   BIO_LED_RED_LO_V);   \
		BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); \
	}
#define HS_KEYPAD_BL_INIT()                                       \
	{                                                             \
	    BIO_TRISTATE(ENC_GPIO_1, BIO_LED_KEYPD_M, 0xFF);          \
		BIO_OUT(ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V); \
	}
#define HS_LCD_BL_INIT()                                          \
	{															  \
		BIO_TRISTATE(DEC_GPIO_0, BIO_LED_BLUE_M,  0xFF);          \
		BIO_TRISTATE(DEC_GPIO_0, BIO_LED_RED_M,   0xFF);          \
		BIO_TRISTATE(DEC_GPIO_1, BIO_LED_GREEN_M, 0xFF);          \
		BIO_OUT(DEC_GPIO_0, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V);  \
		BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M,   BIO_LED_RED_LO_V);   \
		BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); \
	}
#elif (MODEL_ID==MODEL_B1)
#define HS_BACKLIGHT_INIT() \
	{ \
		BIO_TRISTATE( DEC_GPIO_0, BIO_LED_KEYPD_M, 0xFF); \
		BIO_TRISTATE( ENC_GPIO_1, BIO_LED_BLUE_M,  0xFF); \
		BIO_TRISTATE( ENC_GPIO_1, BIO_LED_RED_M,   0xFF); \
		BIO_TRISTATE( ENC_GPIO_1, BIO_LED_GREEN_M, 0xFF); \
		BIO_TRISTATE( DEC_GPIO_0, BIO_LCD_LED_M, 0xFF); \
		BIO_OUT( DEC_GPIO_0, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V); \
		BIO_OUT( ENC_GPIO_1, BIO_LED_BLUE_M,  BIO_LED_BLUE_LO_V); \
		BIO_OUT( ENC_GPIO_1, BIO_LED_RED_M,   BIO_LED_RED_LO_V); \
		BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V); \
		BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_LO_V); \
	}
#else	// #if	( MODEL_ID == MODEL_B1 )
#define HS_BACKLIGHT_INIT() \
  BIO_TRISTATE( DEC_GPIO_1, BIO_BACKLIGHT_M, 0xFF)
#endif	// #if	( MODEL_ID == MODEL_D1 )


/*===========================================================================

MACRO HS_BACKLIGHT_ON

DESCRIPTION
  Turn on the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 )	// by chos 99.03.02 for D1 GPIO
								// tasik 00.04.25 add red backlight
#define HS_BACKLIGHT_ON() \
	BIO_OUT( DEC_GPIO_0, BIO_BACKLIGHT2_M, BIO_BACKLIGHT2_ON_V )
//  BIO_OUT( DEC_GPIO_0, BIO_BACKLIGHT_M, BIO_BACKLIGHT_ON_V )

#elif (MODEL_ID==MODEL_A10)
#define HS_BACKLIGHT_ON() \
	BIO_OUT( ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_HI_V )
#define HS_KEYPAD_BL_ON() \
	BIO_OUT(ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_HI_V)
#define HS_LCD_BL_CONTROL(bl_red, bl_green, bl_blue)			    \
	{															    \
		BIO_OUT(DEC_GPIO_0, BIO_LED_RED_M,						    \
				(bl_red ? BIO_LED_RED_HI_V: BIO_LED_RED_LO_V));     \
		BIO_OUT(DEC_GPIO_1, BIO_LED_GREEN_M,                        \
		        (bl_green ? BIO_LED_GREEN_HI_V:BIO_LED_GREEN_LO_V));\
		BIO_OUT(DEC_GPIO_0, BIO_LED_BLUE_M,                         \
		        (bl_blue ? BIO_LED_BLUE_HI_V : BIO_LED_BLUE_LO_V)); \
	}
#elif (MODEL_ID==MODEL_B1)
#define HS_BACKLIGHT_ON() \
	BIO_OUT( DEC_GPIO_0, BIO_LED_KEYPD_M, BIO_LED_KEYPD_HI_V ); \
	BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_HI_V)

#else	// #if	( MODEL_ID == MODEL_D1 )
#define HS_BACKLIGHT_ON() \
  BIO_OUT( DEC_GPIO_1, BIO_BACKLIGHT_M, BIO_BACKLIGHT_ON_V )
#endif	// #if	( MODEL_ID == MODEL_D1 )



/*===========================================================================

MACRO HS_BACKLIGHT_OFF

DESCRIPTION
  Turn off the LCD and keypad backlight

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 )	// by chos 99.03.02 for D1 GPIO
								// tasik 00.04.25 add red backlight
#define HS_BACKLIGHT_OFF() \
  BIO_OUT( DEC_GPIO_0, BIO_BACKLIGHT2_M, BIO_BACKLIGHT2_OFF_V ) 
//	BIO_OUT( DEC_GPIO_0, BIO_BACKLIGHT_M, BIO_BACKLIGHT_OFF_V )

#elif (MODEL_ID==MODEL_A10)
#define HS_BACKLIGHT_OFF() \
	BIO_OUT(ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V)
#define HS_KEYPAD_BL_OFF() \
	BIO_OUT(ENC_GPIO_1, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V)
#elif (MODEL_ID==MODEL_B1)
#define HS_BACKLIGHT_OFF() \
	BIO_OUT( DEC_GPIO_0, BIO_LED_KEYPD_M, BIO_LED_KEYPD_LO_V ); \
	BIO_OUT( DEC_GPIO_0, BIO_LCD_LED_M, BIO_LCD_LED_LO_V)

#else	// #if	( MODEL_ID == MODEL_D1 )
#define HS_BACKLIGHT_OFF() \
  BIO_OUT( DEC_GPIO_1, BIO_BACKLIGHT_M, BIO_BACKLIGHT_OFF_V )
#endif	// #if	( MODEL_ID == MODEL_D1 )


/*---------------------------------------------------------------------------

  Call LED initialization and control

---------------------------------------------------------------------------*/


/*===========================================================================

MACRO HS_CALL_LED_INIT

DESCRIPTION
  Initialize the GPIO output to drive the Call LED.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 )
	// by chos 99.03.02 for D1 GPIO	// tasik 00.04.25 // tasik 00.06.14
#define HS_CALL_LED_INIT() \
  BIO_TRISTATE( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, 0xFF)

#elif (MODEL_ID == MODEL_A10) // jwpark 00.08.08
#define HS_CALL_LED_INIT() \
  BIO_TRISTATE( ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_OUTPUT_ENA_V);  \
  BIO_OUT(ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_OFF_V)

#elif (MODEL_ID == MODEL_B1)	// 임시로 녹색 led켜도록함 
#define HS_CALL_LED_INIT() \
  BIO_TRISTATE( ENC_GPIO_1, BIO_LED_GREEN_M, 0xFF)
//BIO_TRISTATE( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, 0xFF)
#else	// #if	( MODEL_ID == MODEL_D1 )
#define HS_CALL_LED_INIT() \
  BIO_TRISTATE( ENC_GPIO_1, BIO_CALL_LED_M, 0xFF)
#endif	// #if	( MODEL_ID == MODEL_D1 )


/*===========================================================================

MACRO HS_CALL_LED_ON

DESCRIPTION
  Turn on the Call LED

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 ) // by chos 99.03.02 for D1 GPIO	// tasik 00.04.25
#define HS_CALL_LED_ON() \
  BIO_OUT( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_ON_V )

#elif (MODEL_ID == MODEL_A10)
#define HS_CALL_LED_ON() \
  BIO_OUT( ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_ON_V )

#elif	( MODEL_ID == MODEL_B1 ) // 임시로 녹색 led 켬
#define HS_CALL_LED_ON() \
  BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_HI_V)
//  BIO_OUT( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_ON_V )
#else	// #if	( MODEL_ID == MODEL_D1 )
#define HS_CALL_LED_ON() \
  BIO_OUT( ENC_GPIO_1, BIO_CALL_LED_M, BIO_CALL_LED_ON_V )
#endif	// #if	( MODEL_ID == MODEL_D1 )


/*===========================================================================

MACRO HS_CALL_LED_OFF

DESCRIPTION
  Turn off the Call LED.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#if	( MODEL_ID == MODEL_D1 ) // by chos 99.03.02 for D1 GPIO	// tasik 00.04.25
#define HS_CALL_LED_OFF() \
  BIO_OUT( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_OFF_V )

#elif (MODEL_ID == MODEL_A10)
#define HS_CALL_LED_OFF() \
  BIO_OUT( ENC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_OFF_V )

#elif	( MODEL_ID == MODEL_B1 ) // 임시로 녹색 led 끔 
#define HS_CALL_LED_OFF() \
	BIO_OUT( ENC_GPIO_1, BIO_LED_GREEN_M, BIO_LED_GREEN_LO_V)
//  BIO_OUT( DEC_GPIO_0, BIO_RING_INDICATOR_EN_M, BIO_RING_INDICATOR_OFF_V )

#else
#define HS_CALL_LED_OFF() \
  BIO_OUT( ENC_GPIO_1, BIO_CALL_LED_M, BIO_CALL_LED_OFF_V )
#endif



/*===========================================================================
MACRO HS_MOTOR_INIT
MACRO HS_MOTOR_ON
MACRO HS_MOTOR_OFF
===========================================================================*/
#if (MODEL_ID == MODEL_A10)
#define HS_MOTOR_INIT() \
  BIO_TRISTATE(ENC_GPIO_0, BIO_MOTOR_M, BIO_OUTPUT_ENA_V); \
  BIO_OUT(ENC_GPIO_0, BIO_MOTOR_M, BIO_MOTOR_OFF_V)

#define HS_MOTOR_ON() \
  BIO_OUT(ENC_GPIO_0, BIO_MOTOR_M, BIO_MOTOR_ON_V)

#define HS_MOTOR_OFF() \
  BIO_OUT(ENC_GPIO_0, BIO_MOTOR_M, BIO_MOTOR_OFF_V)
#endif



/*---------------------------------------------------------------------------

  SLEEP allow/disallow macros

---------------------------------------------------------------------------*/

/*===========================================================================

MACRO HS_SLEEP_ALLOW

DESCRIPTION
  Tell the sleep task that we are in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The sleep task may take us up on our offer.

===========================================================================*/
#ifdef T_SLEEP
#define HS_SLEEP_ALLOW() \
  { (void) rex_set_sigs( &sleep_tcb, SLEEP_HS_OKTS_SIG ); }
#else
#define HS_SLEEP_ALLOW()
#endif


/*===========================================================================

MACRO HS_SLEEP_FORBID

DESCRIPTION
  Tell the sleep task that we are NOT in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef T_SLEEP
#define HS_SLEEP_FORBID() \
  { (void) rex_clr_sigs( &sleep_tcb, SLEEP_HS_OKTS_SIG ); }
#else
#define HS_SLEEP_FORBID()
#endif

#endif /* HSIG_H */
#endif //#ifdef MMI_TREX
