#ifdef FEATURE_UASMS /* SMS API */

#ifndef UASMSI_H
#define UASMSI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           U A P I   S H O R T   M E S S A G E   S E R V I C E S (SMS API)

                               ------ Internal Type Definitions

DESCRIPTION
  This module defines the internal data types and functions that support the
  Short Message Services (SMS) UAPI implementation.

Copyright (c) 1998-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/uasmsi.h_v   1.3   31 Jan 2001 17:14:02   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/29/01   HQ      Properly feature-ized FEATURE_NEWUASMS.
10/09/00   HQ      Added message tag field in tl message.
08/25/00   vt      Included nv.h
                   Moved UASMS_DUMMY_SEQ_NUM to uasms.h
                   Moved uasms_reset_seq_num() to uasms.h
06/15/00   HQ      Corrected signal masks so that they don't conflict with
                   common task signals.
02/25/00   HQ      Added support for FEATURE_BROADCAST_SMS.

===========================================================================*/

         
/* <EJECT> */
/*===========================================================================

                             INCLUDE FILES

===========================================================================*/
#include "uasms.h"
#include "rex.h"
#include "queue.h"
#include "cm.h"

#ifdef FEATURE_NEWCM
#error code not present
#endif

/* <EJECT> */
/*===========================================================================
                             
                         DATA TYPE DECLARATIONS
                             
===========================================================================*/

/* Transport Layer parameter mask values:
*/
enum{ UASMS_MASK_TL_NULL                = 0x00000000 };
enum{ UASMS_MASK_TL_TELESERVICE_ID      = 0x00000001 };
enum{ UASMS_MASK_TL_BC_SRV_CATEGORY     = 0x00000002 };
enum{ UASMS_MASK_TL_ADDRESS             = 0x00000004 };
enum{ UASMS_MASK_TL_SUBADDRESS          = 0x00000008 };
enum{ UASMS_MASK_TL_BEARER_REPLY_OPTION = 0x00000040 };
enum{ UASMS_MASK_TL_CAUSE_CODES         = 0x00000080 };
enum{ UASMS_MASK_TL_BEARER_DATA         = 0x00000100 };


/* Timer values
*/
#ifdef FEATURE_UTF
#error code not present
#else
enum{ UASMS_TIMER_MT_ACK          = 2000  /* ms */ };
#endif

enum{ UASMS_TIMER_AWISMS_ACK      = 700  /* ms */ };

#ifdef FEATURE_UASMS_TEST
enum{ UASMS_TIMER_MO_ACK          = ( 2* 60  * 1000 )  /* ms */ };
#else
enum{ UASMS_TIMER_MO_ACK          = ( 20 * 1000 )  /* ms */ };
#endif /* FEATURE_UASMS_TEST */

enum{ UASMS_TIMER_DTC_CALL_IN     = ( 63 * 1000 )  /* ms */ };
enum{ UASMS_TIMER_DTC_CALL_OUT    = ( 63 * 1000 )  /* ms */ };
enum{ UASMS_TIMER_DTC_DISC        = ( 63 * 1000 )  /* ms */ };

enum{ UASMS_TIMER_LAYER2          = ( 99 * 1000 )  /* ms */ };
enum{ UASMS_TIMER_LAYER2_LONG     = ( 99 * 1000 )  /* ms */ };



/* SMS Transport Layer sequence number is 6 bits (0-63).
*/
enum{ UASMS_SEQ_NUM_MAX        = 64 };

#ifndef FEATURE_NEWUASMS
enum{ UASMS_DUMMY_SEQ_NUM      = 255 };
#endif

#ifdef FEATURE_UASMS_SAVE_MEM
enum{ UASMS_MT_Q_MAX           = 5 };
enum{ UASMS_MO_Q_MAX           = 5 };
enum{ UASMS_MAX_CMDS           = 7 };
#else
enum{ UASMS_MT_Q_MAX           = 10 };
enum{ UASMS_MO_Q_MAX           = 10 };
enum{ UASMS_MAX_CMDS           = 15 };
#endif /* FEATURE_UASMS_SAVE_MEM */


