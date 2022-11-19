#ifndef SRCHI_H
#define SRCHI_H
/*===========================================================================

                   S R C H I    H E A D E R    F I L E

DESCRIPTION
   This file contains definitions used internally by the Search task.
   It should NOT be included by any source file outside the Search task!

    Copyright (c) 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
                  2001
                    by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/srchi.h_v   1.53   03 May 2001 15:54:38   evanmell  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/03/01   ejv     Added macros ABS and LABS.
05/01/01   br      Added SRCH Scheduler support. Moved searcher slewing to
                   SRCH drivers. Added shoulder search mode support.
04/26/01   rdh     Changes for 8X searching in all states, code streamlining.
04/10/01   ejv     Externed new function srch_start_rand_buf_fill for FEATURE_DH.
03/29/01   ejv     Added FEATURE_IS2000_P2.
03/16/01   ejv     Removed ULPN mask changes for MSM5100.
03/14/01   ejv     Modified comment.
03/14/01   ejv     Removed FEATURE_IS2000_REL_A from finger structure.
03/12/01   dgy     Added a variable for FEATURE_IS2000_QPCH_CCI.
03/05/01   ejv     Removed #ifdef from around srch_rssi_timer.
03/02/01   br      Added initial support for gpsOne PPM on MSM5105
                   (FEATURE_MSM5105_PPM_SRCH_ENGINE).
02/28/01   br      Added FEATURE_SEARCH3 to featurize searcher 3 operation.
02/27/01   ejv     Added FEATURE_IS2000_REL_A.
02/27/01   ejv     Removed srch_reacq_mstr_posn_set.
02/16/01   dgy     Added FEATURE_IS2000_QPCH_CCI, the initial support for CCI.
02/14/01   ejv     Added changes for setting the ULPN mask on MSM5100.
02/14/01   rdh     Added initial support for System Determination 2.0.
02/07/01   dgy     Added a variable for PC framing type.
02/07/01   ejv     Removed temporary changes for MSM5100.
02/02/01   br      Added FEATURE_GPS_MODE to featurize GPS operation.
01/31/01   rm      Added FEATURE_HW_FING_MERGE_DETECT.
01/31/01   br      Renamed FEATURE_GPS_MODE into FEATURE_GPS_SRCH_ENGINE.
01/25/00   abh     Changes to read the GPS RF Loss calibration value from NV.
01/22/01   ejv     Added SEARCH1_DUMP and SEARCH2_DUMP macros.
01/22/01   ejv     Mainlined T_SLEEP.
01/18/01   ejv     Added MSM5100 support.
01/10/01   dgy     For FEATURE_UP_CLOCK_SWITCHING, used TCXO for the uP clock 
                   during the catnaps between PI1 and PI2 and between PI2 and 
                   the PC slot boundary in QPCH.
01/08/01   ejv     Added new sub-structures for channel parameters.
12/14/00   ejv     Added another parameter to srch_fing_cfg_qof().
12/12/00   ejv     Moved srch_restore_pch() for PLT only.
12/11/00   pj      Put srch_enable_moving_psd code inside FEATURE_GPSONE ifdefs,
                   rather than simply in FEATURE_IS95B* ifdefs.
12/07/00   bgc     Added srch_restore_pch().
11/22/00   dgy     Added support for catnaps between PI2 and PCH slot boundary.
11/21/00   bgc     Updated for PLT support.
11/06/00   ejv     Added new searcher driver interface.
11/03/00   br      Exchanged gpsOne function srch_master_fing_get_pos() with
                   srch_get_master_finger_data().
11/02/00   dgy     Added a variable and moved a definition to srchzz.c for 
                   FEATURE_IS2000_QPCH.
10/23/00   ejv     Externed srch_total_sleep_dur.
09/19/00   abh     Added support for using the RF Delay calibration of GPS Measurements
                   according to the values stored in NV.
09/18/00   ejv     Externed new function srch_trans_qpch_to_pc().
09/14/00   day     Added srch_gps_record_fing_comb_offset() prototype.
09/12/00   day     Move srchzz_enable_and_init_rf_digital() prototype outside
                   of FEATURE_IS2000_QPCH.
09/12/00   pj/abh  Added master_fing_get_pos() prototype.
09/11/00   ajn/dgy Added the QPCH timeline that wakes up before the first PI
           /ejv    bit, goes back to sleep immediately if any of the two PI 
                   bits is zero, and stays awake for PCH if both PI bits are 
                   non-zero. 
09/08/00   eh      Declared qpch channel estimator functions.
09/07/00   lc      Added function prototype for srchtc_process_sch_transaction().
09/06/00   eh      Declared srchzz_enable_and_init_rf_digital().
09/01/00   pj      Added new SRCH_GPS_SRCH_DONE macro.
08/21/00   ejv     Removed FEATURE_PILOT_SCANNER and SRCH_PRE_IS95B_BUILD.
08/06/00   abh/br  Added support for SRCH3 lost status dump and memset().
08/04/00   ejv     Externed srch_get_slot_status() and 2 new last slot qwords.
07/28/00   fc      Mainlined FEATURE_CS_ACTION_TIME_TRANS.
07/13/00   yll     Added "Tune Away Without Drop" capability for FEATURE_GPSONE.
06/21/00   abh     Added macro definition for GPS Srch Status Dump.
06/29/00   abh/br  Added MSM3300 support - FEATURE_GPSONE and FEATURE_SRCH_BATCH_LOG.
                   Changed local definitions from srchtc.c to global and externed 
                   srchtc.c function variables to support new features.
06/19/00   ejv     Externed srch_qpch_log_data.
06/13/00   ejv     Removed srch_enable/disable_demod_bypass from slow clock
                   ifdef.  Edited memcpy prototype to match memory.h/string.h.
06/12/00   lc/dgy  Added FEATURE_CS_ACTION_TIME_TRANS to streamline the processing of 
                   MC transactions and to support soft handoff transactions. 
06/12/00   ejv     Externed srch_fpc_enabled and added ttl_slow to fing struct.
06/09/00    eh     Added support for search window offset.
06/06/00   ejv     Changed int8 to int1.
05/26/00   ejv     Doubled ROLLS_PER_MAX_SLOT_CYCLE.  Externed new slot mask
                   and slot counting variables.  Added new macros for slot
                   mask checking/clearing.  Externed some ZZ functions
                   which are need in PC state. 
05/23/00    eh     Added srch_qpch_enabled.
05/15/00   rdh     Simplified and consolidated fing lock threshold variables.
05/11/00   rdh/wli Added improvements to power control symbol combiner handling.
04/24/00   dgy     Added full support for SCH, including handoff, Active set 
                   updating, and action time processing.
04/10/00   dgy/ejv Added FEATURE_IS2000_QPCH.
04/10/00   ejv     Added CHIPX8_PER_OFFSET and PILOT_PN_MASK.
03/16/00   ejv     Added ROLLS_PER_MAX_SLOT_CYCLE.
02/25/00   ejv     Changed #ifdef FEATURE_DIAG_STREAMING_OUTPUT to
                   #ifndef FEATURE_DIAG_V7_LOG_IFACE.
02/24/00    eh     Declared srch_enable_demod_bypass and
                   srch_disable_demod_bypass. 
02/14/00   rdh     Added FEATURE_FING_MERGE_DETECT.
02/11/00   ejv     Wrapped srch_bypass_interface in FEATURE_SLEEP_SLOW_CLOCK.
02/02/00   dgy     Added support for action time processing.
01/27/00   ejv     Added FEATURE_KEYPRESS_OPTIMIZATION.
01/19/00   ejv     Increased SRCH2_SW_Q_LEN to 42.
01/13/00   ejv     Externed srch_bypass_interface().
01/10/00   rdh     Removed window segmenting.
01/10/00   ajn     Re-Added FEATURE_ENHANCED_STANDBY_III & QUARTER_ROLL_TIMELINE
                   support.
12/21/99   rdh     Added FEATURE_ENHANCED_STANDBY_III.
12/13/99   ejv     Changes for FEATURE_SEARCH2.  Removed duplicate extern
                   of srch_freeze_iq() (it is also in srch.h now).
11/24/99   ajn     Finger "Change Flags" for Supplimental Channel support
                   TEMP_HACKS_FOR_TIGER removed
11/10/99   ejv     Added FEATURE_SEARCH2 and FEATURE_SEARCH2_IDLE.
11/08/99   dgy     Added function prototypes for srch_fing_cfg_qof() and
                   srch_reset_qof().
10/27/99   ajn     Added QPCH support
10/21/99   ejv     Changed srch_wait to use rex_sigs_type instead of words.
10/20/99   dgy     Added support for QOF.
10/07/99   lcc     Merged from PLT 2.0:
           ajn/rm  Added in support for SCH (for PLT). 
10/06/99   ejv     Removed some parameters from the srch_msm_config_struct_
                   type.  Removed use_max_inx from srch_sort_sect input list.
10/05/99   wjn     Exported variable srch_ho_to_oband for sparse AGC shipment
                   to DM before hard handoff to a different band class.
10/04/99   ejv     Merged FEATURE_DIAG_STREAMING_OUTPUT.
09/30/99   yus     IS95B code review cleanup.
09/07/99   ejv     Added prototype for function srch_freeze_iq.
08/25/99   wjn     Export srchpc_rssi_rpt and srchpc_sys_reselect_check() for
                   Analog System Reselection algorithm to be run in ZZ state.
08/23/99   ejv     Mainlined FEATURE_ENHANCED_STANDBY_II.  Removed prototype
                   for srch_sect_end.
08/13/99   ajn     Fixed the INCW_MAX and INCI_MAX macros.
08/12/99   ejv     Lint cleanup.
08/12/99   rm      Removed functions declarations relating to enabling 
                   I/Q Channels. New Macros have been defined in msm_drv.h.
08/12/99   yus     Made several modifications of the structures and command 
                   names under FEATURE_IS95B_MAHHO
08/12/99   yus     Changed FEATURE_IS95B_CFS and FEATURE_IS95B_HARD_HO to
                   FEATURE_IS95B_MAHHO.
08/06/99   ejv     yshi: Added FEATURE_IS95B_CFS support.  ajn: Mainlined
                   FEATURE_ENHANCED_STANDBY and FEATURE_VIRTUAL_COMBINER.
                   Removed pre-MSM3000 support.  Added initial Tiger2 support.
07/24/99   jcw     Merged in PLT changes.
07/23/99   wjn     Added the band class information in macro 
                   CURRENT_FREQ_NEIG(i) definition and in function prototypes
                   srch_sect_find() and srch_nset_dup().
07/20/99   yus     Added ota_win_n member in the sect structure. It's used for
                   saving the over-the-air window size.
07/20/99   yus     Defined WINDOW_SIZE_14. It's used for enable 8x mode if
                   neighbor window size larger than that (IS95B system only.)                   
07/19/99   ml      Added support for PLT.
07/01/99   yus     De-coupling FEATURE_SPECIAL_MDR with FEATURE_IS95B_PILOT_RPT
                   if SRCH_PRE_IS95B_BUILD is defined.
06/28/99   yus     Merged IS95B features to the ASW tip.
06/28/99   yus     Added #ifdef SRCH_PRE_IS95B_BUILD for pre-95b backward build.
06/24/99   dgy     Changed PCS AGC threshold to -100 dBm. Added a new define.
06/10/99   yus     Merged IS95B Hard Hardoff ( w/o Candidate Frequency Search.)
06/09/99   rdh     Merged FEATURE_PILOT_SCANNER.
06/08/99   wli     Removed slow clock code due to redesign. 
05/26/99   ejv     Removed prototype for srch_labs, which no longer exists.
05/25/99   ejv     Added srch_fing_disable and srch_fing_restart.
05/24/99   yus     Added IS95B Periodic PSMM feature.
05/03/99   ejv     Added raw_rssi to the fings struct to store the energy
                   needed for reporting Gen TA packet data.
03/22/99   kmp     Merged the changes listed below from the TGP IS-95B baseline.
                   03/10/99   yus     Added access_attempted to 
                                      srch_sect_struct_type. 
04/22/99   ejv     Added the definition of WIN_TAB_SIZ from srch.c.
04/08/99   ejv     Removed redundant variable declarations.
02/26/99   yus     Merged IS95B features to ARM based code( see below for the 
                   features.)
                   02/03/99   ks      Fixed the AEHO and AHO ifdef feature 
                                      definitions.
                   11/25/98   yus     Added P_REV checking.
                   11/25/98   yus     Added IS95B Neighbor search enhancements.
                   11/25/98   yus     Added IS95B Access handoff and Access 
                                      Entry handoff support.
                   11/25/98   yus     Added IS95B Pilot reporting.
                   11/25/98   yus     Changed #ifdef FEATURE_JSTD008 to run time 
                                      checking.
                   09/23/98   yus     Added IS95B soft handoff algorithm.
                   09/23/98   yus     Added IS95B system reselection in Idle 
                                      feature.
04/06/99   yus     Changed the value of MAX_NUM_SUP_CH_SUPPORT_IF_FOUR_FINGS 
                   to zero.
04/05/99   yus     Added function srch_4th_fing_enable for enabling/disabling
                   the fourth finger.
02/16/99   ejv     Added last_win_siz to srch_sect_struct_type.
02/03/99   ejv     yus: Merge updates for MDR features.
02/03/99   ejv     yus: Added IS95B MDR support.
02/03/99   ejv     Removed debug code, srch_sleep_roll_isr.
02/02/99   ejv     Changed T_VIRTUAL_COMBINER to FEATURE_VIRTUAL_COMBINER.
02/01/99   ejv     Added a number of defines and prototypes for rdh's
                   FEATURE_ENHANCED_STANDBY_II.
01/21/99   ejv     Edited FEATURE_DH - no longer uses DMA.
01/13/99   ejv     ajn: Added knowledge about time reference slamming to
                   srch_comb_arm_slam & added srch_get_vsc_time( )
01/06/99   ejv     Removed _cdecl and ROM - not necessary for ARM.
12/16/98   ejv     Merged code for SLOW_CLOCK.
10/16/98   ejv     Merged changes to support Panther.
10/08/98   ejv     Changed FEATURE_NEW_SLEEP_TIMELINE to
                   FEATURE_ENHANCED_STANDBY.
09/28/98   ajn     Avoid slew computation during finger slew problem.
09/28/98   ejv     Added _cdecl to force C calling to specific functions
                   for Pascal builds.
09/25/98   ejv     Changed prototype of srch_get_freq_est to handle an int2.
09/24/98   ejv     Added const to 2nd parameter in memcpy() to silence
                   compile warnings and match memory.h declaration.
08/24/98   ejv     Removed pilot scanner variables.
08/18/98   ejv     Removed srch_msm versions since hw.h versions are now used.
08/18/98   ejv     Added dh task support, must define FEATURE_DH.
08/18/98   ejv     Added rdh/bgc's FEATURE_DYNAMIC_THROTTLE to the governor.
07/01/98   ajn     Added FEATURE_NEW_SLEEP_TIMELINE macro.
05/13/98   ajn     Used a delayed timeline for MSM2.3 in paging/idle sleep 
                   mode.
04/21/98   mk      Added support for msm2310.
04/13/98   dgy     Include customer.h only if CUST_H is defined. Changed 
                   FING_CNT and added general TA packet and function to set 
                   power combiner threshold for MSM2300.
03/16/98   scdb    Merved CLOCK 2.0 changes. This removes the SRCH_TIMEKEEP_10
                   define.
03/04/98   yus/smo Added new ML eng_to_ecio tables for use in 8x searcher.
02/20/98   smo     Removed new clock support code via ifdef SRCH_TIMEKEEP_10.
                   This represents use of timekeeping VU clock01.00.00.
11/25/97   dgy/smo Changes to use new eng_to_ecio table pointer.
11/17/97   dgy/smo Support for enhanced acquisition algorithm, use of recent
                   temp array.
10/28/97   smo     Added support for msm2300, 8x searcher.
10/28/97   smo     Added support for msm2300, A4.
10/01/97   smo     Added msm2300 support, new srch_msm_version structure.
06/27/97   rdh     Added regional defines for Pilot Scanner.
06/19/97   rdh     Added T_Q, removed pre-NGP support, updated copyright.
05/20/97   rdh     Added Low Power Mode support.
03/21/97   smo     Added customer.h include for FEATUER_JSTD008
03/14/97   jjn     Added (TG==T_MD) to the appropriate conditional compile
                   statements for the Module
01/22/97   rdh     Added srch_get_stop_slew pulled from ZZ state.
12/12/96   smo     Increased SRCH_GOVERN_TIMEOUT to 16ms. Added constant
                   SRCH_GOVERN_ADJ_TIME for governer adjust periods.
11/22/96   rdh     Added Hutch's T_T target definitions.
11/05/96   smo     Increased CDMA_RSSI timeout period to 100ms.
10/22/96   rdh     More ROM savings to make 800 fit 4M flash.
09/26/96   rdh     Improved SC/PC thrash protection with dual-mode sys det.
09/19/96   rdh     Added unconditional PC state idle handoff throttling.
08/15/96   rdh     Added idle handoff delay timers/sigs.
07/11/96   smo     Added more debug information for priority scanning.
06/27/96   rdh     Changed AGC thresh to -90dBm on Systems recommendation.
05/20/96   smo     Added debug variables for JSTD-008 priority scanning
05/20/96   rdh     Added prototypes for sleep-related system time inc functions.
05/09/96   smo     Added JSTD-008 features and definitions.
05/07/96   rdh     Added T_COMP state machine vars in trpt structure.
05/01/96   rdh     Added sleep holdoff time, orig_pend flag for rescan-send.
04/09/96   rdh     Added "fast-revisit" struct members, pathlog proto. consts.
03/06/96   smo     Made RSSI timeout value 100ms for ISS2.
02/29/96   rdh     Added poke capability for thresholds after SRCH_ASET_F.
02/28/96   rdh     Got rid of extra searching Nset, changed over to tick26.
02/01/96   rdh     Added srch_expected_win_pos and srch_sect_stop prototype.
11/30/95   rdh     Added fade report support and internal Nset-keep feature.
11/03/95   rdh     Reduced Micro-acq AGC threshold to -100 from -104 dBm.
09/07/95   rdh     Added FILT_I4 for ZZ slow-open SCI fixes.
09/04/95   rdh     More pre-review cleanup of speed throttling constants.
08/23/95   rdh     Pre- code review cleanup checkin to save RAM/ROM.
07/21/95   rdh     ROM'd some large tables to save RAM space.
06/28/95   rdh     Changes to support fast mini/micro acq on Deep Sleep wakeup.
06/01/95   rdh     Mods for search driver break-up, CSS.
05/10/95   rdh     Added NGP-related interrupt handler declarations.
05/09/95   rdh     Added search speed throttling to allow all window sizes.
05/03/95   rdh     Added agc/pctl enabled flag field.
04/18/95   ras     Added T_I2 for ISS2 operation.
04/14/95   rdh     Added CDMA RSSI processing for all targets.
04/13/95   rdh     Gemini change: Use new sparse AGC/pctl logging packet.
04/06/95   rdh     Made lost dump recovery more responsive (faster).
03/31/95   rdh     Eliminated win_z variability as ZZ now spirals on reacq.
03/30/95   rdh     Merged in lost dump recovery timer code.
03/30/95   rdh     Elim slam bug catch - separated sym siz change from slam arm.
03/23/95   rdh     Added new srch_dz_state definitions for deep sleep.
01/10/95   rdh     Bug fix for PhsIII release - got rid of old "nset_received"
01/09/95   rdh     IS-95 Phase III release:
                    - Added segmented search max calc var. 
01/06/95   rdh     IS-95 Phase III upgrades:
                    - Mods for .pos, time reference rework.
                    - Added find_earliest_path, mods to find_earliest_fing.
                    - Made state and status code types consistent with calls.
                    - Added REX timer for handoff/slew throttling.
                    - PC state idle structure streamlining.
10/31/94   rdh     Added PC_ASSIGN needed parameters to idle structure.
10/24/94   rdh     Added PN mask autoswitching capability and slew state
                   MSM hardware error detection declarations.
09/06/94   rdh     Added finger struct referencing var and no path counter
                   in sect struct for individual window pilot centering.
08/04/94   rdh     Removal of annoying tabs/spaces.
07/30/94   rdh     Added IS-95 phase III upgrades.
04/05/94   jah     Added prototype for srch_find_earliest_finger().
03/01/94   jah     Changed cur_state's to srch_state_code_type.
01/06/94   jah     Changed watchdog iface to use dog_report().
12/09/93   jah     Added finger tracing externs under SRCH_FING_DEBUG define.
11/19/93   jah     Added extern for srch_rudely_awakened.
08/05/93   jah     Added prototype for srch_set_sym_siz()
07/26/93   ptw     Added SET_* definitions.
06/18/93   ptw     Added cell_type struct.
06/08/93   ptw     Added support for pwr_comb_ind bit (pci)
04/30/93   ptw     Added sleep stuff.
04/28/93   jai     Added routines to store and retrieve frequency estimates and
                   added freqeuncy estimate callback routine.
10/13/92   ptw     Added srch_cdma_freq variable.
10/12/92   ptw     Moved some regional declarations from srch.h
04/30/92   ptw     Removed FATAL macro (replaced by ERR_FATAL in msg.h)
01/22/92   ptw     Carved this module out of monolithic SRCH.C module.

===========================================================================*/

#include "dog.h"
#include "log.h"
#include "mccsrch.h"
#include "srch.h"
#include "target.h"
#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

#ifdef FEATURE_IS2000_QPCH
#include "mccqpch.h"
#endif /* FEATURE_IS2000_QPCH */

#ifdef FEATURE_IS2000_SCH
#include "mctrans.h"
#endif /* FEATURE_IS2000_SCH */


/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                           Search Task States
--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_START_STATE,     /* Raw initialization state. */
  SRCH_DZ_STATE,        /* Deep sleep in start state */
  SRCH_CDMA_STATE,      /* Initial state for CDMA operation */
  SRCH_ACQ_STATE,       /* CDMA acquisition of the pilot channel */
  SRCH_SC_STATE,        /* CDMA reception of the sync channel */
  SRCH_SLEW_STATE,      /* CDMA transition from sync to paging channel */
  SRCH_PC_STATE,        /* CDMA operation on the paging channel */
  SRCH_ZZ_STATE,        /* CDMA slotted mode sleep state */
  SRCH_TC_STATE,        /* CDMA operation on the traffic channel */
  SRCH_UNSLEW_STATE,    /* CDMA return from paging or traffic to sync channel */

  SRCH_MAX_STATE        /* Must remain last item in enum. */
}
srch_state_enum_type;



/*--------------------------------------------------------------------------
                          Sector State Codes

   Each sector is assigned to a Set as per IS-95.  This enum defines
   tag values for each set. 
--------------------------------------------------------------------------*/

typedef enum
{
  SET_NONE,            /* Sector does not belong to any set */
  SET_A,               /* Sector belongs to Active Set */
  SET_C,               /* Sector belongs to Candidate Set */
  SET_PC_N,            /* Sector belongs to Neighbor Set, but is currently
                          considered a Pre-Candidate Set member */
  SET_N,               /* Sector belongs to Neighbor Set */
  SET_PC_R,            /* Sector belongs to the Remaining Set, but is
                          currently considered a Pre-Candidate Set member */
#ifdef FEATURE_IS95B_MAHHO
  SET_CF_N,            /* Sector belongs to the Candidate Freq set */
#endif /* FEATURE_IS95B_MAHHO */

  SET_R                /* Sector belongs to the Remaining Set */
}
srch_set_enum_type;

#define SRCH_MSM_NO_VERSION   0xFFFF  /* Undefined MSM version  */

/*--------------------------------------------------------------------------
                          Delayed Finger States

   Used to prevent commanding a new slew when the finger is still slewing
--------------------------------------------------------------------------*/

typedef enum
{
  DELAYED_FINGER_TRACKING,   /* Slew commands allowed */
  DELAYED_FINGER_RESERVED,   /* Finger reserved for slamming */
  DELAYED_FINGER_UNRESERVED  /* Returning to tracking mode. */
}
delayed_fing_state_type;

/*--------------------------------------------------------------------------
                          Wakeup Timeline PN Roll States

   Used to sequence wakeup PN roll timing.
--------------------------------------------------------------------------*/

typedef enum
{
  SRCHZZ_WAIT_WAKEUP_ROLL,      /* Chipx8 turns on at next PN roll        */
  SRCHZZ_QPCH_DEMOD,            /* Demodulating QPCH bits.                */
  SRCHZZ_WAIT_20MS_FRAME,       /* Next roll is 20ms before slot boundary */
  SRCHZZ_QPCH_TO_PC,            /* QPCH Done, transition to PC operation  */
  SRCHZZ_WAIT_SLOT_BOUNDARY,    /* Next roll is the slot boundary         */
  SRCHZZ_AT_SLOT_BOUNDARY,      /* The slot boundary PN roll occurred.    */
  SRCHZZ_AFTER_SLOT_BOUNDARY    /* 1 or more PN rolls after slot boundary */
}
srchzz_wakeup_state_type;

#if defined FEATURE_IS95B_PPSMM || defined FEATURE_IS95B_MAHHO
/*--------------------------------------------------------------------------
                          IS95B enum type define
--------------------------------------------------------------------------*/
typedef enum     /* IS95B 6.6.6.2.12 */
{
  NO_CHECK,                /* Not need check at all     */
  CHECK_PWR_ONLY,          /* Check received power only */
  CHECK_ECIO_ONLY,         /* Check Ec/Io only          */
  CHECK_PWR_ECIO_BOTH      /* Check power and Ec/Io     */
}
srchtc_chk_pat_enum_type;
#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO */


/*--------------------------------------------------------------------------
                         Constant Definitions
--------------------------------------------------------------------------*/

#define  ASET_WIN_SIZ       srch_parms.aset_win_siz
#define  NSET_WIN_SIZ       srch_parms.nset_win_siz
#define  RSET_WIN_SIZ       srch_parms.rset_win_siz
#ifdef FEATURE_IS95B_MAHHO
#define  CFNSET_WIN_SIZ     srch_parms.cf_srch_win_n
#endif /* FEATURE_IS95B_MAHHO */
  /* Over-air window sizes. Units are chipx8. */

#define   WIN_TAB_SIZ     16
  /* The maximim length of the window size mapping table */

#define  WIN_MAX_SIZ        (226 * 2)
  /* Biggest window size ever used, in half-chip units. Buffers allocated
     this big. */

#define  CHIPX8_PER_CHIP    8
  /* The number of 1/8 chip "chipx8" units per chip. Used in slew math. */

#define  CHIPX8_PER_HALF_CHIP    4
  /* The number of 1/8 chip "chipx8" units per half-chip. Used in slew math. */

#define  WIN_MAX_1X_SIZE    (226 * CHIPX8_PER_CHIP)
  /* Max size of the window in PC and TC state which we will search at 1X
     speed.  "226" corresponds to IS-95 OTA window size 13. */

#define  MS_PER_SEC         1000
  /* The number of milliseconds per second. */

#define  MS_PER_SLOT        80
  /* The number of milliseconds per 80ms superframe. */

#define  WIN_CNT            2
  /* We will allocate this many window buffers. */

#define  NO_PWRINX          ((word)~0)
  /* Indicates that no power control symbol combiner is assigned */

#define  NO_FINGER          ((word)~0)
  /* Value used in a finger index to indicate none of the fingers. */

#define  NO_PATH            ((word)~0)
  /* Value used in a path index to indicate none of the paths. */

#define  NO_SECTOR          ((word)~0)
  /* Value used in a sector index to indicate none of the sectors. */

#define  NO_PILOT           (-1)
  /* Value used in a pilot offset to indicate no pilot offset */

#define  PILOT_SELF         (-1)
  /* Value used in a pilot offset to indicate your own pilot offset */

#define  NEVER_LOCKED       ((int4) 0x7FFFFFFF)
  /* Value used in a finger position to indicate never having been in lock */

#define  ROLLS_PER_MAX_SLOT_CYCLE   12288
  /* Number of 26.666...ms PN rolls in the max slot cycle.  This
     has actually been doubled from 6144, because BC SMS slots are
     longer than 6144. */

#define  MAX_PILOT_OFFSET   511
  /* Highest valid value for a Pilot PN Offset, per IS-95 */

#define  PILOT_PN_MASK      511
  /* Mask for normalizing the PN value */

#define  CHIPS_PER_OFFSET   64
  /* The number of chips between the center of each pilot offset */

#define  CHIPX8_PER_OFFSET  (CHIPS_PER_OFFSET * CHIPX8_PER_CHIP)
  /* The number of chipx8 between the center of each pilot offset */

#define  SHORT_CODE_LEN         32768L
#define  SHORT_CODE_HALF_LEN    16384L
#define  SHORT_CODE_QUARTER_LEN  8192L
  /* The length in chips of the short code PN sequence (and
     half and quarter lengths) */

#define  PN_CIRCLE_MASK    0x3FFFF
  /* Mask for normalizing chipx8 values */

#define  FULL_PN_CIRCLE    (SHORT_CODE_LEN * (int4) CHIPX8_PER_CHIP)
#define  HALF_PN_CIRCLE    (SHORT_CODE_HALF_LEN * (int4) CHIPX8_PER_CHIP)
#define  QUARTER_PN_CIRCLE (SHORT_CODE_QUARTER_LEN * (int4) CHIPX8_PER_CHIP)
  /* The number of 1/8 chip "chipx8" units in a full, half, and quarter
     PN sequence */

#define  SRCH_MAX_FADE_ITER   3
  /* The "threshold-of-pain" for consecutive failures to demod 
     Sync/Paging channel */

#define  GOOD_PILOT_BACKUP  (100 * CHIPX8_PER_CHIP)
  /* The amount we will back up the last_good_* positions to try and
     catch the peak on reacq, or miss it on sync failure */

#define CELL_AGC_RSSI_THRESH    -90
  /* This is the minimum dBm threshold for declaring "signal of
     interest" in Cellular Acquisition if performing "micro-Acq" */

#define PCS_AGC_RSSI_THRESH     -100
  /* This is the minimum dBm threshold for declaring "signal of
     interest" in PCS Acquisition if performing "micro-Acq" */

#define  ECIO_TAB_SIZ       64
  /* Size of translation tables from IS-95 Ec/Io to anything.  This
     is just the number of possible Ec/Io values */

