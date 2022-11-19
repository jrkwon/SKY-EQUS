#ifndef PSI_H
#define PSI_H
/*===========================================================================

                   D A T A   T A S K   I N T E R N A L  
                        H E A D E R    F I L E

DESCRIPTION

  Data Task regional header file. All definitions, structures, and functions
  needed between the control and state machine units of the data task.


 Copyright (c) 1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
 Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
 Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/Psi.h_v   1.1   23 Oct 2000 20:37:58   snookala  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/10/00   na      Modified prototypes for some funtions.
04/26/00   na      Added prototype for ps_dsrlp_rx_func().
09/21/99   mvl     Added extern for psi_rlp_ready_for_more which is used by
                   psmgr.c and psctl.c.
05/13/99   ak      Removed extern for psi_wait_transmit, as that function has
                   been deleted from the psctl.c file (no longer needed).
03/09/99   ak      Added externs for psi_wait_transmit and psi_wait.
02/24/99   na/hcg  Added support for DSSock sockets interface.  Added function
                   prototypes.  Moved udp_input() to udp.c.  Removed
                   prototype for psmgr_cleanup_reseq().
12/15/98   na      Added prototype for udp_input.
10/27/98   na      Added prototype for psmgr_cleanup_reseq.
10/15/98   na      Added prototype for ps_process_tcp_send_queue
07/30/98   na      Added prototype for psmgr_register_rlp.
04/14/98   na      Added support ds_cmd_q.
10/18/96   jjw     Enhanced TCP Open process
10/17/96   jjw     Removed antiquated interface stuff. Added UP mode stuff
07/13/95   jjw     Changes for IS-99 PS task integration
10/24/94   jjw     Initial version of file


===========================================================================*/





/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "ps.h"
#include "ds.h"
#include "rex.h"
#include "mc.h"
#include "nv.h"


/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Data Task Signal Definitions
---------------------------------------------------------------------------*/

typedef rex_sigs_type ps_sigs_type;

/*---------------------------------------------------------------------------
  Command and Command Free queue definitions.
---------------------------------------------------------------------------*/
extern q_type psi_app_holding_q;
extern q_type psi_cmd_free_q;
extern q_type psi_ppp_rx_q;

/*---------------------------------------------------------------------------
  Protocol Stack Queue definitions.
---------------------------------------------------------------------------*/
extern q_type             ps_tcp_send_q;
extern dsm_watermark_type ps_to_rlp_wm_item;
extern q_type             ps_ds_cmd_q;    



#ifdef FEATURE_DS_NET_MODEL
/*---------------------------------------------------------------------------
  The variable that indicates to PS whether or not to dequeue from the SIO
  watermark.  It is defined in psmgr.c
---------------------------------------------------------------------------*/
extern boolean psi_rlp_ready_for_more;
#endif /* FEATURE_DS_NET_MODEL */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION PSI_START_TIMER

DESCRIPTION
  This function will set the timer associated with the passed signal mask
  to the specified duration. The time units are in milli-seconds. The
  specified signal will also be cleared.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void psi_start_timer
(
  ps_sigs_type  sig,                     /* signal associated with a timer */
  dword         duration                 /* time interval, in msecs        */
);

/*===========================================================================

FUNCTION PSI_CANCEL_TIMER

DESCRIPTION
  This function will cancel the timer associated with the specified signal
  mask. The specified signal mask will also be cleared. NOTE that the signals
  are associated with timers during initialization by the REX_DEF_TIMER 
  function.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void psi_cancel_timer
(
  ps_sigs_type sig                       /* signal associated with a timer */
);

/*===========================================================================

FUNCTION PSI_GET_TIMER

DESCRIPTION
  This function will return the time remaining in milliseconds for the timer
  associated with the passed signal

DEPENDENCIES
  None.

RETURN VALUE
  The time in milliseconds remaining for the timer associated with the passed
  signal mask.

SIDE EFFECTS
  None.
===========================================================================*/

