#ifndef ACPBCH_H
#define ACPBCH_H
/*===========================================================================

                        ACP BCH Encoding/Decoding Module

DESCRIPTION
   This header file contains the definitions necessary to use
   the BCH Interface to encode/decode BCH data.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpbch.h_v   1.0   Aug 11 2000 11:40:32   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/94   jca     Removed extraneous comma in acpbch_decode function
                   argument list.
05/08/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
**
** BCH decoder return status
**
**-------------------------------------------------------------------------*/
typedef enum {
  ACPBCH_OK,              /* No errors found: Word is valid             */
  ACPBCH_FIXED,           /* Error found and corrected: Word is valid   */
  ACPBCH_ERROR            /* Errors found: Word is invalid              */
} acpbch_decode_status_type;

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION acpbch_enc

DESCRIPTION
  Implementation of the (n-k)-stage encoder/syndrome circuit for the 
  generator polynomial:

         g(X) = X^12 + X^10 + X^8 + X^5 + X^4 + X^3 + X^0
  
  This procedure is used to encode or generate the syndrome of a message.

DEPENDENCIES
  None

RETURN VALUE
  Value of the circuit registers.
  When encoding: Value indicates parity bits.
  When decoding: Value indicates syndrome.

  The return value is contained in the 12 LSB. The 4 MSBs are always set to 0.

SIDE EFFECTS
  None

===========================================================================*/
extern word acpbch_enc
(
  byte *buf, /* Pointer to address containing MS bit of the msg    */
  word  len        /* Length of msg (for encoding len = 36, for decoding */
                   /* len = 40)                                          */
);

/* <EJECT> */
/*===========================================================================

FUNCTION acpbch_dec

DESCRIPTION
  BCH Decoding procedure. The decoding is done by running the received word
  through the syndrome circuit (bch_enc) and verifying the syndrome. If the
  syndrome is 0, word is good. If syndrome is not 0, a lookup through the 
  syndrome table is needed. If a match is found then we can perform a 1-bit
  error correction. If no match is found the word cannot be corrected (more
  than 1-bit error).

DEPENDENCIES
  None

RETURN VALUE
  ACPBCH_OK,     Word is OK
  ACPBCH_FIXED   1-bit error found and corrected
  ACPBCH_ERROR   Word contains errors

SIDE EFFECTS
  None

===========================================================================*/
extern acpbch_decode_status_type acpbch_decode
(
  byte *buf
    /* Pointer to address containing MS bit of the msg */
);

#endif /* ACPBCH_H */
