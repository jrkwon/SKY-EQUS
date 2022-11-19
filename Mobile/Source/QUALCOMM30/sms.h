#ifdef FEATURE_SMS
#ifndef SMS_H
#define SMS_H
/*===========================================================================

                  S M S   D A T A   S T R U C T U R E S

DESCRIPTION
  This file contains data structures and constants which represent
  the external SMS message formats and values transmitted between the
  mobile and base stations.  These message formats are only used
  between the Main Control and TX/RX tasks.

Copyright (c) 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   L:/src/asw/COMMON/vcs/sms.h_v   1.2   14 Dec 2000 13:13:58   chonda  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/14/00   cah     Removed uasmsi.h inclusion.
12/13/00   cah     Replaced obsolete smsi.h inclusion with uasmsi.h.
05/10/99   kmp     Changed T_SMS to FEATURE_SMS
01/25/99   br      Packed Unions for ARM Support.
10/19/98   abh     Packed Structures  for ARM Support.
07/05/98   skk     Added VMN NV reservation. cr4289,4613,5081
08/02/95   day     Change sms_process_mt_msg parameter to pass by address
                   instead by value.  Code optimization eliminated the
                   code which updated this value.
06/29/95   day     Add #ifdef T_SMS to compile file with/without SMS code.
06/25/95   day     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"               /* Basic definitions */
#include "cai.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     DEFINITIONS FOR ALERT WITH INFO SMS SEGMENT VALUES                  */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#define SMS_AWI_INTERMEDIATE 0
#define SMS_AWI_FINAL        1
#define SMS_AWI_FIRST        2
#define SMS_AWI_UNSEGMENTED  3

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     TYPES FOR SMS PARAMETER VALUES                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-637 Section 4.4 SMS Teleservice Layer Message Types */
typedef enum
{
  SMS_RESERVED = 0,   /* IS-637 (Reserved)                           */
  SMS_DELIVER,        /* IS-637 SMS Deliver Message                  */
  SMS_SUBMIT,         /* IS-637 SMS Submit Message                   */
  SMS_CANCEL,         /* IS-637 SMS Cancellation Message             */
  SMS_DELIVERY_ACK,   /* IS-637 SMS Delivery Acknowledgment Message  */
  SMS_USER_ACK        /* IS-637 SMS User Acknowledgment Message      */
} sms_telesrv_msg_type;

/* See IS-91 Table 9 - Extended Protocol (EP) Message Types  */
typedef enum
{
  SMS_VOICE_MAIL = 130,       /* IS-91 Voice Mail          */
  SMS_SHORT_MSG_FULL,         /* IS-91 Short Message Full  */
  SMS_CLI,                    /* IS-91 CLI Order           */
  SMS_SHORT_MSG               /* IS-91 Short Message       */
} sms_ep_mst_type;

/* See IS-637 Table 3.4.3-1 Transport Layer Parameter Identifiers    */
typedef enum
{
  SMS_TL_TELESERVICE_ID = 0,  /* Teleservice Identifier     */
  SMS_TL_BC_SRV_CATEGORY,     /* Broadcast Service Category */
  SMS_TL_ORIG_ADDRESS,        /* Originating Address        */
  SMS_TL_ORIG_SUBADDRESS,     /* Originating Subaddress     */
  SMS_TL_DEST_ADDRESS,        /* Destination Address        */
  SMS_TL_DEST_SUBADDRESS,     /* Destination Subaddress     */
  SMS_TL_BEARER_REPLY_OPTION, /* Bearer Reply Option        */
  SMS_TL_CAUSE_CODES,         /* Cause Codes                */
  SMS_TL_BEARER_DATA          /* Bearer Data                */
} sms_tl_param_id_type;

