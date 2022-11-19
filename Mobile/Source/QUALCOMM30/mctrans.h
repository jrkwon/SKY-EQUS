#ifndef MCTRANS_H
#define MCTRANS_H

/*===========================================================================

          MAIN CONTROL TRANSACTION MANAGER MANAGER

GENERAL DESCRIPTION
  This task manages the transaction (Commands with action times) sent to the lower
  layers.  The lower layers always "confirm" a transaction with the functions
  in this module before going ahead with it. This module also makes sure if one task
  is given an "okay" for a transaction, that is true for all other tasks which hold
  the same transaction. A "confirm" can return "FALSE" if this transaction is either
  no longer valid or has been overidden by a different transaction.

DESCRIPTION
   This module has data structures to keep track of different transaction numbers
   and the transaction types they correspond to.

   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mctrans.h_v   1.3.1.3   17 Jul 2001 11:53:24   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/01   lcc     Added r_fch_rc to mctrans_rlgc_action_type.
05/10/01   jq      Added mctrans_is_trans_pending ().
05/02/01   fc      Added suport for T_ADD_ABORT.
05/03/01   nxb(va) Added pilot rec type in the CF NSET interface.
02/15/01   va      Added prototype for mctrans_is_okay_to_tune_away for GPS ONE
                   support.
02/12/01   lcc     Added member trans_time to mctrans_info_type.
08/23/00   lcc     Removed some obsolete comments.
07/28/00   fc      Mainline FEATURE_CS_ACTION_TIME_TRANS.
07/21/00   fc      Added trans_type to mctrans_rpt_type.
                   Used enumerated types for FPC mode, SCH rate and coding type.
07/17/00   va      Added the CFS transaction types and new actions
                   to go with it.
06/22/00   bgc     Moved mctrans_cmd_type to its own header from mctrans.h
                   to simplify a circular include problem.
06/19/00   lcc     1. Changed byte to uint8 to be consistent.
                   2. Made SCH init_setpt optional in mctrans_ffpc_action_type.
06/15/00   lcc     Changed HO action interface to TXC such that preamble unit
                   is specified.  Also allows nom_pwr to be optional since HDM does
                   not include that field.
06/12/00   lcc     1. Update interfaces to FFPC and RLGC.
                   2. Rearranged some type definitions to circumvent a problem with
                      circular dependency between srch.h and mctrans.h.
                   3. Added function prototype for mctrans_void_trans.
06/02/00   lcc     1. Corrected some comments and some structure definition (not used
                      yet).
                   2. Obsoleted cai_msg in mctrans_info_type which is not needed.
                   3. Removed HO message as a member of
05/16/00   lcc     Added data structures to handle Ho and SCR transactions.
                   Changed data member "code_type" of SCH action to "turbo_code"
                   to better reflect it's meaning.
04/24/00   lcc     Added functions prototypes, data structures and types to support
                   client-server transaction engine.
12/20/99   va      Added new interface for getting "transaction type" from a
                   transaction num
11/15/99   va      Initial revision of the file.
                   Created for the IS2000 Changes. This file implements the
                   MC transaction module.
===========================================================================*/

#include "comdef.h"
#include "cai.h"
#include "caii.h"
#include "cmd.h"
#include "queue.h"
#include "mctrans_cmd.h"
#include "srch.h"

/*===========================================================================
                        TYPE DEFINITIONS
===========================================================================*/
/* Enumeration for the different types of transactions */
typedef enum
{
  MCTRANS_HO=0,
  MCTRANS_SCR,
  MCTRANS_FPC_SUBCHAN_GAIN,
  MCTRANS_PWR_CTRL_MSG,
  MCTRANS_F_SCH0_START,     // SCH1 not support in MSM5000, no need to include here
  MCTRANS_F_SCH0_STOP,
  MCTRANS_R_SCH0_START,
  MCTRANS_R_SCH0_STOP,
  MCTRANS_CFS_NSET,
  MCTRANS_CFS_CTRL,
  MCTRANS_COUNT
} mctrans_transaction_type;

