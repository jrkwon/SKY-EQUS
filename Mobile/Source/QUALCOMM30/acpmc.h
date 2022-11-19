#ifndef ACPMC_H
#define ACPMC_H
/*===========================================================================

                        ACPMC Declarations

DESCRIPTION
   This header file contains the definitions and external interfaces
   to ACP Main Control.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/acpmc.h_v   1.2   16 Feb 2001 13:13:10   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   fc      Added support for System Determination 2.0. All changes
                   are featurized under FEATURE_SD20.
09/11/00   yll     Added support for Position Determination DBM.
03/06/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           kk      analog e911 modifications.
           ram     Merged the following change from the Trimode branch -
           ych     Added support for Tri-mode Silent Redial
10/05/99   lh      Serving System update for CM is added.
08/13/99   lh      CM and SMS phase 2 implementations.
07/30/99   fc      Put in changes based on code review comments on Analog 
                   Registration Randomization.
07/28/99   lh      CM and two-way SMS support added.
07/23/99   fc      Added MAX_REDIRECT_DELAY to ACP_REDIRECTION_TYPE.
06/17/99   ck      Externalised the function acpmc_reload_otasp_nam_info().
05/06/99   kmp     Merge IS-95B changes below and change T_AUTH to FEATURE_AUTH
           kk      ndss_orig flag added to acp_entry_orig_type
03/08/99   br      Added ACP_E911_AMPS_FAILURE_EXIT_ACCELERATOR exit reason.
                   Added acpmc_e911_amps_failure_exit_accelerator_init().
01/05/99   ht      Add ACP_BS_IN_OVLD exit reason.
01/27/98   ldg     FEATURE_DS updates (by hand).
06/27/97   ych     Externalized acpmc_nv_powerdown_write. Also moved
                   acp553.first_idle_id_stat_enabled and acp553.ex_p
                   initialization from acpmc_powerup_init() into acpmc_nv_power
                   up_load().
05/20/97   ych     Added exit reason ACP_LPM for LPM.
04/18/97   dna     Added acpmc_get_otasp_nam_info() for 800/dual band OTASP
02/10/97   lh      Include mc.h.
02/07/97   lh      Added exit reason ACP_OTASP_PROVISIONING and
                   acp_provisioning_type for OTASP origination.
02/06/97   jca     Deleted acp_entry_page_response_type.
01/15/97   jrs     Added ACP_NOT_PREFERRED exit reason.
12/13/96   jca     Modified acp_entry_orig_type & acp_exit_orig_cmd_rxed_type.
10/11/96   jjw     Added AMPS data task initiated call capability.
11/29/95   jca     Deleted acq_sid field from acp_entry_acq_type.
09/14/95   jca     Added initial_entry field to acp_entry_acq_type.
07/20/95   jca     Changes to support Subscriber Access Control.
07/07/95   dna     External declarations for authentication.
06/15/95   jca     Added ACP_AUTO_NAM_CHANGE exit reason.
05/04/93   jca     Added ACP_ORIG_CMD_RXED exit reason.
04/01/93   jca     Added acpmc_enable_first_idle_id_stat() procedure.
12/11/92   JCA     Deleted constant ACP_MAX_DIALED_DIGITS
12/08/92   ARH     Modifications for support of multiple SIDs to acquire
08/26/92   ARH     Updated acpmc_powerup_init() to return boolean status
08/26/92   ARH     Updated per code review comments
06/03/92   JJW     Added acq_sid to entry acquisition type and several other
                   entry and exit items
04/28/92   ARH     Updated to new system determination design
01/24/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"
#include "nv.h"
#include "mc.h"
#ifdef FEATURE_AUTH
#include "auth.h"
#endif
#ifdef FEATURE_OTASP
#include "otaspi.h"
#endif
#include "cmmc.h"
#ifdef FEATURE_UASMS
#include "uasmsi.h"
#endif /* FEATURE_UASMS */

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* ----------------------------------------------------------------------- */
/*  Define reasons for entry into Analog Call Processing [acp_subtask()]   */
/* ----------------------------------------------------------------------- */
typedef enum {
  ACP_ACQUISITION,              /* Enter normal Analog mode of operation   */
  ACP_CDMA_TO_ANALOG_HANDOFF,   /* Digital to Analog handoff               */
#ifndef FEATURE_SD20
  ACP_ORIGINATION,              /* Originate Call in Analog System         */
#endif /* !FEATURE_SD20 */
  ACP_PAGE_RESPONSE,            /* Perform a Page response                 */
  ACP_VOICE_CHAN_ASSIGNMENT,    /* Analog Voice channel assignment         */
#ifndef FEATURE_SD20
  ACP_CONTINUE,                 /* Continue Analog operation (no changes)  */
#endif /* !FEATURE_SD20 */
  ACP_ENTER_OFFLINE             /* Enter analog in OFFLINE mode            */
} acp_entry_name_type;

