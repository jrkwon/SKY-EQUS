/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_vmemo.c
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
** 2000-12-12   hjcho      Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include "ska_vcmm.h"
#include "voc.h"
#include "FileSys.h"




/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define VM_MAX_TMPBUF                     30


/*
  녹음/재생 대기화면
*/
enum tagRECORD_VMEMO_E
{
  RV_ANIMATION  = 0,
  //RV_BACKGROUND   ,
  RV_RECNUMBER    ,
  RV_PROGRESS     ,
  RV_REMAIN_TIME_TITLE,
  RV_REMAIN_TIME
};


/*
  녹음 중 화면
*/
enum tagRECORD_PROGRESSVMEMO_E
{
 RP_ANIMATION = 0,    
 RP_RECORDING_TIME,  
 //RP_BACKGROUND,       
 RP_RECORDING_INDEX,  
 RP_PROGRESS,  
 RP_REMAIN_TIME_TITLE,
 RP_REMAIN_TIME       
};

/*
  플레이 중 화면
*/
enum tagPLAYING_VMEMO_E
{
  PM_ANIMATION = 0,   
  PM_RECTYPE, 
  //PM_BACKGROUND,
  PM_INDEX,        
  PM_RECTIME        
};


enum tagPLAY_VMEMO_E
{
  PV_INFO_TITLE = 0,
  PV_INFO       ,
  PV_TIME       ,
  PV_METHOD     ,
  PV_HOW        ,
  PV_MAX        
};


enum tagPOPUP_E
{
  PU_PLAY_REC = 0, //녹음시작
  PU_DELETE        //전부삭제 
};


enum tagPOPUPPLAY_E
{
  PUP_START_PLAY = 0,
  PUP_DELETE  
};


enum tagPOPUPPLAYING_E
{
  PUP_PLAY_NEXT = 0,      /* 다음재생을 눌렀을 경우 */
  PUP_DELETE_ONE          /* 현재 재생 중인 내용을 지울 경우 */
};


SKY_TOKEN paVRecord[2] =    /* Hot Menu!! */
{
  TKN_VMEMO_START,          /* 녹음시작 */
  TKN_VMEMO_DELETE          /* 전부삭제 */
};


SKY_TOKEN paVPlay[2] =      /* Hot Menu!! */
{
  TKN_VMEMO_PLAY,           /* 내용재생   */
  TKN_VMEMO_DELETE_ONE      /* 삭제       */
};


//SKY_TOKEN paVPlaying[2] =   /* Hot Menu!! */
//{
//  TKN_VMEMO_PLAY_NEXT,      /* 다음재생   */
//  TKN_VMEMO_DELETE_ONE      /* 삭제       */
//};


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagVMPLAYKEYMODE_E
{
  VMP_KEY_UP = 0,
  VMP_KEY_DOWN,
  VMP_KEY_DELETE,
  VMP_KEY_NONE
};

typedef enum tagVMPLAYKEYMODE_E  VMPLAYKEYMODE_E;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

LOCAL void SKY_API sSKA_RecordVMemo(SKY_EVENT_T* pEvent);
LOCAL void SKY_API sSKA_PlayVMemo(SKY_EVENT_T* pEvent);
LOCAL void SKY_API sSKA_DeleteOneItem(SKY_EVENT_T* pEvent);
LOCAL void SKY_API sSKA_GoPlaying(SKY_EVENT_T* pEvent);
LOCAL void SKY_API sSKA_ViewRecordList(SKY_EVENT_T* pEvent);

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/ 
STATIC BYTE  bPlayIndex;
CHAR pszDayOfWeek[7][3] = {"월","화","수","목","금","토","일"};


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
void UpdateTime(DWORD dwCurrTime, CHAR pszVMemo[])
{
   clk_julian_type   Julian;
   clk_secs_to_julian(dwCurrTime, &Julian);
   sprintf(pszVMemo, 
           SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), 
           Julian.minute, 
           Julian.second);
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
void RecDataInit(BYTE bIDX)
{
    RECDATA_T*    RECDataHeader;   

    RECDataHeader = GetRECDataHeader();
	Read_Current_Fn(bIDX);
//	Current_Fn = Fn_sequence[IDX];

	flfclose(GetCurrentFn());
	if(!flfopen(GetCurrentFn() , RD_MODE))
	{
//		MSG_MED("file %d open error ",GetCurrentFn(),0,0);
	}

    flfread(GetCurrentFn() , (BYTE *)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);

//	MSG_MED("RecDataInit file %d idx =  %d", GetCurrentFn(), bIDX,0);
//	MSG_HIGH("CurrentDataHeader so %d t %d"
//		, RECDataHeader->wServiceOption, RECDataHeader->dwTime,0);

    if(RECDataHeader->wServiceOption == CAI_SO_VOICE_EVRC) 
		Play_rec_init(VOC_CAP_EVRC);
	else
		Play_rec_init(VOC_CAP_IS96A);
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
LOCAL BYTE*	sGetDateString(BYTE pszTime[], DWORD dwTime)
{
	clk_julian_type tm;


	if (dwTime <= ONE_YEAR) // invalid Time
	{
		STRCPY((BYTE*)pszTime, (BYTE*)SKY_GET_TOKEN(TKN_VMEMO_WRONG_TIME)); 
		return pszTime;
	}

	clk_secs_to_julian( dwTime, &tm );

	sprintf(pszTime, "%2d.%02d(%s)%2d:%02d%cM", 
			tm.month, 
			tm.day, 
            pszDayOfWeek[tm.day_of_week],
			(tm.hour <= 12) ? tm.hour : tm.hour - 12, 
			tm.minute,
			(tm.hour < 12) ? 'A' : 'P' // 오전 오후
			);

	return pszTime;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetPlayIndex()
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
LOCAL BYTE* sGetDateTimeStringNoDayofWeek(BYTE pszTime[], DWORD dwTime)
{
	clk_julian_type tm;

	if (dwTime <= ONE_YEAR) // invalid Time
	{
		STRCPY((BYTE*)pszTime, (BYTE*)SKY_GET_TOKEN(TKN_VMEMO_WRONG_TIME)); 
		return pszTime;
	}

	clk_secs_to_julian( dwTime, &tm );

	sprintf(pszTime, "%2d.%02d %2d:%02d%cM", 
			tm.month, 
			tm.day, 
			(tm.hour <= 12) ? tm.hour : tm.hour - 12, 
			tm.minute,
			(tm.hour < 12) ? 'A' : 'P' // 오전 오후
			);

	return pszTime;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetPlayIndex()
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
LOCAL void sSetPlayIndex(BYTE bValue)
{
  bPlayIndex = bValue;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetPlayIndex()
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
LOCAL BYTE sGetPlayIndex(void)
{
  return bPlayIndex;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitPlayVoiceMemoDisp()
**
** Description: 
**     Initialize Display in playing voice memo.
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
LOCAL void sInitPlayVoiceMemoDisp(BYTE pszVMemo[])
{
   UINT8         nCtrl, nMAXCtrl;
   HCONTROL      hControl;
   VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();
   RECDATA_T*    CurrentDataHeader = GetRECDataHeader();


// 녹음내용    [2/3]
   EQC_StaticText(EQS_NA, EQS_NA, (EQS_TOKEN)TKN_VMEMO_INFO);
   if(VoiceInfo->bTotalRecNumber > 0)
   {
     sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_INFO), sGetPlayIndex()+1, 
                                                          VoiceInfo->bTotalRecNumber);
   }
   else
   {
     sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_INFO), 0, 
                                                          VoiceInfo->bTotalRecNumber);
   }

   hControl = EQC_StaticText(EQS_NA, EQS_NA, (EQS_TOKEN)TKN_BLANK);
   EQC_SetStaticText(hControl, pszVMemo);   

// 12/21(목)09:06PM 
   sGetDateString(pszVMemo, CurrentDataHeader->dwTime);   
   hControl = EQC_StaticText(EQS_NA, EQS_NA, (EQS_TOKEN)TKN_BLANK);
   EQC_SetStaticText(hControl, (BYTE*)pszVMemo);

// 길게 : 내용재생 
   EQC_StaticText(EQS_NA, EQS_NA,(EQS_TOKEN)TKN_VMEMO_LONG);
   EQC_StaticText(EQS_NA, EQS_NA,(EQS_TOKEN)TKN_VMEMO_PLAY);
   
   nMAXCtrl = PV_MAX;

   EQS_StartFormatter();
   for(nCtrl = 0; nCtrl <nMAXCtrl  ; nCtrl ++)
    {
       EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
       if((nCtrl != PV_INFO_TITLE) && (nCtrl != PV_METHOD))
          EQS_NewLine();           
    }
   EQS_EndFormatter();
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sUpdatePlayVoiceMemoDisp()
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
LOCAL void sUpdatePlayVoiceMemoDisp(BYTE pszVMemo[])
{
   VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();
   RECDATA_T*    CurrentDataHeader = GetRECDataHeader();
    
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_INFO), sGetPlayIndex()+1, 
                                                        VoiceInfo->bTotalRecNumber);
   EQC_SetStaticText(EQS_GetNthIOC(PV_INFO), pszVMemo);

   sGetDateString(pszVMemo, CurrentDataHeader->dwTime);   
   EQC_SetStaticText(EQS_GetNthIOC(PV_TIME), (BYTE*)pszVMemo);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitPlayingVoiceMemoDisp()
**
** Description: 
**     Initialize Display in playing voice memo.
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
#define VM_SYSTEM_CHAR_HEIGHT			EQS_CharHeight('A')
#define VM_SYSTEM_CHAR_WIDTH			EQS_CharWidth('A')

/* 재생 중에 녹음시간을 나타내는 문자열 */
#define VM_PLAY_FMT_DATE_TIME_LENGTH	STRLEN("12.12(목)12:12PM")

#define VM_PLAY_RECORDING_TIME_XPOS     (SKY_SCREEN_WIDTH - (VM_SYSTEM_CHAR_WIDTH*VM_PLAY_FMT_DATE_TIME_LENGTH))/2//11//4//10 //4
#define VM_PLAY_RECORDING_TIME_YPOS     (120 - SKY_ANNUNCIATORS_HEIGHT - 5)

#define VM_PLAY_INDEX_XPOS              VM_RECORD_NUMBER_XPOS//21
#define VM_PLAY_INDEX_YPOS              VM_RECORD_NUMBER_YPOS //47 //45 //46

#define VM_PLAY_REC_TYPE_XPOS           58//68 //62
#define VM_PLAY_REC_TYPE_YPOS           (67 - SKY_ANNUNCIATORS_HEIGHT) //9  

#define VM_PLAY_BACKGROUND_XPOS         VM_RECORD_BACKGROUND_XPOS 
#define VM_PLAY_BACKGROUND_YPOS         VM_RECORD_BACKGROUND_YPOS //43//41 //45

#define VM_PLAY_REC_NUM_IMAGE_XPOS      VM_RECORD_REC_NUM_IMAGE_XPOS
#define VM_PLAY_REC_NUM_IMAGE_YPOS      VM_RECORD_REC_NUM_IMAGE_YPOS



LOCAL void sInitPlayingVoiceMemoDisp(BYTE pszVMemo[])
{
   HCONTROL      hControl;

   /* 재생 에니메이션 */  
   hControl = EQC_Animation( VM_NEW_PLAY_ANIMATION_XPOS,            
                             VM_NEW_PLAY_ANIMATION_YPOS, 
                             &g_AniPlaying,//&g_AniPlay51x34x256c,
                             NULL);           
   EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
   EQC_StartAnimation(hControl, TRUE);
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
LOCAL void sUpdatePlayingVoiceMemoDisp(BYTE bIDX, BYTE* pszVMemo, 
                                       RECDATA_T* CurrentDataHeader,
                                       BYTE nRecordingType)
{
    //DRAW_MODE     OldDrawMode;
	VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();

    //OldDrawMode = EQS_GetDrawMode();
    //EQS_SetDrawMode(DM_OR);

    switch(nRecordingType)
    {
     case VTYPE_NONE:
     case VTYPE_MEMO:    /* 음성녹음 */
         EQS_PutBitmap(VM_PLAY_REC_TYPE_XPOS,
                       VM_PLAY_REC_TYPE_YPOS,
                       &g_BmpRecordPannelRecordVoice);
	     break;

    case VTYPE_NEWANS:   /* 새자동응답 */
         EQS_PutBitmap(VM_PLAY_REC_TYPE_XPOS,
                       VM_PLAY_REC_TYPE_YPOS,
                       &g_BmpAutoAnswerPannelNewAutoAnswer);
		 break;

    case VTYPE_OLDANS:   /* 자동응답 */
         EQS_PutBitmap(VM_PLAY_REC_TYPE_XPOS,
                       VM_PLAY_REC_TYPE_YPOS,
                       &g_BmpAutoAnswerPannelAutoAnswer);
		 break;

    default:
         EQS_PutBitmap(VM_PLAY_REC_TYPE_XPOS,
                       VM_PLAY_REC_TYPE_YPOS,
                       &g_BmpRecordPannelRecordVoice);
		 break;
   } 

   /* Record Type Image */
   //EQS_TextOutStr(VM_PLAY_REC_TYPE_XPOS, VM_PLAY_REC_TYPE_YPOS, (BYTE *)szRecordType);

   /* Background Image */
   //EQS_PutBitmap(VM_PLAY_BACKGROUND_XPOS, 
   //              VM_PLAY_BACKGROUND_YPOS, 
   //				 &g_BmpRemainTimePannel);

   
   /* 인덱스 */
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_INFO), 
                     (sGetPlayIndex()+1), 
                     VoiceInfo->bTotalRecNumber);

   EQS_TextOutStr(VM_PLAY_INDEX_XPOS, 
                  VM_PLAY_INDEX_YPOS, 
                  (BYTE *)pszVMemo);


   /* 녹음한 시간 */
   sGetDateString(pszVMemo, CurrentDataHeader->dwTime);
   EQS_TextOutStr(VM_PLAY_RECORDING_TIME_XPOS, 
                  VM_PLAY_RECORDING_TIME_YPOS, 
                  (BYTE *)pszVMemo);

   //EQS_SetDrawMode(OldDrawMode);

   /* REC 이미지 */
   EQS_PutBitmap(VM_PLAY_REC_NUM_IMAGE_XPOS,
                 VM_PLAY_REC_NUM_IMAGE_YPOS,
				 &g_BmpRecordCount);

   /* 점선 */
   EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                 VM_HORIZONTAL_LINE_YPOS,
                 &g_BmpHorizontalLine);


