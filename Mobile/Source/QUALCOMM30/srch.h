#ifndef SRCH_H
#define SRCH_H
/*===========================================================================

             S E A R C H   T A S K   H E A D E R    F I L E

DESCRIPTION
  This file contains declarations for use by the Search task, and should
  be included by all modules within the Search task, and by any other task
  which desires to use externally-visible Search-owned functions.

    Copyright (c) 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
                  2001
                    by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/srch.h_v   1.28.2.0   24 May 2001 13:15:00   evanmell  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/24/01   ejv     Removed srch_get_cdma_raw_ecio() from FEATURE_JCDMA.
05/03/01   ejv     Removed srch_t_add_abort_cmd_type.
05/03/01   fc      Removed SRCH_T_ADD_ABORT_ON_F and SRCH_T_ADD_ABORT_OFF_F.
05/01/01   br      Added SRCH Scheduler support.
04/20/01   ejv     Added support for USE_T_ADD_ABORT.
04/18/01   ejv     Enable pilot_rec/_type in Nset update cmd for IS2000.
03/28/01   br      Changed PPM database parameter.
03/05/01   ejv     Updated srch_nset_cmd_type with FEATURE_IS2000_REL_A.
03/02/01   br      Added FEATURE_GPS_MODE to featurize GPS operation.
                   Added FEATURE_PPM_DB_1 and FEATURE_PPM_DB_2 to separate
                   different PPM database processing algorithms for
                   different targets.
02/28/01   ejv     Rearranged a structure for better packing.
02/27/01   ejv     Added FEATURE_IS2000_REL_A.
02/27/01   br      Added FEATURE_SEARCH3 to featurize searcher 3 operation.
02/27/01   pj      Removed SRCH_QDSP_DLOAD_DONE_SIG.
02/26/01   sko     Added srch_get_ref_pn().
02/15/01   bgc     Changed FTM to a runtime mode.
02/15/01   rdh     Added RESELECT_MEAS command for SD2.0, shuffled sys meas 
                   command struct members to conform to MC convention.
02/14/01   rdh     Added initial support for System Determination 2.0.
01/26/01   ych     Merged T-53/JCDMA features for SK.
01/22/01   ejv     Mainlined T_SLEEP.
01/08/01   ejv     Added new sub-structures for channel parameters.
11/21/00   bgc     Updated for PLT support.
11/06/00   ejv     Increased Cset to support new drivers interface for ACQ.
09/21/00   pj      Added factory test mode in SA data message from PDSM.
09/12/00   ejv     Removed #ifdef's from around srch_access_inform();
09/07/00   lc      Deleted function prototypes for srchtc_proc_sch_trans()
                   and srchtc_proc_ho_trans().
09/06/00   bc      Added FEATURE_FACTORY_TESTMODE.  
09/01/00   day     Change reference to new sensitivity assist data structure.  
09/01/00   pj      Added SRCH_GPS_SRCH_DONE_ISR_SIG.  
08/24/00   pj      Added new QDSP download signals.
08/21/00   ejv     Removed FEATURE_PRE_95B_BUILD, SRCH_PRE_IS95B_BUILD,
                   FEATURE_PILOT_SCANNER, and FEATURE_SRCH_DYNAMIC_BAND_CLASS.
08/06/00   br      Added support for SRCH3 lost status dump and GPSONE.
08/04/00   br      Added additional FEATURE_GPSONE support.
07/28/00   fc      Mainlined FEATURE_CS_ACTION_TIME_TRANS.
07/16/00   abh     Added Signals for GPS DSP Ready & GPS Dump ISR.
07/16/00   abh     Added the fields for Acq Assist & Sensitivity Assist
                   data in the message from PDSM.
07/07/00   eh/fc   Added support for SCRM processing. 
07/07/00   dgy     Added a flag that indicates whether to update CFS set.
06/29/00   abh/br  Added MSM3300 support - FEATURE_GPSONE and FEATURE_SRCH_BATCH_LOG.
06/22/00   bgc     Replaced include file mctrans.h with mctrans_cmd.h.
06/20/00    eh     Swap OOK_RATE_4800bps and OOK_RATE_9600bps definition.
06/12/00   lc/dgy  Added FEATURE_CS_ACTION_TIME_TRANS to streamline the processing of 
                   MC transactions and to support soft handoff transactions. 
06/12/00   ejv     Externed new function srch_set_fpc_mode.
05/26/00   ejv     Added slot masks and cur_slot_mask parameter to sleep cmd.
05/25/00   ks      Added support for Neighbor Search Window offsets.
05/12/00   day     Check that MC state is not in access state using srch_access_inform 
                   before sending threshold report.
05/03/00   wli     Added FEATURE_NONSLOT_OFREQ_NSRCH
04/24/00   dgy     Added full support for SCH, including handoff, Active set 
                   updating, and action time processing.
04/21/00   ejv     Added prototype for new function srch_update_rc.
04/10/00   dgy     Added FEATURE_IS2000_QPCH.
03/14/00   ejv     Added FEATURE_BROADCAST_SMS.
02/16/00   lcc     Modified srch_tc_aset_type to contain SCH info.
02/14/00   ejv     Added function srch_set_tx_rate_adj.
02/02/00   dgy     Added support for action time processing.
12/13/99   ajn     Re-Added FEATURE_ENHANCED_STANDBY_III & QUARTER_ROLL_TIMELINE
                   support.
12/13/99   ejv     Changed srch_get_dmod_id to return a word instead of byte.
12/08/99   va/dgy  Added handoff interface for FEATURE_IS2000.
11/29/99   ejv     Externed srch_freeze_iq for RF.
11/24/99   ajn     TEMP_HACKS_FOR_TIGER removed
11/08/99   dgy     Added FCH QOF support for handoffs.
10/27/99   ajn     Added Signal for QPCH operation
10/21/99   eh      Added SRCH_PLT_DEMOD_QPCH_FRAME_F and srch_set_demod_qpch_frame_cmd_type
10/20/99   dgy     Added support for QOF.
10/20/99   ejv     Removed cfs_seq from srch_cf_nset_cmd_type.
10/14/99   wjn     Deleted code that defines/undefines
                   FEATURE_SRCH_DYNAMIC_BAND_CLASS within the scope of Search
                   files. 
10/07/99   lcc     Merged from PLT 2.0:
           jcw     Removed rf_present flag since it isn't used.
           eh      add hdr to srch_set_lc_mask_cmd_type
           rm      Added new Interface for SCH (for PLT).
09/30/99   yus     IS95B code review cleanup.
09/14/99   wjn     Defined/Undefined FEATURE_SRCH_DYNAMIC_BAND_CLASS within 
                   the scope of Search files.
08/25/99   wjn     Defined SRCH_RUNTIME_CSET_MAX and SRCH_RUNTIME_MAX_CAN_CNT 
                   macros so that the maximum Candidate size is determined at 
                   run time, depending on P_REV.
08/23/99   ejv     Mainlined FEATURE_ENHANCED_STANDBY_II.
08/12/99   yus     Added function prototype srch_cfs_off_time_fwd  
                   (under FEATURE_IS95B_MAHHO.)
08/12/99   yus     Changed FEATURE_IS95B_CFS and FEATURE_IS95B_HARD_HO to
                   FEATURE_IS95B_MAHHO.
08/06/99   ejv     Extended FEATURE_PRE_95B_BUILD.  Merged yshi's FEATURE_
                   IS95B_CFS support.  ajn: Mainlined FEATURE_VIRTUAL_
                   COMBINER.  wjn: Added FEATURE_SRCH_DYNAMIC_BAND_CLASS.
07/28/99   yus     Added PACKED for internal struct of srch_walsh_struct_type.
07/26/99   ejv     yus: Made srch_walsh_struct_type PACKED.  Added structure
                   srch_walsh_struct_type for displaying walsh codes on the DM.
07/24/99   jcw     Merged in support for PLT.
07/23/99   wjn     Added function prototype 
                   srch_set_dmod_pilot_gain( byte band_class ) for run time
                   CDMA band class switches.
07/19/99   ml/dgy  Added support for PLT.
07/01/99   yus     De-coupling FEATURE_SPECIAL_MDR with FEATURE_IS95B_PILOT_RPT
                   if SRCH_PRE_IS95B_BUILD is defined.
06/28/99   yus     Merged IS95B features to the ASW tip.
06/28/99   yus     Added #ifdef SRCH_PRE_IS95B_BUILD for pre-95b backward build.
06/10/99   yus     Merged IS95B Hard Hardoff ( w/o Candidate Frequency Search.)
06/09/99   rdh     Merged FEATURE_PILOT_SCANNER, FEATURE_SCAN_FING_LOG. 
06/04/99   ejv     Unwrapped srch_get_rx_agc from being conditionally compiled.
05/24/99   yus     Changed add_intercept and drop_intercept to int1 from byte.
05/24/99   yus     Added IS95B Periodic PSMM feature.
04/27/99   yus     Changed SRCH_MAX_CAN_CNT to 10 for IS95B build.
04/22/99   yus     Changed { #if defined(FEATURE_IS95B_MDR) || 
                   defined(FEATURE_SPECIAL_MDR)} to {#if defined(FEATURE_IS95B) 
                   || defined(FEATURE_SPECIAL_MDR)} in case of of win_n and win_r
                   in srch_tc_aset_cmd_type. 
04/21/99   yus     Added win_n and win_r in srch_tc_aset_cmd_type.
03/22/99   kmp     Merged the changes listed below from the TGP IS-95B baseline.
                   03/10/99   yus     Added access_attempted field.
02/26/99   yus     Merged IS95B features to ARM based code( see below for the 
                   features.)
                   12/07/98   yus     Added support for Extended Channel 
                                      Assignment Message.
                   11/25/98   yus     Added P_REV checking.
                   11/25/98   yus     Added IS95B Neighbor search enhancements.
                   11/25/98   yus     Added IS95B Access handoff and Access 
                                      Entry handoff support.
                   11/25/98   yus     Added IS95B Pilot reporting.
                   11/25/98   yus     Changed #ifdef FEATURE_JSTD008 to run time 
                                      checking.
                   10/26/98   yus     Added win_n and win_r into 
                                      srch_tc_aset_cmd_type structure.
                   09/23/98   yus     Added IS95B soft handoff algorithm.
                   09/23/98   yus     Added IS95B system reselection in Idle 
                                      feature.
                   09/23/98   yus     Included mc.h for P_REV checking.                  
02/03/99   ejv     yus: Merge updates for MDR features.
02/03/99   ejv     yus: Added IS95B MDR support.
02/03/99   ejv     lcc: Extracted part of tc_set and tc_update_sup commands 
                   into types.
02/02/99   ejv     Changed T_VIRTUAL_COMBINER to FEATURE_VIRTUAL_COMBINER.
02/01/99   ejv     Merged changes for FEATURE_ENHANCED_STANDBY_II, including
                   function prototypes for srch_get_fing_rssi and srch_fast_
                   sleep_inform.  Also added FEATURE_EXT_LINEARIZER.
01/13/99   ejv     ajn: Added knowledge about time reference slamming to
                   srch_comb_slam_timeline & added srch_get_vsc_time( )
01/06/99   ejv     Removed _cdecl - not necessary for ARM.
09/28/98   ejv     Added _cdecl to force C calling to specific functions
                   for Pascal builds.
09/18/98   ejv     Added target.h to list of include files.
08/18/98   ejv     Added dh task support, must define FEATURE_DH.
07/30/98   bgc/mk  Added prototype for srch_finger_lock_handler().
03/04/98   yus/smo Added support for Turbo/1x switching, srch_select_8x.
06/19/97   rdh     Added T_Q, removed pre-NGP support, updated copyright.
05/20/97   rdh     Added Low Power Mode support.
03/21/97   smo     Added customer.h include for FEATURE_JSTD008
03/14/97   jjn     Added (TG==T_MD) to the appropriate conditional compile
                   statements for the Module
11/22/96   rdh     Added Hutch's T_T target definitions.
09/19/96   rdh     Added unconditional PC state idle handoff throttling.
08/15/96   rdh     Added idle handoff delay timers/sigs.
06/11/96   rdh     Added SRCH_OFF/ON_TX commands for PSMM holdoff if TX off.
05/08/96   smo     Added JSTD-008 features and definitions.
05/01/96   rdh     Split Csize for real and pre-Cans, add parm to rude_wakeup.
04/09/96   rdh     Added prototype for function to flag search for erasures.
04/01/96   jmk     Added func srch_rude_wakeup to rouse the phone from sleep.
02/28/96   rdh     Added win_a field for HO Dir Mssg command. IS-95 issue.
02/28/96   rdh     Added srch_start_pc_state called interface.
11/30/95   rdh     Added fade report typedef and packet type.
09/07/95   rdh     Passing pilot set energies to DIAG in IS-95 format.
08/28/95   rdh     Let governor have its own REX signal instead of muxing.
08/23/95   rdh     Pre- code review cleanup checkin to save RAM/ROM.
08/18/95   rdh     Added srch_get_pilot_set DIAG support.
07/17/95   rdh     Added pctl Punct rate set function for MSM 2p.
06/16/95   rdh     Added powerdown command so Search can tidy up on powerdown.
06/15/95   rdh     Moved all chipx8 bring-down code into new RXC sleep ISR.
06/01/95   rdh     Mods for search driver break-up.
05/09/95   rdh     Added search speed throttling to allow all window sizes.
04/14/95   rdh     Added CDMA RSSI processing for all targets.
04/13/95   rdh     Gemini change: Provide callable AGC/power control log fnct.
03/30/95   rdh     Merged in lost dump recovery timer code.
03/30/95   rdh     Added srch_control_chipx8_clk to fix runt pulse problem.
03/23/95   rdh     Added duration field to SLEEP_F structure for deep sleep.
01/06/95   rdh     IS-95 Phase III: Added REX sigs for handoff/slew throttling.
11/01/94   rdh     Added PC_ASSIGN command structure gottem from Don.
10/27/94   rdh     Added decl for srch_notify_bad_slot.
10/24/94   rdh     Add PN mask autoswitch capability.
07/30/94   rdh     Add CDMA RSSI declaration for ISS1.
08/09/93   ptw/jah Added signal for sleep, and srch_powerdown() prototype.
10/12/92   ptw     Changes for CAI 2.0+ compliance.
07/20/92   jai     Moved report types to mccsrch.h.
07/16/92   ptw     Added signal definitions, etc. formerly in task.h
07/14/92   ptw     Changed to new cmd_hdr.
06/11/92   ptw     Added some support for slotted mode.  Removed some fossils.
01/31/91   ptw     Added support for pilot strength measurements for forward
                   link power control.
01/22/91   ptw     Added more extensive commenting.
07/20/91   rdb     Got rid of TA report to diag task, added new params to
                   paging channel command.   (Rev 1.2)
06/25/91   rdb     Lots of changes to support jump from sync channel timing
                   to traffic channel timing. (Rev 1.1)
06/02/91   rdb     Initial revision.  (Rev 1.0)
05/21/91   rdb     Revamped on eve of first cut integration.
03/20/91   rdb     Created file.

===========================================================================*/


