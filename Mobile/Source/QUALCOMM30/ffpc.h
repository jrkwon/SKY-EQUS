#ifndef FFPC_H
#define FFPC_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  FAST FORWARD POWER CONTROL PROCESSING

GENERAL DESCRIPTION
  This module is responsible for low level processing of data received
  from the base station in CDMA mode of operation for Fast Forward
  Power Control.

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/ffpc.h_v   1.2   21 Feb 2001 16:15:42   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/01   hrk     Renamed ffpc_convert_db_2_lin() to ffpc_sch_lin_setpt() and
                   changed prototype.
                   Renamed ffpc_lin_setpt() to ffpc_fch_lin_setpt()
11/13/00   hrk     Added curr_setpt_lin to ffpc_chan_info_type to track the
                   current linear setpoint.
                   Changed prototype for ffpc_log_frame_init(), ffpc_log_frame().
09/11/00   hrk     Changed prototype to fix errors indicated by Lint.
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE
06/20/00   hrk     Removed unnecessary include files.
06/15/00   bgc     Changed PLT to support fch and sch setpoint.
06/09/00   hrk     Removed inclusion of memory.h, log.h - not needed.
06/05/00   hrk     Added prototype for ffpc_program_walsh_gain().
06/05/00   hrk     Updated prototype definitions.
05/30/00   hrk     Added support for new MC-RXC transaction mechanism.
04/28/00   hrk     Added new enum fpc_mode_type.
04/25/00   hrk     Added FFPC support for SCH.
04/11/00   lad     Added prototypes for FFPC and PLT logging.
12/02/99   hrk     Initial revision.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "qw.h"
#include "rxc.h"
#ifdef T_MSM5000
#include "deint.h"
#endif  /* T_MSM5000 */

/* <EJECT> */
#ifdef FEATURE_FFPC

/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

typedef enum
{
 FPC_MODE_FCH_ONLY=0x0,
 FPC_MODE_FCH_400Hz_SCH_400Hz,
 FPC_MODE_FCH_200Hz_SCH_600Hz,
 FPC_MODE_EIB,
 FPC_MODE_FFPC_DISABLED
} fpc_mode_type;

typedef struct
{
  qword          curr_setpt_lin;
  byte           init_setpt_db;
  dword          curr_setpt_db;
  byte           fpc_subchan_gain_db;
  word           fpc_subchan_gain_lin;
  byte           min_setpt_db;
  byte           max_setpt_db;
  qword          min_setpt_lin;
  qword          max_setpt_lin;
  word           fer_down_adj_lin;
  byte           target_fer;
  word           fer_scaled;
  boolean        olpc_enabled;
  boolean        ilpc_enabled;
  word           hw_eb_no_setpt;
  deint_rc_type  fwd_rc;
  byte           incl_status;
} ffpc_chan_info_type;

typedef struct
{
  boolean   init_setpt_incl;
  byte      init_setpt;       /* valid if init_setpt_incl = 1 */
  boolean   fpc_mode_incl;
  byte      fpc_mode;
  boolean   fpc_subchan_gain_incl;
  byte      fpc_subchan_gain; /* valid if fpc_subchan_gain_incl = 1 */
  boolean   olpc_incl;
  byte      min_setpt;        /* valid if olpc_incl = 1 */
  byte      max_setpt;        /* valid if olpc_incl = 1 */
  byte      target_fer;       /* valid if olpc_incl = 1 */
  boolean   fwd_rc_incl;
  deint_rc_type  fwd_rc;      /* valid if fwd_rc_incl = 1 */
  boolean   sch_thresh_incl;
  uint8     sch_thresh;       /* valid only if sch_thresh_incl = 1 */
#ifdef FEATURE_FACTORY_TESTMODE
  boolean   ilpc_enabled;     /* inner loop enabled/disabled from PLT */
#endif /* FEATURE_FACTORY_TESTMODE */
} ffpc_params_type;

typedef enum
{
  FFPC_FCH=0x0,
  FFPC_DCCH,
  FFPC_SCH
} ffpc_chan_type;

typedef PACKED struct
{
  word pc_acc;
  word fpc_history;
  word rpc_history;
  word fch_fpc_setpt;
  boolean erasure;
  word erasure_cnt;
  word sch_fpc_setpt;
} plt_ffpc_log_type;

#define FFPC_RC_SCALING_FACTOR         8
#define FFPC_LINEAR_RESOLUTION_FACTOR  16
#define FFPC_LINEAR_SCALING_FACTOR \
          (FFPC_RC_SCALING_FACTOR + FFPC_LINEAR_RESOLUTION_FACTOR) 
#define FFPC_DB_SCALING_FACTOR         19
#define FFPC_LIN_SETPT_SATURATE_VALUE  0xFFFFFFFF
#define FFPC_HW_EB_NO_MAX_SETPT        0x7FFF

#define FFPC_DEFAULT_SUBCHAN_GAIN        12  /* 3 dB :units 0.25 dB  */

