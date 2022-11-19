#ifndef ACPFM_H
#define ACPFM_H
/*===========================================================================

                          ACP FM Interface

DESCRIPTION
   This header file contains the definitions necessary to use
   the FM Interface from higher layers.

Copyright (c) 1991,1991,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpfm.h_v   1.0   Aug 11 2000 11:40:36   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/02/96   jca     Changed acpfm_fade_timing_status() function so that
                   it returns more specific SAT detection status.
05/12/95   jca     Added Gemini (MSM2P) target support.
08/09/94   jca     Deleted function acpfm_word_sync().
11/17/92   ARH     Addded acpfm_tx_recc_msg_loop() for diagnostics [IS-55]
10/05/92   ARH     Updated comments. Made get_word_sync_count return a word
09/02/92   ARH     Added debug/IS-55 calls for returning counts
07/23/92   ARH     changed name to acpfm_fade_timing_stat_enabled()
12/17/91   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "acp553.h"
#include "acpcmdef.h"
#include "rex.h"
#include "target.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* Return status from acpfm_rxword() */
typedef enum
{
  ACPFM_RXWORD_NO_WORD,
  ACPFM_RXWORD_NEW_WORD,
  ACPFM_RXWORD_BCH_ERROR
} acpfm_rxword_status_type;

/* Return status from acpfm_txmsg() */
typedef enum
{
  ACPFM_TXMSG_FAILED,
  ACPFM_TXMSG_PENDING,
  ACPFM_TXMSG_PASSED
} acpfm_txmsg_status_type;

/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPFM_RXWORD

DESCRIPTION
  Returns a newly decoded 40 bit word from the forward channel (if available).
  The word is placed in the address indicated by the pointer argument only
  when ACPFM_RXWORD_NEW_WORD is returned.

DEPENDENCIES
  In order for forward words to be received properly the stream and mode
  must be set appropiately beforehand.  See acpfm_stream() and acpfm_mode().

RETURN VALUE
  acpfm_rxword_status_type:
         ACPFM_RXWORD_NO_WORD    No word available.
         ACPFM_RXWORD_NEW_WORD   A new word has been placed in the address
                                 indicated
         ACPFM_RXWORD_BCH_ERROR  A word was received but it contained BCH
                                 errors

SIDE EFFECTS
  None

