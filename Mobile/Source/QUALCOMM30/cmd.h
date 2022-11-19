#ifndef CMD_H
#define CMD_H
/*===========================================================================

                      C O M M A N D    H E A D E R  

DESCRIPTION
  This file contains a global command header.  This header is used
  as the common header within queues which are used for intertask
  communication.  

Copyright (c) 1992 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1998,1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/MSM5000/VCS/cmd.h_v   1.1   10 Sep 1999 17:34:46   lchan  $
   
when       who     what, where, why
--------   ---     -------------------------------------------------------
09/21/98   jct     Incorporate ARM porting changes and update copyright
07/13/92    ip     Initial creation.

===========================================================================*/

#include "rex.h"
#include "queue.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Command header type.
*/

typedef struct {
  q_link_type        link;         /* Queue link */
  rex_tcb_type       *task_ptr;    /* Pointer to requesting task TCB */
  rex_sigs_type      sigs;         /* Signal to set upon cmd completion */
  q_type             *done_q_ptr;  /* Queue to place this cmd on when done */
} cmd_hdr_type;

extern void cmd_done ( cmd_hdr_type *hdr_ptr );

#endif /* CMD_H */
