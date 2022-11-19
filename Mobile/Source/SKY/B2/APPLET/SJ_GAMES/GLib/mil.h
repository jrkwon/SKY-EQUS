//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY HEADER
//
//	FILE NAME
//		MIL.h
//
//	DESCRIPTION
//		Mobile Interface Library Graphic Part
//
//	REFERENCES
//		SINJISOFT SWAP Porting Guide
//
//	Copyright (C) 2000, 2001 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		2000. 5.20  Alpha Release
//		2000. 6.10  Beta Release
//		2000. 7. 3  Beta2 Release
//		2000. 7.24  Beta3 Release
//		2000. 8. 7  Version 1.0 Release
//		2000. 8.28  Version 1.01 Release
//		2000. 9.26  Version 1.02 Release
//		2000.10.27  Version 1.10 Release
//		2000.12.19	Version 1.5 Beta Start
//		2001. 1. 1	Version 1.5 Release
//		2001.10.12 	Version 1.55.01 Beta Release (True Color)
//		2001.10.17	Version 1.55.01 Release
//
//==============================================================================

#ifndef	_MIL_H_
#define	_MIL_H_

//==============================================================================
//
//	Definitions
//
//==============================================================================

//==============================================================
//	MIL Parameter Definitions
//

#define	S_FONT_SMALL				0		// Font size
#define	S_FONT_MEDIUM				1
#define	S_FONT_LARGE				2
#define	S_FONT_DOUBLE				3
#define	S_MAX_FONT					4		// Kind of fonts

#define	S_LEFT						0		// Font alignment
#define	S_CENTER					1
#define	S_RIGHT						2
#define	S_MAX_ALIGN					3		// Kind of align

#define S_DRAWSTR2_BOLD			1
#define S_DRAWSTR2_ITALIC			2
#define S_DRAWSTR2_UNDERLINE		4

#define S_SCROLL_BUFSIZE			256			// Max LCD width, for scroll buffer

//==============================================================
//	Image Type Info Definitions
//

// VDI image type
#define S_IMG_GRAY1					2
#define S_IMG_GRAY2					3
#define S_IMG_GRAY4					4

#define S_IMG_COLOR1				5
#define S_IMG_COLOR2				6
#define S_IMG_COLOR4				7
#define S_IMG_COLOR8				8

#define S_IMG_TC1				    9
#define S_IMG_TC2				    10
#define S_IMG_TC4				    11
#define S_IMG_TC8				    12

// VDI image field access function
#define	VDI_TYPE(img)				(*((img) + 0))			// Get image type
#define	VDI_WIDTH(img)				(*((img) + 1))			// Get image width
#define	VDI_HEIGHT(img)				(*((img) + 2))			// Get image height
#define	VDI_CX(img)					((INT1)*((img) + 3))	// Get image hot-spot x
#define	VDI_CY(img)					((INT1)*((img) + 4))	// Get image hot-spot y

#define	VDI_PAL_PTR(img)			((img) + 5)				// Gray/256 image palette address
#define	VDI_TRFLAG(img)				(*((img) + 5))			// Truecolor image transparent flag
#define	VDI_PALSIZE(img)			(*((img) + 6))			// Truecolor image palette size
#define	VDI_TCPAL_PTR(img)			((img) + 7)				// Truecolor image palette address

#define	VDI_BITMAP_GRAY1(img)		((img) + 6)				// Bitmap address of S_IMG_GRAY1
#define	VDI_BITMAP_GRAY2(img)		((img) + 7)				// Bitmap address of S_IMG_GRAY2
#define	VDI_BITMAP_GRAY4(img)		((img) + 5)				// Bitmap address of S_IMG_GRAY4

#define	VDI_BITMAP_COLOR1(img)		((img) + 7)				// Bitmap address of S_IMG_COLOR1
#define	VDI_BITMAP_COLOR2(img)		((img) + 9)				// Bitmap address of S_IMG_COLOR2
#define	VDI_BITMAP_COLOR4(img)		((img) + 21)			// Bitmap address of S_IMG_COLOR4
#define	VDI_BITMAP_COLOR8(img)		((img) + 5)				// Bitmap address of S_IMG_COLOR8

#define	VDI_BITMAP_TC1(img)			((img) + 11)			// Bitmap address of S_IMG_TC1
#define	VDI_BITMAP_TC2(img)			((img) + 15)			// Bitmap address of S_IMG_TC2
#define	VDI_BITMAP_TC(img)			((img) + 7 + VDI_PALSIZE(img)*3)

