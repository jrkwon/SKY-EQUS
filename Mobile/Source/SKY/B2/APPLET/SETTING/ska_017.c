/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_s017.c
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
** 01-07-13   rosa     Created.
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
//#define MESSAGE_BASE                         1
//#define CALL_ORIGINATION_MESSAGE_XPOS        84

//#define CALL_ORIGINATION_ANIMATION_XPOS      5
//#define CALL_ORIGINATION_ANIMATION_YPOS      7

//#define CALL_ORIGINATION_IMAGE_XPOS          3
//#define CALL_ORIGINATION_IMAGE_YPOS          (DIAL_DIGIT_POS_Y - 3)

/* 발신화면에서 타이틀 높이와 Annunciator의 높이가 틀릴 경우에  */
/* 발신화면과 통화 중 화면이 상하로 움직이는 것처럼 보인다 이를 */
/* 보정할 때 사용된다.                                          */
#define sDIALING_DISPLAY_OFFSET_Y       (EQS_SWINDOW_HEADER_HEIGHT - EQS_SWINDOW_ANNUNCIATOR_HEIGHT)

#define sORIGINATION_IMAGE_XPOS         (DIAL_DIGIT_POS_X - 9)//3
#define sORIGINATION_IMAGE_YPOS         (DIAL_DIGIT_POS_Y - 5)



/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag017SERVICE_E
{
    CF_017_TITLE,
    CF_017_PHONE_OPEN_IMAGE,
    CF_017_ANIMATION,
    CF_017_IMAGE,
    CF_017_NUMBEREDITOR
};

//음성사서함 편집
enum tag_VOICE_MAIL_E {            
    VM_CONNECT_TITLE,
    VM_CONNECT_EDIT,
    VM_SET_TITLE,
    VM_SET_EDIT,
    VM_CANCEL_TITLE,
    VM_CANCEL_EDIT,
    VM_MAX
};

enum tag_SKA_AUTOCON_REV_E {            //자동연결&착신전환 편집
    SA_CONNECT_TITLE,
    SA_CONNECT_EDIT,
    SA_CANCEL_TITLE,
    SA_CANCEL_EDIT,
    SA_MAX
};

//정보샘 & 교통정보 편집 && 자동연결 등록 && 핫메뉴편집
enum tag_SKA_INFO_TRAFFIC_E {
    SI_CONNECT_TITLE,
    SI_CONNECT_EDIT,
    SI_MAX
};

enum tag_SKA_PASSWORD_CALL_E {            //비밀번호 통화
    SPC_TITLE_TXT,
    SPC_PASSWORD_TXT,
    SPC_PASSWORD_EDIT,
    SPC_MAX
};

enum tag_SKA_PASSWORD_E {            //비밀번호 통화 편집
    SP_INTERCALL_TITLE,
    SP_INTERCALL_EDIT,
    SP_ALLCALL_TITLE,
    SP_ALLCALL_EDIT,
    SP_CANCEL_TITLE,
    SP_CANCEL_EDIT,
    SP_MAX
};

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
void SKY_API SKA_Main017Service(SKY_EVENT_T *pEvent)
{

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow( SKY_DEF_WIN_X,
                       SKY_DEF_WIN_Y, 
                       SKY_DEF_WIN_DX, 
                       SKY_DEF_WIN_DY, 
                       CL_NONE_SELECT_NONE, 
                       NULL, 
                       WA_SHOW_CML);

        EQC_MenuList(6, 4, 18, 6, &mn017Srvc, 0, SKY_GET_SEL_MENU_MODE());
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END:
        break;
        
    case SE_APP_RESUME:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

    case SE_IO_MENU_LIST_SELECT:
        EQS_START_CHILD(pEvent->dwParam);
        break;
        
    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
        break;
    
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
        
    }
}

