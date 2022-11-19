/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_evnt.h
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
** 2000-10-30 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_EVNT_H_
#define _SKY_EVNT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* is the EventID is an EQS native event? */
#define SKY_IS_EQS_NATIVE_EVENT(EventID) ((EventID) < EQE_BYPASS)


/* local definition */
#define sNONE_EVENT              -1                           //EQS_OFFSET_MAX_EVENT        500    
#define sOFFSET_KEY_EVENT        EQS_OFFSET_MAX_EVENT+0       /* 500 ~ 599 */
#define sOFFSET_SMS_EVENT        EQS_OFFSET_MAX_EVENT+100     /* 600 ~ 699 */
#define sOFFSET_CALL_EVENT       EQS_OFFSET_MAX_EVENT+200     /* 700 ~ 799 */
#define sOFFSET_PH_EVENT         EQS_OFFSET_MAX_EVENT+300     /* 800 ~ 899 */
#define sOFFSET_TM_EVENT         EQS_OFFSET_MAX_EVENT+400     /* 900 ~ 999 */
#define sOFFSET_SND_EVENT        EQS_OFFSET_MAX_EVENT+500     /* 1000 ~ 1099 */
#define sOFFSET_SYS_EVENT        EQS_OFFSET_MAX_EVENT+600     /* 1100 ~ 1199 */
#define sOFFSET_OTASP_EVENT      EQS_OFFSET_MAX_EVENT+700     /* 1200 ~ 1299 */
#define sOFFSET_INBAND_EVENT     EQS_OFFSET_MAX_EVENT+800     /* 1300 ~ 1399 */
#define sOFFSET_VMEMO_EVENT      EQS_OFFSET_MAX_EVENT+900     /* 1400 ~ 1499 */
#define sOFFSET_WAP_EVENT        EQS_OFFSET_MAX_EVENT+1000    /* 1500 ~ 1599 */
#define sOFFSET_AUTO_ANS_EVENT   EQS_OFFSET_MAX_EVENT+1100    /* 1600 ~ 1699 */
#define sOFFSET_USER_EVENT       EQS_OFFSET_MAX_EVENT+1200    /* 1700 ~ 1799 */
#define sOFFSET_EIF_EVENT        EQS_OFFSET_MAX_EVENT+1300    /* 1800 ~ 1899 */
#define sOFFSET_TEST_EVENT	     EQS_OFFSET_MAX_EVENT+1400    /* 1900 ~ 1999 */
#define sOFFSET_GVM_EVENT	     EQS_OFFSET_MAX_EVENT+1500    /* 2000 ~ 2099 */


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/* SE_XXX is not used, it is used as a delimeter */
enum tagSKY_EVENT_E 
{
#undef DEFINE_EVENT
#undef DEFINE_EVENT_CATEGORY
#define DEFINE_EVENT_CATEGORY(nID, nCategory) SE_##nID = nCategory,
#define DEFINE_EVENT(nID) SE_##nID,
#include "eqs_evnt.tbl"

    SE_KEY_EXT = sOFFSET_KEY_EVENT, 

                                /* No more keycodes available */
    //SE_KEY_SEND,                /* send key */
    SE_KEY_PHONE_OPEN,          /* flip or folder is open */
    SE_KEY_PHONE_CLOSE,         /* flip or folder is close */
    SE_KEY_SPEAKER,             /* speaker phone on/off key */
    SE_KEY_LOCK,                /* lock on/off key */
    SE_KEY_POWER,               /* power key -- on or off ? */
    SE_KEY_POWER_OFF,           /* power off key */
    SE_KEY_MANNERS_LONG,        /* manners mode on/off key */
    SE_KEY_EAR_JACK_ON,         /* ear jack is placed */
    SE_KEY_EAR_JACK_OFF,        /* ear jack is off */
    SE_KEY_EAR_CLICK,           /* ear jack button is clicked */
    SE_KEY_EAR_DOUBLE_CLICK,    /* ear jack button is double-clicked */
    //SE_KEY_EAR_LONG_CLICK,      /* ear jack button is pressed long time */
    SE_KEY_HANDS_FREE_ON,       /* Phone was placed in hands-free cradle */
    SE_KEY_HANDS_FREE_OFF,      /* Phone was lifed from hands-free cradle */
    SE_KEY_RELEASE,             /* a key is released */
    SE_KEY_CHARGING_ON,         /* charging on */
    SE_KEY_CHARGING_OFF,        /* charging on */
    SE_KEY_EXT_POWER_ON,        /* external power is placed in */
    SE_KEY_EXT_POWER_OFF,       /* external power is detached */
    SE_KEY_CAMERA_JACK_ON,         /* Camera inserted */
    SE_KEY_CAMERA_JACK_OFF,        /* Camera removed */
    SE_KEY_RELEASE_AUTO_ANSWER,    /* Auto Answer Key RElease Event */

