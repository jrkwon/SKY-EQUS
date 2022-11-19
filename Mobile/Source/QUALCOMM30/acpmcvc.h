#ifndef ACPMCVC_H
#define ACPMCVC_H
/*===========================================================================

               ACPMC Voice Channel Unit (ACPMCVC) Interface

DESCRIPTION
   This header file includes declarations and functions which provide an
   interface to the ACPMC Voice Channel Unit.

  Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcvc.h_v   1.0   Aug 11 2000 11:40:48   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/09/92   jca     Initial release to DMSS PVCS.
02/18/92   jca     Created file.

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

FUNCTION ACPMCVC_STATE

DESCRIPTION
  This function provides the entry point and contains the main processing
  loop for the Analog Call Processing Voice Channel Unit (ACPMCVC) state
  machine.  This function determines which Voice Channel substate needs to
  be processed and vectors control to the appropriate substate processing 
  function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void acpmcvc_state
(
  acpmc_state_info_type *state_info_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCVC_GET_SUBSTATE

DESCRIPTION
  Returns the current voice channel substate.

DEPENDENCIES
  None

RETURN VALUE
  Current state

SIDE EFFECTS
  None

===========================================================================*/

extern byte acpmcvc_get_substate( void );

#endif
