#ifndef MCCCCL_H
#define MCCCCL_H
/*===========================================================================

     C O D E   C H A N N E L   L I S T   M A I N T E N A N C E

GENERAL DESCRIPTION
  This module maintains the code channel list according to IS95B section 6.6.8.

DESCRIPTION
  This module contains definitions and interfaces for maintaining the code
  channel list.

   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/




/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mccccl.h_v   1.3   07 Mar 2001 17:07:30   fchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/07/01   fc      Moved function pilot_is_in_active_set outside
                   FEATURE_IS2000_SCH. 
03/06/01   jq      Added aset_info logging to replace aset_config loggin.
08/23/00   lcc     1. For IS2000 only, changed function generate_aset_default_sups
                      to include sup_state in parameter.
                   2. Added prototype for function mark_pilot_as_invalid.
07/20/00   fc      Used enumeration type for SCH rate.
06/12/00   lcc     Corrected some featurization on FEATURE_IS2000_SCH/FEATURE_IS2000.
04/13/00   lcc     Added support for SCH (forward and reverse).
03/12/00   ry      Added IS2000 featurization
03/06/00   lcc     Added partial support for SCH management.
11/15/99   va      IS2000 Changes. Added support for QOF.
11/03/99   lcc     Merged from GD4021:
           ry      Added members get_pilots, save, and restore to
                   mccccl_code_channel_list_object_type
           ry      Moved all occurrences of update_all's out of the
                   #ifdef(FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR) block
09/20/99   lcc     Mainlined update_all.
08/03/99   lcc     Added descriptions of functions and removed function returns
                   from some functions as a result of code review.
                   Also added sup_pilots_are_valid function and some miscellaneous
                   cleanup.
06/30/99   kmp     Removed #includes of msg.h and err.h since they are not
                   needed in this header file.
05/19/99   doj     Chaged GHDM pilot type (caii_gen_ho_pilot_type) reference
                   to generic handoff type (caii_ho_pilot_type).
03/26/99   lcc     Added member get_number_of_supplementals to
                   mccccl_code_channel_list_object_type.
02/12/99   lcc     Added complete support for special MDR.
01/26/99   lcc     Initial release.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "caii.h"
#ifdef FEATURE_IS2000
  #include "mccsch.h"
  #include "mctrans.h"
  #include "log.h"
#endif
#include "srch.h"


/*===========================================================================

                      SOME CONSTANTS USED IN THIS FILE

===========================================================================*/
// Reserve twice the number of active pilots so that potential pilots can be
// stored (in case of SCAM linked to GHDM).
#define CCL_SIZE  (CAI_N6M*2)

/*===========================================================================

                      TYPE DEFINITIONS

===========================================================================*/
// State of pilot entry in CCL.
// INVALID - pilot entry does not contain a valid pilot
// SUP_ONLY - pilot entry contains only supplemental code channel (SCCH) info.
//            Not used for SCH.
// ACTIVE - pilot entry contains a valid pilot (in active set, although supplemental channels
//          may not be up yet)
typedef enum
{
  MCCCCL_INVALID,
  MCCCCL_SUP_ONLY,
  MCCCCL_ACTIVE
} mccccl_pilot_state_type;

// This update mode is used for function update_all only.  It is used to indicate
// if all of the CCL or just the pilots are to be updated.
typedef enum
{
  MCCCCL_UPDATE_PILOTS,
  MCCCCL_UPDATE_ALL
} mccccl_update_mode_type;

#ifdef FEATURE_IS2000
// Supplemental Channel (SCH or SCCH) state
typedef enum
{
  MCCCCL_SCCH,
  MCCCCL_SCH
} mccccl_sup_state_type;

// Information contained in a SCCL entry
typedef struct
{
  uint8 num_pilots;       // number of pilots in the active set of this SCH
  cai_sch_rate_type sch_rate;
  struct
  {
    uint16  pilot_pn;
    word    code_chan_sch;
    byte    qof_mask_id_sch;
  } pilots[CAI_N6M];
} mccccl_sccl_entry_type;
#endif /* FEATURE_IS2000 */

