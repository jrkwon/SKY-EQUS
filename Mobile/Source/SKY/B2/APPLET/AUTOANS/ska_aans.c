/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     ska_aans.c
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
** 2000-12-27   hjcho      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h"
#include "voc.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define    MAX_AUTOANSWER_RECORDING_TIME    30

#define    AANS_BUF                         20
#define    AANS_SECS_PER_ONE                30
#define    AANS_NUM_CHAR                    16
#define    AANS_NUM_SET_ITEM                4

#ifndef TREX_VOICE_APP
#define    gnVocoderMode  0
#endif
/*
** �ڵ�������� 
*/
#define AANS_RECORD_ANIMATION_XPOS           4//3//2
#define AANS_RECORD_ANIMATION_YPOS           (20 - SKY_ANNUNCIATORS_HEIGHT)//3//0

#define AANS_RECORD_BACKGROUND_XPOS          4//3
#define AANS_RECORD_BACKGROUND_YPOS          (82 - SKY_ANNUNCIATORS_HEIGHT)//63//43//41//45

#define AANS_RECORD_NUMBER_XPOS              (VM_RECORD_REC_NUM_IMAGE_XPOS + 32)//32
#define AANS_RECORD_NUMBER_YPOS              (VM_RECORD_REC_NUM_IMAGE_YPOS - 2)//(85 - SKY_ANNUNCIATORS_HEIGHT)//66//47//45

#define AANS_RECORD_PBAR_XPOS                11//8
#define AANS_RECORD_PBAR_YPOS                (110 - SKY_ANNUNCIATORS_HEIGHT)//89//66


#define AANS_RECORD_REMAIN_TIME_TITLE_XPOS   17

#define AANS_RECORD_REMAINTIME_XPOS          AANS_RECORD_REMAIN_TIME_TITLE_XPOS + EQS_TextWidth(SKY_GET_TOKEN(TKN_VMEMO_TIME))   
#define AANS_RECORD_REMAINTIME_YPOS          (120 - SKY_ANNUNCIATORS_HEIGHT)//101//75

#define AANS_RECORD_RECORDINGTIME_XPOS       74
#define AANS_RECORD_RECORDINGTIME_YPOS       (57 - SKY_ANNUNCIATORS_HEIGHT)

#define AANS_RECORD_REC_NUM_IMAGE_XPOS       VM_RECORD_REC_NUM_IMAGE_XPOS
#define AANS_RECORD_REC_NUM_IMAGE_YPOS       VM_RECORD_REC_NUM_IMAGE_YPOS



/*
** ����� ���� ��������
*/
#define USER_RECORD_ANIMATION_XPOS          VM_RECORD_ANIMATION_XPOS
#define USER_RECORD_ANIMATION_YPOS          VM_RECORD_ANIMATION_YPOS

#define USER_RECORD_BACKGROUND_XPOS         VM_RECORD_BACKGROUND_XPOS
#define USER_RECORD_BACKGROUND_YPOS         VM_RECORD_BACKGROUND_YPOS//41

#define USER_RECORD_PBAR_XPOS               VM_RECORD_PBAR_XPOS
#define USER_RECORD_PBAR_YPOS               VM_RECORD_PBAR_YPOS

#define USER_RECORD_REMAINTIME_XPOS         VM_RECORD_REMAINTIME_XPOS
#define USER_RECORD_REMAINTIME_YPOS         VM_RECORD_REMAINTIME_YPOS

#define USER_RECORD_REMAIN_TIME_TITLE_XPOS  VM_RECORD_REMAIN_TIME_TITLE_XPOS

#define USER_RECORD_RECORDINGTIME_XPOS      VM_RECORD_RECORDINGTIME_XPOS
#define USER_RECORD_RECORDINGTIME_YPOS      VM_RECORD_RECORDINGTIME_YPOS



/*
**  ����� ���� ��� ����
*/
#define USER_PLAY_RECORDING_TIME_YPOS		(VM_RECORD_REMAINTIME_YPOS - 5)//68

#define USER_PLAY_REC_TYPE_XPOS				68
#define USER_PLAY_REC_TYPE_YPOS				(28 - SKY_ANNUNCIATORS_HEIGHT)//9


  


/*
�ڵ����伳��
��ũ�Ѹ���Ʈ - ���︲�ð�����
��ũ�Ѹ���Ʈ - �ȳ���������
*/
enum tagAUTOANSWERSET
{
  ASS_RING_LIST_TITLE = 0,
  ASS_RING_LIST,       //������ ����
  ASS_MENT_LIST_TITLE,
  ASS_MENT_LIST,       //��Ʈ ����
  ASS_MAX
};


/*
  �ڵ����� ���� �� ȭ��
*/
enum tagRECORD_PROGRESSVMEMO_E
{
 AANS_ANIMATION = 0,    
 AANS_RECORDING_TIME,  
 //AANS_BACKGROUND,       
 AANS_NUMAUTOANSWER,  
 AANS_PROGRESS,  
 AANS_REMAIN_TIME_TITLE,
 AANS_REMAIN_TIME       
};


/*
�������������
��������
����Ȯ��
*/
enum tagRECUSERMENT_E
{
  RU_RECUSERMENT = 0,
  RU_LISTENUSERMENT,
  RU_MAX
};


enum tagRECORD_USERMENTDISP_E
{
 RUD_ANIMATION    = 0,    
 RUD_RECORDING_TIME,  
//RUD_BACKGROUND,       
//RUD_RECORDING_INDEX,  
 RUD_PROGRESS,  
 RUD_REMAIN_TIME_TITLE,
 RUD_REMAIN_TIME       
};


enum tagPLAY_USERMENTDISP_E
{
  PUD_ANIMATION = 0,   
  PUD_RECTYPE, 
//PUD_BACKGROUND,
//PUD_INDEX,        
  PUD_RECTIME        
};


/*
�������������
*/
enum tagPUPUSERMENT_E
{
 PUP_DELETE_USER_MENT    /* ��������  */
};


