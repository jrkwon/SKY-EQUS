#ifndef PS_H
#define PS_H
/*===========================================================================

                   D A T A   P R O T O C O L   T A S K
                        H E A D E R    F I L E

DESCRIPTION
  This is the header file for the Data protocol Task. Contained herein are
  all the definitions, functions, and structures for other software tasks
  to communicate with the Data Protocol Task.


 Copyright (c) 1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
 Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
 Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
 Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/ps.h_v   1.4   18 Jan 2001 12:22:58   varavamu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/18/01   va      Fixed some compiler errors when T_WINNT is defined. 
10/30/00   snn     Removed the following timer rex signals:
                     PS_TCP_DE_ACK_TIMER_SIG.   0x0040 
                     PS_TCP_REXMIT_TIMER_SIG    0x0008
                     
10/25/00   snn     Removed the Feature define around ps_in_rawdata_mode
10/10/00   snn     Added a new signal called PS_TIMER_TICK_SIG for multiple
                   TCP Sockets support.
06/10/00   snn     Added a new signal called PS_DNS_SIG to be used for 
                   issuing a signal when DNS related task should be executed.
05/09/00   rc      Added prototype for function ps_in_rawdata_mode().
04/12/00   mvl     Added header for the ps_send_to_ds() function which has
                   now been externalized from psmgr.c.
03/10/00   snn     Defined the Timer for the CHAP. Note that the CHAP Timer 
                   Signal is same as the LCP timer Signal. Hence, the signal 
                   value should be same as the value that is used for the 
                   LCP Timer.  This synchronization has to be maintained.
11/04/99   rc      Added prototype for function ps_set_app_data_flow().
09/27/99   snn     Moved spaces in comments to follow coding standards.
09/01/99   mvl     Added function headers for RLP hi and low watermark
                   function calls for network model.
08/10/99   snn     Added support for PAP timeouts.
08/09/99   mvl     Added network model changes.
05/13/99   ak      Removed signal which was used when closing stream.  PS
                   task now never closes the sio stream; it only changes its
                   mode, which doesn't require the use of signals.
03/09/99   ak      Added signal to be used when closing a stream.
02/24/99   na/hcg  Added PS_SOCKET_RX_Q_SIG, and socket-specific signal
                   definitions.
12/15/98   na      Added prototype for ps_send_browser_payload() and
                   ps_recv_browser_payload().
12/14/98   ldg     Added prototype for ps_initiate_upper_layer().
09/03/98   ldg     Removed declaration for ps_to_ds_wm_item.
04/14/98   na      Added prototype for ps_tcp_enqueue.
10/17/96   jjw     Changes for UP mode. Removed antiquated I/F stuff.
11/09/95   jjw     Completed changes to support fully compliant IS-99
07/13/95   jjw     Changed Sigs/etc to handle PS task interfacing/IS-99
10/10/94   jjw     Initial version of file


===========================================================================*/


#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "dsm.h"

#ifdef FEATURE_DS_SOCKETS
#ifndef T_WINNT
#include "tcp.h"
#endif
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Data Protocol Task Signal Definitions
---------------------------------------------------------------------------*/

#define PS_TCP_RX_Q_SIG            0x0001 /* Incoming TCP data signal      */
#define PS_DS_CMD_Q_SIG            0x0002 /* DS-PS command Q signal        */
#define PS_PPP_RX_Q_SIG            0x0004 /* Incoming PPP data signal      */
#define PS_PPP_IPCP_TIMER_SIG      0x0010 /* PPP IPCP Timer signal         */
#define PS_PPP_LCP_TIMER_SIG       0x0020 /* PPP LCP Timer signal          */

#define PS_DOG_RPT_TIMER_SIG       0x0080 /* Watchdog Report timeout signal*/
#define PS_STATE_TIMER_SIG         0x0100 /* PS State Timer signal         */

#ifdef FEATURE_DS_SOCKETS
 #define PS_SOCKET_RX_Q_SIG        0x0200 /* Socket API has data to send   */
 #define PS_SOCKET_STATE_TIMER_SIG 0x0800 /* Socket state timer sig        */
