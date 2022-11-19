#ifndef _DSSOCKET_H
#define _DSSOCKET_H

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

   D A T A   S E R V I C E S   S O C K E T   A P I   H E A D E R   F I L E
                   
DESCRIPTION

 The Data Services Socket Header File. Contains shared variables and enums, 
 as well as declarations for functions.

Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/dssocket.h_v   1.3   14 Feb 2001 15:10:54   jeffsong  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/01   js      Added socket options support. 
02/14/01   js      Added DS_EMSGSIZE. 
10/30/00   snn     Renamed MAX_SOCKS as DSS_MAX_SOCKS and MAX_APPS as 
                   DSS_MAX_APPS.
10/10/00   snn     Added MAX_SOCKS, DSS_MAX_TCP_SOCKS and DSS_MAX_UDP_SOCKS 
                   defines.
09/08/00   ttl     Added DNS lookup messages.
09/05/00   snn     Added DNS support
04/21/99   hcg     Removed unused error definitions.
04/16/99   hcg     Updated incorrect comment for dss_socket().
04/01/99   hcg     Replaced functions with macros to perform byte-ordering 
                   for host/network conversions.  Removed the external 
                   function declarations for the removed functions.  
                   Incorporated code review changes/comments.
10/05/98   na/hcg  Created module.

===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "psglobal.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                        Address/Protocol Family
---------------------------------------------------------------------------*/
#define AF_INET         0                     /* Address Family - Internet */
#define PF_INET         0                    /* Protocol Family - Internet */

#ifdef INADDR_ANY                  /* safeguard against previously defined */
#undef INADDR_ANY
#endif

#define INADDR_ANY      0x0L                  /* Local IP wildcard address */
/*---------------------------------------------------------------------------
               Return values indicating error status
---------------------------------------------------------------------------*/
#define DSS_SUCCESS         0                      /* successful operation */
#define DSS_ERROR          -1                    /* unsuccessful operation */

/*---------------------------------------------------------------------------
      IP protocol numbers - use in dss_socket() to identify protocols.
---------------------------------------------------------------------------*/
#define IPPROTO_TCP    6                                   /* TCP Protocol */
#define IPPROTO_UDP   17                                   /* UDP Protocol */

/*---------------------------------------------------------------------------
                             Socket types
---------------------------------------------------------------------------*/
#define SOCK_STREAM     0                        /* TCP - streaming socket */
#define SOCK_DGRAM      1                         /* UDP - datagram socket */

/*---------------------------------------------------------------------------
                      Asynchronous Socket Events
---------------------------------------------------------------------------*/
#define DS_WRITE_EVENT  0x01         /* associated with a writeable socket */
#define DS_READ_EVENT   0x02          /* associated with a readable socket */
#define DS_CLOSE_EVENT  0x04         /* associated with a closeable socket */

/*---------------------------------------------------------------------------
                    Transport Error Condition Values
---------------------------------------------------------------------------*/
#define DS_EEOF               0                             /* end of file */
#define DS_EBADF            100               /* Invalid socket descriptor */
#define DS_EFAULT           101              /* Invalid buffer or argument */
#define DS_EWOULDBLOCK      102                   /* Operation would block */
#define DS_EAFNOSUPPORT     103            /* Address family not supported */
#define DS_EPROTOTYPE       104          /* Wrong protocol for socket type */
#define DS_ESOCKNOSUPPORT   105          /* Socket parameter not supported */
#define DS_EPROTONOSUPPORT  106                  /* Protocol not supported */
#define DS_EMFILE           107   /* No more sockets available for opening */
#define DS_EOPNOTSUPP       108                 /* Operation not supported */
#define DS_EADDRINUSE       109                  /* Address already in use */
#define DS_EADDRREQ         110            /* Destination address required */
#define DS_EINPROGRESS      111    /* Connection establishment in progress */
#define DS_EISCONN          112          /* Connection already established */
#define DS_EIPADDRCHANGED   113   /* IP address changed, causing TCP reset */
#define DS_ENOTCONN         114                    /* socket not connected */
#define DS_ECONNREFUSED     115              /* Connection attempt refused */
#define DS_ETIMEDOUT        116            /* Connection attempt timed out */
#define DS_ECONNRESET       117                        /* Connection reset */
#define DS_ECONNABORTED     118                      /* Connection aborted */
#define DS_EPIPE            119                             /* Broken pipe */
#define DS_ENETDOWN         120           /* Network subsystem unavailable */
#define DS_EMAPP            121          /* no more applications available */
#define DS_EBADAPP          122                  /* Invalid application ID */
#define DS_SOCKEXIST        123              /* there are existing sockets */
#define DS_EINVAL           124                       /* invalid operation */

