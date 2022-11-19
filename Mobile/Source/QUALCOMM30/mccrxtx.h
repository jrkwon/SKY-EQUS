#ifndef MCCRXTX_H
#define MCCRXTX_H
/*===========================================================================

                   R X T X   H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface between
  the Main Control CDMA subtask and the layer 2 (RXTX) task.

Copyright (c) 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header:   L:/src/asw/common/shared/cp/vcs/mccrxtx.h_v   1.0   Aug 11 2000 11:43:46   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/04/98   pms     Neared the variables to save ROM space.
07/01/92   jai     Create file.

===========================================================================*/

#include "comdef.h"
#include "rex.h"
#include "qw.h"
#include "queue.h"
#include "caii.h"
#include "cmd.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Commands to be sent from RXTX to MCC */
typedef enum
{
  MCCRXTX_MSG_F,       /* received message */
  MCCRXTX_ACK_TMO_F    /* Acknowledgement time-out */
} mccrxtx_name_type;

/* Return statuses from MCC */
typedef enum
{
  MCCRXTX_DONE_S,     /* Command processed successfully */
  MCCRXTX_BUSY_S,     /* Command is being processed */
  MCCRXTX_BADCMD_S,   /* Invalid command */
  MCCRXTX_BADPARM_S,  /* Bad Parameters */
  MCCRXTX_BADSEQ_S,   /* Command is out of sequence */
  MCCRXTX_MAX_S
}mccrxtx_status_type;
  
/* Header for all commands */
typedef struct 
{
  cmd_hdr_type        cmd_hdr;      /* command header */
  mccrxtx_name_type   command;      /* The command */
  mccrxtx_status_type  status;      /* Command status */
} mccrxtx_hdr_type;

/* Type for MCCRXTX_MSG_F command */
typedef struct
{
  mccrxtx_hdr_type  hdr;     /* header */
  word              chn;     /* Channel message was received on */
  union 
  {
    byte           sframe;     /* Subframe message was received in (only
                                  applicable for Sync Channel messages) */
    qword           frame;
  } frm;
  caii_rx_msg_type  msg;     /* translated message */
} mccrxtx_msg_type;

/* Type for MCCRXTX_ACK_TMO_F command */
typedef struct
{
  mccrxtx_hdr_type hdr;      /* command header */
}mccrxtx_ack_tmo_type;

/* Union of commands */
typedef union
{
  mccrxtx_hdr_type hdr;
  mccrxtx_msg_type msg;
  mccrxtx_ack_tmo_type ack_tmo;
}mccrxtx_cmd_type;

extern q_type mcc_rxtx_q;
extern q_type          mcc_rxtx_ack_q;
extern q_type mcc_rxtx_free_q;



/*===========================================================================

FUNCTION MCCRXTX_CMD

DESCRIPTION
  This function sends a command to the mcc subtask from the rxtx task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  mccrxtx_cmd_type is placed on the mcc_rxtx_q.

===========================================================================*/

extern void mccrxtx_cmd
(
  mccrxtx_cmd_type  *cmd_ptr   /* Pointer to MCCRXTX message buffer */
);


/*===========================================================================

FUNCTION MCCRXTX_INIT

DESCRIPTION
  This function initializes the queues between the Layer 2 task and the CDMA
  Main Control task.  It also places buffers on the free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mccrxtx_init( void );

#endif /* MCCRXTX_H */
