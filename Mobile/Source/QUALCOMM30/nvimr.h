#ifndef NVIMR_H
#define NVIMR_H
/*==========================================================================

  N V   I T E M    M A N A G E R    R E A D   S E R V I C E S  
                                                     H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary to interface
  with the Non Volatile Memory Read Services, in order to read NVM items
  using EFS.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvimr.h_v   1.0   10 Sep 1999 18:12:26   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08-13-99   rp      Initial ASW version.
07-06-99   rp      Added support for NV_FACTORY_INFO_I including function
                   nvimr_read_factory() which also supports peek command.  
06-23-99   rp      Cleaned up compiler warnings 
04-30-99   rp      Initial version.
   

===========================================================================*/


/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include <stddef.h>
#include "comdef.h"
#include "nvi.h"
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

FUNCTION NVIMR_READ_EFS

DESCRIPTION
  This function processes read requests for EFS.  

DEPENDENCIES
  None.

RETURN VALUE
  Status of read operation.

SIDE EFFECTS
  None.

===========================================================================*/

nv_stat_enum_type nvimr_read_efs
(
  fs_handle_type        handle,         /* Handle of file to write to EFS */
  fs_file_position_type position,       /* Position to seek to */
  PACKED void           *buffer,        /* Buffer to store read bytes */
  dword                 count           /* Number of bytes to read */
);


/*===========================================================================

FUNCTION NVIMR_READ_FACTORY

DESCRIPTION
  This function reads data from the factory data block ($SYS.FACTORY file).

DEPENDENCIES
  None.

RETURN VALUE
  NV_DONE_S if it worked
  Or the failure status from the lower levels.

SIDE EFFECTS
  None

===========================================================================*/

nv_stat_enum_type nvimr_read_factory
(
  PACKED void *data_ptr,  /* Pointer to read data */
  word offset,            /* Start offset into factory data area for read */
  word size               /* Size of data to read */
);


/*===========================================================================

FUNCTION NVIMR_READ

DESCRIPTION
  This procedure processes NVM read requests.  It checks to make sure
  that a valid item has been requested, it reads the NVM item using EFS
  services and it performs the translation from internal NVM format to
  external format. 
  
DEPENDENCIES
  None.

RETURN VALUE
  Status of read operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvimr_read
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);

#ifdef __cplusplus
}
#endif

#endif /* NVIMR.H */

