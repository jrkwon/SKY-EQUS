//************************************************************************
//			
//					L C D		C O N T R O L L E R
//
//	File Name      : Lcd_a7.c
//  Description    : These Functions provide an LCD interface between the 
//				     handset(hsg.c) and the other software tasks, 
//					 so displays information on the handset display.
//	LCD Controller : HD66750 
//	Author		   : SK Teletech Software Team
//
//************************************************************************
//
//					EDIT HISTORY FOR MODULE
//
//  This section contains comments describing changes made to this file.
//  Notice that changes are listed in reverse chronological order.
//
//
//	when       who      what, where, why
//--------     ---      ---------------------------------------------------
// 08/23/00    kmh      LCD의 Blink와 Sleep 상태를 저장하여 RESET시 적용함. 
// 08/22/00    kmh      hs_lcd_reset시 모든 LCD Register 초기화함.
// 06/29/00    kmh      A8 Project -> lcd 초기화시 Waveform AC Drive 변경 
// 06/05/00    kmh      hs_lcd_reset에서 catnap 구간에서 화면 update시 
//                      전류 많이 소모되는 것을 방지하기 위해 Enable 부분 삭제함.
// 05/16/00    kmh      LCD Display시 reset에서 기본적인 Register만 Reset하고
//                      Off line Charging시에는 Lcd reset을 하지 않는다.  
// 05/15/00    kmh      LCD 폭주 문제로 인하여 LCD display시마다 LCD reset을 
//                      해주고 PM1000의 MSMP 전원이 낮아지는 현상을 대비해 
//                      pm1000_init()의 전원 Setting 부분만 실행해 줌
// 03/28/00    kmh      LCD Display 속도 개선
// 02/23/00    kmh      hs_lcd_blink 추가
// 02/16/00	   kmh      hs_lcd_sleep 추가
// 01/20/00	   kmh      New Lcd Spec에 맞추어 hs_lcd_init() 수정함.
// 10/22/99    kmh      Create file for supporting A7 Project
//
//
//*************************************************************************

#include "customer.h"
#include "lcd_a10.h"

#include "hs.h"
#include "hsig.h"
#include "target.h"
#include "clk.h"
#include "err.h"
#include "bio.h"
#include "string.h"
#include "biog.h"
#include <font.h>
#include <hanfont.h>

// added by chul
#include "nv.h"
#include "uixscrn.h"
#include "uistate.h"

#if MMI_TYPE == MMI_A10
#include "..\..\SMB\smb.h"
#include "..\..\SMB\smbdev.h"
#include "bq2018.h"
#endif

#include <PageType.h> // girin. 2000.1.27
#include "db.h"

#include <uiDummy.h>

static byte byAniTypeSav = ANI_TYPE_MAX;

// for LCD test by msrock
extern byte LcdTestFlag;
#ifdef SW_RESET_ENABLE
extern	boolean	mc_bSwReset;
extern  byte IconBuffer[16][LCD_TOTAL_ADDRESS_X];
#endif // SW_RESET_ENABLE

extern unsigned char  uia_PlayAnimation(unsigned char nCommand, unsigned char nAttr);	// woodstok 00.5.2
/*-----------------------------------------------------------------------
												Function Definitions 
-----------------------------------------------------------------------*/

int	  SetASCIIFont ( int nFont );
void hs_BootLogoDisplay(void);


/*-----------------------------------------------------------------------
													Extern LCD data
-----------------------------------------------------------------------*/
/* Screen Data
*/

hs_scrn_type hs_screen;              /* non-blink screen contents      */
hs_scrn_type hs_blink_screen;        /* blink screen contents          */
                                     /* Current screen contents        */
byte *hs_crnt_screen_ptr = hs_screen;

word hs_blink_rate = 0;              /* Rate for blinking, ms / image  */
word hs_blink_active = FALSE;        /* Is blink currently displayed   */

boolean hs_backlight = FALSE;        /* Backlight is currently on      */

#if MMI_TYPE == MMI_A10
hs_backcolor_type hs_backcolor = HS_BACKCOLOR_WHITE;

#ifdef FEATURE_MEM_FONT
// chojw 00.10.30
word SmbStrBuf[16 * 12];
#endif // FEATURE_MEM_FONT

#endif

struct robot { 
                 unsigned int  m16     : 1;   
                 unsigned int  dagger  : 14; 
                 unsigned int  missile : 3; 
             } mazinga; 

/*-----------------------------------------------------------------------
													Local LCD data
-----------------------------------------------------------------------*/

LOCAL hs_scrn_type hs_lcd_screen;          /* LCD screen contents            */
                                           /* Current screen contents        */
/* This array is used to derive a physical address from x,y coords
*/
LOCAL const word hs_lcd_row_address[ HS_LCD_MAX_LINES ] = {
  HS_LCD_LINE0_ADDR,    /* address command for 1st line */
  HS_LCD_LINE1_ADDR,    /* address command for 2nd line */
  HS_LCD_LINE2_ADDR,    /* address command for 3rd line */
  HS_LCD_LINE3_ADDR     /* address command for 4th line */
};
 
/*-----------------------------------------------------------------------
					Font Data Definition
-----------------------------------------------------------------------*/

// LCD Display Buffer Definition :
#if (MMI_TYPE == MMI_QUALCOMM)
byte LCDDataBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ];

byte LCDAniBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ] = {0, };
byte LCDAniBlinkBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X ] = {0. };
#elif (MMI_TYPE == MMI_A10)
byte	LCDWordDataBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2];
static byte LCDWordAniMaskBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2];
byte LCDWordAniBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2] = {0, };
byte LCDWordAniBlinkBuffer[ LCD_TOTAL_ADDRESS_Y ][ LCD_TOTAL_ADDRESS_X * 2] = {0, };

// girin. 2000.3.2
static byte	lcd_PageType = PAGE_NORMAL;
static byte	lcd_DrawMethod = DMETHOD_FOREGROUND;
static byte lcd_Wallpaper = WT_WHITE;
static byte lcd_WallMask = WMT_ALL;

static byte	lcd_PageTypeOld = PAGE_NORMAL;
static byte	lcd_DrawMethodOld = DMETHOD_FOREGROUND;
static byte lcd_WallpaperOld = WT_WHITE;
static byte lcd_WallMaskOld = WMT_ALL;

//static boolean bForceUpdate = FALSE;

static const word WP_SPEED011[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_SPEED011.dhf"
};

static const word WP_TTL[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_TTL.def"
};

static const word WP_nTOP[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_NTOP.def"
};

static const word WP_nTOP2[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_NTOP2.dhf"
};

static const word WP_SKY[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
    #include  "Image\A7\WP_SKY.dhf"
};

static const word WPM_CENTER_MEDIUM[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_CM.def"
};

static const word WPM_SMALL_WATCH[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_SW.def"
};

static const word WP_YHOON[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_IDLE.def"
};

static const word WP_STOPWATCH[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\runner5.dhf"
};

static const word WPM_STOPWATCH[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_STOPWATCH.def"
};

static const word WP_CAL[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_CAL.def"
};

static const word WPM_CAL[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_CAL.def"
};

static const word WP_ALPHA[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_ALPHA.def"
};

static const word WP_CALLSTACK[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_CALLSTACK.def"
};

static const word WP_PASSWORD[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WP_PASSWD.def"
};

static const word WPM_PASSWORD[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_PASSWD.def"
};

static const word WPM_MISSED_CALL[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_MISS.def"
};

static const word WPM_SMS[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_SMS.dhf"
};
//white 00.09.14 inrt for icon-menu.
static const word WPM_TOPMENU[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_MENU.dhf"
};

static const word WPM_TOPCALL[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_CALL.dhf"
};

static const word WPM_TOPBOOK[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_BOOK.dhf"
};

static const word WPM_TOPMSG[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_MSG.dhf"
};

static const word WPM_TOPNTOP[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A7\WPM_NTOP.dhf"
};
//white end.

#ifdef STI_MMI      // woodstok 00.12.11
static const word WP_Itouch017[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = {
	#include  "Image\A10I\itouch017.dhf"
};
#endif

#endif


static byte *pASCIIFontBuffer = (byte *)ASCIIFontBuffer;
static byte *pHanFontBuffer = (byte *)HanFontBuffer;
static byte *pSpecialFontBuffer = (byte *)SpecialFontBuffer;
static byte *pSKFontBuffer = (byte *)SymbolFontBuffer;


#if FONT_DATA != BYTE_DATA
static word *pASCIIWordFontBuffer = (word *)ASCIIWordFontBuffer;
#endif	// #if FONT_DATA == BYTE_DATA

static int	nASCIIFont = NORMAL_SIZE_ASCII_FONT;
static int	nASCIIFontWidth = ASCII_FONT_WIDTH;
static int	nASCIIFontHeight = ASCII_FONT_HEIGHT;
static int	nASCIIFontBufferSizePerChar = ASCII_FONT_BUFFER_SIZE_PER_CHAR;

static int	nHanFont = NORMAL_SIZE_HAN_FONT;
static int	nHanFontWidth = HAN_FONT_WIDTH;
static int	nHanFontHeight = HAN_FONT_HEIGHT;
static int	nHanFontBufferSizePerChar = HAN_FONT_BUFFER_SIZE_PER_CHAR;

static byte nLcdLeftMargin = LCD_LEFT_MARGIN;
static byte nScreenWidth = SCREEN_WIDTH;


#if (MMI_TYPE == MMI_A10)
#ifdef ENHANCED_TEXT_MODE_DISPLAY // girin. 2000.1.27
void TextOutXYnChar(byte *strText, byte *oldText, int nPosX, int nPosY, int nChar, int nDrawMode);
void TextOutXYnCharCursor(byte *strText, byte *oldText, int nPosX, int nPosY, int nChar, byte cursX, int nDrawMode);
#endif
#endif


////////////////////////////
// 4 Gray Widening        //
////////////////////////////
#define WHITE			0x00
#define WHITEGRAY		0x01
#define BLACKGRAY		0x02
#define BLACK			0x03

////////////////////////////
// Function prototype     //
////////////////////////////

static word OutWordCharLCDBuffer(byte *pFont, int nPosX, int nPosY, int nFontWidth, int nFontHeight, int nDrawMode);
static void BuildForegroundBuffer(byte *scr_ptr, byte pagetype);

#ifndef FEATURE_MEM_FONT
void uigd_DrawBannerText(void); //white
#endif
//static void sDrawRecSmbInfo(byte nRecNum, boolean fSmbIdleTraf); // girin. 2000.10.23
//extern const UB	CalSrc[];

// saveLCDOnOff는 현재 LCD의 On/Off 상태를 저장하여 LCD update시마다
// LCD를 reset해 줄 때 현재 LCD의 상태를 그대로 적용한다.
// 현재로서는 Phone이 살아 있을 때 LCD가 Off되어 있을 경우는 없지만, 
// (OFF Line 충전 Mode 제외됨)
// A8 Project의 경우 Folder가 닫혀 있을 경우에는 LCD가 Off되어 있으므로 
// Reset시 LCD의 현재 상태를 적용해야 한다.
// saveLCDCusorBlink도 마찬가지임.
word saveLCDOnOff = 0;		// inserted by kmh 00.08.22
int saveLCDCusorBlink = 0;	// inserted by kmh 00.08.23	-> for HS_LCD_R08
word saveLCDCusorBlinkX = 0;	// for HS_LCD_R11
word saveLCDCusorBlinkY = 0;	// for HS_LCD_R12
word saveLCDSleep = 0;

/*-----------------------------------------------------------------------
                         LCD Controller Routines
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
void hs_lcd_cmd_out
(
  word lcd_command
  /* 16-bit LCD command to output to the LCD controller */
)
{
    INTLOCK();

	// 16 Bit Command를 상/하위 8Bit로 나누어 전송 
	LCD_OUTP_CMD((HS_LCD_MASK & (byte)HS_LCD_HIGH_DATA(lcd_command)));
	LCD_OUTP_CMD((HS_LCD_MASK & (byte)HS_LCD_LOW_DATA(lcd_command)));

	INTFREE();

} /* end of hs_lcd_cmd_out */


/*===========================================================================

EXTERN FUNCTION LCD_WORD_DAT_OUT		// kmh 99.10.22

DESCRIPTION
  Output command to the LCD controller.

DEPENDENCIES
  Clock dependent delays which rely on a 13.5 MHz '186 in an MSM 2p.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_lcd_word_dat_out
(
  word lcd_data
    /* 16-bit LCD command to output to the LCD controller */
)
{
	INTLOCK();

	// 16 Bit Data를 상/하위 8Bit로 나누어 전송 
	LCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_HIGH_DATA(lcd_data)));
	LCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_LOW_DATA(lcd_data)));

	INTFREE();

} /* end of hs_lcd_word_out */

/*===========================================================================

EXTERN FUNCTION LCD_BYTE_DAT_OUT		// kmh 99.10.22

DESCRIPTION
  Output byte data to the LCD controller.

DEPENDENCIES
  Clock dependent delays which rely on a 13.5 MHz '186 in an MSM 2p.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_lcd_byte_dat_out
(
  byte lcd_data
    /* 16-bit LCD command to output to the LCD controller */
)
{
	INTLOCK();

	LCD_OUTP_DATA(lcd_data);

	INTFREE();

} /* end of hs_lcd_word_out */

/*===========================================================================

FUNCTION LCD_ONEBIT_DAT_OUT

DESCRIPTION
  Output command to the LCD controller.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_lcd_onebit_dat_out
(
		byte lcd_data					
)
{
//	byte i, j = 0;		
	word dataBuffer = 0;

	extern const word	ani_Color2ToColor4_tab[256];

#if (0)
	// LCD의 GrayScale Level 0, 1, 2, 3중에서 Font는 0과 4로 구성
	// 따라서 Font Data는 "00" 아니면 "11"로 구성한다.
	if (lcd_data){
		for (i=0; i < 8; i++, j+= 2)
		{ 
			if (lcd_data & (HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
				dataBuffer |= (HS_LCD_MASK_BIT >> j);	// #define HS_LCD_MASK_BIT	0xc000
			else 
				dataBuffer &= ~(HS_LCD_MASK_BIT >> j);
		}
	}	
	else	 dataBuffer = 0;		
#else
	// girin. 2000.3.29
	// Enhanced to use look-up table method
	// Inspired by Chul...
	dataBuffer = ani_Color2ToColor4_tab[lcd_data];
#endif

	LCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_HIGH_DATA(dataBuffer)));
	LCD_OUTP_DATA((HS_LCD_MASK & (byte)HS_LCD_LOW_DATA(dataBuffer)));
} /* end of hs_lcd_onebit_dat_out */
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
void hs_lcd_out
(
		byte lcd_data
    /* 9-bit LCD command to output to the LCD controller */
)
{
} /* end of hs_lcd_out */


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
void hs_lcd_on
(
  boolean on
    /* Whether the display is to be 'on' (True) or 'off' (False) */
)
{
	if(!bInvisable_Charge)		// off 충전 모드가 아닌 경우에만 실행함 by jnlee 00.04.19
	{
	  /* Update the current state
	  */
 		hs_lcd_cmd_out(HS_LCD_R07);	// Select Display on/off Controll Resgister
		/* Double-Height Off, Display On, Fixed displays the first to eight raster rows &  
		   Upper 16 raster-rows are fixed displayed in vertical smooty scrolling */
		if(on)	hs_lcd_word_dat_out(HS_LCD_DISPLAY_ON);
		else	hs_lcd_word_dat_out(HS_LCD_DISPLAY_OFF);
		saveLCDOnOff = on;
	}

} /* end of hs_lcd_on */

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
void hs_lcd_contrast
(
  word contrast
    /* Setting the value of LCD Contrast */
)
{	
	hs_lcd_cmd_out(HS_LCD_R04);		// Setting contrast control register
	// Contrast 조정 Register의 최대 값(0x3f)보다 크면 
	if ( contrast > HS_LCD_CONTRAST_MAX ) 
 	   hs_lcd_word_dat_out(HS_LCD_CONTRAST_MAX);
	// Contrast 조정 Register의 값이 너무 작으면 
	else if ( contrast < HS_LCD_CONTRAST_MIN )  			
 	   hs_lcd_word_dat_out(HS_LCD_CONTRAST_MIN);
	else
	   hs_lcd_word_dat_out(contrast);

} /* end of hs_lcd_contrast */


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
void hs_set_glyph
(
  byte index,           /* Index of the character to set the bitmap of */
  byte *bitmap_ptr      /* Pointer to 7 bytes defining the bitmap      */
)
{
#ifdef QORG

  byte row_addr;

  /* Set the address to the CGRAM character whose bitmap we want to load.
  ** The 0th character is at HS_LCD_CGRAM_ADDR, and each successive character
  ** is HS_LCD_FONT_STEP farther down.
  */
  index = index % HS_GLYPH_CNT;
  hs_lcd_out(HS_LCD_ADDR_I(HS_LCD_CGRAM_ADDR + (index * HS_LCD_FONT_CSTEP)));

  /* For each row of the character, set the row contents
  */
  for (row_addr = 0; row_addr < HS_LCD_FONT_ROWS; row_addr++, bitmap_ptr++) {
    hs_lcd_out( HS_LCD_DATA_I( *bitmap_ptr ) );
  }
#endif

} /* end of hs_set_glyph */


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
#if (MMI_TYPE == MMI_A10)
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/*
void lcd_LoadImage(byte nType)
{
	int				x, y;
	byte			*pAlloc;
	mm_IM1File_Type ImgFile;

	extern const word	ani_Color2ToColor4_tab[256];

	if(mm_IsValidAN1File(ui.ani_option[nType].UserFileName) == FALSE)
	{
		// We failed to open file, so display SKY image
		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
			memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], (LCD_TOTAL_ADDRESS_X-1)*2);
		return;
	}
	if(mm_ImgFileLoadOpen(
			ui.ani_option[nType].UserFileName, 
			sizeof(mm_AN1Frame_Type), 
			&pAlloc) != MM_OK)
	{
		return;
	}

	mm_FileBlockLoad(MM_IM1_HEADER_SIZE);
	memcpy((void*)&ImgFile, pAlloc, MM_IM1_HEADER_SIZE);

	if(ImgFile.NumColors == 2)
	{
		mm_FileBlockLoad(MM_ANI_FRAME_2C_SIZE);
		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
			for(x = 0; x < LCD_TOTAL_ADDRESS_X-1; x++)
				LCDWordAniBuffer[y][x] = 
					ani_Color2ToColor4_tab[pAlloc[y*(LCD_TOTAL_ADDRESS_X-1) + x]];

	}
	else
	{
		mm_FileBlockLoad(MM_ANI_FRAME_4C_SIZE);
		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
			memcpy(
				LCDWordAniBuffer[y], 
				pAlloc[y*(LCD_TOTAL_ADDRESS_X-1)*2], 
				(LCD_TOTAL_ADDRESS_X-1)*2);
	}
	mm_FileLoadClose();
}
*/
void lcd_SetFontType(byte pagetype)
{
	lcd_PageType = pagetype;
	lcd_DrawMethod = DMETHOD_NONMASK_FOREGROUND;
}

