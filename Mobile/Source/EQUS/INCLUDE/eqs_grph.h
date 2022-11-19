/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_grph.h
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
** 2000-06-27 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQS_GRPH_H_
#define _EQS_GRPH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* Graphics system version 1.0 */
#define EQS_GRAPH_VERSION           0x0100  /* 1.0 */

#define INIT_FONT_TYPE              FT_BIG//FT_MED

/* image / animation id */
#define EQUS_ID_LENGTH              3
#define EQUS_IMAGE_ID               { 'I', 'M', 'G' }//"IMG"
#define EQUS_ANIMATION_ID           { 'A', 'N', 'I' }//"ANI"
#define EQUS_IMAGE_ID_STR           "IMG"
#define EQUS_ANIMATION_ID_STR       "ANI"

#define IS_EQUS_IMAGE(pBitmapHeader)        ((pBitmapHeader)->abID[0] == 'I' && (pBitmapHeader)->abID[1] == 'M' && (pBitmapHeader)->abID[2] == 'G')
#define IS_EQUS_ANIMATION(pBitmapHeader)    ((pBitmapHeader)->abID[0] == 'A' && (pBitmapHeader)->abID[1] == 'N' && (pBitmapHeader)->abID[2] == 'I')

/* packed image / animation id */
#define EQUS_X_IMAGE_ID               { 'X', 'M', 'G' }//"IMG"
#define EQUS_X_ANIMATION_ID           { 'X', 'N', 'I' }//"ANI"
#define EQUS_X_IMAGE_ID_STR           "XMG"
#define EQUS_X_ANIMATION_ID_STR       "XNI"

#define IS_EQUS_X_IMAGE(pBitmapHeader)        ((pBitmapHeader)->abID[0] == 'X' && (pBitmapHeader)->abID[1] == 'M' && (pBitmapHeader)->abID[2] == 'G')
#define IS_EQUS_X_ANIMATION(pBitmapHeader)    ((pBitmapHeader)->abID[0] == 'X' && (pBitmapHeader)->abID[1] == 'N' && (pBitmapHeader)->abID[2] == 'I')

/* LZB packed image / animation id  - haenguen  2001-09-26 --> */
#define EQUS_Z_IMAGE_ID               { 'Z', 'M', 'G' }//"IMG"
#define EQUS_Z_ANIMATION_ID           { 'Z', 'N', 'I' }//"ANI"
#define EQUS_Z_IMAGE_ID_STR           "ZMG"
#define EQUS_Z_ANIMATION_ID_STR       "ZNI"

#define IS_EQUS_Z_IMAGE(pBitmapHeader)        ((pBitmapHeader)->abID[0] == 'Z' && (pBitmapHeader)->abID[1] == 'M' && (pBitmapHeader)->abID[2] == 'G')
#define IS_EQUS_Z_ANIMATION(pBitmapHeader)    ((pBitmapHeader)->abID[0] == 'Z' && (pBitmapHeader)->abID[1] == 'N' && (pBitmapHeader)->abID[2] == 'I')
/* <-- */

/* file extension */
#define EQUS_IMAGE_EXT              ".EBI"
#define EQUS_ANIMATION_EXT          ".EBA"
/* packed file extension */
#define EQUS_X_IMAGE_EXT            ".XBI"
#define EQUS_X_ANIMATION_EXT        ".XBA"
/* LZB packed file extension - haengeun  2001-09-26 --> */
#define EQUS_Z_IMAGE_EXT            ".ZBI"
#define EQUS_Z_ANIMATION_EXT        ".ZBA"

#ifdef ABS
    #undef ABS
#endif
#define ABS(a)                      ((BOOL)(((a) > 0) ? (a) : -(a)))
#define ROUND(a)                    ((INT)((a)+0.5))

#define SIZE_OF_ARRAY(a)            (sizeof(a)/sizeof(a[0]))
#define PIXEL2BYTES(n)              (((n)+7)/8)

#define BIT_PER_PIXEL_BW            1
#define BIT_PER_PIXEL_4G            2
#define BIT_PER_PIXEL_256C          8
#define BIT_PER_PIXEL_64KC          16

/* the maximun string length in TextOut */
#define EQS_TO_MAX_STRLEN                 24 /* MaxSceenWidth/SmallestFontWidth */

/* Text Draw Mode -- TEXT_DRAW_MODE */
#define TDM_NORMAL                    0x00
#define TDM_REVERSE                   0x01
#define TDM_BOLD                      0x02
#define TDM_UNDERLINE                 0x04
#define TDM_GRAY                      0x08
#define TDM_SHADOW                    0x10
#define TDM_OUTLINE                   0x20
#define TDM_BLINK                     0x40    
#define TDM_ALL                       0xFF  /* all text draw mode */

