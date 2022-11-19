#ifndef UDP_H
#define UDP_H
/*===========================================================================

     D A T A  S E R V I C E S  U D P  H E A D E R  F I L E
                   
DESCRIPTION
 The Data Services UDP protocol interface header file. This contains all the
 functions, declarations, etc.,  used by the data services to use the UDP 
 protocol.
 
EXTERNALIZED FUNCTIONS

  udp_output()
    Sends UDP packet out to IP, performing pseudo-header checksum 
    calculation.

  udp_input()
    Called from IP layer, and hands off UDP packet to UDP protocol layer.

Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000, 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/udp.h_v   1.2   14 Feb 2001 15:26:44   jeffsong  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/01   js      Added a pointer to udp_cb to the parameters list of 
                   udp_output() to support DSS_IP_TTL. 
02/14/01   js      Removed udp_cb type from the FEATURE_DS_SOCKETS to a 
                   non-feature definition. 
11/11/00   snn     Renamed scb_type as scb_s
04/05/99   hcg     Incorporated code review changes and comments.  Added
                   definition of maximum UDP payload length.
12/09/98   na/hcg  Created module.

===========================================================================*/

/*===========================================================================

                       INCLUDE FILES FOR THE MODULE
                       
===========================================================================*/
#include "netuser.h"
#ifdef FEATURE_DS_SOCKETS
#include "customer.h"
#include "dssocki.h"
#include "dsm.h"
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Definition of the maximum UDP payload length.  This is set to be 1472
  bytes, which accounts for the 20 byte IP header and 8 byte UDP header
  which are added to the payload to create a 1500 byte packet, which is the
  maximum Ethernet payload size.  The IP header does not allow 
  fragmentation (the DF bit is set), thus any payload greater than 1500 
  bytes will be discarded by Ethernet.  
---------------------------------------------------------------------------*/
#define UDP_MAX_PAYLOAD_LEN 1472

/*---------------------------------------------------------------------------
  Structure definition for the UDP header.
---------------------------------------------------------------------------*/
typedef struct
{
  uint16 source;                                            /* Source Port */
  uint16 dest;                                         /* Destination Port */
  uint16 length;                                      /* UDP packet length */
  uint16 chksum;                                    /* UDP packet checksum */
} udp_hdr_type;                                         /* UDP header type */

/*---------------------------------------------------------------------------
  Structure definition for the UDP Control block.
---------------------------------------------------------------------------*/
#ifdef FEATURE_DS_SOCKETS
struct udp_cb
{
  struct connection conn;       /* Source/Dest port IP address combination */
  boolean connected;            /* Is this a connected UDP socket          */
  struct scb_s *scb_ptr;        /* Ptr to socket ctl block                 */
  q_type rcvq;                  /* UDP receive queue for the socket        */
};
#endif /* FEATURE_DS_SOCKETS */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS
===========================================================================*/
/*===========================================================================

FUNCTION UDP_OUTPUT

DESCRIPTION
  This function does the following:
  1. Converts from host to network all the elements of the UDP header
     and inserts the UDP header in the UDP packet.  Host to network 
     conversion is implicit to the put16() function.
  2. Creates the pseudo-header and calculates UDP checksum.
  3. Sends UDP packet down to IP.

DEPENDENCIES
  The source IP address should be correct, i.e, IPCP should have negotiated
  the right IP address when this function is called.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void udp_output
(
  ip_addr_type       *dest_ip_ptr,                  /* Destination IP addr */
  udp_hdr_type       *udp_hdr_ptr,                           /* UDP header */
  dsm_item_type      *bp,                                   /* UDP payload */
  struct udp_cb      *udp_cb_ptr                      /* UDP control block */
);

/*===========================================================================

FUNCTION UDP_INPUT

DESCRIPTION
  This function is called from the IP layer and hands off the UDP packet to
  the UDP protocol layer.
  
  This function runs in the context of the PS task.
  
  It strips off the UDP header, checks the checksum and then passes the UDP
  Payload to the application layer.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void udp_input
(
  struct iface      *iface,        /* Incoming interface (ignored)         */
  struct ip         *ip,           /* IP header                            */
  struct dsm_item_s *data_ptr,     /* Data field, if any                   */
  int               rxbroadcast    /* Incoming broadcast - discard if true */
);
#endif /* UDP_H */