typedef enum
{
  MCTRANS_FFPC_START,       // Start outer loop, may come with parameters
  MCTRANS_FFPC_STOP,        // Stop outer loop, may come with parameters
  MCTRANS_FFPC_CONTINUE     // Continue with current FFPC operation
} mctrans_ffpc_operation_type;

typedef enum
{
  MCTRANS_RLGC_START,       // Start RLGC
  MCTRANS_RLGC_STOP,        // Stop RLGC
  MCTRANS_RLGC_CONTINUE     // Continue with current RLGC operation
} mctrans_rlgc_operation_type;

typedef enum
{
  MCTRANS_PREAMBLE_FRAME,   // Spceifies preamble units in frames
  MCTRANS_PREAMBLE_PCG      // Spceifies preamble units in PCGs
} mctrans_preamble_unit_type;

#ifdef FEATURE_IS2000_R_SCH
// Temporary definition.  To be updated
// Action information for r_sch action
typedef struct
{
  uint8   sch_id;
  boolean start;            // Indicate starting or stopping SCH.  If stop action, following field are not used
  cai_radio_config_type rc;
  cai_sch_rate_type     sch_rate;
  boolean double_size_pdu;  // Indicates if MUX PDU (Type 3) is single or double sized
  cai_sch_coding_type turbo_code;
  uint8   walsh_id;
  uint8   rev_sch_dtx_duration;
} mctrans_txc_r_sch_action_type;

typedef struct
{
  boolean use_t_add_abort; 
    /* Used for SRCH R-SCH action, indicates whether T_ADD_ABORT is enabled */ 
  uint8   t_mulchan;
    /* SCRM pilot strength reporting offset */
} mctrans_srch_r_sch_action_type;
#endif

// Action information for rlgc action
typedef struct
{
  cai_radio_config_type r_fch_rc;
  struct
  {
    mctrans_rlgc_operation_type mode; // Indicates what to do with RLGC
    cai_sch_coding_type sch_code_type;
      // 0 - conv., 1 - turbo, included only when starting
    cai_sch_rate_type sch_rate;        // Included only when starting
  } sch_rlgc_op;
  boolean use_default_rlag;
  struct
  {
    boolean included;
    uint8   value;
  } pwr_cntl_step;
  struct
  {
    boolean included;
    int8    value;
  } rlgain_traffic_pilot;
  struct
  {
    boolean included;
    uint8   value;
  } rlgain_sch_pilot[CAI_MAX_NUM_REV_SCH];
  uint8         num_of_rpc_records;                 // Number of RPC records
  caii_rpc_type rpc_records[CAI_MAX_NUM_RPC_RECS];  // RPC records
} mctrans_rlgc_action_type;

typedef struct
{
  dword  search_period;
    /* Time between successive searches on the CF, in millisecond unit */

  byte  band_class;
    /* Band class of the CF */

  word  cdma_freq;
    /* CDMA frequency for the CF */

  int1  diff_rx_pwr_thresh;
    /* Minimum difference in received power */

  byte  min_total_pilot_ec_io;
    /* Minimum total pilot Ec/Io */

  byte  sf_total_ec_thresh;
    /* Ec threshold for the Serving Frequency */

  byte  sf_total_ecio_thresh;
    /* Ec/Io threshold for the Serving frequency */

  byte  cf_t_add;
    /* Pilot detection threshold for th CF */

  byte  pilot_inc;
    /* Pilot Inc to be used on th CF after handoff */

  byte  cf_srch_win_n;
    /* Default search window size for the CF Search Set */

  byte  cf_srch_win_r;
    /* Search window size for the Remaining Set on the CF */

  boolean pilot_update;
    /* Indicates if a CF set update is included, the following fields make
       sense only if this is set to TRUE */

  word  cf_nset_cnt;
    /* Number of pilots to be searched */

  byte  cf_nghbr_srch_mode;
    /* Search mode for the CF Search Set */

  struct
  {
    int2 pilot;
      /* Pilot PN offset */

    byte srch_flag;
      /* Flag to incidate if the corresponding pilot is to be searched */

    byte srch_pri;
      /* Search priority */

    byte srch_win;
      /* Search window size */

    #ifdef FEATURE_IS2000

    byte srch_offset_nghbr;
      /* Search Window offset */

    caii_add_pilot_rec_type pilot_rec;
    /* Pilot rec type specific fields*/

    #endif /*FEATURE_IS2000 */

  }cf_nset[ CAI_CFSREQ_REC_MAX ];
} mctrans_cfs_nset_action_type;


