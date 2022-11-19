#ifndef TCP_H
#define TCP_H

/*===========================================================================

                              T C P

DESCRIPTION

   This TCP implementation follows RFC 793 as closely as possible.
   This header file includes type definitions for the TCP control block and
   other data structures. It also includes the declarations for the external
   functions of TCP.

   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1999,2000 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/tcp.h_v   1.3   27 Nov 2000 01:03:04   nabrol  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/27/00   na      Cleanup. Rearranged struct tcb to save memory.
11/10/00   snn     Rearranged the elements in the structure tcb. They
                   are arranged according to the types. The structure
                   definitions were moved out of the structure.
                   Renamed dword to uint32 and word to uint16
                   Changed tcp_state to tcp_state_enum_type
                   Renamed callback functions with _cb post-fix.
                   
10/10/00   na      Modified the headers for tcp_delayed and tcp_timeout.
09/05/00   snn     Modified the Default window to 4 times MSS to allow
                   higher throughput in forward direction.
08/25/00   rc      Changed FEATURE_DS_MEM_CHK to FEATURE_DSM_MEM_CHK
11/16/99   rc      Removed an unused field in tcb struct.
08/11/99   ak/rc   Added tcp_ip_upcall fcn ptr to tcb struct, to support
                   ppp renegotiating a new IP address when in sockets mode.
02/25/98   na/hcg  Added support for sockets.  Added socket control block to 
                   TCB structure.  Changed default MSL value for sockets.
12/14/98   ldg     Removed dangling comma from tcp_cleanup_reseq() parameter
10/27/98   ldg     Moved #include psglobal.h to inside FEATURE_DS.
                   Added #include iface.h.
                   ARM porting: ROM to ROM const.  enum to typedef enum.
10/29/98   na      Added prototype for tcp_cleanup_reseq.
04/21/98   na      Pre code review cleanup.
04/14/98   ldg     Changed method of counting backoffs.
                   Added counters for payload sent and received.
                   Removed apparatus for dumping rcvcnt and sndcnt stats.
04/14/98   na      (ldg) made changes for reporting TCP backoff statistics.
12/15/97   ldg     Moved TCP out-of-order segment count from UI stats to
                   general stats.
11/06/97   ldg     Added prototype for send_reset().
10/16/97   na      Changed TCP_MAX_BACKOFFS from 6 to 10.
09/24/97   na      Added TCP_MAX_BACKOFFS to limit the number of attempts
                   at retransmissions.
07/31/97   ldg     Added accessor function for UI to see some TCP vars.
06/16/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/12/97   ldg     Added new FEATURE_DS_PSTATS
11/14/96   jjw     Unwired Planet Upgrade (ie added UDP)
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"       /* Customer Specific Features */
#include "target.h"

#ifdef FEATURE_DS
#include "dsm.h"
#include "queue.h"
#include "rex.h"
#include "iface.h"
#include "internet.h"
#include "ip.h"
#include "netuser.h"
#include "psglobal.h"

#ifdef FEATURE_DS_SOCKETS
#include "dssocki.h"
#endif /* FEATURE_DS_SOCKETS */



/*===========================================================================

                      EXTERNAL MACRO DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Macros to compare TCP sequence numbers.
---------------------------------------------------------------------------*/
#define seq_ge(x,y) ((sint31)((uint32)(x)-(uint32)(y)) >= 0)

#define seq_gt(x,y) ((sint31)((uint32)(x)-(uint32)(y)) > 0)

#define seq_lt(x,y) ((sint31)((uint32)(x)-(uint32)(y)) < 0)

/*---------------------------------------------------------------------------
  Generate Initial Sequence Number Macro. Increment clock at 4MB/sec.
---------------------------------------------------------------------------*/
#define geniss()  ((uint32)msclock() << 12)



