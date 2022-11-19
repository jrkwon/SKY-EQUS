/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_sms.c
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
** 2000-12-16   huni     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/

#include <stdio.h>
#include <stdlib.h>
#include "ska.h"
#ifdef FEATURE_WAP
#include "mmi_data.h"
#endif //#ifdef FEATURE_WAP

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_RECEIVE_STR_LENGTH		(20+1)
#define NUM_SMS_MODE                3
#define NUM_SMS_KIND                7

#define SMS_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT | \
                                CS_SCROLL_LIST_COMBO | \
                                CS_SCROLL_LIST_ENUMBERATED 

#define MESSAGE_NEW             TRUE
#define SMS_DELETE_NO_DATA      0x01
#define SMS_VIEW_DELETE         0x02
#define SMS_VIEW_SAVE           0x04               

// huni 2001.8.9 SKY_ANNUNCIATORS_HEIGHT     19
#define SMS_NOTIFICATION_ANI_X          28
#define SMS_NOTIFICATION_ANI_Y          24 - SKY_ANNUNCIATORS_HEIGHT

#define SMS_NOTIFICATION_TITLE_Y        76 - SKY_ANNUNCIATORS_HEIGHT
#define SMS_NOTIFICATION_DATE_Y         104 - SKY_ANNUNCIATORS_HEIGHT //64
#define SMS_NOTIFICATION_CALL_BACK_Y    121 - SKY_ANNUNCIATORS_HEIGHT //79(119)

#define SMS_NOTIFICATION_FRAME_X          4
#define SMS_NOTIFICATION_FRAME_Y          94 - SKY_ANNUNCIATORS_HEIGHT


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tag_SKA_SMS_E {
    SS_TIME_TITLE,        
    SS_TIME,
    SS_MESSAGE_TITLE,
    SS_MESSAGE,
    SS_MAX
};

enum tag_SKA_SMS_MSG_ALARM {
    SMA_MSG_ALARM_SETTING_TITLE,
    SMA_MSG_ALARM_SETTING, 
    SMA_MSG_ALARM_KIND_TITLE,
    SMA_MSG_ALARM_KIND,
    SMA_MSG_ALARM_VOLUME_TITLE, 
    SMA_MSG_ALARM_VOLUME,
    SMA_MAX
};    
enum tag_SKA_SMS_SETTING_E {
    SS_SMS_TYPE_TITLE,
    SS_SMS_TYPE,
    SS_SMS_KIND_TITLE,
    SS_SMS_KIND,
    SS_SMS_VOL_TITLE,
    SS_SMS_VOL,
    SS_SMS_MAX
};

enum tag_SKA_SMS_WAP_PUSH_E {
    SWP_STATIC_TEXT_1,
    SWP_TEXT_EDIT_1,
    SWP_STATIC_TEXT_2,
    SWP_STATIC_TEXT_3,
    SWP_MAX
}; 
enum tag_SKA_SMS_WAP_MAIL_E {
    SWM_STATIC_TEXT_1,
    SWM_TEXT_EDIT_1,
    SWM_STATIC_TEXT_2,
    SWM_STATIC_TEXT_3,
    SWM_STATIC_TEXT_4,
    SWM_MAX
};   
/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
// SMS Bell Kind
CONST BELL_NAME_TYPE_T aSMS_Bell_List[NUM_SMS_KIND] =
#if (MODEL_ID == MODEL_B0)
{
    {TKN_SMS_ALARM_TONE1,     MUS_SMS_BELL1},
    {TKN_SMS_ALARM_TONE2,     MUS_CUCKOO},
    {TKN_SMS_ALARM_TONE3,     MUS_SMS_BELL3},
    {TKN_SMS_ALARM_TONE4,     MUS_SMS_BELL4},
    {TKN_SMS_ALARM_TONE5,     MUS_SMS_BELL5},
    {TKN_SMS_ALARM_TONE6,     MUS_SMS_BELL6},
    {TKN_SMS_ALARM_TONE7,     MUS_SMS_BELL7}
};
#else
{
    {TKN_SMS_ALARM_TONE1,     MUS_SMS_1},
    {TKN_SMS_ALARM_TONE2,     MUS_SMS_2},
    {TKN_SMS_ALARM_TONE3,     MUS_SMS_3},
    {TKN_SMS_ALARM_TONE4,     MUS_SMS_4},
    {TKN_SMS_ALARM_TONE5,     MUS_SMS_5},
    {TKN_SMS_ALARM_TONE6,     MUS_SMS_6},
    {TKN_SMS_ALARM_TONE7,     MUS_SMS_7}
};
#endif



SKY_TOKEN paTextPopMenu[3]=
{
    TKN_SMS_TEXT_DELETE,    // 삭제
    TKN_SMS_TEXT_REPLY,     // 회신
    TKN_SMS_TEXT_DELIVER    // 전달
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
extern WAP_Card *g_pCard;
#endif //#ifdef FEATURE_WAP

#ifdef FEATURE_SKTT_EQUS_LOCATION

#ifdef FEATURE_WAP
BYTE *paLocationMenu[7];
BYTE *paSmsMenu[6];
#else // #ifdef FEATURE_WAP
BYTE *paLocationMenu[6];
BYTE *paSmsMenu[5];
#endif

#else //#ifdef FEATURE_SKTT_EQUS_LOCATION
BYTE *paSmsMenu[5];
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

BYTE *paSmsSubMenu[3];
BYTE *paSmsDeleteMenu[7];
BYTE *g_pszReceive;
BYTE *g_pszText;
BYTE *g_pszVoice;
BYTE *g_pszMybell;

BYTE *pszDelete1;
BYTE *pszDelete2;
BYTE *pszDelete3;
BYTE *pszDelete4;
BYTE *pszDelete5;
BYTE *pszDelete6;
BYTE *pszDelete7;

BYTE *paMsgListBuf[MAX_RECEIVE_MESSAGE];
BYTE *paMsgViewBuf;

INT8    nListType = UI_SMS_TXT_NEW_NOT_URGENT; // 3
INT8    nListIdx = 0; 
STATIC  INT8    nNewNotUrgentCnt = 0;

// 알람설정
CONST SKY_TOKEN aSmsAlarmMode[NUM_SMS_MODE] =
{
    TKN_SMS_ALARM_SET1,
    TKN_SMS_ALARM_SET2,
    TKN_SMS_ALARM_SET3
};


LOCAL BYTE *ppSMSMode[NUM_SMS_MODE];
LOCAL BYTE *ppSMSKind[NUM_SMS_KIND];

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
//LOCAL VOID SKA_SMSMenu(SKY_EVENT_T* pEvent);
LOCAL VOID SKA_TextMessage(SKY_EVENT_T* pEvent);
//LOCAL VOID SKA_ViewTextMessage(SKY_EVENT_T* pEvent);
//LOCAL VOID SKY_API SKA_MsgAlarm(SKY_EVENT_T* pEvent);
/* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
VOID sGVMDownloadNotifyDisplay(VOID);
#endif
/* <-- */
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

INT8  g_GetLength( BYTE *CheckStr, INT8  nBufLen)
{
	INT8    i;
	INT8    nRetVal = 0;

	for(i=0; i < nBufLen ; i++ ) {
		if( *CheckStr == '\0' )
		{
			return( nRetVal );
		}
		if( (*CheckStr != ' ') && *CheckStr )
			nRetVal = i + 1;
		CheckStr++;
	}

	return( nRetVal );
} 

ui_sms_title_type sMakeNotificationDisplay(void)
{
   
    BYTE            szBuf1[33]; //call back number
    BYTE            szBuf2[LCD_COLUME_SIZE-1]; //time stamp
    BYTE*           TmpBuf;

    ui_sms_title_type    sMessageType;
    BYTE    szTmpStr[LCD_COLUME_SIZE+1];
    UINT8   nPos, nHyphenpos, nCallBackLen;
    //COLOR OldColor;
              
    memset(szBuf1, NULL, 33);
    memset(szTmpStr, NULL, LCD_COLUME_SIZE + 1);
    memset(szBuf2, ' ', LCD_COLUME_SIZE-2);
    szBuf2[LCD_COLUME_SIZE-2] = 0;
    sMessageType = (ui_sms_title_type) g_MessageNotif( szBuf1, szBuf2); 

    EQS_GetDC(TRUE);
    EQS_SetDrawMode(DM_OR);

    EQS_SetTextAlignHoriz(TAH_CENTER);
    //OldColor = EQS_GetTextBackColor();
    EQS_SetTextBackColor(EQS_GetBackColor());

    if ( sMessageType == NOT_URGENT_TEXT_SMS )
    {
        EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_TITLE_Y, (BYTE*)SKY_GET_TOKEN(TKN_SMS_NOTIF_TEXT));
    }
    else if ( sMessageType == URGENT_TEXT_SMS )
    {
        EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_TITLE_Y, (BYTE*)SKY_GET_TOKEN(TKN_SMS_NOTIF_URGENT_TEXT));
    }
    else if ( sMessageType == NOT_URGENT_VOICE_SMS )
    {
        EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_TITLE_Y, (BYTE*)SKY_GET_TOKEN(TKN_SMS_NOTIF_VOICE));
    }
    else 
    {
        EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_TITLE_Y, (BYTE*)SKY_GET_TOKEN(TKN_SMS_NOTIF_URGENT_VOICE));
    }        

    EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_DATE_Y, (BYTE *)szBuf2);

    nCallBackLen = STRLEN(szBuf1);
    if(nCallBackLen > 0)
    {
        if( ( TmpBuf = FindNumber((BYTE*)szBuf1) ) != NULL )
        {
            if(STRLEN(TmpBuf) > LCD_COLUME_SIZE-1)
            {
                (void) memcpy ( szTmpStr, 
                                (const void *) TmpBuf,
                                LCD_COLUME_SIZE-1 );
            }
            else
            {
                (void) memcpy ( szTmpStr, 
	  			                (const void *) TmpBuf, 
                                STRLEN(TmpBuf) );
            }
        }
        else
        {

            nPos = FindCodeSet(szBuf1);
            nHyphenpos = FindHypen( szBuf1, nPos, 8);  // 8 : Max Length (Except 011, 02 length )

            MakeHypenedNumber( szBuf1, szTmpStr,
                               (UINT8)g_GetLength(szBuf1, LCD_COLUME_SIZE-2), 
                               nPos, 
                               nHyphenpos );
        }

        EQS_TextOutStr(SKY_SCREEN_WIDTH/2, SMS_NOTIFICATION_CALL_BACK_Y, (BYTE *)szTmpStr);
    }

    EQS_ReleaseDC();

    return ( sMessageType );
}

VOID sWapMailNotifDisplay(void)
{
    UINT8       nLine, nCtrl;

    SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);

    EQC_StaticText(EQS_NA, EQS_NA,TKN_SMS_WAP_MAIL_TITLE);
    
    nLine = FormatWapMailMessage();

    EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 4, TEM_KOREAN, nLine*SMS_SCREEN_SIZE,
                        paMsgViewBuf, 0);
    EQC_FitTextEditLine(EQS_GetNthIOC(SWM_TEXT_EDIT_1));

    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_LINE);
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_MAIL_SUB1);
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_MAIL_SUB2);
    
    EQS_StartFormatter();
    for( nCtrl = 0 ; nCtrl < SWM_MAX; nCtrl++)
    {

        if(nCtrl == SWM_STATIC_TEXT_1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_TOP, FALSE);
        }
        else if(nCtrl == SWM_TEXT_EDIT_1)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SWM_TEXT_EDIT_1), FALSE); 
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);            
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);       
        }

        EQS_NewLine(); 
    }  

    EQS_EndFormatter();

    EQC_SET_STYLE( EQS_GetNthIOC(SWM_TEXT_EDIT_1), CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );
}

