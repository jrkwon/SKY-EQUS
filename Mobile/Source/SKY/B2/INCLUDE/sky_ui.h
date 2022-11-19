/*
** ============================================================================
**
** Project: 
**     SKY
**
** File:
**     sky_ui.h
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

#ifndef _SKY_UI_H_
#define _SKY_UI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "smsi.h"
#include "uasms.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define SKY_IS_VALID_LOCAL_TIME(dwTime)  ((dwTime) >= SKY_ONE_YEAR)

/* -------------
** time
*/
/* check RSSI every 250 ms */
#define SKY_RSSI_DISP_TIME      250L
/* SKY Timed Pause is 2 seconds */
#define SKY_TIMED_PAUSE_TIME    2000L
/* one year in seconds. use in checking valid time */
#define SKY_ONE_YEAR            31536000L

/* TEMPORARY change, until deep sleep timing bug fixed. */
#define SKY_INVALID_SYS_TIME (31536000L - 1L)  /* less than one year in seconds */


/* Default IMSI values */
#define IMSI_MCC_0              999       /* 000 */
#define IMSI_S1_0               16378855   /* 0000000 */
#define IMSI_S2_0               999       /* 000 */
#define IMSI_CLASS0_ADDR_NUM    0xFF
#define IMSI_11_12_0            99

#define FEATURE_KOREA_BAND

/* Primary Paging Channel Defaults */
#define FMMIN   0
#define CDMAMIN 1

#define NID_DEFAULTS 65535

#define PCH_A_DEF 779           /* Primary Channel A Default */
#define PCH_B_DEF 779           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEF 738           /* Secondary Channel A Default */
#define SCH_B_DEF 738           /* Secondary Channel B Default */


/* Primary Paging Channel Defaults */
#define PCH_A_DEFAULT 779           /* Primary Channel A Default */
#define PCH_B_DEFAULT 779           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEFAULT 738           /* Secondary Channel A Default */
#define SCH_B_DEFAULT 738           /* Secondary Channel B Default */




/* pause character representation : NV uses the same */
#define UI_PSE_CHR 'P'
/* timed pause character representation : NV uses the same */
#define UI_TPSE_CHR 'T'
/* hyphen character INTERNAL representation */
#define UI_HYP_CHR '-'


#define ISPAUSE(chr) \
        ( ( (chr) == UI_TPSE_CHR ) || \
          ( (chr) == UI_PSE_CHR )   )


#define ISLOOP(so) \
        ( ( (so) == CAI_SO_LOOPBACK)    || \
          ( (so) == CAI_SO_LOOPBACK_13K)  )

#define ISMARKOV(so) \
        ( ( (so) == CAI_SO_RS1_MARKOV) || \
          ( (so) == CAI_SO_RS2_MARKOV) || \
          ( (so) == CAI_SO_MARKOV)     || \
          ( (so) == CAI_SO_MARKOV_13K)   )


/* 
** maximum values
*/
#define MC_DIALED_DIGITS        32 //MC_MAX_DIALED_DIGITS
#define MAX_DIALED_DIGITS       32
#define UI_MAX_DIALED_DIGITS    MAX_DIALED_DIGITS
#define UI_MAX_DATA_DIGITS      64

/* Signals for the UI task */
#define UI_CMD_Q_SIG      0x0001  /* Something on the command queue */
#define UI_KEY_SIG        0x0002  /* Key from HS task */
#define UI_RPT_TIMER_SIG  0x0004  /* Time to kick watchdog */
#define UI_NV_SIG         0x0008  /* Return from NV */

#define UI_PLAY_REC_SIG   0x0010  //redstar 00.12.19
#define UI_MULTI_STOP_SIG 0x0020  /* Multitone has ended */
#define UI_TIMERS_SIG     0x0080  /* any of the callback timers */
#define UI_RING_SIG       0x0100  /* End of a ring from SND task */

#ifdef FEATURE_IRDA_SIGNAL
/* rosa 2001-10-15 --> UI_NETWORK_CB_SIG과 동일하여 다르게 수정함 */
//#define UI_IRDA_RX_SIG	  0x00010000
#define UI_IRDA_RX_SIG	  0x0200
/* <-- */
#endif

#define UI_RECVDATA_Q_SIG 0x0400
#define UI_RSVD_0800_SIG  0x0800

