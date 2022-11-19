/*
** ============================================================================
**
** Project: 
**     Ori
**
** File:
**     ska_call.h
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
** 2000-04-01 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKA_CALL_H_
#define _SKA_CALL_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
//#include "eqs.h"
//#include "eqa.h"

#include "cm.h"

/*
#include "uistate.h" // for global variable hjcho 09.07

#include "nv.h"      // for global variable hjcho 09.07
*/
/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define DISP_TIME 250L        /* check RSSI every 250 ms */
#define PAUSE_TIME 2000L      /* Gemini Timed Pause is 2 seconds */
#define ONE_YEAR 31536000L    /* one year in seconds */

#define T_PAUSE_CHAR     'T'
#define P_PAUSE_CHAR     'P'



#define UI_ED_BUF_SIZE  20
#define SKY_CALL_MAX_LTRS     32
#define EQS_MAX_DIAL_DIGITS   32


/* WAP의 dorment 상태에서   */
/* Incomming Call이 왔을 때 */
/* 사용되는 enum type       */
enum tagCALLFROMWAP_E
{
    CALL_SKA_WAPEXIT = 0,
    CALL_SKA_WAPLOADING,
    CALL_SKA_WAPHANDLE_KEY,
    CALL_SKA_WAPINPUT,
    CALL_SKA_WAPNewURL,
	CALL_SKA_PHONE_BOOK,
    CALL_SKA_NONE
};

typedef enum tagCALLFROMWAP_E     CALLFROMWAP_E;



enum tagCALLTYPE_E
{
    CT_CALL_TO = 0,
    CT_CALLED_BY,
    CT_MISSED_CALL,
    CT_SMS_MO,
    CT_SMS_MT,
    CT_CALLED_BY_INPROG
};
typedef enum tagCALLTYPE_E  CALLTYPE_E;

enum tagDATASERVICE_E
{
  DS_ANIMATION = 0,
  DS_FRAME,
  DS_STATE,
  DS_TIME_TITLE,
  DS_TIME
};


enum tagHOTMEMU_E
{
  BURSTMYNUMBER = 0,       /* 자기번호발신  */
  TOGGLEMUTE    ,          /* 송화음차단    */
  CALLFORWARD   ,          /* 호전환 서비스 */
  CALLWAITING              /* 호보류 서비스 */
};

enum tagCALL_MODE_E
{
  CALL_STATE_NONE = 0,
  ORIGATTEMPT,
  ORIGINATED ,
  ALERTED    ,
  WAITRETRY  ,
  INPROG     ,
  PAUSED     ,
  MULTIWAIT  ,
  CALLENDDISPLAY
};
typedef enum tagCALL_MODE_E  CALL_MODE_E;

typedef struct tagTWONUMBER_T           TWONUMBERINFO_T;   
struct tagTWONUMBER_T{                  /* editor buffer type   */
  BOOL    fSecret;                      /* secret number or not */
  BOOL    fChild;                       /* child number         */
  BYTE    aBuf[MAX_DIAL_DIGITS];        /* the data             */
  UINT8   nPos;                         /* length of the data   */
};


/*
**-----------------------------------------------------------------------------
**  CNIP Section                                                   Start
**-----------------------------------------------------------------------------   
*/

/*
** CNI structure
*/
typedef struct tagCNI_T                 CNI_T;
struct tagCNI_T{                                   /* Calling Number ID type      */
  EDITORINFO_T      Num;                           /* Number storage              */
  TWONUMBERINFO_T   CalledParty;                   /* Number storage (Two Number) */
  BYTE              aName[SKY_CALL_MAX_LTRS];            /* Name from CNAP              */
  BYTE              aNameInPhoneBook[SKY_CALL_MAX_LTRS]; /* Name associated with number */
  BYTE              aGroupNAME[SKY_CALL_MAX_LTRS];       /* Group name                  */
  BYTE              bGNameLen;                     /* Group name length           */
  BYTE              bNameLen;                      /* length of the name from CNAP*/
  BYTE              bNameLenInPhoneBook;           /* length of the name          */
  BYTE              bPI;                           /* Display indicator           */
//BYTE              bPersonalBell;                 /* Personal Bell Index         */
  BELL_TYPE_T       GroupBell;                     /* Group Bell Index            */
#ifdef FEATURE_COLOR_ID_LCD                        //white 2001.02.22 inrt.
  ID_LCD_COLOR_E    GroupBacklight;                /* Group Backlight */
#endif
  DWORD             dwPicture;                      /* Picture Index               */
  BOOL              fSecret;                       /* Secret ?                    */
};
/*
**-----------------------------------------------------------------------------
**  CNIP Section                                                   End
**-----------------------------------------------------------------------------   
*/


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct MISSED_CALL_T    MISSED_CALL;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct MISSED_CALL_T {
    UINT8   nMissedCalls;
    DWORD   dwCurrentTime; /* dword current_time */
};

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
/* jrkwon 2001.07.19 */
extern SKY_EVENT_E g_LastCallKeyEvent;