/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define DEF_MSS 536              /* Default maximum segment size           */
#define DEF_WND (DEF_MSS*4)      /* Default receiver window                */
#define DEF_RTT 1000L            /* Initial guess at round trip time (ms)  */
#define MSL2  30                 /* Guess at two maximum-segment lifetimes */
#define MAX_RTO 120000L          /* Maximum Retransmission timeout (ms)    */
#define TCP_MAX_BACKOFFS 10      /* Maximum # of TCP backoff attempts      */
#define MIN_RTO 500L             /* Minimum Retransmission timeout (ms)    */
#define MAX_ATO 500L             /* Maximum Delayed Ack timeout (ms)       */
#define MIN_ATO 100L             /* Minimum Delayed Ack timeout (ms)       */
#define TCP_LIMIT 4              /* limit number of outstanding MSS        */
#define TCP_SYNDATA 1            /* Data sent with Syn                     */
#define SSTHRESH 65535U          /* Initial Slow Start Threshhold          */
#define TCPDUPACKS  3            /* # of conseq. dup acks for fast recovery*/

/*---------------------------------------------------------------------------
  Round Trip Time calculation parameters.
---------------------------------------------------------------------------*/
#define AGAIN   8                /* Average RTT gain = 1/8                 */
#define LAGAIN  3                /* Log2(AGAIN)                            */
#define DGAIN   4                /* Mean deviation gain = 1/4              */
#define LDGAIN  2                /* log2(DGAIN)                            */

/*---------------------------------------------------------------------------
  TCP segment header -- internal representation
  Note that this structure is NOT the actual header as it appears on the
  network (in particular, the offset field is missing).
  All that knowledge is in the functions ntohtcp() and htontcp() in tcpsubr.c
---------------------------------------------------------------------------*/
#define TCPLEN      20           /* Minimum Header length, bytes           */
#define TCP_MAXOPT  40           /* Largest option field, bytes            */
struct tcp 
{
  uint16 source;                 /* Source port                            */
  uint16 dest;                   /* Destination port                       */
  uint32 seq;                    /* Sequence number                        */
  uint32 ack;                    /* Acknowledgment number                  */
  uint16 wnd;                    /* Receiver flow control window           */
  uint16 checksum;               /* Checksum                               */
  uint16 up;                     /* Urgent pointer                         */
  uint16 mss;                    /* Optional max seg size                  */
  struct 
  {
    unsigned int congest:1;      /* Echoed IP congestion experienced bit   */
    unsigned int urg:1;          /* Urgent Bit                             */
    unsigned int ack:1;          /* This segment contains acknowledgement  */
    unsigned int psh:1;          /* PUSH bit                               */
    unsigned int rst:1;          /* RESET the TCP connection               */
    unsigned int syn:1;          /* SYN bit - sync up the TCP connection   */
    unsigned int fin:1;          /* FIN bit - finish the TCP connection    */
  } flags;
  uint8 optlen;                  /* Length of options field, bytes         */
  uint8 options[TCP_MAXOPT];     /* Options field                          */
};

/*---------------------------------------------------------------------------
  TCP option numbers.
---------------------------------------------------------------------------*/
#define EOL_KIND    0
#define NOOP_KIND   1
#define MSS_KIND    2
#define MSS_LENGTH  4

/*---------------------------------------------------------------------------
  These numbers match those defined in the MIB for TCP connection state 
---------------------------------------------------------------------------*/
typedef enum 
{
  TCP_CLOSED=1,
  TCP_LISTEN,
  TCP_SYN_SENT,
  TCP_SYN_RECEIVED,
  TCP_ESTABLISHED,
  TCP_FINWAIT1,
  TCP_FINWAIT2,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_CLOSING,
  TCP_TIME_WAIT
}tcp_state_enum_type ;

/*---------------------------------------------------------------------------
  Send sequence variables
---------------------------------------------------------------------------*/
struct tcb_snd_seq_s
{                    
  uint32       una;    /* First unacknowledged sequence number             */
  uint32       nxt;    /* Next sequence num to be sent for the first time  */
  uint32       ptr;    /* Working transmission pointer                     */
  uint32       wl1;    /* Sequence number used for last window update      */
  uint32       wl2;    /* Ack number used for last window update           */
  uint32       wnd;    /* Other end's offered receive window               */
  uint16       up;     /* Send urgent pointer                              */
};
                                                                           
