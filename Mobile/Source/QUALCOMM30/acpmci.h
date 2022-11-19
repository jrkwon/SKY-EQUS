#ifndef ACPMCI_H
#define ACPMCI_H
/*===========================================================================

                        ACPMCI Declarations

DESCRIPTION
   This header file contains the definitions and INTERNAL interfaces
   to ACP Main Control.

Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1995,1996,1997 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/acpmci.h_v   1.3   16 Feb 2001 16:04:58   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   fc      Added support for system determination 2.0. All changes 
                   are featurized under FEATURE_SD20.
02/16/01   sh      added support for MC_END_F ack
09/11/00   yll     Added support for Position Determination DBM.
04/06/00   va      Merged the following from PP (MSM3100_CP.03.00.17):
           vn      Added extern acpmcini_sid_accept
03/06/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           kk      analog e911 modifications.
           ych     Added suport for Trimode silent redial
08/13/99   lh      CM and SMS phase 2 implementations.
07/28/99   lh      CM and two-way SMS support added.
05/06/99   kmp     Merge IS-95B changes below and change T_AUTH to FEATURE_AUTH
           kk      ndss_orig flag added to acpmc_origination param type.
03/08/99   br      Added ACPMC_EXIT_E911_AMPS_FAILURE_EXIT_ACCELERATOR
                   exit reason.
01/05/99   ht      Add ACPMC_BS_IN_OVLD exit reason.
09/04/98   pms     Neared the variables to save ROM space.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
05/20/97   ck      Added ACPMC_EXIT_LPM and ACPMC_SA_LPM_PWR_DOWN_REG
02/10/97   lh      Added ACPMC_EXIT_OTASP_PROVISIONING.
01/15/97   jrs     Added ACPMC_EXIT_NOT_PREFERRED.
09/17/96   jca     Added support for idle timeout cumulative timer.
06/27/96   jca     Added ACP_EXIT_IDLE_TIMEOUT exit reason.
05/24/96   jjw     Gemini & ISS2 Data Services release.
12/18/95   jca     Added extern declarations for shared buffers.
09/14/95   jca     Added acpmc_acquisition_param_type.
07/20/95   jca     Changes to support Subscriber Access Control.
07/07/95   dna     Added external declarations for authentication support.
06/15/95   jca     Added ACP_EXIT_AUTO_NAM_CHANGE exit reason.
05/04/93   jca     Added ACPMC_EXIT_ORIG_CMD_RXED exit reason
12/11/92   JCA     Changed ACP_MAX_DIALED_DIGITS --> NV_MAX_DIAL_DIGITS
08/26/92   ARH     Updated per code review comments
04/29/92   ARH     Updated entry/exit reasons
01/30/92   ARH     Added signal definitions and updated timer functions
01/24/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "cmmc.h"
#include "nv.h"
#include "mc.h"
#include "acpcmdef.h"
#include "acprx.h"
#include "acptx.h"
#include "acpmc.h"
#include "acp553.h"
#ifdef FEATURE_AUTH
#include "auth.h"
#endif
#ifdef FEATURE_DS     /* if Data Services are compiled */
#include "ds.h"
#endif

/* <EJECT>  */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* ------------------------------------------------------------------------

   State Information definition: Contains information about the different
   reason for entries in the ACPMC state machines (INIT, IDLE, SA, VC, EXIT).
   This information is derived from the CAI as well as implementation
   specific additions.

   ----------------------------------------------------------------------- */

/* ------------------------------------------------------------------------
   State names
   ------------------------------------------------------------------------ */
typedef enum {
  ACPMC_INIT_STATE,                      /* Initialization State       */
  ACPMC_IDLE_STATE,                      /* Idle State                 */
  ACPMC_SA_STATE,                        /* System Access State        */
  ACPMC_VC_STATE,                        /* Voice Channel State        */
  ACPMC_EXIT_STATE,                      /* Exit State                 */
  ACPMC_OFFLINE_STATE                    /* Offline State              */
} acpmc_state_name_type;


/* ------------------------------------------------------------------------
   Reasons for entry
   ------------------------------------------------------------------------ */

