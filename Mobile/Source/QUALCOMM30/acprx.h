#ifndef ACPRX_H
#define ACPRX_H
/*===========================================================================

                      ACPRX Unit Header File 

DESCRIPTION
  This header file contains all the definitions necessary in order 
  to interface with the ACPRX Unit.

Copyright (c) 1991,1992,1993 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1994,1995 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*==========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/common/shared/cp/vcs/acprx.h_v   1.0   Aug 11 2000 11:40:52   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/25/95   dy      Added macros for parsing first Alert With Info SMS word.
05/12/95   jca     Added Gemini target support.
01/27/94   jca     Added macros for parsing Extended Protocol (NAMPS) msgs.
11/27/92   jca     Added cmds and functions to support offline diagnostics.
01/22/92   JJW     First Revision
12/17/91   ARH     Creation

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "acp553.h"
#include "bit.h"

/* <EJECT> */
/*==========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
   Data type representing the types of forward channel messages that can
   be requested from ACPRX. Multiple message types can be requested by ORing
   together the desired types. Note that the ANY_TYPE mask is defined to 
   catch any other possible message type.
---------------------------------------------------------------------------*/

typedef enum
{
  ACPRX_UNKNOWN_MSG     = 0x0000,  /* used by msg builder until type known */
  ACPRX_CTL_FILLER_MSG  = 0x0001,  /* control filler message type          */
  ACPRX_OVERHD_MSG      = 0x0002,  /* overhead message train               */
  ACPRX_MOB_STA_CTL_MSG = 0x0004,  /* mobile station control message type  */
  ACPRX_ANY_MSG         = 0x7FFF   /* state machine can request for any msg*/
} acprx_rxmsg_type;

/*--------------------------------------------------------------------------
             Commands to the ACPRX Unit 
---------------------------------------------------------------------------*/

typedef enum
{
  ACPRX_START,            /* Invoke Analog Receive sub-task              */
  ACPRX_SET_MODE,         /* Set forward channel mode (Control or Voice) */
  ACPRX_SET_STREAM,       /* Set the control channel message stream      */
  ACPRX_START_RXMSG,      /* Start message building process              */
  ACPRX_STOP_RXMSG,       /* Stop message building process               */
  ACPRX_SCAN_RANGE,       /* Scan a range of channels, return best 2     */
  ACPRX_SCAN_LIST,        /* Scan a list of channels, return best 2      */
  ACPRX_TUNE_TO_CHAN,     /* Tune to the passed channel number           */
  ACPRX_SET_FADE_TIMER,   /* Turn fade timer monitoring on/off           */
  ACPRX_RESET,            /* Reset ACPRX sub-task                        */
  ACPRX_SHUTDOWN,         /* Shutdown ACPRX sub-task                     */
  ACPRX_IS55_RCVS1,       /* IS-55 offline diagnostics command           */
  ACPRX_IS55_WSTS,        /* IS-55 offline diagnostics command           */
  ACPRX_IS55_TERM         /* IS-55 offline diagnostics command           */
} acprx_cmd_name_type;

/*--------------------------------------------------------------------------
             Command report data type definitions
---------------------------------------------------------------------------*/

typedef struct                      /* SCAN_RANGE or SCAN_LIST report type */
{
  word ret_strongest;               /* channel number of strongest signal  */
  word ret_2nd_strongest;           /* channel number of 2nd strongest sig */
} acprx_scan_rpt_type;

typedef enum                        /* fade timer report status type       */
{
  ACPRX_NORMAL,                     /* indicates normal operation          */
  ACPRX_FADE_TIMEOUT                /* indicates fade timeout condition    */
} acprx_fade_timer_rpt_type;

/*--------------------------------------------------------------------------
   Complete report data type. Used to report scan results and fade timeout 
   conditions. Report contains the cmd_name for the command being reported.
---------------------------------------------------------------------------*/

typedef struct
{
  acprx_cmd_name_type        cmd_name;           /* command being reported */
  union
  {                                              /* report-specific fields */
    acprx_fade_timer_rpt_type fade_timer_rpt;       
    acprx_scan_rpt_type       scan_list_rpt;
  } parms;
} acprx_cmd_rpt_type;


/*--------------------------------------------------------------------------
                      Command parameter types
---------------------------------------------------------------------------*/

