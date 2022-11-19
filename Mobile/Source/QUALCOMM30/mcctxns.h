#ifndef MCCTXNS_H
#define MCCTXNS_H
/*===========================================================================

    M C   T R A N S A C T I O N   C L I E N T   F U N C T I O N S

GENERAL DESCRIPTION
  This module contains functions to support the handoff and service
  configuration transactions.

   Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/




/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/mcctxns.h_v   1.0   Aug 11 2000 11:44:06   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/17/00   va      Added interface for mcctxns_begin_cfs_trans function.
05/30/00   lcc     Initial release.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "customer.h"

#include "qw.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*===========================================================================

FUNCTION mcctxns_begin_scr_trans

DESCRIPTION
  Starts a SCR transaction.

DEPENDENCIES
  Pending service configuration must be valid.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_begin_scr_trans
(
  qword trans_time
);

/*===========================================================================

FUNCTION mcctxns_begin_ho_trans

DESCRIPTION
  Starts a HO transaction.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_begin_ho_trans
(
  qword trans_time
);

/*===========================================================================

FUNCTION mcctxns_begin_subchan_gain_trans

DESCRIPTION
  Starts a FPC_CHAN_GAIN transaction.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_begin_subchan_gain_trans
(
  qword trans_time
);

/*===========================================================================

FUNCTION mcctxns_begin_pwr_ctrl_trans

DESCRIPTION
  Starts a PWR_CTRL_MSG transaction.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_begin_pwr_ctrl_trans
(
  qword trans_time,
  mctrans_pending_action_msg_type* msg
);

/*===========================================================================

FUNCTION mcctxns_begin_cfs_trans

DESCRIPTION
  Starts a CFS transaction.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_begin_cfs_trans
(
  qword trans_time,
  mctrans_pending_action_msg_type* msg
);

/*===========================================================================

FUNCTION mcctxns_init_ffpc

DESCRIPTION
  Initializes the action packet for FFPC to included nothing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_init_ffpc
(
  mctrans_ffpc_action_type* ffpc
);

/*===========================================================================

FUNCTION mcctxns_init_rlgc

DESCRIPTION
  Initializes the action packet for RLGC to included nothing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcctxns_init_rlgc
(
  mctrans_rlgc_action_type* rlgc
);

#endif // MCCTXNS_H

