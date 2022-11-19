#ifndef RXC_H
#define RXC_H
/*===========================================================================

                R X C D M A   S U B T A S K    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the definitions necessary for the main
  task to interface with the RXC subtask.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/rxc.h_v   1.2.1.1   11 May 2001 16:00:50   jqiu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/11/01   jq      Added RXC_NO_FRAME.
05/04/01   lcc     Changed rxc_returned_type to include "included" field for
                   rev_pwr_cntl_delay.  Also made some minor restructuring.
02/05/01   bgc     Renamed RXC_RIFHHO_CFS_GPS_F.
11/29/00   jq      Moved defination of rxc_wake_type.slot out of BROADCAST_SMS.
10/04/00   jq      Added FTM FCH and SCH loopback feature
09/06/00   bgc     Added FEATURE_FACTORY_TESTMODE.
07/19/00   hrk     Moved member rev_pwr_cntl_delay in rxc_tci_cmd_type out of
                   FEATURE_FFPC.
07/17/00   va      Added support for rev pwr cntl delay.
07/14/00   rm      Added suuport for multiframe interleaving. 
06/05/00   fc      Added rxc_olr_cmd_type.
06/02/00   lad     Changed order of rxc_frame_type enum for logging.
05/31/00   fc      Added command RXC_OLR_F.
05/26/00   jq      Added support for the new rxc design to support BCSMS
05/18/00   et      added support for dtx in sch
04/24/00   hrk     Added new transaction processing support.
04/12/00   hrk     Added prototype for rxc_send_rlp_frames_log().
04/10/00   dgy     Removed FEATURE_IS2000_QPCH.
04/07/00   va      Added RXC_RF_TUNE_F (HHO fix from PP)
03/27/00   sr      Changed the type of scc_energy[] from word to int2
03/24/00   bgc     Added enable_skip_tci in tci_cmd_type for PLT.
03/24/00   na      Added prototype for rxc_init_sch_mux().
03/16/00   na      Included prototypes for rxc_get_sch_info().
02/02/00   hrk     Merged with version GV40401 rxc.h from ASW archive:
           dna     Added timeout and is_hho as payload to the
                   RXC_RIF_HHO_OR_CFS_F command.
           aaj     Added FEATURE_DETECT_SUP_MUTING for MDR sup muting detection
01/31/00   rm      Clean up TEMP_HACKS_FOR_TIGER.
01/24/00   rm      Added in support for ACTION TIME processing.
12/17/99   va      Added transaction_num to rxc_pwr_ctrl_cmd_type,
                   Removed return_from_hho from rxc_returned_type interface.
12/13/99   hrk     Introduced new RXC-FFPC interface. Older implementation
                   of FFPC is featurized to FEATURE_FFPC_OLD
12/01/99   hrk     SOme FFPC parameters expanded to quadword range.
11/22/99   va      Expanded the following MC-RXC interfaces for the IS2000
                   support:
                   RXC_TCI_F
                   RXC_SO_F
                   RXC_RETURNED_F
                   RXC_HO_F (new)
                   RXC_PWR_CTRL_F (new)
                   RXC_FPC_SUBCHAN_GAIN_F (new)
                   The old RXC_HO_F has been replaced by RXC_HHO_BEGIN_F.
11/09/99   rm      Added decode type as part of RXC_SCH command to distinguish
                   between Convolutional / Turbo coding.
11/04/99   lcc     Corrected FFPC featurization in txc_tci_cmd_type.
                   Merged from GD4021:
           kmp     Mainlained the FEATURE_ENHANCED_STANDBY_II feature by
                   removing the #ifdef around the rxc_load_pc_isr() prototype.
           ry      Added extern declaration for rxc_flag_good_frame()
10/27/99   ajn     Added QPCH support
10/21/99   rm      Added new command interface RXC_SCCH_F to support MDR
                   for MSM5000.
10/07/99   lcc     Merged from PLT 2.0:
           jcw     Added support for F3 message control for PLT demod
           hrk     Added support for Outer Loop on F-FCH
           rm      Added new Interface for SCH.
09/17/99   lcc     Merged in new interface definitions for IS2000 and MSM5000
                   support from PLT archive:
                   1. Fixed problem in tci struct when building for MSM3000
                   2. Removed T_MSM5000PLT.
                   3. Made changes in TCI command to add RC parameter for
                      IS2000. Also  changes dec_tc_type to dec_fch_type.
08/26/99   aaj     Merged Panther Branch 3.1 to Mainline
08/25/99   aaj     Added new definition for sup channel SER threshold for RS2
08/09/99   ry      Combined FEATURE_IS95B_HARD_HO and FEATURE_IS95B_CFS into
                   FEATURE_IS95B_MAHHO
07/09/99   ry      Added RXC_RIF_HHO_OR_CFS_F and RXC_RETURNED_F
06/17/99   kmp     Merged in the one change listed below.
06/07/99   ks      Added rxc_pc_fade_type enum for various fade types.
04/06/99   aaj     Removed functions prototypes for RXDSP clock changes.
                   Added function prototype for rxc_voc_xchg()
02/26/99   ejv     Added prototype for rxc_load_pc_isr, part of FEATURE_
                   ENHANCED_STANDBY_II.
02/03/99   aaj     Added protoype declarations for RXDSP clk change routines
01/15/99   aaj     Merged MDR branch to Panther mainline
11/20/98   aaj     Added support for Medium (High) data rate sup channels
11/03/98   abh     Changes for frame no. correction after frame offset change.
09/17/98   bgc     Added rxc_set_frame_offset().
08/31/98   pms     Added Tan's Pascal calling convention code for ROM savings
09/06/96   gb      Added dec_date rate determination field to dec buffer type.
04/09/96   rdh     Added funct to return whether or not we're in Paging slot.
03/06/96   jca     Added RXC_PWR_REP_DELAY_F to rxc_cmd_name_type.
12/12/95   jca     Changed (there is no) rx_cmd_q to rxc_cmd_q.
08/10/95   gb      Added New Markov Service Options and made all service
                     options CAI_SO_*.
07/26/95   rah     Added rxc_reset_voc_init call.
07/13/95   gb      Changed mux option initialization.
06/14/95   gb      Added support for Rate set 2 service options.
08/09/94   jca     Added support for IS96/IS96A dual-capability vocoder.
03/24/94   gb      Added called interface to set isr.
06/24/92   jai     First Revision
10/10/90   rrd     created file

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "qw.h"
#include "queue.h"
#include "dec.h"
#include "deint.h"
#include "cmd.h"

#ifdef FEATURE_IS2000
#include "cai.h"
#include "mctrans.h"
#endif

#if  defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)
#include "mdrrlp.h"
#endif

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
extern boolean rxc_sup_dec_int_enable;
extern boolean rxc_turbo_dec_enabled;


typedef enum {
  RXC_FULL_FRAME    = 0,
  RXC_DTX_FRAME     = 1,
  RXC_ERASURE_FRAME = 2,
  RXC_NO_FRAME      = 3
} rxc_frame_type;

#ifdef FEATURE_RXC_ACTION_TIME_PROCESSING
 /* Possible values for requesting an interrupt in one of the 4 bins
    in a 20ms frame */

