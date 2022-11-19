/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqs_dc.h
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
** ============================================================================
*/

#ifndef _EQS_DC_H_
#define _EQS_DC_H_

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
#define EQS_ClearTextAlign()              EQS_SetTextAlignHoriz(TAH_LEFT); \
                                          EQS_SetTextAlignVert(TAV_TOP)
//#define EQS_ResetViewPort()               ResetViewPort()

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef INT8            HDC;    /* handle of device context */

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
/* ************************************************************************ */
/* Graphics system                                                         */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_InitDC(VOID);
EQS_DCL HDC          EQS_API EQS_GetDC(BOOL bClearDC);
EQS_DCL VOID EQS_API  EQS_ReleaseDC(VOID);

EQS_DCL HDC EQS_API EQS_GetCurrentDCDepth(void); // girin. 2000.12.25 Debug use only.

/* ************************************************************************ */
/* Screen                                                                   */
/* ************************************************************************ */
EQS_DCL BYTE*        EQS_API EQS_GetScreenBuffer(VOID);
//EQS_DCL VOID         EQS_API EQS_ClearScreen(CLEAR_SCREEN_TYPE ClearType);
EQS_DCL VOID         EQS_API EQS_ClearScreen(VOID);
EQS_DCL VOID         EQS_API EQS_ClearViewPort(VOID);
EQS_DCL VOID         EQS_API EQS_ClearRegion(COORD xLeft, COORD yTop, 
                                             COORD xRight, COORD yBottom);

/* ************************************************************************ */
/* Viewport and screen size                                                */
/* ************************************************************************ */
//EQS_DCL VIEW_PORT*   EQS_API EQS_GetViewPort(VOID);
EQS_DCL VOID         EQS_API EQS_GetViewPort(VIEW_PORT* pViewPort);
EQS_DCL VOID         EQS_API EQS_SetViewPort(VIEW_PORT* pViewPort);
EQS_DCL VOID         EQS_API EQS_ResetViewPort(VOID);
//EQS_DCL RECT*        EQS_API EQS_GetViewPortRect(VOID);
EQS_DCL VOID         EQS_API EQS_GetViewPortRect(RECT* pRect);
EQS_DCL VOID         EQS_API EQS_SetViewPortRect(RECT* pRect);
EQS_DCL COORD        EQS_API EQS_GetMaxWidth(VOID);
EQS_DCL COORD        EQS_API EQS_GetMaxHeight(VOID);
EQS_DCL COORD        EQS_API EQS_GetMaxX(VOID);
EQS_DCL COORD        EQS_API EQS_GetMaxY(VOID);

/* ************************************************************************ */
/* Current position                                                        */
/* ************************************************************************ */
EQS_DCL POINT*       EQS_API EQS_GetCurrentPosition(VOID);
EQS_DCL COORD        EQS_API EQS_GetCurrentPositionX(VOID);
EQS_DCL COORD        EQS_API EQS_GetCurrentPositionY(VOID);

/* ************************************************************************ */
/* Color                                                                   */
/* ************************************************************************ */
EQS_DCL COLOR        EQS_API EQS_GetForeColor(VOID);
EQS_DCL COLOR        EQS_API EQS_GetBackColor(VOID);
EQS_DCL COLOR        EQS_API EQS_GetTextColor(VOID);
EQS_DCL COLOR        EQS_API EQS_GetTextBackColor(VOID);
EQS_DCL VOID         EQS_API EQS_SetForeColor(COLOR Color);
EQS_DCL VOID         EQS_API EQS_SetBackColor(COLOR Color);
EQS_DCL VOID         EQS_API EQS_SetTextColor(COLOR Color);
EQS_DCL VOID         EQS_API EQS_SetTextBackColor(COLOR Color);
EQS_DCL VOID         EQS_API EQS_SetDrawMode(DRAW_MODE DrawMode);
EQS_DCL DRAW_MODE    EQS_API EQS_GetDrawMode(VOID);

/* ************************************************************************ */
/* Primitive shapes                                                        */
/* ************************************************************************ */
EQS_DCL COLOR        EQS_API EQS_GetPixel(COORD x, COORD y);
EQS_DCL VOID         EQS_API EQS_SetPixel(COORD x, COORD y, COLOR Color);
EQS_DCL VOID         EQS_API EQS_MoveTo(COORD x, COORD y);
EQS_DCL VOID         EQS_API EQS_MoveRel(COORD dx, COORD dy);
EQS_DCL VOID         EQS_API EQS_Line(COORD xStart, COORD yStart, 
                                      COORD xEnd, COORD yEnd);
EQS_DCL VOID         EQS_API EQS_LineTo(COORD x, COORD y);
EQS_DCL VOID         EQS_API EQS_LineRel(COORD dx, COORD dy);
EQS_DCL VOID         EQS_API EQS_Rectangle(COORD xLeft, COORD yTop, 
                                           COORD xRight, COORD yBottom);
