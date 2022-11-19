#ifndef LCD_A7_H
#define LCD_A7_H

//************************************************************************
//
//							 L C D CONTROLLER DECLARES
//
//	File Name   : Lcd_a7.h
//  Description : Support Decalares of LCD Controller
//	Author		  : SK Teletech Software Team
//
//************************************************************************
//
//		 EDIT HISTORY FOR MODULE
//
//  This section contains comments describing changes made to this file.
//  Notice that changes are listed in reverse chronological order.
//
//
//	when       who      what, where, why
//--------     ---      ---------------------------------------------------
// 02/16/00	   kmh      hs_lcd_sleep 추가
// 02/23/00    kmh      hs_lcd_blink 추가						
//
//
//*************************************************************************

#include "hs.h"
#include "lcddef_a7.h"

#if (MMI_TYPE == MMI_SKME)
#include "lcd_a7_skme.h"
#else // MMI_QUALCOMM || MMI_SK
/*-----------------------------------------------------------------------
                           Extern LCD data
-----------------------------------------------------------------------*/
/* Screen data
*/

extern hs_scrn_type hs_screen;					/* non-blink screen contents      */
extern hs_scrn_type hs_blink_screen;		/* blink screen contents          */
                                        /* Current screen contents        */
extern byte *hs_crnt_screen_ptr;

extern word hs_annun;										/* non-blink annunciators         */
extern word hs_blink_annun;							/* Blink annunciators             */

extern byte hs_rssi;										/* Non-blink RSSI level           */
extern byte hs_blink_rssi;							/* Blink RSSI level               */

extern byte hs_batt;										/* Non-blink Battery level        */
extern byte hs_blink_batt;							/* Blink Battery level            */

extern byte *hs_crnt_sni_ptr;					/* Current SNI level             */
extern byte *hs_crnt_rssi_ptr;					/* Current RSSI level             */
extern byte *hs_crnt_batt_ptr;					/* Current Battery level          */
extern word *hs_crnt_annun_ptr;					/* Current annunciators           */

extern word hs_blink_rate;							/* Rate for blinking, ms / image  */
extern word hs_blink_active;						/* Is blink currently displayed   */

extern boolean hs_backlight;						/* Backlight is currently on      */




/*-----------------------------------------------------------------------
                          Macro for LCD
-----------------------------------------------------------------------*/

/***********************************************************************
 *	A7, A10
 ***********************************************************************/
#if ( MODEL_ID == MODEL_A7 ) || ( MODEL_ID == MODEL_A7A ) || ( MODEL_ID == MODEL_A10 )

#define	LCD_OUTP( Addr, Value )											\
			outp ( (Addr), (Value) )

#define	LCD_OUTP_CMD( Value )											\
			outp ( HS_CMD_ADDR, (Value) )

#define	LCD_OUTP_DATA( Value )											\
			outp ( HS_DAT_ADDR, (Value) )

/***********************************************************************
 *	A8
 ***********************************************************************/
#elif ( MODEL_ID == MODEL_A8 )

#define	LCD_OUTP_CMD( Value )											\
			INTLOCK ();													\
			outpw ( GPIO_INT_OUT_1_WH,									\
				( bio_gpio[ MSM31_GPIO_1 ].out_buf &					\
				~( 0x7f80 | BIO_LCD_A0_HI_V|BIO_LCD_DESEL1_V) ) |		\
				( (Value) << 7 ) );										\
			outp ( HS_CMD_ADDR, (Value) );								\
			INTFREE ()

#define	LCD_OUTP_DATA( Value )											\
			INTLOCK ();													\
			outpw ( GPIO_INT_OUT_1_WH,									\
				( bio_gpio[ MSM31_GPIO_1 ].out_buf &					\
				~( 0x7f80 | BIO_LCD_A0_HI_V|BIO_LCD_DESEL1_V ) ) |		\
				( (Value) << 7 | BIO_LCD_A0_HI_V) );					\
			outp ( HS_DAT_ADDR, (Value) );								\
			INTFREE ()

/***********************************************************************
 *	B1
 ***********************************************************************/