typedef enum {
  RXC_ATMR_5MS_FRAME_1 = 0 ,
  RXC_ATMR_5MS_FRAME_2,
  RXC_ATMR_5MS_FRAME_3,
  RXC_ATMR_5MS_FRAME_4
} rxc_atmr_5ms_enum_type;
#endif /* FEATURE_RXC_ACTION_TIME_PROCESSING */




/* Possible status value for a command */
typedef enum {
  RXC_DONE_S,         /* Generic OK return */
  RXC_BUSY_S,         /* Command is being processed */
  RXC_BADCMD_S,       /* Invalid command */
  RXC_BADPARM_S,      /* Bad Parameters */
  RXC_BADSEQ_S,       /* Command is out of sequence */
  RXC_NOLOCK_S,       /* Couldn't tune synthesizer */
  RXC_MAX_S           /* Number of status codes -- MUST be last entry */
} rxc_status_type;

/*  Possible commands to the RXC subtask                  */
typedef enum
{
  RXC_CDMA_F,          /* Switch to CDMA mode */
  RXC_SC_F,            /* Sync Channel */
  RXC_PC_F,            /* Paging Channel */
  RXC_TCI_F,           /* Traffic Channel initialization processing */
  RXC_TC_F,            /* Traffic Channel */
#ifdef T_MSM5000
  RXC_SCH_F,            /* IS95C Supplemental Channel */
#endif /* T_MSM5000 */
#ifdef T_MSM5000
#ifdef FEATURE_IS95B_MDR
  RXC_SCCH_F,          /* IS95B Supplemental Code Channel */
#endif                 /* FEATURE_IS95B_MDR */
#endif                 /* T_MSM5000 */
  RXC_EXIT_F,          /* Exit to main RX task */
  RXC_VOX_ON_F,        /* Turn on vocoder vox */
  RXC_VOX_OFF_F,       /* Turn off vocoder vox */
  RXC_PWR_F,           /* Change power measurement parameters */
  RXC_ERASE_F,         /* Treat blank and burst as erasures */
  RXC_SO_F,            /* Change service options */
  RXC_RESET_F,         /* Reset fade timer */
  RXC_PWR_ON_F,        /* Resume power control reporting */
  RXC_PWR_RESET_F,     /* Reinitialize power control reporting counters */
  RXC_PWR_REP_DELAY_F, /* Commence PWR_REP_DELAY countdown */
  #ifdef FEATURE_IS2000
  RXC_HHO_BEGIN_F,     /* Perform hard handoff processing, Replaces old RXC_HO_F
                          which is now used for sending HO indication to RXC */
  #else
  RXC_HO_F,            /* Same as HHO_BEGIN_F in the new interface */
  #endif
  RXC_SLOT_F,          /* Check for a valid message to be received in 2 slots*/
  RXC_SLEEP_F,         /* Perform pre-sleep processing */
  RXC_ACC_FADE_F,      /* Use Access Channel fade timer */
  RXC_IDL_FADE_F,      /* Use Idle fade timer */
  RXC_WAKE_F,          /* Perform wakeup processing */
  RXC_IDLE_HO_F,        /* Perform idle handoff processing */
#ifdef FEATURE_IS95B_MAHHO
  RXC_TUNE_RF_F,       /* About to tune to TF/CF - start ignoring good frame */
  RXC_RIFHHO_CFS_GPS_F, /* Perform Return-If-Fail HHO or CFS */
  RXC_RETURNED_F,       /* Return to the SF for a failed Return-If-Fail HHO */
#endif /* FEATURE_IS95B_MAHHO */
#ifdef FEATURE_IS2000
  RXC_PWR_CTRL_F,         /* Change to the new fwd power control parameters */
  RXC_HO_F,               /* Indicates a HO */
  RXC_FPC_SUBCHAN_GAIN_F, /* Change fpc sun chan gain value */
  RXC_RELEASE_F,          /* Equivalent to sending a NULL SO to RXC */
  RXC_MC_TRANS_F,         /* Transaction request from MC */
  RXC_OLR_F,              /* Outer Loop Report request from MC */
#endif
#ifdef FEATURE_FACTORY_TESTMODE
  RXC_FTM_FCH_LOOPBACK,     /* FCH loopback */
  RXC_FTM_SCH_LOOPBACK,     /* SCH loopback */
#endif
  RXC_COMMAND_COUNT     /* End of RXC commands */
} rxc_cmd_name_type;