// Action information for ffpc action
typedef struct
{
  struct
  {
    mctrans_ffpc_operation_type mode; // Indicates what to do with primary channel FFPC
    cai_radio_config_type       rc;   // Indicates RC to use, used for starting and continuing only
  } pri_ffpc_op;
  struct
  {
    mctrans_ffpc_operation_type mode; // Indicates what to do with SCH FFPC
    cai_radio_config_type       rc;   // Indicates RC to use, used for starting and continuing only
  } sch_ffpc_op;
  struct
  {
    boolean           included;
    cai_fpc_mode_type value;
  } fpc_mode;
  struct
  {
    boolean   included;
    uint8     value;
  } fpc_subchan_gain;
  struct
  {
    boolean included;
    uint8   value;                  // 0-fch, 1-DCCH
  } fpc_pri_chan;
  struct
  {
    boolean   included;
    uint8     min_setpt;
    uint8     max_setpt;
    uint8     target_fer;
  } fch_olpc;
  struct
  {
    boolean   included;
    uint8     min_setpt;
    uint8     max_setpt;
    uint8     target_fer;
  } dcch_olpc;
  struct    // Threshold for FCH-DCCH
  {
    boolean included;
    uint8   value;
  } fpc_setpt_thresh;
  struct
  {
    boolean   included;               // Indicates if any of the fields below is included
    boolean   fpc_sch_init_setpt_op;  // 0 = absolute, 1 = relative to fpc_pri_chan
    struct
    {
      boolean included;
      uint8   value;
    } fpc_sec_chan;
    struct
    {
      boolean   included;
      uint8     value;
    } fpc_thresh_sch;
    struct
    {
      boolean   included;
      struct
      {
        boolean included;
        uint8    value;
      } init_setpt;
      uint8     min_setpt;
      uint8     max_setpt;
      uint8     target_fer;
    } sch_olpc[CAI_MAX_NUM_FOR_SCH];
  } sch;
} mctrans_ffpc_action_type;

// Action information for f_sch action to RXC
typedef struct
{
  uint8             sch_id;
  boolean           start;            // Indicate starting or stopping SCH
  boolean           for_sch_fer_rep;  // Indicates if FER report is to be generated at end of burst
  // Note: following fields are not used if stopping
  cai_radio_config_type rc;
  cai_sch_rate_type sch_rate;         // 1X, 2X, 4X, 8X, 16X
  boolean           double_size_pdu;  // Indicates if MUX PDU (Type 3) is single or double sized
  cai_sch_coding_type code_type;      // 0 - convolutional, 1 - turbo
} mctrans_rxc_f_sch_action_type;

// Action information for f_sch action to SRCH
typedef struct
{
  // For each member of the new SCH
  struct
  {
    uint8 num_pilots;
    // 0 if SCH not active

    // Pilot offset indices and walsh channels for active set members
    struct
    {
      uint16  pilot_pn;
      // Pilot PN index of this Active Set member

      uint16  walsh_chn;
      // Walsh channel to be used on the traffic channel with the pilot

      uint8   qof_mask_id;
      // QOF mask ID (0-3), where 0 gives true walsh fns)
    } pilots[CAI_N6M];
  } sch[CAI_MAX_NUM_FOR_SCH];  // maximum of 2 SCH in IS95C. TIGER2 supports only 1
} mctrans_srch_f_sch_action_type;

// Action information for HO action to RXC
typedef struct
{
  boolean hho_ind;        // Indicates if this is a HHO (To wait for sync points)
  boolean rif_ind;        // Indicates if this is Return on Failure HHO
  boolean reset_fpc;      // Indicates if FPC is to be reset after HO
  struct
  {
    boolean included;
    uint8   value;
  } rev_pwr_cntl_delay;    // Rev power control delay value
} mctrans_rxc_ho_action_type;

