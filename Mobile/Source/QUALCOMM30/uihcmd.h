#ifndef UINCMD_H
#define UINCMD_H

/*===========================================================================

           U S E R   I N T E R F A C E   I N T E R N A L     

DESCRIPTION
  Command handling functions

 Copyright (c) 1998 by QUALCOMM INCORPORATED  All Rights Reserved.

===========================================================================*/



/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uihcmd.h_v   1.1   29 Mar 2000 18:10:36   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/

#include "ui.h"

/* <EJECT> */
/*===========================================================================
FUNCTION UI_HANDLE_CMDS

DESCRIPTION
  Handle commands to the user interface task
  Input events to UI state machine if necessary
  Many of them copied from uistate_cmds()
  Need to simplify it further

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void ui_handle_cmds ( ui_cmd_type *cmd_ptr);


#endif
