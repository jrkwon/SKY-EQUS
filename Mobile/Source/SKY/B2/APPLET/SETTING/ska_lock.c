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
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define CHECKBOX_YPOS           3
#define LOCK_CONTENT_NUMBER     11//10//9

#if SKY_MODEL_ID == SKY_MODEL_B0
#define LOCK_CONTENT_LINE       7
#elif SKY_MODEL_ID == SKY_MODEL_B1
#define LOCK_CONTENT_LINE       6
#elif SKY_MODEL_ID == SKY_MODEL_B2
#define LOCK_CONTENT_LINE       8
#endif
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
SKY_TOKEN paLockMenu[2] =       
{
  TKN_ALL_LOCK,          
  TKN_ALL_UNLOCK
};

enum tagLockMenu_E
{
  ALL_LOCK = 0,       
  ALL_UNLOCK
};

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API sSKA_Lock(SKY_EVENT_T *pEvent);
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

void SKY_API SKA_MainLock(SKY_EVENT_T *pEvent)
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
            (BYTE*)SKY_GET_TOKEN(TKN_LOCK), 
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
            EQS_START_SIBLING(sSKA_Lock, FALSE);
        else
            EQC_MessageBox((BYTE *)(BYTE*)SKY_GET_TOKEN(TKN_WRONG_PASSWORD), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP); 
        break;
        
    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

void SKY_API sSKA_Lock(SKY_EVENT_T *pEvent)//void SKY_API sSKA_Lock(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    STATIC BOOL s_fFirstScreen = FALSE;
    
    STATIC HCONTROL s_hControl1 = INVALID_CONTROL_HANDLE;
    STATIC HCONTROL s_hControl2 = INVALID_CONTROL_HANDLE;
    
    CONST BYTE* sLockContent1[LOCK_CONTENT_LINE];  
    CONST BYTE* sLockContent2[LOCK_CONTENT_NUMBER-LOCK_CONTENT_LINE];

    STATIC BOOL s_fAllLock = FALSE;           //체크박스가 활성화될 수 없기 때문에 어쩔 
    STATIC BOOL s_fInterCallLock = FALSE;           //수 없이 발생하는 static 별수들...
    STATIC BOOL s_fPBookLock = FALSE;
    STATIC BOOL s_fMenuLock = FALSE; 
    STATIC BOOL s_fCallStackLock = FALSE;
    STATIC BOOL s_fRecLock = FALSE;
    STATIC BOOL s_fMemoLock = FALSE;
	STATIC BOOL s_fCameraLock = FALSE;
    STATIC BOOL s_fNtopLock = FALSE;
    STATIC BOOL s_fSmsLock = FALSE;
    STATIC BOOL s_fSelfInfoLock = FALSE;

    sLockContent1[0] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_ALL);
    sLockContent1[1] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_INTERCALL);
    sLockContent1[2] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_PHONEBOOK);
    sLockContent1[3] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_MENU);
    sLockContent1[4] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_CALLSTACK);
    sLockContent1[5] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_REC);
#if SKY_MODEL_ID == SKY_MODEL_B0
    sLockContent1[6] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_MEMO);
#elif SKY_MODEL_ID == SKY_MODEL_B2   
	sLockContent1[6] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_CAMERA);
    sLockContent1[7] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_NTOP);
#endif

    //sLockContent2[0] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_NTOP);
    sLockContent2[0] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_MESSAGE);
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
    sLockContent2[1] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_MEMO);
    sLockContent2[2] = (BYTE*)SKY_GET_TOKEN(TKN_LOCK_SELF_INFO);   