typedef struct                     /* Generic external command hdr type    */
{
  acprx_cmd_name_type cmd_name;                  /* command being sent     */
  void (*cmd_rpt_func_ptr)(acprx_cmd_rpt_type*); /* ptr to report function */
} acprx_hdr_type;

/*---------------------------------------------------------------------------
  Usage: Set the msg_type field to a mask of desired message types. The MIN
  1/2 fields are for the mobile ID and are used for MOB_STA_CTL_MSG type
  processing.
---------------------------------------------------------------------------*/
typedef struct                                  /* ACPRX_START_RXMSG       */
{
  acprx_rxmsg_type msg_type;                     /* message type to build  */
  dword min1;                                    /* MIN1 value of mobile   */
  word min2;                                     /* MIN2 value of mobile   */
  void (*ret_msg_func_ptr)(acp553_rxmsg_type *); /* msg report function ptr*/
} acprx_start_rxmsg_type;


typedef struct                                  /* ACPRX_SCAN_RANGE        */
{
  word        first_chan;                       /* 1st channel to scan     */
  word        last_chan;                        /* last channel to scan    */
} acprx_scan_range_type;


typedef struct                                  /* ACPRX_SCAN_LIST         */
{
  word        chan_list[ACP_MAX_CCLIST_POS];    /* array of channel numbers*/
} acprx_scan_list_type; 


typedef struct                                  /* ACPRX_TUNE_TO_CHAN      */
{
  word chan;                                    /* channel to tune to      */
} acprx_tune_to_chan_type;


typedef struct                                  /* ACPRX_SET_FADE_TIMER    */
{
  boolean        state;                         /* fade timer on/off value */
  acp553_scc_type scc;                          /* SAT color code to use   */
  void (*ft_rpt_func_ptr)( acprx_cmd_rpt_type*);/* fade timeout report func*/
} acprx_fade_timer_type;


typedef struct                                  /* ACPRX_SET_STREAM        */
{
  dword min1;                                   /* Mobile station's MIN1   */
} acprx_set_stream_type;

/*--------------------------------------------------------------------------
  Complete command data type. Contains fields for all data needed to send
  commands to ACPRX subtask.
---------------------------------------------------------------------------*/
typedef struct
{
  acprx_hdr_type           hdr; /* contains command and report function ptr*/
  union
  {                                             /* command-specific fields */
    acp553_mode_type        mode;
    acprx_set_stream_type   stream;
    acprx_start_rxmsg_type  start_rxmsg;
    acprx_scan_range_type   scan_range;
    acprx_scan_list_type    scan_list;
    acprx_tune_to_chan_type tune_cc;
    acprx_fade_timer_type   set_fade_timer;
  } parms;
} acprx_cmd_type;

/* <EJECT> */
/*==========================================================================

         FORWARD (RX) CHANNEL MESSAGE FIELD EXTRACTION MACROS

  DESCRIPTION:
    The macros below perform message (bit) field extraction using the 
    b_unpack* subroutines defined in 'bit.h'. The macro names specify
    the field and message type that the particular macro will extract

  ARGUMENTS:
    ptr := pointer to an acp553_rxword

  RETURN VALUE:                                                            
    Each macro returns the specified field

===========================================================================*/

/*--------------------------------------------------------------------------
   Fields applicable to all message types
---------------------------------------------------------------------------*/

#define ACPRX_T1T2( ptr )                b_unpackb( ((byte *) ptr),  0,  2)
#define ACPRX_DCC(  ptr )                b_unpackb( ((byte *) ptr),  2,  2)

/*--------------------------------------------------------------------------
   Control Filler Message Fields 
---------------------------------------------------------------------------*/

#define ACPRX_CF_CMAC( ptr )             b_unpackb( ((byte *) ptr), 10,  3)
#define ACPRX_CF_SDCC1( ptr )            b_unpackb( ((byte *) ptr), 13,  2)
#define ACPRX_CF_SDCC2( ptr )            b_unpackb( ((byte *) ptr), 17,  2)
#define ACPRX_CF_WFOM( ptr )             b_unpackb( ((byte *) ptr), 20,  1)
#define ACPRX_CF_OHD(  ptr )             b_unpackb( ((byte *) ptr), 25,  3)

/*--------------------------------------------------------------------------
   FOCC Mobile Station Control Message Fields 
---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Word 1 - Abbreviated Address Word                                       */

#define ACPRX_CC_MIN1( ptr )             b_unpackd( ((byte *) ptr),  4, 24)

/*-------------------------------------------------------------------------*/
/* Word 2 - Extended Address Word (fields applicable to all)               */