#ifdef FEATURE_SKY_SUBLCD
    {
        clk_julian_type   Julian;
        clk_secs_to_julian(CurrentDataHeader->dwTime, &Julian);
        SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_PLAY_FMT), sGetPlayIndex()+1, VoiceInfo->bTotalRecNumber);
        DisplayDateTimeInSubLCD(Julian.month, Julian.day, Julian.day_of_week, Julian.hour, Julian.minute);
        //SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(SLCD_DATE_TIME_FMT), Julian.minute, Julian.second);
    }
#endif //#ifdef FEATURE_SKY_SUBLCD
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitRecordVoiceMemoDisp()
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
#define VM_RECORD_MESSAGE_XPOS          58
#define VM_RECORD_MESSAGE_YPOS          (67 - SKY_ANNUNCIATORS_HEIGHT)//43


void sInitRecordVoiceMemoDisp(CHAR pszVMemo[])
{
   HCONTROL          hControl;
   clk_julian_type   Julian;
   DWORD             dwCurrentProgress;
   VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();

   
   /* hjcho 01.05.13        인사말녹음과 음성녹음을 분리                  */
   clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian);

   EQC_Image(VM_RECORD_ANIMATION_XPOS,
             VM_RECORD_ANIMATION_YPOS,
   			 &g_BmpRecord);

   //EQC_Image( VM_RECORD_BACKGROUND_XPOS, 
   //           VM_RECORD_BACKGROUND_YPOS, 
   //		    &g_BmpRemainTimePannel);
    
   hControl = EQC_StaticText(VM_RECORD_NUMBER_XPOS,                
                             VM_RECORD_NUMBER_YPOS,
                            (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);

   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), VoiceInfo->bTotalRecNumber);   
   EQC_SetStaticText(hControl ,pszVMemo);
   
   /* hjcho 01.05.13         인사말녹음과 음성녹음을 분리                 */
   /* dwCurrentProgress = VoiceInfo->wRemainedTime;                       */
   dwCurrentProgress = GetVoiceMemoRemainTime();
   hControl = EQC_ProgressBar(VM_RECORD_PBAR_XPOS, 
                              VM_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              VM_MAX_PROGRESS, 
                              dwCurrentProgress,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE); /*포커스를 갖지 못하도록 하는 매크로*/


   sprintf(pszVMemo,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));       /*남은시간에 대한 타이틀*/
   hControl = EQC_StaticText(VM_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             VM_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszVMemo);


   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(VM_RECORD_REMAINTIME_XPOS,         /*남은 시간*/
                             VM_RECORD_REMAINTIME_YPOS, 
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
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
void sUpdateRecordVoiceMemoDisp(CHAR pszVMemo[])
{
    clk_julian_type   Julian;
    WORD              dwCurrentProgress;
    VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();

    //EQS_SEND_EVENT(SE_APP_DRAW, EQS_GetNthIOC(RV_BACKGROUND), EQS_NA);

    // 총녹음 건수 
    sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), VoiceInfo->bTotalRecNumber);
    EQC_SetStaticText(EQS_GetNthIOC(RV_RECNUMBER), pszVMemo);
    EQS_SEND_EVENT(SE_APP_DRAW, EQS_GetNthIOC(RV_RECNUMBER), EQS_NA);

    // 남은 시간
    /* hjcho 01.05.13 인사말녹음과 음성녹음을 분리                 */
    /* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian);      */
    clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian);
    sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(RV_REMAIN_TIME), pszVMemo);

    // Progress Bar 
    /* hjcho 01.05.13 인사말녹음과 음성녹음을 분리                 */
    /* dwCurrentProgress = VoiceInfo->wRemainedTime;               */
    dwCurrentProgress = GetVoiceMemoRemainTime();
    EQC_SetProgressBar(EQS_GetNthIOC(RV_PROGRESS), dwCurrentProgress);  
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
void sInitRecordVoiceMemoInRecording(DWORD dwCurrTime, CHAR pszVMemo[])
{
   HCONTROL          hControl;
   clk_julian_type   Julian;
   DWORD             dwCurrentProgress;   
   VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();


   hControl = EQC_Animation( VM_NEW_RECORD_ANIMATION_XPOS,            /* 녹음 에니메이션 */  
                             VM_NEW_RECORD_ANIMATION_YPOS, 
                             &g_AniRecording,//&g_AniRec51x34x256c,
                             NULL);           
   EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
   EQC_StartAnimation(hControl, TRUE);


   clk_secs_to_julian(dwCurrTime, &Julian);
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   
   hControl = EQC_StaticText(VM_RECORD_RECORDINGTIME_XPOS, 
                             VM_RECORD_RECORDINGTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);        /* 녹음시간             */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl, pszVMemo);

   /* 바탕 이미지 */
   //EQC_Image( VM_RECORD_BACKGROUND_XPOS, 
   //           VM_RECORD_BACKGROUND_YPOS, 
   //		    &g_BmpRemainTimePannel);
  
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), (VoiceInfo->bTotalRecNumber+1));   
   hControl = EQC_StaticText(VM_RECORD_NUMBER_XPOS,        /* 녹음번호             */              
                             VM_RECORD_NUMBER_YPOS,
                            (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl ,pszVMemo);
    
   /* hjcho 01.05.13 인사말녹음과 음성녹음을 분리        */
   /* dwCurrentProgress = VoiceInfo->wRemainedTime;      */
   dwCurrentProgress = GetVoiceMemoRemainTime();
   hControl = EQC_ProgressBar(VM_RECORD_PBAR_XPOS, 
                              VM_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              VM_MAX_PROGRESS, 
                              dwCurrentProgress,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE); /* 포커스를 갖지 못하도록 하는 매크로 */

   sprintf(pszVMemo,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(VM_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             VM_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszVMemo);

   /* hjcho 01.05.13 인사말녹음과 음성녹음을 분리            */
   /* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian); */
   clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian); 
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(VM_RECORD_REMAINTIME_XPOS,             /* 남은 시간 */
                             VM_RECORD_REMAINTIME_YPOS, 
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
VOID SKY_API SKA_CallIncomingInRecordingVMemo(SKY_EVENT_T* pEvent)
{
  CALLINCOMINGFWHERE CallIncomingFromW;

  switch(pEvent->EventID)
  {
   /* EQC_APP */
   case SE_APP_START : /* The very first event */
       gnVocoderMode = VR_RING;
       SKY_AcceptKey(FALSE);
       EQS_GetDC(TRUE);       
       break;

   case SE_APP_END:
       SKY_AcceptKey(TRUE);
       EQS_ReleaseDC(); 
       break;

   case SE_APP_DRAW:
       break;

   case SE_APP_SUSPEND:
       SKY_AcceptKey(TRUE);
       break;

   case SE_APP_RESUME:
       SKY_AcceptKey(FALSE);
       EQS_EndToEvent(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);
//       if(fSaving == FALSE)
//       {
//          EQS_EndToEvent(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);
//       }
//       else
//       {
//           /* 저장이 다 끝나지 않은 상태에서 애플릿을     */
//           /* 종료 시키려고 할 때는 200msec의 delay를 주고*/ 
//           /* 종료시킨다.                                 */
//           EQS_StartTimer(EQS_TIMER_2, 300, FALSE);
//       }
       break;

//   case SE_VM_REC_END :              /* 전화를 받기 위해 현재 녹음내용을 저장  */        
//       fSaving = TRUE;
//
//       if(CallIncomingFromW == CIFW_VMEMO)
//       {
//           Release_play_rec();
//           VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_MEMO);  
//       }
//       else if(CallIncomingFromW == CIFW_USERMENT)
//       {
//        //youandi 01/05/09
//        //자동응답 멘트 녹음 끝나고 녹음 내용에 경고음을 넣기 위해서 이 코드를 삽입함.
//        //자동응답 멘트 녹음은 일반녹음(not통화중)을 사용한다.
//		  //녹음 중 사용되는 ISR은 Fn_Packet_Record이며
//  	  //ISR이 종료되면서 자동응답 멘트가 저장되는 경우는 두 가지이다.
// 		  //1.ISR이 돌다가 저장 공간(시간?)이 모자르면 자체적으로 
//        // REC_Voice_stop을 실행시키고 REC_Voice_stop는 SE_VM_REC_END 이벤트를 발생시켜서
//		  // 여기까지오게 된다.
//		  //2. 사용자가 종료를 지시하는 키를 눌러서 ISR을 종료시킨다.
//		  // 이 경우 키 누름과 동시에 REC_voice_stop_with_no_event이 실행되고 
//		  // VOCODER_DELAY msec 딜레이가 실행된 후에 SE_VM_REC_END이 발생되어 여기까지 온다.
//		  // REC_Voice_stop나 REC_voice_stop_with_no_event는 범용함수이므로 바꾸면 안 되고 
//		  // 녹음멘트헤더 저장하기 직전인 이부분에서 경고음을 넣어주어야 한다.
//		  /////////////////////////////////////////////////////////////////////////////////
//		  MakeBeep();
//  	  /////////////////////////////////////////////////////////////////////////////////
//		  Release_play_rec();
//		  VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_USERMENT);
//       }
//       fSaving = FALSE;
//       break;

    case SE_TM_TIMER1:
       EQS_START_CHILD(SKA_MainClid);
       break;

//    case SE_TM_TIMER2:
//        fSaving = FALSE;
//        pEvent->EventID = SE_CALL_INCOMING;
//        pEvent->dwParam = EQS_NA;
//        pEvent->wParam  = EQS_NA;
//        EQS_DEF_EVENT_PROC(pEvent);
//       break;

    case SE_CALL_INCOMING:
    case SE_CALL_CALLER_ID :  /* UI_CALL_CALLER_ID_E: */
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY :/* UI_CALL_EXT_DISPLAY_E: */
#endif 
       if(pEvent->EventID == SE_CALL_INCOMING)
       {
          CallIncomingFromW = pEvent->wParam;
          REC_voice_stop_with_no_event();


          if(CallIncomingFromW == CIFW_VMEMO)
          {
             Release_play_rec();
             VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_MEMO);  
          }
          else if(CallIncomingFromW == CIFW_USERMENT)
          {
            //youandi 01/05/09
            //자동응답 멘트 녹음 끝나고 녹음 내용에 경고음을 넣기 위해서 이 코드를 삽입함.
            //자동응답 멘트 녹음은 일반녹음(not통화중)을 사용한다.
            //녹음 중 사용되는 ISR은 Fn_Packet_Record이며
            //ISR이 종료되면서 자동응답 멘트가 저장되는 경우는 두 가지이다.
            //1.ISR이 돌다가 저장 공간(시간?)이 모자르면 자체적으로 
            // REC_Voice_stop을 실행시키고 REC_Voice_stop는 SE_VM_REC_END 이벤트를 발생시켜서
            // 여기까지오게 된다.
            //2. 사용자가 종료를 지시하는 키를 눌러서 ISR을 종료시킨다.
            // 이 경우 키 누름과 동시에 REC_voice_stop_with_no_event이 실행되고 
            // VOCODER_DELAY msec 딜레이가 실행된 후에 SE_VM_REC_END이 발생되어 여기까지 온다.
            // REC_Voice_stop나 REC_voice_stop_with_no_event는 범용함수이므로 바꾸면 안 되고 
            // 녹음멘트헤더 저장하기 직전인 이부분에서 경고음을 넣어주어야 한다.
            /////////////////////////////////////////////////////////////////////////////////
            MakeBeep();
            /////////////////////////////////////////////////////////////////////////////////
            Release_play_rec();
            VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_USERMENT);
          }
       }
       if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && (pEvent->EventID != SE_CALL_INCOMING))
       {
         SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);      
       }
       EQS_StartTimer(EQS_TIMER_1, 200, FALSE);
      break;
   
    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:
       break;

   default :
     EQS_DEF_EVENT_PROC(pEvent);
       break;
  }
}


