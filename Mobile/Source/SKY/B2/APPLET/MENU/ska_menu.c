/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-14   girin      Created.
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
LOCAL void SKY_API sSKA_EnterMenu(SKY_EVENT_T *pEvent);
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL g_hMenuControl;
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

void SKA_MainMenuTop(SKY_EVENT_T *pEvent)
{

//    HCONTROL hControl;

	switch (pEvent->EventID)
	{
	case SE_APP_START:

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE, 
            NULL, 
            WA_SHOW_CML);

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
            g_hMenuControl = EQC_MenuList(6, 4, 18, 6, &mnMainMenuTop, 0,SKY_GET_SEL_MENU_MODE());
			EQC_SetMenuImage( g_hMenuControl, &g_BmpMainMenuBase);//&g_BmpMenuBase112x88x256c );
//			EQC_SetMenuBackColor( g_hMenuControl, MENU_BACK_COLOR );

#elif (MODEL_ID == MODEL_B0)
            g_hMenuControl = EQC_MenuList(6, 4, 18, 7, &mnMainMenuTop, 0,SKY_GET_SEL_MENU_MODE());
#endif
        
        EQS_SetIOCActive(g_hMenuControl);

		break;

    case SE_APP_RESUME:


        /*
        if (EQS_GetChild() == EQA_MainMenuSet)
        {
            l_MenuMode = pEvent->wParam;
            if ( l_MenuMode == 1) // Text Mode Menu.
            {
                EQC_SET_STYLE(hControl,
                              CS_MENU_LIST_LEFT_ALIGN 
                            | CS_MENU_LIST_ICON 
                            | CS_MENU_LIST_CAPTION
                            );
            }
            else
            {
                EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT);
            }
        }
        */

        EQS_SetIOCActive(g_hMenuControl);
        break;

	case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_IO_MENU_LIST_SELECT:
        EQS_START_CHILD(pEvent->dwParam);
        break;

    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
	}
    
}

LOCAL VOID sMakeSmsCountMenu()
{
    INT8        bTotalCount;
    BYTE        pszReceive[LCD_COLUME_SIZE+1];

    bTotalCount = g_NewMsgCount( );

    memset(pszReceive, NULL, LCD_COLUME_SIZE+1);
    if ( bTotalCount >= 10 )
    {
        sprintf((char*)pszReceive, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE2), SKY_GET_TOKEN(TKN_SMS_IN_BOX), bTotalCount);
    }
    else if ( bTotalCount > 0 )
    {
        sprintf((char*)pszReceive, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE1), SKY_GET_TOKEN(TKN_SMS_IN_BOX), bTotalCount);
    }
    else
    {
        STRCPY(pszReceive, (BYTE*)SKY_GET_TOKEN(TKN_SMS_IN_BOX));
    }

    EQC_SetMenuItemText(g_hMenuControl, 0, pszReceive);
}

void SKA_SMSMenu(SKY_EVENT_T* pEvent)
{

#ifdef FEATURE_SKTT_EQUS_LOCATION
    LOCATION_INFO_T         location;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION    

    switch (pEvent->EventID)
	{
	case SE_APP_START:

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE, 
            NULL, 
            WA_SHOW_CML);

#ifdef FEATURE_SKTT_EQUS_LOCATION
        location = SKY_GET_LOCATION_INFO();

        if(location.rcvinfo)
        {
            g_hMenuControl = EQC_MenuList(6, 4, 18, 7, &mnSMSMenu2, 0, SKY_GET_SEL_MENU_MODE());
        }
        else
        {
            g_hMenuControl = EQC_MenuList(6, 4, 18, 6, &mnSMSMenu1, 0, SKY_GET_SEL_MENU_MODE());
        }
#else //#ifdef FEATURE_SKTT_EQUS_LOCATION

        g_hMenuControl = EQC_MenuList(6, 4, 18, 6, &mnSMSMenu1, 0, SKY_GET_SEL_MENU_MODE());

#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

		EQC_SetMenuImage( g_hMenuControl, &g_BmpSMSMenuBase);//&g_BmpMenuBase112x88x256c );
//		EQC_SetMenuBackColor( g_hMenuControl, MENU_BACK_COLOR );

        sMakeSmsCountMenu();
        EQS_SetIOCActive(g_hMenuControl);

		break;

    case SE_APP_RESUME:
        sMakeSmsCountMenu();
        EQS_SEND_EVENT(SE_APP_DRAW, g_hMenuControl, EQS_NA);
        EQS_SetIOCActive(g_hMenuControl);
        break;

	case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_IO_MENU_LIST_SELECT:
        EQS_START_CHILD(pEvent->dwParam);
        break;

    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
        break;

    case SE_SMS_MYBELL:
    case SE_SMS_MESSAGE:
        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        sMakeSmsCountMenu();
        EQS_SetIOCActive(g_hMenuControl);
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
	}
    
}