#endif

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_SELECT_SAVE, 
            (BYTE*)SKY_GET_TOKEN(TKN_LOCK1), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        s_fEndToRoot = FALSE;
    
        s_hControl1 = INVALID_CONTROL_HANDLE;
        s_hControl2 = INVALID_CONTROL_HANDLE;

        s_fAllLock = FALSE;           //체크박스가 활성화될 수 없기 때문에 어쩔 
        s_fInterCallLock = FALSE;           //수 없이 발생하는 static 별수들...
        s_fPBookLock = FALSE;
        s_fMenuLock = FALSE; 
        s_fCallStackLock = FALSE;
        s_fRecLock = FALSE;
        s_fMemoLock = FALSE;
		s_fCameraLock = FALSE;
        s_fNtopLock = FALSE;
        s_fSmsLock = FALSE;
        s_fSelfInfoLock = FALSE;

        s_hControl1 = EQC_CheckBox( 0, CHECKBOX_YPOS, (BYTE **)sLockContent1, LOCK_CONTENT_LINE);
        EQC_SET_STYLE(s_hControl1, (EQC_GET_STYLE(s_hControl1) & ~CS_CHECK_BOX_BORDER )| 
                                    CS_CHECK_BOX_HMAXIMIZE                             |
                                    CS_CHECK_BOX_LIST);
        s_fFirstScreen = TRUE;
        EQS_SetIOCActive(s_hControl1);

        if(SKY_IS_AUTO_LOCK())
        {
            s_fAllLock = TRUE;
            EQC_SetCheckBox(s_hControl1, 0);
        }
        if(SKY_IS_OVERSEAS_CALL_LOCK())
        {
            s_fInterCallLock =  TRUE;
            EQC_SetCheckBox(s_hControl1, 1);
        }
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
            s_fPBookLock  = TRUE;
            EQC_SetCheckBox(s_hControl1, 2);
        }
        if(SKY_IS_MENU_LOCK())
        {
            s_fMenuLock = TRUE;
            EQC_SetCheckBox(s_hControl1, 3);
        }
        if(SKY_IS_CALL_STACK_LOCK())
        {
            s_fCallStackLock = TRUE;
            EQC_SetCheckBox(s_hControl1, 4);
        }
        if(SKY_IS_RECORD_PLAY_LOCK())
        {
            s_fRecLock = TRUE;
            EQC_SetCheckBox(s_hControl1, 5);
        }
        if(SKY_IS_MEMO_LOCK())
        {
            s_fMemoLock = TRUE;
#if MODEL_ID == MODEL_B0        
            EQC_SetCheckBox(s_hControl1, 6);
#endif
        }
		if(SKY_IS_CAMERA_LOCK())
        {
            s_fCameraLock = TRUE;
#if MODEL_ID == MODEL_B2        
            EQC_SetCheckBox(s_hControl1, 6);
#endif
        }
        if(SKY_IS_WAP_LOCK())
        {
            s_fNtopLock = TRUE;
            EQC_SetCheckBox(s_hControl1, 7);
        }

        if(SKY_IS_SMS_LOCK())
            s_fSmsLock = TRUE;
        if(SKY_GET_SELFINFOLOCK())
            s_fSelfInfoLock = TRUE;
        break;
        
    case SE_APP_END :   /* The very last event */ 
        s_fFirstScreen = FALSE;
        s_fEndToRoot = FALSE;
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case EQE_IO_MESGBOX_CLEAR:
        if(s_fFirstScreen)
            EQS_SetIOCActive(s_hControl1);
        else
            EQS_SetIOCActive(s_hControl2);
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

    case SE_IO_CHECK_BOX_CHECK:
    case SE_IO_CHECK_BOX_UNCHECK:
        if(pEvent->wParam == s_hControl1)
        {
            s_fAllLock = EQC_GetCheckBox(s_hControl1, 0);
            s_fInterCallLock = EQC_GetCheckBox(s_hControl1, 1);
            s_fPBookLock = EQC_GetCheckBox(s_hControl1, 2);
            s_fMenuLock = EQC_GetCheckBox(s_hControl1, 3);
            s_fCallStackLock = EQC_GetCheckBox(s_hControl1, 4);
            s_fRecLock = EQC_GetCheckBox(s_hControl1, 5);
#if SKY_MODEL_ID == SKY_MODEL_B0
            s_fMemoLock = EQC_GetCheckBox(s_hControl1, 6);
#elif  SKY_MODEL_ID == SKY_MODEL_B2  
			s_fCameraLock = EQC_GetCheckBox(s_hControl1, 6);
            s_fNtopLock = EQC_GetCheckBox(s_hControl1, 7);
#endif			
        }else if(pEvent->wParam == s_hControl2)
        {
            //s_fNtopLock = EQC_GetCheckBox(s_hControl2, 0);
            s_fSmsLock = EQC_GetCheckBox(s_hControl2, 0); 
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
            s_fMemoLock = EQC_GetCheckBox(s_hControl2, 1);
            s_fSelfInfoLock = EQC_GetCheckBox(s_hControl2, 2);
#endif            
        }
        break;

    case SE_IO_CHECK_BOX_UP:
        if(pEvent->wParam == s_hControl2)
        {
            s_fFirstScreen = TRUE;
            EQS_SetSWindowHeaderStr((BYTE*)SKY_GET_TOKEN(TKN_LOCK1));
            //s_fNtopLock = EQC_GetCheckBox(s_hControl2, 0);
            s_fSmsLock = EQC_GetCheckBox(s_hControl2, 0);
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
            s_fMemoLock = EQC_GetCheckBox(s_hControl2, 1);
            s_fSelfInfoLock = EQC_GetCheckBox(s_hControl2, 2);
#endif            

            EQS_DeleteControl(s_hControl2);
            
            s_hControl1 = EQC_CheckBox(0, CHECKBOX_YPOS, (BYTE **)sLockContent1, LOCK_CONTENT_LINE);
            EQC_SET_STYLE(s_hControl1, (EQC_GET_STYLE(s_hControl1) & ~CS_CHECK_BOX_BORDER ) | 
                                        CS_CHECK_BOX_HMAXIMIZE                              |
                                        CS_CHECK_BOX_LIST);
            EQS_SetIOCActive(s_hControl1);

            EQC_SetCheckBoxIndex(s_hControl1 , LOCK_CONTENT_LINE - 1);

            if(s_fAllLock)
                EQC_SetCheckBox(s_hControl1, 0);
            if(s_fInterCallLock)
                EQC_SetCheckBox(s_hControl1, 1);
            if(s_fPBookLock)
                EQC_SetCheckBox(s_hControl1, 2);
            if(s_fMenuLock)
                EQC_SetCheckBox(s_hControl1, 3);
            if(s_fCallStackLock)
                EQC_SetCheckBox(s_hControl1, 4);
            if(s_fRecLock)
                EQC_SetCheckBox(s_hControl1, 5);
#if SKY_MODEL_ID == SKY_MODEL_B0
            if(s_fMemoLock)
                EQC_SetCheckBox(s_hControl1, 6);
#elif  SKY_MODEL_ID == SKY_MODEL_B2    
			if(s_fCameraLock)
                EQC_SetCheckBox(s_hControl1, 6);
#endif
            if(s_fNtopLock)
                EQC_SetCheckBox(s_hControl1, 7);
        }else
            EQS_SetIOCActive(s_hControl1);
        break;

    case SE_IO_CHECK_BOX_DOWN:
        if(pEvent->wParam == s_hControl1)
        {
            s_fFirstScreen = FALSE;
            EQS_SetSWindowHeaderStr((BYTE*)SKY_GET_TOKEN(TKN_LOCK2));
            s_fAllLock = EQC_GetCheckBox(s_hControl1, 0);
            s_fInterCallLock = EQC_GetCheckBox(s_hControl1, 1);
            s_fPBookLock = EQC_GetCheckBox(s_hControl1, 2);
            s_fMenuLock = EQC_GetCheckBox(s_hControl1, 3);
            s_fCallStackLock = EQC_GetCheckBox(s_hControl1, 4);
            s_fRecLock = EQC_GetCheckBox(s_hControl1, 5);
#if SKY_MODEL_ID == SKY_MODEL_B0
            s_fMemoLock = EQC_GetCheckBox(s_hControl1, 6);
#elif  SKY_MODEL_ID == SKY_MODEL_B2    
			s_fCameraLock = EQC_GetCheckBox(s_hControl1, 6);
#endif
            s_fNtopLock = EQC_GetCheckBox(s_hControl1, 7);
            
            EQS_DeleteControl(s_hControl1);

            s_hControl2 = EQC_CheckBox(0, CHECKBOX_YPOS, (BYTE **)sLockContent2, LOCK_CONTENT_NUMBER - LOCK_CONTENT_LINE);
            EQC_SET_STYLE(s_hControl2,  (EQC_GET_STYLE(s_hControl1) & ~CS_CHECK_BOX_BORDER )| 
                                         CS_CHECK_BOX_HMAXIMIZE                             |
                                         CS_CHECK_BOX_LIST);
            EQS_SetIOCActive(s_hControl2);

            //if(s_fNtopLock)
            //    EQC_SetCheckBox(s_hControl2, 0);
            if(s_fSmsLock)
                EQC_SetCheckBox(s_hControl2, 0);   
#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
            if(s_fMemoLock)
                EQC_SetCheckBox(s_hControl2, 1);  
            if(s_fSelfInfoLock)
                EQC_SetCheckBox(s_hControl2, 2);
#endif           
        }else
            EQS_SetIOCActive(s_hControl2);