===========================================================================*/
extern acpfm_rxword_status_type acpfm_rxword
(
  acp553_rxword_type word_buff_ptr
    /* Address to place new rx word */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_GET_RXWORD_COUNT

DESCRIPTION
  Returns the number of words that have been decoded by ACPFM and are waiting
  to be retrieved by a higher level task.

DEPENDENCIES
  None

RETURN VALUE
  word  Indicates the number of words available. If no words are waiting to
        be retrieved a 0 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern word acpfm_get_rxword_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_TX_RECC_MSG

DESCRIPTION
  Sends a request to transmit a reverse message to the base station.
  The input message must be built as a sequence of 36 bit words before
  calling this procedure (see IS-95 section 2.7). A pointer to the
  message is given as an argument to the call.  This routine will
  take care of adding the seizure precursor and word repeats.

  The arguments are used as follows:
    bi_monitor: If monitor of the busy/idle status is required this
    argument should be set to TRUE.

    dcc: Indicates the current received Digital Color Code in order to
    build the seizure precursor (See IS-95 Table 2.7.1-1).

DEPENDENCIES
  The channel mode (control) must be set accordingly beforehand.
  The power amplifier should be on and stable on the desired power level.
  The procedure acpfm_tx_init() should be called beforehand to initialize
  the signal to set when transmission completes (acpfm_tx_init() needs to
  be called only once. It is not necessary to call it before each
  transmission).

RETURN VALUE
  None through this procedure call. A signal is set whenever the request
  for transmission has been completed.

  See acpfm_get_txmsg_status() for return status information

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_tx_recc_msg
(
  acp553_txmsg_type *msg_buff_ptr,      /* Address of msg to be tx'ed */
  byte              dcc,                /* Digital color code         */
  byte              num_of_words,       /* Number of acp553_txwords   */
  boolean           bi_monitor          /* Monitor busy/idle flag     */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_TX_RECC_MSG_LOOP

DESCRIPTION
  Sends a request to transmit continuous reverse control channel messages
  to the base station.  The input message must be built as a sequence of
  48 bit words before calling this procedure.  A pointer to the message
  is given as an argument to the call.  This routine will take care of
  adding the seizure precursor and word repeats.  The message will be
  continously transmitted (loop mode). The transmission can be stopped
  by calling acpfm_stop_recc_msg_loop() or acpfm_tx_shutdown().

DEPENDENCIES
  The channel mode (control) must be set accordingly beforehand.  The
  power amplifier should be on and stable on the desired power level.
  The procedure acpfm_tx_init() should be called beforehand to initialize
  the signal to set when transmission completes. acpfm_tx_init() needs to
  be called only once.  It is not necessary to call it before each
  transmission.

RETURN VALUE
  None through this procedure call.  A signal is set whenever the request
  for transmission has been completed.

  See acpfm_get_txmsg_status() for return status information.

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_tx_recc_msg_loop
(
  byte              *msg_buff_ptr,      /* Address of msg to be tx'ed */
  byte              dcc,                /* Digital color code         */
  byte              num_of_words        /* Number of acp553_txwords   */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_TX_RVC_MSG

DESCRIPTION
  Sends a request to transmit a reverse message to the base station.
  The input message must be built as a sequence of 36 bit words before
  calling this procedure (see IS-95 section 2.7). A pointer to the
  to the message is given as an argument to the call.   This routine
  will take care of adding the seizure precursor and word repeats.

DEPENDENCIES
  The channel mode (voice) must be set accordingly beforehand.  The power
  amplifier should be on and stable on the desired power level.  The
  procedure acpfm_tx_init() should be called beforehand to initialize
  the signal to set when transmission completes.  acpfm_tx_init() needs
  to be called only once.  It is not necessary to call it before each
  transmission.

RETURN VALUE
  None through this procedure call.  A signal is set whenever the request
  for trasmission has been completed.

  See acpfm_get_txmsg_status() for return status information.

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_tx_rvc_msg
(
  acp553_txmsg_type *msg_buff_ptr,       /* Address of msg to be tx'ed */
  byte              num_of_words         /* Number of acp553_txwords   */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_GET_TXMSG_STATUS

DESCRIPTION
  Returns the status of the transmission requested by acpfm_tx_recc_msg()
  or acpfm_tx_rvc_msg().

DEPENDENCIES
  This function should be called after the transmit completion signal
  has been set.

RETURN VALUE
  acpfm_txword_status_type:
         ACPFM_TXMSG_FAILED   Message transmission failed
         ACPFM_TXMSG_PASSED   Message transmitted successfully

SIDE EFFECTS
  None

===========================================================================*/
extern acpfm_txmsg_status_type acpfm_get_txmsg_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_ST

DESCRIPTION
  Switches Signaling Tone transmission on or off.

DEPENDENCIES
  Only meaningful when in a voice channel.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_ST
(
  boolean st_state     /* TRUE -> Turns ST ON, FALSE -> Turns ST OFF */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_SAT

DESCRIPTION
  Switches Supervisory Audio Tone (SAT) transponding on or off.

DEPENDENCIES
  Only meaningful when in a voice channel.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_SAT
(
  boolean st_state   /* TRUE -> Turns SAT ON, FALSE -> Turns SAT OFF */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_SET_SCC

DESCRIPTION
  Sets the SAT color code in order to perform SAT validation.  Used in
  conjunction with acpfm_fade_timing_status().

DEPENDENCIES
  Only meaningful when in a voice channel.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_set_SCC
(
  acp553_scc_type scc        /* SAT Color Code */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_FADE_TIMING_STATUS

DESCRIPTION
  Retrieves the current fade timing status (see IS-95 Sections 2.4.1
  and 2.4.1.3).

DEPENDENCIES
  Only meaningful when in a voice channel.  acpfm_set_SCC must be called
  beforehand to set the Sat Color Code to match.

RETURN VALUE
  ACP553_VALID_SAT   - SAT matches SCCr (expected SAT)
  ACP553_INVALID_SAT - Valid SAT detected but does not match expected SAT
  ACP553_NO_SAT      - No valid SAT detected

SIDE EFFECTS
  None

===========================================================================*/
extern acp553_rsat_status_type acpfm_fade_timing_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_SET_MODE

DESCRIPTION
  Sets the FM operation to 'Control Channel Mode' (ACPFM_CCHAN_MODE) or
  'Voice Channel Mode' (ACPFM_VCHAN_MODE).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
   Queued words and/or words being received are discarded.

===========================================================================*/
extern void acpfm_set_mode
(
  acp553_mode_type mode             /* Channel Mode */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_BUSY_IDLE_STATUS

DESCRIPTION
  Reports the current value of the busy/idle status as received in the
  forward control channel.

DEPENDENCIES
  Results only meaningful when in a control channel.

RETURN VALUE
  boolean   TRUE   Busy/idle is busy
            FALSE  Busy/idle is idle

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpfm_busy_idle_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_SET_STREAM

DESCRIPTION
  Sets the forward control channel stream to listen to: ACP553_STREAM_A
  (for mobiles with LSB of MIN equal to '0') or ACP553_STREAM_B (for
  mobiles with LSB of MIN equal to '1')

DEPENDENCIES
  Only meaningful when in a control channel.

RETURN VALUE
  None

SIDE EFFECTS
   Queued words and/or words being received are discarded

===========================================================================*/
extern void acpfm_set_stream
(
  dword min1
    /* MIN1 value of mobile */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_GET_UNCORR_WORD_COUNT

DESCRIPTION
  Returns the number of uncorrectable words received since the last call
  to acpfm_reset_word_counts().  This procedure can be used to retrieve
  the information required by the IS-55 RCVS1 command.  The counts are
  maintained in both control or voice channel mode.

DEPENDENCIES
  None

RETURN VALUE
  byte - Number of uncorrectable words
  If the count exceeds 255, then 255 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acpfm_get_uncorr_word_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_GET_CORR_WORD_COUNT

DESCRIPTION
  Returns the number of correctable words received since the last call
  to acpfm_reset_word_counts().  This procedure can be used to retrieve
  the information required by the IS-55 RCVS1 command.  The counts are
  maintained in both control or voice channel mode.

DEPENDENCIES
  None

RETURN VALUE
  byte - Number of correctable words
  If the count exceeds 255, then 255 is returned.

SIDE EFFECTS
  None

==========================================================================*/
extern byte acpfm_get_corr_word_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_GET_WORD_SYNC_COUNT

DESCRIPTION
  Returns the number of word sync sequences received since the last call
  to acpfm_reset_word_counts().  This procedure can be used to retrieve
  the information required by the IS-55 WSTS command.  The counts are
  maintained only in control channel mode.

DEPENDENCIES
  Only meaningful when in a control channel.

RETURN VALUE
  word - Number of word sync sequences detected
  If the count exceeds 65535, then 65535 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern word acpfm_get_word_sync_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_RESET_WORD_COUNTS

DESCRIPTION
  Resets the number of correctable, uncorrectable and word sync counters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_reset_word_counts( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_RX_INIT

DESCRIPTION
  Initializes the receive section of the ACPFM interface unit.  The
  parameters indicate the task and signal to set every time a new
  word is received.  The acpfm_rxword() procedure should be called to
  retrieve the word.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_rx_init
(
  rex_tcb_type  *tcb_ptr, /* Task to signal when RX words are available */
  acp_mask_type signal    /* Signal to set when RX words are available  */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_TX_INIT

DESCRIPTION
  Initializes the transmit section of the ACPFM interface unit.  The
  parameters indicate the task and signal to set whenever a request to
  send a message has been completed (either successfully or unsuccessfully).
  The request to send a message is made by the acpfm_txmsg() call.  The
  acpfm_get_txmsg_status() procedure should be called to retrieve the
  status of the transmission request.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_tx_init
(
  rex_tcb_type  *tcb_ptr,  /* Task to signal when a msg has been TX'ed */
  acp_mask_type signal     /* Signal to set when a msg has been TX'ed  */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_TX_SHUTDOWN

DESCRIPTION
  Shuts down the transmit section of the ACPFM interface unit
  (eg. De-installs ISRs, etc.)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_tx_shutdown( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_RX_SHUTDOWN

DESCRIPTION
  Shuts down the receive section of the ACPFM interface unit
  (eg. De-installs ISRs, etc.)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_rx_shutdown( void );

#ifdef T_MSM2P
/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_ENABLE_RXMSG_FILTER

DESCRIPTION
  Enables or disables FOCC message prefilter that saves processing power
  by only processing overhead messages and mobile station control messages
  that have an address matching our mobile station.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_enable_rxmsg_filter
(
  boolean filter_enabled
    /* Enable or disable the FOCC message prefilter */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPFM_RESET_DFM_TXCTRL

DESCRIPTION
  Resets the MSM2P Digital FM Wideband Data Transmit circuitry.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpfm_reset_dfm_txctrl( void );
#endif /* T_MSM2P */

#endif /* ACPFM_H */

