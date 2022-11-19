#ifndef LCDP_H

#define LCDP_H


//============================================================================
//
//                 L C D   H E A D E R    F I L E
//
// DESCRIPTION
//   This file contains the definitions needed for low-level non-stack access
//   to the DMSS LCD hardware.
//
// Copyright (c) 1995, 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
// Copyright (c) 1998, 1999       by QUALCOMM, Incorporated.  All Rights Reserved.
//============================================================================

//============================================================================
//
//                      EDIT HISTORY FOR FILE
//
// This section contains comments describing changes made to this file.
// Notice that changes are listed in reverse chronological order.
//
// $Header:   L:/src/asw/MSM5000/VCS/lcdp.h_v   1.1   10 Sep 1999 18:11:00   lchan  $
//
// when      who    what, where, why   
// --------  -----  ----------------------------------------------------------
// 00.08.23  jwpark add (MMI_TYPE == MMI_A10)
// --------  -----  ----------------------------------------------------------
// 12/08/98  bgc    Added support for target T_Q
// 08/08/96  jah    Changed default contrast to 0x78 to match expected mean.
// 08/04/95  jah    Changed default contrast to 0x7A
// 07/03/95  jah    Created from boot.typ
//
//============================================================================

//============================================================================
//
//                        DATA DECLARATIONS
//
//============================================================================

//----------------------------------------------------------------------------
//                         LCD CONSTANTS
//
// These constants are used to access the LCD controller in the event of a
// fatal error.
//----------------------------------------------------------------------------


#if !defined(MMI_TREX) && (MMI_TYPE == MMI_A10) // jwpark 00.08.23

#if ((TG==T_G) || (TG==T_Q))

#define LCD_LINE_LEN 12              //12 characters per row

// Row Addresses
//
#define LCD_LINE0_ADDR	0x00B0            //Address of 0th line
#define LCD_LINE1_ADDR	0x00C0            //Address of 1st line
#define LCD_LINE2_ADDR	0x00D0            //Address of 2nd line
#define LCD_LINE3_ADDR	0x00E0            //Address of 3rd line

// Delay (in microseconds) after each data/address operation
// Empirically determined to be necessary.
//
#define LCD_OP_DELAY		10

// LCD Command & Data Address 
#define CMD_ADDR				0x2000000    /* A1 ADDR : LCD Command 출력 */
#define	DAT_ADDR				0x2000002    /* A1 ADDR : LCD Data    출력 */

/*--------------------------------------------------------------------------------*/ 	

// LCD IR(Index Register) Setting Values
#define	LCD_R00					0x0000		/* Start Oscillation */
#define LCD_R01					0x0001		/* Driver Output Control (Duty Set) */
#define LCD_R02					0x0002		/* LCD Driving Waveform Control */
#define LCD_R03					0x0003		/* Power Control */
#define LCD_R04					0x0004		/* Contrast Control */
#define LCD_R05					0x0005		/* Entry Mode */
#define LCD_R06					0x0006		/* Rotation */
#define LCD_R07					0x0007		/* Display Control */
#define LCD_R08					0x0008		/* Cursor Control */ 
#define LCD_R09					0x0009		/* Double-height Display Position */
#define LCD_R10					0x000a		/* Vertical Scroll */
#define LCD_R11					0x000b		/* Horizontal Cursor Position */
#define LCD_R12					0x000c		/* Vertical Cursor Position */
#define LCD_R13					0x000d		/* Not Used */
#define LCD_R14					0x000e		/* Not Used */
#define LCD_R15					0x000f		/* Not Used */
#define LCD_R16					0x0010		/* RAM Write Data Mask */
#define LCD_R17					0x0011		/* RAM Address Set */
#define LCD_R18					0x0012		/* Ram Data Write & Read */

// LCD Setting Data Definitions
#define LCD_MASK				0xff
#define LCD_INIT				0x0000

	// HS_LCD_R00
#define LCD_START_OSC		0x0001		/* Oscillation Start */

	// HS_LCD_R01
#define LCD_DUTY_128		0x000f		/* 1/128 Duty */
#define LCD_DUTY_120		0x000e		/* 1/120 Duty */

	// HS_LCD_R02
#define LCD_B_PATTERN		0x0000				/* Wavefrom */

	// HS_LCD_R04
