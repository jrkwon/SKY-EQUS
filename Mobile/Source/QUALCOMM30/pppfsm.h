#ifndef PPPFSM_H
#define PPPFSM_H

/*===========================================================================

                              P P P F S M

DESCRIPTION
  Header file for the PPP protocol suite finite state machine

   Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
   Copyright (c) 1997 by QUALCOMM, Incorporated.  All Rights Reserved.
   Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/COMMON/vcs/pppfsm.h_v   1.0   23 Oct 2000 17:32:28   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/20/99   mvl     Added macro CONFIG_HDR_LEN
08/09/99   mvl     Added network model changes.
04/08/99   na      Added dsm_kind field to the fsm_constant_s struct
11/09/95   jjw     IS-99 Compliant Release

===========================================================================*/


#include "dsm.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/


/*
 *  Copyright 1992-1995 William Allen Simpson
 *  Licensed to Qualcomm -- 95 Jan 21
 */

/* feature set */
#define PPP_IP

/* currently these bits are scattered about the iface trace word */
#ifdef	FEATURE_SKT_DS	//	khekim 00/11/07
#include "msg.h"
#if MSG_LEVEL!=MSG_LVL_NONE	//	khekim 00/11/07
#define	PPP_TRACING
#endif	/* MSG_LEVEL!=MSG_LVL_NONE */

#ifdef	PPP_TRACING
typedef	struct
{
	word	trace;
} PPPIFACE_TYPE;
extern	PPPIFACE_TYPE	PPPiface[1];

#define	PPP_DEBUG_OPTIONS	0x0001
#define	PPP_DEBUG_PACKET	0x0002
#define	PPP_DEBUG_MSG		0x0004
#define	PPP_DEBUG_FSA		0x0008

extern	void	trace_log(PPPIFACE_TYPE pppIface[1], const char *fmt, ...);
#define PPP_DEBUG_CHECKS(x)		if(PPPiface->trace & PPP_DEBUG_MSG) \
								MSG_ERROR(x, 0, 0, 0)
#define PPP_DEBUG_ROUTINES(x)	if(PPPiface->trace & PPP_DEBUG_MSG) \
								MSG_MED(x, 0, 0, 0)
#define PPP_DEBUG_BOOLEAN(x)	if(PPPiface->trace & PPP_DEBUG_OPTIONS) \
								MSG_HIGH(x, 0, 0, 0)
#define PPP_DEBUG_VALUE(x,y)	if(PPPiface->trace & PPP_DEBUG_OPTIONS) \
								MSG_HIGH(x, y, 0, 0)
#define FSM_LOG(x,y)			if(PPPiface->trace & PPP_DEBUG_OPTIONS) \
								MSG_HIGH(y, 0, 0, 0)
#else	/* PPP_TRACE */

#define PPP_DEBUG_CHECKS(x)
#define PPP_DEBUG_ROUTINES(x)
#define PPP_DEBUG_BOOLEAN(x)
#define PPP_DEBUG_VALUE(x,y)
#define FSM_LOG(x,y)

#endif	/* PPP_TRACE */

#else	/* FEATURE_SKT_DS */

#ifdef  PPP_TRACING

#define PPP_DEBUG_CHECKS(x)  if(PPPiface->trace & 0x0004) \
        trace_log(PPPiface,x);
#define PPP_DEBUG_ROUTINES(x)  if(PPPiface->trace & 0x0040) \
        trace_log(PPPiface,x);
#define PPP_DEBUG_PACKET  0x0400
#define PPP_DEBUG_FSA    0x0800
#define PPP_DEBUG_LQR    0x4000
#define PPP_DEBUG_OPTIONS  0x8000
#define PPP_DEBUG_BOOLEAN(x)  if(PPPiface->trace & PPP_DEBUG_OPTIONS) \
        trace_log(PPPiface,x);
#define PPP_DEBUG_VALUE(x,y)  if(PPPiface->trace & PPP_DEBUG_OPTIONS) \
        trace_log(PPPiface,x,y);
#define FSM_LOG(x,y)    fsm_log(x,y)

#else

#define PPP_DEBUG_CHECKS(x)
#define PPP_DEBUG_ROUTINES(x)
#define PPP_DEBUG_BOOLEAN(x)
#define PPP_DEBUG_VALUE(x,y)
#define FSM_LOG(x,y)