/*
** ***************************************************************************
** 1. 음성사서함
** ***************************************************************************
*/
void SKY_API SKA_017VMConnect(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_VOICE_MAIL_CONNECT());

        STRCPY(aPhoneNumber, SKY_GET_S017_VOICE_MAIL_CONNECT());
        nNumDigit = STRLEN(SKY_GET_S017_VOICE_MAIL_CONNECT());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
      break;
        
    case SE_APP_END:
        break;

    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW:
		break;
        
    case SE_SOFTKEY_MENU: 
        break;

    case SE_SOFTKEY_SAVE:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017VMSet(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();
		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_VOICE_MAIL_SET());

        STRCPY(aPhoneNumber, SKY_GET_S017_VOICE_MAIL_SET());
        nNumDigit = STRLEN(SKY_GET_S017_VOICE_MAIL_SET());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);

        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017VMCancel(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_VOICE_MAIL_CANCEL());

        STRCPY(aPhoneNumber, SKY_GET_S017_VOICE_MAIL_CANCEL());
        nNumDigit = STRLEN(SKY_GET_S017_VOICE_MAIL_CANCEL());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                

        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017VMEdit(SKY_EVENT_T *pEvent)
{
    UINT8  nCtrl;
    BYTE   sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        150, 
                        CL_NONE_SELECT_SAVE, 
                        SKY_GET_TOKEN(TKN_017_VOICE_MAIL_EDIT), 
                        WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_VOICE_MAIL_CONNECT());

        EQC_StaticText(EQS_NA, EQS_NA, TKN_SET);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_VOICE_MAIL_SET());

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_VOICE_MAIL_CANCEL());

        // Formatting..
        EQS_StartFormatter();
        for(nCtrl=0; nCtrl<VM_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();

        break;
        
    case SE_APP_END:
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
        
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(VM_CONNECT_EDIT), sNumber);
        SKY_SET_S017_VOICE_MAIL_CONNECT(sNumber);

        EQC_GetNumberEditValue(EQS_GetNthIOC(VM_SET_EDIT), sNumber);
        SKY_SET_S017_VOICE_MAIL_SET(sNumber);

        EQC_GetNumberEditValue(EQS_GetNthIOC(VM_CANCEL_EDIT), sNumber);
        SKY_SET_S017_VOICE_MAIL_CANCEL(sNumber);

        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS );
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_CANCEL),
                        MESSAGE_DEFAULT_TIME, 
                        MBT_NONE, 
                        MBT_SUCCESS);
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM );
        }
        else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM );
        }
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else
            EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
** ***************************************************************************
** 2. 자동연결
** ***************************************************************************
*/
void SKY_API SKA_017ACRegister(SKY_EVENT_T *pEvent)
{
    BYTE sSvrcNum[SKY_MAX_DIAL_017_SERVICE+1];
    BYTE sInputNum[PB_MAX_DIGIT_LENGTH+1];
    UINT8 nNumDigit;
    HCONTROL hDialEditBox;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        SKY_GET_TOKEN(TKN_017_AUTO_CONECT), 
                        WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CON_PHONE_NUM);   

        EQC_Image(sORIGINATION_IMAGE_XPOS, 
                  (sORIGINATION_IMAGE_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpCallPannel);

        hDialEditBox = EQC_NumberEdit( DIAL_DIGIT_POS_X, 
                                       DIAL_DIGIT_POS_Y, 
                                       DIAL_DIGIT_LENGTH_PER_LINE, 
                                       DIAL_DIGIT_LINE, 
                                       MAX_DIAL_DIGIT_LENGTH,
                                       (BYTE*)"" );

        //EQC_SetNumberEditBackColor(hDialEditBox, NUMBER_PANNEL_COLOR);
        EQC_SetNumberEditColor(hDialEditBox, NUMBER_TEXT_COLOR);

       
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_RIGHT2LEFT | \
                                    CS_NUMBER_EDIT_TP_PAUSE | \
                                    CS_NUMBER_EDIT_ALONE |\
                                    CS_NUMBER_EDIT_ORING |\
                                    CS_NUMBER_EDIT_DRAW_ALL_CONTROL);

        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(hDialEditBox);
        break;
        
    case SE_APP_END:
        break;
    
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive((HCONTROL)pEvent->wParam);
        STRCPY(sSvrcNum, SKY_GET_S017_AUTO_CON_REGISTER());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;        
     
    case SE_APP_RESUME:
        EQS_End();
        break;        
       
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017ACCancel(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER );
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_AUTO_CON_CANCEL());

        STRCPY(aPhoneNumber, SKY_GET_S017_AUTO_CON_CANCEL());
        nNumDigit = STRLEN(SKY_GET_S017_AUTO_CON_CANCEL());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;

    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017ACEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            SKY_GET_TOKEN(TKN_AUTOCONNECT_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_AUTO_CON_REGISTER());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_AUTO_CON_CANCEL());

        // Formatting..
        EQS_StartFormatter();
        for(nCtrl=0; nCtrl<SA_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();

        break;

    case SE_APP_END:
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
       
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CONNECT_EDIT), sNumber);            
        SKY_SET_S017_AUTO_CON_REGISTER(sNumber);
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CANCEL_EDIT), sNumber);
        SKY_SET_S017_AUTO_CON_CANCEL(sNumber);

        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_CANCEL), 
                        MESSAGE_DEFAULT_TIME, 
                        MBT_NONE, 
                        MBT_SUCCESS);
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM );
        }
        else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM);
        }
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        break;

    case SE_IO_MESGBOX_NO:
        if(s_fEndToRoot)
        {
            s_fEndToRoot = FALSE;
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
** ***************************************************************************
** 3. 무조건 전환
** ***************************************************************************
*/
void SKY_API SKA_017RCRegister(SKY_EVENT_T *pEvent)
{
    BYTE sSvrcNum[SKY_MAX_DIAL_017_SERVICE+1];
    BYTE sInputNum[PB_MAX_DIGIT_LENGTH+1];
    UINT8 nNumDigit;
    HCONTROL hDialEditBox;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        SKY_GET_TOKEN(TKN_017_REDIRECT_CALL), 
                        WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REV_PHONE_NUM);

        EQC_Image(sORIGINATION_IMAGE_XPOS, 
                  (sORIGINATION_IMAGE_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpCallPannel);

        hDialEditBox = EQC_NumberEdit(  DIAL_DIGIT_POS_X, 
                                        DIAL_DIGIT_POS_Y, 
                                        DIAL_DIGIT_LENGTH_PER_LINE, 
                                        DIAL_DIGIT_LINE, 
                                        MAX_DIAL_DIGIT_LENGTH,
                                        (BYTE*)"" );
        
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_RIGHT2LEFT | \
                                    CS_NUMBER_EDIT_TP_PAUSE | \
                                    CS_NUMBER_EDIT_ALONE |\
                                    CS_NUMBER_EDIT_ORING |\
                                    CS_NUMBER_EDIT_DRAW_ALL_CONTROL);

        //EQC_SetNumberEditBackColor(hDialEditBox, NUMBER_PANNEL_COLOR);
        EQC_SetNumberEditColor(hDialEditBox, NUMBER_TEXT_COLOR);

        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(hDialEditBox);
        break;
        
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive((HCONTROL)pEvent->wParam);
        STRCPY(sSvrcNum ,SKY_GET_S017_REDIR_CALL_REGISTER());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);   
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017RCCancel(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_REDIR_CALL_CANCEL());

        STRCPY(aPhoneNumber, SKY_GET_S017_REDIR_CALL_CANCEL());
        nNumDigit = STRLEN(SKY_GET_S017_REDIR_CALL_CANCEL());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);         
        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;
        
    case SE_APP_RESUME:
        EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017RCEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SELECT_SAVE, 
                        SKY_GET_TOKEN(TKN_017_REDIRECT_CALL_EDIT), 
                        WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_REDIR_CALL_REGISTER());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_REDIR_CALL_CANCEL());

        // Formatting..
        EQS_StartFormatter();
        for(nCtrl=0; nCtrl<SA_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();

        break;
        
    case SE_APP_END:
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

    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(SA_CONNECT_EDIT), sNumber);            
        SKY_SET_S017_REDIR_CALL_REGISTER(sNumber);
        EQC_GetNumberEditValue(EQS_GetNthIOC(SA_CANCEL_EDIT), sNumber);            
        SKY_SET_S017_REDIR_CALL_CANCEL(sNumber);
        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE), 
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS );
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_CANCEL), 
                        MESSAGE_DEFAULT_TIME, 
                        MBT_NONE, 
                        MBT_SUCCESS );
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO,
                            MBT_CONFIRM );
        }
        else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM );
        }
        else
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
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


