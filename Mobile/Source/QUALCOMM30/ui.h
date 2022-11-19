#include <customer.h>
#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10)
	#include <ui.h>
#else //#ifdef MMI_TREX

#ifndef UI_H
#define UI_H
/*===========================================================================

           U S E R   I N T E R F A C E   M A I N   H E A D E R

DESCRIPTION
  This is the external interface for all external modules into the user
  interface.  Calls, commands,  and external definitions.  DMSS software.


 Copyright (c) 1991,1992,1993 by QUALCOMM INCORPORATED  All Rights Reserved.
 Copyright (c) 1994,1995,1996 by QUALCOMM INCORPORATED  All Rights Reserved.
 Copyright (c) 1997,1998-2000 by QUALCOMM INCORPORATED  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/ui.h_v   1.2   29 Mar 2000 18:10:34   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/22/00   CS      Added support for FEATURE_BROADCAST_SMS.
09/01/99   CS      Added support for undefined FEATURE_UASMS
07/07/99   SH      Added CNAP feature
11/23/98   SIU     Added Service changed support.
07/20/98   JGR     Added support for SMS message to Net Mode all changes
                   under FEATURE_DS_UP
07/07/98   vn      Support for call progress tones during analog data calls.
07/01/98   DAK     Added support for FEATURE_DS_VOICE_AS_DATA.
02/27/98   ejl     Moved NET_DATA_ARRIVED outside FEATURE_DS_UP to make TGP tables work.
02/26/98   jgr     Moved NET_DATA_ARRIVED, NET_CONNECT, and NET_DISCONNECT
                   inside FEATURE_DS_UP.
01/19/98   bns     Added support for line control message.
                   #ifdef auth.h include
11/13/97   jgr     Added more support and update support for Unwired Planet.
                   Changes are ifdef FEATURE_DS_UP
10/08/97   jjn     Renamed DIAG_AUDIO_CONTROL_PKT_SIZE to
                   newly defined UI_AUDIO_CONTROL_PKT_SIZE.
10/03/97   jjn     Added more mod_record_event calls for the Module so that
                   the Module does a more thorough reporting of events
09/02/97   dak     Changed UI_SLEEPWAIT_SIG to UI_DSPWAIT_SIG. #ifdef for
                   FEATURE_SINBAD.
08/26/97   day     Added new NV SIG for new TGP NV commands.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/18/97   BK      Eliminated some target pragmas.
06/18/97   RAM     Added support for handling Continuous DTMF start
                   and Continuous DTMF stop commands on the forward
                   traffic channel.
05/21/97   ejl     Surrounded UI_LPM_OK_F with ifdef, also added terminal
                   UI_NUM_UI_COMMANDS so enum never has hanging comma.
05/20/97   ejl     Added UI_LPM_OK_F command for low power mode ACK from MC.
03/13/97   jjn     Added (TG==T_MD) to one conditional compile statement
                   for the Module
03/19/97   CAH     Brought "new_ui_h" to the mainline.  Added T_T support.
                   Add support for AMPS EP Call Waiting CLI.
                   Add support for the AMPS WFO Caller Id mods.
03/13/97   jjn     Added (TG==T_MD) to one conditional compile statement
                   for the Module
02/01/97   CAH     Add new commands for AMPS CLI and target.h include.
01/29/97   BK      Added FEATURE_VIBRATOR initial version.
01/22/97   BK      Added key delay defines for the sound task.
01/20/97   CAH     Changed some signal definitions for TGP UI
                   Add new signal definition for TGP UI.  Checkin for BCX.
01/13/97   BNS     Added UI_DS_END_VCALL_F command.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
11/07/96   RC      Added service option param. in a UI_DATACALL_F cmd.
09/13/96   BK      Added FEATURE_OTASP.
07/30/96   jgr     Added new sms buffers system, changes are ifdef NEW_SMS.
07/15/96   jjw     Added Data Service fields for Voice-originated and
                   Answering of data calls
04/23/96   BK      Created from 1.37. UI_DIAG_ORIG_F is added.
04/16/96   CAH     Rid unused signals.
03/05/96   BK      QCP-1900 ifdefs introduced.
02/19/96   CAH     Rid extra signal define.
01/25/96   RBG     Added UI_SLEEPWAIT_SIG flag for WLL targets
12/19/95   CAH     Add AMPS support for Abbreviated Alert, Intercept and
                     Reorder.  Also modified CDMA Abbrv Alert to match AMPS.
12/08/95   BNS     Removed commands for returning forced registration result
12/06/95   BNS     Added commands for returning forced registration result
11/22/95   CAH     Use the NV define NV_MAX_DIAL_DIGITS instead of my own.
10/30/95   RAS     Updates to have css build from \ref
09/21/95   rah     Added UI_MAX_VOL definition.
08/17/95   CAH     New incoming call handling including support for new
                     Markovs and SO9.
07/05/95   RDH     Added UI_STANDBY_SLEEP_F, _WAKE_F command header types.
06/25/95   dy      Modified UI_DATA_BURST_F to UI_SMS_F and updated
                   corresponding interface structure to new SMS format.
05/01/95   CAH     Initial Gemini mods.
02/13/95   CAH     Added new NAM switched command from mc
11/16/94   CAH     New Lock and Maintenance Required commands from mc
05/18/94   CAH     Ron's ISS0 updates as per release 56
01/06/93   rrd     add data packet support
10/13/92   RRD     Major commenting and cleanup
02/02/92   rrd     Created

===========================================================================*/

