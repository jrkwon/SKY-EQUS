/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_grph.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-12-24 jrkwon     Sub display support. Merry Christmas !! 
** 2000-12-01 girin      For better performance, many tiny functions are 
**                       converted to macro version. You can figure out what
**                       the macro version was by name easily.
** 2000-06-27 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>   /* vsprintf */
#include <stdarg.h>  /* va_list */
#include "eqs.h"


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define FEATURE_USE_LZB

#ifdef   FEATURE_USE_LZB

enum {
	BMP = 1,    //BMP_DATA
	CNT = 2,    //CONTENT_SIZE
	TRS = 3,    //TRANS_DATA
	RXBUF  = 4, //DECODE COMPRESSED IMAGE
	COMP_IMG = 5,
	DECOMP_IMG =6
};
extern BYTE* SeekWorkingArea(INT);
#ifndef WIN32
extern VOID AddressAssign(VOID);
#endif //WIN32

#define MAX_WIDTH               128
#define MAX_BPP		           16 // NKHA 2001.11.09. 4Gray (2) --> 16 bit color (16)
#define MAX_VIEW_HEIGHT         160

#endif //#ifdef   FEATURE_USE_LZB


/* the max count of graphic devices (main display and sub display) */
#define MAX_GRAPHIC_DEVICE          2

#define LOBYTE(w)   ((BYTE) (w)) 
#define HIBYTE(w)   ((BYTE) (((WORD) (w) >> 8) & 0xFF)) 

/* choose one of two */
#define sUSE_14X15_BIG_FONT     1
#define sUSE_14X16_BIG_FONT     2
#define sBIG_FONT_SIZE          sUSE_14X15_BIG_FONT

#define sGET_LOW_BYTE(dwData)       (dwData & 0x00ff)
#define sGET_HIGH_BYTE(dwData)      ((dwData >> 8) & 0x00ff)
#define sCHECK_BIT                  (0x80)

#define MAX_BIT2SHIFT_4G        6      /* 4 GRAY */
#define MAX_BIT2SHIFT_BW        7      /* BW */

#define sIS_TRANSPARENT_COLOR(BmpHeader)    (BmpHeader.abReserved[0] == 1)

/*#define BIT_PER_PIXEL_BW        1
#define BIT_PER_PIXEL_4G        2
#define BIT_PER_PIXEL_256C      8*/


/* get the graphic device of the current screen */
#define sGETGD()                GD[l_CurrentScreen]
/* get the device context of the current graphic device */
#define sGETDC()                GD[l_CurrentScreen].DC
/* get the viewport of the current device context */
#define sGETVP()                GD[l_CurrentScreen].DC.ViewPort
/* get sliding window state */
#ifdef FEATURE_EQS_SLIDING_WINDOW
#define sGETSW()                GD[l_CurrentScreen].SlidingWindow
#endif /* FEATURE_EQS_SLIDING_WINDOW */

//////////////////////

//COORD EQS_API LPx2DPx(COORD x)
#define LPX_2_DPX(xX) (COORD)((xX) + sGETVP().Rect.xLeft + sGETVP().Origin.x)

//COLOR EQS_API GetForeColor(VOID)
#define GET_FORE_COLOR() (sGETDC().ForeColor)

//COLOR EQS_API GetBackColor(VOID)
#define GET_BACK_COLOR() (sGETDC().BackColor)

//COLOR EQS_API GetTextColor(VOID)
#define GET_TEXT_COLOR() (sGETDC().TextColor)

//COLOR EQS_API GetTextBackColor(VOID)
#define GET_TEXT_BACK_COLOR() (sGETDC().TextBackColor)

//VOID EQS_API SetForeColor(COLOR ForeColor)
#define SET_FORE_COLOR(InForeColor) (sGETDC().ForeColor = (InForeColor))

//VOID EQS_API SetBackColor(COLOR BackColor)
#define SET_BACK_COLOR(InBackColor) (sGETDC().BackColor = (InBackColor))

//VOID EQS_API SetTextColor(COLOR TextColor)
#define SET_TEXT_COLOR(InTextColor) (sGETDC().TextColor = (InTextColor))

//VOID EQS_API SetTextBackColor(COLOR TextColor)
#define SET_TEXT_BACK_COLOR(InTextColor) (sGETDC().TextBackColor = (InTextBackColor))

//VOID EQS_API SetDrawMode(DRAW_MODE DrawMode)
#define SET_DRAW_MODE(InDrawMode) (sGETDC().DrawMode = InDrawMode)

//DRAW_MODE EQS_API GetDrawMode(VOID)
#define GET_DRAW_MODE() (sGETDC().DrawMode)

#ifdef FEATURE_EQS_SLIDING_WINDOW
//BOOL EQS_API GetSlidingWindowState(VOID)
#define GET_SLIDING_WINDOW_STATE() (sGETSW().fEnable)

//COORD EQS_API GetSlidingWindowOffsetX(VOID)
#define GET_SLIDING_WINDOW_OFFSET_X() (sGETSW().Offset.x)

//COORD EQS_API GetSlidingWindowOffsetY(VOID)
#define GET_SLIDING_WINDOW_OFFSET_Y() (sGETSW().Offset.y)
#endif

/* DEV_GET_MAX_WIDTH returns the screen buffer width */
//COORD EQS_API DevGetMaxWidth(VOID)
#define DEV_GET_MAX_WIDTH()         (sGETGD().Screen.Info.Size.xWidth)
#define DEV_GET_MAX_BUFFER_WIDTH()  (sGETGD().Screen.Info.BufferSize.xWidth)

//COORD EQS_API DevGetMaxHeight(VOID)
#define DEV_GET_MAX_HEIGHT() (sGETGD().Screen.Info.Size.yHeight)

/*
** call DEV_GET_MAX_X() to get the max width of a device 
** but, the width of memory buffer for the real LCD device is larger 
** than the real screen width by 8 bit 
*/
//COORD EQS_API DevGetMaxX(VOID)
#define DEV_GET_MAX_X() (DEV_GET_MAX_WIDTH() - 1) /* 128 - 120 = 8 */
//#define REAL_GET_MAX_X()    (DEV_GET_MAX_X() - 8)

//COORD EQS_API DevGetMaxY(VOID)
#define DEV_GET_MAX_Y() (DEV_GET_MAX_HEIGHT() - 1)

//COORD EQS_API GetMaxWidth(VOID)
#define GET_MAX_WIDTH() (sGETVP().Rect.xRight - sGETVP().Rect.xLeft + 1)

//COORD EQS_API GetMaxHeight(VOID)
#define GET_MAX_HEIGHT() (sGETVP().Rect.yBottom - sGETVP().Rect.yTop + 1)

//COORD EQS_API GetMaxX(VOID)
#define GET_MAX_X() (sGETVP().Rect.xRight - sGETVP().Rect.xLeft)

//COORD EQS_API GetMaxY(VOID)
#define GET_MAX_Y() (sGETVP().Rect.yBottom - sGETVP().Rect.yTop)

//INT8 EQS_API GetScreenBitsPerPixel(VOID)
#define GET_SCREEN_BITS_PER_PIXEL() ((INT8)sGETGD().Screen.Info.ColorSpace)

//SIZE* EQS_API GetScreenSize(VOID)
#define GET_SCREEN_SIZE() (&(sGETGD().Screen.Info.Size))

//POINT* EQS_API DevGetCurrentPosition(VOID)
#define DEV_GET_CURRENT_POSITION() (&sGETDC().CurrentPosition)

//COORD EQS_API DevGetCurrentPositionX(VOID)
#define DEV_GET_CURRENT_POSITION_X() (sGETDC().CurrentPosition.x)

//COORD EQS_API DevGetCurrentPositionY(VOID)
#define DEV_GET_CURRENT_POSITION_Y() (sGETDC().CurrentPosition.y)

//BYTE* EQS_API GetScreenBuffer(VOID)
#define GET_SCREEN_BUFFER() (sGETGD().Screen.pbBuffer)

//FONT_TYPE EQS_API GetFontType(VOID)
#define GET_FONT_TYPE() (sGETDC().FontType)

//BOOL EQS_API GetTextWrap(VOID)
#define GET_TEXT_WRAP() (sGETDC().fTextWrap)

//////////////////////

#define INDEX_ON_SCREEN(x, y, xWidth) ((x) + ((y)*(xWidth)))

#define BYTE_POS(nIndexOfBuffer, nBitsPerPixel) (((nIndexOfBuffer)*(nBitsPerPixel)) >> 3 )

#define BYTE_POS(nIndexOfBuffer, nBitsPerPixel) (((nIndexOfBuffer)*(nBitsPerPixel)) >> 3 )

#define BYTE_POS_BW(nIndexOfBuffer)   ((nIndexOfBuffer) >> 3 )
#define BYTE_POS_4G(nIndexOfBuffer)   ((nIndexOfBuffer) >> 2 )
#define BYTE_POS_256C(nIndexOfBuffer) (nIndexOfBuffer)
#define BYTE_POS_64KC(nIndexOfBuffer)  ((nIndexOfBuffer) << 1)

#define BIT_POS_BW(nIndexOfBuffer)    ((nIndexOfBuffer) % 8)
#define BIT_POS_4G(nIndexOfBuffer)    (((nIndexOfBuffer) << 1) % 8)
#define BIT_POS_256C(nIndexOfBuffer)  (0)
#define BIT_POS_64KC(nIndexOfBuffer)  (0)

#define sGET_BYTE_IN_BUFFER_BW(pbBuffer, nIndexOfBuffer)   (*((pbBuffer) + (BYTE_POS_BW((nIndexOfBuffer)))))
#define sGET_BYTE_IN_BUFFER_4G(pbBuffer, nIndexOfBuffer)   (*((pbBuffer) + (BYTE_POS_4G((nIndexOfBuffer)))))
#define sGET_BYTE_IN_BUFFER_256C(pbBuffer, nIndexOfBuffer) (*((pbBuffer) + (BYTE_POS_256C((nIndexOfBuffer)))))
#define sGET_BYTE_IN_BUFFER_64KC(pbBuffer, nIndexOfBuffer) (*((pbBuffer) + (BYTE_POS_64KC((nIndexOfBuffer)))))

#define sPIXEL_BW(pbBuffer, nIndexOfBuffer)   (sGET_BYTE_IN_BUFFER_BW((pbBuffer), (nIndexOfBuffer)) & (0x80 >> (BIT_POS_BW((nIndexOfBuffer))))) ? 1 : 0
#define sPIXEL_4G(pbBuffer, nIndexOfBuffer)   ((sGET_BYTE_IN_BUFFER_4G((pbBuffer), (nIndexOfBuffer))) >> (MAX_BIT2SHIFT_4G - (BIT_POS_4G(nIndexOfBuffer))))
#define sPIXEL_256C(pbBuffer, nIndexOfBuffer) (sGET_BYTE_IN_BUFFER_256C((pbBuffer), (nIndexOfBuffer)))
#define sPIXEL_64KC(pbBuffer, nIndexOfBuffer) (sGET_BYTE_IN_BUFFER_64KC((pbBuffer), (nIndexOfBuffer)))

#define GET_PIXEL_BW(x, y, xWidth, pbBuffer)   (sPIXEL_BW((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_PIXEL_4G(x, y, xWidth, pbBuffer)   (sPIXEL_4G((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_PIXEL_256C(x, y, xWidth, pbBuffer) (sPIXEL_256C((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_PIXEL_64KC(x, y, xWidth, pbBuffer) (sPIXEL_64KC((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))

#define GET_COLOR_BYTE_BW(x, y, xWidth, pbBuffer)    (sGET_BYTE_IN_BUFFER_BW((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_COLOR_BYTE_4G(x, y, xWidth, pbBuffer)    (sGET_BYTE_IN_BUFFER_4G((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_COLOR_BYTE_256C(x, y, xWidth, pbBuffer)  (sGET_BYTE_IN_BUFFER_256C((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))
#define GET_COLOR_BYTE_64KC(x, y, xWidth, pbBuffer)  (sGET_BYTE_IN_BUFFER_64KC((pbBuffer), (INDEX_ON_SCREEN((x),(y),(xWidth)))))

#define SET_COLOR_BYTE_BW(x, y, xWidth, pbBuffer, bColor)   *(pbBuffer + BYTE_POS_BW(INDEX_ON_SCREEN((x),(y),(xWidth)))) = bColor
#define SET_COLOR_BYTE_4G(x, y, xWidth, pbBuffer, bColor)   *(pbBuffer + BYTE_POS_4G(INDEX_ON_SCREEN((x),(y),(xWidth)))) = bColor
#define SET_COLOR_BYTE_256C(x, y, xWidth, pbBuffer, bColor) *(pbBuffer + BYTE_POS_256C(INDEX_ON_SCREEN((x),(y),(xWidth)))) = bColor
#define SET_COLOR_BYTE_64KC(x, y, xWidth, pbBuffer, bColor) *(pbBuffer + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(y),(xWidth)))) = bColor

#define sGET_BYTE_IN_BUFFER(pbBuffer, nIndexOfBuffer, nBitsPerPixel) \
                             (*((pbBuffer) + (BYTE_POS((nIndexOfBuffer), (nBitsPerPixel)))))
#define GET_COLOR_BYTE(x, y, xWidth, nBitsPerPixel, pbBuffer) \
                         (sGET_BYTE_IN_BUFFER((pbBuffer), \
                         (INDEX_ON_SCREEN((x),(y),(xWidth))), \
                         (nBitsPerPixel)))
#define SET_COLOR_BYTE(x, y, xWidth, nBitsPerPixel, pbBuffer, bColor) \
                         *(pbBuffer + BYTE_POS(INDEX_ON_SCREEN((x),(y),(xWidth)), nBitsPerPixel)) \
                         = bColor

/* using for local and macro extension. 
** to reduce time to push to or pop from stack
*/
ROM LOCAL BYTE m_abMaskBW[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
ROM LOCAL BYTE m_abMask4G[] = { 0xC0, 0x30, 0x0C, 0x03 };
LOCAL BYTE m_bColor;
LOCAL INT16 m_nIndexOfBuffer;
LOCAL INT16 m_nBytePos;
LOCAL INT8 m_nBitPos;

#define SET_PIXEL_BW(x, y, xWidth, pbBuffer, Color) \
    m_bColor = GET_COLOR_BYTE_BW((x),(y),(xWidth),(pbBuffer)); \
    m_nIndexOfBuffer = INDEX_ON_SCREEN((x),(y),(xWidth)); \
    m_nBytePos = BYTE_POS_BW(m_nIndexOfBuffer); \
    m_nBitPos = BIT_POS_BW(m_nIndexOfBuffer); \
    if(GetDrawMode() == DM_COPY) \
        m_bColor &= ~m_abMaskBW[m_nBitPos]; \
    m_bColor |= ((Color) << (MAX_BIT2SHIFT_BW-m_nBitPos));\
    *((pbBuffer) + m_nBytePos) = m_bColor;

#define SET_PIXEL_4G(x, y, xWidth, pbBuffer, Color) \
    m_bColor = GET_COLOR_BYTE_4G((x),(y),(xWidth),(pbBuffer)); \
    m_nIndexOfBuffer = INDEX_ON_SCREEN((x),(y),(xWidth)); \
    m_nBytePos = BYTE_POS_4G(m_nIndexOfBuffer); \
    m_nBitPos = BIT_POS_4G(m_nIndexOfBuffer); \
    if(GetDrawMode() == DM_COPY) \
        m_bColor &= ~m_abMask4G[m_nBitPos>>1]; \
    m_bColor |= ((Color) << (MAX_BIT2SHIFT_4G-m_nBitPos));\
    *(pbBuffer + m_nBytePos) = m_bColor;

#define SET_PIXEL_256C(x, y, xWidth, pbBuffer, bColor) \
    if(GET_DRAW_MODE() == DM_COPY) \
        *(pbBuffer + BYTE_POS_256C(INDEX_ON_SCREEN((x),(y),(xWidth)))) \
        = bColor; \
    else if(GET_DRAW_MODE() == DM_OR) \
        *(pbBuffer + BYTE_POS_256C(INDEX_ON_SCREEN((x),(y),(xWidth)))) \
        |= bColor 

#define SET_PIXEL_64KC(x, y, xWidth, pbBuffer, wColor) \
    if(GET_DRAW_MODE() == DM_COPY) \
        *(pbBuffer + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(y),(xWidth)))) = LOBYTE(wColor), \
        *(pbBuffer + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(y),(xWidth))) + 1) = HIBYTE(wColor); \
    else if(GET_DRAW_MODE() == DM_OR) \
        *(pbBuffer + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(y),(xWidth)))) |= LOBYTE(wColor), \
        *(pbBuffer + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(y),(xWidth))) + 1) |= HIBYTE(wColor); \

#define SET_BIT(bData, bBit, nBitPos)  \
		  ( (bBit) ? ((bData) |= (0x80 >> (nBitPos))) : \
          ((bData) &= (~(0x80>>(nBitPos)))) )
#define GET_BIT(bData, nBitPos)   \
		  ( ((bData) & (0x80 >> (nBitPos))) ? 1 : 0 )


//LOCAL INT8 m_i;

/* 4 gray bitmap image and font only */
#define SET_COLOR_BIT_1ST_BYTE(bBuffer, bData, nBitPos) \
                                if(GetDrawMode() == DM_COPY) \
                                    (bBuffer) &= ~((0xFF) >> (nBitPos)); \
                                (bBuffer) |= ((bData) >> (nBitPos)); 
                                /*if(GetDrawMode() == DM_XOR) \
                                    (bBuffer) ^= ~((bData) << (8-(nBitPos))); */
#define SET_COLOR_BIT_2ND_BYTE(bBuffer, bData, nBitPos)  \
                                if(GetDrawMode() == DM_COPY) \
                                    (bBuffer) &= ~((0xFF) << (8-(nBitPos))); \
                                (bBuffer) |= ((bData) << (8-(nBitPos))); 
                                /*if(GetDrawMode() == DM_XOR) \
                                    (bBuffer) ^= ~((bData) << (8-(nBitPos))); */

#define SET_PIXEL(x, y, xWidth, pbBuffer, Color) \
        if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC) \
        { \
            SET_PIXEL_64KC((x), (y), (xWidth), (pbBuffer), (Color)); \
        } \
        else if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C) \
        { \
            SET_PIXEL_256C((x), (y), (xWidth), (pbBuffer), (Color)); \
        } \
        else if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_4G) \
        { \
            SET_PIXEL_4G((x), (y), (xWidth), (pbBuffer), (Color)); \
        } \
        else if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW) \
        { \
            SET_PIXEL_BW((x), (y), (xWidth), (pbBuffer), (Color)); \
        } \
        else \
        { \
            ASSERT(0); \
        }

#if !defined(FEATURE_EQS_SLIDING_WINDOW_X) && defined(FEATURE_EQS_SLIDING_WINDOW_Y) && defined(FEATURE_EQS_CLIPPING)
LOCAL COORD m_yTemp;
#define DEV_SET_PIXEL(xIn, yIn, InColor)\
{ \
    if(GET_SLIDING_WINDOW_STATE()) \
        m_yTemp = (COORD)((yIn) - GET_SLIDING_WINDOW_OFFSET_Y()); \
    if(!(xIn >= 0 && xIn <= DEV_GET_MAX_X()) || !(m_yTemp >= 0 && m_yTemp <= DEV_GET_MAX_Y())) \
        return; \
    SET_PIXEL((xIn), m_yTemp, DEV_GET_MAX_BUFFER_WIDTH(), GET_SCREEN_BUFFER(), (InColor)); \
} 
//    SET_PIXEL((xIn), m_yTemp, GET_SCREEN_SIZE()->xWidth, GET_SCREEN_BUFFER(), (InColor));
#else
#error Not supported matrix of features for DEV_SET_PIXEL
#endif



/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagGRAPHIC_DEVICE        GRAPHIC_DEVICE;
typedef struct tagBITMAP_FONT           BITMAP_FONT;
typedef struct tagBITMAP_FONT_SET       BITMAP_FONT_SET;
typedef struct tagBYTE_COLOR            BYTE_COLOR;
#ifdef FEATURE_EQS_SLIDING_WINDOW
typedef struct tagSLIDING_WINDOW        SLIDING_WINDOW;

/* The position of the sliding window on the canvas */
struct tagSLIDING_WINDOW {
    POINT Offset;
    BOOL  fEnable;
};
#endif /* FEATURE_EQS_SLIDING_WINDOW */

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagGRAPHIC_DEVICE {
    SCREEN         Screen;              /* screen device info */
    DEVICE_CONTEXT DC;                  /* device context */
#ifdef FEATURE_EQS_SLIDING_WINDOW
    SLIDING_WINDOW SlidingWindow;       /* sliding window */
#endif /* FEATURE_EQS_SLIDING_WINDOW */

    VOID EQS_API  (*PutBitmap)(COORD x, COORD y, CONST BITMAP* pBitmap);
};

struct tagBITMAP_FONT {
    SIZE Size;
    INT8 nBitsPerPixel;
    ROM BYTE* pbFontBuffer;
};

/* bitmap font binary data buffer currently be using bitmap font */
struct tagBITMAP_FONT_SET {
    /* CES : ASCII */
    ROM BITMAP_FONT* pASCIIBitmapFont;
    /* CES : KSC 5601 */
    ROM BITMAP_FONT* pHangulBitmapFont;
    ROM BITMAP_FONT* pSpecialBitmapFont;
    /* CES : SKY only */
    ROM BITMAP_FONT* pSkySymbolBitmapFont;
#ifdef FEATURE_EQS_HANJA
    /* CES : KSC5601 Hanja */
    ROM BITMAP_FONT* pHanjaBitmapFont;
#endif /* FEATURE_EQS_HANJA */
};

/* the color information in the screen buffer or image buffer */
struct tagBYTE_COLOR {
    BYTE bColor;
    INT8 nMaxBit;
    INT8 nBitPos;
    INT16 nBytePos;
};

