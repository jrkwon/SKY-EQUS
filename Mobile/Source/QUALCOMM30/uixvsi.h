#ifndef UIXVSI_H
#define UIXVSI_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   S E R V I C E S   D E F I N I T I O N S
         I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  This header contains all phone voice services related information for the UI.
  This is the internal header file, and should only be included by the VS UI
  module.  All other modules should include the external header file,
  which is "uinsphvs.h".

    Copyright (c) 1999 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uixvsi.h_v   1.1   29 Mar 2000 18:11:16   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/21/99   RPW     Initial revision.
===========================================================================*/

#ifndef TARGET_H
#include "target.h"
#endif

#ifndef COMDEF_H
#include "comdef.h"
#endif 

#ifndef CUSTOMER_H
#include "customer.h"
#endif

#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

#endif /* UIXVSI_H */