#ifdef FEATURE_DIAG_V7_LOG_IFACE
#define  AGC_BUF_CNT         3
  /* Number of AGC buffers we will allow */

#define  FING_BUF_CNT        1
  /* Size of the finger logging buffer pool. Having only one of
     these throttles the rate at which finger status can be logged.
     This is intentional. */

#define  SRCH_BUF_CNT        2
  /* Size of the search scan logging buffer pool. Having only one of
     these throttles the rate at which searcher windows can be logged.
     This is intentional. However, for path logging, we need more
     than one buffer so we can be filling one while sending one. */

#define  GEN_TA_BUF_CNT      3 
  /* Size of the general TA buffer pool */ 
#endif /* #ifdef FEATURE_DIAG_V7_LOG_IFACE */

#define  MAX_FING_CNT        4
  /* How many fingers in the master table.  Also how many
     in the demodulator hardware */

#define MAX_PCTL_COMB_CNT    3
  /* Max number of power control symbol combiners suppoted by hardware */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)

#define  FING_SUP_CNT        3
  /* Max number of fingers that are able to demod a supplemental 
     code channel */

#define MAX_NUM_SUP_CH_SUPPORT_IF_FOUR_FINGS    0
  /* Max number of supplemental channels can be support without 
     disabling the fourth fingers */

#endif

#define  SRCH_RPT_TIME      340
  /* Watchdog report interval, in millisecs. */

#define  SRCH_RSSI_TIMEOUT  100
  /* CDMA RSSI periodic timer timeout, in millisecs. */

#define  SRCH_SLEEP_DELAY   2560
  /* Delay for holding off sleep when initially slewing in,
     when keys are pressed, etc. This allows search to get a 
     good pilot estimate in these situations, looking for 
     potential idle handoffs. */

#define  SRCH_PC_HO_TIMEOUT 1280
  /* Idle handoff delay interval, in millisecs. */

#define  SRCH_PC_THROT_TIMEOUT 240
  /* Idle handoff throttle interval, in millisecs. */

#define  SRCH_DUMP_TIMEOUT  150 
  /* Status dump timeout for lost dump recovery, in millisecs. */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_DYNAMIC_THROTTLE 
#define SRCH_GOVERN_ADJ_TIME  srch_rate_adj_time
extern word srch_rate_adj_time;
#else
#define SRCH_GOVERN_ADJ_TIME  200    
  /* Search governer adjust time period, in milliseconds. */
#endif

#define  FREQ_TIMER_MS     30000L
  /* Frequency estimation periodic timer timeout value,  in milliseconds */

/* MSM2.3 has 4 peaks sorted in hardware.  MSM2.2 has 1 */
#define SRCH_MAX_PEAKS 4

#define  SRCH_MAX_RECENT_TEMP_CNT  5
  /* The size of the recent_temp array */

#define  GET_TEMP_EST  -99
  /* Value indicating that we want to get a temperature estimate before
     getting a frequency estimate */

extern byte srch_slot_mask;
  /* The bit field used to indicate and keep track of what slot
     we are in and what SLEEP_F command we are waiting for */

extern qword srch_abs_slot_count;
  /* Keeps track of the absolute slot count, in 80 msec units */

extern qword srch_no_slot;
  /* Used to for initializing and comparing with other qwords */

extern qword srch_next_pg_slot;
  /* Used to store the absolute slot count of the next assigned paging
     channel slot, in 80 msec units */

extern qword srch_last_pg_slot;
  /* Used to store the absolute slot count of the last assigned paging
     channel slot, in 80 msec units */

#ifdef FEATURE_BROADCAST_SMS
extern qword srch_next_bc_pg_slot;
  /* Used to store the absolute slot count of the next assigned BCSMS
     slot, in 80 msec units */

extern qword srch_last_bc_pg_slot;
  /* Used to store the absolute slot count of the last assigned BCSMS
     slot, in 80 msec units */

extern qword srch_next_bc_data_slot;
  /* Used to store the absolute slot count of the next BCSMS data
     slot, in 80 msec units */
#endif /* FEATURE_BROADCAST_SMS */

#ifdef FEATURE_IS2000_QPCH
extern qword srch_next_qpch_slot_time;
    /* The slot time of the next QPCH slot in 20msec frame counts */
#endif /* FEATURE_IS2000_QPCH */

#ifdef FEATURE_IS2000
#define SRCH_WIN_OFFSET_TAB_MAX   7
  /* The search window offset multiplier table size */
#endif


/*--------------------------------------------------------------------------
                          IS95B Constants
----------------------------------------------------------------------------*/
#ifdef FEATURE_IS95B 

#define SRCH_MAX_EC_THRESH  115
  /* Maximum EC threshold for system re-slection */

#endif

#if(defined(FEATURE_GPSONE) || defined(FEATURE_IS95B))
#define SRCH_CAI_N12M   10
#endif

#if defined(FEATURE_IS95B_PPSMM)   || \
    defined(FEATURE_IS95B_MAHHO) 
#define  SRCH_PSD_TIMEOUT 20
  /* CDMA power spectral density periodic timer timeout, 
    in millisecs. */

#define MIN_PPSMM_TIME 80
  /* Min periodic PSMM time allowed. */

#define NO_MIN_PILOT_PWR_THRESH_CHK       0x1F
  /* See IS95B 6.6.6.2.12 */

#define NO_MIN_PILOT_EC_IO_THRESH_CHK     0x1F
  /* See IS95B 6.6.6.2.12 */

#define NO_SF_TOTAL_EC_THRESH_CHK         0x1F
  /* See IS95B 6.6.6.2.8.3.2 */
  
#define NO_SF_TOTAL_EC_IO_THRESH_CHK      0x1F
  /* See IS95B 6.6.6.2.8.3.2 */

#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS95B_MAHHO
#define FRAME_LEN 20
  /* Frame length in ms */

#define FRESHNESS_INTERVAL  (40*FRAME_LEN)
  /* Freshness interval in ms (40 frames) */

#define EXPECTED_MAX_OFF_TIME_FWD  (6*FRAME_LEN)
  /* The expected max forward ch. off time due to one cfs visit. 
     It's in ms unit (6 frames)  */

#define EXPECTED_ERASE_RATE 15
  /* The expected erase rate due to CF search, in percentage */

#define CHIP_PERIOD 814
  /* IS95 chip period in 1/1000 us */

#define CFS_SW_LATENCY 5
  /* The estimated software latency of per one CFS visit (ms) */

#endif /* FEATURE_IS95B_MAHHO */
/*--------------------------------------------------------------------------
                          PCS Constants
----------------------------------------------------------------------------*/

#define  SRCH_NUM_PRI_LEVELS  4
  /* Number of priority levels for neighbor set pilots */

#define  SRCH_PRI_LOW   0
#define  SRCH_PRI_MED   1
#define  SRCH_PRI_HIGH  2
#define  SRCH_PRI_VHIGH 3
  /* Neighbor search priorities */

#define  FREQ_INCL_NO_FREQ      FALSE
#define  FREQ_INCL_FREQ         TRUE
  /* Frequency include flag */

/*--------------------------------------------------------------------------
                          Phase80 Declarations
--------------------------------------------------------------------------*/

#define PHASE_80M 0xC000u
  /* Mask used to determine phase of 80 ms superframe from time reference */

#define PHASE_SUB_80M 0x3fffu
  /* Mask to get range within a 26.667ms sub period of the 80ms period. */

#define PHASE0 0x0000u
  /* Value for masked reference time when in 1st 26.66 ms period of 80 ms */

#define PHASE1 0x4000u
  /* Value for masked reference time when in 2nd 26.66 ms period of 80 ms */

#define PHASE2 0x8000u
  /* Value for masked reference time when in 3rd 26.66 ms period of 80 ms */

#define PHASE_80_SHIFT 14
  /* Value to shift PHASE_80M masked value to get to range 0 .. 2 */

#ifdef T_MSM5000
#define FCH_BETA_SCALE 0x20
  /* Value that the SW_BETA_FCH register should be set to (per Haitao) */

#define COMBINER_CODE_CH_0 0x01
#define COMBINER_CODE_CH_1 0x02
#define COMBINER_CODE_CH_2 0x04
#define COMBINER_CODE_CH_3 0x08
#define COMBINER_CODE_CH_4 0x10
#define COMBINER_CODE_CH_5 0x20
#define COMBINER_CODE_CH_6 0x40
#define COMBINER_CODE_CH_7 0x80
  /* Bit masks used to access the code channels */

#define FING_0_MASK 0x01
#define FING_1_MASK 0x02
#define FING_2_MASK 0x04
#define FING_3_MASK 0x08
  /* Bit masks to access individual fingers */

#define ALL_FING_MASK (FING_0_MASK | FING_1_MASK | FING_2_MASK | FING_3_MASK)
  /* A quick way to specify all the fingers */
#endif /* T_MSM5000 */


/*--------------------------------------------------------------------------
                          Finger Change Flags
--------------------------------------------------------------------------*/

typedef byte srch_fing_change_type;
  /* Bit field for following flags */

#define FINGER_CHANGED_SECTOR   0x01
#define FINGER_CHANGED_PILOT    0x02
#define FINGER_CHANGED_PATH     0x04
#define FINGER_CHANGED_FCH      0x08
#define FINGER_CHANGED_SCH      0x10
#define FINGER_CHANGED_SCCH     0x20
#define FINGER_CHANGED_DCCH     0x40
  /* Bit maskes used in srch_fing_cmd( ) to indicate what finger registers
     need to be updated. */

#define FINGER_CHANGED_NOTHING  0x00
  /* Nothing is changed; finger registers (now) match finger structure. */

#define FINGER_CHANGED_EVERYTHING ( \
            FINGER_CHANGED_SECTOR | \
            FINGER_CHANGED_PILOT  | \
            FINGER_CHANGED_PATH   | \
            FINGER_CHANGED_FCH    | \
            FINGER_CHANGED_SCH    | \
            FINGER_CHANGED_SCCH   | \
            FINGER_CHANGED_DCCH   )
  /* For migration purposes only.  Everything needs to be updated.
     Equivalent of the older "new_ass = TRUE".                    */

#ifdef FEATURE_IS2000_QPCH
/*---------------------------------------------------------------------------
                        Constants pertaining to QPCH
---------------------------------------------------------------------------*/

#define PN_WORDS_PER_ROLL  512
  /* Number of PN words per PN roll */

#define ODD_CATNAP_UNIT_LEN  ( SHORT_CODE_LEN / ODD_CATNAP_UNIT_PER_ROLL )
  /* Unit length for non-PN roll catnaps in chips */

#define ODD_CATNAP_UNIT_PER_ROLL  8L
  /* Number of odd catnap unit in one PN roll */

#define REACQ_TIME  ( SHORT_CODE_LEN * 3L / 4L )
  /* Re-acquisition time in chips */
#endif /* FEATURE_IS2000_QPCH */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

/* Search System Determination 2.0 support must 
   include support for the 16X searcher. */

#if ( defined(FEATURE_SD20) && defined(FEATURE_SEARCH2) )
#error code not present
#else
 #undef SRCH_ENABLE_SD20
#endif

#ifdef SRCH_ENABLE_SD20
#error code not present
#endif /* SRCH_ENABLE_SD20 */

/*--------------------------------------------------------------------------
          Defines for IS2000

--------------------------------------------------------------------------*/
#ifdef FEATURE_IS2000
#define SRCH_MAX_MC_TRANS 2
  /* Maximum number of MC transactions SRCH can process at a time */
#endif /* FEATURE_IS2000 */


#ifdef FEATURE_IS95B_MAHHO
/*--------------------------------------------------------------------------
          IS-95B SERVING FREQUENCY INFORMATION STRUCTURE TYPE

This structure is used for IS-95B Serving Frequency Infromation.
--------------------------------------------------------------------------*/
typedef struct
{
  int2 power; 
    /* Serving Frequency Power */

  word cset_cnt;
    /* SF Candidate set size */

  word nset_cnt;
    /* SF Neighbor set size */

  word rset_cnt;
    /* SF Remaining set size */
}
srch_sf_infor_type;
#endif /* FEATURE_IS95B_MAHHO */

#if defined( FEATURE_IS95B_PPSMM ) || defined (FEATURE_IS95B_MAHHO) || \
    defined( FEATURE_GPSONE )
/*--------------------------------------------------------------------------
          IS-95B POWER SPECTRAL DENSITY MOVING AVERAGE STRUCTURE TYPE

This structure is used for IS-95B Serving Frequency moving average of rssi.
--------------------------------------------------------------------------*/
typedef struct
{
  boolean enabled;
    /* Set to enable the moving average calculation */

  boolean full;
    /* Set to indicate the circle buffer of the moveing averge is full */

  byte index;
    /* Index of a array to store the most recent received rssi value */

  int2 psd[ SRCH_CAI_N12M ];
   /* A array to store the member of the moving average */
}
srch_moving_psd_type;
#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO || FEATURE_GPSONE */

#ifdef FEATURE_IS95B_MAHHO
/*--------------------------------------------------------------------------
                 SRCH INTERNAL TIMER STRUCTURE TYPE

This structure is used for SRCH internal timer.
--------------------------------------------------------------------------*/
typedef struct
{
  boolean enabled;
    /* Set to enabled the timer */

  word ticks;
    /* The timer ticks  */
}
srch_timer_type;
#endif /* FEATURE_IS95B_MAHHO */

#if defined FEATURE_IS95B_PPSMM || defined FEATURE_IS95B_MAHHO
/*--------------------------------------------------------------------------
          IS-95B PERIODIC TIMER STRUCTURE TYPE

This structure is used for IS-95B Periodic timer.
--------------------------------------------------------------------------*/
typedef struct
{
  boolean enabled;
  /* Set to enable the periodic procedure. */

  boolean rpt_pending;
  /* A report is pending */

  boolean active;
  /* A timer is running */

  srchtc_chk_pat_enum_type  check_patten;
  /* We use this field to check if we need test PWR or Ec/Io */

  word  periodic_time;
  /* Periodic timer value */

  byte  min_pilot_pwr_thr;
  /* Min. Pilot Ec threshold */

  byte  min_pilot_ec_io_thr;
  /* Min. Pilot Ec/Io threshold */

  word  min_ecio_eng;
  /*  min_pilot_ec_io_thr in linear units */

}
srch_periodic_timer_ctl_type;
#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO */


/*--------------------------------------------------------------------------
               Declaration of Search Parameters Structure
--------------------------------------------------------------------------*/
#ifdef FEATURE_IS2000_SCH
typedef mctrans_server_token_type srch_trans_server_type;
  /* Transaction server type */

typedef mctrans_action_type srch_trans_action_type;
  /* Action type for this transaction */
#endif /* FEATURE_IS2000_SCH */

typedef struct
{
  byte     vc_osc_ctl;
    /* Byte contains info to turn on/off vocoder oscillator */

  byte     frssi_scale;
    /* Scale multiplier for MSM23 */

  byte     frssi_low_mask;  
    /* Mask out lower byte of frssi */  
}
srch_msm_config_struct_type;

typedef struct
{
  int2  pilot_inc;
    /* How far to jump ahead in pilot offset when searching the remaining
       set */

  int2  aset_win_siz;
  int2  nset_win_siz;
  int2  rset_win_siz;
    /* Sweep window sizes for the three sets in 1/8 chip units. 
       These are int4 since they are frequently used in slew
       computations. */

  byte  win_a;
  byte  win_n;
  byte  win_r; 
    /* Sweep window sizes for the three sets in CAI index units. */ 

  int2  t_add;
    /* Ec/Io required for promotion to the candidate set */

  int2  t_drop;
    /* Ec/Io required for demotion from the candidate set */

  word  t_add_eng;
  word  t_drop_eng;
    /* The above two Ec/Io values converted to energies */

  word  t_nset_eng;
    /* Neighbor threshold for promotion to the Pre-Candidate Set,
       corresponding to the t_add value above */

  word  pc_thresh_eng;
    /* Threshold for the Pre-Candidate Set state machine */

  byte  t_comp;
    /* Energy ratio by which a rising candidate must exceed an
       active set pilot to trigger a pilot strength measurement
       message.  In 0.5 dB units. */

  word  t_tdrop;
    /* If a pilot's strength stays below t_drop for this duration,
       its dropout timer expires. In units of 80ms. */

  byte  nghbr_max_age;
    /* The number of Neighbor List Update messages that can pass
       without mentioning a pilot before the pilot is dropped from
       the neighbor set. */

#ifdef FEATURE_IS95B_SOFT_HO
  int2 soft_slope;
    /* Threshold slope value */

  int2 add_intercept;
    /* T_ADD threshold intercept value, in 0.5dB units */

  int2 drop_intercept;
    /* T_DROP threshold intercept value, in 0.5dB units */
#endif

#ifdef FEATURE_IS95B_SYS_RESELEC      /* System re-selection in Idle state */
  boolean  sys_reselect_enabled;
    /* Flag tells if the re-selection thresholds included */

  word  ec_thresh;
    /* Pilot power threshold */

  word  ec_io_thresh;
    /* Pilot Ec/Io threshold */

  word  ecio_thresh_eng;
    /* The Ec/Io values converted to energy */
#endif

#ifdef FEATURE_IS95B_MAHHO
  int1  diff_rx_pwr_thresh; 
    /* Mini. difference in received power */
  
  byte  min_tot_pilot_ecio;   
    /* Minimum total pilot Ec/Io */

  word  min_tot_pilot_eng;
    /* Minimum total pilot Ec/Io in linear unit */

  byte  cfs_seq;
    /* The sequence number for the current search period */

  word  cf_search_period;
    /* Time between successive searches on the CF. (in frame unit)*/
  
  boolean  cf_periodic_srch;
    /* TRUE: CF Periodic search, FALSE, single search. */

  byte  cf_band_class;     
    /* Band class of the CF */
  
  word  cf_cdma_freq;      
    /* CDMA frequency for the CF */
  
  byte  cf_t_add;  
    /* Pilot detection threshold for CDMA on the CF */
  
  byte  cf_pilot_inc;   
    /* pilot PN offset for the CF */
  
  int2  cf_srch_win_n;  
    /* Default search window size for the CF Search Set */
  
  int2  cf_srch_win_r;
    /* Search window size for the Remaining Set on the CF */
  
  word  cf_pilots_cnt;
    /* Number of pilots in CFReq message, it includeds 
       the pilots need to be searched and the pilots don't need
       to be searched */

  word  cf_pilots_srch_cnt;  
    /* Number of pilots to be searched */
  
  byte  cf_nghbr_srch_mode; 
    /* Search mode for the CF */

  word  cf_max_win;
    /* Maximum window size for CF */
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
  byte transaction_num; 
    /* Transaction num of this HO transaction */
  
  qword action_time;  
    /* Action time when this cmd needs to be acted upon */
  
  boolean hho_ind;  
    /* Indicates if this is a HHO */
  
  boolean rif_ind;  
    /* Indicates if we need to return to SF on failure */

  boolean srch_incl; 
    /* Indicates if search parameters are included in this command */

  boolean lcm_change_ind;
    /* Indicates whether to restore long code mask after RIF-HHO */

  qword lcm;
    /* Long code mask */

  struct
  {
    mctrans_cmd_type  trans[ SRCH_MAX_MC_TRANS ];
      /* MC transactions */

    byte              cur_trans;    
      /* Index for current transaction. Initialized to 0 at reset */

    byte              total_trans;
      /* Number of transactions. Initialized to 0 at reset */
  } mc_trans;
    /* MC transaction information */

#endif /* FEATURE_IS2000    */
}
srch_parm_struct_type;


/*--------------------------------------------------------------------------
                             Finger Structure

   This structure is used in the table of fingers.  It really has two
   kinds of functions:

     (1) It keeps track of the status of each finger, and
   
     (2) It centralizes all the information needed to command the finger
         to its new assignment.  The routines that need to adjust a finger
         simply adjust the values in its table entry at will, then call
         srch_fing_cmd() to command the hardware accordingly.
         
   Second structure contains information common to all fingers, instead of
   per-finger information.

--------------------------------------------------------------------------*/
typedef struct
{
  srch_fing_change_type change_flags;
    /* If nonzero, this finger has been changed and needs to be
       given its new assignment(s).  Individual flags indicate
       which data member groups have been changed. */

  word  new_ass;   /* Depricated */
    /* If nonzero, this finger has been changed and needs to be
       given its new assignment.  There is no attempt to keep track
       of what has changed; everything must be reprogrammed whenever
       this flag is nonzero. */

  word  sect;
    /* Index into the table of sectors, indicating which sector this
       finger has been assigned to.  If the finger is unassigned,
       this field is set to NO_SECTOR */

  word  path;
    /* Index into the array of paths, indicating which path this
       finger has been assigned to. */

  int2  pilot;
    /* Pilot offset of the sector this finger has been assigned to. */

  srch_fch_info_type fch_info;
    /* Parameters for the FCH */

  #ifdef FEATURE_IS2000_P2
#error code not present
  #endif /* FEATURE_IS2000_P2 */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  srch_scch_info_type scch_info;
    /* Parameters for the SCCH */
#endif

#ifdef FEATURE_IS2000_SCH
  struct
  {
    byte num;
      /* Number of SCH channels */

    byte walsh[ SRCH_SCH_MAX ];
      /* Array of SCH walsh codes */

    byte qof_mask_id[ SRCH_SCH_MAX ];
      /* Array of SCH QOF mask id's */
  }
  sch_info;
#endif /* FEATURE_IS2000_SCH */
  
  word  pwr_inx;
    /* Index indicating which group of sectors this finger is to be
       combined with for power control purposes.  Used to configure
       the power combiner.  Only indexes 0, 1, and 2 are assigned
       to power combiner groups. */

  word  assg_cnt;
    /* Number of times the finger has been assigned. */

  word  lock_age;
    /* Keeps track of how long the finger has been in lock.  This
       value is incremented whenever an active window search is
       completed and the finger is in lock. */

  word  unlock_age;
    /* Keeps track of how long the finger as been out of lock.  This
       value is incremented whenever an active window search is
       completed and the finger is out of lock. */

  int4  slew;
    /* How far to slew the finger at this command. Signed! */

  int2  pos;
    /* POS_MATH position of this finger relative to reference */

  int4  last_locked_raw_pos;
    /* Last value of raw_pos for which the finger was in lock.  This is
       used to limit how far the finger can wander off its assigned
       position before it gets de-assigned. */

  word  locked;
    /* Whether this finger is locked (as of the last time we did
       a srch_fing_dmp() ). */

  word  rssi;
    /* Received signal strength indicator value for this finger
       (as of the last srch_fing_dmp() ).  The demod chip provides
       16 bits of RSSI, but we only use the 8 MSBs of that. */

  word  raw_rssi;
    /* Used to hold the upper byte of the rssi energy as read from
       the dmod.  These particular values are used only for Gen TA packets.
       All other logging packets use the converted energy found in the
       member "rssi" above. */

  int4  raw_pos;
    /* The finger's position, as of the last srch_fing_dmp(). */

  boolean ttl_slow;
    /* TRUE if slow TTL is desired, FALSE if fast (regular) TTL is needed */
}
srch_fing_struct_type;


typedef struct
{
  word  ttrk_k1_gain;
  word  ttrk_k2_gain;
    /* The gain to be used for finger time tracking. */

  word  lock_hi;
    /* Lock indicator threshold value: when the RSSI goes above
       this value, the finger will go into lock. */

  word  lock_lo;
    /* Lock indicator threshold value: when the RSSI goes below
       this value, the finger will go out of lock. */

  word  lock_new;
    /* Lock indicator preset value: the RSSI accumulator is preset
       to this value when the finger is slewing. */

  boolean changed;
    /* The common parameter information has changed.  Transfer parameters
       to demodulator on next srch_fing_cmd( ) */
  
  word  pctl_lock;
    /* Power control lock threshold - this threshold is downstream
       from the symbol combiner lock thresholds so if this is zeroed,
       you get the locks defined above */
}
srch_fing_parm_struct_type;


/*--------------------------------------------------------------------------
                          Window Structure

    The window structure serves as a buffer allocation mechanism for
  searcher sweep windows.  Instead of allocating a (fairly large) buffer
  for each sector in the table, we allocate and deallocate buffers as
  needed.  Free buffers are kept on srch_win_q.   
--------------------------------------------------------------------------*/
typedef struct
{
  q_link_type  link;
    /* Link for putting this buffer on a queue. */

  word  win[ WIN_MAX_SIZ ];
    /* The buffer itself. */
}
srch_win_struct_type;

/*--------------------------------------------------------------------------
                            TRPT Structure

  This structure is used in the sector structure and in temporary sector
  list structures to contain values used in reporting to the base station.
--------------------------------------------------------------------------*/
typedef struct
{
  word  eng;
    /* Filtered energy in this sector */

  word  ecio;
    /* Ec/Io value corresponding to filtered energy */

  word  len;
    /* Current length of this sector's FILT_N filter */

  int2  pos;
    /* POS_MATH position of this sector's best (usually earliest)
       path in 1/8 chip units. Numbers will be small deltas about
       POS_MATH_OFFSET. */

  int2  ear_pos;
    /* POS_MATH position of this sector's latest earliest path.
       In 1/8 chip units. Numbers will be small deltas about
       POS_MATH_OFFSET. */

  int2  nom_off;
    /* Initial offset of pilot from center in 1/8 chip units.*/

  byte  rev_dens;
    /* "Fast-Revisit" search density for speeding up looks at 
        pilots apporaching T_ADD report threshold */

  byte  rev_indx;
    /* "Fast-Revisit" search index */

  dword  drop_timer;
    /* Handoff drop timer.  This value is set to the current system
       time in 26.66..ms units every time the pilot is observed to be
       above the t_drop threshold.  If this value plus the T_TDROP
       duration ever exceeds the current system time, the handoff
       drop timer will be considered expired. */

  int2  cnt;
    /* Number of entries in the following list */

  int2  pilots[ SRCH_ASET_MAX + 1 ];
    /* List of pilot offsets, used to keep track of what has been
       reported to the base station */

  byte  pilot_cnt[ SRCH_ASET_MAX ];
    /* T_COMP state machine, used to speed up reporting when
       there are many pilots in the Active/Candidate sets */
}
srch_trpt_struct_type;


/*--------------------------------------------------------------------------
                              Sector Structure

   The Sector is in many ways the basic element that the searcher task
   deals with.

   As used in the searcher task, a Sector is a signal or hoped-for signal
   at a particular pilot offset.  This may correspond to one sector of a
   sectored cell, or to an unsectored cell's single sector.

   The sector struct contains different types of information:

      * How to demodulate the sector
      * How to search for the sector's signal
      * The results of the most recent search of the sector
      * The combined results of all recent searches of the sector

   So, the sector struct can be passed around to various kinds of
   functions without also passing a lot of associated information.
   Which fields of the sector are meaningful at any given time varies.
--------------------------------------------------------------------------*/

typedef struct
{
  int2  pilot;
    /* The pilot offset (0-511) that defines this sector. */

  #ifdef FEATURE_IS2000_REL_A
#error code not present
  #endif /* FEATURE_IS2000_REL_A */

  byte config;
    /* The Neighbor config value for this sector */

  byte  pri;
    /* Priority of this particular sector, used in neighbor searches */

  boolean  freq_incl;
    /* Flag indicating existence of frequency and band-class information */

  byte  band_class;
    /* Band Class for this sector:  0 = 800mhz, 1 = PCS */

  word  cdma_freq;
    /* CDMA frequency assignment for this sector */

  srch_fch_info_type fch_info;
    /* Parameters for the FCH */

  #ifdef FEATURE_IS2000_P2
#error code not present
  #endif /* FEATURE_IS2000_P2 */

  #if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  srch_scch_info_type scch_info;
    /* Parameters for the SCCH */
  #endif

  #ifdef FEATURE_IS2000_SCH
  struct
  {
    byte num;
      /* Number of SCH channels */

    byte walsh[ SRCH_SCH_MAX ];
      /* Array of SCH walsh codes */

    byte qof_mask_id[ SRCH_SCH_MAX ];
      /* Array of SCH QOF mask id's */
  }
  sch_info;
  #endif /* FEATURE_IS2000_SCH */

  int2  win_siz;
    /* The sweep window size to use when searching for this sector.
       Units are 1/8 chips to make slew math clear and simple. */

  #ifdef FEATURE_IS95B
  int2  ota_win_n;
    /* The sweep window size of neighbor set received form the over-
       the-air messages. Units are 1/8 chips to make slew math clear
       and simple. */
  #endif

  byte  age;
    /* The "age" of the sector; used only in the Neighbor Set */

  srch_set_enum_type  set;
    /* Which set the sector is currently assigned to */

  int2  pc_state;
    /* Pre-Candidate Set state variable */

    /* Searcher Parameters */

  byte  gain;
    /* Gain factor before the second accumulators in the searcher */
    /* Encoding: 0 = 2, 1 = 4, 2 = 8, 3 = 32 */

  byte  chps;
    /* Number of 4-chips to integrate before converting to energy */

  byte  nc_int;
    /* Number of passes to accumulate non-coherently */
    /* Encoding: 0 = 1 pass, 1 = 2 passes, ... , 7 = 8 passes. */

  word  cell;
    /* Cell index.  This indicates which cell in the local cell
       table contains this sector.  The cell numbering may change
       whenever a Handoff Direction Messsage is received. */

      /* Search Results */

  int2  pos;
    /* The current POS_MATH position of the center of the search window.
       Units are 1/8 chips. Numbers will be relatively small deltas
       about POS_MATH_OFFSET */

  word  tot_eng;
    /* The total energy in the best paths found in the sweep */

  srch_trpt_struct_type  trpt;
    /* Reporting info including filtered energy */

  int4  win_pos;
    /* Absolute position in units of 1/8 chip of
       the END of the search window. */

  int4  max_pos;
    /* Absolute position of the maximum energy path found in the
       window in 1/8 chip units. */

  int2  max_inx[ SRCH_MAX_PEAKS ];
    /* Index in the sweep results of the maximum energy path
       in 1/8 chip units. */

  word  max_eng[ SRCH_MAX_PEAKS ];
    /* Energy of the maximum energy path */

  int2  center_slew;
    /* TC state centering slew memory in 1/8 chips */

  int2  center_cnt;
    /* Centering slew hysteresis counter */

  boolean  center_throttle;
    /* Centering slew throttling flag */

  int2  hard_off;
    /* Hard-handoff related earliest path offset from center of
       srch win. In 1/8 chip units. */

  dword last_srch_cnt;
    /* The absolute search command number during which this
       sector was last searched */

  int2  last_win_siz;
    /* Remember what window size the latest search was done at */

  srch_win_struct_type  *win_ptr;
    /* Pointer to a window buffer containing the sweep results */

  #if defined(FEATURE_IS95B_ACCESS_ENTRY_HO) || \
      defined(FEATURE_IS95B_ACCESS_HO)
  boolean access_entry_ho;
    /* Access Entry Handoff allowed flag */
  #endif

  #if defined(FEATURE_IS95B_ACCESS_ENTRY_HO) || \
      defined(FEATURE_IS95B_ACCESS_HO) || \
      defined(FEATURE_SPECIAL_MDR)
  boolean access_ho_allowed;
    /* Access Handoff allowed flag */
  #endif

  #if defined (FEATURE_IS95B_PILOT_RPT) || \
      defined (FEATURE_SPECIAL_MDR)
  boolean access_attempted;
    /* Flag tells whether we have attempted to access this pilot.
       Note: SRCH doesn't use this flag, it's merely for MC coding
             convenience */
  #endif

  #ifdef FEATURE_IS2000
  byte  srch_offset_nghbr;
    /* Neighbor Window Search Offset for the pilot */
  #endif /* FEATURE_IS2000 */
}
srch_sect_struct_type;


