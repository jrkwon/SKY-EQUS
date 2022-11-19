#ifndef DIAGI_H 
#define DIAGI_H
/*==========================================================================

                  D I A G N O S T I C    T A S K   
    
               I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
  Local Header file for diag unit.

Copyright (c) 1990-1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/COMMON/vcs/diagi.h_v   1.4   06 Dec 2000 14:36:26   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/14/00   lad     Added support for FEATURE_RUNTIME_DEVMAP.
11/07/00   jal     Moved declaration of event_drain() here--it's internal-only
10/17/00   jal     Changes for new event accumulation mechanism
09/25/00   jal     Changes to support use of new DSM interface.
08/28/00   lad     Made changes for diagbuf optimization.
07/06/00   jal     Changed KICK_WATCHDOG to DIAG_KICK_WATCHDOG to avoid
                   conflict with other (FS) VUs
06/23/00   lad     Removed obsolete diag_sleep reference.
06/19/00   lad     Moved some macros and prototypes to diagbuf.h
05/15/00   lad     Moved LOG_NUM_BITS macro to log.h
03/07/00   lad     Fixed flow control for FEATURE_DIAG_NON_STREAMING.
02/28/00   lad     Added support for streaming diag services
02/02/00   lad     Change mode and security from word to byte to save ROM.
                   Added macro for EFS support.  Bitrate change support.
03/31/99   ms      Changed diag_wait return type to rex_sigs_type.
03/17/99   ms      Included prototype for diagp_seq_nums_not_equal.
                   This was earlier in bbjmpout.h, a file which was obsoleted
                   by Virtual Boot Block.
01/15/99   ak      Featurized use of NEW SIO & old sio.
01/12/99   ak      Changed in order to support new SIO architecture. Changes
                   include new function prototypes, and the typedef
                   diag_ahdlc_state_type.
11/04/98   lad     Made necessary changes for ARM port.
08/31/98   bns     Added _Cdecl calling convention where necessary.
05/17/96   jmk     Added extern for chk_security_code_ptr, which is the 
                   security password in the check block. Moved some extern
                   declarations here.
04/09/96   dna     Removed extern for proc_good_frames().
04/08/96   dna     Renamed to proc_good_frames() for consistency.
02/29/96   dna     Added extern for good_frames() which is in bbdiagp.c
02/28/96   dna     Made trivial changes for lint.
12/08/94   jmk     Added DIAG_SLEEP_ALLOW/FORBID() macros, and boolean 
                   diag sleep flag.
11/15/94   jmk     Added sequence number variables.
07/07/93   twp     Removed cast to (void) of memcpy in ERR_PKT macro
09/03/92   twp     Added structure to cache nv data in memory
07/07/92   twp     First release

===========================================================================*/


/*==========================================================================

                       INCLUDE FILES FOR MODULE

===========================================================================*/
#include  <string.h>
#include  "nv.h"
#include  "rxc.h"
#if MMI_TYPE != SKY_UI  //add_htg_00_xmas : To adapt for EQUS
#include  "hs.h"
#else
#include "sky.h"
#endif
#include  "rex.h"
#include  "target.h"
#include  "diagpkt.h"
#include  "diagbuf.h"
#include  "dsm.h"
#ifdef T_SLEEP
#include  "sleep.h"
#endif
#include  "sio.h"



/*==========================================================================

                       STRUCTURE DEFINITIONS FOR MODULE

===========================================================================*/
/* Structure to cache NV data in memory */
typedef struct {
  dword       esn;                              /* ESN */
  byte        current_nam;                      /* current nam index (0-3) */
  dword       min1[NV_MAX_NAMS][NV_MAX_MINS];   /* All the MIN1s */
  word        min2[NV_MAX_NAMS][NV_MAX_MINS];   /* All the MIN2s */
} diag_nv_cache_type;

/*---------------------------------------------------------------------------
  Definitions for Half-Duplex Async-HDLC mode.
---------------------------------------------------------------------------*/
#define DIAG_ASYNC_CONTROL_CHAR 0x7E /* Flag character value.  Corresponds 
                                        to the ~ character                  */
#define DIAG_ASYNC_ESC_CHAR     0x7D /* Escape sequence 1st character value */
#define DIAG_ASYNC_ESC_MASK     0x20 /* Escape sequence complement value    */
#define DIAG_MIN_PKT_SIZE       3    /* Command code and CRC                */
#define DIAG_ESC_EXPAND_SIZE    2
#define DIAG_ASYNC_TAIL_SIZE    3    /* CRC plus tail character             */
#define DIAG_FOOTER_SIZE        5    /* 2 byte CRC plus the possibility of 
                                        expanding per AHDLC protocol plus  
                                        the  ASYNC_FLAG                     */
typedef enum    /* A-HDLC RX states */
{
  DIAG_ASYNC_REGULAR_STATE,            /* regular tx/rx of bytes state          */
  DIAG_ASYNC_ESCAPE_STATE,             /* 2 byte escape in process              */
  DIAG_ASYNC_WAIT_FOR_FLAG_STATE       /* discard bytes until Flag byte arrives */
} diag_ahdlc_state_type;



