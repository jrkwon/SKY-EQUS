#ifndef RXCCOMMON_H
#define RXCCOMMON_H
/*===========================================================================

             C D M A    R E C E I V E   C O M M O N   M O D U L E

DESCRIPTION
  This header file contains definitions for the receive common module that 
  are used by the rxc unit.  This header file is internal to the rxc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/rxccommon.h_v   1.0   10 Nov 2000 16:48:12   ddalke  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "tdso.h"
#include "dec.h"
#include "rex.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Command block for Interrupt handlers */
typedef struct
{
  q_link_type      link;   /* Queue links */
  byte cmd;                /* Command to be processed */
  word sci_mask;           /* current sci_mask */
  word idle_pgslot;        /* assigned paging slot */
}rxc_isr_cmd_type;

/*-------------------------------------------------------------------------*/
/*  Forward Traffic Channel Power Control Parameters                       */
/*-------------------------------------------------------------------------*/

typedef struct
{
  boolean threshold_on;
    /* Threshold report mode indicator */
  boolean periodic_on;
    /* Periodic report mode indicator */
  word delay_frames;
    /* Number of frames to delay after a Power Measurement message is
       sent before recording */
  word pwr_thresh;
    /* Threshold for bad frames before Power Measurement message is sent */
  word pwr_frames;
    /* Maximum number of frames before a Power Measurement message is sent */
  byte num_bad_frames;
    /* Number of category 9 and 10 frames received */
  word num_frames;
    /* Total number of frames received */
  word num_delay_frames;
    /* Number of frames delayed */

#ifdef FEATURE_IS2000
  word sch_tot_frames[2]; /* for SCH0 and SCH1 */
  word sch_bad_frames[2]; /* for SCH0 and SCH1 */
  boolean sch_fer_rep[2]; /* for SCH0 and SCH1 */
#endif

}  rxc_pwr_type;


/*-------------------------------------------------------------------------*/
/* Data structures and variables for RXC ACTION TIME PROCESSING            */
/*-------------------------------------------------------------------------*/

typedef struct rxc_action_list_item_struct
{
  rxc_cmd_msg_type action_cmd;
  qword action_time;
  struct rxc_action_list_item_struct *prev_ptr;
  struct rxc_action_list_item_struct *next_ptr;
} rxc_action_list_item_type;

#ifdef FEATURE_IS95B_MDR
  /*-------------------------------------------------------------------------*/
  /*  Supplemental Code Channel (IS95B SCCH )variable                        */
  /*-------------------------------------------------------------------------*/

    typedef struct
	{
     /* Number of Supplemental Code Channels */
     byte num_sup;
	} rxc_scch_type;
#endif

/*-------------------------------------------------------------------------*/
/* Service Option variables                                                */
/*-------------------------------------------------------------------------*/

typedef struct
{
  dec_rate_set_type rate_set;           /* rate set in use */
  deint_rc_type rc;
  word pri;                /* Primary service option */
  word sec;                /* Secondary service option */
#ifdef FEATURE_PLT
#error code not present
#endif
#ifdef FEATURE_FACTORY_TESTMODE
  boolean enable_skip_tci;   /* disables check for good frames in TCI */
#endif
#ifdef FEATURE_IS2000
  byte transaction_num;  /* transaction number for action times */
#endif /* FEATURE_IS2000 */
} rxc_so_type;

/* function prototype for action time callback function */
typedef void (*rxc_atmr_func_type)(void);

/* function prototype for transaction callback function */
typedef void (*transaction_processor_type) (mctrans_action_packet_type *);

/*-------------------------------------------------------------------------*/
/* Variables for counting slots                                            */
/*-------------------------------------------------------------------------*/
typedef struct
{
  boolean active;     /* indicator of whether we are checking slots */
  boolean valid_msg;  /* indicator of whether a valid msg was received */
  word slot_frames;   /* number of frames remaining in the 2 slot period */
  word slot_now;      /* current slot */
  byte slot_mask;     /* the slot mask indicating the type of slot(s) RXC is about to enter */
  byte valid_msg_mask; /* the type of slots that valid msg reporting is for */
#ifdef FEATURE_BROADCAST_SMS
  word bcpg_slot_frames; /* number of frames remaining in the broadcast page slots */
  word bcsms_slot_frames; /* number of frames remaining in the broadcast SMS slots */
  boolean bc_started; /* the scheduled BC has started in the first slot */
#endif /* FEATURE_BROADCAST_SMS */
  word slot;          /* assigned paging slot modulo sci_mask */
  word idle_pgslot;   /* Hashed paging channel slot */
  word sci_mask;      /* Mask to get position-in-slot from pgslot */
  boolean after_slot_boundary;
  boolean skipping;   /* Decoder interrupts skipped until 20ms after slot */
  boolean page_match; /* Enable page matching for first frame after slot */
  boolean blank;      /* Enable blank for first *int* (not frame) aft slot */
  byte frame_skip_cnt;/* Count used to know when to jump out of skipping state */
} rxc_slot_type;

