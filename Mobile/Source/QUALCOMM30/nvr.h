#ifndef NVR_H
#define NVR_H
/*==========================================================================

         N V   R E A D   S E R V I C E S    H E A D E R    F I L E

DESCRIPTION

  This header file contains all the definitions necessary for to interface
  with the Non Volatile Memory Read Services, in order to read items from NVM.

Copyright (c) 1990,1991,1992,1993,1994,1995,1996,1997,1998,1999 by QUALCOMM Incorporated.  
All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvr.h_v   1.1   10 Sep 1999 18:12:16   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/99   ny      Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
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

FUNCTION NVR_READ

DESCRIPTION
  This function processes NVM read requests.  The function checks
  to make sure that a valid item has been requested, it performs any
  translations necessary from internal NVM format to external format
  and it reads the item from NVM.

DEPENDENCIES
  None.

RETURN VALUE
  Status of read operation.

SIDE EFFECTS
  None.

===========================================================================*/

extern nv_stat_enum_type nvr_read
(
  nv_cmd_type  *cmd_ptr          /* Command block */
);

#endif /* NVR.H */

