#ifndef TXCACCESS_H
#define TXCACCESS_H
/*===========================================================================
                 
            C D M A   T R A N S M I T   A C C E S S   M O D U L E

DESCRIPTION
  This header file contains definitions for the transmit access module that 
  are used by the txc unit.  This header file is internal to the txc unit.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/txcaccess.h_v   1.0   10 Nov 2000 16:48:20   ddalke  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/00   jc      Created after repartition of txc.c

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

typedef struct {
  boolean dotx;                   /* for access channel - transmit? */
  byte    data[24];               /* up to 24 bytes of info */
} txc_ac_frame_type;

/* possible states in access probe sequence */
typedef enum {
  ACC_SETUP1_S,                   /* set up for probe */
  ACC_BACK1_S,                    /* initial backoff */
  ACC_PRE_S,                      /* transmitting preamble */
  ACC_MSG_S,                      /* transmitting message */
  ACC_SETUP2_S,                   /* set up for probe */
  ACC_BACK2_S,                    /* probe backoff randomization */
  ACC_ENDPROBE_S,                 /* ACC_TMO at end of sequence */
  ACC_MSG_DONE_S                  /* done with message */
#ifdef FEATURE_IS95B
  ,ACC_SUSPEND_S                  /* Suspend the probe transmission */
#endif
}txc_acc_state_type;

#if defined(FEATURE_DIAG_V7_LOG_IFACE)
  typedef struct
  {
    log_desc_type desc;             /* log descriptor for logging services */
    log_acc_info_type acc;          /* Access channel information */
  }txc_log_acc_type;
#endif

/* Specific variables for the Access Channel */
typedef struct {
  txc_acc_state_type state;        /* Current state */
  word          preambles;         /* Number of access channel preambles */
  word          slotting;          /* Access Channel slot size */
  word          countdown;         /* Timeout value (in units of frames) */
  boolean       req;               /* Access request? */
  byte          seq_num;           /* Current access probe sequence number */
  byte          max_seq;           /* Max number of access probe sequences */
  byte          power;             /* Power to tx at */
  byte          init_pwr;          /* Initial power to tx at */
  byte          last_pwr;          /* Last power transmitted at */

#ifdef FEATURE_IS95B_OLPEC
  byte          is95a_pwr;         /* xmit power for IS-95A mobile */
  byte          int_corr;          /* IS-95B interference correction */
#endif /* FEATURE_IS95B_OLPEC */

  byte          pstep;             /* Power step */
  byte          nstep;             /* Number of probes left in sequence */
  byte          num_step;          /* Total number of access probes */
  byte          bkoff;             /* BKOFF */
  byte          probe_bkoff;       /* PROBE_BKOFF */
  byte          acc_tmo;           /* Acknowlegement timeout */
  byte          acc_chan;          /* Number of access channels */
  boolean       use_chan;          /* Use specific channel */
  byte          ra;                /* Randomly selected access channel */
  byte          pagech;            /* Current Paging Channel */
  word          base_id;           /* Base station identification */
  word          pilot_pn;          /* Pilot PN sequence offset index */
  word          rn;                /* Hashed number for pn offset */
  dword         p;                 /* Random number for persistence check */
  txc_ac_frame_type frame;         /* Current access frame to send */
  word          sigs;              /* Signal to set when access attempt is
                                      done */
  rex_tcb_type  *tcb_ptr;          /* Pointer to task to signal when access
                                      attempt is done */
  byte          probe_count;       /* Access probe number */
  boolean       first_frame;       /* First frame of message? */
#ifdef FEATURE_IS95B
  boolean       stop_probes;       /* Flag to temporarily stop/start access
                                   ** probes due to T72 Access Fade Timer */
#endif
#ifdef FEATURE_DIAG_V7_LOG_IFACE
  txc_log_acc_type *log_ptr;       /* Pointer to access probe log type */
#endif
} txac_type;

#define NO_OF_ISRS_AFTER_FO 6
extern byte after_fo_handoff_count_isrs;

extern txac_type txac;

#if !defined(FEATURE_DIAG_V7_LOG_IFACE)
  extern log_acc_info_type acc_log;
#else
  #define                                 TXC_NUM_ACC_INFO_BUFS 4
  extern txc_log_acc_type txc_log_acc_info_pool[ TXC_NUM_ACC_INFO_BUFS ];
  extern q_type txc_log_acc_q;              /* logging access probe information */
#endif /*FEATURE_DIAG_V7_LOG_IFACE*/

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TXC_ACC_PROBE_RESUME

DESCRIPTION
  ReSumes the transmission of next Access probe.

DEPENDENCIES
  When this function is called, it is assumed that the TXC is in
  the Access Probe Suspend State. If it is not, then the function
  performs a FATAL ERROR.

RETURN VALUE
  None

SIDE EFFECTS
  The Sequence Number etc is set as per the Access Probe Handoff Requirements.
===========================================================================*/
#ifdef FEATURE_IS95B
  extern void txc_acc_probe_resume(void);
#endif /* FEATURE_IS95B */

/*===========================================================================

FUNCTION TXC_ACCESS_INIT

DESCRIPTION
  Initialization section for access channel.

DEPENDENCIES
  None
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/
extern void txc_access_init( txc_state_type state );

/*===========================================================================

FUNCTION TXC_ACCESS_ATT

DESCRIPTION
  State where access channel transmission is transmitted.

DEPENDENCIES
  None

RETURN VALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/

extern txc_state_type txc_access_att
(
  txc_state_type state
);

/*===========================================================================

FUNCTION TXC_HYPERSPACE

DESCRIPTION
  Performs jump to hyperspace for Transmit task which has not been able to
  jump since transmit clock has been turned off for power saving.

DEPENDENCIES
  Depends on the access channel mask to be used being placed in
  txc_tx.buf.mask.
RETURN VALUE
  None.
SIDE EFFECTS
  None
===========================================================================*/

extern txc_state_type txc_hyperspace
(
  txc_state_type state                /* txc_state - TXC_ACC_JUMP_S */
);

/*===========================================================================

FUNCTION TXC_ACCESS_IDL

DESCRIPTION
  Access channel idle state.  Waits for command to transmit an access
  attempt or leave the access state.

DEPENDENCIES
  None

RETURNVALUE
  New state

SIDE EFFECTS
  None

===========================================================================*/
extern txc_state_type txc_access_idl
(
  txc_state_type state
);

#endif /*TXCACCESS_H*/
