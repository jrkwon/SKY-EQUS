#ifdef FEATURE_UASMS
#ifndef UISMSL_H
#define UISMSL_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


      U S E R   I N T E R F A C E   S M S   L I S T   O P E R A T I O N

GENERAL DESCRIPTION
  Controls and manages the SMS lists of stored messages in NV.

      Copyright (c) 1995 by QUALCOMM, Inc.  All Rights Reserved.
      Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/msm3100/vcs/uiusmsl.h_v   2.1   19 Aug 1999 14:25:36   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/97   jjn     Disabled on-board ringer for the module and also support
                   DM packet that retrieves SMS messages
04/18/96   BK      New UI branching created from uismsl.h ver 1.5
01/03/96   CAH     AMPS Message Waiting Order and CDMA MWI Info records are
                     now converted to IS91 Voice Mail Notification and sent
                     to the SMS handler.
10/18/95   day     Added text match detection for messages/pages and 
                   automatic deletion of duplicate message/page.
09/29/95   day     Added CDMA SMS message's duplicate detection processing.
08/22/95   day     Optimized SMS list structures to save RAM.
08/22/95   day     Added call back number save and prepend functions.
08/10/95   day     Created

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//#include "target.h"
#include "customer.h"

#include "comdef.h"
//#include "ui.h"
#include "smsi.h"

#ifdef FEATURE_SMS_PACKET

#include "mod.h"

#endif /* FEATURE_SMS_PACKET */


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Indicates SMS text message is a duplicate. */
#define UI_SMS_STS_DUPLICATE    0x01  
#define UI_SMS_URGENT_IND       2


#define SMS_SPACE               ' '

/* Structure to save SMS list info per NV index in NV */
typedef struct
{
  byte list_cat;
  byte order_idx;
} ui_sms_nv_list_type;

/* Maximum number of SMS messages that can be    */
/* stored in NV memory. Maximum + 1 represents   */
/* maximum number of SMS NV addresses available. */
#define MAX_NV_MSGS             43 // 23->43 huni Max Size 40 
#define UI_SMS_WIDE             MAX_VIEW_BUF_COLUME_SIZE //17->MAX_VIEW_BUF_COLUME_SIZE
#define MAX_RECEIVE_MESSAGE	    40 // 20->40 huni Max Size 40 
#define MAX_MSG_VIEW_LINE	    20
#define SMS_SCREEN_SIZE         16

/* Structure to hold SMS NV addresses or indicies. */
/* There are 23 available SMS NV indicies (1-23)   */
/* for message storage.  Index 0 is reserved for   */
/* saving the current stored message lists and     */
/* each message's order in a specific list.        */
/* nv_idx contains the actual NV SMS address where */
/* the message is stored.                          */
typedef struct
{
  byte cnt;
  byte nv_idx[MAX_NV_MSGS];
} ui_sms_list_type;

/* SMS list categories are:                   */
/* 0 - available address or NV index list     */
/* 1 - text, new, urgent message list         */
/* 2 - text, new, non-urgent message list     */
/* 3 - text, old message list                 */
/* 4 - voice message list                     */
#define UI_SMS_AVAIL              0
#define UI_SMS_TXT_NEW_URGENT     1
#define UI_SMS_TXT_NEW_NOT_URGENT 2
#define UI_SMS_TXT_OLD            3
#define UI_SMS_VC                 4

/* Maximum number of SMS lists defined. */
#define MAX_SMS_LIST 5

/* List of different categories of SMS message lists. */
extern ui_sms_list_type ui_sms_list[MAX_SMS_LIST];
extern BOOL    g_fSmsJustRead;

extern void ui_sms_update_icons (void);

void ui_sms_power_up (void);

extern void g_DoOldTextMatch ( void );

/* SMS NV list of message line text data */
extern byte ui_sms_text_list[MAX_NV_MSGS+1][UI_SMS_WIDE];
extern BYTE pFormatBuf[MAX_MSG_VIEW_LINE*SMS_SCREEN_SIZE + 1];


/* Structure to save SMS message id and */
/* Message Center timestamp for each    */
/* stored message.  CDMA message's      */
/* duplicate detection will performed   */
/* based on this info which is indexed  */
/* per NV index.                        */
typedef struct
{
  word               message_id;
  boolean            check_time;
  smsi_abs_time_type mc_time;
  boolean            match_old_text;
} ui_sms_detect_list_type;

/* SMS NV list of CDMA message's duplicate */
/* detection data                          */
extern ui_sms_detect_list_type ui_sms_detect_list[MAX_NV_MSGS+1];

