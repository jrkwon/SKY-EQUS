/*
** ===========================================================================
**
** Project: 
**     Bomber
**
** File:
**     ska_call.c
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
** 2000-12-08   hjcho      Created.
**
** ===========================================================================
*/


/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include <string.h>
#include "comdef.h"
#include "ska.h"
#include "customer.h"
#include "target.h"
#include "err.h"
#include "db.h"
#include "mc.h"
#include "mar.h"
#include "cm.h"
#include "snd.h"

#ifdef FEATURE_DS
#include "ds.h"
#include "dsi.h"
#endif

#ifdef  FEATURE_UI_DORMANT
#include "dsskt.h"
#endif

#ifdef  FEATURE_IP_AIRDATA      
#include "uisockapi.h"
#endif

#if (defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR))
#include "mccccl.h"
#endif /* defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */

#ifdef FEATURE_DS_IS2000
#include "rxc.h"
#include "txc.h"
#endif /* FEATURE_DS_IS2000 */


/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#ifdef FEATURE_IS95B_INC_CALL_FWD_ENH
#define sCALL_FORWARD_TIMEOUT           3000  // 3 seconds timeout for call fail 
#endif
#define sMAX_RETRIES                    2     // max # of retry

#define sONE_SECOND                     1000
#define sTWO_SECOND                     2000
#define sTHREE_SECOND                   3000
#define sDTMF_INTERVAL_SECOND           400

#define sTPAUSE_BURST_TIME              5    /* T pause burst time 5secs */
#define sMSG_BOX_DISP_TIME              5    /* 5초 */

#define sMAX_CALL_TIME_LENGTH           10

/*
** ***************************************************************************
** display coordinates
** ***************************************************************************
*/
#define sMESSAGE_BASE                   1
#define sALERT_INFO_YPOS_MARGIN         4

/* 전화 왔습니다. */
#define sALERT_MESSAGE_XPOS             4
#define sALERT_MESSAGE_YPOS             sMESSAGE_BASE

/* 각종 Call 상태에서 디스플레이 용 */
#define sUSER_AREA_HEIGHT               (SKY_SCREEN_HEIGHT - 6)// - (SKY_ANNUNCIATORS_HEIGHT*2))

#define sALERT_INFO_FIRST_XPOS          0
#define sALERT_INFO_FIRST_YPOS          (sUSER_AREA_HEIGHT - (EQS_CharHeight('A')*6) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_SECOND_XPOS         sALERT_INFO_FIRST_XPOS
#define sALERT_INFO_SECOND_YPOS         (sUSER_AREA_HEIGHT - (EQS_CharHeight('A')*5) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_THIRD_XPOS          sALERT_INFO_FIRST_XPOS
#define sALERT_INFO_THIRD_YPOS          (sUSER_AREA_HEIGHT - (EQS_CharHeight('A')*4) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_FOURTH_XPOS         sALERT_INFO_FIRST_XPOS
#define sALERT_INFO_FOURTH_YPOS         (sUSER_AREA_HEIGHT - (EQS_CharHeight('A')*3) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_FIFTH_XPOS          sALERT_INFO_FIRST_XPOS
#define sALERT_INFO_FIFTH_YPOS          (sUSER_AREA_HEIGHT - (EQS_CharHeight('A')*2) + sALERT_INFO_YPOS_MARGIN)

/*
** Alert을 통해 온 Call의 경우는 통화 중 상태에서 착신될 때 
** 날아온 정보를 그대로 보여주어야 한다.
** 이때 Alert에서 사용되었던 좌표들은 command line의 추가로
** 인해 사용할 수 없다.
*/
#define sALERT_INFO_FIRST_XPOS_CML      sALERT_INFO_FIRST_XPOS
#define sALERT_INFO_FIRST_YPOS_CML      (sALERT_INFO_FIRST_YPOS - (SKY_COMMANDLINE_HEIGHT+2) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_SECOND_XPOS_CML     sALERT_INFO_FIRST_XPOS_CML
#define sALERT_INFO_SECOND_YPOS_CML     (sALERT_INFO_SECOND_YPOS - (SKY_COMMANDLINE_HEIGHT+2) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_THIRD_XPOS_CML      sALERT_INFO_FIRST_XPOS_CML
#define sALERT_INFO_THIRD_YPOS_CML      (sALERT_INFO_THIRD_YPOS - (SKY_COMMANDLINE_HEIGHT+2) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_FOURTH_XPOS_CML     sALERT_INFO_FIRST_XPOS_CML
#define sALERT_INFO_FOURTH_YPOS_CML     (sALERT_INFO_FOURTH_YPOS - (SKY_COMMANDLINE_HEIGHT+2) + sALERT_INFO_YPOS_MARGIN)

#define sALERT_INFO_FIFTH_XPOS_CML      sALERT_INFO_FIRST_XPOS_CML
#define sALERT_INFO_FIFTH_YPOS_CML      (sALERT_INFO_FIFTH_YPOS  - (SKY_COMMANDLINE_HEIGHT+2) + sALERT_INFO_YPOS_MARGIN)

/* Origination 중에 사용되는 Definition들 */
#define sORIGINATION_PHONE_OPEN_XPOS    13
#define sORIGINATION_PHONE_OPEN_YPOS    26

#define sORIGINATION_ANIMATION_XPOS     50//5
#define sORIGINATION_ANIMATION_YPOS     32//7
#define sORIGINATION_MESSAGE_XPOS       82//84

#define sORIGINATION_IMAGE_XPOS         (DIAL_DIGIT_POS_X - 9)//3
#define sORIGINATION_IMAGE_YPOS         (DIAL_DIGIT_POS_Y - 5)

/* Origination Call에서 사용되는 Definition들 */
#define sCURRENT_TIME_XPOS              (SKY_SCREEN_WIDTH - (EQS_CharWidth('A') * 17) + 4)//22//23
#define sCURRENT_TIME_YPOS              sMESSAGE_BASE

#define sCALL_TIME_XPOS                 (SKY_SCREEN_WIDTH - (EQS_CharWidth('A') * 9) + 4)//70//72
#define sCALL_TIME_YPOS                 sMESSAGE_BASE + EQS_CharHeight('A')//15

#define sORIGINATION_TIME_XPOS          sCALL_TIME_XPOS
#define sORIGINATION_TIME_YPOS          sCALL_TIME_YPOS

#define sORIGINATION_NUMBER_XPOS        6
#define sORIGINATION_NUMBER_YPOS        50

#define sWHISPER_XPOS                   (sCALL_TIME_XPOS - (EQS_CharWidth('A') * 4))
#define sWHISPER_YPOS                   sORIGINATION_TIME_YPOS

#define sORIGINATION_NAME_XPOS          2
#define sORIGINATION_NAME_YPOS          (DIAL_DIGIT_POS_Y + 46)
/* 46은 General Pannel의 그림높이이다. */

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagCALL_INFO_T       CALL_INFO_T;
typedef struct tagBURST_STRING_T    BURST_STRING_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum tagPAUSED_MODE_E
{
    PM_NONE = 0,
    PM_PPAUSE,
    PM_TPAUSE
};
typedef enum tagPAUSED_MODE_E       PAUSED_MODE_E;

enum tagDISPLAY_NAME_E
{
    DN_CALLER_ID,
    DN_CURRENT_CALL,
    DN_CALL_WAITING_CALL
};
typedef enum tagDISPLAY_NAME_E      DISPLAY_NAME_E;

enum tagCALL_NAME_E
{
    CN_CNAP_NAME = 0,
    CN_PHONEBOOK_NAME,
    CN_NO_NAME
};
typedef enum tagCALL_NAME_E         CALL_NAME_E;

enum tagCALL_NUMBER_E
{
    CN_CNIP,
    CN_CDNI,
    CN_CNIR
};
typedef enum tagCALL_NUMBER_E       CALL_NUMBER_E;

enum tagCALL_DISPLAY_E
{
    CD_ALERT,
    CD_INPROG,
    CD_CALL_WAITING_CALL    
};
typedef enum tagCALL_DISPLAY_E      CALL_DISPLAY_E;

/*
** ***************************************************************************
** DMSS call code -- import from dmss source code
** ***************************************************************************
*/
struct tagCALL_INFO_T {
    byte                  ui_try_cnt;      // # of try, for UI originated call only
    boolean               secret;          // the # is secret? outgoing call only
    cm_num_s_type         num;             // origination/incom #, may include pause 
    cm_num_s_type         called_party_num;// number plus
    cm_alpha_s_type       alpha;           // alpha tag, phonebook match or caller ID
    cm_alpha_s_type       alpha_cnap;      // alpha tag, From CNAP
    dword                 connect_time;    // time the call is connected
    CM_CALL_INFO_T        cm_info;         // information from call manager
    cm_inband_info_s_type inband_info;     // inband info
    byte                  bPI;             // presentation indicator
};

// will be replaced by dial digit in the future
struct tagBURST_STRING_T {
  cm_num_s_type       num;         // # to be bursted 
  int1                pause_pos;   // current pause position, -1 at init
  boolean             secret;      // is the # secret?
};

/*
** ***************************************************************************
** controls used in applets
** ***************************************************************************
*/
enum tagCALL_WAIT_CALL_E
{
    CWC_CURRENT_TIME = 0,      /* 통화 중        */
    CWC_TIME,                  /* 통화 시간      */
    CWC_CNAP,                  /* 홍길동         */
    CWC_CNIP,                  /* 011-XXXX-YYYY  */
    CWC_TWO_NUMBER_MESSAGE,    /* 수신번호       */
    CWC_TWO_NUMBER_EDITOR      /* 011-XXXX-XXXX  */
};

enum tagCALL_FORWARD_E
{
    CF_TITLE = 0,
    CF_PHONE_OPEN_IMAGE,
    CF_IMAGE,
    CF_NUMEDIT
};

/*
** Origination Call일 경우에 사용되는 열거형
** T-pause, P-pause일 경우도 사용된다.
*/
enum tagINPROG_FROM_ORIGINATION_E
{
    IFO_CURRENT_TIME = 0,
    IFO_CALL_TIME,
    IFO_PHONE_OPEN_IMAGE,
    IFO_GENERAL_FRAME,
    IFO_PHONE_NUMBER
};

enum tagIN_PROG_E
{
    IP_TITLE = 0,
    IP_TIME,
    IP_PHONENUMBER,
    IP_WHISPER
};

/* 송화음차단에서 사용되는 열거형 */
enum tagVOICE_MUTE_E
{
    VM_CURRENT_TIME,
    VM_CALL_TIME,
    VM_PHONE_OPEN_IMAGE,
    VM_CURRENT_MODE,
    VM_GENERAL_FRAME,
    VM_PHONE_NUMBER
};

/*
** Alerted을 받아서 통화중으로 연결될 때 
** 사용되는 열거형 (현재시각은 표시 가능)
*/
enum tagINPROG_DISPLAY_CURRENT_TIME_E
{
    IDCT_CURRENT_TIME = 0,
    IDCT_CALL_TIME 
};

enum tagTOGGLE_MUTE_E
{
    TM_TITLE = 0,
    TM_TIME
};

enum tagALERT_STATE_E
{
    AS_TITLE = 0,
    AS_CNAP,                  /* 홍길동         */
    AS_CNIP,                  /* 011-XXXX-YYYY  */
    AS_TWO_NUMBER_MESSAGE,    /* 수신번호       */
    AS_TWO_NUMBER_EDITOR,     /* 011-XXXX-XXXX  */
    AS_MAX
};

enum tagADJUST_VOLUME_E    /* 통화중 볼륨 조절*/
{
    AV_TITLE = 0,           /* Title         */
    AV_VOLUMEBAR,           /* Volume Bar    */
    AV_VOLUMELVL,           /* Volume Number */
    AV_MAX
};


/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
/* jrkwon 2001.07.18 
** save the last key event in CallEndDisp applet.
*/
SKY_EVENT_E g_LastCallKeyEvent = SE_ERROR;

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
LOCAL PAUSED_MODE_E l_PausedMode;
LOCAL BOOL l_fCallOriginated = FALSE;
LOCAL BOOL l_fAutoAnswerFromAlert = FALSE;
LOCAL BOOL l_fWhisperMode = FALSE;
LOCAL BOOL l_fKeyDownInAlert = FALSE;
LOCAL BOOL l_fCallFromAlert = FALSE;
LOCAL BOOL l_fCallEndedByUser = FALSE;
LOCAL MISSED_CALL l_MissedCall;                 /* missed call */
LOCAL CALL_INFO_T  l_CallInfo;
LOCAL BURST_STRING_T l_BurstString;
// indicates a foward burst DTMF tones are being played.
LOCAL BOOL l_fForwardBurstDTMFOn = FALSE;
LOCAL BOOL l_fAutoAnswerAlready = FALSE;
LOCAL BYTE l_szCallTime[sMAX_CALL_TIME_LENGTH];
LOCAL WORD l_wRetryTimeLeft;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL BOOL SKY_API sIsDataCall(void);
LOCAL BOOL SKY_API sIsData(void);
#ifdef FEATURE_UI_DORMANT
LOCAL BOOL SKY_API sIsDormantEnable(void);
#endif
LOCAL void SKY_API sSaveOriginationHistory(void);
LOCAL void SKY_API sUpdateOriginationCallInfo(void);
LOCAL void sUpdateAlertedCallInfo(void);
LOCAL void SKY_API sUpdateOriginatedDataCallInfo(void);
LOCAL void SKY_API sUpdateReceivedDataCallInfo(void);
LOCAL void SKY_API sEndConversation(void);
LOCAL void SKY_API sClearFlashBuffer(void);
LOCAL void SKY_API sAdvancePauseInBurst(void);
LOCAL void SKY_API sInitConversation(void);
LOCAL void SKY_API sOriginatedModeEntry(void);
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
LOCAL BOOL SKY_API sIsAnswerAllowed(const CM_CALL_INFO_T* pCallInfo);
#endif
LOCAL BOOL SKY_API sIsAutoAnswerReady(void);
LOCAL void SKY_API sInProgModeEntry(void);
LOCAL BOOL SKY_API sIsInAlert(void);
LOCAL void SKY_API sInitBurstString(const BYTE* pBuf, UINT nLen, BOOL fSecret);
LOCAL UINT SKY_API sBurstAndAdvancePause(void);
LOCAL BOOL SKY_API sAnyBurstLeft(void);


LOCAL void SKY_API sInProgExit(void);
LOCAL void SKY_API sCallExitFunc(CALL_MODE_E CallMode);
LOCAL void SKY_API sCallEndSetup(void);
LOCAL void SKY_API sExitCallState(void);
LOCAL void sSetCallTimeString(BYTE abCallTimeString[]);
LOCAL void SKY_API sDisplayNameInPhoneBook(DISPLAY_NAME_E DispName);
LOCAL void SKY_API sDisplayNameInCNAP(DISPLAY_NAME_E DispName);
LOCAL void SKY_API sCreateNameAndNoPhoneNumber(CALL_DISPLAY_E DISPLAY_MODE, 
                                 CALL_NAME_E NAME_MODE, 
                                 BYTE pszNameBuf[]);
LOCAL void SKY_API sCreateNameAnd1PhoneNumber(CALL_DISPLAY_E DISPLAY_MODE,
                                CALL_NAME_E    NAME_MODE,   
                                BYTE           pszNameBuf[],
                                CALL_NUMBER_E NUMBER_MODE, 
                                BYTE pszNumberBuf[]);
LOCAL void SKY_API sCreateNameAnd2PhoneNumber(CALL_DISPLAY_E  DISPLAY_MODE,
                                CALL_NAME_E     NAME_MODE,  
                                BYTE            pszNameBuf[],
                                CALL_NUMBER_E   NUMBER_MODE,
                                BYTE            pszCNIPNumberBuf[],  
                                BYTE            pszCDNIPNumberBuf[]);

LOCAL void SKY_API sSKA_CallEndDisp(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_WaitRetry(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_OrigAttempt(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_Originated(SKY_EVENT_T *pEvent);
LOCAL void SKY_API sSKA_Paused(SKY_EVENT_T *pEvent);
#ifdef FEATURE_CHARACTER_BELL
LOCAL void sDisplayCharacbellImage(int index);
#endif // FEATURE_CHARACTER_BELL

/* 
** ---------------------------------------------------------------------------
** ---------------------------------------------------------------------------
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

#define sGENERAL_FRAME_HEIGHT            46
#define sGENERAL_FRAME_WIDTH             120

#define sGENERAL_FRAME_XLEFT             sORIGINATION_IMAGE_XPOS
#define sGENERAL_FRAME_YTOP              sORIGINATION_IMAGE_YPOS

#define sGENERAL_FRAME_XRIGHT            (sGENERAL_FRAME_XLEFT + sGENERAL_FRAME_WIDTH)
#define sGENERAL_FRAME_YBOTTOM           (sORIGINATION_IMAGE_YPOS + sGENERAL_FRAME_HEIGHT)


/* 발신화면에서 타이틀 높이와 Annunciator의 높이가 틀릴 경우에  */
/* 발신화면과 통화 중 화면이 상하로 움직이는 것처럼 보인다 이를 */
/* 보정할 때 사용된다.                                          */
#define sDIALING_DISPLAY_OFFSET_Y           (EQS_SWINDOW_HEADER_HEIGHT - EQS_SWINDOW_ANNUNCIATOR_HEIGHT)

#define sGENERAL_FRAME_YTOP_IN_DIALING      (sGENERAL_FRAME_YTOP - sDIALING_DISPLAY_OFFSET_Y)
#define sGENERAL_FRAME_YBOTTOM_IN_DIALING   (sGENERAL_FRAME_YTOP_IN_DIALING + sGENERAL_FRAME_HEIGHT)



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
//VOID SKY_API sDrawGeneralFrameInDialing(VOID)
//{
//
//       EQS_Rectangle(sGENERAL_FRAME_XLEFT,   sGENERAL_FRAME_YTOP_IN_DIALING,
//                     sGENERAL_FRAME_XRIGHT,  sGENERAL_FRAME_YBOTTOM_IN_DIALING);
//
//       //EQS_FillRectangle(sGENERAL_FRAME_XLEFT, sGENERAL_FRAME_YTOP_IN_DIALING,
//       //                  sGENERAL_FRAME_XRIGHT, sGENERAL_FRAME_YBOTTOM_IN_DIALING,
//       //                  NUMBER_PANNEL_COLOR);
//
//}



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
//void SKY_API DrawGeneralFrame(void)
//{
//
//   EQS_Rectangle(sGENERAL_FRAME_XLEFT,  sGENERAL_FRAME_YTOP,
//                 sGENERAL_FRAME_XRIGHT, sGENERAL_FRAME_YBOTTOM);
//
//   //EQS_FillRectangle(sGENERAL_FRAME_XLEFT, sGENERAL_FRAME_YTOP,
//   //                  sGENERAL_FRAME_XRIGHT, sGENERAL_FRAME_YBOTTOM,
//   //                  NUMBER_PANNEL_COLOR);
//}



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

BOOL SKY_API IsCallTypeSMS(void)
{
    if(l_CallInfo.cm_info.call_type  == CM_CALL_TYPE_SMS)
    {
        return TRUE;
    }
    else 
    {        
        return FALSE;
    }
}    




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     voice call인지 아닌지를 판별하는 함수이다.
**     자료구조 call_info가 call과 관련된 이벤트들이 날아올 때 갱신되므로 
**     반드시 SE_CALL_INCOMING, SE_CALL_ENDED 등과 같이 Call과 관련된 이벤트와 
**     연동해서 써야 한다. 
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
BOOL SKY_API SKY_IsVoiceCall(void)
{
    if(l_CallInfo.cm_info.call_type == CM_CALL_TYPE_VOICE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     Call이 End 되었을 때 End Reason이 무엇인지 return한다. 
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
cm_call_end_e_type SKY_API SKY_GetCallEndStatus(void)
{    
    return(l_CallInfo.cm_info.end_status);    
}










/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     데이터 Call인지 아닌 지를 판별 
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
BOOL SKY_API IsData2DisplayInCallWaitingCall(void)
{
    CNI_T*        pCallerID;

    pCallerID = SKY_GetCallerID();

    if(pCallerID->aName[0] != '\0') 
    {
        /* CNAP정보가 있는 경우 */
        return TRUE;
    }
    else if(pCallerID->CalledParty.aBuf[0] != '\0')
    {
        /* 보여줄 투넘버 서비스 번호가 있는 경우 */
        return TRUE;
    }
    else if(pCallerID->Num.aBuf[0] != '\0')
    {
        /* CNIP번호가 날아온 경우 */
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL SKY_API IsWhisperMode(void)
{
    return l_fWhisperMode;
}

LOCAL BOOL SKY_API sIsDataCall(void)
{
#ifdef FEATURE_DS
    if((l_CallInfo.cm_info.call_state != CM_CALL_STATE_IDLE) &&
       (l_CallInfo.cm_info.call_type  == CM_CALL_TYPE_DATA)  &&
       (!SKY_IS_IN_WAP()))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else //FEATURE_DS
    ASSERT(l_CallInfo.cm_info.call_type != CM_CALL_TYPE_DATA);
    return FALSE;
#endif //FEATURE_DS
}


LOCAL BOOL SKY_API sIsData(void)
{
#ifdef FEATURE_DS
    if((l_CallInfo.cm_info.call_type  == CM_CALL_TYPE_DATA)  &&
       (!SKY_IS_IN_WAP()))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else //FEATURE_DS
    ASSERT(l_CallInfo.cm_info.call_type != CM_CALL_TYPE_DATA);
    return FALSE;
#endif //FEATURE_DS
}


BOOL SKY_API IsInVoiceConversation(void)
{
    return (ISVOICE(l_CallInfo.cm_info.call_type) && ui_is_in_conversation());
}

BOOL SKY_API SKY_IsInCall(void)
{
  return (l_CallInfo.cm_info.call_state != CM_CALL_STATE_IDLE
#ifdef FEATURE_OTASP_OTAPA 
            && !ui_is_in_otapa_call()
#endif        
          );
}

BOOL SKY_API SKY_IsInTraffic(void)
{
    if((GET_EVENT_PARAM()->call_event).call_info.call_state != CM_CALL_STATE_IDLE)
    {
        return TRUE;
    }
    else if (SKY_IS_SOCK_CONNECT())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL SKY_API IsDataCall(void)// CurrentCallIsData(void)
{
    return sIsDataCall();
}

#ifdef FEATURE_UI_DORMANT
LOCAL BOOL SKY_API sIsDormantEnable(void)
{
    /* boolean dsDormantEnabled in dsskt */
    return (sIsDataCall() && dsDormantEnabled); 
}


LOCAL BOOL SKY_API sIsInDormantEnable(void)
{
    extern boolean dsDormantEnabled;

    return (sIsDataCall() && dsDormantEnabled);
}
#endif

BYTE SKY_API GetCallSignalType(void)
{
    return(l_CallInfo.cm_info.signal.signal_type);
}

BYTE SKY_API GetCallSignal(void)
{
    return(l_CallInfo.cm_info.signal.signal);
}

BYTE SKY_API GetCallSignalAlertPitch(void)
{
    return(l_CallInfo.cm_info.signal.alert_pitch);
}

BOOL SKY_API IsEarpieceEnable(void)
{
    if (sIsDataCall() || 
      SKY_IS_HANDS_FREE() || SKY_IS_EAR_JACK_ON()) {    
        return FALSE;
    }
    return TRUE;
}

/*
** ***************************************************************************
** DMSS call code -- import from dmss source code
** ***************************************************************************
*/
void ui_update_cm_call_info(const CM_CALL_INFO_T* cm_call_info_ptr)
{
    l_CallInfo.cm_info = *cm_call_info_ptr;
}

void ui_update_cm_inband_info(const cm_inband_info_s_type* cm_inband_info_ptr)
{
    l_CallInfo.inband_info = *cm_inband_info_ptr;
}

boolean ui_curr_call_allow_sound(void)
{
    // we can play signal for voice call (voice+e911) and data calls
    return  (ISVOICE(l_CallInfo.cm_info.call_type) || 
           ISSMS(l_CallInfo.cm_info.call_type) ||
           sIsData() );
}

boolean ui_curr_call_is_sms(void)
{
#ifdef FEATURE_UASMS
    if (l_CallInfo.cm_info.call_state != CM_CALL_STATE_IDLE &&
      l_CallInfo.cm_info.call_type  == CM_CALL_TYPE_SMS    ) 
    {
        return ((boolean)TRUE);
    }
    else 
        return FALSE;
#else

    ASSERT(l_CallInfo.cm_info.call_type != CM_CALL_TYPE_SMS);
    return ((boolean)FALSE);
#endif
}

#ifdef FEATURE_OTASP_OTAPA 
boolean ui_is_in_otapa_call(void)
{
  return ((l_CallInfo.cm_info.call_state != CM_CALL_STATE_IDLE) &&
          (l_CallInfo.cm_info.call_type == CM_CALL_TYPE_OTAPA) );
}
#endif /* FEATURE_OTASP_OTAPA  */

boolean ui_is_in_conversation(void)
{
  return (l_CallInfo.cm_info.call_state == CM_CALL_STATE_CONV
#ifdef FEATURE_OTASP_OTAPA 
            && !ui_is_in_otapa_call()
#endif        
          );
}

word ui_curr_call_so(void)
{
    return (l_CallInfo.cm_info.srv_opt);
}

static snd_tone_type digit_to_sound(
  byte ascii_digit 
)
{
  if (ascii_digit == '*') {
    return (snd_tone_type)SND_STAR;
  } else if (ascii_digit == '#') {
    return (snd_tone_type)SND_POUND;
  } else {
    ASSERT(ascii_digit >= '0' && ascii_digit <= '9');
    return (snd_tone_type)(ascii_digit-'0'+ SND_0);
  }
}

void ui_stop_fwd_burst_dtmf(void)
{
    if (l_fForwardBurstDTMFOn) {
        SKY_StopMultiTone();
        l_fForwardBurstDTMFOn = FALSE;
    }
}

/*
** ***************************************************************************
** END -- DMSS call code -- import from dmss source code 
** ***************************************************************************
*/

LOCAL void SKY_API sSaveOriginationHistory(void)
{
    AddCallsList(CT_CALL_TO, l_CallInfo.num.buf, l_CallInfo.num.len, l_CallInfo.alpha.buf);
}

/*
** ***************************************************************************
** 통화시간 
** ***************************************************************************
*/
#define sROLL_CALL_TIME (DWORD)600000000 /* Roll over call time at 1000 mins*/
#define sROLL_CALL_CNT  (DWORD)100000    /* Roll over call count*/

LOCAL void SKY_API sUpdateOriginationCallInfo(void)
{
    DWORD dwCallTime;    
    dwCallTime = clk_uptime() - l_CallInfo.connect_time;

    /* 최종통화시간 */
    SKY_SET_VOICE_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_VOICE_CUMULATION_USING_TIME(((SKY_GET_VOICE_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_VOICE_TOTAL_USING_TIME(((SKY_GET_VOICE_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}


LOCAL void sUpdateAlertedCallInfo(void)
{
    DWORD dwCallTime;
    dwCallTime = clk_uptime() - l_CallInfo.connect_time;

    /* 최종통화시간 */
    SKY_SET_VOICE_RECEIVEDCALL_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(
    ((SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(
    ((SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}

LOCAL void SKY_API sUpdateOriginatedDataCallInfo(void)
{
    DWORD dwCallTime;
    dwCallTime = clk_uptime() - l_CallInfo.connect_time;

    /* 최종통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(
    ((SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(
    ((SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}

LOCAL void SKY_API sUpdateReceivedDataCallInfo(void)
{
    DWORD dwCallTime;
    dwCallTime = clk_uptime() - l_CallInfo.connect_time;

    /* 최종통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(
    ((SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(
    ((SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}

void SKY_API UpdateOriginatedDataCallInfo(DWORD dwCallTime)
{
    /* 최종통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(
    ((SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(
    ((SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}

void SKY_API UpdateReceivedDataCallInfo(DWORD dwCallTime)
{ 
    /* 최종통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_LAST_USING_TIME((dwCallTime % sROLL_CALL_TIME));

    /* 누적통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(
    ((SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME()+dwCallTime) % sROLL_CALL_TIME));

    /* 총통화시간 */
    SKY_SET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(
    ((SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME()+dwCallTime)%sROLL_CALL_TIME));
}



LOCAL void SKY_API sEndConversation(void)
{  
    /* 통화종료화면에서 보여줄 Call End Time String을 Set해 준다 */
    sSetCallTimeString(l_szCallTime);

    l_fWhisperMode = FALSE;  
    SKY_SetSoundDevice(SND_MUTE_MUTED, SND_MUTE_MUTED, TRUE);

    SKY_SetStereoMute(FALSE);   // unmute the car stereo
    SKY_SET_TRANSMIT_DTMF(FALSE);   //ui.txdtmf = FALSE;

    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());        //white 2001.06.27
    SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());  

#ifdef FEATURE_UI_PRIVACY_ALERT
        #error code not present
#endif  

    if(l_fCallOriginated && ISVOICE(l_CallInfo.cm_info.call_type))  
    {
        /* 음성통화발신 */
        sUpdateOriginationCallInfo();
        l_fCallOriginated = FALSE;
    }
    else if(l_fCallFromAlert && ISVOICE(l_CallInfo.cm_info.call_type))
    {
        /* 음성통화착신 */
        sUpdateAlertedCallInfo();
    }
    else if(l_fCallOriginated && sIsData())
    {
        /* 데이터통신발신 */
        sUpdateOriginatedDataCallInfo();
        l_fCallOriginated = FALSE;
    }
    else if(l_fCallFromAlert && sIsData())
    {
        /* 데이터통신착신 */
        sUpdateReceivedDataCallInfo();
    }       
}

LOCAL void SKY_API sClearFlashBuffer(void)
{
    if(l_PausedMode == PM_NONE)   /* Paused Mode가 아닐 경우에는 Clear한다 */
    {
        SET_NUMEDIT_POS(0);
    }
}

LOCAL void SKY_API sAdvancePauseInBurst(void)
{
    int1 i = l_BurstString.pause_pos;

    if (l_BurstString.pause_pos == l_BurstString.num.len) 
    {
        // it has already reached the end of burst string
        return;
    }

    // make sure the pause position is valid 
    ASSERT( (i==-1) ||  ISPAUSE(l_BurstString.num.buf[i]) );

    do 
    {
        i++;
    } while ( (i<l_BurstString.num.len) && !ISPAUSE(l_BurstString.num.buf[i]) );

    ASSERT(i>=0 && i<= l_BurstString.num.len);

    l_BurstString.pause_pos = i;
}


LOCAL void SKY_API sInitConversation(void)
{
    SKY_SET_ROAM_CALL(SKY_GET_PHONE_INFO_ROAM_STATUS() != CM_ROAM_STATUS_OFF);  
    
    // set up sound path
    if( ISVOICE(l_CallInfo.cm_info.call_type) ) 
    {
        SKY_SET_MUTE(FALSE);
        SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
        SKY_SET_TRANSMIT_DTMF(TRUE);
    }

    SKY_StopSound();
    sClearFlashBuffer();
    l_CallInfo.connect_time = clk_uptime();
}


LOCAL void SKY_API sInProgModeEntry(void)
{
    if (ISTEST(l_CallInfo.cm_info.call_type) == FALSE) 
    {
        SKY_SET_TRANSMIT_DTMF(TRUE);
    } 
    SKY_AcceptKey(TRUE);
    SKY_SET_KEY_TONE(TRUE);
}

/* wrapper function of sInProgModeEntry used in ska_dsvc */
void SKY_API InProgModeEntry(void)
{
    sInProgModeEntry();
}

LOCAL void SKY_API sOriginatedModeEntry(void)
{
    SKY_AcceptKey(TRUE);

#ifdef FEATURE_DS
    if (sIsDataCall()) 
    {
        l_fCallOriginated = TRUE;
    } 
    else 
#endif /* FEATURE_DS */
    {      
        if(l_CallInfo.secret == FALSE) /* Public Group에 속할 경우, 착/발신 이력에 남긴다. */
        {
            if(ISVOICE(l_CallInfo.cm_info.call_type))
            {
                sSaveOriginationHistory();        /* save origination history for voice call */
            }
        }
        l_fCallOriginated = TRUE;
    }
}


#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS

LOCAL BOOL sIsAnswerAllowed(const CM_CALL_INFO_T* pCallInfo)
{
    return (!pCallInfo->alert_ans_bypass);
}

#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */


LOCAL BOOL SKY_API sIsAutoAnswerReady(void)
{
    // if we are getting incoming voice call AND
    // auto answer is enabled AND
    // earpiece or flip is up AND
    // we've got enough ring to answer,
    // then autoanswer the call 
    return (l_CallInfo.cm_info.call_state == CM_CALL_STATE_INCOM 
        && ISVOICE(l_CallInfo.cm_info.call_type) && SKY_IS_AUTO_ANSWER() 
        && SKY_IS_PHONE_OPEN()) ? TRUE : FALSE;
}


LOCAL BOOL SKY_API sIsInAlert(void)
{
    return (l_CallInfo.cm_info.call_state == CM_CALL_STATE_INCOM
#ifdef FEATURE_OTASP_OTAPA 
            && !ui_is_in_otapa_call()
#endif        
            );
}


LOCAL void SKY_API sInitBurstString(const BYTE* pBuf, UINT nLen, BOOL fSecret)
{
    ASSERT(nLen < sizeof(l_BurstString.num.buf));

    memcpy(l_BurstString.num.buf, pBuf, nLen);
    l_BurstString.num.len   = nLen;
    l_BurstString.secret    = fSecret;
    l_BurstString.pause_pos = -1;
}


LOCAL void SKY_API sFormatBurst(const BYTE* digits, UINT digit_cnt)
{
#ifdef FEATURE_NEW_SOUND_API
    static snd_compact_tone_type snds[NV_MAX_DIAL_DIGITS*2+1];
#else
    static snd_type snds[NV_MAX_DIAL_DIGITS*2+1];
#endif /* FEATURE_NEW_SOUND_API */
    word tone_duration, silence_duration;
    unsigned int j,l;
    byte on_length = CAI_DTMF_ON_150, off_length= CAI_DTMF_OFF_100;

    j = 0;

    /* Setup up the durations of the tones and silences */
    /* AMPS durations */
 
    if ( !SKY_IS_DIGITAL() )  //if(!ui.digital)
    {
        /* Start with 500 ms of silence to minimize chopping tones off */
        /* with the key press tone.                                    */  
        snds[j].tone = SND_SILENCE;
#ifdef FEATURE_NEW_SOUND_API
        snds[j++].param.duration_ms = 500;
#else
        snds[j++].duration_ms = 500;
#endif /* FEATURE_NEW_SOUND_API */

        if (SKY_IS_SHORTBURST_DTMF()) // if(ui.sburst)
        {    
            /* Short Burst durations */    
            tone_duration = 200;
            silence_duration = 200;
        } else { 
            /* Long Burst durations */
            tone_duration = 500;
            silence_duration = 500;
        }
    } 
    else 
    { /* CDMA Durations */   
        if (SKY_IS_SHORTBURST_DTMF()) // if(ui.sburst)
        {
            /* Short Burst durations */      
            tone_duration = 150;
            silence_duration = 100;
            on_length = CAI_DTMF_ON_150 ;
            off_length = CAI_DTMF_OFF_100 ;
        } 
        else 
        {
            /* Long Burst durations */
            tone_duration = 350;
            silence_duration = 200;
            on_length = CAI_DTMF_ON_350 ;
            off_length = CAI_DTMF_OFF_200 ;
        }
    }

    /* Setup the number for local generation.  If this is AMPS */
    /* it's also sent OTA.                                     */
    for( l=0; l<digit_cnt; l++ ) { /* set up number */
        snds[j].tone = digit_to_sound( digits[l] );
#ifdef FEATURE_NEW_SOUND_API
        snds[j++].param.duration_ms = tone_duration;
        snds[j].tone = SND_SILENCE;  /* silent */
        snds[j++].param.duration_ms = silence_duration;
#else
        snds[j++].duration_ms = tone_duration;
        snds[j].tone = SND_SILENCE;  /* silent */
        snds[j++].duration_ms = silence_duration;
#endif /* FEATURE_NEW_SOUND_API */
    }

    /* Cancel the last SND_SILENCE, by replacing it with
    ** SND_STOP. It is to make sure that the speaker
    ** is not muted after the DTMF burst.
    ** Note that for the above for loop, loop
    ** counter j increments at least twice. It is okay
    ** to decrement j once and put SND_STOP into the
    ** array.
    */
    j--;

    snds[j].tone = SND_STOP;
#ifdef FEATURE_NEW_SOUND_API
    snds[j].param.duration_ms = 0;
#else
    snds[j].duration_ms = 0;
#endif /* FEATURE_NEW_SOUND_API */

    /* Finish setting the digital command for OTA */
    if( SKY_IS_DIGITAL() ) //if( ui.digital )
    {  
        if (ui_burst_dtmf(on_length, off_length, (byte) digit_cnt, digits)) 
        {
            /* Local sound generation */
            SKY_PlayMultiTone( FALSE, UI_BEEP_SND, j, snds, MOD_KEY_BEEP, FALSE);
        }
    } 
    else 
    {              /* analog */
        /* Local and OTA sound generation */
        SKY_PlayMultiTone( TRUE, UI_BEEP_SND, j, snds, MOD_KEY_BEEP, FALSE ); 
    }
}

LOCAL UINT SKY_API sBurstAndAdvancePause(void)
{
   static byte digits[NV_MAX_DIAL_DIGITS];
   int start_pos;
   int end_pos; 
   int i;
   unsigned int digit_cnt;   // # of burst

   start_pos = l_BurstString.pause_pos+1;
   sAdvancePauseInBurst();
   end_pos = l_BurstString.pause_pos;

   digit_cnt = 0;
   for (i=start_pos; i<end_pos; i++) 
   {
     if (l_BurstString.num.buf[i] != UI_HYP_CHR) 
     {
       digits[digit_cnt++] = l_BurstString.num.buf[i];
     }
   }

   if (digit_cnt != 0) 
   {
      sFormatBurst(digits, digit_cnt);
   }
   return digit_cnt;
}


LOCAL BOOL SKY_API sAnyBurstLeft(void)
{
    int i = l_BurstString.pause_pos;

    if (i == -1) {
        return (l_BurstString.num.len != 0);
    } 
    else 
    {
        ASSERT((i == l_BurstString.num.len) || ISPAUSE(l_BurstString.num.buf[i]) );
        return ( i != l_BurstString.num.len);
    }
}

void SKY_API CallDefaultEventProc(SKY_EVENT_T* pEvent, CALL_MODE_E  CallMode)
{
    HCONTROL hControl;

    switch (pEvent->EventID) 
    {
    case SE_KEY_CLEAR:
        break;

    case SE_KEY_POWER_OFF ://HS_PWR_OFF_K:
        if (ui_is_in_conversation()) 
        {
            sEndConversation();
        }
        sCallExitFunc(CallMode);
        EQS_PASS_CURRENT_EVENT();
        break;

    case SE_IO_TO_IDLE:
    case SE_SYS_TO_IDLE:
        break;

    case SE_KEY_END :  //HS_END_K:
        if (SKY_IsInCall())
        {
            sEndConversation();
            SKY_EndCall();
            l_fCallEndedByUser = TRUE;
        }
        sCallExitFunc(CallMode);
        break;

    case SE_SYS_OFFLINE ://UI_OFFLINE_F:
        if (ui_is_in_conversation()) 
        {
            sEndConversation();
        }
        sCallExitFunc(CallMode);
        EQS_END_TO(EQS_POP_TO_ROOT); 
        break;

    case SE_KEY_EAR_LONG_CLICK:
        if (ui_is_in_conversation())
        {
           l_fCallEndedByUser = TRUE;
           SKY_EndCall();
        }
        break;

    case SE_KEY_PHONE_CLOSE:
        if(SKY_IS_EAR_JACK_OFF() && SKY_IsInCall())
        {
            l_fCallEndedByUser = TRUE;
            SKY_EndCall();
        }
        break;

#ifdef FEATURE_UI_DORMANT
    case SE_DATA_DORM_F:
        if (ui_is_in_conversation() && sIsDormantEnable()) 
        {
            SKY_SET_DORMANT(TRUE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            SKY_EndCall();
            EndCall();
        }
         break;

    case SE_DATA_IDLE_F:
        if (ui_is_in_conversation()) 
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            sEndConversation();
            sInProgExit();
            // set up display etc, may go to CALL_END_DISP mode or exit call state
            // l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;      
            sCallEndSetup(); //call_end_setup();
        }
        else
        {
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
          break;
#endif

    case SE_CALL_COMMAND_ERROR:
          break;

    case SE_CALL_ENDED:
      sEndConversation();
      sInProgExit();
      DataManagerEnd(pEvent);
      /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
      /* 통화종료화면이 나오지 않는 현상 방지                            */
      if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
      {
          EQS_DeleteControl(hControl);
      }
      // set up display etc, may go to CALL_END_DISP mode or exit call state
      l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;      
      sCallEndSetup(); //call_end_setup();
      if((l_fCallEndedByUser == FALSE) && ISVOICE(l_CallInfo.cm_info.call_type))
      {
            l_fCallEndedByUser = FALSE;
            SKY_PlaySoundEx( UI_CALL_EAR_SND, SND_BUSY_ALERT, 2 , 3);
      }
      else
      {
            l_fCallEndedByUser = FALSE;
      }
          break;

    case SE_CALL_DISPLAY : /* ignore this event */
          break;
    
    case SE_KEY_MANNERS:
      break;

    case SE_DATA_CALL:
        break;

    default:
          EQS_DEF_EVENT_PROC(pEvent);
        break;
    }   
}

void SKY_API IPAirDataManagerEndTime(void)
{
    sEndConversation();
}

/*=============================================================================

                
                                EQUS Section


==============================================================================*/
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
LOCAL BOOL SKY_API sIsCNIValid(void)
{
  UINT8   nLoop;
  CNI_T*  pCallerID;
  pCallerID = SKY_GetCallerID();
  
   /* Loop thru the CNI number looking for an invalid character */
  for ( nLoop=0; nLoop < pCallerID->Num.nPos; nLoop++) 
  {
    switch ( pCallerID->Num.aBuf[nLoop] ) 
    {  
      /* These are all valid for a CNI number */
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
      case '*':
      case '#':
      case '-':
        break;

      /* Any other character is invalid */
      default:
        return FALSE;
    }
  }
  return TRUE;
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
LOCAL void SKY_API sSaveCLI(BYTE bStatus)    
{ 
  BYTE       pDummy[2];
  CNI_T*     pCallerID;
  pCallerID = SKY_GetCallerID();

  if(bStatus != CT_CALLED_BY_INPROG)
  {
    l_fCallOriginated = FALSE;
  }
  pDummy[0] = '\0';

  if(pCallerID->bPI == CAI_ALLOW_PI)
  {
    /* Make sure the CNI number is valid before storing it. */
    if ((pCallerID->Num.nPos > 0) && sIsCNIValid())
    {
        AddCallsList(
                   bStatus,
                   pCallerID->Num.aBuf, 
                   pCallerID->Num.nPos,
                   pCallerID->aName
              );      
    }
    else if(pCallerID->Num.nPos == 0)
    {
        AddCallsList(
                   bStatus,
                   pCallerID->Num.aBuf, 
                   pCallerID->Num.nPos,
                   pCallerID->aName
              );
    }
  }
  else if(pCallerID->bPI == CAI_RES_PI)               
  {  
        AddCallsList(
                     bStatus,
                     pDummy, 
                     0,
                     pCallerID->aName);
  }
} /* SaveCLI() */




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
LOCAL void SKY_API sToggleMute(HCONTROL hControl)
{
  if(SKY_IS_MUTE()) 
  {                                /* mute already on */
    SKY_SET_MUTE(FALSE);
    SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);    
  } 
  else 
  {                                /* mute not on yet */
    SKY_SET_MUTE(TRUE);            /* Mute the input path */
    SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_MUTED, TRUE);
    EQC_SetStaticText(hControl, (BYTE*)SKY_GET_TOKEN(TKN_CALL_MUTE));
  }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sAllowRetry()
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
LOCAL BOOL SKY_API sAllowRetry(CM_CALL_INFO_T* sCallInfoPtr)
{

  if (ui_call_is_from_ui(sCallInfoPtr) 
          && ((sCallInfoPtr->end_status != CM_CALL_END_CLIENT_END) 
          && (sCallInfoPtr->end_status != CM_CALL_END_INCOM_CALL)) 
          && (l_CallInfo.ui_try_cnt < sMAX_RETRIES)) 
  {
    return TRUE;
  }
  else
    return FALSE;
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitWaitRetry()                          wait_retry_init
**
** Description: 
**     set wait retry time
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
LOCAL void SKY_API sInitWaitRetry(void)
{
  l_wRetryTimeLeft = 10;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sOrigAttemptExit(void)                (orig_attempt_mode_exit)
**
** Description: 
**     
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
LOCAL void  SKY_API sOrigAttemptExit(void)
{
    SKY_AcceptKey(TRUE);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sOriginatedExit(void)                       (originated_mode_exit)
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
LOCAL void SKY_API sOriginatedExit(void)
{
   SKY_SET_ORIGINATE(FALSE);

   /* 백라이트 항상 꺼짐 */
   SKY_SET_NO_OFF_BACKLIGHT(FALSE);
   SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sAlertedExit                                 (sAlertedExit)
**
** Description: h
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
LOCAL void SKY_API sAlertedExit(void)
{
  SKY_SET_ALERT(FALSE);

#ifdef FEATURE_UI_ESCALATING_RING
#error code not present
#endif

  SKY_StopSound();  // stop the ringer
  SKY_StopMusic();
  SKY_MotorStop();
  
  ClearCLI();
#ifdef FEATURE_UI_SNI
  // stop lighting SNI LED
  //(void)uiscrn_ann(HS_AN_SNI, 0);
#endif
  /* 백라이트를 사용자의 설정에 맞게 원상복구 시킨다. */
  SKY_SET_NO_OFF_BACKLIGHT(FALSE);
  SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());         // turn on backlighting //white
  SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
  SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
  //SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDEndText(LN_UPPER);
    SKY_SubLCDEndText(LN_LOWER);
    //SKY_SubLCDDefault();
#endif //FEATURE_SKY_SUBLCD
    //이것은 멜로디 칩이 켜졌을 때 착신이 되면 
    //스피커로 통화음이 들리는 "하드웨어 문제" 때문에 들어감.
    //착신때는 곧바로 멜로디 칩의 파워를 꺼줘야 함......
    //SKY_MusicPowerOff(); 함수 안에는  
    //MUSIC_music_stop(); MUSIC_hw_power_dowm();
    //이 있고 다른 모델에서는 필요없을 수도 있음 알아서 빼시길...
    //youandi 01.07.08
    SKY_MusicPowerOff();
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sWaitRetryExit()
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
LOCAL void SKY_API sWaitRetryExit(void)
{
  //N.Y.I
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInProgExit()
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
LOCAL void SKY_API sInProgExit(void)
{
    SKY_AcceptKey(TRUE);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sPausedExit()
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
LOCAL void SKY_API sPausedExit(void)
{
   // N.Y.I
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sMultiWaitExit()
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
LOCAL void SKY_API sMultiWaitExit(void)
{
   // N.Y.I
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sCallEndDispExit(void)
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
LOCAL void SKY_API sCallEndDispExit(void)
{
    // stop call fail sound
    SKY_StopSound();  
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sCallExitFunc()
**
** Description: 
**     Call exit function in each call mode.
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
LOCAL void SKY_API sCallExitFunc(CALL_MODE_E CallMode)
{
    switch(CallMode)
    {
        case ORIGATTEMPT:
            sOrigAttemptExit();     
         break;

        case ORIGINATED :
            sOriginatedExit();
         break;

        case ALERTED    :
            sAlertedExit();
         break;

        case WAITRETRY  :
             sWaitRetryExit();
         break;

        case INPROG     :
             sInProgExit();
         break;

        case PAUSED     :
             sPausedExit();
         break;

        case MULTIWAIT  :
             sMultiWaitExit();
         break;

        case CALLENDDISPLAY  :
             sCallEndDispExit();
         break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     LOCAL void sInitCurrentCallInfo(void)
**
** Description:
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
LOCAL void sInitCurrentCallInfo(void)
{
    // initialize static variable (UI specific), do not initiate
    // call manager related info here
    l_CallInfo.ui_try_cnt              = 0;
    l_CallInfo.secret                  = FALSE;
    l_CallInfo.bPI                     = CAI_ALLOW_PI;
    l_CallInfo.called_party_num.len    = 0;
    l_CallInfo.num.len                 = 0;
    l_CallInfo.alpha.len               = 0;
    l_CallInfo.alpha_cnap.len          = 0;
    l_CallInfo.connect_time            = 0;
    
    memset(l_CallInfo.alpha.buf, '\0', CM_MAX_NUMBER_CHARS);
    memset(l_CallInfo.alpha_cnap.buf, '\0', CM_MAX_NUMBER_CHARS);
    memset(l_CallInfo.num.buf, '\0', CM_MAX_NUMBER_CHARS);
    memset(l_CallInfo.called_party_num.buf, '\0', CM_MAX_NUMBER_CHARS);
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     LOCAL void sEnterCallState(SKY_EVENT_E EventID)
**
** Description:
**     called when entering call state from other state for the first time
**     and decide a mode in alerted mode or orig_attempt mode  
**     
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
LOCAL void sEnterCallState(SKY_EVENT_E EventID)
{
    // initialize current call information
    sInitCurrentCallInfo();
    // no burst string yet
    sInitBurstString(NULL, 0, FALSE);
    SKY_AcceptKey(TRUE);
    SKY_SET_TRANSMIT_DTMF(TRUE);
    SetAutoAnswering(FALSE);
    l_fCallEndedByUser = FALSE;

    if (sIsInAlert()) 
    {
        EQS_START_CHILD_CURRENT_EVENT(SKA_Alerted);
    } 
    else 
    {
        EQS_START_CHILD_CURRENT_EVENT(sSKA_OrigAttempt); /* send this event */
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sAlertedModeEntry()                                 
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
LOCAL void SKY_API sAlertedModeEntry(void) 
{    
    SKY_AcceptKey(TRUE);
    SKY_SET_KEY_TONE(TRUE);
    l_fKeyDownInAlert = FALSE;
    SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());         // turn on backlighting //white
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sUpdateCMCallInfo                          (ui_update_cm_call_info)
**
** Description: 
**     update part (which is from call manager) of current call information     
**
** Input:
**     NONE
**
** Output:
**     NONE
**
** Return value:
**     NONE
**
** Side effects:
**     NONE
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sUpdateCMCallInfo(const CM_CALL_INFO_T* CMCallInfoPtr)
{
    l_CallInfo.cm_info = *CMCallInfoPtr;
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitCurrentCallOnAlert (init_curr_call_on_alert)
**
** Description: 
**     Init current call from *call_info_ptr and ui.cni
**     It is called when incoming call is connected.
**     Before that, CLI state handles everything in ui.cni.     
**
** Input:
**     None
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     None
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sInitCurrentCallOnAlert(const CM_CALL_INFO_T * CallInfoPtr)
{
    CNI_T* pCallerID;

    pCallerID = SKY_GetCallerID();
    sInitCurrentCallInfo();


    l_CallInfo.bPI = pCallerID->bPI;

    /* Called Party 정보 투 넘버서비스 */
    if(SKY_IS_NUMBER_PLUS())
    {
        l_CallInfo.called_party_num.len = pCallerID->CalledParty.nPos;
        if(l_CallInfo.called_party_num.len > 0)
        {
            STRCPY(l_CallInfo.called_party_num.buf, pCallerID->CalledParty.aBuf);
        }
    }

    if(pCallerID->fSecret == FALSE)  /* public group */
    {
        l_CallInfo.ui_try_cnt = 0;
        l_CallInfo.secret     = FALSE;

        /* CNIP 정보 */
        l_CallInfo.num.len    = pCallerID->Num.nPos;
        if(l_CallInfo.num.len > 0)    
        {
            STRCPY(l_CallInfo.num.buf, pCallerID->Num.aBuf);
        }

        if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))
        {
            if(STRCMP(pCallerID->aName, pCallerID->aNameInPhoneBook) != 0)
            {
                /* CNAP와 PhoneBook에 있는 이름이 다른 경우 */           
                /* CNAP 정보 */
                l_CallInfo.alpha_cnap.len = pCallerID->bNameLen;
                STRCPY(l_CallInfo.alpha_cnap.buf,  pCallerID->aName);
                /* PhoneBook 정보 */
                l_CallInfo.alpha.len = pCallerID->bNameLenInPhoneBook;
                STRCPY(l_CallInfo.alpha.buf, pCallerID->aNameInPhoneBook);
            }
            else
            {
                /* CNAP와 PhoneBook에 있는 이름이 동일한 경우 */           
                l_CallInfo.alpha_cnap.len = pCallerID->bNameLen;
                STRCPY(l_CallInfo.alpha_cnap.buf, pCallerID->aName);
            }
        }
        else if((pCallerID->aNameInPhoneBook[0] != '\0') && (pCallerID->aName[0] == '\0'))            
        {          
            /* Phonebook을 통해서 이름을 검색한 경우 */
            l_CallInfo.alpha_cnap.len    = 0;
            l_CallInfo.alpha_cnap.buf[0] = '\0';

            l_CallInfo.alpha.len = pCallerID->bNameLenInPhoneBook;
            STRCPY(l_CallInfo.alpha.buf,  pCallerID->aNameInPhoneBook);
        }
        else if((pCallerID->aNameInPhoneBook[0] == '\0') && (pCallerID->aName[0] != '\0'))
        {
            /* CNAP를 통해서 날아온 경우 */
            l_CallInfo.alpha.len    = 0;
            l_CallInfo.alpha.buf[0] = '\0';

            l_CallInfo.alpha_cnap.len = pCallerID->bNameLen;
            STRCPY(l_CallInfo.alpha_cnap.buf, pCallerID->aName);
        }
        // not used for incoming call  
        l_CallInfo.ui_try_cnt = 0;  
        l_CallInfo.secret     = FALSE;
    }
    else                        /* confidential group */
    {
        // not used for incoming call  
        l_CallInfo.ui_try_cnt           = 0;  
        l_CallInfo.secret               = TRUE;
        l_CallInfo.num.len              = 0;
        l_CallInfo.num.buf[0]           = '\0';
        l_CallInfo.alpha.len            = 0;
        l_CallInfo.alpha_cnap.len       = 0;
        l_CallInfo.alpha_cnap.buf[0]    = '\0';
    }
    l_CallInfo.cm_info  = *CallInfoPtr;  
}






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitCurrentCallOnOriginate
**
** Description: 
**     Init current call from *call_info_ptr and ui.n and ui.a
**     Called when a call is originated (not connected)
**
** Input:
**     None
**
** Output:
**     None
**
** Return value:
**     None
**
** Side effects:
**     None
**
** ---------------------------------------------------------------------------
*/
LOCAL void SKY_API sInitCurrentCallOnOriginate(const CM_CALL_INFO_T* CallInfoPtr)
{
  l_CallInfo.cm_info   = *CallInfoPtr;

  if (ui_call_is_from_ui(CallInfoPtr)) 
  {                       
    // if call originated from UI, call info in the call event
    // is not enough for UI display, bursting etc, need info
    // from editing buffer
    if (l_CallInfo.ui_try_cnt == 0 ) 
    {
      l_CallInfo.secret =  GET_NUMEDIT_SECRET;
      l_CallInfo.num.len = GET_NUMEDIT_POS;
      // buf definition not reasonable or programming error if assert fails, 
      ASSERT(l_CallInfo.num.len <= sizeof(l_CallInfo.num.buf));
      memcpy(l_CallInfo.num.buf, GET_NUMEDIT_BUFFER, l_CallInfo.num.len);
      l_CallInfo.num.buf[l_CallInfo.num.len] = '\0';
      l_CallInfo.num.pi = CAI_ALLOW_PI;
      
      if (GET_ALPHAEDIT_SECRET == FALSE) /* Open Group */
      {
        memcpy(l_CallInfo.alpha.buf, GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
        l_CallInfo.alpha.len = GET_ALPHAEDIT_POS;
        l_CallInfo.alpha.buf[l_CallInfo.alpha.len] = '\0';
      } 
      else                               /* Secret Group */
      {
        l_CallInfo.alpha.buf[0] = '\0';
        l_CallInfo.alpha.len = 0;
      }
      
      l_CallInfo.ui_try_cnt ++;
      // clear ui.a and ui.n buffer, it has finished its job, it will not 
      // mess up with next call      
      // ClearNumEditInfo();                  
      ClearAlphaEditInfo();               
    } 
    else 
    {
      // l_CallInfo already intialized
      l_CallInfo.ui_try_cnt ++;
    }
  } 
  else 
  {
    l_CallInfo.secret = FALSE;
    l_CallInfo.num    = CallInfoPtr->num;
    l_CallInfo.alpha  = CallInfoPtr->alpha;
    l_CallInfo.ui_try_cnt = 0;
  }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SetupCLIfromCallInfo (setup_cli_from_call_info)
**
** Description: 
**     set up CLI from call info of call manager, truncate CLI (if necessary) and
**  perform phone book match
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
void SKY_API SetupCLIfromCallInfo(const CM_CALL_INFO_T* pCallInfo, SKY_EVENT_E Event)
{
    SetupCLICDNIP(
                (CHAR *) pCallInfo->num.buf,
                pCallInfo->num.len,
                pCallInfo->num.pi,
                Event
               );
}


/* 
** ---------------------------------------------------------------------------
** missed call information
** ---------------------------------------------------------------------------
*/
void SKY_API ClearMissedCallInfo(void)
{
    l_MissedCall.nMissedCalls = 0;
    l_MissedCall.dwCurrentTime = 0;
}


void SKY_API NewMissedCall(void)
{
    l_MissedCall.dwCurrentTime = SKY_GetLocalTime();
    l_MissedCall.nMissedCalls++;
}

UINT8 SKY_API GetMissedCallCount(void)
{
    return l_MissedCall.nMissedCalls;
}

DWORD SKY_API GetMissedCallTime(void)
{
    return l_MissedCall.dwCurrentTime;
}


#define sMISSEDCALL_TITLE_XPOS          20//29
#define sMISSEDCALL_TITLE_YPOS          (83/*23*/ - SKY_ANNUNCIATORS_HEIGHT) //1

#define sMISSEDCALL_COUNT_XPOS          74//76 //70
#define sMISSEDCALL_COUNT_YPOS          (53 - SKY_ANNUNCIATORS_HEIGHT)//44 //29 //(42 - MISSEDCALL_OFFSET_YPOS)

#define sMISSEDCALL_ANIMATION_XPOS      12 //12
#define sMISSEDCALL_ANIMATION_YPOS      (34 - SKY_ANNUNCIATORS_HEIGHT) //17 //(23 - 6)

#define sMISSEDCALL_DATE_XPOS           8//11 //18
#define sMISSEDCALL_DATE_YPOS           (107 - SKY_ANNUNCIATORS_HEIGHT) //(75 - MISSEDCALL_OFFSET_YPOS)

#define sMISSEDCALL_TIME_XPOS           17//20
#define sMISSEDCALL_TIME_YPOS           (124 - SKY_ANNUNCIATORS_HEIGHT) //(90 - MISSEDCALL_OFFSET_YPOS)   

#define sMISSEDCALL_BACKGROUND_XPOS     4
#define sMISSEDCALL_BACKGROUND_YPOS     (94 - SKY_ANNUNCIATORS_HEIGHT)


LOCAL void SKY_API sInitMissedCallDisp(void);
LOCAL void SKY_API sDisplayMissedCallDateAndTime(void);
LOCAL void SKY_API sUpdateMissedCallIDLCD(void);



LOCAL void SKY_API sInitMissedCallDisp(void)
{
    HCONTROL            hControl;  
        CHAR                    szBuf[25];
        

    SKY_AcceptKey(TRUE);

    //부재중전화 
    //hControl = EQC_StaticText(sMISSEDCALL_TITLE_XPOS, 
    //                          sMISSEDCALL_TITLE_YPOS, 
    //                          (EQS_TOKEN) TKN_NULL);
    // 
    //sprintf((char*) szBuf, "%s%s%s%s%s", SF_ABSENCE_CALL1_5, 
    //                                     SF_ABSENCE_CALL2_5,
    //                                     SF_ABSENCE_CALL3_5,
    //                                     SF_ABSENCE_CALL4_5,
    //                                     SF_ABSENCE_CALL5_5);
    //EQC_SetStaticText(hControl, (BYTE*)szBuf);


    hControl = EQC_Animation( sMISSEDCALL_ANIMATION_XPOS,            
                              sMISSEDCALL_ANIMATION_YPOS, 
                              &g_AniMissedCall,
                              NULL);           
    EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);

    if(SKY_IS_PHONE_OPEN())
    {
       EQC_StartAnimation(hControl, TRUE);
    }

    EQC_Image(sMISSEDCALL_TITLE_XPOS, 
              sMISSEDCALL_TITLE_YPOS, 
              (BITMAP*)&g_BmpCallPannelMissedCall);


    hControl = EQC_StaticText(sMISSEDCALL_COUNT_XPOS, 
                              sMISSEDCALL_COUNT_YPOS, 
                              (EQS_TOKEN)TKN_NULL);
    sprintf((CHAR*) szBuf, (CHAR*)SKY_GET_TOKEN(TKN_MISSEDCALL_FMT_COUNT), 
                           GetMissedCallCount());
    EQC_SetStaticText(hControl, (BYTE*)szBuf);

#ifdef FEATURE_SKY_SUBLCD
    {           
        BOOL    bAM;
        CHAR    cAMPM;
        clk_julian_type julian; /* julian time conversion */

        clk_secs_to_julian((DWORD)GetMissedCallTime(), &julian);
        bAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);

        if(bAM == FALSE && julian.hour != 12)
            julian.hour -= 12;

        if(bAM)
            cAMPM = 'A';
        else 
            cAMPM = 'P';

        SKY_SubLCDTextOut(LN_UPPER, TKN_SLCD_MISSED_CALL);
        SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_MISSED_CALL_FMT),
                              julian.hour, julian.minute, 
                              cAMPM,
                              GetMissedCallCount());
    }
#endif//#ifdef FEATURE_SKY_SUBLCD
}


LOCAL void SKY_API sDisplayMissedCallDateAndTime(void)
{
    CHAR                szBuf[25];
    HCONTROL            hControl;
    BOOL                bAM;
    CHAR                cAMPM;
    clk_julian_type     julian; /* julian time conversion */
    //DRAW_MODE           OldDrawMode;    

    //EQS_PutBitmap(sMISSEDCALL_BACKGROUND_XPOS, 
    //              sMISSEDCALL_BACKGROUND_YPOS,
    //              &g_BmpGeneralPannel); 

    //OldDrawMode = EQS_GetDrawMode();
    //EQS_SetDrawMode(DM_OR);

    clk_secs_to_julian((DWORD)GetMissedCallTime(), &julian);
    bAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);

    if(bAM == FALSE && julian.hour != 12)
        julian.hour -= 12;

    sprintf((CHAR*) szBuf, (CHAR*)SKY_GET_TOKEN(TKN_MISSEDCALL_FMT_DATE), 
                           julian.month, julian.day, 
                           pszDayOfWeek[julian.day_of_week]);
    EQS_TextOutStr(sMISSEDCALL_DATE_XPOS, sMISSEDCALL_DATE_YPOS, (BYTE *)szBuf);

    if(bAM)
        cAMPM = 'A';
    else 
        cAMPM = 'P';
        
    sprintf((CHAR*) szBuf, (CHAR*)SKY_GET_TOKEN(TKN_MISSEDCALL_FMT_TIME), 
                           (bAM) ? SKY_GET_TOKEN(TKN_DAT_AM) : SKY_GET_TOKEN(TKN_DAT_PM),
                           julian.hour, julian.minute);
    EQS_TextOutStr(sMISSEDCALL_TIME_XPOS, sMISSEDCALL_TIME_YPOS, (BYTE *)szBuf);
    //EQS_SetDrawMode(OldDrawMode);
}


LOCAL void SKY_API sUpdateMissedCallIDLCD(void)
{
    HCONTROL  hControl;
    BOOL      bAM;
    CHAR      cAMPM;
    clk_julian_type julian; /* julian time conversion */

    SKY_AcceptKey(TRUE);

    if(SKY_IS_PHONE_OPEN())
    {
       EQC_StartAnimation(EQS_GetNthIOC(0), TRUE);
    }

    //부재중전화 
    clk_secs_to_julian((DWORD)GetMissedCallTime(), &julian);
    bAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);

    if(bAM == FALSE && julian.hour != 12)
        julian.hour -= 12;

    if(bAM)
        cAMPM = 'A';
    else 
        cAMPM = 'P';
        
#ifdef FEATURE_SKY_SUBLCD
    {
        SKY_SubLCDTextOut(LN_UPPER, TKN_SLCD_MISSED_CALL);
        /* "%02d:%02d%c(%02d)건" */
        SKY_SubLCDTextOutStrF(LN_LOWER, SKY_GET_TOKEN(TKN_SLCD_MISSED_CALL_FMT),
                              julian.hour, julian.minute, 
                              cAMPM,
                              GetMissedCallCount());
    }
#endif//#ifdef FEATURE_SKY_SUBLCD
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MissedCall(SKY_EVENT_T* pEvent)
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
void SKY_API SKA_MissedCall(SKY_EVENT_T* pEvent)
{
    STATIC  UINT8              nPreviousMinute;

    clk_julian_type            julian;
    DWORD                      dwTime;
    ALARM_DISP_TYPE_T          AlarmType;


    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START:        
        EQS_GetDC(TRUE);        
        EQS_SetWindow(SKY_DEF_WIN_X,
                SKY_DEF_WIN_Y, 
                SKY_DEF_WIN_DX, 
                SKY_DEF_WIN_DY, 
                CL_NONE_RECEIPT_NONE, 
                NULL, // show annunciators 
                (WA_SHOW_HEADER|WA_SHOW_CML));
        sInitMissedCallDisp();
        SKY_SNI(HS_SNI_CALL_ALERT, FALSE); //white 2001.01.05.
        if(SKY_GET_AUTO_SUBBACKLIGHT() == AB_ALWAYS || \
            (SKY_GET_AUTO_SUBBACKLIGHT() == AB_TIME && SKY_IsSubBacklightOnTime()) ||\
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
        SKY_SubBacklightOn(SKY_GET_AUTO_SUBBACKLIGHT());
        
        if(SKY_GET_AUTO_BACKLIGHT() == AB_ALWAYS || \
            SKY_GET_NO_OFF_BACKLIGHT() || SKY_IS_HANDS_FREE())
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());      //white 2001.07.03

            dwTime = SKY_GetLocalTime();
            clk_secs_to_julian(dwTime, &julian);
            nPreviousMinute = julian.minute;


        break;

    case SE_APP_END:
        SKY_SubLCDDefault();
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :
        sDisplayMissedCallDateAndTime();
        break;

    case SE_APP_RESUME:
        if( EQS_GET_CHILD() ==  sSKA_PlayAlarm )
        {
            /* 알람 울린 뒤, 원래 대로 ID LCD를 그려 준다 */
            sUpdateMissedCallIDLCD();
            break;
        }

		if( EQS_GET_CHILD() == SKA_MainAllCode )
		{
			if( pEvent->wParam == SE_IO_NEDIT_MATCH_LENGTH )
			{
				EQS_START_CHILD(SKA_MainStackCode);
			}
            else
			{
				EQS_END();
			}
			break;
		}

#ifdef FEATURE_SKTT_EQUS_LOCATION
        if(EQS_GET_CHILD() != SKA_LocationSendMsg)
        {
            EQS_END();
        }
#else
        EQS_END();
#endif //#ifdef FEATURE_SKTT_LOCATION
            break;

    case SE_SYS_PACEMAKER:  //2001.08.2 redstar
#ifdef FEATURE_WAP
        if(SKY_IS_IN_WAP()) 
        {
            EQS_DEF_EVENT_PROC(pEvent);
            break;
        }
#endif //#ifdef FEATURE_WAP

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
        ClearMissedCallInfo(); // it is defined in eqa_call.c and used 
                         // in eqa_idle.c and eqa_call.c
        EQS_END();
        break;

    case SE_SOFTKEY_RECEIPT:
        ClearMissedCallInfo();
        SetReceivedCallStack(TRUE);
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
		{
            //EQS_START_SIBLING(SKA_MainAllCode, FALSE);
			EQS_START_CHILD(SKA_MainAllCode); //redstar 2001.08.21
		}
        else
		{
            EQS_START_CHILD(SKA_MainStackCode);       
		}
                break;    
        
    case SE_KEY_PHONE_OPEN :   /* Earpiece raised, just reset softkeys */
        EQC_StartAnimation(EQS_GetNthIOC(0), TRUE);
        break;
    
    /* jrkwon 2000-05-24 --> flip key close */
    case SE_KEY_PHONE_CLOSE :
    case SE_KEY_F1:
    case SE_KEY_F2:
    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_DOUBLE_CLICK:
    case SE_KEY_EAR_LONG_CLICK:
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
    case SE_KEY_SEND:
        ClearMissedCallInfo(); // it is defined in eqa_call.c and used 
                         // in eqa_idle.c and eqa_call.c
        EQS_END();
        break;    
    
    case SE_KEY_AUTO_ANSWER:
    case SE_KEY_AUTO_ANSWER_LONG:
        break;

    case SE_KEY_RELEASE_AUTO_ANSWER:        
    case SE_KEY_RECORD:
    case SE_KEY_RECORD_LONG:
        if(SKY_IS_EAR_JACK_ON() || SKY_IS_PHONE_OPEN())
        {
            ClearMissedCallInfo(); // it is defined in eqa_call.c and used 
                                         // in eqa_idle.c and eqa_call.c
            EQS_END();
        }
        else
        {
            break;
        }
        break;

    /* case UI_TOIDLE_F: */
    case SE_CALL_INCOMING : //UI_CALL_INCOMING_E:
    case SE_CALL_CALLER_ID : //UI_CALL_CALLER_ID_E:
    case SE_CALL_ORIGINATED : //UI_CALL_ORIGINATED_E:
        EQS_END_CURRENT_EVENT();
        break ;

#ifdef FEATURE_SKTT_EQUS_LOCATION
    case SE_LOCATION_TIMEOUT:
        CheckCurrentTime();
        break;
#endif //#ifdef FEATURE_SKTT_LOCATION
    default : /* no exit until OK or END or CLR or PWR pressed or earpiece */
        EQS_DEF_EVENT_PROC(pEvent);
        break ;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     void SKY_API SKA_SavePhoneNumber(SKY_EVENT_T *pEvent)
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
void SKY_API SKA_SavePhoneNumber(SKY_EVENT_T *pEvent)
{
    static HCONTROL  hNumber;
        HCONTROL hControl;
        BYTE     szBuf[65];
        RECT     Rect;
        INT16    nUsedBook;

    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */ 
        {               
            SKY_TOKEN paMenu[2];

            EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE,
                    NULL, 
                    WA_SHOW_HEADER );

            hNumber = pEvent->wParam;
                                                            
            paMenu[0] = TKN_PBOOK_SAVE_NEW_DATA;
            paMenu[1] = TKN_PBOOK_SAVE_OLD_DATA;
            

            nUsedBook = MAX_BOOK_NUMBER - active_list.nFreeBook;
            sprintf((char*)szBuf, (char*)SKY_GET_TOKEN(TKN_PBOOK_USAGE_MSG),\
                        nUsedBook);

            EQC_PopUpMenuExtend((EQS_TOKEN_EX*)paMenu, 2, 0,\
                                CS_POPUP_TITLE|CS_POPUP_RETURN_TOKEN, 0,\
                                szBuf, NULL);

        }
        break;
        
    case SE_APP_END:            
        break;
        
    case SE_APP_RESUME:
        EQS_END();
        break;
    case SE_IO_POPUP_SELECT: 
        EQC_GetNumberEditValue(hNumber, szBuf);
        switch( pEvent->dwParam )
        {
            case TKN_PBOOK_SAVE_NEW_DATA:
                SaveOnNewBook(szBuf, FALSE); // 새이름에 저장
                break;
            case TKN_PBOOK_SAVE_OLD_DATA:
                SaveOnOldBook(szBuf, FALSE); // 기존이름에 저장
                break;
            default:
                break;
        }
        break;

    case SE_KEY_CLEAR:    
        EQS_END();
        break;

    case SE_IO_POPUP_CANCEL:
        if( pEvent->dwParam != EQC_DELETED )
        {
                EQS_END();
        }
        break;

    case SE_KEY_END:
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    default:
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
PAUSED_MODE_E SKY_API GetPausedMode(void)
{
    return l_PausedMode;
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
BOOL SKY_API IsPauseMode(BYTE* aPhoneNumber)
{
    UINT8 iLoop;

    for(iLoop = 0; aPhoneNumber[iLoop] != '\0'; iLoop++)
    {
        if((aPhoneNumber[iLoop] == P_PAUSE_CHAR) || (aPhoneNumber[iLoop] == T_PAUSE_CHAR))
        {
            l_PausedMode = ((aPhoneNumber[iLoop] == P_PAUSE_CHAR) ? PM_PPAUSE : PM_TPAUSE);
            return TRUE;
        }
    }
    l_PausedMode = PM_NONE;

    return FALSE;
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
UINT8 SKY_API ParsePhoneNumber(BYTE* aPhoneNumber, BYTE* szString)
{
    UINT8 iLoop;

    for(iLoop = 0; (aPhoneNumber[iLoop] != P_PAUSE_CHAR) && (aPhoneNumber[iLoop] != T_PAUSE_CHAR); iLoop++)
    {
        szString[iLoop] = aPhoneNumber[iLoop];
    }
    szString[iLoop] = '\0';
    SetRealPhoneNumberLength(iLoop);

    return iLoop;  
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
enum tagDIALING_CONTROL_E
{
    DC_IMAGE_PHONE,
    DC_IMAGE_BOX,
    DC_DIALEDIT,
    DC_MAX
};


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
enum tagDIALING_MESSAGE_E
{
    SERVICE_PROGRAM_MESSAGE_E = 1,
    PHONE_BOOK_SEARCH_MESSAGE_E,
    NUMBER_SEARCH_FAIL_MESSAGE_E
};


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
enum tagSECRET_CODE_E
{
    SC_NAM,
    SC_NAM_SIMPLE,
    SC_TEST
};

#define sSECRET_CODE_NAM_PROGRAMMING          "*759#626*7388464#" 
#define sSECRET_CODE_NAM_PROGRAMMING_SIMPLE   "#7583538324#626#"
#define sSECRET_CODE_TEST_PROGRAMMING         "#7583538324#8378#"


void SKY_API SKA_Dialing(SKY_EVENT_T* pEvent)
{
    BYTE             aPhoneNumber[NE_MAX_SIZE], szString[40];
    HCONTROL         hDialEditBox;
    HCONTROL         hControl;
    STATIC BOOL      s_bDialing, fGoToNAMProgramming;
    UINT8            nNumDigit, ListHandle, nRealPhoneNumber;
        
    switch(pEvent->EventID)
    {
    case SE_APP_START : /* The very first event */

        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_SAVE_FIND, //CL_NONE_CONFIRM_FIND, 
            SKY_GET_TOKEN(TKN_IDLE_INPUT_DIALNUM), 
            WA_SHOW_HEADER | WA_SHOW_CML);

        s_bDialing = FALSE;
        fGoToNAMProgramming = FALSE;

        EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
                  (sORIGINATION_PHONE_OPEN_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpPhoneOpen);

        EQC_Image(sORIGINATION_IMAGE_XPOS, 
                  (sORIGINATION_IMAGE_YPOS - sDIALING_DISPLAY_OFFSET_Y), 
                  (BITMAP*)&g_BmpCallPannel);

        hDialEditBox = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                      (DIAL_DIGIT_POS_Y - sDIALING_DISPLAY_OFFSET_Y), 
                                      DIAL_DIGIT_LENGTH_PER_LINE, 
                                      DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,
                                      (BYTE*)"" );
        EQC_SetNumberEditBackColor(hDialEditBox, NUMBER_PANNEL_COLOR);
        EQC_SetNumberEditColor(hDialEditBox, NUMBER_TEXT_COLOR);

        if(SKY_IS_AUTO_AREA_CODE())
            EQC_SetLocalAreaCode(hDialEditBox, SKY_GET_AREA_CODE());

        EQC_SET_STYLE(hDialEditBox, CS_NUMBER_EDIT_RIGHT2LEFT | 
                                    CS_NUMBER_EDIT_NO_CML     | 
                                    CS_NUMBER_EDIT_TP_PAUSE   |
                                    CS_NUMBER_EDIT_ALONE      );//|
                                    //CS_NUMBER_EDIT_ORING      |
                                    //CS_NUMBER_EDIT_DRAW_ALL_CONTROL);
        EQC_SetNumberEditBigFont(hDialEditBox, TRUE);   // Set Big Font
        EQS_SetIOCActive(hDialEditBox);
        EQC_NotifyNumberEditMatch( hDialEditBox, (BYTE*)sSECRET_CODE_NAM_PROGRAMMING, SC_NAM);               // entering engineering nam programming.
        EQC_NotifyNumberEditMatch( hDialEditBox, (BYTE*)sSECRET_CODE_NAM_PROGRAMMING_SIMPLE, SC_NAM_SIMPLE); // entering simple nam programming
        EQC_NotifyNumberEditMatch( hDialEditBox, (BYTE*)sSECRET_CODE_TEST_PROGRAMMING, SC_TEST);             // entering test programming.
        break;

    case SE_APP_END:
        if(fGoToNAMProgramming == TRUE)
        {
            SKY_PowerOff(POM_RESET);
        }
#ifdef FEATURE_SKY_SUBLCD
        SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
        EQS_ReleaseDC();
        break;

    case SE_APP_RESUME:
        
#ifdef  FEATURE_SKT_EIF     //redstar 01.06.28          
        if( EQS_GET_CHILD() == SKA_MainEif )
        {
            EQS_SetIOCActive(EQS_GetNthIOC(DC_DIALEDIT));
            break;
        }
#endif //FEATURE_SKT_EIF

        if( EQS_GET_CHILD() == SKA_SavePhoneNumber )
        {
            EQS_END();//EQS_SetIOCActive(EQS_GetNthIOC(DC_DIALEDIT));
        }
        else if( EQS_GET_CHILD() == SKA_BookInput ||\
                 EQS_GET_CHILD() == SKA_SpeedSearch)
        {
            EQS_END();
        }               
        else if( EQS_GET_CHILD() == SKA_MainNAM )
        {
            hControl = EQC_MessageBox((BYTE *)"Exit Service Programming", MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hControl, SERVICE_PROGRAM_MESSAGE_E);
        }
        else if( EQS_GET_CHILD() == SKA_MainSimpleNAM)
        {
            hControl = EQC_MessageBox((BYTE *)"Exit Service Programming", MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hControl, SERVICE_PROGRAM_MESSAGE_E);
        }
        else if(EQS_GET_CHILD() == SKA_MainPbookCode)
        {
            if(pEvent->wParam == BOOKCODE_RETURN_SEARCHING)
            {
                hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING),\
                                           0,\
                                           MBT_NONE, MBT_HELP);
                EQC_SET_USERDATA(hControl, PHONE_BOOK_SEARCH_MESSAGE_E );
                break;
            }else
                EQS_SetIOCActive(EQS_GetNthIOC(DC_DIALEDIT));
            
        }
        break;

    case SE_APP_DRAW:
        //sDrawGeneralFrameInDialing();
        break;

    // 에티켓모드
    case SE_KEY_MANNERS:
        if(EQC_GetNumberEditValue(EQS_GetNthIOC(DC_DIALEDIT), aPhoneNumber) > 1)
            break;

        if(SKY_IS_ETIQUETTE())
        {
#ifdef FEATURE_SKY_SUBLCD
            SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_CLEAR_ETIQUETTE);
            SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD

            SKY_SET_ETIQUETTE(FALSE);
            EQC_NoBeepMessageBox((BYTE*)SKY_GET_TOKEN(TKN_ETIQUETTE_MODE_OFF), 
                           MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);//NULL);
            if(!SKY_IS_ALERT())
                SKY_PlaySoundEx( UI_MSG_SND, SND_PIP_TONE, 5, 1);
        }
        else
        {
#ifdef FEATURE_SKY_SUBLCD
            SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_SET_ETIQUETTE);
            SKY_SubLCDEndText(LN_LOWER);
#endif //#ifdef FEATURE_SKY_SUBLCD
            SKY_SET_ETIQUETTE(TRUE);
            EQC_NoBeepMessageBox((BYTE*)SKY_GET_TOKEN(TKN_ETIQUETTE_MODE_ON), 
                            MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
            SKY_MotorTime(800);
        }

        SKY_SetSoundDevice(SND_MUTE_MAX, SND_MUTE_MAX, FALSE);
        SKY_DisplayBellVibIcon();
        break;


    case SE_IO_NEDIT_PRESS_LONGKEY:                
        if(SKY_IS_PHONE_BOOK_LOCK() && (pEvent->wParam == SE_IO_NEDIT_PRESS_LONGKEY))
        {
            SpeedDial( pEvent->dwParam );        
            break;
        }

        EQC_GetNumberEditValue(pEvent->wParam, aPhoneNumber);

        /* 
        ** jrkwon 2001.09.11 --> 지역번호가 설정되어 있고, 
        ** 사용자가 '0' 등으로 시작하는 번호를 누르면 설정된 지역번호는
        ** 지워진다. 이때 설정된 지역번호와 같은 번호를 수동으로 입력하고,
        ** 두자리 숫자를 입력하면서 두 번째 숫자를 길게 누르면 단축 다이얼로
        ** 동작.
        ** dwParam == 0 이면 자동지역번호가 지워진 상태.
        ** 이 상태에서는 전화번호가 저장된 버퍼에서 지역번호를 빼주는 작업을
        ** 해 주면 안된다.
        */
		//if( SKY_IS_AUTO_AREA_CODE() )
		if(pEvent->dwParam != 0 && SKY_IS_AUTO_AREA_CODE())
        /* <--- */
		{
			INT8  nLen;
			BYTE  szTemp[NE_MAX_SIZE];
            BYTE  szAreaCode[5];

            memset(szAreaCode,0,5);
            nLen = STRLEN(SKY_GET_AREA_CODE());
            STRNCPY(szAreaCode, aPhoneNumber, nLen );
            szAreaCode[nLen] = 0;

            if( STRCMP(szAreaCode, SKY_GET_AREA_CODE()) == 0)
            {
                STRCPY( szTemp, aPhoneNumber+nLen );								    
                STRCPY( aPhoneNumber, szTemp );		
            }                                								    			
		}
        
        /* 
        ** jrkwon 2001.09.11 --> 
        ** 단축 다이얼에 'T'나 'P'가 들어 있으면 잘못.
        */
        if(STRCHR(aPhoneNumber, 'T') || STRCHR(aPhoneNumber, 'P'))
            break;
        /* <-- */

        nNumDigit = STRLEN(aPhoneNumber);
        
        if( nNumDigit == 1 )
        {                                   
            nRealPhoneNumber = aPhoneNumber[0] - '0';			
        }
        else if( nNumDigit == 2)
        {
            nRealPhoneNumber = (aPhoneNumber[0] -'0')*10 + (aPhoneNumber[1]-'0');
        }
        else
		{
			nRealPhoneNumber = 0;
		}

		if( nRealPhoneNumber < 1 || nRealPhoneNumber > MAX_SDIAL_NUMBER )
			break;

           
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
           EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_IO_NEDIT_PRESS_LONGKEY, nRealPhoneNumber); 
        }else
            SpeedDial( nRealPhoneNumber );        

        break;

    case SE_IO_NEDIT_CANCEL:
    case SE_IO_NEDIT_NODIGIT:
        EQS_END();
        break;


    case SE_IO_NEDIT_DONE:
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
            if(pEvent->wParam != BOOKCODE_IN_DIAL)
            {
                EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_IN_DIAL, EQS_NA); 
                break;
            }
        }
        
        EQS_START_CHILD_EVENT(SKA_SavePhoneNumber, SE_APP_START,\
                                      EQS_GetNthIOC(DC_DIALEDIT), EQS_NA);       
        break; 
    case SE_SOFTKEY_FIND:
        if(SKY_IS_PHONE_BOOK_LOCK())
        {
            EQS_START_CHILD_EVENT(SKA_MainPbookCode,SE_APP_START,BOOKCODE_SEARCH_IN_IDLE, EQS_NA);
        }else
        {
            hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_ON_SEARCHING),\
                   0,\
                   MBT_NONE, MBT_HELP);
            EQC_SET_USERDATA(hControl, PHONE_BOOK_SEARCH_MESSAGE_E );
        }
        break; 

    case SE_IO_MESGBOX_END:      
       if( pEvent->dwParam == SERVICE_PROGRAM_MESSAGE_E )  
        {
             EQS_END();           
        }
       else if( pEvent->dwParam == PHONE_BOOK_SEARCH_MESSAGE_E )
       { // 전화번호 검색
            EQC_GetNumberEditValue(EQS_GetNthIOC(DC_DIALEDIT), aPhoneNumber);
            ListHandle = SearchNumber(aPhoneNumber,BSM_NUMBER_PART_MATCH);
            if( ListHandle == 0 )  // 찾는 번호 없음 메시지 출력후 상위로 이동 
            {
               hControl = EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_PBOOK_NO_SEARCH_NUMBER_MSG),\
                                             MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_HELP);
               EQC_SET_USERDATA(hControl, NUMBER_SEARCH_FAIL_MESSAGE_E);
               break;
            }               
            else
            {
                    SortBookName(ListHandle);
                    EQS_START_SIBLING_EVENT( SKA_ViewSearchNumber, SE_APP_START,\
                                                                     (WORD) ListHandle, 0, FALSE );
            }  

       }
       else if( pEvent->dwParam == NUMBER_SEARCH_FAIL_MESSAGE_E )
       {            
               EQS_SetIOCActive(EQS_GetNthIOC(DC_DIALEDIT));
               break;
       }
       else 
       {
         EQS_END();
       }

       break;

    case SE_SYS_TO_IDLE:
        if (!s_bDialing)
            EQS_PassCurrentEvent();
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_SEND:         /* send key */            
		if(SKY_IS_NOSERVICE() == FALSE)
		{
            hDialEditBox = EQS_GetNthIOC(DC_DIALEDIT);                         
			if(hDialEditBox != INVALID_CONTROL_HANDLE )
			{
				nNumDigit = EQC_GetNumberEditValue(hDialEditBox, aPhoneNumber);  
				
    
				if(aPhoneNumber[0] != '\0')
				{
					SET_NUMEDIT_BUFFER(aPhoneNumber, nNumDigit);
					SET_NUMEDIT_POS(nNumDigit);                   

					//PBResult = FindNameWithNum(aPhoneNumber);
                        
					s_bDialing = TRUE;
					if(IsPauseMode(aPhoneNumber))   /* Paused Mode 일 경우 */
					{
					   nRealPhoneNumber = ParsePhoneNumber(aPhoneNumber, szString);
                       if((szString[0] == '0')    && 
						  (szString[1] == '0')    &&
						  (szString[2] >  '0')    && 
						  (nRealPhoneNumber > 2)  &&
						  SKY_IS_OVERSEAS_CALL_LOCK())
                       {
                           EQS_START_SIBLING(SKA_MainInterCode, FALSE);  
                       }else
                       {
                           SKY_SET_ORIGINATE(TRUE);
                           ui_orig_voice_call(szString, nRealPhoneNumber,  
									      GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
                       }
                    }
				    else                            /* Paused Mode가 아닐 경우 */
                    {						
					   if((aPhoneNumber[0] == '0')   && 
						  (aPhoneNumber[1] == '0')   && 
						  (aPhoneNumber[2] >  '0')   &&
						  (nNumDigit       >   2)    &&
						  SKY_IS_OVERSEAS_CALL_LOCK())
                       {
                          EQS_START_SIBLING(SKA_MainInterCode, FALSE);  
                       }
					   else
					   { // check speed dial 
                            /* 
                            ** jrkwon 2001.09.11 --> 지역번호가 설정되어 있고, 
                            ** 사용자가 '0' 등으로 시작하는 번호를 누르면 설정된 지역번호는
                            ** 지워진다. 이때 설정된 지역번호와 같은 번호를 수동으로 입력하고,
                            ** 두자리 숫자를 입력하면서 두 번째 숫자를 길게 누르면 단축 다이얼로
                            ** 동작.
                            ** dwParam == 0 이면 자동지역번호가 지워진 상태.
                            ** 이 상태에서는 전화번호가 저장된 버퍼에서 지역번호를 빼주는 작업을
                            ** 해 주면 안된다.
                            */
						   //if( SKY_IS_AUTO_AREA_CODE() )
						   if( (EQC_GetLocalCodeLength(EQS_GetNthIOC(DC_DIALEDIT)) != 0) &&  SKY_IS_AUTO_AREA_CODE() )
                            /* < -- */
						   {
								INT8  nLen;
								BYTE  szTemp[NE_MAX_SIZE];
                                BYTE  szAreaCode[5];
                                
                                memset(szAreaCode,0,5);
                                nLen = STRLEN(SKY_GET_AREA_CODE());
                                STRNCPY(szAreaCode, aPhoneNumber, nLen );
                                szAreaCode[nLen] = 0;

                                if( STRCMP(szAreaCode, SKY_GET_AREA_CODE()) == 0)
                                {
                                    STRCPY( szTemp, aPhoneNumber+nLen );								    
                                    STRCPY( aPhoneNumber, szTemp );		
                                }                                								    
						   }
						   
						   nNumDigit = STRLEN(aPhoneNumber);
						   
     					   if(nNumDigit == 1)
						   { 
	   							nRealPhoneNumber = aPhoneNumber[0] - '0';   
								if( nRealPhoneNumber > 0 && nRealPhoneNumber < 10 )
								{
									if(SKY_IS_PHONE_BOOK_LOCK())
									{
									   EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
									}
									else
									{
										SpeedDial( nRealPhoneNumber ); 
									}
								}
								else if(aPhoneNumber[0] == '#' || aPhoneNumber[0] == '*' || aPhoneNumber[0] == '0')
								{
									EQS_START_SIBLING(SKA_StartOrigination, FALSE);
								}
						   }
						  else if(nNumDigit == 2)
						  {
                              if( isdigit(aPhoneNumber[0]) && isdigit(aPhoneNumber[1]) )
                              {
							        nRealPhoneNumber = (aPhoneNumber[0] -'0')*10 + (aPhoneNumber[1]-'0');     
							        if(SKY_IS_PHONE_BOOK_LOCK() && (nRealPhoneNumber > 0 && nRealPhoneNumber < 100))
							        {
								        EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, SE_KEY_SEND, nRealPhoneNumber); 
							        }
							        else if(nRealPhoneNumber > 0 && nRealPhoneNumber < 100 )         
							        {
								        SpeedDial( nRealPhoneNumber ); 
							        }
							        else
							        {
								        EQS_START_SIBLING(SKA_StartOrigination, FALSE);
							        }
                              }
                              else
                              {
                                  EQS_START_SIBLING(SKA_StartOrigination, FALSE);
                              }                          
						  }
						  else
						  {
								EQS_START_SIBLING(SKA_StartOrigination, FALSE);
						  }

					   }
					}
                }
            }    
		}
		else
		{
		  EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                 MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
		}        
         break; /* SEND_K */

     case SE_IO_NEDIT_MATCH:
         if(pEvent->dwParam == SC_NAM)
         {
            fGoToNAMProgramming = TRUE;                  
            EQS_START_CHILD(SKA_MainNAM); 
         }
         else if(pEvent->dwParam == SC_NAM_SIMPLE)
         {
            fGoToNAMProgramming = TRUE;                  
            EQS_START_CHILD(SKA_MainSimpleNAM);
         }
         else if(pEvent->dwParam == SC_TEST)
            EQS_START_SIBLING(SKA_MainTestMenuTop, FALSE);
         break;

    case SE_KEY_POUND_LONG:
        EQC_GetNumberEditValue(EQS_GetNthIOC(DC_DIALEDIT), aPhoneNumber);  
        if(aPhoneNumber[0] == '#' && aPhoneNumber[1] == 0 )
        {
            SKY_SET_AUTO_LOCK_MODE(TRUE); 
            SKY_SET_SHORTLOCK(TRUE);
            EQS_END_EVENT(SE_KEY_POUND_LONG, EQS_NA, EQS_NA); //EQS_EndEvent
        }
        break;

#ifdef FEATURE_SMART_BATTERY
    case SE_KEY_0_LONG: /* smart battery information */
        EQC_GetNumberEditValue(EQS_GetNthIOC(DC_DIALEDIT), aPhoneNumber);  
        if(aPhoneNumber[0] == '0' && aPhoneNumber[1] == 0 )
        {
            EQS_START_SIBLING(SKA_ViewSMB, FALSE);
        }
        break;
#endif
#ifdef FEATURE_SKTT_EQUS_LOCATION
        case SE_LOCATION_TIMEOUT:
                CheckCurrentTime();
                break;
#endif //#ifdef FEATURE_SKTT_LOCATION

    default:
        SKY_DefEventProc(pEvent);
        break;
    }
}





/* 
** ---------------------------------------------------------------------------
** calling time information
** ---------------------------------------------------------------------------
*/
void SKY_GetCallingTime(clk_julian_type *pJulian)
{
    clk_secs_to_julian( clk_uptime()-l_CallInfo.connect_time, pJulian);
}

#ifdef FEATURE_SKY_SUBLCD
void SKY_DisplayCallingTimeInSubLCD(clk_julian_type *pJulian)
{
    BYTE szCallTime[50];

    if(SKY_SubLCDIsEndText(LN_LOWER) == FALSE) /* 통화 시간 표시할 수 없으면 */
        return;
    
    /* "통화(%02d:%02d:%02d)" */
    sprintf((char*)szCallTime, (char*)SKY_GET_TOKEN(TKN_SLCD_IN_CALL_TIME_FMT), 
                               pJulian->hour, pJulian->minute, pJulian->second);
    SKY_SubLCDTextOutStr(LN_LOWER, szCallTime);
    SKY_SubLCDEndText(LN_LOWER);
}
#endif//#ifdef FEATURE_SKY_SUBLCD






/* 
** ---------------------------------------------------------------------------
**
** Function:    
**    DisplayCallTime
**
** Description: 
**     dispay call number
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
void DisplayCallTime(HCONTROL hControl)
{
    BYTE szCallTime[50];
    clk_julian_type julian;

    //clk_secs_to_julian( clk_uptime()-l_CallInfo.connect_time, &julian);
    SKY_GetCallingTime(&julian);

    sprintf((char *)szCallTime, (char*)SKY_GET_TOKEN(TKN_CALL_TIMER_FMT),
            julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, szCallTime);

#ifdef FEATURE_SKY_SUBLCD
    SKY_SubLCDEndText(LN_UPPER);
    SKY_SubLCDEndText(LN_LOWER);

    SKY_DrawSubAnnunciators();
    SKY_DisplayCallingTimeInSubLCD(&julian);
    ///* "통화(%02d:%02d:%02d)" */
    //sprintf((char*)szCallTime, (char*)SKY_GET_TOKEN(TKN_SLCD_IN_CALL_TIME_FMT), 
    //                           julian.hour, julian.minute, julian.second);
    //SKY_SubLCDTextOutStr(LN_LOWER, szCallTime);
#endif//#ifdef FEATURE_SKY_SUBLCD

    // 통화1분 경과음
    if(SKY_IS_1MINUTE_WARNING_SIGNAL())
    {
        if(julian.second == 0 && julian.minute > 0)
            SKY_PlayToneEx( UI_BEEP_SND, SND_WARN, 150 , 3 );
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
LOCAL BOOL sMakeDateTimeString(BOOL fDisplayRightNow, BYTE pszTmpBuf[])
{
   DWORD             dwCurrentTime;
   STATIC WORD       s_wMinute = 0;
   clk_julian_type   julian;
   BOOL              fChangeMinute = FALSE, fAM;
   

   dwCurrentTime = SKY_GetLocalTime();

   if(SKY_IS_VALID_LOCAL_TIME(dwCurrentTime))
   {
     clk_secs_to_julian(dwCurrentTime, &julian);
     if((fDisplayRightNow == TRUE) || (s_wMinute != julian.minute))
     {

       fAM = (BOOL) ((julian.hour < 12) ? TRUE : FALSE);
       if(fAM == FALSE && julian.hour != 12)
          julian.hour -= 12;
       else if(fAM == FALSE && julian.hour == 24)
          julian.hour = 0;
         
       sprintf((char*)pszTmpBuf, "%2d.%02d[%s]%2d:%02d%s", /* jrkwon 2001.06.28 --> 시간필드는 0으로 시작하지 않도록 */
                                 julian.month,
                                 julian.day,
                                 (CHAR*)SKY_GET_TOKEN(g_aDayOfWeek[julian.day_of_week]),
                                 julian.hour,
                                 julian.minute,
                                 ((fAM) ? SKY_GET_TOKEN(TKN_IDLE_AM): SKY_GET_TOKEN(TKN_IDLE_PM)));
       s_wMinute = julian.minute;
       fChangeMinute = TRUE;
     }
   }
   else /* Valid한 시간이 아닐 경우 */
   {
     sprintf((char*)pszTmpBuf, "%2d.%02d[%s]%2d:%02d%s", /* jrkwon 2001.06.28 --> 시간필드는 0으로 시작하지 않도록 */
                                 0,
                                 0,
                                 (CHAR*)SKY_GET_TOKEN(g_aDayOfWeek[0]),
                                 0,
                                 0,
                                 SKY_GET_TOKEN(TKN_IDLE_AM));
   }

   return fChangeMinute;
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
LOCAL void sDisplayCurrentDateTime(HCONTROL hControl)
{
    BYTE pszDateTime[20];
    
    if(sMakeDateTimeString(FALSE, pszDateTime) == TRUE)
    {
      EQC_SetStaticText(hControl, pszDateTime);
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sDisplayPhoneNumber
**
** Description: 
**     dispaly origination attempt number
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
LOCAL void sDisplayPhoneNumber(HCONTROL hControl)  
{
#ifndef SKTT_RELEASE    
    //hjcho-test
    //STRCPY(l_CallInfo.num.buf, (BYTE*)"0112935255");
    //l_CallInfo.num.len = sizeof(l_CallInfo.num.buf);
    //hjcho-test
#endif//SKTT_RELEASE
    
    l_CallInfo.num.buf[l_CallInfo.num.len] = '\0';    
#ifdef  FEATURE_IP_AIRDATA      
    if (!SKY_GET_IP_AIRDATAMANAGER())
#endif
        {
            EQC_SetNumberEditValue(hControl, (BYTE*)l_CallInfo.num.buf);   
            EQC_SetNumberEditPos(hControl, l_CallInfo.num.len);
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
LOCAL void sDisplaySomePhoneNumber(HCONTROL hControl)
{ 
   EDITORINFO_T    CurrNumInfo;   

   if(l_PausedMode == PM_NONE)         /* Paused Mode가 아닐 경우 */
   {
      CurrNumInfo = GET_NUMEDIT;
      CurrNumInfo.aBuf[CurrNumInfo.nPos] = '\0';
      if(CurrNumInfo.nPos > 0)
      {
        EQC_SetNumberEditValue(hControl, (BYTE*)CurrNumInfo.aBuf);    
      }
   }
   else                                   /* Paused Mode일 경우      */
   {
     CurrNumInfo = GET_NUMEDIT;
     CurrNumInfo.aBuf[GetRealPhoneNumberLength()] = '\0';
        
     if(GetRealPhoneNumberLength() > 0)
     {         
         EQC_SetNumberEditValue(hControl, (BYTE*)CurrNumInfo.aBuf);    
         EQC_SetNumberEditPos(hControl, GetRealPhoneNumberLength());
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
STATIC CALLFROMWAP_E  gCallFromWap;



CALLFROMWAP_E sGetCallFromWAP()
{
    return gCallFromWap;
}


void SetCallFromWAP(CALLFROMWAP_E CallFromWhere)
{
    gCallFromWap = CallFromWhere;
}


#ifdef INCOMING_CALL_IN_WAP
extern BOOL g_fWAP_RET_CALL;
#endif //INCOMING_CALL_IN_WAP


#ifdef FEATURE_GVM
extern BOOL g_fGVMPaused;
BOOL CallEndedByPhoneCloseInGVM = FALSE;

BOOL SKY_IS_CALL_ENDED_BY_PHONECLOSE_INGVM()
{
    return CallEndedByPhoneCloseInGVM;
}

void SKY_SET_CALL_ENDED_BY_PHONECLOSE_INGVM(BOOL fFlag)
{
   CallEndedByPhoneCloseInGVM = fFlag;
}
#endif //FEATURE_GVM

#ifdef FEATURE_WAVELET
#define SVC_IDLE					0
extern int g_wsvcType;
BOOL CallEndedByPhoneCloseInWavelet = FALSE;

BOOL SKY_IS_CALL_ENDED_BY_PHONECLOSE_INWAVELET()
{
    return CallEndedByPhoneCloseInWavelet;
}

void SKY_SET_CALL_ENDED_BY_PHONECLOSE_INWAVELET(BOOL fFlag)
{
   CallEndedByPhoneCloseInWavelet = fFlag;
}
#endif // FEATURE_WAVELET

#ifdef FEATURE_SKVM
BOOL CallEndedByPhoneCloseInSKVM = FALSE;

BOOL SKY_IS_CALL_ENDED_BY_PHONECLOSE_INSKVM()
{
    return CallEndedByPhoneCloseInSKVM;
}

void SKY_SET_CALL_ENDED_BY_PHONECLOSE_INSKVM(BOOL fFlag)
{
   CallEndedByPhoneCloseInSKVM = fFlag;
}
#endif
 
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
LOCAL void SKY_API sCallEndSetup(void)
{
    STATIC BOOL fNormalVoiceCall;

    fNormalVoiceCall = TRUE;


    ui_stop_fwd_burst_dtmf();
    ClearNumEditInfo();
    l_fKeyDownInAlert = FALSE;
    SetAutoAnswering(FALSE);
    ClearCLI();
    ClearAlphaEditInfo();
    SKY_SET_TRANSMIT_DTMF(FALSE);

    /* SKY_IsInCall()에서는 l_CallInfo.cm_info.call_sate라는 변수의 상태를 보고    */
    /* TRUE, FALSE 값을 반환한다. 에러가 생기는 경우를 방지하기 위해서 명시적으로 */
    /* 값을 할당한다.                                                             */
    /* Call이 끊겼을 경우에는 CM_CALL_STATE_IDLE이다.                             */
    l_CallInfo.cm_info.call_state = CM_CALL_STATE_IDLE;



#ifdef INCOMING_CALL_IN_WAP
    if(g_fWAP_RET_CALL == TRUE)
    {
       g_fWAP_RET_CALL = FALSE;
#ifdef FEATURE_GVM
       g_fGVMPaused = FALSE;
#endif // FEATURE_GVM
#ifdef FEATURE_WAVELET
       SKY_SET_TO_WAVELET(TRUE);
#endif //FEATURE_WAVELET
       fNormalVoiceCall = FALSE;

       /* WAP으로 넘어가면서 ID LCD를 default로 바꾸어 준다. */
       SKY_SubLCDDefault();

       if(sGetCallFromWAP() == CALL_SKA_WAPEXIT)
       {
           ;
        // EQS_START_SIBLING_EVENT(SKA_WAPEXIT, SE_WAP_RET_CALL, EQS_NA, EQS_NA, FALSE);
       }
       else if(sGetCallFromWAP() == CALL_SKA_WAPLOADING)
       {
           ;
         //EQS_START_SIBLING_EVENT(SKA_WAPLOADING, SE_WAP_RET_CALL, EQS_NA, EQS_NA, FALSE);
       }
       else if(sGetCallFromWAP() == CALL_SKA_WAPHANDLE_KEY)
       {
          EQS_START_SIBLING_EVENT(SKA_WAPHANDLE_KEY, SE_APP_RESUME, EQS_NA, EQS_NA, FALSE);
       }
       else if(sGetCallFromWAP() == CALL_SKA_WAPINPUT)
       {
           ;
         //EQS_START_SIBLING_EVENT(SKA_WAPINPUT, SE_WAP_RET_CALL, EQS_NA, EQS_NA, FALSE);
       }
       else if(sGetCallFromWAP() == CALL_SKA_WAPNewURL)
       {
           ;
         //EQS_START_SIBLING_EVENT(SKA_WAPNewURL, SE_WAP_RET_CALL, EQS_NA, EQS_NA, FALSE);
       }
       else if( sGetCallFromWAP() == CALL_SKA_PHONE_BOOK )
       {
           ;
           //ytchoi 12/14 WAP상태에 phonebook 메뉴에서 call받으면 ska_waphandle로 감으로 필요없다.
         //EQS_SetWindowSymbolMode(WTSM_DORMONT_MODE);
         //EQS_START_SIBLING_EVENT(SKA_BookInCalling, SE_APP_RESUME, EQS_NA, EQS_NA, FALSE);
       }
       else ;
    }
#endif //INCOMING_CALL_IN_WAP

/* rosa 01-07-12 --> @why, what */
#ifdef FEATURE_GVM
    if(g_fGVMPaused == TRUE)    // 음성호 착발신 후 되돌아올때
    {
#ifdef INCOMING_CALL_IN_WAP
            g_fWAP_RET_CALL = FALSE;
#endif//INCOMING_CALL_IN_WAP       

        if(!SKY_IS_CALL_ENDED_BY_PHONECLOSE_INGVM())
        {
            fNormalVoiceCall = FALSE;
            SKY_SubLCDDefault();
            /* undersea 2001-08-23 --> to start gvm */
            EQS_END();	
            /* <-- */
        }
        else
        {
            SKY_SET_CALL_ENDED_BY_PHONECLOSE_INGVM(FALSE);
            fNormalVoiceCall = TRUE;  
			/* undersea 2001-10-08 --> to stop gvm */
			g_fGVMPaused = FALSE;
			SKY_SET_FROM_GVM(FALSE);
			SKY_SET_TO_GVM(FALSE);
			/* <-- */
        }
    }
#endif //FEATURE_GVM
#ifdef FEATURE_WAVELET
	if (SKY_IS_FROM_WAVELET())
	{
        if(!SKY_IS_CALL_ENDED_BY_PHONECLOSE_INWAVELET())
        {
            fNormalVoiceCall = FALSE;
            SKY_SubLCDDefault();
            /* undersea 2001-08-23 --> to start gvm */
            /* start the GVM */
            //EQS_END();
            SKY_SET_FROM_WAVELET(FALSE);
			SKY_SET_TO_WAVELET(TRUE);
            if (g_wsvcType == SVC_IDLE) 
			{
                EQS_START_SIBLING(SKA_IdleWavelet, FALSE);
            }
			else
			{
                EQS_START_SIBLING(sSKA_MainOnWavelet, FALSE);
			}
        }
        else
        {
            SKY_SET_CALL_ENDED_BY_PHONECLOSE_INWAVELET(FALSE);
            fNormalVoiceCall = TRUE;  
        }
	}
#endif //FEATURE_WAVELET
/* <-- */

#ifdef FEATURE_SKVM
#ifndef WIN32
        if ((GET_SKVM_STATE == XVMSTATE_FROZEN) && !SKVM_IS_PLAY_BG())
        {
            if(!SKY_IS_CALL_ENDED_BY_PHONECLOSE_INSKVM())
            {
                fNormalVoiceCall = FALSE;
                SKY_SubLCDDefault();
				EQS_SetDirectDraw(TRUE);
				if (!SKVM_IS_PLAY_BG())
	                EQS_START_SIBLING(SKA_MainSKVM, FALSE);
				else EQS_START_SIBLING(SKA_MainSKVMBG, FALSE);
            }
            else
            {
                SKY_SET_CALL_ENDED_BY_PHONECLOSE_INSKVM(FALSE);
                fNormalVoiceCall = TRUE;
				if (!SKVM_IS_PLAY_BG())
	                EQS_START_SIBLING_EVENT(SKA_MainSKVM, SE_SKVM_STOPPED,EQS_NA,EQS_NA,FALSE);
				else EQS_START_SIBLING_EVENT(SKA_MainSKVMBG, SE_SKVM_STOPPED,EQS_NA,EQS_NA,FALSE);
		    }
        }
#endif
#endif

    if(fNormalVoiceCall == TRUE)
        {
      switch (l_CallInfo.cm_info.end_status) 
      {
        case CM_CALL_END_OFFLINE:
            /* does not display anything here, taken care by other states */
            sCallEndDispExit();
            sExitCallState();
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
        
        case CM_CALL_END_CDMA_LOCK:
            sCallEndDispExit();
            sExitCallState();
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;
      
        case CM_CALL_END_NO_SRV:
        case CM_CALL_END_FADE:
        case CM_CALL_END_INTERCEPT:
        case CM_CALL_END_REORDER:
            if(SKY_IS_DEBUG_SCREEN() == FALSE)
            {
                EQS_START_SIBLING(sSKA_CallEndDisp, TRUE);
            }
            else
            {
                sCallEndDispExit();
                sExitCallState();
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            break;
      
        case CM_CALL_END_REL_SO_REJ:
#ifdef FEATURE_DS
              if (sIsData()) 
              {
                if(SKY_IS_DEBUG_SCREEN() == FALSE)
                {
                    EQS_START_SIBLING(sSKA_CallEndDisp, TRUE);
                }
                else
                {
                    sCallEndDispExit();
                    sExitCallState();
                    EQS_END_TO(EQS_POP_TO_ROOT);
                }
              }
              else
#endif      
              {
                sCallEndDispExit();
                sExitCallState();
                EQS_END_TO(EQS_POP_TO_ROOT);
              }
            break;
      
        case CM_CALL_END_REL_NORMAL:
        case CM_CALL_END_CLIENT_END:
            // normal call end
            if (l_CallInfo.connect_time == 0) 
            {
              /* Exit call state right away if call not connected at all
              ** Stop all the sound before existing call state 
              */
              sExitCallState();
              sCallEndDispExit();
              EQS_END_TO(EQS_POP_TO_ROOT);
            } 
            else
            {
              if(SKY_IS_DEBUG_SCREEN() == FALSE)
              {
                EQS_START_SIBLING(sSKA_CallEndDisp, TRUE);
              }
              else
              {
                sCallEndDispExit();
                sExitCallState();
                EQS_END_TO(EQS_POP_TO_ROOT);
              }
            }
            break;
      
        case CM_CALL_END_INCOM_CALL:
            sCallEndDispExit();
            sExitCallState();
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;

        case CM_CALL_END_ALERT_STOP:       
            if(ISVOICE(l_CallInfo.cm_info.call_type))
            {
                NewMissedCall(); /* jrkwon : update miss call count */
            }
            sCallEndDispExit();
            sExitCallState();
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;

        case CM_CALL_END_ACTIVATION:
            sCallEndDispExit();
            sExitCallState();
            EQS_END_TO(EQS_POP_TO_ROOT);
            break;

        case CM_CALL_END_MC_ABORT:
#ifdef FEATURE_IS95B_NDSS
        case CM_CALL_END_NDSS_FAIL:
#endif
#if (defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900))
        case CM_CALL_END_MAX_ACCESS_PROBE:
#endif
           if (l_CallInfo.connect_time == 0) 
           {
              sCallEndDispExit();
              sExitCallState();
              EQS_END_TO(EQS_POP_TO_ROOT);
           } 
           else
           {
             if(SKY_IS_DEBUG_SCREEN() == FALSE)
             {
               EQS_START_SIBLING(sSKA_CallEndDisp, TRUE);
             }
             else
             {
               sCallEndDispExit();
               sExitCallState();
               EQS_END_TO(EQS_POP_TO_ROOT);
             }
           }
            break;

        default:
           l_CallInfo.cm_info.end_status = CM_CALL_END_REL_NORMAL;
           if (l_CallInfo.connect_time == 0) 
           {
               sCallEndDispExit();
               sExitCallState(); 
               EQS_END_TO(EQS_POP_TO_ROOT);
           } 
           else
           {
             if(SKY_IS_DEBUG_SCREEN() == FALSE)
             {             
               EQS_START_SIBLING(sSKA_CallEndDisp, TRUE);
             }
             else
             {
               sCallEndDispExit();
               sExitCallState();
               EQS_END_TO(EQS_POP_TO_ROOT);
             }
           }
           break;
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
BYTE  l_szMyNumber[12];
SKY_API BYTE* GetMyNumber(void)
{
   nv_dir_number_type  nv_dir_num;
   UINT8               iLoop = 0, jLoop;

   nv_dir_num.nam = SKY_GET_CURRENT_NAM();
   (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );

   if( nv_dir_num.dir_number[0] != '0' ) 
   { /* 4자리 국번 011-1234-5678 */
     l_szMyNumber[0] = '0';
     iLoop = 1;

     for(jLoop = 0; jLoop < NV_DIR_NUMB_SIZ; iLoop++,jLoop++)
     {
       l_szMyNumber[iLoop] = nv_dir_num.dir_number[jLoop];
     }
   }
   else
   {
     for(iLoop = 0; iLoop < NV_DIR_NUMB_SIZ; iLoop++)
     {
       l_szMyNumber[iLoop] = nv_dir_num.dir_number[iLoop];
     }
   }

   return l_szMyNumber;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sBurstDTMFWithMyNumber()
**
** Description: 
**     자기번호 발신을 위해 발신할 DTMF 번호를 자신의 번호로 Setting 해 주는 
**     함수
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
LOCAL void SKY_API sBurstDTMFWithMyNumber(void)
{
   nv_dir_number_type  nv_dir_num;
   UINT8               iLoop = 0, jLoop;
   EDITORINFO_T        CurrNumInfo;

   CurrNumInfo = GET_NUMEDIT;

   nv_dir_num.nam = SKY_GET_CURRENT_NAM();
   (void) SKY_GetNV(NV_DIR_NUMBER_I, (nv_item_type *)&nv_dir_num );

   if( nv_dir_num.dir_number[0] != '0' ) 
   { /* 4자리 국번 011-1234-5678 */
     CurrNumInfo.aBuf[0] = '0';
     iLoop = 1;

     for(jLoop = 0; jLoop < NV_DIR_NUMB_SIZ; iLoop++,jLoop++)
     {
       CurrNumInfo.aBuf[iLoop] = nv_dir_num.dir_number[jLoop];
     }
     CurrNumInfo.nPos = iLoop;
   }
   else
   {
     for(iLoop = 0; iLoop < NV_DIR_NUMB_SIZ; iLoop++)
     {
       CurrNumInfo.aBuf[iLoop] = nv_dir_num.dir_number[iLoop];
     }
     CurrNumInfo.nPos = iLoop;
   }
   CurrNumInfo.fSecret = FALSE;
   SET_NUMEDIT(CurrNumInfo);
   sInitBurstString(GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS, GET_NUMEDIT_SECRET);
   sBurstAndAdvancePause();
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
#define sCONVERSATION_VOLUMETITLE_XPOS    20//29
#define sCONVERSATION_VOLUMETITLE_YPOS    (31 - SKY_ANNUNCIATORS_HEIGHT)

#define sCONVERASTION_VOLUME_BAR_XPOS     49//47
#define sCONVERASTION_VOLUME_BAR_YPOS     (61/*55*/ - SKY_ANNUNCIATORS_HEIGHT)

#define sCONVERSATION_PHONEIMAGE_XPOS     26 //27
#define sCONVERSATION_PHONEIMAGE_YPOS     (77/*71*/ - SKY_ANNUNCIATORS_HEIGHT)


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitConversationalVolumeDisp()
**
** Description: 
**     Adjust volume level in conversation
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
LOCAL void SKY_API sInitConversationalVolumeDisp(UINT8 nCurrentVol)
{
   BITMAP     Bitmap;
   HCONTROL   hControl;
   BYTE       pszBuf[20];
        
   SKY_SetVoiceVolume(nCurrentVol);

   //hControl = EQC_StaticText(sCONVERSATION_VOLUMETITLE_XPOS,
   //                          sCONVERSATION_VOLUMETITLE_YPOS,
   //                          TKN_NULL);
   //sprintf((char*)pszBuf, "%s%s%s%s%s%s", SF_CALL_VOL1_6, SF_CALL_VOL2_6,
   //                                       SF_CALL_VOL3_6, SF_CALL_VOL4_6,
   //                                       SF_CALL_VOL5_6, SF_CALL_VOL6_6);
   //EQC_SetStaticText(hControl, (BYTE*) pszBuf);
   
   EQC_Image(sCONVERSATION_VOLUMETITLE_XPOS, 
		     sCONVERSATION_VOLUMETITLE_YPOS, 
			 (BITMAP*)&g_BmpCallPannelVolumeInCall);

   EQS_PutBitmap(sCONVERSATION_PHONEIMAGE_XPOS, sCONVERSATION_PHONEIMAGE_YPOS, &g_BmpPhoneOpen);  

   Bitmap = g_BmpVolumeInCall;
   Bitmap.pbData = g_BmpVolumeInCall.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nCurrentVol;
   EQS_PutBitmap(sCONVERASTION_VOLUME_BAR_XPOS, sCONVERASTION_VOLUME_BAR_YPOS, &Bitmap);    
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sUpdateCurrentVolumeStatus()
**
** Description: 
**     Adjust volume level in conversation
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
LOCAL void SKY_API sUpdateCurrentVolumeStatus(UINT8 nCurrentVol)
{
   BITMAP Bitmap;
        
   SKY_SetVoiceVolume(nCurrentVol);
   EQS_PutBitmap(sCONVERSATION_PHONEIMAGE_XPOS, sCONVERSATION_PHONEIMAGE_YPOS, &g_BmpPhoneOpen);

   Bitmap = g_BmpVolumeInCall;
   Bitmap.pbData = g_BmpVolumeInCall.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * nCurrentVol;
   EQS_PutBitmap(sCONVERASTION_VOLUME_BAR_XPOS, sCONVERASTION_VOLUME_BAR_YPOS, &Bitmap);   
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_AdjstconversationVol()
**
** Description: 
**     Adjust volume level in conversation
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
#define MAX_CONVERSATION_VOLUME        5
#define MIN_CONVERSATION_VOLUME        1

void SKY_API SKA_AdjstConversationVol(SKY_EVENT_T* pEvent)
{
    STATIC UINT8   nCurrentVol;
    STATIC BOOL    fUpDownKey;
    HCONTROL       hControl;    

        switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE_VOLUME_NONE, 
                      NULL,                  /* show annunciator */
                      WA_SHOW_HEADER|WA_SHOW_CML);
        nCurrentVol = SKY_GET_SPEAKER_VOLUME(); 
        sInitConversationalVolumeDisp(nCurrentVol);
        fUpDownKey = FALSE;
        EQS_StartTimer(EQS_TIMER_1, sTWO_SECOND, TRUE);

#ifdef FEATURE_SKY_SUBLCD
        EQS_StartTimer(EQS_TIMER_2, sONE_SECOND, TRUE);
#endif//FEATURE_SKY_SUBLCD
        SKY_SET_KEY_TONE(FALSE);
        break;

    case SE_APP_END : /* The very last event */
        SKY_SET_KEY_TONE(TRUE);
        EQS_ReleaseDC();
        break;    

    case SE_APP_DRAW :
        sUpdateCurrentVolumeStatus(nCurrentVol);
        break;

    case SE_KEY_CENTER:
    case SE_SOFTKEY_VOLUME:
        EQS_END();
        break;

    case SE_KEY_UP:
        if(nCurrentVol < MAX_CONVERSATION_VOLUME) nCurrentVol++;
        else nCurrentVol = MAX_CONVERSATION_VOLUME;

        fUpDownKey = TRUE;              
        EQS_SEND_EVENT(SE_APP_DRAW, 0, 0);
        break;

    case SE_KEY_DOWN:
        if(nCurrentVol > MIN_CONVERSATION_VOLUME) nCurrentVol--;
        else nCurrentVol = MIN_CONVERSATION_VOLUME;

        fUpDownKey = TRUE;
        EQS_SEND_EVENT(SE_APP_DRAW, 0, 0);
        break;

    case SE_TM_TIMER1 ://UI_TIMER_F:
        if(fUpDownKey == FALSE)
        {
          EQS_END();
        }
        else
        {
          fUpDownKey = FALSE;
        }
        break;    

    case SE_TM_TIMER2:
#ifdef FEATURE_SKY_SUBLCD
        {
          clk_julian_type julian;

          SKY_GetCallingTime(&julian);
          SKY_DisplayCallingTimeInSubLCD(&julian);
        }
#endif//#ifdef FEATURE_SKY_SUBLCD
        break;

        case SE_KEY_RECORD:
    case SE_KEY_0 ://HS_0_K:
    case SE_KEY_1 ://HS_1_K:
    case SE_KEY_2 ://HS_2_K:
    case SE_KEY_3 ://HS_3_K:
    case SE_KEY_4 ://HS_4_K:
    case SE_KEY_5 ://HS_5_K:
    case SE_KEY_6 ://HS_6_K:
    case SE_KEY_7 ://HS_7_K:
    case SE_KEY_8 ://HS_8_K:
    case SE_KEY_9 ://HS_9_K:
    case SE_KEY_STAR ://HS_STAR_K:
    case SE_KEY_POUND ://HS_POUND_K:
    case SE_KEY_PHONE_OPEN://HS_EAR_UP_K:
    case SE_KEY_LEFT ://HS_PF1_K:
    case SE_KEY_RIGHT ://HS_PF2_K:
    case SE_KEY_CLEAR :// HS_CLR_K:
    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
    case SE_KEY_SEND ://HS_SEND_K:
        EQS_END();
         break;


    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_END :
    case SE_CALL_ENDED:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        DataManagerEnd(pEvent);
        break;

    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSendCallNumber(BYTE* pszNumber)
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
BYTE l_abPhoneNumberBuffer[EQS_MAX_DIAL_DIGITS+5];

LOCAL void SKY_API sSendCallNumber(BYTE* pszNumber)
{
   STRCPY(l_abPhoneNumberBuffer, pszNumber);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     GetCallNumber(CHAR* pszNumber, UINT8 nLength)
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
LOCAL void SKY_API sGetCallNumber(BYTE* pszNumber)
{
   STRCPY(pszNumber, l_abPhoneNumberBuffer);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     IsSpace2RecordAutoAnswer()
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
BOOL SKY_API IsSpace2RecordAutoAnswer(void)
{
    VMEMOINFO_T*    VoiceInfo;
    
    VoiceInfo = GetVoiceInfo();
    if(((NV_MAX_FL_FILE_NUM-2)   >= VoiceInfo->bTotalRecNumber) &&
      (GetVoiceMemoRemainTime()  >= MIN_AUTO_ANSWER_SETTING_TIME) &&
      (VoiceInfo->dwRemainedByte > VM_MIN_REMAINED_BYTE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
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
#define sCALL_FORWARD_PENDING_TITLE_YPOS              (sMESSAGE_BASE+3)

LOCAL void sInitNumberEditingInCallForward(void)
{
    HCONTROL hNumberControl;

    EQC_StaticText(sALERT_MESSAGE_XPOS, sCALL_FORWARD_PENDING_TITLE_YPOS, (EQS_TOKEN)TKN_CALL_CALLFORWARD);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
			  (BITMAP*)&g_BmpCallPannel);

    hNumberControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                    DIAL_DIGIT_POS_Y, 
                                    DIAL_DIGIT_LENGTH_PER_LINE, 
                                    DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
    
    EQC_SetNumberEditBackColor(hNumberControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hNumberControl, NUMBER_TEXT_COLOR);            

    EQC_SET_STYLE(hNumberControl, CS_NUMBER_EDIT_RIGHT2LEFT | 
                                  CS_NUMBER_EDIT_ALONE      );
                                  //CS_NUMBER_EDIT_ORING      |
                                  //CS_NUMBER_EDIT_DRAW_ALL_CONTROL);
    EQC_SetNumberEditBigFont(hNumberControl, TRUE);   // Set Big Font
    EQS_SetIOCActive(hNumberControl);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_CallForward(SKY_EVENT_T *pEvent)
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
void SKY_API SKA_CallForward(SKY_EVENT_T *pEvent)
{
    UINT8          nNumDigit;
    BYTE           aPhoneNumber[EQS_MAX_DIAL_DIGITS];
    HCONTROL       hControl;
    STATIC BYTE    aTmpBuf[EQS_MAX_DIAL_DIGITS];    
    STATIC BOOL    fInput;
    
    
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
            (WA_SHOW_HEADER | WA_SHOW_CML));
        
        sInitNumberEditingInCallForward(); 
        fInput = FALSE;
        SKY_SET_TRANSMIT_DTMF(FALSE);
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    case SE_APP_END :    /* The very last event */
        SKY_SET_TRANSMIT_DTMF(TRUE);
        EQS_ReleaseDC(); 
        break;

    case SE_SYS_TO_IDLE: /* Ignore this event   */
        break;
    
    case SE_IO_NEDIT_CANCEL:
        EQS_END();
        break;

    case SE_CALL_COMMAND_ERROR:
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END :
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        DataManagerEnd(pEvent);
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_END();
        break;

    case SE_IO_NEDIT_DONE:
        if(fInput == FALSE)
        {
            fInput = TRUE;
            EQS_SetIOCActive(EQS_GetNthIOC(CF_NUMEDIT));
            EQC_SET_STYLE(EQS_GetNthIOC(CF_NUMEDIT), 
            EQC_GET_STYLE(EQS_GetNthIOC(CF_NUMEDIT)) | CS_NUMBER_EDIT_IGNORE_OKKEY);                                            
            nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(CF_NUMEDIT), aPhoneNumber);
            
            if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
            {
               strcpy((CHAR*)aTmpBuf, (CHAR*)SKY_GET_011SERVICE_FORWARD_HOT());            
            }
            else
            {                
               strcpy((CHAR*)aTmpBuf, (CHAR*)SKY_GET_S017_FORWARD_HOT());
            }
            strcat((CHAR*)aTmpBuf, (CHAR*)aPhoneNumber);
            sSendCallNumber(aTmpBuf);    
            EQS_START_CHILD(SKA_FlashWithInfo);
        }
        break;
    
    
    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}
//#endif // FEATURE_IS95B_INC_CALL_FWD_ENH

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
LOCAL void sInitNumberEditingInCallPending(void)
{
    HCONTROL hNumberControl;

    EQC_StaticText(sALERT_MESSAGE_XPOS, sCALL_FORWARD_PENDING_TITLE_YPOS, (EQS_TOKEN)TKN_CALL_CALLPENDING);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
			  (BITMAP*)&g_BmpCallPannel);

    hNumberControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                                    DIAL_DIGIT_POS_Y, 
                                    DIAL_DIGIT_LENGTH_PER_LINE, 
                                    DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
    
    EQC_SetNumberEditBackColor(hNumberControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hNumberControl, NUMBER_TEXT_COLOR);    
    
    EQC_SET_STYLE(hNumberControl, CS_NUMBER_EDIT_RIGHT2LEFT  |
                                  CS_NUMBER_EDIT_ALONE       );
//                                  CS_NUMBER_EDIT_ORING       |
//                                  CS_NUMBER_EDIT_DRAW_ALL_CONTROL);
    EQC_SetNumberEditBigFont(hNumberControl, TRUE);   // Set Big Font
    EQS_SetIOCActive(hNumberControl);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_CallPending()
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
void SKY_API SKA_CallPending(SKY_EVENT_T *pEvent)
{
    UINT8          nNumDigit;
    HCONTROL       hControl;
    BYTE           aPhoneNumber[EQS_MAX_DIAL_DIGITS];
    STATIC BYTE    aTmpBuf[EQS_MAX_DIAL_DIGITS];
    STATIC BOOL    fInput;

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
            (WA_SHOW_HEADER | WA_SHOW_CML));
        sInitNumberEditingInCallPending();
        SKY_SET_TRANSMIT_DTMF(FALSE);
        fInput = FALSE;
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    case SE_APP_DRAW:
//        DrawGeneralFrame();
        break;

    case SE_APP_END :   /* The very last event */
        SKY_SET_TRANSMIT_DTMF(TRUE);
        EQS_ReleaseDC(); 
        break;

    case SE_SYS_TO_IDLE: /* Ignore this event   */
        break;
    
    case SE_TM_TIMER1 :
    case SE_IO_NEDIT_CANCEL:
        EQS_END();
        break;

    case SE_CALL_COMMAND_ERROR:
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END :
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        DataManagerEnd(pEvent);
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_END();
        break;

    case SE_IO_NEDIT_DONE :
        if(fInput == FALSE)
        {
            fInput = TRUE;
            EQS_SetIOCActive(EQS_GetNthIOC(CF_NUMEDIT));
            EQC_SET_STYLE(EQS_GetNthIOC(CF_NUMEDIT), 
            EQC_GET_STYLE(EQS_GetNthIOC(CF_NUMEDIT)) | CS_NUMBER_EDIT_IGNORE_OKKEY);                                            
            nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(CF_NUMEDIT), aPhoneNumber);
            strcpy((CHAR*)aTmpBuf, (CHAR*)SKY_GET_011SERVICE_PENDING_HOT());
            strcat((CHAR*)aTmpBuf, (CHAR*)aPhoneNumber);
            sSendCallNumber(aTmpBuf);   
            EQS_START_CHILD(SKA_FlashWithInfo);
        }
        break;    
    
    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_StopCallWaiting()
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
void SKY_API SKA_StopCallWaiting(SKY_EVENT_T *pEvent)
{
    UINT8          nNumDigit;
    HCONTROL       hControl;
    BYTE           aPhoneNumber[EQS_MAX_DIAL_DIGITS];
    STATIC BYTE    aTmpBuf[EQS_MAX_DIAL_DIGITS];
	STATIC BOOL    fInput;

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
            (WA_SHOW_HEADER | WA_SHOW_CML));
        SKY_SET_TRANSMIT_DTMF(FALSE);
		strcpy((CHAR*)aTmpBuf, (CHAR*)SKY_GET_S017_PENDING_HOT()); //*40
		sSendCallNumber(aTmpBuf);   
        EQS_START_CHILD(SKA_FlashWithInfo);
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    case SE_APP_END :   /* The very last event */
        SKY_SET_TRANSMIT_DTMF(TRUE);
        EQS_ReleaseDC(); 
        break;

    case SE_SYS_TO_IDLE: /* Ignore this event   */
        break;
    
    case SE_TM_TIMER1 :
        EQS_END();
        break;

    case SE_CALL_COMMAND_ERROR:
		break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END :
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        DataManagerEnd(pEvent);
        break;

    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sExitCallState()
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
LOCAL void SKY_API sExitCallState(void)
{
  if(l_fCallFromAlert == TRUE)
  {
     l_fCallFromAlert = FALSE;
  }

  if(l_fAutoAnswerFromAlert == TRUE)
  {
     if(l_fAutoAnswerAlready)
     {
            l_fAutoAnswerFromAlert = FALSE;
     }
     else
     {
            SKY_SET_AUTO_ANSWER(FALSE);
            l_fAutoAnswerFromAlert = FALSE;
     }       
  }  
  SetAutoAnswering(FALSE);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetCallTimeString()
**
** Description: 
**     Call Ending Display
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
#define NUM_BLINK_CALLTIME        6
#define SIZE_CURRENTTIME_STRING  20


LOCAL void sSetCallTimeString(BYTE abCallTimeString[])
{
    BYTE szCallTime[sMAX_CALL_TIME_LENGTH];
    clk_julian_type julian;

    clk_secs_to_julian( clk_uptime()-l_CallInfo.connect_time, &julian);

#ifdef FEATURE_SKY_SUBLCD
    SKY_DisplayCallingTimeInSubLCD(&julian);
#endif//#ifdef FEATURE_SKY_SUBLCD
    sprintf((char *)szCallTime, (char*)SKY_GET_TOKEN(TKN_CALL_TIMER_FMT), 
    julian.hour, julian.minute, julian.second);
    STRCPY(abCallTimeString, szCallTime);
}


/*
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetCallTimeString()
**
** Description: 
**     Call Ending Display
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

LOCAL BYTE* sGetCallTimeString()
{
   return l_szCallTime;
}




/*
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_CallEndDisp()
**
** Description: 
**     Call Ending Display
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
LOCAL void SKY_API sSKA_CallEndDisp(SKY_EVENT_T *pEvent)
{
    STATIC UINT8  nCount;
    STATIC BYTE   abCurrentTimeString[SIZE_CURRENTTIME_STRING];
    STATIC BYTE   abCallTimeString[sMAX_CALL_TIME_LENGTH];
    STATIC HCONTROL hCtrlNoService;

    switch(pEvent->EventID)
    {
    case SE_APP_START :      /* The very first event */
        EQS_GetDC(TRUE);                
        EQS_SetWindow(SKY_DEF_WIN_X,
              SKY_DEF_WIN_Y, 
              SKY_DEF_WIN_DX, 
              SKY_DEF_WIN_DY, 
              CL_NONE, 
              NULL,    /* show annunciator */
              WA_SHOW_HEADER);
                

        switch (l_CallInfo.cm_info.end_status) 
        {
            case CM_CALL_END_NO_SRV:
               hCtrlNoService =  EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_SERVICE), 
                                                        MESSAGE_DEFAULT_TIME, 
                                                        (MESSAGE_BOX_TYPE_E)  MBT_NONE, 
                                                        (MESSAGE_BOX_TITLE_E) MBT_SYSTEM);
                break;

            case CM_CALL_END_FADE:
            case CM_CALL_END_INTERCEPT:
            case CM_CALL_END_REORDER:
               sCallEndDispExit();
               EQS_END_TO(EQS_POP_TO_ROOT);
                break;

            default:
                /* 통화시간을 깜빡 거려주어야 한다. */
                sMakeDateTimeString(TRUE, abCurrentTimeString);
                memcpy(abCallTimeString, sGetCallTimeString(), sMAX_CALL_TIME_LENGTH);
                abCallTimeString[sMAX_CALL_TIME_LENGTH] = '\0';
                nCount = 0;
                EQS_SetWindowCML(CL_NONE);

                if(sIsData()) /* 데이터 서비스의 경우 */
                {
                    EQC_SetStaticText(EQS_GetNthIOC(DS_TIME), abCallTimeString);
                }
                else if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
                {
                    if(l_fCallFromAlert == TRUE)
                    {
                        EQC_SetStaticText(EQS_GetNthIOC(IDCT_CURRENT_TIME), abCurrentTimeString);
                        EQC_SetStaticText(EQS_GetNthIOC(IDCT_CALL_TIME), abCallTimeString);
                    }
                    else
                    {               
                        EQC_SetStaticText(EQS_GetNthIOC(IFO_CURRENT_TIME), abCurrentTimeString);
                        EQC_SetStaticText(EQS_GetNthIOC(IFO_CALL_TIME), abCallTimeString);
                    }                                   
                }
                EQS_StartTimer(EQS_TIMER_1, 300, TRUE);
                break;
        } /* switch l_CallInfo.cm_info.end_status */
        break;

    case SE_APP_END :
        sExitCallState();
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :
//        if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
//        {
//            /* 착신인 경우에는 Frame을 그려주지 않는다 */
//            if(l_fCallFromAlert == FALSE)
//            {
//                DrawGeneralFrame();
//            }
//        }
//        else if(sIsData())
//        {
//            DrawGeneralFrame();
//        }
        break;

    case SE_TM_TIMER1 ://UI_TIMER_F:
        nCount++;

        if (nCount % 2 != 0)
        {
            if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
            {
                if(l_fCallFromAlert == TRUE)
                {
                    EQC_SetStaticText(EQS_GetNthIOC(IDCT_CALL_TIME), abCallTimeString);
                }
                else
                {
                    EQC_SetStaticText(EQS_GetNthIOC(IFO_CALL_TIME), abCallTimeString);
                }           
            } /* 데이터 서비스일 경우 */
            else if(sIsData())
            {
                EQC_SetStaticText(EQS_GetNthIOC(DS_TIME), abCallTimeString);
            }
        }
        else
        {
            if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
            {
                if(l_fCallFromAlert == TRUE)
                {
                    EQC_SetStaticText(EQS_GetNthIOC(IDCT_CALL_TIME), SKY_GET_TOKEN(TKN_NULL));
                }
                else
                {
                    EQC_SetStaticText(EQS_GetNthIOC(IFO_CALL_TIME), SKY_GET_TOKEN(TKN_NULL));
                }
            }
            else if(sIsData()) /* 데이터 서비스 일 경우 */
            {
                EQC_SetStaticText(EQS_GetNthIOC(DS_TIME), SKY_GET_TOKEN(TKN_NULL));
            }
        }
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);

        if(nCount == NUM_BLINK_CALLTIME)
        {
            sCallEndDispExit();
            EQS_StopTimer(EQS_TIMER_1);          
            EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    //case SE_IO_MESGBOX_OK:
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam != hCtrlNoService)
        {
            break;
        }
    case SE_KEY_0 :         //HS_0_K:
    case SE_KEY_1 :         //HS_1_K:
    case SE_KEY_2 :         //HS_2_K:
    case SE_KEY_3 :         //HS_3_K:
    case SE_KEY_4 :         //HS_4_K:
    case SE_KEY_5 :         //HS_5_K:
    case SE_KEY_6 :         //HS_6_K:
    case SE_KEY_7 :         //HS_7_K:
    case SE_KEY_8 :         //HS_8_K:
    case SE_KEY_9 :         //HS_9_K:
    case SE_KEY_STAR :      //HS_STAR_K:
    case SE_KEY_POUND :     //HS_POUND_K:
    case SE_KEY_UP :        //HS_UP_K:
    case SE_KEY_DOWN :      //HS_DOWN_K:
    case SE_KEY_PHONE_OPEN :     //HS_EAR_UP_K:
    case SE_KEY_PHONE_CLOSE :    //HS_EAR_DOWN_K:
    case SE_SOFTKEY_LEFT :  //HS_PF1_K:
    case SE_SOFTKEY_RIGHT : //HS_PF2_K:
    case SE_KEY_CLEAR :     //HS_CLR_K:
    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
    case SE_KEY_POWER_OFF : //HS_PWR_OFF_K:
    case SE_KEY_SEND :      //HS_SEND_K:
    case SE_KEY_END :       //HS_END_K:
    case SE_SYS_INFO :      //UI_INFO_F:
    case SE_SYS_OFFLINE :   //UI_OFFLINE_F:
    case SE_SYS_TO_IDLE :// UI_TOIDLE_F:
        sCallEndDispExit();
        EQS_END_TO(EQS_POP_TO_ROOT);

        /* jrkwon 2001.07.19 */
        g_LastCallKeyEvent = pEvent->EventID;
        
        break;

//    case SE_SOFTKEY_LEFT ://HS_PF1_K:
//        if (l_CallInfo.cm_info.end_status == CM_CALL_END_REL_NORMAL ||
//            l_CallInfo.cm_info.end_status == CM_CALL_END_CLIENT_END ) 
//        {
//            if (!l_CallInfo.secret) 
//            {
//                sCallEndDispExit();
//                EQS_END_TO(EQS_POP_TO_ROOT);
//            }
//        } 
//        else 
//        {
            // quit call state for other cases
//            sCallEndDispExit();
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
//        break;

//    case SE_SOFTKEY_RIGHT ://HS_PF2_K:
//        sCallEndDispExit();
//
//        if (l_CallInfo.cm_info.end_status == CM_CALL_END_REL_NORMAL ||
//            l_CallInfo.cm_info.end_status == CM_CALL_END_CLIENT_END ) 
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        } 
//        else 
//        {
//            EQS_END_TO(EQS_POP_TO_ROOT);
//        }
//        break;

//    case SE_KEY_CLEAR ://HS_CLR_K:
//    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
//    case SE_KEY_POWER_OFF ://HS_PWR_OFF_K:
//    case SE_KEY_SEND ://HS_SEND_K:
//    case SE_KEY_END ://HS_END_K:
//    case SE_KEY_INFO ://HS_INFO_K:
//    case SE_SYS_INFO ://UI_INFO_F:
//    case SE_SYS_OFFLINE ://UI_OFFLINE_F:
//        sCallEndDispExit();               
//        EQS_END_TO(EQS_POP_TO_ROOT);
//        break;
//    case SE_SYS_TO_IDLE :// UI_TOIDLE_F:
//    case SE_TM_TIMER1 ://UI_TIMER_F:
//        sCallEndDispExit();
//        EQS_END_TO(EQS_POP_TO_ROOT);
//        break;


    case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
    case SE_CALL_CALLER_ID:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY :/* UI_CALL_EXT_DISPLAY_E: */
#endif 
        sCallEndDispExit();
        CallDefaultEventProc(pEvent, CALLENDDISPLAY);
//      EQS_START_SIBLING_CURRENT_EVENT(SKA_Alerted,FALSE);
        break;

//    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
//        // not supposed to be here
//        ASSERT(FALSE);
//        break;

        
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//    hjcho Very Important!!!
//
//    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
//    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
//    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
//    case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
//    case SE_CALL_FLASHED ://UI_CALL_FLASHED_E:
//    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
        // not supposed to be here, TOIDLE will
        // drive UI state machine to idle
//        ASSERT(FALSE);
//        break;
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
    case SE_CALL_COMMAND_ERROR:
        sCallEndDispExit();
        EQS_END_TO(EQS_POP_TO_ROOT);            
                break;

    default:
        CallDefaultEventProc(pEvent, CALLENDDISPLAY);
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
void SKY_API InitOrigAttemptDisp(void)
{
    HCONTROL        hControl;
    EDITORINFO_T    CurrNumInfo;   
    //BYTE            sORIGINATION_NAME_XPOS;

    EQC_StaticText(sORIGINATION_MESSAGE_XPOS, 
                   sMESSAGE_BASE, 
                   (EQS_TOKEN)TKN_CALL_ATTEMPT);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
              sORIGINATION_PHONE_OPEN_YPOS, 
              (BITMAP*)&g_BmpPhoneOpen);

    hControl = EQC_Animation(sORIGINATION_ANIMATION_XPOS, 
                             sORIGINATION_ANIMATION_YPOS, 
                             &g_AniOriginatingCall,//&g_AniCall64x39x256c, 
                             NULL);
    EQC_StartAnimation(hControl, TRUE);

    EQC_Image(sORIGINATION_IMAGE_XPOS, 
              sORIGINATION_IMAGE_YPOS, 
              (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );    

    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | 
                            CS_NUMBER_EDIT_NO_CML     |
                            CS_NUMBER_EDIT_ALONE      );
                            //CS_NUMBER_EDIT_ORING      );
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font

    if(l_PausedMode == PM_NONE)
    {
      CurrNumInfo = GET_NUMEDIT;
      CurrNumInfo.aBuf[CurrNumInfo.nPos] = '\0';
#ifdef  FEATURE_IP_AIRDATA      
    if (!SKY_GET_IP_AIRDATAMANAGER())
#endif
      EQC_SetNumberEditValue(hControl, (BYTE*)CurrNumInfo.aBuf);          
    }
    else
    {
      sDisplaySomePhoneNumber(hControl);
    }

    if(GET_ALPHAEDIT_POS > 0)
    {
      //sORIGINATION_NAME_XPOS = SKY_SCREEN_WIDTH/2 - (STRLEN(GET_ALPHAEDIT_BUFFER)*EQS_CharWidth('A'))/2;
        
      hControl = EQC_StaticText(sORIGINATION_NAME_XPOS,
                                sORIGINATION_NAME_YPOS,
                                TKN_NULL);
      EQC_SetStaticText(hControl, (BYTE*) GET_ALPHAEDIT_BUFFER);
    }

    /* 백라이트 항상 켜짐 */
    SKY_SET_NO_OFF_BACKLIGHT(TRUE);
    SKY_BacklightOn(AB_ALWAYS);
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_OrigAttempt()
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
void SKY_API sSKA_OrigAttempt(SKY_EVENT_T *pEvent)
{
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
            WA_SHOW_HEADER);

        SKY_AcceptKey(FALSE);
        InitOrigAttemptDisp();
        break;

    case SE_APP_END : /* The very last event */
        SKY_AcceptKey(TRUE);
        EQS_ReleaseDC();
        break;

   case SE_KEY_END ://HS_END_K:
        EQS_END();
        break;

    case SE_APP_DRAW :                  
        //DrawGeneralFrame();
        break;

    case SE_KEY_0 ://HS_0_K:
    case SE_KEY_1 ://HS_1_K:
    case SE_KEY_2 ://HS_2_K:
    case SE_KEY_3 ://HS_3_K:
    case SE_KEY_4 ://HS_4_K:
    case SE_KEY_5 ://HS_5_K:
    case SE_KEY_6 ://HS_6_K:
    case SE_KEY_7 ://HS_7_K:
    case SE_KEY_8 ://HS_8_K:
    case SE_KEY_9 ://HS_9_K:
    case SE_KEY_STAR ://HS_STAR_K:
    case SE_KEY_POUND ://HS_POUND_K:
    case SE_KEY_UP ://HS_UP_K:
    case SE_KEY_DOWN ://HS_DOWN_K:
    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
    case SE_SOFTKEY_LEFT ://HS_PF1_K:
    case SE_SOFTKEY_RIGHT ://HS_PF2_K:
    case SE_KEY_CLEAR :// HS_CLR_K:
    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
    case SE_KEY_SEND ://HS_SEND_K:
        break;

    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        // ignore the events
        break;

    case SE_TM_TIMER1 ://UI_TIMER_F:
        // not supposed to be here
        ASSERT(FALSE);
        break;


    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        DataManagerEnd(pEvent);
    case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
    case SE_CALL_FLASHED ://UI_CALL_FLASHED_E:
        // not supposed to be here
        ASSERT(FALSE);
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        // there might be race condition, another task could 
        // originate the call right before UI, need to
        // reexamine and setup current call info, and remove
        // the old number displayed in IDLE state
        //(void)uiscrn_deact(UI_NUM_F);
        sInitCurrentCallOnOriginate( &(GET_EVENT_PARAM()->call_event.call_info) );
       //init_curr_call_on_originated( &(ui.evt_param.call_event.call_info) );        
        sOrigAttemptExit();
        EQS_START_SIBLING(sSKA_Originated,FALSE);
        break;

    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
        sOrigAttemptExit();
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;


    case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
        sOrigAttemptExit();
    default:
        CallDefaultEventProc(pEvent, ORIGATTEMPT);
        break;
    }
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     
**
** Description: 
**     Initialize Originate State Display in Voice Call
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
void SKY_API SKA_StartOrigination(SKY_EVENT_T *pEvent)
{
   UINT8            nRealPhoneNumber;
   BYTE             szString[50];
   STATIC HCONTROL  hCtrlNoSrvc;
    
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
            WA_SHOW_HEADER);

        SKY_AcceptKey(FALSE);

        if((SKY_IS_NOSERVICE() == FALSE) && (SKY_IS_MAINTREQ() == FALSE))
        {
            InitOrigAttemptDisp();

            if(IsPauseMode(GET_NUMEDIT_BUFFER))
            {
              nRealPhoneNumber = ParsePhoneNumber(GET_NUMEDIT_BUFFER, szString);
              if((szString[0] == '0')   &&
                                 (szString[1] == '0')   && 
                                 (szString[2] >  '0')   &&
                                 (nRealPhoneNumber > 2) && 
                                 SKY_IS_OVERSEAS_CALL_LOCK())
              {
                EQS_START_SIBLING(SKA_MainInterCode, FALSE);  
              }
              else
              {
                ui_orig_voice_call(szString, nRealPhoneNumber,  
                                           GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
              }
            }
            else
            {
              if(GET_NUMEDIT_BUFFER[0] == '0'  && 
                                 GET_NUMEDIT_BUFFER[1] == '0'  &&
                                 GET_NUMEDIT_BUFFER[2] >  '0'  &&
                                 (GET_NUMEDIT_POS > 2)         && 
                                 SKY_IS_OVERSEAS_CALL_LOCK())
              {
                EQS_START_SIBLING(SKA_MainInterCode, FALSE);
              }
              else
              {
                ui_orig_voice_call( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS, GET_ALPHAEDIT_BUFFER, GET_ALPHAEDIT_POS);
              }
            }        
        }
        else if(SKY_IS_MAINTREQ())
        {
          SKY_SET_ORIGINATE(FALSE);
          ClearNumEditInfo();
          ClearAlphaEditInfo();
          hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_NO_REGISTERED), 
                                     MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
        }
        else if(SKY_IS_NOSERVICE())
        {
          SKY_SET_ORIGINATE(FALSE);
          ClearNumEditInfo();
          ClearAlphaEditInfo();
          hCtrlNoSrvc = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
                                     MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);          
        }
        EQS_SEND_EVENT(SE_USER_1, 0,0);
        break;

    case SE_APP_SUSPEND:
        SKY_AcceptKey(TRUE);
        break;

    case SE_APP_END : /* The very last event */
        SKY_AcceptKey(TRUE);
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW :      
//        DrawGeneralFrame();
        break;

    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
    case SE_KEY_END ://HS_END_K:
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_KEY_0 ://HS_0_K:
    case SE_KEY_1 ://HS_1_K:
    case SE_KEY_2 ://HS_2_K:
    case SE_KEY_3 ://HS_3_K:
    case SE_KEY_4 ://HS_4_K:
    case SE_KEY_5 ://HS_5_K:
    case SE_KEY_6 ://HS_6_K:
    case SE_KEY_7 ://HS_7_K:
    case SE_KEY_8 ://HS_8_K:
    case SE_KEY_9 ://HS_9_K:
    case SE_KEY_STAR ://HS_STAR_K:
    case SE_KEY_POUND ://HS_POUND_K:
    case SE_KEY_UP ://HS_UP_K:
    case SE_KEY_DOWN ://HS_DOWN_K:
    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
    case SE_SOFTKEY_LEFT ://HS_PF1_K:
    case SE_SOFTKEY_RIGHT ://HS_PF2_K:
    case SE_KEY_CLEAR :// HS_CLR_K:
    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
    case SE_KEY_SEND ://HS_SEND_K:
        break;

    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        break;

    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
    case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
    case SE_CALL_FLASHED ://UI_CALL_FLASHED_E:
        // not supposed to be here
        ASSERT(FALSE);
        break;

   
    case SE_IO_MESGBOX_END:
        if(pEvent->wParam == hCtrlNoSrvc)
        {
           EQS_END_TO(EQS_POP_TO_ROOT);
        }
        break;

    default:
      EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitOriginatedDisplayInVoiceCall()
**
** Description: 
**     Initialize Originate State Display in Voice Call
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
LOCAL void SKY_API sInitOriginatedDisplayInVoiceCall(void)
{
    HCONTROL hControl;
    //BYTE     sORIGINATION_NAME_XPOS;

    EQC_StaticText(sORIGINATION_MESSAGE_XPOS, 
                  sMESSAGE_BASE, 
                  (EQS_TOKEN)TKN_CALL_ATTEMPT);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
              sORIGINATION_PHONE_OPEN_YPOS, 
              (BITMAP*)&g_BmpPhoneOpen);

    hControl = EQC_Animation(sORIGINATION_ANIMATION_XPOS, 
                             sORIGINATION_ANIMATION_YPOS, 
                             &g_AniOriginatingCall,//&g_AniCall64x39x256c, 
                              NULL);
    EQC_StartAnimation(hControl, TRUE);

    EQC_Image(sORIGINATION_IMAGE_XPOS, 
              sORIGINATION_IMAGE_YPOS, 
              (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
    //EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);   

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT | 
                            CS_NUMBER_EDIT_NO_CML     |
                            CS_NUMBER_EDIT_ALONE      );
                            //CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
    l_fCallFromAlert = FALSE;

    if(l_PausedMode == PM_NONE)
    {
      sDisplayPhoneNumber(hControl);
    }
    else
    {
      sDisplaySomePhoneNumber(hControl);
    }

    if(l_CallInfo.alpha.len > 0)
    {
      //sORIGINATION_NAME_XPOS = SKY_SCREEN_WIDTH/2 - (STRLEN(l_CallInfo.alpha.buf)*EQS_CharWidth('A'))/2;
        
      hControl = EQC_StaticText(sORIGINATION_NAME_XPOS,
                                sORIGINATION_NAME_YPOS,
                                TKN_NULL);
      EQC_SetStaticText(hControl, (BYTE*) l_CallInfo.alpha.buf);
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitOriginatedDisplayInDataService()
**
** Description: 
**     Initialize Originate State Display in Data Service
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
#define sDATA_SERVICE_TEXT_MARGIN_Y      4

#define sDATA_SERVICE_ANI_X  25
#define sDATA_SERVICE_ANI_Y  9

#define sDATA_SERVICE_FRAME_X   4
#define sDATA_SERVICE_FRAME_Y   (94 - SKY_ANNUNCIATORS_HEIGHT)

#define sDATA_SERVICE_STATE_X   (sDATA_SERVICE_FRAME_X + 5)
#define sDATA_SERVICE_STATE_Y   (sDATA_SERVICE_FRAME_Y + 5)

#define sDATA_SERVICE_TIME_X    (sDATA_SERVICE_STATE_X + 5)
#define sDATA_SERVICE_TIME_Y    (sDATA_SERVICE_STATE_Y + EQS_CharHeight('A') + sDATA_SERVICE_TEXT_MARGIN_Y)

#define sDIAL_DIGIT_LENGTH_PER_LINE_DATA_SERVICE   15


LOCAL void SKY_API sInitOriginatedDisplayInDataService(void)
{
        HCONTROL hControl;  
        l_CallInfo.num.buf[l_CallInfo.num.len] = '\0';


#ifdef  FEATURE_WAVELET  //      khekim 01/02/13
        if (SKY_IS_IN_WAVELET())
        {
            l_CallInfo.num.len = 0;
            EQC_StaticText(0, 0, (EQS_TOKEN)TKN_CALL_DS_WAVE_ORIG);
        }
        else
#endif  //      #ifdef  FEATURE_WAVELET

        {
            SKY_BacklightOn(SKY_GET_AUTO_BACKLIGHT());//white 2001.06.01

            /* 애니메이션 */
            hControl = EQC_Animation(sDATA_SERVICE_ANI_X, sDATA_SERVICE_ANI_Y, &g_AniInDataService, NULL);
            EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);

            /* 프레임 */
            EQC_Image(sDATA_SERVICE_FRAME_X, sDATA_SERVICE_FRAME_Y, (BITMAP*)&g_BmpCallPannel);

#ifdef FEATURE_SKT_AIRDATA  
            if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_WAIT_AIR_DATAMGR1);
            }
            else
#endif //#ifdef FEATURE_SKT_AIRDATA
#ifdef  FEATURE_IP_AIRDATA      
            if (SKY_GET_IP_AIRDATAMANAGER())
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_IP_DATAMGR_TXT);
            }
            else
#endif  // #ifdef       FEATURE_IP_AIRDATA      
            {
            if( DS_ISDATA_RS1(l_CallInfo.cm_info.srv_opt))
            {
              /* rate is 9.6 */
                if (l_CallInfo.cm_info.srv_opt == CAI_SO_PPP_PKT_DATA_3G)
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_153);
                } 
                else if (l_CallInfo.cm_info.srv_opt == CAI_SO_MDR_PKT_DATA_FRS1_RRS1)
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_64);
                }
                else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9FAX);
                }
                else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
                {
                       if ( STRCMP((BYTE *)l_CallInfo.num.buf , (BYTE *)"1501") ==0)
                       {
                           EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9PACKET);
                       }
                       else
                       {
                           EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9MODEM);
                       }
                }
                else if (l_CallInfo.cm_info.srv_opt == CAI_SO_DATA)                  
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9PIWF);
                }
                else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9PACKET);
                }
                else
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9DATA);
                }
            }
            else if( DS_ISDATA_RS2(l_CallInfo.cm_info.srv_opt))
            {
              /* rate is 14.4 */
                if (l_CallInfo.cm_info.srv_opt==CAI_SO_PPP_PKT_DATA_3G)
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_153);
                } 
                else if (l_CallInfo.cm_info.srv_opt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_91);
                }
                else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14FAX);
                }
                else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
                {
                    if ( STRCMP((BYTE*)l_CallInfo.num.buf, (BYTE *)"1501") == 0 )
                    {
                       EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14PACKET);
                    }
                    else
                    {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14MODEM);
                    }
                }
                else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14PACKET);
                }
                else
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14DATA);
                }
            }
            else
            {
                /* rate is unknown */
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE);
            }
        }
        
        EQC_SET_STYLE(EQS_GetNthIOC(DS_STATE), CS_STATIC_TEXT_ORING);
    }
    l_fCallFromAlert = FALSE;
    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              sDATA_SERVICE_TIME_Y,//DATA_SERVICE_STATE_Y + 16, 
                              sDIAL_DIGIT_LENGTH_PER_LINE_DATA_SERVICE, 
                              1, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );

    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT |
                            CS_NUMBER_EDIT_NO_CML     |
                            CS_NUMBER_EDIT_ALONE      );
	//EQC_SetNumberEditBackColor(hControl, NUMBER_EDIT_BACK_COLOR);

    EQC_SET_FOCUS(hControl, FALSE);
    sDisplayPhoneNumber(hControl);
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Originated()
**
** Description: 
**      This applet integrate  originated_mode_entry and originated_mode_event 
**  function  in "uiscall.c".
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
LOCAL void SKY_API sSKA_Originated(SKY_EVENT_T *pEvent)
{
    HCONTROL hControl;
#ifdef FEATURE_SKT_AIRDATA
    STATIC BOOL     s_fAirDMRelease = FALSE;
#endif

    switch(pEvent->EventID)
    {
        case SE_APP_START : /* The very first event */
            EQS_GetDC(FALSE);
            sOriginatedModeEntry();
            EQS_SetWindow(SKY_DEF_WIN_X,
                        SKY_DEF_WIN_Y, 
                        SKY_DEF_WIN_DX, 
                        SKY_DEF_WIN_DY, 
                        CL_NONE, 
                        NULL, // show annunciators
                        WA_SHOW_HEADER);

            s_fAirDMRelease = FALSE;
            g_fRelease = FALSE;
            if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
            {
                sInitOriginatedDisplayInVoiceCall();
                SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_MUTED, FALSE);    
            }
#ifdef FEATURE_DS           
            else if(sIsDataCall()) /* 데이터 서비스  */
            {
                sInitOriginatedDisplayInDataService();
            }
#endif /* FEATURE_DS */
            sInitWaitRetry();
            break;

        case SE_APP_END : /* The very last event */
            g_fRelease = FALSE;
            s_fAirDMRelease = FALSE;
            EQS_ReleaseDC();
            break;
        
        case SE_APP_DRAW :
//            DrawGeneralFrame();
            break;

        case SE_KEY_0 ://HS_0_K:
        case SE_KEY_1 ://HS_1_K:
        case SE_KEY_2 ://HS_2_K:
        case SE_KEY_3 ://HS_3_K:
        case SE_KEY_4 ://HS_4_K:
        case SE_KEY_5 ://HS_5_K:
        case SE_KEY_6 ://HS_6_K:
        case SE_KEY_7 ://HS_7_K:
        case SE_KEY_8 ://HS_8_K:
        case SE_KEY_9 ://HS_9_K:
        case SE_KEY_STAR ://HS_STAR_K:
        case SE_KEY_POUND ://HS_POUND_K:
        case SE_KEY_UP ://HS_UP_K:
        case SE_KEY_DOWN ://HS_DOWN_K:
        // ignore all these events
            break;

        case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
            break;

        case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
        // if not earpiece endable, put up lock icon, otherwise,
        // end the call, waiting for call event to change display
            if (SKY_IS_EAR_JACK_OFF()) 
            {
#ifdef FEATURE_SKT_AIRDATA
                if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
                {
                    s_fAirDMRelease = TRUE;
                    PtpLinkRelease();
                }
                else 
#endif
#ifdef  FEATURE_IP_AIRDATA                    
                if(SKY_GET_IP_AIRDATAMANAGER())
                {
                    SKY_SET_SOCK_MODE(FALSE);
                    SKY_SET_SOCK_CONNECT(FALSE);
                    g_fRelease = TRUE;
#ifndef WIN32
                    SvrLinkRelease();
#endif
                }else
#endif
                    SKY_EndCall();
            } 
            else 
            {
            //    uiscrn_bind_softkey( (byte*)UI_SK_LOCK_F);              
            }
            break;

        case SE_SOFTKEY_LEFT ://HS_PF1_K:
        case SE_SOFTKEY_RIGHT ://HS_PF2_K:
        case SE_KEY_CLEAR :// HS_CLR_K:
        case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
            // ignore all these events
            break;

        case SE_KEY_END ://HS_END_K:
            // send CM end call command, waiting for call event to change display
#ifdef FEATURE_SKT_AIRDATA
            if((s_fAirDMRelease || g_fRelease)
#ifndef WIN32
                && dsi_srvc_mode == DS_AUTODETECT_SRVC_MODE
#endif
                )
            {
                SKY_SET_AIRDATAMANAGER_ORG(FALSE);
                SKY_SET_AIRDATAMANAGER_RCV(FALSE);
                SKY_SET_SOCK_MODE(FALSE);
                SKY_SET_SOCK_CONNECT(FALSE);
                SKY_SET_IP_AIRDATAMANAGER(FALSE);
                s_fAirDMRelease = FALSE;
                g_fRelease = FALSE; 
                sOriginatedExit();
                sCallEndSetup(); 
                break;
            }

            if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
            {
                s_fAirDMRelease = TRUE;
                PtpLinkRelease();
            }else 
#endif  
#ifdef  FEATURE_IP_AIRDATA                
                if(SKY_GET_IP_AIRDATAMANAGER())
            {
                SKY_SET_SOCK_MODE(FALSE);
                SKY_SET_SOCK_CONNECT(FALSE);
                g_fRelease = TRUE;
#ifndef WIN32
                SvrLinkRelease();
#endif
            }else
#endif
             SKY_EndCall();
            break;

        case SE_KEY_SEND ://HS_SEND_K:
            break;

        case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
            // ignored
            break;

        case SE_TM_TIMER1 ://UI_TIMER_F:
                // not supposed to be here
            ASSERT(FALSE);
            break;

//        case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
//        case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
//#ifdef FEATURE_IS95B_EXT_DISP 
//        case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
//#endif 
//            ClearAlphaEditInfo();
//            sOriginatedExit();
//            if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && (pEvent->EventID != SE_CALL_INCOMING))
//            {
//                SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info),  pEvent->EventID);            
//            }
//            EQS_START_SIBLING_CURRENT_EVENT(SKA_Alerted,FALSE);
//            break;

        /* jrkwon 2000-05-20 --> ignore this originated event. it occurred when voice call is originated */
        /* <-- */
        case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        case SE_CALL_FLASHED :   //UI_CALL_FLASHED_E:
        case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
                // not supposed to be here
                ASSERT(FALSE);
            break;

        case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
            // possibly call end command fails,
            // do not change display, hopefully user press end again.
            // what if user close the flip? play some sound? error fatal?
            // (ui.evt_param.call_cmd_err.cmd == CM_CALL_CMD_END ) 
            if (GET_EVENT_PARAM()->call_cmd_err.cmd == CM_CALL_CMD_END )
            {          
              SKY_ERROR_TONE();
            }
            break;

        case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
            //if(!SKY_IS_PHONE_OPEN())
            SKY_SNI(HS_SNI_CALL_CONNECT, TRUE);

            ClearAlphaEditInfo();
            // go to in_prog mode or paused mode
            // or go to pin_prompt mode if necessary
            sOriginatedExit();
            // 통화연결음
            if(SKY_IS_CONNECT_WARNING_SIGNAL())
            {
                if(!SKY_IS_ETIQUETTE())
                {
                #if (MODEL_ID == MODEL_B0)
                    SKY_PlaySoundEx(UI_MUSIC_SND, MUS_TELEPHONE1, 1, 1);
                #elif (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B2)
                    SKY_PlayMusicEx( MUS_DENWA_1, 1, 1);
                #endif
                }
            }

            sInitConversation();
#ifdef FEATURE_UI_SEND_PIN
            EQS_START_SIBLING(SKA_PinPrompt,FALSE);
#else
#ifdef FEATURE_IP_AIRDATA              
            if(SKY_GET_IP_AIRDATAMANAGER())             
            {
                EQS_START_SIBLING(SKA_InProgIPAirDM, FALSE);
            }
            else
#endif //FEATURE_IP_AIRDATA
            {            
                EQS_START_SIBLING(SKA_InProg, FALSE);
            }
#endif
            break;

#ifdef FEATURE_UI_DORMANT
         case SE_DATA_DORM_F:
            if(sIsDormantEnable())
            {
                SKY_SET_DORMANT(TRUE);
                SKY_SetThenCompareInUseState(SKY_GetInUseState());
                sOriginatedExit();
                l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;                
                sCallEndSetup(); // call_end_setup();
                EQS_END_TO(EQS_POP_TO_ROOT);
            }
            break;

        case SE_DATA_IDLE_F:
            SKY_SET_DORMANT(FALSE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            sOriginatedExit();
            //l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;                
            sCallEndSetup(); // call_end_setup();
            break;
#endif //#ifdef FEATURE_UI_DORMANT

        case SE_CALL_ENDED ://UI_CALL_ENDED_E:
            /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
            /* 통화종료화면이 나오지 않는 현상 방지                            */
            if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
            {
               EQS_DeleteControl(hControl);
            }

#ifdef FEATURE_IP_AIRDATA
            if(SKY_GET_IP_AIRDATAMANAGER())
            {
                if(g_fRelease)        //애플릿에서 끊을때
                {
                    SKY_SET_SOCK_MODE(FALSE);
                    SKY_SET_SOCK_CONNECT(FALSE);
                    SKY_SET_IP_AIRDATAMANAGER(FALSE);
                    g_fRelease = FALSE;
                    //EQS_END();
                }
                else
                {
                    SKY_SET_SOCK_MODE(FALSE);
                    SKY_SET_SOCK_CONNECT(FALSE);
                    g_fRelease = TRUE;
#ifndef WIN32
                    SvrLinkRelease();
#endif
                    break;
                }
            }else 
#endif
#ifdef FEATURE_SKT_AIRDATA
            if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
            {
                if(s_fAirDMRelease)
                {
                    SKY_SET_AIRDATAMANAGER_ORG(FALSE);
                    SKY_SET_AIRDATAMANAGER_RCV(FALSE);
                    s_fAirDMRelease = FALSE;
                }else
                {
                    s_fAirDMRelease = TRUE;
                    PtpLinkRelease();
                    break;
                }
            }
#endif            
            if (sAllowRetry(&(GET_EVENT_PARAM()->call_event.call_info)) 
                && SKY_IS_AUTO_REDIAL()
                && !SKY_IS_MAINTREQ()) 
            {           
                sOriginatedExit();
                l_CallInfo.ui_try_cnt++;
                EQS_START_SIBLING(sSKA_WaitRetry,FALSE);
            } 
            else if(SKY_IS_AUTO_REDIAL() 
                    && ui_call_is_from_ui(&(GET_EVENT_PARAM()->call_event.call_info)) 
                    && (l_CallInfo.ui_try_cnt >= sMAX_RETRIES)
                    && !SKY_IS_MAINTREQ())
            {
                sOriginatedExit();
                l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;                 
                EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CALL_RETRY_FAIL), MESSAGE_DEFAULT_TIME,
                               MBT_NONE, MBT_FAIL);
            }
            else 
            {
                sOriginatedExit();
                l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;                
                sCallEndSetup(); // call_end_setup();
            }
            break;

        case SE_IO_MESGBOX_END:
            sCallEndSetup();
             break;

#ifdef FEATURE_IP_AIRDATA
        case SE_SOCK_RELEASED:      //IP DM
            g_fRelease = FALSE; 
            SKY_SET_SOCK_MODE(FALSE);
            SKY_SET_SOCK_CONNECT(FALSE);
            SKY_SET_IP_AIRDATAMANAGER(FALSE);
            /* IP Air Data Manager 종료 후  종료 화면을 보여주기 위해 */
            sOriginatedExit();
            sCallEndSetup(); // call_end_setup();
            //SKY_EndCall();
            break;
#endif

        case SE_CALL_INCOMING:
            ClearAlphaEditInfo();
            sOriginatedExit();
        default:
            CallDefaultEventProc(pEvent, ORIGINATED);
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
LOCAL void sInitWaitandRetryDisplay(void)
{
    HCONTROL        hControl;
    //BYTE            sORIGINATION_NAME_XPOS;

    EQC_StaticText(sORIGINATION_MESSAGE_XPOS, 
                   sMESSAGE_BASE, 
                   (EQS_TOKEN)TKN_CALL_RETRY);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
              sORIGINATION_PHONE_OPEN_YPOS, 
              (BITMAP*)&g_BmpPhoneOpen);

    hControl = EQC_Animation(sORIGINATION_ANIMATION_XPOS, 
                             sORIGINATION_ANIMATION_YPOS, 
                             &g_AniOriginatingCall,//&g_AniCall64x39x256c, 
                             NULL);
    EQC_StartAnimation(hControl, TRUE);

    EQC_Image(sORIGINATION_IMAGE_XPOS, 
              sORIGINATION_IMAGE_YPOS, 
              (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );    
    //EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|
                            CS_NUMBER_EDIT_NO_CML    |
                            CS_NUMBER_EDIT_RETURN_UPDOWN_KEY);
//                            CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
    l_fCallFromAlert = FALSE;


    if(l_PausedMode == PM_NONE)
    {
      sDisplayPhoneNumber(hControl);
    }
    else
    {
      sDisplaySomePhoneNumber(hControl);
    }


    if(GET_ALPHAEDIT_POS > 0)
    {
      //sORIGINATION_NAME_XPOS = SKY_SCREEN_WIDTH/2 - (STRLEN(GET_ALPHAEDIT_BUFFER)*EQS_CharWidth('A'))/2;
      hControl = EQC_StaticText(sORIGINATION_NAME_XPOS,
                                sORIGINATION_NAME_YPOS,
                                TKN_NULL);
      EQC_SetStaticText(hControl, (BYTE*) GET_ALPHAEDIT_BUFFER);
    }    
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_WaitRetry()
**
** Description: 
**     This applet integrate  wait_retry_mode_entry and wait_retry_mode_event 
**   function  in "uiscall.c".
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
LOCAL void SKY_API sSKA_WaitRetry(SKY_EVENT_T *pEvent)
{
  
    STATIC HCONTROL hCtrlNoService;

    switch(pEvent->EventID)
    {
     case SE_APP_START : /* The very first event */
                 EQS_GetDC(TRUE);       
         EQS_SetWindow(SKY_DEF_WIN_X,
              SKY_DEF_WIN_Y, 
              SKY_DEF_WIN_DX, 
              SKY_DEF_WIN_DY, 
              CL_NONE, 
              NULL, // show annunciators
              WA_SHOW_HEADER);
         sInitWaitandRetryDisplay();
         EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        break;

     case SE_APP_END : /* The very last event */
         sWaitRetryExit();
         EQS_ReleaseDC();
         break;

     case SE_APP_DRAW :
//         DrawGeneralFrame();
         break;

     case SE_KEY_0 ://HS_0_K:
     case SE_KEY_1 ://HS_1_K:
     case SE_KEY_2 ://HS_2_K:
     case SE_KEY_3 ://HS_3_K:
     case SE_KEY_4 ://HS_4_K:
     case SE_KEY_5 ://HS_5_K:
     case SE_KEY_6 ://HS_6_K:
     case SE_KEY_7 ://HS_7_K:
     case SE_KEY_8 ://HS_8_K:
     case SE_KEY_9 ://HS_9_K:
     case SE_KEY_STAR ://HS_STAR_K:
     case SE_KEY_POUND ://HS_POUND_K:
        // ignored
        break;

     case SE_KEY_UP:
     case SE_KEY_DOWN:
        sWaitRetryExit();
        EQS_START_CHILD(SKA_AdjstConversationVol);
        break;

    case SE_KEY_PHONE_OPEN ://HS_EAR_UP_K:
        // ignored
        break;

    case SE_KEY_PHONE_CLOSE ://HS_EAR_DOWN_K:
        // exit call state, do not consume the event
        // hopefully idle state will process this event
        ClearAlphaEditInfo();
        sWaitRetryExit();
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

    case SE_SOFTKEY_LEFT ://HS_PF1_K:
    case SE_SOFTKEY_RIGHT ://HS_PF2_K:
        // ignored
        break;

    case SE_KEY_CLEAR :// HS_CLR_K:
    case SE_KEY_CLEAR_LONG ://UI_CLRALL_F:
    case SE_KEY_END ://HS_END_K:
        ClearAlphaEditInfo();
        sWaitRetryExit();
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

     case SE_KEY_SEND ://HS_SEND_K:
        // send origination to CM and go to orig_attempt mode
        sWaitRetryExit();
        if(SKY_IS_NOSERVICE() == FALSE)
        {
            ui_orig_voice_call ( 
                                  l_CallInfo.num.buf, 
                                  l_CallInfo.num.len,
                                  l_CallInfo.alpha.buf,
                                  l_CallInfo.alpha.len
                                );
        }
        else
        {
		    EQS_StopTimer(EQS_TIMER_1);
            hCtrlNoService = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                     MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);
        }
        //EQS_START_SIBLING(sSKA_OrigAttempt,FALSE);
        break;


    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
        // this may happen when a call event happens
        // exit call state, go to idle
        sWaitRetryExit();
        EQS_END_TO(EQS_POP_TO_ROOT);
        break;

     case SE_TM_TIMER1 ://UI_TIMER_F:
        l_wRetryTimeLeft--;
        if (l_wRetryTimeLeft == 0) 
        {
            sWaitRetryExit();
            if(SKY_IS_NOSERVICE() == FALSE)
            {
                ui_orig_voice_call(
                                  l_CallInfo.num.buf,
                                  l_CallInfo.num.len,
                                  l_CallInfo.alpha.buf,
                                  l_CallInfo.alpha.len
                                  );
            }
            else
            {
                EQS_StopTimer(EQS_TIMER_1);
                hCtrlNoService = EQC_MessageBox((BYTE*) SKY_GET_TOKEN(TKN_CALL_NOSERVICE), 
		                          MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SYSTEM);                
            }
            //EQS_START_SIBLING(sSKA_OrigAttempt, FALSE);
        } 
        break;

//     case SE_TM_PAUSE ://UI_PAUSETIMER_F:
//        // not supposed to be here
//        ASSERT(FALSE);
//        break;

//     case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
//     case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
//#ifdef FEATURE_IS95B_EXT_DISP 
//     case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
//#endif 
//        ClearAlphaEditInfo();
//        sWaitRetryExit();
//        if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && (pEvent->EventID != SE_CALL_INCOMING))
//        {
//            SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info),  pEvent->EventID);        
//        }
//        EQS_START_SIBLING_CURRENT_EVENT(SKA_Alerted,FALSE);
//        break;

                
#if 0//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
     case SE_CALL_CONNECT ://UI_CALL_CONNECT_E:
     case SE_CALL_ENDED ://UI_CALL_ENDED_E:
     case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
     case SE_CALL_FLASHED ://UI_CALL_FLASHED_E:
     case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
        // not supposed to be here, if call events
        // do happen, TOIDLE event will drive state
        // machine to idle first
        ASSERT(FALSE);
        break;
#endif //0000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     case SE_IO_MESGBOX_END:
         if(pEvent->wParam == hCtrlNoService)
         {             
             EQS_END_TO(EQS_POP_TO_ROOT);
         }
         break;


     case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
         EQS_START_SIBLING_CURRENT_EVENT(sSKA_OrigAttempt, FALSE);
         break;

     case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
        ClearAlphaEditInfo();
        sWaitRetryExit();
     default:
        CallDefaultEventProc(pEvent, WAITRETRY);
        break;
    }
}


/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sGetPauseString(BYTE* aPausedString)
**
** Description: 
**     
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
LOCAL UINT8 SKY_API sGetPauseString(BYTE* aPausedString)
{
    UINT8           iLoop,jLoop;
    EDITORINFO_T    CurrNumInfo;   

    CurrNumInfo = GET_NUMEDIT;

    for(iLoop = GetRealPhoneNumberLength(), jLoop = 0; CurrNumInfo.aBuf[iLoop] != '\0'; iLoop++,jLoop++)
    {
        if((CurrNumInfo.aBuf[iLoop] != T_PAUSE_CHAR) && (CurrNumInfo.aBuf[iLoop] != P_PAUSE_CHAR))
        {
            aPausedString[jLoop] = CurrNumInfo.aBuf[iLoop];   
        }
        else  
        {
            if(CurrNumInfo.aBuf[iLoop] == T_PAUSE_CHAR)      /* T PAUSE 일 경우 */
            {
                l_PausedMode = PM_TPAUSE;
            }
            else if(CurrNumInfo.aBuf[iLoop] == P_PAUSE_CHAR) /* P PAUSE 일 경우 */
            {
                l_PausedMode = PM_PPAUSE;
            }
            break;
        }
    }

    if(CurrNumInfo.nPos == iLoop)  l_PausedMode = PM_NONE;  /* 더 이상 발신할 Paused String이 없는 경우 */

    SetRealPhoneNumberLength(iLoop);
    aPausedString[jLoop] = '\0';

    return jLoop;
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitTPauseDisplay()
**
** Description: 
**     Initialize T-pause mode display
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
LOCAL void SKY_API sInitPausedDisplay(void)
{
    HCONTROL hControl;
    BYTE     pszTmpBuf[20];

    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                              TKN_NULL); 

    sMakeDateTimeString(TRUE, pszTmpBuf);
    EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf);

    hControl = EQC_StaticText(sORIGINATION_TIME_XPOS, 
                              sORIGINATION_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL); 
    DisplayCallTime(hControl);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
	         (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );

//    EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|
                            CS_NUMBER_EDIT_NO_CML|
                            CS_NUMBER_EDIT_RETURN_UPDOWN_KEY);
//                            CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font    

    sDisplaySomePhoneNumber(hControl);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**    sSKA_MultiWait(SKY_EVENT_T *pEvent)
**
** Description: 
**    This applet integrate  multi_wait_mode_entry and multi_wait_mode_event 
**    function  in "uiscall.c".
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
LOCAL void SKY_API sSKA_MultiWait(SKY_EVENT_T *pEvent)
{
    STATIC UINT8 nConnectingTime;
    BYTE     aPausedString[NE_MAX_SIZE];
    UINT8    nPausedStringLength;
    HCONTROL hControl;
  
    switch(pEvent->EventID)
    {
        case SE_APP_START :   /* The very first event */
        EQS_GetDC(TRUE);      
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE, 
                    NULL, 
                    WA_SHOW_HEADER);
        sInitPausedDisplay();
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        break;
     
     case SE_APP_END :     /* The very last event */
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));
        DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
//        DrawGeneralFrame();
        break;

    case SE_TM_TIMER1:
        nConnectingTime++;

        if((nConnectingTime % sTPAUSE_BURST_TIME) == 0)
        {
            nPausedStringLength = sGetPauseString(aPausedString);
            if(nPausedStringLength > 0)
            {
                sInitBurstString(aPausedString, nPausedStringLength, FALSE);
                sBurstAndAdvancePause();    
            }
            else
            {
                EQS_SEND_EVENT(SE_SND_MULTI_TONE_DONE, EQS_NA, EQS_NA);
            }
        }
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;

    case SE_SND_MULTI_TONE_DONE:              /* T pause 발신이 끝난 경우 */
        if(l_PausedMode == PM_NONE)        /* 더이상 발신할 내용이 없을 경우 */
        {
            ClearNumEditInfo();
            EQS_START_SIBLING(SKA_InProg, FALSE);
        }
        else if(l_PausedMode == PM_TPAUSE) /* T pause를 발신해야할 경우 */
        {
            SetRealPhoneNumberLength(GetRealPhoneNumberLength()+1);
        }
        else if(l_PausedMode == PM_PPAUSE) /* P pause를 발신해야할 경우 */
        {
            SetRealPhoneNumberLength(GetRealPhoneNumberLength()+1);
            EQS_START_SIBLING(sSKA_Paused, FALSE);
        }
        sDisplaySomePhoneNumber(EQS_GetNthIOC(IFO_PHONE_NUMBER));
        break;

    case SE_KEY_UP :      /*  ignore */
    case SE_KEY_DOWN:
    case SE_SYS_TO_IDLE:  
    case SE_KEY_CLEAR:
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_CALL_ENDED:
    case SE_KEY_END:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        DataManagerEnd(pEvent);
        ClearNumEditInfo();
        l_PausedMode = PM_NONE;
        EQS_START_SIBLING_CURRENT_EVENT(SKA_InProg, FALSE);
        break;


    default:
        CallDefaultEventProc(pEvent, MULTIWAIT);
        break;

    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSKA_Paused(SKY_EVENT_T *pEvent)
**
** Description:
**     Burst P-pause
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
LOCAL void SKY_API sSKA_Paused(SKY_EVENT_T *pEvent)
{
    BYTE     aPausedString[NE_MAX_SIZE];
    UINT8    nPausedStringLength;
    HCONTROL hControl;

    
    switch(pEvent->EventID)
    {
    /* EQC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);      
        EQS_SetWindow(SKY_DEF_WIN_X,
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY, 
                    CL_NONE_VOLUME_NEXT, 
                    NULL, 
                   (WA_SHOW_HEADER | WA_SHOW_CML));
        sInitPausedDisplay();
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        break;

    case SE_APP_END :     /* The very last event */
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));
        DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
//        DrawGeneralFrame();
        break;

    case SE_TM_TIMER1:
        EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        break;    
    
    case SE_SOFTKEY_NEXT:    /* 사용자가 예약된 P Pause를 발신할 경우 */
        nPausedStringLength = sGetPauseString(aPausedString);
        
        if(nPausedStringLength > 0)
        {
            sInitBurstString(aPausedString, nPausedStringLength, FALSE);
            sBurstAndAdvancePause();             
        }
        else
        {
            EQS_SEND_EVENT(SE_SND_MULTI_TONE_DONE, EQS_NA, EQS_NA);
        }
        break; 

    case SE_SND_MULTI_TONE_DONE:              /* P pause 발신이 끝난 경우 */
        if(l_PausedMode == PM_NONE)        /* 더이상 발신할 내용이 없을 경우 */
        {
            ClearNumEditInfo();
            EQS_START_SIBLING(SKA_InProg, FALSE);
        }
        else if(l_PausedMode == PM_TPAUSE) /* T pause를 발신해야할 경우 */
        {
            SetRealPhoneNumberLength(GetRealPhoneNumberLength()+1);
            EQS_START_SIBLING(sSKA_MultiWait, FALSE);
        }
        else if(l_PausedMode == PM_PPAUSE) /* P pause를 발신해야할 경우 */
        {
            SetRealPhoneNumberLength(GetRealPhoneNumberLength()+1);
        }
        sDisplaySomePhoneNumber(EQS_GetNthIOC(IFO_PHONE_NUMBER));
        break;

    case SE_KEY_UP :      /* 통화 중 볼륨 조절 */
    case SE_KEY_DOWN:
         EQS_START_CHILD(SKA_AdjstConversationVol);
        break;

    case SE_SYS_TO_IDLE:  /*  ignore */
    case SE_KEY_CLEAR:
        break;

        
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_CALL_ENDED:
    case SE_KEY_END:
#ifdef FEATURE_UI_DORMANT
    case SE_DATA_IDLE_F :
#endif
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        DataManagerEnd(pEvent);
        ClearNumEditInfo();
        l_PausedMode = PM_NONE;
        EQS_START_SIBLING_CURRENT_EVENT(SKA_InProg, FALSE);
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
        CallDefaultEventProc(pEvent, PAUSED);
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
#define sCALL_TOGGLE_MUTE_XPOS      (SKY_SCREEN_WIDTH - (EQS_CharWidth('A') * 10) - 3)
#define sCALL_TOGGLE_MUTE_YPOS      (sORIGINATION_TIME_YPOS + EQS_CharHeight('A'))

LOCAL void SKY_API sInitToggleMuteDisplayInOriginatedCall(void)
{
    HCONTROL hControl;
    BYTE     pszTmpBuf[20];

    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                              TKN_NULL); 

    sMakeDateTimeString(TRUE, pszTmpBuf);
    EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf);    

    hControl = EQC_StaticText(sORIGINATION_TIME_XPOS, 
                              sORIGINATION_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL);  
    DisplayCallTime(hControl);        

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
	 	      (BITMAP*)&g_BmpCallPannel);
    
    hControl = EQC_StaticText(sCALL_TOGGLE_MUTE_XPOS, 
                              sCALL_TOGGLE_MUTE_YPOS, 
                             (EQS_TOKEN)TKN_NULL);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//    EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT |
                            CS_NUMBER_EDIT_NO_CML     |
                            CS_NUMBER_EDIT_ALONE      );
//                            CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
    sDisplayPhoneNumber(hControl);
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
LOCAL void SKY_API sInitToggleMuteDisplayInAlertedCall(void)
{
    HCONTROL  hControl;
    BYTE      pszTmp[20];
    BOOL      fBlinkName, fCNIP, fCDNIP, fCNIR, fCNAP, fPHONEBOOK;
    UINT8     nStaticText4Name;
    UINT8     nNumberEditor4PhoneNumber;

    fBlinkName = FALSE;
    fCNIP      = FALSE;
    fCDNIP     = FALSE;
    fCNIR      = FALSE;
    fCNAP      = FALSE;
    fPHONEBOOK = FALSE;

    nStaticText4Name = 0;
    nNumberEditor4PhoneNumber = 0;
                
#ifndef SKTT_RELEASE
    /* hjcho-test--> 테스트 용 --> */
    //sSetInProgExtendedDispTest(); 
    /* hjcho-test--> 테스트 용 --> */
#endif

    if(SKY_IS_NUMBER_PLUS())
    {
      if(l_CallInfo.called_party_num.buf[0] != '\0')
      {
        if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {
          fCNAP = TRUE;
          fPHONEBOOK = TRUE;
          fBlinkName = TRUE;
          nStaticText4Name++;
        }
        else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
        {         
          fPHONEBOOK = TRUE;
          nStaticText4Name++;
        }
        else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {
          fCNAP = TRUE;
          nStaticText4Name++;
        }

        if(l_CallInfo.num.buf[0] != '\0')
        {
            if(l_CallInfo.bPI == CAI_ALLOW_PI)
            {
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else //presentation is not allowed or restricted!!
            {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            fCDNIP = TRUE;
            nNumberEditor4PhoneNumber++;
        }
        else
        {          
            fCDNIP = TRUE;
            nNumberEditor4PhoneNumber++;
        }
      }
      else  /* Called Party에 번호가 들어 있지 않을 경우 */
      {
        if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {
            fCNAP      = TRUE;
            fPHONEBOOK = TRUE;
            fBlinkName = TRUE;
            nStaticText4Name++;
        }
        else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
        {
            fPHONEBOOK = TRUE;         
            nStaticText4Name++;
        }
        else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {
            fCNAP = TRUE;
            nStaticText4Name++;
        }


        if(l_CallInfo.num.buf[0] != '\0')
        {
          if(l_CallInfo.bPI == CAI_ALLOW_PI)
          {                     
            fCNIP = TRUE;
            nNumberEditor4PhoneNumber++;
          }
          else
          {
            fCNIR = TRUE;
            nNumberEditor4PhoneNumber++;               
          }
        }
      }
    }
    else // Number Plus가 설정되어 있지 않을 경우
    {
      if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
      {
        fCNAP      = TRUE;
        fPHONEBOOK = TRUE;
        fBlinkName = TRUE;
        nStaticText4Name++;
      }
      else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
      {
        fPHONEBOOK = TRUE;
        nStaticText4Name++;        
      }
      else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
      {        
        fCNAP = TRUE;  
        nStaticText4Name++;
      }

      if(l_CallInfo.num.buf[0] != '\0')
      {
        if(l_CallInfo.bPI == CAI_ALLOW_PI)
        {
            fCNIP = TRUE;
            nNumberEditor4PhoneNumber++;
        }
        else
        {
            fCNIR = TRUE;
            nNumberEditor4PhoneNumber++;
        }
      }
    }

    if(nNumberEditor4PhoneNumber == 0)         /* Number Editor가 없는 경우 */
    {
        if(nStaticText4Name == 1)              /* CNAP만 있는 경우 */
        {
            if(fCNAP)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_CNAP_NAME, 
                                          l_CallInfo.alpha_cnap.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_PHONEBOOK_NAME, 
                                          l_CallInfo.alpha.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_NO_NAME, 
                                          l_CallInfo.alpha.buf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 1)    /* Number Editor가 하나인 경우 */
    {        
        if(fCNIP)
        {
            if(fCNAP)
            {            
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIP, 
                                            l_CallInfo.num.buf);

            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
        }
        else if(fCDNIP)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 2)    /* Number Editor가 두개인 경우 */
    {
        if(fCNIP)
        {
            if(fCNAP)                         
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,  
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,  
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
        }
    }

    /* 현재시각 */  
    sMakeDateTimeString(TRUE, pszTmp);
    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                             (EQS_TOKEN)TKN_NULL);
    EQC_SetStaticText(hControl, (BYTE*) pszTmp);

    /* 통화시간 */
    hControl =  EQC_StaticText(sCALL_TIME_XPOS, 
                               sCALL_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL);    
    DisplayCallTime(hControl);

    EQC_StaticText(sCALL_TOGGLE_MUTE_XPOS, 
                   sCALL_TOGGLE_MUTE_YPOS, 
                   (EQS_TOKEN)TKN_NULL); 
}



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_ToggleMute()
**
** Description: 
**     Do Mute
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
void SKY_API SKA_ToggleMute(SKY_EVENT_T *pEvent)
{
   BYTE          szTmp[10];
   CNI_T*        pCallerID;
   HCONTROL      hControl;
   STATIC BOOL   s_fEnableCallRecording;

   switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START : /* The very first event */
                
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE_NONE_RELEASE, 
            NULL, 
            (WA_SHOW_HEADER | WA_SHOW_CML));
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        s_fEnableCallRecording = TRUE;

        if(l_fCallFromAlert)
        {
            sInitToggleMuteDisplayInAlertedCall();
        }
        else
        {
            sInitToggleMuteDisplayInOriginatedCall();
        }
        break;

    case SE_APP_RESUME:
        s_fEnableCallRecording = TRUE;
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        SKY_AcceptKey(TRUE);
        if(EQS_GET_CHILD() == SKA_MainClid)
        {
            pCallerID = SKY_GetCallerID();
            if(IsData2DisplayInCallWaitingCall())
            {
                EQS_START_CHILD(SKA_CallWaitingCall);
            }
        }
        else if(EQS_GET_CHILD() == SKA_MainCallRecord)
        {
            
            /* 통화 중 녹음 후 RECORD KEY를 눌러서 녹음을 저장한 경우  */
            /* 녹음 저장시간으로 인해 SE_KEY_RECORD_LONG 이벤트가      */
            /* 다시 발생해서 사용자가 의도하지 않은 통화 중 녹음이 실행*/
            /* 되는 경우가 있다. 이를 방지하기 위해서 Flag를 이용한다. */
            s_fEnableCallRecording = FALSE;
            EQS_StartTimer(EQS_TIMER_3, 2000, FALSE);
        }
        else if(EQS_GET_CHILD() == SKA_CallWaitingCall)
        {

        }
        break;

    case SE_APP_DRAW  :     

        if(l_fWhisperMode == TRUE) /* 속삭임설정 */
        {
            sprintf((char*)szTmp, "%s%s",SF_WHISPER1_2,SF_WHISPER2_2);    
            EQS_TextOutStr(sWHISPER_XPOS, sWHISPER_YPOS, (BYTE *)szTmp);
        }
//        /* 착신인 경우에는 Frame을 그려주지 않는다 */
//        if(l_fCallFromAlert == FALSE)
//        {
//            DrawGeneralFrame();
//        }
        break;

    case SE_APP_SUSPEND:
        s_fEnableCallRecording = TRUE;
        break;


    case SE_APP_END :                    /* The very last event */
        if(SKY_IS_MUTE()) 
        {                                /* mute already on */
          SKY_SET_MUTE(FALSE);
          SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);    
        }        
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        s_fEnableCallRecording = TRUE;
        EQS_ReleaseDC();
        break;

    case SE_KEY_CLEAR:
    case SE_SOFTKEY_RELEASE:   
	    if(l_fCallFromAlert)
		{
			/* 착신화면에서 "송화음차단"은 통화시간을 나타내는 화면의 바로 */
			/* 아래에 나타난다.                                            */
			sToggleMute(EQS_GetNthIOC(VM_CALL_TIME+1));
		}
		else
		{
			sToggleMute(EQS_GetNthIOC(VM_CURRENT_MODE));
		}        
        EQS_StopTimer(EQS_TIMER_1);
        EQS_END();
        break;

    case SE_IO_POPUP_SELECT :  /* This event from SKA_InProg */

        if(l_fCallFromAlert)
		{
			/* 착신화면에서 "송화음차단"은 통화시간을 나타내는 화면의 바로 */
			/* 아래에 나타난다.                                            */
			sToggleMute(EQS_GetNthIOC(VM_CALL_TIME+1));
		}
		else
		{
			sToggleMute(EQS_GetNthIOC(VM_CURRENT_MODE));
		}
        break;
   
    case SE_CALL_COMMAND_ERROR:
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        DataManagerEnd(pEvent);
        break;

    case SE_TM_TIMER1 : 
        sDisplayCurrentDateTime(EQS_GetNthIOC(VM_CURRENT_TIME));
        DisplayCallTime(EQS_GetNthIOC(VM_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
        break;
        
    case SE_TM_TIMER2 :
        EQS_START_CHILD(SKA_MainClid);
        break;

    case SE_TM_TIMER3:
        s_fEnableCallRecording = TRUE;
        break;

    case SE_KEY_RECORD_LONG :
        if(EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            if(!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK()))
            {
                if(SKY_IS_RECORD_PLAY_LOCK() && (s_fEnableCallRecording == TRUE))
                {
                    EQS_START_CHILD_EVENT(SKA_MainRecCode, SE_APP_START, RECCODE_IN_CALL, EQS_NA);
                }
                else if(s_fEnableCallRecording == TRUE)
                {
                    EQS_START_CHILD_EVENT(SKA_MainCallRecord, SE_KEY_RECORD_LONG, EQS_NA, EQS_NA);
                }
            }
        }
        //EQS_START_CHILD_EVENT(SKA_MainCallRecord, SE_KEY_RECORD_LONG, EQS_NA, EQS_NA);
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

            nLength = ((((GET_EVENT_PARAM()->call_event.call_info).alpha.len)
                        > MAX_DIAL_DIGIT_LENGTH) ? MAX_DIAL_DIGIT_LENGTH
                        : ((GET_EVENT_PARAM()->call_event.call_info).alpha.len));
            STRNCPY(abTemp, 
                    (GET_EVENT_PARAM()->call_event.call_info).alpha.buf, nLength);
            STRCPY(abPhoneNumber, abTemp + 12);
            sInProgExit();
            SetupCLICDNIP((CHAR *) abPhoneNumber, nLength, 
                          GET_EVENT_PARAM()->call_event.call_info.num.pi, pEvent->EventID);
            EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
        }
        break;

    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
#endif 
      /* call waiting, Save Incoming Phone Number in Call List */
      sInProgExit();
      if(pEvent->EventID != SE_CALL_EXT_DISPLAY)
      {
        SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);
      }
          EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
      break;

    case SE_KEY_MANNERS:  
      if (!ISVOICE(l_CallInfo.cm_info.call_type) ) 
          break;

      if(l_fWhisperMode)
      {
         l_fWhisperMode = FALSE;
      }
      else
      {
         l_fWhisperMode = TRUE;
      }
      SKY_SetSoundDevice( SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, FALSE );
      EQS_SEND_EVENT(SE_APP_DRAW, EQS_NA, EQS_NA);
         break;


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
LOCAL void sInitDTMFDisplayInVoiceCall(void)
{
    HCONTROL hControl;
    BYTE     pszTmpBuf[EQS_MAX_DIAL_DIGITS+3];

    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                              TKN_NULL); 

    sMakeDateTimeString(TRUE, pszTmpBuf);
    EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf);

    hControl = EQC_StaticText(sORIGINATION_TIME_XPOS, 
                              sORIGINATION_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL);         
    DisplayCallTime(hControl); 

	EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
	 	      (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);


    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT  | 
                            CS_NUMBER_EDIT_NO_CML      |
                            CS_NUMBER_EDIT_IGNORE_OKKEY|
                            CS_NUMBER_EDIT_RETURN_UPDOWN_KEY);
//                            CS_NUMBER_EDIT_ORING            |
//                            CS_NUMBER_EDIT_DRAW_ALL_CONTROL);
//    EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
    EQS_SetIOCActive(hControl);   

    if((EQS_GET_PARENT() == SKA_CallForward)      || 
       (EQS_GET_PARENT() == SKA_StopCallWaiting)  ||
       (EQS_GET_PARENT() == SKA_CallPending))
    {
       sGetCallNumber(pszTmpBuf);
       EQC_SetNumberEditValue(hControl, pszTmpBuf);
       EQS_SEND_EVENT(SE_KEY_SEND, EQS_NA, EQS_NA);
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
#define NUM_SEND_DTMF_BLINK    6


void SKY_API SKA_FlashWithInfo(SKY_EVENT_T *pEvent)
{
    UINT8            nNumDigit;
    STATIC UINT8     nCount;
    STATIC BYTE      aPhoneNumber[EQS_MAX_DIAL_DIGITS+3];
    BYTE             szTmp[10];
    HCONTROL         hControl;
    STATIC BOOL      s_fEnableCallRecording;
  
   switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START :    /* The very first event */
                EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_MENU_VOLUME_PBOOK, 
            NULL, 
            WA_SHOW_HEADER|WA_SHOW_CML);       
        nCount = 0;
        s_fEnableCallRecording = TRUE;
        sInitDTMFDisplayInVoiceCall();   
        EQS_StartTimer(EQS_TIMER_2, sONE_SECOND, TRUE);
        break;

    case SE_APP_RESUME:
        s_fEnableCallRecording = TRUE;
        EQS_StartTimer(EQS_TIMER_2, sONE_SECOND, TRUE);
        SKY_AcceptKey(TRUE);
        EQS_SetIOCActive(EQS_GetNthIOC(IFO_PHONE_NUMBER));   
        DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* 서브 LCD에 통화 시간 출력 */
        sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));

        if(EQS_GET_CHILD() == SKA_MainClid)
        {
            if(IsData2DisplayInCallWaitingCall())
            {
                EQS_START_CHILD(SKA_CallWaitingCall);
            }
        }
        else if(EQS_GET_CHILD() == SKA_MainCallRecord)
        {
            
            /* 통화 중 녹음 후 RECORD KEY를 눌러서 녹음을 저장한 경우  */
            /* 녹음 저장시간으로 인해 SE_KEY_RECORD_LONG 이벤트가      */
            /* 다시 발생해서 사용자가 의도하지 않은 통화 중 녹음이 실행*/
            /* 되는 경우가 있다. 이를 방지하기 위해서 Flag를 이용한다. */
            s_fEnableCallRecording = FALSE;
            SKY_SetGeneralTimer(2000, FALSE);
        }
        else if(EQS_GET_CHILD() == SKA_CallWaitingCall)
        {

        }
        break;

    case SE_APP_SUSPEND:
        SKY_SetGeneralTimer(0, FALSE);
        s_fEnableCallRecording = TRUE;
        break;

    case SE_APP_END :      /* The very last event */
        SKY_SetGeneralTimer(0, FALSE);
        s_fEnableCallRecording = TRUE;
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW:
        if(l_fWhisperMode == TRUE) /* 속삭임설정 */
        {
            sprintf((char*)szTmp, "%s%s",SF_WHISPER1_2,SF_WHISPER2_2);    
            EQS_TextOutStr(sWHISPER_XPOS, sWHISPER_YPOS, (BYTE *)szTmp);
        }
//        DrawGeneralFrame();
        break;

    case SE_SYS_TO_IDLE :  /* Ignore this event */
        break;
   
    case SE_KEY_SEND :
      // ignore it for nonvoice call, otherwise
      // send CM flash, display flahsed # to cover current call#
      // if the flash # includes pause, UI prompts user to burst      // # upon receiving UI_CALL_FLASHED_E
          
          if (ISVOICE(l_CallInfo.cm_info.call_type)) 
          {        
              // ui.n comes from UI_NUMEDIT_S or UI_RCL_S state
              if(EQS_GET_CHILD() == SKA_MainInterCode && pEvent->wParam == INTERCODE_IN_CALL)
              {
                  nNumDigit = STRLEN((BYTE *)aPhoneNumber);
                  ui_send_flash(aPhoneNumber, nNumDigit);
                  sInitBurstString(aPhoneNumber, nNumDigit, FALSE);
                  if (nNumDigit) 
                  {        
                      sAdvancePauseInBurst();
                  }             
              }else
              { 
                  nNumDigit = EQC_GetNumberEditValue(EQS_GetNthIOC(IFO_PHONE_NUMBER), aPhoneNumber);
                  if((aPhoneNumber[0] == '0')   &&
			         (aPhoneNumber[1] == '0')   && 
			         (aPhoneNumber[2] >  '0')   &&
			         (nNumDigit > 2) && 
			         SKY_IS_OVERSEAS_CALL_LOCK())
                  {
                    EQS_START_CHILD_EVENT(SKA_MainInterCode,SE_APP_START, INTERCODE_IN_CALL, EQS_NA);  
                    break;
                  }else
                  {
                      ui_send_flash(aPhoneNumber, nNumDigit);
                      sInitBurstString(aPhoneNumber, nNumDigit, FALSE);
                      if (nNumDigit) 
                      {        
                          sAdvancePauseInBurst();
                      }
                  }
              }
          }
          EQS_StartTimer(EQS_TIMER_1, sDTMF_INTERVAL_SECOND, TRUE);
          SKY_AcceptKey(FALSE);
        break;

    case SE_CALL_SENDDTMF:     /* A event from SKA_InProg */
        STRCPY(aPhoneNumber, l_CallInfo.num.buf);
        EQC_SetNumberEditValue(EQS_GetNthIOC(IFO_PHONE_NUMBER), l_CallInfo.num.buf);
        ui_send_flash(l_CallInfo.num.buf, l_CallInfo.num.len);
        sInitBurstString(l_CallInfo.num.buf, l_CallInfo.num.len, FALSE);
        if (l_CallInfo.num.len) 
        {        
            sAdvancePauseInBurst();
        }

        EQS_StartTimer(EQS_TIMER_1, sDTMF_INTERVAL_SECOND, TRUE);
        SKY_AcceptKey(FALSE);
        break;

    case SE_IO_NEDIT_PRESS_UPKEY:
    case SE_IO_NEDIT_PRESS_DOWNKEY:
         EQS_START_CHILD(SKA_AdjstConversationVol);
        break;
    
    case SE_IO_NEDIT_CANCEL:
        /* 화면상에는 메뉴키를 누른 것처럼 보이지만     */
        /* 실제로는 Number Editor가 Act되어 있기 때문에 */
        /* Number Editor의 CANCEL EVENT가 온다.         */
        EQS_END_EVENT(SE_SOFTKEY_MENU, EQS_NA, EQS_NA);
        break;

    case SE_IO_NEDIT_NODIGIT:
        EQS_END();
        break;

    case SE_IO_NEDIT_DONE:
        break;

    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
    case SE_SOFTKEY_PBOOK:    
    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
    case SE_KEY_END:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
          EQS_DeleteControl(hControl);
        }
        DataManagerEnd(pEvent);
        EQS_END_CURRENT_EVENT();        
        break;

    case SE_TM_TIMER:
        SKY_SetGeneralTimer(0, FALSE);
        s_fEnableCallRecording = TRUE;
        break;

    case SE_TM_TIMER1 :     
        nCount++;
        if(nCount % 2 == 0)
        {
            if(nCount == NUM_SEND_DTMF_BLINK)
            {
                SKY_AcceptKey(TRUE);             
                EQS_StopTimer(EQS_TIMER_1);
                EQS_END();
            }
            else
            {
                EQC_SetNumberEditValue(EQS_GetNthIOC(IFO_PHONE_NUMBER), aPhoneNumber);                                     
            }
        }
        else
        {
            EQC_SetNumberEditValue(EQS_GetNthIOC(IFO_PHONE_NUMBER), (BYTE*)"");                                     
        }                                               
        break;        

    case SE_TM_TIMER2 :
        //EQS_SEND_EVENT(SE_APP_DRAW, 0, 0);
        DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* 서브 LCD에 통화 시간 출력 */
        sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));
        break;

    case SE_TM_TIMER3:
        EQS_START_CHILD(SKA_MainClid);
        break;

    case SE_KEY_RECORD_LONG :
        if(EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            if(SKY_IS_RECORD_PLAY_LOCK() && (s_fEnableCallRecording == TRUE))
            {
                EQS_START_CHILD_EVENT(SKA_MainRecCode, SE_APP_START, RECCODE_IN_CALL, EQS_NA);
            }
            else if(s_fEnableCallRecording == TRUE)
            {
                EQS_START_CHILD_EVENT(SKA_MainCallRecord, SE_KEY_RECORD_LONG, EQS_NA, EQS_NA);
            }
        }
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

            nLength = ((((GET_EVENT_PARAM()->call_event.call_info).alpha.len)
                        > MAX_DIAL_DIGIT_LENGTH) ? MAX_DIAL_DIGIT_LENGTH
                        : ((GET_EVENT_PARAM()->call_event.call_info).alpha.len));
            STRNCPY(abTemp, 
                    (GET_EVENT_PARAM()->call_event.call_info).alpha.buf, nLength);
            STRCPY(abPhoneNumber, abTemp + 12);
            sInProgExit();
            SetupCLICDNIP((CHAR *) abPhoneNumber, nLength, 
                          GET_EVENT_PARAM()->call_event.call_info.num.pi, pEvent->EventID);
            EQS_StartTimer(EQS_TIMER_3, 200, FALSE);
        }
        break;

    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
#endif 
        /* call waiting, Save Incoming Phone Number in Call List */
        sInProgExit();
        if(pEvent->EventID != SE_CALL_EXT_DISPLAY)
        {
            SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);
        }
        EQS_StartTimer(EQS_TIMER_3, 200, FALSE);
        break;    

    // 속삭임 모드
    case SE_KEY_MANNERS:  
        if (!ISVOICE(l_CallInfo.cm_info.call_type) ) 
        break;

        if(l_fWhisperMode)
        {
            l_fWhisperMode = FALSE;
        }
        else
        {
            l_fWhisperMode = TRUE;
        }
        SKY_SetSoundDevice( SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, FALSE );
        EQS_END();
        break;

#ifdef FEATURE_SKTT_EQUS_LOCATION
   case SE_LOCATION_TIMEOUT:
        CheckCurrentTime();
        break;
#endif //#ifdef FEATURE_SKTT_LOCATION

    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}

#ifndef SKTT_RELEASE
/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetTestData(void)
**
** Description: 
**     CNIP, CNAP, CDNIP의 디스플레이를 테스트 하기 위해 만들어 진 함수 
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
LOCAL void sSetTestData(void)
{
    CNI_T*       pCallerID;
    
    pCallerID = SKY_GetCallerID();
  
    ClearCLI();
    
    
    SKY_SET_NUMBER_PLUS(TRUE);
    //SKY_SET_NUMBER_PLUS(FALSE);


    //pCallerID->bPI = CAI_ALLOW_PI;
    pCallerID->bPI = CAI_RES_PI;

    /*
      투넘버서비스를 위한 데이터를 Set 한다.
    */
    memcpy(pCallerID->CalledParty.aBuf, (BYTE*)"01122933333", 11);
    pCallerID->CalledParty.aBuf[11] = '\0';
    pCallerID->CalledParty.nPos     = 11;
    pCallerID->CalledParty.fSecret     = FALSE;

    /* 
      CNIP를 위한 데이터를 Set 한다.
    */
//    memcpy(pCallerID->Num.aBuf, (BYTE*)"01122935255", 11);
//    pCallerID->Num.aBuf[11] = '\0';
//    pCallerID->Num.nPos     = 11;
//    pCallerID->Num.fSecret = TRUE;

    memcpy(pCallerID->Num.aBuf, (BYTE*)"", 0);
    pCallerID->Num.aBuf[0] = '\0';
    pCallerID->Num.nPos     = 0;
    pCallerID->Num.fSecret = FALSE;

//      memcpy(pCallerID->Num.aBuf, (BYTE*)"발신번호표시금지", STRLEN("발신번호표시금지"));
//      pCallerID->Num.aBuf[STRLEN("발신번호표시금지")] = '\0';
//      pCallerID->Num.nPos     = STRLEN("발신번호표시금지");
//      pCallerID->Num.fSecret = FALSE;

    /*
      CNAP를 위한 데이터를 Set 한다.
    */
//    STRCPY(pCallerID->aName, (BYTE*) "조형준만세만1");
//    pCallerID->bNameLen       =  STRLEN(pCallerID->aName);
    
    /* 
      PhoneBook에서 찾은 이름을 Set 한다.
    */
//    STRCPY(pCallerID->aNameInPhoneBook, (BYTE*) "조형준만세만2");
//    pCallerID->bNameLenInPhoneBook = STRLEN(pCallerID->aNameInPhoneBook);
}
#endif// SKTT_RELEASE



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
LOCAL BOOL SKY_API sInitCallWaitingCallDisp(void)
{
    HCONTROL    hControl;
    BYTE        pszTmp[20];
    CNI_T*      pCallerID;
    BOOL        fBlinkName, fCNIP, fCDNIP, fCNIR, fCNAP, fPHONEBOOK;
    UINT8       nStaticText4Name;
    UINT8       nNumberEditor4PhoneNumber;

    nStaticText4Name = 0;
    nNumberEditor4PhoneNumber = 0;
    
    fBlinkName = FALSE;
    fCNIP      = FALSE;
    fCDNIP     = FALSE;
    fCNIR      = FALSE;
    fCNAP      = FALSE;
    fPHONEBOOK = FALSE;
    pCallerID = SKY_GetCallerID();  

    /* 현재시각 */ 
    sMakeDateTimeString(TRUE, pszTmp);
    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, (EQS_TOKEN)TKN_NULL);
    EQC_SetStaticText(hControl, (BYTE*) pszTmp);
    
    /* 통화시간 */
    EQC_StaticText(sCALL_TIME_XPOS, sCALL_TIME_YPOS, (EQS_TOKEN)TKN_NULL);
    fBlinkName = FALSE;

#ifndef SKTT_RELEASE
    /* 테스트를 위한 코드 --> Start */
    //sSetTestData();
    /* 테스트를 위한 코드 --> End   */
#endif// SKTT_RELEASE

    /* Two Number Service가 설정되어 있고  Called Party에  */
    /* 표시할 정보가 있는 경우                             */
    if(SKY_IS_NUMBER_PLUS() && (pCallerID->CalledParty.aBuf[0] != '\0'))  
    {
        /* 이름정보가 있을 경우 */
        if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))
        {           
           nStaticText4Name++;
           fCNAP      = TRUE;
           fPHONEBOOK = TRUE;

           if(STRCMP(pCallerID->aName, pCallerID->aNameInPhoneBook) != 0)
           {
             fBlinkName = TRUE;
           }
        }
        else if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] == '\0'))            
        {          
          /* CNAP만 날아온 경우 */
           nStaticText4Name++;
           fCNAP      = TRUE;
        }
        else if((pCallerID->aName[0] == '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))                                           
        {
          /* 전화부에 있는 이름을 검색한 경우 */
           nStaticText4Name++;
           fPHONEBOOK  = TRUE;
        }

        if(pCallerID->Num.aBuf[0] != '\0')  
        {
            if(pCallerID->bPI == CAI_ALLOW_PI)  
            {        
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else                        /* 발신번호표시불가능 또는 금지 */
            {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
            }

            fCDNIP = TRUE;
            nNumberEditor4PhoneNumber++;
        }
        else
        {
            fCDNIP = TRUE;
            nNumberEditor4PhoneNumber++;
        }
    }
    else /* Called Party에 표시할 정보가 없을 경우에는 CNAP와 CNIP만 보여준다. */
    {
        if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))       
        {
            /* CNAP 정보와 전화부에 있는 이름이 틀릴 경우 */
            if(STRCMP(pCallerID->aName, pCallerID->aNameInPhoneBook) != 0)
            {
                nStaticText4Name++;
                fCNAP = TRUE;
                fPHONEBOOK = TRUE;
                fBlinkName = TRUE;
            }
            else  /* CNAP로 날아온 이름과 전화부에 저장되어 있는 이름이 동일한 경우 */
            {
                nStaticText4Name++;
                fPHONEBOOK = TRUE;
                fCNAP = TRUE;
            }
        }
        else if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] == '\0'))            
        {          
            /* CNAP만 날아온 경우 */
            nStaticText4Name++;
            fCNAP = TRUE;
        }
        else if((pCallerID->aName[0] == '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))                                           
        {
            nStaticText4Name++;
            fPHONEBOOK = TRUE;
        }

        if(pCallerID->Num.aBuf[0] != '\0')      /* CNIP 정보가 버퍼에 들어있는 경우 */
        {
            if(pCallerID->bPI == CAI_ALLOW_PI)  /* 발신번호표시가능 */
            {
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else                               /* 발신번호표시불가능 또는 금지 */
            {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
            }      
        }
    }



    if(nNumberEditor4PhoneNumber == 0)         /* Number Editor가 없는 경우 */
    {
        if(nStaticText4Name == 1)              /* CNAP만 있는 경우 */
        {
            if(fCNAP)
            {
              sCreateNameAndNoPhoneNumber(CD_CALL_WAITING_CALL, 
                                          CN_CNAP_NAME, 
                                          pCallerID->aName);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_CALL_WAITING_CALL, 
                                          CN_PHONEBOOK_NAME, 
                                          pCallerID->aNameInPhoneBook);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_CALL_WAITING_CALL, 
                                          CN_NO_NAME, 
                                          pCallerID->aNameInPhoneBook);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 1)    /* Number Editor가 하나인 경우 */
    {        
        if(fCNIP)
        {
            if(fCNAP)
            {            
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CNIP, 
                                            pCallerID->Num.aBuf);

            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
        }
        else if(fCDNIP)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 2)    /* Number Editor가 두개인 경우 */
    {
        if(fCNIP)
        {
            if(fCNAP)                         
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_CNAP_NAME,  
                                            pCallerID->aName,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_PHONEBOOK_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_NO_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_CNAP_NAME,  
                                            pCallerID->aName,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_PHONEBOOK_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_CALL_WAITING_CALL,
                                            CN_NO_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
        }
    }

    return fBlinkName;
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     Call Waiting Call이 왔을 경우 착신 정보를 10초간 보여준다.
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
void SKY_API SKA_CallWaitingCall(SKY_EVENT_T *pEvent)
{
    STATIC BYTE          bCallWaitingTime;
    STATIC BOOL          fCallFromCallRecording, fBlinkName;
    STATIC DWORD         dwRemainTime;
    STATIC WORD          wRecordingTime;
    STATIC UINT8         nBlink;
    HCONTROL             hControl;

    
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START :    /* The very first event */
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_NONE, 
                      NULL, // show annunciators
                      WA_SHOW_HEADER);
        fBlinkName = sInitCallWaitingCallDisp();
        bCallWaitingTime = 0;           
        dwRemainTime   = 0;
        wRecordingTime = 0;
        nBlink = 0;
        if(EQS_GET_PARENT() == SKA_StartCallRecording)
        {
            fCallFromCallRecording = TRUE;
        }
        else
        {
            fCallFromCallRecording = FALSE;
        }
        sSaveCLI(CT_CALLED_BY_INPROG);        
        break;

    case SE_APP_RESUME:
        break;

    case SE_APP_END :                /* The very last event */
        ClearCLI();
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
                break;

    case SE_APP_DRAW :
        DisplayCallTime(EQS_GetNthIOC(CWC_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
        break;
        
    case SE_TM_TIMER1 :
       if(bCallWaitingTime < 20)
       {
         if(fCallFromCallRecording == TRUE)
         {
            dwRemainTime--;
            wRecordingTime++;
            Data_Record_time++;
         }
         
         if(fBlinkName && ((bCallWaitingTime % 2) == 0))
         {
            if((nBlink%2) == 0)
            {
               nBlink++;
               sDisplayNameInPhoneBook(DN_CALL_WAITING_CALL);
            }
            else
            {
               nBlink++;
               sDisplayNameInCNAP(DN_CALL_WAITING_CALL);
            }                      
         }

         bCallWaitingTime++;
         EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
       }
       else
       {
        EQS_StopTimer(EQS_TIMER_1);
        if(fCallFromCallRecording == TRUE)
        {
            EQS_END_PARAM(wRecordingTime, dwRemainTime);
        }
        else
        {
            EQS_END();
        }
       }
       break;    

    case SE_CALL_FROM_CALLRECORDING:      /* This event comes from  */
        dwRemainTime   = pEvent->dwParam; /* SKA_StartCallRecording */
        wRecordingTime = pEvent->wParam;
        break;

    case SE_KEY_RECORD:
        if(fCallFromCallRecording == TRUE)
        {
            EQS_END_EVENT(pEvent->EventID, wRecordingTime, dwRemainTime);
        }
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
    case SE_KEY_CLEAR:
        break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
    case SE_CALL_RELEASED:
    case SE_KEY_END:
    case SE_KEY_SEND:
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_PHONE_CLOSE:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }

        if(fCallFromCallRecording == TRUE)
        {
            EQS_END_EVENT(pEvent->EventID, wRecordingTime, dwRemainTime);
        }
        else
        {
            EQS_END_CURRENT_EVENT();
        }
        DataManagerEnd(pEvent);
        break;    

    case SE_SYS_TO_IDLE :  /* Ignore this event */
        break;    

    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;
    }
}





/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     InitInProgDisplayInDataService()
**
** Description:  
**     Initialize Data Service Display in InProg mode
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
#define sDATA_SERVICE_TIME_NUM_X    (sDATA_SERVICE_TIME_X + (EQS_CharWidth('A')*STRLEN(SKY_GET_TOKEN(TKN_CALL_DS_CONNECT))))

void SKY_API InitInProgDisplayInDataService(void)
{
    HCONTROL hControl;

    /* 애니메이션 */
	hControl = EQC_Animation(sDATA_SERVICE_ANI_X, sDATA_SERVICE_ANI_Y, &g_AniInDataService, NULL);
	EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);		
	/* 프레임 */
	EQC_Image(sDATA_SERVICE_FRAME_X, sDATA_SERVICE_FRAME_Y, (BITMAP*)&g_BmpCallPannel);

    EQC_SetStaticText(EQC_StaticText(sDATA_SERVICE_TIME_X, sDATA_SERVICE_TIME_Y, TKN_NULL),
                      (BYTE*)SKY_GET_TOKEN(TKN_CALL_DS_CONNECT));
    
    EQC_StaticText(sDATA_SERVICE_TIME_NUM_X, sDATA_SERVICE_TIME_Y, TKN_NULL);         //IP_TIME
   
#ifdef FEATURE_SKT_AIRDATA  
    if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
    {
        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_WAIT_AIR_DATAMGR1);
    }
    else
#endif //#ifdef FEATURE_SKT_AIRDATA
#ifdef  FEATURE_IP_AIRDATA      
    if (SKY_GET_IP_AIRDATAMANAGER())
    {
        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_IP_DATAMGR_TXT);
    }
    else
#endif  // #ifdef       FEATURE_IP_AIRDATA          
    {
        if( DS_ISDATA_RS1(l_CallInfo.cm_info.srv_opt))
        {
              /* rate is 9.6 */
            if (l_CallInfo.cm_info.srv_opt == CAI_SO_PPP_PKT_DATA_3G)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_MDR_PKT_DATA_FRS1_RRS1)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_64);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
                if ( STRCMP((BYTE *)l_CallInfo.num.buf , (BYTE *)"1501") ==0)
                {
                        EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9PACKET);
                }
                else
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9MODEM);
                }
            }
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_DATA)                  
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PIWF);
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PACKET);
            }
            else
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9DATA);
            }
        }
        else if( DS_ISDATA_RS2(l_CallInfo.cm_info.srv_opt))
        {
              /* rate is 14.4 */
            if (l_CallInfo.cm_info.srv_opt==CAI_SO_PPP_PKT_DATA_3G)
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_91);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
                if ( STRCMP((BYTE*)l_CallInfo.num.buf, (BYTE *)"1501") == 0 )
                {
                   EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14PACKET);
                }
                else
                {
                   EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14MODEM);
                }
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
                 EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14PACKET);
            }
            else
            {
                 EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14DATA);
            }
        }
        else
        {
            /* rate is unknown */
            EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING);
        }   
    }
    
    EQC_SET_STYLE(EQS_GetNthIOC(DS_STATE), (EQC_GET_STYLE(EQS_GetNthIOC(DS_STATE)) |CS_STATIC_TEXT_ORING));
    EQC_SET_STYLE(EQS_GetNthIOC(DS_TIME), (EQC_GET_STYLE(EQS_GetNthIOC(DS_TIME)) |CS_STATIC_TEXT_ORING));
    EQC_SET_STYLE(EQS_GetNthIOC(DS_TIME_TITLE), (EQC_GET_STYLE(EQS_GetNthIOC(DS_TIME_TITLE)) |CS_STATIC_TEXT_ORING));
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
void UpdateInProgDisplayInDataService(void)
{
#ifdef FEATURE_SKT_AIRDATA  
        if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
        {
                return;
        }
        else
#endif //#ifdef FEATURE_SKT_AIRDATA
#ifdef  FEATURE_IP_AIRDATA      
        if (SKY_GET_IP_AIRDATAMANAGER())
        {
                return;
        }
        else
#endif  // #ifdef       FEATURE_IP_AIRDATA          
    {
        EQS_DeleteControl(EQS_GetNthIOC(DS_STATE));

        l_CallInfo.cm_info.srv_opt = 0x21;

        if( DS_ISDATA_RS1(l_CallInfo.cm_info.srv_opt))
        {
            /* rate is 9.6 */
            if (l_CallInfo.cm_info.srv_opt == CAI_SO_PPP_PKT_DATA_3G)
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_MDR_PKT_DATA_FRS1_RRS1)
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_64);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
                if ( STRCMP((BYTE *)l_CallInfo.num.buf , (BYTE *)"1501") ==0)
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9PACKET);
                }
                else
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_9MODEM);
                }
            }
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_DATA)                  
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PIWF);
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PACKET);
            }
            else
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9DATA);
            }
        }
        else if( DS_ISDATA_RS2(l_CallInfo.cm_info.srv_opt))
        {
            /* rate is 14.4 */
            if (l_CallInfo.cm_info.srv_opt==CAI_SO_PPP_PKT_DATA_3G)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_91);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
                if ( STRCMP((BYTE*)l_CallInfo.num.buf, (BYTE *)"1501") == 0 )
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14PACKET);
                }
                else
                {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_ORIGINATE_14MODEM);
                }
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14PACKET);
            }
            else
            {
                EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14DATA);
            }
        }
        else
        {
          /* rate is unknown */
          EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING);
        }   

        EQC_SET_STYLE(EQS_GetNthIOC(DS_STATE), CS_STATIC_TEXT_ORING);
    }
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sSetInProgExtendedDispTest(void)
**
** Description: 
**     통화 중 화면에서 CNIP, CDNIP, CNIR, CNAP등을 테스트 하기 위해서 만들어진
**     함수이다.
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
#ifndef SKTT_RELEASE
LOCAL void sSetInProgExtendedDispTest(void)
{
    l_CallInfo.ui_try_cnt              = 0;
    l_CallInfo.secret                  = FALSE;
    l_CallInfo.called_party_num.len    = 0;
    l_CallInfo.num.len                 = 0;
    l_CallInfo.alpha.len               = 0;
    l_CallInfo.alpha_cnap.len          = 0;
    l_CallInfo.connect_time            = 0;

    l_CallInfo.alpha.buf[0]            = '\0';
    l_CallInfo.alpha_cnap.buf[0]       = '\0';
    l_CallInfo.num.buf[0]              = '\0';
    l_CallInfo.called_party_num.buf[0] = '\0';
    
    l_CallInfo.secret                  = FALSE;    


    SKY_SET_NUMBER_PLUS(TRUE);
        //SKY_SET_NUMBER_PLUS(FALSE);
        //l_CallInfo.bPI = CAI_RES_PI;
        l_CallInfo.bPI = CAI_ALLOW_PI;

    /*
      투넘버서비스를 위한 데이터를 Set 한다.
    */
    memcpy(l_CallInfo.called_party_num.buf, (BYTE*)"01122933333", 11);
    l_CallInfo.called_party_num.len  = 11;

    /* 
      CNIP를 위한 데이터를 Set 한다.
    */
    memcpy(l_CallInfo.num.buf, (BYTE*)"01122935255", 11);
    l_CallInfo.num.len = 11;

    /*
      CNAP를 위한 데이터를 Set 한다.
    */
    //STRCPY(l_CallInfo.alpha_cnap.buf, (BYTE*) "조형준1");
    //l_CallInfo.alpha_cnap.len = STRLEN(l_CallInfo.alpha_cnap.buf);

    /* 
      PhoneBook에서 찾은 이름을 Set 한다.
    */
    //STRCPY(l_CallInfo.alpha.buf, (BYTE*) "조형준2");
    //l_CallInfo.alpha.len      = STRLEN(l_CallInfo.alpha.buf);
}
#endif// SKTT_RELEASE



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitInProgDisplayInVoiceCallFromAlert(void)
**
** Description: 
**     Alert을 통해 Call이 왔을 경우에는 통화중 상태에서도 CNIP, CNAP, CDNIP, 
**     등을 표시해 주어야 함 (Calling Feature 단말기 요구사항 Ver. CAF-1.2)
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
LOCAL void SKY_API sInitInProgDisplayInVoiceCallFromAlert(void)
{
    HCONTROL  hControl;
    BYTE      pszTmp[20];
    BOOL      fBlinkName, fCNIP, fCDNIP, fCNIR, fCNAP, fPHONEBOOK;
    UINT8     nStaticText4Name;
    UINT8     nNumberEditor4PhoneNumber;

    fBlinkName = FALSE;
    fCNIP      = FALSE;
    fCDNIP     = FALSE;
    fCNIR      = FALSE;
    fCNAP      = FALSE;
    fPHONEBOOK = FALSE;

    nStaticText4Name = 0;
    nNumberEditor4PhoneNumber = 0;

#ifndef SKTT_RELEASE    
    /* hjcho-test--> 테스트 용 --> */
    //sSetInProgExtendedDispTest(); 
    /* hjcho-test--> 테스트 용 --> */
#endif//SKTT_RELEASE

    if(SKY_IS_NUMBER_PLUS())
    {
        if(l_CallInfo.called_party_num.buf[0] != '\0')
        {
            if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
            {
                fCNAP = TRUE;
                fPHONEBOOK = TRUE;
                fBlinkName = TRUE;
                nStaticText4Name++;
            }
            else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
            {         
                fPHONEBOOK = TRUE;
                nStaticText4Name++;
            }
            else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
            {
                fCNAP = TRUE;
                nStaticText4Name++;
            }

            if(l_CallInfo.num.buf[0] != '\0')
            {
                if(l_CallInfo.bPI == CAI_ALLOW_PI)
                {
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;
                }
                else //presentation is not allowed or restricted!!
                {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
                }

                fCDNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else
            {          
                fCDNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
        }
        else  /* Called Party에 번호가 들어 있지 않을 경우 */
        {
            if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
            {
                fCNAP      = TRUE;
                fPHONEBOOK = TRUE;
                fBlinkName = TRUE;
                nStaticText4Name++;
            }
            else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
            {
                fPHONEBOOK = TRUE;         
                nStaticText4Name++;
            }
            else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
            {
                fCNAP = TRUE;
                nStaticText4Name++;
            }


            if(l_CallInfo.num.buf[0] != '\0')
            {
                if(l_CallInfo.bPI == CAI_ALLOW_PI)
                {                     
                    fCNIP = TRUE;
                    nNumberEditor4PhoneNumber++;
                }
                else
                {
                    fCNIR = TRUE;
                    nNumberEditor4PhoneNumber++;               
                }
            }
        }
    }
    else // Number Plus가 설정되어 있지 않을 경우
    {
        if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {
            fCNAP      = TRUE;
            fPHONEBOOK = TRUE;
            fBlinkName = TRUE;
            nStaticText4Name++;
        }
        else if((l_CallInfo.alpha.buf[0] != '\0') && (l_CallInfo.alpha_cnap.buf[0] == '\0'))
        {
            fPHONEBOOK = TRUE;
            nStaticText4Name++;        
        }
        else if((l_CallInfo.alpha.buf[0] == '\0') && (l_CallInfo.alpha_cnap.buf[0] != '\0'))
        {        
            fCNAP = TRUE;  
            nStaticText4Name++;
        }

        if(l_CallInfo.num.buf[0] != '\0')
        {
            if(l_CallInfo.bPI == CAI_ALLOW_PI)
            {
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else
            {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
            }
        }
    }

    if(nNumberEditor4PhoneNumber == 0)         /* Number Editor가 없는 경우 */
    {
        if(nStaticText4Name == 1)              /* CNAP만 있는 경우 */
        {
            if(fCNAP)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_CNAP_NAME, 
                                          l_CallInfo.alpha_cnap.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_PHONEBOOK_NAME, 
                                          l_CallInfo.alpha.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_INPROG, 
                                          CN_NO_NAME, 
                                          l_CallInfo.alpha.buf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 1)    /* Number Editor가 하나인 경우 */
    {        
        if(fCNIP)
        {
            if(fCNAP)
            {            
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIP, 
                                            l_CallInfo.num.buf);

            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR, 
                                            l_CallInfo.num.buf);
            }
        }
        else if(fCDNIP)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,   
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,   
                                            l_CallInfo.alpha.buf,
                                            CN_CDNI, 
                                            l_CallInfo.called_party_num.buf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 2)    /* Number Editor가 두개인 경우 */
    {
        if(fCNIP)
        {
            if(fCNAP)                         
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,  
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIP,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_CNAP_NAME,  
                                            l_CallInfo.alpha_cnap.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_PHONEBOOK_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_INPROG,
                                            CN_NO_NAME,  
                                            l_CallInfo.alpha.buf,
                                            CN_CNIR,
                                            l_CallInfo.num.buf,  
                                            l_CallInfo.called_party_num.buf);
            }
        }
    }

    /* 현재시각 */  
    sMakeDateTimeString(TRUE, pszTmp);
    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                             (EQS_TOKEN)TKN_NULL);
    EQC_SetStaticText(hControl, (BYTE*) pszTmp);

    /* 통화시간 */
    hControl =  EQC_StaticText(sCALL_TIME_XPOS, 
                               sCALL_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL);    
    DisplayCallTime(hControl);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sInitInProgDisplayInVoiceCallFromOrigination(void)
**
** Description: 
**     Initialize Voice Call Display in InProg mode
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
LOCAL void sInitInProgDisplayInVoiceCallFromOrigination(void)
{
    HCONTROL hControl;
    BYTE     pszTmpBuf[20];

    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                              TKN_NULL);     

    sMakeDateTimeString(TRUE, pszTmpBuf);
    EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf);


    hControl = EQC_StaticText(sORIGINATION_TIME_XPOS, 
                              sORIGINATION_TIME_YPOS, 
                              (EQS_TOKEN)TKN_NULL); 
    DisplayCallTime(hControl);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
	 	     (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//    EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT|
                            CS_NUMBER_EDIT_NO_CML|
                            CS_NUMBER_EDIT_RETURN_UPDOWN_KEY);
//                            CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font
    sDisplayPhoneNumber(hControl);
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
LOCAL void sInintBurstDTMFWithMyNumberDisplay(void)
{
    HCONTROL hControl;
    BYTE     pszTmpBuf[20];


    hControl = EQC_StaticText(sCURRENT_TIME_XPOS, 
                              sCURRENT_TIME_YPOS, 
                              TKN_NULL); 

    sMakeDateTimeString(TRUE, pszTmpBuf);
    EQC_SetStaticText(hControl, (BYTE*) pszTmpBuf);

    hControl =  EQC_StaticText(sORIGINATION_TIME_XPOS, 
                               sORIGINATION_TIME_YPOS, 
                               (EQS_TOKEN)TKN_NULL);         
    DisplayCallTime(hControl);

    EQC_Image(sORIGINATION_PHONE_OPEN_XPOS, 
		      sORIGINATION_PHONE_OPEN_YPOS, 
			  (BITMAP*)&g_BmpPhoneOpen);

	EQC_Image(sORIGINATION_IMAGE_XPOS, 
	          sORIGINATION_IMAGE_YPOS, 
			  (BITMAP*)&g_BmpCallPannel);

    hControl = EQC_NumberEdit(DIAL_DIGIT_POS_X, 
                              DIAL_DIGIT_POS_Y, 
                              DIAL_DIGIT_LENGTH_PER_LINE, 
                              DIAL_DIGIT_LINE, MAX_DIAL_DIGIT_LENGTH,(BYTE*)"" );
//    EQC_SetNumberEditBackColor(hControl, (COLOR)NUMBER_EDIT_BACK_COLOR);
    EQC_SetNumberEditBackColor(hControl, NUMBER_PANNEL_COLOR);
    EQC_SetNumberEditColor(hControl, NUMBER_TEXT_COLOR);

    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_RIGHT2LEFT |
                            CS_NUMBER_EDIT_NO_CML     |
                            CS_NUMBER_EDIT_ALONE      );
//                            CS_NUMBER_EDIT_ORING);
    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SetNumberEditBigFont(hControl, TRUE);            // Set Big Font    
    sBurstDTMFWithMyNumber();
    sDisplaySomePhoneNumber(hControl); 
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
void SKY_API SKA_BurstDTMFWithMyNumber(SKY_EVENT_T *pEvent)
{
  HCONTROL hControl;

  switch(pEvent->EventID)
  {
    case SE_APP_START :
        EQS_GetDC(TRUE);
        EQS_SetWindow(SKY_DEF_WIN_X,
                  SKY_DEF_WIN_Y, 
                  SKY_DEF_WIN_DX, 
                  SKY_DEF_WIN_DY, 
                  CL_NONE, /* 메뉴  볼륨  전화부*/
                  NULL,                  /* show annunciator */
                  WA_SHOW_HEADER);
        sInintBurstDTMFWithMyNumberDisplay();
        EQS_StartTimer(EQS_TIMER_1, sONE_SECOND, TRUE);
        break;

    case SE_APP_END:
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
        break;

    case SE_APP_DRAW:
        sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));
        DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
//        DrawGeneralFrame();
        break;

    case SE_TM_TIMER1:
        EQS_SEND_EVENT(SE_APP_DRAW, 0, 0);
        break;

    case SE_SND_MULTI_TONE_DONE :    //자기번호발신이 끝난 경우 
        SKY_SetSoundDevice(SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, TRUE);
        EQS_END();
        break;

    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
    case SE_KEY_CLEAR:
        break;

    case SE_CALL_ENDED :        //UI_CALL_ENDED_E:
    case SE_CALL_DISPLAY :
    case SE_CALL_CALLER_ID :    //UI_CALL_CALLER_ID_E:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY :  //UI_CALL_EXT_DISPLAY_E:
#endif
    case SE_KEY_PHONE_CLOSE:
    case SE_KEY_END:
    case SE_SYS_TO_IDLE :       //UI_TOIDLE_F:
    case SE_KEY_RECORD_LONG :
    case SE_KEY_EAR_LONG_CLICK:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        EQS_END_CURRENT_EVENT();
        break;

    default :
        CallDefaultEventProc(pEvent, INPROG);
        break;    
   }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     EndCall
**
** Description: 
**  This applet integrate  InProgModeEntry and in_prog_mode_event function
**  in QualComm.
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
void SKY_API EndCall(void)
{
    HCONTROL hControl;    

    sEndConversation();
    sInProgExit();

    /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
    /* 통화종료화면이 나오지 않는 현상 방지                            */
    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
        EQS_DeleteControl(hControl);
    }

    sCallEndSetup(); //call_end_setup();    
}

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_InProg
**
** Description: 
**  This applet integrate  InProgModeEntry and in_prog_mode_event function
**  in QualComm.
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
#ifndef WIN32
extern void SvrLinkRelease(void);
extern void PtpLinkRelease(void);
#endif

#define sFAST_PACEMAKER_SECOND      1000    /* 1 sec */


void SKY_API SKA_InProg(SKY_EVENT_T *pEvent)
{
    CNI_T*          pCallerID; 
    BYTE            szTmp[10];
    HCONTROL        hControl;
    STATIC BOOL     s_fEnableHoldOn, s_fEnableCallRecording;

    SKY_TOKEN aHotMenuSTI[4] = {        /* Hot Menu!! */
        TKN_CALL_HOT_MENU1,          /* 자기번호발신   */
        TKN_CALL_HOT_MENU2,          /* 송화음차단     */
        TKN_CALL_HOT_MENU3,          /* 호전환서비스   */
        TKN_CALL_HOT_STI_MENU4       /* 통화중대기정지 */
        };

    SKY_TOKEN aHotMenuSKT[4] = {        /* Hot Menu!! */
        TKN_CALL_HOT_MENU1,          /* 자기번호발신   */
        TKN_CALL_HOT_MENU2,          /* 송화음차단     */
        TKN_CALL_HOT_MENU3,          /* 호전환서비스   */
        TKN_CALL_HOT_MENU4           /* 호보류서비스   */
        };


#ifdef FEATURE_SKT_AIRDATA
    STATIC BOOL     s_fAirDMRelease = FALSE;
#endif    
    switch(pEvent->EventID)
    {     
    /* NC_APP */
    case SE_APP_START :
        SKY_SetGeneralTimer(1000, TRUE);
        EQS_GetDC(TRUE);
        sInProgModeEntry();                
        EQS_SetWindow(SKY_DEF_WIN_X,
                      SKY_DEF_WIN_Y, 
                      SKY_DEF_WIN_DX, 
                      SKY_DEF_WIN_DY, 
                      CL_MENU_VOLUME_PBOOK, /* 메뉴  볼륨  전화부*/
                      NULL,                  /* show annunciator */
                      (WA_SHOW_HEADER | WA_SHOW_CML));   
        
        s_fAirDMRelease = FALSE;
        s_fEnableHoldOn = TRUE;
        s_fEnableCallRecording = TRUE;
        SKY_DrawSubAnnunciators();

        if (SKY_IS_DEBUG_SCREEN())
        {
            if( SKY_IS_AUTO_ANSWER() && !GetAutoAnswering() 
                && l_fCallFromAlert  && ISVOICE(l_CallInfo.cm_info.call_type)
                && !l_fKeyDownInAlert
                && IsSpace2RecordAutoAnswer())
            { 
                /* 자동응답 */
                SetAutoAnswering(TRUE);
                EQS_START_SIBLING(SKA_RecordingAutoAnswer,FALSE);
            }
        }
#ifdef FEATURE_DS   /* 데이터 서비스  */
        else if(sIsDataCall())// && !SKY_IS_AUTO_ANSWER())
        {          
				InitInProgDisplayInDataService();
        }
#endif /* FEATURE_DS */
        else if(   SKY_IS_AUTO_ANSWER() && !GetAutoAnswering() 
                && l_fCallFromAlert  && ISVOICE(l_CallInfo.cm_info.call_type)
                && !l_fKeyDownInAlert
                && IsSpace2RecordAutoAnswer())
        { 
            /* 자동응답 */
            SetAutoAnswering(TRUE);
            EQS_START_SIBLING(SKA_RecordingAutoAnswer,FALSE);
        }
        /* 음성통화, 테스트 콜 */
        else if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
        {         
            if(l_PausedMode == PM_PPAUSE)                 /* P-pause 일 경우 */
            {
                /* 문자 P까지 나와야 한다 */
                SetRealPhoneNumberLength((GetRealPhoneNumberLength()+1)); 
                EQS_START_SIBLING(sSKA_Paused, FALSE);
            }
            else if(l_PausedMode == PM_TPAUSE)           /* T-pause 일 경우 */
            {
                /* 문자 T까지 나와야 한다 */
                SetRealPhoneNumberLength((GetRealPhoneNumberLength()+1)); 
                EQS_START_SIBLING(sSKA_MultiWait, FALSE);
            }
            else                                            /*  일반음성통화   */
            {
                if(l_fCallFromAlert == FALSE) 
                {
                    /* Origination Call 일 경우에는 Number Editor가 2줄까지        */
                    /* 나와야한다.                                                 */
                    sInitInProgDisplayInVoiceCallFromOrigination();          
                }
                else                              
                {
                    /* Extended Display (CNIP, CNAP, CDNIP)를 모두 표시하기        */
                    /* 위해서는 Origination Call과 Alert Call로 나누어야 한다.     */
                    sInitInProgDisplayInVoiceCallFromAlert();
                }
            }
        }
        break;

    case SE_APP_END :
        SKY_SetGeneralTimer(0, FALSE);
        s_fAirDMRelease = FALSE;
        s_fEnableCallRecording = TRUE;
        SKY_SET_AIRDATAMANAGER_ORG(FALSE);
        SKY_SET_AIRDATAMANAGER_RCV(FALSE);
        SKY_SubLCDEndText(LN_LOWER); /* 현재 시각 표시해도 좋음 */
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND:
        s_fEnableCallRecording = TRUE;
        SKY_SetGeneralTimer(0, FALSE);
        break;

    case SE_APP_RESUME:
        s_fAirDMRelease = FALSE;
        s_fEnableCallRecording = TRUE;
        SKY_SetGeneralTimer(1000, TRUE);
        SKY_AcceptKey(TRUE);
        if(EQS_GET_CHILD() == SKA_BookInCalling)
        {
            SKY_SET_TRANSMIT_DTMF(TRUE);
            /* restore pacemaker repeat time */
            SKY_SetPaceMakerTimer(CLOCK_PACEMAKER_REPEAT_TIME, TRUE);
        }
        else if(EQS_GET_CHILD() == SKA_MainClid)
        {
          pCallerID = SKY_GetCallerID();
          if(IsData2DisplayInCallWaitingCall())
          {
            EQS_START_CHILD(SKA_CallWaitingCall);
          }
        }
        else if(EQS_GET_CHILD() == SKA_AdjstConversationVol)
        {
            s_fEnableHoldOn = FALSE;
        }
        else if(EQS_GET_CHILD() == SKA_MainHoldOn)
        {
            s_fEnableHoldOn = FALSE;
        }
        else if(EQS_GET_CHILD() == SKA_MainCallRecord)
        {
            
            /* 통화 중 녹음 후 RECORD KEY를 눌러서 녹음을 저장한 경우  */
            /* 녹음 저장시간으로 인해 SE_KEY_RECORD_LONG 이벤트가      */
            /* 다시 발생해서 사용자가 의도하지 않은 통화 중 녹음이 실행*/
            /* 되는 경우가 있다. 이를 방지하기 위해서 Flag를 이용한다. */
            s_fEnableCallRecording = FALSE;
            EQS_StartTimer(EQS_TIMER_3, 2000, FALSE);
        }
        /* RESUME 되었을 경우 Call Time Update를 위해 */
        /* fall through ...                           */

     case SE_CALL_SIGNAL:
     case SE_TM_TIMER:
        if (SKY_IS_DEBUG_SCREEN())
        {
            EQS_SEND_EVENT(SE_APP_DRAW, 0, 0);
            break;
        }

        if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
        {
            if(l_fCallFromAlert == FALSE)
            {
                DisplayCallTime(EQS_GetNthIOC(IFO_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
                sDisplayCurrentDateTime(EQS_GetNthIOC(IFO_CURRENT_TIME));                               
            }
            else
            {                
                DisplayCallTime(EQS_GetNthIOC(IDCT_CALL_TIME)); /* SUB LCD의 LOWER에 통화 시간 표시 */
                sDisplayCurrentDateTime(EQS_GetNthIOC(IDCT_CURRENT_TIME));
            }                       
        }
        else if(sIsDataCall())      /* 데이터 서비스 접속의 경우 */
        {
          DisplayCallTime(EQS_GetNthIOC(DS_TIME));
          EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_ALL, EQS_NA);
        }
        break;

     case SE_TM_TIMER2 :
        EQS_START_CHILD(SKA_MainClid);
        break;

     case SE_TM_TIMER3:
        s_fEnableCallRecording = TRUE;
        break;

     case SE_APP_DRAW:
        if (SKY_IS_DEBUG_SCREEN())
        {
            SKY_DrawDebugScreen();
        }
        else if(l_fWhisperMode == TRUE) /* 속삭임설정 */
        {
            sprintf((char*)szTmp, "%s%s",SF_WHISPER1_2,SF_WHISPER2_2);
    
            EQS_TextOutStr(sWHISPER_XPOS, sWHISPER_YPOS, (BYTE *)szTmp);
        }

//        if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
//        {
//            if(l_fCallFromAlert == FALSE)
//            {
//                DrawGeneralFrame();
//            }
//        }
//        else if(sIsDataCall())      /* 데이터 서비스 접속의 경우 */
//        {
//            DrawGeneralFrame();
//        }
        break;

    case SE_KEY_LEFT:
    case SE_SOFTKEY_MENU:
        /* 자동잠금이 아닌 경우 */
        SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            break;      //white 자동잠금 중에 접근 안됨/
        }

        if(SKY_IS_MENU_LOCK())
        {
            if(pEvent->wParam != MENUCODE_IN_CALL)
            {
                EQS_START_CHILD_EVENT(SKA_MainMenuCode, SE_APP_START, MENUCODE_IN_CALL, EQS_NA);
                break;
            }
        }

        if(!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK()))
        {
            if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
            {
                EQC_PopUpMenu((EQS_TOKEN_EX*)aHotMenuSKT, 4, 0);
            }
            else
            {
                EQC_PopUpMenu((EQS_TOKEN_EX*)aHotMenuSTI, 4, 0);
            }
        }
        break;

    case SE_KEY_RIGHT:
    case SE_SOFTKEY_PBOOK:
        /* 자동잠금> 전화부잠금 */
        if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
        {
            break;  //white 자동잠금 중에 접근 안됨/
        }

        if((!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())) && 
           (SKY_IS_PHONE_BOOK_LOCK() == FALSE))
        {
            SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
            EQS_START_CHILD(SKA_BookInCalling);
            /* 
            ** jrkwon 2001.06.28
            ** 통화 중 녹음 시 외부 LCD에 통화시간이 늦게 표시되는 문제 해결 
            */
            /* make faster for display call time */
            SKY_SetPaceMakerTimer(sFAST_PACEMAKER_SECOND, TRUE);
        }
        else if((!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())) && SKY_IS_PHONE_BOOK_LOCK())
        {
          SKY_SET_TRANSMIT_DTMF(FALSE);    /*전화부 검색 중 DTMF 발신 방지 */
          EQS_START_CHILD_EVENT(SKA_MainPbookCode, SE_APP_START, BOOKCODE_IN_CALL, EQS_NA);
        }
        break;

    case SE_IO_POPUP_CANCEL:
        SKY_SET_TRANSMIT_DTMF(TRUE);    
        break;
    
    case SE_IO_POPUP_SELECT :      
        SKY_SET_TRANSMIT_DTMF(TRUE);    /*전화부 검색 중 DTMF 발신 방지 */
        switch(pEvent->dwParam)
        {
         case BURSTMYNUMBER:
            EQS_START_CHILD(SKA_BurstDTMFWithMyNumber);
            break;

         case TOGGLEMUTE:
            EQS_START_CHILD_EVENT(SKA_ToggleMute, SE_IO_POPUP_SELECT, EQS_NA, EQS_NA);
            break;

         case CALLFORWARD:
            EQS_START_CHILD(SKA_CallForward);
            break;

         case CALLWAITING:
            if(SKY_GET_SERVICE_PROVIDER() != SP_STI)
            {
               EQS_START_CHILD(SKA_CallPending); 
            }
            else
            {                
               EQS_START_CHILD(SKA_StopCallWaiting);
            }
            break;
        }       
       break;  


    case SE_KEY_UP:
    case SE_KEY_DOWN:
         EQS_START_CHILD(SKA_AdjstConversationVol);
        break;

    case SE_KEY_PHONE_OPEN:
        if (!IsEarpieceEnable()) 
        {
           if (SKY_IS_AUTO_LOCK()) 
           {

           } 
           else 
           {

           }
        }
        break;

    case SE_KEY_PHONE_CLOSE:
        // put up lock display if data call, otherwise,
        // send CM call end command, exit call state
        //if (curr_call_is_earpiece_endable()) 
        //{
        // SKY_EndCall();
        //} 
        if(SKY_IS_EAR_JACK_OFF())
        { 
                        l_fCallEndedByUser = TRUE;
#ifdef FEATURE_SKT_AIRDATA
            if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
            {
                s_fAirDMRelease = TRUE;
                PtpLinkRelease();
                hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT), \
                                           sMSG_BOX_DISP_TIME, MBT_NONE,MBT_HELP);
                EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) \
                                        |CS_MESSAGE_BOX_IGNORE_KEYS|CS_MESSAGE_BOX_DEFAULT);
                break;
            }
#endif
#ifdef FEATURE_GVM
          if(g_fGVMPaused == TRUE)
          {
                SKY_SET_CALL_ENDED_BY_PHONECLOSE_INGVM(TRUE);  
          }
                      
#endif //FEATURE_GVM  
#ifdef FEATURE_WAVELET
          if (SKY_IS_FROM_WAVELET())
          {
                SKY_SET_CALL_ENDED_BY_PHONECLOSE_INWAVELET(TRUE);  
          }
#endif //FEATURE_WAVELET		  
#ifdef FEATURE_SKVM
#ifndef WIN32
        if (GET_SKVM_STATE == XVMSTATE_FROZEN)
        {
            SKY_SET_CALL_ENDED_BY_PHONECLOSE_INSKVM(TRUE);  
        }
#endif
#endif
          SKY_EndCall();
        }
        break;
   
#ifdef FEATURE_SKTT_EQUS_LOCATION
    case SE_LOCATION_TIMEOUT:
        CheckCurrentTime();
        break;    
#endif //#ifdef FEATURE_SKTT_LOCATION
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG :   // UI_CLRALL_F
        break; 

    case SE_KEY_MANNERS:       // Hold On 기능 
        if((ISVOICE(l_CallInfo.cm_info.call_type)) && 
           (l_fCallFromAlert == TRUE)              &&
           (s_fEnableHoldOn == TRUE)) 
        {
            EQS_START_CHILD(SKA_MainHoldOn);
        }
        break;      

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
        if(ISVOICE(l_CallInfo.cm_info.call_type)) 
        {
            if(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK())
            {
                EQS_START_CHILD(SKA_MainAllCode);
            }
            else
            {
                EQS_START_CHILD_CURRENT_EVENT(SKA_FlashWithInfo);
            }
        }
        break;

    case SE_KEY_SEND :
        if(ISVOICE(l_CallInfo.cm_info.call_type))
        {
                BYTE abTmpBuf[10];
                abTmpBuf[0] = '\0';
                ui_send_flash(abTmpBuf, 0);
        }
        break;

    case SE_SND_DTMF :
        if (ISVOICE(l_CallInfo.cm_info.call_type) ) 
        {
            sInitBurstString( GET_NUMEDIT_BUFFER, GET_NUMEDIT_POS, GET_NUMEDIT_SECRET);          
            if (sAnyBurstLeft()) 
            {
                sInProgExit();

                if (sBurstAndAdvancePause() == 0) 
                {
                    // no digit to burst, *p or *pp*
                    if (sAnyBurstLeft()) 
                    {
                        EQS_START_SIBLING(sSKA_Paused,FALSE);
                    } 
                }  
                else 
                {
                    EQS_START_SIBLING(sSKA_MultiWait,TRUE);
                }
            } 
        }
        break;

    case SE_IO_TO_IDLE :
    case SE_KEY_EAR_LONG_CLICK:
    case SE_KEY_END :
#ifdef FEATURE_UI_DORMANT
        if(sIsDormantEnable())
        {
            SetUIPktRelease();
            SKY_EndCall();
            EndCall();
            break;
        }
#endif //#ifdef FEATURE_UI_DORMANT
#ifdef FEATURE_SKT_AIRDATA
        if((s_fAirDMRelease)
#ifndef WIN32
            && dsi_srvc_mode == DS_AUTODETECT_SRVC_MODE
#endif
           )
        {
            SKY_SET_AIRDATAMANAGER_ORG(FALSE);
            SKY_SET_AIRDATAMANAGER_RCV(FALSE);
            s_fAirDMRelease = FALSE;
            sInProgExit();
            sCallEndSetup(); 
            break;
        }


        if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
        {
            s_fAirDMRelease = TRUE;
            PtpLinkRelease();

            hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT), \
            sMSG_BOX_DISP_TIME, MBT_NONE,MBT_HELP);
            EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) \
                                    |CS_MESSAGE_BOX_IGNORE_KEYS|CS_MESSAGE_BOX_DEFAULT);
            break;
        }
#endif
        {
            l_fCallEndedByUser = TRUE;
            SKY_EndCall();    // it is defined in uixcm.c
        }

        break;

#ifdef FEATURE_UI_DORMANT
     case SE_DATA_DORM_F:    // Call만 종료함(connection은 유지)
        if(sIsDormantEnable())
        {
            SKY_SET_DORMANT(TRUE);
            SKY_SetThenCompareInUseState(SKY_GetInUseState());
            SKY_EndCall();
            EndCall();
        }
        break;
#endif

    case SE_SYS_TO_IDLE ://UI_TOIDLE_F:
      break;

    case SE_CALL_INCOMING ://UI_CALL_INCOMING_E:
      // not supposed to be here, CM will end the call first
      ASSERT(FALSE);
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

            nLength = ((((GET_EVENT_PARAM()->call_event.call_info).alpha.len)
                        > MAX_DIAL_DIGIT_LENGTH) ? MAX_DIAL_DIGIT_LENGTH
                        : ((GET_EVENT_PARAM()->call_event.call_info).alpha.len));
            STRNCPY(abTemp, 
                    (GET_EVENT_PARAM()->call_event.call_info).alpha.buf, nLength);
            STRCPY(abPhoneNumber, abTemp + 12);
            sInProgExit();
            SetupCLICDNIP((CHAR *) abPhoneNumber, nLength, 
                          GET_EVENT_PARAM()->call_event.call_info.num.pi, pEvent->EventID);
            EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
        }
        break;

    case SE_CALL_CALLER_ID ://UI_CALL_CALLER_ID_E:
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY ://UI_CALL_EXT_DISPLAY_E:
#endif 
        /* call waiting, Save Incoming Phone Number in Call List */
        sInProgExit();
        if(pEvent->EventID != SE_CALL_EXT_DISPLAY)
        {
            SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);
        }

        EQS_StartTimer(EQS_TIMER_2, 200, FALSE);
        break;

    case SE_CALL_ORIGINATED ://UI_CALL_ORIGINATED_E:
        //white 2001.06.26
        if(SKY_GET_IP_AIRDATAMANAGER())
            SKY_AcceptKey(TRUE); 
    case SE_CALL_CONNECT :   //UI_CALL_CONNECT_E:
        ASSERT(FALSE);
        break;

    case SE_NDSS_RECONNECT:
        if(sIsDataCall())
        {
				UpdateInProgDisplayInDataService();
        }
        break;

    case SE_CALL_COMMAND_ERROR ://UI_CALL_CMD_ERR_E:
        break;

    case SE_CALL_ENDED ://UI_CALL_ENDED_E:
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* SE_SOCK_RELEASED 나오지 않는 현상 방지                            */      
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
#ifdef FEATURE_UI_DORMANT
        if(sIsInDormantEnable())   // Dormant일때는 무시
            break;
#endif
#ifdef FEATURE_SKT_AIRDATA
        if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
        {
            if(s_fAirDMRelease)
            {
                SKY_SET_AIRDATAMANAGER_ORG(FALSE);
                SKY_SET_AIRDATAMANAGER_RCV(FALSE);
                s_fAirDMRelease = FALSE;
            }
            else
            {
                s_fAirDMRelease = TRUE;
                PtpLinkRelease();
                hControl = EQC_MessageBox((BYTE *)SKY_GET_TOKEN(TKN_EMAIL_RECV_QUIT), \
                sMSG_BOX_DISP_TIME, MBT_NONE,MBT_HELP);
                EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) \
                                        |CS_MESSAGE_BOX_IGNORE_KEYS|CS_MESSAGE_BOX_DEFAULT);
            }
        }
#endif
        l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;    
        EndCall();
        /* 상대방이 끊었을 경우에는 통화 종료음을 내 준다 */
        /* Voice Call인 경우만                            */
        if((l_fCallEndedByUser == FALSE) && ISVOICE(l_CallInfo.cm_info.call_type))
        {
            l_fCallEndedByUser = FALSE;
            SKY_PlaySoundEx( UI_CALL_EAR_SND, SND_BUSY_ALERT, 2 , 3);
        }
        else
        {
            l_fCallEndedByUser = FALSE;
        }
      break;

#ifdef FEATURE_UI_DORMANT
    case SE_DATA_IDLE_F :   // 실제 Data call을 종료함
        SKY_SET_DORMANT(FALSE);
        SKY_SetThenCompareInUseState(SKY_GetInUseState());
        sEndConversation();
        sInProgExit();
        // set up display etc, may go to CALL_END_DISP mode or exit call state
        // l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;      
        sCallEndSetup(); //call_end_setup();
        break;
#endif

    case SE_CALL_SO_CHANGED ://UI_CALL_SO_CHANGED_E:
        // ignored for now
        break;

    case SE_CALL_FLASHED ://UI_CALL_FLASHED_E:
        if (ISVOICE(l_CallInfo.cm_info.call_type)) 
        {
            if (GET_NUMEDIT_POS > 0) 
            {        
                //calls_list_add_conference_call();
            }
            sClearFlashBuffer();
            if (sAnyBurstLeft()) 
            {        
                sInProgExit();
                EQS_START_SIBLING(sSKA_Paused, FALSE);
            } 
            else 
            {
            // no burst, stay in IN_PROG mode
            }
        } 
        else 
        {
            // something goes wrong
            ASSERT(FALSE);
        }
        break; 


    case SE_KEY_RECORD_LONG :
        if(EQS_GetModal() == INVALID_CONTROL_HANDLE)
        {
            if(!(SKY_GET_AUTO_LOCK_MODE() || SKY_GET_SHORTLOCK()))
            {
                if(SKY_IS_RECORD_PLAY_LOCK() && (s_fEnableCallRecording == TRUE))
                {
                    EQS_START_CHILD_EVENT(SKA_MainRecCode, SE_APP_START, RECCODE_IN_CALL, EQS_NA);
                }
                else if(s_fEnableCallRecording == TRUE)
                {
                    EQS_START_CHILD_EVENT(SKA_MainCallRecord, SE_KEY_RECORD_LONG, EQS_NA, EQS_NA);
                }
            }
        }
        break;


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
LOCAL void sSaveCurrentCallasMissedCall(void)
{
    CNI_T* pCallerID;

    pCallerID = SKY_GetCallerID();

    if(pCallerID->fSecret == FALSE)  /* public group */
    {
        if(ISVOICE(l_CallInfo.cm_info.call_type))
        {
            sSaveCLI(CT_MISSED_CALL);
        }
    }
    ClearCLI();
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
LOCAL void SKY_API sDisplayNameInPhoneBook(DISPLAY_NAME_E DispName)
{
    BYTE     pszTmp[40];

    if(DispName == DN_CALLER_ID)
    {
        CNI_T*   pCallerID;
        BYTE     abName[MAX_SUB_CHAR_NUM+1];

        pCallerID = SKY_GetCallerID();

        sprintf((CHAR*)pszTmp, "%s%s", SF_PHONEBOOK, pCallerID->aNameInPhoneBook);
        EQC_SetStaticText(EQS_GetNthIOC(2), pszTmp);
                
#ifdef FEATURE_SKY_SUBLCD            
        STRCPY(abName, (BYTE*)pCallerID->aNameInPhoneBook);
        MakeShortString(abName, SUB_CHAR_NUM);
        SKY_SubLCDTextOutStr(LN_UPPER, abName);
#endif //FEATURE_SKY_SUBLCD
    }
    else if(DispName == DN_CURRENT_CALL)
    {
        sprintf((CHAR*)pszTmp, "%s%s", SF_PHONEBOOK, l_CallInfo.alpha.buf);
        EQC_SetStaticText(EQS_GetNthIOC(4), l_CallInfo.alpha.buf);
    }
    else if(DispName == DN_CALL_WAITING_CALL)
    {
        CNI_T*   pCallerID;
        BYTE     abName[MAX_SUB_CHAR_NUM+1];

        pCallerID = SKY_GetCallerID();

        sprintf((CHAR*)pszTmp, "%s%s", SF_PHONEBOOK, pCallerID->aNameInPhoneBook);
        EQC_SetStaticText(EQS_GetNthIOC(3), pszTmp);
                
#ifdef FEATURE_SKY_SUBLCD            
        STRCPY(abName, (BYTE*)pCallerID->aNameInPhoneBook);
        MakeShortString(abName, SUB_CHAR_NUM);
        SKY_SubLCDTextOutStr(LN_UPPER, abName);
#endif //FEATURE_SKY_SUBLCD
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
LOCAL void SKY_API sDisplayNameInCNAP(DISPLAY_NAME_E DispName)
{
   BYTE     pszTmp[40];

   if(DispName == DN_CALLER_ID)
   {
        CNI_T*   pCallerID;
        BYTE     abName[MAX_SUB_CHAR_NUM+1];

        pCallerID = SKY_GetCallerID();

        sprintf((CHAR*)pszTmp, "%s%s", SF_CNAP, pCallerID->aName);
        EQC_SetStaticText(EQS_GetNthIOC(2), pszTmp);

#ifdef FEATURE_SKY_SUBLCD            
        STRCPY(abName, (BYTE*)pCallerID->aName);
        MakeShortString(abName, SUB_CHAR_NUM);
        SKY_SubLCDTextOutStr(LN_UPPER, abName);
#endif //FEATURE_SKY_SUBLCD
   }
   else if(DispName == DN_CURRENT_CALL)
   {
        sprintf((CHAR*)pszTmp, "%s%s", SF_CNAP, l_CallInfo.alpha_cnap.buf);
        EQC_SetStaticText(EQS_GetNthIOC(2), pszTmp);
   }
   else if(DispName == DN_CALL_WAITING_CALL)
   {
        CNI_T*   pCallerID;
        BYTE     abName[MAX_SUB_CHAR_NUM+1];

        pCallerID = SKY_GetCallerID();

        sprintf((CHAR*)pszTmp, "%s%s", SF_CNAP, pCallerID->aName);
        EQC_SetStaticText(EQS_GetNthIOC(3), pszTmp);

#ifdef FEATURE_SKY_SUBLCD            
        STRCPY(abName, (BYTE*)pCallerID->aName);
        MakeShortString(abName, SUB_CHAR_NUM);
        SKY_SubLCDTextOutStr(LN_UPPER, abName);
#endif //FEATURE_SKY_SUBLCD
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
LOCAL void SKY_API sCreateNameAndNoPhoneNumber(CALL_DISPLAY_E DISPLAY_MODE, 
                                               CALL_NAME_E NAME_MODE, 
                                               BYTE pszNameBuf[])
{
    BYTE     pszTextBuf[MAX_DIAL_DIGIT_LENGTH+1];
    HCONTROL hControl;

    if(NAME_MODE == CN_CNAP_NAME)
    {
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_CNAP, pszNameBuf);
    }
    else if(NAME_MODE == CN_PHONEBOOK_NAME)
    {
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_PHONEBOOK, pszNameBuf); 
    }

    if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
    {   
        hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS,
                                  sALERT_INFO_FIFTH_YPOS,
                                  TKN_NULL);
        EQC_SetStaticText(hControl, pszTextBuf);


        hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS,
                                  sALERT_INFO_FOURTH_YPOS,
                                  TKN_NULL);
        //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//        sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                   SF_SEND_NUM3_3);
        sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
        EQC_SetStaticText(hControl, pszTextBuf);
    }
    else if(DISPLAY_MODE == CD_INPROG)
    {   
        hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS_CML,
                                  sALERT_INFO_FIFTH_YPOS_CML,
                                  TKN_NULL);
        EQC_SetStaticText(hControl, pszTextBuf);

        hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS,
                                  sALERT_INFO_FOURTH_YPOS,
                                  TKN_NULL);
        //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//        sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//        SF_SEND_NUM3_3);
        sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
        EQC_SetStaticText(hControl, pszTextBuf);
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
LOCAL void SKY_API sCreateNameAnd1PhoneNumber(CALL_DISPLAY_E DISPLAY_MODE,
                                CALL_NAME_E    NAME_MODE,   
                                BYTE           pszNameBuf[],
                                CALL_NUMBER_E NUMBER_MODE, 
                                BYTE pszNumberBuf[])
{
    HCONTROL hControl;
    BYTE     pszTextBuf[MAX_DIAL_DIGIT_LENGTH+3];
    BYTE     pszPhoneNumberBuf[MAX_DIAL_DIGIT_LENGTH+3];

    if(NAME_MODE != CN_NO_NAME)
    {
        if(NAME_MODE == CN_CNAP_NAME)
        {
            sprintf((CHAR*)pszTextBuf, "%s%s", SF_CNAP, pszNameBuf);
        }
        else if(NAME_MODE == CN_PHONEBOOK_NAME)
        {
            sprintf((CHAR*)pszTextBuf, "%s%s", SF_PHONEBOOK, pszNameBuf); 
        }

        if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
        {       
            if((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR))
            {
                /* 이름과 같이 표시되는 번호가 CNIP일 경우 */
                hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS,
                                          sALERT_INFO_FOURTH_YPOS,
                                          TKN_NULL);
                EQC_SetStaticText(hControl, pszTextBuf);

                hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS,
                                          sALERT_INFO_THIRD_YPOS,
                                          TKN_NULL);
                //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//                sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
                sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
                EQC_SetStaticText(hControl, pszTextBuf);
            }
            else if(NUMBER_MODE == CN_CDNI)
            {
                /* 이름과 같이 표시되는 번호가 CDNIP일 경우 */
                hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS,
                                          sALERT_INFO_THIRD_YPOS,
                                          TKN_NULL);
                EQC_SetStaticText(hControl, pszTextBuf);

                hControl = EQC_StaticText(sALERT_INFO_SECOND_XPOS,
                                          sALERT_INFO_SECOND_YPOS,
                                          TKN_NULL);
                //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//                sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
                sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
                EQC_SetStaticText(hControl, pszTextBuf);
            }
        }
        else if(DISPLAY_MODE == CD_INPROG)
        {
            if((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR))
            {                                                       
                /* 이름과 같이 표시되는 번호가 CNIP일 경우 */
                hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS_CML,
                                          sALERT_INFO_FOURTH_YPOS_CML,
                                          TKN_NULL);
                EQC_SetStaticText(hControl, pszTextBuf);

                hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS_CML,
                                          sALERT_INFO_THIRD_YPOS_CML,
                                          TKN_NULL);
                //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//                sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
                sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
                EQC_SetStaticText(hControl, pszTextBuf);

            }
            else if(NUMBER_MODE == CN_CDNI)
            {
                /* 이름과 같이 표시되는 번호가 CDNIP일 경우 */
                hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS_CML,
                                          sALERT_INFO_THIRD_YPOS_CML,
                                          TKN_NULL);
                EQC_SetStaticText(hControl, pszTextBuf);

                hControl = EQC_StaticText(sALERT_INFO_SECOND_XPOS_CML,
                                          sALERT_INFO_SECOND_YPOS_CML,
                                          TKN_NULL);
                //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//                sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
                sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
                EQC_SetStaticText(hControl, pszTextBuf);
            }
        }        
    }

    if(NUMBER_MODE == CN_CNIP)
    {
        MakeHypenedPhoneNumber(pszNumberBuf, pszPhoneNumberBuf);
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszPhoneNumberBuf);
    }
    else if(NUMBER_MODE == CN_CDNI)
    {
        MakeHypenedPhoneNumber(pszNumberBuf, pszPhoneNumberBuf);

        sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszPhoneNumberBuf);
    }
    else if(NUMBER_MODE == CN_CNIR)
    {
        //STRCPY(pszTextBuf, pszNumberBuf);
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszNumberBuf);
    }
    

    if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
    {
        if(((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR)) && (NAME_MODE == CN_NO_NAME))
        {
            /* CNIP만 있고 이름 정보가 없어서 수신된 번호 위에 "발신자"라는 */
            /* 정보를 special font를 이용하여 보여주어야 할 경우            */                       
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS,
                                      sALERT_INFO_FIFTH_YPOS,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* 번호위에 보여 줄 "발신자"라는 표시 */
            hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS,
                                      sALERT_INFO_FOURTH_YPOS,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                 SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else if(((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR)) && (NAME_MODE != CN_NO_NAME))
        {
            /* CNIP와 이름 정보가 동시에 존재하는 경우 */
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS,
                                      sALERT_INFO_FIFTH_YPOS,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else if(NUMBER_MODE == CN_CDNI)
        {
            /* CDNIP 정보가 있어서 단말기에 "착신번호"라는 정보를 special  */
            /* font를 이용해서 보여주어야 할 경우                          */
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS,
                                      sALERT_INFO_FIFTH_YPOS,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* 번호위에 보여 줄 "착신번호"라는 표시 */
            hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS,
                                      sALERT_INFO_FOURTH_YPOS,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_RECEIPT_NUM1_3, SF_RECEIPT_NUM2_3,
//                                                 SF_RECEIPT_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLED_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }
    }
    else if(DISPLAY_MODE == CD_INPROG)
    {
        if(((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR)) && (NAME_MODE == CN_NO_NAME))
        {
            /* CNIP만 있고 이름 정보가 없어서 수신된 번호 위에 "발신자"라는 */
            /* 정보를 special font를 이용하여 보여주어야 할 경우            */                       
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS_CML,
                                      sALERT_INFO_FIFTH_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* 번호위에 보여 줄 "발신자"라는 표시 */
            hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS_CML,
                                      sALERT_INFO_FOURTH_YPOS_CML,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                 SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else if(((NUMBER_MODE == CN_CNIP) || (NUMBER_MODE == CN_CNIR)) && (NAME_MODE != CN_NO_NAME))
        {
            /* CNIP와 이름 정보가 동시에 존재하는 경우 */
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS_CML,
                                      sALERT_INFO_FIFTH_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

        }
        else if(NUMBER_MODE == CN_CDNI)
        {
            /* CDNIP 정보가 있어서 단말기에 "착신번호"라는 정보를 special  */
            /* font를 이용해서 보여주어야 할 경우                          */
            hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS_CML,
                                      sALERT_INFO_FIFTH_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* 번호위에 보여 줄 "착신번호"라는 표시 */
            hControl = EQC_StaticText(sALERT_INFO_FOURTH_XPOS_CML,
                                      sALERT_INFO_FOURTH_YPOS_CML,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_RECEIPT_NUM1_3, SF_RECEIPT_NUM2_3,
//                                     SF_RECEIPT_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLED_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
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
LOCAL void SKY_API sCreateNameAnd2PhoneNumber(CALL_DISPLAY_E  DISPLAY_MODE,
                                CALL_NAME_E     NAME_MODE,  
                                BYTE            pszNameBuf[],
                                CALL_NUMBER_E   NUMBER_MODE,
                                BYTE            pszCNIPNumberBuf[],  
                                BYTE            pszCDNIPNumberBuf[])
{
    BYTE        pszTextBuf[MAX_DIAL_DIGIT_LENGTH+1];
    BYTE        pszPhoneNumberBuf[MAX_DIAL_DIGIT_LENGTH+1];
    HCONTROL    hControl;

    
    if(NAME_MODE != CN_NO_NAME)
    {
        /* CNAP 또는 전화부를 검색한 이름 */
        if(NAME_MODE == CN_CNAP_NAME)
        {
            sprintf((CHAR*)pszTextBuf, "%s%s", SF_CNAP, pszNameBuf);
        }
        else if(NAME_MODE == CN_PHONEBOOK_NAME)
        {
            sprintf((CHAR*)pszTextBuf, "%s%s", SF_PHONEBOOK, pszNameBuf); 
        }

        if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
        {
            hControl = EQC_StaticText(sALERT_INFO_SECOND_XPOS,
                                      sALERT_INFO_SECOND_YPOS,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* "발신자"를 표시할 special font */
            hControl = EQC_StaticText(sALERT_INFO_FIRST_XPOS,
                                      sALERT_INFO_FIRST_YPOS,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else if(DISPLAY_MODE == CD_INPROG)
        {
            hControl = EQC_StaticText(sALERT_INFO_SECOND_XPOS_CML,
                                      sALERT_INFO_SECOND_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            /* "발신자"를 표시할 special font */
            hControl = EQC_StaticText(sALERT_INFO_FIRST_XPOS_CML,
                                      sALERT_INFO_FIRST_YPOS_CML,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }        
    }


    /* CNIP 또는 CNIR */
    if(NUMBER_MODE != CN_CNIR)
    {
        MakeHypenedPhoneNumber(pszCNIPNumberBuf, pszPhoneNumberBuf);
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszPhoneNumberBuf);        
    }
    else
    {
        //STRCPY(pszTextBuf, pszCNIPNumberBuf);
        sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszCNIPNumberBuf);
    }

    if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
    {
        if(NAME_MODE != CN_NO_NAME)
        {
            hControl = EQC_StaticText(  sALERT_INFO_THIRD_XPOS,
                                        sALERT_INFO_THIRD_YPOS,
                                        TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else
        {                       
            hControl = EQC_StaticText(  sALERT_INFO_THIRD_XPOS,
                                        sALERT_INFO_THIRD_YPOS,
                                        TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);


            /* 이름정보가 없기 때문에 "발신자" special font를 CDNIP로 날아온 */
            /* 번호 위에 위치 시킨다.                                        */                     
            hControl = EQC_StaticText(  sALERT_INFO_SECOND_XPOS,
                                        sALERT_INFO_SECOND_YPOS,
                                        TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                 SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }
    }
    else if(DISPLAY_MODE == CD_INPROG)
    {
        if(NAME_MODE != CN_NO_NAME)
        {
            hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS_CML,
                                      sALERT_INFO_THIRD_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);
        }
        else
        {                       
            hControl = EQC_StaticText(sALERT_INFO_THIRD_XPOS_CML,
                                      sALERT_INFO_THIRD_YPOS_CML,
                                      TKN_NULL);
            EQC_SetStaticText(hControl, pszTextBuf);

            
            /* 이름정보가 없기 때문에 "발신자" special font를 CDNIP로 날아온 */
            /* 번호 위에 위치 시킨다.                                        */                     
            hControl = EQC_StaticText(sALERT_INFO_SECOND_XPOS_CML,
                                      sALERT_INFO_SECOND_YPOS_CML,
                                      TKN_NULL);
            //EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
//            sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_SEND_NUM1_3, SF_SEND_NUM2_3,
//                                                     SF_SEND_NUM3_3);
            sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLING_NUMBER));
            EQC_SetStaticText(hControl, pszTextBuf);
        }       
    }    

    /* 투넘버서비스 */
    MakeHypenedPhoneNumber(pszCDNIPNumberBuf, pszPhoneNumberBuf);
    sprintf((CHAR*)pszTextBuf, "%s%s", SF_TELEPHONE, pszPhoneNumberBuf);
    //STRCPY(pszTextBuf, pszPhoneNumberBuf);

    if((DISPLAY_MODE == CD_ALERT) || (DISPLAY_MODE == CD_CALL_WAITING_CALL))
    {
        hControl = EQC_StaticText(  sALERT_INFO_FIFTH_XPOS,
                                    sALERT_INFO_FIFTH_YPOS,
                                    TKN_NULL);
        EQC_SetStaticText(hControl, pszTextBuf);

        /* 번호위에 보여 줄 "착신번호"라는 표시 */
        hControl = EQC_StaticText(  sALERT_INFO_FOURTH_XPOS,
                                    sALERT_INFO_FOURTH_YPOS,
                                    TKN_NULL);
//        EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
        //sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_RECEIPT_NUM1_3, SF_RECEIPT_NUM2_3,
        //                                     SF_RECEIPT_NUM3_3);
        sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLED_NUMBER));
        EQC_SetStaticText(hControl, pszTextBuf);
    }
    else if(DISPLAY_MODE == CD_INPROG)
    {
        hControl = EQC_StaticText(sALERT_INFO_FIFTH_XPOS_CML,
                                  sALERT_INFO_FIFTH_YPOS_CML,
                                  TKN_NULL);            
        EQC_SetStaticText(hControl, pszTextBuf);

        /* 번호위에 보여 줄 "착신번호"라는 표시 */
        hControl = EQC_StaticText(  sALERT_INFO_FOURTH_XPOS_CML,
                                    sALERT_INFO_FOURTH_YPOS_CML,
                                    TKN_NULL);
//        EQC_SET_STYLE(hControl, (EQC_GET_STYLE(hControl) | CS_STATIC_TEXT_BORDER));
        //sprintf((CHAR*)pszTextBuf, "%s%s%s", SF_RECEIPT_NUM1_3, SF_RECEIPT_NUM2_3,
        //                                     SF_RECEIPT_NUM3_3);
        sprintf((CHAR*)pszTextBuf, "%s", SKY_GET_TOKEN(TKN_CALL_CALLED_NUMBER));
        EQC_SetStaticText(hControl, pszTextBuf);
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

LOCAL BITMAP l_BmpBuf = {
    { EQUS_IMAGE_ID, { SKY_SCREEN_BUFFER_WIDTH, SKY_SCREEN_BUFFER_HEIGHT}, 
        SKY_SCREEN_WIDTH, BIT_PER_PIXEL_256C, FALSE, MAX_COLOR, 0 },
    g_abBmpBuf
};



LOCAL BOOL SKY_API sUpdateAlertedDisplayandBlink(void)
{
    HCONTROL  hControl;
    CNI_T*    pCallerID;
    BYTE      s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 1]; 
    HFILE     hFile;   
        DWORD     dwTimeInfo = 0;
    BOOL      fBlinkName, fCNIP, fCDNIP, fCNIR, fCNAP, fPHONEBOOK;
    UINT8     nStaticText4Name;
    UINT8     nNumberEditor4PhoneNumber;
    
#ifdef FEATURE_CHARACTER_BELL
    char        sPhoneNumber[MAX_TITLE_LENGTH+1], phone[MAX_PHONENUMBER_LENGTH];
    char        *loc;
    UINT8       index;
    BOOL        fCharacbellUsed = FALSE;
    BOOL        fCBGroupBellUsed = FALSE;
#endif // FEATURE_CHARACTER_BELL

    fBlinkName = FALSE;
    fCNIP      = FALSE;
    fCDNIP     = FALSE;
    fCNIR      = FALSE;
    fCNAP      = FALSE;
    fPHONEBOOK = FALSE;

    pCallerID = SKY_GetCallerID();  

    nStaticText4Name = 0;       
    nNumberEditor4PhoneNumber = 0;


#ifndef SKTT_RELEASE
    /* hjcho-test --> Start */
    //sSetTestData();
    /* hjcho-test --> End   */
#endif //SKTT_RELEASE
    
#ifdef FEATURE_CHARACTER_BELL
    memset(phone, 0x00, MAX_PHONENUMBER_LENGTH);
    memset(sPhoneNumber, 0x00, MAX_TITLE_LENGTH+1);
    
    strcpy(sPhoneNumber, (char *)pCallerID->Num.aBuf);
 
    if ((SKY_GET_CB_ADDRESS() != 0) && (strlen(sPhoneNumber) != 0)) {
        sPhoneNumber[strlen(sPhoneNumber)] = ',';
        for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
            if (SKY_GET_CB_EMPTY(index)) {
                strcpy(phone, SKY_GET_CB_PHONE(index));
                loc = strstr(phone, sPhoneNumber);
                if (loc != NULL) {
                    fCharacbellUsed = TRUE;
                    break;
                }
            }
        }
    }

    if (SKY_IS_CB_GROUPBELL(pCallerID->GroupBell.wBellID)) {
        // 착신번호로는 연동 안되었으나 그룹단위의 캐릭벨인 경우
        char temp [MAX_TITLE_LENGTH];
        STRCPY((BYTE*)temp, pCallerID->GroupBell.szFileName);
        for (index = 0; index < MAX_NUM_OF_CHARACBELL; index++) {
            if (strcmp(temp, SKY_GET_CB_TITLE(index)) == 0) {
                fCBGroupBellUsed = TRUE;
                break;
            }
        }
    }
    
    if (fCharacbellUsed) { // 캐릭벨이 설정된 경우
        hControl = EQS_GetNthIOC(0);                    
        EQS_DeleteControl(hControl);
        EQS_ClearScreen();

        sDisplayCharacbellImage(index);
    }
    else if (!fCharacbellUsed && fCBGroupBellUsed) {
        hControl = EQS_GetNthIOC(0);                    
        EQS_DeleteControl(hControl);
        EQS_ClearScreen();
        sDisplayCharacbellImage(index);
    }
    else if (pCallerID->dwPicture) 
#else
    if(  pCallerID->dwPicture )
#endif // FEATURE_CHARACTER_BELL
        {   
            ConvertCameraImageFileName(s_psFileName, pCallerID->dwPicture); 
            if(SKY_OpenDataFile(SDF_PHOTO, s_psFileName, &hFile, SFT_READ) == SDR_OK)
            {
                hControl = EQS_GetNthIOC(0);                    
                EQS_DeleteControl(hControl);
                EQS_ClearScreen();

                SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
                SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
                SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
                SKY_CloseDataFile(hFile); 
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
        }

        /* Two Number Service가 설정되어 있고  Called Party에  */
        /* 표시할 정보가 있는 경우                             */
        if(SKY_IS_NUMBER_PLUS() && (pCallerID->CalledParty.aBuf[0] != '\0'))  
        {
            /* 이름정보가 있을 경우 */
            if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))
            {
                if(STRCMP(pCallerID->aName, pCallerID->aNameInPhoneBook) != 0)
                {       
                    /* CNAP로 날아온 이름과 전화부에 저장되어 있는 이름이 틀린 경우 */
                    nStaticText4Name++;
                    fCNAP      = TRUE;
                    fPHONEBOOK = TRUE;
                    fBlinkName = TRUE;
                }
                else
                {
                    /* 두 이름이 동일한 경우 */
                    fCNAP      = TRUE;
                    fPHONEBOOK = TRUE;
                    nStaticText4Name++;
                }
            }
            else if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] == '\0'))            
            {          
                /* CNAP만 날아온 경우 */
                fCNAP      = TRUE;
                nStaticText4Name++;
            }
            else if((pCallerID->aName[0] == '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))                                           
            {
                /* 전화부에 있는 이름을 검색한 경우 */
                fPHONEBOOK = TRUE;
                nStaticText4Name++;
            }

            if(pCallerID->Num.aBuf[0] != '\0')  /* 발신번호표시가능 */
            {
                if(pCallerID->bPI == CAI_ALLOW_PI)  
                {     
                    fCNIP = TRUE;
                    nNumberEditor4PhoneNumber++;
                }
                else                        /* 발신번호표시불가능 또는 금지 */
                {
                    fCNIR = TRUE;
                    nNumberEditor4PhoneNumber++;
                }
                /* 투넘버를 표시하는 부분 */
                fCDNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
            else
            {
                /* 투넘버를 표시하는 부분 */
                fCDNIP = TRUE;
                nNumberEditor4PhoneNumber++;
            }
        }
        else /* Called Party에 표시할 정보가 없을 경우에는 CNAP와 CNIP만 보여준다. */
        {
          if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))       
          {
            /* CNAP 정보와 전화부에 있는 이름이 틀릴 경우 */
            if(STRCMP(pCallerID->aName, pCallerID->aNameInPhoneBook) != 0)
            {
                nStaticText4Name++;
                fCNAP      = TRUE;
                fPHONEBOOK = TRUE;
                fBlinkName = TRUE;
            }
            else  /* CNAP로 날아온 이름과 전화부에 저장되어 있는 이름이 동일한 경우 */
            {
                nStaticText4Name++;
                fCNAP      = TRUE;
                fPHONEBOOK = TRUE;
            }
          }
          else if((pCallerID->aName[0] != '\0') && (pCallerID->aNameInPhoneBook[0] == '\0'))            
          {          
            nStaticText4Name++;
            fCNAP      = TRUE;
          }
          else if((pCallerID->aName[0] == '\0') && (pCallerID->aNameInPhoneBook[0] != '\0'))                                           
          {
            nStaticText4Name++;
            fPHONEBOOK  = TRUE;
          }

          if(pCallerID->Num.aBuf[0] != '\0')    /* CNIP 정보가 버퍼에 들어있는 경우 */
          {
            if(pCallerID->bPI == CAI_ALLOW_PI)  /* 발신번호표시가능 */
            {          
                fCNIP = TRUE;
                nNumberEditor4PhoneNumber++;            
            }
            else                                     /* 발신번호표시불가능 또는 금지 */
            {
                fCNIR = TRUE;
                nNumberEditor4PhoneNumber++;
            }
          }
        }


    if(nNumberEditor4PhoneNumber == 0)         /* Number Editor가 없는 경우 */
    {
        if(nStaticText4Name == 1)              /* CNAP만 있는 경우 */
        {
            if(fCNAP)
            {
              sCreateNameAndNoPhoneNumber(CD_ALERT, 
                                          CN_CNAP_NAME, 
                                          pCallerID->aName);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_ALERT, 
                                          CN_PHONEBOOK_NAME, 
                                          pCallerID->aNameInPhoneBook);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
              sCreateNameAndNoPhoneNumber(CD_ALERT, 
                                          CN_NO_NAME, 
                                          pCallerID->aNameInPhoneBook);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 1)    /* Number Editor가 하나인 경우 */
    {        
        if(fCNIP)
        {
            if(fCNAP)
            {            
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CNIP, 
                                            pCallerID->Num.aBuf);

            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR, 
                                            pCallerID->Num.aBuf);
            }
        }
        else if(fCDNIP)
        {
            if(fCNAP)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_CNAP_NAME,   
                                            pCallerID->aName,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_PHONEBOOK_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd1PhoneNumber( CD_ALERT,
                                            CN_NO_NAME,   
                                            pCallerID->aNameInPhoneBook,
                                            CN_CDNI, 
                                            pCallerID->CalledParty.aBuf);
            }
        }
    }
    else if(nNumberEditor4PhoneNumber == 2)    /* Number Editor가 두개인 경우 */
    {
        if(fCNIP)
        {
            if(fCNAP)                         
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_CNAP_NAME,  
                                            pCallerID->aName,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_PHONEBOOK_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_NO_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIP,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
        }
        else if(fCNIR)
        {
            if(fCNAP)
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_CNAP_NAME,  
                                            pCallerID->aName,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_PHONEBOOK_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
            else if(!fCNAP && !fPHONEBOOK)
            {
                sCreateNameAnd2PhoneNumber( CD_ALERT,
                                            CN_NO_NAME,  
                                            pCallerID->aNameInPhoneBook,
                                            CN_CNIR,
                                            pCallerID->Num.aBuf,  
                                            pCallerID->CalledParty.aBuf);
            }
        }

    }

    return fBlinkName;
}

#ifdef FEATURE_CHARACTER_BELL
LOCAL void sDisplayCharacbellImage(int index) {
    HFILE       hFile;
    HCONTROL    hAnim;
    DWORD       dwTimeInfo = 0;
    WORD        wPictureID; 
    char        szFileName[MAX_TITLE_LENGTH];

    memset(szFileName, 0x00, MAX_TITLE_LENGTH);
    wPictureID = SKY_GET_CB_ITYPE(index);
    strcpy(szFileName, SKY_GET_CB_ITITLE(index));

    if(IS_MY_PICTURE(wPictureID)) {    // 나만의 그림
        if (SKY_OpenDataFile(SDF_IMAGE, (BYTE *)szFileName, &hFile, SFT_READ) == SDR_OK) {
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

            if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
            {
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
            {
                ANIMATION_HEADER_T AniHeader;
                BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                SKY_CloseDataFile(hFile);
                sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, szFileName);
                hAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

                //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
                //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                EQC_StartAnimation(hAnim, TRUE);
            }
        }
    }
#ifdef FEATURE_CHARACBELL
    else if(IS_CHARAC_PICTURE(wPictureID))    // 캐릭벨 그림
    {   
        if (SKY_OpenDataFile(SDF_CHA_IMAGE, (BYTE *)szFileName, &hFile, SFT_READ) == SDR_OK) {
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

            if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
            {
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
            {
                ANIMATION_HEADER_T AniHeader;
                BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                SKY_CloseDataFile(hFile);
                sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, szFileName);
                hAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

                //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
                //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                EQC_StartAnimation(hAnim, TRUE);
            }
        }
    }
#endif // FEATURE_CHARACEBLL
    else if(IS_MY_PHOTO(wPictureID)) // 나만의 사진
    {
        BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 
        if (SKY_OpenDataFile(SDF_PHOTO, (BYTE *)szFileName, &hFile, SFT_READ) == SDR_OK) {
            SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
            SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
            SKY_CloseDataFile(hFile); 

            //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
            EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
        }
    }
    else if(!IS_PICTURE_NOT_USED(wPictureID))
    {
        hAnim = EQC_Animation(0, 0, g_pBuiltInAnimation[wPictureID], NULL);
        //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
        //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
        EQC_StartAnimation(hAnim, TRUE);
    }
} // sDisplayCharacbellImage
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
/// 착신 애니메이션
LOCAL void SKY_API sInitAlertedStateDispInVoiceCall(void)
{
    HFILE  hFile;
    //  BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 
    HCONTROL  hAnim;
    DWORD     dwTimeInfo = 0;

    memset(&l_BmpBuf.Header, NULL, sizeof(l_BmpBuf.Header));

    if(IS_MY_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))    // 나만의 그림
    {
        if(SKY_OpenDataFile(SDF_IMAGE, SKY_GET_INCOMING_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

            if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
            {
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
            {
                ANIMATION_HEADER_T AniHeader;
                BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                SKY_CloseDataFile(hFile);
                sprintf((char *)abBuf, "%s%s%s", (char*)SKY_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_INCOMING_PICTURE().szFileName);
                hAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

                //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
                //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                EQC_StartAnimation(hAnim, TRUE);
            }
        }
    }
#ifdef FEATURE_CHARACBELL // 착신그림으로 캐릭벨의 이미지가 쓰인 경우
    else if(IS_CHARAC_PICTURE(SKY_GET_INCOMING_PICTURE().wPictureID))   // 캐릭벨 그림
    {   
        if (SKY_OpenDataFile(SDF_CHA_IMAGE, SKY_GET_INCOMING_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK) {
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));

            if(IS_EQUS_IMAGE((&(l_BmpBuf.Header))) || IS_EQUS_X_IMAGE(&l_BmpBuf.Header))
            {
                SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));
                SKY_CloseDataFile(hFile);
                //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
                EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
            }
            else if(IS_EQUS_ANIMATION((&(l_BmpBuf.Header))) || IS_EQUS_X_ANIMATION(&l_BmpBuf.Header))
            {
                ANIMATION_HEADER_T AniHeader;
                BYTE abBuf[SKY_PATH_NAME_MAX_LENGTH];

                SKY_ReadDataBlock(hFile, &AniHeader, sizeof(ANIMATION_HEADER_T));
                SKY_CloseDataFile(hFile);
                sprintf((char *)abBuf, "%s%s%s", (char*)SKY_CHA_IMAGE_DIR, SKY_DIR_CHAR, (char*)SKY_GET_INCOMING_PICTURE().szFileName);
                hAnim = EQC_Animation(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), NULL, abBuf);

                //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
                //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
                EQC_StartAnimation(hAnim, TRUE);
            }
        }
    }
#endif // FEATURE_CHARACBELL
    else if(IS_MY_PHOTO(SKY_GET_INCOMING_PICTURE().wPictureID)) // 나만의 사진
    {
        BYTE s_psFileName[SKY_FILE_EXT_NAME_MAX_LENGTH + 6]; 

        if(SKY_OpenDataFile(SDF_PHOTO, SKY_GET_INCOMING_PICTURE().szFileName, &hFile, SFT_READ) == SDR_OK)
        {
            SKY_ReadDataBlock(hFile, s_psFileName, SKY_MAX_CAMERA_IMAGE_FILE_LEN);
            SKY_ReadDataBlock(hFile, &dwTimeInfo, sizeof(dwTimeInfo));
            SKY_ReadDataBlock(hFile, &l_BmpBuf.Header, sizeof(l_BmpBuf.Header));  
            SKY_ReadDataBlock(hFile, (void*)l_BmpBuf.pbData, GET_IMAGE_BYTE_SIZE(l_BmpBuf.Header));   
            SKY_CloseDataFile(hFile); 

            //l_BmpBuf.Header.yStart = SKY_ANNUNCIATORS_HEIGHT;
            EQC_Image(GET_IMG_X_IN_IDLE(l_BmpBuf), GET_IMG_Y_IN_IDLE(l_BmpBuf), &l_BmpBuf);
        }
    }
    else if(!IS_PICTURE_NOT_USED(SKY_GET_INCOMING_PICTURE().wPictureID))
    {
        hAnim = EQC_Animation(0, 0, g_pBuiltInAnimation[SKY_GET_INCOMING_PICTURE().wPictureID], NULL);

        //EQC_SetStartYAnimation(hAnim, SKY_ANNUNCIATORS_HEIGHT);
        //EQC_SetAnimationHeight(hAnim, SKY_SCREEN_HEIGHT-SKY_ANNUNCIATORS_HEIGHT);
        EQC_StartAnimation(hAnim, TRUE);
    }
    else    // 전화 왔습니다.
    {
        EQC_StaticText(0,0, TKN_CALL_ALERT);
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
LOCAL void SKY_API sInitAlertedStateDispInDataService(void)
{
    HCONTROL hControl;
    
    /* 애니메이션 */
	hControl = EQC_Animation(sDATA_SERVICE_ANI_X, sDATA_SERVICE_ANI_Y, &g_AniInDataService, NULL);
	EQC_SET_STYLE(hControl, CS_ANIMATION_REPEAT);
		
	/* 프레임 */
	EQC_Image(sDATA_SERVICE_FRAME_X, sDATA_SERVICE_FRAME_Y, (BITMAP*)&g_BmpCallPannel);


    EQC_SetStaticText(EQC_StaticText(sDATA_SERVICE_TIME_X, sDATA_SERVICE_TIME_Y, TKN_NULL),
                      (BYTE*)SKY_GET_TOKEN(TKN_CALL_DS_CONNECT));    
    EQC_StaticText(sDATA_SERVICE_TIME_NUM_X, sDATA_SERVICE_TIME_Y, TKN_NULL);         //IP_TIME   

#ifdef FEATURE_SKT_AIRDATA  
    if(SKY_GET_AIRDATAMANAGER_ORG() || SKY_GET_AIRDATAMANAGER_RCV())
    {
            EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_WAIT_AIR_DATAMGR1);
    }
    else
#endif //#ifdef FEATURE_SKT_AIRDATA
#ifdef  FEATURE_IP_AIRDATA      
    if (SKY_GET_IP_AIRDATAMANAGER())
    {
            EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_IP_DATAMGR_TXT);
    }
    else
#endif  // #ifdef       FEATURE_IP_AIRDATA      
    {
        if( DS_ISDATA_RS1(l_CallInfo.cm_info.srv_opt))
        {
            /* rate is 9.6 */
            if (l_CallInfo.cm_info.srv_opt == CAI_SO_PPP_PKT_DATA_3G)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_MDR_PKT_DATA_FRS1_RRS1)
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_64);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9MODEM);
            }
            else if (l_CallInfo.cm_info.srv_opt == CAI_SO_DATA)                  
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PIWF);
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9PACKET);
            }
            else
            {
                    EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_9DATA);
            }
        }
        else if( DS_ISDATA_RS2(l_CallInfo.cm_info.srv_opt))
        {
              /* rate is 14.4 */
            if (l_CallInfo.cm_info.srv_opt==CAI_SO_PPP_PKT_DATA_3G)
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_153);
            } 
            else if (l_CallInfo.cm_info.srv_opt==CAI_SO_MDR_PKT_DATA_FRS2_RRS2)
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_91);
            }
            else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14FAX);
            }
            else if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14MODEM);
            }
            else if (DS_ISDATA_PPP_PKT(l_CallInfo.cm_info.srv_opt))
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14PACKET);
            }
            else
            {
               EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING_14DATA);
            }
        }
        else
        {
            /* rate is unknown */
            EQC_StaticText(sDATA_SERVICE_STATE_X, sDATA_SERVICE_STATE_Y, (EQS_TOKEN)TKN_CALL_DS_INCOMING);
        }   
    }
    
    EQC_SET_STYLE(EQS_GetNthIOC(DS_STATE), CS_STATIC_TEXT_ORING);
    EQC_SET_STYLE(EQS_GetNthIOC(DS_TIME), CS_STATIC_TEXT_ORING);
    EQC_SET_STYLE(EQS_GetNthIOC(DS_TIME_TITLE), CS_STATIC_TEXT_ORING);
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     sStopMusicandMotor()
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
**     @SidEffects
**
** ---------------------------------------------------------------------------
*/
#ifdef FEATURE_CAF20_SPEC
LOCAL void sStopMusicandMotor(void)
{           
    SKY_EndAlertSignal();
}
#endif //FEATURE_CAF20_SPEC



/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_Alerted()
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
**     @SidEffects
**
** ---------------------------------------------------------------------------
*/
void SKY_API SKA_Alerted(SKY_EVENT_T *pEvent)
{
    STATIC CNI_T*           pCallerID;
    STATIC BOOL             fBlinkName;
    HCONTROL                hControl;
    STATIC UINT8            nBlink;
    STATIC SKY_EVENT_E      PreviousEvent;

    
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START : /* The very first event */
        EQS_GetDC(TRUE);
        sAlertedModeEntry();
        EQS_SetWindow(SKY_DEF_WIN_X,
            SKY_DEF_WIN_Y, 
            SKY_DEF_WIN_DX, 
            SKY_DEF_WIN_DY, 
            CL_NONE, 
            NULL, // show annunciators
            WA_SHOW_HEADER);

        PreviousEvent = SE_KEY_NONE;
        l_fCallFromAlert = TRUE;
        fBlinkName = FALSE;

        /* 착신 시에는 사용자의 조명설정에 관계 없이 */
        /* 백라이트가 항상 켜진다.                   */
        SKY_SET_NO_OFF_BACKLIGHT(TRUE);
        SKY_BacklightOn(AB_ALWAYS);

        if(sIsDataCall())// && !SKY_IS_AUTO_ANSWER())     
        {
            if (!SKY_IS_FROM_GVM())
                sInitAlertedStateDispInDataService();  
        }
        else if(ISVOICE(l_CallInfo.cm_info.call_type) || ISTEST(l_CallInfo.cm_info.call_type))
        {
            sInitAlertedStateDispInVoiceCall();
        }

#if (SKY_MODEL_ID == SKY_MODEL_B1) || (SKY_MODEL_ID == SKY_MODEL_B2)
#ifdef FEATURE_SKY_SUBLCD
        /* 전화왔습니다 */
        SKY_SubLCDTextOutStr(LN_UPPER, (BYTE*)""); /* remove sub lcd annunciators quickly */
        SKY_SubLCDTextOut(LN_LOWER, TKN_SLCD_INCOMING_CALL);
#endif //#ifdef FEATURE_SKY_SUBLCD
#endif //#if SKY_MODEL_ID == SKY_MODEL_B1

#ifdef FEATURE_SKT_EIF
        if(IsEIF_AutoReceive())
        {
            EQS_StartTimer(EQS_TIMER_3, sTHREE_SECOND, FALSE);
        }
#endif //FEATURE_SKT_EIF
        break;

    case SE_APP_END : /* The very last event */
        SKY_AcceptKey(TRUE);
        SKY_SubLCDEndText(LN_UPPER); /* annunciators 표시해도 좋음 */
        SKY_SubLCDEndText(LN_LOWER); /* 시간 표시해도 좋음 */
        EQS_ReleaseDC();
        break;

    case SE_APP_SUSPEND:
        SKY_AcceptKey(TRUE);
        break;

    case SE_APP_RESUME :
        SKY_AcceptKey(TRUE);
        if(EQS_GET_CHILD() == SKA_MainClid)
        {
            pCallerID = SKY_GetCallerID();

#ifdef FEATURE_SKT_EIF
        if(IsEIF_AutoReceive())
        {
            BYTE aCNIP[MAX_DIAL_DIGITS+1];            

            if(pCallerID->Num.nPos > 0)
            {
                    sprintf((CHAR*)aCNIP,"CNI:%s", pCallerID->Num.aBuf);
                    at_send_extended_text((CHAR*)aCNIP);
            }
        }
#endif //FEATURE_SKT_EIF
        fBlinkName = sUpdateAlertedDisplayandBlink();
        
        if(fBlinkName)
        {
           nBlink = 0;
           EQS_StartTimer(EQS_TIMER_2, 2000, TRUE);
        }
        //if(!SKY_IS_PHONE_OPEN())
        SKY_SNI(HS_SNI_CALL_ALERT, TRUE);
#ifdef FEATURE_SKY_SUBLCD
        if(pCallerID->aName[0] != '\0')   /* 이름 정보가 있다면  */
        {
            BYTE abName[MAX_SUB_CHAR_NUM+1];
            
            STRCPY(abName, (BYTE*)pCallerID->aName);
            MakeShortString(abName, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_UPPER, abName);
        }
        else if(pCallerID->aNameInPhoneBook[0] != '\0')
        {
            BYTE abName[MAX_SUB_CHAR_NUM+1];
    
            STRCPY(abName, (BYTE*)pCallerID->aNameInPhoneBook);
            MakeShortString(abName, SUB_CHAR_NUM);
            SKY_SubLCDTextOutStr(LN_UPPER, abName);
        }
        else /* 이름 정보가 없으면 번호를 표시한다 */
        {
            UINT8 nCodePos, nHypenPos;
            BYTE  abHypenedNumber[MAX_DIAL_DIGIT_LENGTH+1];

            if(STRCMP(pCallerID->Num.aBuf, SKY_GET_TOKEN(TKN_CALL_CNIR)) == 0)
            {
                SKY_SubLCDTextOutStr(LN_UPPER, SKY_GET_TOKEN(TKN_SLCD_IN_CALL_CNIR));             
            }
            else
            {
                nCodePos  = FindCodeSet(pCallerID->Num.aBuf);           
                nHypenPos = FindHypen(pCallerID->Num.aBuf, nCodePos, MAX_DIAL_DIGIT_LENGTH);
                MakeHypenedNumber(pCallerID->Num.aBuf, abHypenedNumber, MAX_DIAL_DIGIT_LENGTH,
                                  nCodePos, nHypenPos);
                SKY_SubLCDTextOutStr(LN_UPPER, abHypenedNumber);
            }
        }
#endif//#ifdef FEATURE_SKY_SUBLCD
      }
         break;

    case SE_APP_DRAW:
//        if(sIsDataCall())
//        {
//            if(!SKY_IS_FROM_GVM())
//            {
//                DrawGeneralFrame();
//            }
//        }
        break;

#ifdef FEATURE_SKT_EIF
    case SE_TM_TIMER3:
#endif //FEATURE_SKT_EIF
    case SE_KEY_CENTER:
    case SE_KEY_F1:
    case SE_KEY_F2:
    case SE_KEY_EAR_CLICK:
    case SE_KEY_EAR_LONG_CLICK:
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
    case SE_KEY_LEFT:
    case SE_KEY_RIGHT:
    case SE_SOFTKEY_LEFT :   /* PF3 : */
    case SE_SOFTKEY_RIGHT :  /* PF2 : */
    case SE_KEY_CLEAR :
    case SE_KEY_CLEAR_LONG : /* UI_CLRALL_F: */
    case SE_KEY_END :
    case SE_ANSWER  :        /* 자동착신 시 날아오는 이벤트 */
          ClearMissedCallInfo();
          l_fKeyDownInAlert = TRUE;
#if defined FEATURE_DS && !defined WIN32                   //white 2001.04.07
            // If this is a data call alert, the user can not answer it
            // from the keypad. Only a UI_ANSWER_F command should answer
            // a data call alert.
#ifdef FEATURE_DS_VOICE_AS_DATA
            // if an AMPS call come in and voice_as_data is enabled, consider
            // it as a data call even MC tells us it is voice call
            if( (sIsDataCall()/*|| ui.voice_as_data != DB_VOICE_AS_DATA_NEVER*/) &&
                 pEvent->EventID != SE_ANSWER )
#else
            if(sIsDataCall() && pEvent->EventID != SE_ANSWER )
#endif
            {
                ds_srvcmode_type incoming_srvcmode;               
                incoming_srvcmode = dsi_call_incoming();
                if (incoming_srvcmode == DS_IS99_CALL)
                {
                #ifdef  FEATURE_SKT_AIRDATA     //In case of AirData Alert, Auto Answer
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt) && !SKY_GET_AIRDATAMANAGER_RCV())
                #else   //      #ifdef  FEATURE_SKT_AIRDATA     
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
                #endif  //      #ifdef  FEATURE_SKT_AIRDATA     
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                  else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                }     
                else if ( incoming_srvcmode == DS_PIWF_CALL) 
                {
                    dsiAnswerPiwfCall();
                    break;
                }
                else
                {
                      ERR("NANA!!! Unknown DS Srvc Type.",0,0,0);
                }
              break;
            }
#endif  //ifdef FEATURE_DS 
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
#ifdef FEATURE_CAF20_SPEC
      if(pEvent->EventID != SE_KEY_END)
      {
        if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
           SKY_AnswerCall();
        }
      }
      else if(pEvent->EventID == SE_KEY_END)
      {
        if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
           sStopMusicandMotor(); 
        }
      }
#else  //FEATURE_CAF20_SPEC
        if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
           SKY_AnswerCall();
        }
#endif //FEATURE_CAF20_SPEC
#else  //FEATURE_IS95B_ALERT_ANS_BYPASS
#ifdef FEATURE_CAF20_SPEC
      if(pEvent->EventID != SE_KEY_END)
      {
        if (SKY_IS_ALERT()) 
        {
          SKY_AnswerCall();
        }
      }
      else if(pEvent->EventID == SE_KEY_END)
      {
        if (SKY_IS_ALERT()) 
        {
           sStopMusicandMotor();
        }
      }
#else //FEATURE_CAF20_SPEC
      if (SKY_IS_ALERT()) 
      {
        SKY_AnswerCall();
      }
#endif//FEATURE_CAF20_SPEC
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
      break;

  case SE_KEY_RECORD:
  case SE_KEY_PLAY:
      if(SKY_IS_PHONE_OPEN() || SKY_IS_HANDS_FREE() ||SKY_IS_EAR_JACK_ON())
      {
        ClearMissedCallInfo();
        l_fKeyDownInAlert = TRUE;
#if defined FEATURE_DS && !defined WIN32                 //white 2001.04.07
            // If this is a data call alert, the user can not answer it
            // from the keypad. Only a UI_ANSWER_F command should answer
            // a data call alert.
#ifdef FEATURE_DS_VOICE_AS_DATA
            // if an AMPS call come in and voice_as_data is enabled, consider
            // it as a data call even MC tells us it is voice call
            if( (sIsDataCall()/*|| ui.voice_as_data != DB_VOICE_AS_DATA_NEVER*/) &&
                 pEvent->EventID != SE_ANSWER )
#else
            if(sIsDataCall() && pEvent->EventID != SE_ANSWER )
#endif
            {
                ds_srvcmode_type incoming_srvcmode;               
                incoming_srvcmode = dsi_call_incoming();
                if (incoming_srvcmode == DS_IS99_CALL)
                {

#ifdef  FEATURE_SKT_AIRDATA     //In case of AirData Alert, Auto Answer
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt) && !SKY_GET_AIRDATAMANAGER_RCV())
#else   //      #ifdef  FEATURE_SKT_AIRDATA     
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
#endif  //      #ifdef  FEATURE_SKT_AIRDATA     
                  {
                    dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                  else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                  {
                    dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                }     
                else if ( incoming_srvcmode == DS_PIWF_CALL) 
                {
                    dsiAnswerPiwfCall();
                    break;
                }
                else
                {
                      ERR("NANA!!! Unknown DS Srvc Type.",0,0,0);
                }
                break;
            }
#endif  //ifdef FEATURE_DS 
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
        if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
            SKY_AnswerCall();
        }      
#else
        if (SKY_IS_ALERT()) 
        {
            SKY_AnswerCall();
        }      
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
      }
      break;

  case SE_KEY_AUTO_ANSWER_LONG:
        if(IsSpace2RecordAutoAnswer())
        {
            if(SKY_IS_AUTO_ANSWER()) /* 자동응답이 이미 설정되어 있는 상태일 경우 */
            {               
                l_fAutoAnswerAlready = TRUE;
                SKY_SET_AUTO_ANSWER(TRUE);
            }
            else                     /* 자동응답이 설정되어 있지 않은 상태에서        */
            {                        /* 자동응답키를 눌러서 자동응답으로 넘어갈 경우 */
                l_fAutoAnswerAlready = FALSE;
                SKY_SET_AUTO_ANSWER(TRUE);
            }
            l_fAutoAnswerFromAlert = TRUE;
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
            if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
            {
                SKY_AnswerCall();
            }      
#else
            if (SKY_IS_ALERT()) 
            {
                SKY_AnswerCall();
            }      
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
        }
      break;

    case SE_KEY_PHONE_CLOSE:
#ifdef FEATURE_CAF20_SPEC
        if(SKY_IS_EAR_JACK_OFF())
        {
            sStopMusicandMotor();
        }
#endif//FEATURE_CAF20_SPEC
          break;    

    case SE_KEY_PHONE_OPEN : /* EAR_UP : */
#ifdef FEATURE_PHONE_VR_HFK
#error code not present
#endif /* FEATURE_PHONE_VR_HFK */
      if(SKY_GET_HOW_TO_ANSWER() != HTA_OPEN) //|| SKY_IS_EAR_JACK_ON()))
            break;
        ClearMissedCallInfo();
#if defined FEATURE_DS && !defined WIN32                //white 2001.04.07
            // If this is a data call alert, the user can not answer it
            // from the keypad. Only a UI_ANSWER_F command should answer
            // a data call alert.
#ifdef FEATURE_DS_VOICE_AS_DATA
            // if an AMPS call come in and voice_as_data is enabled, consider
            // it as a data call even MC tells us it is voice call
            if( (sIsDataCall()/*|| ui.voice_as_data != DB_VOICE_AS_DATA_NEVER*/) &&
                 pEvent->EventID != SE_ANSWER )
#else
            if(sIsDataCall() && pEvent->EventID != SE_ANSWER )
#endif
            {
              ds_srvcmode_type incoming_srvcmode;               
              incoming_srvcmode = dsi_call_incoming();
              if (incoming_srvcmode == DS_IS99_CALL)
              {
#ifdef  FEATURE_SKT_AIRDATA     //In case of AirData Alert, Auto Answer
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt) && !SKY_GET_AIRDATAMANAGER_RCV())
#else   //      #ifdef  FEATURE_SKT_AIRDATA     
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
#endif  //      #ifdef  FEATURE_SKT_AIRDATA     
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                  else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
              }     
              else if ( incoming_srvcmode == DS_PIWF_CALL) 
              {
                  dsiAnswerPiwfCall();
                  break;
              }
              else      
              {

                      ERR("NANA!!! Unknown DS Srvc Type.",0,0,0);
              }
              break;
            }
#endif  //ifdef FEATURE_DS       
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
        if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
            l_fKeyDownInAlert = TRUE;
            SKY_AnswerCall();
        }      
#else
        if(SKY_IS_ALERT())
        {
            l_fKeyDownInAlert = TRUE;
            SKY_AnswerCall();
        }      
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
      break;

    case SE_KEY_SEND :
        ClearMissedCallInfo();
#ifdef FEATURE_PHONE_VR_HFK
#error code not present
#endif /* FEATURE_PHONE_VR_HFK */
      // answer the call if it is an incoming call
      // send flash if it is call waiting
#if defined FEATURE_DS && !defined WIN32                  //white 2001.04.07
            // If this is a data call alert, the user can not answer it
            // from the keypad. Only a UI_ANSWER_F command should answer
            // a data call alert.
#ifdef FEATURE_DS_VOICE_AS_DATA
            // if an AMPS call come in and voice_as_data is enabled, consider
            // it as a data call even MC tells us it is voice call
            if( (sIsDataCall()/*|| ui.voice_as_data != DB_VOICE_AS_DATA_NEVER*/) &&
                 pEvent->EventID != SE_ANSWER )
#else
            if(sIsDataCall() && pEvent->EventID != SE_ANSWER )
#endif
            {
              ds_srvcmode_type incoming_srvcmode;               
              incoming_srvcmode = dsi_call_incoming();
              if (incoming_srvcmode == DS_IS99_CALL)
              {
#ifdef  FEATURE_SKT_AIRDATA     //In case of AirData Alert, Auto Answer
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt) && !SKY_GET_AIRDATAMANAGER_RCV())
#else   //      #ifdef  FEATURE_SKT_AIRDATA     
                  if (DS_ISDATA_ASYNC(l_CallInfo.cm_info.srv_opt))
#endif  //      #ifdef  FEATURE_SKT_AIRDATA     
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
                  else if (DS_ISDATA_G3_FAX(l_CallInfo.cm_info.srv_opt))
                  {
                        dsi_answer_is99_call(); /*  Sends configuation  */
                  }
              }     
              else if ( incoming_srvcmode == DS_PIWF_CALL) 
              {
                  dsiAnswerPiwfCall();
              }
              else
              {
                  ERR("NANA!!! Unknown DS Srvc Type.",0,0,0);
              }
              break;
            }
#endif  //ifdef FEATURE_DS 
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
      if(SKY_IS_ALERT())
      {
        if (sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
        {
          SKY_AnswerCall();/* defined in uixcm.c */
        }
      }      
#else
      if(SKY_IS_ALERT())
      {
        SKY_AnswerCall();
      }      
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
      else 
      {
         ui_send_flash(NULL, 0);/*   defined in uixcm.c  */
      }
      l_fKeyDownInAlert = TRUE;
      break;

//    case SE_KEY_END :
//#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
//      if (SKY_IS_ALERT() && sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)))
//      {
//        SKY_AnswerCall();
//      }      
//#else
//      if(SKY_IS_ALERT())
//      {
//        SKY_AnswerCall();
//      }      
//#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
//       break;

    case SE_AUTO_ANSWER : /* UI_RING_F: */
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
        if (sIsAnswerAllowed(&(GET_EVENT_PARAM()->call_event.call_info)) &&
            IsSpace2RecordAutoAnswer())        
        {
            PreviousEvent = SE_AUTO_ANSWER;
            SKY_AnswerCall();
        }
#else
        if(IsSpace2RecordAutoAnswer())
        {
            SKY_AnswerCall();
        }
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */  
        break;

    case SE_SYS_TO_IDLE :/* UI_TOIDLE_F: */
        /* stay in this mode, a call event is coming */
        break;

    case SE_TM_TIMER1:
        EQS_START_CHILD(SKA_MainClid);
        break;

    case SE_TM_TIMER2:
        if((nBlink%2) == 0)
        {
            nBlink++;
            sDisplayNameInPhoneBook(DN_CALLER_ID);
        }
        else
        {
            nBlink++;
            sDisplayNameInCNAP(DN_CALLER_ID);
        }
        break;

    case SE_CALL_INCOMING :   /* UI_CALL_INCOMING_E: */ 
    case SE_CALL_CALLER_ID :  /* UI_CALL_CALLER_ID_E: */
    case SE_CALL_CALLED_PARTY:
#ifdef FEATURE_IS95B_EXT_DISP 
    case SE_CALL_EXT_DISPLAY :/* UI_CALL_EXT_DISPLAY_E: */
#endif 
        if((pEvent->EventID != SE_CALL_EXT_DISPLAY) && 
            (pEvent->EventID != SE_CALL_INCOMING))
        {
            SetupCLIfromCallInfo(&(GET_EVENT_PARAM()->call_event.call_info), pEvent->EventID);      
        }
        EQS_StartTimer(EQS_TIMER_1, 200, FALSE);
        break;

    case SE_CALL_ORIGINATED : /* UI_CALL_ORIGINATED_E: */
        /* not supposed to be here, origination  */
        /* has lower priority than incoming call */
        ASSERT(FALSE);
        break;

    case SE_CALL_FLASHED : /*  UI_CALL_FLASHED_E:  */
        if(ISVOICE(l_CallInfo.cm_info.call_type))
        {
            sSaveCLI(CT_CALLED_BY);
        }
        sAlertedExit();
        EQS_START_SIBLING_CURRENT_EVENT(SKA_InProg,FALSE);
        break;

    case SE_CALL_SO_CHANGED : /* UI_CALL_SO_CHANGED_E: */
        // not supposed to be here
        ASSERT(FALSE);
        break;

    case SE_CALL_COMMAND_ERROR : /* UI_CALL_CMD_ERR_E: */
        // possibly call answer failed. 
        break;

    case SE_CALL_CONNECT : /* UI_CALL_CONNECT_E: */
        if (SKY_SetThenCompareInUseState(IUS_CONNECT))
        {
            EQS_SEND_EVENT(SE_APP_DRAW, EQS_DRAW_LEVEL_SWINDOW, EQS_NA);
        }
        /* Alerted ID LCD 꺼준다. */
        SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
        //if(!SKY_IS_PHONE_OPEN())
        SKY_SNI(HS_SNI_CALL_CONNECT, TRUE);
        sInitCurrentCallOnAlert(&(GET_EVENT_PARAM()->call_event.call_info));      
        if(l_CallInfo.secret == FALSE) /* Public Group */
        {
            if(PreviousEvent == SE_AUTO_ANSWER)
            {
                if(ISVOICE(l_CallInfo.cm_info.call_type))
                {
                    sSaveCLI(CT_MISSED_CALL);
                }
            }
            else
            {      
                if(ISVOICE(l_CallInfo.cm_info.call_type))
                {
                    sSaveCLI(CT_CALLED_BY);
                }
            }
        }
        sAlertedExit();      
      
      if(IsDataCall())  // 착신
      {
          sInitConversation();  // update l_CallInfo.time

/* rosa 01-05-14 --> @why, what */
          /*if(SKY_IS_FROM_GVM()) /* DATA Call * /
          {
              /* undersea 2001-07-23 --> @why, what * /              
              EQS_START_SIBLING(SKA_RunGVM, FALSE);                            
              //EQS_START_SIBLING(SKA_TCPConnect, FALSE);        
              /* <-- * /
          }
          else*/
/* <-- */
          {
#ifdef FEATURE_UI_SEND_PIN
              EQS_START_CHILD(SKA_PinPrompt);
#else  //FEATURE_UI_SEND_PIN
              
#ifdef FEATURE_IP_AIRDATA              
              if(SKY_GET_IP_AIRDATAMANAGER())             
              {
                EQS_START_SIBLING(SKA_InProgIPAirDM, FALSE);
              }
              else
#endif //FEATURE_IP_AIRDATA
              {         
                EQS_START_SIBLING(SKA_InProg,FALSE);
              }
#endif //FEATURE_UI_SEND_PIN
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */
          }
      }
      else
      {
          sInitConversation();
#ifdef FEATURE_UI_SEND_PIN
          EQS_START_CHILD(SKA_PinPrompt);
#else
          EQS_START_SIBLING(SKA_InProg,FALSE);
#endif
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */
      }      
      break;

    case SE_CALL_ENDED : /* UI_CALL_ENDED_E: */
        /* 팝업메뉴나 메시지박스가 뜬 상태에서 상대방이 전화를 끊을 경우에 */
        /* 통화종료화면이 나오지 않는 현상 방지                            */
        if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
        {
            EQS_DeleteControl(hControl);
        }
        DataManagerEnd(pEvent);
        sSaveCurrentCallasMissedCall();
        sAlertedExit();
        l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;      
        sCallEndSetup(); //call_end_setup();
        break;

    case SE_SYS_BAD_BATT: 
        /* 착신상태에서 사용자가 받기 전에 Low Batt.으로 단말기의 전원이 */
        /* 나갔을 경우, 현재 착신된 전화를 "부재중전화"로 착신이력에 저장*/
        /* 한다.                           hjcho 01.08.02                */
        sSaveCurrentCallasMissedCall();
        EQS_PASS_CURRENT_EVENT();
        break;

#ifdef FEATURE_UI_DORMANT
    case SE_DATA_IDLE_F:
        SKY_SET_DORMANT(FALSE);
        SKY_SetThenCompareInUseState(SKY_GetInUseState());
        sAlertedExit();
        //l_CallInfo.cm_info = GET_EVENT_PARAM()->call_event.call_info;      
        sCallEndSetup(); //call_end_setup();
        break;
#endif //#ifdef FEATURE_UI_DORMANT

    default:
        CallDefaultEventProc(pEvent, ALERTED);
        break;
    }
}




/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     SKA_MainCall(SKY_EVENT_T *pEvent)
**
** Description: 
**     This applet is for Call processing.
**     ska_idle.c and some applets call this function with SE_CALL_INCOMING, 
**     SE_CALL_ORIGINATED events.
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
void SKY_API SKA_MainCall(SKY_EVENT_T *pEvent)
{
    /*
    girin. comment. 2000.12.22

    < Applet transition diagram >

    SKA_MainIdle()
        |
        {child}
        |
    sSKA_Dialing() - {sibling} - SKA_MainCall()
                                    |
                                    {child}
                                    |
                                sSKA_OrigAttempt()

    */
    switch(pEvent->EventID)
    {
    /* NC_APP */
    case SE_APP_START : /* The very first event */
        /* 
           We must clear screen and draw call state screen.
           SKA_InitAppletScreen(&CallAppletScreen);
           s_ClearCallAppletScreen();
        */
///Added by happykid in 2001.1.2
#ifdef FEATURE_SMART_BATTERY
        SmbEvent(SMB_START_TALK_S);
#endif
        break;

    case SE_APP_RESUME:
        /* rosa 01-05-30 --> @why, what */
        if(SKY_IS_FROM_GVM()) /* DATA & VOICE CALL로 인해 CALL상태로 넘어온 경우임 */
        {
            SKY_SET_FROM_GVM(FALSE);
            if (g_fGVMPaused) {
                EQS_START_SIBLING(SKA_MainGVM, FALSE);
            }
            /*if(SKY_IS_TO_WAP())
            {
                EQS_START_SIBLING(SKA_MainGVM, FALSE); /* SKA_MainWAP 넘어감 * /
            }*/
            else
            {    
                EQS_END_TO(EQS_POP_TO_ROOT); /* PtP Call수신후 종료할 때 */
            }
        }
#ifdef FEATURE_WAVELET
		else if (SKY_IS_FROM_WAVELET()) /* DATA & VOICE CALL로 인해 CALL상태로 넘어온 경우임 */
        {
            SKY_SET_FROM_WAVELET(FALSE);
            if (g_wsvcType == SVC_IDLE) {
				SKY_SET_TO_WAVELET(TRUE);
                EQS_START_SIBLING(SKA_IdleWavelet, FALSE);
            }
            else
            {    
                EQS_END_TO(EQS_POP_TO_ROOT); /* PtP Call수신후 종료할 때 */
            }

//SVC_LIVE_STREAM	
//SVC_ONDEMAND_STREAM			
//SVC_DOWNLOAD_PLAY			
//SVC_DOWNLOAD_STORE			
        }
#endif //FEATURE_WAVELET
        else
        {
            EQS_END();
        }     
        
        break;

    case SE_APP_END :   /* The very last event */ 
        /*
           We must clear call state screen 
           SKA_ShutdownAppletScreen();
        */
///Added by happykid in 2001.1.2
#ifdef FEATURE_SMART_BATTERY
                        SmbEvent(SMB_END_TALK_S);
#endif
        if(!SKY_IS_ORIGINATE()) // Incoming call
        {
            // jrkwon 2001.06.05 
            //SKY_SNI(HS_SNI_CALL_ALERT, FALSE);
            SKY_MotorStop();
            SKY_StopMusic();
        }

        l_fWhisperMode = FALSE;
        break;
        
    // girin. 2000.12.22
    // Using CM (Call Manager), on origination call, the events related
    // to origination call are SE_PH_INUSE_STATE , SE_SYS_TO_IDLE and
    // SE_SYS_CALL_ORIGINATED in this order.
    // SE_PH_INUSE_STATE is handled by default event proc. to display
    // in-use annunciator. 
    // SE_SYS_TO_IDLE should be caught here and thrown away not to be
    // handled by default event proc. which get the phone go to SKA_MainIdle().
    case SE_SYS_TO_IDLE:
    case SE_KEY_CLEAR:        
        // Throw it away !!!
        break;
    // girin. end.

    case SE_DATA_CALL:
    case SE_CALL_INCOMING :
    case SE_CALL_ORIGINATED : 
         /* EnterCallState decide a calling mode  */
         /* in alerted mode or orig_attempt mode    */
        sEnterCallState(pEvent->EventID); 
        break;

    default :
        CallDefaultEventProc(pEvent, CALL_STATE_NONE);
//      EQS_DEF_EVENT_PROC(pEvent);
        break;
    }
}



/*****************************************************************************/
/** 통화기록 관련 함수들 및 애플릿                                           */
/*****************************************************************************/

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
#define CALL_INFO_SIZE   20
#define INIT_CUMMULATIVE_INFO   0

enum tagCALLINFO_E
{
  CI_ORIGINATION_VOICECALL_INFO_TITLE = 0,
  CI_ORIGINATION_VOICECALL_INFO,
  CI_ORIGINATION_VOICECALL_INFO_LINE,
  CI_RECEIVED_VOICECALL_INFO_TITLE,
  CI_RECEIVED_VOICECALL_INFO,
  CI_RECEIVED_VOICECALL_INFO_LINE,
  CI_NTOP_INFO_TITLE,
  CI_NTOP_INFO,
  CI_RECEIVED_DATACALL_INFO_LINE,
  CI_ORIGINATION_DATACALL_INFO_TITLE,
  CI_ORIGINATION_DATACALL_INFO,
  CI_ORIGINATION_DATACALL_INFO_LINE,
  CI_RECEIVED_DATACALL_INFO_TITLE,
  CI_RECEIVED_DATACALL_INFO,  
  CI_MAX
};





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
#define sRIGHT_MARGIN       3


void sInitLastCallInfoDisplay()
{
    HCONTROL hControl;
    UINT8    nCtrl;
    BYTE     pszBuffer[CALL_INFO_SIZE];
    clk_julian_type julian;

    /*  음성발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_LAST_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 음성착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_RECEIVEDCALL_LAST_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* N.Top */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_NTOP_USETIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_WAP_LAST_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
           julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);

    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);
    /* 데이터발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_ORIGINATEDCALL_LAST_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
     /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);
    
    /* 데이터착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_RECEIVEDCALL_LAST_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
           julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
   
    

    EQS_StartFormatter();
    for(nCtrl = 0; nCtrl <CI_MAX; nCtrl ++)
    {
        if((nCtrl % 3) != 1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_RIGHT, EVA_CENTER, FALSE);       
            /* jrkwon 2001.06.28 --> I like pretty screen */
            EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= ((R_SCROLLBAR_SIZE)/2 + sRIGHT_MARGIN);
        }
        EQS_NewLine();           
    }
    EQS_EndFormatter();


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
#define SKY_DEF_WIN_DY_CALLINFO  165
void SKY_API SKA_MainLastCallInfo(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START :   /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X, 
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY_CALLINFO,//SKY_DEF_WIN_DY, 
                    CL_NONE, 
                    SKY_GET_TOKEN(TKN_CALL_LASTCALL), 
                    WA_SHOW_HEADER);
      sInitLastCallInfoDisplay();
      EQS_FitCanvasSize();
        break;

    case SE_APP_END :     /* The very last event */
      EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        //sDrawLine();
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
void sInitCumulativeCallTimeDisplay()
{
    HCONTROL hControl;
    UINT8    nCtrl;
    BYTE     pszBuffer[CALL_INFO_SIZE];
    clk_julian_type julian;

    /*  음성발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 음성착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* N.Top */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_NTOP_USETIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_WAP_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);

    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 데이터발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 데이터착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    


    EQS_StartFormatter();
    for(nCtrl = 0; nCtrl <CI_MAX; nCtrl ++)
    {
        if((nCtrl % 3) != 1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_RIGHT, EVA_CENTER, FALSE);       
            /* jrkwon 2001.06.28 --> I like pretty screen */
            EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= ((R_SCROLLBAR_SIZE)/2 + sRIGHT_MARGIN);
        }
        EQS_NewLine();           
    }
    EQS_EndFormatter();

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
void sUpdateCumulativeCallInfoDisp()
{
    BYTE     pszBuffer[CALL_INFO_SIZE];
    clk_julian_type julian;


    /* 음성발신누적통화 */
    clk_secs_to_julian(SKY_GET_VOICE_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
            julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(CI_ORIGINATION_VOICECALL_INFO), pszBuffer);

    /* 음성착신누적통화 */
    clk_secs_to_julian(SKY_GET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
            julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(CI_RECEIVED_VOICECALL_INFO), pszBuffer);

    /* 데이터발신누적통화 */
    clk_secs_to_julian(SKY_GET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
            julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(CI_ORIGINATION_DATACALL_INFO), pszBuffer);

    /* 데이터착신누적통화 */
    clk_secs_to_julian(SKY_GET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
            julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(EQS_GetNthIOC(CI_RECEIVED_DATACALL_INFO), pszBuffer);

    /* N.Top */
    clk_secs_to_julian(SKY_GET_WAP_CUMULATION_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
             julian.hour, julian.minute, julian.second);    
    EQC_SetStaticText(EQS_GetNthIOC(CI_NTOP_INFO), pszBuffer);

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
enum tagHOTMENUINCUMMULATIVE_E
{
  INIT_VOICE_ORIGINATION = 0,       /*음성발신초기화  */
  INIT_VOICE_RECEIVED,              /*음성착신초기화  */
  INIT_N_TOP,                       /*n.Top초기화     */
  INIT_DATA_ORIGINATION,            /*데이터발신초기화*/
  INIT_DATA_RECEIVED,               /*데이터착신초기화*/
  INIT_ALL                          /*전체초기화      */
};
typedef enum tagHOTMENUINCUMMULATIVE_E    CUMMULATIVE_E;


void SKY_API SKA_MainCumulativeCallInfo(SKY_EVENT_T* pEvent)
{
    STATIC CUMMULATIVE_E  ITEM2B_INITIALIZED;
    STATIC HCONTROL       hAskMessageBox, hEndMessageBox;
    HCONTROL              hCtrlPopUp;

    SKY_TOKEN paCummulativeCallTime[6] =       /* Hot Menu!! */
    {
      TKN_CALL_CUMMULATIVE_MENU1,          /*음성발신초기화  */
      TKN_CALL_CUMMULATIVE_MENU2,          /*음성착신초기화  */
      TKN_CALL_CUMMULATIVE_MENU5,          /* nTop초기화     */
      TKN_CALL_CUMMULATIVE_MENU3,          /*데이터발신초기화*/
      TKN_CALL_CUMMULATIVE_MENU4,          /*데이터착신초기화*/      
      TKN_CALL_CUMMULATIVE_MENU6           /*전체초기화      */
    };

    
    switch(pEvent->EventID)
    {
    case SE_APP_START :   /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X, 
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY_CALLINFO, 
                    CL_MENU_NONE_NONE, 
                    SKY_GET_TOKEN(TKN_CALL_CUMMULATIVE), 
                    WA_SHOW_CML | WA_SHOW_HEADER);
      sInitCumulativeCallTimeDisplay();
      EQS_FitCanvasSize();
        break;

    case SE_APP_END :     /* The very last event */
      EQS_ReleaseDC();
        break;
    
    case SE_APP_SUSPEND : /* start child */
        break;
    
    case SE_APP_RESUME :  /* end child */
        break;
    
    case SE_APP_DRAW :    /* draw */
        //sUpdateCumulativeCallInfoDisp();
        break;

    case SE_SOFTKEY_MENU:
        hCtrlPopUp = EQC_PopUpMenu((EQS_TOKEN_EX*)paCummulativeCallTime, 6, 0);
        break;

    case SE_IO_POPUP_SELECT :      

        switch(pEvent->dwParam)
        {
         case INIT_VOICE_ORIGINATION:
             ITEM2B_INITIALIZED = INIT_VOICE_ORIGINATION;
             hAskMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_VOICE_ORIGTIME), 
                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

         case INIT_VOICE_RECEIVED:
             ITEM2B_INITIALIZED = INIT_VOICE_RECEIVED;
             hAskMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_VOICE_RECEIVETIME), 
                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

         case INIT_DATA_ORIGINATION:
             ITEM2B_INITIALIZED = INIT_DATA_ORIGINATION;
             hAskMessageBox = EQC_MessageBox(
                                 (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_DATA_ORIGTIME), 
                                  MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

         case INIT_DATA_RECEIVED:
             ITEM2B_INITIALIZED = INIT_DATA_RECEIVED;
             hAskMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_DATA_RECEIVETIME), 
                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

         case INIT_N_TOP:
             ITEM2B_INITIALIZED = INIT_N_TOP;
             hAskMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_NTOP_TIME), 
                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
            break;

         case INIT_ALL:
             ITEM2B_INITIALIZED = INIT_ALL;
             hAskMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_ALL_TIME), 
                                   MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
             break;
        }        
       break;

//    case SE_SOFTKEY_INIT:
//         EQC_MessageBox((BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_TIME), 
//                        MESSAGE_DEFAULT_TIME, MBT_YESNO, MBT_CONFIRM);
//       break; 

       
    case SE_IO_MESGBOX_YES:
        if(ITEM2B_INITIALIZED == INIT_VOICE_ORIGINATION)
        {
            SKY_SET_VOICE_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                              (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_VOICE_ORIGTIME_END), 
                              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(ITEM2B_INITIALIZED == INIT_VOICE_RECEIVED)
        {
            SKY_SET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                              (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_VOICE_RECEIVETIME_END), 
                              MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(ITEM2B_INITIALIZED == INIT_DATA_ORIGINATION)
        {
            SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_DATA_ORIGTIME_END), 
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(ITEM2B_INITIALIZED == INIT_DATA_RECEIVED)
        {
            SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_DATA_RECEIVETIME_END), 
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(ITEM2B_INITIALIZED == INIT_N_TOP)
        {
            SKY_SET_WAP_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                                  (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_NTOP_TIME_END), 
                                   MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
        else if(ITEM2B_INITIALIZED == INIT_ALL)
        {
            SKY_SET_VOICE_CUMULATION_USING_TIME(0);
            SKY_SET_VOICE_RECEIVEDCALL_CUMULATION_USING_TIME(0);
            SKY_SET_DATA_ORIGINATEDCALL_CUMULATION_USING_TIME(0);
            SKY_SET_DATA_RECEIVEDCALL_CUMULATION_USING_TIME(0);
            SKY_SET_WAP_CUMULATION_USING_TIME(0);
            hEndMessageBox = EQC_MessageBox(
                      (BYTE*)SKY_GET_TOKEN(TKN_CALL_INIT_CUMMULATIVE_ALL_TIME_END), 
                       MESSAGE_DEFAULT_TIME, MBT_NONE, MBT_SUCCESS);
        }
       break;

     case SE_IO_MESGBOX_NO:
       break;

     case SE_IO_MESGBOX_END:
       if(pEvent->wParam == hEndMessageBox)
       {
          sUpdateCumulativeCallInfoDisp();
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
void sInitTotalCallTimeDisplay()
{
    HCONTROL hControl;
    UINT8    nCtrl;
    BYTE     pszBuffer[CALL_INFO_SIZE];
    clk_julian_type julian;

    /*  음성발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_TOTAL_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 음성착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_VOICE_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_VOICE_RECEIVEDCALL_TOTAL_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* N.Top */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_NTOP_USETIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_WAP_TOTAL_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
           julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);    
    
    
    /* 데이터발신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_ORIGINATION_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_ORIGINATEDCALL_TOTAL_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer,(CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
          julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    /* 점선 */
    EQC_Image(EQS_NA, EQS_NA, (BITMAP*)&g_BmpHorizontalLine);

    /* 데이터착신 */
    EQC_StaticText(EQS_NA, EQS_NA, TKN_CALL_DATA_RECEIPT_TIME);
    hControl = EQC_StaticText(EQS_NA, EQS_NA, TKN_BLANK);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_NOBORDER | CS_STATIC_TEXT_LEFT);// | CS_STATIC_TEXT_NO_CHANGE);
    clk_secs_to_julian(SKY_GET_DATA_RECEIVEDCALL_TOTAL_USING_TIME(), &julian);
    sprintf((CHAR*)pszBuffer, (CHAR*)SKY_GET_TOKEN(TKN_CALL_FMT_USE_TIME), 
           julian.hour, julian.minute, julian.second);
    EQC_SetStaticText(hControl, pszBuffer);
    

    EQS_StartFormatter();
    for(nCtrl = 0; nCtrl <CI_MAX; nCtrl ++)
    {
        if((nCtrl % 3) != 1)
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_LEFT, EVA_CENTER, FALSE);       
        }
        else
        {
            EQS_SetControlPosition(EQS_GetNthIOC(nCtrl), ES_AUTO, 0, EHA_RIGHT, EVA_CENTER, FALSE);       
            /* jrkwon 2001.06.28 --> I like pretty screen */
            EQC_GET_X(EQS_GetNthIOC(nCtrl)) -= ((R_SCROLLBAR_SIZE)/2 + sRIGHT_MARGIN);
        }
        EQS_NewLine();           
    }
    EQS_EndFormatter();

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
void SKY_API SKA_MainTotalCallInfo(SKY_EVENT_T* pEvent)
{
    switch(pEvent->EventID)
    {
    case SE_APP_START :   /* The very first event */
      EQS_GetDC(TRUE);
      EQS_SetWindow(SKY_DEF_WIN_X, 
                    SKY_DEF_WIN_Y, 
                    SKY_DEF_WIN_DX, 
                    SKY_DEF_WIN_DY_CALLINFO, 
                    CL_NONE, 
                    SKY_GET_TOKEN(TKN_CALL_TOTAL), 
                    WA_SHOW_HEADER);
      sInitTotalCallTimeDisplay();
      EQS_FitCanvasSize();
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
