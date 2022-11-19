/*
** ============================================================================
**
** Project: 
**     BUNNY
**
** File:
**     sky_scrn.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_SCRN_H_
#define _SKY_SCRN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SUB_CHAR_NUM          (SKY_SUB_SCREEN_WIDTH/CharWidth(' '))
#define MAX_SUB_CHAR_NUM      14   /* SUB_CHAR_NUM */

#define SUB_LCD_FONT_WIDTH    6    /* CharWidth(' ') of system font */

#define SUB_LCD_X             (SKY_SUB_SCREEN_WIDTH/2)

#define OPEN_SUB_LCD()        SelectScreen(SI_SUB_SCREEN)
                              
#define CLEAR_SUB_LCD_ALL()   ClearScreen()
#define CLEAR_SUB_LCD(ln)     if(ln == LN_UPPER) CLEAR_SUB_LCD_UPPER(); \
                              else               CLEAR_SUB_LCD_LOWER()

#define CLEAR_SUB_LCD_UPPER() FillRectangle(0, 0, GetMaxX(), GetMaxY()/2, BLACK)
#define CLEAR_SUB_LCD_LOWER() FillRectangle(0, GetMaxY()/2, GetMaxX(), GetMaxY(), BLACK)
#define CLOSE_SUB_LCD()       SelectScreen(SI_MAIN_SCREEN); \
                              SKY_RefreshSubDisplay()

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum LINE_NUMBER_E 
{
    LN_UPPER,
//#if   SKY_MODEL_ID == SKY_MODEL_B1
    LN_LOWER,
//#endif
    LN_MAX

};
typedef enum LINE_NUMBER_E  LINE_NUMBER_E;

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
extern void SKY_API SKY_InitScreen(SCREEN *pMain, SCREEN *pSub);
extern void SKY_API SKY_SubLCDEndText(LINE_NUMBER_E LineNum);
extern BOOL SKY_API SKY_SubLCDIsEndText(LINE_NUMBER_E LineNum);
extern void SKY_API SKY_SubLCDTextOutStr(LINE_NUMBER_E LineNum, CONST BYTE* pszString);
extern void SKY_API SKY_SubLCDTextOutStrTDM(LINE_NUMBER_E LineNum, CONST BYTE* pszString, 
                                            TEXT_DRAW_MODE TextDrawModeA);
extern void SKY_API SKY_SubLCDTextOutStrF(LINE_NUMBER_E LineNum, CONST BYTE* pszFormat, ...);
extern void SKY_API SKY_SubLCDTextOutStrTDMF(LINE_NUMBER_E LineNum, TEXT_DRAW_MODE TextDrawModeA, 
                                             CONST BYTE* pszFormat, ...);
extern void SKY_API SKY_SubLCDTextOut(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId);
extern void SKY_API SKY_SubLCDTextOutTDM(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId, 
                                         TEXT_DRAW_MODE TextDrawMode);
extern void SKY_API SKY_SubLCDTextOutF(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId, ...);
extern void SKY_API SKY_SubLCDTextOutTDMF(LINE_NUMBER_E LineNum, TEXT_DRAW_MODE TextDrawMode,
                                         SKY_TOKEN TokenId, ...);
extern void SKY_API SKY_SubLCDPutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap);
extern void SKY_API SKY_SubLCDDefault(void);

extern void SKY_API SKY_Screen(ONOFF OnOff);
extern void SKY_API SKY_Backlight(ONOFF OnOff);
extern void SKY_API SKY_BacklightSensor(ONOFF OnOff);
extern void SKY_API SKY_RefreshDisplay(void);
extern void SKY_API SKY_SubBacklight_Confirm(ID_LCD_COLOR_E Color);
extern void SKY_API SKY_SetDisplayContrast(HS_CONTRAST_E DisplayContrast, BYTE DisplayContrastLevel);
#ifdef FEATURE_PARTIAL_DISPLAY
extern void SKY_API SKY_RefreshPartialDisplay(BYTE *pbBuffer, BYTE nXpos, BYTE nYpos, BYTE nXendpos, BYTE nYendpos);
extern void SKY_API SKY_RefreshDisplayRect(BYTE *pbBuffer, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom);
extern void SKY_API SKY_RefreshDisplayRectByRegion(BYTE *pbBuffer, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom);
#endif
#ifdef FEATURE_SKY_SUBLCD
extern void SKY_API SKY_SubScreen(ONOFF OnOff);
#ifdef FEATURE_COLOR_ID_LCD
extern void SKY_API SKY_SubBacklight(ONOFF OnOff, ID_LCD_COLOR_E Color); 
#else
extern void SKY_API SKY_SubBacklight(ONOFF OnOff);
#endif
extern void SKY_API SKY_RefreshSubDisplay(void);
extern void SKY_API SKY_SetSubDisplayContrast(HS_CONTRAST_E DisplayContrast, BYTE DisplayContrastLevel);
extern void SKY_API SKY_SubLCDTextOutBufferLen(LINE_NUMBER_E LineNum, CONST BYTE* pszString, UINT16 nLen);
#endif//#ifdef FEATURE_SKY_SUBLCD
extern void SKY_API SKY_SNI(HS_SNI_E SNI, ONOFF OnOff);
extern void SKY_API SKY_IdleScreenNotify(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_SCRN_H_ */