/* See IS-637 Table 4.5-1 Bearer Data Subparameter Identifiers    */
typedef enum
{
  SMS_BD_MSG_ID = 0,    /* Message Identifier                */
  SMS_BD_USER_DATA,     /* User Data                         */
  SMS_BD_USER_RESP,     /* User Response Code                */
  SMS_BD_MC_TIME,       /* Message Center Time Stamp         */
  SMS_BD_VALID_ABS,     /* Validity Period - Absolute        */
  SMS_BD_VALID_REL,     /* Validity Period - Relative        */
  SMS_BD_DEFER_ABS,     /* Deferred Delivery Time - Absolute */
  SMS_BD_DEFER_REL,     /* Deferred Delivery Time - Relative */
  SMS_BD_PRIORITY,      /* Priority Indicator                */
  SMS_BD_PRIVACY,       /* Privacy Indicator                 */
  SMS_BD_REPLY_OPTION,  /* Reply Option                      */
  SMS_BD_NUM_OF_MSGS,   /* Number of Messages                */
  SMS_BD_ALERT,         /* Alert on Message Delivery         */
  SMS_BD_LANGUAGE,      /* Language Indicator                */
  SMS_BD_CALL_BACK      /* Call Back Number                  */
} sms_bd_param_id_type;

/* See IS-41C Table 177 SMS Teleservice Identifier values which are      */
/* supported in IS-637.  (See Table 3.4.3.1-1)                           */
typedef enum
{
  SMS_CMT_91_TELESRV = 4096, /* AMPS Extended Protocol Enhanced Services */
  SMS_CPT_95_TELESRV,        /* CDMA Cellular Paging Teleservice         */
  SMS_CMT_95_TELESRV,        /* CDMA Cellular Messaging Teleservice      */
  SMS_VMN_95_TELESRV         /* CDMA Voice Mail Notification             */
} sms_telesrv_id_type;

/* SMS Message External Parsing Format Types                             */
/* Used to identify type of SMS message translation should be performed. */
typedef enum
{
  SMS_C_MSG,           /* CMT-95, CPT-95, VMN-95, CMT-91                   */
  SMS_A_CLI,           /* IS-91 Extended Protocol - CLI Order  (analog)    */
  SMS_A_VOICE_MAIL,    /* IS-91 Extended Protocol - Voice Mail (analog)    */
  SMS_A_SHORT_MSG,     /* IS-91 Extended Protocol - Short Message (analog) */
  SMS_A_ALERT_W_INFO   /* IS-95 Alert With Info SMS (analog)               */
} sms_parse_type;

/* Define number of bytes in analog IS-91 SMS message format. */
/* Includes Mobile Station Control Message header word and    */
/* subsequent data words.  SMS message formats for CDMA IS-91 */
/* are different than their analog counterparts, so these     */
/* lengths are relevant to analog formats only.               */
#define SMS_A_CLI_LEN 40
#define SMS_A_VOICE_MAIL_LEN 30
#define SMS_A_SHORT_MSG_LEN 30

/* SMS Function Return Status Types                  */
/* Used to identify status of SMS message processing */
typedef enum
{
  SMS_DONE_S,          /* SMS processing complete, no msgs to be sent      */
  SMS_SPACE_UNAVAIL_S, /* Unable to store SMS message, send status to UI   */
  SMS_ACK_RDY_S,       /* SMS transport layer ack ready for transmit       */
  SMS_MSG_RDY_S,       /* Translated SMS message ready to send to UI       */
  SMS_MSG_ACK_RDY_S,   /* Translated SMS message and SMS transport layer   */
                       /* ack ready to send to UI and for transmit         */
  SMS_STS_ACK_RDY_S    /* SMS transport layer ack ready for transmit       */
                       /* and send space unavailable status to UI          */
} sms_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*           EXTERNAL TYPES USED FOR BIT-PACKED SMS MESSAGES               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* For bit-representation (position and size of message fields) purposes,  */
/* each byte represents one bit.  External formats define the bit position */
/* and size of each field of the bit-packed SMS message.  When the SMS     */
/* message is unpacked, internal message formats are used to represented   */
/* message data on byte-boundaries.  Bit-packing and bit-unpacking         */
/* routines utilize both definitions to translate SMS message data to and  */
/* from bit-packed and byte-aligned formats.                               */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GENERIC PARAMETER HEADER TYPE                                           */

/* SMS Message Type   */
typedef byte sms_msg_id_type[8];

/* SMS Parameter/Subparameter Header */
typedef PACKED struct
{
  byte param_id[8];
  byte param_len[8];
} sms_param_hdr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IS-91 EXTENDED PROTOCOL SMS PARAMETERS                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* IS-91 EXTENDED PROTOCOL SMS PARAMETER DEFINITIONS               */
/* See IS-91 Section 3.7.2.1 for parameter bit positions and sizes */

