//==============================================================================
//
//	SINJISOFT GAME MOBILE INTERFACE
//
//	FILE NAME
//		GExt.h
//
//	DESCRIPTION
//		Mobile interface routines header
//
//	REFERENCES
//		SINJISOFT Game Porting Guide
//
//	Copyright (C) 1999 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		1999.11. 0	Release Version 0.9
//		1999.12.21	Release Version 1.0
//		2000. 1. 7  Release Version 1.1
//		2000. 2.22  Release Version 1.2
//		2000.10. 8  Release Version 2.0
//
//==============================================================================
#include "ska.h"

#ifndef	_GEXT_H_
#define	_GEXT_H_

//==========================================================================================
//			PROJECT CODE (Product Code)
//==========================================================================================
#define SKTT5 			// 64K Color  	120*160		MA2 
//===========================================
//	MAIN MENU LANGUAGE CODE
//  This definition is used in GLibMain.cpp
//===========================================
#define KOREAN
//==============================================================================
//
//	Global type definition
//
//==============================================================================
/*
typedef unsigned char	BYTE;			// Unsigned  8 bit value type
typedef unsigned short	WORD;			// Unsinged 16 bit value type
typedef unsigned long	DWORD;			// Unsinged 32 bit value type
*/
typedef unsigned char	UINT1;			// Unsigned  8 bit value type
typedef unsigned short	UINT2;			// Unsinged 16 bit value type
typedef unsigned long	UINT4;			// Unsinged 32 bit value type

typedef signed char		INT1;			// Signed  8 bit value type
typedef signed short	INT2;			// Signed 16 bit value type
typedef signed long		INT4;			// Signed 32 bit value type

#ifndef T_ARM
#define	__packed
#endif // T_ARM

//==============================================================================
//
//	Handset HW Profile
//
//==============================================================================

//==============================================================
//	LCD type
//
	#define	SWAP_LCD_BW_HORIZ_MF		0x01
	// #define	SWAP_LCD_BW_HORIZ_LF		0x02
	#define	SWAP_LCD_BW_VERT_MF			0x03
	#define	SWAP_LCD_BW_VERT_LF			0x04
	#define SWAP_LCD_BW_END				0x0F

	#define	SWAP_LCD_GRAY_VERT_MF		0x11
	// #define	SWAP_LCD_GRAY_VERT_LF		0x12
	#define	SWAP_LCD_GRAY_HORIZ_MF		0x13
	#define	SWAP_LCD_GRAY_HORIZ_LF		0x14
	#define	SWAP_LCD_GRAY_MP_HORIZ_MF	0x15
	// #define	SWAP_LCD_GRAY_MP_HORIZ_LF	0x16
	#define	SWAP_LCD_GRAY_MP_VERT_MF	0x17
	#define	SWAP_LCD_GRAY_MP_VERT_LF	0x18
	#define	SWAP_LCD_GRAY_VM_HORIZ_MF	0x19
	#define	SWAP_LCD_GRAY_VM_HORIZ_LF	0x1A
	#define	SWAP_LCD_GRAY_HW_HORIZ_LF	0x1B
	#define SWAP_LCD_GRAY_END			0x1F

	#define SWAP_LCD_COLOR16_HORIZ_MF	0x21
	#define SWAP_LCD_COLOR16_HORIZ_LF	0x22
	#define SWAP_LCD_COLOR256_HORIZ		0x23
	#define SWAP_LCD_COLOR_END			0x2F

	#define SWAP_LCD_COLOR4K_HORIZ		0x30
	#define SWAP_LCD_WORDCOLOR_START	0x31
	#define SWAP_LCD_COLOR4K_HORIZ_WORD	0x32
	#define SWAP_LCD_COLOR64K_HORIZ		0x33
	#define SWAP_LCD_TCCOLOR_END		0x3F

#define	SWAP_LCD_TYPE			SWAP_LCD_COLOR256_HORIZ

#if	(SWAP_LCD_TYPE < SWAP_LCD_BW_END)
	#define	SWAP_COLOR_TYPE			0x01
	#define	SWAP_LCD_BW
#elif (SWAP_LCD_TYPE < SWAP_LCD_GRAY_END)
	#define	SWAP_COLOR_TYPE			0x02
	#define	SWAP_LCD_GRAY
#elif (SWAP_LCD_TYPE < SWAP_LCD_COLOR_END)
	#define	SWAP_COLOR_TYPE			0x04
	#define SWAP_LCD_COLOR
