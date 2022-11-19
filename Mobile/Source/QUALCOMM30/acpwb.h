#ifndef ACPWB_H
#define ACPWB_H
/*===========================================================================

                      ACP Wideband Interface

DESCRIPTION
   This header file contains the external definitions for proceudres called 
   from the ACPFM module and the SIO module. 

Copyright (c) 1991,1991,1993 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1994,1995,1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1997,1998,1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/acpwb.h_v   1.2   02 Feb 2000 14:24:08   hkulkarn  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/00   hrk     Merged following changes from GV40401:
           ro      Added acpwb_txfm_enable.
           sm      Merged MSM3100's FEATURE_VOC_SAT_CONTROL.
                   Added acpwb_fm_slotting_lock().
08/10/98   sk      Replaced T_ACP with FEATURE_ACP. DFM reset interface is not 
                   featurized.
06/19/98   sk      Added support to do DFM reset only if not transmitting WBD
                   data.
09/04/96   jca     Changes to support MSM FM access performance workarounds.
06/27/96   jca     Changes to support 10 kHz AMPS frequency drift bug.
01/02/96   jca     Changed acpwb_fade_timing_status() function so that
                   it returns more specific SAT detection status.
05/12/95   jca     Added Gemini (MSM2P) target support.
08/09/94   jca     Deleted function acpwb_word_sync()
02/16/92   ARH     Updated comments
09/15/92   ARH     Added comments to acpwb_set_stream and acpwb_cc_mode
09/02/92   ARH     Added acpwb_reset_word_sync_count() and 
                   acpwb_get_word_sync_count()
06/16/92   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "comdef.h"
#include "rex.h"
#include "acpcmdef.h"
#include "acp553.h"
#include "clk.h"
#ifdef FEATURE_VOC_SAT_CONTROL
#include "voc.h"
#endif /* FEATURE_VOC_SAT_CONTROL */

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

typedef enum {           /* Status from acpwb_get_txmsg_status() */
  ACPWB_TXMSG_FAILED,
  ACPWB_TXMSG_PENDING,
  ACPWB_TXMSG_PASSED
} acpwb_txmsg_status_type;

typedef enum {            /* SAT Color Codes               */
  ACPWB_SCC_00,           /* See EIA/TIA 553 Table 3.7.1-2 */
  ACPWB_SCC_01,
  ACPWB_SCC_10,
  ACPWB_SCC_INVALID
} acpwb_scc_type;

typedef enum {           /* Message transmit modes */
  ACPWB_TX_MONITOR_BI,   
  ACPWB_TX_DONT_MONITOR_BI,
  ACPWB_TX_LOOP_BUFFER,
  ACPWB_TX_ST
} acpwb_tx_mode_type;

#if (defined( FEATURE_ACP ) && defined( T_MSM2P))
extern clk_cb_type acpwb_dfm_mode_cb;
  /* Holds registration handle returned by Clock Services clk_reg function */
#endif

  /* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPWB_TXFM_ENABLE

DESCRIPTION
  Set audio and SAT muting for DFM transmit enable and disable
 
DEPENDENCIES
  acpwb_sat_on_flag

RETURN VALUE
  None

SIDE EFFECTS
  acpwb_txfm_enable_flag

===========================================================================*/
void acpwb_txfm_enable(boolean enable);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_DFM_RESET

DESCRIPTION
  Reset DFM if possible, else return the minimum wait time before another 
  attempt can be made to reset the DFM.

DEPENDENCIES
  None

RETURN VALUE
  Wait time in miliseconds. A return value of zero indicates that DFM reset 
  was done successfully.

SIDE EFFECTS
  None
===========================================================================*/
extern word acpwb_dfm_reset(void);

