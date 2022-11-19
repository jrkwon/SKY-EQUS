/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_bell.c
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
** 2000-12-21 woodstok     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"

#if (MODEL_ID == MODEL_B0)
    #include "Bell_ID.B0"
#elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
    #include "Bell_ID.B1"
#endif
  
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define NUM_BELL_MODE   4
#define NUM_BELL_TIME   4
#define NOT_AVAIL_MYBELL    0xFF
#define BELL_SCROLL_LIST_CHAR   16

#define SBV_SCROLL_LIST_STYLE   CS_SCROLL_LIST_DEFAULT| \
                                CS_SCROLL_LIST_COMBO | \
                                CS_SCROLL_LIST_ENUMBERATED 

#define SBV_SCROLL_LIST_SIZE    3

#define SBV_GET_SLSIZE(n)       (UINT8)((SBV_SCROLL_LIST_SIZE < n)?SBV_SCROLL_LIST_SIZE:n)
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

enum tag_SKA_BELL_VIB_E {
    SBV_BELL_TYPE_TITLE,
    SBV_BELL_ID,
    SBV_BELL_VOLUME_TITLE,
    SBV_BELL_VOLUME,
    SBV_BELL_MODE_TITLE,
    SBV_BELL_MODE,
    SBV_BELL_TIME_TITLE,
    SBV_BELL_TIME,
    SBV_MAX
};


enum tag_SKA_ALARM_BELL_E {
    SAB_BELL_TYPE_TITLE,
    SAB_BELL_ID,
    SAB_BELL_VOLUME_TITLE,
    SAB_BELL_VOLUME,
    SAB_MAX
};

enum tag_SKA_GROUP_BELL_E {
    SGB_GROUP_TITLE,
    SGB_GROUP,
    SGB_BELL_TYPE_TITLE,
    SGB_BELL_ID,
    SGB_MAX
};

#define SBV_BELL            0   // 착신벨
#define SAB_ALARM_BELL      1   // 알람벨
#define SAB_SCHEDULER_BELL  2   // 스케쥴러 알람
#define SGB_BELL            4   // 그룹벨
#define TWO_NUM_BELL        5   // 투넘버 벨

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

// 벨/진동 선택
CONST SKY_TOKEN aBell_Mode[NUM_BELL_MODE] =
{
    TKN_BELLVIB_MODE_BELL,
    TKN_BELLVIB_MODE_VIB,
    TKN_BELLVIB_MODE_BELL_AFTER_VIB,
    TKN_BELLVIB_MODE_BELL_AND_VIB
};

// 벨 울림 시간
CONST SKY_TOKEN aBell_Time[NUM_BELL_TIME] =
{
    TKN_BELLVIB_TIME_UNLIMIT,
    TKN_BELLVIB_TIME_5SEC,
    TKN_BELLVIB_TIME_10SEC,
    TKN_BELLVIB_TIME_15SEC
};


LOCAL BYTE *ppBellMode[NUM_BELL_MODE];  // 벨/진동
LOCAL BYTE *ppBellTime[NUM_BELL_TIME];  // 벨울림시간

#ifdef FEATURE_CHARACTER_BELL
LOCAL BYTE *ppBellGroup[NUM_BELL_GROUP+2];  // 벨종류
LOCAL WORD wBellList[NUM_BELL_GROUP+2];
LOCAL BYTE *ppBellList[NUM_BELL_LIST+SKY_MAX_NUM_FILES+MAX_NUM_OF_CHARACBELL+1];  // 벨 list
#else
LOCAL BYTE *ppBellGroup[NUM_BELL_GROUP+1];  // 벨종류
LOCAL WORD wBellList[NUM_BELL_GROUP+1];
LOCAL BYTE *ppBellList[NUM_BELL_LIST+SKY_MAX_NUM_FILES+1];  // 벨 list
#endif // FEATURE_CHARACTER_BELL

LOCAL BYTE *ppMyBellList[SKY_MAX_NUM_FILES];    // 나만의벨
LOCAL BYTE *ppGroupList[MAX_GROUP_NUMBER];  // 전화부그룹

LOCAL UINT8 s_nMyBell;      // 나만의벨 수
LOCAL UINT8 s_nBellGroup;   // 그룹벨지정해제 index
LOCAL UINT8 s_nMyBellAlloc; // ppMyBellList Size

LOCAL pb_group_info_type  s_GroupInfo;
LOCAL UINT8 s_GroupIndex;
LOCAL UINT8 s_nBellIndex;

#ifdef FEATURE_CHARACTER_BELL
LOCAL BYTE *ppChaBellList[MAX_NUM_OF_CHARACBELL];    // 캐릭벨
LOCAL UINT8 s_nChaBell;
#endif // FEATURE_CHARACTER_BELL
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

// 착신벨 설정
void EQS_API sAddBellVibControls(void);
WORD  EQS_API sGetCurrentBellId(UINT8 nType);
void EQS_API sGetBellListInfo(UINT8 nType);
void EQS_API sAddBellListControl(UINT8 nType);

BOOL EQS_API sIsMyBell(UINT8 nType);
void EQS_API sPlayCurrentBell(UINT8 nType, WORD wVolume);

// 알람벨 설정
void EQS_API sAddAlarmBellControls(UINT8 nType);
// 그룹벨 설정
void EQS_API sAddGroupBellControls(void);

LOCAL void SKY_API sSaveBellVibSetting(void);
LOCAL void SKY_API sSaveAlarmBell(void);
LOCAL void SKY_API sSchedulerBellSetting(void);
LOCAL void SKY_API sSaveGroupBell(void);

#ifdef FEATURE_CHARACTER_BELL
UINT8 EQS_API SKY_FindChaBellIndex(BYTE *pszBell);
BOOL EQS_API sIsChaBell(UINT8 nType);
#endif // FEATURE_CHARACTER_BELL
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

