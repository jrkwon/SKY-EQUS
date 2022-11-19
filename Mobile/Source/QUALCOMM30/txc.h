#ifndef TXC_H
#define TXC_H
/*===========================================================================

          T R A N S M I T   T A S K   D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the transmit task that don't fit
  into one of the other H files such as CAI.H

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/txc.h_v   1.1   01 Dec 2000 15:47:08   akhare  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/01/00   ak      Added function extern for txc_inform_sch_chng.
10/04/00   jq      Added FTM FCH and SCH loopback feature
09/06/00   bgc     Added TXC_FTM_SCH_F to setup SCH channels separate of 
                   FCH for FEATURE_FACTORY_TESTMODE.
08/14/00   sr      Added support for pilot gating and FCH 1/8 rate gating in
                   PLT
07/17/00   va      Added support for 8th rate gating interface.
05/30/00   hrk     Changed prototype for txc_set_pch_fch_gain().
05/11/00   hrk     Added prototype for txc_set_fpc_mode().
04/24/00   lcc     Added TXC_MC_TRANS_F and related command structure.
04/11/00   na      Added prototype for txc_get_sch_info() under
                   FEATURE_DS_IS2000.
02/14/00   hrk     Added prototype for function txc_set_pch_fch_gain ()
02/10/00   hrk     Added prototype for function txc_get_fch_rate_set ()
1/31/00    sr      Added action time processing and implementation of
                   txc_ho_cmd, txc_hho_ok_cmd and txc_returned_cmd.
                   Parameters related to reverse link gain adjustment and
                   power control step size haven't yet been implemented
01/07/00   sr      Added num_preambles to txc_tc_cmd_type
12/22/99   hrk     Moved definition of txc_gain_type out of FEATURE_PLT
12/20/99   va      Added pwr_ctrl_step to the txc_pwr_ctrl_cmd_type,
                   txc_ho_cmd_type and txc_returned_cmd_type interfaces.
11/23/99   sr      Added prototype for txc_data_pump_init()
11/19/99   sr      Minimum implementation of new mc-txc interface
                   including support for new TXC_SO_F, TXC_CONNECT_SO_F
                   and TXC_RELEASE_F commands
                   Vanitha's new mc-txc interface
                   Added changes to support data pump and supplemental channels
08/09/99   ry      Changed FEATURE_IS95B_HARD_HO to FEATURE_IS95B_MAHHO
                   Changed FEATURE_IS95B_CFS to FEATURE_IS95B_MAHHO
                   Updated the extern definition for txc_register_action_time()
                   Added extern definition for txc_get_time_in_1_25ms_unit()
07/19/99   abh     Changes for Access Probe Handoff.
07/09/99   ry      Added prototypes for txc_set_frame_offset(),
                   txc_hho_action_time_chk(), txc_flag_next_int() and
                   txc_int_occurred()
06/17/99   kmp     Merged in the one change listed below.
06/07/99   ks      Added txc_suspend_tx function to suspend/enable Access
                   probes when T72M timer expires.
05/10/99   kmp     Merged in the IS-95B changes listed below
           fc      Put TXC_ACC_PROBE_F command under FEATURE_IS95B_PILOT_RPT.
           fc      Added TXC_ACC_PROBE_F command.
08/25/95   gb      Inform TX of the HHO parameters num_preamble and nom_pwr.
08/10/95   gb      Added New Markov Service Options and made all service
                     options CAI_SO_*.
07/13/95   gb      Changed mux option initialization.
06/14/95   gb      Added support for Rate set 2 service options.
03/29/95   jca     Added sigs and tcb_ptr to txc_tc_cmd_type.
12/23/94   jca     Added nom_pwr to TXC_ACCESS_F command parameters.
11/11/94   jca     Deleted TXC_ACC_KILL_F cmd.  Modified TXC_ACC_STOP_F cmd.
11/01/94   gb      Sector Lock test functionality added.
06/21/94   jca     Added TXC_FRAME_OFFSET_F command.
04/20/94   jca     Added Loopback service option.  Deleted support for
                   DIAG data services.
12/22/93   jca     Modified for IS-95 Phase 1 upgrade.
10/06/92   jai     Added support for access channel logging.
07/14/92   jai     First Revision

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "queue.h"
#include "qw.h"
#include "cmd.h"
#include "cai.h"
#include "enc.h"
#include "caii.h"  //IS2000 change
#ifdef FEATURE_IS2000
#include "mctrans.h"
#endif
#ifdef FEATURE_FFPC
#include "ffpc.h"
#endif /* FEATURE_FFPC */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Possible status value for a command */
typedef enum {
  TXC_DONE_S,                      /* Generic OK return */
  TXC_BUSY_S,                      /* Command is being processed */
  TXC_BADCMD_S,                    /* Invalid command */
  TXC_ERR_S,                       /* Error attempting to do something */
  TXC_MAXIMUM_S                    /* always last, this counts statuses */
} txc_status_type;

