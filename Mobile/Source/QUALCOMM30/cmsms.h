#ifndef CMSMS_H
#define CMSMS_H
/*===========================================================================

         C A L L   M A N A G E R   S M S   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMSMS.c

Copyright (c) 1991 - 2001 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmsms.h_v   1.4   28 Feb 2001 18:00:06   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/01   HQ      Fixed compiler warnings.
02/06/01   RC      Added support for System Determination 2.0
01/29/01   HQ      Properly feature-ized UASMS, GPSONE_DBM and NEWCM.
08/23/00   vt      Added new CM interface changes.
03/16/00   VT      Added BROADCAST SMS support
08/31/99   SH      Added FEATURE_UASMS flags to featurize UASMS 
07/30/99   SH      Initial release.


===========================================================================*/


#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmmc.h"      /* MC to CM interface*/
/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#ifdef FEATURE_UASMS
#define MAX_SMS_HOLD_ORIG_TIME     30 /* 30 seconds for MO SMS hold orig retry*/
#endif
/*SMS type - holds all the information that is associated
** with a SMS object
*/

typedef struct cmsms_s  {

    cm_init_mask            init_mask;          /* Indicate whether this object was
                                                   properly initialized */

#ifdef FEATURE_UASMS

    cm_sms_state_e_type     sms_state;          /* SMS state for MO SMS */

    dword                   max_hold_orig_time; /* maximum allowed time for MO SMS hold
                                                   orig retry */

#ifdef FEATURE_NEWCM
#error code not present
#else
    cm_client_id_type       sms_client;         /* SMS client - i.e. requesting client
                                                   for sending MO SMS msg or AWI ACK */
#endif

    cm_sms_awi_ack_s_type   awi_ack;            /* field for awi ack */

    uint8                   mt_sms_format;      /* message format for MT SMS */

    uint16                  length;             /* length for SMS buffer - used by MT SMS only*/

    uint8                   *data;              /* pointer to sent/received SMS buffer */

    boolean                 l2_ack_requested;   /* l2 ack required or not - used by MO SMS*/

    cm_sms_mo_status_e_type mo_status;          /* status for MO SMS */

#ifdef FEATURE_BROADCAST_SMS

    boolean                 bc_enable;

#endif /* FEATURE_BROADCAST_SMS */

#else
    uint8                   msgs;               /* number of messages waiting
                                                   for CM_SMS_EVENT_MSG_WAITING */
    uint8                   data[14];           /* data field for
                                                   CM_SMS_EVENT_N_VOICE -valid data array index- 0 to 13*/
#endif /* FEATURE_UASMS */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

} cmsms_s_type;

/*===========================================================================

FUNCTION cmsms_ptr

DESCRIPTION
  Return a pointer to the one and only SMS object.


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cmsms_s_type  *cmsms_ptr( void );

/*===========================================================================

FUNCTION cmsms_init

DESCRIPTION
  Initializing the SMS object.

  This function must be called before the SMS object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsms_init( void );


/*===========================================================================

FUNCTION cmsms_mc_rpt_proc

DESCRIPTION
  Process MC reports (i.e. notifications of phone activity or
  replies to clients phone commands).


DEPENDENCIES
  SMS object must have already been initialized with
  cmsms_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsms_mc_rpt_proc(

    const cm_cmd_s_type    *cmd_ptr
        /* Pointer to a CM command */
);


#ifdef FEATURE_UASMS
/*===========================================================================

FUNCTION cmsms_mo_msg_retry

DESCRIPTION
  Retry mobile originated SMS message

DEPENDENCIES
  sms object must have already been initialized with
  cmsms_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsms_mo_msg_retry( void);

/*===========================================================================

FUNCTION cmsms_client_cmd_proc

DESCRIPTION
  Process clients' SMS commands.

DEPENDENCIES
  Phone object must have already been initialized with
  cmsms_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsms_client_cmd_proc(

    cm_cmd_s_type  *cmd_ptr
        /* pointer to a CM command */
);

/*===========================================================================

FUNCTION cmsms_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  SMS object must have already been initialized with
  cmsms_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmsms_timer_proc(

    cm_timer_event_type    timer_event
);



/*===========================================================================

FUNCTION cmsms_mt_msg

DESCRIPTION
 Process mobile terminated sms message

DEPENDENCIES
  none

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmsms_mt_msg (

    uint8      format,
         /* MT SMS message format */

    uint16     len,
         /* msg length */

    uint8      *data
          /* msg buffer pointer */
);
#endif /*FEATURE_UASMS*/


#endif /* CMSMS_H */

