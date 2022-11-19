#ifndef MSM_INIT_H
#define MSM_INIT_H
/*===========================================================================

 M S M 5 0 0 0   D E V I C E   I N I T I A L I Z A T I O N   H E A D E R   F I L E

DESCRIPTION
  This header file contains the definition necessary to do any initialization 
required by the MSM5000.

REFERENCES
  None

Copyright (c) 1992, 1995, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.
   
  $Header:   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/28/00   hrk     Initial revision

===========================================================================*/


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/



/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

  FUNCTION msm_device_init()

  DESCRIPTION
    Initialize MSM to default operating mode.
    
  PARAMETERS
    None.

  DEPENDENCIES
    None.

  RETURN VALUE
    None.

  SIDE EFFECTS
===========================================================================*/

void msm_device_init(void);

#endif /* MSM_INIT_H */

