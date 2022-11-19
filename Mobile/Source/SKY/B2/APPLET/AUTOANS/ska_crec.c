/*
** ===========================================================================
**
** Project: 
**     Boa
**
** File:
**     ska_crec.c
**
** Description: 
**     Call Recording!!
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-01-03   hjcho      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "customer.h"
#include "ska.h"





/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define CR_MAX_TMPBUF                30





enum tagCALL_RECORD_E
{
 CR_TITLE = 0,
 CR_INDEX,
 CR_TIME_TITLE,
 CR_TIME,
 CR_PROGRESS,
 CR_STOP,
 CR_MAX
};


enum tagCALL_EVENT_E
{
  CE_KEY_END = 0,
  CE_CALL_END
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
STATIC BYTE WorkingFn;  //working_fn

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
extern int  getfreeindxnum(void);
extern BYTE getfn(void);


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
void sExitCallRecording()
{
   Release_play_rec();
   gnVocoderMode = VR_NONE;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIsRecord(void)
**
** Description: 
**     Decide whether it is possible to reocrd some conversation ..     
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
BOOL sIsRecord(void)
{
  VMEMOINFO_T*    VoiceInfo;
   
  VoiceInfo = GetVoiceInfo();

  return ((VoiceInfo->bTotalRecNumber < (NV_MAX_FL_FILE_NUM-1)) &&
          /* hjcho 01.05.13 인사말녹음과 음성메모를 분리 */
          /* (VoiceInfo->wRemainedTime   > 0 ) &&        */
          (GetVoiceMemoRemainTime() > 0)    &&
          (getfreeindxnum()>0)              &&
          (getfn() < NV_MAX_FL_FILE_NUM));
 

}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitRecordVoiceMemoInRecording()
**
** Description: 
**     Initialize Display in recording voice memo.
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
/*
  녹음 중 화면
*/
enum tagCALLRECORD_E
{
 CREC_ANIMATION = 0,    
 CREC_RECORDING_TIME,  
// CREC_BACKGROUND,       
 CREC_RECORDING_INDEX,  
 CREC_PROGRESS,  
 CREC_REMAIN_TIME_TITLE,
 CREC_REMAIN_TIME       
};



#define CREC_RECORD_ANIMATION_XPOS          VM_RECORD_ANIMATION_XPOS
#define CREC_RECORD_ANIMATION_YPOS          VM_RECORD_ANIMATION_YPOS

#define CREC_RECORD_RECORDINGTIME_XPOS      VM_RECORD_RECORDINGTIME_XPOS
#define CREC_RECORD_RECORDINGTIME_YPOS      VM_RECORD_RECORDINGTIME_YPOS

#define CREC_RECORD_BACKGROUND_XPOS         VM_RECORD_BACKGROUND_XPOS
#define CREC_RECORD_BACKGROUND_YPOS         VM_RECORD_BACKGROUND_YPOS

#define CREC_RECORD_NUMBER_XPOS             VM_RECORD_NUMBER_XPOS
#define CREC_RECORD_NUMBER_YPOS             VM_RECORD_NUMBER_YPOS

#define CREC_RECORD_PBAR_XPOS               VM_RECORD_PBAR_XPOS
#define CREC_RECORD_PBAR_YPOS               VM_RECORD_PBAR_YPOS

#define CREC_RECORD_REMAIN_TIME_TITLE_XPOS  VM_RECORD_REMAIN_TIME_TITLE_XPOS

#define CREC_RECORD_REMAINTIME_XPOS         VM_RECORD_REMAINTIME_XPOS
#define CREC_RECORD_REMAINTIME_YPOS         VM_RECORD_REMAINTIME_YPOS

#define CREC_RECORD_REC_NUM_IMAGE_XPOS      VM_RECORD_REC_NUM_IMAGE_XPOS
#define CREC_RECORD_REC_NUM_IMAGE_YPOS      VM_RECORD_REC_NUM_IMAGE_YPOS




