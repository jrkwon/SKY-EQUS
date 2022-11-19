/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_volm.c
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

//#define MIC_VOL
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
enum tag_SKA_VOLM_CONTROL_E {
    SVC_EARPIECE_TITLE,
    SVC_EARPIECE,
    SVC_KEYTONE_TITLE,
    SVC_KEYTONE,
#ifdef MIC_VOL
    SVC_MIC_TITLE,
    SVC_MIC,
#endif
    SVC_RINGER_TITLE,
    SVC_RINGER,
    SVC_ALARM_TITLE,
    SVC_ALARM,
    SVC_SMS_TITLE,
    SVC_SMS,
    SVC_GAME_TITLE,
    SVC_GAME,
    SVC_MAX
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
void EQS_API sAddControls(void);
void EQS_API sPlayChangedVolume(HCONTROL hControl, UINT8 nLevel);
void EQS_API sSaveChangedVolume(void);

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
void SKY_API SKA_MainVolumeSetting(SKY_EVENT_T *pEvent)
{
//    STATIC UINT8    nIndex;
    static BOOL fEndMessage, fSaveMessage, fIdle;

    switch(pEvent->EventID)
    {
        /* APP */
    case SE_APP_START : /* The very first event */
        EQS_SetWindow(SKY_DEF_WIN_X, 
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      300, 
                      CL_NONE_SELECT_SAVE, 
                      SKY_GET_TOKEN(TKN_VOLUME_SET), 
                      WA_SHOW_CML | WA_SHOW_HEADER);

        // Add Controls
        sAddControls();
        fEndMessage = fSaveMessage = fIdle = FALSE;
   		SKY_SET_KEY_TONE(FALSE);	// 볼륨 조절시 키톤음 안들리도록
        break;
        
    case SE_APP_END :   /* The very last event */ 
    case SE_APP_SUSPEND: /* child applet is started */
        SKY_StopMusic();
		SKY_SET_KEY_TONE(TRUE);
        break;
        
    case SE_APP_RESUME: /* child applet is ended */
        break;

    case SE_KEY_CLEAR:
    case SE_KEY_END:
		SKY_SET_KEY_TONE(TRUE);
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
      
	case SE_IO_PROGRESS_BAR_CANCEL:
    case SE_IO_PROGRESS_BAR_SELECT:
    case SE_IO_FOCUS_CHG:
		SKY_StopMusic();
		break;

    case SE_IO_PROGRESS_BAR_ACTIVE:
		// go through..
    case SE_IO_PROGRESS_BAR_UP:
    case SE_IO_PROGRESS_BAR_DOWN:
        sPlayChangedVolume((HCONTROL)pEvent->wParam, (UINT8)pEvent->dwParam);
        break;

    case SE_IO_MESGBOX_END:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fEndMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_NO:
        if(fIdle)
            EQS_END_TO(EQS_POP_TO_ROOT);
        else if(fSaveMessage)
            EQS_End();
        break;

    case SE_IO_MESGBOX_YES:
        if(!fSaveMessage)
        {
            SKY_SetVoiceVolume(INIT_SPEAKER_VOLUME);
            SKY_SetBeepVolume (INIT_KEY_TONE_VOLUME);
#ifdef MIC_VOL
            SKY_SetMicLevel(INIT_MIC_VOLUME);
#endif
            SKY_SetRingerAlertVolume(INIT_BELL_VOLUME);
            SKY_SetAlarmAlertVolume(INIT_ALARM_BELL_VOLUME);
            SKY_SetSMSAlertVolume(INIT_SMS_ALERT_BELL_VOLUME);

            SKY_DisplayBellVibIcon();

            EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_PROCESS), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);


            fEndMessage = TRUE;
            break;
        }
        // Go through..
    case SE_SOFTKEY_SAVE:
        SKY_StopMusic();

        sSaveChangedVolume();
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_DONE_SAVE),
                      MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);

        fEndMessage = TRUE;
        break;

    case SE_SOFTKEY_INIT:
        EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_INIT_CONFIRM), 
                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
        break;

    default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}

extern const byte Game[];

