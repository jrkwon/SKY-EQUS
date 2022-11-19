#ifndef RXCTRAFFIC_H
#define RXCTRAFFIC_H
/*===========================================================================

            C D M A    R E C E I V E   T R A F F I C    M O D U L E

DESCRIPTION
  This header file contains definitions for the receive traffic module
  that are used by the rxc unit.  This header file is internal to the rxc
  unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/rxctraffic.h_v   1.2   09 Mar 2001 17:53:22   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/09/01   lcc     Changed rxc_stored_sch_packet to use type mctrans_rxc_f_sch_action_type.
11/13/00   bgc     Added accessor functions for FTM loopback flags.
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "log.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#if defined (FEATURE_DIAG_V7_LOG_IFACE)
  /* Type to log MAR data */
  typedef struct
  {
    log_desc_type desc;           /* log descriptor */
    log_mar_type  entry;          /* mar entry      */
  } rxc_log_mar_type;

  /* Structure for logging vocoder packets */
  typedef struct {
    log_desc_type desc;
    log_voc_type  voc;
  } rxc_log_voc_type;

#endif

#ifdef FEATURE_IS95B_MAHHO
  /* Indicator for a Return-If-Fail HHO or CFS                            */
  typedef struct {
    boolean previous;
    boolean current;
  } rxc_erasure_flags_type;
#endif /* FEATURE_IS95B_MAHHO */


extern byte *rxc_vbits;        /* Command buffer */


extern boolean rxc_erase;
  /* indicator of whether to treat blank and burst as erasures */


#ifdef FEATURE_IS2000
  /* Decision history for FFPC logging */
  extern word rxc_fpc_history;
#endif


#if defined(FEATURE_RLP_LOGGING)
  /* Buffer for RLP frames logging */
  extern log_rlp_frames_type rxc_log_rlp_frm_buf;

  extern boolean rxc_log_sch0_erasure;

  #define RXC_RLP_SCH_PAYLOAD_LOG_LEN 5
#endif /* FEATURE_RLP_LOGGING */


#if !defined (FEATURE_DIAG_V7_LOG_IFACE)
  extern log_mar_type rxc_log_mar_buf; /* Log buffer for accumulated Markov frame log */
#else
  extern q_type rxc_log_mar_q;  /* For logging MAR data */

  #ifdef TMOBILE
#error code not present
  #endif

  /*-------------------------------------------------------------------------*/
  /* Buffers for Logging Queues                                              */
  /*-------------------------------------------------------------------------*/

  /* buffers for rxc_log_mar_q */
  #define                           RXC_NUM_MAR_BUFS 5
  extern rxc_log_mar_type rxc_log_mar_pool[ RXC_NUM_MAR_BUFS ];

  /* Number of mar entries currently in Markov Frame log buffer */
  extern byte rxc_mar_cnt;

  /* pointer to buffer to buffer to log Markov data in */
  extern rxc_log_mar_type *rxc_mar_buf_ptr;

#endif /* defined(FEATURE_DIAG_V7_LOG_IFACE) */



extern boolean rxc_fade; /* indicator of whether fade processingis in
                            progress */
extern word rxc_ho_cnt;  /* number of frames before handoff processing */


extern byte rxc_voc_rate;      /* Current vocoder frame rate */
extern word rxc_lost_voc_pkts; /* Number of lost logged vocoder packets */


/*-------------------------------------------------------------------------*/
/* Variables for Hard Handoff with Return-on-Failure                       */
/*-------------------------------------------------------------------------*/
#ifdef FEATURE_IS95B_MAHHO
  extern word rxc_lost_frames;
  /* Lost frame count during a Return-If-Fail HHO                         */
  extern word rxc_rif_hho_timeout_frames;
  /* Equivalent of the Return-If-Fail hard handoff timer                  */
  extern boolean rxc_rif_hho_or_cfs_in_progress;

  extern rxc_erasure_flags_type rxc_erasure_flags;

  extern boolean rxc_use_stored_eibs;
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_DS_IS2000
  #define SCH_MAX_FWD_PDU 8                     /* Max # of F-SCH MuxPDUs*/
  extern dsm_item_type * rxc_dsm_arr[ SCH_MAX_FWD_PDU]; /* Arr of SCH RLP frames*/
  #define RXC_DSM_ARR_NOT_ALLOC 0xFF               /* Const for invalid arr*/
  extern byte rxc_dsm_arr_cnt;                  /* Current index in arr */
#endif

/*-------------------------------------------------------------------------*/
/*  Supplemental Channel variable                                          */
/*-------------------------------------------------------------------------*/
extern dec_sch_cfg_type rxc_sch;

extern boolean rxc_voc_config_done;
extern rxc_frame_type rxc_sch_frame_status;

extern mctrans_rxc_f_sch_action_type rxc_stored_sch_packet;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION RXC_FRAME_TYPES_SEND_LOG