#define ACPRX_CC_SCC( ptr )              b_unpackb( ((byte *) ptr),  2,  2)
#define ACPRX_CC_MIN2( ptr )             b_unpackw( ((byte *) ptr),  4, 10)

/*-------------------------------------------------------------------------*/
/* Word 2 - Extended Address Word (SCC == 11 && EF == 0)                   */

#define ACPRX_CC_EF( ptr )               b_unpackb( ((byte *) ptr), 14,  1)
#define ACPRX_CC_MSG_TYPE( ptr )         b_unpackb( ((byte *) ptr), 15,  5)
#define ACPRX_CC_ORDQ( ptr )             b_unpackb( ((byte *) ptr), 20,  3)
#define ACPRX_CC_ORDER( ptr )            b_unpackb( ((byte *) ptr), 23,  5)

/*-------------------------------------------------------------------------*/
/* Word 2 - Extended Protocol Word (SCC == 11 && EF == 1)                  */

#define ACPRX_CC_MSL( ptr )              b_unpackb( ((byte *) ptr), 15,  5)
#define ACPRX_CC_MST( ptr )              b_unpackb( ((byte *) ptr), 20,  8)

/*-------------------------------------------------------------------------*/
/* Word 2 - Extended Address Word (SCC != 11)                              */

#define ACPRX_CC_VMAC( ptr )             b_unpackb( ((byte *) ptr), 14,  3)
#define ACPRX_CC_CHAN( ptr )             b_unpackw( ((byte *) ptr), 17, 11)

/*-------------------------------------------------------------------------*/
/* Words 3 and 4 - Directed-Retry Words                                    */

#define ACPRX_CC_CHANPOS1( ptr )         b_unpackb( ((byte *) ptr),  4,  7)
#define ACPRX_CC_CHANPOS2( ptr )         b_unpackb( ((byte *) ptr), 11,  7)
#define ACPRX_CC_CHANPOS3( ptr )         b_unpackb( ((byte *) ptr), 18,  7)

/*-------------------------------------------------------------------------*/
/* Word 3 - Base Station Challenge Order Confirmation Word                 */

#define ACPRX_CC_AUTHBS( ptr )           b_unpackd( ((byte *) ptr),  6, 18)

/*-------------------------------------------------------------------------*/
/* Word 3 - Unique Challenge Order Word                                    */

#define ACPRX_CC_RANDU( ptr )            b_unpackd( ((byte *) ptr),  4, 24)

/*-------------------------------------------------------------------------*/
/* Word 3 - First SSD Update Order Word                                    */

#define ACPRX_CC_RANDSSD_1( ptr )        b_unpackd( ((byte *) ptr),  4, 24)

/*-------------------------------------------------------------------------*/
/* Word 4 - Second SSD Update Order Word                                   */

#define ACPRX_CC_RANDSSD_2( ptr )        b_unpackd( ((byte *) ptr),  4, 24)

/*-------------------------------------------------------------------------*/
/* Word 5 - Third SSD Update Order Word                                    */

#define ACPRX_CC_RANDSSD_3( ptr )        b_unpackb( ((byte *) ptr), 16,  8)

/*-------------------------------------------------------------------------*/
/* Words 3 thru 6 - Extended Protocol Voice Mail Message Word              */

#define ACPRX_CC_EP_VOICE_CH1( ptr )     b_unpackb( ((byte *) ptr),  4,  6)
#define ACPRX_CC_EP_VOICE_CH2( ptr )     b_unpackb( ((byte *) ptr), 10,  6)
#define ACPRX_CC_EP_VOICE_CH3( ptr )     b_unpackb( ((byte *) ptr), 16,  6)
#define ACPRX_CC_EP_VOICE_CH4( ptr )     b_unpackb( ((byte *) ptr), 22,  6)

/*-------------------------------------------------------------------------*/
/* Words 3 thru 6 - Extended Protocol Short Message Word                   */

#define ACPRX_CC_EP_SHORT_CH1( ptr )     b_unpackb( ((byte *) ptr),  4,  6)
#define ACPRX_CC_EP_SHORT_CH2( ptr )     b_unpackb( ((byte *) ptr), 10,  6)
#define ACPRX_CC_EP_SHORT_CH3( ptr )     b_unpackb( ((byte *) ptr), 16,  6)
#define ACPRX_CC_EP_SHORT_CH4( ptr )     b_unpackb( ((byte *) ptr), 22,  6)

