#ifndef ACP553_H
#define ACP553_H
/*===========================================================================

                        ACP 553 Declarations

DESCRIPTION
   This header file contains the definitions for the analog numeric
   indicators and other related constants defined in IS-95 Chapter 1.

Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996,1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/acp553.h_v   1.1   16 Feb 2001 13:10:38   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
07/23/99   fc      Added digital_reg_sp, cdma_mode_s and max_redirect_delay
                   fields to acp553 structure.
05/10/99   kmp     Replace T_ACP with FEATURE_ACP
01/05/98   ht      Modify exit_analog_mode flag to a enum.
12/22/98   ht      Add exit_analog_mode flag in acp553 structure.
09/04/98   pms     Neared the variables to save ROM space.
01/27/98   ldg     FEATURE_DS updates (by hand).
05/20/97   ck      Added lpm_stat_enabled indicator
05/24/96   jjw     Gemini & ISS2 Data Services release.
01/02/96   jca     Added acp553_rsat_status_type.
12/21/95   jca     Code update to support KMT "silent paging".
12/19/95   jca     Added #defines for AMPS Local Control Orders.
12/18/95   jca     Increased ACP553_RXMSG_WORD_LEN.
11/29/95   jca     Deleted sid_acq_list and acq_sid_list_enabled fields
                   from acp553 struct.
11/28/95   dna     Account for RAND = 0 for registration authentication.
07/20/95   jca     Added service redirection parameters to acp553 struct.
07/07/95   dna     Added acp553 struct fields for authentication
06/25/95   dy      Added define for Alert With Info SMS msg type.
01/27/94   jca     Added defines for extended protocol (NAMPS) msg types.
03/29/93   jca     Added SIDr.  Added new type codes for Directed Retry msg.
12/08/92   ARH     Updates for change between single SID acquisition to SID
                   list acquisition.
11/13/92   JCA     Added LRCCs (IS-54 A/B upgrade).
09/22/92   ARH     Added GAM overhead code
08/31/92   JCA     Added powerdown_stat_enabled indicator.
06/09/92   ARH     Added SID lockout information
05/08/92   ARH     Changed names of an indicator. Made idle_timeout a word
05/05/92   ARH     Modified names on ordqs for consistency
05/05/92   ARH     Updated indicators per new system determination
03/17/92   ARH     Changed indicators to lowercase
02/12/92   ARH     Removed state info definitions to ACPMCI.H
01/10/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "nv.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
   Miscellaneous type definitions
---------------------------------------------------------------------------*/
typedef enum {
  ACP553_STREAM_A,        /* For MINs whose LSB is '0' */
  ACP553_STREAM_B         /* For MINs whose LSB is '1' */
} acp553_stream_type;


typedef enum {
  ACP553_CCHAN_MODE,      /* Control Channel Mode */
  ACP553_VCHAN_MODE       /* Voice Channel Mode   */
} acp553_mode_type;


typedef enum {            /* SAT Color Codes               */
  ACP553_SCC_00,          /* See EIA/TIA 553 Table 3.7.1-2 */
  ACP553_SCC_01,
  ACP553_SCC_10,
  ACP553_SCC_INVALID
} acp553_scc_type;


typedef enum {                    /* Exit Analog Mode Reason Codes */
  ACP553_DO_NOT_EXIT,
  ACP553_EXIT_BS_IN_OVLD,         /* Rcv BS Overload Msg */
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
  ACP553_EXIT_REASON_MAX
} acp553_exit_analog_mode_type;

typedef enum {        /* SAT Detection Status Codes                   */
  ACP553_VALID_SAT,   /* SAT matches SCCr (expected SAT)              */
  ACP553_INVALID_SAT, /* SAT detected but does not match expected SAT */
  ACP553_NO_SAT       /* No valid SAT detected                        */
} acp553_rsat_status_type;


#define SYSA_1ST_DED_CTL_CHAN  333
  /* System A first dedicated control channel number  */

#define SYSB_1ST_DED_CTL_CHAN  334
  /* System B first dedicated control channel number  */

/* Constant definitions related to numeric indicators */
#define ACP_MAX_CCLIST_POS 6

/* ------------------------------------------------------------------------
  The following type contains all the numeric and status indicators defined
  in EIA/TIA 553 Section 1
  ------------------------------------------------------------------------- */

