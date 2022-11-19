#ifndef LCD_B1_H
#define LCD_B1_H

#include "biog.h"
#include "comdef.h"

/*
-----------------------------------------------------------------------
					LCD Command & Data Address, MACRO
-----------------------------------------------------------------------
*/

#define LCD_COMMAND_ADDRESS			0x2000000    /* A1 ADDR : LCD Command 출력 */
#define	LCD_DATA_ADDRESS			0x2000002    /* A1 ADDR : LCD Data    출력 */

#define	LCD_OUTP_CMD( Value )	outp ( LCD_COMMAND_ADDRESS , (Value) );								
#define	LCD_OUTP_DATA( Value )	outp ( LCD_DATA_ADDRESS , (Value) );								

/*
-----------------------------------------------------------------------
						   LCD Address Size
-----------------------------------------------------------------------
*/


// Total Display Size (LCD display capacity)
#define	LCD_TOTAL_PAGE_SIZE		120			// a row or line = a page, Y
#define LCD_TOTAL_COLUMN_SIZE	120			// X

#define HS_LCD_MAX_LINES	5	/* Number of lines on the display    */

// Actual Display Size

#define LCD_ORIGIN				0
// 현재 화면 데이터가 가로가 128이므로 임시로 아래와 같이 함
// 추후 MMI와 협의하여 제대로 고치도록 할것   ... by youandi 00.08.20
#define LCD_WIDTH				(LCD_TOTAL_COLUMN_SIZE + 7)
#define LCD_HEIGHT				96



/*
-----------------------------------------------------------------------
							LCD command
-----------------------------------------------------------------------
*/


#define DISPLAY_ON					0xAF	// display on
#define DISPLAY_OFF					0xAE	// display off
#define DISPLAY_NORMAL				0xA6	// display normal
#define DISPLAY_INVERSE				0xA7	// display inverted
#define DISPLAY_CONTROL				0xCA	// display control
#define SLEEP_IN					0x95	// sleep in
#define SLEEP_OUT					0x94	// sleep out
#define PAGE_ADDRESS_SET			0x75	// page address set
#define COLUMN_ADDRESS_SET			0x15	// column address set
#define DATA_CONTROL				0xBC	// data control
#define RAM_WRITE					0x5C	// RAM write
#define PARTIAL_DISPLAY_IN			0xA8	// partial display in
#define PARTIAL_DISPLAY_OUT			0xA9	// partial display out
#define READ_MODIFY_WRITE_IN		0xE0	// read modify write in
#define READ_MODIFY_WRITE_OUT		0xEE	// read modify write out
#define AREA_SCROLL_SET				0xAA	// area scroll set
#define SCROLL_START_BLOCK			0xAB	// scroll block start
#define VOLUMN_CONTROL				0xC6	// volumn control
#define NOP							0x25	// no operation

/*
-----------------------------------------------------------------------
					LCD Display Control Parameters
-----------------------------------------------------------------------
*/

	/* F1,F2 operation pattern */
#define F12_OP_PATTERN_4H				0x01
#define F12_OP_PATTERN_8H				0x00
#define F12_OP_PATTERN_16H				0x02

	/* 분주 */
#define DIVIDED_PERIOD_2				0x00
#define DIVIDED_PERIOD_4				0x08
#define DIVIDED_PERIOD_8				0x0C
#define DIVIDED_PERIOD_16				0x04

	/* Test mode */
#define F12_NORMAL_MODE					0x00
#define F12_TEST_MODE					0x10
#define FRC_NORMAL_MODE					0x00
#define FRC_TEST_MODE					0x20

	/* FRC mode */
#define FRC_FORWARD_FLOW				0x00
#define FRC_REVERSE_FLOW				0x40
#define FRC_FRAME_FLOW					0x00
#define FRC_1H_FLOW						0x80

	/* duty cycle */
#define LCD_DUTY_1_120					0x1D	// 1/120
#define LCD_DUTY_1_160					0x27	// 1/160
// we can define other duty cycle 

	/* inverted line number */
#define INVERTED_LINE_12				0x0B

// Volumn Control Parameter

	/* Step Voltage Number for Contrast */
#define STEP_VOLT_NUMBER_22				0x16

	/* Contrast up from the current or down */
#define CONTRAST_UP_FROM_CURRENT		0x20
#define CONTRAST_DOWN_FROM_CURRENT		0X00	

// End-to-End display address
#define FIRST_COLUMN_ADDRESS			0x00
#define LAST_COLUMN_ADDRESS				0x77

#define FIRST_PAGE_ADDRESS				0x00
#define LAST_PAGE_ADDRESS				0x77

// data control
	/* page address */
#define LOCATION_0_PAGE_LN1				0x00
#define LOCATION_0_PAGE_LN168			0x01

	/* column address */
	// LPOS="H" 경우
#define LOCATION_0_COLUMN_SEG123		0x00  //0 column = segment 1 2 3
#define LOCATION_239_COLUMN_SEG123		0x02
	// LPOS=="L" 경우
#define LOCATION_120_COLUMN_SEG123		0x00
#define LOCATION_119_COLUMN_SEG123		0x02

	/* scan direction */
#define COLUMN_SCAN_DIRECTION			0x00
#define PAGE_SCAN_DIRECTION				0x04

	/* blue data conversion */
#define CONVERT_01_TO_001				0x00
#define CONVERT_01_TO_010				0x08
#define CONVERT_01_TO_011				0x10

#define CONVERT_10_TO_100				0x00
#define CONVERT_10_TO_101				0x20
#define CONVERT_10_TO_110				0x40

	/* RGB arrangement */ // refer to manual 
#define RGB_RGB_1_ARRANGEMENT			0x00
#define BGR_BGR_1_ARRANGEMENT			0x01
#define RGB_BGR_1_ARRANGEMENT			0x02
#define BGR_RGB_1_ARRANGEMENT			0x03

#define RGB_RGB_2_ARRANGEMENT			0x04
#define BGR_BGR_2_ARRANGEMENT			0x05
#define RGB_BGR_2_ARRANGEMENT			0x06
#define BGB_BGR_2_ARRANGEMENT			0x07



/*
-----------------------------------------------------------------------
					        Font Sizes
-----------------------------------------------------------------------
*/

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

#endif /* LCD_B1_H*/

