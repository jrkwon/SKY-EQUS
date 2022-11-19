/*
** ===========================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_scrn.c
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
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>   /* vsprintf */
#include <stdarg.h>  /* va_list */
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
//#define TRANSPARENT_COLOR       RGB332(0, 0, 64)
/* initialize EQUS graphics. must be called before EQS_Initialize */
void SKY_API SKY_InitScreen(SCREEN *pMain, SCREEN *pSub)
{
    //BOOL fResult = TRUE;
    
    /* main screen */
    OpenGraph(pMain, SI_MAIN_SCREEN, INIT_FONT_TYPE);
    //SetFontType(INIT_FONT_TYPE);    /* system font */
    //EQS_SetTheme(SKY_GET_COLOR_SCHEME(), SKY_GET_FONT_TYPE(), SKY_GET_SCROLLBAR_SIZE());
    
    /*
    ** set transparent color 
    ** we can't use this dark blue-RGB(0,0,64) color in the bitmap image.
    */
//    SetTransparentColor(TRANSPARENT_COLOR);
        
    EQS_InitDC();

    /* sub screen */
    OpenGraph(pSub, SI_SUB_SCREEN, FT_MED);
    //SetFontType(FT_MED);//INIT_FONT_TYPE);    /* system font */
    /* set view port of the sub screen */
    {
        RECT Rect;

        SelectScreen(SI_SUB_SCREEN);

        Rect.xLeft = 0;
        Rect.yTop = SKY_SUB_SCREEN_Y_OFFSET;
        Rect.xRight = DevGetMaxX();
        Rect.yBottom = DevGetMaxY();
        SetViewPortRect(&Rect);
        SetTextAlignHoriz(TAH_CENTER);
    }

    SelectScreen(SI_MAIN_SCREEN);
}

#ifdef FEATURE_SKY_SUBLCD /* ---------------------------------------------> */

#if   SKY_MODEL_ID == SKY_MODEL_B0
LOCAL ROM l_LineNumber[] = { 0 };
#elif   (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
LOCAL ROM l_LineNumber[] = { 0, 16/*17*/ };
#endif

#define sSUB_LCD_TEXT_DRAW_MODE     TDM_NORMAL//TDM_BOLD

/* 
** Default event handler draws Annunciators automatically.
** So, we need the flag about annunciators can be displayed or not
*/
LOCAL BOOL l_fEndText[LN_MAX] = { TRUE, TRUE }; 

/* default display in sub lcd */
void SKY_API SKY_SubLCDDefault(void)
{
    SKY_SubLCDEndText(LN_UPPER);
    SKY_SubLCDEndText(LN_LOWER);
    /* jrkwon 2001-05-04 --> */
#ifdef WAPPER
    if(SKY_IS_IN_WAP())
    {
        SendCommand2Wap(WAP_SLCD_DISP_ANNUN);
        SendCommand2Wap(WAP_SLCD_DISP_TIME);
    }
    else
#endif
    {
        SKY_DrawSubAnnunciators();
        DisplayCurrentDateTimeInSubLCD(TRUE);
    }
    /* <-- */
}

void SKY_API SKY_SubLCDEndText(LINE_NUMBER_E LineNum)
{
    l_fEndText[LineNum] = TRUE;
}

BOOL SKY_API SKY_SubLCDIsEndText(LINE_NUMBER_E LineNum)
{
    return l_fEndText[LineNum];
}

void SKY_API SKY_SubLCDTextOutStr(LINE_NUMBER_E LineNum, CONST BYTE* pszString)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], pszString, sSUB_LCD_TEXT_DRAW_MODE);
        l_fEndText[LineNum] = FALSE;

        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutBufferLen(LINE_NUMBER_E LineNum, CONST BYTE* pszString, UINT16 nLen)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        CLEAR_SUB_LCD(LineNum);
        TextOutBufferLenTDM(SUB_LCD_X, l_LineNumber[LineNum], pszString, sSUB_LCD_TEXT_DRAW_MODE, nLen);
        l_fEndText[LineNum] = FALSE;

        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutStrTDM(LINE_NUMBER_E LineNum, CONST BYTE* pszString, 
                                     TEXT_DRAW_MODE TextDrawModeA)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], pszString, TextDrawModeA);
        l_fEndText[LineNum] = FALSE;
    
        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutStrF(LINE_NUMBER_E LineNum, CONST BYTE* pszFormat, ...)
{
    //if(IsScreenChangable())
    {
        va_list vaArgPtr;
        BYTE szString[EQS_TO_MAX_STRLEN+1];

        OPEN_SUB_LCD();

        va_start(vaArgPtr, pszFormat);
    
        vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], szString, sSUB_LCD_TEXT_DRAW_MODE);
        l_fEndText[LineNum] = FALSE;
    
        va_end(vaArgPtr);

        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutStrTDMF(LINE_NUMBER_E LineNum, TEXT_DRAW_MODE TextDrawModeA, 
                                      CONST BYTE* pszFormat, ...)
{
    //if(IsScreenChangable())
    {
        va_list vaArgPtr;
        BYTE szString[EQS_TO_MAX_STRLEN+1];

        OPEN_SUB_LCD();

        va_start(vaArgPtr, pszFormat);
    
        vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], szString, TextDrawModeA);
        l_fEndText[LineNum] = FALSE;
    
        va_end(vaArgPtr);
    
        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOut(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], SKY_GET_TOKEN(TokenId), sSUB_LCD_TEXT_DRAW_MODE);
        l_fEndText[LineNum] = FALSE;
    
        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutTDM(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId, 
                                  TEXT_DRAW_MODE TextDrawMode)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], SKY_GET_TOKEN(TokenId), TextDrawMode);
        l_fEndText[LineNum] = FALSE;
    
        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutF(LINE_NUMBER_E LineNum, SKY_TOKEN TokenId, ...)
{
    //if(IsScreenChangable())
    {
        va_list vaArgPtr;
        BYTE szString[EQS_TO_MAX_STRLEN+1];
        CONST BYTE* pszFormat = SKY_GET_TOKEN(TokenId);

        OPEN_SUB_LCD();

        va_start(vaArgPtr, pszFormat);
    
        vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], szString, sSUB_LCD_TEXT_DRAW_MODE);
        l_fEndText[LineNum] = FALSE;
    
        va_end(vaArgPtr);
    
        CLOSE_SUB_LCD();
    }
}

