#ifdef FEATURE_UASMS

#ifndef UIXUASMS_H
#define UIXUASMS_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           U I  - S M S   A P I   A D A P T I O N   M O D U L E

DESCRIPTION
  This module is the link between the SMS API and the rest of the UI code.
  It acts as the client of the SMS API.

Copyright (c) 1998-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/uixuasms.h_v   1.4   14 Dec 2000 08:40:32   hqu  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/12/00   HQ      Added support of FEATURE_COMPOSE_MO_SMS.
10/09/00   HQ      Added support of FEATURE_RUIM_SMS.
05/19/00   HQ      Added maintenance of delivery ack and user ack status for
                   each MO msg;
                   return delivery ack and user ack in uinuasms_get_status();
                   added uinuasms_del_ack_received() and
                   uinuasms_user_ack_received() for setting ack status.
03/16/00   CS      Removed Broadcast SMS report API support.
02/22/00   CS      Added support for FEATURE_BROADCAST_SMS.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                             INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "target.h"
#include "uasms.h"

#ifdef FEATURE_SKTT_EQUS_SMS
#include "smsi.h"    
#endif
/* <EJECT> */
/*===========================================================================

                         DATA TYPE DECLARATIONS

===========================================================================*/
#ifdef FEATURE_UASMS_SAVE_MEM
enum{ UIUASMS_MAX_MO_MSGS   = 1 };
#else
enum{ UIUASMS_MAX_MO_MSGS   = 8 };
#endif


/* <EJECT> */
/*===========================================================================

                         GLOBAL DATA DECLARATIONS

===========================================================================*/
extern boolean          uiuasms_SMS_call_active;

#ifdef FEATURE_SKTT_EQUS_SMS
// by reo 2000-12-29 {-- 
extern uasms_client_bd_type         uinuasms_mo_bds[];
extern uasms_client_message_type    uinuasms_mo_msgs[];
extern uasms_status_e_type          uinuasms_mo_status[];
extern uasms_error_class_e_type     uiuasms_mo_error_class[];
extern boolean                      uiuasms_del_ack_status[];
extern boolean                      uiuasms_user_ack_status[];
extern word   uinuasms_msg_id;
// by reo 2000-12-29 --}
#endif //FEATURE_SKTT_EQUS_SMS

/* <EJECT> */
/*===========================================================================

                           FUNCTIONS

===========================================================================*/
// by reo 2000-12-29 {-- 
#ifdef FEATURE_SKTT_EQUS_SMS
extern byte uinuasms_pack_ascii
(
  const char        * ascii_ptr,       /* IN */
  byte              * data,            /* OUT */
  byte              * data_len_ptr,    /* OUT */
  byte              * padding_bits_ptr /* OUT */
);
extern byte uinuasms_ascii2dtmf
(
  char     *ascii,
  byte     *dtmf
);

extern uasms_teleservice_e_type uinuasms_convert_to_internal_ti
(
    smsi_teleservice_type  InternalTeleID      // input    
);
#endif //FEATURE_SKTT_EQUS_SMS
// by reo 2000-12-29 --}

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_init

DESCRIPTION
  Initialize the fixed MO messages so that they can be sent to the SMS API
  later.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  Initialize the local array of SMS messages.

===========================================================================*/
extern void uiuasms_init
(
  void
);


/* <EJECT> */
/*===========================================================================

FUNCTION    uinuasms_send_MO

DESCRIPTION
  Send the next MO message to the SMS API in the local array. If the last
  message is reached, go back to the first message and send it.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
extern void uinuasms_send_MO
(
  long index
);


/* <EJECT> */
/*=========================================================================
FUNCTION   uiuasms_clear_all_statuses

DESCRIPTION
  Clear the status of each message.

DEPENDENCIES

  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
extern void uiuasms_clear_all_statuses
(
  void
);


/* <EJECT> */
/*=========================================================================
FUNCTION   uiuasms_get_status

DESCRIPTION
  Get the status of a message.

DEPENDENCIES

  None

RETURN VALUE
  The status of the message.

SIDE EFFECTS
  None

=========================================================================*/
extern uasms_status_e_type uiuasms_get_status
(
  long  index,
  boolean *del_ack_requested,
  boolean *del_ack_rereceived,
  boolean *user_ack_requested,
  boolean *user_ack_received
);

/* <EJECT>^L */
/*===========================================================================

FUNCTION    uiuasms_del_ack_received

DESCRIPTION
  Set the status of delivery ack and return the msg index. msg id number is
  incrementing by 1 each time, so it makes sense to return the fixed index.

DEPENDENCIES
  None

RETURN VALUE
  The index to the matching message.
  -1 means not found.

SIDE EFFECTS
  None

===========================================================================*/
int uiuasms_del_ack_received
(
  word msg_id
);


/* <EJECT>^L */
/*===========================================================================

FUNCTION    uiuasms_user_ack_received

DESCRIPTION
  Set the status of user ack and return the msg index. msg id number is
  incrementing by 1 each time, so it makes sense to return the fixed index.

DEPENDENCIES
  None

RETURN VALUE
  The index to the matching message.
  -1 means not found.

SIDE EFFECTS
  None

===========================================================================*/
int uiuasms_user_ack_received
(
  word msg_id
);


#ifdef FEATURE_BROADCAST_SMS

/* <EJECT> */
/*===========================================================================

FUNCTION    uisms_bcsms_init

DESCRIPTION
  Initialize the BC SMS configuration, preferences and service table.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_init(void);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_set_pref

DESCRIPTION
  Set the broadcast SMS user preference.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_set_pref (
  uasms_bc_pref_e_type pref
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_srvc_enable
DESCRIPTION
  Enable/disable an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_set_srvc_enable (
  uasms_bc_service_id_type  srvc_id,
  boolean                   selected,
  uasms_priority_e_type     priority
);


/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_set_srvc_priority
DESCRIPTION
  Enable/disable an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_set_srvc_priority (
  uasms_bc_service_id_type  srvc_id,
  boolean                   selected,
  uasms_priority_e_type     priority
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_delete_srvc

DESCRIPTION
 Delete an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_delete_srvc (
  uasms_bc_service_id_type  srvc_id
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_add_srvc

DESCRIPTION
 Add an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_add_srvc (
  uasms_bc_service_info_type  srvc_info
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_change_label

DESCRIPTION
 Change the label of an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_change_label (
  uasms_bc_service_id_type  srvc_id,
  char *label
);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_get_all_service_ids

DESCRIPTION
 Get all the service IDs.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_get_all_service_ids (void);

/* <EJECT> */
/*===========================================================================

FUNCTION    uiuasms_bcsms_get_service_info

DESCRIPTION
 Change the information of an individual broadcast SMS service.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void uiuasms_bcsms_get_service_info (
  uasms_bc_service_id_type  srvc_id
);

#endif /* FEATURE_BROADCAST_SMS */


/*===========================================================================

FUNCTION    uiuasms_get_next_msg_id

DESCRIPTION
  Get the next msg id value for teleservice data.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  Increment uiuasms_msg_id

===========================================================================*/
word uiuasms_get_next_msg_id
(
  void
);


#ifdef FEATURE_RUIM_SMS
#error code not present
#endif /* FEATURE_RUIM_SMS */


#endif /* UIXUASMS_H */
#endif /* FEATURE_UASMS */

