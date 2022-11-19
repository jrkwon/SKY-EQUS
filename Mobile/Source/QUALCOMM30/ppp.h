#ifndef PPP_H
#define PPP_H

/*===========================================================================

                                 P P P

DESCRIPTION
  Header file for the PPP protocol suite Interfacing functions.

   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/ppp.h_v   1.0   23 Oct 2000 17:32:26   jeffd  $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/04/00    mvl    changed interface to network model version of
                   ppp_outgoing() to fix bug where item was freed in this
                   function then freed again lower down.
12/10/99    mvl    Added comment in front of netmodel PPP stats.
10/28/99    mvl    Added code review changes
10/22/99    mvl    Added support for network model related statistics.
08/25/99    mvl    Added header for ppp_asy_in_init().
08/09/99    mvl    Added network model changes.
10/27/98    ldg    ARM porting changes: ROM to ROM const.
07/31/97    ldg    Added accessor function for UI to see some PPP vars.
06/12/97    ldg    Added new FEATURE_DS_PSTATS
11/09/95    jjw    IS-99 Compliant Release

===========================================================================*/


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/



/*
 *  Copyright 1992-1995 William Allen Simpson
 *  Licensed to Qualcomm -- 95 Jan 21
 *
 *  Acknowledgements and correction history may be found in PPP.C
 */
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "dsm.h"
#include "psi.h"
#ifdef FEATURE_DS_NET_MODEL
#include "dsnetmdl.h"
#endif


struct ppp_hdr
{
  uint8  addr;

#define HDLC_ALL_ADDR           0xff  /* HDLC all-station                  */

  uint8  control;

#define HDLC_UI                 0x03  /* HDLC Unnumbered Information       */

  uint16 protocol;

#define PPP_IP_PROTOCOL       0x0021  /* Internet Protocol                 */
#define PPP_OSINL_PROTOCOL    0x0023  /* OSI Network Layer                 */
#define PPP_XNS_PROTOCOL      0x0025  /* Xerox NS IDP                      */
#define PPP_DN4_PROTOCOL      0x0027  /* DECnet Phase IV                   */
#define PPP_AT_PROTOCOL       0x0029  /* Appletalk                         */
#define PPP_IPX_PROTOCOL      0x002b  /* Novell IPX                        */
#define PPP_VJCH_PROTOCOL     0x002d  /* Van Jacobson Compressed TCP/IP    */
#define PPP_VJUCH_PROTOCOL    0x002f  /* Van Jacobson Uncompressed TCP/IP  */
#define PPP_BPDU_PROTOCOL     0x0031  /* Bridging PDU                      */
#define PPP_ST2_PROTOCOL      0x0033  /* Stream Protocol (ST-II)           */
#define PPP_BV_PROTOCOL       0x0035  /* Banyan Vines                      */

#define PPP_8021D_PROTOCOL    0x0201  /* 802.1d Hello Packets              */
#define PPP_LUXCOM_PROTOCOL   0x0231  /* Luxcom                            */
#define PPP_SIGMA_PROTOCOL    0x0233  /* Sigma Network Systems             */

#define PPP_IPCP_PROTOCOL     0x8021  /* Internet Protocol Control Protocol*/
#define PPP_OSINLCP_PROTOCOL  0x8023  /* OSI Network Layer Control Protocol*/
#define PPP_XNSCP_PROTOCOL    0x8025  /* Xerox NS IDP Control Protocol     */
#define PPP_DN4CP_PROTOCOL    0x8027  /* DECnet Phase IV Control Protocol  */
#define PPP_ATCP_PROTOCOL     0x8029  /* Appletalk Control Protocol        */
#define PPP_IPXCP_PROTOCOL    0x802b  /* Novell IPX Control Protocol       */
#define PPP_BNCP_PROTOCOL     0x8031  /* Bridging NCP                      */
#define PPP_ST2CP_PROTOCOL    0x8033  /* Stream Protocol Control Protocol  */
#define PPP_BVCP_PROTOCOL     0x8035  /* Banyan Vines Control Protocol     */

#define PPP_LCP_PROTOCOL      0xc021  /* Link Control Protocol             */
#define PPP_PAP_PROTOCOL      0xc023  /* Password Authentication Protocol  */
#define PPP_CHAP_PROTOCOL     0xc223  /* Challenge Handshake A P           */
#define PPP_LQR_PROTOCOL      0xc025  /* Link Quality Report               */
};

#define PPP_HDR_LEN  4  /* Max bytes for PPP/HDLC envelope header          */
#define PPP_PADDING  8  /* Max bytes for PPP/HDLC padding                  */

/*---------------------------------------------------------------------------
  HDLC envelope constants
---------------------------------------------------------------------------*/
#define HDLC_ENVLEN  8         /* Max bytes for HDLC envelope (outgoing)   */

#define HDLC_FLAG       0x7e   /* HDLC async start/stop flag               */
#define HDLC_ESC_ASYNC  0x7d   /* HDLC transparency escape flag for async  */
#define HDLC_ESC_COMPL  0x20   /* HDLC transparency bit complement mask    */