VOID sWapPushNotifDisplay(void)
{
    UINT8 nLine, nCtrl;

#ifdef FEATURE_SKTT_PM
    STATIC BOOL s_fIsPhotoMail = FALSE;
#endif //

    SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);    

#ifdef FEATURE_SKTT_PM    
    nLine = FormatWapPushMessage(&s_fIsPhotoMail);
    if(s_fIsPhotoMail == TRUE)
    {
        EQC_StaticText(EQS_NA, EQS_NA,TKN_PM_PHOTO_MAIL); 
        s_fIsPhotoMail = FALSE;
    }
    else
    {
        EQC_StaticText(EQS_NA, EQS_NA,TKN_SMS_WAP_PUSH_TITLE);    
    }        
#else
    nLine = FormatWapPushMessage();
    EQC_StaticText(EQS_NA, EQS_NA,TKN_SMS_WAP_PUSH_TITLE);
#endif

    EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 4, TEM_KOREAN, 
                        nLine*SMS_SCREEN_SIZE, paMsgViewBuf, 0);
    
    EQC_FitTextEditLine(EQS_GetNthIOC(SWP_TEXT_EDIT_1));

    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_LINE);
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_PUSH_SUB);
    
    EQS_StartFormatter();
    for( nCtrl = 0 ; nCtrl < SWP_MAX; nCtrl++)
    {
        
        if(nCtrl == SWP_STATIC_TEXT_1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_TOP, FALSE);
        }
        else if(nCtrl == SWP_TEXT_EDIT_1)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(nCtrl), FALSE);
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
        }
        EQS_NewLine();      
    }
    EQS_EndFormatter();

    EQC_SET_STYLE( EQS_GetNthIOC(SWP_TEXT_EDIT_1), CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );
}

#ifdef FEATURE_SKVM
VOID sSkvmLoadNotifDisplay(void)
{
    UINT8 nLine, nCtrl;

    SKY_StopMusic();
    SKY_DoSMSAlert();
    SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);    

    EQC_StaticText(EQS_NA, EQS_NA,TKN_SKVM_DOWNLOAD_TITLE);
    
    nLine = FormatSkvmLoadMessage();
    EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 3, TEM_KOREAN, 
                         nLine*SMS_SCREEN_SIZE, paMsgViewBuf, 0);
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SKVM_DOWNLOAD1);
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SKVM_DOWNLOAD2);
    
    EQS_StartFormatter();
    for( nCtrl = 0 ; nCtrl < SWP_MAX; nCtrl++)
    {        

        if(nCtrl == SWP_STATIC_TEXT_1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_TOP, FALSE);
        }
        else if(nCtrl == SWP_TEXT_EDIT_1)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SWP_TEXT_EDIT_1), FALSE); 
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);            
            
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);       
        }

        EQS_NewLine();
        

    }
    EQS_EndFormatter();

    EQC_SET_STYLE( EQS_GetNthIOC(SWP_TEXT_EDIT_1), CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );
}
#endif

/* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
VOID sGVMDownloadNotifyDisplay(VOID) {
    UINT8 nLine, nCtrl;    
    static char        *l_tempBuf;
    int     fFlag;
    
    SKY_StopMusic();
    SKY_DoSMSAlert();
    
    EQC_StaticText(EQS_NA, EQS_NA,TKN_GVM_DOWNLOAD_TITLE); 
    nLine = FormatGVMMessage();

    EQC_TextEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 3, TEM_KOREAN, 
                         nLine*SMS_SCREEN_SIZE, paMsgViewBuf, 0);
    
    if (g_fDownloadRequestFromSMS) {    // flag == 1
        EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_DOWNLOAD1);
        EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_DOWNLOAD2);
    }
    
    EQS_StartFormatter();
    for( nCtrl = 0 ; nCtrl < SWP_MAX; nCtrl++)
    {        

        if(nCtrl == SWP_STATIC_TEXT_1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_TOP, FALSE);
        }
        else if(nCtrl == SWP_TEXT_EDIT_1)
        {
            EQC_SET_FOCUS(EQS_GetNthIOC(SWP_TEXT_EDIT_1), FALSE); 
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);            
            
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);       
        }

        EQS_NewLine();      

    }
    EQS_EndFormatter();

    EQC_SET_STYLE( EQS_GetNthIOC(SWP_TEXT_EDIT_1), CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );    
}
#endif 
/* <-- */
VOID SKA_MessageNotif(SKY_EVENT_T* pEvent)      //white 2001.03.21
{
    STATIC  ui_sms_title_type       s_MsgType;
    STATIC  TELESEVICE_KIND_TYPE    s_TeleIdType;
    STATIC  HCONTROL           hCtrlNoSrvc, hCtrlAni;
	STATIC  UINT8              nPreviousMinute;
    

	clk_julian_type            julian;
	DWORD                      dwTime;
	ALARM_DISP_TYPE_T          AlarmType;
   
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        g_DoOldTextMatch();
        s_TeleIdType = (TELESEVICE_KIND_TYPE) g_TeleId();

#ifdef FEATURE_SKVM
		if (s_TeleIdType == SKVM_LOAD)
			EQS_SetWindow(SKY_DEF_WIN_X,
				SKY_DEF_WIN_Y, 
				SKY_DEF_WIN_DX, 
				160, //SKY_DEF_WIN_DY, 
				NULL, 
				NULL, 
				WA_SHOW_HEADER | WA_SHOW_CML);

		else
#endif
		{
			EQS_SetWindow(SKY_DEF_WIN_X,
				SKY_DEF_WIN_Y, 
				SKY_DEF_WIN_DX, 
				160, //SKY_DEF_WIN_DY, 
				CL_NONE_CONFIRM_NONE, 
				NULL, 
				WA_SHOW_HEADER | WA_SHOW_CML);

		}
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);

		dwTime = SKY_GetLocalTime();
		clk_secs_to_julian(dwTime, &julian);
		nPreviousMinute = julian.minute;
        
        // Every 2 min Blink SNI.... huni 2001.11.12
//        if (SKY_GET_SMS_ALERT_TYPE() == SAT_EVERY_2MINIUTE)
//        {
            SKY_SetGeneralTimer(60 * 2 * 1000, TRUE);
//        }
        /* jrkwon 2001.04.26 */
//		EQS_StartTimer(EQS_TIMER_1, 5*1000, TRUE);      //white 2001.06.01 Deleted.
//        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);


        if( s_TeleIdType == COMMON_SMS )
        {
#ifdef FEATURE_WAP
            if(SKY_IS_IN_WAP()) 
            {
                EQS_END();        
            }
            else
            {
                //idle 상태 
//                hCtrlAni = EQC_Animation(SMS_NOTIFICATION_ANI_X, SMS_NOTIFICATION_ANI_Y, &g_AniSmsRece54x39x256c, NULL);
                hCtrlAni = EQC_Animation(SMS_NOTIFICATION_ANI_X, SMS_NOTIFICATION_ANI_Y, &g_AniReceiveMessage, NULL);

                if(SKY_IS_PHONE_OPEN()) 
                {
                    /* 프레임 */
//		            EQC_Image(SMS_NOTIFICATION_FRAME_X, SMS_NOTIFICATION_FRAME_Y, (BITMAP*)&g_BmpGeneralPannel);
                    
                    EQC_StartAnimation(hCtrlAni, TRUE);

                }                
            }
#else   //FEATURE_WAP
//            hCtrlAni = EQC_Animation(SMS_NOTIFICATION_ANI_X, SMS_NOTIFICATION_ANI_Y, &g_AniSmsRece54x39x256c, NULL);
            hCtrlAni = EQC_Animation(SMS_NOTIFICATION_ANI_X, SMS_NOTIFICATION_ANI_Y, &g_AniReceiveMessage, NULL);

            if(SKY_IS_PHONE_OPEN()) 
            {
                    /* 프레임 */
//		            EQC_Image(SMS_NOTIFICATION_FRAME_X, SMS_NOTIFICATION_FRAME_Y, (BITMAP*)&g_BmpGeneralPannel);
                    
					EQC_StartAnimation(hCtrlAni, TRUE);
            }
#endif//FEATURE_WAP
        }
        else if( s_TeleIdType == WAP_MAIL )
        {
            sWapMailNotifDisplay();        
        }
        else if( s_TeleIdType == WAP_PUSH )
        {
            sWapPushNotifDisplay();        
        }
        /* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
        else if ( s_TeleIdType == GVM_DOWNLOAD) {
            sGVMDownloadNotifyDisplay();
        }
#endif
        /* <-- */
#ifdef FEATURE_SKVM
        else if( s_TeleIdType == SKVM_LOAD )
            sSkvmLoadNotifDisplay();
#endif

        EQS_FitCanvasSize();
        break;

    case SE_APP_RESUME:    /* end child */
        if(SKY_IS_PHONE_OPEN())
        {
            if( s_TeleIdType == COMMON_SMS )
                EQC_StartAnimation(hCtrlAni, TRUE);        
        }
        break;

    case SE_APP_DRAW:
        if( s_TeleIdType == COMMON_SMS )
        {
            s_MsgType = (ui_sms_title_type) sMakeNotificationDisplay(); // Text out으로 구현되어 있으므로, 
                                                                        // SE_APP_DRAW event를 받을때 마다
                                                                        // update시킨다.
        }
        break;

    case SE_APP_END :     /* The very last event */
        if( s_TeleIdType == COMMON_SMS )
            EQC_StopAnimation(hCtrlAni);  

        SKY_SetGeneralTimer(0, FALSE);  // Stop general timer.
        /* undersea 2001-08-20 --> @why, what */
        g_fDownloadRequestFromSMS = FALSE;
        /* <-- */
        EQS_ReleaseDC();
        break;


    case SE_APP_SUSPEND : /* start child */
        break;
#ifdef FEATURE_WAP
    case SE_CALL_ENDED ://UI_CALL_ENDED_E 
	case SE_CALL_INCOMING:		
        if(SKY_IS_IN_WAP()) 
        {
            if(g_pCard != NULL)
            {                
                EQS_END_EVENT(pEvent->EventID, EQS_NA, EQS_NA);
            }
            else
            {
                //WAP_NOTI_DSCONNECT_INPB();
                EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
            }                     
        }
        else
        {
			EQS_DEF_EVENT_PROC(pEvent);			
		}
        break;

#endif //#ifdef FEATURE_WAP

    case SE_SYS_PACEMAKER:  //2001.07.26 redstar
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP()) 
        {
			EQS_DEF_EVENT_PROC(pEvent);
			break;
		}