/* ColorSpace means BitsPerPixel of the graphic device. */
#define GetScreenColorSpace           GetScreenBitsPerPixel

#define ClearTextAlign()              SetTextAlignHoriz(TAH_LEFT); \
                                      SetTextAlignVert(TAV_TOP)

#define GET_IMAGE_BYTE_SIZE(BmpHeader) ((BmpHeader).Size.xWidth * (BmpHeader).Size.yHeight * PIXEL2BYTES((BmpHeader).nBitsPerPixel))

/*
** The initial system value of EQUS graphic system 
** If you want to change the default values,
** change following values
*/
/* Coordinate System */
#define INIT_CS                     CS_SCREEN
/* Text Align */
#define INIT_TAH                    TAH_LEFT
#define INIT_TAV                    TAV_TOP
/* Color */
#define INIT_FORECOLOR              WHITE
#define INIT_BACKCOLOR              BLACK
/* Draw Mode */
#define INIT_DRAWMODE               DM_COPY
/* Text */
//#define INIT_TEXTCOLOR              INIT_FORECOLOR
//#define INIT_TEXTBACKCOLOR          BLACK
#define INIT_TEXTDRAWMODE           TDM_NORMAL
/* Font Type */
//#define INIT_FONT_TYPE              FT_MED
/* Character Space inter Character */
#define INIT_INTERCHARSPACE         0
/* Text Wrap */
#define INIT_TEXTWRAP               FALSE;

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef BYTE                          TEXT_DRAW_MODE;

enum tagSCREEN_INDEX_E {
    SI_MAIN_SCREEN,
    SI_SUB_SCREEN,
    SI_MAX_SCREEN
};
typedef enum tagSCREEN_INDEX_E       SCREEN_INDEX_E;

/* COLOR_SPACE is a friendly name. we use this value as bits per pixel */
enum tagCOLOR_SPACE {
    CS_BW           = 1,           /* black and white */
    CS_4GRAY        = 2,           /* 4 grays */
    CS_16COLOR      = 4,           /* 16 colors */
    CS_256COLOR     = 8,            /* 256 colors */
    CS_64KCOLOR     = 16
};
/* typedef enum */
typedef enum tagCOLOR_SPACE         COLOR_SPACE;

#define RGB332(r,g,b)       ((((r)/32) << 5) | (((g)/32) << 2) | ((b)/64))
#define RGB565(r,g,b)       ((((r)/8) << 11) | (((g)/4) << 5) | ((b)/8))

#ifdef FEATURE_EQS_COLOR_256
#define EQS_RGB         RGB332
#elif defined FEATURE_EQS_COLOR_64K
#define EQS_RGB         RGB565
#endif /* FEATURE_EQS_COLOR_XXX */

#define MAX_COLOR_2         2
#define MAX_COLOR_4         4
#define MAX_COLOR_256       256
#define MAX_COLOR_64K       65536

/* 
** 4 gray LCD has a light screen in the power off state. 
** Turning on all color bits means BLACK in 4 gray, 
** but it means WHITE in any other systems.
** For keeping consistence, I'd rather use the same color 
** policies in all of the systems. 
** In short, we can use same color both 4 gray, 16 color and 256 color systems.
** And we have to use following policies in the 4 gray color system.
** WHITE means black dot in the 4 gray LCD screen. 
** BLACK means no color in the LCD screen.
*/
enum tagCOLOR {
#if defined(FEATURE_EQS_COLOR_BW)
    WHITE = 1,
    BLACK = 0,
#elif defined(FEATURE_EQS_COLOR_4GRAY)
    BLACK        = 0,       /* -    */
    BLUE         = 0,
    GREEN        = 0,
    CYAN         = 0,        
    
    RED          = 1,
    MAGENTA      = 1,
    BROWN        = 1,
    GRAY         = 1,       /* --   */
    
    LIGHTGRAY    = 2,       /* ---  */
    LIGHTBLUE    = 2,
    LIGHTGREEN   = 2,
    LIGHTCYAN    = 2,        

