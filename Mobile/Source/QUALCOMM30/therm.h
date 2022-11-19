#ifndef THERM_H
#define THERM_H
/*===========================================================================

       T H E R M I S T O R   U T I L I T I E S   H E A D E R   F I L E

DESCRIPTION
  This header file contains all of the definitions necessary for other
  tasks to interface with the thermistor utilities.

REFERENCES
  None

Copyright (c) 1995, 1996, 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998, 1999       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.
   
  $Header:   L:/src/asw/MSM5000/VCS/therm.h_v   1.1   10 Sep 1999 18:11:46   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/95   tst     Created from vbatt.h.

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* The range of Therm readings [THERM_SCALED_MIN,THERM_SCALE] returned
** by therm_read
*/
#define THERM_SCALED_MIN   0
#define THERM_SCALE      255


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION THERM_READ

DESCRIPTION
  This function reads the current thermistor level (temperature).

DEPENDENCIES
  None

RETURN VALUE
  A scaled version of the thermistor level where the highest temperature is
  THERM_SCALED_MIN, and the lowest temperature is THERM_SCALE.

SIDE EFFECTS
  None

===========================================================================*/
extern byte therm_read( void );


/*===========================================================================

FUNCTION THERM_CALIBRATE

DESCRIPTION
  This function calibrates the thermistor level reading returned by
  therm_read().  This routine need only be called once.  This calibration
  is to correct for the error in the resisters used in the voltage divider
  at the input to the ADC and for tolerance of the thermistor.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Values returned by therm_read() may shift drastically from values
  returned in previous calls.

===========================================================================*/
extern void therm_calibrate
(
  byte min,
    /* Value returned by the ADC when the thermistor is at the minimum
    ** value (maximum temperature) at which the phone can operate.  Any
    ** safety margin added to this measurement should be done by the
    ** calibration, to provide the correction at the highest temperature.
    */
  byte max
    /* Value returned by the ADC when the thermistor is at the highest value
    ** (lowest temperature) at which the phone can operate.  Any margin added
    ** to this measurement should be done by the calibration, to provide the
    ** correction at the lowest temperature.
    */
);

#endif /* THERM_H */