void SKY_API SKA_MainBellVibSetting(SKY_EVENT_T *pEvent)
{
    UINT8 index;
    static BOOL fEndMessage, fSaveMessage, fIdle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_BELL_VIB), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        // Add Controls
        sAddBellVibControls();
        fEndMessage = fSaveMessage = fIdle = FALSE;
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyBellAlloc; index ++)
            SKY_Free(ppMyBellList[index]);

        SKY_StopMusic();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
//잡음의 원인happykid제공- in 2001.03.08
//       SKY_StopTone();
        SKY_StopMusic();

		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_TM_TIMER1:
        if(EQS_HaveFocus(EQS_GetNthIOC(SBV_BELL_ID)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            //SKY_StopMusic();
            sPlayCurrentBell(SBV_BELL, 
                EQC_GetProgressBar(EQS_GetNthIOC(SBV_BELL_VOLUME)));
        }
        break;

    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SBV_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        case SBV_BELL_VOLUME:
            if(pEvent->dwParam) 
                break;
            SKY_StopMusic();
            sPlayCurrentBell(SBV_BELL, 
                (WORD)EQC_GetProgressBar(EQS_GetNthIOC(SBV_BELL_VOLUME)));
            break;

        default:
            break;
        }
        break;

    // Handle I/O Control Events
    case SE_IO_SCROLL_LIST_SELECT:
	case SE_IO_FOCUS_CHG:
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        SKY_StopMusic();
        break;

    case SE_IO_PROGRESS_BAR_SELECT:
        if(pEvent->wParam == EQS_GetNthIOC(SBV_BELL_VOLUME))
        {
            SKY_StopMusic();
        }
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SBV_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(SBV_BELL_ID))
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;

	case SE_IO_PROGRESS_BAR_CANCEL:
		SKY_StopMusic();
		break;

    case SE_IO_PROGRESS_BAR_UP:
    case SE_IO_PROGRESS_BAR_DOWN:
        if(pEvent->wParam == EQS_GetNthIOC(SBV_BELL_VOLUME))
        {
            SKY_StopMusic();
            sPlayCurrentBell(SBV_BELL, (WORD)pEvent->dwParam);
        }
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
//잡음의 원인제공-happykid in 2001.03.08
        //SKY_StopTone();
        SKY_StopMusic();

        sSaveBellVibSetting();

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        
        fEndMessage = TRUE;
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSaveBellVibSetting();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        else
        {
            // 초기화
            SKY_SET_BELL_ID(INIT_BELL_ID);
            SKY_SET_BELL_FILE_NAME(INIT_BELL_FILE_NAME);
            SKY_SET_BELL_VIBRATION(INIT_BELL_VIBRATION);
            SKY_SetRingerAlertVolume(INIT_BELL_VOLUME);
            SKY_SET_BELL_TIME(INIT_BELL_TIME);

            SKY_DisplayBellVibIcon();

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSaveBellVibSetting(void)
{
    UINT8 nCtrl;

    for(nCtrl = 0; nCtrl < SBV_MAX; nCtrl++)
    {
        switch(nCtrl)
        {
        case SBV_BELL_ID:
            if(sIsMyBell(SBV_BELL))
            {
                UINT8 nBellIndex;
                nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBV_BELL_ID));
                SKY_SET_BELL_FILE_NAME(ppMyBellList[nBellIndex-NUM_BELL_LIST]);
                SKY_SET_BELL_ID(SKY_MY_BELL_ID);
            }
            else
            {
                SKY_SET_BELL_ID(sGetCurrentBellId(SBV_BELL));
            }
            break;

        case SBV_BELL_MODE:
            SKY_SET_BELL_VIBRATION((BELL_VIBRATION_E)
                        EQC_GetScrollLogicalIndex(EQS_GetNthIOC(nCtrl)));
            break;

        case SBV_BELL_VOLUME:
            SKY_SetRingerAlertVolume(EQC_GetProgressBar(EQS_GetNthIOC(nCtrl)));
            break;

        case SBV_BELL_TIME:
            SKY_SET_BELL_TIME((BELL_TIME_E)
                        EQC_GetScrollLogicalIndex(EQS_GetNthIOC(nCtrl)));
            break;

        default:
            break;
        }
    }

    SKY_DisplayBellVibIcon();
}