/* ----------------------------------------------------------------------- */

/*
** Normal Analog Mode Acquisition parameters  (ACP_ACQUISITION)
*/
typedef enum {               /* define system to acquire      */
  ACP_ACQ_SYSTEM_A,          /* A System/Any SID              */
  ACP_ACQ_SYSTEM_B,          /* B System/Any SID              */
  ACP_ACQ_HOME_SID           /* Home System/Home SID          */
} acp_entry_acq_sys_type;

/*
** Service Redirection parameters received from System Determination
*/
typedef struct
{
  boolean enabled;              /* Indicates if REDIRECTIONs is enabled    */
  word    expected_sid;         /* Expected SID                            */
  boolean ignore_cdma;          /* Ignore CDMA Available Message indicator */
#ifdef FEATURE_IS95B
  boolean cdma_mode;  /* Indicates if registration randomization is needed */
  byte    max_redirect_delay;   /* Maximum redirect delay                  */
#endif /* FEATURE_IS95B */
} acp_redirection_type;

typedef struct
{
  acp_entry_acq_sys_type system_acq;      /* System Acquisition info       */
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
  boolean                initial_entry;   /* Initial entry to FM mode      */
  boolean                ignore_cdma_gam; /* Ignore CDMA Global Action Msg */
  word                   idle_timeout;    /* Timeout interval in minutes   */
                                          /* 0 means no timeout specified  */
  acp_redirection_type   redirection;     /* Redirection parameters        */
#endif /* FEATURE_SD20 */
} acp_entry_acq_type;

/* ----------------------------------------------------------------------- */

/*
** CDMA to Analog Handoff parameters (ACP_CDMA_TO_ANALOG_HANDOFF)
*/
typedef enum {
  ACP_HANDOFF_WAIT_FOR_ORDER,      /* Handoff during wait for order state  */
  ACP_HANDOFF_WAIT_FOR_ANSWER,     /* Handoff during wait for answer state */
  ACP_HANDOFF_CONVERSATION         /* Handoff during conversation state    */
} acp_entry_handoff_substate_type;

typedef struct {
  word                            sid;       /* System ID                 */
  byte                            vmac;      /* Power Level               */
  word                            chan;      /* Chanel number             */
  byte                            scc;       /* SAT Color Code            */
  boolean                         mem;       /* Message encryption mode   */
  acp_entry_handoff_substate_type substate;  /* Substate to handoff to    */
} acp_entry_handoff_type;

/* ----------------------------------------------------------------------- */

#ifndef FEATURE_SD20
/* ---------------------------------------------------
** Origination indication parameters (ACP_ORIGINATION)
** --------------------------------------------------- */
typedef struct {
  acp_entry_acq_type        sys_acq;           /* System Acquisition      */
  byte cnt;                                    /* How many digits         */
  byte called_address[ NV_MAX_DIAL_DIGITS ];   /* ASCII Called Address    */
#ifdef FEATURE_DS
  boolean      ds_originated;            /* Source of Orig, UI or DS       */
  boolean      amps_data;                /* AMPS only data/voice indicator */
#endif /* FEATURE_DS */
#ifdef FEATURE_IS95B_NDSS
  boolean      ndss_orig;              /* indicator of an NDSS origination */ 
#endif /* FEATURE_IS95B_NDSS */
} acp_entry_orig_type;
#endif /* !FEATURE_SD20 */

/* ---------------------------------------------------------------------- */

/*
** Analog Voice Channel Assignment parameters (ACP_VOICE_CHAN_ASSIGNMENT)
*/

