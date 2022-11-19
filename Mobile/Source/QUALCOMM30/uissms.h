#ifdef FEATURE_UASMS
#ifndef UISSMS_H
#define UISSMS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


          U S E R   I N T E R F A C E   S M S   S T A T E

GENERAL DESCRIPTION
  SMS substate control for displaying SMS menus and processing
  events.

      Copyright (c) 1995 by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uissms.h_v   1.1   29 Mar 2000 18:10:50   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/17/98   JGR     Added support for Net Mode and Net Alerts all are under
                   FEATURE_DS_UP.
06/12/98   skk     Fixed a compile warning related to ui_sms_update_icons not being
                   prototyped. 
04/16/98   skk     Fixed a problem where SMS was storing a message without properly 
                   updating control list, resulting in that message disappearing and
                   a discrepency in NV memory size cr5085
12/16/97   skk     Added SMS option 6 capability.
04/22/97   BK      Regional naming convention for SMS var.
07/11/96   CAH     Added special non-zero Message Waiting Notification
                    support.
05/09/96   BK      Fixed common icon bug, added means to track icon condition. 
04/18/96   BK      New UI branching created from uissms.h ver 1.4
09/20/95   day     Added alert reminder logic for unacknowledged
                   notifications.
09/13/95   day     Use #define value for urgent message check.
09/06/95   day     Added no access SMS state.
08/22/95   day     Added call back number save and prepend functions.
08/10/95   day     Created

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "comdef.h"
#include "ui.h"
#include "uiuint.h"
#include "uiusmsl.h"
#include "smsi.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_UI_ENVELOPE_VM_ICON
#error code not present
#endif

/* UI SMS states */
typedef enum {
  UI_SMS_IDLE_S,                  /* 0 - idle state                      */
  UI_SMS_MSG_RXD_S,               /* 1 - message notification state      */
  UI_SMS_MSG_ACCESS_S,            /* 2 - message access state            */
  UI_SMS_TEXT_S,                  /* 3 - text message access state       */
  UI_SMS_SCROLL_S,                /* 4 - text message scroll state       */
  UI_SMS_ERASE_S,                 /* 5 - text message erase state        */
  UI_SMS_PREPEND_S,               /* 6 - call back number prepend state  */
  UI_SMS_CALL_BACK_S,             /* 7 - text message call back state    */
  UI_SMS_VOICE_S,                 /* 8 - voice mail access state         */
#ifdef FEATURE_DS_UP
#error code not present
#endif  
  UI_SMS_NO_ACCESS_S              /* 9 or 12 - no message access state         */
} ui_sms_state_type;

/* Maximum number of NV bytes in SMS pool */
#define UI_SMS_NV_MAX_BYTES  700

/* Value of urgent priority indicator */
#define UI_SMS_URGENT_IND    2

#ifdef FEATURE_UI_NON_ZERO_MWN 
/* Value to denote a non zero voice mail message count */
#define NON_ZERO_MWN 0xF
#endif /* FEATURE_UI_NON_ZERO_MWN */

/* percentage of SMS memory used */
extern word sms_mem_percent;      

/* Unpacked SMS message */
extern smsi_msg_type ui_sms_msg;

/* SMS list operation and info */
extern ui_sms_edit_info_type list_info;

/* Text message read type indicator */
extern boolean reading_new;

/* Reminder alert indicator */
extern boolean ui_sms_alert_status;

extern boolean ui_sms_notif_cleared_flag;  /* SMS6 */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_REMINDER

DESCRIPTION
  Determines if reminder alerts should be turned on for
  unacknowledged notifications.  Acknowledgement constitutes
  a keypad press or earpiece up by the user.  Reminder alert
  interval is every 2 minutes until the user acknowledges.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_reminder
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION DO_OLD_TEXT_MATCH  
cr5085
DESCRIPTION


DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void do_old_text_match
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_POWER_UP

DESCRIPTION
  Power-up processing for SMS functionality.  Read SMS control
  list from NV to create SMS lists which contain message data
  and NV index to where packed message is stored.  Also, the
  first line of text or call back number will be saved for
  each stored message entry.  Initialize UI SMS state and
  data.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_power_up
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_POWER_DOWN

DESCRIPTION
  Power-down processing for SMS functionality.  Create SMS control
  list from SMS lists which contain message data and NV index to 
  where packed message is stored and store in NV.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_power_down
(
void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_SMS

DESCRIPTION
  SMS state controller handles SMS event processing including SMS
  displays, alerts and SMS data management.
  
DEPENDENCIES
  Called from UI_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
extern ui_maj_type uistate_sms 
(
void
);
/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_UPDATE_ICONS

DESCRIPTION

DEPENDENCIES

SIDE EFFECTS
  None
===========================================================================*/
void ui_sms_update_icons
(
void
);


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===============================================================================

                                   EQUS UI Variables
	                                    ( SMS )       

=================================================================================*/
/*
**--------------------------------------------------------------------------------
**  Type Definitions
**--------------------------------------------------------------------------------
*/
//typedef struct tagSMS             eqsSMS;

#if 0
/*
**--------------------------------------------------------------------------------
**  General Definitions
**--------------------------------------------------------------------------------
*/
struct tagSMS{
#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
  	uasms_bc_config_e_type          BCConfig;        //ui.bcsms_config
	uasms_bc_pref_e_type            BCPref;          //ui.bcsms_pref
	ui_uasms_bc_table_type          BCSVCTable;      //ui.bcsms_srvc_table
	byte                            BCMenuIndex;     //ui.bcsms_menu_index
	ui_uasms_bc_service_info_type   BCSVCInfo;       //ui.bcsms_srvc_info
	boolean                         UTCTime;         //ui.sms_as_is_time
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */	
};
#endif


/*
**--------------------------------------------------------------------------------
**  Variable Declarations
**--------------------------------------------------------------------------------
*/
//extern eqsSMS  gSMS;    // defined in uihcmd.c
#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)





#endif
#endif /* FEATURE_UASMS */