#define HDLC_FCS_START  0xffff /* Starting bit string for FCS calculation  */
#define HDLC_FCS_FINAL  0xf0b8 /*FCS when summed over frame and sender FCS */


#ifdef FEATURE_DS_PSTATS

/*---------------------------------------------------------------------------
  Type to index the array of PPP stats.
---------------------------------------------------------------------------*/
typedef enum
{
  PPP_STATS_IN_LCP,       /* The number of LCP messages RX'd.              */
  PPP_STATS_OUT_LCP,      /* The number of LCP messages TX'd.              */

  PPP_STATS_IN_IPCP,      /* The number of IPCP messages RX'd.             */
  PPP_STATS_OUT_IPCP,     /* The number of IPCP messages TX'd.             */

#ifdef FEATURE_DS_NET_MODEL

  /*-------------------------------------------------------------------------
    The code where these statistics are formatted is sensitive to their order
    therefore it is IMPERATIVE that the ORDER of the statistics below
                               NOT BE CHANGED!
  -------------------------------------------------------------------------*/
  PPP_STATS_UM_FRAMED_PACKETS,  /* # Rm PPP packets that we don't unframe  */
  PPP_STATS_RM_FRAMED_PACKETS,  /* # Rm PPP packets that we don't unframe  */

  PPP_STATS_UM_UNFRAMED_PACKETS,  /* # of Um PPP packets that are unframed */
  PPP_STATS_RM_UNFRAMED_PACKETS,  /* # of Um PPP packets that are unframed */

  PPP_STATS_UM_TOTAL_PACKETS,   /* total PPP packets that arrive           */
  PPP_STATS_RM_TOTAL_PACKETS,   /* total PPP packets that arrive           */

#endif /* FEATURE_DS_NET_MODEL */

  PPP_STATS_IN_UNICAST,   /* The number of packets RX'd in internal mode.  */
  PPP_STATS_OUT_UNICAST,  /* The number of packets TX'd in internal mode.  */
  PPP_STATS_IN_DISCARDS,  /* The number of incoming packets discarded due
                             to lack of buffer space.                      */
  PPP_STATS_OUT_DISCARDS, /* The number of outgoing packets discarded due
                             to lack of buffer space                       */
  PPP_STATS_IN_ERRORS,    /* The number of packets received in error.      */
  PPP_STATS_OUT_ERRORS,   /* The number of outgoing packets in error
                             (bad IP packet, etc.)                         */

  PPP_STATS_IN_UNKNOWN,   /* The number of incoming packets with unknown
                             protocol.                                     */
  PPP_STATS_IN_CHECKSUM,  /* The number of incoming packets with checksum
                             error.                                        */
  PPP_STATS_COUNT         /* The number of PPP stats. */
} ppp_stats_enum;

#ifdef FEATURE_DS_DEBUG_UI
/*---------------------------------------------------------------------------
  Type to index the array of PPP stats for the debug UI.
---------------------------------------------------------------------------*/
typedef enum {
  PPP_UISTATS_STATE,      /* Copy of ppp_cb.phase */
  PPP_UISTATE_COUNT
} ppp_uistats_enum;

#endif /* FEATURE_DS_DEBUG_UI */

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const ppp_stats_name_ptr[PPP_STATS_COUNT];

/*---------------------------------------------------------------------------
  Copies of dword-sized PPP stats.  These are stored separately because
  the array of "word" will not hold them.
---------------------------------------------------------------------------*/
extern dword ppp_in_octets;
extern dword ppp_out_octets;
extern dword ppp_in_good_octets;

#endif /* FEATURE_DS_PSTATS */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION PPP_INIT

DESCRIPTION
  Initialize PPP control structures for a Point-to-Point interface (called
  from setencap)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_init
(
  ps_sigs_type lcp_timer_mask,
  ps_sigs_type ipcp_timer_mask
);

/*===========================================================================
FUNCTION PPP_DISCARD

DESCRIPTION
  This function will queue a PPP Discard packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
int ppp_discard( struct dsm_item_s *bp);


/*===========================================================================
FUNCTION PPP_INITIATE

DESCRIPTION
  This function will initiate the starting of a PPP session for IS-99.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_initiate( void);


/*===========================================================================
FUNCTION PPP_CLOSE

DESCRIPTION
  This function will initiate the closing of a PPP session.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_close( void);

/*===========================================================================
FUNCTION RESET_PPP_PKT 

DESCRIPTION
  This function will delete the packet attached to the ppp_pkt_head_ptr
  global and set the variable to NULL.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void reset_ppp_pkt( void);

/*===========================================================================
FUNCTION PPP_INCOMING

DESCRIPTION
  This function will process the passed PPP packet as an inbound packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_incoming
(
  dsm_item_type         *item_ptr     /* Pointer to the packet             */
#ifdef FEATURE_DS_NET_MODEL
  ,iface_stack_enum_type iface        /* iface (Rm or Um) pkt arrived on   */
#endif /* FEATURE_DS_NET_MODEL */
);

#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION PPP_OUTGOING

