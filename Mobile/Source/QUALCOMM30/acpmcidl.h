#ifndef ACPMCIDL_H
#define ACPMCIDL_H

/*===========================================================================

                       A C P M C I D L   H E A D E R

GENERAL DESCRIPTION
  This header file contains the definitions and declarations needed to 
  interface with the ACPMCIDL Unit of the ACP sub-system


       Copyright (c) 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcidl.h_v   1.0   Aug 11 2000 11:40:40   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/03/92   jjw     Creation

===========================================================================*/


#include "acpmci.h"

/*===========================================================================

                       PUBLIC DATA DECLARATIONS 

===========================================================================*/






/*===========================================================================

                   PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*==========================================================================

   FUNCTION ACPMCIDL_STATE
  
   DESCRIPTION
     This function is the entry point and main processing function for the 
     IDLE state of the Analog Call Processing sub-system. The passed state 
     info structure contains the information necessary to perform IDLE state 
     operations as defined in CAI 2.6.1 in addition to the entry/exit 
     transitions as defined for the DMSS in the Main Control section of the 
     DMSS Software Design Document.
  
   DEPENDENCIES
     None
  
   RETURN VALUE
     None

   SIDE EFFECTS
     None

===========================================================================*/ 

extern void acpmcidl_state                          
(
  acpmc_state_info_type *state_info_ptr    /* Pointer to state information */
);


#endif


