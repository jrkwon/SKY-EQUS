/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     CtrlTest.c
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
** 2000-10-27 woodstok     Created.
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

EQS_TOKEN paPmnuItem[] ={
    TKN_POPUP_MENU_SET, 
    TKN_POPUP_MENU_RESET
};

enum tag_CTEST_CTRL_E {
    CCE_TEXT_EDIT_TITLE,
    CCE_TEXT_EDIT,
    CCE_NUMBER_EDIT_1_TITLE,
    CCE_NUMBER_EDIT_1,
    CCE_NUMBER_EDIT_2_TITLE,
    CCE_NUMBER_EDIT_2,
    CCE_PROGRESS_BAR,
    CCE_VOLUME_1,
    CCE_VOLUME_2,
    CCE_VOLUME_3,
    CCE_CHECK_BOX,
    CCE_RADIO_BUTTON,
    CCE_MAX
};


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

VOID SKA_CtrlTestMenuTop(SKY_EVENT_T *pInEvent)
{
    UINT8 nCtrl;
    BYTE   pMsgStr[50];
    HCONTROL hControl;

    STATIC  HCONTROL    hiddenTE;

    CONST BYTE* sCtrlTestMenu1[5];
    CONST BYTE* sCtrlTestMenu2[3];
    
    sCtrlTestMenu1[0] = SKY_GET_TOKEN(TKN_PBOOK_NUMBER_MENU1);
    sCtrlTestMenu1[1] = SKY_GET_TOKEN(TKN_PBOOK_NUMBER_MENU2);
    sCtrlTestMenu1[2] = SKY_GET_TOKEN(TKN_PBOOK_NUMBER_MENU3);
    sCtrlTestMenu1[3] = SKY_GET_TOKEN(TKN_PBOOK_NUMBER_MENU4);
    sCtrlTestMenu1[4] = SKY_GET_TOKEN(TKN_PBOOK_NUMBER_MENU5);


    sCtrlTestMenu2[0] =   SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU1);
    sCtrlTestMenu2[1] =   SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU2);
    sCtrlTestMenu2[2] =   SKY_GET_TOKEN(TKN_PBOOK_ANNIV_MENU3);
    

	switch (pInEvent->EventID)
	{
	case SE_APP_START:
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            600, //SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, 
            WA_SHOW_HEADER);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_TEST_APPLET_TEXT_EDIT);
        EQC_TextEdit(EQS_NA, EQS_NA, 16, 1, TEM_KOREAN, 50, (BYTE*)"", 0);

        hControl = EQC_StaticText(EQS_NA, EQS_NA,TKN_TEST_APPLET_TEXT_EDIT);
        EQC_SetStaticText(hControl, (BYTE*) "NumEditor(Left)");
        EQC_NumberEdit(EQS_NA, EQS_NA, 16, 1, 34,(BYTE*)"0237094819");
        hControl = EQC_StaticText(EQS_NA, EQS_NA,TKN_TEST_APPLET_TEXT_EDIT);
        EQC_SetStaticText(hControl, (BYTE*) "NumEditor(Right)");
        EQC_NumberEdit(EQS_NA, EQS_NA, 16, 1, 34,(BYTE*)"0112935227");
      
        EQC_ProgressBar(EQS_NA, EQS_NA,100,13, 0,10,3,FALSE);

        EQC_Spin(EQS_NA, EQS_NA, 1900, 2999, 1, 2000);
        EQC_Spin(EQS_NA, EQS_NA,1,12,1,11);
        EQC_Spin(EQS_NA, EQS_NA,1,30,1,21);
   
        EQC_CheckBox(EQS_NA, EQS_NA, (BYTE **)sCtrlTestMenu1, 5);

        EQC_RadioButton(EQS_NA, EQS_NA, 6, 3, (BYTE **)sCtrlTestMenu2,  2);

        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < CCE_MAX; nCtrl++)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

            if( nCtrl != CCE_VOLUME_1 && nCtrl != CCE_VOLUME_2)
                EQS_NewLine();
        }

        EQS_EndFormatter();

        EQC_SET_STYLE(EQS_GetNthIOC(CCE_NUMBER_EDIT_2), 
            CS_NUMBER_EDIT_DEFAULTS | CS_NUMBER_EDIT_RIGHT2LEFT);

        EQC_SET_STYLE(EQS_GetNthIOC(CCE_TEXT_EDIT), 
            CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY);

        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_KEY_F1:
        // girin: EQC_PopUpMenu(paPmnuItem, 2, 0);
		break;	

    case SE_KEY_F2:
        //EQC_SetTextEditData(EQS_GetNthIOC(CCE_TEXT_EDIT), (BYTE*)"\C");
        EQC_SetTextEditData(EQS_GetNthIOC(CCE_TEXT_EDIT), (BYTE*)"C");
        hControl = EQC_MessageBox((BYTE*)"메시지박스 예제", 10,  MBT_YESNO, MBT_HELP);
        EQC_SET_STYLE(hControl, 
                    CS_MESSAGE_BOX_TITLE|
                    CS_MESSAGE_BOX_DEFAULT|
                    CS_MESSAGE_BOX_IGNORE_KEYS);
        break;

    case SE_KEY_1:
        hiddenTE = EQC_TextEdit(0, 1, 16, 1, TEM_KOREAN, 50, (BYTE*)"", 0);
        EQC_SET_STYLE(hiddenTE, 
            (EQC_GET_STYLE(EQS_GetNthIOC(CCE_TEXT_EDIT)) | CS_TEXT_EDIT_HIDDEN));

        EQS_SetIOCActive(hiddenTE);
        break;

    case SE_KEY_2:
