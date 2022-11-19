#ifndef TXCMUX_H
#define TXCMUX_H

/*===========================================================================
                 
             C D M A     M U L T I P L E X I N G    M O D U L E

DESCRIPTION
  This header file contains definitions for the multiplexing module that 
  are used by the txc unit.  This header file is internal to the txc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/txcmux.h_v   1.1   07 Dec 2000 14:56:18   bcalder  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/07/00   bgc     Fixed typo for FEATURE_PLT.
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "log.h"
#include "queue.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#if !defined (FEATURE_DIAG_V7_LOG_IFACE)
/* Log buffer for accumulated MUX frame log */
typedef PACKED struct 
{
  log_r_mux1_type       entry;
  byte                  count;
}txc_log_mux_buf_type; 

extern txc_log_mux_buf_type txc_log_mux_buf; /* Log buffer for accumulated Markov frame log */

#define TXC_SEND_MUX1_LOG()                                     \
  do {                                                          \
    log_submit((log_type *) &txc_log_mux_buf);                  \
                                                                \
    /* Re-initialize counters */                                \
    txc_log_mux_buf.count = 0;                                  \
    txc_log_mux_buf.entry.hdr.len = FSIZ(log_r_mux1_type, hdr); \
  } while (0)

#else

extern q_type txc_log_mux1_q;             /* for logging traffic frame statistics */
/* buffers for txc_log_mux1_q */
#define                            TXC_NUM_MUX1_BUFS 2
extern txc_log_mux1_type txc_log_mux1_pool[ TXC_NUM_MUX1_BUFS ];
#endif /*FEATURE_DIAG_V7_LOG_IFACE*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION TXC_PRI

DESCRIPTION
  This function provides the primary traffic to the Multiplex sublayer
  dependant on what the current primary service option is set to.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_pri (void);

#ifdef FEATURE_PLT
#error code not present
#endif

/*===========================================================================

FUNCTION TXC_BUILD_FRAME

DESCRIPTION
  This function builds the next frame to be transmitted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void txc_build_frame ( void );

#endif /*TXCMUX_H*/