#define LCD_CONTRAST_LLL	0x0000
#define LCD_CONTRAST_LL		0x0025
#define LCD_CONTRAST_L		0x0030
#define LCD_CONTRAST_D		0x0033		/* Default Contrast Value */
#define LCD_CONTRAST_H		0x0036
#define LCD_CONTRAST_HH		0x003b
#define LCD_CONTRAST_HHH	0x003f

	// HS_LCD_R07
#define LCD_DISPLAY_ON		0x0001		/* upper 16 raster-rows are fixed display, Display on */
#define LCD_DISPLAY_OFF		0x0000

#define LCD_CHECK_BIT			0x80
#define LCD_MASK_BIT			0xc000

#define	LCD_TOTAL_ADDRESS_X		16		/* X Address Range : 0x01 ~ 0x0f */
#define	LCD_TOTAL_ADDRESS_Y		96		/* Y Address Range : 0x01 ~ 0x60 */

/*--------------------------------------------------------------------------------*/ 	

#define LCD_LOW_DATA(addr)	(addr & 0x00ff)					/* Low Byte of Lcd Word Command */
#define LCD_HIGH_DATA(addr)	((addr >> 8) & 0x00ff)	/* High Byte of Lcd Word Command */

#endif		// #if ((TG==T_G) || (TG==T_Q))


#else // #if (MMI_TYPE == MMI_A10) // jwpark 00.08.23


#if ((TG==T_G) || (TG==T_Q))

#define LCD_LINE_LEN 12              //12 characters per row

// Row Addresses
//
#define LCD_LINE0_ADDR 0x00B0            //Address of 0th line
#define LCD_LINE1_ADDR 0x00C0            //Address of 1st line
#define LCD_LINE2_ADDR 0x00D0            //Address of 2nd line
#define LCD_LINE3_ADDR 0x00E0            //Address of 3rd line

// Data definitions for GPIO control
//
#define LCD_SELECT_M 0x008             //Select:  mask, high, low
#define LCD_SELECT_V 0x000
#define LCD_DESELECT_V 0x008

#define LCD_A0_M 0x004             //A0:  mask, high, low
#define LCD_A0_HI_V 0x004
#define LCD_A0_LO_V 0x000

#define LCD_CLK_M 0x002             //Clock:  mask, high, low
#define LCD_CLK_HI_V 0x002
#define LCD_CLK_LO_V 0x000

#define LCD_DATA_M 0x001             //Data:  mask, high, low
#define LCD_DATA_HI_V 0x001
#define LCD_DATA_LO_V 0x000

// I/O Addresses
//
#define DEC_GPIO_0 0x0190            //Decoder GPIO #0
#define DEC_GPIO_0_TS 0x0192            //3-state control for Dec GPIO #0

// Epson SED1230 Instructions
//

#define LCD_A0 0x0100            //A0 Flag for lcd_out

// Cursor Home:  clear the address counter and move the cursor to
// the left edge of the first line.
//
#define LCD_HOME_I 0x00010

// Static Display Control:  control COMS1 and SEGS1 on/off/blink
// Set as All display segments ON
//
#define LCD_STATIC_I 0x00023

// Display on/off control:  on/off control for the display and cursors
// Set the display on/off, with no cursors.
//
#define LCD_DISPLAY_ON_I 0x00031
#define LCD_DISPLAY_OFF_I 0x00030

// Power Save control:  setup or cancel powersave mode
// Powersave with the oscillator on, or non-power-save w/oscillator
//
#define LCD_POWERSAVE_OFF_I 0x00042
#define LCD_POWERSAVE_ON_I 0x00043

// Power supply control:  control the internal power supply circuit.
// Turn on all of the supplies
//
#define LCD_POW_SUPPLY_I 0x0005F

// RAM control:  configure the RAM shape and CGRAM on/off
// 4 lines, use CGRAM
//
#define LCD_RAM_CTL_I 0x00069

// Contrast control:  electronic contrast control
// Set the contrast to midscale
//
#define LCD_CONTRAST_I 0x00078

// Delay (in microseconds) after each data/address operation
// Empirically determined to be necessary.
//
#define LCD_OP_DELAY 10

#endif

/*---------------------------------------------------------------------------

  LCD Controller macros, commands, definitions, and types

---------------------------------------------------------------------------*/

/* Mask of GPIO bits for the LCD controller serial interface
*/
#define LCD_MASK     ( BIO_LCD_DATA_M   \
		       | BIO_LCD_CLK_M    \
		       | BIO_LCD_A0_M     \
		       | BIO_LCD_SELECT_M )


#endif // #if (MMI_TYPE == MMI_A10) // jwpark 00.08.23


#endif
