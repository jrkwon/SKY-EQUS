#ifndef MC_H
#define MC_H
/*===========================================================================

          M A I N   C O N T R O L   T A S K    H E A D E R

DESCRIPTION
  This file contains global declarations and external references
  for Main Control task.  This file contains the data structures
  and interface definitions to allow any task to interface to the
  Main Control task (MC) and its subtasks (MCCDMA and ACPMC).  All
  messages sent to MC and its subtasks are queued to the MC command
  queue via services provided by the MC_CMD unit.

Copyright (c) 1990,1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR FILE

   This section contains comments describing changes made to this file.
   Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mc.h_v   1.12   08 Mar 2001 13:19:34   varavamu  $

when       who     what, where, why
--------   ---     -------------------------------------------------------
03/08/01   va      Changed MCC_TXC_REV_DTX_SIG from 0x40000000 to 0x80000000, as it
                   was contending with FS_OP_COMPLETE_SIG which MC waits on during
                   boot up.
03/06/01   fc      Added enumeration mc_event_op_mode_type. 
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
01/24/01   ych     Merged T-53 Changes
12/15/00   lh      Add support for MS originated Long Code Transistion Request
                   Order
12/14/00   cah     Remove sms uasmsi.h inclusion.
12/13/00   cah     Replaced obsolete smsi.h inclusion with uasmsi.h.
11/30/00   yll     Removed FEATURE_GPSONE_TAWD_TEST_ONLY.
11/28/00   ck      Added constants and masks to support STORE_ESN_ME command.
11/01/00   lcc     Added member DRS in mc_origination_type.
10/02/00   yll     Added support for Position Determination DBM.
08/23/00   jq      Added MCC_TXC_REV_DTX_SIG.
08/02/00   yll     Added GPSONE TAWD (tuning away w/o drop) test support.
07/17/00   jq      Modified mc_end_type to include end_reason.
07/14/00   ks      Defined new signal MCC_SLOTTED_TIMER_SIG.
06/19/00   fc      Added MC_SCRM_F to support SCRM.
06/12/00   lcc     Removed MC_TRANSACTION_DONE_F which was added by mistake.
04/24/00   lcc     Added MC_TRANSACTION_DONE_F.
04/11/00   lcc     Added definition for P_REV 6.
04/07/00   ry      Merged the following from PP (MSM3100_CP.03.00.17):
           jq      Added Broadcast SMS feature support.
03/07/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ych     Added support for Tri-mode Silent Redial
           ram     Added two Macros BAND_IS_CELLULAR and BAND_IS_PCS to
                   check the current band class during Tri-mode operation.
11/15/99   va      IS2000 Changes.
                   New macro added for p_rev 6 check
10/28/99   lcc     Moved PLT related code to plt_mc.c.
09/16/99   lcc     Merged in signals for PLT command interface from PLT archive.
08/21/99   ks      Changed Signal MCC_ACCPR_HO_OK_SIG to MCC_TXC_APHO_OK_SIG.
08/13/99   lh      CM and SMS phase 2 implementations.
08/09/99   ry      Changed FEATURE_IS95B_HARD_HO to FEATURE_IS95B_MAHHO
07/30/99   fc      Put in changes based on code review comments on Analog
                   Registration Randomization.
07/28/99   lh      CM and two-way SMS support added.
07/23/99   fc      Added a new signal ACPMC_REDIRECT_TIMER_SIG.
07/19/99   abh     Changes for Access Probe Handoff.
07/09/99   ry      Updated MCC_SCAM_TIMER_SIG, MCC_MDR_DURATION_TIMER_SIG,
                   and MCC_TXC_PROBE_SIG to new 32-bit constants
           ry      Added MCC_TXC_FRAME_SIG and MCC_HANDOFF_TIMER_SIG
05/06/99   kmp     Merged IS-95B changes below.
           fc      Put in changes based on input from code review.
           fc      Redefined the signal value for MCC_TXC_PROBE_SIG.
           fc      Renamed MCC_ACC_PROBE_SIG to MCC_TXC_PROBE_SIG and
                   defined it under FEATURE_IS95B_PILOT_RPT.
           ks      Removed the AEHO and AHO Feature ifdefs.
           ks      Added AEHO and AHO related FEATURE ifdef checking.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           fc      Added new signal MCC_ACC_PROBE_SIG.
           lh      P_REV_IS_5_OR_GREATER macro added.
           ks      Added defines for representing various P_REVs in mobile.
           lh      Added support for run-time P_REV and Band Class checking.
02/09/99   br      FEATURE_CDMA_800 changed into FEATURE_SSPR_800, T_AUTH
                   into FEATURE_AUTH, T_SMS into FEATURE_SMS and T_ACP
                   FEATURE_ACP.
03/09/99   ram     Defined SCAM/GHDM duration timer signal for IS95B MDR
02/19/99   ram     Defined SCAM start timer signal for IS95B MDR
01/13/99   lcc     Merged from MDR branch:
                   - Added run-time P_REV and Band Class checking functions from
                     IS95B.
11/24/98   br      Added changes for storing new NAM.
08/25/98   ck      Added the signal MCC_OTASP_AKEY_COMMIT_SIG
08/25/98   ck      Added DH support for builds with FEATURE_DH defined.
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements
06/25/98   kis     Added ASCII mode related fields in ms_origination_type.
06/17/98   wjn     Added Feature E911 Support -- Modification of
                   "mc_pref_sys_changed_type".
03/20/98   abh     Moved mcsys function prototype for PRL Force Mode
                   in mcsyspr.h
01/28/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
01/28/98   wjn     Added a prototype UI-MC Interface function
                   mc_get_prl_version();
05/20/96   ych     Added new command MC_LPM_F.
12/13/96   jca     Modified mc_origination_type.
09/13/96   dna     Added two fields to origination cmd for FEATURE_OTASP.
06/25/96   dna     Added MCC_OTASP_COMMIT_SIG for FEATURE_OTASP.
05/24/96   jjw     Gemini & ISS2 Data Services release.
04/15/96   jca     Changed ms data type in mc_stop_cont_dtmf_type to word.
04/15/96   day     Added OTASP MC command.  Using FEATURE_OTASP ifdef.
04/09/96   day     Added TMSI addressing modifications.  Using T_TMSI ifdef.
12/08/95   bns     Removed MC_REG_F for ISS2 forced registration
12/06/95   bns     Added MC_REG_F for ISS2 forced registration
11/27/95   gb      Fixed length field of AC_MSG tmob command.
08/24/95   day     Removed SMS data declaration for mobile-originated
                   messages to recover 4.7k RAM bytes.
07/13/95   rdb     Added MCC_SRV_TIMER_SIG for use with service negotiation.
07/07/95   dna     Added new signals for ACP for Authentication
07/06/95   jca     Now have one consolidated ORIGINATION command with a
                   specified service option number.
06/25/95   dy      Modified MC_DATA_BURST_F to MC_SMS_F and updated
                   corresponding data structure for new SMS interface.
06/20/95   jca     Added MC_LOOPBACK_F command.  Added new parameters to
                   MC_DATA_BURST_F command.
06/15/95   jca     MC, MCC and ACPMC now using same signal definitions for
                   NV commands and kicking the watchdog to avoid conflicts.
06/09/95   jca     Now using one rex_sig (MCC_Q_SIG) for all MCC queues.
06/07/95   dna     Added Authentication Definitions
03/24/95   jca     Added service option field to MC_DATA_F command.
11/01/94   gb      Sector Lock test functionality added.
05/12/94   gb      Added new test commands.
04/20/94   jca     Added MC_DATA_END_F command.
01/06/94   jah     Updated mc_task_start() prototype for watchdog iface change
12/22/93   jca     Modified for IS-95 Phase 1 upgrade.
01/05/93   jai     Added support for Mobile originated markov calls and
                   data services.
12/09/92    ip     Added duration of continuous DTMF to STOP_CONT_DTMF
                   message and added MCC_DTMF_TIMER_SIG signal.
08/22/92    ip     Fixes after code review and initial integration.
08/10/92    ip     Initial release for code review.
07/13/92    ip     Included CDMA signals definitions.
07/13/92    ip     Updated to include new cmd_hdr type from cmd.h.
07/06/92    ip     Initial release to PVCS.
05/05/92    ip     Initial porting of file from Brassboard to DMSS.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "customer.h"
#include "rex.h"
#include "queue.h"
#include "cmd.h"
#include "cai.h"
#include "dog.h"
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Signals are defined below.  Signals are shared between MC, MCC and      */
/* ACPMC, as shown below.  The only exception is the main command queue    */
/* signal, since this has to be known by an outside calling task.          */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*                              MC signals                                 */
/*                              ----------                                 */

