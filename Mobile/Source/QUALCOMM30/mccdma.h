#ifndef MCCDMA_H
#define MCCDMA_H
/*===========================================================================

    M A I N   C O N T R O L   T A S K   C D M A    S U B - S Y S T E M
                       H E A D E R    F I L E

DESCRIPTION
  This file data and function declarations necessary for the CDMA Sub-system
  of the Main Control task.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mccdma.h_v   1.14.2.0   04 May 2001 11:48:02   varavamu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/17/01   va      Added nghbr_pilot_rec_type in the neighbor list.
                   Fixed bug in GNLM_MSG_IS_RECVD and GNLM_MSG_NOT_RECVD macro.
03/06/01   fc      Added support for events report: Message received, Message
                   transmitted, Timer expired, Counter threshold and Call 
                   processing state changes.
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
02/16/01   sh      added support for MC_END_F ack
01/24/01   ych     Merged T-53/JCDMA features for SK.
01/16/01   jq      Moved/renamed mccsch_retry_delay_type, mccsch_retry_delay to
                   here.
12/14/00   cah     Remove a uasmsi.h inclusion for FEATURE_SMS.
12/13/00   cah     Replaced obsolete smsi.h inclusion with uasmsi.h.
11/28/00   ck      Externalised the definition for RUIM Access buffer.
                   Added ruim_ota_features to the cdma structure.
11/13/00   lcc     Added function prototype for mcc_update_bs_p_rev.
11/01/00   lcc     Added drs member in cdms_ms_type to support DRS bit in ORM.
09/11/00   yll     Added support for Position Determination DBM.
09/06/00   jq      Implemented FEATURE_FACTORY_TESTMODE based on FEATURE_PLT
08/23/00   jq      Added mcc_set_rsch_dtx_sig ()
08/18/00   fc      Added retry_type and retry_delay to mccdma_pc_ord_type.
07/21/00   fc      Used enumeration type for FPC mode.
07/17/00   jq      added end_reason to cdma_ms_type.
07/14/00   ks      defined t_slotted in cdma_ms_type. Defined ecl_rx, egsd_rx
                   in cdma_msg_type. Define ecl, egsd in cdma_bs_type. These
                   fields support new PC messages and Slotted timer.
07/14/00   va      Added pwr_cntl_delay and rev_fch_gating_on in cdma_ms_type.
07/06/00   yll     Added the interface to provide the MSID and registered BS
                   info, include: band class, channel, bs ID, SID and NID.
06/26/00   sh      added AEHO_OVHD_MSG_OK
06/14/00   ks      Added band_class parameter to cdma_rf_init function.
06/04/00   jq      Changed parameter list of mcc_parse_page() and
                   mcc_detect_dup_bcast()
05/26/00   jq      Added slot_mask related defines and processing functions.
                   Fixed MAX_BC_PER_PAGE_MSG.
04/25/00   sh      Increase MC_MAX_BASES to 10
04/11/00   lcc     Added data structure and members to support QPCH.
04/06/00   va      Merged the following from PP (MSM3100_CP.03.00.17):
           jq      Added Broadcast SMS feature support.
03/10/00   ry      Added IS2000 featurization
           ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ht      Add mcsyspr_set_last_cdma function prototype.
           ram     Fixed a typo in my last check-in which caused problems
                   in the SUPPORTED_BANDS, SUPPORTED_CDMA_BANDS, and
                   SUPPORTED_OP_MODES macros for trimode targets.
           ck      Added band class to the cdma_bs_type and also the to
                   the mcc_entry_init prototype.
           ram     Extended the Macros SUPPORTED_BANDS, SUPPORTED_CDMA_BANDS,
                   SUPPORTED_OP_MODES to include Trimode and Dual CDMA band
                   Targets.
01/26/00   va      Merged in changes from GV40401
                   Removed mcc_handoff_timer.
                   Modified SUPPORTED_OP_MODES to allow op codes 0 and 1 for
                   IS-95 MS in cellular or PCS band.
12/20/99   va      Added new data struct to store power control info temporarily.
                   Added fpc_subchan_gain to cdma_ms_type.
11/15/99   va      IS2000 cHANGES FOR VOICE.
                   Support for QOF.
                   mccdma_chnasn_type expanded for support of enhanced ECAM.
                   New mcros added for RC_CLASS() determination.
                   mcc_info_rec prototype changed.
10/28/99   lcc     Moved PLT related code to plt_mc.c.  Also added extern
                   function declarations for some functions used in plt_mc.c
08/24/99   fc      Removed nset_cnt from plt_rpt_ovhd_rec_type because SRCH
                   will provide the count of pilots in Pilot Strength
                   Measurement report.
08/21/99   ks      Modified plt_rpt_ovhd_rec_type and othrpt_pilot_info to
                   support pilot reporting for already attempted pilots.
08/13/99   lh      CM and SMS phase 2 implementations.
08/09/99   ry      Changed FEATURE_IS95B_HARD_HO to FEATURE_IS95B_MAHHO
           ry      Fixed a merge problem in which a #endif for FEATURE_ACP
                   was misplaced
07/28/99   lh      CM and two-way SMS support added.
07/28/99   lh      CM support added.
07/22/99   fc      Changes in Pilot Reporting to support Access Probe Handoff.
07/19/99   abh     Changes for Access Probe Handoff.
07/09/99   ry      Added a new rex time for IS95B hard handoff
06/24/99   ks      Fixed a typo in AHO_OVHD_MSG_OK macro
06/22/99   kmp     Moved the definitions for IMSI_S1_ZERO, IMSI_S2_ZERO,
                   IMSI_MCC_ZERO and IMSI_11_12_ZERO to this file from
                   mccdma.c since these values are also used in mccidl.c in
                   function proc_true_imsi() when FEATURE_IS95B_TRUE_IMSI is
                   defined.
06/17/99   kmp     Merged in the following IS-95B changes.
06/13/99   fc      Changed the data structure of plt_rpt_ovhd_rec_type,
                   aho_pilot_info_type and othrpt_pilot_info.
06/07/99   ks      Added roam_indi field in mccdma_pc_ord_type structure.
06/04/99   jq      Added cdma_sa_orig_ministate_type for Release on Access
                   Channel feature
06/04/99   sh      changed data type from byte to int1 for add_intercept and
                   drop_intercept in mcc_ho_cb_type
05/26/99   fc      Added the support for closed loop power control step size
                   in the cdma_ms_type structure.
05/24/99   doj     Added p_rev_in_use to cdma_ms_type (for FEATURE_IS95B).
                   This was already done in the IS-95B branch is required
                   for all builds not just FEATURE_IS95B.
05/12/99   ks      Added Macros to check if AMPS is supported for a band class.
05/10/99   kmp     Merged the following IS-95B related changes and changed
                   T_SMS, T_ACP and T_AUTH to FEATURE_SMS, FEATURE_ACP,
                   and FEATURE_AUTH respectively.
           ks      Added AHO_OVHD_MSG_OK macro to validate overhead messages
                   during Access Handoff or Access probe Handoff.
05/06/99   kmp     Merged IS-95B changes below and changed T_SMS, T_ACP and
                   T_AUTH to FEATURE_SMS, FEATURE_ACP, and FEATURE_AUTH
                   respectively.
           kmp     Modified the mcc_pc_frames array to be 3D by adding
                   ADDR_TYPE checking for duplicate page messages for IS-95B.
           ks      Addded an external function is_in_access_ho_list.
           ks      Added an external function is_access_ho_allowed.
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Put in changes based on code review input.
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Put in changes based on code review input.
           jq      Modified some naming due to code review
           fc      Fixed lint error.
           ks      Removed CDMA_IDLE_AHO from cdma_idle_state_type enum.
           kmp     changed type of cdma.ho_state to cdma_ho_type from a word
                   Added include for mccsrch.h for pc_srch_rpt function proto.
           fc      Modified comments. Renamed plt_rpt_bookeep structure to
                   plt_rpt_ovhd_rec and pilot_rpt_bookeep structure to
                   pilot_rpt_ovhd_rec.
           jq      Added declaration for pc_srch_rpt, so lint is happy
           jq      Modified mccdma_chnasn_type, and channel assignment
                   related structures to accomodate both CAM and ECAM
           lh      Authentication using IMSI_M.
           fc      Defined ACCESS_HO_LIST, OTHER_REPORTED_LIST and
                   PLT_RPT_BOOKEEP.
           fc      Added PLT_RPT_BKP, ACC_HO_LIST and OTHER_RPTED_LIST data
                   types for pilot reporting. Added ACCESS_ATTEMPTED field in
                   CDMA_NGHBR_SET_TYPE to support pilot reporting. Defined
                   NO_PREV_PILOT. Defined ACC_MSG_NUM_PLT_ALLOWED() and
                   ACC_MSG_PLT_RPT_NEEDED() macros.
           ks      Removed CDMA_HO_STATE from main state enumerations.
           sh      Added mcc_send_srch_parms function.
           ks      Defined a new enumeration for various handoffs.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           kmp     Added Phased Deployment support, added PACA cancel substate
                   to the cdma_sa_state_type enum.
           sh      added IS95B soft handoff parameters
           ks      Added a new macro to check status of Nghbr List messages.
           ks      Added IS95B overhead message processing fields. Added fields
                   ho_sa_state and ho_state fields for Access Handoff. Added
                   cdma_prev_bs_info data type for holding previous BS info.
           lh      p_rev_in_use is added to cdma_ms_type structure.
           lh      IS-95B True IMSI implementation added.
06/21/99   lh      Two-way SMS and CM support.
06/01/99   sh      added mcc_check_alert_signal_info()
04/25/99   abh     Added prototype for mcc_init_ovhd_info, which was introduced to
                   move the initilisation of bs_info array at the start of CDMA Acquisition
                   process, rather than after getting Sync Message.
04/07/99   abh     Changes for the new Queue Interface.
03/18/99   ck      Added imsi_t variables, auth_data and spc_change_enabled
                   to cdma structure.
03/18/99   ck      Do not define mcc_voc_selftest if FEATURE_VOCODER_MANAGER.
                   Merged the change from SPSW rev 1.85 by ro
03/09/99   ram     Externalised mcc_mdr_duration_timer for IS95B MDR
02/19/99   ram     Externalised mcc_scam_start_timer for IS95B MDR
01/27/99   ck      mcc_pref_voice_so is ifdefed under FEATURE_EVRC_SO_MGMT
12/08/98   dna     Fixed some compile warnings (merged from ASW 1.11)
10/09/98   ck      Externalised mcc_pref_voice_so
09/15/98   pms     Backed out _near pointers
09/04/98   pms     Neared the variables to save ROM space.
08/25/98   ck      Added functions mcc_queue_dh_rpt() and mcc_dh_cmd() to
                   support the dh task.
08/24/98   dna     Retain message encryption on CDMA->AMPS handoff by
                   separating auth_reset from auth_resync.
08/20/98   ck      Removed mcc_voc_capability definition, no longer needed.
08/04/98   ych     Merged SSPR, E911 and Acquisition improvements.
07/09/98   lcc     Fixed problem with illegal characters after #endif statement
07/02/98   pms     Added element "end_by_data" to cdma_ms_type struct.
07/02/98   pms     Added prototype for process_ds_call_failed().
06/25/98   kis     Added digit_mode and ASCII related fields in cdma_ms_type.
05/21/98   abh     MAX_SID_NID changed to MAX_HOME_SID_NID for SSPR-800
02/18/98   dna     Added SUPPORTED_CDMA_BANDS and BIT_MASK macro to allow
                   System Determination to run-time check for supported band.
01/27/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
01/27/98   ych     Featurized mcc_idle_timeout and defined a constant
                   MAX_DWORD_VAL. Also added customer.h to the include file
                   list
08/17/97   ych     Changed the MCC_VALID_OVHD_TIMEOUT timer value to 30000
                   from 10000.
07/15/97   dna     Moved SUPPORTED_BANDS and SUPPORTED_OP_MODES here.
06/16/97   dna     New func. sets hold_orig and timer when returning to idle
05/20/97   ck      Added support for LPM.
04/18/97   dna     Changed some #ifdefs to make class 1 IMSI its own feature
01/25/97   dna     More flexible reject of status request.
12/13/96   jca     Added new fields to mccdma_chnasn_am2_type.
10/18/96   dna     Added TEMP_NL_SEQ
09/13/96   dna     Added cdma.otasp_call flag.
09/11/96   jca     Changed mcc_xlate_dtmf() to mcc_encode_dtmf().  Added new
                   function mcc_decode_dtmf().
08/08/96   dna     Add Service Programming Code to cdma. structure for OTASP.
07/10/96   day     Add mcc_pc_pg_frames array for layer2 duplicate detection.
                   Add input parameter to identify page/non-page messages
                   in mcc_detect_duplicate.
06/25/96   dna     For FEATURE_OTASP, added mob_dir to cdma structure.
06/10/96   jpf     Removed BS tmsi_zone from cdma_ms_type.
05/28/96   dna     Added imsi_addr_num for class 1 IMSI support.
05/24/96   jjw     Gemini & ISS2 Data Services release.
05/15/96   dna     Changed auth_on to auth_mode and encryption_enabled to
                   encrypt_mode.
05/13/96   jca     Added band_class and block_or_sys to cdma_ms_type.
04/15/96   jca     Changed cdma_dtmf_type to accomodate queued Burst DTMFs.
04/09/96   day     Added TMSI addressing modifications.  Using T_TMSI ifdef.
03/13/96   day     Added J-STD-008 PCS message modifications.  Using FEATURE_JSTD008
                   ifdef.
02/08/96   jca     Put ()s around macros to make Lint happy.
12/08/95   bns     Removed flag for ISS2 forced registration
12/06/95   bns     Added flag for ISS2 forced registration
11/29/95   jca     Deleted sid_acq field from cdma_ms_type.
11/14/95   jca     Removed support for configurable service option 1 def.
10/05/95   dna     Added cdma.authr_valid so zero could be valid RAND value
08/18/95   dna     Changed mcc_pc_frames array, added assign mode '101'
08/17/95   jca     Added function mcc_process_info_records().
08/10/95   rdb     Added declarations to support enhanced service messaging.
07/26/95   jca     Added an_chan_type field to mccdma_chnasn_am3_type.
07/20/95   jca     Added support for Subscriber Access Control.
07/13/95   rdb     Added types for assign mode 4 of chan assign message
                   Added serv_neg to cdma structure.
06/25/95   dy      Added CDMA SMS data structures and functions to support
                   transmission of SMS messages.
06/09/95   jca     Changed mcc_wait() interface to support new queue sigs.
06/07/95   dna     Added Authentication Definitions
11/14/94   dna     Added headers for all externalized functions.
                   Added page_chan_s to cdma_ms_type.
08/09/94   jca     Deleted function mcc_get_voc_version and other misc mods.
05/12/94   gb      Added new test commands.
04/25/94   gb      Parameterised all IS95 timeouts in cai_tmo.
04/20/94   jca     Modified for IS-95 Phase 2 release.
12/22/93   jca     Modified for IS-95 Phase 1 release.
03/24/93   gb      Removed debug t_tdrop variable.
07/24/92   jai     Changed externs for several procedures for modified calling
                   parameters.
06/26/92   jai     Created file for DMSS.
12/20/90   jai     Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "target.h"
#include "comdef.h"
#include "queue.h"
#include "qw.h"
#include "mc.h"
#include "mcc.h"
#include "mccrxtx.h"
#include "rxc.h"
#include "txc.h"
#include "rxtx.h"
#include "nv.h"
#include "srch.h"
#include "cmmc.h"
#include "db.h"
#include "caii.h"
#include "voc.h"
#ifdef FEATURE_UASMS
#include "uasmsi.h"
#endif /* FEATURE_UASMS */
#ifdef FEATURE_AUTH
#include "auth.h"
#endif /* FEATURE_AUTH */
#ifdef FEATURE_DS
#include "ds.h"
#endif /* FEATURE_DS */
#ifdef FEATURE_OTASP
#include "otaspi.h"
#endif /* FEATURE_OTASP */
#ifdef FEATURE_DH
#include "dh.h"
#endif /* FEATURE_DH */
#include "mccsrch.h"

