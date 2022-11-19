#ifndef MCCTCHO_H
#define MCCTCHO_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C D M A   T R A F F I C   C H A N N E L   S T A T E
         H A N D O F F   R O U T I N E S   H E A D E R   F I L E

GENERAL DESCRIPTION
  This module externalizes the handoff support routines for the CDMA
  Sub-system, Mobile Station Control on the Traffic Channel state.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mcctcho.h_v   1.2   26 Feb 2001 16:53:56   marci  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/26/01   mai     Changed FEATURE_GPSONE to FEATURE_GPS_MODE to move code needed
                   only for GPS part of position location to a new #define.
08/23/00   lcc     Changed mcctcho_generate_aset to return sup_state and removed
                   transaction_num from function parameter.
07/28/00   fc      Mainline FEATURE_CS_ACTION_TIME_TRANS.
07/17/00   va      Support for 8th rate gating and rev pwr cntl delay.
06/19/00   yll     Added GPS support: Provide a function to check if a handoff
                   or single shot CFS is in progress.
06/12/00   lcc     Minor changes to mcctcho_ho_data_type as part of unifying
                   transaction processing.
05/25/00   ks      Added support for Neighbor Search Window offset.
04/13/00   lcc     Added definition for MCCTCHO_IMPLICIT_ACTION_TIME.
04/11/00   cah     Featurize CFS items in the mcctcho_pending_msg_type under
                   the FEATURE_IS95B_MAHHO define.
03/24/00   ry      Added IS2000 featurization
           ry      Merged the following from MSM3100_CP.02.00.10:
           dna     CR12461 Changed tf_aset to cf_srch_results, since it stores
                   results from CF search as well as failed handoffs. Increased
                   size from 6 to 46 to allow for CF search set to fit.
           vn      Merged in the following from trimode build TM1005:
           ck      Featurised different_band_ho under Trimode.
           ck      Added band class to mcctcho_serv_freq_config_type.
                   Added the flag different_band_ho to mcctcho_ho_data_type.
03/06/00   lcc     Included ho_msg in mcctcho_data_type and removed unnecessary
                   members.
12/20/99   va      Support for restoring fpc_subchan_gain correctly.
11/15/99   va      IS2000 Changes:
                   mcctcho_ho_data has been extended so that all parameters needed for
                   post processing HO is stored there.
10/28/99   ry      Updated mcctcho_cand_freq_srch_off_time_type such that all
                   off times are defined as byte, instead of word
10/12/99   ry      Added srv_cfg to mcctcho_serv_freq_config_type
           ry      Added service_included and restart_periodic_srch to
                   mcctcho_ho_data_type
10/11/99   ry      Removed aset from mcctcho_serv_freq_config_type
10/05/99   ry      Removed mcctcho_action_time_pending()
08/19/99   ry      Added mcctcho_cand_freq_srch_off_time_type
           ry      Added last_srch_msg_seq to mcctcho_cand_freq_srch_parm_type
           ry      Renamed cfs_msg_seq to cur_srch_seq_num
08/13/99   doj     Added pwr_cntl_step to mcctcho_ho_data_type.
08/09/99   ry      Updated mcctcho_cand_freq_srch_parm_type to support both
                   single and periodic Candidate Frequency Search
           ry      Added tf_band_class to mcctcho_ho_data_type
           ry      Added extern definitions for mcctcho_start_cf_meas() and
                   mcctcho_process_cand_freq_srch_ctrl_msg()
08/08/99   ry      Changed FEATURE_IS95B_HARD_HO to FEATURE_IS95B_MAHHO
           ry      Changed FEATURE_IS95B_CFS to FEATURE_IS95B_MAHHO
           ry      Reuse the structure mcc_ho_cb_type inside
                   mcctcho_serv_freq_config_type, instead of defining each
                   of the handoff parameters separately
           ry      Moved mcctcho_hard_handoff_in_progress,
                   mcctcho_hard_handoff_successful, and
                   mcctcho_cf_srch_completed into the structure
                   mcctcho_ho_data_type
