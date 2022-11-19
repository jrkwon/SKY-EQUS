#ifndef VBATT_H
#define VBATT_H
/*===========================================================================

    B A T T E R Y   L E V E L   U T I L I T I E S   H E A D E R   F I L E

DESCRIPTION
  This header file contains all of the definitions necessary for other
  tasks to interface with the battery level utilities.

REFERENCES
  None

Copyright (c) 1992, 1995, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.
   
  $Header:   L:/src/asw/MSM5000/VCS/vbatt.h_v   1.1   10 Sep 1999 18:11:48   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/11/98   aks     Added definitions for dual-battery (5GP) gauging support.
12/08/97   aks     Added definitions for internal charger and multiple battery
                   capability.
11/02/97   BK      Added missing T_Q.
09/17/97   aks     Improved overall precision and temperature compensation.  Power
                   drop and non-linear thermistor compensation added.
08/15/97   aks     Made vbatt_read_gauge() defined only for TGP targets.
08/13/97   aks     Added the externalized vbatt_read_gauge() function and
                   associated return type definitions.
03/01/95   tst     Changed to use VBATT_SCALED_MIN instead of 0.
02/14/95   tst     Added VBATT_SCALED_MIN.
02/04/93   jah     Changed vbatt_calibrate() to take byte inputs.
12/17/92   jah     Created

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* The range of Vbatt readings [VBATT_SCALED_MIN,VBATT_SCALE] returned
** by vbatt_read
*/
#define VBATT_SCALED_MIN   0
#define VBATT_SCALE      255

#if (TG==T_T || TG==T_Q)
#error code not present
#endif

#if (TG==T_T)
#error code not present
#elif (TG==T_Q)
#error code not present
#else
/* Others have no internal charger and no multiple battery types. */                            
#undef  VBATT_PHONE_HAS_INT_CHARGER
#undef  VBATT_PHONE_HAS_MULTI_BATT_ID
#endif


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION VBATT_READ

DESCRIPTION
  This function reads the current battery level.

DEPENDENCIES
  None

RETURN VALUE
  A scaled version of the batter level where empty is VBATT_SCALED_MIN,
  and full is VBATT_SCALE.

SIDE EFFECTS
  None

===========================================================================*/
extern byte vbatt_read( void );


/*===========================================================================

FUNCTION VBATT_CALIBRATE

DESCRIPTION
  This function calibrates the battery level reading returned by
  vbatt_read().  This routine need only be called once.  This calibration
  is to correct for the error in the resisters used in the voltage divider
  at the input to the ADC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Values returned by vbatt_read() may shift drastically from values
  returned in previous calls.

===========================================================================*/
extern void vbatt_calibrate
(
  byte min,
    /* Value returned by the ADC when the battery is at the minimum level
    ** at which the phone can operate.  Any safety margin added to this
    ** measurement should be done by the calibration, to provide the
    ** correction at the lowest level.
    */
  byte max
    /* Value returned by the ADC when the battery is at the fully charged
    ** level.  Any "show full when it's at max charge" margin added to this
    ** measurement should be done by the calibration, to provide the
    ** correction at the lowest level.
    */
);

#if (TG==T_T || TG==T_Q)
#error code not present
#endif

#endif /* VBATT_H */


