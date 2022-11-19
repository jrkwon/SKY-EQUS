#ifndef ULPN_H
#define ULPN_H
/*===========================================================================

                   U L P N    H E A D E R    F I L E

DESCRIPTION
   This file contains definitions used by the User Long PN services.

   It is used only in the portable subscriber station.

   Copyright (c) 1992, 1993 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 2000       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/ulpn.h_v   1.2   12 Mar 2001 20:10:50   degangy  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/12/01   dgy     Added FEATURE_IS2000_QPCH_CCI.
11/18/99   ajn     Enhanced Standby III ... 320ms based ULPN calculations
08/06/99   ejv     ajn: Externed function ulpn_quarter_advance.
01/13/99   ejv     Removed _cdecl - not necessary for ARM.
09/01/98   thh     Added keyword _cdecl to the declarations of those functions
                   which are called by assembly codes using C function calling
                   convention.
                   Updated copyright date.
03/02/93   ptw     Created.

===========================================================================*/

#include "comdef.h"

/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Declaration of Data Type for User Long PN Code
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct
  {
  dword  low;
  word   high;
  } ulpn_type;
   /* This structure of 48 bits is used to handle the 42-bit user long
      code PN states.  Most arithmetic on these numbers is done 48 bits
      wide for simplicity. */


/*===========================================================================

                      REGIONAL FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION ulpn_advance

DESCRIPTION
  This function computes the user long PN code state at some time
  in the future, given the current user long PN code state and the
  number of intervening 26.666... ms periods.

DEPENDENCIES
  The function depends on the magic properties of the pn_advance array.

RETURN VALUE
  No formal return value, but the ULPN state at *pn_state_ptr is
  updated to give the new ULPN state value.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ulpn_advance
(
ulpn_type *pn_state_ptr,
   /* Pointer to the value of the user long PN;
      At entry: the old value
      At exit: the new value */

word advance
   /* The number of 26ms periods to advance the long code */
   /* Each 26ms period is 0x8000 chips.  These 15 zero LSBs mean
      that we can skip ahead in the multiplication.
      This value ranges from 0 to about 9000, so we can treat it
      as a 16-bit word and not lose anything. This also means that
      we can end the multiplication early. */
);


/*===========================================================================

FUNCTION ulpn_advance_320

DESCRIPTION
  This function computes the user long PN code state at some time
  in the future, given the current user long PN code state and the
  number of intervening 320ms periods.

DEPENDENCIES
  The function depends on the magic properties of the pn_advance_320 array.

RETURN VALUE
  No formal return value, but the ULPN state at *pn_state_ptr is
  updated to give the new ULPN state value.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ulpn_advance_320
(
  ulpn_type *pn_state_ptr,
    /* Pointer to the value of the user long PN;
       At entry: the old value
       At exit: the new value */

  word advance
    /* The number of 320ms periods to advance the long code */
);

/*===========================================================================

FUNCTION ULPN_QUARTER_ADVANCE

DESCRIPTION
  Advances the PN value by 1/4 PN cirle.

DEPENDENCIES
  The function depends on the magic properties of the pn_advance array.
  (See comment at pn_advance array declaration.)

RETURN VALUE 
  No formal return value, but the ULPN state at *new_pn_ptr is
  updated to give the new ULPN state value.

SIDE EFFECTS
  None

===========================================================================*/

extern void ulpn_quarter_advance
(
  ulpn_type old_pn,
   /* PN value to be advanced. */

  ulpn_type *new_pn_ptr
    /* PN value incremented by 1/4 PN circle. */
);

/*===========================================================================

FUNCTION ULPN_3_QUARTER_RETARD

DESCRIPTION
  Retards the PN value by 3/4 PN ROLL in time.

DEPENDENCIES

RETURN VALUE 
  No formal return value, but the ULPN state at *new_pn_ptr is
  updated to give the new ULPN state value.

SIDE EFFECTS
  None

===========================================================================*/

void ulpn_3_quarter_retard
(
  ulpn_type old_pn,
    /* Current PN value */

  ulpn_type *new_pn_ptr
    /* The Current PN value incremented by 1/4 PN circle. */
);

#ifdef FEATURE_IS2000_QPCH_CCI
/*===========================================================================

FUNCTION ULPN_3_ROLL_ADVANCE

DESCRIPTION
  Advances the PN value by 3 PN ROLLs in time.

DEPENDENCIES

RETURN VALUE 
  No formal return value, but the ULPN state at *new_pn_ptr is
  updated to give the new ULPN state value.

SIDE EFFECTS
  None

===========================================================================*/

extern void ulpn_3_roll_advance
(
  ulpn_type old_pn,
    /* Current PN value */

  ulpn_type *new_pn_ptr
    /* The Current PN value incremented by 3 PN rolls. */
);
#endif /* FEATURE_IS2000_QPCH_CCI */

#endif /* ULPN_H */

