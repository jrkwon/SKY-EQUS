#ifndef RXCDEMUX_H
#define RXCDEMUX_H
/*===========================================================================

           C D M A    D E -M U L T I P L E X I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the receive de-multiplexing module 
  that are used by the rxc unit.  This header file is internal to the rxc 
  unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/rxcdemux.h_v   1.0   10 Nov 2000 16:48:12   ddalke  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00   jc      Created after repartition of rxc.c

===========================================================================*/
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION RXC_TC_DATA

DESCRIPTION
  This procedure processes data received from the decoder while the Receive
  task is processing the Traffic Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void rxc_tc_data( void );

#endif /* RXCDEMUX_H */