EQS_DCL VOID         EQS_API EQS_RoundRectangle(COORD xLeft, COORD yTop, 
                                                COORD xRight, COORD yBottom,
                                                INT16 nRadius);
EQS_DCL VOID         EQS_API EQS_Circle(COORD xCenter, COORD yCenter, INT16 nRadius);
EQS_DCL VOID         EQS_API EQS_Ellipse(COORD xCenter, COORD yCenter, 
                                         COORD xRadius, COORD yRadius);
EQS_DCL VOID         EQS_API EQS_Polyline(POINT* pPoint, WORD wCount);
EQS_DCL VOID         EQS_API EQS_FillRectangle(COORD xLeft, COORD yTop, COORD xRight, 
                                               COORD yBottom, COLOR FillColor);
#ifdef FEATURE_GRAPH_ENHANCED
EQS_DCL VOID         EQS_API EQS_Arc(VOID);
EQS_DCL VOID         EQS_API EQS_ArcTo(VOID);
EQS_DCL VOID         EQS_API EQS_Pie(VOID);
EQS_DCL VOID         EQS_API EQS_Sector(VOID);
EQS_DCL VOID         EQS_API EQS_SetFillStyle(VOID);
EQS_DCL VOID         EQS_API EQS_SetFillPattern(VOID);
EQS_DCL VOID         EQS_API EQS_FillCircle(COORD xCenter, COORD yCenter, 
                                            INT16 nRadius, COLOR FillColor); 
EQS_DCL VOID         EQS_API EQS_FillEllipse(VOID);
EQS_DCL VOID         EQS_API EQS_FillPoly(VOID);
#endif /* FEATURE_GRAPH_ENHANCED */

/* ************************************************************************ */
/* Save and restore region                                                 */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_GetRegionImage(COORD xLeft, COORD yTop, 
                                          COORD xRight, COORD yBottom, 
                                          BYTE* pbBuffer);
EQS_DCL VOID         EQS_API EQS_PutRegionImage(COORD xLeft, COORD yTop, 
                                          BYTE* pbBuffer);
EQS_DCL WORD         EQS_API EQS_GetRegionImageSize(COORD xLeft, COORD yTop, 
                                        COORD xRight, COORD yBottom);
EQS_DCL VOID         EQS_API EQS_GetRegionImageBuffer(COORD xLeft, COORD yTop, 
                                                  COORD xRight, 
                                                  COORD yBottom, 
                                                  BYTE** ppbBuffer);

/* ************************************************************************ */
/* Draw bitmap image                                                       */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_PutBitmapRect(COORD x, COORD y, CONST BITMAP* pBitmap, RECT Rect);
EQS_DCL VOID         EQS_API EQS_PutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);

/* ************************************************************************ */
/* Font                                                                    */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetFontType(FONT_TYPE FontType);
EQS_DCL FONT_TYPE    EQS_API EQS_GetFontType(VOID);

/* ************************************************************************ */
/* Text align                                                              */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetTextAlign(TEXT_ALIGN* pTextAlign);
EQS_DCL VOID         EQS_API EQS_SetTextAlignHoriz(TEXT_ALIGN_HORIZ TextAlignHoriz);
EQS_DCL VOID         EQS_API EQS_SetTextAlignVert(TEXT_ALIGN_VERT TextAlignVert);
//EQS_DCL TEXT_ALIGN*  EQS_API EQS_GetTextAlign(VOID);
EQS_DCL VOID         EQS_API EQS_GetTextAlign(TEXT_ALIGN* pTextAlign);

/* ************************************************************************ */
/* Text Wrapping                                                            */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetTextWrap(BOOL fTextWrap);
EQS_DCL BOOL         EQS_API EQS_GetTextWrap(VOID);

/* ************************************************************************ */
/* Text width and height                                                   */
/* ************************************************************************ */
EQS_DCL WORD         EQS_API EQS_TextWidth(CONST BYTE* pszString);
EQS_DCL WORD         EQS_API EQS_TextHeight(CONST BYTE* pszString);
EQS_DCL UINT8        EQS_API EQS_CharWidth(CONST BYTE bChar);
EQS_DCL UINT8        EQS_API EQS_CharHeight(CONST BYTE bChar);
EQS_DCL CONST SIZE*  EQS_API EQS_GetFontSize(FONT_TYPE FontType, FONT_KIND FontKind);