#endif//#ifdef FEATURE_WAP
		dwTime = SKY_GetLocalTime();
		clk_secs_to_julian(dwTime, &julian);
		if( nPreviousMinute != julian.minute)
		{
			nPreviousMinute = julian.minute;
			if( SKY_GetActiveAlarmFlag() == FALSE )
			{
				sFindAlarmOn( &julian, &AlarmType );
				if( AlarmType.Type > 0 )
				{
					EQS_START_CHILD_EVENT( sSKA_PlayAlarm, SE_APP_START, AlarmType.Type, AlarmType.nAddr );
					break;
				}
			}
		}        
		EQS_DEF_EVENT_PROC(pEvent);
		break;

    case SE_TM_TIMER:
        SKY_StopMusic();
        if (SKY_GET_SMS_ALERT_TYPE() == SAT_EVERY_2MINIUTE)
            SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_UNREAD_SMS, TRUE);
        break;

    case SE_SMS_MESSAGE:
        EQS_START_SIBLING(SKA_MessageNotif, FALSE);
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_KEY_PHONE_OPEN:
        if( s_TeleIdType == COMMON_SMS ) 
            EQC_StartAnimation(hCtrlAni, TRUE);        
        break;

    case SE_KEY_PHONE_CLOSE:
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP()) 
        {
            EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        }
        else
#endif  //#ifdef FEATURE_WAP
            EQS_PASS_CURRENT_EVENT();
        break;

    case SE_SOFTKEY_CONFIRM:
        SKY_StopMusic();
        {
            if( s_TeleIdType == COMMON_SMS)
            {
                if(SKY_GET_SMS_LOCK_MODE())// && !SKY_IS_WAP_LOCK())       //white 2001.03.21 
                {
                    EQS_START_SIBLING_EVENT(SKA_MainSmsCode,SE_APP_START,SMSCODE_MESSAGE,EQS_NA, FALSE);
                }
                else
                {
                    SKY_SET_SMS_LOCK_MODE(SKY_IS_SMS_LOCK());

                    SKY_SetGeneralTimer(0, FALSE);  // Stop general timer.
                    if ( s_MsgType == NOT_URGENT_TEXT_SMS || s_MsgType == URGENT_TEXT_SMS ) // Text Message
                        EQS_START_SIBLING_EVENT(SKA_ViewTextMessage,
                                                SE_APP_START,
                                                EQS_NA,
                                                MESSAGE_NEW,
                                                FALSE);
                    else if ( s_MsgType == NOT_URGENT_VOICE_SMS || s_MsgType == URGENT_VOICE_SMS )
                        EQS_START_SIBLING_EVENT(SKA_VoiceMessage,
                                                SE_APP_START,
                                                EQS_NA,
                                                MESSAGE_NEW,
                                                FALSE);
                }
            }
#ifdef FEATURE_GVM
            else if ( s_TeleIdType == GVM_DOWNLOAD) {
                SKY_SetGeneralTimer(0, FALSE);  // Stop general timer.
                EQS_START_SIBLING_EVENT(SKA_ViewTextMessage,
                                        SE_APP_START,
                                        EQS_NA,
                                        MESSAGE_NEW,
                                        FALSE);
            }
#endif // FEATURE_GVM
            else
            {
#ifdef FEATURE_WAP
                if(SKY_IS_IN_WAP()) 
                {
                    if( s_TeleIdType == WAP_PUSH )
                        EQS_END_EVENT(SE_WAP_PUSH, EQS_NA, EQS_NA);
                    else if( s_TeleIdType == WAP_MAIL )
                        EQS_END_EVENT(SE_WAP_MAIL, EQS_NA, EQS_NA);
                }
                else
                {
                    if(SKY_IS_WAP_LOCK())
                    {   
                        if( s_TeleIdType == WAP_PUSH )
                            EQS_START_SIBLING_EVENT(SKA_MainNtopCode, SE_APP_START, 
                                                INITURL_IS_WAPPUSH,EQS_NA, FALSE);
                        else if( s_TeleIdType == WAP_MAIL )
                            EQS_START_SIBLING_EVENT(SKA_MainNtopCode, SE_APP_START,
                                            INITURL_IS_WAPMAIL ,EQS_NA, FALSE);

                    }else if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
		            {
                        if( s_TeleIdType == WAP_PUSH )
                        EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                            EQS_NA,INITURL_IS_WAPPUSH ,FALSE);     
                        else if( s_TeleIdType == WAP_MAIL )
                        EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                            EQS_NA,INITURL_IS_WAPMAIL ,FALSE);
		            }
		            else if(SKY_IS_MAINTREQ())
		            {    
		              hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		                                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);          
		            }
		            else if(SKY_IS_NOSERVICE())
		            {			 
		              hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
		            }                       
                }
#endif  // FEATURE_WAP
            }
        }
        break;

#ifdef FEATURE_WAP
   case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrlNoSrvc)
		{
		   EQS_END_TO(EQS_POP_TO_ROOT);
		}
        break;
#endif //FEATURE_WAP

#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_TIMEOUT:
		CheckCurrentTime();
		break;
#endif //#ifdef FEATURE_SKTT_LOCATION
        
    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:
    case SE_KEY_0:
    case SE_KEY_STAR:
    case SE_KEY_POUND:
        EQS_END();
        break;

    //* undersea 2001-07-09 --> @why, what */
    case SE_KEY_SEND:
        if( g_TeleId() == GVM_DOWNLOAD && g_fDownloadRequestFromSMS)
        {
            EQS_START_SIBLING(SKA_MainGVM, FALSE);
        }
#ifdef FEATURE_SKVM
		else if ( g_TeleId() == SKVM_LOAD) {
		    SKY_StopMusic(); 
			EQS_SetDirectDraw(TRUE);
			if (!SKVM_IS_PLAY_BG())
				EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_APP_START, EQS_NA,INIT_SKVM_MSDURL, FALSE);     
			else EQS_START_SIBLING_EVENT(SKA_MainSKVMBG, SE_APP_START, EQS_NA,INIT_SKVM_MSDURL, FALSE);     
		}
#endif //FEATURE_SKVM
        break;
    /* <-- */
        
    default :
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP() && pEvent->EventID == SE_KEY_END) 
        {
            EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        }
        else
#endif  //#ifdef FEATURE_WAP
	    EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

#if 0
LOCAL VOID SKA_MessageDelete(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_SUB_MENU1), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        g_DeleteTextMessage();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
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
#endif

VOID SKA_ViewTextMessage(SKY_EVENT_T* pEvent)
{
    STATIC  BYTE    szCallBackBuf[33];  //32+1
    STATIC  BYTE    szCallBackNameBuf[LCD_COLUME_SIZE-2];
    STATIC  BYTE    szTitle[LCD_COLUME_SIZE];

    STATIC  ui_sms_kind_type    sSmsKindType;
    STATIC  HCONTROL hHnd1, hHnd2, hHnd3, hHnd4, hHnd5;
    STATIC  INT8     nCallBackLength;

    UINT8   nCtrl;
    INT8    nLine;
    BYTE    szTimeBuf[LCD_COLUME_SIZE-1];
    BYTE*   TmpBuf;
    HCONTROL    hText, hPopup, hMsg;


    SKY_TOKEN paSMSRcvMsgMenuItem[] = {
        TKN_SMS_TEXT_DELETE,
        TKN_SMS_SAVE_MSG
    };

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        memset(szTimeBuf, ' ', LCD_COLUME_SIZE-2);
        szTimeBuf[LCD_COLUME_SIZE-2] = 0;
        memset(szCallBackBuf, NULL, 33);
        memset(szCallBackNameBuf, NULL, LCD_COLUME_SIZE-2);
        memset(szTitle, NULL, LCD_COLUME_SIZE);

        nLine = 0;

        if(pEvent->dwParam == MESSAGE_NEW)
        {            
            ReadNewMessage();
        }

        sSmsKindType = g_FormatTextMessage( szTimeBuf, szCallBackBuf, &nLine); 


        if(sSmsKindType == NORMAL_SMS)
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_NORMAL));
        }
        else if ( sSmsKindType == URGENT_SMS )
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_URGENT));
        }            
        else if ( sSmsKindType == EMERGENCY_SMS )
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_SMS_MSG_KIND_EMERGENCY));
        }
        else if(sSmsKindType == WAP_PUSH_SMS)
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_SMS_WAP_PUSH_TITLE));        
        }
        else if(sSmsKindType == WAP_MAIL_SMS)
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_SMS_WAP_MAIL_TITLE));                
        }
#ifdef FEATURE_SKTT_PM
        else if(sSmsKindType == WAP_PUSH_PHOTOMAIL)
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_PM_PHOTO_MAIL));                       
        }            
#endif //FEATURE_SKTT_PM
        /* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
        else if (sSmsKindType == GVM_DL_SMS)
        {
            strcpy((CHAR *)szTitle, (CHAR *)SKY_GET_TOKEN(TKN_GVM_DOWNLOAD_TITLE)); //TKN_SMS_GVM_DOWNLOAD_TITLE));
        }
#endif //FEATURE_GVM
        /* <-- */

        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            210, // SKY_DEF_WIN_DY, 
            CL_MENU_REPLY_FWD,
            szTitle,
            WA_SHOW_CML | WA_SHOW_HEADER);


        // 시간정보 title 
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_RECEIVE_DATE);
        // 시간정보
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_NOTIF_NULL);

        // 메시지
        EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CHRI_CONTENTS);

        hText = EQC_TextEdit( EQS_NA, EQS_NA, LCD_COLUME_SIZE-2, MAX_MSG_VIEW_LINE, TEM_KOREAN, 
                        MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE, paMsgViewBuf, 0);
        EQC_GetTextEditLine(hText);
        EQC_FitTextEditLine(hText);

        // 회신번호
        nCallBackLength = g_GetLength( szCallBackBuf, 32);
        if(  nCallBackLength > 0 )
        {
            hHnd1 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_CALLBACK);

            if( ( TmpBuf = FindNumber((BYTE*)szCallBackBuf) ) != NULL )
            {
                if(STRLEN(TmpBuf) > LCD_COLUME_SIZE-2)
                {
                    (void) memcpy ( szCallBackNameBuf, 
                                    (const void *) TmpBuf,
                                    LCD_COLUME_SIZE-2 );
                }
                else
                {
                    (void) memcpy ( szCallBackNameBuf, 
	  			                    (const void *) TmpBuf, 
                                    STRLEN(TmpBuf) );
                }
                hHnd2 = EQC_TextEdit(EQS_NA, EQS_NA, LCD_COLUME_SIZE-2, 1, TEM_KOREAN, 
                                        LCD_COLUME_SIZE-2, szCallBackNameBuf, 0);
            }
            else
            {            
                hHnd2 = EQC_NumberEdit(EQS_NA, EQS_NA, SMS_SCREEN_SIZE, 1, 32, szCallBackBuf );
            }
        }
        else
        {
            hHnd1 = NULL;
            hHnd2 = NULL;
        }

        //WAP PUSH/WAP MAIL
#if defined(FEATURE_SKTT_PM)
        if((sSmsKindType == WAP_MAIL_SMS) 
            || (sSmsKindType == WAP_PUSH_SMS)
            || (sSmsKindType == WAP_PUSH_PHOTOMAIL) )            
#else   //FEATURE_SKTT_PM
        if((sSmsKindType == WAP_MAIL_SMS) || (sSmsKindType == WAP_PUSH_SMS))
