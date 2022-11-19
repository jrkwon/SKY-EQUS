#ifndef DSSUDP_H
#define DSSUDP_H
/*===========================================================================

        D A T A  S E R V I C E S  S O C K E T  U D P  F I L E
                   
DESCRIPTION

 The Data Services sockets UDP interface file. This contains all the 
 functions used by the sockets library to access UDP socket function calls.

EXTERNALIZED FUNCTIONS

  dssudp_socket()
    UDP specific socket() function.  Sets up the UDP socket and creates
    the UDP control block.

  dssudp_write()
    UDP specific write() function.  Sends specified number of bytes out
    the UDP datagram transport.

  dssudp_read()
    UDP specific read() function.  Reads specified number of bytes from
    the UDP datagram transport.

  dssudp_close()
    UDP specific close() function.  Simply cleans up socket control
    block and makes the socket available for re-use.
    
  dssudp_bind()
    This is the UDP specific bind() function. It writes the port value into
    local.conn.port of the socket control block structure.
    
Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved. 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/dssudp.h_v   1.1   15 Nov 2000 21:24:42   snookala  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00    snn    Renamed scb_type as scb_s
08/03/00   mvl/js  Added prototype for dssudp_bind() function.
12/09/98   na/hcg  Created module.

===========================================================================*/
#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

                       INCLUDE FILES FOR THE MODULE
                       
===========================================================================*/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/

/*===========================================================================

FUNCTION DSSUDP_SOCKET

DESCRIPTION
  This function sets up the UDP socket and creates the UDP control block.
  
  This function is called from the context of the application task.

DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS
  
  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EMFILE           too many descriptors open.  A socket is already open or 
                      has not closed compeletely.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dssudp_socket
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSUDP_WRITE

DESCRIPTION
  Sends dsm memory item out to UDP using udp_output found in udp.c.
  
  This function is called from the context of the PS task.

DEPENDENCIES
  None.

RETURN VALUE
  The number of bytes written.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dssudp_write
(
  protocol_ctl_block_type pcb,                   /* protocol control block */
  dsm_item_type *item_ptr          /* ptr to head of dsm memory pool items */
);

/*===========================================================================

FUNCTION DSSUDP_READ

DESCRIPTION
  Reads 'nbytes' bytes in the buffer from the UDP transport.  Fills in
  address structure with values from who sent the data in fromaddr.
  This function assumes that fromaddr is not NULL. 

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes to be written, which could be less than the number
      of bytes specified.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dssudp_read
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  byte   *buffer,               /* user buffer from which to copy the data */
  uint16 nbytes,                          /* number of bytes to be written */
  struct sockaddr_in *fromaddr,                     /* destination address */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSSUDP_CLOSE

DESCRIPTION
  This function is the UDP specific close() function.  It simply sets the
  socket state to DSSOCKI_NULL and frees up the socket control block and
  related data structures.
                         
DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS.

SIDE EFFECTS
  Socket state is set to NULL.  Socket is freed and made available for
  re-use.

===========================================================================*/
extern sint15 dssudp_close
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  sint15 *errno                                   /* error condition value */
);


/*===========================================================================
FUNCTION DSSUDP_BIND

DESCRIPTION
  This is the UDP specific bind() function. It writes the port value into
  local.conn.port of the socket control block structure.
                         
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void dssudp_bind
(
  struct scb_s* scb_ptr,     /* Ptr to socket control block for the socket */
  struct sockaddr_in *la     /* Ptr to the structure contains the local 
                                address.                                   */
);

#endif /* FEATURE_DS_SOCKETS */

#endif /* DSSUDP_H */