    /*========================================================================
    ** SMS events
    */
//#ifdef FEATURE_UASMS
    SE_SMS = sOFFSET_SMS_EVENT,
    SE_SMS_MESSAGE,             /* SMS message received */
    SE_SMS_MYBELL,
    SE_SMS_STATUS_MESSAGE,      /* SMS status message received */
    SE_SMS_MO_STATUS_MESSAGE,   /* SMS MO status message */
	SE_LOCATION_INFORM,
	SE_LOCATION_TIMEOUT,        /* Location Timer Time out */
//#endif /* FEATURE_UASMS */

    /*========================================================================
    ** Call managing events
    */
    SE_CALL = sOFFSET_CALL_EVENT,
    SE_CALL_INCOMING,             /* incoming call */
    SE_CALL_ORIGINATED,           /* a call is originated */
    SE_CALL_CONNECT,              /* call connected */
    SE_CALL_ENDED,                /* The call ended  */
    SE_CALL_PRIVACY_CHANGED,      /* the voice privacy is changed */
    SE_CALL_SO_CHANGED,           /* the service option is changed */
    SE_CALL_CALLER_ID,            /* Caller ID could be call waiting */
    SE_CALL_FLASHED,              /* a flash is sent */
    SE_CALL_COMMAND_ERROR,        /* some error happened to call commands */
    SE_CALL_EXT_DISPLAY,          /* display exit display infomation */
    SE_CALL_SIGNAL,               /* UI_CALL_SIGNAL_F command */
    SE_CALL_SIGNAL_TIMEOUT,
    SE_CALL_PLAY_BELL,            /* BV_BELL_AFTER_VIBRATION */
    SE_CALL_STOP_BELL,            /* Stop alerting */
    /* newly added for EQUS engine */
    SE_CALL_RELEASED,             /* UI_RELEAE_F call is released */
    SE_CALL_DISPLAY,
    SE_CALL_SENDDTMF,
	SE_CALL_CALLED_PARTY,         /* 투넘버서비스 */
	SE_CALL_FROM_CALLRECORDING,   /* 통화 중 녹음 중에 Second Call이 왔을 경우*/
    SE_CALL_STACK_CHANGED,

    /*========================================================================
    ** phone events 
    */
    SE_PH = sOFFSET_PH_EVENT,
    SE_PH_INUSE_STATE,          /* inuse state is changed */
    SE_PH_SERVICE_STATE,        /* service state is changed */
    SE_PH_OPERATING_MODE,       /* operating mode is changed */
    SE_PH_CMDA_LOCK_MODE,       /* CDMA lock mode is changed */
    SE_PH_PREFERED_MODE,        /* prefered mode is changed */
    SE_PH_PREFERED_SYSTEM,      /* prefered system is changed */
    SE_PH_ANSWER_VOICE,         /* answer voice as data setting is changed*/
    SE_PH_RESTRICTION_LEVEL,    /* restriction level is changed */
    SE_PH_CURRENT_NAM,          /* current NAM is changed */
    SE_PH_NAM_SELECT,           /* NAM selection is changed */
    SE_PH_ROAM_STATUS,          /* roaming status is changed */
    SE_PH_INFO_AVAIL,           /* Phone information is now available */
    SE_PH_COMMAND_ERROR,        /* some error happend to phone commands */
    /* newly added for EQUS engine */
    SE_PH_MAINTREQ,             /* Maintenance requested */
    SE_PH_UNLOCK,                
    SE_PH_RSSI,                 /* new RSSI level */

    /*========================================================================
    ** sound events 
    */
    SE_SND = sOFFSET_SND_EVENT,
    SE_SND_MULTI_TONE_DONE,     /* multi-tone done playing */
    SE_SND_MUTE,                /* speaker mute */
    SE_SND_ABBR_ALERT,          /* abbreviated alert */
    SE_SND_DTMF,