/* Types for channels */
typedef enum
{
  RXC_CDMA_SC,  /* Message received on CDMA Sync Channel */
  RXC_CDMA_PC,  /* Message received on CDMA Paging Channel */
  RXC_CDMA_TC   /* Message received on CDMA Traffic Channel */
} rxc_chan_type;


/* States of RXC state machine */
typedef enum
{
  RXC_CDMA_STATE,
  RXC_SC_STATE,
  RXC_PC_STATE,
  RXC_TCI_STATE,
  RXC_TC_STATE,
  RXC_EXIT_STATE
}rxc_state_type;

#ifdef FEATURE_IS95B
/* -----------------------------------------------------
** This represents the state of the mobile while it
** it is monitoring the Paging channel for Fade reports.
** ----------------------------------------------------- */
typedef enum
{
  CDMA_IDLE_FADE,
  CDMA_ACCESS_FADE
} rxc_pc_fade_type;
#endif /* FEATURE_IS95B */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* This is the big one - All the possible commands, unionized */

/* The common header for all the commands */
typedef struct {
    cmd_hdr_type       cmd_hdr;      /* command header - includes q_link */
    rxc_cmd_name_type  command;      /* The command */
    rxc_status_type    status;       /* Space for message status */
} rxc_hdr_type;

typedef struct {                  /* RXC_CDMA_F */
  rxc_hdr_type hdr;
} rxc_cdma_cmd_type;

typedef struct {          /* RXC_SC_F */
  rxc_hdr_type hdr;
} rxc_sc_cmd_type;

typedef struct {          /* RXC_PC_F */
  rxc_hdr_type hdr;
  word rate;              /* Paging Channel data rate */
  qword frame;            /* Frame number for jump to Hyperspace */
} rxc_pc_cmd_type;

#if defined (FEATURE_FFPC) || defined (FEATURE_FFPC_OLD)
typedef struct {
  byte target_fer;
  byte min_setpt;
  byte max_setpt;
} rxc_fpc_olc_param_type;
#endif /* FEATURE_FFPC || FEATURE_FFPC_OLD */

#ifdef FEATURE_FFPC_OLD
#error code not present
#endif /* FEATURE_FFPC_OLD */

typedef struct {          /* RXC_TCI_F */
  rxc_hdr_type hdr;
  #ifndef FEATURE_IS2000
  dec_rate_set_type rate_set;     /* initial rate set to use */
  #else
  boolean fch_incl; /* Set to 1 if fundamental channel is allocated */
  cai_radio_config_type fwd_fch_rc; /* RC of fwd FCH, valid only if fch_incl =1 */
  boolean fpc_incl; /* Indicates if Fwd Pwr Ctrl parameters are included here */
  cai_fpc_pri_chan_type fpc_pri_chan; /* FPC primary channel 0-FCH 1-DCCH, validif fpc_incl is 1*/
  byte fpc_subchan_gain; /* fpc subchan gain , valid only if fpc_incl = 1 */
  #if defined (FEATURE_FFPC) || defined (FEATURE_FFPC_OLD)
  rxc_fpc_olc_param_type fch_olc_params; /* Outer loop params for FPC on FCH
                             Valid only if fch_incl and fpc_incl is set to 1 */
  byte fch_init_setpt; /* Valid only if fch_incl and fpc_incl are 1 */
  /* Following 2 members temporarily included here per Harsha */
  boolean ilpc_enabled;
  boolean olpc_enabled;
  #endif /* (FEATURE_FFPC) || (FEATURE_FFPC_OLD) */

  byte rev_pwr_cntl_delay; /* Rev FCH Pwr Ctrl delay */

  #endif /* FEATURE_IS2000 */
  #ifdef FEATURE_PLT
#error code not present
  #endif
  #ifdef FEATURE_FACTORY_TESTMODE
  boolean enable_skip_tci;   /* disables check for good frames in TCI */
  #endif
} rxc_tci_cmd_type;