/*===========================================================================

FUNCTION ACPWB_INIT

DESCRIPTION
  Initializes the channel for the serial interface to the Sony Chipset.
  Must be called once after powerup.

  Mobile:    Channel B of the Serial Communications Controler (Z85C30)
  Portable:  FM Data Port of the MSM
  
DEPENDENCIES
  Mobile: The Z85C30 must be in an 'after reset' state in channel B (i.e. no 
          interrupts enabled, etc.)

  Portable: MSM initialized

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_TX_ISR

DESCRIPTION
  Wideband Data TRANSMIT Interrupt Service Routine This procedure should
  be called whenever a transmit interrupt has been received and it's
  determined that it should be processed, ie. Interrupt is Pending.

DEPENDENCIES
  acpwb_init should be called upon powerup to initialize the interface

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_tx_isr( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RX_ISR

DESCRIPTION
  Wideband Data RECEIVE Interrupt Service Routine. This procedure should
  be called whenever a receive interrupt has been received and it's
  determined that it should be processed, ie. Interrupt is Pending.
  
DEPENDENCIES
  acpwb_init should be called upon powerup to initialize the interface

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_rx_isr( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RX_INIT

DESCRIPTION
  Initializes the receive section of the ACPWB interface unit.
  The parameters indicate the task and signal to set every time a 
  new word is received. The new word is held in an internal queue. 
  The acpfm_rxword() procedure should be called to retrieve the word.

  This procedure must be called every time analog mode of operation is
  started.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_rx_init
(
  rex_tcb_type  *tcb_ptr, /* Task to signal when RX words are available */
  acp_mask_type signal    /* Signal to set when RX words are available  */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_TX_INIT

DESCRIPTION
  Initializes the transmit section of the ACPWB interface unit.  The
  parameters indicate the task and signal to set whenever a request
  to send a message has been completed (either successful or unsuccessful)
  The request to send a message is made by the acpfm_txmsg() call.
  The acpfm_get_txmsg_status() procedure should be called to retrieve the
  status of the transmission request.

  This procedure must be called every time analog mode of operation is
  started.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_tx_init