#ifdef FEATURE_SKT_EIF	//yjahn96 01.03.15
#define UI_UPONG_SIG	  0x1000
#endif	//#ifdef FEATURE_SKT_EIF


#ifdef FEATURE_VAS
#define UI_NETWORK_CB_SIG 0x00010000
#define UI_SOCKET_CB_SIG  0x00020000
#define UI_DNS_CB_SIG     0x00040000
#define UI_PS_TIMER_SIG   0x00080000
#define UI_EVT_PROCESS_SIG 0x00100000
#endif//feature_vas

#ifdef FEATURE_WAP
#define WAP_CMD_Q_SIG  0x000200000
#define BAR_ANI_SIG    0x000400000
#define WAP_ANIMATION_TIMER_SIG 0x001000000
#define WAP_NOWRAP_TIMER_SIG    0x002000000
#define WAP_BROWSER_TIMER_SIG   0x004000000
#endif //#ifdef FEATURE_WAP 

#ifdef FEATURE_SKVM
#define XVM_TIMERS_SIG	0x8000    /*reuse TASK_START_SIG */
#define XVM_TIMERS2_SIG 0x000800000
#endif

#ifdef FEATURE_WAVELET
#define WAVELET_ALIVE_TIMER_SIG	0x008000000    /* */
#endif //FEAUTRE_WAVELET

#ifdef FEATURE_SKVM
	#ifdef SUPPORT_REX_TIMER_IN_SKVM
		#define XVM_ACTIVE_TIMER1_SIG 0x010000000    /* */
		#define XVM_ACTIVE_TIMER2_SIG 0x020000000    /* */
		#define XVM_ACTIVE_TIMER3_SIG 0x040000000    /* */
	#endif
#endif

#ifdef T_AUTH                     /* auth code available */
#define UI_A_KEY_DIGITS AUTH_A_KEY_DIGITS
#else
#define UI_A_KEY_DIGITS 26
#endif

#define UI_AUDIO_CONTROL_PKT_SIZE 34

#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif /* FEATURE_AUDIO_CONTROL */

/*--------------------------------------------------------------------------*/
#ifdef T_SLEEP
#include "sleep.h"

#define UI_ENABLE_SLEEP()                                   \
{                                                           \
    if(g_fOK2Sleep == FALSE)                                \
    {                                                       \
        SKY_Set_Signal(&sleep_tcb, SLEEP_UI_OKTS_SIG);      \
        SKY_TRACE(TL_HIGH, "UI Sleep enable");              \
        g_fOK2Sleep = TRUE;                                 \
    }                                                       \
}

