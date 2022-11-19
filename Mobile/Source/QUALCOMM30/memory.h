#ifndef MEMORY_H
#define MEMORY_H
/*==========================================================================

                         Standard Memory Header 
                         
DESCRIPTION
  The ARM compiler does not include a memory.h header, so we provide one
  here and point to the standard ARM include file which does have
  memory prototypes.

Copyright (c) 1997,1998 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999      by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/memory.h_v   1.2   01 May 2000 15:57:26   rajeevg  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/01/00    gr     Modified to pull in string.h for NT builds.
11/03/98   jct     Added the else clause to include <memory.h> for non-ARM builds.
07/10/98   jct     Revised for coding standard, removed unused code
01/01/98   bwj     Created

===========================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

#ifdef TG
  #include "target.h"
#endif

#if defined(T_ARM) || defined(T_WINNT)
   /* --------------------------------------
   ** For the ARM Compiler, the prototypes
   ** which are typcially found in memory.h
   ** are instead in string.h
   ** -------------------------------------- */ 
   #include <string.h>

#endif

#endif /* END MEMORY_H */
