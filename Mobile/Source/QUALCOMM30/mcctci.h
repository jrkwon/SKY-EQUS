#ifndef MCCTC_H
#define MCCTC_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C D M A   T R A F F I C   C H A N N E L   S T A T E 
                       H E A D E R   F I L E

GENERAL DESCRIPTION
  This module externalizes data items needed by the support routines
  for Mobile Station Control on the Traffic Channel state.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/mcctci.h_v   1.1   27 Oct 2000 15:30:24   jqiu  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/27/00   jq      Merged T_AUTH to FEATURE_AUTH.
07/09/99   ry      Removed the extern definitions for the handoff state
                   indicators mcc_frame_offset_ho_pending and
                   mcc_hard_handoff_in_progress
05/30/96   dna     Created header.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"

/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Lookup table for Forward Power Control pwr_rep_frames parameter */
extern word pwr_rep_frames [];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Buffer for message with requested action time                           */
extern mccrxtx_msg_type tc_action_msg;

/* Buffer for Service Option Control Order with requested action time      */
extern mccrxtx_msg_type tc_action_so_ctl_order;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Power Control Reporting state indicators                                */

/* Periodic power control reporting state  */
extern boolean mcc_rx_periodic_rpt_on;

/* Threshold power control reporting state */
extern boolean mcc_rx_threshold_rpt_on;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Variables associated with Start and Stop Continous DTMF processing      */

extern boolean tc_stop;
  /* Indicates that when mcc_dtmf_timer expires a DTMF_STOP order must
     be transmitted */
extern boolean tc_ack_wait;
  /* Indicator that we are waiting for an ack to a Start or Stop Continous
     DTMF order or a Burst DTMF message */
extern byte tc_cont_dtmf;
  /* Pending continous DTMF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
extern db_items_value_type tc_value;
  /* Buffer for use in db_put calls */

#ifdef FEATURE_AUTH
extern boolean tc_private_lcm_active;
  /* Indicates which long code mask in active */
#endif /* FEATURE_AUTH */

extern word mar_rate;
    /* Markov rate to send to Markov Service Option */

#endif /* MCCTC_H */

