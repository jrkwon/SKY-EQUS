/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     eqa_idle.c
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
** 2001-02-17 jrkwon     Make change to releas for ES
** 2000-07-21 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "comdef.h"
#ifndef WIN32
#include "dsskt.h"
#endif
#include <stdio.h>
#include "ska.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define sMAX_INFO_BUFFERS          10
#define sIS_MESSAGE_INFO()        ((q_cnt( &l_MessageInfoQ ) > 0 ) ? TRUE : FALSE)


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
EDITORINFO_T       g_NumEditInfo;   
EDITORINFO_T       g_AlphaEditInfo; 
UINT8              g_nRealPhoneNumberLength;


/* token array */
ROM SKY_TOKEN g_aDayOfWeek[] = {
    TKN_DAT_MON, TKN_DAT_TUE, TKN_DAT_WED, TKN_DAT_THU,
    TKN_DAT_FRI, TKN_DAT_SAT, TKN_DAT_SUN
};

/* token array */
ROM SKY_TOKEN g_aAMPM[] = {
    TKN_DAT_AM, TKN_DAT_PM
};

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL BOOL l_fDTR = FALSE;
LOCAL BOOL l_fActiveAlarm = FALSE;

LOCAL INFO_MESSAGE_T l_aMessageBuffers[sMAX_INFO_BUFFERS];
LOCAL QUEUE_T        l_MessageInfoQ;       /* queue for info - ui_cmd does this */
LOCAL QUEUE_T        l_MessageInfoFreeQ;  /* free buffers for talking to UI */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void  SKY_API sInitMessageDisp(void);
LOCAL void  SKY_API sAddMessage2MessageQ(BYTE* pszBuf);
LOCAL void  SKY_API sGetMessageFromMessageQ(BYTE* pszBuf);
LOCAL void  SKY_API sUpdateMessageBoxContents(BYTE* pszBuf);
LOCAL void  SKY_API sSKA_ShowMessage(SKY_EVENT_T *pEvent);
LOCAL void  SKY_API sDrawMainIdleCML(void);
LOCAL BOOL  SKY_API sChangeMinute(void);
LOCAL void  SKY_API sDisplayAutoAnswerCurrentTime(BOOL fDisplayRightNow);
LOCAL EQS_APPLET* SKY_API sWhereToGo(void);


/*
** ***************************************************************************
** info message
** ***************************************************************************
*/
LOCAL void SKY_API sInitMessageDisp(void)
{
    HCONTROL           hControl;
    INFO_MESSAGE_T*	 pMessageData;
    DWORD              dwLength;


    if ((pMessageData=(INFO_MESSAGE_T *)q_get((q_type *)&l_MessageInfoFreeQ)) == NULL) 
    {
        // There is no free buffer 
        MSG_ERROR("No Free MessageData Buffers", 0, 0, 0);
    }
    else
    {   		  
        dwLength = (GET_EVENT_PARAM()->call_event.call_info).alpha.len; 
        (GET_EVENT_PARAM()->call_event.call_info).alpha.buf[dwLength] = '\0';	
        STRCPY(pMessageData->szText, (GET_EVENT_PARAM()->call_event.call_info).alpha.buf);
        // Queue Message Data
        if(dwLength > 0)
        {
            q_put((q_type *)&l_MessageInfoQ, (q_link_type *)&pMessageData->Link);
        }
    }

    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
     EQS_DeleteControl(hControl);
    }

    if(sIS_MESSAGE_INFO())
    {
        if((pMessageData=(INFO_MESSAGE_T *)q_get((q_type *)&l_MessageInfoQ)) != NULL)
        {
          hControl = EQC_NoBeepMessageBox(pMessageData->szText, MESSAGE_BOX_TIME_UNLIMIT,  
                                          MBT_OK, MBT_CONFIRM);
        }
    }
    else
    {
        EQS_END();
    }
}


LOCAL void SKY_API sAddMessage2MessageQ(BYTE* pszBuf)
{
    INFO_MESSAGE_T*	 pMessageData;
    DWORD            dwLength;    

    if(q_cnt( &l_MessageInfoQ ) <= sMAX_INFO_BUFFERS)
    {
        if ((pMessageData=(INFO_MESSAGE_T *)q_get((q_type *)&l_MessageInfoFreeQ)) != NULL) 
        {
            dwLength = (GET_EVENT_PARAM()->call_event.call_info).alpha.len;
            (GET_EVENT_PARAM()->call_event.call_info).alpha.buf[dwLength] = '\0';			  
            STRCPY(pMessageData->szText, (GET_EVENT_PARAM()->call_event.call_info).alpha.buf);
            STRCPY(pszBuf, (GET_EVENT_PARAM()->call_event.call_info).alpha.buf);
            // Queue Message Data
            q_put((q_type *)&l_MessageInfoQ, (q_link_type *)&pMessageData->Link);

        }
    }
}

LOCAL void SKY_API sGetMessageFromMessageQ(BYTE* pszBuf)
{
    INFO_MESSAGE_T*	 pMessageData;
  
    if((pMessageData=(INFO_MESSAGE_T *)q_get((q_type *)&l_MessageInfoQ)) != NULL)
    {
        STRCPY(pszBuf, pMessageData->szText);
    }
}

LOCAL void SKY_API sUpdateMessageBoxContents(BYTE* pszBuf)
{
  HCONTROL hControl;

  if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
  {
     EQS_DeleteControl(hControl);
  }

  EQC_NoBeepMessageBox(pszBuf, MESSAGE_BOX_TIME_UNLIMIT,  MBT_OK, MBT_CONFIRM);
}

void SKY_API InitMessageQueue(void)
{
	UINT8 i;

    /* Initialize the informational display queues and buffers */
    (void) q_init( &l_MessageInfoQ);
    (void) q_init( &l_MessageInfoFreeQ );
    for( i=0; i<sMAX_INFO_BUFFERS; i++ ) 
    {
        q_put( &l_MessageInfoFreeQ, q_link( &l_aMessageBuffers[i], 
               &l_aMessageBuffers[i].Link ));
    }
}

#define sMAX_MESSAGE_LENGTH         50

LOCAL void SKY_API sSKA_ShowMessage(SKY_EVENT_T *pEvent)
{
    BYTE pszMessage[sMAX_MESSAGE_LENGTH];

    switch(pEvent->EventID)
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      NULL, // show annunciators
                      WA_SHOW_HEADER|WA_SHOW_CML);
        sInitMessageDisp();
    break;

	case SE_APP_END:
		EQS_ReleaseDC();
		break;

    case SE_APP_DRAW:
		break;

    case SE_APP_RESUME:
		break;

    case SE_SYS_INFO:     /* Ignore this event */
		break;
    
    case SE_SYS_TO_IDLE:  /* Ignore this event */
        break;

    case SE_CALL_DISPLAY:
        sAddMessage2MessageQ(pszMessage);
        break;    

    case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_CLEAR:
    case SE_KEY_END:
    case SE_KEY_CENTER:
    case SE_KEY_F1:
    case SE_KEY_F2:
    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_DOUBLE_CLICK:
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_RECORD_LONG:
    case SE_KEY_PLAY_LONG:
    case SE_KEY_0 :
    case SE_KEY_1 :
    case SE_KEY_2 :
    case SE_KEY_3 :
    case SE_KEY_4 :
    case SE_KEY_5 :
    case SE_KEY_6 :
    case SE_KEY_7 :
    case SE_KEY_8 :
    case SE_KEY_9 :
    case SE_KEY_STAR :
    case SE_KEY_POUND :
    case SE_KEY_UP :
    case SE_KEY_DOWN :
    case SE_SOFTKEY_LEFT :   /* PF3 : */
    case SE_SOFTKEY_RIGHT :  /* PF2 : */
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG : /* UI_CLRALL_F: */
        if(sIS_MESSAGE_INFO())
        {
            sGetMessageFromMessageQ(pszMessage);
            sUpdateMessageBoxContents(pszMessage);
        }
        else
        {
            EQS_END();
        }
	    break;   

    case SE_KEY_PHONE_CLOSE:
        /* Phone을 닫을 경우에는 메시지 Queue에 있는 내용을 모두 Pop하고 */
        /* Applet을 종료한다.                                            */
        while(sIS_MESSAGE_INFO())
        {
            sGetMessageFromMessageQ(pszMessage);
        }
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC(pEvent);      
        break;  
    }
}


/*
** ***************************************************************************
** display date and time (or image/animation) in idle screen
** ***************************************************************************
*/

#ifdef FEATURE_SKY_SUBLCD

void SKY_API DisplayDateTimeInSubLCD(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute)
{
    BYTE szStr[SKY_SUB_SCREEN_WIDTH/SUB_LCD_FONT_WIDTH+1];
    BYTE bAMPM = 'A';

    if(SKY_SubLCDIsEndText(LN_LOWER) == FALSE) /* 시간 표시할 수 없으면, 중지 */
        return;

    if(wHour >= 12)
    {
        if(wHour != 12)
            wHour -= 12;
        bAMPM = 'P';
    }
    //if(wHour == 12)// && wMinute != 0)
    //    bAMPM = 'P';
    //else if((wHour == 12 || wHour == 0) && wMinute == 0)
    //{
    //    /* noon or midnight */
    //    bAMPM = ' ';
    //}

    /* %02d.%02d%c%c %02d:%02d%c */
    /* 월화수목금토일 --> 그림 문자는 ASCII 1~21 */
    {
        char aDayOfWeekCode[] = { 0, 3, 6, 9, 12, 15, 20 }; /* 토 : 15,18,19 */

        if(wDayOfWeek == 5) /* 토요일 */
        {
            sprintf((char*)szStr, (char*)SKY_GET_TOKEN(TKN_SLCD_DATE_TIME_FMT), 
                           wMonth, wDay, 
                           //(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[wDayOfWeek]),
                           aDayOfWeekCode[wDayOfWeek], aDayOfWeekCode[wDayOfWeek]+3, aDayOfWeekCode[wDayOfWeek]+4,
                           wHour, wMinute, bAMPM);
        }
        else
        {
            sprintf((char*)szStr, (char*)SKY_GET_TOKEN(TKN_SLCD_DATE_TIME_FMT), 
                           wMonth, wDay, 
                           //(CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[wDayOfWeek]),
                           aDayOfWeekCode[wDayOfWeek], aDayOfWeekCode[wDayOfWeek]+1, aDayOfWeekCode[wDayOfWeek]+2,
                           wHour, wMinute, bAMPM);
        }

#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
        SKY_SubLCDTextOutBufferLen(LN_LOWER, szStr, 14);
        SKY_SubLCDEndText(LN_LOWER);
#elif SKY_MODEL_ID == SKY_MODEL_B0
        SKY_SubLCDTextOutStr(LN_UPPER, szStr);
        SKY_SubLCDEndText(LN_UPPER);
#endif //#ifdef FEATURE_SKY_SUBLCD
    }
}

BOOL SKY_API DisplayCurrentDateTimeInSubLCD(BOOL fDrawNow)
{
    DWORD dwCurrentTime;
    STATIC WORD s_wMinute = 0;
    clk_julian_type julian;
    BOOL fChangeMinute = FALSE;

    dwCurrentTime = SKY_GetLocalTime();
            
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);
        if(fDrawNow || (s_wMinute != julian.minute))
        {
#ifdef WAPPER
            if(SKY_IS_IN_WAP())
                SendCommand2Wap(WAP_SLCD_DISP_TIME);
            else 
#endif //#ifdef WAPPER
#ifdef FEATURE_SKY_SUBLCD
                DisplayDateTimeInSubLCD(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
#endif
            s_wMinute = julian.minute;
            fChangeMinute = TRUE;
        }
    }
    return fChangeMinute;
}

#endif //FEATURE_SKY_SUBLCD

LOCAL void SKY_API sDrawMainIdleCML(void)
{
    BITMAP *pBitmap;
    BOOL fIconMenu;

    fIconMenu = SKY_GET_SEL_MENU_MODE();

    switch(SKY_GET_COLOR_SCHEME())
    {
    case CS_STANDARD :
    case CS_STANDARD_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleBlueI : (BITMAP *)&g_BmpCommandlineIdleBlueT;
        break;
    case CS_RED_WHITE :
    case CS_RED_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleRedI : (BITMAP *)&g_BmpCommandlineIdleRedT;
        break;
    case CS_GREEN_WHITE :
    case CS_GREEN_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleGreenI : (BITMAP *)&g_BmpCommandlineIdleGreenT;
        break;
    case CS_PURPLE_WHITE :
    case CS_PURPLE_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleVioletI : (BITMAP *)&g_BmpCommandlineIdleVioletT;
        break;
    case CS_MARIN_WHITE :
    case CS_MARIN_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleDeepBlueI : (BITMAP *)&g_BmpCommandlineIdleDeepBlueT;
        break;
    case CS_ORANGE_WHITE :
    case CS_ORANGE_BLACK :
        pBitmap = fIconMenu ? (BITMAP *)&g_BmpCommandlineIdleBrownI : (BITMAP *)&g_BmpCommandlineIdleBrownT;
        break;
    }

    EQS_PutBitmap(0, SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+(pBitmap->Header.Size.yHeight)), pBitmap);
}