/*==========================================================================

                       EXTERNALIZED DATA MEMBERS

===========================================================================*/

/*----------------------------------------------------------------------------
                     Handset Emulation commands.
----------------------------------------------------------------------------*/
#if MMI_TYPE != SKY_UI
extern hs_packets_type diag_hs_cmd;   /* Used to send commands to hs driver */
#else
extern HS_COMMAND_U    diag_hs_cmd;       /* Pointer to the current command */
#endif
/*----------------------------------------------------------------------------
  Flags to indicate a variety of conditions to the main Diag task.
----------------------------------------------------------------------------*/
extern boolean diag_dload;         /* Jump to Downloader signal             */
extern boolean diag_bad_spc;       /* Flag that a bad SPC was tried         */
extern boolean diag_bad_password;  /* Flag that a bad password was tried    */

/*----------------------------------------------------------------------------
  Masks to indicate the operating state of Diag.
----------------------------------------------------------------------------*/
extern byte diag_mode;          /* Mode of operation: online/offline   */
extern byte diag_security;      /* Security of the phone: locked/unlocked */
#ifdef	MODEL_SKTT
extern byte esn_security;       /* taegon 00.03.27, protect esn           */
#endif	// #ifdef	MODEL_SKTT

/*----------------------------------------------------------------------------
  Buffers for NV Commands and Responses.
----------------------------------------------------------------------------*/
extern nv_cmd_type  ncmd;         /* Command buffer for NV interface  */
extern nv_item_type nitem;        /* NV Item buffer for NV interface  */

/*--------------------------------------------------------------------------
  Regional (to diag) buffers for nv information that is read at initializa-
  tion from NV.
--------------------------------------------------------------------------*/
extern diag_nv_cache_type diagnv;  /* Cached versions of NV data   */

/*----------------------------------------------------------------------------
  Declaration of the 8 bytes in the check block which is the security password
----------------------------------------------------------------------------*/
extern byte const chk_security_code_ptr[];

/*----------------------------------------------------------------------------
  Declaration of the constant which defines an un-set security password.
  (The value is all FF's)
----------------------------------------------------------------------------*/
extern byte const diag_no_security_code_c[];

/*----------------------------------------------------------------------------
  Boolean indicating whether or not a command to give up SIO has been received
----------------------------------------------------------------------------*/
#ifndef FEATURE_RUNTIME_DEVMAP
extern boolean            diag_close_flag;
#endif
extern sio_bitrate_type   diag_new_bitrate;
extern sio_stream_id_type diag_sio_stream_id;

extern byte diagbuf_buf[]; /* Circular output buffer */

extern volatile int diagbuf_head; /* Head of circular buffer */
extern int diagbuf_tail; /* Tail of circular buffer */

/*------------------------------------------------------
  Special flags to tell DIAG when to wrap or skip data.
  Since holes (after a shorten) could be 1 byte, the 
  length field after a hole is 1 byte, not 2.
------------------------------------------------------*/
#define DIAGBUF_COMMIT_FLAG   0x8000 /* b 1000 0000 0000 0000 */
#define DIAGBUF_HOLE_FLAG     0x4000 /* b 0100 0000 0000 0000 */
#define DIAGBUF_HOLE_1B_FLAG  0x2000 /* b 0010 0000 0000 0000 */

/* The wrap flag must not contain the "hole" flag bit. */
#define DIAGBUF_WRAP_FLAG     0xBFFF /* Flag to wrap buffer */

/*----------------------------------------------------------------------------
  Flag to tell diagbuf_drain()/event_drain() whether SIO is flow-controlled
----------------------------------------------------------------------------*/
extern boolean diag_OKTX;

#ifndef FEATURE_DIAG_NO_EVENTS
extern rex_timer_type event_stale_timer;

/*===========================================================================

FUNCTION EVENT_RETIRE

DESCRIPTION
  Retire (commit) the existing event reporting buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The event report buffer will be commited and global pointers reset.

===========================================================================*/
extern void event_retire(void);
#endif

/*===========================================================================

FUNCTION DIAG_START_FLOW
FUNCTION DIAG_STOP_FLOW

DESCRIPTION
  Flow control function pointers for queueing data to SIO. 
  Starts and stops streaming.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  DIAG will stop/start streaming as a result of these functions.
  
===========================================================================*/
extern void diag_stop_flow(dsm_mempool_id_enum_type);
extern void diag_start_flow(dsm_mempool_id_enum_type);

#ifdef FEATURE_DIAG_NON_STREAMING
/*===========================================================================

FUNCTION DIAG_SEND_TX_PACKET

DESCRIPTION
  This routine will call the necessary SIO routines to TX the response
  packet.  Outgoing packet will be escaped and broken into little
  (transmittable) pieces.
  
DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE if tx was successful, else returns FALSE.

SIDE EFFECTS
  None.
 
===========================================================================*/
extern boolean diag_send_tx_packet(
  byte *source_ptr,              /* Unescaped response packet which will
                                    be escaped and transmitted in pieces.  */
  word  temp_pkt_len             /* Size of unescaped packet.              */
);