#endif  //FEATURE_SKTT_PM
        {
            if(nCallBackLength > 0)
                EQS_SetWindowCML(CL_DELETE_REPLY_NONE);
            else
                EQS_SetWindowCML(CL_DELETE_NONE_NONE);

            hHnd3 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_LINE);
            hHnd4 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_SUB1);
            hHnd5 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_SUB2);
        }
        /* undersea 2001-08-20 --> @why, what */
#ifdef FEATURE_GVM
        else if (sSmsKindType == GVM_DL_SMS) {
            hHnd3 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_LINE);
            hHnd4 = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_DOWNLOAD1);
            hHnd5 = EQC_StaticText(EQS_NA, EQS_NA, TKN_GVM_DOWNLOAD2);
        }
#endif //FEATURE_GVM
        /* <-- */
#ifdef FEATURE_SKVM
		else if (sSmsKindType == SKVM_LOAD_SMS){
            hHnd3 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_WAP_LINE);
            hHnd4 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SKVM_DOWNLOAD1);
            hHnd5 = EQC_StaticText(EQS_NA, EQS_NA, TKN_SKVM_DOWNLOAD2);
		}
#endif //FEATURE_SKVM
        else
        {
            hHnd3 = NULL;
            hHnd4 = NULL;
            hHnd5 = NULL;
        }

        EQC_SetStaticText(EQS_GetNthIOC(SS_TIME), szTimeBuf);

        // Screen Formatting
        EQS_StartFormatter(); 

        for(nCtrl = 0; nCtrl < SS_MAX; nCtrl++)
        {
            if( (nCtrl == SS_TIME)
                ||(nCtrl == SS_MESSAGE) )

            {
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
                EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= (R_SCROLLBAR_SIZE)/2;
            }
            else
                EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
                    ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

            if( nCtrl == SS_MESSAGE )
                EQC_SET_FOCUS(EQS_GetNthIOC(SS_MESSAGE), FALSE);

            EQS_NewLine();
        }

        if(hHnd1 != NULL)
        {
            EQS_SetControlPosition(hHnd1, 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

            EQS_NewLine();
        }

        if(hHnd2 != NULL)
        {
            EQC_SET_FOCUS(hHnd2, FALSE);

            EQS_SetControlPosition(hHnd2, 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);

            EQC_GET_X(hHnd2) -= (R_SCROLLBAR_SIZE)/2;
/*
            EQS_SetControlPosition(hHnd2, 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
*/
            EQS_NewLine();
        }

        if(hHnd3 != NULL)
        {

            EQS_SetControlPosition(hHnd3, 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQC_GET_X(hHnd3) -= (R_SCROLLBAR_SIZE)/2;
/*
            EQS_SetControlPosition(hHnd3, 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
*/
            EQS_NewLine();        
        }

        if(hHnd4 != NULL)
        {

            EQS_SetControlPosition(hHnd4, 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQC_GET_X(hHnd4) -= (R_SCROLLBAR_SIZE)/2;
/*
            EQS_SetControlPosition(hHnd4, 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
*/
            EQS_NewLine(); 
        }

        if(hHnd5 != NULL)
        {

            EQS_SetControlPosition(hHnd5, 
                ES_AUTO, 0, EHA_CENTER, EVA_CENTER, FALSE);
            EQC_GET_X(hHnd5) -= (R_SCROLLBAR_SIZE)/2;
/*
            EQS_SetControlPosition(hHnd5, 
                ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);
*/
            EQS_NewLine(); 
        }


        EQS_EndFormatter();

        EQC_SET_STYLE( EQS_GetNthIOC(SS_MESSAGE), CS_TEXT_EDIT_DEFAULT | CS_TEXT_EDIT_READONLY );

 		EQS_StartTimer(EQS_TIMER_1, 1*1000, TRUE);
       
        EQS_FitCanvasSize();
        break;

    case SE_APP_END :     /* The very last event */
        EQS_StopTimer(EQS_TIMER_1);
        /* undersea 2001-08-20 --> @why, what */
        g_fDownloadRequestFromSMS = FALSE;
        /* <-- */
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */

        break;

    case SE_TM_TIMER1:
        EQS_SendEvent((EQS_EVENT_ID)SE_KEY_DOWN, EQS_DRAW_LEVEL_APP, EQS_NA);
        break;

    case SE_KEY_UP:
        EQS_StopTimer(EQS_TIMER_1);
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_KEY_SEND :
        if( (sSmsKindType == NORMAL_SMS) 
            || (sSmsKindType == URGENT_SMS)
            || (sSmsKindType == EMERGENCY_SMS) )
        {
            if( nCallBackLength > 0 )
            {
                SET_NUMEDIT_BUFFER(szCallBackBuf, nCallBackLength);
                SET_NUMEDIT_POS(nCallBackLength);                
        
                EQS_START_SIBLING(SKA_StartOrigination, FALSE);
            }
        }
#ifdef FEATURE_SKVM
		else if(sSmsKindType == SKVM_LOAD_SMS)
		{
			EQS_SetDirectDraw(TRUE);
			if (!SKVM_IS_PLAY_BG())
				EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_APP_START, EQS_NA, INIT_SKVM_MSDURL, FALSE);             
			else EQS_START_SIBLING_EVENT(SKA_MainSKVMBG, SE_APP_START, EQS_NA, INIT_SKVM_MSDURL, FALSE);             
		}
#endif
        /* undersea 2001-08-20 --> SMS로 다운로드 받기 */
#ifdef FEATURE_GVM
        else if (sSmsKindType == GVM_DL_SMS) {
            EQS_START_SIBLING(SKA_MainGVM, FALSE);
        }
#endif // FEATURE_GVM
        /* <-- */

#ifdef FEATURE_WAP
        if(SKY_IS_WAP_LOCK())
        {
#ifdef FEATURE_SKTT_PM
            if( (sSmsKindType == WAP_PUSH_SMS)||(sSmsKindType == WAP_PUSH_PHOTOMAIL) )
#else            
            if(sSmsKindType == WAP_PUSH_SMS)
#endif
                EQS_START_SIBLING_EVENT(SKA_MainNtopCode, SE_APP_START, 
                                    INITURL_IS_WAPPUSH,EQS_NA, FALSE);
            else if(sSmsKindType == WAP_MAIL_SMS)
                EQS_START_SIBLING_EVENT(SKA_MainNtopCode, SE_APP_START,
                                INITURL_IS_WAPMAIL ,EQS_NA, FALSE);

        }
        else 
        {
#ifdef FEATURE_SKTT_PM
            if( (sSmsKindType == WAP_PUSH_SMS)||(sSmsKindType == WAP_PUSH_PHOTOMAIL) )
#else
            if(sSmsKindType == WAP_PUSH_SMS)
#endif
            {
                EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                        EQS_NA, INITURL_IS_WAPPUSH, FALSE);     
            }
            else if(sSmsKindType == WAP_MAIL_SMS)
            {            
                EQS_START_SIBLING_EVENT(SKA_MainWAP, SE_APP_START,
                                        EQS_NA, INITURL_IS_WAPMAIL, FALSE);             
            }
        }
#endif  // FEATURE_WAP
        break;

    case SE_SOFTKEY_DELETE:
        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_SOFTKEY_MENU:
        EQS_StopTimer(EQS_TIMER_1);
        hPopup = EQC_PopUpMenu((EQS_TOKEN_EX*)paSMSRcvMsgMenuItem, 2, 0);
        EQC_SET_STYLE(hPopup, EQC_GET_STYLE(hPopup) | CS_POPUP_RETURN_TOKEN );
        break;      

    case SE_SOFTKEY_REPLY:
        FillSMSMOSendMsg(szCallBackBuf, paMsgViewBuf, MO_SEND_REPLY);
        EQS_START_CHILD(SKA_SMSEditMessage);
        break;

    case SE_SOFTKEY_FWD:
        FillSMSMOSendMsg(NULL, paMsgViewBuf, MO_SEND_FORWARD);
        EQS_START_CHILD(SKA_SMSEditMessage);
        break;

    case SE_IO_POPUP_SELECT:    
        switch(pEvent->dwParam)
        {
        case TKN_SMS_TEXT_DELETE: //삭제 
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01), 
                    MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

        case TKN_SMS_SAVE_MSG:    //문장저장
            if( g_nMOSaveSize == MAX_SMS_MO )
            {
                hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_NOT_SAVE_IN_SAVEBOX),
                                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);    
                EQC_SET_USERDATA(hMsg, SMS_VIEW_SAVE);
            }
            else
            {
                memset(&g_UIMOBuffer, NULL, sizeof(SMSMO_SEND_T));    
                STRCPY(g_UIMOBuffer.pszMsg, paMsgViewBuf);
                g_UIMOBuffer.nMsgSize = STRLEN(g_UIMOBuffer.pszMsg);            
                SaveSMSMOSaveMsg(&g_UIMOBuffer);
                EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_SAVE_SAVE_BOX),
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            }
            break;
        }
        break;

    case SE_IO_MESGBOX_YES:
        g_DeleteTextMessage();
        hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS); 
        EQC_SET_USERDATA(hMsg, SMS_VIEW_DELETE);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SMS_VIEW_DELETE)
            EQS_END();
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_KEY_CLEAR:
        EQS_END();
        break;

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL INT8 sTextCount( )
{
    INT8    nCount;

    g_DoOldTextMatch ();
    nNewNotUrgentCnt = ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt;
    nCount = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt +
             ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt;

    return nCount;
}

BOOL    fIsThereText()
{

    INT8     sTextCount;

    sTextCount = gReadTextMessage ();

    if (sTextCount > 0)
        return TRUE;
    else
        return FALSE;

}