#ifdef FEATURE_IS2000_QPCH
  #include "mccqpch.h"
#endif

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */ 

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Main states */
#define  CDMA_ENTER            0x0000
#define  CDMA_INIT             0x0100
#define  CDMA_IDLE             0x0200
#define  CDMA_SYS_ACC          0x0300
#define  CDMA_TC               0x0400
#define  CDMA_EXIT             0x0500

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Enumerations for substates                                              */

/* State codes are separated by 256 and substate codes are consecutive.
   No substates are needed for the CDMA_ENTER or CDMA_EXIT states.         */

/* ---------------------------------
** Substates of Initialization State
** --------------------------------- */
typedef enum
{
  CDMA_PIL_ACQ = CDMA_INIT,  /* Pilot Channel Acquisition */
  CDMA_SYNC_ACQ,             /* Sync Channel Acquisition (part of IS-95
                                Sync Channel Acquisition substate) */
  CDMA_SYNC_MSG,             /* Process Sync channel Message (part of
                                IS-95 Sync Channel Acquistion substate) */
  CDMA_JUMP,                 /* Jump to Hyperspace (IS-95 Timing Change
                                Substate) */
  CDMA_UNJUMP                /* Jump back from Hyperspace */
} cdma_init_state_type;

/* -----------------------
** Substates of Idle State
** ----------------------- */
typedef enum
{
  CDMA_IDLE_INIT = CDMA_IDLE,    /* Initial entry into idle */
  CDMA_IDLE_MSG_PENDING,         /* Had to send ACK and leave msg pending.
                                    System Access finished, process msg now */
  CDMA_UNSLOTTED,                /* Idle not operating in slotted mode */
  CDMA_SLOTTED,                  /* Idle operating in slotted mode */
  CDMA_OVHD                      /* System Access Update Overhead info */
} cdma_idle_state_type;

/* --------------------------------
** Substates of System Access State
** -------------------------------- */
typedef enum
{
  CDMA_UPDATE_OVH = CDMA_SYS_ACC, /* Update Overhead Information */
  CDMA_ORIG,                      /* Mobile Station Origination Attempt */
  CDMA_PAGE_RES,                  /* Page Response */
  CDMA_ORD_RES,                   /* Mobile Station Order/Message Response */
  CDMA_REG,                       /* Registration Access */
  CDMA_MSG                        /* Message transmission */
#ifdef FEATURE_IS95B
  ,CDMA_PACA_CAN                  /* PACA Cancel Substate */
#endif /* FEATURE_IS95B */
} cdma_sa_state_type;

#if defined(FEATURE_IS95B_RELEASE_ON_AC) || defined(FEATURE_T53)
/* Substates of Origination state. With Release Order on AC feature, the
 * mobile could be waiting for Lay2 ACK of Release Order instead of CAM/ECAM
 * in the Origination state. */
typedef enum
{
  CDMA_ORIG_NORM,   /* Origination state while not in release process */
  CDMA_ORIG_RELS    /* Origination state while in release process */
} cdma_sa_orig_ministate_type;
#endif /* FEATURE_IS95B_RELEASE_ON_AC || FEATURE_T53 */

/* ----------------------------------
** Substates of Traffic Channel State
** ---------------------------------- */
typedef enum
{
  CDMA_TCI = CDMA_TC,            /* Traffic Channel Initialization */
  CDMA_WFO,                      /* Waiting For Order */
  CDMA_WFA,                      /* Waiting for Mobile Station Answer */
  CDMA_CONV,                     /* Conversation */
  CDMA_RELEASE                   /* Release */
} cdma_tc_state_type;

/* ----------------------------------
** Powerdown registration types
** ---------------------------------- */

typedef enum
{
  CDMA_NORMAL,
  CDMA_LPM,
  CDMA_NAM_CHANGE
} cdma_powerdown_reg_type;

/* ------------------------------
** IMSI types
** ------------------------------ */
typedef enum
{
  CDMA_IMSI_M,
  CDMA_IMSI_T
} cdma_imsi_type;

/* ----------------------------------
** Various Mobile Handoff types
** ---------------------------------- */
typedef enum
{
  CDMA_IDLE_HO,
  CDMA_AEHO,
  CDMA_AHO,
  CDMA_APHO
} cdma_ho_type;

#ifdef FEATURE_IS2000
// This new struct was introduced for FEATURE_IS2000
// All code channels are now replaced by this struct
// For bw compatbility the QOF mask Id can be set to 0
typedef struct
{
   word code_chan;
   byte qof_mask_id;
} cdma_walsh_fn_type;
#endif /* FEATURE_IS2000 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Enumerations for event report                                           */

/* Call processing states */
typedef enum
{
  CDMA_SYS_DET_S,    /* System determination state */ 
  CDMA_PIL_ACQ_S,    /* Pilot channel acquisition state */
  CDMA_SYNC_ACQ_S,   /* Sync channel acquisition state */
  CDMA_TIME_CHG_S,   /* Timing change state */
  CDMA_IDLE_S,       /* Idle state */
  CDMA_UPDATE_OVH_S, /* Update Overhead Information */
  CDMA_ORIG_S,       /* Mobile Station Origination Attempt */
  CDMA_PAGE_RES_S,   /* Page Response */
  CDMA_ORD_RES_S,    /* Mobile Station Order/Message Response */
  CDMA_REG_S,        /* Registration Access */
  CDMA_MSG_S,        /* Message transmission */
#ifdef FEATURE_IS95B
  CDMA_PACA_CAN_S,   /* PACA Cancel Substate */
#endif /* FEATURE_IS95B */
  CDMA_TCI_S,        /* Traffic Channel Initialization */
  CDMA_WFO_S,        /* Waiting For Order */
  CDMA_WFA_S,        /* Waiting for Mobile Station Answer */
  CDMA_CONV_S,       /* Conversation */
  CDMA_RELEASE_S,    /* Release */
  CDMA_MAX_S   
} mcc_event_cp_state_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Miscellaneous definintions                                              */

#ifndef FEATURE_SD20
#define MINUTE_TIME 60000U
   /* The number of milliseconds for a one minute interval.  Used for the
      IDLE timeout entry condition.  Allows for time intervals in multiples
      of one minute. */
#endif /* !FEATURE_SD20 */

#define PRI_PAGECH 1
  /* The primary paging channel (IS-95 section 7.1.3.4) */

#define CHANGE_PAGE_CHAN_WAIT 80
  /* Time (in ms) after searcher is told to change paging channels
     at which we can be sure the new mask is in effect */

#ifdef FEATURE_TMSI
#define INVALID_TMSI_CODE 0xFFFFFFFFUL
  /* Value used to indicate invalid TMSI code */
#endif /* FEATURE_TMSI */

/* Designated arbitrary default values for MCC and IMSI_11_12              */
#define IMSI_S1_ZERO 0xF9EBE7L
#define IMSI_S2_ZERO 999
#define IMSI_MCC_ZERO 999
#define IMSI_11_12_ZERO 99

#define IMSI_CLASS_0_ADDR_NUM 0xFF
#define IMSI_CLASS_1_ADDR_NUM_MAX 7

#define CLASS_1_IMSI(a)  \
((((byte) (a)) <= IMSI_CLASS_1_ADDR_NUM_MAX) ? TRUE : FALSE)
  /* cdma.imsi_addr_num (type byte) should be passed as "a". */

#define MCC_VALID_OVHD_TIMEOUT 30000
  /* Maximum time to obtain updated overhead messages arriving
     on the Paging Channel while operating in CDMA_IDLE_INIT state */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Constants for origination message translation                           */

#define MCC_DIAL_0    0xA
#define MCC_DIAL_STAR 0xB
#define MCC_DIAL_PND  0xC
#define MCC_DIAL_NULL 0x0

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                     BAND CLASS MASKS                                    */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#define BIT_MASK( val ) (1 << val) /* Macro to convert to maskable value   */
                                   /* Only tested for values 0-7           */
#define BAND_CELL_MASK (1 << CAI_BAND_CELL) /* 800 MHz cellular band */
#define BAND_PCS_MASK  (1 << CAI_BAND_PCS)  /* 1.8 to 2.0 GHz band for PCS */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                   OPERATING MODE MASKS                                  */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#define J_STD_008_OP_MODE_MASK (1 << CAI_J_STD_008_OP_MODE) /* ANSI JSTD008 */
#define IS95_CDMA_OP_MODE_MASK (1 << CAI_IS95_CDMA_OP_MODE) /* IS-95 CDMA   */
#define IS95_ANAL_OP_MODE_MASK (1 << CAI_IS95_ANAL_OP_MODE) /* IS-95 analog */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* Specify which Band Class(es) and Operating Mode(s) is (are) supported   */
/* depending on which features are being compiled in.                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
#if (defined( FEATURE_CDMA_800 ) && defined( FEATURE_CDMA_1900) && \
     defined( FEATURE_ACP ))
/* This target is a Tri-mode target. It supports Cellular band CDMA,
Cellular band AMPS and PCS band J-STD-008 */

  #define SUPPORTED_BANDS (BAND_CELL_MASK | BAND_PCS_MASK)
  #define SUPPORTED_CDMA_BANDS (BAND_CELL_MASK | BAND_PCS_MASK)
  #define SUPPORTED_OP_MODES (IS95_CDMA_OP_MODE_MASK | \
          J_STD_008_OP_MODE_MASK  | IS95_ANAL_OP_MODE_MASK)

#elif (defined( FEATURE_CDMA_800 ) && defined( FEATURE_CDMA_1900))
/* This target supports Cellular band CDMA, and PCS band J-STD-008 */

  #define SUPPORTED_BANDS (BAND_CELL_MASK | BAND_PCS_MASK)
  #define SUPPORTED_CDMA_BANDS (BAND_CELL_MASK | BAND_PCS_MASK)
  #define SUPPORTED_OP_MODES (IS95_CDMA_OP_MODE_MASK | \
          J_STD_008_OP_MODE_MASK)

/* -----------------------------------------------------------------
** For Pre-IS95B, cellular band supports only op mode 1 and PCS band
** supports only op mode 0.
** For IS-95B, both cellular and PCS band support op modes 0 and 1.
** ----------------------------------------------------------------- */

#elif (defined( FEATURE_CDMA_1900 ) && defined( FEATURE_ACP ))

  /* This target supports PCS band J-STD-008 and cellular band AMPS */
  #define SUPPORTED_BANDS (BAND_CELL_MASK | BAND_PCS_MASK)
  #define SUPPORTED_CDMA_BANDS BAND_PCS_MASK
  #ifdef FEATURE_IS95B
    #define SUPPORTED_OP_MODES \
      (J_STD_008_OP_MODE_MASK | IS95_CDMA_OP_MODE_MASK | IS95_ANAL_OP_MODE_MASK)
  #else
    #define SUPPORTED_OP_MODES (J_STD_008_OP_MODE_MASK | IS95_ANAL_OP_MODE_MASK)
  #endif /* FEATURE_IS95B */

#elif ( defined( FEATURE_CDMA_1900 ) )

  /* This target only supports PCS band J-STD-008 */
  #define SUPPORTED_BANDS BAND_PCS_MASK
  #define SUPPORTED_CDMA_BANDS BAND_PCS_MASK
  #ifdef FEATURE_IS95B
    #define SUPPORTED_OP_MODES (J_STD_008_OP_MODE_MASK | IS95_CDMA_OP_MODE_MASK)
  #else
    #define SUPPORTED_OP_MODES J_STD_008_OP_MODE_MASK
  #endif /* FEATURE_IS95B */

#elif defined( FEATURE_ACP )

  /* This target supports cellular band CDMA and AMPS */
  #define SUPPORTED_BANDS BAND_CELL_MASK
  #define SUPPORTED_CDMA_BANDS BAND_CELL_MASK
  #ifdef FEATURE_IS95B
    #define SUPPORTED_OP_MODES \
      (J_STD_008_OP_MODE_MASK | IS95_CDMA_OP_MODE_MASK | IS95_ANAL_OP_MODE_MASK)
  #else
    #define SUPPORTED_OP_MODES (IS95_CDMA_OP_MODE_MASK | IS95_ANAL_OP_MODE_MASK)
  #endif /* FEATURE_IS95B */

#else

  /* This configuration is for WLL, cellular band CDMA only */
  #define SUPPORTED_BANDS BAND_CELL_MASK
  #define SUPPORTED_CDMA_BANDS BAND_CELL_MASK
  #ifdef FEATURE_IS95B
    #define SUPPORTED_OP_MODES (J_STD_008_OP_MODE_MASK | IS95_CDMA_OP_MODE_MASK)
  #else
    #define SUPPORTED_OP_MODES IS95_CDMA_OP_MODE_MASK
  #endif /* FEATURE_IS95B */

