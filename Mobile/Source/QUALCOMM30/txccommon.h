#ifndef TXCCOMMON_H
#define TXCCOMMON_H

/*===========================================================================
                 
          C D M A    T R A N S M I T   C O M M O N   M O D U L E
 
 DESCRIPTION 
  This header file contains definitions for the transmit common module that 
  are used by the txc unit.  This header file is internal to the txc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/txccommon.h_v   1.1   16 Feb 2001 11:21:12   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   hrk     Added prototype for txc_rpctrl_send_log().
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "cai.h"
#include "bio.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* commands to the interrupt handlers */
typedef enum {
  INT_TC_F,                       /* traffic channel */
  INT_FRAME_F                     /* initialize frame count */
}txc_int_cmd_type;

/* define buffers to hold the messages to be sent */
typedef struct {
  txc_cmd_type *cmd_ptr;           /* command pointer */
  boolean     blank;              /* blank and burst. */
  word        len;                /* length, in bits of the message */
  word        pos;                /* current position, in bits */
  qword       mask;               /* access channel mask */
  byte        msg[MAX(CAI_REV_TC_MAX_MSG_BYTES,CAI_AC_MAX_MSG_BYTES)]; /* the message */
} txc_msg_buf_type;

/* macro used to set interrupt command and wait for one interrupt to occur */
#define DO_INT_CMD()                    \
  (void)rex_clr_sigs( &tx_tcb, TXC_INT_CMD_SIG );  \
  txc_int_cmd.iscmd = TRUE;              \
  (void)txc_wait( TXC_INT_CMD_SIG );               \
  (void)rex_clr_sigs( &tx_tcb, TXC_INT_CMD_SIG );


/* This moves the conv encoder interrupt to 10ms from the begining of the
** frame. This is done to separate the viterbi decoder interrupt which happens
** at 4.5ms into the frame and the conv encoder interrupt which *used to*
** happen at 5ms previously. Applicable only to voice calls.
*/
#define TXC_INT_OFFSET_PCG      8                /* 1 PCG = 1.25 ms */

#ifdef FEATURE_DATA_TIMELINE_ADVANCE
#define TXC_INT_OFFSET_PCG_DATA 4                /* 1 PCG = 1.25 ms */
#endif // FEATURE_DATA_TIMELINE_ADVANCE


#define TXC_INT_OFFSET_WALSH    0

/* Queue definitions */
extern q_type txc_cmd_q;

/* Timer to wake Transmit task so that it can set its Watchdog task
   Report signal */
extern rex_timer_type  txc_rf_timer;

extern dword txc_pwr_boost_cnt;  /* Number of times full rate boost occurred */
extern int1 txc_pwr_boost;       /* Power boost */

/* Data rate of current frame */
extern cai_data_rate_type txc_data_rate;

extern byte zeros[ENC_14400_FULL_RATE_SIZE];

/* The possible states the TXC task can be in */
typedef enum {
  TXC_CDMA_S,                      /* Initializing CDMA */
  TXC_ACC_IDLE_S,                  /* Idle on Paging channel */
  TXC_ACC_JUMP_S,                  /* Performing jump to hyperspace */
  TXC_ACC_ATT_S,                   /* Performing an access attempt */
  TXC_TRAFFIC_S,                   /* Traffic channel */
  TXC_EXIT_S                       /* Stop state */
}txc_state_type;
/* Curent state of TXC state machine */
extern txc_state_type  txc_state;

/* command block for interrupt handler commands */
typedef struct  {
  boolean iscmd;                  /* is there a command? */
  txc_int_cmd_type    cmd;
  union 
  {
    struct {                      /* traffic channel */
      qword mask;                 /* long pn mask to use */
    } tc;
  } parm;
} txc_int_cmd_type_main;
extern txc_int_cmd_type_main txc_int_cmd;

/* hold most of the global variables for the transmit task */
typedef struct {
  word             int_cnt;        /* count interrupts */
  qword            frame_cnt;      /* count transmit frames */
  qword            mask;           /* long pn mask to use */
  boolean          active_msg;     /* flag for active message buffer */
  boolean          tx_on;          /* indicator of whether transmitter
                                      is on */
  boolean          tx_blank;       /* expect a blank from vocoder */
  boolean          jump_traffic;   /* test mode straight ot traffic on */
  word             delay;          /* frame delay for msg sent signal */
  txc_msg_buf_type buf;            /* buffer to send */
} txc_tx_type;
extern txc_tx_type txc_tx;

