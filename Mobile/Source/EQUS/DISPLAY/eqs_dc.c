/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_dc.c
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
** 2000-07-06 jrkwon     Created.
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
#define MAX_DC_COUNT    10   /* should be same as the max applet depth */

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL DEVICE_CONTEXT    DC[MAX_DC_COUNT];
LOCAL HDC               nCurrentDC;
LOCAL BOOL              l_fFocusDraw;
LOCAL BOOL              l_fInActiveDraw;
LOCAL BOOL              l_fActiveDraw;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL VOID   EQS_API sInitDC(HDC hDC);


VOID EQS_API EQS_InitDC(VOID)
{
    INT8 i;

    nCurrentDC      = 0;
	l_fFocusDraw    = FALSE;
	l_fInActiveDraw = FALSE;
	l_fActiveDraw   = FALSE;
    
    for(i = 0; i < MAX_DC_COUNT; i++)
        sInitDC(i);
}


/* same with initialize routine at OpenGraph in eqs_grph.c */
LOCAL VOID EQS_API sInitDC(HDC hDC)
{

    POINT Pos = { 0, 0 };
    TEXT_ALIGN TextAlign = { INIT_TAH, INIT_TAV };

    DC[hDC].ViewPort.CoordSystem = INIT_CS;
    DC[hDC].ViewPort.Origin = Pos;
    DC[hDC].ViewPort.Rect.xLeft = 0;
    DC[hDC].ViewPort.Rect.yTop = 0;
    DC[hDC].ViewPort.Rect.xRight = DevGetMaxX();
    DC[hDC].ViewPort.Rect.yBottom = DevGetMaxY();
    //DC[hDC].ViewPort = ViewPort;
    DC[hDC].CurrentPosition = Pos;
    DC[hDC].DrawMode = DM_COPY;


	if( l_fFocusDraw )
	{
		DC[hDC].BackColor =  BACK_COLOR;//EQS_CURSOR_BACK_COLOR;
		DC[hDC].ForeColor =  FORE_COLOR;//EQS_CURSOR_FORE_COLOR;
		DC[hDC].TextColor =  CS_TEXT_FORE_COLOR;//EQS_CURSOR_FORE_COLOR;
		DC[hDC].TextBackColor = CS_TEXT_BACK_COLOR;//EQS_CURSOR_BACK_COLOR;
	}
	else if( l_fActiveDraw )
	{
		DC[hDC].BackColor =  BACK_COLOR;//EQS_CURSOR_BACK_COLOR;
		DC[hDC].ForeColor =  FORE_COLOR;//EQS_CURSOR_FORE_COLOR;
		DC[hDC].TextColor =  CA_TEXT_FORE_COLOR;//EQS_CURSOR_FORE_COLOR;
		DC[hDC].TextBackColor = CA_TEXT_BACK_COLOR;//EQS_CURSOR_BACK_COLOR;
	}
	else
	{			
		DC[hDC].BackColor =  BACK_COLOR;//CN_TEXT_BACK_COLOR;//R_GRAY;	
		DC[hDC].ForeColor =  FORE_COLOR;//CN_TEXT_FORE_COLOR;//R_BLACK;		
		if( l_fInActiveDraw )
		{
			DC[hDC].TextColor     = CD_TEXT_FORE_COLOR;//INIT_TEXTCOLOR;
			DC[hDC].TextBackColor = CD_TEXT_BACK_COLOR;//LIGHTGRAY;
		}
		else
		{				
			DC[hDC].TextColor     = TEXT_FORE_COLOR;//INIT_TEXTCOLOR;
			DC[hDC].TextBackColor = TEXT_BACK_COLOR;//INIT_TEXTCOLOR;
		}
	}
    DC[hDC].nInterCharSpace = INIT_INTERCHARSPACE;
    DC[hDC].TextAlign = TextAlign;
    DC[hDC].TextDrawMode = INIT_TEXTDRAWMODE;

    DC[hDC].FontType = INIT_FONT_TYPE;
    DC[hDC].fTextWrap = INIT_TEXTWRAP;
}


HDC EQS_API EQS_GetDC(BOOL bClearDC)
{
    ASSERT(nCurrentDC <= MAX_DC_COUNT);
    nCurrentDC++;
    if(bClearDC)
        sInitDC(nCurrentDC);
    return nCurrentDC;
}

VOID EQS_API EQS_ReleaseDC(VOID)
{
    ASSERT(nCurrentDC > 0);
    //sInitDC(nCurrentDC);
    --nCurrentDC;
}

