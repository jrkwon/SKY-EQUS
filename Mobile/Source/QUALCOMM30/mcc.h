#ifndef MCC_H
#define MCC_H
/*===========================================================================

                        MCC Declarations

DESCRIPTION
   This header file contains the definitions and external interfaces
   to the MC CDMA Main Control subtask.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/mcc.h_v   1.2   16 Feb 2001 13:06:20   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   fc      Added support for system determination 2.0. All changes
                   are featurized under FEATURE_SD20.
09/11/00   yll     Added support for Position Determination DBM.
03/06/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           kk      mcc_exit_type modified to include mcc_system_lost_type
           ych     Added support for Tri-mode Silent Redial
11/01/99   lcc     Moved Pseudo MC (used in PLT) related to plt_mc.
09/16/99   lcc     Merged in PLT support from PLT archive.
08/18/99   lh      Bug fix in mcc_entry_orig_type.
08/13/99   lh      CM and SMS phase 2 implementations.
07/07/99   ht      Add mcc_new_system_type structure.
06/22/99   ck      Changed the prototype of function mcc_is_spc_locked to
                   mcc_ignore_activation_code.
05/06/99   kmp     Merge IS-95B changes below & change T_ACP to FEATURE_ACP
           kk      NDSS feature modifications; new exit type (MCC_NDSS_OFF)
                   and ndss parameters added.
           kka     New exit code for system reselection added.
01/18/99   abh     Changes for OTASP 3.0
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements
07/20/98   pms     Added the MCC_AHO_EXIT_TYPE() macro.
06/25/98   kis     Added the ASCII related fields in mcc_entry_orig_type,
                   mcc_auto_nam_change_type and mcc_orig_cmd_rxed_type.
02/18/98   dna     Added a .gen member to the mcc_entry union to allow
                   easy reference to common channel and band class fields.
01/27/98   ldg     FEATURE_DS updates (by hand).
10/17/97   jrs     Added a redirection type field to the redirection exit
                   structure from CDMA. This is used to identify the type
                   of redirection as either global or directed.
06/10/97   jrs     Removed MCC_OTASP_ORIG_RXED exit reason.
05/20/97   ych     Added a new exit reason MCC_LPM.
02/06/97   jca     Added mcc_aho_page_type.
12/13/96   jca     Modified mcc_entry_orig_type.  Added mcc_aho_orig_type
                   and mcc_orig_cmd_rxed_type.
09/26/96   dna     Added MCC_OTASP_ORIG_RXED exit reason for FEATURE_OTASP
09/13/96   dna     Added new entry and exit reasons for FEATURE_OTASP
06/24/96   jrs     Added support for Preferred Roaming.
05/13/96   jca     Added band_class and block_or_sys to mcc_entry_acq_type.
02/08/96   jca     Added MCC_MAX_ACCESS_PROBES exit reason.
11/29/95   jca     Deleted MCC_ACQ_SID_ACQ enum from mcc_sys_acq_enum_type.
07/20/95   jca     Changes to support Subscriber Access Control.
06/15/95   jca     Added MCC_AUTO_NAM_CHANGE exit reason.
11/14/94   dna     Added MCC_AHO_WAIT_FOR_PAGE exit reason and removed
                   aho_page_response from the mcc_exit_type.
04/20/94   jca     Externalized MCC_POWERUP_LOAD function to facilitate
                   resetting NAM.
05/04/93   jca     Added new entry and exit reasons.
09/03/92    ip     Added normal_exit structure to ACQ_LOST exit reason.
08/29/92    ip     Changed mcc_powerup_init to typed (boolean) procedure.
08/22/92    ip     Added exit types to idle timeout and acq terminated.
07/04/92    ip     Initial creation.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "customer.h"
#include "nv.h"
#include "comdef.h"
#include "acpmc.h"
#include "caii.h"
#ifdef FEATURE_UASMS
#include "uasmsi.h"
#endif /* FEATURE_UASMS */

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*  Reasons for entry into CDMA Call Processing.                           */
/*                                                                         */
/*-------------------------------------------------------------------------*/