#define  MC_CMD_Q_SIG         0x0001 /* Command queue signal               */
#define  MC_NV_CMD_SIG        0x0002 /* NV command response signal         */
#define  MC_RPT_TIMER_SIG     0x0004 /* Watchdog report signal             */
#define  MC_ACK_SIG           0x0008 /* State change ack signal            */
#define  MC_CMD_SIG           0x0010 /* Command response signal            */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*                            -------------                                */
/*                            ACPMC signals                                */
/*                            -------------                                */

#define ACPMC_CMD_Q_SIG        MC_CMD_Q_SIG
  /* Indicates new item on ACPMC CMD Q  */

#define ACPMC_NV_DONE_SIG      MC_NV_CMD_SIG
  /* NV command response signal         */

#define ACPMC_DOGRPT_TIMER_SIG MC_RPT_TIMER_SIG
 /* Watchdog report interval timer      */

#define ACPMC_ACPRX_MSG_Q_SIG  0x0008
  /* Indicates new item on ACPRX MSG Q  */

#define ACPMC_ACPRX_RPT_Q_SIG  0x0010
  /* Indicates new item on ACPRX RPT Q  */

#define ACPMC_ACPTX_RPT_Q_SIG  0x0020
  /* Indicates new item on ACPTX RPT Q  */

#define ACPMC_CMD_TIMER_SIG    0x0040
  /* Time to report back has expired    */
#define ACPMC_STATE_TIMER_SIG  0x0080
  /* Access timer(SA)/Release timer(VC) */

#define ACPMC_FLASH_TIMER_SIG  0x0100
  /* Active when valid flash terminates */

#define ACPMC_AUTH_Q_SIG       0x0200
  /* Indicates new item on MC_AUTH_Q */

#define ACPMC_CMD_SIG          0x0400
  /* Synchronizes Auth Commands */