//#define sBOX_X                  12
//#define sBOX_Y                  45//52
//#define sPHONE_NAME_Y           29//56
#define sREC_ICON_X                 66//93//73
#define sREC_ICON_Y                 105//116//81
#define sREC_X                      97//111//92
#define sREC_Y                      105//115//80
//#define sDATE_Y                 77//19
#define sAM_X                       107//97//84
#define sAM_Y                       79//52//34
#define sPM_X                       sAM_X//84
#define sPM_Y                       sAM_Y//45
#define sTIME_X                     9//15//18
#define sTIME_Y                     64 //52//34
#define sNUM4TIME_DX                19//16//13 // 16
#define sNUM4TIME_SPACE             0

//#define sNAME_DY                7
#define sOUTLINE_COLOR              ((SKY_GET_COLOR_SCHEME() >= COLOR_SCHEME_COUNT) ? (COLOR)EQS_RGB(68,68,68) : (COLOR)EQS_RGB(210, 210, 210))
#define sTEXT_COLOR                 ((SKY_GET_COLOR_SCHEME() >= COLOR_SCHEME_COUNT) ? (COLOR)WHITE : (COLOR)BLACK)
#define sGAP_DATE                   (4+2)

#define sANALOG_CLOCK_MIN_LENGTH    (30)
#define sANALOG_CLOCK_HOUR_LENGTH   (20)

#define sANALOG_CLOCK_ORG_X         (63)
#define sANALOG_CLOCK_ORG_Y         (62)

#define sANALOG_CLOCK1_X            (22)
#define sANALOG_CLOCK1_Y            (21)
#define sANALOG_CLOCK2_X            (23)
#define sANALOG_CLOCK2_Y            (22)

#define PI                          3.1415926535

LOCAL SKY_API sDrawAnalogWatch(WORD wHour, WORD wMinute, IDLE_DISPLAY_WATCH_E Watch)
{
    FLOAT fAngleHour, fAngleMinute;
    UINT8 nHourDx, nHourDy, nMinDx, nMinDy;
    COLOR OldColor = GetForeColor();
    COORD xHourEnd, yHourEnd;
    COORD xMinEnd, yMinEnd;
    FLOAT afAngleTable[] = { PI/4.0, (PI*3)/4.0, (PI*5)/4.0, (PI*7)/4.0, 2*PI };
    /* 
    ** these function prototypes are very important.
    ** if not defined, sin, cos malfunction.
    */
    double  cos(double);
    double  sin(double);

    if(Watch == IDW_ANALOG1)
        EQS_PutBitmap(sANALOG_CLOCK1_X, sANALOG_CLOCK1_Y, &g_BmpClock01);
    else /* IDW_ANALOG2 */
        EQS_PutBitmap(sANALOG_CLOCK2_X, sANALOG_CLOCK2_Y, &g_BmpClock02);

    fAngleMinute = wMinute / 60.0 * 2 * PI;
    fAngleHour = (wHour / 12.0 + (wMinute / 60.0) / 12.0) * 2 * PI;

    if(fAngleHour >= afAngleTable[0] && fAngleHour < afAngleTable[1])
        nHourDx = 0, nHourDy = 1;
    else if(fAngleHour >= afAngleTable[1] && fAngleHour < afAngleTable[2])
        nHourDx = 1, nHourDy = 0;
    else if(fAngleHour >= afAngleTable[2] && fAngleHour < afAngleTable[3])
        nHourDx = 0, nHourDy = 1;
    else if((fAngleHour >= 0 && fAngleHour < afAngleTable[0])
            || (fAngleHour >= afAngleTable[3] && fAngleHour < (2*PI)))
        nHourDx = 1, nHourDy = 0;

    if(fAngleMinute >= afAngleTable[0] && fAngleMinute < afAngleTable[1])
        nMinDx = 0, nMinDy = 1;
    else if(fAngleMinute >= afAngleTable[1] && fAngleMinute < afAngleTable[2])
        nMinDx = 1, nMinDy = 0;
    else if(fAngleMinute >= afAngleTable[2] && fAngleMinute < afAngleTable[3])
        nMinDx = 0, nMinDy = 1;
    else if((fAngleMinute >= 0 && fAngleMinute < afAngleTable[0])
            || (fAngleMinute >= ((PI*7)/4.0) && fAngleMinute < (2*PI)))
        nMinDx = 1, nMinDy = 0;

    xHourEnd = sANALOG_CLOCK_ORG_X + sin(fAngleHour) * sANALOG_CLOCK_HOUR_LENGTH;
    yHourEnd = sANALOG_CLOCK_ORG_Y - cos(fAngleHour) * sANALOG_CLOCK_HOUR_LENGTH;
    xMinEnd  = sANALOG_CLOCK_ORG_X + sin(fAngleMinute) * sANALOG_CLOCK_MIN_LENGTH;
    yMinEnd  = sANALOG_CLOCK_ORG_Y - cos(fAngleMinute) * sANALOG_CLOCK_MIN_LENGTH;

    /* draw hour, minute arms */
    SetForeColor(sTEXT_COLOR);
    /* hour */
    Line(sANALOG_CLOCK_ORG_X, sANALOG_CLOCK_ORG_Y, xHourEnd, yHourEnd);
    Line(sANALOG_CLOCK_ORG_X+nHourDx, sANALOG_CLOCK_ORG_Y+nHourDy, xHourEnd+nHourDx, yHourEnd+nHourDy);
    Line(sANALOG_CLOCK_ORG_X-nHourDx, sANALOG_CLOCK_ORG_Y-nHourDy, xHourEnd-nHourDx, yHourEnd-nHourDy);
    /* minute */
    Line(sANALOG_CLOCK_ORG_X, sANALOG_CLOCK_ORG_Y, xMinEnd, yMinEnd);
    Line(sANALOG_CLOCK_ORG_X+nMinDx, sANALOG_CLOCK_ORG_Y+nMinDy, xMinEnd+nMinDx, yMinEnd+nMinDy);
    //Line(sANALOG_CLOCK_ORG_X-nDx, sANALOG_CLOCK_ORG_Y-nDy, xMinEnd-nDx, yMinEnd-nDy);

    /* draw border of hour and minute arms */
    SetForeColor(sOUTLINE_COLOR);
    /* hour */
    Line(sANALOG_CLOCK_ORG_X+nHourDx*2, sANALOG_CLOCK_ORG_Y+nHourDy*2, xHourEnd+nHourDx*2, yHourEnd+nHourDy*2);
    Line(sANALOG_CLOCK_ORG_X-nHourDx*2, sANALOG_CLOCK_ORG_Y-nHourDy*2, xHourEnd-nHourDx*2, yHourEnd-nHourDy*2);
    /* minute */
    Line(sANALOG_CLOCK_ORG_X+nMinDx*2, sANALOG_CLOCK_ORG_Y+nMinDy*2, xMinEnd+nMinDx*2, yMinEnd+nMinDy*2);
    Line(sANALOG_CLOCK_ORG_X-nMinDx, sANALOG_CLOCK_ORG_Y-nMinDy, xMinEnd-nMinDx, yMinEnd-nMinDy);
    /* tip of the hour arm */
    Line(xHourEnd, yHourEnd, xHourEnd, yHourEnd);
    Line(xHourEnd+nHourDx, yHourEnd+nHourDy, xHourEnd+nHourDx, yHourEnd+nHourDy);
    Line(xHourEnd-nHourDx, yHourEnd-nHourDy, xHourEnd-nHourDx, yHourEnd-nHourDy);
    /* tip of the minute arm */
    Line(xMinEnd, yMinEnd, xMinEnd, yMinEnd);
    Line(xMinEnd+nMinDx, yMinEnd+nMinDy, xMinEnd+nMinDx, yMinEnd+nMinDy);

    SetForeColor(OldColor);
}

