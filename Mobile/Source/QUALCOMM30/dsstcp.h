#ifndef DS_DSSTCP_H
#define DS_DSSTCP_H

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

     D A T A  S E R V I C E S  S O C K E T  T C P  H E A D E R  F I L E
                   
DESCRIPTION

 The Data Services sockets TCP interface file. This contains all the 
 functions used by the sockets library to access TCP socket function calls.
 
EXTERNALIZED FUNCTIONS

  dsstcp_socket()
    TCP specific socket() function.  Sets socket state to INIT.

  dsstcp_connect_setup()
    Sets up specific parameters for TCP sockets.  Called prior 
    dsstcp_connect(), it sets up the appropriate state of the socket
    so that the correct error response will be generated as needed.

  dsstcp_connect()
    Connects a TCP socket.

  dsstcp_write()
    TCP specific write() function.  Sends the dsm memory item out
    the TCP transport.

  dsstcp_read()
    TCP specific read() function.  Reads specified number of bytes from
    the TCP transport.

  dsstcp_read_dsm_chain()
    TCP specific read_dsm_chain() function.  Reads a DSM item chain from
    the TCP transport.

  dsstcp_close()
    TCP specific close() function.  Initiates the TCP active close.
    If the connection is already closed, cleans up the TCP socket and 
    associated resources.

  dsstcp_abort()
    TCP specific abort() function. Aborts the TCP connection and cleans-up.
    This is usually called when we are dormant.
    

  dsstcp_get_conn_error()
    Gets the appropriate error code for connection errors.

  dsstcp_lookup_conn()
    Given a connection structure, looks up the connection to determine
    if this is a valid connection. 
    
  dsstcp_new_ip_upcall()
    If a new IP address is negotiated with the Base Station, then send a 
    reset to the peer, and then close down sockets and notify the user.       
    
  dsstcp_bind()
    This is the TCP specific bind() function. It writes the port value into
    local.conn.port of the socket control block structure.

Copyright (c) 1998 - 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dsstcp.h_v   1.3   09 Feb 2001 13:21:02   louiel  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/08/01   yll     Added dsstcp_read_dsm_chain().
11/10/00   snn     Changed scb_type to scb_s
10/10/00   na      Removed tcp_ptr which is not longer used. Added prototype
                   for dsstcp_init_tcb_array().
08/03/00   mvl/js  Added prototype for dsstcp_bind() function.
03/22/00   rc/na   Added prototype for function dsstcp_new_ip_upcall().
09/22/99   rc      Incorporated code review changes and comments.
08/11/99   rc/ak   Extern for tcp abort function.
12/14/98   na/hcg  Created module.

===========================================================================*/


/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "dssocki.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION DSSTCP_SOCKET

DESCRIPTION
  This function is the protocol-specific socket function.  For TCP, it
  just sets the socket state to INIT.
  
  This function is called from the context of the application task.

DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS
  
  On error, return DSS_ERROR and places the error condition value in *errno.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dsstcp_socket
(
  struct scb_s* scb_ptr,         /* Ptr to socket control block for the socket */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSTCP_CONNECT_SETUP

DESCRIPTION
  This function sets up specific parameters for TCP sockets, and should
  be called prior to making a call to the TCP connect function. Specifically
  it sets the appropriate state of the socket, within the context of the
  calling application, so that the correct error responses will be generated
  as needed.

DEPENDENCIES
  None.

RETURN VALUE
  Returns DSS_SUCCESS.

SIDE EFFECTS
  None.
===========================================================================*/
extern sint15 dsstcp_connect_setup
(
  struct scb_s* scb_ptr          /* Ptr to socket control block for the socket */
);

/*===========================================================================

FUNCTION DSSTCP_CONNECT

DESCRIPTION
  This function connects a TCP socket and should only be called when 
  PPP/traffic channel is established.
    
  This function is called from the context of the PS  task.

DEPENDENCIES
  Before this function is called, the destination IP address/port number
  should have been put in the scb in the name and peername fields.

RETURN VALUE
  On success, returns DSS_SUCCESS.

SIDE EFFECTS
  None.
===========================================================================*/
extern sint15 dsstcp_connect
(
  struct scb_s* scb_ptr      /* Ptr to socket control block for the socket */
);

/*===========================================================================

FUNCTION DSSTCP_WRITE

DESCRIPTION
  Sends dsm memory item out to TCP.
  
  This function is called from the context of the PS task.

DEPENDENCIES
  None.

RETURN VALUE
  Uses return values from send_tcp().  On error, the function returns -1.
  Otherwise, it returns the number of bytes appended to TCP send queue.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dsstcp_write
(
  protocol_ctl_block_type pcb,                   /* protocol control block */
  dsm_item_type *dsm_item          /* ptr to head of dsm memory pool items */
);