/*============================================================================= 
Function:    
SKA_MainAlarmBellSetting

  Description: 
  알람벨 설정
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_MainAlarmBellSetting(SKY_EVENT_T *pEvent)
{
    UINT8 index;
    static BOOL fEndMessage, fSaveMessage, fIdle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_ALARM_LIST_MENU2), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        // Add Controls
        sAddAlarmBellControls(SAB_ALARM_BELL);
        fEndMessage = fSaveMessage = fIdle = FALSE;
		EQS_FitCanvasSize();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyBellAlloc; index ++)
            SKY_Free(ppMyBellList[index]);

        SKY_StopMusic();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
//잡음의 원인제공-happykid in 2001.03.08
        //SKY_StopTone();
        SKY_StopMusic();

		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_TM_TIMER1:
        if(EQS_HaveFocus(EQS_GetNthIOC(SAB_BELL_ID)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            sPlayCurrentBell(SAB_ALARM_BELL, 
                EQC_GetProgressBar(EQS_GetNthIOC(SBV_BELL_VOLUME))); 
        }
        break;

    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SAB_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        case SAB_BELL_VOLUME:
            if(pEvent->dwParam) 
                break;
            SKY_StopMusic();
            sPlayCurrentBell(SAB_ALARM_BELL, 
                (WORD)EQC_GetProgressBar(EQS_GetNthIOC(SAB_BELL_VOLUME)));
            break;

        default:
            break;
        }
        break;

    // Handle I/O Control Events
    case SE_IO_SCROLL_LIST_SELECT:
	case SE_IO_FOCUS_CHG:
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SAB_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_ID))
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;

    case SE_IO_PROGRESS_BAR_SELECT:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_VOLUME))
        {
            SKY_StopMusic();
        }
        break;

	case SE_IO_PROGRESS_BAR_CANCEL:
		SKY_StopMusic();
		break;

    case SE_IO_PROGRESS_BAR_UP:
    case SE_IO_PROGRESS_BAR_DOWN:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_VOLUME))
        {
            SKY_StopMusic();
            sPlayCurrentBell(SAB_ALARM_BELL, (WORD)pEvent->dwParam);
        }
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
//잡음의 원인제공-happykid in 2001.03.08
        //SKY_StopTone();
        SKY_StopMusic();

        sSaveAlarmBell();

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        
        fEndMessage = TRUE;
        break;
        
    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSaveAlarmBell();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        else
        {
            // 초기화
            SKY_SET_ALARM_BELL_ID(INIT_ALARM_BELL_ID);
            SKY_SET_ALARM_BELL_TYPE_FILE_NAME(INIT_ALARM_BELL_FILE_NAME);

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSaveAlarmBell(void)
{
    UINT8 nCtrl;

    for(nCtrl = 0; nCtrl < SAB_MAX; nCtrl++)
    {
        switch(nCtrl)
        {
        case SAB_BELL_ID:
            if(sIsMyBell(SAB_ALARM_BELL))
            {
                UINT8 nBellIndex;
                nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SAB_BELL_ID));
                SKY_SET_ALARM_BELL_TYPE_FILE_NAME(ppMyBellList[nBellIndex-NUM_BELL_LIST]);
                SKY_SET_ALARM_BELL_ID(SKY_MY_BELL_ID);
            }
            else
            {
                SKY_SET_ALARM_BELL_ID(sGetCurrentBellId(SAB_ALARM_BELL));
            }
            break;

        case SAB_BELL_VOLUME:
            SKY_SET_ALARM_BELL_VOLUME(EQC_GetProgressBar(EQS_GetNthIOC(nCtrl)));
            SKY_SetAlarmAlertVolume(SKY_GET_ALARM_BELL_VOLUME());
            break;

        default:
            break;
        }
    }
}

/*============================================================================= 
Function:    
SKA_MainSchedulerBellSetting

  Description: 
  스케쥴러 알람 벨 설정
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void SKY_API SKA_MainSchedulerBellSetting(SKY_EVENT_T *pEvent)
{
    UINT8 index;
    static BOOL fEndMessage, fSaveMessage, fIdle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_SCHEDULE_ALARM_BELL_TITLE), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        // Add Controls
        sAddAlarmBellControls(SAB_SCHEDULER_BELL);
        fEndMessage = fSaveMessage = fIdle = FALSE;
		EQS_FitCanvasSize();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyBellAlloc; index ++)
            SKY_Free(ppMyBellList[index]);

        SKY_StopMusic();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		//잡음의 원인제공-happykid in 2001.03.08
        //SKY_StopTone();
        SKY_StopMusic();

		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_TM_TIMER1:
        if(EQS_HaveFocus(EQS_GetNthIOC(SAB_BELL_ID)) && EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            //SKY_StopMusic();

            sPlayCurrentBell(SAB_SCHEDULER_BELL, 
                EQC_GetProgressBar(EQS_GetNthIOC(SBV_BELL_VOLUME))); 
        }
        break;

    case SE_APP_IO_MODE:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SAB_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        case SAB_BELL_VOLUME:
            if(pEvent->dwParam) 
                break;
            SKY_StopMusic();
            sPlayCurrentBell(SAB_SCHEDULER_BELL, 
                (WORD)EQC_GetProgressBar(EQS_GetNthIOC(SAB_BELL_VOLUME)));
            break;

        default:
            break;
        }
        break;

    // Handle I/O Control Events
    case SE_IO_SCROLL_LIST_SELECT:
	case SE_IO_FOCUS_CHG:
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        SKY_StopMusic();
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
        switch(EQS_GetIOCOrder((UINT8)(pEvent->wParam)))
        {
        case SAB_BELL_ID:
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
            break;

        default:
            break;
        }
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_ID))
        {
            SKY_StopMusic();
            EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        }
        break;

    case SE_IO_PROGRESS_BAR_SELECT:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_VOLUME))
        {
            SKY_StopMusic();
        }
        break;

	case SE_IO_PROGRESS_BAR_CANCEL:
		SKY_StopMusic();
		break;

    case SE_IO_PROGRESS_BAR_UP:
    case SE_IO_PROGRESS_BAR_DOWN:
        if(pEvent->wParam == EQS_GetNthIOC(SAB_BELL_VOLUME))
        {
            SKY_StopMusic();
            sPlayCurrentBell(SAB_SCHEDULER_BELL, (WORD)pEvent->dwParam);
        }
        break;

    case SE_SOFTKEY_SAVE:
        //잡음의 원인제공-happykid in 2001.03.08
		//SKY_StopTone();
        SKY_StopMusic();

        sSchedulerBellSetting();

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;
        
    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSchedulerBellSetting();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        else
        {
            // 초기화
            SKY_SET_SCHL_ALERT_BELL_ID(INIT_SCHL_ALERT_BELL_ID);
            SKY_SET_SCHL_ALERT_BELL_TYPE_FILE_NAME(INIT_SCHL_ALERT_BELL_FILE_NAME);

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSchedulerBellSetting(void)
{
    UINT8 nCtrl;

    for(nCtrl = 0; nCtrl < SAB_MAX; nCtrl++)
    {
        switch(nCtrl)
        {
        case SAB_BELL_ID:
            if(sIsMyBell(SAB_SCHEDULER_BELL))
            {
                UINT8 nBellIndex;
                nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SAB_BELL_ID));
                SKY_SET_SCHL_ALERT_BELL_TYPE_FILE_NAME(ppMyBellList[nBellIndex-NUM_BELL_LIST]);
                SKY_SET_SCHL_ALERT_BELL_ID(SKY_MY_BELL_ID);
            }
            else
            {
                SKY_SET_SCHL_ALERT_BELL_ID(sGetCurrentBellId(SAB_SCHEDULER_BELL));
            }
            break;
            
        case SAB_BELL_VOLUME:
            SKY_SET_ALARM_BELL_VOLUME(EQC_GetProgressBar(EQS_GetNthIOC(nCtrl)));
            SKY_SetAlarmAlertVolume(SKY_GET_ALARM_BELL_VOLUME());
            break;

        default:
            break;
        }
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainGroupBellSetting()
**
** Description: 
**     그룹별 벨 설정
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
void SKY_API SKA_MainGroupBellSetting(SKY_EVENT_T *pEvent)
{
    UINT8 nBellIndex =0, index;
    static BOOL fEndMessage, fSaveMessage, fIdle;
    static HCONTROL activeControl;
#ifdef FEATURE_CHARACTER_BELL
    BYTE    temp1[SKY_FILE_NAME_MAX_LENGTH+4];
    BYTE    temp2[SKY_FILE_NAME_MAX_LENGTH+4];
    char    *loc;
#endif // FEATURE_CHARACTER_BELL
    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      200, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        fEndMessage = fSaveMessage = fIdle = FALSE;

        if(g_nGroup == 0)
        {
            EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_BELLVIB_NOGROUP), 
                         MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            
            fEndMessage = TRUE;
            break;
        }
        else
        {
            // Add Controls
            sAddGroupBellControls();
        }

        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;
        
    case SE_APP_END :   /* The very last event */ 
        for(index=0; index < s_nMyBellAlloc; index ++)
            SKY_Free(ppMyBellList[index]);

        SKY_StopMusic();
        break;
        
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
        //잡음의 원인제공-happykid in 2001.03.08
		//SKY_StopTone();
        SKY_StopMusic();

		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);

			fSaveMessage = TRUE;
            if(pEvent->EventID == SE_KEY_END)
                fIdle = TRUE;
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
				EQS_END_TO(EQS_POP_TO_ROOT);
			else
				EQS_End();
		}
        break;

    case SE_TM_TIMER1:
        if(EQC_GET_TYPE(EQS_GetFocusedIOC()) == CT_SCROLL_LIST)
        {
            s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetFocusedIOC());
            sPlayCurrentBell(SGB_BELL, SKY_GET_BELL_VOLUME());
        }
        break;

    case SE_APP_IO_MODE:
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(pEvent->wParam);
        activeControl = pEvent->wParam;

        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);

        break;

    // Handle I/O Control Events
    case SE_IO_FOCUS_CHG:
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_SCROLL_LIST_SELECT:
/*#ifdef FEATURE_CHARACTER_BELL
        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(pEvent->wParam);
        memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
        memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

        STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
        loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

        if (loc == NULL)
        { // 기존에 캐릭벨이 아니었던 경우
            if (sIsChaBell(SGB_BELL) && 
                (s_nBellIndex != (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨로 변경
                sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
        else 
        { // 기존에 캐릭벨이었던 경우
            if (!(sIsChaBell(SGB_BELL)) || 
                (s_nBellIndex == (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨이 아님.
                STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
#endif // FEATURE_CHARACTER_BELL*/
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_SCROLL_LIST_CHG:
    case SE_IO_SCROLL_TAB_CHG:
#ifdef FEATURE_CHARACTER_BELL
        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(pEvent->wParam);
        memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
        memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

        STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
        loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

        if (loc == NULL)
        { // 기존에 캐릭벨이 아니었던 경우
            if (sIsChaBell(SGB_BELL) && 
                (s_nBellIndex != (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨로 변경
                sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
        else 
        { // 기존에 캐릭벨이었던 경우
            if (!(sIsChaBell(SGB_BELL)) || 
                (s_nBellIndex == (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨이 아님.
                STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
#endif // FEATURE_CHARACTER_BELL
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
#ifndef FEATURE_CHARACTER_BELL
        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(pEvent->wParam);
#endif // !FEATURE_CHARACTER_BELL
        activeControl = pEvent->wParam;
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
#ifdef FEATURE_CHARACTER_BELL
        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(pEvent->wParam);
        memset(temp1, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);
        memset(temp2, 0x00, SKY_FILE_NAME_MAX_LENGTH+4);

        STRCPY(temp1, EQC_GetStaticText(EQS_GetFocusedIOC()-1));
        loc = STRSTR (temp1, (BYTE*)SF_CHARACBELL_2);

        if (loc == NULL)
        { // 기존에 캐릭벨이 아니었던 경우
            if (sIsChaBell(SGB_BELL) && 
                (s_nBellIndex != (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨로 변경
                sprintf(temp2, "%s%s%s", temp1, SF_CHARACBELL_2, SF_CHARACBELL_3);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
        else 
        { // 기존에 캐릭벨이었던 경우
            if (!(sIsChaBell(SGB_BELL)) || 
                (s_nBellIndex == (NUM_BELL_LIST + s_nMyBell + s_nChaBell)))
            { // 지금은 캐릭벨이 아님.
                STRNCPY(temp2, temp1, STRLEN(temp1)-4);
                EQC_SetStaticText(EQS_GetFocusedIOC()-1, temp2);
            }
        }
#endif // FEATURE_CHARACTER_BELL
        SKY_StopMusic();
        EQS_StartTimer(EQS_TIMER_1, 400, FALSE);
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_SOFTKEY_SAVE:
        //잡음의 원인제공-happykid in 2001.03.08
		//SKY_StopTone();
        SKY_StopMusic();

        sSaveGroupBell();

        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                        MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;
        
    case SE_IO_MESGBOX_YES:
        if(fSaveMessage)
        {
            sSaveGroupBell();
            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE), 
                MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

            fEndMessage = TRUE;
        }
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        if(fSaveMessage)
            EQS_End();
        break;

     default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

LOCAL void SKY_API sSaveGroupBell(void)
{
    UINT8 nBellIndex, index;

    for(index=0; index < g_nGroup; index++)
    {
        GetGroupInfo(&s_GroupInfo, index);

        s_nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(index*2 +1));

#ifdef FEATURE_CHARACTER_BELL
        if(s_nBellIndex == NUM_BELL_LIST + s_nMyBell + s_nChaBell)
#else
        if(s_nBellIndex == NUM_BELL_LIST+s_nMyBell)
#endif // FEATURE_CHARACTER_BELL
        {
            s_GroupInfo.Bell.wBellID = 0;
        }
        else if(sIsMyBell(SGB_BELL))
        {
            STRCPY(s_GroupInfo.Bell.szFileName, ppMyBellList[s_nBellIndex-NUM_BELL_LIST]);
            s_GroupInfo.Bell.wBellID = SKY_MY_BELL_ID;
        }
#ifdef FEATURE_CHARACTER_BELL
        else if(sIsChaBell(SGB_BELL))
        {
            STRCPY(s_GroupInfo.Bell.szFileName, ppChaBellList[s_nBellIndex-NUM_BELL_LIST-s_nMyBell]);
            s_GroupInfo.Bell.wBellID = SKY_CB_GROUPBELL;
        }
#endif // FEATURE_CHARACTER_BELL
        else
        {
            s_GroupInfo.Bell.wBellID = sGetCurrentBellId(SGB_BELL);
        }
        SetGroupInfo(&s_GroupInfo, index);
    }
}

void SKY_API TwoNumberBellCtrl(void)
{
    CONST BYTE* sTwoNumber[1];
    HCONTROL hControl;

    sTwoNumber[0] = SKY_GET_TOKEN(TKN_ON_DISPLAY);
    //sTwoNumber[1] = SKY_GET_TOKEN(TKN_OFF_DISPLAY);

    EQC_StaticText(EQS_NA, EQS_NA, TKN_TWO_NUM);

    if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
    {   
        EQC_SetNumberEditPrefix(EQC_NumberEdit(EQS_NA, EQS_NA, 16/*18*/, 1, 16/*18*/,SKY_GET_NUMBER_PLUS_NUMBER_SKT()),
        3); // 011 prefix
    }
    else
    {                
        EQC_SetNumberEditPrefix(EQC_NumberEdit(EQS_NA, EQS_NA, 16/*18*/, 1, 16/*18*/,SKY_GET_NUMBER_PLUS_NUMBER_STI()),
        3); // 017 prefix
    }
    
    hControl = EQC_CheckBox(EQS_NA, EQS_NA, (BYTE **)sTwoNumber, 1);
    EQC_SET_STYLE(hControl,  (EQC_GET_STYLE(hControl) & ~CS_CHECK_BOX_BORDER));
    
    if(SKY_IS_NUMBER_PLUS())
        EQC_SetCheckBox(hControl, 0);
    
    sGetBellListInfo(TWO_NUM_BELL);
    sAddBellListControl(TWO_NUM_BELL);

    // Formatting..
    EQS_AutoFormatting(TNC_MAX, EHA_LEFT, EVA_CENTER);
}


void SKY_API TwoNumberBellCtrlFree(void)
{
    UINT8 index;

    for(index=0; index < s_nMyBellAlloc; index ++)
        SKY_Free(ppMyBellList[index]);
}

void SKY_API SaveTwoNumBell(void)
{
    UINT8 nBellIndex;

    if(sIsMyBell(TWO_NUM_BELL))
    {
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(TNC_BELL_SCRL));
        SKY_SET_NUMBER_PLUS_BELL_FILENAME(ppMyBellList[nBellIndex-NUM_BELL_LIST]);
        SKY_SET_NUMBER_PLUS_BELL_ID(SKY_MY_BELL_ID);
    }
    else
    {
        SKY_SET_NUMBER_PLUS_BELL_ID(sGetCurrentBellId(TWO_NUM_BELL));
    }
}

BOOL SKY_API CompareTwoNumBell(void)
{
    UINT8 nBellIndex;

    if(sIsMyBell(TWO_NUM_BELL))
    {
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(TNC_BELL_SCRL));
        if((SKY_GET_NUMBER_PLUS_BELL_ID() == SKY_MY_BELL_ID) && 
            (!STRCMP(SKY_GET_NUMBER_PLUS_BELL_FILENAME(),ppMyBellList[nBellIndex-NUM_BELL_LIST])))
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if(SKY_GET_NUMBER_PLUS_BELL_ID() == sGetCurrentBellId(TWO_NUM_BELL))
            return TRUE;
        else 
            return FALSE;
    }
}

/*============================================================================= 
Function:    
sAddBellVibControls

  Description: 
  착신벨 화면 설정
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/

void EQS_API sAddBellVibControls(void)
{
    UINT8   index;

    // Init List
    for(index=0; index<NUM_BELL_MODE; index++)
        ppBellMode[index] = (BYTE *)SKY_GET_TOKEN(aBell_Mode[index]);

    for(index=0; index<NUM_BELL_TIME; index++)
        ppBellTime[index] = (BYTE *)SKY_GET_TOKEN(aBell_Time[index]);

    sGetBellListInfo(SBV_BELL);
    sAddBellListControl(SBV_BELL);

    // 벨크기
    EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_VOLUME);
    EQC_ProgressBar(EQS_NA, EQS_NA, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_BELL_VOLUME(),FALSE);

    // 벨/진동
    EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_MODE);
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, BELL_SCROLL_LIST_CHAR, 
                   SBV_GET_SLSIZE(NUM_BELL_MODE), 
                   ppBellMode, 
                   NUM_BELL_MODE, NUM_BELL_MODE,
                   (UINT8)SKY_GET_BELL_VIBRATION()),
                   SBV_SCROLL_LIST_STYLE);

    // 벨울림 시간
    EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TIME);
    EQC_SET_STYLE(EQC_ScrollCombo(EQS_NA, EQS_NA, BELL_SCROLL_LIST_CHAR, 
                   SBV_GET_SLSIZE(NUM_BELL_TIME), 
                   ppBellTime, 
                   NUM_BELL_TIME, NUM_BELL_TIME,
                   (UINT8)SKY_GET_BELL_TIME()),
                   SBV_SCROLL_LIST_STYLE);

    // Screen Formatting
    EQS_AutoFormatting(SBV_MAX, EHA_LEFT, EVA_CENTER);
}

/*============================================================================= 
Function:    
sAddAlarmBellControls

  Description: 
  알람벨/스케쥴러 알림 벨 화면 설정
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/

void EQS_API sAddAlarmBellControls(UINT8 nType)
{
    sGetBellListInfo(nType);
    sAddBellListControl(nType);

    // 알람벨크기(스케쥴러와 동일)
    EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_VOLUME);
    EQC_ProgressBar(EQS_NA, EQS_NA, PROGRESS_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_ALARM_BELL_VOLUME(),FALSE);

    // Screen Formatting
    EQS_AutoFormatting(SAB_MAX, EHA_LEFT, EVA_CENTER);
    EQS_SetCanvasSize(SKY_DEF_WIN_DX, 200);
}

/*============================================================================= 
Function:    
sAddGroupBellControls

  Description: 
  그룹별 벨 설정 화면 
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void EQS_API sAddGroupBellControls(void)
{
    UINT8   index;
    BYTE    szTmp[50];

    // Load Group List
    sGetBellListInfo(SGB_BELL);

    for(index=0; index < g_nGroup; index++)
    {
#ifdef FEATURE_CHARACTER_BELL
        GetGroupInfo(&s_GroupInfo, index);
        if (SKY_IS_CB_GROUPBELL(s_GroupInfo.Bell.wBellID)) 
        {
            BYTE TempName[20];
            memset(TempName, 0x00, 20);
            sprintf(TempName, SF_GROUP"%s%s", GetGroupName(index), " ");
            STRNCPY(szTmp, TempName, BELL_SCROLL_LIST_CHAR-2); //14
            STRCAT(szTmp, SF_CHARACBELL_2);
            STRCAT(szTmp, SF_CHARACBELL_3);
        }
        else
        {
            sprintf(szTmp, SF_GROUP"%s", GetGroupName(index));
        }
#else
        sprintf(szTmp, SF_GROUP"%s", GetGroupName(index));
#endif // FEATURE_CHARACTER_BELL
        EQC_SetStaticText(EQC_StaticText(EQS_NA, EQS_NA, TKN_NULL), szTmp);
        s_GroupIndex = index;
        sAddBellListControl(SGB_BELL);
    }
        //ppGroupList[index] = GetGroupName(index);

/*
    // 그룹리스트
    EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_GROUPBELL_GROUP);

    EQC_SET_STYLE( EQC_ScrollCombo(EQS_NA, EQS_NA, BELL_SCROLL_LIST_CHAR, 
                   SBV_GET_SLSIZE(g_nGroup), 
                   ppGroupList, 
                   g_nGroup, g_nGroup,
                   0),
                   SBV_SCROLL_LIST_STYLE);
*/
    //sAddBellListControl(SGB_BELL);

    // Screen Formatting
    EQS_AutoFormatting(g_nGroup*2, EHA_LEFT, EVA_CENTER);
}

/*============================================================================= 
Function:    

  Description: 
    Common functions
  
    Input:
    @ArgumentsList
    
      Output:
      @TheResultOfThisFunction
      
        Return value:
        @ReturnValue
        
          Side effects:
===============================================================================*/
void EQS_API sGetBellListInfo(UINT8 nType)
{
    UINT16   index, nBellIndex = 0, i;
    WORD wBellID = 0;
    BYTE *pBellFile = NULL;
#ifdef FEATURE_CHARACTER_BELL
    UINT8   nIndex, nPosition;
#endif // FEATURE_CHARACTER_BELL

    s_nMyBellAlloc = (UINT8)SKY_GetFileNumberInDataFile(SDF_MELODY);

    if(s_nMyBellAlloc > 0)
    {
        // Load user file list
        for(index=0; index < s_nMyBellAlloc; index ++)
            ppMyBellList[index] = (BYTE*)SKY_Malloc(SKY_PATH_NAME_MAX_LENGTH);

        s_nMyBell = SKY_GetFileExtList(SKY_MELODY_DIR, ppMyBellList);
    }
    else
    {
        s_nMyBell = 0;
    }

     s_nBellGroup = NUM_BELL_GROUP;
    for(index=0; index<s_nBellGroup; index++)
        ppBellGroup[index] = (BYTE *)SKY_GET_TOKEN(aBell_Group[index].GroupName);
#ifdef FEATURE_CHARACTER_BELL
    if(nType == SGB_BELL)
        ppBellGroup[index] = SKY_GET_TOKEN(TKN_CHARACBELL_MENU);
#endif // FEATURE_CHARACTER_BELL

    for(index=0; index<NUM_BELL_LIST; index++)
        ppBellList[index] = (BYTE *)SKY_GET_TOKEN(aBell_List[index].BellName);

	for(i = index ; i < NUM_BELL_LIST + s_nMyBell; i++)
    {
		ppBellList[i] = ppMyBellList[i - index];
    }

#ifdef FEATURE_CHARACTER_BELL
    if(nType == SGB_BELL) {
        s_nChaBell = SKY_GET_CB_ADDRESS();
        if(s_nChaBell > 0)
        {
            for(nIndex = 0; nIndex < s_nChaBell; nIndex ++)
                ppChaBellList[nIndex] = (BYTE*)SKY_Malloc(MAX_TITLE_LENGTH);
        
            for (nIndex=0, nPosition = 0; nIndex < MAX_NUM_OF_CHARACBELL; nIndex++) {
                if (SKY_GET_CB_EMPTY(nIndex)) {
                    STRCPY(ppChaBellList[nPosition], SKY_GET_CB_TITLE(nIndex));
                    nPosition++;
                }
            }
        }
        for(index = i ; index < NUM_BELL_LIST + s_nMyBell + s_nChaBell; index++)
  		    ppBellList[index] = ppChaBellList[index - i];
        ppBellList[index] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);   
    }
#else
    if(nType == SGB_BELL)
        ppBellList[i] = SKY_GET_TOKEN(TKN_BELLVIB_GROUPBELL_UNSET);    
#endif // FEATURE_CHARACTER_BELL
}

void EQS_API sAddBellListControl(UINT8 nType)
{
    UINT16   index, nBellIndex = 0;
    WORD wBellID = 0;
    BYTE *pBellFile = NULL;
    HCONTROL hControl;

    // 벨종류
    switch(nType){
    case SBV_BELL:
        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);

        wBellID = SKY_GET_BELL_ID();
        pBellFile = SKY_GET_BELL_FILE_NAME(); 
        break;

    case SAB_ALARM_BELL:
        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);

        wBellID = SKY_GET_ALARM_BELL_ID();
        pBellFile = SKY_GET_ALARM_BELL_FILE_NAME();
        break;

    case SAB_SCHEDULER_BELL:
        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);

        wBellID = SKY_GET_SCHL_ALERT_BELL_ID();
        pBellFile = SKY_GET_SCHL_ALERT_BELL_FILE_NAME();
        break;

    case SGB_BELL:
        //EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);
#ifdef FEATURE_CHARACTER_BELL
        ppBellGroup[s_nBellGroup+1] = (BYTE *)SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);   // 해제
#else
        ppBellGroup[s_nBellGroup] = (BYTE *)SKY_GET_TOKEN(TKN_PBOOK_NO_GROUP_BELL);   // 해제
#endif // FEATURE_CHARACTER_BELL
        break;

    case TWO_NUM_BELL:
        EQC_StaticText(EQS_NA, EQS_NA, TKN_BELLVIB_TYPE);

        wBellID = SKY_GET_NUMBER_PLUS_BELL_ID();
        pBellFile = SKY_GET_NUMBER_PLUS_BELL_FILENAME(); 
        break;
    }

    if(nType != SGB_BELL)
    {
        // 그룹
        if(SKY_IS_MY_BELL(wBellID))
            nBellIndex = SKY_FindMyBellIndex(pBellFile); 
        else
            nBellIndex = SKY_FindBellIndex(wBellID);
    }
    else
    {
        //s_GroupIndex = 0;
        GetGroupInfo(&s_GroupInfo, s_GroupIndex);
        // 그룹
        if(!s_GroupInfo.Bell.wBellID)
        {
#ifdef FEATURE_CHARACTER_BELL
            nBellIndex = NUM_BELL_LIST + s_nMyBell + s_nChaBell;
#else
            nBellIndex = NUM_BELL_LIST + s_nMyBell;
#endif // FEATURE_CHARACTER_BELL
        }
        else
        {
            if(SKY_IS_MY_BELL(s_GroupInfo.Bell.wBellID))
                nBellIndex = SKY_FindMyBellIndex(s_GroupInfo.Bell.szFileName); 
#ifdef FEATURE_CHARACTER_BELL
            else if(SKY_IS_CB_GROUPBELL(s_GroupInfo.Bell.wBellID))
                nBellIndex = SKY_FindChaBellIndex(s_GroupInfo.Bell.szFileName); 
#endif // FEATURE_CHARACTER_BELL
            else
                nBellIndex = SKY_FindBellIndex(s_GroupInfo.Bell.wBellID);
        }
    }

    if(nType == SGB_BELL)
    {
#ifdef FEATURE_CHARACTER_BELL
	    hControl = EQC_ScrollList(EQS_NA, EQS_NA,BELL_SCROLL_LIST_CHAR, 
                                    4, 
                                    ppBellList, 
                                    NUM_BELL_LIST + s_nMyBell + s_nChaBell + 1, 
                                    NUM_BELL_LIST + s_nMyBell + s_nChaBell + 1, 
                                    nBellIndex);

        for(index=0; index < NUM_BELL_GROUP; index ++)
        {
		    wBellList[index] = aBell_Group[index].nMelodyNumber;
        }
        
        wBellList[s_nBellGroup-1] = s_nMyBell;
        wBellList[s_nBellGroup] = s_nChaBell;
        wBellList[s_nBellGroup+1] = 1;
#else
        hControl = EQC_ScrollList(EQS_NA, EQS_NA,BELL_SCROLL_LIST_CHAR, 
                                    4, 
                                    ppBellList, 
                                    NUM_BELL_LIST + s_nMyBell +1, 
                                    NUM_BELL_LIST + s_nMyBell +1, 
                                    nBellIndex);

        for(index=0; index < NUM_BELL_GROUP; index ++)
        {
		    wBellList[index] = aBell_Group[index].nMelodyNumber;
        }
        
        wBellList[s_nBellGroup-1] = s_nMyBell;
        wBellList[s_nBellGroup] = 1;
#endif // FEATURE_CHARACTER_BELL
    }
    else
    {
	    hControl = EQC_ScrollList(EQS_NA, EQS_NA,BELL_SCROLL_LIST_CHAR, 
                                    4, 
                                    ppBellList, 
                                    NUM_BELL_LIST + s_nMyBell, 
                                    NUM_BELL_LIST + s_nMyBell, 
                                    nBellIndex);

	    for(index=0; index < NUM_BELL_GROUP; index ++)
        {
		    wBellList[index] = aBell_Group[index].nMelodyNumber;
        }
        wBellList[s_nBellGroup-1] = s_nMyBell;
    }

    

	EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) |\
				            CS_SCROLL_LIST_COMBO |\
							CS_SCROLL_LIST_TAB );

    if(nType == SGB_BELL)
#ifdef FEATURE_CHARACTER_BELL
        EQC_SetScrollTab(hControl,s_nBellGroup+2,4,FALSE,0);
#else
        EQC_SetScrollTab(hControl,s_nBellGroup+1,3,FALSE,0);
#endif // FEATURE_CHARACTER_BELL
    else
        EQC_SetScrollTab(hControl,s_nBellGroup,3,FALSE,0);
    EQC_SetScrollTabData(hControl, wBellList, ppBellGroup);
}

