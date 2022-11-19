#ifndef LOOPBACK_H
#define LOOPBACK_H
/*===========================================================================

               L O O P B A C K    H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the Mobile Station
  Loopback service option.

Copyright (c) 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/loopback.h_v   1.0   Aug 11 2000 11:43:26   lpetrucc  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/14/95   gb      Added new Loopback_13K service option support.
03/15/94   jca      Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "cai.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* -----------------------------------------------------------------------
** Define structure used for buffering Loopback rx packet data
** ----------------------------------------------------------------------- */

typedef struct
{
  word frame_category;
    /* Received packet frame category (see IS-95 6.2.2.2) */
  byte data_bits[36];
    /* Received Loopback Packet data bits */
} loopback_data_type;

/* <EJECT> */
/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION LOOPBACK_RX

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

extern void loopback_rx
(
  loopback_data_type rx_data
    /* Received packet data for MS Loopback service option */
);

/* <EJECT> */
/*===========================================================================

FUNCTION LOOPBACK_TX

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  The frame rate to use is returned.

SIDE EFFECTS

===========================================================================*/

extern cai_data_rate_type loopback_tx
(
  cai_data_rate_type max_rate,
    /* Max rate for this frame */
  cai_frame_type     *frame_data
    /* Pointer to loopback data bits */
);

/* <EJECT> */
/*===========================================================================

FUNCTION LOOPBACK_INIT

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

extern void loopback_init( int rate );

#endif /* LOOPBACK_H */