DESCRIPTION
  Terminate and send the accumulated forward frame types log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A forward frame types log may be sent.

===========================================================================*/
#if defined(FEATURE_IS2000)
extern void rxc_frame_types_send_log(void);
#endif /* FEATURE_IS2000 */

/*===========================================================================

FUNCTION RXC_TCI

DESCRIPTION
  Traffic Channel Initialization processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern word rxc_tci ( void );

/*===========================================================================

FUNCTION RXC_TC

DESCRIPTION
  Traffic Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern word rxc_tc( void );

/*===========================================================================

FUNCTION RXC_LOG_MAR

DESCRIPTION
  This function logs the Forward Traffic Channel Markov test data.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_log_mar(byte mar,byte ber);

/*===========================================================================

FUNCTION RXC_TC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Traffic Channel.
  Due to timing requirements imposed by the 13k decoder and the erasure
  indicator bit turnaround time we need to first process the decoded frame
  and decide what rate it is before building the final tx frame at this
  point in time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_isr( void );

/*===========================================================================

FUNCTION RXC_RLP_FRAME_LOG_INIT

DESCRIPTION
  This function initializes the next frame packet, as well as the
  pointer to it, in the RLP frames log buffer.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to frame log entry.

SIDE EFFECTS
  A RLP partial frames log may be sent.

===========================================================================*/
#if defined(FEATURE_RLP_LOGGING)
  extern log_rlp_pkt_type * rxc_rlp_frame_log_init(void);
#endif /* FEATURE_RLP_LOGGING */

/*===========================================================================

FUNCTION RXC_LOG_RLP_STATISTICS

DESCRIPTION
  Logs the current values of the RLP statistics to the off-target diagnostic
  tool.  If we can't allocate space for the log packet on the DIAG ring
  buffer, we'll abort and try again next time.

  Input Parameters:
    sr_id: The Service ID for the particular service. Primary is usually 1
           and secondary is usually 2.
    callTerminated: True if we're logging at call termination, False if
           we're logging periodically during the call.

DEPENDENCIES
  Logging mask, rxc_rlp_stats_interval module global

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#if defined(FEATURE_RLP_LOGGING)
  extern void rxc_log_rlp_statistics( byte sr_id, boolean callTerminated );
#endif /* FEATURE_RLP_LOGGING */

/*===========================================================================

FUNCTION PWR_MSR_CHECK

DESCRIPTION
  This procedure processes the updated rxc_pwr structure to determine if a
  power measurement report should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A power measurement report may be sent to MC.

===========================================================================*/
extern void pwr_msr_check( void );

/*=========================================================================
FUNCTION RXC_LOG_FRAME_TYPE

DESCRIPTION
  This function populates frame type logs.

DEPENDENCIE
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
#if defined(FEATURE_IS2000)
  extern void rxc_log_frame_type
  ( int16 frame_rate,                /* FCH  Frame rate */
    log_fch_type_enum_type fch_type, /* FCH  Frame type */
    log_sch_rate_enum_type sch0_type /* SCH0 Frame type */
  );
#endif

/*===========================================================================

FUNCTION RXC_TC_PARSE

DESCRIPTION
  This function collects the signaling bits of the Traffic Channel.
  When it determines that an entire Traffic Channel message has been
  received and the message CRC checks, it will place the message on
  a queue for the Layer 2 task and signal the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicator of whether message data was good.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_tc_parse
  (
    qword  frame_num,
    byte   frame[],    /* Address of 1st byte of frame. */
    word   frame_len   /* Number of bits in frame */
  );


/*===========================================================================

FUNCTION RXC_TCI_INIT

DESCRIPTION
  This funtion initializes the Traffic Channel initialization state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tci_init(  word curr_state );


/*===========================================================================

FUNCTION FADE_CHECK

DESCRIPTION
  This procedure processes the frame rate decision and updates the fade
  good and bad frame counts accordingly to determine if a fade report
  should be sent to MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A fade report may be sent to MC.

===========================================================================*/
extern void fade_check( byte mux );

#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================

FUNCTION RXC_FCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether FCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_fch_loopback_query( void );

/*===========================================================================

FUNCTION RXC_SCH_LOOPBACK_QUERY

DESCRIPTION
  This procedure returns whether SCH loopback is enabled for Factory
  Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean, true if loobpack is on.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean rxc_sch_loopback_query( void );

/*===========================================================================

FUNCTION RXC_SET_FCH_LOOPBACK

DESCRIPTION
  This procedure sets FCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
extern void rxc_set_fch_loopback( boolean );

/*===========================================================================

FUNCTION RXC_SET_SCH_LOOPBACK

DESCRIPTION
  This procedure sets SCH loopback for Factory Test Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Loopback mode is set in rxctraffic.

===========================================================================*/
extern void rxc_set_sch_loopback( boolean );

#endif

#endif /* RXCTRAFFIC_H */