#ifdef FEATURE_IS95B
#define ACPMC_REGDELAY_TIMER_SIG 0x0800
  /* Active when registration delay timer expires */
#endif /* FEATURE_IS95B */

/* Currently not used */

#define ACPMC_RSVD_1000_SIG    0x1000

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*                             -----------                                 */
/*                             MCC signals                                 */
/*                             -----------                                 */

#define MCC_Q_SIG              MC_CMD_Q_SIG
  /* This signal is set when an item is placed on any MCC queue */

#define MCC_CMD_SIG            MC_NV_CMD_SIG
  /* This signal is set when either Searcher, Receive, Transmit, RXTX, NV
     or SND tasks complete processing of a command from the Main Control
     CDMA subtask */

#define MCC_RPT_TIMER_SIG      MC_RPT_TIMER_SIG
  /* This signal is set when the mc_rpt_timer expires     */

#define MCC_STATE_TIMER_SIG    0x0008
  /* This signal is set when the mc_state_timer expires   */

#define MCC_ACTION_TIMER_SIG   0x0010
  /* This signal is set when the  mc_action_timer expires */

#define MCC_CMD_TIMER_SIG      0x0020
  /* This signal is set when the mc_cmd_timer expires     */

#define MCC_DTMF_TIMER_SIG     0x0040
  /* This signal is set when the mcc_dtmf_timer expires   */

#define MCC_SO_CTL_TIMER_SIG   0x0080
  /* This signal is set when the mcc_so_ctl_timer expires */

#define MCC_TXC_DONE_SIG       0x0100
  /* This signal is set by TXC when an access attempt or transition to
     Traffic Channel state is completed */

#define MCC_SRV_TIMER_SIG      0x0200
  /* This signal is set when the srv_timer expires */

#ifdef FEATURE_TMSI
#define MCC_FULL_TMSI_TIMER_SIG  0x0400
  /* This signal is set for expiration of the full TMSI information */
#define MCC_TMSI_EXP_TIMER_SIG   0x0800
  /* This signal is set for expiration of TMSI code */
#else
/* Currently not used */

#define MCC_RSVD_0400_SIG      0x0400
#define MCC_RSVD_0800_SIG      0x0800
#endif /* FEATURE_TMSI */

#ifdef FEATURE_OTASP
#define MCC_OTASP_COMMIT_SIG   0x1000
#else
/* Currently not used */
#define MCC_RSVD_1000_SIG      0x1000
#endif /* FEATURE_OTASP */

/* re-using the MCC_SRV_TIMER_SIG as it is a traffic channel signal */
#ifdef FEATURE_IS2000
#define MCC_SLOTTED_TIMER_SIG  0x0200
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Signals 0x2000 thru 0x8000 are system wide and defined in task.h        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


#ifdef FEATURE_IS95B_ACCESS_PROBE_HO
/* MCC_TXC_APHO_OK_SIG reuses OTASP Commit Signal. This can be done since */
/* both these signals are temporally exclusive                            */

#define MCC_TXC_APHO_OK_SIG   0x1000 /* This signal is set by TXC when it */
                                     /* is ok to perform APHO after it has*/
                                     /* been triggered by Srch/RXC.       */

#endif /* FEATURE_IS95B_ACCESS_PROBE_HO */


/*                       -----------------                                 */
/*                       MCC queue signals                                 */
/*                       -----------------                                 */

#define MCC_CMD_Q_SIG      0x00010000 /* This signal is set when an item   */
                                      /* is placed on the mc_cmd_q         */

#define MCC_RXTX_Q_SIG     0x00020000 /* This signal is set when an item   */
                                      /* is put on the mcc_rxtx_q          */

#define MCC_SRCH_Q_SIG     0x00040000 /* This signal is set when an item   */
                                      /* is put on the mc_srch_q           */

#define MCC_RX_RPT_Q_SIG   0x00080000 /* This signal is set when an item   */
                                      /* is put on the mcc_rx_rpt_q        */

#define MCC_RXTX_ACK_Q_SIG 0x00100000 /* This signal is set when an item   */
                                      /* is put on the mcc_rxtx_ack_q      */
#ifdef FEATURE_AUTH
#define MCC_AUTH_Q_SIG     0x00200000 /* This signal is set when an item   */
                                      /* is put on the mc_auth_q           */
#endif /* FEATURE_AUTH */

#if (defined (FEATURE_DH) && defined(FEATURE_DH_EXP))
#define MCC_DH_Q_SIG       0x00400000 /* This signal is set when an item   */
                                      /* is put on the mc_dh_q           */
#endif /*FEATURE_DH */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Signals 0x01000000 thru 0x80000000 are now available for REX signals    */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_IS95B_MDR
#define MCC_SCAM_TIMER_SIG         0x01000000
#endif /* FEATURE_IS95B_MDR */

#if ( defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) )
#define MCC_MDR_DURATION_TIMER_SIG 0x02000000
/* This is used for MDR supplemental channel duration timing */
#endif /* ( defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) ) */

#if defined (FEATURE_IS95B_PILOT_RPT) || defined (FEATURE_SPECIAL_MDR)
/* This signal is set by TX to notify MC to send the next access probe */
#define MCC_TXC_PROBE_SIG          0x04000000
#endif /* FEATURE_IS95B_PILOT_RPT || FEATURE_SPECIAL_MDR */

