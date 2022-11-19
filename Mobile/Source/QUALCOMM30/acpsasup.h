#ifndef ACPSASUP_H
#define ACPSASUP_H

/*===========================================================================
                        
                         A C P S A S U P

DESCRIPTION
  Contains miscellaneous ACPMCSA support routines.


  Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpsasup.h_v   1.0   Aug 11 2000 11:41:00   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/09/02   jca     Initial release to DMSS PVCS.
05/08/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
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

FUNCTION ACPSASUP_PROCESS_OVERHEAD_MSG

DESCRIPTION
  This function performs processing of a received overhead message train
  as specified in CAI Section 2.6.3.4 Update Overhead Information.

DEPENDENCIES
  The address pointed to by rxmsg_ptr must contain an overhead message.

RETURN VALUE
  TRUE  - Continue to process the Update Overhead Information task.
  FALSE - Exit the Update Overhead Information task and enter the
          Serving System Determination task.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpsasup_process_overhead_msg
(
  acpmc_state_info_type *sa_state_info_ptr,
    /* Contains reason for entry to SA state along with associated data */
  acp553_rxmsg_type     *rxmsg_ptr
    /* Pointer to FOCC overhead message */
);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPSASUP_PROCESS_IVCD_MSG

DESCRIPTION
  This function performs processing of an Initial Voice Channel Designate
  Message as specified in CAI Section 2.6.3.8 Await Message.

DEPENDENCIES
  The address pointed to by rxmsg_ptr must contain an Initial Voice
  Channel Designate Message.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpsasup_process_ivcd_msg
(
  acp553_rxmsg_type  *rxmsg_ptr
    /* Pointer to Initial Voice Channel Designate message */
);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPSASUP_PROCESS_DIR_RETRY_MSG

DESCRIPTION
  This function performs processing of a received Directed-Retry Message
  as specified in CAI Section 2.6.3.8 Await Message.

DEPENDENCIES
  The address pointed to by rxmsg_ptr must contain a Directed-Retry Message.

RETURN VALUE
  Length of CCLISTs (list of control channels to be scanned) provided by
  the received Directed-Retry Message.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acpsasup_process_dir_retry_msg
(
  acp553_rxmsg_type  *rxmsg_ptr
    /* Pointer to Directed Retry Order */
);

#endif
