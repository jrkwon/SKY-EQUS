#ifndef LCDA10_H
#define LCDA10_H

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
// 00.08.23  jwpark     add (MMI_TYPE == MMI_A10)
// --------    ---      ---------------------------------------------------
// 02/01/00    kmh      Change Contrast Value for New LCD spec.
// 10/22/99    KMH      Create file for supporting A7 Project
//						Support HD66750 LCD Controller
//
//
//
//*************************************************************************


#if (MMI_TYPE == MMI_A10) // jwpark 00.08.23


#include "hs.h"
#include "lcddef_a10.h"

/*-----------------------------------------------------------------------
                           Extern LCD data
-----------------------------------------------------------------------*/
extern hs_scrn_type hs_screen;			/* non-blink screen contents      */
extern hs_scrn_type hs_blink_screen;	/* blink screen contents          */
                                        /* Current screen contents        */
extern byte *hs_crnt_screen_ptr;

extern word hs_annun;					/* non-blink annunciators         */
extern word hs_blink_annun;				/* Blink annunciators             */

extern byte hs_rssi;					/* Non-blink RSSI level           */
extern byte hs_blink_rssi;				/* Blink RSSI level               */

extern byte hs_batt;					/* Non-blink Battery level        */
extern byte hs_blink_batt;				/* Blink Battery level            */

extern byte hs_smb;					    /* Non-blink Smart Battery level        */

extern byte *hs_crnt_sni_ptr;			/* Current SNI level             */
extern byte *hs_crnt_rssi_ptr;			/* Current RSSI level             */
extern byte *hs_crnt_batt_ptr;			/* Current Battery level          */
extern word *hs_crnt_annun_ptr;			/* Current annunciators           */
extern byte *hs_crnt_smb_ptr;			/* Current Smart Battery level          */

extern word hs_blink_rate;				/* Rate for blinking, ms / image  */
extern word hs_blink_active;			/* Is blink currently displayed   */

extern boolean hs_backlight;			/* Backlight is currently on      */
#if MMI_TYPE == MMI_A10
extern hs_backcolor_type hs_backcolor;
#endif

/*-----------------------------------------------------------------------
                          Macro for LCD
-----------------------------------------------------------------------*/

#define	LCD_OUTP(Addr, Value) outp((Addr), (Value))
#define	LCD_OUTP_CMD(Value)   outp(HS_CMD_ADDR, (Value))
#define	LCD_OUTP_DATA(Value)  outp(HS_DAT_ADDR, (Value))


/*-----------------------------------------------------------------------
                          Extern LCD function
-----------------------------------------------------------------------*/

/*===========================================================================

EXTERN FUNCTION LCD_CMD_OUT		// kmh 99.10.22

DESCRIPTION
  Output command to the LCD controller.

DEPENDENCIES
  Clock dependent delays which rely on a 13.5 MHz '186 in an MSM 2p.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_cmd_out
(
  word lcd_command
    /* 16-bit LCD command to output to the LCD controller */
);


/*===========================================================================

EXTERN FUNCTION LCD_DAT_OUT

DESCRIPTION
  Output data to the LCD controller.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_word_out
(
  word lcd_data
    /* 16-bit LCD data to output to the LCD controller */
);


/*===========================================================================

EXTERN FUNCTION HS_LCD_INIT		// kmh 99.10. 22

DESCRIPTION
  This function performs power up initialization on the HD44780 LCD
  controller. It sets up the controller in 8 bit mode, selects 2 line
  display mode. It also sets the LCD into a default mode and initializes
  the instruction states for all instructions which have bit fields which
  may vary. The default mode is to turn the display ON and set it up to
  increment the address counter when DDRAM data is written.  No other modes
  are enabled at this time.


DEPENDENCIES
  Must be called at least 15ms after power is applied to the LCD.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_init(void);


/*===========================================================================

EXTERN FUNCTION HS_LCD_SLEEP		// kmh 00.02.16

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_sleep(boolean bSleep);


/*===========================================================================

EXTERN FUNCTION HS_LCD_BLINK		// kmh 00.02.23

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_blink(int nPosX,int nPosY,int nWidth,int nHeight,int nBlinkMode);


/*===========================================================================

EXTERN FUNCTION HS_LCD_ON			// kmh 99.10.22

DESCRIPTION
  This function controls the on/off state of the display.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_on( boolean on );


/*===========================================================================

EXTERN FUNCTION HS_LCD_CONTRAST

DESCRIPTION
  This function controls the contrast state of the display.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  HS_LCD_INIT이 실행 되면 다시 처음의 Default value로 변환됨.

===========================================================================*/
extern void hs_lcd_contrast(word contrast);


