#ifndef MCLK_5000_H
#define MCLK_5000_H

/*===========================================================================

   M I C R O   C L O C K   I N T E R N A L    H E A D E R    F I L E

DESCRIPTION
  This header file contains all the internal definitions used by the
  dynamic micro clock switching code.

  Copyright (c) 1999, 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/mclk_5000.h_v   1.0   15 Feb 2001 17:23:46   robertom  $   

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/12/01   rmd     Initial Revision

===========================================================================*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/


#if defined(FEATURE_MCLK_DYNAMIC_SPEED_CTRL)

/* This type definition specifies the different speed modes of the MCLK */
typedef enum {
        CLK_SLOW,
        CLK_NORM,
        CLK_FAST
} mclk_speed_type;

/*===========================================================================

FUNCTION MCLK_CONVERT_SCH_RATE_TO_SPEED

===========================================================================*/
void mclk_convert_sch_rate_to_speed(byte fsch_rate, byte rsch_rate);

/*===========================================================================

FUNCTION MCLK_SWITCHING_THROTTLE


===========================================================================*/
void mclk_switching_throttle(mclk_speed_type mclk_speed);

#endif /*FEATURE_MCLK_DYNAMIC_SPEED_CTRL*/
#endif /*MCLK_5000_H*/

