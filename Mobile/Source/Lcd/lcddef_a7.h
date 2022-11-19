#ifndef LCDDEF_A7_H
#define LCDDEF_A7_H

//************************************************************************
//
//							  L C D CONTROLLER DEFINITIONS
//
//	File Name   : LcdDef_a7.h
//  Description : Support Fuction of A7 Project LCD Controller
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
// 06/29/00    kmh      A8 Project -> LCD Contrast Range를 A7 대비 한단계씩 낮춤
// 02/01/00    kmh      Change Contrast Value for New LCD spec.
// 10/22/99    KMH      Create file for supporting A7 Project
//						Support HD66750 LCD Controller
//
//
//*************************************************************************

#include "comdef.h"

// Font Data Define
// Word Data를 Display하는 관련 함수의 경우 함수 Name에 "Word"가 들어감. 
#define	FONT_DATA							BYTE_DATA
#define BYTE_DATA							1
#define WORD_DATA							2

/* Animation Test Define */
// #define KMH_ANI_TEST		1	

/* Qualcomm MMI */
#define QUALCOMM_SRC		2

/* New Font Format Test */
#define A7_NEW_FONT			3

//------------------------------------------------------------------------

// LCD Command & Data Address 
#define HS_CMD_ADDR				0x2000000    /* A1 ADDR : LCD Command 출력 */
#define	HS_DAT_ADDR				0x2000002    /* A1 ADDR : LCD Data    출력 */

#if (MODEL_ID==MODEL_A10) /* temporary .. will be deleted in new pcb a10 */
	#if defined (A10_NEW) || defined (A10_NEW2)
		#define HS_CMD()
		#define HS_DATA()
	#else
		#define HS_CMD()  BIO_OUT(MSM31_GPIO_2,BIO_GPIO_INT_36_M,BIO_GPIO_INT_36_LO_V)
		#define HS_DATA() BIO_OUT(MSM31_GPIO_2,BIO_GPIO_INT_36_M,BIO_GPIO_INT_36_HI_V)
	#endif
#endif

/*--------------------------------------------------------------------------------*/ 	

// LCD IR(Index Register) Setting Values
#define	HS_LCD_R00				0x0000		/* Start Oscillation */
#define HS_LCD_R01				0x0001		/* Driver Output Control (Duty Set) */
#define HS_LCD_R02				0x0002		/* LCD Driving Waveform Control */
#define HS_LCD_R03				0x0003		/* Power Control */
#define HS_LCD_R04				0x0004		/* Contrast Control */
#define HS_LCD_R05				0x0005		/* Entry Mode */
#define HS_LCD_R06				0x0006		/* Rotation */
#define HS_LCD_R07				0x0007		/* Display Control */
#define HS_LCD_R08				0x0008		/* Cursor Control */ 
#define HS_LCD_R09				0x0009		/* Double-height Display Position */
#define HS_LCD_R10				0x000a		/* Vertical Scroll */
#define HS_LCD_R11				0x000b		/* Horizontal Cursor Position */
#define HS_LCD_R12				0x000c		/* Vertical Cursor Position */
#define HS_LCD_R13				0x000d		/* Not Used */
#define HS_LCD_R14				0x000e		/* Not Used */
#define HS_LCD_R15				0x000f		/* Not Used */
#define HS_LCD_R16				0x0010		/* RAM Write Data Mask */
#define HS_LCD_R17				0x0011		/* RAM Address Set */
#define HS_LCD_R18				0x0012		/* Ram Data Write & Read */

#define	FONT_WIDTH				6					/* Col. Dot No. */
#define	FONT_HEIGHT				3					/* Row  Dot No. */

/*--------------------------------------------------------------------------------*/ 	

// LCD Setting Data Definitions
#define HS_LCD_MASK				0xff
#define HS_LCD_INIT				0x0000

	// HS_LCD_R00
#define HS_LCD_START_OSC		0x0001		/* Oscillation Start */

	// HS_LCD_R01
#define HS_LCD_DUTY_128			0x000f		/* 1/128 Duty */
#define HS_LCD_DUTY_120			0x000e		/* 1/120 Duty */

	// HS_LCD_R02
#define HS_LCD_B_PATTERN		0x0000		/* Wavefrom */
#define HS_LCD_C_PATTERN		0x0040

	// HS_LCD_R04
#define HS_LCD_CONTRAST_MIN	(HS_LCD_CONTRAST_D - 8)		// 0x002f
#define HS_LCD_CONTRAST_LLL	(HS_LCD_CONTRAST_D - 6)		// 0x0031
#define HS_LCD_CONTRAST_LL	(HS_LCD_CONTRAST_D - 4)		// 0x0033
#define HS_LCD_CONTRAST_L	(HS_LCD_CONTRAST_D - 2)		// 0x0035

// kmh modify - 교세라 요청으로 전체 LCD Contrast를 한단계씩 낮춤
#if MODEL_ID == MODEL_A8
#define HS_LCD_CONTRAST_D	0x0035						// Default Contrast Value 
#else	// A8을 제외한 다른 Project
#define HS_LCD_CONTRAST_D	0x0037						// Default Contrast Value 
#endif	