#endif
#endif	/* FEATURE_SKT_DS */

/* config packet header */
struct config_hdr 
{
  uint8  code;

#define CONFIG_REQ      1
#define CONFIG_ACK      2
#define CONFIG_NAK      3
#define CONFIG_REJ      4
#define TERM_REQ        5
#define TERM_ACK        6
#define CODE_REJ        7
#define PROT_REJ        8
#define ECHO_REQ        9
#define ECHO_REPLY     10
#define DISCARD_REQ    11
#define IDENTIFICATION 12
#define TIME_REMAINING 13

  uint8  identifier;
  uint16 length;
};
typedef struct config_hdr ppp_config_hdr_type;

/* Length of config packet header */
#define CONFIG_HDR_LEN 4

/*---------------------------------------------------------------------------
  This is the max. number of bytes per PPP option that this implementation
  supports. This is used to declare byte arrays to store option values.
---------------------------------------------------------------------------*/
#define PPP_OPT_MAX_LEN 4 

/* config option header */
struct option_hdr 
{
  uint8  type;          /* protocol dependent types */
  uint8  length;
};
typedef struct option_hdr ppp_option_hdr_type;

/* Length of option header */
#define OPTION_HDR_LEN 2 

/* Supported Configuration Protocol index */
enum 
{
  Lcp,
  IPcp,
  fsmi_Size
};

struct fsm_s;          /* forward declaration */
struct fsm_side_s;     /* forward declaration */

/* Protocol Constants needed by State Machine */
struct fsm_constant_s
{
  char   *name;         /* Name of protocol */
  uint16 protocol;      /* Protocol number */
  byte   dsm_kind;      /* The DSM kind field for this protocol */
  uint16 recognize;     /* Codes to use (bits)*/
  char   **codes;       /* names of codes */

  uint8  fsmi;             /* Finite State Machine index */
  uint8  try_configure;    /* # tries for configure */
  uint8  try_nak;          /* # tries for nak substitutes */
  uint8  try_terminate;    /* # tries for terminate */
  uint16 timeout;          /* Time for timeouts (milliseconds)*/

  uint8  option_limit;     /* maximum option number */

  /* To free structure */

  void (*free)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  /* Lower Layer Down event */

  void (*down)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  /* This Layer Start: entering Starting */

  void (*starting)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  /* This Layer Up: entering Opened */

  void (*opening)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  /* This Layer Down: leaving Opened */

  void (*closing)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  /* This Layer Finish: entering Closed or Stopped */

  void (*stopping)
  (
    struct fsm_s *fsm_p   /* ptr to a Finite State Machine (FSM) structure */
  );

  void (*option)
  (
    struct fsm_s      *fsm_p,        /* ptr to a FSM structure */
    void              *vp,           /*   */
    struct dsm_item_s **bpp,         /*   */
    uint8             o_type         /*   */
  );

  int (*check)
  (
    struct fsm_s      *fsm_p,      /*  */
    struct option_hdr *ohp,        /*  */
    struct dsm_item_s **bpp,       /*  */
    int               request      /*  */
  );

  int (*reject)
  (
    struct fsm_s      *fsm_p,      /*   */
    struct option_hdr *ohp         /*   */
  );
};

/* FSM states */

enum
{
  fsmINITIAL,
  fsmSTARTING,
  fsmCLOSED,
  fsmSTOPPED,
  fsmCLOSING,
  fsmSTOPPING,
  fsmREQ_Sent,
  fsmACK_Rcvd,
  fsmACK_Sent,
  fsmOPENED,
  fsmState_Size
};

/*
 *  local.will:  Options to accept in a NAK from remote,
 *      which may be different from the desired value.
 *  local.want:  Options to request.
 *      Contains desired value.
 *      Only non-default options need to be negotiated.
 *      Initially, all are default.
 *  local.work:  Options currently being negotiated.
 *      Value is valid only when negotiate bit is set.
 *
 *  remote.will:  Options to accept in a REQ from remote,
 *      which may be different from the desired value.
 *  remote.want:  Options to suggest by NAK if not present in REQ.
 *      Contains desired value.
 *  remote.work:  Options currently being negotiated.
 *      Value is valid only when negotiate bit is set.
 */