    /*========================================================================
    ** system events 
    */
    SE_SYS = sOFFSET_SYS_EVENT,
    SE_SYS_OFFLINE,             /* go offline */
    SE_SYS_INFO,                /* information came in from base station */
    SE_SYS_TO_IDLE,             /* back out to idle state */
    SE_SYS_BAD_BATT,            /* Battery used up, powering down: girin. */
    SE_SYS_LOW_BATT,            /* Battery almost runs short, low batt. girin */
#ifdef FEATURE_MULTI_SID
    SE_SYS_MULTI_SID,           /* incomming multi SID */               
#endif //FEATURE_MULTI_SID
    SE_SYS_UPDATE_DISPLAY,      /* update display at once */
    
#ifdef FEATURE_OTASP
    /*========================================================================
    ** otasp events 
    */
    SE_OTASP = sOFFSET_OTASP_EVENT,
    SE_OTASP_END_ACTIVATION,    /* End otasp activation call */
    SE_OTASP_STATUS,            /* Sent otasp activation ending status */
#endif

    /*========================================================================
    ** inband events 
    */
    SE_INBAND = sOFFSET_INBAND_EVENT,
    SE_INBAND_FORWARD_BURST_DTMF,
    
    /*========================================================================
    ** voice memo events 
    */
    SE_VM    = sOFFSET_VMEMO_EVENT,
    SE_VM_REC_END,
    SE_VM_PLAY_END,
    SE_VM_PLAY_CONTINUE,
    SE_VM_INCOMINGCALL_INRECORDING,
    SE_VM_PLAY_TIMEDELAY,
    SE_VM_REC_START,
    SE_VM_PLAY_START,
	SE_AANS_PLAY_END,
    SE_VM_KEY_RECORD_LONG_DUMMY,

	/*========================================================================
    ** wap events 
    */
    SE_WAP  =  sOFFSET_WAP_EVENT,
#ifdef FEATURE_WAP
    /*========================================================================
    ** ytchoi 01/07/16
	** AU4.3 Event  Now according to #define, enum name may conflict!!
    */ 
    SE_WAP_REPORTERROR,
    SE_WAP_GOMINOR,
    SE_WAP_COMPLETE,
    SE_GOTO_SKVM,
    SE_GOTO_GVM,
    SE_WAP_MAKECALL,
    SE_WAP_PUSH,
    SE_WAP_MAIL,
    SE_WAP_SO_REQ,
    SE_WAP_REDIRECT,
    SE_WAP_REDIRECTFAIL,
    SE_WAP_INITACCEESSFAIL,
    SE_WAP_SHOWCARD,
    SE_WAP_IN_PIM,
    SE_WAVELET_IN_WAP,
    SE_MUXNEGO_SUCCESS,
    SE_MUXNEGO_FAIL,
#ifdef	FEATURE_SKT_EIF	//yjahn96 01.03.15
    SE_WAP_EIF_INPUT,
    SE_WAP_EIF_SELECT,
	SE_WAP_EIF_AFTER_SELECT,
	SE_WAP_EIF_UPONG,
	SE_WAP_EIF_NKEY_OPEN,		
	SE_WAP_EIF_LINKTEXT_OPEN,	
#endif	//#ifdef	FEATURE_SKT_EIF
#endif //FEATURE_WAP

#ifdef FEATURE_MCOMMERCE
	SE_WAP_SCRIPT,
	SE_WAP_SCRIPT_RESULT,
	SE_MW_CERT_MIME,
	SE_MW_USER_SIGNING,
	SE_MW_USER_SIGNING_PIN,
	SE_MW_USER_CANCEL,
#endif //#ifdef FEATURE_MCOMMERCE
#ifdef FEATURE_USE_IRDA
    SE_IRDA_EX_RX_DATA_F,
#endif
    SE_DM_MO_CMD_F,
	/*========================================================================
    ** auto answer 
    */
    SE_AUTO  = sOFFSET_AUTO_ANS_EVENT,
	SE_AUTO_ANSWER,
    SE_AUTO_ANSWER_MENT,
    SE_ANSWER,      // 자동착신

	SE_USER  = sOFFSET_USER_EVENT,
	SE_USER_1,
	SE_USER_2,
	SE_USER_3,
	SE_USER_4,

