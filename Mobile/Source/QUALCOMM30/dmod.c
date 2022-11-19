/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              D E M O D U L A T O R    C H I P    S E R V I C E S

GENERAL DESCRIPTION
  This file contains declarations and definitions to support interaction
  with the QUALCOMM CDMA Demodulator Chip.

EXTERNALIZED FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 1990, 1997, 1998, 1999, 2000, 2001
                    by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dmod.c_v   1.3   18 Jan 2001 16:31:56   evanmell  $
   
      Rev 1.0   13 Aug 1992 13:38:34   PWILLIAMSON
   Initial revision.
   
      Rev 1.0   28 Mar 1991 08:02:26   JIsaac
   Initial revision.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/18/01   ejv     Added MSM5100 support.
11/10/00   rmd     Moved 8bit/16bit shadow register support to the hw code.
08/06/99   ejv     Mainlined T_MSM3 support.  Added intial Tiger2 support.
01/13/99   ejv     Removed #ifndef T_MSM3 code.
10/16/98   ejv     Merged changes to support Panther.
04/03/97   fkm     Updated PVCS from Log to Header so we get proper PVCS
                   tracking.
03/14/91   jai     Created module. 

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "processor.h"
#include "dmod.h"
#include "target.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*-------------------------------------------------------------------------*/

/* Array used by software to buffer the write ports related to the
     fingers */
#ifdef T_MSM_5100
#error code not present
#else
  byte MSM_fimage[ DMOD_NUM_F ][ MSM_FMAX_INX ];
#endif

#ifndef FEATURE_16BIT_SHADOW_REG
/* Array used by software to buffer the write ports. */
    byte MSM_image[ MSM_MAX_INX ];

/* Array used by software to buffer the WORD (16bits) write ports
   of the MSM3000 */
    word MSM_image_H[ MSM_MAX_H_INX ];
#endif