// Structure used by ECAM/CAM/HDM/EHDM to pass pilot information into CCL and
// to generate active set info to be passed to searcher.
// See below for description of the following "member" functions of
// mccccl_code_channel_list_object_type
typedef struct
{
  byte (*get_pilots)( caii_pilot_rec_type pilot_array[] );

  #ifdef FEATURE_IS2000
  byte (*generate_aset)( srch_tc_aset_type* aset,
                         mccccl_sup_state_type* sup_state,  // This parameter added for IS2000
                         boolean use_sups );
  byte (*generate_aset_default_sups)( srch_tc_aset_type* aset,
                                      mccccl_sup_state_type* sup_state );
  #else
  byte (*generate_aset)( srch_tc_aset_type* aset, boolean use_sups );
  byte (*generate_aset_default_sups)( srch_tc_aset_type* aset );
  #endif

  #ifdef FEATURE_IS2000
  void (*initialize)( byte band_class, word cdma_channel,
                                    byte num_pilots, caii_ext_ch_ind3_pilot_rec_type pilot_array[] );
  void (*update_pilots)( byte band_class, word cdma_channel,
                                       byte num_pilots, caii_ext_ch_ind3_pilot_rec_type pilot_array[] );
  #else
  void (*initialize)( byte band_class, word cdma_channel,
                                    byte num_pilots, caii_pilot_rec_type pilot_array[] );
  void (*update_pilots)( byte band_class, word cdma_channel,
                                       byte num_pilots, caii_pilot_rec_type pilot_array[] );
  #endif
  void (*update_all)( byte band_class, word cdma_channel,
                                    byte num_pilots, mccccl_update_mode_type update_mode,
                                    byte num_sup, caii_ho_pilot_type pilot_info[] );

#if defined(FEATURE_IS95B_MDR)
  boolean (*sup_pilots_are_valid)( byte num_sup_pilots,
                                   caii_scam_var_type pilot_info[] );
  void (*update_sups)( byte num_sup_pilots, byte num_sup,
                       caii_scam_var_type pilot_info[] );
#endif

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  byte (*generate_aset_sups)( srch_tc_sup_type* sups, boolean use_sups );
  byte (*get_number_of_supplementals)( void );
#endif

#ifdef FEATURE_IS95B_MAHHO
  void (*save)( void );
  void (*restore)( void );
#endif /* FEATURE_IS95B_MAHHO */

#ifdef FEATURE_IS2000
  void (*update_ccl_with_uhdm)( caii_ho_msg_type* uhdm );
  byte (*get_aset_info)(dm_active_set_type *aset, byte *fsch_rate, dm_aset_rsch_type *rsch);
#endif  // FEATURE_IS2000
#ifdef FEATURE_IS2000_SCH
  void (*update_ccl_with_escam)( caii_escam_type* escam );
  boolean (*generate_sch_aset)( mctrans_srch_f_sch_action_type* sch_aset );
  void (*activate_f_sch_assign)( uint8 sch_id, uint8 sccl_index );
  void (*activate_r_sch_assign)( uint8 sch_id, cai_sch_rate_type sch_rate );
  void (*deactivate_f_sch)( uint8 sch_id );
  void (*deactivate_r_sch)( uint8 sch_id );
  const mccccl_sccl_entry_type* (*get_sccl_entry)(uint8 sch_id, uint8 index );
  uint8 (*get_r_sch_walsh)(uint8 sch_id, uint8 rate );
#endif
  boolean (*pilot_is_in_active_set)( uint16 pilot_pn );
} mccccl_code_channel_list_object_type;

extern const mccccl_code_channel_list_object_type mccccl_code_channel_list;


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION mccccl_initialize

DESCRIPTION
  Performs some initialization for the code channel list object.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccccl_initialize
(
  void
);

/*===========================================================================

      DESCRIPTIONS OF FUNCTIONS IN mccccl_code_channel_list_object_type

===========================================================================*/