/* Extended Protocol Message Length */
typedef byte sms_ep_msl[5];

/* Extended Protocol Message Type */
typedef byte sms_ep_mst[8];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* TELESERVICE LAYER BEARER DATA (BD) SMS PARAMETERS AND MESSAGES          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* TELESERVICE LAYER BEARER DATA (BD) PARAMETER DEFINITIONS     */
/* See IS-637 Section 4.5 for parameter bit positions and sizes */

/* Message Identifier */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte message_type[4];
  byte message_id[16];
  byte reserved[4];
} sms_bd_msg_id_type;

/* User Data */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte msg_encoding[5];
  byte mst[8];
  byte num_fields[8];
} sms_bd_user_data_fix_type;

typedef union
{
  byte digit4[4];  /* IS-91 bit representation of data (4-bit)  */
  byte digit6[6];  /* IS-91 bit representation of data (6-bit)  */
  byte digit7[7];  /* 7-bit ASCII/IA5 representation of data    */
  byte digit8[8];  /* Unspecified 8-bit representation of data  */
} sms_bd_user_data_var_type;

/* Maximum number of digits is limited by the largest data burst
   message size (Reverse Traffic Channel) */
#define SMS_USER_DATA_MAX  (CAI_REV_TC_BURST_MAX -         \
   sizeof( sms_msg_id_type ) - sizeof( sms_param_hdr_type ) - \
   sizeof( sms_bd_user_data_fix_type ))

typedef PACKED struct
{
  sms_bd_user_data_fix_type fix;
  sms_bd_user_data_var_type var[SMS_USER_DATA_MAX];
} sms_bd_user_data_type;

/* User Response Code */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte resp_code[8];
} sms_bd_user_resp_type;

/* Message Center Time Stamp         */
/* Validity Period - Absolute        */
/* Deferred Delivery Time - Absolute */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte yr[8];
  byte mon[8];
  byte day[8];
  byte hrs[8];
  byte min[8];
  byte sec[8];
} sms_bd_abs_time_type;

/* Validity Period - Relative       */
/* Deferred Delivery Time -Relative */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte time[8];
} sms_bd_rel_time_type;

/* Priority Indicator */
/* Privacy Indicator  */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte indicator[2];
  byte reserved[6];
} sms_bd_indicator_type;

/* Reply Option */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte user_ack_req;
  byte dak_req;
  byte reserved[6];
} sms_bd_reply_opt_type;

/* Number Of Messages  */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte msg_cnt[8];
} sms_bd_num_of_msg_type;

/* Alert On Message Delivery */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
} sms_bd_alert_type;

/* Language Indicator */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte language[8];
} sms_bd_language_type;

/* Call Back Number */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte digit_mode;
  byte num_type[3];
  byte num_plan[4];
  byte num_fields[8];
} sms_bd_call_back_fix_type;

typedef union
{
  byte digit4[4];  /* 4 bit representation of the digit */
  byte digit8[8];  /* ASCII representation of the digit */
} sms_bd_call_back_var_type;

/* Maximum number of dialing digits is 32 */
#define SMS_CALL_BACK_MAX  32

typedef PACKED struct
{
  sms_bd_call_back_fix_type fix;
  sms_bd_call_back_var_type var[SMS_CALL_BACK_MAX];
} sms_bd_call_back_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*     EXTERNAL TELESERVICE LAYER BEARER DATA (BD) SMS MESSAGE FORMATS     */
/* External Bearer Data SMS Deliver Message Format */
typedef PACKED struct
{
  sms_bd_user_data_type  user_data;  /* User Data                          */
  sms_bd_abs_time_type   mc_time;    /* Message Center Time Stamp          */
  sms_bd_abs_time_type   valid_abs;  /* Validity Period - Absolute         */
  sms_bd_rel_time_type   valid_rel;  /* Validity Period - Relative         */
  sms_bd_abs_time_type   defer_abs;  /* Deferred Delivery Time - Absolute  */
  sms_bd_rel_time_type   defer_rel;  /* Deferred Delivery Time - Relative  */
  sms_bd_indicator_type  priority;   /* Priority Indicator                 */
  sms_bd_indicator_type  privacy;    /* Privacy Indicator                  */
  sms_bd_reply_opt_type  reply_opt;  /* Reply Option                       */
  sms_bd_num_of_msg_type num_of_msg; /* Number Of Messages                 */
  sms_bd_alert_type      alert;      /* Alert On Message Delivery          */
  sms_bd_language_type   lang;       /* Language Indicator                 */
  sms_bd_call_back_type  call_back;  /* Call Back Number                   */
} sms_bd_deliver_msg_type;