/*--------------------------------------------------------------------------
                            Cell Structure

   For the purposes of the Searcher task, a Cell is a collection of
   one or more Sectors, all of which carry the same power control
   subchannel.  This property means that finger assignment should
   favor cell diversity, and that the power control symbol combiners
   should be programmed to soft-combine power control symbols from all
   the sectors of a cell.

   Membership in a Cell is maintained in the Sector structure.  The
   Cell structure contains only information common to all sectors
   of the Cell.

   The cell table lists the current cells in the Active Set, in the
   order they were received in the last Handoff Direction Message.
   This means that cell numbering can change whenever a Handoff
   Direction Message is processed.

--------------------------------------------------------------------------*/

typedef struct
{
  int2  fing_cnt;
    /* The number of fingers actively assigned in this sector. */

  word  pwr_inx;
    /* The index (0,1,2, or NO_PWRINX) of the power control symbol combiner
       assigned to this cell. */

  word  sum_power;
    /* Summation of correlated energy contained within this cell */

  int2  pctl_fing_cnt;
    /* The number of fingers actively assigned in this sector for power control 
       symbol combiner assignment purposes */
}
srch_cell_struct_type;

/*--------------------------------------------------------------------------
                 Declaration of Structure for Path Tables

      In evaluating the results of a sweep, an early step is to identify
      the distinct paths observed and choose the best.  This struct is
      used to keep track of paths.
--------------------------------------------------------------------------*/

typedef struct
{
  word  sect;
    /* Index into srch_sects of the sector this path belongs to */

  word  fing;
    /* Index into srch_fings of a finger for this path */

  int2  pos;
    /* POS_MATH position of this path */

  word  eng;
    /* Energy of this path */

  word  inx;
    /* Index into search array of this path */
}
srch_path_struct_type;



/*--------------------------------------------------------------------------
                   Searcher speed governer parameter type

  This structure is used throughout Search to monitor and control the 
  searcher rate, duty cycle and overall system load.
--------------------------------------------------------------------------*/

typedef struct
{
  int4 srch_rate;
    /* Overall searcher rate */

  int4 srch_cnt;
    /* Counter used to determine search rate. */

  int4 aset_rate;
    /* Active set rate */

  int4 nset_rate;
    /* Neighbor set rate */

  int2 rate_max;
    /* Max searcher rate desired */

  int2 rate_min;
    /* Min searcher rate desired */

  int2 gov_dcy;
    /* Governor duty cycle */

  word gov_cnt;
    /* Duty cycle counter until next delay. */

  word timeout;
    /* Governer timeout period (ms) */

  byte attack;
    /* Attack rate */

  byte decay;
    /* Decay rate */

  byte max_pct;
    /* Max percent throttling */

  byte min_pct;
    /* Min percent throttling */
}
srch_govern_struct_type;



/*--------------------------------------------------------------------------
                   Searcher demod parameter type

  This structure is used throughout Search to carry around 
  and load some of the common demodulation parameters for the searcher.
--------------------------------------------------------------------------*/

typedef struct
{
  int2  win_siz;
    /* Size of the search window in 1/8 chip units (Used mainly in ACQ) */

  byte  gain; 
    /* Gain in the searcher hardware */

  byte  chps;
    /* Number of chips to integrate coherently */

  byte  nc_int;
    /* Number of integrations to add non-coherently */

  word  thresh; 
    /* Threshold to accept a window (used in ACQ only) */
}
srch_sect_parm_struct_type;


/*--------------------------------------------------------------------------
             Declaration of Structures for Temporary Tables

   In updating the Active Set and Neighbor sets, it is useful to make
   a temporary list of what will be put into the actual sets.  These
   lists need only contain a minimum of information from each sector,
   so a special stripped-down sector struct is defined.
--------------------------------------------------------------------------*/

typedef struct
{
  int2  pilot;
    /* The pilot offset that defines this sector. */

  byte config;
    /* The Neighbor config value for this sector */

  byte  pri;
    /*  The search priority for this pilot */

  boolean  freq_incl;
    /*  Flag indicating inclusion of frequency information */

  byte  band_class;
    /*  Band Class of this pilot */

  word  cdma_freq;
    /*  CDMA Frequency of this pilot */

  srch_trpt_struct_type  trpt;
    /* Reporting info including filtered energy */

  byte  age;
    /* "Age" of pilot; actually number of Neighbor Set Updates that
         have arrived since it was demoted from the Candidate or Active
         Set.  Compared to nghbr_max_age. */

#if defined (FEATURE_IS95B_PILOT_RPT) || \
           defined (FEATURE_SPECIAL_MDR)
  boolean access_attempted;
    /* Flag tells whether we have attempted to access this pilot.
       Note: SRCH doesn't use this flag, it's merely for MC coding
             convenience */
#endif

#ifdef FEATURE_IS95B
  boolean access_entry_ho;      
  /* Access Entry Handoff allowed */

  boolean access_ho_allowed;    
  /* Access (Probe) handodd allowed */

  int2  win_siz; 
  /* The sweep window size to use when searching for this sector. */
#endif

#ifdef FEATURE_IS2000
  byte  srch_offset_nghbr;
    /* Neighbor Window Search Offset for the pilot */
#endif /* FEATURE_IS2000 */
}
srch_tmp_nset_struct_type;

/*------------------------------------------------------------------------*/

typedef struct
{
  int2  pilot;
    /* The pilot offset that defines this sector. */

  #ifdef FEATURE_IS2000_REL_A
#error code not present
  #endif /* FEATURE_IS2000_REL_A */

  boolean  pci;
    /* PWR_COMB_IND: TRUE if this sector is in the same cell
       (that is, carries an identical power control subchannel)
       as the previous sector (in the aset array) */

  srch_fch_info_type fch_info;
    /* Parameters for the FCH */

  #ifdef FEATURE_IS2000_P2
#error code not present
  #endif /* FEATURE_IS2000_P2 */

  #if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  srch_scch_info_type scch_info;
    /* Parameters for the FCH */
  #endif

  #ifdef FEATURE_IS2000_SCH
  struct
  {
    byte num;
      /* Number of SCH channels */

    byte walsh[ SRCH_SCH_MAX ];
      /* Array of SCH walsh codes */

    byte qof_mask_id[ SRCH_SCH_MAX ];
      /* Array of SCH QOF mask id's */
  }
  sch_info;
  #endif /* FEATURE_IS2000_SCH */
  
  srch_trpt_struct_type  trpt;
    /* Reporting info including filtered energy */

  int2  max_inx[ SRCH_MAX_PEAKS ];
    /* Index in the sweep results of the maximum energy path
       in 1/8 chip units. */

  word  max_eng[ SRCH_MAX_PEAKS ];
    /* Energy of the maximum energy path */

  int2  win_siz;
    /* Hard-handoff related storage for search window size in 1/8 chips */

  #ifdef FEATURE_IS2000
  byte  srch_offset_nghbr;
      /* Neighbor Window Search Offset for the pilot */
  #endif
}
srch_tmp_aset_struct_type;


/*--------------------------------------------------------------------------
               Declarations of Structures for DIAG reports
--------------------------------------------------------------------------*/
typedef struct
{
  word  set_mask;
    /* Specifies which type of pilot sets should be reported.
       Bit 15 => report active set pilot scans.
       Bit 14 => report candidate set pilot scans.
       Bit 13 => report neighbor set pilot scans.
       Bit 12 => report remaining set pilot scans. */

  word  desired_ta_cnt;
    /* Specifies desired size of each scan in 1/2 chip units. */
}
srch_ta_parm_struct_type;
  /* This structure keeps track of what is to be sent to the
     temporal analyzer. */

#ifdef FEATURE_DIAG_V7_LOG_IFACE
/*------------------------------------------------------------------------*/

typedef struct
{
  log_desc_type   desc;
  log_srch2_type  entry;
}
srch_log_struct_type;
  /* Type declaration for a searcher log entry */

/*------------------------------------------------------------------------*/

typedef struct
{
  log_desc_type  desc;
  log_fing_type  entry;
}
srch_fing_log_struct_type;
  /* Type declaration for a finger log entry */

/*------------------------------------------------------------------------*/

typedef struct
{
  log_desc_type    desc;
  log_gen_ta_type  entry;
}
srch_gen_ta_log_struct_type;
  /* Type declaration for a general TA log entry */

/*------------------------------------------------------------------------*/

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
typedef struct
{
  log_desc_type    desc;
  log_gen_ta_sup_ch_type  entry;
}
srch_gen_ta_sup_log_struct_type;
#endif
/*------------------------------------------------------------------------*/

typedef struct
{
  log_desc_type      desc;
  log_s_agc_pctl_type  entry;
}
srch_agc_log_struct_type;
  /* Declaration of Structure for AGC Reports */
#endif /* #ifdef FEATURE_DIAG_V7_LOG_IFACE */

/*------------------------------------------------------------------------*/

typedef struct
{
#ifndef FEATURE_DIAG_V7_LOG_IFACE
  log_s_agc_pctl_type log;
#else
  srch_agc_log_struct_type  *ptr;
    /* Pointer to the current AGC log buffer we're filling up, or
       NULL if there isn't currently one allocated */
#endif

  int2  inx;
    /* Index into the AGC log buffer */

  word  drops;
    /* Count of how many times we've dropped the AGC logging operation
       for lack of a buffer */

  boolean  enabled;
    /* Tells whether or not logging is enabled */
}
srch_agc_pctl_vars_struct_type;


/*--------------------------------------------------------------------------
                Declaration of ISR structure variables
--------------------------------------------------------------------------*/

typedef struct
{
  word  phase80;
    /* 0,1,2 count of 26ms periods within an 80ms */

  dword  tick26;
    /* Overall count of 26.66.. ms roll periods */
}
srch_roll_isr_vars_struct_type;
  /* PN roll ISR variables */

/*------------------------------------------------------------------------*/

typedef struct
{
  srch_sect_struct_type  *sect_ptr;
    /* Pointer to the sector struct defining the current search */

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#endif /* FEATURE_SRCH_SCHEDULER */

  boolean  active;
    /* TRUE if the search is still in progress */

#ifdef FEATURE_DYNAMIC_THROTTLE
  dword srch_chips;
    /* A running total of chips searched (in chipx8 units) */
#endif

  dword srch_cnt;
    /* A running total of search commands given */
}
srch_dump_isr_vars_struct_type;
  /* Search status dump ISR variables */

typedef struct
{
  int4 current_pos;
    /* Current "reported" searcher position */

  int4 raw_pos;
    /* Raw searcher position */

  int4 expected_pos;
    /* Expected searcher position */

#if defined(FEATURE_RELATIVE_DMOD_POS) || defined(FEATURE_IS2000)
  int2 pn_offset;
    /* The cumulative offset PN mask */

  int4 pos_offset;
    /* The cumulative offset position in chipx8 */

  int2 offset_nghbr_slew;
    /* The amount of slew required for neighbor search offset */
#endif /* FEATURE_RELATIVE_DMOD_POS || FEATURE_IS2000 */
}srch_position_data_type;


/*--------------------------------------------------------------------------
                Declaration of 16x variables
--------------------------------------------------------------------------*/

/* The following type is used to specify which searcher to query or affect
   when calling function which could affect both h/w searchers.  It is
   generally used as an input to the function. */
typedef enum
{
  SRCH_8X_SEARCHER,     /* 8x searcher */
#ifdef FEATURE_SEARCH2
  SRCH_16X_SEARCHER,    /* 16x searcher */
#endif /* FEATURE_SEARCH2 */
  SRCH_NO_SEARCHERS     /* neither searcher */
}
srch_hw_search_type;

#define SRCH2_HW_Q_LEN  8
  /* The current size of the SEARCH2 h/w queue */

#if defined(FEATURE_GPSONE) && defined(FEATURE_SRCH_SCHEDULER)
#error code not present
#else
#define SRCH2_SW_Q_LEN  42
  /* Length of the s/w queue used to store search parameters and results.
     Current lenght of 42 is based on a max of 40 neighbours plus 2 active
     set entries. */
#endif /* FEATURE_GPSONE && FEATURE_SRCH_SCHEDULER */


#define ENABLE_DUMP     0x8000
  /* Used to mask on/off the dump enable bit when programming searches */

#ifdef FEATURE_SEARCH2
typedef struct
{
  boolean active;
    /* Set when the 16x searcher is active */

  boolean continue_srch;
    /* This is normally TRUE so that the DUMP ISR will continue searching the
       next available searches on the s/w queue, but in the case of
       srch_sect_clr or srch_sect_stop, we want the h/w to stop searching,
       so set this to FALSE so the ISR knows better */

  boolean last_srch_dmp;
    /* Keep track of whether or not the last search programmed on the 16x
       hardware is going to dump or not.  This info is useful when calling
       srch_sect_clr(), so the function knows whether or not to add an
       extra search to allow all the valid searches to complete and results
       get read */

  /* NOTE: the following indices are not updated in real time, but only
     at specific times, by software.  The hardware pointers themselves
     are updated in real time, but these soft copies of the pointers
     must be updated manually. */
  byte    q_write_inx;
    /* The next place on the hardware queue that we will put the search
       parameters for the searcher */
  byte    q_read_inx;
    /* The next place on the hardware queue that the searcher will read from
       to get the search parameters */
  byte    q_read_sub_inx;
    /* The next place on the hardware queue that the searcher will read from
       to get the search parameters - sub-index for each q_read_inx */
  byte    res_write_inx;
    /* The next place on the hardware queue that the searcher will put the
       latest search results */
  byte    res_write_sub_inx;
    /* The next place on the hardware queue that the searcher will put the
       latest search results - sub-index for each res_write_inx */
  byte    res_read_inx;
    /* The next position from which the software will read results off the
       hardware queue */

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#endif /* FEATURE_SRCH_SCHEDULER */
}
srch_search2_info_type;
#endif /* FEATURE_SEARCH2 */

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#endif /* FEATURE_SRCH_SCHEDULER */

typedef struct
{
  q_link_type  link;
    /* Link for putting this buffer on a queue. */

  srch_set_enum_type  set;
    /* Which set the sector is currently assigned to */

  boolean                age;
  int2                   pilot;
  byte                   nc_int;
  byte                   chps;
  byte                   gain;
#ifdef FEATURE_SRCH_SLEW_IN_DRV
#error code not present
#else
  int4                   slew;
#endif
  int4                   winsiz;
    /* The above parameters are the standard parameters used to program
       a search.  they should be calculated ahead of time and stored in
       this structure, so that the searches can be quickly loaded into
       the queue */

  boolean                dump;
    /* An extra field for the 16x searcher which signals whether or not
       we want to receive a dump interrupt after this particular search */

  int4                   exp_pos;
    /* The expected position of the searcher after it has completed this
       search on the queue, assuming no interruptions to the searcher */

  srch_sect_struct_type  *sect_ptr;
    /* Pointer to the original sector structure which this search
       belongs to.  ie. if the parameters in this search belong to
       srch_aset[ 0 ], then *sect_ptr = srch_aset[ 0 ]  This is used
       to place the final results in the correct sector structure */

  int2  max_inx[ SRCH_MAX_PEAKS ];
    /* Index in the sweep results of the maximum energy path
       in 1/8 chip units. */

  word  max_eng[ SRCH_MAX_PEAKS ];
    /* Energy of the maximum energy path */

#ifdef FEATURE_SRCH_4_SHOULDERS
#error code not present
#endif
}
srch_sect_cmd_struct_type;

typedef struct
{
  srch_sect_cmd_struct_type  parms[ SRCH2_SW_Q_LEN ];
    /* This is the software queue which is used to hold the search parameters
       for programming queued searches and is also used to hold the results
       after a queued search is finished.  The parameters are first written
       into this structure at the task level and then the hardware is
       programmed at either the task level or ISR level.  Results are
       transferred from the hardware into this structure at the ISR level,
       then the task level moves the results from here and into the proper
       sector structures */

#ifdef FEATURE_SEARCH2
  byte                       slot_total;
    /* Total number of searches put on the queue for the most recent slot
       (used as a metric for performance and for debug) */
#endif /* FEATURE_SEARCH2 */
}
srch_search_buf_struct_type;


/*--------------------------------------------------------------------------
                         Function-like Macros
--------------------------------------------------------------------------*/

/* Absolute value */
#ifdef ABS
#undef ABS
#endif
#define  ABS( x ) ( ((x) < 0) ? -(x) : (x) )
#define LABS( x ) ( ((x) < 0) ? -(x) : (x) )
  /* Keep separate, just to realize the difference in types where used */

/* Clear the rf-tune-done signal, and return whether it was set */
#define  RF_TUNE_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_RF_TUNE_TIMER_SIG ) & \
                              SRCH_RF_TUNE_TIMER_SIG )

/* Check to see if this neigbor is on the current CDMA frequency */
#define CURRENT_FREQ_NEIG( i ) \
                   ( (srch_nset[ i ].freq_incl == FREQ_INCL_NO_FREQ) || \
                    ((srch_nset[ i ].freq_incl == FREQ_INCL_FREQ) && \
                     ( (srch_nset[ i ].cdma_freq == srch_cdma_freq) && \
                       (srch_nset[ i ].band_class == srch_cdma_band_class))))

#if (defined(FEATURE_IS95B_ACCESS_ENTRY_HO) || \
     defined(FEATURE_IS95B_ACCESS_HO))

/* Check to see if this neighbor is allowed to do Idle Handoff */
#define IDLE_HO_ALLOWED_NEIG( i ) \
                  ( ((srchpc_acc_state == SRCHPC_ACC_ENTRY) && \
                     (srch_nset[ i ].access_entry_ho) ) || \
                     ((srchpc_acc_state == SRCHPC_ACC_PROBE) && \
                         (srch_nset[ i ].access_ho_allowed))  || \
                       (srchpc_acc_state == SRCHPC_ACC_IDLE) )
#endif

/* Kick the watchdog */
#define  DOG_RPT() \
  dog_report(DOG_SRCH_RPT); \
  (void) rex_set_timer ( &srch_rpt_timer, SRCH_RPT_TIME )

/* Clear the sweep-done signal and return whether it was set */
#define  DUMP_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_DUMP_ISR_SIG ) & SRCH_DUMP_ISR_SIG )

/* Clear the 26ms PN roll signal and return whether it was set */
#define  ROLL_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_ROLL_ISR_SIG ) & SRCH_ROLL_ISR_SIG )

/* Clear the STOP signal and return whether it was set */
#define  STOP_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_STOP_SIG ) & SRCH_STOP_SIG )

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#else

/* Clear the LOST_DUMP signal and return whether it was set */
#define  LOST_DUMP_TIMER_EXPIRED()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_LOST_DUMP_SIG ) & SRCH_LOST_DUMP_SIG )
#endif /* FEATURE_SRCH_SCHEDULER */ 

/* Clear the search speed governor signal and return whether it was set */
#define  GOVERN_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_GOVERN_SIG ) & SRCH_GOVERN_SIG )

/* Clear the Wakeup signal and return whether it was set */
#define  WAKEUP_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_WAKEUP_SIG ) & SRCH_WAKEUP_SIG )

/* Clear the FAST_SLEEP signal and return whether it was set */
#define  FAST_SLEEP_CMD() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_FAST_SLEEP_SIG ) & SRCH_FAST_SLEEP_SIG )

/* Clear the ACQ_TIMER signal and return whether it was set */
#define  ACQ_TIMER_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_ACQ_TIMER_SIG ) & SRCH_ACQ_TIMER_SIG )

/* Clear the RSSI_TIMER signal and return whether it was set */
#define  SRCH_RSSI_TIMER_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_RSSI_TIMER_SIG ) & SRCH_RSSI_TIMER_SIG )

/* Clear the FREQ_TIMER signal and return whether it was set */
#define  FREQ_TIMER_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_FREQ_TIMER_SIG ) & SRCH_FREQ_TIMER_SIG )

/* Clear the DELAY_TIMER signal and return whether it was set */
#define  DELAY_TIMER_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_DELAY_TIMER_SIG ) & SRCH_DELAY_TIMER_SIG )

/* Clear the REF_TIMER signal and return whether it was set */
#define  REF_TIMER_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_REF_TIMER_SIG ) & SRCH_REF_TIMER_SIG )

/* Clear the CTR_TIMER signal and return whether it was set */
#define  CTR_TIMER_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_CTR_TIMER_SIG ) & SRCH_CTR_TIMER_SIG )

/* Clear the PC_HO_TIMER signal and return whether it was set */
#define  PC_HO_TIMER_DONE()\
  ( rex_clr_sigs ( &srch_tcb, SRCH_PC_HO_TIMER_SIG ) & SRCH_PC_HO_TIMER_SIG )

/* Clear the PC_THROT_TIMER signal and return whether it was set */
#define  PC_THROT_TIMER_DONE()\
  ( rex_clr_sigs(&srch_tcb,SRCH_PC_THROT_TIMER_SIG) & SRCH_PC_THROT_TIMER_SIG )

/* Clear the OFFLINE signal and return whether it was set */
#define  OFFLINE_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_OFFLINE_SIG ) & SRCH_OFFLINE_SIG )

#ifdef FEATURE_IS95B_PPSMM
/* Clear the PPSMM signal and return whether it was set */
#define  PPSMM_TIMER_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_PPSMM_TIMER_SIG ) & SRCH_PPSMM_TIMER_SIG )
#endif

#ifdef FEATURE_QUARTER_ROLL_TIMELINE
#error code not present
#endif

#ifdef FEATURE_IS2000

#ifdef FEATURE_IS2000_QPCH
/* Clear the OOK decode signal */
#define  OOK_DONE() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_OOK_SIG ) & SRCH_OOK_SIG )

/* Sign extend the OOK measurements */
#define OOK_SIGN_EXTEND( x ) \
  if ( x > 1023 ) { x -= 2048; }
#endif /* FEATURE_IS2000_QPCH */

/* Clear the Action Time signal and return whether it was set */
#define  ACTION_TIME() \
  ( rex_clr_sigs ( &srch_tcb, SRCH_ACTION_TIME_SIG ) & SRCH_ACTION_TIME_SIG )
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/* Decide if the handoff drop timer (T_TDROP) for a given pilot has expired.
   It uses the current value of T_TDROP, which is required by IS-95
   section 6.6.6.2.3. Arguments: "trpt" = The trpt struct for that sector,
   which contains the relevant dropout timer in 26.66..ms units. "time" = The
   current time in 26.66..ms units */
#define DROP_TIMER_EXPIRED(trpt,time) \
  ((trpt.drop_timer+srch_parms.t_tdrop) <= time)

/* Convert a gain and a chip count into an RSSI value */
#define  RSSI_SCALE( gain, chps ) \
  ( ((dword) gain * (dword) gain) * ((dword) chps * (dword) chps) )
  
/* Common RSSI conversion factor: Gain=2, Chips=512 */
#define  RSSI_SCALE_2_512     (dword)1048576L

/* Convert a RSSI value to an energy */
#define  RSSI_TO_ENG( rssi, scale ) \
  ((word) ( ((dword) (rssi) * (dword) (scale)) / 65536 ))

/* Convert an energy to a RSSI value */
#define  ENG_TO_RSSI( eng, scale ) \
  ( ((dword) (eng) * 65536) / (scale) );

/* Runs an "(n-1)/n" style IIR filter. */
#define  FILT_N( val, new, n ) \
  ((word) (( ((dword) (val) * ((n)-1)) + (dword) (new) ) / (n)))

/* Runs an "(n-1)/n" style IIR filter on INT4 quantities */
#define  FILT_I4( val, new, n ) \
  ((int4) (( ((int4) (val) * (((int4)n)-1)) + (int4) (new) ) / ((int4)n)))

/* Reduces a specified value by a specified number of 1/2 db steps. */    
#define  DB_DOWN( val, db ) \
  ((word) (((dword) ((val) * (dword) srch_db_down_tab[ (db) ]) + 128) / 256))

/* Increment an unsigned word, saturating at FFFF */
#define  INCW_MAX( v ) ((v) = ((v) < 65535) ? (v)+1 : (v))

/* Increment a signed int, saturating at 7FFF */
#define  INCI_MAX( i ) ((i) = ((i) < 32767) ? (i)+1 : (i))

#define  SIGNX( val ) \
  /*lint -e740  Yes, this is an unusual pointer cast */ \
  W_PTR ( val )[ 1 ] = srch_sign_words[ W_PTR ( val )[ 1 ] & 0x3 ];\
  /*lint +e740  */

#ifdef SRCH_VERBOSE
  #define SRCH_DEBUG_MSG(str,arg0,arg1,arg2) MSG_LOW(str,arg0,arg1,arg2)
#else
  #define SRCH_DEBUG_MSG(str,arg0,arg1,arg2)
#endif

#ifdef FEATURE_SEARCH2
/* Determine if the h/w queue is full already */
#define  SRCH2_HW_Q_FULL( write_inx, read_inx ) \
  (((byte) write_inx + 1) % SRCH2_HW_Q_LEN == (byte) read_inx)
  /* Add 1 so that we can actually start searches when the ptrs
     are reset - this also provides a buffer of one search for
     this case */

/* Return whether or not the 16x searcher dumped */
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )
#define  SEARCH2_DUMP( check ) \
  ( (check) & SEARCH_STATUS_DUMP_STATUS_RB__SEARCH2_DUMP_EN_MASK )
#else
#define  SEARCH2_DUMP( check ) \
  ( (check) & SRCH_STATUS_DUMP_STATUS_RH__SEARCH2_DUMP_STATUS_MASK )
#endif /* T_MSM5000 */
#endif /* FEATURE_SEARCH2 */

/* Return whether or not the 1x/8x searcher dumped */
#ifdef T_MSM_5100
#error code not present
#elif defined( T_MSM5000 )
#define  SEARCH1_DUMP( check ) \
  ( (check) & SEARCH_STATUS_DUMP_STATUS_RB__SEARCH1_DUMP_EN_MASK )
#else
#define  SEARCH1_DUMP( check ) \
  ( (check) & SRCH_STATUS_DUMP_STATUS_RH__SEARCH1_DUMP_STATUS_MASK )
#endif /* T_MSM5000 */
  
#define SLOT_INACTIVE() \
  ( (srch_slot_mask) == 0 )
  /* True if all slot mask bits have been cleared - we are no longer waiting
     for a slot to finish */

#define UPDATE_SLOT_MASK( mask ) \
  ( srch_slot_mask &= ~(mask) )
  /* Clear the bits in "srch_slot_mask" as specified by "mask" */


/*--------------------------------------------------------------------------
                           Regional Variables

   These variables are used all over, so it's more convenient to
   declare them here than in each module.
--------------------------------------------------------------------------*/

extern srch_position_data_type srch_8x_data;
  /* Keep track of the 8x searcher offset data */

#ifdef FEATURE_SEARCH2
extern srch_position_data_type srch_16x_data;
  /* Keep track of the 8x searcher offset data */
#endif /* FEATURE_SEARCH2 */

#ifdef FEATURE_SRCH_SLEW_IN_DRV
#error code not present
#endif

extern srch_search_buf_struct_type srch_search_buf;
  /* Used to hold searches to be put on the queue */

#ifdef FEATURE_SEARCH2
extern srch_search2_info_type srch2;
  /* Used to keep track of some h/w queue parameters */

extern boolean srch_active_search_done;
  /* Used to remember if we have finished an active search in one of the
     queued searches so that we can do triage on it */
#endif /* FEATURE_SEARCH2 */

#ifdef FEATURE_IS95B_MAHHO
extern word srch_max_off_time_fwd;
  /* Maximun off time of CFS search (one visit) */

extern word srch_total_off_time_fwd;
  /* Total off time of CFS search (all visits) per period */

extern word srch_msm_srch_speed;
  /* The MSM hardware search speed. Default is 8x for CFS */

#endif

extern srch_govern_struct_type srch_govern;
  /* Used to monitor and control searcher rate and overall task loading. */

extern srch_state_enum_type  srch_state;
  /* Keeps track of the current state of the Searcher Task state machine. */

extern srch_parm_struct_type  srch_parms;
  /* Parameters that control the behavior of the searcher task.
     These are settable by the base station via MCC */

extern srch_ta_parm_struct_type  srch_ta_parms;
  /* This structure keeps track of what is to be sent to the
     temporal analyzer. */

extern byte  srch_cdma_band_class;
  /* The current CDMA band class */

extern word  srch_cdma_freq;
  /* The current CDMA Channel number */

extern word  srch_fing_cnt;
  /* The number of fingers available in the MSM (3 for MSM2.2, 4 for MSM2.3 */

extern word  srch_fingers;
  /* The number of unreserved fingers usable in the searcher.  During the idle
     paging mode in MSM2.3+, the last finger may be reserved for a 6.67ms
     delayed timeline generation. */

