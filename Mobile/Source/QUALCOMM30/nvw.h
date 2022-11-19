#ifndef NVW_H
#define NVW_H
/*==========================================================================

         N V   W R I T E   S E R V I C E S    H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary for to interface
  with the Non Volatile Memory Write Services, in order to write items to NVM.

Copyright (c) 1990,1991,1992,1993,1994,1995,1996,1998,1999 by QUALCOMM 
Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvw.h_v   1.1   10 Sep 1999 18:12:20   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/99   ny      Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
07/27/98   ls      Added rental timer item write function.
01/21/96   ras     Pre-code review cleanup
05/09/95   ras     Initial next generation nv checkin
07/18/92    ip     Release after code review and initial unit test.
07/03/92    ip     First creation and release for code review.
02/24/92    ip     Initial porting from BrassBoard to Subscriber Station.

===========================================================================*/


/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "nvi.h"


/*===========================================================================
 
                         DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                       FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION NVW_WRITE

DESCRIPTION
  This function processes NVM write requests.  The function checks
  to make sure that a valid item has been requested, it performs any
  translations necessary from external format to internal NVM format
  and writes the item to NVM.  If the write fails, the item will be
  relocated.

DEPENDENCIES
  None.

RETURN VALUE
  Status of write operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvw_write
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);


/*===========================================================================

FUNCTION NVW_WRITE_RENTAL_TIMER

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

nv_stat_enum_type nvw_increment
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);

#endif /* NVW.H */