// Action information for HO action to TXC
typedef struct
{
  boolean hho_ind;      // HHO Indication
  boolean rif_ind;      // Return on Failure Indication

  uint8   frame_offset;  // Frame offset, change only if different
  struct
  {
    boolean included;
    uint8   value;
  } nom_pwr;            // nom_pwr is not included in HDM
  struct
  {
    mctrans_preamble_unit_type  unit;
    uint8                       value;
  } num_preamble;
  struct
  {
    boolean included;
    qword   value;
  } lcm;              // Long code mask
} mctrans_txc_ho_action_type;

// Action information for SCR action to TRC
typedef struct
{
  struct
  {
    boolean included;           // Indicates if primary SO is included
    uint16  value;              // Primary SO to switch to, if included
  } pri_so;

  struct
  {
    boolean included;           // Indicates if secondary SO is included
    uint16  value;              // Secondary SO to switch to, if included
  } sec_so;
  struct
  {
    boolean included;           // Indicates if FCH RC is included
    cai_radio_config_type value;
  } fch_rc;
  //Rev pwr cntl delay value, This can change
  // if the RC class changes.
  struct
  {
    boolean included;           // Indicates if rev pwr cntl delay is included
    uint8 value;//Rev pwr cntl delay value
  } rev_pwr_cntl_delay;

} mctrans_rxc_scr_action_type;

typedef struct {
  struct
  {
    boolean included;           // Indicates if primary SO is included
    uint16  value;              // Primary SO to switch to, if included
    boolean connect_so;         // Indicates if this SO can be connected with the vocoder
                                // 1 if we are in CONV state already; 0 otherwise
  } pri_so;

  // Secondary SO not used now.
  struct
  {
    boolean included;           // Indicates if secondary SO is included
    uint16  value;              // Secondary SO to switch to, if included
    boolean connect_so;         // Indicates if this SO can be connected with the vocoder
                                // 1 if we are in CONV state already; 0 otherwise
  } sec_so;

  struct
  {
    boolean included;           // Indicates if FCH RC is included
    cai_radio_config_type value;
  } fch_rc;
} mctrans_txc_scr_action_type;

typedef struct {
  boolean rev_fch_gating_on; /* Indicates if 8th rate gating should be on */
} mctrans_txc_gating_action_type;

typedef union
{
  caii_gen_type           gen;
  caii_pwr_ctrl_type      pwr_ctrl;
#ifdef FEATURE_IS2000_SCH
  caii_escam_type         escam;
#endif // FEATURE_IS2000_SCH
  caii_cfs_req_type cfs_req;
  caii_cfs_ctrl_type cfs_ctrl;
} mctrans_pending_action_msg_type;

// Some data structures for the transaction report queue.
// Transaction report is sent when a transaction is completed.
#ifdef FEATURE_IS2000
typedef struct
{
  cmd_hdr_type             rpt_hdr;      // report header - includes q_link
  uint8                    trans_num;
  mctrans_transaction_type trans_type;
} mctrans_rpt_type;
#endif
extern q_type  mctrans_rpt_q;

// This header file inclusion has to be placed here to resolve a circular
// dependency problem between srch.h and mctrans.h.
struct mctrans_action_packet_struct;

// Action information for HO action to SRCH
typedef struct
{
  boolean hho_ind;        // Indicates if this is a HHO
  boolean rif_ind;        // Return-on-failure indicator

  struct
  {
    boolean included;
    qword   value;
  } lcm;                  // Long code mask

  struct
  {
    boolean included;     // Indicates if following parameters are included.
    caii_ho_srch_type values;
  } srch_parms;
  srch_tc_aset_info_type tc_aset; // Note: this aset information is used for SHO only
} mctrans_srch_ho_action_type;

