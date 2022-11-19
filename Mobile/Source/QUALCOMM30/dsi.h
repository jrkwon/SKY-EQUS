#ifndef DSI_H
#define DSI_H
/*===========================================================================

                   D A T A   T A S K   I N T E R N A L  
                        H E A D E R    F I L E

DESCRIPTION

  Data Task regional header file. All definitions, structures, and functions
  needed between the control and state machine units of the data task.


Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996,1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/dsi.h_v   1.7.1.0   16 May 2001 03:03:30   akhare  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/11/01   ak      Changed extern for ds_scrm_chng_cback()
03/29/01   ak      Added externs for retry delay functions.
03/12/01   kk      New value added for P_REV 7 to ds_bs_prev_enum_type.
01/25/01   na      Removed FEATURE defines from service mode enums.
                   Cleanup & removed FEATURE_NEW_SIO( always on).
01/16/01   jq      Changed naming of some variables and functions.
01/15/01   igt     Added signature for ds_end_call_upon_sync_timeout(void)
12/15/00   ak      Support for AT$QCTRTL, which turns off R-SCH throttling.
10/30/00   ak      Added extra param to ds_initiate_origination to account
                   for DRS bit.
08/06/00   ak      Re-featurized extern for the retry cback to be under
                   DS_IS2000 instead of IS2000_R_SCH.
07/24/00   ak      Removed AT$QCMDR=4 option, as that will not be supported.
                   Extern for ds_scrm_chng_cback.  Support AT$QCSCRM.
07/07/00   ak      Added externs and function prototypes to support SCRM
                   processing.  Also, dsmgr_start/cancel_timer are now
                   available outside of dsmgr.
02/22/00   snn     Added FEATURE_DS_CHAP around FEATURE_DS_PAP.
02/10/00   ak      Added rateset enum, more modes for MDR, and an enum
                   for p_revs.
01/26/00   na      Fixed compile time error in #define.
01/01/00   na      Set DS_MDR_MODE_DEFAULT to SO33 when FEATURE_DS_RLP3 is
                   compiled in.
12/27/99   na      Fixed ds_mdr_mode_enum_type to include support for service
                   option 33.
11/03/99   snn     Defined a Queue to be used to hold AT Command Parser 
                   responses
09/22/99   snn     Cleaned up PAP related code, to follow software 
                   guidelines. Kept PAP related extern definitions inside
                   FEATURE_DS_PAP_NV because, they are needed only if NV 
                   does not have the PAP items.
09/22/99   rc      Changed DS_UP_PKT_SRVC_MODE to DS_BROWSER_PKT_SRVC_MODE.
08/20/99   na      Added prototype for ds_set_rm_state().
08/18/99   ak      Made fixes so compiles when FEATURE_DS turned off.
08/05/99   mvl     Added network model changes.
07/09/99   snn     Added support for PAP implementation
04/11/99   smp     Changed MDR Mode default to MDR if available (was No MDR).
04/09/99   jss     Merged in changes to allow user to answer a voice call 
                   through the AT$QCCAV command.
03/04/99   hcg     Added sockets service mode under FEATURE_DS_SOCKETS.
02/10/99   smp     Put the function prototype for dsi_set_service_options()
                   and mdrrlp.h under FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR.
01/29/99   hcg     changed duration param type in dsi_start_timer to dword
01/17/99   ak      New SIO is now under a compile time switch.
01/13/99   smp     Added definition for MDR Mode enum. Also include mdrrlp.h
                   instead of rlp.h when FEATURE_IS95B_MDR is defined.
01/09/99   jjw     Integrated definitions to support new SIO interface
12/14/98   ldg     Fixed #ifdef around dsi_answer_amps_call() prototype.
11/12/98   ldg     Added const qualifier to function argument.
10/19/98   ldg     Changed QNC_DEFAULT from FALSE to TRUE.
10/15/98   dbd     Added support for analog fax on SSS targets
04/30/98   na      Added function prototype for ds_close_serial_port.
04/14/98   na      Added function prototype for ds_clr_err_log. Added function 
                   protototype for ds_set_inbound_flow and added associated
                   signals.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
11/18/97   ldg     Added type to represent result codes from checking the
                   data call origination capability.
11/14/97   ldg     Corrected misspelled service option names for PROPTRY and
                   IS707.
11/04/97   ldg     Added externs removed from various .c files.
                   Added #define's to specify originating service
                   options according to FEATURE.
                   Added prototypes for DS get/put NV functions.
                   Added prototype for dsi_set_service_options.
06/05/97   na      Added support for function that checks cmd queue.
05/30/96   dbd     Gemini & ISS2 Data Services release. 
11/09/95   jjw     Completed changes to support fully compliant IS-99
04/05/95   jjw     Added PS task queue
01/22/95   jjw     Changes to support use of IS-99 service data structures
04/21/94   jjw     Changes in support of IS-99 RLP frame formats
08/17/93   jjw     Initial version of file


===========================================================================*/





