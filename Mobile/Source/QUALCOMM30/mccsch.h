#ifndef MCCSCH_H
#define MCCSCH_H
/*===========================================================================

    S U P P L E M E N T A L   C H A N N E L   ( S C H )   S E R V I C E S

GENERAL DESCRIPTION
  This module contains utilities related to operation of SCH.

   Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/




/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mccsch.h_v   1.6.1.1   10 May 2001 17:28:42   jqiu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/01   jq      Added mccsch_verify_fwd_assignment () 
05/02/01   fc      Added function prototypes mccsch_process_t_add_abort and 
                   mccsch_update_ignore_escam.
01/16/01   jq      Moved/renamed mccsch_retry_delay_type, mccsch_retry_delay to
                   mccdma.h.
01/10/01   lcc     Added data structures for use by mccsch_sch_config_inform
                   function.
09/18/00   jq      Moved mccsch_verify_rate() from mccsch.c to here.
08/28/00   fc      Featurized mccsch_reg_retry_delay() and
                   mccsch_update_retry_delay() under FEATURE_IS2000.
08/23/00   jq      Added mccsch_proc_r_dtx()
08/23/00   lcc     Added function prototype for mccsch_stop_f_sch0_trans_now.
07/25/00   fc      Added the prototype for mccsch_reg_ds_scrm_init().
07/10/00   fc      Changed scrm_blob_generator prototype to support STDSO.
06/19/00   fc      Added support for SCRM.
06/12/00   lcc     Added prototype for mccsch_sch_is_active.
02/23/00   lcc     Initial release.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#ifdef FEATURE_IS2000_R_SCH
#include "memory.h"
#endif /* FEATURE_IS2000_R_SCH */
#include "comdef.h"
#include "target.h"
#include "customer.h"

#include "cai.h"
#ifdef FEATURE_IS2000_R_SCH
#include "caii.h"
#include "mccsrch.h"
#endif /* FEATURE_IS2000_R_SCH */
#include "mctrans.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
// Type to specify forward or reverse
typedef enum
{
  MCCSCH_FORWARD,
  MCCSCH_REVERSE
} mccsch_ch_dir_type;

// Type for reporting new SCH operation
typedef struct
{
  boolean           changed;  // Indicates if this channel's operation has changed
  boolean           active;   // Indicates if channel on or off
  cai_sch_rate_type sch_rate; // If active, rate of the channel
} mccsch_sch_state_type;

// Type for reporting new SCH operation
typedef struct
{
  mccsch_sch_state_type f_sch_0;
  #ifdef FEATURE_IS2000_R_SCH
  mccsch_sch_state_type r_sch_0;
  #endif
} mccsch_sch_info_type;

