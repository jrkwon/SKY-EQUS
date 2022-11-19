#ifndef UIXVS_H
#define UIXVS_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   S E R V I C E S   D E F I N I T I O N S

DESCRIPTION
  This header contains all phone Voice Services related information for
  the UI.

    Copyright (c) 1999 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uixvs.h_v   1.2   29 Mar 2000 18:11:14   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/28/00   HQ      Added HFK support - wake-up training, voice activation,
                   incoming call voice answer, and expert window.
10/07/99   RPW     Added one global instance of the uivs_state_type structure
                   and removed all instances from the UI stack, to reduce
                   stack usage.  Removed most calls to uivs_state_read since
                   it is no longer necessary to read the state structure
                   more than once after bootup.
09/29/99   RPW     Added UIVS_MAX_LANGUAGES definition.
09/27/99   RPW     Added a new function uivs_init which performs boot-time
                   initialization steps, including the establishment of
                   links for remote files.
08/31/99   RPW     Fixed CR 11159, 11396 - added a function uivs_adjust_volume
                   to adjust the earpiece, speaker, or headset volume,
                   whichever is currently active, without using the
                   traditional UI list state that changes the display and
                   requires a change of UI state.
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

/* This following part of the module is defined for either VR or VM. */
#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

#endif /* UIXVS_H */

