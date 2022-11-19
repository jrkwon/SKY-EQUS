#ifndef UISVMI_H
#define UISVMI_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   M E M O   D E F I N I T I O N S
         I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  This header contains all VM related information
  This is the internal header file, and should only be included by the VM UI
  module.  All other modules should include the external header file,
  which is "uinsphvm.h".

    Copyright (c) 1999 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uisvmi.h_v   1.1   29 Mar 2000 18:10:56   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/20/99   RPW     Fixed answering machine feature bugs
07/14/99   RPW     Added support for answering machine feature (in-call
                   voice memo)
05/05/99   RPW     Initial revision.
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

/* This entire module is skipped unless voice memo feature is included. */
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

#endif /* UISVMI_H */