/*===========================================================================

EXTERN FUNCTION HS_UPDATE_SCREEN		// kmh 99.10.22

DESCRIPTION
  This procedure updates the screen, making the screen appear the
  same as new_screen.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the display RAM address and contents.

===========================================================================*/
extern void hs_update_screen(byte Command, byte Attr);


/*===========================================================================

EXTERN FUNCTION HS_UPDATE_ANNUN		// kmh 99.10.22

DESCRIPTION
  This procedure updates the annunciators, both LED and LCD.

DEPENDENCIES
  Not called from an ISR

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_update_annun( void );


/*===========================================================================

EXTERN FUNCTION HS_SET_GLYPH		// kmh 99.10.22

DESCRIPTION
  Set the bitmap of a specified character.  The data format of a bitmap
  is a map of each row of the character, 5 bits wide (LSB justified) and
  7 rows tall.  For example:
  
    0x04, 0x0e, 0x1f, 0x03, 0x03, 0x18, 0x18 is

        ..X..
        .XXX.
        XXXXX
        ...XX
        ...XX
        XX...
        XX...

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  By changing the bitmap of the character, any characters of that type
  currently being displayed will change to the new character.

===========================================================================*/
extern void hs_set_glyph
(
  byte index,           /* Index of the character to set the bitmap of */
  byte *bitmap_ptr      /* Pointer to 7 bytes defining the bitmap      */
);


/*===========================================================================

EXTERN FUNCTION LCD_OUT		// kmh 99.10.22

DESCRIPTION
  Output command to the LCD controller.

DEPENDENCIES
  Clock dependent delays which rely on a 13.5 MHz '186 in an MSM 2p.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_out
(
  byte lcd_command
    /* 9-bit LCD command to output to the LCD controller */
);

/*===========================================================================

EXTERN FUNCTION HS_LCD_RESET

DESCRIPTION
   
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_reset( void );	// inserted by kmh 00.05.15

/*===========================================================================

EXTERN FUNCTION HS_LCD_RESET_INSTRUCTION

DESCRIPTION
   
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_reset_instruction ( void );


#if FONT_DATA == BYTE_DATA		// kmh test : font data display
/*===========================================================================

FUNCTION TextOutXY

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void	TextOutXY(byte *strText,byte *oldText,int nPosX,int nPosY,int nDrawMode);
extern byte LCDWordAniBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2];
extern byte LCDWordAniBlinkBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2 ];
void BuildLCDBufferAll(void);
void CopyLCDBufferAll (void);
void DisplayLCDBufferEnhanced(void);
void ClearLCDBufferAll(void);
void BlinkLCDDataBuffer ( void );
int	SetHanFont ( int nFont );
void DisplayLCDBufferLine0(void);
void lcd_SetDispType(hs_scrn_type scrn_ptr, byte PageType,
					 byte WallpaperType, byte WallMaskType, byte DrawMethod);
byte lcd_GetPageType(void);
byte lcd_GetWallType(void);
byte lcd_GetWallMask(void);
byte lcd_GetDrawMethod(void);
#define CURRENT_BLINK	0x00
#define ANI_BLINK		0x01
#define TEXT_BLINK		0x02
void LCDBlinkTimerStart(byte Type, dword tick); // tick in miliseconds
void LCDBlinkTimerStop(void);
#else		// #if FONT_DATA == BYTE_DATA
/*===========================================================================

FUNCTION TextWordOutXY

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void TextWordOutXY(byte *strText,byte *oldText,int nPosX,int nPosY,int nDrawMode);
#endif		// #if FONT_DATA == BYTE_DATA

/*===========================================================================

LOCAL FUNCTION DisplayLCDBufferAll

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void  DisplayLCDBufferAll(void);

/*===========================================================================

EXTERN FUNCTION LCD_TEST

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void hs_lcd_test(void);


#else // #if (MMI_TYPE == MMI_A10) // jwpark 00.08.23


#include "comdef.h"

// Font Data Define
// Word Data를 Display하는 관련 함수의 경우 함수 Name에 "Word"가 들어감. 
#define	FONT_DATA							BYTE_DATA
#define BYTE_DATA							1
#define WORD_DATA							2

//------------------------------------------------------------------------

// LCD Command & Data Address 
#define HS_CMD_ADDR				0x2000000    /* A1 ADDR : LCD Command 출력 */
#define	LCD_REG_ADDRESS				( * (volatile byte*)( 0x2000000 ) )
#define	HS_DAT_ADDR				0x2000002    /* A1 ADDR : LCD Data    출력 */
#define	LCD_DATA_ADDRESS				( * (volatile byte*)( 0x2000002 ) )