/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#ifdef FEATURE_DS
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
#include "mdrrlp.h"
#else
#include "rlp.h"
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */
#include "ds.h"
#include "rex.h"
#include "mc.h"
#include "nv.h"
#include "sio.h"

#ifdef FEATURE_DS_IS2000
#include "mccdma.h"
#endif


/*===========================================================================

                      REGIONAL DATA DECLARATIONS

===========================================================================*/



#if defined(FEATURE_SKT_ATPKT_DS) || defined(WAP_USE_DORM)
/*---------------------------------------------------------------------------
  Type that represents the current protocol state of the PS task.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_PROTOCOL_CLOSED,
  DS_PROTOCOL_OPENING,
  DS_PROTOCOL_OPEN,
  DS_PROTOCOL_CLOSING
} dsi_protocol_state_enum_type;

extern dsi_protocol_state_enum_type dsi_protocol_state;

#endif /* defined(FEATURE_SKT_ATPKT_DS) || defined(WAP_USE_DORM) */

/*--------------------------------------------------------------------------
  Type that represents the result codes from checking the data call
  Origination capability.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_OK_TO_ORIG,                 /* OK to Originate a data call            */
  DS_INUSE_CANNOT_ORIG,          /* Cannot Orig, phone in Use              */
  DS_NO_DIGITAL_CANNOT_ORIG,     /* Cannot Orig, phone NOT in Digital mode */
  DS_NO_SRV_CANNOT_ORIG,         /* Cannot Orig, service not acquired      */
  DS_HOLD_ORIG_CANNOT_ORIG       /* Cannot Orig, Hold Orig active          */
} dsi_orig_chk_enum_type;

/*--------------------------------------------------------------------------
  Enum to represent the service mode of a call.
---------------------------------------------------------------------------*/

typedef enum
{
  DS_NO_CALL,                         /* No Call incoming                  */
  DS_AMPS_CALL,                       /* AMPS service mode call indicator  */
  DS_IS99_CALL                        /* IS-99 service mode call indicator */
#ifdef	FEATURE_SKT_PIWF_DS	//	khekim 00/02/13
  ,DS_PIWF_CALL                        /* PIWF service mode call indicator */
#endif	/* FEATURE_SKT_PIWF_DS */
} ds_srvcmode_type;

/*---------------------------------------------------------------------------
  Data Task Service Mode Type Definitions
---------------------------------------------------------------------------*/
typedef enum
{
  DS_NULL_SRVC_MODE,
  DS_AUTODETECT_SRVC_MODE,     /* Autodetect Mode: detect service type     */
  DS_PKT_SRVC_MODE,            /* Packet Service mode                      */
  DS_RAWDATA_SRVC_MODE,        /* IS-99 Raw data, xfer data mode           */
  DS_BROWSER_PKT_SRVC_MODE,    /* Browser mode                             */
  DS_A_DATA_SRVC_MODE,         /* AMPS data service via DOTG card          */
  DS_SOCKET_SRVC_MODE          /* Socket service mode                      */
#ifdef	FEATURE_SKT_PTPAPI // khekim 00/03/01
  ,DS_AIRDATA_SRVC_MODE		   /* air data service mode                    */
#endif	/* FEATURE_SKT_PTPAPI */
} dsi_data_srvc_enum_type;

#ifdef	FEATURE_SKT_DS  //white 2001.06.13
/*---------------------------------------------------------------------------
  Originally the following type was defined in dsmgr.c
  Variable to keep track of the serial mode.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_SERIAL_CLOSED_MODE,    /* Serial port running 38.4, DS has no SIO */
  DS_SERIAL_OPENED_MODE,    /* Serial port 19.2, DS has and uses SIO   */
  DS_SERIAL_SUSPENDED_MODE  /* Serial port 19.2, DS has and doesn't use SIO */
} dsi_serial_mode_enum_type;
#endif /* FEATURE_SKT_DS */     //white end.

#if defined(WAPPER) || defined(FEATURE_SKT_DS)
/*---------------------------------------------------------------------------
  Data Task Call State Type Definitions
---------------------------------------------------------------------------*/
typedef enum
{
  DS_STARTUP_CSTATE,       /* Initial State upon coldstart                 */
  DS_IDLE_CSTATE,          /* Data Task IDLE, not in call                  */
  DS_ORIG_CALL_CSTATE,     /* Data Task origination in process             */
  DS_PROCESS_PAGE_CSTATE,  /* Received Data page, DTR deasserted           */
  DS_CALL_ACTIVE_CSTATE,   /* Data Call in process                         */
  DS_END_CALL_CSTATE,      /* End call via release from MC                 */
  DS_ENDING_CSTATE         /* Call ended by sending END-F to MC            */
} dsi_callstate_enum_type;

extern dsi_callstate_enum_type      dsi_callstate;

extern void dsi_cleanup_queues(void);
extern void dsi_set_callstate( dsi_callstate_enum_type new_callstate);
extern dsi_callstate_enum_type dsi_get_callstate(void);
#endif /* WAPPER || FEATURE_SKT_DS */

