#ifndef DSSOCTL_H
#define DSSOCTL_H
/*===========================================================================

                         D A T A   S E R V I C E S
                           
                 S E R V I C E   O P T I O N   C O N T R O L

                          H E A D E R   F I L E

DESCRIPTION
  This file contains interface functions for handling service option control
  messages related to data service options. Currently, only the Packet Data
  Dormant Timer control feature is supported. Other service option control
  messages for Packet/Async Data/Fax service options are not supported.


   Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/dssoctl.h_v   1.0   23 Oct 2000 17:32:20   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20//99  rc      Added prototypes for ds_soctl_reg_pzid_recon and 
                   ds_soctl_dereg_pzid_recon for PZID based reconnection.
08/20/99   na      Added prototype for ds_soctl_disable_pzid_recon().
02/18/99   smp     Changed SID handler parameters to word instead of byte.
02/10/99   smp     Created file.

===========================================================================*/

#include "snm.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Packet Data Dormant Timer value (in milliseconds).
---------------------------------------------------------------------------*/
extern word ds_dormant_timer;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION DS_SOCTL_HANDLE_SOCTL_MSG

DESCRIPTION
  This function processes service option control messages related to data
  service options. Currently, the only service option control message that is
  supported is for Packet Data Dormant Timer control.

DEPENDENCIES
  None

RETURN VALUE
  Returns VALID if the message was processed successfully, or the appropriate
  reason for failure (MSG_TYP/NOT_SUPPORTED) if some type of error occurred.

SIDE EFFECTS
  The value of ds_dormant_timer may change as a result of processing the 
  Packet Data dormant timer service option control message.

===========================================================================*/

extern snm_socm_return_type ds_soctl_handle_soctl_msg
(
  word length,
    /* Length of the type-specific fields in the service option control
       message, in bytes */

  byte *soctl_msg,
    /* Pointer to the type-specific fields in the service option control
       message */

  word service_option
    /* The service option that is currently connected */
);

/*===========================================================================

FUNCTION DS_SOCTL_SID_CHANGE_HANDLER

DESCRIPTION
  This function performs the necessary actions, in relation to data service
  options, when the System ID changes. When a SID change is detected, the
  Packet Data Dormant Timer is reset to its default value, per IS-707-A.5/9,
  Section 2.2.6.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Resets the ds_dormant_timer value to its default value.

===========================================================================*/

void ds_soctl_sid_change_handler
(
  word old_sid,
    /* Old System Id */

  word new_sid
    /* New System Id */
);
#ifdef FEATURE_DS_PZID_RECON
/*===========================================================================

FUNCTION DS_SOCTL_DISABLE_PZID_RECON

DESCRIPTION
  Disables packet zone id based reconnection for packet calls.
  This function deregisters the soctli_pzid_active_change_cb() and the
  soctli_pzid_idle_change_cb(). The list of visited packet zones is also 
  cleared.
  
  This function is called when (see sec.2.2.7 of IS-707.A.5):
  (a) BS sends a SOCTL message disable PZID reconnection
  (b) BS sends a PZID of 0.
  (c) MS comes out of the active state - PPP session has ended.
  (d) MS detects a change in SID.
  
  This function can be called from either the DS or the MC task (from the
  callbacks).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  ds_soctl_disable_pzid_recon
(
  void
);

/*===========================================================================

FUNCTION DS_SOCTL_REG_PZID_RECON

DESCRIPTION
  This function registers PZID based reconnection for packet calls.
  It registers the active and idle change callbacks and initializes the PZID
  list to one entry.
  
  This function is called when (see sec.2.2.7 of IS-707.A.5):
  (a) A packet call (sockets/relay/network) is initiated (packet service
  goes into active state).
  
  Note, the idle change handler will reoriginate a packet call on detecting
  a PZID change.
          
  This function can be called from the DS task

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void  ds_soctl_reg_pzid_recon
(
  void
);

/*===========================================================================

FUNCTION DS_SOCTL_DEREG_PZID_RECON

DESCRIPTION
  This function deregisters PZID based reconnection for packet calls.
  It sets the active and idle PZID change handlers to NULL and resets the 
  visited PZID list.
  
  This function is called when (see sec.2.2.7 of IS-707.A.5):
  (a) A packet call (sockets/relay/network) is ended (packet service
  goes into Inactive state) - not called when packet data service goes 
  dormant.
  
  This function can be called from the DS task

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void  ds_soctl_dereg_pzid_recon
(
  void
);


#endif /* FEATURE_DS_PZID_RECON */

#endif /* DSSOCTL_H */