/* woodstok 2000-11-28 --> @why, what */
void lcd_SetWallpaperType(byte wallpapertype)
{
	lcd_Wallpaper = wallpapertype;
}
/* <-- */

byte lcd_GetPageType(void)
{
	return lcd_PageType;
}

byte lcd_GetWallType(void)
{
	return lcd_Wallpaper;
}

byte lcd_GetWallMask(void)
{
	return lcd_WallMask;
}

byte lcd_GetDrawMethod(void)
{
	return lcd_DrawMethod;
}

void lcd_SetDispType(hs_scrn_type scrn_ptr, byte PageType, byte WallpaperType, byte WallMaskType, byte DrawMethod)
{
	int y;
	register int x_centinel;
	boolean bChange;
	extern void LCDBlinkTimerStart(byte Type, dword tick); // tick in miliseconds
	extern void LCDBlinkTimerStop(void);

	bChange = FALSE;

	if (lcd_DrawMethod != DrawMethod)
	{
		bChange = TRUE;
		lcd_DrawMethod = DrawMethod;

		if (byAniTypeSav != ANI_TYPE_MAX)
		{
			uia_PlayAnimation(LCD_ANI_STOP, byAniTypeSav);
			byAniTypeSav = ANI_TYPE_MAX;
		}
	}

	if ((PageType != PAGE_CURRENT) && (lcd_PageType != PageType))
	{
		bChange = TRUE;
		lcd_PageType = PageType;
	}


	if (scrn_ptr != NULL)
	{
		bChange = TRUE;
		BuildForegroundBuffer(scrn_ptr, lcd_PageType);
	}

	if ((lcd_WallMask != WallMaskType) && (WallMaskType != WMT_CURRENT))
	{

		bChange = TRUE;

		lcd_WallMask = WallMaskType;

		if (byAniTypeSav != ANI_TYPE_MAX)
		{
			uia_PlayAnimation(LCD_ANI_STOP, byAniTypeSav);
			byAniTypeSav = ANI_TYPE_MAX;
		}

		switch (WallMaskType)
		{
		case WMT_ALL:
			memset(LCDWordAniMaskBuffer, 0xff, sizeof(LCDWordAniMaskBuffer));
			break;

		case WMT_NONE: // banning foreground 
			memset(LCDWordAniMaskBuffer, 0x00, sizeof(LCDWordAniMaskBuffer));
			break;

		case WMT_SMALL_WATCH: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_SMALL_WATCH[y], x_centinel);
			}
			break;

#ifdef FEATURE_FOLDER_PHONE	// woodstok 00.6.13
		case WMT_SMALL_WATCH_WITHOUT_MENU: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_SMALL_WATCH_WITHOUT_MENU[y], x_centinel);
			}
			break;
#endif	// FEATURE_FOLDER_PHONE

#ifdef GIZMO_A7M
		case WMT_SMALL_WATCH_WITH_MENU:
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_SMALL_WATCH_WITH_MENU[y], x_centinel);
			}
			break;
#endif // GIZMO_A7M

		case WMT_CENTER_MEDIUM: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_CENTER_MEDIUM[y], x_centinel);
			}
			break;

		case WMT_STOPWATCH: //	woodstok
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_STOPWATCH[y], x_centinel);
			}
			break;

		case WMT_CAL: //white
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_CAL[y], x_centinel);
			}
			break;

		case WMT_MISSED_CALL: // girin
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_MISSED_CALL[y], x_centinel);
			}
			break;

		case WMT_SMS: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_SMS[y], x_centinel);
			}
			break;

		case WMT_PASSWORD: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_PASSWORD[y], x_centinel);
			}
			break;

//white 00.09.14 inrt for icon menu.
		case WMT_TOP_LEFT: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_TOPMENU[y], x_centinel);
			}
    		break;

		case WMT_TOP_RIGHT: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_TOPNTOP[y], x_centinel);
			}
    		break;

        case WMT_TOP_UP: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_TOPCALL[y], x_centinel);
			}
    		break;
        
        case WMT_TOP_DOWN: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_TOPMSG[y], x_centinel);
			}
    		break;

        case WMT_TOP_CENTER: 
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y; y++)
			{
				memcpy(LCDWordAniMaskBuffer[y], (byte*)WPM_TOPBOOK[y], x_centinel);
			}
    		break;

//white end.
        }
	}


	if (((WallpaperType != WT_CURRENT) && (lcd_Wallpaper != WallpaperType)) 
		|| (WallpaperType == WT_UPDATE) // forcing update.
		|| (WallpaperType == WT_UPDATE_BLINK) // forcing update.
		)
	{
		bChange = TRUE;

		if (byAniTypeSav != ANI_TYPE_MAX)
		{
			uia_PlayAnimation(LCD_ANI_STOP, byAniTypeSav);
			byAniTypeSav = ANI_TYPE_MAX;
		}

		// girin. 2000.3.26: changed to use clk_ interface: rex_clr_timer( &hs_blink_timer );
		LCDBlinkTimerStop();
		rex_clr_sigs( &hs_tcb, HS_BLINK_TIMER_SIG );

		lcd_WallpaperOld = lcd_Wallpaper = WallpaperType;

		switch (WallpaperType)
		{
		//--------------------------------------------------------------------------------------//
		case WT_UPDATE: // forcing update.
			//bForceUpdate = TRUE; // forcing display upadte when the foreground get the focus.			
			////DisplayLCDBufferEnhanced();
			break;

		case WT_UPDATE_BLINK:
			//bForceUpdate = TRUE; // forcing display upadte when the foreground get the focus.
			// girin. 2000.3.26: changed to use clk_ interface: rex_set_timer( &hs_blink_timer, 300 ); // Start ani buffer blinking.
			LCDBlinkTimerStart(ANI_BLINK, 300);
			////DisplayLCDBufferEnhanced();
			break;
		//-------------------------------------------------------------------------------------//
		case WT_NTOP: //
			switch (ui.ani_option[ANI_TYPE_NTOP].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);
				
				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);
				
				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);
				
				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_IDLE);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;
			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);
				
				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif
			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_IDLE);
				byAniTypeSav = ANI_TYPE_NTOP;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}

			break;
			
		case WT_IDLE: //
#ifdef FEATURE_FOLDER_PHONE	// woodstok 00.6.13
		case WT_IDLE_MENU:
#endif
			switch (ui.ani_option[ANI_TYPE_IDLE].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_IDLE);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif

			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_IDLE);
				byAniTypeSav = ANI_TYPE_IDLE;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}
#ifdef FEATURE_FOLDER_PHONE		// woodstok 00.6.13

			if(WallpaperType == WT_IDLE_MENU){
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;
    			for (y = 64; y < 80 ; y++)
    				memcpy(LCDWordAniBuffer[y], (byte*)WP_MENU_BOOK_A8[y], x_centinel);
			}
			
#endif //FEATURE_FOLDER_PHONE			

			break;

		case WT_PREVIEW:
#ifdef FEATURE_FOLDER_PHONE		// woodstok 00.6.13
		case WT_PREVIEW_MENU:
#endif
			switch (ui.ani_option[ANI_TYPE_PREVIEW].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif
//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_PREVIEW);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;

			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
				byAniTypeSav = ANI_TYPE_PREVIEW;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}

#ifdef FEATURE_FOLDER_PHONE	// woodstok 00.6.13
			if(WallpaperType == WT_PREVIEW_MENU){
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;
    			for (y = 64; y < 80 ; y++)
    				memcpy(LCDWordAniBuffer[y], (byte*)WP_MENU_BOOK_A8[y], x_centinel);
			}
			
#endif //FEATURE_FOLDER_PHONE			
			break;

		case WT_YHOON:
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_YHOON[y], x_centinel);

#ifndef FEATURE_MEM_FONT
            uigd_DrawBannerText();
#endif

			break;

		case WT_STOPWATCH_RUNNING: // woodstok	00.4.12
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_STOPWATCH;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

		case WT_STOPWATCH_STOP:	// woodstok 00.4.12
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_STOPWATCH[y], x_centinel);

			break;

		case WT_CAL: // white
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_CAL[y], x_centinel);

			break;

		case WT_SMS_NOTIF: 
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_SMSNOTIF;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

		case WT_ALPHA: // girin
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_ALPHA[y], x_centinel);

			break;

		case WT_CALLSTACK: // girin
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_CALLSTACK[y], x_centinel);

			break;

		case WT_PASSWORD: // girin
			x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

			for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
				memcpy(LCDWordAniBuffer[y], (byte*)WP_PASSWORD[y], x_centinel);

			break;

//white 00.09.14 inrt for icon menu.

		case WT_TOP_LEFT: 
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_TOPMENU;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

        case WT_TOP_RIGHT: 
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_TOPNTOP; //ANI_OPTION_TOPBOOK;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

        case WT_TOP_UP: 
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_TOPCALL;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

        case WT_TOP_DOWN: 
            ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_TOPMSG;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

        case WT_TOP_CENTER: 
            ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_TOPBOOK; //ANI_OPTION_TOPNTOP;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

//white end.

		case WT_HOLDON:
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_HOLDON;
			uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;

		case WT_POWER_ON:
			switch (ui.ani_option[ANI_TYPE_POWER_ON].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();
				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif
//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_POWER_ON);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;

			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_POWER_ON);
				byAniTypeSav = ANI_TYPE_POWER_ON;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}

			break;

		case WT_POWER_OFF:
			switch (ui.ani_option[ANI_TYPE_POWER_OFF].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif
//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_POWER_OFF);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;

			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_POWER_OFF);
				byAniTypeSav = ANI_TYPE_POWER_OFF;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}

			break;

		case WT_PAGING:
		case WT_PAGING_CLI:
			switch (ui.ani_option[ANI_TYPE_PAGING].Option)
			{
			case ANI_OPTION_IMG_SKY:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SKY[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_SPEED011:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_SPEED011[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_TTL:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_TTL[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_NTOP:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

			case ANI_OPTION_IMG_NTOP2:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_nTOP2[y], x_centinel);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

				break;

#ifdef STI_MMI  // woodstok 00.12.11
			case ANI_OPTION_017:
				x_centinel = (LCD_TOTAL_ADDRESS_X-1)*2;

				for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
					memcpy(LCDWordAniBuffer[y], (byte*)WP_Itouch017[y], x_centinel);
			
				break;

#endif
//			case ANI_OPTION_IMG_USER_FILE:
//				lcd_LoadImage(ANI_TYPE_PAGING);

				//if (lcd_DrawMethod == DMETHOD_BACKGROUND)
				//	DisplayLCDBufferEnhanced();

//				break;

			case ANI_OPTION_SKY:
			case ANI_OPTION_SPEED011:
			case ANI_OPTION_TTL:
			case ANI_OPTION_DANCING_PHONE:
			case ANI_OPTION_PHONE_CALL:
			case ANI_OPTION_FLYING:
			case ANI_OPTION_WINDOW:
			case ANI_OPTION_CAT:			
			case ANI_OPTION_PHONE:
			case ANI_OPTION_LUCK:
			case ANI_OPTION_RING:
			case ANI_OPTION_DOG:
//			case ANI_OPTION_DANCING:
			// case ANI_OPTION_SKYCF:
			case ANI_OPTION_SKYROLL:
			case ANI_OPTION_SMALLDOG:
			case ANI_OPTION_GOODBYE:
			case ANI_OPTION_GOODBYE2:
			case ANI_OPTION_CF1:
			case ANI_OPTION_CF2:
			case ANI_OPTION_CF3:
			case ANI_OPTION_CF4:
			case ANI_OPTION_CF5:
			case ANI_OPTION_CF6:
			case ANI_OPTION_ANI_USER_FILE:

				//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PAGING);
				byAniTypeSav = ANI_TYPE_PAGING;
				break;

			default:
				memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
				break;
			}

			break;

		case WT_MISSED_CALL:
			ui.ani_option[ANI_TYPE_PREVIEW].Option = ANI_OPTION_MISSED_CALL;
			//uia_PlayAnimation(LCD_ANI_START, ANI_TYPE_PREVIEW);
			byAniTypeSav = ANI_TYPE_PREVIEW;
			break;


		case WT_WHITE:
		default:
			memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));

			//DisplayLCDBufferEnhanced();
			break;
		}
	}
		
	//-----------------------------------------------//
	if (bChange)
	{
		bChange = FALSE;
		if (byAniTypeSav != ANI_TYPE_MAX)
			uia_PlayAnimation(LCD_ANI_START, byAniTypeSav);
		else{
			DisplayLCDBufferEnhanced();
		}
	}
	//-----------------------------------------------//
}