typedef enum {
  TXC_CDMA_F,                      /* Switch to CDMA mode */
  TXC_PC_F,                        /* Move to Paging Channel */
  TXC_TC_F,                        /* Move to Traffic Channel */
  TXC_EXIT_F,                      /* Exit CDMA operation */
  TXC_MSG_F,                       /* Send a message */
  TXC_ACCESS_F,                    /* Make an access attempt */
  TXC_ACC_STOP_F,                  /* Stop access attempts immediately */
  TXC_OFF_TX_F,                    /* Stop transmitting */
  TXC_ON_TX_F,                     /* Start transmitting */
  TXC_NULLS_F,                     /* Transmit null data on TC */
  TXC_VOC_F,                       /* Wait for voc initialization */
  TXC_SO_F,                        /* Switch service options */
  TXC_RDR_F,                       /* Reverse data rate limit parameters */
  TXC_FRAME_OFFSET_F,              /* Set frame offset */
#if defined (FEATURE_IS95B_PILOT_RPT) || defined(FEATURE_SPECIAL_MDR)
  TXC_ACC_PROBE_F,                 /* Transmit access probe */
#endif
#ifdef FEATURE_IS95B_ACCESS_PROBE_HO
  TXC_ACC_SUSPEND_F,
#endif /* FEATURE_IS95B_ACCESS_PROBE_HO */
#ifdef FEATURE_IS2000
  TXC_PWR_CTRL_F,                 /* Pass along new power ctrl parameters */
  TXC_CONNECT_SO_F,               /* Indicates we are in CONV substate,
                                     Connect stored SO with vocoder if applicable */
  TXC_HO_F,                       /* HO command */
  TXC_HHO_SUCCESS_F,              /* Indicates that a HHO was successful, Can program
                                     necessary parameters */
  TXC_RETURNED_F,                 /* Indicates a return back to SF after a failed HHO,
                                     Reprogram necessary params back to old values */
  TXC_RELEASE_F,                  /* Sent after a release, Equivalent to old command of
                                     sending a NULL SO command */
  TXC_MC_TRANS_F,                 /* Transaction from MC */
#endif

#ifdef FEATURE_PLT
#error code not present
#endif // FEATURE_PLT

#ifdef FEATURE_FACTORY_TESTMODE
  TXC_FTM_SCH_F,                  /* Command to start SCH */
  TXC_FTM_FCH_LOOPBACK,           /* Start FCH loopback */
  TXC_FTM_SCH_LOOPBACK,           /* Start SCH loopback */
#endif

  TXC_COMMAND_COUNT                /* End of the enum */
} txc_cmd_name_type;

/* The common Header for all the commands */
typedef struct {
  cmd_hdr_type         cmd_hdr;        /* command header */
  txc_cmd_name_type    command;        /* The command */
  txc_status_type       status;        /* Space for message status */
} txc_hdr_type;