LOCAL VOID SKA_TextMessage(SKY_EVENT_T* pEvent)
{
    STATIC  INT8        nIndex, sMenuSize, sWindowSize, sNewTextCount, nPos;
    STATIC  HCONTROL    hHnd;   //1: all delete, 2: each delete

    INT8                nPopIndex;
    HCONTROL            hMsgYes, hMsg;

    SKY_TOKEN paListPopMenuProtect[]=
    {
        TKN_SMS_TEXT_DELETE,
        TKN_SMS_DELETE_ALL,    // 메시지 전부 삭제
        TKN_SMS_TEXT_PROTECT    // 메시지 보호 설정   
    };
    
    SKY_TOKEN paListPopMenuProtectReset[]=
    {
        TKN_SMS_TEXT_DELETE,
        TKN_SMS_DELETE_ALL,
        TKN_SMS_TEXT_PROTECT_RESET  //메시지 보호 해제 
    };              

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
//                      CL_MENU_NONE_NONE, 
                      CL_MENU_CONFIRM_NONE,                        
                      SKY_GET_TOKEN(TKN_SMS_SUB_MENU1), 
                      WA_SHOW_CML | WA_SHOW_HEADER);
        
        sNewTextCount = sTextCount();
        sMenuSize = gReadTextMessage ();
        if( sMenuSize > 0 )
        {
		    gMakeMsgList( ); // pViewBuf );

            sWindowSize = ( sMenuSize > 6 ) ? 6 : sMenuSize;
            hHnd = EQC_ScrollList(5, 5, LCD_COLUME_SIZE, sWindowSize, paMsgListBuf, sMenuSize, sMenuSize, 0); 
            EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
                                 CS_SCROLL_LIST_ENUMBERATED |
                                 CS_SCROLL_LIST_MAXIMIZE );

            EQS_SetIOCActive( hHnd );
        }
        else
        {
            EQS_END();
        }

        break;

    case SE_APP_END :     /* The very last event */
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */

        EQS_DeleteControl(hHnd);
    
        sNewTextCount = sTextCount();
        sMenuSize = gReadTextMessage ();
        if ( nPos >= sMenuSize )
            nPos = sMenuSize-1;

        if( sMenuSize > 0 )
        {
		    gMakeMsgList( ); // pViewBuf );

            sWindowSize = ( sMenuSize > 6 ) ? 6 : sMenuSize;
            hHnd = EQC_ScrollList(5, 5, 18, sWindowSize, paMsgListBuf, sMenuSize, sMenuSize, nPos); 
            EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
                           CS_SCROLL_LIST_ENUMBERATED | CS_SCROLL_LIST_MAXIMIZE );

            EQS_SetIOCActive( hHnd );
        }
        else
        {
            EQS_END();
        }
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    
    case SE_SOFTKEY_MENU:
        nIndex = EQC_GetScrollPhysicalIndex(hHnd);
        if( (INT8)nIndex < sMenuSize )
        {
            if ( (INT8)nIndex < sNewTextCount )
            {
                if( (INT8)nIndex < ( sNewTextCount - nNewNotUrgentCnt ) )
                {
                    nListType = UI_SMS_TXT_NEW_URGENT;
                    nListIdx = sNewTextCount - nNewNotUrgentCnt - ( (INT8)nIndex + 1 );
                }
                else
                {
                    nListType = UI_SMS_TXT_NEW_NOT_URGENT;
                    nListIdx = sNewTextCount - ( (INT8)nIndex + 1 );
                }
            }
            else
            {
                nListType = UI_SMS_TXT_OLD;
                nListIdx = sMenuSize - ( (INT8)nIndex + 1 );
            }
            
        }
        if(IsMsgProtect())
        {            
            EQC_PopUpMenu((EQS_TOKEN_EX*)paListPopMenuProtectReset, 3, 0);
        }
        else
        {
            EQC_PopUpMenu((EQS_TOKEN_EX*)paListPopMenuProtect, 3, 0);        
        }
        break;

    case SE_IO_SCROLL_LIST_CHG:
        nPos = (INT8)pEvent->dwParam;
        break;

    case SE_SOFTKEY_SELECT:
    case SE_IO_SCROLL_LIST_SELECT:
        nPos = (INT8)pEvent->dwParam;
        if( (INT8)pEvent->dwParam < sMenuSize )
        {
            if ( (INT8)pEvent->dwParam < sNewTextCount )
            {
                if( (INT8)pEvent->dwParam < ( sNewTextCount - nNewNotUrgentCnt ) )
                {
                    nListType = UI_SMS_TXT_NEW_URGENT;
                    nListIdx = sNewTextCount - nNewNotUrgentCnt - ( (INT8)pEvent->dwParam + 1 );
                }
                else
                {
                    nListType = UI_SMS_TXT_NEW_NOT_URGENT;
                    nListIdx = sNewTextCount - ( (INT8)pEvent->dwParam + 1 );
                }
            }
            else
            {
                nListType = UI_SMS_TXT_OLD;
                nListIdx = sMenuSize - ( (INT8)pEvent->dwParam + 1 );
            }
            EQS_START_CHILD( SKA_ViewTextMessage );
        }
        else
        {
            EQS_SetIOCActive(hHnd);
        }
        break;

    case SE_SMS_MESSAGE:
        EQS_DeleteControl(hHnd);
        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        
        sNewTextCount = sTextCount();
        sMenuSize = gReadTextMessage ();

        if( sMenuSize > 0 )
        {
		    gMakeMsgList( ); 

            sWindowSize = ( sMenuSize > 6 ) ? 6 : sMenuSize;
            hHnd = EQC_ScrollList(5, 5, 18, sWindowSize, paMsgListBuf, sMenuSize, sMenuSize, 0); 
            EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
                                 CS_SCROLL_LIST_ENUMBERATED |
                                 CS_SCROLL_LIST_MAXIMIZE );

            EQS_SetIOCActive( hHnd );
        }
        else
        {
            EQS_END();
        }
        break;

    case SE_IO_POPUP_SELECT:
        nPopIndex = pEvent->dwParam;     // dwParam == 0 --- 삭제 1 --- 보호/해제 
        nIndex = EQC_GetScrollPhysicalIndex(hHnd);

        switch( nPopIndex )
        {
        case 0:
            nIndex = EQC_GetScrollPhysicalIndex(hHnd);
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_01), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ORDER);
            break;

        case 1:  // 삭제 
            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_11), 
                MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

            EQC_SET_USERDATA(hMsg, SMS_MESG_DELETE_ALL_ORDER);
            break;

        case 2:  // 보호/해제 
            if( (INT8)nIndex < sMenuSize )
            {
                if ( (INT8)nIndex < sNewTextCount )
                {
                    if( (INT8)nIndex < ( sNewTextCount - nNewNotUrgentCnt ) )
                    {
                        nListType = UI_SMS_TXT_NEW_URGENT;
                        nListIdx = sNewTextCount - nNewNotUrgentCnt - ( (INT8)nIndex + 1 );
                    }
                    else
                    {
                        nListType = UI_SMS_TXT_NEW_NOT_URGENT;
                        nListIdx = sNewTextCount - ( (INT8)nIndex + 1 );
                    }
                }
                else
                {
                    nListType = UI_SMS_TXT_OLD;
                    nListIdx = sMenuSize - ( (INT8)nIndex + 1 );
                }
                gMakeProtect( ); // Make Protect
		        gMakeMsgList( ); 

                EQS_SEND_EVENT(SE_APP_DRAW, (WORD)hHnd, EQS_NA);

                EQS_SetIOCActive(hHnd); //  IMSI
            }
            break;
        }
        break;

    case SE_IO_MESGBOX_END:
        if (pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            EQS_DeleteControl(hHnd);
        
            sNewTextCount = sTextCount();
            sMenuSize = gReadTextMessage ();

            if ( nPos >= sMenuSize )
                nPos = sMenuSize-1;

            if( sMenuSize > 0 )
            {
		        gMakeMsgList( ); // pViewBuf );

                sWindowSize = ( sMenuSize > 6 ) ? 6 : sMenuSize;
                hHnd = EQC_ScrollList(5, 5, 18, sWindowSize, paMsgListBuf, sMenuSize, sMenuSize, nPos); 
                EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
                                     CS_SCROLL_LIST_ENUMBERATED |
                                     CS_SCROLL_LIST_MAXIMIZE );

                EQS_SetIOCActive( hHnd );
            }
            else
            {
                EQS_END();
            }
        }
        else if (pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            EQS_END();        
        }
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SMS_MESG_DELETE_ORDER)
        {
            if( (INT8)nIndex < sMenuSize )
            {
                if ( (INT8)nIndex < sNewTextCount )
                {
                    if( (INT8)nIndex < ( sNewTextCount - nNewNotUrgentCnt ) )
                    {
                        nListType = UI_SMS_TXT_NEW_URGENT;
                        nListIdx = sNewTextCount - nNewNotUrgentCnt - ( (INT8)nIndex + 1 );
                    }
                    else
                    {
                        nListType = UI_SMS_TXT_NEW_NOT_URGENT;
                        nListIdx = sNewTextCount - ( (INT8)nIndex + 1 );
                    }
                }
                else
                {
                    nListType = UI_SMS_TXT_OLD;
                    nListIdx = sMenuSize - ( (INT8)nIndex + 1 );
                }
                g_DeleteTextMessage();
            }
            hMsgYes = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_02), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);  
            EQC_SET_USERDATA(hMsgYes, SMS_MESG_DELETE_ORDER);
        }
        else if(pEvent->dwParam == SMS_MESG_DELETE_ALL_ORDER)
        {
            gDeleteMsg();
            ui_sms_update_icons ();

            hMsgYes = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_13), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsgYes, SMS_MESG_DELETE_ALL_ORDER);
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam == EQC_DELETED)
        {
            break;  //scroll list가 delete될때 SE_IO_SCROLL_LIST_CANCEL 발생 
        }            
        else
        {
            EQS_END();
        }
        break;
    
    case SE_KEY_CLEAR:
        EQS_END();
        break;

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

BOOL IsSmsNewMsg()
{
    INT8        bTextCount, bVoiceCount, bMybellCount;

    bTextCount = sTextCount();
    bVoiceCount = g_VoiceCount();
    bMybellCount = g_MybellCount();

    if(bTextCount == 0 && bVoiceCount == 0 && bMybellCount == 0)
        return FALSE;
    else
        return TRUE;
}

INT8 g_NewMsgCount( )
{
    INT8        bTextCount, bVoiceCount, bMybellCount;

    bTextCount = sTextCount();
    bVoiceCount = g_VoiceCount();
    bMybellCount = g_MybellCount();

    return (bTextCount + bVoiceCount + bMybellCount);
}

LOCAL VOID sMakeSmsSubMenu()
{
    INT8        bTextCount, bVoiceCount, bMybellCount;

    bTextCount = sTextCount();
    bVoiceCount = g_VoiceCount();
    bMybellCount = g_MybellCount();


    if ( bTextCount >= 10 )  // Text Message
    {
        sprintf((char*)g_pszText, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE2), SKY_GET_TOKEN(TKN_SMS_SUB_MENU1), bTextCount);
        paSmsSubMenu[0] = g_pszText;
    }
    else if ( bTextCount > 0 )
    {
        sprintf((char*)g_pszText, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE1), SKY_GET_TOKEN(TKN_SMS_SUB_MENU1), bTextCount);
        paSmsSubMenu[0] = g_pszText;
    }
    else
    {
        STRCPY(g_pszText, (BYTE*)SKY_GET_TOKEN(TKN_SMS_SUB_MENU1));
        paSmsSubMenu[0] = g_pszText;
    }

    if ( bVoiceCount >= 10 )  // Voice Message
    {
        sprintf((char*)g_pszVoice, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE2), SKY_GET_TOKEN(TKN_SMS_SUB_MENU2), bVoiceCount);
        paSmsSubMenu[1] = g_pszVoice;
    }
    else if ( bVoiceCount > 0 )
    {
        sprintf((char*)g_pszVoice, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE1), SKY_GET_TOKEN(TKN_SMS_SUB_MENU2), bVoiceCount);
        paSmsSubMenu[1] = g_pszVoice;
    }
    else
    {
        STRCPY(g_pszVoice, (BYTE *)SKY_GET_TOKEN(TKN_SMS_SUB_MENU2));
        paSmsSubMenu[1] = g_pszVoice;
    }

    if ( bMybellCount >= 10 )  // Mybell Message
    {
        sprintf((char*)g_pszMybell, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE2), SKY_GET_TOKEN(TKN_SMS_SUB_MENU3), bMybellCount);
        paSmsSubMenu[2] = g_pszMybell;
    }
    else if ( bMybellCount > 0 )
    {
        sprintf((char*)g_pszMybell, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE1), SKY_GET_TOKEN(TKN_SMS_SUB_MENU3), bMybellCount);
        paSmsSubMenu[2] = g_pszMybell;
    }
    else
    {
        STRCPY(g_pszMybell, (BYTE*)SKY_GET_TOKEN(TKN_SMS_SUB_MENU3)); 
        paSmsSubMenu[2] = g_pszMybell;
    }
}