//#ifdef FEATURE_EQS_SLIDING_WINDOW
///* The position of the sliding window on the canvas */
//struct tagSLIDING_WINDOW {
//    POINT Offset;
//    BOOL  fEnable;
//};
//#endif /* FEATURE_EQS_SLIDING_WINDOW */

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
VOID EQS_API (*PutBitmap)(COORD x, COORD y, CONST BITMAP* pBitmap);
VOID EQS_API (*PutBitmapRect)(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT

/* KSSM code(compound type hangul code) table to KS5601 */
ROM WORD awKSSMCodeTable[] = 
{
    0x8861,0x8862,0x8865,0x8868,0x8869,0x886A,0x886B,0x8871,0x8873,0x8874,
    0x8875,0x8876,0x8877,0x8878,0x8879,0x887B,0x887C,0x887D,0x8881,0x8882,
    0x8885,0x8889,0x8891,0x8893,0x8895,0x8896,0x8897,0x88A1,0x88A2,0x88A5,
    0x88A9,0x88B5,0x88B7,0x88C1,0x88C5,0x88C9,0x88E1,0x88E2,0x88E5,0x88E8,
    0x88E9,0x88EB,0x88F1,0x88F3,0x88F5,0x88F6,0x88F7,0x88F8,0x88FB,0x88FC,
    0x88FD,0x8941,0x8945,0x8949,0x8951,0x8953,0x8955,0x8956,0x8957,0x8961,
    0x8962,0x8963,0x8965,0x8968,0x8969,0x8971,0x8973,0x8975,0x8976,0x8977,
    0x897B,0x8981,0x8985,0x8989,0x8993,0x8995,0x89A1,0x89A2,0x89A5,0x89A8,
    0x89A9,0x89AB,0x89AD,0x89B0,0x89B1,0x89B3,0x89B5,0x89B7,0x89B8,0x89C1,
    0x89C2,0x89C5,0x89C9,0x89CB,0x89D1,0x89D3,0x89D5,0x89D7,0x89E1,0x89E5,
    0x89E9,0x89F3,0x89F6,0x89F7,0x8A41,0x8A42,0x8A45,0x8A49,0x8A51,0x8A53,
    0x8A55,0x8A57,0x8A61,0x8A65,0x8A69,0x8A73,0x8A75,0x8A81,0x8A82,0x8A85,
    0x8A88,0x8A89,0x8A8A,0x8A8B,0x8A90,0x8A91,0x8A93,0x8A95,0x8A97,0x8A98,
    0x8AA1,0x8AA2,0x8AA5,0x8AA9,0x8AB6,0x8AB7,0x8AC1,0x8AD5,0x8AE1,0x8AE2,
    0x8AE5,0x8AE9,0x8AF1,0x8AF3,0x8AF5,0x8B41,0x8B45,0x8B49,0x8B61,0x8B62,
    0x8B65,0x8B68,0x8B69,0x8B6A,0x8B71,0x8B73,0x8B75,0x8B77,0x8B81,0x8BA1,
    0x8BA2,0x8BA5,0x8BA8,0x8BA9,0x8BAB,0x8BB1,0x8BB3,0x8BB5,0x8BB7,0x8BB8,
    0x8BBC,0x8C61,0x8C62,0x8C63,0x8C65,0x8C69,0x8C6B,0x8C71,0x8C73,0x8C75,
    0x8C76,0x8C77,0x8C7B,0x8C81,0x8C82,0x8C85,0x8C89,0x8C91,0x8C93,0x8C95,
    0x8C96,0x8C97,0x8CA1,0x8CA2,0x8CA9,0x8CE1,0x8CE2,0x8CE3,0x8CE5,0x8CE9,
    0x8CF1,0x8CF3,0x8CF5,0x8CF6,0x8CF7,0x8D41,0x8D42,0x8D45,0x8D51,0x8D55,
    0x8D57,0x8D61,0x8D65,0x8D69,0x8D75,0x8D76,0x8D7B,0x8D81,0x8DA1,0x8DA2,
    0x8DA5,0x8DA7,0x8DA9,0x8DB1,0x8DB3,0x8DB5,0x8DB7,0x8DB8,0x8DB9,0x8DC1,
    0x8DC2,0x8DC9,0x8DD6,0x8DD7,0x8DE1,0x8DE2,0x8DF7,0x8E41,0x8E45,0x8E49,
    0x8E51,0x8E53,0x8E57,0x8E61,0x8E81,0x8E82,0x8E85,0x8E89,0x8E90,0x8E91,
    0x8E93,0x8E95,0x8E97,0x8E98,0x8EA1,0x8EA9,0x8EB6,0x8EB7,0x8EC1,0x8EC2,
    0x8EC5,0x8EC9,0x8ED1,0x8ED3,0x8ED6,0x8EE1,0x8EE5,0x8EE9,0x8EF1,0x8EF3,
    0x8F41,0x8F61,0x8F62,0x8F65,0x8F67,0x8F69,0x8F6B,0x8F70,0x8F71,0x8F73,
    0x8F75,0x8F77,0x8F7B,0x8FA1,0x8FA2,0x8FA5,0x8FA9,0x8FB1,0x8FB3,0x8FB5,
    0x8FB7,0x9061,0x9062,0x9063,0x9065,0x9068,0x9069,0x906A,0x906B,0x9071,
    0x9073,0x9075,0x9076,0x9077,0x9078,0x9079,0x907B,0x907D,0x9081,0x9082,
    0x9085,0x9089,0x9091,0x9093,0x9095,0x9096,0x9097,0x90A1,0x90A2,0x90A5,
    0x90A9,0x90B1,0x90B7,0x90E1,0x90E2,0x90E4,0x90E5,0x90E9,0x90EB,0x90EC,
    0x90F1,0x90F3,0x90F5,0x90F6,0x90F7,0x90FD,0x9141,0x9142,0x9145,0x9149,
    0x9151,0x9153,0x9155,0x9156,0x9157,0x9161,0x9162,0x9165,0x9169,0x9171,
    0x9173,0x9176,0x9177,0x917A,0x9181,0x9185,0x91A1,0x91A2,0x91A5,0x91A9,
    0x91AB,0x91B1,0x91B3,0x91B5,0x91B7,0x91BC,0x91BD,0x91C1,0x91C5,0x91C9,
    0x91D6,0x9241,0x9245,0x9249,0x9251,0x9253,0x9255,0x9261,0x9262,0x9265,
    0x9269,0x9273,0x9275,0x9277,0x9281,0x9282,0x9285,0x9288,0x9289,0x9291,
    0x9293,0x9295,0x9297,0x92A1,0x92B6,0x92C1,0x92E1,0x92E5,0x92E9,0x92F1,
    0x92F3,0x9341,0x9342,0x9349,0x9351,0x9353,0x9357,0x9361,0x9362,0x9365,
    0x9369,0x936A,0x936B,0x9371,0x9373,0x9375,0x9377,0x9378,0x937C,0x9381,
    0x9385,0x9389,0x93A1,0x93A2,0x93A5,0x93A9,0x93AB,0x93B1,0x93B3,0x93B5,
    0x93B7,0x93BC,0x9461,0x9462,0x9463,0x9465,0x9468,0x9469,0x946A,0x946B,
    0x946C,0x9470,0x9471,0x9473,0x9475,0x9476,0x9477,0x9478,0x9479,0x947D,
    0x9481,0x9482,0x9485,0x9489,0x9491,0x9493,0x9495,0x9496,0x9497,0x94A1,
    0x94E1,0x94E2,0x94E3,0x94E5,0x94E8,0x94E9,0x94EB,0x94EC,0x94F1,0x94F3,
    0x94F5,0x94F7,0x94F9,0x94FC,0x9541,0x9542,0x9545,0x9549,0x9551,0x9553,
    0x9555,0x9556,0x9557,0x9561,0x9565,0x9569,0x9576,0x9577,0x9581,0x9585,
    0x95A1,0x95A2,0x95A5,0x95A8,0x95A9,0x95AB,0x95AD,0x95B1,0x95B3,0x95B5,
    0x95B7,0x95B9,0x95BB,0x95C1,0x95C5,0x95C9,0x95E1,0x95F6,0x9641,0x9645,
    0x9649,0x9651,0x9653,0x9655,0x9661,0x9681,0x9682,0x9685,0x9689,0x9691,
    0x9693,0x9695,0x9697,0x96A1,0x96B6,0x96C1,0x96D7,0x96E1,0x96E5,0x96E9,
    0x96F3,0x96F5,0x96F7,0x9741,0x9745,0x9749,0x9751,0x9757,0x9761,0x9762,
    0x9765,0x9768,0x9769,0x976B,0x9771,0x9773,0x9775,0x9777,0x9781,0x97A1,
    0x97A2,0x97A5,0x97A8,0x97A9,0x97B1,0x97B3,0x97B5,0x97B6,0x97B7,0x97B8,
    0x9861,0x9862,0x9865,0x9869,0x9871,0x9873,0x9875,0x9876,0x9877,0x987D,
    0x9881,0x9882,0x9885,0x9889,0x9891,0x9893,0x9895,0x9896,0x9897,0x98E1,
    0x98E2,0x98E5,0x98E9,0x98EB,0x98EC,0x98F1,0x98F3,0x98F5,0x98F6,0x98F7,
    0x98FD,0x9941,0x9942,0x9945,0x9949,0x9951,0x9953,0x9955,0x9956,0x9957,
    0x9961,0x9976,0x99A1,0x99A2,0x99A5,0x99A9,0x99B7,0x99C1,0x99C9,0x99E1,
    0x9A41,0x9A45,0x9A81,0x9A82,0x9A85,0x9A89,0x9A90,0x9A91,0x9A97,0x9AC1,
    0x9AE1,0x9AE5,0x9AE9,0x9AF1,0x9AF3,0x9AF7,0x9B61,0x9B62,0x9B65,0x9B68,
    0x9B69,0x9B71,0x9B73,0x9B75,0x9B81,0x9B85,0x9B89,0x9B91,0x9B93,0x9BA1,
    0x9BA5,0x9BA9,0x9BB1,0x9BB3,0x9BB5,0x9BB7,0x9C61,0x9C62,0x9C65,0x9C69,
    0x9C71,0x9C73,0x9C75,0x9C76,0x9C77,0x9C78,0x9C7C,0x9C7D,0x9C81,0x9C82,
    0x9C85,0x9C89,0x9C91,0x9C93,0x9C95,0x9C96,0x9C97,0x9CA1,0x9CA2,0x9CA5,
    0x9CB5,0x9CB7,0x9CE1,0x9CE2,0x9CE5,0x9CE9,0x9CF1,0x9CF3,0x9CF5,0x9CF6,
    0x9CF7,0x9CFD,0x9D41,0x9D42,0x9D45,0x9D49,0x9D51,0x9D53,0x9D55,0x9D57,
    0x9D61,0x9D62,0x9D65,0x9D69,0x9D71,0x9D73,0x9D75,0x9D76,0x9D77,0x9D81,
    0x9D85,0x9D93,0x9D95,0x9DA1,0x9DA2,0x9DA5,0x9DA9,0x9DB1,0x9DB3,0x9DB5,
    0x9DB7,0x9DC1,0x9DC5,0x9DD7,0x9DF6,0x9E41,0x9E45,0x9E49,0x9E51,0x9E53,
    0x9E55,0x9E57,0x9E61,0x9E65,0x9E69,0x9E73,0x9E75,0x9E77,0x9E81,0x9E82,
    0x9E85,0x9E89,0x9E91,0x9E93,0x9E95,0x9E97,0x9EA1,0x9EB6,0x9EC1,0x9EE1,
    0x9EE2,0x9EE5,0x9EE9,0x9EF1,0x9EF5,0x9EF7,0x9F41,0x9F42,0x9F45,0x9F49,
    0x9F51,0x9F53,0x9F55,0x9F57,0x9F61,0x9F62,0x9F65,0x9F69,0x9F71,0x9F73,
    0x9F75,0x9F77,0x9F78,0x9F7B,0x9F7C,0x9FA1,0x9FA2,0x9FA5,0x9FA9,0x9FB1,
    0x9FB3,0x9FB5,0x9FB7,0xA061,0xA062,0xA065,0xA067,0xA068,0xA069,0xA06A,
    0xA06B,0xA071,0xA073,0xA075,0xA077,0xA078,0xA07B,0xA07D,0xA081,0xA082,
    0xA085,0xA089,0xA091,0xA093,0xA095,0xA096,0xA097,0xA098,0xA0A1,0xA0A2,
    0xA0A9,0xA0B7,0xA0E1,0xA0E2,0xA0E5,0xA0E9,0xA0EB,0xA0F1,0xA0F3,0xA0F5,
    0xA0F7,0xA0F8,0xA0FD,0xA141,0xA142,0xA145,0xA149,0xA151,0xA153,0xA155,
    0xA156,0xA157,0xA161,0xA162,0xA165,0xA169,0xA175,0xA176,0xA177,0xA179,
    0xA181,0xA1A1,0xA1A2,0xA1A4,0xA1A5,0xA1A9,0xA1AB,0xA1B1,0xA1B3,0xA1B5,
    0xA1B7,0xA1C1,0xA1C5,0xA1D6,0xA1D7,0xA241,0xA245,0xA249,0xA253,0xA255,
    0xA257,0xA261,0xA265,0xA269,0xA273,0xA275,0xA281,0xA282,0xA283,0xA285,
    0xA288,0xA289,0xA28A,0xA28B,0xA291,0xA293,0xA295,0xA297,0xA29B,0xA29D,
    0xA2A1,0xA2A5,0xA2A9,0xA2B3,0xA2B5,0xA2C1,0xA2E1,0xA2E5,0xA2E9,0xA341,
    0xA345,0xA349,0xA351,0xA355,0xA361,0xA365,0xA369,0xA371,0xA375,0xA3A1,
    0xA3A2,0xA3A5,0xA3A8,0xA3A9,0xA3AB,0xA3B1,0xA3B3,0xA3B5,0xA3B6,0xA3B7,
    0xA3B9,0xA3BB,0xA461,0xA462,0xA463,0xA464,0xA465,0xA468,0xA469,0xA46A,
    0xA46B,0xA46C,0xA471,0xA473,0xA475,0xA477,0xA47B,0xA481,0xA482,0xA485,
    0xA489,0xA491,0xA493,0xA495,0xA496,0xA497,0xA49B,0xA4A1,0xA4A2,0xA4A5,
    0xA4B3,0xA4E1,0xA4E2,0xA4E5,0xA4E8,0xA4E9,0xA4EB,0xA4F1,0xA4F3,0xA4F5,
    0xA4F7,0xA4F8,0xA541,0xA542,0xA545,0xA548,0xA549,0xA551,0xA553,0xA555,
    0xA556,0xA557,0xA561,0xA562,0xA565,0xA569,0xA573,0xA575,0xA576,0xA577,
    0xA57B,0xA581,0xA585,0xA5A1,0xA5A2,0xA5A3,0xA5A5,0xA5A9,0xA5B1,0xA5B3,
    0xA5B5,0xA5B7,0xA5C1,0xA5C5,0xA5D6,0xA5E1,0xA5F6,0xA641,0xA642,0xA645,
    0xA649,0xA651,0xA653,0xA661,0xA665,0xA681,0xA682,0xA685,0xA688,0xA689,
    0xA68A,0xA68B,0xA691,0xA693,0xA695,0xA697,0xA69B,0xA69C,0xA6A1,0xA6A9,
    0xA6B6,0xA6C1,0xA6E1,0xA6E2,0xA6E5,0xA6E9,0xA6F7,0xA741,0xA745,0xA749,
    0xA751,0xA755,0xA757,0xA761,0xA762,0xA765,0xA769,0xA771,0xA773,0xA775,
    0xA7A1,0xA7A2,0xA7A5,0xA7A9,0xA7AB,0xA7B1,0xA7B3,0xA7B5,0xA7B7,0xA7B8,
    0xA7B9,0xA861,0xA862,0xA865,0xA869,0xA86B,0xA871,0xA873,0xA875,0xA876,
    0xA877,0xA87D,0xA881,0xA882,0xA885,0xA889,0xA891,0xA893,0xA895,0xA896,
    0xA897,0xA8A1,0xA8A2,0xA8B1,0xA8E1,0xA8E2,0xA8E5,0xA8E8,0xA8E9,0xA8F1,
    0xA8F5,0xA8F6,0xA8F7,0xA941,0xA957,0xA961,0xA962,0xA971,0xA973,0xA975,
    0xA976,0xA977,0xA9A1,0xA9A2,0xA9A5,0xA9A9,0xA9B1,0xA9B3,0xA9B7,0xAA41,
    0xAA61,0xAA77,0xAA81,0xAA82,0xAA85,0xAA89,0xAA91,0xAA95,0xAA97,0xAB41,
    0xAB57,0xAB61,0xAB65,0xAB69,0xAB71,0xAB73,0xABA1,0xABA2,0xABA5,0xABA9,
    0xABB1,0xABB3,0xABB5,0xABB7,0xAC61,0xAC62,0xAC64,0xAC65,0xAC68,0xAC69,
    0xAC6A,0xAC6B,0xAC71,0xAC73,0xAC75,0xAC76,0xAC77,0xAC7B,0xAC81,0xAC82,
    0xAC85,0xAC89,0xAC91,0xAC93,0xAC95,0xAC96,0xAC97,0xACA1,0xACA2,0xACA5,
    0xACA9,0xACB1,0xACB3,0xACB5,0xACB7,0xACC1,0xACC5,0xACC9,0xACD1,0xACD7,
    0xACE1,0xACE2,0xACE3,0xACE4,0xACE5,0xACE8,0xACE9,0xACEB,0xACEC,0xACF1,
    0xACF3,0xACF5,0xACF6,0xACF7,0xACFC,0xAD41,0xAD42,0xAD45,0xAD49,0xAD51,
    0xAD53,0xAD55,0xAD56,0xAD57,0xAD61,0xAD62,0xAD65,0xAD69,0xAD71,0xAD73,
    0xAD75,0xAD76,0xAD77,0xAD81,0xAD85,0xAD89,0xAD97,0xADA1,0xADA2,0xADA3,
    0xADA5,0xADA9,0xADAB,0xADB1,0xADB3,0xADB5,0xADB7,0xADBB,0xADC1,0xADC2,
    0xADC5,0xADC9,0xADD7,0xADE1,0xADE5,0xADE9,0xADF1,0xADF5,0xADF6,0xAE41,
    0xAE45,0xAE49,0xAE51,0xAE53,0xAE55,0xAE61,0xAE62,0xAE65,0xAE69,0xAE71,
    0xAE73,0xAE75,0xAE77,0xAE81,0xAE82,0xAE85,0xAE88,0xAE89,0xAE91,0xAE93,
    0xAE95,0xAE97,0xAE99,0xAE9B,0xAE9C,0xAEA1,0xAEB6,0xAEC1,0xAEC2,0xAEC5,
    0xAEC9,0xAED1,0xAED7,0xAEE1,0xAEE2,0xAEE5,0xAEE9,0xAEF1,0xAEF3,0xAEF5,
    0xAEF7,0xAF41,0xAF42,0xAF49,0xAF51,0xAF55,0xAF57,0xAF61,0xAF62,0xAF65,
    0xAF69,0xAF6A,0xAF71,0xAF73,0xAF75,0xAF77,0xAFA1,0xAFA2,0xAFA5,0xAFA8,
    0xAFA9,0xAFB0,0xAFB1,0xAFB3,0xAFB5,0xAFB7,0xAFBC,0xB061,0xB062,0xB064,
    0xB065,0xB069,0xB071,0xB073,0xB076,0xB077,0xB07D,0xB081,0xB082,0xB085,
    0xB089,0xB091,0xB093,0xB096,0xB097,0xB0B7,0xB0E1,0xB0E2,0xB0E5,0xB0E9,
    0xB0EB,0xB0F1,0xB0F3,0xB0F6,0xB0F7,0xB141,0xB145,0xB149,0xB185,0xB1A1,
    0xB1A2,0xB1A5,0xB1A8,0xB1A9,0xB1AB,0xB1B1,0xB1B3,0xB1B7,0xB1C1,0xB1C2,
    0xB1C5,0xB1D6,0xB1E1,0xB1F6,0xB241,0xB245,0xB249,0xB251,0xB253,0xB261,
    0xB281,0xB282,0xB285,0xB289,0xB291,0xB293,0xB297,0xB2A1,0xB2B6,0xB2C1,
    0xB2E1,0xB2E5,0xB357,0xB361,0xB362,0xB365,0xB369,0xB36B,0xB370,0xB371,
    0xB373,0xB381,0xB385,0xB389,0xB391,0xB3A1,0xB3A2,0xB3A5,0xB3A9,0xB3B1,
    0xB3B3,0xB3B5,0xB3B7,0xB461,0xB462,0xB465,0xB466,0xB467,0xB469,0xB46A,
    0xB46B,0xB470,0xB471,0xB473,0xB475,0xB476,0xB477,0xB47B,0xB47C,0xB481,
    0xB482,0xB485,0xB489,0xB491,0xB493,0xB495,0xB496,0xB497,0xB4A1,0xB4A2,
    0xB4A5,0xB4A9,0xB4AC,0xB4B1,0xB4B3,0xB4B5,0xB4B7,0xB4BB,0xB4BD,0xB4C1,
    0xB4C5,0xB4C9,0xB4D3,0xB4E1,0xB4E2,0xB4E5,0xB4E6,0xB4E8,0xB4E9,0xB4EA,
    0xB4EB,0xB4F1,0xB4F3,0xB4F4,0xB4F5,0xB4F6,0xB4F7,0xB4F8,0xB4FA,0xB4FC,
    0xB541,0xB542,0xB545,0xB549,0xB551,0xB553,0xB555,0xB557,0xB561,0xB562,
    0xB563,0xB565,0xB569,0xB56B,0xB56C,0xB571,0xB573,0xB574,0xB575,0xB576,
    0xB577,0xB57B,0xB57C,0xB57D,0xB581,0xB585,0xB589,0xB591,0xB593,0xB595,
    0xB596,0xB5A1,0xB5A2,0xB5A5,0xB5A9,0xB5AA,0xB5AB,0xB5AD,0xB5B0,0xB5B1,
    0xB5B3,0xB5B5,0xB5B7,0xB5B9,0xB5C1,0xB5C2,0xB5C5,0xB5C9,0xB5D1,0xB5D3,
    0xB5D5,0xB5D6,0xB5D7,0xB5E1,0xB5E2,0xB5E5,0xB5F1,0xB5F5,0xB5F7,0xB641,
    0xB642,0xB645,0xB649,0xB651,0xB653,0xB655,0xB657,0xB661,0xB662,0xB665,
    0xB669,0xB671,0xB673,0xB675,0xB677,0xB681,0xB682,0xB685,0xB689,0xB68A,
    0xB68B,0xB691,0xB693,0xB695,0xB697,0xB6A1,0xB6A2,0xB6A5,0xB6A9,0xB6B1,
    0xB6B3,0xB6B6,0xB6B7,0xB6C1,0xB6C2,0xB6C5,0xB6C9,0xB6D1,0xB6D3,0xB6D7,
    0xB6E1,0xB6E2,0xB6E5,0xB6E9,0xB6F1,0xB6F3,0xB6F5,0xB6F7,0xB741,0xB742,
    0xB745,0xB749,0xB751,0xB753,0xB755,0xB757,0xB759,0xB761,0xB762,0xB765,
    0xB769,0xB76F,0xB771,0xB773,0xB775,0xB777,0xB778,0xB779,0xB77A,0xB77B,
    0xB77C,0xB77D,0xB781,0xB785,0xB789,0xB791,0xB795,0xB7A1,0xB7A2,0xB7A5,
    0xB7A9,0xB7AA,0xB7AB,0xB7B0,0xB7B1,0xB7B3,0xB7B5,0xB7B6,0xB7B7,0xB7B8,
    0xB7BC,0xB861,0xB862,0xB865,0xB867,0xB868,0xB869,0xB86B,0xB871,0xB873,
    0xB875,0xB876,0xB877,0xB878,0xB881,0xB882,0xB885,0xB889,0xB891,0xB893,
    0xB895,0xB896,0xB897,0xB8A1,0xB8A2,0xB8A5,0xB8A7,0xB8A9,0xB8B1,0xB8B7,
    0xB8C1,0xB8C5,0xB8C9,0xB8E1,0xB8E2,0xB8E5,0xB8E9,0xB8EB,0xB8F1,0xB8F3,
    0xB8F5,0xB8F7,0xB8F8,0xB941,0xB942,0xB945,0xB949,0xB951,0xB953,0xB955,
    0xB957,0xB961,0xB965,0xB969,0xB971,0xB973,0xB976,0xB977,0xB981,0xB9A1,
    0xB9A2,0xB9A5,0xB9A9,0xB9AB,0xB9B1,0xB9B3,0xB9B5,0xB9B7,0xB9B8,0xB9B9,
    0xB9BD,0xB9C1,0xB9C2,0xB9C9,0xB9D3,0xB9D5,0xB9D7,0xB9E1,0xB9F6,0xB9F7,
    0xBA41,0xBA45,0xBA49,0xBA51,0xBA53,0xBA55,0xBA57,0xBA61,0xBA62,0xBA65,
    0xBA77,0xBA81,0xBA82,0xBA85,0xBA89,0xBA8A,0xBA8B,0xBA91,0xBA93,0xBA95,
    0xBA97,0xBAA1,0xBAB6,0xBAC1,0xBAE1,0xBAE2,0xBAE5,0xBAE9,0xBAF1,0xBAF3,
    0xBAF5,0xBB41,0xBB45,0xBB49,0xBB51,0xBB61,0xBB62,0xBB65,0xBB69,0xBB71,
    0xBB73,0xBB75,0xBB77,0xBBA1,0xBBA2,0xBBA5,0xBBA8,0xBBA9,0xBBAB,0xBBB1,
    0xBBB3,0xBBB5,0xBBB7,0xBBB8,0xBBBB,0xBBBC,0xBC61,0xBC62,0xBC65,0xBC67,
    0xBC69,0xBC6C,0xBC71,0xBC73,0xBC75,0xBC76,0xBC77,0xBC81,0xBC82,0xBC85,
    0xBC89,0xBC91,0xBC93,0xBC95,0xBC96,0xBC97,0xBCA1,0xBCA5,0xBCB7,0xBCE1,
    0xBCE2,0xBCE5,0xBCE9,0xBCF1,0xBCF3,0xBCF5,0xBCF6,0xBCF7,0xBD41,0xBD57,
    0xBD61,0xBD76,0xBDA1,0xBDA2,0xBDA5,0xBDA9,0xBDB1,0xBDB3,0xBDB5,0xBDB7,
    0xBDB9,0xBDC1,0xBDC2,0xBDC9,0xBDD6,0xBDE1,0xBDF6,0xBE41,0xBE45,0xBE49,
    0xBE51,0xBE53,0xBE77,0xBE81,0xBE82,0xBE85,0xBE89,0xBE91,0xBE93,0xBE97,
    0xBEA1,0xBEB6,0xBEB7,0xBEE1,0xBF41,0xBF61,0xBF71,0xBF75,0xBF77,0xBFA1,
    0xBFA2,0xBFA5,0xBFA9,0xBFB1,0xBFB3,0xBFB7,0xBFB8,0xBFBD,0xC061,0xC062,
    0xC065,0xC067,0xC069,0xC071,0xC073,0xC075,0xC076,0xC077,0xC078,0xC081,
    0xC082,0xC085,0xC089,0xC091,0xC093,0xC095,0xC096,0xC097,0xC0A1,0xC0A5,
    0xC0A7,0xC0A9,0xC0B1,0xC0B7,0xC0E1,0xC0E2,0xC0E5,0xC0E9,0xC0F1,0xC0F3,
    0xC0F5,0xC0F6,0xC0F7,0xC141,0xC142,0xC145,0xC149,0xC151,0xC153,0xC155,
    0xC157,0xC161,0xC165,0xC176,0xC181,0xC185,0xC197,0xC1A1,0xC1A2,0xC1A5,
    0xC1A9,0xC1B1,0xC1B3,0xC1B5,0xC1B7,0xC1C1,0xC1C5,0xC1C9,0xC1D7,0xC241,
    0xC245,0xC249,0xC251,0xC253,0xC255,0xC257,0xC261,0xC271,0xC281,0xC282,
    0xC285,0xC289,0xC291,0xC293,0xC295,0xC297,0xC2A1,0xC2B6,0xC2C1,0xC2C5,
    0xC2E1,0xC2E5,0xC2E9,0xC2F1,0xC2F3,0xC2F5,0xC2F7,0xC341,0xC345,0xC349,
    0xC351,0xC357,0xC361,0xC362,0xC365,0xC369,0xC371,0xC373,0xC375,0xC377,
    0xC3A1,0xC3A2,0xC3A5,0xC3A8,0xC3A9,0xC3AA,0xC3B1,0xC3B3,0xC3B5,0xC3B7,
    0xC461,0xC462,0xC465,0xC469,0xC471,0xC473,0xC475,0xC477,0xC481,0xC482,
    0xC485,0xC489,0xC491,0xC493,0xC495,0xC496,0xC497,0xC4A1,0xC4A2,0xC4B7,
    0xC4E1,0xC4E2,0xC4E5,0xC4E8,0xC4E9,0xC4F1,0xC4F3,0xC4F5,0xC4F6,0xC4F7,
    0xC541,0xC542,0xC545,0xC549,0xC551,0xC553,0xC555,0xC557,0xC561,0xC565,
    0xC569,0xC571,0xC573,0xC575,0xC576,0xC577,0xC581,0xC5A1,0xC5A2,0xC5A5,
    0xC5A9,0xC5B1,0xC5B3,0xC5B5,0xC5B7,0xC5C1,0xC5C2,0xC5C5,0xC5C9,0xC5D1,
    0xC5D7,0xC5E1,0xC5F7,0xC641,0xC649,0xC661,0xC681,0xC682,0xC685,0xC689,
    0xC691,0xC693,0xC695,0xC697,0xC6A1,0xC6A5,0xC6A9,0xC6B7,0xC6C1,0xC6D7,
    0xC6E1,0xC6E2,0xC6E5,0xC6E9,0xC6F1,0xC6F3,0xC6F5,0xC6F7,0xC741,0xC745,
    0xC749,0xC751,0xC761,0xC762,0xC765,0xC769,0xC771,0xC773,0xC777,0xC7A1,
    0xC7A2,0xC7A5,0xC7A9,0xC7B1,0xC7B3,0xC7B5,0xC7B7,0xC861,0xC862,0xC865,
    0xC869,0xC86A,0xC871,0xC873,0xC875,0xC876,0xC877,0xC881,0xC882,0xC885,
    0xC889,0xC891,0xC893,0xC895,0xC896,0xC897,0xC8A1,0xC8B7,0xC8E1,0xC8E2,
    0xC8E5,0xC8E9,0xC8EB,0xC8F1,0xC8F3,0xC8F5,0xC8F6,0xC8F7,0xC941,0xC942,
    0xC945,0xC949,0xC951,0xC953,0xC955,0xC957,0xC961,0xC965,0xC976,0xC981,
    0xC985,0xC9A1,0xC9A2,0xC9A5,0xC9A9,0xC9B1,0xC9B3,0xC9B5,0xC9B7,0xC9BC,
    0xC9C1,0xC9C5,0xC9E1,0xCA41,0xCA45,0xCA55,0xCA57,0xCA61,0xCA81,0xCA82,
    0xCA85,0xCA89,0xCA91,0xCA93,0xCA95,0xCA97,0xCAA1,0xCAB6,0xCAC1,0xCAE1,
    0xCAE2,0xCAE5,0xCAE9,0xCAF1,0xCAF3,0xCAF7,0xCB41,0xCB45,0xCB49,0xCB51,
    0xCB57,0xCB61,0xCB62,0xCB65,0xCB68,0xCB69,0xCB6B,0xCB71,0xCB73,0xCB75,
    0xCB81,0xCB85,0xCB89,0xCB91,0xCB93,0xCBA1,0xCBA2,0xCBA5,0xCBA9,0xCBB1,
    0xCBB3,0xCBB5,0xCBB7,0xCC61,0xCC62,0xCC63,0xCC65,0xCC69,0xCC6B,0xCC71,
    0xCC73,0xCC75,0xCC76,0xCC77,0xCC7B,0xCC81,0xCC82,0xCC85,0xCC89,0xCC91,
    0xCC93,0xCC95,0xCC96,0xCC97,0xCCA1,0xCCA2,0xCCE1,0xCCE2,0xCCE5,0xCCE9,
    0xCCF1,0xCCF3,0xCCF5,0xCCF6,0xCCF7,0xCD41,0xCD42,0xCD45,0xCD49,0xCD51,
    0xCD53,0xCD55,0xCD57,0xCD61,0xCD65,0xCD69,0xCD71,0xCD73,0xCD76,0xCD77,
    0xCD81,0xCD89,0xCD93,0xCD95,0xCDA1,0xCDA2,0xCDA5,0xCDA9,0xCDB1,0xCDB3,
    0xCDB5,0xCDB7,0xCDC1,0xCDD7,0xCE41,0xCE45,0xCE61,0xCE65,0xCE69,0xCE73,
    0xCE75,0xCE81,0xCE82,0xCE85,0xCE88,0xCE89,0xCE8B,0xCE91,0xCE93,0xCE95,
    0xCE97,0xCEA1,0xCEB7,0xCEE1,0xCEE5,0xCEE9,0xCEF1,0xCEF5,0xCF41,0xCF45,
    0xCF49,0xCF51,0xCF55,0xCF57,0xCF61,0xCF65,0xCF69,0xCF71,0xCF73,0xCF75,
    0xCFA1,0xCFA2,0xCFA5,0xCFA9,0xCFB1,0xCFB3,0xCFB5,0xCFB7,0xD061,0xD062,
    0xD065,0xD069,0xD06E,0xD071,0xD073,0xD075,0xD077,0xD081,0xD082,0xD085,
    0xD089,0xD091,0xD093,0xD095,0xD096,0xD097,0xD0A1,0xD0B7,0xD0E1,0xD0E2,
    0xD0E5,0xD0E9,0xD0EB,0xD0F1,0xD0F3,0xD0F5,0xD0F7,0xD141,0xD142,0xD145,
    0xD149,0xD151,0xD153,0xD155,0xD157,0xD161,0xD162,0xD165,0xD169,0xD171,
    0xD173,0xD175,0xD176,0xD177,0xD181,0xD185,0xD189,0xD193,0xD1A1,0xD1A2,
    0xD1A5,0xD1A9,0xD1AE,0xD1B1,0xD1B3,0xD1B5,0xD1B7,0xD1BB,0xD1C1,0xD1C2,
    0xD1C5,0xD1C9,0xD1D5,0xD1D7,0xD1E1,0xD1E2,0xD1E5,0xD1F5,0xD1F7,0xD241,
    0xD242,0xD245,0xD249,0xD253,0xD255,0xD257,0xD261,0xD265,0xD269,0xD273,
    0xD275,0xD281,0xD282,0xD285,0xD289,0xD28E,0xD291,0xD295,0xD297,0xD2A1,
    0xD2A5,0xD2A9,0xD2B1,0xD2B7,0xD2C1,0xD2C2,0xD2C5,0xD2C9,0xD2D7,0xD2E1,
    0xD2E2,0xD2E5,0xD2E9,0xD2F1,0xD2F3,0xD2F5,0xD2F7,0xD341,0xD342,0xD345,
    0xD349,0xD351,0xD355,0xD357,0xD361,0xD362,0xD365,0xD367,0xD368,0xD369,
    0xD36A,0xD371,0xD373,0xD375,0xD377,0xD37B,0xD381,0xD385,0xD389,0xD391,
    0xD393,0xD397,0xD3A1,0xD3A2,0xD3A5,0xD3A9,0xD3B1,0xD3B3,0xD3B5,0xD3B7
};

ROM WORD awKSSMPieceHangul[] = {
    0x8841,0x8C41,0x8444,0x9041,0x8446,0x8447,0x9441,0x9841,
    0x9C41,0x844A,0x844B,0x844C,0x844D,0x844E,0x844F,0x8450,
    0xA041,0xA441,0xA841,0x8454,0xAC41,0xB041,0xB441,0xB841,
    0xBC41,0xC041,0xC441,0xC841,0xCC41,0xD041,0x8461,0x8481,
    0x84A1,0x84c1,0x84E1,0x8541,0x8561,0x8581,0x85A1,0x85C1,
    0x85E1,0x8641,0x8661,0x8681,0x86A1,0x86C1,0x86E1,0x8741,
    0x8761,0x8781,0x87A1
};

#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */


/* girin. 2000.12.31
	This table used for converting 2 color image to 4 color image.
*/
ROM WORD wColor2ToColor4Table[256] =
{
	0x0000,0x0003,0x000C,0x000F,0x0030,0x0033,0x003C,0x003F,
	0x00C0,0x00C3,0x00CC,0x00CF,0x00F0,0x00F3,0x00FC,0x00FF,
	0x0300,0x0303,0x030C,0x030F,0x0330,0x0333,0x033C,0x033F,
	0x03C0,0x03C3,0x03CC,0x03CF,0x03F0,0x03F3,0x03FC,0x03FF,
	0x0C00,0x0C03,0x0C0C,0x0C0F,0x0C30,0x0C33,0x0C3C,0x0C3F,
	0x0CC0,0x0CC3,0x0CCC,0x0CCF,0x0CF0,0x0CF3,0x0CFC,0x0CFF,
	0x0F00,0x0F03,0x0F0C,0x0F0F,0x0F30,0x0F33,0x0F3C,0x0F3F,
	0x0FC0,0x0FC3,0x0FCC,0x0FCF,0x0FF0,0x0FF3,0x0FFC,0x0FFF,
	0x3000,0x3003,0x300C,0x300F,0x3030,0x3033,0x303C,0x303F,
	0x30C0,0x30C3,0x30CC,0x30CF,0x30F0,0x30F3,0x30FC,0x30FF,
	0x3300,0x3303,0x330C,0x330F,0x3330,0x3333,0x333C,0x333F,
	0x33C0,0x33C3,0x33CC,0x33CF,0x33F0,0x33F3,0x33FC,0x33FF,
	0x3C00,0x3C03,0x3C0C,0x3C0F,0x3C30,0x3C33,0x3C3C,0x3C3F,
	0x3CC0,0x3CC3,0x3CCC,0x3CCF,0x3CF0,0x3CF3,0x3CFC,0x3CFF,
	0x3F00,0x3F03,0x3F0C,0x3F0F,0x3F30,0x3F33,0x3F3C,0x3F3F,
	0x3FC0,0x3FC3,0x3FCC,0x3FCF,0x3FF0,0x3FF3,0x3FFC,0x3FFF,
	0xC000,0xC003,0xC00C,0xC00F,0xC030,0xC033,0xC03C,0xC03F,
	0xC0C0,0xC0C3,0xC0CC,0xC0CF,0xC0F0,0xC0F3,0xC0FC,0xC0FF,
	0xC300,0xC303,0xC30C,0xC30F,0xC330,0xC333,0xC33C,0xC33F,
	0xC3C0,0xC3C3,0xC3CC,0xC3CF,0xC3F0,0xC3F3,0xC3FC,0xC3FF,
	0xCC00,0xCC03,0xCC0C,0xCC0F,0xCC30,0xCC33,0xCC3C,0xCC3F,
	0xCCC0,0xCCC3,0xCCCC,0xCCCF,0xCCF0,0xCCF3,0xCCFC,0xCCFF,
	0xCF00,0xCF03,0xCF0C,0xCF0F,0xCF30,0xCF33,0xCF3C,0xCF3F,
	0xCFC0,0xCFC3,0xCFCC,0xCFCF,0xCFF0,0xCFF3,0xCFFC,0xCFFF,
	0xF000,0xF003,0xF00C,0xF00F,0xF030,0xF033,0xF03C,0xF03F,
	0xF0C0,0xF0C3,0xF0CC,0xF0CF,0xF0F0,0xF0F3,0xF0FC,0xF0FF,
	0xF300,0xF303,0xF30C,0xF30F,0xF330,0xF333,0xF33C,0xF33F,
	0xF3C0,0xF3C3,0xF3CC,0xF3CF,0xF3F0,0xF3F3,0xF3FC,0xF3FF,
	0xFC00,0xFC03,0xFC0C,0xFC0F,0xFC30,0xFC33,0xFC3C,0xFC3F,
	0xFCC0,0xFCC3,0xFCCC,0xFCCF,0xFCF0,0xFCF3,0xFCFC,0xFCFF,
	0xFF00,0xFF03,0xFF0C,0xFF0F,0xFF30,0xFF33,0xFF3C,0xFF3F,
	0xFFC0,0xFFC3,0xFFCC,0xFFCF,0xFFF0,0xFFF3,0xFFFC,0xFFFF
};

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL GRAPHIC_DEVICE    GD[MAX_GRAPHIC_DEVICE]; /* Graphic Device */
LOCAL SCREEN_INDEX_E    l_CurrentScreen = SI_MAIN_SCREEN;
LOCAL BOOL              l_fScreenChangable = TRUE;

#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT

LOCAL CONST BYTE* pbCompHangulFontBuffer;
LOCAL BYTE* EQS_API sGetCompoundHangulImage(WORD wCode);
//LOCAL SHORT EQS_API sCodeCompare(WORD* p, WORD* q);
LOCAL WORD EQS_API  sKS5601_2KSSM(WORD wCode);

#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */

//LOCAL VOID EQS_API (*sPutFontBitmap)(COORD x, COORD y, CONST BITMAP* pBitmap);

//#ifdef FEATURE_EQS_SLIDING_WINDOW
//LOCAL SLIDING_WINDOW  SlidingWindow;
//#endif /* FEATURE_EQS_SLIDING_WINDOW */

LOCAL BOOL l_fPutFont;
//LOCAL COLOR l_TransparentColor = MAX_COLOR; /* no transparent */

#ifdef FEATURE_SKVM
EQS_DCL BYTE*        EQS_API EQS_SetSKVMTextDrawMode(CONST BYTE* pSrc, INT8 nWidthByte, INT8 nHeight);
EQS_DCL CONST BYTE*  EQS_API EQS_GetFontBuffer(WORD wCode, CONST BITMAP_FONT *pBitmapFont);
EQS_DCL CONST BITMAP_FONT* EQS_API EQS_GetBitmapFontByCode(WORD wCode);
#endif 
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void EQS_API sSetMemoryByWord(WORD *pDest, WORD wSetData, WORD wCount);
LOCAL VOID               EQS_API sQuadrantCircle(COORD xCenter, COORD yCenter, 
                                                 INT16 nRadius, INT8 nQuadrant);
LOCAL VOID               EQS_API sPlotQuadrantCirclePoints(
                                             COORD xCenter, COORD yCenter, 
                                             COORD x, COORD y, INT8 nQuadrant);
LOCAL VOID               EQS_API sPlotEllipsePoints(COORD xCenter, COORD yCenter, 
                                              COORD x, COORD y);
/* ************************************************************************ */
/* Draw bitmap font                                                        */
/* ************************************************************************ */
LOCAL VOID               EQS_API sPutBitmapFont(COORD x, COORD y, WORD wCode, 
                                               CONST BITMAP_FONT* pBitmapFont);
LOCAL CONST BITMAP_FONT_SET*  EQS_API sGetCurrentBitmapFontSet(VOID);

LOCAL CONST BYTE*        EQS_API sGetFontBuffer(WORD wCode, 
                                              CONST BITMAP_FONT *pBitmapFont);
LOCAL CONST BITMAP_FONT* EQS_API sGetBitmapFontByCode(WORD wCode);
LOCAL BYTE*              EQS_API sSetTextDrawMode(CONST BYTE* pSrc, 
                                              INT8 nWidthByte, INT8 nHeight);

/* 
** Draw bitmap image 
*/
/* PutFontBitmap, PutBitmap in 64K color system */
#ifdef FEATURE_EQS_COLOR_64K
//LOCAL VOID EQS_API s256C_PutFontBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s64KC_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s64KC_PutBitmapRect(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

LOCAL VOID EQS_API s64KC_PutBitmap256C(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s64KC_PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

LOCAL VOID EQS_API s64KC_PutBitmap4G(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s64KC_PutBitmapBW(COORD x, COORD y, CONST BITMAP* pBitmap);
#endif /* FEATURE_EQS_COLOR_64K */

/* PutFontBitmap, PutBitmap in 256 color system */
#ifdef FEATURE_EQS_COLOR_256
//LOCAL VOID EQS_API s256C_PutFontBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s256C_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s256C_PutBitmap256C(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s256C_PutBitmap4G(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API s256C_PutBitmapBW(COORD x, COORD y, CONST BITMAP* pBitmap);
#endif /* FEATURE_EQS_COLOR_256 */

/* PutFontBitmap, PutBitmap in BW or 4 gray color system */
//LOCAL VOID EQS_API sBW4G_PutFontBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
LOCAL VOID EQS_API sBW_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
//#ifdef FEATURE_EQS_COLOR_4GRAY
LOCAL VOID EQS_API s4G_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
//#endif /* FEATURE_EQS_COLOR_4GRAY */

//#ifdef FEATURE_EQS_COLOR_4GRAY
LOCAL void EQS_API s4G_VertLine(COORD xPos, COORD yStart, COORD yEnd);
LOCAL void EQS_API s4G_HorizLine(COORD xStart, COORD yPos, COORD xEnd);
//#endif//#ifdef FEATURE_EQS_COLOR_4GRAY


/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Graphics system                                                         */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
EQS_RESULT EQS_API OpenGraph(SCREEN* pScreen, SCREEN_INDEX_E ScreenIndex, FONT_TYPE FontType)
{
    EQS_RESULT Result = RESULT_OK;
    POINT Pos = { 0, 0 };
    TEXT_ALIGN TextAlign = { INIT_TAH, INIT_TAV };
//    SIZE_T stBufSize;

#ifdef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER
    /* the memory allocation for the screen buffer */
    stBufSize = 
        ((pScreen->Info.Size.xWidth)*(pScreen->Info.Size.yHeight)
        *(pScreen->Info.ColorSpace))/8;

    GD[ScreenIndex].Screen.pbBuffer = (BYTE*)EQS_Calloc(1, stBufSize);
#else
    GD[ScreenIndex].Screen.pbBuffer = pScreen->pbBuffer;
#endif /* FEATURE_EQS_ALLOCATED_SCREEN_BUFFER */

    EQS_ASSERT(GD[ScreenIndex].Screen.pbBuffer != NULL);

#ifndef FEATURE_EQS_COLOR_256
    /* 
    ** if the EQUS graphic system is not 256 color,
    ** setting color space to CS_256COLOR is not valid.
    */
    if(pScreen->Info.ColorSpace == CS_256COLOR)
        return RESULT_NOT_SUPPORTED;
#endif /* CONFIG_EQS_COLOR_256 */

    GD[ScreenIndex].Screen = *pScreen;
    GD[ScreenIndex].DC.ViewPort.CoordSystem = INIT_CS;
    GD[ScreenIndex].DC.ViewPort.Origin = Pos;
    GD[ScreenIndex].DC.ViewPort.Rect.xLeft = 0;
    GD[ScreenIndex].DC.ViewPort.Rect.yTop = 0;
    GD[ScreenIndex].DC.ViewPort.Rect.xRight = DEV_GET_MAX_X();//REAL_GET_MAX_X();
    GD[ScreenIndex].DC.ViewPort.Rect.yBottom = DEV_GET_MAX_Y();

    GD[ScreenIndex].DC.BackColor = BACK_COLOR;//INIT_BACKCOLOR;
    GD[ScreenIndex].DC.ForeColor = FORE_COLOR;//INIT_FORECOLOR;

    GD[ScreenIndex].DC.CurrentPosition = Pos;

    GD[ScreenIndex].DC.DrawMode = INIT_DRAWMODE;

    GD[ScreenIndex].DC.TextColor = TEXT_FORE_COLOR;//INIT_TEXTCOLOR;
    GD[ScreenIndex].DC.TextBackColor = TEXT_BACK_COLOR;//INIT_TEXTBACKCOLOR;

    GD[ScreenIndex].DC.nInterCharSpace = INIT_INTERCHARSPACE;
    GD[ScreenIndex].DC.TextAlign = TextAlign;
    GD[ScreenIndex].DC.TextDrawMode = INIT_TEXTDRAWMODE;

    GD[ScreenIndex].DC.FontType = FontType;//INIT_FONT_TYPE;
    GD[ScreenIndex].DC.fTextWrap = INIT_TEXTWRAP;

//    switch(GET_SCREEN_BITS_PER_PIXEL())
    switch(GD[ScreenIndex].Screen.Info.ColorSpace)
    {
    case CS_64KCOLOR : 
#ifdef FEATURE_EQS_COLOR_64K
        PutBitmap = GD[ScreenIndex].PutBitmap = s64KC_PutBitmap;
		PutBitmapRect = s64KC_PutBitmapRect;
#endif /* FEATURE_EQS_COLOR_64C */
        break;
    case CS_256COLOR :
#ifdef FEATURE_EQS_COLOR_256 
        //sPutFontBitmap = s256C_PutFontBitmap;
        PutBitmap = GD[ScreenIndex].PutBitmap = s256C_PutBitmap;
#endif /* FEATURE_EQS_COLOR_256 */
        break;
    case CS_BW :
        PutBitmap = GD[ScreenIndex].PutBitmap = sBW_PutBitmap;
        break;
//#ifdef FEATURE_EQS_COLOR_4GRAY
    case CS_4GRAY :
        //sPutFontBitmap = sBW4G_PutFontBitmap;
        PutBitmap = GD[ScreenIndex].PutBitmap = s4G_PutBitmap;
//#endif /* FEATURE_EQS_COLOR_256 */
        break;
    default :
        /* Invalid color space in OpenGraph !! **/
        Result = RESULT_ERROR;
        break;
    }
    return Result;
}

EQS_RESULT EQS_API CloseGraph(SCREEN_INDEX_E ScreenIndex)
{
    EQS_RESULT Result = RESULT_OK;

#ifdef FEATURE_EQS_ALLOCATED_SCREEN_BUFFER
    EQS_Free(&(GD[ScreenIndex].Screen.pbBuffer));
#endif /* FEATURE_EQS_ALLOCATED_SCREEN_BUFFER */

    return Result;
}

SCREEN_INDEX_E EQS_API SelectScreen(SCREEN_INDEX_E ScreenIndex)
{
    SCREEN_INDEX_E OldScreenIndex;

    /* get the old screen index */
    OldScreenIndex = l_CurrentScreen;
    
    /* only if screen can be changed */
    if(IsScreenChangable())
    {
        PutBitmap = GD[ScreenIndex].PutBitmap;
        /* set the new screen index */
        l_CurrentScreen = ScreenIndex;
    }
    
    /* return the old screen index */
    return OldScreenIndex;
}

SCREEN_INDEX_E EQS_API GetCurrentScreen(void)
{
    return l_CurrentScreen;
}

void EQS_API SetScreenChangable(BOOL fChangable)
{
    l_fScreenChangable = fChangable;
}

BOOL EQS_API IsScreenChangable(void)
{
    return l_fScreenChangable;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Screen                                                                   */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

BYTE* EQS_API GetScreenBuffer(VOID)
{
    return sGETGD().Screen.pbBuffer;
}

// girin. 2000.12.27
// For external text display routines (e.g. WAP)
ROM BYTE* EQS_API GetBitmapFontBufferByCode(WORD wCode)
{
    CONST BITMAP_FONT* pBitmapFont;

    pBitmapFont = sGetBitmapFontByCode(wCode);

	return sGetFontBuffer(wCode, pBitmapFont);
}

LOCAL void EQS_API sSetMemoryByWord(WORD *pDest, WORD wSetData, WORD wCount)
{
    register int i;
    //WORD* pwDest = pDest;

    for(i = 0; i < wCount; i++)
    {
        *pDest++ = wSetData;
    }
}

//VOID EQS_API ClearScreen(CLEAR_SCREEN_TYPE ClearType)
VOID EQS_API ClearScreen(VOID)
{
    SIZE_T stBufSize;
    SIZE* pSize;
    COORD y;

    /* get the size of scrren buffer */
    stBufSize = 
        ((DEV_GET_MAX_BUFFER_WIDTH())*(DEV_GET_MAX_HEIGHT())//((DEV_GET_MAX_WIDTH())*(DEV_GET_MAX_HEIGHT())
        *(sGETGD().Screen.Info.ColorSpace))/8;

    /*
    ** if the current background color is BLACK or WHITE,
    ** just fill 0x00 or 0xFF instead of calling
    ** SET_PIXEL by the pixel unit
    */
    if(GET_BACK_COLOR() == BLACK)
        memset(sGETGD().Screen.pbBuffer, '\0', stBufSize);
    else if(GET_BACK_COLOR() == WHITE)
        memset(sGETGD().Screen.pbBuffer, 0xFF, stBufSize);
    else
    {
        /* 
        ** if 256 color system, just fill the color 
        ** byte into the screen buffer 
        */
        if(GET_SCREEN_BITS_PER_PIXEL() == CS_256COLOR)
            memset(sGETGD().Screen.pbBuffer, (BYTE)/*R_LIGHTGRAY*/GET_BACK_COLOR(), stBufSize);
        else if(GET_SCREEN_BITS_PER_PIXEL() == CS_64KCOLOR)
        {
            sSetMemoryByWord((WORD*)(sGETGD().Screen.pbBuffer), (WORD)GET_BACK_COLOR(), (WORD)(stBufSize>>1));
        }
        else
        {
            COLOR OldColor;

            OldColor = GetForeColor();
            SetForeColor(GetBackColor());
            pSize = GET_SCREEN_SIZE();
            for(y = 0; y < pSize->yHeight; y++)
                Line(0, y, (COORD)(pSize->xWidth - 1), y);
            SetForeColor(OldColor);
        }
    }
}

/* clear the current viewport area */
VOID EQS_API ClearViewPort(VOID)
{
    RECT Rect;
    COORD xWidth, yHeight;
    COORD /*x, */y;
    COLOR OldColor;
    
    GetViewPortRect(&Rect);

    xWidth = Rect.xRight - Rect.xLeft + 1;
    yHeight = Rect.yBottom - Rect.yTop + 1;

    OldColor = GetForeColor();
    SetForeColor(GetBackColor());
    for(y = 0; y < yHeight; y++)
    {
        Line(Rect.xLeft, y, Rect.xRight, y);
        /*for(x = 0; x < xWidth; x++)
        {
            
            SetPixel(x, y, GET_BACK_COLOR());
        }*/
    }
    SetForeColor(OldColor);
}

/* clear the region -- viewport */
VOID EQS_API ClearRegion(COORD xLeft, COORD yTop, COORD xRight, COORD yBottom)
{
    COORD /*x, */y;
    COLOR OldColor;

    OldColor = GetForeColor();
    SetForeColor(GetBackColor());
   
    for(y = yTop; y <= yBottom; y++)
    {
        Line(xLeft, y, xRight, y);
        /*for(x = xLeft; x < xRight; x++)
        {
            SetPixel(x, y, GET_BACK_COLOR());
        }*/
    }
    SetForeColor(OldColor);
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Sliding window                                                           */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
#ifdef FEATURE_EQS_SLIDING_WINDOW

VOID EQS_API SetSlidingWindowState(BOOL fEnable)
{
    sGETSW().fEnable = fEnable;
}

BOOL EQS_API GetSlidingWindowState(VOID)
{
    return sGETSW().fEnable;
}

#if defined(FEATURE_EQS_SLIDING_WINDOW_X) \
    && defined(FEATURE_EQS_SLIDING_WINDOW_Y)

VOID EQS_API SetSlidingWindowOffset(POINT* pOffset)
{
    sGETSW().Offset = *pOffset;
}

POINT* EQS_API GetSlidingWindowOffset(VOID)
{
    return &(sGETSW().Offset);
}

#endif /* defined(FEATURE_EQS_SLIDING_WINDOW_X) \
    && defined(FEATURE_EQS_SLIDING_WINDOW_Y) */

#ifdef FEATURE_EQS_SLIDING_WINDOW_X

VOID EQS_API SetSlidingWindowOffsetX(COORD xOffset)
{
    //ASSERT(xOffset >= 0);
	if( xOffset > 0 )
	{
		sGETSW().Offset.x = xOffset;
	}
	else
	{
		sGETSW().Offset.x = 0;
	}
}

COORD EQS_API GetSlidingWindowOffsetX(VOID)
{
    return sGETSW().Offset.x;
}

#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y

VOID EQS_API SetSlidingWindowOffsetY(COORD yOffset)
{
    //ASSERT(yOffset >= 0);
	if( yOffset > 0 )
	{
		sGETSW().Offset.y = yOffset;
	}
	else
	{
		sGETSW().Offset.y = 0;
	}
}

COORD EQS_API GetSlidingWindowOffsetY(VOID)
{
    return sGETSW().Offset.y;
}

#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#endif /* FEATURE_EQS_SLIDING_WINDOW */

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Viewport and screen info                                                */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API GetViewPort(VIEW_PORT* pViewPort)
{
    *pViewPort = sGETDC().ViewPort; /* copy the member of structure */
}

VOID EQS_API GetViewPortRect(RECT *pRect)
{
    *pRect = sGETVP().Rect;
}

VOID EQS_API SetViewPort(VIEW_PORT* pViewPort)
{
    sGETDC().ViewPort = *pViewPort;
    SetCoordinates(sGETVP().CoordSystem);
    SetOrigin(&(sGETVP().Origin));
}

VOID EQS_API SetViewPortRect(RECT* pRect)
{
    sGETVP().Rect = *pRect;
}

VOID EQS_API ResetViewPort(VOID)
{
    POINT Origin = { 0, 0 };

    sGETVP().Rect.xLeft = 0;
    sGETVP().Rect.yTop = 0;
    sGETVP().Rect.xRight = DEV_GET_MAX_X();//REAL_GET_MAX_X();
    sGETVP().Rect.yBottom = DEV_GET_MAX_Y();
    SetCoordinates(INIT_CS);
    SetOrigin(&Origin);
}

COORD EQS_API DevGetMaxWidth(VOID)
{
    return DEV_GET_MAX_WIDTH();
}

COORD EQS_API DevGetMaxHeight(VOID)
{
    return DEV_GET_MAX_HEIGHT();
}

COORD EQS_API DevGetMaxX(VOID)
{
    return DEV_GET_MAX_WIDTH() - 1;
//    return REAL_GET_MAX_X();
}

COORD EQS_API DevGetMaxY(VOID)
{
    return DEV_GET_MAX_HEIGHT() - 1;
}

COORD EQS_API GetMaxWidth(VOID)
{
    return sGETVP().Rect.xRight - sGETVP().Rect.xLeft + 1;
}

COORD EQS_API GetMaxHeight(VOID)
{
    return sGETVP().Rect.yBottom - sGETVP().Rect.yTop + 1;
}

COORD EQS_API GetMaxX(VOID)
{
    return GET_MAX_WIDTH() - 1;
}

COORD EQS_API GetMaxY(VOID)
{
    return GET_MAX_HEIGHT() - 1;
}

INT8 EQS_API GetScreenBitsPerPixel(VOID)
{
    return (INT8)sGETGD().Screen.Info.ColorSpace;
}

SIZE* EQS_API GetScreenSize(VOID)
{
    return &(sGETGD().Screen.Info.Size);
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Current position                                                        */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

POINT* EQS_API DevGetCurrentPosition(VOID)
{
    return &sGETDC().CurrentPosition;
}

COORD EQS_API DevGetCurrentPositionX(VOID)
{
    return sGETDC().CurrentPosition.x;
}

COORD EQS_API DevGetCurrentPositionY(VOID)
{
    return sGETDC().CurrentPosition.y;
}

POINT* EQS_API GetCurrentPosition(VOID)
{
    DP2LP(&sGETDC().CurrentPosition);
    return &sGETDC().CurrentPosition;
}

COORD EQS_API GetCurrentPositionX(VOID)
{
    return DPx2LPx(sGETDC().CurrentPosition.x);
}

COORD EQS_API GetCurrentPositionY(VOID)
{
    return DPy2LPy(sGETDC().CurrentPosition.y);
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Color                                                                   */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/*COLOR EQS_API GetTransparentColor(VOID)
{
    return l_TransparentColor;
}*/

COLOR EQS_API GetForeColor(VOID)
{
    return sGETDC().ForeColor;
}

COLOR EQS_API GetBackColor(VOID)
{
    return sGETDC().BackColor;
}

COLOR EQS_API GetTextColor(VOID)
{
    return sGETDC().TextColor;
}

COLOR EQS_API GetTextBackColor(VOID)
{
    return sGETDC().TextBackColor;
}

/*VOID EQS_API SetTransparentColor(COLOR TransparentColor)
{
    l_TransparentColor = TransparentColor;
}*/

VOID EQS_API SetForeColor(COLOR ForeColor)
{
    sGETDC().ForeColor = ForeColor;
}

VOID EQS_API SetBackColor(COLOR BackColor)
{
    sGETDC().BackColor = BackColor;
}

VOID EQS_API SetTextColor(COLOR TextColor)
{
    sGETDC().TextColor = TextColor;
}

VOID EQS_API SetTextBackColor(COLOR TextBackColor)
{
    sGETDC().TextBackColor = TextBackColor;
}

VOID EQS_API SetDrawMode(DRAW_MODE DrawMode)
{
    sGETDC().DrawMode = DrawMode;
}

DRAW_MODE EQS_API GetDrawMode(VOID)
{
    return sGETDC().DrawMode;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Primitive shapes                                                        */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

COLOR EQS_API DevGetPixel(COORD x, COORD y)
{
#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

    /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
    if(!(x >= 0 && x <= DEV_GET_MAX_X()) || !(y >= 0 && y <= DEV_GET_MAX_Y()))
        return MAX_COLOR;
#endif /* FEATURE_EQS_CLIPPING */

    if(GET_SCREEN_BITS_PER_PIXEL() == CS_256COLOR)
    { 
        //return GET_PIXEL_256C(x, y, GET_SCREEN_SIZE()->xWidth, GET_SCREEN_BUFFER()); 
        return (COLOR)GET_PIXEL_256C(x, y, DEV_GET_MAX_BUFFER_WIDTH(), GET_SCREEN_BUFFER()); 
    } 
    else if(GET_SCREEN_BITS_PER_PIXEL() == CS_4GRAY) 
    { 
//        return GET_PIXEL_4G(x, y, GET_SCREEN_SIZE()->xWidth, GET_SCREEN_BUFFER()); 
        return (COLOR)GET_PIXEL_4G(x, y, DEV_GET_MAX_BUFFER_WIDTH(), GET_SCREEN_BUFFER()); 
    } 
    else if(GET_SCREEN_BITS_PER_PIXEL() == CS_BW) 
    { 
//        return GET_PIXEL_BW(x, y, GET_SCREEN_SIZE()->xWidth, GET_SCREEN_BUFFER()); 
        return (COLOR)GET_PIXEL_BW(x, y, DEV_GET_MAX_BUFFER_WIDTH(), GET_SCREEN_BUFFER()); 
    } 
    else 
    {
        ASSERT(0);
        return MAX_COLOR;
    }
}

COLOR EQS_API GetPixel(COORD x, COORD y)
{
    return DevGetPixel(LPX_2_DPX(x), LPy2DPy(y));
}

VOID EQS_API DevSetPixel(COORD x, COORD y, COLOR Color)
{
#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

    /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
    if(!(x >= 0 && x <= DEV_GET_MAX_X()) || !(y >= 0 && y <= DEV_GET_MAX_Y()))
        return;
#endif /* FEATURE_EQS_CLIPPING */

    SET_PIXEL(x, y, DEV_GET_MAX_BUFFER_WIDTH(), /*GET_SCREEN_SIZE()->xWidth, */
              GET_SCREEN_BUFFER(), Color);
}
    
VOID EQS_API SetPixel(COORD x, COORD y, COLOR Color)
{
    //DevSetPixel(LPX_2_DPX(x), LPy2DPy(y), Color);
    DEV_SET_PIXEL(LPX_2_DPX(x), LPy2DPy(y), Color);
}
    

VOID EQS_API MoveTo(COORD x, COORD y)
{
    sGETDC().CurrentPosition.x = LPX_2_DPX(x);
    sGETDC().CurrentPosition.y = LPy2DPy(y);
}

VOID EQS_API MoveRel(COORD dx, COORD dy)
{
    sGETDC().CurrentPosition.x += dx;
    sGETDC().CurrentPosition.y += dy;
}

//#ifdef FEATURE_EQS_COLOR_4GRAY

#define sCHECK_BITS4GRAY       0xC0  /* 1100 0000 */
#ifndef MIN
#define MIN(a,b)    ((a)<(b)?(a):(b))
#endif

LOCAL void EQS_API s4G_VertLine(COORD xPos, COORD yStart, COORD yEnd)
{
    BYTE bData;
    BYTE bWrite2Data;
    INT8 nBitPos;
    BYTE bBitMask;
//    INT8 i;
    COORD y;
//    BYTE ab4G[] = { 0x00, 0x30, 0xC0, 0x0F0 };

    /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
    if(xPos < 0)
        xPos = 0;
    if(xPos > DEV_GET_MAX_X())
        xPos = DEV_GET_MAX_X();
    if(yStart < 0)
        yStart = 0;
    if(yStart > DEV_GET_MAX_Y())
        yStart = DEV_GET_MAX_Y();
    if(yEnd < 0)
        yEnd = 0;
    if(yEnd > DEV_GET_MAX_Y())
        yEnd = DEV_GET_MAX_Y();
#endif /* FEATURE_EQS_CLIPPING */

    /* get foreground color */
    bBitMask = GET_FORE_COLOR() << 6; /* 00, 01, 10, 11 */
    
    //y = yStart;
    //dx = xEnd - xStart;
    yEnd = MIN(yEnd, DEV_GET_MAX_Y());

    //for(x = xStart; x < xEnd; x++)
    for(y = yStart; y <= yEnd; y++)
    {
        /* get the byte where the start point is located from screen buffer */
        bData = *(GET_SCREEN_BUFFER()
                     + BYTE_POS_4G(INDEX_ON_SCREEN(xPos, y, DEV_GET_MAX_BUFFER_WIDTH())));//GET_SCREEN_SIZE()->xWidth)));
        /* get the bit position of start x in the byte which just get */
        nBitPos = BIT_POS_4G(INDEX_ON_SCREEN(xPos, y, DEV_GET_MAX_BUFFER_WIDTH()));//GET_SCREEN_SIZE()->xWidth));

        bWrite2Data = bData & ~(sCHECK_BITS4GRAY >> nBitPos);
        if(bData) /* data exist in the screen buffer on that position */
        {
/*            if(bData & (bBitMask >> nBitPos))
            {
                //bWrite2Data &= ~(bBitMask >> nBitPos);
                if(GET_DRAW_MODE() == DM_COPY)
                    bWrite2Data |= (bBitMask >> nBitPos);
            }
            else*/
            if(GET_DRAW_MODE() == DM_XOR)
            {
                bWrite2Data &= ~(bBitMask >> nBitPos);
            }
            else
            {
                bWrite2Data |= (bBitMask >> nBitPos);
            }
        }
        else
        {
            bWrite2Data |= (bBitMask >> nBitPos);
        }

//GET_SCREEN_SIZE()->xWidth, 
        SET_COLOR_BYTE_4G(xPos, y, DEV_GET_MAX_BUFFER_WIDTH(),
                          GET_SCREEN_BUFFER(), bWrite2Data);
        //x += xOffset;
    }
}

LOCAL void EQS_API s4G_HorizLine(COORD xStart, COORD yPos, COORD xEnd)
{
    BYTE bData;
    BYTE bWrite2Data;
    INT8 nBitPos;
    BYTE bBitMask;
    INT8 i;
    COORD x, xOld;//, dx;

    /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
    if(xStart < 0)
        xStart = 0;
    if(xStart > DEV_GET_MAX_X())
        xStart = DEV_GET_MAX_X();
    if(yPos < 0)
        yPos = 0;
    if(yPos > DEV_GET_MAX_Y())
        yPos = DEV_GET_MAX_Y();
    if(xEnd < 0)
        xEnd = 0;
    if(xEnd > DEV_GET_MAX_X())
        xEnd = DEV_GET_MAX_X();
#endif /* FEATURE_EQS_CLIPPING */
    /* get foreground color */
    bBitMask = GET_FORE_COLOR() << 6; /* 00, 01, 10, 11 */
    
    x = xStart;
    //dx = xEnd - xStart;
    xEnd = MIN(xEnd, DEV_GET_MAX_X());

    //for(x = xStart; x < xEnd; x++)
    /* 
    ** 추가 개선점
    ** 
    ** 시작점과 끝점 사이에서 바이트 단위로 처리될 수 있는 중간부분은 
    ** 픽셀(2비트) 단위로 처리하지 않고, 0x00, 0x55, 0xAA, 0xFF의 
    ** 색 테이블을 바이트단위로 직접 써주면 좀 더 빠르게 디스플레이될 것이다.
    */
    while(x <= xEnd)
    {
        xOld = x;

        /* get the byte where the start point is located from screen buffer */
        bData = *(GET_SCREEN_BUFFER()
                     + BYTE_POS_4G(INDEX_ON_SCREEN(x, yPos, DEV_GET_MAX_BUFFER_WIDTH())));//GET_SCREEN_SIZE()->xWidth)));
        /* get the bit position of start x in the byte which just get */
        nBitPos = BIT_POS_4G(INDEX_ON_SCREEN(x, yPos, DEV_GET_MAX_BUFFER_WIDTH()));//GET_SCREEN_SIZE()->xWidth));
        if(bData) /* data exist in the screen buffer on that position */
        {
            bWrite2Data = bData & ~(sCHECK_BITS4GRAY >> nBitPos);
            for(i = nBitPos; i < 8 && x <= xEnd; i+=2, x++)
            {
                bWrite2Data &= ~(sCHECK_BITS4GRAY >> i);
                if(bData & (bBitMask >> i))
                {
                    bWrite2Data &= ~(bBitMask >> i);
                    if(GET_DRAW_MODE() != DM_XOR)
                        bWrite2Data |= (bBitMask >> i);
                }
                else
                {
                    bWrite2Data |= (bBitMask >> i);
                }
            }
        }
        else
        {
            for(i = nBitPos, bWrite2Data = 0; i < 8 && x <= xEnd; i+=2, x++)
            {
                bWrite2Data |= (bBitMask >> i);
            }
        }

//GET_SCREEN_SIZE()->xWidth, 
        SET_COLOR_BYTE_4G(xOld, yPos, DEV_GET_MAX_BUFFER_WIDTH(),
                          GET_SCREEN_BUFFER(), bWrite2Data);
        //x += xOffset;
    }
}

//#endif /* FEATURE_EQS_COLOR_4GRAY */

/* Bresenham algorithm */
VOID EQS_API Line(COORD xStart, COORD yStart, COORD xEnd, COORD yEnd)
{
    COORD dx, dy;
    INT16 nDy2, nDyDx2;
    COORD x, y, xEndPos;
    //COORD xTemp, yTemp;
    INT16 nPos;
    INT16 nDx2, nDxDy2;
    INT16 nPos1;					
    COORD yEndPos;
	//int i;

    xStart = LPX_2_DPX(xStart);
    yStart = LPy2DPy(yStart);
    xEnd = LPX_2_DPX(xEnd);
    yEnd = LPy2DPy(yEnd);

    dx = ABS(xStart - xEnd);
    dy = ABS(yStart - yEnd);

    if(dx == 0) /* vertical line */
    {
        y = (yStart - yEnd) > 0 ? yEnd : yStart;
        yEnd = (yStart - yEnd) > 0 ? yStart : yEnd;

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
        if(GET_SLIDING_WINDOW_STATE())
            xStart -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
        if(GET_SLIDING_WINDOW_STATE())
        {
            y -= GET_SLIDING_WINDOW_OFFSET_Y();
            yEnd -= GET_SLIDING_WINDOW_OFFSET_Y();
        }
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

        /*if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW)
        {
        }
        else*/ if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_4G)
        {
            s4G_VertLine(xStart, y, yEnd);
        }
        else if((GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C)
                || (GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW)
                || (GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC))
        {
            /* this drawing routine is too slow to display in BW device 
            ** but, I'm busy now(2001.01.29) ....
            */
            while(y <= yEnd)
            {
        /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
                if(!(xStart >= 0 && xStart <= DEV_GET_MAX_X()) || !(y >= 0 && y <= DEV_GET_MAX_Y()))
                {
                    y++;
                    continue;
                }
#endif /* FEATURE_EQS_CLIPPING */

                SET_PIXEL(xStart, y, DEV_GET_MAX_BUFFER_WIDTH(),//GET_SCREEN_SIZE()->xWidth, 
                          GET_SCREEN_BUFFER(), GET_FORE_COLOR());
                y++;
            }
        }
    }
    else if(dy == 0) /* horizontal line */
    {
        x = (xStart - xEnd) > 0 ? xEnd : xStart;
        xEnd = (xStart - xEnd) > 0 ? xStart : xEnd;

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
        if(GET_SLIDING_WINDOW_STATE())
        {
            x -= GET_SLIDING_WINDOW_OFFSET_X();
            xEnd -= GET_SLIDING_WINDOW_OFFSET_X();
        }
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
        if(GET_SLIDING_WINDOW_STATE())
            yStart -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif // FEATURE_EQS_SLIDING_WINDOW_Y 

        /*if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW)
        {
        }
        else*/ if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_4G)
        {
            s4G_HorizLine(x, yStart, xEnd);
        }
        else if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC && GET_DRAW_MODE() == DM_COPY)
        {
            if(x < 0) x = 0;
            if(xEnd > DEV_GET_MAX_X()) xEnd = DEV_GET_MAX_X();
            if(yStart < 0) yStart = 0;
            if(yStart > DEV_GET_MAX_Y()) yStart = DEV_GET_MAX_Y();

            sSetMemoryByWord((WORD*)(GET_SCREEN_BUFFER() + BYTE_POS_64KC(INDEX_ON_SCREEN((x),(yStart),(DEV_GET_MAX_BUFFER_WIDTH())))), (WORD)GET_FORE_COLOR(), (WORD)(xEnd-x+1));
        }
        else if(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C && GET_DRAW_MODE() == DM_COPY)
        {
            if(x < 0) x = 0;
            if(xEnd > DEV_GET_MAX_X()) xEnd = DEV_GET_MAX_X();
            if(yStart < 0) yStart = 0;
            if(yStart > DEV_GET_MAX_Y()) yStart = DEV_GET_MAX_Y();

            memset(GET_SCREEN_BUFFER() + BYTE_POS_256C(INDEX_ON_SCREEN((x),(yStart),(DEV_GET_MAX_BUFFER_WIDTH()))), GET_FORE_COLOR(), xEnd-x+1);
        }
        else if((GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW)
                || (GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C)
                || (GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC))
        {
            /* this drawing routine is too slow to display in BW device 
            ** but, I'm busy now(2001.01.29) ....
            */
            while(x <= xEnd)
            {
        /* the simplest clipping */
#ifdef FEATURE_EQS_CLIPPING
                if(!(x >= 0 && x <= DEV_GET_MAX_X()) || !(yStart >= 0 && yStart <= DEV_GET_MAX_Y()))
                {
                    x++;
                    continue;
                }
#endif /* FEATURE_EQS_CLIPPING */

                SET_PIXEL(x, yStart, DEV_GET_MAX_BUFFER_WIDTH(),//GET_SCREEN_SIZE()->xWidth, 
                          GET_SCREEN_BUFFER(), GET_FORE_COLOR());

                x++;
            }
        }
    }
    else /* slanted line */
    {
        nDy2 = dy * 2;
        nDyDx2 = (dy - dx) * 2;
        nPos = (2 * dy) - dx;
        nDx2 = dx * 2;
        nDxDy2 = (dx - dy) * 2;
        nPos1 = (2 * dx) - dy;

        if (dx >= dy)
        {
            /* Determine which point to use as start, which as end */
            if (xStart > xEnd)
                x = xEnd, y = yEnd, xEndPos = xStart;
            else
                x= xStart, y = yStart, xEndPos = xEnd;

            //DevSetPixel(x, y, GET_FORE_COLOR());
            DEV_SET_PIXEL(x, y, GET_FORE_COLOR());

            while(x < xEndPos)
            {
                x++;
                if(nPos < 0)
                {
                    nPos += nDy2;
                }
                else
                {
                    if (xStart > xEnd)
                    {
                        if((yEnd - yStart) > 0)
                            y--;
                        else
                            y++;
                    }
                    else
                    {
                        if((yEnd - yStart) > 0)
                            y++;
                        else
                            y--;
                    }

                    nPos += nDyDx2;
                }
                //DevSetPixel(x, y, GET_FORE_COLOR());
                DEV_SET_PIXEL(x, y, GET_FORE_COLOR());
            }
        }
        else
        {
            /* Determine which point to use as start, which as end */
            if (yStart > yEnd)
                y = yEnd, x = xEnd, yEndPos = yStart;
            else
                y = yStart, x = xStart, yEndPos = yEnd;

            //DevSetPixel(x, y, GET_FORE_COLOR());
            DEV_SET_PIXEL(x, y, GET_FORE_COLOR());

            while(y < yEndPos)
            {
                y++;
                if(nPos1 < 0)
                {
                    nPos1 += nDx2;
                }
                else
                {
                    if (yStart > yEnd)
                    {
                        if((xEnd - xStart) > 0)
                            x--;
                        else
                            x++;
                    }
                    else
                    {
                        if((xEnd - xStart) > 0)
                            x++;
                        else
                            x--;
                    }
                    nPos1 += nDxDy2;
                }
                //DevSetPixel(x, y, GET_FORE_COLOR());
                DEV_SET_PIXEL(x, y, GET_FORE_COLOR());
            }
        }
    }
}
                
VOID EQS_API LineTo(COORD x, COORD y)
{
    Line(GetCurrentPositionX(), GetCurrentPositionY(), x, y);
}

VOID EQS_API LineRel(COORD dx, COORD dy)
{
    LineTo((COORD)(GetCurrentPositionX() + dx), 
           (COORD)(GetCurrentPositionY() + dy));
}

VOID EQS_API Rectangle(COORD xLeft, COORD yTop, COORD xRight, COORD yBottom)
{
    Line(xLeft, yTop, xRight, yTop);
    Line(xRight, yTop, xRight, yBottom);
    Line(xRight, yBottom, xLeft, yBottom);
    Line(xLeft, yBottom, xLeft, yTop);
}

VOID EQS_API RoundRectangle(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom,
                            INT16 nRadius)
{
    Line((COORD)(xLeft+nRadius), yTop, (COORD)(xRight-nRadius), yTop);
    sQuadrantCircle((COORD)(xLeft+nRadius), (COORD)(yTop+nRadius), nRadius, 2);
    Line(xRight, (COORD)(yTop+nRadius), xRight, (COORD)(yBottom-nRadius));
    sQuadrantCircle((COORD)(xRight-nRadius), (COORD)(yTop+nRadius), nRadius, 1);
    Line((COORD)(xRight-nRadius), yBottom, (COORD)(xLeft+nRadius), yBottom);
    sQuadrantCircle((COORD)(xRight-nRadius), (COORD)(yBottom-nRadius), nRadius, 4);
    Line(xLeft, (COORD)(yBottom-nRadius), xLeft, (COORD)(yTop+nRadius));
    sQuadrantCircle((COORD)(xLeft+nRadius), (COORD)(yBottom-nRadius), nRadius, 3);
}

LOCAL VOID EQS_API sPlotQuadrantCirclePoints(COORD xCenter, COORD yCenter, 
                                             COORD x, COORD y, INT8 nQuadrant)
{
    COLOR Color;

    Color = GET_FORE_COLOR();
    if(nQuadrant == 1)          /* the first quadrant */
    {
        //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter - y), Color);
        DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter - y), Color);
        //DevSetPixel((COORD)(xCenter + y), (COORD)(yCenter - x), Color);
        DEV_SET_PIXEL((COORD)(xCenter + y), (COORD)(yCenter - x), Color);
    }
    else if(nQuadrant == 2)     /* the second quadrant */
    {
        //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter - y), Color);
        DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter - y), Color);
        //DevSetPixel((COORD)(xCenter - y), (COORD)(yCenter - x), Color);
        DEV_SET_PIXEL((COORD)(xCenter - y), (COORD)(yCenter - x), Color);
    }
    else if(nQuadrant == 3)     /* the third quadrant */
    {
        //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter + y), Color);
        DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter + y), Color);
        //DevSetPixel((COORD)(xCenter - y), (COORD)(yCenter + x), Color);
        DEV_SET_PIXEL((COORD)(xCenter - y), (COORD)(yCenter + x), Color);
    }
    else if(nQuadrant == 4)     /* the fourth quadrant */
    {
        //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter + y), Color);
        DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter + y), Color);
        //DevSetPixel((COORD)(xCenter + y), (COORD)(yCenter + x), Color);
        DEV_SET_PIXEL((COORD)(xCenter + y), (COORD)(yCenter + x), Color);
    }
}