#elif (SWAP_LCD_TYPE < SWAP_LCD_TCCOLOR_END)
	#define	SWAP_COLOR_TYPE			0x08
	#define SWAP_LCD_COLOR
#endif

//==============================================================
//	LCD resolution & class
//
#define	SWAP_LCD_WIDTH			128
#define	SWAP_LCD_HEIGHT			160

	#define SWAP_LCD_CLASS_0		0x01	//  96x 64 ~ 120x 80 미만
	#define SWAP_LCD_CLASS_1		0x02	// 120x 80 ~ 128x128 미만
	#define SWAP_LCD_CLASS_2		0x04	// 128x128 ~ 176x176 미만
	#define SWAP_LCD_CLASS_3		0x08	// 176x176 ~ 256x256 미만

#if (SWAP_LCD_WIDTH < 120 || SWAP_LCD_HEIGHT < 80)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_0
#elif (SWAP_LCD_WIDTH < 128 || SWAP_LCD_HEIGHT < 128)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_1
#elif (SWAP_LCD_WIDTH < 176 || SWAP_LCD_HEIGHT < 176)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_2
#else // if (SWAP_LCD_WIDTH < 256 || SWAP_LCD_HEIGHT < 256)
	#define SWAP_LCD_CLASS			SWAP_LCD_CLASS_3
#endif

//==============================================================
//	Audio type
//
	#define	SWAP_AUDIO_BUZZER		0x00
	#define	SWAP_AUDIO_MA1			0x01
	#define	SWAP_AUDIO_MA2			0x02
	#define	SWAP_AUDIO_CMX			0x03

#define	SWAP_AUDIO_TYPE			SWAP_AUDIO_MA2
//======================================
// Language select
//======================================

	#define	SWAP_ASCII				0x01
	#define	SWAP_KSC5601			0x02
	#define	SWAP_SHIFTJIS			0x03

#define	SWAP_LANG				SWAP_ASCII

//======================================
// Game info
//======================================

// Number of games
#define	G_MAX_GAME_SIZE			5

// Number of score save data size
#define	G_SCOREBOARD_SIZE		5

//==============================================================
//	LCD buffer size
//
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_HEIGHT + 7)/8)*SWAP_LCD_WIDTH)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_VERT_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_HEIGHT + 7)/8)*2*SWAP_LCD_WIDTH)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 3)/4)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*2*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 7)/8)*SWAP_LCD_HEIGHT*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*((SWAP_LCD_HEIGHT + 7)/8)*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 3)/4)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_COLOR16_HORIZ_LF)
	#define	S_LCD_BUFFER				(((SWAP_LCD_WIDTH + 1)/2)*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR256_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR4K_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*3/2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR4K_HORIZ_WORD)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*2)
#elif (SWAP_LCD_TYPE == SWAP_LCD_COLOR64K_HORIZ)
	#define	S_LCD_BUFFER				(SWAP_LCD_WIDTH*SWAP_LCD_HEIGHT*2)
#endif // SWAP_LCD_TYPE


//======================================
//	KEY PRESS DEFINITION
//		Definition of first parameter
//		of glib_KeyPress function
//======================================

#define	GLIB_KEY_1				0x01
#define	GLIB_KEY_2				0x02
#define	GLIB_KEY_3				0x03
#define	GLIB_KEY_4				0x04
#define	GLIB_KEY_5				0x05
#define	GLIB_KEY_6				0x06
#define	GLIB_KEY_7				0x07
#define	GLIB_KEY_8				0x08
#define	GLIB_KEY_9				0x09
#define	GLIB_KEY_0				0x0A

#define	GLIB_KEY_STAR			0x0B
#define	GLIB_KEY_SHARP			0x0C

#define	GLIB_KEY_1_L			0x0D
#define	GLIB_KEY_2_L			0x0E
#define	GLIB_KEY_3_L			0x0F
#define	GLIB_KEY_4_L			0x10
#define	GLIB_KEY_5_L			0x11
#define	GLIB_KEY_6_L			0x12
#define	GLIB_KEY_7_L			0x13
#define	GLIB_KEY_8_L			0x14
#define	GLIB_KEY_9_L			0x15
#define	GLIB_KEY_0_L			0x16

#define	GLIB_KEY_CLR			0x17
#define	GLIB_KEY_SND			0x18
#define	GLIB_KEY_END			0x19

