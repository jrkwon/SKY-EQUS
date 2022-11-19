#ifndef AUTHI_H
#define AUTHI_H
/*===========================================================================

               I N T E R N A L   A U T H E N T I C A T I O N
                      T A S K   D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the Authentication Task.

Copyright (c) 1995, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/authi.h_v   1.0   Aug 11 2000 11:41:14   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/29/95   dna     Created Module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "queue.h"
#include "qw.h"
#include "cmd.h"
#include "cai.h"
#include "caii.h"
#include "ulpn.h"
#include "nv.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Status of auth initialization */
typedef enum {
  AUTH_UNINITIALIZED,               /* Init has not completed successfully */
  AUTH_INIT_OK,                     /* Normal initialization has been done */
  AUTH_BAD_SSD                      /* No SSD has been generated */
} auth_init_status_type;

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION AUTH_GET_NV_ITEM

DESCRIPTION
  This function gets a specific item from NV.

DEPENDENCIES
  None.

RETURN VALUE
  Status returned from the NV read request.  Status can be either:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active
  Any other status causes ERR_FATAL and the function does not return.

SIDE EFFECTS
  While this function is running all other task activities are
  suspended until the NV item is read in.  If a read status other
  than the ones listed above is returned, ERR_FATAL is called.

===========================================================================*/

extern nv_stat_enum_type auth_get_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get */
  nv_item_type        *item_ptr        /* Pointer where to put the item */
);

/* <EJECT> */
/*===========================================================================

FUNCTION AUTH_PUT_NV_ITEM

DESCRIPTION
  This function puts a specific item to NV.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  While this function is running all other task activities are
  suspended until the NV item is written.

===========================================================================*/

extern void auth_put_nv_item
(
  nv_items_enum_type  item_code,       /* Item to put */
  nv_item_type        *item_ptr        /* Pointer where to get the item */
);

#endif /* AUTHI_H */