/* SMS task signals
*/
enum{ UASMS_CMD_Q_SIG               =  0x0100 };
enum{ UASMS_MT_ACK_TIMER_SIG        =  0x0200 };
enum{ UASMS_MO_ACK_TIMER_SIG        =  0x0400 };
enum{ UASMS_DTC_CALL_IN_TIMER_SIG   =  0x0800 };
enum{ UASMS_DTC_CALL_OUT_TIMER_SIG  =  0x10000 };
enum{ UASMS_DTC_IDLE_TIMER_SIG      =  0x20000 };
enum{ UASMS_AWISMS_ACK_TIMER_SIG    =  0x40000 };
#define UASMS_RPT_TIMER_SIG         CM_RPT_TIMER_SIG
enum{ UASMS_NV_SIG                  = 0x100000 };

#define UASMS_SIGNALS ( UASMS_CMD_Q_SIG \
                        | UASMS_MT_ACK_TIMER_SIG \
                        | UASMS_MO_ACK_TIMER_SIG \
                        | UASMS_DTC_CALL_IN_TIMER_SIG \
                        | UASMS_DTC_CALL_OUT_TIMER_SIG \
                        | UASMS_DTC_IDLE_TIMER_SIG \
                        | UASMS_AWISMS_ACK_TIMER_SIG )



/* Parameter lengths
*/
enum{ UASMS_OTA_ADDRESS_MAX      = ( UASMS_ADDRESS_MAX + 5 ) };
enum{ UASMS_OTA_SUBADDRESS_MAX   = ( UASMS_SUBADDRESS_MAX + 4 ) };
enum{ UASMS_OTA_CAUSE_CODES_MAX  = 4 };

enum{ UASMS_OTA_TL_ACK_MAX       = ( 1 + UASMS_OTA_ADDRESS_MAX +      
                                         UASMS_OTA_SUBADDRESS_MAX +   
                                         UASMS_OTA_CAUSE_CODES_MAX ) };

enum{ UASMS_OTA_PARM_MAX        =  UASMS_MAX_LEN };

enum{ UASMS_EP_MSG_LEN_BITS     =  5 };
enum{ UASMS_EP_MST_BITS         =  8 };
enum{ UASMS_EP_CB_DIGITS        =  32 };

enum{ UASMS_A_CLI_LEN           = 40 };
enum{ UASMS_A_VOICE_MAIL_LEN    = 30 };
enum{ UASMS_A_SHORT_MSG_LEN     = 30 };

enum{ UASMS_MWI_LEN             = 1 };


/* SMS Service Options
*/
typedef enum
{
  UASMS_SO_NONE = 0,
  UASMS_SO_6    = 6,
  UASMS_SO_14   = 14

} uasms_so_e_type;


/* Transport Layer parameter Ids:
*/
typedef enum
{
  UASMS_TL_DUMMY          = -1,  /* dummy */
  UASMS_TL_TELESERVICE_ID = 0,  /* Teleservice Identifier     */
  UASMS_TL_BC_SRV_CATEGORY,     /* Broadcast Service Category */
  UASMS_TL_ORIG_ADDRESS,        /* Originating Address        */
  UASMS_TL_ORIG_SUBADDRESS,     /* Originating Subaddress     */
  UASMS_TL_DEST_ADDRESS,        /* Destination Address        */
  UASMS_TL_DEST_SUBADDRESS,     /* Destination Subaddress     */
  UASMS_TL_BEARER_REPLY_OPTION, /* Bearer Reply Option        */
  UASMS_TL_CAUSE_CODES,         /* Cause Codes                */
  UASMS_TL_BEARER_DATA          /* Bearer Data                */

} uasms_tl_parm_id_e_type;


/* Bearer Data subparameter Ids:
*/
typedef enum
{
  UASMS_BD_DUMMY  = -1,   /* dummy */
  UASMS_BD_MSG_ID = 0,    /* Message Identifier                */
  UASMS_BD_USER_DATA,     /* User Data                         */
  UASMS_BD_USER_RESP,     /* User Response Code                */
  UASMS_BD_MC_TIME,       /* Message Center Time Stamp         */
  UASMS_BD_VALID_ABS,     /* Validity Period - Absolute        */
  UASMS_BD_VALID_REL,     /* Validity Period - Relative        */
  UASMS_BD_DEFER_ABS,     /* Deferred Delivery Time - Absolute */
  UASMS_BD_DEFER_REL,     /* Deferred Delivery Time - Relative */
  UASMS_BD_PRIORITY,      /* Priority Indicator                */
  UASMS_BD_PRIVACY,       /* Privacy Indicator                 */
  UASMS_BD_REPLY_OPTION,  /* Reply Option                      */
  UASMS_BD_NUM_OF_MSGS,   /* Number of Messages                */
  UASMS_BD_ALERT,         /* Alert on Message Delivery         */
  UASMS_BD_LANGUAGE,      /* Language Indicator                */
  UASMS_BD_CALLBACK       /* Call Back Number                  */

} uasms_bd_sub_parm_id_e_type;