SKY_TOKEN paPlayUserMent[1] =   /* Hot Menu!! */
{
  TKN_VMEMO_DELETE_ONE          /* �������� */
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
BOOL  gAutoAnswering = FALSE;
UINT8 nRemainTime;
STATIC BYTE*   pszRingTime[AANS_NUM_SET_ITEM];
STATIC BYTE*   pszMentType[AANS_NUM_SET_ITEM];

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
extern BYTE  getfn(void);
extern BOOL  flfopen(BYTE fn, BYTE op_type);
extern BOOL  flfopen(BYTE fn, BYTE op_type);
extern BOOL  flfclose(BYTE fn);
extern DWORD flfwrite(BYTE fn, BYTE *data, DWORD size);
extern DWORD flfread(BYTE fn, BYTE *data, DWORD size);




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
BOOL GetAutoAnswering()
{
  return gAutoAnswering;
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
void SetAutoAnswering(BOOL fAnswering)
{
  gAutoAnswering = fAnswering;
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
UINT8 sGetRemainTime()
{
  return nRemainTime;
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
UINT8 sUpdateRemainTime()
{
  return nRemainTime--;
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
void SetRemainTime(UINT8 nNewRemainTime)
{
  nRemainTime = nNewRemainTime;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     InitAutoAnsDisp()
**
** Description: 
**     Initialize Autoanswer Display
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
//#define AANS_CALL_CURRENT_TIME_XPOS          23
//#define AANS_CALL_CURRENT_TIME_YPOS           1 
#define sAUTO_ANSWER_MODE_XPOS   64
#define sAUTO_ANSWER_MODE_YPOS   (60 - SKY_ANNUNCIATORS_HEIGHT)

#define sAANS_TIME_FRAME_IMAGE_XPOS        57//61//58
#define sAANS_TIME_FRAME_IMAGE_YPOS        2 //3//17


/* �ڵ����伳�� �� ����ð� ���� */
#define sAANS_TIME_FIRST_XPOS              62//sAANS_TIME_FRAME_IMAGE_XPOS+2
#define sAANS_TIME_FIRST_YPOS              (26 - SKY_ANNUNCIATORS_HEIGHT)//sAANS_TIME_FRAME_IMAGE_YPOS+3

#define sAANS_TIME_SECOND_XPOS             sAANS_TIME_FIRST_XPOS
#define sAANS_TIME_SECOND_YPOS             sAANS_TIME_FIRST_YPOS + EQS_CharHeight('A')

#define sAANS_AM_PM_XPOS                   (sAANS_TIME_FIRST_XPOS + EQS_TextWidth((BYTE*)"12:13") + 3)//98
#define sAANS_AM_PM_YPOS                   (sAANS_TIME_SECOND_YPOS + 3)




void InitAutoAnsDisp(BYTE bMode)
{
    CHAR                szBuf[AANS_BUF];  
    CHAR                szBufMode[AANS_BUF];
    clk_julian_type     Julian;
    VMEMOINFO_T*        VoiceInfo = GetVoiceInfo();
    HCONTROL            hControl;
	ANIMATION_INFO_T*   pANIMATION;
    DWORD               dwCurrentProgress;
    SKY_TOKEN           AutoAns;
    BYTE                bAutoAnsMode_XPOS;
    DWORD               dwCurrentTime;
    BOOL                fAM;
    INT8                nAMPM;
    STATIC BITMAP       Bitmap;



    switch(SKY_GET_AUTO_ANSWER_GREETING())
    {
    case GM_GENERAL:   /* �Ϲ� - �ý��� �⺻ */
        pANIMATION =  &g_AniAutoAnswerGeneral; //g_AniAutorply55x39x256c;
		sprintf(szBufMode, "  %s  ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
        break;

    case GM_DRIVING:   /* ���� �� */
        pANIMATION = &g_AniAutoAnswerDrive;   //g_AniAutorplyDrive55x39x256c;
        sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_DRIVE));
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
        break;

    case GM_MEETING:   /* ȸ�� �� */
        pANIMATION = &g_AniAutoAnswerMeeting; //g_AniAutorplyMeet55x39x256c; 
        sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_MEETING));
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
        SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
        break;


    case GM_USER:      /* ����� ���� ���� */
        pANIMATION = &g_AniAutoAnswerUser;   //g_AniAutorplyUser55x39x256c;    
		sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_USER));
        AutoAns = TKN_SLCD_AUTOANS_USER;
        break;

    default :
        pANIMATION = &g_AniAutoAnswerGeneral; //g_AniAutorply55x39x256c;
		sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
		SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
		break;
    }

	hControl = EQC_Animation( AANS_RECORD_ANIMATION_XPOS,   /* �ڵ����� �ִϸ��̼� */  
                              AANS_RECORD_ANIMATION_YPOS,   //+ AANS_DISPLAY_ANI_OFFSET_YPOS, 
                              pANIMATION,
                              NULL);
	if(SKY_IS_PHONE_OPEN())
	{
	  EQC_StartAnimation(hControl, TRUE);
	}


    /* ����ð� ���� */
    dwCurrentTime = SKY_GetLocalTime();
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &Julian);

        fAM = (BOOL) ((Julian.hour < 12) ? TRUE : FALSE);

        nAMPM = (fAM) ? 0 : 1;

        if(fAM == FALSE && Julian.hour != 12)
        {
            Julian.hour -= 12;
        }

        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                     Julian.month, Julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[Julian.day_of_week]));
        //EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);
        hControl = EQC_StaticText(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);

        
        /* display hour */
        sprintf((char*)szBuf, (char*)"%2d:%02d", Julian.hour, Julian.minute);
        hControl = EQC_StaticText(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);


        if(nAMPM >=0 ) /* AM */        
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            //EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
        }

    }
    else /* Valid�� �ð��� �ƴ� ��� */
    {
        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                          1, 1, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[0]));
        //EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);
        hControl = EQC_StaticText(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);
        fAM = TRUE;
        if(fAM) /* AM */
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            //EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
        }
        /* display hour */
        sprintf((char*)szBuf, (char*)"%02d:%02d", 0, 0);
        EQS_TextOutStr(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, (BYTE *)szBuf); 
        hControl = EQC_StaticText(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);
        //s_wMinute = julian.minute;
        //fChangeMinute = TRUE;
    }
    //

   hControl = EQC_StaticText(sAUTO_ANSWER_MODE_XPOS, 
                             sAUTO_ANSWER_MODE_YPOS,
                             (EQS_TOKEN)TKN_AUTOANS_NULL);   /*���� ��� */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_BORDER));
   EQC_SetStaticText(hControl, szBufMode);
    
   /* ���� */
   EQC_Image(VM_HORIZONTAL_LINE_XPOS,
             VM_HORIZONTAL_LINE_YPOS,
             &g_BmpHorizontalLine);

   /* New �̹��� */
   EQC_Image(VM_RECORD_REC_NUM_IMAGE_XPOS,
             VM_RECORD_REC_NUM_IMAGE_YPOS,
             &g_BmpAutoAnswerPannelNew);

   sprintf(szBuf, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), VoiceInfo->bNewAutoAnsNumber);   
   hControl = EQC_StaticText(AANS_RECORD_NUMBER_XPOS,      /*���ڵ����䰳��*/              
                             AANS_RECORD_NUMBER_YPOS,      //+ AANS_DISPLAY_OFFSET_YPOS,
                            (EQS_TOKEN)TKN_NULL);
   //EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, szBuf);

   //dwCurrentProgress = VoiceInfo->wRemainedTime;        /* Progress Bar */
   // hjcho 01.05.13 �λ縻������ �����޸�����ð� �и�
   dwCurrentProgress  = GetVoiceMemoRemainTime();
   hControl = EQC_ProgressBar(AANS_RECORD_PBAR_XPOS, 
                              AANS_RECORD_PBAR_YPOS,      //+ AANS_DISPLAY_OFFSET_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              VM_MAX_PROGRESS, 
                              dwCurrentProgress,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE);    /* ��Ŀ���� ���� ���ϵ��� �ϴ� ��ũ�� */

   sprintf(szBuf,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(AANS_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             AANS_RECORD_REMAINTIME_YPOS, //+ AANS_DISPLAY_OFFSET_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   //EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, szBuf);


   //clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian); hjcho 01.05.13
   clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian);
   sprintf(szBuf, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(AANS_RECORD_REMAINTIME_XPOS,     /* ���� �ð� */
                             AANS_RECORD_REMAINTIME_YPOS, //+ AANS_DISPLAY_OFFSET_YPOS, 
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NO_CHANGE);
   EQC_SetStaticText(hControl, szBuf);   
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     InitAutoAnsDisp()
**
** Description: 
**     Initialize Autoanswer Display
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
SKY_API void UpdateCurrentDateandTimeInAutoAnswer()
{
    DWORD             dwCurrentTime;
    clk_julian_type   julian;
    BYTE              szBuf[20];
    STATIC BITMAP     Bitmap;
    BOOL              fAM;
    INT8              nAMPM = 0;

    dwCurrentTime = SKY_GetLocalTime();

    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &julian);

        fAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);

        nAMPM = (fAM) ? 0 : 1;

        if(fAM == FALSE && julian.hour != 12)
        {
            julian.hour -= 12;
        }

        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                     julian.month, julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]));
        EQC_SetStaticText(EQS_GetNthIOC(AAD_DATE), szBuf);        

        /* display hour */
        sprintf((char*)szBuf, (char*)"%2d:%02d", julian.hour, julian.minute);
        EQC_SetStaticText(EQS_GetNthIOC(AAD_TIME), szBuf);

        if(nAMPM >=0 ) /* AM */
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            
            //EQS_DeleteControl(EQS_GetNthIOC(AAD_AMPM));
            //EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_SetImage(EQS_GetNthIOC(AAD_AMPM), &Bitmap);
        }
    }
    else /* Valid�� �ð��� �ƴ� ��� */
    {
        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                          1, 1, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[0]));
        EQC_SetStaticText(EQS_GetNthIOC(AAD_DATE), szBuf);
        fAM = TRUE;
        
        /* display hour */
        sprintf((char*)szBuf, (char*)"%02d:%02d", 0, 0);
        EQC_SetStaticText(EQS_GetNthIOC(AAD_TIME), szBuf);        

        if(fAM) /* AM */
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            //EQS_DeleteControl(EQS_GetNthIOC(AAD_AMPM));
            //EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_SetImage(EQS_GetNthIOC(AAD_AMPM), &Bitmap);

        }
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
void InitAutoAnsDispNoSpace(BYTE bMode)
{
    CHAR                szBuf[AANS_BUF];  
    CHAR                szBufMode[AANS_BUF];  
    clk_julian_type     Julian;
    VMEMOINFO_T*        VoiceInfo = GetVoiceInfo();
    HCONTROL            hControl;
	ANIMATION_INFO_T*   pANIMATION;
    DWORD               dwCurrentProgress;
    SKY_TOKEN           AutoAns;
    BYTE                XPOS;
    BOOL                fAM;
    INT8                nAMPM;
    STATIC BITMAP       Bitmap;
    DWORD               dwCurrentTime;



    switch(SKY_GET_AUTO_ANSWER_GREETING())
    {
    case GM_GENERAL:   /* �Ϲ� - �ý��� �⺻ */
        pANIMATION =  &g_AniAutoAnswerGeneral; //g_AniAutorply55x39x256c;
		sprintf(szBufMode, "  %s  ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
        break;

    case GM_DRIVING:   /* ���� �� */
        pANIMATION = &g_AniAutoAnswerDrive;   //g_AniAutorplyDrive55x39x256c;
        sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_DRIVE));
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
        break;

    case GM_MEETING:   /* ȸ�� �� */
        pANIMATION = &g_AniAutoAnswerMeeting; //g_AniAutorplyMeet55x39x256c; 
        sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_MEETING));
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
        SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
        break;


    case GM_USER:      /* ����� ���� ���� */
        pANIMATION = &g_AniAutoAnswerUser;   //g_AniAutorplyUser55x39x256c;    
		sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_USER));
        AutoAns = TKN_SLCD_AUTOANS_USER;
        break;

    default :
        pANIMATION = &g_AniAutoAnswerGeneral; //g_AniAutorply55x39x256c;
		sprintf(szBufMode, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
		SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
		break;
    }

	hControl = EQC_Animation( AANS_RECORD_ANIMATION_XPOS,   /* �ڵ����� �ִϸ��̼� */  
                              AANS_RECORD_ANIMATION_YPOS,   //+ AANS_DISPLAY_ANI_OFFSET_YPOS, 
                              pANIMATION,
                              NULL);
	if(SKY_IS_PHONE_OPEN())
	{
	  EQC_StartAnimation(hControl, TRUE);
	}


    /* ����ð� ���� */
    dwCurrentTime = SKY_GetLocalTime();
    if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
    {
        clk_secs_to_julian(dwCurrentTime, &Julian);

        fAM = (BOOL) ((Julian.hour < 12) ? TRUE : FALSE);

        nAMPM = (fAM) ? 0 : 1;

        if(fAM == FALSE && Julian.hour != 12)
        {
            Julian.hour -= 12;
        }

        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                     Julian.month, Julian.day, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[Julian.day_of_week]));
        //EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);
        hControl = EQC_StaticText(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);

        
        /* display hour */
        sprintf((char*)szBuf, (char*)"%2d:%02d", Julian.hour, Julian.minute);
        hControl = EQC_StaticText(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);


        if(nAMPM >=0 ) /* AM */        
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            //EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
        }

    }
    else /* Valid�� �ð��� �ƴ� ��� */
    {
        sprintf((char*)szBuf, (char*)"%2d.%02d[%2s]", 
                          1, 1, (CHAR *)SKY_GET_TOKEN(g_aDayOfWeek[0]));
        //EQS_TextOutStr(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, (BYTE *)szBuf);
        hControl = EQC_StaticText(sAANS_TIME_FIRST_XPOS, sAANS_TIME_FIRST_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);
        fAM = TRUE;
        if(fAM) /* AM */
        {
            Bitmap = g_BmpBigAMPM;//g_BmpAPM15x9x256C;
            Bitmap.pbData = g_BmpBigAMPM.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nAMPM;
            //EQS_PutBitmap(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
            EQC_Image(sAANS_AM_PM_XPOS, sAANS_AM_PM_YPOS, &Bitmap);
        }
        /* display hour */
        sprintf((char*)szBuf, (char*)"%02d:%02d", 0, 0);
        EQS_TextOutStr(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, (BYTE *)szBuf); 
        hControl = EQC_StaticText(sAANS_TIME_SECOND_XPOS, sAANS_TIME_SECOND_YPOS, TKN_NULL);
        EQC_SetStaticText(hControl, szBuf);
        //s_wMinute = julian.minute;
        //fChangeMinute = TRUE;
    }
    //


   hControl = EQC_StaticText(sAUTO_ANSWER_MODE_XPOS, 
                             sAUTO_ANSWER_MODE_YPOS,
                             (EQS_TOKEN)TKN_AUTOANS_NULL);   /*���� ��� */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_BORDER));
   EQC_SetStaticText(hControl, szBufMode);
    
   /* ���� */
   EQC_Image(VM_HORIZONTAL_LINE_XPOS,
             VM_HORIZONTAL_LINE_YPOS,
             &g_BmpHorizontalLine);

   /* New �̹��� */
   EQC_Image(VM_RECORD_REC_NUM_IMAGE_XPOS,
             VM_RECORD_REC_NUM_IMAGE_YPOS,
             &g_BmpAutoAnswerPannelNew);
 
   
   sprintf(szBuf, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), VoiceInfo->bNewAutoAnsNumber);   
   hControl = EQC_StaticText(AANS_RECORD_NUMBER_XPOS,      /*���ڵ����䰳��*/              
                             AANS_RECORD_NUMBER_YPOS,      //+ AANS_DISPLAY_OFFSET_YPOS,
                            (EQS_TOKEN)TKN_NULL);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, szBuf);

   
   XPOS = SKY_SCREEN_WIDTH/2 - (STRLEN(SKY_GET_TOKEN(TKN_AUTOANSWER_NO_SPACE))*EQS_CharWidth(' '))/2;

   hControl = EQC_StaticText(XPOS,
                             AANS_RECORD_PBAR_YPOS + 6,
                             TKN_BLANK);
   //EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, SKY_GET_TOKEN(TKN_AUTOANSWER_NO_SPACE));
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
void sInitAutoAnsRecordingDisp(UINT8 nRemainedTime, BYTE pszAAns[])
{
    VMEMOINFO_T*       VoiceInfo = GetVoiceInfo();
    ANIMATION_INFO_T*  pANIMATION;
    HCONTROL           hControl;
    clk_julian_type    Julian;
    DWORD              dwCurrentProgress;
    SKY_TOKEN           AutoAns;


    if(SKY_GET_AUTO_ANSWER_GREETING() == GM_DRIVING)         /* ������ */
    {
        pANIMATION = &g_AniAutoAnswerDrive;//&g_AniAutorplyDrive55x39x256c;
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
    }
    else if(SKY_GET_AUTO_ANSWER_GREETING() == GM_MEETING)    /* ȸ���� */
    {
        pANIMATION = &g_AniAutoAnswerMeeting;//&g_AniAutorplyMeet55x39x256c;
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
    }
    else if(SKY_GET_AUTO_ANSWER_GREETING() == GM_GENERAL)    /*  �Ϲ�  */
    {
        pANIMATION = &g_AniAutoAnswerGeneral;//&g_AniAutorply55x39x256c;
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
    }
    else                                                     /* ����� */
    {
        pANIMATION = &g_AniAutoAnswerUser;//&g_AniAutorplyUser55x39x256c;
        AutoAns = TKN_SLCD_AUTOANS_USER;
    }

    EQC_StartAnimation(EQC_Animation( AANS_RECORD_ANIMATION_XPOS,   /* �ڵ����� �ִϸ��̼� */  
                                      AANS_RECORD_ANIMATION_YPOS, 
                                      pANIMATION,
                                      NULL), TRUE);
    //EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                                    SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                                    SKY_GET_TOKEN(AutoAns));
    DisplayCurrentDateTimeInSubLCD(TRUE);
#endif //#ifdef FEATURE_SKY_SUBLCD


   sprintf(pszAAns, SKY_GET_TOKEN(TKN_FMT_AUTOANS_SEC), nRemainedTime);
   
   hControl = EQC_StaticText(AANS_RECORD_RECORDINGTIME_XPOS, 
                             AANS_RECORD_RECORDINGTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);        /* �����ð� */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl, pszAAns);


   //EQC_Image( AANS_RECORD_BACKGROUND_XPOS, 
   //           AANS_RECORD_BACKGROUND_YPOS, 
   //			  &g_BmpRemainTimePannel);
   //           //(BITMAP*)&g_BmpTable02_115x47x256c);     /*  ���� �̹��� */
   
   
   sprintf(pszAAns, SKY_GET_TOKEN(TKN_VMEMO_FMT_RECORD_NUM), (VoiceInfo->bNewAutoAnsNumber+1));   
   hControl = EQC_StaticText(AANS_RECORD_NUMBER_XPOS,      /*���ڵ����䰳��*/              
                             AANS_RECORD_NUMBER_YPOS,
                            (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszAAns);

// dwCurrentProgress = VoiceInfo->wRemainedTime;         /* hjcho 01.05.13 */
   dwCurrentProgress = GetVoiceMemoRemainTime();
   hControl = EQC_ProgressBar(AANS_RECORD_PBAR_XPOS, 
                              AANS_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              VM_MAX_PROGRESS, 
                              dwCurrentProgress,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE);    /* ��Ŀ���� ���� ���ϵ��� �ϴ� ��ũ�� */

   sprintf(pszAAns,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(AANS_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             AANS_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszAAns);


// clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian);
   clk_secs_to_julian(GetVoiceMemoRemainTime(), &Julian);
   sprintf(pszAAns, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(AANS_RECORD_REMAINTIME_XPOS,     /* ���� �ð� */
                             AANS_RECORD_REMAINTIME_YPOS, 
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl, pszAAns);
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
void sUpdateAutoAnswerRecordingStatus 
(
 UINT8 nRemainTime, 
 DWORD dwTotalRecRemainTime
)
{
    CHAR    szBuf[AANS_BUF];
    SKY_TOKEN           AutoAns;

    /* �ڵ����� ���� �ð� */
    sprintf(szBuf, SKY_GET_TOKEN(TKN_FMT_AUTOANS_SEC), nRemainTime); 
    EQC_SetStaticText(EQS_GetNthIOC(AANS_RECORDING_TIME), szBuf);
#ifdef FEATURE_SKY_SUBLCD
    /* �ڵ����� ���� �ð� */
    SKY_SubLCDTextOutStr(LN_LOWER, szBuf); /* 00�� */

    /* �ڵ����� ���� */
    if(SKY_GET_AUTO_ANSWER_GREETING() == GM_DRIVING)         /* ������ */
    {
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
    }
    else if(SKY_GET_AUTO_ANSWER_GREETING() == GM_MEETING)    /* ȸ���� */
    {
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
    }
    else if(SKY_GET_AUTO_ANSWER_GREETING() == GM_GENERAL)    /*  �Ϲ�  */
    {
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
    }
    else                                                     /* ����� */
    {
        AutoAns = TKN_SLCD_AUTOANS_USER;
    }
    /* �ڵ����� */
    SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                                    SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                                    SKY_GET_TOKEN(AutoAns));
#endif //#ifdef FEATURE_SKY_SUBLCD

    /* Progress Bar */
    EQC_SetProgressBar(EQS_GetNthIOC(AANS_PROGRESS), dwTotalRecRemainTime);

    /* ���������� �ð� */
    UpdateTime(dwTotalRecRemainTime, szBuf);
    EQC_SetStaticText(EQS_GetNthIOC(AANS_REMAIN_TIME),szBuf);

    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
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
void sExitAutoAnswerState()
{
    SKY_SET_MUTE(FALSE);
    SKY_SetSoundDevice( SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE );
    gnVocoderMode = VR_NONE;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     �����ð��� 30�� ��� �� �� call�� ����� ��쿡�� sound path�� ������ 
**     ���� �ʴ´�. ������ �� ��� ���濡�� �� ��Ҹ��� 1������ �鸰��.
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
void sExitAutoAnswerStateWithNoSoundPathRestore()
{
    SKY_SET_MUTE(TRUE);
    SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_MUTED, TRUE);
    gnVocoderMode = VR_NONE;
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainAutoAnswer()
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
STATIC BYTE   bWorkingFn;

enum tagAUTOANSWER_END_MODE_E   
{  
   AEM_END_BY_CALLING_PARTY = 0, /*��ȭ�� �� ����� ������ ���*/
   AEM_END_BY_TIMEOUT,           /*�����ð��� �� �Ǿ ���� ���*/
   AEM_END_BY_CALLED_PARTY       /*�������� �ڵ������� �����ϰ� ��ȭ������ �� ���*/
};

typedef enum tagAUTOANSWER_END_MODE_E AUTOANSWER_END_MODE_E;


void SKY_API sSKA_StartRecording(SKY_EVENT_T* pEvent)
{
   STATIC DWORD                   dwTotalRecRemainTime;
   STATIC AUTOANSWER_END_MODE_E   EndMode;
   VMEMOINFO_T*                   VoiceInfo;  
   STATIC HCONTROL                hCtrl, hCtrlSuccess;
   STATIC BOOL                    fSavingAutoAnswer;

    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :     /* The very first event */
      EQS_GetDC(TRUE);
      VoiceInfo = GetVoiceInfo();
      dwTotalRecRemainTime = GetVoiceMemoRemainTime();
      sUpdateAutoAnswerRecordingStatus(sGetRemainTime(), dwTotalRecRemainTime);  
      EndMode = AEM_END_BY_CALLING_PARTY;
      fSavingAutoAnswer = FALSE;
        break;

    case SE_APP_END :       /* The very last event */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif //FEATURE_SKY_SUBLCD
        if(EndMode == AEM_END_BY_TIMEOUT)  /* �����ð� 30�ʸ� �� ����ؼ� ���� ��� */
        {
          sExitAutoAnswerStateWithNoSoundPathRestore();
        }
        else
        {
          sExitAutoAnswerState();
        }
        EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND :   /* start child */
        if(EndMode == AEM_END_BY_TIMEOUT)  /* �����ð� 30�ʸ� �� ����ؼ� ���� ��� */
        {
          sExitAutoAnswerStateWithNoSoundPathRestore();
        }
        else
        {
          sExitAutoAnswerState();
        }
        break;
    
    case SE_APP_RESUME :    /* end child */
        break;
    
    case SE_APP_DRAW :      /* draw */
        /* NEW �̹��� */
        EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
                      VM_RECORD_REC_NUM_IMAGE_YPOS,
                      &g_BmpAutoAnswerPannelNew);
        /* ���� */
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                      VM_HORIZONTAL_LINE_YPOS,
                      &g_BmpHorizontalLine);
        break;

    case SE_SYS_TO_IDLE:    /* ignore */
        break;

    case SE_AANS_PLAY_END:    /* PLAY_voice_start_traffic�� ���� �̺�Ʈ�� ���ƿ´�.*/
        if ((bWorkingFn = VoiceRecordTransacStart(VTYPE_NEWANS)) < NV_MAX_FL_FILE_NUM) 
		{
		  //youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
		  //Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
		  //���� Ű���Լ��� ����Ѵ�. ����� ��ȭ�������� ���ٸ� �ʿ����.
          //sSka_StartRecording
		  REC_voice_start_traffic(bWorkingFn);		 
          EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
		}
    	else
		{
		  sExitAutoAnswerState();
          EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;
 
    case SE_VM_REC_END:       /* SE_VM_REC_EDN from REC_voice_stop_traffic() */
        hCtrl = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVING),
                                     0, MBT_NONE, MBT_SUCCESS);
		break;

    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrl)
        {
           VoiceRecordTransacEnd(bWorkingFn, VTYPE_NEWANS);
           //sExitAutoAnswerState();
           hCtrlSuccess = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVED), 1,
                                               MBT_NONE, MBT_SUCCESS);
        }
        else if(pEvent->wParam == hCtrlSuccess)
        {               
            if(EndMode == AEM_END_BY_CALLING_PARTY) /*������ ���� ������ ���*/
            {
              sExitAutoAnswerState();
              EQS_START_SIBLING_EVENT(SKA_InProg, SE_CALL_ENDED, EQS_NA, EQS_NA, FALSE);        
            }
            else if(EndMode == AEM_END_BY_TIMEOUT)  /* �����ð� 30�ʸ� �� ����ؼ� ���� ��� */
            {
              sExitAutoAnswerStateWithNoSoundPathRestore();
              EQS_START_SIBLING_EVENT(SKA_InProg, SE_KEY_END, EQS_NA, EQS_NA, FALSE);        
            }
            else if(EndMode == AEM_END_BY_CALLED_PARTY) /* ����ڰ� ��ȭ�� ���� ��� */
            {          
              sExitAutoAnswerState();
              EQS_START_SIBLING(SKA_InProg, FALSE);
            }
        }
		break;


    case SE_CALL_ENDED    :   /* Call End */
        EQS_StopTimer(EQS_TIMER_1);
        /* 30�� ������ ������ Recording�� �����ϴ� ���߿� ������ call�� ���� ��� */
        /* �ٽ� REC_voice_stop_traffic�� ���� ��Ű�� ��ȭ �� ���� �Ѿ�� Call�� */
        /* ������ �ʴ� ������ �߻��Ѵ�.                                             */
        if(EndMode != AEM_END_BY_TIMEOUT)
        {    		 
             REC_voice_stop_traffic();
        }
        else
        {
             EndMode = AEM_END_BY_CALLING_PARTY;
        }
        break;


    case SE_TM_TIMER1 :       /* UI_TIMER_F:  */
        sUpdateRemainTime();
        if(sGetRemainTime() > 0)
        {           
		   Data_Record_time++;
           dwTotalRecRemainTime--;
           sUpdateAutoAnswerRecordingStatus(sGetRemainTime(), dwTotalRecRemainTime);
        }
        else
        {           
           EQS_StopTimer(EQS_TIMER_1);
           sUpdateAutoAnswerRecordingStatus(sGetRemainTime(), dwTotalRecRemainTime);
           EndMode = AEM_END_BY_TIMEOUT;
		   REC_voice_stop_traffic();
        }
        break;

    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_DOUBLE_CLICK:
    case SE_KEY_AUTO_ANSWER:
    case SE_KEY_RECORD:
    case SE_KEY_RECORD_LONG:
    case SE_KEY_PLAY:
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
    case SE_KEY_CENTER:
    case SE_KEY_STAR :
    case SE_KEY_POUND :
    case SE_KEY_UP :
    case SE_KEY_DOWN :
    case SE_KEY_LEFT:
    case SE_KEY_RIGHT:
    case SE_SOFTKEY_LEFT :   /* PF3 : */
    case SE_SOFTKEY_RIGHT :  /* PF2 : */
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG : /* UI_CLRALL_F: */
    case SE_KEY_F1:
    case SE_KEY_F2:
        break;


    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND:
       EQS_StopTimer(EQS_TIMER_1);
       ClearMissedCallInfo();
	   EndMode = AEM_END_BY_CALLED_PARTY;
       REC_voice_stop_traffic();
        break;	  


    case SE_KEY_END:
       EQS_StopTimer(EQS_TIMER_1);
       ClearMissedCallInfo();
	   EndMode = AEM_END_BY_TIMEOUT;
       REC_voice_stop_traffic();
        break;

    case SE_IO_TO_IDLE:  /* Ignore this event */
        break;

    case SE_KEY_MANNERS:
        break;

    case SE_KEY_PHONE_CLOSE: 
    case SE_KEY_PHONE_OPEN:
		break;

    case SE_CALL_COMMAND_ERROR:
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
        //EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_RecordingAutoAnswer()