void SKY_API SKY_DisplayDateTime(WORD wMonth, WORD wDay, WORD wDayOfWeek, WORD wHour, WORD wMinute)
{
    BYTE szBuf[80];
    BITMAP Bitmap;
    COORD x, yDate, yName;
    INT8 nAMPM = 0;
    DRAW_MODE OldDrawMode;
    COLOR OldColor;
    FONT_TYPE oldFont;
    FONT_TYPE OldFontType;
    IDLE_SCREEN_PREVIEW_INFO_T PreviewInfo, ViewInfo;
    IDLE_SCREEN_E IdlePicture;

    BYTE* pszText;

    if(IsIdleScreenPreview()) /* preview */
    {
        GetIdleScreenPreviewInfo(&PreviewInfo);
        ViewInfo = PreviewInfo;
    }
    else /* general */
    {
        ViewInfo.fDisplayName = SKY_GET_IDLE_DISPLAY_NAME();
        ViewInfo.Screen = SKY_GET_IDLE_SCREEN();
        STRCPY(ViewInfo.szDispalyName, SKY_GET_BANNER());
        ViewInfo.Watch = SKY_GET_IDLE_DISPLAY_WATCH();
    }

    OldFontType = EQS_GetFontType();
    OldDrawMode = EQS_GetDrawMode();

    EQS_SetFontType(FT_MED);
    EQS_SetDrawMode(DM_OR);

    if(ViewInfo.Screen != IS_PICTURE)
        ClearViewPort();

    /* ********************************************
    ** 휴대폰 이름 표시
    */
    /* 1. 휴대폰 이름 결정 */
    pszText = NULL;

    /* 미리보기할 때는 무조건 설정한 이름으로 표시 */
    if(IsIdleScreenPreview() && PreviewInfo.fDisplayName)
    {
        pszText = PreviewInfo.szDispalyName;
    }
    else if(g_fIrDAPortForPC) /* PC와 IrDA 통신 중에는 휴대폰 이름 표시 여부와 상관없이 항상 표시하도록 함 */
    {
        pszText = (BYTE*)SKY_GET_TOKEN(TKN_IDLE_IRDA_PC);
    }
    /* do not display phone name in case of analog clock */
    else if(ViewInfo.fDisplayName)//SKY_GET_IDLE_DISPLAY_NAME())
    {
        if(SKY_IS_MAINTREQ())   // 미등록 단말기
        {
            pszText = (BYTE*)SKY_GET_TOKEN(TKN_IDLE_MAINT_REQ);
        }
        else if(SKY_IS_OFFLINE())   // 애프터서비스
        {
            pszText = (BYTE*)SKY_GET_TOKEN(TKN_IDLE_NEED_AFTERSERVICE);
        }
        else if(l_fDTR)
        {
            if(SKY_GET_DTR_DISP() == MODEM_INCOMMING)
            {
                pszText = (BYTE*)SKY_GET_TOKEN(TKN_MODEM_INCOMING);
            }
            else if(SKY_GET_DTR_DISP() == FAX_INCOMMING)
            {
                pszText = (BYTE*)SKY_GET_TOKEN(TKN_FAX_INCOMING);
            }
            else
            {
                l_fDTR = FALSE;
                pszText = (BYTE*)ViewInfo.szDispalyName;//SKY_GET_BANNER();
            }
        }
        else
        {
            pszText = (BYTE*)ViewInfo.szDispalyName;//SKY_GET_BANNER();
        }
    }

    /* 2. 휴대폰 이름 좌표 결정 */
    if(ViewInfo.Watch == IDW_SMALL)
    {
        if(ViewInfo.Screen == IS_PICTURE) /* 그림이 있는 경우 */
        {
            yName = 2;
            yDate = 113;//SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)-EQS_CharHeight(' ')-sGAP_CML;
        }
        else /* 그림이 없는 경우 */
        {
            yName = 47;
            yDate = yName + EQS_CharHeight(' ') + sGAP_DATE;
        }
    }
    else if(ViewInfo.Watch == IDW_ANALOG1 || ViewInfo.Watch == IDW_ANALOG2)
    {
        //pszText = NULL; /* 제목 표시하지 않음 */
        yName = 2;
        yDate = 113;
    }
    else /* 큰 시계 */
    {
        yName = 25;
        yDate = 47;
    }

    if(pszText)
    {
        x = SKY_SCREEN_WIDTH/2 - (STRLEN(pszText)*EQS_CharWidth(' '))/2;

        OldColor = EQS_GetTextColor();
        EQS_SetTextColor(sOUTLINE_COLOR);
        EQS_TextOutStrTDM(x, yName, pszText, TDM_OUTLINE);
        
        EQS_SetTextColor(sTEXT_COLOR);
        EQS_TextOutStr(x, yName, pszText);
        EQS_SetTextColor(OldColor);
    }

    /* ********************************************
    ** 날짜 및 시간 표시 
    */
    /* am/pm */
    nAMPM = 0;
    if(wHour >= 12)
    {
        if(wHour != 12)
            wHour -= 12;
        nAMPM = 1;
    }
    //if(wHour == 12)// && wMinute != 0)
    //    nAMPM = 1;
    //else if((wHour == 12 || wHour == 0) && wMinute == 0)
    //{
    //    /* noon or midnight */
    //    if(wHour == 12)
    //        nAMPM = 1;
    //    else
    //        nAMPM = 0;
    //}

    /* date */
    if(ViewInfo.Watch != IDW_NO)
    {
        if(ViewInfo.Watch == IDW_SMALL)
        {
            sprintf((char*)szBuf, (char*)"%2d.%02d[%2s] %2d:%02d %cM", 
                                  wMonth, wDay, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[wDayOfWeek]),
                                  wHour, wMinute, nAMPM == 1 ? 'P' : 'A');
        }
        else
        {
            sprintf((char*)szBuf, (char*)"%2d월 %02d일 [%2s]", 
                                  wMonth, wDay, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[wDayOfWeek]));
        }
        x = SKY_SCREEN_WIDTH/2 - (STRLEN(szBuf)*EQS_CharWidth(' '))/2;

        OldColor = EQS_GetTextColor();
        EQS_SetTextColor(sOUTLINE_COLOR);
        EQS_TextOutStrTDM(x, yDate, szBuf, TDM_OUTLINE);

        EQS_SetTextColor(sTEXT_COLOR);
        EQS_TextOutStr(x, yDate, (BYTE *)szBuf);
        EQS_SetTextColor(OldColor);
    
        /* 큰 시계인 경우 */
        if(ViewInfo.Watch == IDW_BIG)
        {
            /* display hour */
            x = sTIME_X;
            Bitmap = g_BmpBigWatchNumber;
            if((wHour/10) != 0)
            {
                Bitmap.pbData = g_BmpBigWatchNumber.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*(wHour/10);
                EQS_PutBitmap(x, sTIME_Y, &Bitmap);
            }
            x += sNUM4TIME_DX + sNUM4TIME_SPACE;
            Bitmap.pbData = g_BmpBigWatchNumber.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*(wHour%10);
            EQS_PutBitmap(x, sTIME_Y, &Bitmap);
            /* colon */
            x += sNUM4TIME_DX + sNUM4TIME_SPACE;
            Bitmap.pbData = g_BmpBigWatchNumber.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*10;
            EQS_PutBitmap(x, sTIME_Y, &Bitmap);
            // display minute
            x += sNUM4TIME_DX + sNUM4TIME_SPACE;
            Bitmap.pbData = g_BmpBigWatchNumber.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*(wMinute/10);
            EQS_PutBitmap(x, sTIME_Y, &Bitmap);
            x += sNUM4TIME_DX + sNUM4TIME_SPACE;
            Bitmap.pbData = g_BmpBigWatchNumber.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*(wMinute%10);
            EQS_PutBitmap(x, sTIME_Y, &Bitmap);

            if(nAMPM >= 0)
            {
                BITMAP Bitmap = g_BmpBigAMPM;
                if(nAMPM == 1) /* pm */
                    Bitmap.pbData = Bitmap.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header);
                EQS_PutBitmap(sPM_X, sPM_Y, &Bitmap);
            }
        }
        else if(ViewInfo.Watch == IDW_ANALOG1 || ViewInfo.Watch == IDW_ANALOG2)
        {
            sDrawAnalogWatch(wHour, wMinute, ViewInfo.Watch);
        }
    }

    /* 녹음 개수 */
    /* 그림이 없는 경우, 아날로그 시계가 아닌 경우에만 표시 */
    if(ViewInfo.Screen != IS_PICTURE && (ViewInfo.Watch != IDW_ANALOG1 && ViewInfo.Watch != IDW_ANALOG2)) 
    {
        EQS_PutBitmap(sREC_ICON_X, sREC_ICON_Y, &g_BmpRecordCount);
        
        OldColor = EQS_GetTextColor();
        EQS_SetTextColor(sOUTLINE_COLOR);
        EQS_TextOutStrTDMF(sREC_X, sREC_Y, TDM_OUTLINE, (BYTE*)"%2d개%", GetVoiceInfo()->bTotalRecNumber);

        EQS_SetTextColor(sTEXT_COLOR);
        EQS_TextOutStrF(sREC_X, sREC_Y, (BYTE*)"%2d개%", GetVoiceInfo()->bTotalRecNumber);
        EQS_SetTextColor(OldColor);
    }

    /* IDLE 화면 명령행 */
    sDrawMainIdleCML();

    EQS_SetDrawMode(OldDrawMode);
    EQS_SetFontType(OldFontType);
}

#define sNAME_TEXT_Y             3//24
#define sTIME_TEXT_Y             113

#define sBOX_COLOR               (COLOR)EQS_RGB(143, 148, 235)
//#define sOUTLINE_COLOR           (COLOR)EQS_RGB(210, 210, 210)

LOCAL BOOL SKY_API sChangeMinute(void)
{
    DWORD dwCurrentTime;
    STATIC WORD s_wMinute = 0, s_wHour = 0;
    clk_julian_type julian;
    BOOL fChangeMinute = FALSE;

    dwCurrentTime = SKY_GetLocalTime();
            
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);
        //if( l_fActiveAlarm && ((s_wHour != julian.hour )||(s_wMinute != julian.minute )) )
        //    l_fActiveAlarm = FALSE;

        if((s_wHour != julian.hour ) || (s_wMinute != julian.minute))
        {
            s_wHour = julian.hour;
            s_wMinute = julian.minute;
            fChangeMinute = TRUE;
        }
    }

    return fChangeMinute;
}

BOOL SKY_API DisplayDateTime(BOOL fDisplayRightNow)
{
    DWORD dwCurrentTime;
    STATIC WORD s_wMinute = 0, s_wHour = 0;
    clk_julian_type julian;
    BOOL fChangeMinute = FALSE;

    dwCurrentTime = SKY_GetLocalTime();
  
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);
        if(l_fActiveAlarm && ((s_wHour != julian.hour )||(s_wMinute != julian.minute)))
            l_fActiveAlarm = FALSE;

        if(fDisplayRightNow || (s_wHour != julian.hour ) || (s_wMinute != julian.minute))
        {
            //if(SKY_GET_IDLE_SCREEN() == IS_PICTURE)
            //    SKY_DisplayDateTimeWithPic(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
            //else
                SKY_DisplayDateTime(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
#ifdef FEATURE_SKY_SUBLCD
            DisplayDateTimeInSubLCD(julian.month, julian.day, julian.day_of_week, julian.hour, julian.minute);
#endif //#ifdef FEATURE_SKY_SUBLCD
            s_wHour = julian.hour;
            s_wMinute = julian.minute;
            fChangeMinute = TRUE;
        }
    }
    else
    {
        /* not a real date and time */
        //if(SKY_GET_IDLE_SCREEN() == IS_PICTURE)
        //    SKY_DisplayDateTimeWithPic(0, 0, 0, 0, 0);
        //else
            SKY_DisplayDateTime(0, 0, 0, 0, 0);
    }
    return fChangeMinute;
}


/*
** ***************************************************************************
** number edit and alpha edit buffer
** ***************************************************************************
*/
void SKY_API ClearNumEditInfo(void)
{
    EDITORINFO_T NumEdit;

    NumEdit         =  GET_NUMEDIT;
    NumEdit.fSecret =  FALSE;
    memset(NumEdit.aBuf,' ', EQS_MAX_DIAL_DIGITS);
    NumEdit.nPos    =  0;

    SET_NUMEDIT(NumEdit);
}

void SKY_API ClearAlphaEditInfo(void)
{
    EDITORINFO_T AlphaEdit;

    AlphaEdit         =  GET_ALPHAEDIT;
    AlphaEdit.fSecret =  FALSE;
    memset(AlphaEdit.aBuf,' ', EQS_MAX_DIAL_DIGITS);
    AlphaEdit.nPos    =  0;

    SET_ALPHAEDIT(AlphaEdit);
}

READPBOOK_E SKY_API IsConfidentialGroup(BYTE* aPhoneNumber)
{
    UINT16 nBookAddress = SearchNumber(aPhoneNumber, BSM_NUMBER_FULL_MATCH);

	return (nBookAddress == 0) ? PB_READ_NO_BOOK_INFO
                               : (IS_SECRET_GROUP(g_PersonGroup[nBookAddress-1]) == TRUE) ? PB_SECRET_OK : PB_READ_OK;
}


BYTE* SKY_API GetNumEditInfoBuf(void)
{
    return g_NumEditInfo.aBuf;
}

void SKY_API SetNumEditInfoBuf(BYTE* pbBuffer, UINT8 nLength)
{
    memcpy(g_NumEditInfo.aBuf, pbBuffer, nLength);
    g_NumEditInfo.aBuf[nLength] = '\0';
}

BYTE* SKY_API GetAlphaEditInfoBuf( void )
{
    return g_AlphaEditInfo.aBuf;
}

void SKY_API SetAlphaEditInfoBuf(BYTE* pbBuffer, UINT8 nLength )
{
    memcpy(g_AlphaEditInfo.aBuf, pbBuffer, nLength);  
}

void SKY_API MakeHypenedPhoneNumber(BYTE abSource[], BYTE abTarget[])
{
    UINT8 nCodePos, nHypenPos;
    
    nCodePos  = FindCodeSet(abSource);		
    nHypenPos = FindHypen(abSource, nCodePos, MAX_DIAL_DIGIT_LENGTH);
    MakeHypenedNumber(abSource, abTarget, MAX_DIAL_DIGIT_LENGTH,
                      nCodePos, nHypenPos);

}

void SKY_API SetRealPhoneNumberLength(UINT8 nLength)
{
    g_nRealPhoneNumberLength = nLength;
}

UINT8 SKY_API GetRealPhoneNumberLength(void)
{
    return g_nRealPhoneNumberLength;
}




#if 0 //000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
#define sAANS_TIME_FRAME_IMAGE_XPOS        57//61//58
#define sAANS_TIME_FRAME_IMAGE_YPOS        2 //3//17

#define sAANS_TIME_FIRST_XPOS              62//sAANS_TIME_FRAME_IMAGE_XPOS+2
#define sAANS_TIME_FIRST_YPOS              (26 - SKY_ANNUNCIATORS_HEIGHT)//sAANS_TIME_FRAME_IMAGE_YPOS+3

#define sAANS_TIME_SECOND_XPOS             sAANS_TIME_FIRST_XPOS
#define sAANS_TIME_SECOND_YPOS             sAANS_TIME_FIRST_YPOS + EQS_CharHeight('A')

#define sAANS_AM_PM_XPOS                   (sAANS_TIME_FIRST_XPOS + EQS_TextWidth((BYTE*)"12:13") + 3)//98
#define sAANS_AM_PM_YPOS                   (sAANS_TIME_SECOND_YPOS + 3)

LOCAL void SKY_API sDisplayAutoAnswerCurrentTime(BOOL fDisplayRightNow)
{
    DWORD             dwCurrentTime;
    clk_julian_type   julian;
    STATIC WORD       s_wMinute = 0;
    BYTE              szBuf[20];
    BITMAP            Bitmap;
    BOOL              fChangeMinute = FALSE, fAM;
    INT8              nAMPM = 0;
    //DRAW_MODE         OldDrawMode;

    //EQS_PutBitmap(sAANS_TIME_FRAME_IMAGE_XPOS, sAANS_TIME_FRAME_IMAGE_YPOS, &g_BmpDateTimePannel);
	    //g_BmpClockBox57x33x256c);	
    dwCurrentTime = SKY_GetLocalTime();

    //OldDrawMode = EQS_GetDrawMode();
    //EQS_SetDrawMode(DM_OR);

    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);
        if((fDisplayRightNow == TRUE) || (s_wMinute != julian.minute))
        {
            fAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);

            nAMPM = (fAM) ? 0 : 1;

            if(fAM == FALSE && julian.hour != 12)
            {
                julian.hour -= 12;
            }
            else if(fAM == FALSE && julian.hour == 24)
            {
                julian.hour = 0;
            }

            sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                         julian.month, julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]));
            EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);


            if(nAMPM >=0 ) /* AM */
            {
                Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
                Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
                EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            }

            /* display hour */
            sprintf((char*)szBuf, (char*)"%2d:%02d", julian.hour, julian.minute);
            EQS_TextOutStr(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, (BYTE *)szBuf); 
            s_wMinute = julian.minute;
            fChangeMinute = TRUE;
        }
    }
    else /* Valid한 시간이 아닐 경우 */
    {
        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                          1, 1, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[0]));
        EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);
        fAM = TRUE;
        if(fAM) /* AM */
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
        }
        /* display hour */
        sprintf((char*)szBuf, (char*)"%02d:%02d", 0, 0);
        EQS_TextOutStr(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, (BYTE *)szBuf); 
        s_wMinute = julian.minute;
        fChangeMinute = TRUE;
    }   

    //EQS_SetDrawMode(OldDrawMode);
}



