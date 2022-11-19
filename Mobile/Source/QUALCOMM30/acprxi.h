#ifndef ACPRXI_H
#define ACPRXI_H
/*===========================================================================

                      ACPRX Unit Internal Header File

DESCRIPTION

  This header file contains all the definitions necessary in order
  for the modules of the ACPRX unit to interface with each other

Copyright (c) 1992,1993,1994 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1995 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*==========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/acprxi.h_v   1.1   21 Feb 2001 11:54:08   jqiu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/21/00   jq/lad  Cleaned up ACP logging so it uses the new DIAG interface.
12/12/95   jca     Added extern declaration for acprx_cmd_q.
06/28/95   jca     Added acprx_clk_cb_struct for FM Frequency Tracking
                   callback routine.
10/02/92   jjw     Added analog logging type definition
1/14/92    jjw     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "acpcmdef.h"
#include "acprx.h"
#include "log.h"
#ifdef T_MSM2P
#include "clk.h"
#endif /* T_MSM2P */

/* <EJECT> */
/*===========================================================================

                     REGIONAL DATA DECLARATIONS

===========================================================================*/

#ifdef T_MSM2P
extern clk_cb_type acprx_clk_cb_struct;
  /* Holds registration handle returned by Clock Services clk_reg function */
#endif /* T_MSM2P */

/* The ACPRX command queue is declared external here so that the RX task
** can check to make sure an item is on the queue before passing control
** to the ACPRX subtask. */

extern q_type acprx_cmd_q;


/*===========================================================================

                 REGIONAL FUNCTION DEFINITIONS

===========================================================================*/



/*==========================================================================

   FUNCTION ACPRXI_START_TIMER

   DESCRIPTION
     This function will set the timer associated with the passed signal to
     the specified duration. The time units are in milli-seconds. The
     specified signal will also be cleared (by rex_set_timer).


   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxi_start_timer
(
  acp_mask_type sig,                     /* signal associated with a timer */
  word duration                          /* timer interval, in msecs       */
);

/*==========================================================================

   FUNCTION ACPRXI_CANCEL_TIMER

   DESCRIPTION
     This function will cancel the timer associated with the specified signal
     mask. The specified signal will also be cleared

   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxi_cancel_timer
(
  acp_mask_type sig                 /* signal mask associated with a timer */
);

#if defined(FEATURE_DIAG_V7_LOG_IFACE)
/*==========================================================================

   FUNCTION ACPRXI_PUT_LOG

   DESCRIPTION
          This function will log the passed analog logging data.

   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxi_put_log
(
  log_analog_info_type *entry_ptr
);
#endif /* FEATURE_DIAG_V7_LOG_IFACE */


/*==========================================================================

   FUNCTION ACPRXI_GET_CMD

   DESCRIPTION
     This function will do the work necessary to remove the next command
     from the ACPRX command queue then copy it to the passed command buffer
     and free the queue link (ie. put the link onto the free queue.

   DEPENDENCIES
     None

   RETURN VALUE
     TRUE if a command was removed, FALSE if no command available

   SIDE EFFECTS
     None

===========================================================================*/

extern boolean acprxi_get_cmd
(
  acprx_cmd_type *cmd_ptr               /* ptr to buffer to load link into */
);

/*==========================================================================

   FUNCTION ACPRXI_CHECK_CMD

   DESCRIPTION
     This function will return (thru passed parameter) a pointer to the 1st
     item on the command queue, if the queue is not empty.

   DEPENDENCIES
     None

   RETURN VALUE
     TRUE if an item was found, else FALSE

   SIDE EFFECTS
     The passed cmd_ptr argument is only modified if an item was found

===========================================================================*/

extern boolean acprxi_check_cmd
(
  acprx_cmd_type *cmd_ptr       /* ptr to command buffer to assign link to */
);

#endif /* ACPRXI_H */