/* Just in case the .C file didn't include all the right stuff... */
#include "comdef.h"
#include "queue.h"
#include "qw.h"
#include "ulpn.h"
#include "cmd.h"
#include "enc.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "target.h"
#include "mc.h"
#include "cai.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm_cmdi.h"
#endif /* FEATURE_FACTORY_TESTMODE */

#ifdef FEATURE_PLT
#error code not present
#endif

#ifdef FEATURE_IS2000
#include "deint.h"
#include "mctrans_cmd.h"
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_IS2000
#include "caii.h"
#endif /* FEATURE_IS2000 */


/*===========================================================================

                           DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                             Signal Masks
--------------------------------------------------------------------------*/
#define  SRCH_RF_TUNE_TIMER_SIG  SRCH_REF_TIMER_SIG       /* 0x0200 */
  /* After tuning to a new frequency in the mobile station idle state,
     SRCH waits for the RF circuits to settle using this signal. This
     is not done in TC state, and can be shared with the REF_TIMER_SIG. */ 

#define  SRCH_START_SIG          TASK_START_SIG            /* 0x8000 */
  /* After creation and initialization, the Searcher Task waits for the Main
     Control Task to set this signal before beginning its normal execution. */

#define  SRCH_STOP_SIG           TASK_STOP_SIG             /* 0x4000 */
  /* The Main Control Task sets this signal to tell the Searcher Task to
     stop execution. */

#define  SRCH_OFFLINE_SIG        TASK_OFFLINE_SIG          /* 0x2000 */
  /* This signal is set to tell SRCH to go into offline mode.
     Actually,  this doesn't do anything to SRCH. */

#define  SRCH_GOVERN_SIG         0x1000      
  /* This signal is used to throttle the search max speed. */

#ifdef FEATURE_SRCH_SCHEDULER
#error code not present
#else
#define  SRCH_LOST_DUMP_SIG      0x0800
  /* This signal is set when the "lost status dump" timer has expired. */
#endif /* FEATURE_SRCH_SCHEDULER */

#define  SRCH_CTR_TIMER_SIG      0x0400
  /* This signal is set when the TC state centering slew throttle
     timer has expired. */

#define  SRCH_REF_TIMER_SIG      0x0200
  /* This signal is set when the TC state time reference slew throttle
     timer has expired. */

#define  SRCH_PC_HO_TIMER_SIG      SRCH_REF_TIMER_SIG
  /* This signal is set when the idle handoff delay timer has expired. */

#define  SRCH_PC_THROT_TIMER_SIG   SRCH_CTR_TIMER_SIG
  /* This signal is set when the idle handoff throttle timer has expired. */

#define  SRCH_DELAY_TIMER_SIG      0x0100
  /* This signal is set when the delay timer has expired. */

#define  SRCH_WAKEUP_SIG         0x0080
  /* This signal is set when the NMI handler finds that the sleep
     timer has expired. */

#define  SRCH_ACQ_TIMER_SIG      0x0040
  /* This signal is used to time substates of acquisition. */

#define  SRCH_RSSI_TIMER_SIG     SRCH_ACQ_TIMER_SIG
  /* This signal is used to time CDMA RSSI update periods. It is shared
     with the Acq timer sig as the two uses are mutually exclusive */

#define  SRCH_FREQ_TIMER_SIG     0x0020
  /* This signal is set by the frequency offset timer callback procedure when
     the timer expires */

#define  SRCH_NV_CMD_SIG         0x0010
  /* This signal is set when NV has completed a commmand for the Search task */

#define  SRCH_FAST_SLEEP_SIG    SRCH_NV_CMD_SIG
  /* This signal is set when Search must react to a "Fast-sleep" request */

#define  SRCH_RPT_TIMER_SIG      0x0008
  /* This signal is set when the srch_rpt_timer expires. This timer is used to
     enable SRCH to kick the watchdog on time during a wait for some event. */

#define  SRCH_CMD_Q_SIG          0x0004
  /* This signal is set when an item is put on the srch_cmd_q. */

#define  SRCH_ROLL_ISR_SIG       0x0002
  /* This signal is set by the PN Roll interrupt handler each time a
     PN Roll (26.666... ms boundary of system time according to the
     mobile station's time reference) occurs.  Used by SRCH to synchronize
     events with system time.  */

#define  SRCH_DUMP_ISR_SIG       0x0001
  /* This signal is set by the Status Dump interrupt handler each time
     the searcher hardware completes a programmed sweep. */

#ifdef FEATURE_IS95B_PPSMM
#define SRCH_PPSMM_TIMER_SIG  SRCH_WAKEUP_SIG
  /* This signal is set if the PPSMM timer is expired. It is shared
     with the wakeup timer sig as the two uses are mutually exclusive */
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_QUARTER_ROLL_TIMELINE
#error code not present
#endif

#ifdef FEATURE_IS2000

#ifdef FEATURE_IS2000_QPCH
#define SRCH_OOK_SIG             0x00100000
  /* This signal is set when an OOK bit has been decoded */
#endif /* FEATURE_IS2000_QPCH */

#define SRCH_ACTION_TIME_SIG     0x00010000
  /* This signal is set within the 20 msec frame just before the action time */ 
#endif /* FEATURE_IS2000 */

#define PAGE_SLOT     0x01
  /* Indicates an assigned page slot */

#define BC_PAGE_SLOT  0x02
  /* Indicates an assigned BCSMS page slot */

#define BC_DATA_SLOT  0x04
  /* Indicates a BCSMS data slot */

/*--------------------------------------------------------------------------
                          Queue Declarations
--------------------------------------------------------------------------*/

extern q_type          srch_cmd_q;
   /* This queue receives commands from MCC to SRCH */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */
/*--------------------------------------------------------------------------
                            CAI Constants
--------------------------------------------------------------------------*/

#define  SRCH_ASET_MAX   CAI_N6M    /* Maximum size of the Active Set */
  /* per IS-95 Appendex 2, Table D-2 */

#define  SRCH_MAX_CAN_CNT CAI_N7M  /* Max number of real Candidates by spec */
#define  SRCH_RUNTIME_MAX_CAN_CNT (P_REV_IS_4_OR_GREATER ? 10 : 5)
     /* Max number of real Candidates by spec, depending on P_REV */

  /* per IS-95 Appendex 2, Table D-2 */
#define  SRCH_CSET_MAX  (SRCH_MAX_CAN_CNT + 11)  /* Maximum size of the (pC+C) Set */
#define  SRCH_RUNTIME_CSET_MAX  (SRCH_RUNTIME_MAX_CAN_CNT + 5)
     /* P_REV dependant run time Maximum size of the (pC+C) Set */

#define  SRCH_NSET_MAX    CAI_N8M  /* Maximum size of the Neighbor Set */
  /* per IS-95 Appendex 2, Table D-2 */