**
** Description: 
**     ��ȭ �� �ڵ����� ���� �Ѿ�� ��Ʈ�� �� ������ ������ �����ϴ� �Լ�
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

#define sFAST_PACEMAKER_SECOND      1000    /* 1 sec */

void SKY_API SKA_RecordingAutoAnswer(SKY_EVENT_T* pEvent)
{
    VMEMOINFO_T*           VoiceInfo;
    STATIC UINT8           nRemainSecs;
	STATIC UINT8           nCurrentVol;
    STATIC BOOL            fFirst, fMentStart, fEndBeep, fStartBeep;
    BYTE                   pszAAns[AANS_BUF]; 

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);      
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_AUTOANSWER_RECORDING), 
            WA_SHOW_HEADER);  
        VoiceInfo = GetVoiceInfo();
        NewMissedCall();    /* Missed Call */
        SKY_SET_MUTE(TRUE);
        SKY_SetSoundDevice( SND_MUTE_UNMUTED, SND_MUTE_MUTED, TRUE );
        gnVocoderMode = VR_AUTO;
        //nRemainSecs = MIN(VoiceInfo->wRemainedTime, AANS_SECS_PER_ONE);
        nRemainSecs = MIN(GetVoiceMemoRemainTime(), AANS_SECS_PER_ONE);
        SetRemainTime(nRemainSecs);
        sInitAutoAnsRecordingDisp(nRemainSecs, pszAAns);
  		nCurrentVol = SKY_GET_SPEAKER_VOLUME(); 
        EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, FALSE);
        fFirst = TRUE;
        fMentStart = FALSE;
        fEndBeep   = FALSE;
        fStartBeep = FALSE;
        SKY_SET_KEY_TONE(FALSE);
		SKY_SET_TRANSMIT_DTMF(FALSE);        