/*--------------------------------------------------------------------------
  Data Task Signal Definitions
---------------------------------------------------------------------------*/
typedef rex_sigs_type ds_sigs_type;

/*--------------------------------------------------------------------------
  Watermark item allocation for SIO to either ATCoP (IS-99) or to
  RLP (Packet Service).
---------------------------------------------------------------------------*/
extern dsm_watermark_type ds_from_sio_wm_item;

/*--------------------------------------------------------------------------
  Definition for Queue used with PS inbound watermark item.
--------------------------------------------------------------------------*/

extern q_type ds_ps_rx_q;

/*--------------------------------------------------------------------------
  Definition for Queue used to hold AT Command Parser responses
--------------------------------------------------------------------------*/

extern q_type ds_atcop_res_hold_q;

/*--------------------------------------------------------------------------
  NV item for DS
--------------------------------------------------------------------------*/

extern nv_item_type ds_nv_item;

/*--------------------------------------------------------------------------
  To hold the value retrieved from NV
--------------------------------------------------------------------------*/

extern nv_esn_type ds_esn;

/*---------------------------------------------------------------------------
  Variables needed for using SIO services.
--------------------------------------------------------------------------*/

extern sio_open_type           ds_sio_open;   /* Structure used in opening
                                                 SIO streams              */
extern sio_stream_id_type      dsi_stream_id; /* Stream Idenifier         */
extern dsi_data_srvc_enum_type dsi_srvc_mode; /* Current active service   */

extern sio_ioctl_param_type    dsi_sio_ioctl_param; /* Parameter used in 
                                                       calls to sio_ioctl */

extern sio_bitrate_type        ds_sio_autodetect_bitrate; /* Bitrate for 
                                                   AutoDetect (AT command) 
                                                   mode                   */

extern sio_port_id_type        ds_sio_port_id;

/*---------------------------------------------------------------------------
  Required for UI function which displays the status of the last ended
  DS call.
 --------------------------------------------------------------------------*/
extern ds_last_call_enum_type ds_last_call_var;

#if (defined (FEATURE_DS_PAP) || defined (FEATURE_DS_CHAP))
//#if (defined (FEATURE_DS_PAP_NV) || defined (FEATURE_DS_CHAP_NV))

/*---------------------------------------------------------------------------
  The following variables are used when to load default values.
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Store the length of the user_id. Used for length field in PAP config-req 
---------------------------------------------------------------------------*/
extern uint8 user_id_default_len;                   
    
/*---------------------------------------------------------------------------
  Array to store the the user_id. 
---------------------------------------------------------------------------*/
extern char user_id_default_info[]; 

/*---------------------------------------------------------------------------
  Store the length of the password. Used for password length field in PAP 
  config-req  
---------------------------------------------------------------------------*/
extern uint8 passwd_default_len;                    
    
/*---------------------------------------------------------------------------
  Array to store the password      
---------------------------------------------------------------------------*/
extern char passwd_default_info[];

#endif /* FEATURE_DS_PAP_NV */

#ifdef FEATURE_DS_QNC

/*-------------------------------------------------------------------------
  Default setting for QNC -- used when QNC NV item is not yet written.
 -------------------------------------------------------------------------*/
#ifdef	FEATURE_SKT_DS	//	khekim 00/08/08
#define DS_QNC_DEFAULT (FALSE)
#else	/* FEATURE_SKT_DS */
#define DS_QNC_DEFAULT (TRUE)
#endif	/* FEATURE_SKT_DS */

#endif /* FEATURE_DS_QNC */

#ifdef FEATURE_DS_CHOOSE_SO

/*-------------------------------------------------------------------------
  Values to select service options by set.  These are stored in NV.
  All builds with FEATURE_DS_CHOOSE_SO must support all of these
  service option sets, otherwise the NV selection will not remain
  valid from one build to the next.
  If supported service option sets are to vary from one phone to another,
  it will necessary to validate the selection before writing to NV,
  or, when used in DS, to default (with some notification) to another set.
 -------------------------------------------------------------------------*/
typedef enum
{
  DS_SO_SET_PRE707 =  0, /* IS-707.0 (pre-707) service option numbers   */
  DS_SO_SET_PROPTRY = 1, /* Qualcomm proprietary service option numbers */
  DS_SO_SET_IS707 =   2, /* IS-707 service option numbers               */
  DS_SO_SET_COUNT
} ds_so_set_type;

#endif /* FEATURE_DS_CHOOSE_SO */

/*-------------------------------------------------------------------------
  Define the originating service options according the set chosen
 -------------------------------------------------------------------------*/

#if defined(FEATURE_DS_ORIG_PRE707_SO)

#ifdef FEATURE_DS_CHOOSE_SO
  #define DS_SO_SET_DEFAULT             DS_SO_SET_PRE707
