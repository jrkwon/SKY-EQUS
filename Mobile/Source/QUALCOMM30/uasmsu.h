#ifndef UASMSU_H
#define UASMSU_H
#ifdef FEATURE_UASMS
/*===========================================================================

                        U A S M S   U T I L I T I E S

DESCRIPTION
  This file contains data structures, constants and function declarations
  which define the internal SMS message format and the conversions 
  between the SMS API format and the internal format.

Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   L:/src/asw/COMMON/vcs/uasmsu.h_v   1.1   14 Dec 2000 08:57:40   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/13/00   HQ      Added functions uasmsu_unpack_ascii and uasmsu_dtmf2ascii;
                   make smsi_srvc_cat_type always defined.
10/23/00   HQ      Initial version.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"               /* Basic definitions */
#include "uasms.h"
#include "nv.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*     SMS INTERNAL PARAMETER MASK DEFINITIONS                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*                INTERNAL SMS PARAMETER FLAGS                       */
/* In the internal SMS message format, a 3 byte mask is provided to  */
/* represent which parameters are specified in the SMS message.      */
#define SMSI_NO_PARAMS       0x000000 /* Indicates no parameters specified */
                                      /* in SMS message                    */

#define SMSI_BEARER_REPLY    0x000001 /* Indicates internal SMS message    */
                                      /* contains a bearer reply parameter */

#define SMSI_VALID_ABS       0x000002 /* Indicates internal SMS message    */
                                      /* contains a valid period -         */
                                      /* absolute parameter                */

#define SMSI_VALID_REL       0x000004 /* Indicates internal SMS message    */
                                      /* contains a valid period -         */
                                      /* relative parameter                */

#define SMSI_DEFER_ABS       0x000008 /* Indicates internal SMS message    */
                                      /* contains a deferred delivery time */
                                      /* - absolute parameter              */

#define SMSI_DEFER_REL       0x000010 /* Indicates internal SMS message    */
                                      /* contains a deferred delivery time */
                                      /* - relative parameter              */

#define SMSI_USER_RESP       0x000020 /* Indicates internal SMS message    */
                                      /* contains a user response code     */
                                      /* parameter                         */

#define SMSI_CAUSE_CODES     0x000040 /* Indicates internal SMS message    */
                                      /* contains a cause codes parameter  */

#define SMSI_ORIG_ADDR       0x000080 /* Indicates internal SMS message    */
                                      /* contains originating address      */
                                      /* parameter                         */

#define SMSI_ORIG_SUBADDR    0x000100 /* Indicates internal SMS message    */
                                      /* contains originating subaddress   */
                                      /* parameter                         */

#define SMSI_DEST_ADDR       0x000200 /* Indicates internal SMS message    */
                                      /* contains destination address      */
                                      /* parameter                         */

#define SMSI_DEST_SUBADDR    0x000400 /* Indicates internal SMS message    */
                                      /* contains destination subaddress   */
                                      /* parameter                         */

#define SMSI_USER_DATA       0x000800 /* Indicates internal SMS message    */
                                      /* contains a user data parameter    */

#define SMSI_PRIORITY        0x001000 /* Indicates internal SMS message    */
                                      /* contains a priority indicator     */
                                      /* parameter                         */

#define SMSI_PRIVACY         0x002000 /* Indicates internal SMS message    */
                                      /* contains a privacy indicator      */
                                      /* parameter                         */

#define SMSI_REPLY_OPTION    0x004000 /* Indicates internal SMS message    */
                                      /* contains a reply option           */
                                      /* parameter                         */

#define SMSI_NUM_OF_MSGS     0x008000 /* Indicates internal SMS message    */
                                      /* contains a number of messages     */
                                      /* parameter                         */

#define SMSI_ALERT           0x010000 /* Indicates alert on SMS message    */
                                      /* delivery (no parameter reqd)      */

#define SMSI_LANGUAGE        0x020000 /* Indicates internal SMS message    */
                                      /* contains a language indicator     */
                                      /* parameter                         */

#define SMSI_CALL_BACK       0x040000 /* Indicates internal SMS message    */
                                      /* contains a call back number       */
                                      /* parameter                         */

#define SMSI_MC_TIME_STAMP   0x080000 /* Indicates internal SMS message    */
                                      /* contains a Message Center time    */
                                      /* stamp parameter                   */

#define SMSI_BEARER_DATA     0x100000 /* Indicates internal SMS message    */
                                      /* contains a bearer data parameter  */

#define SMSI_MSG_ID          0x200000 /* Indicates internal SMS message    */
                                      /* contains the msg ID parameter     */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     TYPES FOR SMS INTERNAL PARAMETER VALUES                             */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-637 Section 3.4.2 SMS Transport Layer Message Categories */
typedef enum
{
  SMS_PTP = 0,     /* IS-637 SMS Point-to-Point Message */
  SMS_BC,          /* IS-637 SMS Broadcast Message      */
  SMS_ACK          /* IS-637 SMS Acknowledge Message    */
} smsi_msg_cat_type;