typedef struct {                  /* TXC_CDMA_F */
  txc_hdr_type   hdr;
} txc_cdma_cmd_type;

typedef struct {                  /* TXC_PC_F */
  txc_hdr_type   hdr;
} txc_pc_cmd_type;

typedef struct {                  /* TXC_TC_F */
  txc_hdr_type   hdr;
  word           sigs;            /* Signal to set when TX task has acquired
                                     timing and moved to Traffic state */
  rex_tcb_type  *tcb_ptr;         /* Pointer to task to signal */
  qword          mask;            /* Long mask */
  #ifndef FEATURE_IS2000
  enc_rate_set_type rate_set;     /* initial rate set to use */
  #else
  boolean fch_incl;               /* Is FCH allocated */
  cai_radio_config_type rev_fch_rc; /* Radio Config of rev FCH, valid only if
                                       fch_incl is TRUE */
  byte rl_gain_adj;      /*In 8-bit 2's compliment, units of 1db*/
  byte rlgain_traffic_pilot;      /*In 8-bit 2's compliment*/
  boolean rev_fch_gating_on; /* Indicates if 8th rate gating is on 
                                     or off on R_FCH */
  #endif

#ifdef FEATURE_FACTORY_TESTMODE
  enc_rate_type     fch_rate;
  boolean           sch_on;
  boolean           sch_turbo;
  word              sch_ltu_size;
  enc_sch_rate_type sch_rate;
  word              num_preambles;
  dword             tx_pattern;
#endif // FEATURE_FACTORY_TESTMODE

} txc_tc_cmd_type;

#ifdef FEATURE_FACTORY_TESTMODE
typedef struct {
  txc_hdr_type      hdr;
  cai_radio_config_type radio_cfg;
  boolean           sch_on;
  enc_sch_rate_type sch_rate;
  boolean           sch_turbo;
  dword             tx_pattern;
} txc_ftm_sch_cmd_type;

typedef struct {
  txc_hdr_type      hdr;
  boolean           loopback_on;
} txc_ftm_loopback_cmd_type;

#endif // FEATURE_FACTORY_TESTMODE

/* Signaling mode for txc_tc_frame_type */
typedef enum
{
  NORM  = 0,                         /* normal mode */
  DIM   = 1,                         /* dim and burst frame */
  BLANK = 2                          /* blank and burst frame */
} txc_sig_frame_type;

typedef struct
{
  txc_sig_frame_type sig;        /* Signaling mode */
  byte               data [ENC_14400_FULL_RATE_SIZE];
} txc_tc_frame_type;

typedef struct
{
  byte               data [ENC_SCH_16X_SIZE];
  word               size; /* for FACTORY_TESTMODE, the amount of data in */
                           /* the data buffer */
} txc_tc_supp_frame_type;                              

typedef struct {                  /* TXC_EXIT_F */
  txc_hdr_type   hdr;
} txc_exit_cmd_type;

typedef struct {                  /* TXC_MSG_F */
  txc_hdr_type   hdr;
  word          len;              /* number of bits in message */
  byte          *msg_ptr;         /* the message to send */
} txc_msg_cmd_type;

typedef struct {                  /* TXC_ACCESS_F */
  txc_hdr_type  hdr;
  word          len;              /* Number of bits in message */
  boolean       req;              /* True if access request */
  byte          seq_num;          /* Max number of access probe sequences */
  byte          pagech;           /* Current Paging Channel */
  word          base_id;          /* Base station identification */
  word          pilot_pn;         /* Pilot PN sequence offset index */
  byte          pam_sz;           /* Access Channel preamble length */
  byte          max_cap_sz;       /* Max Access Channel msg capsule size */
  byte          bkoff;            /* Probe sequence backoff range */
  byte          probe_bkoff;      /* Probe backoff range */
  byte          acc_tmo;          /* Acknowledgement timeout */
  byte          init_pwr;         /* Initial power offset for access */
  byte          nom_pwr;          /* Nominal transmit power offset */
  byte          pwr_step;         /* Power increment */
  byte          num_step;         /* Number of access probes */
  byte          acc_chan;         /* Number of access channels */
  boolean       use_chan;         /* Test feature to use specific acc chan */
  word          rn;               /* Random number of chips backoff - ignored now */
  dword         p;                /* Persistence probability */
  byte          *msg_ptr;         /* Access Channel message to send */
  word          sigs;             /* Signal to set when access attempt is
                                     done */
  rex_tcb_type  *tcb_ptr;         /* Pointer to task to signal when access
                                     attempt is done */
} txc_acc_cmd_type;