void SetVoiceInfoTestData()
{
  VMEMOINFO_T*         VoiceInfo;
  VoiceInfo     = GetVoiceInfo();

  VoiceInfo->bTotalRecNumber = 0;
  VoiceInfo->wRemainedTime   = 270;
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_ProgressRecord()
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
void SKY_API sSKA_ProgressRecord(SKY_EVENT_T* pEvent)
{
    STATIC DWORD         dwRecordingTime; 
    STATIC DWORD         dwRemainTime;
    STATIC VMEMOINFO_T*  VoiceInfo;
    STATIC RECDATA_T*    RECDataHeader;
    STATIC HCONTROL      hCtrlMessageBox, hCtrlMsgSaved;
	HCONTROL             hCtrlCancelMesgBox;
    STATIC BOOL          fFirst, fPopToRoot, fFirstShowUp;
    WORD                 wCurrentProgress;
    CHAR                 pszVMemo[VM_MAX_TMPBUF];      

  
  switch(pEvent->EventID)
  {
    /* EQC_APP */
    case SE_APP_START :    /* The very first event */
      EQS_GetDC(TRUE);      
      VoiceInfo       = GetVoiceInfo();      
      RECDataHeader   = GetRECDataHeader();
      dwRecordingTime = 0; 
      /* hjcho 01.05.13            인사말녹음과 음성녹음을 분리 */
      /* dwRemainTime    = VoiceInfo->wRemainedTime;            */
      dwRemainTime = GetVoiceMemoRemainTime();
      EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_SAVE, 
            SKY_GET_TOKEN(TKN_VMEMO_RECORD_TITLE), 
            WA_SHOW_HEADER | WA_SHOW_CML);       
      sInitRecordVoiceMemoInRecording(dwRecordingTime, pszVMemo);   
#ifdef FEATURE_SKY_SUBLCD
      {
        clk_julian_type   Julian;
        clk_secs_to_julian(dwRemainTime, &Julian);
        SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_RECORD_FMT), VoiceInfo->bTotalRecNumber+1);
        SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_RECROD_TIME_FMT), Julian.minute, Julian.second);
      }
#endif //#ifdef FEATURE_SKY_SUBLCD      
      fFirst  = TRUE;
	  fPopToRoot = FALSE;
      fFirstShowUp = TRUE;
       break;

    case SE_APP_DRAW :
	  /* REC 이미지 */
	  EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
                    VM_RECORD_REC_NUM_IMAGE_YPOS,
		    	    &g_BmpRecordCount);
      /* 점선 */
      EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                    VM_HORIZONTAL_LINE_YPOS,
                    &g_BmpHorizontalLine);
	  break;

    case SE_APP_END :     /* The very last event */
		SKY_SET_KEY_TONE(TRUE);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
