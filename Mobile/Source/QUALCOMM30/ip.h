#ifndef IP_H
#define IP_H

/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION


   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 2000 by QUALCOMM, Incorporated.  All Rights Reserved.   
===========================================================================*/

#include "comdef.h"
#include "iface.h"
#include "psglobal.h"
#include "dsm.h"
#include "internet.h"
#include "customer.h"       /* Customer Specific Features */



/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/ip.h_v   1.0   23 Oct 2000 17:32:24   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/20/00   snn     Added support for IP fragments Reassembly.
12/14/98   ldg     Commented out extern declaration for Raw_ip.
10/27/98   ldg     Merged from ARM port: Include comdef.h, iface.h.
06/12/97   ldg     Added new FEATURE_DS_PSTATS
11/14/96   jjw     Unwired Planet Upgrade (ie added UDP)
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/


#define TLB    30     /* Default reassembly timeout, sec */
#define IPVERSION  4
#define IP_CS_OLD  1  /* use saved checksum */
#define IP_CS_NEW  0  /* calculate checksum */


#define ip_addr_type ip4a

/* SNMP MIB variables, used for statistics and control. See RFC 1066 */
extern struct mib_entry    Ip_mib[];
#define ipForwarding       Ip_mib[1].value.integer
#define ipDefaultTTL       Ip_mib[2].value.integer
#define ipInReceives       Ip_mib[3].value.integer
#define ipInHdrErrors      Ip_mib[4].value.integer
#define ipInAddrErrors     Ip_mib[5].value.integer
#define ipForwDatagrams    Ip_mib[6].value.integer
#define ipInUnknownProtos  Ip_mib[7].value.integer
#define ipInDiscards       Ip_mib[8].value.integer
#define ipInDelivers       Ip_mib[9].value.integer
#define ipOutRequests      Ip_mib[10].value.integer
#define ipOutDiscards      Ip_mib[11].value.integer
#define ipOutNoRoutes      Ip_mib[12].value.integer
#define ipReasmTimeout     Ip_mib[13].value.integer
#define ipReasmReqds       Ip_mib[14].value.integer
#define ipReasmOKs         Ip_mib[15].value.integer
#define ipReasmFails       Ip_mib[16].value.integer
#define ipFragOKs          Ip_mib[17].value.integer
#define ipFragFails        Ip_mib[18].value.integer
#define ipFragCreates      Ip_mib[19].value.integer

#define NUMIPMIB  19

#ifdef FEATURE_DS_PSTATS

/*---------------------------------------------------------------------------
  Type to index the array of IP stats.
---------------------------------------------------------------------------*/
typedef enum
{
  IP_STATS_IN_RECEIVES,       /* The total number of input datagrams received
                                 from interfaces, including those received
                                 in error. */

  IP_STATS_IN_HDR_ERRORS,     /* The numbe of input datagrams discarded due to
                                 errors in their IP headers, including bad
                                 checksums, version number mismatch, other
                                 format errors, time-to-live exceeded, errors
                                 discovered in processing thier IP options,
                                 etc. */

  IP_STATS_IN_UNKNOWN_PROTOS, /* The number of locally-addressed datagrams
                                 received successfully but discarded because
                                 of an unknown or unsupported protocol. */

  IP_STATS_IN_DELIVERS,       /* The total number of input datagrams succ-
                                 essfully delivered to IP user-protocols
                                 (including ICMP). */

  IP_STATS_OUT_REQUESTS,      /* The total number of IP datagrams which local
                                 IP user-protocols (including ICMP) supplied
                                 to IP in requests for transmission. */

  IP_STATS_COUNT              /* The number of IP stats. */
} ip_stats_enum;

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const ip_stats_name_ptr[IP_STATS_COUNT];

#endif /* FEATURE_DS_PSTATS */

/* IP header, INTERNAL representation */

#define IPLEN      20  /* Length of standard IP header */
#define IP_MAXOPT  40  /* Largest option field, bytes */

struct ip 
{
  ip4a source;      /* Source address */
  ip4a dest;        /* Destination address */
  uint16 length;    /* Total length */
  uint16 id;        /* Identification */
  uint16 offset;    /* Fragment offset in bytes */
  uint16 checksum;  /* Header checksum */