//        EQS_FitCanvasSize(); 

        /* make faster for display call time */
        SKY_SetPaceMakerTimer(sFAST_PACEMAKER_SECOND, TRUE);
        break;

    case SE_APP_END :     /* The very last event */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif//FEATURE_SKY_SUBLCD
	    SKY_SetVoiceVolume(nCurrentVol);
		SKY_SET_TRANSMIT_DTMF(TRUE);
        SKY_SET_KEY_TONE(TRUE);
        EQS_ReleaseDC();
        
        /* restore pacemaker repeat time */
        SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);
        break;
    
    case SE_APP_SUSPEND : /* start child */
	    SKY_SetVoiceVolume(nCurrentVol);
		SKY_SET_TRANSMIT_DTMF(TRUE);
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        /* NEW �̹��� */
        EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
                      VM_RECORD_REC_NUM_IMAGE_YPOS,
                      &g_BmpAutoAnswerPannelNew);
        /* ���� */
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                      VM_HORIZONTAL_LINE_YPOS,
                      &g_BmpHorizontalLine);
        break;    

    case SE_TM_TIMER1:
	    SKY_SetVoiceVolume(1);
		//youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
		//Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
		//���� Ű���Լ��� ����Ѵ�. ����� ��ȭ�������� ���ٸ� �ʿ����.
        //sSka_StartRecording
        PLAY_voice_start_traffic();      
        fMentStart = TRUE;
        break;

    case SE_AANS_PLAY_END: 
       if(fFirst == TRUE) /* PLAY_voice_start_traffic�� ���� �̺�Ʈ�� ���ƿ´�.*/
       {
         PLAY_voice_stop_traffic();
         fFirst = FALSE;
       }
       else               /* PLAY_voice_stop_traffic�� ���� �̺�Ʈ�� ���ƿ´�.*/
       {
            if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS || \
                SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());      //white 2001.07.03

         EQS_START_SIBLING_EVENT(sSKA_StartRecording, SE_AANS_PLAY_END, EQS_NA, EQS_NA, TRUE);
       }
        break;

    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:
        break;

    case SE_CALL_ENDED:
  //youandi 01.06.28 
  //tramp_set_isr(TRAMP_VOC_DEC_ISR, NULL)�� ���� ó���� �ʿ䰡 ����.
  //�̺κ��� Ȧ����̳� �ڵ����� �޼����� �����ϴ� ���Դϴ�.
  //�ݿ��尡 �Ͼ�� Ȧ����̳� �ڵ����� �޼����� ����� �� 
  //�޽��� ��������� �ʹ� �ʾ����� 
  //���ڴ��� ���̽���Ŷ�� �����ִ� ���·� ���ڴ������� �Ǿ� Ű���� �����ϴ�.
  //���� �޼��� ��������� ���ڴ� ������(�̰� �� ����Ǹ鼭������ �Ǵ� ���Դϴ�.)
  //���� VOCODER_DELAY msec ���� �ξ�� �ϸ� �ִ��� �ݿ��尡 �����ϴ� �մ����� ��ġ���Ѿ� �մϴ�.
  //������ ����� UI_TASK�� �ݿ��� ó���κп��� �޼����� �����ߴµ�
  //����� CM_TASK�� cmcall.c �� cmcall_end���� �����մϴ�.
	    PLAY_voice_stop_traffic_with_noevent();
        SKY_SET_MUTE(FALSE);
        SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
        gnVocoderMode = VR_NONE;
		SKY_SET_TRANSMIT_DTMF(TRUE);
        EQS_START_SIBLING_CURRENT_EVENT(SKA_InProg, FALSE);
        break;

    case SE_CALL_COMMAND_ERROR:/*UI_CALL_CMD_ERR_E: */
        break;

    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_DOUBLE_CLICK:
    case SE_KEY_AUTO_ANSWER:
    case SE_KEY_RECORD:
    case SE_KEY_RECORD_LONG:
    case SE_KEY_PLAY:
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
    case SE_KEY_CENTER:
    case SE_KEY_STAR :
    case SE_KEY_POUND :
    case SE_KEY_UP :
    case SE_KEY_DOWN :
    case SE_KEY_LEFT:
    case SE_KEY_RIGHT:
    case SE_SOFTKEY_LEFT :   /* PF3 : */
    case SE_SOFTKEY_RIGHT :  /* PF2 : */
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG : /* UI_CLRALL_F: */
	case SE_KEY_F1:
    case SE_KEY_F2:
        break;


    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND:  
       if(fMentStart == TRUE)
       {
         /* Ment�� ������ �ִ� ���¿��� Ű �̺�Ʈ�� �߻����� ��� */
         /* ������ ��Ʈ�� ���߰� ��ȭ�� ���·� �Ѿ��.          */
         /* PLAY_voice_stop_traffic()���� �̺�Ʈ�� �����ִ� �κи�*/
         /* ������ �ڵ��̴�.                                      */
         PLAY_voice_stop_traffic_with_noevent();
         fMentStart = FALSE;
	   }
       sExitAutoAnswerState();
       ClearMissedCallInfo();
       EQS_START_SIBLING(SKA_InProg, FALSE);
        break;
    

    case SE_KEY_END:
       if(fMentStart == TRUE)
       {
         /* Ment�� ������ �ִ� ���¿��� Ű �̺�Ʈ�� �߻����� ��� */
         /* ������ ��Ʈ�� ���߰� ��ȭ�� ���·� �Ѿ��.          */
         /* PLAY_voice_stop_traffic()���� �̺�Ʈ�� �����ִ� �κи�*/
         /* ������ �ڵ��̴�.                                      */
         PLAY_voice_stop_traffic_with_noevent();
         fMentStart = FALSE;
	   }
       sExitAutoAnswerState();
       ClearMissedCallInfo();
       EQS_START_SIBLING_EVENT(SKA_InProg, SE_KEY_END, EQS_NA, EQS_NA, FALSE);
        break;


    case SE_IO_MESGBOX_END:
       sExitAutoAnswerState();
       EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_KEY_MANNERS:
        break;


    case SE_KEY_PHONE_CLOSE: 
    case SE_KEY_PHONE_OPEN:
		break;

    default :
        CallDefaultEventProc(pEvent, INPROG);
        //EQS_DEF_EVENT_PROC(pEvent);
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
#define AA_MAX_MEMT_MODE  4


void UpdateAutoAnsDisp(BYTE bMode)
{
    BYTE                bCurrentMode;
    CHAR                szBuf[AANS_BUF];
    SKY_TOKEN           AutoAns;
	ANIMATION_INFO_T*   pANIMATION;
	HCONTROL            hControl;
    
    bCurrentMode = (bMode + 1) % AA_MAX_MEMT_MODE;
    
    szBuf[0] ='\0';
    
    switch(bCurrentMode)
    {
    case GM_GENERAL:   /* �Ϲ� - �ý��� �⺻ */
        pANIMATION = &g_AniAutoAnswerGeneral;//&g_AniAutorply55x39x256c;
		sprintf(szBuf, "  %s  ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
        SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
        break;

    case GM_DRIVING:   /* ���� �� */
        pANIMATION = &g_AniAutoAnswerDrive;//&g_AniAutorplyDrive55x39x256c;
        sprintf(szBuf, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_DRIVE));
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
        SKY_SET_AUTO_ANSWER_GREETING(GM_DRIVING);
        break;

    case GM_MEETING:   /* ȸ�� �� */
        pANIMATION = &g_AniAutoAnswerMeeting;//&g_AniAutorplyMeet55x39x256c;
        sprintf(szBuf, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_MEETING));
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
        SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
        break;


    case GM_USER:      /* ����� ���� ���� */
        pANIMATION = &g_AniAutoAnswerUser;//&g_AniAutorplyUser55x39x256c;
		sprintf(szBuf, " %s ", SKY_GET_TOKEN(TKN_AUTOANS_USER));
        AutoAns = TKN_SLCD_AUTOANS_USER;
        SKY_SET_AUTO_ANSWER_GREETING(GM_USER);
        break;

    default :
        pANIMATION = &g_AniAutoAnswerGeneral;//&g_AniAutorply55x39x256c;
		sprintf(szBuf, "  %s  ", SKY_GET_TOKEN(TKN_AUTOANS_GENERAL));
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
		SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
		break;
    }
    
	EQS_DeleteControl(EQS_GetNthIOC(AAD_ANIMATION));

	hControl = EQC_Animation( AANS_RECORD_ANIMATION_XPOS,   /* �ڵ����� �ִϸ��̼� */  
                              AANS_RECORD_ANIMATION_YPOS,   // + AANS_DISPLAY_ANI_OFFSET_YPOS, 
                              pANIMATION,
                              NULL);
	if(SKY_IS_PHONE_OPEN())
	{
  	  EQC_StartAnimation(hControl, TRUE);
	}

    EQC_SetStaticText(EQS_GetNthIOC(AAD_MODE), szBuf);