/* SMS list edit commands and what information */
/* is required for each operation.             */
/* Since reading from the list is not an edit  */
/* operation, it can be performed by directly  */
/* accessing the list without using an edit    */
/* command.                                    */
typedef enum
{
  UI_SMS_MOVE,   /* Requires src and dest lists and indicies */
  UI_SMS_DELETE, /* Requires src list and index              */
  UI_SMS_ADD     /* Requires dest list and sms message       */
} ui_sms_edit_cmd_type;

/* SMS NV list edit operation and info */
typedef struct
{
  ui_sms_edit_cmd_type cmd;      /* SMS NV list edit command to be performed */
  byte                protect;   // 문자메시지 보호/해제.
  byte                src_list;  /* Source SMS NV list to retrieve item from */
  byte                src_idx;   /* Index to source SMS NV list item         */
  byte                dest_list; /* Destination SMS NV list to put item in   */
  byte                dest_idx;  /* Index to destination SMS NV list item    */
  smsi_msg_type       *sms_msg;  /* Pointer to SMS message to write to NV    */
} ui_sms_edit_info_type;

/* SMS list read commands which identifies how the     */
/* SMS lists will be read.                             */
/* When reading a new message, it will be moved from   */
/* new to the old (read) list.  So the next message    */
/* will always be at the top of the new list.          */
typedef enum
{

  UI_SMS_READ_NEW = 0, /* Read new messages only,     */
                       /* SMS list move after read    */

  UI_SMS_READ_OLD,   /* Read old messages only,     */
                     /* no SMS list move after read */
  UI_SMS_READ_ONE,   /* Read a specifc message only,*/
                     /* SMS list move after read    */
  UI_SMS_READ_NOTIF, /* Read a specifc message only,*/
                     /* no SMS list move after read */
  UI_SMS_READ_ANY    /* Read new then old messages, */
                     /* SMS list move after read    */
} ui_sms_read_cmd_type;

/* UI SMS Message Type */
typedef enum
{
  NORMAL_SMS = 0,   
  URGENT_SMS = 2,   
  EMERGENCY_SMS,
  WAP_MAIL_SMS,
  WAP_PUSH_SMS
#ifdef FEATURE_SKTT_PM
  ,WAP_PUSH_PHOTOMAIL
#endif    
#ifdef FEATURE_SKVM
 ,SKVM_LOAD_SMS
 ,SKVM_DATA_SMS
#endif
 /* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
 , GVM_DL_SMS
#endif
/* <-- */
} ui_sms_kind_type;

/* 문자메시지 Type */
typedef enum
{
  NOT_URGENT_TEXT_SMS = 0,   
  URGENT_TEXT_SMS,
//  EMERGENCY_TEXT_SMS,       //added by reo 2001.05.25
  NOT_URGENT_VOICE_SMS,   
  URGENT_VOICE_SMS   
} ui_sms_title_type;

/* 문자메시지 Type */
typedef enum
{
  COMMON_SMS = 0,   
  WAP_MAIL,
  WAP_PUSH
#ifdef FEATURE_SKVM
 ,SKVM_LOAD
 ,SKVM_DATA
#endif
/* undersea 2001-07-03 --> @why, what */
#ifdef FEATURE_GVM
 , GVM_DOWNLOAD
#endif
/* <-- */
} TELESEVICE_KIND_TYPE;

/* SMS list read operation and info */
typedef struct
{
  ui_sms_read_cmd_type cmd;        /* SMS list read command */
  boolean              new;        /* Message read from new or old SMS list */
  boolean              msg_read;   /* Message read status */
  byte                 read_list;  /* SMS list to be read if read one selected */
  byte                 read_idx;   /* SMS list entry to be read if read one selected */
} ui_sms_read_op_type;

/* SMS list information for latest SMS message received */
typedef struct
{
  byte  new_read_list;  /* SMS list of new message received       */
  byte  new_read_idx;   /* SMS list entry of new message received */
} ui_sms_read_info_type;

/* Used for tagging the latest SMS message received.   */
extern ui_sms_read_info_type ui_sms_read_info;

/* Structure identifying the item to be */
/* written, read or freed from NV.      */
extern nv_sms_type ui_sms_nv_item;
extern BOOL    g_fSmsJustRead;

/* added this for sms.c so linker would not complain. */
extern word sms_voice_mail_mem_size;     /* VMN NV reservation */

/* UI SMS operational status */
typedef enum
{
  UI_SMS_OP_IN_PROG,   /* UI SMS operation in progress      */
  UI_SMS_OP_NV_ERR,    /* NV status error returned          */
  UI_SMS_OP_NO_IDX,    /* No NV index to add or delete      */
  UI_SMS_OP_DUPLICATE, /* Received message is a duplicate   */
  UI_SMS_OP_OK         /* UI_SMS operation successful       */
} ui_sms_op_status_type;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
extern UINT8 FormatWapMailMessage(VOID);