	/*========================================================================
    ** EIF Event
    */
	SE_EIF  = sOFFSET_EIF_EVENT,  
    SE_SET_LCD,             //redstar  2001.02.05
    SE_RESET_LCD,
    SE_CLEAR_LCD,
    SE_DRAW_LCD,
    SE_MO_REQ,
    SE_EM_REQ,
    SE_ORI,
    SE_ORI_CALL,
    SE_PIN_UP,
	SE_EIF_PLAY_BEEP,
	SE_EIF_PLAY_BELL,
	SE_EIF_STOP_BELL,
//#ifdef FEATURE_UI_DORMANT
	SE_DATA_DORM_F,
	SE_DATA_IDLE_F,
//#endif

	/*========================================================================
    ** test event 
    */
	SE_KEYPAD_TEST = sOFFSET_TEST_EVENT,

	/*========================================================================
    ** GVM Event
    */
	SE_TCPCONNECT_SUCCESS = sOFFSET_GVM_EVENT,
	SE_TCPCONNECT_FAIL,
    SE_SOCK_RELEASED,
    SE_PTP_CONNECT_SUCCESS,
    SE_PTP_CONNECT_FAIL,
    SE_PTP_RELEASED,
	SE_TONETGAME,
	SE_RECV_DATA,
	SE_GVM_TIMER1,
	SE_GVM_TIMER2,
	SE_GVM_TIMER3,

	/*========================================================================
    ** Data Service Event        white 2001.03.20 
    */
    SE_DTR_DISPLAY_CHANGE_F,        
    SE_DATA_CALL,

	/*========================================================================
	** PHOTOMAIL Event
    */
	SE_PHOTOMAIL,

    /*========================================================================
	** SKVM Event
    */
#ifdef FEATURE_SKVM
	SE_TOSKVM,
	SE_SKVM_INPUT_DATA,
	SE_SKVM_KEYEVENT,
    SE_SKVM_STARTED,
    SE_SKVM_STOPPED,
    SE_SKVM_STOPPED_TO_GVM,
    SE_SKVM_SMS_SEND,
    SE_SKVM_VOICECALL_CONNECT,
    SE_SKVM_PAUSED,
    SE_SKVM_RESUMED,
    SE_SKVM_RECEIVED_DATA,
    SE_SKVM_STOPPED_TO_WAP,
    SE_SKVM_STOPPED_BY_ERROR,
	SE_SKVM_PLAY_ALARM,
#ifndef TEST_TORTOL
#define TEST_TORTOL
#endif
#ifdef TEST_TORTOL
	SE_SKVM_PACEMAKER,
	SE_SYS_PACEMAKER_FROM_SKVM,
#endif // TEST_TORTOL
#endif

    /*========================================================================
	** WAVELET Event in WAP 
    */
    SE_TOWAVELET,    
    SE_DM_UPDATE_IDLE,      //white 2001.06.12 for DataManager Schedule
    SE_DM_IN_PROCESSING,        //white 2001.06.19 for DataManager Schedule
    SE_DM_OUT_PROCESSING,
#ifdef FEATURE_VAS
    /*========================================================================
	** VAS Event 
    */
    SE_SUCCESS_F,    
	SE_FAIL_F,
	SE_NET_CONNECTED,
	SE_NET_DISCONNECTED,
#endif//feature_vas

	SE_NDSS_RECONNECT,

#ifdef FEATURE_WAVELET // yoursun 01/06/08
	SE_WAV_PLAY,
	SE_WAV_STREAM,
#endif //#ifdef FEATURE_WAVELET

    SE_ERROR                       /* always last */
};
typedef enum   tagSKY_EVENT_E           SKY_EVENT_E;


typedef struct tagSKY_EVENT_T   SKY_EVENT_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagSKY_EVENT_T
{
    SKY_EVENT_E EventID;
    WORD        wParam;
    DWORD       dwParam;
//    DWORD       dwExtEventID;  /* passing the SKY event to EQUS system */
};
extern BOOL g_fSubBackLightOn;
extern BOOL g_fSubBackLightOff;

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern void SKY_API (*g_pfnEQS_Dispatcher)(EQS_APPLET *pfnApplet, EQS_EVENT* pEvent);
extern void SKY_API SKY_DefEventProc(SKY_EVENT_T* pInEvent);

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_EVNT_H_ */