typedef struct {          /* RXC_TC_F */
  rxc_hdr_type hdr;
} rxc_tc_cmd_type;

#ifdef T_MSM5000
typedef struct {        /* RXC_SCH_F */
  rxc_hdr_type hdr;
  deint_rc_type rc;
  dec_sch_rate_type sch_rate;     /* 2X, 4X, 8X, 16X */
  boolean double_size_pdu;   /* Indicates if MUX PDU (Type 3) is single
                               or double sized */
  dec_coding_type code_type;
#ifdef FEATURE_MULTIFRAME_INTERLEAVING
#error code not present
#endif  /* FEATURE_MULTIFRAME_INTERLEAVING */
} rxc_sch_cmd_type;
#endif /* T_MSM5000 */

#ifdef FEATURE_IS2000
typedef struct
{
  rxc_hdr_type      hdr;  /* Common header */
  mctrans_cmd_type  cmd;  /* Transaction command from MC */
} rxc_mc_trans_cmd_type;
#endif // FEATURE_IS2000

#ifdef T_MSM5000
#ifdef FEATURE_IS95B_MDR
typedef struct         /* RXC_SCCH_F */
{
  rxc_hdr_type hdr;
  byte num_sup;
  /* Number of Supplemental Code Channels */
} rxc_scch_cmd_type;

#endif  /* FEATURE_IS95B_MDR */
#endif  /* T_MSM5000 */

typedef struct {          /* RXC_EXIT_F */
  rxc_hdr_type hdr;
} rxc_exit_cmd_type;

typedef struct {          /* RXC_VOX_ON_F */
  rxc_hdr_type hdr;
} rxc_vox_on_cmd_type;

typedef struct {          /* RXC_VOX_OFF_F */
  rxc_hdr_type hdr;
} rxc_vox_off_cmd_type;

typedef struct {          /* RXC_PWR_F */
  rxc_hdr_type hdr;
  boolean pwr_thresh_enable;
    /* Threshold report mode indicator */
  boolean pwr_period_enable;
    /* Periodic report mode indicator */
  byte pwr_rep_thresh;
    /* Power control reporting threshold */
  word pwr_rep_frames;
    /* Power control reporting frame count */
  word pwr_rep_delay;
    /* Power report delay */
} rxc_pwr_cmd_type;

typedef struct {          /* RXC_PWR_ON_F */
  rxc_hdr_type hdr;
  boolean pwr_thresh_enable;
    /* Threshold report mode indicator */
  boolean pwr_period_enable;
    /* Periodic report mode indicator */
} rxc_pwr_on_cmd_type;

typedef struct {          /* RXC_PWR_RESET_F */
  rxc_hdr_type hdr;
} rxc_pwr_reset_cmd_type;

typedef struct {          /* RXC_PWR_REP_DELAY_F */
  rxc_hdr_type hdr;
  boolean pwr_thresh_enable;
    /* Threshold report mode indicator */
  boolean pwr_period_enable;
    /* Periodic report mode indicator */
} rxc_pwr_rep_delay_cmd_type;

typedef struct {          /* RXC_ERASE_F */
  rxc_hdr_type hdr;
} rxc_erase_type;

#ifdef FEATURE_IS2000

typedef struct {
  byte fwd_sch_id;
  cai_radio_config_type fwd_sch_rc;
  word fwd_sch_mux_option;
  boolean fwd_sch_coding_type;
  byte fwd_sch_multi_fo;
} rxc_fwd_sch_cfg_info_type;

typedef struct {
  boolean change_pri;           /* Indicator whether to switch primary SOs */
  word pri;                     /* Primary SO to switch to */
  boolean change_sec;           /* Indicator whether to switch sec SO */
  word sec;                     /* Secondary SO to switch to */
  boolean fpc_incl;             /* Indicates if FPC params are included here */
  byte fpc_mode;                /* FPC mode for fast fwd pwr ctrl */
  cai_fpc_pri_chan_type fpc_pri_chan;         /* FPC pri channel 0=FCH and 1=dcch */
  boolean fch_fpc_incl;         /* Indicates if FPC params for FCh is included*/
                                /* valid only if fpc_incl is 1 */
  #if defined (FEATURE_FFPC) || defined (FEATURE_FFPC_OLD)
  rxc_fpc_olc_param_type fch_olc_params; /* Valid only if above is 1 */
  #endif
  boolean fch_incl;             /* Indicator for if FCH information is included */
  boolean fch_5ms_frame_allowed;/* Indicated if 5ms frames allowed on FCH */
                                /* Valid only if fch_incl is 1 */
  cai_radio_config_type fwd_fch_rc; /*Fwd RC of FCH, valid only if fch_incl=1 */
  //Fwd-supp information
  byte num_fwd_sch;
  rxc_fwd_sch_cfg_info_type fwd_sch_info[2]; /*Actual size is num_fwd_sch */
} rxc_scr_info_type;
#endif

typedef struct {          /* RXC_SO_F */
  rxc_hdr_type hdr;
  #ifndef FEATURE_IS2000
  boolean change_pri;     /* Indicator of whether to switch primary service
                             options */
  word pri;               /* Primary service option to switch to */
  boolean change_sec;     /* Indicator of whether to switch secondary service
                             options */
  word sec;               /* Secondary service option to switch to */
  dword  action_time;     /* frame time at which service option switch
                                     will take place */
  #else
  rxc_scr_info_type scr;
  qword action_time;
  byte transaction_num;
  #endif

} rxc_so_cmd_type;