#ifdef FEATURE_QUARTER_ROLL_TIMELINE
#error code not present
#endif /* FEATURE_QUARTER_ROLL_TIMELINE */

extern word  srch_delayed_fing;
  /* Finger on which 6.67ms delayed PN rolls are created */

extern word  srch_track_fing;
  /* Finger which the delayed finger is tracking. */

extern srchzz_wakeup_state_type srchzz_wakeup_state;
  /* Wakeup state relative to slot boundary based on PN rolls. */

extern ulpn_type srch_slam_ulpn;
  /* ULPN value to set to generator at slam point t=-20ms. */

extern ulpn_type srch_slot_ulpn;
  /* ULPN value to set to generator at slam point t=0ms). */

extern delayed_fing_state_type srch_delayed_fing_state;
  /* Used to prevent slew commands when finger is already slewing */

/* Virtual symbol combiner ... */

extern boolean srch_comb_slam_pending;
  /* This flag indicates a discontinuity in the symbol combiner timing
     is about to occur. */

extern word srch_comb_pre_slam_phase;
extern word srch_comb_post_slam_phase;
  /* Symbol combiner phase before and after a slam */

extern word srch_comb_offset;
extern word srch_comb_post_slam_offset;
  /* Value to add to symbol combiner position to get "true" time.
     (in 2-chip units) */

extern word srch_comb_post_slam_tt_fing;
  /* The finger to timetracking after slam a slam */



extern int4 srch_total_sleep_dur;
  /* Total sleep duration, in chips.  This value is incremented at the
     start of each catnap in the sleep series */

extern word srch_catnap_ctr_zz;
  /* The number of catnaps remaining in the current slotted sleep. */

extern dword srch_catnap_ctr_dz;
  /* The number of catnaps remaining in the current deep sleep. */

/*---------------------------------------------------------------------------
                 Regional Enhanced-Standby, Phase II Defines
---------------------------------------------------------------------------*/
#ifdef FEATURE_SEARCH2_IDLE
#undef SRCHZZ_ISR_SEARCHER
  /* This feature is not used when the 16x searcher is being used */
#else
#define SRCHZZ_ISR_SEARCHER
  /* We want the ZZ list searcher to operate in the 
     interrupt context rather than at task level. 
     This gives greatest searching rate. */
#endif /* FEATURE_SEARCH2 */

extern boolean srchzz_list_search_active;
extern boolean srchzz_fast_raho_pending;
extern void srchzz_list_srch( void ); 
  /* Externs which the ISR needs to do work
     on behalf of ZZ state */ 

extern srch_sect_parm_struct_type  srchpc_sect_parm;
  /* PC state normal search parameters */

extern word srchpc_last_slot_eng;
  /* Energy of the pilot the last time we slept. */

extern boolean srch_age_searches;
  /* Regional flag indicating whether or not we wish to 
     mark searches as having been searched by the search counter */

extern int4 srchtri_delayed_fing_posn;
  /* Location we last programmed the delayed finger to.  If the
     finger is not where we expect it to be, then a slew operation
     must still be in progress. */

extern int4    srch_reacq_mstr_posn;
  /* Raw position of the location of the multipath to use as the
     new mobile station timing reference. */

#ifdef FEATURE_IS95B_SYS_RESELEC
extern boolean srchpc_rssi_rpt;
  /* Flag remembering if we have reported on insufficient RSSI */
#endif

#ifdef SRCH_ENABLE_SD20          
#error code not present
#endif /* SRCH_ENABLE_SD20 */

#ifdef FEATURE_IS2000
/*---------------------------------------------------------------------------
                       Regional IS-2000 Definition
---------------------------------------------------------------------------*/
extern boolean srch_action_time_ho;
  /* Flag indicating if we have received a handoff command and need to 
     prepare for a soft handoff at the action time */

extern boolean srch_reset_fing_frame_cnt;
  /* Flag indicating if we need to reset finger frame counters */

extern boolean srch_implicit_action_time;
  /* Flag indicating if the action time is implicit, i.e., in the past or 
     now */ 

#ifdef FEATURE_IS2000_SCH
extern boolean srch_sch_trans;
  /* Flag indicating if we need to process an SCH transaction */

extern boolean srch_implicit_sch_trans_time;
  /* Flag indicating if the SCH transaction time is implicit, i.e., in the
     past or now*/ 

extern boolean srch_ho_sch_at_same_time;
  /* Flag indicating if handoff and SCH transaction have the same
     action or transaction time */
#endif /* FEATURE_IS2000_SCH */

extern boolean srch_fpc_enabled;
  /* Keeps track of the FPC mode during TC */

extern int2 const srch_win_offset_tab[ ];
  /* The search window offset multiplier table */

extern boolean srch_hho_in_progress;
  /* Indicate whether HHO is in progress or not. This is used for
     window offset search */

#ifdef FEATURE_IS2000_QPCH
/*---------------------------------------------------------------------------
         Variables and Constants pertaining to QPCH
---------------------------------------------------------------------------*/

extern mccqpch_qpch_info_type srch_qpch_info;
  /* Used to maintain the general QPCH info - basically everything about
     the current QPCH except PI bit positions.  This must be updated for
     each active set pilot, in idle mode. */

extern mccqpch_pi_pos_type srch_qpch_pi_pos[ ];
  /* Maintains the QPCH PI bit positions for 4800 and 9600 rate QPCH.
     This must be updated for each QPCH slot. */

extern mccqpch_status_type srch_qpch_status;
  /* The return type from MC when updating the QPCH info.  This will
     indicate whether or not the QPCH is supported and if the data
     is valid or not. */

extern mccqpch_pch_framing_type srch_qpch_pch_framing;
  /* The return type from another MC function call if QPCH is not
     supported.  This will indicate 95B or pre-95B framing for
     PCH frames, which can be used to optimize the sleep timeline. */

extern mccqpch_pch_framing_type srch_prev_qpch_pch_framing;
  /* Indicates the Paging Channel framing type before any change during
     the current slot */

extern int4 srchzz_last_catnap;
  /* Length of the last catnap in odd catnap unit */

extern int4 srch_early_sleep_till_roll;
  /* Time from early sleep at a manually-generated roll till the
     next symbol combiner roll that would have happened. In chips */

#ifdef FEATURE_UP_CLOCK_SWITCHING 
#error code not present
#endif /* FEATURE_UP_CLOCK_SWITCHING */

/*-----------------------------------------------------------------------------
                             Catnap States

   Used to identify various catnap types.
-----------------------------------------------------------------------------*/

typedef enum
{
  SRCHZZ_REGULAR_CATNAP,         /* Regular catnap                           */
  SRCHZZ_CATNAP_BEFORE_PI1,      /* The catnap just before PI1               */
  SRCHZZ_CATNAP_BETWEEN_PI_BITS, /* The catnap between PI1 and PI2           */
#ifdef FEATURE_IS2000_QPCH_CCI
  SRCHZZ_CATNAP_AFTER_PI2,       /* The catnap between PI2 and slot boundary */
  SRCHZZ_CATNAP_BEFORE_CCI       /* The catnap between a PI bit and a CCI bit*/
#else
  SRCHZZ_CATNAP_AFTER_PI2        /* The catnap between PI2 and slot boundary */
#endif /* FEATURE_IS2000_QPCH_CCI */
}
srchzz_catnap_state_type;

extern srchzz_catnap_state_type srchzz_catnap_state;
  /* Catnap state we are in */

typedef struct 
{
#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

  srch_ook_bits_type ook_bits;
    /* Storage for OOK data and bit decisions */

#ifdef FEATURE_IS2000_QPCH_CCI
  srch_ook_bits_type cci_bits;
    /* Storage for CCI data and bit decisions */
#endif /* FEATURE_IS2000_QPCH_CCI */

  word               ook_pn_word;
    /* PN word of OOK bit (0 to 511) relative to PN roll */

  word               ook_sc_pos;
    /* Symbol Combiner position value at end of OOK bit.  The h/w expects
       OOK bit specified in 1 PN roll range.  This allows increasing the
       range to 3 PN rolls, for PLT. */
  
  int4               ook_chips_from_slot;
    /* Number of chips from the beginning of the QPCH slot to the OOK bit */
  
  word               attempts;
    /* Detection of catastrophic failure.  We only allow 3 attempts,
       corresponding to the 3 PN phases. */
} srch_qpch_var_type;

extern srch_qpch_var_type srch_qpch_var;
  /* QPCH data and decisions */

extern word srch_decode_pi_bit;
  /* Indicate which PI bit to decode: 0 for bit 1; 1 for bit 2 */

extern boolean srch_qpch_enabled;
  /* Indicate whether QPCH is enabled/disabled */

#ifdef FEATURE_IS2000_QPCH_CCI
extern boolean srch_decode_cci_bit;
  /* Indicates whether we are decoding CCI bit */

extern ulpn_type srch_ulpn_80ms_after_slot;
  /* ULPN value to set to generator for slam at 80 msec after the Paging 
     Channel slot boundary. */
#endif /* FEATURE_IS2000_QPCH_CCI */

#endif /* FEATURE_IS2000_QPCH */
#endif /* FEATURE_IS2000 */

#if defined( FEATURE_ANTIREGISTRATION_BIAS ) || defined ( FEATURE_IS2000_QPCH )
extern mcc_srch_nbr_info_type srch_nbr_info;
  /* Used to store the overhead info status for the given neighbour */
#endif /* FEATURE_ANTIREGISTRATION_BIAS || FEATURE_IS2000_QPCH */

extern byte srchzz_sci;
  /* Slot cycle index in use for this sleep period
     As per IS-95:     0 =   1.28 seconds (  16 x 80ms),
                       1 =   2.56 seconds (  32 x 80ms), ...
                       7 = 163.84 seconds (2048 x 80ms) */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*--------------------------------------------------------------------------*/
/* SRCHTC SPECIFIC TYPE DEFINITIONS                       */
/*--------------------------------------------------------------------------*/
typedef enum          /* Define values for the substate */
{
  SRCHTC_SS0_INIT,         /* Punt */
  SRCHTC_SS1_ASET,         /* Process sweep of Active Set member */
  SRCHTC_SS2_CSET,         /* Process sweep of Candidate Set member */
  SRCHTC_SS3_NSET,         /* Process sweep of Neighbor Set member */
  SRCHTC_SS4_RSET,         /* Process sweep of Remaining Set member */
  SRCHTC_SS5_NREV          /* Process revisit of interesting Neighbor */
#ifdef FEATURE_IS95B_MAHHO
  ,SRCHTC_SS6_CFNSET       /* Process sweep of Candidate Freq. Set member */
#endif /* FEATURE_IS95B_MAHHO */
}
srchtc_substate_enum_type;

#ifdef FEATURE_IS95B_MAHHO
  typedef enum          /* Define values for the hard handoff substate */
{
  SRCHTC_HHO_DONE,         /* No hard handoff in progress */
  SRCHTC_HHO_ACQ,          /* Acquiring another frequency pilot */ 
  SRCHTC_HHO_CONFIRM,      /* Waiting hard handoff success signal */
  SRCHTC_HHO_ABORT,        /* Aborting hand hanoff due ec/io < thr */
  SRCHTC_HHO_CFSRCH        /* Waiting CF Nset search to be finished */
}
srchtc_hho_state_enum_type;

typedef enum          /* Define values for the CFS substate */
{
  SRCHTC_CFS_SF,           /* In Serving Frequency Now */
  SRCHTC_CFS_CF,           /* In Candidate Frequency Now */
  SRCHTC_CFS_TUNING        /* It's tuning the channel now */
}
srchtc_cfs_state_enum_type;
#endif /* FEATURE_IS95B_MAHHO */

/*--------------------------------------------------------------------------*/
/* SRCHTC EXTERN VARIABLES                                                  */
/*--------------------------------------------------------------------------*/
extern int2  srchtc_last_center_slew;
  /* Individual pilot centering slew memory */

#ifdef FEATURE_IS95B_MAHHO
extern srchtc_hho_state_enum_type srchtc_hho_state;
  /* Hard handoff sub state. */
extern srchtc_cfs_state_enum_type srchtc_cfs_state;
  /* State of Candidate Frequency Search */
extern srch_timer_type srchtc_cfs_visit_timer;
  /* Visit timer */
#endif /* FEATURE_IS95B_MAHHO */

/*===========================================================================

FUNCTION SRCHTC_NEXT_SECTOR

DESCRIPTION    This function figures out what sector to sweep next
      with the searcher, and initiates the sweep.  It imposes the
      following schedule on the searching sequence:
         1. All Active Set sectors
         2. All Candidate sectors
         3. All Pre-candidate Set sectors
         4. One Neighbor or Remaining Set sector

      The Neighbor or Remaining Set sector cycles through the entire
      Neighbor Set, then picks one sector from the Remaining Set, if
      we determine it is time to search Remaining pilots, then repeats.

DEPENDENCIES   A sweep must not be in progress when this function is
      called.

RETURN VALUE   New state of the TC state machine, indicating what kind
      of sector is currently being swept:
            SRCHTC_SS1_ASET      Active Set sector
            SRCHTC_SS2_CSET      Candidate or Pre-Candidate Set sector
            SRCHTC_SS3_NSET      Neighbor Set sector
            SRCHTC_SS4_RSET      Remaining Set sector

SIDE EFFECTS   Various demod chip registers are written.

===========================================================================*/

extern srchtc_substate_enum_type srchtc_next_sector
(
  word  cur_substate,
    /* The current state of the TC state machine */

  srch_sect_struct_type  **sect_ptr,
    /* Pointer to a srch_sect_struct_type* variable which contains:
       On entry: pointer to the last sector swept
       On exit:  pointer to the sector we just started to sweep */

  int4  *slew
    /* Additional slew to impose on the search window. */
);

#if defined(FEATURE_SEARCH3) && defined(FEATURE_SRCH_BATCH_LOG)
#error code not present
#endif /* defined(FEATURE_SEARCH3) && defined(FEATURE_SRCH_BATCH_LOG) */

/*===========================================================================

FUNCTION SRCH_ZZ_COMPUTE_NEXT_SLOT

DESCRIPTION
  This function calculates the next assigned slot boundary.

DEPENDENCIES
  Many

RETURN VALUE
  None

SIDE EFFECTS
  srchzz_slot_cycle_length & srchzz_wakeup_time are modified.

===========================================================================*/

extern void srch_zz_compute_next_slot ( void );

/*===========================================================================

FUNCTION SRCH_ZZ_COMPUTE_SCI

DESCRIPTION
  This function calculates the desired SCI based on the mobile's allowed
  value and the BS's desired value.

DEPENDENCIES

RETURN VALUE
  The SCI that will be used.

SIDE EFFECTS

===========================================================================*/

extern byte srch_zz_compute_sci
(
  srch_sleep_cmd_type  *cmd_ptr
    /* Pointer to command which is causing us to enter this state. */
);

#ifdef FEATURE_DH
/*===========================================================================

FUNCTION SRCH_START_RAND_BUF_FILL

DESCRIPTION    This function sets the appropriate flag to start filling the
      buffer with random bits, necessary for DH task.  It also grabs the
      start time of the process.

DEPENDENCIES   None.

RETURN VALUE   Good status.

SIDE EFFECTS   None.

===========================================================================*/
extern srch_cmd_status_type srch_start_rand_buf_fill ( void );
#endif /* FEATURE_DH */

/*===========================================================================

FUNCTION SRCH_SET_SLOT_STATUS

DESCRIPTION    This function checks the time of all the upcoming slots versus
      the given slot_time and sets the appropriate bits if any of them match. 

DEPENDENCIES   None.

RETURN VALUE   The new bits that have been set from the most recent call.

SIDE EFFECTS   srch_slot_mask is updated, which may generate a wakeup report.

===========================================================================*/
extern byte srch_set_slot_status
(
  qword slot_time
    /* "slot time" of the next slot */
);

/*===========================================================================

FUNCTION SRCH_GET_SLOT_STATUS

DESCRIPTION    This function checks whether or not the mobile is possibly
      in an important slot.  An "important slot" meaning either an assigned
      paging slot, a BC SMS paging slot, or a BC SMS data slot.  To do this,
      the time must also check at least 1 slot after and 1 slot before
      for all the slots, including 2 slots after for the BC data slot.

      The last BC data slot does not need to be kept track of or checked,
      because it is not updated until the previous BC data slot has ended.
      This is different from the other slots which are updated calculted
      within SRCH context (not passed from MC), and may get updated before
      the actual slot has ended.

DEPENDENCIES   None.

RETURN VALUE   TRUE if in the assigned paging slot, +/- 1 slot.
               TRUE if in the BC page slot, +/- 1 slot.
               TRUE if in the BC data slot, -1 slot or +2 slots.
               FALSE otherwise.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_get_slot_status ( void );

#ifdef T_MSM5000
/*===========================================================================

FUNCTION SRCH_FING_CHAN_ENABLE

DESCRIPTION    This function enables the desired finger channel for a given
      set of fingers.

DEPENDENCIES   srch_dmod_reset should have been called.

RETURN VALUE   None.

SIDE EFFECTS   Finger registers are written.

===========================================================================*/

extern void  srch_fing_chan_enable
(
  byte fing_mask,
    /* Mask for specifying the finger to access */

  byte chan_mask
    /* Mask for speficying which channel to set */
);


/*===========================================================================

FUNCTION SRCH_COMB_CHAN_ENABLE

DESCRIPTION    This function enables the desired combiner code channel.

DEPENDENCIES   srch_dmod_reset should have been called.

RETURN VALUE   None.

SIDE EFFECTS   Combiner register is written.

===========================================================================*/

extern void  srch_comb_chan_enable
(
  byte chan_mask
    /* Mask for speficying which channel to set */
);
#endif /* T_MSM5000 */

/*===========================================================================

FUNCTION SRCH_GENERATE_EPOCH

DESCRIPTION    This function generates an EPOCH, which establishes an
               arbitrary zero-offset time reference.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   PN generates, fingers, searcher, PDM's etc initialized.

===========================================================================*/

extern void  srch_generate_epoch ( void );


/*===========================================================================

FUNCTION SRCH_RANGE_SLEW

DESCRIPTION    This function makes sure a particular slew amount is
      within the allowable + - 32768-chip range.

DEPENDENCIES   None.

RETURN VALUE   The range-justified slew.

SIDE EFFECTS   None.

===========================================================================*/
extern int4 srch_range_slew 
(
  int4 slew
    /* The slew to keep within good range */
);

/*===========================================================================

FUNCTION SRCHPC_UPDATE_NSET

DESCRIPTION    This function is called to handle a Neighbor Set Update
      command when on the Paging Channel.  The command contains a list of
      pilot offset values for sectors that might be neighbors.  We will
      copy this list into our neighbor set list (keeping the trpt values
      for any we already had, initializing trpt values for new ones)
      except that we discard any duplicates and any that have the same
      pilot offset as the active sector.

DEPENDENCIES   srchpc_*, srch_nset

RETURN VALUE   Command return status.  Always SRCH_DONE_S (good status).

SIDE EFFECTS   The nset array is updated.

===========================================================================*/
extern word  srchpc_update_nset
(
  srch_nset_cmd_type  *cmd_ptr
    /* Pointer to the Neighbor Set Update command that got us here */
);


/*===========================================================================

FUNCTION SRCH_SECT_SEARCH

DESCRIPTION    This function is an interface to the appropriate "cmd"
      function for programming a search in the searcher hardware.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   

===========================================================================*/

extern void  srch_sect_search ( void );


/*--------------------------------------------------------------------------
         Akey rand bits variables
--------------------------------------------------------------------------*/
#ifdef FEATURE_DH

extern boolean srch_rand_fill_buf;
  /* Set if MC wants a buffer filled for seed
     Put this here so that we can poke it from DM */

extern qword srch_akey_start_time;
extern qword srch_akey_end_time;
extern qword srch_akey_delta_time;
#endif

/*--------------------------------------------------------------------------
         PCS specific Regional Variables
---------------------------------------------------------------------------*/

extern word srch_pri_mask;
  /* The priority mask for extended neighbor list searches */


#ifdef FEATURE_JSTD008_DEBUG

#define SRCH_DEBUG_INX_MAX  200
  /* Number of index traces we want to keep */

typedef struct
{
  byte pri_mask;
  byte neig_inx;
  byte pri_neig_cnt;
  byte pc_done;
} srch_pri_scan_debug_struct_type;

extern word  srch_debug_inx;

extern srch_pri_scan_debug_struct_type srch_pri_debug_struct[];
  /* Neig index of neigs scanned to determine scan order */

extern dword srch_pri_scan_rate[];
  /* Scan rate statistics for prioritized neighbors */

#endif  /* FEATURE_JSTD008_DEBUG */

extern byte  srch_freq_est;
  /* Our most recent estimate of the proper value for the CAR_FREQ register */

extern word  srch_temp_est;
  /* Our most recent temperature estimate */

extern int2  srch_win_siz;
  /* Default search window size in chipx8 units. Over-air parms will
     supercede this value. */

extern int2  srch_ref_pilot;
  /* Record of which pilot offset is the time reference */

extern word  srch_ref_inx;
  /* Reference pilot Active set index number */

extern int2  srch_agc_rssi;
  /* Regional variable holding AGC RSSI (appx -105dBm.. scale) */

extern nv_cmd_type  srch_nv_cmd;
  /* Buffer to send command to NV */

extern nv_item_type  srch_nv_item;
  /* Buffer to place NV items in */

extern srch_dump_isr_vars_struct_type  srch_dump_isr_vars;
  /* The search status dump ISR variables */

extern srch_roll_isr_vars_struct_type  srch_roll_isr_vars;
  /* The 26.667ms PN roll ISR variables */

extern srch_agc_pctl_vars_struct_type  srch_agc_pctl_vars;
  /* AGC/Power control logging variables */

extern int2  srch_winsize_slew;
  /* Window size-related slewing regional memory */

extern int4  srch_rate_search_cnt;
  /* Total searches processed since last avg computation */

extern word  srch_gov_time_accum;
  /* Governor time interval accumulator */

extern int2  srch_init_jump_pilot;
  /* Which pilot offset we used in the initial jump to hyperspace */

extern int2  srch_last_good_pilot;
  /* The pilot index of the most recent reference pilot. This
     is used as a guess of where to start when re-acquiring. */

extern int4  srch_last_good_window;
  /* The position of the end of the search window for the most
     recent reference finger. This is used as a guess of where
     to start when re-acquiring. */

extern boolean  srch_last_good_reacq;
  /* Flag indicating we should reacq on the last good pilot */

extern boolean srch_orig_pend;
  /* Flag indicating whether or not an origination is imminent */

extern word srch_sc_fade_iter_cnt;
  /* Count of consecutive failures to demodulate Sync Channel */

extern word srch_pc_fade_iter_cnt;
  /* Count of consecutive failures to demodulate Paging Channel */

#if defined ( FEATURE_ENHANCED_STANDBY_III ) || \
    defined ( FEATURE_KEYPRESS_OPTIMIZATION )
extern boolean srch_keypress_event;
  /* Regional variable to remember keypress events */
#endif

extern boolean srch_rudely_awakened;
  /* Flag: nonzero if an event other than the expiration of the
     sleep timer has resulted in a decision to wake up the MSM. */

extern word srch_msm_version;
  /*  Variable allowing search to configure for a specific MSM version. */

extern srch_msm_config_struct_type srch_msm_config;
  /* Structure holding MSM version related configuration parameters */

extern word *srch_ANCR_sets_ecio_tab_ptr;
  /* Pointer pointing to the Searcher Energy to Ec/Io Lookup Table to be used
     for Active, Neighbor, Candidate and Remaining sets */

extern word *srch_tcpc_ecio_tab_ptr;
  /* Pointer pointing to the Searcher Energy to Ec/Io Lookup Table to be used
     for the Traffic Channel pre-Candidate set */

extern boolean srch_ho_to_oband;
  /* Flag indicating if the mobile just handed-off to a different band class.
     Set during SRCH_TC_ASET_F processing, and used and reset in the function
     srch_log_agc_pctl() */

#ifdef T_MSM5000

#define SRCH_FAST_POS_SLEW_THRESH (2341 * CHIPX8_PER_CHIP)
  /* The MSM5000 can advance the PN generator at 7x the normal rate.
     More over, at the Quarter PN circle boundaries, the finger can
     jump to any other quarter instantly, reducing the maximim slew
     distance to 1/2 the PN circle.  Therefore, there is a 14x speed
     improvement.  Slews of more than 32768/14 should will execute
     faster if made in the negative direction. */

extern boolean srch_fast_positive_finger_slew;
  /* Fast negative slewing (advance) of fingers is part of the MSM5000
     architecture, and cannot be disabled.  Fast positive slewing (retard)
     is not builtin.  This can be emulated in software by slewing the other
     direction around the PN circle. */ 
#endif /* T_MSM5000 */

/*---------------------------------------------------------------------------
         "Variable-constants" for poking in fixed parms from DM 
---------------------------------------------------------------------------*/

extern word srch_fixed_parm_poke;
  /* Poking "1" enables the following poked threshold values */

extern word srch_fixed_parm_tadd;
  /* Pilot add threshold (-14dB Ec/Io) */

extern word srch_fixed_parm_tdrop;
  /* Pilot drop threshold (-16dB Ec/Io) */

extern word srch_fixed_parm_tcomp;
  /* Pilot compare threshold (2.5 dB) */

extern word srch_fixed_parm_ttdrop;
  /* Drop timer (1 second) */

extern word srch_fixed_win_poke;
  /* Poking "1" enables the following poked window values */

extern word srch_fixed_win_a;
  /* Active Set CAI window size ("8" = 60 chips) */

extern word srch_fixed_win_n;
  /* Neighbor Set CAI window size ("8" = 60 chips) */

extern word srch_fixed_win_r;
  /* Remaining Set CAI window size ("8" = 60 chips) */


/*---------------------------------------------------------------------------
                    Regionalized Timer Definitions
---------------------------------------------------------------------------*/

extern rex_timer_type  srch_rpt_timer;
  /* This timer is used during REX waits for an event. It wakes up
     the Search Task in time to signal the watchdog task. It is
     only used in the srch_wait function. */

extern rex_timer_type  srch_acq_timer;
  /* This timer is used to time substates of acquisition. */

extern rex_timer_type  srch_rssi_timer;
  /* This timer is used to time CDMA RSSI update time periods. */

#ifdef  FEATURE_SRCH_SCHEDULER
#error code not present
#else
extern rex_timer_type  srch_lost_dump_timer;
  /* This timer is used when searches are sent. If this timer expires,
     we have lost a status dump and should take corrective action */
#endif /* FEATURE_SRCH_SCHEDULER */

extern rex_timer_type  srch_govern_timer;
  /* This timer is used to throttle search max scan freqency. */

extern rex_timer_type  srch_delay_timer;
  /* This timer is used to delay events such as for throttle Pilot Strength
     Measurement Messages, sleep after keypresses, etc. */

extern rex_timer_type  srch_ref_timer;
  /* This timer is used to throttle Mobile Station Time Reference slews. */

extern rex_timer_type  srch_ctr_timer;
  /* This timer is used to throttle pilot re-centering slews. */

extern rex_timer_type  srch_pc_ho_timer;
  /* This timer is used to delay idle handoffs. */

extern rex_timer_type  srch_pc_throt_timer;
  /* This timer is used to throttle idle handoffs. */

extern clk_cb_type     srch_freq_timer;
  /* This timer is used to time when to store the frequency estimate.
     It expires periodically while on the Paging or Traffic Channel. */

extern rex_timer_type  srch_rf_tune_timer;
  /* This timer is used to time rf_tunes to other channels. */

#ifdef FEATURE_IS95B_PPSMM
extern rex_timer_type srch_ppsmm_timer;
  /* This timer is used for Periodic Pilot Strength Measurement */
#endif

#ifdef FEATURE_IS95B_MAHHO
extern rex_timer_type srch_cf_periodic_timer;
  /* This timer is used for CF Periodic Search */
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*---------------------------------------------------------------------------
                 Regionalized Queue and Buffer Definitions
---------------------------------------------------------------------------*/

extern srch_win_struct_type  srch_win_pool[ ];    
  /* Storage allocation for sweep window buffers */

extern q_type  srch_win_q;
  /* Free queue for sweep window buffers not in use */

#ifdef FEATURE_DIAG_V7_LOG_IFACE
extern srch_fing_log_struct_type  srch_fing_pool[ ];
  /* Storage allocation for the finger log entries */

extern q_type  srch_fing_buf_q;
  /* Free queue for finger log entries not in use. */

extern srch_log_struct_type  srch_srch_pool[ ];
  /* Storage allocation for the searcher log entries */

extern q_type  srch_srch_buf_q;
  /* Free queue for searcher log entries not in use. */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
extern srch_gen_ta_sup_log_struct_type  srch_gen_ta_pool[ GEN_TA_BUF_CNT ];
#else
extern srch_gen_ta_log_struct_type  srch_gen_ta_pool[ ];
#endif
  /* Storage allocation for the general TA log entries */

extern q_type  srch_gen_ta_buf_q;
  /* Free queue for general TA log entries not in use. */

extern srch_agc_log_struct_type  srch_agc_pool[ ];
  /* Storage allocation for the AGC logging buffers */

extern q_type  srch_agc_buf_q;
  /* Free queue for AGC logging buffers not in use. */
#endif /* #ifdef FEATURE_DIAG_V7_LOG_IFACE */

extern q_type srch_search_free_q;
  /* Free queue for storing empty search buffers */

extern q_type srch_search_program_q;
  /* Queue for putting search buffers once they are full of search data */

extern q_type srch_search_8x_pending_q;
  /* Queue for putting the search buffer once it has been programmed in the
     8x hardware */

#ifdef FEATURE_SEARCH2
extern q_type srch_search_16x_pending_q;
  /* Queue for putting the search buffer once it has been programmed in the
     16x hardware */
#endif /* FEATURE_SEARCH2 */

extern q_type srch_search_results_q;
  /* Queue for putting search buffers once the search is complete */

#if defined(FEATURE_GPSONE) && defined(FEATURE_SRCH_SCHEDULER)
#error code not present
#endif /* FEATURE_GPSONE && FEATURE_SRCH_SCHEDULER */


