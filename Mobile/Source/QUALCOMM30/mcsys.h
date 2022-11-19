#ifndef MCSYS_H
#define MCSYS_H
/*===========================================================================

       M C    S Y S T E M    D E T E R M I N A T I O N    H E A D E R

DESCRIPTION
  This file contains system determination module declarations.

Copyright (c) 1990,1991,1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/mcsys.h_v   1.1   18 Dec 2000 16:57:54   htung  $

when       who     what, where, why
--------   ---     -------------------------------------------------------
12/18/00   ht      Add params sid/nid/band_class to mcsys_cdma_continue().
07/12/00   kk      added new function prototype mcsys_connect_rxed()
04/06/00   va      Merged with PP (MSM3100_CP.03.00.17)
03/16/00   vn      Merged fix for CR7346.
05/12/99   kmp     Replaced T_ACP with FEATURE_ACP
01/20/99   ck      Added enum type for results returned by the function
                   mcsys_validate_roaming_list
11/24/98   br      Added prototype for mcsys_set_curr_nam().
08/21/98   ych     Renamed mcsys_validate_cdma_avail_gam() to mcsys_ignore
                   _cdma_avail_gam().
08/06/98   ych     Added prototype for mcsys_reset_idle_timeout(). Also fixed
                   some lint errors.
08/04/98   ych     Merged E911, SSPR-800, and Acq. Improvements. Also added
                   SSPR CDMA available GAM processing support.
06/17/98   wjn     Added Feature E911 Support.
03/20/98   abh     Enhancements for Home Only
02/02/98   abh     Merged changes for PRL Force Mode.
01/28/98   wjn     Add function prototype mcsys_get_prl_version().
08/18/79   ych     Extended support for the all the targets to ignore a global
                   service redirection until all the over head messages are
                   received. Previously, only targets with FEATURE_PREFERRED_
                   ROAMING defined used to do this.
08/06/97   jrs     Added two new functions for preferred roaming targets:
                   mcsys_cdma_get_roam_ind() and mcsys_analog_get_roam_ind().
06/06/97   jrs     Removed the function prototype for mcsys_activate_ok().
01/15/97   jrs     Added functions mcsys_continue_in_analog() and
                   mcsys_set_last_analog_system().
12/13/96   jca     Added function mcsys_analog_pref_sys().
11/13/96   jrs     Added mcsys_validate_roaming_list().
09/13/96   dna     Added functions for OTASP activation origination, etc.
09/05/96   jrs     Added mcsys_system_selected().
08/26/96   dna     Added mcsys_get_rl_stats()
06/27/96   jca     Added function mcsys_acp_reset().
06/24/96   jrs     Added support for Preferred Roaming.
06/30/95   jca     Added function mcsys_get_request_mode().
06/15/95   jca     Code updates to support Auto NAM processing.
12/01/94   ghd     Changed formal param of mcsys_cdma_continue to void.
06/21/93   jca     Internalized mc_state_enum_type.
08/10/93   jca     Added function mcsys_acp_continue() and
                   mcsys_cdma_continue().
06/30/93   jca     Added function mcsys_set_last_cdmach().
09/09/92    ip     Added reporting of entry reasons to analog and CDMA.
08/11/92    ip     Initial release.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "comdef.h"
#include "db.h"
#include "mc.h"
#include "acpmc.h"
#include "mcc.h"
#ifdef FEATURE_OTASP
#include "otaspi.h"
#endif /* FEATURE_OTASP */

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Last entry reasons to analog and digital.                               */

typedef struct {
  acp_entry_name_type    analog;
  mcc_entry_name_type    cdma;
} mcsys_entry_type;

#ifdef FEATURE_E911
typedef enum {
  MCSYS_NON_E911_SVC_ACQ,         /* Non-E911 Service Acquisition         */
  MCSYS_E911_ORIG_SVC_ACQ,        /* E911 Origination service acquisition */
  MCSYS_E911_CALLBACK_SVC_ACQ     /* E911 Callback Service Acquisition    */
} mcsys_svc_acq_mode_type;
#endif /* FEATURE_E911 */

#ifdef FEATURE_PREFERRED_ROAMING
#ifdef FEATURE_OTASP
/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_GET_RL_STATS

DESCRIPTION
  This function returns information about the Preferred Roaming List for
  OTASP configuration request processing.

DEPENDENCIES
  None.