#endif /* FEATURE_CDMA_1900 and/or FEATURE_ACP */

/* ----------------------------------------------------------------------- */
/* Macro to check if a CDMA band is supported                              */
#define IS_CDMA_BAND_SUPPORTED(band) \
          ((BIT_MASK(band) & SUPPORTED_CDMA_BANDS)? TRUE: FALSE)

/*-------------------------------------------------------------------------*/
/* Macros to check if the AMPS is supported for a given band class */
#define IS_AMPS_BAND_SUPPORTED(band) (band == 0)
#define IS_AMPS_MODE_SUPPORTED (SUPPORTED_OP_MODES & IS95_ANAL_OP_MODE_MASK)

/*---------------MACRO for FEATURE_IS2000-----------------------*/
#ifdef FEATURE_IS2000
#define RC_CLASS(rc)  ( (rc > CAI_RC_2)? 2:1)
#define RATE_SET_OF_FWD_RC(rc) \
       ( ((rc==CAI_RC_1)||(rc==CAI_RC_3)||(rc==4))? 1:2)
#define RATE_SET_OF_REV_RC(rc) \
       ( ((rc==CAI_RC_1)||(rc==CAI_RC_3))? 1:2)
#endif


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type which contains dialed digits                                       */

typedef struct
{
  byte digits[CM_MAX_DIALED_DIGITS];
    /* Dialed digits */
  byte num_digits;
    /* Number of digits in digits array */
  byte digits_sent;
    /* Indicator of how many digits were sent in the Origination Message
       or need to be sent during the Conversation state */
} cdma_dial_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CHANNEL ASSIGNMENT and EXTENDED CHANNEL ASSIGNMENT MESSAGE (see IS-95B
 * Section 7.7.2.3.2.8/21). CAM and ECAM share this internal data structure
 * so the processing of both messages is the same.  ECAM is basically a super
 * set of CAM, with assign_mode 100 and 101 of CAM mapped to assign_mode 00
 * and 01 of ECAM.*/

/* CAM assign_mode 000 and 100, ECAM assign_mode 00 and 100*/
typedef struct
{
  boolean freq_incl;     /* Frequency included indicator */
  byte default_config;   /* Default configuration */
  boolean bypass_alert_answer; /* Bypass indicator */
  byte num_pilots;       /* Number of pilot records following */
                         /* Please note that in IS95-B std, this field is the
                          * actual pilot number - 1, but here, num_pilot is
                          * the actual number of pilot to be compatible with
                          * CAM implementation */
  byte granted_mode;     /* Granted mode */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  byte band_class;       /* Band class */
  byte frame_offset;     /* Frame offset */
  byte encrypt_mode;     /* Message encryption mode */
    /* Active Set Pilot record information */
  #ifndef FEATURE_IS2000
  caii_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM0_MAX];
  #else
  //The following pilot rec is used becos it is a superset of all other pilot rec types
  caii_ext_ch_ind3_pilot_rec_type pilot_rec[CAI_EXT_CHNASN_AM4_MAX];
  cai_radio_config_type for_rc; //RC of the forward CH
  cai_radio_config_type rev_rc; // RC of the reverse CH
  cai_chind_type ch_ind;
  byte fpc_subchan_gain; //FPC subchan gain for FFPC
  byte rl_gain_adj; // Not used for the time being, phase 2 stuff 4-bit 2's compliment
  byte fpc_fch_init_setpt; // Init setpt for FCH for FFPC
  byte fpc_fch_fer; // Target FER for puter loop forward power control
  byte fpc_fch_min_setpt; // This and the following parameter given the setpt range
  byte fpc_fch_max_setpt;
  byte fpc_dcch_init_setpt; // Init setpt for DCCH  for FFPC
  byte fpc_dcch_fer; // Target FER for puter loop forward power control
  byte fpc_dcch_min_setpt; // This and the following parameter given the setpt range
  byte fpc_dcch_max_setpt;
  boolean fpc_pri_chan; // Set to "0" i.e FCH till DCCH is supported
  boolean rev_fch_gating_on; //Indicates if 8th rate gating is on or off */
  boolean rev_pwr_cntl_delay_incl; /* Is Rev pwr cntl delay incl */
  byte rev_pwr_cntl_delay; /* Rev pwr Cntl delay */
  #endif

} mccdma_chnasn_am0_type;

/* CAM assign_mode 001 and 101, ECAM assign_mode 01 */
typedef struct
{
  boolean respond;       /* Respond on new Access Channel indicator */
  boolean freq_incl;     /* Frequency included indicator */
  byte band_class;       /* Band class */
  word cdma_freq;        /* Frequency assignment (only present if
                            freq_incl field is set to 1) */
  word num_pilots;       /* Number of entries in pilot_pn array */
                         /* Please note that in IS95-B std, this field is the
                          * actual pilot number - 1, but here, num_pilot is
                          * the actual number of pilot to be compatible with
                          * CAM implementation */
  word pilot_pn[CAI_EXT_CHNASN_AM1_MAX]; /* Pilot PN sequence offset index */
} mccdma_chnasn_am1_type;

/* CAM assign_mode 010, ECAM assign_mode 10 */
typedef struct
{
  boolean respond;        /* Respond on analog control channel indicator */
  byte analog_sys;        /* System indicator */
  boolean use_analog_sys; /* Use analog system indicator */
  byte band_class;        /* Band class */
} mccdma_chnasn_am2_type;

/* CAM assign_mode 011, ECAM assign_mode 11 */
typedef struct
{
  word sid;              /* System identification of the analog system */
  byte vmac;             /* Voice mobile station attenuation code */
  word analog_chan;      /* Voice channel number */
  byte scc;              /* SAT color code */
  boolean mem;           /* Message encryption mode indiator */
  byte an_chan_type;     /* Analog voice channel type */
  byte band_class;       /* Band class */
} mccdma_chnasn_am3_type;

/* Union of Channel Assignment messages */
typedef union
{
  mccdma_chnasn_am0_type am0;
  mccdma_chnasn_am1_type am1;
  mccdma_chnasn_am2_type am2;
  mccdma_chnasn_am3_type am3;
} mccdma_ca_ord_type;

/* Channel Assignment message type */
typedef struct
{

  byte msg_type;     /* Message type */
  caii_pc_hdr_type hdr;  /* Paging Channel common header fields */
  byte assign_mode;      /* Assignment mode */
  mccdma_ca_ord_type mode; /* Union for different assign modes */
} mccdma_chnasn_type;

#ifdef FEATURE_IS2000
/* This data struct is used for temporarily storing forward pwr ctrl info
   till the action time is reached, so that it can be updated in
   the non negotiable SCR */
typedef struct
{
  byte trans_num;  /* Trans num of this cmd given to lower layers */
  byte pwr_cntl_step;  /* Pwr Control Step */
  boolean fpc_incl; /* Is FPC values included */
  cai_fpc_mode_type fpc_mode;  /* FPC mode */
  boolean fpc_pri_chan; /* FPC primary channel */
  boolean fpc_olpc_fch_incl; /* Is FCH outer loop params included */
  byte fpc_fch_fer;  /* FCH target FER */
  byte fpc_fch_min_setpt; /* FCH min setpt */
  byte fpc_fch_max_setpt;  /* FCH max setpt */
  boolean fpc_olpc_dcch_incl; /* Is DCCH outer loop parameters included */
  byte fpc_dcch_fer;   /* DCCH target FER */
  byte fpc_dcch_min_setpt; /* DCCH min setpt */
  byte fpc_dcch_max_setpt;  /* DCCH max set point */
} mccdma_pwr_ctrl_info_type;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type which contains Mobile Station specific variables                   */

/* ----------------------------------------------
** Many of these fields are to store CDMA Numeric
** Information defined in IS-95A section 1.1.2.2
** ---------------------------------------------- */
typedef struct
{
  word curr_state;     /* Current state of CDMA state machine */
  byte curr_nam;       /* Current NAM */
  boolean serv_neg;    /* TRUE  => service negotiation  */
                       /*  FALSE => service option negotiation */
  word so_req;         /* Service Option of outstanding request */
  word so_cur_pri;     /* Current Primary service option */
  word so_cur_sec;     /* Current Secondary sevice option */
  dword esn;           /* Electronic serial number */
  word cdmach;         /* Current CDMA Channel number */
  mcc_entry_type *entry_ptr;
    /* Pointer to mode entry information received from main MC */

    /* Following are IMSI_O items */
  dword imsi_s1;       /* First part of Operational IMSI (IMSI_O_S1) */
  word imsi_s2;        /* Second part of Operational IMSI (IMSI_O_S2) */
  qword imsi_s;        /* IMSI_S1 and IMSI_S2 converted to a qword imsi_s */
  byte imsi_11_12;     /* Mobile Network Code (MNC) for IMSI_O address */
  word mcc;            /* Mobile Country Code (MCC) for IMSI_O address */
  byte imsi_addr_num;  /* Number of digits in NMSI - 4, or class 0 IMSI flag */

#if defined(FEATURE_IS95B_TRUE_IMSI) \
    || (defined(FEATURE_OTASP) && !defined(FEATURE_NSOTASP))
    /* Following are IMSI_M items.  */
  dword imsi_m_s1;       /* First part of MIN based IMSI (IMSI_M_S1) */
  word imsi_m_s2;        /* Second part of MIN based IMSI (IMSI_M_S2) */
  qword imsi_m_s;        /* IMSI_M_S1 and IMSI_M_S2 converted to a qword */
  byte imsi_m_11_12;     /* Mobile Network Code (MNC) for IMSI_M address */
  word imsi_m_mcc;       /* Mobile Country Code (MCC) for IMSI_M address */
  byte imsi_m_addr_num;  /* Number of digits in NMSI - 4,or class 0 IMSI flag*/

    /* Following are IMSI_T items.  */
  dword imsi_t_s1;       /* First part of True IMSI (IMSI_T_S1) */
  word imsi_t_s2;        /* Second part of True IMSI (IMSI_T_S2) */
  qword imsi_t_s;        /* IMSI_T_S1 and IMSI_T_S2 converted to a qword */
  byte imsi_t_11_12;     /* Mobile Network Code (MNC) for IMSI_T address */
  word imsi_t_mcc;       /* Mobile Country Code (MCC) for IMSI_T address */
  byte imsi_t_addr_num;  /* Number of digits in NMSI - 4,or class 0 IMSI flag*/

  cdma_imsi_type imsi_type;   /* indicator of the type of IMSI_O */
#endif /* FEATURE_IS95B_TRUE_IMSI || FEATURE_OTASP && !FEATURE_NSOTASP */

  boolean mob_term_home;
    /* Indicator of whether phone is configured to receive calls when using
       a home SID,NID pair */
  boolean mob_term_for_sid;
    /* Indicator of whether phone is configured to receive calls as a
       foreign SID roamer */
  boolean mob_term_for_nid;
    /* Indicator of whether phone is configured to receive calls as a
       foreign NID roamer */
  byte accolc;         /* Access overload class */
#ifdef FEATURE_SSPR_ENHANCEMENTS
  nv_sid_nid_pair_type pair[NV_MAX_HOME_SID_NID]; /* SID,NID pairs for MIN */
#else
  nv_sid_nid_pair_type pair[NV_MAX_SID_NID]; /* SID,NID pairs for MIN */
#endif
  dword imsi_hash_key;                       /* MIN based hash key */
#ifndef FEATURE_SD20
  word sid_lock[NV_MAX_SID_LOCK];            /* SIDs NOT to acquire */
#endif /* !FEATURE_SD20 */
  byte mob_cai_rev;   /* Protocol revision number of the mobile station */
  byte mob_model;     /* Model number of the mobile station */
  word mob_firm_rev;  /* Firmware revision number for mobile station */
  byte scm;           /* Station class mark */
#ifdef FEATURE_IS2000
  word code_chan;
#else
  byte code_chan;
#endif /* FEATURE_IS2000 */
    /* Code channel for channel assignment or CDMA-to-CDMA handoff */
  byte pagech;        /* Current CDMA Paging Channel number */
  byte page_chan_s;   /* Number of Paging Channels on current CDMA channel */
  byte slot_cycle_index_p;
    /* SLOT_CYCLE_INDEXp - the slot cycle index prefered by user */
  mccdma_chnasn_type chnasn;
    /* Channel assignment message received during the System Access state
     * and retained for use in TC state*/
  boolean call_orig;
    /* Indicator of whether the call in progress is mobile originated (TRUE)
       or mobile terminated (FALSE) */
  boolean rel_orig;
    /* Indicator of whether the user initiated the call release (TRUE)
       or the release was initiated by the base station (FALSE) */
#ifdef FEATURE_DS
  boolean end_by_data;
    /* Indicator of whether Data Services ended the call (TRUE)
       or some other entity (FALSE) */
#endif
  byte reg_type;
    /* Type of registration required -- this field must be set when the
       next state is CDMA_REG - the codes are defined in the IS-95 spec */
  cdma_powerdown_reg_type powerdown_reg_type;
    /* type of powerdown registration required- whether from powerdown
       or from lpm */
  word sa_state;
    /* Substate within System Access state which should be entered
       after Update Overhead Information substate */
  cdma_ho_type ho_state;
    /* Keeps track of various Handoff states */
#ifdef FEATURE_IS95B
  word ho_sa_state;
    /* This holds the current sub-state of the Access State during
       Access Handoffs */
#endif
  byte sa_resp_msg;
    /* Message to be sent, during order/message response substate, in
       response to a received message */
  caii_ac_ord_type sa_resp_ord;
    /* Specific order to transmit when the sa_resp_msg is an Order Message */
  boolean digit_mode; /* Identifies between DTMF and ASCII call type */
  byte number_type;   /* Number type for an ASCII mode */
  byte number_plan;   /* Number plan for an ASCII mode */
  cdma_dial_type dial; /* Structure containing digits dialed by user */
  byte remaining_digits; /* Number of digits that didn't fit in origination */
  mc_msg_type offline_cmd; /* Command msg to MCCDMA for offline processing */
  byte frame_offset;
    /* Current Traffic Channel frame offset, in units of 1.25 msec */
  byte hdm_seq;
    /* Handoff Direction Message sequence number */
#ifndef FEATURE_SD20
  boolean redirection_enabled;
    /* Service Redirection indicator - Set to enabled to indicate that
       service redirection is currently in effect; otherwise set to
       disabled */
  word expected_sid;
    /* Expected SID - If the base station is redirecting the mobile station
       to a specific system, this field shall be set to the SID of that
       system; otherwise the field is set to zero. */
  word expected_nid;
    /* Expected NID - If the base station is redirecting the mobile station
       to a specific network, this field shall be set to the NID of that
       network; otherwise the field is set to 65535. */
#endif /* !FEATURE_SD20 */
  boolean vp_desired; /* Incidates if the user wants voice privacy.  This
       is the value of the pm bit in a page response or origination */
  byte encrypt_mode; /* Indicates if message encryption is on */
  byte auth_mode;    /* Indicates if authentication data sent on access */
  byte band_class;  /* CDMA band class (cellular or PCS) */
  nv_block_or_sys_enum_type block_or_sys; /* Cellular serv-sys (A or B) or
                                             PCS frequency block (A-F)   */
#ifdef FEATURE_AUTH
  dword rand;
    /* Value of RAND used in last computation of AUTHR for registrations */
  boolean authr_valid; /* Signifies whether the authr value can be used */
  dword authr;  /* Value of AUTHR used is registration messages */
  dword randu;  /* Random value received in Auth Challenge Order */
#endif /* FEATURE_AUTH */
#ifdef TMOBILE
#error code not present
#endif /* TMOBILE */
#ifdef FEATURE_TMSI
  caii_tmsi_data_type assigning_tmsi;
  byte tmsi_code_len;
  boolean tmsi_full_timer_set;
#endif /* FEATURE_TMSI */
#ifdef FEATURE_OTASP
  otaspi_mob_dir_data_type mob_dir; /* Mobile Directory Number */
  byte spc[NV_SEC_CODE_SIZE];       /* Service Programming Code */
  otasp_call_type otasp_call; /* Indicates service provisioning type */
#ifdef FEATURE_RUIM
    /* OTA capability features supported by the card */
  ruim_otasp_features_type ruim_ota_features; 
#endif /* FEATURE_RUIM */
#ifdef FEATURE_OTASP_OTAPA
  boolean otapa_enabled; /* Indicate if OTAPA is enabled for current NAM */
  boolean nam_lock;   /* Indicates if the current NAM is protected by SPASM */
#endif /* FEATURE_OTASP_OTAPA */
#endif /* FEATURE_OTASP */
  dword auth_data;   /* auth data for a mobile originated call */
  boolean spc_change_enabled;  /* indicates if user has enabled the spc change */
  byte p_rev_in_use;    /* P_REV_IN_USE */
#ifdef FEATURE_IS95B
  byte pwr_cntl_step;   /* Closed loop power control step size */
#endif
#ifdef FEATURE_IS2000
  byte fpc_subchan_gain;  /* FPC subchan gain */
  byte end_reason;    /* reason for ending a call */
  word t_slotted;         /* Slotted timer value */
  boolean  rev_fch_gating_on; /* Rev FCH 8th-rate gating mode on indicator */
  byte  rev_pwr_cntl_delay; /* Rev pwr ctrl delay */
  boolean drs;                /* DRS bit in Origination message */
#endif
#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
  byte                mo_dbm_type;   /* mobile orig'ed data burst msg type */
  void               *dbm_buf_ptr;   /* pointer to the MO DBM data */
  word                dbm_wait_for_l2_ack;
          /* Indicates if MC is waiting for layer 2 ack for an MO DBM request */
          /* It is set to the value according to the masks defined below.     */
          /* In TC, this is set if mc is waiting for L2 ACK. MC can possibly  */
          /* waiting for both SMS and PD layer 2 ack.                         */
          /* In INIT, IDLE, SA, it is set when the SMS request is received.   */
          /* MC can only wait for either SMS or PD layer 2 ack, not both.     */
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */
} cdma_ms_type;