//            s_hControl2 = EQC_CheckBox(EQS_NA, EQS_NA, (BYTE **)sLockContent2, 4);
        break;
        
    case SE_SOFTKEY_SAVE:
        SKY_SET_AUTO_LOCK_MODE(s_fAllLock);  
        SKY_SET_SMS_LOCK_MODE(s_fSmsLock);

        SKY_SET_AUTO_LOCK(s_fAllLock);
        SKY_SET_OVERSEAS_CALL_LOCK (s_fInterCallLock);
        SKY_SET_PHONE_BOOK_LOCK(s_fPBookLock);
        SKY_SET_MENU_LOCK (s_fMenuLock);
        SKY_SET_CALL_STACK_LOCK(s_fCallStackLock);
        SKY_SET_RECORD_PLAY_LOCK(s_fRecLock);
        SKY_SET_MEMO_LOCK(s_fMemoLock);
		SKY_SET_CAMERA_LOCK(s_fCameraLock);
        SKY_SET_WAP_LOCK(s_fNtopLock);
        SKY_SET_SMS_LOCK(s_fSmsLock);
        SKY_SET_SELFINFOLOCK(s_fSelfInfoLock);
        s_hMsgControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
    
    case SE_IO_CHECK_BOX_CANCEL:
        if(pEvent->dwParam != EQC_DELETED)
        {
            if(pEvent->wParam == s_hControl1)
            {
                if(SKY_IS_AUTO_LOCK() != EQC_GetCheckBox(s_hControl1, 0))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_OVERSEAS_CALL_LOCK() != EQC_GetCheckBox(s_hControl1, 1))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_PHONE_BOOK_LOCK() != EQC_GetCheckBox(s_hControl1, 2))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_MENU_LOCK() != EQC_GetCheckBox(s_hControl1, 3))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_CALL_STACK_LOCK() != EQC_GetCheckBox(s_hControl1, 4))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_RECORD_PLAY_LOCK() != EQC_GetCheckBox(s_hControl1, 5))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_CAMERA_LOCK() != EQC_GetCheckBox(s_hControl1, 6))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_WAP_LOCK() != EQC_GetCheckBox(s_hControl1, 7))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_SMS_LOCK() != s_fSmsLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_MEMO_LOCK() != s_fMemoLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
				if(SKY_IS_CAMERA_LOCK() != s_fCameraLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_GET_SELFINFOLOCK() != s_fSelfInfoLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                EQS_End();
            }else if(pEvent->wParam == s_hControl2)
            {
                if(SKY_IS_AUTO_LOCK() != s_fAllLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_OVERSEAS_CALL_LOCK() != s_fInterCallLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_PHONE_BOOK_LOCK() != s_fPBookLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_MENU_LOCK() != s_fMenuLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_CALL_STACK_LOCK() != s_fCallStackLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_RECORD_PLAY_LOCK() != s_fRecLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_CAMERA_LOCK() != s_fCameraLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_WAP_LOCK() != s_fNtopLock)
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_SMS_LOCK() != EQC_GetCheckBox(s_hControl2, 0))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_IS_MEMO_LOCK() != EQC_GetCheckBox(s_hControl2, 1))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                if(SKY_GET_SELFINFOLOCK() != EQC_GetCheckBox(s_hControl2, 2))
                {
                    EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                    break;
                }
                EQS_End();
            }
            break;
        }
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        if(EQS_IsActive(s_hControl1) || s_fFirstScreen)
        {
            if(SKY_IS_AUTO_LOCK() != EQC_GetCheckBox(s_hControl1, 0))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_OVERSEAS_CALL_LOCK() != EQC_GetCheckBox(s_hControl1, 1))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_PHONE_BOOK_LOCK() != EQC_GetCheckBox(s_hControl1, 2))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_MENU_LOCK() != EQC_GetCheckBox(s_hControl1, 3))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_CALL_STACK_LOCK() != EQC_GetCheckBox(s_hControl1, 4))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_RECORD_PLAY_LOCK() != EQC_GetCheckBox(s_hControl1, 5))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_CAMERA_LOCK() != EQC_GetCheckBox(s_hControl1, 6))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_WAP_LOCK() != EQC_GetCheckBox(s_hControl1, 7))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_SMS_LOCK() != s_fSmsLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
			if(SKY_IS_MEMO_LOCK() != s_fMemoLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_GET_SELFINFOLOCK() != s_fSelfInfoLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        else if(EQS_IsActive(s_hControl2) || !s_fFirstScreen)
        {
            if(SKY_IS_AUTO_LOCK() != s_fAllLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_OVERSEAS_CALL_LOCK() != s_fInterCallLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_PHONE_BOOK_LOCK() != s_fPBookLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_MENU_LOCK() != s_fMenuLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_CALL_STACK_LOCK() != s_fCallStackLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_RECORD_PLAY_LOCK() != s_fRecLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_CAMERA_LOCK() != s_fCameraLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_WAP_LOCK() != s_fNtopLock)
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_SMS_LOCK() != EQC_GetCheckBox(s_hControl2, 0))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_IS_MEMO_LOCK() != EQC_GetCheckBox(s_hControl2, 1))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            if(SKY_GET_SELFINFOLOCK() != EQC_GetCheckBox(s_hControl2, 2))
            {
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
                break;
            }
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    case SE_IO_POPUP_CANCEL:
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paLockMenu, 2, 0);
        break;

    case SE_IO_POPUP_SELECT : 
        switch(pEvent->dwParam)
        {
         case ALL_LOCK:
            s_fAllLock = TRUE;
            s_fInterCallLock =  TRUE;
            s_fPBookLock  = TRUE;
            s_fMenuLock = TRUE;
            s_fCallStackLock = TRUE;
            s_fRecLock = TRUE;
            s_fNtopLock = TRUE;
            s_fSmsLock = TRUE;
            s_fMemoLock = TRUE;
			s_fCameraLock = TRUE;
            s_fSelfInfoLock= TRUE;
            if(s_fFirstScreen)
            {
                EQC_SetCheckBox(s_hControl1, 0);
                EQC_SetCheckBox(s_hControl1, 1);
                EQC_SetCheckBox(s_hControl1, 2);
                EQC_SetCheckBox(s_hControl1, 3);
                EQC_SetCheckBox(s_hControl1, 4);
                EQC_SetCheckBox(s_hControl1, 5);
				EQC_SetCheckBox(s_hControl1, 6);
                EQC_SetCheckBox(s_hControl1, 7);
            }else
            {
                EQC_SetCheckBox(s_hControl2, 0);
                EQC_SetCheckBox(s_hControl2, 1);
                EQC_SetCheckBox(s_hControl2, 2);   
            }
            break;

         case ALL_UNLOCK:
            s_fAllLock = FALSE;
            s_fInterCallLock =  FALSE;
            s_fPBookLock  = FALSE;
            s_fMenuLock = FALSE;
            s_fCallStackLock = FALSE;
            s_fRecLock = FALSE;
            s_fNtopLock = FALSE;
            s_fSmsLock = FALSE;
            s_fMemoLock = FALSE;
			s_fCameraLock = FALSE;
            s_fSelfInfoLock= FALSE;
            if(s_fFirstScreen)
            {
                EQC_ResetCheckBox(s_hControl1, 0);
                EQC_ResetCheckBox(s_hControl1, 1);
                EQC_ResetCheckBox(s_hControl1, 2);
                EQC_ResetCheckBox(s_hControl1, 3);
                EQC_ResetCheckBox(s_hControl1, 4);
                EQC_ResetCheckBox(s_hControl1, 5);
				EQC_ResetCheckBox(s_hControl1, 6);
                EQC_ResetCheckBox(s_hControl1, 7);                
            }else
            {
                EQC_ResetCheckBox(s_hControl2, 0);
                EQC_ResetCheckBox(s_hControl2, 1);
                EQC_ResetCheckBox(s_hControl2, 2);            
            }
            break;
        }       
        break;  

    case SE_IO_MESGBOX_YES:
        if(s_fFirstScreen)
            EQS_SetIOCActive(s_hControl1);
        else
            EQS_SetIOCActive(s_hControl2);

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

