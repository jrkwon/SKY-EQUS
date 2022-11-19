/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_mans.c
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
** 4333-12-15 white     Created.
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
void SKY_API sSKA_NAMSelect(SKY_EVENT_T *pEvent);
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
void SKY_API SKA_MainNAMSelect(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        {

            EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_NONE_CANCEL, 
                SKY_GET_TOKEN(TKN_NAM_SET), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
            //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

            EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT); 
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
			EQS_SetIOCActive(sNumHandle);
        }
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));//EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;
        
    case SE_IO_MESGBOX_END:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASSWORD_EDIT), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(PI_PASSWORD_EDIT));
        break;

    case EQE_IO_NEDIT_MATCH_LENGTH:                         //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
            EQS_START_SIBLING(sSKA_NAMSelect, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_APP_DRAW:
        TextOutInputCodeMessage();
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
===============================================================================*/
void SKY_API sSKA_NAMSelect(SKY_EVENT_T *pEvent)
{
    CONST BYTE* sNAMSelect[5];
    STATIC HCONTROL s_hMsgControl;
    nv_item_type  nvi;
    STATIC BOOL s_fEndToRoot = FALSE;

    sNAMSelect[0] = SKY_GET_TOKEN(TKN_NAM_1);
    sNAMSelect[1] = SKY_GET_TOKEN(TKN_NAM_2);
    sNAMSelect[2] = SKY_GET_TOKEN(TKN_NAM_3);
    sNAMSelect[3] = SKY_GET_TOKEN(TKN_NAM_4);
    sNAMSelect[4] = SKY_GET_TOKEN(TKN_NAM_5);

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 160, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160, //SKY_DEF_WIN_DY,
            CL_NONE_SELECT_NONE,
            SKY_GET_TOKEN(TKN_NAM_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 5, (BYTE **)sNAMSelect, (UINT8)SKY_GET_CURRENT_NAM());
        EQS_SetIOCActive(EQS_GetNthIOC(0));

        EQS_StartFormatter();

        EQS_SetControlPosition(EQS_GetNthIOC(0), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
        EQS_NewLine();

        EQS_EndFormatter();
        EQC_SET_STYLE(EQS_GetNthIOC(0), (EQC_GET_STYLE(EQS_GetNthIOC(0)) & ~CS_RADIO_BUTTON_BORDER) | CS_RADIO_BUTTON_HMAXIMIZE);

        break;
        
    case SE_IO_RADIO_BUTTON_SELECT:
    case SE_SOFTKEY_SAVE:
        SKY_SET_CURRENT_NAM((CM_NAM_E)EQC_GetRadioButton(EQS_GetNthIOC(0)));
        nvi.curr_nam = (BYTE)SKY_GET_CURRENT_NAM();
        SKY_PutNV(NV_CURR_NAM_I, &nvi);
        s_hMsgControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE , MBT_SUCCESS);
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_SOFTKEY_CANCEL:
    case SE_IO_RADIO_BUTTON_CANCEL:
    case SE_KEY_CLEAR:
        if(SKY_GET_CURRENT_NAM() != (CM_NAM_E)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(SKY_GET_CURRENT_NAM() != (CM_NAM_E)EQC_GetRadioButton(EQS_GetNthIOC(0)))
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
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
    
    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            SKY_PowerOff(POM_RESET);//ui_power_reset();
            EQS_End();
        }
        break;
        
    case SE_IO_MESGBOX_CLEAR:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