  struct 
  {
    unsigned int congest:1; /* Congestion experienced bit (exp) */
    unsigned int df:1;      /* Don't fragment flag */
    unsigned int mf:1;      /* More Fragments flag */
  } flags;

  uint8 version;            /* IP version number */
  uint8 tos;                /* Type of service */
  uint8 ttl;                /* Time to live */
  uint8 protocol;           /* Protocol */
  uint8 optlen;             /* Length of options field, bytes */
  uint8 options[IP_MAXOPT]; /* Options field */
};

/* Fields in option type byte */

#define OPT_COPIED  0x80  /* Copied-on-fragmentation flag */
#define OPT_CLASS   0x60  /* Option class */
#define OPT_NUMBER  0x1f  /* Option number */

/* IP option numbers */

#define IP_EOL    0      /* End of options list */
#define IP_NOOP   1      /* No Operation */
#define IP_SECURITY  2   /* Security parameters */
#define IP_LSROUTE  3    /* Loose Source Routing */
#define IP_TIMESTAMP  4  /* Internet Timestamp */
#define IP_RROUTE  7     /* Record Route */
#define IP_STREAMID  8   /* Stream ID */
#define IP_SSROUTE  9    /* Strict Source Routing */

/* Timestamp option flags */

#define TS_ONLY   0      /* Time stamps only */
#define TS_ADDRESS  1    /* Addresses + Time stamps */
#define TS_PRESPEC  3    /* Prespecified addresses only */

extern uint16 Id_cntr;   /* Datagram serial number */

/* Structure for handling raw IP user sockets */

struct raw_ip 
{
  struct raw_ip *next;   /* Linked list pointer */

  struct dsm_item_s *rcvq;            /* receive queue */
  void (*r_upcall)(struct raw_ip *);
  int protocol;                       /* Protocol */
  int user;                           /* User linkage */
};

//This is declared static in ip.c:
//extern struct raw_ip *Raw_ip;

/* Transport protocol link table */

struct iplink 
{
  char proto;
  void (*funct)(struct iface *,struct ip *,struct dsm_item_s *,int);
};

/*---------------------------------------------------------------------------
  ip_reasm_type is a structure used for storing the information about the 
  datagram and its fragments during the reassembling of the ip fragments.
  This structure contains:
  
    Queue to store the IP datagrams. Each of the queue will be used for a
    single datagram. This is a queue of the dsm_item_types. Each of the
    elements of this queue will be a fragment received. The fragments are 
    stored in the queue and after receiving all the fragments, they are made
    into a single element.
    
    IP Header is used to store the IP header that should be passed to the 
    upper layers. Note that each of the fragments will have the IP header,
    but standard (RFC) mentions that only the IP header of the first fragment
    i.e. offset = 0, should contain all the options. So, we need to pass only
    the header of the first offset to upper layers.
    
    in_use flag is used to indicate that the current element of the 
    reassembly structure/array is being used. 
    
    datagram_len gives the total length of the datagram length i.e. sum of 
    all the fragments that will be received. The value to be stored here is
    obtained by calculating the length from the last offset ( value of the
    last offset + length of the last fragment).
    
    Rcvd_bytes is used to calculate the number of the bytes received in the
    fragments for a particular datagram.
---------------------------------------------------------------------------*/
struct ip_reasm_type 
{
  q_type *fraglist_q;       /* Points to the datagrams                     */
  struct ip  ip;            /* Extracted IP header                         */
  boolean   in_use;         /* current element is used                     */
  uint16     datagram_len;  /* length of datagram. All fragments           */
  uint16     rcvd_bytes;    /* Bytes received till now                     */
};

/*---------------------------------------------------------------------------
  The number of elements in the reassembly structure. This value gives the
  total number of fragmented-datagrams that can be handled at a given time.
  
  Note that, if more than 2 datagrams may be fragmented at a given time, this
  should be increased.
---------------------------------------------------------------------------*/
#define NUM_DATA_FRAG 2

extern struct iplink Iplink[];

/* List of TCP port numbers to be given priority queuing */

extern int Tcp_interact[];

/* In ip.c: */