LOCAL VOID EQS_API sQuadrantCircle(COORD xCenter, COORD yCenter, 
                                   INT16 nRadius, INT8 nQuadrant)
{
    COORD x, y;
    INT16 nPos;

    xCenter = LPX_2_DPX(xCenter);
    yCenter = LPy2DPy(yCenter);

    x = 0, y = nRadius, nPos = 1 - nRadius;

    sPlotQuadrantCirclePoints(xCenter, yCenter, x, y, nQuadrant);

    while(x < y)
    {
        x++;
        if(nPos < 0)
            nPos += 2 * x + 1;
        else
            y--, nPos += 2 * (x - y) + 1;

        sPlotQuadrantCirclePoints(xCenter, yCenter, x, y, nQuadrant);
    }
}

LOCAL VOID EQS_API sPlotCirclePoints(COORD xCenter, COORD yCenter, COORD x, COORD y)
{
    COLOR Color;

    Color = GET_FORE_COLOR();
    //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter + y), Color);
    DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter + y), Color);
    //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter + y), Color);
    DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter + y), Color);
    //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter - y), Color);
    DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter - y), Color);
    //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter - y), Color);
    DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter - y), Color);
    //DevSetPixel((COORD)(xCenter + y), (COORD)(yCenter + x), Color);
    DEV_SET_PIXEL((COORD)(xCenter + y), (COORD)(yCenter + x), Color);
    //DevSetPixel((COORD)(xCenter - y), (COORD)(yCenter + x), Color);
    DEV_SET_PIXEL((COORD)(xCenter - y), (COORD)(yCenter + x), Color);
    //DevSetPixel((COORD)(xCenter + y), (COORD)(yCenter - x), Color);
    DEV_SET_PIXEL((COORD)(xCenter + y), (COORD)(yCenter - x), Color);
    //DevSetPixel((COORD)(xCenter - y), (COORD)(yCenter - x), Color);
    DEV_SET_PIXEL((COORD)(xCenter - y), (COORD)(yCenter - x), Color);
}