#define RXC_TIMER_NOT_SET -1

/* Table to convert forward link RC to rate set */
/* Note: RC starts from 1, zeroth entry is a filler.  All RCs entries defined */
/* here, but Tiger 2 only supports 1 - 5. */
extern const dec_rate_set_type fwd_rc_to_rate_set[10];


extern rex_timer_type  rxc_fade_timer;
  /* Timer for amount of time since mobile last detected a valid SAT while
     communicating with the base station over the voice channel */

#ifdef FEATURE_IS95B
  extern rex_timer_type rxc_aho_fade_timer;
  /* This timer is used for T72M timer monitoring */
#endif /*FEATURE_IS95B*/

/* Queues */

extern q_type          rxc_dec_q;
  /* Queue for signaling data packets received from the decoder */

extern q_type          rxc_dec_free_q;
  /* Queue for free buffers to be placed, when filled,
     on the rxc_dec_q queue */

extern q_type          rxc_isr_q;
  /* Queue for command packets to be processed by the Receive interrupt
     handler */

extern byte rxc_frame_offset;
  /* Stores the current frame offset of the forward link traffic
     channel */

extern byte rxc_frame_offset_countdown;
  /* Used as a counter to determine of rxc_dec_frame needs to be recomputed */


extern dec_fch_status_type rxc_fch_status;

extern byte  rxc_zero_count; /* Indicator of whether SER statistics should be set to
                        zero - anything other than 0 = reset */

extern int rxc_time_cnt;


/*-------------------------------------------------------------------------*/
/* Interrupt Service Routine variables */
/*-------------------------------------------------------------------------*/

/* Frame number */
extern qword  rxc_dec_frame;

/* command block for commands to be sent to isrs */
extern rxc_isr_cmd_type rxc_isr_cmd_buf;

#ifdef FEATURE_IS2000
  /* logging AGC values */
  extern byte log_rx_agc;
  extern byte log_tx_pwr;
  extern byte log_tx_gain_adj;
#endif

#if defined (FEATURE_DIAG_V7_LOG_IFACE)
extern q_type rxc_log_msg_q;  /* for logging received messages */
  typedef struct
  {
    log_desc_type desc;           /* log descriptor */
    union
        {
      log_hdr_type       hdr;    /* log header */
      log_sc_msg_type sc_msg;    /* Sync Channel message */
      log_pc_msg_type pc_msg;    /* Paging Channel message */
      log_f_tc_msg_type tc_msg;  /* Traffic Channel message */
        }entry;
  } rxc_log_msg_type;

  /* buffers for rxc_log_msg_q */
#define                          RXC_NUM_MSG_BUFS 5
extern rxc_log_msg_type rxc_log_msg_pool[ RXC_NUM_MSG_BUFS ];
#endif

/*  Forward Traffic Channel Power Control Parameters  */

extern rxc_pwr_type rxc_pwr;

/* Service Option variables */

extern rxc_so_type rxc_so;

#ifdef FEATURE_IS95B_MDR
/*-------------------------------------------------------------------------*/
/*  Supplemental Code Channel (IS95B SCCH )variable                        */
/*-------------------------------------------------------------------------*/
   extern rxc_scch_type rxc_scch;
#endif  /* FEATURE_IS95B_MDR */

extern boolean rxc_sup_dec_int_enable;
extern boolean rxc_turbo_dec_enabled;

#ifdef FEATURE_PLT
#error code not present
#endif  /* FEATURE_PLT */

/*-------------------------------------------------------------------------*/
/* Variables for counting slots                                            */
/*-------------------------------------------------------------------------*/
extern rxc_slot_type rxc_slot;

extern word rxc_log_msg_drop;     /* number of messages dropped due to no
                                 available buffer */

#ifdef FEATURE_IS95B_MAHHO
  /* Indicator to ignore good frame while doing a Return-If-Fail HHO      */
  extern boolean rxc_ignore_good_frame;
#endif /* FEATURE_IS95B_MAHHO */
  
extern q_type rxc_action_item_free_q;

extern atmr_handle_type rxc_atmr_handle;