#ifdef FEATURE_IS95B
#define  SRCH_CFNSET_MAX  SRCH_NSET_MAX
#endif

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
#define  SRCH_SUP_CHANNEL_MAX 7 /* Max number of supplemental channels by spec */
#endif

#ifdef FEATURE_IS2000_SCH
#define  SRCH_SCH_MAX  CAI_MAX_NUM_FOR_SCH 
  /* Maximum number of Supplemental Channels(SCHs) in IS-2000 */

#define  SRCH_SCH_ASET_MAX  CAI_N6M     
  /* Maximum SCH Active Set size */
#endif /* FEATURE_IS2000_SCH */

#ifdef FEATURE_IS2000_R_SCH
#define SRCH_MAX_SCRM_RPT_PN  CAI_MAX_SCRM_RPT_PN
  /* Maximum PN that can be reported for SCRM */
#endif /* FEATURE_IS2000_R_SCH */

#if defined (FEATURE_IS95B_PILOT_RPT) || defined (FEATURE_SPECIAL_MDR)
#define  SRCH_PILOTS_MAX   ( SRCH_NSET_MAX + 1 )  
  /* Max size of the reported pilot set in PC, which is all possible neighbors
     plus ane active pilot. */
#endif

#ifdef FEATURE_IS95B_MAHHO

#define SRCH_NO_CFS       0x0   /* No CF search    */
#define SRCH_SINGLE_CFS   0x1   /* Single search   */
#define SRCH_PERIODIC_CFS 0x3   /* Periodic search */

#define SRCH_CF_NO_PRI_OR_WIN 0x0 /* No search priorities/windows specified */
#define SRCH_CF_PRI_ONLY      0x1 /* Search priorities specified */
#define SRCH_CF_WIN_ONLY      0x2 /* Search windows specified */
#define SRCH_CF_PRI_AND_WIN   0x3 /* Search priorities and windows specified */

#endif
/*--------------------------------------------------------------------------
         Akey rand bits variables
--------------------------------------------------------------------------*/
#ifdef FEATURE_DH

#define SRCH_MAX_RAND_BUF_SIZE  320
  /* Number of bytes required for akey seed */

extern byte srch_rand_buf[ SRCH_MAX_RAND_BUF_SIZE ];
  /* This is the buffer where we will store the data */

#endif


/*--------------------------------------------------------------------------
                          SEARCH COMMAND CODES

The following codes are for use with the 'cmd' field of the search command
header type defined below, to define the operation requested by the command.
--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_START_F,     /* Back to start mode */
  SRCH_CDMA_F,      /* Enter CDMA mode */
  SRCH_ACQ_F,       /* Acquire pilot PN */
  SRCH_SC_F,        /* Begin sync channel operation */
  SRCH_SLEW_F,      /* Slew timing to align with system time */
  SRCH_PC_F,        /* Begin paging channel operation */
  SRCH_LPM_F,       /* Transition to Low Power mode */
  SRCH_SLEEP_F,     /* Put the system to sleep until next slot */
  SRCH_WAKEUP_F,    /* Initiate a wakeup now */
  SRCH_TC_F,        /* Begin traffic channel operation */
  SRCH_UNSLEW_F,    /* Unslew timing to align to sync channel */
               /* The above commands all request a state change. */

  SRCH_PARM_F,      /* Change search parameters */
  SRCH_MEAS_F,      /* Return Pilot Strength Measurements */
  SRCH_FPC_MEAS_F,  /* Return Forward Power Control Pilot Measurements */
  SRCH_PC_ASET_F,   /* Active set update for paging */
  SRCH_PC_NSET_F,   /* Neighbor set update for paging */
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
#ifdef FEATURE_IS95B
  SRCH_MCC_IDLE_F,        /* MCC's Idle state signal */
  SRCH_MCC_ACC_ENTRY_F,   /* MCC's Access Entry state signal */
  SRCH_MCC_ACC_PROBE_F,   /* MCC's Access (Probe) State signal */
#endif
#ifdef FEATURE_IS95B_PPSMM
  SRCH_START_PPSM_F,      /* Start PPSMM timer */
  SRCH_STOP_PPSM_F,       /* Stop PPSMM timer */
  SRCH_PPSM_F,            /* One time PPSMM reporting */
#endif
#ifdef FEATURE_IS95B_MAHHO
  SRCH_CF_NSET_F,         /* Candidate Frequency Neighbor Infor. */
  SRCH_START_CFS_F,       /* Start Candidate Freq search */
  SRCH_STOP_CFS_F,        /* Stop Candidate Freq search */
  SRCH_CFS_MEAS_F,        /* Perform pilot measurement on CF */
  SRCH_START_RIF_HHO_F,   /* Start IS95B Hard handoff */
  SRCH_RIF_HHO_SUCCESS_F, /* A successful hard handoff */
  SRCH_RIF_HHO_FAILURE_F, /* An unsuccessful hard handoff */
  SRCH_REACQ_ASET_F,      /* Hard handoff failure, restore the SF */
#endif /* FEATURE_IS95B_MAHHO */

#if defined (FEATURE_IS95B_PILOT_RPT) || \
        defined (FEATURE_SPECIAL_MDR)
  SRCH_PILOT_LIST_F,  /* Pilot Reporting request */ 
#endif

  SRCH_TC_ASET_F,   /* Active set update for traffic */
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  SRCH_TC_UPDATE_SUP_F,  /* Supplemental channel update */
#endif
#ifdef FEATURE_IS2000_SCH
  SRCH_TC_ADD_SCH_F,     /* Keep until 3GDATA prototype is not needed */
#endif /* FEATURE_IS2000_SCH */
  SRCH_TC_NSET_F,   /* Neighbor set update for traffic */
  SRCH_PC_ASSIGN_F, /* Search pilot list given in paging channel assign */
  SRCH_POWERDOWN_F, /* MC wants to power down - perform housekeeping */
  SRCH_FADE_F,      /* MC has received fade indication from MC. Deal with it */
  SRCH_OFF_TX_F,    /* TX is off - hold off autonomous PSMMs temporarily */
  SRCH_ON_TX_F,     /* TX is on now - resume reporting autonomous PSMMs */
#ifdef FEATURE_DH
  SRCH_RAND_BITS_F, /* MC wants a buffer for A-key seed */
#endif
#ifdef FEATURE_IS2000
  SRCH_HO_F,        /* MC sends handoff information to Search */
  SRCH_MC_TRANS_F,  /* Transaction request from MC */
#if ( ( defined FEATURE_IS2000_QPCH ) && ( defined FEATURE_PLT ) )
#error code not present
#endif /* FEATURE_IS2000_QPCH && FEATURE_PLT */

#ifdef FEATURE_IS2000_R_SCH
  SRCH_SCRM_MEAS_F, /* SCRM pilot measurements request */
#endif /* FEATURE_IS2000_R_SCH */

#endif /* FEATURE_IS2000 */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifndef FEATURE_PLT
  SRCH_MAX_F     /* Tells number of commands. MUST be last enum item. */
#else
#error code not present
#endif /* FEATURE_PLT */
}
srch_cmd_code_type;

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*--------------------------------------------------------------------------
              STRUCTURES FOR CHANNEL SPECIFIC INFO

--------------------------------------------------------------------------*/

typedef struct
{
  word  walsh_chn;
    /* Walsh channel to be used on the traffic channel with the pilot */

  #ifdef FEATURE_IS2000
  byte  qof_mask_id;
    /* QOF mask ID (0-3), where 0 gives true walsh functions */
  #endif /* FEATURE_IS2000 */

  #ifdef FEATURE_IS2000_P2
#error code not present
  #endif /* FEATURE_IS2000_P2 */
}
srch_fch_info_type;

#ifdef FEATURE_IS2000_P2
#error code not present
#endif /* FEATURE_IS2000_P2 */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
typedef struct
{
  byte num;
    /* Number of supplemental channels, can vary from 0 to 7. */

  word walsh_chn[ SRCH_SUP_CHANNEL_MAX ];
    /* Array of Supplemental Walsh channel to be used on the traffic 
       channel. Can vary from 0 to 7 */
}
srch_scch_info_type;
#endif /* if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */

#ifdef FEATURE_IS2000_SCH
typedef struct
{
  word  walsh_chn;
    /* Walsh channel to be used on the traffic channel with the pilot */

  byte  qof_mask_id;
    /* QOF mask ID (0-3), where 0 gives true walsh function */

  boolean  active; 
    /* Indicates if this SCH is active */
}
srch_sch_info_type;
#endif /* FEATURE_IS2000_SCH */

/*--------------------------------------------------------------------------
              TRAFFIC CHANNEL ACTIVE SET UPDATE COMMAND

This command replaces the Active Set on the traffic channel.
--------------------------------------------------------------------------*/

/* Type definition for each member of the new Active Set. */
/* Include as part of the command */
typedef struct
{
  int2  pilot;
    /* Pilot PN index of this Active Set member */

  #ifdef FEATURE_IS2000_REL_A
#error code not present
  #endif /* FEATURE_IS2000_REL_A */

  srch_fch_info_type fch_info;
    /* Parameters for the FCH */

  #ifdef FEATURE_IS2000_P2
#error code not present
  #endif /* FEATURE_IS2000_P2 */

  #if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  srch_scch_info_type scch_info;
    /* Parameters for the SCCH of this pilot */
  #endif /* if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) */

  #ifdef FEATURE_IS2000
  byte  srch_offset_nghbr;
    /* Neighbor Window Search Offset for the pilot */
  #endif /* FEATURE_IS2000 */

  #ifdef FEATURE_IS2000_SCH
  srch_sch_info_type sch_info[ SRCH_SCH_MAX ];  
    /* Parameters for the SCH of this pilot.
       Maximum of 2 SCH in IS-2000. TIGER2 supports only 1 */
  #endif /* FEATURE_IS2000_SCH */

  byte  pwr_comb_ind;
    /* Flag: nonzero if the power control subchannel on this pilot
       contains the same symbols as that on the previous pilot. */
} srch_tc_aset_type;

/* For IS2000 changes, tc_aset_cmd_type is obsolete, instead the active
   set is retrieved by means of this new srch_tc_aset_info_type data struct */
#ifdef FEATURE_IS2000

/* The following interface is defined for SRCH to make a
   function call into MC to get the active set in case of a
   SHO and SCH assignment at the action time */

typedef struct
{
  word  cdma_freq;
    /* CDMA Channel number */
    
  byte  band_class;         
    /* CDMA Band Class */

  word  aset_cnt;
    /* Number of Active Set members */

#ifdef FEATURE_IS2000_SCH
  boolean use_sch;
    /* Indicates whether to use SCH or SCCH information */
#endif /* FEATURE_IS2000_SCH */

  srch_tc_aset_type aset[ SRCH_ASET_MAX ];
    /* Pilot offset indices and walsh channels for active set members */
}
srch_tc_aset_info_type;
#endif

#ifdef FEATURE_IS2000
/*--------------------------------------------------------------------------
                  TRANSACTION-BASED ACTION PACKET TYPE

This type contains the data for an action in a transaction requested by MC
task. MC task allocates the memory for a pointer to this type and fills
in the data.
--------------------------------------------------------------------------*/