typedef struct
{
  boolean pending;     /* Indicates whether a frame offset cmd is pending */
  byte frame_offset;   /* Traffic Channel frame offset */
  qword action_frame;  /* Frame when frame offset takes effect */
} txc_frame_offset_cmd_type_pend;
extern txc_frame_offset_cmd_type_pend txc_frame_offset_cmd;

extern byte txc_frame_offset;
/* Offset of the transmit interrupt from the frame boundary for Traffic
** Channel.*/


#ifdef FEATURE_FINGER_POLL

/* clock call-back for fingerlock*/
extern clk_cb_type txc_fnglock_call_back;

/* Has the txc_fnglock_call_back been defined?*/
extern boolean txc_fnglock_cb_defined;

/* Has the txc_fnglock_call_back been registered?*/
extern boolean txc_fnglock_cb_registered;
#endif /*FEATURE_FINGER_POLL*/



#if defined (FEATURE_DIAG_V7_LOG_IFACE)
extern q_type txc_log_msg_q;              /* for logging sent messages */
#ifdef TMOBILE
#error code not present
#endif

/*-------------------------------------------------------------------------*/
/* Buffers for Logging Queues                                              */
/*-------------------------------------------------------------------------*/

typedef struct {
  log_desc_type desc;             /* log descriptor for logging services */
  log_voc_type  voc;              /* vocoder frame log type */
} txc_log_voc_type;

typedef struct
{
  log_desc_type desc;             /* log descriptor for logging services */
  log_r_mux1_type  entry;         /* transmitted frame rates log */
} txc_log_mux1_type;
extern txc_log_mux1_type *txc_mux1_ptr;
    /* pointer to current log buffer */


typedef struct
{
  log_desc_type desc;             /* log descriptor for logging services */
  union
  {
    log_hdr_type       hdr;       /* log header */
    log_ac_msg_type    ac_msg;    /* access channel message buffer */
    log_r_tc_msg_type  tc_msg;    /* traffic channel message buffer */
  }entry;
} txc_log_msg_type;
/* buffers for txc_log_msg_q */
#define                          TXC_NUM_MSG_BUFS 4
extern txc_log_msg_type txc_log_msg_pool[ TXC_NUM_MSG_BUFS ];
#endif /* FEATURE_DIAG_V7_LOG_IFACE */


typedef struct
{
   enc_fch_radio_config_type fch_rc;
//
// rate set is redundant info, but we convert rc to rate set and store it here anyway
//
  enc_rate_set_type rate_set;
  word pri;
  word sec;
#ifdef FEATURE_IS2000
  word stored_pri;
#endif

#ifdef FEATURE_FFPC
  fpc_mode_type fpc_mode;
#endif
} txc_so_type;
extern txc_so_type txc_so;

/* gain table taken from Tige2 HLD */
extern txc_gain_type txc_pch_fch_sch_gain_tab[ ENC_NUM_SCH_RATES ];

extern txc_gain_type txc_pch_fch_gain_tab[ ENC_NUM_FRAC_RATES ];

/* Adjusted R-PCH Gain Table when only R-FCH is active
** and R-SCH is not assigned */
extern int16 txc_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ];

#ifdef FEATURE_IS2000_R_SCH
   extern int16 txc_sch_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ];
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000_R_SCH
  /* Gain Table when R-FCH and R-SCH is assigned and R-SCH is active */
  extern txc_gain_type txc_gain_tab[ ENC_NUM_FRAC_RATES ];

  /* Gain table when R-FCH and R-SCH is assigned but R-SCH is inactive */
  extern txc_gain_type txc_dtx_gain_tab[ ENC_NUM_FRAC_RATES ];

  extern int16 txc_dtx_pch_adj_gain_tab[ ENC_NUM_FRAC_RATES ];
#endif /* FEATURE_IS2000_R_SCH */

extern word  txc_action_time;
  /* System Action Time as specified in OTA message */

extern boolean txc_action_time_chk_enabled;
  /* When TRUE, txc_traffic() should check if the action time is up
   * while processing "TXC_INT_SIG". If the action time is up,
   * TXC will send "MCC_TXC_FRAME_SIG" to MCC
   */

extern boolean txc_action_time_stop_tx_enabled;
  /* When TRUE, TXC will command the transmitter to be shut down at
   * action time, i.e. while the first MCC_TXC_FRAME_SIG is generated
   */

