#ifndef ACPMCVCI_H
#define ACPMCVCI_H
/*===========================================================================

                       A  C  P  M  C  V  C  I

DESCRIPTION
  This header file contains INTERNAL data definitions and declarations 
  for the Analog Call Processing Voice Channel (ACPMCVC) Unit.

Copyright (c) 1992,1993,1994 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1995 by QUALCOMM Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acpmcvci.h_v   1.0   Aug 11 2000 11:40:50   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/04/98   pms     Neared the variables to save ROM space.
12/18/95   jca     Extern buffer declarations moved to acpmci.h.
11/24/92   jca     Added acpmcvc_dtmf_muted variable.
11/16/92   jca     Release after DMSS code review.
08/07/92   jca     Increased waiting for order timeout to 10 secs.
07/09/92   jca     Initial release to DMSS PVCS.
05/15/92   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "acpcmdef.h"
#include "ui.h"
#include "mc.h"
#include "acpmci.h"
#include "acprx.h"
#include "acptx.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define WAIT_FOR_ORDER_TASK_INTERVAL 10000
  /* 10 seconds (Refer to IS-95A section 2.6.4.3.1) */

#define WAIT_FOR_ANSWER_TASK_INTERVAL 65000
  /* 65 seconds (Refer to IS-95A section 2.6.4.3.2) */

#define RELEASE_DELAY_DURATION 500
  /* 500 ms (Refer to IS-95A section 2.6.4.3.2) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef enum {
  CONFIRM_INITIAL_VC_SS,  /* IS-95A 2.6.4.2 Confirm Init Voice Chan        */
  WAITING_FOR_ORDER_SS,   /* IS-95A 2.6.4.3.1 Waiting for Order            */
  WAITING_FOR_ANSWER_SS,  /* IS-95A 2.6.4.3.2 Waiting for Answer           */
  CONVERSATION_SS,        /* IS-95A 2.6.4.4 Conversation                   */
  RELEASE_SS,             /* IS-95A 2.6.4.5 Release/2.6.4.6 Powerdown      */
  HANDOFF_SS,             /* Process Handoff (to Analog Voice Chan) Order  */
  CHANGE_POWER_SS,        /* Process Change Power Order                    */
  FLASH_SS,               /* Process User Flash Request                    */
  CDMA_HANDOFF_SS,        /* Process Handoff (from CDMA) processing        */
  TIMER_EXPIRED_SS        /* Handle (Alert, Order or Fade Timer) timeout   */
} acpmcvc_substate_enum_type;
  /* The substates the ACPMC Voice Channel Unit can be in */

typedef enum {
  CIVC_CONFIRM_VOICE_CHANNEL_MS,  /* Tune to designated voice channel      */
  CIVC_SET_TX_POWER_LEVEL_MS,     /* Set RF power level                    */
  CIVC_TURN_ON_TRANSMITTER_MS,    /* Turn on the SAT transponder and PA    */
  CIVC_EXIT_CIVC_SUBSTATE_MS      /* Exit CIVC substate when PA settles    */
} civc_mini_state_type;
  /* CONFIRM_INITIAL_VC_SS mini-states */

typedef enum {
  WFO_SET_ORDER_TIMER_MS,   /* Set the order timer                         */
  WFO_WAITING_FOR_ORDER_MS, /* Wait for rx message or order timer timeout  */
  WFO_ENTER_WFA_STATE_MS    /* Transition to WAITING_FOR_ANSWER_SS when
                                 delay timer expires                       */
} wfo_mini_state_type;
  /* WAITING_FOR_ORDER_SS mini-states */

typedef enum {
  WFA_SET_ALERT_TIMER_MS,       /* Set the alert timer                     */
  WFA_WAITING_FOR_ANSWER_MS,    /* Wait for user to answer the phone,
                                   rx message or an alert timer timeout    */
  WFA_PROCESS_HANDOFF_ORDER_MS, /* Enter HANDOFF_SS when timer expires     */
  WFA_ENTER_RELEASE_STATE_MS    /* Enter RELEASE_SS when timer expires     */
} wfa_mini_state_type;
  /* WAITING_FOR_ANSWER_SS mini-states */

typedef enum {
  CONV_SET_RELEASE_DELAY_TIMER_MS, /* Set the release-delay timer          */
  CONV_WAITING_FOR_ORDER_MS,      /* Wait for user command or rx message   */
  CONV_ENTER_WFA_STATE_MS,        /* Enter WAITING_FOR_ANSWER_SS when
                                       delay timer expires                 */
  CONV_ENTER_RELEASE_STATE_MS     /* Enter RELEASE_SS when timer expires   */
} conversation_mini_state_type;
  /* CONVERSATION_SS mini-states */

