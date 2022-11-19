#ifndef IDLCDB10_H
#define IDLCDB10_H

//************************************************************************
//
//							 I D L C D CONTROLLER DEFINITIONS
//
//	File Name   :
//  Description :zz Support Fuction of B17 Project ID LCD Controller
//	Author		  : SK Teletech Software Team
//
//************************************************************************
//
//			EDIT HISTORY FOR MODULE
//
//  This section contains comments describing changes made to this file.
//  Notice that changes are listed in reverse chronological order.
//
//
// 	 when      who      what, where, why
// --------    ---      ---------------------------------------------------
//  10/25/00   youandi	
//
//
//*************************************************************************

#include "comdef.h"

/*
-----------------------------------------------------------------------
					LCD Command & Data Address, MACRO
-----------------------------------------------------------------------
*/
#define HS_CMD_ADDR				0x2000000    /* B1 ADDR : ID LCD Command 출력 */
#define	LCD_REG_ADDRESS				( * (volatile byte*)( 0x2000000 ) )
#define	HS_DAT_ADDR				0x2000002    /* B1 ADDR : ID LCD Data    출력 */
#define	LCD_DATA_ADDRESS				( * (volatile byte*)( 0x2000002 ) )

#define	LCD_OUTP_CMD( Value )	outp ( 0x2000000 , (Value) );								
#define	LCD_OUTP_DATA( Value )	outp ( 0x2000002 , (Value) );								

/*
-----------------------------------------------------------------------
							LCD command
-----------------------------------------------------------------------
*/


#define DISPLAY_START_LINE			0x40	// Set Display Start Line

/*
-----------------------------------------------------------------------
						   LCD Address Size
-----------------------------------------------------------------------
*/
/* LCD Application Specifications
** 1. LCD SIZE          : 
** 2. Default Font Size : 
** 3. Total Charactoer  : 
** 4. Total Line        : 
*/ 
#define	LCD_TOTAL_ADDRESS_X		16		/* X Address Range : 0x01 ~ 0x0f */
#define	LCD_TOTAL_ADDRESS_Y		96		/* Y Address Range : 0x01 ~ 0x60 */

#define HS_LCD_MAX_LINES		5			/* Number of lines on the display    */

/* LCD Dot Size */
#define	LCD_WIDTH			(LCD_TOTAL_ADDRESS_X - 1) * 8  /*  LCD Col. Dot No. : 120 */
#define	LCD_HEIGHT			LCD_TOTAL_ADDRESS_Y	           /*  LCD Row Dot No.  : 96  */


/*
-----------------------------------------------------------------------
					        Font Sizes
-----------------------------------------------------------------------
*/
/* Screen graphic image type
*/
typedef byte hs_word_grp_scrn_type[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2];
typedef byte hs_grp_scrn_type[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

/*--------------------------------------------------------------------------------*/ 	

#define CHAR_BIT_WIDTH							8

// The number of characters in case of medium font
#define MED_SCREEN_WIDTH						20
// The number of characters in case of standard large font
#define SCREEN_WIDTH							(HS_SCRN_WIDE-1)

// Medium font 12*13
#define	MED_HAN_FONT_WIDTH						12 
#define	MED_HAN_FONT_HEIGHT						13 
#define	MED_HAN_FONT_BUFFER_SIZE_PER_CHAR		26 	/* byte */

// Standard font 14*16
#define	HAN_FONT_WIDTH							14 
#define	HAN_FONT_HEIGHT							16 
#define	HAN_FONT_BUFFER_SIZE_PER_CHAR			32 	/* byte */

#define MED_ASCII_FONT_WIDTH					6  
#define MED_ASCII_FONT_HEIGHT					13 
#define MED_ASCII_FONT_BUFFER_SIZE_PER_CHAR		13 	/* byte */

#define	ASCII_FONT_WIDTH						7  
#define	ASCII_FONT_HEIGHT						16 
#define	ASCII_FONT_BUFFER_SIZE_PER_CHAR			16 	/* byte */

#define	BIG_ASCII_FONT_WIDTH					10
#define	BIG_ASCII_FONT_HEIGHT					21
#define	BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR		42	/* byte */

#define	SMALL_ASCII_FONT_WIDTH					6
#define	SMALL_ASCII_FONT_HEIGHT					8
#define	SMALL_ASCII_FONT_BUFFER_SIZE_PER_CHAR	6	/* byte */

#define	CALENDAR_FONT_WIDTH						5
#define	CALENDAR_FONT_HEIGHT					6
#define	CALENDAR_FONT_BUFFER_SIZE_PER_CHAR		8	/* byte */


// The margin number of pixels when you use MED_SIZE_HAN_FONT font
#define LCD_LEFT_MARGIN							4 /* bits */
/*--------------------------------------------------------------------------------*/ 	

#define HS_LCD_GRAY_SCALE_ON				0x01
#define HS_LCD_CHECK_BIT					0x80
#define HS_LCD_MASK_BIT						0xc000

/*--------------------------------------------------------------------------------*/ 	

/* Define for Draw Mode */
#define	DRAW_NORMAL								0			/* Normal            */
#define	DRAW_FOREGROUND_BLINK					1			/* Character Blink   */
#define	DRAW_BACKGROUND_BLINK					2			/* Background Blink  */
#define	DRAW_REVERSE							3			/* Character Reverse */
#define	DRAW_NORMAL_BLINK						0x81		/* Normal Blink      */
#define	DRAW_REVERSE_BLINK						0x82		/* Reverse Blink     */
#define	DRAW_BLINK								DRAW_FOREGROUND_BLINK
#define DRAW_SCROLL								5

#define	NO_DISPLAY_LCD							0
#define	DISPLAY_LCD								1

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

#define HS_LCD_LINE0_ADDR       ASCII_FONT_HEIGHT * 0  /* Start address of display line 0   */
#define HS_LCD_LINE1_ADDR       ASCII_FONT_HEIGHT * 1  /* Start address of display line 1   */
#define HS_LCD_LINE2_ADDR       ASCII_FONT_HEIGHT * 2  /* Start address of display line 2   */
#define HS_LCD_LINE3_ADDR       ASCII_FONT_HEIGHT * 3  /* Start address of display line 3   */



/******************************************************************************
	ADDED FOR MMI_SK
******************************************************************************/
word ScrollTextXY 
( 
  byte *strText,    
  byte *oldText,
  int nPosX, 		
  int nPosY,		
  int nDrawMode	
);



#endif /* LCDA10_H */

