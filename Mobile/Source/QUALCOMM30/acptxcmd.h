#ifndef ACPTXCMD_H
#define ACPTXCMD_H

/*===========================================================================

             ACPTX Command Processing (ACPTXCMD) Unit Header File 

DESCRIPTION

  This header file contains all the definitions necessary for the
  ACPTX Control Unit to interface with the ACPTX Command Processing Unit.

Copyright (c) 1991,1992 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acptxcmd.h_v   1.0   Aug 11 2000 11:41:04   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "acpcmdef.h"

/* <EJECT> */
/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPTXCMD_STATE_MACHINE

DESCRIPTION
  The acptxcmd_state_machine function provides the top level control
  of the ACPTXCMD state machine.  This function calls the approriate
  function according to the current ACPTXCMD state.

DEPENDENCIES
  When entering analog mode operation, acptxcmd_init() should be called
  before calling acptxcmd_state_machine() for the first time.

  The input value of signal_mask reflects the state of the ACPTX task's 
  signals after the last call to acptxi_wait.  By examining signal_mask,
  the ACPTXCMD state machine is able to determine the cause for its
  reactivation, i.e., which signal(s) were set.

RETURN VALUE
  None

SIDE EFFECTS
  The output value of signal_mask indicates the signal(s) that the ACPTXCMD
  state machine wishes to suspend on.

  When the ACPTXCMD state machine receives a command to shut down analog
  mode processing, the boolean acptx_exit will be set to TRUE, otherwise
  it is set to FALSE.

===========================================================================*/

extern void acptxcmd_state_machine
(
  acp_mask_type  *signal_mask,
    /* On entry indicates which signal was set after last call
       to acptxi_wait.  On exit indicates the signal(s) that
       the ACPTXCMD state machine wishes to suspend on. */
  
  boolean        *acptx_exit
    /* Will be set to TRUE when exiting this function if a command has
       been received to shut down analog mode processing  */
);

/* <EJECT> */
/*==========================================================================
  
FUNCTION ACPTXCMD_INIT
  
DESCRIPTION
  This function performs the ACPTX Command Processing unit initialization.
  
DEPENDENCIES
  None
  
RETURN VALUE
  The initial signal that the ACPTXCMD state machine wishes to suspend on.
  
SIDE EFFECTS
  None
 
===========================================================================*/

extern acp_mask_type acptxcmd_init( void );

#endif