WORD  EQS_API sGetCurrentBellId(UINT8 nType)
{
    UINT8 nBellIndex;
    WORD  nBellId;

    switch(nType){
    case SBV_BELL:
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBV_BELL_ID));
        break;

    case SAB_ALARM_BELL:
    case SAB_SCHEDULER_BELL:
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SAB_BELL_ID));
        break;

    case SGB_BELL:
        nBellIndex = s_nBellIndex;
        //nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SGB_BELL_ID));
        break;

    case TWO_NUM_BELL:
        nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(TNC_BELL_SCRL));
        break;

    default:
        nBellIndex = 0;
    }

    nBellId = aBell_List[nBellIndex].wBellId;

    return nBellId;
}

BOOL EQS_API sIsMyBell(UINT8 nType)
{
    switch(nType){
    case SBV_BELL:
        if(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBV_BELL_ID))
            >= NUM_BELL_LIST)
            return TRUE;
        break;

    case SAB_ALARM_BELL:
    case SAB_SCHEDULER_BELL:
        if(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SAB_BELL_ID))
            >= NUM_BELL_LIST)
            return TRUE;
        break;

    case SGB_BELL:
#ifdef FEATURE_CHARACTER_BELL
        if(s_nBellIndex >= NUM_BELL_LIST && s_nBellIndex < (NUM_BELL_LIST+s_nMyBell))
            return TRUE;