/* External Bearer Data SMS Submit Message Format */
typedef PACKED struct
{
  sms_bd_user_data_type  user_data;  /* User Data                          */
  sms_bd_abs_time_type   valid_abs;  /* Validity Period - Absolute         */
  sms_bd_rel_time_type   valid_rel;  /* Validity Period - Relative         */
  sms_bd_abs_time_type   defer_abs;  /* Deferred Delivery Time - Absolute  */
  sms_bd_rel_time_type   defer_rel;  /* Deferred Delivery Time - Relative  */
  sms_bd_indicator_type  priority;   /* Priority Indicator                 */
  sms_bd_indicator_type  privacy;    /* Privacy Indicator                  */
  sms_bd_reply_opt_type  reply_opt;  /* Reply Option                       */
  sms_bd_alert_type      alert;      /* Alert On Message Delivery          */
  sms_bd_language_type   lang;       /* Language Indicator                 */
  sms_bd_call_back_type  call_back;  /* Call Back Number                   */
} sms_bd_submit_msg_type;

/* External Bearer Data SMS Cancellation Message Format      */
/* contains only the message identifier which is declared in */
/* sms_bd_msg_type for all bearer data message types         */

/* External Bearer Data SMS User Acknowledgment Message Format */
typedef PACKED struct
{
  sms_bd_user_data_type  user_data;  /* User Data                  */
  sms_bd_user_resp_type  user_resp;  /* User Response Code         */
  sms_bd_abs_time_type   mc_time;    /* Message Center Time Stamp  */
} sms_bd_user_ack_msg_type;

/* External Bearer Data SMS Delivery Acknowledgment Message Format */
typedef PACKED struct
{
  sms_bd_user_data_type  user_data;  /* User Data                  */
  sms_bd_abs_time_type   mc_time;    /* Message Center Time Stamp  */
} sms_bd_delivery_ack_msg_type;

/* External Teleservice Layer Bearer Data (BD) SMS Message */
/* Formats for CMT-95, CPT-95 and VMN-95 teleservices      */
typedef PACKED struct
{
  sms_bd_msg_id_type             msg_id;   /* Bearer Data Message Id       */
  PACKED union {
    sms_bd_deliver_msg_type      deliver;  /* Bearer Data SMS Deliver      */
    sms_bd_submit_msg_type       submit;   /* Bearer Data SMS Submit       */
    sms_bd_user_ack_msg_type     user_ack; /* Bearer Data SMS User Ack     */
    sms_bd_delivery_ack_msg_type del_ack;  /* Bearer Data SMS Delivery Ack */
  } var;
} sms_bd_95_msg_type;

/* External Teleservice Layer Bearer Data (BD) SMS Message */
typedef PACKED struct
{
  sms_param_hdr_type             hdr;      /* Bearer Data Parameter Header */
  PACKED union {
    sms_bd_user_data_type        data_91;  /* Bearer Data - CMT-91 format  */
    sms_bd_95_msg_type           data_95;  /* Bearer Data - CMT/CPT/VMN-95 */
  } var;
} sms_bd_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SMS TRANSPORT LAYER (TL) PARAMETERS AND MESSAGES                        */
/* (see IS-637 Section 3.4)                                                */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTERNAL TRANSPORT LAYER (TL) PARAMETER DEFINITIONS                     */
/* See IS-637 Section 3.4.3 for parameter bit positions and sizes          */

/* Teleservice Identifier */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte teleservice[16];
} sms_tl_teleservice_type;

/* Broadcast Service Category */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte bc_srv_cat[16];
} sms_tl_bc_srv_cat_type;

/* Address Parameters */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte digit_mode;
  byte num_mode;
  byte num_type[3];
  byte num_plan[4];
  byte num_fields[8];
} sms_tl_address_fix_type;