HDC EQS_API EQS_GetCurrentDCDepth(void) // girin. 2000.12.25 Debug use only.
{
    return nCurrentDC;
}

/* ************************************************************************ */
/* Screen                                                                   */
/* ************************************************************************ */
BYTE* EQS_API EQS_GetScreenBuffer(VOID)
{
    return GetScreenBuffer();
}

//VOID EQS_API EQS_ClearScreen(CLEAR_SCREEN_TYPE ClearType)
VOID EQS_API EQS_ClearScreen(VOID)
{
    COLOR BackColor;

    /* get the old properties of GD */
    BackColor = GetBackColor();
    /* Set the properties of GD temporaly */
    SetBackColor(EQS_GetBackColor());
    /* clear screen */
    ClearScreen();
    /* restore the original properties */
    SetBackColor(BackColor);
}

VOID EQS_API EQS_ClearViewPort(VOID)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR BackColor;

    /* get the old properties of GD */
    GetViewPort(&OldViewPort);
    BackColor = GetBackColor();
    
    /* Set the properties of GD temporaly */
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    SetBackColor(EQS_GetBackColor());
    
    ClearViewPort();
    
    /* restore the original properties */
    SetViewPort(&OldViewPort);
    SetBackColor(BackColor);
}

/* clear the region -- viewport */
VOID EQS_API EQS_ClearRegion(COORD xLeft, COORD yTop, 
                             COORD xRight, COORD yBottom)
{
    COLOR BackColor;

    /* get the old properties of GD */
    BackColor = GetBackColor();
    
    /* Set the properties of GD temporaly */
    SetBackColor(EQS_GetBackColor());
    
    ClearRegion(xLeft, yTop, xRight, yBottom);
    
    /* restore the original properties */
    SetBackColor(BackColor);
}

/* ************************************************************************ */
/* Viewport and screen size                                                 */
/* ************************************************************************ */
VOID EQS_API EQS_GetViewPort(VIEW_PORT* pViewPort)
{
    RECT ViewPortRect;

    *pViewPort = (DC[nCurrentDC].ViewPort);
    
    GetViewPortRect(&ViewPortRect);
    pViewPort->Rect.xLeft   += ViewPortRect.xLeft;
    pViewPort->Rect.yTop    += ViewPortRect.yTop;
    pViewPort->Rect.xRight  += ViewPortRect.xLeft;
    pViewPort->Rect.yBottom += ViewPortRect.yTop;
	
	if(pViewPort->Rect.xRight > GetMaxX())
		pViewPort->Rect.xRight = GetMaxX();
	if(pViewPort->Rect.yBottom > GetMaxY())
		pViewPort->Rect.yBottom = GetMaxY();
}

VOID EQS_API EQS_SetViewPort(VIEW_PORT* pViewPort)
{
/*    RECT ViewPortRect;

    GetViewPortRect(&ViewPortRect);
    pViewPort->Rect.xLeft   += ViewPortRect.xLeft;
    pViewPort->Rect.yTop    += ViewPortRect.yTop;
    pViewPort->Rect.xRight  += ViewPortRect.xLeft;
    pViewPort->Rect.yBottom += ViewPortRect.yTop;
*/
    DC[nCurrentDC].ViewPort = *pViewPort;
    EQS_SetCoordinates(DC[nCurrentDC].ViewPort.CoordSystem);
    EQS_SetOrigin(&(DC[nCurrentDC].ViewPort.Origin));
}

VOID EQS_API EQS_ResetViewPort(VOID)
{
    POINT Origin = { 0, 0 };
    RECT ViewPortRect;

    ViewPortRect.xLeft = 0;
    ViewPortRect.yTop = 0;
    ViewPortRect.xRight = GetMaxX();
    ViewPortRect.yBottom = GetMaxY();
    EQS_SetViewPortRect(&ViewPortRect);
    EQS_SetCoordinates(INIT_CS);
    EQS_SetOrigin(&Origin);
}

VOID EQS_API EQS_GetViewPortRect(RECT* pRect)
{
    RECT ViewPortRect;

    *pRect = (DC[nCurrentDC].ViewPort.Rect);

    GetViewPortRect(&ViewPortRect);
    pRect->xLeft   += ViewPortRect.xLeft;
    pRect->yTop    += ViewPortRect.yTop;
    pRect->xRight  += ViewPortRect.xLeft;
    pRect->yBottom += ViewPortRect.yTop;

	if(pRect->xRight > GetMaxX())
		pRect->xRight = GetMaxX();
	if(pRect->yBottom > GetMaxY())
		pRect->yBottom = GetMaxY();
}