/*---------------------------------------------------------------------------
   Receive sequence variables 
---------------------------------------------------------------------------*/
struct tcb_rcv_seq_s
{
  uint32       nxt;              /* Incoming sequence number expected next */
  uint32       wnd;              /* Our offered receive window             */
  uint16       up;               /* Receive urgent pointer                 */
};                                 

/*---------------------------------------------------------------------------
   TCP control flags
---------------------------------------------------------------------------*/
struct tcb_flags_s  
{   
  unsigned int force:1;          /* Force transmit a segment               */
  unsigned int clone:1;          /* Server-type TCB,cloned on incoming SYN */
  unsigned int retran:1;         /* A retransmission has occurred          */
  unsigned int active:1;         /* TCB created with an active open        */
  unsigned int synack:1;         /* Our SYN has been acked                 */
  unsigned int rtt_run:1;        /* We're timing a segment                 */
  unsigned int congest:1;        /* Copy of last IP congest bit received   */
};

/*---------------------------------------------------------------------------
  TCP connection control block.
  Note, this has been arranged so that the fields are packed.
---------------------------------------------------------------------------*/
struct tcb   
{
  uint32         iss;            /* Initial send sequence number           */
  uint32         resent;         /* Count of bytes retransmitted           */
  uint32         cwind;          /* Congestion window                      */
  uint32         ssthresh;       /* Slow-start threshold                   */
  uint32         irs;            /* Initial receive sequence number        */
  uint32         rerecv;         /* Count of duplicate bytes received      */
  uint32         mss;            /* Maximum segment size                   */
  uint32         window;         /* Receiver window and send queue limit   */
  uint32         limit;          /* User specified congestion window limit */
  uint32         minrto;         /* User specified minimum rto             */
  uint32         maxrto;         /* User specified maximum rto             */
  uint32         rtt_time;       /* Stored clock values for RTT            */
  uint32         rttseq;         /* Sequence number being timed            */
  uint32         rttack;         /* Ack at start of timing (for txbw calc) */
  uint32         srtt;           /* Smoothed round trip time, milliseconds */
  uint32         mdev;           /* Mean deviation, milliseconds           */
  uint32         rtt;            /* Last received RTT (for debugging)      */
  uint32         rttds;          /* Time sequence datagram size            */
  uint32         quench;         /* Count of incoming ICMP source quenches */
  uint32         unreach;        /* Count of incoming ICMP unreachables    */
  uint32         timeouts;       /* Count of retransmission timeouts       */
  uint32         lastack;        /* Time of last received ack              */
  uint32         txbw;           /* Estimate of transmit bandwidth         */
  uint32         lastrx;         /* Time of last received data             */
  uint32         rxbw;           /* Estimate of receive bandwidth          */
  uint32         tcp_rexmit_timer_val; /* Rexmit timer value               */  
  dsm_item_type *sndq_tail_ptr;  /* Ptr to last item on send queue         */
  dsm_item_type *rcvq;           /* Receive queue                          */
  dsm_item_type *sndq;           /* Send queue                             */
#ifdef FEATURE_DS_SOCKETS
  struct         scb_s *scb_ptr; /* Ptr to socket ctl block                */
#endif
  
  /*-------------------------------------------------------------------------
    Call when "significant" amount of data arrives 
  -------------------------------------------------------------------------*/ 
  void (*r_upcall)(struct tcb *tcb,uint32 cnt);  

  /*-------------------------------------------------------------------------
    Call when ok to send more data
  -------------------------------------------------------------------------*/ 
  void (*t_upcall)(struct tcb *tcb,uint32 cnt);     

  /*-------------------------------------------------------------------------
    Call when connection state changes
  -------------------------------------------------------------------------*/ 
  void (*s_upcall)(struct tcb *tcb,
                   tcp_state_enum_type old, 
                   tcp_state_enum_type new
                  ); 

