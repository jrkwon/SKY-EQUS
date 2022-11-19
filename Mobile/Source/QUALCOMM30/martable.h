#ifndef MARTABLE_H
#define MARTABLE_H
/*===========================================================================

           B E R    T A B L E   H E A D E R    F I L E

DESCRIPTION
  This file contains declarations associated with the MAR services

Copyright (c) 1991,1992,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996           by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/martable.h_v   1.0   Aug 11 2000 11:43:28   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/99   ms      Added FEATURE_NO_VBB
08/31/98   pms     Added Tan's Pascal calling convention code for ROM savings
07/29/98   lcc     Merged in ARM support.
04/08/96   dna     Changed external array names to follow convention.
02/28/96   dna     Moved tables to a C file and added the proper external
                   declarations here.
06/14/95   gb      Added support for Rate set 2 service options.
06/27/91   gb      Created file.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/* <EJECT> */
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Types for different array sizes */
typedef word mar_2x4_type [2] [4];
typedef dword mar_2x4dw_type [2] [4];
typedef word mar_prob_type [5] [16] [3];

/* Enums for indexing frame and bit error counter test structure */
/* which is declared as an array internally but accessed as a    */
/* structure from externally */

/* lookup tables for masks, use (fer_rate - 1) as index */

#if defined FEATURE_NO_VBB

/* no. of 24 bits in packets
 * 16,40,80,171 rate set 1
 * 20,54,124,266 rate set 2
*/
extern mar_2x4_type * mar_tri_bytes;

/* mask last dword to fill packet */
extern mar_2x4dw_type * mar_last_mask;

/* no. of bytes in packet - 1 */
extern mar_2x4_type * mar_bytes;

/* mask last byte to compare bits */
extern mar_2x4_type * mar_last_byte;

#else  /* FEATURE_NO_VBB */

/* no. of 24 bits in packets
 * 16,40,80,171 rate set 1
 * 20,54,124,266 rate set 2
*/
extern mar_2x4_type const mar_tri_bytes;

/* mask last dword to fill packet */
extern mar_2x4dw_type const mar_last_mask;

/* no. of bytes in packet - 1 */
extern mar_2x4_type const mar_bytes;

/* mask last byte to compare bits */
extern mar_2x4_type const mar_last_byte;


#endif /* FEATURE_NO_VBB */

 /* lookup table for bits set in a byte used for bit error counting */

extern byte const mar_ones[];

#define NUM_OF_ONES_IN (byte b) ones [b]

/*===========================================================================

           MARKOV TABLE BINARY PROBABILITIES

Probabilities are listed as values which ran# must be less than to be
valid. Probs are listed eighth, eighth+quarter, eighth+quarter+half.
If ran# is greater or equal than last value then transition to full.

Sn = State at frame n.  Rn = Rate at frame n.
State is table 1st dim index.     Transition is index to table 2nd dim.

Sn+1 = 4 * ( Sn mod 4 ) + ( 3 - Trans ).
Rn+1 = 4 - ( Sn+1 mod 4 ).

===========================================================================*/

#if defined FEATURE_NO_VBB
extern mar_prob_type * mar_Markov_prob;
#else
extern mar_prob_type const mar_Markov_prob;
#endif
extern byte const mar_mux_late[];
#endif /* MARTABLE_H */