/* Function hook to send diag pkt */
void diagbuf_commit_static_pkt (void);
#endif


/*===========================================================================

MACRO DIAGBUF_EMPTY
MACRO DIAGBUF_FULL

DESCRIPTION
  Conditional macros for DIAG streaming buffer.

PARAMETERS
  None.
  
DEPENDENCIES
  None.

RETURN VALUE
  Conditional values.

SIDE EFFECTS
  None.

===========================================================================*/
#define DIAGBUF_EMPTY(h, t) (h == t)

#define DIAGBUF_FULL(h, t) ((h >= t) ? h - t : 2 * DIAGBUF_SIZE + h - t)

/*===========================================================================

MACRO DIAGBUF_USED
MACRO DIAGBUF_FREE

DESCRIPTION
  Status calculating macros for the DIAG ring buffer.

PARAMETERS
  h - head index
  t - tail index

DEPENDENCIES
  None.

RETURN VALUE
  Number of bytes.

SIDE EFFECTS
  None.

===========================================================================*/
#define DIAGBUF_USED(h, t) ((h >= t) ? (h - (t)) : (h - (t) + 2*DIAGBUF_SIZE))

#define DIAGBUF_FREE(h, t) (DIAGBUF_SIZE - DIAGBUF_USED(h, t))


/*===========================================================================

MACRO DIAGBUF_CALC_IDX

DESCRIPTION
  Calculates index in DIag output buffer.

PARAMETERS
  Abstract buffer index 

DEPENDENCIES
  0 <= x < 2 * DIAGBUF_SIZE

RETURN VALUE
  Index in diag ring buffer.

SIDE EFFECTS
  None.

===========================================================================*/
#define DIAGBUF_CALC_IDX(x) ((x >= DIAGBUF_SIZE) ? (x - DIAGBUF_SIZE) : x)

/*===========================================================================

MACRO DIAGBUF_WRAP

DESCRIPTION
  Wraps abstract buffer index.

PARAMETERS
  Abstract buffer index.

DEPENDENCIES
  0 <= x < 2 * DIAGBUF_SIZE

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
#define DIAGBUF_WRAP(x)                    \
do {                                       \
  x += DIAGBUF_SIZE - DIAGBUF_CALC_IDX(x); \
  if (x >= 2 * DIAGBUF_SIZE)               \
    x -= 2 * DIAGBUF_SIZE;                 \
/*lint -e717 */                            \
} while(0)                                 \
/*lint +e717 */

#define DIAGBUF_MOVE(i, l)   \
do {                         \
  i += l;                    \
  if (i >= 2 * DIAGBUF_SIZE) \
    i -= 2 * DIAGBUF_SIZE;   \
/*lint -e717 */              \
} while(0)                   \
/*lint +e717 */

/*===========================================================================

MACRO DIAG_SLEEP_ALLOW

DESCRIPTION
  Tell the sleep task that we are in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The sleep task may take us up on our offer.

===========================================================================*/
#ifdef T_SLEEP
#define DIAG_SLEEP_ALLOW() \
  (void) rex_set_sigs( &sleep_tcb, SLEEP_DIAG_OKTS_SIG )
#else
#define DIAG_SLEEP_ALLOW()
#endif


/*===========================================================================

MACRO DIAG_SLEEP_FORBID

DESCRIPTION
  Tell the sleep task that we are NOT in a state where we can sleep.

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef T_SLEEP
#define DIAG_SLEEP_FORBID() \
  (void) rex_clr_sigs( &sleep_tcb, SLEEP_DIAG_OKTS_SIG )
#else
#define DIAG_SLEEP_FORBID()
#endif



/*===========================================================================

                         FUNCTION PROTOTYPES

===========================================================================*/

extern rex_sigs_type diag_wait
( 
  rex_sigs_type mask       /* Mask of signals to wait for */ 
);

nv_stat_enum_type diag_nv_read ( 
  nv_items_enum_type     item,              /* Which NV item to read */
  nv_item_type          *buf_ptr            /* Where to put the data */
);

#ifndef FEATURE_DIAG_NO_EVENTS

extern void event_stale(void);

extern diagbuf_status_enum_type event_drain (
  dsm_item_type **tx_item_ptr,    /* Current DSM item to enqueue to */
  boolean       *needControlFlag  /* Does next DSM item need ctrl flag? */
);
#endif

#define DISPATCH_DECLARE(func) \
extern diag_rsp_type * func(diag_req_type *req_pkt, word len)

#define DIAG_KICK_WATCHDOG()                             \
  do                                                     \
  {                                                      \
    dog_report(DOG_DIAG_RPT);                            \
    rex_set_timer( &diag_rpt_timer, DOG_DIAG_RPT_TIME ); \
    /*lint -e717 */                                      \
  } while(0) /*lint +e717 */

#ifdef FEATURE_EFS
#include "diagpkt.h"

#define DIAG_FS_REQ_LEN_DIFF FPOS(diag_fs_op_req_type, fs_req)
#endif

#endif /* DIAGI_H */