#ifdef FEATURE_IS95B_MAHHO
  extern boolean txc_next_int_occurred;
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
  extern q_type txc_action_q;
  extern q_type txc_action_buf_q;
  typedef struct
  {
     q_link_type link;
     qword action_time;
     byte transaction_num;
  } txc_action_entry_hdr_type;

  typedef struct
  {
     txc_action_entry_hdr_type hdr;
     txc_cmd_type  action;
  } txc_action_entry_type;

  #define TXC_NUM_ACTION_BUFS 10
  extern txc_action_entry_type txc_action_buf_pool[ TXC_NUM_ACTION_BUFS ];
#endif

extern boolean txc_erasure_flag;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TXC_CMD_DONE

DESCRIPTION
  Dispose of a command buffer when we're done with it.  Depending of
  the contents of the buffer, this will involve placing it on a free
  queue if a free queue is designated,  and if a tcb is given,  the
  appropriate task will be notified.

DEPENDENCIES
  None.
RETURN VALUE
  Just returns the status passed to it
SIDE EFFECTS
  Could signal another task
===========================================================================*/
extern void txc_cmd_done(
  txc_cmd_type   *cmd_ptr,         /* buffer to dispose of */
  word          status            /* set status of buffer */
);


/*===========================================================================

FUNCTION TXC_CLR_MSGS

DESCRIPTION
  Move any pending messages in the messages buffer over to the free
  queue,  effectively erasing any queued messages.

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_clr_msgs( void );

/*===========================================================================

FUNCTION TXC_WAIT

DESCRIPTION
  Wait for the watchdog and for any other signals that might get set.
  Return if one of the other signals is set.  Always kick the watchdog
  whenever we wake up.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the signals for the current task.

SIDE EFFECTS
  None.

===========================================================================*/
extern word txc_wait
(
  word requested_mask
    /* Mask of signals to wait for */
);


/*===========================================================================

FUNCTION TXC_CDMA_ENC_SETUP

DESCRIPTION
  Set up the encoder / interleaver / deinterleaver for CDMA

DEPENDENCIES
  Assume interleaver already hard reset
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
extern void txc_cdma_enc_setup(void);

/*===========================================================================

FUNCTION TXC_CDMA_ENC_SHUTDOWN

DESCRIPTION
  Shut down the encoder / interleaver / deinterleaver & PA for CDMA

DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
extern void txc_cdma_enc_shutdown(void);

/*===========================================================================

FUNCTION TXC_EXIT

DESCRIPTION
  This function performs processing necessary for exiting CDMA Transmit
  subtask.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void txc_exit (void );

#if defined(FEATURE_RLP_LOGGING)
/* <EJECT> */
/*===========================================================================

FUNCTION TXC_SEND_RLP_FRAMES_LOG

DESCRIPTION
  Terminate and send the accumulated RLP partial frames log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A RLP frames log may be sent.

===========================================================================*/
extern void txc_send_rlp_frames_log(void);

#endif /*FEATURE_RLP_LOGGING*/

#if defined(FEATURE_IS2000)
/* <EJECT> */
/*===========================================================================

FUNCTION TXC_RPCTRL_LOG_INIT

DESCRIPTION
  This function initializes the Reverse Power Control log subrecord, as well as the
  pointer to it and the frame entry, in the log buffer.  If necessary, the log is
  sent and a new one is generated.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A reverse power control log may be sent.

===========================================================================*/
extern log_rev_pctrl_frame_type * txc_rpctrl_log_frame_init(void);

/*===========================================================================

FUNCTION TXC_SCR_ACTION

DESCRIPTION
  Performs a service option change "action"

DEPENDENCIES
  None.
RETURN VALUE
  Never returns.
SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_scr_action( txc_scr_info_type scr_info );

/*===========================================================================
FUNCTION TXC_SERVICE_TRANS_CMD

DESCRIPTION This function services the TXC_MC_TRANS_F command
at action time.

DEPENDENCIES ??

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
extern void
txc_service_trans_cmd
(
  txc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION TXC_ADD_ACTION

DESCRIPTION
  Adds an action in its right location--based on action time--into
  the sorted action time queue

DEPENDENCIES
  None.
RETURN VALUE
  Never returns.
SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_add_action( txc_action_entry_type *action_ptr );

/*===========================================================================

FUNCTION TXC_RPCTRL_SEND_LOG

DESCRIPTION
  Sends RPC log.

DEPENDENCIES
  None.
RETURN VALUE
  Never returns.
SIDE EFFECTS
  None.
===========================================================================*/
extern void txc_rpctrl_send_log (void);

#endif /*FEATURE_IS2000*/

#endif /*TXC_COMMON_H*/