/* See IS-637 Section 4.4.1 SMS Teleservice types                    */
/* Used internally between MC and UI tasks for SMS message parsing.  */
typedef enum
{
  SMSI_CPT_95,             /* IS-95A Cellular Paging                  */
  SMSI_CMT_95,             /* IS-95A Cellular Messaging               */
  SMSI_VMN_95,             /* IS-95A Voice Mail Notification          */
  SMSI_CMT_91_CLI,         /* IS-91 Extended Protocol - CLI Order     */
  SMSI_CMT_91_VOICE_MAIL,  /* IS-91 Extended Protocol - Voice Mail    */
  SMSI_CMT_91_SHORT_MSG,   /* IS-91 Extended Protocol - Short Message */
  SMSI_AWI_95              /* IS-95A Alert With Info                  */
} smsi_teleservice_type;

/* <EJECT> */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*               INTERNAL TYPES USED FOR SMS MESSAGES                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Internal message formats are used to represented SMS message data on    */
/* byte-aligned boundaries.  Bit-packing and bit-unpacking routines        */
/* utilize these definitions to translate SMS message data to and from     */
/* bit-packed formats.                                                     */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* INTERNAL SMS PARAMETERS AND MESSAGES                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SMSI_MSG_MAX 255
#define SMSI_USER_DATA_MAX 255
#define SMSI_CALL_BACK_MAX 32
#define SMSI_TL_ADDRESS_MAX 50

typedef uint16 smsi_srvc_cat_type;

/* Alert With Info SMS info_data field */
typedef byte smsi_awi_data_type[SMSI_MSG_MAX];

/* Parameter Header */
typedef PACKED struct
{
  byte parm_type;
  byte parm_len;
} smsi_param_hdr_type;

/* Message Identifier */
typedef PACKED struct
{
  byte message_type;
  word message_id;
} smsi_msg_id_type;

/* User Data */
typedef PACKED struct
{
  byte msg_encoding;
  byte mst;
#ifdef FEATURE_RUIM_SMS
  byte data_len;
  byte padding_bits;
#endif
  byte num_fields;
  byte chari[SMSI_USER_DATA_MAX];
#ifdef FEATURE_DS_UP
#error code not present
#endif
} smsi_user_data_type;

/* User Response Code */
typedef byte smsi_user_resp_type;

/* Message Center Time Stamp         */
/* Validity Period - Absolute        */
/* Deferred Delivery Time - Absolute */
typedef PACKED struct
{
  byte yr;
  byte mon;
  byte day;
  byte hrs;
  byte min;
  byte sec;
} smsi_abs_time_type;

/* Validity Period - Relative        */
/* Deferred Delivery Time - Relative */
typedef byte smsi_rel_time_type;

/* Priority Indicator */
/* Privacy Indicator  */
typedef byte smsi_indicator_type;

/* Reply Option */
typedef PACKED struct
{
  byte user_ack_req;
  byte dak_req;
} smsi_reply_opt_type;

/* Number Of Messages  */
typedef byte smsi_num_of_msgs_type[2];

/* Language Indicator */
typedef byte smsi_language_type;

/* Call Back Number */
typedef PACKED struct
{
  byte digit_mode;
  byte num_type;
  byte num_plan;
  byte num_fields;
  byte chari[SMSI_CALL_BACK_MAX];
} smsi_call_back_type;


/* Address Parameters */
typedef PACKED struct
{
  byte digit_mode;
  byte num_mode;
  byte num_type;
  byte num_plan;
  byte num_fields;
  byte chari[SMSI_TL_ADDRESS_MAX];
} smsi_address_type;

/* Subaddress */
typedef PACKED struct
{
  byte type;
  byte odd;
  byte num_fields;
  byte chari[SMSI_TL_ADDRESS_MAX];
} smsi_subaddress_type;

/* Bearer Reply Option */
typedef byte smsi_bearer_reply_type;

/* Cause Codes */
typedef PACKED struct
{
  byte reply_seq;
  byte error_class;
  byte cause_code;
} smsi_cause_codes_type;

/* Used to hold bit-packed SMS message for transmit */
typedef struct
{
  byte len;
  byte chari[SMSI_USER_DATA_MAX];
} smsi_ext_msg_type;

/* Message tag
*/
typedef enum
{
  /* The enum values here match those in the R-UIM standard
     for easy translation
  */
  SMSI_INVALID       = 0,
  SMSI_MT_READ       = 1,
  SMSI_MT_NOT_READ   = 3,
  SMSI_MO_SENT       = 5,
  SMSI_MO_NOT_SENT   = 7,
  SMSI_MO_TEMPLATE   = 0x0100 

} smsi_msg_tag_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                  INTERNAL SMS MESSAGE FORMATS                           */