#elif ( MODEL_ID == MODEL_B1 )
//#define	B1_COLOR_LCD	임시로 지운 것 00.06.01
#ifdef B1_COLOR_LCD	
#if 0 
		#define	LCD_OUTP_CMD( Value )										\
				INTLOCK ();													\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_A01_LO_V | BIO_LCD_WR_LO_V ) );				\
				outp ( HS_CMD_ADDR , (Value) );								\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_WR_HI_V ) );				\
				INTFREE ()

		#define	LCD_OUTP_DATA( Value )										\
				INTLOCK ();													\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_A01_HI_V | BIO_LCD_WR_LO_V ) );				\
				outp ( HS_DAT_ADDR , (Value) );								\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_WR_HI_V ) );				\
				INTFREE ()
#else
		#define	LCD_OUTP_CMD( Value )										\
				outp ( HS_CMD_ADDR , (Value) );								

		#define	LCD_OUTP_DATA( Value )										\
				outp ( HS_DAT_ADDR , (Value) );								
#endif
	#else
		#define	LCD_OUTP_CMD( Value )										\
				INTLOCK ();													\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_A01_LO_V | BIO_LCD_WR_LO_V ) );				\
				outp ( HS_CMD_ADDR , (Value) );								\
				INTFREE ()

		#define	LCD_OUTP_DATA( Value )										\
				INTLOCK ();													\
				outpw ( GPIO_INT_OUT_2_WH,									\
					( bio_gpio[ MSM31_GPIO_2 ].out_buf &					\
					~( BIO_LCD_A01_M |BIO_LCD_WR_M ) ) |					\
					( BIO_LCD_A01_HI_V | BIO_LCD_WR_LO_V ) );				\
				outp ( HS_DAT_ADDR , (Value) );								\
				INTFREE ()
	#endif	// #ifdef B1_COLOR_LCD
#endif	// #if ( MODEL_ID == )

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
extern void hs_lcd_init( void );


/*===========================================================================

EXTERN FUNCTION HS_LCD_SLEEP		// kmh 00.02.16

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_sleep ( boolean bSleep );


/*===========================================================================

EXTERN FUNCTION HS_LCD_BLINK		// kmh 00.02.23

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void hs_lcd_blink ( int nPosX, int nPosY, int nWidth, int nHeight, int nBlinkMode );


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
extern void hs_lcd_contrast ( word contrast  /* Setting the value of LCD Contrast */ );


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
#if (MMI_TYPE == MMI_SK)
extern void hs_update_screen(byte Command, byte Attr);
#else
extern void hs_update_screen( void );
#endif


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
extern void	TextOutXY ( byte *strText, byte *oldText, int nPosX, int nPosY,	int nDrawMode );

/******************************************************************************
	ADDED FOR MMI_SK
******************************************************************************/

#if ( MMI_TYPE == MMI_QUALCOMM ) 
extern byte LCDDataBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];
extern byte LCDAniBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X];
extern byte LCDAniBlinkBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X];

#elif ( MMI_TYPE == MMI_SK )
extern byte LCDWordAniBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2];
extern byte LCDWordAniBlinkBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2 ];

#else
#error Invalid MMI_TYPE
#endif

void BuildLCDBufferAll(void);
void CopyLCDBufferAll (void);
void DisplayLCDBufferEnhanced(void); // girin. 2000.3.27. For speeding-up the display refresh.
void ClearLCDBufferAll(void);
void BlinkLCDDataBuffer ( void );
int	SetHanFont ( int nFont );
void DisplayLCDBufferLine0(void);

#if ( MMI_TYPE == MMI_SK )
void lcd_SetDispType(hs_scrn_type scrn_ptr, byte PageType, byte WallpaperType, byte WallMaskType, byte DrawMethod);
byte lcd_GetPageType(void);
byte lcd_GetWallType(void);
byte lcd_GetWallMask(void);
byte lcd_GetDrawMethod(void);

#define CURRENT_BLINK	0x00
#define ANI_BLINK		0x01
#define TEXT_BLINK		0x02
void LCDBlinkTimerStart(byte Type, dword tick); // tick in miliseconds
void LCDBlinkTimerStop(void);

#endif

#else		// #if FONT_DATA == BYTE_DATA
/*===========================================================================

FUNCTION TextWordOutXY

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void	TextWordOutXY ( byte *strText, byte *oldText, int nPosX, int nPosY,	int nDrawMode );


#endif		// #if FONT_DATA == BYTE_DATA

/*===========================================================================

LOCAL FUNCTION DisplayLCDBufferAll

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void  DisplayLCDBufferAll ( void );

/*===========================================================================

EXTERN FUNCTION LCD_TEST

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void hs_lcd_test( void );

#endif // MMI_QUALCOMM || MMI_SK

#endif	// #ifndef LCD_A7_H