#else
        if(s_nBellIndex >= NUM_BELL_LIST)
            return TRUE;
#endif // FEATURE_CHARACTER_BELL
        break;
    case TWO_NUM_BELL:
        if(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(TNC_BELL_SCRL))
            >= NUM_BELL_LIST)
            return TRUE;
        break;
    }
    return FALSE;
}

#ifdef FEATURE_CHARACTER_BELL
BOOL EQS_API sIsChaBell(UINT8 nType)
{
    switch(nType){
    case SGB_BELL:
        if(s_nBellIndex >= (NUM_BELL_LIST+ s_nMyBell))
            return TRUE;
        break;
    }
    return FALSE;
} // sIsChaBell
#endif // FEATURE_CHARACTER_BELL

void EQS_API PlayTwoNumBell(void)
{
    sPlayCurrentBell(TWO_NUM_BELL, SKY_GET_BELL_VOLUME());
}

void EQS_API sPlayCurrentBell(UINT8 nType, WORD wVolume)
{
    UINT8 nBellIndex;

    if(sIsMyBell(nType))
    {
        switch(nType){
        case SBV_BELL:
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SBV_BELL_ID));
            SKY_PlayMusicFileEx(ppMyBellList[nBellIndex-NUM_BELL_LIST], wVolume);
            break;
        case SAB_ALARM_BELL:
        case SAB_SCHEDULER_BELL:
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(SAB_BELL_ID));
            SKY_PlayMusicFileEx(ppMyBellList[nBellIndex-NUM_BELL_LIST], wVolume);
            break;
        case SGB_BELL:
            nBellIndex = s_nBellIndex;
            if(nBellIndex < NUM_BELL_LIST+s_nMyBell)
                SKY_PlayMusicFileEx(ppMyBellList[nBellIndex-NUM_BELL_LIST], wVolume);
            break;
        case TWO_NUM_BELL:
            nBellIndex = (UINT8)EQC_GetScrollLogicalIndex(EQS_GetNthIOC(TNC_BELL_SCRL));
            SKY_PlayMusicFileEx(ppMyBellList[nBellIndex-NUM_BELL_LIST], wVolume);
            break;
        }
    }