#define MCC_TXC_FRAME_SIG          0x08000000

#ifdef FEATURE_IS95B_MAHHO
#define MCC_HANDOFF_TIMER_SIG      0x10000000
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
#define MCC_TRANS_DONE_SIG         0x20000000
#endif

/* Note, We cannot use the 0x40000000 signal as this is used
by FS FS_OP_COMPLETE_SIG,  which MC waits on during boot up when 
it makes EFS function calls, on behalf of Runtime Dev Map module */

#ifdef FEATURE_IS2000_R_SCH
#define MCC_TXC_REV_DTX_SIG        0x80000000
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Constants and Masks for STORE_ESN_ME command to the R-UIM               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_RUIM
#define RUIM_STORE_ESN_ESN_MASK    0x0F
#define RUIM_STORE_ESN_USAGE_MASK  0x10
#define RUIM_STORE_ESN_LENGTH      0x04
#define RUIM_STORE_ESN_USE_ESN     0x00
#define RUIM_STORE_ESN_USE_UIM_ID  0x10
#endif /* FEATURE_RUIM */

/*-------------------------------------------------------------------------*/

/* The MC command free queue is declared as external here so that other    */
/* tasks can get free buffers from here before they queue them to the MC   */
/* command queue.                                                          */

extern  q_type  mc_cmd_free_q;

#ifdef FEATURE_AUTH
/* The auth report free queue is declared as external here so that other   */
/* tasks can get free buffers from here before they queue them to the      */
/* mc_auth_q.                                                              */

extern  q_type  mc_auth_free_q;
#endif /* FEATURE_AUTH */

#if (defined(FEATURE_DH) && defined(FEATURE_DH_EXP))
/* The auth report free queue is declared as external here so that other   */
/* tasks can get free buffers from here before they queue them to the      */
/* mc_dh_free_q.                                                           */
extern  q_type  mc_dh_free_q;
#endif /*FEATURE_DH */

/*-------------------------------------------------------------------------*/

/* Maximum dialed digits and data characters.  These are sent as ASCII     */
/* string.                                                                 */

#define MC_MAX_DIALED_DIGITS  32
#define MC_MAX_DATA_DIGITS    32

/*-------------------------------------------------------------------------*/
/*  P_REV values                       */

#define P_REV_JSTD008 1
#define P_REV_IS95A 3
#define P_REV_IS95B 4
#define P_REV_IS2000  6

/*-------------------------------------------------------------------------*/
/* Definitions for run-time P_REV and Band Class checking                  */

/* For Band Class checking */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  /* Tri-mode operation: we need run-time band-class
  checking */
  #define BAND_IS_CELLULAR ( mc_get_band_class() == CAI_BAND_CELL )
  #define BAND_IS_PCS ( mc_get_band_class() == CAI_BAND_PCS )
#elif defined(FEATURE_CDMA_800)
  #define BAND_IS_CELLULAR  (1)
  #define BAND_IS_PCS       (0)
#elif defined(FEATURE_CDMA_1900)
  #define BAND_IS_CELLULAR  (0)
  #define BAND_IS_PCS       (1)
#else
  #error Cannot determine CDMA band class.
#endif

/* Example of run-time BC checking:

   if ( BAND_IS_CELLULAR )
   {
     // Handle cellular processing here
   }
   else if( BAND_IS_PCS )
   {
     // Handle PCS processing here
   }
*/

/* For P_REV checking */
#if ( MOB_P_REV == 1 )                          /* JSTD-008 Only */
  #define P_REV_IS_1             (1)
  #define P_REV_IS_2_OR_3        (0)
  #define P_REV_IS_4_OR_GREATER  (0)
  #define P_REV_IS_5_OR_GREATER  (0)
  #define P_REV_IS_6_OR_GREATER (0)
#elif ( MOB_P_REV == 2 || MOB_P_REV == 3 )      /* IS-95A Only */
  #define P_REV_IS_1             (0)
  #define P_REV_IS_2_OR_3        (1)
  #define P_REV_IS_4_OR_GREATER  (0)
  #define P_REV_IS_5_OR_GREATER  (0)
  #define P_REV_IS_6_OR_GREATER (0)
#elif ( MOB_P_REV < 6 )                        /* IS-95B */
  #define P_REV_IS_1            (mc_get_p_rev_in_use() == 1)
  #define P_REV_IS_2_OR_3       (mc_get_p_rev_in_use() == 2 || \
                                 mc_get_p_rev_in_use() == 3)
  #define P_REV_IS_4_OR_GREATER (mc_get_p_rev_in_use() >= 4)
  #define P_REV_IS_5_OR_GREATER (mc_get_p_rev_in_use() >= 5)
  #define P_REV_IS_6_OR_GREATER (0)
#elif ( MOB_P_REV >=6 )
  #define P_REV_IS_1            (mc_get_p_rev_in_use() == 1)
  #define P_REV_IS_2_OR_3       (mc_get_p_rev_in_use() == 2 || \
                                 mc_get_p_rev_in_use() == 3)
  #define P_REV_IS_4_OR_GREATER (mc_get_p_rev_in_use() >= 4)
  #define P_REV_IS_5_OR_GREATER (mc_get_p_rev_in_use() >= 5)
  #define P_REV_IS_6_OR_GREATER (mc_get_p_rev_in_use() >= 6)
#else
  #error Undefined P_REV