#define	LCD_OUTP( Addr, Value )			outp ( (Addr), (Value) )
//#define	LCD_OUTP_CMD( Value )			LCD_OUTP ( HS_CMD_ADDR, (Value) )
//#define	LCD_OUTP_DATA( Value )			LCD_OUTP ( HS_DAT_ADDR, (Value) )
#define	LCD_OUTP_CMD( Value )			(LCD_REG_ADDRESS = Value)
#define	LCD_OUTP_DATA( Value )			(LCD_DATA_ADDRESS = Value)

/*--------------------------------------------------------------------------------*/ 	

// LCD IR(Index Register) Setting Values
#define	HS_LCD_R00				0x0000		/* Start Oscillation				*/
#define HS_LCD_R01				0x0001		/* Driver Output Control (Duty Set) */
#define HS_LCD_R02				0x0002		/* LCD Driving Waveform Control		*/
#define HS_LCD_R03				0x0003		/* Power Control					*/
#define HS_LCD_R04				0x0004		/* Contrast Control					*/
#define HS_LCD_R05				0x0005		/* Entry Mode						*/
#define HS_LCD_R06				0x0006		/* Rotation							*/
#define HS_LCD_R07				0x0007		/* Display Control					*/
#define HS_LCD_R08				0x0008		/* Cursor Control					*/ 
#define HS_LCD_R09				0x0009		/* Double-height Display Position	*/
#define HS_LCD_R0A				0x000a		/* Vertical Scroll					*/
#define HS_LCD_R0B				0x000b		/* Horizontal Cursor Position		*/
#define HS_LCD_R0C				0x000c		/* Vertical Cursor Position			*/
 
#define HS_LCD_R10				0x0010		/* RAM Write Data Mask				*/
#define HS_LCD_R11				0x0011		/* RAM Address Set					*/
#define HS_LCD_R12				0x0012		/* Ram Data Write & Read			*/

//#define	FONT_WIDTH				6					/* Col. Dot No. */
//#define	FONT_HEIGHT				3					/* Row  Dot No. */

/*--------------------------------------------------------------------------------*/ 	

// LCD Setting Data Definitions
#define HS_LCD_MASK				0xff
#define HS_LCD_INIT				0x0000

	// HS_LCD_R00							   xxxx xxxx xxxx xxx1 
#define HS_LCD_START_OSC		0x0001		/* Oscillation Start */

	// HS_LCD_R01							   xxxx xx__ x_xx ____	
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
#define HS_LCD_CONTRAST_D	0x0037						// Default Contrast Value 
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
#define	LCD_TOTAL_ADDRESS_X		16		/* X Address Range : 0x01 ~ 0x0f */
#define	LCD_TOTAL_ADDRESS_Y		96		/* Y Address Range : 0x01 ~ 0x60 */

#define HS_LCD_MAX_LINES		5			/* Number of lines on the display    */

/* LCD Dot Size */
#define	LCD_WIDTH			(LCD_TOTAL_ADDRESS_X - 1) * 8  /*  LCD Col. Dot No. : 120 */
#define	LCD_HEIGHT			LCD_TOTAL_ADDRESS_Y	           /*  LCD Row Dot No.  : 96  */


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
/*
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
*/
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

#ifdef FEATURE_NUI_A10
/*===========================================================================

EXTERN FUNCTION HS_LCD_RESET

DESCRIPTION
   
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_reset( void );	// inserted by kmh 00.05.15

#endif //FEATURE_NUI_A10
#endif // #if (MMI_TYPE == MMI_A10) // jwpark 00.08.23
#endif /* LCDA10_H */