#ifdef FEATURE_SKY_SUBLCD
    //{
        //BYTE szAuto[AANS_BUF];
        
        /*sprintf(szAuto, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                        SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                        SKY_GET_TOKEN(AutoAns));*/
        SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                                        SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                                        SKY_GET_TOKEN(AutoAns));
        //SKY_SubLCDTextOutStr(LN_LOWER, szBuf);
    
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
    //}
#endif //#ifdef FEATURE_SKY_SUBLCD
    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
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
void UpdateAutoAnsDispInDebugScreen(BYTE bMode)
{
    BYTE                bCurrentMode;
    CHAR                szBuf[AANS_BUF];
    SKY_TOKEN           AutoAns;
	ANIMATION_INFO_T*   pANIMATION;
	HCONTROL            hControl;
    
    bCurrentMode = (bMode + 1) % AA_MAX_MEMT_MODE;
    
    szBuf[0] ='\0';
    
    switch(bCurrentMode)
    {
    case GM_GENERAL:   /* �Ϲ� - �ý��� �⺻ */
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
        SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
        break;

    case GM_DRIVING:   /* ���� �� */
        AutoAns = TKN_SLCD_AUTOANS_DRIVE;
        SKY_SET_AUTO_ANSWER_GREETING(GM_DRIVING);
        break;

    case GM_MEETING:   /* ȸ�� �� */
        AutoAns = TKN_SLCD_AUTOANS_MEETING;
        SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
        break;


    case GM_USER:      /* ����� ���� ���� */
        AutoAns = TKN_SLCD_AUTOANS_USER;
        SKY_SET_AUTO_ANSWER_GREETING(GM_USER);
        break;

    default :
        AutoAns = TKN_SLCD_AUTOANS_GENERAL;
		SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
		break;
    }
    
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDTextOutStrF(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_SET_AUTO_ANSWER_FMT), 
                                        SKY_GET_TOKEN(TKN_AUTO_ANSWER), 
                                        SKY_GET_TOKEN(AutoAns));
    
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
    EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
}







/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainAutoAnswer()
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
void SKY_API SKA_MainAutoAnswer(SKY_EVENT_T* pEvent)
{
    BYTE                   bCurrentMode;  
    STATIC HCONTROL        hCtrl, hCtrlFull, hCtrlRelease; 
    VMEMOINFO_T*           VoiceInfo;
    STATIC BOOL            fDefence;
  
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            SKY_GET_TOKEN(TKN_AUTOANS_TITLE), 
            WA_SHOW_HEADER);       

      bCurrentMode = SKY_GET_AUTO_ANSWER_GREETING();  /* Set Current Mode from NV */
      InitAutoAnsDisp(bCurrentMode);
//      EQS_FitCanvasSize();
      
      fDefence = TRUE;
      VoiceInfo = GetVoiceInfo();
      if(((NV_MAX_FL_FILE_NUM-1) >= VoiceInfo->bTotalRecNumber) &&
         /*(VoiceInfo->wRemainedTime  >= 30) && */
         (GetVoiceMemoRemainTime() >= 30) &&
         (VoiceInfo->dwRemainedByte > VM_MIN_REMAINED_BYTE))
      {
        
        SKY_DisplayBellVibIcon();
        SKY_SET_AUTO_ANSWER(TRUE);
        hCtrl = EQC_MessageBox(SKY_GET_TOKEN(TKN_AUTOANSWER_START),
                               MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        EQC_SET_STYLE(hCtrl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);          
      }
      else
      {
        SKY_SET_AUTO_ANSWER(FALSE);
        hCtrlFull = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOSPACE),
                               MESSAGE_BOX_TIME_UNLIMIT, MBT_NONE, MBT_FAIL);
        EQC_SET_STYLE(hCtrl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY); 
      }
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

    case SE_KEY_RECORD:
    case SE_KEY_PLAY:
    case SE_KEY_END:
        break;

    case SE_KEY_AUTO_ANSWER:
       bCurrentMode = SKY_GET_AUTO_ANSWER_GREETING();
       UpdateAutoAnsDisp(bCurrentMode);      
        break;

    case SE_KEY_AUTO_ANSWER_LONG:
      if((SKY_IS_AUTO_ANSWER() == TRUE) && (fDefence == FALSE))
      {
        SKY_SET_AUTO_ANSWER(FALSE);
        SKY_DisplayBellVibIcon();
        hCtrlRelease =  EQC_MessageBox(SKY_GET_TOKEN(TKN_AUTOANSWER_END),
                                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
        EQC_SET_STYLE(hCtrlRelease, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
      }
      break;


    case SE_IO_MESGBOX_END:
      fDefence = FALSE;
      if((pEvent->wParam == hCtrlFull) || (pEvent->wParam == hCtrlRelease))
      {        
        EQS_END();
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
void sInitRingTimeMenu()
{
   pszRingTime[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_RING_TIME1); 
   pszRingTime[1] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_RING_TIME2);
   pszRingTime[2] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_RING_TIME3);
   pszRingTime[3] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_RING_TIME4);
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
void sInitMentMenu()
{
  pszMentType[0] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_GENERAL); 
  pszMentType[1] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_DRIVE); 
  pszMentType[2] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_MEETING); 
  pszMentType[3] = (BYTE*)SKY_GET_TOKEN(TKN_AUTOANS_USER);   
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitSetAutoAnswerDisp(CHAR szBuf[]) 
**
** Description: 
**     Initialize AutoAnswer Setting Menu Display
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
void sInitSetAutoAnswerDisp()
{
  HCONTROL hControl;
  UINT8    nCtrl;

  sInitRingTimeMenu();
  sInitMentMenu();

  //���︲�ð�����
  EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTOANS_RING_TITLE);
  //���︲ ���ð� ���õ� �޴�
  hControl = EQC_ScrollCombo(EQS_NA, EQS_NA, AANS_NUM_CHAR, AANS_NUM_SET_ITEM, pszRingTime, 
                            AANS_NUM_SET_ITEM, AANS_NUM_SET_ITEM, SKY_GET_AUTO_ANSWER_BELL_TIME());
  EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_ENUMBERATED );
  //�ȳ���������
  EQC_StaticText(EQS_NA, EQS_NA, TKN_AUTOANS_MENT_TITLE);  
  //�ȳ��������ð� ���õ� �޴�
  hControl = EQC_ScrollCombo(EQS_NA, EQS_NA, AANS_NUM_CHAR, AANS_NUM_SET_ITEM, pszMentType,
                            AANS_NUM_SET_ITEM, AANS_NUM_SET_ITEM, SKY_GET_AUTO_ANSWER_GREETING());
  EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_SCROLL_LIST_ENUMBERATED );


  EQS_StartFormatter();
  for(nCtrl = 0; nCtrl < ASS_MAX; nCtrl ++)
  {
     EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);              
     EQS_NewLine();
  }
   EQS_EndFormatter();
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
void SKY_API SKA_SetAutoAnswer(SKY_EVENT_T* pEvent)
{
    STATIC BOOL         fKeyEnd;
    STATIC HCONTROL     hCtrlSaved;
    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :   /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X, 
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE_SELECT_SAVE, 
                    SKY_GET_TOKEN(TKN_AUTO_ANSWER_SET), 
                    WA_SHOW_CML | WA_SHOW_HEADER);
	  sInitSetAutoAnswerDisp();