#define	GLIB_KEY_LEFT			0x1A
#define	GLIB_KEY_RIGHT			0x1B
#define	GLIB_KEY_UP				0x1C
#define	GLIB_KEY_DOWN			0x1D
#define	GLIB_KEY_OK				0x1E

//======================================
//	TONE DEFINITION
//		Definition of tone table value
//======================================

typedef	enum {
	SWAP_NOTE_G3,	// 0
	SWAP_NOTE_G3P,
    SWAP_NOTE_A3,
    SWAP_NOTE_A3P,
    SWAP_NOTE_B3,
    SWAP_NOTE_C4,
    SWAP_NOTE_C4P,
    SWAP_NOTE_D4,
    SWAP_NOTE_D4P,
    SWAP_NOTE_E4,
    SWAP_NOTE_F4,	// 10
    SWAP_NOTE_F4P,
    SWAP_NOTE_G4,
    SWAP_NOTE_G4P,
    SWAP_NOTE_A4,
    SWAP_NOTE_A4P,
    SWAP_NOTE_B4,
    SWAP_NOTE_C5,
    SWAP_NOTE_C5P,
    SWAP_NOTE_D5,
    SWAP_NOTE_D5P,	// 20
    SWAP_NOTE_E5,
    SWAP_NOTE_F5,
    SWAP_NOTE_F5P,
    SWAP_NOTE_G5,
    SWAP_NOTE_G5P,
    SWAP_NOTE_A5,
    SWAP_NOTE_A5P,
    SWAP_NOTE_B5,
    SWAP_NOTE_C6,
    SWAP_NOTE_C6P,	// 30
    SWAP_NOTE_D6,
    SWAP_NOTE_D6P,
    SWAP_NOTE_E6,
    SWAP_NOTE_F6,
    SWAP_NOTE_F6P,
    SWAP_NOTE_G6,
	SWAP_NOTE_R,
	SWAP_NOTE_END	// 38: End of tone table
} S_NoteDef;

//======================================
//	TONE TABLE STRUCTURE
//		Tone table for game melody
//======================================

typedef	__packed struct _S_NoteType	{
	BYTE	Note;						// Tone height, SWAP_NOTEDef type
	BYTE	Milisec3;					// Tone duration, 3 milisecond unit
} S_NoteType;

//==============================================================================
//	Game main function prototype
//==============================================================================
#if defined (SKTT5)
extern  int		Apache5Main(int nReason, int nData);		//G80 Apache5		120*160 SKTT5
extern  int		Raccoon3Main(int nReason, int nData);		//G81 Reccoon2		120*160 SKTT5
extern  int		MarsScouter3Main(int nReason, int nData);	//G82 MarsScouter2	120*160 SKTT5
extern  int		Samtris3Main(int nReason, int nData);		//G83 Samtris2		120*160 SKTT5
extern  int		Puzzle3Main(int nReason, int nData);		//G84 Puzzle2		120*160 SKTT5
#endif	//#ifdef SKTT5

//==============================================================================
//	MOBILE SIDE API
//		Defintion of mobile side API
//==============================================================================

extern	void	gext_InitMemory(void);
extern	void	gext_CloseMemory(void);
extern	void	gext_DrawLcd(const char* pLcdBuf);
extern	void	gext_PlaySound(const BYTE *pData, const int nSize);
extern	void	gext_PlaySound_background(const BYTE *pData, const int nSize);
extern	void	gext_StopSound(void);
extern	void	gext_StartVib(const int nSec);
extern	void	gext_StopVib(void);
extern	void	gext_StartTimer(const int nTid, const int nMsec, const int nRepeat);
extern	void	gext_StopTimer(const int nTid);
extern	void	gext_GetNV(const int nGid, BYTE* const pData);
extern	void	gext_PutNV(const int nGid, const BYTE* pData);
extern	void	gext_GetDate(int* year, int* month, int* day, int* hour, int* min, int* sec);
extern	void	gext_RandomSeed(const int nSeed);
extern	int		gext_Random(const int nBegin, const int nEnd);
extern	void	gext_OnKeyTone(void);
extern	void	gext_OffKeyTone(void);

//==============================================================================
//	GEMUL SIDE API
//		Defintion of Emulator side API
//==============================================================================

extern	void	glib_Init_SINJISOFT_Game(void);
extern	int		glib_GameEvent(int state, int data);

#endif	// _GEXT_H_