VOID EQS_API Circle(COORD xCenter, COORD yCenter, INT16 nRadius)
{
    COORD x, y;
    INT16 nPos;

    xCenter = LPX_2_DPX(xCenter);
    yCenter = LPy2DPy(yCenter);

    x = 0, y = nRadius, nPos = 1 - nRadius;

    sPlotCirclePoints(xCenter, yCenter, x, y);

    while(x < y)
    {
        x++;
        if(nPos < 0)
            nPos += 2 * x + 1;
        else
            y--, nPos += 2 * (x - y) + 1;

        sPlotCirclePoints(xCenter, yCenter, x, y);
    }
}

LOCAL VOID EQS_API sPlotEllipsePoints(COORD xCenter, COORD yCenter, 
                                     COORD x, COORD y)
{
    //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter + y), GET_FORE_COLOR()); 
    DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter + y), GET_FORE_COLOR()); 
    //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter + y), GET_FORE_COLOR()); 
    DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter + y), GET_FORE_COLOR()); 
    //DevSetPixel((COORD)(xCenter + x), (COORD)(yCenter - y), GET_FORE_COLOR()); 
    DEV_SET_PIXEL((COORD)(xCenter + x), (COORD)(yCenter - y), GET_FORE_COLOR()); 
    //DevSetPixel((COORD)(xCenter - x), (COORD)(yCenter - y), GET_FORE_COLOR()); 
    DEV_SET_PIXEL((COORD)(xCenter - x), (COORD)(yCenter - y), GET_FORE_COLOR()); 
}

VOID EQS_API Ellipse(COORD xCenter, COORD yCenter, COORD xRadius, COORD yRadius)
{
    COORD xRadiusSquare, yRadiusSquare;
    COORD xRadiusSquare2, yRadiusSquare2;
    INT16 nPos;
    COORD x, y;
    COORD xPos, yPos;

    xCenter = LPX_2_DPX(xCenter);
    yCenter = LPy2DPy(yCenter);
    
    xRadiusSquare = xRadius * xRadius;
    yRadiusSquare = yRadius * yRadius;

    xRadiusSquare2 = xRadiusSquare * 2;
    yRadiusSquare2 = yRadiusSquare * 2;

    x = 0, y = yRadius;
    xPos = 0, yPos = xRadiusSquare2 * y;

    /* Plot the first set of points */
    sPlotEllipsePoints(xCenter, yCenter, x, y);

    /* Region 1 */
    nPos = ROUND(yRadiusSquare - (xRadiusSquare * yRadius) 
                    + (0.25 * xRadiusSquare));

    while(xPos < yPos) 
    {
        x++;
        xPos += yRadiusSquare2;
        if(nPos < 0)
            nPos += yRadiusSquare + xPos;
        else 
            y--, yPos -= xRadiusSquare2, nPos += yRadiusSquare + xPos - yPos;

        sPlotEllipsePoints(xCenter, yCenter, x, y);
    }

    /* Region 2 */
    nPos = ROUND(yRadiusSquare * (x + 0.5) * (x + 0.5)
                    + xRadiusSquare * (y - 1) * (y - 1) 
                    - xRadiusSquare * yRadiusSquare);

    while(y > 0)
    {
        y--;
        yPos -= xRadiusSquare2;

        if(nPos > 0)
            nPos += xRadiusSquare - yPos;
        else
            x++, xPos += yRadiusSquare2, nPos += xRadiusSquare - yPos + xPos;

        sPlotEllipsePoints(xCenter, yCenter, x, y);
    }
}

/* draws a set of line segments */
/* pPoint : pointer to array of points */
/* wCount: count of points */
VOID EQS_API Polyline(POINT* pPoint, WORD wCount)
{
    WORD wIndex;
    
    MoveTo(pPoint[0].x, pPoint[0].y);

    for(wIndex = 1; wIndex < wCount; wIndex++)
    {
        LineTo(pPoint[wIndex].x, pPoint[wIndex].y);
    }

    LineTo(pPoint[0].x, pPoint[0].y);
}

VOID EQS_API FillRectangle(COORD xLeft, COORD yTop, COORD xRight, 
                          COORD yBottom, COLOR FillColor)
{
    COORD y;
    COLOR OldColor;

    OldColor = GET_FORE_COLOR();
    SET_FORE_COLOR(FillColor);

    for(y = yTop; y <= yBottom; y++)
        Line(xLeft, y, xRight, y);

    SET_FORE_COLOR(OldColor);
//    Rectangle(xLeft, yTop, xRight, yBottom);
}

