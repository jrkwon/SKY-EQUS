
/*****************************************************************************
 *
 *		LCD control module
 *
 *		Copyright (C) 1998, 1999, 2000 SK Teletech
 *
 *****************************************************************************
 *
 *		FileName : LCD_BIOS_M.h
 *		Created : 00.06.26		by tasik
 *
 *****************************************************************************/
#ifndef	__LCD_BIOSB1_HEADER
#define __LCD_BIOSB1_HEADER

/*****************************************************************************
 *		Include external header files
 *****************************************************************************/

#include "comdef.h"


/*****************************************************************************
 *	define LCD control parameter & constants
 *****************************************************************************/

// define model dependent constants
#include  "LCD_B1B1.h"

// define for draw mode (text)
#define	DRAW_NORMAL				0						// normal
#define	DRAW_FOREGROUND_BLINK	1						// 글자만 clear 됐다 나타남
#define	DRAW_BACKGROUND_BLINK	2						// 바탕만 fill 됐다 지워짐
#define	DRAW_REVERSE			3						// reverse
#define	DRAW_NORMAN_BLINK		0x81					// normal로 시작해서 반전
#define	DRAW_REVERSE_BLINK		0x82					// reverse로 시작해서 반전
#define	DRAW_BLINK				DRAW_FOREGROUND_BLINK

#define	NO_DISPLAY_LCD			0
#define	DISPLAY_LCD				1

#define	BLINK_TIME				(100/SY_ITV_TM)			// 100 ms
#define	BLINK_REVERSE_TIME		6						// 전체 16 에서 반전 mode 시간


/*****************************************************************************
 *	Global function prototypes
 *****************************************************************************/

// LCD buffer control functions
void DrawBitToLCDBuffer ( int indexX, int indexY,
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
void BlinkProcess ( void );

// basic LCD control functions
void InitialLCD ( void );
void LCDDisplayOn ( void );
void LCDDisplayOff ( void );

// LCD address control

void SetLCDAddress(byte,byte,byte,byte);
void SetLCDIconAddress(byte);


// LCD Data Convert 

#define LCD_CONVERT_BIT_TO_BYTE		// this definition is TEMPORARY!!.

#endif	// #ifndef	__LCD_BIOSB1_HEADER