typedef struct {
  /* Regular Indicators */
  word            firstcha_s;  /* Number of first analog control channel */
  word            firstchd_s;  /* Number of first control channel        */
  word            firstchp_s;  /* Number of first paging channel         */
  word            lastcha_s;   /* Number of last analog control channel  */
  word            lastchd_s;   /* Number of last control channel         */
  word            lastchp_s;   /* Number of last paging channel          */
  byte            cmax_s;      /* Maximum number of channels to scan     */
  word            regincr_s;   /* Increments between registrations       */
  dword           regid_s;     /* Registration number received           */
  word            sid_s;       /* System ID rxed on dedicated control chan */
  word            sid_r;       /* System ID rxed on paging or access chan  */
  boolean         s_s;         /* Send serial number enable/disabled     */
  boolean         r_s;         /* Registration enabled/disabled          */
  boolean         e_s;         /* Send MIN1 or MIN1-MIN2 enab/disab.     */
  boolean         dtx_s;       /* Discontinous transmission enab/disb    */
  byte            n_s;         /* Number of paging channels to scan      */
  boolean         rcf_s;       /* Read control filler enable/disable     */
  boolean         cpa_s;       /* Combined Paging/Access channels        */
  boolean         wfom_s;      /* Wait for ovhd message                  */
  acp553_scc_type scc_s;       /* SAT color code                         */
  byte            dcc_s;       /* Digital Color Code                     */
  byte            sdcc1_s;     /* Supplementary Digital Color Code       */
  byte            sdcc2_s;     /* Supplementary Digital Color Code       */
  boolean         lt_s;        /* Next access attempt is "Last Try"      */
  byte            maxbusy_sl;  /* Max. busy occurrences allowed          */
  byte            maxsztr_sl;  /* Max. number of seizure attempts        */
  byte            pl_s;        /* Mobile station RF Power Level          */
  boolean         bis_s;       /* Idle-to-busy transition check          */
  word            cclist_s[ACP_MAX_CCLIST_POS]; /* Control Channel List  */
  boolean         auth_s;      /* Authentication procedures supported    */
  dword           rand_s;      /* Random Challenge Memory                */
  word            locaid_s;    /* Location area identifier               */
  byte            pureg_s;     /* Power-Up Registration Status           */
  byte            pdreg_s;     /* Power-Down Registration Status         */
  byte            lreg_s;      /* Location Registration Status           */
  word            lrcc_s;      /* Last registration control channel used */
#ifdef FEATURE_AUTH
  dword           reg_rand;    /* RAND used to compute AUTHR for last reg */
  boolean         authr_valid; /* Identifies if valid authr is stored     */
  dword           reg_authr;   /* AUTHR used is registration messages     */
  dword           randu;       /* Value received in Auth Challenge Order  */
  dword           randbs;      /* Value sent in BS Challenge Order        */
  dword           auth_sig;    /* Signature from Authentication Task      */
  boolean         encryption_enabled; /* Indicates message encryption on  */
#endif/* FEATURE_AUTH */

  /* Status indicators */
  boolean  serv_sys_stat_enabled;      /* enabled/disabled                */
  boolean  first_regid_stat_enabled;   /* First-registration ID status    */
  boolean  roam_stat_enabled;          /* enabled/disabled                */
  boolean  termination_stat_enabled;   /* enabled/disabled                */
  boolean  powerdown_stat_enabled;     /* enabled/disabled                */
  /*  a separate non_standard variable for LPM_Powerdown */
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
  boolean  fade_timing_stat_enabled;   /* enabled/disabled                */
  boolean  first_locaid_stat_enabled;  /* First-location area ID status   */
  boolean  first_idle_id_stat_enabled; /* First-idle ID status            */
  boolean  loc_reg_id_stat_enabled;    /* Location-registration ID status */

  /* Permanent indicators stored in NAM (Cached from NVM) */
  word            accolc_p;     /* Access Overload Class                 */
  boolean         ex_p;         /* Access Method                         */
  word            firstchp_p;   /* First Paging Channel                  */
  dword           min1_p;       /* Mobile Identification Number (Part 1) */
  word            min2_p;       /* Mobile Identification Number (Part 2) */
  word            home_sid_p;   /* System Identification                 */
  byte            scm_p;        /* Station Class Mark                    */
  dword           esn_p;        /* Electronic Serial Number (TBD)        */
  boolean         autoreg_enabled_p; /* Autonomous Registration Enabled  */
  boolean         autoreg_whereabouts_p; /* Autonomous Reg. Whereabouts  */


  /* Semi-Permanent indicators */
  dword           nxtreg_sp;    /* Next Registration Time                */
  word            sid_sp;       /* System ID of last registration        */
  word            locaid_sp;    /* Location area identifier              */
  byte            pureg_sp;     /* Power-up registration identifier      */
  byte            count_sp;     /* Call History Parameter                */

  /* System Acquisition Indicators */
  /* The following are not numeric indicators from the standard.
  ** They are our additions in implementing system determination.
  */
  boolean         acq_home_sid_only_enabled;
                                        /* Enabled if home SID is to
                                        ** be acquired  */

#ifndef FEATURE_SD20
  boolean         ignore_cdma_gam;      /* Ignore CDMA Global Action Msg */

  word            idle_timeout;         /* Return to system determination
                                        ** after being in idle state for
                                        ** 'idle_timeout' minutes. If
                                        ** idle_timeout = 0 no timeout is
                                        ** to be performed */

  word            sid_lock_list[NV_MAX_SID_LOCK];
                                        /* Contains a list of SIDs to lock
                                        ** out when performing acquisition
                                        ** A SID of 0 means the entry should
                                        ** be ignored */

  /* The following indicator is used to indicate that the program must
   * exit to system determination once it is set to a non-zero value
   */
#endif /* !FEATURE_SD20 */

  acp553_exit_analog_mode_type  exit_analog_mode;

#ifndef FEATURE_SD20
  /* Service Redirection indicators */

  boolean redirection_enabled;
    /* Service Redirection indicator - Set to enabled to indicate that
       service redirection is currently in effect; otherwise set to
       disabled */

  boolean ignore_cdma;
    /* Ignore CDMA Available indicator - Set to '1' to indicate that the
       mobile station is to ignore the CDMA Capability Message on the
       analog system to which it has been redirected. */

  word    expected_sid;
    /* Expected SID - If the base station is redirecting the mobile station
       to a specific system, this field shall be set to the  SID of that
       system; otherwise the field is set to zero. */

  boolean reacquire_analog;
    /* If set to TRUE then the mobile must reenter the Paging Channel
       Selection Task after executing Serving-System Determination. */
#endif /* !FEATURE_SD20 */

#ifdef FEATURE_IS95B
  /* ----------------------------------------------------------------
  ** The following fields are used to support the following features:
  ** (1) Analog Registration Improvement 
  ** (2) Registration Randomization after GSRM
  ** ---------------------------------------------------------------- */
  boolean cdma_mode_s;
    /* If set to TRUE then the mobile must randomize the analog
       registration based on the maximum redirect delay of GSRM */
  boolean digital_reg_sp;
    /* If set to TRUE then the mobile must register to the analog system 
       since it exits from a CDMA system which it has registered */
  byte    max_redirect_delay_s;
    /* Maximum redirect delay - If the base station is redirecting the
       mobile station from a CDMA system to an Analog system, this field 
       shall be set to the maximum redirect delay of GSRM */
#endif /* FEATURE_IS95B */
} acp553_num_type;


