#ifndef DLOAD_H
#define DLOAD_H
/*===========================================================================

              B O O T   D O W N L O A D   S E R V I C E S
                      H E A D E R    F I L E 

DESCRIPTION
  This file contains declarations necessary to start the boot download
  services.

Copyright (c) 1993-1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/MSM5000/VCS/dload.h_v   1.1   10 Sep 1999 18:10:26   lchan  $

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/16/99   kmp     Removed keyword _cdecl from prototype.
08/11/99   kk      Merge from SS baseline.
07/22/93   jah     Changed entry point routine from download() to dload_entry()
                   per code review issue with namespace pollution.
01/11/93   jah     Initial revision

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION DLOAD_ENTRY

DESCRIPTION
  Do a bootstrap download.  Initialize the MSM dataport and perform memory
  operations at the direction of an external agent via the MSM dataport.

DEPENDENCIES
  Called from a DMSS Task during offline mode, or as a part of the Boot
  module's error recovery.

RETURN VALUE
  This routine does not return.  It may branch to a new program loaded
  into RAM.

SIDE EFFECTS
  The watchdog timer is reset, interrupts are disabled.

===========================================================================*/
extern void dload_entry( void );

#endif  /* DLOAD_H */
