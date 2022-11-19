#ifndef DSSINTERNAL_H
#define DSSINTERNAL_H

/*===========================================================================

         I N T E R N A L  D A T A  S E R V I C E  S O C K E T S  
                       H E A D E R  F I L E
                   
DESCRIPTION

 The Internal Data Services Sockets Header File. Contains shared variables 
 and enums, as well as declarations for functions for each protocol.


Copyright (c) 2001 by QUALCOMM, Incorporated. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.



when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/28/01    js     Created module. 
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "dsm.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
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
  DSS_IPPROTO_IP                   /* IP protocol level                   */
}dss_sockopt_levels_type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

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
extern sint15 dss_setsockopt
(
  int sockfd,                            /* socket descriptor              */
  int level,                             /* socket option level            */
  int optname,                           /* option name                    */
  void *optval,                          /* value of the option            */
  uint32 *optlen,                        /* size of the option value       */
  sint15 *dss_errno                       /* error condition value          */
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
extern sint15 dss_getsockopt
(
  int sockfd,                            /* socket descriptor              */
  int level,                             /* socket option level            */                  
  int optname,                           /* option name                    */
  void *optval,                          /* value of the option            */
  uint32 *optlen,                        /* size of the option value       */
  sint15 *dss_errno                      /* error condition value          */
);


/*===========================================================================

FUNCTION DSS_WRITE_DSM_CHAIN

DESCRIPTION
  Sends the specified DSM item chain over the TCP transport.  Lower layers
  will be responsible for deallocating the DSM item chain.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes written.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block
  DS_EOPNOTSUPP       invalid server address specified

SIDE EFFECTS
  The pointer to the DSM item chain is set to NULL.

===========================================================================*/
extern sint15 dss_write_dsm_chain
(
  sint15 sockfd,                                      /* socket descriptor */
  dsm_item_type **item_ptr,          /* DSM item chain containing the data */
  sint15 *errno                                   /* error condition value */
);


/*===========================================================================

FUNCTION DSS_READ_DSM_CHAIN

DESCRIPTION
  Reads a DSM item chain from the TCP transport.

DEPENDENCIES
  None.

RETURN VALUE
  n - the number of bytes read.  A return of 0 indicates that an End-of-File 
      condition has occurred.

  On error, return DSS_ERROR and places the error condition value in *errno.

  Errno Values
  ------------
  DS_EBADF            invalid socket descriptor is specfied
  DS_ENOTCONN         socket not connected
  DS_ECONNRESET       TCP connection reset by server
  DS_ECONNABORTED     TCP connection aborted due to timeout or other failure
  DS_EIPADDRCHANGED   IP address changed, causing TCP connection reset
  DS_EPIPE            broken pipe
  DS_EADDRREQ         destination address required
  DS_ENETDOWN         network subsystem unavailable
  DS_EFAULT           application buffer no valid part of address space
  DS_EWOULDBLOCK      operation would block

SIDE EFFECTS
  None.

===========================================================================*/
extern sint15 dss_read_dsm_chain
(
  sint15 sockfd,                                      /* socket descriptor */
  dsm_item_type  **item_ptr,          /* ptr to item chain containing data */
  sint15 *errno                                   /* error condition value */
);

#endif