    LIGHTRED     = 3, 
    LIGHTMAGENTA = 3,
    YELLOW       = 3,
    WHITE        = 3,        /* ---- */
#elif defined(FEATURE_EQS_COLOR_16)
    BLACK        = 0,
    BLUE         = 1,
    GREEN        = 2,
    CYAN         = 3,        
    RED          = 4,
    MAGENTA      = 5,
    BROWN        = 6,
    GRAY         = 7,
    LIGHTGRAY    = 8,
    LIGHTBLUE    = 9,
    LIGHTGREEN   = 10,
    LIGHTCYAN    = 11,        
    LIGHTRED     = 12,
    LIGHTMAGENTA = 13,
    YELLOW       = 14,
    WHITE        = 15,   
#elif defined(FEATURE_EQS_COLOR_256)
    BLACK        = RGB332(0,0,0),       /* 0,0,0 */
    BLUE         = RGB332(0,0,255),     /* 0,0,255 */
    GREEN        = RGB332(0,255,0),     /* 0,255,0 */
    CYAN         = RGB332(0,255,255),   /* 0,255,255 */     
    RED          = RGB332(255,0,0),     /* 255,0,0 */
    MAGENTA      = RGB332(255,0,255),   /* 255,0,255*/
    BROWN        = RGB332(128,0,0),     /* 128,0,0 */
    GRAY         = RGB332(128,128,128), /* 128,128,128 */
    LIGHTGRAY    = RGB332(192,192,192), /* 192,192,192 */
    LIGHTBLUE    = RGB332(64,64,255),   /* 64,64,255 */
    LIGHTGREEN   = RGB332(64,255,64),   /* 64,255,64 */
    LIGHTCYAN    = RGB332(64,255,255),  /* 64,255,255 */
    LIGHTRED     = RGB332(255,64,64),   /* 255,64,64 */
    LIGHTMAGENTA = RGB332(255,64,255),  /* 255,64,255 */
    YELLOW       = RGB332(255,255,0),   /* 255,255,0 */
    WHITE        = RGB332(255,255,255), /* 255,255,255 */
#elif defined(FEATURE_EQS_COLOR_64K)
    BLACK        = RGB565(0,0,0),       /* 0,0,0 */
    BLUE         = RGB565(0,0,255),     /* 0,0,255 */
    GREEN        = RGB565(0,255,0),     /* 0,255,0 */
    CYAN         = RGB565(0,255,255),   /* 0,255,255 */     
    RED          = RGB565(255,0,0),     /* 255,0,0 */
    MAGENTA      = RGB565(255,0,255),   /* 255,0,255*/
    BROWN        = RGB565(128,0,0),     /* 128,0,0 */
    GRAY         = RGB565(128,128,128), /* 128,128,128 */
    LIGHTGRAY    = RGB565(192,192,192), /* 192,192,192 */
    LIGHTBLUE    = RGB565(64,64,255),   /* 64,64,255 */
    LIGHTGREEN   = RGB565(64,255,64),   /* 64,255,64 */
    LIGHTCYAN    = RGB565(64,255,255),  /* 64,255,255 */
    LIGHTRED     = RGB565(255,64,64),   /* 255,64,64 */
    LIGHTMAGENTA = RGB565(255,64,255),  /* 255,64,255 */
    YELLOW       = RGB565(255,255,0),   /* 255,255,0 */
    WHITE        = RGB565(255,255,255), /* 255,255,255 */
#endif
    MAX_COLOR
};
typedef enum tagCOLOR                   COLOR;

enum tagDRAW_MODE {
    DM_COPY,    /* copy bit image to screen as it is */
    DM_OR,      /* inclusive or bit image and screen */
    DM_XOR      /* exclusive or bit image and screen */
    //DM_AND,     /* and bit image and screen */
    //DM_NOT      /* Copy inverse of bitmap to screen */
};
typedef enum tagDRAW_MODE               DRAW_MODE;

/*
** ***************************************************************************
** Font and Text
** ***************************************************************************
**
** FONT SIZE
**
**   10 x 21 : BIG BIG FONT (ascii only)
**   14 x 16 : BIG FONT (hangul, special, sky symbol)
**    7 x 16 : BIG FONT (ascii)
**   12 x 13 : SYSTEM FONT (hangul, special, sky symbol)
**    6 x 13 : SYSTEM FONT (ascii)
**
** tagFONT is using the index for BitmapFontSet[] array. it should be 0 based.
*/
enum tagFONT_TYPE {
    FT_BIGBIG,             /* ASCII : 10 x 21  HANGUL : NO */
    FT_BIG,                /* ASCII :  7 x 16  HANGUL : 14 x 16 */
    FT_MED,                /* ASCII :  6 x 13  HANGUL : 12 x 13 */
    FT_SMALL,              /* ASCII :  5 X  9  HANGUL : NO */
    /* --> INSERT HERE for the new font type with completion hangul font type */
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT
    /* using the compound type Hangul font */
    FT_BIG_COMP_BATANG,    /* ASCII :  7 x 16  HANGUL : 14 x 16 */
    FT_BIG_COMP_DODUM,     /* ASCII :  7 x 16  HANGUL : 14 x 16 */
    //FT_BIG_COMP_GULIM,     /* ASCII :  7 x 16  HANGUL : 14 x 16 */
    //FT_MED_COMP_DODUM,     /* ASCII :  6 x 13  HANGUL : 12 x 13 */
    /* --> INSERT HERE for the new font type with compound type hangul font */
#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */
    FT_MAX
};
typedef enum tagFONT_TYPE               FONT_TYPE;