typedef enum {
  MCC_ACQUISITION,   /* Enter normal CDMA mode of operation */
#ifndef FEATURE_SD20
  MCC_CONTINUE,      /* Continue previous operation (quick reacquisition) */
#endif /* !FEATURE_SD20 */
  MCC_ENTER_OFFLINE  /* Enter offline CDMA mode of operation */
#ifndef FEATURE_SD20
  ,MCC_ORIGINATION   /* Originate call in CDMA mode of operation */
#ifdef FEATURE_OTASP
  ,MCC_ACTIVATION    /* OTASP activation call */
#endif /* FEATURE_OTASP */
#endif /* !FEATURE_SD20 */
} mcc_entry_name_type;

/*-------------------------------------------------------------------------*/
/* General entry parameters.  All members of the 'param' union must have   */
/* These fields first.                                                     */

typedef struct {
  byte             band_class;         /* CDMA band class        */
  word             cdma_channel;       /* Channel to acquire     */
#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
} mcc_entry_gen_type;

#ifndef FEATURE_SD20
/*-------------------------------------------------------------------------*/
/* MCC_ACQUISITION entry parameters.                                       */

typedef enum {
  MCC_ACQ_SID_NID,   /* Acquire any SID+NID pair */
  MCC_ACQ_HOME_SID   /* Acquire only SID+NID found in SID+NID list */
} mcc_sys_acq_enum_type;

typedef struct {
  byte                    band_class;       /* CDMA band class             */
  word                    cdma_channel;     /* Channel to acquire          */
  mcc_sys_acq_enum_type   system_acq;       /* Entry acquisition info      */
  nv_block_or_sys_enum_type block_or_sys;   /* Cellular serv-sys (A or B)
                                               or PCS freq block (A-F)     */
  boolean                 cdma_locked;      /* Set if in LOCK or LOCK_P    */
  word                    idle_timeout;     /* Idle timeout in minutes,    */
                                            /* with 0 meaning 'none'.      */
  boolean                 new_mode_or_sys;  /* Switching from analog mode or
                                               different serving-system, band
                                               class or PCS freq block     */
  boolean                 redirection_enabled; /* Indicates if REDIRECTIONs
                                                  is enabled               */
  word                    expected_sid;     /* Expected SID                */
  word                    expected_nid;     /* Expected NID                */
} mcc_entry_acq_type;

/*-------------------------------------------------------------------------*/
/* MCC_ENTER_OFFLINE entry parameters.                                     */

typedef struct {
  byte                    band_class;       /* CDMA band class        */
  word                    cdma_channel;     /* Channel to acquire     */
} mcc_entry_offline_type;

/*-------------------------------------------------------------------------*/
/* MCC_ORIGINATION entry parameters.                                       */

typedef struct
{
  mcc_entry_acq_type  sys_acq;                 /* System Acquisition   */
  byte cnt;                                    /* How many digits      */
  byte called_address[ NV_MAX_DIAL_DIGITS ];   /* ASCII Called Address */
  word service_option;                         /* Service Option Number*/
#ifdef FEATURE_ACP
  boolean force_amps;               /* Force AMPS call indicator       */
#endif /* FEATURE_ACP */
#ifdef FEATURE_DS
  boolean ds_originated;            /* Source of Orig, UI or DS        */
  boolean amps_data;                /* AMPS only data/voice indicator  */
#endif /* FEATURE_DS */
  boolean digit_mode;               /* TRUE if digits have non-numeric */
  byte number_type;                 /* Number Type in case of ASCII mode */
  byte number_plan;                 /* Number Plan in case of ASCII mode */
#ifdef FEATURE_IS95B_NDSS
  byte return_cause;                /* Failure cause for NDSS Origination */ 
#endif /* FEATURE_IS95B_NDSS */
#if (defined(FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
  mc_orig_type_type orig_type;      /* indicates if orig is for SMS or PD */
  void              *dbm_buf_ptr;   /* pointer to the MO DBM buffer */
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */
} mcc_entry_orig_type;
#endif /* !FEATURE_SD20 */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*  Entry reason and parameters structure.                                 */
/*                                                                         */
/*-------------------------------------------------------------------------*/

