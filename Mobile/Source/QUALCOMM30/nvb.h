#ifndef NVB_H
#define NVB_H
/*==========================================================================

          N V   B U I L D   S E R V I C E S   H E A D E R   F I L E

DESCRIPTION

  This header file contains all the definitions necessary to interface
  with the Non Volatile Memory Build Services, in order to build the
  NVM at powerup.

Copyright (c) 1990,1991,1992,1993,1994,1995,1996,1997,1998,1999 by QUALCOMM Incorporated.  
All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/nvb.h_v   1.1   10 Sep 1999 18:12:10   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/14/99   ny      Cleaned up the compilation to remove warnings/error messges.
                   Applied merge guidelines 3.0 to clean up the code
01/21/96   ras     pre-code review cleanup.
07/18/92    ip     Release after code review and initial unit test.
07/03/92    ip     First creation and release for code review.

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

FUNCTION NVB_BUILD_NVM

DESCRIPTION
  This function builds all NVM variables.  Each variable is built by
  allocating to it a free space and pointing a pointer to it.  All
  'active' flags are set to 'false'.

DEPENDENCIES
  nva_init() must have been called.

RETURN VALUE
  NV_DONE_S if it worked
  NV_FAIL_S if a variable could not be initialized.

LIMITATIONS
  Selective/Partial builds of the dynamic pool items are not supported.

SIDE EFFECTS
  NVM ready for writing after this call.  The exact handling
  of parameters after NV build is left to applications tasks.
  If nvm is allocated, several data structures global to the NV task,
  will be modified.

===========================================================================*/

extern nv_stat_enum_type nvb_build_nvm
(
  word first_item,   /* First item in enumerated item list to be created */
  word last_item     /* Last item in enumerated item list to be created */
);

#endif /* NVB.H */