/*

	typedef __packed struct s_TC_Palette {
		BYTE transarent_flag;		// 0=no transparent
		BYTE pal_size;				// 2~256
		BYTE palette[n];			// RGB Palette data
	} TC_Palette;

	typedef	__packed struct _VDI_Header {
		BYTE type;					// 1bit, 2bit, 4bit, 8bit
		BYTE w, h;					// Width, height
		INT1 cx, cy;				// Hot spot
		union	{
			BYTE		palette[n];	// Palette data
			TC_Palette	tp;			// True color palette
		};
	} VDI_Header;

*/

//==============================================================
//	Color Definitions
//

#define S_GRAY0						0x00		// White
#define S_GRAY1						0x01		// Light Gray
#define S_GRAY2						0x02		// Dark Gray
#define S_GRAY3						0x03		// Black
#define S_TRANSPARENT				0x04		// Transparent color

#define S_WHITE						S_GRAY0
#define S_BLACK						S_GRAY3

#ifdef SWAP_LCD_COLOR
#define	S_MAX_COLOR					182
#else // SWAP_LCD_BW | SWAP_LCD_GRAY
#define	S_MAX_COLOR					16
#endif // SWAP_LCD_COLOR

//==============================================================================
//
//	Type Defines 
//	jimylee
//
//==============================================================================
typedef BYTE s_ImgBlock;

//==============================================================================
//
//	External MIL Functions
//
//==============================================================================

extern	void	mil_InitLcd(int, int, BYTE*, BYTE*);
extern	int		mil_GetLcdBufferSize(void);

extern  void    mil_InitDQ(void);
extern	int		mil_AddDQ(BYTE*, int, int, int);
extern  void    mil_DrawDQ(int);

extern	void	mil_ClearWhite(void);
extern	void	mil_ClearBlack(void);
extern	void	mil_Clear(int);

extern  void    mil_SetColor(int);
extern  void	mil_SetBlinkColor(int);

extern	void	mil_PutPixel(int, int, int);
extern	void	mil_DrawStr(int, int, char*, int, int, int, int, int);

#define	mil_DrawString(x, y, str, f, fc, a)				mil_DrawStr(x, y, str, f, fc, 0, a, 0)
#define	mil_DrawStringSolid(x, y, str, f, fc, bc, a)	mil_DrawStr(x, y, str, f, fc, bc, a, 1)

extern  void    mil_DrawLine(int, int, int, int);
extern  void    mil_DrawLineH(int, int, int);
extern  void    mil_DrawLineV(int, int, int);
extern  void    mil_DrawRect(int, int, int, int);
extern  void    mil_FillRect(int, int, int, int);
extern  void    mil_DrawEllipse(int, int, int, int);
extern	void	mil_FillEllipse(int, int, int, int);

extern  void    mil_CopyImage(BYTE*, int, int);
extern  void    mil_CopyImageDir(BYTE*, int, int, int);
extern  void    mil_CopyImagePal(BYTE*, int, int, BYTE*);
extern  void    mil_CopyImageDirPal(BYTE*, int, int, int, BYTE*);

extern  void    mil_SetGamma(int);
extern	void	mil_SaveGamma(void);
extern	void	mil_RestoreGamma(void);

extern	void	mil_SaveLCD(void);
extern	void	mil_RestoreLCD(void);

//======================================
// Additional libraries
// on SWAP version 1.5
//======================================

extern	void	mil_SetClip(int x1, int y1, int x2, int y2);
extern	void	mil_ResetClip(void);

extern	int		mil_GetPixel(int x, int y);
extern  void    mil_DrawRectRound(int x1, int y1, int x2, int y2, int r);
extern  void    mil_FillRectRound(int x1, int y1, int x2, int y2, int r);
extern	void	mil_InvertRect(int x1, int y1, int x2, int y2);

extern	void	mil_ScrollLCD(int lcd, int dx, int dy, int wrap);

extern	void	mil_DrawStr2(int, int, char*, int, int, int, int, int, int);

#define	mil_DrawString2(x, y, str, f, fc, a, m)				mil_DrawStr2(x, y, str, f, fc, 0, a, m, 0)
#define	mil_DrawStringSolid2(x, y, str, f, fc, bc, a, m)	mil_DrawStr2(x, y, str, f, fc, bc, a, m, 1)

#endif // _MIL_H_