#endif //#ifdef FEATURE_SKY_SUBLCD
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
		SKY_SET_KEY_TONE(TRUE);
        break;

    case SE_APP_RESUME :  /* end child */
		SKY_SET_KEY_TONE(TRUE);
        break;

    case SE_KEY_RECORD_LONG:   /* Long Key를 누르고 있으면 이벤트가 계속 들어오고 계속      */
      if(fFirst == TRUE)       /* Rec_voice_start를 하게 된다. 당근 녹음이 이상하게 됩니다. */
      {
        REC_voice_start(); 
        EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
        fFirst = FALSE;
      }
        break;   

    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:       /* Ignore this event */
		break;
    
	
	case SE_KEY_POWER_OFF:
    case SE_KEY_PHONE_CLOSE:
	case SE_KEY_END:
		fPopToRoot = TRUE;
    case SE_SOFTKEY_SAVE:    
    case SE_KEY_RECORD:  
        EQS_StopTimer(EQS_TIMER_1);
        /* 녹음 중에 녹음 짧은 키를 눌렀을 경우 --> 녹음을 저장  */
        /* Key Tone 소리 후 VOCODER_DELAY msec의 delay를 주기 위해           */
        /* REC_voice_stop에서 SE_VM_REC_END 이멘트를 날려주는 것을 */
        /* 막기 위해 REC_voice_stop을 다음 네 줄로 바꾸었다.       */
        SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
        tramp_set_isr(TRAMP_VOC_ENC_ISR, NULL);
        nVocoderReportControl               = IDLE_VOCODER_DOWN;
        RecoredVoiceData.dwRecordDataLength = dwVoiceDataOffset;
        SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
        EQS_StartTimer(EQS_TIMER_3, VOCODER_DELAY, FALSE);
        break; 

    case SE_KEY_CLEAR:        /* 녹음을 저장하지 않고 취소할 경우 */
    case SE_SOFTKEY_CANCEL:
        EQS_StopTimer(EQS_TIMER_1);
        SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE); 
        tramp_set_isr(TRAMP_VOC_ENC_ISR, NULL);
        nVocoderReportControl               = IDLE_VOCODER_DOWN;
        RecoredVoiceData.dwRecordDataLength = dwVoiceDataOffset;
        EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
        break;

    case SE_TM_TIMER1 :       /* 녹음 중에 사용되는 타이머*/
		SKY_SET_KEY_TONE(TRUE);
        dwRemainTime--;  

        if(dwRemainTime == 0)
        {
            /* 녹음용량을 다 채웠을 경우              */
			/* 녹음내용을 저장하고 대기상태로 나온다. */
			EQS_SEND_EVENT(SE_KEY_RECORD, 0, 0);
        }
        else 
        {
            dwRecordingTime++;
            Data_Record_time++;
            
            //EQS_SEND_EVENT(SE_APP_DRAW, EQS_GetNthIOC(RP_BACKGROUND), EQS_NA);
            
            /* Progress Bar */
            wCurrentProgress = EQC_GetProgressBar(EQS_GetNthIOC(RP_PROGRESS));
            wCurrentProgress--;
            EQC_SetProgressBar(EQS_GetNthIOC(RP_PROGRESS), wCurrentProgress);        

            /* 현재 녹음 진행 시각 */
            UpdateTime(dwRecordingTime, pszVMemo);
            EQC_SetStaticText(EQS_GetNthIOC(RP_RECORDING_TIME), pszVMemo);

            /* 남은 시간 */
            UpdateTime(dwRemainTime, pszVMemo);
            EQC_SetStaticText(EQS_GetNthIOC(RP_REMAIN_TIME), pszVMemo);
			EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
#ifdef FEATURE_SKY_SUBLCD
            {
                clk_julian_type   Julian;
                clk_secs_to_julian(dwRemainTime, &Julian);
                SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_RECORD_FMT), VoiceInfo->bTotalRecNumber+1);
                SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_RECROD_TIME_FMT), Julian.minute, Julian.second);
            }
#endif //#ifdef FEATURE_SKY_SUBLCD
       }
        break;

    case SE_TM_TIMER2 :
	   EQS_StopTimer(EQS_TIMER_1);
       Release_play_rec();       
       hCtrlCancelMesgBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_CANCEL), MESSAGE_DEFAULT_TIME, MBT_NONE,
                                           MBT_HELP);
       //EQC_SET_STYLE(hCtrlCancelMesgBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);       
//#ifdef FEATURE_SKY_SUBLCD
//        SKY_SubLCDDefault();
//#endif//#ifdef FEATURE_SKY_SUBLCD
        break;

    case SE_TM_TIMER3:
       /* 녹음저장종료메시지가 끝나기 전에 녹음키를 누르면 다시 녹음 저장중이라는 */
       /* 메시지가 나타나는 현상을 방지하기 위해                                  */

       if(fFirstShowUp == TRUE)
       {
          gnVocoderMode = VR_NONE;
          fFirstShowUp = FALSE;
          EQS_StopTimer(EQS_TIMER_1);
          hCtrlMessageBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVING), 0, MBT_NONE,
                                           MBT_HELP); 
       }
       //EQC_SET_STYLE(hCtrlMessageBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//#ifdef FEATURE_SKY_SUBLCD
//        SKY_SubLCDDefault();
//#endif//#ifdef FEATURE_SKY_SUBLCD
        break;


    case SE_CALL_INCOMING:
       SKY_SET_ALERT(TRUE);
       gnVocoderMode = VR_RING;
       EQS_START_SIBLING_EVENT(SKA_CallIncomingInRecordingVMemo, SE_CALL_INCOMING,
                               CIFW_VMEMO, EQS_NA, TRUE);
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrlMessageBox)
        {
          Release_play_rec();
          VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_MEMO);
		  hCtrlMsgSaved = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVE_SUCCESS), 1, MBT_NONE,
                                         MBT_SUCCESS); 
          //EQC_SET_STYLE(hCtrlMessageBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);		  
		}
		else if(pEvent->wParam == hCtrlMsgSaved)
		{
		  if(fPopToRoot == TRUE)
		  {
            fPopToRoot = FALSE;
			EQS_END_TO(EQS_POP_TO_ROOT);
		  }
		  else
		  {
			EQS_END();
		  }
		}
		else
		{
          EQS_END();
		}
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
void SKY_API sSKA_EraseAll(SKY_EVENT_T* pEvent)
{
    STATIC HCONTROL hControl, hCtrlEraseSuccess;
    STATIC   SKY_EVENT_E      PreviousEvent;

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_GetDC(TRUE);
      hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_DELETE_ALL), 0, MBT_NONE, 
                                MBT_SUCCESS);
      PreviousEvent = SE_KEY_NONE;
      //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);      
        break;

    case SE_APP_END :     /* The very last event */
      EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        break;

    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:
        break;

    case SE_KEY_END:      /* ignore this event!! */
    case SE_KEY_PHONE_CLOSE:
        PreviousEvent = pEvent->EventID;
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hControl)
        {
    		VoiceEraseAllTransac();
            hCtrlEraseSuccess = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_DELETEALL), 1,
                                                MBT_NONE, MBT_SUCCESS);
        }
        else if(pEvent->wParam == hCtrlEraseSuccess)
        {
            if((PreviousEvent == SE_KEY_END) ||
               (PreviousEvent == SE_KEY_PHONE_CLOSE))
            {
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            else
            {            
                EQS_START_SIBLING(sSKA_RecordVMemo, FALSE);
            }
        }
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
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sIsRecord()
**
** Description: 
**     Record a voice from a user and save that data.
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
BOOL sIsSpace2Record()
{
    VMEMOINFO_T*         VoiceInfo;

    VoiceInfo = GetVoiceInfo();

    if(((NV_MAX_FL_FILE_NUM-1) <= VoiceInfo->bTotalRecNumber)  ||
        (VoiceInfo->dwRemainedByte < VM_MIN_REMAINED_BYTE)     ||
        (GetVoiceMemoRemainTime() == 0))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_RecordVMemo()
**
** Description: 
**     Record a voice from a user and save that data.
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
LOCAL void SKY_API sSKA_RecordVMemo(SKY_EVENT_T* pEvent)
{
  STATIC RECDATA_T*    RECDataHeader;
  STATIC HCONTROL      hCtrlFull;
  STATIC HCONTROL      hCtrlDel;
  STATIC HCONTROL      hCtrlNoRecordItem, hCtrlMessageBox;
  HCONTROL             hControl, hCtrlPopUp;
  CHAR                 pszVMemo[VM_MAX_TMPBUF];
  VMEMOINFO_T*         VoiceInfo;
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :       /* The very first event */
       EQS_GetDC(TRUE);
       EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_LIST_PLAY, /* 메뉴 목록   재생 */
            SKY_GET_TOKEN(TKN_VMEMO_RECORD_PLAY_TITLE), 
            WA_SHOW_HEADER | WA_SHOW_CML);

      sInitRecordVoiceMemoDisp(pszVMemo);  
#ifdef FEATURE_SKY_SUBLCD
      SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD

	  /* 녹음/재생, 자동응답 후에는 VR_AUTO, VR_MEMO로 되어있는 경우가 */
	  /* 발생해서 벨소리가 나지 않을 수 있습니다. 이를 제거하기 위해   */
	  /* 추가했습니다.                                                 */
	   gnVocoderMode = VR_NONE;
       RECDataHeader = GetRECDataHeader();
         break;

    case SE_APP_DRAW :
        
        /* 길게:녹음시작 */
        EQS_PutBitmap(VM_RECORD_MESSAGE_XPOS,
				      VM_RECORD_MESSAGE_YPOS,
					  &g_BmpRecordPannelStartRecord);

		/* REC이미지 */
		EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
				      VM_RECORD_REC_NUM_IMAGE_YPOS,
					  &g_BmpRecordCount);  
        
        /* 점선 */
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                      VM_HORIZONTAL_LINE_YPOS,
                      &g_BmpHorizontalLine);
		 break;

    case SE_APP_END :     /* The very last event */
      EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */
      /* 녹음/재생, 자동응답 후에는 VR_AUTO, VR_MEMO로 되어있는 경우가 */
	  /* 발생해서 벨소리가 나지 않을 수 있습니다. 이를 제거하기 위해   */
	  /* 추가했습니다.                                                 */
	  gnVocoderMode = VR_NONE;
      if((EQS_GET_CHILD() == sSKA_ProgressRecord) || 
		 (EQS_GET_CHILD() == sSKA_PlayVMemo))
      {
		if(SKY_IS_PHONE_CLOSE())
		{
		  EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else
		{
		  sUpdateRecordVoiceMemoDisp(pszVMemo);
		}
      }
      else if(EQS_GET_CHILD() == sSKA_ViewRecordList)
      {
        if(SKY_IS_PHONE_CLOSE())
		{
		  EQS_END_TO(EQS_POP_TO_ROOT);
		}
		else
		{
		  sUpdateRecordVoiceMemoDisp(pszVMemo);
		}
      }
        break;
    
    case SE_KEY_RECORD_LONG:  
	   if((hControl = EQS_GetModal()) == INVALID_CONTROL_HANDLE) 

	   {
		  VoiceInfo     = GetVoiceInfo();  
		  if(((NV_MAX_FL_FILE_NUM-1) <= VoiceInfo->bTotalRecNumber)  ||
			 (VoiceInfo->dwRemainedByte  <VM_MIN_REMAINED_BYTE)      ||
             /* hjcho 01.05.13       인사말녹음과 음성녹음을 분리     */
             (GetVoiceMemoRemainTime() == 0))
		  {
#ifdef FEATURE_SKY_SUBLCD
                SKY_SubLCDTextOut(LN_UPPER, TKN_SLCD_NO_SPACE_4_RECORD_1);
                SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_NO_SPACE_4_RECORD_2);
                //SKY_SubLCDEndText(LN_UPPER);
#endif//#ifdef FEATURE_SKY_SUBLCD
			  /* 더이상 녹음내용을 저장할 물리적 공간이 없을 경우   */
			  /* 메시지 박스를 보여주고 현재상태를 그대로 유지한다. */
			  /* "더 이상 녹음할 공간이 없습니다."                  */
			  hCtrlFull = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOSPACE), MESSAGE_DEFAULT_TIME, MBT_NONE, 
										 MBT_FAIL);
		  }
		  else
		  {
			//youandi 01.06.28아이들에서 녹음하기
			// sSka_MainVMemo-->sSka_RecordVMemo(Play_rec_init)-->sSka_ProgressRecord(Rec_voice_start)
            //녹음하기 위햐서 렉 이니트를 해준다.
			//재생전에는 키톤을 울려줘야하는데 녹음 어떨지 모르겠다.
			Play_rec_init(VOC_CAP_EVRC);
			SetCurrentFn(VoiceRecordTransacStart(VTYPE_MEMO));
			RECDataHeader->wServiceOption = CAI_SO_VOICE_EVRC;
			if ((RECDataHeader->dwTime = SKY_GetLocalTime()) <= ONE_YEAR)
				 RECDataHeader->dwTime = (DWORD)0;
        
			flfwrite(GetCurrentFn(), (byte *)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);        
			EQS_START_CHILD_CURRENT_EVENT(sSKA_ProgressRecord);
		  } 
	   }
	   //else if(((hControl = EQS_GetModal()) == INVALID_CONTROL_HANDLE) && 
	   // 	       (fDummyRecordLong == TRUE))
	   //{
	   //	 fDummyRecordLong = FALSE;
	   //}
       break;

    case SE_KEY_PLAY:
        /* hjcho 2001-01-29 --> @why, what */
