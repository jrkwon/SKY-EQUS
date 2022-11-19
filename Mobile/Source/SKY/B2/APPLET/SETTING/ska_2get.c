/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_2get.c
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
** 4333-12-18 white     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "sky.h"
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SIC_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT| \
                                CS_SCROLL_LIST_COMBO | \
                                CS_SCROLL_LIST_ENUMBERATED | \
                                CS_SCROLL_LIST_WRAP
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

/*============================================================================= 
Function:    
SKA_MainXxxx

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
#if 0
void SKY_API SKA_MainHowToGet(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sHow2Get[2];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    sHow2Get[0] = (BYTE*)SKY_GET_TOKEN(TKN_ONLY_OPEN);
    sHow2Get[1] = (BYTE*)SKY_GET_TOKEN(TKN_OPEN_PRESS);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,
            (BYTE*)SKY_GET_TOKEN(TKN_HOW_TO_GETCALL), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Get, (UINT8)SKY_GET_HOW_TO_ANSWER());
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(0), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_SOFTKEY_SAVE:
        SKY_SET_HOW_TO_ANSWER((HOW_TO_ANSWER_E)EQC_GetRadioButton(EQS_GetNthIOC(0)));
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_IO_RADIO_BUTTON_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_GET_HOW_TO_ANSWER() != (HOW_TO_ANSWER_E)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_GET_HOW_TO_ANSWER() != (HOW_TO_ANSWER_E)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
       
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*============================================================================= 
Function:    
SKA_MainHowToTedit

  Description: 
  @Description
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_MainHowToTedit(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sHow2Edit[2];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    sHow2Edit[0] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_OLD_HANGUL);
    sHow2Edit[1] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_SKY_HANGUL);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,
            (BYTE*)SKY_GET_TOKEN(TKN_INPUT_SEL_HANGUL), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Edit, (UINT8)SKY_GET_SEL_HANGUL());
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(0), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_SOFTKEY_SAVE:
        SKY_SET_SEL_HANGUL((BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)));
        EQC_SetSKYHangulMode(SKY_GET_SEL_HANGUL());
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_IO_RADIO_BUTTON_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_GET_SEL_HANGUL() != (BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_GET_SEL_HANGUL() != (BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
    
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;        

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*============================================================================= 
 Function:    
     SKA_MainXxxx

 Description: 
     @Description

 Input:
     @ArgumentsList

 Output:
     @TheResultOfThisFunction

 Return value:
     @ReturnValue

 Side effects:
==============================================================================*/
void SKY_API SKA_MainSelectMode(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sHow2Mode[2];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    sHow2Mode[0] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_TEXT_MENU);
    sHow2Mode[1] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_ICON_MENU);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE,
            (BYTE*)SKY_GET_TOKEN(TKN_MENUMODE_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Mode, (UINT8)SKY_GET_SEL_MENU_MODE());
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(0), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_SOFTKEY_SAVE:
        SKY_SET_SEL_MENU_MODE((BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)));
	    EQC_SetMenuIconMode( g_hMenuControl, SKY_GET_SEL_MENU_MODE());
			
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_IO_RADIO_BUTTON_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_GET_SEL_MENU_MODE() != (BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_GET_SEL_MENU_MODE() != (BOOL)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
      
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
#endif //#if 0
enum tag_CONFIG_SET_E {
    CS_GET_TITLE,
    CS_GET_RADIO,
    CS_EDIT_TITLE,
    CS_EDIT_RADIO,
    CS_MODE_TITLE,
    CS_MODE_RADIO,
    CS_CONFIG_MAX
};

void SKY_API SKA_MainConfigSet(SKY_EVENT_T *pEvent)
{
    BYTE i;
    CONST BYTE* sHow2Edit[2];
    CONST BYTE* sHow2Mode[2];
    CONST BYTE* sHow2Get[2];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    sHow2Edit[0] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_OLD_HANGUL);
    sHow2Edit[1] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_SKY_HANGUL);

    sHow2Mode[0] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_TEXT_MENU);
    sHow2Mode[1] = (BYTE*)SKY_GET_TOKEN(TKN_SEL_ICON_MENU);

    sHow2Get[0] = (BYTE*)SKY_GET_TOKEN(TKN_ONLY_OPEN);
    sHow2Get[1] = (BYTE*)SKY_GET_TOKEN(TKN_OPEN_PRESS);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            200, 
            CL_NONE_SELECT_SAVE,
            (BYTE*)SKY_GET_TOKEN(TKN_OPTION_SELECT), //TKN_CONTROL_PANNEL_TITLE), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_HOW_TO_GETCALL);
        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Get, (UINT8)SKY_GET_HOW_TO_ANSWER());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_INPUT_SEL_HANGUL);
        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Edit, (UINT8)SKY_GET_SEL_HANGUL());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_MENUMODE_SET);
        EQC_RadioButton(EQS_NA, EQS_NA, 6, 2, (BYTE **)sHow2Mode, (UINT8)SKY_GET_SEL_MENU_MODE());
        
        EQS_StartFormatter();

        for(i = 0; i < CS_CONFIG_MAX ; i++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(i), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        EQS_FitCanvasSize();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            if(s_fEndToRoot)
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
                s_fEndToRoot = FALSE;
            }
            else
                EQS_End();
        }
        break;

    case SE_APP_IO_MODE:
        EQS_SetWindowCML(CL_NONE_CONFIRM_SAVE);
        break;

    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_IO_RADIO_BUTTON_CANCEL:
        EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        break;

    case SE_SOFTKEY_SAVE:
        //EQS_GetNthIOC(SIC_ALERT_SCRL))
        SKY_SET_HOW_TO_ANSWER((HOW_TO_ANSWER_E)EQC_GetRadioButton(EQS_GetNthIOC(CS_GET_RADIO)));
        SKY_SET_SEL_HANGUL((BOOL)EQC_GetRadioButton(EQS_GetNthIOC(CS_EDIT_RADIO)));
        EQC_SetSKYHangulMode(SKY_GET_SEL_HANGUL());
        SKY_SET_SEL_MENU_MODE((BOOL)EQC_GetRadioButton(EQS_GetNthIOC(CS_MODE_RADIO)));
	    EQC_SetMenuIconMode( g_hMenuControl, SKY_GET_SEL_MENU_MODE());

        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
     
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
       
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCFocus(EQS_GetNthIOC(0));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