06/09/99   ry      Added new data structures to support IS95B hard handoff and
                   candidate frequency search
           ry      Added extern definitions for the following functions:
                     mcctcho_action_time_pending()
                     mcctcho_process_failed_hho()
                     mcctcho_return_to_serv_freq()
                     mcctcho_post_process_handoff()
                     mcctcho_process_txc_frame_sig()
                     mcctcho_process_srch_cf_nset_rpt()
                     mcctcho_process_cand_freq_srch_req_msg()
05/18/99   doj     Created module from pieces of MCCTCSUP.H to consolidate
                   handoff processing.  Look in MCCTCSUP.H (v1.2) for history
                   prior to this date.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "customer.h"
#ifdef FEATURE_IS2000
  #include "mccccl.h"
#endif
#include "mccdma.h"
#include "mccrxtx.h"
#include "mccsrch.h"
#include "srch.h"


/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define MCCTCHO_IMPLICIT_ACTION_TIME  0xff

typedef enum
{
  REJECT_HANDOFF_MESSAGE, /* We should reject this HO message. */
  OKAY_TO_HANDOFF,        /* It is okay to handoff right away. */
  WAIT_TO_HANDOFF         /* We need to wait for proper time to handoff. */
} handoff_action_type;

typedef struct
{
  word    pilot_pn;          /* Pilot PN sequence offset index */
  boolean pwr_comb_ind;      /* Power control symbol combining indicator */
  byte    code_chan;         /* Fundamental Code channel index */
  word    ecio;              /* Pilot strength - used to report the TF Active
                                Set in the case of failed HHO */
} mcctcho_pilot_type;

typedef struct
{
  word num_pilots;              /* Number of PNs in the Active Set */
  mcctcho_pilot_type pilot[SRCH_NSET_MAX + SRCH_ASET_MAX];
                                /* Store search results of from handoff active
                                 * set and/or candidate frequency search set */
} mcctcho_cf_srch_result_type;


#ifdef FEATURE_IS95B_MAHHO
typedef enum
{
  MCCTCHO_HANDOFF_TIMER_EXPIRE,
  MCCTCHO_DIFF_RX_PWR_TOO_LOW,
  MCCTCHO_TOTAL_PILOT_EC_IO_TOO_LOW
} mcctcho_hho_failure_reason_type;

typedef struct
{
  word cdma_freq;
  byte band_class;
  byte frame_offset;
#ifdef FEATURE_AUTH
  boolean private_lcm;
#endif /* FEATURE_AUTH */
  caii_complete_srv_cfg_type srv_cfg;
  mcc_ho_cb_type ho_parm;
#ifdef FEATURE_IS2000
  boolean scr_incl;
#endif /* FEATURE_IS2000 */
} mcctcho_serv_freq_config_type;

/* The various off times that are associated with a CFS */
typedef struct
{
  byte tot_off_time_fwd;
                /* Total time that the MS is off the Forward Traffic Channel */
  byte max_off_time_fwd;
              /* Maximum time that the MS is off the Forward Traffic Channel */
  byte tot_off_time_rev;
                /* Total time that the MS is off the Reverse Traffic Channel */
  byte max_off_time_rev;
              /* Maximum time that the MS is off the Reverse Traffic Channel */
} mcctcho_cand_freq_srch_off_time_type;

/* Parameters that are received from the CFSReqM or CFSCtrlM */
typedef struct
{
  byte last_srch_msg_seq;                     /* CFSRM_SEQ/CFSCM_SEQ/HDM_SEQ */
  byte cf_band_class;                      /* Candidate Frequency Band Class */
  word cf_cdma_freq;                       /* Candidate Frequency assignment */
  byte tf_wait_time;             /* CDMA Candidate Frequence Total Wait Time */
  word num_pilots;                  /* Number of pilots in the CF Search Set */
  byte cur_srch_type;                                 /* Current Search Type */
  byte cur_srch_seq_num;                   /* Current Search Sequence Number */
  byte srch_period;            /* Time between successive searches on the CF */
  mcctcho_cand_freq_srch_off_time_type off_times;               /* Off times */
  #ifdef FEATURE_IS2000
  boolean cf_srch_offset_incl;   /* CF neighbor pilot search window offset
                                    included indicator. */
  #endif /* FEATURE_IS2000 */
} mcctcho_cand_freq_srch_parm_type;
#endif /* FEATURE_IS95B_MAHHO */