//      EQS_FitCanvasSize();
	  fKeyEnd = FALSE;      
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

    
	case SE_KEY_END:
		fKeyEnd = TRUE;
	case SE_KEY_CLEAR:
		if(EQS_IsControlDataChanged())
		{
			EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ASK_SAVING_MSG), 
							MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
		}
		else
		{
			if(pEvent->EventID == SE_KEY_END)
			{
				
				EQS_END_TO(EQS_POP_TO_ROOT);
			}
			else
				EQS_End();
		}
		break;

    case SE_IO_MESGBOX_NO:
		EQS_END();
		break;

    case SE_IO_MESGBOX_YES:
    case SE_SOFTKEY_SAVE:        
        switch(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(ASS_RING_LIST)))
        {
        case 0:
          SKY_SET_AUTO_ANSWER_BELL_TIME(AAB_IMMEDIATE);
          break;

        case 1:
          SKY_SET_AUTO_ANSWER_BELL_TIME(AAB_VIB_ONCE);
          break;

        case 2:
          SKY_SET_AUTO_ANSWER_BELL_TIME(AAB_BELL_5SECOND);
          break;

        case 3:
          SKY_SET_AUTO_ANSWER_BELL_TIME(AAB_BELL_10SECOND);
          break;
        }


        switch(EQC_GetScrollLogicalIndex(EQS_GetNthIOC(ASS_MENT_LIST)))
        {
        case 0:
          SKY_SET_AUTO_ANSWER_GREETING(GM_GENERAL);
          break;

        case 1:
          SKY_SET_AUTO_ANSWER_GREETING(GM_DRIVING);
          break;

        case 2:
          SKY_SET_AUTO_ANSWER_GREETING(GM_MEETING);
          break;

        case 3:
          SKY_SET_AUTO_ANSWER_GREETING(GM_USER);
          break;
        } 

        hCtrlSaved =  EQC_MessageBox(SKY_GET_TOKEN(TKN_AUTOANSWER_SETTING_SAVED), MESSAGE_DEFAULT_TIME, 
                                     MBT_NONE, MBT_SUCCESS);

      break;
    
    case SE_IO_MESGBOX_END:
		if((fKeyEnd == TRUE) && (pEvent->wParam == hCtrlSaved))
		{
		  EQS_END_TO(EQS_POP_TO_ROOT);
		}
        else if((fKeyEnd == FALSE) && (pEvent->wParam == hCtrlSaved))
        {
          EQS_END();
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
**     sInitAutoAnswerSettingDisplay()
**
** Description: 
**     Initialize User Ment Display
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
//void sInitAutoAnswerSettingDisplay()
//{
//  UINT8 nCtrl;
//  
//  EQC_PushButton(EQS_NA, EQS_NA, TKN_AUTOANS_START_RECORDING);
//  EQC_PushButton(EQS_NA, EQS_NA, TKN_AUTOANS_LISTEN_USERMENT);
//
//  EQS_StartFormatter();
//  for(nCtrl = 0; nCtrl < RU_MAX; nCtrl ++)
//  {
//     EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);              
//     EQS_NewLine();
//  }
//  EQS_EndFormatter();
//}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitPlayUserMentDisplay()
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
void sInitPlayUserMentDisplay()
{
   HCONTROL      hControl;
   BYTE          pszVMemo[AANS_BUF];
   VMEMOINFO_T*  VoiceInfo = GetVoiceInfo();
   INT           nUSERMENT_XPOS;
   //RECDATA_T*    CurrentDataHeader = GetRECDataHeader(); 


   /* ��� ���ϸ��̼� */  
   EQC_StartAnimation(EQC_Animation( USER_RECORD_ANIMATION_XPOS,            
                  USER_RECORD_ANIMATION_YPOS,
                  &g_AniPlaying,//&g_AniPlay51x34x256c, //&g_AniRepuse52x47x256C,
                  NULL), TRUE);   

   /* �������� */
   hControl = EQC_StaticText(USER_PLAY_REC_TYPE_XPOS, 
                             USER_PLAY_REC_TYPE_YPOS, 
                             TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);

   
   /* Background Image */
   //EQC_Image( USER_RECORD_BACKGROUND_XPOS, 
   //           USER_RECORD_BACKGROUND_YPOS, 
   //		    &g_BmpRemainTimePannel);
             //(BITMAP*)&g_BmpTable07_115x47x256c); /* ���� �̹���*/  
    

   /* �λ縻 ����� */
   nUSERMENT_XPOS = (SKY_SCREEN_WIDTH - EQS_TextWidth(SKY_GET_TOKEN(TKN_USER_MENT_PLAYING)))/2;

   hControl = EQC_StaticText(nUSERMENT_XPOS, 
                             USER_PLAY_RECORDING_TIME_YPOS, 
                             (EQS_TOKEN)TKN_USER_MENT_PLAYING);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);


#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDEndText(LN_UPPER);
    SKY_SubLCDEndText(LN_LOWER);

    SKY_DrawSubAnnunciators();
    SKY_SubLCDTextOut(LN_LOWER, TKN_USER_MENT_PLAYING);    
#endif //#ifdef FEATURE_SKY_SUBLCD
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
void sInitRecordUserMentDisplay(BYTE pszVMemo[], DWORD dwRecordingTime)
{
   HCONTROL          hControl;
   clk_julian_type   Julian;
   //VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();
   WORD              wRemainTime = GetUserMentRemainTime();
   
   EQC_StartAnimation(EQC_Animation( USER_RECORD_ANIMATION_XPOS, 
                                     USER_RECORD_ANIMATION_YPOS, 
                                     &g_AniRecording,//&g_AniRec51x34x256c,//&g_AniRepuse52x47x256C,
                                     NULL), TRUE);


   clk_secs_to_julian(dwRecordingTime, &Julian);
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   
   hControl = EQC_StaticText(USER_RECORD_RECORDINGTIME_XPOS, 
                             USER_RECORD_RECORDINGTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);        /* �����ð�             */ 
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl, pszVMemo);


   //EQC_Image( USER_RECORD_BACKGROUND_XPOS, 
   //           USER_RECORD_BACKGROUND_YPOS, 
   //  		    &g_BmpRemainTimePannel);
             //(BITMAP*)&g_BmpTable07_115x47x256c);          /* ���� �̹���*/
   
   hControl = EQC_ProgressBar(USER_RECORD_PBAR_XPOS, 
                              USER_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              /* hjcho 01.05.13 �λ縻������ ���������� �и� */
                              /* VM_MAX_PROGRESS,                            */
                              USERMENT_MAX_PROGRESS,
                              wRemainTime,
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE); /* ��Ŀ���� ���� ���ϵ��� �ϴ� ��ũ�� */

   sprintf(pszVMemo,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(USER_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             USER_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszVMemo);

   /* hjcho 01.05.13 �λ縻������ ���������� �и�            */
   /* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian); */
   clk_secs_to_julian(wRemainTime, &Julian);
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(USER_RECORD_REMAINTIME_XPOS,             /* ���� �ð� */
                             USER_RECORD_REMAINTIME_YPOS, 
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
void sUpdateRecordUserMentDisplay
(
 BYTE pszVMemo[], 
 DWORD dwRecordingTime, 
 DWORD dwRemainTime
)
{
    clk_julian_type   Julian;

    //�����ð�
    clk_secs_to_julian(dwRecordingTime, &Julian);
    sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(RUD_RECORDING_TIME), pszVMemo);

    // ���� �ð�
    clk_secs_to_julian(dwRemainTime, &Julian);
    sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(RUD_REMAIN_TIME), pszVMemo);

    // Progress Bar 
    EQC_SetProgressBar(EQS_GetNthIOC(RUD_PROGRESS), dwRemainTime);

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDTextOut(LN_UPPER, TKN_USER_MENT_RECORDING);
    SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_RECROD_TIME_FMT), Julian.minute, Julian.second);