/*-------------------------------------------------------------------------*/
/* Words 3 thru 8 - Extended Protocol CLI Word                             */

#define ACPRX_CC_EP_CLI_DIGIT1( ptr )     b_unpackb( ((byte *) ptr),  4,  4)
#define ACPRX_CC_EP_CLI_DIGIT2( ptr )     b_unpackb( ((byte *) ptr),  8,  4)
#define ACPRX_CC_EP_CLI_DIGIT3( ptr )     b_unpackb( ((byte *) ptr), 12,  4)
#define ACPRX_CC_EP_CLI_DIGIT4( ptr )     b_unpackb( ((byte *) ptr), 16,  4)
#define ACPRX_CC_EP_CLI_DIGIT5( ptr )     b_unpackb( ((byte *) ptr), 20,  4)
#define ACPRX_CC_EP_CLI_DIGIT6( ptr )     b_unpackb( ((byte *) ptr), 24,  4)

/*--------------------------------------------------------------------------
   Overhead message and global action message fields 
---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Fields applicable to all overhead messages (Note END field is applicable
   to all with the exception of the System Parameter word 1                */

#define ACPRX_OH_END( ptr)               b_unpackb( ((byte *) ptr), 24,  1)
#define ACPRX_OH_OHD(  ptr )             b_unpackb( ((byte *) ptr), 25,  3)

/*-------------------------------------------------------------------------*/
/* Word 1 - System Parameter Overhead Message                              */

#define ACPRX_OH_SID1( ptr )             b_unpackw( ((byte *) ptr),  4, 14)
#define ACPRX_OH_EP( ptr )               b_unpackb( ((byte *) ptr), 18,  1)
#define ACPRX_OH_AUTH( ptr )             b_unpackb( ((byte *) ptr), 19,  1)
#define ACPRX_OH_PCI( ptr )              b_unpackb( ((byte *) ptr), 20,  1)
#define ACPRX_OH_NAWC( ptr )             b_unpackb( ((byte *) ptr), 21,  4)

/*-------------------------------------------------------------------------*/
/* Word 2 -  System Parameter Overhead Message                             */

#define ACPRX_OH_S( ptr)                 b_unpackb( ((byte *) ptr),  4,  1)
#define ACPRX_OH_E( ptr)                 b_unpackb( ((byte *) ptr),  5,  1)
#define ACPRX_OH_REGH( ptr)              b_unpackb( ((byte *) ptr),  6,  1)
#define ACPRX_OH_REGR( ptr)              b_unpackb( ((byte *) ptr),  7,  1)
#define ACPRX_OH_DTX( ptr)               b_unpackb( ((byte *) ptr),  8,  2)
#define ACPRX_OH_N1( ptr)                b_unpackb( ((byte *) ptr), 10,  5)
#define ACPRX_OH_RCF( ptr)               b_unpackb( ((byte *) ptr), 15,  1)
#define ACPRX_OH_CPA( ptr)               b_unpackb( ((byte *) ptr), 16,  1)
#define ACPRX_OH_CMAX1( ptr)             b_unpackb( ((byte *) ptr), 17,  7)

/*-------------------------------------------------------------------------*/
/* Global Action Overhead Messages, fields applicable to all               */

#define ACPRX_OH_ACT( ptr)               b_unpackb( ((byte *) ptr),  4,  4)

/*-------------------------------------------------------------------------*/
/* Registration Increment Global Action Message                            */

#define ACPRX_OH_REGINCR( ptr)           b_unpackw( ((byte *) ptr),  8, 12)

/*-------------------------------------------------------------------------*/
/* Location Area Global Action Message                                     */

#define ACPRX_OH_PUREG( ptr )            b_unpackb( ((byte *) ptr),  8,  1)
#define ACPRX_OH_PDREG( ptr )            b_unpackb( ((byte *) ptr),  9,  1)
#define ACPRX_OH_LREG( ptr )             b_unpackb( ((byte *) ptr), 10,  1)
#define ACPRX_OH_LOCAID( ptr )           b_unpackw( ((byte *) ptr), 12, 12)

/*-------------------------------------------------------------------------*/
/* CDMA Capability Global Action Message                                   */

#define ACPRX_OH_CDMA_FREQ( ptr )        b_unpackw( ((byte *) ptr),  8, 11)
#define ACPRX_OH_CDMA_AVAIL( ptr )       b_unpackb( ((byte *) ptr), 19,  1)