typedef struct {                  /* TXC_ACC_STOP_F  */
  txc_hdr_type  hdr;
  word          sigs;             /* Signal to set when access attempt is
                                     done */
  rex_tcb_type  *tcb_ptr;         /* Pointer to task to signal when access
                                     attempt is done */
} txc_stop_cmd_type;

typedef struct {                  /* TXC_ON_TX_F */
  txc_hdr_type   hdr;
} txc_on_cmd_type;

typedef struct {                  /* TXC_OFF_TX_F */
  txc_hdr_type   hdr;
} txc_off_cmd_type;

typedef struct {                  /* TXC_NULLS_F */
  txc_hdr_type   hdr;
} txc_nulls_cmd_type;

typedef struct {                  /* TXC_VOC_F */
  txc_hdr_type   hdr;
} txc_voc_cmd_type;

typedef struct {
   boolean change_pri;
   word pri;
   boolean change_sec;
   word sec;
   boolean connect_so;             /* Indicates if this SO can be connected with the vocoder
                                      1 if we are in CONV state already; 0 otherwise */
   boolean fch_incl;
   cai_radio_config_type rev_fch_rc;
   boolean fch_5ms_frame_allowed;
} txc_scr_info_type;

typedef struct {                  /* TXC_SO_F */
  txc_hdr_type   hdr;
  #ifndef FEATURE_IS2000
  boolean  change_pri;            /* indicator of whether change is required
                                     in primary service option */
  word           pri;             /* primary service option */
  boolean change_sec;             /* indicator of whether change is required
                                     in secondary service option */
  word           sec;             /* secondary service option */
  dword  action_time;             /* frame time at which service option switch
                                     will take place */
  #else
  qword action_time;              /* Action time when the new params take effect */
  txc_scr_info_type scr_info;     /* The new service config info */
  byte transaction_num;           /* transaction number for this MC-TXC transaction*/
  #endif
} txc_so_cmd_type;

#ifdef FEATURE_IS2000
typedef struct
{
  txc_hdr_type      hdr;  /* Common header */
  mctrans_cmd_type  cmd;  /* Transaction command from MC */
} txc_mc_trans_cmd_type;
#endif // FEATURE_IS2000


typedef struct {                  /* TXC_RDR_F */
  txc_hdr_type hdr;
  byte rdr_limit;                 /* Reverse data rate limit */
  byte rdr_ena_thresh;            /* Reverse data rate limit enable
                                     threshold */
  byte rdr_dis_thresh;            /* Reverse data rate limit disable
                                     threshold */
} txc_rdr_cmd_type;

typedef struct {                  /* TXC_FRAME_OFFSET_F */
  txc_hdr_type hdr;
  byte frame_offset;              /* Traffic Channel frame offset */
  qword action_frame;             /* Frame when frame offset takes effect */
} txc_frame_offset_cmd_type;

/***** Power Control Parameters**********/

#ifdef FEATURE_IS2000