#define UI_DISABLE_SLEEP()                                  \
{                                                           \
    if(g_fOK2Sleep)                                         \
    {                                                       \
        SKY_Clear_Signal(&sleep_tcb, SLEEP_UI_OKTS_SIG);    \
        SKY_TRACE(TL_HIGH, "UI Sleep disable");             \
        g_fOK2Sleep = FALSE;                                \
    }                                                       \
}
#else
#define UI_ENABLE_SLEEP()   SKY_TRACE(TL_LOW, "UI Sleep enable");
#define UI_DISABLE_SLEEP()  SKY_TRACE(TL_LOW, "UI Sleep disable");
#endif
/*--------------------------------------------------------------------------*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Commands to be sent to the User Interface task */
typedef enum {
  
  UI_SMS_F,                    /* Display/process incoming SMS message     */
  UI_SMS_STATUS_F,             /* Unable to store incoming SMS message     */
  UI_SMS_MO_STATUS_F,          /* SMS MO STATUS : added by reo :2001-01-04 */       
  UI_DATACALL_F,               /* Making a data call                       */
  UI_DS_ORIG_VCALL_F,          /* Voice origination from DS (autodialer)   */
  UI_DS_ANSWER_F,              /* DS Answered call, voice or data          */
  UI_DS_END_VCALL_F,           /* End call from DS                         */
  UI_DIAG_ORIG_F,              /* Originate a call                         */
  UI_DIAG_END_F,               /* End the call                             */
//UI_NET_DATA_ARRIVED_F,       /* Got UDP data for UP */
  
#ifdef FEATURE_DS_UP
#error code not present
#endif
#ifdef FEATURE_NV_WRITE_ONLINE
  UI_NV_WRITE_ONLINE_F,         /* Command to force UI to update           */
                                /* variable(s) that shadow a given NV item.*/
                                /* This is equivalent to a menu operation  */
#endif
#ifdef FEATURE_SMS_PACKET
  UI_GET_SMS_F,                 /* Command to read SMS message.  It is     */
                                /* important to do this inside the UI task */

  UI_DELETE_SMS_F,              /* Command to delete SMS message.          */
#endif

 
#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif

#ifdef FEATURE_ANALOG_DATA
#error code not present
#endif
 
  UI_CALL_INCOMING_F,             /* incoming call */
  UI_CALL_ORIGINATED_F,           /* a call is originated */
  UI_CALL_ANSWERED_F,             /* the call is answered */
  UI_CALL_CONNECT_F,              /* the call is connected */
  UI_CALL_ENDED_F,                /* the call is ended */
  UI_CALL_SO_CHANGED_F,           /* the service option is changed */
  UI_CALL_PRIVACY_CHANGED_F,      /* the voice privacy is changed */
  UI_CALL_CALLER_ID_F,            /* caller id, could be call waiting */
  UI_CALL_FLASHED_F,              /* a flash is sent */
  UI_CALL_SIGNAL_F,               /* new for ring event from base station */
  UI_CALL_ABRV_ALERT_F,           /* Generate a CDMA/AMPS abbreviated alert   */
  UI_CALL_ABRV_REORDER_F,         /* Generate an AMPS abbreviated reorder     */
  UI_CALL_ABRV_INTERCEPT_F,       /* Generate an AMPS abbreviated intercept   */
  UI_CALL_DISPLAY_F,              /* Display CDMA information                 */
  UI_CALL_CALLED_PARTY_F,         /* Called Party information                 */
  UI_CALL_CONNECTED_NUM_F,        /* Responding party information             */

#ifndef FEATURE_UASMS
  UI_SMS_MSG_WAITING_F,           /* message waiting */
#ifdef FEATURE_ACP
  UI_SMS_N_VOICE_F,               /* NAMPS Voice mail                         */
#endif /* FEATURE_ACP */
#endif /* FEATURE_UASMS */

#ifdef FEATURE_IS95B_EXT_DISP
  UI_CALL_EXT_DISPLAY_F,          /* extended display */
#endif

#ifdef FEATURE_IS95B_NDSS
  UI_CALL_NDSS_START_F,           /* NDSS start */
  UI_CALL_NDSS_CONNECT_F,         /* NDSS connect */
#endif
  
  UI_PH_INUSE_STATE_F,            /* inuse state is changed */
  UI_PH_SRV_STATE_F,              /* service state is changed */
  UI_PH_OPRT_MODE_F,              /* operating mode is changed */
  UI_PH_CDMA_LOCK_MODE_F,         /* CDMA lock mode is changed */
  UI_PH_MODE_PREF_F,              /* prefered mode is changed */
  UI_PH_SYS_PREF_F,               /* prefered system is changed */
  UI_PH_ANSWER_VOICE_F,           /* answer voice as data setting is changed*/
  UI_PH_RES_LEVEL_F,              /* restriction level is changed */
  UI_PH_CURR_NAM_F,               /* current NAM is changed */
  UI_PH_NAM_SEL_F,                /* NAM selection is changed */
  UI_PH_ROAM_STATUS_F,            /* roaming status is changed */
  UI_PH_INFO_AVAIL_F,             /* Phone information is now available */
  UI_PH_MAINTREQ_F,               /* CDMA Maintenance Required command        */
  UI_PH_RSSI_F,                   /* new RSSI value */
  UI_PH_STANDBY_SLEEP_F,          /* Entering powerdown sleep mode            */
  UI_PH_STANDBY_WAKE_F,           /* Exiting powerdown sleep mode             */

  UI_SS_SRV_CHANGED_F,            /* serving system parameter changed */

  UI_INBAND_REV_BURST_DTMF_F,     /* Reverse burst DTMF from UI or another app */
  UI_INBAND_FWD_BURST_DTMF_F,     /* Forwared burst DTMF from the base station */
  UI_INBAND_REV_START_CONT_DTMF_F,/* Reverse start cont DTMF from UI or another app */
  UI_INBAND_FWD_START_CONT_DTMF_F,/* Forward start cont DTMF from the base station */
  UI_INBAND_REV_STOP_CONT_DTMF_F, /* Reverse stop cont DTMF from UI or another app */
  UI_INBAND_FWD_STOP_CONT_DTMF_F, /* Forward stop cont DTMF from the base station */

  UI_CALL_CMD_ERR_F,              /* some error happend to call commands */
  UI_PH_CMD_ERR_F,                /* some error happend to phone commands */
  UI_INBAND_CMD_ERR_F,            /* some error happend to inband commands */

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
  UI_BCSMS_CMD_ERR_F,             /* some error happened */
  UI_BCSMS_CONFIG_F,              /* BC SMS configuration info */
  UI_BCSMS_PREF_F,                /* BC SMS user preference info */
  UI_BCSMS_TABLE_F,               /* BC SMS service table */
  UI_BCSMS_SRV_IDS_F,             /* BC SMS service IDs */
  UI_BCSMS_SRV_INFO_F,            /* BC SMS service info */
  UI_BCSMS_SRV_ADDED_F,           /* BC SMS service added */
  UI_BCSMS_SRV_DELETED_F,         /* BC SMS service deleted */
  UI_BCSMS_SRV_UPDATED_F,         /* BC SMS service updated */
  UI_BCSMS_ENABLE_FAILURE_F,      /* BC SMS service enable failure */
  UI_BCSMS_DISABLE_FAILURE_F,     /* BC SMS service disable failure */
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

#ifdef FEATURE_MCOMMERCE
  UI_WAP_SCRIPT_F,
  UI_WAP_CERT_MIME_F,
#endif //#ifdef FEATURE_MCOMMERCE
  
  UI_DIAG_KEYPAD_TEST_F,
  UI_MYBELL_F,
  UI_WAP_MAIL_CMDS,
  UI_WAP_PUSH_CMDS,
#ifdef FEATURE_SKVM
  UI_SKVM_LOAD_CMDS,
  UI_SKVM_DATA_CMDS,
  UI_SKVM_STARTED,
  UI_SKVM_STOPPED,
  UI_SKVM_STOPPED_TO_GVM,
  UI_SKVM_SMS_SEND,
  UI_SKVM_VOICECALL_CONNECT,
  UI_SKVM_PAUSED,
  UI_SKVM_RESUMED,
  UI_SKVM_RECEIVED_DATA,
  UI_SKVM_STOPPED_TO_WAP,
  UI_SKVM_STOPPED_BY_ERROR,
  UI_SKVM_PLAY_ALARM,
#endif
/* reo 2001.03.15 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
  UI_LOCATION_F,	
#endif
/* reo 2001.03.15 --} */

#ifdef	FEATURE_GVM
  UI_TONETGAME_F,
#endif //FEATURE_GVM
#if defined(FEATURE_SKT_SOCKAPI) || defined(FEATURE_SKT_PTPAPI)	//	khekim 01/02/15
  UI_SOCK_TCPCONNECT_SUCCESS_F,
  UI_SOCK_TCPCONNECT_FAIL_F,
  UI_SOCK_RELEASED_F,
  UI_PTP_CONNECT_SUCCESS_F,
  UI_PTP_CONNECT_FAIL_F,
  UI_PTP_RELEASED_F,
  UI_GVM_DOWNLOADSMS_CMD_F,
#endif	//	#if defined(FEATURE_SKT_SOCKAPI) || defined(FEATURE_SKT_PTPAPI)
  UI_DTR_DISPLAY_F,             /* white 2001.03.20 */
#ifdef FEATURE_USE_IRDA
  UI_IRDA_EX_RX_DATA_CMD,
#endif
  UI_DM_MO_CMD_F,
#ifdef	FEATURE_SKT_EIF  //redstar 2001.02.08
  UI_EIF_CMD_F,
#endif  //FEATURE_SKT_EIF
#ifdef FEATURE_UI_DORMANT
  UI_PKT_DORM_F,
  UI_PKT_IDLE_F,
  UI_NDSS_RECONNECT_F,
#endif //#ifdef FEATURE_UI_DORMANT
  UI_RECVDATA_F,
#ifdef FEATURE_SKTT_PM
  UI_PHOTOMAIL_F,
#endif//pm

#ifdef FEATURE_SKVM
  UI_TO_SKVM_F,
#endif //FEATURE_SKVM

#ifdef FEATURE_WAP_WAVELET
  UI_TOWAVELET_F,
#endif //FEATURE_WAP_WAVELET
  UI_DM_UPDATE_IDLE_F,              //white 2001.06.12 for DataManager Schedule
  UI_DM_IN_PROCESSING_F,            //white 2001.06.19 for DataManager Schedule
  UI_DM_OUT_PROCESSING_F,

#ifdef FEATURE_VAS
  // WAP
	WAP_ENTER_CMD,          
	WAP_KEY_CMD,
	WAP_RELEASE_CMD,
	WAP_POWERDOWN_CMD,
	WAP_GOTOURL_CMD,
	WAP_EXIT_DORMANT_CMD,
  // XVM
	XVM_ENTER_CMD,          
	XVM_END_CMD,
  //for test
	VAS_PPPOPEN_TEST_CMD,   
	VAS_PPPCLOSE_TEST_CMD,
	VAS_UDPTXRX_CMD,
	VAS_TCPTXRX_CMD,
	VAS_TXSEND_CMD,

	VAS_RXDATA_CMD,
	VAS_MULTISOCKET_CMD,
 //for test
    VAS_RELEASE_CMD,
	UI_SUCCESS_F,
	UI_FAIL_F,
	UI_NET_CONNECTED_F,
	UI_NET_DISCONNECTED_F,
#endif//feature_vas
#ifdef FEATURE_WAP
    /*========================================================================
    ** ytchoi 01/07/16
	** AU4.3 Event  Now according to #define, enum name may conflict!!
    */ 
    UI_WAP_REPORT_ERROR_F,
	UI_MUXNEGO_SUCCESS_F,
	WAP_EXIT_CMD,
	WAP_CONNECTION_DONE_CMD,    
    UI_WAP_SO_REQ_F,
#endif //#ifdef FEATURE_WAP



#ifdef FEATURE_WAVELET // yoursun 01/06/08
  UI_WAV_PLAY_CMD_F,
  UI_WAV_STREAM_CMD_F,
#endif //#ifdef FEATURE_WAVELET

#ifdef FEATURE_MULTI_SID // hjcho 01.09.17
  UI_MULTI_SID_F,
#endif //FEATURE_MULTI_SID

  UI_NUM_UI_COMMANDS,              /* End of command list (note no comma!)    */
  UI_END_PARTIAL_CMD		//asdf 2001.09.18 partial off시 annunciator를 그리는데 사용 
} ui_name_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef ui_name_type    UI_COMMAND_E;

