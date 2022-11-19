#ifndef CMINBAND_H
#define CMINBAND_H
/*===========================================================================

         C A L L   M A N A G E R   I N B A N D   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMINBAND.C

Copyright (c) 1991 - 1999 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/cminband.h_v   1.0   Aug 18 2000 11:41:52   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
8/31/99    SH      Corrected some comments
5/25/99    SH      Initial release.


===========================================================================*/


#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmmc.h"      /* MC to CM interface*/



/*===========================================================================

FUNCTION cminband_client_cmd_proc

DESCRIPTION
  Process clients' inband commands.

DEPENDENCIES
  none
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cminband_client_cmd_proc(

    cm_cmd_s_type  *cmd_ptr
        /* pointer to a CM command */
);


/*===========================================================================

FUNCTION cminband_mc_rpt_proc

DESCRIPTION
  Process MC inband related reports

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cminband_mc_rpt_proc(

    const cm_cmd_s_type    *cmd_ptr
        /* Pointer to a CM command */
);

#endif /* CMINBAND_H */