#if (MODEL_ID == MODEL_B0) 
        EQS_START_SIBLING_CURRENT_EVENT(SKA_MainVMemo, FALSE);
#elif (MODEL_ID == MODEL_B1)
        EQS_END_CURRENT_EVENT();
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
        /* <-- */
       break;


    case SE_KEY_CENTER:
    case SE_SOFTKEY_LIST:
        VoiceInfo = GetVoiceInfo();

        if(VoiceInfo->bTotalRecNumber == 0)
        {
           hCtrlNoRecordItem = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_NONE), MESSAGE_DEFAULT_TIME, 
                                              MBT_NONE, MBT_FAIL);
        }
        else
        {
          EQS_START_CHILD(sSKA_ViewRecordList);
        }
       break;

    case SE_TM_TIMER1 :  //UI_TIMER_F:
       break;

    case SE_IO_MESGBOX_YES :
      /* "OK" 라는 응답이 올 경우, 삭제를 시작한다.*/
      if(pEvent->wParam == hCtrlDel)
      {
        EQS_START_SIBLING(sSKA_EraseAll, TRUE);
      }
       break;

    case SE_SOFTKEY_MENU:
        VoiceInfo   = GetVoiceInfo();

        if((VoiceInfo->bTotalRecNumber > 0) && sIsSpace2Record())
        {
          BOOL      paActive[2] = {TRUE, TRUE};
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paVRecord, 2, 0,
                                              paActive);            
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_START);
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE);
        }
        else if((VoiceInfo->bTotalRecNumber == 0) && sIsSpace2Record())
        {
          BOOL      paActive[2] = {TRUE, FALSE};
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paVRecord, 2, 0,
                                              paActive);            
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_START);
        }
        else if((VoiceInfo->bTotalRecNumber > 0) && !sIsSpace2Record())
        {
          BOOL      paActive[2] = {FALSE, TRUE};
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paVRecord, 2, 0,
                                              paActive);            
          EQC_EnablePopupMenuItem(hCtrlPopUp, TKN_VMEMO_DELETE);
        }
        else if((VoiceInfo->bTotalRecNumber == 0) && !sIsSpace2Record())
        {
          BOOL      paActive[2] = {FALSE, FALSE};
          hCtrlPopUp = EQC_ExceptPopUpMenu((EQS_TOKEN_EX *)paVRecord, 2, 0,
                                              paActive);            
        }       
       break;

    case SE_SOFTKEY_PLAY: 
       VoiceInfo     = GetVoiceInfo();
       if(VoiceInfo->bTotalRecNumber == 0) /*녹음된 내용이 하나도 없을 경우 */
       {
          hCtrlMessageBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_NONE), 
                                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL);           
       }
       else
       {
         EQS_START_CHILD_EVENT(sSKA_PlayVMemo, SE_KEY_PLAY_LONG, EQS_NA, EQS_NA);        
       }
       break;

    case SE_IO_MESGBOX_NO:
    case SE_IO_MESGBOX_END:
        /* 사용자의 확인이 없는 경우, Message Box를 닫고 그냥 내버려 둔다. */
        /* id lcd 디폴트 표시 */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        break;

    case SE_IO_POPUP_SELECT:
       switch(pEvent->dwParam)
       {
         case PU_PLAY_REC:
           EQS_SEND_EVENT(SE_KEY_RECORD_LONG, 0, 0);
            break;

         case PU_DELETE:
            VoiceInfo     = GetVoiceInfo();  

            if(VoiceInfo->bTotalRecNumber >0)
            {
              hCtrlDel = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETEALL), MESSAGE_DEFAULT_TIME, MBT_YESNO, 
                                      MBT_CONFIRM);
            }
            else if(VoiceInfo->bTotalRecNumber == 0)
            {
              hControl = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_NONE), MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL); 
            }
          //EQC_SET_STYLE(hCtrlDel, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
          /* 녹음된 내용을 전부 지울까요? 라는 메시지를 보여준다. */
          /* '예'라는 응답이 올 경우에 삭제를 시작한다.           */
            break;
       }
       break;

    case SE_IO_TO_IDLE:
	case SE_SYS_TO_IDLE:
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
VOID SKY_API SKA_CallIncomingInPlayingVMemo(SKY_EVENT_T* pEvent)
{
  STATIC BOOL fReleaseVocoder;
    
  switch(pEvent->EventID)
  {
   /* EQC_APP */
   case SE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
       fReleaseVocoder = FALSE;
       SKY_AcceptKey(FALSE);
       EQS_StartTimer(EQS_TIMER_1, VOCODER_DELAY, FALSE);
	   SKY_SET_KEY_TONE(FALSE);
       break;

   case SE_APP_END:
       SKY_AcceptKey(TRUE);
	   SKY_SET_KEY_TONE(TRUE);
       EQS_ReleaseDC(); 
       break;

   case SE_APP_SUSPEND:
       SKY_AcceptKey(TRUE);
	   SKY_SET_KEY_TONE(TRUE);
       break;

   case SE_APP_DRAW:       
       break;

   case SE_APP_RESUME:
	   SKY_SET_KEY_TONE(TRUE);
       SKY_AcceptKey(FALSE);
       if(fReleaseVocoder == TRUE)
       {
        EQS_EndToEvent(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);
       }
       else 
       {
         /* Vocoder를 Release하지 않은 상태이면 */
         /* 100 msec의 delay를 더 준다          */
         EQS_StartTimer(EQS_TIMER_3, 100, FALSE);
       }
       break;

   case SE_TM_TIMER1:
	   SKY_SET_KEY_TONE(TRUE);
       if(fReleaseVocoder == FALSE)
       {
         Release_play_rec();
         fReleaseVocoder = TRUE;
       }
       break;

    case SE_TM_TIMER2:
       EQS_START_CHILD(SKA_MainClid);
       break;
    
    case SE_TM_TIMER3:
       EQS_EndToEvent(EQS_POP_TO_ROOT, SE_CALL_INCOMING, EQS_NA, EQS_NA);
       break;

    case SE_CALL_INCOMING    :
    case SE_CALL_CALLER_ID   :  /* UI_CALL_CALLER_ID_E: */
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY :  /* UI_CALL_EXT_DISPLAY_E: */
#endif 
      if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && 
         (pEvent->EventID != SE_CALL_INCOMING))
      {
         SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);      
      }
      EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
      break;
   
    case SE_IO_TO_IDLE:
	case SE_SYS_TO_IDLE:
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
**     sSKA_GoPlaying()
**
** Description: 
**     Play a saved voice memo.
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
//#define ROLL_PLAY_CNT    (DWORD)30     /* Roll over call count*/


