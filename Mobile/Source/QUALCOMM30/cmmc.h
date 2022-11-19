#ifndef CMMC_H
#define CMMC_H
/*===========================================================================

              C A L L   M A N A G E R   M C  H E A D E R   F I L E

DESCRIPTION
  This header file contains all the message definitions necessary for
  MC to interface with the Call Manager module.

Copyright (c) 1991 - 2000 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmmc.h_v   1.11   16 Feb 2001 18:40:30   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/01   SH      Added CM_END_ACK_F
02/07/01   RC      Added support for System Determination 2.0.
01/29/01   HQ      Properly feature-ize NEWCM, UASMS and GPSONE.
01/23/01   ych     Merged T53 and JCDMA features.
12/28/00   fc      Added support for retry order.
09/15/00   PJ      Added support for DBM status types.
08/23/00   vt      Added new CM interface changes.
03/13/00   RY      Added OTAPA support
09/09/99   SH      Modified serving system info interface
08/19/98   SH      Added call orig abort and access failure for orig failure
08/17/99   SH      Removed CM_CALL_ORIG_ERR_ABORT 
11/08/99   SH      Added NDSS related command IDs
08/09/99   SH      Added alert_ans_bypass for incoming call msg
08/02/99   SH      Added mo_status and orig_fail interfaces
07/15/99   SH      Initial release.

===========================================================================*/
#include "cm.h"
#ifndef CAII_H
#include "caii.h"      /* caii_ext_disp_type */
#endif /* CAII_H */
#include "nv.h"
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif

#ifdef FEATURE_SD20
#error code not present
#endif


#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Maximum size of a data burst - defined in the CAI */
#define CM_MAX_DATA_DIGITS    64

