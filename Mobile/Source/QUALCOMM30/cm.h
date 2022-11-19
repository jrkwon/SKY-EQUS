#ifndef CM_H
#define CM_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

              C A L L   M A N A G E R   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary for
  clients to interface with the Call Manager module.

Copyright (c) 1991 - 2001 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cm.h_v   1.13   28 Feb 2001 17:59:58   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/01   HQ      Fixed compiler warnings.
02/26/01   sko     Removed reported FER and FER events.
                   Chaned data type of tx_power and deleted unused type.
02/07/01   RC      Added support for System Determination 2.0.
01/29/01   HQ      Properly feature-ized NEWCM, UASMS and GPSONE_DBM.
01/23/01   ych     Merged T53 and JCDMA features.
12/28/00   fc      Added support for retry order.
12/15/00   HQ      Added CM_CALL_CMD_PRIVACY_PREF, CM_CALL_EVENT_PRIVACY_PREF
                   and CM_CALL_CMD_ERR_BAD_PRIVACY_PREF.
10/06/00   HD      Added support to pass the DRS bit setting to CP.
10/09/00   HQ      Added call end status RUIM_NOT_PRESENT and call command
                   error RUIM_NOT_PRESENT for RUIM support.
09/15/00   PJ      Added support for PD commands (FEATURE_GPSONE_DBM)
08/23/00   vt      Added new CM interface changes.
07/17/00   jq      Added cm_call_cmd_end_with_reason 
03/27/00   HD      Added support for MODE_OFFLINE_ANALOG
03/16/00   VT      Added BROADCAST SMS support
03/13/00   RY      Added OTAPA support
01/24/00   VT      Undid the last change.
01/23/00   CS      Defined CM_CALL_END_MAX_ACCESS_PROBE for all builds.
01/11/00   VT      Added cm_is_valid_mode_pref().
12/03/99   VT      Merged TriMode Code.
09/10/99   SH      Removed CM_SMS_EVENT_N_MSG and CM_SMS_EVENT_N_CLI
09/09/99   SH      Added band, p_rev and regitration related fields for serving system
09/01/99   SH      Featurized OTASP
08/30/99   SH      Added CM_INBAND_CMD_ERR_OFFLINE_S
08/19/99   SH      Added MC abort for call end status
07/28/99   SH      Added SMS object
07/27/99   SH      Moved link in cm_client_s to be the first element
07/07/99   SH      Added CNAP feature
06/30/99   SH      Changed type of activate_code_ptr to cm_activate_code_type *
06/28/99   SH      Used client id for client related registration functions
06/28/99   SH      Added activate_code to the origination request
06/21/99   SH      Removed cm_async_request_status_e_type
05/31/99   SH      Added CM 2.0 features
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
12/08/98   RC      Changes per second week of I&T.
11/24/98   RC      Initial release.

===========================================================================*/


#include "customer.h"  /* Customer configuration file */
#include "comdef.h"    /* Definition for basic types and macros */

#ifndef FEATURE_NEWCM
#include "queue.h"     /* Interface to queue services */
#include "nv.h"        /* Interface to NV services */
#include "mc.h"        /* Interface to MC Task */
#include "db.h"        /* Interface to Database services */
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */
#endif

#ifdef FEATURE_SD20
#error code not present
#endif

/* <EJECT> */
/*===========================================================================
=============================================================================
========================  DEFINES ===========================================
=============================================================================
===========================================================================*/

#ifndef FEATURE_NEWCM
#define CM_CS_TIMER_SIG     0x0001  /* CM clock service timer signal - not used */
#define CM_RPT_TIMER_SIG    0x0002  /* Time to kick watchdog */
#define CM_TIMER_SIG        0x0004  /* Time to update CM */
#define CM_RSSI_TIMER_SIG   0x0008  /* Time to check and update RSSI */
#define CM_CMD_Q_SIG        0x0010  /* Reserved - not used */
#define CM_MC_RPT_Q_SIG     0x0020  /* CM receiving mc cmd signal */
#define CM_CLIENT_CMD_Q_SIG 0x0040  /* CM client command signal */
#endif /* FEATURE_NEWCM */

#define CM_NV_SIG           0x0080  /* CM NV done signal */
#define CM_RUIM_STATUS_SIG  0x0100  /* CM RUIM Done signal */

#ifndef FEATURE_NEWCM
#define CM_TIME             1000    /* Time to update CM */
#define CM_RSSI_TIME        2240    /* 320*7 ms */
//#define CM_RPT_TIME         25

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

typedef q_type   cm_cmd_q_type;

extern cm_cmd_q_type    cm_mc_rpt_free_q;
    /* Queue to manage MC report pool */

extern rex_timer_type  cm_rssi_timer;
    /* RSSI checking timer */

/*===========================================================================
FUNCTION CM_TASK

DESCRIPTION
  The main call manager function.  This is the big loop outside of the
  state machine which handles signals and all sorts of inputs.

DEPENDENCIES
  REX is running.

RETURN VALUE
  Never exits.

SIDE EFFECTS

===========================================================================*/
extern void cm_task (
  dword dummy
);

#endif /* FEATURE_NEWCM */


/* <EJECT> */
/*===========================================================================
=============================================================================
=========================== CLIENT INTERFACE BLOCK ==========================
======================== PUBLIC TYPES, DATA & FUNCTIONS =====================
=============================================================================
===========================================================================*/


#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
/* Type for init mask variable.
*/
typedef byte cm_init_mask;



/* Client ID type to differentiate CM client objects from one another.
*/
typedef struct cm_client_s  *cm_client_id_type;
#endif /* FEATURE_NEWCM */

/* Anonymous client ID to be used by clients that are not interested
** in being notified of CM events and therefore do not allocate a
** an object of type cm_client_s_type in their own program space.
*/
#define CM_CLIENT_ID_ANONYMOUS  (cm_client_id_type) (-1)

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */
/* <EJECT> */

#ifdef FEATURE_IS2000
/*---------------------------------------------------------------------------
                             RETRY ORDER TYPES
---------------------------------------------------------------------------*/

/* Retry delay type */
typedef enum
{
  CM_RETRY_TYPE_NONE = -1,    /* No retry order                           */ 
  CM_RETRY_TYPE_CLEAR_ALL,    /* Clear all retry order                    */
  CM_RETRY_TYPE_ORIGINATION,  /* Origination retry order                  */
  CM_RETRY_TYPE_RESOURCE_REQ, /* Resource request retry order             */
  CM_RETRY_TYPE_SUPP_CH_REQ,  /* Supplemental channel request retry order */ 
  CM_RETRY_TYPE_RESERVED4,
  CM_RETRY_TYPE_RESERVED5,
  CM_RETRY_TYPE_RESERVED6,
  CM_RETRY_TYPE_RESERVED7,
  CM_RETRY_TYPE_MAX

} cm_retry_type_e_type;

/* Retry order info */
typedef struct
{
  cm_retry_type_e_type retry_type;     /* Retry delay type */
  word                 srv_opt;        /* Service option   */
  boolean              infinite_delay; 
    /* True if infinite delay, it should always be FALSE for origination */  
  uint32               retry_delay_in_ms;
    /* Only meaningful if infinite_delay is FALSE */
} cm_retry_order_type;
#endif /* FEATURE_IS2000 */

/*---------------------------------------------------------------------------
                             CALL RELATED TYPES
---------------------------------------------------------------------------*/


/* Tunable call parameters.
*/




#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
#define CM_MAX_DIALED_DIGITS      NV_MAX_DIAL_DIGITS
#endif    
    /* Max length of dialed digit string */

#define CM_MAX_CALLER_ID_CHARS    32
    /* Max length of Caller ID string */

#define CM_MAX_CALLED_INFO_CHARS  64   /*  CM_MAX_DATA_DIGITS*/
    /* Max length of Called info string */


#define CM_MAX_CONNECTED_NUM_CHARS  64  /* CM_MAX_DATA_DIGITS */
   /* Max length of Connected num string */


#define CM_MAX_NUMBER_CHARS      CM_MAX_CALLED_INFO_CHARS
    /* Max length of CM number string  This is the Maximum
       of CM_MAX_DIALED_DIGITS, CM_MAX_CALLER_ID_CHARS
       CM_MAX_CALLED_INFO_CHARS and CM_MAX_CALLED_INFO_CHARS */


#define CM_MAX_ALPHA_LEN_FOR_CALL_ORIG      12
#define CM_MAX_ALPHA_LEN_FOR_DISPLAY_INFO   64  /* CM_MAX_DATA_DIGITS */

#define CM_MAX_ALPHA_TAG_CHARS    CM_MAX_ALPHA_LEN_FOR_DISPLAY_INFO
    /* Max length of CM alpha tag string. This is the maximum of
       CM_MAX_ALPHA_LEN_FOR_CALL_ORIG and
       CM_MAX_ALPHA_LEN_FOR_DISPLAY_INFO  */

#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
#ifdef FEATURE_OTASP
typedef  mc_activate_code_type   cm_activate_code_type;
#define CM_OTASP_ACT_CODE_NONE   ( (cm_activate_code_type) ((int)PREFERRED+1) )

#else /* FEATURE_OTASP */

typedef enum {
 CM_OTASP_ACT_CODE_NONE
} cm_activate_code_type;