/* Over-the-air (raw) message structure
*/
typedef struct uasms_OTA_message_struct
{
  uasms_format_e_type         format;
  uns16                       data_len;
  uns8                        data[ UASMS_MAX_LEN ];
} uasms_OTA_message_type;


/* Transport Layer message types
*/
typedef enum
{
  UASMS_TL_TYPE_MIN               = 0,

  UASMS_TL_TYPE_POINT_TO_POINT    = 0,
  UASMS_TL_TYPE_BROADCAST         = 1,
  UASMS_TL_TYPE_ACK               = 2,

  UASMS_TL_TYPE_MAX               = 2

} uasms_tl_message_type_e_type;


/* Cause code
*/
typedef PACKED struct
{
  uns8                        reply_seq_num;
  uasms_error_class_e_type    error_class;

  /* If error_class is NoError, status should be ignored.
     Only the first half of the enums are used in this structure.
  */
  uasms_status_e_type         status;

} uasms_cause_code_type;



/* TL fields without bearer data
*/
typedef PACKED struct
{
  uasms_tl_message_type_e_type   tl_message_type;

  /* the mask indicates which fields are present in this message */
  uns16                          mask;

  uasms_teleservice_e_type       teleservice;
  uasms_address_type             address;
  uasms_subaddress_type          subaddress;
  uns8                           bearer_reply_seq_num; /* 0..63 */
  uasms_service_e_type           service;
  uasms_cause_code_type          cause_code;
  uasms_message_tag_e_type       tag;

  /* decoded bearer data */
  uasms_client_bd_type           cl_bd;

} uasms_tl_message_type;


/* Analog AWI SMS flag
*/
typedef enum
{
  UASMS_AWI_INTERMEDIATE = 0,
  UASMS_AWI_FINAL        = 1,
  UASMS_AWI_FIRST        = 2,
  UASMS_AWI_UNSEGMENTED  = 3

} uasms_awi_flag_e_type;



/* Message record state
*/
/* NOTE:
   For MO msg: IDLE->QUEUED->LAERY2_IN_PROGRESS->WAIT_FOR_ACK->IDLE
   For MT msg: IDLE->WAIT_FOR_ACK->QUEUED->LAYER2_IN_PROGRESS->IDLE
*/
typedef enum
{
  UASMS_MSG_IDLE   = 0,
  UASMS_MSG_QUEUED,
  UASMS_MSG_LAYER2_IN_PROGRESS,
  UASMS_MSG_WAIT_FOR_ACK

} uasms_msg_state_e_type;


/* Message record
*/
typedef struct
{
  uasms_msg_state_e_type         state;

  byte                           seq_num;

  void                           * user_data;

  uasms_format_e_type            format;

  uasms_address_type             address;

  uasms_subaddress_type          subaddress;

  /* each message has its own timer to wake up the SMS task
  */
  rex_timer_type                 timer;

} uasms_message_record_type;



/* Command header
*/
typedef struct
{
  q_link_type          link;         /* Queue link */
  rex_tcb_type         *task_ptr;    /* Pointer to requesting task TCB */
  rex_sigs_type        sigs;         /* Signal to set upon cmd completion */
  q_type               *done_q_ptr;  /* Queue to place this cmd on when done */

  uasms_cmd_id_e_type  id;
  /* cmd status ?? */

} uasms_cmd_hdr_type;



/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type             hdr;

  uasms_message_listener_type    msg_l;
  uasms_status_listener_type     status_l;
  uasms_event_listener_type      event_l;

} uasms_cmd_conf_listeners_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  void                        * user_data;
  uasms_client_message_type   cl_msg;

  uasms_message_record_type   * rec_ptr;   /* used only for pending msg */

} uasms_cmd_send_msg_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_transaction_id_type   tid;
  uasms_error_class_e_type    error_class;
  uasms_status_e_type         status;

  uasms_message_record_type   * rec_ptr;   /* used only for pending msg */

} uasms_cmd_ack_msg_type;