/******** No need to redefine these data structures again
typedef struct {
  boolean fch_incl;
  byte fch_chan_adj_gain;
  boolean sch_0_incl;
  byte sch_0_chan_gain;
  boolean sch_1_incl;
  byte sch_1_chan_gain;
} txc_chan_adj_gain_type;

typedef struct {
  boolean adj_gain_type;
  boolean rs1_20ms_incl;
  byte rl_adj_gain_;
  boolean rs2_20ms_incl;
  byte rl_adj_gain_;
} txc_basic_rate_adj_gain_type;

typedef struct {
  boolean adj_gain_type;
  cai_sch_coding_type coding_type;
  boolean rs1_incl;
  byte rl_adj_gain_19200;
  byte rl_adj_gain_38400;
  byte rl_adj_gain_38400;
  byte adj_gain_rs1[6];
  boolean rs2_incl;
  byte rl_adj_gain_;
} txc_high_rate_adj_gain_type;

typedef struct {
   // revisit
   //cai_adj_record_type  rpc_record_type;
   union {
      txc_chan_adj_gain_type chan_gain_adj;
      txc_basic_rate_adj_gain_type basic_rate_gain_adj;
      txc_high_rate_adj_gain_type high_rate_gain_adj;
   } rpc_adj;

} txc_rpc_gain_adj_rec_type;
*******************/

typedef struct {
  txc_hdr_type hdr;
  qword action_time;
  byte transaction_num; /* Transaction num of this command */
  byte pwr_ctrl_step; /* power control step size */

  boolean rpc_incl; /* Indicates if RPC records are present */
  byte num_of_rpc_records; /* Number of RPC records */
  caii_rpc_type rpc_records[CAI_MAX_NUM_RPC_RECS]; /* RPC records */
} txc_pwr_ctrl_cmd_type;

typedef struct {                  /* TXC_CONNECT_SO_F */
  txc_hdr_type   hdr;
} txc_conn_so_cmd_type;

typedef struct {
  txc_hdr_type hdr;
  byte transaction_num; /* Transaction num to validate the trans */
  qword action_time; /* Action time of this transaction */

  boolean hho_ind; /* HHO Indication */
  boolean rif_ind; /* Return on Failure Indication */

  boolean extra_params; /* Extra params indication */
  byte frame_offset; /* Valid only if extra_params is 1, Change FO only if different */
  byte nom_pwr; /* Valid only if extra_params is 1 */
  byte num_preamble; /* Valid only if extra_params is 1 */
  boolean lcm_change_ind; /* Valid only if extra_params is 1 */
  qword lcm;
  boolean rpc_incl;  /* Valid only if extra_params is 1 */
  byte rlgain_traffic_pilot; /* Valid only if rpc_incl is 1 */
  boolean default_rlag; /* Valid only if rpc_incl is 1 */

  boolean pwr_ctrl_step_incl;  /* Pwr control step size is included in this cmd */
  byte pwr_ctrl_step;  /* pwr ctrl step size valid only if above flag is 1 */

  boolean scr_info_incl;  /* SCR info incl indicator */
  txc_scr_info_type scr_info;  /* SCR info valid only if above is true */

} txc_ho_cmd_type;

typedef struct {
  txc_hdr_type hdr;
} txc_hho_ok_cmd_type;

typedef struct {
  txc_hdr_type hdr;
} txc_rel_cmd_type;

typedef struct {
  txc_hdr_type hdr;
  boolean frame_offset_incl; /* FO included indicator */
  byte frame_offset;  /* Frame offset , valid only if above is TRUE */
  boolean pwr_ctrl_step_incl;  /* Pwr control step size is included in this cmd */
  byte pwr_ctrl_step;  /* pwr ctrl step size valid only if above flag is 1 */
  boolean scr_incl; /* SCR included indicator */
  txc_scr_info_type scr_info; /* SCR info valid only if above is true*/
  boolean lcm_change_ind; /* LCM change indicator */
  qword lcm;  /* Valid only if above is TRUE */
  boolean rev_fch_gating_on; /* Indicates if 8th rate gating is on 
                                     or off on R_FCH */
} txc_returned_cmd_type;