/////////////////////////////
// 
void DisplayLCDBufferEnhanced(void)
{
	int x, y;
	register int x_centinel;
	byte byteData, byteBackData;
	byte byteMask, bitPos, bitMask;
	int index;

	static const byte Decrease_Gray_tab[256] = 
	{ 
#if (1)
		// Black to dark gray.

		0x00, 0x01, 0x02, 0x02, // (0000 00XX)b 
		0x04, 0x05, 0x06, 0x06, // (0000 01XX)b
		0x08, 0x09, 0x0a, 0x0a, // (0000 10XX)b
		0x08, 0x09, 0x0a, 0x0a, // (0000 11XX)b

		0x10, 0x11, 0x12, 0x12, // (0001 00XX)b 
		0x14, 0x15, 0x16, 0x16, // (0001 01XX)b
		0x18, 0x19, 0x1a, 0x1a, // (0001 10XX)b
		0x18, 0x19, 0x1a, 0x1a, // (0001 11XX)b

		0x20, 0x21, 0x22, 0x22, // (0010 00XX)b 
		0x24, 0x25, 0x26, 0x26, // (0010 01XX)b
		0x28, 0x29, 0x2a, 0x2a, // (0010 10XX)b
		0x28, 0x29, 0x2a, 0x2a, // (0010 11XX)b

		0x20, 0x21, 0x22, 0x22, // (0011 00XX)b 
		0x24, 0x25, 0x26, 0x26, // (0011 01XX)b
		0x28, 0x29, 0x2a, 0x2a, // (0011 10XX)b
		0x28, 0x29, 0x2a, 0x2a, // (0011 11XX)b

		0x40, 0x41, 0x42, 0x42, // (0100 00XX)b 
		0x44, 0x45, 0x46, 0x46, // (0100 01XX)b
		0x48, 0x49, 0x4a, 0x4a, // (0100 10XX)b
		0x48, 0x49, 0x4a, 0x4a, // (0100 11XX)b

		0x50, 0x51, 0x52, 0x52, // (0101 00XX)b 
		0x54, 0x55, 0x56, 0x56, // (0101 01XX)b
		0x58, 0x59, 0x5a, 0x5a, // (0101 10XX)b
		0x58, 0x59, 0x5a, 0x5a, // (0101 11XX)b

		0x60, 0x61, 0x62, 0x62, // (0110 00XX)b 
		0x64, 0x65, 0x66, 0x66, // (0110 01XX)b
		0x68, 0x69, 0x6a, 0x6a, // (0110 10XX)b
		0x68, 0x69, 0x6a, 0x6a, // (0110 11XX)b

		0x60, 0x61, 0x62, 0x62, // (0111 00XX)b 
		0x64, 0x65, 0x66, 0x66, // (0111 01XX)b
		0x68, 0x69, 0x6a, 0x6a, // (0111 10XX)b
		0x68, 0x69, 0x6a, 0x6a, // (0111 11XX)b





		0x80, 0x81, 0x82, 0x82, // (1000 00XX)b 
		0x84, 0x85, 0x86, 0x86, // (1000 01XX)b
		0x88, 0x89, 0x8a, 0x8a, // (1000 10XX)b
		0x88, 0x89, 0x8a, 0x8a, // (1000 11XX)b

		0x90, 0x91, 0x92, 0x92, // (1001 00XX)b 
		0x94, 0x95, 0x96, 0x96, // (1001 01XX)b
		0x98, 0x99, 0x9a, 0x9a, // (1001 10XX)b
		0x98, 0x99, 0x9a, 0x9a, // (1001 11XX)b

		0xa0, 0xa1, 0xa2, 0xa2, // (1010 00XX)b 
		0xa4, 0xa5, 0xa6, 0xa6, // (1010 01XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1010 10XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1010 11XX)b

		0xa0, 0xa1, 0xa2, 0xa2, // (1011 00XX)b 
		0xa4, 0xa5, 0xa6, 0xa6, // (1011 01XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1011 10XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1011 11XX)b

		0x80, 0x81, 0x82, 0x82, // (1100 00XX)b 
		0x84, 0x85, 0x86, 0x86, // (1100 01XX)b
		0x88, 0x89, 0x8a, 0x8a, // (1100 10XX)b
		0x88, 0x89, 0x8a, 0x8a, // (1100 11XX)b

		0x90, 0x91, 0x92, 0x92, // (1101 00XX)b 
		0x94, 0x95, 0x96, 0x96, // (1101 01XX)b
		0x98, 0x99, 0x9a, 0x9a, // (1101 10XX)b
		0x98, 0x99, 0x9a, 0x9a, // (1101 11XX)b

		0xa0, 0xa1, 0xa2, 0xa2, // (1110 00XX)b 
		0xa4, 0xa5, 0xa6, 0xa6, // (1110 01XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1110 10XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1110 11XX)b

		0xa0, 0xa1, 0xa2, 0xa2, // (1111 00XX)b 
		0xa4, 0xa5, 0xa6, 0xa6, // (1111 01XX)b
		0xa8, 0xa9, 0xaa, 0xaa, // (1111 10XX)b
		0xa8, 0xa9, 0xaa, 0xaa  // (1111 11XX)b
#else
		// light gray only.
		0x00, 0x01, 0x01, 0x01, // (0000 00XX)b 
		0x04, 0x05, 0x05, 0x05, // (0000 01XX)b
		0x04, 0x05, 0x05, 0x05, // (0000 10XX)b
		0x04, 0x05, 0x05, 0x05, // (0000 11XX)b
		
		0x10, 0x11, 0x11, 0x11, // (0001 00XX)b 
		0x14, 0x15, 0x15, 0x15, // (0001 01XX)b
		0x14, 0x15, 0x15, 0x15, // (0001 10XX)b
		0x14, 0x15, 0x15, 0x15, // (0001 11XX)b

		0x10, 0x11, 0x11, 0x11, // (0010 00XX)b 
		0x14, 0x15, 0x15, 0x15, // (0010 01XX)b
		0x14, 0x15, 0x15, 0x15, // (0010 10XX)b
		0x14, 0x15, 0x15, 0x15, // (0010 11XX)b

		0x10, 0x11, 0x11, 0x11, // (0011 00XX)b 
		0x14, 0x15, 0x15, 0x15, // (0011 01XX)b
		0x14, 0x15, 0x15, 0x15, // (0011 10XX)b
		0x14, 0x15, 0x15, 0x15, // (0011 11XX)b

		0x40, 0x41, 0x41, 0x41, // (0100 00XX)b 
		0x44, 0x45, 0x45, 0x45, // (0100 01XX)b
		0x44, 0x45, 0x45, 0x45, // (0100 10XX)b
		0x44, 0x45, 0x45, 0x45, // (0100 11XX)b

		0x50, 0x51, 0x51, 0x51, // (0101 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (0101 01XX)b
		0x54, 0x55, 0x55, 0x55, // (0101 10XX)b
		0x54, 0x55, 0x55, 0x55, // (0101 11XX)b

		0x50, 0x51, 0x51, 0x51, // (0110 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (0110 01XX)b
		0x54, 0x55, 0x55, 0x55, // (0110 10XX)b
		0x54, 0x55, 0x55, 0x55, // (0110 11XX)b

		0x50, 0x51, 0x51, 0x51, // (0111 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (0111 01XX)b
		0x54, 0x55, 0x55, 0x55, // (0111 10XX)b
		0x54, 0x55, 0x55, 0x55, // (0111 11XX)b

		0x40, 0x41, 0x41, 0x41, // (1000 00XX)b 
		0x44, 0x45, 0x45, 0x45, // (1000 01XX)b
		0x44, 0x45, 0x45, 0x45, // (1000 10XX)b
		0x44, 0x45, 0x45, 0x45, // (1000 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1001 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1001 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1001 10XX)b
		0x54, 0x55, 0x55, 0x55, // (1001 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1010 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1010 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1010 10XX)b
		0x54, 0x55, 0x55, 0x55, // (1010 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1011 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1011 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1011 10XX)b
		0x54, 0x55, 0x55, 0x55, // (1011 11XX)b

		0x40, 0x41, 0x41, 0x41, // (1100 00XX)b 
		0x44, 0x45, 0x45, 0x45, // (1100 01XX)b
		0x44, 0x45, 0x45, 0x45, // (1100 10XX)b
		0x44, 0x45, 0x45, 0x45, // (1100 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1101 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1101 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1101 10XX)b
		0x54, 0x55, 0x55, 0x55, // (1101 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1110 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1110 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1110 10XX)b
		0x54, 0x55, 0x55, 0x55, // (1110 11XX)b

		0x50, 0x51, 0x51, 0x51, // (1111 00XX)b 
		0x54, 0x55, 0x55, 0x55, // (1111 01XX)b
		0x54, 0x55, 0x55, 0x55, // (1111 10XX)b
		0x54, 0x55, 0x55, 0x55 // (1111 11XX)b
#endif 
	};

	// for LCD test by msrock
	if (LcdTestFlag) return;

	/*----------------------------------------------------

    -----------------------------------------------------*/

	if(!bInvisable_Charge)		// off 충전 모드가 아닌 경우에만 실행함 
	{
		/*----------------------------------------------------
		   00.05.15
		   LCD 폭주 현상이 일어날 경우를 대비하여 
		   LCD Display시마다 Initialize를 한다.
		-----------------------------------------------------*/
		hs_lcd_reset();
	}

	hs_lcd_reset_instruction();

	// Ram Address Set Register
	hs_lcd_cmd_out(HS_LCD_R17);

	// girin. 2000.1.31. Reserve picto area.
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR);		// Start Address set
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR - (16*16)); // Start Address set. 0x07ff
	// Use constant value for performance improvement.
	hs_lcd_word_dat_out(0x06ff); // Should be same to HS_LCD_START_ADDR - 16 * 16.

	hs_lcd_reset_instruction();	
	hs_lcd_cmd_out(HS_LCD_R18);


	switch (lcd_DrawMethod)
	{
	case DMETHOD_BACKGROUND:
		for (index = 0 ; index < (sizeof ( LCDWordAniBuffer )); index++) 
		{
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA(*((byte*)LCDWordAniBuffer+index));
			//INTFREE();	// delete for performance improvement by kmh 00.03.28
		}

		break;

	case DMETHOD_TRANSPARENT:

		x_centinel = (LCD_TOTAL_ADDRESS_X - 1)*2;

		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				if (LCDWordAniMaskBuffer[y][x] == 0x00)
				{
					// just assign wallpaper data.
					byteData = LCDWordAniBuffer[y][x];

				}
				else
				{
					byteMask = LCDWordAniMaskBuffer[y][x];
					byteData = LCDWordDataBuffer[y][x];
					
					byteBackData = Decrease_Gray_tab[LCDWordAniBuffer[y][x]];

					for (bitPos = 0, bitMask = 0xc0; bitPos < 8; bitPos += 2, bitMask = (bitMask >> 2))
					{
						if (byteMask & bitMask)
						{
							if (byteData & bitMask) 
							{
								// nop.
							}
							else
							{
								byteData |= (byteBackData & bitMask);
							}
						}
						else
						{
							byteData |= (byteBackData & bitMask);
						}
					}
				}

				//INTLOCK();	 // delete for performance improvement by kmh 00.03.28
				LCD_OUTP_DATA(byteData);
				//INTFREE();	 // delete for performance improvement by kmh 00.03.28
			}

			// Dummy, but for alignment.
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA(0x00);
			LCD_OUTP_DATA(0x00);
			//INTFREE();	// delete for performance improvement by kmh 00.03.28
		}

		break;
	case DMETHOD_OR:

		break;
	case DMETHOD_AND:

		break;
	case DMETHOD_XOR:

		break;

	case DMETHOD_FOREGROUND:	

		x_centinel = (LCD_TOTAL_ADDRESS_X - 1)*2;

		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				if (LCDWordAniMaskBuffer[y][x] == 0x00)
				{
					// just assign wallpaper data.
					byteData = LCDWordAniBuffer[y][x];

				}
				else
				{
					byteMask = LCDWordAniMaskBuffer[y][x];
					byteData = LCDWordDataBuffer[y][x];
					byteBackData = LCDWordAniBuffer[y][x];

					for (bitPos = 0, bitMask = 0xc0; bitPos < 8; bitPos += 2, bitMask = (bitMask >> 2))
					{
						if (byteMask & bitMask)
						{
							// nop.
						}
						else
						{
							byteData &= ~bitMask;
							byteData |= (byteBackData & bitMask);
						}
					}
				}

				//INTLOCK();	// delete for performance improvement by kmh 00.03.28
				LCD_OUTP_DATA (byteData);
				//INTFREE();	// delete for performance improvement by kmh 00.03.28
			}

			// Dummy, but for alignment.
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA (0x00);
			LCD_OUTP_DATA (0x00);
			//INTFREE();	// delete for performance improvement by kmh 00.03.28

		}

		break;

#ifdef GIZMO_A7M		// woodstok 00.7.5
	case DMETHOD_TRANSPARENT_MENU:
		x_centinel = (LCD_TOTAL_ADDRESS_X - 1)*2;

		// Draw Transparent Part

		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-(HAN_FONT_HEIGHT*2); y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				if (LCDWordAniMaskBuffer[y][x] == 0x00)
				{
					// just assign wallpaper data.
					byteData = LCDWordAniBuffer[y][x];

				}
				else
				{
					byteMask = LCDWordAniMaskBuffer[y][x];
					byteData = LCDWordDataBuffer[y][x];
					
					byteBackData = Decrease_Gray_tab[LCDWordAniBuffer[y][x]];

					for (bitPos = 0, bitMask = 0xc0; bitPos < 8; bitPos += 2, bitMask = (bitMask >> 2))
					{
						if (byteMask & bitMask)
						{
							if (byteData & bitMask) 
							{
								// nop.
							}
							else
							{
								byteData |= (byteBackData & bitMask);
							}
						}
						else
						{
							byteData |= (byteBackData & bitMask);
						}
					}
				}

				//INTLOCK();	 // delete for performance improvement by kmh 00.03.28
				LCD_OUTP_DATA(byteData);
				//INTFREE();	 // delete for performance improvement by kmh 00.03.28
			}

			// Dummy, but for alignment.
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA(0x00);
			LCD_OUTP_DATA(0x00);
			//INTFREE();	// delete for performance improvement by kmh 00.03.28
		}

		// Draw Foreground Part (MENU)
		for (; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				byteData = LCDWordDataBuffer[y][x];
				//INTLOCK();	// delete for performance improvement by kmh 00.03.28
				LCD_OUTP_DATA (byteData);
				//INTFREE();	// delete for performance improvement by kmh 00.03.28
			}

			// Dummy, but for alignment.
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA (0x00);
			LCD_OUTP_DATA (0x00);
			//INTFREE();	// delete for performance improvement by kmh 00.03.28

		}

		break;

#endif //GIZMO_A7M

	case DMETHOD_NONMASK_FOREGROUND:
	default:
		for (index = 0 ; index < (sizeof ( LCDWordDataBuffer )); index++) 
		{
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA (*((byte*)LCDWordDataBuffer+index));
			//INTFREE();	// delete for performance improvement by kmh 00.03.28
		}
		break;
	}
}

void BuildLCDBufferAll(void)
{
#if (0)

	int x, y;
	register int x_centinel;
	byte byteData, byteBackData;
	byte byteMask, bitPos, bitMask;

	switch (lcd_DrawMethod)
	{
	case DMETHOD_BACKGROUND:
		memcpy(LCDWordDataBuffer, LCDWordAniBuffer, sizeof(LCDWordAniBuffer));
		break;

	case DMETHOD_TRANSPARENT:

		x_centinel = (LCD_TOTAL_ADDRESS_X - 1)*2;

		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				if (LCDWordAniMaskBuffer[y][x] == 0x00)
				{
					// just assign wallpaper data.
					LCDWordDataBuffer[y][x] = LCDWordAniBuffer[y][x];

				}
				else
				{
					byteMask = LCDWordAniMaskBuffer[y][x];
					byteData = LCDWordDataBuffer[y][x];
					byteBackData = LCDWordAniBuffer[y][x];

					for (bitPos = 0, bitMask = 0xc0; bitPos < 8; bitPos += 2, bitMask = (bitMask >> 2))
					{
						if (byteMask & bitMask)
						{
							if (byteData & bitMask) 
							{
								// nop.
							}
							else
							{
								byteData |= (byteBackData & bitMask);
							}
						}
						else
						{
							byteData |= (byteBackData & bitMask);
						}
					}

					LCDWordDataBuffer[y][x] = byteData;
				}
			}
		}

		break;
	case DMETHOD_OR:

		break;
	case DMETHOD_AND:

		break;
	case DMETHOD_XOR:

		break;

	case DMETHOD_FOREGROUND:	

		x_centinel = (LCD_TOTAL_ADDRESS_X - 1)*2;

		for (y = 0; y < LCD_TOTAL_ADDRESS_Y-HAN_FONT_HEIGHT; y++)
		{
			for (x = 0; x < x_centinel; x++)
			{
				if (LCDWordAniMaskBuffer[y][x] == 0x00)
				{
					// just assign wallpaper data.
					LCDWordDataBuffer[y][x] = LCDWordAniBuffer[y][x];

				}
				else
				{
					byteMask = LCDWordAniMaskBuffer[y][x];
					byteData = LCDWordDataBuffer[y][x];
					byteBackData = LCDWordAniBuffer[y][x];

					for (bitPos = 0, bitMask = 0xc0; bitPos < 8; bitPos += 2, bitMask = (bitMask >> 2))
					{
						if (byteMask & bitMask)
						{
							// nop.
						}
						else
						{
							byteData &= ~bitMask;
							byteData |= (byteBackData & bitMask);
						}
					}

					LCDWordDataBuffer[y][x] = byteData;
				}
			}
		}

		break;

	case DMETHOD_NONMASK_FOREGROUND:
	default:
		// nop.
		break;
	}
#endif // Not Used
}


void hs_update_screen(byte Command, byte Attr)
{
	byte *new_scr_ptr, *old_scr_ptr; /* ex) *_scr_ptr = " Service Required " */

#if defined (PM1000_DEBUG) || defined (QPMTEST_DEBUG)
    return;
#endif

	switch (Command)
	{
		case LCD_ANI_TIMEOUT:

			uia_PlayAnimation(Command, Attr);
			break;

		case HS_ALL_SCRN:

			new_scr_ptr = hs_crnt_screen_ptr;
			old_scr_ptr = hs_lcd_screen;

			// girin. add. 2000.3.11
			// If there is no change on LCD display, no more proceeding.
			if ((lcd_WallpaperOld == lcd_Wallpaper) 
				&& (lcd_PageTypeOld == lcd_PageType) 
				&& (lcd_WallMaskOld == lcd_WallMask) 
				&& (lcd_DrawMethodOld == lcd_DrawMethod) 
				&& (cursor_x == 0xff)
				//&& (!bForceUpdate)
				)
			{
				if (!memcmp(new_scr_ptr, old_scr_ptr, sizeof(hs_screen)))
					return; // no need LCD display update.
				else
				{
					memcpy(old_scr_ptr, new_scr_ptr, sizeof(hs_screen));	
				}
			}
			else
			{
				lcd_PageTypeOld = lcd_PageType;
				lcd_WallpaperOld = lcd_Wallpaper;
				lcd_WallMaskOld = lcd_WallMask;
				lcd_DrawMethodOld = lcd_DrawMethod;

				memcpy(old_scr_ptr, new_scr_ptr, sizeof(hs_screen));	
				//bForceUpdate = FALSE;
			}

			//---------------------------------------------//
			BuildForegroundBuffer(new_scr_ptr, lcd_PageType);
			//---------------------------------------------//

			//--------------------//
			DisplayLCDBufferEnhanced();
			//--------------------//

			break;

		default:
			ERR_FATAL("Invalid Cmd", 0, 0, 0);
			break;
		}
} /* end of hs_update_screen */

