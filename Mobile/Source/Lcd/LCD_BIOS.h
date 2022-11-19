
/*****************************************************************************
 *
 *		LCD control module
 *
 *		Copyright (C) 1998, 1999, 2000 SK Teletech
 *
 *****************************************************************************
 *
 *		FileName : LCD_BIOS.h
 *		Revision : 0.00.001
 *		 Created : 00.02.29		by chos
 *
 *****************************************************************************
 *
 *		Revision detail
 *		---------------
 *
 *          Rev 0.00.002    00.04.09        jrkwon
 *              rename BLINK_TIMER to BLINK_TIME
 *			Rev	0.00.001	00.02.29 09:00	chos
 *				created this module
 *
 *****************************************************************************/

#ifndef	__LCD_BIOS_HEADER
	#define __LCD_BIOS_HEADER


/*****************************************************************************
 *		Include external header files
 *****************************************************************************/

#include "comdef.h"


/*****************************************************************************
 *	define LCD control parameter & constants
 *****************************************************************************/

// define model dependent constants
#if (MODEL_ID == MODEL_D1)
	#include	"LCD_D1.h"

#elif (MODEL_ID==MODEL_A10)
	#include	"LCD_A10.h"

#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
    #include  "LCD_B1.h"

#endif	// #if MODEL_ID ==


#if (MODEL_ID != MODEL_A10)

// for MSM GPIO interface
#include "bio.h"
#define	PORT_HIGH( PortID, BitMask )	\
			BIO_OUT( PortID, BitMask, 0xffff )
#define	PORT_LOW( PortID, BitMask )	\
			BIO_OUT( PortID, BitMask, 0x0000 )

#define	PORT_OUTPUT_MODE( PortID, BitMask )	\
			BIO_TRISTATE( PortID, BitMask, BitMask )

// define for draw mode (text)
#define	DRAW_NORMAL				0		// normal
#define	DRAW_FOREGROUND_BLINK	1		// 글자만 clear 됐다 나타남
#define	DRAW_BACKGROUND_BLINK	2		// 바탕만 fill 됐다 지워짐
#define	DRAW_REVERSE			3		// reverse
#define	DRAW_NORMAN_BLINK		0x81	// normal로 시작해서 반전
#define	DRAW_REVERSE_BLINK		0x82	// reverse로 시작해서 반전
#define	DRAW_BLINK				DRAW_FOREGROUND_BLINK

#define	NO_DISPLAY_LCD			0
#define	DISPLAY_LCD				1

#define	BLINK_TIME				(100/SY_ITV_TM)	// 100 ms
//#define	BLINK_TIMER				(100/SY_ITV_TM)	// 100 ms
#define	BLINK_REVERSE_TIME		6		// 전체 16 에서 반전 mode 시간


/*****************************************************************************
 *	Global function prototypes
 *****************************************************************************/

// LCD buffer control functions
void DrawBitToLCDBuffer ( int nIndexX, int nIndexY,
			byte DrawPattern, byte MaskBitPattern,
			int nDrawMode );
void DisplayLCDBufferAll ( void );
void ClearLCDBufferAll ( void );
void ClearLCDDataBufferAll ( void );
void ClearLCDIconBufferAll ( void );
void FillLCDBufferAll ( void );
void BeginDraw ( void );
void EndDraw ( void );
void BeginDrawWait ( void );

// LCD blinking control functions
void LCDBlinkTimerStart ( void );
void LCDBlinkTimerStop ( void );
void LCDBlinkON ( void );
void LCDBlinkOFF ( void );
byte GetBlinkTimerID ( void );
void BlinkProcess ( void );

// basic LCD control functions
void InitialLCD ( void );
void LCDDisplayOn ( void );
void LCDDisplayOff ( void );

#endif
#endif	// #ifndef	__LCD_BIOS_HEADER