#endif
  #define DS_SO_ASYNC_DATA_13K          CAI_SO_ASYNC_DATA_13K_PRE707
  #define DS_SO_G3_FAX_13K              CAI_SO_G3_FAX_13K_PRE707
  #define DS_SO_PPP_PKT_DATA_13K        CAI_SO_PPP_PKT_DATA_13K_PRE707
  #define DS_SO_ASYNC_DATA              CAI_SO_ASYNC_DATA_PRE707
  #define DS_SO_G3_FAX                  CAI_SO_G3_FAX_PRE707
  #define DS_SO_PPP_PKT_DATA            CAI_SO_PPP_PKT_DATA_PRE707

#elif defined(FEATURE_DS_ORIG_PROPTRY_SO)

#ifdef FEATURE_DS_CHOOSE_SO
  #define DS_SO_SET_DEFAULT             DS_SO_SET_PROPTRY
#endif
  #define DS_SO_ASYNC_DATA_13K          CAI_SO_ASYNC_DATA_13K_PROPTRY
  #define DS_SO_G3_FAX_13K              CAI_SO_G3_FAX_13K_PROPTRY
  #define DS_SO_PPP_PKT_DATA_13K        CAI_SO_PPP_PKT_DATA_13K_PROPTRY
  #define DS_SO_ASYNC_DATA              CAI_SO_ASYNC_DATA_PRE707
  #define DS_SO_G3_FAX                  CAI_SO_G3_FAX_PRE707
  #define DS_SO_PPP_PKT_DATA            CAI_SO_PPP_PKT_DATA_PRE707

#elif defined(FEATURE_DS_ORIG_IS707_SO)

#ifdef FEATURE_DS_CHOOSE_SO
  #define DS_SO_SET_DEFAULT             DS_SO_SET_IS707
#endif
  #define DS_SO_ASYNC_DATA_13K          CAI_SO_ASYNC_DATA_13K_PRE707
  #define DS_SO_G3_FAX_13K              CAI_SO_G3_FAX_13K_PRE707
  #define DS_SO_PPP_PKT_DATA_13K        CAI_SO_PPP_PKT_DATA_13K_PRE707
  #define DS_SO_ASYNC_DATA              CAI_SO_ASYNC_DATA_IS707
  #define DS_SO_G3_FAX                  CAI_SO_G3_FAX_IS707
  #define DS_SO_PPP_PKT_DATA            CAI_SO_PPP_PKT_DATA_IS707

#elif defined (FEATURE_JCDMA_DS)
#error code not present
#else

  #error Must define one of FEATURE_DS_ORIG_xxx_SO.

#endif /* FEATURE_DS_ORIG_xxx_SO */

#ifdef FEATURE_ANALOG_DATA    //  ANALOG_FAX
#error code not present
#endif /* FEATURE_ANALOG_DATA  ANALOG_FAX */

/*---------------------------------------------------------------------------
  Values to select service options by rate set
 --------------------------------------------------------------------------*/
typedef enum
{
  DS_RATESET1 = 0,     /* Rate Set 1 */
  DS_RATESET2 = 1,     /* Rate Set 2 */
  DS_RATESET_COUNT
} dsi_rateset_type;

#if ( defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_DS_IS2000) )

/*-------------------------------------------------------------------------
  Type for MDR Mode. The MDR Mode determines the type of service that
  will be provided to the user for Packet Data calls.
-------------------------------------------------------------------------*/
typedef enum
{
  DS_MDR_MODE_MDR_ONLY = 0,  /* Use only MDR packet data services        */
  DS_MDR_MODE_IF_AVAIL = 1,  /* Use MDR if available, else use low-speed */
  DS_MDR_MODE_NO_MDR   = 2,  /* Use only low-speed packet data services  */
#ifdef FEATURE_DS_IS2000
  DS_MDR_MODE_SO33_PREF= 3,  /* Service Option 33 (1x channel)           */
#endif /* FEATURE_DS_IS2000 */
  DS_MDR_MODE_MAX            /* Max values of this enum                  */
} ds_mdr_mode_enum_type;

#define NUM_MDR_MODES    DS_MDR_MODE_MAX

/*-------------------------------------------------------------------------
  Default value for MDR Mode. Set to MDR if available.
-------------------------------------------------------------------------*/
#ifdef FEATURE_DS_IS2000
#define DS_MDR_MODE_DEFAULT    DS_MDR_MODE_SO33_PREF
#else 
#define DS_MDR_MODE_DEFAULT    DS_MDR_MODE_IF_AVAIL
#endif

#ifdef FEATURE_DS_IS2000
/*-------------------------------------------------------------------------
  Base station prev's that are meaningful to data.
-------------------------------------------------------------------------*/
typedef enum
{
  DS_BS_PREV_3 = 0,         /* Normal, lowspeed data; uses RLP 1         */
  DS_BS_PREV_5 = 1,         /* Base Station can do MDR, uses RLP 2       */
  DS_BS_PREV_6 = 2,         /* Base Station cad do 3G/1x; uses RLP 3     */
  DS_BS_PREV_7 = 3,         /* Release A                                 */
  DS_BS_PREV_MAX            /* Max values of this enum                   */
} ds_bs_prev_enum_type;