// Receive Message Menu
VOID SKA_ReceiveMsg(SKY_EVENT_T* pEvent)
{
    static HCONTROL s_hHnd;
    static INT8 bSelectMenu;
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, 
            WA_SHOW_CML );
        
        g_pszText = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        g_pszVoice = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        g_pszMybell = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        bSelectMenu = 0;

        sMakeSmsSubMenu();

        s_hHnd = EQC_ScrollList(5, 10, 18, 4, paSmsSubMenu, 3, 3, 0); 
        EQC_SET_STYLE( s_hHnd, (EQC_GET_STYLE(s_hHnd) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE) & ~CS_SCROLL_LIST_SCROLL_BAR );                  

        EQC_SetScrollTitle( s_hHnd, (BYTE*)SKY_GET_TOKEN(TKN_SMS_IN_BOX));
        EQC_ShowScrollTitle( s_hHnd, TRUE );

        EQS_SetIOCActive( s_hHnd );
        break;

    case SE_APP_RESUME:    /* end child */
        sMakeSmsSubMenu();
        EQS_SEND_EVENT(SE_APP_DRAW, s_hHnd, EQS_NA);
        EQS_SetIOCActive( s_hHnd );
        break;


    case SE_APP_END :     /* The very last event */
	    SKY_Free(g_pszText);
	    SKY_Free(g_pszVoice);
	    SKY_Free(g_pszMybell);
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_IO_MESGBOX_END:
        EQS_SetIOCActive( s_hHnd );
        break;
      
    case SE_IO_SCROLL_LIST_SELECT: 
        bSelectMenu = pEvent->dwParam;
        switch( bSelectMenu )
        {
        case 0 :
            if( fIsThereText() )
                EQS_START_CHILD( SKA_TextMessage );
            else
                EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_03), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

            break;
        case 1 :
            if( gfIsThereVms() )
                EQS_START_CHILD( SKA_VoiceMessage );
            else
                EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_VMS_MESSAGE_01), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);

            break;
        case 2:
            if( gfIsThereMB() )
                EQS_START_CHILD( SKA_MybellMessage );
            else
                EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_MYB_MESSAGE_01), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            break;

        default:
            EQS_SetIOCActive(s_hHnd);
            break;
        }

        break;

    case SE_SMS_MYBELL:
    case SE_SMS_MESSAGE:
        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());  
        sMakeSmsSubMenu();
        EQS_SEND_EVENT(SE_APP_DRAW, s_hHnd, EQS_NA);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->dwParam == EQC_DELETED)
            break;
    
    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
        EQS_END();
        break;

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL VOID sDeleteRcvMsg()
{
    // 문자메시지 
    gDeleteMsg();
}

LOCAL VOID sDeleteVMSMsg()
{
    gDeleteAllVms();
}
LOCAL VOID sDeleteMybellMsg()
{
    gDeleteAllMybell();
}        
LOCAL VOID sDeleteSendMsg()
{
    DeleteAllSendMsg();
}

LOCAL VOID sDeleteSentMsg()
{
    DeleteAllSentMsg();
}
LOCAL VOID sDeleteSaveMsg()
{
    DeleteAllSaveMsg();
}

LOCAL VOID sDeleteAllMsg()
{
    sDeleteRcvMsg();

    sDeleteVMSMsg();

    sDeleteMybellMsg();

    sDeleteSendMsg(); // 보낼메시지

    sDeleteSentMsg(); // 보낸메시지

    sDeleteSaveMsg();
}

VOID SKY_API SKY_InitSmsData(void) 
{
    // Mybell
    gDeleteAllMybell();

    sDeleteAllMsg();
}

// 받은메시지 삭제
LOCAL VOID SKA_DeleteRcvMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_RECEIVE), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_11), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    
            
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        sDeleteRcvMsg();
        ui_sms_update_icons ();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_13), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
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

LOCAL VOID SKA_DeleteVMSMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_RECEIVE), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_17), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    
            
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        sDeleteVMSMsg();
        ui_sms_update_icons ();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_19), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
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
LOCAL VOID SKA_DeleteMybellMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_RECEIVE), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_18), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    
            
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        sDeleteMybellMsg();
        ui_sms_update_icons ();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_20), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
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

LOCAL VOID SKA_DeleteSaveMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_RECEIVE), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_12), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    
            
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_YES:
        sDeleteSaveMsg();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_16), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);       
        
        break;

    case SE_IO_MESGBOX_NO:
        EQS_END();
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

// 보낼메시지를 모두 삭제
LOCAL VOID SKA_DeleteSendMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_SEND), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_06), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;

    case EQE_IO_MESGBOX_YES:
        sDeleteSendMsg();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_05), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case EQE_IO_MESGBOX_NO:
        EQS_END();
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

// 보낸메시지를 모두 삭제
LOCAL VOID SKA_DeleteSentMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_SENT), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_07), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    
            
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
            EQS_END();
        break;

    case EQE_IO_MESGBOX_YES:
        sDeleteSentMsg();
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_05), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        
        break;

    case EQE_IO_MESGBOX_NO:
        EQS_END();
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

// 모든메시지를 삭제
LOCAL VOID SKA_DeleteAllMessage(SKY_EVENT_T *pEvent)
{
    STATIC HCONTROL s_hMsgControl, s_hMsgControl2;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);

        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                150, //SKY_DEF_WIN_DY, 
                CL_NONE, 
                SKY_GET_TOKEN(TKN_SMS_DELETE_ALL), 
                WA_SHOW_HEADER);

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_08), 
            MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

        break;
        
    case SE_APP_END :   /* The very last event */ 
         EQS_ReleaseDC();
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == s_hMsgControl)
        {
            sDeleteAllMsg();
            ui_sms_update_icons ();
            s_hMsgControl2 = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_05), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        }
        else if(pEvent->wParam == s_hMsgControl2)
        {
            EQS_END();
        }
        break;

    case SE_IO_MESGBOX_CLEAR:
        EQS_END();
        break;    

    case EQE_IO_MESGBOX_YES:
        s_hMsgControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_09), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);        
        break;

    case EQE_IO_MESGBOX_NO:
        EQS_END();
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


LOCAL VOID sMakeDeleteSubMenu()
{
    sprintf((char*)pszDelete1, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_RECEIVE));
    paSmsDeleteMenu[0] = pszDelete1;

    sprintf((char*)pszDelete2, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_VMS));
    paSmsDeleteMenu[1] = pszDelete2;

    sprintf((char*)pszDelete3, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_MYBELL));
    paSmsDeleteMenu[2] = pszDelete3;

    sprintf((char*)pszDelete4, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_SEND));
    paSmsDeleteMenu[3] = pszDelete4;

    sprintf((char*)pszDelete5, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_SENT));
    paSmsDeleteMenu[4] = pszDelete5;

    sprintf((char*)pszDelete6, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_SAVE));
    paSmsDeleteMenu[5] = pszDelete6;

    sprintf((char*)pszDelete7, (const char*)SKY_GET_TOKEN(TKN_SMS_DELETE_ALL_MSG));
    paSmsDeleteMenu[6] = pszDelete7;
}

// All Message Data Delete Menu
VOID SKY_API SKA_DeleteAllMsg(SKY_EVENT_T* pEvent)
{
    static HCONTROL hHnd;
    static INT8 bSelectMenu;

    HCONTROL    hMsgHnd;
    UINT8       nCount = 0;
    
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, 
            WA_SHOW_CML);

		pszDelete1 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
		pszDelete2 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
		pszDelete3 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
		pszDelete4 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        pszDelete5 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        pszDelete6 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        pszDelete7 = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);

		sMakeDeleteSubMenu();

        hHnd = EQC_ScrollList(5, 10, 18, 5, paSmsDeleteMenu, 7, 7, 0); 
        EQC_SET_STYLE( hHnd, EQC_GET_STYLE(hHnd) |
                             CS_SCROLL_LIST_ENUMBERATED |
                             CS_SCROLL_LIST_MAXIMIZE );
        EQC_SetScrollTitle( hHnd, (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU5));
        EQC_ShowScrollTitle(hHnd, TRUE );

        EQS_SetIOCActive( hHnd );
        break;

    case SE_APP_RESUME:    /* end child */
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        break;


    case SE_APP_END :     /* The very last event */
		SKY_Free(pszDelete1);
		SKY_Free(pszDelete2);
		SKY_Free(pszDelete3);
		SKY_Free(pszDelete4);
        SKY_Free(pszDelete5);
        SKY_Free(pszDelete6);
        SKY_Free(pszDelete7);
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_IO_SCROLL_LIST_SELECT: 
        bSelectMenu = pEvent->dwParam;
        switch( bSelectMenu )
        {
        case 0 :
            nCount = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt +
                        ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt +
                        ui_sms_list [UI_SMS_TXT_OLD].cnt;
            if(nCount > 0)
            {                
                EQS_START_CHILD( SKA_DeleteRcvMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);
            }
            break;

        case 1:
            nCount = g_VmsSize;
            if(nCount > 0)
            {
                EQS_START_CHILD( SKA_DeleteVMSMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);            
            }
            break;

        case 2:
            nCount = g_MyBellSize;
            if(nCount > 0)
            {
                EQS_START_CHILD( SKA_DeleteMybellMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);                       
            }
            break;

        case 3:            
            GetMOSendSize(&g_nMOSendSize);
            nCount = g_nMOSendSize;
            if(nCount > 0)
            {
                EQS_START_CHILD( SKA_DeleteSendMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);                 
            }
            break;
        case 4:
            GetMOSentSize(&g_nMOSentSize);
            nCount = g_nMOSentSize;
            if(nCount > 0)
            {
                EQS_START_CHILD( SKA_DeleteSentMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);                             
            }
            break;
        case 5:
            GetMOSaveSize(&g_nMOSaveSize);
            nCount = g_nMOSaveSize;
            if(nCount > 0)                
            {
                EQS_START_CHILD( SKA_DeleteSaveMessage );            
            }
            else
            {                
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_22),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);                          
            }
            break;

        case 6:
            GetMOSendSize(&g_nMOSendSize);
            GetMOSentSize(&g_nMOSentSize);
            GetMOSaveSize(&g_nMOSaveSize);
            
            nCount = ui_sms_list[UI_SMS_TXT_NEW_URGENT].cnt +
                        ui_sms_list[UI_SMS_TXT_NEW_NOT_URGENT].cnt +
                        ui_sms_list [UI_SMS_TXT_OLD].cnt +              //수신문자메시지
                        g_VmsSize +  //수신음성메시지
                        g_MyBellSize + //수신마이벨메시지
                        g_nMOSendSize + //보낼메시지
                        g_nMOSentSize + //보낸메시지 
                        g_nMOSaveSize ; //메시지보관함 
            if(nCount > 0)
            {                
                EQS_START_CHILD( SKA_DeleteAllMessage );
            }
            else
            {
                hMsgHnd = EQC_BeepMessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_21),
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hMsgHnd, SMS_DELETE_NO_DATA);                  
                
            }
            break;

        default:
            EQS_SetIOCActive(hHnd);
            break;
        }

        break;
        
    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:    
    case SE_IO_SCROLL_LIST_CANCEL:
        EQS_END();
        break;

    case SE_IO_MESGBOX_END:
