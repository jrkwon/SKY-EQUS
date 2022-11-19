#ifndef TIMETEST_H
#define TIMETEST_H
/*===========================================================================

           T I M E T E S T   H E A D E R    F I L E

DESCRIPTION
  This header file contains all the definitions necessary for supporting
  timing tests via the LED output buffer.

Copyright (c) 1993, 1995, 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997, 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/MSM5000/VCS/timetest.h_v   1.1   10 Sep 1999 17:34:58   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/13/99    ms     Removed pre-MSM2 definitions. Added defs for MSM3.
11/25/97   jjn     Instead of defining TIMETEST_PORT as 0x03E just for
                   Gemini, defined TIMETEST_PORT as 0x03E for all targets
                   that use MSM 2 family CDMA chip
04/04/95   jah     Added Gemini support
12/09/93   jah     Added comments
05/27/93   jah     Added Beta II support
02/16/93   jah     Initial revision.

===========================================================================*/

#include "target.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Define the address of the buffer which drives the outputs
** hooked to the profiling mux.
*/
#if defined(T_MSM2) && !defined (T_MSM3)

/* Gemini Portable
*/
#define TIMETEST_PORT   0x03E

#elif defined (T_MSM3)

/* MSM3000 targets
*/

/* Value on the buffer output when in the Interrupt Handler
*/
#define TIMETEST_INT   0x1f

/* TIMETEST Port address 
*/
#define TIMETEST_PORT  0x02800000

#endif

/* Value pulsed on the buffer output once each second.
*/
#define TIMETEST_1PPS   0x1E

#endif  /* TIMETEST_H */
