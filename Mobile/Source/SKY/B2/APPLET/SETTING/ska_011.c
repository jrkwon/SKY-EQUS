/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_011.c
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

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum tag_SKA_SORISAM_E {            //소리샘 편집
    SS_CONNECT_TITLE,
    SS_CONNECT_EDIT,
    SS_REGIST_TITLE,
    SS_REGIST_EDIT,
    SS_DIRECT_REGIST_TITLE,
    SS_DIRECT_REGIST_EDIT,
    SS_CANCEL_TITLE,
    SS_CANCEL_EDIT,
    SS_MAX
};

enum tag_SKA_AUTOCON_REV_E {            //자동연결&착신전환 편집
    SA_CONNECT_TITLE,
    SA_CONNECT_EDIT,
    SA_CANCEL_TITLE,
    SA_CANCEL_EDIT,
    SA_MAX
};

enum tag_SKA_INFO_TRAFFIC__E {//정보샘&교통정보 편집 && 자동연결 등록 &&  핫메뉴편집
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
LOCAL void SKY_API sSKA_Menu011Service(SKY_EVENT_T *pEvent);
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

void SKY_API SKA_MainMenuService(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
        {
            EQS_START_CHILD(sSKA_Menu011Service);
        }
        else
        {            
            EQS_START_CHILD(SKA_Main017Service);
        }
        break;
        
    case SE_APP_END :   /* The very last event */ 
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_END();
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
LOCAL void SKY_API sSKA_Menu011Service(SKY_EVENT_T *pEvent)
{

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);

        EQC_MenuList(6, 4, 18, 6, &mn011Srvc, 0,SKY_GET_SEL_MENU_MODE());
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
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
#define MESSAGE_BASE                         1
#define CALL_ORIGINATION_MESSAGE_XPOS        84





enum tag011SERVICE_E
{
  SKT011_TITLE,
  SKT011_PHONE_OPEN_IMAGE,
  SKT011_ANIMATION,
  SKT011_IMAGE,
  SKT011_NUMBEREDITOR
  //SKT011_IMAGE,
  //SKT011_TITLE,
  //SKT011_NUMBEREDITOR
};

#if 0//000000000000000000000000000000000000000000000000000000000000000000
//void sInitOriginatedDisplayIn011Service()
//{
//    HCONTROL hControl;
//
//    EQC_StaticText(CALL_ORIGINATION_MESSAGE_XPOS, 
//		           MESSAGE_BASE, 
//				   (EQS_TOKEN)TKN_CALL_ATTEMPT);
//
//	EQC_Image(CALL_ORIGINATION_PHONE_OPEN_XPOS, 
//		      CALL_ORIGINATION_PHONE_OPEN_YPOS, 
//			  (BITMAP*)&g_BmpPhoneOpen);
//
//	hControl = EQC_Animation(CALL_ORIGINATION_ANIMATION_XPOS, 
//		                     CALL_ORIGINATION_ANIMATION_YPOS, 
//							 &g_AniOriginatingCall,//&g_AniCall64x39x256c, 
//							 NULL);
//	EQC_StartAnimation(hControl, TRUE);
//
//	EQC_Image(CALL_ORIGINATION_IMAGE_XPOS, 
//	          CALL_ORIGINATION_IMAGE_YPOS, 
//			  (BITMAP*)&g_BmpGeneralPannel);
//
//
//	hControl = EQC_Animation(CALL_ORIGINATION_ANIMATION_XPOS, 
//		                     CALL_ORIGINATION_ANIMATION_YPOS, 
//							 //&g_AniCall80x39x256C, 
//                           &g_AniCall64x39x256c,
//							 NULL);
//	EQC_StartAnimation(hControl, TRUE);

//  EQC_Image( 
//		       CALL_ORIGINATION_IMAGE_XPOS, 
//		       CALL_ORIGINATION_IMAGE_YPOS, 
//			   (BITMAP*)&g_BmpTable01_115x42x256c
//	    	  );

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
#endif //00000000000000000000000000000000000000000000000000000000000000000000000



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
void SKY_API SKA_SorisamConnect(SKY_EVENT_T *pEvent)
{
    
    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_SORISAM_CON());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_SORISAM_CON());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_SORISAM_CON());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
      break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
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
void SKY_API SKA_SorisamRegi(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_SORISAM_SET());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_SORISAM_SET());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_SORISAM_SET());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                


        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
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
void SKY_API SKA_SorisamDirectRegi(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

        EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_SORISAM_IMME());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_SORISAM_IMME());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_SORISAM_IMME());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);

        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
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
void SKY_API SKA_SorisamCancel(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_SORISAM_RESET());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_SORISAM_RESET());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_SORISAM_RESET());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                

        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
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
void SKY_API SKA_SorisamEditing(SKY_EVENT_T *pEvent)
{
    UINT8  nCtrl;
    BYTE   sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            150, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_SORISAM_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_SORISAM_CON());

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_SORISAM_SET());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_DIRECT_REGI);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_SORISAM_IMME());

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_SORISAM_RESET());
        //EQS_SetIOCActive(EQS_GetNthIOC(1));

        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < SS_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
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
        
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SS_CONNECT_EDIT), sNumber);
        SKY_SET_011SERVICE_SORISAM_CON(sNumber);

        EQC_GetNumberEditValue( EQS_GetNthIOC(SS_REGIST_EDIT), sNumber);
        SKY_SET_011SERVICE_SORISAM_SET(sNumber);

        EQC_GetNumberEditValue( EQS_GetNthIOC(SS_DIRECT_REGIST_EDIT), sNumber);
        SKY_SET_011SERVICE_SORISAM_IMME(sNumber);

        EQC_GetNumberEditValue( EQS_GetNthIOC(SS_CANCEL_EDIT), sNumber);
        SKY_SET_011SERVICE_SORISAM_RESET(sNumber);

        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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