/* jrkwon 2001.06.28 */
extern BOOL SKY_API IsInVoiceConversation(void);
extern void IPAirDataManagerEndTime(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_cm_call_info

DESCRIPTION
  update part (which is from call manager) of current call information

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_cm_call_info(const cm_call_info_s_type* cm_call_info_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_cm_inband_info

DESCRIPTION
  update part (which is from call manager) of current inband information

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_cm_inband_info(const cm_inband_info_s_type* cm_inband_info_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_start_fwd_burst_dtmf

DESCRIPTION
  Start playing the foward burst DTMF tones

DEPENDENCIES
  None

RETURNS
  void

SIDE EFFECT
  None
===========================================================================*/
void ui_start_fwd_burst_dtmf(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_stop_fwd_burst_dtmf

DESCRIPTION
  Stop playing the foward burst DTMF tones

DEPENDENCIES
  None

RETURNS
  void

SIDE EFFECT
  None
===========================================================================*/
void ui_stop_fwd_burst_dtmf(void);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_setup_voice_privacy_disp

DESCRIPTION
  set up voice privacy display

DEPENDENCIES
  English, size of UI_TIME_F >=12, will be changed later

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_setup_voice_privacy_disp (void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_clear_voice_privacy_disp

DESCRIPTION
  clear voice privacy display

DEPENDENCIES
  English, size of UI_TIME_F >=12, will be changed later

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_clear_voice_privacy_disp (void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_autoanswer_is_ready

DESCRIPTION
  should we autoanswer the call?

DEPENDENCIES
  global variable ui.isanswer, ui.earpiceup, ui.answer, ui.rings

RETURNS
  Ture if autoanswer conditions are met, False otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_autoanswer_is_ready (void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_autoanswer_call

DESCRIPTION
  auto answer the call, record module event if necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_autoanswer_call (void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_call

DESCRIPTION
  Is the phone in a call

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in a call?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_call(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_conversation

DESCRIPTION
  Is the phone in a conversation

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in a conversation?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_conversation(void);



/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_alert

DESCRIPTION
  Is the phone getting an incoming call

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in alert ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_alert(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_curr_call_allow_sound

DESCRIPTION
  Is the service option enable playing singal (sound)?

DEPENDENCIES
  None

RETURN VALUE
  Is the service option enable playing singal (sound)?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_allow_sound(void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_mutable

DESCRIPTION
  Is current call mutable ?

DEPENDENCIES
  None

RETURNS
  Is current call mutable ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_mutable(void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_so

DESCRIPTION
  Get service option of current call

DEPENDENCIES
  None

RETURNS
  service option of current call

SIDE EFFECTS
  None
===========================================================================*/
word ui_curr_call_so(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_is_data

DESCRIPTION
  Is current call a data call ?

DEPENDENCIES
  None

RETURNS
  Is current call a data call ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_is_data(void);


/*===========================================================================
FUNCTION  ui_curr_call_is_sms

DESCRIPTION
  Is current call a sms call ?

DEPENDENCIES
  None

RETURNS
  Is current call a sms call ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_is_sms(void);

#ifdef FEATURE_SKTT_EQUS_SMS    //added by reo 2001.06.25
boolean ui_curr_call_is_sms_in_tc(void);     //traffic channel
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_data_alpha_tag

DESCRIPTION
  return alpha tag for current data call

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
char* ui_curr_data_alpha_tag(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_e911_orig_if_needed

DESCRIPTION
  update E911 origination display if it is necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_e911_orig_if_needed(
  cm_srv_state_e_type old_srv_state, // old service state
  cm_srv_state_e_type new_srv_state  // new service state
);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_e911_cb_if_needed

DESCRIPTION
  Update E911 call back display when the service is changed  if it is 
  necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_e911_cb_if_needed(
  cm_srv_state_e_type old_srv_state, // old service state
  cm_srv_state_e_type new_srv_state  // new service state
);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_data_status_if_needed

DESCRIPTION
  Update data call status if necessary, will be called every 250 ms

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_data_status_if_needed(void);

void in_prog_mode_entry(void);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_get_so

DESCRIPTION
  Get the service option of the current call.

DEPENDENCIES
  None

RETURNS
  Service option

SIDE EFFECTS
  None
===========================================================================*/
word ui_curr_call_get_so
(
  void
);

extern BOOL SKY_API IsWhisperMode(void);
extern BOOL SKY_API IsInVoiceConversation(void);
extern BYTE SKY_API GetCallSignalType(void);
extern BYTE SKY_API GetCallSignal(void);
extern BYTE SKY_API GetCallSignalAlertPitch(void);
extern BOOL SKY_API IsDataCall(void);
extern BOOL SKY_API IsEarpieceEnable(void);
extern void SKY_API UpdateOriginatedDataCallInfo(DWORD dwCallTime);
extern void SKY_API UpdateReceivedDataCallInfo(DWORD dwCallTime);
extern void SKY_API InProgModeEntry(void);


void ui_update_cm_call_info(const CM_CALL_INFO_T* cm_call_info_ptr);
void ui_update_cm_inband_info(const cm_inband_info_s_type* cm_inband_info_ptr);
boolean ui_curr_call_allow_sound(void);
boolean ui_curr_call_is_sms(void);
#ifdef FEATURE_OTASP_OTAPA 
boolean ui_is_in_otapa_call(void);
#endif
boolean ui_is_in_conversation(void);
word ui_curr_call_so(void);
static snd_tone_type digit_to_sound(
  byte ascii_digit 
);
void ui_stop_fwd_burst_dtmf(void);


void SetOriginationCall(BOOL fOriginationCall);
void NewMissedCall(void);
BYTE* GetMyNumber(void);

extern void SKY_API EndCall(void);
extern void SKY_API InitInProgDisplayInDataService(void);
extern void SKY_API InitOrigAttemptDisp(void);

extern void SKY_API SKA_MainCall(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_MainLastCallInfo(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_MainCumulativeCallInfo(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_MainTotalCallInfo(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_InProg(SKY_EVENT_T *pInEvent);
extern void SKY_API SKA_CallWaitingCall(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_StartOrigination(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_Alerted(SKY_EVENT_T *pEvent);
extern void SKY_API SetupCLIfromCallInfo(const cm_call_info_s_type* pCallInfo, SKY_EVENT_E Event);
extern void SKY_GetCallingTime(clk_julian_type *pJulian);
#ifdef FEATURE_SKY_SUBLCD
extern void SKY_DisplayCallingTimeInSubLCD(clk_julian_type *pJulian);
#endif
extern BOOL SKY_IsInCall(void);
extern void SetCallFromWAP(CALLFROMWAP_E CallFromWhere);
extern BOOL SKY_API IsSpace2RecordAutoAnswer(void);
extern void UpdateOriginatedDataCallInfo(DWORD dwCallTime);
extern void UpdateReceivedDataCallInfo(DWORD dwCallTime);
extern void ProcessAlertWithInfoMessage(SKY_EVENT_T* pEvent);
extern void CallDefaultEventProc(SKY_EVENT_T* pEvent,CALL_MODE_E  CallMode);
extern void DisplayCallTime(HCONTROL hControl);
extern void SKA_BurstDTMFWithMyNumber(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_ToggleMute(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_CallForward(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_CallPending(SKY_EVENT_T *pEvent);
extern void SKY_API SKA_AdjstConversationVol(SKY_EVENT_T* pEvent);
extern void SKY_API SKA_FlashWithInfo(SKY_EVENT_T *pEvent);

extern void SKY_API ClearMissedCallInfo(void);
extern void SKY_API NewMissedCall(void);
extern DWORD SKY_API GetMissedCallTime(void);
extern void SKY_API SKA_MissedCall(SKY_EVENT_T* pEvent);
extern BOOL IsCallTypeSMS(void);    

BOOL    SKY_API IsPauseMode(BYTE* aPhoneNumber);
UINT8   SKY_API ParsePhoneNumber(BYTE* aPhoneNumber, BYTE* szString);
BOOL    SKY_API IsData2DisplayInCallWaitingCall(void);

BOOL SKY_API IsWhisperMode(void);
BOOL SKY_API SKY_IsInTraffic(void);
BOOL SKY_API SKY_IsVoiceCall(void);
cm_call_end_e_type SKY_API SKY_GetCallEndStatus(void);

//void SKY_API DrawGeneralFrame(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _NUA_CALL_H_ */
