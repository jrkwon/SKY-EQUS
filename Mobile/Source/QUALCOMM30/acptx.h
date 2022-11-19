#ifndef ACPTX_H
#define ACPTX_H

/*===========================================================================

                      ACPTX Unit Header File 

DESCRIPTION

  This header file contains all the definitions necessary for other tasks
  to interface with the ACPTX Unit.

Copyright (c) 1991,1992,1993,1994 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:   L:/src/asw/common/shared/cp/vcs/acptx.h_v   1.0   Aug 11 2000 11:41:02   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/25/95   dy      Added macros for building Alert With Info SMS Order
                   Confirmation message.
01/27/94   jca     Added macros for building Extended Protocol (NAMPS) msgs.
11/24/92   jca     Expanded ACPTX_MUTE command.
11/17/92   jca     Added ACPTX_IS55_CDATA command.
09/15/92   jca     Added ACPTX_MUTE command.
07/08/92   jca     Added ACPTX_START command.  Added include for BIT.H.
12/20/91   jca     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "acp553.h"
#include "bit.h"

/* <EJECT> */
/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/


/*-------------------------------------------------------------------------*/
/*                      ACPTX Command Names                                */
/*-------------------------------------------------------------------------*/

typedef enum
{
  ACPTX_SWITCH_PA,               /* Turn on/off power amp (transmitter)    */
  ACPTX_SWITCH_SAT,              /* Enable or disable transponding of SAT  */
  ACPTX_GENERATE_ST,             /* Generate ST for specified duration     */
  ACPTX_SET_TX_POWER_LEVEL,      /* Set transmit power level               */
  ACPTX_TX_RECC_MSG,             /* Transmit msg on the reverse ctl chan   */
  ACPTX_TX_RVC_MSG,              /* Transmit msg on the reverse voice chan */
  ACPTX_ACCESS_TIMEOUT,          /* System Access Timer has expired        */
  ACPTX_START,                   /* Activate ACPTX subtask                 */
  ACPTX_RESET,                   /* Reset to an idle state                 */
  ACPTX_SHUTDOWN,                /* Exit analog mode operation             */
  ACPTX_MUTE,                    /* (Un)Mute transmit voice and DTMF tones */
  ACPTX_IS55_CDATA               /* IS-55 offline diagnostics command      */
} acptx_cmd_name_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-------------------------------------------------------------------------*/
/*                     Command argument types                              */
/*-------------------------------------------------------------------------*/

/*--------------------------------*/
/* ACPTX_GENERATE_ST sub-commands */
/*--------------------------------*/
typedef enum {
  ACPTX_ST_RELEASE,      /* Transmit signaling tone for 1.8 secs    */
  ACPTX_ST_FLASH,        /* Transmit signaling tone for 400 ms       */
  ACPTX_ST_HANDOFF,      /* Transmit signaling tone for 50 ms       */
  ACPTX_ST_CONTINUOUS,   /* Transmit a continuous signaling tone    */
  ACPTX_ST_OFF           /* Stop transmission of the signaling tone */
} acptx_st_cmd_type;

/* -----------------------
** ACPTX_MUTE sub-commands
** ----------------------- */
typedef enum {
  ACPTX_MUTE_ALL,        /* Mute transmit voice and DTMF tones   */
  ACPTX_MUTE_DTMF,       /* Mute DTMF tones only                 */
  ACPTX_UNMUTE           /* Unmute transmit voice and DTMF tones */
} acptx_mute_cmd_type;

/*-------------------------------------------------------------------------*/
/*                     Command Response types                              */
/*-------------------------------------------------------------------------*/

/*------------------------------------------*/
/* ACPTX_TX_RECC_MSG command status options */
/*------------------------------------------*/
typedef enum {
  ACPTX_MSG_SUCCESS_S,   /* Ctl or Voice channel msg was transmitted     */
  ACPTX_MSG_FAILURE_S    /* Ctl chn collision or busy condition detected */
} acptx_recc_msg_status_type;

/*------------------------------------------------------------*/
/* Data structure for all command reports from the ACPTX unit */
/*------------------------------------------------------------*/
typedef struct {
  acptx_cmd_name_type cmd_name; /* Identifies report for what cmd */
  union {
    acptx_recc_msg_status_type recc_msg_status;
      /* ACPTX_TX_RECC_MSG command status */
    acptx_st_cmd_type          st_cmd;
      /* Identifes ACPTX_GENERATE_ST sub-command that was issued */
  } parms;
} acptx_cmd_rpt_type;

/*-------------------------------------------------------------------------*/
/*                     Header for all commands                             */
/*-------------------------------------------------------------------------*/