typedef uint16 negotiate_t;

struct fsm_side_s
{
  negotiate_t will;
  negotiate_t want;
  void  *want_pdv;
  negotiate_t work;
  void  *work_pdv;
};
typedef struct fsm_side_s ppp_fsm_side_type;

/* State Machine Control Block */

struct fsm_s
{
  uint8  state;       /* FSM state */
  uint8  lastid;      /* ID of last REQ we sent */

  uint8  flags;

  uint8  retry_request;    /* counter for timeouts */
  uint8  try_configure;    /* # tries for configure */
  uint8  try_terminate;    /* # tries for terminate */

  uint8  retry_nak;    /* counter for naks of requests */
  uint8  try_nak;      /* # tries for nak substitutes */

//  struct timer timer;
  rex_sigs_type timer;         /* Rex signal which starts timer */
  struct fsm_constant_s *pdc;  /* protocol dependent constants */
  void *pdv;                   /* protocol dependent variables */

  struct fsm_side_s local;
  struct fsm_side_s remote;
};
typedef struct fsm_s ppp_fsm_type;

/* Link Phases */

enum
{
  pppDEAD,          /* Waiting for physical layer */
  pppESTABLISH,     /* Link Establishment Phase */
  pppAUTHENTICATE,  /* Authentication Phase */
  pppNETWORK,       /* Network Protocol Phase */
  pppTERMINATE,     /* Termination Phase */
  pppPhase_Size
};


/* PPP control block */

struct ppp_s
{
  uint8  phase;                /* phase of link initialization */
  uint8  id;                   /* id counter for connection */

  uint8  flags;
#define PPP_AP_LOCAL   0x10    /* waiting for local authentication */
#define PPP_AP_REMOTE  0x20    /* waiting for remote authentication */
#define PPP_AP_FAILED  0x40    /* authentication failed */

  struct fsm_s fsm[fsmi_Size]; /* finite state machines */

  uint32 upsince;              /* Timestamp when Link Opened */
  char   *peername;            /* Peername from remote (if any) */

  uint32 OutOpenFlag;          /* # of open flags sent */
  uint32 OutNCP[fsmi_Size];    /* # NCP packets sent by protocol */

  uint32 InGoodOctets;         /* # of octets in good packets */
  uint32 InOpenFlag;           /* # of open flags */
  uint32 InNCP[fsmi_Size];     /* # NCP packets by protocol */
  uint32 InUnknown;            /* # unknown packets received */
  uint32 InChecksum;           /* # packets with bad checksum */
  uint32 InFrame;              /* # packets with frame error */
};

#ifndef FEATURE_DS_NET_MODEL
extern struct ppp_s ppp_cb;
#endif /* FEATURE_DS_NET_MODEL */
extern struct ppp_s *ppp_cb_ptr;

extern char *fsmStates[];
extern char *fsmCodes[];

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

int ntohcnf
(
  struct config_hdr *cnf,
  struct dsm_item_s **bpp
);

void htonopt_copy
(
  struct dsm_item_s **bpp,
  struct option_hdr *opt,
  struct dsm_item_s **copy_bpp
);

int ntohopt
(
  struct option_hdr *opt,
  struct dsm_item_s **bpp
);

void fsm_log
(
  struct fsm_s *fsm_p,
  char *comment
);

void fsm_timer
(
  struct fsm_s *fsm_p
);

int fsm_send
(
  struct fsm_s      *fsm_p,
  uint8             code,
  uint8             id,
  struct dsm_item_s *bp
);

void fsm_proc
(
  struct fsm_s      *fsm_p,
  struct dsm_item_s *bp
);

boolean fsm_timeout
(
  struct fsm_s *fsm_p
);

void fsm_up
(
  struct fsm_s *fsm_p
);

void fsm_down
(
  struct fsm_s *fsm_p
);

void fsm_open
(
  struct fsm_s *fsm_p
);

void fsm_close
(
  struct fsm_s *fsm_p
);

void fsm_init
(
  struct fsm_s *fsm_p
);

void fsm_free
(
  struct fsm_s *fsm_p
);


#endif /* _PPPFSM_H */