/*-------------------------------------------------------------------------*/
/* Overload Control Global Action Message                                  */

#define ACPRX_OH_OLC( ptr)               b_unpackw( ((byte *) ptr),  8, 16)

/*-------------------------------------------------------------------------*/
/* New Access Channel Set Global Action Message                            */

#define ACPRX_OH_NEWACC( ptr)            b_unpackw( ((byte *) ptr),  8, 11)

/*-------------------------------------------------------------------------*/
/* Access Type Parameters Global Action Message                            */

#define ACPRX_OH_BIS( ptr)               b_unpackb( ((byte *) ptr),  8,  1)

/*-------------------------------------------------------------------------*/
/* Access Attempt Parameters Global Action Message                         */

#define ACPRX_OH_MAXBUSY_PGR( ptr)       b_unpackb( ((byte *) ptr),  8,  4)
#define ACPRX_OH_MAXSZTR_PGR( ptr)       b_unpackb( ((byte *) ptr), 12,  4)
#define ACPRX_OH_MAXBUSY_OTHER( ptr)     b_unpackb( ((byte *) ptr), 16,  4)
#define ACPRX_OH_MAXSZTR_OTHER( ptr)     b_unpackb( ((byte *) ptr), 20,  4)

/*-------------------------------------------------------------------------*/
/* Random Challenge A Global Action Message                                */

#define ACPRX_OH_RAND1_A( ptr )          b_unpackw( ((byte *) ptr),  8, 16)

/*-------------------------------------------------------------------------*/
/* Random Challenge B Global Action Message                                */

#define ACPRX_OH_RAND1_B( ptr )          b_unpackw( ((byte *) ptr),  8, 16)

/*-------------------------------------------------------------------------*/
/* Registration ID Message                                                 */

#define ACPRX_OH_REGID( ptr)             b_unpackd( ((byte *) ptr),  4, 20)

/*--------------------------------------------------------------------------
   FVC Mobile Station Control Message Fields
---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Mobile Station Control Message Word 1 (applicable to all)               */

#define ACPRX_VC_SCC( ptr )              b_unpackb( ((byte *) ptr),  2,  2)
#define ACPRX_VC_PSCC( ptr )             b_unpackb( ((byte *) ptr),  4,  2)
#define ACPRX_VC_EF( ptr )               b_unpackb( ((byte *) ptr),  6,  1)

/*-------------------------------------------------------------------------*/
/* Mobile Station Control Message Word 1 (SCC == 11 && EF== 0)             */

#define ACPRX_VC_MSG_TYPE( ptr )         b_unpackb( ((byte *) ptr), 15,  5)
#define ACPRX_VC_ORDQ( ptr )             b_unpackb( ((byte *) ptr), 20,  3)
#define ACPRX_VC_ORDER( ptr )            b_unpackb( ((byte *) ptr), 23,  5)

/*-------------------------------------------------------------------------*/
/* Extended Protocol MS Control Msg Word 1 (SCC == 11 && EF == 1)          */

#define ACPRX_VC_MSL( ptr )              b_unpackb( ((byte *) ptr), 15,  5)
#define ACPRX_VC_MST( ptr )              b_unpackb( ((byte *) ptr), 20,  8)

/*-------------------------------------------------------------------------*/
/* Mobile Station Control Message Word 1 (SCC != 11)                       */

#define ACPRX_VC_VMAC( ptr )             b_unpackb( ((byte *) ptr), 14,  3)
#define ACPRX_VC_CHAN( ptr )             b_unpackw( ((byte *) ptr), 17, 11)

/*-------------------------------------------------------------------------*/
/* Word 2 - Base Station Challenge Order Confirmation                      */

#define ACPRX_VC_AUTHBS( ptr )           b_unpackd( ((byte *) ptr),  6, 18)

/*-------------------------------------------------------------------------*/
/* Word 2 - Unique Challenge Order Word                                    */

#define ACPRX_VC_RANDU( ptr )            b_unpackd( ((byte *) ptr),  4, 24)

/*-------------------------------------------------------------------------*/
/* Word 2 - First SSD Update Order Word                                    */

#define ACPRX_VC_RANDSSD_1( ptr )        b_unpackd( ((byte *) ptr),  2, 24)

/*-------------------------------------------------------------------------*/
/* Word 2 -  Common length field for both Alert and Flash With Info Word 2 */