#define FFPC_DEFAULT_FCH_INIT_SETPT_VAL  40  /* 5 dB :units 0.125 dB */
#define FFPC_DEFAULT_FCH_MIN_SETPT_VAL   0   /* 0 dB :units 0.125 dB */
#define FFPC_DEFAULT_FCH_MAX_SETPT_VAL   80  /* 10 dB:units 0.125 dB */
#define FFPC_DEFAULT_FCH_TARGET_FER      2   /* 1 %                  */

#define FFPC_DEFAULT_SCH_INIT_SETPT_VAL  56  /* 7 dB :units 0.125 dB */
#define FFPC_DEFAULT_SCH_MIN_SETPT_VAL   16  /* 2 dB :units 0.125 dB */
#define FFPC_DEFAULT_SCH_MAX_SETPT_VAL   96  /* 12 dB:units 0.125 dB */
#define FFPC_DEFAULT_SCH_TARGET_FER      2   /* 1 %                  */
#define FFPC_DEFAULT_SCH_THRESH          255 /* set to a 8 bit max   */


#define FFPC_FWD_RC_M           0x01
#define FFPC_FWD_RC_INCL        0x01

#define FFPC_MODE_M             0x02
#define FFPC_MODE_INCL          0x02

#define FFPC_INIT_SETPT_M       0x04
#define FFPC_INIT_SETPT_INCL    0x04

#define FFPC_OLPC_M             0x08
#define FFPC_OLPC_INCL          0x08

#define FFPC_SUBCHAN_GAIN_M     0x10
#define FFPC_SUBCHAN_GAIN_INCL  0x10

/* <EJECT> */
/*===========================================================================

                      FUNCTION PROTOTYPES                                    

===========================================================================*/


void ffpc_init                   (ffpc_params_type *ffpc);
void ffpc_update_setpt           (rxc_frame_type frame_status,
                                  ffpc_chan_type);
word ffpc_scale_fer              (byte fer_bin);
dword ffpc_adj_db_setpt_good     (dword curr_setpt,
                                  word  fer,
                                  byte  min_setpt);
void ffpc_adj_lin_setpt_good     (qword curr_setpt,
                                  word  down_adj,
                                  qword min_setpt,
                                  qword new_setpt);
dword ffpc_adj_db_setpt_erasure  (dword curr_setpt, byte max_setpt);
void ffpc_adj_lin_setpt_erasure  (qword curr_setpt,
                                  qword max_setpt,
                                  qword new_setpt);
void ffpc_sch_lin_setpt          (byte          db_val,
                                  qword         scaled_val);
void ffpc_add_subchan_gain       (qword linear_val,
                                  byte  subchan_gain,
                                  qword scaled_val);
void ffpc_program_hw             (void);
void ffpc_program_fpc_mode       (fpc_mode_type);
word ffpc_get_hw_setpt           (qword);
void ffpc_program_setpt          (word, ffpc_chan_type);

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

void ffpc_process_scr            (rxc_scr_info_type *scr,
                                  ffpc_params_type  *ffpc);
void ffpc_tci_cmd                (rxc_cmd_msg_type *cmd_ptr);
void ffpc_ho_cmd                 (rxc_cmd_msg_type *cmd_ptr);
void ffpc_so_cmd                 (rxc_cmd_msg_type *cmd_ptr);
void ffpc_subchan_gain_cmd       (rxc_cmd_msg_type *cmd_ptr);
void ffpc_pwr_ctrl_cmd           (rxc_cmd_msg_type *cmd_ptr);
void ffpc_returned_failure_cmd   (rxc_cmd_msg_type *cmd_ptr);
void ffpc_check_setpt_thresh     (void);
void ffpc_send_olrm              (void);
void ffpc_disable                (void);
void ffpc_disable_sch_olpc       (void);
void ffpc_trans_cmd              (mctrans_action_packet_type *);
void ffpc_sch_cmd                (mctrans_ffpc_action_type *);
void ffpc_sch_init_params        (mctrans_ffpc_action_type *);
boolean ffpc_rc_changed          (deint_rc_type, deint_rc_type);
void ffpc_fch_init_params        (mctrans_ffpc_action_type *);
void ffpc_init_setpts            (mctrans_ffpc_action_type *, ffpc_chan_type);
void ffpc_init_curr_setpt        (qword, ffpc_chan_type);
void ffpc_init_outer_loop_setpt  (qword, ffpc_chan_type);
void ffpc_fch_lin_setpt          (byte, deint_rc_type, byte, qword);
void ffpc_init_outer_loop_params (mctrans_ffpc_action_type *, ffpc_chan_type);
void ffpc_test (void);
void ffpc_program_walsh_gain     (void);

/* Prototypes for logging functions. */
extern void ffpc_send_log        (void);
extern void ffpc_log_frame       (byte, word, word, boolean, boolean);

/* <EJECT> */

#endif /* FEATURE_FFPC */

#endif /* FFPC_H */