LOCAL void SKY_API sSKA_GoPlaying(SKY_EVENT_T* pEvent)
{
    BYTE                     pszVMemo[VM_MAX_TMPBUF];
    STATIC BOOL              fContinue, fUseTimer, fCompleteUp, fCompleteDown, fIsDelete;
	STATIC SKY_EVENT_E       PreviousEvent;
    VMEMOINFO_T*             VoiceInfo;
    BYTE                     bIndex;
    STATIC VMPLAYKEYMODE_E   KeyMode;
    HCONTROL                 hCtrlDelete;
    STATIC RECDATA_T*        CurrentDataHeader; 
    STATIC BYTE              bRecordingType;
    //STATIC DWORD             dwUpDownKeyCount;
    

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
       EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_DELETE_MOVE_NONE,   //CL_NONE_MOVE_DELETE, 
            SKY_GET_TOKEN(TKN_VMEMO_PLAY), 
            WA_SHOW_HEADER | WA_SHOW_CML);

       Read_Current_Fn(sGetPlayIndex());
       CurrentDataHeader = GetRECDataHeader();
       flfclose(GetCurrentFn());
       flfopen(GetCurrentFn() , RD_MODE);
       flfread(GetCurrentFn() , (BYTE *)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);
       flfclose(GetCurrentFn());
     
       sInitPlayingVoiceMemoDisp(pszVMemo);
       gnVocoderMode = VR_RING;
       bRecordingType = GetRecordType(sGetPlayIndex());
       fIsDelete = FALSE;
       fContinue = FALSE;
	   fUseTimer = TRUE;  
       fCompleteUp = TRUE;
       fCompleteDown = TRUE;
       KeyMode = VMP_KEY_NONE;
	   PreviousEvent = SE_KEY_NONE; 
       //dwUpDownKeyCount = 0;
        break;

    case SE_APP_END : /* The very last event */
        SKY_SET_KEY_TONE(TRUE);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDDefault();
#endif//#ifdef FEATURE_SKY_SUBLCD
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
		SKY_SET_KEY_TONE(TRUE);
        break;

    case SE_APP_RESUME :  /* end child */
		SKY_SET_KEY_TONE(TRUE);
        break;

    case SE_APP_DRAW :    /* draw */
        sUpdatePlayingVoiceMemoDisp(sGetPlayIndex(), pszVMemo, CurrentDataHeader, bRecordingType);
        break;

    case SE_KEY_PLAY_LONG:
        if(fContinue == FALSE)
        {          
          /* 재생전에 sound task에 siginal을 줘서 sound task를 깨워  줄 필요가 있음*/
          /* 여기서는 Key Tone을 통해서 sound task를 sleep상태에서 깨워줌          */
          /* 깨워 주지 않으면 Phone이 팍팍 죽습니다.       YouandI, Hjcho          */
          SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
          EQS_StartTimer(EQS_TIMER_1, VOCODER_DELAY, FALSE);
          fContinue = TRUE;
          SKY_SET_KEY_TONE(FALSE);
        }
        else
        {
          PLAY_voice_stop();
        }
        break;

    case SE_TM_TIMER1:
		SKY_SET_KEY_TONE(TRUE);
        RecDataInit(sGetPlayIndex());
        Fn_PLAY_voice_start(GetCurrentFn());
#ifdef FEATURE_SKY_SUBLCD
        {
            clk_julian_type   Julian;
            VoiceInfo = GetVoiceInfo();
			CurrentDataHeader = GetRECDataHeader(); 

            clk_secs_to_julian(CurrentDataHeader->dwTime, &Julian);
            SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_PLAY_FMT), sGetPlayIndex()+1, VoiceInfo->bTotalRecNumber);
            DisplayDateTimeInSubLCD(Julian.month, Julian.day, Julian.day_of_week, Julian.hour, Julian.minute);
            //SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(SLCD_DATE_TIME_FMT), Julian.minute, Julian.second);
        }
#endif //#ifdef FEATURE_SKY_SUBLCD
        break;

    case SE_TM_TIMER2:        
		CurrentDataHeader = GetRECDataHeader();
		if(CurrentDataHeader->wServiceOption == CAI_SO_VOICE_EVRC)
			Play_rec_init(VOC_CAP_EVRC);
		else
			Play_rec_init(VOC_CAP_IS96A);

        Fn_PLAY_voice_start(GetCurrentFn());
		SKY_SET_KEY_TONE(TRUE);
		fUseTimer = TRUE;
        break;

    case SE_TM_TIMER3:
		Release_play_rec();
		SKY_SET_KEY_TONE(TRUE);
		if((PreviousEvent == SE_KEY_PHONE_CLOSE) || 
           (PreviousEvent == SE_KEY_END))
		{		  
            EQS_END_TO(EQS_POP_TO_ROOT);
		}
        else if(PreviousEvent == SE_SOFTKEY_DELETE)
        {
            PreviousEvent = SE_KEY_NONE;
            break;
        }
		else
		{
		  EQS_END();
		}
		break;

    case SE_VM_PLAY_CONTINUE :
        if(fUseTimer == TRUE)
		{
          bRecordingType = GetRecordType(sGetPlayIndex());
          SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
          /* 키톤이 울린 후 VOCODER_DELAY msec의 delay를 준다. */
          EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
		  SKY_SET_KEY_TONE(FALSE);
		  fUseTimer = FALSE;
          EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		}
        break;    

    case SE_VM_PLAY_END:            
        bIndex = sGetPlayIndex();
        VoiceInfo = GetVoiceInfo();
        flfclose(GetCurrentFn());
        /* 자동으로 재생하다가 끝냈을 경우 */
        if(((bIndex + 1) == VoiceInfo->bTotalRecNumber) && (KeyMode == VMP_KEY_NONE))
        {
          SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
          EQS_SEND_EVENT(SE_KEY_CLEAR, EQS_NA, EQS_NA);
        }
        else if(KeyMode == VMP_KEY_NONE)  /* 더 재생할 것이 있을 경우   */
        {
          bIndex++;
          sSetPlayIndex(bIndex);
		  CurrentDataHeader = GetRECDataHeader(); 
          Read_Current_Fn(sGetPlayIndex());
          flfopen(GetCurrentFn() , RD_MODE);
          flfread(GetCurrentFn() , (BYTE *)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);

          EQS_SEND_EVENT(SE_VM_PLAY_CONTINUE, 0, 0);
        }
       else if(KeyMode == VMP_KEY_DELETE)
       {
          sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_DELETEONE2), sGetPlayIndex()+1);          
          fIsDelete = TRUE;
          KeyMode = VMP_KEY_NONE;
          gnVocoderMode = VR_NONE;
          PreviousEvent = SE_SOFTKEY_DELETE;
          hCtrlDelete = EQC_MessageBox((BYTE*)pszVMemo, 0, MBT_YESNO, MBT_CONFIRM);
          SKY_SET_KEY_TONE(TRUE);
          /* 삭제 여부를 묻는 메시지박스가 떠 있는 동안 vocoder를 꺼준다.   */
          /* VOCODER_DELAY msec의 delay를 주는 이유는 Key Tone이 깨지는 것을 방지하기 */
          /* 위함이다.                        hjcho                         */
          EQS_StartTimer(EQS_TIMER_3, VOCODER_DELAY, FALSE);
		  SKY_SET_KEY_TONE(FALSE);
       }
        break;

    case SE_SOFTKEY_DELETE:
          EQC_StopAnimation(EQS_GetNthIOC(0));
		  SKY_SET_KEY_TONE(FALSE);
          PLAY_voice_stop();
          KeyMode = VMP_KEY_DELETE;
		break;

    case SE_KEY_UP:
         //dwUpDownKeyCount++;
         //if(dwUpDownKeyCount >= ROLL_PLAY_CNT)
         //{
         //   /* Up, Down Key가 100회이상 눌려졌을 때는 SKY_KickDog을 사용해 */
         //   /* Bad Dog에 걸리지 않도록 해 준다.                            */
         //   /* Kick Dog을 해주지 않으면 dog에 걸려서 Phone이 죽는다.       */
         //   dwUpDownKeyCount = dwUpDownKeyCount % ROLL_PLAY_CNT;
         //   SKY_KickDog();
         //}

         if(fCompleteUp == TRUE)
         {
            fCompleteUp = FALSE;
            if(fUseTimer == TRUE)
            {
                PLAY_voice_stop_with_no_event();
                bIndex = sGetPlayIndex();
                VoiceInfo = GetVoiceInfo();
                flfclose(GetCurrentFn());

                if(bIndex == 0)
                {
                    bIndex = (VoiceInfo->bTotalRecNumber - 1);
                }
                else
                {
                    bIndex--;
                }

                sSetPlayIndex(bIndex);
                CurrentDataHeader = GetRECDataHeader(); 
                Read_Current_Fn(sGetPlayIndex());
                flfopen(GetCurrentFn() , RD_MODE);
                flfread(GetCurrentFn() , (BYTE *)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);
                KeyMode = VMP_KEY_NONE; 
            
                bRecordingType = GetRecordType(sGetPlayIndex());
                SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
                /* 키톤이 울린 후 VOCODER_DELAY msec의 delay를 준다. */
                EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
				SKY_SET_KEY_TONE(FALSE);
                fUseTimer = FALSE;
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
            }
            fCompleteUp = TRUE;
         }
          break;


    case SE_KEY_RECORD :                     /* Increment Playing Index */    
    case SE_KEY_DOWN :
        //dwUpDownKeyCount++;
        //if(dwUpDownKeyCount >= ROLL_PLAY_CNT)
        //{
        //    /* Up, Down Key가 100회이상 눌려졌을 때는 SKY_KickDog을 사용해 */
        //    /* Bad Dog에 걸리지 않도록 해 준다.                            */
        //    /* Kick Dog을 해주지 않으면 dog에 걸려서 Phone이 죽는다.       */            
        //    dwUpDownKeyCount = dwUpDownKeyCount % ROLL_PLAY_CNT;
        //    SKY_KickDog();
        //}

        if(fCompleteDown == TRUE)
        {
            fCompleteDown = FALSE;

           if(fUseTimer == TRUE)
		   {            
                KeyMode = VMP_KEY_NONE;
                PLAY_voice_stop_with_no_event();
                bIndex = sGetPlayIndex();
                VoiceInfo = GetVoiceInfo();
                flfclose(GetCurrentFn());

                bIndex++;
                if(bIndex == VoiceInfo->bTotalRecNumber)
                {
                  bIndex = 0;
                }
                sSetPlayIndex(bIndex);
                //      RecDataInit(bIndex);
                CurrentDataHeader = GetRECDataHeader(); 
                Read_Current_Fn(sGetPlayIndex());
                flfopen(GetCurrentFn() , RD_MODE);
                flfread(GetCurrentFn() , (BYTE *)&(CurrentDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);
  
                KeyMode = VMP_KEY_NONE;


                bRecordingType = GetRecordType(sGetPlayIndex());
                SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
                /* 키톤이 울린 후 VOCODER_DELAY msec의 delay를 준다. */
                EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
				SKY_SET_KEY_TONE(FALSE);
		        fUseTimer = FALSE;
                EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
		   }
            fCompleteDown = TRUE;
        }
        break;


    case SE_IO_TO_IDLE:
	case SE_SYS_TO_IDLE: /* Ignore this event */
		break;

    case SE_KEY_END:		
    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
    case SE_KEY_PHONE_CLOSE:
        PreviousEvent = pEvent->EventID;
        SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
	    tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
		nVocoderReportControl = IDLE_VOCODER_DOWN;
        flfclose(GetCurrentFn());
		EQS_StartTimer(EQS_TIMER_3, VOCODER_DELAY, FALSE);
        SKY_SET_KEY_TONE(FALSE);
        break;

    case SE_CALL_INCOMING:
        SKY_SET_ALERT(TRUE);
        SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);
	    tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL);
		nVocoderReportControl = IDLE_VOCODER_DOWN;
        flfclose(GetCurrentFn());
        if(fIsDelete == FALSE)
        {
            
            /* 삭제 메시지 박스가 떠 있는 상태에서 전화가 왔을 때는 */
            /* 사용자 설정에 따라 벨이 울려야 한다.                 */
            gnVocoderMode = VR_RING;
        }
        EQS_START_SIBLING_EVENT(SKA_CallIncomingInPlayingVMemo, 
                                SE_CALL_INCOMING, EQS_NA, EQS_NA, TRUE);
        break;

    case SE_IO_MESGBOX_YES:
        EQS_START_SIBLING(sSKA_DeleteOneItem, FALSE);
        break;

    case SE_IO_MESGBOX_NO:
        SKY_SET_KEY_TONE(TRUE);
        EQS_END();
		break;

    case SE_IO_MESGBOX_CLEAR:
    case SE_IO_MESGBOX_END:
		SKY_SET_KEY_TONE(TRUE);
		EQS_END();
        break;

    case SE_IO_ANIM_TURN_AROUND:        
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