/*
** ***************************************************************************
** 4. 통화중대기
** ***************************************************************************
*/
void SKY_API SKA_017CWRegister(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_CALL_WAITING_REGISTER());

        STRCPY(aPhoneNumber, SKY_GET_S017_CALL_WAITING_REGISTER());
        nNumDigit = STRLEN(SKY_GET_S017_CALL_WAITING_REGISTER());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
      break;
        
    case SE_APP_END:
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017CWCancel(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_CALL_WAITING_CANCEL());

        STRCPY(aPhoneNumber, SKY_GET_S017_CALL_WAITING_CANCEL());
        nNumDigit = STRLEN(SKY_GET_S017_CALL_WAITING_CANCEL());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                

        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017CWEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_SELECT_SAVE, 
                        SKY_GET_TOKEN(TKN_017_CALL_WAITING_EDIT), 
                        WA_SHOW_HEADER | WA_SHOW_CML );

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_CALL_WAITING_REGISTER());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_CALL_WAITING_CANCEL());
        
        // Formatting..
        EQS_StartFormatter();
        for(nCtrl = 0; nCtrl < SA_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();
        break;
        
    case SE_APP_END:
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

    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(SA_CONNECT_EDIT), sNumber);            
        SKY_SET_S017_CALL_WAITING_REGISTER(sNumber);
        EQC_GetNumberEditValue(EQS_GetNthIOC(SA_CANCEL_EDIT), sNumber);
        SKY_SET_S017_CALL_WAITING_CANCEL(sNumber);
        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE), 
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_CANCEL), 
                        MESSAGE_DEFAULT_TIME, 
                        MBT_NONE, 
                        MBT_SUCCESS );
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM);
        }
        else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM);
        }
        else
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
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
** ***************************************************************************
** 5. 파워리모콘
** ***************************************************************************
*/
void SKY_API SKA_017PRConnect(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER );

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_POWER_REMOTE());

        STRCPY(aPhoneNumber, SKY_GET_S017_POWER_REMOTE());
        nNumDigit = STRLEN(SKY_GET_S017_POWER_REMOTE());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API SKA_017PREdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        SKY_GET_TOKEN(TKN_017_POWER_REMOTE_EDIT), 
                        WA_SHOW_HEADER| WA_SHOW_CML );

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_POWER_REMOTE());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));
        
        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER | \
                                                      CS_NUMBER_EDIT_ALONE | \
                                                      CS_NUMBER_EDIT_CURSOR);

        // Formatting..
        EQS_StartFormatter();
        for(nCtrl = 0; nCtrl < SI_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        EQS_FitCanvasSize();
        break;
        
    case SE_APP_END:
        break;

    case SE_IO_MESGBOX_END:
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;
        
    case SE_IO_NEDIT_DONE:
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SI_CONNECT_EDIT), sNumber);            
        SKY_SET_S017_POWER_REMOTE(sNumber);
        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS );
        break;
    
    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM);
        }
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
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