typedef struct {
  txc_hdr_type hdr;
  enc_gating_mode_type gating_mode;
  word gating_pattern;
  byte num_frames_gating_on;
  byte num_frames_gating_off;
} txc_pilot_gating_cmd_type;

#endif //FEATURE_IS2000


typedef union {
  txc_hdr_type       hdr;          /* Generic header */
  txc_cdma_cmd_type  cdma;         /* CDMA init */
  txc_pc_cmd_type    pc;           /* page channel */
  txc_tc_cmd_type    tc;           /* traffic channel */
  txc_exit_cmd_type  exit;         /* exit cdma  */
  txc_msg_cmd_type   msg;          /* message to transmit */
  txc_acc_cmd_type   acc;          /* access attempt */
  txc_stop_cmd_type  stop;         /* stop access attempt and TX clock */
  txc_on_cmd_type    on;           /* start transmitting */
  txc_off_cmd_type   off;          /* stop transmitting */
  txc_nulls_cmd_type nulls;        /* transmit null traffic data */
  txc_so_cmd_type    so;           /* switch service options */
  txc_rdr_cmd_type   rdr;          /* reverse data rate limit parameters */
  txc_frame_offset_cmd_type fo;    /* Set frame offset                   */
  #ifdef FEATURE_IS2000
  txc_pwr_ctrl_cmd_type pwr_ctrl;        /* Change Power Control Parameters */
  txc_conn_so_cmd_type conn_so;    /* Connect SO with vocoder if necessary */
  txc_ho_cmd_type ho;              /* Ho command */
  txc_hho_ok_cmd_type hho_ok;      /* HHO Okay Command*/
  txc_returned_cmd_type ret;       /* HHO returned command */
  txc_rel_cmd_type      rel;       /* Equivalent to sending a SO cmd with null SO */
  txc_mc_trans_cmd_type mc_trans;  /* Transaction command from MC */

  #ifdef FEATURE_PLT
#error code not present
  #endif

  #ifdef FEATURE_FACTORY_TESTMODE
  txc_ftm_sch_cmd_type  ftm_sch;   /* FTM turn on SCH command */
  txc_ftm_loopback_cmd_type loopback; /* FTM loopback command */
  #endif

  #endif
} txc_cmd_type;

typedef struct
{
   byte pch_gain;
   byte fch_gain;
   byte sch_gain;
} txc_gain_type;

typedef enum
{
  TXC_FCH_ONLY=0x0,
  TXC_SCH_ACTIVE,
  TXC_SCH_DTX
} txc_chan_status_type;

#ifdef FEATURE_PLT
#error code not present
#endif // FEATURE_PLT
/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_IS95B
/*===========================================================================

FUNCTION TXC_SUSPEND_TX

DESCRIPTION
  This functions enables/Disables the Transmitter and enables/disables the
  transmission of Access probes.

DEPENDENCIES
  Depends on the current state of the TX task.

RETURN VALUE
  None.

SIDE EFFECTS
  This function is called when the T72 fade timer expires to temporarily
  enable/disable the transmission of the Access Probes.

===========================================================================*/
void txc_suspend_tx (boolean);    /* Enable/Disable probe transmission */
#endif

/*===========================================================================

FUNCTION TXC_POWERUP_INIT

DESCRIPTION
  Initializes timers and queues for CDMA Transmit task processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_powerup_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_CMD

DESCRIPTION
  This is how the outside world interfaces with the tx task.  Call this
  procedure with the address of the txc_cmd_type and the command will be
  placed on the txc_cmd_q

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_cmd
(
  txc_cmd_type *cmd_ptr
    /* The command to queue up */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_RPT_VOC_RATE

DESCRIPTION
  Reports the vocoder rate to the calling routine.

DEPENDENCIES
  None