#endif


/* Example of run-time P_REV checking

  if( P_REV_IS_1 )
  {
    // JSTD008 processing
  }
  else if( P_REV_IS_2_OR_3 )
  {
    // IS95A processing
  }
  else if( P_REV_IS_4_OR_GREATER )
  {
    // IS95B processing
  }
*/

/*-------------------------------------------------------------------------*/

/* Commands to be sent to the Main Control task */

typedef enum {
  MC_ONLINE_F,             /* Enter online operation.                      */
  MC_ORIGINATION_F,        /* Call origination.                            */
  MC_ANSWER_F,             /* Answer incoming call.                        */
  MC_FLASH_F,              /* Send flash.                                  */
  MC_END_F,                /* End call.                                    */
  MC_DATA_END_F,           /* End call.                                    */
  MC_START_CONT_DTMF_F,    /* Start continuous DTMF.                       */
  MC_STOP_CONT_DTMF_F,     /* Stop continuous DTMF.                        */
  MC_BURST_DTMF_F,         /* Send busrt DTMF.                             */
  MC_MO_DBM_F,             /* Send an mobile originated data burst message.
                              Currently, it includes SMS and PD messages   */
  MC_PRIVACY_PREF_F,        /* Enter privacy mode.                          */
  MC_PREF_SYS_CHANGED_F,   /* Preferred system changed.                    */
  MC_NAM_CHANGED_F,        /* Switched to another NAM.                     */
  MC_OFFLINE_DIGITAL_F,    /* Enter offline operation, digital mode.       */
  MC_OFFLINE_ANALOG_F,     /* Enter offline operation, analog mode.        */
  MC_RESET_F,              /* Perform a reset.                             */
  MC_POWERDOWN_F,          /* Perform power down processing.               */
  MC_JUMP_TRAFFIC_F,       /* Jump to traffic offline test command.        */
  MC_JUMP_ACCESS_F,        /* Jump to access offline test command.         */
  MC_SECTOR_LOCK_F         /* Use only one sector, ccdma_ch, pg_ch, acc_ch.*/
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
#ifdef FEATURE_UASMS
  ,MC_AWI_SMS_ACK_F
#endif /* FEATURE_UASMS */
#ifdef FEATURE_BROADCAST_SMS
  ,MC_BCSMS_ENABLE_F
#endif /* FEATURE_BROADCAST_SMS */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,MC_SILENT_REDIAL_F       /* Silent Redial                               */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#if defined (MMI_TREX) || (MMI_TYPE == MMI_A10) || (MMI_TYPE == SKY_UI)
  ///플래쉬 데이터를 삭제중에는 페이징을 잃어비리므로 삭제 후에 system determination mode로 
  ///보내야 한다. 이를 지정하는 명령어다.
  , MC_RE_DETERMIN_F
#endif
#ifdef	FEATURE_SKT_INTRAFFIC_MONEGO	//	khekim 01/02/18
  ,MC_INTRAFFIC_MONEGO_F	//	In Traffic MO nego command
#endif /* FEATURE_SKT_INTRAFFIC_MONEGO */
#ifdef FEATURE_IS2000_R_SCH
  ,MC_SCRM_F              /* SCRM request from DS */
#endif /* FEATURE_IS2000_R_SCH */
#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)
  ,MC_WAP_F /* for MUX Option */
#endif //WAP95C_TEST || FEATURE_WAP_MUXOPTION
#ifdef FEATURE_MULTI_SID
  ,MC_SELECTED_SID_F    // by reo
#endif //FEATURE_MULTI_SID
} mc_name_type;

/*--------------------------------------------------------------------------*/
/* The following enumeration enumerates actions to be taken by MC for a
 * given silent redial command. Anytime a MC_SILENT_REDIAL_F is sent to
 * MC, mc_sr_name_type information shall be passed along with the command
 */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef enum
{
  MC_SR_SCANLIST_AUTOMATIC, /* build an Automatic silent redial scan list */
  MC_SR_END,                /* end silent redial */
  MC_SR_NAME_TYPE_COUNT     /* count of enum items */
} mc_sr_name_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Status returned from the Main Control task */

typedef enum {
  MC_DONE_S,            /* General OK return */
  MC_BUSY_S,            /* Command is being processed */
  MC_BADCMD_S,          /* Invalid command */
  MC_BADPARM_S          /* Bad Parameters */
} mc_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Command header type.  Each command to MC starts with this header.       */

typedef struct {
  cmd_hdr_type       cmd_hdr;      /* Command header */
  mc_name_type       cmd;          /* Command name */
  mc_status_type     status;       /* Completion code status */
} mc_hdr_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_ONLINE_F.                                                   */

typedef struct {
  mc_hdr_type  hdr;                   /* Header */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

} mc_online_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_ORIGINATION_F.                                              */

#ifdef FEATURE_OTASP
typedef enum {
  ABAND,
  BBAND,
  ABLOCK,
  BBLOCK,
  CBLOCK,
  DBLOCK,
  EBLOCK,
  FBLOCK,
  PREFERRED
} mc_activate_code_type;

/* Macro to convert Activation code to System */
/* Does not work with act_code == PREFERRED */
#define ACTIVATE_CODE_TO_SYS(act_code) ((act_code + 6)%8)
#endif /* FEATURE_OTASP */

