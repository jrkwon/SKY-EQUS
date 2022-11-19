#ifndef ACPVCSUP_H
#define ACPVCSUP_H

/*===========================================================================
                        
                         A C P V C S U P

DESCRIPTION
  Contains miscellaneous ACPMCVC support routines.


  Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpvcsup.h_v   1.0   Aug 11 2000 11:41:08   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/13/99   lh      CM and SMS phase 2 implementations.
07/09/92   jca     Initial release to DMSS PVCS.
05/15/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "acp553.h"
#include "acpmci.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* <EJECT>  */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPVCSUP_FADE_TIMER_EXPIRED

DESCRIPTION
  This function examines the ACPRX command report queue to see if
  the ACPRX task has issued a fade timer expiration command report.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the fade timer has expired.
  FALSE if the fade timer has NOT expired.
 
SIDE EFFECTS
  None

===========================================================================*/

extern boolean acpvcsup_fade_timer_expired( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCSUP_VALID_ORDER

DESCRIPTION
  This function determines if a valid mobile station control order
  has been received (while the mobile station is executing in the
  Conversation substate).

DEPENDENCIES
  None

RETURN VALUE
  TRUE is a valid order has been received; FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpvcsup_valid_order
(
  acp553_rxmsg_type *rx_msg_ptr
    /* Pointer to FVC message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCSUP_PROCESS_MSC_MSG

DESCRIPTION
  This function compares SCCs to the present SAT color code (PSCC) field
  in the received Forward Analog Voice Channel (FVC) message and, if a 
  valid mobile station control order has been received, calls the appropriate
  function for the action to be taken.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpvcsup_process_msc_msg
(
  acp553_rxmsg_type *rx_msg_ptr
    /* Pointer to FVC message */
);

#ifdef FEATURE_UASMS
/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCSUP_SEND_AWI_SMS_ACK

DESCRIPTION
  This function sends the ACK of ALERT W/Info message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  This function sends a message to acptx task.

===========================================================================*/
void acpvcsup_send_awi_sms_ack(mc_msg_type* cmd);
#endif /* FEATURE_UASMS */

#endif