RETURN VALUE
  Current Vocoder rate.

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_rpt_voc_rate( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_SUBTASK

DESCRIPTION
  Transmit subtask to process CDMA.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void txc_subtask( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_FLAG_ERASURE

DESCRIPTION
  This function is called by RXC  whenever it detects an erasure. TXC reaponds
  by setting the power bit in the frame TXC detects the erasure flag.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_flag_erasure( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_HHO

DESCRIPTION
  This function is called by MC on a hard handoff boundary to set the
  number of preamble frames to be transmitted and the new nominal power.

DEPENDENCIES
 Relies on the fact that TX_OFF_TX_F has already been sent and TXC is now
 waiting for TX_ON_TX_F.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_hho(
word num_preambles,
word nom_pwr
);

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_REGISTER_ACTION_TIME

DESCRIPTION
  This function is called by MC, upon receiving (E)HDM, to enable
  MCC_TXC_FRAME_SIGs for MC's HO action time (within the frame before
  System Action Time) and for RF tune time (within the frame after the
  System Action Time).

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void txc_register_action_time
(
  boolean use_time,
  word action_time,
  boolean enable_second_frm_sig,
  boolean stop_tx_flag
);

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_SET_FRAME_OFFSET

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void txc_set_frame_offset( byte new_frame_offset );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_GET_TIME_IN_1_25MS_UNIT

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern dword txc_get_time_in_1_25ms_unit( void );

#ifdef FEATURE_IS95B_MAHHO
/* <EJECT> */
/*===========================================================================

FUNCTION TXC_FLAG_NEXT_INT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void txc_flag_next_int( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_INT_OCCURRED

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean txc_int_occurred( void );
#endif /* FEATURE_IS95B_MAHHO */

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_DATA_PUMP_INIT

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_PLT
#error code not present
#endif // FEATURE_PLT

/* <EJECT>^L */
/*===========================================================================

FUNCTION TXC_GET_FCH_RATE_SET

DESCRIPTION
  Returns the rate set for FCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern enc_rate_set_type txc_get_fch_rate_set ( void );

/* <EJECT>^L */
/*===========================================================================

FUNCTION TXC_SET_PCH_FCH_GAIN

DESCRIPTION
  Sets the gain values in txc_pch_fch_gain_tab for the specified rate

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void txc_set_pch_fch_gain
(
 byte,
 txc_gain_type,
 int16,
 txc_chan_status_type
);

#ifdef FEATURE_DS_IS2000
/*===========================================================================

FUNCTION TXC_GET_SCH_INFO

DESCRIPTION This function returns current configuration information for
            the SCHs. This is used by the UI in the data display.

            PARAMETERS:
            sch_rate_ptr - points to the current SCH rate.
            double_ptr   - is TRUE when double sized PDUs are being used
                           is FALSE when single sized PDUs are being used

            Note, the above two pointer return values are only meaningful
            when SCH is assigned

DEPENDENCIES None

RETURN VALUE TRUE:  If SCH are assigned.
             FALSE: If no SCH are assigned.

SIDE EFFECTS None

===========================================================================*/
boolean txc_get_sch_info
(
  enc_sch_rate_type* sch_rate_ptr,                   /* ptr to rate of SCH */
  boolean* double_ptr                   /* Ptr to single/double sized PDUs */
);
#endif  /* FEATURE_DS_IS2000 */

#ifdef FEATURE_FFPC
/*===========================================================================
FUNCTION TXC_SET_FPC_MODE

DESCRIPTION
  Sets the value of FPC_MODE in txc_so struct.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void txc_set_fpc_mode (fpc_mode_type);

#endif /* FEATURE_FFPC */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION       TXC_INFORM_SCH_CHNG

DESCRIPTION    Called by data when the SCH config has changed.  This
               figures out the R-SCH throttling parameters.

DEPENDENCIES   None

SIDE EFFECTS   None

RETURN VALUE   None
===========================================================================*/
extern void txc_inform_sch_chng(void);
#endif

#endif /* TXC_H */

