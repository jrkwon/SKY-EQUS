#ifdef FEATURE_UASMS  /* The SMS API */ 

#ifndef UASMS_H
#define UASMS_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           U A P I   S H O R T   M E S S A G E   S E R V I C E S (SMS API)

DESCRIPTION
  This module defines the data types and functions for the Short Message
  Services (SMS) that are used by the client software and are part of the
  UAPI (User Application Programming Interface). A separate header file,
  uasmsx.h, defines the data types and functions for bearer data encoding
  and decoding.

Copyright (c) 1998-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/uasms.h_v   1.3   31 Jan 2001 17:14:08   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/29/01   HQ      Properly feature-ized FEATURE_NEWUASMS.
10/09/00   HQ      Added message tag field in the client message.
08/25/00   vt      Moved uasms_reset_seq_num() from uasmsi.h.
                   Copied UASMS_DUMMY_SEQ_NUM from uasmsi.h.
                   Moved uasmsx_encode_bearer_data() from uasmsx.h.
                   Moved uasmsx_decode_bearer_data() from uasmsx.h.
05/19/00   HQ      Added padding_bits in uasms_user_data_type.
02/25/00   HQ      Added support for FEATURE_BROADCAST_SMS.

===========================================================================*/


#ifdef __cplusplus
extern "C" {
#endif


         
/* <EJECT> */
/*===========================================================================

                             INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "uapi.h"



/* <EJECT> */
/*===========================================================================
                             
                         DATA TYPE DECLARATIONS
                             
===========================================================================*/


/* ====================================================================== */
/* ---------------------------------------------------------------------- */
/* -------------- Definitions related to client message: ---------------- */
/* ---------------------------------------------------------------------- */
/* ====================================================================== */

enum { UASMS_MAX_LEN        =     253 };
enum { UASMS_ADDRESS_MAX    =     32  };
enum { UASMS_SUBADDRESS_MAX =     32  };

#ifdef FEATURE_NEWUASMS
enum { UASMS_DUMMY_SEQ_NUM  =     255 };
#endif

/* ------------------------ */
/* ---- Teleservice Id ---- */
/* ------------------------ */
typedef enum
{
  /*----------------------------------------------------------------
     The following values are used in CDMA mode and Analog AWI SMS
     as defined in IS-637.
  -----------------------------------------------------------------*/
  /* NOTE: In case of teleservice CM_91, the encoding type of the user data
     indicates whether the teleservice is actually CPT, VMN or CMT and the
     user data is extracted into the corresponding bearer data fields.
  */
  UASMS_TELESERVICE_CMT_91             = 4096,  /* embedded IS91 SMS */
  UASMS_TELESERVICE_CPT_95             = 4097,  /* page */
  UASMS_TELESERVICE_CMT_95             = 4098,  /* voice mail notification */
  UASMS_TELESERVICE_VMN_95             = 4099,  /* short message */

  ////////////////////////////////////////////////////////////////
  UASMS_TELESERVICE_SHINSEGI_MYBELL     = 61456,
  UASMS_TELESERVICE_SHINSEGI_MYBELL2    = 61457,
  UASMS_TELESERVICE_SHINSEGI_WAP_PUSH   = 61472,
  /////////////////////////////////////////////신세기 ////////////

  UASMS_TELESERVICE_MYBELL	 	       = 64000,	// MyBell
  UASMS_TELESERVICE_MYBELL2            = 64001,
  UASMS_TELESERVICE_MYBELL3            = 64002,
  UASMS_TELESERVICE_WAP_MAIL           = 65490,
  UASMS_TELESERVICE_WAP_PUSH           = 65491,
/* reo 2001.03.15 --} */
#ifdef FEATURE_SKVM
  UASMS_TELESERVICE_SKVM_LOAD          = 64100,
  UASMS_TELESERVICE_SKVM_DATA          = 64101,
#endif
/* reo 2001.03.15 {-- */
#ifdef FEATURE_SKTT_EQUS_LOCATION
  UASMS_TELESERVICE_LOCATION           = 65300, // 위치정보         	
#endif //FEATURE_SKTT_EQUS_LOCATION
/* reo 2001.03.15 --} */
/* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
  UASMS_TELESERVICE_GVM                 = 63000, // GVM
#endif
/* <-- */

  /*----------------------------------------------------------------
     The following are not defined as a teleservice in IS-637,
     but they are used to indicate the teleservice type in Analog mode.
  -----------------------------------------------------------------*/
  UASMS_TELESERVICE_IS91_PAGE          = 0,
  UASMS_TELESERVICE_IS91_VOICE_MAIL    = 1,
  UASMS_TELESERVICE_IS91_SHORT_MESSAGE = 2,


  /*----------------------------------------------------------------
     Voice mail notification through Message Waiting Indication in
     CDMA mode or Analog mode
  -----------------------------------------------------------------*/
  UASMS_TELESERVICE_MWI                = 3,


  UASMS_TELESERVICE_UNKNOWN            = 0xFFFF

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_teleservice_e_type;



/* -------------------- */
/* ---- Digit mode ---- */
/* -------------------- */
typedef enum
{
  UASMS_DIGIT_MODE_4_BIT     = 0,  /* DTMF digits */
  UASMS_DIGIT_MODE_8_BIT     = 1
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_digit_mode_e_type;


/* --------------------- */
/* ---- Number type ---- */
/* --------------------- */
typedef enum
{
  UASMS_NUMBER_UNKNOWN        = 0,

  UASMS_NUMBER_INTERNATIONAL  = 1,
  UASMS_NUMBER_NATIONAL       = 2,
  UASMS_NUMBER_NETWORK        = 3,
  UASMS_NUMBER_SUBSCRIBER     = 4,
  UASMS_NUMBER_RESERVED_5     = 5,
  UASMS_NUMBER_ABBREVIATED    = 6,
  UASMS_NUMBER_RESERVED_7     = 7,

  /* The above are used when number mode is not data network address.
     The following are used only when number mode is data network address
     mode.
  */
  UASMS_NUMBER_DATA_IP        = 1
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_number_type_e_type;


/* --------------------- */
/* ---- Number plan ---- */
/* --------------------- */
typedef enum
{
  UASMS_NUMBER_PLAN_UNKNOWN     = 0,
  UASMS_NUMBER_PLAN_TELEPHONY   = 1,  /* CCITT E.164 and E.163,
                                         including ISDN plan */
  UASMS_NUMBER_PLAN_RESERVED_2  = 2,
  UASMS_NUMBER_PLAN_DATA        = 3, /* CCITT X.121 */
  UASMS_NUMBER_PLAN_TELEX       = 4, /* CCITT F.69 */
  UASMS_NUMBER_PLAN_RESERVED_5  = 5,
  UASMS_NUMBER_PLAN_RESERVED_6  = 6,
  UASMS_NUMBER_PLAN_RESERVED_7  = 7,
  UASMS_NUMBER_PLAN_RESERVED_8  = 8,
  UASMS_NUMBER_PLAN_PRIVATE     = 9,
  UASMS_NUMBER_PLAN_RESERVED_10 = 10,
  UASMS_NUMBER_PLAN_RESERVED_11 = 11,
  UASMS_NUMBER_PLAN_RESERVED_12 = 12,
  UASMS_NUMBER_PLAN_RESERVED_13 = 13,
  UASMS_NUMBER_PLAN_RESERVED_14 = 14,
  UASMS_NUMBER_PLAN_RESERVED_15 = 15
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_number_plan_e_type;


/* --------------------- */
/* ---- Number mode ---- */
/* --------------------- */
typedef enum
{
  UASMS_NUMBER_MODE_NONE_DATA_NETWORK      = 0,
  UASMS_NUMBER_MODE_DATA_NETWORK           = 1
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_number_mode_e_type;


/* ----------------- */
/* ---- Address ---- */
/* ----------------- */
typedef PACKED struct
{
  uasms_digit_mode_e_type          digit_mode;  /* indicates 4-bit or 8-bit */
  uasms_number_mode_e_type         number_mode; /* only meaningful when digit
                                                  mode is 8-bit */
  uasms_number_type_e_type         number_type; /* only meaningful when digit
                                                   mode is 8-bit */
  uasms_number_plan_e_type         number_plan; /* only meaningful when digit
                                                   mode is 8-bit and number
                                                   mode is not data network
                                                   address mode */
  uns8                             number_of_digits;

  /* Each byte in the following array represents a 4-bit or 8-bit digit of
     address data:
  */
  uns8                             digits[ UASMS_ADDRESS_MAX ];

} uasms_address_type;


/* ------------------------- */
/* ---- Subaddress type ---- */
/* ------------------------- */
typedef enum
{
  UASMS_SUBADDRESS_NSAP           = 0,  /* CCITT X.213 or ISO 8348 AD2 */
  UASMS_SUBADDRESS_USER_SPECIFIED = 1   /* e.g. X.25 */
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_subaddress_type_e_type;


/* -------------------- */
/* ---- Subaddress ---- */
/* -------------------- */
typedef PACKED struct
{
  uasms_subaddress_type_e_type         type;
  boolean                              odd;   /* TRUE means the last byte's
                                                 lower 4 bits should be
                                                 ignored */
  uns8                                 number_of_digits;

  /* Each byte in the following array represents a 8-bit digit of
     subaddress data:
  */
  uns8                                 digits[ UASMS_SUBADDRESS_MAX ];

} uasms_subaddress_type;


/* ------------------------ */
/* -- Service category ---- */
/* ------------------------ */
typedef enum
{
  UASMS_SRV_UNKOWN          = 0,
  UASMS_SRV_EMERGENCY,      // 1
  UASMS_SRV_ADMIN,          // 2
  UASMS_SRV_MAINTENANCE,    // 3
  UASMS_SRV_GEN_NEWS_LOC,   // 4
  UASMS_SRV_GEN_NEWS_REG,   // 5
  UASMS_SRV_GEN_NEWS_NAT,   // 6
  UASMS_SRV_GEN_NEWS_INT,   // 7
  UASMS_SRV_FIN_NEWS_LOC,   // 8
  UASMS_SRV_FIN_NEWS_REG,   // 9
  UASMS_SRV_FIN_NEWS_NAT,   // 0xA
  UASMS_SRV_FIN_NEWS_INT,   // 0xB
  UASMS_SRV_SPT_NEWS_LOC,   // 0xC
  UASMS_SRV_SPT_NEWS_REG,   // 0xD
  UASMS_SRV_SPT_NEWS_NAT,   // 0xE
  UASMS_SRV_SPT_NEWS_INT,   // 0xF
  UASMS_SRV_ENT_NEWS_LOC,   // 0x10
  UASMS_SRV_ENT_NEWS_REG,   // 0x11
  UASMS_SRV_ENT_NEWS_NAT,   // 0x12
  UASMS_SRV_ENT_NEWS_INT,   // 0x13
  UASMS_SRV_LOC_WEATHER,    // 0x14
  UASMS_SRV_AREA_TRAFFIC,   // 0x15
  UASMS_SRV_AIRPORT_SCHED,  // 0x16
  UASMS_SRV_RESTAURANTS,    // 0x17
  UASMS_SRV_LODGINGS,       // 0x18
  UASMS_SRV_RETAILS,        // 0x19
  UASMS_SRV_ADS,            // 0x1A
  UASMS_SRV_STOCK_QUOTES,   // 0x1B
  UASMS_SRV_JOBS,           // 0x1C
  UASMS_SRV_MEDICAL,        // 0x1D
  UASMS_SRV_TECH_NEWS,      // 0x1E
  UASMS_SRV_MULTI,          // 0x1F

  UASMS_SRV_MAX             = 0xffff /* at least 16 bits */

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_service_e_type;



/* ------------------------ */
/* ---- Message format ---- */
/* ------------------------ */
/* Message format indicator that is used for message encoding and decoding.
   NOTE: this type is normally not used by the client directly.
*/
typedef enum
{
  UASMS_FORMAT_CDMA         = 0,  /* IS-95 */
  UASMS_FORMAT_ANALOG_CLI,        /* IS-91 */
  UASMS_FORMAT_ANALOG_VOICE_MAIL, /* IS-91 */
  UASMS_FORMAT_ANALOG_SMS,        /* IS-91 */
  UASMS_FORMAT_ANALOG_AWISMS,     /* IS-95 Alert With Information SMS */
  UASMS_FORMAT_MWI,               /* Message Waiting Indication */

  UASMS_NUMBER_OF_FORMATS
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_format_e_type;


/* --------------------- */
/* -- Raw Bearer Data -- */
/* --------------------- */
/* NOTE: Normally the client need not care about the internal structure
         of this type.
*/
typedef struct
{
  uasms_format_e_type        format;
  uns16                      bd_len; /* if 0, no bd data is present */
  uns8                       bd_data[ UASMS_MAX_LEN ];

} uasms_raw_bd_type;


/* --------------------- */
/* -- Message Tag    --- */
/* --------------------- */
typedef enum
{
  /* The enum values here match those in the R-UIM standard
     for easy translation
  */
  UASMS_INVALID       = 0,
  UASMS_MT_READ       = 1,
  UASMS_MT_NOT_READ   = 3,
  UASMS_MO_SENT       = 5,
  UASMS_MO_NOT_SENT   = 7,
  UASMS_MO_TEMPLATE   = 0x0100
  
} uasms_message_tag_e_type;


#define UASMS_IS_MO( tag )       \
  ( tag == UASMS_MO_SENT ||      \
    tag == UASMS_MO_NOT_SENT ||  \
    tag == UASMS_MO_TEMPLATE )


/* ------------------------------------------------------------ */
/* --------- Message passed between client and API ------------ */
/* ------------------------------------------------------------ */
typedef struct uasms_client_message_struct
{
  uasms_teleservice_e_type       teleservice; /* If not available, will be
                                                 set to "Unknown" */

  /* For incoming messages, the address is the origination address;
     otherwise, it is the destination address.
     NOTE: Mandatory in both incoming and outgoing messages.
  */
  uasms_address_type             address;

  /* Optional: If subaddress.number_of_digits == 0, this field is not present.
  */
  uasms_subaddress_type          subaddress;

  /* Indicates if the client needs to confirm whether the message is received
     successfully or not.
  */
  boolean                        is_tl_ack_requested;

  uasms_service_e_type           service;

  /* Indicates if the message received is from a broadcast service.
  */
  boolean                        is_broadcast;

  /* Raw (un-decoded) bearer data; transparent to the client:
  */
  uasms_raw_bd_type              raw_bd;

  uasms_message_tag_e_type       tag;

} uasms_client_message_type;



/* ====================================================================== */
/* ---------------------------------------------------------------------- */
/* ---------------- Definitions related to Bearer Data: ----------------- */
/* ---------------------------------------------------------------------- */
/* ====================================================================== */

enum { UASMS_USER_DATA_MAX     =  229 };
enum { UASMS_CALL_NUMBER_MAX   =  32  };


/* Bearer data subparameter mask values:
*/
enum{ UASMS_MASK_BD_NULL             =   0x00000000 };
enum{ UASMS_MASK_BD_MSG_ID           =   0x00000001 };
enum{ UASMS_MASK_BD_USER_DATA        =   0x00000002 };
enum{ UASMS_MASK_BD_USER_RESP        =   0x00000004 };
enum{ UASMS_MASK_BD_MC_TIME          =   0x00000008 };
enum{ UASMS_MASK_BD_VALID_ABS        =   0x00000010 };
enum{ UASMS_MASK_BD_VALID_REL        =   0x00000020 };
enum{ UASMS_MASK_BD_DEFER_ABS        =   0x00000040 };
enum{ UASMS_MASK_BD_DEFER_REL        =   0x00000080 };
enum{ UASMS_MASK_BD_PRIORITY         =   0x00000100 };
enum{ UASMS_MASK_BD_PRIVACY          =   0x00000200 };
enum{ UASMS_MASK_BD_REPLY_OPTION     =   0x00000400 };
enum{ UASMS_MASK_BD_NUM_OF_MSGS      =   0x00000800 };
enum{ UASMS_MASK_BD_ALERT            =   0x00001000 };
enum{ UASMS_MASK_BD_LANGUAGE         =   0x00002000 };
enum{ UASMS_MASK_BD_CALLBACK         =   0x00004000 };


/* ----------------------- */
/* ---- Message types ---- */
/* ----------------------- */
typedef enum
{
  UASMS_BD_TYPE_RESERVED_0    = 0,
  UASMS_BD_TYPE_DELIVER,
  UASMS_BD_TYPE_SUBMIT,
  UASMS_BD_TYPE_CANCELLATION,
  UASMS_BD_TYPE_DELIVERY_ACK,
  UASMS_BD_TYPE_USER_ACK
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_bd_message_type_e_type;


/* -------------------- */
/* ---- Message Id ---- */
/* -------------------- */
typedef PACKED struct
{
  uasms_bd_message_type_e_type   type;

  /* Identifies a Teleservice message. Set to 0 if not used.
  */
  uns16                          id_number;

} uasms_message_id_type;


/* ----------------------------- */
/* -- User data encoding type -- */
/* ----------------------------- */
typedef enum 
{
  UASMS_ENCODING_OCTET        = 0,
  UASMS_ENCODING_IS91EP,
  UASMS_ENCODING_ASCII,
  UASMS_ENCODING_IA5

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_user_data_encoding_e_type;


/* ------------------------ */
/* -- IS-91 EP data type -- */
/* ------------------------ */
typedef enum  
{
  UASMS_IS91EP_VOICE_MAIL         = 0x82,
  UASMS_IS91EP_SHORT_MESSAGE_FULL = 0x83,
  UASMS_IS91EP_CLI_ORDER          = 0x84,
  UASMS_IS91EP_SHORT_MESSAGE      = 0x85
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_IS91EP_type_e_type;


/* ------------------- */
/* ---- User Data ---- */
/* ------------------- */
typedef PACKED struct
{
  uasms_user_data_encoding_e_type      encoding;
  uasms_IS91EP_type_e_type             is91ep_type;

  /*----------------------------------------------------------------------
     'data_len' indicates the valid number of bytes in the 'data' array. 

     'padding_bits' (0-7) indicates how many bits in the last byte of 'data'
     are invalid bits. This parameter is only used for Mobile-Originated
     messages. There is no way for the API to tell how many padding bits
     exist in the received message. Instead, the application can find out how
     many padding bits exist in the user data when decoding the user data.

     'data' has the raw bits of the user data field of the SMS message.
     The client software should decode the raw user data according to its
     supported encoding types and languages.

     EXCEPTION: CMT-91 user data raw bits are first translated into BD fields
     (e.g. num_messages, callback, etc.) The translated user data field in
     VMN and Short Message is in the form of ASCII characters, each occupying
     a byte in the resulted 'data'.

     'number_of_digits' is the number of digits (7, 8, 16, or whatever bits)
     in the raw user data, which can be used by the client when decoding the
     user data according to the encoding type and language.
  -------------------------------------------------------------------------*/
  uns8                                 data_len;
  uns8                                 padding_bits;
  uns8                                 data[ UASMS_USER_DATA_MAX ];
  uns8                                 number_of_digits;

} uasms_user_data_type;


/* ----------------------- */
/* ---- User response ---- */
/* ----------------------- */
typedef uns8            uasms_user_response_type;


/* ------------------- */
/* ---- Timestamp ---- */
/* ------------------- */
typedef PACKED struct
{
  /* If 'year' is between 96 and 99, the actual year is 1900 + 'year';
     if 'year' is between 00 and 95, the actual year is 2000 + 'year'.
     NOTE: Each field has two BCD digits.
  */
  uns8      year;        /* 0x0-0x99 */
  uns8      month;       /* 0x1-0x12 */
  uns8      day;         /* 0x1-0x31 */
  uns8      hour;        /* 0x0-0x23 */
  uns8      minute;      /* 0x0-0x59 */
  uns8      second;      /* 0x0-0x59 */

} uasms_timestamp_type;


/* ----------------------- */
/* ---- Relative time ---- */
/* ----------------------- */
typedef PACKED struct
{
  uns8       value;

} uasms_relative_time_type;


/* ------------------ */
/* ---- Priority ---- */
/* ------------------ */
typedef enum
{
  UASMS_PRIORITY_NORMAL      = 0,
  UASMS_PRIORITY_INTERACTIVE,
  UASMS_PRIORITY_URGENT,
  UASMS_PRIORITY_EMERGENCY
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_priority_e_type;


/* ----------------- */
/* ---- Privacy ---- */
/* ----------------- */
typedef enum
{
  UASMS_PRIVACY_NORMAL      = 0,
  UASMS_PRIVACY_RESTRICTED,
  UASMS_PRIVACY_CONFIDENTIAL,
  UASMS_PRIVACY_SECRET

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_privacy_e_type;


/* ---------------------- */
/* ---- Reply option ---- */
/* ---------------------- */
typedef PACKED struct
{
  /* whether user ack is requested
  */
  boolean          user_ack_requested;

  /* whether delivery ack is requested.
     Should be FALSE for incoming messages.
  */
  boolean          delivery_ack_requested;

} uasms_reply_option_type;


/* ------------------ */
/* ---- Language ---- */
/* ------------------ */
typedef enum
{
  UASMS_LANGUAGE_UNSPECIFIED = 0,
  UASMS_LANGUAGE_ENGLISH,
  UASMS_LANGUAGE_FRENCH,
  UASMS_LANGUAGE_SPANISH,
  UASMS_LANGUAGE_JAPANESE,
  UASMS_LANGUAGE_KOREAN,
  UASMS_LANGUAGE_CHINESE,
  UASMS_LANGUAGE_HEBREW
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_language_e_type;


/* --------------------- */
/* ---- Call number ---- */
/* --------------------- */
typedef PACKED struct
{
  uasms_digit_mode_e_type          digit_mode;  /* indicates 4-bit or 8-bit */
  uasms_number_type_e_type         number_type; /* only meaningful when digit
                                                   mode is 8-bit */
  uasms_number_plan_e_type         number_plan; /* only meaningful when digit
                                                   mode is 8-bit */
  uns8                             number_of_digits;

  /* each uns8 byte represents a 4- or 8- digit number
  */
  uns8                             digits[ UASMS_CALL_NUMBER_MAX ];

} uasms_call_number_type;


/* ---------------------------------- */
/* ---------- Bearer Data ----------- */
/* ---------------------------------- */
typedef PACKED struct 
{
   /* the mask indicates which fields are present in this message */
   uns32                         mask;

   uasms_message_id_type         message_id;
   uasms_user_data_type          user_data;
   uasms_user_response_type      user_response;
   uasms_timestamp_type          mc_time;
   uasms_timestamp_type          validity_absolute;
   uasms_relative_time_type      validity_relative;
   uasms_timestamp_type          deferred_absolute;
   uasms_relative_time_type      deferred_relative;
   uasms_priority_e_type         priority;
   uasms_privacy_e_type          privacy;
   uasms_reply_option_type       reply_option;
   uns8                          num_messages;  /* the actual value; not BCDs */
   boolean                       alert_on_delivery;
   uasms_language_e_type         language;
   uasms_call_number_type        callback;

} uasms_client_bd_type;



#ifdef FEATURE_BROADCAST_SMS
/* ====================================================================== */
/* ---------------------------------------------------------------------- */
/* ---------------------- Broadcast SMS definitions: -------------------- */
/* ---------------------------------------------------------------------- */
/* ====================================================================== */

#define UASMS_BC_SRV_LABEL_SIZE    10
#define UASMS_BC_TABLE_MAX         ( 32 * 4 )


/* ---------- data definitions ------------ */

/* Carrier's configuration
*/
typedef enum
{
  UASMS_BC_CONFIG_DISALLOW,
  UASMS_BC_CONFIG_ALLOW_TABLE,
  UASMS_BC_CONFIG_ALLOW_ALL

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_bc_config_e_type;


/* User preference
*/
typedef enum
{
  UASMS_BC_PREF_DEACTIVATE,
  UASMS_BC_PREF_ACTIVATE_TABLE,
  UASMS_BC_PREF_ACTIVATE_ALL,

  UASMS_BC_PREF_MAX

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_bc_pref_e_type;


/* Service ID, which is a service/language pair
*/
typedef struct
{
  uasms_service_e_type   service;
  uasms_language_e_type  language;
} uasms_bc_service_id_type;


/* Service info, which is a entry in the service table
*/
typedef struct
{
  uasms_bc_service_id_type  srv_id;
  boolean                   selected;
  uasms_priority_e_type     priority;
  char                      label[UASMS_BC_SRV_LABEL_SIZE];
} uasms_bc_service_info_type;

#endif /* FEATURE_BROADCAST_SMS */


/* ====================================================================== */
/* ---------------------------------------------------------------------- */
/* ---------------------- Other API definitions: ------------------------ */
/* ---------------------------------------------------------------------- */
/* ====================================================================== */


/* ---------------------- */
/* --- Transaction Id --- */
/* ---------------------- */
typedef uns32  uasms_transaction_id_type;



/* ---------------------- */
/* --- Command Ids ------ */
/* ---------------------- */
typedef enum
{
  /* commands from the client through API functions
  */
  UASMS_CMD_CONF_LISTENERS_F,
  UASMS_CMD_SEND_MSG_F,
  UASMS_CMD_ACK_MSG_F,
  UASMS_CMD_ENABLE_AUTO_DISC_F,
  UASMS_CMD_DISABLE_AUTO_DISC_F,
  UASMS_CMD_DTC_CONNECT_F,
  UASMS_CMD_DTC_DISC_F,

#ifdef FEATURE_BROADCAST_SMS
  UASMS_CMD_BC_GET_CONFIG,
  UASMS_CMD_BC_GET_PREF,
  UASMS_CMD_BC_SET_PREF,
  UASMS_CMD_BC_GET_TABLE,
  UASMS_CMD_BC_SELECT_SRV,
  UASMS_CMD_BC_GET_ALL_SRV_IDS,
  UASMS_CMD_BC_GET_SRV_INFO,
  UASMS_CMD_BC_ADD_SRV,
  UASMS_CMD_BC_DELETE_SRV,
  UASMS_CMD_BC_CHANGE_LABEL,
#endif /* FEATURE_BROADCAST_SMS */


  /* --- the following are for internal use only --- */
  /* commands from MC
  */
  UASMS_CMD_MC_MT_MSG_F,
  UASMS_CMD_MC_MO_STATUS_F,
  UASMS_CMD_MC_STATE_CHANGED_F,
  UASMS_CMD_MC_MWI_F,

  /* events from CM
  */
  UASMS_CMD_CALL_INCOM_E,
  UASMS_CMD_CALL_CONNECT_E,
  UASMS_CMD_CALL_END_E,
  UASMS_CMD_CALL_ERROR_E,  /* in case of CM cmd errors */
#ifdef FEATURE_BROADCAST_SMS
  UASMS_CMD_BC_ENABLE_FAILURE_E,
#endif /* FEATURE_BROADCAST_SMS */


  /* last */
  UASMS_CMD_DUMMY

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_cmd_id_e_type;


/* ---------------- */
/* -- Cmd status -- */
/* ---------------- */
typedef enum
{
  UASMS_CMD_ERR_NONE,
#ifdef FEATURE_BROADCAST_SMS
  UASMS_CMD_ERR_BC_BAD_PREF,
  UASMS_CMD_ERR_BC_CANNOT_ACTIVATE,
  UASMS_CMD_ERR_BC_CANNOT_ACTIVATE_ALL,
  UASMS_CMD_ERR_BC_NV_WRITE,
  UASMS_CMD_ERR_BC_BAD_SRV_ID,
  UASMS_CMD_ERR_BC_TABLE_FULL,
  UASMS_CMD_ERR_BC_DUPLICATE_SRV,
//tbd*******************
#endif /* FEATURE_BROADCAST_SMS */
  UASMS_CMD_ERR_LAST

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_cmd_status_e_type;


/* ---------------- */
/* ---- Events ---- */
/* ---------------- */
typedef enum
{
  UASMS_EVENT_DTC_INCOMING_REQUEST = 0,
  UASMS_EVENT_DTC_CONNECTED,
  UASMS_EVENT_DTC_ABORTED,
  UASMS_EVENT_DTC_DISCONNECTED,

#ifdef FEATURE_BROADCAST_SMS
  UASMS_EVENT_BC_CONFIG,
  UASMS_EVENT_BC_PREF,
  UASMS_EVENT_BC_TABLE,
  UASMS_EVENT_BC_SRV_IDS,
  UASMS_EVENT_BC_SRV_INFO,
  UASMS_EVENT_BC_SRV_ADDED,
  UASMS_EVENT_BC_SRV_DELETED,
  UASMS_EVENT_BC_SRV_UPDATED,
  UASMS_EVENT_BC_ENABLE_FAILURE,  /* lower layer (CM & CP) failure */
  UASMS_EVENT_BC_DISABLE_FAILURE, /* lower layer (CM & CP) failure */
#endif /* FEATURE_BROADCAST_SMS */

  UASMS_EVENT_LAST
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_event_e_type;


/* --------------------- */
/* ---- Error class ---- */
/* --------------------- */
typedef enum
{
  UASMS_ERROR_NONE        = 0,
  UASMS_ERROR_RESERVED_1  = 1,
  UASMS_ERROR_TEMP        = 2,
  UASMS_ERROR_PERM        = 3
  
  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_error_class_e_type;


/* ---------------------- */
/* ---- Status codes ---- */
/* ---------------------- */
typedef enum
{
  /* The first half of the enums are from IS-41D SMS cause codes
     with the exact binary values as in IS-41D. They are in the range
     of 0x00 to 0xFF.
  */

  /* A. Network Problems:
  */
  UASMS_ADDRESS_VACANT_S                     = 0,
  UASMS_ADDRESS_TRANSLATION_FAILURE_S,
  UASMS_NETWORK_RESOURCE_SHORTAGE_S,
  UASMS_NETWORK_FAILURE_S,
  UASMS_INVALID_TELESERVICE_ID_S,
  UASMS_OTHER_NETWORK_PROBLEM_S,
  UASMS_OTHER_NETWORK_PROBLEM_MORE_FIRST_S   = 6,
  /* all values within this range are treated as
     UASMS_OTHER_NETWORK_PROBLEM_S
  */
  UASMS_OTHER_NETWORK_PROBLEM_MORE_LAST_S    = 31,

  /* B. Terminal Problems:
  */
  UASMS_NO_PAGE_RESPONSE_S                   = 32,
  UASMS_DESTINATION_BUSY_S,
  UASMS_NO_ACK_S,
  UASMS_DESTINATION_RESOURCE_SHORTAGE_S,
  UASMS_SMS_DELIVERY_POSTPONED_S,
  UASMS_DESTINATION_OUT_OF_SERVICE_S,
  UASMS_DESTINATION_NO_LONGER_AT_THIS_ADDRESS_S,
  UASMS_OTHER_TERMINAL_PROBLEM_S,
  UASMS_OTHER_TERMINAL_PROBLEM_MORE_FIRST_S  = 40,
  /* all values within this range are treated as
     UASMS_OTHER_TERMINAL_PROBLEM_S
  */
  UASMS_OTHER_TERMINAL_PROBLEM_MORE_LAST_S   = 47,
  UASMS_SMS_DELIVERY_POSTPONED_MORE_FIRST_S  = 48,
  UASMS_SMS_DELIVERY_POSTPONED_MORE_LAST_S   = 63,

  /* C. Radio Interface Problems:
  */
  UASMS_RADIO_IF_RESOURCE_SHORTAGE_S         = 64,
  UASMS_RADIO_IF_INCOMPATIBLE_S,
  UASMS_OTHER_RADIO_IF_PROBLEM_S,
  UASMS_OTHER_RADIO_IF_PROBLEM_MORE_FIRST_S  = 67,
  /* all values within this range are treated as
     UASMS_OTHER_RADIO_IF_PROBLEM_S
  */
  UASMS_OTHER_RADIO_IF_PROBLEM_MORE_LAST_S   = 95, 

  /* D. General Problems:
  */
  UASMS_UNEXPECTED_PARM_SIZE_S               = 96,
  UASMS_SMS_ORIGINATION_DENIED_S,
  UASMS_SMS_TERMINATION_DENIED_S,
  UASMS_SUPPL_SERVICE_NOT_SUPPORTED,
  UASMS_SMS_NOT_SUPPORTED_S,
  UASMS_RESERVED_101_S,
  UASMS_MISSING_EXPECTED_PARM_S,
  UASMS_MISSING_MANDATORY_PARM_S,
  UASMS_UNRECOGNIZED_PARM_VALUE_S,
  UASMS_UNEXPECTED_PARM_VALUE_S,
  UASMS_USER_DATA_SIZE_ERROR_S,
  UASMS_OTHER_GENERAL_PROBLEMS_S,
  UASMS_OTHER_GENERAL_PROBLEMS_MORE_FIRST_S  = 108,
  /* all values within this range are treated as
     UASMS_OTHER_GENERAL_PROBLEMS_S
  */
  UASMS_OTHER_GENERAL_PROBLEMS_MORE_LAST_S   = 255,


  /* The codes in the second half are not defined in IS-41 and are
     normally used by the SMS API to report the status of an API function
     call or the status of an originated message.
     The most significant bit is set to 1.
  */
  UASMS_OK_S                  = 0x8000,       /* 32768 */
  UASMS_WAITING_FOR_TL_ACK_S, /* intermediate status for msg requesting TL ack */
  UASMS_OUT_OF_RESOURCES_S,   /* e.g. out of memory buffer */
  UASMS_ACCESS_TOO_LARGE_S,   /* message is too large to be sent over access */
  UASMS_DTC_TOO_LARGE_S,      /* message is too large to be sent over DTC */
  UASMS_NETWORK_NOT_READY_S,  /* lower layer is not in a ready state to send msg */
  UASMS_PHONE_NOT_READY_S,    /* phone is not allowed to send msg */
  UASMS_NOT_ALLOWED_IN_AMPS_S,/* cannot sent msg in analog mode */
  UASMS_CANNOT_SEND_BROADCAST_S,
  UASMS_INVALID_TRANSACTION_ID_S,

  // for internal use only
  UASMS_MSG_NOT_SENT_S,
  UASMS_MSG_BEING_SENT_S

  #ifdef FEATURE_RPC
#error code not present
  #endif

} uasms_status_e_type;



/* ---------------------- */
/* ----- Event info  ---- */
/* ---------------------- */

#ifdef FEATURE_BROADCAST_SMS
/* the whole service table
*/
typedef struct
{
  uint16                      size;
  uasms_bc_service_info_type  entries[UASMS_BC_TABLE_MAX];
} uasms_bc_table_type;


/* all service IDs in the service table
*/
typedef struct
{
  uint16                      size;
  uasms_bc_service_id_type    entries[UASMS_BC_TABLE_MAX];
} uasms_bc_service_ids_type;
#endif /* FEATURE_BROADCAST_SMS */


/* event info
*/
typedef union
{
  int                        dummy;
#ifdef FEATURE_BROADCAST_SMS
  uasms_bc_config_e_type     bc_config;
  uasms_bc_pref_e_type       bc_pref;
  uasms_bc_table_type        bc_table;
  uasms_bc_service_ids_type  bc_srv_ids;
  uasms_bc_service_info_type bc_srv_info;
  uasms_bc_service_id_type   bc_deleted_srv;
  uasms_status_e_type        bc_enable_error;
#endif /* FEATURE_BROADCAST_SMS */
} uasms_event_info_type;



/* <EJECT> */
/*===========================================================================
                             
                    DEFINITIONS OF CLIENT'S LISTENER FUNCTIONS
                             
===========================================================================*/

/* <EJECT> */
/*=========================================================================
FUNCTION TYPE
  uasms_message_listener_type

DESCRIPTION
  This client listener function is called by the UAPI to deliver the 
  incoming mobile-terminated SMS message. The UAPI provides the transaction
  Id. If Ack is requested in the received message, the client should confirm
  the message within the time out as specified below.
   
DEPENDENCIES
  The following functions should have been called previously:
    - uasms_config_listeners()

RETURN VALUE
  The status of the message delivery.

SIDE EFFECTS
  None

SERVICE DURATION
  2 seconds.

  If Ack is requested in this message and the client does not
  acknowledge the reception of the message within the above interval, the 
  UAPI will assume the message is received unsuccessfully and send an SMS
  Ack Message to the base station with an error code.
=========================================================================*/
#ifdef FEATURE_NEWUASMS
typedef void (* uasms_message_listener_type)
#else
typedef uasms_status_e_type (* uasms_message_listener_type)
#endif /* FEATURE_NEWUASMS */
(
  uasms_transaction_id_type               transaction_id,
  const uasms_client_message_type         * message_ptr
);


/* <EJECT> */
/*=========================================================================
FUNCTION TYPE
  uasms_status_listener_type

DESCRIPTION
  This client listener function is called by the UAPI to notify the client
  of the status of a previous transaction request from the client, e.g. a
  message origination.
     
DEPENDENCIES
  The following functions should have been called previously:
    - uasms_config_listeners()

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
typedef void (* uasms_status_listener_type)
(
  const void                        * user_data,
  uasms_error_class_e_type          error_class,
  uasms_status_e_type               status
);


/* <EJECT> */
/*=========================================================================
FUNCTION TYPE
  uasms_event_listener_type

DESCRIPTION
  This client listener function is called by the UAPI to notify the client
  of an event.
     
DEPENDENCIES
  The following functions should have been called previously:
    - uasms_config_listeners()

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
typedef void (* uasms_event_listener_type )
(
  uasms_event_e_type         event,
  uasms_event_info_type      * info_ptr
);

#ifdef FEATURE_BROADCAST_SMS
typedef void (* uasms_bc_cmd_cb_type )
(
  uasms_cmd_id_e_type        cmd,
  uasms_cmd_status_e_type    status
);
#endif /* FEATURE_BROADCAST_SMS */


/* <EJECT> */
/*===========================================================================
                             
                       UAPI FUNCTION DECLARATIONS
                             
===========================================================================*/


/*=========================================================================
FUNCTION
  uasms_config_listeners

DESCRIPTION
  This function registers the listener functions for receiving messages,
  statuses and events.

DEPENDENCIES
  None
   
RETURN VALUE
  None

SIDE EFFECTS
  New listener function pointers are stored.

=========================================================================*/
extern uasms_status_e_type  uasms_config_listeners
( 
  uasms_message_listener_type    message_listener_f,
  uasms_status_listener_type     status_listener_f,
  uasms_event_listener_type      event_listener_f
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_send_message

DESCRIPTION
  This function is used to send mobile-originated SMS messages. It 
  queues the message in the command queue, and notifies the SMS task to
  process this message.

DEPENDENCIES
  None

RETURN VALUE
  Status of sending the message.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
extern uasms_status_e_type uasms_send_message
(
  /* The client can pass a void* pointer with any chosen value.
     The UAPI later delivers the message submission status with this 
     same user_data pointer, so that the client can correlate the message 
     submitted in this function with the status of it.
  */
  const void                         * user_data,

  /* The message to be sent:
  */
  const uasms_client_message_type    * message_ptr
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_ack_message

DESCRIPTION
  This function acknowledges with a status that a message was received by 
  the client's message listener function. An SMS Acknowledgment message
  will be sent to the base station.
   
  This function should be called only when the SMS Point-to-Point Message
  received had the Bearer Reply Option parameter. Otherwise the call to
  this function will have no effect.

DEPENDENCIES
  The following functions should have been called previously:
    - uasms_config_listeners()

RETURN VALUE
  Status of the ack.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
extern uasms_status_e_type uasms_ack_message
(
  uasms_transaction_id_type         transaction_id,
  uasms_error_class_e_type          error_class,
  uasms_status_e_type               status
);



/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_DTC_enable_auto_disconnect

DESCRIPTION
  This function enables the feature of auto-disconnecting the DTC connection
  when there is no message sent or received during the specified time out
  period. The client will receive an event if the connection is disconnected.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
extern uasms_status_e_type uasms_DTC_enable_auto_disconnect
(
  uns32 timeout    /* in seconds */
);   


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_DTC_disable_auto_disconnect

DESCRIPTION
  This function disables the feature of auto-disconnecting the DTC connection
  when there is no message sent or received during the specified time out
  period.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
extern uasms_status_e_type uasms_DTC_disable_auto_disconnect
(
  void
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_DTC_connect

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
extern uasms_status_e_type uasms_DTC_connect
(
  void
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasms_DTC_disconnect

DESCRIPTION
  This function is used to disconnect the DTC connection when there is a 
  request from the client or from the base station. It will send CDMA
  messages to the base station to disconnect the DTC connection.
   
DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
extern uasms_status_e_type uasms_DTC_disconnect
(
  void
);

#ifdef FEATURE_NEWUASMS
/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_encode_bearer_data

DESCRIPTION
  This function encodes the SMS bearer data from the client 
  structure to the raw format.
  
DEPENDENCIES
  None

RETURN VALUE
  Encoding result. UASMS_OK_S means encoding succeeded; otherwise it failed.

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_encode_bearer_data
(
  const uasms_client_bd_type                * cl_bd_ptr,       /* IN */
  uasms_raw_bd_type                         * raw_bd_ptr       /* OUT */
);


/* <EJECT> */
/*=========================================================================
FUNCTION 
  uasmsx_decode_bearer_data

DESCRIPTION
  This function decodes the SMS bearer data from the raw format
  to the client structure.
  
DEPENDENCIES
  None

RETURN VALUE
  Encoding result. UASMS_OK_S means encoding succeeded; otherwise it failed.

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uasmsx_decode_bearer_data
(
  const uasms_raw_bd_type            * raw_bd_ptr,   /* IN */
  uasms_client_bd_type               * cl_bd_ptr     /* OUT */
);


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

#ifdef FEATURE_BROADCAST_SMS

/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_get_config

DESCRIPTION
  This function is used to retrieve the configuration for broadcast SMS.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_get_config
(
  uasms_bc_cmd_cb_type   cb
);

/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_get_pref

DESCRIPTION
  This function is used to retrieve the user preference for broadcast SMS.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_get_pref
(
  uasms_bc_cmd_cb_type   cb
);

/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_set_pref

DESCRIPTION
  This function is used to set the user preference for broadcast SMS.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_set_pref
(
  uasms_bc_cmd_cb_type     cb,
  uasms_bc_pref_e_type     pref
);

/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_get_table

DESCRIPTION
  This function is used to retrieve the service table for broadcast SMS.

  NOTE: To prevent retrieving a large service table, the function
  uasms_bc_get_all_service_ids() can be called to retrieve all service
  IDs, where each ID is much smaller than a complete table entry, and then
  the function uasms_bc_get_service_info() can be called for each service ID
  in order to retrieve the table entries one by one.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_get_table
(
  uasms_bc_cmd_cb_type   cb
);

/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_select_service

DESCRIPTION
  This function is used to select a broadcast SMS service

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_select_service
(
  uasms_bc_cmd_cb_type      cb,
  #ifdef FEATURE_NEWUASMS
  uasms_bc_service_id_type  *srv_id,
  #else  
  uasms_bc_service_id_type  srv_id,
  #endif  
  boolean                   selected,
  uasms_priority_e_type     priority
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_get_all_service_ids

DESCRIPTION
  This function is used to retrieve all the service IDs in the broadcast
  SMS service table. After the retrieval of the service IDs, the function
  uasms_bc_get_service_info() can be called to retrieve the complete
  table entries one by one.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_get_all_service_ids
(
  uasms_bc_cmd_cb_type  cb
);


/* <EJECT> */
/*=========================================================================
FUNCTION
  uasms_bc_get_service_info

DESCRIPTION
  This function is used to retrieve a table entry in the broadcast SMS
  service table using a service ID.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_get_service_info
(
  uasms_bc_cmd_cb_type      cb,
  #ifdef FEATURE_NEWUASMS
  uasms_bc_service_id_type  *srv_id
  #else
  uasms_bc_service_id_type  srv_id
  #endif
);


/* <EJECT>*/
/*=========================================================================
FUNCTION
  uasms_bc_add_service

DESCRIPTION
  This function is used to add an entry to the broadcast SMS service table.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_add_service
(
  uasms_bc_cmd_cb_type        cb,
  #ifdef FEATURE_NEWUASMS
  uasms_bc_service_info_type  *srv_info
  #else
  uasms_bc_service_info_type  srv_info
  #endif
);


/* <EJECT>*/
/*=========================================================================
FUNCTION
  uasms_bc_delete_service

DESCRIPTION
  This function is used to delete an entry to the broadcast SMS service table.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_delete_service
(
  uasms_bc_cmd_cb_type     cb,
  #ifdef FEATURE_NEWUASMS
  uasms_bc_service_id_type *srv_id
  #else
  uasms_bc_service_id_type srv_id
  #endif
);


/* <EJECT>*/
/*=========================================================================
FUNCTION
  uasms_bc_change_label

DESCRIPTION
  This function is used to change an entry in the broadcast SMS service table.

DEPENDENCIES
  None

RETURN VALUE
  Status of the request.

SIDE EFFECTS
  Request is added to the request queue.

=========================================================================*/
uasms_status_e_type uasms_bc_change_label
(
  uasms_bc_cmd_cb_type     cb,
  #ifdef FEATURE_NEWUASMS
  uasms_bc_service_id_type *srv_id,
  #else
  uasms_bc_service_id_type srv_id,
  #endif
  char                  *label_ptr
);



#endif /* FEATURE_BROADCAST_SMS */


#ifdef __cplusplus
}
#endif

                              
#endif /* UASMS_H */

#endif /* FEATURE_UASMS */