  q_type         reseq;          /* Out-of-order segment queue             */
  int timer;                     /* Timer ID for TCP timeout timer         */
  int acker;                     /* Timer ID for TCP Delayed ACK timer     */
  int close_timer;               /* Timer ID for TCP close timer           */
  struct         connection conn;/* TCP 4-tuple; IP addr. & port numbers   */
  int user;                      /* User params                            */
  struct         tcb_snd_seq_s snd;/* Transmit data structures             */ 
  uint16 sndcnt;                 /* # of unack. seq# on sndq- incl. SYN/FIN*/
  struct         tcb_rcv_seq_s rcv;/* Receive data structures              */
  uint16 rcvcnt;                 /* Count of items on rcvq                 */
#ifdef FEATURE_DSM_MEM_CHK
  uint16 sndcnt_himark;          /* Highest count for send Queue           */
  uint16 rcvcnt_himark;          /* Highest count for receive Queue        */
#endif
  tcp_state_enum_type state;     /* State of TCP                           */
  struct         tcb_flags_s flags;/* TCP flags                            */

  byte dupacks;                  /* Count of duplicate (do-nothing) ACKs   */
  byte backoff;                  /* Number of backoffs                     */
  uint8 reason;                  /* Reason for closing                     */
#define NORMAL    0              /* Normal close                           */
#define RESET     1              /* Reset by other end                     */
#define TIMEOUT   2              /* Excessive retransmissions              */
#define NETWORK   3              /* Network problem (ICMP message)         */

  /*-------------------------------------------------------------------------
    If reason == NETWORK, the ICMP type and code values are stored here     
  -------------------------------------------------------------------------*/
  uint8 type;                    /* ICMP type value                        */
  uint8 code;                    /* ICMP code value                        */
  uint8 tos;                     /* Type of service (for IP)               */
};
  
/*---------------------------------------------------------------------------
   TCP round-trip timer cache
---------------------------------------------------------------------------*/
struct tcp_rtt 
{
  ip4a addr;                     /* Destination IP address                 */
  uint32 srtt;                   /* Most recent SRTT                       */
  uint32 mdev;                   /* Most recent mean deviation             */
  uint32 rttds;                  /* Most recent RTT datagram size          */
};

extern struct tcp_rtt Tcp_rtt;

/*---------------------------------------------------------------------------
   TCP statistics counters.
---------------------------------------------------------------------------*/
struct tcp_stat   
{
  uint16 runt;                   /* Smaller than minimum size              */
  uint16 checksum;               /* TCP header checksum errors             */
  uint16 conout;                 /* Outgoing connection attempts           */
  uint16 conin;                  /* Incoming connection attempts           */
  uint16 resets;                 /* Resets generated                       */
  uint16 bdcsts;                 /* Bogus broadcast packets                */
};

/*---------------------------------------------------------------------------
   TCP MIB definition.
---------------------------------------------------------------------------*/
extern  struct          mib_entry Tcp_mib[];
#define tcpRtoAlgorithm Tcp_mib[1].value.integer
#define tcpRtoMin       Tcp_mib[2].value.integer
#define tcpRtoMax       Tcp_mib[3].value.integer
#define tcpMaxConn      Tcp_mib[4].value.integer
#define tcpActiveOpens  Tcp_mib[5].value.integer
#define tcpPassiveOpens Tcp_mib[6].value.integer
#define tcpAttemptFails Tcp_mib[7].value.integer
#define tcpEstabResets  Tcp_mib[8].value.integer
#define tcpCurrEstab    Tcp_mib[9].value.integer
#define tcpInSegs       Tcp_mib[10].value.integer
#define tcpOutSegs      Tcp_mib[11].value.integer
#define tcpRetransSegs  Tcp_mib[12].value.integer
#define tcpInErrs       Tcp_mib[14].value.integer
#define tcpOutRsts      Tcp_mib[15].value.integer

#define NUMTCPMIB  15