#if 0
LOCAL VOID EQS_API sFillQuadrantCircle(COORD xCenter, COORD yCenter, 
                                   COLOR FillColor, INT16 nRadius, INT8 nQuadrant)
{
    COORD x, y;
    INT16 nPos;
 
	COLOR OldColor;
    OldColor = GET_FORE_COLOR();
    SET_FORE_COLOR(FillColor);

    xCenter = LPX_2_DPX(xCenter);
    yCenter = LPy2DPy(yCenter);

    x = 0, y = nRadius, nPos = 1 - nRadius;
	
	switch(nQuadrant) {
		case 1:
			Line(xCenter, yCenter-y, xCenter+x, yCenter-y);
			Line(xCenter, yCenter-x, xCenter+y, yCenter-x);
			break;
		
		case 2:
			Line(xCenter, yCenter-y, xCenter-x, yCenter-y);
			Line(xCenter, yCenter-x, xCenter-y, yCenter-x);
			break;
		case 3:
			Line(xCenter, yCenter+y, xCenter-x, yCenter+y);
			Line(xCenter, yCenter+x, xCenter-y, yCenter+x);
			break;
		case 4:
			Line(xCenter, yCenter+y, xCenter+x, yCenter+y);
			Line(xCenter, yCenter+x, xCenter+y, yCenter+x);
			break;

	}
    while(x < y)
    {
        x++;
        if(nPos < 0)
            nPos += 2 * x + 1;
        else
            y--, nPos += 2 * (x - y) + 1;
		switch(nQuadrant){
			case 1:
				Line(xCenter, yCenter-y, xCenter+x, yCenter-y);
				Line(xCenter, yCenter-x, xCenter+y, yCenter-x);
				break;
			case 2:
				Line(xCenter, yCenter-y, xCenter-x, yCenter-y);
				Line(xCenter, yCenter-x, xCenter-y, yCenter-x);
				break;
			case 3:
				Line(xCenter, yCenter+y, xCenter-x, yCenter+y);
				Line(xCenter, yCenter+x, xCenter-y, yCenter+x);
				break;
			case 4:
				Line(xCenter, yCenter+y, xCenter+x, yCenter+y);
				Line(xCenter, yCenter+x, xCenter+y, yCenter+x);
				break;
		}
	}
	SET_FORE_COLOR(OldColor);
}

VOID EQS_API RoundFillRectangle(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom,
                            COLOR FillColor, INT16 nRadius)
{
    /*******************************************************
     다섯개의 사각형과 4개의 부채꼴을 찍어 준다. 
     
	      *****************
         **   *****************  **

        ***   *****************  ***
        ***   *****************  *** 
        ***   *****************  ***
        ***   *****************  *** 

         **   *****************  **
              *****************
		
    *********************************************************/
    FillRectangle((COORD)(xLeft+nRadius)        , yTop                     ,
		                 (COORD)(xRight-nRadius), (COORD)(yTop+nRadius)     , FillColor);
    FillRectangle((COORD)(xRight-nRadius)       , (COORD)(yTop+nRadius)    , 
		                  xRight                , (COORD)(yBottom-nRadius)  , FillColor);
    FillRectangle((COORD)(xLeft+nRadius)        , (COORD)(yBottom-nRadius) ,
		                 (COORD)(xRight-nRadius),  yBottom                  , FillColor);
    FillRectangle((COORD)(xLeft)                , (COORD)(yTop+nRadius)    ,
		                 (COORD)(xLeft+nRadius) , ( COORD)(yBottom-nRadius) , FillColor);
    FillRectangle((COORD)(xLeft+nRadius)        , (COORD)(yTop+nRadius)    , 
				         (COORD)(xRight-nRadius) , (COORD)(yBottom-nRadius) , FillColor);
	sFillQuadrantCircle((COORD)(xLeft+nRadius), (COORD)(yTop+nRadius), FillColor, nRadius, 2);
    sFillQuadrantCircle((COORD)(xRight-nRadius), (COORD)(yTop+nRadius), FillColor, nRadius, 1);
    sFillQuadrantCircle((COORD)(xRight-nRadius), (COORD)(yBottom-nRadius), FillColor,  nRadius, 4);
    sFillQuadrantCircle((COORD)(xLeft+nRadius), (COORD)(yBottom-nRadius), FillColor, nRadius, 3);
}

VOID     EQS_API FillCircle(COORD xCenter, COORD yCenter, 
                            INT16 nRadius, COLOR FillColor) 
{
    COORD x, y;
    INT16 nPos;
    
	COLOR OldColor;
    OldColor = GET_FORE_COLOR();
    SET_FORE_COLOR(FillColor);

    xCenter = LPX_2_DPX(xCenter);
    yCenter = LPy2DPy(yCenter);

    x = 0, y = nRadius, nPos = 1 - nRadius;

    sPlotCirclePoints(xCenter, yCenter, x, y);
	Line(xCenter, yCenter-y,xCenter, yCenter+y);
    while(x < y)
    {
        x++;
        if(nPos < 0)
            nPos += 2 * x + 1;
        else
            y--, nPos += 2 * (x - y) + 1;

		Line((COORD)(xCenter + x), (COORD)(yCenter + y),
			 (COORD)(xCenter + x), (COORD)(yCenter - y));
		Line((COORD)(xCenter + y), (COORD)(yCenter + x),
			 (COORD)(xCenter + y), (COORD)(yCenter - x));
		Line((COORD)(xCenter - x), (COORD)(yCenter + y),
			 (COORD)(xCenter - x), (COORD)(yCenter - y));
		Line((COORD)(xCenter - y), (COORD)(yCenter + x),
			 (COORD)(xCenter - y), (COORD)(yCenter - x));
	}
	SET_FORE_COLOR(OldColor);
}

#endif // if 0

#ifdef FEATURE_GRAPH_ENHANCED
VOID     EQS_API Arc(VOID) {}
VOID     EQS_API ArcTo(VOID) {}
VOID     EQS_API Pie(VOID) {}
VOID     EQS_API Sector(VOID) {}
VOID     EQS_API SetFillStyle(VOID) {}
VOID     EQS_API SetFillPattern(VOID) {}
VOID     EQS_API FillCircle(COORD xCenter, COORD yCenter, 
                            INT16 nRadius, COLOR FillColor) {}
VOID     EQS_API FillEllipse(VOID) {}
VOID     EQS_API FillPoly(VOID) {}
#endif /* FEATURE_GRAPH_ENHANCED */

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Draw bitmap image                                                       */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

#if defined(FEATURE_EQS_COLOR_64K) || defined(FEATURE_EQS_COLOR_256)

#define MAX_REPEAT_COUNT        64
#define REPEAT_FLAG             0xC0

LOCAL WORD sUnpackLine64K(CONST BYTE* pbSource, BYTE* pbDest, WORD wLineByte)
{
    WORD wCountInLine;
    WORD wReadByte;
    //UINT8 nRepeatCount;
    BYTE bKey;
	WORD wReadData;
	WORD* pDest;

    wCountInLine = 0;
    wReadByte = 0;

	pDest = (WORD*) pbDest;

    do 
    {
        /* get a key byte */
        bKey = pbSource[wReadByte++];
		if( bKey > 128 )
			bKey = 128;
		wReadData = pbSource[wReadByte];
		wReadData <<= 8;
		wReadData += pbSource[wReadByte+1];
		wReadByte+=2;
		while( bKey > 0 )
		{
			pDest[wCountInLine++] = wReadData;
			bKey--;
		}
    } while(wCountInLine*2 < wLineByte);
    return wReadByte;
}


LOCAL WORD sUnpackLine(CONST BYTE* pbSource, BYTE* pbDest, WORD wLineByte)
{
    WORD wCountInLine;
    WORD wReadByte;
    UINT8 nRepeatCount;
    BYTE bKey;
	//WORD wReadData;
	

    wCountInLine = 0;
    wReadByte = 0;

    do 
    {
        /* get a key byte */
        bKey = pbSource[wReadByte++];
        /* if it's a run of bytes field */
        if((bKey & REPEAT_FLAG) == REPEAT_FLAG)
        {
            /* and off the high bits */
            nRepeatCount = bKey & ~REPEAT_FLAG;
            /* get the run byte */
            bKey = pbSource[wReadByte++];
            /* run the byte */
            while(nRepeatCount--)
            {
                /* 
                ** It is an error condition, but
                ** to prevent from crashing the system
                */
                if(wCountInLine == wLineByte)
                    break;
                pbDest[wCountInLine++] = ~bKey;
            }
        }
        else /* just store it */
        {
            pbDest[wCountInLine++] = ~bKey;
        }
    } while(wCountInLine < wLineByte);
    return wReadByte;
}

#endif /* defined(FEATURE_EQS_COLOR_64K) || defined(FEATURE_EQS_COLOR_256) */

#define MAX_SCREEN_WIDTH		128

#ifdef FEATURE_EQS_COLOR_64K

LOCAL VOID s64KC_PutBitmap64KC(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    UINT16 nByteWidth;
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH*BIT_PER_PIXEL_64KC/8];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */
#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;
#ifdef WIN32
	BYTE c_data[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
	BYTE ab_Frame[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
#else
	BYTE* c_data = NULL;
	BYTE* ab_Frame = NULL;

	AddressAssign();

	c_data = SeekWorkingArea(COMP_IMG);
	ab_Frame = SeekWorkingArea(DECOMP_IMG);
#endif  //WIN32

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    nByteWidth = pBitmap->Header.Size.xWidth<<1;//(pBitmap->Header.Type == BT_FONT) 
    //          ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    yHeight = pBitmap->Header.Size.yHeight;
    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.yStart; yOffset++)
		    nPackedData += sUnpackLine64K(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, nByteWidth);
		//pbImageData = abUnpackedData;
    }

    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
		if(fPackedData)
        {
			nPackedData += sUnpackLine64K(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, nByteWidth);
			pbImageData = abUnpackedData;
        }
#ifdef FEATURE_USE_LZB		    
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (nByteWidth)*(pBitmap->Header.yStart+yOffset);
		}
#endif
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (nByteWidth)*(pBitmap->Header.yStart+yOffset);
        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + pBitmap->Header.xWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : pBitmap->Header.xWidth;
        //if(l_TransparentColor == MAX_COLOR) /* no transparent color */
        if(!sIS_TRANSPARENT_COLOR(pBitmap->Header))//.TransparentColor == MAX_COLOR)
        {
            memcpy(pbScreenData, pbImageData, xWidth<<1);//pBitmap->Header.xWidth);
        }
        else
        {
            for(xOffset = 0; xOffset < xWidth; xOffset++)
            {
                //if(l_TransparentColor == *pbImageData) /* transparent color */
                if(pBitmap->Header.TransparentColor == (((*(pbImageData+1))<<8)|*pbImageData))
                {
                    if(GetDrawMode() == DM_OR)
                    {
                        pbScreenData+=2, pbImageData+=2;
                    }
                    else /* DM_COPY */
                    {
                        *pbScreenData++ = BackColor;
                        *pbScreenData++ = BackColor;
                        pbImageData+=2;
                    }
                }
                else
                {
                    *pbScreenData++ = *pbImageData++;
                    *pbScreenData++ = *pbImageData++;
                }
            }
        }
    }
}


LOCAL VOID s64KC_PutBitmapRect64KC(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
    COORD xWidth, yHeight, ImageWidth;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    UINT16 nByteWidth;
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH*BIT_PER_PIXEL_64KC/8];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */

#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;
#ifdef WIN32
	BYTE c_data[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
	BYTE ab_Frame[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
#else
	BYTE* c_data = NULL;
	BYTE* ab_Frame = NULL;

	AddressAssign();

	c_data = SeekWorkingArea(COMP_IMG);
	ab_Frame = SeekWorkingArea(DECOMP_IMG);
#endif  //WIN32

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    nByteWidth = pBitmap->Header.Size.xWidth<<1;//(pBitmap->Header.Type == BT_FONT) 
    //          ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
	ImageWidth = (Rect.xRight - Rect.xLeft +1);
	yHeight    = Rect.yBottom - Rect.yTop + 1;
	if( yHeight + Rect.yTop -1 > pBitmap->Header.Size.yHeight )
	{
		yHeight = pBitmap->Header.Size.yHeight - Rect.yTop ;
	}

    //yHeight = pBitmap->Header.Size.yHeight;
    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < Rect.yTop; yOffset++)
		    nPackedData += sUnpackLine64K(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, nByteWidth);
		//nPackedData += Rect.xLeft;
		//pbImageData = abUnpackedData + Rect.xLeft * 2;
    }

    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
		if(fPackedData)
        {
			nPackedData += sUnpackLine64K(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, nByteWidth);
			//nPackedData += Rect.xLeft;
			pbImageData = abUnpackedData + Rect.xLeft *2;
        }
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.06	    
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (nByteWidth)*(Rect.yTop+yOffset);
            pbImageData += Rect.xLeft*2;
		}
#endif		
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (nByteWidth)*(Rect.yTop+yOffset);
            pbImageData += Rect.xLeft*2;
        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + ImageWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : ImageWidth;//pBitmap->Header.xWidth;
        //if(l_TransparentColor == MAX_COLOR) /* no transparent color */
        if(!sIS_TRANSPARENT_COLOR(pBitmap->Header))//.TransparentColor == MAX_COLOR)
        {
            memcpy(pbScreenData, pbImageData, xWidth<<1);//pBitmap->Header.xWidth);
        }
        else
        {
            for(xOffset = 0; xOffset < xWidth; xOffset++)
            {
                //if(l_TransparentColor == *pbImageData) /* transparent color */
                if(pBitmap->Header.TransparentColor == (((*(pbImageData+1))<<8)|*pbImageData))
                {
                    if(GetDrawMode() == DM_OR)
                    {
                        pbScreenData+=2, pbImageData+=2;
                    }
                    else /* DM_COPY */
                    {
                        *pbScreenData++ = BackColor;
                        *pbScreenData++ = BackColor;
                        pbImageData+=2;
                    }
                }
                else
                {
                    *pbScreenData++ = *pbImageData++;
                    *pbScreenData++ = *pbImageData++;
                }
            }
        }
    }
}

VOID PutBitmapRect64KC(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
	s64KC_PutBitmapRect64KC(x,y,pBitmap,Rect);
}

LOCAL VOID EQS_API s64KC_PutBitmap256C(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    //UINT16 nByteWidth;
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */

#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;
#ifdef WIN32
	BYTE c_data[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
	BYTE ab_Frame[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
#else
	BYTE* c_data = NULL;
	BYTE* ab_Frame = NULL;

	AddressAssign();

	c_data = SeekWorkingArea(COMP_IMG);
	ab_Frame = SeekWorkingArea(DECOMP_IMG);
#endif  //WIN32

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    //xWidth = (pBitmap->Header.Type == BT_FONT) 
    //          ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    yHeight = pBitmap->Header.Size.yHeight;
    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.yStart; yOffset++)
		    nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, pBitmap->Header.Size.xWidth);
		//pbImageData = abUnpackedData;
    }

    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
         pbScreenData = (sGETGD().Screen.pbBuffer + nBytePos);
		if(fPackedData)
        {
			nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, pBitmap->Header.Size.xWidth);
			pbImageData = abUnpackedData;
        }
#ifdef FEATURE_USE_LZB	// NKHA 2001.11.06	    
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth)*(pBitmap->Header.yStart+yOffset);
	        //pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth>>2)*yOffset;
		}
#endif
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth)*(pBitmap->Header.yStart+yOffset);
        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + pBitmap->Header.xWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : pBitmap->Header.xWidth;
        for(xOffset = 0; xOffset < xWidth; xOffset++)
        {
            //if(l_TransparentColor == *pbImageData) /* transparent color */
            if(pBitmap->Header.TransparentColor == *pbImageData)
            {
                if(GetDrawMode() == DM_OR)
                {
                    pbScreenData+=2, pbImageData++;
                }
                else /* DM_COPY */
                {
                    *pbScreenData++ = LOBYTE(BackColor);
                    *pbScreenData++ = HIBYTE(BackColor);
                    pbImageData++;
                }
            }
            else
            {
                BYTE bR, bG, bB;
                WORD wColor;

                bR = ((*pbImageData >> 5) << 5) + 31;
                bG = (((*pbImageData >> 2) & 0x07) << 5) + 31;
                bB = ((*pbImageData & 0x03) << 6) + 63;

				wColor = RGB565(bR, bG, bB);
            
                *pbScreenData++ = LOBYTE(wColor);
                *pbScreenData++ = HIBYTE(wColor);
                
                pbImageData++;
            }
        }
    }
}

LOCAL VOID EQS_API s64KC_PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
    COORD xWidth, yHeight, ImageWidth;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    //UINT16 nByteWidth;
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */

#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;
#ifdef WIN32
	BYTE c_data[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
	BYTE ab_Frame[MAX_WIDTH*MAX_VIEW_HEIGHT*MAX_BPP/8];
#else
	BYTE* c_data = NULL;
	BYTE* ab_Frame = NULL;

	AddressAssign();

	c_data = SeekWorkingArea(COMP_IMG);
	ab_Frame = SeekWorkingArea(DECOMP_IMG);
#endif  //WIN32

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */


    //xWidth = (pBitmap->Header.Type == BT_FONT) 
    //          ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    //yHeight = pBitmap->Header.Size.yHeight;
    yHeight    = Rect.yBottom - Rect.yTop + 1;
	ImageWidth = Rect.xRight - Rect.xLeft +1;

    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < Rect.yTop; yOffset++)
		    nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, ImageWidth);
		//pbImageData = abUnpackedData;
		//nPackedData += Rect.xLeft;	
    }

    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
		if(fPackedData)
        {
			nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, ImageWidth);
			//nPackedData += Rect.xLeft;
			pbImageData = abUnpackedData;
        }
#ifdef FEATURE_USE_LZB // NKHA 2001.11.06
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth)*(Rect.yTop+yOffset) + Rect.xLeft;
	        //pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth>>2)*yOffset;
		}
#endif
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth)*(Rect.yTop+yOffset) + Rect.xLeft;

        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + ImageWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : ImageWidth;
        for(xOffset = 0; xOffset < xWidth; xOffset++)
        {
            //if(l_TransparentColor == *pbImageData) /* transparent color */
            if(pBitmap->Header.TransparentColor == *pbImageData)
            {
                if(GetDrawMode() == DM_OR)
                {
                    pbScreenData+=2, pbImageData++;
                }
                else /* DM_COPY */
                {
                    *pbScreenData++ = LOBYTE(BackColor);
                    *pbScreenData++ = HIBYTE(BackColor);
                    pbImageData++;
                }
            }
            else
            {
                BYTE bR, bG, bB;
                WORD wColor;

                bR = ((*pbImageData >> 5) << 5) + 31;
                bG = (((*pbImageData >> 2) & 0x07) << 5) + 31;
                bB = ((*pbImageData & 0x03) << 6) + 63;

				wColor = RGB565(bR, bG, bB);
            
                *pbScreenData++ = LOBYTE(wColor);
                *pbScreenData++ = HIBYTE(wColor);
                
                pbImageData++;
            }
        }
    }
}

#define PALETTE_SIZE_BW                 2
#define PALETTE_SIZE_4GRAY              4
#define PALETTE_SIZE_16COLOR            16
#define PALETTE_SIZE_256COLOR           256

LOCAL VOID EQS_API s64KC_PutBitmap4G(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;  /* start pointer of each line of bitmap data */
    BYTE bImageData;
    INT8 i, j;
    ROM BYTE ab4GrayMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    ROM INT8 anShift[] = { 6, 4, 2, 0 };
    /*ROM */COLOR a4GrayColor[] = { BLACK, GRAY, LIGHTGRAY, WHITE };//{ BLACK, GRAY, LIGHTGRAY, WHITE };
    UINT8 nIndex;

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    if(pBitmap->Header.fPalette)
    {
        memcpy(a4GrayColor, pBitmap->pbPalette, sizeof(COLOR)*PALETTE_SIZE_4GRAY);
    }

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;
    xWidth <<= 1;

    yHeight = pBitmap->Header.Size.yHeight;

    /* -------------------------
    ** draw mode (OR, COPY, XOR)
    **
    ** followng code looks like redundant but, it is for better speed.
    ** I just use the some redundant code  
    ** instead of the if statement inside the loop and function calls.
    */
    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
        nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

        pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*(pBitmap->Header.yStart+yOffset);//yOffset;

        for(i = 0; (i<<3) < xWidth; i++)
        {
            /* image data */
            bImageData = *(pbImageData + i);
            for(j = 0; ((i<<3)+(j<<1)) < xWidth && j < 4; j++)
            {
                nIndex = (bImageData & ab4GrayMask[j]) >> anShift[j];
                //if(sIS_TRANSPARENT_COLOR(pBitmap->Header) && (nIndex == pBitmap->Header.TransparentColor))
                if(pBitmap->Header.TransparentColor != MAX_COLOR_4 && (nIndex == pBitmap->Header.TransparentColor)) /* no transparent color */
                //if(sIS_TRANSPARENT_COLOR(pBitmap->Header) && (nIndex == pBitmap->Header.TransparentColor))
                {
                    pbScreenData++;
                    pbScreenData++;
                }
                else
                {
                    *(pbScreenData++) = LOBYTE(a4GrayColor[nIndex]);
                    *(pbScreenData++) = HIBYTE(a4GrayColor[nIndex]);
                }
            }
        }
    }
}

LOCAL VOID EQS_API s64KC_PutBitmapBW(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD yOffset, yStart;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;
    BYTE bImageData;
    INT8 i, j;
    ROM BYTE abBWMask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    ROM INT8 anShift[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR ForeColor;// = (l_fPutFont) ? (COLOR)GET_TEXT_COLOR() : (COLOR)GET_FORE_COLOR();
    COLOR BackColor;// = (l_fPutFont) ? (COLOR)GET_TEXT_BACK_COLOR() : (COLOR)GET_BACK_COLOR();
#endif /* FEATURE_EQS_FONT_COLOR */
    WORD wData;

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_64KC);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        ForeColor = (COLOR)GET_TEXT_COLOR();
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else if(!pBitmap->Header.fPalette)
    {
        ForeColor = (COLOR)GET_FORE_COLOR();
        BackColor = (COLOR)GET_BACK_COLOR();
    }
    else
#endif /* FEATURE_EQS_FONT_COLOR */
    {
        ForeColor = (COLOR)pBitmap->pbPalette[0];
        BackColor = (COLOR)pBitmap->pbPalette[1];
    }
    

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

    yStart = (l_fPutFont) ? 0 : pBitmap->Header.yStart;

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;
    yHeight = pBitmap->Header.Size.yHeight;

    /* -------------------------
    ** draw mode (OR, COPY, XOR)
    **
    ** followng code looks like redundant but, it is for better speed.
    ** I just use the some redundant code  
    ** instead of the if statement inside the loop and function calls.
    */
    if(GetDrawMode() == DM_COPY)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
                {
                    wData = ((bImageData & abBWMask[j]) >> anShift[j]);
#ifdef FEATURE_EQS_FONT_COLOR
                    *(pbScreenData++) = LOBYTE(wData ? ForeColor : BackColor);//0;
                    *(pbScreenData++) = HIBYTE(wData ? ForeColor : BackColor);//0;

#else
                    *(pbScreenData++) = LOBYTE(wData);
                    *(pbScreenData++) = HIBYTE(wData);
#endif
                }
            }
        }
    }
    else if(GetDrawMode() == DM_OR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
                {
                    if(((bImageData & abBWMask[j]) >> anShift[j]))
                    {
                        *(pbScreenData++) = LOBYTE(ForeColor);
                        *(pbScreenData++) = HIBYTE(ForeColor);
                    }
                    else
                    {
                        pbScreenData+=2;
                    }
                }
            }
        }
    }
    else if(GetDrawMode() == DM_XOR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_64KC(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
                {
#ifdef FEATURE_EQS_FONT_COLOR
                    *(pbScreenData++) ^= LOBYTE(((bImageData & abBWMask[j]) >> anShift[j]) ? ForeColor : BackColor);//0;
                    *(pbScreenData++) ^= HIBYTE(((bImageData & abBWMask[j]) >> anShift[j]) ? ForeColor : BackColor);//0;
#else
                    *(pbScreenData++) ^= LOBYTE(((bImageData & abBWMask[j]) >> anShift[j]));
                    *(pbScreenData++) ^= HIBYTE(((bImageData & abBWMask[j]) >> anShift[j]));
#endif
                }
            }
        }
    }
}

LOCAL VOID EQS_API s64KC_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    switch(pBitmap->Header.nBitsPerPixel)
    {
    case BIT_PER_PIXEL_64KC :
        s64KC_PutBitmap64KC(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_256C :
        s64KC_PutBitmap256C(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_4G :
        s64KC_PutBitmap4G(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_BW :
        s64KC_PutBitmapBW(x, y, pBitmap);
        break;
    }
}

LOCAL VOID EQS_API s64KC_PutBitmapRect(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
    switch(pBitmap->Header.nBitsPerPixel)
    {
    case BIT_PER_PIXEL_64KC :
        s64KC_PutBitmapRect64KC(x, y, pBitmap, Rect);		
        break;
    case BIT_PER_PIXEL_256C :
        s64KC_PutBitmapRect256C(x, y, pBitmap, Rect);
        break;
    case BIT_PER_PIXEL_4G :
        //s64KC_PutBitmap4G(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_BW :
        //s64KC_PutBitmapBW(x, y, pBitmap);
        break;
    }
}

#endif //#ifdef FEATURE_EQS_COLOR_64K

/* set palette color into the 4gray image. only in 256 color display system */
void SetPaletteColor4G(BITMAP *pBitmap, CONST COLOR *pColor)
{
    pBitmap->pbPalette = pColor;
}

#ifdef FEATURE_EQS_COLOR_256

LOCAL VOID s256C_PutBitmap256C(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */

#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    //xWidth = (pBitmap->Header.Type == BT_FONT) 
    //          ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    yHeight = pBitmap->Header.Size.yHeight;
    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.yStart; yOffset++)
		    nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, pBitmap->Header.Size.xWidth);
		//pbImageData = abUnpackedData;
    }

    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
		if(fPackedData)
        {
			nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, pBitmap->Header.Size.xWidth);
			pbImageData = abUnpackedData;
        }
#ifdef FEATURE_USE_LZB // NKHA 2001.11.06
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth)*(pBitmap->Header.yStart+yOffset);
	        //pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth>>2)*yOffset;
		}
#endif
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth)*(pBitmap->Header.yStart+yOffset);
        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + pBitmap->Header.xWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : pBitmap->Header.xWidth;
        //if(l_TransparentColor == MAX_COLOR) /* no transparent color */
        if(pBitmap->Header.TransparentColor == MAX_COLOR_256)
        {
            memcpy(pbScreenData, pbImageData, xWidth);//pBitmap->Header.xWidth);
        }
        else
        {
            for(xOffset = 0; xOffset < xWidth; xOffset++)
            {
                //if(l_TransparentColor == *pbImageData) /* transparent color */
                if(pBitmap->Header.TransparentColor == *pbImageData)
                {
                    if(GetDrawMode() == DM_OR)
                    {
                        pbScreenData++, pbImageData++;
                    }
                    else /* DM_COPY */
                    {
                        *pbScreenData++ = BackColor, pbImageData++;
                    }
                }
                else
                {
                    *pbScreenData++ = *pbImageData++;
                }
            }
        }
    }
}