#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
/* Masks to identify if MC is waiting for a layer 2 ack for an SMS or
 * PD request */
#define MCC_NOT_WAIT_FOR_L2_ACK         0x0000
#define MCC_SMS_WAIT_FOR_L2_ACK_MASK    0x0001
#define MCC_PD_WAIT_FOR_L2_ACK_MASK     0x0002
#endif /* FEATURE_UASMS || FEATURE_GPSONE */

#ifdef FEATURE_SKT_NDSS
extern word	so_req_save;
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type which contains message sequence numbers and message receipt flags  */

typedef struct
{
  qword msg_time;
    /* Time of receipt of most recently received message for this base
       station */
  boolean sy_rx;
    /* indicator of whether valid Sync channel information is in the
       sy structure */
  boolean sp_rx;
    /* System Parameter Message received */
  boolean nl_rx;
    /* Neighbor List Message received */
  boolean cl_rx;
    /* CDMA Channel List Message received */
#ifdef FEATURE_IS95B
  boolean gnl_rx;
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000
    /* Extended CDMA Channel List Message Received */
  boolean ecl_rx;

    /* Extended Global Service Redirection Message Received */
  boolean egsd_rx;
#endif
  boolean ap_rx;
    /* Access Parameter Message received */
  boolean esp_rx;
    /* Extended System Parameters Message received */
  boolean gsd_rx;
    /* Global Service Redirection Message received */
  byte config_msg_seq;
    /* Configuration Message sequence number which applies to the received
       System Parameter, Neighbor List, CDMA Channel List, Extended System
       Parameter and Global Service Redirection messages */
  byte acc_msg_seq;
    /* Access Parameter sequence number which applies to the received
       Access Parameter message */
} cdma_msg_rx_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Enum used to indicate what message type to retrieve from BS Info array  */

typedef enum
{
  MC_SYNC_MSG = 0x10,
       /* a Sync Channel message type */
  MC_RX_MSG
    /* Message receipt structure */
} cdma_mc_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   Types to store Neighbor Set and Active Set                            */

typedef struct
{
  word pilot_pn;  /* Neighbor pilot PNs */
  byte age;       /* AGE of neighbor */
  byte config;    /* Neighbor configuration */
  byte search_pri;     /* Pilot Channel search priority */
  boolean freq_incl;   /* Frequency included indicator */
  byte band;           /* Neighbor band class */
  word freq;           /* Neighbor frequency assignment */
#ifdef FEATURE_IS95B
  boolean access_entry_ho;    /* If Access Entry HO allowed */
  boolean access_ho_allowed;  /* If ACCESS HO and Access Probe allowed */
  byte  srch_win_nghbr;       /* Neighbor window for the pilot */
  boolean timing_incl;        /* If Timing included for the Neighbor */
  byte nghbr_tx_offset;       /* Neighbor TX offset */
  byte nghbr_tx_duration;     /* Neighbor TX Duration */
  byte nghbr_tx_period;       /* Neighbor TX PERIOD */
#endif /* FEATURE_IS95B */
  #ifdef FEATURE_IS2000
  boolean add_pilot_rec_incl;
  caii_add_pilot_rec_type nghbr_pilot_rec;
  byte srch_offset_nghbr;
#endif
} cdma_nghbr_set_type;

typedef struct
{
  byte pilot_inc;       /* Current pilot increment between neighbors */
  byte nghbr_max_age;   /* Maximum AGE for neighbors */
  word num_nghbr_pns;   /* Number of PNs in the Neighbor Set */
  cdma_nghbr_set_type nset[SRCH_NSET_MAX];   /* Neighbor set */
} cdma_nghbr_type;

#ifdef FEATURE_IS95B
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
** Type which contains previous base station's registration and
** Authorization information used during the Access Handoffs
**------------------------------------------------------------------------*/
typedef struct
{
  caii_sysparm_type sp;
  /* Variable to store Auth field temporarily */
  byte  auth;
  dword rand;
} cdma_prev_bs_info;
#endif /* FEATURE_IS95B */

typedef struct
{
  word num_active_pns;           /* Number of PNs in the Active Set */
  word pilot_pn[SRCH_ASET_MAX];  /* Active Set pilot PNs */
} cdma_active_set_type;

#ifdef FEATURE_IS2000_QPCH
typedef struct
{
  uint8   band_class;
  uint8   pch;
  uint16  cdma_channel;
  uint16  pn;
} mccdma_pch_id_type;
#endif

#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type which contains base station specific information                   */

typedef struct
{
  boolean valid_pn;
    /* Indicator of whether pilot_pn is valid
       (i.e. whether entry has been written to or not) */
  word pilot_pn;
    /* pilot pn offset */
  cdma_msg_rx_type rx;
    /* structure containing information about which messages have been
       received */
  caii_sync_type sy;
    /* Most recently received Sync Channel message */
  caii_sysparm_type sp;
    /* Most recently received System Parameter Message */
  caii_accparm_type ap;
    /* Most recently received Access Parameter Message */
  cdma_nghbr_type nl;

#ifdef FEATURE_IS95B
  byte gnl_config_msg_seq;
    /* Config msg sequence of General Neighbor List Message */
  caii_analog_nghbr_type anl;
    /* Analog Neighbor List as received part of GNLM */
  boolean setting_search_win;
    /* flag to indicate if the GNLM is received and SRCH WIN needs be be
       defaulted: A case where GNLM arrives before the SPM */
  byte curr_acc_msg_seq;
    /* contains the previous base station's info during Access handoffs */
#endif /* FEATURE_IS95B */
#ifdef FEATURE_IS2000

  /* Extended Global Service Redirection Message */
  caii_ext_glb_redir_type egsd;

  /* Extended Channel List Message */
  caii_ext_chnlist_type  ecl;

#endif
    /* Most recently received Neighbor List Message */
  caii_ext_sysparm_type esp;
    /* Most recently received Extended System Parameters Message */
  caii_glob_redirect_type gsd;
    /* Most recently received Global Service Redirection Message */
  byte nl_config_msg_seq;
    /* Config Seq num of most recently received Neighbor List Message */
  caii_chnlist_type cl;
    /* Most recently received Channel List Message */
  word cdma_ch;
    /* CDMA channel base station is using */
  byte page_ch;
    /* Paging channel mobile has been assigned to on this base station. The
       base station can have different parameters for different channels */
  byte band_class;
    /* Band class of the base station */
  #ifdef FEATURE_IS2000_QPCH
  mccqpch_channel_type qpch_num;
    /* Quick paging channel number.  The number is generated when needed so that
       we don't need to determine if the right messages have arrived.  When this
       information is needed, i.e. entering sleeping, we are sure that all messages
       are current. */
  dword imsi_hash_key;
  #endif
}cdma_bs_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type which contains fields of a matched order (unbundled)               */

typedef struct
{
  byte msg_type;            /* Message type */
  caii_pc_hdr_type hdr;     /* Paging Channel common header fields */
  byte order;               /* Order code */
  byte ordq;                /* Order qualification code */
  dword authbs;             /* For BS Challenge Confirmation Order only */
#ifdef FEATURE_IS95B
  byte roam_indi;           /* Enhanced Roaming Indication for CAI_REG_ACC */
#endif
#ifdef FEATURE_IS2000
  byte retry_type;          /* Retry type for CAI_RETRY_ORDER */
  byte retry_delay;         /* Retry delay for CAI_RETRY_ORDER */
#endif /* FEATURE_IS2000 */
} mccdma_pc_ord_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Union for received non-duplicate message addressed to the mobile        */

/* ------------------------------------------------------------------
** Note - The msg_type and hdr fields of all structures in this union
** can be accessed through the caii_gen_pc_type.  This union and the
** supporting order and channel assignment types allow processing of
** the layer 2 fields of a directed paging channel message to be done
** without reguard to the message type.
** ------------------------------------------------------------------ */

typedef union
{
  caii_gen_pc_type            gen_pc;
  mccdma_pc_ord_type          pc_ord; /* Defined above */
  mccdma_chnasn_type          chnasn; /* Defined above */
#ifdef FEATURE_TMSI
  caii_pc_tmsi_asgn_type      pc_tmsi;
#endif /* FEATURE_TMSI */
  caii_pc_burst_type          pc_burst;
  caii_pc_auth_ch_type        pc_auth;
  caii_pc_ssd_type            pc_ssd;
  caii_pc_feature_type        pc_feat;
  caii_serv_redirect_type     serv_redir;
  caii_pc_streq_type          pc_streq;
#ifdef FEATURE_IS95B
  caii_paca_type              paca;
#endif /* FEATURE_IS95B */
} mccdma_matched_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type containing fields for layer 2 processing of paging channel msgs    */

typedef struct
{
  byte msg_seq;
  byte ack_seq;
  byte ack_type;
  byte valid_ack;
  boolean idle_msg_pending;
  mccdma_matched_msg_type pending_idle_msg;
} mcc_layer2_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type and queues for DTMF command procesing                              */

typedef struct
{
  q_link_type link;        /* Queue link */
  mc_name_type dtmf_cmd;   /* Type of DTMF command received from UI */
  union
  {
    word ms;      /* Number of ms key depressed (MC_STOP_CONT_DTMF) */
    byte digit;               /* Digit to send (MC_START_CONT_DTMF) */
    mc_burst_dtmf_type burst; /* Burst DTMF data (MC_BURST_DTMF_F)  */
  } parm;
} cdma_dtmf_type;

extern q_type mcc_dtmf_q;
  /* queue to place outstanding dtmf commands */
extern q_type mcc_dtmf_free_q;
  /* queue to place free buffers to be placed on the mcc_dtmf_free_q */

#ifdef FEATURE_IS95B_TRUE_IMSI
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type for IMSI_O processing                                                */

typedef struct
{
  dword imsi_o_s1;       /* First part of Mobile Identification Number (MIN) */
  word  imsi_o_s2;       /* Second part of Mobile Identification Number (MIN)*/
  qword imsi_o_s;        /* min1 and min2 converted to a qword imsi_s */
  byte  imsi_o_11_12;    /* Moblie Network Code (MNC) for IMSI address */
  word  mcc_o;           /* Mobile Country Code (MCC) for IMSI address */
  byte  imsi_o_addr_num; /* Number of digits in NMSI-4, or class 0 IMSI flag */
} cdma_imsi_o_type;
#endif /* FEATURE_IS95B_TRUE_IMSI */

#if defined (FEATURE_IS95B_PILOT_RPT) || defined (FEATURE_SPECIAL_MDR)

/* Default value for prev_pilot                                            */
#define NO_PREV_PILOT -1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Type for Pilot Report Processing                                        */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Structure to hold pilot reporting overhead information */