/*
        if(pEvent->dwParam == SMS_DELETE_NO_DATA)
        {
            EQS_END();
        }
*/
        EQS_SetIOCActive( hHnd );

        break;

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

/*
LOCAL VOID sMakeSmsMenu()
{
    INT8        bTextCount, bVoiceCount, bMybellCount, bTotalCount;
#ifdef FEATURE_SKTT_EQUS_LOCATION
    LOCATION_INFO_T         location;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
    bTextCount = sTextCount();
    bVoiceCount = g_VoiceCount();
    bMybellCount = g_MybellCount();
    bTotalCount = bTextCount + bVoiceCount + bMybellCount;

    if ( bTotalCount >= 10 )
    {
        sprintf((char*)g_pszReceive, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE2), SKY_GET_TOKEN(TKN_SMS_IN_BOX), bTotalCount);
        paSmsMenu[0] = g_pszReceive;
    }
    else if ( bTotalCount > 0 )
    {
        sprintf((char*)g_pszReceive, (const char*)SKY_GET_TOKEN(TKN_SMS_FMT_RECEIVE1), SKY_GET_TOKEN(TKN_SMS_IN_BOX), bTotalCount);
        paSmsMenu[0] = g_pszReceive;
    }
    else
    {
        STRCPY(g_pszReceive, (BYTE*)SKY_GET_TOKEN(TKN_SMS_IN_BOX));
        paSmsMenu[0] = g_pszReceive;
    }
#ifdef FEATURE_SKTT_EQUS_LOCATION
    location = SKY_GET_LOCATION_INFO();

    if(location.rcvinfo)
    {
        paLocationMenu[0] = paSmsMenu[0];
        paLocationMenu[1] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_SENT_BOX);
        paLocationMenu[2] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU4);
        paLocationMenu[3] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU5);
        paLocationMenu[4] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_TITLE);
#ifdef FEATURE_WAP
        paLocationMenu[5] = (BYTE*)SKY_GET_TOKEN(TKN_NTOP_MAIL_TITLE);
        paLocationMenu[6] = (BYTE*)SKY_GET_TOKEN(TKN_LOCATION_SETTING);
#else //#ifdef FEATURE_WAP
        paLocationMenu[5] = (BYTE*)SKY_GET_TOKEN(TKN_LOCATION_SETTING);
#endif    //FEATURE_WAP    
    }
    else
    {        
        paSmsMenu[1] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_SENT_BOX);
        paSmsMenu[2] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU4);
        paSmsMenu[3] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU5);
        paSmsMenu[4] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_TITLE);
#ifdef FEATURE_WAP
        paSmsMenu[5] = (BYTE*)SKY_GET_TOKEN(TKN_NTOP_MAIL_TITLE);
#endif //#ifdef FEATURE_WAP 
    }
#else
    paSmsMenu[1] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_SENT_BOX);
    paSmsMenu[2] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU4);
    paSmsMenu[3] = (BYTE*)SKY_GET_TOKEN(TKN_SMS_MENU5);
    paSmsMenu[4] = (BYTE*)SKY_GET_TOKEN(TKN_EMAIL_TITLE);
#ifdef FEATURE_WAP
    paSmsMenu[5] = (BYTE*)SKY_GET_TOKEN(TKN_NTOP_MAIL_TITLE);
#endif //#ifdef FEATURE_WAP

#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
}
*/
/*
LOCAL VOID SKA_SMSMenu(SKY_EVENT_T* pEvent)
{
    static HCONTROL	        s_hHnd;
    static UINT8            bSelectMenu;
#ifdef FEATURE_SKTT_EQUS_LOCATION
    LOCATION_INFO_T         location;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION    
    switch(pEvent->EventID)
    {
    case SE_APP_START : 
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE, 
                      SKY_GET_TOKEN(TKN_SMS), 
                      WA_SHOW_CML);

		g_pszReceive = (BYTE*)SKY_Malloc(MAX_RECEIVE_STR_LENGTH);
        
		sMakeSmsMenu();
        bSelectMenu = 0;
#ifdef FEATURE_SKTT_EQUS_LOCATION
        location = SKY_GET_LOCATION_INFO();

        if(location.rcvinfo)
        {
#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 7, 7, 0);             
#else //#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 6, 6, 0);             
#endif
        }
        else
        {
#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 6, 6, 0);             
#else //#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 5, 5, 0);             
#endif
        }
#else //#ifdef FEATURE_SKTT_EQUS_LOCATION

#ifdef WAPPER
        s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 6, 6, 0); 
#else //#ifdef WAPPER
        s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 5, 5, 0); 
#endif 

#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

        // jrkwon 2001.08.04 --> remove scroll bar( | ~CS_SCROLL_LIST_SCROLL_BAR) 
        // huni 2001.08.09 --> restore scroll bar 
        EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                            CS_SCROLL_LIST_ENUMBERATED |
                            CS_SCROLL_LIST_MAXIMIZE );
//                            CS_SCROLL_LIST_MAXIMIZE | ~CS_SCROLL_LIST_SCROLL_BAR);
       
#ifdef FEATURE_SKTT_EQUS_LOCATION
        if(location.rcvinfo)
        {
#ifdef WAPPER
            EQC_SetScrollAccelKey(s_hHnd, 6, ACCEL_KEY_STAR);
#else
            EQC_SetScrollAccelKey(s_hHnd, 5, ACCEL_KEY_STAR);
#endif
        }
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
        EQC_SetScrollTitle( s_hHnd, (BYTE*)SKY_GET_TOKEN(TKN_SMS_TITLE));
        EQC_ShowScrollTitle(s_hHnd, TRUE );

        EQS_SetIOCActive( s_hHnd );
        break;

    case SE_APP_END :     // The very last event 
		SKY_Free(g_pszReceive);
//        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : // start child 
        break;

    case SE_APP_RESUME:    // end child 
#ifdef FEATURE_SKTT_EQUS_LOCATION
        bSelectMenu = EQC_GetScrollCursor(s_hHnd);        
        
        EQS_DeleteControl(s_hHnd);
        
        memset(g_pszReceive, NULL, MAX_RECEIVE_STR_LENGTH);

		sMakeSmsMenu();
        location = SKY_GET_LOCATION_INFO();

        if(location.rcvinfo)
        {
#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 7, 7, bSelectMenu);             
#else //#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 6, 6, bSelectMenu);             
#endif
        }
        else
        {          
#ifdef WAPPER
            if(bSelectMenu > 5) 
            {
                bSelectMenu = 0;
            }
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 6, 6, bSelectMenu);             
#else //#ifdef WAPPER
            if(bSelectMenu > 4)
            {
                bSelectMenu = 0;
            }
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 5, 5, bSelectMenu);             
#endif
        }

        EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                            CS_SCROLL_LIST_ENUMBERATED |
                            CS_SCROLL_LIST_MAXIMIZE );
        if(location.rcvinfo)
        {
#ifdef WAPPER
            EQC_SetScrollAccelKey(s_hHnd, 6, ACCEL_KEY_STAR);
#else
            EQC_SetScrollAccelKey(s_hHnd, 5, ACCEL_KEY_STAR);
#endif
        }
        EQC_SetScrollTitle( s_hHnd, (BYTE*)SKY_GET_TOKEN(TKN_SMS_TITLE));
        EQC_ShowScrollTitle(s_hHnd, TRUE );

        EQS_SetIOCActive( s_hHnd );
#else   //FEATURE_SKTT_EQUS_LOCATION
        sMakeSmsMenu();         // 내용 update
        EQS_SEND_EVENT(SE_APP_DRAW, s_hHnd, EQS_NA);
        EQS_SetIOCActive(s_hHnd);
#endif //FEATURE_SKTT_EQUS_LOCATION
        break;

    case SE_APP_DRAW :    // draw 
        break;

#ifdef FEATURE_SKTT_EQUS_LOCATION
    case SE_LOCATION_INFORM:
        bSelectMenu = EQC_GetScrollCursor(s_hHnd);        
        EQS_DeleteControl(s_hHnd);

        memset(g_pszReceive, NULL, MAX_RECEIVE_STR_LENGTH);

		sMakeSmsMenu();

        location = SKY_GET_LOCATION_INFO();

        if(location.rcvinfo)
        {
#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 7, 7, bSelectMenu);             
#else //#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paLocationMenu, 6, 6, bSelectMenu);             
#endif
        }
        else
        {
#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 6, 6, bSelectMenu);             
#else //#ifdef WAPPER
            s_hHnd = EQC_ScrollList(5, 10, 18, EQC_GetScrollDisplayLine(), paSmsMenu, 5, 5, bSelectMenu);             
#endif
        }

        EQC_SET_STYLE( s_hHnd, EQC_GET_STYLE(s_hHnd) |
                            CS_SCROLL_LIST_ENUMBERATED |
                            CS_SCROLL_LIST_MAXIMIZE );
        if(location.rcvinfo)
        {
#ifdef WAPPER
            EQC_SetScrollAccelKey(s_hHnd, 6, ACCEL_KEY_STAR);
#else
            EQC_SetScrollAccelKey(s_hHnd, 5, ACCEL_KEY_STAR);
#endif
        }
        EQC_SetScrollTitle( s_hHnd, (BYTE*)SKY_GET_TOKEN(TKN_SMS_TITLE));
        EQC_ShowScrollTitle(s_hHnd, TRUE );

        EQS_SetIOCActive( s_hHnd );
        EQS_PASS_CURRENT_EVENT();
        break;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

    case SE_IO_SCROLL_LIST_SELECT:
        bSelectMenu = pEvent->dwParam;

#ifdef FEATURE_SKTT_EQUS_LOCATION
        location = SKY_GET_LOCATION_INFO();
        if(location.rcvinfo)
        {
            switch( bSelectMenu )
            {
            case 0 :
                EQS_START_CHILD( SKA_ReceiveMsg );
                break;
            case 1 :
                EQS_START_CHILD( SKA_SendMsg );
                break;
            case 2:
                EQS_START_CHILD( SKA_MsgAlarm );
                break;
            case 3:
                EQS_START_CHILD( SKA_DeleteAllMsgCode );
                break;
            case 4:
                EQS_START_CHILD( SKA_EnterEmailMenu );
                break;
#ifdef WAPPER
            case 5:
                EQS_START_CHILD( SKA_SendWAPMsg );
                break;

            case 6:
                EQS_START_CHILD( SKA_LocationCode );
                break;
#else //#ifdef WAPPER
            case 5:
                EQS_START_CHILD( SKA_LocationCode );
                break;
#endif
            default:
                EQS_SetIOCActive(s_hHnd);
                break;
            }            
        }
        else
        {
            switch( bSelectMenu )
            {
            case 0 :
                EQS_START_CHILD( SKA_ReceiveMsg );
                break;
            case 1 :
                EQS_START_CHILD( SKA_SendMsg );
                break;
            case 2:
                EQS_START_CHILD( SKA_MsgAlarm );
                break;

            case 3:
                EQS_START_CHILD( SKA_DeleteAllMsgCode );
                break;

            case 4:
                EQS_START_CHILD( SKA_EnterEmailMenu );
                break;
#ifdef WAPPER
            case 5:
                EQS_START_CHILD( SKA_SendWAPMsg );
                break;
#endif
            default:
                EQS_SetIOCActive(s_hHnd);
                break;
            }         
        }
#else
        switch( bSelectMenu )
        {
        case 0 :
            EQS_START_CHILD( SKA_ReceiveMsg );
            break;

        case 1 :
            EQS_START_CHILD( SKA_SendMsg );
            break;

        case 2:
            EQS_START_CHILD( SKA_MsgAlarm );
            break;

        case 3:
            EQS_START_CHILD( SKA_DeleteAllMsgCode );
            break;

        case 4:
            EQS_START_CHILD( SKA_EnterEmailMenu );
            break;

#ifdef WAPPER
        case 5:
            EQS_START_CHILD( SKA_SendWAPMsg );
            break;
#endif  //#ifdef WAPPER

        default:
            EQS_SetIOCActive(s_hHnd);
            break;
        }
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
        break;
    case SE_SMS_MESSAGE:
    case SE_SMS_MYBELL:        
        SKY_DoSMSAlert();
        SKY_SNI(HS_SNI_SMS_RECEIVING, TRUE);
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());          
        sMakeSmsMenu();
        EQS_SEND_EVENT(SE_APP_DRAW, s_hHnd, EQS_NA);
        break;

    case SE_IO_SCROLL_LIST_CANCEL:   // IO Control List Cancel
        if(pEvent->dwParam == EQC_DELETED)
            break;    
    case SE_KEY_CLEAR:               // Clear Key
    case SE_SOFTKEY_CANCEL:          // Message Box ... Cancel
        EQS_END();
        break;

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}
*/
WORD  sGetCurrentSMSBellId(void)
{
    UINT8 nBellIndex;
    WORD  nBellId;

    nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SS_SMS_KIND));
    nBellId = aSMS_Bell_List[nBellIndex].wBellId;

    return nBellId;
}