typedef enum {                         /* ACP Initialization State  */
  ACPMC_INIT_RETRIEVE_SYS_PARMS,       /* Enter INIT at retrieve sys parms  */
  ACPMC_INIT_PAGING_CHAN_SELECT,       /* Enter INIT at paging chan select. */
#ifndef FEATURE_SD20
  ACPMC_INIT_ORIGINATION,              /* Do acquisition and originate      */
#endif /* !FEATURE_SD20 */
  ACPMC_INIT_PAGE_RESPONSE             /* Do acquisition and page resp.     */
} acpmc_init_entry_reason_type;


typedef enum {                         /* ACP Idle State                    */
  ACPMC_IDLE_WAIT                      /* Enter IDLE (no special processing)*/
} acpmc_idle_entry_reason_type;


typedef enum {                         /* ACP System Access State   */
  ACPMC_SA_ORIGINATION,                /* Origination                       */
  ACPMC_SA_PAGE_RESPONSE,              /* Page Response                     */
  ACPMC_SA_ORDER_RESPONSE,             /* Order Response                    */
  ACPMC_SA_REGISTRATION,               /* Registration                      */
  ACPMC_SA_PWR_DOWN_REG,               /* Power Down Registration           */
  ACPMC_SA_NAM_CHANGED_REG,            /* Power down reg. due to nam change */
  ACPMC_SA_BASE_CHALLENGE,             /* Base Challenge                    */
  ACPMC_SA_SERV_SYS_DETERMINATION      /* Enter Serv-Sys Determination Task */
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */

} acpmc_sa_entry_reason_type;


typedef enum {                            /* ACP Voice Channel State        */
  ACPMC_VC_IVCD_ORIGINATION,              /* Origination indication         */
  ACPMC_VC_IVCD_PAGE,                     /* Page indication                */
  ACPMC_VC_CDMA_HANDOFF                   /* Handoff from CDMA              */
} acpmc_vc_entry_reason_type;


typedef enum {                            /* ACP Exit State                 */
  ACPMC_EXIT_ACQ_FAILURE,                 /* Could not acquire analog       */
  ACPMC_EXIT_NEW_SYSTEM,                  /* SIDr not equal to SIDs         */
  ACPMC_EXIT_RESCAN,                      /* Rescan Message received        */
  ACPMC_EXIT_SYSTEM_LOST,                 /* Paging Channel was lost        */
  ACPMC_EXIT_WRONG_SYSTEM,                /* SIDs not equal to EXPECTED_SID */
  ACPMC_EXIT_CDMA_AVAILABLE,              /* CDMA is available              */
  ACPMC_EXIT_RESELECTION,                 /* Normal exit indication         */
  ACPMC_EXIT_IDLE_TIMEOUT,                /* Idle State timeout has expired */
  ACPMC_EXIT_ACQ_TERMINATED,              /* SID not accepted by user prefs */
  ACPMC_EXIT_PREF_SYS_CHANGED,            /* Preferred system has changed   */
  ACPMC_EXIT_NAM_CHANGED,                 /* Nam has changed                */
  ACPMC_EXIT_OFFLINE_ANALOG,              /* ACP received OFFLINE Analog    */
  ACPMC_EXIT_OFFLINE_DIGITAL,             /* ACP received OFFLINE Digital   */
  ACPMC_EXIT_RESET,                       /* ACP rxed RESET (offline only)  */
  ACPMC_EXIT_POWERDOWN,                   /* ACP received POWERDOWN cmd     */
  ACPMC_EXIT_ORIG_CMD_RXED,               /* Rxed valid orig cmd during acq */
  ACPMC_EXIT_BS_IN_OVLD                  /* Rcvd Ovld msg from BS */
#ifdef FEATURE_PREFERRED_ROAMING
  ,ACPMC_EXIT_NOT_PREFERRED               /* Not a preferred system         */
#endif /* FEATURE PREFERRED_ROAMING */
  ,ACPMC_EXIT_AUTO_NAM_CHANGE             /* Auto NAM indicates NAM change  */
#ifdef FEATURE_OTASP
  ,ACPMC_EXIT_OTASP_PROVISIONING          /* Got otasp orig cmd             */
#ifndef FEATURE_NSOTASP
  ,ACPMC_EXIT_ACTIVATION_ORIG            /* OTASP Orig while MS has service */
#endif
#endif /* FEATURE_OTASP */
#ifdef FEATURE_LPM
#error code not present
#endif /* FEATURE_LPM */
#ifndef FEATURE_SD20
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
  ,ACPMC_SILENT_REDIAL      /* Silent Redial Exit */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#endif /* !FEATURE_SD20 */
} acpmc_exit_entry_reason_type;