#ifdef FEATURE_CHARACTER_BELL
    else if (sIsChaBell(nType)) {
        switch(nType){
        case SGB_BELL:
            nBellIndex = s_nBellIndex;
            if(nBellIndex < NUM_BELL_LIST+s_nMyBell+s_nChaBell) {
                char    temp[MAX_TITLE_LENGTH];
                UINT8   index, sIndex, inc;

                sIndex = nBellIndex - NUM_BELL_LIST - s_nMyBell;

                for(index = 0, inc = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
                    if (SKY_GET_CB_EMPTY(index)) {
                        if (inc == sIndex) break;
                        inc++;
                    }
                }
                inc = index;

                STRCPY(temp, SKY_GET_CB_MTITLE(inc));
                if (SKY_GET_CB_MTYPE(inc) == SKY_MY_BELL_ID)
                    SKY_PlayMusicFileEx(temp, wVolume);
                else
                    SKY_PlayMusicEx(SKY_GET_CB_MTYPE(inc), wVolume, 0);
            }
            break;
        }
    }
#endif // FEATURE_CHARACTER_BELL
    else
    {
        if(nType != SGB_BELL)
            SKY_PlayMusicEx(sGetCurrentBellId(nType), wVolume, 0);
        else
            SKY_PlayMusicEx(sGetCurrentBellId(nType), wVolume, 0);
    }
}