typedef struct mctrans_action_packet_struct srchtc_trans_action_pkt_type;
  /* Data for an action in a transaction */

extern srchtc_trans_action_pkt_type *srchtc_action_pkt_ptr;
  /* Pointer to the data for an action in a transaction */
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */



/*--------------------------------------------------------------------------
                       SEARCH COMMAND STATUS VALUES

The following status codes are for use with the 'status' field of the
search command header type defined below and as the return values for
function srch_cmd.
--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_DONE_S,      /* Function completed successfully */
  SRCH_BUSY_S,      /* Command queued, but not yet completed */
  SRCH_BADPARM_S,   /* Illegal parameter(s) specified for command */
  SRCH_BADSEQ_S,    /* Illegal sequencing of commands */
  SRCH_TMO_S,       /* Command could not be completed in time */
  SRCH_ERR_S,       /* Internal error occurred */
  SRCH_TOOLATE_S,   /* Now is not a good time for that command */
  SRCH_TUNE_S,      /* Command queued, but tuning in progress  */
  SRCH_SLEEP_S,     /* We are asleep. Please come back in the morning */

  SRCH_MAX_S        /* Tells number of status codes. MUST be last item
                       in enum */
}
srch_cmd_status_type;


/*--------------------------------------------------------------------------
                 PILOT SET REQUEST REPORT STRUCTURE

This structure is used in the called interface when the caller desires to
get a read on how many and what pilots are in the Search sets.
--------------------------------------------------------------------------*/

typedef struct
{
  word pilot_inc;             /* Pilot increment for remaining sets */
  byte active_cnt;            /* Count of pilots in active set */
  byte cand_cnt;              /* Count of pilots in candidate set */
  byte neigh_cnt;             /* Count of pilots in neighbor set */

  struct
  {
    word pilot_pn;              /* Pilot offset */
    byte pilot_strength;        /* Pilot strength */
  }
  pilot_sets[ SRCH_ASET_MAX + SRCH_CSET_MAX + SRCH_NSET_MAX ];
    /* Sets of pilots (in the above order) */
}
srch_pilot_set_struct_type;


#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/*--------------------------------------------------------------------------
                 WALSH CODE INFORMATION STRUCTURE

This structure is used in providing walsh code information to DM.
--------------------------------------------------------------------------*/
typedef PACKED struct
{
  word num_aset;        
    /* number of active set */
  
  PACKED struct 
  {
    int2 pilot_off;     
      /* Pilot offset used by serach */
    
    byte num_sup_channels;
      /* Number of supplemental channels */

    word walsh_code[ SRCH_SUP_CHANNEL_MAX + 1 ];    
      /* Specifies which walsh code is being used, including fundamental */

  }ch_data [ SRCH_ASET_MAX ]; 
    /* One for each Aset */

}srch_walsh_struct_type;


#endif
/*--------------------------------------------------------------------------
                          SEARCH COMMAND HEADER TYPE

This header type is contained in every search command. It contains info
common to all commands.
--------------------------------------------------------------------------*/

typedef struct
{
  cmd_hdr_type          cmd_hdr;
    /* Includes the queue links,  pointer to requesting task TCB,
       signal to set on completion,  and done queue */

  srch_cmd_code_type    cmd;
    /* Specifies which command to perform. See enumerated command codes
       above. */

  srch_cmd_status_type  status;
    /* Specifies the status of the command. Initially, this field is set
       to SRCH_BUSY_S by function srch_cmd. When command processing is
       finished, this field is set to one of the statuses enumerated
       above. */
}
srch_cmd_hdr_type;

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */
/*--------------------------------------------------------------------------
                      ENTER START MODE COMMAND

This command must be sent on startup to get the searcher task initialized.
It may be sent again at any time to have the searcher give up on what it
is doing and return to the initial state.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

   /* No further information required. */
}
srch_start_cmd_type;

/*--------------------------------------------------------------------------
                      ENTER CDMA MODE COMMAND

This command is sent to make the transition from START state to CDMA state.
This must be done before beginning to acquire the CDMA signal.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

   /* No further information required. */
}
srch_cdma_cmd_type;


/*--------------------------------------------------------------------------
                      ACQUIRE PILOT PN COMMAND

This command is sent to move from CDMA, SC, or PC states to the ACQ state,
and begin searching for a pilot signal.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

  word  cdma_freq;
   /* CDMA Channel number */

  byte  band_class;
    /* CDMA Band Class */

#ifdef FEATURE_FACTORY_TESTMODE
  ftm_acq_mode_type acq_mode;
    /* The acquisition mode, Full, Mini, Micro */
#endif /* FEATURE_FACTORY_TESTMODE */

}
srch_acq_cmd_type;
/*--------------------------------------------------------------------------
                   BEGIN SYNC CHANNEL OPERATION COMMAND

This command is sent to move from the ACQ, PC, or UNSLEW states
to the SC state, and begin receiving the sync channel.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

   /* No further information required. */
}
srch_sc_cmd_type;

/*--------------------------------------------------------------------------
                      SLEW TO SYSTEM TIMING COMMAND

This command is given to move from the SC state to the SLEW state, and
to begin moving demod chip timing from sync channel timing to system
timing (the jump to hyperspace).  The parameters contain some of the
information received from the sync channel message - just enough to let
us decide how many 26ms periods to slide before reporting (with a
SRCH_SLEW_R report) that we are aligned for the jump to hyperspace on
the next 26ms boundary.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  int2  pilot_index;
    /* Which pilot offset index the acquired cell is using */

  word  phase_80ms;
    /* Which 80 millisecond phase (0,1 or 2) the start of the
       sync channel message was received in */
}
srch_slew_cmd_type;


/*--------------------------------------------------------------------------
                   BEGIN PAGING CHANNEL OPERATION COMMAND

This message is sent immediately after the SRCH_SLEW_R report is received,
to move from the SLEW state to the PC state, make the jump to hyperspace,
and begin receiving the paging channel.  This command MUST arrive between
the 26ms boundary that triggered the SRCH_SLEW_R and the next 26ms boundary.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  qword  sys_time;
    /* System time value extracted from sync message */
    /* This time is applicable on the xth 80ms boundary after the
       start of the sync message, so that's when we will jump to
       hyperspace. */

  qword  pn_state;
    /* PN long code state to be used at hyperspace boundary */

  qword  pn_mask;
    /* PN long code mask to be used on paging channel */

  word  walsh_chn;
    /* Walsh channel number to tune to for the paging channel */

  word  rate;
    /* Data rate for paging channel */

  word  cdma_freq;
    /* CDMA Channel number for paging channel */
  
  byte  band_class;     /* CDMA Band Class */

}
srch_pc_cmd_type;
/*--------------------------------------------------------------------------
                           SLEEP STATE COMMAND

This message is sent while on the Paging Channel to allow the searcher
to put the MSM and itself to sleep until the next Paging Channel slot,
or, it is sent in Start state to tell the search task to deep sleep.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

  byte  slot_cycle_index; /* 0 to 7 as per CAI */
   /* The slot cycle index in use by the base station. This indicates
      how long the mobile can sleep between slots */

  word  pgslot;           /* 0 to 639 as per CAI */
   /* Which slot we're supposed to listen in.  This value has been
      computed from our ESN. */

  word  duration;
   /* Optional deep sleep interval in seconds */

#ifdef FEATURE_BROADCAST_SMS
  byte bcast_index;  /* 0 to 7 as per CAI */
    /* Indicates the cycle that the mobile wakes up on to receive broadcast
       messages or pages */

  qword next_bcast_slot;
    /* Indicates the next paging slot  to wake up to receive broadcast
       messages.  If no more messages, 0 is assigned. */
#endif

  byte cur_slot_mask;
    /* Indicates what type of slot has just finished */
}
srch_sleep_cmd_type;

/*--------------------------------------------------------------------------
                   BEGIN TRAFFIC CHANNEL OPERATION COMMAND

This command is sent to move from PC state to TC state, and switch from
receiving the paging channel to receiving the assigned traffic channel.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  qword  pn_mask;
    /* PN long code mask to be used on the traffic channel */

  word  aset_cnt;
    /* Number of Active Set members */

  /* For each member of the new Active Set: */
  struct
  {
    int2  pilot;
      /* Pilot PN index of this Active Set member */

    #ifdef FEATURE_IS2000_REL_A
#error code not present
    #endif /* FEATURE_IS2000_REL_A */

    srch_fch_info_type fch_info;
      /* Parameters for the FCH */

    #ifdef FEATURE_IS2000_P2
#error code not present
    #endif /* FEATURE_IS2000_P2 */

    byte  pwr_comb_ind;
      /* Flag: nonzero if the power control subchannel on this pilot
         contains the same symbols as that on the previous pilot. */
  }
  aset[ SRCH_ASET_MAX ];
    /* Pilot offset indices and walsh channels for active set members */

  word   cdma_freq;
    /* CDMA Channel number for the traffic channel */

  byte band_class;      /* CDMA Band Class */

}
srch_tc_cmd_type;

#ifdef FEATURE_IS2000_SCH

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  word  num_sch;
    /* Number of SCH */

  /* For each member of the new SCH */
  struct
  {
    int2  pilot;
      /* Pilot PN index of this Active Set member */

    word  walsh_chn;
      /* Walsh channel to be used on the traffic channel with the pilot */

    byte  qof_mask_id;  
      /* QOF mask ID (0-3), where 0 gives true walsh fns) */
  }
  srch_tc_sch_type[ SRCH_SCH_MAX ];  
    /* Pilot offset indices and walsh channels for active set members */
    /* Maximum of 2 SCH in IS-2000. TIGER2 supports only 1 */

}
srch_tc_add_sch_cmd_type;
#endif  /* FEATURE_IS2000_SCH */

#ifdef FEATURE_IS2000_R_SCH
/*--------------------------------------------------------------------------
      SUPPLEMENTAL CHANNEL REQUEST PILOT STRENGTH MEASUREMENTS COMMAND

This message requests that the Searcher generate a Supplemental Channel
Request Pilot Strength Measurement Message.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
  byte t_mulchan;
    /* SCRM pilot strength reporting offset */
}
srch_scrm_meas_cmd_type;
#endif  /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000

/*--------------------------------------------------------------------------
                   TRANSACTION COMMAND FROM MC TASK

This command is sent from MC to SRCH to process a transaction at a specified
time.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  mctrans_cmd_type   cmd;  
    /* Transaction command from MC */
}
srch_mc_trans_cmd_type;
#endif  /* FEATURE_IS2000 */

/*--------------------------------------------------------------------------
                      UNSLEW TO SYNC TIMING COMMAND

This command is sent to move from PC or TC state to the UNSLEW state,
and begin to move back from paging/traffic timing to sync channel timing.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

   /* No further information required. */
} 
srch_unslew_cmd_type;