typedef union {                           /* Union of reason types     */
  acpmc_init_entry_reason_type init;
  acpmc_idle_entry_reason_type idle;
  acpmc_sa_entry_reason_type   sa;
  acpmc_vc_entry_reason_type   vc;
  acpmc_exit_entry_reason_type exit;
} acpmc_reason_type;

/* ------------------------------------------------------------------------
   Parameters
   ------------------------------------------------------------------------ */

#ifndef FEATURE_SD20
typedef struct
{
  boolean initial_entry;            /* TRUE if initial entry to analog mode */
} acpmc_acquisition_param_type;
#endif /* !FEATURE_SD20 */

typedef struct {                    /* Originations                         */
  byte count;                       /* Number of digits in the array        */
  byte called_address[NV_MAX_DIAL_DIGITS];  /* Number to call               */
#ifndef FEATURE_SD20
#ifdef FEATURE_IS95B_NDSS
  boolean ndss_orig;                /* indicator for an NDSS origination    */
#endif /* FEATURE_IS95B_NDSS */
#endif /* !FEATURE_SD20 */
} acpmc_origination_param_type;

typedef struct {                    /* SA Page Response */
  boolean one_word_page;            /* TRUE= MS control msg was 1 word long */
} acpmc_page_response_param_type;


typedef struct {              /* SA orders and other responses             */
  boolean extended_protocol;  /* Indicates extended protocol msg response  */
  byte    order_field;        /* Value to place in ORDER field (or MST field
                                 for extended protocol message responses)  */
  byte    ordq_field;         /* Value to place in ORDQ field              */
  byte    msg_type_field;     /* Value to place in MSG_TYPE field          */
} acpmc_order_response_param_type;


typedef struct {
  word sid;                         /* System ID                            */
  byte vmac;                        /* Voice Mobile Attenuation Code        */
  word chan;                        /* Channel Number                       */
  byte scc;                         /* SAT Color Code                       */
  boolean mem;                      /* Message encryption mode              */
} acpmc_ivcd_param_type;            /* Initial Voice Channel Designation    */

typedef acp_entry_handoff_type
  acpmc_cdma_handoff_param_type;    /* Handoff from CDMA parameters         */

#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
typedef acp_exit_cdma_avail_type    /* Channel number of CDMA Available msg */
  acpmc_exit_cdma_param_type;

typedef acp_exit_orig_cmd_rxed_type   /* Called Address info from Orig cmd  */
  acpmc_exit_orig_cmd_rxed_type;

typedef acp_exit_auto_nam_change_type /* NAM info from Auto NAM function    */
  acpmc_exit_auto_nam_change_type;

#ifdef FEATURE_OTASP
typedef acp_exit_provisioning_type    /*  otasp orig infomation */
  acpmc_exit_provisioning_type;
#endif /* FEATURE_OTASP */

#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
typedef acp_exit_sr_type              /* Silent redial information */
  acpmc_exit_sr_type;
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */

typedef acp_exit_reselection_type
  /* indicaton of reselection due to user or base station release */
  acpmc_exit_reselection_type;   
#endif /* FEATURE_SD20 */


/* ------------------------------------------------------------------------
   This is the full State Information type
   ------------------------------------------------------------------------ */
