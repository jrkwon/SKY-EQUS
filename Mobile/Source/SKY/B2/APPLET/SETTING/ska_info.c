/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     
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
enum tag_PHONE_INFO_E {
    PI_NAME_TITLE,
    PI_NAME_INPUT,
    PI_PASS_TITLE,
    PI_PASS_INPUT,
    PI_INFO_MAX
};
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
LOCAL void SKY_API sSKA_PasswordChange(SKY_EVENT_T *pEvent);
//LOCAL void SKY_API sSKA_PhoneInfo(SKY_EVENT_T *pEvent);
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
/*
void SKY_API SKA_MainPhoneInfo(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    STATIC HCONTROL sHandle;
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
    case SE_APP_START : 
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CML_CANCEL_NONE_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_PHONE_INFO), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        sHandle = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INPUT_PASSWORD), 2, MBT_NONE, (BYTE*)"");

        break;
        
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == sHandle)
        {
            EQC_StaticText(37, 25, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(45, 40, 4, 1, 34,"");
        
            EQC_SET_STYLE(EQS_GetNthIOC(1), CS_NUMBER_EDIT_PASSWD | \
                    CS_NUMBER_EDIT_PLAIN_NUM |CS_NUMBER_EDIT_BORDER|CS_NUMBER_EDIT_NO_CML);

            EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
        }

        EQC_SetNumberEditValue(EQS_GetNthIOC(1), (BYTE*)"");
        EQS_SetIOCActive(EQS_GetNthIOC(1));

        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive(EQS_GetNthIOC(1));
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;

    case SE_IO_NEDIT_MATCH_LENGTH:                     //SE_IO_NEDIT_DONE:
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
        if(!STRCMP(sPasswd, SKY_GET_PASSWORD()))
           EQS_START_SIBLING( sSKA_PhoneInfo ,FALSE);        
        else
           EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), 2, MBT_NONE, (BYTE*)""); 
        break;

    case SE_APP_END :   
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: 
        break;
        
    case SE_APP_RESUME: 
        break;
        
   default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
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
void SKY_API SKA_MainPhoneInfo(SKY_EVENT_T *pEvent)//void SKY_API sSKA_PhoneInfo(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;
    STATIC HCONTROL s_hEndControl;

    STATIC BYTE   sPasswd[5];
    STATIC BYTE   sAgnPasswd[5];
    STATIC BYTE   pMsgStr[MAX_BANNER_LENGTH+1];
    STATIC BOOL   sIdent = FALSE, s_Again = FALSE;
    UINT8       nCtrl;
    
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
            CL_NONE_NONE_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_PHONE_INFO), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        STRCPY((BYTE*)pMsgStr, SKY_GET_BANNER());
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHONE_NAME);
        EQC_TextEdit(EQS_NA,EQS_NA, 19,1 , TEM_KOREAN, MAX_BANNER_LENGTH,pMsgStr,0);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_NEW_PASSWORD);
        EQC_NumberEdit(EQS_NA, EQS_NA, 4, 1, 34, SKY_GET_PASSWORD());//(BYTE *)"");
        //EQC_StaticText(1, 59, TKN_CONFIRM_PASSWORD);
        //EQC_NumberEdit(92, 59, 4, 1, 34, SKY_GET_PASSWORD());//(BYTE *)"");
        //EQS_SetIOCActive(EQS_GetNthIOC(1));
      
        EQC_SET_STYLE(EQS_GetNthIOC(PI_PASS_INPUT), CS_NUMBER_EDIT_PASSWD | \
                CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);
        EQC_NotifyNumberEditMatchLen(EQS_GetNthIOC(PI_PASS_INPUT), 4);

        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < PI_INFO_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();

        //EQC_SET_STYLE(EQS_GetNthIOC(5), CS_NUMBER_EDIT_PASSWD | 
        //        CS_NUMBER_EDIT_PLAIN_NUM | CS_NUMBER_EDIT_BORDER);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_SOFTKEY_SAVE:
        EQC_GetTextEditData(EQS_GetNthIOC(PI_NAME_INPUT), pMsgStr);
        SKY_SET_BANNER(pMsgStr);
        if(sIdent)
        {
            EQC_GetNumberEditValue(EQS_GetNthIOC(PI_NAME_INPUT), sAgnPasswd);
            SKY_SET_PASSWORD(sAgnPasswd);
        }else
            SKY_SET_PASSWORD(SKY_GET_PASSWORD());
        
        s_hEndControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_MESGBOX_END:
        if (pEvent->wParam == s_hMsgControl)
        {
            EQS_SetIOCActive(EQS_GetNthIOC(PI_PASS_INPUT));
        }else if( pEvent->wParam == s_hEndControl)
            EQS_End();
        
        break;

    case SE_IO_NEDIT_MATCH_LENGTH://SE_IO_NEDIT_DONE:
        if(!sIdent)//pEvent->wParam == EQS_GetNthIOC(PI_PASS_INPUT))
        {
            EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sPasswd);
            if(sPasswd[0] == '\0')
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_INPUT_NUM), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQS_SetIOCActive(EQS_GetNthIOC(PI_PASS_INPUT));
            }
            else
            {
                if(!s_Again)
                {
                    s_hMsgControl =
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_ONE_MORE_PUT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    EQS_SetIOCActive(EQS_GetNthIOC(PI_PASS_INPUT));
                    s_Again = TRUE;
                }else
                {
                    EQC_GetNumberEditValue(EQS_GetNthIOC(PI_PASS_INPUT), sAgnPasswd);
                    if(sAgnPasswd[0] == sPasswd[0] && sAgnPasswd[1] == sPasswd[1] &&
                        sAgnPasswd[2] == sPasswd[2] && sAgnPasswd[3] == sPasswd[3])//!STRCMP(sAgnPasswd, sPasswd))
                    {
                        sIdent = TRUE;
                    }
                    else
                    {
                        s_hMsgControl =
                            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NOT_IDENTITY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                        sIdent = FALSE;
                    }
                }
            }
        }else
        {
        
        }
        break;

    case SE_APP_IO_MODE:
        if(pEvent->wParam == EQS_GetNthIOC(PI_PASS_INPUT))
            EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASS_INPUT), (BYTE *)"");
        break;
     
    case SE_SOFTKEY_CANCEL:
        EQS_End();
        break;

    case SE_IO_TEDIT_DONE:
        break;

    case SE_IO_TEDIT_CANCEL:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_SetNumberEditValue(EQS_GetNthIOC(PI_PASS_INPUT), SKY_GET_PASSWORD());
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_KEY_CLEAR:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_KEY_END:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        EQS_End();
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_PasswordChange(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[5];
    HCONTROL sNumHandle;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_CANCEL, 
            (BYTE*)SKY_GET_TOKEN(TKN_PASSWORD_CHANGE), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret120x93x256c
               &g_BmpLock
			  ); 

        //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
        sNumHandle = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        //EQC_SetNumberEditBackColor(sNumHandle, NUMBER_EDIT_BACK_COLOR);

        EQC_SET_STYLE(sNumHandle, SECRET_NUMBER_EDITOR_DEFAULT);
        EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
        EQC_NotifyNumberEditMatchLen(sNumHandle, 4);
		EQS_SetIOCActive(sNumHandle);
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
            EQS_START_SIBLING(sSKA_PasswordChange, FALSE);
        else
            EQC_MessageBox((BYTE *)(BYTE*)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



#define INPUT_NEW_CODE_MSG_POS_X  48
#define INPUT_NEW_CODE_MSG_POS_Y  IMAGE_Y - EQS_CharHeight('A')

void SKY_API TextOutInputNewCodeMessage(void)
{
    EQS_TextOutStr(INPUT_NEW_CODE_MSG_POS_X, INPUT_NEW_CODE_MSG_POS_Y, 
                   (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_NEW)); 
    EQS_TextOutStr(INPUT_NEW_CODE_MSG_POS_X, (INPUT_NEW_CODE_MSG_POS_Y + EQS_CharHeight('A')), 
                   (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_1));
    EQS_TextOutStr(INPUT_NEW_CODE_MSG_POS_X, (INPUT_NEW_CODE_MSG_POS_Y + EQS_CharHeight('A')*2), 
                   (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_2));
    EQS_TextOutStr(INPUT_NEW_CODE_MSG_POS_X, (INPUT_NEW_CODE_MSG_POS_Y + EQS_CharHeight('A')*3), 
                   (BYTE*)(BYTE*)SKY_GET_TOKEN(TKN_INPUT_CODE_MESSAGE_3));
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
//#define IN_PASS_X      55//44//43
//#define IN_PASS_Y      68//65//69//40 + 5

void SKY_API sSKA_PasswordChange(SKY_EVENT_T *pEvent)
{
    STATIC BYTE     sPasswd[5], sAgnPasswd[5];
    STATIC HCONTROL s_hControl, s_hMsgControl, s_EndControl;
    STATIC BOOL s_Again = FALSE, sIdent = FALSE;
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_CANCEL, 
            (BYTE*)SKY_GET_TOKEN(TKN_PASSWORD_CHANGE), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecretnew120x93x256c
               (BITMAP*)&g_BmpLock
			  );

        //EQC_StaticText(EQS_NA, EQS_NA, TKN_NEW_PASSWORD);
        s_hControl  = EQC_NumberEdit(PASS_X, PASS_Y, 4, 1, 4,(BYTE *)"");
        
        EQC_SET_STYLE(s_hControl,(SECRET_NUMBER_EDITOR_DEFAULT|CS_NUMBER_EDIT_DIGIT_ONLY));

        EQC_SetNumberEditBigFont(s_hControl, TRUE);  
        EQC_NotifyNumberEditMatchLen(s_hControl, 4);
        EQS_SetIOCActive(s_hControl);

        EQC_SetNumberEditValue(s_hControl, (BYTE *)"");
        break;
      
    case SE_IO_NEDIT_NODIGIT:
        EQS_End();
        break;
        
    case SE_IO_NEDIT_CANCEL:
        EQS_SetIOCActive(s_hControl);
        break;

    case SE_APP_END :   /* The very last event */ 
        sIdent = FALSE;
        s_Again = FALSE;
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            EQC_SetNumberEditValue(s_hControl, (BYTE*)"");
            EQS_SetIOCActive(s_hControl);
        }else if(pEvent->wParam == s_EndControl)
        {
            EQS_End();
        }
        break;

    case SE_IO_NEDIT_MATCH_LENGTH://SE_IO_NEDIT_DONE:
        if(!sIdent)
        {
/*
            if(sPasswd[0] == '\0')
            {
                //EQS_SetIOCActive(s_hControl);
                s_hMsgControl = 
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NO_INPUT_NUM), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
            else
*/
            {
                if(!s_Again)
                {
                    s_Again = TRUE;
                    EQC_GetNumberEditValue(s_hControl, sPasswd);
                    //EQS_SetIOCActive(s_hControl);
                    EQS_SetSWindowHeaderStr((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_PASSWORD));
                    s_hMsgControl =
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_ONE_MORE_PUT), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                }else
                {
                    EQC_GetNumberEditValue(s_hControl, sAgnPasswd);
                    if(sAgnPasswd[0] == sPasswd[0] && sAgnPasswd[1] == sPasswd[1] &&
                        sAgnPasswd[2] == sPasswd[2] && sAgnPasswd[3] == sPasswd[3])//!STRCMP(sAgnPasswd, sPasswd))
                    {
                        //sIdent = FLASE;
                        SKY_SET_PASSWORD(sPasswd);
                        s_EndControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    }
                    else
                    {
                        sIdent = FALSE;
                        s_hMsgControl =
                            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_NOT_IDENTITY), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                    }
                }
            }
        }else
        {
//            SKY_GET_PASSWORD(sPasswd);
//            s_EndControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        break;

    case SE_IO_NEDIT_DONE:
        break;

    case SE_APP_IO_MODE:
//        if(pEvent->wParam == s_hControl)
//            EQC_SetNumberEditValue(s_hControl, (BYTE *)"");
        break;
     
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
    
    case SE_APP_DRAW:
        TextOutInputNewCodeMessage();
        break; 
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


