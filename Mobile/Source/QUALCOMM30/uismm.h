#ifndef UISMM_H
#define UISMM_H
/*===========================================================================

         U S E R   I N T E R F A C E
         M U L T I M E D I A   D E F I N I T I O N S

DESCRIPTION
  This header contains all VM related information

    Copyright (c) 1999 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uismm.h_v   1.1   04 Aug 2000 10:41:36   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/29/00  SSW      Added define for karaoke buffer size
04/27/00   HD      Some comment cleanup.
03/09/00   HD      Initial revision.
===========================================================================*/

#ifndef TARGET_H
#include "target.h"
#endif

#ifndef COMDEF_H
#include "comdef.h"
#endif

#ifndef CUSTOMER_H
#include "customer.h"
#endif

/* This entire module is skipped unless voice memo feature is included. */
#ifdef FEATURE_MULTIMEDIA
#error code not present
#endif /* FEATURE_MULTIMEDIA */

#endif /* UISMM_H */