#define ACPRX_VC_INFO_RL_W( ptr )       b_unpackb( ((byte *) ptr),  2,  5)

/*-------------------------------------------------------------------------*/
/* Word 2 -  First Alert With Info Word                                    */

#define ACPRX_VC_ALERT_RL_W( ptr )       b_unpackb( ((byte *) ptr),  2,  5)
#define ACPRX_VC_ALERT_SIGNAL( ptr )     b_unpackb( ((byte *) ptr),  7,  8)
#define ACPRX_VC_ALERT_CPN_RL( ptr )     b_unpackb( ((byte *) ptr), 15,  6)
#define ACPRX_VC_ALERT_PI( ptr )         b_unpackb( ((byte *) ptr), 21,  2)
#define ACPRX_VC_ALERT_SI( ptr )         b_unpackb( ((byte *) ptr), 23,  2)

/*-------------------------------------------------------------------------*/
/* Word 2 -  First Flash With Info Word                                    */

#define ACPRX_VC_FLASH_RL_W( ptr )       b_unpackb( ((byte *) ptr),  2,  5)
#define ACPRX_VC_FLASH_CPN_RL( ptr )     b_unpackb( ((byte *) ptr),  7,  6)
#define ACPRX_VC_FLASH_PI( ptr )         b_unpackb( ((byte *) ptr), 13,  2)
#define ACPRX_VC_FLASH_SI( ptr )         b_unpackb( ((byte *) ptr), 15,  2)

/*-------------------------------------------------------------------------*/
/* Word 2 -  First Alert With Info SMS Word                                */

#define ACPRX_VC_ALERT_SMS_RL_W( ptr )     b_unpackb( ((byte *) ptr),  2,  7)
#define ACPRX_VC_ALERT_SMS_SEQ_NO( ptr )   b_unpackb( ((byte *) ptr),  9,  3)
#define ACPRX_VC_ALERT_SMS_BF( ptr )       b_unpackb( ((byte *) ptr), 12,  2)
#define ACPRX_VC_ALERT_SMS_TASK_TM( ptr )  b_unpackb( ((byte *) ptr), 14,  1)

/*-------------------------------------------------------------------------*/
/* Word 3 - Second SSD Update Order Word                                   */

#define ACPRX_VC_RANDSSD_2( ptr )        b_unpackd( ((byte *) ptr),  2, 24)

/*-------------------------------------------------------------------------*/
/* Words 3..N - Flash With Info / Alert With Info                          */

#define ACPRX_VC_CHARACTER1( ptr )       b_unpackb( ((byte *) ptr),  4,  8)
#define ACPRX_VC_CHARACTER2( ptr )       b_unpackb( ((byte *) ptr), 12,  8)
#define ACPRX_VC_CHARACTER3( ptr )       b_unpackb( ((byte *) ptr), 20,  8)

/*-------------------------------------------------------------------------*/
/* Word 4 - Third SSD Update Order Word                                    */

#define ACPRX_VC_RANDSSD_3( ptr )        b_unpackb( ((byte *) ptr),  2,  8)

/*-------------------------------------------------------------------------*/
/* Words 2 thru 5 - Extended Protocol Voice Mail Message Word              */

#define ACPRX_VC_EP_VOICE_CH1( ptr )     b_unpackb( ((byte *) ptr),  4,  6)
#define ACPRX_VC_EP_VOICE_CH2( ptr )     b_unpackb( ((byte *) ptr), 10,  6)
#define ACPRX_VC_EP_VOICE_CH3( ptr )     b_unpackb( ((byte *) ptr), 16,  6)
#define ACPRX_VC_EP_VOICE_CH4( ptr )     b_unpackb( ((byte *) ptr), 22,  6)

/*-------------------------------------------------------------------------*/
/* Words 2 thru 5 - Extended Protocol Short Message Word                   */

#define ACPRX_VC_EP_SHORT_CH1( ptr )     b_unpackb( ((byte *) ptr),  4,  6)
#define ACPRX_VC_EP_SHORT_CH2( ptr )     b_unpackb( ((byte *) ptr), 10,  6)
#define ACPRX_VC_EP_SHORT_CH3( ptr )     b_unpackb( ((byte *) ptr), 16,  6)
#define ACPRX_VC_EP_SHORT_CH4( ptr )     b_unpackb( ((byte *) ptr), 22,  6)

/*-------------------------------------------------------------------------*/
/* Words 2 thru 7 - Extended Protocol CLI Word                             */