/*--------------------------------------------------------------------------
                   CHANGE PILOT SEARCH PARAMETERS COMMAND

This command provides the Searcher Task with search parameters from
base station messages.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte  win_a;
    /* Active set search window size index    */

  byte  win_n;
    /* Neighbor set search window size index  */

  byte  win_r;
    /* Remaining set search window size index */

  byte  t_add;
    /* Threshold of energy at which a pilot moves up to the Candidate Set */

  byte  t_drop;
    /* Threshold of energy at which a pilot should be dropped from the
       Active Set or Candidate Set */

  byte  t_comp;
    /* Margin by which a pilot must exceed an Active Set pilot to trigger a
       new Power Measurement Report */

  byte  t_tdrop;
    /* Index of the duration for which a pilot must be below t_drop before
       being dropped or reported */

  byte  nghbr_max_age;
    /* Maximum "age" for a pilot to remain in the Neighbor Set */

#ifdef FEATURE_IS95B_SOFT_HO
  byte soft_slope;
  /* Threshold slope value */

  int1 add_intercept;
  /* T_ADD threshold intercept value, in 0.5dB units */

  int1 drop_intercept;
  /* T_DROP threshold intercept value, in 0.5dB units */
#endif

#ifdef FEATURE_IS95B_SYS_RESELEC      /* System re-selection in Idle state */
  boolean  sys_reselect_included;
  /* Flag tells if the re-selection thresholds included */

  byte  ec_thresh;
  /* Pilot power threshold */

  byte  ec_io_thresh;
  /* Pilot Ec/Io threshold */
#endif
}
srch_parm_cmd_type;


/*--------------------------------------------------------------------------
                 RETURN PILOT STRENGTH MEASUREMENTS COMMAND

This message requests that the Searcher generate a Pilot Strength
Measurement Message,  as if autonomously.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
}
srch_meas_cmd_type;

/*--------------------------------------------------------------------------
       RETURN FORWARD POWER CONTROL PILOT STRENGTH MEASUREMENTS COMMAND

This command is sent to request the information required to send a
Power Measurement Report Message from the Searcher Task.

  Note: this command can fail, returning SRCH_ERR_S status, if the last
      active set update contained duplicates.
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  word  meas_cnt;
    /* When the command is processed, this field is updated
       to reflect the actual number of measurements returned. */

  struct
  {
    byte  ecio;
  }
  meas[ SRCH_ASET_MAX ];
    /* When the command is processed, the current filtered pilot strengths
       of the active set sectors are placed here, in the order of the last
       active set update. */
}
srch_fpc_meas_cmd_type;

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */


/*--------------------------------------------------------------------------
               PAGING CHANNEL ACTIVE SET UPDATE COMMAND

This command tells the searcher to perform an idle handoff from one
Paging Channel pilot to another.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  int2  aset_pilot;
    /* The pilot PN index of the new Active Set pilot */

  word  aset_walsh;
    /* The walsh channel to be used with the new paging channel */

  qword  pn_mask;
    /* PN long code mask to be used on the new paging channel */

  word   cdma_freq;
    /* CDMA Channel number of the new paging channel */
  byte  band_class;     /* CDMA Band Class */

}
srch_pc_aset_cmd_type;

/*--------------------------------------------------------------------------
                      NEIGHBOR SET UPDATE COMMAND

This command updates the Neighbor Set.

Note: this command format is used for both paging and traffic channel
variants.  On the traffic channel, this command is sent for each and
every Neighbor List Update message received.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte  pilot_inc;
    /* Spacing of potential pilot offset in use in this system.  The
       remaining set consists of all pilot offsets which are multiples
       of this value (but are not in another set). */

  word  nset_cnt;  
    /* Number of neighbor set members */

  struct
  {
    int2  pilot;
      /* The pilot PN index of the neighbor set pilot */

    #ifdef FEATURE_IS2000
    caii_add_pilot_rec_type pilot_rec;
      /* Union of possible additional pilot record types */
    #endif /* FEATURE_IS2000 */
    
    byte config;
      /* The Neighbor config flag */

    byte  pri;
      /* The priority of the pilot */
    
    boolean freq_incl;
      /* Flag tells if the frequency information is included */

    byte  band_class;
      /* CDMA Band Class */

    word  cdma_freq;
      /* CDMA Channel number of the pilot */

#if defined (FEATURE_IS95B_PILOT_RPT) || \
            defined (FEATURE_SPECIAL_MDR)
    boolean access_attempted;
      /* Flag tells whether we have attempted to access this pilot.
         Note: SRCH doesn't use this flag, it's merely for MC coding
               convenience */
#endif

#ifdef FEATURE_IS95B
    boolean access_entry_ho;    
      /* If Access Entry HO allowed */
    
    boolean access_ho_allowed;  
      /* If ACCESS HO and Access Probe allowed */
    
    byte  srch_win_nghbr;       
      /* Neighbor window for the pilot */

    #ifdef FEATURE_IS2000
    byte  srch_offset_nghbr;
      /* Neighbor Window Search Offset for the pilot */
    #endif /* FEATURE_IS2000 */

    boolean timing_incl;        
      /* If Timing included for the Neighbor */
    
    byte nghbr_tx_offset;       
      /* Neighbor TX offset */
    
    byte nghbr_tx_duration;     
      /* Neighbor TX Duration */
    
    byte nghbr_tx_period;       
      /* Neighbor TX PERIOD */
#endif
  }
  nset[ SRCH_NSET_MAX ];
    /* Pilot offset indices for neighbor set members */
}
srch_nset_cmd_type;


#ifdef FEATURE_IS2000

/* This command is used for communicating active set to SRCH during a HHO */
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
  
  word  cdma_freq;
    /* CDMA Channel number */
  
  byte  band_class;         
    /* CDMA Band Class */

  word  aset_cnt;
    /* Number of Active Set members */

  srch_tc_aset_type aset[ SRCH_ASET_MAX ];
    /* Pilot offset indices and walsh channels for active set members */
} srch_tc_aset_cmd_type;
#endif  /* FEATURE_IS2000 */

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

#ifdef FEATURE_IS2000
  boolean lcm_change_ind; 
    /* Flag indicating whether to restore long code mask after RIF-HHO */

  qword lcm;
    /* Long code mask */
#endif /* FEATURE_IS2000 */

  word  cdma_freq;
    /* CDMA Channel number */
  
  byte  band_class;
    /* CDMA Band Class */

  byte  win_a;
    /* Active set search window size index */

  byte  t_add;
    /* Threshold of energy at which a pilot moves up to the Candidate Set */

  byte  t_drop;
    /* Threshold of energy at which a pilot should be dropped from the
       Active Set or Candidate Set */

  byte  t_comp;
    /* Margin by which a pilot must exceed an Active Set pilot to trigger a
       new Power Measurement Report */

  byte  t_tdrop;
    /* Index of the duration for which a pilot must be below t_drop before
       being dropped or reported */

#ifdef FEATURE_IS95B_SOFT_HO
  byte soft_slope;
    /* Threshold slope value */

  int1 add_intercept;
    /* T_ADD threshold intercept value, in 0.5dB units */

  int1 drop_intercept;
    /* T_DROP threshold intercept value, in 0.5dB units */
#endif

#if defined(FEATURE_IS95B ) || defined(FEATURE_SPECIAL_MDR)
  byte  win_n;
    /* Neighbor set search window size index  */

  byte  win_r;
    /* Remaining set search window size index */
#endif

  word  aset_cnt;
    /* Number of Active Set members */

  srch_tc_aset_type aset[ SRCH_ASET_MAX ];
    /* Pilot offset indices and walsh channels for active set members */
} /* This data struct is used for HO and Reacq in pre-IS2000 code */
#ifndef FEATURE_IS2000
srch_tc_aset_cmd_type;
#else
srch_tc_reacq_cmd_type;
#endif  /* In IS2000 code, this data struct is used for reacq only */

/* The following is the new command is convey handoff info to SRCH */
#ifdef FEATURE_IS2000
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte transaction_num; 
    /* Transaction num of this HO transaction */
  
  qword action_time;  
    /* Action time when this cmd needs to be acted upon */
  
  boolean hho_ind;  
    /* Indicates if this is a HHO */
  
  boolean rif_ind;  
    /* Indicates if we need to return to SF on failure */

  boolean lcm_change_ind; 
    /* Flag indicating whether to change long code mask */

  qword lcm;
    /* Long code mask */
  
  boolean srch_incl; 
    /* Indicates if the following srch parameters are included in this 
       command */

  byte  win_a;
    /* Active set search window size index , valid only if srch_incl = 1*/

  byte  t_add;
    /* Threshold of energy at which a pilot moves up to the Candidate Set , 
       valid only if srch_incl = 1*/

  byte  t_drop;
    /* Threshold of energy at which a pilot should be dropped from the
       Active Set or Candidate Set valid only if srch_incl = 1*/

  byte  t_comp;
    /* Margin by which a pilot must exceed an Active Set pilot to trigger a
       new Power Measurement Report valid only if srch_incl = 1*/

  byte  t_tdrop;
    /* Index of the duration for which a pilot must be below t_drop before
       being dropped or reported valid only if srch_incl = 1*/

#ifdef FEATURE_IS95B_SOFT_HO
  byte soft_slope;
    /* Threshold slope value valid only if srch_incl = 1*/

  int1 add_intercept;
    /* T_ADD threshold intercept value, in 0.5dB units 
       valid only if srch_incl = 1*/

  int1 drop_intercept;
    /* T_DROP threshold intercept value, in 0.5dB units 
       valid only if srch_incl = 1*/
#endif

#if defined(FEATURE_IS95B ) || defined(FEATURE_SPECIAL_MDR)
  byte  win_n;
    /* Neighbor set search window size index, valid only if srch_incl = 1  */

  byte  win_r;
    /* Remaining set search window size index valid only if srch_incl = 1*/
#endif
}
srch_tc_ho_cmd_type;
#endif /* FEATURE_IS2000 */



#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/*--------------------------------------------------------------------------
              TRAFFIC SUPPLEMENTAL CHANNEL UPDATE COMMAND

This command updates supplemental channels on the traffic channel.
--------------------------------------------------------------------------*/
typedef struct
{
  int2 pilot;
      /* Pilot PN index of this Active Set member */

  byte num_sup_channels;
    /* Number of supplemental channels included in command, can vary 
    from 0 to 7. */

  word sup_walsh_codes[ SRCH_SUP_CHANNEL_MAX ];
    /* Walsh channels of supplementals with the pilot */
  
  #ifdef FEATURE_IS2000
  word sup_qof_id[ SRCH_SUP_CHANNEL_MAX ];
    /* QOF ids of supplementals with the pilot */
  #endif
} srch_tc_sup_type;

typedef struct
{
  srch_cmd_hdr_type hdr;
    /* Common header */

  byte num_sup_pilots;
    /* For each pilot in supplemental active set */

  srch_tc_sup_type sup_pilots[ SRCH_ASET_MAX ];
    /* Details on each pilot in supplemental active set.  See above. */
} srch_tc_update_sup_cmd_type;