typedef struct {
  acptx_cmd_name_type cmd_name;
     /* Type of command */
  void (*cmd_rpt_func_ptr) (acptx_cmd_rpt_type*);
     /* Pointer to command report function */
} acptx_hdr_type;

/*-------------------------------------------------------------------------*/
/*                         Command types                                   */
/*-------------------------------------------------------------------------*/

typedef struct {         /* Parameters for ACPTX_SWITCH_PA cmd */
  boolean state;
} acptx_switch_pa_type;


typedef struct {         /* Parameters for ACPTX_SWITCH_SAT cmd */
  boolean state;
} acptx_switch_sat_type;


typedef struct {         /* Parameters for ACPTX_GENERATE_ST cmd */
  acptx_st_cmd_type cmd;
} acptx_generate_st_type;


typedef struct {         /* Parameters for ACPTX_TX_RECC_MSG cmd */
  boolean            bis_s;       /* Busy-idle status monitor indicator  */
  byte               maxbusy_sl;  /* Max number of RECC busy occurences  */
  byte               maxsztr_sl;  /* Max number of RECC seizure attempts */
  byte               pl_s;        /* Mobile station RF power level       */
  byte               dcc_s;       /* Digital Color Code                  */
  byte               wrd_cnt;     /* Number of tx words in RECC msg      */
  acp553_txmsg_type  msg;         /* The RECC msg (bit-packed format)    */
} acptx_recc_msg_cmd_type;


typedef struct {         /* Parameters for ACPTX_TX_RVC_MSG cmd */
  byte               wrd_cnt;   /* Number tx words in RVC msg      */
  acp553_txmsg_type  msg;       /* The RVC msg (bit-packed format) */
} acptx_rvc_msg_cmd_type;


typedef struct {         /* Parameters for ACPTX_SET_TX_POWER_LEVEL cmd */
  byte level;  /* Mobile Station RF Power Level */
} acptx_set_pwr_level_type;


typedef struct {         /* Parameters for ACPTX_MUTE command */
  acptx_mute_cmd_type tx_audio;
} acptx_mute_type;


typedef struct {         /* Parameters for ACPTX_IS55_CDATA_CMD command */
  byte dcc;              /* Digital Color Code                          */
  byte tx_data[6];       /* 48 bit tx data word (includes parity bits)  */
} acptx_is55_cdata_cmd_type;

/*-----------------------------------------------*/
/* Data structure for all commands to ACPTX unit */
/*-----------------------------------------------*/
typedef struct {
  acptx_hdr_type hdr;
  union {
    acptx_switch_pa_type      pa;
    acptx_switch_sat_type     sat;
    acptx_generate_st_type    st;
    acptx_recc_msg_cmd_type   recc;
    acptx_rvc_msg_cmd_type    rvc;
    acptx_set_pwr_level_type  tx_pwr;
    acptx_mute_type           mute;
    acptx_is55_cdata_cmd_type cdata;
  } parms;
} acptx_cmd_type;

    
/* <EJECT> */
/*===========================================================================

REVERSE CHANNEL MESSAGE BIT PACKING MACROS

DESCRIPTION
  This collection of macros are utilized to translate analog reverse
  channel messages from the internal unpacked format to the external 
  CDMA CAI bit-packed format.

DEPENDENCIES
  None

ARGUMENTS
  Value - Value to pack into specified bit field of the specified message
          buffer
  Ptr   - Pointer to the message buffer to pack into

SIDE EFFECTS
  None

===========================================================================*/

/* Reverse Control Channel (RECC) Message Macros */

/* Fields applicable to all RECC messages */

#define ACPTX_CC_F( value, ptr )                  b_packb( value, ptr,  0,  1)
#define ACPTX_CC_NAWC( value, ptr )               b_packb( value, ptr,  1,  3)

/* Fields applicable to all Extended Protocol Message words */
#define ACPTX_CC_EP_F( value, ptr )               b_packb( value, ptr,  0,  2)
#define ACPTX_CC_EP_RSVD( value, ptr )            b_packb( value, ptr,  2,  2)

/* Word A - Abbreviated Address Word */

#define ACPTX_CC_ABBR_ADDR_T( value, ptr )        b_packb( value, ptr,  4,  1)
#define ACPTX_CC_ABBR_ADDR_S( value, ptr )        b_packb( value, ptr,  5,  1)
#define ACPTX_CC_ABBR_ADDR_E( value, ptr )        b_packb( value, ptr,  6,  1)
#define ACPTX_CC_ABBR_ADDR_ER( value, ptr )       b_packb( value, ptr,  7,  1)
#define ACPTX_CC_ABBR_ADDR_SCM( value, ptr )      b_packb( value, ptr,  8,  4)
#define ACPTX_CC_ABBR_ADDR_MIN1( value, ptr )     b_packd( value, ptr, 12, 24)