#endif //#ifdef FEATURE_SKY_SUBLCD

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
void SKY_API sSKA_StartRecordingUserMent(SKY_EVENT_T* pEvent)
{
    VMEMOINFO_T*         VoiceInfo;
    STATIC RECDATA_T*    RECDataHeader;
    STATIC DWORD         dwRecordingTime; 
    STATIC DWORD         dwRemainTime;
    STATIC HCONTROL      hCtrlDel, hCtrlFull;
    STATIC HCONTROL      hCtrlCancelMsgBox, hCtrlSaveMsgBox, hCtrlMsgSaved;
    STATIC BOOL          fCancel, fFirst, fKeyEnd, fPopUpDeletePreviousMessage;
    BYTE                 pszVMemo[AANS_BUF];

    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START :   /* The very first event */
      EQS_GetDC(TRUE);
      RECDataHeader   = GetRECDataHeader();
      VoiceInfo       = GetVoiceInfo();
      /*    hjcho 01.05.13 �λ縻������ �����޸� �и�    */
      /*    dwRemainTime    = VoiceInfo->wRemainedTime;    */
      dwRemainTime = GetUserMentRemainTime();
      dwRecordingTime = 0;  
      EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_SAVE, 
            SKY_GET_TOKEN(TKN_USER_MENT_RECORDING), 
            WA_SHOW_HEADER | WA_SHOW_CML);
      fCancel = FALSE;
      fFirst  = TRUE;
	  fKeyEnd = FALSE;
      fPopUpDeletePreviousMessage = FALSE;
      sInitRecordUserMentDisplay(pszVMemo, dwRecordingTime);
	  
     
      if(VoiceInfo->fUserVoice)
      {
        gnVocoderMode = VR_NONE;
        fPopUpDeletePreviousMessage = TRUE;
        hCtrlDel = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_DELETING_PREVIOUSMEMT), 0,
                                  MBT_NONE, MBT_HELP);
        //EQC_SET_STYLE(hCtrlDel, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
      }
      else
      {     
        //youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
	    //Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
	    //���� Ű���Լ��� ����Ѵ�. 
        SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
        EQS_StartTimer(EQS_TIMER_3, VOCODER_DELAY, FALSE);
      }
        break;

    case SE_APP_END :     /* The very last event */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
        SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :    /* draw */
        /* �λ縻�̹��� */
		EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
				      VM_RECORD_REC_NUM_IMAGE_YPOS-2,
					  &g_BmpUserMentRecord);
        /* ���� */ 
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                      VM_HORIZONTAL_LINE_YPOS,
                      &g_BmpHorizontalLine);
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;

    case SE_CALL_INCOMING:
         SKY_SET_ALERT(TRUE);
         gnVocoderMode = VR_RING;
         EQS_START_SIBLING_EVENT(SKA_CallIncomingInRecordingVMemo, SE_CALL_INCOMING,
                                 CIFW_USERMENT, EQS_NA, TRUE);
        break;
    
    case SE_VM_REC_END :  
        if(fCancel)  /* �������� ������ �ߴ��ϰ� ���� ��� */
        {
		  Release_play_rec();
		  VoiceRecordTransacCancel(GetCurrentFn(), VTYPE_USERMENT);
          gnVocoderMode = VR_NONE;
          hCtrlCancelMsgBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_CANCEL), MESSAGE_DEFAULT_TIME, 
                                                   MBT_NONE, MBT_HELP);
          //EQC_SET_STYLE(hCtrlCancelMsgBox, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
          fCancel = FALSE;
        }
        else         /* ���������� ������ ��� */
        {		          
          gnVocoderMode = VR_NONE;
          hCtrlSaveMsgBox = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVING), 0, 
                                                 MBT_NONE, MBT_SUCCESS);
        }
       break;


    case SE_VM_REC_START:
        VoiceInfo = GetVoiceInfo();

        SetCurrentFn(VoiceRecordTransacStart(VTYPE_USERMENT));
        if(GetCurrentFn() < NV_MAX_FL_FILE_NUM)
        {
          RECDataHeader = GetRECDataHeader();
          RECDataHeader->wServiceOption = CAI_SO_VOICE_EVRC;
          RECDataHeader->dwTime = SKY_GetLocalTime();
          flfwrite(GetCurrentFn() , (BYTE *)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);

          //sSKA_StartRecordingUserMent
          Play_rec_init(VOC_CAP_EVRC);		  
		  //sSKA_StartRecordingUserMent
          REC_voice_start();          

		  EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
        }
        else
        {
          EQS_END();
        }
        /* hjcho 01.05.13  �λ縻������ ���������� �и�                           */
        /* if(VoiceInfo->dwRemainedByte < 1024 || VoiceInfo->wRemainedTime == 0)  */
        if(VoiceInfo->dwRemainedByte < 1024 || GetUserMentRemainTime() == 0)  
        {
          hCtrlFull = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOMORE_RECORDING), MESSAGE_DEFAULT_TIME, 
                                     MBT_NONE, MBT_FAIL);
          //EQC_SET_STYLE(hCtrlFull, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
        }
       break;

    case SE_TM_TIMER1 :  //UI_TIMER_F:
       dwRemainTime--;
       if(dwRemainTime == 0)
       {
	     gnVocoderMode = VR_NONE;
         dwRecordingTime++;
         sUpdateRecordUserMentDisplay(pszVMemo, dwRecordingTime, dwRemainTime);
         EQS_StopTimer(EQS_TIMER_1);
         EQS_SEND_EVENT(SE_KEY_RECORD, EQS_NA, EQS_NA);
         //EQS_End();
       }
       else 
       {
         dwRecordingTime++;
         Data_Record_time++;
         sUpdateRecordUserMentDisplay(pszVMemo, dwRecordingTime, dwRemainTime);
       }
       EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);       
      break;

    case SE_TM_TIMER2:
       EQS_SEND_EVENT(SE_VM_REC_END, EQS_NA, EQS_NA);
      break;


    case SE_TM_TIMER3:         
         EQS_SEND_EVENT(SE_VM_REC_START, 0, 0);
         break;

    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END :
		fKeyEnd = TRUE;
    case SE_KEY_RECORD:   /* ���� �߿� ���� ª�� Ű�� ������ ��� --> ������ ���� ��Ų��. */
    case SE_SOFTKEY_SAVE:
       if(fFirst == TRUE)
       {
         fFirst = FALSE;
         REC_voice_stop_with_no_event();
         EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
         //REC_voice_stop();
       }
        break;    

    case SE_KEY_CLEAR:
    case SE_SOFTKEY_CANCEL:
       if(fCancel == FALSE)
       {
        fCancel = TRUE;
        REC_voice_stop_with_no_event();
        EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
		//REC_voice_stop();
       }
        break;

    case SE_SYS_TO_IDLE:
		break;

    case SE_IO_MESGBOX_END:
       if((pEvent->wParam == hCtrlDel) && (fPopUpDeletePreviousMessage== TRUE))
       {
         gnVocoderMode = VR_RING;
         VoiceInfo = GetVoiceInfo();
         VoiceEraseTransac(VoiceInfo->bUserVoiceFn);
         SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
         SetCurrentFn(VoiceRecordTransacStart(VTYPE_USERMENT));

         if(GetCurrentFn() < NV_MAX_FL_FILE_NUM)
         {
           
		   RECDataHeader = GetRECDataHeader();
           RECDataHeader->wServiceOption = CAI_SO_VOICE_EVRC;
           RECDataHeader->dwTime = SKY_GetLocalTime();
           flfwrite(GetCurrentFn() , (BYTE *)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);

		   //sSKA_StartRecordingUserMent
           REC_voice_start();
		   
		   VoiceInfo = GetVoiceInfo();
           /*    hjcho 01.05.13 �λ縻������ ���������� �и�    */
           /*    dwRemainTime    = VoiceInfo->wRemainedTime;    */
           dwRemainTime = GetUserMentRemainTime();
           dwRecordingTime = 0;
		   sUpdateRecordUserMentDisplay(pszVMemo, dwRecordingTime, dwRemainTime);
           EQS_StartTimer(EQS_TIMER_1, VM_ONE_SEC, TRUE);
         }
         else
         {
           EQS_END();
         }
       }
       else if(pEvent->wParam == hCtrlSaveMsgBox)
       {
          //youandi 01/05/09
		  //�ڵ����� ��Ʈ ���� ������ ���� ���뿡 ������� �ֱ� ���ؼ� �� �ڵ带 ������.
		  //�ڵ����� ��Ʈ ������ �Ϲݳ���(not��ȭ��)�� ����Ѵ�.
		  //���� �� ���Ǵ� ISR�� Fn_Packet_Record�̸�
		  //ISR�� ����Ǹ鼭 �ڵ����� ��Ʈ�� ����Ǵ� ���� �� �����̴�.
		  //1.ISR�� ���ٰ� ���� ����(�ð�?)�� ���ڸ��� ��ü������ 
		  // REC_Voice_stop�� �����Ű�� REC_Voice_stop�� SE_VM_REC_END �̺�Ʈ�� �߻����Ѽ�
		  // ����������� �ȴ�.
		  //2. ����ڰ� ���Ḧ �����ϴ� Ű�� ������ ISR�� �����Ų��.
		  // �� ��� Ű ������ ���ÿ� REC_voice_stop_with_no_event�� ����ǰ� 
		  // VOCODER_DELAY msec �����̰� ����� �Ŀ� SE_VM_REC_END�� �߻��Ǿ� ������� �´�.
		  // REC_Voice_stop�� REC_voice_stop_with_no_event�� �����Լ��̹Ƿ� �ٲٸ� �� �ǰ� 
		  // ������Ʈ��� �����ϱ� ������ �̺κп��� ������� �־��־�� �Ѵ�.
		  /////////////////////////////////////////////////////////////////////////////////
		  MakeBeep();
		  /////////////////////////////////////////////////////////////////////////////////
		  Release_play_rec();
		  VoiceRecordTransacEnd(GetCurrentFn(), VTYPE_USERMENT);  
          hCtrlMsgSaved = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_SAVE_SUCCESS), 1, MBT_NONE,
                                         MBT_SUCCESS);
       }
       else
       {
         if(fKeyEnd ==TRUE)
		 {
		    EQS_END_TO(EQS_POP_TO_ROOT);
		 }
		 else
		 {
           EQS_END();
		 }
       }
        break;

     case SE_KEY_MANNERS:
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
void SKY_API sSKA_PlayingUserMent(SKY_EVENT_T* pEvent)
{
    VMEMOINFO_T*        VoiceInfo;
    RECDATA_T*          RECDataHeader;
    STATIC BOOL         fFirst;
    STATIC BOOL         fErase;
    STATIC HCONTROL     hCtrlSave, hCtrlDelete, hCtrlInDelete, hCtrlEndDelete, hCtrlNone;
	STATIC SKY_EVENT_E  PreviousEvent;
 
    switch(pEvent->EventID)
    {
    case SE_APP_START :   /* The very first event */
       EQS_GetDC(TRUE);
       EQS_SetWindow(SKY_DEF_WIN_X, 
                     SKY_DEF_WIN_Y, 
                     SKY_DEF_WIN_DX, 
                     SKY_DEF_WIN_DY, 
                     CL_NONE_NONE_DELETE, 
                     SKY_GET_TOKEN(TKN_USER_MENT_PLAY), 
                     WA_SHOW_CML | WA_SHOW_HEADER);
       fFirst = TRUE;
       fErase = FALSE;
       gnVocoderMode = VR_MEMO;
	   PreviousEvent = SE_KEY_NONE;
       sInitPlayUserMentDisplay();
        break;

    case SE_APP_END :     /* The very last event */
        SKY_SET_KEY_TONE(TRUE);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
        EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
        gnVocoderMode = VR_NONE;
        SKY_SET_KEY_TONE(TRUE);
        break;
    
    case SE_APP_RESUME :  /* end child */
		SKY_SET_KEY_TONE(TRUE);
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_UPPER);
        SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
        break;
    
    case SE_APP_DRAW :    /* draw */
        /* �λ縻�̹��� */
		EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
				      VM_RECORD_REC_NUM_IMAGE_YPOS-2,
					  &g_BmpUserMentRecord);
        /* ���� */
        EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                    VM_HORIZONTAL_LINE_YPOS,
                    &g_BmpHorizontalLine);
        break;

    case SE_VM_PLAY_START :
       VoiceInfo = GetVoiceInfo();
       if(VoiceInfo->fUserVoice == FALSE)
       {
         gnVocoderMode = VR_NONE;
         hCtrlNone = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOUSERMENT), MESSAGE_DEFAULT_TIME,
                                    MBT_NONE, MBT_FAIL);
         //EQC_SET_STYLE(hCtrlNone, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
       }
       else
       {  
         //youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
		 //Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
		 //���� Ű���Լ��� ����Ѵ�. 
         SKY_PlayToneEx(UI_MSG_SND, SND_RING_A5, 100, SKY_GET_KEY_TONE_VOLUME());
         EQS_StartTimer(EQS_TIMER_1, VOCODER_DELAY, FALSE);  
		 SKY_SET_KEY_TONE(FALSE);
       }    
        break;

    case SE_TM_TIMER1:
	   SKY_SET_KEY_TONE(TRUE);
       if(fFirst)
       {
         fFirst = FALSE;
         VoiceInfo = GetVoiceInfo();
         RECDataHeader = GetRECDataHeader();
	 
   		 //youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
		 //Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
		 //���� Ű���Լ��� ����Ѵ�. 
         Play_rec_init(VOC_CAP_EVRC);

         if(!flfopen(VoiceInfo->bUserVoiceFn, RD_MODE))
         {
             EQS_END();         
         }
         flfread(VoiceInfo->bUserVoiceFn, (BYTE*)&(RECDataHeader->wServiceOption), REC_DATA_HEADER_SIZE);
		 
		 //youandi 01.06.28 ����/��� ���ۺκ� Ȯ�� �ٶ� Play_rec_int�� �Ҿ���
		 //Play_rec_init �տ��� snd_task�� �ݵ�� ������ ��.
		 //���� Ű���Լ��� ����Ѵ�. 
		 //sSKA_PlayingUserMent
         Fn_PLAY_voice_start(VoiceInfo->bUserVoiceFn);

       }
       else
       {
	     PLAY_voice_stop();		 
     	 gnVocoderMode = VR_NONE;
       }
       break;


    case SE_TM_TIMER2:
       Release_play_rec();  //added by hjcho
	   SKY_SET_KEY_TONE(TRUE);
       VoiceInfo = GetVoiceInfo();
       if(!flfclose(VoiceInfo->bUserVoiceFn))  EQS_END();       

       if(fErase == TRUE)
       {
         SKY_SET_KEY_TONE(TRUE);
         hCtrlDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, 
                                      MBT_CONFIRM);
       }
	    break;


    case SE_KEY_END:
    case SE_KEY_PHONE_CLOSE:
		 PreviousEvent = pEvent->EventID;
	case SE_KEY_CLEAR:
		 PLAY_voice_stop();
     	 gnVocoderMode = VR_NONE;
	    break;


    case SE_SYS_TO_IDLE: /* Ignore this event */
		break;


    case SE_VM_PLAY_END :
       Release_play_rec();  //added by hjcho
       VoiceInfo = GetVoiceInfo();
       if(!flfclose(VoiceInfo->bUserVoiceFn))  EQS_END();       

       if(fErase == FALSE)
       {
         gnVocoderMode = VR_NONE;
         hCtrlSave = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_COMPLETE_PLAY), MESSAGE_DEFAULT_TIME,
                                    MBT_NONE, MBT_SUCCESS);
       }
       else if(fErase == TRUE)
       {
         SKY_SET_KEY_TONE(TRUE);
         hCtrlDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETE), MESSAGE_DEFAULT_TIME, MBT_YESNO, 
                                      MBT_CONFIRM);
       }
	    break;


   case SE_SOFTKEY_DELETE:
       if(fErase == FALSE)
       {
         SKY_SET_KEY_TONE(FALSE);
         gnVocoderMode = VR_NONE;
         fErase = TRUE;
         PLAY_voice_stop_with_no_event();
         EQS_StartTimer(EQS_TIMER_2, VOCODER_DELAY, FALSE);
		 SKY_SET_KEY_TONE(FALSE);
       }
       break;

   case SE_IO_MESGBOX_YES:
       VoiceInfo = GetVoiceInfo(); 
       if(pEvent->wParam == hCtrlDelete)
       {
         if(fErase == TRUE)
         {
           fErase = FALSE;
           hCtrlInDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETEING), 0, MBT_NONE, 
                                          MBT_HELP);
           //EQC_SET_STYLE(hCtrlInDelete, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
         }
       }
        break;

    case SE_CALL_INCOMING:
        SKY_SET_ALERT(TRUE);
        gnVocoderMode = VR_RING;
        PLAY_voice_stop_with_no_event();
        EQS_START_SIBLING_EVENT(SKA_CallIncomingInPlayingVMemo, 
                                SE_CALL_INCOMING, EQS_NA, EQS_NA, TRUE);
        break;

   case SE_IO_MESGBOX_NO:
	   SKY_SET_KEY_TONE(TRUE);
       EQS_END();
       break;

   case SE_IO_MESGBOX_END:
       if(pEvent->wParam == hCtrlInDelete)
       {
		 VoiceInfo = GetVoiceInfo();
		 VoiceEraseTransac(VoiceInfo->bUserVoiceFn);
         hCtrlEndDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETED), MESSAGE_DEFAULT_TIME, MBT_NONE, 
                                          MBT_SUCCESS);
         //EQC_SET_STYLE(hCtrlEndDelete, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
       }
       else
       {
         if((PreviousEvent == SE_KEY_END) || (PreviousEvent == SE_KEY_PHONE_CLOSE))
		 {
           SKY_SET_KEY_TONE(TRUE);
		   EQS_END_TO(EQS_POP_TO_ROOT);
		 }	
		 else
		 {
		  SKY_SET_KEY_TONE(TRUE);
		  EQS_END();
		 }
       }
        break;

   case SE_KEY_MANNERS:
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
**     sInitRECPLAYUserMentDisp()
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