/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
#elif (MMI_TYPE == MMI_QUALCOMM)
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
void hs_update_screen( void )
{
	byte *new_scr_ptr, *old_scr_ptr; /* ex) *_scr_ptr = " Service Required " */
	word j, addr_op=0;
	static int start_x[HS_SCRN_HIGH]={0,};
	int  display_mode;
	
	// Check whether now the phone is displaying page animation or not
	//if ( kj_st_Animation == KJ_ANI_PAGING) return;

#if defined (PM1000_DEBUG) || defined (QPMTEST_DEBUG)
    return;
#endif
	// Clear LCD Buffer
	memset(LCDDataBuffer, 0x00, sizeof(LCDDataBuffer));	

	new_scr_ptr = hs_crnt_screen_ptr;
	old_scr_ptr = hs_lcd_screen;

	// Reserve pictograph area
	addr_op += HAN_FONT_HEIGHT;

	SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
	SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */
	for (j = 0 ; j < HS_SCRN_HIGH ; j++) 
	{
		display_mode = DRAW_NORMAL;
		start_x[j] = 0;
		TextOutXY( new_scr_ptr, NULL, start_x[j], addr_op, display_mode );
		addr_op += nASCIIFontHeight;

		if(j < HS_SCRN_HIGH - 1 ) 
		{ 
			new_scr_ptr = new_scr_ptr + HS_SCRN_WIDE;
		}
	} // end of for-loop

	DisplayLCDBufferAll( );

} /* end of hs_update_screen */
#else 
#error
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
/////////////////////////////
#endif // MMI_TYPE

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
//#define	HS_OLD_LCD
void hs_lcd_init( void )
{
	int nIndex = 0;
    
#ifdef SW_RESET_ENABLE
	if(mc_bSwReset)
	{
		(void) memset((byte *)LCDWordDataBuffer, 0x00, sizeof(LCDWordDataBuffer));
		(void) memset((byte *)LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
		(void) memset((byte *)LCDWordAniMaskBuffer, 0xff, sizeof(LCDWordAniMaskBuffer));
		(void) memset((byte *)IconBuffer, 0x00, sizeof(IconBuffer));

		hs_blink_annun = 0;

		HS_BACKLIGHT_INIT();  /* Backlight control GPIO */
		HS_CALL_LED_INIT();   /* Call LED control GPIO  */
//		BIO_TRISTATE( DEC_GPIO_1, BIO_LCD_STB_M, BIO_LCD_STB_M );
		return;
	}
#endif // SW_RESET_ENABLE

#if (MODEL_ID==MODEL_A8)
  /* Enable the LCD serial interface outputs */
  BIO_OUT( DEC_GPIO_0, BIO_LCD_SEL1_M | BIO_LCD_A0_M | BIO_LCD_DATA_M,
           BIO_LCD_DESEL1_V | BIO_LCD_A0_HI_V | BIO_LCD_DATA_V );

  /* Enable the SUB LCD serial interface outputs */
  BIO_TRISTATE( DEC_GPIO_0, BIO_LCD_SEL1_M | BIO_LCD_A0_M | BIO_LCD_DATA_M,
                BIO_OUTPUT_ENA_V );

#elif (MODEL_ID==MODEL_B1)
  // GPIO이용 LCD_WR, LCD_A1 이용 by jnlee 00.05.13
  BIO_OUT (MSM31_GPIO_2,
			BIO_LCD_A01_M | BIO_LCD_WR_M, BIO_LCD_A01_LO_V | BIO_LCD_WR_LO_V);
  BIO_TRISTATE( MSM31_GPIO_2, 
			BIO_LCD_A01_M | BIO_LCD_WR_M, BIO_OUTPUT_ENA_V );

	#ifdef B1_COLOR_LCD
	// color LCD TESt	by jnlee test 00.05.26
	LCD_OUTP_CMD(0xCA);		// DISCTL
	LCD_OUTP_DATA(0x00);
	LCD_OUTP_DATA(0x1D);
	LCD_OUTP_DATA(0x0B);

	LCD_OUTP_CMD(0xC6);		// VOLCTL
	LCD_OUTP_DATA(0x36);

	LCD_OUTP_CMD(0x75);		// PASET
	LCD_OUTP_DATA(0x00);
	LCD_OUTP_DATA(0x77);

	LCD_OUTP_CMD(0x15);		// CASET
	LCD_OUTP_DATA(0x00);
	LCD_OUTP_DATA(0x77);

	LCD_OUTP_CMD(0xBC);		// DACTL
	LCD_OUTP_DATA(0x02);
	LCD_OUTP_DATA(0x01);

	LCD_OUTP_CMD(0xBC);		// DACTL
	LCD_OUTP_DATA(0x02);
	LCD_OUTP_DATA(0x01);

	LCD_OUTP_CMD(0xA7);		// DISINV

	LCD_OUTP_CMD(0x94);		// SLPOUT

	LCD_OUTP_CMD(0xAF);		// DISPON

	{
		word i,j,m;
		word k=1000;
		byte *fu;

		LCD_OUTP_CMD(0x5c);	
		for(j=0;j<120;j++)
			for(i=0;i<120;i++)
			{
				LCD_OUTP_DATA(0xff);	
			}
		LCD_OUTP_CMD(0xAF);		// DISPON 표시부 on 

//////////////////////////1//////////////////////////////
		LCD_OUTP_CMD(0x75);		// PASET
		LCD_OUTP_DATA(0x00);	// start page
		LCD_OUTP_DATA(0x77);	// end page

		LCD_OUTP_CMD(0x15);		// CASET
		LCD_OUTP_DATA(0x00);	// start column address
		LCD_OUTP_DATA(0x77);	// end col addr

		fu = (byte *)0x50002d;

		LCD_OUTP_CMD(0x5c);	
		for(j=0;j<120;j++)
			for(i=0;i<120;i++)
			{
				byte outdata, R,G,B;
				R = *(fu++) & 0xe0;
				G = (*(fu++) & 0xe0 )>>3;
				B = (*(fu++) & 0xc0 )>>6;
				LCD_OUTP_DATA(R|G|B);	
			}
		LCD_OUTP_CMD(0xAF);		// DISPON 표시부 on 
////////////////////////////2//////////////////////////////////

		for(m=0;m<30000; m++)
		{
			byte *chch;
			word divider;
			divider = 10;

			LCD_OUTP_CMD(0x75);		// PASET
			LCD_OUTP_DATA(0x00);	// start page
			LCD_OUTP_DATA(0x77);	// end page

			LCD_OUTP_CMD(0x15);		// CASET
			LCD_OUTP_DATA(0x00);	// start column address
			LCD_OUTP_DATA(0x77);	// end col addr

			i = m%divider;
			fu = (byte *)(0x40002d + 0x10000 * i);

			LCD_OUTP_CMD(0x5c);	
			for(j=0;j<120;j++)
				for(i=0;i<120;i++)
				{
					byte outdata, R,G,B;
					R = *(fu++) & 0xe0;
					G = (*(fu++) & 0xe0 )>>3;
					B = (*(fu++) & 0xc0 )>>6;
					LCD_OUTP_DATA(R|G|B);	
				}
			LCD_OUTP_CMD(0xAF);		// DISPON 표시부 on 

			for(i=0;i<k;i++)
				dog_kick();
		}
/////////////////////////////3////////////////////////////////
		for(m=0;m<30000; m++)
		{
			byte *chch;

			LCD_OUTP_CMD(0x75);		// PASET
			LCD_OUTP_DATA(0x00);	// start page
			LCD_OUTP_DATA(0x77);	// end page

			LCD_OUTP_CMD(0x15);		// CASET
			LCD_OUTP_DATA(0x00);	// start column address
			LCD_OUTP_DATA(0x77);	// end col addr

			switch (m%5)
			{
				case 0:	chch = &(aaaa[0][0]); break;
				case 1:	chch = &(cccc[0][0]); break;
				case 2:	chch = &(eeee[0][0]); break;
				case 3:	chch = &(gggg[0][0]); break;
				case 4:	chch = &(iiii[0][0]); break;
			}

			LCD_OUTP_CMD(0x5c);	
			for(j=0;j<120;j++)
				for(i=0;i<120;i++)
				{
					LCD_OUTP_DATA(*(chch++));	
				}
			LCD_OUTP_CMD(0xAF);		// DISPON 표시부 on 

			for(i=0;i<k;i++)
				dog_kick();
		}
///////////////////////////////////////////////////////////////
	}
	#endif	// define B1_color_lcd
#endif

	// 1. Start Oscillation executed 
	//  : Stand by mode의 halt 상태로부터 Oscillator를 다시 재시작 시킴.
	hs_lcd_cmd_out(HS_LCD_R00);
	hs_lcd_word_dat_out(HS_LCD_START_OSC);
	
	// 2. Driver Output Control 
	//  : 128 * 96 dots Set.
	// 00.01.20 LCD New Spec에 의해 변경 0x024b --> 0x034b
	hs_lcd_cmd_out(HS_LCD_R01);
	hs_lcd_word_dat_out(0x024b);

	// 3. Waveform AC Drive 
	// 00.04.04 LCD New Spec 0x0000-> 0x005c
	hs_lcd_cmd_out(HS_LCD_R02);
#if MODEL_ID == MODEL_A8	// kmh modify - 교세라 요청으로 Setting Value 수정
	hs_lcd_word_dat_out(0x005d);
#else
	hs_lcd_word_dat_out(0x005c);
#endif
	
	// 4. Power Control & Bias Set & Boost Set
	//  : LCD drive Bias Value -> 1/10 bias drive
	//    V5OUT Output Level -> 6 Times Boost
	//    Operating Clock Frequency in the Booster -> 16 Divided clock
	//    Amount of Fixed Current in the Operational Amplifier -> Small
	// 00.03.02 LCD New Spec에 의해 변경 0x0614 --> 0x0918
	hs_lcd_cmd_out(HS_LCD_R03);
#ifdef HS_OLD_LCD
	hs_lcd_word_dat_out(0x0614);
#else
	hs_lcd_word_dat_out(0x0918);
#endif	
	// 5. Contrast Control
	//  : Default Contrast Value Set
	hs_lcd_cmd_out(HS_LCD_R04);
#ifdef HS_OLD_LCD
	hs_lcd_word_dat_out(0x002a);
#else
	hs_lcd_word_dat_out(HS_LCD_CONTRAST_H);
#endif

	// 6. Entry Mode 
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R05);
	hs_lcd_word_dat_out(0x0000);
	
	// 7. Rotation
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R06);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 8. Display Control
	//  : LCD Off
	hs_lcd_cmd_out(HS_LCD_R07);
	hs_lcd_word_dat_out(HS_LCD_DISPLAY_OFF);
	saveLCDOnOff = FALSE;		// LCD Off 상태 저장
	
	// 9. Cursor Control
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R08);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 10. Double-Height Display Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R09);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 11. Vertical Scroll
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R10);
	hs_lcd_word_dat_out(0x0020);

	// 12. Horizontal Cursor Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R11);
	hs_lcd_word_dat_out(HS_LCD_INIT);
	
	// 13. Vertical Cursor Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R12);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 15. Initialize the screens. 
	// Setup the LCD from hs_screen by calling hs_update_screen()
	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (LCD_TOTAL_ADDRESS_Y * LCD_TOTAL_ADDRESS_X + 1); nIndex++) {
		hs_lcd_word_dat_out(0x0000);
	}

	// Initialize LcdByteDataBuffer.
#if (MMI_TYPE == MMI_QUALCOMM)
	memset(LCDDataBuffer, 0x00, sizeof(LCDDataBuffer));	
#else
	memset(LCDWordDataBuffer, 0x00, sizeof(LCDWordDataBuffer));	
	memset(LCDWordAniBuffer, 0x00, sizeof(LCDWordAniBuffer));
	memset(LCDWordAniMaskBuffer, 0xff, sizeof(LCDWordAniMaskBuffer));
#endif

  /* Set the initial value of the annunciators
  */
	hs_update_annun();
	hs_BootLogoDisplay(); // jwpark 00.09.26 : remove PM1000 Feature
} /* end of hs_lcd_init */


/*===========================================================================

EXTERN FUNCTION HS_LCD_SLEEP		// kmh 00.02.16

DESCRIPTION
   In the Sleep Mode, the device stops all internal display operations,
   thus reducint current confumption. 
   Specifically, LCD operation is completely halted.
   (It means LCD control is turned off.)

   * Power control register : HS_LCD_R03
  
	  -------------------------------------------------
	  | 2^7 | 2^6 | 2^5 | 2^4 | 2^3 | 2^2 | 2^1 | 2^0 | 
	  -------------------------------------------------

	        2^1 : Sleep Control bit
			      1 = Sleep Start
				  0 = Sleep Exit 
   
DEPENDENCIES
  Do not change any other bits of HS_LCD_R03 except of Sleep control bit
  Any other bits must be same as the state of HS_LCD_INIT()

RETURN VALUE
  None

SIDE EFFECTS
  <주의> 00.05.16 by kmh 
  현재는 이 함수를 사용하지 않으나, 만약 이 함수를 불러 사용하게 될 시에는 
  HS_LCD_R03에 Setting하는 값을 global로 저장한후 LCD display시마다 Reset
  할 때 그 값으로 Setting할 수 있도록 변경하여 사용하여야 한다.

===========================================================================*/
void hs_lcd_sleep ( boolean bSleep )
{
	// 00.08.23 kmh insert for HS_LCD_RESET
	// save Current Sleep State
	saveLCDSleep = bSleep;

	// change SLP bit of HS_LCD_R03
	hs_lcd_cmd_out ( HS_LCD_R03 );
	hs_lcd_word_dat_out ( bSleep ? 0x091a : 0x0918 );

} /* end of hs_lcd_sleep */


/*===========================================================================

EXTERN FUNCTION HS_LCD_RESET_INSTRUCTION		// kmh 00.02.16

DESCRIPTION
   Noise등의 영향으로 Data의 전송 누락이 발생하면
   표시 System이 폭주하게 되므로, 이를 방지하기 위해
   상위, 하위의 8 Bit Data 전송을 Count하는 내부 
   Counter를 정기적으로 Reset해 준다.
   
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hs_lcd_reset_instruction ( void )
{
	hs_lcd_cmd_out(HS_LCD_R00);
	hs_lcd_cmd_out(HS_LCD_R00);
	hs_lcd_cmd_out(HS_LCD_R00);
	hs_lcd_cmd_out(HS_LCD_R00);
} /* end of hs_lcd_sleep */

/*===========================================================================

EXTERN FUNCTION HS_LCD_RESET
   LCD가 폭주할 경우를 대비하여 LCD의 기본적인 Register를 Setting한다.
   이때 평소 사용하는 Register는 제외한다.
   
DESCRIPTION
   
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_lcd_reset( void )
{
	extern word saveLCDContrast;

	/*----------------------------------------------------
	   00.05.15
	   MSMP 전원이 낮아지는 현상을 대비하여 LCD Display
	   시마다 전원 Setting을 다시 해 준다.
	-----------------------------------------------------*/
	/* Setup the LDO voltage levels */
	// kmh 00.06.05 
	// catnap 구간에서 화면 update시 전류 많이 소모되는 것을 방지하기 위해 삭제함. 
//	PM1000_OUTM( PM1000_PROG_LDO_1_R, PM1000_MSMC_LDO_M | PM1000_MSMC_LDO_EN_M,
//									  PM1000_MSMC_LDO_EN_M | (MSMC_LEVEL << 3));
//	PM1000_OUT ( PM1000_PROG_LDO_2_R, (MSMP_LEVEL << 4) | MSMA_LEVEL );
//	PM1000_OUT ( PM1000_PROG_LDO_3_R, (IF_LEVEL << 4) | TCXO_LEVEL );
//	PM1000_OUT ( PM1000_PROG_LDO_4_R, (SYNT_LEVEL << 4) | RFTX_LEVEL );
//	PM1000_OUTM( PM1000_PROG_LDO_5_R, PM1000_RFRX_LDO_M, RFRX_LEVEL << 4 );

#if (MODEL_ID==MODEL_A8)
  /* Enable the LCD serial interface outputs */
  BIO_OUT( DEC_GPIO_0, BIO_LCD_SEL1_M | BIO_LCD_A0_M | BIO_LCD_DATA_M,
           BIO_LCD_DESEL1_V | BIO_LCD_A0_HI_V | BIO_LCD_DATA_V );

  /* Enable the SUB LCD serial interface outputs */
  BIO_TRISTATE( DEC_GPIO_0, BIO_LCD_SEL1_M | BIO_LCD_A0_M | BIO_LCD_DATA_M,
                BIO_OUTPUT_ENA_V );