#define DS_NUM_BS_PREVS    DS_BS_PREV_MAX
#endif /* FEATURE_DS_IS2000 */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
/*-------------------------------------------------------------------------
  Default setting for SCRM - Reverse SCH request message enable.
 -------------------------------------------------------------------------*/
#define DS_SCRM_DEFAULT (TRUE)
#endif /* FEATURE_DS_IS2000 && FEATURE_IS2000_R_SCH */

#if defined(FEATURE_DS_IS2000) && defined(FEATURE_IS2000_R_SCH)
/*-------------------------------------------------------------------------
  Default setting for R-SCH Throttle.
 -------------------------------------------------------------------------*/
#define DS_TRTL_DEFAULT (TRUE)
#endif /* FEATURE_DS_IS2000 && FEATURE_IS2000_R_SCH */

#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR || FEATURE_DS_IS2000 */

/*---------------------------------------------------------------------------
  Masks for the function calls that toggle SIO inbound flow control (CTS/RTS)
---------------------------------------------------------------------------*/
#define DS_FLOW_ALL              0xFFFF  /* Mask to change flow by overide */
#define DS_FLOW_DSM_LARGE_Q_MASK 0x0001    /* Mask when out of large items */
#define DS_FLOW_DSM_SMALL_Q_MASK 0x0002    /* Mask when out of small items */
#define DS_FLOW_PKT_WM_MASK      0x0004 /* Flow mask for the pkt watermark */
#define DS_FLOW_TCP_SNDQ_MASK    0x0008        /* Flow mask for TCP Sendq  */
#ifdef FEATURE_DS_NET_MODEL
  #define DS_FLOW_NETMODEL_MASK  0x0010      /* the flow mask for netmodel */
#endif /* FEATURE_DS_NET_MODEL */

#ifdef	FEATURE_USE_IRDA
#define DS_FLOW_IRDA             0x0020        /* Flow mask for IrDA       */
#endif	/* FEATURE_USE_IRDA */

/*--------------------------------------------------------------------------
  Rm interface pkt call ctl states. This tracks whether Rm interface is 
  dormant or not. It is used in PZID registrations.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_PKT_NULL_STATE,            /* PPP on DTE is down                      */
  DS_PKT_DORM_IDLE_STATE,       /* DTE PPP up, traffic channel  down       */
  DS_PKT_CONNECTED_STATE        /* DTE PPP up, traffic channel  up         */
} ds_rm_pkt_call_ctl_enum_type;

extern ds_rm_pkt_call_ctl_enum_type ds_rm_pkt_call_ctl_state; /* Rm state */

#ifdef FEATURE_IS2000_R_SCH
 extern boolean ds_scrm_timer_active;
 extern boolean ds_scrm_timer_from_bs;
#endif

/*---------------------------------------------------------------------------
  Semaphores to remember if NO CARRIER and OK were sent out of the serial
  port. 
  Note, these have been added because Japan carriers want the mobile to 
  txmit NO_CARRIER & OK, if the IWF did not reflect them back.
---------------------------------------------------------------------------*/
extern boolean ds_no_carrier_txed;
extern boolean ds_ok_result_txed;

#define DS_ORIG_WITH_DATA     TRUE   /* orig data call with data to send   */
#define DS_ORIG_WITH_NO_DATA  FALSE  /* orig data call w/ no data (PZID    */
                                     /* change w/dormancy? )               */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION DSI_GET_NV_ITEM

DESCRIPTION
  This function retrieves the specified item from NV.

DEPENDENCIES
  The NV task has been started and is running.
  This functions is Non-reentrant.

RETURN VALUE
  Status returned from the NV read request.  Status can be either:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

SIDE EFFECTS
  While this function is running all other DS task activities are
  suspended except for watchdog kicking, and until the NV item is
  read in.

===========================================================================*/

extern nv_stat_enum_type dsi_get_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *item_ptr        /* Pointer where to put the item    */
);
/* <EJECT> */
/*===========================================================================
FUNCTION DSI_PUT_NV_ITEM

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  This routine is not reentrant.  Shouldn't be a problem, as it doesn't exit
  till we're done, and it's only called from the DS task.
===========================================================================*/
nv_stat_enum_type dsi_put_nv_item(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================

FUNCTION DSI_GET_CMD

DESCRIPTION
  This function will retrieve the next command from the Data task command
  queue.
  The command retrieved is copied into the command item of the passed parameter.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if a command was removed from command queue, else FALSE

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean dsi_get_cmd
(
  ds_cmd_type *cmd_ptr                /* pointer to item to copy buffer to */
);

/*===========================================================================

FUNCTION DSI_CHECK_CMD

DESCRIPTION
  This function will copy the pointer of the command at the head of the data
  command queue into the passed data command pointer. The command is not 
  removed from the data command queue.
  
  This function is useful if want to wait for a certain event to occur before
  removing the command from the command queue.

DEPENDENCIES
  An item must be on the DS command queue or an ERR_FATAL will occur.

RETURN VALUE
  TRUE if a command was checked the command queue, else FALSE

SIDE EFFECTS
  None.

===========================================================================*/

boolean dsi_check_cmd
(
  ds_cmd_type *cmd_ptr                /* pointer to item to copy buffer to */
);

/*===========================================================================

FUNCTION DSI_START_TIMER

DESCRIPTION
  This function will set the timer associated with the passed signal mask
  to the specified duration. The time units are in milli-seconds. The
  specified signal will also be cleared.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void dsi_start_timer
(
  ds_sigs_type  sig,                     /* signal associated with a timer */
  dword         duration                 /* time interval, in msecs        */
);

