#ifndef RLGC_H
#define RLGC_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  REVERSE LINK POWER CONTROL PROCESSING

GENERAL DESCRIPTION
  This module is responsible for low level processing of data received
  from the base station in CDMA mode of operation for Reverse Link
  Power Control.

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/rlgc.h_v   1.1   22 Nov 2000 15:04:06   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/22/00   hrk     Removed inclusion of err.h and msg.h
09/19/00   hrk     Added prototype for rpc_plt_get_rl_gain_adj (),
                   rpc_plt_get_fch_rate (), rpc_fch_gating_cmd().
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE.
08/24/00   hrk     Mainlined FEATURE_CS_ACTION_TIME_TRANS.
06/09/00   hrk     Initial revision.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "qw.h"
#include "rxc.h"
#include "txc.h"
#include "enci.h"
#include "enc.h"
//#ifdef T_MSM5000
#include "caii.h"
#include "deint.h"
//#endif  /* T_MSM5000 */

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm_cmd.h"
#endif

#ifdef FEATURE_RLGC

/*===========================================================================
                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

typedef struct
{
  int8  nom_att_gain;
  byte  pilot_ref_lvl;
} rl_att_gain_type;

/* 2x, 4x, 8x, 16x */
#define Max_Num_Of_High_Rates     4

/* 0 : Convolutional; 1 : Turbo */
#define Max_Num_Of_Coding_Types   2

/* First entri in rpc_gain_adj_tab corresponds to PCH gain of 30 */
#define TABLE_OFFSET    30

#define RPC_153_6_RATE  (ENC_153_6_RATE - ENC_19_2_RATE)
#define RPC_76_8_RATE   (ENC_76_8_RATE  - ENC_19_2_RATE)
#define RPC_38_4_RATE   (ENC_38_4_RATE  - ENC_19_2_RATE)
#define RPC_19_2_RATE   (ENC_19_2_RATE  - ENC_19_2_RATE)

/* <EJECT> */
/*===========================================================================

                      FUNCTION PROTOTYPES                                    

===========================================================================*/

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_COMPUTE_GAINS

DESCRIPTION
  Computes the relative gains for FCH, PCH, SCH 

DEPENDENCIES
  

RETURN VALUE
  Relative gains for FCH, PCH, SCH.

SIDE EFFECTS
  None.
===========================================================================*/
txc_gain_type
rpc_compute_gains
(
 int16    fch,
 boolean  sch_assigned,
 int16    sch
);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_CHAN_TO_PCH_GAIN

DESCRIPTION
  Performs a table lookup to get the linear value for the FCH/SCH to PCH
  gain.

DEPENDENCIES

RETURN VALUE
  Returns the linear value.

SIDE EFFECTS
  None.
===========================================================================*/
word
rpc_chan_to_pch_gain (int16  chan);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_SQRT32

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  Returns the Square Root.

SIDE EFFECTS
  None.
===========================================================================*/
dword
rpc_sqrt32 (dword val);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_MIN_GAIN

DESCRIPTION
  Finds out MIN { FCH_GAIN, PCH_GAIN, SCH_GAIN }

DEPENDENCIES

RETURN VALUE
  Returns the min value.

SIDE EFFECTS
  None.
===========================================================================*/
word
rpc_min_gain
(
  word     gp,
  word     gf,
  word     gs
);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_MAX_GAIN

DESCRIPTION
  Finds out MAX { FCH_GAIN, PCH_GAIN, SCH_GAIN }

DEPENDENCIES

RETURN VALUE
  Returns the max value.

SIDE EFFECTS
  None.
===========================================================================*/
word
rpc_max_gain
(
  word  gp,
  word  gf,
  word  gs
);

