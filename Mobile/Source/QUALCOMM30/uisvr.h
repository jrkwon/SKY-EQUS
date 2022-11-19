#ifndef UISVR_H
#define UISVR_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   R E C O G N I T I O N   D E F I N I T I O N S

DESCRIPTION
  This header contains all phone VR related information for the UI.

    Copyright (c) 1999,2000 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uisvr.h_v   1.1   29 Mar 2000 18:10:58   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/11/00   HQ      Added uivr_incoming_call_in_prog().
01/28/00   HQ      Added HFK support - wake-up training, voice activation,
                   incoming call voice answer, and expert window.
08/31/99   RPW     Fixed CR 11395 - added hooks to capture SMS UI events for
                   the VR and VM states before they cause the UI state to
                   jump into the SMS state - the events are re-inserted into
                   the UI priority event queue once VR/VM exits.
07/19/99   RPW     Added FEATURE_VR_SI_TEST under FEATURE_PHONE_VR, to
                   implement the Speaker-Independent test mode.
07/14/99   RPW     Added support for answering machine feature (in-call
                   voice memo)
07/09/99   RPW     Took VRVM language selection, and everything upon which
                   it depends, out of FEATURE_PHONE_VR blocks and put it
                   under FEATURE_VS_TASK blocks to allow the language to
                   be set when VR is not enabled but VM is enabled.
05/25/99   RPW     Added VR_ENTRY_VOICETAG_CLR entrypoint to VR for clearing
                   all voicetags at once.
05/05/99   RPW     Moved all UIVR-internal references to internal header file
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

/* This rest of this module is skipped unless VR feature is included. */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

#endif /* UISVR_H */

