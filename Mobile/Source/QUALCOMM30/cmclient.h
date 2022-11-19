#ifndef CMCLIENT_H
#define CMCLIENT_H
/*===========================================================================

       C A L L   M A N A G E R   C L I E N T   H E A D E R   F I L E

DESCRIPTION
  This header file contains definitions necessary to internally interface
  with CMCLIENT.C.

  Note that definitions that are necessary to externally interface with
  CMCLIENT.C are provided in CM.H


Copyright (c) 1991 - 2001 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmclient.h_v   1.6   28 Feb 2001 18:00:08   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/01   HQ      Fixed compiler warnings.
01/29/01   HQ      Properly feature-ized UASMS, GPSONE_DBM and NEWCM.
01/23/01   ych     Merged JCDMA features.
10/26/00   PJ      Added prototype for cmclient_list_pd_event_ntfy, to fix
                   compiler warning. 
09/15/00   PJ      Added support for PD commands/events. (FEATURE_GPSONE_DBM).
08/23/00   vt      Added new CM interface changes.
08/31/99   SH      Modified definitions to featurize UASMS 
08/06/99   SH      Added SMS support
07/07/99   SH      Added prototype for cmclient_free_q_init
06/21/99   SH      Fixed incorrects argument names for cmclient_list_ss_cmd_err_ntfy
05/31/99   SH      Added CM 2.0 features
01/07/99   RC      Changes per ARM compiler.
11/24/98   RC      Initial release.

===========================================================================*/


#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */



/* <EJECT> */
/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/


/* <EJECT> */
/*---------------------------------------------------------------------------
                               CLIENT
---------------------------------------------------------------------------*/

/*===========================================================================

FUNCTION cm_client_ph_event_ntfy

DESCRIPTION
  Notify a client of a specified phone event iff the client is registered
  for this event.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_client_ph_event_ntfy(

    const cm_client_s_type     *client_ptr,
        /* pointer to a client struct */

    cm_ph_event_e_type         ph_event,
        /* notify client of this phone event */

    const cm_ph_info_s_type    *ph_info_ptr
        /* pointer to phone state information struct */
);

/*===========================================================================

FUNCTION cm_client_ss_event_ntfy

DESCRIPTION
  Notify a client of a specified serving system event if the client is
  registered for this event.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cm_client_ss_event_ntfy(

    const cm_client_s_type     *client_ptr,
        /* pointer to a client struct */

    cm_ss_event_e_type         ss_event,
        /* notify client of this serving system event */

    const cm_ss_info_s_type    *ss_info_ptr
        /* pointer to serving system state information struct */
);

/* <EJECT> */
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */


/* <EJECT> */
/*---------------------------------------------------------------------------
                                CLIENT LIST
---------------------------------------------------------------------------*/


/*===========================================================================

FUNCTION cmclient_list_init

DESCRIPTION
  Initializes the client list.

  This function must be called before using the client list is used in any
  way, place or form.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void cmclient_list_init( void );



/*===========================================================================

FUNCTION cmclient_list_call_event_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified call event.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmclient_list_call_event_ntfy(

    cm_call_event_e_type   call_event,
        /* notify clients of this call event */

    const cm_call_info_s_type    *call_info_ptr
        /* pointer to call state information struct */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmclient_list_call_cmd_err_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified call command
  error.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmclient_list_call_cmd_err_ntfy(

    cm_call_cmd_err_e_type      call_cmd_err,
        /* call command error code */

    const cm_call_cmd_s_type    *call_cmd_ptr
        /* pointer to call command struct */

);



/* <EJECT> */
/*===========================================================================

FUNCTION cmclient_list_ph_event_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified phone event.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmclient_list_ph_event_ntfy(

    cm_ph_event_e_type   ph_event,
        /* notify clients of this phone event */

    const cm_ph_info_s_type    *ph_info_ptr
        /* pointer to phone state information struct */
);

/* <EJECT> */
/*===========================================================================

FUNCTION cmclient_list_ss_event_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified serving system event.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmclient_list_ss_event_ntfy(

    cm_ss_event_e_type   ss_event,
        /* notify clients of this serving system event */

    const cm_ss_info_s_type    *ss_info_ptr
        /* pointer to serving system state information struct */
);


/* <EJECT> */
/*===========================================================================

FUNCTION cmclient_list_ph_cmd_err_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified phone command
  error.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmclient_list_ph_cmd_err_ntfy(

    cm_ph_cmd_err_e_type      ph_cmd_err,
        /* phone command error code */

    const cm_ph_cmd_s_type    *ph_cmd_ptr
        /* pointer to ph command struct */

);

/*===========================================================================

FUNCTION cmss_list_ss_cmd_err_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified serving system command
  error.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_list_ss_cmd_err_ntfy(

    cm_ss_cmd_err_e_type      ss_cmd_err,
        /* call command error code */

    const cm_ss_cmd_s_type    *ss_cmd_ptr
        /* pointer to ss command struct */

);

/*===========================================================================

FUNCTION cmclient_list_inband_cmd_err_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified inband command
  error.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_list_inband_cmd_err_ntfy(

    cm_inband_cmd_err_e_type      inband_cmd_err,
        /* phone command error code */

    const cm_inband_cmd_s_type    *inband_cmd_ptr
        /* pointer to inband command struct */

);

#ifdef FEATURE_UASMS

/*===========================================================================

FUNCTION cmclient_list_sms_cmd_err_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified SMS command
  error.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_list_sms_cmd_err_ntfy(

    cm_sms_cmd_err_e_type      sms_cmd_err,
        /* phone command error code */

    const cm_sms_cmd_s_type    *sms_cmd_ptr
        /* pointer to sms command struct */

);
#endif /* FEATURE_UASMS */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */


#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */
 
 
/* <EJECT> */
/*===========================================================================

FUNCTION cm_client_call_event_ntfy

DESCRIPTION
  Notify a client of a specified call event iff the client is registered
  for this event.

DEPENDENCIES
  Client must have already been initialized with cm_client_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_client_call_event_ntfy(

    const cm_client_s_type       *client_ptr,
        /* pointer to a client struct */

    cm_call_event_e_type         call_event,
        /* notify client of this call event */

    const cm_call_info_s_type    *call_info_ptr
        /* pointer to call state information struct */
);



/*===========================================================================

FUNCTION cmclient_list_inband_event_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified inband event.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_list_inband_event_ntfy(

    cm_inband_event_e_type         inband_event,
        /* notify clients of this inband event */

    const cm_inband_info_s_type    *inband_info_ptr
        /* pointer to phone state information struct */
);

/* <EJECT> */
/*===========================================================================

FUNCTION cmclient_list_sms_event_ntfy

DESCRIPTION
  Notify all the clients in the client list of a specified sms event.

DEPENDENCIES
  Client list must have already been initialized with cmclient_list_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_list_sms_event_ntfy(

    cm_sms_event_e_type         sms_event,
        /* notify clients of this sms event */

    const cm_sms_info_s_type    *sms_info_ptr
        /* pointer to sms information struct */
);

/* <EJECT> */
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */


/*===========================================================================

FUNCTION cmclient_free_q_init

DESCRIPTION
  Initialize the client free queue.

  This function must be called before the cmclinet free queue is used.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmclient_free_q_init( void );


#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

/* <EJECT> */
#ifdef CM_DEBUG
#error code not present
#endif /* CM_DEBUG */




#endif /* CMCLIENT_H */