/* <EJECT> */
/*===========================================================================
 FUNCTION RPC_INIT_ATT_ADJ_GAIN

DESCRIPTION
  Initialzes Attribute Adjust Gains array for basic and high rates.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_init_att_adj_gain (void);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_INIT_CHAN_ADJ_GAIN

DESCRIPTION
  Initializes Cahnnel Adjust Gains for FCH, DCCH, SCH0, SCH1

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_init_chan_adj_gain (void);

/* <EJECT> */
/*===========================================================================
 FUNCTION RPC_MEAN_CODE_CHAN_OUTPUT_POWER

DESCRIPTION
  Computes the Mean Code Channel Output Power.

DEPENDENCIES
  This function should be called after RLGC parameters have been initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_mean_code_chan_output_power (void);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_TC_CMD

DESCRIPTION
  Performs initialization for Reverse Link Gain Control processing.

DEPENDENCIES
  Rate Set should be initialized in txc_so before invoking this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_tc_cmd (txc_cmd_type  *cmd_ptr);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_HO_CMD

DESCRIPTION
  Initialzes RLGC parameters specified in Handoff message.

DEPENDENCIES
  Rate Set should be initialized in txc_so before invoking this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_ho_cmd (txc_cmd_type *cmd_ptr);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_SO_CMD

DESCRIPTION
  Initialzes RLGC parameters specified in Service Connect message.

DEPENDENCIES
  Rate Set should be initialized in txc_so before invoking this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_so_cmd (txc_cmd_type *cmd_ptr);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_PWR_CTRL_CMD

DESCRIPTION
  Calls function RPC_INIT_PWR_CTRL_PARAM to initialize RLGC parameters
  specified in PCM message.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_pwr_ctrl_cmd (txc_cmd_type *cmd_ptr);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_GET_PCH_ADJ_GAIN

DESCRIPTION
  Returns the adjusted gain value for PCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
int16
rpc_get_pch_adj_gain (byte pch_gain);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_GET_RELATIVE_GAINS

DESCRIPTION
  Returns the adjusted gain value for PCH.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_get_relative_gains
(
  int16 *fch2pch,
  int16 *sch02pch,
  enc_rate_type rate,
  txc_chan_status_type chan_status
);

/* <EJECT> */
/*===========================================================================
FUNCTION RPC_INIT_PWR_CTRL_PARAMS

DESCRIPTION
  Calls function RPC_INIT_PWR_CTRL_PARAM to initialize RLGC parameters
  specified in PCM message.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_init_pwr_ctrl_params
(
  byte  num_rpc_recs,
  caii_rpc_type *rpc_rec
);

/* <EJECT> */
/*===========================================================================
 FUNCTION RPC_GET_FCH2PCH_GAIN_RATIO

DESCRIPTION
  This function computes the gain ratio between R-SCH and R-PICH.

DEPENDENCIES
  This function should be called after RLGC parameters have been initialized.

RETURN VALUE
  Returns the R-FCH to R-PICH gain ratio.

SIDE EFFECTS
  None.
===========================================================================*/
int2
rpc_get_fch2pch_gain_ratio
(
  byte rate,
  boolean fch_only
);

/* <EJECT> */
/*===========================================================================
 FUNCTION RPC_TRANS_CMD

DESCRIPTION
  This function process the MCTRANS_RLGC_ACTION_CMD from MC.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_trans_cmd (mctrans_action_packet_type *cmd_ptr);

/* <EJECT> */
/*===========================================================================
 FUNCTION RPC_GET_SCH2PCH_GAIN_RATIO

DESCRIPTION
  This function computes the gain ratio between R-SCH and R-PICH.

DEPENDENCIES
  This function should be called after RLGC parameters have been initialized.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
int2
rpc_get_sch2pch_gain_ratio
(
 enc_sch_rate_type rate,
 boolean code_type
);

/* <EJECT>^L */
/*===========================================================================
 FUNCTION RPC_FCH_GATING_MODE

DESCRIPTION
  If the R-FCH 1/8 Rate gating mode changes, this function re-calculates the
  gains.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void
rpc_fch_gating_cmd
(
 boolean rev_fch_gating_on
);

#ifdef FEATURE_PLT
#error code not present
#endif

#endif /* FEATURE_RLGC */

#endif /* RLGC_H */