//        EQC_NotifyNumberEditMatch( EQS_GetNthIOC(CCE_NUMBER_EDIT_1), (BYTE*)"1234" );
        break;


/** 
    Event process of End IO-Control 
**/

///////////////////////////////////
// Popup Menu가 종료시
///////////////////////////////////
    case SE_IO_POPUP_SELECT:       // POPUP Menu
        sprintf((char*)pMsgStr, "%d Selected", (int)pInEvent->dwParam);
        EQC_MessageBox(pMsgStr, 3, MBT_NONE, MBT_HELP);
        break;

    case SE_IO_POPUP_SELECT_CANCEL:       // POPUP Menu
        EQC_MessageBox((BYTE*)"Cancel Applet", 3, MBT_NONE, MBT_HELP);
        break;

///////////////////////////////////
// Number Edit가 종료시
///////////////////////////////////
    case SE_IO_NEDIT_DONE:
        if(EQC_GetNumberEditValue((HCONTROL)pInEvent->wParam, pMsgStr))
            EQC_MessageBox(pMsgStr, 3, MBT_NONE, MBT_HELP);
        break;
    
    case SE_IO_NEDIT_MATCH:
        EQC_MessageBox((BYTE*)"Number Matched!!", 3, MBT_NONE, MBT_HELP);
        break;


    case SE_IO_NEDIT_CANCEL:
        EQC_MessageBox((BYTE*)"취소되었습니다", 3, MBT_NONE, MBT_HELP);
        break;

///////////////////////////////////
// Text Edit가 종료시
///////////////////////////////////
    case SE_IO_TEDIT_DONE:
        EQC_GetTextEditData((HCONTROL)pInEvent->wParam, pMsgStr);
        if(STRLEN(pMsgStr))
            EQC_MessageBox(pMsgStr, 3, MBT_NONE, MBT_HELP);
        break;


        
    case SE_IO_TEDIT_CANCEL:
        EQC_MessageBox((BYTE*)"취소되었습니다", 3, MBT_NONE, MBT_HELP);
        break;

	default:
		SKY_DefEventProc(pInEvent);
		break;
	}
}