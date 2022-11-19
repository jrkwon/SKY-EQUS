#ifndef PSMISC_H
#define PSMISC_H
/*===========================================================================

                   D A T A   T A S K   M A N A G E R
                        H E A D E R    F I L E

DESCRIPTION
  This file contains all interface functions for the Data Task State machine
   unit.

 Copyright (c) 1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
 Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
 Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/psmisc.h_v   1.0   23 Oct 2000 17:32:32   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/05/99   rc      Added support for UDP debug statistics for Sockets.
03/04/99   hcg     Changed browser interface feature to FEATURE_DS_BROWSER_INTERFACE.
06/16/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/12/97   ldg     Added new FEATURE_DS_PSTATS
04/11/95   jjw     Initial version of file


===========================================================================*/





/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "target.h" 

#ifdef FEATURE_DS
#include "qw.h"
#include "clk.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_DS_PSTATS
#define DS_PSTATS(a) (a)
#else
#define DS_PSTATS(a) (0)
#endif

#if defined(FEATURE_DS_BROWSER_INTERFACE) || defined(FEATURE_DS_SOCKETS)
#ifdef FEATURE_DS_PSTATS
/*---------------------------------------------------------------------------
  Type to index the array of UDP stats.
---------------------------------------------------------------------------*/
typedef enum               /* From UDP MIB, RFC 2018                       */
{
  UDP_STATS_IN_DATAGRAMS,  /* The total number of UDP datagrams delivered
                              to UDP users.                                */
  UDP_STATS_OUT_DATAGRAMS, /* The total number of UDP datagrams sent from
                              this entity.                                 */
  UDP_STATS_IN_ERRORS,     /* The number of received UDP datagrams that
                              could not be delivered for reasons other
                              than the lack of an application at the
                              destination port.                            */

  UDP_STATS_COUNT          /* The number of UDP stats                      */
} udp_stats_enum;

/*---------------------------------------------------------------------------
  Array of UDP stats.
  These stats are cumulative since the last udp_reset_stats or power-on.
---------------------------------------------------------------------------*/
extern word udp_stats[UDP_STATS_COUNT];

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const udp_stats_name_ptr[UDP_STATS_COUNT];

#endif /* FEATURE_DS_PSTATS */
#endif /* FEATURE_DS_BROWSER_INTERFACE || FEATURE_DS_SOCKETS*/


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION get16

DESCRIPTION
  This function return the word value from the 2 byte value 

DEPENDENCIES
  None

RETURN VALUE
  A word representing the 2 bytes pointed at by the passed parameter

SIDE EFFECTS
  None
===========================================================================*/

extern word get16
(
  byte *cp               /* pointer to byte string to get the 16 bits from */
);

/*===========================================================================

FUNCTION get32

DESCRIPTION
  This function 
  Machine-independent, alignment insensitive network-to-host long conversion

DEPENDENCIES
  None

RETURN VALUE
  32 bit value representing the 4 network bytes in the passed char string.

SIDE EFFECTS
  None
===========================================================================*/

extern dword get32
(
  byte *cp               /*  */
);

/*===========================================================================

FUNCTION put16

DESCRIPTION
  This function 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern byte *put16
(
  byte *cp,      /*  */
  word x         /*  */
);

/*===========================================================================

FUNCTION put32

DESCRIPTION
  This function 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern byte *put32
( 
  byte *cp,             /*  */
  dword x               /*  */
);

/*===========================================================================

FUNCTION msclock

DESCRIPTION
  This function will return the time in milliseconds since ....

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern dword msclock( void);

/*===========================================================================

FUNCTION  lcsum

DESCRIPTION
  This function 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern word lcsum
(
  word *wp,
  word len
);


#if defined(FEATURE_DS_BROWSER_INTERFACE) || defined(FEATURE_DS_SOCKETS)
#ifdef FEATURE_DS_PSTATS

/*===========================================================================

FUNCTION UDP_GET_STATS

DESCRIPTION
  This function will copy UDP stats into the given array.

DEPENDENCIES
  The given array must have at least UDP_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void udp_get_stats
(
  word *stats_ptr /* Pointer to array into which stats will be copied */
);

/*===========================================================================

FUNCTION UDP_RESET_STATS

DESCRIPTION
  This function will reset those UDP stats which can be reported
  with udp_get_stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void udp_reset_stats(void);

#endif /* FEATURE_DS_PSTATS */
#endif /* FEATURE_DS_BROWSER_INTERFACE || FEATURE_DS_SOCKETS */

#endif /* FEATURE_DS */
#endif /* PSMISC_H */