/*===========================================================================

FUNCTION mccsch_initialize

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_initialize( void );

/*===========================================================================

FUNCTION mccsch_stop_f_sch0_trans

DESCRIPTION
  Function to generate a transaction to stop the SCH 0 processing.  This is
  call after the completion of a start transaction and is called from the
  transaction client's (MC) context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_stop_f_sch0_trans( void );

/*===========================================================================

FUNCTION mccsch_stop_f_sch0_trans_now

DESCRIPTION
  Function to generate a transaction to stop the SCH 0 processing immediately.
  This is used after a SHO which drops the last leg on F-SCH (shouldn't have
  happened though).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_stop_f_sch0_trans_now( void );

/*===========================================================================

FUNCTION mccsch_stop_r_sch0_trans

DESCRIPTION
  Function to generate a transaction to stop the R-SCH 0 processing.  This is
  call after the completion of a start transaction and is called from the
  transaction client's (MC) context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_stop_r_sch0_trans( void );

/*===========================================================================

FUNCTION mccsch_start_trans

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_begin_trans
(
  mctrans_transaction_type  trans_type,
  mctrans_pending_action_msg_type* msg
);

/*===========================================================================

FUNCTION mccsch_update_sch_assign_with_uhdm

DESCRIPTION
  Pre-processor for "stop f-sch 0" transaction.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_update_sch_assign_with_uhdm
(
  caii_ho_msg_type* uhdm
);

/*===========================================================================

FUNCTION mccsch_compute_start_time

DESCRIPTION
  Computes the start time of a start/stop SCH transaction.  "start_time"
  is as specified in the Standard which is in unit of "start_time_unit".

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_compute_start_time
(
  uint8               start_time,
  uint8               sch_id,
  mccsch_ch_dir_type  dir
);

/*===========================================================================

FUNCTION mccsch_compute_stop_time

DESCRIPTION
  Computes the stop time of a SCH burst assignment based on "duration" value,
  as defined in the Standard.


DEPENDENCIES
  Start time must have been computed before.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_compute_stop_time
(
  uint8               duration,
  uint8               sch_id,
  mccsch_ch_dir_type  dir
);

/*===========================================================================

FUNCTION mccsch_process_escam

DESCRIPTION
  Process message ESCAM received in various states in traffic channel.  It
  first validates the message and sends reject code if not valid.  Then it
  will go ahead a actually process the content of the message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_process_escam
(
  caii_escam_type* escam
);

/*===========================================================================

FUNCTION mccsch_sch_is_active

DESCRIPTION
   This function checks whether the specified SCH is active.  Note the SCH_ID is
   assumed to be valid.  If invalid SCH_ID is specified, return value is
   unpredictable.

DEPENDENCIES
  None.

RETURN VALUE
  True if SCH is active, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsch_sch_is_active
(
  mccsch_ch_dir_type  dir,
  uint8               sch_id
);

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================

FUNCTION mccsch_update_t_mulchan

DESCRIPTION
  This function updates SCRM pilot strength reporting offset.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_update_t_mulchan
(
  uint8 t_mulchan
);

/*===========================================================================

FUNCTION mccsch_update_ignore_escam

DESCRIPTION
  This function updates ignore ESCAM indicator.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_update_ignore_escam
(
  boolean ignore_escam
);

/*===========================================================================

FUNCTION mccsch_reg_sch_config_inform

DESCRIPTION
  This function binds a supplemental channel configuration inform function to
  a function pointer. If an old supplemental channel configuration inform
  function already exists, it overwrites it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_reg_sch_config_inform
(
  void (*ds_sch_config_inform)(mccsch_sch_info_type *)
);

/*===========================================================================

FUNCTION mccsch_reg_scrm_blob_generator

DESCRIPTION
  This function binds a SCRM blob generator to a function pointer. If an old
  SCRM blob generator already exists, it overwrites it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_reg_scrm_blob_generator
(
  boolean (*ds_scrm_blob_generator)(uint8* sz_blob, uint8* blob, uint16 so)
);
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================

FUNCTION mccsch_build_sup_chan_req_msg

DESCRIPTION
  This function builds a Supplemental Channel Request message from DS task's
  SCRM blob info and/or SRCH's task SCRM pilot strength measurement report.

DEPENDENCIES
  mccsch_scrm.sz_req_blob and mccsch_scrm.req_blob must already setup
  properly.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_build_sup_chan_req_msg
(
  caii_tx_msg_type *msg_ptr,
    /* Pointer to Supplemental Channel Request Message */
  mccsrch_rpt_type *srch_rpt_ptr,
    /* SRCH SCRM Pilot Strength Measurements or T_ADD_ABORT report */
  boolean use_scrm_seq_num
    /* Indicator whether to use SCRM sequence number */
);

/*===========================================================================

FUNCTION mccsch_process_scrm_request

DESCRIPTION
  This function gets SCRM blob from DS task. If the sise of SCRM blob is not
  zero, it commands SRCH task to send SCRM pilot strength measurement report.
  Otherwise, it sends SCRM right away.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_process_scrm_request(void);

/*===========================================================================

FUNCTION mccsch_reg_ds_scrm_init

DESCRIPTION
  This function binds a DS SCRM initialization function to a function pointer.
  If an old DS SCRM initializtion function already exists, it overwrites it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_reg_ds_scrm_init
(
  void (*ds_scrm_init)(void)
);

/*===========================================================================

FUNCTION mccsch_proc_r_dtx

DESCRIPTION
  This function is invoked when DTX is detected. It tears down the RSCH and
  sends a SCRM to the BS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_proc_r_dtx ( void );

/*===========================================================================

FUNCTION mccsch_process_t_add_abort

DESCRIPTION
  This function dispatches transaction to stop R-SCH at the end of the 
  current 20 ms frame, set ignore ESCAM indicator and send SCRM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsch_process_t_add_abort(mccsrch_rpt_type *);

#endif /* FEATURE_IS2000_R_SCH */

/*===========================================================================

FUNCTION mccsch_verify_rate

DESCRIPTION
  Function to verify rate versus SCH mux option

DEPENDENCIES
  None.

RETURN VALUE
  TURE if verification is successful.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsch_verify_rate
(
  uint16            sch_mux,
  cai_sch_rate_type sch_rate
);

/*===========================================================================

FUNCTION mccsch_verify_fwd_assigment

DESCRIPTION
  Function verify the sch_assign fwd entries after a HO is completed.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: if the assignment is still valid
  FALSE:  ..              .. not valid

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsch_verify_fwd_assignment(void);

#endif // MCCSCH_H