/*===========================================================================

FUNCTION DSI_CANCEL_TIMER

DESCRIPTION
  This function will cancel the timer associated with the specified signal
  mask. The specified signal mask will also be cleared. NOTE that the signals
  are associated with timers during initialization by the REX_DEF_TIMER 
  function.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void dsi_cancel_timer
(
  ds_sigs_type sig                       /* signal associated with a timer */
);

/*===========================================================================

FUNCTION DSI_GET_MC_CMD

DESCRIPTION
  This function will unqueue an MC command from the mc_cmd_free_q and
  set the REX-related fields to the appropriate values prior to returning the
  pointer to the link.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the initialized MC command link, or NULL

SIDE EFFECTS
  None.

===========================================================================*/

extern mc_msg_type *dsi_get_mc_cmd( void);

/*===========================================================================

FUNCTION SIGNAL_EMPTY_RX_RING_BUF

DESCRIPTION
  This function will set the DS_RXBUF_LAST_BYTE_SIG to signal the occurance
  of the Serial RX ringer buffer becoming empty.

  This function is passed via a function pointer to the RLP unit which invokes
  this function each time the External (serial) ring buffer goes empty.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void signal_empty_rx_ring_buf( void);

/*===========================================================================

FUNCTION DS_INITIATE_ORIGINATION

DESCRIPTION
  This function will perform the processing to Attempt a Data Service
  Origination. 

DEPENDENCIES
  The 'ds_srvc_mode' variable must be set to either DS_AUTODETECT_SRVC_MODE
  or DS_PKT_SRVC_MODE in order for this function to properly Originate a
  data call.

  The data_to_send parameter indicates if the call is due to there being
  data to send.  This applies to packet calls, which go dormant and then
  re-orig due to PZID changes.  All other calls will set this param to
  TRUE.

RETURN VALUE
  TRUE if an Origination was attempted, else FALSE.

SIDE EFFECTS
  None.
===========================================================================*/

extern boolean ds_initiate_origination
( 
  mc_srvc_req_enum_type srvc_type,                         /* Service Type */
  const byte *dial_string_ptr,                       /* ptr to dial string */
  boolean digit_mode,                       /* dial str type, TRUE means   */
                                            /* non-numeric ASCII in string */
  boolean                data_to_send       /* if call has data to send    */
);

/*===========================================================================

FUNCTION DSI_ANSWER_VOICE_CALL

DESCRIPTION
  This function will perform the work necessary to Answer voice call.
  
DEPENDENCIES
  The DB item answer_voice_as_data should not be set and the service type
  should be voice or analog.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void dsi_answer_voice_call( void );   

/*===========================================================================

FUNCTION DSI_ANSWER_IS99_CALL

DESCRIPTION
  This function will perform the work necessary to Answer an IS-99 data call.
  
DEPENDENCIES
  The Call State must be DS_PROCESS_PAGE_CSTATE and the incoming call must
  be for an Async Data of G3 FAX service option.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void dsi_answer_is99_call( void);

#ifdef	FEATURE_SKT_PIWF_DS	//	khekim 00/02/14
extern void dsiAnswerPiwfCall( void);
#endif	/* FEATURE_SKT_PIWF_DS */

#ifdef FEATURE_DS_DOTG_DATA
#error code not present
#endif

/*===========================================================================

FUNCTION DSI_CALL_INCOMING

DESCRIPTION
  This function will return the type of call incoming, IS-99 or AMPS.
  
DEPENDENCIES
  None.

RETURN VALUE
  DS_IS99_CALL if an IS-99 Service Option call is incoming, DS_AMPS_CALL if
  an AMPS call, else DS_NO_CALL.

SIDE EFFECTS
  None.
===========================================================================*/

extern ds_srvcmode_type dsi_call_incoming( void);

