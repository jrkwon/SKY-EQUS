#ifndef RXTX_H
#define RXTX_H
/*===========================================================================

                   R X T X   H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface with
  the rxtx (layer 2) task.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/rxtx.h_v   1.0   Aug 11 2000 11:44:34   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/19/00   yll     Added support for GPS: RXTX_START_GPS_F
03/06/00   ry      Added IS2000 featurization
11/15/99   va      Added new interface for HO RXTX_HO_F with reset_l2 and 
                   encrypt mode as payload. 
10/02/98   pms     Backed out nearing of variables
10/02/98   pms     Neared variables to save ROM space
08/03/95   dna     Support for message encryption
12/22/93   jca     Modified for IS-95 Phase 1 upgrade.
07/23/92   jai     Modified file for LINT and linking.
07/09/92   jai     First DMSS release
09/11/91   jai     Created file. 

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "rex.h"
#include "queue.h"
#include "caii.h"
#include "cmd.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Signal definitions */
#define  RXTX_RPT_TIMER_SIG  0x0001
  /* This signal is set when the mc_rpt_timer expires. */

#define  RXTX_RX_Q_SIG  0x0002
  /* This signal is set by the Receive task when an item is place on
     the rxtx_rx_q */

#define  RXTX_MC_Q_SIG  0x0004
  /* This signal is set when an item is placed on the rxtx_mc_q */

#define  RXTX_FRAME_SIG 0x0008
  /* This signal is set by RX every frame during Paging and Traffic
     Channel processing */
 
#define RXTX_TX_CMD_SIG 0x0010
  /* This signal is set by the TX task after processing a commmand from 
     the RXTX task. */

/* Timer definitions */
extern rex_timer_type  rxtx_rpt_timer;

/* Queue definitions */
extern q_type          rxtx_rx_q;
extern q_type          rxtx_rx_free_q;
extern q_type          rxtx_mc_q;
extern q_type          rxtx_mc_free_q;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Definitions for communication between MC and RXTX                       */

/* Commands to be sent from MC to RXTX */
typedef enum
{
  RXTX_START_F,      /* Return to Startup state processing */
  RXTX_TC_F,         /* Process Traffic channel messages */
  RXTX_TC_MSG_F,     /* Send a Traffic channel message */
  RXTX_STOP_TX_F,    /* Perform actions necessary with transmitter off */
  RXTX_START_TX_F,   /* Perform actions necessary with transmitter on */
  RXTX_ENCRYPT_MODE_F, /* Set the encrypt mode */
#ifdef FEATURE_IS2000
  RXTX_HO_F,         /* Perform acknowledgement procedures reset */   
#else
  RXTX_TC_RESET_F,   /* Perform acknowledgement procedures reset */   
#endif /* FEATURE_IS2000 */
#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */
  RXTX_MAX_F         /* Number of rxtx cmd name types. must be last entry */
} rxtx_cmd_name_type;

/* Return statuses from RXTX */
typedef enum
{
  RXTX_DONE_S,     /* Command processed successfully */
  RXTX_BUSY_S,     /* Command is being processed */
  RXTX_BADCMD_S,   /* Invalid command */
  RXTX_BADPARM_S,  /* Bad Parameters */
  RXTX_BADSEQ_S,   /* Command is out of sequence */
  RXTX_MAX_S
} rxtx_status_type;

/* Header for all commands */
typedef struct 
{
  cmd_hdr_type       cmd_hdr;      /* command header */
  rxtx_cmd_name_type command;      /* The command */
  rxtx_status_type    status;      /* Space for message status */
} rxtx_hdr_type;

/* Type for RXTX_START_F command */
typedef struct
{
  rxtx_hdr_type hdr;
} rxtx_start_type;

/* Type for RXTX_TC_F command */
typedef struct
{
  rxtx_hdr_type hdr;
} rxtx_tc_type;

/* Type for RXTX_TC_MSG_F command */
typedef struct
{
  rxtx_hdr_type hdr;
  caii_tx_msg_type msg;
  boolean ack_req;
    /* Indicator of whether message must be acknowledged */
} rxtx_tc_msg_type;

/* Type for RX_AC_MSG_F command */
typedef struct
{
  rxtx_hdr_type hdr;
  caii_tx_msg_type msg;
} rxtx_ac_msg_type;

/* Type for RX_STOP_TX_F command */
typedef struct
{
  rxtx_hdr_type hdr;
} rxtx_stop_tx_type;

/* Type for RX_START_TX_F command */
typedef struct
{
  rxtx_hdr_type hdr;
} rxtx_start_tx_type;

/* Type for RX_ENCRYPT_MODE_F command */
typedef struct
{
  rxtx_hdr_type hdr;
  byte encrypt_mode;
} rxtx_encrypt_mode_type;

#ifdef FEATURE_IS2000
/* Type for sending parameters changed due to HO */
typedef struct
{
  rxtx_hdr_type hdr;
  boolean reset_l2;
  byte encrypt_mode;
} rxtx_ho_type;
#else
/* Type for RXTX_TC_RESET_F command */
typedef struct
{
  rxtx_hdr_type hdr;
} rxtx_tc_reset_type;
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/* Union of command types */
typedef union
{
  rxtx_hdr_type hdr;
  rxtx_start_type start;
  rxtx_tc_type tc;
  rxtx_tc_msg_type tc_msg;
  rxtx_ac_msg_type ac_msg;
  rxtx_stop_tx_type stop_tx;
  rxtx_start_tx_type start_tx;
  rxtx_encrypt_mode_type encrypt_mode;
#ifdef FEATURE_IS2000
  rxtx_ho_type ho;
#else
  rxtx_tc_reset_type tc_reset;
#endif /* FEATURE_IS2000 */
#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */
} rxtx_cmd_type;

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Definitions for communication from RX and to RXTX */
typedef struct
{
  q_link_type      link;     /* Queue links */
  q_type  *return_q_ptr;     /* queue to place buffer on when done */
  word              chn;     /* Channel message was received on */
  word           length;     /* Message size ( in bits ) */
  cai_rx_msg_type   msg;     /* CDMA message */
} rxtx_rx_msg_type;
  
/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXTX_TASK

DESCRIPTION
  This task performs layer 2 processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rxtx_task
(
  dword dummy
    /* Required for REX, ignore */
);

/* <EJECT> */
/*===========================================================================

FUNCTION RXTX_CMD

DESCRIPTION
  This function should be called to send a command to the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for Layer 2 task and signal will be set to indicate
  that the item has been enqueued.

===========================================================================*/

extern void rxtx_cmd( rxtx_cmd_type * );

#endif /* RXTX_H */