LOCAL void SKY_API sDisplayAutoAnswerMode(void)
{   
    /* NEW 이미지 */
    EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
                  VM_RECORD_REC_NUM_IMAGE_YPOS,
                  &g_BmpAutoAnswerPannelNew);
    /* 점선 */
    EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                  VM_HORIZONTAL_LINE_YPOS,
                  &g_BmpHorizontalLine);
    
//    switch(SKY_GET_AUTO_ANSWER_GREETING())
//    {
//    case GM_GENERAL:   /* 일반 - 시스템 기본 */
//        EQS_PutBitmap(sAUTO_ANSWER_MODE_XPOS, sAUTO_ANSWER_MODE_YPOS, (BITMAP*)&g_BmpAutoAnswerPannelGeneral);
//        SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
//        break;
//
//    case GM_DRIVING:   /* 운전 중 */
//        EQS_PutBitmap(sAUTO_ANSWER_MODE_XPOS, sAUTO_ANSWER_MODE_YPOS, (BITMAP*)&g_BmpAutoAnswerPannelDrive);
//        SKY_SET_AUTO_ANSWER_GREETING(GM_DRIVING);
//        break;
//
//    case GM_MEETING:   /* 회의 중 */
//        EQS_PutBitmap(sAUTO_ANSWER_MODE_XPOS, sAUTO_ANSWER_MODE_YPOS, (BITMAP*)&g_BmpAutoAnswerPannelMeeting);
//        SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
//        break;
//
//    case GM_USER:      /* 사용자 음성 녹음 */
//        EQS_PutBitmap(sAUTO_ANSWER_MODE_XPOS, sAUTO_ANSWER_MODE_YPOS, (BITMAP*)&g_BmpAutoAnswerPannelUser);
//        SKY_SET_AUTO_ANSWER_GREETING(GM_USER);
//        break;
//
//    default :
//        EQS_PutBitmap(sAUTO_ANSWER_MODE_XPOS, sAUTO_ANSWER_MODE_YPOS, (BITMAP*)&g_BmpAutoAnswerPannelGeneral);
// 	      SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
//		  break;
//    }    
}
#endif //000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000






void SKY_API SpeedDial(UINT8 nShortDial)
{
    pb_sdial_type          SpeedDial;
    pb_number_info_type    NumberInfo;
    UINT8  nLen;
    BYTE  szString[40];
    //UINT8 nRealPhoneNumber;
    BYTE* number_type[] = 
	{ 
		(BYTE*)SF_PBOOK_CELL,
		(BYTE*)SF_PBOOK_HOME,
		(BYTE*)SF_PBOOK_WORK,
		(BYTE*)SF_PBOOK_PAGER
	};   
    
	SpeedDial.address = nShortDial;
	if( ui_get_pb( PB_SDIAL_I, &SpeedDial) )
	{
		NumberInfo.address = SpeedDial.dial_address;
		if( ui_get_pb(PB_NUMBER_INFO_I, &NumberInfo) )
		{
			sprintf((CHAR*) szString,(CHAR*) SKY_GET_TOKEN(TKN_PBOOK_FMT_NAME_TYPE),
                                              g_szPersonName[NumberInfo.book_address-1],\
										      number_type[NumberInfo.number_type-1]);
			nLen = STRLEN(szString) ;
			SET_ALPHAEDIT_BUFFER(szString, nLen);
			SET_ALPHAEDIT_POS(nLen);

			nLen = STRLEN(NumberInfo.digits);
			if(  NumberInfo.digits[0] == '0'   && \
			     NumberInfo.digits[1] == '0'   && \
				 NumberInfo.digits[2] >  '0'   && \
			     (nLen > 2)					   && \
			     SKY_IS_OVERSEAS_CALL_LOCK())
			{
				SET_NUMEDIT_BUFFER(NumberInfo.digits, nLen);
				SET_NUMEDIT_POS(nLen);

				//PBResult = FindNameWithNum(aPhoneNumber);
				//s_bDialing = TRUE;

				EQS_START_SIBLING(SKA_MainInterCode, FALSE);  
				return ;
			}               
			if(NumberInfo.digits[0] != '\0')
			{
				SET_NUMEDIT_BUFFER(NumberInfo.digits, nLen);
				SET_NUMEDIT_POS(nLen);                   
                EQS_START_SIBLING(SKA_StartOrigination, FALSE);
				return ;
			}   
		}
		else
		{
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NONVALID_SDIAL_MSG),\
                           MESSAGE_DEFAULT_TIME,\
                           MBT_NONE, MBT_HELP);	
		}
	}
}

LOCAL BOOL   l_fAutoAnswerRelease = FALSE;
LOCAL BOOL   l_fAutoAnswerFromNoSpace = FALSE;
LOCAL UINT8  l_nCount = 0;


#define sSLIDE_SHOW_INTERVAL        1000        /* 1 second */
#define sMESSAGE_SHOW_INTERVAL      2000        /* 2 second */
#define sIDLE_INTERVAL              500         /* */

//#define GET_IMG_X_IN_IDLE(Bmp)     ((SKY_SCREEN_WIDTH-((Bmp).Header.xWidth))/2)
//#define GET_IMG_Y_IN_IDLE(Bmp)     ((SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)-((Bmp).Header.Size.yHeight))/2)

#define sMAX_CDMA_TC_IN_IDEL        5           /* 5 count - 5 second */

void SKY_API SKA_MainIdleScreen(SKY_EVENT_T* pEvent)
{
    UINT8                       /*nRealPhoneNumberLength,*/ i;
    STATIC BOOL                 fDefence,  fPopupMessageBox;
    /* rosa 01-01-18 --> @why, what */
    DWORD                       current_time;
    clk_julian_type             julian;
    ALARM_DISP_TYPE_T           AlarmType;
    HCONTROL                    hCtrl, hControl;
    static HCONTROL             hIdlePicture = INVALID_CONTROL_HANDLE;
    STATIC   HCONTROL           hCtrlRelease, hCtrlFull, hCtrlStart;
    VMEMOINFO_T*                VoiceInfo;
	HFILE						hFile;
    DWORD                       dwTimeInfo = 0;
    STATIC   HCONTROL           hCtrlNoSrvc;
    static   BYTE *ppMyPhotoList[SKY_MAX_NUM_FILES];
    static   UINT8  s_nPhotoIndex, s_nPhoto;
    SKY_TOKEN                   AutoAns;
    STATIC s_aPhotoList[SKY_MAX_NUM_FILES][SKY_PATH_NAME_MAX_LENGTH];
    STATIC BITMAP s_Bitmap = {
        { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
            SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0,  },
        g_abBmpBuf
    };
    STATIC  BOOL                s_fNeedDrawNotify;
    STATIC  BOOL                s_fChangeAutoAnswerMode;
    STATIC BOOL                 s_fTrafficInIdleCount = 0;
	STATIC  BOOL                s_fFirstDraw;
    STATIC BOOL                 s_fEnableDraw;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        ASSERT(EQS_GetCurrentDCDepth() == 0);

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators 
            WA_SHOW_HEADER);//WA_SHOW_HEADER | WA_SHOW_CML);
         
        SKY_SubLCDDefault();
        /* 
        ** partial display후에 hs task에서 강제로 lcd를 update하는데 
        ** 이 때, annunciator 부분이 늦게 표시되는 경우가 있음.
        ** 그래서 강제로 처음에 무조건 그려줌.
        */
        //SKY_DrawAnnunciators();
		s_fFirstDraw = TRUE;
        s_fEnableDraw = TRUE;

        //EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);

        if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS || \
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
        SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

        SetCalendarTime( SKY_GetLocalTime() );
    	/* 자동응답 메시지 박스가 떠 있는 상태에서 SE_SYS_PACEMAKER를 */
        /* 받으면 메시지 박스 뒤에 DispClock 화면이 나타난다.         */
		/* 이를 방지하기 위해 불가피하게 flag를 사용합니다. T.T       */
		fPopupMessageBox = FALSE;
        
		/* 녹음/재생, 자동응답 후에는 VR_AUTO, VR_MEMO로 되어있는 경우가 */
		/* 발생해서 벨소리가 나지 않을 수 있습니다. 이를 제거하기 위해   */
		/* 추가했습니다.                                                 */
		gnVocoderMode = VR_NONE;

        s_fChangeAutoAnswerMode = TRUE;

        hIdlePicture = INVALID_CONTROL_HANDLE;
        s_nPhoto = s_nPhotoIndex = 0;

        /* jrkwon 2001.07.19 
        ** APP_START 앞쪽으로 옮겨서 이미지/애니메이션을 아예 시작하지 않도록
        ** 하는 것이 좋겠지만, 테스트가 좀 더 필요하므로
        ** B2에 적용해서 문제점을 파악해본다.
        */
        if(g_LastCallKeyEvent != SE_ERROR)
        {
            /* the condition that is going to go Dialing applet */
            if(g_LastCallKeyEvent >= SE_KEY_0 && g_LastCallKeyEvent <= SE_KEY_POUND)
            {
                /* send the last key event in CallEndDisp applet */
                EQS_SEND_EVENT(g_LastCallKeyEvent, EQS_NA, EQS_NA);

                /* clear the event */
                g_LastCallKeyEvent = SE_ERROR;

                DisplayCurrentDateTimeInSubLCD(FALSE);
                /* do not start idle screen */
                break;
            }

            /* clear the event */
            g_LastCallKeyEvent = SE_ERROR;
        }

        if(SKY_IS_AUTO_ANSWER() && (l_fAutoAnswerRelease == FALSE))
        {
            /* clear */
            SKY_SubLCDEndText(LN_LOWER);

            fDefence = TRUE;
            l_fAutoAnswerRelease = FALSE;
     
            VoiceInfo = GetVoiceInfo();

            if(IsSpace2RecordAutoAnswer())
            {
                InitAutoAnsDisp(SKY_GET_AUTO_ANSWER_GREETING());  //자동응답설정
                SKY_DisplayBellVibIcon();
                if(l_nCount == 0)
                {
                    l_nCount++;

                    if((SKY_IS_AUTO_ANSWER_FROM_POWERUP() == FALSE) && 
					   (l_fAutoAnswerFromNoSpace == FALSE))
                    {
                        hCtrlStart = EQC_NoBeepMessageBox((BYTE*)SKY_GET_TOKEN(TKN_AUTOANSWER_START),
                                                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
                        fPopupMessageBox = TRUE;
                    }
                    else if((SKY_IS_AUTO_ANSWER_FROM_POWERUP() == TRUE) ||
						    (l_fAutoAnswerFromNoSpace == TRUE))
                    {
                        SKY_SET_AUTO_ANSWER_FROM_POWERUP(FALSE);
                   		fPopupMessageBox = FALSE;
						l_fAutoAnswerFromNoSpace = FALSE;
                        fDefence = FALSE;
                    #ifdef FEATURE_SKY_SUBLCD
                        SKY_SubLCDDefault();
                    #endif //FEATURE_SKY_SUBLCD
                    }					
#ifdef FEATURE_SKY_SUBLCD
                    switch(SKY_GET_AUTO_ANSWER_GREETING())
                    {
                    case GM_GENERAL:   /* 일반 - 시스템 기본 */
                        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
                        break;

                    case GM_DRIVING:   /* 운전 중 */
                        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
                        break;

                    case GM_MEETING:   /* 회의 중 */
                        AutoAns = TKN_SLCD_AUTOANS_MEETING;
                        break;


                    case GM_USER:      /* 사용자 음성 녹음 */
                        AutoAns = TKN_SLCD_AUTOANS_USER;
                        break;

                    default :
                        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
		                break;
                    }
                    SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                                                    SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                                                    SKY_GET_TOKEN(AutoAns));

                    SKY_SubLCDEndText(LN_UPPER);
                    SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
                }
                else
                {
                    fDefence = FALSE;
                }
            }
            else
            {
#ifdef FEATURE_SKY_SUBLCD
                SKY_SubLCDTextOut(LN_UPPER, TKN_SLCD_NO_SPACE_4_RECORD_1);
                SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_NO_SPACE_4_RECORD_2);
                /* 2 초 후에 메시지 지운다. */
                EQS_StartTimer(EQS_TIMER_2, sMESSAGE_SHOW_INTERVAL, FALSE);

                //SKY_SubLCDEndText(LN_UPPER);
#endif//#ifdef FEATURE_SKY_SUBLCD
                fPopupMessageBox = FALSE;
                fDefence = FALSE;
				l_fAutoAnswerFromNoSpace = TRUE;
                InitAutoAnsDispNoSpace(SKY_GET_AUTO_ANSWER_GREETING());  //자동응답설정
                SKY_SET_AUTO_ANSWER_FROM_POWERUP(FALSE);
            }
        }
        else if((SKY_IS_AUTO_ANSWER() == FALSE) && l_fAutoAnswerRelease)
        {
            fDefence = TRUE;
			l_fAutoAnswerFromNoSpace = FALSE;
            SKY_DisplayBellVibIcon();
            SKY_SET_AUTO_ANSWER_FROM_POWERUP(FALSE);
            l_fAutoAnswerRelease = FALSE;
            hCtrlRelease =  EQC_NoBeepMessageBox((BYTE*)SKY_GET_TOKEN(TKN_AUTOANSWER_END),
                                                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
			fPopupMessageBox = TRUE;

#ifdef FEATURE_SKY_SUBLCD
            //SKY_SubLCDEndText(LN_UPPER);
            
            SKY_SubLCDTextOut(LN_UPPER, TKN_SLCD_CLEAR_AUTO_ANSWER);
            SKY_SubLCDEndText(LN_UPPER);
            SKY_SubLCDEndText(LN_LOWER);
#endif//#ifdef FEATURE_SKY_SUBLCD
        }
        else
        {
            fDefence = FALSE;
		    l_fAutoAnswerFromNoSpace = FALSE;
            l_fAutoAnswerRelease = FALSE;
            SKY_SET_AUTO_ANSWER_FROM_POWERUP(FALSE);

		    memset(&s_Bitmap.Header, NULL, sizeof(s_Bitmap.Header));

		    if(SKY_GET_IDLE_SCREEN() == IS_PICTURE)  
		    {
			    if(IS_MY_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))    // 나만의 그림
			    {
				    if(SKY_OpenDataFile(SDF_IMAGE, SKY_GET_IDLE_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
                    {
					    SKY_ReadDataBlock(hFile, &s_Bitmap.Header, sizeof(s_Bitmap.Header));

					    if(IS_EQUS_IMAGE((&(s_Bitmap.Header))) || IS_EQUS_X_IMAGE(&s_Bitmap.Header))
					    {
						    SKY_ReadDataBlock(hFile, (void*)s_Bitmap.pbData, GET_IMAGE_BYTE_SIZE(s_Bitmap.Header));
						    SKY_CloseDataFile(hFile);
                            //s_Bitmap.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
						    hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), &s_Bitmap);
					    }
					    else if(IS_EQUS_ANIMATION((&(s_Bitmap.Header))) || IS_EQUS_X_ANIMATION(&s_Bitmap.Header))
					    {
						    ANIMATION_HEADER_T AniHeader;
						    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                            COORD yImageHeight;

						    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
						    SKY_CloseDataFile(hFile);
						    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_IDLE_PICTURE().szFileName);
						    hIdlePicture = EQC_Animation(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), NULL, abBuf);

						    //EQC_SetStartYAnimation(hIdlePicture, SKY_ANNUNCIATORS_HEIGHT);
                            if(s_Bitmap.Header.Size.yHeight > SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT))
                                yImageHeight = SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)+1;
                            else
                                yImageHeight = s_Bitmap.Header.Size.yHeight;
						    EQC_SetAnimationHeight(hIdlePicture, yImageHeight);

                            //if(SKY_IS_PHONE_OPEN())
						    //    EQC_StartAnimation(hIdlePicture, TRUE);
					    }
                    }
			    }