LOCAL VOID s256C_PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

VOID PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
	s256C_PutBitmapRect256C(x,y,pBitmap,Rect);
}

LOCAL VOID s256C_PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{
    COORD xWidth, yHeight, ImageWidth;
    COORD xOffset, yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;
	BOOL fPackedData;
	BYTE abUnpackedData[MAX_SCREEN_WIDTH];
	UINT16 nPackedData;
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR BackColor;
#endif /* FEATURE_EQS_FONT_COLOR */

#ifdef FEATURE_USE_LZB
    /* LZB의 경우 풀어서 pbData를 ab_Frame에 저장한다. haengeun  2001-09-27 --> */
	WORD wFrameSize;
	BOOL fLZBPacked = FALSE ;

	if(IS_EQUS_Z_IMAGE(&(pBitmap->Header)) 
       || IS_EQUS_Z_ANIMATION(&(pBitmap->Header)))
	{  
	  fLZBPacked = TRUE;
	  wFrameSize = (*(pBitmap->pbData) << 8) + (*(pBitmap->pbData+1));
	  memcpy(c_data,(pBitmap->pbData+2),wFrameSize);	  
	  LZBDecompress((DWORD *)c_data, ab_Frame);  
    }	        
	/* <-- */
#endif

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */


    yHeight    = Rect.yBottom - Rect.yTop + 1;
	ImageWidth = Rect.xRight - Rect.xLeft +1;
    fPackedData = (IS_EQUS_X_IMAGE(&(pBitmap->Header)) || IS_EQUS_X_ANIMATION(&(pBitmap->Header))) ? TRUE : FALSE;
    
	nPackedData = 0;
    /* wind up to yStart position */
	if(fPackedData)
    {
        for(yOffset = 0; yOffset < Rect.yTop; yOffset++)
		    nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, pBitmap->Header.Size.xWidth);
		//nPackedData += Rect.xLeft;		
    }


    for(yOffset = 0; yOffset < yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if(((y + yOffset) < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            continue; // 2001.05.18
#endif /* FEATURE_EQS_CLIPPING */

        nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
		if(fPackedData)
        {
			nPackedData += sUnpackLine(pBitmap->pbData + nPackedData + sizeof(WORD), abUnpackedData, ImageWidth);
			//nPackedData += Rect.xLeft;
			pbImageData = abUnpackedData;
        }
#ifdef FEATURE_USE_LZB // NKHA 2001.11.06
		else if(fLZBPacked)
		{   
			pbImageData = ab_Frame + (pBitmap->Header.Size.xWidth)*(Rect.yTop+yOffset) + Rect.xLeft;
		}
#endif
		else
        {
			pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth)*(Rect.yTop+yOffset) + Rect.xLeft;
        }

        /* offset is the negative value, do not draw */
        //if(yOffset < 0)
        //    continue;

        xWidth = ((x + ImageWidth) >= DEV_GET_MAX_WIDTH()) 
                 ? DEV_GET_MAX_WIDTH() - x : ImageWidth;
        //if(l_TransparentColor == MAX_COLOR) /* no transparent color */
        if(pBitmap->Header.TransparentColor == MAX_COLOR_256)
        {
            memcpy(pbScreenData, pbImageData, xWidth);//pBitmap->Header.xWidth);
        }
        else
        {
            for(xOffset = 0; xOffset < xWidth; xOffset++)
            {
                //if(l_TransparentColor == *pbImageData) /* transparent color */
                if(pBitmap->Header.TransparentColor == *pbImageData)
                {
                    if(GetDrawMode() == DM_OR)
                    {
                        pbScreenData++, pbImageData++;
                    }
                    else /* DM_COPY */
                    {
                        *pbScreenData++ = BackColor, pbImageData++;
                    }
                }
                else
                {
                    *pbScreenData++ = *pbImageData++;
                }
            }
        }
    }
}



//#define GET_PALETTE_POS(pBitmap) (pBitmap->Header.Size.xWidth/8*pBitmap->Header.Size.yHeight*pBitmap->Header.nBitsPerPixel)

#define PALETTE_SIZE_4GRAY              4
#define PALETTE_SIZE_16COLOR            16
#define PALETTE_SIZE_256COLOR           256


LOCAL VOID s256C_PutBitmap4G(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD yOffset;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;  /* start pointer of each line of bitmap data */
    BYTE bImageData;
    INT8 i, j;
    ROM BYTE ab4GrayMask[] = { 0xC0, 0x30, 0x0C, 0x03 };
    ROM INT8 anShift[] = { 6, 4, 2, 0 };
    /*ROM */COLOR a4GrayColor[] = { WHITE, LIGHTGRAY, GRAY, BLACK };//{ BLACK, GRAY, LIGHTGRAY, WHITE };

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C);

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

    if(pBitmap->Header.fPalette)
    {
        memcpy(a4GrayColor, pBitmap->pbPalette, sizeof(COLOR)*PALETTE_SIZE_4GRAY);
    }

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;
    xWidth <<= 1;

    yHeight = pBitmap->Header.Size.yHeight;

    /* -------------------------
    ** draw mode (OR, COPY, XOR)
    **
    ** followng code looks like redundant but, it is for better speed.
    ** I just use the some redundant code  
    ** instead of the if statement inside the loop and function calls.
    */
    if(GetDrawMode() == DM_COPY)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*(pBitmap->Header.yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+(j<<1)) < xWidth && j < 4; j++)
                    *(pbScreenData++) = a4GrayColor[(bImageData & ab4GrayMask[j]) >> anShift[j]];
            }
        }
    }
    else if(GetDrawMode() == DM_OR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*(pBitmap->Header.yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+(j<<1)) < xWidth && j < 4; j++)
                    *(pbScreenData++) |= a4GrayColor[(bImageData & ab4GrayMask[j]) >> anShift[j]];
            }
        }
    }
    else if(GetDrawMode() == DM_XOR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*(pBitmap->Header.yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+(j<<1)) < xWidth && j < 4; j++)
                    *(pbScreenData++) ^= a4GrayColor[(bImageData & ab4GrayMask[j]) >> anShift[j]];
            }
        }
    }
}

LOCAL VOID s256C_PutBitmapBW(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth, yHeight;
    COORD yOffset, yStart;
    UINT16 nBytePos; /* byte position at the screen position */
    BYTE* pbScreenData;
    BYTE* pbImageData;
    BYTE bImageData;
    INT8 i, j;
    ROM BYTE abBWMask[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    ROM INT8 anShift[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
#ifdef FEATURE_EQS_FONT_COLOR
    COLOR ForeColor;// = (l_fPutFont) ? (COLOR)GET_TEXT_COLOR() : (COLOR)GET_FORE_COLOR();
    COLOR BackColor;// = (l_fPutFont) ? (COLOR)GET_TEXT_BACK_COLOR() : (COLOR)GET_BACK_COLOR();
#endif /* FEATURE_EQS_FONT_COLOR */

    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_256C);

#ifdef FEATURE_EQS_FONT_COLOR
    if(l_fPutFont)
    {
        ForeColor = (COLOR)GET_TEXT_COLOR();
        BackColor = (COLOR)GET_TEXT_BACK_COLOR();
    }
    else
    {
        ForeColor = (COLOR)GET_FORE_COLOR();
        BackColor = (COLOR)GET_BACK_COLOR();
    }
#endif /* FEATURE_EQS_FONT_COLOR */

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

    yStart = (l_fPutFont) ? 0 : pBitmap->Header.yStart;

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;
    yHeight = pBitmap->Header.Size.yHeight;

    /* -------------------------
    ** draw mode (OR, COPY, XOR)
    **
    ** followng code looks like redundant but, it is for better speed.
    ** I just use the some redundant code  
    ** instead of the if statement inside the loop and function calls.
    */
    if(GetDrawMode() == DM_COPY)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
#ifdef FEATURE_EQS_FONT_COLOR
                    *(pbScreenData++) = ((bImageData & abBWMask[j]) >> anShift[j]) ? ForeColor : BackColor;//0;
#else
                    *(pbScreenData++) = ((bImageData & abBWMask[j]) >> anShift[j]);
#endif
            }
        }
    }
    else if(GetDrawMode() == DM_OR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
                {
                    if(/*TextColor == BLACK && */((bImageData & abBWMask[j]) >> anShift[j]))
                        *(pbScreenData++) = ForeColor;
                    else
                        pbScreenData++;
#ifdef FEATURE_EQS_FONT_COLOR
//                        *(pbScreenData++) |= ((bImageData & abBWMask[j]) >> anShift[j]) ? TextColor : 0;
#else
//                        *(pbScreenData++) |= ((bImageData & abBWMask[j]) >> anShift[j]);
#endif
                }
            }
        }
    }
    else if(GetDrawMode() == DM_XOR)
    {
        for(yOffset = 0; yOffset < yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            nBytePos = BYTE_POS_256C(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;

            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*(yStart+yOffset);//yOffset;

            for(i = 0; (i<<3) < xWidth; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);
                for(j = 0; ((i<<3)+j) < xWidth && j < 8; j++)
#ifdef FEATURE_EQS_FONT_COLOR
                    *(pbScreenData++) ^= ((bImageData & abBWMask[j]) >> anShift[j]) ? ForeColor : BackColor;//0;
#else
                    *(pbScreenData++) ^= ((bImageData & abBWMask[j]) >> anShift[j]);
#endif
            }
        }
    }
}

LOCAL VOID EQS_API s256C_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    switch(pBitmap->Header.nBitsPerPixel)
    {
    case BIT_PER_PIXEL_256C :
        s256C_PutBitmap256C(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_4G :
        s256C_PutBitmap4G(x, y, pBitmap);
        break;
    case BIT_PER_PIXEL_BW :
        s256C_PutBitmapBW(x, y, pBitmap);
        break;
    }
}

#endif /* FEATURE_EQS_COLOR_256 */

/* draw mode (oring, copying) are no meaning in the black and white display device */

LOCAL ROM BYTE l_abBitMSB2LSB[] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
LOCAL ROM BYTE l_abBitLSB2MSB[] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
LOCAL ROM INT8 l_anShift[] = { 0, 7, 6, 5, 4, 3, 2, 1 };

/* use for drawing by other task at a diffent screen */
VOID EQS_API PutBitmapBW(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    sBW_PutBitmap(x, y, pBitmap);
}

LOCAL VOID EQS_API sBW_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth; /* the image width. in case of font image, the font width */
    COORD yOffset; /* y offset in both the screen buffer and the image buffer */
    UINT16 nBytePos; /* byte position at the screen position */
    INT8 nBitPos; /* bit position at the nBytePos */
    INT8 i;
    BYTE* pbScreenData;
    BYTE bScreenData;
    BYTE* pbImageData;
    BYTE bImageData;
    INT16 nBitPosInLastByte;
    BOOL fContinue;

    EQS_ASSERT(pBitmap->Header.nBitsPerPixel == BIT_PER_PIXEL_BW);
    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_BW);

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;

    for(yOffset = 0; yOffset < pBitmap->Header.Size.yHeight; yOffset++)
    {
#ifdef FEATURE_EQS_CLIPPING
        if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
            break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
        nBytePos = BYTE_POS_BW(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
        nBitPos = BIT_POS_BW(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

        /* get screen data at the position */
        pbScreenData = sGETGD().Screen.pbBuffer + nBytePos;
        //pbImageData = (BYTE*)(pBitmap->pbData) + PIXEL2BYTES(pBitmap->Header.Size.xWidth)*yOffset;
        pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>3)*yOffset;

        for(i = 0, fContinue = TRUE; fContinue; i++)
        {
            /* image data */
            bImageData = *(pbImageData + i);

            nBitPosInLastByte = ((i+1) << 3) - xWidth; /* might be larger than -127 */
            if(nBitPosInLastByte > 0) /* break condition */
            {
                if(nBitPosInLastByte >= 8)
                    break;
                bImageData &= ~l_abBitLSB2MSB[nBitPosInLastByte];
                fContinue = FALSE;
            }
            /* get the screen data and clear the bit that will be written on it */
            bScreenData = (*(pbScreenData+i)) & l_abBitMSB2LSB[nBitPos]; 
            /* set screen data */
            bScreenData |= bImageData >> nBitPos;
            *(pbScreenData+i) = bScreenData;
        
            if((i << 3) + l_anShift[nBitPos] >= xWidth)
                break;

            if(nBitPos)
            {
                bScreenData = (*(pbScreenData+i+1)) & ~l_abBitMSB2LSB[nBitPos]; 
                bScreenData |= bImageData << l_anShift[nBitPos];
                *(pbScreenData+i+1) = bScreenData;
            }
        }
    }
}

//#ifdef FEATURE_EQS_COLOR_4GRAY

/* 
** 4 gray bitmap only 
** BW bitmap image should be converted into 4gray one.
*/
LOCAL VOID EQS_API s4G_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    COORD xWidth; /* the image width. in case of font image, the font width */
    COORD yOffset; /* y offset in both the screen buffer and the image buffer */
    WORD wBytePos; /* byte position at the screen position */
    INT8 nBitPos; /* bit position at the wBytePos */
    INT8 i;
    BYTE* pbScreenData;
    BYTE bScreenData;
    BYTE* pbImageData;
    BYTE bImageData;
    INT16 nBitPosInLastByte;
    BOOL fContinue;

    EQS_ASSERT(pBitmap->Header.nBitsPerPixel == BIT_PER_PIXEL_4G);
    EQS_ASSERT(GET_SCREEN_BITS_PER_PIXEL() == BIT_PER_PIXEL_4G);

    x = LPX_2_DPX(x); 
    y = LPy2DPy(y);

#ifdef FEATURE_EQS_SLIDING_WINDOW_X
    if(GET_SLIDING_WINDOW_STATE())
        x -= GET_SLIDING_WINDOW_OFFSET_X();
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */

#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
    if(GET_SLIDING_WINDOW_STATE())
        y -= GET_SLIDING_WINDOW_OFFSET_Y();
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */

#ifdef FEATURE_EQS_CLIPPING
    if(x > DEV_GET_MAX_X())
        return;
#endif /* FEATURE_EQS_CLIPPING */

//    xWidth = (pBitmap->Header.Type == BT_FONT) 
//              ? pBitmap->Header.xWidth : pBitmap->Header.Size.xWidth;
    xWidth = pBitmap->Header.xWidth;
    xWidth <<= 1;

    /* -------------------------
    ** draw mode (OR, COPY, XOR)
    **
    ** followng code looks like redundant but, it is for better speed.
    ** I just use the some redundant code  
    ** instead of the if statement inside the loop and function calls.
    */
    if(GetDrawMode() == DM_COPY)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.Size.yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            wBytePos = BYTE_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            nBitPos = BIT_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + wBytePos;
//            pbImageData = (BYTE*)(pBitmap->pbData) + PIXEL2BYTES(pBitmap->Header.Size.xWidth<<1)*yOffset;
            pbImageData = (BYTE*)(pBitmap->pbData) + (pBitmap->Header.Size.xWidth>>2)*yOffset;

            for(i = 0, fContinue = TRUE; fContinue; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);

                nBitPosInLastByte = ((i+1) << 3) - xWidth; /* might be larger than -127 */
                if(nBitPosInLastByte > 0) /* break condition */
                {
                    if(nBitPosInLastByte >= 8)
                        break;
                    bImageData &= ~l_abBitLSB2MSB[nBitPosInLastByte];
                    fContinue = FALSE;
                }
                /* get the screen data and clear the bit that will be written on it */
                bScreenData = (*(pbScreenData+i)) & l_abBitMSB2LSB[nBitPos]; 
                /* set screen data */
                bScreenData |= bImageData >> nBitPos;
                *(pbScreenData+i) = bScreenData;
        
                if((i << 3) + l_anShift[nBitPos] >= xWidth)
                    break;

                if(nBitPos)
                {
                    bScreenData = (*(pbScreenData+i+1)) & ~l_abBitMSB2LSB[nBitPos]; 
                    bScreenData |= bImageData << l_anShift[nBitPos];
                    *(pbScreenData+i+1) = bScreenData;
                }
            }
        }
    }
    else if(GetDrawMode() == DM_OR)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.Size.yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            wBytePos = BYTE_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            nBitPos = BIT_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + wBytePos;
            pbImageData = (BYTE*)(pBitmap->pbData) + PIXEL2BYTES(pBitmap->Header.Size.xWidth*2)*yOffset;

            for(i = 0, fContinue = TRUE; fContinue; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);

                nBitPosInLastByte = ((i+1) << 3) - xWidth; /* might be larger than -127 */
                if(nBitPosInLastByte > 0) /* break condition */
                {
                    if(nBitPosInLastByte >= 8)
                        break;
                    bImageData &= ~l_abBitLSB2MSB[nBitPosInLastByte];
                    fContinue = FALSE;
                }
                /* get the screen data and clear the bit that will be written on it */
                bScreenData = (*(pbScreenData+i));// & l_abBitMSB2LSB[nBitPos]; 
                /* set screen data */
                bScreenData |= bImageData >> nBitPos;
                *(pbScreenData+i) = bScreenData;
        
                if((i << 3) + l_anShift[nBitPos] >= xWidth)
                    break;

                if(nBitPos)
                {
                    bScreenData = (*(pbScreenData+i+1));// & ~l_abBitMSB2LSB[nBitPos]; 
                    bScreenData |= bImageData << l_anShift[nBitPos];
                    *(pbScreenData+i+1) = bScreenData;
                }
            }
        }
    }
    else if(GetDrawMode() == DM_XOR)
    {
        for(yOffset = 0; yOffset < pBitmap->Header.Size.yHeight; yOffset++)
        {
#ifdef FEATURE_EQS_CLIPPING
            if((y < 0) || ((y + yOffset) > DEV_GET_MAX_Y()))
                break;//continue;
#endif /* FEATURE_EQS_CLIPPING */
            wBytePos = BYTE_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));
            nBitPos = BIT_POS_4G(INDEX_ON_SCREEN(x, y+yOffset, DEV_GET_MAX_BUFFER_WIDTH()));

            /* get screen data at the position */
            pbScreenData = sGETGD().Screen.pbBuffer + wBytePos;
            pbImageData = (BYTE*)(pBitmap->pbData) + PIXEL2BYTES(pBitmap->Header.Size.xWidth*2)*yOffset;

            for(i = 0, fContinue = TRUE; fContinue; i++)
            {
                /* image data */
                bImageData = *(pbImageData + i);

                nBitPosInLastByte = ((i+1) << 3) - xWidth; /* might be larger than -127 */
                if(nBitPosInLastByte > 0) /* break condition */
                {
                    if(nBitPosInLastByte >= 8)
                        break;
                    bImageData &= ~l_abBitLSB2MSB[nBitPosInLastByte];
                    fContinue = FALSE;
                }
                /* get the screen data and clear the bit that will be written on it */
                bScreenData = (*(pbScreenData+i));// & l_abBitMSB2LSB[nBitPos]; 
                /* set screen data */
                bScreenData ^= bImageData >> nBitPos;
                *(pbScreenData+i) = bScreenData;
        
                if((i << 3) + l_anShift[nBitPos] >= xWidth)
                    break;

                if(nBitPos)
                {
                    bScreenData = (*(pbScreenData+i+1));// & ~l_abBitMSB2LSB[nBitPos]; 
                    bScreenData ^= bImageData << l_anShift[nBitPos];
                    *(pbScreenData+i+1) = bScreenData;
                }
            }
        }
    }
}

//#endif /* FEATURE_EQS_COLOR_4GRAY */


/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Save and restore region                                                 */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

/* 
** Get/PutImage buffer
**
**    should be the device coordinates system.
**
**    WORD wWidth;
**    WORD wHeight;
**    BYTE* pbData;
*/
VOID EQS_API GetRegionImage(COORD xLeft, COORD yTop, 
                            COORD xRight, COORD yBottom, BYTE* pbBuffer)
{
    INT8 nBitsPerPixel;
    COORD x, y;
    COORD xWidth, yHeight;
    BYTE* pbScreenBuffer;

    ASSERT(xLeft >= 0 && xLeft < DEV_GET_MAX_WIDTH());
    ASSERT(xRight >= 0 && xRight < DEV_GET_MAX_WIDTH());
    ASSERT(yTop >= 0 && yTop < DEV_GET_MAX_HEIGHT());
    ASSERT(yBottom >= 0 && yBottom < DEV_GET_MAX_HEIGHT());

    nBitsPerPixel = GET_SCREEN_BITS_PER_PIXEL();
    xWidth = xRight - xLeft + 1;
    yHeight = yBottom - yTop + 1;

    ASSERT(xWidth > 0);
    ASSERT(yHeight > 0);

    *pbBuffer++ = (BYTE)(xWidth);
    *pbBuffer++ = (BYTE)(xWidth >> 8);
    *pbBuffer++ = (BYTE)(yHeight);
    *pbBuffer++ = (BYTE)(yHeight >> 8);

    pbScreenBuffer = GET_SCREEN_BUFFER();
    for(y = yTop; y <= yBottom; y++)
    {
        INT16 nCurBytePos;
        INT16 nPrevBytePos = -1;
        for(x = xLeft; x <= xRight; x++)
        {
            nCurBytePos = BYTE_POS(INDEX_ON_SCREEN(x, y, 
                                                DEV_GET_MAX_WIDTH()),
                                   nBitsPerPixel);
            if(nCurBytePos != nPrevBytePos)
            {
                *pbBuffer++ = GET_COLOR_BYTE(x, y, DEV_GET_MAX_WIDTH(), nBitsPerPixel, pbScreenBuffer);
            }
            nPrevBytePos = nCurBytePos;
        }
    }
}

VOID EQS_API GetRegionImageRect(RECT *pRect, BYTE* pbBuffer)
{
    GetRegionImage(pRect->xLeft, pRect->yTop, 
                   pRect->xRight, pRect->yBottom, pbBuffer);
}

VOID EQS_API PutRegionImage(COORD xLeft, COORD yTop, BYTE* pbBuffer)
{
//    INT16 nIndexOnScreen;
    INT8 nBitsPerPixel;
//    INT16 nBytePos;
    COORD x, y;
    COORD xWidth, yHeight;
    COORD xRight, yBottom;

    nBitsPerPixel = GET_SCREEN_BITS_PER_PIXEL();

    xWidth = pbBuffer[0] + (pbBuffer[1] << 8);
    yHeight = pbBuffer[2] + (pbBuffer[3] << 8);

    xRight = xLeft + xWidth - 1;
    yBottom = yTop + yHeight - 1;

    ASSERT(xLeft > 0 && xLeft < DEV_GET_MAX_WIDTH());
    ASSERT(xRight > 0 && xRight < DEV_GET_MAX_WIDTH());
    ASSERT(yTop > 0 && yTop < DEV_GET_MAX_HEIGHT());
    ASSERT(yBottom > 0 && yBottom < DEV_GET_MAX_HEIGHT());

    ASSERT((xRight - xLeft) > 0);
    ASSERT((yBottom - yTop) > 0);
    
    pbBuffer += 4;  /* skip width and height part */
    for(y = yTop; y <= yBottom; y++)
    {
        INT16 nCurBytePos;
        INT16 nPrevBytePos = -1;
 
        for(x = xLeft; x <= xRight; x++)
        {
            nCurBytePos = BYTE_POS(INDEX_ON_SCREEN(x, y, 
                                                DEV_GET_MAX_WIDTH()),
                                nBitsPerPixel);
            if(nCurBytePos != nPrevBytePos)
            {
                SET_COLOR_BYTE(x, y, DEV_GET_MAX_WIDTH(),
                               sGETGD().Screen.Info.ColorSpace, sGETGD().Screen.pbBuffer, 
                               *pbBuffer++);
            }
            nPrevBytePos = nCurBytePos;
        }
    }
}

WORD EQS_API GetRegionImageSize(COORD xLeft, COORD yTop, 
                                COORD xRight, COORD yBottom)
{
    WORD wBytesPerLine;
    
    ASSERT(xLeft >= 0 && xLeft < DEV_GET_MAX_WIDTH());
    ASSERT(xRight >= 0 && xRight < DEV_GET_MAX_WIDTH());
    ASSERT(yTop >= 0 && yTop < DEV_GET_MAX_HEIGHT());
    ASSERT(yBottom >= 0 && yBottom < DEV_GET_MAX_HEIGHT());

    wBytesPerLine = PIXEL2BYTES(xRight-xLeft+1);
    return wBytesPerLine * (yBottom - yTop + 1) 
           * sGETGD().Screen.Info.ColorSpace + 4;
}

WORD EQS_API GetRegionImageSizeRect(RECT *pRect)
{
    return GetRegionImageSize(pRect->xLeft, pRect->yTop, 
                              pRect->xRight, pRect->yBottom);
}

VOID EQS_API GetRegionImageBuffer(COORD xLeft, COORD yTop, COORD xRight, 
                                  COORD yBottom, BYTE** ppbBuffer)
{
    *ppbBuffer= (BYTE*)EQS_Malloc(EQS_GetRegionImageSize(xLeft, yTop, xRight, yBottom));
}