void SKY_API SKA_AutoConReg(SKY_EVENT_T *pEvent)
{
    BYTE sSvrcNum[SKY_MAX_DIAL_011_SERVICE+1];
    BYTE sInputNum[PB_MAX_DIGIT_LENGTH+1];
    UINT8 nNumDigit;
    HCONTROL hDialEditBox;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 120, CML_NONE, WA_SHOW_HEADER);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_011_AUTOCONEC), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CON_PHONE_NUM);
         
        
        EQC_Image(sORIGINATION_IMAGE_XPOS, 
                  (sORIGINATION_IMAGE_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpCallPannel);
        
        hDialEditBox = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                      DIAL_DIGIT_POS_Y, 
                                      DIAL_DIGIT_LENGTH_PER_LINE, 
                                      DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
        
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
        
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive((HCONTROL)pEvent->wParam);
        STRCPY(sSvrcNum, SKY_GET_011SERVICE_AUTOCON_SET());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);

//        ui_orig_voice_call ( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS,  
//            GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
//        EQS_START_CHILD(SKA_MainCall);              

        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
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
void SKY_API SKA_AutoConCancel(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_AUTOCON_RESET());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_AUTOCON_RESET());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_AUTOCON_RESET());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);    
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW :                  
//        DrawGeneralFrame();
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
void SKY_API SKA_AutoConEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_AUTOCONNECT_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_AUTOCON_SET());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_AUTOCON_RESET());
        //EQS_SetIOCActive(EQS_GetNthIOC(1));

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
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
       
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CONNECT_EDIT), sNumber);            
        SKY_SET_011SERVICE_AUTOCON_SET(sNumber);
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CANCEL_EDIT), sNumber);
        SKY_SET_011SERVICE_AUTOCON_RESET(sNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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
void SKY_API SKA_RevCallReg(SKY_EVENT_T *pEvent)
{
    BYTE sSvrcNum[SKY_MAX_DIAL_011_SERVICE+1];
    BYTE sInputNum[PB_MAX_DIGIT_LENGTH+1];
    UINT8 nNumDigit;
    HCONTROL hDialEditBox;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_011_REVERSECALL), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REV_PHONE_NUM);
        
        EQC_Image(sORIGINATION_IMAGE_XPOS, 
                  (sORIGINATION_IMAGE_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpCallPannel);

        hDialEditBox = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                      DIAL_DIGIT_POS_Y, 
                                      DIAL_DIGIT_LENGTH_PER_LINE, 
                                      DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
        
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_RIGHT2LEFT | \
                                    CS_NUMBER_EDIT_TP_PAUSE | \
                                    CS_NUMBER_EDIT_ALONE |\
                                    CS_NUMBER_EDIT_ORING |\
                                    CS_NUMBER_EDIT_DRAW_ALL_CONTROL);

        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        
        //EQC_SetNumberEditBackColor(hDialEditBox, NUMBER_PANNEL_COLOR);
        EQC_SetNumberEditColor(hDialEditBox, NUMBER_TEXT_COLOR);
        EQS_SetIOCActive(hDialEditBox);

        break;
        
    case SE_IO_NEDIT_DONE:
        EQS_SetIOCActive((HCONTROL)pEvent->wParam);
        STRCPY(sSvrcNum ,SKY_GET_011SERVICE_CALLREDIR_SET());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);   
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_IO_MESGBOX_END:
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
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
void SKY_API SKA_RevCallCancel(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_CALLREDIR_RESET());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_CALLREDIR_RESET());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_CALLREDIR_RESET());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);         
        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW :                  
