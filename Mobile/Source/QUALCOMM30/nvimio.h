#ifndef NVIMIO_H
#define NVIMIO_H
/*==========================================================================

         N V  I T E M    M A N A G E R  I / O  H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary to interface
  with the NV Item Manager I/O services.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvimio.h_v   1.0   10 Sep 1999 18:12:22   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08-13-99   rp      Initial ASW version.
06-23-99   rp      Now including comdef.h for PACK 
04-30-99   rp      Initial version.
   

===========================================================================*/


/* ======================================================================

                       INCLUDE FILES FOR MODULE

========================================================================*/
#include <stddef.h>
#include "nv.h"                  /* for nv_item_type */
#include "nvi.h"
#include "nvim.h"
#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 
                         DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                       FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION NVIMIO_FIXED

DESCRIPTION
  Read/Write a fixed NVM item to the EFS.

DEPENDENCIES

RETURN VALUE
  Status of the I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvimio_fixed
(
  PACKED void     *value,           /* Data buffer for I/O operation */
  word            value_size,       /* # bytes of data to be transfered */
  nv_io_enum_type read_write,       /* I/O type */
  const nvim_item_location_type *fs_item /* EFS file details for the item */
);


/*===========================================================================

FUNCTION NVIMIO_FIXED_ARRAY

DESCRIPTION
  Read/Write a fixed NVM array item to the EFS.
 
DEPENDENCIES

RETURN VALUE
  Status of I/O operation.

SIDE EFFECTS
  None.

===========================================================================*/
extern nv_stat_enum_type nvimio_fixed_array
(
  byte                 index_range,   /* Maximum allowed range for index */
  byte                 index,         /* Item index within its array */
  PACKED void          *value,        /* Data buffer for I/O operation */
  word                 value_size,    /* # of bytes to be transfered */
  nv_io_enum_type      read_write,    /* I/O type */
  const nvim_item_location_type *fs_item /* EFS file details for the item */
);


/*===========================================================================

FUNCTION NVIMIO_IO

DESCRIPTION
  Initiate the correct type of I/O operation for the specified item.

DEPENDENCIES

RETURN VALUE
  Status of I/O operation.

SIDE EFFECTS

===========================================================================*/
extern nv_stat_enum_type nvimio_io
(
  nv_cmd_type  *cmd_ptr,          /* Command block */
  nv_io_enum_type io_type         /* read/write/? */
);

#ifdef __cplusplus
}
#endif

#endif /* NVIMIO.H */

