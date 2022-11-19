#ifndef MCCSYOBJ_H
#define MCCSYOBJ_H
/*===========================================================================

             P A C K E T   Z O N E   I D   S U P P O R T

GENERAL DESCRIPTION
  This module contains supporting functions for packet zone ID.

DESCRIPTION
  This module contains definitions and interfaces for service configuration
  and negotiation for Data Services service options.  This functions are
  created for support of MDR Release 1.0.  Note that once Call Manager is
  implemented, these function may become obsolete.

   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/




/*===========================================================================

                      EDIT HISTORY FOR FILE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/mccsyobj.h_v   1.1   13 Nov 2000 14:41:32   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/13/00   lcc     Added members related to NID in mccsyobj_system_object_type.
11/03/99   lcc     Merged from GD4021:
           doj     Made reg_prev_change_handler, process_prev and get_prev
                   only defined for FEATURE_SPECIAL_MDR.
02/18/99   lcc     Corrected SID to use a word instead of byte.
01/25/99   lcc     Initial release

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "target.h"
#include "customer.h"

#include "caii.h"


/*===========================================================================

                      SOME CONSTANTS USED IN THIS FILE

===========================================================================*/

/*===========================================================================

                      TYPE DEFINITIONS

===========================================================================*/
typedef struct
{
  struct
  {
    // Registers handler for changing packet zone ID when on paging channel
    void ( *reg_idle_change_handler )(
            void ( *pzid_idle_change_handler )( byte old_pzid, byte new_pzid ) );
    // Registers handler for changing packet zone ID when on traffic channel
    void ( *reg_active_change_handler )(
            void ( *pzid_active_change_handler )( byte old_pzid, byte new_pzid ) );
    // Function to call when packet zone ID received on paging channel
    // (changed or not)
    void ( *process_when_idle )( byte pzid );
    // Function to call when packet zone ID received on traffic channel
    // (changed or not)
    void ( *process_when_active )( byte pzid );
    // Function to get current stored value of PZID
    byte ( *get )( void );
  } pzid;

#ifdef FEATURE_SPECIAL_MDR //Only used for Special MDR
  struct
  {
    // Registers handler for changing protocol revision
    void ( *reg_change_handler )(
            void ( *prev_change_handler )( byte old_prev, byte new_prev ) );
    // Function to call when protocol revision received
    void ( *process )( byte prev );
    // Function to get current stored value of PREV
    byte ( *get )( void );
  } prev;
#endif //FEATURE_SPECIAL_MDR
  struct
  {
    // Registers handler for changing SID
    void ( *reg_change_handler )(
            void ( *sid_change_handler )( word old_sid, word new_sid ) );
    // Function to call when SID received
    void ( *process )( word sid );
    // Function to get current stored value of SID
    word ( *get )( void );
  } sid;

  struct
  {
    // Function to call when NID received
    void ( *process )( word nid );
    // Function to get current stored value of NID
    word ( *get )( void );
  } nid;
} mccsyobj_system_object_type;

extern const mccsyobj_system_object_type mccsyobj_system;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION mccsyobj_initialize

DESCRIPTION
  This function initializes the mccsyobj_system object.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccsyobj_initialize
(
  void
);
#endif /* MCCSYOBJ_H */