VOID EQS_API GetRegionImageBufferRect(RECT *pRect, BYTE** ppbBuffer)
{
    GetRegionImageBuffer(pRect->xLeft, pRect->yTop, 
                   pRect->xRight, pRect->yBottom, ppbBuffer);
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Draw bitmap font                                                        */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

/*
** ***************************************************************************
** The naming convention of the font file
** 
** EBF : Equs Bitmap Font
** ECF : Equs Compound type bitmap Font
** EBI : Equs Bitmap Image
**
** <SIZE><CLASSIFICATION><BITS PER PIXEL>.EBI
*/
#define TKN_BBIG_ASCII_FONT_BW          "0818ASC1.EBF"//"0820ASC1.EBF"
#if sBIG_FONT_SIZE == sUSE_14X16_BIG_FONT
#define TKN_BIG_ASCII_FONT_BW           "0716ASC1.EBF"
#define TKN_BIG_HANGUL_FONT_BW          "1416HAN1.EBF"
#define TKN_BIG_SPECIAL_FONT_BW         "1416SPC1.EBF"
#define TKN_BIG_SKYSYMBOL_FONT_4G       "1416SKY2.EBF"
#elif sBIG_FONT_SIZE == sUSE_14X15_BIG_FONT
#define TKN_BIG_ASCII_FONT_BW           "0715ASC1.EBF"
#define TKN_BIG_HANGUL_FONT_BW          "1415HAN1.EBF"
#define TKN_BIG_SPECIAL_FONT_BW         "1415SPC1.EBF"
#define TKN_BIG_SKYSYMBOL_FONT_256C     "1415SKY8.EBF"
#else
#error Choose one font size at least !!
#endif

#ifdef FEATURE_EQS_HANJA
#define TKN_BIG_HANJA_FONT_BW           "1616HAJ1.EBF"
#endif /* FEATURE_EQS_HANJA */
#define TKN_MED_ASCII_FONT_BW           "0613ASC1.EBF"
#define TKN_MED_HANGUL_FONT_BW          "1213HAN1.EBF"
#define TKN_MED_SPECIAL_FONT_BW         "1213SPC1.EBF"
#define TKN_MED_SKYSYMBOL_FONT_BW       "1213SKY1.EBF"
#if defined FEATURE_EQS_COLOR_256 || defined FEATURE_EQS_COLOR_64K
#define TKN_MED_SKYSYMBOL_FONT_256C     "1213SKY8.EBF"
#endif
#define TKN_SMALL_ASCII_FONT_BW         "0610ASC1.EBF"//"0509ASC1.EBF"
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
/* ************************************************************************* *
** The count of initial consonat        : 8 sets
** The count of medial vowel            : 4 sets 
** The final consonant                  : 4 sets
** ************************************************************************* */
#define TKN_COMP_BIG_BATANG_FONT_BW     "1416HBAT.ECF"
#define TKN_COMP_BIG_DODUM_FONT_BW      "1416HDOD.ECF"
//#define TKN_COMP_BIG_GULIM_FONT_BW      "1416HGUL.ECF"
//#define TKN_COMP_MED_DODUM_FONT_BW      "1213HDOD.ECF"
#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */

/*
** ***************************************************************************
** Font binary data
*/

/* 10x18 */
LOCAL ROM BYTE abBigBigASCIIFontBuffer[] = {
    #include TKN_BBIG_ASCII_FONT_BW
};

/* 7x16 or 7x15 */
LOCAL ROM BYTE abBigASCIIFontBuffer[] = {
    #include TKN_BIG_ASCII_FONT_BW
};

/* 14x16 or 14x15*/
LOCAL ROM BYTE abBigHangulFontBuffer[] = {
    #include TKN_BIG_HANGUL_FONT_BW
};

/* 14x16 or 14x15 */
LOCAL ROM BYTE abBigSpecialFontBuffer[] = {
    #include TKN_BIG_SPECIAL_FONT_BW
};

#ifdef FEATURE_EQS_HANJA
/* 16x16 */
LOCAL ROM BYTE abBigHanjaFontBuffer[] = {
    #include TKN_BIG_HANJA_FONT_BW
};
#endif /* FEATURE_EQS_HANJA */

/* 14x16  or 14x15*/
LOCAL ROM BYTE abBigSkySymbolFontBuffer[] = {
#if sBIG_FONT_SIZE == sUSE_14X16_BIG_FONT
    #include TKN_BIG_SKYSYMBOL_FONT_4G
#elif sBIG_FONT_SIZE == sUSE_14X15_BIG_FONT
    #include TKN_BIG_SKYSYMBOL_FONT_256C
#else
#error Choose one font size at least !!
#endif
};

/* 6x13 */
LOCAL ROM BYTE abMedASCIIFontBuffer[] = {
    #include TKN_MED_ASCII_FONT_BW
};

/* 12x13 */
LOCAL ROM BYTE abMedHangulFontBuffer[] = {
    #include TKN_MED_HANGUL_FONT_BW
};

/* 12x13 */
LOCAL ROM BYTE abMedSpecialFontBuffer[] = {
    #include TKN_MED_SPECIAL_FONT_BW
};

/* 12x13 */
LOCAL ROM BYTE abMedSkySymbolFontBuffer[] = {
#if defined FEATURE_EQS_COLOR_256 || defined FEATURE_EQS_COLOR_64K
    #include TKN_MED_SKYSYMBOL_FONT_256C
#else
    #include TKN_MED_SKYSYMBOL_FONT_BW
#endif
};

/* 06x10 */
LOCAL ROM BYTE abSmallASCIIFontBuffer[] = {
    #include TKN_SMALL_ASCII_FONT_BW
};

#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT

/* 14 x 16 */
LOCAL ROM BYTE abBigCompHangulBatangFontBuffer[] = {
    #include TKN_COMP_BIG_BATANG_FONT_BW
};

/* 14 x 16 */
LOCAL ROM BYTE abBigCompHangulDodumFontBuffer[] = {
    #include TKN_COMP_BIG_DODUM_FONT_BW
};

///* 14 x 16 */
//LOCAL ROM BYTE abBigCompHangulGulimFontBuffer[] = {
//    #include TKN_COMP_BIG_GULIM_FONT_BW
//};

///* 12 x 13 */
//LOCAL ROM BYTE abMedCompHangulDodumFontBuffer[] = {
//    #include TKN_COMP_MED_DODUM_FONT_BW
//};

#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */

/*
** ***************************************************************************
** Bitmap font information
*/
#define LARGEST_FONT_HEIGHT                 18 // 20
#define LARGEST_FONT_WIDTH_BYTE             1  //2

#define BBIG_ASCII_FONT_SIZE                { 8, LARGEST_FONT_HEIGHT } //10, LARGEST_FONT_HEIGHT }
#if sBIG_FONT_SIZE == sUSE_14X16_BIG_FONT
#define BIG_ASCII_FONT_SIZE                 {  7, 16 }
#define BIG_HANGUL_FONT_SIZE                { 14, 16 }
#define BIG_SPECIAL_FONT_SIZE               { 14, 16 }
#define BIG_SKYSYMBOL_FONT_SIZE             { 14, 16 }
#elif sBIG_FONT_SIZE == sUSE_14X15_BIG_FONT
#define BIG_ASCII_FONT_SIZE                 {  7, 15 }
#define BIG_HANGUL_FONT_SIZE                { 14, 15 }
#define BIG_SPECIAL_FONT_SIZE               { 14, 15 }
#define BIG_SKYSYMBOL_FONT_SIZE             { 14, 15 }
#else
#error Choose one font size at least !!
#endif

#ifdef FEATURE_EQS_HANJA
#define BIG_HANJA_FONT_SIZE                 { 16, 16 }
#endif /* FEATURE_EQS_HANJA */
#define MED_ASCII_FONT_SIZE                 {  6, 13 }
#define MED_HANGUL_FONT_SIZE                { 12, 13 }
#define MED_SPECIAL_FONT_SIZE               { 12, 13 }
#define MED_SKYSYMBOL_FONT_SIZE             { 12, 13 }
#define SMALL_ASCII_FONT_SIZE               {  6, 10 }
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
#define BIG_COMP_HANGUL_BATANG_FONT_SIZE    { 14, 16 }
#define BIG_COMP_HANGUL_DODUM_FONT_SIZE     { 14, 16 }
//#define BIG_COMP_HANGUL_GULIM_FONT_SIZE     { 14, 16 }
//#define MED_COMP_HANGUL_DODUM_FONT_SIZE     { 12, 13 }
#endif

LOCAL ROM BITMAP_FONT BigBigASCIIFont = {
    BBIG_ASCII_FONT_SIZE, 1, abBigBigASCIIFontBuffer
};

LOCAL ROM BITMAP_FONT BigASCIIFont = {
    BIG_ASCII_FONT_SIZE, 1, abBigASCIIFontBuffer
};

LOCAL ROM BITMAP_FONT BigHangulFont = {
    BIG_HANGUL_FONT_SIZE, 1, abBigHangulFontBuffer
};

LOCAL ROM BITMAP_FONT BigSpecialFont = {
    BIG_SPECIAL_FONT_SIZE, 1, abBigSpecialFontBuffer
};

LOCAL ROM BITMAP_FONT BigSkySymbolFont = {
#if sBIG_FONT_SIZE == sUSE_14X16_BIG_FONT
    BIG_SKYSYMBOL_FONT_SIZE, 2, abBigSkySymbolFontBuffer
#elif sBIG_FONT_SIZE == sUSE_14X15_BIG_FONT
    BIG_SKYSYMBOL_FONT_SIZE, 8, abBigSkySymbolFontBuffer
#else
#error Choose one font size at least !!
#endif
};

#ifdef FEATURE_EQS_HANJA
LOCAL ROM BITMAP_FONT BigHanjaFont = {
    BIG_HANJA_FONT_SIZE, 1, abBigHanjaFontBuffer
};
#endif /* FEATURE_EQS_HANJA */

LOCAL ROM BITMAP_FONT MedASCIIFont = {
    MED_ASCII_FONT_SIZE, 1, abMedASCIIFontBuffer
};

LOCAL ROM BITMAP_FONT MedHangulFont = {
    MED_HANGUL_FONT_SIZE, 1, abMedHangulFontBuffer
};

LOCAL ROM BITMAP_FONT MedSpecialFont = {
    MED_SPECIAL_FONT_SIZE, 1, abMedSpecialFontBuffer
};

#if defined FEATURE_EQS_COLOR_256 || defined FEATURE_EQS_COLOR_64K
LOCAL ROM BITMAP_FONT MedSkySymbolFont = { 
    MED_SKYSYMBOL_FONT_SIZE, 8, abMedSkySymbolFontBuffer
};
#else
LOCAL ROM BITMAP_FONT MedSkySymbolFont = { 
    MED_SKYSYMBOL_FONT_SIZE, 1, abMedSkySymbolFontBuffer
};
#endif
LOCAL ROM BITMAP_FONT SmallASCIIFont = {
    SMALL_ASCII_FONT_SIZE, 1, abSmallASCIIFontBuffer
};

#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT

LOCAL ROM BITMAP_FONT BigCompHangulBatangFont = {
    BIG_COMP_HANGUL_BATANG_FONT_SIZE, 1, abBigCompHangulBatangFontBuffer
};

LOCAL ROM BITMAP_FONT BigCompHangulDodumFont = {
    BIG_COMP_HANGUL_DODUM_FONT_SIZE, 1, abBigCompHangulDodumFontBuffer
};

//LOCAL ROM BITMAP_FONT BigCompHangulGulimFont = {
//    BIG_COMP_HANGUL_GULIM_FONT_SIZE, 1, abBigCompHangulGulimFontBuffer
//};
//
//LOCAL ROM BITMAP_FONT MedCompHangulDodumFont = {
//    MED_COMP_HANGUL_DODUM_FONT_SIZE, 1, abBigCompHangulDodumFontBuffer
//};

#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */




ROM BITMAP_FONT_SET BitmapFontSet[] = {
    /* FT_BIG_BIG */
    { 
        &BigBigASCIIFont, NULL, NULL, NULL
#ifdef FEATURE_EQS_HANJA
        , NULL
#endif
    },
    /* FT_BIG */
    { 
        &BigASCIIFont, &BigHangulFont, &BigSpecialFont, &BigSkySymbolFont
#ifdef FEATURE_EQS_HANJA
       , &BigHanjaFont
#endif
    },
    /* FT_MED */
    { 
        &MedASCIIFont, &MedHangulFont, &MedSpecialFont, &MedSkySymbolFont
#ifdef FEATURE_EQS_HANJA
       , NULL
#endif
    },
    /* FT_SMALL */
    { 
        &SmallASCIIFont, NULL, NULL, NULL
#ifdef FEATURE_EQS_HANJA
        , NULL
#endif
    },

    /* FT_BIG_COMP_BATANG */ 
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
    { 
        &BigASCIIFont, &BigCompHangulBatangFont, &BigSpecialFont, &BigSkySymbolFont 
#ifdef FEATURE_EQS_HANJA
        , &BigHanjaFont
#endif
    },
    /* FT_BIG_COMP_DODUM */
    { 
        &BigASCIIFont, &BigCompHangulBatangFont, &BigSpecialFont, &BigSkySymbolFont 
#ifdef FEATURE_EQS_HANJA
        , &BigHanjaFont
#endif
    }
//    /* FT_BIG_COMP_GULIM */
//    { 
//        &BigASCIIFont, &BigCompHangulBatangFont, &BigSpecialFont, &BigSkySymbolFont 
//#ifdef FEATURE_EQS_HANJA
//        , &BigHanjaFont
//#endif
//    },
//    /* FT_MED_COMP_DODUM */
//    { 
//        &MedASCIIFont, &MedHangul844Font, &MedSpecialFont, &MedSkySymbolFont 
//#ifdef FEATURE_EQS_HANJA
//        , NULL
//#endif
//    }
#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */
};

/* 
** MAX_FONT_BUFFER_A_CHAR = LARGEST_FONT_HEIGHT * the width in byte 
** 42 = LARGEST_FONT_HEIGHT * LARGEST_FONT_WIDTH_BYTE
*/
#define MAX_FONT_BUFFER_A_CHAR      42*2  /* byte */


#ifdef FEATURE_SKVM
BYTE*   EQS_API EQS_SetSKVMTextDrawMode(CONST BYTE* pSrc, INT8 nWidthByte, INT8 nHeight){
	return sSetTextDrawMode(pSrc, nWidthByte, nHeight);
}
#endif 
//LOCAL VOID EQS_API sSetTextDrawMode(BYTE** ppDest, CONST BYTE* pSrc, 
//                                   INT8 nWidthByte, INT8 nHeight)
LOCAL BYTE* EQS_API sSetTextDrawMode(CONST BYTE* pSrc, 
                                   INT8 nWidthByte, INT8 nHeight)
{
    STATIC BYTE Buffer[MAX_FONT_BUFFER_A_CHAR];
    BYTE TempBuffer[MAX_FONT_BUFFER_A_CHAR];
    //BYTE* pBuffer;
    CONST BYTE* pTempSrc;
    BYTE* pTempDest;
    BYTE bDrawMode;
    INT8 i, j;

    memcpy(Buffer, pSrc, (SIZE_T)(nWidthByte * nHeight));
    //pBuffer = Buffer;

    bDrawMode = GetTextDrawMode();
    
    if(bDrawMode & TDM_BOLD)
    {
        pTempSrc = pSrc;
        pTempDest = Buffer;
        for(i = 0; i < nHeight; i++)
        {
            for(j = 0; j < nWidthByte; j++)
            {
                *pTempDest |= (BYTE)(*pTempSrc >> 1);
                if(j > 0)
                    *pTempDest |= (BYTE)(*(pTempSrc-1) << 7);
                
                pTempDest++;
                pTempSrc++;
            }
        }
    }
    memcpy(TempBuffer, Buffer, (SIZE_T)(nWidthByte * nHeight));

    if(bDrawMode & TDM_SHADOW)
    {
        pTempDest = Buffer + nWidthByte;
        pTempSrc = TempBuffer + nWidthByte;
        for(i = 1; i < nHeight; i++)
        {
            for(j = 0; j < nWidthByte; j++)
            {
                *pTempDest |= *(pTempSrc - nWidthByte);
                *pTempDest |= (BYTE)((*pTempSrc) >> 1);
                if(j > 0)
                    *pTempDest |= (BYTE)(*(pTempSrc-1) << 7);
                
                pTempDest++;
                pTempSrc++;
            }
        }
    }

    if(bDrawMode & TDM_OUTLINE)
    {
        pTempDest = Buffer;
        pTempSrc = TempBuffer;
        for(i = 0; i < nHeight; i++)
        {
            for(j = 0; j < nWidthByte; j++)
            {
                if( i > 0) *pTempDest |= *(pTempSrc-nWidthByte);
                if(i < nHeight - 1)
                    *pTempDest |= *(pTempSrc + nWidthByte);

                *pTempDest |= (BYTE)(*pTempSrc << 1);
                *pTempDest |= (BYTE)(*pTempSrc >> 1);
                if(j > 0)
                    *pTempDest |= (BYTE)(*(pTempSrc-1) << 7);
                if(j < nWidthByte-1)
                    *pTempDest |= (BYTE)(*(pTempSrc+1) >> 7);
                
                pTempDest++;
                pTempSrc++;
            }
        }
    }

    if(bDrawMode & TDM_UNDERLINE)
    {
        pTempDest = Buffer + (nHeight-1)*nWidthByte;
        for(j = 0; j < nWidthByte; j++)
            *pTempDest++ |= 0xFF;
    }

    if(bDrawMode & TDM_GRAY)
    {
        pTempDest = Buffer;
        for(i = 0; i < nHeight; i++)
        {
            for(j = 0; j < nWidthByte; j++, pTempDest++)
                *pTempDest |= ((i%2) ? 0xAA : 0x55);
        }
    }

    if((bDrawMode & TDM_SHADOW) || (bDrawMode & TDM_OUTLINE))
    {
        pTempSrc = TempBuffer;
        pTempDest = Buffer;
        for(j = 0; j < nWidthByte*nHeight; j++)
            *pTempDest++ ^= *pTempSrc++;
    }

    if(bDrawMode & TDM_REVERSE)
    {
        pTempDest = Buffer;
        for(j = 0; j < nWidthByte*nHeight; j++)
            *pTempDest++ ^= 0xFF;
    }
   
    return Buffer;
}

LOCAL BYTE* EQS_API sMake4GrayFromBWFontImage(CONST BYTE* pbSrc, INT8 nBytePerChar)
{
    STATIC BYTE s_abBuffer[MAX_FONT_BUFFER_A_CHAR];
    //INT8 i, j, index;
    INT8 index;
    WORD dwMaskBit;
	WORD dwBuffer;

    dwMaskBit = GET_TEXT_COLOR() << 14;
	for(index = 0; index < nBytePerChar; index++)
	{

        // girin. 200.12.32
        // Use table look-up method rahter bit-wise operations
        // to increse performance.
/*
		if (*pbSrc)
		{
			for(i = 0, j = 0, dwBuffer = 0; i < 8; i++, j+= 2)
			{ 
				if (*pbSrc & (sCHECK_BIT >> i))
					dwBuffer |= (dwMaskBit >> j);
				else 
					dwBuffer &= ~(dwMaskBit >> j);
			}
		}	
		else
        {
            dwBuffer = 0;
        }
*/
        dwBuffer = wColor2ToColor4Table[*pbSrc];

        // girin. end.

		s_abBuffer[index*2 + 1] = (BYTE)sGET_LOW_BYTE(dwBuffer);
		s_abBuffer[index*2]     = (BYTE)sGET_HIGH_BYTE(dwBuffer);

		pbSrc++;
	}

	return s_abBuffer;
}

LOCAL VOID EQS_API sPutBitmapFont(COORD x, COORD y, WORD wCode, 
                                 CONST BITMAP_FONT* pBitmapFont)
{
    BITMAP Bitmap;

    /* make a bitmap from a bitmap font buffer */
    //Bitmap.Header.Type = BT_FONT;
#if defined FEATURE_EQS_COLOR_256 || defined FEATURE_EQS_COLOR_64K
    if(pBitmapFont->nBitsPerPixel == BIT_PER_PIXEL_256C)
    {
        Bitmap.Header.abID[0] = EQUS_IMAGE_ID_STR[0];
        Bitmap.Header.abID[1] = EQUS_IMAGE_ID_STR[1];
        Bitmap.Header.abID[2] = EQUS_IMAGE_ID_STR[2];
        Bitmap.Header.nBitsPerPixel = pBitmapFont->nBitsPerPixel;
        Bitmap.Header.Size = pBitmapFont->Size;
        Bitmap.Header.xWidth = pBitmapFont->Size.xWidth;
        Bitmap.Header.TransparentColor = (COLOR)0x01;
        Bitmap.Header.yStart = 0;
    }
    else
#endif
    {
        Bitmap.Header.abID[0] = EQUS_IMAGE_ID_STR[0];
        Bitmap.Header.abID[1] = EQUS_IMAGE_ID_STR[1];
        Bitmap.Header.abID[2] = EQUS_IMAGE_ID_STR[2];
        Bitmap.Header.fPalette = FALSE;
        Bitmap.Header.nBitsPerPixel = pBitmapFont->nBitsPerPixel;
        Bitmap.Header.Size.xWidth = PIXEL2BYTES(pBitmapFont->Size.xWidth) * 8;
        Bitmap.Header.Size.yHeight = pBitmapFont->Size.yHeight;
        Bitmap.Header.xWidth = pBitmapFont->Size.xWidth;
    }
    /* get the start pointer of the font data of wCode */
    /* we can only set text draw mode about the black and white font */
    if(pBitmapFont->nBitsPerPixel == 1) 
    {
        //sSetTextDrawMode(&(Bitmap.pbData), sGetFontBuffer(wCode, pBitmapFont), 
        Bitmap.pbData = sSetTextDrawMode(sGetFontBuffer(wCode, pBitmapFont), 
                        (INT8)PIXEL2BYTES(pBitmapFont->Size.xWidth),
                        (INT8)pBitmapFont->Size.yHeight);
        /* --------------------------------------------
        ** convert bw font image into 4 gray font image 
        ** for accelerating display speed 
        */
        if(GET_SCREEN_BITS_PER_PIXEL() == CS_4GRAY) /* 4gray only */
        {
            Bitmap.Header.nBitsPerPixel = BIT_PER_PIXEL_4G;
            Bitmap.pbData = sMake4GrayFromBWFontImage(Bitmap.pbData, 
                                         (INT8)(PIXEL2BYTES(pBitmapFont->Size.xWidth)
                                          * pBitmapFont->Size.yHeight));
//            Bitmap.Header.Size.xWidth <<= 1; /* multiply 2 */
//            Bitmap.Header.xFontWidth <<= 1;
        }
    }
    else
    {
        Bitmap.pbData = sGetFontBuffer(wCode, pBitmapFont);
    }
    l_fPutFont = TRUE;
    sGETGD().PutBitmap(x, y, &Bitmap);
    //PutBitmap(x, y, &Bitmap);
    l_fPutFont = FALSE;
}

LOCAL CONST BITMAP_FONT_SET* EQS_API sGetCurrentBitmapFontSet(VOID)
{
    return &BitmapFontSet[GET_FONT_TYPE()];
}

#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */

//LOCAL SHORT EQS_API sCodeCompare(WORD* p, WORD* q)
//{
//    return (*p - *q);
//}

LOCAL WORD EQS_API sKS5601_2KSSM(WORD wCode)
{
    BYTE b1st, b2nd;

    b1st = wCode >> 8;
    b2nd = wCode & 0x00FF;

    if(b1st == 0xA4)
        return (b2nd >= 0xA1 && b2nd <= 0xD3) 
                ? awKSSMPieceHangul[b2nd - 0xA1] : 0;
    else if(b1st < 0xB0 || b1st > 0xC8 || b2nd < 0xA1 || b2nd > 0xFE)
        return 0;
    
    return awKSSMCodeTable[(b1st - 0xB0) * 0x5E + (b2nd - 0xA1)];
}

/* the count of the set of inital consonant */
#define SET_INIT_CONSONANT              8 
/* the count of the set of medial vowel */
#define SET_MED_VOWEL                   4 
/* the count of the set of final consonant */
#define SET_FINAL_CONSONANT             4 

#define COUNT_INIT_CONSONANT            19 
#define COUNT_MED_VOWEL                 21 
#define COUNT_FINAL_CONSONANT           27 

#define SIZE_HANGUL_FONT_BUFFER         32 /* 16*16/8 */

/* 608 = COUNT_INIT_CONSONANT * SIZE_HANGUL_FONT_BUFFER */
#define INIT_CONSONANT_OFFSET           608
/* 672 = COUNT_MED_VOWEL * SIZE_HANGUL_FONT_BUFFER */
#define MED_VOWEL_OFFSET                672
/* 864 = COUNT_FINAL_CONSONANT * SIZE_HANGUL_FONT_BUFFER */
#define FINAL_CONSONANT_OFFSET          864

LOCAL VOID sMemOring(BYTE* pb1, ROM BYTE* pb2, INT8 nLen)
{
    do {
        *pb1++ |= *pb2++;
    } while(--nLen);
}

LOCAL BYTE* EQS_API sGetCompoundHangulImage(WORD wCode)
{
    STATIC BYTE bBuffer[SIZE_HANGUL_FONT_BUFFER];
    INT8 nInit;         /* the initial consonant code of KSSM */
    INT8 nMed;          /* the medial vowel code of KSSM */
    INT8 nFinal;        /* the final consonant code of KSSM */
    INT8 nInitType;     /* the type of the initial consonant in the 8 sets */
    INT8 nMedType;      /* the type of the medial vowel in the 4 sets */
    INT8 nFinalType;    /* the type of the final consonant in the 4 sets */
    /* initial consonant index table */
    ROM INT8 anInitConsonant[] = {
         0,  0,  1,  2,  3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
        15, 16, 17, 18, 19, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0
    };
    /* medial vowel index table */
    ROM INT8 anMedVowel[] = {
         0,  0,  0,  1,  2,  3,  4,  5, 0, 0,  6,  7,  8,  9, 10, 11, 
         0,  0, 12, 13, 14, 15, 16, 17, 0, 0, 18, 19, 20, 21,  0,  0
    };
    /* final consonant index table */
    ROM INT8 anFinalConsonant[] = {
         0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 
        15, 16,  0, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,  0,  0
    };
    /* an initial consonant according to medial vowel - no final consonant */
    ROM INT8 anInitConsNoFinal2Med[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0
    };
    /* an initial consonant according to medial vowel - yes final consonant */
    ROM INT8 anInitConsYesFinal2Med[] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5
    };
    /* an final consonant according to medial vowel */
    ROM INT8 anFinalCons2Med[] = {
        0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1
    };
    CONST BYTE* pbInitConsonant  = pbCompHangulFontBuffer;
    /* 4864 = SET_INIT_CONSONANT * COUNT_INIT_CONSONANT * SIZE_HANGUL_FONT_BUFFER */
    CONST BYTE* pbMedVowel       = pbCompHangulFontBuffer + 4864;
    /* 7552 = 4864 + SET_MED_VOWEL * COUNT_MED_VOWEL * SIZE_HANGUL_FONT_BUFFER */
    CONST BYTE* pbFinalConsonant = pbCompHangulFontBuffer + 7552;

    /* get the code */
    nInit  = (wCode >>10) & 0x1F;
    nMed   = (wCode >> 5) & 0x1F;
    nFinal = (wCode)      & 0x1F;

    /* get the index from the code */
    nInit  = anInitConsonant[nInit];
    nMed   = anMedVowel[nMed];
    nFinal = anFinalConsonant[nFinal];

    /* get the type of a peice of syllable */
    nInitType  = nFinal ? anInitConsYesFinal2Med[nMed] 
                          : anInitConsNoFinal2Med[nMed];
    nMedType   = ((nInit == 0 || nInit == 1 || nInit == 16) ? 0 : 1) 
                 + (nFinal ? 2 : 0);
    nFinalType = anFinalCons2Med[nMed];

    if(nInit)
        memcpy(bBuffer,
                   pbInitConsonant + nInitType * INIT_CONSONANT_OFFSET
                   + (nInit-1) * SIZE_HANGUL_FONT_BUFFER, 
                   SIZE_HANGUL_FONT_BUFFER);
    else 
        memset(bBuffer, 0, SIZE_HANGUL_FONT_BUFFER);
    if(nMed)
        sMemOring(bBuffer, 
                  pbMedVowel + nMedType * MED_VOWEL_OFFSET
                  + (nMed-1) * SIZE_HANGUL_FONT_BUFFER, 
                  SIZE_HANGUL_FONT_BUFFER);
    if(nFinal)
        sMemOring(bBuffer, 
                  pbFinalConsonant + nFinalType * FINAL_CONSONANT_OFFSET
                  + (nFinal-1) * SIZE_HANGUL_FONT_BUFFER, 
                  SIZE_HANGUL_FONT_BUFFER);
    return bBuffer;
}

/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */

#ifdef FEATURE_SKVM
CONST BYTE* EQS_API EQS_GetFontBuffer(WORD wCode, CONST BITMAP_FONT *pBitmapFont){
	return sGetFontBuffer(wCode, pBitmapFont);
}
#endif 
LOCAL CONST BYTE* EQS_API sGetFontBuffer(WORD wCode, 
                                         CONST BITMAP_FONT *pBitmapFont)
{
    WORD wRow, wColumn, wIndex;
    UINT8 nBytesPerCharRow;
    UINT8 nBytesPerChar;
    CONST BYTE* pbBuffer;

    if(pBitmapFont == NULL)
        return NULL;

    /* KSC 5601 special character code */
    if(wCode >= 0xA1A1 && wCode <=0xACFE)
    {
        wCode -= 0xA1A1;
        wRow = wCode / 0x100;
        wColumn = wCode % 0x100;
        wIndex = wRow * 0x5E + wColumn;
    }
    /* SKY symbol code */
    else if(wCode >= 0x81A0 && wCode <= 0x89FF)
    {
        wCode -= 0x81A0;
        wRow = wCode / 0x100;
        wColumn = wCode % 0x100;
        wIndex = wRow * 0x60 + wColumn;
    }
    /* Hangul */
    else if(wCode >= 0xB0A1 && wCode <= 0xC9AA)
    {
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
        /* convert KSC5601 Hangul code to KSSM */
        if(sGETDC().FontType >= FT_BIG_COMP_BATANG)
        {
            wCode = sKS5601_2KSSM(wCode);
            pbBuffer = sGetCompoundHangulImage(wCode);
            return pbBuffer;
        }
        else
        {
#endif
        wCode -= 0xB0A1;
        wRow = wCode / 0x100;
        wColumn = wCode % 0x100;
        wIndex = wRow * 0x5E + wColumn;
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
        }
#endif
    }
#ifdef FEATURE_EQS_HANJA
    /* Hanja */
    else if(wCode >= 0xCAA1 && wCode <= 0xFDFE)
    {
        wCode -= 0xCAA1;
        wRow = wCode / 0x100;
        wColumn = wCode % 0x100;
        wIndex = wRow * 0x5E + wColumn;
    }
#endif /* FEATURE_EQS_HANJA */
    /* ASCII */
    else
    {
        wCode &= 0x7F;
        wIndex = wCode;
    }
    
    if(pBitmapFont->nBitsPerPixel == BIT_PER_PIXEL_256C)
    {
        nBytesPerChar = pBitmapFont->Size.xWidth * pBitmapFont->Size.yHeight;
    }
    else
    {
        nBytesPerCharRow = PIXEL2BYTES(pBitmapFont->Size.xWidth);
        nBytesPerChar = nBytesPerCharRow * (pBitmapFont->Size.yHeight) 
                        * pBitmapFont->nBitsPerPixel;
    }
    pbBuffer = pBitmapFont->pbFontBuffer + wIndex * nBytesPerChar;

    return pbBuffer;
}

CONST SIZE* EQS_API GetFontSize(FONT_TYPE FontType, FONT_KIND FontKind)
{
    CONST BITMAP_FONT_SET* pBitmapFontSet;
    CONST BITMAP_FONT* pBitmapFont = NULL;

    /* Get bitmap font */
    pBitmapFontSet = &BitmapFontSet[FontType];

    ASSERT(pBitmapFontSet);
    
    switch(FontKind)
    {
    case FK_ASCII :
        pBitmapFont = pBitmapFontSet->pASCIIBitmapFont;
        break;
    case FK_HANGUL :
        pBitmapFont = pBitmapFontSet->pHangulBitmapFont;
        break;
    case FK_SKYSYMBOL :
        pBitmapFont = pBitmapFontSet->pSkySymbolBitmapFont;
        break;
#ifdef FEATURE_EQS_HANJA
    case FK_HANJA :
        pBitmapFont = pBitmapFontSet->pHanjaBitmapFont;
        break;
#endif /* FEATURE_EQS_HANJA */
    }
    if(pBitmapFont)
        return &(pBitmapFont->Size);
    else
        return NULL;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Font                                                                    */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API SetFontType(FONT_TYPE FontType)
{
    sGETDC().FontType = FontType;
    
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT

    switch(FontType)
    {
    case FT_BIG_COMP_BATANG :
        pbCompHangulFontBuffer = abBigCompHangulBatangFontBuffer;
        break;
    case FT_BIG_COMP_DODUM :
        pbCompHangulFontBuffer = abBigCompHangulDodumFontBuffer;
        break;
    //case FT_BIG_COMP_GULIM :
    //    pbCompHangulFontBuffer = abBigCompHangulGulimFontBuffer;
    //    break;
    //case FT_MED_COMP_DODUM :
    //    pbCompHangulFontBuffer = abMedCompHangulDodumFontBuffer;
    //    break;
    }

#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */
}

FONT_TYPE EQS_API GetFontType(VOID)
{
    return sGETDC().FontType;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Text align                                                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API SetTextAlign(TEXT_ALIGN* pTextAlign)
{
    sGETDC().TextAlign = *pTextAlign;
}

VOID EQS_API SetTextAlignHoriz(TEXT_ALIGN_HORIZ TextAlignHoriz)
{
    sGETDC().TextAlign.Horiz = TextAlignHoriz;
}

VOID EQS_API SetTextAlignVert(TEXT_ALIGN_VERT TextAlignVert)
{
    sGETDC().TextAlign.Vert = TextAlignVert;
}

//TEXT_ALIGN* EQS_API GetTextAlign(VOID)
//{
//    return &sGETDC().TextAlign;
//}
VOID EQS_API GetTextAlign(TEXT_ALIGN* pTextAlign)
{
    *pTextAlign = sGETDC().TextAlign;
    //return &sGETDC().TextAlign;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Text Wrapping                                                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API SetTextWrap(BOOL fTextWrap)
{
    sGETDC().fTextWrap = fTextWrap;
}

BOOL EQS_API GetTextWrap(VOID)
{
    return sGETDC().fTextWrap;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Text width and height                                                   */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

WORD EQS_API TextWidth(CONST BYTE* pszString)
{
    CONST BITMAP_FONT* pBitmapFont;
#ifdef FEATURE_EQS_VWF    
    BYTE bChar;
    WORD wCode, wLength;

    while((BOOL)(bChar = *pszString++))
    {
        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);

            wCode = (bChar << 8) | *pszString++;

            pBitmapFont = sGetBitmapFontByCode(wCode);
            //ASSERT(pBitmapFont);
            wLength += pBitmapFont->Size.xWidth;
        }
        else
        {
            pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
            //ASSERT(pBitmapFont);
            wLength += pBitmapFont->Size.xWidth;
        }
    }

    return wLength; 
#else
    /* fiexed width font */
    pBitmapFont = sGetBitmapFontByCode('A'); /* any character */
    //ASSERT(pBitmapFont);
    return pBitmapFont->Size.xWidth*STRLEN(pszString);
#endif /* FEATURE_EQS_VWF */
}

WORD EQS_API TextBufferWidth(CONST BYTE* pszString, UINT16 nLen)
{
    CONST BITMAP_FONT* pBitmapFont;
    //UINT16  i;
#ifdef FEATURE_EQS_VWF    
    BYTE bChar;
    WORD wCode, wLength;

    for(i = 0; i < nLen; i++)
    //while((BOOL)(bChar = *pszString++))
    {
        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);

            wCode = (bChar << 8) | *pszString++;

            pBitmapFont = sGetBitmapFontByCode(wCode);
            //ASSERT(pBitmapFont);
            wLength += pBitmapFont->Size.xWidth;
        }
        else
        {
            pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
            //ASSERT(pBitmapFont);
            wLength += pBitmapFont->Size.xWidth;
        }
    }

    return wLength; 
#else
    /* fiexed width font */
    pBitmapFont = sGetBitmapFontByCode('A'); /* any character */
    //ASSERT(pBitmapFont);
    return pBitmapFont->Size.xWidth*nLen;
#endif /* FEATURE_EQS_VWF */
}

WORD EQS_API TextHeight(CONST BYTE* pszString)
{
    CONST BITMAP_FONT* pBitmapFont;
#ifdef FEATURE_EQS_VWF
    WORD wHeight;
    BYTE bChar;
    WORD wCode;

    wHeight = 0;
    while((BOOL)(bChar = *pszString++))
    {
        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);

            wCode = (bChar << 8) | *pszString++;

            pBitmapFont = sGetBitmapFontByCode(wCode);
            //ASSERT(pBitmapFont);
            if(pBitmapFont->Size.yHeight > wHeight)
                wHeight = pBitmapFont->Size.yHeight;
        }
        else
        {
            pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
            //ASSERT(pBitmapFont);
            if(pBitmapFont->Size.yHeight > wHeight)
                wHeight = pBitmapFont->Size.yHeight;
        }
    }

    return wHeight; 
#else
    pBitmapFont = sGetBitmapFontByCode('A'); /* any character */
    //ASSERT(pBitmapFont);
    return pBitmapFont->Size.yHeight;
#endif /* FEATURE_EQS_VWF */
}

UINT8 EQS_API CharWidth(CONST BYTE bChar)
{
    CONST BITMAP_FONT* pBitmapFont;

    pBitmapFont = sGetBitmapFontByCode(bChar);
    //ASSERT(pBitmapFont);
    return (UINT8)pBitmapFont->Size.xWidth;
}

UINT8 EQS_API CharHeight(CONST BYTE bChar)
{
    CONST BITMAP_FONT* pBitmapFont;

    pBitmapFont = sGetBitmapFontByCode(bChar);
    //ASSERT(pBitmapFont);
    return (UINT8)pBitmapFont->Size.yHeight;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Draw text                                                               */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
#ifdef FEATURE_SKVM
CONST BITMAP_FONT* EQS_API EQS_GetBitmapFontByCode(WORD wCode){
	return sGetBitmapFontByCode(wCode);
}
#endif

LOCAL CONST BITMAP_FONT* EQS_API sGetBitmapFontByCode(WORD wCode)
{
    CONST BITMAP_FONT_SET* pBitmapFontSet;
    CONST BITMAP_FONT* pBitmapFont = NULL;

    /* Get bitmap font */
    pBitmapFontSet = sGetCurrentBitmapFontSet();
    ASSERT(pBitmapFontSet);
    /* ASCII */
    if(/*wCode >= 0x00 && */wCode <= 0xFF)
    {
        pBitmapFont = pBitmapFontSet->pASCIIBitmapFont;
    }
    /* KSC 5601 Hangul */
    else if(wCode >= 0xB0A1 && wCode <= 0xC9AA)
    {
        pBitmapFont = pBitmapFontSet->pHangulBitmapFont;
    }
    /* KSC 5601 special character code */
    else if(wCode >= 0xA1A1 && wCode <=0xACFE)
    {
        pBitmapFont = pBitmapFontSet->pSpecialBitmapFont;
    }
    /* SKY symbol code */
    else if(wCode >= 0x81A0 && wCode <= 0x89FF)
    {
        pBitmapFont = pBitmapFontSet->pSkySymbolBitmapFont;
    }
#ifdef FEATURE_EQS_HANJA
    /* Hanja */
    else if(wCode >= 0xCAA1 && wCode <=0xFDFE)
    {
        pBitmapFont = pBitmapFontSet->pHanjaBitmapFont;
    }
#endif /* FEATURE_EQS_HANJA */
    else
    {
        //ASSERT(pBitmapFont);
        /* invalid character code.. but... */
        pBitmapFont = pBitmapFontSet->pASCIIBitmapFont;
    }

    ASSERT(pBitmapFont);

    return pBitmapFont;
}

VOID EQS_API SetTextDrawMode(TEXT_DRAW_MODE TextDrawMode)
{
    /*
    ** the real value of TDM_NORMAL is 0. but when the value is set,
    ** we have to clear the all bits of TextDrawMode;
    */
    if(TextDrawMode == TDM_NORMAL) 
        sGETDC().TextDrawMode = TextDrawMode; /* clear all bit flags */
    else
        sGETDC().TextDrawMode |= TextDrawMode;
}

VOID EQS_API ClearTextDrawMode(TEXT_DRAW_MODE TextDrawMode)
{
    sGETDC().TextDrawMode &= ~TextDrawMode;
}

BYTE EQS_API GetTextDrawMode(VOID)
{
    return sGETDC().TextDrawMode;
}

VOID EQS_API TextOutStr(COORD x, COORD y, CONST BYTE* pszString)
{
    TEXT_ALIGN TextAlign;
    CONST BITMAP_FONT* pBitmapFont;
    BYTE bChar;
    WORD wCode;

    GetTextAlign(&TextAlign);
    if(TextAlign.Horiz == TAH_CENTER)
        x -= TextWidth(pszString)/2;
    else if(TextAlign.Horiz == TAH_RIGHT)
        x -= TextWidth(pszString);

    if(TextAlign.Vert == TAV_CENTER)
        y -= TextHeight(pszString)/2;
    else if(TextAlign.Vert == TAV_BOTTOM)
        y -= TextWidth(pszString);

    while((BOOL)(bChar = *pszString++))
    {
        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);
            if( *pszString == 0 )
			{
				bChar = ' ';
				pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
				if(GET_TEXT_WRAP() 
				   && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
				{
					x = 0;
					y += pBitmapFont->Size.yHeight;
				}
				sPutBitmapFont(x, y, (WORD)bChar, pBitmapFont);
			}
			else
			{			
				wCode = (bChar << 8) | *(pszString++);
				pBitmapFont = sGetBitmapFontByCode(wCode);

				if(GET_TEXT_WRAP() 
				   && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
				{
					x = 0;
					y += pBitmapFont->Size.yHeight;
				}
				sPutBitmapFont(x, y, wCode, pBitmapFont);
				x += pBitmapFont->Size.xWidth;
			}

        }
        else
        {
            pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
            if(GET_TEXT_WRAP() 
               && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
            {
                x = 0;
                y += pBitmapFont->Size.yHeight;
            }
            sPutBitmapFont(x, y, (WORD)bChar, pBitmapFont);
            x += pBitmapFont->Size.xWidth;
        }
    }
}

VOID EQS_API TextOutBufferLen(COORD x, COORD y, CONST BYTE* pszString, UINT16 nLen)
{
    TEXT_ALIGN TextAlign;
    CONST BITMAP_FONT* pBitmapFont;
    BYTE bChar;
    WORD wCode;
    UINT16 i;

    GetTextAlign(&TextAlign);
    if(TextAlign.Horiz == TAH_CENTER)
        x -= TextBufferWidth(pszString, nLen)/2;
    else if(TextAlign.Horiz == TAH_RIGHT)
        x -= TextBufferWidth(pszString, nLen);

    if(TextAlign.Vert == TAV_CENTER)
        y -= TextHeight(pszString)/2;
    else if(TextAlign.Vert == TAV_BOTTOM)
        y -= TextWidth(pszString);

    //while((BOOL)(bChar = *pszString++))
    for(i = 0; i < nLen; i++)
    {
        bChar = pszString[i];

        if(bChar & 0x80) /* multibyte character */
        {
            /* check valid multibyte character */
            //ASSERT(*pszString & 0x80);
            if( *pszString == 0 )
			{
				bChar = ' ';
				pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
				if(GET_TEXT_WRAP() 
				   && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
				{
					x = 0;
					y += pBitmapFont->Size.yHeight;
				}
				sPutBitmapFont(x, y, (WORD)bChar, pBitmapFont);
			}
			else
			{			
				wCode = (bChar << 8) | *(pszString++);
				pBitmapFont = sGetBitmapFontByCode(wCode);

				if(GET_TEXT_WRAP() 
				   && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
				{
					x = 0;
					y += pBitmapFont->Size.yHeight;
				}
				sPutBitmapFont(x, y, wCode, pBitmapFont);
				x += pBitmapFont->Size.xWidth;
			}

        }
        else
        {
            pBitmapFont = sGetBitmapFontByCode((WORD)bChar);
            if(GET_TEXT_WRAP() 
               && (x + pBitmapFont->Size.xWidth > GET_MAX_WIDTH()))
            {
                x = 0;
                y += pBitmapFont->Size.yHeight;
            }
            sPutBitmapFont(x, y, (WORD)bChar, pBitmapFont);
            x += pBitmapFont->Size.xWidth;
        }
    }
}

VOID EQS_API TextOutStrTDM(COORD x, COORD y, CONST BYTE* pszString, 
                           TEXT_DRAW_MODE TextDrawModeA)
{
    TEXT_DRAW_MODE TextDrawMode;
    
    TextDrawMode = GetTextDrawMode();
    SetTextDrawMode(TextDrawModeA);
    TextOutStr(x, y, pszString);
    SetTextDrawMode(TextDrawMode);
}

VOID EQS_API TextOutStrLen(COORD x, COORD y, CONST BYTE* pszString, INT8 nLength)
{
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    INT8 i;

    STRNCPY(szString, pszString, (SIZE_T)nLength);
    for(i = (INT8)STRLEN(pszString); i < nLength; i++)
        szString[i] = ' '; /* fill a space */
    szString[nLength] = '\0';

    TextOutStr(x, y, szString);
}

VOID EQS_API TextOutStrLenTDM(COORD x, COORD y, CONST BYTE* pszString, 
                              INT8 nLength, TEXT_DRAW_MODE TextDrawModeA)
{
    TEXT_DRAW_MODE TextDrawMode;
    
    TextDrawMode = GetTextDrawMode();
    SetTextDrawMode(TextDrawModeA);
    TextOutStrLen(x, y, pszString, nLength);
    SetTextDrawMode(TextDrawMode);
}

VOID EQS_API TextOutStrF(COORD x, COORD y, CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStr(x, y, szString);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutStrTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawModeA, 
                            CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrTDM(x, y, szString, TextDrawModeA);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutStrLenF(COORD x, COORD y, INT8 nLength, 
                            CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrLen(x, y, szString, nLength);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutStrLenTDMF(COORD x, COORD y, INT8 nLength, 
                               TEXT_DRAW_MODE TextDrawModeA, 
                               CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrLenTDM(x, y, szString, nLength, TextDrawModeA);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutBufferLenTDM(COORD x, COORD y, CONST BYTE* pszString, 
                           TEXT_DRAW_MODE TextDrawModeA, UINT16 nLen)
{
    TEXT_DRAW_MODE TextDrawMode;
    
    TextDrawMode = GetTextDrawMode();
    SetTextDrawMode(TextDrawModeA);
    TextOutBufferLen(x, y, pszString, nLen);
    SetTextDrawMode(TextDrawMode);
}


VOID EQS_API TextOut(COORD x, COORD y, EQS_TOKEN TokenId)
{
    TextOutStr(x, y, EQS_GET_TOKEN(TokenId));
}

VOID EQS_API TextOutTDM(COORD x, COORD y, EQS_TOKEN TokenId, 
                        TEXT_DRAW_MODE TextDrawMode)
{
    TextOutStrTDM(x, y, EQS_GET_TOKEN(TokenId), TextDrawMode);
}

VOID EQS_API TextOutLen(COORD x, COORD y, EQS_TOKEN TokenId, INT8 nLength)
{
    TextOutStrLen(x, y, EQS_GET_TOKEN(TokenId), nLength);
}

VOID EQS_API TextOutLenTDM(COORD x, COORD y, EQS_TOKEN TokenId, 
                           INT8 nLength, TEXT_DRAW_MODE TextDrawMode)
{
    TextOutStrLenTDM(x, y, EQS_GET_TOKEN(TokenId), nLength, TextDrawMode);
}

VOID EQS_API TextOutF(COORD x, COORD y, EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStr(x, y, szString);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawMode,
                         EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrTDM(x, y, szString, TextDrawMode);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutLenF(COORD x, COORD y, INT8 nLength, 
                         EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrLen(x, y, szString, nLength);
    
    va_end(vaArgPtr);
}

VOID EQS_API TextOutLenTDMF(COORD x, COORD y, INT8 nLength, 
                            TEXT_DRAW_MODE TextDrawMode,
                            EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    TextOutStrLenTDM(x, y, szString, nLength, TextDrawMode);
    
    va_end(vaArgPtr);
}

#ifdef FEATURE_GRAPH_DRAWTEXT

VOID EQS_API DrawText(RECT* pRect, TEXT_DRAW_FORMAT DrawFormat, 
                     BYTE* pszString)
{
}

VOID EQS_API DrawTextF(RECT* pRect, TEXT_DRAW_FORMAT DrawFormat, 
                      CONST BYTE* pszFormat, ...)
{
}

#endif /* FEATRUE_GRAPH_DRAWTEXT */


/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Coordinate                                                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API SetCoordinates(COORD_SYSTEM CoordSystem)
{
    POINT Origin = { 0, 0 };

    if(sGETVP().CoordSystem != CoordSystem)
        sGETVP().Origin.y = -sGETVP().Origin.y;
    sGETVP().CoordSystem = CoordSystem;
    
    SetOrigin(&Origin);
}

COORD_SYSTEM EQS_API GetCoordinates(VOID)
{
    return sGETVP().CoordSystem;
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Origin                                                                  */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

VOID EQS_API SetOrigin(POINT* pOrigin)
{
    sGETVP().Origin = *pOrigin;
}

//POINT* EQS_API GetOrigin(VOID)
VOID EQS_API GetOrigin(POINT* pOrigin)
{
    *pOrigin = sGETVP().Origin;
    //return &(sGETVP().Origin);
}


/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Device Position <--> Logical Postion                                    */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

/* convert a device position to a logical position */
COORD EQS_API DPx2LPx(COORD x)
{
    return x - (sGETVP().Rect.xLeft + sGETVP().Origin.x);
}

COORD EQS_API DPy2LPy(COORD y)
{
    if(GetCoordinates() == CS_MATH)
        return -(sGETVP().Origin.y) + sGETVP().Rect.yTop - y;
    else
        return y - (sGETVP().Rect.yTop + sGETVP().Origin.y);
}

/* convert a logical position to a device position */
COORD EQS_API LPx2DPx(COORD x)
{
    COORD xDP;
    
    xDP = x + sGETVP().Rect.xLeft + sGETVP().Origin.x;

//    ASSERT(xDP >= 0 && xDP < DEV_GET_MAX_WIDTH());

    return xDP;
}

COORD EQS_API LPy2DPy(COORD y)
{
    COORD yDP;

    if(GetCoordinates() == CS_MATH)
        yDP = sGETVP().Rect.yTop - (sGETVP().Origin.y + y);
    else
        yDP = sGETVP().Rect.yTop + sGETVP().Origin.y + y;

//    ASSERT(yDP >= 0 && yDP < DEV_GET_MAX_HEIGHT());
    
    return yDP;
}

/* convert a device position to a logical position */
VOID EQS_API DP2LP(POINT* pPos)
{
    pPos->x = pPos->x - (sGETVP().Rect.xLeft + sGETVP().Origin.x);
    if(GetCoordinates() == CS_MATH)
        pPos->y = -(sGETVP().Origin.y) + sGETVP().Rect.yTop - pPos->y;
    else
        pPos->y = pPos->y - (sGETVP().Rect.yTop + sGETVP().Origin.y);
}

/* convert a logical position to a device position */
VOID EQS_API LP2DP(POINT* pPos)
{
    pPos->x = pPos->x + sGETVP().Rect.xLeft + sGETVP().Origin.x;
    if(GetCoordinates() == CS_MATH)
        pPos->y = sGETVP().Rect.yTop - (sGETVP().Origin.y + pPos->y);
    else
        pPos->y = sGETVP().Rect.yTop + sGETVP().Origin.y + pPos->y;

//    ASSERT(pPos->x >= 0 && pPos->x < DEV_GET_MAX_WIDTH());
//    ASSERT(pPos->y >= 0 && pPos->y < DEV_GET_MAX_HEIGHT());
}

/* -- 화면 전체 이미지 밝게 ---> 256 칼라 전용
**
**  3-3-2
**  --- --- --
**  012 345 67
**  123 456 7-
**     AND
**  110 110 10
**  --- --- --
**  120 450 70
**
**  1) shift left by 1
**  2) clear bit by 0xDA (11011010)
*/
void EQS_API MakeScreenLighter(UINT8 nTimes)
{
#ifdef FEATURE_EQS_COLOR_256
    BYTE* pbBuffer = GetScreenBuffer();
    SIZE *pSize = GetScreenSize();
    DWORD dwTotalBufferSize;
    DWORD i;

	dwTotalBufferSize = pSize->xWidth * pSize->yHeight;
    while(nTimes--)
    {
        for (i = 0 ; i < dwTotalBufferSize; i++) 
        {
            *(pbBuffer+i) <<= 1;
            *(pbBuffer+i) &= 0xDA;
        }
    }
#elif defined FEATURE_EQS_COLOR_64K
    BYTE* pbBuffer = GetScreenBuffer();

    SIZE *pSize = GetScreenSize();
    DWORD dwTotalBufferSize;
    DWORD i;
    WORD wData;
    COLOR Red, Green, Blue;

	dwTotalBufferSize = pSize->xWidth * pSize->yHeight*2;
    while(nTimes--)
    {
        for (i = 0 ; i < dwTotalBufferSize; i+=2) 
        {
            wData = *(pbBuffer+i) << 8;
            wData |= *(pbBuffer+i+1);

            Red   = ((wData & 0xF800) >> 11) << 3;
            Green = ((wData & 0x07E0) >> 5) << 2;
            Blue  = ((wData & 0x001F)) << 3;

            Red <<= 1;
            Green <<= 1;
            Blue <<= 1;

            wData = EQS_RGB(Red, Green, Blue);

            *(pbBuffer+i)   = LOBYTE(wData);
            *(pbBuffer+i+1) = HIBYTE(wData);
        }
    }
#else
    UNUSED(nTimes);
#endif //#ifdef FEATURE_EQS_COLOR_256
}

/* -- 화면 전체 이미지 어둡게 ---> 256칼라 전용
**
**  3-3-2
**  --- --- --
**  012 345 67
**  -01 234 56
**     AND
**  011 011 01
**  --- --- --
**  023 056 08
**
**  1) shift right by 1
**  2) clear bit by 0x6D (01101101)
*/

/* -- 화면 전체 이미지 어둡게 ---> 64K 칼라 전용
**
**  5-6-5
**  ----- ------ -----
**  01234 567890 12345
**  -0123 456789 01234
**     AND
**  01111 011111 01111
**  ----- ------ -----
**
**  1) shift right by 1
**  2) clear bit by 0x7BEF (01111011 11101111)
*/
void EQS_API MakeScreenDarker(UINT8 nTimes)
{
#ifdef FEATURE_EQS_COLOR_256
    BYTE* pbBuffer = GetScreenBuffer();

    SIZE *pSize = GetScreenSize();
    DWORD dwTotalBufferSize;
    DWORD i;

	dwTotalBufferSize = pSize->xWidth * pSize->yHeight;
    while(nTimes--)
    {
        for (i = 0 ; i < dwTotalBufferSize; i++) 
        {
            *(pbBuffer+i) >>= 1;
            *(pbBuffer+i) &= 0x6D;
        }
    }
#elif defined FEATURE_EQS_COLOR_64K
    BYTE* pbBuffer = GetScreenBuffer();

    SIZE *pSize = GetScreenSize();
    DWORD dwTotalBufferSize;
    DWORD i;
    WORD wData;
    COLOR Red, Green, Blue;

	dwTotalBufferSize = pSize->xWidth * pSize->yHeight*2;
    while(nTimes--)
    {
        for (i = 0 ; i < dwTotalBufferSize; i+=2) 
        {
            wData = *(pbBuffer+i) << 8;
            wData |= *(pbBuffer+i+1);

            Red   = ((wData & 0xF800) >> 11) << 3;
            Green = ((wData & 0x07E0) >> 5) << 2;
            Blue  = ((wData & 0x001F)) << 3;

            Red >>= 1;
            Green >>= 1;
            Blue >>= 1;

            wData = EQS_RGB(Red, Green, Blue);

            *(pbBuffer+i)   = LOBYTE(wData);
            *(pbBuffer+i+1) = HIBYTE(wData);
        }
    }
#else
    UNUSED(nTimes);
#endif //#ifdef FEATURE_EQS_COLOR_256
}

