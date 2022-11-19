#ifndef UINTSTMN_H
#define UINTSTMN_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           U I    T E S T   M E N U S

DESCRIPTION
  This module implements the UI's test menus for performing various testing
  functions from the phone.

  External functions:

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/



/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/uiutstmn.h_v   1.2   29 Mar 2000 18:11:04   hdowlat  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/07/99   CS      Removed feature FEATURE_UASMS ifdef.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                             INCLUDE FILES

===========================================================================*/
#include "uiumenu.h"

/* rosa 00-05-17 --> @why, what */
#ifdef FEATURE_NUI_D1 
/* do not use this file */
#else

/* <EJECT> */
/*===========================================================================

                             GLOBAL DATA

===========================================================================*/
extern const uimenu_entry_type    tests_menu[];
extern uimenu_tier_type           tests_menu_tier;

#endif /* FEATURE_NUI_D1 */
/* <-- */
#endif /* UINTSTMN_H */