// Structure to hold any information related to an action
typedef struct mctrans_action_packet_struct
{
  mctrans_action_type action_type;
  union
  {
    mctrans_ffpc_action_type        ffpc;
    mctrans_rlgc_action_type        rlgc;
    mctrans_txc_ho_action_type      txc_ho;
    mctrans_rxc_ho_action_type      rxc_ho;
    mctrans_srch_ho_action_type     srch_ho;
    mctrans_rxc_scr_action_type     rxc_scr;
    mctrans_txc_scr_action_type     txc_scr;
    #ifdef FEATURE_IS2000_F_SCH
    mctrans_rxc_f_sch_action_type   rxc_f_sch;
    mctrans_srch_f_sch_action_type  srch_f_sch;
    #endif
    #ifdef FEATURE_IS2000_R_SCH
    mctrans_txc_r_sch_action_type   txc_r_sch;
    mctrans_srch_r_sch_action_type  srch_r_sch;
    #endif
    mctrans_cfs_nset_action_type    srch_cfs_nset;
    mctrans_txc_gating_action_type  txc_gating;
  } action;
} mctrans_action_packet_type;

/* Data structure that holds the state of each active transaction */
typedef struct
{
  boolean is_valid;         // Whether transaction entry is valid
  boolean is_commited;      // Whether transaction is already committed
  mctrans_transaction_type  transaction_type;
  qword   trans_time;       // Time transaction to be executed
  // Outstanding actions for each transaction server
  uint16  outstanding_actions[MCTRANS_SERVER_COUNT];
  void (*preprocessor)(uint8 trans_num);
  void (*postprocessor)(uint8 trans_num);
  void (*action_packet_gen)(uint8 trans_num, mctrans_action_type,
                            mctrans_server_token_type, mctrans_action_packet_type*);
} mctrans_info_type;

// This structure is used by transaction client (MC) to prepare transaction
// to be despatched.  The intent is that all static information related to
// a transaction is stored here which can be initialized with static initializer.
typedef struct
{
  mctrans_transaction_type  transaction_type;
  struct
  {
    mctrans_server_token_type server_token;
    uint16                    server_cmd_code;
    uint8                     num_actions;
    mctrans_action_type       actions[MCTRANS_MAX_ACTION_IN_TRANS];
  } server[MCTRANS_SERVER_COUNT];
  void (*preprocessor)(uint8 trans_num);
  void (*postprocessor)(uint8 trans_num);
  void (*action_packet_gen)(uint8 trans_num, mctrans_action_type,
                            mctrans_server_token_type, mctrans_action_packet_type* );
} mctrans_despatch_info_type;

/*===========================================================================

FUNCTION mctrans_initialize

DESCRIPTION
  Performs necessary initialization for the transaction engine.  This function
  is called when first get onto traffic channel.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
void mctrans_initialize ( void );

/*===========================================================================

FUNCTION mctrans_get_new_transaction

DESCRIPTION
  This function generates a new transaction number and updates the
  latest "transaction number" for a given transaction type.

DEPENDENCIES
  None

RETURN VALUE
  New transaction number

SIDE EFFECTS
  Could cause older transactions of the same type to fail verification when "verify"
  function is called by the lower layers
===========================================================================*/

extern uint8 mctrans_get_new_transaction( mctrans_transaction_type trans_type);

/*===========================================================================

FUNCTION mctrans_confirm_transaction

DESCRIPTION
  This function gives the "go ahead" to the lower layers to go ahead and complete
  a transaction. A transaction is not confirmed if anotehr similar transaction type has
  overridden this transaction (provided this din't happen before some other task confirmed
  it). This function guarantees that if a "go ahead" is given to a task for a transaction,
  that will be the case for all tasks for the same transaction

DEPENDENCIES
  None

RETURN VALUE
  TRUE if transaction is still valid, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/

extern boolean mctrans_confirm_transaction( uint8 transaction_num );

/*===========================================================================

FUNCTION mctrans_get_transaction_type

DESCRIPTION
  This function returns the transaction type for a given transaction
  number.

DEPENDENCIES
  None

RETURN VALUE
  Transaction type for this trans num

SIDE EFFECTS
===========================================================================*/

