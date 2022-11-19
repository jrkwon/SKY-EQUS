/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_ntop.c
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

#ifdef FEATURE_WAP
#include "wapcommon.h"
#endif //#ifdef FEATURE_WAP

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
enum tag_NTOP_SETTING_E {
    NS_PROXY_TITLE,
    NS_PROXY_EDIT,
    NS_URL_TITLE,
    NS_URL_EDIT,
    NS_MAX
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
#ifdef FEATURE_WAP
LOCAL int waptmp_length = 0;
#endif //#ifdef FEATURE_WAP

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
void SKY_API sSKA_NtopSet(SKY_EVENT_T *pEvent);
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
void SKY_API SKA_MainNtopSet(SKY_EVENT_T *pEvent)
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
                SKY_GET_TOKEN(TKN_NTOP_SET), 
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       IMAGE_X, 
		       IMAGE_Y, 
               (BITMAP*)&g_BmpLock
			   //(BITMAP*)&g_BmpSecret120x93x256c
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
            EQS_START_SIBLING(sSKA_NtopSet, FALSE);
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
void SKY_API sSKA_NtopSet(SKY_EVENT_T *pEvent)
{
    UINT8 nCtrl;
    STATIC HCONTROL s_hMsgControl, s_hMsgOutYesNo;
    STATIC HCONTROL s_hMsgYesControl;
    STATIC BOOL s_fEndToRoot = FALSE;
    
    STATIC BYTE   pProxyUrl[MAX_NAME_LENGTH+1];
    STATIC BYTE   pInitUrl[MAX_URL_LENGTH+1];

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
         
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_INIT_SELECT_SAVE, 
            SKY_GET_TOKEN(TKN_NTOP_SET), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        EQC_StaticText(EQS_NA, EQS_NA, TKN_PROXY_ADDR);
        EQC_TextEdit(EQS_NA, EQS_NA, 17 ,1 , \
            TEM_ENGLISH_LOWER, MAX_NAME_LENGTH,SKY_GET_WAP_PROXY_URL(),0);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_INIT_URL);
        EQC_TextEdit(EQS_NA, EQS_NA, 17 ,2 , \
            TEM_ENGLISH_LOWER, MAX_URL_LENGTH,SKY_GET_WAP_INIT_URL(),0);
        //EQS_SetIOCActive(EQS_GetNthIOC(1));

        // Formatting..
        EQS_StartFormatter();

        for(nCtrl = 0; nCtrl < NS_MAX; nCtrl++)
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
        
    case SE_APP_SUSPEND: /* child applet is started */
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;
        
    case SE_IO_POPUP_CANCEL:
        //EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;        