typedef struct {          /* RXC_RESET_F */
  rxc_hdr_type hdr;
} rxc_reset_type;

#ifndef FEATURE_IS2000
typedef struct {          /* RXC_HO_F */
  rxc_hdr_type hdr;
} rxc_ho_type;
#endif

typedef struct {          /* RXC_SLEEP_F */
  rxc_hdr_type hdr;
} rxc_sleep_type;

typedef struct {          /* RXC_ACC_FADE_F */
  rxc_hdr_type hdr;
} rxc_acc_fade_type;

typedef struct {          /* RXC_IDL_FADE_F */
  rxc_hdr_type hdr;
} rxc_idl_fade_type;

typedef struct {          /* RXC_WAKE_F */
  rxc_hdr_type hdr;
  word idle_pgslot;       /* Hashed paging channel slot */
  word sci_mask;          /* Mask to get position-in-slot from pgslot */
  byte slot_mask;         /* indicate the type of slot */
  word slot;              /* the slot RXC is waking up to */
#ifdef FEATURE_BROADCAST_SMS
  boolean bcsms_rxd;      /* the scheduled bcsms received in the sched'd slot */
#endif /* FEATURE_BROADCAST_SMS */
} rxc_wake_type;

#ifdef FEATURE_BROADCAST_SMS
#define RXC_BC_PEEK_BOUNDRY 7
/* the number of frames left before RXC stop peeking into a BC SMS slot for the
 * beginning of a Data Burst Message */
#endif /* FEATURE_BROADCAST_SMS */

typedef struct {          /* RXC_IDLE_HO_F */
  rxc_hdr_type hdr;
} rxc_idle_ho_type;

#ifdef FEATURE_IS95B_MAHHO
typedef struct {          /* RXC_TUNE_RF_F */
  rxc_hdr_type hdr;
} rxc_tune_rf_type;

typedef struct {          /* RXC_RIF_HHO_OR_CFS_F */
  rxc_hdr_type hdr;
  boolean is_hho;         /* Flag if this is a Hard Handoff or CFS */
  byte timeout;           /* Timeout (TF_WAIT_TIME) in 80ms units */
} rxc_rif_hho_type;

typedef struct {          /* RXC_RETURNED_F */
  rxc_hdr_type hdr;
  #ifdef FEATURE_IS2000
  struct
  {
    boolean             included;
    rxc_scr_info_type   data;
  } scr;                  /* SCR to be restored */
  struct
  {
    boolean included;
    uint8   value;
  } fpc_subchan_gain,     /* FPC subchan gain */
    rev_pwr_cntl_delay;   /* Rev FCH Pwr Ctrl delay  to be restored */
  #endif
} rxc_returned_type;
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
typedef struct {
  rxc_hdr_type hdr;
  qword action_time;        /* action time when the changes have to take place */
  byte transaction_num;     /* Transaction num for this command */
  byte fpc_mode;            /* Fwd Pwr Ctrl Mode */
  cai_fpc_pri_chan_type fpc_pri_chan;     /* FPC primary channel, 0-fch,1-DCCH */
  boolean fpc_fch_incl;     /* Indicator if FPC for FCH is included */
  boolean master_sec_incl;  /* Is master chan included for sec inner loop FPC */
  byte fpc_sec_chan;        /* Secondary channel */
  byte num_sups;            /* Num of supplementals for which FPC info is given */
  #if defined (FEATURE_FFPC) || defined (FEATURE_FFPC_OLD)
  rxc_fpc_olc_param_type fch_olc_params; /* FCH FPC outer loop params */
  rxc_fpc_olc_param_type sup_olc_params[2]; /*Outer loop parameters for supp channels */
                            /* revisit */
  #endif
  byte sch_ids[2];          /* SCH_IDs to which the above outer loop params apply */
                            /* revisit */
  boolean fpc_thresh_incl;
  byte fpc_setpt_thresh;
  boolean fpc_thresh_sch_incl;
  boolean fpc_setpt_thresh_sch;

} rxc_pwr_ctrl_cmd_type;

typedef   struct
{
     byte sch_id;
     byte sch_init_setpt;
     byte sch_FER;
     byte sch_min_setpt;
     byte sch_max_setpt;
}  rxc_fpc_sch_params_type;

typedef struct
{
    byte for_sch_id;
    qword start_time;
     byte for_sch_duration;
    boolean for_sch_fer_rep;
    byte for_sch_rate; //This will eventually be an enum
}  rxc_sch_params_type;

typedef struct
{
     rxc_hdr_type hdr;
     byte transaction_num;
     byte num_for_schs_assigned;
     rxc_fpc_sch_params_type fwd_sch_params[2];  //Real size is same as above
     boolean fpc_incl;
     byte fpc_mode;
     boolean fpc_olpc_sch_master_incl;
     byte fpc_sec_chan;
     byte num_schs;
     rxc_sch_params_type  sch_params[2]; //Real size is as above
     boolean  fch_sch_setpt_thresh_incl;
     byte  fch_sch_setpt_thresh;
}  rxc_escam_cmd_type;