typedef struct {
  acpmc_state_name_type state;     /* New state to enter                    */
  acpmc_reason_type     reason;    /* Reason for entering new state         */
  union
  {
#ifndef FEATURE_SD20
    acpmc_acquisition_param_type    acq;             /* Acquisition params  */
#endif /* !FEATURE_SD20 */
    acpmc_origination_param_type    orig;            /* Origination params  */
    acpmc_page_response_param_type  page_resp;       /* Page resp params    */
    acpmc_order_response_param_type order_resp;      /* Order resp params   */
    acpmc_ivcd_param_type           ivcd;            /* IVCD parameters     */
    acpmc_cdma_handoff_param_type   cdma_ho;         /* CDMA handoff params */
#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */
    acpmc_exit_cdma_param_type      cdma_exit;       /* CDMA exit params    */
    acpmc_exit_orig_cmd_rxed_type   orig_cmd_rxed;   /* Called address      */
    acpmc_exit_auto_nam_change_type auto_nam_change; /* NAM ID params       */
    acpmc_exit_reselection_type     reselection;     /* Reselection         */
#ifdef FEATURE_OTASP
    acpmc_exit_provisioning_type    provisioning;    /* OTASP orig info     */
#endif /* FEATURE_OTASP */
#if defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900)
    acpmc_exit_sr_type              silent_redial;   /* silent redial type  */
#endif /* defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) */
#endif /* FEATURE_SD20 */
  } param;
} acpmc_state_info_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ----------------------
** Idle timeout variables
** ---------------------- */
#define ONE_MINUTE 60000
  /* The number of milliseconds for a one minute interval.  Used for
     the idle timeout entry condition. Allows for time intervals
     to be set in multiples of one minute. */

extern word acpmc_timeout_minutes;
  /* Contains the number of minutes left until an idle timeout occurs */
extern word acpmc_timeout_msecs;
  /* Contains the current expiration count for the idle timeout rex timer */

extern acp553_rxmsg_type acpmc_rx_msg;
  /* Buffer for rx message */
extern acprx_cmd_rpt_type acpmc_acprx_rpt;
  /* Buffer for command report from the ACPRX task */
extern acptx_cmd_rpt_type acpmc_acptx_rpt;
  /* Buffer for command report from the ACPTX task */
extern acprx_cmd_type acpmc_acprx_cmd;
  /* Buffer for command to ACPRX task */
extern acptx_cmd_type acpmc_acptx_cmd;
  /* Buffer for command to ACPTX task */
extern cm_mc_rpt_type acpmc_cm_cmd;
  /* Buffer for command to the UI task */
extern mc_msg_type acpmc_mc_cmd;
  /* Buffer for retrieving command to ACPMC task */

#ifdef FEATURE_DS     /* if Data Services are compiled */
extern ds_cmd_type acpmc_ds_cmd;
  /* Buffer for sending command to DS task */
#endif

extern boolean acpmcini_sid_accept;

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

This procedures are 'regional' procedures internal to analog call processing

===========================================================================*/

/*===========================================================================

FUNCTION ACPMCI_START_TIMER

DESCRIPTION
  Starts the timer associated with the given signal. The timer will expire
  after 'interval' (in msecs) and the corresponding signal will be raised.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_start_timer
(
  acp_mask_type mask_sig,      /* Timer signal                        */
  word interval                /* Timer expiration interval in msecs  */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CANCEL_TIMER

DESCRIPTION
  This function cancels the timer and clears the signal associated with it.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_cancel_timer