    case SE_IO_POPUP_SELECT_CANCEL:
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox(SKY_GET_TOKEN(TKN_INIT_CONFIRM), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(s_hMsgOutYesNo == pEvent->wParam)
        {
            EQS_SEND_EVENT(SE_SOFTKEY_SAVE, EQS_NA, EQS_NA);
        }else
        {
            SKY_SET_WAP_PROXY_URL(INIT_WAP_PROXY_URL); /*기본 초기값으로 설정*/
            SKY_SET_WAP_INIT_URL(INIT_WAP_INIT_URL);
		    SKY_SET_WAP_INIT_MAIL(INIT_WAP_INIT_MAIL);
#ifdef FEATURE_WAP
            //copy to global proxy var
            waptmp_length = get_length((byte *)SKY_GET_WAP_PROXY_URL(),130);
		    memset(gWAPproxy_url,'\x00',130);
		    memcpy(gWAPproxy_url,(void *)SKY_GET_WAP_PROXY_URL(),waptmp_length);

            //copy to global url var
            waptmp_length = get_length((byte *)SKY_GET_WAP_INIT_URL(),130);
            memset(gWAPinit_url,'\x00',130);
		    memcpy(gWAPinit_url,(void *)SKY_GET_WAP_INIT_URL(),waptmp_length);

            //copy to global wap mail address var
            waptmp_length = get_length((byte *)SKY_GET_WAP_INIT_MAIL(),130);
		    memset(gWAPEmail_Url,'\x00',130); //for using PUSH
		    memcpy(gWAPEmail_Url,(const void *)SKY_GET_WAP_INIT_MAIL(),waptmp_length);
#endif //

            s_hMsgYesControl = 
                EQC_MessageBox(SKY_GET_TOKEN(TKN_INIT_PROCESS_END), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }        
        break;

    case SE_IO_MESGBOX_NO:
        if(s_hMsgOutYesNo == pEvent->wParam)
        {
            if(s_fEndToRoot)
            {
                s_fEndToRoot = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
                EQS_End();
        }
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl || pEvent->wParam == s_hMsgYesControl )
        {
            if(s_fEndToRoot)
            {
                s_fEndToRoot = FALSE;
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
                EQS_End();
        }
        break;
 
    case SE_SOFTKEY_SAVE:
        EQC_GetTextEditData(EQS_GetNthIOC(NS_PROXY_EDIT), pProxyUrl);
        EQC_GetTextEditData(EQS_GetNthIOC(NS_URL_EDIT), pInitUrl);

        if(pProxyUrl[0] == 0 || pInitUrl[0] == 0)
        {
            EQC_MessageBox(SKY_GET_TOKEN(TKN_NO_INPUT_NTOP_ADDR), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        }
        else
        {
            SKY_SET_WAP_PROXY_URL(pProxyUrl);
            SKY_SET_WAP_INIT_URL(pInitUrl);
#ifdef FEATURE_WAP
            //copy to global proxy var
            waptmp_length = get_length((byte *)SKY_GET_WAP_PROXY_URL(),130);
		    memset(gWAPproxy_url,'\x00',130);
		    memcpy(gWAPproxy_url,(void *)SKY_GET_WAP_PROXY_URL(),waptmp_length);

            //copy to global url var
            waptmp_length = get_length((byte *)SKY_GET_WAP_INIT_URL(),130);
            memset(gWAPinit_url,'\x00',130);
		    memcpy(gWAPinit_url,(void *)SKY_GET_WAP_INIT_URL(),waptmp_length);

            //copy to global wap mail address var
            waptmp_length = get_length((byte *)SKY_GET_WAP_INIT_MAIL(),130);
		    memset(gWAPEmail_Url,'\x00',130); //for using PUSH
		    memcpy(gWAPEmail_Url,(const void *)SKY_GET_WAP_INIT_MAIL(),waptmp_length);
#endif //
            s_hMsgControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_DONE_SAVE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        break;
        
    case SE_IO_TEDIT_DONE:
        break;

    case SE_KEY_CLEAR:
        EQC_GetTextEditData(EQS_GetNthIOC(NS_PROXY_EDIT), pProxyUrl);
        EQC_GetTextEditData(EQS_GetNthIOC(NS_URL_EDIT), pInitUrl);

        if(STRCMP(SKY_GET_WAP_PROXY_URL(), pProxyUrl))
        {
            s_hMsgOutYesNo = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        if(STRCMP(SKY_GET_WAP_INIT_URL(),pInitUrl))
        {
            s_hMsgOutYesNo = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        EQS_End();
        break;

    case SE_KEY_END:
        s_fEndToRoot = TRUE;
        EQC_GetTextEditData(EQS_GetNthIOC(NS_PROXY_EDIT), pProxyUrl);
        EQC_GetTextEditData(EQS_GetNthIOC(NS_URL_EDIT), pInitUrl);

        if(STRCMP(SKY_GET_WAP_PROXY_URL(), pProxyUrl))
        {
            s_hMsgOutYesNo = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        if(STRCMP(SKY_GET_WAP_INIT_URL(),pInitUrl))
        {
            s_hMsgOutYesNo = EQC_MessageBox(SKY_GET_TOKEN(TKN_CONFIRM_SAVE), MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;
        }
        s_fEndToRoot = FALSE;
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

   default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