/* Word B - Extended Address Word */

#define ACPTX_CC_EXT_ADDR_MSG_TYPE( value, ptr )  b_packb( value, ptr,  4,  5)
#define ACPTX_CC_EXT_ADDR_ORDQ( value, ptr )      b_packb( value, ptr,  9,  3)
#define ACPTX_CC_EXT_ADDR_ORDER( value, ptr )     b_packb( value, ptr, 12,  5)
#define ACPTX_CC_EXT_ADDR_LT( value, ptr )        b_packb( value, ptr, 17,  1)
#define ACPTX_CC_EXT_ADDR_EP( value, ptr )        b_packb( value, ptr, 18,  1)
#define ACPTX_CC_EXT_ADDR_SCM( value, ptr )       b_packb( value, ptr, 19,  1)
#define ACPTX_CC_EXT_ADDR_MPCI( value, ptr )      b_packb( value, ptr, 20,  2)
#define ACPTX_CC_EXT_ADDR_SDCC1( value, ptr )     b_packb( value, ptr, 22,  2)
#define ACPTX_CC_EXT_ADDR_SDCC2( value, ptr )     b_packb( value, ptr, 24,  2)
#define ACPTX_CC_EXT_ADDR_MIN2( value, ptr )      b_packw( value, ptr, 26, 10)

/* Word B' - Extended Protocol Extended Address Word */

#define ACPTX_CC_EP_EXT_ADDR_MSL( value, ptr )    b_packb( value, ptr,  4,  5)
#define ACPTX_CC_EP_EXT_ADDR_MST( value, ptr )    b_packb( value, ptr,  9,  8)

/* Word C - Serial Number Word */

#define ACPTX_CC_SER_NUM_SERIAL( value, ptr )     b_packd( value, ptr,  4, 32)

/* Word C - Authentication Word */

#define ACPTX_CC_AUTH_COUNT( value, ptr )         b_packb( value, ptr,  4,  6)
#define ACPTX_CC_AUTH_RANDC( value, ptr )         b_packb( value, ptr, 10,  8)
#define ACPTX_CC_AUTH_AUTHR( value, ptr )         b_packd( value, ptr, 18, 18)

/* Word C - Unique Challenge Order Confirmation Word */

#define ACPTX_CC_CHAL_CONF_RSVD( value, ptr )     b_packw( value, ptr,  4, 14)
#define ACPTX_CC_CHAL_CONF_AUTHU( value, ptr )    b_packd( value, ptr, 18, 18)

/* Word C - Base Station Challenge Word */

#define ACPTX_CC_BS_CHAL_RANDBS( value, ptr )     b_packd( value, ptr,  4, 32)

/* Words D to G - Called-Address Words */

#define ACPTX_CC_CALL_ADDR_DIG1( value, ptr )     b_packb( value, ptr,  4,  4)
#define ACPTX_CC_CALL_ADDR_DIG2( value, ptr )     b_packb( value, ptr,  8,  4)
#define ACPTX_CC_CALL_ADDR_DIG3( value, ptr )     b_packb( value, ptr, 12,  4)
#define ACPTX_CC_CALL_ADDR_DIG4( value, ptr )     b_packb( value, ptr, 16,  4)
#define ACPTX_CC_CALL_ADDR_DIG5( value, ptr )     b_packb( value, ptr, 20,  4)
#define ACPTX_CC_CALL_ADDR_DIG6( value, ptr )     b_packb( value, ptr, 24,  4)
#define ACPTX_CC_CALL_ADDR_DIG7( value, ptr )     b_packb( value, ptr, 28,  4)
#define ACPTX_CC_CALL_ADDR_DIG8( value, ptr )     b_packb( value, ptr, 32,  4)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Reverse Voice Channel (RVC) Message Macros */

/* Fields applicable to all RVC messages */

#define ACPTX_VC_F( value, ptr )                  b_packb( value, ptr,  0,  1)
#define ACPTX_VC_NAWC( value, ptr )               b_packb( value, ptr,  1,  2)
#define ACPTX_VC_T( value, ptr )                  b_packb( value, ptr,  3,  1)

/* Order Confirmation Message */