/*---------------------------------------------------------------------------
               Regionalized storage for a set of Paths

     This array contains information about paths. A path is either
     an energy peak detected by the searcher during a sweep, or a
     previously-detected signal now being tracked by a demodulating
     finger. The paths array is filled by one or more sweep evaluations,
     then used to make finger assignment decisions. The paths are
     always at the beginning of the array, and "srch_path_cnt" keeps
     track of how many are valid.
---------------------------------------------------------------------------*/

extern srch_path_struct_type  srch_paths[ ];
  /* Conservative storage allocation.  We might find and keep
     track of one path per finger each time we do a sweep.
     In traffic channel mode we do a sweep for each member of
     the active set before using the results, so we must
     allocate FING_CNT * SRCH_ASET_MAX paths for new paths
     discovered by the sweeps.  In addition, it is possible that
     none of the fingers are on detected paths, so we must
     allocate an additional FING_CNT paths for those paths. */

extern word  srch_path_cnt;
  /* Count of paths stored */


/*-------------------------------------------------------------------------
              Regionalized storage for sets of Sectors

   Many modules in the searcher task unit deal with these sets of
   sectors. Four sets are defined:

      The ACTIVE set consists of those sectors currently being
      used to receive data.  The base station controls which sectors
      are in the active set on the paging and traffic channels.

      The CANDIDATE set consists of those sectors that the mobile
      station currently believes should be considered for promotion
      to the active set.  Members of the candidate set are chosen
      from the neighbor set by the mobile based on signal strength.
      Candidates are reported to the base station for possible
      addition to the active set.

      The NEIGHBOR set consists of those sectors which, according to
      the base station, may be in range of the mobile station (less
      those promoted to the candidate set).  The mobile station watches
      the neighbor set sectors for possible promotion to the candidate
      set.

      The REMAINING set consists of all other possible sectors.
      This is not really maintained as a set, since the number of
      possible pilot offsets is large.  However, the mobile station
      does search (at a low rate) across the entire space of possible
      sectors, looking for an unknown sector.

      This code allocates storage space for the four sets.  Each
      set is maintained in the first locations of the array, and
      the srch_?set_cnt variable keeps track of how many are valid.
-------------------------------------------------------------------------*/

extern srch_sect_struct_type  srch_aset[ ];    /* Active set */
extern srch_sect_struct_type  srch_cset[ ];    /* Candidate set */  
extern srch_sect_struct_type  srch_nset[ ];    /* Neighbor set */
extern srch_sect_struct_type  srch_rset[ ];    /* Remaining set */
#ifdef FEATURE_IS95B_MAHHO
extern srch_sect_struct_type  srch_cfnset[ ]; /* CF Neighbor set */
#endif /* FEATURE_IS95B_MAHHO */
  /* Storage for Active, Candidate, Neighbor, and Remaining
     sets sectors, respectively */

extern word  srch_aset_cnt;    /* Active set size */
extern word  srch_cset_cnt;    /* Candidate set size */
extern word  srch_nset_cnt;    /* Neighbor set size */
extern word  srch_rset_cnt;    /* Remaining "set" size */
  /* Latest sizes of the Active, Candidate, Neighbor, and Remaining
     sets, respectively */


/*---------------------------------------------------------------------------
              Regionalized storage for Finger parameters

     This array contains information about the demodulating fingers.
     Lock thresholds, positions, slew values, pilot offsets, and
     assignment status for each finger is contained in this structure.
---------------------------------------------------------------------------*/

extern srch_fing_struct_type  srch_fings[ ];
  /* The array of fingers. */

extern srch_fing_parm_struct_type srch_fings_common;
  /* Non per-finger data */


/*-------------------------------------------------------------------------
             Regionalized storage for a list of Cells

   This count+array list keeps track of the cells in the Active Set.
   For this purpose, a cell is a collection of sectors that all carry
   the same power control subchannel.

   Membership of sectors in a cell is maintained in the sector struct.
   These cell structs just keep some common information for each cell.
-------------------------------------------------------------------------*/

extern srch_cell_struct_type  srch_cells[ ];
  /* Data about each cell. There is at most one cell per
     sector in the Active Set, so SRCH_ASET_MAX is a safe
     size for this list. */

extern word  srch_cell_cnt;
  /* Count of the number of cells in the list */


/*---------------------------------------------------------------------------
              Regionlized variables for temperature estimation
---------------------------------------------------------------------------*/

extern word srch_recent_temp[ SRCH_MAX_RECENT_TEMP_CNT ];
  /* The recent_temp array contains the most recent temperature estimates */

extern boolean srch_recent_temp_updated;
  /* Flag indicating if the recent_temp array is updated. It is set after
     the first update */

/*---------------------------------------------------------------------------
                Regionalized Sign Extension Lookup Table

   This table is used together with the SIGNX macro to sign extend
   a 18-bit signed two's-complement number stored in the LS bits of
   a 32-bit word to the full 32 bits. This is useful for positions
   as reported by the demod chip.
---------------------------------------------------------------------------*/

extern word srch_sign_words[ ];

/*---------------------------------------------------------------------------
       Regionalized Energy to Neighbor Set Threshold Lookup Table

   This table is used to convert the T_ADD value sent over the air
   into a threshold for Neighbor Set members to enter the Pre-Candidate
   set. This is computed to match, with optimum probability, the
   Ec/Io values required by IS-95. Note: These energies differ from the
   standard Ec/Io table by some fraction. The difference is that this
   table is used in single-sweep evaluations, whereas the standard table
   is averaged over many sweeps including the effects of early dumps.
---------------------------------------------------------------------------*/
extern word  const srch_tc_nthresh_tab[ ];


/*---------------------------------------------------------------------------
      Regionalized Energy to Pre-Candidate Set Threshold Lookup Table

   This table is used to convert the T_ADD value sent over the air
   into a threshold for Pre-Candidate Set members to advance their
   state variables within the Pre-Candidate Set.  This is computed
   to match, with optimum probability, the Ec/Io values required by
   IS-95.

---------------------------------------------------------------------------*/

extern word  const srch_tc_pcthresh_tab[ ];

/*---------------------------------------------------------------------------
                Regionalized dB Attenuation Lookup Table

   This table is used to reduce an energy value by a specified number
   of half dB steps.  It is used by the DB_DOWN macro, shown here but
   defined in SRCHI.H for use throughout the unit.

 define  DB_DOWN( val, db )\
  ((dword) ((val) * (dword) srch_db_down_tab[ (db) ]) + 128) / 256
---------------------------------------------------------------------------*/

extern word  const srch_db_down_tab[ ];

/*---------------------------------------------------------------------------
          Regionalized Window Size Decoding Lookup Table

   This table is used to expand the window size code in the
   search parameters command into an actual window size.
   This is Table 6.6.6.2.1-1 from CAI 2.0.
---------------------------------------------------------------------------*/

extern int2  const win_tab[ ];

/*---------------------------------------------------------------------------
              Regionalized T_TDROP Decoding Lookup Table

   This table is used to expand the T_TDROP index in the search parameters
   command into an actual duration. This is Table 6.6.6.2.3-1 from CAI 2.0.
   Values appear in units of 26.66..ms. Where the CAI value is not an even
   multiple of 26.66..ms, the value is rounded up.
---------------------------------------------------------------------------*/

extern word  const srch_t_tdrop_tab [ ];

/*---------------------------------------------------------------------------
     Regionalized Default Ec/Io to CDMA RSSI display percentage table

   This table is used to convert energy (6-bit unsigned Ec/Io values as 
   required by 6.7.2.3.2.5 of IS-95) into a corresponding linearized 
   device-independent CDMA RSSI percentage of display to show.
---------------------------------------------------------------------------*/

extern byte  const srch_rssi_index_tab[ ];

/*-------------------------------------------------------------------------*/

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

#ifdef FEATURE_FING_MERGE_DETECT
/*---------------------------------------------------------------------------
  Finger merge detection. If we are in a call, check all the finger positions 
  and lock status. If two in-lock fingers are too close in position to one 
  another, deassign the weaker one, leaving the stronger one to demodulate
  the energy path. This minimizes cases when there is concern that having
  more than one finger riding on a multipath will somehow interfere with
  data demodulation or power control combining.
---------------------------------------------------------------------------*/

extern word  srchtri_fing_gran;
  /* When checking if two fingers are so close together that
     they aren't both useful, the minimum offset that will
     be accepted as distinct. */
#endif /* FEATURE_FING_MERGE_DETECT */

#ifdef FEATURE_SRCH_SCHEDULER_MONITOR
#error code not present
#endif /* FEATURE_SRCH_SCHEDULER_MONITOR */


/*===========================================================================

                      REGIONAL FUNCTION DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
               Standard Library Function Declarations

   These functions are used all over.  Rather than include the standard
   headers for these functions (and bring in lots of irrelevant stuff)
   it is convenient to simply declare them here.
---------------------------------------------------------------------------*/

#ifndef _enable
extern void  _enable(void);
#endif
#ifndef _disable
extern void  _disable(void);
#endif
#ifndef __size_t
#define __size_t 1
typedef unsigned int size_t;   /* from <stddef.h> */
#endif
extern void *  memset(void * s, int c, size_t n);
extern void *  memcpy(void *dest, const void *src, unsigned int count);
extern void *  memmove(void *dest, const void *src, unsigned int count);



/*===========================================================================

FUNCTION SRCH_START_INIT                                     

DESCRIPTION    This function is the entry initialization code for the START
      state. Its operation depends on the state from which it is entered.

      All Search task states (START, CDMA, ACQ, SC, SLEW, PC, TC, UNSLEW,
      ZZ, DZ) may successfully init the START state. If START is initialized
      from any valid DMSS state, good status is returned indicating that the
      state transition to START is approved. In addition, if START is
      initialized from TC state, closed-loop power control and its associated
      puncturing is also disabled, having been in use when the DMSS was in a
      call.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to a illegal state transition.

SIDE EFFECTS   puncturing and closed-loop power control may be disabled.

===========================================================================*/

extern srch_cmd_status_type  srch_start_init
(
  srch_state_enum_type  cur_state
    /* Specifies current state from which this function is being invoked. */
);


/*===========================================================================

FUNCTION SRCH_START_STATE                                   

DESCRIPTION    This function is the state handler for the START state.

      Search START state is entered when the Search task is initially
      created, when CDMA pilot acquisition is lost and a full restart
      is desired, and when Main Control is about to signal that the DMSS
      is entering a low power Deep Sleep state, going offline or is
      completely powering down.

      Upon entry, START state unhooks the PN roll and status dump ISR's,
      sets the AGC PDM accumulator to its standby (FM) value and enters a
      loop that continues as long as the Search task is in the START state.
      In first-generation DMSS targets, AGC/pctl logging and associated
      1.25ms TICK ISR is also disabled.

      The START state's main loop waits for, and processes events as follows:
       
    Event      Origin   START state's action     Results (If successful)
    -----      ------   --------------------     -----------------------
   START_F       MC      Init START state         Stay in START 
   CDMA_F        MC      Init CDMA state          Transition to CDMA
   SLEEP_F       MC      Init DZ state            Transition to DZ 
   POWERDOWN_F   MC      Call srch_powerdown()    Stay in START
   STOP sig      MC      Ack the STOP sig         Stay in START
   OFFLINE sig   MC      Ack the OFFLINE sig      Stay in START
  
      Any other MC-SRCH command received causes START to return a BADSEQ_S
      status. Any other REX signal sent to Search during START is ignored.

      Normal powered-on conditions with a CDMA signal present in-band will
      result in MC commanding a state change from START to CDMA, anticipating
      CDMA acquisition.

DEPENDENCIES   This function may only be called after a successful call of
      srch_start_init.

RETURN VALUE   The new state of the searcher task:  START, CDMA, or DZ states.

SIDE EFFECTS   The PN roll and status dump isr's are unhooked.
               The 1.25ms TICK ISR and AGC/pctl logging may be disabled.
               The AGC PDM accumulator is set to its standby value.

===========================================================================*/

extern srch_state_enum_type  srch_start_state ( void );


/*===========================================================================

FUNCTION SRCH_CDMA_INIT                                     

DESCRIPTION    This function is the entry code for the CDMA state.
      It performs no function, other than enforcing the constraint
      that the CDMA state may only be entered from the START state.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to a illegal state transition.

SIDE EFFECTS   None.

===========================================================================*/

extern srch_cmd_status_type  srch_cdma_init
(
  srch_state_enum_type  cur_state
    /* Specifies current state from which this function is being invoked. */
);


/*===========================================================================

FUNCTION SRCH_CDMA_STATE

DESCRIPTION    This function is the state handler for the CDMA state.

      Search CDMA state is transitional, designed sinply to match and
      sequence with Main Control's state transitons. CDMA state consists
      of a loop that continues as long as the Search task is in the CDMA
      state.

      The CDMA state's main loop waits for, and processes events as follows:
       
    Event      Origin   CDMA state's action      Results (If successful)
    -----      ------   --------------------     -----------------------
   START_F       MC      Init START state        Transition to START state
   ACQ_F         MC      Init ACQ state          Transition to ACQ state

      Any other MC-SRCH command received causes CDMA to return a BADSEQ_S
      status. Any other REX signal sent to Search during CDMA is ignored.

      Under normal conditions, MC will transition quickly from START,
      through CDMA, and into later ACQ and subsequent states once a pilot
      has been acquired.

DEPENDENCIES   This function may only be called after a successful call of
      srch_cdma_init.

RETURN VALUE   The new state of the Search task, START or ACQ states.

SIDE EFFECTS   None.

===========================================================================*/

extern srch_state_enum_type  srch_cdma_state ( void );


/*===========================================================================

FUNCTION SRCH_ACQ_INIT                                       

DESCRIPTION    This function is the entry code for the ACQ state.
  
      Search ACQ state may only be entered from the START state via
      transitional CDMA state. If ACQ is initialized from CDMA state,
      it first performs various initializations of the demod chip such
      as shifting AGC to CDMA mode, enabling frequency tracking and
      loading an initial carrier frequency estimate based on temperature.
      The PN roll ISR is also hooked up to keep timestamp services moving
      and to enable AGC/pctl logging if it is called for. Having performed
      these initializations, the init function returns good status. If ACQ
      is initialized from any other state, it returns bad status.

DEPENDENCIES   Freq estimation -vs- temperature table.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   The roll interrupt service routine is hooked up.
               Various demod chip registers are written. 

===========================================================================*/

extern srch_cmd_status_type  srch_acq_init
(
  srch_state_enum_type  cur_state
    /* Specifies current state from which this function is being invoked. */
);



/*===========================================================================

FUNCTION SRCH_ACQ_STATE                                       

DESCRIPTION    This function is the state handler for the ACQ state.

      In the ACQ state, the Search task is trying to find a good pilot.
      Upon entry, ACQ state initializes and slews the searcher and finger
      hardware, gets Cellular/PCN and Full/Mini/Micro modes from DB services,
      and begins the first search. Then, ACQ enters a loop which continues as
      long as the Search task is in the ACQ state.

      The ACQ state's main loop waits for, and processes events as follows:
       
    Event       Origin   ACQ state's action       Results (If successful)
    -----       ------   --------------------     -----------------------
   START_F        MC      Init START state         Transition back to START
   SC_F           MC      Init SC state            Transition forward to SC
   DUMP sig     searcher  Process based on SS      Continue acquiring in ACQ
   ACQ timeout    REX     Process based on SS      Continue acquiring in ACQ
   Lost dmp t/o   REX     Trigger status dump      ACQ state machine continues.
  
      Any other MC-SRCH command received causes ACQ to return a BADSEQ_S
      status. Any other REX signal sent to Search during ACQ is ignored.

      The ACQ state's sends messages and data to other tasks as follows:
       
   What Sent    Sent To   Why sent                 Expected response
   ---------    -------   --------                 -----------------
   ACQFAIL_R      MC      Micro-acq AGC too low    Command back to START
   ACQFAIL_R      MC      Mini-acq stage switch    Command back to START
   ACQ_R          MC      Acquired, found pilot    Command forward to SC
   TA packets    DIAG     Search scans available   None
   Fing packets  DIAG     Finger status available  None

      During a full-up acquisition, no ACQFAIL_R message is ever sent
      to MC if no pilot is found. ACQ state just continues looking for
      a pilot, relying on MC to timeout and command Search back to START.

      The ACQ state uses four stages of acquisition parameters. Changing
      stages result in searches which look deeper and deeper (lower and
      lower Ec/Io) into the signal, looking for a pilot, as the stage is
      incremented. As higher stages of acquisition are used, lower Ec/Io's
      will be looked for, but it will take longer and longer to traverse
      around the PN circle.

      The ACQ state has four substates: SS1_DETECT, SS2_DWELL, SS3_VERIFY,
      and SS4_PULL_IN. These substates are detailed in the ACQ event processing
      function header pertaining to each substate.

DEPENDENCIES   srchaq_*

RETURN VALUE   The new state of the searcher task: START or SC.

SIDE EFFECTS   srch_aset[0] is used.
               srch_fings is used.
               Various demod chip registers are written.

===========================================================================*/

extern srch_state_enum_type  srch_acq_state ( void );


/*===========================================================================

FUNCTION SRCH_SC_INIT

DESCRIPTION    This function is the entry code for the SC state.

      Search SC state may be initialized from ACQ or UNSLEW states. In
      both cases, various initializations are performed such as setting
      up frequency tracking and disabling scrambling for SC state.
      Having performed these initializations, the init function returns
      good status. If SC is initialized from any other state, it returns
      bad status.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   Various demod chip registers are written. 

===========================================================================*/

extern srch_cmd_status_type  srch_sc_init
(
  srch_state_enum_type  cur_state
    /* Specifies current state from which this function is being invoked. */
);



/*===========================================================================

FUNCTION SRCH_SC_STATE

DESCRIPTION    This function is the state handler for the SC state.
      The purpose of SC state is to set up the demod path for receiving
      sync channel frames, and to keep demodulator fingers assigned
      to the strongest energy paths for highest quality demodulation.

      Upon entry to SC state, initializations take place such as
      setting the combiner and finger symbol size to 256 chips/symbol,
      slamming combiner to locked finger timing, and setting up the
      Active set structure to contain SC-relates search parameters.
      Once these initializations are performed, SC enters a loop which
      continues as long as the Search task is in the SC state.

      The SC state's main loop waits for, and processes events as follows:
       
    Event       Origin   SC state's action        Results (If successful)
    -----       ------   --------------------     -----------------------
   START_F        MC      Init START state         Transition back to START
   SLEW_F         MC      Init SLEW state          Transition forward to SLEW
   ROLL sig     combiner  Process based on SS      Continue in SC, change SS
   DUMP sig     searcher  Process based on SS      Continue in SC, searching 
   Governor sig   REX     Wait for governor t/o    Continue in SC, searching
   RSSI sig       REX     Write CDMA RSSI to DB    Continue in SC, searching
   Lost dmp t/o   REX     Trigger status dump      SC state machine continues.
  
      Any other MC-SRCH command received causes ACQ to return a BADSEQ_S
      status. Any other REX signal sent to Search during ACQ is ignored.

      The SC state only sends TA / Finger log packets to DIAG. SC state
      sends no *_R messages back to MC. SC just relies upon MC to receive a
      good sync channel message and command Search to transition forward
      to SLEW state. If good messages are not received, SC state relies
      upon MC to time out and send Search back to START.
       
      SC state has three substates:  SS1_SKIP, SS2_SYNC, and SS3_TRACK.

      In SS1_SKIP, we simply wait for several PN rolls to pass. This
      makes sure the combiner slam has plenty of time to take effect
      before we start moving fingers around. When the roll countdown
      runs out, we move on to SS2_SYNC.

      In SS2_SYNC, we wait for a PN roll to pass, at which time the
      combiner will be aligned to finger (and thus searcher) timing.
      When that occurs, we enable combiner time tracking on the slammed
      finger, originate a searcher scan, and go on to SS3_TRACK.

      In SS3_TRACK, we wait for the searcher to complete sweeps of the
      active window. When a sweep is complete, we run the search "triage"
      operation to get the best finger assignments and begin a new
      search. We stay in this substate forever, until MC commands Search
      to transition to a new state.

DEPENDENCIES   srchsc_*
               Assumes the searcher is still lined up on the signal at entry.

RETURN VALUE   The new state of the searcher task: START or SLEW.

SIDE EFFECTS   srch_aset[0] is used.
               srch_fings is used.
               Various demod registers are written.

===========================================================================*/

extern srch_state_enum_type  srch_sc_state ( void );


/*===========================================================================

FUNCTION SRCH_SLEW_INIT

DESCRIPTION    This function is the entry code for the SLEW state.

      Search SLEW state may only be initialized from the SC state.
      Upon entry from SC state, the initialization function copies the
      information from the slew command (pilot offset, 80ms phase),
      shifts fingers and combiner to using 64 chips/symbol and returns
      good status. If SLEW is initialized from any other state, it returns
      bad status.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   srch_init_jump_pilot is updated.
               Various demod chip registers are written. 

===========================================================================*/

extern srch_cmd_status_type  srch_slew_init
(
  srch_state_enum_type  cur_state,
    /* Specifies current state from which this function is being invoked. */

  srch_slew_cmd_type  *cmd_ptr
    /* Pointer to command which caused us to go to this state. */
);


/*===========================================================================

FUNCTION SRCH_SLEW_STATE

DESCRIPTION    This function is the state handler for the SLEW state.

      In the SLEW state, the Search task performs the manipulations on
      the demod chip to place the fingers, searcher, and symbol combiner
      all on the correct paging channel timing.

      Upon entry to SLEW state, SLEW enters loop which continues as long
      as the Search task is in the SLEW state. The SLEW state's main loop
      waits for, and processes events as follows:
       
    Event       Origin   SLEW state's action      Results (If successful)
    -----       ------   --------------------     -----------------------
   START_F        MC      Init START state         Transition back to START
   PC_F           MC      Init PC state            Transition forward to PC
   ROLL sig     combiner  Process based on SS      Continue in SLEW, change SS
  
      Any other MC-SRCH command received causes SLEW to return a BADSEQ_S
      status. Any other REX signal sent to Search during SLEW is ignored.

      The SLEW state only sends the SLEW_R report back to MC on the PN roll
      right before the hyperspace boundary. The receiver of the SLEW_R report
      must act on it quickly. Originally, there was exactly 320ms from the
      start of the sync channel was received to the hyperspace boundary, but
      once SLEW state finshes working, the very next 26ms roll is the boundary.
      MC must cause a PC_F command to be sent to the Search task in time
      for it to act before the next 26ms PN roll, or the jump to hyperspace
      will be missed. The Search task does not currently detect if this happens.

      The SLEW state has five substates: SS1_SLEW, SS2_PHS_ADJ,
      SS3_UNLOCK, SS4_REPORT, and SS5_DONE.

      In SS1_SLEW, we wait for the 26ms PN roll that starts the middle
      26ms period of an 80ms, according to the existing sync channel
      timing in the symbol combiner. At that time, we compute the
      slew value needed to reach system timing by taking out the pilot
      offset per the sync channel message (using a full 26ms slew in
      the case of zero pilot offset). We command the fingers to slew,
      command the symbol combiner to align its timing with that of
      the fingers (forcing F0 into lock so the combiner can align to
      it), and command the searcher to slew to a position that will
      align the search window on the computed signal position on the
      paging channel. We then advance to SS2_PHS_ADJ.

      In SS2_PHS_ADJ, we wait for a 26ms PN roll that starts the first
      26ms period of an 80ms. This will be the next 26ms PN roll, since
      we programmed the combiner to align its timing before leaving
      SS1_SLEW. When it arrives, we program the new 80ms phase to align
      the next 80ms boundary on the right 26ms boundary (given by
      alignment with the start of the sync channel message). We then
      advance to SS3_UNLOCK.

      In SS3_UNLOCK, we wait for the new 80ms boundary to pass. When it
      does, we remove the forced lock from finger 0.  We then go on to
      SS4_REPORT.

      In SS4_REPORT, we wait for a 26ms PN roll that starts the last 26ms
      period of a (new) 80ms. When it comes around, we make a slew done
      report and advance to SS5_DONE.

      In SS5, we do nothing.  We just wait for a state change command.

      So, the net effect is the following sequence:
            1. Wait for PN roll, phase 1, on sync channel timing.
            2. Everything slews by up to 26ms.
            3. At the next PN roll, we are on system timing except for
               80ms phase.
            4. Adjust 80ms phase to get true system time.
            5. 1, 2, or 3 PN rolls later, we're in phase 0 on system time.
            6. Unforce lock on finger 0.
            7. PN roll, phase 1.
            8. PN roll, phase 2.
            9. Make a slew report.  
           10. Wait for a state change command.
           11. Next PN roll is phase 0, and time to load the PN state
               for the paging channel (per the sync channel message).
               This operation is accomplished later in srch_pc_init.
               This 80ms boundary is the hyperspace boundary.

DEPENDENCIES   srch_slew_vars
               Assumes that the searcher and all the fingers (especially
               finger 0) are lined up on the sync channel signal at entry.

RETURN VALUE   The new state of the searcher task: START or PC.

SIDE EFFECTS   Various demod registers are written.

===========================================================================*/

extern srch_state_enum_type  srch_slew_state ( void );


/*===========================================================================

FUNCTION SRCH_PC_INIT

DESCRIPTION    This function is the entry code for the PC state.

      PC state may be initialized and entered from SLEW or ZZ states.
      If it is initialized from the SLEW state as normal, it copies the
      pilot offset and walsh channel for the paging channel, initializes
      the long code PN state valid at the next 26ms PN roll (the 80ms
      hyperspace boundary) according to the information from the PC
      command, sets the system timestamp clock, fills in sector and 
      parm structures for PC state, performs other initializations,
      and returns good status. If entered from ZZ state, it does nothing
      and returns good status. If entered from any other state, it
      returns bad status.

DEPENDENCIES   srch_init_jump_pilot
               DIP switch 3 enables long code descrambling.
               All the A, N, R sector structures.
               This function must be called during the last 26.666... ms
               period before the hyperspace boundary corresponding to the sync
               channel message received.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   Many PC-state local variables are updated.
               Various demod chip registers are written.
               The timestamp clock is initialized.
               AGC and power control logging may be started.
               All the sector structures are initialized for PC state.
               Over-air parameters are updated, if possible.

===========================================================================*/

extern srch_cmd_status_type  srch_pc_init
(
  srch_state_enum_type  cur_state,
    /* Specifies current state from which this function is being invoked. */

  srch_pc_cmd_type  *cmd_ptr
    /* Pointer to command which is causing us to enter this state. */
);


/*===========================================================================

FUNCTION SRCH_PC_STATE

DESCRIPTION    This function is the state handler for the PC state.

      The purpose of PC state is to keep the demodulating fingers in the
      best places to receive paging channel data. It works with the
      searcher hardware to place fingers on the strongest energy paths of
      the acquired, active pilot. PC will call for an idle handoff if need
      be, if it is determined that a neighbor pilot would yield the best 
      energy paths for the fingers to demodulate.

      Upon entry to PC state, finger and other initializations are performed,
      and a loop is entered which continues as long as the Search task is in
      the PC state. This loop waits for, and processes events as follows:

    Event        Origin     PC state's action       Results (If successful)
    -----        ------     --------------------    -----------------------
   START_F         MC        Init START              Transition back to START
   SLEEP_F         MC        Init ZZ                 Transition to ZZ state
   TC_F            MC        Init TC                 Transition to TC state
   UNSLEW_F        MC        Init UNSLEW             Transition to UNSLEW state
   PARM_F          MC        Load over-air parms     Continue in PC, searching
   PC_ASET_F       MC        Update Active pilot     Continue in PC, searching
   PC_NSET_F       MC        Update Neighbor list    Continue in PC, searching
   PC_ASSIGN_F     MC        Search out Assign list  Continue in PC, searching
   DUMP sig     searcher     Process based on SS     Continue in PC, searching 
   Governor sig   REX        Wait for governor t/o   Continue in PC, searching
   RSSI timer sig REX        Write CDMA RSSI to DB   Continue in PC, searching
   PSM timer sig  REX        Enable PSM reports      Continue in PC, searching
   Lost dmp t/o   REX        Trigger status dump     PC state machine continues

      Any other MC-SRCH command received causes PC to return a BADSEQ_S
      status. Any other REX signal sent to Search during PC is ignored.

      The PC state sends messages and data to other tasks as follows:
       
   What Sent    Sent To    Why sent                 Expected response
   ---------    -------    --------                 -----------------
   PC_MEAS_R      MC      Idle handoff requested    Idle handoff
   PC_ASSIGN_R    MC      PC assign search done     Neighbor list update
   TA packets    DIAG     Search scans available    None
   Finger pkts   DIAG     Finger status available   None

      The PC state calls the dump-driven srch_pc_search function to
      complete non-slotted and awake-slotted searching. This will continue
      indefinitely until we receive a command to change to another state.
      See srch_pc_search for details.

DEPENDENCIES   srchpc_*
             
RETURN VALUE   The new state of the searcher task: START, TC, ZZ, or UNSLEW.

SIDE EFFECTS   srch_aset, srch_nset, srch_rset, srch_fings are updated.
               freq estim table and CDMA RSSI DB values are updated.
               Millions of writes to various demod registers.

===========================================================================*/

extern srch_state_enum_type  srch_pc_state ( void );


/*========================================================================

FUNCTION SRCH_PC_REINIT

DESCRIPTION    This function is used to reinitialize idle search structure
      parameters after SRCHZZ finishes reacquisition.
   
DEPENDENCIES   srchpc_* local variables.

RETURN VALUE   None.

SIDE EFFECTS   srchpc_* local variables are initialized.
               Sector filter lengths are initialized.
               A Rex timer may be set.

========================================================================*/

extern void srch_pc_reinit
(
  int4 slew,
    /* Slew to use on the first search originated */

  byte handoff_pending
    /* Flag telling if there is a handoff pending at the time
       this function is called */
);



