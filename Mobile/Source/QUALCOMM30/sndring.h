#ifndef SNDRING_H
#define SNDRING_H
/*===========================================================================

    R I N G E R   C O N T R O L  R E G I S T E R   H E A D E R   F I L E

DESCRIPTION

  Definitions of the ringer control registers in the MSM 2.2 and 2.3.
  Definitions of the ringer programming values used to generate tones of
  various frequencies.

REFERENCES
  MSM2.2 User's Manual.

Copyright (c) 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/sndring.h_v   1.1   10 Sep 1999 18:11:28   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/04/99    st     Moved everything to ring.h and included SND_RINGER_STOP
                   to be backwards compatible with error.c file.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.
01/21/98    ha     Added additional piano notes to complete octaves 4 and 5. 
                   Updated Copyright.
11/21/97    ha     Added DS6 note (2489.1Hz) and corrected FS6's N-value entry.
07/23/97    ro     Added include's to make this file self-sufficient.
07/23/97    ro     Created from sndi.h.  Added SND_RINGER_STOP macro.
                   Redefine ringer frequency constants with N less than 4095.

===========================================================================*/

#include "ring.h"                    /* ringer definitions          */

#define SND_RINGER_STOP RINGER_STOP


#endif /* SNDRING_H */