#define ACPTX_VC_ORDR_CONF_MSG_TYPE( value, ptr)  b_packb( value, ptr,  4,  5)
#define ACPTX_VC_ORDR_CONF_ORDQ( value, ptr)      b_packb( value, ptr,  9,  3)
#define ACPTX_VC_ORDR_CONF_ORDER( value, ptr)     b_packb( value, ptr, 12,  5)
#define ACPTX_VC_ORDR_BF( value, ptr)             b_packb( value, ptr, 17,  1)
#define ACPTX_VC_ORDR_ERROR_CLASS( value, ptr)    b_packb( value, ptr, 18,  2)
#define ACPTX_VC_ORDR_CAUSE_CODE( value, ptr)     b_packb( value, ptr, 20,  8)
#define ACPTX_VC_ORDR_SEQ_NO( value, ptr)         b_packb( value, ptr, 28,  3)
#define ACPTX_VC_ORDR_CONF_RSVD( value, ptr)      b_packd( value, ptr, 17,  19)

/* Called-Address Message (Words 1 thru 4) */

#define ACPTX_VC_CALL_ADDR_DIG1( value, ptr )     b_packb( value, ptr,  4,  4)
#define ACPTX_VC_CALL_ADDR_DIG2( value, ptr )     b_packb( value, ptr,  8,  4)
#define ACPTX_VC_CALL_ADDR_DIG3( value, ptr )     b_packb( value, ptr, 12,  4)
#define ACPTX_VC_CALL_ADDR_DIG4( value, ptr )     b_packb( value, ptr, 16,  4)
#define ACPTX_VC_CALL_ADDR_DIG5( value, ptr )     b_packb( value, ptr, 20,  4)
#define ACPTX_VC_CALL_ADDR_DIG6( value, ptr )     b_packb( value, ptr, 24,  4)
#define ACPTX_VC_CALL_ADDR_DIG7( value, ptr )     b_packb( value, ptr, 28,  4)
#define ACPTX_VC_CALL_ADDR_DIG8( value, ptr )     b_packb( value, ptr, 32,  4)

/* Word 2 of Serial Number Response message */

#define ACPTX_VC_SER_NUM_RSP_ESN( value, ptr )    b_packd( value, ptr,  4, 32)

/* Unique Challenge Order Confirmation Message */

#define ACPTX_VC_CHAL_CONF_MSG_TYPE( value, ptr ) b_packb( value, ptr,  4,  5)
#define ACPTX_VC_CHAL_CONF_ORDQ( value, ptr )     b_packb( value, ptr,  9,  3)
#define ACPTX_VC_CHAL_CONF_ORDER( value, ptr )    b_packb( value, ptr, 12,  5)
#define ACPTX_VC_CHAL_CONF_AUTHU( value, ptr )    b_packd( value, ptr, 17, 18)
#define ACPTX_VC_CHAL_CONF_RSVD( value, ptr )     b_packw( value, ptr, 35,  1)

/* Word 2 of Base Station Challenge Order message */

#define ACPTX_VC_BS_CHAL_RANDBS( value, ptr )     b_packd( value, ptr,  4, 32)

/* Word 1' - First Word of the Extended Protocol Message */

#define ACPTX_VC_EP_F1( value, ptr )             b_packb( value, ptr,  0,  2)
#define ACPTX_VC_EP_RSVD1( value, ptr )          b_packb( value, ptr,  2,  1)
#define ACPTX_VC_EP_T( value, ptr )              b_packb( value, ptr,  3,  1)
#define ACPTX_VC_EP_MSL( value, ptr)             b_packb( value, ptr,  4,  5)
#define ACPTX_VC_EP_MST( value, ptr)             b_packb( value, ptr,  9,  8)
#define ACPTX_VC_EP_ER( value, ptr)              b_packb( value, ptr, 17,  1)
#define ACPTX_VC_EP_EP( value, ptr)              b_packb( value, ptr, 18,  1)
#define ACPTX_VC_EP_RSVD2( value, ptr )          b_packd( value, ptr, 19, 17)

/*===========================================================================

                     PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ACPTX_POWERUP_INIT

DESCRIPTION
  This function is called once at system powerup so that the ACPTX task
  can perform preliminary initialization.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void acptx_powerup_init (void);


/* <EJECT> */
/*===========================================================================

FUNCTION ACPTX_SUBTASK

DESCRIPTION
   This is the entry point for the ACPTX subtask.  It contains the main
   processing loop for the subtask.
  
DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None

===========================================================================*/

extern void acptx_subtask (void);


/* <EJECT> */
/*===========================================================================

FUNCTION ACPTX_CMD

DESCRIPTION
   This is how the outside world interfaces with the ACPTX subtask.
   Call this function with a pointer to a command and the command
   will be placed on the ACPTX subtask command queue.
  
DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None

===========================================================================*/

extern void acptx_cmd
(
  acptx_cmd_type *cmd_ptr  /* Pointer to command to be queued */
);

#endif