typedef struct
{
  word msg_len;
    /* Length of Access Channel message excluding pilot report information */
  byte num_pilots_allowed;
    /* Number of pilots that can be reported in Access Channel Msg */
  int2 first_pilot;
    /* Active pilot of first access probe */
  int2 prev_pilot;
    /* Active pilot for the immediately previous access probe */
  byte num_pilots_attempted;
    /* Number of pilots that has been access attempted */
  boolean first_probe;
    /* Flag if first access probe */
  boolean apho_rebuild;
    /* Flag if ACCESS_HO_LIST need to build because of access probe handoff */
  cdma_nghbr_set_type attempt_list[CAI_N13M];
    /* List of pilots that have been access attempted */
} plt_rpt_ovhd_rec_type;

/* Structure to hold Access Handoff List */

typedef struct
{
  int2 pilot_pn;
    /* ---------------------------------------------------------------------
    ** Pilot PN is used as a key to perform the record matching of Access
    ** Handoff List against the Searcher Pilot Strength Measurement report
    ** during update
    ** ---------------------------------------------------------------------*/
  int2    pilot_pn_phase;   /* Pilot pn phase */
  byte    pilot_strength;   /* Pilot strength */
  boolean access_attempted; /* Whether access is attempted */
} aho_pilot_info_type ;

typedef struct
{
  byte                num_pilots; /* number of pilots in the list */
  aho_pilot_info_type pilot_list[CAI_N13M];
} aho_list_type;

/* Structure to hold Other Reported List */

typedef struct
{
  int2    pilot_pn;
    /* -----------------------------------------------------------------
    ** Pilot PN is used as a key to perform the record matching of Other
    ** Reported List against the SRCH NSet during swapping
    ** ----------------------------------------------------------------- */
  int2    pilot_pn_phase;   /* Pilot pn phase */
  byte    pilot_strength;   /* Pilot strength */
} othrpt_pilot_info_type ;

typedef struct
{
 byte                   num_pilots; /* number of pilots in the list */
 othrpt_pilot_info_type pilot_list[CAI_MAX_ADD_PILOTS];
} othrpt_list_type;

#endif /* FEATURE_IS95B_PILOT_RPT || FEATURE_SPECIAL_MDR */

#ifdef FEATURE_IS95B_PILOT_RPT
extern boolean mccsa_access_attempt_in_progress(void);
  /* Check whether access attempt is in progress */
extern void mccsa_set_prev_pilot_info(cdma_bs_type *);
  /* Set Previous pilot PN and its neigbor record in Access Attempt List */
extern void mccsa_update_access_attempt_list(void);
  /* Update Access Attempt list with info from neighbor list */
extern void mccsa_update_srch_neighs(void);
  /* Update neighbor set which will be sent to Searcher task with info from
     Access Attempt list */
#endif /* FEATURE_IS95B_PILOT_RPT */

#ifdef FEATURE_RUIM
  /* Buffer used to retrieve the result of R-UIM operations */
extern byte ruim_access_buffer[RUIM_MAX_CHARS];
#endif /* FEATURE_RUIM */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                    MACROS                                               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Macro to determine if additional overhead messages are required before
   a System Access can be attempted */

#define INVALID_SEQ 0xff
#define TEMP_NL_SEQ 0x7f
  /* CONFIG_MSG_SEQ is 6 bits, so anything over 0x3f is invalid */


/* -------------------------------------------------------------------------
** OVHD_MSG_OK macro checks if the Mobile received all the Overhead messages
** These messages are based on if MS is IS2000, IS95B or IS95A compatible.
** -----------------------------------------------------------------------*/

#ifdef FEATURE_IS2000

#ifdef FEATURE_SKT_CAI
#define OVHD_MSG_OK() \
    ( ( (P_REV_IS_6_OR_GREATER) && \
	    (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ecl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.cl_rx  && \
		cur_bs_ptr->rx.ecl_rx  && \
		cur_bs_ptr->rx.ap_rx  && \
		cur_bs_ptr->rx.esp_rx && \
		cur_bs_ptr->rx.gnl_rx && \
		cur_bs_ptr->rx.gsd_rx && \
		cur_bs_ptr->rx.egsd_rx ) || \
      ( (!(P_REV_IS_6_OR_GREATER)) && \
        (P_REV_IS_4_OR_GREATER) && \
	    (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.cl_rx  && \
		cur_bs_ptr->rx.ap_rx  && \
		cur_bs_ptr->rx.esp_rx && \
		cur_bs_ptr->rx.gnl_rx && \
		cur_bs_ptr->rx.gsd_rx ) || \
      ( (!(P_REV_IS_4_OR_GREATER)) && \
	    (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.cl_rx  && \
		cur_bs_ptr->rx.ap_rx  && \
		cur_bs_ptr->rx.esp_rx && \
		cur_bs_ptr->rx.gsd_rx ) )
#else /* FEATURE_SKT_CAI */
#define OVHD_MSG_OK() \
  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->ecl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
    cur_bs_ptr->rx.sp_rx  && \
    cur_bs_ptr->rx.nl_rx  && \
    cur_bs_ptr->rx.cl_rx  && \
    cur_bs_ptr->rx.ecl_rx  && \
    cur_bs_ptr->rx.ap_rx  && \
    cur_bs_ptr->rx.esp_rx && \
    cur_bs_ptr->rx.gnl_rx && \
    cur_bs_ptr->rx.gsd_rx && \
    cur_bs_ptr->rx.egsd_rx )
#endif /* FEATURE_SKT_CAI */

#elif defined(FEATURE_IS95B)
#define OVHD_MSG_OK() \
  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
    cur_bs_ptr->rx.sp_rx  && \
    cur_bs_ptr->rx.nl_rx  && \
    cur_bs_ptr->rx.cl_rx  && \
    cur_bs_ptr->rx.ap_rx  && \
    cur_bs_ptr->rx.esp_rx && \
    cur_bs_ptr->rx.gnl_rx && \
    cur_bs_ptr->rx.gsd_rx )


#else
#define OVHD_MSG_OK() \
  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) && \
    cur_bs_ptr->rx.sp_rx  && \
    cur_bs_ptr->rx.nl_rx  && \
    cur_bs_ptr->rx.cl_rx  && \
    cur_bs_ptr->rx.ap_rx  && \
    cur_bs_ptr->rx.esp_rx && \
    cur_bs_ptr->rx.gsd_rx )
#endif /* FEATURE_IS95B */

/* -------------------------------------------------------------------------
** CLR_OVHD_MSG sets all the OVHD messages as not received.  This forces the
** Mobile to re-process all the OVHD messages. CLR_OVHD_PREV_MISMATCH macro
** sets all the OVHD messages as not received if the P_REV is changed due
** to an Idle Handoff.
** -----------------------------------------------------------------------*/

#ifdef FEATURE_IS2000

#define CLR_OVHD_MSGS() \
    cur_bs_ptr->rx.config_msg_seq = INVALID_SEQ;\
    cur_bs_ptr->rx.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.cl_rx  = FALSE; \
    cur_bs_ptr->rx.ecl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.esp_rx = FALSE; \
    cur_bs_ptr->rx.gnl_rx = FALSE; \
    cur_bs_ptr->rx.egsd_rx = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE

#define CLR_OVHD_PREV_MISMATCH() \
    cur_bs_ptr->sp.config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->nl_config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->gnl_config_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->gsd.config_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->curr_acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.gnl_rx = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE; \
    cur_bs_ptr->rx.egsd_rx = FALSE;

#elif defined(FEATURE_IS95B) /* end of FEATURE_IS2000 */

#define CLR_OVHD_MSGS() \
    cur_bs_ptr->rx.config_msg_seq = INVALID_SEQ;\
    cur_bs_ptr->rx.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.cl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.esp_rx = FALSE; \
    cur_bs_ptr->rx.gnl_rx = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE

#define CLR_OVHD_PREV_MISMATCH() \
    cur_bs_ptr->sp.config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->nl_config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->gnl_config_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->gsd.config_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->curr_acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.gnl_rx = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE;

#else /* For IS95A mobile */

#define CLR_OVHD_MSGS() \
    cur_bs_ptr->rx.config_msg_seq = INVALID_SEQ;\
    cur_bs_ptr->rx.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.cl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.esp_rx = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE

#define CLR_OVHD_PREV_MISMATCH() \
    cur_bs_ptr->sp.config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->nl_config_msg_seq  = INVALID_SEQ; \
    cur_bs_ptr->gsd.config_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ; \
    cur_bs_ptr->rx.sp_rx  = FALSE; \
    cur_bs_ptr->rx.nl_rx  = FALSE; \
    cur_bs_ptr->rx.ap_rx  = FALSE; \
    cur_bs_ptr->rx.gsd_rx = FALSE;

#endif /* FEATURE_IS2000 */

/* -------------------------------------------------------------------------
** AHO_OVHD_MSG_OK checks if the set of OVHD messages required during Access
** Handoff are received by the Mobile.
**
** AEHO_OVHD_MSG_OK macro checks if the set of OVHD message required during
** Access Entry Handoff are received.
** -----------------------------------------------------------------------*/
#if defined(FEATURE_IS95B)

#ifdef FEATURE_SKT_CAI
#define AHO_OVHD_MSG_OK() \
  (	P_REV_IS_4_OR_GREATER ? \
	  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ap.acc_msg_seq != INVALID_SEQ) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.ap_rx  && \
		cur_bs_ptr->rx.esp_rx && \
		cur_bs_ptr->rx.gnl_rx ) : \
	  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->ap.acc_msg_seq != INVALID_SEQ) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.ap_rx  && \
		cur_bs_ptr->rx.esp_rx ) )

#define AEHO_OVHD_MSG_OK() \
  (	P_REV_IS_4_OR_GREATER ? \
	  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.cl_rx  && \
		cur_bs_ptr->rx.esp_rx && \
		cur_bs_ptr->rx.gnl_rx  ) : \
	  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
		(cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
		cur_bs_ptr->rx.sp_rx  && \
		cur_bs_ptr->rx.nl_rx  && \
		cur_bs_ptr->rx.cl_rx  && \
		cur_bs_ptr->rx.esp_rx ) )

#else /* FEATURE_SKT_CAI */

#define AHO_OVHD_MSG_OK() \
  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->ap.acc_msg_seq != INVALID_SEQ) && \
    cur_bs_ptr->rx.sp_rx  && \
    cur_bs_ptr->rx.nl_rx  && \
    cur_bs_ptr->rx.ap_rx  && \
    cur_bs_ptr->rx.esp_rx && \
    cur_bs_ptr->rx.gnl_rx )

#define AEHO_OVHD_MSG_OK() \
  ( (cur_bs_ptr->sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) && \
    (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) && \
    cur_bs_ptr->rx.sp_rx  && \
    cur_bs_ptr->rx.nl_rx  && \
    cur_bs_ptr->rx.cl_rx  && \
    cur_bs_ptr->rx.esp_rx && \
    cur_bs_ptr->rx.gnl_rx  )

#endif /* FEATURE_SKT_CAI */

#endif

#define CLR_ACC_MSG_SEQ() \
    (cur_bs_ptr->rx.acc_msg_seq = INVALID_SEQ)

#ifdef FEATURE_IS95B
#define GNLM_MSG_NOT_RECVD \
    ( cur_bs_ptr->gnl_config_msg_seq != cur_bs_ptr->rx.config_msg_seq )
#define GNLM_MSG_IS_RECVD \
    ( cur_bs_ptr->gnl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq )
#endif /* FEATURE_IS95B */

#define ESP_MSG_NOT_RECVD \
    ( cur_bs_ptr->esp.config_msg_seq != cur_bs_ptr->rx.config_msg_seq )
#define SP_MSG_NOT_RECVD \
    ( cur_bs_ptr->sp.config_msg_seq != cur_bs_ptr->rx.config_msg_seq  )
#define SP_MSG_IS_RECVD \
    ( ( cur_bs_ptr->rx.sp_rx == TRUE) && ( cur_bs_ptr->sp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq  )  )
#define ESP_MSG_IS_RECVD \
    ( ( cur_bs_ptr->rx.esp_rx == TRUE) && (cur_bs_ptr->esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq )  )
#define NGHBR_LIST_MSGS_RECVD \
    (( cur_bs_ptr->nl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq ) && \
    ( cur_bs_ptr->gnl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq ))

/* Macro to determine if a PN offset is valid */
#define VALID_PN( pn )  \
  ( cdma_nghbrs.pilot_inc ? ((pn % cdma_nghbrs.pilot_inc ) == 0 ) : FALSE )

#ifdef FEATURE_IS2000
/* Macro to determine if a code channel is valid */
#define VALID_CODE_CHN( chn)  \
  ( (chn > 0) && (chn < 128 ) ) //Range extended for FEATURE_IS2000
/* Macro to determine if a QOF mask id is valid */
#define VALID_QOF_MASK_ID( id)  \
  ( id < 4 ) //Range extended for FEATURE_IS2000
#else
/* Macro to determine if a code channel is valid */
#define VALID_CODE_CHN( chn)  \
  ( (chn > 0) && (chn < 64 ) )
#endif /* FEATURE_IS2000 */

/* Test for channel to be in set allocated for digital CDMA system */
#define ALLOCATED_CDMA_CHN( chan ) TRUE
/*
#define ALLOCATED_CDMA_CHN( chan )\
   ( ( ( chan > 0   ) && ( chan < 312 ) ) || \
     ( ( chan > 355 ) && ( chan < 645 ) ) || \
     ( ( chan > 688 ) && ( chan < 695 ) ) || \
     ( ( chan > 738 ) && ( chan < 778 ) ) || \
     ( ( chan > 1013) && ( chan < 1024 ) ) )
*/

/* Macro to determine next ACK_REQ_SEQ to be sent to the
   base station */
/*
#define NEXT_ACK( ack_req ) \
  ( ( ack_req + 1) % 3 )
*/

/* Macro to find main state from state variable      */

#define MAIN_STATE(state)  (state & 0xFF00 )

typedef enum
{
  SLOTTED_PAGE,                   /* Slotted Page Message */
  NON_SLOTTED_PAGE,               /* Page Message */
  GENERAL_PAGE                    /* General Page Message */
} mcc_page_enum_type;

typedef struct
{
  byte msg_seq;                   /* msg_seq of page addressed to mobile */
  boolean special_service;        /* whether or not the page included SO */
  word service_option;            /* Service Option (SO) included */
#ifdef FEATURE_TMSI
  dword tmsi_code;                /* TMSI code from general page record */
#endif /* FEATURE_TMSI */
} mcc_page_info_type;


