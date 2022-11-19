#ifndef MCCAP_H
#define MCCAP_H
/*===========================================================================

          MAIN CONTROL SYSTEM CAPACITY MANAGER

GENERAL DESCRIPTION
   This module is keeping track of the underlying hardware and system capability.
   All other call processing modules look to this module to figure out if a
   particular parameter or configuration is do-able by the mobile.

DESCRIPTION
   This module has data structures to keep track of the capacity info and
   the interfaces to allow for the other modules to query  or validate any
   parameter

   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/mccap.h_v   1.1   29 Nov 2000 17:33:20   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/29/00   lcc/hrk Added prototype for mccap_get_preferred_sch_coding().
07/10/00   va      Added prototype for mccap_get_geo_loc_cap().
06/12/00   va      Removed FEATURE_IS2000_SCH to check the num of
                   supported fwd and rev SCHs.
06/02/00   lcc     Added prototype for mccap_is_r_fch_gating_supported.
04/07/00   va      Added support to retrieve min pwr control step size.
03/06/00   lcc     Corrected some comments and added some SCH related function
                   prototypes.
12/20/99   va      Added mccap_get_ext_mux_option_info for supporting
                   Extended mux option IR.
11/15/99   va      Initial revision of the file.
                   Created for the IS2000 Changes. This file keeps the capabaility
                   database and initializes it correctly based on s/w, h/w
                   capability.
===========================================================================*/
#include "cai.h"
#include "target.h"
#include "customer.h"
#include "caii.h"

#ifdef FEATURE_IS2000
typedef enum
{
   MCCAP_FCH=0,
   MCCAP_DCCH=1,
   MCCAP_FWD_SCH=2,
   MCCAP_REV_SCH=3
} mccap_ch_type;

/*===========================================================================

FUNCTION mccap_check_chind_and_rc

DESCRIPTION
  Given a pair of RCs and chind, this function checks if these RCs are doable
  and compatible with each other for the given channels.

DEPENDENCIES
  None.

RETURN VALUE
  status: true if this is good and false if it is not doable.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mccap_check_chind_and_rc
(
  cai_chind_type chind,
  cai_radio_config_type fwd_rc,
  cai_radio_config_type rev_rc
);

/*===========================================================================

FUNCTION mccap_is_enhanced_operation_supported

DESCRIPTION
   This function returns true if QPCH or any RC class 2 RCs are supported, if
   niether of these 2 is supported, false is returned

DEPENDENCIES
  None.

RETURN VALUE
  status: true or false as described above.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mccap_is_enhanced_operation_supported( void );

/*===========================================================================

FUNCTION mccap_is_mac_supported

DESCRIPTION
  This function returns true if MAC is supported else FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  status: true or false as described above.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mccap_is_mac_supported( void );


/*===========================================================================

FUNCTION mccap_is_otd_supported

DESCRIPTION
   This function returns true if OTD is supported else false.

DEPENDENCIES
  None.

RETURN VALUE
   This function returns true if OTD is supported else false

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mccap_is_otd_supported(void);

/*===========================================================================

FUNCTION mccap_is_qpch_supported

DESCRIPTION
   This function returns true if QPCH is supported else false.

DEPENDENCIES
  initialize function should be called before this.

RETURN VALUE
   This function returns true if QPCH is supported else false

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mccap_is_qpch_supported( void );

/*===========================================================================

FUNCTION mccap_is_dcch_supported

DESCRIPTION
  This function returns true if DCCH is supported else FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  status: true or false as described above.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccap_is_dcch_supported( void );

/*===========================================================================

FUNCTION mccap_is_r_fch_gating_supported

DESCRIPTION
  This function returns true if reverse FCH gating is supported else FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  status: true or false as described above.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccap_is_r_fch_gating_supported( void );

#ifdef FEATURE_IS2000_SCH
/*===========================================================================

FUNCTION mccap_num_f_sch_supported

DESCRIPTION
  This function returns number of F-SCH supported.

DEPENDENCIES
  None.

RETURN VALUE
  See description above.

SIDE EFFECTS
  None.

===========================================================================*/
byte mccap_num_f_sch_supported( void );

/*===========================================================================

FUNCTION mccap_num_r_sch_supported

DESCRIPTION
  This function returns number of R-SCH supported.

DEPENDENCIES
  None.

RETURN VALUE
  See description above.

SIDE EFFECTS
  None.

===========================================================================*/
byte mccap_num_r_sch_supported( void );
#endif // FEATURE_IS2000_SCH

/*===========================================================================

FUNCTION mccap_get_ch_specific_fields

DESCRIPTION
  This function returns the channel specific fields for a given channel

DEPENDENCIES
  None

RETURN VALUE
  status: true if the specified channel is supported else false

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean mccap_get_ch_specific_fields
(
  mccap_ch_type  chan_type,
  void * data_ptr
);

/*===========================================================================

FUNCTION mccap_get_chan_cfg_info

DESCRIPTION
   This function populates the passed in channel config IR

DEPENDENCIES
  initialize function should be called before this.

RETURN VALUE
   None

SIDE EFFECTS
  None.

===========================================================================*/
extern void  mccap_get_chan_cfg_info( caii_chan_cfg_capa_info_type *ccc_ptr);

/*===========================================================================

FUNCTION mccap_get_advanced_capability_fields

DESCRIPTION
   This function populates IS2000 specific fields for
   the capability info record passed in.

DEPENDENCIES
  initialize function should be called before this.

RETURN VALUE
   None

SIDE EFFECTS
  None.

===========================================================================*/
extern void  mccap_get_advanced_capability_fields( caii_capa_rec_type *cap_ptr);

/*===========================================================================

FUNCTION mccap_get_ext_mux_option_info

DESCRIPTION
   This function populates the ext mux option record pointer
   from the system capability info

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
  None.

===========================================================================*/
extern void mccap_get_ext_mux_option_info ( caii_ext_mult_opt_info_type  *rec_ptr);

/*===========================================================================

FUNCTION mccap_get_min_pwr_ctrl_step

DESCRIPTION
   This function returns the minimum pwr ctrl step size that can
   be supported by the underlying MSM

DEPENDENCIES
   None

RETURN VALUE
   Min pwr control step size

SIDE EFFECTS
  None.

===========================================================================*/
extern byte mccap_get_min_pwr_ctrl_step ( void );

/*===========================================================================

FUNCTION mccap_get_geo_loc_cap

DESCRIPTION
   This function returns the GEO location Capability of the MS

DEPENDENCIES
   None

RETURN VALUE
  geo location capability value

SIDE EFFECTS
  None.

===========================================================================*/
extern byte mccap_get_geo_loc_cap ( void );


/*===========================================================================

FUNCTION mccap_get_preferred_sch_coding

DESCRIPTION
  This function returns turbo coding if turbo code is supported; otherwise,
  convoluational coding is returned.  It is assumed that if turbo coding is
  supported, then convolutional coding must also be.  For simplicity, we are
  making the assumption that coding support is uniform across all SCH physical
  channels (0, 1, F, R).

DEPENDENCIES
  The caller has to make sure that SCH is supported.  If this function is called
  when SCH is not supported, the result is unpredictable..

RETURN VALUE
  Coding type.

SIDE EFFECTS
  None.

===========================================================================*/
cai_sch_coding_type mccap_get_preferred_sch_coding( void );

#endif // FEATURE_IS2000
#endif // MCCAP_H
