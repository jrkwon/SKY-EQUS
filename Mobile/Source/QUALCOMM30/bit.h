#ifndef BIT_H
#define BIT_H
/*===========================================================================

              B I T    M A N I P U L A T I O N    S E R V I C E S

                            D E C L A R A T I O N S

DESCRIPTION
  The following declarations are for use with the Bit Manipulation Services.

Copyright (c) 1991-1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/bit.h_v   1.1   10 Sep 1999 17:34:46   lchan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/31/99    ms     Added prototypes for b_copy_ltobw_s2d0 and b_copy_ltobw_s2d0
12/15/98   jct     Removed 80186 support
11/20/98    ms     Added prototype for function b_copy_litend_to_bigend_word.
08/28/98   jct     Added updates from ARM port, added comments, moved debug
                   definitions to this header
12/20/96   rdh     Changed functions to Pascal calling convention.
07/07/92   ip      Release to DMSS PVCS.
04/29/92   gb      Large model version.
04/23/92   ip      Initial porting of file from Brassboard to DMSS.
01/23/91   rdb     Created first cut of file.

===========================================================================*/

#include "comdef.h"
#include "qw.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

extern void  b_packb ( byte  val, byte buf[ ], word pos, word len );
extern void  b_packw ( word  val, byte buf[ ], word pos, word len );
extern void  b_packd ( dword val, byte buf[ ], word pos, word len );
extern void  b_packq ( qword val, byte buf[ ], word pos, word len );

extern byte   b_unpackb ( byte buf[ ], word pos, word len );
extern word   b_unpackw ( byte buf[ ], word pos, word len );
extern dword  b_unpackd ( byte buf[ ], word pos, word len );
extern void   b_unpackq ( byte buf[ ], word pos, word len, qword qw );


extern void  b_copy ( 
                   void  *sbuf,
                   word  spos,
                   void  *dbuf,
                   word  dpos,
                   word  len      );

extern void  b_copy_litend_to_bigend_word ( 
                   word  *sbuf,
                   word  spos,
                   byte  *dbuf,
                   word  dpos,
                   word  len      );

extern void  b_copy_ltobw_s1d0 ( 
                   word  *sbuf,
                   byte  *dbuf,
                   word  len      );

extern void  b_copy_ltobw_s2d0 ( 
                   word  *sbuf,
                   byte  *dbuf,
                   word  len      );

#endif /* BIT_H */
