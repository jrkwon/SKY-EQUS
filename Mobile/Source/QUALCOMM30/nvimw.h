#ifndef NVIMW_H
#define NVIMW_H
/*==========================================================================

  N V   I T E M    M A N A G E R    W R I T E   S E R V I C E S  
                                                     H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary for to interface
  with the Non Volatile Memory Write Services, in order to write NVM items
  using EFS.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvimw.h_v   1.0   10 Sep 1999 18:12:26   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08-13-99   rp      Initial ASW version.
07-06-99   rp      Added support for NV_FACTORY_INFO_I including function
                   nvimw_write_factory() which also supports poke command.  
06-23-99   rp      Cleaned up compiler warnings 
04-30-99   rp      Initial version.
   

===========================================================================*/


/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include <stddef.h>
#include "comdef.h"
#include "nv.h"
#include "nvi.h"
#include "nvim.h"
#include "fs.h"

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

FUNCTION NVIMW_WRITE_EFS

DESCRIPTION
  This function processes write requests for EFS.  

DEPENDENCIES
  None.

RETURN VALUE
  Status of write operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvimw_write_efs
(
  fs_handle_type        handle,         /* Handle of file to write to EFS */
  fs_file_position_type position,       /* Position to seek to */
  PACKED const void     *buffer,        /* Buffer with bytes to write */
  dword                 count           /* Number of bytes to write */
);


/*===========================================================================

FUNCTION NVIMW_WRITE

DESCRIPTION
  This function processes NVM write requests.  The function checks
  to make sure that a valid item has been requested, it performs any
  translations necessary from external format to internal NVM format
  and writes the item using EFS write services.

DEPENDENCIES
  None.

RETURN VALUE
  Status of write operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvimw_write
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);


/*===========================================================================

FUNCTION NVIMW_WRITE_FACTORY

DESCRIPTION
  This function writes data to the factory data block ($SYS.FACTORY file).
  It temporarily turns off the permanent file attribute on the $SYS.FACTORY
  file in order to write the data. 

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S if it worked
  Or the failure status from the lower levels.

SIDE EFFECTS
  None

===========================================================================*/

nv_stat_enum_type nvimw_write_factory
(
  PACKED void *data_ptr,  /* Pointer to write data */
  word size               /* Size of data to write */
);


/*===========================================================================

FUNCTION NVIMW_WRITE_RENTAL_TIMER

DESCRIPTION
  This function writes an arbitrary value to the rental timer item.  
  Since this item is expected to be updated with very high frequency,
  it is stored in a way that minimizes EEPROM cell wear.  This unusual
  storage format calls for a special routine to write and read the
  value.
  
  The NV "increment" command would normally be used to update this item;
  this routine should only be used to initialize the item to some desired
  specific value.
  
  
DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S    if it worked
  other        if a called function returns a failure status

SIDE EFFECTS
  None

===========================================================================*/
#ifdef NV_FEATURE_RENTAL_ITEMS
#error code not present
#endif


/*===========================================================================

FUNCTION NVW_INCREMENT

DESCRIPTION
  This function processes NVM item "increment" requests.  Currently, only
  the rental timer item can use this operation.  This special processing
  is needed because the rental timer item is an unusually high wear item,
  expected to be written very frequently.

DEPENDENCIES
  None.

RETURN VALUE
  Status of the increment operation.

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nvimw_increment
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);

#ifdef __cplusplus
}
#endif

#endif /* NVIMW.H */