#endif /* FEATURE_OTASP */
#endif  /* FEATURE_NEWCM */



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of call states. At all times a CM call must be in one
** of the states below.
*/
typedef enum cm_call_state_e {

    CM_CALL_STATE_NONE=-1,  /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_STATE_IDLE,     /* Call is in idle state - i.e. no call */
    CM_CALL_STATE_ORIG,     /* Call is in origination state */
    CM_CALL_STATE_INCOM,    /* Call is in alerting state */
    CM_CALL_STATE_CONV,     /* Call is in conversation state */

    CM_CALL_STATE_MAX       /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_state_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of CM call types.
**
** When originating or answering a call, clients need to specify
** one of these call types.
*/
typedef enum cm_call_type_e {

    /* Call types for originating and answering calls.
    */

    CM_CALL_TYPE_NONE=-1,
        /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_TYPE_VOICE,
        /* Voice call type.
        **
        ** NOTE! Answering with this type is only successful if the
        ** incoming call is of type VOICE */

    CM_CALL_TYPE_DATA,
        /* Data call type - (for modem, fax, packet, etc. calls).
        **
        ** NOTE! Answering with this call is only successful if the
        ** incoming call is of type DATA or VOICE (with AMPS service) */

    CM_CALL_TYPE_SMS,
        /* SMS call type (SMS SO-6 and SO-14)
        **
        ** NOTE! Answering with this call type is only successful
        ** if incoming call is of type SMS */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

    CM_CALL_TYPE_TEST,
        /* Test call type (for Markov, Loopback, etc.)
        **
        ** NOTE! Answering with this call type is only successful
        ** if incoming call is of type TEST */

    CM_CALL_TYPE_OTAPA,
        /* OTAPA call type (OTAPA SO-18 and SO-19)
        **
        ** NOTE! Answering with this call type is only successful
        ** if incoming call is of type OTAPA */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Call types that are automatically assigned by call manager when
    ** detecting an origination dial-string that matches one of the
    ** special numbers that are stored in the phone-book.
    **
    ** NOTE! This call types should not be used to originate
    ** or answer calls.
    */

    CM_CALL_TYPE_STD_OTASP,
        /* Standard OTASP call type.
        **
        ** This call type is automatically assigned by call manager when
        ** detecting an origination dial-string that matches one of the
        ** standard OTASP numbers that are stored in the phone-book.
        **
        ** NOTE! This call type should not be used to originate
        ** or answer calls */

    CM_CALL_TYPE_NON_STD_OTASP,
        /* None standard OTASP call type.
        **
        ** This call type is automatically assigned by call manager when
        ** detecting an origination dial-string that matches one of the none
        ** standard OTASP numbers that are stored in the phone-book.
        **
        ** NOTE! This call type should not be used to originate
        ** or answer calls */

    CM_CALL_TYPE_E911,
        /* Emergency call type.
        **
        ** This call type is automatically assigned by call manager when
        ** detecting an origination dial-string that matches one of the
        ** E911 numbers that are stored in the phone-book.
        **
        ** NOTE! this call type should not be used to originate
        ** or answer calls
        **
        ** NOTE! When call manager assigned this call type, the mode
        ** preference is forced to E911 before origination. If at the
        ** end of the call the phone is not in E911 callback mode,
        ** the mode is automatically restored to its original selection.
        ** Else the E911 mode stays in effect until changed by a client
        ** or power-cycle */

    CM_CALL_TYPE_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_type_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to decide whether a given call type is an OTASP
** (standard or none standard ).
*/
#define CM_CALL_TYPE_IS_OTASP( call_type )  \
              ((call_type) == CM_CALL_TYPE_STD_OTASP    || \
                                (call_type) == CM_CALL_TYPE_NON_STD_OTASP   )

#define CM_CALL_TYPE_IS_OTAPA( call_type )  \
              ((call_type) == CM_CALL_TYPE_OTAPA)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of service types.
**
** When originating a call, clients need to specify
** one of these service types.
*/
typedef enum cm_srv_type_e {

    CM_SRV_TYPE_NONE,
        /* FOR INTERNAL USE OF CM ONLY! */

    CM_SRV_TYPE_AMPS,
        /* Call is to be connected over analog service.
        **
        ** Note that the client does NOT need to specify service option when
        ** originating with this call type.
        **
        ** NOTE! When originating with this service type, the mode preference
        ** might be forced to ANALOG ONLY before origination and returned to
        ** its original setting at the end of the call */

    CM_SRV_TYPE_CDMA_SPECIFIC,
        /* Call is to be connected over CDMA service with specific client
        ** designated CDMA service option.
        **
        ** Note that the client need to specify service option when
        ** originating with this call type.
        **
        ** NOTE! When originating with this service type, the mode preference
        ** might be forced to DIGITAL ONLY before origination and returned to
        ** its original setting at the end of the call */

    CM_SRV_TYPE_CDMA_AUTOMATIC,
        /* Call is to be connected over CDMA service where CM automatically
        ** originate with the best available service option for the specified
        ** call type.
        **
        ** Note that the client does NOT need to specify service option when
        ** originating with this call type */

    CM_SRV_TYPE_AUTOMATIC,
        /* Call is to be connected over available service - AMPS or CDMA
        ** with best available CDMA service option.
        **
        ** Note that the client does NOT need to specify service option when
        ** originating with this call type */

    CM_SRV_TYPE_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_srv_type_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of call end statuses
** - i.e. the reason for ending a call.
*/
typedef enum cm_call_end_e {

    CM_CALL_END_NONE=-1,            /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_END_OFFLINE,            /* phone is offline */
    CM_CALL_END_CDMA_LOCK,          /* phone is CDMA locked until power cycle */
    CM_CALL_END_NO_SRV,             /* phone has no service */
    CM_CALL_END_FADE,               /* Call has ended abnormally */
    CM_CALL_END_INTERCEPT,          /* received intercept from BS - orig only */
    CM_CALL_END_REORDER,            /* received reorder from BS - orig only */
    CM_CALL_END_REL_NORMAL,         /* received release from BS - no reason given */
    CM_CALL_END_REL_SO_REJ,         /* received release from BS - SO reject */
    CM_CALL_END_INCOM_CALL,         /* received incoming call from BS */
    CM_CALL_END_ALERT_STOP,         /* received alert stop from BS - incom only */
    CM_CALL_END_CLIENT_END,         /* client ended the call */
    CM_CALL_END_ACTIVATION,         /* received end activation - OTASP call only */
    CM_CALL_END_MC_ABORT,           /* MC aborts the origination/conversation*/
    
    /* Obsolete in SD 2.0 */
    CM_CALL_END_NDSS_FAIL,          /* NDSS failure */
    CM_CALL_END_MAX_ACCESS_PROBE,   /* Max access probes transmitted */
    
    /* JCDMA */
    CM_CALL_END_PSIST_NG,           /* Persistence test failure */
    
    /* RUIM */
    CM_CALL_END_RUIM_NOT_PRESENT,   /* RUIM is not present */ 
    
    /* New for SD 2.0 */
    CM_CALL_END_ACC_IN_PROG,        /* Access attempt already in progress */
    CM_CALL_END_ACC_FAIL,           /* Access failure for reason other than
                                    ** the above */
    /* IS-2000 */
    CM_CALL_END_RETRY_ORDER,        /* Received retry order - orig only */

    CM_CALL_END_MAX                 /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_end_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of digit modes - see J-STD-008 2.7.1.3.2.4.
*/
typedef enum cm_digit_mode_e {

    CM_DIGIT_MODE_NONE=-1,      /* FOR INTERNAL USE OF CM ONLY! */

    CM_DIGIT_MODE_4BIT_DTMF,    /* 4-bit DTMF */
    CM_DIGIT_MODE_8BIT_ASCII,   /* 8-bit ASCII */

    CM_DIGIT_MODE_MAX           /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_digit_mode_e_type;


/* Privacy mode during a call
*/
typedef enum
{
  CM_PRIVACY_PREF_NONE = -1,
  CM_PRIVACY_PREF_STANDARD,
  CM_PRIVACY_PREF_ENHANCED,
  CM_PRIVACY_PREF_MAX
} cm_privacy_pref_e_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of call commands client are able to send to CM.
*/
typedef enum cm_call_cmd_e {

    CM_CALL_CMD_NONE=-1,       /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_CMD_ORIG,          /* Originate a call */
    CM_CALL_CMD_ANSWER,        /* Answer an incoming call */
    CM_CALL_CMD_END,           /* End a call in orig, incom or conv state */
    CM_CALL_CMD_FLASH,         /* Send a flash with information */
    CM_CALL_CMD_PRIVACY_PREF,  /* Change privacy pref */
    CM_CALL_CMD_INFO_GET,      /* Get a sanpshot of the call information in the CM */
#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

    CM_CALL_CMD_MAX            /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_cmd_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of call events.
** Clients are able to register with CM to be notified of any subsets
** of these events.
*/
typedef enum cm_call_event_e {

    CM_CALL_EVENT_NONE=-1,    /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_EVENT_ORIG,       /* phone originated a call */
    CM_CALL_EVENT_ANSWER,     /* Incoming call was answered */
    CM_CALL_EVENT_END,        /* Originated/incoming call was ended */
    CM_CALL_EVENT_FLASH,      /* phone sent Flash / Flash with Info to BS */

    CM_CALL_EVENT_INCOM,      /* phone received an incoming call */
    CM_CALL_EVENT_CONNECT,    /* Originated/incoming call was connected */
    CM_CALL_EVENT_SRV_OPT,    /* Service option changed while in a call */
    CM_CALL_EVENT_PRIVACY,    /* Privacy mode changed while in a call */
    CM_CALL_EVENT_PRIVACY_PREF, /* Privacy mode preference changed */
    CM_CALL_EVENT_CALLER_ID,  /* Caller ID info was received from BS */

    /* Events added for CM 2.0 */
    CM_CALL_EVENT_ABRV_ALERT,     /* CMDA/AMPS abbreviated alert */
    CM_CALL_EVENT_ABRV_REORDER,   /* AMPS abbreviated reorder */
    CM_CALL_EVENT_ABRV_INTERCEPT, /* AMPS abbreviated intercept */
    CM_CALL_EVENT_SIGNAL,         /* signal info was received from BS */
    CM_CALL_EVENT_DISPLAY,        /* display info was received from BS */
    CM_CALL_EVENT_CALLED_PARTY,   /* called party info was received from BS */
    CM_CALL_EVENT_CONNECTED_NUM,  /* connected number info was received from BS */
    CM_CALL_EVENT_INFO,           /* call information, this event only sent to the client
                                     that request this information through
                                     cm_call_cmd_get_call_info */
    /* IS-95B */
    CM_CALL_EVENT_EXT_DISP,       /* extended display was received from BS */
    CM_CALL_EVENT_NDSS_START,     /* Start NDSS redirection */
    CM_CALL_EVENT_NDSS_CONNECT,   /* call was re-connected due to NDSS */

    /* JCDMA */
    CM_CALL_EVENT_EXT_BRST_INTL,  /* Extended Burst Type - International */
    CM_CALL_EVENT_NSS_CLIR_REC,   /* National Supplementary services - CLIR */
    CM_CALL_EVENT_NSS_REL_REC,    /* National Supplementary services - Release */
    CM_CALL_EVENT_NSS_AUD_CTRL,   /* National Supplementary services - Audio Control */
    CM_CALL_EVENT_L2ACK_CALL_HOLD,/* Call Hold */

    CM_CALL_EVENT_MAX         /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_event_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of call command errors returned by call command
** callback functions.
*/
typedef enum cm_call_cmd_err_e {

    CM_CALL_CMD_ERR_NONE=-1,
        /* FOR INTERNAL USE OF CM ONLY! */


    CM_CALL_CMD_ERR_NOERR,
        /* No errors found */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients feeding a call command
    ** function with bad or inappropriate parameters.
    */

    CM_CALL_CMD_ERR_CLIENT_ID_P,
        /* invalid client ID - null or not initialized*/

    CM_CALL_CMD_ERR_CALL_TYPE_P,
        /* Bad call type parameter */

    CM_CALL_CMD_ERR_SRV_TYPE_P,
        /* Bad service type parameter */

    CM_CALL_CMD_ERR_SRV_OPT_P,
        /* Bad service option parameter */

    CM_CALL_CMD_ERR_NO_NUM_P,
        /* Expecting a number to be provided via the number parameter */

    CM_CALL_CMD_ERR_NULL_NUM_P,
        /* Number length is > 0, but number buffer points to NULL */

    CM_CALL_CMD_ERR_NUM_P,
        /* Invalid digits found in number buffer */

    CM_CALL_CMD_ERR_NUM_LEN_P,
        /* Out of range value found in number length field */

    CM_CALL_CMD_ERR_NULL_ALPHA_P,
        /* Alpha length is > 0, but alpha buffer points to NULL */

    CM_CALL_CMD_ERR_ALPHA_LEN_P,                                     /* 10 */
        /* Out of range value found in alpha length field */

#ifdef FEATURE_OTASP
    CM_CALL_CMD_ERR_ACTIVATE_CODE_P,
        /* Invalid OTASP activation code */
#endif /* FEATURE_OTASP */
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients trying to instruct
    ** CM to perform inappropriate actions relative to the current
    ** state of the call and/or phone.
    */

    CM_CALL_CMD_ERR_OFFLINE_S,
        /* Phone is offline */

    CM_CALL_CMD_ERR_CDMA_LOCK_S,
        /* Phone is CDMA locked */

    CM_CALL_CMD_ERR_FLASH_S,
        /* Client tried to send a flash while a call that not supporting
        ** flashes is in progress */

    CM_CALL_CMD_ERR_ORIG_RES_S,
        /* Dialed number is not allowed under the current origination
        ** restriction */

    CM_CALL_CMD_ERR_E911_MODE_S,
        /* Client tried to originate a NONE E911 call while E911 mode
        ** preference is in effect */

    CM_CALL_CMD_ERR_IN_USE_S,
        /* Can't perform the command in question while phone is in-use */

    CM_CALL_CMD_ERR_SRV_TYPE_S,
        /* Client originated with service type that is currently not
        ** supported by the phone/PRL */

    CM_CALL_CMD_ERR_CALL_TYPE_S,
        /* Client answered with inappropriate call type relative to
        ** to the type of the incoming call */

    CM_CALL_CMD_ERR_CALL_STATE_S,
        /* The command in question is invalid in the current call state -
        ** e.g. answering a call that was already answered,
        ** ending a call that was already ended, etc. */
    
     CM_CALL_CMD_ERR_SRV_STATE_S,
        /* The command in question is invalid in the current Phone serving state -
           e.g answering a call but serving state is no service
         */    
    
    CM_CALL_CMD_ERR_NO_SO_S,
        /* no valid service found for origination with AUTOMATIC 
           or CDMA_AUTOMATIC srv type  */
    
    #ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
    
    CM_CALL_CMD_ERR_ALERT_ANS_BYPASS_S,
        /* Client is not allowed to answer the incoming call */
    #endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */   

#ifdef FEATURE_RUIM
    CM_CALL_CMD_ERR_RUIM_NOT_PRESENT_S,
#endif /* FEATURE_RUIM */
    
    CM_CALL_CMD_ERR_BAD_PRIVACY_PREF_S,

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from operational limitation
    ** of CM.
    */

    
    CM_CALL_CMD_ERR_NO_BUF_L,
        /* No available CM command buffers to queue the command */

    CM_CALL_CMD_ERR_MC_COM_L,
        /* Communication problems with MC - e.g. MC buffer shortage */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    CM_CALL_CMD_ERR_OTHER,
        /* A problem other than the above was found */

    CM_CALL_CMD_ERR_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_call_cmd_err_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for CM signal types - see J-STD-008 3.7.5.5 and CAI.H file.
*/
typedef struct cm_signal_s {

    boolean    is_signal_info_avail;
        /* Indicate whether signal information is available.
        **
        ** NOTE! if this variable is FALSE, the values found in other
        ** fields of this structure as NO meaning */

    byte       signal_type;
        /* Signal type - Tone Signal, ISDN Alerting or IS-54B Alerting */

    byte       alert_pitch;
        /* Alert pitch - Medium, High or Low */

    byte       signal;
        /* Indicate which Tone signals, ISDN Alerting or IS_54B Alerting */

} cm_signal_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for CM number.
** Able to store a dialed number or a caller ID number.
**
** NOTE! In the case where the digit_mode parameter is set to
** CM_DIGIT_MODE_4BIT_DTMF the number buffer may only contain
** 0-9, "#", or "*" characters.
**
** In the case where the digit_mode parameter is set to
** CM_DIGIT_MODE_8BIT_ASCII the number buffer may contain any
** 8-bit ASCII characters.
*/
typedef struct cm_num_s {


    byte                    buf[CM_MAX_NUMBER_CHARS];
        /* Number buffer */

    byte                    len;
        /* Length of number stored in num_buf */

    cm_digit_mode_e_type    digit_mode;
        /* Origination digit mode - see J-STD-008 2.7.1.3.2.4 */

    byte                    number_type;
        /* Number type - see J-STD-008 2.7.1.3.2.4-2
        ** NOTE! ONLY VALID FOR CDMA CALLS */

    byte                    number_plan;
        /* Numbering Plan - see J-STD-008 2.7.1.3.2.4-3
        ** NOTE! ONLY VALID FOR CDMA CALLS */

    byte                    pi;
        /* Presentation Indicator - see J-STD-008  2.7.4.4-1 */

    byte                    si;
        /* Screening Indicator - see J-STD-008 2.7.4.4-2 */

} cm_num_s_type;


/* Macro to return the length of a CM number.
*/
#define CM_NUM_LEN( num_ptr )  ( (num_ptr)->len )


/* Macro to return the Number type of a call manager number -
** see J-STD-008 2.7.1.3.2.4-2
** NOTE! ONLY VALID FOR CDMA CALLS.
*/
#define CM_NUM_NUMBER_TYPE( num_ptr )  ( (num_ptr)->number_type )


/* Macro to return the Numbering Plan of a CM number -
** see J-STD-008 2.7.1.3.2.4-3
** NOTE! ONLY VALID FOR CDMA CALLS
*/
#define CM_NUM_NUMBER_PLAN( num_ptr )  ( (num_ptr)->number_plan )


/* Macro to return the Presentation Indicator of a CM number -
** see J-STD-008  2.7.4.4-1
*/
#define CM_NUM_PI( num_ptr )  ( (num_ptr)->pi )


/* Macro to return the Screening Indicator of a CM number -
** see J-STD-008 2.7.4.4-2
*/
#define CM_NUM_SI( num_ptr )  ( (num_ptr)->si )


/* Macro to return the Origination digit mode of a CM number -
** see J-STD-008 2.7.1.3.2.4
** NOTE! ONLY VALID FOR CDMA CALLS
*/
#define CM_NUM_DIGIT_MODE( num_ptr )  ( (num_ptr)->digit_mode )


/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_get

DESCRIPTION
  Copy a CM number into a specified byte buffer.

DEPENDENCIES
  CM number must have already been initialized with
  cm_num_init()

RETURN VALUE
  The numbers digits actually copied.

SIDE EFFECTS
  none

===========================================================================*/
extern byte cm_num_get(

    const cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number to copy from */

    byte*                  num_buf,
        /* Pointer to a byte buffer to copy CM number into */

    byte                   max_len
        /* Size of byte buffer */

);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_cmp

DESCRIPTION
  Compare a CM number byte with a byte buffer number over a specified
  length.

DEPENDENCIES
  CM number must have already been initialized with
  cm_num_init()

RETURN VALUE
  TRUE if both numbers match over the specified compare length,
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_num_cmp(

    const cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number */

    byte*            num_buf,
        /* Pointer to a byte buffer number */

    unsigned int     cmp_len
        /* Length to perform comparison over */
);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_cmp_exact

DESCRIPTION
  Compare a CM number byte with a byte buffer for an exact match -
  i.e. length and pattern.

DEPENDENCIES
  CM number must have already been initialized with
  cm_num_init()

RETURN VALUE
  TRUE if both numbers match exactly, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_num_cmp_exact(

    const cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number */

    byte*                  num_buf,
        /* Pointer to a byte buffer number */

    unsigned int           num_len
        /* Length off byte buffer number */
);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for CM alpha.
** Able to store alpha tag text.
*/
typedef struct cm_alpha_s {

    byte    buf[CM_MAX_ALPHA_TAG_CHARS];
        /* Alpha buffer */

    byte    len;
        /* Length of alpha tag stored in alpha_buf */
} cm_alpha_s_type;

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold call state information.
** When notified of call events, clients are given
** a reference to such a structure.
*/
typedef struct cm_call_info_s {

    cm_call_state_e_type   call_state;
        /* Call state - idle, orig, incom, conv */

    cm_client_id_type      call_client;
        /* Call client - originating/answering client */

    cm_call_type_e_type    call_type;
        /* Call type - VOICE, DATA, OTASP, etc */

    word                   srv_opt;
        /* Service option as defined in CAI.H file
        **
        ** NOTE! The service option takes on the CM_SO_ANALOG
        ** value (defined in this file) when the call is connected
        ** (or to be connected) over analog service */

    cm_num_s_type          num;
        /* Number buffer:
        ** CM_CALL_EVENT_ORIG      - contains dialed digits string.
        ** CM_CALL_EVENT_FLASH     - contains dialed digits string.
        ** CM_CALL_EVENT_INCOM     - contains caller ID string.
        ** CM_CALL_EVENT_CALLER_ID - contains caller ID string. */

    cm_alpha_s_type        alpha;
        /* Alpha tag to be displayed along with dialed address
        ** of an originated call.
        **
        ** NOTE! this is not a phone book name match, but rather
        ** an indication for the UI to display some text for an
        ** outgoing call
        **
        ** We probably need to change this one to a UI text component to
        ** support I18N
        **
        Alpha buffer:
        CM_CALL_EVENT_ORIG
        CM_CALL_EVENT_DISPLAY
        CM_CALL_EVENT_EXT_DISP        */

    cm_signal_s_type       signal;
        /* Alert signal information to be played for the incoming call */

    cm_privacy_pref_e_type privacy_pref;
      /* NOTE: This is the user preference. It is different from is_privacy
      ** which indicates the actual status of privacy during the call.
      */

    boolean                is_privacy;
        /* Indicate whether privacy mode is on */

    cm_call_end_e_type     end_status;
        /* Call end status (i.e. reason for ending a call in origination,
        incoming or conversation state) */

    boolean                is_otasp_commit;
        /* Indicate whether OTASP activation is successful */


    boolean                is_last_cdma_info_rec;
        /* this field indicates whether this is the last
           CDMA info record or not */

#ifdef FEATURE_IS95B_EXT_DISP
    byte                        ext_disp_type;
        /* display type for extended display info record */
#endif /* FEATURE_IS95B_EXT_DISP */


#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
    boolean                 alert_ans_bypass;
         /* Indicate whether the incoming call need to be answered by the clients */
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */

#ifdef FEATURE_IS95B_NDSS
    boolean                     ndss_in_progress;
        /* MC is performing NDSS redirection */
#endif /* FEATURE_IS95B_NDSS */

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_IS2000
    byte end_reason;  
      /* the reason for ending a call, it's different from end_status to keep
       * the old interface unchanged while adding more IS2000 specific reasons
       * */

    cm_retry_order_type retry_order;
      /* retry order info */
#endif /* FEATURE_IS2000 */

} cm_call_info_s_type;



/* <EJECT> */
/*---------------------------------------------------------------------------
                           PHONE RELATED TYPES
---------------------------------------------------------------------------*/


/* Enumeration of phone service states.
*/
typedef enum cm_srv_state_e {

    CM_SRV_STATE_NONE=-1,       /* FOR INTERNAL USE OF CM ONLY! */

    CM_SRV_STATE_NO_SRV,        /* phone has no service */
    CM_SRV_STATE_ANALOG,        /* phone has analog service */
    CM_SRV_STATE_DIGITAL,       /* phone has digital service */

    CM_SRV_STATE_MAX            /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_srv_state_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of phone operating modes.
*/
typedef enum cm_oprt_mode_e_ {

    CM_OPRT_MODE_NONE=-1,   /* FOR INTERNAL USE OF CM ONLY! */

    CM_OPRT_MODE_PWROFF,    /* phone is powering off */
    CM_OPRT_MODE_OFFLINE,   /* phone is offline Digital*/
    CM_OPRT_MODE_OFFLINE_A, /* phone is offline analog */
    CM_OPRT_MODE_ONLINE,    /* phone is online */
    CM_OPRT_MODE_LPM,       /* phone is in LPM - Low Power Mode */
    CM_OPRT_MODE_RESET,     /* phone is resetting - i.e. power-cycling */

    CM_OPRT_MODE_MAX        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_oprt_mode_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of mode preferences.
*/
typedef enum cm_mode_pref_e {

    CM_MODE_PREF_NONE=-1,     /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
    
    CM_MODE_PREF_ANALOG_ONLY  = NV_MODE_ANALOG_ONLY,
        /* Service is limited to analog only */

    CM_MODE_PREF_DIGITAL_ONLY = NV_MODE_DIGITAL_ONLY,
        /* Service is limited to digital (CDMA or HDR) only */

    CM_MODE_PREF_AUTOMATIC    = NV_MODE_AUTOMATIC,
        /* Determine the mode automatically from the PRL order */

    CM_MODE_PREF_E911         = NV_MODE_E911,
        /* Emergency mode */

#if ( !defined(FEATURE_SD20) && \
      defined(FEATURE_CDMA_800) && \
      defined(FEATURE_CDMA_1900) )
    CM_MODE_PREF_PCS_CDMA_ONLY= NV_MODE_PCS_CDMA_ONLY,
        /* PCS mode */
    
    CM_MODE_PREF_CELL_CDMA_ONLY= NV_MODE_CELL_CDMA_ONLY,
        /* Cellular mode */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

#endif /* FEATURE_NEWCM */
    
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
    /* New for SD 2.0.
    */
    
    CM_MODE_PREF_CDMA_ONLY,
        /* Service is limited to CDMA only */
    
    CM_MODE_PREF_HDR_ONLY,
        /* Service is limited to HDR only */
    
    CM_MODE_PREF_PERSISTENT,
        /* Return to persistent mode preference */

    CM_MODE_PREF_NO_CHANGE,
        /* Do not change the mode preference */
    
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
    CM_MODE_PREF_MAX          /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_mode_pref_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of mode terms - i.e designate the mode span.
*/
typedef enum cm_mode_term_e {

  CM_MODE_TERM_NONE=-1,      /* FOR INTERNAL USE OF CM ONLY! */


  CM_MODE_TERM_PERMANENT,
      /* Permanent mode change */

  CM_MODE_TERM_PWR_CYCLE,
      /* Until power cycle */

  CM_MODE_TERM_1_CALL,
      /* Until end of next call or power-cycle */

  CM_MODE_TERM_1_CALL_OR_TIME,
      /* Until end of next call, specified timeout or power-cycle */

  CM_MODE_TERM_CM_1_CALL,
  CM_MODE_TERM_CM_1_CALL_PLUS,
  CM_MODE_TERM_CM_MO_SMS,
      /* Mode terms that are assigned automatically by call manager
      ** when mode is forced on the fly during a call origination or
      ** mobile originated SMS.
      **
      ** CM unforce the mode back to it original settings at end of
      ** call or some short time after end of call/message transmission.
      **
      ** NOTE! These mode term should NOT be used by clients
      ** when changing the mode preference */


  CM_MODE_TERM_MAX           /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_mode_term_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of system preferences - obsolete in SD 2.0.
*/
typedef enum cm_sys_pref_e {

    CM_SYS_PREF_NONE=-1,      /* FOR INTERNAL USE OF CM ONLY! */


#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
    CM_SYS_PREF_A_ONLY     = NV_SP_A_ONLY,
        /* Service is limited to A system only
        ** NOTE! SHOULD ONLY BE USED IN 800 MHz TARGETS */

    CM_SYS_PREF_B_ONLY     = NV_SP_B_ONLY,
        /* Service is limited to B system only
        ** NOTE! SHOULD ONLY BE USED IN 800 MHz TARGETS */

    CM_SYS_PREF_HOME_ONLY  = NV_SP_HOME_ONLY,
        /* Service is limited to home system only */

    CM_SYS_PREF_STANDARD   = NV_SP_STANDARD,
        /* Home service is preferred */


    CM_SYS_PREF_MAX           /* FOR INTERNAL USE OF CM ONLY! */

#endif /* FEATURE_NEWCM */

} cm_sys_pref_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of roaming statuses.
*/
typedef enum cm_roam_status_e {

    CM_ROAM_STATUS_NONE=-1,   /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
    
    CM_ROAM_STATUS_OFF   = DB_ROAM_OFF,
    CM_ROAM_STATUS_ON    = DB_ROAM_ON,
    CM_ROAM_STATUS_BLINK = DB_ROAM_BLINK,

#endif /* FEATURE_NEWCM */

    CM_ROAM_STATUS_MAX   = 0xFF   /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_roam_status_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of answer voice as data settings
*/
typedef enum cm_answer_voice_e {

    CM_ANSWER_VOICE_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
    CM_ANSWER_VOICE_AS_VOICE = DB_VOICE_AS_DATA_NEVER,
        /* Answer voice as voice */

    CM_ANSWER_VOICE_AS_FAX_ONCE = DB_VOICE_AS_FAX_ONCE,
        /* Answer voice as fax once (for the next 10 minutes) */

    CM_ANSWER_VOICE_AS_FAX_ALWAYS = DB_VOICE_AS_FAX_ALWAYS,
         /* Answer voice as fax always */

    CM_ANSWER_VOICE_AS_MODEM_ONCE = DB_VOICE_AS_MODEM_ONCE,
        /* Answer voice as modem once (for the next 10 minutes) */

    CM_ANSWER_VOICE_AS_MODEM_ALWAYS = DB_VOICE_AS_MODEM_ALWAYS,
        /* Answer voice as modem always */

    CM_ANSWER_VOICE_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#endif /* FEATURE_NEWCM */


} cm_answer_voice_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of NAM selections
*/
typedef enum cm_nam_e {

    CM_NAM_NONE=-1,         /* FOR INTERNAL USE OF CM ONLY! */

    CM_NAM_1,               /* NAM 1*/
    CM_NAM_2,               /* NAM 2 - for targets with more than 1 NAM */
    CM_NAM_3,               /* NAM 3 - for targets with more than 2 NAM's */
    CM_NAM_4,               /* NAM 4 - for targets with more than 3 NAM's */
#ifdef FEATURE_SKT_CAI
    CM_NAM_5,               /* NAM 5 - for targets with more than 4 NAM's */
#endif /* FEATURE_SKT_CAI */
    CM_NAM_AUTO,            /* Auto NAM - for targets with more than 1 NAM */

    CM_NAM_MAX              /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_nam_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of phone CDMA lock modes.
*/
typedef enum cm_cdma_lock_mode_e {

    CM_CDMA_LOCK_MODE_NONE=-1,  /* FOR INTERNAL USE OF CM ONLY! */

    CM_CDMA_LOCK_MODE_OFF,      /* phone is NOT CDMA locked */
    CM_CDMA_LOCK_MODE_ON,       /* phone is CDMA locked until power-cycle */

    CM_CDMA_LOCK_MODE_MAX       /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_cdma_lock_mode_e_type;


#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of phone commands client are able to send to CM.
*/
typedef enum cm_ph_cmd_e {

    CM_PH_CMD_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

    CM_PH_CMD_OPRT_MODE,        /* Change the operating mode */
    CM_PH_CMD_MODE_PREF,        /* Change the mode preference */
    CM_PH_CMD_SYS_PREF,         /* Change the system preference - ! SD20 */
    CM_PH_CMD_SS_PREF,          /* Change the system selection pref - SD20 */
    CM_PH_CMD_ANSWER_VOICE,     /* Change the answer-voice-as-data setting */
    CM_PH_CMD_NAM_SEL,          /* Change the NAM selection */
    CM_PH_CMD_CLIENT_ACT,       /* A CM client activated itself */
    CM_PH_CMD_INFO_GET,         /* Get a sanpshot of the phone information in the CM */
    CM_PH_CMD_RSSI_DELTA,       /* Set RSSI change delta threshold for RSSI reporting */
#ifdef FEATURE_JCDMA_DS
#error code not present
#endif
#ifdef FEATURE_JCDMA
#error code not present
#endif

    CM_PH_CMD_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ph_cmd_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of phone events.
** Clients are able to register with CM to be notified of any subsets
** of these phone events.
*/
typedef enum cm_ph_event_e {

    CM_PH_EVENT_NONE=-1,        /* FOR INTERNAL USE OF CM ONLY! */

    CM_PH_EVENT_OPRT_MODE,      /* Operating mode was changed */
    CM_PH_EVENT_MODE_PREF,      /* Mode preference was changed */
    CM_PH_EVENT_SYS_PREF,       /* System preference was changed - ! SD20 */
    CM_PH_EVENT_SS_PREF,        /* System Selection pref was changed - SD20*/
    CM_PH_EVENT_ANSWER_VOICE,   /* Answer voice as data was changed */
    CM_PH_EVENT_NAM_SEL,        /* NAM selection was changed */
    CM_PH_EVENT_CURR_NAM,       /* Current NAM was changed */

    CM_PH_EVENT_IN_USE_STATE,   /* In use state was changed */
    CM_PH_EVENT_SRV_STATE,      /* Service state was changed */
    CM_PH_EVENT_CDMA_LOCK_MODE, /* CDMA lock mode was changed */
    CM_PH_EVENT_ROAM_STATUS,    /* Roaming status was changed */

    CM_PH_EVENT_MAINTREQ,       /* CDMA maintenance required command */
    CM_PH_EVENT_STANDBY_SLEEP,  /* Entering powerdown sleep mode */
    CM_PH_EVENT_STANDBY_WAKE,   /* Existing powerdown sleep mode */
    CM_PH_EVENT_RSSI,           /* RSSI change */
    CM_PH_EVENT_INFO,           /* Phone information, this event only sent to the client
                                   that request this information through
                                   cm_ph_cmd_get_ph_info */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

    CM_PH_EVENT_INFO_AVAIL,     /* Phone information is now available */

    CM_PH_EVENT_MAX             /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ph_event_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of phone command errors returned by phone command
** callback functions.
*/
typedef enum cm_ph_cmd_err_e {

    CM_PH_CMD_ERR_NONE=-1,
        /* FOR INTERNAL USE OF CM ONLY! */


    CM_PH_CMD_ERR_NOERR,
        
    
    /* No errors found */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from FEATURES_XXX not being defined.
    */

    CM_PH_CMD_ERR_ANSWER_VOICE_F,
        /* Answer voice feature is not defined */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients feeding a phone command
    ** function with bad or inappropriate parameters.
    */

    CM_PH_CMD_ERR_CLIENT_ID_P,
        /* Invalid client ID; null or not initialized */

    CM_PH_CMD_ERR_OPRT_MODE_P,
        /* Bad operating mode parameter */

    CM_PH_CMD_ERR_MODE_PREF_P,
        /* Bad mode preference parameter */

    CM_PH_CMD_ERR_MODE_TERM_P,
        /* Bad mode term parameter */

    CM_PH_CMD_ERR_SYS_PREF_P,
        /* Bad system preference parameter - obsolete in SD 2.0 */
    
    CM_PH_CMD_ERR_BAND_PREF_P,
        /* Bad band preference parameter - SD 2.0 */
    
    CM_PH_CMD_ERR_ROAM_PREF_P,
        /* Bad roam preference parameter - SD 2.0 */
    
    CM_PH_CMD_ERR_ANSWER_VOICE_P,
        /* Bad answer voice parameter */

    CM_PH_CMD_ERR_ANSWER_DURATION_P,
        /* Bad answer duration parameter. When indicating a temp. setting,
        ** such as CM_ANSWER_VOICE_AS_FAX_ONCE or DB_VOICE_AS_MODEM_ONCE
        ** a non-zero duration must be indicated, as well */

    CM_PH_CMD_ERR_NAM_P,
        /* Bad NAM parameter */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients trying to instruct
    ** CM to perform inappropriate actions relative to the current
    ** state of the phone.
    */

    CM_PH_CMD_ERR_IN_USE_S,
        /* Phone is in-use (i.e. in a call) */

    CM_PH_CMD_ERR_ONLINE_S,
        /* Phone is offline */

    CM_PH_CMD_ERR_OPRT_MODE_S,
        /* Invalid command or operating mode switch relatively to the current
        ** operating mode - e.g. OFFLINE -> ONLINE */

    CM_PH_CMD_ERR_MODE_PREF_S,
        /* Invalid mode preference selection relatively to what we
        ** currently have in the PRL - e.g. ANALOG_ONLY when there are no
        ** analog entries in the PRL */

    CM_PH_CMD_ERR_SYS_PREF_S,
        /* Invalid system preference selection for this target
        ** - e.g. A_ONLY for a none 800 MHz target, etc. - obsolete in SD20*/

    CM_PH_CMD_ERR_SS_PREF_S,
        /* Invalid system selection preference for this target/PRL
        ** - e.g. band-class 1 for a PCS only target - SD 2.0 */
    
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from operational limitation
    ** of CM.
    */

    CM_PH_CMD_ERR_NO_BUF_L,
        /* No available CM command buffers to queue the command */


    CM_PH_CMD_ERR_MC_COM_L,
        /* MC communication problems */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    CM_PH_CMD_ERR_OTHER,
        /* A problem other than the above was found */


    CM_PH_CMD_ERR_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ph_cmd_err_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold phone information.
** When notified of phone events, clients are given
** a reference to such a structure.
*/
typedef struct cm_ph_info_s {

    boolean                     is_in_use;
         /* Indicate whether phone is currently in use */

    cm_oprt_mode_e_type         oprt_mode;
        /* Indicate current operating mode */

    cm_srv_state_e_type         srv_state;
        /* Indicate current service state */

    cm_cdma_lock_mode_e_type    cdma_lock_mode;
        /* Indicate whether phone is CDMA locked until power cycle */

    cm_answer_voice_e_type      answer_voice;
        /* Indicate whether phone should answer incoming
        ** voice calls as voice, fax or modem */

    cm_mode_pref_e_type         mode_pref;
        /* Indicate current mode preference */

    cm_mode_term_e_type         mode_term;
        /* Indicate the current mode preference term -
        ** one-call, power-cycle, etc.*/

    cm_sys_pref_e_type          sys_pref;
        /* Indicate current system preference - obsolete in SD 2.0 */

    #ifdef FEATURE_SD20
#error code not present
    #endif
    
    cm_roam_status_e_type       roam_status;
        /* Indicate current roaming status */

    cm_nam_e_type               nam_sel;
        /* Indicate the NAM selection */

    cm_nam_e_type               curr_nam;
        /* Indicate the current NAM -
        ** needed when NAM selection is set to AUTO-NAM */

    uint16                       rssi;
       /* RSSI in positive dBm i.e. 75 means -75dBm */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

} cm_ph_info_s_type;



/* <EJECT> */
/*---------------------------------------------------------------------------
                      SERVING SYSTEM RELATED TYPES
---------------------------------------------------------------------------*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of serving system commands client are able to send to CM.
*/
typedef enum cm_ss_cmd_e {

    CM_SS_CMD_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

    CM_SS_CMD_INFO_GET,         /* Get serving system info */

    CM_SS_CMD_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ss_cmd_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of serving system command errors returned by call command
** callback functions.
*/
typedef enum cm_ss_cmd_err_e {

    CM_SS_CMD_ERR_NONE=-1,
        /* FOR INTERNAL USE OF CM ONLY! */


    CM_SS_CMD_ERR_NOERR,
        /* No errors found */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients feeding a serving system command
    ** function with bad or inappropriate parameters.
    */

    CM_SS_CMD_ERR_CLIENT_ID_P,
        /* Bad client parameter */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from operational limitation
    ** of CM.
    */

    CM_SS_CMD_ERR_NO_BUF_L,
        /* No available CM command buffers to queue the command */


    CM_SS_CMD_ERR_OTHER,
        /* any other errors not listed above */


    CM_SS_CMD_ERR_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ss_cmd_err_e_type;



/* Enumeration of serving system events.
** Clients are able to register with CM to be notified of any subsets
** of these serving system events.
*/
typedef enum cm_ss_event_e {

    CM_SS_EVENT_NONE=-1,        /* FOR INTERNAL USE OF CM ONLY! */

    CM_SS_EVENT_SRV_CHANGED,    /* Serving system information changed */

    CM_SS_EVENT_INFO,          /* Serving system information, this event only sent to the client
                                  that request this information through
                                  cm_ss_cmd_get_ss_info */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */


    CM_SS_EVENT_MAX             /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_ss_event_e_type;




typedef enum cm_band_class_e {

    CM_BAND_CLASS_NONE=-1,        /* FOR INTERNAL USE OF CM ONLY! */

 
    CM_BAND_CELLULAR,             /* 800 MHz cellular band */
    CM_BAND_PCS,                  /* 1.8 to 2.0 GHz band for PCS or Korean PCS see cai.h*/

    CM_BAND_CLASS_MAX             /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_band_class_e_type;




typedef enum cm_block_or_system_e {

    CM_BLOCK_OR_SYSTEM_NONE=-1,        /* FOR INTERNAL USE OF CM ONLY! */
 
    CM_PCS_BLOCK_A,                    /* PCS block A */
    CM_PCS_BLOCK_B,                    /* PCS block B */
    CM_PCS_BLOCK_C,                    /* PCS block C */
    CM_PCS_BLOCK_D,                    /* PCS block D */
    CM_PCS_BLOCK_E,                    /* PCS block E */
    CM_PCS_BLOCK_F,                    /* PCS block F */
    CM_CELLULAR_SYS_A,                 /* Cellular system A */
    CM_CELLULAR_SYS_B,                 /* Cellular system B */

    CM_BLOCK_OR_SYSTEM_MAX             /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_block_or_system_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Default values for serving system stored information.
*/
#define CM_SS_SID_NONE          ( (word) 0 )
#define CM_SS_NID_NONE          ( (word) 0 )
#define CM_SS_REG_ZONE_NONE     ( (word) 0xFFFF )
#define CM_SS_PACKET_ZONE_NONE  ( (byte) 0 )
#define CM_SS_BS_P_REV_NONE     ( (byte) 0 )
#define CM_SS_P_REV_IN_USE_NONE ( (byte) 0 )


/* Bit masks indicating which fields are changed
** in cm_ss_info_s_type structure.
*/
#define CM_SS_SID_MASK              ( (word) (1<<0) )
#define CM_SS_NID_MASK              ( (word) (1<<1) )
#define CM_SS_REG_ZONE_MASK         ( (word) (1<<2) )
#define CM_SS_PACKET_ZONE_MASK      ( (word) (1<<3) )
#define CM_SS_BS_P_REV_MASK         ( (word) (1<<4) )
#define CM_SS_P_REV_IN_USE_MASK     ( (word) (1<<5) )


/* Type to hold serving system information.
** When notified of serving system events, clients are given
** a reference to such a structure.
*/
typedef struct cm_ss_info_s {


  cm_srv_state_e_type   srv_state;
        /* analog or digital */
 
  boolean               is_registered;
        /* is mobile registered on the current system */

  cm_band_class_e_type  band_class;
       /* cellular or PCS */

  cm_block_or_system_e_type        block_or_system;
       /* PCS frequency block: A to F Cellular System A or B*/    

  
  word                  changed_fields;
      /* Bit mask indicating which of the following
      ** serving system fields were changed since 
      last notification  
      
      Bit mask:
        CM_SS_SID_MASK                ( 0x0001 )
        CM_SS_NID_MASK                ( 0x0002 )
        CM_SS_REG_ZONE_MASK           ( 0x0004 )
        CM_SS_PACKET_ZONE_MASK        ( 0x0008 )     
        CM_SS_BS_P_REV_MASK           ( 0x0010 )       
        CM_SS_P_REV_IN_USE_MASK       ( 0x0020 )         
      */


  word                  sid;
      /* 15 bit System ID */

  word                  nid;
      /* 16 bit Network ID */

  word                  reg_zone;
      /* 12 bit registration zone */

  byte                  packet_zone;
      /* 8  bit packet zone */

  byte                  bs_p_rev;
      /* 8  bit base station p_rev */
  
  byte                  p_rev_in_use;
      /* 8  bit p_rev in use the lessor of 
         BS p_rev and mobile p_rev */


} cm_ss_info_s_type;

/* <EJECT> */
/*---------------------------------------------------------------------------
                      INBAND RELATED TYPES
---------------------------------------------------------------------------*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of inband commands client are able to send to CM.
*/
typedef enum cm_inband_cmd_e {

    CM_INBAND_CMD_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

    CM_INBAND_CMD_BURST_DTMF,       /* request to send  a burst DTMF */
    CM_INBAND_CMD_START_CONT_DTMF,  /* request to start a continuous DTMF tone */
    CM_INBAND_CMD_STOP_CONT_DTMF,   /* request to stop  a continuous DTMF tone */


    CM_INBAND_CMD_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_inband_cmd_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of inband command errors returned by call command
** callback functions.
*/
typedef enum cm_inband_cmd_err_e {

    CM_INBAND_CMD_ERR_NONE=-1,      /* FOR INTERNAL USE OF CM ONLY! */


    CM_INBAND_CMD_ERR_NOERR,        /* No errors found */

    /* parameter related error */

    CM_INBAND_CMD_ERR_CLIENT_ID_P,   /* invalid client ID */
    CM_INBAND_CMD_ERR_ON_LEN_P,     /* invalid DTMF pulse width */
    CM_INBAND_CMD_ERR_OFF_LEN_P,    /* invalid DTMF inter-digit interval */
    CM_INBAND_CMD_ERR_NULL_BUF_P,   /* null digit buffer */
    CM_INBAND_CMD_ERR_DIGIT_P,      /* invalid digits */
    CM_INBAND_CMD_ERR_CNT_P,        /* the value of cnt exceeds the maximum allowed value */


    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients trying to instruct
    ** CM to perform inappropriate actions relative to the current
    ** state of the phone.
    */

    CM_INBAND_CMD_ERR_BAD_STATE_S,  /* invalid state for inband request */
    CM_INBAND_CMD_ERR_OFFLINE_S,    /* Phone is offline */  

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from operational limitation
    ** of CM.
    */

    CM_INBAND_CMD_ERR_NO_BUF_L,
        /* No available CM command buffers to queue the command */


    /* other error */
    CM_INBAND_CMD_ERR_OTHER,        /* A problem other than the above was found */


    CM_INBAND_CMD_ERR_MAX           /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_inband_cmd_err_e_type;



/* Enumeration of inband events.
** Clients are able to register with CM to be notified of any subsets
** of these inband events.
*/
typedef enum cm_inband_event_e {

    CM_INBAND_EVENT_NONE=-1,              /* FOR INTERNAL USE OF CM ONLY! */

    CM_INBAND_EVENT_REV_BURST_DTMF,       /* send a burst DTMF */
    CM_INBAND_EVENT_REV_START_CONT_DTMF,  /* start a continuous DTMF tone */
    CM_INBAND_EVENT_REV_STOP_CONT_DTMF,   /* stop a continuous DTMF tone */
    CM_INBAND_EVENT_FWD_BURST_DTMF,       /* received a CDMA-buest DTMF message */
    CM_INBAND_EVENT_FWD_START_CONT_DTMF,  /* received a CDMA start-continuous DTMF tone order */
    CM_INBAND_EVENT_FWD_STOP_CONT_DTMF,   /* received a CDMA stop-continuous DTMF tone order */
#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

    CM_INBAND_EVENT_MAX                   /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_inband_event_e_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



#ifdef FEATURE_JCDMA
#error code not present
#else
#define CM_INBAND_MAX_REV_DIALED_DIGITS 32
#endif /* FEATURE_JCDMA */
    /* Max length of reverse DTMF digit string */

#define CM_INBAND_MAX_FWD_DIALED_DIGITS 64
    /* Max length of forward DTMF digit string */


#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */
    /* Max length either reverse or forward DTMF digit string */

/* Type to hold inband information.
** When notified of inband events, clients are given
** a reference to such a structure.
*/
typedef struct cm_inband_info_s {

    cm_client_id_type        client;
        /* request client ID for reverse DTMF requests */
    cm_srv_state_e_type     srv_state;
        /* analog or digital */
    uint8                   on_length;
        /* DTMF pulse width */
    uint8                   off_length;
        /* DTMF inter-digit interval */
    uint8                   cnt;
        /* digit count */
#ifdef FEATURE_JCDMA
#error code not present
#else
    uint8                   digits[CM_INBAND_MAX_FWD_DIALED_DIGITS];
#endif /* FEATURE_JCDMA */
        /* DTMF digit buffer */
    uint16                  duration;
        /* duration for continuous DTMF; in milliseconds
           for reverse stop continuous DTMF event only
        */

} cm_inband_info_s_type;


/* <EJECT> */
/*---------------------------------------------------------------------------
                      DBM RELATED TYPES & FUNCTIONS
---------------------------------------------------------------------------*/

/*---------------------------------
  This enum stores the various status
  codes that the MC can send to the 
  CM regarding MO DBM messages.
 *------------------------------ */

#if(defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM))
typedef enum
{
  CM_DBM_MO_NONE=-1,                  /* FOR INTERNAL USE OF CM ONLY! */
  CM_DBM_MO_OK,                       /* SMS msg has been completed successfully */
  CM_DBM_MO_HOLD_ORIG_RETRY_TIMEOUT,  /* HOLD ORIG retry timeout*/
  CM_DBM_MO_HOLD_ORIG,                /* MC can not process MO SMS because HOLD ORIG is TRUE*/
  CM_DBM_MO_NO_SRV,                   /* can not send SMS msg  - no service */
  CM_DBM_MO_ABORT,                    /* MC or CM aborts the SMS request */
  CM_DBM_MO_NOT_ALLOWED_IN_AMPS,      /* can not sent SMS msg in analog mode */
  CM_DBM_MO_L2_ACK_FAILURE,           /* not receiving L2 ack from base station */
  CM_DBM_MO_OUT_OF_RESOURCES,         /* e.g. out of memory buffer */
  CM_DBM_MO_ACCESS_TOO_LARGE,         /* message is too large to be sent over access */
  CM_DBM_MO_DTC_TOO_LARGE,            /* message is too large to be sent over DTC */
  CM_DBM_MO_MAX                       /* FOR INTERNAL USE OF CM ONLY! */
} cm_dbm_mo_status_e_type;
#endif /* defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/* <EJECT> */
/*---------------------------------------------------------------------------
                      SMS RELATED TYPES & FUNCTIONS
---------------------------------------------------------------------------*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Enumeration of SMS events.
** Clients are able to register with CM to be notified of any subsets
** of these events.
*/
typedef enum cm_sms_event_e {

    CM_SMS_EVENT_NONE=-1,    /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_UASMS
    CM_SMS_EVENT_MO_MSG,
    CM_SMS_EVENT_AWI_ACK,
    CM_SMS_EVENT_MO_STATUS,
    CM_SMS_EVENT_MT_MSG,
#ifdef FEATURE_BROADCAST_SMS
    CM_SMS_EVENT_BC_ENABLED,
#endif /* FEATURE_BROADCAST_SMS */
#else /* FEATURE_UASMS */
    CM_SMS_EVENT_MSG_WAITING,
    CM_SMS_EVENT_N_VOICE,
#endif /* FEATURE_UASMS */

    CM_SMS_EVENT_MAX         /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_sms_event_e_type;


typedef enum cm_sms_cmd_e {

    CM_SMS_CMD_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_UASMS
    CM_SMS_CMD_MO_MSG,           /* MO SMS */

    CM_SMS_CMD_AWI_ACK,          /* Analog Alert with info ack */

#ifdef FEATURE_BROADCAST_SMS
    CM_SMS_CMD_BC_ENABLE,
#endif /* FEATURE_BROADCAST_SMS */

#endif /* FEATURE_UASMS */
    CM_SMS_CMD_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_sms_cmd_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Enumeration of SMS command errors returned by SMS command
** callback functions.
*/
typedef enum cm_sms_cmd_err_e {

    CM_SMS_CMD_ERR_NONE=-1,
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_UASMS
    CM_SMS_CMD_ERR_NOERR,
        /* No errors found */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error codes resulting from clients feeding a serving system command
    ** function with bad or inappropriate parameters.
    */

    CM_SMS_CMD_ERR_CLIENT_ID_P,
        /* Bad client parameter */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


    CM_SMS_CMD_ERR_NOT_IDLE_S,
     /* Can not accept SMS origination while SMS state is not IDLE */

    CM_SMS_CMD_ERR_OFFLINE_S,
        /* Phone is offline */

    CM_SMS_CMD_ERR_CDMA_LOCK_S,
        /* Phone is CDMA locked */

    CM_SMS_CMD_ERR_CALL_ORIG_S,
        /* Call origination is in progress */

    CM_SMS_CMD_ERR_E911_MODE_S,
        /*  phone is in E911 callback mode only E911 call is allowed  */
    
    CM_SMS_CMD_ERR_NO_SRV_S,
        /* Client tried to send MO SMS but there is no service */

    CM_SMS_CMD_ERR_NOT_ALLOWED_IN_AMPS_S,
        /* Client tried to send MO SMS but the srv state is analog */

    CM_SMS_CMD_ERR_NOT_ANALOG_S,
        /* Client tried to send AWI ACK but the srv state is not analog */

    /* Error codes resulting from operational limitation
    ** of CM.
    */

    CM_SMS_CMD_ERR_NO_BUF_L,
        /* No available CM command buffers to queue the command */


    CM_SMS_CMD_ERR_OTHER,
        /* any other errors not listed above */
#endif /* FEATURE_UASMS */

    CM_SMS_CMD_ERR_MAX
        /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_sms_cmd_err_e_type;


#ifdef FEATURE_UASMS
/* Enumeration of SMS states. At all times a CM SMS must be in one
** of the states below.
*/
typedef enum cm_sms_state_e {

    CM_SMS_STATE_NONE=-1,           /* FOR INTERNAL USE OF CM ONLY! */

    CM_SMS_STATE_IDLE,              /* SMS is in idle state */

    CM_SMS_STATE_HOLD_ORIG_RETRY,   /* SMS is checking Hold Orig status periodically
                                       to determine whether MO SMS can be sent to MC */
    CM_SMS_STATE_WAIT_FOR_STATUS,   /* SMS is waiting for MO SMS status from MC */

    CM_SMS_STATE_MAX                /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_sms_state_e_type;


typedef struct cm_sms_awi_ack_s {

    boolean                bf;                /* begin/final flag */
    
    byte                   seq_no;            /* sequence number */

    byte                   status;
   
    byte                   error_class;
} cm_sms_awi_ack_s_type;

/*---------------------------------
  Now the possible statuses, as
  returned by MC. Since SMS uses
  DBMs, these must map to the DBM
  enum, defined above.
 *------------------------------ */

typedef enum cm_sms_mo_status_e {

   CM_SMS_MO_NONE=-1,                  
     /* FOR INTERNAL USE OF CM ONLY! */
   CM_SMS_MO_OK=CM_DBM_MO_OK,                       
     /* SMS msg has been completed successfully */
   CM_SMS_MO_HOLD_ORIG_RETRY_TIMEOUT=CM_DBM_MO_HOLD_ORIG_RETRY_TIMEOUT, 
     /* HOLD ORIG retry timeout*/
   CM_SMS_MO_HOLD_ORIG=CM_DBM_MO_HOLD_ORIG,               
     /* MC can not process MO SMS because HOLD ORIG is TRUE*/
   CM_SMS_MO_NO_SRV=CM_DBM_MO_NO_SRV,                  
     /* can not send SMS msg  - no service */
   CM_SMS_MO_ABORT=CM_DBM_MO_ABORT,                   
     /* MC or CM aborts the SMS request */
   CM_SMS_MO_NOT_ALLOWED_IN_AMPS=CM_DBM_MO_NOT_ALLOWED_IN_AMPS,     
     /* can not sent SMS msg in analog mode */
   CM_SMS_MO_L2_ACK_FAILURE=CM_DBM_MO_L2_ACK_FAILURE,          
     /* not receiving L2 ack from base station */
   CM_SMS_MO_OUT_OF_RESOURCES=CM_DBM_MO_OUT_OF_RESOURCES,        
     /* e.g. out of memory buffer */
   CM_SMS_MO_ACCESS_TOO_LARGE=CM_DBM_MO_ACCESS_TOO_LARGE,        
     /* message is too large to be sent over access */
   CM_SMS_MO_DTC_TOO_LARGE=CM_DBM_MO_DTC_TOO_LARGE,           
     /* message is too large to be sent over DTC */
   CM_SMS_MO_MAX                      
     /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_sms_mo_status_e_type;

#endif /* FEATURE_UASMS */

typedef struct cm_sms_info_s {
#ifdef FEATURE_UASMS
    cm_client_id_type       sms_client;         /* requesting client - MO_SMS/AWI_ACK */

    cm_sms_state_e_type     sms_state;          /* SMS state for MO SMS */

    cm_sms_awi_ack_s_type   awi_ack;            /* field for awi ack */

    uint8                   mt_sms_format;      /* message format for MT SMS */

    uint16                  length;             /* length for SMS buffer - used by MT SMS only*/

    uint8                   *data;              /* pointer to sent/received SMS buffer */

    boolean                 l2_ack_requested;   /* l2 ack required or not - used by MO SMS*/

    cm_sms_mo_status_e_type mo_status;          /* status for MO SMS */

#ifdef FEATURE_BROADCAST_SMS
    boolean                 bc_enable;          /* Enable broadcast SMS */
#endif /* FEATURE_BROADCAST_SMS */

#else /* FEATURE_UASMS */
    uint8                   msgs;               /* number of messages waiting
                                                   for CM_SMS_EVENT_MSG_WAITING */
    uint8                   data[14];           /* data field for
                                                   CM_SMS_EVENT_N_VOICE -valid data array index- 0 to 13*/
#endif /* FEATURE_UASMS */
} cm_sms_info_s_type;



#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

/* <EJECT> */
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/* <EJECT> */
/*---------------------------------------------------------------------------
                      CLIENT RELATED TYPES & FUNCTIONS
---------------------------------------------------------------------------*/


/* Types for CM event callback functions.
**
** A client that want to be notified of CM events needs to register
** with the appropriate type of callback function with CM.
**
** CM calls on the client's callback functions when events to which the
** client is registered for occur. Each client is responsible for the
** implementation of its callback functions. Note that since callback
** functions are being called from CM context they are expected to
** only queue commands to their corresponding client and return. No extra
** processing is allowed and in particular no rex_wait() and alike.
**
** NOTE! callback functions are not allowed to change the content
** of the info structure which they are given a pointer.
**
** NOTE! clients are not allowed to access the info structure after
** the their callback functions return.
*/


/* Type for call event callback function.
**
** A client that want to be notified of call events needs to register
** with such a function with CM.
*/
typedef void (cm_call_event_f_type) (

    cm_call_event_e_type         call_event,
        /* Indicate call event */

    const cm_call_info_s_type    *call_info_ptr
        /* A pointer to a call state information structure */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for phone event callback function.
**
** A client that want to be notified of phone events needs to register
** with such a function with CM.
*/
typedef void (cm_ph_event_f_type) (

    cm_ph_event_e_type           ph_event,
        /* Indicate phone event */

    const cm_ph_info_s_type      *ph_info_ptr
        /* A pointer to a phone state information structure */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for serving system event callback function.
**
** A client that want to be notified of serving system events needs to
** register with such a function with CM.
*/
typedef void (cm_ss_event_f_type) (

    cm_ss_event_e_type           ss_event,
        /* Indicate serving system event */

    const cm_ss_info_s_type      *ss_info_ptr
        /* A pointer to a serving system information structure */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for inband event callback function.
**
** A client that want to be notified of inband events needs to
** register with such a function with CM.
*/
typedef void (cm_inband_event_f_type) (

    cm_inband_event_e_type           inband_event,
        /* Indicate inband event */

    const cm_inband_info_s_type      *inband_info_ptr
        /* A pointer to a inband information structure */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for SMS event callback function.
**
** A client that want to be notified of inband events needs to
** register with such a function with CM.
*/
typedef void (cm_sms_event_f_type) (

    cm_sms_event_e_type           sms_event,
        /* Indicate sms event */

    const cm_sms_info_s_type      *sms_info_ptr
        /* A pointer to a sms information structure */

);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for call command error callback function.
**
** A client that want to be notified of OTHER clients call command errors
** needs to register with such a function with CM.
*/
typedef void (cm_call_cmd_err_f_type) (

    cm_client_id_type            client,
        /* Indicate which client caused the error */

    cm_call_cmd_e_type           cmd,
        /* Indicate for which call command this error is reported */

    cm_call_cmd_err_e_type       cmd_err
        /* Indicate nature of error */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for phone command error callback function.
**
** A client that want to be notified of OTHER clients phone command errors
** needs to register with such a function with CM.
*/
typedef void (cm_ph_cmd_err_f_type) (

    cm_client_id_type       client,
        /* Indicate which client caused the error */

    cm_ph_cmd_e_type        cmd,
        /* Indicate for which phone command this error is reported */

    cm_ph_cmd_err_e_type    cmd_err
        /* Indicate nature of error */

);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for serving system command error callback function.
**
** A client that want to be notified of OTHER clients serving system command errors
** needs to register with such a function with CM.
*/
typedef void (cm_ss_cmd_err_f_type) (

    cm_client_id_type       client,
        /* Indicate which client caused the error */

    cm_ss_cmd_e_type        cmd,
        /* Indicate for which serving system command this error is reported */

    cm_ss_cmd_err_e_type    cmd_err
        /* Indicate nature of error */

);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for inband command error callback function.
**
** A client that want to be notified of OTHER clients inband command errors
** needs to register with such a function with CM.
*/
typedef void (cm_inband_cmd_err_f_type) (

    cm_client_id_type           client,
        /* Indicate which client caused the error */

    cm_inband_cmd_e_type        cmd,
        /* Indicate for which inband command this error is reported */

    cm_inband_cmd_err_e_type    cmd_err
        /* Indicate nature of error */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for SMS command error callback function.
**
** A client that want to be notified of OTHER clients sms command errors
** needs to register with such a function with CM.
*/
typedef void (cm_sms_cmd_err_f_type) (

    cm_client_id_type           client,
        /* Indicate which client caused the error */

    cm_sms_cmd_e_type           cmd,
        /* Indicate for which sms command this error is reported */

    cm_sms_cmd_err_e_type       cmd_err
        /* Indicate nature of error */

);


#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifndef FEATURE_NEWCM

/* Type for bit array of arbitrary size.
** Being used to indicate for which call and phone events
** a client is registered.
*/
typedef struct bit_arr_s {

    byte    *bit_buf;
        /* Pointer to bit buffer */

    int     bit_size;
        /* Bit size of the buffer pointed by bit_buf */

} bit_arr_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for CM client.
**
** Client structures keep a record of all the CM events for which their
** respective client is registered and thus call on their client callback
** functions as needed.
**
** Each CM client needs to allocate such a CM client structure in its own
** program space.
**
** Clients register and/or de-register for CM events dynamically via methods
** that operate on the CM client object that is allocated in their own
** program space.
**
** NOTE THAT NONE OF THE CM CLIENT STRUCTURE FIELDS SHOULD BE MODIFIED OR
** ACCESSED DIRECTLY BY THE ITS CLIENT. ANY INITIALIZATION OR MANIPULATION
** OF THE CM CLIENT STRUCTURE IS TO BE DONE VIA THE CM CLIENT METHODS
** SUPPLIED IN THIS MODULE.
*/
typedef struct cm_client_s {


    q_link_type               link;
        /* Link element to link clients into a client-list */
        /* this element has to be the first in this structure
           because the address of link is also used as
           the "self pointer" (to save memory space by eliminating
           self_ptr in q_link_type) to access other elements in this
           data structure */

    byte                      init_mask;
        /* Indicate whether this object was properly initialized */

    boolean                   is_active;
        /* Indicate whether client object is active. Note that CM
        ** only notifies active clients of CM events */

    cm_client_id_type         client;
        /* Client ID - each initialized client structure has
        ** a unique client ID */


    byte                      call_bit_buf[1 + (int)CM_CALL_EVENT_MAX/8];

    bit_arr_s_type            call_bit_arr;
        /* Bit array indicating to which CALL events this
        ** client is registered */

    byte                      ph_bit_buf[1 + (int)CM_PH_EVENT_MAX/8];

    bit_arr_s_type            ph_bit_arr;
        /* Bit array indicating to which PHONE events this
        ** client is registered */


    byte                      ss_bit_buf[1 + (int)CM_SS_EVENT_MAX/8];
    bit_arr_s_type            ss_bit_arr;
        /* Bit array indicating to which SERVING SYSTEM events this
        ** client is registered */

    byte                      inband_bit_buf[1 + (int)CM_INBAND_EVENT_MAX/8];
    bit_arr_s_type            inband_bit_arr;
        /* Bit array indicating to which inband events this
        ** client is registered */

    byte                      sms_bit_buf[1 + (int)CM_SMS_EVENT_MAX/8];
    bit_arr_s_type            sms_bit_arr;
       /* Bit array indicating to which SMS events this
       ** client is registered */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

    cm_call_event_f_type      *call_event_func;
        /* Function pointer to notify client of call events */

    cm_ph_event_f_type        *ph_event_func;
        /* Function pointer to notify client of phone events */

    cm_ss_event_f_type        *ss_event_func;
        /* Function pointer to notify client of serving system events */

    cm_inband_event_f_type    *inband_event_func;
        /* Function pointer to notify client of inband events */

    cm_sms_event_f_type       *sms_event_func;
        /* Function pointer to notify client of SMS events */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

    cm_call_cmd_err_f_type    *call_cmd_err_func;
        /* Function to notify client of call command errors of OTHER
        ** clients.
        ** NOTE that typically only a UI like client registers for such a
        ** service since it needs to display messages for command errors
        ** on behalf of other clients - e.g. data services failed to
        ** originate because phone is offline */

    cm_ph_cmd_err_f_type      *ph_cmd_err_func;
        /* Function to notify the client of phone command errors of OTHER
        ** clients.
        ** NOTE that typically only a UI like client registers for such a
        ** service since it needs to display messages for command errors
        ** on behalf of other clients */

    cm_ss_cmd_err_f_type      *ss_cmd_err_func;
        /* Function to notify the client of serving command errors of OTHER
        ** clients.
        ** NOTE that typically only a UI like client registers for such a
        ** service since it needs to display messages for command errors
        ** on behalf of other clients */

    cm_inband_cmd_err_f_type  *inband_cmd_err_func;
        /* Function to notify the client of inband command errors of OTHER
        ** clients.
        ** NOTE that typically only a UI like client registers for such a
        ** service since it needs to display messages for command errors
        ** on behalf of other clients */

    cm_sms_cmd_err_f_type     *sms_cmd_err_func;
        /* Function to notify the client of SMS command errors of OTHER
        ** clients.
        ** NOTE that typically only a UI like client registers for such a
        ** service since it needs to display messages for command errors
        ** on behalf of other clients */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

} cm_client_s_type;

#endif /* FEATURE_NEWCM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Enumeration of client API return status*/

typedef enum cm_client_status_e {

    CM_CLIENT_ERR_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

    CM_CLIENT_OK,                   /* No error, request was completed successfully */

    CM_CLIENT_ERR_CLIENT_ID_PTR,    /* Invalid client ID pointer*/

    CM_CLIENT_ERR_CLIENT_ID,        /* Invalid client ID */

    CM_CLIENT_ERR_EVENT_REG_TYPE,   /* Invalid event registration type */

    CM_CLIENT_ERR_FROM_EVENT,       /* Invalid from event */

    CM_CLIENT_ERR_TO_EVENT,         /* Invalid to event */

    CM_CLIENT_ERR_CLIENT_INIT_FAIL, /* Unable to initialize the client */


    CM_CLIENT_ERR_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_client_status_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Enumeration of client event registration type */

typedef enum cm_client_event_reg_e {

    CM_CLIENT_EVENT_NONE=-1,          /* FOR INTERNAL USE OF CM ONLY! */

    CM_CLIENT_EVENT_REG,              /* event registration */

    CM_CLIENT_EVENT_DEREG,            /* event de-registration */

    CM_CLIENT_EVENT_MAX               /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_client_event_reg_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/*===========================================================================

FUNCTION cm_client_init

DESCRIPTION
  Initializing a client object.

  This function must be called exactly once on a client object before
  it is being used, in any way, place, or form.

  Note that in order to get notified of CM events a client still needs
  to do the following:
  1. Register its callback functions and specific events with each functional group
  2. Activate its client object with cm_client_act()

DEPENDENCIES
  none

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
extern cm_client_status_e_type cm_client_init(

    cm_client_id_type               *client_ptr
        /* Client ID pointer*/
);
#endif /* FEATURE_NEWCM */


#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_call_reg

DESCRIPTION
  This function allows a client to register the following call-related
  callbacks and events with CM:

   1. Callback function to receive registered call events
   2. Register and de-register call events
   3. Callback function to receive other clients' call-command errors.

   If a client registers a callback function to receive call event
   notification, it also needs to register one or more call events it is
   interested in receiving. This API has a cumulative effect for event
   registration. For example, in order to register for two different ranges
   of call events, a client needs to call on this API twice- once for each
   range of call events.

  If a client registers a callback function to receive other clients'
  call-command errors, it will receive notifications on all possible
  call-command errors caused by other clients. A client cannot register to
  receive only a subset of call-command errors. The NULL function pointer can
  also be used for the callback function if a client is not interested in
  receiving that callback. For example, if a client is not interested in
  receiving other clients' call-command errors, it can set
  call_cmd_err_func = NULL. However, if the call_event_func is set to NULL,
  all registered call events are also de-registered and the event_reg_type,
  from_call_event, and to_call_event parameters are ignored.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
cm_client_status_e_type cm_client_call_reg(

    cm_client_id_type           client,
        /* Requesting client */


    cm_call_event_f_type        *call_event_func,
        /* Pointer to a callback function to notify the client of call
        ** events */

    cm_client_event_reg_e_type  event_reg_type,

    cm_call_event_e_type        from_call_event,
        /* register from this event (including) */

    cm_call_event_e_type        to_call_event,
          /* to this event (including) */

    cm_call_cmd_err_f_type      *call_cmd_err_func
      /* pointer to a callback function to notify the client of call
      ** command errors of OTHER clients */
);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_ph_reg

DESCRIPTION
  This function allows a client to register the following phone-related
  callbacks and events with CM:

   1. Callback function to receive registered phone events
   2. Register and de-register phone events
   3. Callback function to receive other clients' phone-command errors.

   If a client registers a callback function to receive phone event
   notification, it also needs to register one or more phone events it is
   interested in receiving. This API has a cumulative effect for event
   registration. For example, in order to register for two different ranges
   of call events, a client needs to call on this API twice- once for each
   range of call events.

  If a client registers a callback function to receive other clients'
  phone-command errors, it will receive notifications on all possible
  phone-command errors caused by other clients. A client cannot register to
  receive only a subset of phone-command errors. The NULL function pointer can
  also be used for the callback function if a client is not interested in
  receiving that callback. For example, if a client is not interested in
  receiving other clients' phone-command errors, it can set
  ph_cmd_err_func = NULL. However, if the ph_event_func is set to NULL,
  all registered phone events are also de-registered and the event_reg_type,
  from_ph_event, and to_ph_event parameters are ignored.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
cm_client_status_e_type cm_client_ph_reg(

    cm_client_id_type           client,
        /* Requesting client */

    cm_ph_event_f_type          *ph_event_func,
        /* Pointer to a callback function to notify the client of phone
        ** events */

    cm_client_event_reg_e_type  event_reg_type,

    cm_ph_event_e_type          from_ph_event,
        /* register from this event (including) */

    cm_ph_event_e_type          to_ph_event,
          /* to this event (including) */

    cm_ph_cmd_err_f_type        *ph_cmd_err_func
      /* pointer to a callback function to notify the client of phone
      ** command errors of OTHER clients */
);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_ss_reg

DESCRIPTION
  This function allows a client to register the following serving system-related
  callbacks and events with CM:

   1. Callback function to receive registered serving system events
   2. Register and de-register serving system events
   3. Callback function to receive other clients' serving system-command errors.

   If a client registers a callback function to receive serving system event
   notification, it also needs to register one or more serving system events it is
   interested in receiving. This API has a cumulative effect for event
   registration. For example, in order to register for two different ranges
   of call events, a client needs to call on this API twice- once for each
   range of call events.

  If a client registers a callback function to receive other clients'
  serving system-command errors, it will receive notifications on all possible
  serving system-command errors caused by other clients. A client cannot register to
  receive only a subset of serving system-command errors. The NULL function pointer can
  also be used for the callback function if a client is not interested in
  receiving that callback. For example, if a client is not interested in
  receiving other clients' serving system-command errors, it can set
  ss_cmd_err_func = NULL. However, if the ss_event_func is set to NULL,
  all registered serving system events are also de-registered and the event_reg_type,
  from_ss_event, and to_ss_event parameters are ignored.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
cm_client_status_e_type cm_client_ss_reg(

    cm_client_id_type           client,
        /* Requesting client */

    cm_ss_event_f_type          *ss_event_func,
        /* Pointer to a callback function to notify the client of serving system
           ** events */

    cm_client_event_reg_e_type  event_reg_type,

    cm_ss_event_e_type          from_ss_event,
        /* register from this event (including) */

    cm_ss_event_e_type          to_ss_event,
          /* to this event (including) */

    cm_ss_cmd_err_f_type        *ss_cmd_err_func
      /* pointer to a callback function to notify the client of serving system
      ** command errors of OTHER clients */
);

/*===========================================================================

FUNCTION cm_client_inband_reg

DESCRIPTION
  This function allows a client to register the following inband-related
  callbacks and events with CM:

   1. Callback function to receive registered inband events
   2. Register and de-register inband events
   3. Callback function to receive other clients' inband-command errors.

   If a client registers a callback function to receive inband event
   notification, it also needs to register one or more inband events it is
   interested in receiving. This API has a cumulative effect for event
   registration. For example, in order to register for two different ranges
   of inband events, a client needs to call on this API twice- once for each
   range of inband events.

  If a client registers a callback function to receive other clients'
  inband-command errors, it will receive notifications on all possible
  inband-command errors caused by other clients. A client cannot register to
  receive only a subset of inband-command errors. The NULL function pointer can
  also be used for the callback function if a client is not interested in
  receiving that callback. For example, if a client is not interested in
  receiving other clients' inband-command errors, it can set
  inband_cmd_err_func = NULL. However, if the inband_event_func is set to NULL,
  all registered inband events are also de-registered and the event_reg_type,
  from_inbnad_event, and to_inband_event parameters are ignored.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
cm_client_status_e_type cm_client_inband_reg(

    cm_client_id_type               client,
        /* Requesting client */

    cm_inband_event_f_type          *inband_event_func,
        /* Pointer to a callback function to notify the client of inband
        ** events */

    cm_client_event_reg_e_type      event_reg_type,

    cm_inband_event_e_type          from_inband_event,
        /* register from this event (including) */

    cm_inband_event_e_type          to_inband_event,
          /* to this event (including) */

    cm_inband_cmd_err_f_type        *inband_cmd_err_func
      /* pointer to a callback function to notify the client of inband
      ** command errors of OTHER clients */
);


/*===========================================================================

FUNCTION cm_client_sms_reg

DESCRIPTION
  This function allows a client to register the following sms-related
  callbacks and events with CM:

   1. Callback function to receive registered sms events
   2. Register and de-register sms events
   3. Callback function to receive other clients' sms-command errors.

   If a client registers a callback function to receive inband event
   notification, it also needs to register one or more inband events it is
   interested in receiving. This API has a cumulative effect for event
   registration. For example, in order to register for two different ranges
   of sms events, a client needs to call on this API twice- once for each
   range of sms events.

  If a client registers a callback function to receive other clients'
  sms-command errors, it will receive notifications on all possible
  sms-command errors caused by other clients. A client cannot register to
  receive only a subset of sms-command errors. The NULL function pointer can
  also be used for the callback function if a client is not interested in
  receiving that callback. For example, if a client is not interested in
  receiving other clients' sms-command errors, it can set
  sms_cmd_err_func = NULL. However, if the sms_event_func is set to NULL,
  all registered sms events are also de-registered and the event_reg_type,
  from_sms_event, and to_sms_event parameters are ignored.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
cm_client_status_e_type cm_client_sms_reg(

    cm_client_id_type               client,
        /* Requesting client */

    cm_sms_event_f_type             *sms_event_func,
        /* Pointer to a callback function to notify the client of sms
        ** events */

    cm_client_event_reg_e_type      event_reg_type,

    cm_sms_event_e_type             from_sms_event,
        /* register from this event (including) */

    cm_sms_event_e_type             to_sms_event,
          /* to this event (including) */

    cm_sms_cmd_err_f_type           *sms_cmd_err_func
      /* pointer to a callback function to notify the client of sms
      ** command errors of OTHER clients */
);

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */


#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */



/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_act

DESCRIPTION
  Activate the client object, so that the client will be notified
  of call manager events (for which it is registered).

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
   request status

SIDE EFFECTS
  Add the client object to the client list.

===========================================================================*/
extern cm_client_status_e_type cm_client_act(

    cm_client_id_type               client
        /* Requesting client */

);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_deact

DESCRIPTION
  Deactivate a client object, so that the client will NOT be notified
  of call manager events (for which it is registered).

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  request status

SIDE EFFECTS
  Remove the client object from the client list.

===========================================================================*/
extern cm_client_status_e_type cm_client_deact(

    cm_client_id_type               client
        /* Requesting client */

);



/* <EJECT> */
/*---------------------------------------------------------------------------
                      COMMAND RELATED TYPES & FUNCTIONS
---------------------------------------------------------------------------*/


/* Type for call command callback functions.
**
** A client that wants to be notified of the result of a  call command
** needs to supply a pointer to a function of this type as the first
** parameter to a call-command-function.
**
** NOTE! Client's provided callback functions must be reentrant since they
** might be called from client, as well as, CM context simultaneously.
*/
typedef void (cm_call_cmd_cb_f_type) (

    void                      *data_block_ptr,
        /* Pointer to client provided data block.
        **
        ** NOTE! Depending on the client implementation of this function
        ** the data block pointed by this pointer might be accessed from
        ** client, as well as, CM context simultaneously */

    cm_call_cmd_e_type        call_cmd,
        /* Indicate which command is this error status for */

    cm_call_cmd_err_e_type    call_cmd_err
        /* Call command error code, indicating whether command
        ** is rejected and the reason */
);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for phone command callback functions.
**
** A client that wants to be notified of the result of a
** phone-command-function needs to supply a pointer to a function
** of this type as the first parameter to a phone-command-function.
*/
typedef void (cm_ph_cmd_cb_f_type) (

    void                    *data_block_ptr,
        /* Pointer to client provided data block */

    cm_ph_cmd_e_type        ph_cmd,
        /* Indicate which phone command is this error status for */

    cm_ph_cmd_err_e_type    ph_cmd_err
        /* Phone command error code, indicating whether command
        ** is rejected and the reason */
);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for serving system command callback functions.
**
** A client that wants to be notified of the result of a
** serving system-command-function needs to supply a pointer to a function
** of this type as the first parameter to a serving system-command-function.
*/
typedef void (cm_ss_cmd_cb_f_type) (

    void                    *data_block_ptr,
        /* Pointer to client provided data block */

    cm_ss_cmd_e_type        ss_cmd,
        /* Indicate which serving system command is this error status for */

    cm_ss_cmd_err_e_type    ss_cmd_err
        /* Serving system command error code, indicating whether command
        ** is rejected and the reason */
);




/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for inband command callback functions.
**
** A client that wants to be notified of the result of a
** inband-command-function needs to supply a pointer to a function
** of this type as the first parameter to a inband-command-function.
*/
typedef void (cm_inband_cmd_cb_f_type) (

    void                        *data_block_ptr,
        /* Pointer to client provided data block */

    cm_inband_cmd_e_type        inband_cmd,
        /* Indicate which inband command is this error status for */

    cm_inband_cmd_err_e_type    inband_cmd_err
        /* Inband command error code, indicating whether command
        ** is rejected and the reason */
);

#ifdef FEATURE_UASMS

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for SMS command callback functions.
**
** A client that wants to be notified of the result of a
** sms-command-function needs to supply a pointer to a function
** of this type as the first parameter to a inband-command-function.
*/



typedef void (cm_sms_cmd_cb_f_type) (

    void                        *data_block_ptr,
        /* Pointer to client provided data block */

    cm_sms_cmd_e_type           sms_cmd,
        /* Indicate which sms command is this error status for */

    cm_sms_cmd_err_e_type       sms_cmd_err
        /* SMS command error code, indicating whether command
        ** is rejected and the reason */
);

#endif /* FEATURE_UASMS */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */


/* <EJECT> */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* CM command functions
**
** The CM command functions defines the Client->CM interface.
**
** Each command function takes a pointer to a client provided callback
** function, a client provided void pointer and the command's required set
** of fields.
**
** If the command function find one or more of the parameters to contain
** invalid values ( e.g. not one of the enum values for an enum parameter),
** it calls on the client's callback function IMMEDIATELY with the
** appropriate command error code. NOTE that in this case the callback
** function is actually being called from the client context - NOT CM.
**
** If all parameters found to be valid, the command function queues a
** command to CM. When CM gets the command it checks whether the command
** is valid relative to the current state of the phone/call.
**
** In case the command is not valid (e.g. ending a call that was already
** ended), CM calls on the client callback function with the appropriate
** command error code. NOTE that in this case the callback function is
** indeed being called from the CM context.
**
** If the command is successful, CM calls on the client callback function
** with an error code that indicates successful command. In addition CM
** notified registered clients of the corresponding CM event (e.g. if a
** client successfully ends a call, CM notify clients that are registered
** for the call-end-event of this event).
*/


/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_orig

DESCRIPTION
  Command CM to originate a call.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_orig(

        cm_call_cmd_cb_f_type  cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type      client,
            /* originating client */

        cm_call_type_e_type    call_type,
            /* call type - voice, data, etc. */

        cm_srv_type_e_type     srv_type,
            /* service type - AMPS, CDMA, automatic, etc. */

        word                   srv_opt,
            /* Service option as defined in CAI.H file */

        const byte             *num_buf,
            /* Dialed address buffer.
            **
            ** NOTE! In the case where the digit_mode parameter is set to
            ** CM_DIGIT_MODE_4BIT_DTMF the number buffer may only contain
            ** 0-9, "#", or "*" characters.
            **
            ** In the case where the digit_mode parameter is set to
            ** CM_DIGIT_MODE_8BIT_ASCII the number buffer may contain any
            ** 8-bit ASCII characters */

        byte                   num_len,
            /* dialed address length */

        cm_digit_mode_e_type   digit_mode,
            /* digit mode - see J-STD-008 2.7.1.3.2.4 */

        const byte             *alpha_buf,
            /* alpha tag to be displayed for the
            ** origination */

        byte                   alpha_len,
            /* alpha tag length */

        cm_activate_code_type  activate_code
            /* OTASP activation code */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_answer

DESCRIPTION
  Command CM to answer an incoming call.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_answer(

        cm_call_cmd_cb_f_type  cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type      client,
            /* answering client */

        cm_call_type_e_type    call_type
            /* call type */

);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_end

DESCRIPTION
  Command CM to end a call - in origination, incoming or conversation state.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_end(

        cm_call_cmd_cb_f_type  cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type       client
            /* ending client */

);

#ifdef FEATURE_IS2000
/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_end_with_reason

DESCRIPTION
  Command CM to end a call - in origination, incoming or conversation state.
  Unlike, cm_cal_cmd_end(), client can provide an end reason with this call.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_end_with_reason(

        cm_call_cmd_cb_f_type  cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type       client,
            /* ending client */
        byte                    end_reason
            /* reason for ending the call */
);

/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_orig_with_drs

DESCRIPTION
  Command CM to originate a call with specified DRS bit.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_orig_with_drs(

        cm_call_cmd_cb_f_type  cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type      client,
            /* originating client */

        cm_call_type_e_type    call_type,
            /* call type - voice, data, etc. */

        cm_srv_type_e_type     srv_type,
            /* service type - AMPS, CDMA, automatic, etc. */

        word                   srv_opt,
            /* Service option as defined in CAI.H file */

        const byte             *num_buf,
            /* Dialed address buffer.
            **
            ** NOTE! In the case where the digit_mode parameter is set to
            ** CM_DIGIT_MODE_4BIT_DTMF the number buffer may only contain
            ** 0-9, "#", or "*" characters.
            **
            ** In the case where the digit_mode parameter is set to
            ** CM_DIGIT_MODE_8BIT_ASCII the number buffer may contain any
            ** 8-bit ASCII characters */

        byte                   num_len,
            /* dialed address length */

        cm_digit_mode_e_type   digit_mode,
            /* digit mode - see J-STD-008 2.7.1.3.2.4 */

        const byte             *alpha_buf,
            /* alpha tag to be displayed for the
            ** origination */

        byte                   alpha_len,
            /* alpha tag length */

        cm_activate_code_type  activate_code,
            /* OTASP activation code */

        boolean drs_bit
            /* set the drs bit or not */
);



#endif /* FEATURE_IS2000 */


/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_privacy_pref

DESCRIPTION
  Command CM to change the privacy pref.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
boolean cm_call_cmd_privacy_pref
(
  cm_call_cmd_cb_f_type   cmd_cb_func,
  void                    *data_block_ptr,
  cm_client_id_type       client,
  cm_privacy_pref_e_type  privacy_pref
);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_call_cmd_flash

DESCRIPTION
  Command CM to send a flash with info. to the BS.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_call_cmd_flash(

        cm_call_cmd_cb_f_type    cmd_cb_func,
            /* client callback function */

        void                     *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type        client,
            /* sending client */

        const byte               *num_buf,
            /* dialed address buffer */

        byte                     num_len
            /* dialed address length */

);


/*===========================================================================

FUNCTION cm_call_cmd_get_call_info

DESCRIPTION
  Get a sanpshot of the current call information in the CM

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_call_cmd_get_call_info(

    cm_call_cmd_cb_f_type  cmd_cb_func,
        /* client callback function */

    void                   *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type       client
        /* requesting client ID */

);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_ph_cmd_oprt_mode

DESCRIPTION
  Command CM to put phone in a specified operating mode, such as offline,
  online, LPM, etc.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_ph_cmd_oprt_mode(

        cm_ph_cmd_cb_f_type    cmd_cb_func,
            /* client callback function */

        void                   *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type       client,
            /* Requesting client */

        cm_oprt_mode_e_type    oprt_mode
            /* Switch to this operating mode */

);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_ph_cmd_mode_pref

DESCRIPTION
  Command CM to change the phone's mode preference to a specified
  selection, such as automatic, digital-pref, digital-only, etc.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_ph_cmd_mode_pref(

        cm_ph_cmd_cb_f_type        cmd_cb_func,
            /* client callback function */

        void                       *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type          client,
            /* Requesting client */

        cm_mode_pref_e_type        mode_pref,
            /* Switch to this mode preference */

        cm_mode_term_e_type        mode_term,
            /* indicate the mode term */

        dword                      mode_duration
            /* indicate seconds duration for temp. mode term */

);


#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */


/* <EJECT> */
/*===========================================================================

FUNCTION cm_ph_cmd_sys_pref

DESCRIPTION
  Command CM to change the phone's system preference to a specified
  selection, such as standard, home-only, etc.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  none

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_ph_cmd_sys_pref(

        cm_ph_cmd_cb_f_type        cmd_cb_func,
            /* client callback function */

        void                       *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type          client,
            /* Requesting client */

        cm_sys_pref_e_type         sys_pref
            /* Switch to this system preference */

);
#endif /* ! FEATURE_SD20 */



/*===========================================================================

FUNCTION cm_ph_cmd_answer_voice

DESCRIPTION
  Command CM to change the phone's answer-voice-as setting to a specified
  selection, such as answer-voice-as-data, answer-voice-as-voice, etc.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_ph_cmd_answer_voice(

        cm_ph_cmd_cb_f_type        cmd_cb_func,
            /* client callback function */

        void                       *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type          client,
            /* Requesting client */

        cm_answer_voice_e_type     answer_voice,
            /* Indicate whether phone should answer incoming
            ** voice calls as voice, fax or modem */

        dword                      answer_duration
            /* Indicate seconds duration for temp. setting, such as
            ** CM_ANSWER_VOICE_AS_FAX_ONCE or DB_VOICE_AS_MODEM_ONCE */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_ph_cmd_nam_sel

DESCRIPTION
  Command CM to change the phone's NAM selection to NAM-1, NAM-2, etc.

  NOTE! This command fails when trying to change the NAM to illegal
  selection. E.g. changing to NAM-2 for a 1 NAM target.

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  none

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_ph_cmd_nam_sel(

        cm_ph_cmd_cb_f_type        cmd_cb_func,
            /* client callback function */

        void                       *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type          client,
            /* Requesting client */

        cm_nam_e_type              nam_sel
            /* Change NAM selection to this NAM */

);

/*===========================================================================

FUNCTION cm_ph_cmd_rssi_delta_threshold

DESCRIPTION
  Get a sanpshot of the current phone information in the CM

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE if call information is valid, FALSE otherwise.

  Note that call information only becomes valid after CM is initialized
  with cm_init_before_task_start() and cm_init_after_task_start().

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_ph_cmd_rssi_delta_threshold(

    cm_ph_cmd_cb_f_type    cmd_cb_func,
        /* client callback function */

    void                   *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type      client,
        /* requesting client ID */

    uint8                  delta
      /* RSSI change delta threshold for RSSI reporting */
);

/*===========================================================================

FUNCTION cm_ph_cmd_get_ph_info

DESCRIPTION
  Get a sanpshot of the current phone information in the CM

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_ph_cmd_get_ph_info(

    cm_ph_cmd_cb_f_type    cmd_cb_func,
        /* client callback function */

    void                   *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type      client
        /* requesting client ID */
);


#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */


/*===========================================================================

FUNCTION cm_ss_cmd_get_ss_info

DESCRIPTION
  Get a sanpshot of the current serving system information in the CM

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request


SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_ss_cmd_get_ss_info(

    cm_ss_cmd_cb_f_type  cmd_cb_func,
        /* client callback function */

    void                 *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type       client
        /* requesting client ID */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_inband_cmd_burst_dtmf

DESCRIPTION
  Command CM to send a burst DTMF

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  cm_init_before_task_start() must have already been called.

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_inband_cmd_burst_dtmf(

        cm_inband_cmd_cb_f_type     cmd_cb_func,
            /* client callback function */

        void                        *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type           client,
            /* Requesting client */

        uint8                       on_length,
            /* DTMF pulse width */

        uint8                       off_length,
            /* DTMF inter-digit interval */

        uint8                       cnt,
            /* DTMF digit count */

        const uint8                 *dtmf_digit_ptr
            /* Pointer to the DTMF digit buffer */

);
/*===========================================================================

FUNCTION cm_inband_cmd_start_cont_dtmf

DESCRIPTION
  Command CM to start a continuous DTMF

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  CM must have already been initialized with

  cm_init_before_task_start() and cm_init_after_task_start()
RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_inband_cmd_start_cont_dtmf(

        cm_inband_cmd_cb_f_type     cmd_cb_func,
            /* client callback function */

        void                        *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type           client,
            /* Requesting client */

        uint8                       digit
            /* DTMF digit */
);
/*===========================================================================

FUNCTION cm_inband_cmd_stop_cont_dtmf

DESCRIPTION
  Command CM to stop a continuous DTMF

  Note that the client supplied callback function, cmd_cb_func,
  will be called to notify the client of the command status.

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request

SIDE EFFECTS
  If command is successful, CM clients will be notified of the
  corresponding CM event.

===========================================================================*/
extern boolean cm_inband_cmd_stop_cont_dtmf(

        cm_inband_cmd_cb_f_type     cmd_cb_func,
            /* client callback function */

        void                        *data_block_ptr,
            /* pointer to client callback data block */

        cm_client_id_type           client,
            /* Requesting client */

        uint16                       duration
            /* DTMF duration */
);

#ifdef FEATURE_UASMS

/*===========================================================================

FUNCTION cm_sms_cmd_mo_msg

DESCRIPTION

     Send a mobile originated SMS message

DEPENDENCIES

  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request


SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_sms_cmd_mo_msg(

    cm_sms_cmd_cb_f_type    cmd_cb_func,
        /* client callback function */

    void                    *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type       client,
        /* requesting client ID */

    boolean                 l2_ack_requested,
        /* l2 ack is required or not */

    uint8                    *data
        /* pointer to SMS buffer */


);


/*===========================================================================

FUNCTION cm_sms_cmd_awi_ack

DESCRIPTION

  Send analog alert with info ack message to base station

DEPENDENCIES

  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request


SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_sms_cmd_awi_ack(

    cm_sms_cmd_cb_f_type  cmd_cb_func,
        /* client callback function */

    void                 *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type       client,
        /* requesting client ID */

    byte                   seq_no,
        /* sequence number */

    boolean                bf,
        /* begin/final flag */

    byte                   error_class,
        /* error class */

    byte                   status
        /* ack status */

);


#ifdef FEATURE_BROADCAST_SMS
/*===========================================================================

FUNCTION cm_sms_cmd_bc_enable

DESCRIPTION

  Enable/disable broadcast SMS

DEPENDENCIES

  CM must have already been initialized with
  cm_init_before_task_start() and cm_init_after_task_start()

RETURN VALUE
  TRUE - request sent, FALSE - No buffer to send request


SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_sms_cmd_bc_enable(

    cm_sms_cmd_cb_f_type  cmd_cb_func,
        /* client callback function */

    void                  *data_block_ptr,
        /* pointer to client callback data block */

    cm_client_id_type     client,
        /* requesting client ID */

    boolean               bc_enable
        /* enable broadcast message */

);
#endif /* FEATURE_BROADCAST_SMS */

#endif /* FEATURE_UASMS */


#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

/* <EJECT> */
/*===========================================================================
=============================================================================
======================NUMBER CLASSIFICATION INTERFACE BLOCK==================
======================== PUBLIC TYPES, DATA & FUNCTIONS =====================
=============================================================================
===========================================================================*/


/* This block group all the functionality that is associated with
** number-classification interfacing.
**
** When call manager is commanded to originate a call, it consults the
** number classification of the following information:
**
** 1. Whether the originating number matches one of the special
**    numbers that are stored in the number-classification (i.e. E911, OTASP, etc.)
**    If a match is found, CM modify the originating call type accordingly.
**
** 2. After deciding on the originating call type, CM consults the
**    number-classification whether the originating number is allowed by the
**    origination restriction currently in effect. If originating number
**    is not allowed, CM blocks the outgoing call and notifies clients of
**    origination failure.
**
** Note that CM expects a UI like client to register a number-classification callback
** functions with CM during initialization time.
**
** Until such callback functions are registered with CM, CM uses a default
** hard-coded number-classification to decide on the call type of special numbers.
*/


/* Type for number-classification callback function to determine whether an originating
** number matches one of the special number-classification number, such as E911 or
** OTASP. If a match is found, this function should indicate the originating
** call type via the call_type_ptr and if applicable the OTASP activation
** code via the activate_code_ptr.
*/
typedef void ( cm_nc_resolve_call_type_f_type ) (

    const cm_num_s_type      *orig_num_ptr,
        /* originating number */

    cm_call_type_e_type      *call_type_ptr,
        /* Associated call type */

    cm_activate_code_type    *activate_code_ptr
        /* Associated OTASP activation code (for OTASP call types) */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for number-classification callback function to determine whether an originating
** number is allowed by the origination restriction currently in effect. If
** originating number is allowed this function should return TRUE, otherwise
** this function should return FALSE.
*/
typedef boolean ( cm_nc_is_call_allowed_f_type ) (

    const cm_num_s_type      *orig_num_ptr
        /* originating number */

);


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


typedef enum cm_nc_status_e {

    CM_NC_ERR_NONE=-1,              /* FOR INTERNAL USE OF CM ONLY! */

    CM_NC_OK,                       /* No error, request was completed successfully */

    CM_NC_ERR_NULL_CALL_TYPE,       /* Null function pointer for resolve call type callback function */

    CM_NC_ERR_NULL_CALL_ALLOWED,    /* Null function pointer for call allowed callback function */

    CM_NC_ERR_MAX                   /* FOR INTERNAL USE OF CM ONLY! */

#ifdef FEATURE_RPC
#error code not present
#endif /* FEATURE_RPC */

} cm_nc_status_e_type;





/* <EJECT> */
/*===========================================================================

FUNCTION cm_nc_reg

DESCRIPTION
  Register number-classification callback functions with CM

  When call manager is commanded to originate a call, it consults the
  number-classification callback functions of the following information:

  1. Whether the originating number matches one of the special
     numbers that are stored in the number-classification (i.e. E911, OTASP, etc.)
     If a match is found, CM modify the originating call type accordingly.

  2. After deciding on the originating call type, CM consults the
     number-classification whether the originating number is allowed by the
     origination restriction currently in effect. If originating number
     is not allowed, CM blocks the outgoing call and notifies clients of
     origination failure.

DEPENDENCIES
  CM must have already been initialized with
  cm_init_before_task_start().

RETURN VALUE
  request status

SIDE EFFECTS
  none

===========================================================================*/
extern cm_nc_status_e_type cm_nc_reg(

    cm_nc_resolve_call_type_f_type     *resolve_call_type_func,
        /* pointer number-classification function resolving call types */

    cm_nc_is_call_allowed_f_type        *cm_nc_is_call_allowed_func
        /* pointer to a number-classification deciding whether a specific call type
        ** is allowed for origination */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

);



/* <EJECT> */
/*===========================================================================
=============================================================================
========================= MISCELLANEOUS FUNCTIONS ===========================
=============================================================================
===========================================================================*/


typedef union   cm_mc_rpt_u  *cm_mc_rpt_p_type;
    /* Forward declaration for MC report structure */



#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */



/* <EJECT> */
/*===========================================================================

FUNCTION cm_is_valid_mode_pref

DESCRIPTION
  Check whether a given mode preference is valid.

DEPENDENCIES
  none
  
RETURN VALUE
  TRUE if mode preference is valid, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_is_valid_mode_pref(

    cm_mode_pref_e_type    mode_pref
        /* mode preference to check */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_get_cur_nam

DESCRIPTION
  Returns the current NAM.

DEPENDENCIES
  Phone object must be initialized.

RETURNS
  The current NAM. If phone was not yet initialized, return CM_NAM_NONE.

SIDE EFFECTS
  None

===========================================================================*/
extern cm_nam_e_type  cm_get_curr_nam( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cm_autonam_is_enabled

DESCRIPTION
  Check whether auto-NAM is currently enabled.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  TRUE if auto NAM is enabled. FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cm_autonam_is_enabled (void);



/* <EJECT> */
/*===========================================================================

FUNCTION CM_MC_RPT

DESCRIPTION
  Allows MC to queue up messages to the CM.  Grab a buffer from the
  cm_mc_rpt_free_q, fill it in, and call cm_mc_rpt with it.  All this will
  do is queue it and set a signal for the CM task.  When the command
  is done, the buffer will be queued as directed in the command header,
  and a task you specify may be signaled.

DEPENDENCIES
  cm_mc_rpt_q already initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void cm_mc_rpt
(
    cm_mc_rpt_p_type cmd_ptr
);



#ifdef FEATURE_UASMS
/*===========================================================================

FUNCTION cm_mt_sms_msg

DESCRIPTION
 CM provides this function for MC to deliver mobile terminated sms message

DEPENDENCIES
  none

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cm_mt_sms_msg (

    uint8      format,
         /* MT SMS message format */

    uint16     len,
         /* msg length */

    uint8      *data
          /* msg buffer pointer */
);

#endif /* FEATURE_UASMS */


#if(defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM))

/*===========================================================================

FUNCTION cm_mt_dbm_msg

DESCRIPTION
  This function should be used as the **one** function MC should call when
  a mobile-terminated data burst message is received. This function will
  dispatch either the SMS callback or PD callback depending on the 
  type of message received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None 

===========================================================================*/

void cm_mt_dbm_msg(
  uint8 dbm_msg_type,
    /* The data burst type */
  uint8 format,
    /* The format of the data burst type. Relevant only for SMS */
  uint8* data,
    /* Pointer to the pay-load */
  uint16 data_len
    /* The number of bytes in the payload */
  );
#endif /* defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM) */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */



#ifdef FEATURE_IS95B_EXT_DISP

#define CM_EXTDISP_MAX_SUBREC_LEN  20      /* from ANSI T1.610 Annex D */


typedef struct {  /* Extended Display subrecord */
  byte display_tag;                        /* Indicator of the display info type */
  byte display_len;                        /* The display length */
  byte chari[CM_EXTDISP_MAX_SUBREC_LEN];   /* display characters */
} cm_ext_disp_subrecord;


typedef struct {  /* Extended Display subrecord iterator */
  cm_alpha_s_type*  info_ptr;              /* pointer to info being iterated */
  byte curr_offset;                        /* byte offset of start of */
                                           /*  current record */
} cm_ext_disp_itr_type;



/*===========================================================================

FUNCTION CM_EXT_DISP_ITR_RESET

DESCRIPTION
  Resets the iterator that walks through the extended display subrecords
  which exist in a packed format in the alpha field of the call event info

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void cm_ext_disp_itr_reset
(
  cm_ext_disp_itr_type  *itr_ptr,            /* ptr to iterator */
  cm_alpha_s_type       *info_ptr            /* ptr to packed info structure */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CM_EXT_DISP_ITR_NEXT

DESCRIPTION
  Increments the iterator that walks through the extended display subrecords
  which exist in a packed format in the alpha field of the call event info.

  If the end was already reached, the iterator is unchanged.

DEPENDENCIES
  Iterator is initialized using cm_ext_disp_itr_reset

RETURN VALUE
  none.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void cm_ext_disp_itr_next
(
  cm_ext_disp_itr_type *itr_ptr             /* pointer to iterator */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CM_EXT_DISP_ITR_MORE

DESCRIPTION
  Tests the iterator to see if it points to the end of the records.

DEPENDENCIES
  Iterator is initialized using cm_ext_disp_itr_reset

RETURN VALUE
  FALSE if the iterator points past the end of the record, TRUE otherwise.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern boolean cm_ext_disp_itr_more
(
  cm_ext_disp_itr_type *itr_ptr             /* pointer to iterator */
);

/* <EJECT> */
/*===========================================================================

FUNCTION CM_EXT_DISP_ITR_GET

DESCRIPTION
  The contents of the current record pointed to by the iterator are
  copied into the supplied cm_ext_disp_subrecord structure.

DEPENDENCIES
  Iterator is initialized using cm_ext_disp_itr_reset

RETURN VALUE
  none.

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern void cm_ext_disp_itr_get
(
  cm_ext_disp_itr_type *itr_ptr,             /* pointer to iterator */
  cm_ext_disp_subrecord *subrec_ptr          /* area to copy next record to */
);

#endif /* FEATURE_IS95B_EXT_DISP */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */



#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */



#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

#endif /* CM_H */