/* External Declaration */
extern acp553_num_type acp553;


#ifdef FEATURE_DS
/* ------------------------------------------------------------------------
  Analog data-related booleans used to indicate the type of call in process.
  ------------------------------------------------------------------------- */
extern boolean acpmc_datacall;             /* Analog Data call in process.  */

#endif /* FEATURE_DS */

/*---------------------------------------------------------------------------
   Forward channel message field definitions.  EIA/TIA-553 Section 3.7
---------------------------------------------------------------------------*/

/*                 EIA-553 constant definitions                            */

/* Overhead Message types    (Table 3.7.1.2-1)                              */
#define ACP553_REG_ID_OHD           0x0000  /* registration ID ohd field    */
#define ACP553_CTL_FILLER_OHD       0x0001  /* ctl filler OHD field value   */
#define ACP553_GAM_OHD              0x0004  /* Global Action Msg            */
#define ACP553_SP_WORD1_OHD         0x0006  /* sys parm OHD field val, wd 1 */
#define ACP553_SP_WORD2_OHD         0x0007  /* ohd field val sys parm word 2*/


/* Miscellaneous T1T2 fields */
#define ACP553_1WORD_CC_T1T2        0x0000  /* 1st word type field for 1word*/
#define ACP553_MWORD_CC_W1_T1T2     0x0001  /* 1st word type field mult. wds*/
#define ACP553_MWORD_CC_T1T2        0x0002  /* all words after word 1       */
#define ACP553_VC_WORD1_T1T2        0x0002  /* T1T2 for voice channel word1 */
#define ACP553_VC_MWORD_T1T2        0x0001  /* T1T2 for voi chan multiwords */
#define ACP553_OVERHD_MSG_T1T2      0x0003  /* overhead type field value    */

