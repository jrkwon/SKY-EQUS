#ifndef UISVM_H
#define UISVM_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   M E M O   D E F I N I T I O N S

DESCRIPTION
  This header contains all VM related information

    Copyright (c) 1999 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uisvm.h_v   1.1   29 Mar 2000 18:10:56   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/31/99   RPW     Fixed CR 11395 - added hooks to capture SMS UI events for
                   the VR and VM states before they cause the UI state to
                   jump into the SMS state - the events are re-inserted into
                   the UI priority event queue once VR/VM exits.
07/20/99   RPW     Fixed answering machine feature bugs and moved enabling
                   of answering machine feature to a separate option under
                   the voice services menu
07/14/99   RPW     Added support for answering machine feature (in-call
                   voice memo)
05/05/99   RPW     Moved all UIVM-internal references to internal header file
04/23/99   RPW     Initial revision.
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

#endif /* UISVM_H */

