#ifndef ACPRXCMD_H
#define ACPRXCMD_H
/*===========================================================================

                   A C P R X C M D     H E A D E R

GENERAL DESCRIPTION
  This is the header file to interface with the ACPRXCMD command processing
  unit of the ACPRX sub-task.


       Copyright (c) 1992 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/common/shared/cp/vcs/acprxcmd.h_v   1.0   Aug 11 2000 11:40:52   lpetrucc  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/18/98   br      Added acprxcmd_state_end() function declaration.
02/28/92   jjw     creation

===========================================================================*/


#include "comdef.h"
#include "acprx.h"
#include "acpcmdef.h"


/*===========================================================================

                     PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                  PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*==========================================================================

   FUNCTION ACPRXCMD_STATE_MACH

   DESCRIPTION
     This is the entry point and main processing loop for the ACPRX state
     machine. This state machine handles ACPRX command reception and command
     processing. This processing includes monitoring the SAT color code when
     enabled via command and setting the fade timeout condition when it
     occurs.

   DEPENDENCIES
     The state machine must be initialized via 'acprxcmd_state_init' prior to
     invoking this function for the first time.

   RETURN VALUE
     TRUE if sub-task has completed SHUTDOWN command processing, else FALSE

   SIDE EFFECTS
     None

===========================================================================*/

extern boolean acprxcmd_state_mach
(
  acp_mask_type          *set_sigs          /* Pointer to set signals mask */
);

/*==========================================================================

   FUNCTION ACPRXCMD_STATE_INIT

   DESCRIPTION
     This function prepares the ACPRX command processing state machine for
     command processing. It must be called prior to invoking the ACPRX state
     machine function for the first time.

   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxcmd_state_init
(
  void
);


/*==========================================================================

   FUNCTION ACPRXCMD_STATE_END

   DESCRIPTION
     This function aknowledges ACPMC that Analog Receive processing is
     terminated.

   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxcmd_state_end
(
  void
);

#endif