typedef struct {
  mcc_entry_name_type            entry;      /* Specifies entry type      */
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
  union {                                    /* Parameters for:           */
    mcc_entry_gen_type           gen;        /* Items in common for all   */
    mcc_entry_acq_type           acq;        /* MCC_ACQUISITION           */
    mcc_entry_acq_type           cont;       /* MCC_CONTINUE              */
    mcc_entry_offline_type       offline;    /* MCC_ENTER_OFFLINE         */
    mcc_entry_orig_type          orig;       /* MCC_ORIGINATION           */
#ifdef FEATURE_OTASP
    mcc_entry_orig_type          activate;   /* MCC_ACTIVATION            */
#endif /* FEATURE_OTASP */
  } param;
#endif /* FEATURE_SD20 */
} mcc_entry_type;

/*-------------------------------------------------------------------------*/
/*                                                                         */
/*  Reasons for exit out of CDMA Call Processing.                          */
/*                                                                         */
/*-------------------------------------------------------------------------*/
typedef enum {
  MCC_RESELECTION,        /* Normal exit indication                        */
  MCC_ACQ_FAILED,         /* Could not acquire CDMA system                 */
#ifndef FEATURE_SD20
  MCC_WRONG_SYSTEM,       /* SIDr not equal to EXPECTED_SID                */
  MCC_WRONG_NETWORK,      /* NIDr not equal to EXPECTED_NID                */
#endif /* !FEATURE_SD20 */
  MCC_RELEASE,            /* Call has been released                        */
  MCC_SYSTEM_LOST,        /* Acquisition lost                              */
  MCC_MAX_ACCESS_PROBES,  /* Max access probes txed with no BS response    */
  MCC_NEW_SYSTEM,         /* Idle Handoff to unknown configuration         */
  MCC_RESCAN,             /* RESCAN field in System Params Msg set to '1'  */
  MCC_REDIRECTION,        /* Service redirection commanded by base station */
  MCC_REG_REJECTED,       /* Registration request rejected by base station */
  MCC_ACCESS_DENIED,      /* Mobile station failed access persistence test */
#ifndef FEATURE_SD20
  MCC_ERROR,              /* Mobile doesn't support assigned CDMA channel  */
  MCC_ACQ_TERMINATED,     /* SID does not match                            */
#endif /* !FEATURE_SD20 */
  MCC_AHO_VOICE,          /* Analog handoff: acquire and talk              */
  MCC_AHO_ORIGINATION,    /* Analog handoff: originate a call              */
  MCC_AHO_PAGE_RESPONSE,  /* Analog handoff: perform a page response       */
  MCC_AHO_WAIT_FOR_PAGE,  /* Analog handoff: wait for a page               */
  MCC_AHO_TC_ASSIGNMENT,  /* Analog handoff: traffic channel assignment    */
  MCC_LOCK,               /* Received LOCK or LOCK_P order                 */
  MCC_UNLOCK,             /* Received Unlock Order                         */
  MCC_INCOMP_CAI_REV,     /* MOB_CAI_REV < MIN_CAI_REV                     */
  MCC_PREF_SYS_CHANGED,   /* System preferences have changed               */
  MCC_NAM_CHANGED,        /* NAM has changed                               */
  MCC_OFFLINE_ANALOG,     /* Received command to go into offline analog    */
  MCC_OFFLINE_DIGITAL,    /* Received command to go into offline digital   */
  MCC_RESET,              /* Received command to reset                     */
  MCC_POWERDOWN,          /* Received command to powerdown                 */
#ifndef FEATURE_SD20
  MCC_ORIG_CMD_RXED,      /* Rxed origination cmd/aborted CDMA acquisition */
  MCC_AUTO_NAM_CHANGE,    /* Auto NAM function indicates NAM switch needed */
#endif /* !FEATURE_SD20 */
  MCC_NOT_PREFERRED       /* Acquired system is not preferred              */
#ifndef FEATURE_SD20
#ifdef FEATURE_OTASP
#ifndef FEATURE_NSOTASP
  ,MCC_PROVISIONING_ORIG
#endif
  ,MCC_PROVISIONING       /* An OTASP origination has been requested       */
  ,MCC_END_ACTIVATION     /* The OTASP origination has terminated          */
#endif /* FEATURE_OTASP */
#endif /* !FEATURE_SD20 */
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
#ifdef FEATURE_IS95B_SYS_RESELEC
  ,MCC_SYSTEM_RESELECTION /* Switch to analog                               */
#endif /* FEATURE_IS95B_SYS_RESELEC */
#ifdef FEATURE_IS95B_NDSS
  ,MCC_NDSS_OFF
#endif /* FEATURE_IS95B_NDSS */
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,MCC_SILENT_REDIAL          /* Silent Redial */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#endif /* FEATURE_SD20 */
} mcc_exit_name_type;