//        DrawGeneralFrame();
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
void SKY_API SKA_RevCallEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_REVERSECALL_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_REGISTRY);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_CALLREDIR_SET());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_CALLREDIR_RESET());
        
        //EQS_SetIOCActive(EQS_GetNthIOC(1));

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

    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CONNECT_EDIT), sNumber);            
        SKY_SET_011SERVICE_CALLREDIR_SET(sNumber);
        EQC_GetNumberEditValue( EQS_GetNthIOC(SA_CANCEL_EDIT), sNumber);            
        SKY_SET_011SERVICE_CALLREDIR_RESET(sNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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
void SKY_API SKA_InfoSamCon(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_INFO_POND());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_INFO_POND());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_INFO_POND());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW :                  
//        DrawGeneralFrame();
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
void SKY_API SKA_InfoSamEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_INFOSAM_EDIT), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_INFO_POND());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));
        
        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER |\
                      CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);

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
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
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
        SKY_SET_011SERVICE_INFO_POND(sNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
    
    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        /*
        EQC_GetNumberEditValue( EQS_GetNthIOC(SI_CONNECT_EDIT), sNumber);            
        if(STRCMP(sNumber, SKY_GET_011SERVICE_INFO_POND()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        */
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
        EQS_SetIOCActive(EQS_GetNthIOC(1));
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
void SKY_API SKA_TrafficInfoCon(SKY_EVENT_T *pEvent)
{

    BYTE             aPhoneNumber[NE_MAX_SIZE];
    UINT8            nNumDigit;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

		EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);
		//sInitOriginatedDisplayIn011Service();
		InitOrigAttemptDisp();

		EQC_SetNumberEditValue(EQS_GetNthIOC(SKT011_NUMBEREDITOR),SKY_GET_011SERVICE_TRAFFIC_INFO());

        STRCPY(aPhoneNumber, SKY_GET_011SERVICE_TRAFFIC_INFO());
        nNumDigit = STRLEN(SKY_GET_011SERVICE_TRAFFIC_INFO());

        SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);                
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
        break;

    case SE_APP_DRAW :                  
