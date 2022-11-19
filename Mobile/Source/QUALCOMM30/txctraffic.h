#ifndef TXCTRAFFIC_H
#define TXCTRAFFIC_H
/*===========================================================================
                 
           C D M A   T R A N S M I T   T R A F F I C   M O D U L E
            
DESCRIPTION
  This header file contains definitions for the transmit traffic module  that 
  are used by the txc unit.  This header file is internal to the txc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/txctraffic.h_v   1.3   21 Feb 2001 10:32:22   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/01   hrk     Added tx_rate_adj to txtc_type.
12/07/00   bgc     Removed initialization of extern for PLT.
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "bio.h"
#include "voc.h"
#include "cai.h"
#include "txc.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Specific variables for the Traffic Channel */
typedef struct
{
  boolean            preambles;    /* Send Traffic Channel preambles  */
  word               num_preambles;/* # of Traffic Channel preambles  */
  boolean            nulls;        /* Send null data  */
  txc_tc_frame_type  frame;        /* Current frame to send */
  byte               dim_delay;    /* Wait for correct rate vocoder frame */
  byte               power;        /* Power level */
  enc_rate_type      rate;         /* current frame encoder rate */
  enc_rate_type      last_rate;    /* Rate of the last tc frame */
  word               sigs;         /* Signal to set when TX task has acquired
                                      timing has moved to traffic state */
  rex_tcb_type       *tcb_ptr;     /* Pointer to task to signal when TX task
                                      has moved to traffic state */
  boolean            ready_to_transmit;
                                   /* Flag to indicate if TC is ready to
                                      transmit (the preamble) during TC
                                      initialization.  We do not transmit
                                      anything before the 2 good frames are
                                      received in TCI. */
  txc_tc_supp_frame_type  supp_frame; /* current supp. frame to send */
  enc_sch_rate_type       supp_rate;


#ifdef FEATURE_IS2000
  int8               rl_gain_adj;
  boolean            fch_8th_rate_gating;
#endif
  byte               offset_pwr; /* turn around constant for
                                    Open Loop Power Estimation */

  boolean            sch_on;
  boolean            supp_turbo;    /* supp. channel Turbo?  */
  word               supp_ltu_size; /* supp. channel ltu size   */

#ifdef FEATURE_IS2000_R_SCH
  boolean            txc_dtx_off;           /* DTX expired? */
  boolean            dtx_duration_included; /* DTX activate? */
  byte               dtx_duration_org;      /* original/reset value */
  uint16             dtx_duration;          /* active value */

  byte               pot_num_mux_pdus; /* # of MuxPDUs in a R-SCH frame    */
  byte               ok_num_mux_pdus;  /* allowed # mux=-pdus in a frame   */

  boolean            double_sized;  /* TRUE if double sized fr being tx */
#endif /* FEATURE_IS2000_R_SCH */

  byte               pwr_ctrl_step_size; /* Step size */
#ifdef FEATURE_FACTORY_TESTMODE
  boolean            fch_loopback;  /* fch loopback mode */
  boolean            sch_loopback;  /* sch loopback mode */
#endif
  int16              tx_rate_adj;
} txtc_type;

extern txtc_type txtc;

extern byte *prim_data_ptr;           /* Pointer to primary data */
extern cai_data_rate_type prim_rate;  /* Primary data rate */

#ifdef FEATURE_FACTORY_TESTMODE
  /* Holding buffer for TX data on the reverse FCH*/
  extern txc_tc_frame_type  txc_ftm_rfch_data;
  extern enc_rate_type txc_ftm_rfch_rate;
 
  /* Holding buffer for TX data on the reverse SCH*/
  extern txc_tc_supp_frame_type  txc_ftm_rsch_data;
  extern enc_sch_rate_type txc_ftm_rsch_rate;
#endif /*FEATURE_FACTORY_TESTMODE*/

#if defined(FEATURE_IS2000)
  /* Accumulation buffer */
  extern log_rev_pctrl_type txc_log_rpctrl_buf;
  /* Sub-record pointer */
  extern log_rev_pctrl_subrecord_type *txc_log_rpctrl_sr_ptr;

  #define TXC_RPCTRL_LOG_OFFSET \
  (txc_log_rpctrl_buf.hdr.len - FPOS(log_rev_pctrl_type, var_len_buf))

  /* Reverse frame types log accumulation buffer */
  /* Accumulation buffer */
  extern log_rev_frame_types_type           txc_log_frm_types_buf;

  /* R-SCH0 frame type */
  extern log_sch_rate_enum_type txc_sch0_frame_rate;

  /* Sub-record pointer */
  extern log_rev_frame_types_subrecord_type *txc_log_frm_types_sr_ptr;

  #define TXC_FRM_TYPES_LOG_OFFSET \
  (txc_log_frm_types_buf.hdr.len - FPOS(log_rev_frame_types_type, var_len_buf))
#endif /* FEATURE_IS2000 */


#if defined(FEATURE_RLP_LOGGING)
  /* Buffer for RLP frames logging */
  extern log_rlp_frames_type txc_log_rlp_frm_buf; /* Buffer to accumulate RLP log   */
#endif

#ifdef FEATURE_PLT
#error code not present
#endif // FEATURE_PLT

/* Handoff Action Time information */
extern boolean txc_use_action_time;
  /* TRUE --- explicit action time;
   * FALSE --- implicit action time.
   */

extern boolean txc_second_frm_sig_enabled;
  /* When "txc_second_frm_sig_enabled" is TRUE, TXC will report
   * "MCC_TXC_FRAME_SIG" to MC at the next "TXC_INT_SIG" after
   * TXC sends the first "MCC_TXC_FRAME_SIG" so that MC can tune
   * to different frequency after the transmitter is turned off
   */

#ifdef FEATURE_IS2000
   extern boolean txc_connect_so;
   extern boolean txc_hho_preamble_pcg_units;

   #ifdef FEATURE_MAX_PWR_PROTECTION
	extern int16  tx_extra_gain;
   #endif /* FEATURE_MAX_PWR_PROTECTION */
#endif

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TXC_TRAFFIC

DESCRIPTION
  Process traffic channel state

DEPENDENCIES
  None
RETURN VALUE
  Next state.
SIDE EFFECTS
  None
===========================================================================*/
extern txc_state_type txc_traffic( txc_state_type state );

/*========================================================================
FUNCTION TXC_RLGC_TRANS_CMD

DESCRIPTION
   The RLGC transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
#ifdef FEATURE_IS2000
  extern void txc_rlgc_trans_cmd( mctrans_action_packet_type *pkt_ptr );
#endif

/*========================================================================
FUNCTION TXC_HO_TRANS_CMD

DESCRIPTION
   The HO transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
#ifdef FEATURE_IS2000
  extern void txc_ho_trans_cmd( mctrans_action_packet_type *pkt_ptr );
#endif

/*===========================================================================

FUNCTION TXC_TRAFFIC_INIT

DESCRIPTION
  Initialization section for traffic channel.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
extern void txc_traffic_init( void );

#ifdef FEATURE_IS2000_R_SCH
/*========================================================================
FUNCTION TXC_REV_SCH_TRANS_CMD

DESCRIPTION
   The reverse SCH transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void txc_rev_sch_trans_cmd( mctrans_action_packet_type *pkt_ptr );
#endif

/*===========================================================================

FUNCTION TXC_TRAFFIC_ISR

DESCRIPTION
  Interrupt handler for the traffic channel.  Merge the vocoder and
  signaling data and give them to the encoder.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_traffic_isr( void );

#endif