(
  rex_tcb_type  *tcb_ptr,  /* Task to signal when a msg has been TX'ed */
  acp_mask_type signal     /* Signal to set when a msg has been TX'ed  */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_TX_SHUTDOWN

DESCRIPTION
  Shuts down the transmit section of the ACPWB interface unit

  Must be called every time we leave analog mode of operation
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_tx_shutdown( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RX_SHUTDOWN

DESCRIPTION
  Shuts down the receive section of the ACPFM interface unit

  Must be called every time we leave analog mode of operation

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_rx_shutdown( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_GET_TXMSG_STATUS

DESCRIPTION
  Returns the status of the transmission.

DEPENDENCIES
  This function should be called after the transmit completion signal
  has been set.

RETURN VALUE
  acpwb_txword_status_type:   
         ACPWB_TXMSG_FAILED   Message transmission failed
         ACPWB_TXMSG_PASSED   Message transmitted successfully
  
SIDE EFFECTS
  None

===========================================================================*/
extern acpwb_txmsg_status_type acpwb_get_txmsg_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_TX_MSG

DESCRIPTION
  Enable TX to start sending a wideband data msg. The arguments provide the
  parameters for the type of transmission.  The message pointer should point
  to a buffer prepared for transmision, ie. buffer has all repeats and BCH 
  encoding. The number of bytes variable contains the message length
  in bytes.

  Modes: Three modes of transmission are defined:
     ACPWB_TX_MONITOR_BI:      Tx ISR will monitor B/I status for correct 
                               transition

     ACPWB_TX_DONT_MONITOR_BI: Tx ISR will send the data without monitoring
                               busy idle.

     ACPWB_TX_LOOP_BUFFER:     Tx ISR will send the data indicated in the
                               msg buffer continously until TX interrupts
                               are disabled.

     ACPWB_TX_ST:              Tx ISR will send signaling tone 

DEPENDENCIES
  None

RETURN VALUE
  None. The task indicated is signalled with the appropiate signal by the ISR
  at the end of the transmission (except in ACPWB_TX_LOOP_BUFFER and 
  ACPWB_TX_ST mode).

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_tx_msg
(
  byte *tx_msg_ptr,            /* Pointer to the msg to be transmitted */
  word number_of_bytes,        /* Number of bytes in the msg           */
  acpwb_tx_mode_type tx_mode   /* Monitor B/I, Don't, or loop          */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_TX_STOP

DESCRIPTION
 Stops transmission of any ongoing msg. Used mainly to stop tx loops
 (e.g. signalling tone, fixed data patterns, etc.)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_tx_stop( void );

/* <EJECT> */
#ifdef FEATURE_VOC_SAT_CONTROL
/*==========================================================================

FUNCTION ACPWB_CHECK_RSAT

DESCRIPTION
  This function is called by the clock interrupt as part of the clock
  services call-back services.  This function retrieves the SAT read
  status from the MSM2 VC_FM_STATUS register.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_check_rsat
(
  voc_change_status_type status
  /* New Status of Vocoder */
);

/* <EJECT> */
#endif /* FEATURE_VOC_SAT_CONTROL */
/*===========================================================================

FUNCTION ACPWB_SAT_ON

DESCRIPTION
  Turn SAT On or Off.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void acpwb_sat_on
(
  boolean sat_on  /* TRUE = Turns SAT on  FALSE = Turns SAT off */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_SET_SCC

DESCRIPTION
  Sets value of Sat Color Code (SCC).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_set_scc
(
  acpwb_scc_type scc
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_FADE_TIMING_STATUS

DESCRIPTION
  Returns the value of the fade_timing_status.

DEPENDENCIES
  None

RETURN VALUE
  ACP553_VALID_SAT   - SAT matches SCCr (expected SAT)
  ACP553_INVALID_SAT - Valid SAT detected but does not match expected SAT
  ACP553_NO_SAT      - No valid SAT detected

SIDE EFFECTS
  None

===========================================================================*/
extern acp553_rsat_status_type acpwb_fade_timing_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_CC_MODE

DESCRIPTION
  Set mode to Control Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_cc_mode( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_VC_MODE

DESCRIPTION
  Set mode to Voice Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_vc_mode( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_BUSY_IDLE_STATUS

DESCRIPTION
  Return current state of the busy/Idle status accumulator.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern boolean acpwb_busy_idle_status( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_SET_STREAM

DESCRIPTION
  Sets the forward control channel stream to listen to: Stream A 
  (for mobiles with LSB of MIN equal to '0') or Strean B (for 
  mobiles with LSB of MIN equal to '1').

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
   Queued words and/or words being received are discarded

===========================================================================*/
extern void acpwb_set_stream
(
  dword min1
    /* First part of Mobile Station Identification Number */
);

/*  <EJECT> */
/*===========================================================================

FUNCTION ACPWB_GET_WORD_SYNC_COUNT

DESCRIPTION
  Returns the number of FOCC word synchronization sequences received since
  the last call to acpwb_reset_word_sync_count().

DEPENDENCIES
  None

RETURN VALUE
  word - Number of word sync sequences. If count exceeds 0xFFFF then 0xFFFF
  is returned

SIDE EFFECTS
  None

===========================================================================*/
extern word acpwb_get_word_sync_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RESET_WORD_SYNC_COUNT

DESCRIPTION
  Resets the number of word synchronization sequences count.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_reset_word_sync_count( void );

#ifdef T_MSM2P
/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_ENABLE_RXMSG_FILTER

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
extern void acpwb_enable_rxmsg_filter
(
  boolean filter_enabled
    /* Enable or disable the FOCC message prefilter */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RESET_DFM_TXCTRL

DESCRIPTION
  Resets MSM2P Digital FM Wideband Data Transmit by setting DFM_TXCTRL_RESET
  to zero.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_reset_dfm_txctrl( void );

/* <EJECT> */
/*==========================================================================

FUNCTION ACPWB_SET_TRACKING_MODE

DESCRIPTION
  This function is called by the clock interrupt as part of the clock
  services call-back services.  This function calls RF Interface to 
  set the MSM to DFM tracking mode.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None   
===========================================================================*/

void acpwb_set_tracking_mode
(
  int4 ms_interval
    /* Time interval since last expiration - not used */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_CC_DISABLE

DESCRIPTION
  This function disables FM receive wideband data interrupts.  The
  acpfm_rxword_q is emptied to ensure no items remain that may have been
  there from a previous mode.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void acpwb_cc_disable( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_RESET_DFM_WBD

DESCRIPTION
  This function resets the MSM's DFM receive wideband data circuitry and
  reenables FM receive wideband data interrupts.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void acpwb_reset_dfm_wbd( void );

#endif /* T_MSM2P */

#if defined (FEATURE_FM_SLOTTED)
/*===========================================================================

FUNCTION ACPWB_FM_SLOTTING

DESCRIPTION
  This function enables/disables the FM slotting algorithm.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void acpwb_fm_slotting
(
  boolean slotting_enabled
    /* enable/disable FM slotting */
);

/* <EJECT> */
/*===========================================================================

FUNCTION ACPWB_FM_SLOTTING_LOCK

DESCRIPTION
  Enables/Disables ability to enter FM slotting mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Disables FM slotting if slotting lock is enabled.

===========================================================================*/
void acpwb_fm_slotting_lock (boolean slotting_lock_enabled);

#endif /* FEATURE_FM_SLOTTED */

#endif /* ACPWB_H */