/* Commands to be sent to the Call Manager task */
typedef enum {
  CM_ABRV_ALERT_F,             /* Generate a CDMA/AMPS abbreviated alert   */
  CM_ABRV_REORDER_F,           /* Generate an AMPS abbreviated reorder     */
  CM_ABRV_INTERCEPT_F,         /* Generate an AMPS abbreviated intercept   */
  CM_ALERT_F,                  /* Analog display/process alert info        */
  CM_INCOMING_CALL_F,          /* CDMA Incoming call                       */
  CM_STOP_ALERT_F,             /* Stop the incoming alert or pending alert */
  CM_REORDER_F,                /* Generate a reorder tone                  */
  CM_INTERCEPT_F,              /* Generate an intercept tone               */
  CM_FLASH_F,                  /* Analog display/process flash information */

#ifdef FEATURE_HANDLE_FWD_DTMF
  CM_DTMF_BURST_F,             /* Play a series of DTMFs                   */
  CM_DTMF_CONT_START_F,        /* Start playing a continuous DTMF          */
  CM_DTMF_CONT_STOP_F,         /* Stop playing a continuous DTMF           */
#endif /* FEATURE_HANDLE_FWD_DTMF */

  CM_CALL_RELEASE_F,           /* Base station call release                */
  CM_CALL_FADE_F,              /* Call has ended abnormally                */
  CM_MARKOV_F,                 /* Markov Call made                         */
  CM_CALL_CONNECTED_F,         /* Call has been established                */
  CM_DISPLAY_F,                /* Display CDMA information                 */

  
  CM_CALLED_PARTY_F,           /* Called Party information                 */
  CM_CALLING_PARTY_F,          /* Calling Party information                */
  CM_CONNECTED_NUM_F,          /* Responding party information             */
  CM_SIGNAL_F,                 /* Signal information                       */

  CM_UPCALL_F,                 /* Immediately into voice call              */

  CM_LOCK_F,                   /* CDMA Lock order command                  */
  CM_UNLOCK_F,                 /* CDMA unlock command                      */
  CM_MAINTREQ_F,               /* CDMA Maintenance Required command        */
  CM_NAM_CHANGED_F,            /* NAM changed via Auto NAM command         */
  CM_STANDBY_SLEEP_F,          /* Entering powerdown sleep mode            */
  CM_STANDBY_WAKE_F,           /* Exiting powerdown sleep mode             */
  CM_N_VC_MSG_F,               /* NAMPS Short Message rcvd on Voice Channel*/
  CM_N_VC_CLI_F,               /* NAMPS Call Line Id recvd on Voice Channel*/

#ifdef FEATURE_OTASP
  CM_END_ACTIVATION_F,
  CM_OTASP_COMMIT_STATUS_F,    /* notify if an OTASP commit is in progress */
#endif /* FEATURE_OTASP */

#ifdef FEATURE_DS_VOICE_AS_DATA
  CM_VOICE_AS_DATA_F,             /* notify voice_as_data attribute changed */
#endif /* FEATURE_DS_VOICE_AS_DATA */

  CM_LPM_OK_F,                    /* Command to enter low power mode         */

#ifdef FEATURE_IS95B_WLL
#error code not present
#endif /* FEATURE_IS95B_WLL */

  CM_E911_CALLBACK_F,             /* Enter E911 callback mode         */
  CM_SRV_SYS_INFO_F,              /* Serving system parameter info */
  CM_CALL_ORIG_FAIL_F,            /* Call origination failure          */

#if(defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM))
  CM_MO_DBM_STATUS_F,
#endif /* defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM) */

  CM_MSG_WAITING_F,               /* Display number of messages waiting       */
  CM_N_VOICE_F,                   /* NAMPS Voice mail                         */
  CM_N_MSG_F,                     /* NAMPS Short Message                      */
  CM_N_CLI_F,                     /* NAMPS Call Line Identifier               */

#ifdef FEATURE_IS95B_EXT_DISP
  CM_EXT_DISPLAY_F,               /* Extended Display record                  */
#endif /*FEATURE_IS95B_EXT_DISP*/

#ifdef FEATURE_IS95B_NDSS
  CM_NDSS_REDIRECTION_F,          /* NDSS redirection is started - obsolete in SD 20 */
  CM_NDSS_ORIG_FAILURE_F,         /* NDSS call origination failure - obsolete in SD 20 */
#endif /*FEATURE_IS95B_NDSS */

#ifdef FEATURE_SD20
#error code not present
#else    
  CM_END_ACK_F,                   /* ack for MC_END_F */  
#endif/* FEATURE_SD20 */

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */
  
#ifdef FEATURE_IS2000
   CM_CALL_FAIL_RETRY_ORDER_F,    /* Retry order for call origination failure */  
#endif /* FEATURE_IS2000 */
  
  CM_NUM_CM_COMMANDS

} cm_name_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* status from the CM task, in command buffer */

typedef enum
{
  CM_DONE_S,                      /* General OK return */
  CM_BUSY_S,                      /* Command is being processed */
  CM_BADCMD_S,                    /* Invalid command */
  CM_BADPARM_S                    /* Bad Parameters */
} cm_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Command header type.  Each command to UI starts with this header.       */

/* These are the common fields (base class) that all command headers to CM_  */
/* must !start! with.  They are defined as a macro to insure that all       */
/* headers contain the same set.                                            */


#define CM_CMD_HDR_COMMON_FIELDS                                             \
  q_link_type        link;         /* Queue link */                          \
  cm_name_type       cmd;          /* Command to execute */                  \
  rex_tcb_type       *task_ptr;    /* Pointer to requesting task TCB */      \
  rex_sigs_type      sigs;         /* Signal to set upon cmd completion */   \
  cm_status_type     status;       /* Completion code status */              \
  q_type             *done_q_ptr   /* Queue to place this cmd on when done */

/* Generic command header type - all cmds to CM must start with this or one */
/* of its supersets                                                         */
typedef struct {
  CM_CMD_HDR_COMMON_FIELDS;        /* All CM headers must !start! with this */
} cm_hdr_type;



/* Info Record Command header type.  This provides more grouping info needed */
/* to process info records.  Any fields added must be added after the fields */
/* that cm_info_hdr_type and cm_hdr_type share in common.                    */
typedef struct {
  CM_CMD_HDR_COMMON_FIELDS;        /* All ui headers must !start! with this */
  boolean            last_record;  /* Marks last msg in a group of msgs */
} cm_info_hdr_type;