typedef enum {
  ACP_CHAN_ASSIGNMENT_PAGE,                  /* Perform Page Processing   */
  ACP_CHAN_ASSIGNMENT_ORIG                   /* Perform Origination Proc. */
} acp_entry_chan_assignment_type;

typedef struct {
  word                            sid;       /* System ID                 */
  byte                            vmac;      /* Power Level               */
  word                            chan;      /* Chanel number             */
  byte                            scc;       /* SAT Color Code            */
  boolean                         mem;       /* Message encription mode   */
  acp_entry_chan_assignment_type  proc;      /* Processing to perform     */
} acp_entry_chan_type;

/* ----------------------------------------------------------------------- */
/* Entry Reason Type                                                       */
/* ----------------------------------------------------------------------- */
typedef struct {
  acp_entry_name_type entry;                 /* Specifies entry type      */
  union {                                    /* Parameters for:           */
    acp_entry_acq_type           acq;        /* ACP_ACQUISITION           */
    acp_entry_handoff_type       handoff;    /* ACP_CDMA_TO_ANALOG_HANDOFF*/
#ifndef FEATURE_SD20
    acp_entry_orig_type          orig;       /* ACP_ORIGINATION           */
#endif /* !FEATURE_SD20 */
    acp_entry_chan_type          chan;       /* ACP_VOICE_CHAN_ASSIGNMENT */
#ifndef FEATURE_SD20
    acp_entry_acq_type           cont;       /* ACP_CONTINUE              */
#endif /* !FEATURE_SD20 */
  } param;
} acp_entry_type;

/* ----------------------------------------------------------------------- */
/* Define reasons for exit                                                 */
/* ----------------------------------------------------------------------- */

typedef enum {
  ACP_ACQ_FAILURE,     /* Could not acquire analog system                  */
  ACP_NEW_SYSTEM,      /* SIDr not equal to SIDs                           */
  ACP_RESCAN,          /* Rescan Message received                          */
  ACP_SYSTEM_LOST,     /* The paging channel was lost                      */
#ifndef FEATURE_SD20
  ACP_WRONG_SYSTEM,    /* SIDs not equal to EXPECTED_SID                   */
#endif /* !FEATURE_SD20 */
  ACP_CDMA_AVAILABLE,  /* CDMA is available                                */
  ACP_RESELECTION,     /* Normal exit indication                           */
#ifndef FEATURE_SD20
  ACP_ACQ_TERMINATED,  /* SID does not match                               */
#endif /* !FEATURE_SD20 */
  ACP_PREF_SYS_CHANGED,/* System preferences have changed (A/B, Ana/Dig)   */
  ACP_NAM_CHANGED,     /* NAM has changed (NAM switch)                     */
  ACP_OFFLINE_ANALOG,  /* Command to go into offline analog mode received  */
  ACP_OFFLINE_DIGITAL, /* Command to go into offline digital mode received */
  ACP_RESET,           /* Reset command received (Offline mode only)       */
  ACP_POWERDOWN,       /* Command to powerdown                             */
#ifndef FEATURE_SD20
  ACP_ORIG_CMD_RXED,   /* Rxed origination cmd/aborted analog acquisition  */
#endif /* !FEATURE_SD20 */
  ACP_BS_IN_OVLD       /* Received Overload message from Base Station */
#ifdef FEATURE_PREFERRED_ROAMING
  ,ACP_NOT_PREFERRED   /* Not a preferred system                           */
#endif /* FEATURE_PREFERRED_ROAMING */
#ifndef FEATURE_SD20
  ,ACP_AUTO_NAM_CHANGE  /* Auto NAM function indicates NAM switch required  */
#ifdef FEATURE_OTASP
  ,ACP_OTASP_PROVISIONING/*Got otasp orig cmd return to system determination*/
#ifndef FEATURE_NSOTASP
  ,ACP_ACTIVATION_ORIG     /* Otasp Origination, while there is service     */
#endif /* !FEATURE_NSOTASP */
#endif /* FEATURE_OTASP */
#endif /* !FEATURE_SD20 */
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
#ifndef FEATURE_SD20
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,ACP_SILENT_REDIAL      /* Silent Redial Exit */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#endif /* !FEATURE_SD20 */

} acp_exit_name_type;

/* ----------------------------------------------------------------------- */

