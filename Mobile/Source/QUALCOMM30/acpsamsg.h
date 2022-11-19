#ifndef ACPSAMSG_H
#define ACPSAMSG_H

/*===========================================================================
                        
                         A C P S A M S G

DESCRIPTION
  Contains ACPMCSA RECC message building routines.


  Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpsamsg.h_v   1.0   Aug 11 2000 11:40:58   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/09/92   jca     Initial release to DMSS PVCS.
05/11/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "acp553.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* <EJECT>  */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPSAMSG_BUILD_RECC_MSG

DESCRIPTION
  This function builds the RECC message which is included in the
  ACPTX_TX_RECC_MSG command.  The information that is to be included
  in the RECC message is determined as per CDMA CAI Section 2.6.3.7
  Service Request.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RECC message.

SIDE EFFECTS
  None

===========================================================================*/

byte acpsamsg_build_recc_msg
(
  acpmc_state_info_type *sa_state_info_ptr,
    /* Contains reason for entry to SA state along with associated data */
  acp553_txmsg_type     *tx_msg_ptr
    /* Pointer to RECC message */
);

#endif