typedef struct
{
#ifdef FEATURE_IS2000
  boolean explicit_at;    //IS2000 changes
  qword abs_action_time;  //IS2000 changes
  byte trans_num;         //IS2000 changes
#endif /* FEATURE_IS2000 */
  byte frame_offset;

  boolean disjoint_active_sets_ho;
  boolean different_frame_offset_ho;
  boolean different_freq_ho;
#if defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900)
  boolean different_band_ho;
#endif
  boolean hard_handoff_in_progress;
    /* Indicator of whether a Hard Handoff is in progress */
#if (defined(FEATURE_IS95B_MAHHO) && defined (FEATURE_GPS_MODE))
#error code not present
#endif /* FEATURE_IS95B_MAHHO && FEATURE_GPS_MODE */

#if defined FEATURE_IS95B || defined FEATURE_SPECIAL_MDR
  boolean service_included;
#endif

#ifdef FEATURE_IS95B_MAHHO
  byte tf_band_class;
  word tf_cdma_freq;
  word tf_tot_rx_pwr;
  mcctcho_cf_srch_result_type cf_srch_results;
  boolean cf_srch_completed;
  boolean hard_handoff_successful;
    /* Indicator of whether a Hard Handoff is successful */
  boolean restart_periodic_srch;
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_IS2000
  boolean fpc_subchan_gain_incl;      // FPC subchan gain included indicator
  boolean process_subchan_gain_at_ho; // Indicates if subchan gain is to be processed at action time of handoff
  qword subchan_gain_action_time;     // Absolute action time of subchan_gain if used
  boolean update_dec_sup;             // Do we need to send sups info to RXC
  boolean num_sups;                   // Num of SCCH to be informed to RXC
  boolean high_rc_before_ho;         // This indicates if were in a high RC
                                       // before HO
  boolean high_rc_after_ho;         // This indicates if we'll be in a
  /* higher RC after the HO, needed to  populate action packets at AT */
#endif
  caii_ho_msg_type  ho_msg;       // It's much easier to put the whole message here than
                                  // to put piece of it, both for understanding and maintenance.
} mcctcho_ho_data_type;

/* The following struct definition is to avoid using mccrxtx_msg_type to
    store pending messages, as that struct takes too much memory */
typedef struct
{
  union
  {
    caii_gen_type gen;
#ifdef FEATURE_IS95B_MAHHO
    caii_cfs_req_type cfs_req;
    caii_cfs_ctrl_type cfs_ctrl;
#endif /* FEATURE_IS95B_MAHHO */
  }msg;
} mcctcho_pending_msg_type;