enum tagTEXT_ALIGN_HORIZ {
    TAH_LEFT,
    TAH_CENTER,
    TAH_RIGHT
};
typedef enum tagTEXT_ALIGN_HORIZ        TEXT_ALIGN_HORIZ;

enum tagTEXT_ALIGN_VERT {
    TAV_TOP,
    TAV_CENTER,
    TAV_BOTTOM
};
typedef enum tagTEXT_ALIGN_VERT         TEXT_ALIGN_VERT;

#ifdef FEATURE_EQS_GRAPH_DRAWTEXT
enum tagTEXT_DRAW_FORMAT {
    TDF_CENTER, 
    TDF_LEFT,
    TDF_RIGHT,
    TDF_VCENTER,
    TDF_TOP,
    TDF_BOTTOM
};
typedef enum tagTEXT_DRAW_FORMAT        TEXT_DRAW_FORMAT;
#endif /* FEATRUE_GRAPH_DRAWTEXT */

/* coordinates system */
/* ************************************************************************ *

       |                       y                    
       |Origin                 ^                    
     --+----------> x          |                    
       |                       |                    
       |                       |Origin              
       |                     --+----------> x       
       v                       |                    
                               |    y               
   < CS_SCREEN >                 < CS_MATH >            

 * ************************************************************************ */

enum tagCOORD_SYSTEM {
    CS_SCREEN,
    CS_MATH         
};
typedef enum tagCOORD_SYSTEM            COORD_SYSTEM;

//enum tagCLEAR_SCREEN_TYPE {
//    CS_FILL_NULL,    /* clear the screen buffer by NULL */
//    CS_FILL_BGCOLOR  /* fill the screen buffer by the background color */
//};
//typedef enum tagCLEAR_SCREEN_TYPE       CLEAR_SCREEN_TYPE;

enum tagBITMAP_TYPE {
    BT_IMAGE,
    BT_FONT
};
typedef enum tagBITMAP_TYPE             BITMAP_TYPE;

enum tagFONT_KIND {
    FK_ASCII,
    FK_HANGUL,
#ifdef FEATURE_EQS_HANJA
    FK_HANJA,
#endif /* FEATURE_EQS_HANJA */
    FK_SKYSYMBOL
};
typedef enum tagFONT_KIND    FONT_KIND;


typedef INT16                           COORD;      

typedef struct tagPOINT_T               POINT;
typedef struct tagSIZE_T                SIZE;
typedef struct tagRECT_T                RECT;
typedef struct tagTEXT_ALIGN            TEXT_ALIGN;
typedef struct tagSCREEN                SCREEN;
typedef struct tagSCREEN_INFO           SCREEN_INFO;
typedef struct tagVIEW_PORT             VIEW_PORT;

typedef struct tagBITMAP_HEADER         BITMAP_HEADER;
/* 
** I don't want use tagBITMAP_T instead of tagBITMAP 
** But, VC++ debugger confuses the tagBITMAP of that of Windows GDI 
*/
typedef struct tagBITMAP_T              BITMAP;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagPOINT_T {
    COORD   x;
    COORD   y;
};

struct tagSIZE_T {
    COORD   xWidth;
    COORD   yHeight;
};

struct tagRECT_T {
    COORD   xLeft;
    COORD   yTop;
    COORD   xRight;
    COORD   yBottom;
};

struct tagTEXT_ALIGN {
    TEXT_ALIGN_HORIZ Horiz;
    TEXT_ALIGN_VERT  Vert;
};

struct tagSCREEN_INFO {
    COLOR_SPACE     ColorSpace;         /* color space - bits per pixel */
    SIZE            Size;               /* screen size */
    SIZE            BufferSize;         /* buffer size */
};
    
struct tagSCREEN {
    SCREEN_INFO     Info;               /* screen info */
    BYTE*           pbBuffer;           /* screen buffer */
};

struct tagVIEW_PORT {
    RECT            Rect;
    COORD_SYSTEM    CoordSystem;
    POINT           Origin;
};

#if 0 /* 0000000000000000000000000000000000000000000000000000000000000000000 */
struct tagBITMAP_HEADER {
    /* ------------------
    ** bitmap id 
    ** IMG : bitmap image
    ** ANI : animation bitmap image
    ** XMG : packed bitmap image
    ** XNI : packed animation image
    */ 
    BYTE abID[EQUS_ID_LENGTH];
    /* ------------------
    ** Size.xWidth must be a multiple of 8 in the black and white image
    ** and be a multiple of 4 in the 4 gray image
    */
    SIZE Size;          /* image buffer size */
    /* 
    ** the real width of the font image doesn't match its bitmap image size 
    ** if the bitmap type is BT_IMAGE, just you can ignore this.
    */
    COORD xWidth;       /* the real width of image not buffer size */
    INT8 nBitsPerPixel;
    BOOL fPalette;      /* use palette ? */
    COLOR TransparentColor; /* transparent color, MAX_COLOR means no transparent color */
    COORD yStart;       /* Start y position. default : 0 */
    BYTE abReserved[32]; /* reserved area */
};
#endif /* 000000000000000000000000000000000000000000000000000000000000000000 */