void sInitCallRecordingDisplay(DWORD dwCurrTime, CHAR pszVMemo[], BYTE bRecIndex)
{
   HCONTROL          hControl;
   clk_julian_type   Julian;
   DWORD             dwCurrentProgress;
   VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();

   hControl = EQC_Animation( CREC_RECORD_ANIMATION_XPOS,            /* 녹음 에니메이션 */  
                             CREC_RECORD_ANIMATION_YPOS, 
                             &g_AniRecording,
                             NULL);           
   EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
   EQC_StartAnimation(hControl, TRUE);


   clk_secs_to_julian(0, &Julian);
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);

   hControl = EQC_StaticText(CREC_RECORD_RECORDINGTIME_XPOS, 
                             CREC_RECORD_RECORDINGTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);        /* 녹음시간             */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl, pszVMemo);


   //EQC_Image( CREC_RECORD_BACKGROUND_XPOS, 
   //           CREC_RECORD_BACKGROUND_YPOS, 
   //			  &g_BmpRemainTimePannel);
   
   
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), bRecIndex);   
   hControl = EQC_StaticText(CREC_RECORD_NUMBER_XPOS,        /* 녹음번호             */              
                             CREC_RECORD_NUMBER_YPOS,
                            (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl ,pszVMemo);

   dwCurrentProgress = GetVoiceMemoRemainTime();
   hControl = EQC_ProgressBar(CREC_RECORD_PBAR_XPOS, 
                              CREC_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              VM_MAX_PROGRESS, 
                              dwCurrentProgress,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */

   sprintf(pszVMemo,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(CREC_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             CREC_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszVMemo);

   /* hjcho 01.05.13 인사말녹음과 음성메모 분리               */
   /* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian);  */
   clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian);   

   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(CREC_RECORD_REMAINTIME_XPOS,             /* 남은 시간 */
                             CREC_RECORD_REMAINTIME_YPOS, 
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl ,pszVMemo);
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
//void sUpdateRemainTimeDisplay(DWORD dwRemainTime,BYTE pszVMemo[])
//{
//  clk_julian_type   Julian;
//
//  clk_secs_to_julian(dwRemainTime, &Julian);
//  sprintf((CHAR*)pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
//  EQC_SetStaticText(EQS_GetNthIOC(CR_TIME), pszVMemo);
//}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_EnterInProg(SKY_EVENT_T* pEvent)
**
** Description: 
**     녹음할 공간이 없거나 녹음키를 다시 눌러서 녹음이 종료되는 경우
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
//#if 0
//void SKY_API sSKA_EnterInProg(SKY_EVENT_T* pEvent)
//{
//   STATIC DWORD         dwRemainTime;
//   STATIC VMEMOINFO_T*  VoiceInfo;
//   CHAR                 pszVMemo[CR_MAX_TMPBUF];
//  
//   switch(pEvent->EventID)
//    {
//    /* EQC_APP */
//    case SE_APP_START : /* The very first event */
//      EQS_GetDC(TRUE);
//      EQS_SetWindow(SKY_DEF_WIN_X,
//            SKY_DEF_WIN_Y, 
//            SKY_DEF_WIN_DX, 
//            200, //SKY_DEF_WIN_DY, 
//            CL_MENU_NONE_NONE, 
//            NULL, 
//            WA_SHOW_CML);
//      VoiceInfo       = GetVoiceInfo();
//      dwRemainTime    = VoiceInfo->wRemainedTime;
//      sInitCallRecording(dwRemainTime, pszVMemo);
//      REC_voice_stop_traffic();
//      VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
//      sExitCallRecording();
//      SKY_AcceptKey(FALSE);
//        break;
//
//    case SE_APP_END :     /* The very last event */
//      EQS_ReleaseDC();
//        break;
//    
//    case SE_APP_SUSPEND : /* start child */
//        break;
//    
//    case SE_APP_RESUME :  /* end child */
//        break;
//    
//    case SE_APP_DRAW :    /* draw */
//        break;
//
//   case SE_VM_REC_END :  /* 녹음이 종료되는 경우 */
//      //SKY_SET_GETKEY(TRUE);
//      SKY_AcceptKey(TRUE);
//      EQS_END();
//        break;
//
//    case SE_KEY_RECORD:
//    case SE_KEY_PLAY:
//    case SE_KEY_END:
//    case SE_KEY_AUTO_ANSWER:
//        break;
//
//    /* EQC_KEY */
//    /* EQC_SOFTKEY */
//    /* EQC_SMS */
//    /* EQC_CM */
//    /* EQC_PH */
//    /* EQC_TM */
//    /* EQC_IN */
//    /* EQC_X */
//    default :
//        EQS_DEF_EVENT_PROC(pEvent);
//        break;
//    }
//}
//#endif //0



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
STATIC SKY_EVENT_E   bPreviousEvent;
void sSetPreviousEvent(SKY_EVENT_E bPreEvent)
{
  bPreviousEvent = bPreEvent;
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
SKY_EVENT_E sGetPreviousEvent()
{
  return bPreviousEvent;
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
extern BOOL IsEarpieceEnable(void);
//STATIC DWORD         dwRemainTime;
//STATIC WORD          wRecordingTime;

//#define sFAST_PACEMAKER_SECOND      1000    /* 1 sec */

void SKY_API SKA_StartCallRecording(SKY_EVENT_T* pEvent)
{
    STATIC RECDATA_T*    RECDataHeader;
    STATIC DWORD         dwRemainTime;
	STATIC WORD          wRecordingTime;
    STATIC VMEMOINFO_T*  VoiceInfo;
    STATIC BOOL          fFirst, fCallWaitingCall;
    WORD                 wCurrentProgress;
    BYTE                 pszVMemo[CR_MAX_TMPBUF];

  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :   /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_SAVE, 
            NULL, 
            WA_SHOW_CML | WA_SHOW_HEADER);
        fFirst          = TRUE;
        VoiceInfo       = GetVoiceInfo();
        RECDataHeader   = GetRECDataHeader();
        /* hjcho 01.05.13  인사말녹음과 음성녹음을 분리 */
        /* dwRemainTime    = VoiceInfo->wRemainedTime;  */
        dwRemainTime = GetVoiceMemoRemainTime();
        wRecordingTime  = 0;
        fCallWaitingCall = FALSE;
        sInitCallRecordingDisplay(dwRemainTime, pszVMemo, (VoiceInfo->bTotalRecNumber+1));
        /* 
        ** jrkwon 2001.06.28
        ** 통화 중 녹음 시 외부 LCD에 통화시간이 늦게 표시되는 문제 해결 
        */
        /* make faster for display call time */
        //SKY_SetPaceMakerTimer(sFAST_PACEMAKER_SECOND, TRUE);

        break;

    case SE_APP_END :     /* The very last event */
        EQS_ReleaseDC();
        /* restore pacemaker repeat time */
        //SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        if(EQS_GET_CHILD() == SKA_MainClid)
        {
          if(IsData2DisplayInCallWaitingCall())
          {
            EQS_START_CHILD_EVENT(SKA_CallWaitingCall, 
			                    SE_CALL_FROM_CALLRECORDING, 
								wRecordingTime, 
								dwRemainTime);
          }
        }
		else if(EQS_GET_CHILD() == SKA_CallWaitingCall)
		{
		  VoiceInfo = GetVoiceInfo();
		  wRecordingTime = Data_Record_time;
          /* 01.05.13                         인사말녹음과 음성녹음을 분리 */
		  /* dwRemainTime   = VoiceInfo->wRemainedTime - Data_Record_time; */
          dwRemainTime = GetVoiceMemoRemainTime() - Data_Record_time;

          /* Progress Bar */
		  EQC_SetProgressBar(EQS_GetNthIOC(CREC_PROGRESS), dwRemainTime);        
         
          /* 현재 녹음 진행 시각 */
          UpdateTime(wRecordingTime, pszVMemo);
          EQC_SetStaticText(EQS_GetNthIOC(CREC_RECORDING_TIME), pszVMemo);

          /* 남은 시간 */
          UpdateTime(dwRemainTime, pszVMemo);
          EQC_SetStaticText(EQS_GetNthIOC(CREC_REMAIN_TIME), pszVMemo);
		  EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
		}
        break;
    
    case SE_APP_DRAW :    /* draw */  
	  /* REC 이미지 */
	  EQS_PutBitmap(CREC_RECORD_REC_NUM_IMAGE_XPOS,
                    CREC_RECORD_REC_NUM_IMAGE_YPOS,
		    	    &g_BmpRecordCount);	
      /* 점선 */
      EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                    VM_HORIZONTAL_LINE_YPOS,
                    &g_BmpHorizontalLine);
        break;

    case SE_SYS_TO_IDLE:  /* ignore */
        break; 

    case SE_KEY_CLEAR:
        break;

    case SE_KEY_RECORD_LONG:
      if(fFirst == TRUE)
      {
       REC_voice_start_traffic(WorkingFn);
       SKY_AcceptKey(TRUE);
       EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
       fFirst = FALSE;
      }
        break;

	case SE_KEY_SEND:
	   if(fCallWaitingCall == TRUE)
	   {
         sSetPreviousEvent(pEvent->EventID);
         REC_voice_stop_traffic();
         sExitCallRecording();
	   }
	   break;

    case SE_KEY_RECORD:
	case SE_KEY_EAR_LONG_CLICK:
	case SE_SOFTKEY_SAVE:
    case SE_KEY_END:
    case SE_CALL_ENDED:    
    case SE_SYS_OFFLINE:
      sSetPreviousEvent(pEvent->EventID);
      REC_voice_stop_traffic();
      sExitCallRecording();
      break;

    case SE_KEY_PHONE_CLOSE:
	  //if(IsEarpieceEnable())
	  if(SKY_IS_EAR_JACK_OFF())
	  {
        sSetPreviousEvent(pEvent->EventID);
        REC_voice_stop_traffic();
        sExitCallRecording();
	  }
	  break;

    case SE_VM_REC_END :  /* Call이 종료되어서 녹음이 종료되는 경우 */
      switch(sGetPreviousEvent())
      {
	    case SE_KEY_RECORD:
	    case SE_KEY_EAR_LONG_CLICK:
	    case SE_KEY_PHONE_CLOSE:
	    case SE_SOFTKEY_SAVE:
		case SE_KEY_END:
		case SE_KEY_SEND:
        case SE_CALL_ENDED:
        case SE_SYS_OFFLINE:
             EQS_END_EVENT(sGetPreviousEvent(), EQS_NA, EQS_NA);
            break;

        default :
            EQS_END();
      }
      break;

    case SE_TM_TIMER1:
      /* jrkwon 2001.06.28 */
      EQS_SEND_EVENT(SE_SYS_PACEMAKER, 0, 0);

      dwRemainTime--;      

      if(dwRemainTime == 0)
      {
        /* REC_voice_stop_traffic();                     */
        /* VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO); */
        /* sExitCallRecording();                         */
		EQS_SEND_EVENT(SE_KEY_RECORD, 0, 0);
      }
      else
      {
        Data_Record_time++;
        wRecordingTime++;          

        /* Progress Bar */
        wCurrentProgress = EQC_GetProgressBar(EQS_GetNthIOC(CREC_PROGRESS));
        wCurrentProgress--;
        EQC_SetProgressBar(EQS_GetNthIOC(CREC_PROGRESS), wCurrentProgress);        
         
        /* 현재 녹음 진행 시각 */
        UpdateTime(wRecordingTime, pszVMemo);
        EQC_SetStaticText(EQS_GetNthIOC(CREC_RECORDING_TIME), pszVMemo);

        /* 남은 시간 */
        UpdateTime(dwRemainTime, pszVMemo);
        EQC_SetStaticText(EQS_GetNthIOC(CREC_REMAIN_TIME), pszVMemo);
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
      }    
      
      break;

  	case SE_TM_TIMER2 :
      EQS_START_CHILD(SKA_MainClid);
      break;

    case SE_KEY_PLAY:
      break;

    case SE_KEY_AUTO_ANSWER:
      break;

    case SE_CALL_DISPLAY : 
        /* 원래는 SE_CALL_CALLER_ID나 SE_CALL_EXT_DISPLAY가 와야 하는데 */
        /* SE_CALL_DISPLAY가 날아온다. --> Call Waiting Call            */
        /* 게으름이 SK 텔레콤 --                                        */
        /*                            1         2
        **                  0123456789012345678901 
        **   alpha.buf[] = "02/05 20:32 0112980012" 
        **   alpha.len = 22
        */
        {
            BYTE abPhoneNumber[MAX_DIAL_DIGIT_LENGTH+1];
            BYTE abTemp[256];
            INT8 nLength;
            fCallWaitingCall = TRUE;
            nLength = ((((GET_EVENT_PARAM()->call_event.call_info).alpha.len)
                        > MAX_DIAL_DIGIT_LENGTH) ? MAX_DIAL_DIGIT_LENGTH
                        : ((GET_EVENT_PARAM()->call_event.call_info).alpha.len));
            STRNCPY(abTemp, 
                    (GET_EVENT_PARAM()->call_event.call_info).alpha.buf, nLength);
            STRCPY(abPhoneNumber, abTemp + 12);
            SetupCLICDNIP((CHAR *) abPhoneNumber, nLength, 
                          GET_EVENT_PARAM()->call_event.call_info.num.pi, pEvent->EventID);
            //SaveCLI(CALLED_BY_INPROG);
            EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
        }
        break;

    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
#endif 
          /* call waiting, Save Incoming Phone Number in Call List */
	      fCallWaitingCall = TRUE;
          if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && 
             (pEvent->EventID != SE_CALL_INCOMING))
          {
            SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);
          }

	      EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
          break;

    
    case SE_KEY_MANNERS: /* 통화 중 녹음에서는 에티켓 키 먹지 않음 */
          break;
 
    case SE_CALL_COMMAND_ERROR: /* Ignore this event */
		  break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :
        CallDefaultEventProc(pEvent, INPROG);
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
void SKY_API SKA_MainCallRecord(SKY_EVENT_T* pEvent)
{
   STATIC DWORD         dwRemainTime;
   STATIC VMEMOINFO_T*  VoiceInfo;
   BYTE                 pszVMemo[CR_MAX_TMPBUF];
   STATIC HCONTROL      hControl, hCtrlFull, hCtrlKeyEnd, hCtrlRecord;
   STATIC BOOL          fFirst, fReturnFromRecording;
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_SAVE, 
            NULL, 
            WA_SHOW_CML | WA_SHOW_HEADER);
        fFirst  =  TRUE;
        fReturnFromRecording = FALSE;
        VoiceInfo       = GetVoiceInfo();
        /* hjcho 01.05.13  인사말녹음과 음성녹음을 분리 */
        /* dwRemainTime    = VoiceInfo->wRemainedTime;  */
        dwRemainTime = GetVoiceMemoRemainTime();
        sInitCallRecordingDisplay(dwRemainTime, pszVMemo, (VoiceInfo->bTotalRecNumber+1));
        break;

    case SE_APP_END :     /* The very last event */
        EQS_ReleaseDC();
        SKY_AcceptKey(FALSE);
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* 녹음 내용을 저장!! */
        fReturnFromRecording = TRUE;
		fFirst = FALSE;
        VoiceInfo  = GetVoiceInfo();
        /* hjcho 01.05.13 인사말녹음과 음성메모를 분리                  */
		/* dwRemainTime   = VoiceInfo->wRemainedTime - Data_Record_time;*/
        dwRemainTime = GetVoiceMemoRemainTime() - Data_Record_time;

        // 총녹음 건수 
        sprintf(pszVMemo, "%2d", VoiceInfo->bTotalRecNumber);
        EQC_SetStaticText(EQS_GetNthIOC(CREC_RECORDING_INDEX), pszVMemo);

        /* Progress Bar */
        //EQC_SetProgressBar(EQS_GetNthIOC(CREC_PROGRESS), VoiceInfo->wRemainedTime);        
		EQC_SetProgressBar(EQS_GetNthIOC(CREC_PROGRESS), dwRemainTime);        
         
        /* 녹음 시간 */
        EQC_SetStaticText(EQS_GetNthIOC(CREC_RECORDING_TIME), SKY_GET_TOKEN(TKN_BLANK));

        /* 남은 시간 */
        //UpdateTime(VoiceInfo->wRemainedTime, pszVMemo);
		UpdateTime(dwRemainTime, pszVMemo);
        EQC_SetStaticText(EQS_GetNthIOC(CREC_REMAIN_TIME), pszVMemo);
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;
    
    case SE_APP_DRAW :    /* draw */
        /* REC 이미지 */
        EQS_PutBitmap(CREC_RECORD_REC_NUM_IMAGE_XPOS,
                      CREC_RECORD_REC_NUM_IMAGE_YPOS,
                      &g_BmpRecordCount);
        /* 점선 */
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                      VM_HORIZONTAL_LINE_YPOS,
                      &g_BmpHorizontalLine);
        break;

    case SE_KEY_RECORD_LONG:
        if(fFirst == TRUE)
        {
          if(sIsRecord() && ((WorkingFn = VoiceRecordTransacStart(VTYPE_MEMO)) < NV_MAX_FL_FILE_NUM))
          {
            EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, FALSE);
			SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
            gnVocoderMode = VR_CALLREC;
          }
          else
          {
            hCtrlFull = EQC_NoBeepMessageBox((BYTE*)SKY_GET_TOKEN(TKN_VMEMO_NOSPACE), MESSAGE_DEFAULT_TIME, MBT_NONE, 
                                        MBT_FAIL);
            //EQC_SET_STYLE(hCtrlFull, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
          }
          fFirst = FALSE;
        }
        break;


     case SE_IO_MESGBOX_END:
          if(pEvent->wParam == hCtrlFull) /* 녹음 내용을 저장할 공간이 없는 경우 */
          {
            EQS_END();
          }
          else if((pEvent->wParam == hControl) || (pEvent->wParam == hCtrlKeyEnd))
          {
            VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
			EQS_END_EVENT(sGetPreviousEvent(), EQS_NA, EQS_NA);
          }
		  else if(pEvent->wParam == hCtrlRecord)
		  {
            VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
            EQS_END();
            //hCtrlRecordEnd =  EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 2, MBT_NONE, 
            //                                 MBT_SUCCESS);            
            //EQC_SET_STYLE(hCtrlRecordEnd, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
			//EQS_END_EVENT(SE_VM_KEY_RECORD_LONG_DUMMY, EQS_NA, EQS_NA);
		  }
          break;

     case SE_TM_TIMER1:
		fReturnFromRecording = FALSE;
        EQS_START_CHILD_EVENT(SKA_StartCallRecording, SE_KEY_RECORD_LONG, EQS_NA, EQS_NA);        
        break;

    case SE_KEY_PLAY:
    case SE_KEY_AUTO_ANSWER:
        break;

    case SE_KEY_PHONE_CLOSE:
		if((fReturnFromRecording == TRUE) && (SKY_IS_EAR_JACK_OFF()))//(IsEarpieceEnable()))
		{
          snd_mute_control_type MUTE_IN = SND_MUTE_UNMUTED;
          
          if (SKY_IS_MUTE()) 
          {
            MUTE_IN = SND_MUTE_MUTED;
          }
          SKY_SetSoundDevice(SND_MUTE_UNMUTED, MUTE_IN, TRUE);
          fReturnFromRecording = FALSE;
          hCtrlKeyEnd = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 0, MBT_NONE, 
                                       MBT_HELP);
          //EQC_SET_STYLE(hCtrlKeyEnd, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//		  VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
        }
		else
		{
		  EQS_END_CURRENT_EVENT();
		}
		break;

    case SE_KEY_EAR_LONG_CLICK:
	case SE_KEY_END:
    case SE_KEY_SEND:
        if(fReturnFromRecording == TRUE)
        {
          snd_mute_control_type MUTE_IN = SND_MUTE_UNMUTED;
          
          if (SKY_IS_MUTE()) 
          {
            MUTE_IN = SND_MUTE_MUTED;
          }
          SKY_SetSoundDevice(SND_MUTE_UNMUTED, MUTE_IN, TRUE);
          fReturnFromRecording = FALSE;
          hCtrlKeyEnd = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 0, MBT_NONE, 
                                       MBT_HELP);