extern dword psi_get_timer
(
  ps_sigs_type sig                       /* signal associated with a timer */
);

/*===========================================================================

FUNCTION PSI_INIT

DESCRIPTION
  Initialization function for the data services task. This function 
  performs the functions needed for the data task to exit disabled state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern ps_sigs_type psi_init( void);

/*===========================================================================

FUNCTION PSI_MGR

DESCRIPTION
  
DEPENDENCIES
  None.

RETURN VALUE
  A signal mask for the next set of state signals to use for the wait 
  function.

SIDE EFFECTS
  None.
===========================================================================*/

extern ps_sigs_type psi_mgr
(
  ps_sigs_type signal_mask                /* mask of signals that occurred */
);

/*===========================================================================

FUNCTION PSMGR_REGISTER_RLP

DESCRIPTION
 This function is used to register RLP. This is called externally from dsmgr.
 This was exported, so that all the variables need to be externalized.
 
 This registers RLP for RAWDATA and Unwired Planet modes.
 
 This function is called in the context of the DS  task.
 
 Note, this function was added because of the effect of changing DS-PS task
 priorities.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void psmgr_register_rlp( void);


/*===========================================================================

FUNCTION PS_PROCESS_TCP_SEND_QUEUE

DESCRIPTION
  This function processes all the items that the application layers want TCP
  to transmit.
  
  The items that sent by the serial port are escaped for 617 and Telnet 
  characters, while the 617 commands or responses are enqueued as is into the
  TCP send queue.

  The argument ack_piggyback means that this function is called when a TCP
  acknowledgement has to be sent. If this argument is TRUE, only one segment
  is transmitted, otherwise segments are transmitted till the psi_tcp_send_q
  is empty.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_process_tcp_send_queue
( 
  struct tcb* tcb_ptr,                     /* Pointer to TCP control block */
  boolean ack_piggyback                    /* If TCP ACK is piggybacked    */      
);


/*===========================================================================

FUNCTION PS_DONT_EXCEED_MEM_EVENT

DESCRIPTION
 This function is called when the donot exceed count is reached in the memory
 pool.
  
 This function is used to cleanup the TCP resequence queues and the 
 psi_ppp_rx_q, when the system is running out of memory. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dont_exceed_mem_event
(
  void
);

/*===========================================================================

FUNCTION PS_LO_MARK_MEM_EVENT

DESCRIPTION
 This function is called when the lowatermark count is reached in the memory
 pool.

 This can be executed in any task or ISR context.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_lo_mark_mem_event
(
  void
);

/*===========================================================================

FUNCTION PS_PROCESS_UDP

DESCRIPTION
  This function processes the passed UDP packet. 
  
  For unwired planet calls, it stores the packet in the psi_udp_q and sends a
  command to the UI task, which will remove the packet from the queue.
  
  For socket calls, this function stores the packet in the UDP receive queue
  for the socket, and informs the application if it is waiting for the READ
  event.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_process_udp
( 
  dsm_item_type* item_ptr,            /* Contains payload and socket info. */
  word self_port                         /* Dest. port # in the UDP packet */
);

#if defined( FEATURE_DS_RLP1) || defined( FEATURE_DS_RLP2) || \
    defined( FEATURE_DS_RLP3)
/*===========================================================================

FUNCTION PS_DSRLP_RX_FUNC

DESCRIPTION
  This function is called when a RLP frame is received. It sets a signal to 
  the PS task.
  This function is called when the dsrlp*.c is used.

  This function runs in the context of the RX task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dsrlp_rx_func( void);
#endif /* DS_RLPs defined */

#if (defined(WAP_USE_PZID2) || defined(FEATURE_SKT_DS)) && defined(FEATURE_DS_PZID_RECON)
void ps_process_pzid(void);
#endif /* (WAP_USE_PZID2 ||  FEATURE_SKT_DS) && FEATURE_DS_PZID_RECON) */

#endif /* PSI_H */