/*
** DO NOT USE enum type in the file header.
** The size of enum type is dependent to the system
*/
struct tagBITMAP_HEADER {
    /* ------------------
    ** bitmap id 
    ** IMG : bitmap image
    ** ANI : animation bitmap image
    ** XMG : packed bitmap image
    ** XNI : packed animation image
    */ 
    /* 4 bytes */
    BYTE abID[EQUS_ID_LENGTH+1];
    /* ------------------
    ** Size.xWidth must be a multiple of 8 in the black and white image
    ** and be a multiple of 4 in the 4 gray image
    */
    /* SIZE - COORD xWidth, COORD yHeight 
    ** COORD - INT16
    ** 4 bytes
    */
    SIZE Size;          /* image buffer size */
    /* 
    ** the real width of the font image doesn't match its bitmap image size 
    ** if the bitmap type is BT_IMAGE, just you can ignore this.
    */
    /* 2 bytes */
    COORD xWidth;       /* the real width of image not buffer size */
    /* 1 byte */
    INT8 nBitsPerPixel;
    /* BOOL - char 
    ** 1 byte 
    */
    BOOL fPalette;      /* use palette ? */
    /* 2 bytes */    
    WORD TransparentColor; /* transparent color, MAX_COLOR means no transparent color */
    /* 2 bytes */
    COORD yStart;       /* Start y position. default : 0 */
    /* ---------------------------------------------------------- 16 bytes -- */
    /* 32 byte */
    BYTE abReserved[32]; /* reserved area */
};

struct tagBITMAP_T { 
    BITMAP_HEADER Header;
    CONST BYTE* pbData;
    CONST COLOR* pbPalette;        /* palette informations */
};

struct tagDEVICE_CONTEXT {
    VIEW_PORT      ViewPort;
    
    COLOR          BackColor;           /* background color */
    COLOR          ForeColor;           /* foreground color */
    POINT          CurrentPosition;     /* current position */

    DRAW_MODE      DrawMode;            /* draw mode */

    COLOR          TextColor;           /* text color */
    COLOR          TextBackColor;       /* text background color */
    INT8           nInterCharSpace;     /* intercharacter space */
    TEXT_ALIGN     TextAlign;           /* text align */
    TEXT_DRAW_MODE TextDrawMode;        /* text draw mode */
    BOOL           fTextWrap;           /* wrap the text */

    FONT_TYPE      FontType;            /* font type */
};
typedef struct tagDEVICE_CONTEXT        DEVICE_CONTEXT;

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
#ifdef FEATURE_EQS_COMPOUND_HANGUL_FONT 
/* use this table in the Hangul input system */
EQS_DCL ROM WORD awKSCodeTable[];
#endif /* FEATURE_EQS_COMPOUND_HANGUL_FONT */

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

/* ************************************************************************ */
/* Graphics system                                                         */
/* ************************************************************************ */
EQS_DCL EQS_RESULT   EQS_API OpenGraph(SCREEN* pScreen, SCREEN_INDEX_E ScreenIndex, FONT_TYPE FontType);
EQS_DCL EQS_RESULT   EQS_API CloseGraph(SCREEN_INDEX_E ScreenIndex);
EQS_DCL SCREEN_INDEX_E EQS_API SelectScreen(SCREEN_INDEX_E ScreenIndex);
EQS_DCL SCREEN_INDEX_E EQS_API GetCurrentScreen(void);
EQS_DCL void         EQS_API SetScreenChangable(BOOL fChangable);
EQS_DCL BOOL         EQS_API IsScreenChangable(void);

/* ************************************************************************ */
/* Screen                                                                   */
/* ************************************************************************ */
EQS_DCL BYTE*        EQS_API GetScreenBuffer(VOID);
//EQS_DCL VOID         EQS_API ClearScreen(CLEAR_SCREEN_TYPE ClearType);
EQS_DCL VOID         EQS_API ClearScreen(VOID);
EQS_DCL VOID         EQS_API ClearViewPort(VOID);
EQS_DCL VOID         EQS_API ClearRegion(COORD xLeft, COORD yTop, 
                                         COORD xRight, COORD yBottom);

/* ************************************************************************ */
/* Sliding window                                                           */
/* ************************************************************************ */
#ifdef FEATURE_EQS_SLIDING_WINDOW
EQS_DCL VOID         EQS_API SetSlidingWindowState(BOOL fEnable);
EQS_DCL BOOL         EQS_API GetSlidingWindowState(VOID);
#if defined(FEATURE_EQS_SLIDING_WINDOW_X) \
    && defined(FEATURE_EQS_SLIDING_WINDOW_Y)