#ifdef FEATURE_IS2000
/* Type to specify retry info */
typedef struct
{
  uint8   retry_type;           /* Origination, Resource Request or SCRM */
  boolean infinite_delay;       /* True if infinite delay */
  uint32  retry_delay_duration; /* Retry delay duration, in unit of ms */
} retry_delay_type;
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
/* <EJECT> */
/*===========================================================================

                      DATA DECLARATIONS

===========================================================================*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*   External declarations for structures declared in mc_cdma.c            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_DS       /* if Data Services are compiled in */
/* Regional definitions to support DS-originated voice calls               */
extern boolean mcc_dsvoice;   /* indicates Voice call originated by DS task */
#endif

/* All mobile station specific variables */
extern cdma_ms_type cdma;

#ifdef FEATURE_UASMS
extern uasms_cdma_sms_type    cdma_uasms_msg; /* data needed for sending sms */
#else
#ifdef FEATURE_SMS
/* Internal format of SMS message to be transmitted */
extern smsi_msg_type cdma_sms_int;

/* Bit-packed format of SMS message to be transmitted */
extern smsi_ext_msg_type cdma_sms_ext;
#endif /* FEATURE_SMS */
#endif /* FEATURE_UASMS */

/* Internal format of Status Response message to be transmitted
   on the Access Channel */
extern caii_ac_strsp_type cdma_strsp;

#define MC_MAX_BASES 10
  /* Maximum number of base stations for which overhead information will
     be stored */

/* Base Station specific variables */
extern cdma_bs_type bs_info[MC_MAX_BASES];

/* Pointer to Current Base Station in Base Station Information array */
extern cdma_bs_type *cur_bs_ptr;

/* Current Active Set */
extern cdma_active_set_type cdma_active_set;

/* Last state of CDMA state machine to process */
extern word cdma_last_state;

/* Structure containing exit information */
extern mcc_exit_type mcc_exit;

#ifdef FEATURE_IS95B
extern boolean is_access_ho_allowed( void );
extern boolean is_in_access_ho_list( word );
#endif /* FEATURE_IS95B */

#define MCC_MAX_ACK_TYPES 2
#define MCC_MAX_MSG_SEQ 8

#ifdef FEATURE_IS95B
  #define MCC_MAX_ADDR_TYPE 8
  /* IMSI_S, ESN, IMSI, TMSI, BROADCAST. There are 3 unused fields, that  */
  /* are in the array.                                                    */
  /* Refer to sections 6.6.2.1.2 and 7.7.2.3.1 in IS-95B for more info    */
#else
  #define MCC_MAX_ADDR_TYPE 1
  /* Not used for duplciate detection prior to IS-95B, keep as 2D array   */
#endif

/* Structure containing Paging Channel duplicate msg sequence time stamps */
/* for non-page messages.                                                 */
extern qword
  mcc_pc_frames[MCC_MAX_ACK_TYPES][MCC_MAX_MSG_SEQ][MCC_MAX_ADDR_TYPE];

/* Structure containing Paging Channel duplicate msg sequence time stamps */
/* for page records.                                                      */
extern qword
  mcc_pc_pg_frames[MCC_MAX_MSG_SEQ];

#ifndef FEATURE_SD20
/* Return to system determination after being in idle state for idle_timeout
   minutes. If idle_timeout = 0 no timeout is to performed */
#ifdef FEATURE_MC_TIMER_FIX
extern int mcc_idle_timeout;
#define MAX_DWORD_VAL 0xFFFFFFFFUL
#else
extern word mcc_idle_timeout;
#endif /* FEATURE_MC_TIMER_FIX */
#endif /* !FEATURE_SD20 */

/* Timers */
extern rex_timer_type  mcc_rpt_timer;
extern rex_timer_type mcc_state_timer;
extern rex_timer_type  mcc_action_timer;
#ifdef FEATURE_IS95B_MDR
/* MDR SCAM message start timer */
extern rex_timer_type  mcc_scam_start_timer;
#endif /* FEATURE_IS95B_MDR */
#if ( defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) )
extern rex_timer_type  mcc_mdr_duration_timer;
#endif /* ( defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) ) */
extern rex_timer_type  mcc_cmd_timer;
extern rex_timer_type  mcc_dtmf_timer;
extern rex_timer_type  mcc_so_ctl_timer;
extern rex_timer_type  mcc_srv_timer;
#ifdef FEATURE_TMSI
extern rex_timer_type  mcc_full_tmsi_timer;
extern rex_timer_type  mcc_tmsi_exp_timer;
#endif /* FEATURE_TMSI */

#ifdef FEATURE_IS2000
extern rex_timer_type  mcc_slotted_timer;  /* Slotted Timer */
#endif

extern nv_cmd_type mcc_nv_buf;      /* Buffer for command to NV task */
extern nv_item_type mcc_nv_data;    /* Buffer for data from NV task */
extern txc_cmd_type mcc_tx_buf;     /* Buffer for command to Transmit task */
extern rxc_cmd_msg_type mcc_rx_buf; /* Buffer for command to Receive task */
extern rxtx_cmd_type mcc_rxtx_buf;  /* Buffer for command to Layer 2 task */
extern srch_cmd_type mcc_srch_buf;  /* Buffer for command to Search task */
#ifdef FEATURE_DS
extern ds_cmd_type mcc_ds_buf;      /* Buffer for command to Data task */
#endif /* FEATURE_DS */


/* Debug handoff parameters */
typedef struct
{
  byte t_add;
  byte t_drop;
  byte t_tdrop;
  byte t_comp;
  byte win_a;
  byte win_n;
  byte win_r;
  byte nghbr_max_age;

#ifdef FEATURE_IS95B_SOFT_HO
  byte soft_slope;
  int1 add_intercept;
  int1 drop_intercept;
#endif /* FEATURE_IS95B_SOFT_HO */

}mcc_ho_cb_type;

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

extern mcc_ho_cb_type mcc_ho_debug;
extern mcc_ho_cb_type mcc_ho_real;

extern mcc_layer2_type mcc_layer2;

#ifdef FEATURE_EVRC_SO_MGMT
/* Structure containing Preferred Voice Service Option information         */
extern nv_pref_voice_so_type mcc_pref_voice_so;
#endif

#ifdef FEATURE_BROADCAST_SMS
/* ------------------------------------------------------------------------
   Broadcast SMS global definations, and variables
   ------------------------------------------------------------------------*/

#define MAX_BC_PER_PAGE_MSG  \
         ((CAI_PC_BODY_SIZE-sizeof(cai_page_fix_type)-12) \
         / (sizeof (cai_pc_3_sc_0_fix_type) + CAII_BC_ADDR_LEN*8))
/* All the done bits plus reserved and add_length bits is 12 */

#define MAX_BC_DUP_DETECT (MAX_BC_PER_PAGE_MSG * 4)

typedef enum
{
  NO_MATCH,
  PAGE_MATCH,
  BROADCAST_FOUND
} mccdma_page_match_results_type;

/* the record to hold BC_ADDRs and BURST_TYPEs, once BC pages pass the
 * filter, to schedule SRCH to receive BC Msgs. It's also needed again to
 * varify the BURST_TYPE and BC_ADDR when BC Msgs arrives. */
typedef struct
{
  byte bc_addr[CAII_BC_ADDR_LEN];
  byte burst_type;
  /* the slot to wakeup, it's used for dup detection */
  qword wakeup_slot;
} mccdma_bc_page_rec_type;

typedef struct
{
  byte burst_type;
  byte bc_addr[CAII_BC_ADDR_LEN];
  dword pc_slot_stamp; /* the pc slot stamp of the message */
} mccdma_bc_dup_detect_type;

/* a list to keep all the unique broadcast msgs with time stamps for
 * duplication detection */
extern mccdma_bc_dup_detect_type mccdma_bc_unique_enteries[MAX_BC_DUP_DETECT];

extern int mccdma_bc_list_begin;
/* the beginning of the list, the oldest entry */
extern int bc_tot_entries;
/* the total number of entries to be compared in the list */

/* a record array to keep track of matched Broadcast pages */
extern mccdma_bc_page_rec_type mccdma_bc_page_recs[MAX_BC_PER_PAGE_MSG];

/* the next BC Msg index in mccdma_bc_page_recs to be received */
extern byte mccdma_nxt_bc_rec_index;

/* total number of msg bursts to be received */
extern byte mccdma_tot_bc_msg;

/* broadcast SMS enabled? */
extern boolean mcc_bc_sms_enable;

/* the broadcast slot cycle */
extern word bc_slot_cycle;

/* bc_index initialized or not */
extern boolean mccdma_bc_init;

/* the storage to hold the GPM when bc_index is not updated */
extern mccrxtx_msg_type mccdma_bc_page;

/* indication whether there's a pending broadcast page to be processed */
extern boolean mccdma_bc_pending;

#endif /*  FEATURE_BROADCAST_SMS */

/* the slot(s) whose sleeping criteria are satisfied */
extern byte sleep_slot_mask;

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION CDMA_INIT

DESCRIPTION
  This procedure determines which substate of the initialization substate
  should be processed and vectors control to the appropriate procedure
  to process the substate.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word cdma_init( void );     /* Initialization state */

/* <EJECT> */
/*===========================================================================

FUNCTION CDMA_IDLE

DESCRIPTION
  This procedure determines which mode of idle operation is required and
  calls the appropriate procedure to handle the designated mode.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the next state within the CDMA state machine to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word cdma_idle( void );     /* Idle state */

/* <EJECT> */
/*===========================================================================

FUNCTION CDMA_SA

DESCRIPTION
  This function is the lowest level state machine for the System Acces
  state.  It determines which sub-state needs to be processed and calls
  the appropriate procedure to process the sub-state.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the next state within the CDMA state machine to be processed.

SIDE EFFECTS
  The mobile will attempt to access the system and send a message to the
  base station during the System Access state.

===========================================================================*/

extern word cdma_sa( void );       /* System Access state */

/* <EJECT> */
/*===========================================================================

FUNCTION CDMA_TC

DESCRIPTION
  This function is the lowest level state machine for the Mobile Station
  Control on the Traffic Channel state. It determines which sub-state needs to
  be processed and calls the appropriate procedure to process the sub-state.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word cdma_tc( void );       /* Traffic Channel state */

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_GET_MSG

DESCRIPTION
  This procedure checks inside the base station information array for a
  message received for the currently active base station and returns a
  pointer to the requested message if a valid message is found.

DEPENDENCIES
  Requested Message must be one that is stored in the base station
  information array.

RETURN VALUE
  A pointer to the requested message if a valid message is found.  NULL if
  the message is not found.

SIDE EFFECTS
  During this procedure the base station information array will be checked
  for old information.  Any base station whose information is over 10
  minutes old will be marked inactive.

===========================================================================*/

extern void *mcc_get_msg
(
  word msg_type
    /* message type to retrieve */
);

#ifdef FEATURE_IS2000_QPCH
/*===========================================================================

FUNCTION MCC_GET_BS_INFO

DESCRIPTION
  This procedure checks inside the base station information array for an
  entry for the specified paging channel and returns a pointer to the
  information if the entry is found.  The time input parameter specifies
  when the information message will be used (thus can be used to check for
  expiration).  If NULL (a pointer) time is specified, the information will
  be returned without checking for expiration.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the requested information is found.  FALSE otherwise.

SIDE EFFECTS
  During this procedure the base station information array will be checked
  for old information.  Any base station whose information is over 10
  minutes old will be marked inactive.

===========================================================================*/

boolean mcc_get_bs_info
(
  mccdma_pch_id_type pch_id,
    /* specifies the paging channel to which the message belongs */
  qword info_use_time,
    /* the time the bs_info is used */
  cdma_bs_type **bs
    /* for returning the BS info */
);
#endif // FEATURE_IS2000

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PUT_MSG

DESCRIPTION
  This procedure places a message for the currently active base station
  into the base station information array.

DEPENDENCIES
  Message to be placed in base station information array must be a
  valid type.

RETURN VALUE
  None.

SIDE EFFECTS
  This procedure will examine the message sequences of the messages
  being placed into the base station information array and adjust the
  message receipt flags and the message sequence numbers accordingly.

===========================================================================*/