/*---------------------------------------------------------------------------
                    DNS Lookup Error Messages
---------------------------------------------------------------------------*/
#define DS_NAMEERR        125                       /* Domain Name Error */
#define DS_NAMEMISS       126                   /* Domain Name not found */
#define DS_NETERR         127                   /* Network is not opened */
#define DS_NOMEMORY       128                           /* Out of memory */
#define DS_INUSE          129            /* The resolver is in used now. */

/*---------------------------------------------------------------------------
                  dss_sendto() Error Conditions
---------------------------------------------------------------------------*/
#define DS_EMSGSIZE       130           /* The message is too large.       */

/*---------------------------------------------------------------------------
                       Socket Option Errors
---------------------------------------------------------------------------*/
#define DS_ENOPROTOOPT 131 /* The option is unknown at the level indicated */

/*---------------------------------------------------------------------------
                       Network Subsystem Errors
---------------------------------------------------------------------------*/
#define DS_ENETISCONN           200 /* subsystem established and available */
#define DS_ENETINPROGRESS       201 /* subsystem establishment in progress */
#define DS_ENETNONET            202       /* network subsystem unavailable */
#define DS_ENETCLOSEINPROGRESS  203                      /* PPP is closing */
#define DS_ENETEXIST            204    /* existing net subsystem resources */


/*---------------------------------------------------------------------------
                     Specify the number of applications
---------------------------------------------------------------------------*/
#define DSS_MAX_APPS         3           /* maximum number of applications */

/*---------------------------------------------------------------------------
  Constants to be defined for Multiple Sockets
---------------------------------------------------------------------------*/
#define DSS_MAX_TCP_SOCKS    3           /* Maximum TCP sockets allowed    */
#define DSS_MAX_UDP_SOCKS    4           /* Maximum UDP sockets allowed    */

#define DSS_MAX_SOCKS      (DSS_MAX_TCP_SOCKS + DSS_MAX_UDP_SOCKS)  

/*---------------------------------------------------------------------------
                       Socket Address Structures
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  The socket address structures follow the BSD convention, including data
  types, etc.  These are the BSD generic address structures needed to 
  support Internet-family addressing.  
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                   BSD Generic Socket Address structure
---------------------------------------------------------------------------*/
struct sockaddr                        /* generic socket address structure */
{
  uint16 sa_family;                               /* socket address family */
  unsigned char   sa_data[14];                             /* address data */
};

/*---------------------------------------------------------------------------
             Internet-family specific host internet address
---------------------------------------------------------------------------*/
struct in_addr                       /* structure for "historical" reasons */
{
  uint32 s_addr;                                         /* socket address */
};

/*---------------------------------------------------------------------------
             Internet-family Socket Address Structures
---------------------------------------------------------------------------*/
struct sockaddr_in                        /* Internet style socket address */
{
  uint16 sin_family;                             /* internet socket family */
  uint16 sin_port;                                 /* internet socket port */
  struct in_addr sin_addr;                      /* internet socket address */
  char sin_zero[8];              /* zero'ed out data for this address type */
};

/*---------------------------------------------------------------------------
                       Non-Contiguous Buffer Structure
---------------------------------------------------------------------------*/
struct iovec
{
  byte *iov_base;                            /* starting address of buffer */
  uint16 iov_len;                           /* size of the buffer in bytes */
};


/*---------------------------------------------------------------------------
                       Socket Options Data Types
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Definition for various options that affect the behaviors of a socket. 
  Only the options whose names are defined here are supported. The 
  following definition is also used as index to the table of all supported 
  options. 
---------------------------------------------------------------------------*/
typedef enum
{
  DSS_SOCKOPT_MIN = -1,                                     /* lower bound */
  DSS_IP_TTL,                                              /* time-to-live */
  DSS_SOCKOPT_MAX                  /* determine upper bound and array size */
}dss_sockopt_names_type;


/*---------------------------------------------------------------------------
  Socket option level specifies the code in the system to interpret the 
  option: the heneral socket code. or some protocol-specific code.  If
  an option is unknown at a given level, an error will be generated. 
---------------------------------------------------------------------------*/
typedef enum 
{
  DSS_IPPROTO_IP                    /* IP protocol level                   */
}dss_sockopt_levels_type;