#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
typedef enum {
  ORIGINATION,    /* regular origination */
  SMS,            /* SMS origination */
  POSITION_DET    /* PD message origination */
} mc_orig_type_type;
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */

typedef struct {
  mc_hdr_type  hdr;                      /* Header                         */
  word         service_option;           /* Service option number          */
#ifdef FEATURE_ACP
  boolean      force_amps;               /* Force AMPS call indicator      */
#endif /* FEATURE_ACP */
#ifdef FEATURE_DS
  boolean      ds_originated;            /* Source of Orig, UI or DS       */
  boolean      amps_data;                /* AMPS only data/voice indicator */
#endif /* FEATURE_DS */
  boolean      digit_mode;               /* TRUE if digits have non-numeric*/
  byte         number_type;              /* Number type for ASCII digit */
  byte         number_plan;              /* Number plan for ASCII digit */
  byte         cnt;                      /* How many digits                */
  byte         called_address[MC_MAX_DIALED_DIGITS];  /* Called address */
#ifdef FEATURE_OTASP
  boolean      otasp_activation;         /* If this is an OTASP activation */
  mc_activate_code_type activate_code;   /* Tells where to look for service*/
#endif /* FEATURE_OTASP */
#ifdef FEATURE_IS2000
  boolean      drs;                      /* DRS (data ready to send) bit in ORM */
#endif
} mc_origination_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_ANSWER_F.                                                   */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */
#ifdef FEATURE_DS
  boolean      ds_answered;                  /* indicates DS answered call */
  boolean      a_data;                       /* indicates A-data call      */
#endif /* FEATURE_DS */
} mc_answer_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_FLASH_F.                                                    */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */
  byte         cnt;                                   /* How many digits */
  byte         called_address[MC_MAX_DIALED_DIGITS];  /* Dialed digits */
} mc_flash_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_END_F.                                                      */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */
#ifdef FEATURE_IS2000
  byte         end_reason;  /* the reason for ending a call */
#endif /* FEATURE_IS2000 */
} mc_end_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_START_CONT_DTMF_F.                                       */

typedef struct {
  mc_hdr_type  hdr;                    /* Header */
  byte         digit;                  /* DTMF digit (tone) to send */
} mc_start_cont_dtmf_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_STOP_CONT_DTMF_F.                                           */

typedef struct {
  mc_hdr_type hdr;          /* Header */
  word        ms;           /* Duration of DTMF in milliseconds */
} mc_stop_cont_dtmf_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_BURST_DTMF_F                                                */

typedef struct {
  mc_hdr_type hdr;          /* Header */
  byte        on_length;    /* DTMF pulse width
                               (see IS-95A Table 6.7.2.3.2.7-1) */
  byte        off_length;   /* DTMF interdigit interval
                               (see IS-95A Table 6.7.2.3.2.7-2) */
  byte        cnt;          /* Digit count */
  byte        digits[MC_MAX_DIALED_DIGITS]; /* DTMF digits */
} mc_burst_dtmf_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for SMS request.                                       */

#ifdef FEATURE_UASMS
/* Type for MC_AWI_SMS_ACK_F.                                  */
typedef struct {
  mc_hdr_type              hdr;            /* Header           */
  boolean                  bf;             /* begin/final flag */
  byte                     error_class;    /* IS-41 error class*/
  byte                     status;         /* IS-41 error type */
  byte                     seq_no;         /* Current sequence number */
} mc_awi_sms_ack_type;

#endif /* FEATURE_UASMS */


/* Type for MC_MO_DBM_F.                                       */

typedef struct {
  mc_hdr_type  hdr;        /* Header           */
#if (defined(FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
  byte         burst_type; /* Data Burst Type. Currently, it
                              supports SMS and PD only. */
  boolean      ack_req;    /* L2 ACK required  */
  void         *dbm_buf_ptr;
                           /* Actual data is in a static buffer
                              to save space */
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */
} mc_mo_dbm_type;


#ifdef FEATURE_BROADCAST_SMS
typedef struct
{
  mc_hdr_type   hdr;
  boolean       enable;
} mc_bcsms_enable_type;
#endif /* FEATURE_BROADCAST_SMS */



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_PRIVACY_PREF_F.                                             */
typedef enum {
  MC_LONG_CODE_PUBLIC,
  MC_LONG_CODE_PRIVATE
} mc_long_code_e_type;


typedef struct {
  mc_hdr_type  hdr;                         /* Header */
  mc_long_code_e_type long_code;            /* On or Off */
} mc_privacy_mode_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_PREF_SYS_CHANGED_F.                                         */

typedef struct {
  mc_hdr_type  hdr;           /* Header */
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
  nv_mode_enum_type mode;               /* New mode preference */
  nv_sys_pref_enum_type sys;            /* New system preference */
#endif /* FEATURE_SD20 */
} mc_pref_sys_changed_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_NAM_CHANGED_F.                                              */

typedef struct {
  mc_hdr_type  hdr;                   /* Header */
  byte         nam;                   /* New NAM */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
  
} mc_nam_changed_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_OFFLINE_F.                                                  */

typedef struct {
  mc_hdr_type  hdr;                             /* Header */
} mc_offline_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_OFFLINE_ANALOG_F.                                           */

typedef struct {
  mc_hdr_type  hdr;                             /* Header */
} mc_offline_analog_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_OFFLINE_DIGITAL_F.                                          */