#ifdef FEATURE_OTASP
typedef struct {
  cm_hdr_type hdr;                    /* header */
} cm_end_activation_type;

/* Type for CM_OTASP_COMMIT_STATUS_F command from MC to CM */
typedef struct {
  cm_hdr_type hdr;              /* header */
  boolean commit_in_progress;   /* TRUE if an OTASP commit is in progress */
                                /* FALSE if an OTASP commit is finished */
} cm_otasp_commit_status_type;

#endif /* FEATURE_OTASP */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_ABRV_ALERT_F - CDMA Only */

typedef struct {
  cm_hdr_type hdr;                /* header */
  int         signal;             /* type of alert - call forwarding,  ?      */
} cm_abrv_alert_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_ALERT_F - Analog only */

typedef struct {
  cm_hdr_type hdr;                /* header */
  int  signal;                    /* Pitch and cadence of user alert tone     */
  byte cnt;                       /* Number of chars in first instance of
                                     the Calling Party Number                 */
  byte pi;                        /* Presentation indicator                   */
  byte si;                        /* Screening indicator                      */
  byte cpn[CM_MAX_DATA_DIGITS];   /* calling party number */
} cm_alert_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_INCOMING_CALL_F - CDMA only */

typedef struct {
  cm_hdr_type hdr;                /* header */
  word so;                        /* service option */
#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
  boolean alert_ans_bypass;         
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS */
} cm_incoming_call_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_STOP_ALERT_F */
 /* When this command is received, it signals the UI to end the incoming */
 /* call alert.  Additionally, if we're not currently alerting in AMPS   */
 /* mode, this signals the UI that the pending Alert failed.  In this    */
 /* case, we need to clear out any Caller Id information we have stored. */

typedef struct {
  cm_hdr_type hdr;             /* header */
} cm_stop_alert_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_REORDER_F */

typedef struct {
  cm_hdr_type hdr;             /* header */
} cm_reorder_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_INTERCEPT_F */

typedef struct {
  cm_hdr_type hdr;             /* header */
} cm_intercept_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_FLASH_F - Analog only */

typedef struct {
  cm_hdr_type hdr;             /* header */
  byte cnt;                    /* Number of chars in first instance of
                                  the Calling Party Number                 */
  byte pi;                     /* Presentation indicator                   */
  byte si;                     /* Screening indicator                      */
  byte cpn[CM_MAX_DATA_DIGITS]; /* calling party number */
} cm_flash_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_MSG_WAITING_F */
typedef struct {
  cm_info_hdr_type hdr;
  byte msgs;                   /* Indicates number of messages waiting   */
} cm_msg_waiting_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_DTMF_BURST_F */

typedef struct
{
  cm_hdr_type hdr;                        /* header */
  byte dtmf_on;                           /* dtmf on  CAI 6.7.2.3.2.7-1 */
  byte dtmf_off;                          /* dtmf off  CAI 6.7.2.3.2.7-2 */
  byte cnt;                               /* digits count */
  byte digits[CM_MAX_DATA_DIGITS];        /* Data burst digits */
} cm_dtmf_burst_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_DTMF_CONT_START_F */

#ifdef FEATURE_HANDLE_FWD_DTMF
typedef struct
{
  cm_hdr_type hdr;                        /* header */
  byte dtmf_code;                         /* dtmf code CAI 6.7.1.3.2.4-4 */
} cm_dtmf_cont_start_type;
#endif /* FEATURE_HANDLE_FWD_DTMF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_DTMF_CONT_STOP_F */

#ifdef FEATURE_HANDLE_FWD_DTMF
typedef struct
{
  cm_hdr_type hdr;                        /* header */
} cm_dtmf_cont_stop_type;
#endif /* FEATURE_HANDLE_FWD_DTMF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for CM_CALL_RELEASE_F */


typedef enum {

  CM_CALL_REL_NORMAL,             /* BS released the call and no reason is given */
  CM_CALL_REL_SO_REJECT           /* BS released the call because SO is not supported */

}cm_rel_reason_e_type;