typedef struct
{
    rxc_hdr_type hdr;
    byte transaction_num; //Transaction num for this transaction
    qword action_time;    //Action time for this transaction
    boolean hho_ind;      //Indicates if this is a HHO (To wait for sync points)
    boolean rif_ind;      //Indicates if this Return on Failure HHO
    boolean reset_fpc;    // Indicates if FPC is to be reset after HO
    boolean fo_change_ind; // Indicates if FO has changed (so vocoder can be reset)
    byte frame_offset;
    boolean fpc_subchan_gain_incl;  //Indicates if fpc_subchan_gain is incl
    byte fpc_subchan_gain; //Units of 0.25 db
    boolean scr_incl;  // Indicates if SCR info is included
    rxc_scr_info_type scr;  // SCR info
} rxc_ho_cmd_type;

typedef struct
{
  rxc_hdr_type hdr;
  byte transaction_num;
  qword action_time;
  byte fpc_subchan_gain; // Units of 0.25 db
} rxc_fpc_subchan_gain_cmd_type;

typedef struct
{
  rxc_hdr_type hdr;
} rxc_hho_begin_cmd_type;

typedef struct
{
  rxc_hdr_type hdr;
} rxc_rel_cmd_type;

typedef struct
{
  rxc_hdr_type hdr;
} rxc_olr_cmd_type;
#endif

#ifdef FEATURE_FACTORY_TESTMODE
typedef struct {
  rxc_hdr_type      hdr;
  boolean           loopback_on;
} rxc_ftm_loopback_cmd_type;
#endif

/* And, the union of them all.... */
typedef union {
  rxc_hdr_type               hdr;
  rxc_cdma_cmd_type          cdma;
  rxc_sc_cmd_type            sc;
  rxc_pc_cmd_type            pc;
  rxc_tci_cmd_type           tci;
  rxc_tc_cmd_type            tc;
#ifdef T_MSM5000
  rxc_sch_cmd_type           sch;
#endif  /* T_MSM5000 */
#ifdef T_MSM5000
#ifdef FEATURE_IS95B_MDR
  rxc_scch_cmd_type          scch;
#endif  /* FEATURE_IS95B_MDR */
#endif  /* T_MSM5000 */
  rxc_exit_cmd_type          exit;
  rxc_vox_on_cmd_type        vox_on;
  rxc_vox_off_cmd_type       vox_off;
  rxc_pwr_cmd_type           pwr;
  rxc_pwr_on_cmd_type        pwr_on;
  rxc_pwr_reset_cmd_type     pwr_reset;
  rxc_pwr_rep_delay_cmd_type pwr_rep_delay;
  rxc_erase_type             erase;
  rxc_so_cmd_type            so;
  rxc_reset_type             reset;
  #ifndef FEATURE_IS2000
  rxc_ho_type                ho; //Old Interface to tell RXC to begin HHO
  #endif
  rxc_sleep_type             sleep;
  rxc_acc_fade_type          acc_fade;
  rxc_idl_fade_type          idl_fade;
  rxc_wake_type              wake;
  rxc_idle_ho_type           idle_ho;
  #ifdef FEATURE_IS95B_MAHHO
  rxc_tune_rf_type           tune_rf;
  rxc_rif_hho_type           rif_hho;
  rxc_returned_type          ret;
  #endif /* FEATURE_IS95B_MAHHO */
  #ifdef FEATURE_IS2000
  rxc_pwr_ctrl_cmd_type      pwr_ctrl;
  rxc_ho_cmd_type            ho;
  rxc_fpc_subchan_gain_cmd_type fpc_subchan;
  rxc_hho_begin_cmd_type     hho_begin; // Replaces the old rxc_ho_type command
  rxc_rel_cmd_type           rel;       //Equivalent to sending SO cmd with null SO
  rxc_mc_trans_cmd_type      mc_trans;  // Transaction command from MC
  rxc_olr_cmd_type           olr;       // Outer Loop Report command from MC
  #endif
#ifdef FEATURE_FACTORY_TESTMODE
  rxc_ftm_loopback_cmd_type loopback;
#endif
} rxc_cmd_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Decoder Signalling Data buffer types */

/* Sync Channel buffer type */
typedef byte rxc_dec_sc_type [ DEC_96_BITS / 8 ];

/* Paging Channel buffer type */
typedef struct
{
  dec_pc_type bits;
  qword frame;
} rxc_dec_pc_type;

/* ------------------------------------------------------------------------------------ */

#define RXC_TOTAL_TC ( DEC_192_BITS + DEC_96_BITS + DEC_48_BITS + DEC_24_BITS)

/* ------------------------------------------------------------------------------------ */
#if defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)

#define RXC_SCC_ERASURE_THRESHOLD       77      /* Same as for Fundamental channel
                                                   full rate data. see function
                                                   rxc_9600_dec_rate_sel() for details */

#define RXC_SCC_ERASURE_THRESHOLD_RS2   50