typedef struct {
  mc_hdr_type  hdr;                             /* Header */
} mc_offline_digital_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_RESET_F.                                                    */

typedef struct {
  mc_hdr_type  hdr;                             /* Header */
} mc_reset_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_POWERDOWN_F.                                                */

typedef struct {
  mc_hdr_type  hdr;                             /* Header */
} mc_powerdown_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_JUMP_TRAFFIC_F.                                             */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */
  dword esn;             /* Electronic serial number */
  byte code_chan;        /* Code Channel */
  word cdma_freq;        /* Frequency Assignment for the CDMA channel */
  byte frame_offset;     /* Frame offset */
  byte encrypt_mode;     /* Message encryption mode */
  word so;               /* Service option to use */
  byte mar_rate;         /* Rate for Markov SO Call */

} mc_jump_traffic_type;


/* Type for MC_JUMP_ACCESS_F.                                             */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */

  word          len;              /* Number of bytes in message */
  boolean       req;              /* True: access request, False: response */
  byte          seq_num;          /* Max number of access probe sequences */

  /* set the next 3 parameters all to zero to get a long code mask
  ** of ZERO which implies no scrambling.
  */
  byte          pagech;           /* Current Paging Channel */
  word          base_id;          /* Base station identification */
  word          pilot_pn;         /* Pilot PN sequence offset index */

  byte          pam_sz;           /* Access Channel preamble length */
  byte          max_cap_sz;       /* Max Access Channel msg capsule size */
  byte          bkoff;            /* Probe sequence backoff range */
  byte          probe_bkoff;      /* Probe backoff range */
  byte          acc_tmo;          /* Acknowledgement timeout */
  byte          init_pwr;         /* Initial power offset for access */
  byte          pwr_step;         /* Power increment */
  byte          num_step;         /* Number of access probes */
  byte          acc_chan;         /* Number of access channels */
  word          rn;      /* Random number of chips backoff - ignored now */
  dword         p;                /* Persistence probability */

  byte          *msg_ptr;    /* pointer to message buffer */

} mc_jump_access_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_SECTOR_LOCK_F.                                             */

typedef struct {
  mc_hdr_type  hdr;                                   /* Header */
  word pn_offset;        /* Pilot PN to acquire */
  word cdma_chan;        /* Frequency Assignment for the CDMA channel */
  byte paging_chan;      /* Paging channel to use */
  byte access_chan;      /* Access channel to use */

} mc_sector_lock_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for MC_SILENT_REDIAL_F. Any task can use this message to convey
 * silent redial information to MC task. This message contains the silent
 * redial type and also conveys whether roam_side originations are allowed
 * or not.
 */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef struct
{
  mc_hdr_type     hdr;         /* header information */
  mc_sr_name_type type;        /* silent redial type */
  boolean         home_only;   /* roam_side originations allowed or not*/
} mc_sr_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)
typedef struct {
  mc_hdr_type     hdr;         /* header information */
  int          select;         /* option value */  
} mc_wapmux_type;
#endif //#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-------------------------------------------------------------------------*/

/* Union of all command types.  The header is always present and it        */
/* specifies the command type and attributes.  The command messages        */
/* follow in the union.                                                    */

typedef union {
  mc_hdr_type                 hdr;
  mc_online_type              online;
  mc_origination_type         origination;
  mc_answer_type              answer;
  mc_flash_type               flash;
  mc_end_type                 end;
  mc_start_cont_dtmf_type     start_cont_dtmf;
  mc_stop_cont_dtmf_type      stop_cont_dtmf;
  mc_burst_dtmf_type          burst_dtmf;
  mc_mo_dbm_type              mo_dbm;
  mc_privacy_mode_type        privacy_mode;
  mc_pref_sys_changed_type    pref_sys_changed;
  mc_nam_changed_type         nam_changed;
  mc_offline_type             offline;
  mc_offline_analog_type      offline_analog;
  mc_offline_digital_type     offline_digital;
  mc_reset_type               reset;
  mc_powerdown_type           powerdown;
  mc_jump_traffic_type        jump_traffic;
  mc_jump_access_type         jump_access;
  mc_sector_lock_type         sector_lock;
#ifdef FEATURE_UASMS
  mc_awi_sms_ack_type         awi_ack;
#endif /* FEATURE_UASMS */
#ifdef FEATURE_BROADCAST_SMS
  mc_bcsms_enable_type        bcsms_enable;
#endif /* FEATURE_BROADCAST_SMS */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  mc_sr_type                  silent_redial;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)
  mc_wapmux_type              waptest;
#endif //#if defined (WAP95C_TEST) || defined (FEATURE_WAP_MUXOPTION)
} mc_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Enumerations for event report                                           */

/* Operation modes */
typedef enum {
  MC_OP_MODE_POWERUP, /* Power up mode */
  MC_OP_MODE_AMPS,    /* Analog operation mode */ 
  MC_OP_MODE_CDMA     /* CDMA operation mode */
} mc_event_op_mode_type;

/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION MC_TASK

DESCRIPTION
  This is the MC task entry point.

DEPENDENCIES
  None.

RETURN VALUE
  None.  This function never returns.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mc_task