/*===========================================================================

FUNCTION initialize

DESCRIPTION
  Initialize the code channel list with pilots and fundamental code channels.
  This function is called after receiving channel assignment (CAM or ECAM) from
  the base station.

DEPENDENCIES
  None.

RETURN VALUE
  MCCCCL_SUCCESS if CCL initialized.  MCCCCL_FAILURE if invalid num_pilots passed in.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION update_sups

DESCRIPTION
  This function updates the supplemental code channels of each pilots in CCL
  according to IS95B.  This is called upon receiving the SCAM with
  FOR_SUP_CONFIG = 10 or 11.


DEPENDENCIES
  None.

RETURN VALUE
  MCCCCL_SUCCESS if CCL updated.  MCCCCL_FAILURE if num_sup_pilots or num_sup are out of range.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION update_pilots

DESCRIPTION
  This function updates the pilots in CCL according to IS95B.  The supplemental
  channels for pilots that are not deleted are not affected.  This function is used
  by EHDM or HDM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION update_all

DESCRIPTION
  This function is used by GHDM to update the complete CCL or just the pilots in
  CCL (in this case it it similar to update_pilots).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION GENERATE_ASET

DESCRIPTION

  This function reads the code channel list and generates the active
  set information which can be sent to SRCH. This fucntion should be
  called only if the entire active set needs to be updated. If only
  supplemental channel information is needed use generate_aset_sups.

  Note that only the following parameters are updated for each
  active set member, - the pilot pn, the fundamental code channel, and
  if requested, the array of all active supplemental code channels.
  The other fields of the srch_tc_aset_type structure needs to be updated
  by the calling function.

DEPENDENCIES
  None

RETURN VALUE

  The number of active pilots in the code channel list. (Will be zero if
  no pilots are active).

SIDE EFFECTS

  None.

===========================================================================*/


/*===========================================================================

FUNCTION GENERATE_ASET_SUPS

DESCRIPTION

  This function reads the code channel list and generates the supplemental
  channel information that can be sent to SRCH using the tc_sup command.
  It will work only if an MDR call is up and the service connection has been
  made.

DEPENDENCIES

  FEATURE_IS95B_MDR or FEATURE_SPECIAL_MDR must be turned on. An MDR call must be
  up and the service connection should be made.

RETURN VALUE

  The number of active pilots currently in the ccl.

SIDE EFFECTS

  None.

===========================================================================*/


/*===========================================================================

FUNCTION sup_pilots_are_valid

DESCRIPTION

  This function checks the pilots specified in the input array to see if they
  are in the current active.

DEPENDENCIES

  None.

RETURN VALUE

  FALSE if any of specified pilots are not in the active set; TRUE otherwise.
  Note: if no pilot or too many pilots are specified, it returns FALSE.

SIDE EFFECTS

  None.

===========================================================================*/


/*===========================================================================

FUNCTION get_number_of_supplementals

DESCRIPTION
  This function returns the number of supplemental channels that are currently
  active.  If the supplemental channels exist in the CCL but are not active, 0
  is returned.

DEPENDENCIES
  NONE

RETURN VALUE
  The number of supplemental channels that are active.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION GENERATE_ASET_DEFAULT_SUPS

DESCRIPTION
  This function is similar to generate_aset except that the parameter "use_sups"
  is defaulted to the internal stored value of sups_active in CCL.  Essentially,
  it updates the active set without affecting the processing of supplementals.
  It is used by HDM, EHDM and GHDM.

DEPENDENCIES
  NONE

RETURN VALUE
  The number of active pilots currently in the ccl.

SIDE EFFECTS
  None.

===========================================================================*/

/*===========================================================================

FUNCTION GET_PILOTS

DESCRIPTION
  This function reads the code channel list and returns all the active pilots.
  Note that the pilot_array that is passed in is assumed to be big enough
  (CAI_N6M) to store all the returned pilots; otherwise, memory scribbling
  will occur

DEPENDENCIES
  NONE

RETURN VALUE
  The number of active pilots currently in the ccl.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION SAVE

DESCRIPTION
  This function saves the code channel list by copying the internal data
  structure CCL into a separate structure called CCL_OLD

DEPENDENCIES
  NONE

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/


/*===========================================================================

FUNCTION RESTORE

DESCRIPTION
  This function restores the code channel list by copying CCL_OLD into CCL

DEPENDENCIES
  NONE

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/



#endif // MCCCCL_H