/*===========================================================================

                      PUBLIC MACRO DECLARATIONS

===========================================================================*/

/*===========================================================================

MACRO DSS_HTONL

DESCRIPTION
  Converts host-to-network long integer.  Handles potential byte order
  differences between different computer architectures and different network
  protocols.

PARAMETERS
  x     unsigned long integer value to be converted.

DEPENDENCIES
  None.

RETURN VALUE
  The network byte-ordered value.

SIDE EFFECTS
  None.

===========================================================================*/
#define dss_htonl(x) \
        ((uint32)((((uint32)(x) & 0x000000ffU) << 24) | \
        (((uint32)(x) & 0x0000ff00U) <<  8) | \
        (((uint32)(x) & 0x00ff0000U) >>  8) | \
        (((uint32)(x) & 0xff000000U) >> 24)))

/*===========================================================================

MACRO DSS_HTONS

DESCRIPTION
 Converts host-to-network short integer.  Handles potential byte order
 differences between different computer architectures and different network
 protocols.

PARAMETERS
  x     unsigned short integer value to be converted.

DEPENDENCIES
  None.

RETURN VALUE
  The network byte-ordered value.

SIDE EFFECTS
  None.

===========================================================================*/
#define dss_htons(x) \
        ((uint16)((((uint16)(x) & 0x00ff) << 8) | \
        (((uint16)(x) & 0xff00) >> 8)))

/*===========================================================================

MACRO DSS_NTOHL

DESCRIPTION
  Converts network-to-host long integer.  Handles potential byte order
  differences between different computer architectures and different network
  protocols.

PARAMETERS
  x     unsigned long integer value to be converted.

DEPENDENCIES
  None.

RETURN VALUE
  The host byte-ordered value.

SIDE EFFECTS
  None.

===========================================================================*/
#define dss_ntohl(x) \
        ((uint32)((((uint32)(x) & 0x000000ffU) << 24) | \
        (((uint32)(x) & 0x0000ff00U) <<  8) | \
        (((uint32)(x) & 0x00ff0000U) >>  8) | \
        (((uint32)(x) & 0xff000000U) >> 24)))

/*===========================================================================

MACRO DSS_NTOHS

DESCRIPTION
 Converts network-to-host short integer.  Handles potential byte order
  differences between different computer architectures and different network
  protocols.

PARAMETERS
  x     unsigned short integer value to be converted.

DEPENDENCIES
  None.

RETURN VALUE
  The host byte-ordered value.

SIDE EFFECTS
  None.

===========================================================================*/
#define dss_ntohs(x) \
        ((uint16)((((uint16)(x) & 0x00ff) << 8) | \
        (((uint16)(x) & 0xff00) >> 8)))

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION DSS_SOCKET

DESCRIPTION
 Create a socket and related data structures, and return a socket descriptor.

 The mapping to actual protocols is as follows:
                
 ADDRESS FAMILY         Stream          Datagram

 AF_INET                TCP             UDP

  Note this function must be called to obtain a valid socket descriptor, for
  use with all other socket-related functions.  Thus, before any socket
  functions can be used (e.g. I/O, asynchronous notification, etc.), this
  call must have successfully returned a valid socket descriptor.  The
  application must also have made a call to dss_open_netlib() to obtain
  a valid application ID, and to put the Data Services task into "sockets"
  mode.

  Note:  This implementation version has no support for Raw IP sockets, and
         will return an error, if the application attempts to create one.

DEPENDENCIES
  The function dss_open_netlib() must be called to open the network library
  and put the DS/PS managers into sockets mode.