#endif

#define PS_MEM_EVENT_SIG          0x0400 /* Memory pool event has happened */

#ifdef FEATURE_DS_NET_MODEL
 #define PS_RM_PPP_RX_Q_SIG        0x1000 /* netmodel Rm PPP RX            */
#endif

/*---------------------------------------------------------------------------
  This signal is given by application task, PS task, timer to the PS task. On 
  receiving this signal the PS task should call the resolver() function. This 
  function, will have its own state variable, based on that variable, it 
  executes the appropriate functions.
  Note this has been made as 0x10000 because, 0x2000, 0x4000 and 0x8000 are
  already chosen for Task related signals, but not shown in the table above. 
---------------------------------------------------------------------------*/
#ifdef FEATURE_DS_SOCKETS
 #define PS_DNS_SIG                0x10000 /* Transmit domain name to DNS  */
#endif

/*---------------------------------------------------------------------------
  This signal is given by the rex when the API timer expires. API timer 
  starts the first time some task uses the pstimer mecahnism. When this
  signal is set, the PS task will decrement the timer on the head of the list
  and if it becomes zero, then its related call back is called. It also 
  checks if any subsequent timers have gone off (since it is delta list, the
  value should be zero to go off).
---------------------------------------------------------------------------*/
#define PS_TIMER_TICK_SIG          0x20000 /* API Timer expired            */



#ifdef FEATURE_DS_PAP
/*---------------------------------------------------------------------------
  PAP Timer signal definition
  Note that the PAP Timer Signal is same as the LCP timer Signal. Hence, the 
  signal value should be same as the value that is used for the LCP Timer. 
  This synchronization has to be maintained.
---------------------------------------------------------------------------*/
#define PS_PPP_PAP_TIMER_SIG       PS_PPP_LCP_TIMER_SIG  
#endif /*  FEATURE_DS_PAP */

#ifdef FEATURE_DS_CHAP
/*---------------------------------------------------------------------------
  CHAP Timer signal definition
  Note that the CHAP Timer Signal is same as the LCP timer Signal. Hence, the 
  signal value should be same as the value that is used for the LCP Timer. 
  This synchronization has to be maintained.
---------------------------------------------------------------------------*/
#define PS_PPP_CHAP_TIMER_SIG       PS_PPP_LCP_TIMER_SIG  
#endif /*  FEATURE_DS_CHAP */

/*--------------------------------------------------------------------------
  PS Task Command links and queues
---------------------------------------------------------------------------*/
#ifdef FEATURE_DS_SOCKETS
extern q_type   ps_socket_send_q;
#endif


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/*===========================================================================

FUNCTION PSI_PROTOCOL_CLOSED_EVENT

DESCRIPTION
  This function will report to the DS task that the protocol stack is
  completely closed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void psi_protocol_closed_event( void);


/*===========================================================================

FUNCTION PS_TASK

DESCRIPTION
  This function is the entry point and main processing loop for the Data
  Service(s) task.

DEPENDENCIES
  None.

RETURN VALUE
  Does not Return.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ps_task
(
  dword dummy           /* Parameter required for REX. Tell lint to ignore */
                        /* lint -esym(715,dummy) */
);

/*===========================================================================
FUNCTION PS_TCP_ENQUEUE

DESCRIPTION
  This function adds the passed dsm item to the ps_tcp_send_q. It does do not
  exceed and hiwater checks for the entire sendq stored in the phone.

  It can be called at ISR level by the serial driver to send serial data or at
  the DS task level by dsatcop to send 617 reflected commands and results.

DEPENDENCIES
  Passed item pointer should be a single item and not a chain.

RETURN VALUE
  The value of the passed item ptr becomes NULL after this function.

SIDE EFFECTS
  Changes ps_tcp_send_q and psi_tcp_send_q_cnt.
===========================================================================*/

void ps_tcp_enqueue
(
  dsm_item_type **item_ptr
);