#ifdef FEATURE_CHARACBELL
                else if(IS_CHARAC_PICTURE(SKY_GET_IDLE_PICTURE().wPictureID))    // 나만의 그림
			    {
				    if(SKY_OpenDataFile(SDF_CHA_IMAGE, SKY_GET_IDLE_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
                    {
					    SKY_ReadDataBlock(hFile, &s_Bitmap.Header, sizeof(s_Bitmap.Header));

					    if(IS_EQUS_IMAGE((&(s_Bitmap.Header))) || IS_EQUS_X_IMAGE(&s_Bitmap.Header))
					    {
						    SKY_ReadDataBlock(hFile, (void*)s_Bitmap.pbData, GET_IMAGE_BYTE_SIZE(s_Bitmap.Header));
						    SKY_CloseDataFile(hFile);
                            //s_Bitmap.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
						    hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), &s_Bitmap);
					    }
					    else if(IS_EQUS_ANIMATION((&(s_Bitmap.Header))) || IS_EQUS_X_ANIMATION(&s_Bitmap.Header))
					    {
						    ANIMATION_HEADER_T AniHeader;
						    BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];
                            COORD yImageHeight;

						    SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
						    SKY_CloseDataFile(hFile);
						    sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_IDLE_PICTURE().szFileName);
						    hIdlePicture = EQC_Animation(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), NULL, abBuf);

						    //EQC_SetStartYAnimation(hIdlePicture, SKY_ANNUNCIATORS_HEIGHT);
                            if(s_Bitmap.Header.Size.yHeight > SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT))
                                yImageHeight = SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT)+1;
                            else
                                yImageHeight = s_Bitmap.Header.Size.yHeight;
						    EQC_SetAnimationHeight(hIdlePicture, yImageHeight);

                            //if(SKY_IS_PHONE_OPEN())
						    //    EQC_StartAnimation(hIdlePicture, TRUE);
					    }
                    }
			    }
#endif // FEATURE_CHARACBELL
                else if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID)) // 나만의 사진 - 슬라이드
                {
                    BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

                    s_nPhoto = (UINT8)SKY_GetFileNumberInDataFile(SDF_PHOTO);

                    for(i=0; i < s_nPhoto; i ++)
                        ppMyPhotoList[i] = (BYTE*)s_aPhotoList[i];
    //                        ppMyPhotoList[i] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);


                    s_nPhoto = SKY_GetFileExtList(SKY_PHOTO_DIR, ppMyPhotoList);
                    s_nPhotoIndex = 0;

				    if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[s_nPhotoIndex], &hFile, SFT_READ) == SDR_OK)
                    {
					    SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                        SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
					    SKY_ReadDataBlock(hFile, &s_Bitmap.Header, sizeof(s_Bitmap.Header));  
					    SKY_ReadDataBlock(hFile, (void*)s_Bitmap.pbData, GET_IMAGE_BYTE_SIZE(s_Bitmap.Header));   
					    SKY_CloseDataFile(hFile); 

                        //s_Bitmap.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
					    hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), &s_Bitmap);
                    }
                    // jrkwon : 2001.05.20 start timer only if phone is open
                    if(SKY_IS_PHONE_OPEN())
                        EQS_StartTimer(EQS_TIMER_1, sSLIDE_SHOW_INTERVAL, TRUE);
                }
                else if(IS_MY_PHOTO(SKY_GET_IDLE_PICTURE().wPictureID)) // 나만의 사진
			    {
				    BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

				    if(SKY_OpenDataFile(SDF_PHOTO, SKY_GET_IDLE_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
                    {
					    SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                        SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
					    SKY_ReadDataBlock(hFile, &s_Bitmap.Header, sizeof(s_Bitmap.Header));  
					    SKY_ReadDataBlock(hFile, (void*)s_Bitmap.pbData, GET_IMAGE_BYTE_SIZE(s_Bitmap.Header));   
					    SKY_CloseDataFile(hFile); 

                        //s_Bitmap.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
					    hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), &s_Bitmap);
                    }
			    }
			    else if(!IS_PICTURE_NOT_USED(SKY_GET_IDLE_PICTURE().wPictureID))
			    {
			        hIdlePicture = EQC_Animation(0, 0, g_pBuiltInAnimation[SKY_GET_IDLE_PICTURE().wPictureID], NULL);

			        //EQC_SetStartYAnimation(hIdlePicture, SKY_ANNUNCIATORS_HEIGHT);
			        EQC_SetAnimationHeight(hIdlePicture, SKY_SCREEN_HEIGHT-(SKY_ANNUNCIATORS_HEIGHT+SKY_COMMANDLINE_HEIGHT));
                    //if(SKY_IS_PHONE_OPEN())
    				//    EQC_StartAnimation(hIdlePicture, TRUE);
			    }
		    }
            //if(SKY_IsKeyBufferEmpty() == FALSE)
            {
                EQS_StartTimer(EQS_TIMER_3, sIDLE_INTERVAL, FALSE);
                s_fEnableDraw = FALSE;
            }
        }
        DisplayCurrentDateTimeInSubLCD(FALSE);
        s_fNeedDrawNotify = TRUE;
        break;
        
    case SE_TM_TIMER3 :
        s_fEnableDraw = TRUE;
        
        if(SKY_IS_PHONE_OPEN() && (EQC_GET_TYPE(hIdlePicture) == CT_ANIMATION))
        {
		    EQC_StartAnimation(hIdlePicture, TRUE);
        }
        break;

    case SE_APP_RESUME:
        ASSERT(EQS_GetCurrentDCDepth() == 1);
        l_fAutoAnswerRelease = FALSE;
        s_fChangeAutoAnswerMode = TRUE;
        EQS_END();

        break;

    /* 
    ** 폴더를 닫으면 default event handler에서 POP_TO_ROOT 한다.
    ** 따라서 일단 폴더를 닫으면, 현재 이 애플릿을 종료하므로 애니메이션은 자동 종료한다.
    ** sWhereToGo함수에 의해 다시 이 애플릿으로 진입하면 폴더가 닫혀있으므로 애니메이션은 시작되지 않는다.
    ** 이 상태에서 폴더를 열면, 아래 이벤트 핸들러에 의해 애니메이션이 시작된다.
    */

    case SE_TM_TIMER1:
        if(s_nPhoto && !SKY_IS_AUTO_ANSWER())
        {
            BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

            if(hIdlePicture != INVALID_CONTROL_HANDLE)
                EQS_DeleteControl(hIdlePicture);

            s_nPhotoIndex ++;
            if(s_nPhotoIndex >= s_nPhoto)
                s_nPhotoIndex = 0;

			if(SKY_OpenDataFile(SDF_PHOTO, ppMyPhotoList[s_nPhotoIndex], &hFile, SFT_READ) == SDR_OK)
            {
				SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
				SKY_ReadDataBlock(hFile, &s_Bitmap.Header, sizeof(s_Bitmap.Header));  
				SKY_ReadDataBlock(hFile, (void*)s_Bitmap.pbData, GET_IMAGE_BYTE_SIZE(s_Bitmap.Header));   
				SKY_CloseDataFile(hFile); 

                //s_Bitmap.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
				hIdlePicture = EQC_Image(GET_IMG_X_IN_IDLE(s_Bitmap), GET_IMG_Y_IN_IDLE(s_Bitmap), &s_Bitmap);
            }
            else
                hIdlePicture = INVALID_CONTROL_HANDLE;
        }
        break;

    case SE_TM_TIMER2:
        /* 폴더 닫힌 상태에서 녹음키를 누를 경우 "EARPHONE연결"이라는 메시지를 2초간 보인 후*/
        /* 녹음 공간 부족 2초간 표시 후 */
        /* Default Sub LCD로 보이게 하기 위해서                                             */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        break;

    case SE_KEY_PHONE_OPEN:
		if(SKY_IS_AUTO_ANSWER())
		{
			s_fChangeAutoAnswerMode = TRUE;
            EQC_StartAnimation(EQS_GetNthIOC(AAD_ANIMATION), TRUE);
		}
        // jrkwon : 2001.05.20 --> start timer only if phone is open
        else if(IS_MY_PHOTO_ALL(SKY_GET_IDLE_PICTURE().wPictureID)) // 나만의 사진 - 슬라이드
        {
            EQS_StartTimer(EQS_TIMER_1, sSLIDE_SHOW_INTERVAL, TRUE);
        }
        // <--
        else if(hIdlePicture != INVALID_CONTROL_HANDLE && (EQC_GET_TYPE(hIdlePicture) == CT_ANIMATION)) /* only if idle animation is valid */
    		EQC_StartAnimation(hIdlePicture, TRUE);

        s_fNeedDrawNotify = TRUE;
        break;

    case SE_DTR_DISPLAY_CHANGE_F:   // 착신모드 화면 변경
        if(SKY_GET_DTR_DISP() == VOICE_INCOMMING)
            l_fDTR = FALSE;
        else
            l_fDTR = TRUE;

        ClearViewPort();
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);      
        break;

    case SE_SYS_PACEMAKER:
//        if((SKY_IS_AUTO_ANSWER() == FALSE) && (fPopupMessageBox == FALSE))
         /* AUTO_ANSWER가 TRUE일 경우에도 날려주어야 */
         /* 시간 갱신이 됩니다.    hjcho 01.06.23    */
        if(fPopupMessageBox == FALSE)
        {       
            /* display date and time */
            //if(DisplayDateTime(FALSE))
            /* 
            ** jrkwon : 2001-03-27 
            ** do not call draw function otherwise APP_DRAW 
            */
            if(sChangeMinute())
            {
                ClearViewPort();
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);                
            }
        }
#ifndef WIN32
        /* jrkwon 2001.08.29 --> */
        if(MAIN_STATE( cdma.curr_state ) == CDMA_TC)
        {
            s_fTrafficInIdleCount++;
            if(s_fTrafficInIdleCount == sMAX_CDMA_TC_IN_IDEL)
                ui_end_call();
        }
        else
        {
            /* reset */
            s_fTrafficInIdleCount = 0;
        }
        /* <-- */
#endif
        EQS_DEF_EVENT_PROC(pEvent);
        //EQS_PASS_CURRENT_EVENT();
        break;

    case SE_APP_DRAW :
        /* rosa 01-03-07 --> @why, what */
        /* check alarm */
        if(l_fActiveAlarm == FALSE)
        {
            current_time = SKY_GetLocalTime();
            clk_secs_to_julian( current_time, &julian );
            sFindAlarmOn( &julian, &AlarmType );
            if( AlarmType.Type > 0 )
            {
                EQS_START_CHILD_EVENT( sSKA_PlayAlarm, SE_APP_START, AlarmType.Type, AlarmType.nAddr );
            }                
        }

		if( s_fFirstDraw )
		{
			DRAW_MODE OldDrawMode;

			s_fFirstDraw = FALSE;
			OldDrawMode = EQS_GetDrawMode();
            EQS_SetDrawMode(DM_OR);
			sDrawMainIdleCML();
			EQS_SetDrawMode(OldDrawMode);			
    		EQS_SEND_EVENT( SE_APP_DRAW, EQS_DRAW_LEVEL_APPLET_ONLY, 0);
			break;
		}
		s_fFirstDraw = FALSE;
        
        /* */
        //if((SKY_IsKeyBufferEmpty() == FALSE) && (s_fEnableDraw == FALSE))
        //    break;
        //if(s_fEnableDraw == FALSE)
        //    break;

        /* 자동응답설정 상태 화면에서 애니메이션이 동작할 경우          */
        /* 400msec마다 SE_APP_DRAW이벤트가 발생한다.                    */
        /* 따라서 400msec마다 현재시간과 자동응답모드를 새로            */
        /* 그려주는데 command가 과도하게 발생해서 Phone이 죽는 경우가   */
        /* 발생한다. 이를 방지하기 위해서 sChangeMinute()와             */
        /* 변수 s_fChangeAutoAnswerMode를 이용해서 시간이 변경된 경우와 */
        /* 모드를 다시 그려주어야 될 경우에만 다시 그려주도록 한다.     */
        /* hjcho 01.10.04                                               */
        if(SKY_IS_AUTO_ANSWER())
		{	
            
#if 0 //0000000000000000000000000000000000000000000000000000000000000000000000            
            /* EQS_DRAW_LEVEL_ALL을 할 경우에는 버퍼가 clear되기 때문에 */
            /* 현재 시간이 그려지지 않는 경우가 발생한다. 이를 방지하기 */
            /* 위해 SE_APP_DRAW와 함께 날아오는 wParam을 검사해서       */
            /* EQS_DRAW_LEVEL_ALL일 경우에는 다시 그려준다.             */
            /* hjcho 01.10.20                                           */

//            if(pEvent->wParam == EQS_DRAW_LEVEL_ALL)
//            {
//                s_fChangeAutoAnswerMode = TRUE;
//            }            
//            
//            if(sChangeMinute() || (s_fChangeAutoAnswerMode == TRUE))
//            {
//                //s_fChangeAutoAnswerMode = TRUE;
//                //sDisplayAutoAnswerCurrentTime(TRUE);
//                sDrawMainIdleCML();
//            }
//
//            if(s_fChangeAutoAnswerMode == TRUE)
//            {
//                //sDisplayAutoAnswerMode();
//                //sDrawMainIdleCML();
//                s_fChangeAutoAnswerMode = FALSE;
//            }
#endif //0000000000000000000000000000000000000000000000000000000000000000000000

            if(sChangeMinute())
            {
                UpdateCurrentDateandTimeInAutoAnswer();
            }
            sDrawMainIdleCML();
            /* 
            ** partial display후에 hs task에서 강제로 lcd를 update하는데 
            ** 이 때, annunciator 부분이 늦게 표시되는 경우가 있음.
            ** 그래서 강제로 처음에 무조건 그려줌.
            */
			if( pEvent->wParam != EQS_DRAW_LEVEL_ALL )
			{
				SKY_DrawAnnunciators();
			}
		}
        else
        {
            /* <-- */
            /* jrkwon : 2001.06.15 --> move to after checking l_fActiveAlarm */
            DisplayDateTime(TRUE);           

            /* 
            ** partial display후에 hs task에서 강제로 lcd를 update하는데 
            ** 이 때, annunciator 부분이 늦게 표시되는 경우가 있음.
            ** 그래서 강제로 처음에 무조건 그려줌.
            */
			if( pEvent->wParam != EQS_DRAW_LEVEL_ALL )
			{
				SKY_DrawAnnunciators();
			}
        }
        /* 폴더 개폐시에만 idle screen이 모두 그려졌는지를 hs에 알려준다 */
        if(s_fNeedDrawNotify)
        {
            /* 
            ** partial display후에 hs task에서 강제로 lcd를 update하는데 
            ** 이 때, annunciator 부분이 늦게 표시되는 경우가 있음.
            ** 그래서 강제로 처음에 무조건 그려줌.
            */

            //SKY_DrawAnnunciators();

            SKY_IdleScreenNotify();
            s_fNeedDrawNotify = FALSE;
        }
        break;

//    case SE_TM_TIMER:
//        l_fActiveAlarm = FALSE;
//        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
//        break;

    case SE_APP_END:        
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif//#ifdef FEATURE_SKY_SUBLCD
/*
        if(s_nPhoto > 0) // 나만의 사진 - 슬라이드
        {
            for(i=0; i < s_nPhoto; i ++)
                SKY_Free(ppMyPhotoList[i]);
        }
*/
        EQS_ReleaseDC();

        ASSERT(EQS_GetCurrentDCDepth() == 0);

        break;

    case SE_KEY_EAR_LONG_CLICK:
	case SE_KEY_SEND:
		if( TransferLastDialInfo2CurrentDialInfo() && 
            (SKY_IS_CALL_STACK_LOCK() == FALSE))
		{
            EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        }
		break;

    /* This event comes from BYTE EIF_Ori(BYTE* pszCommand) in ska_eif.c */
    case SE_ORI_CALL:
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_KEY_POUND:
    case SE_KEY_STAR:
    case SE_KEY_0:
    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:
        l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_CURRENT_EVENT(SKA_Dialing);
        break;       
    
    case SE_KEY_POUND_LONG:
        EQS_End();
        break;
    
	case SE_KEY_F2_LONG:
        l_fAutoAnswerRelease = FALSE;
//#ifdef FEATURE_WAP
//        EQS_START_CHILD( SKA_MainWAP );
//#else //#ifdef FEATURE_WAP    
#ifdef FEATURE_WAP
        if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
		{
            if(SKY_IS_WAP_LOCK())
                EQS_START_CHILD(SKA_MainNtopCode);      //white 2001.02.26
            else
                EQS_START_CHILD( SKA_MainWAP );        //잠금 기능때문에 SKA_MainWAP를 한번 타고 WAP으로 들어간다.
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
#endif //FEATURE_WAP
//#endif //#ifdef FEATURE_WAP
        break;
        
    case SE_KEY_F2:
    case SE_SOFTKEY_WAP:
		l_fAutoAnswerRelease = FALSE;
    
#ifndef WIN32
        if(SKY_IS_WAP_LOCK())
        {
            EQS_START_CHILD_EVENT(SKA_MainNtopCode, SE_APP_START, NATE_CODE_CHOOSE_MENU, EQS_NA);
        }
        else
        {
            EQS_START_SIBLING( SKA_ChooseMenu, FALSE); 
        }
#endif
		break;

	case SE_KEY_F1_LONG:
    case SE_KEY_F1:	
    case SE_SOFTKEY_SMS:
        l_fAutoAnswerRelease = FALSE;
    //case SE_KEY_F1:
#ifndef WIN32
		EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_KEY, EQS_NA);
#else // for test
        //EQS_START_CHILD_CURRENT_EVENT(SKA_MainSMS);
		EQS_START_CHILD(SKA_SMSMenu);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_INFORM:
#endif // #ifdef FEATURE_SKTT_STI_LOCATION
    case SE_SMS_MESSAGE:
        l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		EQS_START_CHILD_CURRENT_EVENT(SKA_MainSMS);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_TIMEOUT:
		l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		CheckCurrentTime();
#endif //
		break;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
#ifdef FEATURE_SKVM
    case SE_SKVM_INPUT_DATA:
        SKY_MotorTime(800);
		EQS_SetDirectDraw(TRUE);
        EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_APP_START,EQS_NA,INIT_SKVM_INPUTDATA ,FALSE);
		break;	
#endif
    case SE_KEY_CENTER:
    case SE_SOFTKEY_MENU:
        l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainMenuTop);  //redstar
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainCameraCode);
#else
        EQS_START_CHILD(SKA_MainMenuCode);          //white  
#endif //FEATURE_CALL_STACK_SEPERATED
        break;

    case SE_KEY_UP:
        l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainCallList); 
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED        
        EQS_START_CHILD(SKA_MainStackCode);
		break;
        
    case SE_KEY_DOWN:
        l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(TRUE);
        EQS_START_CHILD(SKA_MainStackCode);
#else
        EQS_START_CHILD(SKA_MainCameraCode);
#endif//FEATURE_CALL_STACK_SEPERATED        
        break;

	case SE_KEY_RIGHT:
        l_fAutoAnswerRelease = FALSE;
//#if (MODEL_ID == MODEL_B1)// youandi 00.01.10 lcd contrast test
//#ifdef WIN32
//        MakeScreenDarker(1);
//        EQS_UpdateDisplay();
//#else
//		if(lcdlvl>0){
//			SKY_SetLCDContrastTest(1);//hs_lcd_contrast (1);	// 어둡게 
//			--lcdlvl;
//		}
//#endif
        EQS_START_CHILD(SKA_MainPbookCode); //redstar
        break;

    case SE_KEY_LEFT:
        l_fAutoAnswerRelease = FALSE;
//#ifdef WIN32
//        MakeScreenLighter(1);
//        EQS_UpdateDisplay();
//#else
//		if(lcdlvl<31){
//			SKY_SetLCDContrastTest(0x21);//hs_lcd_contrast (0x21);	// 밝게 	
//			++lcdlvl;
//		}
//#endif
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainMenuCode);          //white
#else
        EQS_START_CHILD(SKA_MainSchedule);
#endif // FEATURE_CALL_STACK_SEPERATED
        break;
//#endif// youandi 00.01.10 lcd contrast test
    case SE_DM_IN_PROCESSING:
        EQS_START_CHILD(SKA_IN_DataManager);
        break;

    case SE_DM_UPDATE_IDLE://white 2001.06.12 for DataManager Schedule
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_DM_MO_CMD_F:        // 2001.02.09 Send Mo in DataManager.
		l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA, TI_DEFAULT);
        break;


    case SE_KEY_RECORD_LONG:
        /* 폴더가 닫힌 상태에서는 녹음/재생이 작동하지 않도록 수정 */
        /* 01.06.19 hjcho             Ear Jack이 꽂혔을 경우 제외  */
        if(SKY_IS_EAR_JACK_ON())
        {
            l_fAutoAnswerRelease = FALSE;
            //white  
            if(SKY_IS_RECORD_PLAY_LOCK())            
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode);
            else
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
            //white 
        }
        else
        {
#ifdef FEATURE_SKY_SUBLCD
          SKY_SubLCDTextOutStr(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_IN_IDLE_EARPHONE)); 
          SKY_SubLCDEndText(LN_UPPER);
          SKY_SubLCDEndText(LN_LOWER);
          EQS_StartTimer(EQS_TIMER_2, sMESSAGE_SHOW_INTERVAL, FALSE);