#include "comdef.h"               /* basic definitions */
#include "queue.h"                /* queue services */
#include "rex.h"                  /* REX services */
#include "target.h"               /* Target configuration file */
#include "customer.h"             /* Customer Specific Definitions */
#include "mc.h"                   /* need MC_MAX_DIALED_DIGITS */
#include "nv.h"                   /* need NV_MAX_DIAL_DIGITS */

#ifdef FEATURE_AUTH
#error code not present
#endif /* FEATURE_AUTH */

#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif


#if (TG==T_MD)
#error code not present
#endif /* TG==T_MD */

#include "cm.h"

#ifdef FEATURE_UASMS
#include "uasms.h"
#endif /* FEATURE_UASMS */

#include "smsi.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define UI_MAX_VOL 0xFF

/* Maximum number of digits in a phone number */
#define UI_MAX_DIALED_DIGITS  NV_MAX_DIAL_DIGITS

/* Maximum size of a data burst - defined in the CAI */
#define UI_MAX_DATA_DIGITS    64


/* Signals for the UI task */
#define UI_CMD_Q_SIG      0x0001  /* Something on the command queue */
#define UI_KEY_SIG        0x0002  /* Key from HS task */
#define UI_RPT_TIMER_SIG  0x0004  /* Time to kick watchdog */
#define UI_NV_SIG         0x0008  /* Return from NV */

#define UI_MULTI_STOP_SIG 0x0020  /* Multitone has ended */
#define UI_TIMERS_SIG     0x0080  /* any of the callback timers */
#define UI_RING_SIG       0x0100  /* End of a ring from SND task */

#ifdef FEATURE_DS_UP
#error code not present
#endif

#ifdef FEATURE_SINBAD
#error code not present
#else
#define UI_RSVD_0400_SIG  0x0400
#endif

#define UI_RSVD_0800_SIG  0x0800

/* New signal definition for TGP UI */
#if ( TG == T_T )
#error code not present
#endif  /* ( TG == T_T ) */


#define UI_SND_KEY_TIME   95
#define UI_SND_KEY_SPACE  200
#define UI_SND_MAX_KEYS   10

#ifdef T_AUTH                     /* auth code available */
#error code not present
#else
#define UI_A_KEY_DIGITS 26
#endif

#define UI_AUDIO_CONTROL_PKT_SIZE 34

#ifdef FEATURE_AUDIO_CONTROL
#error code not present
#endif /* FEATURE_AUDIO_CONTROL */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Commands to be sent to the User Interface task */
typedef enum {
  
  UI_SMS_F,                    /* Display/process incoming SMS message     */
  UI_SMS_STATUS_F,             /* Unable to store incoming SMS message     */
        
  UI_DATACALL_F,               /* Making a data call                       */
  UI_DS_ORIG_VCALL_F,          /* Voice origination from DS (autodialer)   */
  UI_DS_ANSWER_F,              /* DS Answered call, voice or data          */
  UI_DS_END_VCALL_F,           /* End call from DS                         */
  UI_DIAG_ORIG_F,              /* Originate a call                         */
  UI_DIAG_END_F,               /* End the call                             */
 
  UI_NET_DATA_ARRIVED_F,        /* Got UDP data for UP */
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
  UI_NUM_UI_COMMANDS              /* End of command list (note no comma!)    */
} ui_name_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


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

#
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

} ui_cmd_type;

extern q_type ui_cmd_free_q;      /* free buffers for talking to UI */
#ifdef NEW_SMS
extern q_type ui_sms_free_q;      /* free buffers for sms data to UI */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#endif





/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
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


/* <EJECT> */
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

#endif

#endif //#ifdef MMI_TREX