VOID EQS_API EQS_SetViewPortRect(RECT* pRect)
{
/*    RECT ViewPortRect;

    GetViewPortRect(&ViewPortRect);
    pRect->xLeft   += ViewPortRect.xLeft;
    pRect->yTop    += ViewPortRect.yTop;
    pRect->xRight  += ViewPortRect.xLeft;
    pRect->yBottom += ViewPortRect.yTop;
*/
    DC[nCurrentDC].ViewPort.Rect = *pRect;
}

COORD EQS_API EQS_GetMaxWidth(VOID)
{
    return DC[nCurrentDC].ViewPort.Rect.xRight 
            - DC[nCurrentDC].ViewPort.Rect.xLeft + 1;
}

COORD EQS_API EQS_GetMaxHeight(VOID)
{
    return DC[nCurrentDC].ViewPort.Rect.yBottom 
            - DC[nCurrentDC].ViewPort.Rect.yTop + 1;
}

COORD EQS_API EQS_GetMaxX(VOID)
{
    return EQS_GetMaxWidth()-1;
}

COORD EQS_API EQS_GetMaxY(VOID)
{
    return EQS_GetMaxHeight()-1;
}

/* ************************************************************************ */
/* Current position                                                         */
/* ************************************************************************ */
POINT* EQS_API EQS_GetCurrentPosition(VOID)
{
    return &(DC[nCurrentDC].CurrentPosition);
}

COORD EQS_API EQS_GetCurrentPositionX(VOID)
{
    return DC[nCurrentDC].CurrentPosition.x;
}

COORD EQS_API EQS_GetCurrentPositionY(VOID)
{
    return DC[nCurrentDC].CurrentPosition.y;
}

/* ************************************************************************ */
/* Color                                                                   */
/* ************************************************************************ */
COLOR EQS_API EQS_GetForeColor(VOID)
{
    return DC[nCurrentDC].ForeColor;
}

COLOR EQS_API EQS_GetBackColor(VOID)
{
    return DC[nCurrentDC].BackColor;
}

COLOR EQS_API EQS_GetTextColor(VOID)
{
    return DC[nCurrentDC].TextColor;
}

COLOR EQS_API EQS_GetTextBackColor(VOID)
{
    return DC[nCurrentDC].TextBackColor;
}

VOID EQS_API EQS_SetForeColor(COLOR ForeColor)
{
    DC[nCurrentDC].ForeColor = ForeColor;
}

VOID EQS_API EQS_SetBackColor(COLOR BackColor)
{
    DC[nCurrentDC].BackColor = BackColor;
}

VOID EQS_API EQS_SetTextColor(COLOR TextColor)
{
    DC[nCurrentDC].TextColor = TextColor;
}

VOID EQS_API EQS_SetTextBackColor(COLOR TextBackColor)
{
    DC[nCurrentDC].TextBackColor = TextBackColor;
}

VOID EQS_API EQS_SetDrawMode(DRAW_MODE DrawMode)
{
    DC[nCurrentDC].DrawMode = DrawMode;
}

DRAW_MODE EQS_API EQS_GetDrawMode(VOID)
{
    return DC[nCurrentDC].DrawMode;
}

/* ************************************************************************ */
/* Primitive shapes                                                        */
/* ************************************************************************ */
COLOR EQS_API EQS_GetPixel(COORD x, COORD y)
{
    return DevGetPixel(EQS_LPx2DPx(x), EQS_LPy2DPy(y));
}

VOID EQS_API EQS_SetPixel(COORD x, COORD y, COLOR Color)
{
    DevSetPixel(EQS_LPx2DPx(x), EQS_LPy2DPy(y), Color);
}

VOID EQS_API EQS_MoveTo(COORD x, COORD y)
{
    DC[nCurrentDC].CurrentPosition.x = EQS_LPx2DPx(x);
    DC[nCurrentDC].CurrentPosition.y = EQS_LPy2DPy(y);
}

VOID EQS_API EQS_MoveRel(COORD dx, COORD dy)
{
    DC[nCurrentDC].CurrentPosition.x += dx;
    DC[nCurrentDC].CurrentPosition.y += dy;
}