#ifdef FEATURE_ANALOG_DATA
#error code not present
#endif

/* Statuses from the User Interface task, in command buffer */

typedef enum
{
  UI_DONE_S,                      /* General OK return */
  UI_BUSY_S,                      /* Command is being processed */
  UI_BADCMD_S,                    /* Invalid command */
  UI_BADPARM_S                    /* Bad Parameters */
} ui_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Command header type.  Each command to UI starts with this header.       */

/* These are the common fields (base class) that all command headers to UI  */
/* must !start! with.  They are defined as a macro to insure that all       */
/* headers contain the same set.                                            */


#define UI_CMD_HDR_COMMON_FIELDS                                             \
  q_link_type        link;         /* Queue link */                          \
  ui_name_type       cmd;          /* Command to execute */                  \
  rex_tcb_type       *task_ptr;    /* Pointer to requesting task TCB */      \
  rex_sigs_type      sigs;         /* Signal to set upon cmd completion */   \
  ui_status_type     status;       /* Completion code status */              \
  q_type             *done_q_ptr   /* Queue to place this cmd on when done */

/* Generic command header type - all cmds to UI must start with this or one */
/* of its supersets                                                         */
typedef struct {
  UI_CMD_HDR_COMMON_FIELDS;        /* All ui headers must !start! with this */
} ui_hdr_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DS

 /* Type for UI_DATACALL_F or UI_DS_ORIG_VCALL_F - Analog or CDMA */