/*-------------------------------------------------------------------------*/
/* Macro MCC_AHO_EXIT_TYPE() returns TRUE if arg is one of the 5 AHO types */

#define MCC_AHO_EXIT_TYPE(arg) \
        (((arg) == MCC_AHO_VOICE) || \
        ((arg) == MCC_AHO_ORIGINATION) || \
        ((arg) == MCC_AHO_PAGE_RESPONSE) || \
        ((arg) == MCC_AHO_WAIT_FOR_PAGE) || \
        ((arg) == MCC_AHO_TC_ASSIGNMENT))

#ifdef FEATURE_SD20 
#error code not present
#else /* FEATURE_SD20 */
/*-------------------------------------------------------------------------*/
/* MCC_AUTO_NAM_CHANGE exit parameters                                     */

typedef struct
{
  word    acq_sid;        /* Currently acquired SID */
  word    acq_nid;        /* Currently acquired NID */
  boolean orig_pending;   /* Indicates if origination was in progress */
  boolean digit_mode;     /* TRUE if digits have non-numeric */
  byte    number_type;    /* Number type in case of ASCII */
  byte    number_plan;    /* Number plan in case of ASCII */
  byte    called_address[NV_MAX_DIAL_DIGITS]; /* ASCII Called Address */
  byte    cnt;                                /* How many digits      */
} mcc_auto_nam_change_type;

/*-------------------------------------------------------------------------*/
/* MCC_REDIRECTION exit parameters                                         */

typedef enum {
  MCC_GLOBAL_REDIRECTION,   /* Global Service Redirection   */
  MCC_DIRECTED_REDIRECTION  /* Directed Service Redirection */
} mcc_redirect_type;

typedef struct
{
  boolean return_if_fail;           /* Return if fail indicator  */
  mcc_redirect_type redirect_type;  /* The type of redirection   */
#ifdef FEATURE_IS95B_NDSS
  boolean ndss_on;                  /* Indicator for NDSS redirection */
  byte cnt;                         /* How many digits           */
  byte called_address [NV_MAX_DIAL_DIGITS];
                                    /* ASCII Called Address      */
  word    service_option;           /* Service option number     */
#endif /* FEATURE_IS95B_NDSS */
  byte record_type;                 /* Redirection record type   */
  union
  {
    caii_redirect_rec1_type rec1;   /* Analog redirection record */
    caii_redirect_rec2_type rec2;   /* CDMA redirection record   */
  } redir;
} mcc_redirection_type;

/*-------------------------------------------------------------------------*/
/* MCC_PROVISIONING exit parameters                                        */

#ifdef FEATURE_OTASP
typedef struct
{
  word         service_option;           /* Service option number          */
  byte         cnt;                      /* How many digits                */
  byte         called_address[MC_MAX_DIALED_DIGITS];  /* Called address    */
  mc_activate_code_type activate_code;   /* Tells where to look for service*/
} mcc_provisioning_type;
#endif /* FEATURE_OTASP */