/* Other miscellaneous defines */
#define ACP553_OVERHD_LAST_END      0x0001  /* END field val for last word  */

/* mobile station control order field definitions */

#define ACP553_ORDER_SCC               0x0003 /* SCC field value for order  */
#define ACP553_PAGE_ORDER              0x0000
#define ACP553_ORIGINATION_ORDER       0x0000
#define ACP553_ALERT_ORDER             0x0001
#define ACP553_ALERT_W_INFO_ORDER      0x0011
#define ACP553_FLASH_W_INFO_ORDER      0x0012
#define ACP553_RELEASE_ORDER           0x0003
#define ACP553_REORDER_ORDER           0x0004
#define ACP553_MSG_WAITING_ORDER       0x0005
#define ACP553_STOP_ALERT_ORDER        0x0006
#define ACP553_AUDIT_ORDER             0x0007
#define ACP553_SEND_CALLED_ADDR_ORDER  0x0008
#define ACP553_INTERCEPT_ORDER         0x0009
#define ACP553_MAINTENANCE_ORDER       0x000A
#define ACP553_SET_POWER_LEVEL_ORDER   0x000B
#define ACP553_DIRECTED_RETRY_ORDER    0x000C
#define ACP553_REGISTR_ORDER           0x000D
#define ACP553_REGISTR_W_AUTH_ORDER    0x0018
#define ACP553_LOCAL_CONTROL_ORDER     0x001E
#define ACP553_PARAM_OR_SERIAL_ORDER   0x000F
#define ACP553_PAGE_RESP_W_AUTH_ORDER  0x0002
#define ACP553_ORIGINATE_W_AUTH_ORDER  0x0002
#define ACP553_BS_CHALLENGE_ORDER      0x0013
#define ACP553_UNIQUE_CHALLENGE_ORDER  0x0014
#define ACP553_SSD_UPDATE_ORDER        0x0015
#define ACP553_DISABLE_DTMF_ORDER      0x0016
#define ACP553_MSG_ENCRYPT_ORDER       0x0017

/*   mobile station control order qualifier field defs                     */

#define ACP553_ALERT_ORDQ               0x0000
#define ACP553_ABBR_ALERT_ORDQ          0x0001
#define ACP553_DIR_RETRY_NOTLAST_ORDQ   0x0000
#define ACP553_DIR_RETRY_LAST_ORDQ      0x0001
#define ACP553_POWER_LEVEL0_ORDQ        0x0000
#define ACP553_POWER_LEVEL1_ORDQ        0x0001
#define ACP553_POWER_LEVEL2_ORDQ        0x0002
#define ACP553_POWER_LEVEL3_ORDQ        0x0003
#define ACP553_POWER_LEVEL4_ORDQ        0x0004
#define ACP553_POWER_LEVEL5_ORDQ        0x0005
#define ACP553_POWER_LEVEL6_ORDQ        0x0006
#define ACP553_POWER_LEVEL7_ORDQ        0x0007
#define ACP553_AUT_REG_UNKNOWN_ORDQ     0x0002
#define ACP553_AUT_REG_KNOWN_ORDQ       0x0003
#define ACP553_AUT_REG_PDOWN_ORDQ       0x0003
#define ACP553_PARAM_UPDATE_ORDQ        0x0000
#define ACP553_SER_NUM_REQUEST_ORDQ     0x0001
#define ACP553_MSG_ENCRYPT_DISABLE_ORDQ 0x0000
#define ACP553_MSG_ENCRYPT_ENABLE_ORDQ  0x0001
#define ACP553_SSD_UPDATE_FAILURE_ORDQ  0x0000
#define ACP553_SSD_UPDATE_SUCCESS_ORDQ  0x0001
#define ACP553_LOCAL_CONTROL_ORDQ       0x0000

/*   Mobile station control message type field defs               */

