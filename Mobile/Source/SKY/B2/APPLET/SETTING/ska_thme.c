/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_thme.c
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
** 2001-02-17 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* rosa 2001-09-24 --> @why, what */
#define MAX_NUM_THEME_BACK      2//CSB_MAX
#define MAX_NUM_THEME_FORE      6//CSF_MAX

#define COLOR_SCROLL_LIST_CHAR  16
#define COLOR_SCROLL_LIST_SIZE  3
#define COLOR_GET_SLSIZE(n)     (UINT8)((COLOR_SCROLL_LIST_SIZE < n)?COLOR_SCROLL_LIST_SIZE:n)

enum tagSET_THEME_COLOR_E {
    STC_BACK_TITLE,
    STC_BACK,           // Scroll List
    STC_FORE_TITLE,
    STC_FORE,           // Scroll List
    STC_MAX
};
/* <-- */

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
/* rosa 2001-09-24 --> @why, what */
LOCAL ROM SKY_TOKEN l_ThemeBackStr[MAX_NUM_THEME_BACK] = {
    TKN_THEME_BACK_WHITE,
    TKN_THEME_BACK_BLACK
};

LOCAL ROM SKY_TOKEN l_ThemeForeStr[MAX_NUM_THEME_FORE] = {
    TKN_THEME_FORE_BLUE,
    TKN_THEME_FORE_RED,
    TKN_THEME_FORE_GREEN,
    TKN_THEME_FORE_PURPLE,        
    TKN_THEME_FORE_MARIN,
    TKN_THEME_FORE_ORANGE
};
/* <-- */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
//#define sINDICATOR_COLOR        EQS_RGB(254,98,50)

LOCAL ROM COLOR aCmlBluePalette[] = {
    EQS_RGB(190,209,246), EQS_RGB(45,85,196), EQS_RGB(141,165,235), WHITE
};

LOCAL ROM COLOR aCmlRedPalette[] = {
    EQS_RGB(255,199,194), EQS_RGB(217,40,0), EQS_RGB(255,134,129), WHITE
};

LOCAL ROM COLOR aCmlGreenPalette[] = {
    EQS_RGB(193,249,188), EQS_RGB(63,209,56), EQS_RGB(122,233,117), WHITE
};

LOCAL ROM COLOR aCmlPurplePalette[] = {
    EQS_RGB(253,210,250), EQS_RGB(194,2,165), EQS_RGB(243,136,226), WHITE
};

LOCAL ROM COLOR aCmlMarinePalette[] = {
    EQS_RGB(214,180,242), EQS_RGB(64,25,88), EQS_RGB(139,81,175), WHITE
};

LOCAL ROM COLOR aCmlOrangePalette[] = {
    EQS_RGB(255,233,178), EQS_RGB(232,112,0), EQS_RGB(255,178,106), WHITE
};



LOCAL ROM COLOR* ROM aCommandLinePalette[] = {
    aCmlBluePalette,
    aCmlRedPalette,
    aCmlGreenPalette,
    aCmlPurplePalette,
    aCmlMarinePalette,
    aCmlOrangePalette
};

