#ifndef ACPMCINI_H
#define ACPMCINI_H

/*===========================================================================

                       A C P M C I N I   H E A D E R

GENERAL DESCRIPTION
  This header file contains the definitions and declarations needed to 
  interface with the ACPMCINI Unit of the ACP sub-system


       Copyright (c) 1992 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcini.h_v   1.0   Aug 11 2000 11:40:42   lpetrucc  $


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

   FUNCTION ACPMCINI_STATE
  
   DESCRIPTION 
     This function is the entry point and main processing function for the 
     INIT state of the Analog Call Processing sub-system. The passed state 
     info structure contains the information necessary to perform INIT state 
     operations as defined in CAI 2.6.1 with the entry/exit transitions as
     defined for the DMSS in the Main Control section of the DMSS Software
     Design Document.
  
   DEPENDENCIES
     None

   RETURN VALUE
     None

   SIDE EFFECTS
     None

 ==========================================================================*/

extern void acpmcini_state
(
  acpmc_state_info_type *state_info    /* Pointer to ACP state information */
);


#endif