extern mctrans_transaction_type mctrans_get_transaction_type( uint8 transaction_num );

/*===========================================================================

FUNCTION mctransr_reg_processors

DESCRIPTION
  This function registers pre- and post-processors for a transaction.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
===========================================================================*/
void mctrans_reg_processors
(
  uint8 transaction_num,
  void (*preprocessor)(uint8 trans_num),
  void (*postprocessor)(uint8 trans_num),
  void (*action_packet_gen)(uint8 trans_num, mctrans_action_type,
                            mctrans_server_token_type, mctrans_action_packet_type*)
);

/*===========================================================================

FUNCTION mctrans_despatch_transaction

DESCRIPTION
  This function despatches a transaction based on information in "despatch_info".
  Proper commands to corresponding servers will be issued.  Callers has the option
  of passing in a message (IS2000 message) to be stored internally which can be used
  later when processing the actions.

DEPENDENCIES
  None.

RETURN VALUE
  Transaction number that was despatched, MCTRANS_MAX_TRANSACTIONS if
  not successful.

SIDE EFFECTS
===========================================================================*/
uint8 mctrans_despatch_transaction
(
  qword   trans_time,
  const mctrans_despatch_info_type* despatch_info,
  mctrans_pending_action_msg_type* msg
);

/*===========================================================================

FUNCTION mctrans_process_action

DESCRIPTION
  This function is called by a transaction server to process an action in a
  transaction.  It is called each time an action in a transaction is to be
  processed.  This function runs in the server's context and must be brief
  (except, of course, what's in action_processor, which is controlled by the
  transaction server.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if transaction is valid and action executed.  False if transaction is
  expired.

SIDE EFFECTS
===========================================================================*/

boolean mctrans_process_action
(
  mctrans_action_type action,
  uint8               trans_num,
  mctrans_server_token_type token,
  void (*action_processor)(mctrans_action_packet_type* action_packet)
);

/*===========================================================================

FUNCTION mctrans_record_pending_msg

DESCRIPTION
  Saves the pending message in the internal pending message buffer.

DEPENDENCIES
  None

RETURN VALUE
  True if successful.  FALSE if internal buffer is full.

SIDE EFFECTS
===========================================================================*/
boolean mctrans_record_pending_msg
(
  mctrans_pending_action_msg_type* action_msg,
  uint8 trans_num
);

/*===========================================================================

FUNCTION mctrans_get_pending_msg

DESCRIPTION
  Retrive the pending message for specified transaction number.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to pending message.  NULL if no message for transaction.

SIDE EFFECTS
===========================================================================*/
mctrans_pending_action_msg_type* mctrans_get_pending_msg
(
  uint8 trans_num
);

/*===========================================================================

FUNCTION mctrans_get_pending_msg

DESCRIPTION
  Release the pending message for the specified transaction number.  The message
  entry will be free to be used by other transactions.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void mctrans_release_pending_msg
(
  uint8 trans_num
);

/*===========================================================================

FUNCTION mctrans_report_trans_done

DESCRIPTION
  This function sends a MCC_TRANS_DONE_SIG to notify MC that a transaction
  has just been completed.  This is used only for transactions that requires
  MC to perform some further processing after the transaction completes.  If
  no such processing is required, this function need not be called.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void mctrans_report_trans_done
(
  uint8 trans_num
);

/*===========================================================================

FUNCTION mctrans_void_trans

DESCRIPTION
  This function invalidates the most recent transaction specified by trans_type
  if the transaction has not been committed.  Note that if multiple transactions
  of the same type are pending, this function can only be used to cancel the latest
  one.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void mctrans_void_trans
(
  mctrans_transaction_type trans_type
);

/*===========================================================================

FUNCTION mctrans_is_trans_pending

DESCRIPTION
  This function checks whether there is a transaction pending.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE is assignment is pending

SIDE EFFECTS
  None.
===========================================================================*/
boolean mctrans_is_trans_pending ( mctrans_transaction_type trans ); 

#ifdef FEATURE_GPSONE
#error code not present
#endif

#endif /* MCTRANS_H */