#endif

	// 1. Start Oscillation executed 
	//  : Stand by mode의 halt 상태로부터 Oscillator를 다시 재시작 시킴.
	hs_lcd_cmd_out(HS_LCD_R00);
	hs_lcd_word_dat_out(HS_LCD_START_OSC);
	
	// 2. Driver Output Control 
	//  : 128 * 96 dots Set.
	// 00.01.20 LCD New Spec에 의해 변경 0x024b -> 0x034b
	hs_lcd_cmd_out(HS_LCD_R01);
	hs_lcd_word_dat_out(0x024b);

	// 3. Waveform AC Drive 
	// 00.04.04 LCD New Spec 0x0000 -> 0x005c
	hs_lcd_cmd_out(HS_LCD_R02);
#if MODEL_ID == MODEL_A8	// kmh modify - 교세라 요청으로 Setting Value 수정
	hs_lcd_word_dat_out(0x005d);
#else
	hs_lcd_word_dat_out(0x005c);
#endif
	
	// 4. Power Control & Bias Set & Boost Set
	//  : LCD drive Bias Value -> 1/10 bias drive
	//    V5OUT Output Level -> 6 Times Boost
	//    Operating Clock Frequency in the Booster -> 16 Divided clock
	//    Amount of Fixed Current in the Operational Amplifier -> Small
	// 00.03.02 LCD New Spec에 의해 변경 0x0614 --> 0x0918
	// 현재 Sleep 상태를 반영	00.08.23
	hs_lcd_cmd_out ( HS_LCD_R03 );
	hs_lcd_word_dat_out ( saveLCDSleep ? 0x091a : 0x0918 );

	// 5. Contrast Control
	//  : Default Contrast Value Set
	hs_lcd_cmd_out(HS_LCD_R04);
	hs_lcd_word_dat_out(saveLCDContrast);

	// 6. Entry Mode 
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R05);
	hs_lcd_word_dat_out(0x0000);

	// 7. Rotation
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R06);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 8. Display Control
	// 현재 LCD On/Off 상태를 Check하여 적용하도록 한다.
	hs_lcd_cmd_out(HS_LCD_R07);
	if (saveLCDOnOff)	hs_lcd_word_dat_out(HS_LCD_DISPLAY_ON);
	else				hs_lcd_word_dat_out(HS_LCD_DISPLAY_OFF);
	
	// 10. Double-Height Display Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R09);
	hs_lcd_word_dat_out(HS_LCD_INIT);

	// 11. Vertical Scroll
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R10);
	hs_lcd_word_dat_out(0x0020);

	// 12. Horizontal Cursor Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R11);
	if (saveLCDCusorBlink == HS_LCD_BLINK_OFF)
		hs_lcd_word_dat_out(HS_LCD_INIT);
	else
		hs_lcd_word_dat_out(saveLCDCusorBlinkX);
	
	// 13. Vertical Cursor Position
	//  : Initialize
	hs_lcd_cmd_out(HS_LCD_R12);
	if (saveLCDCusorBlink == HS_LCD_BLINK_OFF)
		hs_lcd_word_dat_out(HS_LCD_INIT);
	else
		hs_lcd_word_dat_out(saveLCDCusorBlinkY);

	// 9. Cursor Control
	//  : Initialize
	// 현재 Blink 상태를 반영 -> Blink할 위치(HS_LCD_R11,HS_LCD_R12)를
	// 지정해 준 후 Active함.	00.08.23 kmh insert
	hs_lcd_cmd_out(HS_LCD_R08);
	if (saveLCDCusorBlink == HS_LCD_BLINK_OFF)
		hs_lcd_word_dat_out(HS_LCD_CURSOR_OFF);
	else
		hs_lcd_word_dat_out( HS_LCD_CURSOR_START | (saveLCDCusorBlink & 0x03) );

} /* end of hs_lcd_reset */

/*===========================================================================

EXTERN FUNCTION HS_LCD_BLINK		// kmh 00.02.23

DESCRIPTION
   현재 선택된 Area를 BlinkMode에 따라 Blink한다.
   이 함수를 통한 Blink는 전체 LCD에서 단 하나의 Area에만 적용할 수 있다.
   이 때 선택 Area는 Icon Area와 Text Display Area의 구분 없이 사용한다. 
   
   < Argument Description >

   1) Blink Area 

	                   nPosX (Start X Position)
					     |
	                     |
	         nPoxY  ---- ------------  -
	 (Start Y Position)  |          |  |
	 					 |  Blink   |  |--> nHeight
					     |  Cursor  |  |
					     |          |  | 
				         ------------  -
						 |- nWidth -|
   
   2) Blink Mode

      < nBlinkMode >
	    HS_LCD_CURSOR_OFF : Blink Off
	    HS_LCD_WHITE_BLINK : White-blink Cursor
		                     선택된 Area의 현재 Display bit와 And되어 Blink함.
		                     따라서 현재 Display되어 있는 Bit들이 Blink됨. 
        HS_LCD_BLACK_BLINK : Black-blink Cursor
		                     선택된 Area의 현재 Display Bit와 Or되어 Blink함.
			                 따라서 현재 선택된 Area가 전체 On되어 Blink됨.
        HS_LCD_BLACK_WHITE_REVERSE : Black-and-white reversed Cursor
		                             선택된 Area의 현재 Display Bit와 Xor되어 Display되고 
									 Blink는 하지 않음.
			                         따라서 현재 선택된 Area의 Reverse display가 됨.
        HS_lCD_BLACK_WHITE_BLINK : Black-and-White blink Cursor
		                           선택된 Area의 현재 Display Bit와 Xor되어 Blink됨.

   3) Cursor control register : HS_LCD_R08
  
	  -----------------------------------------------------------------------
	  |2^15|2^14|2^13|2^12|2^11|2^10|2^9|2^8|2^7|2^6|2^5|2^4|2^3|2^2|2^1|2^0|  
	  -----------------------------------------------------------------------

      2^15 ~ 2^3 : Not Used
	         2^2 : 0 = Not Use cursor display control      
		 	       1 = Cursor display is started
      2^1  ~ 2^0 : 0 = White-blink Cursor
                   1 = Black-blink Cursor
				   2 = Black-and-white reversed Cursor
				   3 = Black-and-White blink Cursor

   3) Window Cursor Control Register 

      < HS_LCD_R11 : Horizontal Cursor Position >
	  -----------------------------------------------------------------------
	  |2^15|2^14|2^13|2^12|2^11|2^10|2^9|2^8|2^7|2^6|2^5|2^4|2^3|2^2|2^1|2^0|  
	  -----------------------------------------------------------------------

             2^15 : Not Used
	  2^14 ~ 2^08 : End X Address Set
             2^07 : Not Used
	  2^06 ~ 2^00 : Start X Address Set
      

      < HS_LCD_R12 : Vertical Cursor Position >
	  -----------------------------------------------------------------------
	  |2^15|2^14|2^13|2^12|2^11|2^10|2^9|2^8|2^7|2^6|2^5|2^4|2^3|2^2|2^1|2^0|  
	  -----------------------------------------------------------------------

             2^15 : Not Used
	  2^14 ~ 2^08 : End Y Address Set
             2^07 : Not Used
	  2^06 ~ 2^00 : Start Y Address Set


DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hs_lcd_blink ( int nPosX, int nPosY, int nWidth, int nHeight, int nBlinkMode )
{
	/*----------------------------------
	  Blinking Area가 전체 LCD Area를 
      벗어나면 그냥 Return한다. 	   
    ----------------------------------*/
	if ( ( (nPosX + nWidth) > 120) || ( (nPosY + nHeight) > 96) )
		return;

	// 00.08.23 kmh insert for HS_LCD_RESET
	// Save Current Cusor Blink State
	saveLCDCusorBlink = nBlinkMode;

	/*---------------------------------
	  1. Blink Off Mode
	----------------------------------*/
	if (nBlinkMode == HS_LCD_BLINK_OFF)
	{
		/*---------------------------------
		  1. Cursor Control Off
		----------------------------------*/
		hs_lcd_cmd_out(HS_LCD_R08);
		hs_lcd_word_dat_out(HS_LCD_CURSOR_OFF);
	}
	/*---------------------------------
	  1. Blink Setting Mode
	----------------------------------*/
	else
	{
		/*----------------------------------
		  1. Horizontal Cursor Position
			 LCD Start X Address = 0x7f 
		----------------------------------*/
		hs_lcd_cmd_out(HS_LCD_R11);
		// 00.08.23 kmh insert for HS_LCD_RESET
		// Save Current Cusor Blink State
		saveLCDCusorBlinkX = ( (0x7f - nPosX) << 8 ) | ( 0x7f - nPosX - nWidth ); 
		hs_lcd_word_dat_out(saveLCDCusorBlinkX);
		
		/*----------------------------------
		  2. Vertical Cursor Position
			 LCD Start Y Address = 0x5f
		----------------------------------*/
		hs_lcd_cmd_out(HS_LCD_R12);
		// 00.08.23 kmh insert for HS_LCD_RESET
		// Save Current Cusor Blink State
		saveLCDCusorBlinkY = ( (0x5f - nPosY) << 8 ) | (0x5f - nPosY - nHeight);
		hs_lcd_word_dat_out(saveLCDCusorBlinkY);

		/*---------------------------------
		  3. Cursor Control Start
		----------------------------------*/
		hs_lcd_cmd_out(HS_LCD_R08);
		hs_lcd_word_dat_out( HS_LCD_CURSOR_START | (nBlinkMode & 0x03) );
	}

} /* end of hs_lcd_blink */

/*-----------------------------------------------------------------------
                         LCD 
-----------------------------------------------------------------------*/

/*===========================================================================

LOCAL FUNCTION DisplayLCDBufferAll

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void DisplayLCDBufferAll ( void )
{
	int index;

	// for LCD test by msrock
	if (LcdTestFlag) return;

	if(!bInvisable_Charge)		// off 충전 모드가 아닌 경우에만 실행함 
	{
		/*----------------------------------------------------
		   00.05.15
		   LCD 폭주 현상이 일어날 경우를 대비하여 
		   LCD Display시마다 Initialize를 한다.
		-----------------------------------------------------*/
		hs_lcd_reset();
	}

#if (MMI_TYPE == MMI_QUALCOMM) 
	hs_lcd_reset_instruction();
	// Ram Address Set Register
	hs_lcd_cmd_out(HS_LCD_R17);

	// girin. 2000.1.31. Reserve picto area.
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR);		// Start Address set
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR - (16*16)); // Start Address set. 0x07ff
	// Use constant value for performance improvement.
	hs_lcd_word_dat_out(0x06ff); // Should be same to HS_LCD_START_ADDR - 16 * 16.

	hs_lcd_reset_instruction();
	hs_lcd_cmd_out(HS_LCD_R18);

	for (index = 0 ; index < sizeof ( LCDDataBuffer ) ; index++)
	{
		hs_lcd_onebit_dat_out(*((byte *)LCDDataBuffer + index));
	}
#else
	// Ram Address Set Register
	hs_lcd_reset_instruction();
	hs_lcd_cmd_out(HS_LCD_R17);

	// girin. 2000.1.31. Reserve picto area.
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR);		// Start Address set
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR - (16*16)); // Start Address set. 0x07ff
	// Use constant value for performance improvement.
	hs_lcd_word_dat_out(0x06ff); // Should be same to HS_LCD_START_ADDR - 16 * 16.

	hs_lcd_reset_instruction();
	hs_lcd_cmd_out(HS_LCD_R18);

	for (index = 0 ; index < (sizeof ( LCDWordDataBuffer )); index++) 
	{
		// edited by chul for performance improvement
		//hs_lcd_byte_dat_out(*((byte *)LCDWordDataBuffer + index));
		//INTLOCK();	// delete for performance improvement by kmh 00.03.28
		LCD_OUTP_DATA(*((byte*)LCDWordDataBuffer+index));
		//INTFREE();	// delete for performance improvement by kmh 00.03.28
	}
#endif
}

#if (MMI_TYPE == MMI_SK) || (MMI_TYPE == MMI_QUALCOMM) || (MMI_TYPE == MMI_A10)
/*===========================================================================

FUNCTION SetASCIIFont

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
int	SetASCIIFont ( int nFont )
{
	int	nOldFont = nASCIIFont;	// 이전 값을 return 하기 위해

	nASCIIFont = nFont;

	switch ( nASCIIFont )
	{
	case MED_SIZE_ASCII_FONT:
		nLcdLeftMargin = 0;
		nScreenWidth = MED_SCREEN_WIDTH;

		nASCIIFontWidth = MED_ASCII_FONT_WIDTH;
		nASCIIFontHeight = MED_ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = MED_ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)MedASCIIFontBuffer;
		break;

	case	BIG_SIZE_ASCII_FONT :
		nLcdLeftMargin = 0;
		nScreenWidth = SCREEN_WIDTH;

		// ASCII font 크기에 관한 값들을 big size로
		nASCIIFontWidth = BIG_ASCII_FONT_WIDTH;
		nASCIIFontHeight = BIG_ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)BigASCIIFontBuffer;
		break;

	case	SMALL_SIZE_ASCII_FONT :
		nLcdLeftMargin = LCD_LEFT_MARGIN;

		// ASCII font 크기에 관한 값들을 small size로
		nASCIIFontWidth = SMALL_ASCII_FONT_WIDTH;
		nASCIIFontHeight = SMALL_ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = SMALL_ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)SmallASCIIFontBuffer;
		break; 

	case	CALENDAR_FONT :
		nLcdLeftMargin = 0;

		// ASCII font 크기에 관한 값들을 small size로
		nASCIIFontWidth = CALENDAR_FONT_WIDTH;
		nASCIIFontHeight = CALENDAR_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = CALENDAR_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)SmallASCIIFontBuffer;
		break;

	case	NORMAL_SIZE_ASCII_FONT :
	default :
		nLcdLeftMargin = LCD_LEFT_MARGIN;
		nScreenWidth = SCREEN_WIDTH;

		// ASCII font 크기에 관한 값들을 표준 size로
		nASCIIFontWidth = ASCII_FONT_WIDTH;
		nASCIIFontHeight = ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)ASCIIFontBuffer;
		break;
	}
	return ( nOldFont );
}		// end of SetASCIIFont

/*===========================================================================

FUNCTION SetHanFont

DESCRIPTION
	Set Korean & ASCII font type

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
	Including Korean font type, ASCII font type will be changed

===========================================================================*/
int	SetHanFont ( int nFont )
{
	int	nOldFont = nASCIIFont;	// 이전 값을 return 하기 위해

	nHanFont = nFont;

	switch ( nHanFont )
	{
	case MED_SIZE_HAN_FONT:
		nLcdLeftMargin = 0;
		nScreenWidth = MED_SCREEN_WIDTH;

		nHanFontWidth = MED_HAN_FONT_WIDTH;
		nHanFontHeight = MED_HAN_FONT_HEIGHT;
		nHanFontBufferSizePerChar = MED_HAN_FONT_BUFFER_SIZE_PER_CHAR;
		pHanFontBuffer = (byte *)MedHanFontBuffer;
		pSpecialFontBuffer = (byte *)MedSpecialFontBuffer;
		pSKFontBuffer = (byte *)MedSymbolFontBuffer;

		nASCIIFontWidth = MED_ASCII_FONT_WIDTH;
		nASCIIFontHeight = MED_ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = MED_ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)MedASCIIFontBuffer;
		break;

	case	NORMAL_SIZE_ASCII_FONT :
	default :
		nLcdLeftMargin = LCD_LEFT_MARGIN;
		nScreenWidth = SCREEN_WIDTH;

		// ASCII font 크기에 관한 값들을 표준 size로
		nHanFontWidth = HAN_FONT_WIDTH;
		nHanFontHeight = HAN_FONT_HEIGHT;
		nHanFontBufferSizePerChar = HAN_FONT_BUFFER_SIZE_PER_CHAR;
		pHanFontBuffer = (byte *)HanFontBuffer;
		pSpecialFontBuffer = (byte *)SpecialFontBuffer;
		pSKFontBuffer = (byte *)SymbolFontBuffer;

		nASCIIFontWidth = ASCII_FONT_WIDTH;
		nASCIIFontHeight = ASCII_FONT_HEIGHT;
		nASCIIFontBufferSizePerChar = ASCII_FONT_BUFFER_SIZE_PER_CHAR;
		pASCIIFontBuffer = (byte *)ASCIIFontBuffer;
		break;
	}
	return ( nOldFont );
}		// end of SetASCIIFont