/* Due to better data rate the RS2 generally is less prone to Symbol errors. The
** other factor is that the chainback length (number of symbols viterbi decoder
** traces back) is 96 for RS2 whereas it is 64 for RS1 in >= MSM2.3. Hence
** RS2 SER threshold can be lower than RS1.
*/

typedef struct {
    byte num_sup;                          /* number of supplementals */
    byte scc_crc;                          /* CRC status for sup channels */
    byte scc_ser[DEC_MAX_SCC_NUM];         /* SER for supplemental channels */
#ifdef T_MSM5000
    int2 scc_energy[DEC_MAX_SCC_NUM];     /* Energy for supplemental channels */
#endif /* T_MSM5000 */
    word bits[DEC_SCC_SIZE_14400 * DEC_MAX_SCC_NUM ];  /* word buffer,  SCC channel data */
  } rxc_scc_buf_type;

/* Traffic Channel buffer type */
typedef struct
{
#ifdef T_MSM5000
  dec_fch_type bits;
#else
  dec_tc_type bits;
#endif
  qword frame;
  rxc_scc_buf_type scc;
} rxc_dec_tc_type;

#else /* defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR) */

/* Traffic Channel buffer type */
typedef struct
{
#ifdef T_MSM5000
  dec_fch_type bits;
#else
  dec_tc_type bits;
#endif
  qword frame;
} rxc_dec_tc_type;

#endif /* defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR) */

/* ------------------------------------------------------------------------------------ */

/* Type to be placed on rxc_dec_q */
typedef struct
{
  q_link_type      link;     /* queue link */
  word            state;     /* State of RXC state machine when bits were
                                received */
  word            dec_rate;  /* rate determination for this frame */
  union
  {
    dec_sc_type sc_buf;      /* Bits received on Sync Channel */
    rxc_dec_pc_type pc_buf;  /* Bits received on Paging Channel */
    rxc_dec_tc_type tc_buf;  /* Bits received on Traffic Channel */
  }buf;
} rxc_dec_type;

/*-------------------------------------------------------------------------*/
/* Type for sending storing symbol error rates */
#ifdef T_MSM5000
typedef struct
{
  dword frame_cnt;
  dword ser;
} rxc_sc_ser_type;

typedef rxc_sc_ser_type rxc_pc_ser_type;
#endif

typedef struct
{
  dword frame_cnt;  /* Total frame count */
  dword ser1;   /* Sum of all 192 bit frame symbol error counts */
  dword ser2;   /* Sum of all 96 bit frame symbol error counts */
  dword ser4;   /* Sum of all 48 bit frame symbol error counts */
  dword ser8;   /* Sum of all 24 bit frame symbol error counts */
} rxc_ser_type;

/* Type for sending crc counters */
typedef struct
{
  dword good_crcs;  /* number of messages received with good CRCs */
  dword bad_crcs;   /* number of messages received with bad CRCs */
}rxc_crc_cnt_type;


/* The RXC command queue is declared external here so that the RX task
** can check to make sure an item is on the queue before passing control
** to the RXC subtask. */

extern q_type rxc_cmd_q;   /* Command queue */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

extern void rxc_powerup_init( void );
  /* use this to perform powerup intialization */

extern void  rxc_cmd ( rxc_cmd_msg_type *cmd_ptr );
  /* use this to submit a command */

extern void  rxc_subtask( void );
  /* main rxc subtask */

extern word rxc_rpt_voc_rate(void);
  /* report current vocoder frame rate */

extern rxc_state_type rxc_get_state(void);
  /* report current state of Receive task state machine */

extern word rxc_get_mode(void);
  /* report current mode (voice or Markov) */

extern void rxc_markov_reset( void );
  /* reset Markov test statistics */

extern void rxc_ser_reset( void );
  /* reset ser statistics */

extern void rxc_ser_report ( rxc_ser_type * );
  /* report on the collected ser statistics */

extern void rxc_get_sc_crc( rxc_crc_cnt_type *crc_ptr);
  /* Get Sync Channel message crc counts */

extern void rxc_get_pc_crc( rxc_crc_cnt_type *crc_ptr);
  /* Get Paging Channel message crc counts */

extern qword *rxc_get_frame( void );
  /* Get receive task frame number */

extern void rxc_clear_isr (void);
  /* set the DEC_INT isr to NULL for now */

extern void rxc_set_pc_isr (void);
  /* setup the DEC_INT isr as the paging isr */

extern void rxc_reset_voc_init ( void );
  /* Reset the rxc_voc_init_done flag */

extern boolean rxc_get_slot_status( void );
  /* Get status of whether or not we are in our paging slot */

extern void rxc_set_frame_offset ( byte frame_offset );
  /* Set the frame offset for use in time correction calculation */

extern void rxc_voc_xchg (void);
  /* Writes the decoded over the air voice bits to the vocoder */

#if  defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)

extern rlp_rate_enum_type rxc_scc_rate_sel_rlp( rxc_dec_type *buf_ptr, byte sup_index );
  /* Returns RLP Variable. Returns if the sup channel is erasure or full rate frame */

#ifdef FEATURE_DETECT_SUP_MUTING
extern void rxc_sup_mute_init( void );
  /* initialize data indicator for each supplemental to 1 */

extern rlp_rate_enum_type rxc_sup_mute(rlp_rate_enum_type sup_status, byte si );
 /* This function executes a algorithm to detect muting of sup channels */