#define NUM_RECPLAY_LIST_CHAR  33
#define NUM_RECPLAY_LIST_ITEM  20
STATIC BYTE*        pszRECPLAYList[NUM_RECPLAY_LIST_ITEM+1];


void SKY_API sInitViewRecordListDisp(BYTE* pszViewList)
{
  HCONTROL        hControl;
  STATIC BYTE     szList[NUM_RECPLAY_LIST_ITEM+1][NUM_RECPLAY_LIST_CHAR];
  BYTE            szTmpBuf[NUM_RECPLAY_LIST_CHAR];
  UINT8           nLoop, nSize;
  VMEMOINFO_T*    VoiceInfo;
  RECDATA_T*      RECDataHeader;


  VoiceInfo = GetVoiceInfo();
  nSize = VoiceInfo->bTotalRecNumber;

  for( nLoop = 0; nLoop < NUM_RECPLAY_LIST_ITEM; nLoop++)
	  szList[nLoop][0] = 0;

  for(nLoop = 0; nLoop <NUM_RECPLAY_LIST_ITEM; nLoop++)
  {
     if(nLoop < nSize)
     {
        RECDataHeader = GetRECDataHeader();
        Read_Current_Fn(nLoop);
        flfopen(GetCurrentFn(), RD_MODE);
        flfread(GetCurrentFn(), (BYTE*)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);  
        flfclose(GetCurrentFn());
        sGetDateTimeStringNoDayofWeek(szTmpBuf, RECDataHeader->dwTime);

        switch(GetRecordType(nLoop))
        {
        case VTYPE_NONE:
        case VTYPE_MEMO:
            sprintf((CHAR*)szList[nLoop], SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), 
				                          SF_VOICE_REC1_2, szTmpBuf);
	        break;

        case VTYPE_NEWANS:
	        sprintf((CHAR*)szList[nLoop], SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), 
				                          SF_NEW_AUTORPLY1_2, szTmpBuf);
	        break;

        case VTYPE_OLDANS:
	        sprintf((CHAR*)szList[nLoop], SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), 
				                          SF_AUTORPLY1_2, szTmpBuf);
	        break;

        default:
	        sprintf((CHAR*)szList[nLoop], SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), 
				                          SF_VOICE_REC1_2, szTmpBuf);
	        break;
        }
     }
     pszRECPLAYList[nLoop] = szList[nLoop];
  }

  hControl = EQC_ScrollList(0, 0, NUM_RECPLAY_LIST_CHAR,
                            EQC_GetScrollDisplayLine(),
	                        pszRECPLAYList, NUM_RECPLAY_LIST_ITEM, 
							nSize, 0);
  
  EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl)     | CS_SCROLL_LIST_BORDER | CS_SCROLL_LIST_MAXIMIZE |
                           CS_SCROLL_LIST_ENUMBERATED) |
                           CS_SCROLL_LIST_NO_CML);
  EQC_SetScrollTitle(hControl, (BYTE*)SKY_GET_TOKEN(TKN_VMEMO_RECORD_LIST));
  EQC_ShowScrollTitle(hControl, TRUE);
  EQS_SetIOCActive(hControl);
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
void SKY_API sUpdateViewList(BYTE* pszBuffer, BYTE bIndex)
{
    RECDATA_T*      RECDataHeader;
    BYTE            szTmpBuf[NUM_RECPLAY_LIST_CHAR];


    RECDataHeader = GetRECDataHeader();
    Read_Current_Fn(bIndex);

    flfclose(GetCurrentFn());
    flfopen(GetCurrentFn(), RD_MODE);
    flfread(GetCurrentFn(), (BYTE*)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);  
    flfclose(GetCurrentFn());
    sGetDateTimeStringNoDayofWeek(szTmpBuf, RECDataHeader->dwTime);
        
    switch(GetRecordType(bIndex))
    {
    case VTYPE_NONE:
    case VTYPE_MEMO:
        sprintf((CHAR*)pszBuffer, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), SF_VOICE_REC1_2, szTmpBuf);
	    break;

    case VTYPE_NEWANS:
	    sprintf((CHAR*)pszBuffer, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), SF_NEW_AUTORPLY1_2, szTmpBuf);
	    break;

    case VTYPE_OLDANS:
	    sprintf((CHAR*)pszBuffer, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), SF_AUTORPLY1_2, szTmpBuf);
	    break;

    default:
	    sprintf((CHAR*)pszBuffer, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_TIME), SF_VOICE_REC1_2, szTmpBuf);
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
LOCAL void SKY_API sSKA_ViewRecordList(SKY_EVENT_T* pEvent)
{
   BYTE     pszViewList[VM_MAX_TMPBUF];
   STATIC   HCONTROL hControl, hCtrlDeleteOne;
   UINT8    nCurrIndex, nSize, iLoop;
    
	switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
		EQS_GetDC(TRUE);
		EQS_SetWindow(SKY_DEF_WIN_X,
		SKY_DEF_WIN_Y, 
		SKY_DEF_WIN_DX, 
		SKY_DEF_WIN_DY, 
		CL_DELETE_PLAY_NONE,             //CL_NONE_PLAY_DELETE,  
		NULL,        //SKY_GET_TOKEN(TKN_VMEMO_PLAY), 
		WA_SHOW_CML);
		sInitViewRecordListDisp(pszViewList);
        gnVocoderMode = VR_NONE;
        hControl = EQS_GetNthIOC(0);
        break;

    case SE_APP_END :     /* The very last event */
        SKY_SET_KEY_TONE(TRUE);
        EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        gnVocoderMode = VR_NONE;
        if(EQS_GET_CHILD() == sSKA_DeleteOneItem)
        { 
          SKY_SET_KEY_TONE(TRUE);
          nCurrIndex  = EQC_GetScrollLogicalIndex(hControl);
          EQC_DeleteScrollData(hControl, nCurrIndex);/* 화면에 처음 뿌려질 데이터의 인덱스 */
          nSize = EQC_GetScrollMaxDataLine(hControl);

          if( nSize == 0) EQS_END();
          else if( nSize > NUM_RECPLAY_LIST_ITEM )  nSize = NUM_RECPLAY_LIST_ITEM;
          EQS_SetIOCActive(hControl);  /* Scroll list를 Active 시킴 */
        }
        else if(EQS_GET_CHILD() == sSKA_GoPlaying)
        {
          nSize = EQC_GetScrollMaxDataLine(hControl);
          for(iLoop = 0; iLoop < nSize; iLoop++)
          {
            sUpdateViewList(EQC_GetScrollData(hControl, iLoop), iLoop);
          }
          nCurrIndex = sGetPlayIndex();
		  EQC_SetScrollLogicalIndex(hControl, nCurrIndex);
          EQS_SetIOCActive(hControl);  /* Scroll list를 Active 시킴 */
        }
        break;
    
    case SE_APP_DRAW :    /* draw */
        break;    

    case SE_TM_TIMER1:
        nCurrIndex = EQC_GetScrollLogicalIndex(hControl);
        sSetPlayIndex(nCurrIndex);
		SKY_SET_KEY_TONE(TRUE);
        EQS_START_CHILD_EVENT(sSKA_GoPlaying, SE_KEY_PLAY_LONG, EQS_NA, EQS_NA);
        break;

    case SE_SOFTKEY_DELETE:
        hCtrlDeleteOne = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_IDLE_QUESTION_ERASE), MESSAGE_DEFAULT_TIME, 
                                              MBT_YESNO, MBT_CONFIRM);
        break;

    case SE_IO_TO_IDLE:
	case SE_SYS_TO_IDLE:
		break;

    case SE_SOFTKEY_PLAY:
    case SE_IO_SCROLL_LIST_SELECT:
        EQS_SetIOCActive(EQS_GetNthIOC(0));
        EQS_StartTimer(EQS_TIMER_1, VOCODER_DELAY, FALSE);
		SKY_SET_KEY_TONE(FALSE);
        break;

    case SE_IO_MESGBOX_YES:
        if(pEvent->wParam == hCtrlDeleteOne)
        {          
          SKY_SET_KEY_TONE(FALSE);  
          nCurrIndex = EQC_GetScrollLogicalIndex(hControl);
          sSetPlayIndex(nCurrIndex);
          Read_Current_Fn(sGetPlayIndex());
          EQS_START_CHILD(sSKA_DeleteOneItem);
        }
        break;

    case SE_IO_MESGBOX_NO:
        break;

    case SE_IO_MESGBOX_END:
        break;

    case SE_IO_SCROLL_LIST_TOP:
    case SE_IO_SCROLL_LIST_BOTTOM:
        break;

    case SE_IO_SCROLL_LIST_CANCEL:
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