RETURN VALUE
  On successful creation of a socket, this function returns socket file 
  descriptor which is a sint15 value between 0x1000 and 0x1FFF.
  
  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EAFNOSUPPORT     address family not supported
  DS_EBADAPP          invalid application ID
  DS_EPROTOTYPE       specified protocol invalid for socket type
  DS_ESOCKNOSUPPORT   invalid or unsupported socket parameter specified
  DS_EPROTONOSUPPORT  specified protocol not supported
  DS_EMFILE           too many descriptors open.  A socket is already open or 
                      has not closed compeletely.

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_socket
(
  sint15 app_id,                                         /* application ID */
  byte   family,                          /* Address family - AF_INET only */
  byte   type,                                              /* socket type */
  byte   protocol,                                        /* protocol type */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_CONNECT

DESCRIPTION
  For TCP, attempts to establish the TCP connection.  Upon
  successful connection, calls the socket callback function asserting that
  the DS_WRITE_EVENT is TRUE.  The implementation does not support connected
  UDP sockets and will return an error.  The function must receive
  (as a parameter) a valid socket descriptor, implying a previous successful
  call to dss_socket().

DEPENDENCIES
  Network subsystem must be established and available.

RETURN VALUE
  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EWOULDBLOCK      operation would block
  DS_EBADF            invalid socket descriptor is specfied
  DS_ECONNREFUSED     connection attempt refused
  DS_ETIMEDOUT        connection attempt timed out
  DS_EFAULT           addrlen parameter is invalid
  DS_EIPADDRCHANGED   IP address changed due to PPP resync
  DS_EINPROGRESS      connection establishment in progress
  DS_EISCONN          a socket descriptor is specified that is already 
                      connected
  DS_ENETDOWN         network subsystem unavailable
  DS_EOPNOTSUPP       invalid server address specified
  DS_EADDRREQ         destination address is required.

SIDE EFFECTS
  For TCP, initiates active open for connection.

===========================================================================*/
extern sint15 dss_connect
(
  sint15 sockfd,                                      /* Socket descriptor */
  struct sockaddr *servaddr,                        /* destination address */
  uint16 addrlen,                                    /* length of servaddr */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_OPEN_NETLIB

DESCRIPTION

  Opens up the network library.  Assigns application ID and sets the
  application-defined callback functions to be called when library and 
  socket calls would make progress.  Puts data services manager into 
  "socket" mode.

  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  Returns application ID on success.

  On error, return DSS_SUCCESS and places the error condition value in 
  *errno.

  Errno Values
  ------------
  DS_EMAPP      no more applications available - max apps exceeded.
            
SIDE EFFECTS
  Puts data services manager into "socket" mode.

===========================================================================*/
extern sint15 dss_open_netlib
(
  void   (*net_callback_fcn)(void *),         /* network callback function */
  void   (*socket_callback_fcn)(void *),       /* socket callback function */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_CLOSE_NETLIB

DESCRIPTION

  Closes the network library for the application.  All sockets must have
  been closed for the application, prior to closing.  If this is the last
  remaining application, the network subsytem (PPP/traffic channel) must 
  have been brought down, prior to closing the network library.  If this 
  is the last active application using the network library, this function 
  takes the data services manager out of "socket" mode.

  This function is called from the context of the socket client's task.

DEPENDENCIES
  None.

RETURN VALUE
  On success, returns DSS_SUCCESS.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADAPP        invalid application ID
  DS_ESOCKEXIST     there are existing sockets
  DS_ENETEXIST      the network subsystem exists

SIDE EFFECTS
  Puts data services manager into "autodetect" mode.

===========================================================================*/
extern sint15 dss_close_netlib
(
  sint15 app_id,                                         /* application ID */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_PPPOPEN

DESCRIPTION
  Starts up the network subsystem (CDMA data service and PPP) over the Um 
  interface for all sockets.  
  
DEPENDENCIES
  dss_pppopen() cannot be called by the application if the network is in the 
  process of closing. The network layer cannot queue the open request until 
  the close is completely finished.  Therefore, the application should wait 
  for the net_callback_fn() to be called (after dss_pppclose() has 
  completed), before making a call to dss_pppopen().  Note that a valid
  application ID must be specified as a parameter, obtained by a successful
  return of dss_open_netlib().

RETURN VALUE
  If the network subsytem is already established, return DSS_SUCCESS.

  Return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADAPP               invalid application ID specified
  DS_EWOULDBLOCK           the operation would block
  DS_ENETCLOSEINPROGRESS   network close in progress. The application
                           should only call dss_pppopen() after the 
                           close/abort has completed.

SIDE EFFECTS
  Initiates call origination and PPP negotiation.

===========================================================================*/
extern sint15 dss_pppopen
(
  sint15 app_id,                                         /* application id */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_BIND

DESCRIPTION
  For all client sockets, attaches a local address and port value to the 
  socket.  If the call is not explicitly issued, the socket will implicitly
  bind in calls to dss_connect() or dss_sendto().  Note that this function
  does not support binding a local IP address, but rather ONLY a local port
  number.  The local IP address is assigned automatically by the sockets 
  library.  The function must receive (as a parameter) a valid socket 
  descriptor, implying a previous successful call to dss_socket().

DEPENDENCIES
  None.

RETURN VALUE
  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_EOPNOTSUPP       operation not supported
  DS_EADDRINUSE       the local address is already in use.
  DS_EINVAL           the socket is already attached to a local name
  DS_EFAULT           invalid address parameter has been specified

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_bind
(
  sint15 sockfd,                                      /* socket descriptor */
  struct sockaddr *localaddr,                             /* local address */
  uint16 addrlen,                                     /* length of address */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_CLOSE

DESCRIPTION
  Non-blocking close of a socket.  Performs all necessary clean-up of data 
  structures and frees the socket for re-use.  For TCP initiates the active 
  close for connection termination.  Once TCP has closed, the DS_CLOSE_EVENT 
  will become TRUE, and the application can call dss_close() again to free 
  the socket for re-use.  UDP sockets also need to call this to 
  clean-up the socket and free it for re-use.
                         
DEPENDENCIES
  None.

RETURN VALUE
  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EWOULDBLOCK      operation would block - TCP close in progress
  DS_EBADF            invalid socket descriptor is specfied

SIDE EFFECTS
  Initiates active close for TCP connections.

===========================================================================*/
extern sint15 dss_close
(
  sint15 sockfd,                                      /* socket descriptor */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_PPPCLOSE

DESCRIPTION
  Initiates termination to bring down PPP and the traffic channel.  Upon
  successful close of the network subsystem, invokes the network callback
  function.
                         
DEPENDENCIES
  None.

RETURN VALUE
  If the network subsytem is already closed, return DSS_SUCCESS.

  Returns DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADAPP               invalid application ID specified
  DS_EWOULDBLOCK           operation would block
  DS_ENETCLOSEINPROGRESS   network close in progress. A call to 
                           dss_pppclose() has already been issued.

SIDE EFFECTS
  Initiates termination of PPP.  Brings down PPP and traffic channel.

===========================================================================*/
extern sint15 dss_pppclose
(
  sint15 app_id,                                         /* application id */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_NETSTATUS

DESCRIPTION
  Provides status of network subsystem.  Called in response to DS_ENETDOWN
  errors.  Note that origination status is based on the last attempted 
  origination.
                         
DEPENDENCIES
  None.

RETURN VALUE

  Returns DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADAPP               invalid application ID specified
  DS_ENETNONET             network subsystem unavailable for some unknown 
                           reason
  DS_ENETISCONN            network subsystem is connected and available
  DS_ENETINPROGRESS        network subsystem establishment currently in 
                           progress
  DS_ENETCLOSEINPROGRESS   network subsystem close in progress. 

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_netstatus
(
  sint15 app_id,                                         /* application ID */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_ASYNC_SELECT

DESCRIPTION
  Enables the events to be notified about through the asynchronous 
  notification mechanism.  Application specifies a bitmask of events that it
  is interested in, for which it will receive asynchronous notification via
  its application callback function.  This function also performs a real-time
  check to determine if any of the events have already occurred, and if so
  invokes the application callback.

DEPENDENCIES
  None.

RETURN VALUE

  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied

SIDE EFFECTS
  Sets the relevant event mask in the socket control block.  Will also 
  notify the application via the callback function.

===========================================================================*/
extern sint31 dss_async_select
(
  sint15 sockfd,                                      /* socket descriptor */
  sint31 interest_mask,                        /* bitmask of events to set */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_ASYNC_DESELECT

DESCRIPTION
  Clears events of interest in the socket control block interest mask.  The
  application specifies a bitmask of events that it wishes to clear; events 
  for which it will no longer receive notification.

DEPENDENCIES
  None.

RETURN VALUE
  Returns DSS_SUCCESS on success.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  

SIDE EFFECTS
  Clears specified events from the relevant event mask.

===========================================================================*/
extern sint15 dss_async_deselect
(
  sint15 sockfd,                                      /* socket descriptor */
  sint31 clr_interest_mask,                  /* bitmask of events to clear */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_GETNEXTEVENT

DESCRIPTION
  This function performs a real-time check to determine if any of the events 
  of interest specified in the socket control block's event mask have 
  occurred.  It also clears any bits in the event mask that have occurred.
  The application must re-enable these events through a subsequent call to 
  dss_async_select().  The application may pass in a pointer to a single 
  socket descriptor to determine if any events have occurred for that socket.

  Alternatively, the application may set this pointer's value to NULL (0) 
  (note, not to be confused with a NULL pointer, but rather a pointer whose 
  value is 0) in which case the function will return values for the next 
  available socket.  The next available socket's descriptor will be placed 
  in the socket descriptor pointer, and the function will return.  If no 
  sockets are available (no events have occurred across all sockets for
  that application) the pointer value will remain NULL (originally value 
  passed in), and the function will return 0, indicating that no events
  have occurred.

DEPENDENCIES
  None.

RETURN VALUE
  Returns an event mask of the events that were asserted.  A value of 0
  indicates that no events have occurred.

  On passing a pointer whose value is NULL into the function for 
  the socket descriptor (not to be confused with a NULL pointer), places 
  the next available socket descriptor in *sockfd_ptr and returns the
  event mask for that socket. If no sockets are available (no events have 
  occurred across all sockets for that application) the pointer value 
  will remain NULL (originally value passed in), and the function will 
  return 0, indicating that no events have occurred.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADAPP           invalid app descriptor is specfied
  DS_EBADF             invalid socket descriptor is specfied

SIDE EFFECTS
  Clears the bits in the socket control block event mask, corresponding to 
  the events that have occurred.
  
===========================================================================*/
extern sint31 dss_getnextevent
( 
  sint15 app_id,                                         /* application ID */
  sint15 *sockfd_ptr,                                 /* socket descriptor */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_WRITE

DESCRIPTION
  Sends specified number of bytes in the buffer over the TCP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes written, which could be less than the number of 
      bytes specified.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required - connectionless socket
                      did not call dss_connect()
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_write
(
  sint15 sockfd,                                      /* socket descriptor */
  byte   *buffer,                   /* user buffer from which to copy data */
  uint16 nbytes,                /* number of bytes to be written to socket */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_WRITEV

DESCRIPTION
  Provides the gather write variant of the dss_write() call, which 
  allows the application to write from non-contiguous buffers.    Sends 
  specified number of bytes in the buffer over the TCP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes written, which could be less than the number of 
      bytes specified.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required - connectionless socket
                      did not call dss_connect()
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_writev
(
  sint15 sockfd,                                      /* socket descriptor */
  struct iovec iov[],     /* array of data buffers from which to copy data */
  sint15 iovcount,                                /* number of array items */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_READ

DESCRIPTION
  Reads specified number of bytes into buffer from the TCP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes read, which could be less than the number of 
      bytes specified.  A return of 0 indicates that an End-of-File condition 
      has occurred.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required - connectionless socket
                      did not call dss_connect()
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_read
(
  sint15 sockfd,                                      /* socket descriptor */
  byte   *buffer,                     /* user buffer to which to copy data */
  uint16 nbytes,                 /* number of bytes to be read from socket */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_READV

DESCRIPTION
  Provides the scatter read variant of the dss_read() call, which 
  allows the application to read into non-contiguous buffers.    Reads
  specified number of bytes into the buffer from the TCP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes read, which could be less than the number of 
      bytes specified.  A return of 0 indicates that an End-of-File condition 
      has occurred.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required - connectionless socket
                      did not call dss_connect()
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_readv
(
  sint15 sockfd,                                      /* socket descriptor */
  struct iovec iov[],           /* array of data buffers to copy data into */
  sint15 iovcount,                                /* number of array items */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_SENDTO

DESCRIPTION
  Sends 'nbytes' bytes in the buffer over the UDP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes to be written, which could be less than the number
      of bytes specified.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_EAFNOSUPPORT     address family not supported
  DS_EWOULDBLOCK      operation would block
  DS_EADDRREQ         destination address required 
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EOPNOSUPPORT     option not supported

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_sendto
(
  sint15 sockfd,                                      /* socket descriptor */
  byte   *buffer,               /* user buffer from which to copy the data */
  uint16 nbytes,                          /* number of bytes to be written */
  uint16 flags,                                                  /* unused */
  struct sockaddr *toaddr,                          /* destination address */
  uint16 addrlen,                                        /* address length */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_RECVFROM

DESCRIPTION
  Reads 'nbytes' bytes in the buffer from the UDP transport.  Fills in
  address structure with values from who sent the data.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes to be written, which could be less than the number
      of bytes specified.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_EAFNOSUPPORT     address family not supported
  DS_EWOULDBLOCK      operation would block
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EOPNOSUPPORT     option not supported

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_recvfrom
(
  sint15 sockfd,                                      /* socket descriptor */
  byte   *buffer,               /* user buffer from which to copy the data */
  uint16 nbytes,                          /* number of bytes to be written */
  uint16 flags,                                                  /* unused */
  struct sockaddr *fromaddr,                        /* destination address */
  uint16 *addrlen,                                       /* address length */
  sint15 *errno                                   /* error condition value */
);

/*===========================================================================

FUNCTION DSS_GETHOSTADDR

DESCRIPTION
  This function returns the IP address for a query domain name if it can be
  found in the CASH table. Otherwise it'll call resolver() to get from the
  DNS server given by the user and return DS_EWOULDBLOCK to the caller. The
  user application can call this function again once it is informed by the 
  given callback function to obtain the IP address that queried.
  If the resolver is busy in getting an answer from the DNS server, and the
  querying domain name is not found in the CASH table, it'll return DS_INUSE.
  
  
Parameters passed:
  Domain Name:        This contains the name of the string for which we need
                      to get the IP address. Example, www.qualcomm.com
  
  IP ADDRESS:         IP address for the querying domain name. If answer is
                      not available, this field will be 0.
  
  Callback function:  Pass the pointer to the callback function. This 
                      function is called by the resolver/PS after obtaining 
                      the IP address. The application can call this function
                      again after the callback function get called.


DEPENDENCIES
  Size of the hostname:         This function assumes that the maximum size 
                                of the host name is less MAX_DNS_STR_LEN 
                                bytes including the <CR>.
  
RETURN VALUE
  None

SIDE EFFECTS
  

===========================================================================*/

extern sint15 dss_gethostaddrs( 
  char    *domain_name, 
  uint32  *ip_addr,
  void (*apps_dns_cb)(void)
);



/*===========================================================================

FUNCTION DSS_SETSOCKOTP

DESCRIPTION
  Set the options associated with a socket. This fuction expects the 
  following parameters:
  
DEPENDENCIES
  None. 
  
PARAMETERS  
  int sockfd        -     Socket file descriptor.
  int level         -     Socket option level. 
  int optname,      -     Option name. 
  void *optval      -     Pointer to the option value.
  uint32 *optlen    -     Pointer to the size of the option value.  
  sint15 *errno     -     Error condition value.     


RETURN VALUE
  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF                invalid socket descriptor is specfied
  DS_ENOPROTOOPT          the option is unknown at the level indicated
  DS_EINVAL               invalid option name or invalid option value
  DS_EFAULT               Invalid buffer or argument

SIDE EFFECTS
  None.

===========================================================================*/
int dss_setsockopt
(
  int sockfd,                            /* socket descriptor              */
  int level,                             /* socket option level            */
  int optname,                           /* option name                    */
  void *optval,                          /* value of the option            */
  uint32 *optlen,                        /* size of the option value       */
  sint15 *errno                          /* error condition value          */
);


/*===========================================================================
FUNCTION DSS_GETSOCKOPT

DESCRIPTION
  Return an option associated with a socket. This fuction expects the 
  following parameters:
  
DEPENDENCIES
  None. 

PARAMETERS  
  int sockfd        -     Socket file descriptor.
  int level         -     Socket option level. 
  int optname,      -     Option name. 
  void *optval      -     Pointer to the option value.
  uint32 *optlen    -     Pointer to the size of the option value.  
  sint15 *errno     -     Error condition value.     

RETURN VALUE
  optlen is a value-result parameter, initially containing the size of 
  the buffer pointed to by optval, and modified on return to indicate the  
  actual  size  of the value returned. On error, return DSS_ERROR and places 
  the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF                invalid socket descriptor is specfied
  DS_ENOPROTOOPT          the option is unknown at the level indicated
  DS_EINVAL               invalid option name or invalid option value  
  DS_EFAULT               Invalid buffer or argument  

SIDE EFFECTS
  None.

===========================================================================*/
int dss_getsockopt
(
  int sockfd,                            /* socket descriptor              */
  int level,                             /* socket option level            */                  
  int optname,                           /* option name                    */
  void *optval,                          /* value of the option            */
  uint32 *optlen,                        /* size of the option value       */
  sint15 *errno                          /* error condition value          */
);

#endif  /* FEATURE_DS_SOCKETS */

#endif  /* _DSSOCKET_H */