/*===========================================================================

LOCAL FUNCTION GETASCIIFONTBUFFER

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static byte *GetASCIIFontBuffer ( word nCode )
{
    byte *pFont;

    nCode &= 0x7f;

    pFont = (byte *)pASCIIFontBuffer + ( nCode * nASCIIFontBufferSizePerChar );
    return ( pFont );
}		// end of GetASCIIFontBuffer

/*===========================================================================

FUNCTION		GetHanFontBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#if (MMI_TYPE == MMI_QUALCOMM)	// woodstok 00.5.2 used Only MMI_QUALCOMM

static byte	*GetHanFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	// SK specified font
	if (nCode >= 0x81A0 && nCode <= 0x8aFF) 
	{
		nCode -= 0x81A0;
		nColumn = nCode % 0x100; 
		nRow = nCode / 0x100;
		nIndex = nRow * 0x60 + nColumn;
		pFont = (byte*)pSKFontBuffer + nIndex * nHanFontBufferSizePerChar;		
	}
	// KS5601 special font
	else if (nCode >= 0xA1A1 && nCode <= 0xACFE) {
		nCode -= 0xA1A1;
		nColumn = nCode % 0x100;
		nRow = nCode / 0x100;
		nIndex = nRow * 0x5e + nColumn;

		pFont = (byte*)pSpecialFontBuffer +
			nIndex * nHanFontBufferSizePerChar;
		return ( pFont );
	}
	// KS5601 Korean font
	else if(nCode >= 0xB0A1 && nCode <= 0xC9AA)
	{
		nCode -= 0xb0a1;
		nColumn = nCode % 0x100;
		nRow = nCode / 0x100;
		nIndex = nRow * 0x5e + nColumn ;

		pFont = (byte*)pHanFontBuffer +
			nIndex * nHanFontBufferSizePerChar;
	}
	else // erronous nCode. 2-byte space used instead.
	{
		nCode -= 0xA1A1;
		nColumn = nCode % 0x100;
		nRow = nCode / 0x100;
		nIndex = nRow * 0x5e + nColumn;

		pFont = (byte*)pSpecialFontBuffer + 0; // means 2-byte space (0xA1A1)
	}

	return ( pFont );
}		// end of GetHanFontBuffer
#endif

static byte	*GetHangulFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	// KS5601 Korean font
	nCode -= 0xb0a1;
	nColumn = nCode % 0x100;
	nRow = nCode / 0x100;
	nIndex = nRow * 0x5e + nColumn ;

	pFont = (byte*)pHanFontBuffer +
		nIndex * nHanFontBufferSizePerChar;

	return ( pFont );
}

static byte	*GetSKSymbolFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	// SK specified font
	nCode -= 0x81A0;
	nColumn = nCode % 0x100; 
	nRow = nCode / 0x100;
	nIndex = nRow * 0x60 + nColumn;
	pFont = (byte*)pSKFontBuffer + nIndex * nHanFontBufferSizePerChar * 2; // See doubling !!		

	return ( pFont );
}


static byte	*GetKS5601SpecialFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	// KS5601 special font
	nCode -= 0xA1A1;
	nColumn = nCode % 0x100;
	nRow = nCode / 0x100;
	nIndex = nRow * 0x5e + nColumn;

	pFont = (byte*)pSpecialFontBuffer +
		nIndex * nHanFontBufferSizePerChar;

	return ( pFont );
}

static byte	*GetSpaceFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	// erronous nCode. 2-byte space used instead.
	nCode -= 0xA1A1;
	nColumn = nCode % 0x100;
	nRow = nCode / 0x100;
	nIndex = nRow * 0x5e + nColumn;

	pFont = (byte*)pSpecialFontBuffer + 0; // means 2-byte space (0xA1A1)

	return ( pFont );
}

/*===========================================================================

FUNCTION		DrawBitToLCDBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void DrawBitToLCDBuffer ( int nIndexX, int nIndexY,	
						  byte DrawPattern, byte MaskBitPattern,
						  int nDrawMode )
{
	byte buf = 0;
	word dataBuffer = 0;
//	byte i = 0, j = 0;
	extern const word	ani_Color2ToColor4_tab[256];

	switch	( nDrawMode )
	{
	case	DRAW_NORMAL_BLINK :
		// draw data buffer
		if ( !hs_blink_active )	// normal 상태
		{
			// normal mode와 같다
			buf &= ~MaskBitPattern;
			buf |= DrawPattern;
		}
		else							// reverse 상태
		{
			// 현재 blink하여 reverse 상태면 reverse mode와 같다
			buf |= MaskBitPattern;
			buf ^= DrawPattern;
		}
		break;

    case DRAW_REVERSE :
		  buf |= MaskBitPattern;
		  buf ^= DrawPattern;
		break;

    case DRAW_NORMAL :
	  default :
		  buf &= ~MaskBitPattern;
		  buf |= DrawPattern;
		break;
  } /* end of switch loop */

#if (MMI_TYPE == MMI_SK) || (MMI_TYPE == MMI_A10)
#if (0)
	// Doubling.
	// LCD의 GrayScale Level 0, 1, 2, 3중에서 Font는 0과 4로 구성
	// 따라서 Font Data는 "00" 아니면 "11"로 구성한다.
	if (buf)
	{
		for (i=0; i < 8; i++, j+= 2)
		{ 
			if (buf & (HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
				dataBuffer |= (HS_LCD_MASK_BIT >> j);	// #define HS_LCD_MASK_BIT	0xc000
			else 
				dataBuffer &= ~(HS_LCD_MASK_BIT >> j);
		}
	}	
	else	 dataBuffer = 0;		
#else
	// girin. 2000.3.29
	// Enhanced to use look-up table method
	// Inspired by Chul...
	dataBuffer = ani_Color2ToColor4_tab[buf];
#endif

	LCDWordDataBuffer[ nIndexY ][ nIndexX*2 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);
	LCDWordDataBuffer[ nIndexY ][ nIndexX*2 + 1 ] = (byte)HS_LCD_LOW_DATA(dataBuffer);

#elif (MMI_TYPE == MMI_QUALCOMM)
	LCDDataBuffer[ nIndexY ][ nIndexX ] = buf;
#else
#error Invalid MMI_TYPE
#endif
}		// end of DrawBitToLCDBuffer

/*===========================================================================

FUNCTION OutCharLCDBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
word OutCharLCDBuffer
( 
  byte *pFont, 
  int nPosX, 
  int nPosY,
  int nFontWidth, 
  int nFontHeight, 
  int nDrawMode 
)
{
	DWORD_HWORD	BitPattern, BitMask;
	int		nIndexX, nIndexY, nBitPosX;
	word	i;	
	word    ret_width;

#ifdef TEMPORARY_COMPILE
	if( (unsigned long)pFont < 
			(unsigned long)(MedSymbolFontBuffer + 704*MED_HAN_FONT_BUFFER_SIZE_PER_CHAR) &&
		(unsigned long)pFont >= (unsigned long)MedSymbolFontBuffer)
	{
		nFontWidth = MED_HAN_FONT_WIDTH;
		nFontHeight = MED_HAN_FONT_HEIGHT;
	}
#endif
//#ifdef QUALCOMM_SRC
//	if( nPosX <= 96 - nFontWidth) 
//#else		// #ifdef QUALCOMM_SRC
	if( nPosX <= (LCD_WIDTH * 2) - nFontWidth) 
//#endif	// #ifdef QUALCOMM_SRC
		ret_width = nFontWidth;
	else
		ret_width = 0;

	nPosX += nLcdLeftMargin;

	nIndexX = nPosX / 8;
	nBitPosX = nPosX % 8;
	nIndexY = nPosY;
	if ( nBitPosX < 0 )
	{
		nIndexX --;
		nBitPosX = 8 + nBitPosX;
	}

	BitMask.DWord = ~ ( (dword)0xffffffff >> nFontHeight );
	BitMask.DWord >>= nBitPosX;

	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont ++ )
	{
		if ( nIndexX >= -4 )
		{
			BitPattern.DWord = 0;	
			BitPattern.HWord.high.Byte.high = *pFont;
			if ( nFontWidth > 8 )
				BitPattern.HWord.high.Byte.low = *( pFont + nFontHeight );
			if ( nFontWidth > 16 )
				BitPattern.HWord.low.Byte.high = *( pFont + nFontHeight * 2 );
			BitPattern.DWord >>= nBitPosX;
		
			if ( ( nIndexX >= 0 ) && ( nIndexX < LCD_TOTAL_ADDRESS_X ) )
			{
				DrawBitToLCDBuffer ( nIndexX, nIndexY,
					BitPattern.HWord.high.Byte.high,
					BitMask.HWord.high.Byte.high,
					nDrawMode );
			}
		
			if ( ( nIndexX + 1 >= 0 ) &&
				( nIndexX + 1 < LCD_TOTAL_ADDRESS_X ) )
			{
				DrawBitToLCDBuffer ( nIndexX + 1, nIndexY,
					BitPattern.HWord.high.Byte.low,
					BitMask.HWord.high.Byte.low,
					nDrawMode );
			}
		
			if ( ( nIndexX + 2 >= 0 ) &&
				( nIndexX + 2 < LCD_TOTAL_ADDRESS_X ) &&
				( nFontWidth > 9 ) )
			{
				DrawBitToLCDBuffer ( nIndexX + 2, nIndexY,
					BitPattern.HWord.low.Byte.high,
					BitMask.HWord.low.Byte.high,
					nDrawMode );
			}
	
			if ( ( nIndexX + 3 >= 0 ) &&
				( nIndexX + 3 < LCD_TOTAL_ADDRESS_X ) &&
				( nFontWidth > 17 ) )
			{
				DrawBitToLCDBuffer ( nIndexX + 3, nIndexY,
					BitPattern.HWord.low.Byte.low,
					BitMask.HWord.low.Byte.low,
					nDrawMode );
			}
		}
	}

	return ( ret_width);	
}			// end of OutCharLCDBuffer
#endif

#if (MMI_TYPE == MMI_SK) || (MMI_TYPE == MMI_A10) // girin. 2000.1.27

//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////

INLINE byte *Widening(byte *pFont, byte nBytePerChar)
{
	static byte WordOneCharBuffer[BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR*2];
//	byte i, j;
	byte indx;
	word dataBuffer;

	extern const word ani_Color2ToColor4_tab[256]; // defined in Ani.c

	for (indx = 0; indx < nBytePerChar; indx++)
	{
#if (0)
		if (*pFont)
		{
			for (i=0, j=0, dataBuffer = 0; i < 8; i++, j+= 2)
			{ 
				if (*pFont & (HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
					dataBuffer |= (HS_LCD_MASK_BIT >> j);	// #define HS_LCD_MASK_BIT	0xc000
				else 
					dataBuffer &= ~(HS_LCD_MASK_BIT >> j);
			}
		}	
		else	 dataBuffer = 0;
#else
		// girin. 2000.3.29
		// Enhanced to use look-up table method
		// Inspired by Chul...
		dataBuffer = ani_Color2ToColor4_tab[*pFont];
#endif

		// A7 Proejct Only: Because Hangul and ASCII font uses the .def files from B5 project and those files are not 
		// arranged to fit in little-endian by FontTool.exe of earlier version, the conversion process from 
		// big-endian to little endian is done here. 
		// If those .def files are to be arranged someday, switch the assigning order like below example :
		// 
		// WordOneCharBuffer[ indx*2 ]     = (byte)HS_LCD_LOW_DATA(dataBuffer);
		// WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);

		WordOneCharBuffer[ indx*2 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);
		WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_LOW_DATA(dataBuffer);

		pFont++;
	}

	return WordOneCharBuffer;
}


INLINE byte *Widening4Gray(byte *pFont, byte nBytePerChar, byte by4Gray)
{
	static byte WordOneCharBuffer[BIG_ASCII_FONT_BUFFER_SIZE_PER_CHAR*2];
//	byte i, j, indx;
//	word dataBuffer;

#if (0) 
	// Hey man !! If you want to use this function, use it after you modify this function
	// using table look-up method , rather bitwise operation
	// Don't want ? Then, I have no responsibility on your code's performance problem...

	for (indx = 0; indx < nBytePerChar; indx++)
	{
		dataBuffer = 0;

		if (*pFont)
		{
			for (i=0, j=0, dataBuffer = 0; i < 8; i++, j+= 2)
			{ 
				if (*pFont & (HS_LCD_CHECK_BIT >> i)) 	// #define HS_LCD_CHECK_BIT 0x80
				{
					switch (by4Gray)
					{
					case BLACKGRAY:
						dataBuffer |= (0x8000 >> j);	// #define HS_LCD_MASK_BIT	0xc000
						break;
					default:
						dataBuffer |= (HS_LCD_MASK_BIT >> j);
						break;
					}
				}
				else 
					dataBuffer &= ~(HS_LCD_MASK_BIT >> j);
			}
		}	
		else	 dataBuffer = 0;		

		// A7 Proejct Only: Because Hangul and ASCII font uses the .def files from B5 project and those files are not 
		// arranged to fit in little-endian by FontTool.exe of earlier version, the conversion process from 
		// big-endian to little endian is done here. 
		// If those .def files are to be arranged someday, switch the assigning order like below example :
		// 
		// WordOneCharBuffer[ indx*2 ]     = (byte)HS_LCD_LOW_DATA(dataBuffer);
		// WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);

		WordOneCharBuffer[ indx*2 ] = (byte)HS_LCD_HIGH_DATA(dataBuffer);
		WordOneCharBuffer[ indx*2 + 1 ] = (byte)HS_LCD_LOW_DATA(dataBuffer);

		pFont++;
	}
#endif

	return WordOneCharBuffer;
}


void TextOutXYnChar(byte *strText, byte *oldText, int nPosX, int nPosY, int nChar, int nDrawMode)
{
	byte *pFont;
	word wChar;
	word nTextWidth = 0, nCharWidth;
	int i;
	byte *pWordFont;
	

	for(i=0; i < nChar; i++)
	{
		if (*strText == 0) break;

		wChar = *strText;

		// girin. 2000.5.20
		if ( wChar == 0xff)
		{
			nTextWidth += 8; // because of word buffer.
			nPosX += 8;
			strText++;

		} else 
		// girin. end.
		if ( wChar & 0x80 )
		{	// 한글 처리
			strText++;
			i++;

			if (*strText == 0) break;

			wChar <<= 8;
			wChar += *strText;

			// Hangul
			if(wChar >= 0xB0A1 && wChar <= 0xC9AA) 
			{
				pFont = GetHangulFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
			// KS5601 special font
			else if (wChar >= 0xA1A1 && wChar <= 0xACFE) 
			{
				pFont = GetKS5601SpecialFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
			// SK specified font
            // chojw 00.12.20 - char 0x8bff reserved for Smart Battery Info
//			else if (wChar >= 0x81A0 && wChar <= 0x8bFF) 
			else if (wChar >= 0x81A0 && wChar < 0x8bFF) 
			{
				pFont = GetSKSymbolFontBuffer(wChar);
				nCharWidth = OutWordCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
#ifdef FEATURE_MEM_FONT
            // chojw 00.10.30 - 메모리 폰트
            // for 대기중 A10 녹음 & Smart Battery 정보 표시
            else if (wChar == 0x8bFF)
            {
				memcpy(&(LCDWordDataBuffer[50 + i][0]), &SmbStrBuf, sizeof(word) * 16 * 12);

				// for 1Line Skip
				i += 15; 

				nCharWidth = 16;
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
            }
#endif // FEATURE_MEM_FONT
			// Error. set default (2-byte space)
			else
			{
				pFont = GetSpaceFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}

		} 
		else 
		{
			// ASCII

			switch (wChar)
			{
			// Some ASCII characters are used specially for some display effect and sould be 4-gray-scaled.
			case '\5':
			case '\6':

				pFont = GetASCIIFontBuffer( wChar );
				pWordFont = Widening(pFont, nASCIIFontBufferSizePerChar);	// woodstok
				nCharWidth = OutWordCharLCDBuffer(pWordFont, nPosX, nPosY, nASCIIFontWidth*2, nASCIIFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;

				break;

			// Other ASCII characters are simply widen.
			default:

				pFont = GetASCIIFontBuffer( wChar );
				pWordFont = Widening(pFont, nASCIIFontBufferSizePerChar);
				nCharWidth = OutWordCharLCDBuffer(pWordFont, nPosX, nPosY, nASCIIFontWidth*2, nASCIIFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;

				break;
			}
		}
	}
	
} // end of TextOutXYnChar


void TextOutXYnCharCursor(byte *strText, byte *oldText, int nPosX, int nPosY, int nChar, byte cursX, int nDrawMode)
{
	byte *pFont;
	word wChar;
	word nTextWidth = 0, nCharWidth;
	int i;
	byte *pWordFont;
	

	for(i=0; i < nChar; i++)
	{
		if (*strText == 0) break;

		wChar = *strText;

		if ( wChar & 0x80 )
		{	// 한글 처리
			strText++;
			i++;

			if (*strText == 0) break;

			wChar <<= 8;
			wChar += *strText;

			// Hangul
			if(wChar >= 0xB0A1 && wChar <= 0xC9AA) 
			{
				pFont = GetHangulFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				if(cursX == (i-1)) 
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,DRAW_NORMAL_BLINK);
				else
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
			// KS5601 special font
			else if (wChar >= 0xA1A1 && wChar <= 0xACFE) 
			{
				pFont = GetKS5601SpecialFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				if(cursX == (i-1)) 
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,DRAW_NORMAL_BLINK);
				else
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
			// SK specified font
			else if (wChar >= 0x81A0 && wChar <= 0x8BFF)     
			{
				pFont = GetSKSymbolFontBuffer(wChar);
				if (cursX == (i-1))
					nCharWidth = OutWordCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,DRAW_NORMAL_BLINK);
				else
					nCharWidth = OutWordCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}
			// Error. set default (2-byte space)
			else
			{
				pFont = GetSpaceFontBuffer(wChar);
				pWordFont = Widening(pFont, nHanFontBufferSizePerChar);
				if(cursX == (i-1)) 
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,DRAW_NORMAL_BLINK);
				else
					nCharWidth = OutWordCharLCDBuffer(pWordFont,nPosX,nPosY,nHanFontWidth*2,nHanFontHeight,nDrawMode);
				nTextWidth += nCharWidth;
				nPosX += nCharWidth;
				strText++;
			}

		} 
		else 
		{
			// ASCII
			pFont = GetASCIIFontBuffer( wChar );
			pWordFont = Widening(pFont, nASCIIFontBufferSizePerChar);
			if (cursX == i)
				nCharWidth = OutWordCharLCDBuffer(pWordFont, nPosX, nPosY, nASCIIFontWidth*2, nASCIIFontHeight,DRAW_NORMAL_BLINK);
			else
				nCharWidth = OutWordCharLCDBuffer(pWordFont, nPosX, nPosY, nASCIIFontWidth*2, nASCIIFontHeight,nDrawMode);
			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			strText++;
		}
	}
	
} // end of TextOutXYnChar


//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
//////////////////////
#endif // MMI_SK


#if (MMI_TYPE == MMI_QUALCOMM) 
void	TextOutXY ( byte *strText, byte *oldText, int nPosX, int nPosY,	int nDrawMode )
{
	byte *pFont;
	word wChar;
	word nTextWidth = 0, nCharWidth;
	int cursor_x = 0;					// kmh insert for test 99.10.29
	int i;

	#ifdef ENHANCED_TEXT_MODE_DISPLAY // girin. 2000.1.28
	for(i=0; i < HS_SCRN_WIDE_EX; i++)
	#else
	for(i=0; i < HS_SCRN_WIDE; i++)
	#endif

	{
		if (*strText == 0) break;

		wChar = *strText;
		
		if ( wChar & 0x80 ){		// 한글 처리
			strText++;

			i++;

			if (*strText == 0) break;

			wChar <<= 8;
			wChar += *strText;

			pFont = GetHanFontBuffer( wChar );
			if(nDrawMode == DRAW_NORMAL_BLINK) {
				if(cursor_x == (i-1)) 
					nCharWidth = OutCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth,nHanFontHeight,nDrawMode);
				else  nCharWidth = OutCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth,nHanFontHeight,DRAW_NORMAL);
			} else 
				nCharWidth = OutCharLCDBuffer(pFont,nPosX,nPosY,nHanFontWidth,nHanFontHeight,nDrawMode);
			
			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			strText++;

		} else {                                   // 아스키 처리
			pFont = GetASCIIFontBuffer( wChar );

			if(nDrawMode == DRAW_NORMAL_BLINK) {
				if(cursor_x == i) 
					nCharWidth = OutCharLCDBuffer(pFont, nPosX, nPosY, nASCIIFontWidth,
					                              nASCIIFontHeight,nDrawMode);
				else 
					nCharWidth = OutCharLCDBuffer(pFont, nPosX, nPosY, nASCIIFontWidth,
					                              nASCIIFontHeight,DRAW_NORMAL);
			} else 
				nCharWidth = OutCharLCDBuffer(pFont, nPosX, nPosY, nASCIIFontWidth,
				                              nASCIIFontHeight,nDrawMode);

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			strText++;

		}
	}
	
}			// end of TextOutXY

#endif

/*===========================================================================

LOCAL FUNCTION GETWORDASCIIFONTBUFFER

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#if FONT_DATA != BYTE_DATA

static byte *GetWordASCIIFontBuffer ( word nCode )
{
    byte *pFont;

    nCode &= 0x7f;

    pFont = (byte *)pASCIIWordFontBuffer + ( nCode * nASCIIFontBufferSizePerChar ) * 2;
    return ( pFont );
}		// end of GetWordASCIIFontBuffer


/*===========================================================================

FUNCTION		GetWordHanFontBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static byte *GetWordHanFontBuffer ( word nCode )
{
	int		nRow, nColumn, nIndex;
	byte	*pFont;

	if (nCode >= 0x81A0 && nCode <= 0x8bFF) {
		nCode -= 0x81A0;
		nColumn = nCode % 0x100; 
		nRow = nCode / 0x100;
		nIndex = nRow * 0x60 + nColumn;

		pFont = (byte*)SymbolWordFontBuffer +
			(nIndex * nHanFontBufferSizePerChar) * 2;
		return ( pFont );
	}

	if (nCode >= 0xA1A1 && nCode <= 0xACFE) {
		nCode -= 0xA1A1;
		nColumn = nCode % 0x100;
		nRow = nCode / 0x100;
		nIndex = nRow * 0x5e + nColumn;

		pFont = (byte*)SpecialWordFontBuffer +
			(nIndex * nHanFontBufferSizePerChar) * 2;
		return ( pFont );
	}

	nCode -= 0xb0a1;
	nColumn = nCode % 0x100;
	nRow = nCode / 0x100;
	nIndex = nRow * 0x5e + nColumn;

	pFont = (byte*)HanWordFontBuffer +
		(nIndex * nHanFontBufferSizePerChar) * 2;

	return ( pFont );
}		// end of GetWordHanFontBuffer
#endif

#if (MMI_TYPE == MMI_SK) || (MMI_TYPE == MMI_A10)
/*===========================================================================

FUNCTION		DrawWordBitToLCDBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void DrawWordBitToLCDBuffer ( int nIndexX, int nIndexY,	
						  byte DrawPattern, byte MaskBitPattern,
						  int nDrawMode )
{
	switch	( nDrawMode )
	{
		case	DRAW_NORMAL_BLINK :
			// draw data buffer
			if ( !hs_blink_active )	// normal 상태
			{
				// normal mode와 같다
				LCDWordDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
				LCDWordDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
			}
			else							// reverse 상태
			{
				// 현재 blink하여 reverse 상태면 reverse mode와 같다
				LCDWordDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
				LCDWordDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
			}
			break;

    case DRAW_REVERSE :
		  LCDWordDataBuffer[ nIndexY ][ nIndexX ] |= MaskBitPattern;
		  LCDWordDataBuffer[ nIndexY ][ nIndexX ] ^= DrawPattern;
		break;

    case DRAW_NORMAL :
	  default :
		  LCDWordDataBuffer[ nIndexY ][ nIndexX ] &= ~MaskBitPattern;
		  LCDWordDataBuffer[ nIndexY ][ nIndexX ] |= DrawPattern;
		break;
  } /* end of switch loop */
}		// end of DrawWordBitToLCDBuffer