void SKA_MainTestMenuTop(SKY_EVENT_T *pEvent)
{
	switch (pEvent->EventID)
	{
	case SE_APP_START:

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SELECT_NONE, 
            NULL, 
            WA_SHOW_CML);

        EQS_SetIOCActive(EQC_MenuList(EQS_NA, EQS_NA, EQS_NA, EQS_NA, &mnTestMenu, 0, SKY_GET_SEL_MENU_MODE()));

		break;

    case SE_APP_RESUME:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;

	case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_IO_MENU_LIST_SELECT:
        EQS_START_CHILD(pEvent->dwParam);
        break;

    case SE_IO_MENU_LIST_CANCEL:
        EQS_END();
        break;

	default:
		EQS_DEF_EVENT_PROC(pEvent);
		break;
	}
    
}

#define NUM_CODE_6      6

#define SRV_PASS_X      (SKY_SCREEN_WIDTH - (EQS_CharWidth('A') * NUM_CODE_6)) / 2//35
#define SRV_PASS_Y      PASS_Y//61//65//69//40 + 5
#define SRV_IMAGE_X     IMAGE_X//0//16
#define SRV_IMAGE_Y     IMAGE_Y//0//TEXT_Y

void SKY_API SKA_SeviceMenu(SKY_EVENT_T *pEvent)
{
    BYTE   sPasswd[7];
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
                SKY_GET_TOKEN(TKN_MENU_SERVICE_CODE),
                WA_SHOW_HEADER| WA_SHOW_CML);

            EQC_Image( 
		       SRV_IMAGE_X, 
		       SRV_IMAGE_Y, 
			   //(BITMAP*)&g_BmpSecret6_120x93x256c
               (BITMAP*)&g_BmpLock
			  ); 
        
            //EQC_StaticText(TEXT_X, TEXT_Y, TKN_PHONE_PASSWORD);
            sNumHandle = EQC_NumberEdit(SRV_PASS_X, SRV_PASS_Y, 6, 1, 6,(BYTE *)"");

            EQC_SET_STYLE(sNumHandle,SECRET_NUMBER_EDITOR_DEFAULT);
            EQC_SetNumberEditBigFont(sNumHandle, TRUE);   // Set Big Font
            EQC_NotifyNumberEditMatchLen(sNumHandle, 6);
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
        if(!STRCMP(sPasswd, (BYTE*)"000000"))
            EQS_START_SIBLING(sSKA_EnterMenu, FALSE);
        else
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_WRONG_PASSWORD),\
			                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        break;

    case EQE_IO_NEDIT_NODIGIT:
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

LOCAL void SKY_API sSKA_EnterMenu(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE_SELECT_NONE, 
                NULL, 
                WA_SHOW_CML);

        EQC_MenuList(EQS_NA,EQS_NA, EQS_NA, 12, &mnControlTestMenu, 0, FALSE);//SKY_GET_SEL_MENU_MODE());
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;
        
    case SE_APP_END :   /* The very last event */ 
        EQS_ReleaseDC();
        break;
        
    case SE_APP_RESUME:
        hControl = EQS_GetNthIOC(0);
        EQS_SetIOCActive(hControl);
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
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