/*===========================================================================

FUNCTION DSSTCP_READ

DESCRIPTION
  Reads specified number of bytes into buffer from the TCP transport.
  
  This function does no sanity checking. The sanity checking should be done
  in dss_read().
  
  This function is called from the context of the socket application task.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes read, which could be less than the number of 
      bytes specified.
      
  If there are no bytes to be read in the rcvq, the return value is 
  DSS_ERROR and DS_EWOULDBLOCK is returned in errno.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dsstcp_read
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  byte   *buffer,                     /* user buffer to which to copy data */
  uint16 nbytes,                 /* number of bytes to be read from socket */
  struct sockaddr_in *fromaddr,           /* destination address - ignored */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSTCP_READ_DSM_CHAIN

DESCRIPTION
  Reads a DSM item chain from the TCP transport.
  
  This function does no sanity checking. The sanity checking should be done
  in dss_read_dsm_chain().
  
  This function is called from the context of the socket application task.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes read from the DSM item chain.

  If there are no items to be read in the rcvq, the return value is 
  DSS_ERROR and DS_EWOULDBLOCK is returned in errno.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dsstcp_read_dsm_chain
(
  struct scb_s* scb_ptr,         /* Ptr to socket control block for the socket */
  dsm_item_type **item_ptr,         /* Ptr to DSM item chain to be read in */
  struct sockaddr_in *fromaddr,           /* destination address - ignored */
  sint15 *errno                                   /* error condition value */
);
/*===========================================================================

FUNCTION DSSTCP_CLOSE

DESCRIPTION
  Initiates the active close for TCP.  If the connection is already closed,
  cleans up the TCP socket and associated resources.
                         
DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS.

SIDE EFFECTS
  Initiates active close for TCP connections.

===========================================================================*/
extern sint15 dsstcp_close
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSTCP_ABORT

DESCRIPTION
  Aborts the TCP connection and cleans-up. This is usually called when we are
  dormant.

DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS.

SIDE EFFECTS
  SCB is freed up and the pointer is no longer valid.

===========================================================================*/
extern sint15 dsstcp_abort
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  sint15 *errno              /* Error condition value                      */
);


/*===========================================================================

FUNCTION DSSTCP_GET_CONN_ERROR

DESCRIPTION
  Gets the appropriate error code from the closed_state_reason.  Depending
  on the type of call (connect(), or read()/write()), different error
  values can be returned.

DEPENDENCIES
  None.

RETURN VALUE
  Error condition for connection state.

SIDE EFFECTS
  None.
===========================================================================*/
extern sint15 dsstcp_get_conn_error
(
  struct scb_s *scb_ptr,                    /* ptr to socket control block */
  dssocki_caller_enum_type type             /* type of the caller function */
);




/*===========================================================================

FUNCTION DSSTCP_LOOKUP_CONN

DESCRIPTION
  Given a connection structure, looks up in the tcb to see if this is a 
  valid connection.
                         
DEPENDENCIES
  None.

RETURN VALUE
  Returns pointer to TCB if valid connection.  If not, returns NULL.

SIDE EFFECTS
  None.
===========================================================================*/
extern struct tcb *dsstcp_lookup_conn
(
  struct connection *conn                    /* connection to be looked up */
);




/*===========================================================================

FUNCTION DSSTCP_NEW_IP_UPCALL

DESCRIPTION
  This function will be invoked by PPPIPCP when a new IP address is 
  negotiated with the Base Station.  If that is the case, then 
  send a reset to the peer, and then close down sockets and notify 
  the user.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void dsstcp_new_ip_upcall
(                                                                            
  uint32       old_ip_addr,                              /* Old IP Address */
  uint32       new_ip_addr                               /* New IP Address */
);


/*===========================================================================
FUNCTION DSSTCP_BIND

DESCRIPTION
  This is the TCP specific bind() function. It writes the port value into
  local.conn.port of the socket control block structure.
                         
DEPENDENCIES
  None.

RETURN VALUE
  None. 

SIDE EFFECTS
  None.
===========================================================================*/
extern void dsstcp_bind
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  struct sockaddr_in *la     /* Ptr to the structure contains the local 
                                address.                                   */
);

                        
/*===========================================================================

FUNCTION DSSTCP_INIT_TCB_ARRAY

DESCRIPTION
  This function initializes the array of tcbs to zero.
  
  This function should be called at startup.
  
  This function is called from the context of the PS task.
  

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
extern void dsstcp_init_tcb_array
(
  void
);

#ifdef  FEATURE_SKT_DS  //  khekim 01/02/28
extern void  dsstcp_init_tcb_pointer
(
  void
);
#endif  /* FEATURE_SKT_DS */
                        
#endif /* FEATURE_DS_SOCKETS */

#endif /* DS_DSSTCP_H */