VOID EQS_API EQS_Line(COORD xStart, COORD yStart, 
                    COORD xEnd, COORD yEnd)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    Color = GetForeColor();
    SetForeColor(EQS_GetForeColor());
    SetViewPort(&ViewPort);
    
    Line(xStart, yStart, xEnd, yEnd);
    
    SetViewPort(&OldViewPort);
    SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

VOID EQS_API EQS_LineTo(COORD x, COORD y)
{
    EQS_Line(EQS_GetCurrentPositionX(), EQS_GetCurrentPositionY(), x, y);
}

VOID EQS_API EQS_LineRel(COORD dx, COORD dy)
{
    EQS_LineTo((COORD)(EQS_GetCurrentPositionX() + dx), 
              (COORD)(EQS_GetCurrentPositionY() + dy));
}

VOID EQS_API EQS_Rectangle(COORD xLeft, COORD yTop, 
                         COORD xRight, COORD yBottom)
{
    EQS_Line(xLeft, yTop, xRight, yTop);
    EQS_Line(xRight, yTop, xRight, yBottom);
    EQS_Line(xRight, yBottom, xLeft, yBottom);
    EQS_Line(xLeft, yBottom, xLeft, yTop);
}

VOID EQS_API EQS_RoundRectangle(COORD xLeft, COORD yTop, 
                                COORD xRight, COORD yBottom,
                                INT16 nRadius)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    Color = GetForeColor();
    SetForeColor(EQS_GetForeColor());
    SetViewPort(&ViewPort);
    
    RoundRectangle(xLeft, yTop, xRight, yBottom, nRadius);
    
    SetViewPort(&OldViewPort);
    SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

VOID EQS_API EQS_Circle(COORD xCenter, COORD yCenter, INT16 nRadius)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    Color = GetForeColor();
    SetForeColor(EQS_GetForeColor());
    SetViewPort(&ViewPort);
    
    Circle(xCenter, yCenter, nRadius);
    
    SetViewPort(&OldViewPort);
    SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

VOID EQS_API EQS_Ellipse(COORD xCenter, COORD yCenter, 
                    COORD xRadius, COORD yRadius)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    Color = GetForeColor();
    SetForeColor(EQS_GetForeColor());
    SetViewPort(&ViewPort);
    
    Ellipse(xCenter, yCenter, xRadius, yRadius);
    SetViewPort(&OldViewPort);
    SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