RETURN VALUE
  Fills in structure with MAX_PR_LIST_SIZE, CUR_PR_LIST_SIZE, NUM_ACQ_RECS,
  and NUM_SYS_RECS.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcsys_get_rl_stats
(
  otaspi_roam_siz_type *rl_siz
    /* Structure to store Roaming List stats */
);
#endif /* FEATURE_PREFERRED_ROAMING */
#endif /* FEATURE_OTASP */

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_SYSTEM_SELECTED

DESCRIPTION
  This function returns whether or not a system has been selected. MCCIDL
  uses this return value to decide if redirection should be honored. If
  a system has not been selected, redirection is ignored.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if a system has been selected.
  FALSE if a system has not been selected.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsys_system_selected(void);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_SET_SYSTEM_SELECTED

DESCRIPTION
  This function sets the system selected flag to TRUE or FALSE depending on
  argument passed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcsys_set_system_selected
(
  boolean system_status
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_ENTRY

DESCRIPTION
  This function returns a pointer to a structure containing the last
  analog (ACP) and digital (MCC) call processing entry reason.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the entry reason structure.

SIDE EFFECTS
  None

===========================================================================*/

extern mcsys_entry_type *mcsys_get_entry( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_SYSTEM_DETERMINATION

DESCRIPTION
  This function performs system determination.  Based on entry action
  and next state read from NV, either analog or digital subtask is entered,
  and either in online state or offline state.  Everytime control returns
  from these subtasks this function decides what next subtask to call.
  This function returns to caller only if and when the exit reason requires
  transition to powerdown or reset.  Transition to offline is handled here
  and either analog or digital subtasks are called in offline with fixed
  entry parameters.

DEPENDENCIES
  None

RETURN VALUE
  The action code causing the exit.

SIDE EFFECTS
  None

===========================================================================*/

extern mc_name_type mcsys_system_determination
(
  mc_name_type  ui_action
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_SET_LAST_CDMACH

DESCRIPTION
  This function is called when and if CDMA acquires the designated system
  and frequency.  This allows system determination to record the last
  CDMA channel acquired.  This will be either the primary or secondary
  channel on either serving system A or serving system B.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcsys_set_last_cdmach( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_ACP_CONTINUE

DESCRIPTION
  This function is called from analog call processing to determine if
  control should be passed back to system determination when the analog
  exit reason is set to "normal exit."

DEPENDENCIES
  None

RETURN VALUE
  TRUE if we are operating in the preferred system and we can remain in
  analog call processing mode; otherwise FALSE.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean mcsys_acp_continue( void );

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_ACP_RESET

DESCRIPTION
  This function is called from analog call processing to reset the analog
  mode entry parameters when a determination has been made from the
  analog call processing layer NOT to return to system determination but
  rather to continue on in analog mode regardless of the system preference
  settings.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mcsys_acp_reset( boolean service_acquired );

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_CDMA_CONTINUE

DESCRIPTION
  This function is called from CDMA call processing to determine if
  control should be passed back to system determination when the
  CDMA exit reason is set to "normal exit."

DEPENDENCIES
  None

RETURN VALUE
  TRUE if we are operating in the preferred system and we can remain in
  CDMA call processing mode; otherwise FALSE.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean mcsys_cdma_continue (word sid, word nid, byte band_class);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_ANALOG_NEED_SWITCH_NAM

DESCRIPTION
  This function determines if the user allows for Auto NAM Change and
  returns False if the user does not.  If the user does, the function
  will determine whether the currently acquired System ID (SID) is the
  Home SID in the current NAM.  If it is, no NAM change will be
  required.  Otherwise, this function will compare the acquired SID
  with the Home SIDs in the other NAMs.  If there is a match, the
  function will return True.

DEPENDENCIES
  Local mcsys_nv_analog_home_sid provides the home SID of current NAM.
  Local mcsys_nv_curr_nam provides the ID of current NAM.

RETURN VALUE
  TRUE  if there is a need to change to a new NAM.
  FALSE if there is no need to change NAM.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsys_analog_need_switch_nam
(
  word acq_sid         /* Currently acquired SID */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_CDMA_NEED_SWITCH_NAM

DESCRIPTION
  This function determines if the user allows for Auto NAM Change and
  returns False if the user does not.  If the user does, this function
  will determine whether the currently acquired SID-NID pair is the
  Home SID-NID in the current NAM.  If it is, no NAM change will be
  required.  Otherwise, this function will compare the acquired SID-NID
  with the Home SID-NIDs in the other NAMs.  If there is a match, the
  function will return True.
  A match is defined here as: either the acquired SID-NID is equal to
  the current NAM's home SID-NID, or the acquired SID is equal to Home
  SID and Home NID is 65535 (wild card).

DEPENDENCIES
  Local mcsys_nv_sid_nid_pair provides the home SID-NID of current NAM.
  Local mcsys_nv_curr_nam provides the ID of current NAM.

RETURN VALUE
  TRUE  if there is a need to change to a new NAM.
  FALSE if there is no need to change NAM.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsys_cdma_need_switch_nam
(
  word acq_sid,      /* Currently acquired SID */
  word acq_nid       /* Currently acquired NID */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_GET_REQUEST_MODE

DESCRIPTION
  This function returns the REQUEST_MODE code based on the user mode
  preference for the current NAM.

DEPENDENCIES
  None

RETURN VALUE
  REQUEST_MODE code as specified in IS-95A Table 6.7.1.3.2.4-1 corresponding
  to user mode preferences for the current NAM.

SIDE EFFECTS
  None

===========================================================================*/

extern byte mcsys_get_request_mode( void );

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
/*===========================================================================

FUNCTION MCSYS_CONTINUE_IN_CDMA_IDLE

DESCRIPTION
  This function is called by idle_state() once all overhead messages have
  been received, hold origination has been set to FALSE, the service
  indicator has been set, and the digital indicator has been set. Based on
  the acquired SID/NID, this function decides whether service should
  continue on this system, or if control should return to system
  determination so that another channel can be scanned.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if CDMA operation should continue in the Idle State.
  FALSE if control should return to system determination.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcsys_continue_in_cdma_idle
(
  word sid,           /* SID of acquired system */
  word nid            /* NID of acquired system */
);
/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_VALIDATE_ROAMING_LIST

DESCRIPTION
  This function validates the passed roaming list. It is called by OTASP
  and the diag task just before writing a roaming list to NV so that a
  valid/invalid indication can be saved with it. When a roaming list is
  read from NV (i.e. powerup, NAM change) the valid flag is used to
  determine if it is valid, and if not, the phone is placed in the offline
  state.

  The following validations are made:

    * The number of acquisition records must be greater than 0.
    * The number of system records must be greater than 0.
    * The type field of each acquisition record must be valid.
    * The a_b field of each cellular acquisition record must not equal the
      reserved value.
    * The block field of each PCS CDMA Using Blocks record must not equal the
      reserved value.
    * The chan field of each PCS CDMA Using Channels record must be between
      25 and 1175 (inclusive).
    * The GEO field of the first system record must indicate that this is the
      start of a new geographical region.
    * The acq_index field of each system record must be less than the number
      of acquisition records.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the roaming list is valid; FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
/* This enumerated type lists the types of results returned while
** validating the roaming list
*/
typedef enum
{                              
  PRL_INVALID_CRC = 0,         /* Invalid CRC  */
  PRL_INVALID_NUM_OF_RECORDS,  /* Invalid number of records */
  PRL_INVALID_RECORD,          /* Invalid record */
  PRL_INVALID_SIZE,            /* Invalid size of the roaming list */
  PRL_VALID,                   /* Valid PRL */
  PRL_MAX_TYPE
} prl_validation_enum_type;

prl_validation_enum_type mcsys_validate_roaming_list(byte *roaming_list);

#endif /* FEATURE_PREFERRED_ROAMING */

#ifdef FEATURE_ACP
/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_ANALOG_PREF_SYS

DESCRIPTION
  This function returns the Analog serving-system indicator corresponding
  to the analog preferred system for the current NAM.

DEPENDENCIES
  None

RETURN VALUE
  0 - Analog System A
  1 - Analog System B

SIDE EFFECTS
  None

===========================================================================*/

extern byte mcsys_analog_pref_sys( void );

#endif /* FEATURE_ACP */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYS_CONTINUE_IN_ANALOG

DESCRIPTION
  This function is called by verify_overhead_info_substate() once all
  overhead messages have been received, hold origination has been set to
  FALSE, the service indicator has been set, and the digital indicator has
  been extinguiehed. Based on the acquired SID, this function decides whether
  service should continue on this system, or if control should return to
  system determination so that another system can be scanned.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if operation should continue in analog mode.
  FALSE if control should return to system determination.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcsys_continue_in_analog
(
  word sid            /* SID of acquired system */
);

extern boolean mcsyspr_sid_is_acceptable( word sid);

#endif /* T_ACP */
#endif /* FEATURE_PREFERRED_ROAMING */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYS_SET_LAST_ANALOG_SYSTEM

DESCRIPTION
 This function is called by verify_overhead_info_substate() after the
 decision has been made to continue to provide service on a system. This
 allows system determination to record the acquired analog cellular system
 in the MRU table.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcsys_set_last_analog_system(void);

#endif /* FEATURE_ACP */
#endif /* FEATURE_PREFERRED_ROAMING */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
/*===========================================================================

FUNCTION MCSYS_CDMA_GET_ROAM_IND

DESCRIPTION
  This function sets the roaming indicator based on the passed SID/NID.

DEPENDENCIES
  None.

RETURN VALUE
  The roaming indicator value for the passed system.

SIDE EFFECTS
  None.

===========================================================================*/

db_roam_type mcsys_cdma_get_roam_ind
(
  word sid,     /* SID of the acquired system */
  word nid      /* NID of the acquired system */
);
#endif /* FEATURE_PREFERRED_ROAMING */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYS_ANALOG_GET_ROAM_IND

DESCRIPTION
  This function gets the roaming indicator based on the passed SID.

DEPENDENCIES
  None.

RETURN VALUE
  The roaming indicator value for the passed system.

SIDE EFFECTS
  None.

===========================================================================*/

db_roam_type mcsys_analog_get_roam_ind
(
  word sid      /* SID of the acquired system */
);

#endif /* FEATURE_ACP */
#endif /* FEATURE_PREFERRED_ROAMING */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING
/*===========================================================================

FUNCTION MCSYS_GET_PRL_VERSION

DESCRIPTION
  This function returns the PRL version stored in local RAM during the power
  up.

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

word mcsys_get_prl_version(void);
#endif /* FEATURE_PREFERRED_ROAMING */

/* <EJECT> */
#ifdef FEATURE_PREFERRED_ROAMING

#if defined(FEATURE_PRL_FORCE_MODE) || defined(FEATURE_HOME_ONLY) || \
    defined(FEATURE_E911)
/*===========================================================================

FUNCTION MCSYS_PREF_MODE_VALID

DESCRIPTION
  Checks the PRL & MRU to verify, if the preferred mode can be changed
  to the user  preferred value successfully

DEPENDENCIES
  None.

RETURN VALUE
  True: if a successfull mode change can be made by the current MRU & PRL
        entries.
  False: Otherwise.
SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mcsys_pref_mode_valid(nv_mode_enum_type mode);
#endif /* FEATURE_PRL_FORCE_MODE || FEATURE_HOME_ONLY || FEATURE_E911 */

#endif /* FEATURE_PREFERRED_ROAMING */

/*===========================================================================

FUNCTION MCSYS_PROCESS_PREF_SYS_CHANGE

DESCRIPTION
   This function kicks off the Pref Sys Change Exit Processing

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
boolean mcsys_process_pref_sys_change
(
  nv_mode_enum_type mode,              /* New preferred mode */
  nv_sys_pref_enum_type sys            /* New system preference */
);

/* <EJECT> */
#ifdef FEATURE_E911

/*===========================================================================

FUNCTION MCSYS_E911_ENTER_CALLBACK

DESCRIPTION
  This function is called by Call Processing modules to notify System
  Determination that an E911 origination has succeeded and the phone is
  entering E911 Callback Waiting Mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcsys_e911_enter_callback(void);


/*===========================================================================

FUNCTION MCSYS_E911_ENTER_CALLBACK

DESCRIPTION
  This function is called by Call Processing modules to notify System
  Determination that an E911 origination has succeeded and connect message
  is received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mcsys_e911_connect_rxed(void);


/* <EJECT> */
/*===========================================================================

FUNCTION MCSYS_CUR_SVC_ACQ_MODE

DESCRIPTION
   This functions returns the current service acquisiton mode.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
mcsys_svc_acq_mode_type mcsys_cur_svc_acq_mode(void);

#endif /* FEATURE_E911 */

#ifdef FEATURE_SSPR_ENHANCEMENTS
#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYS_ANALOG_LOCKED_SIDS

DESCRIPTION
   Called from analog to determine if any SIDs are forbidden for analog.

DEPENDENCIES
  None.

RETURN VALUE
   TRUE If there is at least one forbidden SID
   FALSE otherwise.

SIDE EFFECTS
   None.

===========================================================================*/
boolean mcsys_analog_locked_sids (void);
#endif /* FEATURE_ACP */

/*===========================================================================

FUNCTION MCSYS_SYSTEM_FORBIDDEN

DESCRIPTION
   Called when the SID is first received in the overhead messages
   on an analog or CDMA system.  Checks to see if the system is forbidden.

DEPENDENCIES
  None.

RETURN VALUE
   TRUE If the system is supposed to be locked out from the user,
   FALSE otherwise.

SIDE EFFECTS
   None.

===========================================================================*/
boolean mcsys_system_forbidden
(
  byte mode,          /* The mode of the acquired system */
  word sid,           /* The SID of the acquired system */
  word nid            /* The NID of the acquired system */
);
#endif /* FEATURE_SSPR_ENHANCEMENTS */


#if defined (FEATURE_SSPR_800) && defined(FEATURE_SSPR_ENHANCEMENTS)
/*===========================================================================

FUNCTION MCSYS_IGNORE_CDMA_AVAIL_GAM

DESCRIPTION
  This function is called by analog call processing. This function determines
  if the CDMA available GAM needs to be further processed or ignored

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if CDMA available gam is to be ignored
  FALSE - if CDMA available gam is to be processed

SIDE EFFECTS
  None

===========================================================================*/

boolean mcsys_ignore_cdma_avail_gam(word chan);
#endif /* (FEATURE_SSPR_800) && (FEATURE_SSPR_ENHANCEMENTS) */

#ifdef FEATURE_ACP
/*===========================================================================

FUNCTION MCSYS_RESET_IDLE_TIMEOUT

DESCRIPTION
  This function checks to see if there is more desirable AMPS system

DEPENDENCIES
  None.

RETURN VALUE
  0 - If there is a more desirable AMPS system
  1 - If there is not a more desirable AMPS system

SIDE EFFECTS
  None.

===========================================================================*/

byte mcsys_reset_idle_timeout(void);
#endif /* FEATURE_ACP */

/*===========================================================================

FUNCTION MCSYS_SET_CURR_NAM

DESCRIPTION
  This function is called to save the new NAM when the NAM is changed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  mcsyspr_new_nam_after_chg modified.

===========================================================================*/

extern void mcsys_set_curr_nam
(
  byte new_nam                  /* new NAM that should be set */
);

/*===========================================================================

FUNCTION MCSYS_GET_CURR_NAM

DESCRIPTION
  This function returns value of the current NAM. If the function is 
  called for the first time it reads NAM from the NV, otherwise reads
  the latest set NAM from the memory. 

DEPENDENCIES
  None.

RETURN VALUE
  Current NAM value.

SIDE EFFECTS
  mcsys_new_nam_after_chg and mcsys_curr_nam can be modified.

===========================================================================*/

extern byte mcsys_get_curr_nam (void);


#if defined(FEATURE_OTASP) && !defined(FEATURE_NSOTASP)
/*===========================================================================

FUNCTION MCSYS_GET_SERVING_SYS_OR_BLOCK

DESCRIPTION
  Returns the Cellular System/PCS Block of the serving system.

DEPENDENCIES
  None.

RETURN VALUE
  mcsyspr_system_selected is returned. 
  If mcsyspr_system_selected = True, Block/Sys of the serving system is returned 
  in serving_sys

SIDE EFFECTS
  None.
===========================================================================*/
boolean mcsys_get_serving_sys_or_block(nv_block_or_sys_enum_type *serving_sys);
/*===========================================================================

FUNCTION MCSYS_MC_ENTER_ACTIVATION

DESCRIPTION
 Called by CDMA/Analog Call Processing if it decides to proceed with the
 ACTIVATION Origination without exitting to System Determination.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  mcsyspr_scan mode is changed to MCSYSPR_SCAN_PROVISIONING.
===========================================================================*/
void mcsys_mc_enter_activation(byte mode,
                          mc_activate_code_type activate_code,
                          word service_option,
                          byte dialed_digit_count,
                          char * dialed_digits);
                         
#endif /* FEATURE_OTASP && !FEATURE_NSOTASP */

#endif /* MCSYS_H */