EQS_DCL VOID         EQS_API SetSlidingWindowOffset(POINT* pOffset);
EQS_DCL POINT*       EQS_API GetSlidingWindowOffset(VOID);
#endif /* defined(FEATURE_EQS_SLIDING_WINDOW_X) \
    && defined(FEATURE_EQS_SLIDING_WINDOW_Y) */
#ifdef FEATURE_EQS_SLIDING_WINDOW_X
EQS_DCL VOID         EQS_API SetSlidingWindowOffsetX(COORD xOffset);
EQS_DCL COORD        EQS_API GetSlidingWindowOffsetX(VOID);
#endif /* FEATURE_EQS_SLIDING_WINDOW_X */
#ifdef FEATURE_EQS_SLIDING_WINDOW_Y
EQS_DCL VOID         EQS_API SetSlidingWindowOffsetY(COORD yOffset);
EQS_DCL COORD        EQS_API GetSlidingWindowOffsetY(VOID);
#endif /* FEATURE_EQS_SLIDING_WINDOW_Y */
#endif /* FEATURE_EQS_SLIDING_WINDOW */

/* ************************************************************************ */
/* Viewport and screen size                                                */
/* ************************************************************************ */
//EQS_DCL VIEW_PORT*   EQS_API GetViewPort(VOID);
EQS_DCL VOID         EQS_API GetViewPort(VIEW_PORT* pViewPort);
EQS_DCL VOID         EQS_API SetViewPort(VIEW_PORT* pViewPort);
EQS_DCL VOID         EQS_API ResetViewPort(VOID);
//EQS_DCL RECT*        EQS_API GetViewPortRect(VOID);
EQS_DCL VOID         EQS_API GetViewPortRect(RECT* pRect);
EQS_DCL VOID         EQS_API SetViewPortRect(RECT* pRect);
EQS_DCL COORD        EQS_API GetMaxWidth(VOID);
EQS_DCL COORD        EQS_API GetMaxHeight(VOID);
EQS_DCL COORD        EQS_API GetMaxX(VOID);
EQS_DCL COORD        EQS_API GetMaxY(VOID);
EQS_DCL COORD        EQS_API DevGetMaxWidth(VOID);
EQS_DCL COORD        EQS_API DevGetMaxHeight(VOID);
EQS_DCL COORD        EQS_API DevGetMaxX(VOID);
EQS_DCL COORD        EQS_API DevGetMaxY(VOID);
EQS_DCL INT8         EQS_API GetScreenBitsPerPixel(VOID);
EQS_DCL SIZE*        EQS_API GetScreenSize(VOID);

/* ************************************************************************ */
/* Current position                                                        */
/* ************************************************************************ */
EQS_DCL POINT*       EQS_API DevGetCurrentPosition(VOID);
EQS_DCL POINT*       EQS_API GetCurrentPosition(VOID);
EQS_DCL COORD        EQS_API DevGetCurrentPositionX(VOID);
EQS_DCL COORD        EQS_API DevGetCurrentPositionY(VOID);
EQS_DCL COORD        EQS_API GetCurrentPositionX(VOID);
EQS_DCL COORD        EQS_API GetCurrentPositionY(VOID);

/* ************************************************************************ */
/* Color                                                                   */
/* ************************************************************************ */
EQS_DCL COLOR        EQS_API GetForeColor(VOID);
EQS_DCL COLOR        EQS_API GetBackColor(VOID);
EQS_DCL COLOR        EQS_API GetTextColor(VOID);
EQS_DCL COLOR        EQS_API GetTextBackColor(VOID);
EQS_DCL VOID         EQS_API SetForeColor(COLOR ForeColor);
EQS_DCL VOID         EQS_API SetBackColor(COLOR BackColor);
EQS_DCL VOID         EQS_API SetTextColor(COLOR TextColor);
EQS_DCL VOID         EQS_API SetTextBackColor(COLOR TextColor);
EQS_DCL VOID         EQS_API SetDrawMode(DRAW_MODE DrawMode);
EQS_DCL DRAW_MODE    EQS_API GetDrawMode(VOID);


/* ************************************************************************ */
/* Primitive shapes                                                        */
/* ************************************************************************ */
EQS_DCL COLOR        EQS_API DevGetPixel(COORD x, COORD y);
EQS_DCL COLOR        EQS_API GetPixel(COORD x, COORD y);
EQS_DCL VOID         EQS_API DevSetPixel(COORD x, COORD y, COLOR Color);
EQS_DCL VOID         EQS_API SetPixel(COORD x, COORD y, COLOR Color);
EQS_DCL VOID         EQS_API MoveTo(COORD x, COORD y);
EQS_DCL VOID         EQS_API MoveRel(COORD dx, COORD dy);
EQS_DCL VOID         EQS_API Line(COORD xStart, COORD yStart, 
                                  COORD xEnd, COORD yEnd);
