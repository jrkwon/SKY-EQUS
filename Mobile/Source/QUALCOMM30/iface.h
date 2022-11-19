#ifndef _IFACE_H
#define _IFACE_H

/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION


   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/iface.h_v   1.0   23 Oct 2000 17:32:22   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/22/99   mvl     Added support for network model related statistics.
10/27/98   ldg     Removed #include "tcp.h"
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/


#include "psglobal.h"
#include "customer.h"       /* Customer Specific Features */
#ifdef FEATURE_DS_NET_MODEL
#include "dsnetmdl.h"
#endif /* FEATURE_DS_NET_MODEL */

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

enum if_status_t {
  ifsUp = 1,
  ifsDown,
  ifsTesting,
  ifsUnknown,
  ifsDormant
};


/* Interface control structure */
struct iface {
  char *name;      /* Ascii string with interface name */
  uint32 index;    /* interface number for ifMib */

  ip4a addr;       /* IP address */
  ip4a broadcast;  /* Broadcast address */
  ip4a netmask;    /* Network mask */
  ip4a peeraddr;   /* peer address, if assigned */

  uint32 mru;                     /* Maximum receive unit size */
  uint32 mtu;                     /* Maximum transmission unit size */
#define MXU_DEFAULT     1500                /* default for both MRU and MTU */
#define MXU_MINIMUM  576          /* minimum for both MRU and MTU */
#define MTU_SLIP     1006         /* SLIP default for MTU */

  enum if_status_t AdminStatus;
  enum if_status_t OperStatus;

  uint16 flags;                  /* Configuration flags */
#define IFF_CONNECTED  0x0001    /* Send datagrams in connected mode */
#define IFF_ADDRESS  0x0100      /* address assigned by command */
#define IFF_DYNAMIC  0x0200      /* address dynamically assigned */
#define IFF_PEER  0x0400         /* peer address assigned by command */
#define IFF_POOL  0x0800         /* peer address assigned from pool */

  /* Counters */
  uint32 InIP;           /* IP datagrams sent */
  uint32 OutIP;          /* IP datagrams received */

  uint32 lastsent;       /* Clock time of last send */
  uint32 lastrecv;       /* Clock time of last receive */

  uint32 Speed;          /* current bandwidth, bits per second */

#ifdef FEATURE_DS_NET_MODEL
  uint32 UnframedPkts[IFACE_MAX_STACK];    /* # of pkts unframed per iface */
  uint32 FramedPkts[IFACE_MAX_STACK];  /* # of pkts not unframed per iface */
  uint32 TotalPkts[IFACE_MAX_STACK];          /* total # packets per iface */
#endif /* FEATURE_DS_NET_MODEL */

  uint32 InOctets;       /* # octets received */
  uint32 InUnicast;      /* # unicast packets */
  uint32 InMulticast;    /* # multicast packets */
  uint32 InBroadcast;    /* # broadcast packets */
  uint32 InDiscards;     /* # discards without error */
  uint32 InErrors;       /* # error packets */

  uint32 OutOctets;      /* # octets sent */
  uint32 OutUnicast;     /* # unicast packets */
  uint32 OutMulticast;   /* # multicast packets */
  uint32 OutBroadcast;   /* # broadcast packets */
  uint32 OutDiscards;    /* # discards without error */
  uint32 OutErrors;      /* # errors on send */
};
extern struct iface if_cb;

#endif  /* _IFACE_H */
