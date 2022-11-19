#ifndef RXCRDA_H
#define RXCRDA_H

/*===========================================================================

       C D M A    R A T E    D E T E R M I N A T I O N   M O D U L E
        
DESCRIPTION
  This header file contains definitions for the rate determination module 
  that are used by the rxc unit.  This header file is internal to the rxc 
  unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/rxcrda.h_v   1.0   10 Nov 2000 16:48:16   ddalke  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "rlp.h"
#include "dec5000.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Indicates whether supplemental frame is full rate or erasure */
typedef enum
{
  RXC_SUP_ERASURE = 0,
  RXC_SUP_FULL
}rxc_tc_sup_rate_type;

#ifdef FEATURE_DETECT_SUP_MUTING
/*
 This feature is used to detect muting of supplemental channels by the
 basestation during a MDR call. IS-95B is lenient enough to allow the
 basestation to not to de-allocate the unused supplemental channels while
 in a MDR call. Hence, typically a basestation may allocate supplementals to
 the mobile when the MDR call is initiated but, if there is no forward
 link data to send, then it may just stop transmitting on these channels
 without de-allocating them.  Thus saving some signalling bandwidth for ch
 allocation in case those are required in near future. This results in mobile
 demodulating un-coded walsh channels which can result in random CRC passes.
 We term this not-transmitting-on-allocated-traffic-channels as "muting".

 This feature runs an IIR estimator for the quality of supplemental channels
 to detect an abrupt good frame which follows a long sequence of bad frames.
 It also uses Yamamato quality metric to finally make a decision that this sup
 is not really a good frame but is an accidental full rate CRC pass.
*/

  /* Threshold for the engery of frame quality estimator */
  #define DATA_IND_THRESHOLD 15

  extern byte cur_di[DEC_MAX_SCC_NUM];  /* current data indicator value */
  extern byte past_di[DEC_MAX_SCC_NUM]; /* past data indicator value */
  extern rlp_rate_enum_type past_sup_status[DEC_MAX_SCC_NUM]; /* past CRC values */
#endif /* FEATURE_DETECT_SUP_MUTING*/

  
#ifdef FEATURE_RDA_CORRECTION
  extern int8  dtx_fix_counter;
#endif /* FEATURE_RDA_CORRECTION */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION RXC_SCC_RATE_SEL

DESCRIPTION
  Determine the rate of the specified SCC. It is either FULL rate or ERASURE.
  This function returns the rate type rxc_tc_sup_rate_type

DEPENDENCIES
  None.

RETURN VALUE
  DEC_RATE_ALL : If the supplemental channel is an erasure.
  DEC_RATE_FULL : If the supplemental channel is full rate.

SIDE EFFECTS
  None.

===========================================================================*/
#if  defined (FEATURE_IS95B_MDR) || defined (FEATURE_SPECIAL_MDR)
  extern rxc_tc_sup_rate_type rxc_scc_rate_sel ( rxc_dec_type *buf_ptr, byte sup_index );
  /* Decide if the sup channel is erasure or full rate frame */
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */

/*===========================================================================

FUNCTION RXC_DEC_RATE_SEL

DESCRIPTION
  This function calls the appropriate rate procedure to
  process the decoder data received from the Traffic Channel
  and determine the data rate depending on the current rate set.

DEPENDENCIES
  None.

RETURN VALUE
  Decoder rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern word rxc_dec_rate_sel (void);

/*===========================================================================

FUNCTION RXC_MAR_RATE_SEL

DESCRIPTION
  This function determines the proper rate to input to the vocoder based
  on the frame CAI category ( mux1 category ).

DEPENDENCIES
  None.

RETURN VALUE
  Vocoder frame rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern byte rxc_mar_rate_sel(word mux1 );

/*===========================================================================

FUNCTION RXC_VOC_RATE_SEL

DESCRIPTION
  This function determines the proper rate to input to the vocoder based
  on the frame CAI category ( mux1 category ).

DEPENDENCIES
  None.

RETURN VALUE
  Vocoder frame rate.

SIDE EFFECTS
  None.

===========================================================================*/
extern byte rxc_voc_rate_sel( word mux1 );


/*===========================================================================

FUNCTION RXC_RDA_CORRECTION

DESCRIPTION
  If a DTX frame was incorrectly detected as ERASURE, this function tries to
  re-classify it as DTX.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

#ifdef FEATURE_RDA_CORRECTION
  extern rxc_frame_type rxc_rda_correction (rxc_frame_type);
#endif /* FEATURE_RDA_CORRECTION */


#endif /* RXCRDA_H */