/////
/// Global Function
//////
UINT8 EQS_API SKY_BellFindGroup(UINT8 nBellId, UINT8 nBellGroup)
{
    UINT8   nTmp=0, index;

    for(index=0; index < nBellGroup; index ++)
    {
        nTmp += aBell_Group[index].nMelodyNumber;

        if(nBellId < nTmp)
            return index;
    }

    return 0;
}

UINT8 EQS_API SKY_BellFindGroupIndex(UINT8 nGroup)
{
    UINT8   nTmp=0, index;

    for(index=0; index < nGroup; index ++)
    {
        nTmp += aBell_Group[index].nMelodyNumber;
    }

    return nTmp;
}

UINT8 EQS_API SKY_FindBellIndex(WORD wBellId)
{
    UINT8   index;

    for(index=0; index < NUM_BELL_LIST; index ++)
    {
        if(aBell_List[index].wBellId == wBellId)
            return index;
    }

    return 0;
}

UINT8 EQS_API SKY_FindMyBellIndex(BYTE *pszBell)
{
    UINT8   index;

    for(index=0; index < s_nMyBell; index ++)
    {
        if(!STRCMP(ppMyBellList[index], pszBell))
            return (index + (NUM_BELL_LIST));
    }

    return NOT_AVAIL_MYBELL;
}

#ifdef FEATURE_CHARACTER_BELL
UINT8 EQS_API SKY_FindChaBellIndex(BYTE *pszBell)
{
    UINT8   index;

    for(index=0; index < s_nChaBell; index ++)
    {
        if(!STRCMP(ppChaBellList[index], pszBell))
            return (index + NUM_BELL_LIST + s_nMyBell);
    }

    return NOT_AVAIL_MYBELL;
} // SKY_FindChaBellIndex
#endif // FEATURE_CHARACTER_BELL