#define HS_LCD_CONTRAST_H	(HS_LCD_CONTRAST_D + 2)		// 0x0039
#define HS_LCD_CONTRAST_HH	(HS_LCD_CONTRAST_D + 4)		// 0x003b
#define HS_LCD_CONTRAST_HHH	(HS_LCD_CONTRAST_D + 6)		// 0x003d
#define HS_LCD_CONTRAST_MAX	(HS_LCD_CONTRAST_D + 8)		// 0x003f

	// HS_LCD_R07
#define HS_LCD_DISPLAY_ON	0x0001		/* upper 16 raster-rows are fixed display, Display on */
#define HS_LCD_DISPLAY_OFF	0x0000

	// HS_LCD_R08
#define HS_LCD_CURSOR_START				0x0004
#define HS_LCD_CURSOR_OFF				0x0000

/*----------------------------------------------------
  현재 blink를 Off함
-----------------------------------------------------*/
#define HS_LCD_BLINK_OFF				0x00ff
/*----------------------------------------------------
  선택된 Area의 현재 Display bit와 And되어 Blink함.
  따라서 현재 Display되어 있는 Bit들이 Blink됨. 
-----------------------------------------------------*/
#define HS_LCD_WHITE_BLINK				0x0000
/*----------------------------------------------------
  선택된 Area의 현재 Display Bit와 Or되어 Blink함.
  따라서 현재 선택된 Area가 전체 On되어 Blink됨.
-----------------------------------------------------*/
#define	HS_LCD_BLACK_BLINK				0x0001
/*----------------------------------------------------
  선택된 Area의 현재 Display Bit와 Xor되어 Display되고 Blink는 하지 않음.
  따라서 현재 선택된 Area의 Reverse display가 됨.
-----------------------------------------------------*/
#define HS_LCD_BLACK_WHITE_REVERSE		0x0002
/*----------------------------------------------------
  선택된 Area의 현재 Display Bit와 Xor되어 Blink됨.
-----------------------------------------------------*/
#define	HS_LCD_BLACK_WHITE_BLINK		0x0003

	// HS_LCD_R17
#define HS_LCD_START_ADDR	0x07ff
#define HS_LCD_CENTER_ADDR	0x069c			// for Qualcomm MMI 

/*--------------------------------------------------------------------------------*/ 	

#define HS_LCD_LOW_DATA(addr)	(addr & 0x00ff)			/* Low Byte of Lcd Word Command */
#define HS_LCD_HIGH_DATA(addr)	((addr >> 8) & 0x00ff)	/* High Byte of Lcd Word Command */

/*--------------------------------------------------------------------------------*/ 	

/* LCD Application Specifications
** 1. LCD SIZE          : 120 * 96
** 2. Default Font Size : 14 * 16 예정
** 3. Total Charactoer  : 8 Char.
** 4. Total Line        : 5 Line (6 - 1(Icon Area))
*/ 

#if (MMI_TYPE == MMI_SKME)
#define	LCD_TOTAL_ADDRESS_X		32		/* X Address Range : 0x01 ~ 0x0f */
#define	LCD_TOTAL_ADDRESS_Y		96		/* Y Address Range : 0x01 ~ 0x60 */

#define HS_LCD_MAX_LINES		5			/* Number of lines on the display    */

/* LCD Dot Size */
#define	LCD_WIDTH			(LCD_TOTAL_ADDRESS_X / 2 - 1) * 8  /*  LCD Col. Dot No. : 120 */
#define	LCD_HEIGHT			LCD_TOTAL_ADDRESS_Y	           /*  LCD Row Dot No.  : 96  */

#elif (MMI_TYPE == MMI_QUALCOMM) || (MMI_TYPE == MMI_SK)
#define	LCD_TOTAL_ADDRESS_X		16		/* X Address Range : 0x01 ~ 0x0f */
#define	LCD_TOTAL_ADDRESS_Y		96		/* Y Address Range : 0x01 ~ 0x60 */

#define HS_LCD_MAX_LINES		5			/* Number of lines on the display    */

/* LCD Dot Size */
#define	LCD_WIDTH			(LCD_TOTAL_ADDRESS_X - 1) * 8  /*  LCD Col. Dot No. : 120 */
#define	LCD_HEIGHT			LCD_TOTAL_ADDRESS_Y	           /*  LCD Row Dot No.  : 96  */

#else
#error Invalid MMI_TYPE
#endif

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

//------------------------------------------------------------------------

typedef __packed union 
{ 
  __packed struct  { byte low, high; } Byte;
  word HWord;
} HWORD_BYTE;

typedef __packed union
{ 
  __packed struct  { HWORD_BYTE low, high; } HWord;
  dword DWord;
} DWORD_HWORD;

//------------------------------------------------------------------------

// kmh test code 99.11.2
#define START_BIT	0x80
#define test_bit	0x80
// kmh test code 99.11.2


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

#define	BLINK_TIMER					100			/* 100 ms */
#define	BLINK_REVERSE_TIME			6			/* 전체 16 에서 반전 mode 시간 */

#endif	// #ifndef LCDDEF_A7_H