void SKY_API SKY_SubLCDTextOutTDMF(LINE_NUMBER_E LineNum, TEXT_DRAW_MODE TextDrawMode,
                                   SKY_TOKEN TokenId, ...)
{
    //if(IsScreenChangable())
    {
        va_list vaArgPtr;
        BYTE szString[EQS_TO_MAX_STRLEN+1];
        CONST BYTE* pszFormat = SKY_GET_TOKEN(TokenId);

        OPEN_SUB_LCD();

        va_start(vaArgPtr, pszFormat);
    
        vsprintf((CHAR*)szString, (CHAR*)pszFormat, vaArgPtr);
        CLEAR_SUB_LCD(LineNum);
        TextOutStrTDM(SUB_LCD_X, l_LineNumber[LineNum], szString, TextDrawMode);
        l_fEndText[LineNum] = FALSE;
    
        va_end(vaArgPtr);
    
        CLOSE_SUB_LCD();
    }
}


void SKY_API SKY_SubLCDPutBitmap(COORD x, COORD y, CONST BITMAP* pBitmap)
{
    //if(IsScreenChangable())
    {
        OPEN_SUB_LCD();

        PutBitmap(x, y, pBitmap);
    
        CLOSE_SUB_LCD();
    }
}

#endif /* #ifdef FEATURE_SKY_SUBLCD --------------------------------------> */



void SKY_API SKY_Screen(ONOFF OnOff)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SCREEN;
        pCommand->Header.pTask = NULL;
        pCommand->Control.fOn = OnOff;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set screen");
    }
}

#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_SubScreen(ONOFF OnOff)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SUB_SCREEN;
        pCommand->Header.pTask = NULL;
        pCommand->Control.fOn = OnOff;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set sub screen");
    }
}

#endif//#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_Backlight(ONOFF OnOff)
{
    HS_COMMAND_U *pCommand;      /* command to handset */
#ifdef SW_RESET_ENABLE
    if(mc_bSwReset)
	    return;  
#endif
    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_BACKLIGHT;
        pCommand->Header.pTask = NULL;
        pCommand->Control.fOn = OnOff;
        pCommand->Control.SensorOn=TRUE;        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set backlight");
    }
}
void SKY_API SKY_BacklightSensor(ONOFF OnOff)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_BACKLIGHT;
        pCommand->Header.pTask = NULL;
        pCommand->Control.fOn = OnOff;
        pCommand->Control.SensorOn=TRUE;
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set backlight");
    }
}
#ifdef FEATURE_SKY_SUBLCD
#ifdef FEATURE_COLOR_ID_LCD
void SKY_API SKY_SubBacklight(ONOFF OnOff, ID_LCD_COLOR_E Color) 
#else
void SKY_API SKY_SubBacklight(ONOFF OnOff)
#endif //FEATURE_COLOR_ID_LCD
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SUB_BACKLIGHT;
        pCommand->Header.pTask = NULL;
        pCommand->Control.fOn = OnOff;
#ifdef FEATURE_COLOR_ID_LCD
        pCommand->Control.Color = Color;
#endif //FEATURE_COLOR_ID_LCD
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set sub backlight");
    }
}

void SKY_API SKY_SubBacklight_Confirm(ID_LCD_COLOR_E Color) 
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SUB_BACKLIGHT_CONFIRM;
        pCommand->Header.pTask = NULL;
#ifdef FEATURE_COLOR_ID_LCD
        pCommand->Control.Color = Color;
#endif //FEATURE_COLOR_ID_LCD
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set sub backlight");
    }
}

#endif//#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_RefreshDisplay(void)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_SCREEN;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    }
}