//        DrawGeneralFrame();
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
void SKY_API SKA_TrafficEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC BOOL s_fEndToRoot = FALSE;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_TRAFFICINFO_EDIT), 
            WA_SHOW_HEADER| WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_TRAFFIC_INFO());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));

        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER|\
                      CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);

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
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
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
        SKY_SET_011SERVICE_TRAFFIC_INFO(sNumber);
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        /*
        if(STRCMP(sNumber,SKY_GET_011SERVICE_TRAFFIC_INFO()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        */
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
void SKY_API SKA_InterCallCTRL(SKY_EVENT_T *pEvent)
{

    BYTE sSvrcNum[SKY_MAX_DIAL_011_SERVICE+1];
    BYTE sInputNum[SKY_MAX_DIAL_011_SERVICE+1];
    UINT8 nCtrl, nNumDigit;
    HCONTROL hDialEditBox;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_INTERCALL_CTRL), 
            WA_SHOW_HEADER|WA_SHOW_CML);
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_INTER_CTRL);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHONE_PASSWORD);
        hDialEditBox = EQC_NumberEdit(EQS_NA, 
                                      EQS_NA, 
                                      4, 
                                      1, 4,(BYTE*)"" );
        
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_BORDER| \
                      CS_NUMBER_EDIT_TP_PAUSE| CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_ALONE);
        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font

        EQS_SetIOCActive(EQS_GetNthIOC(SPC_PASSWORD_EDIT));
        
        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < SPC_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;     
        
    case SE_IO_NEDIT_DONE:
        STRCPY(sSvrcNum ,SKY_GET_011SERVICE_BAN_FOREIGN());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);   
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
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
void SKY_API SKA_AllCallCTRL(SKY_EVENT_T *pEvent)
{

    BYTE sSvrcNum[SKY_MAX_DIAL_011_SERVICE+1];
    BYTE sInputNum[SKY_MAX_DIAL_011_SERVICE+1];
    UINT8 nCtrl, nNumDigit;
    HCONTROL hDialEditBox;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_ALLCALL_CTRL), 
            WA_SHOW_HEADER|WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_ALL_CTRL);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHONE_PASSWORD);
        hDialEditBox = EQC_NumberEdit(EQS_NA, 
                                      EQS_NA, 
                                      4, 
                                      1, 4,(BYTE*)"" );

        
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_BORDER| CS_NUMBER_EDIT_TP_PAUSE|\
                                    CS_NUMBER_EDIT_PLAIN_NUM|CS_NUMBER_EDIT_ALONE);
        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(EQS_GetNthIOC(SPC_PASSWORD_EDIT));

        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl <SPC_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        break;

    case SE_IO_NEDIT_DONE:
        STRCPY(sSvrcNum ,SKY_GET_011SERVICE_BAN_ALL());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit = STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);       
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
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
void SKY_API SKA_SecretCancel(SKY_EVENT_T *pEvent)
{

    BYTE sSvrcNum[SKY_MAX_DIAL_011_SERVICE+1];
    BYTE sInputNum[SKY_MAX_DIAL_011_SERVICE+1];
    UINT8 nCtrl, nNumDigit;
    HCONTROL hDialEditBox;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        //EQS_SetWindow(0, 0, 120, 100, CML_NONE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_CANCEL), 
            WA_SHOW_HEADER | WA_SHOW_CML);
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL_CTRL);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_PHONE_PASSWORD);
        hDialEditBox = EQC_NumberEdit(EQS_NA, 
                                      EQS_NA, 
                                      4, 
                                      1, 4,(BYTE*)"" );
        
        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_BORDER| CS_NUMBER_EDIT_TP_PAUSE|\
            CS_NUMBER_EDIT_PLAIN_NUM);
        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(EQS_GetNthIOC(SPC_PASSWORD_EDIT));
        
        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < SPC_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
            EQS_NewLine();
        }

        EQS_EndFormatter();
        break;
        
    case SE_IO_MESGBOX_END:
        EQS_End();
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_DONE:
        STRCPY(sSvrcNum ,SKY_GET_011SERVICE_BAN_RESET());
        EQC_GetNumberEditValue((HCONTROL)pEvent->wParam, sInputNum);
        STRCAT(sSvrcNum , sInputNum);
        nNumDigit =  STRLEN(sSvrcNum);

        SET_NUMEDIT_BUFFER(sSvrcNum, nNumDigit);
        SET_NUMEDIT_POS(nNumDigit);        
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;    
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        EQS_End();
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
void SKY_API SKA_SecretEdit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            150, 
            CL_NONE_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_SECRETCALL_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_INTERCALL_CTRL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_BAN_FOREIGN());
        
        EQC_StaticText(EQS_NA, EQS_NA, TKN_ALLCALL_CTRL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_BAN_ALL());

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CANCEL);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_BAN_RESET());
        //EQS_SetIOCActive(EQS_GetNthIOC(1));    

        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < SP_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
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
        if(s_fEndToRoot)
        {
            EQS_END_TO(EQS_POP_TO_ROOT);
            s_fEndToRoot = FALSE;
        }
        else
            EQS_End();
        break;
        
    case SE_SOFTKEY_SAVE:
        EQC_GetNumberEditValue( EQS_GetNthIOC(SP_INTERCALL_EDIT), sNumber);            
        SKY_SET_011SERVICE_BAN_FOREIGN(sNumber);
        EQC_GetNumberEditValue( EQS_GetNthIOC(SP_ALLCALL_EDIT), sNumber);            
        SKY_SET_011SERVICE_BAN_ALL(sNumber);
        EQC_GetNumberEditValue( EQS_GetNthIOC(SP_CANCEL_EDIT), sNumber);            
        SKY_SET_011SERVICE_BAN_RESET(sNumber);

        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_IO_NEDIT_CANCEL:
        if(EQS_IsControlDataChanged())
        {
        	EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();        
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
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

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
        if(EQS_IsControlDataChanged())
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
            EQS_End();
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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
void SKY_API SKA_HotMenu3Edit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_CONFIRM_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_HOT_MENU3_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_FORWARD_HOT());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));

        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER|\
            CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);

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
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
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
        SKY_SET_011SERVICE_FORWARD_HOT(sNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        /*
        EQC_GetNumberEditValue( EQS_GetNthIOC(SI_CONNECT_EDIT), sNumber);            
        if(STRCMP(sNumber, SKY_GET_011SERVICE_FORWARD_HOT()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        */
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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
void SKY_API SKA_HotMenu4Edit(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    BYTE sNumber[SKY_MAX_DIAL_011_SERVICE+1];
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        //EQS_SetWindow(0, 0, 120, 150, CML_MENU_SAVE, WA_SHOW_HEADER | WA_SHOW_CML);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_CONFIRM_NONE, 
            (BYTE*)SKY_GET_TOKEN(TKN_HOT_MENU4_EDIT), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_CONNECT);
        EQC_NumberEdit(EQS_NA, EQS_NA, 13, 1, 34,SKY_GET_011SERVICE_PENDING_HOT());
        EQS_SetIOCActive(EQS_GetNthIOC(SI_CONNECT_EDIT));

        EQC_SET_STYLE(EQS_GetNthIOC(SI_CONNECT_EDIT), CS_NUMBER_EDIT_BORDER|\
            CS_NUMBER_EDIT_ALONE|CS_NUMBER_EDIT_CURSOR);
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
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
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
        SKY_SET_011SERVICE_PENDING_HOT(sNumber);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;

    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_IO_NEDIT_CANCEL:
        EQS_End();
        //EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        /*
        EQC_GetNumberEditValue( EQS_GetNthIOC(SI_CONNECT_EDIT), sNumber); 
        if(STRCMP(sNumber, SKY_GET_011SERVICE_PENDING_HOT()))
        {
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        */
        break;

    case SE_KEY_END:
        if(EQS_IsControlDataChanged())
        {
            s_fEndToRoot = TRUE;
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        }else
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