/* ************************************************************************ */
/* Draw text                                                               */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetTextDrawMode(TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API EQS_ClearTextDrawMode(TEXT_DRAW_MODE TextDrawMode);
EQS_DCL BYTE         EQS_API EQS_GetTextDrawMode(VOID);
EQS_DCL VOID         EQS_API EQS_TextOutStr(COORD x, COORD y, CONST BYTE* pszString);
EQS_DCL VOID         EQS_API EQS_TextOutStrTDM(COORD x, COORD y, CONST BYTE* pszString,
                                               TEXT_DRAW_MODE TextDrawModeA);
EQS_DCL VOID         EQS_API EQS_TextOutStrLen(COORD x, COORD y, 
                                               CONST BYTE* pszString, INT8 nLength);
EQS_DCL VOID         EQS_API EQS_TextOutStrLenTDM(COORD x, COORD y, 
                                                  CONST BYTE* pszString, INT8 nLength,
                                                  TEXT_DRAW_MODE TextDrawModeA);
EQS_DCL VOID         EQS_API EQS_TextOutStrF(COORD x, COORD y, 
                                             CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API EQS_TextOutStrDTMF(COORD x, COORD y, 
                                                TEXT_DRAW_MODE TextDrawMode,
                                                CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API EQS_TextOutStrLenF(COORD x, COORD y, INT8 nLength, 
                                                CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API EQS_TextOutStrLenTDMF(COORD x, COORD y, INT8 nLength,
                                                   TEXT_DRAW_MODE TextDrawMode,
                                                   CONST BYTE* pszFormat, ...);
EQS_DCL VOID         EQS_API EQS_TextOut(COORD x, COORD y, EQS_TOKEN TokenId);
EQS_DCL VOID         EQS_API EQS_TextOutTDM(COORD x, COORD y, 
                                            EQS_TOKEN TokenId, TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API EQS_TextOutLen(COORD x, COORD y, 
                                            EQS_TOKEN TokenId, INT8 nLength);
EQS_DCL VOID         EQS_API EQS_TextOutLenTDM(COORD x, COORD y, 
                                               EQS_TOKEN TokenId, 
                                               INT8 nLength, TEXT_DRAW_MODE TextDrawMode);
EQS_DCL VOID         EQS_API EQS_TextOutF(COORD x, COORD y, 
                                          EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API EQS_TextOutTDMF(COORD x, COORD y, TEXT_DRAW_MODE TextDrawMode,
                                             EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API EQS_TextOutLenF(COORD x, COORD y, INT8 nLength, 
                                             EQS_TOKEN TokenId, ...);
EQS_DCL VOID         EQS_API EQS_TextOutLenTDMF(COORD x, COORD y, INT8 nLength, 
                                                TEXT_DRAW_MODE TextDrawMode,
                                                EQS_TOKEN TokenId, ...);

#ifdef FEATURE_GRAPH_DRAWTEXT
EQS_DCL VOID         EQS_API EQS_DrawText(RECT* pRect, 
                                    TEXT_DRAW_FORMAT DrawFormat, 
                                    BYTE* pszString);
EQS_DCL VOID         EQS_API EQS_DrawTextF(RECT* pRect, 
                                     TEXT_DRAW_FORMAT DrawFormat, 
                                     CONST BYTE* pszFormat, ...);
#endif /* FEATURE_GRAPH_DRAWTEXT */

/* ************************************************************************ */
/* Coordinate                                                              */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetCoordinates(COORD_SYSTEM CoordSystem);
EQS_DCL COORD_SYSTEM EQS_API EQS_GetCoordinates(VOID);

/* ************************************************************************ */
/* Origin                                                                  */
/* ************************************************************************ */
EQS_DCL VOID         EQS_API EQS_SetOrigin(POINT* pOrigin);
//EQS_DCL POINT*       EQS_API EQS_GetOrigin(VOID);
EQS_DCL VOID         EQS_API EQS_GetOrigin(POINT* pOrigin);

/* ************************************************************************ */
/* Device Position <--> Logical Postion                                    */
/* ************************************************************************ */
EQS_DCL COORD        EQS_API EQS_DPx2LPx(COORD x);
EQS_DCL COORD        EQS_API EQS_DPy2LPy(COORD y);
EQS_DCL COORD        EQS_API EQS_LPx2DPx(COORD x);
EQS_DCL COORD        EQS_API EQS_LPy2DPy(COORD y);
EQS_DCL VOID         EQS_API EQS_DP2LP(POINT* pPos);
EQS_DCL VOID         EQS_API EQS_LP2DP(POINT* pPos);


VOID EQS_API EQS_SetFocusDraw(BOOL fFocusDraw);
BOOL EQS_API EQS_GetFocusDraw(VOID);
VOID EQS_API EQS_SetInActiveDraw(BOOL fInActiveDraw);
BOOL EQS_API EQS_GetInActiveDraw(VOID);

VOID EQS_API EQS_SetActiveDraw(BOOL fActiveDraw );
BOOL EQS_API EQS_GetActiveDraw(VOID);

#ifdef __cplusplus
}
#endif

#endif  /* _EQS_DC_H_ */
