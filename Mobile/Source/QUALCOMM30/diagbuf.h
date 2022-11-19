#ifndef DIAGBUF_H 
#define DIAGBUF_H

/*==========================================================================

 D I A G N O S T I C S   B U F F E R   A L L O C A T I O N    M A N A G E R

DESCRIPTION
  Type and function definitions for the DIAG output allocation manager.

Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

      $Header:   L:/src/asw/COMMON/vcs/diagbuf.h_v   1.2   05 Dec 2000 20:01:44   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/05/00   lad     Added MAX response packet length.
11/07/00   jal     Renamed DIAGBUF_DATA_AVAIL_S to DIAGBUF_READY_S
09/01/00   pj      Increased DIAGBUF_MAX_SIZE to 10KB. Such a large buffer is
                   considered a temporary measure until the GPS SRCH code can
                   be sped up (it is currently starving the diag task, thus
                   requiring a large buffer).
08/28/00   lad     Removed use of DIAGBUF_MAX_SIZE.
06/19/00   lad     Moved some macros and prototypes from diagi.h.
05/15/00   lad     Updated load balancing.
05/10/00   lad     Added DIAGBUF_NICE_SIZE for load balancing.  Updated API.
02/28/00   lad     Created file.

===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"

#include "comdef.h"

#include "diagpkt.h"



/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed to interface with the DIAG output allocation manager.

===========================================================================*/

/* Increase Buffer size to handle the cases when GPS multipeak logging */
/* is turned on. Since the GPS Math takes a large chunk of CPU time, many */
/* other tasks do not get a chance to get scheduled. This doesn't affect  */
/* most of the operations, since during GPS visit, CDMA RF chain is shut  */
/* down, but if Searcher task is also logging the Search data, and we can */
/* not flush it fast enough on SIO since we don't get scheduled, we start */
/* dropping logs.                                                         */
/* This situation will get corrected once GPS floating point MATH routines*/
/* are optimized, and buffer size can be reduced to original size again.  */

#if defined( FEATURE_BIG_DIAG_BUFFER_FOR_GPS_SRCH )
#error code not present
#else
#define DIAGBUF_SIZE 2048
#endif

/* Some external devices in the past shared diagpkt.h with the DMSS.  
** This is the max size they expect, so we don't go larger that 944
** until those external devices are updated and distributed to handle
** arbitrary packet lengths.
**
**  Also, it is also to remember that this number must always be >= to
**  DIAGBUF_SIZE - sizeof(uint16) - sizeof(uint16), and would still
**  be really unlikely to alloc successfully (unless it is a guaranteed 
**  response).
*/
#define DIAGBUF_MAX_PKT_LEN 944

/* For load balancing, all messages may can use this to calculate niceness */
#define DIAGBUF_BALANCE_PAD 0x140

typedef enum
{
  DIAGBUF_EMPTY_S,        /* Nothing in ring buffer or event buffer        */
  DIAGBUF_READY_S,        /* Data from ring buffer queued to DSM, but ring */
                          /* buffer still has more data to be queued--make */
                          /* sure diagbuf_drain() gets called again soon!  */
  DIAGBUF_NOT_READY_S     /* Ring/event buffer has data to be queued to    */
                          /* DSM, but we're blocked from queueing because  */
                          /* it's uncomitted, we don't have an SIO stream  */
                          /* yet, or we're flow-controlled by DSM.         */
} diagbuf_status_enum_type;


/*==========================================================================

                                  MACROS!!!

===========================================================================*/
/*----------------------------------------------------------------------------
   Macro to build error packet -- Note that the commas cause the sizeof
   to be returned when this is used in an assignment, e.g.
   ret_val = ERR_PKT ( something );
----------------------------------------------------------------------------*/
diag_rsp_type * diag_error_packet(byte code,
                                  diag_req_type *req_ptr);

#define ERR_PKT( code ) \
  diag_error_packet (code, (diag_req_type *) req_ptr)


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION DIAGBUF_INIT

DESCRIPTION
  This function initializes the diag allocation buffer at startup.

DEPENDENCIES
  None.
         
RETURN VALUE
  None.
      
SIDE EFFECTS
  None.

============================================================================*/
extern void diagbuf_init(void);