EQS_DCL VOID         EQS_API LineTo(COORD x, COORD y);
EQS_DCL VOID         EQS_API LineRel(COORD dx, COORD dy);
EQS_DCL VOID         EQS_API Rectangle(COORD xLeft, COORD yTop, 
                                     COORD xRight, COORD yBottom);
EQS_DCL VOID         EQS_API RoundRectangle(COORD xLeft, COORD yTop, 
                                     COORD xRight, COORD yBottom,
                                     INT16 nRadius);
EQS_DCL VOID         EQS_API Circle(COORD xCenter, COORD yCenter, INT16 nRadius);
EQS_DCL VOID         EQS_API Ellipse(COORD xCenter, COORD yCenter, 
                                     COORD xRadius, COORD yRadius);
EQS_DCL VOID         EQS_API Polyline(POINT* pPoint, WORD wCount);
EQS_DCL VOID         EQS_API FillRectangle(COORD xLeft, COORD yTop, COORD xRight, 
                                           COORD yBottom, COLOR FillColor);

#ifdef FEATURE_EQS_GRAPH_ENHANCED
EQS_DCL VOID         EQS_API Arc(VOID);
EQS_DCL VOID         EQS_API ArcTo(VOID);
EQS_DCL VOID         EQS_API Pie(VOID);
EQS_DCL VOID         EQS_API Sector(VOID);
EQS_DCL VOID         EQS_API SetFillStyle(VOID);
EQS_DCL VOID         EQS_API SetFillPattern(VOID);
EQS_DCL VOID         EQS_API FillCircle(COORD xCenter, COORD yCenter, 
                                        INT16 nRadius, COLOR FillColor);
EQS_DCL VOID         EQS_API FillEllipse(VOID);
EQS_DCL VOID         EQS_API FillPoly(VOID);
#endif /* FEATURE_EQS_GRAPH_ENHANCED */

extern void SetPaletteColor4G(BITMAP *pBitmap, CONST COLOR *pColor);

/* ************************************************************************ */
/* Save and restore region                                                 */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API GetRegionImage(COORD xLeft, COORD yTop, 
                                          COORD xRight, COORD yBottom, 
                                          BYTE* pbBuffer);
EQS_DCL VOID         EQS_API PutRegionImage(COORD xLeft, COORD yTop, 
                                          BYTE* pbBuffer);
EQS_DCL WORD         EQS_API GetRegionImageSize(COORD xLeft, COORD yTop, 
                                        COORD xRight, COORD yBottom);
EQS_DCL VOID         EQS_API GetRegionImageBuffer(COORD xLeft, COORD yTop, 
                                                  COORD xRight, 
                                                  COORD yBottom, 
                                                  BYTE** ppbBuffer);
EQS_DCL VOID         EQS_API GetRegionImageRect(RECT* pRect,
                                                BYTE* pbBuffer);
EQS_DCL WORD         EQS_API GetRegionImageSizeRect(RECT* pRect);
EQS_DCL VOID         EQS_API GetRegionImageBufferRect(RECT* pRect,
                                                      BYTE** ppbBuffer);

