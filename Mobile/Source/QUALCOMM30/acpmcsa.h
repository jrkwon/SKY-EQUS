#ifndef ACPMCSA_H
#define ACPMCSA_H
/*===========================================================================

               ACPMC System Access Unit (acpmcsa) Interface

DESCRIPTION
   This header file includes declarations and functions which provide an
   interface to the ACPMC System Access Unit.

  Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcsa.h_v   1.0   Aug 11 2000 11:40:44   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/28/92   jca     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "acpmci.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPMCSA_STATE

DESCRIPTION
  This function is the entry point for the Analog Call Processing System
  Access Unit (ACPMCSA) state machine. This function determines which
  substate needs to be processed and vectors control to the appropriate
  function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void acpmcsa_state
(
  acpmc_state_info_type *state_info_ptr
);

#endif

