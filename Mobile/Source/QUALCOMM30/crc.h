#ifndef CRC_H
#define CRC_H
/*===========================================================================

              C R C    S E R V I C E S    D E C L A R A T I O N S

DESCRIPTION
  The following declarations are for use with the CRC Services.

Copyright(c) 1991,1992,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright(c) 1998,1999      by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/crc.h_v   1.1   10 Sep 1999 17:34:50   lchan  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/17/99   ms      Removed comments related to bb_ for Virtual Boot Block.
12/16/98   jct     Removed 80186 support
09/21/98   jct     Incorporate ARM porting changes, cdecl changes
02/28/96   dna     Changed crc_16_l_table extern to be a pointer so the 
                   macro now accesses the table through the Boot Block jump
                   table.
06/21/93   jah     Corrected CRC_16_L_OK to match with crc_16_l_calc().
06/07/93   jah     Added prototype for crc_16_l_calc(), and improved comments
                   for CRC_16_L_STEP().
04/26/93   jah     Added extern for crc_16_l_table[] for macro.
04/22/93   jah     Added CRC definitions and macro to support CRC CCITT-16
                   processed LSB first.
07/09/92   jah     Changed crc_16_step() from bits to bytes
06/18/92   jah     Ported from brassboard and converted to table-driven.

===========================================================================*/

#include "comdef.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Residual CRC value to compare against return value of crc_16_calc().
** Use crc_16_calc() to calculate a 16 bit CRC, and append it to the buffer.
** When crc_16_calc() is applied to the unchanged result, it returns CRC_16_OK.
*/
#define CRC_16_OK               0xE2F0

/* Mask for CRC-16 polynomial:
**
**      x^16 + x^12 + x^5 + 1
**
** This is more commonly referred to as CCITT-16.
** Note:  the x^16 tap is left off, it's implicit.
*/
#define CRC_16_POLYNOMIAL       0x1021

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
** well as allows detection of an entire data stream of zeroes.
*/
#define CRC_16_SEED             0xFFFF

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Seed value for CRC register.  The all zeroes seed is inverted prior to
** being used in the step-wise CRC CCITT-16.  This behaves as CRC_16_SEED.
*/
#define CRC_16_STEP_SEED        (~((word) CRC_16_SEED))

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Residual CRC value to compare against return value of crc_30_calc().
** Use crc_30_calc() to calculate a 30 bit CRC, and append it to the buffer.
** When crc_30_calc() is applied to the unchanged result, it returns CRC_30_OK.
*/
#define  CRC_30_OK  0x0B105AA5

/* Mask for CRC-30 polynomial:
**
**      x^30+x^29+x^21+x^20+x^15+x^13+x^12+x^11+x^8+x^7+x^6+x^2+x^1+1
**
** Note:  the x^30 tap is left off, it's implicit.
*/
#define CRC_30_POLYNOMIAL       0x6030B9C7

/* Seed value for CRC register.  The all ones seed allows detection of
** an entire data stream of zeroes.
*/
#define CRC_30_SEED             0x3FFFFFFF

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* Mask for CRC-16 polynomial:
**
**      x^16 + x^12 + x^5 + 1
**
** This is more commonly referred to as CCITT-16.
** Note:  the x^16 tap is left off, it's implicit.
*/
#define CRC_16_L_POLYNOMIAL     0x8408

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
** well as allows detection of an entire data stream of zeroes.
*/
#define CRC_16_L_SEED           0xFFFF

/* Residual CRC value to compare against return value of a CRC_16_L_STEP().
** Use CRC_16_L_STEP() to calculate a 16 bit CRC, complement the result,
** and append it to the buffer.  When CRC_16_L_STEP() is applied to the
** unchanged entire buffer, and complemented, it returns CRC_16_L_OK.
** Crc_16_l_calc() of the same entire buffer returns CRC_16_L_OK.
*/
#define CRC_16_L_OK             0x0F47



/*===========================================================================

MACRO CRC_16_L_STEP

DESCRIPTION
  This function calculates one byte step of an LSB-first 16-bit CRC over
  a specified number of data bits.  It can be used to produce a CRC and
  to check a CRC.

PARAMETERS
  xx_crc  Current value of the CRC calculation, 16-bits
  xx_c    New byte to figure into the CRC, 8-bits

DEPENDENCIES
  None

RETURN VALUE
  The new CRC value, 16-bits.  If this macro is being used to check a
  CRC, and is run over a range of bytes, the return value will be equal
  to CRC_16_L_OK (defined in crc.h) if the CRC checks correctly.  When
  generating a CRC to be appended to such a range of bytes, the final
  result must be XOR'd with CRC_16_L_SEED before being appended.

SIDE EFFECTS
  xx_crc is evaluated twice within this macro.

===========================================================================*/

/* crc_16_l_table is a pointer in the Boot Block jump table which
** points to the actual table crc_16_l_table 
*/
extern const word crc_16_l_table[];       /* Extern for macro (global) */

#define CRC_16_L_STEP(xx_crc,xx_c) \
  (((xx_crc) >> 8) ^ crc_16_l_table[((xx_crc) ^ (xx_c)) & 0x00ff])


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION CRC_16_CALC

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern word crc_16_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
);


/*===========================================================================

FUNCTION CRC_16_L_CALC

DESCRIPTION
  This function calculates an LSB-first 16-bit CRC over a specified number
  of data bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_L_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern word crc_16_l_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the LS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
);


/*===========================================================================

FUNCTION CRC_30_CALC

DESCRIPTION
  This function calculates a 30-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a double word holding 30 bits which are the contents of the
  CRC register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_30_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern dword crc_30_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
);


/*===========================================================================

FUNCTION CRC_16_STEP

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.  The CRC value
  passed in is used to continue the CRC calculation from a previous call,
  this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

extern word crc_16_step
(
  word seed,
    /* Either the result of a previous crc_16_step() or CRC_16_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word byte_len
    /* Number of data bytes to calculate the CRC over */
);

#endif /* CRC_H */