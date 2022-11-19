#ifndef ACPVCMSG_H
#define ACPVCMSG_H

/*===========================================================================
                        
                         A C P V C M S G

DESCRIPTION
  Contains ACPMCVC RVC (Reverse Voice Channel) message building routines.


Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpvcmsg.h_v   1.0   Aug 11 2000 11:41:06   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/07/95   dna     Added extern declarations for authentication support
01/27/94   jca     Added function acpvcmsg_build_ep_response_msg().
07/09/92   jca     Initial release to DMSS PVCS.
05/15/92   jca     Creation

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

FUNCTION ACPVCMSG_BUILD_ORDER_CONF_MSG

DESCRIPTION
  This function builds an Order Confirmation reverse voice channel message
  as specified in CAI Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acpvcmsg_build_order_conf_msg
(
  acp553_txmsg_type *tx_msg_ptr,
    /* Pointer to RVC message */
  byte              order_field,
     /* Value to place in Order Code field     */
  byte              ordq_field,
    /* Value to place in Order Qualifier field */
  byte              msg_type_field
    /* Value to place in Message Type field    */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCMSG_BUILD_CALLED_ADDR_MSG

DESCRIPTION
  This function builds a Called-Address reverse voice channel message
  as specified in CAI Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/

extern byte acpvcmsg_build_called_addr_msg
(
  acp553_txmsg_type            *tx_msg_ptr,
    /* Pointer to RVC message */
  acpmc_origination_param_type *msg_data_ptr
    /* Pointer to called-address data */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCMSG_BUILD_SER_NUM_RSP_MSG

DESCRIPTION
  This function builds a Serial Number Response reverse voice channel
  message as specified in CAI Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/

extern byte acpvcmsg_build_ser_num_rsp_msg
(
  acp553_txmsg_type            *tx_msg_ptr
    /* Pointer to RVC message */
);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPVCMSG_BUILD_EP_RESPONSE_MSG

DESCRIPTION
  This function builds an Extended Protocol Order confirmation reverse
  voice channel message as specified in IS-88 Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acpvcmsg_build_ep_response_msg
(
  acp553_txmsg_type *tx_msg_ptr,
    /* Pointer to RVC message */
  byte              mst_field
    /* Value to place in MST field */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPVCMSG_BUILD_BS_CHAL_MSG

DESCRIPTION
  This function builds a Base Station Challenge reverse voice channel
  message as specified in CAI Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/

extern byte acpvcmsg_build_bs_chal_msg
(
  acp553_txmsg_type            *tx_msg_ptr
    /* Pointer to RVC message */
);

/* <EJECT>  */
/*===========================================================================

FUNCTION ACPVCMSG_BUILD_UNIQUE_CHAL_MSG

DESCRIPTION
  This function builds a Unique Challenge Confirmation voice channel message
  as specified in CAI Section 2.7.2.1 RVC Messages.

DEPENDENCIES
  None

RETURN VALUE
  Count of how many transmit words are contained in the RVC message.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acpvcmsg_build_unique_chal_msg
(
  acp553_txmsg_type *tx_msg_ptr
    /* Pointer to RVC message */
);
#endif