void SKY_API SKY_SetCommandLineColor(COLOR_SCHEME_E ColorScheme)
{
    if(ColorScheme >= COLOR_SCHEME_COUNT)
        ColorScheme -= COLOR_SCHEME_COUNT ;

    SetPaletteColor4G(&g_BmpControlCommandLineLeft, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpControlCommandLineCenter, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpControlCommandLineRight, aCommandLinePalette[ColorScheme]);

    //SetPaletteColor4G(&g_BmpCommandLineIdle, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpCommandLineLeft, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpCommandLineCenter, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpCommandLineRight, aCommandLinePalette[ColorScheme]);
    

    /* command line for wap */
    SetPaletteColor4G(&g_BmpWapCommandLineCenter, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpWapCommandLineCenterArrow, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpWapCommandLineCenterOK, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpWapCommandLineLeft, aCommandLinePalette[ColorScheme]);
    SetPaletteColor4G(&g_BmpWapCommandLineRight, aCommandLinePalette[ColorScheme]);
    
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainXxx()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     pEvent :
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/

void SKY_API SKA_MainSetTheme(SKY_EVENT_T *pEvent)
{
    UINT8 nIndexBack, nIndexFore, nIndex;
    UINT8 i;
    /* rosa 2001-09-24 --> @why, what */
    BYTE *ppThemeBack[MAX_NUM_THEME_BACK];
    BYTE *ppThemeFore[MAX_NUM_THEME_FORE];
    /* <-- */
    COLOR TextColorTable[] = { 
        BLACK, WHITE
    };
    STATIC BOOL s_fEndMessage, s_fSaveMessage, s_fToIdle;
    //STATIC COLOR_SCHEME_E s_ColorScheme;
    

    switch(pEvent->EventID)
    {
    case SE_APP_START :
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_THEME_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        s_fEndMessage = s_fSaveMessage = s_fToIdle = FALSE;

        /* rosa 2001-09-25 --> @why, what */
        for(i=0; i< MAX_NUM_THEME_BACK; i++)
            ppThemeBack[i] = (BYTE*)SKY_GET_TOKEN(l_ThemeBackStr[i]);

        for(i=0; i< MAX_NUM_THEME_FORE; i++)
            ppThemeFore[i] = (BYTE*)SKY_GET_TOKEN(l_ThemeForeStr[i]);
        
        /*s_ColorScheme = */nIndexFore = nIndex = SKY_GET_COLOR_SCHEME();
        nIndexBack = CSB_WHITE;
        if(nIndex >= COLOR_SCHEME_COUNT)
        {
            nIndexBack = CSB_BLACK;
            nIndexFore -= COLOR_SCHEME_COUNT;
        }

        // create
        EQC_StaticText(EQS_NA, EQS_NA, TKN_THEME_BACK);
        EQC_ScrollCombo(EQS_NA, 
                        EQS_NA, 
                        COLOR_SCROLL_LIST_CHAR, 
                        COLOR_GET_SLSIZE(MAX_NUM_THEME_BACK), 
                        ppThemeBack, 
                        MAX_NUM_THEME_BACK, 
                        MAX_NUM_THEME_BACK,
                        nIndexBack);
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_THEME_FORE);
        EQC_ScrollCombo(EQS_NA, 
                        EQS_NA, 
                        COLOR_SCROLL_LIST_CHAR, 
                        COLOR_GET_SLSIZE(MAX_NUM_THEME_FORE), 
                        ppThemeFore, 
                        MAX_NUM_THEME_FORE, 
                        MAX_NUM_THEME_FORE,
                        nIndexFore);
        
        // Screen Formatting
        EQS_StartFormatter();
        for(i = 0; i < STC_MAX; i++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(i), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();
        /* <-- */
        break;
        
    case SE_APP_END:
        //if(SKY_GET_COLOR_SCHEME() != s_ColorScheme)
        {
            EQS_SetTheme(SKY_GET_COLOR_SCHEME(), SKY_GET_FONT_TYPE(), SKY_GET_SCROLLBAR_SIZE());
            SKY_SetCommandLineColor(SKY_GET_COLOR_SCHEME());
        }
        break;       
       
    case SE_IO_MESGBOX_NO:
        if(s_fToIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(s_fSaveMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_YES:
        if(!s_fSaveMessage)
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            s_fEndMessage = TRUE;
            break;
        }
        // fall through
    case SE_SOFTKEY_SAVE:
        nIndexBack = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(STC_BACK));
        nIndexFore = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(STC_FORE));

        /* rosa 2001-09-25 --> @why, what */
        SKY_SET_COLOR_SCHEME((nIndexFore + (nIndexBack == CSB_BLACK ? COLOR_SCHEME_COUNT : 0)));
        // comfirm message
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_THEME_TITLE_MSG), 
					    MESSAGE_DEFAULT_TIME, 
                        MBT_NONE, 
                        MBT_SUCCESS);
        /* <-- */
        s_fEndMessage = TRUE;
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_IO_SCROLL_LIST_CHG:
        nIndexBack = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(STC_BACK));
        nIndexFore = EQC_GetScrollLogicalIndex(EQS_GetNthIOC(STC_FORE));

        EQS_SetTheme((nIndexFore + (nIndexBack == CSB_BLACK ? COLOR_SCHEME_COUNT : 0)), 
                        SKY_GET_FONT_TYPE(), SKY_GET_SCROLLBAR_SIZE());
        SKY_SetCommandLineColor((COLOR_SCHEME_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(STC_FORE)));

        /* set static text color for updating live */
        EQC_SetStaticTextColor(EQS_GetNthIOC(STC_BACK_TITLE), TextColorTable[nIndexBack]);
        EQC_SetStaticTextColor(EQS_GetNthIOC(STC_FORE_TITLE), TextColorTable[nIndexBack]);

        EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_END:
        if(s_fToIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(s_fEndMessage)
            EQS_End();
        break;

    case SE_KEY_END:
        s_fToIdle = TRUE;
        // fall through
    case SE_KEY_CLEAR:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
			s_fSaveMessage = TRUE;
		}
		else
		{
			if(s_fToIdle)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