typedef enum {
  RLS_GENERATE_SIGNAL_TONE_MS, /* Generate "release" signaling tone        */
  RLS_RESET_ACPTX_TASK_MS,     /* Reset the ACPTX subtask                  */
  RLS_RESET_ACPRX_TASK_MS,     /* Reset the ACPRX subtask                  */
  RLS_EXIT_VC_STATE_MS         /* Enter Serving-System Determination state */
} release_mini_state_type;
  /* RELEASE_SS mini-states */

typedef enum {
  HO_GENERATE_SIGNAL_TONE_MS, /* Generate "handoff" signaling tone         */
  HO_ADJUST_POWER_LEVEL_MS,   /* Change the RF power level                 */
  HO_STOP_FADE_TIMER_MS,      /* Suspend fade timer processing             */
  HO_STOP_RX_MSGS_MS,         /* Suspend rx message processing             */
  HO_TUNE_TO_NEW_CHANNEL_MS,  /* Tune to the designated voice channel      */
  HO_TURN_ON_TRANSMITTER_MS   /* Resume normal voice channel operation     */
} handoff_mini_state_type;
  /* HANDOFF_SS mini-states */

typedef enum {
  CDMA_HO_TUNE_TO_NEW_CHANNEL_MS,  /* Tune to the designated voice channel */
  CDMA_HO_ADJUST_POWER_LEVEL_MS,   /* Change the RF power level            */
  CDMA_HO_TURN_ON_TRANSMITTER_MS   /* Transition to CONVERSATION substate  */
} cdma_handoff_mini_state_type;
  /* CDMA_HANDOFF_SS mini-states */

typedef enum {
  CP_ADJUST_POWER_LEVEL_MS,     /* Change the RF power level               */
  CP_SEND_ORDER_CONFIRMATION_MS /* Send order confirmation to base station */
} change_power_mini_state_type;
  /* CHANGE_POWER_SS mini-states */

typedef enum {
  FLASH_GENERATE_SIGNAL_TONE_MS, /* Generate "flash" signaling tone        */
  FLASH_PROCESSING_REQUEST_MS,   /* Wait for signaling tone cmd response,
                                      user command or rx message           */
  FLASH_CANCELING_REQUEST_MS     /* Wait for cancel signaling tone cmd rsp */
} flash_mini_state_type;
  /* FLASH_SS mini-states */

typedef enum {
  TMOUT_RESET_ACPTX_TASK_MS, /* Reset the ACPTX subtask                    */
  TMOUT_RESET_ACPRX_TASK_MS, /* Reset the ACPRX subtask                    */
  TMOUT_EXIT_VC_STATE_MS     /* Enter Serving-System Determination state   */
} timeout_mini_state_type;
  /* TIMER_EXPIRED_SS mini-states */

typedef struct {
  acpmcvc_substate_enum_type substate;
  union {
    civc_mini_state_type          civc;
    wfo_mini_state_type           wfo;
    wfa_mini_state_type           wfa;
    conversation_mini_state_type  conv;
    release_mini_state_type       rls;
    handoff_mini_state_type       ho;
    cdma_handoff_mini_state_type  cdma_ho;
    change_power_mini_state_type  cp;
    flash_mini_state_type         flash;
    timeout_mini_state_type       tmout;
  } mini_state;
} acpmcvc_state_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*----------------------------------------------------------------*/
/* The following variables are shared by ACPMCVC.C and ACPVCSUP.C */
/*----------------------------------------------------------------*/

extern acpmcvc_state_type acpmcvc_current;
  /* The current state of the Voice Channel state machine */

extern acpmcvc_state_type            acpmcvc_previous;
  /* The previous state of the Voice Channel state machine */

extern acp_mask_type acpmcvc_signal_mask;
  /* When calling acpmci_wait this variable indicates which task signal(s) to
     suspend execution on.  When returning from call to acpmci_wait it holds
     the current state of the task signal mask (at least one of the mask bits
     matches the specified mask). */

extern acpmc_origination_param_type  acpmcvc_flash_data;
  /* Holds called address data from a user flash request */

extern acpmc_exit_entry_reason_type acpmcvc_exit_reason;
  /* Identifies the command/order that caused us to initiate release
     processing.  NOTE: If a MC_PREF_SYS_CHANGED_F, MC_NAM_CHANGED_F,
     MC_OFFLINE_DIGITAL_F or MC_OFFLINE_ANALOG_F command is received
     while processing a page response we handle these commands like a
     MC_POWERDOWN_F command.      */

extern boolean                       acpmcvc_dtmf_muted;
  /* Indicates if transmit DTMF tones are muted */

#endif