#ifdef FEATURE_DS_BROWSER_INTERFACE
/*===========================================================================

FUNCTION PS_SEND_BROWSER_PAYLOAD

DESCRIPTION
  This function is invoked by the UI task. The  browser calls this function
  when there is data to send and the protocols are up. This function makes
  the UDP header and sends the payload to IP.

  Invoked External from PS task.

DEPENDENCIES
  None

RETURN VALUE
  Returns -1 on failure and 0 on success.

SIDE EFFECTS
  None.
===========================================================================*/

int ps_send_browser_payload
(
  struct socket* destaddr,              /* destination IP address & port # */
  uint16 source_port,                       /* source port for this packet */
  void       *data,                          /* ip datagram payload        */
  long       length                          /* length of datagram payload */
);
/*===========================================================================

FUNCTION PS_RECV_BROWSER_PAYLOAD

DESCRIPTION
  This function is called by the browser, when it gets an indication that
  there is data available. This function copies the incoming  packet to the
  buffer provided by the browser.

  Invoked External from PS task.

DEPENDENCIES
  None

RETURN VALUE
  Returns 0 on success.

SIDE EFFECTS
  None.
===========================================================================*/
int ps_recv_browser_payload
(
  struct socket* srcaddr,                    /* source IP address & port # */
  void       *data,                          /* ip datagram payload        */
  long       *length                         /* length of datagram payload */
);
#endif /* FEATURE_DS_BROWSER_INTERFACE */



#if (defined(FEATURE_DS_PAP) || defined(FEATURE_DS_CHAP)) || defined(DNSIP)	//	khekim 00/11/22
/*===========================================================================
FUNCTION PS_IN_RAWDATA_MODE

DESCRIPTION
  This functions checks to see if ps is in PS_RAWDATA_MODE or not.
  If ps is in PS_RAWDATA_MODE, the function returns 
  TRUE, else the functions returns FALSE.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If ps is in rawdata mode.
  FALSE: If ps is in null, sockets or Browser mode.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean ps_in_rawdata_mode(void);

#endif /* FEATURE_DS_PAP || FEATURE_DS_CHAP */



/*===========================================================================

FUNCTION PS_INITIATE_UPPER_LAYER

DESCRIPTION
  This function will initiate the Opening of TCP for Async/Fax service or IP
  for UP client service.

DEPENDENCIES
  This function must only be called upon completion of the PPP IPCP layer
  Open.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void ps_initiate_upper_layer( void);

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

FUNCTION PS_PROCESS_SOCKET_STATE_TIMER_SIG

DESCRIPTION
  This function will perform cleanup after a TCP socket is closed.

DEPENDENCIES
  Must be called at the end of a TCP socket call.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_process_socket_state_timer_sig
(
 struct tcb* tcb_ptr
);
#endif

#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION PS_NETMDL_RLP_HI_WM_FUNC

DESCRIPTION
  This is the function that is called when the hi watermark is reached for
  the RLP watermark in a network model call.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef NOT_USE_IN_WAP //ytchoi 01.02.01 because WARNING
LOCAL void ps_netmdl_hi_wm_func
(
  void
);

/*===========================================================================
FUNCTION PS_NETMDL_RLP_LO_WM_FUNC

DESCRIPTION
  This is the function that is called when the hi watermark is reached for
  the RLP watermark in a network model call.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
LOCAL void ps_netmdl_lo_wm_func
(
  void
);
#endif //NOT_USE_IN_WAP
#endif /* FEATURE_DS_NET_MODEL */


/*===========================================================================

FUNCTION PS_SET_APP_DATA_FLOW

DESCRIPTION
  This function will enable or disable data flow over Um by setting
  the psi_app_tx_enabled variable.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void ps_set_app_data_flow 
(
  boolean state                           /*  state of psi_app_tx_enabled  */
);

/*===========================================================================
FUNCTION PS_SEND_TO_DS

DESCRIPTION
  This function sends the message specified in kind to dsmgr. This
  function's main purpose is to consolidate the sending code in one
  place.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_send_to_ds
(
  word kind                               /* kind field in dsm_item_type   */
);

#endif /* PS_H */