DESCRIPTION
  This function will process the passed datagram into a PPP packet for TX.

  This function is defined when FEATURE_DS_NET_MODEL is compiled in.

  Input Conditions:

  1. iface specifies the iface( Rm or Um) where the PPP pkt. is to be sent.

  2. The 'kind' field of the incoming_pkt_ptr specifies the protocol
     of this packet. This field can be:
     DSM_IP_COMPRESS: For IP datagrams. This will cause the packets to go
     through the VJ compressor.
     PPP_IP_PROTOCOL: For IP datagrams. IP packets will not go through
     the VJ compressor.
     PPP_LCP_PROTOCOL: For LCP proto.
     PPP_IPCP_PROTOCOL: For IPCP
     PPP_PAP_PROTOCOL: For PAP.
     PPP_VJCH_PROTOCOL: For VJ Compressed pkts.
     PPP_VJUCH_PROTOCOL: For VJ Uncompressed pkts.

     If it is not a known field, then the protocol parameter is used.

  3. For all packets the address and control
     fields(FF03) and the protocol fields should be stripped.

   Output Conditions:
     This function takes care to free the passed packet chain even if there
     is a error. If there is no error, it passes the packet chain to the
     lower layer.

DEPENDENCIES
  None

RETURN VALUE
  -1 on error.
   0 on success

SIDE EFFECTS
  None
===========================================================================*/
int ppp_outgoing
(
  iface_stack_enum_type out_iface,              /* iface to be sent on     */
  uint16                protocol,               /* protocol of packet      */
  struct dsm_item_s    **incoming_pkt_ptr       /* ptr to PPP packet chain */
);

#else /* if FEATURE_DS_NET_MODEL not defined */

/*===========================================================================
FUNCTION PPP_OUTGOING

DESCRIPTION
  This function is compiled in when FEATURE_DS_NET_MODEL is not defined.

  This function will process the passed datagram into a PPP packet for TX.

DEPENDENCIES
  None

RETURN VALUE
  -1 on error.
   0 on success.

SIDE EFFECTS
  None
===========================================================================*/
int ppp_outgoing
(
  struct dsm_item_s *incoming_pkt_ptr
);
#endif  


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION PPP_ASY_IN

DESCRIPTION
  This function will Packetize PPP input from asynchronous device

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_asy_in
(
  iface_stack_enum_type iface,        /* iface (Rm or Um) pkt arrived on   */
  dsm_item_type       **item_head_ptr /* address of ptr to a dsm_item_type */
);

#else /* if FEATURE_DS_NET_MODEL not defined */

/*===========================================================================
FUNCTION PPP_ASY_IN

DESCRIPTION
  This function will Packetize PPP input from asynchronous device

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_asy_in
(
  struct dsm_item_s **bpp             /* address of ptr to a dsm_item_type */
);

#endif /* FEATURE_DS_NET_MODEL */


#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================
FUNCTION PPP_ASY_IN_INIT

DESCRIPTION
  This function will free any packets that ppp_asy_in() had hanging around
  from the previous call.  Also clear out the various other params.

DEPENDENCIES
Should only be called when entering Autodetect mode!

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_asy_in_init
(
  iface_cb_type *iface_cb_ptr           /* the iface control block to init */ 
);
#endif



#ifdef FEATURE_DS_NET_MODEL
/*===========================================================================

FUNCTION PPP_NET_MODEL_INIT

DESCRIPTION
  This function initializes LCP and IPCP for network model operation.
  It also sets up the LCP and IPCP timers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_net_model_init
(
  ps_sigs_type lcp_timer_mask,           /* timer mask for LCP             */
  ps_sigs_type ipcp_timer_mask           /* timer mask for IPCP            */
);
#endif /* FEATURE_DS_NET_MODEL */


#ifdef FEATURE_DS_PSTATS
/*===========================================================================
FUNCTION PPP_GET_STATS

DESCRIPTION
  This function will copy stats (enumerated in ppp_stats_enum) to the
  given array.

DEPENDENCIES
  The given array contains at least PPP_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_get_stats
(
  word *stats_ptr     /* Pointer to array into which stats will be copied. */
);

/*===========================================================================
FUNCTION PPP_RESET_STATS

DESCRIPTION
  This function will reset those PPP stats which are reported
  with ppp_get_stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_reset_stats(void);

#ifdef FEATURE_DS_DEBUG_UI
/*===========================================================================
FUNCTION PPP_GET_STATS_UI

DESCRIPTION
  This function will copy stats (enumerated in ppp_uistats_enum) to the
  given array for the debug UI.

DEPENDENCIES
  The given array contains at least PPP_UISTATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ppp_get_stats_ui
(
  word *stats_ptr     /* Pointer to array into which stats will be copied. */
);
#endif /* FEATURE_DS_DEBUG_UI */

#endif /* FEATURE_DS_PSTATS */

#ifdef	FEATURE_SKT_GVM_ONLINE	//	khekim 01/04/18
extern void resetPPPPacket
(
	void
);
#endif /* FEATURE_SKT_GVM_ONLINE */
#endif  /* PPP_H */