#endif //FEATURE_SKY_SUBLCD
        }
        break;

    case SE_KEY_RECORD:
    case SE_KEY_PLAY:
        /* 폴더가 닫힌 상태에서는 녹음/재생이 작동하지 않도록 수정 */
        /* 01.06.19 hjcho                                          */
        if(SKY_IS_PHONE_OPEN() || SKY_IS_EAR_JACK_ON())
        {
            l_fAutoAnswerRelease = FALSE;
           /* 팝업메뉴나 메시지박스가 뜬 상태에서 녹음키를 누르는 경우 */
           /* 녹음/재생화면으로 넘어가지 않는 경우 방지                */
           if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
           {
               /* Sub LCD에 글자가 나올 수 있도록 */
               if(SKY_IS_AUTO_ANSWER() == FALSE)
               {
                 l_nCount = 0;
               }

               SKY_SubLCDEndText(LN_UPPER);
               SKY_SubLCDEndText(LN_LOWER);               
               EQS_DeleteControl(hControl);
           }

            //white 
            if(SKY_IS_RECORD_PLAY_LOCK())// && SKY_IS_PHONE_OPEN())
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode );
            else
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
            //white 
        }
        break;

    /* Ignore this event */
    case SE_KEY_AUTO_ANSWER:
        break;

    case SE_KEY_RELEASE_AUTO_ANSWER:
        if(SKY_IS_AUTO_ANSWER() && SKY_IS_PHONE_OPEN())
        {
            UpdateAutoAnsDisp(SKY_GET_AUTO_ANSWER_GREETING());  //자동응답설정상태를 바꾸는 부분
            s_fChangeAutoAnswerMode = TRUE;
        }
        break;


//    case SE_TM_TIMER1:
//        /* 자동응답 설정 시 소리와 함께 Motor 진동이 발생하는 부분 */
//        l_fAutoAnswerRelease = FALSE;
//        if(!SKY_IS_HANDS_FREE())  SKY_MotorTime(400);
//        EQS_END();
//        break;

    case SE_KEY_AUTO_ANSWER_LONG:
        if(fDefence == FALSE)
        {
          if(SKY_IS_AUTO_ANSWER() == FALSE)
          {
            if(SKY_IS_ETIQUETTE())
			{
			  if(!SKY_IS_HANDS_FREE()) SKY_MotorTime(800);

              SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;
              EQS_END();
			}
			else
			{		
  	          SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;                
#ifndef WIN32
              if(SKY_GET_AUTO_ANSWER_BELL_TIME() != AAB_VIB_ONCE)
              {                
                SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_START);
              }
              else
#endif //WIN32
              {
                if(!SKY_IS_ETIQUETTE() && !SKY_IS_HANDS_FREE()) SKY_MotorTime(800);
              }

              EQS_END();
			}		
          }
          else 
          {
            if(SKY_IS_ETIQUETTE() == FALSE)
			{
			  //SKY_PlaySound(UI_ALERT_SND, SND_ANSWER);
              //SKY_PlaySoundEx( UI_MSG_SND, SND_PIP_TONE, 5, 1);
#ifndef WIN32
              SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_END);
#endif
			}

    		SKY_SET_AUTO_ANSWER(FALSE);
            l_fAutoAnswerRelease = TRUE;
			EQS_END();			
          }
        }
        break;

    case SE_SYS_TO_IDLE:
    case SE_KEY_CLEAR:
        // Should be ignored.
        break;

    case SE_KEY_END:
#ifdef FEATURE_UI_DORMANT
        if(SKY_IS_DORMANT())
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
            SetUIPktRelease();
        }
#endif
        break;

    case SE_IO_MESGBOX_END:   
        if(SKY_IS_AUTO_ANSWER())
        {
            s_fChangeAutoAnswerMode = TRUE;
        }

		fPopupMessageBox = FALSE;
        fDefence = FALSE;
        if(pEvent->wParam == hCtrlRelease)
        {
          l_nCount = 0;
          l_fAutoAnswerRelease = FALSE;
          EQS_END();
        }
        else if(pEvent->wParam == hCtrlStart)
        {          
#ifdef FEATURE_SKY_SUBLCD
          SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        }
#ifdef FEATURE_WAP
        else if(pEvent->wParam == hCtrlNoSrvc)
		{
		   EQS_END();
		}
#endif //#ifdef FEATURE_WAP
        break;

        /* undersea 2001-08-20 --> @IDLE상태에서 At Command(Exec) 실행  */
#ifdef FEATURE_GVM
    case SE_TONETGAME:
        EQS_START_CHILD(SKA_MainGVM);
        break;
#endif
        /* <-- */

    /* 
    ** jrkwon 2001.12.05 통화연결음 들리는 순간 종료키를 여러번 눌러 종료하는 경우에
    ** CALL_COMMAND_ERROR 이벤트가 여러번 연속해서 UI로 전달된다.
    ** 이 이벤트는 EQS_DEF_EVENT_PROC에서 처리되는데, 매번 POP_TO_ROOT하기 때문에
    ** 그 때마다 SKA_MainIdle ..sWhereTogo ..SKA_MainIdleScreen가 순차적으로 실행되며
    ** 이 때문에 화면의 휴대폰이름과 시계가 깜박거리는 문제가 생긴다.
    */
    case SE_CALL_COMMAND_ERROR :
        break;

    default:

        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




void SKY_API SKA_IdleDebugScreen(SKY_EVENT_T* pEvent)
{
    STATIC BOOL                 fDefence,  fPopupMessageBox;
    DWORD                       current_time;
    clk_julian_type             julian;
    ALARM_DISP_TYPE_T           AlarmType;
    HCONTROL                    hCtrl, hControl;
    STATIC   HCONTROL           hCtrlRelease, hCtrlFull, hCtrlStart;
    VMEMOINFO_T*                VoiceInfo;
    STATIC   HCONTROL           hCtrlNoSrvc;

	STATIC  UINT8              nPreviousMinute;   
	DWORD                      dwTime;
    

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        ASSERT(EQS_GetCurrentDCDepth() == 0);

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators 
            WA_SHOW_HEADER);//WA_SHOW_HEADER | WA_SHOW_CML);
        
        SKY_SubLCDDefault();

        SetCalendarTime( SKY_GetLocalTime() );
    	/* 자동응답 메시지 박스가 떠 있는 상태에서 SE_SYS_PACEMAKER를 */
        /* 받으면 메시지 박스 뒤에 DispClock 화면이 나타난다.         */
		/* 이를 방지하기 위해 불가피하게 flag를 사용합니다. T.T       */
		fPopupMessageBox = FALSE;
        
		/* 녹음/재생, 자동응답 후에는 VR_AUTO, VR_MEMO로 되어있는 경우가 */
		/* 발생해서 벨소리가 나지 않을 수 있습니다. 이를 제거하기 위해   */
		/* 추가했습니다.                                                 */
		gnVocoderMode = VR_NONE;
        
        DisplayCurrentDateTimeInSubLCD(FALSE);

		dwTime = SKY_GetLocalTime();
		clk_secs_to_julian(dwTime, &julian);
		nPreviousMinute = julian.minute;
        // fall through.

    case SE_APP_RESUME:
        ASSERT(EQS_GetCurrentDCDepth() == 1);
        SKY_ReinitMemory();

        EQS_StartTimer(EQS_TIMER_1, 1000, TRUE);

        /* 녹음/재생, 자동응답 후에는 VR_AUTO, VR_MEMO로 되어있는 경우가 */
		/* 발생해서 벨소리가 나지 않을 수 있습니다. 이를 제거하기 위해   */
		/* 추가했습니다.                                                 */
		gnVocoderMode = VR_NONE;

        // Update Display
        
        DisplayCurrentDateTimeInSubLCD(TRUE);
        break;
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
#ifndef WIN32
        /* jrkwon 2001.08.29 --> */
        if(MAIN_STATE( cdma.curr_state ) == CDMA_TC)
        {
            ui_end_call();
        }
#endif
        /* <-- */
		EQS_DEF_EVENT_PROC(pEvent);
		break;
    case SE_APP_DRAW :
        SKY_DrawDebugScreen();
        /* 
        ** partial display후에 hs task에서 강제로 lcd를 update하는데 
        ** 이 때, annunciator 부분이 늦게 표시되는 경우가 있음.
        ** 그래서 강제로 처음에 무조건 그려줌.
        */
        SKY_DrawAnnunciators();
        SKY_IdleScreenNotify();
        break;

    case SE_TM_TIMER1:
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_KEY_PHONE_OPEN:
        break;

    case SE_DTR_DISPLAY_CHANGE_F:   // 착신모드 화면 변경
        if(SKY_GET_DTR_DISP() == VOICE_INCOMMING)
            l_fDTR = FALSE;
        else
            l_fDTR = TRUE;

        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_APP, EQS_NA);      
        break;

    case SE_APP_END:        
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif//#ifdef FEATURE_SKY_SUBLCD

        EQS_ReleaseDC();

        ASSERT(EQS_GetCurrentDCDepth() == 0);

        break;

    case SE_KEY_EAR_LONG_CLICK:
	case SE_KEY_SEND:
		if( TransferLastDialInfo2CurrentDialInfo() && 
            (SKY_IS_CALL_STACK_LOCK() == FALSE))
		{
            EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        }
		break;

    /* This event comes from BYTE EIF_Ori(BYTE* pszCommand) in ska_eif.c */
    case SE_ORI_CALL:
        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
        break;

    case SE_KEY_POUND:
    case SE_KEY_STAR:
    case SE_KEY_0:
    case SE_KEY_1:
    case SE_KEY_2:
    case SE_KEY_3:
    case SE_KEY_4:
    case SE_KEY_5:
    case SE_KEY_6:
    case SE_KEY_7:
    case SE_KEY_8:
    case SE_KEY_9:
        l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_CURRENT_EVENT(SKA_Dialing);
        break;       
    
    case SE_KEY_POUND_LONG:
        EQS_End();
        break;
    
    case SE_KEY_F2:
    case SE_SOFTKEY_WAP:
        l_fAutoAnswerRelease = FALSE;
    //case SE_KEY_F2:
#ifdef FEATURE_WAP
        if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
		{
          if(SKY_IS_WAP_LOCK())
          EQS_START_CHILD(SKA_MainNtopCode);      //white 2001.02.26
          else
          EQS_START_CHILD( SKA_MainWAP );        //잠금 기능때문에 SKA_MainWAP를 한번 타고 WAP으로 들어간다.
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
#endif //#ifdef FEATURE_WAP
        break;
        
	case SE_KEY_F2_LONG:
		l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_WAP
        if(SKY_IS_WAP_LOCK())
        {
            EQS_START_CHILD_EVENT(SKA_MainNtopCode, SE_APP_START, NATE_CODE_CHOOSE_MENU, EQS_NA);
        }
        else
        {
            EQS_START_SIBLING( SKA_ChooseMenu, FALSE); 
        }
#endif  // #ifdef FEATURE_WAP
		break;

    case SE_KEY_F1:
    case SE_SOFTKEY_SMS:
        l_fAutoAnswerRelease = FALSE;
    //case SE_KEY_F1:
#ifndef WIN32
		EQS_START_CHILD_EVENT(SKA_MainSmsCode, SE_APP_START, SMSCODE_KEY, EQS_NA);
#else // for test
        EQS_START_CHILD_CURRENT_EVENT(SKA_SMSMenu);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_INFORM:
#endif // #ifdef FEATURE_SKTT_STI_LOCATION
    case SE_SMS_MESSAGE:
        l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		EQS_START_CHILD_CURRENT_EVENT(SKA_MainSMS);
#endif
        break;
#ifdef FEATURE_SKTT_EQUS_LOCATION
	case SE_LOCATION_TIMEOUT:
		l_fAutoAnswerRelease = FALSE;
#ifndef WIN32
		CheckCurrentTime();
#endif //
		break;
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
    case SE_KEY_CENTER:
    case SE_SOFTKEY_MENU:
        l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainMenuTop);  //redstar
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainCameraCode);
#else
        EQS_START_CHILD(SKA_MainMenuCode);          //white  