#endif
/*--------------------------------------------------------------------------
               PAGING CHANNEL ASSIGNMENT COMMAND

This command tells the searcher to find the strongest pilot_pn in the given
list on the given cdma frequency.
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  word  cdma_freq;
    /* CDMA Channel number of assignment */
  
  byte  band_class;     /* CDMA Band Class */

  word  assign_set_cnt;  
    /* Number of pilots given in assignment */

  struct
  {
    int2  pilot;
  }
  assign_set[ SRCH_NSET_MAX ]; 
    /* Pilot offset indices for pilots on new cdma frequency */
}
srch_pc_assign_cmd_type;

/*--------------------------------------------------------------------------
                     SC/PC FADE PROCESSING COMMAND
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */

  word  freq;
   /* No further information required. */

  byte  band_class;
   /* CDMA Band Class */
}
srch_fade_cmd_type;

/*--------------------------------------------------------------------------
                     TC TX OFF/ON PROCESSING COMMAND
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
   /* Common header */
}
srch_tx_off_on_cmd_type;

/*--------------------------------------------------------------------------
                     PC NEIGHBOR SET PILOT REPORTING COMMAND
This command tells the searcher to report the neighbor pilot strength.                     
--------------------------------------------------------------------------*/
#if defined (FEATURE_IS95B_PILOT_RPT) || \
            defined (FEATURE_SPECIAL_MDR)
typedef struct
{
  srch_cmd_hdr_type hdr;
    /* Common header */
}
srch_pilot_rpt_cmd_type;       
#endif

#ifdef FEATURE_DH
/*--------------------------------------------------------------------------
                        TC FILL AKEY SEED BUFFER
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
}
srch_rand_cmd_type;

#endif

#ifdef FEATURE_IS95B
/*--------------------------------------------------------------------------
                 PERIODIC POWER STRENGTH MEASUREMENT MESSAGE
This command tells the searcher to do Periodic PSMM.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
  word    per_timer;  
    /* Periodic timer value in ms */
  byte    min_pil_pwr_thr;  
    /* Min. Pilot Ec threshold */
  byte    min_pil_ec_io_thr;  
    /* Min. Pilot Ec/Io threshold */

}srch_ppm_cmd_type;

#ifdef FEATURE_IS95B_MAHHO
/*--------------------------------------------------------------------------
                 START CANDIDATE FREQUENCY SEARCH MESSAGE
This command tells the searcher to start the Candidate Frequency search.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte cfs_seq;
    /* The sequence number for the current search period */

  byte search_type;
    /* Search type: 01 single search; 11 periodic search. */
}srch_start_cfs_cmd_type;

/*--------------------------------------------------------------------------
                 STOP CANDIDATE FREQUENCY SEARCH MESSAGE
This command tells the searcher to stop the Candidate Frequency search.                     
--------------------------------------------------------------------------*/

typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte cfs_seq;
    /* The sequence number for the current search period */

}srch_stop_cfs_cmd_type;

/*--------------------------------------------------------------------------
                START CANDIDATE FREQUENCY MEAS MESSAGE
This command tells the searcher to start the Candidate Frequency measurement.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  byte cfs_seq;
    /* The sequence number for the current search period */
}srch_cfs_meas_cmd_type;

/*--------------------------------------------------------------------------
                 CANDIDATE FREQUENCY NEIGHBOR INFORMATION MESSAGE
This command tells the searcher the Candidate Frequency neighbor informtion.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
  
  dword  search_period;
    /* Time between successive searches on the CF, in millisecond unit */
  
  byte  band_class;
    /* Band class of the CF */
  
  word  cdma_freq;
    /* CDMA frequency for the CF */
  
  int1  diff_rx_pwr_thresh;
    /* Minimum difference in received power */
  
  byte  min_total_pilot_ec_io;
    /* Minimum total pilot Ec/Io */
  
  byte  sf_total_ec_thresh;
    /* Ec threshold for the Serving Frequency */
  
  byte  sf_total_ecio_thresh;
    /* Ec/Io threshold for the Serving frequency */ 
  
  byte  cf_t_add;
    /* Pilot detection threshold for th CF */
  
  byte  pilot_inc;
    /* Pilot Inc to be used on th CF after handoff */
  
  byte  cf_srch_win_n;
    /* Default search window size for the CF Search Set */
  
  byte  cf_srch_win_r;
    /* Search window size for the Remaining Set on the CF */
  
  boolean pilot_update; 
    /* Indicates if a CF set update is included, the following fields make
       sense only if this is set to TRUE */
  
  word  cf_nset_cnt;
    /* Number of pilots to be searched */
  
  byte  cf_nghbr_srch_mode;
    /* Search mode for the CF Search Set */

  struct
  {
    int2 pilot;
      /* Pilot PN offset */
    
    byte srch_flag;
      /* Flag to incidate if the corresponding pilot is to be searched */
    
    byte srch_pri;
      /* Search priority */
    
    byte srch_win;
      /* Search window size */

    #ifdef FEATURE_IS2000

    byte srch_offset_nghbr;
      /* Search Window offset */

    #endif /*FEATURE_IS2000 */

  }cf_nset[ SRCH_CFNSET_MAX ];

}srch_cf_nset_cmd_type;

/*--------------------------------------------------------------------------
            START HARD HANDOFF MESSAGE
This command tells the searcher to start a hard handoff.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */
  
  boolean freq_chg;
    /* If a Hard Handoff with a frequency change */

}srch_start_hho_cmd_type;

/*--------------------------------------------------------------------------
                HARD HANDOFF SUCCESS MESSAGE
This command tells the searcher a successful hard handoff.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

}srch_hho_success_cmd_type;

/*--------------------------------------------------------------------------
                HARD HANDOFF FAILURE MESSAGE
This command tells the searcher an unsuccessful hard handoff.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

}srch_hho_fail_cmd_type;

/*--------------------------------------------------------------------------
                REACQ SF ASET MESSAGE
This command tells the searcher to reacq the serving frequency.                     
--------------------------------------------------------------------------*/
typedef struct
{
  srch_cmd_hdr_type  hdr;
    /* Common header */

  word  cdma_freq;
    /* CDMA Channel number */

  byte  band_class;         
    /* CDMA Band Class */

  byte  win_a;
    /* Active set search window size index */

  byte  t_add;
    /* Threshold of energy at which a pilot moves up to the Candidate Set */

  byte  t_drop;
    /* Threshold of energy at which a pilot should be dropped from the
       Active Set or Candidate Set */

  byte  t_comp;
    /* Margin by which a pilot must exceed an Active Set pilot to trigger a
       new Power Measurement Report */

  byte  t_tdrop;
    /* Index of the duration for which a pilot must be below t_drop before
       being dropped or reported */

#ifdef FEATURE_IS95B_SOFT_HO
  byte soft_slope;
    /* Threshold slope value */

  int1 add_intercept;
    /* T_ADD threshold intercept value, in 0.5dB units */

  int1 drop_intercept;
    /* T_DROP threshold intercept value, in 0.5dB units */

  byte  win_n;
    /* Neighbor set search window size index  */

  byte  win_r;
    /* Remaining set search window size index */
#endif
  word  aset_cnt;
    /* Number of Active Set members */

    /* For each member of the new Active Set: */
  struct
  {
    int2  pilot;
      /* Pilot PN index of this Active Set member */

    word  walsh;
      /* Walsh channel to be used on the traffic channel with the pilot */
    
    #ifdef FEATURE_IS2000
    byte qof_id;
      /* QOF mask ID (0-3), where 0 gives true walsh functions */
    #endif

    byte  pwr_comb_ind;
      /* Flag: nonzero if the power control subchannel on this pilot
         contains the same symbols as that on the previous pilot. */
  }
  aset[ SRCH_ASET_MAX ];
    /* Pilot offset indices and walsh channels for active set members */

}srch_hho_reacq_cmd_type;  
#endif /* FEATURE_IS95B_MAHHO */
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000_QPCH

/* -----------------------------------------------------
** Quick Paging Channel enumerations values
** ----------------------------------------------------- */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

typedef enum 
{
  OOK_ZERO,           /* OOK bit decoded as a ZERO */
  OOK_ERASURE,        /* OOK bit was not clearly decoded */
  OOK_ONE,            /* OOK bit decoded as a ONE */
  OOK_MISSED,         /* OOK bit was missed (not on purpose) */
  OOK_SKIPPED,        /* OOK bit was skipped (on purpose) */
  OOK_NOT_DECODED     /* OOK bit has not been decoded */
} srch_ook_decision_type;


/* QPCH Soft decision and decision threshold value type (11 bits, signed) */
typedef int2 srch_ook_value_type;

/* QPCH On/Off Key (OOK) data structure */
typedef struct 
{
  srch_ook_value_type    soft_decision_i;
  srch_ook_value_type    soft_decision_q;
  srch_ook_value_type    decision_threshold_i;
  srch_ook_value_type    decision_threshold_q;
} srch_ook_data_type;

/* QPCH On/Off Key (OOK) data/decision structure per bit */
typedef struct 
{
  srch_ook_data_type     raw_data;
  srch_ook_decision_type decision;
  } srch_ook_bit_type;

/* QPCH On/Off Key (OOK) data/decision structure per QPCH slot */
typedef struct 
{
  srch_ook_bit_type      bit[2];
  srch_ook_decision_type decision;
} srch_ook_bits_type;

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

#endif /* FEATURE_IS2000_QPCH */


#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */


/*--------------------------------------------------------------------------
                      UNION OF ALL SEARCH COMMANDS

--------------------------------------------------------------------------*/

typedef union
{
  srch_cmd_hdr_type       hdr;
  srch_start_cmd_type     start;
  srch_cdma_cmd_type      cdma;
  srch_acq_cmd_type       acq;
  srch_sc_cmd_type        sc;
  srch_slew_cmd_type      slew;
  srch_pc_cmd_type        pc;
  srch_sleep_cmd_type     sleep;
  srch_tc_cmd_type        tc;
  srch_unslew_cmd_type    unslew;
  srch_parm_cmd_type      parm;
  srch_meas_cmd_type      meas;
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
  srch_fpc_meas_cmd_type  fpc_meas;
  srch_pc_aset_cmd_type   pc_aset;
  srch_nset_cmd_type      nset;
  srch_tc_aset_cmd_type   tc_aset;
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  srch_tc_update_sup_cmd_type  tc_sup;
#endif
#ifdef FEATURE_IS2000_SCH
  srch_tc_add_sch_cmd_type  tc_sch;
#endif /* FEATURE_IS2000_SCH */
#ifdef FEATURE_IS2000_R_SCH
  srch_scrm_meas_cmd_type   scrm_meas;
#endif /* FEATURE_IS2000_R_SCH */
  srch_pc_assign_cmd_type pc_assign;
  srch_fade_cmd_type      fade;
  srch_tx_off_on_cmd_type off_on;
#ifdef FEATURE_DH
  srch_rand_cmd_type      rand;
#endif

#if defined (FEATURE_IS95B_PILOT_RPT) || \
           defined (FEATURE_SPECIAL_MDR)
  srch_pilot_rpt_cmd_type pilot_rpt;
#endif

#ifdef FEATURE_IS95B_PPSMM
  srch_ppm_cmd_type     ppsmm;
#endif
#ifdef FEATURE_IS95B_MAHHO
  srch_cf_nset_cmd_type   cfnset;
  srch_start_cfs_cmd_type start_cfs;
  srch_stop_cfs_cmd_type  stop_cfs;
  srch_cfs_meas_cmd_type  cfs_meas;
  srch_start_hho_cmd_type hho_st;
  srch_hho_success_cmd_type hho_ok;
  srch_hho_fail_cmd_type  hho_fail;
  #ifndef FEATURE_IS2000
  srch_tc_aset_cmd_type   hho_reacq;
  #else
  srch_tc_ho_cmd_type ho;
  srch_tc_reacq_cmd_type   hho_reacq;
  srch_mc_trans_cmd_type   mc_trans;
  #endif
#endif /* FEATURE_IS95B_MAHHO */

#if ( ( defined FEATURE_IS2000_QPCH ) && ( defined FEATURE_PLT ) )
#error code not present
#endif  /* FEATURE_IS2000_QPCH && FEATURE_PLT */

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

}
srch_cmd_type;

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION SRCH_TASK