/* Internal SMS Message Data Format */
typedef struct
{
  smsi_address_type      address;      /* Orig/Dest Address            */
  smsi_subaddress_type   subaddress;   /* Orig/Dest Subaddress         */
  smsi_user_data_type    user_data;    /* User Data                    */
  smsi_indicator_type    priority;     /* Priority Indicator           */
  smsi_indicator_type    privacy;      /* Privacy Indicator            */
  smsi_reply_opt_type    reply_opt;    /* Reply Option                 */
  smsi_num_of_msgs_type  num_of_msgs;  /* Number Of Messages           */
  smsi_language_type     lang;         /* Language Indicator           */
  smsi_call_back_type    call_back;    /* Call Back Number             */
  smsi_abs_time_type     mc_time;      /* Message Center Time Stamp    */
  int                    ack_index;    /* which MO msg the del/user ack matches */
  boolean                alert_on_delivery; /* whether to alert user or not */
} smsi_msg_data_type;


/* Transport Layer SMS Message */
typedef struct
{
  smsi_teleservice_type  tele_srv;       /* Internal Teleservice Type    */
  smsi_msg_cat_type      sms_msg_cat;    /* SMS Message Category         */
  smsi_srvc_cat_type     sms_srvc_cat;   /* SMS service category       */
  dword                  sms_param_m;    /* SMS Parameter Mask           */
  smsi_msg_id_type       sms_msg_id;     /* SMS Message Identifier       */
  smsi_bearer_reply_type bearer_reply;   /* Bearer Reply                 */
  smsi_abs_time_type     valid_abs;      /* Validity Period - Absolute   */
  smsi_rel_time_type     valid_rel;      /* Validity Period - Relative   */
  smsi_abs_time_type     defer_abs;      /* Deferred Delivery Time - abs */
  smsi_rel_time_type     defer_rel;      /* Deferred Delivery Time - rel */
  smsi_user_resp_type    user_resp;      /* User Response Code           */
  smsi_cause_codes_type  cause_codes;    /* Cause Codes                  */
  smsi_msg_data_type     msg_data;       /* Message Data                 */
  smsi_msg_tag_type      tag;            /* Message Tag                  */
} smsi_msg_type;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_pack_ascii

DESCRIPTION
  Pack the 7 bits in each character of a null-terminated ASCII string into a
  a byte array.

DEPENDENCIES
  None

RETURN VALUE
  The string length.

SIDE EFFECTS
  None

===========================================================================*/
byte uasmsu_pack_ascii
(
  const char        * ascii_ptr,       /* IN */
  byte              * data,            /* OUT */
  byte              * data_len_ptr,    /* OUT */
  byte              * padding_bits_ptr /* OUT */
);


/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_unpack_ascii

DESCRIPTION
  Unpack the 7-bit ASCII string from the packed format in a byte array.

DEPENDENCIES
  None

RETURN VALUE
  The string length.

SIDE EFFECTS
  None

===========================================================================*/
byte uasmsu_unpack_ascii
(
  uasms_user_data_type    *ud,    /* IN */
  byte    buf_len,                /* IN */
  byte    *buf                    /* OUT */
);


/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_convert_to_smsi

DESCRIPTION
  Convert the SMS API client message to the internal SMS structure that
  is used by the UI.

  The user data digits are also decoded if FEATURE_RUIM_SMS is not defined.
  Under FEATURE_RUIM_SMS, the user data digits are not decoded.

DEPENDENCIES
  None

RETURN VALUE
  Status of the conversion.

SIDE EFFECTS
  None

===========================================================================*/
uasms_status_e_type uasmsu_convert_to_smsi
(
  const uasms_client_message_type   * cl_ptr,
  smsi_msg_type                     * smsi_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_ascii2dtmf

DESCRIPTION
  Convert the ASCII digit string to DTMF digits

DEPENDENCIES
  None

RETURN VALUE
  Number of digits converted

SIDE EFFECTS
  None

===========================================================================*/
byte uasmsu_ascii2dtmf
(
  char     *ascii,
  byte     *dtmf
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_dtmf2ascii

DESCRIPTION
  Convert the DTMF digits to ASCII digits.

DEPENDENCIES
  None

RETURN VALUE
  Number of digits converted

SIDE EFFECTS
  None

===========================================================================*/
byte uasmsu_dtmf2ascii
(
  byte     len,
  byte     *dtmf,
  byte     *ascii
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uasmsu_convert_from_smsi

DESCRIPTION
  Convert the SMS API client message from the internal SMS structure that
  is used by the UI.

DEPENDENCIES
  None

RETURN VALUE
  Status of the conversion.

SIDE EFFECTS
  None

===========================================================================*/
uasms_status_e_type uasmsu_convert_from_smsi
(
  smsi_msg_type                     * smsi_ptr,
  uasms_client_message_type         * cl_ptr
);

/* <EJECT> */
/*===========================================================================
FUNCTION uasmsu_nv_data

DESCRIPTION
  Pack or unpack SMS data stored in or retrieved from NV.

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void uasmsu_nv_data
(
  boolean       packing,
  nv_sms_type   *pack_data_ptr,
  smsi_msg_type *unpack_data_ptr
#ifdef FEATURE_RUIM_SMS
  , boolean     raw_user_data /* whether user data is in the raw format or */
                              /* in the decoded text format                */
#endif /* FEATURE_RUIM_SMS */
);

#endif /* FEATURE_UASMS */
#endif /* UASMSU_H */