#ifdef FEATURE_IS2000_TDSO
  extern tdso_frame_block_type rxc_tdso_frame_block;
#endif // FEATURE_IS2000_TDSO
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXC_EXIT

DESCRIPTION
  This function performs processing necessary when leaving the RX CDMA
  subtask

DEPENDENCIES
  none.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_exit( void );


/*========================================================================
FUNCTION RXC_SCR_TRANS_CMD

DESCRIPTION
  Process SCR transaction request from MC.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/  
#ifdef FEATURE_IS2000
  extern void rxc_scr_trans_cmd (mctrans_action_packet_type *cmd_ptr);
#endif /* FEATURE_IS2000 */
/*========================================================================

FUNCTION RXC_CONFIG_DEMOD

DESCRIPTION This function configures the demodulator and combiner based on
            Radio Configuration

DEPENDENCIES ??

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
extern void rxc_config_demod( deint_rc_type rc );

/*========================================================================
FUNCTION RXC_INIT_ACTION_ITEM

DESCRIPTION
  This function initializes the RXC Action Item Free Queue.

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None

========================================================================*/
extern void rxc_init_action_item(void);

/*========================================================================

FUNCTION RXC_ATMR_INSERT_CMD

DESCRIPTION
  Insert a command which has action time associated in the Action Time
  Queue. This queue contains rxc commands from MC which are stored in
  the ascending order of their action times (time in units of 20ms frame
  time).

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

========================================================================*/
extern void rxc_atmr_insert_cmd(rxc_action_list_item_type *atmr_item_ptr);

/*=============================================================================
FUNCTION RXC_ATMR_ISR

DESCRIPTION
  This is the Action Timer System Interrupt. In this, we check the Action Item
  Queue to service the commands that need to be serviced before the next frame
  time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
extern void rxc_atmr_isr(void);

/*============================================================================
FUNCTION RXC_ATMR_DISPATCHER

DESCRIPTION
  This function invokes the callback function that have been registered
  based on the phase(0,1,2,3) of the system timeline interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
extern void rxc_atmr_dispatcher(void);

/*===========================================================================

FUNCTION RXC_MC_RPT

DESCRIPTION
  This function generates a report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_mc_rpt(mccrx_rpt_type *rpt_ptr);


/*===========================================================================

FUNCTION RXC_ISR_CMD

DESCRIPTION
  This procedure posts a command for the interrupt handler, continues to
  kick the watchdog while waiting for a response and returns the response.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_isr_cmd( rxc_isr_cmd_type *cmd_ptr );

/*===========================================================================

FUNCTION RXC_FLAG_ERASURE

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#ifdef FEATURE_IS95B_MAHHO
  extern void rxc_flag_erasure( boolean erasure_flag );
#endif /* FEATURE_IS95B_MAHHO */

/*===========================================================================

FUNCTION SEND_PWR_RPT

DESCRIPTION
  This function sends a power report to the Main Control task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void send_pwr_rpt( void );

/*===========================================================================

FUNCTION RXC_WAIT

DESCRIPTION
  This function continues to reset the Watchdog signal and timer while
  waiting for any signal in a specified signal mask to be set. If this
  procedure detects that any signal in the specified signal mask is set,
  it will return the entire mask of signals set in the task'stcb.

DEPENDENCIES
  None.

RETURN VALUE
  Mask of signals returned by the rex_wait call.

SIDE EFFECTS
  None.

===========================================================================*/
extern rex_sigs_type rxc_wait( rex_sigs_type wait_sigs );

/*========================================================================

FUNCTION RXC_CLEANUP_SCH_MUX

DESCRIPTION This function cleansup the array that is used to hold
            the SCH MUX frames

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
#ifdef FEATURE_DS_IS2000
  extern void rxc_cleanup_sch_mux( void );
#endif

/*===========================================================================

FUNCTION RXC_SER_ACCUM

DESCRIPTION
  This function accumulates the symbol error rate statistics.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_ser_accum( void );

/*===========================================================================
FUNCTION RXC_RESET_FRAME_COUNT

DESCRIPTION
  This function resets the frame counters of valid message reporting

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Some RXC global counters will be changed.

===========================================================================*/
extern void rxc_reset_frame_count( void );

/*==========================================================================
FUNCTION RXC_SC_ser_ACCUM

DESCRIPTION
  This function accumulates the status information for SYNC Channel.
  The status information includes CRC/Quality, Energy Metrics and SER

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void rxc_sc_ser_accum( void );

#endif /* RXCCOMMON_H */