/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uns32                       timeout;

} uasms_cmd_enable_auto_disc_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

} uasms_cmd_disable_auto_disc_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_so_e_type             so;

} uasms_cmd_dtc_connect_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

} uasms_cmd_dtc_disc_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_OTA_message_type      ota;

} uasms_cmd_mc_mt_msg_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_error_class_e_type    error_class;
  uasms_status_e_type         status;

} uasms_cmd_mc_mo_status_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

} uasms_cmd_mc_state_changed_type;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  byte                        msg_count;

} uasms_cmd_mc_mwi;


/* cmd
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  cm_call_cmd_e_type          call_cmd;
  cm_call_cmd_err_e_type      call_cmd_err;

} uasms_cmd_call_error_type;



#ifdef FEATURE_BROADCAST_SMS
/* Each of the following structures corresponds to an API function
*/
typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

} uasms_cmd_bc_get_config_type;

typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

} uasms_cmd_bc_get_pref_type;

typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_pref_e_type        bc_pref;

} uasms_cmd_bc_set_pref_type;

typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

} uasms_cmd_bc_get_table_type;

typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_service_id_type    srv_id;
  boolean                     selected;
  uasms_priority_e_type       priority;

} uasms_cmd_bc_select_srv_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

} uasms_cmd_bc_get_all_srv_ids_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_service_id_type    srv_id;

} uasms_cmd_bc_get_srv_info_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_service_info_type  srv_info;

} uasms_cmd_bc_add_srv_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_service_id_type    srv_id;

} uasms_cmd_bc_delete_srv_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  uasms_bc_cmd_cb_type        cmd_cb;

  uasms_bc_service_id_type    srv_id;
  char                        label[UASMS_BC_SRV_LABEL_SIZE];

} uasms_cmd_bc_change_label_type;


typedef struct
{
  uasms_cmd_hdr_type          hdr;

  cm_sms_cmd_err_e_type       sms_cmd_err;

} uasms_cmd_bc_enable_failure_type;
#endif /* FEATURE_BROADCAST_SMS */


/* Command structure
*/
typedef union
{
  uasms_cmd_hdr_type                        hdr;

  /* commands from API functions
  */
  uasms_cmd_conf_listeners_type             conf_listeners;
  uasms_cmd_send_msg_type                   send_msg;
  uasms_cmd_ack_msg_type                    ack_msg;
  uasms_cmd_enable_auto_disc_type           enable_auto_disc;
  uasms_cmd_disable_auto_disc_type          disable_auto_disc;
  uasms_cmd_dtc_connect_type                dtc_connect;
  uasms_cmd_dtc_disc_type                   dtc_disc;

#ifdef FEATURE_BROADCAST_SMS
  uasms_cmd_bc_get_config_type              bc_get_config;
  uasms_cmd_bc_get_pref_type                bc_get_pref;
  uasms_cmd_bc_set_pref_type                bc_set_pref;
  uasms_cmd_bc_get_table_type               bc_get_table;
  uasms_cmd_bc_select_srv_type              bc_select_srv;
  uasms_cmd_bc_enable_failure_type          bc_enable_failure;
  uasms_cmd_bc_get_all_srv_ids_type         bc_get_ids;
  uasms_cmd_bc_get_srv_info_type            bc_get_srv_info;
  uasms_cmd_bc_add_srv_type                 bc_add_srv;
  uasms_cmd_bc_delete_srv_type              bc_delete_srv;
  uasms_cmd_bc_change_label_type            bc_change_label;
#endif /* FEATURE_BROADCAST_SMS */

  /* commands from MC 
  */
  uasms_cmd_mc_mt_msg_type                  mt_msg;
  uasms_cmd_mc_mo_status_type               mc_mo_status;
  uasms_cmd_mc_state_changed_type           mc_state_changed;
  uasms_cmd_mc_mwi                          mwi;

  /* events from CM
  */
  uasms_cmd_call_error_type                 call_error;

} uasms_cmd_type;



/* Struct for MC task to send sms
*/
typedef struct
{
  uasms_OTA_message_type     ota;      /* the raw SMS data to be sent */
  uasms_address_type         address;  /* SMS destination addr for auth. */

} uasms_cdma_sms_type;