LOCAL void SKY_API sSKA_DeleteOneItem(SKY_EVENT_T* pEvent)
{
    STATIC   HCONTROL hControl;
    STATIC   HCONTROL hCtrlEnd;
    CHAR     pszVMemo[VM_MAX_TMPBUF]; 
    STATIC   SKY_EVENT_E     PreviousEvent;

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
       SKY_SET_KEY_TONE(FALSE);
       PreviousEvent = SE_KEY_NONE;
       
       sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_DELETINGONE2), sGetPlayIndex()+1);          
       hControl = EQC_MessageBox((BYTE*)pszVMemo, 0, MBT_NONE, MBT_HELP);
        break;

    case SE_APP_END :     /* The very last event */
	  SKY_SET_KEY_TONE(TRUE);
      EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        break;

    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:
        break;

    case SE_KEY_END:      /* ignore this event!! */
    case SE_KEY_PHONE_CLOSE:
        PreviousEvent = pEvent->EventID;
        break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hControl)
        {
           VoiceEraseTransac(GetCurrentFn());

           gnVocoderMode = VR_NONE;
           Release_play_rec();
          
           sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_DELETEONE1_SUCCESS), 
                   sGetPlayIndex()+1);

           hCtrlEnd = EQC_MessageBox((BYTE*)pszVMemo, 1, MBT_NONE, 
                                     MBT_SUCCESS);
           
           if((gVMemoInfo.bTotalRecNumber <= sGetPlayIndex()) && (sGetPlayIndex() != 0))
           {
                sSetPlayIndex(gVMemoInfo.bTotalRecNumber - 1);
           }
        }
        else if(pEvent->wParam == hCtrlEnd)
        {
           if((PreviousEvent == SE_KEY_END) ||
              (PreviousEvent == SE_KEY_PHONE_CLOSE))
           {
               EQS_END_TO(EQS_POP_TO_ROOT);
           }
           else
           {
              EQS_END();
           }
        }
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
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_PlayVMemo()
**
** Description: 
**     Play a saved voice memo.
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
LOCAL void SKY_API sSKA_PlayVMemo(SKY_EVENT_T* pEvent)
{
    STATIC HCONTROL      hCtrlDelete;
    STATIC HCONTROL      hCtrlMessageBox;
    VMEMOINFO_T*         VoiceInfo;
    BYTE                 pszVMemo[VM_MAX_TMPBUF];
    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
       EQS_GetDC(TRUE);
       sSetPlayIndex(0);
       EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_NONE_NONE, 
            SKY_GET_TOKEN(TKN_VMEMO_PLAY), 
            WA_SHOW_HEADER | WA_SHOW_CML);
//       EQS_FitCanvasSize();
       VoiceInfo = GetVoiceInfo(); 
       sInitPlayVoiceMemoDisp(pszVMemo);
        break;

    case SE_APP_END :     /* The very last event */
       EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;

    case SE_APP_RESUME :  /* end child */
#if (MODEL_ID == MODEL_B0) 
		if(EQS_GET_CHILD() == sSKA_DeleteOneItem)
        {
          VoiceInfo = GetVoiceInfo();
          if(VoiceInfo->bTotalRecNumber > 0)
          {
             sUpdatePlayVoiceMemoDisp(pszVMemo);
          }
          else  /* 녹음된 내용이 없는 경우 */
          {
             EQS_SEND_EVENT(SE_KEY_PLAY_LONG, 0, 0);
          }
        }
#else
       EQS_END();
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
       /* <-- */
        break;

//    case SE_TM_TIMER1:
//        EQS_StopTimer(EQS_TIMER_1);
//        EQS_SEND_EVENT(SE_CALL_INCOMING,0 ,0);
//        break;

    case SE_APP_DRAW :    /* draw */
        break;

//    case SE_KEY_RECORD :
//       EQS_START_SIBLING_CURRENT_EVENT(SKA_MainVMemo, FALSE);
//        break;
    
    case SE_KEY_PLAY_LONG:
       VoiceInfo     = GetVoiceInfo();
       if(VoiceInfo->bTotalRecNumber == 0) /*녹음된 내용이 하나도 없을 경우 */
       {
          hCtrlMessageBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_NONE), 
                                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_FAIL); 
          //EQC_SET_STYLE(hCtrlMessageBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_DEFAULT);
       }
       else
       {
          EQS_START_CHILD_CURRENT_EVENT(sSKA_GoPlaying);
       }
        break;

    case SE_SOFTKEY_MENU:
        EQC_PopUpMenu((EQS_TOKEN_EX*)paVPlay, 2, 0);
       break;


    case SE_IO_POPUP_SELECT:
       switch(pEvent->dwParam)
       {
         case PUP_START_PLAY:
           EQS_SEND_EVENT(SE_KEY_PLAY_LONG, 0, 0);
            break;

         case PUP_DELETE:
          PLAY_voice_stop();
          gnVocoderMode = VR_NONE;
          flfclose(GetCurrentFn());

          sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_DELETEONE2), sGetPlayIndex()+1);          

          hCtrlDelete = EQC_MessageBox((BYTE*)pszVMemo, MESSAGE_DEFAULT_TIME, MBT_YESNO, 
                                        MBT_CONFIRM);
            break;
       }
       break;

    case SE_IO_MESGBOX_OK:
       if(hCtrlMessageBox == pEvent->wParam)
          EQS_END();
       break;

    case SE_IO_MESGBOX_YES:
      if(hCtrlDelete == pEvent->wParam)
          EQS_START_CHILD(sSKA_DeleteOneItem);
        break;

    case SE_IO_MESGBOX_NO:
          EQS_END();
        break;    

    case SE_IO_TO_IDLE:
	case SE_SYS_TO_IDLE:
		break;
    
    case SE_IO_MESGBOX_END:
       if(hCtrlMessageBox == pEvent->wParam)
           EQS_END();
        break;


//   case SE_CALL_INCOMING:
//      EQS_END_CURRENT_EVENT();
//      break;
    /* EQC_KEY */
    /* EQC_SOFTKEY */
    /* EQC_SMS */
    /* EQC_CM */
    /* EQC_PH */
    /* EQC_TM */
    /* EQC_IN */
    /* EQC_X */
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
void SKY_API SKA_MainVMemo(SKY_EVENT_T* pEvent)
{
    STATIC VMEMOINFO_T*  VoiceInfo;
    HCONTROL             hCtrlMessageBox; 

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        break;

    case SE_APP_END : /* The very last event */
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
         EQS_END();
        break;
    
    case SE_APP_DRAW :  /* draw */
        break;

    case SE_KEY_RECORD_LONG:
        EQS_START_CHILD_CURRENT_EVENT(sSKA_RecordVMemo);
        break;
    
    case SE_KEY_RECORD:
        EQS_START_CHILD(sSKA_RecordVMemo);
        break;
    

    case SE_KEY_PLAY:
        VoiceInfo = GetVoiceInfo();
        if (VoiceInfo->bTotalRecNumber > 0)
        {
           EQS_START_CHILD(sSKA_PlayVMemo);
        }
        else
        {
          hCtrlMessageBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_RECORD_NONE), MESSAGE_DEFAULT_TIME , MBT_NONE,
                                           MBT_FAIL); 
          //EQC_SET_STYLE(hCtrlMessageBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_DEFAULT);
        }
        break;

    case SE_IO_MESGBOX_END:
        EQS_END();
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
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}