/*
** ***************************************************************************
** 6. 파워멜로디
** ***************************************************************************
*/
void SKY_API SKA_017PMConnect(SKY_EVENT_T *pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
		EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER );

		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(CF_017_NUMBEREDITOR), SKY_GET_S017_POWER_MELODY());

        STRCPY(aPhoneNumber, SKY_GET_S017_POWER_MELODY());
        nNumDigit = STRLEN(SKY_GET_S017_POWER_MELODY());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END:
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
** ***************************************************************************
** 호 전환
** ***************************************************************************
*/
void SKY_API SKA_017HotMenu3Edit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_017_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_SetWindow(  SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE_CONFIRM_NONE, 
                        SKY_GET_TOKEN(TKN_HOT_MENU3_EDIT), 
                        WA_SHOW_HEADER | WA_SHOW_CML );

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34, SKY_GET_S017_FORWARD_HOT());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));

        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER | \
                                                      CS_NUMBER_EDIT_ALONE | \
                                                      CS_NUMBER_EDIT_CURSOR);

        // Formatting..
        EQS_StartFormatter();
        for(nCtrl = 0; nCtrl < SI_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }
        EQS_EndFormatter();
        break;
        
    case SE_APP_END:
        break;

    case SE_IO_NEDIT_DONE:
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue(EQS_GetNthIOC(SI_CONNECT_EDIT), sNumber);            
        SKY_SET_S017_FORWARD_HOT(sNumber);
        s_hMsgControl = EQC_MessageBox( SKY_GET_TOKEN(TKN_DONE_SAVE),
                                        MESSAGE_DEFAULT_TIME, 
                                        MBT_NONE, 
                                        MBT_SUCCESS );
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox( SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
                            MESSAGE_DEFAULT_TIME, 
                            MBT_YESNO, 
                            MBT_CONFIRM );
        }
        else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
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

    case SE_IO_MESGBOX_END:
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

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitOriginatedDisplayInVoiceCall()
**
** Description: 
**     Initialize Originate State Display in Voice Call
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
//static void sInitOriginatedDisplayIn017Service()
//{
//    HCONTROL hControl;
//
//    EQC_StaticText( CALL_ORIGINATION_MESSAGE_XPOS, 
//		            MESSAGE_BASE, 
//				    (EQS_TOKEN)TKN_CALL_ATTEMPT );
//
//	hControl = EQC_Animation( CALL_ORIGINATION_ANIMATION_XPOS, 
//		                      CALL_ORIGINATION_ANIMATION_YPOS, 
//                              &g_AniCall64x39x256c,
//                              NULL);
//	EQC_StartAnimation(hControl, TRUE);
//
//    EQC_Image( CALL_ORIGINATION_IMAGE_XPOS, 
//		       CALL_ORIGINATION_IMAGE_YPOS, 
//			   (BITMAP*)&g_BmpTable01_115x42x256c );
//
//    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
//                              DIAL_DIGIT_POS_Y, 
//                              DIAL_DIGIT_LENGTH_PER_LINE, 
//                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//    
//    EQC_SetNumberEditBackColor(hControl, NUMBER_EDIT_BACK_COLOR);
//    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | \
//        CS_NUMBER_EDIT_NO_CML );
//    EQC_SET_FOCUS(hControl, FALSE);
//    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
//
//    /* 백라이트 항상 켜짐 */
//    SKY_SET_NO_OFF_BACKLIGHT(TRUE);
//    SKY_BacklightOn(AB_ALWAYS);
//}