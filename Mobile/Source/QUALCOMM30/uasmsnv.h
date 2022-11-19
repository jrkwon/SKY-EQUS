#ifdef FEATURE_BROADCAST_SMS

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 S M S   N V   O P E R A T I O N S

GENERAL DESCRIPTION

  This module provides the utility functions for the use by UASMS for accessing
  NV items needed for SMS processing.

EXTERNALIZED FUNCTIONS
  uasms_nv_read_wait
    Read an item from NV, handle non-active items

  uasms_nv_write_wait
    Write an item to NV

INITIALIZATION AND SEQUENCING REQUIREMENTS
  
  Copyright (c) 1999-2000 by QUALCOMM INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/uasmsnv.h_v   1.0   Aug 18 2000 11:46:26   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/17/00   HQ      Initial version for Broadcast SMS feature.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include <memory.h>

#include "uasmsi.h"
#include "err.h"
#include "nv.h"
#include "cmi.h"


/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/* <EJECT> */
/*===========================================================================
FUNCTION UASMS_NV_WRITE_WAIT

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from one task.
===========================================================================*/
nv_stat_enum_type uasms_nv_write_wait
(
  nv_items_enum_type item,        /* which item */
  nv_item_type       *data_ptr    /* pointer to data for item */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UASMS_NV_READ_WAIT

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from one task.
===========================================================================*/
nv_stat_enum_type uasms_nv_read_wait
(
  nv_items_enum_type item,        /* which item */
  nv_item_type       *data_ptr          /* pointer to space for item */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UASMS_NV_INIT

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from one task.
===========================================================================*/
void uasms_nv_init
(
  uasms_bc_info_type  *bc_ptr
);

#endif /* FEATURE_BROADCAST_SMS */