/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_TC_INIT

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_tc_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_TC_EXIT

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_tc_exit( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_GENERIC_HANDOFF_MSG

DESCRIPTION
  This function processes a Generic Handoff Direction Message
  (HDM, EHDM, GHDM, ...).

DEPENDENCIES
  See dependencies of called functions.

RETURN VALUE
  None.

SIDE EFFECTS
  It may cause the mobile to do a soft or hard handoff.  It may update the
  search parameters.  It may update the code channel list (both fundamental
  and supplemental channels) used by the searcher.  It may modify the forward
  supplemental channel duration timer.  It may change the CDMA frequency.  It
  may queue a reject message.  It may reset layer 2 and the forward power
  control parameters.  It may change a whole host of base station related
  parameters in the event of a hard handoff.

===========================================================================*/
extern void mcctcho_process_generic_handoff_msg
(
  mccrxtx_msg_type *msg_ptr
    /* Pointer to message received from the base station */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_POST_PROCESS_HANDOFF

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_post_process_handoff( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_FM_HO_MSG

DESCRIPTION
  This function processes an Analog Handoff Direction Message.  It fills
  the handoff information buffer which is shared between the FM and CDMA
  sub-systems with the received handoff information if the mobile is
  configured for analog operation.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
extern word mcctcho_process_fm_ho_msg
(
  caii_fm_ho_type *msg_ptr,
    /* Pointer to received Analog Handoff Direction Message */
  word curr_state
    /* Traffic Channel substate currently being processed */
);

#ifdef TMOBILE
#error code not present
#endif    /* ifdef TMOBILE */

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_TXC_FRAME_SIG

DESCRIPTION
  This function does processing associated with the TXC_FRAME_SIG, which is
  received by MC to signal the handoff action time.  All it does is kick
  off the processing of the message that it previously stored, or return
  immediately if none was stored.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void mcctcho_process_txc_frame_sig( void );

/*===========================================================================

FUNCTION DO_SUCCESSFUL_HANDOFF_OPERATIONS

DESCRIPTION
  This function performs all the operations associated with a successful
  handoff

DEPENDENCIES
  For a soft handoff, this function is always called.  For a HHO, this
  function is called only if the handoff is successful

RETURN VALUE
  None

SIDE EFFECTS
  This function may update the service negotiation type, packet zone ID,
  p_rev_in_use, and power control step.  It will also send a handoff
  completion message

===========================================================================*/

extern void do_successful_handoff_operations( void );


#ifdef FEATURE_IS95B_MAHHO
/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_FAILED_HHO

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_process_failed_hho
(
  mcctcho_hho_failure_reason_type hho_failure_reason
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_RETURN_TO_SERV_FREQ

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_return_to_serv_freq
(
  mccsrch_rpt_type *rpt_ptr
    /* Pointer to the failure report received from SRCH */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_START_CF_MEAS

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_start_cf_meas
(
  mccsrch_rpt_type *rpt_ptr
    /* Pointer to the failure report received from SRCH */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_CAND_FREQ_SRCH_REQ_MSG

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void mcctcho_process_cand_freq_srch_req_msg
(
  caii_cfs_req_type *cfs_req
    /* Pointer to message received from the base station */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_CAND_FREQ_SRCH_CTRL_MSG

DESCRIPTION
  This function processes a Candidate Frequency Search Control Message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void mcctcho_process_cand_freq_srch_ctrl_msg
(
  caii_cfs_ctrl_type *cfs_ctrl
    /* Pointer to message received from the base station */
);


#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION MCCTCHO_UPDATE_CFS_PARAMS

DESCRIPTION
  This function updates CFS parameters locally stored in MC based on the
  CFS Req or the CFS Ctrl message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern void mcctcho_update_cfs_params
(
  const mctrans_pending_action_msg_type *cfs_msg_ptr
);
#endif

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTCHO_PROCESS_SRCH_CF_NSET_RPT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void mcctcho_process_srch_cf_nset_rpt
(
  mccsrch_rpt_type *rpt_ptr
    /* Pointer to the CF_NSET_R report received from SRCH */
);
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION MCCTCHO_GET_HO_DATA

DESCRIPTION
  This function returns a pointer to the mcctcho_ho_data data structure.

DEPENDENCIES
  None.

RETURN VALUE
  Constant pointer to the mcctcho_ho_data.

SIDE EFFECTS
  None.

===========================================================================*/
mcctcho_ho_data_type *mcctcho_get_ho_data
(
  void
);

/*===========================================================================

FUNCTION mcctcho_generate_aset

DESCRIPTION
  This function updates code channel list table and returns aset in
  the parameters passed as references

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  Code Channel list table in mccccl.c could get modified.

===========================================================================*/

mccccl_sup_state_type mcctcho_generate_aset(
  word * aset_count,
  srch_tc_aset_type * aset
    /* Pointer to active set parameters */
);
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

#endif /* MCCTCHO_H */