(
  dword param   /* Entry paramater, not used. */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MC_CMD

DESCRIPTION
  This is the outside world's interface to the main control task.
  This function takes a filled out mc_msg_type which it places
  on the mc command queue, it marks the request as busy in the status
  field, it signals the mc task and then it returns to the caller.
  Return to the caller does not mean that the command has been executed
  yet, only that it is queued for execution.  When mc is done processing
  the request it returns the buffer to the specified queue if a return
  queue adress is provided and it signals the calling task if specified.

  An example MC origination request is as follows:

  mc_msg_type  msg_buf;                       - Define message buffer
  byte         digits[MC_MAX_DIALED_DIGITS]   - Called address buffer

  msg_buf.hdr.cmd = MC_ORIGINATION_F;         - Origination request
  msg_buf.hdr.task_ptr = rex_self();          - Caller's TCB
  msg_buf.hdr.sigs = XXX_MC_SIG;              - Caller's end signal
  msg_buf.hdr.done_q_ptr = NULL;              - No buffer return

  msg_buf.msg.origination.cnt = sizeof(called_address);
  strncpy (msg_buf.msg.origination.called_address, digits, sizeof(digits));

  rex_clr_sigs (rex_self(), XXX_MC_SIG);      - Clear the signal
  mc_cmd (&msg_buf);                          - Issue the command

DEPENDENCIES
  None.

RETURN VALUE
  None directly.  The status variable of the mc_hdr_type will be updated
  to reflect the current status of this command as it is processed.  In the
  above example msg_buf.origination.hdr.status is set to:

  MC_DONE_S             - General OK return
  MC_BUSY_S             - Command is being processed
  MC_BADCMD_S           - Invalid command
  MC_BADPARM_S          - Bad Parameters

SIDE EFFECTS
  The msg_buf is placed on the MC command queue.  It must not be modified
  until the command has been processed and its status changed to MC_DONE_S.

===========================================================================*/

extern void mc_cmd
(
  mc_msg_type *msg_ptr  /* Pointer to mc cmd message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MC_TASK_START

DESCRIPTION
  This function is called by each task when it is created.  The function
  must be called before the task enters normal processing.  The function
  then performs the startup processing for the calling task.  The function
  first sets the acknowledge signal back to MC to acknowledge that is had
  been created.  It then waits for the start signal back to it.  While
  waiting for the start signal and keeps kicking the watchdog timer
  everytime its interval timer has expired.  When it gets the start
  signal it acknowledges it back to MC and it then returns to the caller.
  The caller task is now formally started and it can start normal application
  processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The calling task stays here until it receives the START signal.

===========================================================================*/

extern void mc_task_start
(
  rex_sigs_type    rpt_sig,        /* report timer signal for task */
  dog_report_type  dog_rpt,        /* Watchdog report for the task */
  rex_timer_type   *rpt_timer_ptr  /* pointer to report timer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MC_STATE

DESCRIPTION
  This function returns the current state of MC.

DEPENDENCIES
  None.

RETURN VALUE
  Current state of MC.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mc_state( void );

#ifndef FEATURE_SD20
/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
/*===========================================================================

FUNCTION MC_GET_PRL_VERSION

DESCRIPTION
  This function returns the PRL version of the PRL for the current NAM.

DEPENDENCIES
  None.

RETURN VALUE
  The PRL version number determined as follows:
    - MCSYSPR_DEFAULT_PRL_VER if the roaming list is invalid or if no
      roaming list stored in NV or if the roaming does not contain a
      system record with MCSYSPR_SPECIAL_SID_FOR_PRL_VER in the SID field;
    - "NID value" in the system record with MCSYSPR_SPECIAL_SID_FOR_PRL_VER
      in the SID field. If not NID_INCL, the PRL version number is
      PRL_WILDCARD_NID - 65535.

SIDE EFFECTS
  None.

===========================================================================*/

extern word mc_get_prl_version(void);

#ifdef FEATURE_SSPR_800
/*===========================================================================

FUNCTION MCSYS_ROAMING_LIST_VALID

DESCRIPTION
   Tells the caller if MC has a roaming list it can use.

DEPENDENCIES
  None.

RETURN VALUE
   TRUE if there is a roaming list in NV and it is valid,
   FALSE otherwise.

SIDE EFFECTS
   None.

===========================================================================*/
extern boolean mcsys_roaming_list_valid( void );
#endif /* FEATURE_SSPR_800 */
#endif /* FEATURE_PREFERRED_ROAMING */
#endif /* !FEATURE_SD20 */

/*===========================================================================

FUNCTION MC_GET_BAND_CLASS

DESCRIPTION
  This function returns the current operating band class of the mobile.

DEPENDENCIES
  None.

RETURN VALUE
  Current band class.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mc_get_band_class(void);


/*===========================================================================

FUNCTION MC_GET_P_REV_IN_USE

DESCRIPTION
  This function returns the current P_REV_IN_USE of the mobile.

DEPENDENCIES
  None.

RETURN VALUE
  Current P_REV_IN_USE.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mc_get_p_rev_in_use(void);

#ifdef FEATURE_SD20      
#error code not present
#endif /* FEATURE_SD20 */

/*===========================================================================

FUNCTION MC_REPORT_EVENT_OP_MODE

DESCRIPTION
   Updates internal data and report events if necessary when operation mode
   changes.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
extern void mc_report_event_op_mode
(
  mc_event_op_mode_type new_op_mode
);

#endif /* MC_H */

