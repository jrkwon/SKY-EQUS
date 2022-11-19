#ifndef ICMP_H
#define ICMP_H

/*===========================================================================

                   M O D E L    H E A D E R    F I L E

DESCRIPTION


   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/icmp.h_v   1.0   23 Oct 2000 17:32:22   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/12/97   ldg     Added new FEATURE_DS_PSTATS
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/

#include "psglobal.h"
#include "customer.h"       /* Customer Specific Features */
#include "dsm.h"
#include "iface.h"
#include "internet.h"
#include "ip.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/

/* Internet Control Message Protocol */

/* Message types */

#define ICMP_ECHO_REPLY        0  /* Echo Reply */
#define ICMP_DEST_UNREACH      3  /* Destination Unreachable */
#define ICMP_QUENCH            4  /* Source Quench */
#define ICMP_REDIRECT          5  /* Redirect */
#define ICMP_ECHO              8  /* Echo Request */
#define ICMP_TIME_EXCEED      11  /* Time-to-live Exceeded */
#define ICMP_PARAM_PROB       12  /* Parameter Problem */
#define ICMP_TIMESTAMP        13  /* Timestamp */
#define ICMP_TIME_REPLY       14  /* Timestamp Reply */
#define ICMP_INFO_RQST        15  /* Information Request */
#define ICMP_INFO_REPLY       16  /* Information Reply */
#define ICMP_ADDR_MASK        17  /* Address mask request */
#define ICMP_ADDR_MASK_REPLY  18  /* Address mask reply */
#define ICMP_FQDN_REQUEST     37  /* Domain Name Request */
#define ICMP_FQDN_REPLY       38  /* Domain Name Reply */
#define ICMP_TYPES            39  /* (next) */

/* Internal format of an ICMP header (checksum is missing) */
struct icmp 
{
  uint8 type;
  uint8 code;
  union icmp_args 
  {
    uint16 mtu;
    uint32 unused;
    uint8  pointer;
    ip4a   address;
    struct 
    {
      uint16 id;
      uint16 seq;
    } echo;
  } args;
};

#define ICMPLEN   8  /* Length of ICMP header on the net */

/* Destination Unreachable codes */

#define ICMP_NET_UNREACH  0   /* Net unreachable */
#define ICMP_HOST_UNREACH  1  /* Host unreachable */
#define ICMP_PROT_UNREACH  2  /* Protocol unreachable */
#define ICMP_PORT_UNREACH  3  /* Port unreachable */
#define ICMP_FRAG_NEEDED  4   /* Fragmentation needed and DF set */
#define ICMP_ROUTE_FAIL   5   /* Source route failed */
#define ICMP_ADMIN_PROHIB  13 /* Administrativly prohibbited */

#define NUNREACH    14  /* (next) */

/* Time Exceeded codes */

#define ICMP_TTL_EXCEED   0  /* Time-to-live exceeded */
#define ICMP_FRAG_EXCEED  1  /* Fragment reassembly time exceeded */

#define NEXCEED     2  /* (next) */

/* Redirect message codes */

#define ICMP_REDR_NET  0  /* Redirect for the network */
#define ICMP_REDR_HOST  1 /* Redirect for the host */
#define ICMP_REDR_TOS  2  /* Redirect for Type of Service, or-ed with prev */

#define NREDIRECT  3  /* (next) */

#ifdef FEATURE_DS_PSTATS

/*---------------------------------------------------------------------------
  Type to index the array of ICMP stats.
---------------------------------------------------------------------------*/
typedef enum
{
  /* Stats from ICMP MIB, RFC 2011. */

  ICMP_STATS_IN_MSGS,          /* The total number of ICMP messages which
                                  the entity received.  Note that this
                                  counter includes all those counted by
                                  icmpInErrors.                            */

  ICMP_STATS_OUT_MSGS,         /* The total number of ICMP messages which
                                  this entity attempted to send.  Note that
                                  this counter includes all those counted
                                  by icmpOutErrors.                        */

  ICMP_STATS_IN_ECHO_REQUEST,  /* # of ICMP Echo (request) messages RX'd
                                  (see icmpInEcho)                         */

  ICMP_STATS_IN_TIMESTAMP,     /* # of ICMP Timestamp (request) msgs RX'd  */

  /* Stats derived from ICMP.C, calls to MSG_*. */

  ICMP_STATS_IN_INFO_REQUEST,  /* # of ICMP Info Request messages RX'd     */

  ICMP_STATS_IN_BROADCAST,     /* # of broadcast ICMP messages RX'd        */

  ICMP_STATS_IN_CHECKSUM,      /* # of messages with bad ICMP checksum
                                  (see icmpInErrors)                       */

  ICMP_STATS_IN_INVALID,       /* # of invalid ICMP messages.              */

  ICMP_STATS_COUNT             /* # of ICMP stats                          */
} icmp_stats_enum;

/*---------------------------------------------------------------------------
  Array of ICMP stats.
  These stats are cumulative since the last icmp_reset_stats or power-on.
---------------------------------------------------------------------------*/
extern word icmp_stats[ICMP_STATS_COUNT];

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const icmp_stats_name_ptr[ICMP_STATS_COUNT];

#endif /* FEATURE_DS_PSTATS */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* In icmp.c: */

void icmp_input
(
  struct iface      *iface,
  struct ip         *ip,
  struct dsm_item_s *bp,
  int               rxbroadcast
);

void htonicmp
(
  struct icmp   *icmp,
  dsm_item_type **bpp
);

int ntohicmp
(
  struct icmp   *icmp,
  dsm_item_type **bpp
);

#ifdef FEATURE_DS_PSTATS

/*===========================================================================

FUNCTION ICMP_GET_STATS

DESCRIPTION
  This function will copy ICMP stats (enumerated in icmp_stats_enum)
  into the given array.

DEPENDENCIES
  The given array must have at least ICMP_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void icmp_get_stats
(  
  word *stats_ptr /* Pointer to array into which stats will be copied */
);

/*===========================================================================

FUNCTION ICMP_RESET_STATS

DESCRIPTION
  This function will reset those ICMP stats which can be reported
  with icmp_get_stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void icmp_reset_stats(void);

#endif /* FEATURE_DS_PSTATS */

#endif  /* ICMP_H */