#endif /* FEATURE_DETECT_SUP_MUTING */

#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

/*===========================================================================

FUNCTION RXC_LOAD_PC_ISR

DESCRIPTION
  This externally-called function installs the PC ISR and controls
  whether or not the decoder should have its state metrics
  cleared until the first frame in the slot.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void rxc_load_pc_isr ( boolean blank_frame );

#ifdef FEATURE_IS95B_MAHHO
/* <EJECT> */
/*===========================================================================

FUNCTION RXC_FLAG_GOOD_FRAME

DESCRIPTION
  This function is called by SRCH during a Return-If-Fail HHO, after
  verifying none of the HHO failure conditions is met.  This is to
  inform RXC that it's okay to start processing good frame.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  RXC will start processing good frame

===========================================================================*/
extern void rxc_flag_good_frame( void );
#endif /* FEATURE_IS95B_MAHHO */

#ifdef T_MSM5000
/*========================================================================

FUNCTION RXC_POWER_COMB_CONFIG

DESCRIPTION This function configures the power control puncturing position
            and PUNCT_POS_SEL based on Radio Configuration

DEPENDENCIES Power control and puncturing must be enabled before.

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
extern void rxc_power_comb_config
(
  deint_rc_type rc
);

/*========================================================================

FUNCTION RXC_SET_MAC_RND

DESCRIPTION This function sets the MAC Engine Rounding factors based on the
             spreading Rate.

DEPENDENCIES MAC Engine should be enabled.

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
extern void rxc_set_mac_rnd
(
  deint_rc_type rc,
  dec_sch_rate_type sch_rate,
  dec_coding_type code_type

);
#endif /* T_MSM5000 */

#ifdef FEATURE_RXC_ACTION_TIME_PROCESSING
/*========================================================================
FUNCTION RXC_ATMR_SET

DESCRIPTION
 This function provides an interface to  register callback function to be
 triggered on one of the 4 5ms bins (0, 1, 2, 3) in a 20ms frame.

DEPENDENCIES
 atmr_get should have returned a valid handle for TIME_INT2.

RETURN VALUE
  None

SIDE EFFECTS
  rxc_atmr_maks gets updated.
=========================================================================*/


extern void rxc_atmr_set(
  void                    (*callback_func_ptr)(void),
  rxc_atmr_5ms_enum_type   which_5ms
);
#endif /* FEATURE_RXC_ACTION_TIME_PROCESSING */

#ifdef FEATURE_DS_IS2000
/*========================================================================

FUNCTION RXC_GET_SCH_INFO

DESCRIPTION This function returns current configuration information for
            the SCHs. This is used by the UI in the data display.

            PARAMETERS:
            sch_rate_ptr - points to the current SCH rate.
            double_ptr   - is TRUE when double sized PDUs are being used
                           is FALSE when single sized PDUs are being used

DEPENDENCIES None

RETURN VALUE TRUE:  If SCH are assigned.
             FALSE: If no SCH are assigned.

SIDE EFFECTS None

=========================================================================*/
boolean rxc_get_sch_info
(
  dec_sch_rate_type* sch_rate_ptr,
  boolean* double_ptr
);

/*========================================================================

FUNCTION RXC_INIT_SCH_MUX

DESCRIPTION This function initializes the array that will be used to hold
            the SCH Mux frames for RLP

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_init_sch_mux( void );

#endif  /* FEATURE_DS_IS2000 */

#if defined(FEATURE_RLP_LOGGING)
/*===========================================================================

FUNCTION RXC_SEND_RLP_FRAMES_LOG

DESCRIPTION
  Terminate and send the accumulated RLP partial frames log.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  A RLP partial frames log may be sent.

===========================================================================*/
void rxc_send_rlp_frames_log(void);

#endif /* FEATURE_RLP_LOGGING */

#ifdef FEATURE_IS2000

/*========================================================================

FUNCTION RXC_SERVICE_TRANS_CMD

DESCRIPTION
   This function services a group of transactions at action time

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

=========================================================================*/
void rxc_service_trans_cmd( rxc_cmd_msg_type *cmd_ptr );

#ifdef FEATURE_IS2000_F_SCH
/*========================================================================
FUNCTION RXC_FWD_SCH_TRANS_CMD

DESCRIPTION
   The fwd. SCH transaction callback

DEPENDENCIES
  None

SIDE EFFECTS
  None

RETURN VALUE
  None
========================================================================*/
void rxc_fwd_sch_trans_cmd( mctrans_action_packet_type *fwd_sch_packet );

#endif // FEATURE_IS2000_F_SCH

#endif // FEATURE_IS2000

/*========================================================================

FUNCTION RXC_DTX_DET_SCH

DESCRIPTION This function determines if the current frame is a DTX frame,
an erasure frame, or a full frame.

DEPENDENCIES None

RETURN VALUE enum, either RXC_FULL_FRAME, RXC_DTX_FRAME, or RXC_ERASURE_FRAME


SIDE EFFECTS None

=========================================================================*/
rxc_frame_type rxc_dtx_det_sch(dec_rc_type rc, boolean crc_pass);

#endif /* RXC_H */