typedef struct {
  cm_hdr_type hdr;                /* header */
  cm_rel_reason_e_type rel_code;  /* the release code - indicates why call is released*/
} cm_call_release_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_CALL_FADE_F */

typedef struct {
  cm_hdr_type hdr;                /* header */
} cm_call_fade_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_MARKOV_F */

typedef struct {
  cm_hdr_type hdr;                /* header */
} cm_markov_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_CALL_CONNECTED_F */

typedef struct {
  cm_hdr_type hdr;                /* header */
  word  so;                       /* service option */
} cm_call_connected_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_DISPLAY_F */

typedef struct {
  cm_info_hdr_type hdr;
  byte num_chars;                   /* number of characters in chars array */
  byte chars[ CM_MAX_DATA_DIGITS];  /* Characters to display */
} cm_display_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef FEATURE_IS95B_EXT_DISP

#define CM_EXTDISP_MAX_RECORD_LEN  64



typedef struct {  /* Extended Display packed info record */
  byte display;                            /* display type indicator */
  byte datalen;                            /* length of the packed array */
  byte data[CM_EXTDISP_MAX_RECORD_LEN];    /* packed array */
} cm_ext_disp_info;



/* Type for CM_EXT_DISPLAY_F */

typedef struct {
  cm_info_hdr_type hdr;                  /* header */
  cm_ext_disp_info disp_info;       /* struct for packed info */
} cm_ext_disp_type;

#endif /* FEATURE_IS95B_EXT_DISP */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_CALLED_PARTY_F */

typedef struct {
  cm_info_hdr_type hdr;
  byte number_type;                 /* Type of number - see CAI section
                                       7.7.5.2                            */
  byte number_plan;                 /* Numbering plan - see CAI section
                                       7.7.5.2                             */
  byte num_chars;                   /* number of characters in chars array */
  byte chars[ CM_MAX_DATA_DIGITS];  /* Characters to display */
} cm_called_party_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_CALLING_PARTY_F */

typedef struct {
  cm_info_hdr_type hdr;
  byte number_type;                 /* Type of number - see CAI section
                                       7.7.5.3                           */
  byte number_plan;                 /* Numbering plan - see CAI section
                                       7.7.5.3                           */
  byte pi;                          /* Presentation indicator - see CAI
                                       section 7.7.5.3                   */
  byte si;                          /* Screening indicator - see CAI
                                       section 7.7.5.3                   */
  byte num_chars;                   /* number of characters in chars array */
  byte chars[ CM_MAX_DATA_DIGITS];  /* Characters to display */
} cm_calling_party_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_CONNECTED_NUM_F */

typedef struct {
  cm_info_hdr_type hdr;

  byte number_type;                 /* Type of number - see CAI section
                                       7.7.5.4                          */
  byte number_plan;                 /* Numbering plan - see CAI section
                                       7.7.5.4                           */
  byte pi;                          /* Presentation indicator - see CAI
                                       section 7.7.5.3                   */
  byte si;                          /* Screening indicator - see CAI
                                       section 7.7.5.3                   */
  byte num_chars;                   /* number of characters in chars array */
  byte chars[ CM_MAX_DATA_DIGITS];  /* Characters to display */
} cm_connected_num_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_SIGNAL_F */

typedef struct {
  cm_info_hdr_type hdr;

  int  signal_type;               /* Type of signal to generate see CAI
                                     section 7.7.5.5                         */
  int alert_pitch;                /* Pitch of alerting signal see CAI
                                     section 7.7.5.5                          */
  int signal;                     /* desired signal - see CAI section 7.7.5.5 */
} cm_signal_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Type for CM_UPCALL_F */

typedef struct {
  cm_hdr_type hdr;             /* header */
  word so;                     /* service option */
} cm_upcall_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* This is the IS91 Extended Protocol Voice Mail Notification */
 /* Type for CM_N_VOICE_F - Analog only */

typedef struct {
  cm_hdr_type hdr;                /* header */
  byte  data[14];                 /* 6-bit chunks from message as bytes */
} cm_n_voice_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_N_MSG_F or CM_N_VC_MSG_F - Analog only */
/* This is the IS91 Extended Protocol Short Message */
/* The VC flavor is sent if the message is received during  */
/* Voice Channel.  While on this channel, this message is   */
/* interpreted to be CLI info, otherwise it's considered    */
/* SMS info.                                                */
/* The data is ASCII and is ETX terminated.                 */

