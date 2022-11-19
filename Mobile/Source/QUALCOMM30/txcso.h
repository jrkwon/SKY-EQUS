#ifndef TXCSO_H
#define TXCSO_H

/*===========================================================================
                 
   C D M A   T R A N S M I T   S E R V I C E   O P T I O N S  M O D U L E

DESCRIPTION
  This header file contains definitions for the transmit service options
  module that are used by the txc unit.  This header file is internal to 
  the txc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/txcso.h_v   1.2   13 Nov 2000 22:21:20   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/13/00   bgc     (merge) Separated FCH and SCH TDSO blocks.
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "cai.h"
#include "txc.h"
#include "tdso.h"

#ifdef FEATURE_IS2000
  #ifdef FEATURE_IS2000_TDSO
    extern tdso_frame_block_type fch_tdso_frame_block;
    extern tdso_frame_block_type sch_tdso_frame_block;
  #endif // FEATURE_IS2000_TDSO
#endif /* FEATURE_IS2000 */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION TXC_VOC_TRAFFIC

DESCRIPTION
  Get traffic data (primary or secondary) from the vocoder.

DEPENDENCIES
  The vocoder should be initialized already.

RETURN VALUE
  Returns the address of the vocoder data in a pointer passed to the
  procedure.

  Returns the rate of the vocoder data

SIDE EFFECTS
  None

===========================================================================*/
extern cai_data_rate_type txc_voc_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  txc_sig_frame_type signext
    /* Signaling mode for next packet */
);


/*===========================================================================

FUNCTION TX_LOOPBACK_TRAFFIC

DESCRIPTION
  Get loopback data bits (primary only) from the Loopback Service Option
  layer.

DEPENDENCIES
  The Loopback Service Option layer should be initialized already.

RETURN VALUE
  Returns the address of the loopback data bits in a pointer passed to the
  procedure.

  Returns the rate of the loopback data.

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_loopback_traffic
(
  byte **data_ptr,
    /* Returns pointer to loopback data bits */
  txc_sig_frame_type signext
    /* Signaling mode for this frame */
);

/*===========================================================================

FUNCTION TXC_MAR_TRAFFIC

DESCRIPTION
  Get traffic data (primary or secondary) from Markov frame error rate tests.

DEPENDENCIES
  The tx.frame_cnt has to be right.

RETURN VALUE
  Returns the address of the FER data in a variable passed to the
  procedure.

  Returns the rate of the FER data.

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_mar_traffic
(
  byte **data_ptr
    /* Returns pointer to data */
);

#ifdef FEATURE_IS2000_TDSO
/*===========================================================================

FUNCTION TXC_TDSO_TRAFFIC

DESCRIPTION
  Get traffic data (primary) from TDSO

DEPENDENCIES

RETURN VALUE
  Returns the data rate

SIDE EFFECTS
  None

===========================================================================*/

extern cai_data_rate_type txc_tdso_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  txc_sig_frame_type signext
    /* Signaling mode for next packet */
);
#endif /*FEATURE_IS2000_TDSO*/

#endif /*TXCSO_H*/