#ifndef FEATURE_SD20
/* ----------------------------------------------
** CDMA Available parameters (ACP_CDMA_AVAILABLE)
** ---------------------------------------------- */
typedef struct {
  word cdma_freq;      /* Channel number of the CDMA frequency to acquire  */
} acp_exit_cdma_avail_type;
#endif /* !FEATURE_SD20 */

#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
/* -----------------------------------------------------------
** Origination command received parameters (ACP_ORIG_CMD_RXED)
** ----------------------------------------------------------- */
typedef struct
{
#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
  mc_orig_type_type orig_type;       /* indicates if this is an MO DBM exit*/
  void              *dbm_buf_ptr;        /* Pointer to the MO DBM buffer   */
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */
  word         service_option;           /* Service option number          */
  boolean      force_amps;               /* Force AMPS call indicator      */
#ifdef FEATURE_DS
  boolean      ds_originated;            /* Source of Orig, UI or DS       */
  boolean      amps_data;                /* AMPS only data/voice indicator */
#endif /* FEATURE_DS */
  boolean      digit_mode;               /* TRUE if digits have non-numeric*/
  byte         cnt;                      /* How many digits      */
  byte called_address[ NV_MAX_DIAL_DIGITS ];   /* ASCII Called Address */
} acp_exit_orig_cmd_rxed_type;

/*      ------------------------------------------------
** Auto NAM change parameters (ACP_AUTO_NAM_CHANGE)
** ------------------------------------------------ */
typedef struct
{
  word    acq_sid;        /* currently acquired SID */
  boolean orig_pending;   /* whether a call is pending */
  byte    called_address[NV_MAX_DIAL_DIGITS]; /* ASCII Called Address */
  byte    cnt;                                /* How many digits      */
} acp_exit_auto_nam_change_type;

/* --------------------------------------------------
** ACP_OTASP_PROVISIONING exit parameters
** -------------------------------------------------- */
#ifdef FEATURE_OTASP
typedef struct
{
  word         service_option;           /* Service option number          */
  byte         cnt;                      /* How many digits                */
  byte         called_address[MC_MAX_DIALED_DIGITS];  /* Called address    */
  mc_activate_code_type activate_code;   /* Tells where to look for service*/
} acp_exit_provisioning_type;
#endif /* FEATURE_OTASP */
#endif /* FEATURE_SD20 */

#ifndef FEATURE_SD20
/* --------------------------------------------------
** ACP_SILENT_REDIAL exit parameters
** -------------------------------------------------- */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef struct
{
  mc_sr_name_type type;     /* silent redial type */
  boolean         home_only;/* silent originate on roaming systems or not */         
} acp_exit_sr_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

typedef struct
{
  boolean release_ind; /* indicaton of reselection due to user
                          or base station release */
} acp_exit_reselection_type;  
#endif /* !FEATURE_SD20 */

/* ----------------------------------------------------------------------- */
/* Define parameters to the reasons for exit                               */
/* ----------------------------------------------------------------------- */
typedef struct
{
  acp_exit_name_type exit;                         /* Specifies exit reason*/
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
  union {                                          /* parameters for:      */
    acp_exit_cdma_avail_type      cdma_avail;      /* ACP_CDMA_AVAILABLE   */
    acp_exit_orig_cmd_rxed_type   orig_cmd_rxed;   /* ACP_ORIG_CMD_RXED    */
    acp_exit_auto_nam_change_type auto_nam_change; /* ACP_AUTO_NAM_CHANGE  */
    acp_exit_reselection_type     reselection;     /* ACP_RESELECTION      */
#ifdef FEATURE_OTASP
    acp_exit_provisioning_type    provisioning;    /*ACP_OTASP_PROVISIONING*/
#endif /* FEATURE_OTASP */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
    acp_exit_sr_type              silent_redial;   /* MCC_SILENT_REDIAL    */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
  } param;
#endif /* FEATURE_SD20 */
} acp_exit_type;

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPMC_SUBTASK

DESCRIPTION
  This is the entry procedure for the ACPMC sub-task. It contains the main
  processing loop for the task.

DEPENDENCIES
  acpmc_powerup_init() should be called once on powerup before calling
  this procedure.

RETURN VALUE
  acp_exit_type    ** Pointer to structure indicating reason for exit **

SIDE EFFECTS
  None