typedef struct {
  cm_hdr_type hdr;                /* header */
  byte  data[15];                 /* ASCII data, ETX=03 terminated */
} cm_n_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_N_CLI_F or CM_N_VC_CLI_F - Analog only */
/* This is the IS91 Extended Protocol Caller Line ID.  */
/* The VC flavor is sent if the message is received during  */
/* Voice Channel.  While on this channel, this message is   */
/* interpreted to be CLI info, otherwise it's considered    */
/* SMS info.                                                */
/* The data is ASCII and is NULL terminated.                */

typedef struct {
  cm_hdr_type hdr;                /* header */
  byte  data[32];                 /* 4-bit chunks from message as ASCII */
} cm_n_cli_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type for CM_NAM_CHANGED_F */

typedef struct {
  cm_hdr_type hdr;                /* header */
  byte nam;                       /* New NAM number */
} cm_nam_changed_type;


#ifdef FEATURE_IS95B_WLL
#error code not present
#endif /* FEATURE_IS95B_WLL */

/* Type for CM_E911_CALLBACK_F */

typedef struct {
  cm_hdr_type hdr;             /* header */
} cm_e911_callback_type;

/*
      Bit mask:
        CM_SS_SID_MASK                ( 0x0001 )
        CM_SS_NID_MASK                ( 0x0002 )
        CM_SS_REG_ZONE_MASK           ( 0x0004 )
        CM_SS_PACKET_ZONE_MASK        ( 0x0008 )     
        CM_SS_BS_P_REV_MASK           ( 0x0010 )       
        CM_SS_P_REV_IN_USE_MASK       ( 0x0020 )    */

typedef struct {
  cm_hdr_type                   hdr;             /* header */
  byte                          band_class;       /* band class see cai.h 0-Cellular 1-PCS 4-Korean PCS*/
  nv_block_or_sys_enum_type     block_or_system;  /* PCSc block A to F or cellular system A or B*/   
  word                          rcv_fields;     /* 
                                                     bit mask to indicate the corresponding
                                                     field is rcv'd or not; 1- rcv'd 0- not Rcv'd
                                                     0x0001 - SID MASK
                                                     0x0002 - NID MASK
                                                     0x0004 - Reg zone MASK
                                                     0x0008 - packet zone MASK
                                                     0x0010 - bs_p_rev (BS protocol revision)
                                                     0x0020 - p_rev_in_use (protocol revision in use)
                                                     
                                                  */
  word                          sid;              /* 15 bit System ID */
  word                          nid;              /* 16 bit Network ID */
  word                          reg_zone;         /* 12 bit registration zone */
  byte                          packet_zone;      /* 8  bit packet zone */
  byte                          bs_p_rev;         /* 8  bit  base station p_rev */               
  byte                          p_rev_in_use;     /* 8  bit  lessor value of BS p_rev and mobile p_rev */      
} cm_srv_sys_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if(defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM))
typedef struct {
  cm_hdr_type hdr;
  uint8    msg_type; 
    /* type of message for which status is being reported */
  cm_dbm_mo_status_e_type status;
    /* status of the message */
} cm_dbm_mo_status_type;
#endif /* defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for CM_CALL_ORIG_FAIL_F */

#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */

typedef enum {

  CM_CALL_ORIG_ERR_HOLD_ORIG,         /* hold orig is TRUE */
  CM_CALL_ORIG_ERR_NO_SRV,            /* No service */
  CM_CALL_ORIG_ERR_CDMA_ACCESS_FAIL,  /* CDMA access failure */
  CM_CALL_ORIG_ERR_CDMA_PSIST_NG,     /* CDMA access failure by persistence test */
    
  CM_CALL_ORIG_ERR_ABORT              /* MC abort the message other than above reasons */ 

}cm_call_orig_fail_e_type;

#endif /* ! FEATURE_SD20 */