//          EQC_SET_STYLE(hCtrlKeyEnd, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        }
		else
		{
		  EQS_END_CURRENT_EVENT();
		}
		break;

        /* 저장 중이라는 메시지 박스가 떠 있는 상태에서 END KEY를 눌렀을 경우 */
        /* 날아오는 이벤트. 통화를 끊기 위해 SE_KEY_END를 SKA_InProg로 보낸다.*/
    case SE_IO_TO_IDLE:
        //EQS_StartTimer(EQS_TIMER_2, 3000, FALSE);
        break;

    case SE_TM_TIMER2:
        EQS_END_EVENT(SE_KEY_END, EQS_NA, EQS_NA);
        break;


    case SE_SOFTKEY_SAVE:
        if(fReturnFromRecording == TRUE)
        {
          snd_mute_control_type MUTE_IN = SND_MUTE_UNMUTED;

		  sSetPreviousEvent(SE_SOFTKEY_SAVE);
          
          if (SKY_IS_MUTE()) 
          {
            MUTE_IN = SND_MUTE_MUTED;
          }
          SKY_SetSoundDevice(SND_MUTE_UNMUTED, MUTE_IN, TRUE);
          fReturnFromRecording = FALSE;
          hCtrlRecord = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 0, MBT_NONE, 
                                             MBT_HELP);
          //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//		  VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
        }
        break;

    case SE_KEY_RECORD:
		if(fReturnFromRecording == TRUE)
		{			
		  sSetPreviousEvent(SE_KEY_RECORD);         
          SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
          fReturnFromRecording = FALSE;
          hCtrlRecord = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 0, MBT_NONE, 
                                       MBT_HELP);
          //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);		
		}
		else
		{
		  EQS_END();
		}
		break;
    
    case SE_SYS_TO_IDLE:
		break;

    case SE_CALL_ENDED:
    case SE_SYS_OFFLINE:
		if(fReturnFromRecording == TRUE)
		{
		  snd_mute_control_type MUTE_IN = SND_MUTE_UNMUTED;

		  sSetPreviousEvent(pEvent->EventID);		

		  if (SKY_IS_MUTE()) 
		  {
		   MUTE_IN = SND_MUTE_MUTED;
		  }
		  SKY_SetSoundDevice(SND_MUTE_UNMUTED, MUTE_IN, TRUE);
	  	  hControl = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 0, MBT_NONE, 
						            MBT_HELP);
//		  EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//		  VoiceRecordTransacEnd(WorkingFn, VTYPE_MEMO);
		}
		else
		{
		  EQS_END_CURRENT_EVENT();
		}
       break;

    case SE_KEY_MANNERS: /* 통화 중 녹음에서는 에티켓 키 먹지 않음 */
        break;

    case SE_CALL_COMMAND_ERROR: /* Ignore this event */
		break;

    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
    default :

        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}