#ifdef FEATURE_SKTT_PM
extern UINT8 FormatWapPushMessage(BOOL* pfIsPhotoMail);
#else
extern UINT8 FormatWapPushMessage(VOID);
#endif
/* undersea 2001-07-09 --> @why, what */
#ifdef FEATURE_GVM
extern UINT8 FormatGVMMessage(VOID);
VOID sParsingGVMMessage(BYTE* pMsg, BOOL fIsNotify);
#endif
/* <-- */

extern ui_sms_title_type g_MessageNotif
(
    BYTE    *pCallback,
    BYTE    *pTimeStamp
);

extern byte ui_sms_auto_format_one_line
(
  UINT8   nCharNum,           /* One line character num          */
  byte    *input_txt,         /* Pointer to text to be formatted */
  word    input_len,          /* Number of bytes to be formatted */
  byte    *format_txt,        /* Pointer to formatted text       */
  word    format_len          /* Size of formatted text          */
);

extern  TELESEVICE_KIND_TYPE g_TeleId( void );

extern  INT8 gReadTextMessage ( void );

extern  void gMakeMsgList ( void );

extern  void gMakeProtect ( void );

extern  void gDeleteMsg ( void );

extern VOID    g_DeleteTextMessage( void );

extern VOID ReadNewMessage ( void );

extern BOOL IsMsgProtect( void );

extern ui_sms_kind_type g_FormatTextMessage
( 
    BYTE    *pTimeStamp,
    BYTE    *pCallback,
    INT8    *nLine
);

extern char *ui_sms_bcd_ascii_cvt
(
  CHAR  bcd_num,                 /* 2 BCD numbers to convert */
  CHAR  *txt                     /* ascii conversion for up to 2 numbers */
);

#ifdef FEATURE_SKTT_EQUS_LOCATION
extern UINT8 WhatLocationType(VOID);
extern WORD SetRcvLocationSmsMsg(ui_cmd_type *cmd_ptr);
#endif //#ifdef FEATURE_SKTT_EQUS_LOCATION
#ifdef FEATURE_SKVM
extern WORD SetRcvSKVMDataSmsMsg(ui_cmd_type *cmd_ptr);
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_NV_DATA

DESCRIPTION
  Pack or unpack SMS data stored in or retrieved from NV.
  
DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ui_sms_nv_data 
(
  boolean       packing,
  nv_sms_type   *pack_data_ptr,
  smsi_msg_type *unpack_data_ptr
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_TEXT_MATCH

DESCRIPTION
  Compares user data text in the new message or page against the
  text in the stored unread or read messages or pages to determine 
  duplicate text message.  If duplicate text occurs, then the stored 
  messages with duplicate text are deleted and the new message is 
  tagged as duplicate text.  Caller indicates if text match should 
  be performed against stored unread messages or stored read messages.
  Since text matching causes an automatic delete of stored messages, the 
  following rules must be followed: 1) When text matching the new 
  message against unread messages, text matching must be performed
  before the new message is added to NV since the message delete 
  could obsolete the bookmark info on the new message when added to
  NV.  2) When text matching the new message against old read messages,
  be sure the current SMS state assumes the user cannot be reading 
  old messages.  Text matching is performed in two separate places 
  in order to maintain the maximum amount of SMS NV memory and maximum 
  number of available SMS NV indicies at all times and to prevent the 
  concurrent read by the user and automatic deletion of the same message.
  Assumes new message to be compared is in ui_sms_msg.  This function 
  is not performed on voice mail.  Function returns boolean indicating 
  if text match was found.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_sms_text_match