#define ACPRX_VC_EP_CLI_DIGIT1( ptr )     b_unpackb( ((byte *) ptr),  4,  4)
#define ACPRX_VC_EP_CLI_DIGIT2( ptr )     b_unpackb( ((byte *) ptr),  8,  4)
#define ACPRX_VC_EP_CLI_DIGIT3( ptr )     b_unpackb( ((byte *) ptr), 12,  4)
#define ACPRX_VC_EP_CLI_DIGIT4( ptr )     b_unpackb( ((byte *) ptr), 16,  4)
#define ACPRX_VC_EP_CLI_DIGIT5( ptr )     b_unpackb( ((byte *) ptr), 20,  4)
#define ACPRX_VC_EP_CLI_DIGIT6( ptr )     b_unpackb( ((byte *) ptr), 24,  4)

/* <EJECT> */
/*==========================================================================

                       PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*==========================================================================
  
   FUNCTION ACPRX_CMD
  
   DESCRIPTION 
     This is how the outside world interfaces with the ACPRX subtask. Call 
     this function with a specified command and the command will be placed 
     on the ACPRX subtask command queue. NOTE: this function runs in the 
     context of another task. Care must be given to the processing performed.
  
   DEPENDENCIES 
     None 
    
   RETURN VALUE
     None
  
   SIDE EFFECTS
     None

===========================================================================*/

extern void acprx_cmd
(
  acprx_cmd_type *cmd_ptr              /* pointer to external command type */
);

/* <EJECT> */
/*==========================================================================
   
   FUNCTION ACPRX_POWERUP_INIT
  
   DESCRIPTION 
     This function performs the once-per-DMSS-powerup sub-task initialization 
     needed for the ACPRX sub-task. Upon completion of this function commands 
     can be queued on the ACPRX command queue.
  
   DEPENDENCIES 
     This function must be called from within the same task (RX task)
  
   RETURN VALUE 
     None
  
   SIDE EFFECTS 
     None
 
===========================================================================*/

extern void acprx_powerup_init( void );

/* <EJECT> */
/*==========================================================================

   FUNCTION ACPRX_SUBTASK
  
   DESCRIPTION 
     This is the entry point and main loop for the analog receive sub-task 
     ACPRX. The loop will wait for signals to indicate a forward channel 
     message word is available or that a state machine signal has been raised.
     Processing will be vectored to the message building module and/or to the 
     state machine module based upon the signals. This function will not exit 
     until a ACPRX_SHUTDOWN command has been received and completely processed.
     
   DEPENDENCIES 
     - The main module for the task must have already performed REX-related 
       task startup/sync work
     - 'acprx_powerup_init' must be called prior to entering this sub-task

   RETURN VALUE 
     None
  
   SIDE EFFECTS
     None

===========================================================================*/

extern void acprx_subtask( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPRX_GET_UNCORR_WORD_COUNT

DESCRIPTION
  Returns the number of uncorrectable words received since the last call
  to acprx_reset_word_counts().

  This procedure can be used to retrieve the information required by the 
  IS-55 RCVS1 command.

  The counts are maintained in both control or voice channel mode.

DEPENDENCIES
  None

RETURN VALUE
  byte - Number of uncorrectable words
  If the count exceeds 255, then 255 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acprx_get_uncorr_word_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPRX_GET_CORR_WORD_COUNT

DESCRIPTION
  Returns the number of correctable words received since the last call
  to acprx_reset_word_counts().

  This procedure can be used to retrieve the information required by the 
  IS-55 RCVS1 command.

  The counts are maintained in both control or voice channel mode.
 
DEPENDENCIES
  None

RETURN VALUE
  byte - Number of correctable words
  If the count exceeds 255, then 255 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern byte acprx_get_corr_word_count( void );

/* <EJECT> */
/*===========================================================================

FUNCTION ACPRX_GET_WORD_SYNC_COUNT

DESCRIPTION
  Returns the number of word sync sequences received since the last call
  to acprx_reset_word_counts().

  This procedure can be used to retrieve the information required by the 
  IS-55 WSTS command.

  The counts are maintained in both control or voice channel mode.

DEPENDENCIES
  None

RETURN VALUE
  word - Number of word sync sequences detected
  If the count exceeds 65535, then 65535 is returned.

SIDE EFFECTS
  None

===========================================================================*/
extern word acprx_get_word_sync_count( void );

#endif