UINT8 sFindSmsBellIndex(WORD wBellId)
{
    UINT8   index;

    for(index=0; index < NUM_SMS_KIND; index ++)
    {
        if(aSMS_Bell_List[index].wBellId == wBellId)
            return index;
    }

    return 0;
}

void sMsgAlarmControls(void)
{
    STATIC  UINT8   nBellIdx;

    UINT8   nCtrl, index;
    HCONTROL        hProgBar;

    for( index=0; index < NUM_SMS_MODE; index++ )
        ppSMSMode[index] = (BYTE *)SKY_GET_TOKEN(aSmsAlarmMode[index]);

    for( index=0; index < NUM_SMS_KIND; index++ )
        ppSMSKind[index] = (BYTE *)SKY_GET_TOKEN(aSMS_Bell_List[index].BellName);

    // 알람설정
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_ALARM_SET);

    EQC_SET_STYLE( EQC_ScrollCombo(EQS_NA, EQS_NA, LCD_COLUME_SIZE -1, 
                   3, 
                   ppSMSMode, 
                   NUM_SMS_MODE, NUM_SMS_MODE,
                   (UINT8)SKY_GET_SMS_ALERT_TYPE()),
                   SMS_SCROLL_LIST_STYLE);

    // 알람종류
    nBellIdx = sFindSmsBellIndex(SKY_GET_SMS_BELL_ID());
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_ALARM_KIND);

    EQC_SET_STYLE( EQC_ScrollCombo(EQS_NA, EQS_NA, LCD_COLUME_SIZE -1, 
                   3, 
                   ppSMSKind, 
                   NUM_SMS_KIND, NUM_SMS_KIND,
                   nBellIdx),
                   SMS_SCROLL_LIST_STYLE);

    // 알람크기
    EQC_StaticText(EQS_NA, EQS_NA, TKN_SMS_ALARM_VOL);
    hProgBar = EQC_ProgressBar(EQS_NA, EQS_NA, 
        PROGRESS_BAR_WIDTH + 8, PROGRESS_BAR_HEIGHT +4, 0, 5, SKY_GET_SMS_BELL_VOLUME(),FALSE);
//    EQC_SetProgressBarLimit(hProgBar, 1, 5);
    // Formatting
    EQS_StartFormatter();

    for(nCtrl = 0; nCtrl < SS_SMS_MAX; nCtrl++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), 
            ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);

        EQS_NewLine();
    }

    EQS_EndFormatter();

}


// Message Arriving Alarm Setting Menu
void SKY_API SKA_MsgAlarm(SKY_EVENT_T* pEvent)
{
    STATIC HCONTROL hMsgConfirm, hMsgSave;
    WORD   nIdx;
    UINT8  nCtrl;
    HCONTROL        hMsg;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_SMS_MENU4), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        sMsgAlarmControls(); // Add Controls
        EQS_FitCanvasSize();
        break;
        
    case SE_APP_END :   /* The very last event */ 
        SKY_StopMusic();
        EQS_ReleaseDC();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;


    case SE_TM_TIMER1:
        if(EQS_HaveFocus(EQS_GetNthIOC(SS_SMS_KIND)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            SKY_PlayMusicEx(sGetCurrentSMSBellId(),  
                EQC_GetProgressBar(EQS_GetNthIOC(SS_SMS_VOL)), 0); 
        }
        break;

    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SS_SMS_KIND:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        case SS_SMS_VOL:
            if(pEvent->dwParam) 
                break;
            SKY_StopMusic();
            SKY_PlayMusicEx(sGetCurrentSMSBellId(), EQC_GetProgressBar(EQS_GetNthIOC(SS_SMS_VOL)), 0); 
            break;

        default:
            break;
        }
        break;

    // Handle I/O Control Events
    case SE_IO_SCROLL_LIST_SELECT:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SS_SMS_KIND:
            SKY_StopMusic();
            break;

        default:
            break;
        }
        break;

    case SE_IO_PROGRESS_BAR_SELECT:
        if(pEvent->wParam == EQS_GetNthIOC(SS_SMS_VOL))
        {
            SKY_StopMusic();
        }
        break;

    case SE_IO_SCROLL_LIST_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SS_SMS_KIND:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_PROGRESS_BAR_UP:
    case SE_IO_PROGRESS_BAR_DOWN:
       if(pEvent->wParam == EQS_GetNthIOC(SS_SMS_VOL))
        {
            SKY_StopMusic();
            SKY_PlayMusicEx(sGetCurrentSMSBellId(), (WORD)pEvent->dwParam, 0);
        }
        break;

    case SE_SOFTKEY_SAVE:
        SKY_StopTone();
        SKY_StopMusic();

        for(nCtrl = 0; nCtrl < SS_SMS_MAX; nCtrl++)
        {
            switch(nCtrl)
            {
            case SS_SMS_TYPE:
                SKY_SET_SMS_ALERT_TYPE((SMS_ALERT_TYPE_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(nCtrl)));
                break;

            case SS_SMS_KIND:
                nIdx = sGetCurrentSMSBellId();
//                SKY_SET_SMS_ALERT_BELL_ID(sGetCurrentSMSBellId());
                SKY_SET_SMS_ALERT_BELL_ID(nIdx);
                break;

            case SS_SMS_VOL:
                SKY_SET_SMS_ALERT_BELL_VOLUME(EQC_GetProgressBar(EQS_GetNthIOC(nCtrl)));
                break;

            default:
                break;
            }
        }

        EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_SMS_TEXT_MESSAGE_10), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        break;
/*
    case SE_SOFTKEY_INIT:
        hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                    MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_SUCCESS);
        EQC_SET_USERDATA(hMsg, pEvent->EventID);
        break;
*/
       
    case SE_IO_MESGBOX_NO:        
    case SE_IO_MESGBOX_END:
        if(pEvent->dwParam == SE_KEY_END) 
        {
            EQS_END_TO(EQS_POP_TO_ROOT);            
        }
        else
        {
            EQS_END();
        }
        break;

	case SE_IO_MESGBOX_YES:
        if(pEvent->dwParam == SE_SOFTKEY_INIT)
        {
            SKY_SET_SMS_ALERT_TYPE((SMS_ALERT_TYPE_E)INIT_SMS_ALERT_TYPE);
            SKY_SET_SMS_ALERT_BELL_ID(INIT_SMS_ALERT_BELL_ID);
            SKY_SET_SMS_ALERT_BELL_VOLUME(INIT_SMS_ALERT_BELL_VOLUME);

            hMsg = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                    MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
            EQC_SET_USERDATA(hMsg, SE_SOFTKEY_INIT);
        }
        else
        {
            for(nCtrl = 0; nCtrl < SS_SMS_MAX; nCtrl++)
            {
                switch(nCtrl)
                {
                case SS_SMS_TYPE:
                    SKY_SET_SMS_ALERT_TYPE(
                        (SMS_ALERT_TYPE_E)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(nCtrl)));
                    break;

                case SS_SMS_KIND:
                    nIdx = sGetCurrentSMSBellId();
                    SKY_SET_SMS_ALERT_BELL_ID(nIdx);
                    break;

                case SS_SMS_VOL:
                    SKY_SET_SMS_ALERT_BELL_VOLUME(EQC_GetProgressBar(EQS_GetNthIOC(nCtrl)));
                    break;

                default:
                    break;
                }
            }

            hMsgSave = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            EQC_SET_USERDATA(hMsgSave, pEvent->dwParam);    //SE_KEY_CLEAR or SE_KEY_END
        }
		break;

    case SE_IO_SCROLL_LIST_CANCEL:
    case SE_IO_PROGRESS_BAR_CANCEL:
        SKY_StopMusic();
        break;

    case SE_IO_FOCUS_CHG:
        if(pEvent->wParam != EQS_GetNthIOC(SMA_MSG_ALARM_KIND))
        {
            SKY_StopMusic();
        }
        else
        {
            SKY_PlayMusicEx(sGetCurrentSMSBellId(), 
                (WORD)EQC_GetProgressBar(EQS_GetNthIOC(SS_SMS_VOL)), 0);
        }
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		if(EQS_IsControlDataChanged())
		{
            SKY_StopMusic();
            hMsgConfirm = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CONFIRM_SAVE), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

            EQC_SET_USERDATA(hMsgConfirm, pEvent->EventID);
        }
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_END();
		}
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


VOID SKA_MainSMS(SKY_EVENT_T *pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START :   /* The very first event */
        break;

    case SE_APP_END :     /* The very last event */
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */
        break;

    case SE_APP_DRAW :    /* draw */
        break;

    case SE_SMS_MESSAGE:
        EQS_START_SIBLING(SKA_MessageNotif, FALSE);
        EQS_PASS_CURRENT_EVENT();
        break;
#if defined FEATURE_SKTT_EQUS_LOCATION && !defined WIN32
    case SE_LOCATION_INFORM:
        EQS_START_SIBLING(SKA_LocationNotif, FALSE);
        EQS_PASS_CURRENT_EVENT();
        break;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION

    case SE_KEY_F1:
    case SE_SOFTKEY_SMS:
// huni 01.08.13
//        EQS_START_SIBLING(SKA_SMSMenu, FALSE);
        break;        

    default :
		EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}