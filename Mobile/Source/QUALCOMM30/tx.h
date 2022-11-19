#ifndef TX_H
#define TX_H

/*===========================================================================

          M A I N   T R A N S M I T   T A S K   D E F I N I T I O N S

DESCRIPTION
  This file contains global declarations and external references
  for the Main Transmit task.

Copyright (c) 1992,1993 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/tx.h_v   1.0   Aug 11 2000 11:44:46   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/11/93   jca     Added TXC_RF_TIMER_SIG.
07/15/92   jai     Deleted TXC_STOP_SIG (not used).
07/08/92   jca     Deleted references to TXCDMA sigs (prefix is now TXC).
07/07/92   arh     Initial check-in for DMSS
04/16/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*-------------------------------------------------------------------------*/
/*             ACP TRANSMIT SUBTASK (ACPTX) SIGNAL DECLARATIONS            */
/*-------------------------------------------------------------------------*/

/* Timer Signals                                                           */

#define ACPTX_DOGRPT_TIMER_SIG  0x0001  /* Watchdog report interval timer  */
#define ACPTX_CMD_TIMER_SIG     0x0002  /* Command processing timer        */
#define ACPTX_HANDOFF_TIMER_SIG 0x0004  /* Handoff timer                   */
#define ACPTX_ST_TIMER_SIG      0x0008  /* Ensures 500 ms gap between
                                             ST transmissions              */

/* Used by the FM Interface ISR to signal when a request for message
   transmission has been completed                                         */
#define ACPTX_TX_MSG_SIG        0x0010

/* ACPTX Subtask Command Queue Signal
   (NOTE! This sig def must differ from the TXC cmd queue sig def.)         */
#define ACPTX_CMD_Q_SIG         0x0020 /* Indicates new item on ACPTX CMD Q */

/* Currently not used                                                      */
#define ACPTX_RSVD_0040_SIG     0x0040
#define ACPTX_RSVD_0080_SIG     0x0080
#define ACPTX_RSVD_0100_SIG     0x0100
#define ACPTX_RSVD_0200_SIG     0x0200
#define ACPTX_RSVD_0400_SIG     0x0400
#define ACPTX_RSVD_0800_SIG     0x0800

/*-------------------------------------------------------------------------*/
/*             CDMA TRANSMIT SUBTASK (TXC) SIGNAL DECLARATIONS             */
/*-------------------------------------------------------------------------*/

#define  TXC_RPT_TIMER_SIG  0x0001
  /* This signal is set when the tx_rpt_timer expires. */

#define  TXC_CMD_Q_SIG      0x0002
  /* This signal is set when an item is put on the tx_cmd_q. */

#define  TXC_INT_SIG        0x0004
  /* This signal is used for signalling the Transmit Task from the Encoder 
     Interrupt Handler */

#define TXC_PN_ROLL_SIG     0x0008
  /* Lets the transmit task know that a PN roll interrupt has occured - set
     by the Search task and used when the transmit task has to perform a
     timing change before transmitting an access attempt */

#define TXC_INT_CMD_SIG     0x0010
  /* Lets the transmit ISRs tell the transmit tasks that they have 
     performed a command that was given to them. */

#define TXC_RF_TIMER_SIG    0x0020
  /* Signal associated with timer that allows for delay between turning on
     the transmit portion of the RF hardware and programming the transmit
     synthesizers. */

/* Currently not used                                                      */

#define TXC_RSVD_0040_SIG  0x0040
#define TXC_RSVD_0080_SIG  0x0080
#define TXC_RSVD_0100_SIG  0x0100
#define TXC_RSVD_0200_SIG  0x0200
#define TXC_RSVD_0400_SIG  0x0400

/*-------------------------------------------------------------------------*/
/*             MAIN TRANSMIT TASK SIGNAL DECLARATIONS                      */
/*-------------------------------------------------------------------------*/

#define TX_RPT_TIMER_SIG  0x0001 /* Watchdog report interval timer         */

/* Subtask Command Queue Signals                                           */

#if (TXC_CMD_Q_SIG == ACPTX_CMD_Q_SIG)
#error CMD_Q_SIGs can't be equal
#endif

#define TX_CDMA_CMD_Q_SIG   TXC_CMD_Q_SIG
/* Indicates new item on the TXC subtask command queue                     */
#define TX_ACP_CMD_Q_SIG    ACPTX_CMD_Q_SIG
/* Indicates new item on the ACPTX subtask command queue                   */


/* Currently not used                                                      */
#define TX_RSVD_0002_SIG  0x0002
#define TX_RSVD_0004_SIG  0x0004 
#define TX_RSVD_0008_SIG  0x0008 
#define TX_RSVD_0010_SIG  0x0010 
#define TX_RSVD_0040_SIG  0x0040
#define TX_RSVD_0080_SIG  0x0080
#define TX_RSVD_0100_SIG  0x0100 
#define TX_RSVD_0200_SIG  0x0200 
#define TX_RSVD_0400_SIG  0x0400 

/* Signals 0x1000, 0x2000, 0x4000, and 0x8000 are system wide common and
   are defined in TASK.H                                                   */

/* <EJECT> */
/*===========================================================================

                     PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TX_TASK

DESCRIPTION
  This procedure is the entrance procedure for the Main Transmit task.
  It contains the main processing loop for the Main Transmit task which
  controls the activation of the ACPTX and TXC subtasks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void tx_task (
  dword dummy
    /* Required for REX, ignore */
);

#endif