#define ACP553_ALERT_SMS_MSG_TYPE       0x0001
#define ACP553_AUT_REG_PDOWN_MSG_TYPE   0x0001
#define ACP553_DIR_RETRY_MSG_TYPE       0x0000
#define ACP553_DIR_RETRY_AUTH_DIS_MTYPE 0x0001
#define ACP553_DIR_RETRY_AUTH_ENA_MTYPE 0x0002
#define ACP553_LOC_CTL_ABR_INT_MTYPE    0x0001
#define ACP553_LOC_CTL_ABR_REORD_MTYPE  0x0002
#define ACP553_LOC_CTL_ABR_ALERT_MTYPE  0x0005

/*   overhead message train (global action) types definition     */

#define ACP553_RESCAN_ACT             0x0001
#define ACP553_REGINCR_ACT            0x0002
#define ACP553_LOCAID_ACT             0x0003
#define ACP553_CDMA_AVAILABLE_ACT     0x0004
#define ACP553_NEW_ACCESS_CHAN_ACT    0x0006
#define ACP553_RANDOM_CHALLENGE_A_ACT 0x0007
#define ACP553_OVERLOAD_CTL_ACT       0x0008
#define ACP553_ACCESS_TYPE_PARMS_ACT  0x0009
#define ACP553_ACCESS_ATT_PARMS_ACT   0x000A
#define ACP553_RANDOM_CHALLENGE_B_ACT 0x000B
#define ACP553_LOCAL_CONTROL_1_ACT    0x000E
#define ACP553_LOCAL_CONTROL_2_ACT    0x000F

/* ---------------------------------------------------
** Extended protocol message types (see IS-88 Table 9)
** --------------------------------------------------- */
#define ACP553_VOICE_MAIL_EP_MSG  0x82
#define ACP553_CLI_EP_MSG         0x84
#define ACP553_SHORT_MSG_EP_MSG   0x85
#define ACP553_NOT_CAPABLE_EP_MSG 0x07

/*---------------------------------------------------------------------------
  Control and voice channel order message lengths
---------------------------------------------------------------------------*/

#define ACP553_CC_DIR_RETRY_SIZ  4        /* Directed retry message length */
#define ACP553_CC_CHALLENGE_SIZ  3        /* BS and unique challenge length*/
#define ACP553_CC_SSD_UPDATE_SIZ 5        /* SSD Update message length     */

#define ACP553_VC_CHALLENGE_SIZ  2        /* BS and Unique challenge length*/
#define ACP553_VC_SSD_UPDATE_SIZ 4        /* SSD Update length             */

/*---------------------------------------------------------------------------
   EIA/TIA-553 Forward and Reverse Message/Word Sizes Section 2.7 & 3.7
---------------------------------------------------------------------------*/

/*
** Length of a full FWD word is 40 bits including parity. The parity is
** handled within the ACPFM interface so the data part of it is
** 40 - 12 = 28 bits. We need 4 (8-bit) bytes to hold 28 bits. The
** 4 least significant bits are meaningless
*/
#define ACP553_RXWORD_BYTE_LEN  4
typedef byte acp553_rxword_type[ ACP553_RXWORD_BYTE_LEN ];

/*
** Max length of forward message based on max length AWI SMS message
** where all 7 bits of the RL_W field are set to '1'
*/
#define ACP553_RXMSG_WORD_LEN 129

/*
** Forward message type is an array of acp553_rxword_type(s).
*/
typedef struct {
  acp553_rxword_type rxword[ ACP553_RXMSG_WORD_LEN];
} acp553_rxmsg_type;

/*
** Length of a full FWD word is 48 bits including parity. The parity is
** handled within the ACPFM interface so the data part of it is
** 48 - 12 = 36 bits. We need 5 (8-bit) bytes to hold 36 bits. The
** 4 least significant bits are meaningless
*/
#define ACP553_TXWORD_BYTE_LEN 5
typedef byte acp553_txword_type[ ACP553_TXWORD_BYTE_LEN ];

/*
** Full Reverse Message type definition as an array of acp553_txword_type(s).
** Since we are supporting 32 digit dialing, the maximum number of words in
** a reverse message is 8 (See CDMA CAI 4.1.2.2)
*/
#define ACP553_TXMSG_WORD_LEN 8  /* Max length of a rev msg in 553 words */
typedef struct {
  acp553_txword_type txword[ ACP553_TXMSG_WORD_LEN ];
} acp553_txmsg_type;

#endif