/*===========================================================================

FUNCTION SRCH_PC_SEARCH

DESCRIPTION
      This function is the dump-driven state machine for PC state
      searching. It is used in both srch_zz_state for slotted-mode
      (sleep) searching and in srch_pc_state for slotted and non-
      slotted searching.

      The PC state has four substates: SS0_INIT, SS1_ASET, SS2_NSET
      and SS3_RSET. Generally we start in SS1_ASET and circulate
      SS2, SS3, SS1, ... unless for some reason we deem it necessary
      to punt the search schedule and start again
      
      In SS0_INIT, we wait for the current sweep to finish, discard its
      results, begin a new search of the active window, and return to
      SS1_ASET. It is used to terminate the current activity so we
      can begin afresh. 

      In SS1_ASET, we wait for an active set sweep to finish.  When it
      does, we grab its results and begin a neighbor or remaining set
      sweep, as appropriate, going to SS2_NSET (or SS3_RSET for Remaining
      pilots), unless there are no Neighbors or the Remaining pilot is not
      to be searched. In that case the active set again swept and SS1_ASET
      is not left).  After starting the new sweep, we run the search triage
      process on the new results to get a new finger assignment.

      In SS2_NSET, we wait for a neighbor set sweep to finish.  When it does,
      we grab its results and begin an active set sweep, going to SS1_ASET.
      After starting the new sweep, we evaluate the results (and possibly
      report strong neighbors to MC).

      In SS3_RSET, we wait for a remaining set sweep to finish.  When it does,
      we grab its results and begin an active set sweep, going to SS1_ASET.
      After starting the new sweep, we evaluate the results (and possibly
      add the pilot to the neighbor set).

      Thus, the overall effect is to alternate between a sweep of the
      active set and a sweep of either a remaining or neighbor set window.
      Explicitly, the pilot search schedule will be:

         A  N1  A  N2  A .. Ni   A  N1, ... A  Ni   A  R1   <etc>

      That is, we always search the Active pilot every other search,
      we circulate amongst all the Neighbors when we are not searching
      the Active pilot, and occasionally we will insert a single Remaining
      pilot search into the search schedule. The remaining search PN offset
      slowly crawls around the PN circle as each single Remaining search
      is performed. An Rset search is performed once every "rcnt_min"
      Active pilot searches. This process of sweeping continues indefinitely
      until the calling routine receives a command to change to another state.

      To grasp the expected revisit frequency of various pilots, consider
      the following benchmarks of search revisit speed versus window size:

                 Search               Approximate
               Window size           Raw srch rate 
                (chips)              (searches/sec)
               ----------            --------------
                  20                     150
                  28                     120
                  40                     100
                  60                      70
                  80                      50

      These numbers assume equal window sizes for all sets and are approximate
      benchmarks taken from MSM1 and early MSM2P-based targets. The revisit
      frequencies of unique pilot PN offsets in each set will be:

        Active Pilot:    Fa = R / 2                          

        Each Neighbor:   Fn = (R / 2) * (1/nset_size)            

        Each Remaining:  Fr = (R / 2) * (1/rcnt_min) 
                                      / ((512 / pilot_inc) - 1 - nset_size)

      where R is the raw search rate, nset_size is the size of the Neighbor 
      list being searched, rcnt_min is the number of Active searches per 
      Remaining search, and pilot_inc is the pilot increment value PILOT_INC 
      obtained from the base station. For a 40-chip window, 2 Neighbors, 
      rcnt_min=50, and pilot_inc=8, we can expect Fa = 50 Hz, Fn = 25 Hz, 
      and Fr = .0167 Hz, or, taken another way, we will perform a Remaining 
      search once per second and take around 61 seconds to move around the 
      circle.

      The most critical parameters are Fa and Fn. Fa determines how fast finger
      assignments are refreshed, and Fn determines how fast we can react to
      changes in the environment such that idle handoffs are needed to continue
      to keep acquisition and demodulate good paging channel frames.
      
      Also, the Active, Neighbor, and Remaining sets may require different
      search window sizes. This is accounted for by continually remembering
      the latest slew we backed the searcher up by in order to center the
      search scan's window symmetrically about the pilot offset. On each
      search, the previous window-related slew is removed, the new window's
      "back-up" slew is added, the search is sent, and the slew memory is
      refreshed with the new window-related slew.
      
DEPENDENCIES   srchpc_*
 
RETURN VALUE   None.

SIDE EFFECTS   srch_aset, srch_nset, srch_rset, srch_fings are updated.

===========================================================================*/

extern void srch_pc_search
(
  boolean perform_triage,
    /* Flag controlling whether or not triage is allowed. There are
       cases, such as in ZZ, where performing finger assignments could
       rip a finger right out from beneath a slam which is taking effect */

  boolean allow_handoff
    /* Flag controlling whether or not handoff reports will be allowed to
       be sent to MC. In ZZ state, we don't want to idle handoff in this
       function as ZZ might RAHO at the same time */
);


/*===========================================================================

FUNCTION SRCH_TC_INIT

DESCRIPTION    This function is the entry code for the TC state.
      If TC state is initialized from PC as normal, it copies the Walsh
      channel to be used, initializes the long code mask and puncturing
      and power control configurations for the traffic channel, performs
      other initializations, and returns good status.  If entered from any
      other state, it returns bad status.

DEPENDENCIES   srchtc_* local variables
               DIP switch 3 enables long code descrambling.
               DIP switch 4 enables power control puncturing.
               DIP switch 5 enables power control.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   Various demod chip registers are written.
               REX timers may be set.
         
===========================================================================*/

extern srch_cmd_status_type  srch_tc_init
(
  srch_state_enum_type  cur_state,
    /* Specifies current state from which this function is being invoked. */

  srch_tc_cmd_type  *cmd_ptr
    /* Pointer to command which is causing us to enter this state. */
);


/*===========================================================================

FUNCTION SRCH_TC_STATE

DESCRIPTION    This function is the state handler for the TC state.

      The purpose of TC state is to keep the demodulating fingers in the
      best places to receive traffic channel data. It works with the
      searcher hardware to triage fingers amongst the best sectors
      in the Active set. TC will generate a Pilot Strength Measurement
      Message if any Active, Candidate, or Neighbor's pilot energy or age
      time crosses certain thresholds specified by IS-95. The base station
      then determines what type of handoff (soft/softer/hard) is necessary, and
      will signal the DMSS, thus MC and Search tasks, to complete the handoff.

      Upon entry to TC state, sector, finger, and other initializations are
      performed, and a loop is entered which continues as long as the Search
      task is in the TC state. This loop waits for, and processes events as
      follows:

    Event        Origin     TC state's action       Results (If successful)
    -----        ------     --------------------    -----------------------
   START_F         MC        Init START              Transition back to START
   UNSLEW_F        MC        Init UNSLEW             Transition to UNSLEW state
   PARM_F          MC        Load over-air parms     Stay in TC, punt schedule
   TC_ASET_F       MC        Update Active pilot     Stay in TC, punt schedule
   TC_NSET_F       MC        Update Neighbor list    Stay in TC
   MEAS_F          MC        Send TC_MEAS_R rpt      Stay in TC
   FPC_MEAS_F      MC        Send TC_MEAS_R rpt      Stay in TC
   DUMP sig     searcher     Process based on SS     Stay in TC
   Governor sig   REX        Wait for governor t/o   Stay in TC
   Freq timer sig REX        Update freq-temp table  Stay in TC
   RSSI timer sig REX        Write CDMA RSSI to DB   Stay in TC
   PSM timer sig  REX        Allow TS_MEAS reports   Stay in TC
   REF timer sig  REX        Allow time ref adj      Stay in TC
   CTR timer sig  REX        Allow centering slews   Stay in TC
   Lost dmp t/o   REX        Trigger status dump     TC state machine continues

      Any other MC-SRCH command received causes TC to return a BADSEQ_S
      status. Any other REX signal sent to Search during TC is ignored.

      The TC state sends messages and data to other tasks as follows:
       
   What Sent    Sent To    Why sent                   Expected response
   ---------    -------    --------                   -----------------
   TC_MEAS_R      MC      Pilot Eng/age cross thresh  Handoff Direction Message
                           or Base station requests    or nothing at all.

   TA packets    DIAG     Search scans available      None
   Finger pkts   DIAG     Finger status available     None

      The TC state has five substates: SS0_INIT, SS1_ASET, SS2_CSET, SS3_NSET,
      and SS4_RSET. Generally we start in SS1_ASET and only drop back to
      SS0_INIT if we decide to punt the search schedule and start over.

      In SS0_INIT, we are merely waiting for the current sweep to finish
      so we can discard it and begin again at SS1_ASET. This substate is
      entered when either a PARM_F or TC_ASET_F command arrives. It is
      used to terminate the current activity so we can begin afresh with
      (possibly) new parameters. Punting when a PARM_F is necessary as we
      do not want to update sector window sizes and other parms on the fly.
      Computations and evaluations using the old parameters may be fouled up.
      Punting on a ASET_F command is needed as we may have completely
      reorganized the sector structures and reordered the search schedule.

      In SS1_ASET, we are waiting for an Active Set sweep to finish.  When
      it does, we grab its results, being the next search, and go to the
      substate that corresponds to the type of search selected. If this
      was the last Active Set sector, we run finger assignment on the
      new Active Set results.

      In SS2_CSET, we are waiting for a Candidate or Pre-Candidate Set
      sweep to finish.  When it does, we grab its results, begin the next
      search, and go to the substate that corresponds to the type of
      search selected.  We then evaluate the sweep results.

      In SS3_NSET, we are waiting for a Neighbor Set sweep to finish. When
      it does, we grab its results, begin the next search, and go to the
      corresponding substate.  We then evaluate the sweep results.

      In SS4_RSET, we are waiting for a Remaining Set search to finish.
      When it does, we grab its results, begin the next search, and go to
      the corresponding substate.  We then evaluate the sweep results.

      Explicitly, in TC state the pilot search schedule will be:

         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    N1
         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    N2
           <etc>
         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    Nk

         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    N1
         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    N2
           <etc>
         A1  A2  A3 .. Ai    C1  C2  C3 .. Cj    R1     ..etc, etc

      That is, we search all Active pilots, all Candidates and Pre-Candidiates, 
      one Neighbor, then repeat. Occasionally we will substitute a Remaining
      pilot search for a Neighbor search.

      is performed. An Rset search is performed once every "rcnt_max" spins
      spins through the A/C/N schedule. The process of sweeping continues
      indefinitely until the calling routine receives a command to change to
      another state.

      To grasp the expected revisit frequency of various pilots, consider
      the following benchmarks of search revisit speed versus window size:

                 Search               Approximate
               Window size           Raw srch rate 
                (chips)              (searches/sec)
               ----------            --------------
                  20                     150
                  28                     120
                  40                     100
                  60                      70
                  80                      50

      These numbers assume equal window sizes for all sets and are approximate
      benchmarks taken from MSM1 and early MSM2P-based targets. The TC revisit
      frequencies of unique pilot PN offsets in each set will approximately be:

        Each Active Pilot:  Fa = R / (aset_size + cset_size + 1)

        Each Candidate:     Fc = Fa

        Each Neighbor:      Fn = R / (aset_size + cset_size + nset_size)

        Each Remaining:     Fr = (Fa / rcnt_max) 
                     / ((512/pilot_inc) - aset_size-cset_size - nset_size)

      where R is the raw search rate, a/c/nset_size are the sizes of Active,
      Candidate, and Neighbor sets being searched, and rcnt_max is the number
      of schedules per Remaining search.
      
      The most critical parameters are Fa and Fn. Fa determines how fast finger
      assignments are refreshed, and Fn determines how fast we can react to
      changes in the environment such that soft handoff is needed to continue
      to keep acquisition and demodulate good traffic channel frames.

      For 40-chip windows, 3 Active pilots, 1 Candidate, and 6 Neighbors,
      we can expect Fa = Fc = 20 Hz, Fn = 10 Hz, and thus the finger assignment
      rate also equals 20 Hz.

      Allowable set transitions and conditions for reporting are all according
      to IS-95 and are detailed in the search evaluation functions for each set.
      See them for more details.

      The Active, Neighbor, and Remaining sets may also require different
      search window sizes. This is accounted for by continually remembering
      the latest slew we backed the searcher up by in order to center the
      search scan's window symmetrically about the pilot offset. On each
      search, the previous window-related slew is removed, the new window's
      "back-up" slew is added, the search is sent, and the slew memory is
      refreshed with the new window-related slew.

      When pilots are transitioned between sets, different window sizes require
      special handling of each pilot's nominal offset from center. For example,
      Highly offset Neighbors found in wide Neighbor set windows can be lost
      during narrow Candidate searches unless this is taken into account.

      In addition, in TC state, Active and Candidate searches are recommended
      to each be centered about their earliest usable multipath component. This
      recommendation and its related need for centering slew rate throttling
      and snapback hysteresis are all handled.

      Finally, IS-95 requires that a "time reference" be established, and srches
      and pilot measurement reports be in accordance with this time reference.
      In TC state, this is taken into account via the POS_MATH baseline
      (the latest time reference baseline is always set to be the POS_MATH
      position center relative to the reference sector). This time reference
      is gradually changed as the reference is changed according to maximum slew
      rates specified by IS-95 and reflected in symbol combiner operation.

      Therefore, in a dynamic environment with delayed pilots changing set
      membership, all these slew movements (window sizing, nominal offset,
      centering slew, snapback hysteresis, and time reference baseline
      movements) work together to center search windows and make reports
      according to IS-95 requirements and recommendations.

DEPENDENCIES   srchtc_*
               srch_roll_isr_vars.tick26
               srch_aset, srch_cset, srch_nset, ...

RETURN VALUE   The new state of the Search task:  START or UNSLEW.

SIDE EFFECTS   Many srchtc_* local variables are heavily used.
               Regional sector, finger, path, cell structs are heavily used.
               Various demod chip registers are written

===========================================================================*/

extern srch_state_enum_type  srch_tc_state ( void );



/*===========================================================================

FUNCTION SRCH_UNSLEW_INIT

DESCRIPTION    This function is the entry code for the UNSLEW state.

      Search UNSLEW state may only be initialized from PC or TC states.
      If it is initialized from the TC state, it turns off puncturing and
      closed loop power control. If UNSLEW is initialized from the PC or TC
      state, the initialization function returns good status. If UNSLEW is
      initialized from any other state, it returns bad status.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:
      SRCH_DONE_S if successful.
      SRCH_BADSEQ_S if unsuccessful due to an illegal state transition.

SIDE EFFECTS   Various demod chip registers are writeen.

===========================================================================*/

extern srch_cmd_status_type  srch_unslew_init
(
  srch_state_enum_type  cur_state
    /* Specifies current state from which this function is being invoked. */
);



/*===========================================================================

FUNCTION SRCH_UNSLEW_STATE

DESCRIPTION    This function is the state handler for the UNSLEW state.

      The purpose of the UNSLEW state is to perform the manipulations
      of the demod chip to return the fingers, searcher, and symbol
      combiner from paging/traffic timing to sync channel timing. 

      Upon entry to UNSLEW state, the largest-energy pilot in any of
      the Active, Candidate, or Neighbor sets is chosen as the pilot
      of interest. Slew values to center this pilot on sync channel
      timing and remove out off-centeredness is computed. The fingers
      and searcher are then commanded to slew, and an UNSLEW_R report
      is sent back to MC. Also, the Neighbor set is conditioned to
      reflect the fact that we may be unslewing to some pilot other
      than the old Active pilot. 

      Finally, a loop is entered which continues as long as the Search
      task is in the UNSLEW state. The UNSLEW state's main loop waits for,
      and processes events as follows:
       
     Event     Origin   UNSLEW state's action    Results (If successful)
     -----     ------   --------------------     -----------------------
    START_F      MC      Init START state         Transition back to START
    SC_F         MC      Init SC state            Transition back to SC

      Any other MC-SRCH command received causes UNSLEW to return a BADSEQ_S
      status. Any other REX signal sent to Search during UNSLEW is ignored.
      UNSLEW loops forever, waiting for a state change.

DEPENDENCIES   srch_aset, srch_cset, srch_nset

RETURN VALUE   The new state of the Search task: START or SC.

SIDE EFFECTS   srch_aset, srch_cset, srch_nset may be modified
               srch_fings may be modified
               Various demod registers are written.

===========================================================================*/

extern srch_state_enum_type  srch_unslew_state ( void );



/*===========================================================================

FUNCTION SRCH_ZZ_INIT


DESCRIPTION    This function is the entry code for the ZZ state.

      If ZZ is entered from any state other than PC, it returns bad status.
      Otherwise, it does a preliminary computation of the duration of the
      sleep (it always schedules the wakeup in time for our next assigned
      paging channel slot).  If there isn't enough time left to sleep and
      wakeup before our assigned slot, it returns SRCH_TOOLATE_S (and the
      state transition into the sleep state should not take place). Otherwise
      the sleep should proceed normally, so it returns good status.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:

      SRCH_DONE_S     if it was successful.

      SRCH_BADSEQ_S   if it was unsuccessful due to a bad
                      sequence (that is, an illegal state transition).

      SRCH_TOOLATE_S  if there isn't enough time between now and our
                      next slot time to go to sleep and wake up again.

SIDE EFFECTS   None.

===========================================================================*/

extern srch_cmd_status_type  srch_zz_init
(
  srch_state_enum_type  cur_state,
    /* Specifies current state from which this function is being invoked. */

  srch_sleep_cmd_type  *cmd_ptr
    /* Pointer to command which is causing us to enter this state. */
);



/*===========================================================================

FUNCTION SRCH_ZZ_STATE

DESCRIPTION    This function is the state handler for the ZZ state.

      The purpose of ZZ state is to put the MSM to sleep and wake it
      back up in time for a designated Paging Channel slot. This is
      a powersaving technique, useful when the DMSS is idling, waiting
      for incoming pages.

      After some initialization, it enters a loop which continues as
      long as the Search task is in the ZZ state.  The ZZ state's
      main loop waits for, and processes events as follows:
       
    Event        Origin     DZ state's action       Results (If successful)
    -----        ------     --------------------    -----------------------
   START_F         MC        Wakeup, Init START      Transition back to START
   PC_F            MC        Init PC                 Transition to PC state
   WAKEUP_F        MC        Set rude wake flag      Stay in ZZ, rude wake armed
   WAKEUP sig  sleep timer   Process based on SS     Wakeup/catnap again in ZZ   
   ROLL sig     combiner     Process based on SS     Sleep/warmup proc in ZZ
   DUMP sig     searcher     Process based on SS     Continue in ZZ, searching 
   Lost dmp t/o   REX        Trigger status dump     ZZ state machine continues.

      Any other MC-SRCH command received causes ZZ to return a BADSEQ_S
      status. Any other REX signal sent to Search during DZ is ignored.
      In addition, ZZ can only legally transition to the PC state from
      final substate SS8, and will return a "bad sequence" status for other
      substates.  

      The ZZ state sends messages and data to other tasks as follows:
       
   What Sent    Sent To    Why sent                 Expected response
   ---------    -------    --------                 -----------------
   WAKEUP_R       MC      Initial wakeup            MC waits for further rpt
   REACQ_R        MC      Reacquired, found pilot   Command to PC state 
   RAHO_R         MC      Mini-acq stage switch     Idle handoff, cmd to PC 
   RAFAIL_R       MC      Micro-acq AGC too low     Command back to START
   TA packets    DIAG     Search scans available    None
   Finger pkts   DIAG     Finger status available   None

      In the ZZ state, we walk through a careful timeline that lets the
      DMSS maintain a good estimate of system time, yet remain mostly
      powered off for a substantial period of time, and still be capable
      of waking up and receiving the channel on fairly short notice to
      service a user requirement for use of the channel.

      The ZZ state has 8 substates:  SS1_INIT, SS2_NAP, SS3_WARMUP, SS4_ASRCH,
      SS5_NSRCH, SS6_RESYNC, SS7_SLAMMING, and SS8_DONE. The state machine
      usually starts in SS2_NAP. In SS2_NAP, the sleep counter is programmed,
      and the MSM goes to sleep on the next PN Roll. If the combiner time
      is too close to a PN Roll, we cannot setup for a sleep before the PN
      Roll occurs, so we pass through SS1_INIT first.

      In SS1_INIT, we wait for a PN Roll to occur.  When the roll occurs, we
      know there's time to program the MSM to freeze on the next PN Roll, so we
      program it to start sleep at the next roll.  Sleep is done in "catnaps".
      The first catnap is at least SLOT_MINIMUM_CATNAP * 26.67ms, successive
      catnaps are SLOT_CATNAP_LENGTH * 26.67ms.  After the MSM is programmed to
      sleep, the sleep task is told that the searcher task is ready to sleep.

      Many other jobs are done at the same time the sleep timer is programmed.
      We set the encoder to sleep mode to make sure PA/PUNCT is in the right
      state. We also inform the RF driver that we are going to sleep.  We save
      the frequency carrier tracking accumulator and park TRK_LO_ADJ. Finally,
      the watchdog is set to kick itself so that the CPU may go to sleep.

      In SS2_NAP, we wait for a wakeup interrupt.  The wakeup interrupt signals
      that the sleep timer has expired and that the warmup timer has started
      running. When the wakeup occurs, we decide if it's really time to wake up
      the MSM. If it is, we clear the RF_SLEEP/_MASK/ bit (allowing the RF to
      power-up) and reprogram the RF circuitry.  We then go to SS3_WARMUP.
  
      We remain in SS2_NAP, if it is not time to wake up.  We set the
      RF_SLEEP/_MASK/ bit to inhibit RF power-up and program the MSM for
      another catnap. RF_SLEEP/_MASK/ cannot be set when the sleep timer is
      initially programmed (e.g. SS1 or SS2), as chipx8 is still gated into
      the Demodulator core at that point in time. By the time we've reached
      SS2_NAP, the MSM has already started sleep, and gated-off chipx8 to
      the Demodulator. This accounts for a single 40us slew in time while the
      sleep interrupt is processed. If it is time to stop catnapping and wake
      up, the RF hardware is powered up and re-programmed, the Sleep task is
      notified of the wakeup (so that the processor will stay awake), and we
      move to SS3_WARMUP.

      In SS3_WARMUP, we wait for the PN ROLL interrupt that indicates the
      warmup timer has expired and the MSM is running again.  We program the
      searcher to search for the sector that was in the active set when we went
      to sleep. Then we compute the new ULPN forward scrambling code state based
      on the duration of sleep. This new long code state, along with the new
      80ms phase, is loaded on the combiner slam setup in SS6_RESYNC. Meanwhile,
      we also take the watchdog out of autokick mode, restart clocks, and
      reload / re-activate carrier frequency tracking (TRK_LO_ADJ). We then
      notify MC that we are fully awake and go to SS4_ASRCH.

      In SS4_ASRCH, we wait for the sweep of the old active sector to end.
      When it ends, we evaluate the results to determine if we have successfully
      reacquired it. If we have reacquired, we assign fingers and go to
      SS6_RESYNC. If a PN Roll occurs, the ULPN and 80ms phase are advanced to
      account for it.

      If the active pilot is not found, we program the searcher to sweep for the
      strongest neighbor set sector and go to SS5_NSRCH. If there is no neighbor
      set sectors we notify MC that reacquisition has failed and go to SS8_DONE.

      In SS5_NSRCH, we wait for the sweep of the neighbor sector to end. When it
      ends, we evaluate the results to determine if we have successfully
      reacquired it. If the neighbor sector sweep does not find a pilot, we
      search the next neighbor the sector until we run out of neighbors,
      remaining in SS5_NSRCH.  If we have acquired, we assign fingers and make
      the neighbor sector the active sector and go to SS6.  If we run out of
      neighbors, we notify MC that re-acquisition has failed and go to SS8.
      If a PN Roll occurs, the ULPN and 80ms phase are advanced to account for
      it.

      In SS6_RESYNC, we begin a PC-state slotted idle search pattern of pilots
      while waiting for a PN Roll. At the PN Roll, we setup a slam on the
      earliest finger and go to SS7_SLAMMING.

      In SS7_SLAMMING, we wait for the PN Roll while continuing slotted-idle
      searching. When the PN Roll occurs, the symbol combiner has slammed it's
      timing to match the earliest finger. We program the ULPN and 80ms phase.
      The ULPN and 80ms phase become active on the next PN Roll. We then go
      to SS8_DONE.

      In SS8_DONE, there's nothing left to do.  We just continue slotted idle
      searching and wait for a command to change states.

      In ZZ state there are several mechanisms in-place which aid reacquisition
      of the active pilot once we have woken up. First, sleep-to-sleep slews
      required to center the searcher on the pilot are passed through a 1:3 IIR
      filter. This yields a prediction of the average pilot time drift due to an
      inactive frequency tracking TRK_LO_ADJ PDM during sleep. This average
      drift is dealt with in two ways: by recentering the searcher during the
      reacquisition sweep, and, by slowly adding/removing clock ticks from the
      sleep timer during each catnap such that on wakeup the apparent long-term
      average time drift is zero and the reacquisition search finds the active
      pilot centered. Removing ticks from the sleep timer keeps the time drift
      transparent to not only the searcher but also the fingers and symbol
      combiner, as well. This is important because the symbol combiner cannot be
      slewed to take out the time drift - from sleep to sleep the error simply
      builds up in the deskew FIFOs and quickly becomes too large to handle.
      With gradual addition/subtraction of the estimated error from the sleep
      timer, slamming the combiner is not required on every wakeup, but only
      occasionally, when we have determined that the built-up drift is close
      to exceeding the combiner's de-skew FIFO's ability to compensate.
      Therefore, we usually skip directly from reacquiring (in SS4 or SS5) to
      SS8_DONE. This saves up to 3 PN rolls of wake time.

      To assist reaquisition in the presence of freq-error time drift, we will
      perform a spiraling maneuver on the active search window to locate the
      pilot if we cannot reacquire the active pilot on the first reacq search.
      If this fails, only then will we try neighbors, beginning with the
      neighbor which had the strongest energy when we went to sleep.

      In addition, to get a good estimate of drift, we place TRK_LO_ADJ in a
      known state (either railed or tri-stated to midpoint) when we go to sleep
      such that the largest amount of error is a known, fixed value we can begin
      prediction with.

      Finally, we "walkup" the slot cycle index used from sleep to sleep.
      That is, if we desire to sleep for long (large SCI) sleep periods, when
      we arrive on the paging channel we will sleep first using SCI=0, then
      SCI=1, ..., until we reach the SCI desired. We only increment our
      working SCI when we know our drift estimate is stable. When we have
      reason to believe the estimate is not yet stable (we had to use spiraling
      to find the pilot or the estimated drift will place the pilot outside
      the window using a larger SCI), the SCI is held at the current value or
      even dropped back.

DEPENDENCIES   None.

RETURN VALUE   The new state of the searcher task:  START or PC.

SIDE EFFECTS   Millions of writes to the MSM and RF hardware.

===========================================================================*/

extern srch_state_enum_type  srch_zz_state ( void );



/*===========================================================================

FUNCTION SRCH_ZZ_RESTART_SCI

DESCRIPTION    This procedure lets the caller signal for ZZ to clear its local
      value of slot cycle index and begin walking up again from zero.

DEPENDENCIES   srchzz_sci

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_zz_restart_sci ( void );



/*===========================================================================

FUNCTION SRCH_ZZ_INC_SYS_TIME

DESCRIPTION    This procedure increments system time to its proper time
      upon wakeup.

DEPENDENCIES   srchzz_sleep_dur, srchzz_catnaps

RETURN VALUE   None.

SIDE EFFECTS   srchzz_sleep_dur may be modified

===========================================================================*/

extern void srch_zz_inc_sys_time ( boolean increment );



/*===========================================================================

FUNCTION SRCH_DZ_INC_SYS_TIME

DESCRIPTION    This procedure increments system time to its proper time
      upon wakeup.

DEPENDENCIES   srchzz_sleep_dur, srchzz_catnaps

RETURN VALUE   None.

SIDE EFFECTS   srchzz_sleep_dur may be modified

===========================================================================*/

extern void srch_dz_inc_sys_time ( boolean increment );



/*===========================================================================

FUNCTION SRCH_DZ_INIT

DESCRIPTION    This function is the entry code for the DZ state.

      If DZ is initialized from any state other than START, it returns
      bad status. Otherwise, it does a preliminary computation of the
      duration of the sleep. If there isn't enough time left to sleep
      before wakeup is desired, it returns SRCH_TOOLATE_S (and the state
      transition into the sleep state should not take place).  Otherwise,
      the sleep should proceed normally, so it returns good status.

DEPENDENCIES   None.

RETURN VALUE   Status of the requested state transition:

      SRCH_DONE_S     if it was successful.

      SRCH_BADSEQ_S   if it was unsuccessful due to a bad
                      sequence (that is, an illegal state transition).

      SRCH_TOOLATE_S  if there isn't enough time to sleep between now and
                      when we want to wake up again.

SIDE EFFECTS   None.

===========================================================================*/

extern srch_cmd_status_type  srch_dz_init
(
  srch_state_enum_type  cur_state,
    /* Specifies the current state from which this init function is being
       invoked. */

  srch_sleep_cmd_type  *cmd_ptr, 
    /* Pointer to command which is causing us to enter this state. */

  srch_cmd_code_type  cmd
    /* Type code of command which is causing us to enter this state. */
);



