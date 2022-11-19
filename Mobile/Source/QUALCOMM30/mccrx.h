#ifndef MCCRX_H
#define MCCRX_H
/*===========================================================================

                M C C R X   H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface between
  the Main Control CDMA subtask and the Receive task.

Copyright (c) 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   L:/src/asw/common/shared/cp/vcs/mccrx.h_v   1.0   Aug 11 2000 11:43:44   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/05/00   fc      Modified mccrx_ol_type.
05/30/00   fc      Included the header file cai.h.
05/30/00   fc      Added RXC_OL_R report.
05/26/00   jq      Added cur_slot_mask into mccsrch_wakeup_rpt_type
04/24/00   lcc     Added SCH FER field to mccrx_pwr_type.
03/13/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ks      Removed FEATURE_ENHANCED_STANDBY_II definition.
01/26/00   va      Merged the GV40401 Changes
                   Added RXC_HHO_TIMEOUT_R report
12/20/99   va      Changed RX action time report to "RXC_AT_R" to make it
                   more general
11/15/99   va      IS2000 Changes.
                   Added RXC_SCR_AT_R report for RXC to inform when
                   action time for a SCR change has arrived.
06/17/99   kmp     Merged in the one change listed below.
06/07/99   ks      RXC_PC_T72M_TMO_R & RXC_VALID_PC_MSG_R two reports are
                   defined to support T72M/T40M timers processing.
02/26/99   ejv     Merged the fix for timestamp SLOT_R message.
09/04/98   pms     Neared the variables to save ROM space.
10/27/94   rdh     Removed slotted tmo warning message field.
10/24/94   rdh     Addition for PN autoswitch/error correction.
07/20/92   jai     Created file.

===========================================================================*/

#include "cmd.h"
#include "qw.h"
#ifdef FEATURE_IS2000
#include "cai.h"
#endif /* FEATURE_IS2000 */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* REPORTS from CDMA Receive task to CDMA Main Control task */

/* Report Codes */
typedef enum
{
  RXC_PC_TMO_R,  /* No valid message with valid CRC received in T30M secs */
#ifdef FEATURE_IS95B
  RXC_PC_T72M_TMO_R, /* No valid PC message with valid CRC in T72M secs */
  RXC_VALID_PC_MSG_R, /* Valid PC message after T72M expires */
#endif
  RXC_TX_OFF_R,  /* Timeout on receiving valid frames occured on the
                   Forward Traffic Channel indicating transmitter should
                   be disabled */
  RXC_TX_ON_R,   /* Valid frames received which indicate that transmitter
                   should be re-enabled */
  RXC_TC_TMO_R,  /* All received traffic channel frames placed into
                   category 9 or 10  */
  RXC_VALID_R,   /* A Valid frame has been received on the Traffic Channel
                   during the Traffic Channel Initialization substate */
  RXC_PWR_R,     /* Power Measurement report */
  RXC_SLOT_R,    /* A valid message was received and in at least 2 slots */
  RXC_PWR_ON_R,   /* Resume FPC after CDMA to CDMA hard handoff */
#ifdef FEATURE_IS95B_MAHHO
  RXC_HHO_TIMEOUT_R, /* Return-on-failure hard handoff timed out, so return */
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_IS2000
  RXC_AT_R,   /* Action time for SCR has arrived */
  RXC_OL_R,   /* Outer Loop report */
#endif /* FEATURE_IS2000 */
  RXC_TOTAL_COMMANDS_R
}mccrx_rpt_name_type;

typedef struct {
    cmd_hdr_type        rpt_hdr;      /* report header - includes q_link */
    mccrx_rpt_name_type   rpt;          /* Report type */
} mccrx_rpt_hdr_type;

/* RXC_PC_TMO_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
}mccrx_pc_tmo_type;

/* RXC_TX_OFF_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
}mccrx_tx_off_type;

/* RXC_TX_ON_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
} mccrx_tx_on_type;

/* RXC_TC_TMO_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
} mccrx_tc_tmo_type;

/* RXC_VALID_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
}mccrx_valid_type;

/* RXC_PWR_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
  word            pwr_meas_frames;
  byte            errors_detected;
#ifdef FEATURE_IS2000_SCH
  boolean         sch_pwr_meas_included;
  byte            sch_id;
  word            sch_pwr_meas_frames;
  word            sch_errors_detected;
#endif // FEATURE_IS2000_SCH
} mccrx_pwr_type;

/* RXC_SLOT_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
  qword rpt_time;
  byte slot_mask;
} mccrx_slot_type;

/* RXC_PWR_ON_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
} mccrx_pwr_on_type;

#ifdef FEATURE_IS95B_MAHHO
/* RXC_HHO_TIMEOUT_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
} mccrx_hho_timeout_type;
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
/* RXC_SCR_AT_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
  byte trans_num;
} mccrx_at_type;

/* RXC_OL_R */
typedef struct
{
  mccrx_rpt_hdr_type hdr;
  boolean            fch_incl;            /* FCH included indicator */
  byte               fpc_fch_curr_setpt;  /* FCH outer loop Eb/Nt setpoint */
  boolean            dcch_incl;           /* DCCH included indicator */
  byte               fpc_dcch_curr_setpt; /* F-DCCH outer loop Eb/Nt setpoint */
  byte               num_sup;             /* Number of SCHs */
  struct 
  {
    boolean sch_id;                       /* SCH index */
    byte fpc_sch_curr_setpt;              /* SCH outer loop Eb/Nt setpoint */
  }                  sch[CAI_OUTER_RPT_MAX_SUP];              
} mccrx_ol_type;
#endif /* FEATURE_IS2000 */

/* Union of reports */
typedef union
{
  mccrx_rpt_hdr_type    hdr;
  mccrx_pc_tmo_type  pc_tmo;
  mccrx_tx_off_type  tx_off;
  mccrx_tx_on_type   tx_on;
  mccrx_tc_tmo_type  tc_tmo;
  mccrx_valid_type   valid;
  mccrx_pwr_type       pwr;
  mccrx_slot_type    slot;
  mccrx_pwr_on_type  pwr_on;
#ifdef FEATURE_IS95B_MAHHO
  mccrx_hho_timeout_type hho_timeout;
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_IS2000
  mccrx_at_type at;
  mccrx_ol_type ol;
#endif
}mccrx_rpt_type;

/* queue for free buffers */
extern q_type mcc_rx_free_q;

/* Queue for reports from RXC to MCC */
extern q_type mcc_rx_rpt_q;


/*===========================================================================

FUNCTION MCCRX_INIT

DESCRIPTION
  This function initializes the queues between the Receive task and the CDMA
  Main Control task.  It also places buffers on the free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mccrx_init( void );

/*===========================================================================

FUNCTION MCC_GET_CUR_MSG_SEQ

DESCRIPTION
  This function gets the current config message sequence number in use
  on this serving base station.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the current config_msg_seq.

SIDE EFFECTS
  None.

===========================================================================*/

extern word mcc_get_cur_msg_seq( void );

#endif /* MCCRX_H */