/*===========================================================================

FUNCTION DSI_END_DATA_CALL

DESCRIPTION
  This function will End a DS-involved data call. If the current call was 
  Originated or Answered by DS then DS will end the call.

  NOTE: This function does nothing if the current call is not a data-related 
        call.

DEPENDENCIES
  The current call must have been Originated or Answered by DS.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void dsi_end_data_call( void);


/*===========================================================================

FUNCTION DS_SET_DCD

DESCRIPTION
  This function is used to control the Circuit 109 (DCD) pin. It is to
  be called whenever there is a state change which may affect the pin.

  The actual state of the pin is controlled by the values of 'ds_callstate',
  'ds_srvc_mode, 'ds_atcop_amp_c_val' (the value of the AT&C parameter), 
  'ds_atcop_qcpma' (the value of the AT$QCPMA parameter), and the 
  value of the function's parameter.

  NB: The DCD pin will not necessarily be set to the value specified by
  the argument.
  
DEPENDENCIES
  All of the state variables mentioned above must be set to their new
  values prior to calling this function.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void ds_set_dcd
(
  void
) ;


/*===========================================================================

FUNCTION DSI_GET_NV_ITEMS

DESCRIPTION
  This function gets all the NV items for the Data task and stores them in
  the appropriate variables.

DEPENDENCIES
  The NV task must be started and running.

RETURN VALUE
  None

SIDE EFFECTS
 Each call to 'dsi_nv_get_item' results in a Wait and watchdog kicking
===========================================================================*/
void dsi_get_nv_items( void);

#if defined(FEATURE_DS_QNC) || defined(FEATURE_DS_CHOOSE_SO) || defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)  || defined(FEATURE_DS_IS2000)
/*===========================================================================

FUNCTION DSI_SET_SERVICE_OPTIONS

DESCRIPTION
  This function will set the local service option variables
  according to the NV items SO Set Parameter and QNC Enable.

DEPENDENCIES
  The NV items must have been set by DIAG, AT commands, or UI.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dsi_set_service_options(void);
#endif /* defined(FEATURE_DS_QNC) || defined(FEATURE_DS_CHOOSE_SO) || defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined(FEATURE_DS_IS2000)*/


/*===========================================================================

FUNCTION DSI_ORIG_OK_CHECK

DESCRIPTION
  This function will determine if it's OK to originate a data call and return
  the status of the check.

DEPENDENCIES
  The DB items accessed below must have previously been initialized.

RETURN VALUE
  An enum representing the reason for failure or DS_OK_TO_ORIG.

SIDE EFFECTS
  None.
===========================================================================*/
dsi_orig_chk_enum_type dsi_orig_ok_check //LGHACK
(
  boolean cdma_required               /* CDMA service required */
);

/*===========================================================================

FUNCTION DS_SET_INBOUND_FLOW

DESCRIPTION
  This function is controls the enabling or diabling of inbound flow ctl.
  This function was required because inbound flow ctl. can be changed 
  from several places for different reasons, and we need an arbitrator
  function to handle those changes.
  
  Basically, the function enables inbound flow, when no caller wants it
  disabled. If even one caller wants the flow disabled, the inbound flow
  is disabled.
  

DEPENDENCIES
  This is the only function that should be used to toggle the inbound flow.
  This is important because the function keeps track of the state of inbound
  flow, and if the state of inbound flow is changed


RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_set_inbound_flow
(
  word in_mask,                          /* input mask of calling function */
  boolean enable         /* specifies if flow is to be enabled or disabled */
);

/*===========================================================================

FUNCTION DS_CLR_ERR_LOG

DESCRIPTION
  This function clears the NV error log whenever the command AT$QCCLR is 
  issued.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_clr_err_log( void);


/*===========================================================================

FUNCTION DS_CLOSE_SERIAL_PORT

DESCRIPTION
  This function switches the serial port to autodetect mode, without changing
  the service mode in dsmgr.
  
  This function provides an external interface to the AT command processor to
  close the serial port, when it detects a NO CARRIER result code.
  The functionality was added because earlier NO CARRIER was sent before the
  protocols and the call came down completely. As a result AT commands for 
  the next call were lost. This has been fixed by switching the serial port
  to Autodetect while leaving dsmgr in Rawdata.

DEPENDENCIES
  dsi_srvc_mode should be DS_RAWDATA_SRVC_MODE.

RETURN VALUE
  None

SIDE EFFECTS
  Changes value of the global semaphore dsi_serial_set_to_autodetect.
===========================================================================*/
void ds_close_serial_port( void);

/*==========================================================================
  
FUNCTION DSI_WAIT

DESCRIPTION 
  This function performs the Data task waiting function. This function will
  not return until a signal from the requested mask is set or a queue
  associated with a requested signal contains an item. Checks are made to
  insure that signals associated with queues are not set without items in
  the queue. 
  The watchdog is also kicked at least once.

DEPENDENCIES 
  None

RETURN VALUE 
  A signal mask respresenting the set sigs or non-empty queues for the
  requested signals

SIDE EFFECTS
  None

===========================================================================*/
ds_sigs_type dsi_wait
(
  ds_sigs_type requested_mask               /* mask of signals to wait for */
);                 