/*===========================================================================

FUNCTION SRCH_DZ_STATE

DESCRIPTION    This function is the state handler for the DZ state.
      The purpose of DZ state is to put the MSM to sleep and wake it
      back up again after a fixed period of time if neither CDMA nor
      FM service can be found by DMSS MC System Determination. DZ state
      is activated while the Search task is in START state since the
      Search task always loiters in START state when MC System Determination
      is performed.

      Upon entry to DZ state, the digital portion of the RF hardware is
      powered up in case we entered whilst the DMSS was previously in FM mode.
      DZ then sets the RXC SLEEP_OKTS signal as RXC may be offline and cannot
      notify the Sleep task for itself.
      
      After these actions have been completed, DZ enters a loop which
      continues as long as the Search task is in the DZ state. The DZ state's
      main loop waits for, and processes events as follows:
       
    Event       Origin     DZ state's action        Results (If successful)
    -----       ------     --------------------     -----------------------
   START_F        MC        Wakeup, Init START       Transition back to START
   WAKEUP_F       MC        Set rude wake flag       Stay in DZ, rude wake armed
   WAKEUP sig  sleep timer  Process based on SS      Wakeup/catnap again in DZ   
   ROLL sig     combiner    Process based on SS      Sleep/warmup proc in DZ

      Any other MC-SRCH command received causes DZ to return a BADSEQ_S
      status. Any other REX signal sent to Search during DZ is ignored.

      The DZ state sends only the WAKEUP_R report back to MC. This is done
      once all wakeup/warmup processing has completed. DZ state then
      waits forever for an expected START_F state transition from MC.

      In the DZ state, we walk through a careful timeline that lets the DMSS
      carefully control the hardware for sleeping/waking up from deep sleep.

      The DZ state has 4 substates: SS1_INIT, SS2_NAP, SS3_WARMUP, and SS4_DONE.
      The state machine usually starts in SS2_NAP.  The sleep counter is
      programmed, and the MSM goes to sleep on the next PN Roll. If the
      combiner time is too close to a PN Roll, we cannot setup for a sleep
      before the PN Roll occurs, so we pass through SS1_INIT first.

      In SS1_INIT, we wait for a PN Roll to occur.  When the roll occurs, we
      know there's time to program the MSM to freeze on the next PN Roll,
      so we program it to start sleep at the next roll.  Sleep is done in
      "catnaps", where catnaps are CATNAP_LENGTH * 26.67ms.  After the MSM
      is programmed to sleep, the sleep task is told that the Search task
      is ready to sleep. Many other jobs are done at the same time the
      sleep timer is programmed. We set the encoder to sleep mode to make
      sure PA/PUNCT is in the right state. We also inform the RF driver
      that we are going to sleep.  We save the frequency carrier tracking
      accumulator and park TRK_LO_ADJ. Finally, the watchdog is set to
      kick itself so that the CPU may go to sleep.

      In SS2_NAP, we wait for a wakeup interrupt.  The wakeup interrupt signals
      that the sleep timer has expired and that the warmup timer has started
      running. When the wakeup occurs, we decide if it's really time to wake up
      the MSM. If it is, we clear the RF_SLEEP/_MASK/ bit (allowing the RF to
      power-up) so that the RF circuitry can be reinitialized.  We then go to
      SS3_WARMUP.
  
      We remain in SS2_NAP if it is not yet time to wake up.  We set the
      RF_SLEEP/_MASK/ bit to inhibit RF power-up and program the MSM for another
      catnap. RF_SLEEP/_MASK/ cannot be set when the sleep timer is initially
      programmed (e.g. SS1 or SS2), as chipx8 is still gated into the
      Demodulator core at that point in time.  By the time we've reached SS2,
      the MSM has already started sleep, and gated-off chipx8 to the Demodulator.
      This accounts for a single 40us slew in time while the sleep interrupt is
      processed. If it is time to stop catnapping and wake up, the RF hardware
      is powered up and re-programmed, the Sleep task is notified of the
      wakeup (so that the processor will stay awake), and we move to SS3_WARMUP.

      In SS3_WARMUP, we wait for the PN ROLL interrupt that indicates the warmup
      timer has expired and the MSM is running again.  Meanwhile, we take the
      watchdog out of autokick mode, restart clocks, and reload / re-activate
      carrier frequency tracking (TRK_LO_ADJ). We then notify MC that we are
      fully awake and go to SS4_DONE.

      In SS4_DONE, there's nothing left to do.  We just wait for a command to
      change states. This is expected to be to START state.

DEPENDENCIES   None.

RETURN VALUE   The new state of the searcher task: START.

SIDE EFFECTS   Millions of writes to the MSM and RF hardware.

===========================================================================*/

extern srch_state_enum_type  srch_dz_state ( void );



/*===========================================================================

FUNCTION SRCH_SORT_SECT

DESCRIPTION    This function looks through a sweep window of energies and
   finds the best few paths  It records the details of the paths found
   (and some totals for the whole window) in the sects and paths arrays.

DEPENDENCIES

RETURN VALUE   The number of paths in the paths array after we've added
   any we find.

SIDE EFFECTS   Uses srchtri_path_ray as a working buffer.

===========================================================================*/

extern word  srch_sort_sect 
(
  srch_sect_struct_type  sects[ ],
    /* The array of current sectors */

  word  s,
    /* Index of the sector of interest for this operation */

  srch_path_struct_type  paths[ ],
    /* Array of paths */
    /* Optional; if NULL path info is not stored */

  word  path_cnt,
    /* Number of entries already in the paths array */
    /* Set to 0 if paths is NULL */

  boolean individual
    /* Flag telling to modify pilot POS positions based on
       individual pilot centering accumulated sums of slews */
);



/*===========================================================================

FUNCTION SRCH_TRIAGE

DESCRIPTION    This function is called after a searcher sweep of an
      active window to take care of finger assignment and related
      activities.
      
      In general, the functions processes as follows:

       1. We match up the existing finger assignments with paths found, 
          discarding any duplicate finger assignments, and adding any 
          finger assignments not found in the sweep to paths.

       2. We Assign fingers to the strongest cells that don't have fingers,
          until we run out of fingers. Then possibly let the next ONE
          strongest cell rob a finger from a current assignment. 
         
       3. If no finger was reassigned in 2, we assign fingers to the
          strongest PATHS that don't have fingers, until we run out of
          fingers. Then we possibly let the next ONE strongest path rob
          a finger from a current assignment, but only if it doesn't reduce
          the coverage of cells.

       4. Wrap up: assign power control indices, update each finger,
          find the reference finger and take care of time tracking
          for the fingers, combiner, and search window, and return
          the motion of the search window 

      All of the functions of srch_triage are delegated to the routines
      called by srch_triage.  See their descriptions for details.

DEPENDENCIES   None.

RETURN VALUE   How far to slew the active window.

SIDE EFFECTS   Lots.  See the called routines for details.

===========================================================================*/

extern int4  srch_triage
(
  srch_sect_struct_type  sects[ ],
    /* The list of current sectors */

  srch_fing_struct_type  fings[ ],
    /* The list of current fingers */

  word  fing_cnt,
    /* The number of fingers in the list */

  srch_path_struct_type  paths[ ],
    /* The list of paths found by the searcher */

  word  path_cnt, 
    /* The number of paths in the list */

  boolean  enable_baseline_adjust,
    /* Flag telling us whether or not we should go ahead
       and adjust the baseline this time around */

  boolean  limit_baseline_slew,
    /* Flag telling us whether or not we should hard
       limit any baseline adjustments to "srch_win_gran" */

  boolean  *baseline_moved
    /* Return - informs the caller whether or not the
       time reference baseline moved at all this time */
);



/*===========================================================================

FUNCTION SRCH_FIND_EARLIEST_FING

DESCRIPTION    Search a list of fingers and return the index of the
      earliest finger (which has been locked long enough) assigned to
      a sector.

DEPENDENCIES   None.

RETURN VALUE   The index of the earliest finger assigned to a sector.
               If no fingers are assigned to any sector, or if none of the
               fingers have been in lock long enough, NO_FINGER is returned.

SIDE EFFECTS   None.

===========================================================================*/

extern word srch_find_earliest_fing
(
  srch_fing_struct_type  fings[],
    /* A list of fingers */

  word       fing_cnt,
    /* The number of fingers in the list */

  word     lock_age
    /* Lock age threshold to use to use as valid finger */
);

/*===========================================================================

FUNCTION SRCH_FIND_EARLIEST_PATH

DESCRIPTION    Search a list of paths and return the index of the earliest
      path in a list of paths.

DEPENDENCIES   None.

RETURN VALUE   The index of the earliest path in a sector.
               If no paths are present, NO_PATH is returned.

SIDE EFFECTS   None.

===========================================================================*/

extern word srch_find_earliest_path
(
  srch_path_struct_type  paths[],
    /* A list of fingers */

  word       new_cnt, 
    /* The number of new paths in the list */

  word       path_cnt
    /* The number of paths already in the list */
);


/*===========================================================================

FUNCTION SRCH_LOAD_FING_PARMS

DESCRIPTION    This function is called to load the finger lock
      and gain parameters.
      
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Finger structures are updated.

===========================================================================*/
extern void srch_load_fing_parms ( void );


/*===========================================================================

FUNCTION SRCH_ADJ_SRCH_GOV

DESCRIPTION    This module adjusts the amount of delay inserted into the
      back-to-back search processing in order to slow down the average
      search rate. It is needed in order to keep processor loading under
      control when using small search window sizes.

DEPENDENCIES   srch_rate_search_cnt, srch_rate_gov_dcy
    
RETURN VALUE   None.

SIDE EFFECTS   srch_rate_search_cnt is zeroed
               srch_rate_gov_dcy is adjusted up or down.

===========================================================================*/

extern void srch_adj_srch_gov
(
  word  elapsed_time_ms
    /* Period over which to compute average search rate */
);


/*===========================================================================

FUNCTION SRCH_SET_SRCH_GOV

DESCRIPTION    This module inserts delay, as needed, into back-to-back
      search processing in order to slow down the average search rate.
      It is needed in order to keep processor loading under control when
      using small search window sizes.

DEPENDENCIES   srch_rate_search_cnt, srch_rate_gov_dcy
    
RETURN VALUE   None.

SIDE EFFECTS   REX timers may be set.

===========================================================================*/

extern void  srch_set_srch_gov ( void );

/*===========================================================================

FUNCTION SRCH_GET_FREQ_EST

DESCRIPTION    This function is used to retrieve the frequency estimate
      based on temperature( agc reference ).

DEPENDENCIES   None.

RETURN VALUE   Estimated frequency 

SIDE EFFECTS   None.

===========================================================================*/

extern byte  srch_get_freq_est
(
  int2 temp
    /* Temperature estimate */
);


/*===========================================================================

FUNCTION SRCH_STORE_FREQ_EST

DESCRIPTION    This function is used to store the frequency estimate for
      the the current agc reference ( temperature estimate ).

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   The srch_freq_est table is updated.

===========================================================================*/

extern void srch_store_freq_est( void );



/*===========================================================================

FUNCTION SRCH_FREQ_TIMER_DONE

DESCRIPTION    This function called by the callback timer when the frequency
      estimate timer has expired and it is time to store the current
      frequency.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   The SRCH_FREQ_TIMER_SIG signal is set.

===========================================================================*/

extern void srch_freq_timer_done
(
  int4 ms_interval
    /* Time interval since last expiration - not used */
);


/*===========================================================================

FUNCTION SRCH_POWERDOWN

DESCRIPTION    This function is called to write the frequency offset table to
      NV upon a powerdown.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Frequency offset table is written to NV.

===========================================================================*/

extern void srch_powerdown( void );


/*===========================================================================

FUNCTION SRCH_ENG_TO_ECIO

DESCRIPTION    This function converts an energy into the 6-bit Ec/Io
      value required by CAI 2.0 in section 6.6.6.2.2 for pilot strength
      measurements.  It works by doing a binary search on the ecio_tab
      to find the closest value (without going over), and returning
      the index in the ecio_tab.

DEPENDENCIES   None.

RETURN VALUE   The 6-bit Ec/Io value.  If the energy is out of range,
      the return value is clipped to 0 or 63.

SIDE EFFECTS   None.

===========================================================================*/

extern byte  srch_eng_to_ecio
(
  word eng,
    /* The energy to be converted */

  const word ecio_tab[]
    /* Pointer to the conversion table to be used */
);


/*===========================================================================

FUNCTION SRCH_ECIO_TO_ENG

DESCRIPTION    This function converts a 6-bit Ec/Io value (per CAI 2.0
      section 6.6.6.2.2) into an energy in demod chip units.  It just
      performs a table lookup on ecio_tab (clipping values that are
      too large).

DEPENDENCIES   None.

RETURN VALUE   The energy.

SIDE EFFECTS   None.

===========================================================================*/

extern word  srch_ecio_to_eng
(
  byte  ecio,
      /* The Ec/Io value to convert */

  word ecio_tab[]
      /* Pointer to the conversion table to be used */
);



/*===========================================================================

FUNCTION SRCH_NEXT_RSET_PILOT

DESCRIPTION    This function finds the next pilot offset in the
      Remaining Set.  The Remaining Set is defined to contain all
      multiples of srch_parms.pilot_inc that are not in the Active,
      Candidate, or Neighbor Sets.

DEPENDENCIES   None.

RETURN VALUE   The next pilot offset, or NO_PILOT if there are no
      pilots in the Remaining Set.

SIDE EFFECTS   Various demod chip registers are written.

===========================================================================*/

extern int2  srch_next_rset_pilot
(
  int2  old_pilot
    /* The previous pilot offset */
);


/*========================================================================

FUNCTION SRCH_ADD_NSET_PILOT

DESCRIPTION    This function adds a pilot to the Nset and handles
      full sets ala IS-95.
   
DEPENDENCIES   srch_nset

RETURN VALUE   None.

SIDE EFFECTS   srch_nset and srch_nset_cnt is modified.

========================================================================*/

extern void srch_add_nset_pilot
(
  srch_sect_struct_type *sect_ptr,
    /* Sector we want to add to Nset */

  byte set
    /* Set from whence it came */
);


/*===========================================================================

FUNCTION SRCH_NSET_DUP

DESCRIPTION    This function checks a temporary neighbor set list to
      see if a given pilot is already in the list.

DEPENDENCIES   None.

RETURN VALUE   The index of the matching entry if there is one,
               -1 if not.

SIDE EFFECTS   None.

===========================================================================*/
extern word  srch_nset_dup
(
  srch_tmp_nset_struct_type  tmp_nset[ ],
    /* The temporary neighbor set list to search */

  word  cnt,
    /* Number of entries in the temporary neighbor set list */

  int2 pilot,
    /* Pilot offset value to search for */

  boolean freq_incl,
    /* Freq include flag on the pilot */

  byte band_class,
    /* Band Class of this pilot */

  int2 freq
    /* Frequency of this pilot */
);

/*===========================================================================

FUNCTION SRCH_PILOT_NEW

DESCRIPTION    This function checks the pilots list in a trpt struct
      for a given pilot.  If the pilot is present, it returns its index
      in the list.  If the pilot is not present, it is added and NO_PILOT
      is returned.

      This is used to keep track of what has been reported to the base
      station.  For each sector, fake pilot value PILOT_SELF is put on this
      list to mark the sector itself as having been reported to the base.
      Where this sector is being compared to other sectors for certain
      conditions, a report of this sector relative to another sector is
      marked by putting that other sector's pilot on this list.

DEPENDENCIES   None.

RETURN VALUE   The index in the trpt.pilots list if there is a match, or
               NO_PILOT if not.

SIDE EFFECTS   The pilot may be added to the trpt.pilots list.

===========================================================================*/
extern int2  srch_pilot_new
(
  srch_trpt_struct_type  *trpt_ptr,
    /* Pointer to the trpt struct we're to check and maybe modify */

  int2  pilot
    /* The pilot offset value we're to look for */
);

/*===========================================================================

FUNCTION SRCH_SECT_FIND

DESCRIPTION    This function searches the a given table of sectors (linearly)
      for a sector that matches the given pilot offset.

DEPENDENCIES   srch_sects

RETURN VALUE   The index into srch_sects of the matching sector,
      or NO_SECTOR if no matching sector was found.

SIDE EFFECTS   None.

===========================================================================*/

extern word  srch_sect_find
(
  srch_sect_struct_type  sects[ ],
    /* Pointer to the sector table to be searched */

  word  cnt,
    /* Number of entries in the sector table */

  int2 pilot,
    /* Pilot offset value to look for in the sector table */

  boolean freq_incl,
    /* Freq include flag on the pilot */

  byte band_class,
    /* Band Class of this pilot */

  int2 freq
    /* Frequency of this pilot */
);


/*===========================================================================

FUNCTION SRCH_SECT_DELETE

DESCRIPTION    This function removes a given pilot from a set.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_sect_delete
(
  srch_sect_struct_type   set[],
    /* Pointer to the set array */

  word  *set_cnt_ptr,
    /* Pointer to the count for the set */

  word  which
    /* Index of the element to be removed from the set */
);


#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#else /* FEATURE_SRCH_SCHEDULER */
/*===========================================================================

FUNCTION SRCH_SECT_CLR

DESCRIPTION    This function is used to discard the results of any sweep,
      when we're not sure if the sweep has completed or not, or even if
      there is one to discard.  If the sweep is still in progress, it
      waits for it to complete before releasing its window buffer.

DEPENDENCIES   srch_dump_isr_vars

RETURN VALUE   None.

SIDE EFFECTS   srch_dump_isr_vars

===========================================================================*/

extern void  srch_sect_clr ( void );


/*===========================================================================

FUNCTION SRCH_SECT_STOP

DESCRIPTION    This function is used to IMMEDIATELY STOP and discard the
      results of a searcher sweep. Note that after this is used, the searcher
      freezes in its tracks.

DEPENDENCIES   srch_dump_isr_vars

RETURN VALUE   None.

SIDE EFFECTS   REX signals SRCH_DUMP_ISR_SIG, SRCH_LOST_DUMP_SIG are
      cleared, srch_dump_isr_vars is modified.

===========================================================================*/

extern void  srch_sect_stop ( void );


#endif /* FEATURE_SRCH_SCHEDULER */
/*===========================================================================

FUNCTION SRCH_GET_STOP_SLEW

DESCRIPTION    This function gets the amount of slew needed to
      "trim-up" searcher position to that expected when the
      previous search was performed. This is designed to be
      called sometime after stopping a search in its tracks
      rather than waiting for it to finish.

DEPENDENCIES   None.

RETURN VALUE   int4 slew to add to next search to bring
               searcher back up to expected position before
               the stop.

SIDE EFFECTS   Triggers a searcher status dump

===========================================================================*/

extern int4 srch_get_stop_slew( void );


/*===========================================================================

FUNCTION SRCH_SECT_SET_PARMS

DESCRIPTION    This function is used to load common demodulation parameters
      into a sector structure. These are: 

           1.  Gain in the searcher hardware 
           2.  Number of chips to integrate coherently 
           3.  Number of integrations to add non-coherently 
           4.  Number of chips before early dump test 
           5.  Threshold for early dump 

      Loading these values is done frequently in ACQ, SC, PC, TC states 
      so it makes sense to have a function to do it rather than explicitly
      loading all these every time.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_sect_set_parms
(
  srch_sect_struct_type  sect[ ], 
    /* The sector to fill searcher demod parameters into */

  word  s, 
    /* The index of the sector to fill in */

  const srch_sect_parm_struct_type  *parms
    /* The parameters to fill into the sector structure */
);


/*===========================================================================

FUNCTION SRCH_LOG_AGC_PCTL_ENA

DESCRIPTION    This function is invoked to enable logging of AGC and power
      control values.

DEPENDENCIES   srch_agc_pctl_vars

RETURN VALUE   None.

SIDE EFFECTS   Messages can be sent to DIAG task.

===========================================================================*/

extern void  srch_log_agc_pctl_ena ( void );


/*===========================================================================

FUNCTION SRCH_LOAD_WIN_PARMS

DESCRIPTION    This function is called to load the over-air
      window parameters into the srch_sect structures. It must be
      called at certain correct times so that index calculations
      during srch dump processing are correct.
      
DEPENDENCIES   Over-air window parms.

RETURN VALUE   None.

SIDE EFFECTS   Sector structures are updated.

===========================================================================*/

extern void srch_load_win_parms
(
  srch_set_enum_type set
    /* The set which we should load win parms of */
);


/*===========================================================================

FUNCTION SRCH_SET_PARM

DESCRIPTION    This function is called to implement a command to set
      certain parameters that control the behavior of the searcher task.

DEPENDENCIES   None.

RETURN VALUE   Command status, always SRCH_DONE_S (good status).

SIDE EFFECTS   srch_parms is updated.

===========================================================================*/

extern word  srch_set_parm
(
  srch_parm_cmd_type  *cmd_ptr
    /* Pointer to the Set Search Parameters command */
);

/*===========================================================================

FUNCTION SRCH_RPT_TA

DESCRIPTION    This function formats and sends a temporal analyzer
      report for a given sector, if logging for the specified set
      is enabled. 

      Also, path packet packaging and rate equalization is performed.
      This is a modified version of the old "srch_rpt_ta()" in srch.c 
      It is added here to localize support for path packet logging.

DEPENDENCIES   srch_fing_buf_q
                     srch_ta_parm

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

void  srch_rpt_ta
(
  srch_sect_struct_type  *sect_ptr,
    /* Pointer to the sector to report on */

  word  set_mask,
    /* Mask indicating which set the sector belongs to */

  word  sect_inx,
    /* Sector index */

  srch_path_struct_type  paths[ ],
    /* Array of paths */
    /* Optional; if NULL path info is not stored */

  word  cur_path_cnt,
    /* Number of entries already in the paths array */
    /* Set to 0 if paths is NULL */

  word  new_path_cnt,
    /* Number of entries already in the paths array */
    /* Set to 0 if paths is NULL */

  srch_fing_struct_type  fings[ ],
    /* The fingers array */

  word  fing_cnt,
    /* Number of fingers in the fingers array */

  word filt_len
    /* Length of IIR filters used in energy filtering */
);


/*===========================================================================

FUNCTION SRCH_RPT

DESCRIPTION    This function enqueues a specified report from the Search
      Task to the Main Control Task.

DEPENDENCIES   This function assumes that the 'mcc_srch_q' has been properly
      initialized.
  
RETURN VALUE   None.

SIDE EFFECTS   The MC Task dequeues and processes the specified report.

===========================================================================*/

extern void  srch_rpt
(
  mccsrch_rpt_type  *rpt_ptr
    /* Pointer to searcher report. */
);


/*===========================================================================

FUNCTION SRCH_WAIT

DESCRIPTION    This function just performs a REX wait as specified by the
      caller, except that it automatically kicks the watchdog timer every
      so often while waiting.

DEPENDENCIES   Relies on SRCH_RPT_TIMER_SIG being raised often enough to
      avoid watchdog alarms, so should not be called until srch_init has
      run.

RETURN VALUE   Returns the value returned by rex_wait.

SIDE EFFECTS   The watchdog gets kicked periodically.

===========================================================================*/

extern rex_sigs_type  srch_wait
(
  rex_sigs_type  sigs
    /* Mask of signals to wait for */
);


/*===========================================================================

FUNCTION SRCH_GET_CMD

DESCRIPTION    This function gets a command for the searcher task.
      It just gets one off the srch_cmd_q.

DEPENDENCIES   srch_cmd_q

RETURN VALUE   Pointer to the command.

SIDE EFFECTS   srch_cmd_q

===========================================================================*/

extern srch_cmd_type  *srch_get_cmd ( void );


/*===========================================================================

FUNCTION SRCH_POST_CMD

DESCRIPTION    This function post-processes a command.  It reports
      errors to the diagnostic monitor.  It handles returning the
      command to a queue and/or signalling a task as specified in
      the command itself.  It maintains a count for debug purposes
      of how many times each return value was returned.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   A command is posted back to MC.

===========================================================================*/

extern void  srch_post_cmd
(
  srch_cmd_type  *cmd_ptr,
    /* Points to the command to perform post processing on. */

  srch_cmd_status_type  status
    /* Status to post for command. */
);


#ifdef FEATURE_DH

/*===========================================================================

FUNCTION SRCH_GET_RAND_BUF

DESCRIPTION     This function is only called when a buffer has been requested
      from DH as an akey seed.  The buffer takes multiple sweeps to fill, so
      aset, nset, and rset data may be used.  One exception is that the buffer
      is filled as soon as the searcher hits PC, after that it is requested
      from DH.

DEPENDENCIES    Data may not all be valid if a sweep was not first conducted,
      which replaced the early-dump thresholds with the desired values for the
      necessary data.

RETURN VALUE    Whether or not the buffer has been filled.

SIDE EFFECTS

===========================================================================*/

extern void srch_get_rand_buf ( void );
#endif



/*===========================================================================

FUNCTION SRCH_ROLL_CHECK

DESCRIPTION    Tests the hardware for a pending ROLL interrupt

DEPENDENCIES   None.

RETURN VALUE   TRUE if a ROLL has been signalled.

SIDE EFFECTS   None.

===========================================================================*/

extern boolean srch_roll_check( void ) ;


/*===========================================================================

FUNCTION SRCHZZ_NO_SLOT_ULPN( )

DESCRIPTION    This function ensures that a ULPN value will be recorded at
               the next 80ms slot boundary, whether it is the mobile's
               assigned slot, or not.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Any stored ULPN value is forgotten.

===========================================================================*/

void srchzz_no_slot_ulpn( void );

/*===========================================================================

FUNCTION SRCHZZ_CAPTURE_SLOT_ULPN( )

DESCRIPTION    This procedure monitors the slot time, and captures the ULPN
   state at moments which correspond to the mobile's assigned page slot.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

void srchzz_capture_slot_ulpn ( void );


#ifdef FEATURE_QUARTER_ROLL_TIMELINE
#error code not present
#else /* ! FEATURE_QUARTER_ROLL_TIMELINE */
/*===========================================================================

FUNCTION SRCH_DELAY_LAST_FINGER

DESCRIPTION     This function reserves the last finger for establishing a
                delayed timeline for standby-improvements.

DEPENDENCIES    Several.

RETURN VALUE    None.

SIDE EFFECTS    Last finger is reserved, disabled, and move 1/4 PN circle
                later than the other fingers.

===========================================================================*/

extern void srch_delay_last_finger ( void );

/*===========================================================================

FUNCTION SRCH_UNDELAY_LAST_FINGER

DESCRIPTION     This function unreserves the last finger which was used to
                establishing a delayed timeline for standby-improvements.

DEPENDENCIES    Several.

RETURN VALUE    None.

SIDE EFFECTS    Last finger is unreserved, enabled, and move back near the
                other fingers.

===========================================================================*/

extern void srch_undelay_last_finger ( void );

#endif /* FEATURE_QUARTER_ROLL_TIMELINE */

/*===========================================================================

FUNCTION SRCH_ROLL_ISR

DESCRIPTION    This function is invoked to service the 26.6667 millisecond
      interrupt generated by the demodulator chip.

DEPENDENCIES   This function assumes that pre- and post-interrupt processing
      with respect to stacks and rex are taken care of by the caller.

RETURN VALUE   None.

SIDE EFFECTS   timestamp clock is ticked forward.
               SRCH_ROLL and TXC_ROLL signals are set.
               srch_roll_isr_vars fields are updated.
               AGC/pctl logging may be enabled or disabled.

===========================================================================*/

extern void  srch_roll_isr ( void );

/*===========================================================================

FUNCTION SRCH_DUMP_ISR

DESCRIPTION    This function is invoked to service the end of search
      interrupt generated by the demodulator chip.

DEPENDENCIES   This function assumes that pre- and post-interrupt processing
      with respect to stacks and rex are taken care of by the caller.

RETURN VALUE   None.

SIDE EFFECTS   SRCH_DUMP_ISR signal is set.
               srch_dump_isr_vars fields are updated.

===========================================================================*/

extern void  srch_dump_isr ( void );


/*===========================================================================

FUNCTION SRCH_GET_WIN_POS

DESCRIPTION    This function gets the searcher window position.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   The window position.

SIDE EFFECTS   None.

===========================================================================*/

extern int4 srch_get_win_pos ( void );


#ifdef FEATURE_SEARCH2

/*===========================================================================

FUNCTION SRCH_SELECT_HW

DESCRIPTION    This function will set the desired searcher hardware to be
      used and will slew the new searcher to match the position of the
      current searcher, if desired.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

void srch_select_hw
(
  srch_hw_search_type srch_hw,
    /* Signals which searcher we want to use from now on */

  boolean sync
    /* True or False whether we want to sync the searcher positions or not.
       The new searcher will be slewed to the current searcher position. */
);
#endif /* FEATURE_SEARCH2 */


/*===========================================================================

FUNCTION SRCH_SEARCH_RESET_PTRS

DESCRIPTION    This function resets the 16x hardware queue pointers (if used)
      and also resets the search queues that are used for all searches.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_search_reset_ptrs ( void );
  
#ifdef  FEATURE_SRCH_SCHEDULER
#error code not present
#else /* FEATURE_SRCH_SCHEDULER */
/*===========================================================================

FUNCTION SRCH_SECT_Q_DMP

DESCRIPTION    This function retrieves the results of a 16x searcher sweep
      and places them in the s/w queue for later processing.  This function
      just gets the values out of registers in the searcher.

DEPENDENCIES   srch_dmod_reset should have been previously called,
               srch_dump_isr_vars is used.

RETURN VALUE   None.

SIDE EFFECTS   srch_dump_isr_vars is updated.
               Various demod chip registers written.

===========================================================================*/

extern void  srch_sect_q_dmp ( void );

/*===========================================================================

FUNCTION SRCH_SECT_DMP

DESCRIPTION    This function retrieves the results of a searcher sweep
      and places them in the specified sector struct.  The bulk of the
      results were DMA'd in by the searcher.  This function just gets
      a few values out of registers in the searcher.

DEPENDENCIES   srch_dump_isr_vars
               srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   srch_dump_isr_vars

===========================================================================*/

extern void  srch_sect_dmp ( void );

#endif /* FEATURE_SRCH_SCHEDULER */
/*===========================================================================

FUNCTION SRCH_SECT_ADD_STRUCT

DESCRIPTION    This function adds a search to the searcher software queue.
      The search is specified by the data within the given search structure
      and the accompanying slew.  Once the search is loaded into the
      software queue, an attempt is made to program the search into the
      searcher hardware, if space is available in the given hardware mode.

DEPENDENCIES   srch_dmod_reset should have been previously called.
      A buffer must be available from the srch_search_free_q to not impact
      the search scheduling at the task level.

RETURN VALUE   None.

SIDE EFFECTS   A queue item is removed from the srch_search_free_q and
      put on the srch_search_program_q.

===========================================================================*/

extern void  srch_sect_add_struct
(
  srch_sect_struct_type  *sect_ptr,
    /* Pointer to the sector that specifies the sweep */

  int4  slew,
    /* Amount to slew the searcher before beginning the sweep */

  boolean dump
    /* Indicate whether or not this search should trigger a status dump */
);

#ifdef FEATURE_SEARCH2
/*===========================================================================

FUNCTION SRCH_SECT_ADD

DESCRIPTION    This function adds a search to the searcher software queue.
      The search is specified by the accompanying data which is passed into
      the function.  Once the search is loaded into the software queue, an
      attempt is made to program the search into the searcher hardware, if
      space is available in the given hardware mode.

DEPENDENCIES   srch_dmod_reset should have been previously called.
      A buffer must be available from the srch_search_free_q to not impact
      the search scheduling at the task level.

RETURN VALUE   None.

SIDE EFFECTS   A queue item is removed from the srch_search_free_q and
      put on the srch_search_program_q.

===========================================================================*/