/*-------------------------------------------------------------------------*/
/* MCC_END_ACTIVATION exit parameters                                      */

#ifdef FEATURE_OTASP
typedef struct
{
  boolean reload_nam;                    /* Tells whether to reload NAM    */
} mcc_end_activation_type;
#endif /* FEATURE_OTASP */

/*-------------------------------------------------------------------------*/
/* MCC_ORIG_CMD_RXED exit parameters                                       */

typedef struct
{
#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
  mc_orig_type_type orig_type;      /* SMS/PD uses the same exit structure,
                                       here is the indication */
  void              *dbm_buf_ptr;   /* Pointer for MO DBM buffer */
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */
  word    service_option;           /* Service option number           */
#ifdef FEATURE_ACP
  boolean force_amps;               /* Force AMPS call indicator       */
#endif /* FEATURE_ACP */
#ifdef FEATURE_DS
  boolean ds_originated;            /* Source of Orig, UI or DS        */
  boolean amps_data;                /* AMPS only data/voice indicator  */
#endif /* FEATURE_DS */
  boolean digit_mode;               /* TRUE if digits have non-numeric */
  byte number_type;                 /* Number Type in case of ASCII mode */
  byte number_plan;                 /* Number Plan in case of ASCII mode */
  byte    cnt;                      /* How many digits                 */
  byte    called_address[MC_MAX_DIALED_DIGITS];  /* Called address     */
} mcc_orig_cmd_rxed_type;

/*-------------------------------------------------------------------------*/
/* MCC_AHO_ORIGINATION exit parameters                                     */

typedef struct
{
  boolean use_analog_sys;  /* Use analog system indicator */
  byte    analog_sys;      /* Analog system indicator     */
  byte    cnt;             /* How many digits             */
  byte    called_address[MC_MAX_DIALED_DIGITS];  /* Called address     */
#ifdef FEATURE_DS
  boolean ds_originated;   /* Source of Orig, UI or DS        */
  boolean amps_data;       /* AMPS only data/voice indicator  */
#endif /* FEATURE_DS */
} mcc_aho_orig_type;

/*-------------------------------------------------------------------------*/
/* MCC_AHO_PAGE_RESPONSE and MCC_AHO_WAIT_FOR_PAGE exit parameters         */

typedef struct
{
  boolean use_analog_sys;  /* Use analog system indicator */
  byte    analog_sys;      /* Analog system indicator     */
} mcc_aho_page_type;


/*-------------------------------------------------------------------------*/
/* MCC_NEW_SYSTEM exit parameters                                          */

typedef struct
{
  byte mode;                  /* The mode: CDMA or analog                  */
  byte band_class;            /* The band class: cellular or PCS           */
  nv_cs_union_type cs;        /* CDMA channel or cellular system           */
} mcc_new_system_type;

/*-------------------------------------------------------------------------*/
/* MCC_SILENT_REDiAL exit parameters                                       */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef struct
{
  mc_sr_name_type type;     /* silent redial type */
  boolean         home_only;/* silent originate on roaming systems or not */
} mcc_sr_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

/*-------------------------------------------------------------------------*/
/* MCC_SYSTEM_LOST exit parameters                                         */

typedef struct
{
  boolean tc_init_failure;    /* Indication of system lost during Traffic  */
                              /* Channel Initialization                                    */
} mcc_system_lost_type;
#endif /* FEATURE_SD20 */

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* Exit structure.                                                         */
/*                                                                         */
/*-------------------------------------------------------------------------*/

/* Note that all cdma-to-analog exit reasons are imported from file        */
/* apcmc.h, where they are defined as entry reasons.                       */