#ifdef FEATURE_DS_PSTATS
/*---------------------------------------------------------------------------
  Type to index the array of TCP stats.
  From TCP MIB, rfc 2012
---------------------------------------------------------------------------*/
typedef enum
{
  TCP_STATS_ACTIVE_OPENS,  /* The number of times TCP connections have made
                              a direct transition to the SYN-SENT state from
                              the CLOSED state. */

  TCP_STATS_PASSIVE_OPENS, /* The number of times TCP connections have made a
                              direct transition to the SYN-RCVD state from the
                              LISTEN state. */

  TCP_STATS_ATTEMPT_FAILS, /* The number of times TCP connections have made a
                              direct transition to the CLOSED state from
                              either the SYN-SENT state or th SYN-RCVD state,
                              plus the number of times TCP connections have
                              made a direct transition to the LISTEN state
                              from the SYN-RCVD state. */

  TCP_STATS_ESTAB_RESETS,  /* The number of times TCP connections have made a
                              direct transition to the CLOSED state from
                              either the ESTABLISHED state or the CLOSE-WAIT
                              state. */

  TCP_STATS_IN_SEGS,       /* The total number of segments received, including
                              those received in error.  This count includes
                              segments received on currently established
                              connections. */

  TCP_STATS_OUT_SEGS,      /* The total number of segments sent, including
                              those on current connections but excluding those
                              containing only retransmitted octets. */

  TCP_STATS_RETRANS_SEGS,  /* The total number of segments retransmitted -
                              that is, the number of TCP segments transmitted
                              containing one or more previously transmitted
                              octets. */

  TCP_STATS_IN_ERRS,       /* The total number of segments received in error
                              (e.g., bad TCP checksums). */

  TCP_STATS_OUT_RSTS,      /* The number of TCP segments sent containing the
                              RST flag.*/

  /* Stats from Tcb */

  TCP_STATS_RX_OUT_OF_ORDER,
                           /* Number of times a segment was received out of
                              order */

  TCP_STATS_COUNT          /* Number of TCP stats. */
} tcp_stats_enum;

#ifdef FEATURE_DS_DEBUG_UI
typedef enum {
  TCP_UISTATS_STATE,        /* From Tcb.state */
  TCP_UISTATS_COUNT
} tcp_uistats_enum;
#endif /* FEATURE_DS_DEBUG_UI */

/*---------------------------------------------------------------------------
  Counters for payload sent and received.
---------------------------------------------------------------------------*/
extern uint32 tcp_payload_sent;
extern uint32 tcp_payload_received;

/*---------------------------------------------------------------------------
  Counter array for backoff runs, and counter of backoff instances
---------------------------------------------------------------------------*/
#define TCP_BACKOFF_ARRAY_SIZE 10
extern byte tcp_backoffs[TCP_BACKOFF_ARRAY_SIZE];
extern byte tcp_backoff_instance;

/*---------------------------------------------------------------------------
  Counter for segments received out of order.
---------------------------------------------------------------------------*/
extern uint16 tcp_rx_out_of_order;

/*---------------------------------------------------------------------------
  Array of pointers to stats names.
---------------------------------------------------------------------------*/
extern const byte * const tcp_stats_name_ptr[TCP_STATS_COUNT];

#endif /* FEATURE_DS_PSTATS */

/*---------------------------------------------------------------------------
  TCP control block used for async calls.
---------------------------------------------------------------------------*/
extern struct tcb Tcb;    

/*---------------------------------------------------------------------------
  Arrays of TCP states and reasons.
---------------------------------------------------------------------------*/
extern char *Tcpstates[];
extern char *Tcpreasons[];



/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/* In tcphdr.c: */
void htontcp
(
  struct dsm_item_s    **bpp,
  struct tcp           *tcph,
  pseudo_header_type *ph
);


int ntohtcp
(
  struct tcp *tcph,
  struct dsm_item_s **bpp
);

/* In tcpin.c: */

void reset
(
  struct ip *ip,
  struct tcp *seg
);


