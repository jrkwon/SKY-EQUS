#ifndef MUX_H
#define MUX_H
/*===========================================================================

           M U L T I P L E X    S U B L A Y E R   D E C L A R A T I O N S

DESCRIPTION
  All the declarations and definitions necessary to support the frame  
  partitioning into the different information streams (primary, secondary
  and signalling).
  
REFERENCES
  PCS spec PN-3384, ANSI J-STD-008

Copyright (c) 1990, 1991, 1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993, 1994, 1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996, 1997, 1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999             by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

 $Header:   L:/src/asw/MSM5000/VCS/mux.h_v   1.1   10 Sep 1999 18:10:24   lchan  $

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/14/99   aaj     Changed the mux table variabels from 4 byte int to 2 byte word
02/17/99   aaj     Added const const int to move the tables to ROM space
08/15/95   gb      Added MUX2 signalling parameter lookups for TXC.C
01/01/95   gb      Created module.

===========================================================================*/

#include "comdef.h"     /* Definitions for byte, word, etc.     */
#include "dec.h"
#include "enc.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Rate set 2, Mux option 2 - 14400 frame definitions */
#define   MUX2_ERASE_BIT   0x80      /* the reverse link frame erased bit */
#define   MUX2_MM_BIT      0x40      /* the Mixed Mode Bit */
#define   MUX2_FULL_FM     0x3C      /* Mask for 14400 Frame Mode bits */
#define   MUX2_FULL_SHIFT  0x02      /* Shift for 14400 Frame Mode bits */
#define   MUX2_HALF_FM     0x38      /* Mask for 7200 Frame Mode bits */
#define   MUX2_HALF_SHIFT  0x03      /* Shift for 7200 Frame Mode bits */
#define   MUX2_QTR_FM      0x30      /* Mask for 3600 Frame Mode bits */
#define   MUX2_QTR_SHIFT   0x04      /* Shift for 3600 Frame Mode bits */

#define   MUX2_PRI_POS         2     
#define   MUX2_FULL_PRI_LEN    266
#define   MUX2_HALF_PRI_LEN    124
#define   MUX2_QTR_PRI_LEN     54
#define   MUX2_EIGHTH_PRI_LEN  20

/* Rate set 1, Mux option 1 - 9600 frame definitions */
#define   MUX1_MM_BIT      0x80      /* the Mixed Mode Bit */
#define   MUX1_FULL_FM     0x70      /* Mask for Frame Mode bits */
#define   MUX1_FULL_SHIFT  0x04      /* Shift for Frame Mode bits */

#define   MUX1_FULL_PRI_POS    1     
#define   MUX1_FULL_PRI_LEN    171
#define   MUX1_OTHER_PRI_POS   0
#define   MUX1_HALF_PRI_LEN    80
#define   MUX1_QTR_PRI_LEN     40
#define   MUX1_EIGHTH_PRI_LEN  16


/*
** Translation Table for CAI_rate type to useful enc_rate_type.
*/
extern const enc_rate_type mux_cai_to_enc [5];


/* Full Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 4 bit FM (frame mode) lookup
 */

extern const word mux2_full_pri_pos [16];

extern const word mux2_full_pri_len [16];

extern const word mux2_full_sig_pos [16];

extern const word mux2_full_sig_len [16];

extern const word mux2_full_sec_pos [16];

extern const word mux2_full_sec_len [16];

/* Half Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 3 bit FM (frame mode) lookup
 */

extern const word mux2_half_pri_pos [8]; 

extern const word mux2_half_pri_len [8]; 

extern const word mux2_half_sig_pos [8]; 

extern const word mux2_half_sig_len [8]; 

extern const word mux2_half_sec_pos [8]; 

extern const word mux2_half_sec_len [8];

/* Quarter Rate Mux 2 position lookup table:
 * we assume MM = 1, and the lookup is a 2 bit FM (frame mode) lookup
 */

extern const word mux2_qtr_pri_pos [4];

extern const word mux2_qtr_pri_len [4];

extern const word mux2_qtr_sig_pos [4];

extern const word mux2_qtr_sig_len [4];

extern const word mux2_qtr_sec_pos [4];

extern const word mux2_qtr_sec_len [4];


extern const word mux2_sig_blank_len [4];

extern const word mux2_sig_blank_pos [4];

extern const word mux2_sig_blank_fm  [4];

extern const word mux2_sig_blank_som [4];


extern const word mux2_sig_dim_len [4] [4];

extern const word mux2_sig_dim_pos [4] [4];

extern const word mux2_sig_dim_fm  [4] [4];

extern const word mux2_pri_dim_len [4] [4];

extern const word mux2_pri_dim_pos [4] [4];

extern const word MUX2_BLANK [4];

extern const word MUX2_DIM [4] [4];

/* Rate Set 1 definitions */

extern const word mux1_full_pri_pos [8];

extern const word mux1_full_pri_len [8];

extern const word mux1_full_sig_pos [8];

extern const word mux1_full_sig_len [8];

extern const word mux1_full_sec_pos [8];

extern const word mux1_full_sec_len [8];


#endif  /* MUX_H */
