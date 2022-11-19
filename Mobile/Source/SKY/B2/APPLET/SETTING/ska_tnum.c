/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_tnum.c
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
#define TNC_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT| \
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
//void SKY_API sSKA_TwoNum(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_TwoNum(SKY_EVENT_T *pEvent);
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
void SKY_API SKA_MainTwoNum(SKY_EVENT_T *pEvent)
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
                SKY_GET_TOKEN(TKN_TWO_NUM_SRVC_SET), 
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
            EQS_START_SIBLING(sSKA_TwoNum, FALSE);
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
LOCAL void SKY_API sSKA_TwoNum(SKY_EVENT_T *pEvent)
{
    STATIC BYTE  sNum[12];
    STATIC HCONTROL s_hMsgControl, s_hIdentMsg;
    UINT8   nIdx;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            160,//SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            SKY_GET_TOKEN(TKN_TWO_NUM_SRVC_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        TwoNumberBellCtrl();

#if 0  //000000000000000000000000000000000000000000000000000000000000000000000000000
//        if(!EQC_GetCheckBox(EQS_GetNthIOC(TNC_CHECK_BOX),  0))
//       {
//            EQC_SET_FOCUS(EQS_GetNthIOC(TNC_BELL_SCRL), FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */
//        }else
//        {
//            EQC_SET_FOCUS(EQS_GetNthIOC(TNC_BELL_SCRL), TRUE);
//        }
#endif //000000000000000000000000000000000000000000000000000000000000000000000000000

        EQS_FitCanvasSize();
        break;
        
    case SE_SOFTKEY_CANCEL:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;

    case SE_APP_END :   /* The very last event */
        TwoNumberBellCtrlFree();
        SKY_StopMusic();
        EQS_ReleaseDC();
        break;
     
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
    
    case SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(TNC_ADDNUM_EDIT), sNum);
        if(!STRCMP(GetMyNumber(), (BYTE*)sNum))
        {
            s_hIdentMsg = EQC_MessageBox(SKY_GET_TOKEN(TKN_CANT_NOT_SET_NUM),MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        break;

    case SE_TM_TIMER1:
        if(EQS_HaveFocus(EQS_GetNthIOC(TNC_BELL_SCRL)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            PlayTwoNumBell();
        }
        break;

    case SE_APP_IO_MODE:
        if(EQS_GetIOCOrder((UINT8)(pEvent->wParam)) == TNC_BELL_SCRL)
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
            break;

    case SE_IO_SCROLL_LIST_SELECT:
    case SE_IO_FOCUS_CHG:
        SKY_StopMusic();
        if(pEvent->EventID == SE_IO_FOCUS_CHG)
        {
            if(pEvent->wParam == EQS_GetNthIOC(TNC_CHECK_BOX))
            {
                if(EQC_GetCheckBox(EQS_GetNthIOC(TNC_CHECK_BOX), 0))
                    EQS_SetWindowCML(CL_NONE_DISSOLVE_SAVE);
                else
                    EQS_SetWindowCML(CL_NONE_SET_SAVE);
            }else if(pEvent->wParam == EQS_GetNthIOC(TNC_BELL_SCRL))
            {
                EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
                EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
            }else
                EQS_SetWindowCML(CL_NONE_SELECT_SAVE);
        }
        //TwoNumberBellChgScrl(EQS_GetIOCOrder((UINT8)(pEvent->wParam)), pEvent->dwParam);
        break;

#if 0//00000000000000000000000000000000000000000000000000000000000000000000000000
//    case SE_IO_CHECK_BOX_CHECK:
//    case SE_IO_CHECK_BOX_UNCHECK:
//        if(!EQC_GetCheckBox(EQS_GetNthIOC(TNC_CHECK_BOX),  0))
//        {
//            EQC_SET_FOCUS(EQS_GetNthIOC(TNC_BELL_SCRL), FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */
//        }else
//        {
//            EQC_SET_FOCUS(EQS_GetNthIOC(TNC_BELL_SCRL), TRUE);
//        }
//        break;
#endif//0000000000000000000000000000000000000000000000000000000000000000000000000

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case TNC_BELL_SCRL:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(TNC_BELL_SCRL))
        {
            SKY_StopMusic();
        }
        break;    

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_CANCEL),MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
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
        }else if(pEvent->wParam == s_hIdentMsg)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(TNC_ADDNUM_EDIT));    
        }
        break;

    case SE_SOFTKEY_SAVE:
        SKY_StopTone();
        SKY_StopMusic();

        EQC_GetNumberEditValue(EQS_GetNthIOC(TNC_ADDNUM_EDIT), sNum);

        if(!STRCMP(GetMyNumber(), (BYTE*)sNum))
        {
            s_hIdentMsg = EQC_MessageBox(SKY_GET_TOKEN(TKN_CANT_NOT_SET_NUM),MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            break;
        }
        
        if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
        {
            SKY_SET_NUMBER_PLUS_NUMBER_SKT(sNum);            
        }
        else
        {                
            SKY_SET_NUMBER_PLUS_NUMBER_STI(sNum);
        }

        if(EQC_GetCheckBox(EQS_GetNthIOC(TNC_CHECK_BOX), 0))
        {
            SKY_SET_NUMBER_PLUS(TRUE);
        }
        else
        {
            SKY_SET_NUMBER_PLUS(FALSE);
        }

        SaveTwoNumBell();
        s_hMsgControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_KEY_CLEAR:
        EQC_GetNumberEditValue(EQS_GetNthIOC(TNC_ADDNUM_EDIT), sNum);
        if(EQS_IsControlDataChanged())
        {
            s_hMsgControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
         
        }else
			EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        EQC_GetNumberEditValue(EQS_GetNthIOC(TNC_ADDNUM_EDIT), sNum);
        if(EQS_IsControlDataChanged())
        {
            s_hMsgControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
		{
			s_fEndToRoot = FALSE;
			EQS_END_TO(EQS_POP_TO_ROOT);
		}
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

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}