extern void mcc_put_msg
(
  byte msg_type,      /* message type being placed in base station info */
  caii_rx_msg_type *msg_ptr          /* pointer to message to be placed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PUT_SEQ

DESCRIPTION
  This procedure places a message sequence number (overhead or access
  parameters) into the base station information array for the currently
  active base station.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Message receipt flags for the base station may change.

===========================================================================*/

extern void mcc_put_seq
(
  byte seq_num,                  /* sequence number to insert */
  boolean acc_seq                /* TRUE = acc_msg_seq, FALSE = ovhd_msg_seq */
);

/* <EJECT> */
/*===========================================================================
FUNCTION MCC_INIT_OVHD_INFO

DESCRIPTION
 This function initialises the Base station Info array to ensure that
 all the overhead messages are marked as invalid. This is done so that
 the Overhead Messages are processed properly when we re-enter CDMA.

 This function must be called each time CDMA is entered.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/
void mcc_init_ovhd_info(void);
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_ENTRY_INIT

DESCRIPTION
  This procedure initializes an entry in the base station information array
  with the parameters for the base station having the given pilot_pn and
  using the given cdma channel and paging channel.  If an entry already exists
  for the base station with the given pilot_pn and channel configuration, a
  new entry will not be created.  Since a base station may have different
  parameters for different channels, a separate entry will be kept for each
  different channel we use.  This procedure will set the current base station
  pointer to the correct entry.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the base station information array is full, this procedure will
  replace the entry with the oldest time stamp with this entry.

===========================================================================*/

extern void mcc_entry_init
(
  word pilot_pn,
    /* Pilot PN for new base station */
  word cdma_ch,
    /* CDMA channel */
  byte page_ch,
    /* Paging channel */
  byte band_class
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_REORDER

DESCRIPTION
  This function processes a reorder order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_reorder(void);


/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_INTERCEPT

DESCRIPTION
  This function processes an intercept order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_intercept(void);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_MS_ORIG

DESCRIPTION
  This function processes the Mobile Station Origination operation.
  During this operation the number dialed by the user is stored.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the header within the input mc_msg_type requests that the buffer
  be placed on a queue after processing is completed this procedure will
  do so.

===========================================================================*/

extern void mcc_ms_orig
(
  mc_msg_type *cmd_ptr
    /* Pointer to message received from the handset */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_MS_SMS

DESCRIPTION
  This function processes the Mobile Station SMS operation.
  During this operation the SMS message data sent by the user is stored.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the header within the input mc_msg_type requests that the buffer
  be placed on a queue after processing is completed this procedure will
  do so.

===========================================================================*/

extern void mcc_ms_sms
(
  mc_msg_type *cmd_ptr
    /* Pointer to message received from the handset */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_VALIDATE_SO_REQ

DESCRIPTION
  Determines if the service option request is acceptable

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sets value of cdma.so_req and cdma.service_option_1

===========================================================================*/

extern void mcc_validate_so_req
(
  mcc_page_info_type *page_info
    /* Pointer to the body of the matched page */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_COMP_ADDR

DESCRIPTION
  This function takes a pointer to the header of a paging channel message
  and checks to see if it is addressed to the mobile.
  When IMSI addressing gets added this function will become much larger.
  This function is called from mccidl.c and mccsa.c.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the address matches, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcc_comp_addr
(
  caii_pc_hdr_type *msg_hdr
    /* Header of paging channel message to match address of */
#ifdef FEATURE_BROADCAST_SMS
  ,qword msg_frame
#endif /* FEATURE_BROADCAST_SMS */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_MATCH_MSG

DESCRIPTION
  This function checks all messages except page messages to see if the
  address matches and if the message is not a duplicate.  If both are
  true the message is coppied into  matched_message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the address of the message matches and the message is
  not a duplicate, FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean mcc_match_msg
(
  caii_rx_msg_type *msg_ptr,
    /* Pointer to Order Message */
  mccdma_matched_msg_type *mm_ptr
    /* Pointer to struct to copy message into if match is found. */
#ifdef FEATURE_BROADCAST_SMS
  ,qword msg_frame /* frame which msg was received on */
#endif /* FEATURE_BROADCAST_SMS */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PARSE_PAGE

DESCRIPTION
  This function takes a pointer to a page message and searches through all
  the pages in it to see if any are addressed to the mobile.  If one is
  found, information from the page is coppied into a general structure
  to avoid having to parse the page twice. Since a mobile specific page match
  has highest priority, if a match is found, all other page records can be
  discarded, if not found, all page records (including broadcast pages if
  supported) need to be checked.  This function is called from mccidl.c and
  mccsa.c.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if a page is found which is addressed to the mobile and is not a
  duplicate.  FALSE otherwise.

SIDE EFFECTS
  Stores the new config and access message sequences
  If broadcast paging is supported and broadcast pages are found, the
  BURST_TYPEs and BC_ADDRs fields are copied into mccdma_bc_page_rec[].

===========================================================================*/

extern
#ifdef FEATURE_BROADCAST_SMS
mccdma_page_match_results_type
#else
boolean
#endif /*  FEATURE_BROADCAST_SMS */
mcc_parse_page
(
  caii_rx_msg_type *msg_ptr,
    /* Pointer to received message */
  mcc_page_enum_type page_type,
    /* The type of page to be processed (General, Slotted, Non-slotted) */
  mcc_page_info_type *page_info
    /* Structure to return information stored in the page */
#ifdef FEATURE_BROADCAST_SMS
  ,qword msg_slot /* the slot which the msg was received on */
  ,boolean * bc_pending /* a return status indicating BC records are found but
  not scheduled due to insufficient over head information */
#endif /*  FEATURE_BROADCAST_SMS */
);

/* <EJECT> */
/*===========================================================================
FUNCTION MCC_SRCH_CMD

DESCRIPTION
  This function sends a command to the Searcher task.  If the Searcher task
  responds with a status other than SRCH_DONE_S,  this function will call
  ERR_FATAL to log the fatal error.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.  If this function returns, the status returned by the Searcher
  task was SRCH_DONE_S.

SIDE EFFECTS
  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_srch_cmd
(
  srch_cmd_type *cmd_ptr
    /* pointer to command to send to Searcher */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_RXC_CMD

DESCRIPTION
  This function sends a command to the CDMA Receive subtask.  If the Receive
  responds with a status other than RXC_DONE_S,  this function will call
  ERR_FATAL.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.  If this function returns the status returned by RXC was RXC_DONE_S.

SIDE EFFECTS
  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_rxc_cmd
(
  rxc_cmd_msg_type *cmd_ptr
    /* pointer to command to send to Receive task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_TXC_CMD

DESCRIPTION
  This function sends a command to the CDMA Transmit subtask.  If the Transmit
  task responds with a status other than TXC_DONE_S,  this function will call
  ERR_FATAL.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.

SIDE EFFECTS
  Uses command timer.  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_txc_cmd
(
  txc_cmd_type *cmd_ptr
    /* pointer to command to send to Transmit task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_RXTX_CMD

DESCRIPTION
  This function sends a command to the Layer 2 task.  If the Layer 2
  responds with a status other than RXTX_DONE_S, this function will call
  ERR_FATAL.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.

SIDE EFFECTS
  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_rxtx_cmd
(
  rxtx_cmd_type *cmd_ptr
    /* pointer to command to send to Layer 2 task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_NV_CMD
  This function sends a command to the NV task.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  Status returned by nv_cmd procedure.

SIDE EFFECTS
  Could make call to ERR_FATAL.

===========================================================================*/

extern nv_stat_enum_type mcc_nv_cmd
(
  nv_cmd_type *cmd_ptr
    /* Pointer to command to send to NV */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_WAIT

DESCRIPTION
  This procedure waits for specified signals to be set.  While it waits,
  this procedure continues to set the Watchdog report signal and reset
  the mcc_rpt_timer.

DEPENDENCIES
  Depends on previously having used the mcc_rpt_timer to time when to
  set the watchdog signal.

RETURN VALUE
  Signal mask from signals that were set after rex_wait call.

SIDE EFFECTS
  None.

===========================================================================*/

extern dword mcc_wait
(
  dword mask
    /* Mask of signals to wait for */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_ENCODE_DTMF

DESCRIPTION
  This function converts a called-address digit received from UI
  (in ASCII format) to BCD format.

DEPENDENCIES
  None

RETURN VALUE
  Translated character.

SIDE EFFECTS
  None

===========================================================================*/

extern byte mcc_encode_dtmf
(
  byte ui_char
    /* ASCII digit received from the user interface */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_DECODE_DTMF

DESCRIPTION
  This function converts a BCD encoded called-address digit
  to ASCII format.

DEPENDENCIES
  None

RETURN VALUE
  Translated character.

SIDE EFFECTS
  None

===========================================================================*/

extern byte mcc_decode_dtmf
(
  byte bcd_digit
    /* BCD digit to be converted to ASCII equivalent */
);

/* <EJECT> */
#ifndef FEATURE_SD20
/*===========================================================================

FUNCTION MCC_ROAM

DESCRIPTION
  This function determines the current roam status based on the input SID
  and NID.

DEPENDENCIES
  None.

RETURN VALUE
  Roam value to be placed in the database.

SIDE EFFECTS
  None.

===========================================================================*/

extern db_roam_type mcc_roam
(
  word sid,   /* received system id */
  word nid    /* received network id */
);
#endif /* !FEATURE_SD20 */

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_RETURN_TO_IDLE_INIT

DESCRIPTION
  This function does processing required when the mobile station returns to
  CDMA_IDLE_INIT state from CDMA_OVHD or any other System Access Substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_return_to_idle_init (void);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_GET_MIN_INFO

DESCRIPTION
  This function loads the CDMA structure with the information for the
  orig_min.

DEPENDENCIES
  Depends on current nam being in cdma structure.

RETURN VALUE
  Indicator of whether all parameters were successfully read from NV.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcc_get_min_info ( void );

/* <EJECT> */
#ifndef FEATURE_VOCODER_MANAGER
/*===========================================================================

FUNCTION MCC_VOC_SELFTEST

DESCRIPTION
  This procedure powers up and sends the selftest command to the vocoder.
  If the selftest fails the phone err_fatals, else the vocoder version is
  stored.

DEPENDENCIES
  None.

RETURN VALUE
  Result of tests in vocoder.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_voc_selftest( void );

#endif /* FEATURE_VOCODER_MANAGER */
/* <EJECT> */
/*===========================================================================

FUNCTION PERMUTE_ESN

DESCRIPTION
  This function permutes the ESN as described in IS-95 section 6.1.3.1.8.

DEPENDENCIES
  None.

RETURN VALUE
  Permuted esn.

SIDE EFFECTS
  None.

===========================================================================*/

extern dword permute_esn
(
  dword esn  /* ESN to permute */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_DETECT_DUPLICATE

DESCRIPTION
  This function detects whether a duplicate message has been received
  on the Paging Channel.  If a duplicate message was NOT received this
  function records the time at which the message was received.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - A duplicate message was received on the Paging Channel.
  FALSE - The received Paging Channel message was NOT a duplicate.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_detect_duplicate
(
  boolean page,
    /* Identify if page or non-page message */
  caii_pc_hdr_type *hdr
    /* Header of paging channel message to process layer 2 fields of */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_RESET_PC_FRAMES

DESCRIPTION
  This function resets the mcc_pc_frames and mcc_pc_pg_frames array
  to indicate that no messages received on the Paging Channel will
  be duplicates.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_reset_pc_frames( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_IDLE_HO_RX

DESCRIPTION
  This procedure clears messages received from the current base station and
  sends a request to the receive task to perform idle handoff processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  mcc_rxtx_q will be emptied.

===========================================================================*/

extern void mcc_idle_ho_rx( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_SEND_SRCH_PAGECH

DESCRIPTION
  This procedure sends the Search task a PC_ASET command to switch
  paging channels.

DEPENDENCIES
  Depends on new paging channel being in cdma.pagech.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_send_srch_pagech( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PC_ASSIGN

DESCRIPTION
  This procedure does the required processing for a paging channel assignment
  (mode '001' or '101') from Idle or System Access state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_pc_assign
(
  mccdma_chnasn_type *pc_assign_msg_ptr
    /* Message addressed to this mobile which specifies a Paging Channel
       assignment (ASSIGN_MODE equal to '001' or '101') */
);

#ifdef TMOBILE
#error code not present
#endif /* TMOBILE */

#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_AUTH_CMD

DESCRIPTION
  This function sends a command to the Authentication Task.  If the
  task responds with a status other than AUTH_RECEIVED_S,  this
  function will call ERR_FATAL.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.

SIDE EFFECTS
  Uses command timer.  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_auth_cmd
(
  auth_cmd_type *cmd_ptr
    /* pointer to command to send to Transmit task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_RESYNC_AUTH

DESCRIPTION
  This function sends a command to re-synchronize the Authentication Task
  command queue by queueing a resync command and waiting for it to be
  processed.  If auth has other commands on its queue or is currently
  processing some other command, we will wait for it to finish everything
  and eventually get to our resync command and send the resync report.
  Any other reports it may send before that are discarded and returned to the
  free queue, so they will not be received later when they are not expected.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void mcc_resync_auth( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_QUEUE_AUTH_RPT

DESCRIPTION
  This function enqueues a specified report from the Authentication Task
  to the CDMA subtask of the Main Control Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void mcc_queue_auth_rpt
(
  auth_rpt_type *rpt_ptr
    /* Pointer to authentication report */
);
#endif /* FEATURE_AUTH */

#if (defined(FEATURE_DH) && defined (FEATURE_DH_EXP))
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_QUEUE_DH_RPT

DESCRIPTION
  This function enqueues a specified report from the DH Task
  to the CDMA subtask of the Main Control Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_queue_dh_rpt
(
  dh_rpt_type  *dh_rpt_buf_ptr
    /* Pointer to dh report buffer filled in by dh task */
);
#endif /* FEATURE_DH and FEATURE_DH_EXP */

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SO_CHANGE

DESCRIPTION
  This function changes service options.  It does not do any checking to
  see if the new service option is different from the current service option.
  All error checking regarding the new service option should be done prior to
  calling this procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void  tc_so_change
(
  word new_so
    /* Service option to change to */
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION TC_INFORM_SCR_CHANGE

DESCRIPTION
  This function informs lower layers about the SCR change

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes get queued up to take effect at the action time

===========================================================================*/

extern void tc_inform_scr_change( word new_so, byte action_time);
#endif /* FEATURE_IS2000 */


/* <EJECT> */
/*===========================================================================

FUNCTION MCC_INFO_REC

DESCRIPTION
  This function builds a specified information record into a specified
  buffer.

DEPENDENCIES

RETURN VALUE
   This function returns an integer value as follows:
    -1 - if the type of the requested information record is invalid or
         the specified qualification information is invalid.
     0 - if the requested record will not fit in the specified buffer.
     n - where n is a positive integer indicating the size, in bytes,
         of the requested information record.

SIDE EFFECTS
  None

===========================================================================*/

extern int mcc_info_rec
(
  boolean pc,
    /* TRUE  if status request received on paging chan;
       FALSE if on traffic chan */
  byte band,
    /* Band class qualifier */
  byte op_mode,
    /* Operating mode qualifier */
  byte req_typ,
    /* Type of information record requested */
  void *buf_ptr,
    /* Pointer to place to build information record. */
  word buf_siz,
    /* Size of information record buffer, in bytes */
#ifdef FEATURE_IS2000
  word *ext_fmt_rec_size,
    /* Pointer to hold the size of this IR when packed in ext format */
#endif /* FEATURE_IS2000 */
  byte *reject_reason
    /* Reason for rejecting the request */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PROCESS_INFO_RECORDS

DESCRIPTION
  This function processes the Feature Notification, Alert With Information
  and Flash With Information messages.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if an Alert With Information Message contains a signal record with
  SIGNAL_TYPE field set to 1 or 2.  FALSE otherwise. (This information is
  needed during the Conversation State).

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mcc_process_info_records
(
  byte msg_type,
    /* Message type */
  byte num_recs,
    /* Number of information records in message */
  byte *rec_ptr
    /* Pointer to information records */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_GET_STATE

DESCRIPTION
  Returns the current state of the phone.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern byte mcc_get_state( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_CHECK_OVHD

DESCRIPTION
  This function sets up for the transition to Update Overhead Information
  State of System Access State.  It stores the pending System Access State
  in cdma.sa_state.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the next state to be processed.

SIDE EFFECTS
  Stores the state we are going to after our overhead messages are current
  in cdma.sa_state.

===========================================================================*/

extern word mcc_check_ovhd
(
  word curr_state,
    /* Current Idle substate */
  word sa_state
    /* System Access substate to enter after overhead messages are current */
);

#ifndef FEATURE_SD20
#ifdef FEATURE_OTASP
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PROCESS_OTASP_ORIG

DESCRIPTION
  This function processes OTASP origination commands.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None
===========================================================================*/

extern word mcc_process_otasp_orig
(
  mc_msg_type *cmd_ptr,
    /* Pointer to MC_ORIGINATION_F command */
  word curr_state
    /* Current state being processed */
);

#endif /* FEATURE_OTASP */

#ifdef FEATURE_ACP
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_PROCESS_FORCE_AMPS_ORIG

DESCRIPTION
  This function processes MC_ORIGINATION_F commands that have the
  force_amps field set to TRUE.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None
===========================================================================*/

extern word mcc_process_force_amps_orig
(
  byte cnt,
    /* How many digits in the called address */
  byte *called_address,
    /* ASCII called address */
#ifdef FEATURE_DS
  boolean ds_originated,
    /* Source of Orig command (UI or DS) */
  boolean amps_data,
    /* AMPS only data/voice indicator */
#endif /* FEATURE_DS */
  word curr_state
    /* Current state being processed */
);
#endif /* FEATURE_ACP */
#endif /* !FEATURE_SD20 */

#if (defined(FEATURE_DH) && defined (FEATURE_DH_EXP))
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_DH_CMD

DESCRIPTION
  This function sends a command to the DH Task.  If the
  task responds with a status other than AUTH_RECEIVED_S,  this
  function will call ERR_FATAL.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.

SIDE EFFECTS
  Uses command timer.  Could make call to ERR_FATAL.

===========================================================================*/

extern void mcc_dh_cmd
(
  dh_cmd_type *cmd_ptr
    /* pointer to command to send to Transmit task */
);
#endif /* FEATURE_DH and FEATURE_DH_EXP*/

#ifdef FEATURE_IS95B_TRUE_IMSI
/* <EJECT> */
/*====================================================================

FUNCTION MCC_DEFAULT_IMSI_S1

DESCRIPTION
  This routine will compute default IMSI_S1 value as per IS-95B section 6.3.
  The algorithm is copied from uinv_default_imsi_s1.

DEPENDENCIES
  This function requires that the esn has been loaded into cdma.esn structure.

RETURN VALUE
  This function returns the encoded value of IMSI_S1 with the four least
  significant digits set to ESNp, converted directly from binary to decimal,
  modulo 10000. The other digits are set to zero.

SIDE EFFECTS

====================================================================*/
dword mcc_default_imsi_s1(void);

#endif /* FEATURE_IS95B_TRUE_IMSI */

/* <EJECT> */

/*===========================================================================

FUNCTION MCC_GET_AUTH_IMSI_S1

DESCRIPTION
  This function returns correct imsi_s1 for authentication purposes.

DEPENDENCIES
  None

RETURN VALUE
  IMSI_M_S1 if IMSI_M is programmed. Otherwise, return IMSI_T_S1 (See
  IS-95B section 6.3.12.1 ).

SIDE EFFECTS
  None

===========================================================================*/
dword mcc_get_auth_imsi_s1(void);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_GET_AUTH_IMSI_S2

DESCRIPTION
  This function returns correct imsi_s2 for authentication purposes.

DEPENDENCIES
  None

RETURN VALUE
  IMSI_M_S2 if IMSI_M is programmed. Otherwise, return IMSI_T_S2 (See
  IS-95B section 6.3.12.1 ).

SIDE EFFECTS
  None

===========================================================================*/
word mcc_get_auth_imsi_s2(void);


#ifdef FEATURE_GPSONE
#error code not present
#endif /* FEATURE_GPSONE */

/*===========================================================================

FUNCTION MCC_SEND_SRCH_PARMS

DESCRIPTION
  This procedure sends the updated search parameters to the searcher.

DEPENDENCIES
  mcc_ho_real must have been updated appropriately

RETURN VALUE
  None.

SIDE EFFECTS
  The search parmameters in the searcher will be changed.

===========================================================================*/
void mcc_send_srch_parms( void );

/* <EJECT> */
/*===========================================================================

FUNCTION PC_SRCH_RPT

DESCRIPTION
  This procedure processes the idle search report processing.

DEPENDENCIES
  Depends on the Initialization state already having been processed.

RETURN VALUE
  Returns the next state within the CDMA state machine to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

word pc_srch_rpt
(
  mccsrch_rpt_type *rpt_ptr,
    /* Report received from searcher */
  word new_state
    /* Next state to be processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_CM_MC_RPT

DESCRIPTION
  This function sends a report to the CM task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_cm_mc_rpt
(
  cm_mc_rpt_type *cmd_ptr
    /* pointer to command to send to cm */
);

/* <EJECT> */
/*===========================================================================

FUNCTION mcc_check_alert_signal_info

DESCRIPTION
  This function checks whether the info record contains a signal info record
  or not and it also checks the whether the signal type is set to 1 or 2
  if the info record contains a signal info record

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if an Alert With Information Message contains a signal record with
  SIGNAL_TYPE field set to 1 or 2. or it does not contain a signal information record
  FALSE otherwise. (This information is needed during the Conversation State).

SIDE EFFECTS
  None.

===========================================================================*/
boolean mcc_check_alert_signal_info
(
  byte msg_type,
    /* Message type */
  byte num_recs,
    /* Number of information records in message */
  byte *rec_ptr
    /* Pointer to information records */
);

#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
/* <EJECT> */
/*===========================================================================

FUNCTION MCC_SEND_MO_DBM_STATUS

DESCRIPTION
  This function sends a status report to the CM task for a mobile originated
  data burst message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcc_send_mo_dbm_status
(
  byte burst_type,
    /* Data Burst Message type, currently, it supports only SMS and PD */
  cm_dbm_mo_status_e_type err_code
    /* Mobile originated data burst message error code */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_CHECK_MO_DBM_STATUS

DESCRIPTION
  This function checks if the MC is waiting for a layer2 ack for a specific
  data burst message (SMS or PD), and if so, sends out a status report to CM
  task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcc_check_mo_dbm_status
(
  cm_dbm_mo_status_e_type err_code
    /* Mobile originated data burst message error code */
);

#endif /* FEATURE_UASMS || FEATURE_GPSONE */

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_SEND_ORIG_FAILURE

DESCRIPTION
  This function sends a report to the CM task for an origination failure .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcc_send_orig_failure(cm_call_orig_fail_e_type err_code);



#ifndef FEATURE_SD20
/*===========================================================================

FUNCTION MCSYSPR_SET_LAST_ACTIVE_CDMA

DESCRIPTION
  This function sets the last active cdma system.  Information
  recorded included the CDMA channel and band class.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the passed system was set as the last active CDMA system
  FALSE Otherwise

SIDE EFFECTS
  Modifies the mcsyspr_last_active_cdma

===========================================================================*/
boolean mcsyspr_set_last_active_cdma
(
  word cdmach,              /* cdma channel */
  byte band_class           /* band class */
);
#endif /* !FEATURE_SD20 */

/* <EJECT> */
#ifdef FEATURE_BROADCAST_SMS
/*===========================================================================

FUNCTION MCC_DETECT_DUP_BCAST

DESCRIPTION
  This function checks for duplicate broadcast message based on the
  BURST_TYPE and BC_ADDR.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if a duplicate is found, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean mcc_detect_dup_bcast
(
  /* all the info needed to determine whether a dup is received */
  byte burst_type,
  byte *bc_addr,
  dword pc_slot_stamp,
  word bc_expire_cycle /* the expiration cycle in paging slots */
);

/*===========================================================================

FUNCTION MCC_INSERT_UNIQUE_BCAST

DESCRIPTION
  This function inserts the broadcast msg time stamp into the
  bc_unqiue_entries[] based on BURST_TYPE and BC_ADDR.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  bc_unqiue_entries[] is updated

===========================================================================*/
void mcc_insert_unique_bcast
(
  /* all the info needed to determine whether a dup is received */
  byte burst_type,
  byte *bc_addr,
  dword pc_slot_stamp
);

/*===========================================================================

FUNCTION MCC_INIT_BCAST_DUP_LIST

DESCRIPTION
  Initialize the broadcast duplicate detection list

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mcc_init_bcast_dup_list (void);

/*===========================================================================

FUNCTION MCC_IS_BC_PAGE_SLOT

DESCRIPTION
  This function returns whether the given slot is a broadcast page slot. A
  paging slot is a broadcast slot, if it's the first or second slot within
  the current broadcast cycle.
  It takes in the relative paging slot position within the broadcast cycle as
  parameter.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it's a broadcast page slot

SIDE EFFECTS
  None

===========================================================================*/
boolean mcc_is_bc_page_slot (word bc_slot_pos);

/*===========================================================================

FUNCTION MCC_IS_BC_DATA_SLOT

DESCRIPTION
  This function returns whether the given slot is a scheduled broadcast slot to
  receive data burst.
  It takes in the relative paging slot position within the broadcast cycle as
  parameter.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if it's a broadcast slot

SIDE EFFECTS
  None

===========================================================================*/
boolean mcc_is_bc_data_slot (word bc_slot_pos, word bc_slot_cycle);

/*===========================================================================

FUNCTION MCC_BC_MATCH

DESCRIPTION
  This function parse the *pc_hdr to see if it is a BC message header that
  contains the expected BC_ADDR.

DEPENDENCIES
  It depends on the calling function to made sure that *pc_hdr contains enough
  useful bits (at least CAII_BC_HDR_LEN bits). And the current slot is the
  scheduled BC SMS slot.

RETURN VALUE
  TRUE if it contains the expected BC_ADDR, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean mcc_bc_match(byte * pc_hdr);

#endif /*  FEATURE_BROADCAST_SMS */


#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================

FUNCTION MCC_SET_RSCH_DTX_SIG

DESCRIPTION
  This function sets the MCC_TXC_REV_DTX_SIG. It is called by TXC when it
  detects REV DTX.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mcc_set_rsch_dtx_sig (void);
#endif

#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================

FUNCTION CDMA_RF_INIT

DESCRIPTION
  This procedure initializes rf for the CDMA mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void cdma_rf_init
(
  word channel,
    /* channel to acquire */
  byte band_class
    /*  band class to acquire in */
);

/*===========================================================================

FUNCTION CDMA_EXIT_INIT

DESCRIPTION
  This procedure initializes the Exit state for CDMA mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void cdma_exit_init( void );

/*===========================================================================

FUNCTION CDMA_CONTINUE

DESCRIPTION
  This function is called to reset the CDMA acquisition parameters when
  the call to function mcsys_cdma_continue() indicates that we are to
  continue operating in CDMA mode without passing control back to the
  system determination level first (i.e., we are currently operating on
  the preferred system).

DEPENDENCIES
  None

RETURN VALUE
  The next substate to be processed.

SIDE EFFECTS
  RXTX and Searcher tasks will go to Start mode.  Receive and Transmit
  tasks will reset to CDMA initialization mode.

===========================================================================*/

word cdma_continue
(
  boolean voc_pwr_off,
    /* Indicator of whether vocoder power needs to be turned off */
  mcc_entry_type *mcc_entry_reason_ptr
    /* Contains entry parameters */
);

/*===========================================================================

FUNCTION CDMA_EXIT

DESCRIPTION
  This procedure is the exit state for the CDMA state machine.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  RXTX task will go to start mode,  Receive task will exit CDMA mode,
  Transmit task will exit CDMA mode and Search task will go to Start
  mode.

===========================================================================*/

void cdma_exit
(
  boolean voc_pwr_off
    /* indicator of whether vocoder power needs to be turned off */
);

#endif /* FEATURE_FACTORY_TESTMODE */

/*===========================================================================

FUNCTION MCC_UPDATE_BS_P_REV

DESCRIPTION
   Updates internal data and report events when BS _PREV is received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_update_bs_p_rev
(
  uint8 bs_p_rev
);

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION reg_retry_delay

DESCRIPTION
  This function binds a retry delay function to a function pointer. If an old
  retry delay function already exists, it overwrites it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void reg_retry_delay
(
  void (*ds_retry_delay_inform)(retry_delay_type retry_parm)
);

/*===========================================================================

FUNCTION update_retry_delay

DESCRIPTION
  This function informs DS the latest retry delay duration by calling DS's
  registered retry delay processing function.  It uses the current system
  time and given retry delay to compute the actual retry timer duration in
  unit of milliseconds.

  SCRM retry timer duration = (time left before the next 80ms boundary +
                               retry delay * 320) ms

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void update_retry_delay
(
  uint8 retry_type,
  uint8 retry_delay
);
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_SD20      
#error code not present
#else /* FEATURE_SD20 */
/*===========================================================================

FUNCTION MCC_SEND_CALL_END_ACK

DESCRIPTION
  This function sends an ACK to the Call Manager for the MC_END_F command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcc_send_call_end_ack(void);
#endif /* FEATURE_SD20 */

/*===========================================================================

REDFUNCTION MCC_REPORT_EVENT_MSG_RXED

DESCRIPTION
  Report event Message received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_report_event_msg_rxed
(
  cai_rx_channel_id_type channel_id, 
  byte *msg_ptr
);
  
/*===========================================================================

REDFUNCTION MCC_REPORT_EVENT_MSG_TXED

DESCRIPTION
  Report event Message transmitted.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_report_event_msg_txed
(
  cai_tx_channel_id_type channel_id, 
  byte *msg_ptr 
);

/*===========================================================================

REDFUNCTION MCC_REPORT_EVENT_TIMER

DESCRIPTION
  Report event Timers T1m, T5m, T20m, T21m, T30m, T72m, T40m, T41m, T42m,
  T50m, T51m, T52m, T53m, T55m, T57m, T64m, T65m or T68m expired.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_report_event_timer
(
  cai_timer_id_type timer_id
);
  
/*===========================================================================

REDFUNCTION MCC_REPORT_EVENT_COUNTER

DESCRIPTION
  Report event Counters N1m, N14m, N15m, N2m, N3m, N5m, N11m threshold
  reached.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_report_event_counter
(
  cai_counter_id_type counter_id 
);

/*===========================================================================

FUNCTION MCC_REPORT_EVENT_CP_STATE

DESCRIPTION
  Report events Call processing state change, Slotted mode operation. and
  QPCH in use. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void mcc_report_event_cp_state
(
  word new_state
);

#endif /* MCCDMA_H */