#if (defined (FEATURE_DS_PAP) || defined(FEATURE_DS_CHAP) )
/*===========================================================================

FUNCTION DS_WRITE_AUTH_INFO

DESCRIPTION
  This function writes the user-id and password from the pointers given in 
  function arguments into the ppp data structure. The ppp data structure 
  stores the pasword and user_id to transmit to base station in the 
  Auth-Req packet.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void ds_write_auth_info
(
  uint8 user_id_length,   /* The id length field in PAP config-req         */
  char *user_id,          /* The peer id                                   */
  uint8 password_length,  /* The passwd len field in PAP Congig-Req        */
  char *password          /* The password of the peer                      */
);

#endif /* FEATURE_DS_PAP || FEATURE_DS_CHAP */

/*===========================================================================

FUNCTION DS_SET_RM_PKT_CALL_CTL_STATE

DESCRIPTION
  This function is called whenever the ds_rm_pkt_call_ctl_state variable
  changes.
  
  The ds_rm_pkt_call_ctl_state variable tries to map the packet data service
  call control states of the mobile station as described in IS-707.A.5, 
  sec. 1.4.3.2. It only implements a small, useful subset of the states in
  that chapter.
  
  The variable is set and housekeeping done for each call_ctl state.

  Valid protocol states are-
  DS_PKT_NULL_STATE  - PPP on DTE is down.
  DS_PKT_DORM_IDLE_STATE - PPP on DTE is up and traffic channel is down.
  DS_PKT_CONNECTED_STATE - PPP on DTE is up, traffic channel is up.

  Any change to this state variable should be through this function only.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Changes the global ds_rm_pkt_call_ctl_state

===========================================================================*/
void ds_set_rm_pkt_call_ctl_state
(
  ds_rm_pkt_call_ctl_enum_type new_rm_call_ctl_state          /* new state */
);

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION DS_SCRM_REG_FUNCS

DESCRIPTION
  Register functions with MC which are used for SCRM processing.

DEPENDENCIES
  Assumes MC has already started.  Called only at powerup.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_scrm_reg_funcs(void);
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION     DS_SCRM_SCH_CHNG_CBACK

DESCRIPTION  Called by MC when there is a configuration change to the 
             SCH's.  This triggers DS to see if another SCRM should be
             sent out.
             
             Also called by DS/SIO/RLP when the RLP 3 Tx watermarks
             are particularly empty or particularly full, indicating
             that a change to the R-SCH may be required.
             
             If the timer is inactive, trigger the signal by setting for
             a zero-second timer.

DEPENDENCIES None.

RETURN VALUE None.

SIDE EFFECTS None.
===========================================================================*/
void ds_scrm_sch_chng_cback(void);
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION DS_SCRM_RETRY_PROCESSING

DESCRIPTION
  Called when the SCRM retry timer goes off.  If a SCRM request is pending,
  then a cmd is sent to MC asking for a SCRM to be sent.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_scrm_retry_processing(void);
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_DS_IS2000
/*===========================================================================
FUNCTION DS_SOCTL_RETRY_DELAY_CBACK

DESCRIPTION
  Called from MC whenever MC receives a message from the base station
  indicating what the retry callback should be.  The timer is a one-shot
  timer.
  
  Timer is immediately set to the value of the timer.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_soctl_retry_delay_cback
(
  retry_delay_type retry_parm
);
#endif /* FEATURE_DS_IS2000 */

/*===========================================================================
FUNCTION DSMGR_START_TIMER

DESCRIPTION
  This function will set the timer associated with the passed signal mask
  to the specified duration. The time units are in milliseconds. The
  specified signal will also be cleared.
  The specified signal is removed from the signaled events mask as a 
  precaution against race conditions in which the two signals are set in the
  ds tcb at a time. If a timer is started for a particular signal, the 
  signal should not be acted upon for the present set of signals in the ds 
  tcb. Thus, signaled events_mask needs to be cleared.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  dsi_requested_sig_mask and dsi_signaled_events_mask are changed.

===========================================================================*/
void dsmgr_start_timer
(
  ds_sigs_type  sig,                     /* signal associated with a timer */
  dword         duration                 /* time interval, in msecs        */
);

/*===========================================================================
FUNCTION DSMGR_CANCEL_TIMER

DESCRIPTION
  This function will cancel the timer associated with the specified signal
  mask. The specified signal mask will also be cleared. NOTE that the 
  signals are associated with timers during initialization by the 
  REX_DEF_TIMER function.
  The specified signal is also removed from the signaled events mask and the
  requested signal mask.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  dsi_requested_sig_mask and dsi_signaled_events_mask are changed.

===========================================================================*/
void dsmgr_cancel_timer
(
  ds_sigs_type sig                       /* signal associated with a timer */
);


#ifdef FEATURE_DS_RLP3
/*===========================================================================
FUNCTION DS_END_CALL_UPON_SYNC_TIMEOUT

DESCRIPTION
  This function will end the call. It is called upon a sync timeout from
  the rlp code.  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ds_end_call_upon_sync_timeout(void);
#endif /* FEATURE_DS_RLP3 */


#endif /* FEATURE_DS */
#endif /* DSI_H */

