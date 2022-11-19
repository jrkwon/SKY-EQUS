#ifndef ACPTXI_H
#define ACPTXI_H
/*===========================================================================

                   ACPTX Subtask Internal Interface

DESCRIPTION
   This header file includes function declarations which provide an
   interface layer to REX on behalf of the other ACPTX units.

Copyright (c) 1992,1993 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acptxi.h_v   1.0   Aug 11 2000 11:41:04   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/11/93   jca     Added function prototype for acptxi_wait().
07/08/92   jca     Moved signal declarations to TX.H.
01/10/92   jca     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "acpcmdef.h"
#include "acptx.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define HANDOFF_ST_DURATION 50
  /* 50 ms (CDMA CAI Sections 2.6.4.3.1, 2.6.4.3.2 and 2.6.4.4) */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION ACPTXI_START_TIMER

DESCRIPTION
  This function sets the timer associated with the specified mask to
  expire after the specified interval.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void acptxi_start_timer
(
  acp_mask_type timer_mask,
    /* The signal mask associated with the timer */
  word          interval
    /* The number of msecs after which the timer is to expire. */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPTXI_CANCEL_TIMER

DESCRIPTION
  This function cancels (clears) the timer associated with the given
  signal mask.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The timer's signal is also cleared.

===========================================================================*/

extern void acptxi_cancel_timer
(
  acp_mask_type timer_mask
    /* The signal mask associated with the timer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPTXI_CHECK_TIMER

DESCRIPTION
  This function returns the expiration count for the timer associated
  with the given signal mask.

DEPENDENCIES
  None

RETURN VALUE
  The time (in msecs) until the timer expires.  If the timer has expired
  the return value will be zero (0).

SIDE EFFECTS
  None
===========================================================================*/

extern word acptxi_check_timer
(
  acp_mask_type timer_mask
    /* The signal mask associated with the timer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPTXI_GET_CMD

DESCRIPTION
  This function retrieves the first command available on the ACPTX 
  command queue and places it at the address specified by the
  parameter 'cmd_ptr'.

DEPENDENCIES
  The command is one that is stored on the ACPTX command queue.

RETURN VALUE
  This function returns a boolean value of FALSE if the ACPTX command
  queue is empty, TRUE if a command was found and placed at the address
  indicated by 'cmd_ptr.'

SIDE EFFECTS
  The command is removed from the ACPTX command queue.

===========================================================================*/

extern boolean acptxi_get_cmd (
  acptx_cmd_type *cmd_ptr
    /* This is a pointer to to a buffer where the first available command
       on the ACPTX command queue will be placed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPTXI_CHECK_CMD

DESCRIPTION
  This function retrieves a copy of the first command available on the
  ACPTX command queue and places it at the address specified by the
  parameter 'cmd_ptr'.  The command is NOT removed from the command queue.
  
DEPENDENCIES
  The command is one that is stored on the ACPTX command queue.

RETURN VALUE
  This function returns a boolean value of FALSE if the ACPTX command
  queue is empty, TRUE if a command was found and placed at the address
  indicated by 'cmd_ptr.'

SIDE EFFECTS
  None
===========================================================================*/

extern boolean acptxi_check_cmd
(
  acptx_cmd_type *cmd_ptr
    /* This is a pointer to to a buffer where the first available command
       on the ACPTX command queue will be placed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPTXI_WAIT

DESCRIPTION
  This is the main wait routine for ACPTX subtask. This routine performs
  the following functions:

    - Checks the command queue to see if a command is available.  The
      command queue will be checked to see if anything is available
      before performing a rex_wait().  (There is no need to wait if
      something is on the queue already).  If an item is found on the
      queue the corresponding bit on the returned mask is set.

    - Kicks the watchdog timer while waiting for the specified mask

DEPENDENCIES
  None

RETURN VALUE
  An acp_mask_type value indicating all signals set (the current state
  of all signals).  The calling unit can then check for the reason for 
  exiting the wait.

SIDE EFFECTS
  None
===========================================================================*/

extern acp_mask_type acptxi_wait
(
  acp_mask_type requested_mask
    /* Mask of signals to wait for */
);

#endif