===========================================================================*/
extern acp_exit_type *acpmc_subtask
(
  acp_entry_type *entry_reason_ptr
    /* Reason for entry into the analog mode of operation */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_POWERUP_INIT

DESCRIPTION
  This procedure should be called once upon powerup to allow ACPMC to
  perform internal initialization

DEPENDENCIES
  This procedure loads parameters from NV.  The NV task should be ready
  to receive commands when this procedure is called.

RETURN VALUE
  TRUE  - Initialization processing was successful
  FALSE - Initialization processing was NOT successful

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmc_powerup_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_OFFLINE

DESCRIPTION
  This procedure should be called when the DMSS is to transition to
  OFFLINE state.  It performs the necessary processing required before
  OFFLINE state.

DEPENDENCIES
  This procedure saves parameters in NV.  The NV task should be ready
  to receive commands when this procedure is called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmc_offline( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_POWERDOWN

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
extern void acpmc_powerdown( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_ENABLE_FIRST_IDLE_ID_STAT

DESCRIPTION
  This procedure sets the First-Idle ID status to enabled.

DEPENDENCIES
 This routine is only called during CDMA operation when the mobile
 station has successfully registered on the CDMA base station.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmc_enable_first_idle_id_stat( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_NV_POWERUP_LOAD

DESCRIPTION
  This function retrieves the analog permanent and semi-permanent items from
  NV and sets the '553' data structure accordingly.

  It reads all the analog-related NV items one at a time.  The boolean
  variable 'nv_get' determines when to load from NV and when to set the
  corresponding '553' numeric indicator.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - Powerup load was successful
  FALSE - Powerup load was not successful

SIDE EFFECTS
  None

===========================================================================*/

extern boolean acpmc_nv_powerup_load( void );

#ifdef FEATURE_OTASP
/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_GET_OTASP_NAM_INFO

DESCRIPTION
  This procedure is called from CDMA mode to retrieve stored NAM values
  needed for the OTASP Configuration Response Message (sent on CDMA Traffic
  Channel).

DEPENDENCIES
  This routine is only called during CDMA operation.

RETURN VALUE
  firstchp_p, home_sid_p, and ex_p from the acp553 structure are placed into
  the structure whose pointer is passed in as the parameter.

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmc_get_otasp_nam_info
(
  otaspi_rtc_cell_data_type *cell
    /* Cellular Indicator Data for OTASP Configuration Response Message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_RELOAD_OTASP_NAM_INFO

DESCRIPTION
  This procedure is called from CDMA mode to update the ACP NAM values when
  they have been committed to NV through OTASP.

DEPENDENCIES
  This routine is only called during CDMA operation.

RETURN VALUE
  None.
  
SIDE EFFECTS
  Updates Analog Home Sid and First Paging channel

===========================================================================*/
extern void acpmc_reload_otasp_nam_info
(
  nv_otasp_commit_type *commit_info 
);

#endif /* FEATURE_OTASP */

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_GET_STATE

DESCRIPTION
  Returns the current state for DIAG.

DEPENDENCIES
  None.

RETURN VALUE
  Current state

SIDE EFFECTS
  None.
===========================================================================*/

extern byte acpmc_get_state( void );

#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================

FUNCTION ACPMC_QUEUE_AUTH_RPT

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

extern void acpmc_queue_auth_rpt
(
  auth_rpt_type  *rpt_ptr
    /* Pointer to authentication report */
);
#endif /* FEATURE_AUTH */

/*===========================================================================

FUNCTION ACPMC_NV_POWERDOWN_WRITE

DESCRIPTION
  This function writes the analog semi-permanent items to NV.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmc_nv_powerdown_write( void );



/*==========================================================================

FUNCTION ACPMC_REGISTERED

DESCRIPTION
  This function compares the value sid_r in acp553 and the sid in DB.
  If both are non-zero and identical to each other.  That indicates that
  the mobile has registered.  The function will return TRUE, otherwise,
  it returns FALSE.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if mobile is registered

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean acpmc_registered(void);

/*==========================================================================

FUNCTION ACPMC_SRV_SYS_INFO_RPT

DESCRIPTION
  This function reports the serving system info to CM. In AMPS, Band class
  is always 0. System A or B is derived from SID.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void acpmc_srv_sys_info_rpt( word sid );

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */
#endif /* ACPMC_H */