typedef struct {
  mcc_exit_name_type exit;        /* Specifies exit reason                 */
  union {                         /* Parameters for:                       */
#ifdef FEATURE_SD20
#error code not present
#else 
    mcc_auto_nam_change_type auto_nam_change;   /* MCC_AUTO_NAM_CHANGE   */
#endif /* FEATURE_SD20 */
    acp_entry_handoff_type   aho_voice;         /* MCC_AHO_VOICE         */
#ifndef FEATURE_SD20
    mcc_aho_orig_type        aho_origination;   /* MCC_AHO_ORIGINATION   */
    mcc_aho_page_type        aho_page;          /* MCC_AHO_PAGE_RESPONSE */
#endif /* !FEATURE_SD20 */
    acp_entry_chan_type      aho_tc_assignment; /* MCC_AHO_TC_ASSIGNMENT */
#ifndef FEATURE_SD20
    mcc_orig_cmd_rxed_type   orig_cmd_rxed;     /* MCC_ORIG_CMD_RXED     */
    mcc_redirection_type     redirection;       /* MCC_REDIRECTION       */
#ifdef FEATURE_OTASP
    mcc_provisioning_type    provisioning;      /* MCC_PROVISIONING      */
    mcc_end_activation_type  end_activation;    /* MCC_END_ACTIVATION    */
#endif /* FEATURE_OTASP */
#ifdef FEATURE_IS95B_SYS_RESELEC
    caii_analog_nghbr_type   analog_nghbr_lst;  /* MCC_SYSTEM_RESELECTION*/
#endif /* FEATURE_IS95B_SYS_RESELEC   */
    mcc_new_system_type      new_system;        /* MCC_NEW_SYSTEM CR6955 */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
    mcc_sr_type              silent_redial;     /* MCC_SILENT_REDIAL     */   
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
    mcc_system_lost_type     system_lost;       /* MCC_SYSTEM_LOST */
#endif /* !FEATURE_SD20 */
  } param;
} mcc_exit_type;

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION MCC_SUBTASK

DESCRIPTION
    This is the entry procedure for the MCC sub-task. It contains the main
    processing loop for the task.

DEPENDENCIES
    mcc_powerup_init() should be called once on powerup before calling
    this procedure.

RETURN VALUE
    mcc_exit_type - pointer to structure indicating reason for exit

SIDE EFFECTS
    None

===========================================================================*/

extern mcc_exit_type *mcc_subtask
(
  mcc_entry_type *mcc_entry_reason_ptr  /* Reason for entry */
);


/*===========================================================================

FUNCTION MCC_POWERUP_INIT

DESCRIPTION
    This procedure should be called once upon powerup to allow MCC to
    perform internal initialization

DEPENDENCIES
    This procedure loads parameters from NV.  The NV task should be ready
    to receive commands when this procedure is called.

RETURN VALUE
    TRUE  - NV parameters successfully loaded.
    FALSE - NV parameters could not be loaded.

SIDE EFFECTS
    None

===========================================================================*/

extern boolean mcc_powerup_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_POWERUP_LOAD

DESCRIPTION
  This function loads CDMA parameters from NV.

DEPENDENCIES
  None

RETURN VALUE
  Indicator of whether all parameters were successfully read from NV.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean mcc_powerup_load( void );


/*===========================================================================

FUNCTION MCC_OFFLINE

DESCRIPTION
    This procedure should be called when the DMSS is to transition to
    OFFLINE state.  It performs the necessary processing required before
    OFFLINE state.

DEPENDENCIES
    ???

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void mcc_offline (void);


/*===========================================================================

FUNCTION MCC_POWERDOWN

DESCRIPTION
    This procedure should be called once when the DMSS is to power down.
    It performs the necessary processing required before powering down.

DEPENDENCIES
    This procedure saves parameters in NV.  The NV task should be ready
    to receive commands when this procedure is called.

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void mcc_powerdown (void);


#ifndef FEATURE_NSOTASP
/*===========================================================================

FUNCTION MCC_IGNORE_ACTIVATION_CODE

DESCRIPTION
  This function checks the status of the SPC and validity of PRL 
  to decide if it should not use the act_code dialed by user to acquire 
  a system for Activation.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if MS should ignore activation code else FALSE.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean mcc_ignore_activation_code(void);

#endif /* !FEATURE_NSOTASP */

#endif /* MCC_H */

