#ifndef MCTRANS_CMD_H
#define MCTRANS_CMD_H
/*===========================================================================

          MAIN CONTROL TRANSACTION COMMAND MANAGER MANAGER

GENERAL DESCRIPTION
  This is the interface for the MC transaction command type only.  

DESCRIPTION
   This module has data structures to keep track of different transaction numbers
   and the transaction types they correspond to.

   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/mctrans_cmd.h_v   1.0   Aug 11 2000 11:44:20   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/17/00   va      Added support for CFS and gating actions. Changed num of max 
                   actions per transaction to 4.
06/22/00   bgc     Moved mctrans_cmd_type to its own header from mctrans.h
                   to simplify a circular include problem.
===========================================================================*/


#include "comdef.h"
#include "qw.h"

/*===========================================================================
                        SOME DEFINES
===========================================================================*/
#define MCTRANS_MAX_ACTION_IN_TRANS   4

// Actions that may be included in a transaction
typedef enum
{
  MCTRANS_FFPC_ACTION = 1,
  MCTRANS_RLGC_ACTION = 2,
  MCTRANS_F_SCH_ACTION = 4,
  MCTRANS_R_SCH_ACTION = 8,
  MCTRANS_HO_ACTION = 0x10,
  MCTRANS_SCR_ACTION = 0x20,
  MCTRANS_CFS_NSET_ACTION = 0x40,
  MCTRANS_CFS_CTRL_ACTION =0x80,
  MCTRANS_GATING_ACTION =0x100,
  MCTRANS_UNDEFINED_ACTION
} mctrans_action_type;

// Don't change the order of following servers
typedef enum
{
  MCTRANS_SERVER_RXC = 0,
  MCTRANS_SERVER_TXC,
  MCTRANS_SERVER_SRCH,
  MCTRANS_SERVER_COUNT,
  MCTRANS_SERVER_NULL
} mctrans_server_token_type;

// Structure to hold a transaction command.  This is despatched to a transaction
// server for processing
typedef struct
{
  uint8                     transaction_num;
  qword                     transaction_time;
  mctrans_server_token_type server_token;
  uint8                     num_actions;
  mctrans_action_type       actions[MCTRANS_MAX_ACTION_IN_TRANS];
} mctrans_cmd_type;

#endif /* MCTRANS_CMD_H */
