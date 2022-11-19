#ifndef RXCPAGING_H
#define RXCPAGING_H

/*===========================================================================

                 C D M A    P A G I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the paging module that are 
  used by the rxc unit.  This header file is internal to the rxc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.

===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/rxcpaging.h_v   1.0   10 Nov 2000 16:48:14   ddalke  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* number of Paging Channel messages with good CRCs */
extern dword rxc_pc_crc_good;

/* number of Paging Channel messages with bad CRCs */
extern dword rxc_pc_crc_bad;

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION RXC_PC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Paging Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_pc_isr( void );

/*===========================================================================

FUNCTION RXC_PC

DESCRIPTION
  Paging Channel state processing.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
extern word rxc_pc ( void );

/*===========================================================================

FUNCTION RXC_PC_INIT

DESCRIPTION
  This funtion initializes the Paging Channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_pc_init
(
  word curr_state,  /* Current RXC state machine state */
  word pc_rate,     /* Rate of Paging Channel */
  qword frame_num   /* received frame number */
);

#endif /* RXCPAGING_H */