/*===========================================================================

FUNCTION DIAGBUF_ALLOC

DESCRIPTION
  This function allocates the specified amount of space in the DIAG output 
  buffer.

DEPENDENCIES
  diagbuf_commit() must be called to commit a buffer to the DIAG pool.  This 
  must happen reasonably soon after this call completes.  If the buffer is
  needed to accumulate data over time, then you should use your own buffer
  use this just long enough to copy the data.
       
RETURN VALUE
  Pointer to allocated buffer.  NULL if space is not available.
      
SIDE EFFECTS
  diagbuf_head may be updated.

============================================================================*/
extern diag_rsp_type* diagbuf_alloc ( 
  int length, /* Length of requested buffer */
  int nice    /* Niceness padding value     */
);



/*===========================================================================

FUNCTION DIAGBUF_PKT_ALLOC

DESCRIPTION
  This function allocates the specified amount of space in the DIAG output 
  buffer.  If space is unavailable, access to the allocation buffer will be
  blocked until space is available.  This function does the same thing as
  diagbuf_alloc() with the addition of blocking.

DEPENDENCIES
  This may only be called by the DIAG task for response packets!  This
  function is *not* re-entrant.
  
  diagbuf_commit() must be called to commit a buffer to the DIAG pool.  This 
  must happen reasonably soon after this call completes.
       
RETURN VALUE
  Pointer to allocated buffer.
      
SIDE EFFECTS
  diagbuf_head will be updated.
  More logs/messages may be dropped while DIAG is blocking for space.

============================================================================*/
extern diag_rsp_type * diagbuf_pkt_alloc(int length);



/*===========================================================================

FUNCTION DIAGBUF_SHORTEN

DESCRIPTION
  This function reduces the length field of a previously allcated buffer. 

DEPENDENCIES
  diagbuf_alloc() or diagbuf_pkt_alloc() must be called prior to this call,
  and 'ptr' must be the pointer that was returned from that call.
       
RETURN VALUE
  None.
      
SIDE EFFECTS
  Using this method will reduce buffer availability.  Use sparingly!

============================================================================*/
void diagbuf_shorten (
  diag_rsp_type *ptr, /* Pointer returned from diagbuf_alloc() */
  int length          /* New length of buffer                  */
);



/*===========================================================================

FUNCTION DIAGBUF_COMMIT

DESCRIPTION
  This function commits previously allocated space in the DIAG output buffer.

DEPENDENCIES
  Must first have allocated space using diagbuf_alloc() 
  or diagbuf_pkt_alloc().
  
RETURN VALUE
  None.
      
SIDE EFFECTS
  This function signals the DIAG task and may cause a context switch!

============================================================================*/
extern void diagbuf_commit
( 
  diag_rsp_type* ptr
);


#if !defined(FEATURE_DIAG_NON_STREAMING)

/*===========================================================================

FUNCTION DIAGBUF_DRAIN

DESCRIPTION
  This function drains the DIAG allocation buffer to the SIO DSM queues.
  Escaping, etc is done in here.

DEPENDENCIES
  This function may only be called by the DIAG task.  It is not re-entrant!

RETURN VALUE
  If the buffer is empty, FALSE is returned.  If non-empty, TRUE is returned.
  
SIDE EFFECTS
  None.

===========================================================================*/
extern diagbuf_status_enum_type diagbuf_drain( rex_sigs_type );



/*===========================================================================

FUNCTION DIAGBUF_STOP_STREAMING

DESCRIPTION
  This function configures all streaming data items off.  No more allocations
  will occur until these settings are changed.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  DIAG will not stop streaming immeidiately, the data currently allocated 
  will still be streamed.

===========================================================================*/
extern void diagbuf_stop_streaming (void);


#else /* !defined(FEATURE_DIAG_NON_STREAMING) */

/*===========================================================================

FUNCTION DIAGBUF_GET_LOG

DESCRIPTION
  This function emulates the old (DIAG version  7) poll/response log
  protocol by retrieving one log at a time from the ring buffer.

DEPENDENCIES
  None.

RETURN VALUE
  Null.
  
SIDE EFFECTS
  A log item is removed from the ring buffer or log queue.
  
===========================================================================*/
extern diag_rsp_type * diagbuf_get_log( void );

#endif /* !defined(FEATURE_DIAG_NON_STREAMING) */
#endif /* DIAGBUF_H  */