VOID EQS_API EQS_Polyline(POINT* pPoint, WORD wCount)
{
    VIEW_PORT OldViewPort, ViewPort;
    COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    Color = GetForeColor();
    SetForeColor(EQS_GetForeColor());
    SetViewPort(&ViewPort);
    
    Polyline(pPoint, wCount);

    SetViewPort(&OldViewPort);
    SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

VOID EQS_API EQS_FillRectangle(COORD xLeft, COORD yTop, COORD xRight, 
                                         COORD yBottom, COLOR FillColor)
{
    VIEW_PORT OldViewPort, ViewPort;
    //COLOR Color;
    DRAW_MODE OldDrawMode;

    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    //Color = GetForeColor();
    ////SetForeColor(EQS_GetForeColor());
    //SetForeColor(FillColor);
    SetViewPort(&ViewPort);
    
    FillRectangle(xLeft, yTop, xRight, yBottom, FillColor);
    
    SetViewPort(&OldViewPort);
    //SetForeColor(Color);
    SetDrawMode(OldDrawMode);
}

#ifdef FEATURE_GRAPH_ENHANCED
VOID         EQS_API EQS_Arc(VOID) {}
VOID         EQS_API EQS_ArcTo(VOID) {}
VOID         EQS_API EQS_Pie(VOID) {}
VOID         EQS_API EQS_Sector(VOID) {}
VOID         EQS_API EQS_SetFillStyle(VOID) {}
VOID         EQS_API EQS_SetFillPattern(VOID) {}
VOID         EQS_API EQS_FillCircle(COORD xCenter, COORD yCenter, 
                                    INT16 nRadius, COLOR FillColor) {}
VOID         EQS_API EQS_FillEllipse(VOID) {}
VOID         EQS_API EQS_FillPoly(VOID) {}
#endif /* FEATURE_GRAPH_ENHANCED */ 

/* ************************************************************************ */
/* Save and restore region                                                 */
/* ************************************************************************ */
VOID EQS_API EQS_GetRegionImage(COORD xLeft, COORD yTop, 
                              COORD xRight, COORD yBottom, 
                              BYTE* pbBuffer)
{
    VIEW_PORT OldViewPort, ViewPort;

    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    GetRegionImage(xLeft, yTop, xRight, yBottom, pbBuffer);

    SetViewPort(&OldViewPort);
}

VOID EQS_API EQS_PutRegionImage(COORD xLeft, COORD yTop, 
                              BYTE* pbBuffer)
{
    VIEW_PORT OldViewPort, ViewPort;

    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    PutRegionImage(xLeft, yTop, pbBuffer);

    SetViewPort(&OldViewPort);
}

WORD EQS_API EQS_GetRegionImageSize(COORD xLeft, COORD yTop, 
                                  COORD xRight, COORD yBottom)
{
    VIEW_PORT OldViewPort, ViewPort;
    WORD wSize;

    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    wSize = GetRegionImageSize(xLeft, yTop, xRight, yBottom);

    SetViewPort(&OldViewPort);
    return wSize;
}

VOID EQS_API EQS_GetRegionImageBuffer(COORD xLeft, COORD yTop, 
                                      COORD xRight, COORD yBottom,
                                      BYTE** ppbBuffer)
{
    VIEW_PORT OldViewPort, ViewPort;

    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    GetRegionImageBuffer(xLeft, yTop, xRight, yBottom, ppbBuffer);

    SetViewPort(&OldViewPort);
}

VOID EQS_PutBitmapRect(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect)
{ 
    VIEW_PORT OldViewPort, ViewPort;
    DRAW_MODE OldDrawMode;
    COLOR OldColor;

    OldColor = GetBackColor();
    SetBackColor(EQS_GetBackColor());
    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    PutBitmapRect(x, y, pBitmap, Rect);	

    SetBackColor(OldColor);
    SetViewPort(&OldViewPort);
    SetDrawMode(OldDrawMode);
}


/* ************************************************************************ */
/* Draw bitmap image                                                       */
/* ************************************************************************ */
VOID EQS_API EQS_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{ 
    VIEW_PORT OldViewPort, ViewPort;
    DRAW_MODE OldDrawMode;
    COLOR OldColor;

    OldColor = GetBackColor();
    SetBackColor(EQS_GetBackColor());
    OldDrawMode = GetDrawMode();
    SetDrawMode(EQS_GetDrawMode());
    GetViewPort(&OldViewPort);
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    
    PutBitmap(x, y, pBitmap);

    SetBackColor(OldColor);
    SetViewPort(&OldViewPort);
    SetDrawMode(OldDrawMode);
}

/* ************************************************************************ */
/* Font                                                                    */
/* ************************************************************************ */
VOID EQS_API EQS_SetFontType(FONT_TYPE FontType)
{
    DC[nCurrentDC].FontType = FontType;
}

FONT_TYPE EQS_API EQS_GetFontType(VOID)
{
    return DC[nCurrentDC].FontType;
}

CONST SIZE*  EQS_API EQS_GetFontSize(FONT_TYPE FontType, FONT_KIND FontKind)
{
    return GetFontSize(FontType, FontKind);
}

/* ************************************************************************ */
/* Text align                                                              */
/* ************************************************************************ */
VOID EQS_API EQS_SetTextAlign(TEXT_ALIGN* pTextAlign)
{
    DC[nCurrentDC].TextAlign = *pTextAlign;
}

VOID EQS_API EQS_SetTextAlignHoriz(TEXT_ALIGN_HORIZ TextAlignHoriz)
{
    DC[nCurrentDC].TextAlign.Horiz = TextAlignHoriz;
}

VOID EQS_API EQS_SetTextAlignVert(TEXT_ALIGN_VERT TextAlignVert)
{
    DC[nCurrentDC].TextAlign.Vert = TextAlignVert;
}

VOID EQS_API EQS_GetTextAlign(TEXT_ALIGN* pTextAlign)
{
    *pTextAlign = DC[nCurrentDC].TextAlign;
    //return &GD.DC.TextAlign;
}

VOID EQS_API EQS_SetTextWrap(BOOL fTextWrap)
{
    DC[nCurrentDC].fTextWrap = fTextWrap;
}

BOOL EQS_API EQS_GetTextWrap(VOID)
{
    return DC[nCurrentDC].fTextWrap;
}

/* ************************************************************************ */
/* Text width and height                                                   */
/* ************************************************************************ */
WORD EQS_API EQS_TextWidth(CONST BYTE* pszString)
{
    WORD wWidth;
    FONT_TYPE FontType;
    
    FontType = GetFontType();
    SetFontType(EQS_GetFontType());
    wWidth = TextWidth(pszString);
    SetFontType(FontType);
    
    return wWidth;
}

WORD EQS_API EQS_TextHeight(CONST BYTE* pszString)
{
    WORD wHeight;
    FONT_TYPE FontType;
    
    FontType = GetFontType();
    SetFontType(EQS_GetFontType());
    wHeight = TextHeight(pszString);
    SetFontType(FontType);
    
    return wHeight;
}

UINT8 EQS_API EQS_CharWidth(CONST BYTE bChar)
{
    UINT8 nWidth;
    FONT_TYPE FontType;
    
    FontType = GetFontType();
    SetFontType(EQS_GetFontType());
    nWidth = CharWidth(bChar);
    SetFontType(FontType);

    return nWidth;
}

UINT8 EQS_API EQS_CharHeight(CONST BYTE bChar)
{
    UINT8 nHeight;
    FONT_TYPE FontType;
    
    FontType = GetFontType();
    SetFontType(EQS_GetFontType());
    nHeight = CharHeight(bChar);
    SetFontType(FontType);

    return nHeight;
}

/* ************************************************************************ */
/* Draw text                                                               */
/* ************************************************************************ */
VOID EQS_API EQS_SetTextDrawMode(TEXT_DRAW_MODE TextDrawMode)
{
    if(TextDrawMode == TDM_NORMAL) 
        DC[nCurrentDC].TextDrawMode = TextDrawMode; /* clear all bit flags */
    else
        DC[nCurrentDC].TextDrawMode |= TextDrawMode;
}

VOID EQS_API EQS_ClearTextDrawMode(TEXT_DRAW_MODE TextDrawMode)
{
    DC[nCurrentDC].TextDrawMode &= ~TextDrawMode;
}

BYTE EQS_API EQS_GetTextDrawMode(VOID)
{
    return DC[nCurrentDC].TextDrawMode;
}


VOID EQS_API EQS_TextOutStr(COORD x, COORD y, CONST BYTE* pszString)
{
    TEXT_ALIGN OldTextAlign, TextAlign;
    TEXT_DRAW_MODE TextDrawMode;
    DRAW_MODE OldDrawMode;
    FONT_TYPE FontType;
    VIEW_PORT OldViewPort, ViewPort;
    COLOR OldTextColor, OldTextBackColor, OldBackColor;
    BOOL fOldTextWrap;

    /* get the old properties */
    GetTextAlign(&OldTextAlign);
    FontType = GetFontType();
    TextDrawMode = GetTextDrawMode(); 
    OldDrawMode = GetDrawMode();
    GetViewPort(&OldViewPort);
    OldTextColor = GetTextColor();
    OldTextBackColor = GetTextBackColor();
    OldBackColor = GetBackColor();
    fOldTextWrap = GetTextWrap();

    /* set the properties by the current DC's properties */
    SetFontType(EQS_GetFontType());
    EQS_GetTextAlign(&TextAlign);
    SetTextAlign(&TextAlign);
    SetTextDrawMode(EQS_GetTextDrawMode());
    SetDrawMode(EQS_GetDrawMode());
    EQS_GetViewPort(&ViewPort);
    SetViewPort(&ViewPort);
    SetTextColor(EQS_GetTextColor());
    SetTextBackColor(EQS_GetTextBackColor());
    SetBackColor(EQS_GetBackColor());
    SetTextWrap(EQS_GetTextWrap());

    TextOutStr(x, y, pszString);
    
    /* restore the old properties */
    SetTextAlign(&OldTextAlign);
    SetFontType(FontType);
    SetTextDrawMode(TextDrawMode);
    SetDrawMode(OldDrawMode);
    SetViewPort(&OldViewPort);
    SetTextColor(OldTextColor);
    SetTextBackColor(OldTextBackColor);
    SetBackColor(OldBackColor);
    SetTextWrap(fOldTextWrap);
}

VOID EQS_API EQS_TextOutStrTDM(COORD x, COORD y, CONST BYTE* pszString,
                               TEXT_DRAW_MODE TextDrawModeA)
{
    TEXT_DRAW_MODE TextDrawMode;

    TextDrawMode = EQS_GetTextDrawMode();
    EQS_SetTextDrawMode(TextDrawModeA);
    EQS_TextOutStr(x, y, pszString);
    EQS_SetTextDrawMode(TextDrawMode);
}

VOID EQS_API EQS_TextOutStrLen(COORD x, COORD y, 
                       CONST BYTE* pszString, INT8 nLength)
{
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    INT8 i;

    STRNCPY(szString, pszString, (SIZE_T)nLength);
    for(i = (INT8)STRLEN(pszString); i < nLength; i++)
        szString[i] = ' '; /* fill a space */
    szString[nLength] = '\0';

    EQS_TextOutStr(x, y, szString);
}

VOID EQS_API EQS_TextOutStrLenTDM(COORD x, COORD y, 
                                  CONST BYTE* pszString, INT8 nLength,
                                  TEXT_DRAW_MODE TextDrawModeA)
{
    TEXT_DRAW_MODE TextDrawMode;

    TextDrawMode = EQS_GetTextDrawMode();
    EQS_SetTextDrawMode(TextDrawModeA);
    EQS_TextOutStrLen(x, y, pszString, nLength);
    EQS_SetTextDrawMode(TextDrawMode);
}

VOID EQS_API EQS_TextOutStrF(COORD x, COORD y, 
                     CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStr(x, y, szString);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutStrTDMF(COORD x, COORD y, 
                                TEXT_DRAW_MODE TextDrawMode,
                                CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrTDM(x, y, szString, TextDrawMode);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutStrLenF(COORD x, COORD y, INT8 nLength, 
                                       CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrLen(x, y, szString, nLength);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutStrLenTDMF(COORD x, COORD y, INT8 nLength,
                                   TEXT_DRAW_MODE TextDrawMode,
                                   CONST BYTE* pszFormat, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrLenTDM(x, y, szString, nLength, TextDrawMode);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOut(COORD x, COORD y, EQS_TOKEN TokenId)
{
    EQS_TextOutStr(x, y, EQS_GET_TOKEN(TokenId));
}

VOID EQS_API EQS_TextOutTDM(COORD x, COORD y, 
                            EQS_TOKEN TokenId, TEXT_DRAW_MODE TextDrawMode)
{
    EQS_TextOutStrTDM(x, y, EQS_GET_TOKEN(TokenId), TextDrawMode);
}

VOID EQS_API EQS_TextOutLen(COORD x, COORD y, 
                            EQS_TOKEN TokenId, INT8 nLength)
{
    EQS_TextOutStrLen(x, y, EQS_GET_TOKEN(TokenId), nLength);
}

VOID EQS_API EQS_TextOutLenTDM(COORD x, COORD y, 
                            EQS_TOKEN TokenId, 
                            INT8 nLength, TEXT_DRAW_MODE TextDrawMode)
{
    EQS_TextOutStrLenTDM(x, y, EQS_GET_TOKEN(TokenId), nLength, TextDrawMode);
}

VOID EQS_API EQS_TextOutF(COORD x, COORD y, 
                          EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStr(x, y, szString);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawMode,
                          EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrTDM(x, y, szString, TextDrawMode);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutLenF(COORD x, COORD y, INT8 nLength, 
                             EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrLen(x, y, szString, nLength);
    
    va_end(vaArgPtr);
}

VOID EQS_API EQS_TextOutLenTDMF(COORD x, COORD y, INT8 nLength, 
                                TEXT_DRAW_MODE TextDrawMode,
                                EQS_TOKEN TokenId, ...)
{
    va_list vaArgPtr;
    BYTE szString[EQS_TO_MAX_STRLEN+1];
    CONST BYTE* pszFormat = EQS_GET_TOKEN(TokenId);

    va_start(vaArgPtr, pszFormat);
    
    vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
    EQS_TextOutStrLen(x, y, szString, nLength);
    
    va_end(vaArgPtr);
}

#ifdef FEATURE_GRAPH_DRAWTEXT
VOID EQS_API EQS_DrawText(RECT* pRect, 
                        TEXT_DRAW_FORMAT DrawFormat, 
                        BYTE* pszString)
{
}

VOID EQS_API EQS_DrawTextF(RECT* pRect, 
                         TEXT_DRAW_FORMAT DrawFormat, 
                         CONST BYTE* pszFormat, ...)
{
}
#endif /* FEATURE_GRAPH_DRAWTEXT */

/* ************************************************************************ */
/* Coordinate                                                              */
/* ************************************************************************ */
VOID EQS_API EQS_SetCoordinates(COORD_SYSTEM CoordSystem)
{
    DC[nCurrentDC].ViewPort.CoordSystem = CoordSystem;
}

COORD_SYSTEM EQS_API EQS_GetCoordinates(VOID)
{
    return DC[nCurrentDC].ViewPort.CoordSystem;
}


/* ************************************************************************ */
/* Origin                                                                  */
/* ************************************************************************ */
VOID EQS_API EQS_SetOrigin(POINT* pOrigin)
{
    DC[nCurrentDC].ViewPort.Origin = *pOrigin;
}

VOID EQS_API EQS_GetOrigin(POINT* pOrigin)
{
    *pOrigin = DC[nCurrentDC].ViewPort.Origin;
}

/* ************************************************************************ */
/* Device Position <--> Logical Postion                                    */
/* ************************************************************************ */
COORD EQS_API EQS_DPx2LPx(COORD x)
{
    return x - (DC[nCurrentDC].ViewPort.Rect.xLeft 
                + DC[nCurrentDC].ViewPort.Origin.x);
}

COORD EQS_API EQS_DPy2LPy(COORD y)
{
    if(EQS_GetCoordinates() == CS_MATH)
        return (-1)*DC[nCurrentDC].ViewPort.Origin.y 
                + DC[nCurrentDC].ViewPort.Rect.yTop - y;
    else
        return y - (DC[nCurrentDC].ViewPort.Rect.yTop 
               + DC[nCurrentDC].ViewPort.Origin.y);
}

COORD EQS_API EQS_LPx2DPx(COORD x)
{
    COORD xDP;
    
    xDP = x + DC[nCurrentDC].ViewPort.Rect.xLeft 
          + DC[nCurrentDC].ViewPort.Origin.x;

//    ASSERT(xDP >= 0 && xDP < GetScreenSize()->xWidth);

    return xDP;
}

COORD EQS_API EQS_LPy2DPy(COORD y)
{
    COORD yDP;

    if(GetCoordinates() == CS_MATH)
        yDP = DC[nCurrentDC].ViewPort.Rect.yTop 
              - (DC[nCurrentDC].ViewPort.Origin.y + y);
    else
        yDP = DC[nCurrentDC].ViewPort.Rect.yTop 
              + DC[nCurrentDC].ViewPort.Origin.y + y;

//    ASSERT(yDP >= 0 && yDP < GetScreenSize()->yHeight);
    
    return yDP;
}

VOID EQS_API EQS_DP2LP(POINT* pPos)
{
    pPos->x = pPos->x - (DC[nCurrentDC].ViewPort.Rect.xLeft 
              + DC[nCurrentDC].ViewPort.Origin.x);
    if(GetCoordinates() == CS_MATH)
        pPos->y = (-1)*DC[nCurrentDC].ViewPort.Origin.y 
                  + DC[nCurrentDC].ViewPort.Rect.yTop - pPos->y;
    else
        pPos->y = pPos->y - (DC[nCurrentDC].ViewPort.Rect.yTop 
                  + DC[nCurrentDC].ViewPort.Origin.y);
}

VOID EQS_API EQS_LP2DP(POINT* pPos)
{
    pPos->x = pPos->x + DC[nCurrentDC].ViewPort.Rect.xLeft 
              + DC[nCurrentDC].ViewPort.Origin.x;
    if(GetCoordinates() == CS_MATH)
        pPos->y = DC[nCurrentDC].ViewPort.Rect.yTop 
                  - (DC[nCurrentDC].ViewPort.Origin.y + pPos->y);
    else
        pPos->y = DC[nCurrentDC].ViewPort.Rect.yTop 
                  + DC[nCurrentDC].ViewPort.Origin.y + pPos->y;

//    ASSERT(pPos->x >= 0 && pPos->x < GetScreenSize()->xWidth);
//    ASSERT(pPos->y >= 0 && pPos->y < GetScreenSize()->yHeight);
}

VOID EQS_API EQS_SetFocusDraw(BOOL fFocusDraw)
{
	l_fFocusDraw = fFocusDraw;
}


BOOL EQS_API EQS_GetFocusDraw()
{
	return l_fFocusDraw;
}

VOID EQS_API EQS_SetInActiveDraw(BOOL fInActiveDraw)
{
	l_fInActiveDraw = fInActiveDraw;
}

BOOL EQS_API EQS_GetInActiveDraw(VOID)
{
	return l_fInActiveDraw;
}

VOID EQS_API EQS_SetActiveDraw(BOOL fActiveDraw )
{
	l_fActiveDraw = fActiveDraw;
}

BOOL EQS_API EQS_GetActiveDraw(VOID)
{
	return l_fActiveDraw;
}