/*===========================================================================

FUNCTION OutWordCharLCDBuffer

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static word OutWordCharLCDBuffer
( 
  byte *pFont, 
  int nPosX, 
  int nPosY,
  int nFontWidth, 
  int nFontHeight, 
  int nDrawMode 
)
{
	DWORD_HWORD	BitPattern, BitMask;
	int		nIndexX, nIndexY, nBitPosX;
	word	i;	
    word    ret_width;

	byte BitMaskRear, BitPatternRear;

	if( nPosX <= (LCD_WIDTH * 2)- nFontWidth) 
		ret_width = nFontWidth;
	else
		ret_width = 0;

	nPosX += (nLcdLeftMargin*2);

	nIndexX = nPosX / 8;
	nBitPosX = (nPosX % 8);
	nIndexY = nPosY;
	if ( nBitPosX < 0 )
	{
		nIndexX --;
		nBitPosX = 8 + nBitPosX;
	}

	BitMask.DWord = ~ ( (dword)0xffffffff >> nFontWidth );
	BitMask.DWord >>= nBitPosX;

	// girin. 2000.2.21
	// If a font is splitted between two 'nIndexX', the rear part would be drawn
	// after the front part done.
	if (nBitPosX + nFontWidth > 32)
	{
		BitMaskRear = ~ ((byte)0xff >> (nBitPosX + nFontWidth - 32)) ;
	}
	else
	{
		BitMaskRear = 0;
	}


	for ( i = 0;
		( i < nFontHeight ) && ( nIndexY < LCD_TOTAL_ADDRESS_Y );
		i ++, nIndexY ++, pFont +=2 )
	{
		if ( nIndexX >= -4)
		{
			BitPattern.DWord = 0;	

			BitPattern.HWord.high.Byte.high = *pFont;
			if ( nFontWidth > 8)		
				BitPattern.HWord.high.Byte.low = *( pFont + 1 );
			if ( nFontWidth > 16) 	
				BitPattern.HWord.low.Byte.high = *( pFont + nFontHeight*2);
			if ( nFontWidth > 24 )
				BitPattern.HWord.low.Byte.low = *( pFont + 1 + nFontHeight*2);

			if (BitMaskRear != 0)
			{
				BitPatternRear = BitPattern.HWord.low.Byte.low << (8 - nBitPosX);
			}
			// To pacify the compiler
			else
			{
				BitPatternRear = 0;
			}
	
			BitPattern.DWord >>= nBitPosX;
		
			if ( ( nIndexX >= 0 ) && ( nIndexX < (LCD_TOTAL_ADDRESS_X * 2) ) )
			{
				DrawWordBitToLCDBuffer ( nIndexX, nIndexY,
					BitPattern.HWord.high.Byte.high,
					BitMask.HWord.high.Byte.high,
					nDrawMode );
			}
		
			if ( ( nIndexX + 1 >= 0 ) &&
				( nIndexX + 1 < LCD_TOTAL_ADDRESS_X  * 2) )
			{
				DrawWordBitToLCDBuffer ( nIndexX + 1, nIndexY,
					BitPattern.HWord.high.Byte.low,
					BitMask.HWord.high.Byte.low,
					nDrawMode );
			}
		
			if ( ( nIndexX + 2 >= 0 ) &&
				( nIndexX + 2 < LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 9))
			{
				DrawWordBitToLCDBuffer ( nIndexX + 2, nIndexY,
					BitPattern.HWord.low.Byte.high,
					BitMask.HWord.low.Byte.high,
					nDrawMode );
			}
	
			if ( ( nIndexX + 3 >= 0 ) &&
				( nIndexX + 3 < LCD_TOTAL_ADDRESS_X * 2) &&
				( nFontWidth > 17 ))
			{
				DrawWordBitToLCDBuffer ( nIndexX + 3, nIndexY,
					BitPattern.HWord.low.Byte.low,
					BitMask.HWord.low.Byte.low,
					nDrawMode );

				///////////////////////
				// Rear Part Drawing //
				///////////////////////

				if ((BitMaskRear) && (nIndexX + 4 < LCD_TOTAL_ADDRESS_X * 2))
				{
					DrawWordBitToLCDBuffer ( nIndexX + 4, nIndexY,
											BitPatternRear,
											BitMaskRear,
											nDrawMode );
				}
	
			}
		}
	}

	return ( ret_width );	
}			// end of OutWordCharLCDBuffer

#endif // MMI_TYPE == MMI_SK

/*===========================================================================

FUNCTION TextWordOutXY

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
#if FONT_DATA != BYTE_DATA
void	TextWordOutXY ( byte *strText, byte *oldText, int nPosX, int nPosY,	int nDrawMode )
{
	byte *pFont;
	word wChar;
	word nTextWidth = 0, nCharWidth;
	int cursor_x = 0;					// kmh insert for test 99.10.29

	int i;
	// kmh insert start
	nPosX *= 2;
	// kmh insert end


	#ifdef ENHANCED_TEXT_MODE_DISPLAY // girin. 2000.1.28
	for(i = 0; i < HS_SCRN_WIDE_EX; i++)
	#else
	for(i = 0; i < HS_SCRN_WIDE; i++)
	#endif
	{
		if (*strText == 0) break;

		wChar = *strText;
		
		if ( wChar & 0x80 ){		// 한글 처리
			strText++;

			i++;

			if (*strText == 0) break;

			wChar <<= 8;
			wChar += *strText;

			pFont = GetWordHanFontBuffer( wChar );
			if(nDrawMode == DRAW_NORMAL_BLINK) {
				if(cursor_x == (i-1))
					nCharWidth = OutWordCharLCDBuffer(pFont, nPosX + CHAR_BIT_WIDTH, nPosY, nHanFontWidth * 2, 
													nHanFontHeight, nDrawMode);
				else  nCharWidth = OutWordCharLCDBuffer(pFont, nPosX, nPosY, nHanFontWidth * 2,
															nHanFontHeight, DRAW_NORMAL);
			} else	nCharWidth = OutWordCharLCDBuffer(pFont, nPosX, nPosY, nHanFontWidth * 2, 
															nHanFontHeight, nDrawMode);

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			strText++;
		} else {                                   // 아스키 처리
			pFont = GetWordASCIIFontBuffer( wChar );

			if(nDrawMode == DRAW_NORMAL_BLINK) {
				if(cursor_x == i)		nCharWidth = OutWordCharLCDBuffer(pFont, nPosX, nPosY, 
																						ASCII_FONT_WIDTH * 2, nASCIIFontHeight, nDrawMode);
				else 	nCharWidth = OutWordCharLCDBuffer(pFont, nPosX, nPosY, 
															ASCII_FONT_WIDTH * 2, nASCIIFontHeight, DRAW_NORMAL);
			} else 	nCharWidth = OutWordCharLCDBuffer(pFont, nPosX, nPosY, 
															ASCII_FONT_WIDTH * 2, nASCIIFontHeight, nDrawMode);

			nTextWidth += nCharWidth;
			nPosX += nCharWidth;
			strText++;
		}
	}
}			// end of TextWordOutXY

#endif

//**************************
//**************************
//#endif											// #ifdef FONT_DATA == BYTE_DATA
//**************************
//**************************

#ifdef	KMH_ANI_TEST

void CompareAnimation()
{
	int i = 0;
	int nIndex = 0;

	// 1. Eva
	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));
	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Eva[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));
	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Eva_[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

	// 2. Jin1
	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));

	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Jin1[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));

	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Jin1_[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

	// 3. Jin2
	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));
	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Jin2[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

	(void) memset((unsigned short *)LCDPictureBuffer, 0xffff, sizeof(LCDPictureBuffer));
	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_Jin2_[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

}

#endif	// #ifdef	KMH_ANI_TEST

#ifdef LCDTEST
/*===========================================================================

EXTERN FUNCTION HS_LCD_TEST

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void hs_lcd_test( void )
{
#ifdef	KMH_ANI_TEST

	int i = 0;
	int nIndex = 0;

#endif 	// #ifdef	KMH_ANI_TEST

	byte * test_str1=(byte *)"부/가서비스";
//	byte * test_str2=(byte *)"abcdefghabcdefgh";
    byte * test_str3=(byte *)"이름으로찾기";
	byte * test_str4=(byte *)"꼐밖갔예외";
	byte * dummy    =(byte *)"                ";

#if FONT_DATA == BYTE_DATA		// kmh test : font data display
//	TextOutXYnChar( (byte *)test_str1, dummy, 0, 0, 11, DRAW_REVERSE );
//	TextOutXY( (byte*)new_str1, (byte *)dummy, 0, 0, DRAW_NORMAL );
//	TextOutXY( (byte *)test_str2, dummy, 0, 39, DRAW_NORMAL );
	//TextOutXY( (byte *)test_str3, dummy, 0, 16, DRAW_NORMAL );
	//TextOutXY( (byte *)test_str4, dummy, 0, 32, DRAW_NORMAL );
#else			// #if FONT_DATA == BYTE_DATA
//	TextWordOutXY( (byte *)test_str1, dummy, 0, 0, DRAW_NORMAL );
//	TextWordOutXY( (byte *)test_str2, dummy, 0, 39, DRAW_NORMAL );
//	TextWordOutXY( (byte *)test_str3, dummy, 5, 15, DRAW_NORMAL );
//	TextWordOutXY( (byte *)test_str4, dummy, 0, 65, DRAW_NORMAL );
#endif		// #if FONT_DATA == BYTE_DATA


#ifdef	KMH_ANI_TEST

	CompareAnimation();	// 4 Grayscale과 2 Grayscale 화면 비교 
	
	// 1. 화면 TEST 1
	(void) memset((unsigned short *)LCDPictureBuffer, 0x0000, sizeof(LCDPictureBuffer));
	for (i = 0; i < 96 + 1; i++)
		memcpy(&LCDPictureBuffer[i][0], &src_testPicture20[i][0], 30);

	hs_lcd_cmd_out(HS_LCD_R17);
	hs_lcd_word_dat_out(HS_LCD_START_ADDR);

	hs_lcd_cmd_out(HS_LCD_R18);
	for (nIndex = 0; nIndex < (16 * 96) + 1; nIndex++) {
		hs_lcd_word_dat_out(*((unsigned short *)LCDPictureBuffer + nIndex));
	}

#endif	// #ifdef	KMH_ANI_TEST

	DisplayLCDBufferAll( );

} /* end of hs_lcd_test */
#endif	// #ifdef LCDTEST