(
boolean unread
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_SAVE_LIST_TEXT

DESCRIPTION
  Save first line of text or call back number in the SMS lists
  that indicate a stored message.  This function is performed
  at power-up and for each new SMS message to be added to an
  SMS list.  Formatting of the line is performed before storing
  the line of text.  This saved text is used for the stored
  message list display.  This function uses the source list
  information and the pointer to the SMS message from the list
  info

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_sms_save_list_text
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_LIST_CMD

DESCRIPTION
  Command edits to the SMS lists - delete item from list,
  move item from one list to another and add new item to list.
  NV indicies (1-23) are assigned to separate SMS lists.  The
  pool of indicies are never deleted from or added to.  They
  are moved from one SMS list to another depending on how their
  SMS list status changes.  NV index is reserved for storing the 
  SMS control list - this list indicates which of the other
  indicies (1-23) is assigned to a specific SMS list.  Commands
  to NV will be issued for adding new item (allocating/writing) or 
  deleting (freeing) an item.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern ui_sms_op_status_type ui_sms_list_cmd
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_STORE_NEW_MSG

DESCRIPTION
  Store new received SMS message in NV and display appropriate
  icon.  Blink icon if message indicates urgent priority.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
word ui_sms_store_new_msg
(
  UI_COMMAND_U *pCommand
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_READ_VOICE

DESCRIPTION
  Determines if there are any voice mail to be read from new or
  old lists.  Read operation indicates how the SMS lists should be
  searched for the next read and if voice mail was read and if
  it was from a new (unread) or old (read) list.  The message read
  will be unpacked and placed in ui_sms_msg.  Destination list and
  index in list info gives the location of the current message that
  was read.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_read_voice 
(
  ui_sms_read_op_type *read_op
    /* Read operation information */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_READ_TEXT

DESCRIPTION
  Determines if there are any text messages to be read from new or
  old lists.  Read operation indicates how the SMS lists should be
  searched for the next read and if a text message was read and if
  it was from a new (unread) or old (read) list.  The message read
  will be unpacked and placed in ui_sms_msg.  Destination list and
  index in list info gives the location of the current message that
  was read.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_read_text 
(
  ui_sms_read_op_type *read_op
    /* Read operation information */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_CREATE_N_VOICE_MSG

DESCRIPTION
  Creates an SMS Voice Mail Notification message command based on a
  Number of Messages count.  The Teleservice type created will be
  IS-91 Extended Protocol Voice Mail, Point to Point, with Message
  Identifier = 0, No Bearer Reply.
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_create_n_voice_msg
(
  ui_cmd_type *cmd_ptr,      /* Command structure to store VMN info into */
  word count                 /* Number of Messages count */
);

#ifdef FEATURE_SMS_PACKET

/*===========================================================================
FUNCTION UI_SMS_NEW_TXT_MSG

DESCRIPTION
  Returns True is there are any new SMS text messages
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ui_sms_new_txt_msg
(
  void
);

/*===========================================================================
FUNCTION UI_SMS_OLD_TXT_MSG

DESCRIPTION
  Returns True is there are any old SMS messages that haven't been deleted
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ui_sms_old_txt_msg
(
  void
);

/*===========================================================================
FUNCTION UI_SMS_VC_MSG

DESCRIPTION
  Returns True is there are any SMS voice messages
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean ui_sms_vc_msg
(
  void
);

/*===========================================================================
FUNCTION UI_SMS_DELETE

DESCRIPTION
  Deletes the SMS messages that are indicated by erase_code

DEPENDENCIES
  None

SIDE EFFECTS
  Will erase varying numbers of SMS message, depending on the erase_code

===========================================================================*/
void ui_sms_delete
(
  mod_sms_erase_code_type erase_code
);

/*===========================================================================
FUNCTION UI_SMS_DELETE

DESCRIPTION
  Deletes the SMS messages that are indicated by erase_code

DEPENDENCIES
  None

SIDE EFFECTS
  Will erase varying numbers of SMS message, depending on the erase_code

===========================================================================*/

boolean ui_sms_delete_single( boolean new, boolean text );

/*===========================================================================
FUNCTION UI_CHECK_MORE_SMS_TO_READ

DESCRIPTION
  Check if there are any of the specified type of message available 
  for reading or deletion

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean ui_check_more_sms_to_delete
(
  boolean                 text
);

/*===========================================================================

FUNCTION UISMSL_GET_SMS_MESSAGE

DESCRIPTION
  Gets an SMS message.  

DEPENDENCIES
  None

RETURN VALUE
  Indicates whether or not there was an SMS message to read, whether or
  not there are more to read, whether the SMS message was voice or text,
  whether or not the SMS was a new (un-read) message, etc  

SIDE EFFECTS
  None

===========================================================================*/

void uismsl_get_sms_message( void );

/*===========================================================================
FUNCTION UISMSL_RETURN_SMS

DESCRIPTION
  This function returns the result of an SMS read operation to the calling
  task

DEPENDENCIES
  This function should be called only after the completion of a successful
  SMS read command and before the commencement of another.

SIDE EFFECTS
  None

===========================================================================*/
smsi_msg_type uismsl_return_sms( void );

/*===========================================================================
FUNCTION UISMSL_RETURN_READ_STATUS

DESCRIPTION

DEPENDENCIES

SIDE EFFECTS

===========================================================================*/
byte uismsl_return_read_status( void );

/*===========================================================================
FUNCTION UISMSL_RETURN_DELETE_STATUS

DESCRIPTION
  Returns the status of the delete operation

DEPENDENCIES
  Should be called after the delete operation is completed

SIDE EFFECTS
  None
===========================================================================*/
byte uismsl_return_delete_status( void );

#endif /* FEATURE_SMS_PACKET */

#endif
#endif /* FEATURE_UASMS */