void send_syn
(
  struct tcb *tcb
);


void tcp_input
(
  struct iface      *iface,
  struct ip         *ip,
  struct dsm_item_s *bp,
  int               rxbroadcast
);


void tcp_icmp
(
  ip4a              icsource,
  ip4a              source,
  ip4a              dest,
  uint8             type,
  uint8             code,
  struct dsm_item_s **bpp
);


/* In tcpsubr.c: */

void close_self
(
  struct tcb *tcb,
  int        reason
);


struct tcb *create_tcb
(
  struct connection *conn
);

struct tcb *lookup_tcb(struct connection *conn);

void rtt_add
(
  ip4a   addr,
  uint32 rtt
);


struct tcp_rtt *rtt_get
(
  ip4a addr
);


int seq_within
(
  uint32 x,
  uint32 low,
  uint32 high
);

/* In tcpshell.c */

sint15 send_tcp
(
  struct tcb        *tcb,
  struct dsm_item_s *bp
);

extern struct tcb *open_tcp
(
  struct socket *lsocket,  /* Local socket */
  struct socket *fsocket,  /* Remote socket */
  int           mode,      /* Active/passive/server */
  uint16        window,    /* Receive window (and send buffer) sizes */
  int           tos,       /* Type of service   */
  int           user,      /* User linkage area */
  struct tcb    *Tcb_ptr
);



void setstate(struct tcb *tcb, tcp_state_enum_type newstate);

/* In tcpout.c: */

void tcp_output
(
  struct tcb *tcb
);

/* In tcptimer.c: */

void tcp_delayed_cb
(
  void *
);

void tcp_timeout_cb
(
  void *
);


/* In tcpuser.c: */

int close_tcp
(
  struct tcb *tcb
);

uint32 recv_tcp
(
  struct tcb        *tcb,
  struct dsm_item_s **bpp,
  uint32            cnt
);

void reset_tcp
(
  struct tcb *tcb
);

char *tcp_port
(
  uint16 n
);

int tcpval
(
  struct tcb *tcb
);

extern void set_ip_addresses
( 
  ip_addr_type local_address,     /* local IP address  */
  ip_addr_type remote_address     /* remote IP address */
);


/*===========================================================================

FUNCTION SEND_RESET

DESCRIPTION
  This function will Send an acceptable reset (RST) response for this segment
  The RST reply is composed in place on the input segment

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void send_reset
(
  struct ip           *ip,             /* Offending IP header */
  register struct tcp *seg             /* Offending TCP header */
);



/*===========================================================================

FUNCTION TCP_CLEANUP_RESEQ

DESCRIPTION
  This function cleans up the resequencing queue for the given TCP connection

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tcp_cleanup_reseq 
(
  register struct tcb *tcb        /* ptr to TCP Task control block         */
);

#ifdef FEATURE_DS_PSTATS
/*===========================================================================

FUNCTION TCP_GET_STATS

DESCRIPTION
  This function copies TCP stats (enumerated in tcp_stats_enum)
  into the given array.

DEPENDENCIES
  The given array must have at least TCP_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tcp_get_stats
(  
  uint16 *stats_ptr /* Pointer to the array into which stats will be copied */
);

/*===========================================================================

FUNCTION TCP_RESET_STATS

DESCRIPTION
  This function will reset those TCP stats which can be reported
  with tcp_get_stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tcp_reset_stats(void);


#ifdef FEATURE_DS_DEBUG_UI
/*===========================================================================

FUNCTION TCP_GET_STATS_UI

DESCRIPTION
  This function copies TCP stats into the given array forthe debug UI.

DEPENDENCIES
  The given array must have at least TCP_UISTATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tcp_get_stats_ui
(
  uint16 *stats_ptr /* Pointer to the array into which stats will be copied*/
);
#endif /* FEATURE_DS_DEBUG_UI */

#endif /* FEATURE_DS_PSTATS */

#endif /* FEATURE_DS */
#endif  /* TCP_H */