extern void ip_recv
(
  struct iface      *iface,
  struct ip         *ip,
  struct dsm_item_s *bp,
  int               rxbroadcast
);

extern int ip_send
(
  ip4a              source,
  ip4a              dest,
  uint8             protocol,
  uint8             tos,
  uint8             ttl,
  struct dsm_item_s *bp,
  uint16            length,    
  uint16            id,
  uint8             df
);

extern int ip_route
(
  struct iface      *i_iface, /* Input interface */
  struct dsm_item_s *bp,      /* Input packet */
  uint8             *source,  /* media source address, if any */
  int               broadcast /* True if packet had link broadcast address */
);

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* In iphdr.c: */

extern uint16 cksum
(
  pseudo_header_type  *ph,
  struct dsm_item_s    *m,
  uint16               len
);

extern uint16 eac
(
  uint32 sum
);

extern void htonip
(
  struct dsm_item_s **bpp,
  struct ip         *ip,
  int               cflag
);

extern int ntohip
(
  struct ip         *ip,
  struct dsm_item_s **bpp
);

/* In either lcsum.c or pcgen.asm: */

extern uint16 lcsum
(
  uint16 *wp,
  uint16 len
);

/*---------------------------------------------------------------------------
  ip_reasm_array is used for accessing the datagram information.
---------------------------------------------------------------------------*/
  extern struct ip_reasm_type ip_reasm_array[NUM_DATA_FRAG];

#ifdef FEATURE_DS_PSTATS

/*===========================================================================

FUNCTION IP_GET_STATS

DESCRIPTION
  This function will copy stats (enumerated in ip_stats_enum) into
  the given array.

DEPENDENCIES
  The given array has at least IP_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ip_get_stats
(
  word *stats_ptr /* Pointer to array into which stats will be copied */
);


/*===========================================================================

FUNCTION IP_RESET_STATS

DESCRIPTION
  This function will reset those IP stats which can be reported
  with ip_get_stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ip_reset_stats(void);

#endif /* FEATURE_DS_PSTATS */

/*=====================================================================

FUNCTION IP_FRAGHANDLE
Process IP datagram fragments. If datagram is complete, returns address of 
the datagram else returns NULL.


DESCRIPTION


DEPENDENCIES

RETURN VALUE
If datagram is complete, returns address of the datagram else returns NULL.

SIDE EFFECTS

=====================================================================*/

extern struct dsm_item_s *ip_fraghandle( struct ip *ip, struct dsm_item_s *bp);


/*===========================================================================

FUNCTION IP_LOOKUP_REASM

DESCRIPTION
  This function will look for the element in the re-assembly descriptor 
  array, that contains the same identifier, source, destination and protocol
  as the incoming packet. If a match for the parameters exist, then the 
  address of the array element is returned. If match is not found then NULL 
  is returned.

DEPENDENCIES
  None.

RETURN VALUE
  If a match for the parameters exist, then the address of the array element 
  is returned. If match is not found then NULL is returned.

SIDE EFFECTS
  None
===========================================================================*/

extern struct ip_reasm_type *ip_lookup_reasm(struct ip *ip);

/*===========================================================================

FUNCTION IP_CREATE_REASM

DESCRIPTION
  This function will create a reassembly descriptor and it will be used to 
  store the information of each IP packet.

DEPENDENCIES
  

RETURN VALUE
  The address of the free element of the reassembly structure.

SIDE EFFECTS
  None
===========================================================================*/

extern struct ip_reasm_type *ip_create_reasm(struct ip *ip);

/*===========================================================================

FUNCTION IP_FREE_REASM

DESCRIPTION
  This function will free a reassembly descriptor and it will be used to 
  store the information of each IP packet.

DEPENDENCIES

RETURN VALUE
 
SIDE EFFECTS
  None
==-----====================================================================*/

extern void ip_free_reasm(struct ip_reasm_type *rp);

/*===========================================================================

FUNCTION IP_REASM_INIT

DESCRIPTION
  This function will initialize the reassembly data structures.

DEPENDENCIES

RETURN VALUE
 
SIDE EFFECTS
  None
==-----====================================================================*/

extern void ip_reasm_init(void );

#endif /* IP_H */