void SKY_API SKY_RefreshDisplay_256C(void)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_SCREEN_256C;
        pCommand->Header.pTask = NULL;
        hs_cmd( pCommand );
    }
}



#ifdef FEATURE_PARTIAL_DISPLAY
void SKY_API SKY_RefreshPartialDisplay(BYTE *pbBuffer, BYTE nXpos, BYTE nYpos, BYTE nXendpos, BYTE nYendpos)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_PARTIAL_SCREEN;
		pCommand->Header.pTask = NULL;
        pCommand->PartialScreen.Buffer = pbBuffer;
		pCommand->PartialScreen.StartX = nXpos;
		pCommand->PartialScreen.StartY = nYpos;
		pCommand->PartialScreen.EndX = nXendpos;
		pCommand->PartialScreen.EndY = nYendpos;
        hs_cmd( pCommand );
    }
}

void SKY_API SKY_RefreshPartialDisplay_256C(BYTE *pbBuffer, BYTE nYpos, BYTE nYendpos)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = 	HC_UPDATE_PARTIAL_DISPLAY_256C;
		pCommand->Header.pTask = NULL;
        pCommand->PartialScreen.Buffer = pbBuffer;
		pCommand->PartialScreen.StartY = nYpos;
		pCommand->PartialScreen.EndY = nYendpos;
        hs_cmd( pCommand );
    }
}


void SKY_API SKY_RefreshDisplayLine(BYTE *pbBuffer, BYTE nYpos, BYTE nYendpos)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_PARTIAL_DISPLAY_LINE;
		pCommand->Header.pTask = NULL;
        pCommand->PartialScreen.Buffer = pbBuffer;
		pCommand->PartialScreen.StartY = nYpos;
		pCommand->PartialScreen.EndY = nYendpos;
        hs_cmd( pCommand );
    }
	
}

void SKY_API SKY_RefreshDisplayRect(BYTE *pbBuffer, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_PARTIAL_DISPLAY_RECT;
		pCommand->Header.pTask = NULL;
        pCommand->PartialScreen.Buffer = pbBuffer;
		pCommand->PartialScreen.StartX = xleft;
		pCommand->PartialScreen.StartY = ytop;
		pCommand->PartialScreen.EndX = xright;
		pCommand->PartialScreen.EndY = ybottom;
        hs_cmd( pCommand );
    }
}

void SKY_API SKY_RefreshDisplayRectByRegion(BYTE *pbBuffer, UINT8 xleft, UINT8 ytop, UINT8 xright, UINT8 ybottom)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_PARTIAL_DISPLAY_RECT_BY_REGION;
		pCommand->Header.pTask = NULL;
        pCommand->PartialScreen.Buffer = pbBuffer;
		pCommand->PartialScreen.StartX = xleft;
		pCommand->PartialScreen.StartY = ytop;
		pCommand->PartialScreen.EndX = xright;
		pCommand->PartialScreen.EndY = ybottom;
        hs_cmd( pCommand );
    }
}
#endif


#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_RefreshSubDisplay(void)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_UPDATE_SUB_SCREEN;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    }
}

#endif//#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_SetDisplayContrast(HS_CONTRAST_E DisplayContrast, BYTE DisplayContrastLevel)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_CONTRAST;
        pCommand->Header.pTask = NULL;
        pCommand->Contrast.Contrast = DisplayContrast;
		//contrast기준값 항목 추가 happykid 01.05.08
		pCommand->Contrast.ContrastLevel = DisplayContrastLevel;

        hs_cmd( pCommand );
    }
}

#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_SetSubDisplayContrast(HS_CONTRAST_E DisplayContrast, BYTE DisplayContrastLevel)
{
    HS_COMMAND_U *pCommand;           /* command to handset */

    if( ( pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SUB_CONTRAST;
        pCommand->Header.pTask = NULL;
        pCommand->Contrast.Contrast = DisplayContrast;
		//contrast기준값 항목 추가 happykid 01.05.09
		pCommand->Contrast.ContrastLevel = DisplayContrastLevel;

        hs_cmd( pCommand );
    }
}

#endif//#ifdef FEATURE_SKY_SUBLCD

void SKY_API SKY_SNI(HS_SNI_E SNI, ONOFF OnOff)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_SNI;
        pCommand->Header.pTask = NULL;
        pCommand->SNI.SNI = SNI;
        pCommand->SNI.OnOff = OnOff;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't set backlight");
    }
}

void SKY_API SKY_IdleScreenNotify(void)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HC_IDLE_SCREEN;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't notify idle screen update");
    }
}

void SKY_API SKY_PartialDisplayOn(void)
{
    HS_COMMAND_U *pCommand;      /* command to handset */

    /* turn the screen on or off */
    if ( (pCommand = (HS_COMMAND_U *) q_get( &hs_cmd_free_q )) != NULL ) 
    {
        pCommand->Header.Command = HS_PARTIAL_DIS_ON;
        pCommand->Header.pTask = NULL;
        
        hs_cmd( pCommand );
    } 
    else 
    {
        SKY_ERROR("Couldn't notify idle screen update");
    }
}