#endif //FEATURE_CALL_STACK_SEPERATED
        break;

    case SE_KEY_UP:
        l_fAutoAnswerRelease = FALSE;
        //EQS_START_CHILD(SKA_MainCallList); 
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(FALSE);
#endif//FEATURE_CALL_STACK_SEPERATED        
        EQS_START_CHILD(SKA_MainStackCode);
		break;
        
    case SE_KEY_DOWN:
        l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_CALL_STACK_SEPERATED
        SetReceivedCallStack(TRUE);
        EQS_START_CHILD(SKA_MainStackCode);
#else
        EQS_START_CHILD(SKA_MainCameraCode);
#endif//FEATURE_CALL_STACK_SEPERATED        
        break;

	case SE_KEY_RIGHT:
        l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD(SKA_MainPbookCode); //redstar
        break;

    case SE_KEY_LEFT:
        l_fAutoAnswerRelease = FALSE;
#ifdef FEATURE_CALL_STACK_SEPERATED
        EQS_START_CHILD(SKA_MainMenuCode);          //white
#else
        EQS_START_CHILD(SKA_MainSchedule);
#endif // FEATURE_CALL_STACK_SEPERATED
        break;
//#endif// youandi 00.01.10 lcd contrast test
    case SE_DM_IN_PROCESSING:
        EQS_START_CHILD(SKA_IN_DataManager);
        break;

    case SE_DM_UPDATE_IDLE://white 2001.06.12 for DataManager Schedule
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_DM_MO_CMD_F:        // 2001.02.09 Send Mo in DataManager.
		l_fAutoAnswerRelease = FALSE;
        EQS_START_CHILD_EVENT(SKA_SMSSendMsg, SE_APP_START, EQS_NA, TI_DEFAULT);
        break;

    case SE_KEY_RECORD_LONG:
        l_fAutoAnswerRelease = FALSE;
        //white  
        if(SKY_IS_RECORD_PLAY_LOCK())
            EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode );
        else
            EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
        //white 
        break;

    case SE_KEY_RECORD:
    case SE_KEY_PLAY:
        if(SKY_IS_PHONE_OPEN())
        {
            l_fAutoAnswerRelease = FALSE;
           /* 팝업메뉴나 메시지박스가 뜬 상태에서 녹음키를 누르는 경우 */
           /* 녹음/재생화면으로 넘어가지 않는 경우 방지                */
           if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
           {
               /* Sub LCD에 글자가 나올 수 있도록 */
               if(SKY_IS_AUTO_ANSWER() == FALSE)
               {
                 l_nCount = 0;
               }

               SKY_SubLCDEndText(LN_UPPER);
               SKY_SubLCDEndText(LN_LOWER);               
               EQS_DeleteControl(hControl);
           }

            //white 
            if(SKY_IS_RECORD_PLAY_LOCK())// && SKY_IS_PHONE_OPEN())
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainRecCode );
            else
                EQS_START_CHILD_CURRENT_EVENT(SKA_MainVMemo);
            //white 
        }
        break;

    case SE_KEY_RELEASE_AUTO_ANSWER:

        if(SKY_IS_AUTO_ANSWER() && SKY_IS_PHONE_OPEN())
        {
            UpdateAutoAnsDispInDebugScreen(SKY_GET_AUTO_ANSWER_GREETING());  //자동응답설정상태를 바꾸는 부분    
        }
        break;

    case SE_KEY_AUTO_ANSWER_LONG:
        if(fDefence == FALSE)
        {
          if(SKY_IS_AUTO_ANSWER() == FALSE)
          {
            if(SKY_IS_ETIQUETTE())
			{
			  if(!SKY_IS_HANDS_FREE()) SKY_MotorTime(800);

              SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;
              EQS_END();
			}
			else
			{		
  	          SKY_SET_AUTO_ANSWER(TRUE);
              l_fAutoAnswerRelease = FALSE;                
#ifndef WIN32
              if(SKY_GET_AUTO_ANSWER_BELL_TIME() != AAB_VIB_ONCE)
              {                
                SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_START);
              }
              else
#endif //WIN32
              {
                if(!SKY_IS_ETIQUETTE() && !SKY_IS_HANDS_FREE()) SKY_MotorTime(800);
              }

              EQS_END();
			}		
          }
          else 
          {
            if(SKY_IS_ETIQUETTE() == FALSE)
			{
			  //SKY_PlaySound(UI_ALERT_SND, SND_ANSWER);
              //SKY_PlaySoundEx( UI_MSG_SND, SND_PIP_TONE, 5, 1);
#ifndef WIN32
              SKY_PlaySound(UI_MSG_SND, SND_AUDIO_APP_END);
#endif
			}

    		SKY_SET_AUTO_ANSWER(FALSE);
            l_fAutoAnswerRelease = TRUE;
			EQS_END();			
          }
        }
        break;

    case SE_SYS_TO_IDLE:
    case SE_KEY_CLEAR:
        // Should be ignored.
        break;

    case SE_KEY_END:
#ifdef FEATURE_UI_DORMANT
        if(SKY_IS_DORMANT())
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
            SetUIPktRelease();
        }
#endif
        break;

    case SE_IO_MESGBOX_END:        
		fPopupMessageBox = FALSE;
        fDefence = FALSE;
        if(pEvent->wParam == hCtrlRelease)
        {
          l_nCount = 0;
          l_fAutoAnswerRelease = FALSE;
          EQS_END();
        }
        else if(pEvent->wParam == hCtrlStart)
        {
#ifdef FEATURE_SKY_SUBLCD
          SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        }
#ifdef FEATURE_WAP
        else if(pEvent->wParam == hCtrlNoSrvc)
		{
		   EQS_END();
		}
#endif //#ifdef FEATURE_WAP
        break;

    default:

        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }

}
                             
#ifdef FEATURE_SKVM
void SKY_API SKA_PushSmsInSkvm(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
        EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_APP_START,EQS_NA,INIT_SKVM_INPUTDATA ,FALSE);
		break;
	default:
        EQS_DEF_EVENT_PROC(pEvent);        
		break;
	}
}

void SKY_API	SKA_PlaySkvmContents(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
		EQS_SetDirectDraw(TRUE);
        EQS_START_SIBLING_EVENT(SKA_MainSKVMBG, SE_APP_START,EQS_NA,EQS_NA,FALSE);
		break;
	default:
        EQS_DEF_EVENT_PROC(pEvent);        
		break;
	}
}

#endif           
LOCAL EQS_APPLET* SKY_API sWhereToGo(void)
{   
    /* re-initialize dynamic memory pool */
	EQS_ReinitMemory();
    SKY_ReinitMemory();
	InitBookListTable();
#ifdef FEATURE_SKVM
	if (SKY_IS_PUSH_SMS_IN_SKVM())
	{
		SKY_SET_PUSH_SMS_IN_SKVM(FALSE);
		return (EQS_APPLET *)SKA_PushSmsInSkvm;
	}
	else
#endif
    if(sIS_MESSAGE_INFO())
		return (EQS_APPLET *)sSKA_ShowMessage;

	if (GetMissedCallCount() > 0)
    {
       return (EQS_APPLET *)SKA_MissedCall;
    }
    else
    {
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            return (EQS_APPLET *)SKA_MainAllCode;
        }
        else 
        {
#ifdef FEATURE_SKVM
			if (SKY_GET_JAVAASBG())
			{
				return (EQS_APPLET *)SKA_PlaySkvmContents;
			}
			else 
#endif
            return SKY_IS_DEBUG_SCREEN() ? (EQS_APPLET *)SKA_IdleDebugScreen : (EQS_APPLET *)SKA_MainIdleScreen;
        }
    }
}

void SKY_API SKA_MainIdle(SKY_EVENT_T* pEvent)
{
    EQS_APPLET* NextApplet;
	STATIC HCONTROL hCtrlNoSrvc;
    HCONTROL hControl;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */
        NextApplet = sWhereToGo();
        EQS_START_CHILD(NextApplet);
        break;
    case SE_APP_END :
        break;

    case SE_APP_RESUME:
        if (pEvent->wParam == 0) // There is no pending event to be processed.
        {
            NextApplet = sWhereToGo();
            EQS_START_CHILD(NextApplet);
        }
        break;

    /* ignore draw event */
    case SE_APP_DRAW:
        break;

    /* EQC_X */
    case SE_SYS_TO_IDLE : /* very important */
        NextApplet = sWhereToGo();
        EQS_START_CHILD(NextApplet);
        break;

    
    case SE_SND_DTMF:
        ASSERT(FALSE);            
        break;


    /* EQC_KEY */
   case SE_KEY_PHONE_OPEN:   /* Earpiece raised */                          
             
        break;
    case SE_KEY_PHONE_CLOSE:  /* Earpiece Lowered */                          

        break;

    case SE_KEY_UP:           /* change volume */          
    case SE_KEY_DOWN:

        break;
    case SE_KEY_CLEAR:          /* clear back to idle screen */
    case SE_KEY_CLEAR_LONG:
    case SE_KEY_END:                          
        //fPrepend = FALSE;        
        //SKY_StopSound();
        break;

    case SE_SYS_OFFLINE: /*UI_OFFLISE_F:       go offline */
        SKY_SET_OFFLINE(TRUE);
        SKY_SaveNV();    // ui_nv_savestuff();

        SKY_TRACE( TL_HIGH, "UI has gone offline");
        break;
        
    case SE_DATA_CALL:
        SKY_SET_DATACALL(TRUE);
        ui_update_cm_call_info( &(GET_EVENT_PARAM()->call_event.call_info) ); 
        EQS_START_CHILD_CURRENT_EVENT(SKA_MainCall);
        break;

    case SE_CALL_INCOMING:/*UI_CALL_INCOMING_E:*/
/*  DefEventProc 으로 옮김
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
*/
		SKY_SET_ALERT(TRUE);   // ui.alert = TRUE;
        ui_update_cm_call_info( &(GET_EVENT_PARAM()->call_event.call_info) ); 

        /* jrkwon 2000-05-20 --> @why, what */
        EQS_START_CHILD_CURRENT_EVENT(SKA_MainCall);
        /* <-- */
        break;
        
    case SE_DM_IN_PROCESSING:
        EQS_START_CHILD(SKA_IN_DataManager);
        break;       

    case SE_CALL_ORIGINATED:/*UI_CALL_ORIGINATED_E:*/
		if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
		{
          SKY_SET_ORIGINATE(TRUE);    // ui.originate = TRUE;
          EQS_START_CHILD_CURRENT_EVENT(SKA_MainCall);
		}
		else if(SKY_IS_MAINTREQ())
		{
          SKY_EndCall();
		  SKY_SET_ORIGINATE(FALSE);
		  ClearNumEditInfo();
		  ClearAlphaEditInfo();
		  hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
		                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);          
		}
		else if(SKY_IS_NOSERVICE())
		{
		  SKY_EndCall();
		  SKY_SET_ORIGINATE(FALSE);
		  ClearNumEditInfo();
		  ClearAlphaEditInfo();
		  hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
		}
        break;
        
    case SE_CALL_ENDED:/*UI_CALL_ENDED_E: */
        // something else bring UI to idle (offline etc) then
        // this event happens. Ignore this event    
        break;
        
    case SE_CALL_COMMAND_ERROR:/*UI_CALL_CMD_ERR_E: */
        // some other tasks may initiate a call
        // and failed for some reason
        if (GET_EVENT_PARAM()->call_cmd_err.cmd == CM_CALL_CMD_ORIG) 
        {        
            if((SKY_IS_NOSERVICE() == TRUE) || (SKY_IS_MAINTREQ() == TRUE))
			{
			  SKY_SET_ORIGINATE(FALSE);
		      ClearNumEditInfo();
		      ClearAlphaEditInfo();
		      hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                                  MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
			}          
        }
		else
		{
		   EQS_END();
		}
       break;

    case SE_CALL_CALLER_ID://UI_CALL_CALLER_ID_E:
    case SE_CALL_SO_CHANGED://UI_CALL_SO_CHANGED_E:
    case SE_CALL_CONNECT: //UI_CALL_CONNECT_E:
    case SE_CALL_FLASHED: //UI_CALL_FLASHED_E:
        // the events are not supposed to
        // appear in idling state if the call is not SMS

        if( ! ISSMS(GET_EVENT_PARAM()->call_event.call_info.call_type) )      
        {
          ASSERT(FALSE);
        }

    case SE_IO_MESGBOX_END:
		if(pEvent->wParam == hCtrlNoSrvc)
		{
		   EQS_END();
		}
	   break;
    
    case SE_CALL_DISPLAY:
        EQS_START_CHILD(sSKA_ShowMessage);
       break;

    default:                

        EQS_DEF_EVENT_PROC(pEvent);        
        break;
    }
}

BOOL SKY_API SKY_GetActiveAlarmFlag(void)
{
	return l_fActiveAlarm;
}