typedef struct {
  ui_hdr_type hdr;                    /* header */
  boolean     dig_mode;               /* TRUE if CDMA data call, else AMPS */
  word        service_option;         /* service option of data call */
  byte        cnt;                                      /* How many digits */
  byte        called_address[MC_MAX_DIALED_DIGITS];     /* Called address  */
} ui_dscall_type;
#endif

typedef struct {            /* white 2001.03.20 */
  ui_hdr_type hdr;                  /* header */
  byte mode;        // 0x00: dtr off, 0x01:모뎀착신모드, 0x02:FAX 착신모드
} ui_dtr_display_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_DS

 /* Type for UI_DS_ANSWER_F - Analog or CDMA */

typedef struct {
  ui_hdr_type hdr;                   /* header */
  boolean     amps_data;             /* TRUE if AMPS data call, else FALSE */
} ui_dsanswer_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for UI_DIAG_ORIG_F */

typedef struct {
  ui_hdr_type  hdr ;
  byte         cnt ;      /* number of valid digits */
  byte         call_digits[ UI_MAX_DIALED_DIGITS ] ;  /* number to dial */
  word         so ;      /* CAI service options */
} ui_diag_orig_type ;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for UI_SMS_F */
#ifdef FEATURE_UASMS 
#ifdef NEW_SMS
typedef struct
{
  q_link_type link;                       /* header */
  smsi_msg_type data;                     /* SMS message area */
} ui_sms_data_type;