typedef union
{
  byte digit4[4];  /* 4 bit representation of the digit */
  byte digit8[8];  /* ASCII representation of the digit */
} sms_tl_address_var_type;

/* Maximum number of digits is limited by the largest data burst
   message size (Reverse Traffic Channel) */
#define SMS_TL_ADDRESS_MAX  (CAI_REV_TC_BURST_MAX -           \
   sizeof( sms_msg_id_type ) - sizeof( sms_param_hdr_type ) - \
   sizeof( sms_tl_address_fix_type ))

typedef PACKED struct
{
  sms_tl_address_fix_type fix;
  sms_tl_address_var_type var[SMS_TL_ADDRESS_MAX];
} sms_tl_address_type;

/* Subaddress */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte type[3];
  byte odd;
  byte num_fields[8];
} sms_tl_subaddress_fix_type;

typedef PACKED struct
{
  sms_tl_subaddress_fix_type fix;
  sms_tl_address_var_type var[SMS_TL_ADDRESS_MAX];
} sms_tl_subaddress_type;

/* Bearer Reply Option */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte reply_seq[6];
  byte reserved[2];
} sms_tl_bearer_reply_type;

/* Cause Codes */
typedef PACKED struct
{
  sms_param_hdr_type hdr;
  byte reply_seq[6];
  byte error_class[2];
  byte cause_code[8];
} sms_tl_cause_codes_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*           EXTERNAL TRANSPORT LAYER (TL) MESSAGE FORMATS                 */
/* External SMS Point-to-Point Message Format */
typedef PACKED struct
{
  sms_tl_teleservice_type  tel_id;  /* Teleservice Identifier             */
  sms_tl_address_type      addr;    /* Originating/Destination Address    */
  sms_tl_subaddress_type   subaddr; /* Originating/Destination Subaddress */
  sms_tl_bearer_reply_type reply;   /* Bearer Reply Option                */
  sms_bd_msg_type          data;    /* Bearer Data                        */
} sms_tl_ptp_msg_type;

/* External SMS Broadcast Message Format */
typedef PACKED struct
{
  sms_tl_bc_srv_cat_type bc_srv;  /* Broadcast Service Category          */
  sms_bd_msg_type        data;    /* Bearer Data                         */
} sms_tl_bc_msg_type;

/* External SMS Acknowledge Message Format */
typedef PACKED struct
{
  sms_tl_address_type     addr;    /* Destination Address                 */
  sms_tl_subaddress_type  subaddr; /* Destination Subaddress              */
  sms_tl_cause_codes_type cause;   /* Bearer Reply Option                 */
} sms_tl_ack_msg_type;

/* External Transport Layer SMS Message Format - CDMA teleservices */
typedef PACKED struct
{
  sms_msg_id_type       msg_id;  /* Transport Layer SMS Message Type   */
  PACKED union {
    sms_tl_ptp_msg_type ptp;     /* Transport Layer SMS Point-to-Point */
    sms_tl_bc_msg_type  bc;      /* Transport Layer SMS Broadcast      */
    sms_tl_ack_msg_type ack;     /* Transport Layer SMS Acknowledge    */
  } var;
} sms_cdma_msg_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* Procedure which processes mobile-terminated (MT) SMS messages     */
/* and determines if message is ready to be translated and generates */
/* which messages should be sent.                                    */
extern word sms_process_mt_msg
(
  word sms_len,
    /* length of sms message in bytes */
  sms_parse_type  sms_parse,
    /* sms message parsing type for external formats */
  byte *sms_ptr,
    /* Pointer to received (mobile-terminated) SMS message */
  smsi_msg_type *sms_msg_ptr,
    /* Pointer to translated SMS message */
  byte *ack_len,
    /* Pointer to byte length of SMS acknowledgment message */
  byte *ack_ptr
    /* Pointer to SMS acknowledgment message to be sent if required */
);
/*===========================================================================
FUNCTION CALC_SMS_NV_DATA
had to externalize becasue of VMN NV reservation
DESCRIPTION
  Calculate the packed SMS data that will be stored in NV.  Return
  number of bytes to be used for packed data.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern word calc_sms_nv_data
(
  smsi_msg_type *unpack_data_ptr
);

#endif /* SMS_H */
#endif /* FEATURE_SMS */

