#ifndef APWRX_H
#define APWRX_H

#include "target.h"
#if (defined( FEATURE_DH) && defined(FEATURE_DH_EXP))

/*===========================================================================

DESCRIPTION
  This file contains defines for integer size independence

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/apwrx.h_v   1.0   Aug 11 2000 11:41:10   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/30/00   ks      Added casts to LOW and MSG macros to get rid of compiler
                   warnings.
01/20/99   ck      Checked in the file.

===========================================================================*/

/* <EJECT> */
/*=========================================================================*/


#define WORD long

#define BITCT (8*sizeof(unsigned WORD))
#define LOW ((unsigned WORD)(((unsigned WORD)1L << (BITCT/2)) - 1))
#define MSB ((unsigned WORD)((unsigned WORD)1L << (BITCT-1)))

/* set Nmax equal to the maximum number of bits divided by BITCT */

#define MaxBits 512
#define Nmax ((MaxBits+BITCT-1)/BITCT)

int apwrxN(int n, unsigned WORD *a, unsigned WORD *x, unsigned WORD *ax,
       unsigned WORD *p);

void InitMont(unsigned WORD *m,   /* storage for -1/p */
        unsigned WORD *p);  /* modulus */

#endif /* FEATURE_DH and FEATURE_DH_EXP */
#endif /* APWRX_H */