typedef struct
{
  ui_hdr_type hdr;                        /* header           */
  ui_sms_data_type *buf_ptr;              /* ptr to SMS message data */
  uasms_transaction_id_type    tid;

} ui_sms_type;

#else
typedef struct
{
  ui_hdr_type hdr;                        /* header           */
  smsi_msg_type data;                     /* SMS message data */
} ui_sms_type;

#endif /* NEW_SMS */

#else
typedef struct
{
  ui_hdr_type hdr;                        /* header           */
} ui_sms_type;
#endif /* FEATURE_UASMS */

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
typedef struct
{
  ui_hdr_type                hdr;      /* header */
  uasms_cmd_id_e_type        cmd;      /* which call command */
  uasms_cmd_status_e_type    cmd_err;  /* what error */
} ui_bcsms_cmd_err_type;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for UI_SMS_STATUS_F */

typedef struct
{
  ui_hdr_type hdr;                        /* header           */

#ifdef FEATURE_UASMS
  uasms_transaction_id_type    tid;
  uasms_error_class_e_type     error_class;
  uasms_status_e_type          status;
#endif /* FEATURE_UASMS */

} ui_sms_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for UI_DATACALL_F */

typedef struct {
  ui_hdr_type hdr;                                    /* header */
  byte         cnt;                                   /* How many digits */
  byte         called_address[MC_MAX_DIALED_DIGITS];  /* Called address */
} ui_datacall_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NV_WRITE_ONLINE

/* Type for UI_NV_WRITE_ONLINE_F */

typedef struct {
  ui_hdr_type         hdr;                      /* header       */
  nv_items_enum_type  item;                     /* Which item   */
  nv_item_type        item_data;                /* Parameters   */
} ui_nv_write_online_type;

#endif /* #ifdef FEATURE_NV_WRITE_ONLINE */

#ifdef FEATURE_SMS_PACKET

/* Type for UI_GET_SMS_F */

/* This is just a command to read the latest SMS message        */
/* So, do not need to provide any information other than        */
/* the header, with contains the command                        */

typedef struct {
  ui_hdr_type         hdr;                      /* header       */

} ui_sms_read_type;

/* This is for a command to erase certain SMS message(s).       */
/* There are certain SMS messages that cannot be erased         */
/* by itself                                                    */

/* Type for UI_DELETE_SMS_F */

typedef struct {
  ui_hdr_type             hdr;          /* header               */
  mod_sms_erase_code_type erase_code;   /* which SMS message(s) */
} ui_sms_delete_type;


#endif /* FEATURE_SMS_PACKET */

#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif /* FEATURE_AUDIO_CONTROL */


#ifdef FEATURE_ANALOG_DATA
#error code not present
#endif

 
typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_call_info_s_type        call_info;/* call information */
} ui_call_event_type;


#ifndef FEATURE_UASMS
typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_sms_info_s_type         sms_info; /* SMS info */
} ui_sms_event_type;
#endif /* FEATURE_UASMS */

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
typedef struct
{
  ui_hdr_type                hdr;      /* header */
  uasms_event_info_type      bcsms_info;/* BC SMS information */
} ui_bcsms_event_type;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_ph_info_s_type          ph_info;  /* phone information */
} ui_ph_event_type;


typedef struct
{
  ui_hdr_type                hdr;          /* header */
  cm_inband_info_s_type      inband_info;  /* inband information */
} ui_inband_event_type;

typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_ss_info_s_type          ss_info;  /* serving system information */
} ui_ss_event_type;

typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_client_id_type          client;   /* which client */
  cm_call_cmd_e_type         cmd;      /* which call command */
  cm_call_cmd_err_e_type     cmd_err;  /* what error */
} ui_call_cmd_err_type;

typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_client_id_type          client;   /* which client */
  cm_ph_cmd_e_type           cmd;      /* which phone command */
  cm_ph_cmd_err_e_type       cmd_err;  /* what error */
} ui_ph_cmd_err_type;


typedef struct
{
  ui_hdr_type                hdr;      /* header */
  cm_client_id_type          client;   /* which client */
  cm_inband_cmd_e_type       cmd;      /* which phone command */
  cm_inband_cmd_err_e_type   cmd_err;  /* what error */
} ui_inband_cmd_err_type;

typedef struct
{
  ui_hdr_type                hdr;      /* header */
  boolean					 mode;
} ui_diag_keypad_test_type;

/* FEATURE_WAP */
#ifdef WAPPER
typedef struct {
  ui_hdr_type             hdr;         /* header                 */
  int				      number;	   /* defined in errortype.h */
  int				      type;		   /* defined in errortype.h */
} ui_wap_error_type;
#endif //WAPPER

#ifdef FEATURE_VAS
/*
** key input control
*/
typedef struct {
  ui_hdr_type             hdr;
  unsigned char           KeyCode;
} ui_key_input_type;

/*
** Go to the specific URL
*/
typedef struct {
  ui_hdr_type             hdr;
  unsigned char	    	  GotoUrl[100];
} ui_gotourl_type;

/*
** wap command
*/
typedef struct {
  ui_hdr_type             hdr;
  unsigned char	    	  objectID;
  unsigned char           channelID;
  unsigned char           value;
  boolean                 result;
} ui_wap_cmd_type;

#endif//feature_vas

#ifdef FEATURE_MCOMMERCE
typedef struct {
  ui_hdr_type             hdr;
  int	    			  script;
  boolean                 append_info;
  int	    			  sign_info;
  int	    			  encrypt_info;
} ui_wap_script_type;

typedef struct {
  ui_hdr_type             hdr;
  int	    			  type;
} ui_wap_cert_mime_type;
#endif //#ifdef FEATURE_MCOMMERCE

/*-------------------------------------------------------------------------*/

/* Union of all command types.  The header is always present and it        */
/* specifies the command type and attributes.  If the command has          */
/* arguments they follow in the union.                                     */

typedef union ui_cmd_u {
    ui_hdr_type                   hdr;
#ifdef FEATURE_DS
    ui_dscall_type                dscall;
    ui_dsanswer_type              dsanswer;
#endif
    ui_sms_type                   sms;
    ui_sms_status_type            sms_status;
    /* reo  2001-01-05 --> @why, what */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)
    ui_sms_status_type            sms_mo_status;   
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
    /* <-- */
    ui_datacall_type              datacall;

    ui_diag_orig_type             diag_orig;

#ifdef FEATURE_NV_WRITE_ONLINE
    ui_nv_write_online_type       nv_write_online;
#endif 
#ifdef FEATURE_SMS_PACKET
    ui_sms_read_type              sms_read;
    ui_sms_delete_type            sms_delete;
#endif 
#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif 
#ifdef FEATURE_ANALOG_DATA
#error code not present
#endif 
    ui_call_event_type            call_event;
    ui_ph_event_type              ph_event;
    ui_inband_event_type          inband_event;
#ifndef FEATURE_UASMS
    ui_sms_event_type             sms_event;
#endif /* FEATURE_UASMS */
    ui_ss_event_type              ss_event;

    ui_call_cmd_err_type          call_cmd_err;
    ui_ph_cmd_err_type            ph_cmd_err;
    ui_inband_cmd_err_type        inband_cmd_err;
#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
    ui_bcsms_cmd_err_type         bcsms_cmd_err;
    ui_bcsms_event_type           bcsms_event;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */
	ui_diag_keypad_test_type	  diagkeytest;
#ifdef WAPPER
	ui_wap_error_type         wap_error;
#endif //WAPPER
    ui_dtr_display_type		dtr_disp;       /* white 2001.03.20 */
#ifdef FEATURE_VAS
	ui_key_input_type		KeyInput;	
	ui_gotourl_type         GotoUrl;
	ui_wap_cmd_type         Command;
#endif//featur_vas
	ui_wap_script_type			   wml;
	ui_wap_cert_mime_type		   cert;