DESCRIPTION    This function is the main routine for the Search Task.

      Several main purpose(s) of the Search task are to:

        1.  Initially search for, and find, a CDMA pilot in-channel,

        2.  Measure neighboring cell's pilot energies and call for handoffs,

        3.  Establish and maintain CDMA system time and frequency track,

        4.  Keep multipath RAKE receiver fingers/combiners tracking good pilots,

        5.  Put the MSM chip to sleep and re-establish acquisition after wakeup.

      The Search task's state machine begins in START, and under control by
      Main Control, proceeds generally through the following transitions:
                                    _____________________________
                                   |           /|\               |
                                   |            |                |
                                  \|/           |                | 
           START -> CDMA -> ACQ -> SC -> SL -> PC -> TC -> UN -> +
            /|\                                /|\
             |                                  |
            \|/                                \|/
            DZ                                 ZZ

      Interfacing between Search and other tasks is accomplished by means of
      queues, called interfaces, and REX signals. Types of information passed
      between Search and other tasks include state change commands, reports, 
      data tables to store to and read from NV, search, finger, and AGC/power
      control data packets, and mode information.
      
      Also, Search interacts heavily with the CDMA demodulation hardware.
      This hardware includes a searcher, demodulation fingers, symbol,
      frequency, and power combiners, and AGC reference control circuitry.
      The Search task interfaces with the demodulation hardware by means
      of device drivers and interrupt callback functions.

      The following table summarizes the main interfaces which exist between
      Search and the rest of DMSS:

       Interface       Type of          
         With         Interface             Why Information Exchanged
        ------        ---------             -------------------------
        MC Task       REX queues            MC sending commands to Search
                                            Search sending reports to MC

        CDMA Demod    Device drivers        Search controlling demod hardware
        Hardware                            (searcher, fingers, combiner, AGC)

        TRAMP         Interrupt callbacks   Demod hardware events (PN roll, 
        Services                            searcher dump, wakeup, keypress)

        DIAG Task     REX queues            Search sending searcher, finger,
                                            AGC, power control data to the DM

        NV Task       Called interfaces     Search storing/retrieving PN mask,
                                            sleep status, freq-temp estimation
                                            table to/from nonvolatile memory

        DB Services   Called interfaces     Search storing/retrieving sleep
                                            status, acquisition mode, RF mode
                                            (PCN -vs- Cellular)

        Sleep Task    Called interfaces     Search programming sleep timer, 
                      REX signals           signaling Sleep task for powerdown

        DOG Task      Called interface      Search kicking the watchdog while
                                            waiting on REX signals.

      Upon task creation, the Search task main routine performs initializations
      such as setting up queues, defining timers, and reading parameters from
      NV, then enters an infinite loop which runs the Search main state machine.

DEPENDENCIES   This function assumes that the demodulator as already been
      initialized into a benign state.

RETURN VALUE   None.

SIDE EFFECTS   Everything in the world.

===========================================================================*/

extern void  srch_task
(
  dword  param
    /* Task startup parameter. Ignored by Search task. */
);


/*===========================================================================

FUNCTION SRCH_CMD

DESCRIPTION    This function places a specified command onto the Search
      Task's command queue for processing by the Search Task. Notice that
      this function is invoked by tasks other than the Search Task.

DEPENDENCIES   This function assumes that the 'srch_cmd_q' has been properly
      initialized.
  
RETURN VALUE   None.

SIDE EFFECTS   The Search Task dequeues and processes the specified command.
===========================================================================*/

extern void  srch_cmd
(
  srch_cmd_type  *cmd_ptr
    /* Pointer to searcher command. Upon queueing, the 'status'
       field of the command block is set to 'SRCH_BUSY_S'. */
);


/*===========================================================================

FUNCTION SRCH_SET_TA_PARM

DESCRIPTION    This function is used to set what the searcher task
      reports to the temporal analyzer.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   srch_ta_parms

===========================================================================*/

extern void srch_set_ta_parm
(
  word  set_mask,
    /* Specifies which type of pilot sets should be reported.
       Bit 15 => report active set pilot scans.
       Bit 14 => report candidate set pilot scans.
       Bit 13 => report neighbor set pilot scans.
       Bit 12 => report remaining set pilot scans. */

  word  ta_cnt
    /* Specifies desired size of each scan in 1/2 chip units. */
);


/*===========================================================================

FUNCTION SRCH_LOG_AGC_PCTL

DESCRIPTION    This function is invoked to log AGC and power control values 
      for DIAG.

DEPENDENCIES   srch_agc_pctl_vars

RETURN VALUE   None.

SIDE EFFECTS   Messages can be sent to DIAG task.

===========================================================================*/

extern void  srch_log_agc_pctl ( void );

/*===========================================================================

FUNCTION SRCH_GET_PILOT_SET

DESCRIPTION    This function fills up a diagnostic pilot set status
      packet with current set populations and memberships.

DEPENDENCIES   srch_?set, srch_?set_cnt, srch_parms.pilot_inc
    
RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/
extern void srch_get_pilot_set
(
  srch_pilot_set_struct_type *pkt_ptr
    /* The DIAG pilot packet to fill in */
);


/*===========================================================================

FUNCTION SRCH_GET_CDMA_RSSI

DESCRIPTION    This function returns a byte representing the current
      CDMA RSSI value in P(AGC) + Ec/Io form. This byte can then be
      arbitrarily mapped to UI display space using an equation, table,
      or case statement.  RSSI is stored internal to search as a word,
      for better filtering results.
      
      The RSSI if filtered using an (N-1)/N IIR filter for a smoother signal
      strength display. This CDMA RSSI is written into DB (database)
      for consistency with analog RSSI. This database value is thereby
      updated at a rate determined by the caller.

      The function returns the raw, unfiltered CDMA RSSI. Targets which
      need unfiltered values must call this function explicitly rather
      than using the filtered DB value.

DEPENDENCIES    srch_aset[], srch_aset_cnt.
    
RETURN VALUE    A 6-bit RSSI index, 0..63,  representing RSSI from
                -75 dBm to -106.5 dBm, in 0.5 dBm steps.

SIDE EFFECTS    Filtered database CDMA_RSSI is updated.

===========================================================================*/

extern byte  srch_get_cdma_rssi ( void );

/*===========================================================================

FUNCTION SRCH_GET_BEST_RSSI

DESCRIPTION    This function returns a byte representing the best-potential
      CDMA RSSI value in P(AGC) + Ec/Io form.
      
DEPENDENCIES    srch_?set[], srch_?set_cnt.
    
RETURN VALUE    A 6-bit RSSI index, 0..63, representing RSSI from
                -75 dBm to -106.5 dBm, in 0.5 dBm steps.

SIDE EFFECTS    None.

===========================================================================*/

extern byte  srch_get_best_rssi ( void );


/*===========================================================================

FUNCTION SRCH_START_PC_STATE

DESCRIPTION    This function returns TRUE the first time it is called
      after Search enters PC state.
      
DEPENDENCIES   srch_state.

RETURN VALUE   boolean indicating if this is first call (TRUE).

SIDE EFFECTS   none.

===========================================================================*/

extern boolean srch_start_pc_state(boolean reset_only);


/*===========================================================================

FUNCTION SRCH_FLAG_ERASURE

DESCRIPTION    This function is called to flag whether or not a frame is
      an erasure.

DEPENDENCIES   srch_rxc_frame_cnt
                     srch_rxc_erase_cnt

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

void srch_flag_erasure 
(
  boolean is_erasure
    /* TRUE if this frame is an erasure, FALSE otherwise */
);


/*===========================================================================

FUNCTION SRCH_GET_DMOD_ID

DESCRIPTION    This function returns the version number ID of the
      demodulator ASIC or demod section of the MSM.

DEPENDENCIES   None.

RETURN VALUE   The hardware revision number.
      Bits [11: 8] - Process ID
      Bits [ 7: 4] - MSM ID
      Bits [ 3: 0] - Version ID

SIDE EFFECTS   None.

===========================================================================*/

extern word  srch_get_dmod_id ( void );

/*===========================================================================

FUNCTION SRCH_DMOD_RESET

DESCRIPTION    This function performs all needed initializations to the
      demodulator for normal operations. This function should be called
      before any other demodulator reads/writes are done.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void srch_dmod_reset(void);

/*===========================================================================

FUNCTION SRCH_GET_REF_TIME

DESCRIPTION    This function reads the symbol combiner's time reference
      register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   The 16-bit time reference register value.  Note that this
      value is not exactly binary.  It is binary encoded, but its maximum
      value is 0xBFFF.  Each LSB is two PN chips.  The top two bits are
      the 26.666... ms phase within an 80ms superframe.

SIDE EFFECTS   The time reference latch-on-micro-write register is
      updated.

===========================================================================*/

extern word  srch_get_ref_time ( void );

/*===========================================================================

FUNCTION SRCH_GET_VSC_TIME

DESCRIPTION    This function reads the "virtual symbol combiner's"
      time reference register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   The 16-bit time reference register value.  Note that this
      value is not exactly binary.  It is binary encoded.  Each LSB is
      two PN chips.  The value returned may correspond to a position
      larger than 80ms.

SIDE EFFECTS   The time reference latch-on-micro-write register is
      updated.

===========================================================================*/

extern word  srch_get_vsc_time ( void );



/*===========================================================================

FUNCTION SRCH_GET_ULPN_STATE

DESCRIPTION    This function reads the User Long PN Code state from
      the demod ASIC.

DEPENDENCIES   Calling this function asynchronously with the 26.666... ms
      boundary is dangerous.  If the 26.666... ms boundary passes while
      this function is reading the registers, a bogus value will be
      returned. Also, srch_dmod_reset should have been previously called,

RETURN VALUE   No formal return value,  but the User Long PN Code state
      is returned in a ulpn_type passed in by reference.  This state is
      the one that was valid at the last 26.666... ms boundary.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_get_ulpn_state
(
  ulpn_type   *ulpn
    /* Pointer to where to put the ULPN value read from the demod */
);