#define NUM_RECPLAY_USER_MENT_POPUP    2
#define MIN_RECORD_USER_MENT_TIME      10


SKY_TOKEN paUserMentRecord[NUM_RECPLAY_USER_MENT_POPUP] =    /* Hot Menu!! */
{
  TKN_VMEMO_START,              /* �������� */
  TKN_VMEMO_DELETE_ONE          /* ���λ��� */
};

enum tagRECPLAY_USERMENT_E
{
 RPU_ANIMATION    = 0,    
// RPU_BACKGROUND,  
// RPU_NUMBER_OF_RECORD,
 RPU_PROGRESS,  
 RPU_REMAIN_TIME_TITLE,
 RPU_REMAIN_TIME       
};

enum tagPOPUP_E
{
  PU_PLAY_REC = 0, //��������
  PU_DELETE        //���λ��� 
};


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitRECPLAYUserMentDisp()
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

void sInitRECPLAYUserMentDisp(BYTE pszVMemo[])
{
   HCONTROL          hControl;
   clk_julian_type   Julian;
   VMEMOINFO_T*      VoiceInfo = GetVoiceInfo();


   EQC_StartAnimation(EQC_Animation( USER_RECORD_ANIMATION_XPOS,   /* �ڵ����� �ִϸ��̼� */  
                                     USER_RECORD_ANIMATION_YPOS, 
                                     &g_AniAutoAnswerUser,  //&g_AniAutorplyUser55x39x256c,
                                     NULL), TRUE);

   //EQC_Image( USER_RECORD_BACKGROUND_XPOS, 
   //           USER_RECORD_BACKGROUND_YPOS, 
   //			  &g_BmpRemainTimePannel);
             //(BITMAP*)&g_BmpTable07_115x47x256c);     /* ���� �̹���*/
   
   
   hControl = EQC_ProgressBar(USER_RECORD_PBAR_XPOS, 
                              USER_RECORD_PBAR_YPOS, 
                              VM_PBAR_SIZE,
                              VM_PBAR_HEIGHT,
                              VM_MIN_PROGRESS, 
                              USERMENT_MAX_PROGRESS,
                              GetUserMentRemainTime(),
                              TRUE);
   EQC_SetProgressBarStep(hControl, VM_PBAR_STEPSIZE);
   EQC_SET_FOCUS(hControl, FALSE); /* ��Ŀ���� ���� ���ϵ��� �ϴ� ��ũ�� */

   sprintf(pszVMemo,"%s", SKY_GET_TOKEN(TKN_VMEMO_TIME));
   hControl = EQC_StaticText(USER_RECORD_REMAIN_TIME_TITLE_XPOS, 
                             USER_RECORD_REMAINTIME_YPOS,
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, CS_STATIC_TEXT_ORING);
   EQC_SetStaticText(hControl, pszVMemo);

   /*         hjcho 01.05.13 �λ縻�� ���������� �и�          */
   /* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian);   */
   clk_secs_to_julian(GetUserMentRemainTime(), &Julian);
   sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
   hControl = EQC_StaticText(USER_RECORD_REMAINTIME_XPOS,             /* ���� �ð� */
                             USER_RECORD_REMAINTIME_YPOS, 
                             (EQS_TOKEN)TKN_BLANK);
   EQC_SET_STYLE(hControl, (CS_STATIC_TEXT_NO_CHANGE|CS_STATIC_TEXT_ORING));
   EQC_SetStaticText(hControl ,pszVMemo);   

#ifdef FEATURE_SKY_SUBLCD
   SKY_SubLCDDefault();
#endif//FEATURE_SKY_SUBLCD
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
void sUpdateRECPLAYUserMentDisplay(BYTE pszVMemo[])
{
	clk_julian_type  Julian;
	//DWORD            dwCurrentProgress;
	VMEMOINFO_T*     VoiceInfo = GetVoiceInfo();

	//EQS_SEND_EVENT(SE_APP_DRAW, EQS_GetNthIOC(RPU_BACKGROUND), 0);

    
	/* �������� */
	//if(VoiceInfo->fUserVoice)
    //{
    //  sprintf(pszVMemo, "%2d", 1);   
    //}
    //else
    //{
	//  sprintf(pszVMemo, "%2d", 0);   
    //}
    //EQC_SetStaticText(EQS_GetNthIOC(RPU_NUMBER_OF_RECORD), pszVMemo);

	// �����ð�
    /*           hjcho 01.05.13 �λ縻�� ���������� �и�      */
	/* clk_secs_to_julian(VoiceInfo->wRemainedTime, &Julian); */
    clk_secs_to_julian(GetUserMentRemainTime(), &Julian);
	sprintf(pszVMemo, SKY_GET_TOKEN(TKN_VMEMO_FMT_REMAIN_TIME), Julian.minute, Julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(RPU_REMAIN_TIME), pszVMemo);

    //Progress Bar
    /*           hjcho 01.05.13 �λ縻�� ���������� �и�      */
	/*      dwCurrentProgress = VoiceInfo->wRemainedTime;     */
    //dwCurrentProgress = GetUserMentRemainTime();
	EQC_SetProgressBar(EQS_GetNthIOC(RPU_PROGRESS), GetUserMentRemainTime());

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDDefault();
#endif//FEATURE_SKY_SUBLCD
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
/*
 UserMent Recording
 EQS_START_CHILD(sSKA_StartRecordingUserMent);
*/

void SKY_API SKA_RECPLAYUserMent(SKY_EVENT_T* pEvent)
{
    STATIC HCONTROL   hCtrlFull, hCtrlDelete, hCtrlInDelete, hCtrlEndDelete;
    STATIC HCONTROL   hCtrlNoMessage;
	HCONTROL          hControl;
	STATIC BOOL       fFirst;
	BYTE              pszVMemo[AANS_BUF];
	VMEMOINFO_T*      VoiceInfo;


    switch(pEvent->EventID)
    {
      case SE_APP_START:
	       EQS_GetDC(TRUE);
           EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_DELETE_REC_PLAY,  /* ���� ���� ��� */
                        SKY_GET_TOKEN(TKN_VMEMO_RECORDPLAY_USERMENT), 
                        WA_SHOW_HEADER | WA_SHOW_CML);
		   fFirst = TRUE;
           gnVocoderMode = VR_NONE;
		   sInitRECPLAYUserMentDisp(pszVMemo);
           break;

      case SE_APP_END :     /* The very last event */
		   EQS_ReleaseDC();
           break;
    
      case SE_APP_SUSPEND : /* start child */
           break;
    
      case SE_APP_RESUME :  /* end child */
		   fFirst = TRUE;
           gnVocoderMode = VR_NONE;
           sUpdateRECPLAYUserMentDisplay(pszVMemo);
           break;
    
      case SE_APP_DRAW :    /* draw */
            /* �λ縻�̹��� */
            EQS_PutBitmap(VM_RECORD_REC_NUM_IMAGE_XPOS,
			              VM_RECORD_REC_NUM_IMAGE_YPOS-2,
			              &g_BmpUserMentRecord);
            /* ���� */
            EQS_PutBitmap(VM_HORIZONTAL_LINE_XPOS,
                           VM_HORIZONTAL_LINE_YPOS,
                           &g_BmpHorizontalLine);
            break;

      case SE_SOFTKEY_REC:
      case SE_KEY_RECORD_LONG:
		   if(fFirst == TRUE)
		   {
			   fFirst = FALSE;
 			   EQS_START_CHILD(sSKA_StartRecordingUserMent);
		   }
		   break;

      case SE_SOFTKEY_PLAY:    /* ����� ������ ��� */
           EQS_START_CHILD_EVENT(sSKA_PlayingUserMent, SE_VM_PLAY_START, EQS_NA, EQS_NA);
           break;

//      case SE_SOFTKEY_MENU:    /* �޴��� ������ ��� */
//           EQC_PopUpMenu((EQS_TOKEN_EX*)paUserMentRecord, NUM_RECPLAY_USER_MENT_POPUP, 0);
//           break;

//      case SE_IO_POPUP_SELECT:
//		   VoiceInfo = GetVoiceInfo();
//           switch(pEvent->dwParam)
//		   {
//		    case PU_PLAY_REC:
//			   EQS_SEND_EVENT(SE_KEY_RECORD_LONG, 0, 0);
//		       break;
//
//			case PU_DELETE:
//             if(VoiceInfo->fUserVoice == TRUE)
//			   {
//				   hCtrlDelete = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETE), 
//												MESSAGE_DEFAULT_TIME, 
//												MBT_YESNO, 
//												MBT_CONFIRM);
//				   //EQC_SET_STYLE(hCtrlDelete, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//			   }
//			   else
//			   {
//				   hControl = EQC_NoBeepMessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOUSERMENT),
//					                         MESSAGE_DEFAULT_TIME,
//											 MBT_NONE,
//											 MBT_CONFIRM);
//				   //EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
//			   }
//				break;
//		   }
//		   break;

		case SE_SOFTKEY_DELETE:

           VoiceInfo = GetVoiceInfo();
           if(VoiceInfo->fUserVoice == TRUE)
           {
			   hCtrlDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETE), 
				                            MESSAGE_DEFAULT_TIME, 
											MBT_YESNO, 
											MBT_CONFIRM);
           }
		   else
		   {
    		   hCtrlNoMessage = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_NOUSERMENT),
					                           MESSAGE_DEFAULT_TIME,
										       MBT_NONE,
										       MBT_FAIL);
		   }
				break;

      case SE_IO_MESGBOX_YES :
           if(pEvent->wParam == hCtrlDelete)
		   {
             hCtrlInDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETEING), 0, MBT_NONE, 
                                            MBT_HELP);
             //EQC_SET_STYLE(hCtrlInDelete, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
		   }
           break;


      case SE_KEY_MANNERS:
           break;

      case SE_IO_MESGBOX_END:
           if(pEvent->wParam == hCtrlInDelete)
		   {
             VoiceInfo = GetVoiceInfo();
		     VoiceEraseTransac(VoiceInfo->bUserVoiceFn);			 
             hCtrlEndDelete = EQC_MessageBox(SKY_GET_TOKEN(TKN_VMEMO_QUESTION_DELETED), 1, MBT_NONE, 
                                             MBT_SUCCESS);
             //EQC_SET_STYLE(hCtrlEndDelete, CS_MESSAGE_BOX_TITLE|CS_MESSAGE_BOX_SOFTKEY);
		   }
		   else if(pEvent->wParam == hCtrlEndDelete)
		   {
             sUpdateRECPLAYUserMentDisplay(pszVMemo);
		   }
           else if(pEvent->wParam == hCtrlNoMessage)
           {
               /* �λ縻�� �����Ǿ� ���� �ʾ��� ��쿡�� �״�� �д�. */
               break;
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
void SKY_API SKA_PlayingUserMent(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
      case SE_APP_START:
          EQS_START_CHILD_EVENT(sSKA_PlayingUserMent, SE_VM_PLAY_START, EQS_NA, EQS_NA);
        break;

      case SE_APP_END :     /* The very last event */
        break;
    
      case SE_APP_SUSPEND : /* start child */
        break;
    
      case SE_APP_RESUME :  /* end child */
        EQS_END();
        break;
    
      case SE_APP_DRAW :    /* draw */
        break;

      default :
        EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