#ifdef FEATURE_BROADCAST_SMS
/* Structure to keep all state variables about broadcast SMS
*/
typedef struct
{
  uasms_bc_config_e_type        bc_config;
  uasms_bc_pref_e_type          bc_pref;
  uint16                        bc_table_size;
  uint16                        bc_num_active; /* how many srv's active */
  nv_sms_bc_service_table_type  bc_table[UASMS_BC_TABLE_MAX];

  uint16                        bc_num_selected; /* how many srv's selected */
  boolean                       bc_enable; /* BC is actually enabled or not */
} uasms_bc_info_type;
#endif /* FEATURE_BROADCAST_SMS */


/* <EJECT> */
/*===========================================================================
                             
                          FUNCTION DECLARATIONS
                             
===========================================================================*/


/*=========================================================================
FUNCTION
  uasms_process_mt_msg

DESCRIPTION
  This function receives an SMS message and sends a command to the SMS task
  for processing.
  For Analog AWI SMS messages, segments are assembled into a complete
  message before being sent to the SMS task and an ack order is returned
  for intermediate segments.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  Add a command to the command queue. AWI SMS info is updated for AWI SMS
  segments.

=========================================================================*/
extern void  uasms_process_mt_msg
(
  uasms_format_e_type    format,

  /* length of sms message in bytes */
  word                   sms_len,

  /* Pointer to received (mobile-terminated) SMS message */
  byte                   * sms_ptr
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_mc_mo_status

DESCRIPTION
  This function reports the status of SMS data burst origination to the SMS.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  Add a command to the SMS command queue.

=========================================================================*/
extern void uasms_mc_mo_status
(
  uasms_error_class_e_type  error_class,
  uasms_status_e_type       status
);



/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_mc_state_changed

DESCRIPTION
  This function notifies the SMS task that the MC's state has changed.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  Add a command to the SMS command queue.

=========================================================================*/
extern void uasms_mc_state_changed
(
  void
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_task

DESCRIPTION
  The entry function for the SMS task. It does task initialization,
  waits for signals and processes the signals for commands and timers.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern void  uasms_task
(
  dword dummy
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_cmd

DESCRIPTION
  Get a command buffer from the command free queue and send a command to
  the SMS task.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  Add a command to the command queue.

=========================================================================*/
extern void uasms_cmd
(
  uasms_cmd_type * cmd_ptr
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_init

DESCRIPTION
  This function initializes all the internal data of the SMS task, including
  command queues, message records, timers, signals, CM client, etc.

DEPENDENCIES
  None

RETURN VALUE
  Status of the init.

SIDE EFFECTS
  Internal data is initialized.

=========================================================================*/
uasms_status_e_type uasms_init( void );


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_init_cm

DESCRIPTION
  This function initializes all the internal data that is related
  to the use of CM.

DEPENDENCIES
  None

RETURN VALUE
  Status of the init.

SIDE EFFECTS
  Internal data is initialized.

=========================================================================*/
uasms_status_e_type uasms_init_cm( void );



/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_process_signals

DESCRIPTION
  Process the signals for SMS related commands and timers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void uasms_process_signals
(
  rex_sigs_type              sigs
);

#ifndef FEATURE_NEWUASMS
/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_reset_seq_num

DESCRIPTION
  Reset internal count for the SMS TL sequence number.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  sequence number is reset to 0.

=========================================================================*/
void uasms_reset_seq_num
(
  void
);
#endif /* FEATURE_NEWUASMS */

/* <EJECT>^L */
/*=========================================================================
FUNCTION
  uasms_DTC_connect_i

DESCRIPTION
  This function requests to establish a DTC connection. It will queue
  the request and signal the SMS task to process it. Once the DTC connection
  is established or fails, an event will be sent to the client.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_DTC_connect_i
(
  uasms_so_e_type so
);



#ifdef FEATURE_BROADCAST_SMS
/* <EJECT>^L */
/*=========================================================================
FUNCTION
  uasms_bc_addr_match

DESCRIPTION
  This function checks the BC_ADDR of a received broadcast page or
  broadcast message to see if it passes the filter.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - the mobile should process the received message
  FALSE - the mobile should discard the received message

SIDE EFFECTS
  None

=========================================================================*/
extern boolean uasms_bc_addr_match
(
  byte *bc_addr_ptr
);
#endif /* FEATURE_BROADCAST_SMS */


#endif /* UASMSI_H */

#endif /* FEATURE_UASMS */