(
  acp_mask_type mask_sig      /* Signal identifier of the timer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CHECK_TIMER

DESCRIPTION
  This function checks the timer associated with the given mask signal.
  If the timer has already expired (is inactive) a value of '0' is
  returned. If the timer is active, the remaining time to expiration in
  msecs is returned

DEPENDENCIES
  None

RETURN VALUE
  If the timer has not expired, the remaining time to expiration
  is returned (in msecs).
  If the timer has expired the return value will be zero (0).

SIDE EFFECTS
  None

===========================================================================*/
extern word acpmci_check_timer
(
  acp_mask_type mask_sig     /* Signal identifier of the timer */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_GET_CMD

DESCRIPTION
  This function retrieves the first command available in the ACPMC
  command queue and places it on the address specified by the parameter
  'cmd_ptr'. The command is removed from the command queue.

DEPENDENCIES
  The command queue and types are shared betweeen ACP and CDMA.

RETURN VALUE
  TRUE    A command was found and placed in 'cmd_ptr'
  FALSE   The command queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_get_cmd
(
  mc_msg_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CHECK_CMD

DESCRIPTION
  This function places the first command available in the ACPMC
  command queue on the address specified by the parameter
  'cmd_ptr'.
  The command is NOT removed from the command queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A command was found and placed in 'cmd_ptr'
  FALSE   The command queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_check_cmd
(
   mc_msg_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_GET_ACPRX_RPT

DESCRIPTION
  This function retrieves the first command available in the ACPMC
  command report queue for ACPRX and places it on the address specified
  by the parameter 'cmd_ptr'.
  The command is removed from the command queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A command report was found and placed in 'cmd_ptr'
  FALSE   The command report queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_get_acprx_rpt
(
  acprx_cmd_rpt_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CHECK_ACPRX_RPT

DESCRIPTION
  This function checks the first command available in the ACPMC
  command report queue for ACPRX and places it on the address specified
  by the parameter 'cmd_ptr'.
  The command is NOT removed from the command queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A command was found and placed in 'cmd_ptr'
  FALSE   The command queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_check_acprx_rpt
(
  acprx_cmd_rpt_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_GET_ACPTX_RPT

DESCRIPTION
  This function retrieves the first command available in the ACPMC
  command report queue for ACPTX and places it on the address specified
  by the parameter 'cmd_ptr'.
  The command is removed from the command queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A command report was found and placed in 'cmd_ptr'
  FALSE   The command report queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_get_acptx_rpt
(
  acptx_cmd_rpt_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CHECK_ACPTX_RPT

DESCRIPTION
  This function checks the first command available in the ACPMC
  command report queue for ACPTX and places it on the address specified
  by the parameter 'cmd_ptr'.
  The command is NOT removed from the command queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A command was found and placed in 'cmd_ptr'
  FALSE   The command queue is empty. 'cmd_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_check_acptx_rpt
(
  acptx_cmd_rpt_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_GET_ACPRX_MSG

DESCRIPTION
  This function retrieves the first 553 message available in the ACPMC
  message queue and places it on the address specified by the parameter
  'msg_ptr'.
  The message is removed from the message queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A message was found and placed in 'msg_ptr'
  FALSE   The message queue is empty. 'msg_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_get_acprx_msg
(
  acp553_rxmsg_type *msg_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_CHECK_ACPRX_MSG

DESCRIPTION
  This function checks the first 553 message available in the ACPMC
  message queue and places it on the address specified by the parameter
  'msg_ptr'.
  The message is NOT removed from the message queue.

DEPENDENCIES
  None

RETURN VALUE
  TRUE    A message was found and placed in 'msg_ptr'
  FALSE   The message queue is empty. 'msg_ptr' is not modified.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpmci_check_acprx_msg
(
  acp553_rxmsg_type *msg_ptr
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_RECEIVE_ACPRX_RPT

DESCRIPTION
  This function performs the role of copying the report buffer pointed
  to by 'rpt_ptr' to the ACPMC ACPRX rpt queue and signaling the report
  arrival (ACPMC_ACPRX_RPT_Q_SIG).

DEPENDENCIES
  ** NOTE: THIS FUNCTION IS CALLED DIRECTLY FROM ANOTHER TASK.
  ** NO MAJOR FUNCTIONALITY SHOULD BE ADDED TO THIS FUNCTION OTHER
  ** THAN FOR SIGNALING PURPOSES.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_receive_acprx_rpt
(
  acprx_cmd_rpt_type *rpt_ptr         /* Pointer to command report */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_RECEIVE_ACPTX_RPT

DESCRIPTION
  This function performs the role of copying the report buffer pointed
  to by 'rpt_ptr' to the ACPMC ACPTX rpt queue and signaling the report
  arrival (ACPMC_ACPTX_RPT_Q_SIG).

DEPENDENCIES
  ** NOTE: THIS FUNCTION IS CALLED DIRECTLY FROM ANOTHER TASK.
  ** NO MAJOR FUNCTIONALITY SHOULD BE ADDED TO THIS FUNCTION OTHER
  ** THAN FOR SIGNALING PURPOSES.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_receive_acptx_rpt
(
  acptx_cmd_rpt_type *rpt_ptr         /* Pointer to command report */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_RECEIVE_ACPRX_MSG

DESCRIPTION
  This function performs the role of copying the msg buffer pointed
  to by 'msg_ptr' to the ACPMC ACPRX MSG queue and signaling the report
  arrival (ACPMC_ACPRX_MSG_Q_SIG).

DEPENDENCIES
  ** NOTE: THIS FUNCTION IS CALLED DIRECTLY FROM ANOTHER TASK.
  ** NO MAJOR FUNCTIONALITY SHOULD BE ADDED TO THIS FUNCTION OTHER
  ** THAN FOR SIGNALING PURPOSES.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_receive_acprx_msg
(
  acp553_rxmsg_type *msg_ptr             /* Pointer to msg buffer      */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_WAIT

DESCRIPTION
  This function performs a wait for a specified mask. It will not
  return to the caller until one of the signals specified on the
  mask matches. This function takes care of informing the watchdog
  task that we are still alive.

DEPENDENCIES
  None

RETURN VALUE
  Current signals set. At least one of the signal bits matches
  the specified mask.

SIDE EFFECTS
  None

===========================================================================*/
extern acp_mask_type acpmci_wait
(
  acp_mask_type mask             /* mask to wait for */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_GET_NV_ITEM

DESCRIPTION
  This function gets a specific item from NV.
  NOTE: This call contains a wait call!

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - Load was successful
  FALSE - Load was not successful

SIDE EFFECTS
  None

===========================================================================*/

extern boolean acpmci_get_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get */
  nv_item_type        *item_ptr        /* Pointer where to put the item */
);

#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_AUTH_CMD

DESCRIPTION
  This function sends a command to the Authentication Task then waits
  for the task to signal back that the command has been received.

DEPENDENCIES
  Depends on all command dependent fields of the command block being
  filled out properly.

RETURN VALUE
  None.

SIDE EFFECTS
  Uses command timer.  Could make call to ERR_FATAL.

===========================================================================*/

extern void acpmci_auth_cmd
(
  auth_cmd_type *cmd_ptr
    /* pointer to command to send to Transmit task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_RESYNC_AUTH

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
extern void acpmci_resync_auth( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_QUEUE_AUTH_RPT

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

extern void acpmci_queue_auth_rpt
(
  auth_rpt_type  *rpt_ptr
    /* Pointer to authentication report */
);
#endif /* FEATURE_AUTH */

#ifndef FEATURE_SD20
#ifdef FEATURE_OTASP
#ifndef FEATURE_NSOTASP
/*===========================================================================
FUNCTION ACPMC_PROCESS_OTASP_ORIG

DESCRIPTION
  This function processes the passed Origination command and determines
  if MS should exit from ACP to handle this  OTASP call. If it needs to
  exit, then this function sets the exit parameters and returns TRUE.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if MS should exit, else FALSE.

SIDE EFFECTS
  If returned TRUE, the state_info  parametres are changed to set the proper 
  exit parameters.

  When returned FALSE, and the calling function is in ACPMC IDLE, the cur_state
  parameters are changed in order to make an automatic Origination.
===========================================================================*/
boolean acpmc_process_otasp_orig(mc_msg_type * orig_cmd_msg,
                                 acpmc_state_info_type *state_info,
                                 acpmc_state_info_type * cur_state
                                );
#endif /* !FEATURE_NSOTASP */
#endif /* FEATURE_OTASP */
#endif /* !FEATURE_SD20 */

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_SEND_RPT_TO_CM

DESCRIPTION
  This function performs the role of copying the msg buffer pointed
  to by 'msg_ptr' to the ACPMC UI free queue and sending the command
  to the UI task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpmci_send_rpt_to_cm
(
  cm_mc_rpt_type *msg_ptr             /* Pointer to msg buffer (command)   */
);



#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_SEND_MO_DBM_STATUS

DESCRIPTION
  This function sends a report to the CM task for a mobile originated data
  burst message status.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void acpmci_send_mo_dbm_status
(
  byte burst_type, 
    /* Data Burst Message type, currently, it supports only SMS and PD */
  cm_dbm_mo_status_e_type err_code
    /* Mobile originated data burst message error code */
);
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_SEND_ORIG_FAILURE

DESCRIPTION
  This function sends a report to the CM task for a mobile originated SMS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void acpmci_send_orig_failure(cm_call_orig_fail_e_type err_code);

#ifndef FEATURE_SD20

/* <EJECT> */
/*===========================================================================

FUNCTION ACPMCI_SEND_CALL_END_ACK

DESCRIPTION
  This function sends an Ack the CM task for the MC_END_F command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void acpmci_send_call_end_ack(void);
#endif /* !FEATURE_SD20 */


#endif /* ACPMCI_H */