/* ************************************************************************ */
/* Draw bitmap image                                                       */
/* ************************************************************************ */
//EQS_DCL VOID         EQS_API PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
EQS_DCL VOID         EQS_API (*PutBitmap)(COORD x, COORD y, CONST BITMAP* pBitmap);
EQS_DCL VOID         EQS_API (*PutBitmapRect)(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

/* ************************************************************************ */
/* Font                                                                    */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetFontType(FONT_TYPE FontType);
EQS_DCL FONT_TYPE    EQS_API GetFontType(VOID);
EQS_DCL CONST SIZE*  EQS_API GetFontSize(FONT_TYPE FontType, FONT_KIND FontKind);

/* ************************************************************************ */
/* Text align                                                               */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetTextAlign(TEXT_ALIGN* pTextAlign);
EQS_DCL VOID         EQS_API SetTextAlignHoriz(TEXT_ALIGN_HORIZ TextAlignHoriz);
EQS_DCL VOID         EQS_API SetTextAlignVert(TEXT_ALIGN_VERT TextAlignVert);
//EQS_DCL TEXT_ALIGN*  EQS_API GetTextAlign(VOID);
EQS_DCL VOID         EQS_API GetTextAlign(TEXT_ALIGN* pTextAlign);

/* ************************************************************************ */
/* Text Wrapping                                                            */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetTextWrap(BOOL fTextWrap);
EQS_DCL BOOL         EQS_API GetTextWrap(VOID);

/* ************************************************************************ */
/* Text width and height                                                    */
/* ************************************************************************ */
EQS_DCL WORD         EQS_API TextWidth(CONST BYTE* pszString);
EQS_DCL WORD         EQS_API TextHeight(CONST BYTE* pszString);
EQS_DCL UINT8        EQS_API CharWidth(CONST BYTE bChar);
EQS_DCL UINT8        EQS_API CharHeight(CONST BYTE bChar);

/* ************************************************************************ */
/* Draw text                                                                */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetTextDrawMode(TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API ClearTextDrawMode(TEXT_DRAW_MODE TextDrawMode);
EQS_DCL BYTE         EQS_API GetTextDrawMode(VOID);

EQS_DCL VOID         EQS_API TextOutStr(COORD x, COORD y, CONST BYTE* pszString);
EQS_DCL VOID         EQS_API TextOutStrTDM(COORD x, COORD y, CONST BYTE* pszString, 
                                           TEXT_DRAW_MODE TextDrawModeA);
EQS_DCL VOID         EQS_API TextOutStrLen(COORD x, COORD y, 
                                           CONST BYTE* pszString, INT8 nLength);
EQS_DCL VOID         EQS_API TextOutStrLenTDM(COORD x, COORD y, CONST BYTE* pszString, 
                                              INT8 nLength, TEXT_DRAW_MODE TextDrawModeA);
EQS_DCL VOID         EQS_API TextOutStrF(COORD x, COORD y, 
                                         CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API TextOutStrTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawModeA, 
                                            CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API TextOutStrLenF(COORD x, COORD y, INT8 nLength, 
                                            CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API TextOutStrLenTDMF(COORD x, COORD y, INT8 nLength, 
                                               TEXT_DRAW_MODE TextDrawModeA, 
                                               CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API TextOut(COORD x, COORD y, EQS_TOKEN TokenId);
EQS_DCL VOID         EQS_API TextOutTDM(COORD x, COORD y, EQS_TOKEN TokenId, 
                                        TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API TextOutLen(COORD x, COORD y, 
                                        EQS_TOKEN TokenId, INT8 nLength);
EQS_DCL VOID         EQS_API TextOutLenTDM(COORD x, COORD y, EQS_TOKEN TokenId, 
                                           INT8 nLength, TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API TextOutF(COORD x, COORD y, 
                                      EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API TextOutTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawMode,
                                         EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API TextOutLenF(COORD x, COORD y, INT8 nLength, 
                                         EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API TextOutLenTDMF(COORD x, COORD y, INT8 nLength, 
                                            TEXT_DRAW_MODE TextDrawMode,
                                            EQS_TOKEN TokenId, ...);
EQS_DCL VOID EQS_API TextOutBufferLenTDM(COORD x, COORD y, CONST BYTE* pszString, 
                           TEXT_DRAW_MODE TextDrawModeA, UINT16 nLen);

#ifdef FEATURE_EQS_GRAPH_DRAWTEXT
EQS_DCL VOID         EQS_API DrawText(RECT* pRect, 
                                    TEXT_DRAW_FORMAT DrawFormat, 
                                    BYTE* pszString);
EQS_DCL VOID         EQS_API DrawTextF(RECT* pRect, 
                                     TEXT_DRAW_FORMAT DrawFormat, 
                                     CONST BYTE* pszFormat, ...);
#endif /* FEATURE_EQS_GRAPH_DRAWTEXT */

/* ************************************************************************ */
/* Coordinate                                                              */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetCoordinates(COORD_SYSTEM CoordSystem);
EQS_DCL COORD_SYSTEM EQS_API GetCoordinates(VOID);

/* ************************************************************************ */
/* Origin                                                                  */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API SetOrigin(POINT* pOrigin);
//EQS_DCL POINT*       EQS_API GetOrigin(VOID);
EQS_DCL VOID         EQS_API GetOrigin(POINT* pOrigin);

/* ************************************************************************ */
/* Device Position <--> Logical Postion                                    */
/* ************************************************************************ */
EQS_DCL COORD        EQS_API DPx2LPx(COORD x);
EQS_DCL COORD        EQS_API DPy2LPy(COORD y);
EQS_DCL COORD        EQS_API LPx2DPx(COORD x);
EQS_DCL COORD        EQS_API LPy2DPy(COORD y);
EQS_DCL VOID         EQS_API DP2LP(POINT* pPos);
EQS_DCL VOID         EQS_API LP2DP(POINT* pPos);

extern  void         EQS_API MakeScreenLighter(UINT8 nTimes);
extern  void         EQS_API MakeScreenDarker(UINT8 nTimes);

extern void PutBitmapRect256C(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);
extern void PutBitmapRect64KC(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);

#ifdef __cplusplus
}
#endif

#endif  /* _EQS_GRPH_H_ */
