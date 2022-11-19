#ifndef UISVRI_H
#define UISVRI_H
/*===========================================================================

         U S E R   I N T E R F A C E
         V O I C E   R E C O G N I T I O N   D E F I N I T I O N S
         I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  This header contains all phone VR related information for the UI.
  This is the internal header file, and should only be included by the VR UI
  module.  All other modules should include the external header file,
  which is "uinsphvr.h".

    Copyright (c) 1999,2000 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uisvri.h_v   1.1   29 Mar 2000 18:10:58   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/11/00   HQ      Use the new voice activation VS API.
01/28/00   HQ      Added HFK support - wake-up training, voice activation,
                   incoming call voice answer, and expert window.
07/19/99   RPW     Added FEATURE_VR_SI_TEST under FEATURE_PHONE_VR, to
                   implement the Speaker-Independent test mode.
07/14/99   RPW     Added support for answering machine feature (in-call
                   voice memo)
07/09/99   RPW     Took VRVM language selection, and everything upon which
                   it depends, out of FEATURE_PHONE_VR blocks and put it
                   under FEATURE_VS_TASK blocks to allow the language to
                   be set when VR is not enabled but VM is enabled.
                   Moved stuff over from uinsphvr.c that should have been put
                   there in the first place.
05/24/99   RPW     Added vdbrwset.h in include file list.
05/19/99   RPW     Added VS_EVENT_FAILURE code to distinguish it from
                   VS_EVENT_ABORTED.
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

/*============================================================================
** VR-only interface
**============================================================================
*/
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

#endif /* UISVRI_H */