void EQS_API sPlayChangedVolume(HCONTROL hControl, UINT8 nLevel)
{
    UINT8   nCtrl;    

    for(nCtrl = 0; nCtrl < SVC_MAX; nCtrl++)
    {
        if(EQS_GetNthIOC(nCtrl) == hControl)
        {
            switch(nCtrl)
            {
            case SVC_EARPIECE:  // 통화음 크기
                SKY_PlayToneEx( UI_BEEP_SND, SND_2ND, 300 , nLevel);
                break;

            case SVC_KEYTONE:   // 키톤 크기
                SKY_PlayToneEx( UI_BEEP_SND, SND_2ND, 300 , nLevel);
                break;
#ifdef MIC_VOL
            case SVC_MIC:       // 마이크 크기
                break;
#endif  
            case SVC_RINGER:    // 착신벨 크기
                SKY_StopMusic();
                if(SKY_IS_MY_BELL(SKY_GET_BELL_ID()))
                    SKY_PlayMusicFileEx(SKY_GET_BELL_FILE_NAME(), nLevel);
#ifdef FEATURE_CHARACBELL
                else if(SKY_IS_CHARAC_BELL(SKY_GET_BELL_ID())) {
                    SetCharacbellSelected(TRUE);
                    SKY_PlayMusicFileEx(SKY_GET_BELL_FILE_NAME(), nLevel);
                }
#endif // FEATURE_CHARACBELL
                else
                    SKY_PlayMusicEx(SKY_GET_BELL_ID(), nLevel, 0);
                break;

            case SVC_ALARM:     // 알람벨 크기
                SKY_StopMusic();
                if(SKY_IS_MY_BELL(SKY_GET_ALARM_BELL_ID()))
                    SKY_PlayMusicFileEx(SKY_GET_ALARM_BELL_FILE_NAME(), nLevel);
#ifdef FEATURE_CHARACBELL
                else if(SKY_IS_CHARAC_BELL(SKY_GET_ALARM_BELL_ID())) {
                    SetCharacbellSelected(TRUE);
                    SKY_PlayMusicFileEx(SKY_GET_ALARM_BELL_FILE_NAME(), nLevel);
                }
#endif // FEATURE_CHARACBELL
                else
                    SKY_PlayMusicEx(SKY_GET_ALARM_BELL_ID(), nLevel, 0);
                break;

            case SVC_SMS:
                SKY_StopMusic();
                SKY_PlayMusicEx(SKY_GET_SMS_BELL_ID(), nLevel, 0);
                break;

            case SVC_GAME:
                SKY_StopMusic();
                SKY_PlayMusicBufferEx((BYTE*)Game, nLevel, 0);
                break;

            default:
                break;
            }
            break;
        }
    }
}

void EQS_API sSaveChangedVolume(void)
{
    UINT8   nCtrl;    
    UINT8   nLevel = 0;

    for(nCtrl = 0; nCtrl < SVC_MAX; nCtrl++)
    {
        if(EQC_GET_TYPE(EQS_GetNthIOC(nCtrl)) != CT_PROGRESS_BAR)
            continue;

        nLevel = EQC_GetProgressBar(EQS_GetNthIOC(nCtrl));

        switch(nCtrl)
        {
        case SVC_EARPIECE:  // 통화음 크기
            SKY_SetVoiceVolume(nLevel);
            break;

        case SVC_KEYTONE:   // 키톤 크기
            SKY_SetBeepVolume (nLevel);
            break;
#ifdef MIC_VOL
        case SVC_MIC:       // 마이크 크기
            SKY_SetMicLevel((MIC_VOLUME_E)nLevel);
            break;
#endif
        case SVC_RINGER:    // 착신벨 크기
            SKY_SetRingerAlertVolume(nLevel);
            break;

        case SVC_ALARM:     // 알람벨 크기
            SKY_SetAlarmAlertVolume(nLevel);
            break;

        case SVC_SMS:
            SKY_SetSMSAlertVolume(nLevel);
            break;

        case SVC_GAME:
            SKY_SET_GAME_VOLUME(nLevel);
            break;

        default:
            break;
        }
    }

    SKY_DisplayBellVibIcon();
}

#ifdef MIC_VOL
#define SVC_BAR_WIDTH   67
#else
#define SVC_BAR_WIDTH   70
#endif

#define SVC_BAR_HEIGHT   4

#define SVC_BAR_POS      4

void EQS_API sAddControls(void)
{
	UINT8    nCtrl;
	HCONTROL hControl;
	COORD    StartX, StartX1, StartY, DY;

    MARGIN_T Margin;
    COORD    xSpace, ySpace;	

    EQS_StartFormatter();   
    EQS_GetControlMargin(&Margin);
    EQS_GetControlSpace(&xSpace, &ySpace);
    EQS_EndFormatter();


	StartX = Margin.xLeft;
	StartY = Margin.yTop;

    // 통화음 크기
    hControl = EQC_StaticText(StartX, StartY, TKN_VOLUME_EARPIECE);  
	DY = EQC_GET_DY(hControl) + 2;
	StartX1 = StartX + EQC_GET_DX(hControl) + xSpace;
	hControl = EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_SPEAKER_VOLUME(),FALSE);
    EQC_SetProgressBarLimit(hControl, 1,  5);
	StartY += DY;
    // 키톤 크기
    EQC_StaticText(StartX, StartY, TKN_VOLUME_KEYTONE);    
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_KEY_TONE_VOLUME(),FALSE);
	StartY += DY;
#ifdef MIC_VOL
    // 마이크 크기
    EQC_StaticText(StartX, StartY, TKN_VOLUME_MIC);    
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS,SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT, 0, 3, SKY_GET_MIC_VOLUME(),FALSE);
	StartY += DY;
#endif
    // 착신벨 크기
    EQC_StaticText(StartX, StartY, TKN_VOLUME_RINGER);   
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_BELL_VOLUME(),FALSE);
	StartY += DY;
    // 알람벨 크기
    EQC_StaticText(StartX, StartY, TKN_VOLUME_ALARM); 
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH, PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_ALARM_BELL_VOLUME(),FALSE);
	StartY += DY;	
    // 메시지 도착 알람 크기
    EQC_StaticText(StartX, StartY, TKN_VOLUME_SMS);
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_SMS_BELL_VOLUME(),FALSE);
    StartY += DY;	
    // 게임 음량
    EQC_StaticText(StartX, StartY, TKN_VOLUME_GAME);
	EQC_ProgressBar(StartX1, StartY+SVC_BAR_POS, SVC_BAR_WIDTH,PROGRESS_BAR_HEIGHT,0, 5, SKY_GET_GAME_VOLUME(),FALSE);
    
      
    EQS_FitCanvasSize();
}
