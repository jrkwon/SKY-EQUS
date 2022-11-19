#ifndef SNDITAB_H
#define SNDITAB_H
/*===========================================================================

     S O U N D   T A B L E (S)   I N T E R N A L   H E A D E R   F I L E

DESCRIPTION
  This file contains the definitions needed for the sound driver to access
  tables found in SNDITAB.C.

Copyright(c) 1995, 1996, 1997 by QUALCOMM, Incorporated. All Rights Reserved.
Copyright(c) 1998 by QUALCOMM, Incorporated. All Rights Reserved.
===========================================================================*/

#include "comdef.h"
#include "sndi.h"

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/MSM5000/VCS/snditab.h_v   1.3   12 Sep 2000 09:11:40   nbouchar  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/00   nxb     Removed SND_ROM.
03/04/99    st     Use snd_compact_type instead of snd_type.
12/04/98    sm     Support selectable ROM keyword for backward compatibility.
11/04/98    sm     Merged ARM/MSM3000 support to mainline.  Updated copyright
03/28/97    ro     Update copyright.
06/20/95   fkm     Created file.

===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

extern snd_compact_type const * const snd_sound_ptrs[]; 
                                            /* See WARNING in SNDITAB.C    */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#endif /* SNDITAB_H */