#if (MMI_TYPE == MMI_SK) || (MMI_TYPE == MMI_A10)

void CopyLCDBufferAll (void)
{
	DisplayLCDBufferEnhanced();
}

void ClearLCDBufferAll(void)
{
	memset ( (byte *)LCDWordDataBuffer,      0x00, sizeof ( LCDWordDataBuffer ) );
	memset ( (byte *)LCDWordAniBlinkBuffer, 0x00, sizeof ( LCDWordAniBlinkBuffer ) );
	memset ( (byte *)LCDWordAniBuffer,      0x00, sizeof ( LCDWordAniBuffer ) );
	memset ( (byte *)LCDWordDataBuffer,      0x00, sizeof ( LCDWordDataBuffer ) );

	memset ( (byte *)hs_lcd_screen, 0x00, sizeof (hs_lcd_screen) );

}

void BlinkLCDDataBuffer ( void )
{
	int	nIndex;

#if (0) // girin. 2000.3.30.
	// LCD data buffer length 만큼 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDWordAniBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte *)LCDWordAniBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte *)LCDWordDataBuffer + nIndex ) ^=
				*( (byte *)LCDWordAniBlinkBuffer + nIndex );
		}
	}

	DisplayLCDBufferAll();
#else
	// LCD data buffer length 만큼 loop
	for ( nIndex = 0; nIndex < sizeof ( LCDWordAniBlinkBuffer ) ; nIndex ++ )
	{
		// blink bit 가 있는지 확인
		if ( *( (byte *)LCDWordAniBlinkBuffer + nIndex ) != 0x00 )
		{
			// blink bit position의 data bit를 반전
			*( (byte *)LCDWordAniBuffer + nIndex ) ^=
				*( (byte *)LCDWordAniBlinkBuffer + nIndex );
		}
	}

	DisplayLCDBufferEnhanced();

#endif
}

// girin .add. 99.5.3
word ScrollTextXY 
( 
  byte *strText,    
  byte *oldText,
  int nPosX, 		
  int nPosY,		
  int nDrawMode	
)
{
	return 0;
} /* end of TextOutXY */
#endif // MMI_TYPE

// girin. 99.4.29
void DisplayLCDBufferLine0(void)
{
#ifdef TEMPORAL_AFFAIRE
	hs_lcd_bri_out(0x1a );
	hs_lcd_bri_out(0x1d );
	hs_lcd_add_out(LCD_COMMAND_DOT_ADDRESS_SET, 0x00, 0x00);
	hs_lcd_scr_out( );
#endif
}

// end.

const word hs_BootLogo[LCD_TOTAL_ADDRESS_Y][LCD_TOTAL_ADDRESS_X-1] = 
{
#ifdef STI_MMI  // woodstok 00.12.11
    #include "itouch017.dhf"
#else
    #include "sky_im2400.dhf"
#endif  // STI_MMI
};

void hs_BootLogoDisplay(void)
{
	word x, y;

	hs_lcd_on(TRUE);

	HS_BACKLIGHT_INIT();  /* Backlight control GPIO */
#if MMI_TYPE == MMI_A10
    HS_LCD_BL_CONTROL(TRUE, TRUE, TRUE);
    HS_KEYPAD_BL_ON();
#else
    HS_BACKLIGHT_ON();    /* Turn on the backlight  */
#endif

    HS_CALL_LED_INIT();   /* Call LED control GPIO  */
	HS_CALL_LED_ON();     // jwpark 00.09.26

	// Ram Address Set Register
	hs_lcd_reset_instruction();
	hs_lcd_cmd_out(HS_LCD_R17);

	// girin. 2000.1.31. Reserve picto area.
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR);		// Start Address set
	//hs_lcd_word_dat_out(HS_LCD_START_ADDR - (16*16)); // Start Address set. 0x07ff
	// Use constant value for performance improvement.
	hs_lcd_word_dat_out(0x06ff); // Should be same to HS_LCD_START_ADDR - 16 * 16.

	hs_lcd_reset_instruction();
	hs_lcd_cmd_out(HS_LCD_R18);

	for (y=0 ; y < LCD_TOTAL_ADDRESS_Y; y++) 
	{
		for(x=0; x<LCD_TOTAL_ADDRESS_X-1; x++)
		{
			//INTLOCK();	// delete for performance improvement by kmh 00.03.28
			LCD_OUTP_DATA(hs_BootLogo[y][x]);
			LCD_OUTP_DATA(hs_BootLogo[y][x] >> 8);
			//INTFREE();	// delete for performance improvement by kmh 00.03.28
		}
		//INTLOCK();	// delete for performance improvement by kmh 00.03.28
		LCD_OUTP_DATA(0x00);
		LCD_OUTP_DATA(0x00);
		//INTFREE();
	}
}
static void BuildForegroundBuffer(byte *scr_ptr, byte pagetype)
{
	word j, addr_op=0;
	int  display_mode =0;
	extern byte cursor_x, cursor_y; // defined in uixscrn.c
									// This is sort of violation of task boundary
									// rule so need to be revised.

	memset(LCDWordDataBuffer, 0x00, sizeof(LCDWordDataBuffer));

	switch(pagetype)
	{
	case PAGE_NORMAL:
	case PAGE_NORMAL_REVERSE_0:
	case PAGE_NORMAL_REVERSE_1:
	case PAGE_NORMAL_REVERSE_2:
	case PAGE_NORMAL_REVERSE_3:

		SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			if ((j + 1) == pagetype)
				display_mode = DRAW_REVERSE;
			else
				display_mode = DRAW_NORMAL;


			if (cursor_y == j)
				TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
										PAGE_NORMAL_CHAR_NUM, 
										cursor_x, display_mode );
			else
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, display_mode );

			addr_op += nASCIIFontHeight;

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

		case PAGE_CAL: //white 00.03.18

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0
			case 1: // Line 1
			case 2: // Line 2
			case 3: // Line 3
			case 4: // Line 4

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				break;
			}


			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;
//white end
	case PAGE_NORMAL_TITLE:

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 3: // Line 0		// woodstok 00.4.17 0->3
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
#ifdef FEATURE_MEM_FONT
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
#else
				TextOutXYnChar( scr_ptr, NULL, 24, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
#endif
				addr_op += nASCIIFontHeight;
				addr_op += 2; // 3-dot width chink.

				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_ASCII_FONT);

				break;


			case 0: // Line 3	// woodstok 00.4.17 3->0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_ASCII_FONT);

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight+1;

				break;

			case 1: // Line 1
			case 2: // Line 2
			case 4: // Line 4
				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				break;
			}


			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_DATA_BANNER_REVERSE:

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 24, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_REVERSE );
				addr_op += nASCIIFontHeight;
				addr_op += 3; // 3-dot width chink.

				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_ASCII_FONT);

				break;

			case 1: // Line 1
			case 2: // Line 2
			case 3: // Line 3
			case 4: // Line 4

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				break;
			}


			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_BIG_ASCII:
	
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 1: // Line 1
				// Skip this line for backward compability to B5.
				break;

			case 0: // Line 0
			case 2: // Line 2
			case 3: // Line 3
				SetASCIIFont(BIG_SIZE_ASCII_FONT); /* set font attrib */

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
											PAGE_BIG_ASCII_CHAR_NUM, 
											cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_BIG_ASCII_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				if (j == 0)
				{
					// One dot chink.
					addr_op += 1; 
				}

				break;

			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_BIG_ASCII2:
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 2: // Line 1
				// Skip this line for backward compability to B5.
				addr_op += (LCD_TOTAL_ADDRESS_Y - HAN_FONT_HEIGHT * 4 - BIG_ASCII_FONT_HEIGHT);
				break;

			case 3: // Line 3
				SetASCIIFont(BIG_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_BIG_ASCII_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;

				break;

			case 4:
			case 0: // Line 0
			case 1: // Line 2
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}
		break;

	case PAGE_BIG_ASCII3:
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
											PAGE_NORMAL_CHAR_NUM, 
											cursor_x, display_mode );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, display_mode );

				addr_op += nASCIIFontHeight;

				break;

			case 1: // Line 1
				addr_op += 6; // 6 dot chink.
				break;

			case 2: // Line 2
			case 3: // Line 3
				SetASCIIFont(BIG_SIZE_ASCII_FONT); /* set font attrib */
				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
											PAGE_NORMAL_CHAR_NUM, 
											cursor_x, display_mode );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, display_mode );

				addr_op += nASCIIFontHeight;

				break;

			case 4: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
											PAGE_NORMAL_CHAR_NUM, 
											cursor_x, display_mode );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, display_mode );

				addr_op += nASCIIFontHeight;

				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}
		break;

///////////////////////////
//  WP_STOPWATCH 에 맞춰서 출력하도록 하는 Page type 
//  DISP_STOPWATCH에서 사용 
//
	case PAGE_STOP_WATCH:
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0	- Med size (Display History)
				addr_op += 1;
				SetASCIIFont(MED_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight+2;

				break;

			case 1:	// Line 1,2 - Not Used
			case 2:
				addr_op += nASCIIFontHeight;
				break;

			case 3:	// Line 3 - Big ASCII (Display Time - hour/min/sec)
				SetASCIIFont(BIG_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_BIG_ASCII_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight+1;

				break;

			case 4: // Line 4  - Normal (Display Softkey)
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight; 
				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_SMALL_CLOCK:
		
		SetASCIIFont(MED_SIZE_ASCII_FONT);
		SetHanFont(MED_SIZE_ASCII_FONT);
		scr_ptr = scr_ptr + HS_SCRN_WIDE_EX; // From second line.

#ifdef FEATURE_FOLDER_PHONE
		TextOutXYnChar( scr_ptr, NULL, 48, 51, 16, DRAW_NORMAL );
#else 
		TextOutXYnChar( scr_ptr, NULL, 48, 67, 16, DRAW_NORMAL );
#endif //FEATURE_FOLDER_PHONE


		break;

#ifdef FEATURE_FOLDER_PHONE	// woodstok 00.6.13
	case PAGE_SMALL_CLOCK_WITHOUT_MENU:
		
		SetASCIIFont(MED_SIZE_ASCII_FONT);
		SetHanFont(MED_SIZE_ASCII_FONT);
		scr_ptr = scr_ptr + HS_SCRN_WIDE_EX; // From second line.

		TextOutXYnChar( scr_ptr, NULL, 48, 67, 16, DRAW_NORMAL );
		break;
#endif	// FEATURE_FOLDER_PHONE

#ifdef GIZMO_A7M
	case PAGE_SMALL_CLOCK_WITH_MENU:

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 1:
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);

				TextOutXYnChar( scr_ptr, NULL, 48, 51, 16, DRAW_NORMAL );
				break;
			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

				TextOutXYnChar( scr_ptr, NULL, 0, 65, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				break;
			default:
				break;
			}
			
			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}
		break;

#endif // GIZMO_A7M

	case PAGE_SMS_NOTIF:
		SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */
		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0
			case 1: // Line 1
			case 2: // Line 2
			case 3: // Line 3
			case 4: // Line 4

				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				break;
			}


			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_REVERSE_2LINES:

		SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			if (j == 1 || j == 2) 
				display_mode = DRAW_REVERSE;
			else
				display_mode = DRAW_NORMAL;


			if (cursor_y == j)
				TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
										PAGE_NORMAL_CHAR_NUM, 
										cursor_x, display_mode );
			else
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, display_mode );

			addr_op += nASCIIFontHeight;

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_ALPHA_EDIT:

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 24, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 1; // 1-dot width chink.

				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_ASCII_FONT);

				break;

			case 1: // Line 1
			case 2: // Line 2
			case 3: // Line 3
			case 4: // Line 4

				if (cursor_y == j)
					TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, cursor_x, DRAW_NORMAL );
				else
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

				if (j == 1)
					addr_op += 2; // 2-dot width chink.

				break;
			}


			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_CALLSTACK_NORMAL:
		
		/*
		for (j = 0; j < HS_SCRN_HIGH_EX; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 1; // 2-dot chink.
				break;

			case 1:
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 4, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 2; // 2-dot width chink.
				break;

			case 2:
			case 3: // Line 3
			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}
		break;
		*/


		for (j = 0; j < HS_SCRN_HIGH_EX; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, 2, DRAW_NORMAL );
				TextOutXYnChar( scr_ptr+6, NULL, 48, addr_op, 10, DRAW_NORMAL ); // dot pos * 2 = physical pos, cause, word buf.
				addr_op += nASCIIFontHeight;
				addr_op += 1; // 2-dot chink.
				break;

			case 1:
				nLcdLeftMargin = 0;
				TextOutXYnChar( scr_ptr+2, NULL, -6, addr_op, 4, DRAW_NORMAL );
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				addr_op += 2;
				TextOutXYnChar( scr_ptr, NULL, 46, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				break;

			case 2:
			case 3: // Line 3
			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				break;
			}
		}
		break;

	case PAGE_CALLSTACK_BIG:
		/*

		for (j = 0; j < HS_SCRN_HIGH_EX; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 1; // 2-dot chink.
				break;

			case 1:
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 4, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 13; // 11-dot width chink.
				break;

			case 2:
				// Skip 
				break;

			case 3: // Line 3
				SetASCIIFont(BIG_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_BIG_ASCII_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;

			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}
		break;

  */

		for (j = 0; j < HS_SCRN_HIGH_EX; j++)
		{
			switch (j)
			{
			case 0: // Line 0
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, 2, DRAW_NORMAL );
				TextOutXYnChar( scr_ptr+6, NULL, 48, addr_op, 10, DRAW_NORMAL ); // dot pos * 2 = physical pos, cause, word buf.
				addr_op += nASCIIFontHeight;
				addr_op += 1; // 2-dot chink.
				break;

			case 1:
				nLcdLeftMargin = 0;
				TextOutXYnChar( scr_ptr+2, NULL, -6, addr_op, 4, DRAW_NORMAL );
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				addr_op += 2;
				TextOutXYnChar( scr_ptr, NULL, 46, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				addr_op += 11; // 11-dot width chink.
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				break;

			case 2:
				// Skip 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				break;

			case 3: // Line 3
				SetASCIIFont(BIG_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_BIG_ASCII_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
				break;

			case 4:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;
			}

		}
		break;


	case PAGE_PASSWORD:

		SetASCIIFont(BIG_SIZE_ASCII_FONT);

		scr_ptr = scr_ptr + HS_SCRN_WIDE_EX*3+7; // From second line.

		//TextOutXYnChar( scr_ptr, NULL, 58, 35, 4, DRAW_NORMAL );
		TextOutXYnCharCursor( scr_ptr, NULL, 58, 35, 5, cursor_x-7, DRAW_NORMAL );

		break;

	case PAGE_AUTOANS:

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			switch (j)
			{

			case 0:
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
				SetHanFont(NORMAL_SIZE_ASCII_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;

			case 1:
			case 2:
			case 3:

				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				addr_op += nASCIIFontHeight;
				break;

			case 4:
#if (MODEL_ID!=MODEL_A8)  && !defined(GIZMO_A7M)
				SetASCIIFont(MED_SIZE_ASCII_FONT);
				SetHanFont(MED_SIZE_ASCII_FONT);
				addr_op += 3; // 3-dot width chink.
				TextOutXYnChar( scr_ptr, NULL, 48, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
#else
#ifdef GIZMO_A7M
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				if(ui.earpieceup)
					TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );
				else
					TextOutXYnChar( UI_SK_NTOP_BOOK_F, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;

#else	// MODEL_ID == MODEL_A8
// The above code is changed as follows due to support displaying auto-answer screen 
//  and menu bar on folder type phone.
				SetASCIIFont(NORMAL_SIZE_ASCII_FONT);
				SetHanFont(NORMAL_SIZE_HAN_FONT);
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

				addr_op += nASCIIFontHeight;
#endif	//GIZMO_A7M
#endif  // (MODEL_ID!=MODEL_A8) && !defined(GIZMO_A7M)
    			break;
			}

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	case PAGE_MEDIUM:
	case PAGE_CALENDAR:
	case PAGE_NORMAL_BIG_ASCII:
		break;


	default:

		SetASCIIFont(NORMAL_SIZE_ASCII_FONT); /* set font attrib */
		SetHanFont(NORMAL_SIZE_HAN_FONT); /* set font attrib */

		for (j = 0; j < HS_SCRN_HIGH_EX /* A7: 5 line */; j++)
		{
			if (cursor_y == j)
				TextOutXYnCharCursor( scr_ptr, NULL, 0, addr_op, 
										PAGE_NORMAL_CHAR_NUM, 
										cursor_x, DRAW_NORMAL );
			else
				TextOutXYnChar( scr_ptr, NULL, 0, addr_op, PAGE_NORMAL_CHAR_NUM, DRAW_NORMAL );

			addr_op += nASCIIFontHeight;

			if(j < HS_SCRN_HIGH_EX - 1 ) 
			{ 
				scr_ptr = scr_ptr + HS_SCRN_WIDE_EX;
			}
		}

		break;

	}
}
