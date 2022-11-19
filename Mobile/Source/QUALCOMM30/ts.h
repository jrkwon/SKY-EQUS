#ifndef TS_H
#define TS_H
/*===========================================================================

                 T I M E  S T A M P   H E A D E R    F I L E

DESCRIPTION
  This module contains defintions needed to interface with the time
  stamp module.

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1998       by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


  $Header:   L:/src/asw/MSM5000/VCS/ts.h_v   1.2   12 Sep 2000 15:46:00   sanjayr  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/00   sr      Added function declarations for ts_set_frame_offset()
                   and ts_get_20ms_frame_offset_time()
09/05/98   scdb    Added _cdecl to ts_dmod_valid().
09/01/98   scdb    Added _cdecl to some functions to save ROM
07/16/98   scdb    Added prototype for ts_dmod_valid()
04/05/94   jah     Added prototype for ts_get_20ms_frame_time()
02/17/94   jah     Added macro TS_SYSTIME_TO_MS, added comdef/qw includes.
08/11/93   jah     Added prototype for ts_inc()
05/14/93   jah     Added prototype for ts_get_slot_time()
09/01/92   jah     Updated from code review.  Improved comments.
08/06/92   jah     Ported from brassboard

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "qw.h"         /* definition of qword                  */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

MACRO TS_SYSTIME_TO_MS

DESCRIPTION
  Convert a timestamp from System Time to millisecond units

PARAMETERS
  xxx   timestamp (qword) to convert

DEPENDENCIES
  None

RETURN VALUE
  The converted value is stored in xxx.

SIDE EFFECTS
  None

===========================================================================*/

#define TS_SYSTIME_TO_MS(xxx) \
  (void) qw_div( (xxx), (xxx), (word) 1024 ); \
         qw_mul( (xxx), (xxx), (dword) 5L ); \
  (void) qw_div( (xxx), (xxx), (word) ( 64 * 4 ) )

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION TS_SET

DESCRIPTION
  This procedure sets the system timestamp to a specified value which is
  the combination of a multiple of 80 milliseconds and a 26.67 ms phase
  of 80 milliseconds (0, 1, 2).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Initializes the system timestamp and sets system time.

===========================================================================*/

extern void ts_set
(
  qword set_val,
    /* Number of elapsed 80 ms units since base date. */

  word phase80
    /* Specifies the current phase of the 80 ms. */
);


/*===========================================================================

FUNCTION TS_INC

DESCRIPTION
  Increment the 64-bit system timestamp in 26.6 ms units.

DEPENDENCIES
  The system clock must be set to a reasonable value inorder for
  this function to perform a meaningful operation on the timestamp.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void ts_inc
(
  word increment
    /* The number of 26.6ms units to add to system time */
);


/*===========================================================================

FUNCTION TS_TICK

DESCRIPTION
  This function is called at every PN Roll (26.67 ms).  It increments the
  timestamp by 80 ms each time phase80 is 0.  Phase80 being 0 indicates that
  80 ms have passed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ts_tick
(
  word phase80
    /* Specifies the current phase of the 80 ms. */
);


/*===========================================================================

FUNCTION TS_GET

DESCRIPTION
  This procedure is called to read the 64-bit system timestamp.

DEPENDENCIES
  Ts_set() needs to be called when accurate time is available from the
  base station.  Prior to that time, ts_get() returns the value from
  the system clock.  The system clock must be set to a reasonable value
  inorder for this function to return a good timestamp.  Timestamp
  format is described at the top of ts.c.

RETURN VALUE
  ts_val is the current timestamp value, in timestamp format.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ts_get
(
  qword ts_val
    /* Address of qword in which to return 64-bit system timestamp. */
);


/*===========================================================================

FUNCTION TS_GET_SLOT_TIME

DESCRIPTION
  This procedure is called to convert the 64-bit system timestamp
  to units of slot time, 80 millisecond units, modulo 2048 (0x800).

DEPENDENCIES
  The value returned by this function is meaningless unless the demodulator
  is running and contains a valid value. This function does not fall back
  on "Time Of Day".

RETURN VALUE
  The current timestamp value, as specified above.

SIDE EFFECTS
  None.

===========================================================================*/

extern word  ts_get_slot_time( void );


/*===========================================================================

FUNCTION TS_GET_20MS_FRAME_TIME

DESCRIPTION
  This procedure is returns the 64-bit system timestamp in units of
  20 millisecond frame time (traffic / paging / access channel frame
  time).

DEPENDENCIES
  None

RETURN VALUE
  The current timestamp value, as specified above.

SIDE EFFECTS
  None

===========================================================================*/

extern void ts_get_20ms_frame_time
(
  qword  ts_val
    /* Address of qword in which to return 64-bit frame time. */
);


/*===========================================================================

FUNCTION TS_DMOD_VALID

DESCRIPTION
  Permits other code in the phone to tell TS that the contents of the
  demodulator are not meaningful. This causes TS to fall back on "Time
  Of Day" instead.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void ts_dmod_valid
(
  boolean dmod_valid
    /* if TRUE then the demodulator can be used. */
    /* if FALSE then fall back on Time Of Day. */
);



/*===========================================================================

FUNCTION TS_FRAME_OFFSET

DESCRIPTION
   Set the frame offset in TS, so calls to ts_get_20ms_frame_offset_time()
   are adjusted by the frame offset.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void ts_set_frame_offset
(
  byte frame_offset
);



/*===========================================================================

FUNCTION TS_GET_20MS_FRAME_OFFSET_TIME

DESCRIPTION
  This procedure returns the 64-bit system timestamp (in units of
  20 millisecond frame time) adjusted by frame offset.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void ts_get_20ms_frame_offset_time( qword ts_offset_val );

#endif /* TS_H */