/*===========================================================================

FUNCTION SRCH_SET_LC_MASK

DESCRIPTION    This function sets the demod long code mask register to a value.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Demod long code mask register is written.

===========================================================================*/

extern void  srch_set_lc_mask
(
  qword pn_mask
    /* PN long code mask to load into the mask register */
);


/*===========================================================================

FUNCTION SRCH_PCTL_RATE_SET

DESCRIPTION    This function enables/disables one-symbol (14.4K)
      puncturing mode.

DEPENDENCIES   srch_dmod_reset should have been previously called,

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void  srch_pctl_rate_set
(
  enc_rate_set_type  rate
    /* The encoder rate being used */
);

#ifndef FEATURE_EXT_LINEARIZER

/*===========================================================================

FUNCTION SRCH_GET_TX_ADJ

DESCRIPTION    This function returns the current value of the tx gain
      adjust register.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern byte  srch_get_tx_adj ( void );

/*===========================================================================

FUNCTION SRCH_SET_TX_ADJ

DESCRIPTION    This function allows external tasks (namely the Transmit
  Task) to have access to the tx gain adjust register of the demod chip. 

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Changes the contents of the tx gain adjust register of the
               demod chip.

===========================================================================*/

extern void  srch_set_tx_adj
(
  byte  tx_adj
    /* The 8-bit value to be written to the tx gain adjust register. 
       Note that this is a signed value. For maximum power, use the
       value -128 (0x80); for minimum power, use +127 (0x7F). */
);
#endif

#ifdef T_MSM5000

/*===========================================================================

FUNCTION SRCH_SET_TX_RATE_ADJ

DESCRIPTION    This function allows external tasks (namely the Transmit
  Task) to have access to the tx gain adjust register for the data rate.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Changes the contents of the tx rate adjust register of the
               demod chip.

===========================================================================*/

extern void  srch_set_tx_rate_adj
(
  int16  tx_adj
    /* The 9-bit value to be written to the tx rate adjust register. */
);
#endif /* T_MSM5000 */

/*===========================================================================

FUNCTION SRCH_GET_RX_AGC

DESCRIPTION    This function returns the current value of the rx agc register.
      For targets using MSM2 or higher, this is simply a passthru for RF
      CAGC function calls.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern byte  srch_get_rx_agc ( void );

/*===========================================================================

FUNCTION SRCH_ACCESS_INFORM

DESCRIPTION    This function is invoked to signal the searcher that an
               access is in progress.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   srchpc_access_pending is set or cleared.

===========================================================================*/

extern void srch_access_inform( boolean access_pending );



/*===========================================================================

FUNCTION SRCH_FREEZE_IQ

DESCRIPTION    This function places the I_OFFSET and Q_OFFSET PDMs in
      an appropriate inactive state getting ready for sleep or restores
      normal operation.

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None.

SIDE EFFECTS   Various demod chip registers.

===========================================================================*/

extern void srch_freeze_iq
(
  boolean freeze
    /* Indication (TRUE/FALSE) of whether we want to freeze
       the I/Q offset PDMs for sleep mode. */
);


/*===========================================================================

FUNCTION SRCH_RUDE_WAKEUP

DESCRIPTION    This function is invoked to signal the searcher task to 
               wake up.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Search SLEEP signal may be cleared.
               srch_rudely_awakened flag is set.

===========================================================================*/

extern void srch_rude_wakeup( boolean orig );

/*===========================================================================

FUNCTION SRCH_KEYPRESS_ISR

DESCRIPTION    This function is invoked to service the keypress interrupt
      generated by the MSM. It just signals the searcher task.

DEPENDENCIES   This function is to be called through the interrupt trampoline.

RETURN VALUE   None.

SIDE EFFECTS   Keypress ISR is de-registered.
               Search SLEEP signal may be cleared.
               srch_rudely_awakened flag is set.

===========================================================================*/

extern void srch_keypress_isr( void );

/*===========================================================================

FUNCTION SRCH_WAKEUP_ISR

DESCRIPTION    This function is invoked to service the wakeup interrupt
      generated by the MSM. It just signals the searcher task.

DEPENDENCIES   This function is to be called through the interrupt trampoline.

RETURN VALUE   None.

SIDE EFFECTS   Keysense and wakeup ISRs are de-registered.
               Search SLEEP signal may be cleared.
               Search WAKEUP signal is set.

===========================================================================*/

extern void srch_wakeup_isr( void );

// by chos 00.12.28
// watchdog autokick 관련 하여 Qc source에 뭔가 문제가 있는 것 같다
// 최초 sleep 진입 할때 원인을 알 수 없는 reset(watchdog)과
// catnap구간에서 autokick가 아닌데 dogkick를 안하는 구조 이므로
// 이구간에서는 GP_TIMER를 이용한 watch dog autokick를 하게함
#ifdef	FEATURE_GP_TIMER_AUTO_KICK
void EnableGpTimerAutoKicking ( void );
void DisableGpTimerAutoKicking ( void );
#endif	//#ifdef	FEATURE_GP_TIMER_AUTO_KICK

/*===========================================================================

FUNCTION SRCH_SELECT_8X

DESCRIPTION    This function enables and disables searcher 8X mode of 
               operation for MSM 2.3 

DEPENDENCIES   srch_dmod_reset should have been previously called.

RETURN VALUE   None. 

SIDE EFFECTS   Searcher dumps will occur more frequently, no DMA data will
               be available for TA.  Forced use of Searcher peak sort.
===========================================================================*/

extern void srch_select_8x
(
  boolean enable
    /* TRUE if we want to enable searcher 8x mode, FALSE otherwise */
);

/*===========================================================================

FUNCTION SRCH_GET_PILOT_ECIO

DESCRIPTION    This function returns a byte representing the current
               active pilot Ec/Io value. 

DEPENDENCIES    srch_aset[], srch_aset_cnt.

RETURN VALUE    The 6-bit Ec/Io value.  If the energy is out of range,
                the return value is clipped to 0 or 31.5. This is in 
                db unit. 
      
SIDE EFFECTS    None.

===========================================================================*/
extern byte srch_get_pilot_ecio  (void);

/*===========================================================================

FUNCTION SRCH_FINGER_LOCK_HANDLER

DESCRIPTION
  This function should be called periodically to monitor the power
  combiner lock status of all fingers.  If all fingers go out of
  lock, this function will disable power control.
  
DEPENDENCIES
  Only applies to MSM2300. The power combiner lock state is peeked on
  using proprietary test registers in the MSM2300.

RETURN VALUE
  None

SIDE EFFECTS
  Power control may be disabled/enabled.

===========================================================================*/
extern void srch_finger_lock_handler ( int4 ms_interval );

/*===========================================================================

FUNCTION SRCH_GET_FING_RSSI

DESCRIPTION    This function gets the RSSI of the best finger.
               This is intended to be called as a diagnostic tool.
                    
DEPENDENCIES   None.

RETURN VALUE   Word RSSI of best fing assigned and in lock.
               "-1" implies no fingers are in lock.
               "-2" implies no fingers were assigned.

SIDE EFFECTS   None.
              
===========================================================================*/

extern int2 srch_get_fing_rssi( void );

#ifdef FEATURE_IS2000

/*===========================================================================

FUNCTION SRCH_UPDATE_RC

DESCRIPTION    This function updates the srch's local copy of the FCH RC
      value which is necessary for logging.  This function is called from
      the MUX layer where the RC value is updated.

DEPENDENCIES   None.
    
RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_update_rc
(
  deint_rc_type  fch_rc_val,
    /* Fundamental channel RC value */

  deint_rc_type  sch0_rc_val,
    /* Supplemental channel 0 RC value */

  deint_rc_type  sch1_rc_val,
    /* Supplemental channel 1 RC value */

  deint_rc_type  dcch_rc_val
   /* Dedicated Control channel RC value */
);

/*===========================================================================

FUNCTION SRCH_SET_FPC_MODE

DESCRIPTION    This function updates SRCH's local copy of the FPC
      state which is necessary for setting the finger fast/slow time-
      tracking.

DEPENDENCIES   None.
    
RETURN VALUE   None.

SIDE EFFECTS   None.

===========================================================================*/

extern void srch_set_fpc_mode
(
  boolean  fpc_enabled
    /* Forward power control state */
);
#endif /* FEATURE_IS2000 */

/*===========================================================================

FUNCTION SRCH_FAST_SLEEP_INFORM

DESCRIPTION
  This function is used to signal the sleep controller that 
  a page match did or did not pass, allowing or disallowing
  a fast sleep action.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  REX sigs in SRCH TCB are set.
  SRCH task priority is raised to MAX for instant response.

===========================================================================*/

extern void srch_fast_sleep_inform( boolean fast_sleep_ok );

/*===========================================================================

FUNCTION SRCH_SET_DMOD_PILOT_GAIN

DESCRIPTION    This function sets PILOT_FILTER_GAIN according to the Band 
               Class.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   Changes the content of the finger pilot filter gain register 
               of the demod chip.

===========================================================================*/

extern void srch_set_dmod_pilot_gain( byte band_class );

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
/*===========================================================================

FUNCTION SRCH_ASET_WALSH

DESCRIPTION
  This function is used to provide walsh code infromation of the Active
  Set.
  
DEPENDENCIES
  srch_aset[], srch_aset_cnt.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srch_aset_walsh( srch_walsh_struct_type *ptr );
#endif

#ifdef FEATURE_IS95B_MAHHO
/*===========================================================================

FUNCTION SRCH_CFS_OFF_TIME_FWD

DESCRIPTION    This function provides the max and total off time of forward 
               channel of CFS. It's only used for MC to estimated the off time.

DEPENDENCIES   None.

RETURN VALUE   None. 
      
SIDE EFFECTS   srch_max_off_time_fwd
               and srch_max_off_time_fwd are updated.

===========================================================================*/
extern void srch_cfs_off_time_fwd
(
  word max_win,
    /* The maximum window size of the neighbors in chip unit*/
  
  word num_set,
    /* Number of the total CF neighbor set */

  word *max_off,
    /* Pointer to max off time address*/

  word *total_off
    /* Pointer to total off time address */
);
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */ 

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

/*===========================================================================

FUNCTION SRCH_GET_CDMA_RAW_ECIO

DESCRIPTION    This function returns a byte representing the active set
               pilot Ec/IO.
      
DEPENDENCIES    srch_aset[], srch_aset_cnt.
    
RETURN VALUE    negtive Ec/IO value in 0.5 dBm steps.

SIDE EFFECTS    None.

===========================================================================*/
extern byte  srch_get_cdma_raw_ecio ( void );

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif /* FEATURE_JCDMA_MONITOR */

#endif /* SRCH_H */