typedef struct {
  cm_hdr_type hdr;                 /* header */
  cm_call_orig_fail_e_type code;   /* call orig fail reason call */
} cm_call_orig_fail_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_SD20
#error code not present
#else
/* Type for CM_END_ACK_F */

typedef struct {
  cm_hdr_type hdr;             /* header */
} cm_end_ack_type;

#endif /* FEATURE_SD20 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_IS2000
/* Type for CM_CALL_FAIL_RETRY_ORDER */

typedef struct
{
  cm_hdr_type         hdr;        /* header           */
  cm_retry_order_type retry_info; /* retry order info */
} cm_retry_order_report_type;
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

/*-------------------------------------------------------------------------*/

/* Union of all MC report types.  The header is always present and it        */
/* specifies the command type and attributes.  If the command has          */
/* arguments they follow in the union.                                     */

typedef union cm_mc_rpt_u {
    cm_hdr_type                   hdr;
#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

    cm_abrv_alert_type            abrv;
    cm_alert_type                 alert;
    cm_incoming_call_type         incoming_call;
    cm_stop_alert_type            stop;
    cm_reorder_type               reorder;
    cm_intercept_type             inter;
    cm_flash_type                 flash;


#ifdef FEATURE_HANDLE_FWD_DTMF
    cm_dtmf_burst_type            dtmf;
    cm_dtmf_cont_start_type       dtmf_cont_start;
    cm_dtmf_cont_stop_type        dtmf_cont_stop;
#endif /* FEATURE_HANDLE_FWD_DTMF */

    cm_call_release_type          release;
    cm_call_fade_type             fade;
    cm_markov_type                markov;
    cm_call_connected_type        connect;
    cm_display_type               display;

#ifdef FEATURE_IS95B_EXT_DISP
    cm_ext_disp_type              ext_disp;
#endif /* FEATURE_IS95B_EXT_DISP */

    cm_called_party_type          called_party;
    cm_calling_party_type         calling_party;
    cm_connected_num_type         connected_num;
    cm_signal_type                signal;

    cm_upcall_type                upcall;
    cm_nam_changed_type           nam_changed;

#ifdef FEATURE_OTASP
    cm_end_activation_type        end_activation;
    cm_otasp_commit_status_type   otasp_commit_status;
#endif /* FEATURE_OTASP */



#ifdef FEATURE_IS95B_WLL
#error code not present
#endif /* FEATURE_IS95B_WLL */
    cm_e911_callback_type         e911_callback;
    cm_srv_sys_info_type          srv_sys_info;

#if(defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM))
    cm_dbm_mo_status_type         mo_dbm_status;
#endif /* defined(FEATURE_UASMS) || defined(FEATURE_GPSONE_DBM) */

    cm_msg_waiting_type           msgs;
    cm_n_voice_type               n_voice;
    cm_n_msg_type                 n_msg;
    cm_n_cli_type                 n_cli;

    cm_call_orig_fail_type        orig_fail;

#ifdef FEATURE_SD20
#error code not present
#else
    cm_end_ack_type               end_ack;
#endif /* FEATURE_SD20 */

#ifdef FEATURE_IS2000
    cm_retry_order_report_type    retry_order;
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_T53
#error code not present
#endif

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

} cm_mc_rpt_type;

/* ************************************************************************************/

#ifdef FEATURE_IS95B_EXT_DISP

/*===========================================================================

FUNCTION CM_EXT_DISP_BLD_INFO

DESCRIPTION
  This function translates a CAII-format extended display information
  record into a structure that can be iterated to get at the information.

DEPENDENCIES
  none.

RETURN VALUE
  number of bytes of the caii_ext_disp_type structure's var field that
  were used to build the packed structure

SIDE EFFECTS
  Nothing.

===========================================================================*/
extern word cm_ext_disp_bld_info
(
  caii_ext_disp_type* disp_ptr,             /* ptr to caii-format info  */
  cm_ext_disp_info*  info_ptr               /* ptr to packed info struct */
);
#endif /* FEATURE_IS95B_EXT_DISP */

#ifdef FEATURE_NEWCM
#error code not present
#endif /*FEATURE_NEWCM */
#endif /* CMMC_H */