extern void  srch_sect_add
(
  srch_sect_struct_type  *sect_ptr,
    /* Pointer to the sector that specifies the sweep */

  srch_set_enum_type  set,
    /* The pilot set this search belongs to */

  int2 pilot,
    /* Pilot PN */

  boolean age,
    /* Should this pilot search be aged */

  byte nc_int,
  byte gain,
  byte chps,
    /* Non-coherent integrations, gain, and chips for this search */

  int4 winsiz,
    /* The window size to use */

  boolean dump,
    /* Indicates whether or not to generate a status dump after this search */

#ifdef FEATURE_SRCH_SLEW_IN_DRV
#error code not present
#else
  int4  exp_pos,
    /* The expected position after this search is complete */

  int4  slew
    /* Amount to slew the searcher before beginning the sweep */
#endif
);
#endif /* FEATURE_SEARCH2 */

#if defined(FEATURE_GPSONE) && defined(FEATURE_SEARCH2) && \
    defined(FEATURE_MSM5105_PPM_SRCH_ENGINE)
#error code not present
#endif /* FEATURE_GPSONE && FEATURE_SEARCH2 && FEATURE_MSM5105_PPM_SRCH_ENGINE */

/*===========================================================================

FUNCTION SRCH_SECT_FREE

DESCRIPTION    This function is called by the task level to free the queue
      buffers that have been filled with search results at the drivers level.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_sect_free ( void );

/*===========================================================================

FUNCTION SRCH_TRIG_STAT_DMP

DESCRIPTION    This function is used to trigger a status dump if a dump
     interrupt got lost along the way. If no search was pending, it is
     assumed that the dump isr will just ignore this status dump.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   MSM status dump is triggered.

===========================================================================*/
extern void  srch_trig_stat_dmp ( void );

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#else  /* FEATURE_SRCH_SCHEDULER */
/*===========================================================================

FUNCTION SRCH_TRIG_DMP_ERR

DESCRIPTION    This function is used to trigger a status dump if a dump
     interrupt got lost along the way. If no search was pending, it is
     assumed that the dump isr will just ignore this status dump.
     An error message is printed from this function.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   MSM status dump is triggered.

===========================================================================*/
extern void  srch_trig_dmp_err ( void );

#endif /* FEATURE_SRCH_SCHEDULER */

/*===========================================================================

FUNCTION SRCH_FING_MOVE

DESCRIPTION    This function moves the fingers in time, without disturbing
               other finger registers.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Demod chip registers are written.

===========================================================================*/

extern void srch_fing_move
(
  word fing,
    /* Which finger to move */

  int4 slew
    /* Slew value to move fingers */
);

/*===========================================================================

FUNCTION SRCH_FING_CMD

DESCRIPTION    This function commands the demodulating fingers in the
      demod chip to match the parameters called for in the fings array.
      Only fingers with new_ass set (fingers with a new assignment) are
      commanded, but otherwise there is no attempt to minimize the number
      of writes.  Everything on each newly-assigned finger is set.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_fing_cmd
(
  srch_fing_struct_type  fings[ ],
    /* The array of finger parameters */

  word  fing_cnt
    /* The number of fingers in the array */
);


#ifdef T_MSM5000
/*===========================================================================

FUNCTION SRCH_FING_CFG_QOF

DESCRIPTION    This function configures QOF for a given channel on a finger.
 
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void  srch_fing_cfg_qof
(
  word f,
    /* Finger to configure */

  byte  channel,
    /* Channel to configure */
  
  word  qof_mask_id
    /* QOF mask id */
);

/*===========================================================================

FUNCTION SRCH_FING_CFG_FCH_QOF

DESCRIPTION    This function configures QOF for FCH.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_fing_cfg_fch_qof
(
  srch_fing_struct_type  fings[ ],
    /* The array of finger parameters */

  byte  fing_cnt,
    /* The number of fingers in the array for supplemental code channels 
       MSM3000 only three can be used */
  
  byte  qof_mask_id
    /* QOF mask id */
);

/*===========================================================================

FUNCTION SRCH_RESET_QOF

DESCRIPTION    This function resets QOF mask id's.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void  srch_reset_qof( void );

/*===========================================================================

FUNCTION SRCH_FING_RESET_QOF

DESCRIPTION    This function resets QOF for all fingers. 
      
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void  srch_fing_reset_qof
(
  srch_fing_struct_type  fings[ ],
    /* The array of finger parameters */

  byte  fing_cnt
    /* The number of fingers in the array for supplemental code channels 
       MSM3000 only three can be used */
);

/*===========================================================================

FUNCTION SRCH_GEN_MANUAL_ROLL

DESCRIPTION    This function generates an emulated 26.66 msec roll at which 
               the sleep controller starts sleeping.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Bit PN_ROLL_CTL in SYMB_COMB_CTL0 register must be cleared so 
               that the sleep controller can sleep on a regular 26.66 msec 
               roll.

===========================================================================*/
extern void srch_gen_manual_roll( void );
  /* srch_gen_manual_roll */

/*===========================================================================

FUNCTION SRCH_CLR_MANUAL_ROLL

DESCRIPTION    This function directs the sleep controller to use symbol
               combiner rolls instead of manually-generated rolls.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_clr_manual_roll( void );
  /* srch_clr_manual_roll */
#endif /* T_MSM5000 */

#if defined( FEATURE_IS2000_REL_A ) || defined( FEATURE_IS2000_P2 )
#error code not present
#endif /* FEATURE_IS2000_REL_A || FEATURE_IS2000_P2 */

#ifdef FEATURE_IS2000_QPCH
/*===========================================================================

FUNCTION SRCH_TRANS_QPCH_TO_PC

DESCRIPTION    This function arms a slam to finger 0 at the slot boundary
      if the combiner offset is currently not aligned with system time.
      The ULPN for the slot is also loaded to take effect at the slot
      boundary.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Symbol combiner may slam on the next finger roll and ULPN
      will be set on the next slam/roll.

===========================================================================*/
extern void srch_trans_qpch_to_pc( void );
#endif /* FEATURE_IS2000_QPCH */


#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION SRCH_FING_SUP_INIT

DESCRIPTION    This function initiates the demodulating fingers' supplemental 
               code channel registers. 
      
DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip supplemental code registers.

===========================================================================*/
 extern void  srch_fing_sup_init ( void );


/*===========================================================================

FUNCTION SRCH_4TH_FING_ENABLE

DESCRIPTION    This function enables or disables the 4th finger. 
 
      
DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   srch_fing_cnt value changes. The 4th finger registers.

===========================================================================*/

void srch_4th_fing_enable
(
   boolean fourth_fing_enable
);


#endif
/*===========================================================================

FUNCTION SRCH_FING_DMP

DESCRIPTION    This function gets the status of the demodulating fingers.

DEPENDENCIES   srch_dump_isr_vars
               srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_fing_dmp
(
  srch_fing_struct_type  fings[ ],
    /* The fingers array */

  word  fing_cnt
    /* How many fingers in the array */
);


/*===========================================================================

FUNCTION SRCH_FING_FORCE_LOCK

DESCRIPTION    This function forces a particular finger into lock, and
      removes the force-lock on a particular finger. 

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Finger control registers are written.

===========================================================================*/

extern void srch_fing_force_lock
(
  word fing,
    /* Which finger we want to force into lock */

  boolean in_lock
    /* TRUE if we want to force this finger into lock, else remove force lock */
);

/*===========================================================================

FUNCTION SRCH_FING_DISABLE

DESCRIPTION    This function disables a specific finger.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Demod chip registers are written.

===========================================================================*/

extern void srch_fing_disable
(
  word fing
    /* Which finger to disable */
);

/*===========================================================================

FUNCTION SRCH_FING_RESTART

DESCRIPTION    This function restarts a finger after sleep, by resetting the
      RSSI, enabling the finger, and ensures the finger starts out-of-lock.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Demod chip registers are written.

===========================================================================*/

extern void srch_fing_restart
(
  word fing
    /* Which finger to disable */
);


/*===========================================================================

FUNCTION SRCH_FING_SET_RSSI

DESCRIPTION    This function writes a value into the finger RSSI register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Demod chip registers are written.

===========================================================================*/

extern void srch_fing_set_rssi
(
  word fing,
    /* Which finger to write to */

  byte rssi
    /* RSSI value to write to finger */
);


/*===========================================================================

FUNCTION SRCH_FING_TTRK_CLR

DESCRIPTION    This function clears the time tracking accumulator.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_fing_ttrk_clr ( void );

/*===========================================================================

FUNCTION SRCH_SET_SYM_SIZ

DESCRIPTION    This function sets the symbol size in the demodulating
      fingers and the symbol combiner. This operates both on fingers
      ans symbol combiner as we want them to change symbol size together.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_set_sym_siz
(
  srch_state_enum_type  state
    /* Mode of combiner operation desired */
);


/*===========================================================================

FUNCTION SRCH_COMB_ARM_SLAM

DESCRIPTION    This function arms the symbol combiner to
      align itself in time with ("slam to") the specified finger timing.
      The actual alignment occurs on the next PN roll of that finger.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_comb_arm_slam
(
  word  f_ref
    /* The reference finger to align combiner timing with */
);

/*===========================================================================

FUNCTION SRCH_COMB_SLAM_TIMELINE

DESCRIPTION    This function arms the symbol combiner to
      align itself in time with ("slam to") the specified finger timing.
      The actual alignment occurs on the next PN roll of that finger.
      The symbol combiner phase is set to the given phase after the
      slam takes effect.

DEPENDENCIES   srch_dmod_reset should have been previously called.
      Neither the symbol combiner nor the finger we are slamming to should
      be about to roll.

RETURN VALUE   None.

SIDE EFFECTS   Combiner control registers are written.

===========================================================================*/
extern void  srch_comb_slam_timeline
(
  word  f_ref,
    /* The reference finger to align combiner timing with */

  word  new_phase,
    /* Desired phase after slam takes effect */

  word  new_offset
    /* Offset from system timing, in 2-chip units */
);

/*===========================================================================

FUNCTION SRCH_COMB_SET_PN_PHS

DESCRIPTION    This function sets the symbol combiner to a desired
               phase80 value.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Combiner control registers are written.

===========================================================================*/
extern void srch_comb_set_pn_phs
(
  word phase
    /* Phase of 80ms we want to set the combiner to */
);


/*===========================================================================

FUNCTION SRCH_COMB_TTRK_ENA

DESCRIPTION    This function enables the symbol combiner time tracking
      circuits.  The symbol combiner can track time in two ways.
         1. It tries to keep its time aligned with a given reference
            finger (typically the earliest one) by slewing to match
            the finger's movements.  Finger time tracking has a strictly
            limited (by the CAI) rate.

         2. It can try to compensate for a fixed oscillator frequency error
            by slewing 1/8 chip every so often to compensate for the
            cumulative errors in time.  This compensation can be much
            larger than finger time tracking.

      This function enables only the first kind of time tracking.  In
      the production mobile and portable, the frequency tracking loop
      compensates the CHIPX8 clock itself, so there's no need for a
      time tracking correction.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.
      If a slam is pending, the reference finger is stored, and
      assigned after the slam takes effect.

===========================================================================*/

extern void  srch_comb_ttrk_ena
(
  word  fing
    /* Reference (earliest) finger, to be tracked by the combiner. */
);


/*===========================================================================

FUNCTION SRCH_COMB_TTRK_DIS

DESCRIPTION    This function disables the symbol combiner time tracking
      circuits.  It disables both kinds of combiner time tracking.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.
      If a slam is pending, the time tracking is disabled after the
      slam takes effect.

===========================================================================*/

extern void  srch_comb_ttrk_dis ( void );


/*===========================================================================

FUNCTION SRCH_SET_ULPN_STATE

DESCRIPTION    This function sets the demod long code state register to a value.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Demod long code state register is written.

===========================================================================*/

extern void  srch_set_ulpn_state
(
  ulpn_type  lc_state
    /* PN long code state to load into the state register */
);

/*===========================================================================

FUNCTION SRCH_COMB_FTRK_ACQ

DESCRIPTION    This function shifts the symbol combiner freq tracking into
               acq or track mode, using PDM mode for freq combiner output.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Combiner control registers are written.

===========================================================================*/

extern void  srch_comb_ftrk_acq
(
  boolean acq_mode
    /* If TRUE, shift frequency tracking into ACQ mode, else TRACK mode */
);


/*===========================================================================

FUNCTION SRCH_GET_CARRIER_FREQ

DESCRIPTION    This function returns the current value of the carrier frequency,
      which is effectively the high byte of the frequency tracking accumulator.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern byte  srch_get_carrier_freq ( void );


/*===========================================================================

FUNCTION SRCH_SET_CARRIER_FREQ

DESCRIPTION
  This function sets the value of the high byte of the carrier frequency
  tracking accumulator.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_set_carrier_freq
(
  byte freq
    /* The frequency we want to write into the accumulator */
);


/*===========================================================================

FUNCTION SRCH_GET_FRQ_TRK_ACC

DESCRIPTION    This function returns the current value of the frequency
      tracking accumulator.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   The accumulator value.

SIDE EFFECTS   None.

===========================================================================*/

extern word  srch_get_frq_trk_acc ( void );


/*===========================================================================

FUNCTION SRCH_SET_FRQ_TRK_ACC

DESCRIPTION    This function sets the value of the carrier frequency
      tracking accumulator.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_set_frq_trk_acc
(
  word accum
    /* The full 16-bit accumulator value we want to write */
);


/*===========================================================================

FUNCTION SRCH_PARK_TRK_LO_ADJ

DESCRIPTION    This function places TRK_LO_ADJ in an appropriate inactive
      state getting ready for sleep or restores normal operation.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void srch_park_trk_lo_adj
(
  boolean park_it
    /* Indication (TRUE/FALSE) of whether we want to park
       TRK_LO_ADJ for sleep mode. */
);


/*===========================================================================

FUNCTION SRCH_CONFIG_MSM

DESCRIPTION    This function determines which MSM2x we are running, and
               configures searcher accordingly. Specific action needs to be
               taken for versions of MSM2.3 and greater.

DEPENDENCIES   None. 

RETURN VALUE   None.

SIDE EFFECTS   Searcher may change its search schedule, run in 8x mode, 
               disable DMA, use a new DM packet, use hardware peak sort
               at all times, use a different dynamic governer.....whew. 
               
===========================================================================*/

extern void  srch_config_msm ( void );
  
/*===========================================================================

FUNCTION SRCH_PCTL_ENA

DESCRIPTION    This function enables and disables puncturing and
      closed-loop power control.

DEPENDENCIES   BIO software switches.
               srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_pctl_ena
(
  boolean enable
    /* TRUE if we want to enable puncturing/pctl, else disable both */
);

/*===========================================================================

FUNCTION SRCH_SET_AGC_CDMA

DESCRIPTION    This function sets the I/Q offset PDMs to receive data from
      the AGC unit.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   IQ_OFFSET_CTL register.

===========================================================================*/
extern void srch_set_agc_cdma ( void );

/*===========================================================================

FUNCTION SRCH_SET_PWR_COMB_THRSH

DESCRIPTION    This function sets the power combiner threshold of a finger.
               It is for MSM23 only.
               
DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   A threshold of 0 does not qualify the lock state to the power
               combiner.

===========================================================================*/

extern void  srch_set_pwr_comb_thrsh 
(
  word fing,
    /* Finger to set */

  byte thresh
    /* Power combiner threshold of the finger */
);

#if defined(FEATURE_FING_MERGE_DETECT) || defined(FEATURE_HW_FING_MERGE_DETECT)
/*===========================================================================

FUNCTION SRCH_FING_MERGE_DETECT

DESCRIPTION    This function checks all the finger positions and 
  lock status. If two in-lock fingers are too close in position to one 
  another, we deassign the weaker one, leaving the stronger one to demodulate
  the energy path. This minimizes cases when there is concern that having
  more than one finger riding on a multipath will somehow interfere with
  data demodulation or power control combining.

DEPENDENCIES   srch_fings, srch_fing_cnt, srch_state

RETURN VALUE   None.

SIDE EFFECTS   Updates finger positions in srch_fings structure.
               May deassign one or more fingers.

===========================================================================*/
extern void srch_fing_merge_detect ( void );

#endif /* FEATURE_FING_MERGE_DETECT || FEATURE_HW_FING_MERGE_DETECT */

#ifdef FEATURE_HW_FING_MERGE_DETECT
#error code not present
#endif /* FEATURE_HW_FING_MERGE_DETECT */

#if defined(FEATURE_IS95B_PPSMM) || defined(FEATURE_IS95B_MAHHO)
/*===========================================================================

FUNCTION SRCH_GET_MOVING_PSD

DESCRIPTION    This function writes a moving average power spectral density
         value of serving frequency to the passed variable.
 
DEPENDENCIES    srch_moving_psd.

RETURN VALUE    TURE if the moving average is available,
                FALSE if it is not available:
                  1) The calcualtion is not enabled, OR
                  2) The samples is less than SRCH_CAI_N12M (10 ). 
      
SIDE EFFECTS    None.

===========================================================================*/

extern boolean srch_get_moving_psd 
(
  int2 *psd
  /*  Store the moving average of rssi in dB */
);

/*===========================================================================

FUNCTION SRCH_RESET_MOVING_PSD

DESCRIPTION    This function will also reset all the previous stored psd 
               sample values.
 
DEPENDENCIES    srch_moving_psd.

RETURN VALUE    None. 
      
SIDE EFFECTS    None.

===========================================================================*/
extern void srch_reset_moving_psd(void);

/*===========================================================================

FUNCTION SRCH_SAVE_PSD_SAMPLE

DESCRIPTION    This function saves the most recent received power spectral 
         density sample and discards the oldest sample.

DEPENDENCIES    srch_moving_psd.

RETURN VALUE    None. 
      
SIDE EFFECTS    None.

===========================================================================*/

extern void srch_save_psd_sample( void );

/*===========================================================================

FUNCTION SRCH_SET_TOTAL_ENG

DESCRIPTION    This function is called to calculate the total energies of
         a set.
            
DEPENDENCIES   srch_?set[], srch_?set_cnt..

RETURN VALUE   Sum of total energies.

SIDE EFFECTS   None.

===========================================================================*/
extern word srch_set_total_eng
(
  srch_set_enum_type  set
  /* The set which we should calculate the total energies */
);

#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO */

#if defined(FEATURE_IS95B_PPSMM) || defined(FEATURE_IS95B_MAHHO) || defined(FEATURE_GPSONE)
/*===========================================================================

FUNCTION SRCH_ENABLE_MOVING_PSD

DESCRIPTION    This function enables the power spectral density moving 
         average calculation for the serving frequency. And it will also
         reset all the previous stored psd sample values.
 
DEPENDENCIES    srch_moving_psd.

RETURN VALUE    None. 
      
SIDE EFFECTS    None.

===========================================================================*/

extern void srch_enable_moving_psd
(
  boolean enable
  /* TRUE, enable it. FALSE, disable it. */
);
#endif /* FEATURE_IS95B_PPSMM || FEATURE_IS95B_MAHHO || FEATURE_GPSONE */

#ifdef FEATURE_IS95B_SYS_RESELEC
/*===========================================================================

FUNCTION SRCHPC_SYS_RESELECT_INIT

DESCRIPTION    This function initializes system re-selection process.

DEPENDENCIES   srch_aset.

RETURN VALUE   None.  

SIDE EFFECTS   Initilizes srchpc_rssi, srchpc_rssi_ecio, srchpc_rssi_low_cnt, 
               srchpc_rssi_samp_time.
===========================================================================*/
void srchpc_sys_reselect_init( void );


/*===========================================================================

FUNCTION SRCHPC_SYS_RESELECT_CHECK

DESCRIPTION    This function checks system re-selection condition.

DEPENDENCIES   srch_aset.

RETURN VALUE   None.  

SIDE EFFECTS   Updates srchpc_rssi, srchpc_rssi_ecio, srchpc_rssi_low_cnt, 
               srchpc_rssi_samp_time.
===========================================================================*/
boolean srchpc_sys_reselect_check( void );

#endif

/*===========================================================================

FUNCTION SRCH_SET_RSSI_TIMER

DESCRIPTION    This function is called to set the srch_rssi_timer.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   srch_rssi_timer is set.

===========================================================================*/
extern void srch_set_rssi_timer
(
  rex_timer_cnt_type time
    /* The time to set the timer for */
);

#ifdef FEATURE_IS95B_MAHHO
/*===========================================================================

FUNCTION SRCH_SET_TIMER

DESCRIPTION    This functions sets a SRCH internal timer. 

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_set_timer
(
  srch_timer_type *timer,
    /* Pointer to the timer be set */

  word count
    /* Counter value */
);
/*===========================================================================

FUNCTION SRCH_CLR_TIMER

DESCRIPTION    This functions clears a SRCH internal timer. 

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_clr_timer
(
  srch_timer_type *timer
    /* Pointer to the timer be cleared */
);
/*===========================================================================

FUNCTION SRCH_TICK_TIMER

DESCRIPTION    This functions ticks down a SRCH internal timer. 

DEPENDENCIES   None.

RETURN VALUE   boolean, TURE if ticks is not equal zero.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_tick_timer
(
  srch_timer_type *timer
    /* Pointer to the timer be ticked */
); 
/*===========================================================================

FUNCTION SRCH_TIMER_EXPIRED

DESCRIPTION    This functions checks a SRCH internal timer expired. 

DEPENDENCIES   None.

RETURN VALUE   boolean, TURE if it's expired.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_tick_expired
(
  srch_timer_type *timer
    /* Pointer to the timer be checked */
);
/*===========================================================================

FUNCTION SRCHTC_CFS_NUM_SRCH

DESCRIPTION    This function calculates the numebr of CF search per
               one visit.

DEPENDENCIES   srchtc_cfs_sw_time, srch_msm_srch_speed, 
               srchtc_cf_sect_parm.

RETURN VALUE   Number of Cf neighbor search per visit.

SIDE EFFECTS   None.

===========================================================================*/
extern word srchtc_cfs_num_srch
(
  word max_win_siz,
    /* The maximum window size of the neighbors */

  word num_cfnset,
    /* Number of the total CF neighbor set */

  word msm_speed,
    /* MSM searcher speed */

  word *visit_period
    /* pointer to visit period */

);
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

#ifdef FEATURE_SLEEP_SLOW_CLOCK
/*===========================================================================

FUNCTION SRCH_BYPASS_INTERFACE

DESCRIPTION    For MSM3000B and newer chips, this function is designed to
      bypass the uP interface for a period of 1.25msec before the -26.7msec
      PN roll, until the roll actually occurs.  The function first determines
      the number of slow clock counts which will be equivalent to the desired
      1.25msec.  This value is then compared vs. the sleep controller value
      (which can be read in MSM3000B and newer MSMs).  Once the 1.25msec
      barrier is reached, the uP interface is bypassed until the -26.7msec
      PN roll is detected.  The bypass is then disabled and the function
      returns.
               
DEPENDENCIES   The sleep controller must be in use.  Interrupts must not
      be locked when calling this function.

RETURN VALUE   None.

SIDE EFFECTS   The uP interface is temporarily bypassed.

===========================================================================*/

extern void srch_bypass_interface ( void );

#endif /* FEATURE_SLEEP_SLOW_CLOCK */

#ifdef T_MSM5000

/*===========================================================================


FUNCTION SRCH_ENABLE_DEMOD_BYPASS

DESCRIPTION    For MSM5000B and newer chips, this function is designed to
      bypass the demod uP interface before the -26.7msec PN roll, until the roll
      actually occurs.  
                     
DEPENDENCIES   

RETURN VALUE   None.

SIDE EFFECTS   The demod uP interface is temporarily bypassed.

===========================================================================*/

extern void  srch_enable_demod_bypass ( void );

/*===========================================================================

FUNCTION SRCH_DISABLE_DEMOD_BYPASS

DESCRIPTION    For MSM5000B and newer chips, this function is designed to
      disable the demod uP interface bypass after the first PN roll after
      wake up.  
                     
DEPENDENCIES   

RETURN VALUE   None.

SIDE EFFECTS

===========================================================================*/

extern void  srch_disable_demod_bypass ( void );

#endif /* T_MSM5000 */

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION SRCH_ATMR_ISR

DESCRIPTION    This function is invoked to service the ATMR interrupt. It
      sets the searcher Action Time signal within the 20 msec frame just 
      before the action time.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   SRCH_ACTION_TIME_SIG may be set.

===========================================================================*/
extern void srch_atmr_isr( void ); 

#ifdef FEATURE_IS2000_QPCH
/*===========================================================================

FUNCTION SRCH_QPCH_INIT

DESCRIPTION    This function initializes the chip for QPCH functionality.

DEPENDENCIES   TBD.

RETURN VALUE   None.

SIDE EFFECTS   TBD.

===========================================================================*/
extern void srch_qpch_init
(
  word walsh
    /* The walsh channel to set */
); /* srch_qpch_init */

/*===========================================================================

FUNCTION SRCH_QPCH_ARM_DECODE

DESCRIPTION
  This funtion initializes the Quick Paging Channel state, and arms the
  hardware to decode the required OOK bit.

DEPENDENCIES
  TBD
  
RETURN VALUE
  None.

SIDE EFFECTS
  TDB.

===========================================================================*/
extern void srch_qpch_arm_decode
(
#ifdef FEATURE_PLT
#error code not present
#else
  void
#endif /* FEATURE_PLT */
); /* srch_qpch_arm_decode */

/*===========================================================================

FUNCTION SRCH_QPCH_DECODE_BIT

DESCRIPTION
  This procedure processes the Quick Paging Channel OOK data received,
  from the decoder.
  
DEPENDENCIES
  TBD

RETURN VALUE
  None.

SIDE EFFECTS
  TBD

===========================================================================*/
extern srch_ook_decision_type srch_qpch_decode_bit
( 
  srch_ook_data_type *ook_data,
    /* OOK I and Q soft decision and pilot strength estimate */
  int1 power_level,
    /* QPCH power relative to Pilot power index */
  byte rate 
    /* QPCH rate */
); /* srch_qpch_decode_bit */

/*============================================================================

FUNCTION       SRCH_QPCH_WRAPUP

DESCRIPTION    This function complete the demodulation of the QPCH bits.
               
DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   TBD 
============================================================================*/

extern void srch_qpch_wrapup( void );

/*===========================================================================

FUNCTION SRCH_QPCH_LOG_DATA

DESCRIPTION    This function formats and sends a QPCH log report,
      if QPCH logging is enabled and DIAG can provide the necessary
      buffer size.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_qpch_log_data
(
  srch_ook_bit_type  * ook_bit_ptr,
    /* Pointer to the OOK bit data */

  word  pi_bit,
    /* The PI bit to log */

  boolean stop
    /* TRUE if not continuing with QPCH so must return the log buffer */
);


#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_START

DESCRIPTION    This function initializes the channel estimator FIR filter
               with the RSSI reading when entering slotted mode for the first 
               time. Upon subsequent entry into slotted mode, this function
               uses the RSSI reading as one of the filter input to do channel 
               estimation. 
               It is called when the phone go into slotted mode from
               non-slotted mode and just before going to sleep.

DEPENDENCIES   srchzz_sci.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_start(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_DECISION

DESCRIPTION    This function makes a decision whether to enable/disable QPCH.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern boolean srch_qpch_ch_est_decision(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_PI1_MEAS

DESCRIPTION    This function takes the peak energies and the pilot strength
               estimate after demodulation of PI1 as input for the filter.
               It is called before we decode the PI1 bit.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_pi1_meas(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_PI2_MEAS

DESCRIPTION    This function takes the peak energies and the pilot strength
               estimate after demodulation of PI2 as input for the filter.
               It is called before we decode the PI2 bit.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_pi2_meas(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_SLOT_MEAS

DESCRIPTION    This function takes the RSSI reading at the end of paging message
               demodulation in slotted mode as input for the filter. It is called
               after we finish demodulating the paging message and before we go 
               back to sleep.
               
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_slot_meas(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_NON_SLOT_MEAS

DESCRIPTION    This function takes the RSSI reading when the phone is in 
               non-slotted mode as input for the filter. It uses a timer that
               is decremented every roll and when the timer expired the RSSI
               is read. It is called from the roll done event processing in
               the idle state.
                              
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_non_slot_meas(void);

/*===========================================================================

FUNCTION       SRCH_QPCH_CH_EST_CALC

DESCRIPTION    This function calculates all of the filter's input to estimate
               the Ec/Io of the next slot. This Ec/Io estimate is used to 
               decide whether to enable/disable QPCH for the next slot.
               It is called just before we go back to sleep.
                
DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_qpch_ch_est_calc(void);

#endif /* FEATURE_IS2000_QPCH */

#ifdef FEATURE_IS2000_SCH
/*===========================================================================

FUNCTION SRCHTC_PROCESS_SCH_TRANSACTION

DESCRIPTION    This function is called to process SCH transactions requested by
               MC task.  The transaction must only contain one action, which must
               be SCH.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srchtc_process_sch_transaction
(
  mctrans_cmd_type*  trans_ptr
);
#endif /* FEATURE_IS2000_SCH */

#endif /* FEATURE_IS2000 */

#ifdef FEATURE_RELATIVE_DMOD_POS
/*===========================================================================

FUNCTION SRCH_ROTATE_CIRCLE

DESCRIPTION    This function adjusts the searcher and fingers to account for
      a shift in the PN circle timimg, possibly due to sleeping for non-PN
      roll lengths.

DEPENDENCIES   srch_dmod_reset should have been called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void  srch_rotate_circle
(
  int4 resid_chips
    /* Amount to rotate the circle by */
);
#endif /* FEATURE_RELATIVE_DMOD_POS */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*===========================================================================

FUNCTION      SRCHZZ_ENABLE_AND_INIT_RF_DIGITAL

DESCRIPTION    This function enables and initializes the RF hardware for CDMA 
               by calling rf_enable_digital_rx_pwr() and rf_init_digital().
               It is called by wakeup ISR.
               
DEPENDENCIES   rfmsm.c.
               srchzz_slot_catnap_len. 

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srchzz_enable_and_init_rf_digital ( void );

#endif /* SRCHI_H */

