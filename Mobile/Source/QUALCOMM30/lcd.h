#ifndef LCD_H
#define LCD_H
/*===========================================================================

                L C D   M E S S A G E   S E R V I C E S
                      H E A D E R    F I L E

DESCRIPTION
  This file contains delarations necessary to display a "last gasp" error
  message on the LCD prior to transitioning to the fault-reset code in
  the boot module.  This module is Portable specific.

Copyright (c) 1993-1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998,1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/lcd.h_v   1.1   10 Sep 1999 18:11:00   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/01/98   thh     Updated copyright date.
07/16/93   jah     Deleted LCD_MESSAGE macro, per code review.
06/29/93   jah     Initial revision

===========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION LCD_MESSAGE

DESCRIPTION
  Display a "last gasp" message on the LCD, and jump to the boot error
  handler for a reset or to allow factory code download.

DEPENDENCIES
  None.

RETURN VALUE
  This routine does not return.  It branches to a boot_reset.

SIDE EFFECTS
  None

===========================================================================*/
extern void lcd_message( void *msg_ptr );

#endif  /* LCD_H */