#ifdef FEATURE_MCOMMERCE
#endif //#ifdef FEATURE_MCOMMERCE

} ui_cmd_type;

typedef ui_cmd_type    UI_COMMAND_U;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
#define MAX_INFO_MESSAGE_LENGTH     (200) /* should be enough */
struct tagINFO_MESSAGE_T
{
    QUEUE_LINK_T Link;
    BYTE szText[MAX_INFO_MESSAGE_LENGTH+1];
};
typedef struct tagINFO_MESSAGE_T        INFO_MESSAGE_T;

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/
extern q_type ui_cmd_free_q;      /* free buffers for talking to UI */
#ifdef NEW_SMS
extern q_type ui_sms_free_q;      /* free buffers for sms data to UI */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#endif

extern BOOL   g_fOK2Sleep;
//extern DWORD  dwSleepFlag;        /* flag for checking ok to sleep   */

#ifdef SW_RESET_ENABLE
extern BOOL    mc_bSwReset;
extern DWORD   g_savedTime;
#endif

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void  SKY_API SKY_AddEventAtTail(SKY_EVENT_E Event);
extern void  SKY_API SKY_AddEvent(SKY_EVENT_E Event);
extern void  SKY_API SKY_ClearAllEvent(void);
extern void  SKY_API SKY_AddPriorityEvent(SKY_EVENT_E Event);
extern void  SKY_API SKY_KickDog(void);
extern void  SKY_API SKY_DoEvent(SKY_EVENT_E SkyEventID);
extern DWORD SKY_API SKY_GetLocalTime(void);
extern void  SKY_API SKY_PutNewInfoMessage(UI_COMMAND_E    Command,
                                           UINT8           nLength,
                                           BYTE            *pText);
extern void SKY_API SKY_AcceptKey(BOOL fAccept);
extern BOOL SKY_API SKY_ReinitMemory(void);
extern BOOL SKY_API SKY_IsKeyBufferEmpty(void);


/*===========================================================================
FUNCTION UI_TASK

DESCRIPTION
  The main user interface function.  This is the big loop outside of the
  state machine which handles signals and all sorts of inputs.  It mostly
  serves to convert inputs into events for the state machine, but it
  also handles keypad incidentals - backlighting and key beeps.

DEPENDENCIES
  REX is running.  NV and DIAG and HS are running.

RETURN VALUE
  Never exits.

SIDE EFFECTS
  Plays with variable ui.

===========================================================================*/
extern void ui_task (
  dword dummy
);

/*===========================================================================

FUNCTION UI_CMD

DESCRIPTION
  Allows other tasks to queue up messages to the UI.  Grab a buffer from the
  ui_cmd_free_q, fill it in, and call ui_cmd with it.  All this will
  do is queue it and set a signal for the UI task.  When the command
  is done, the buffer will be queued as directed in the command header,
  and a task you specify may be signaled.

DEPENDENCIES
  ui_cmd_q already initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void ui_cmd
(
  ui_cmd_type *cmd_ptr            /* command block to UI */
);

#ifdef FEATURE_VAS
/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
extern void send_ui_cmd(ui_name_type cmd);
extern void send_ui_cmd_id(ui_name_type  cmd, 
						   unsigned char objID, 
						   unsigned char chID,  
						   unsigned char value);
extern void wap_key_cmd(INT8 key_value);
extern void wap_gotourl_cmd(UINT8 *gotourl);
extern void wap_enter_cmd(void);
extern void wap_end_cmd(void);
extern void wap_exit_cmd(void);
extern void wap_connection_done_cmd(unsigned char objId, 
									unsigned char chID, 
									unsigned char b_success);
//for test
extern void vas_pppopen_test_cmd(void);
extern void vas_pppclose_test_cmd(void);
extern void vas_udptxrx_cmd(void);
extern void vas_tcptxrx_cmd(void);
extern void vas_txsend_cmd(void);
extern void vas_rxdata_cmd(void);
extern void vas_multisocket_cmd(void);
//for test

extern void ProcessNetworkCallback(void);
extern void ProcessSocketCallback(void);
extern void UI_ProcessEvents(void);
extern void app_network_callback(void *p);
extern void app_socket_callback(void *p);
extern void app_dns_callback(void);

extern boolean ui_wait_sig(long int timer_cnt, REX_SIGNAL wait_sig);

#endif//feature_vas

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_UI_H_ */


