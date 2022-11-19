#ifndef DS_H
#define DS_H
/*===========================================================================

                   D A T A   S E R V I C E   T A S K   
                        H E A D E R    F I L E

DESCRIPTION

  This is the header file for the Data Services Task. Contained herein are
  all the definitions, functions, and structures for other software tasks
  to communicate with the Data Task.


 Copyright (c) 1993-1998 by QUALCOMM Incorporated.  All Rights Reserved.
 Copyright (c) 1999,2000,2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   O:/src/asw/COMMON/vcs/ds.h_v   1.10   28 Feb 2001 01:04:08   akhare  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/22/01   ak      Moved case statements around so that SO33 is always first.
                   This helps w/performance in IS2000.
02/19/01   dwp     Rename DS_DISABLE_DATA_MODE_CMD to DS_DISBALE_SIO_CMD. Add
                   DS_ENABLE_SIO_CMD.
02/12/01   rmd     Removed ds_scrm_need_27MHZ function definition. This 
                   functionally is now supported in mclk_XXXX.c
02/02/01   na      Removed references to FEATURE_DS_CM.
01/25/01   na/ych  Removed FEATURE_NEW_SIO - it is always turned on now. Added
                   JCDMA support                     
01/18/01   va      Took care of some compile errors when compiling under WINNT.
11/14/00   dwp     Added support for FEATURE_RUNTIME_DEVMAP.
11/07/00   tl/ak   Added extern for ds_scrm_max_allowable_sch_rate.
10/18/00   ak      Add ds_scrm_call_init, since now called from TDSO.
09/12/00   jd      Merge PPG r1.15 changes to tiger - adds definitions of
                   DS_BT_DEVICE_ENABLE_CMD and DS_BT_DEVICE_DISABLE_CMD
08/25/00   rc      Added prototypes for FEW, MANY and DONT_EXCEED memory
                   events.
07/24/00   ak      Added include for cai.h to support 27Mhz function.
07/24/00   ak      Added extern for DS_SCRM_NEED_27MHz function.
07/07/00   ak      Added a signal for the SCRM retry timer.
06/05/00   ak      Added extern for callback function for rlp 3, to indicate
                   dormancy conditions.
04/11/00   mvl     Added the DS_CM_UI_CALL_RELEASE_CMD to indicate when a
                   release occurs due to the end key is pressed.
11/30/99   na      Added defines for dsrlp operation.
11/01/99   rc      Added DS_CM_CALL_RELEASE_CMD for notification of base 
                   station release.
09/22/99   rc      Incorporated code review changes and comments.
                   Changed DS_ENTER_UP_MODE_CMD to DS_ENTER_BROWSER_MODE_CMD
                   Changed DS_EXIT_UP_MODE_CMD to DS_EXIT_BROWSER_MODE_CMD.
09/01/99   smp     Added macro DS_ISDATA_IS707(so) to check whether or not a
                   service option belongs to the IS-707 service option set.
08/20/99   na      Added DS_PZID_CHANGE_CMD for pkt zone id related changes
                   and added ds_pzid_reconnect_cb() prototype
08/18/99   ak      Made fixes so compiles when FEATURE_DS turned off.
08/11/99   ak/rc   For sockets - added fade event for CM, a new RE-orig cmd,
                   and externs for the RLP wm empty/non-empty events.
07/23/99   mvl/rc  Changed the extern declaration of dsi_cm_client to be
                   consistant with new CM usage.
07/23/99   mvl/rc  Added call manager support to this file. 
02/23/99   hcg     Added DSSock sockets interface commands.
02/10/99   smp     Put MDR changes under FEATURE_SPECIAL_MDR.
01/15/99    ak     Re-integrated in the old SIO under a compile
                   time switch.
01/13/99   smp     Added MDR service options to data services macros. Added
                   prototype for ds_rlp_rx_process_frames() and included
                   mdrrlp.h (only if FEATURE_IS95B_MDR is defined).
01/09/99   jjw     Changed to generic browser interface
10/27/98   ldg     ARM porting: enum commas.
10/21/98   dbd     Added #ifdef FEATURE_ANALOG_FAX
10/15/98   dbd     Added MC_ANALOG_FAX_SRVC to mc_srvc_req_enum_type
07/23/98   ldg     Added to macros the new numbers for analog end to end fax
                   (from IS-707A).
06/16/98   na      Removed ds_frame_item_type.
05/29/98   dbd     Added SSS analog data commands to enum
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
12/17/97   ldg     Moved DS stats accessor function prototype and typedef
                   from dsmgr.h.
                   Added three generic temp variables for debugging.
11/04/97   ldg     Added macros for determinining whether a given service
                   option is data, or a given rateset, or a given service
                   type (async, fax, or packet).
                   Added a command to support QNC.
                   Added prototype for ds_process_rlp_rx_frame.
10/24/97   na      Added support for Unwired Planet application timeout based
                   dormant mode.
01/23/97   na      Added support for UI function that returns last status
10/17/96   jjw     Added UP mode interface definitions
05/24/96   jjw     Gemini & ISS2 Data Services release.
11/09/95   jjw     Completed changes to support fully compliant IS-99
07/13/95   jjw     Changes to support IS-99 protocol integration
10/16/94   jjw     Upgrade to IS-99 data service: signal definitions, command
12/13/94   jjw     Added parameter to ALERT cmd.
08/11/94   jjw     renamed a type, code review upgrades
04/21/94   jjw     I dunno
08/09/93   jjw     Initial version of file


===========================================================================*/


#include "comdef.h"
#include "customer.h"                        /* Customer specific features */
#include "dsm.h"
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
#include "mdrrlp.h"
#else
#include "rlp.h"
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */
#ifndef T_WINNT
#include "tcp.h"
#endif /*T_WINNT*/
#include "cm.h"
#ifdef FEATURE_DS_IS2000
#include "cai.h"
#endif
#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01
#include	"nv.h"
#endif	/* FEATURE_SKT_PTPAPI */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
  Data Services Task Signal Definitions
---------------------------------------------------------------------------*/

/* Serial Port signals */

#define DS_1ST_SIO_RX_SIG       0x0001   /* 1st byte into Serial RX buffer */
#define DS_SIO_RX_PKT_SIG       0x0002   /* Protocol Packet rx'd signal    */
#define DS_SIO_ESCAPE_SIG       0x0004   /* AT escape Sequence rx'd signal */
#define DS_DTR_CHANGED_SIG      0x0008   /* DTR Pin changed state Signal   */

/* Protocol-related signals */

#define DS_PS_RX_Q_SIG          0x0010  /* Data packet(s) from PS task sig */
#define DS_RLP_IDLE_TIMEOUT_SIG 0x0020  /* RLP Link Layer inactivity signal*/

/* Intertask Command & Generic task signals */

#define DS_STATE_TIMER_SIG      0x0040   /* State-related Processing signal*/
#define DS_CMD_Q_SIG            0x0080   /* Command Rx'd signal            */
#define DS_DOG_RPT_TIMER_SIG    0x0100   /* Watchdog Report timeout signal */
#define DS_NV_CMD_SIG           0x0200   /* NV Item retrieval signal (???) */


/* Signal used in synchronizing with SIO */
#define DS_SIO_CONTROL_SIG      0x0400   /* Indication from SIO that control
                                            of SIO may be transferred to 
                                            another                        */
                                            
#ifdef FEATURE_IS2000_R_SCH
#define DS_SCRM_RETRY_TIMER_SIG 0x0800   /* SCRM retry Timer signal        */
#endif

/* Currently NOT USED signals                                              */
#ifdef	FEATURE_IRDA_UPGRADE
#define DS_IRDA_SIG             0x1000
#else
#define DS_RSVD_1000_SIG        0x1000      /* not used         */
#endif	/* EATURE_IRDA_UPGRADE */
#define DS_RSVD_2000_SIG        0x2000      /* TASK_OFFLINE_SIG */
#define DS_RSVD_4000_SIG        0x4000      /* TASK_STOP_SIG    */
#define DS_RSVD_8000_SIG        0x8000      /* TASK_START_SIG   */

/*---------------------------------------------------------------------------
 Flow control type.  Order is largely dictated by standard.                                                       
---------------------------------------------------------------------------*/                  
typedef enum
{
  DS_FCTL_OFF,                             /* Flow control disabled.       */
  DS_XONXOFF_STRIP_FCTL,                   /* Use XON/XOFF flow control 
                                              but strip XON/XOFF characters
                                              from stream.                 */
  DS_CTSRFR_FCTL,                          /* Hardware flow control.       */
  DS_XONXOFF_NSTRIP_FCTL,                  /* Use XON/XOFF flow control 
                                              and leave XON/XOFF characters
                                              in stream.                   */
  DS_MAX_FLOW                              /* For bounds checking only.    */
} ds_flow_ctl_type;                

typedef enum
{
  MC_ASYNC_SRVC,
  MC_FAX_SRVC,
  MC_PKT_SRVC,
#ifdef	FEATURE_SKT_PIWF_DS	//	khekim	00/1/21
  MC_PIWF_SRVC,
#endif /* FEATURE_SKT_PIWF_DS */
#ifdef	FEATURE_SKT_ATPKT_DS //	khekim 00/07/12
  MC_ATPKT_SRVC,
#endif /* FEATURE_SKT_ATPKT_DS */
#ifdef	FEATURE_SKT_DS	//	00/1/21
  MC_SKT_PKT_END_SRVC,
#endif /* FEATURE_SKT_DS */
  MC_AMPS_DATA_SRVC,
  MC_CURR_VOICE_SRVC,
  MC_CDMA_VOICE_SRVC,
  MC_AMPS_VOICE_SRVC
#ifdef FEATURE_ANALOG_FAX
#error code not present
#endif /* FEATURE_ANALOG_FAX */  
} mc_srvc_req_enum_type;


/*---------------------------------------------------------------------------
  Data Task Command Definitions. These are the commands that can be sent to
  one.
---------------------------------------------------------------------------*/

typedef enum
{
  /* Call Control Commands */
  DS_ALERT_CMD,             /* Notification of incoming (Page) data call   */
  DS_STOP_ALERT_CMD,        /* Cancellation of incoming (Page) data call   */
  DS_CALL_CONNECTED_CMD,    /* Successful Data Origination response        */
  DS_CALL_FAILED_CMD,       /* Unsuccessful Data Origination response      */
  DS_RELEASE_CMD,           /* Notification of Data Call end               */
  DS_ENABLE_DS_TASK_CMD,    /* 5:Command DS Task from Disabled to Idle mode*/
  DS_RDM_OPEN_CMD,          /* used for Runtime Devmap issued open cmd     */
  DS_ENABLE_SIO_CMD,        /* Tells DS to open a SIO;used w/Runtime Devmap*/
  DS_DISABLE_SIO_CMD,       /* Disable (close) the SIO of the DS Task      */
  DS_CM_ALERT_CMD,          /* Notification of incoming (Page) data call   */
  DS_CM_STOP_ALERT_CMD,     /* Cancellation of incoming (Page) data call   */
  DS_CM_CALL_CONNECTED_CMD, /* Successful Data Origination response        */
  DS_CM_CALL_ENDED_CMD,     /* 10: Notification of Data Call end           */
  DS_CM_UI_CALL_RELEASE_CMD,/* Notification of Client Call Release [end]   */
  DS_ENTER_BROWSER_MODE_CMD,/* Enter Browser Mode                          */
  DS_EXIT_BROWSER_MODE_CMD, /* Exit Browser Mode                           */
  DS_ORIG_NET_CALL_CMD,     /* Start a Browser protocol session            */
  DS_RELEASE_NET_CALL_CMD,  /* 15: End a Browser protocol session          */
  DS_REORIG_WAP_CMD,        /* For WAP reorigination                       */
  DS_PZID_WAP_CMD,
  DS_TOGGLE_QNC_ENABLE_CMD, /* Enable or disable QNC                       */
  DS_PROCESS_DSP_REQ_CMD,   /* ADSP Requests processing signal             */
  DS_INBND_FLOW_DISABLE_CMD,/* Inbound flow disable                        */
  DS_INBND_FLOW_ENABLE_CMD, /* Inbound flow enable                         */
  DS_TX_DATA_READY_CMD,     /* 20: TX data from PS is ready                */
  DS_ENTER_SOCKET_MODE_CMD, /* Enter Socket Mode                           */
  DS_EXIT_SOCKET_MODE_CMD,  /* Exit Socket Mode                            */
  DS_ORIG_SOCKET_NET_CMD,   /* Originate PPP/Traffic channel               */
  DS_REORIG_SOCKET_NET_CMD, /* ReOriginate Traffic channel                 */
  DS_RELEASE_SOCKET_NET_CMD,/* 25: Gracefully Terminate network            */
  DS_ABORT_SOCKET_NET_CMD,  /* Abort the network                           */
  DS_PZID_CHANGE_CMD,       /* Pkt Zone ID changed - originate pkt call    */
  DS_SET_F92_CMD            /* Sets F92 mode when JCDMA_DS is turned on    */
#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01
  ,DS_ENTER_AIRDATA_MODE_CMD /* Enter AirData Mode                         */
  ,DS_EXIT_AIRDATA_MODE_CMD  /* Exit AirData Mode                          */
  ,DS_ORIG_AIRDATA_CALL_CMD  /* Start a AirData protocol session            */
#endif	/* FEATURE_SKT_PTPAPI */
#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
  ,DS_DTR_CHANGED_CMD
#endif	/* FEATURE_SKT_DTRSENSE_DS */
#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
  ,DS_ENABLE_SERIAL_CMD
#endif	/* FEATURE_SKT_ENABLE_SERIAL */
#ifdef FEATURE_SKT_EIF  // redstar 2001.012.13
	,DS_EIF_MO_OK
	,DS_EIF_MO_NG
	,DS_EIF_MT_NEW
	,DS_EIF_MT_NEW2
	,DS_EIF_KEY_SCAN
#endif /* FEATURE_SKT_EIF */
#ifdef	FEATURE_SKT_CANCEL_CALL	//	khekim 00/12/04
  ,DS_CANCEL_CALL_CMD
#endif	/* FEATURE_SKT_CANCEL_CALL */
} ds_cmd_name_type;

#ifdef	FEATURE_IRDA_UPGRADE
#define	DS_IRDA_TIMER_0_TIMEOUT_CMD_SIG		0x0001
#define	DS_IRDA_TIMER_1_TIMEOUT_CMD_SIG		0x0002
#define	DS_IRDA_PROTOCOL_CMD_SIG			0x0004
#define	DS_IRDA_GET_MORE_RX_CMD_SIG			0x0008
#endif	/* FEATURE_IRDA_UPGRADE */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

/*---------------------------------------------------------------------------
  Macros to determine properties of a service option without referring
  explicitly to service option numbers.  This keeps code generic as we
  add support for additional service options.
---------------------------------------------------------------------------*/

/* Is it a rateset 1 (Pre-707) data s.o.? */

#define DS_ISDATA_RS1_PRE707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_PRE707) || \
   ((so) == CAI_SO_ASYNC_DATA_PRE707) || \
   ((so) == CAI_SO_G3_FAX_PRE707))

#define DS_CASE_DATA_RS1_PRE707 \
  case CAI_SO_PPP_PKT_DATA_PRE707: \
  case CAI_SO_ASYNC_DATA_PRE707: \
  case CAI_SO_G3_FAX_PRE707:

/* Is it a rateset 1 (IS-707) data s.o.? */

#define DS_ISDATA_RS1_IS707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_IS707) || \
   ((so) == CAI_SO_ASYNC_DATA_IS707) || \
   ((so) == CAI_SO_G3_FAX_IS707))

#define DS_CASE_DATA_RS1_IS707 \
  case CAI_SO_PPP_PKT_DATA_IS707: \
  case CAI_SO_ASYNC_DATA_IS707: \
  case CAI_SO_G3_FAX_IS707:

/* Is it a rateset 2 (Pre-707) data s.o.? */
#define DS_ISDATA_RS2_PRE707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_13K_PRE707) || \
   ((so) == CAI_SO_ASYNC_DATA_13K_PRE707) || \
   ((so) == CAI_SO_G3_FAX_13K_PRE707))

#define DS_CASE_DATA_RS2_PRE707 \
  case CAI_SO_PPP_PKT_DATA_13K_PRE707: \
  case CAI_SO_ASYNC_DATA_13K_PRE707: \
  case CAI_SO_G3_FAX_13K_PRE707:

/* Is it a rateset 2 (QC proprietary) data s.o.? */

#define DS_ISDATA_RS2_QC_PROPTRY(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_13K_PROPTRY) || \
   ((so) == CAI_SO_ASYNC_DATA_13K_PROPTRY) || \
   ((so) == CAI_SO_G3_FAX_13K_PROPTRY))

#define DS_CASE_DATA_RS2_QC_PROPTRY \
  case CAI_SO_PPP_PKT_DATA_13K_PROPTRY: \
  case CAI_SO_ASYNC_DATA_13K_PROPTRY: \
  case CAI_SO_G3_FAX_13K_PROPTRY:

/* Is it a IS-707 data s.o.? */
#ifdef	FEATURE_SKT_DS //	khekim 00/01/31
#define DS_ISDATA_IS707(so) \
  ( (CAI_SO_DATA == (so)) || (DS_ISDATA_RS1_PRE707(so)) || \
    (DS_ISDATA_RS1_IS707(so)) || (DS_ISDATA_RS2_PRE707(so)) )
#else	
#define DS_ISDATA_IS707(so) \
  ( (DS_ISDATA_RS1_IS707(so)) || (DS_ISDATA_RS2_PRE707(so)) )
#endif	/* FEATURE_SKT_DS */
/* Note: 13K IS-707 so's are same as 13K Pre-707 so's */
  
#ifndef FEATURE_ANALOG_FAX  
/*  MACROS for non- FEATURE_ANALOG_FAX targets  */ 

/* Is it a rateset 1 data s.o.? */

#ifdef FEATURE_DS_IS2000
#define DS_ISDATA_RS1(so) \
  ( \
   (CAI_SO_PPP_PKT_DATA_3G == (so)) || \
    (CAI_SO_DATA == (so)) || \
    DS_ISDATA_RS1_PRE707(so) || \
    DS_ISDATA_RS1_IS707(so) || \
    (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) \
  )
#else
#define DS_ISDATA_RS1(so) \
  ( \
    (CAI_SO_DATA == (so)) || \
    DS_ISDATA_RS1_PRE707(so) || \
    DS_ISDATA_RS1_IS707(so) || \
    (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) \
  )
#endif /* FEATURE_DS_IS2000 */

#ifdef FEATURE_DS_IS2000
#define DS_CASE_DATA_RS1 \
     case CAI_SO_PPP_PKT_DATA_3G: \
     case CAI_SO_DATA: \
     DS_CASE_DATA_RS1_PRE707 \
     DS_CASE_DATA_RS1_IS707 \
     case CAI_SO_MDR_PKT_DATA_FRS1_RRS1:
#else
#define DS_CASE_DATA_RS1 \
     case CAI_SO_DATA: \
     DS_CASE_DATA_RS1_PRE707 \
     DS_CASE_DATA_RS1_IS707 \
     case CAI_SO_MDR_PKT_DATA_FRS1_RRS1:
#endif /* FEATURE_DS_IS2000 */

/* Is it a rateset 2 data s.o.? */

#define DS_ISDATA_RS2(so) \
  ( \
    DS_ISDATA_RS2_PRE707(so) || \
    DS_ISDATA_RS2_QC_PROPTRY(so) || \
    (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)) \
  )

#define DS_CASE_DATA_RS2 \
     DS_CASE_DATA_RS2_PRE707 \
     DS_CASE_DATA_RS2_QC_PROPTRY \
     case CAI_SO_MDR_PKT_DATA_FRS2_RRS2:

#else 
#error code not present
#endif  /*  FEATURE_ANALOG_FAX  */

/* Is it a data s.o.? */

#define DS_ISDATA(so) \
  (DS_ISDATA_RS1(so) || DS_ISDATA_RS2(so))

#define DS_CASE_DATA \
  DS_CASE_DATA_RS1 \
  DS_CASE_DATA_RS2

/* Is it a {fax,async,packet} data s.o.? */

#define DS_ISDATA_G3_FAX(so) \
  ((CAI_SO_G3_FAX_PRE707 == (so)) || \
   (CAI_SO_G3_FAX_IS707 == (so)) || \
   (CAI_SO_G3_FAX_13K_PRE707 == (so)) || \
   (CAI_SO_G3_FAX_13K_PROPTRY == (so)))

#define DS_ISDATA_ASYNC(so) \
  ((CAI_SO_ASYNC_DATA_PRE707 == (so)) || \
   (CAI_SO_ASYNC_DATA_IS707 == (so)) || \
   (CAI_SO_ASYNC_DATA_13K_PRE707 == (so)) || \
   (CAI_SO_ASYNC_DATA_13K_PROPTRY == (so)))

#ifdef FEATURE_DS_IS2000
#define DS_ISDATA_PPP_PKT(so) \
  ((CAI_SO_PPP_PKT_DATA_3G == (so)) || \
   (CAI_SO_DATA == (so)) || \
   (CAI_SO_PPP_PKT_DATA_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_IS707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PROPTRY == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)))
#else
#define DS_ISDATA_PPP_PKT(so) \
  ((CAI_SO_DATA == (so)) || \
   (CAI_SO_PPP_PKT_DATA_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_IS707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PROPTRY == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)))
#endif /* FEATURE_DS_IS2000 */

#define DS_ISDATA_RLP_TYPE2(so) \
     ( (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) || \
       (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)) )

#define DS_ISDATA_MDR(so)    DS_ISDATA_RLP_TYPE2(so)

#ifdef FEATURE_DS_IS2000
#define DS_ISDATA_RLP_TYPE3(so) \
     ( (CAI_SO_PPP_PKT_DATA_3G == (so)) )

#define DS_ISDATA_3G(so)     DS_ISDATA_RLP_TYPE3(so)
#endif /* FEATURE_DS_IS2000 */

#ifdef FEATURE_ANALOG_FAX
#error code not present
#endif

/*---------------------------------------------------------------------------
  The following enum is required for the UI ds_last_call_status function.
---------------------------------------------------------------------------*/
typedef enum
{
  DS_LAST_ESTABLISHED,           /* Last call was established              */
  DS_LAST_RLP_NOT_ESTABLISHED,   /* RLP did not sync up in last call       */
  DS_LAST_TCP_NOT_ESTABLISHED   /* TCP did not establish in the last call  */
} ds_last_call_enum_type;


/*---------------------------------------------------------------------------
  Complete Command data type. Contains fields for all data needed to send 
  commands to the Data Task
---------------------------------------------------------------------------*/
typedef struct 
{
  ds_cmd_name_type cmd_name;   /* actual command for Data Task to process  */
  union
  {
    word                  srv_opt;   /* Service Option for ALERT command   */
#ifdef T_WINNT
#error code not present
#else
    ip_addr_type          ip_addr;   /* IP address for NET calls           */
#endif
#ifdef	FEATURE_SKT_DTRSENSE_DS	//	khekim 00/02/22
	byte				dtrValue;
#endif	/* FEATURE_SKT_DTRSENSE_DS */
#ifdef	FEATURE_SKT_PTPAPI	//	khekim 00/03/01
	byte				dialStr[NV_MAX_DIAL_DIGITS+1];
#endif	/* FEATURE_SKT_PTPAPI */
#ifdef	FEATURE_SKT_ENABLE_SERIAL	//	khekim 00/07/26
	byte				bEnableSerial;
#endif	/* FEATURE_SKT_ENABLE_SERIAL */
#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */
  } parms;
} ds_cmd_type;


/*---------------------------------------------------------------------------
  Shared Watermark for DS-to-PS interface. Queue for PS-to-DS interface.
---------------------------------------------------------------------------*/
extern dsm_watermark_type ds_to_ps_wm_item;
extern q_type             ds_from_ps_q;
#if defined( FEATURE_DS_RLP1) || defined( FEATURE_DS_RLP2) || \
    defined( FEATURE_DS_RLP3)
extern dsm_watermark_type ds_rlp_to_ps_wm_item;        /* From RLP to PPP  */
#endif /* Any of new RLPs defined */

/*--------------------------------------------------------------------------
  DSRLP specific globals. To keep track of sr_id and the receive and tx
  watermarks.
---------------------------------------------------------------------------*/
#if defined( FEATURE_DS_RLP1) || defined( FEATURE_DS_RLP2) || \
    defined( FEATURE_DS_RLP3)
extern byte                ds_curr_sr_id;    /* Curent service sr_id       */
extern dsm_watermark_type* ds_rlp_tx_wm_ptr; /* Txmit watermark for RLP    */
extern dsm_watermark_type* ds_rlp_rx_wm_ptr; /* Receive watermark for RLP  */
#endif /* Any of new RLPs defined */

/*---------------------------------------------------------------------------
  General-purpose temporary variables for debugging
---------------------------------------------------------------------------*/
extern word ds_temp1;
extern word ds_temp2;
extern word ds_temp3;

/*---------------------------------------------------------------------------
  Support for the call manager
---------------------------------------------------------------------------*/
extern cm_client_id_type dsi_cm_client;

#ifdef FEATURE_DS_DEBUG_UI
/*---------------------------------------------------------------------------
  Enumeration of DS statistics and state items.
---------------------------------------------------------------------------*/
typedef enum {
  DS_STATS_PROTOCOL_STATE,
  DS_STATS_SRVC_MODE,
  DS_STATS_CALLSTATE,
  DS_STATS_COUNT
} ds_stats_enum;
#endif /* FEATURE_DS_DEBUG_UI */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/




/*===========================================================================

FUNCTION DS_CMD

DESCRIPTION
  This function is the interface function for the data task. Call this 
  function with a command (or RLP RX data frame) and the appropriate
  processing will occur. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

#ifdef	FEATURE_USE_IRDA
extern boolean ds_cmd
#else	/* FEATURE_USE_IRDA */
extern void ds_cmd
#endif	/* FEATURE_USE_IRDA */
(
  ds_cmd_type *cmd_ptr                      /* pointer to the command data */
);

/*===========================================================================

FUNCTION DS_TASK

DESCRIPTION
  This function is the entry point and main processing loop for the Data
  Service(s) task.

DEPENDENCIES
  None.

RETURN VALUE
  Does not Return.

SIDE EFFECTS
  None.

===========================================================================*/

extern void ds_task
( 
  dword dummy           /* Parameter required for REX. Tell lint to ignore */
                        /* lint -esym(715,dummy) */
); 

/*===========================================================================

FUNCTION DS_LAST_CALL_STATUS

DESCRIPTION
  This function is for the UI to display the status of the just ended
  data call.

DEPENDENCIES
  None.

RETURN VALUE
  Status of the last ended call.
  DS_LAST_ESTABLISHED         : if call was established.
  DS_LAST_RLP_NOT_ESTABLISHED : if RLP did not get established.
  DS_LAST_TCP_NOT_ESTABLISHED : if TCP did not get established.

SIDE EFFECTS
  None.
===========================================================================*/
ds_last_call_enum_type ds_last_call_status
(
  void
);

/*===========================================================================

FUNCTION DS_PROCESS_RLP_RX_FRAMES

DESCRIPTION
  This function invokes the RLP RX function to process RLP frames received
  over the air, and terminates the call if the RLP protocol fails to
  synchronize. For packet data calls, it also maintains the dormancy state
  based on the return value from the RLP RX processing,

  This function should be called from the RX task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ds_process_rlp_rx_frames
(
#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR)
  rlp_fwd_frame_block_type  *fwd_frame_block  /* Frame block containing rx'ed
                                                 fundamental and supplemental
                                                 RLP frames */
#else
  rlp_rate_enum_type frame_rate,              /* Rate of RLP frame         */
  boolean            pri_chan,                /* Primary Channel indicator */
  dsm_item_type      *rlp_item_ptr            /* ptr to RLP frame item     */
#endif
);

#if (defined( FEATURE_DS_RLP1) || defined( FEATURE_DS_RLP2) || \
     defined( FEATURE_DS_RLP3))
/*===========================================================================
FUNCTION      DS_RLP_DATA_RXED

DESCRIPTION   This function is called from the rlp layer, after the frames
              received in a 20 msec period have been processed.  If the
              input parameter is FALSE, then no data/ctl/init frames have
              been received in the past 20 msec.  Otherwise at least one
              data frame was received.  If data has been received, then the
              idle timer should be reset, so that it doesn't expire.
  
              This function should be called from the RX task.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_rlp_data_rxed
(
  boolean     data_frs_rxed      /* tells if any non-idle/fill frames rxed */
);
#endif /* any rlp */

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

FUNCTION DS_RLP_TX_GONE_EMPTY_EVENT

DESCRIPTION
  This function performs the 'Now Empty' event processing for the PACKET
  DATA SERVICES RLP Tx watermark. This function will determine if the 
  Idle Timer should be invoked.

  This function is called when the outgoing Tx RLP watermark goes
  empty. If the mode is socket and the dormant timeout is enabled then 
  the Idle timer is started.

DEPENDENCIES
  DS must be in Packet Service mode.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_rlp_tx_gone_empty_event(void);

#endif /* FEATURE_DS_SOCKETS */

#ifdef FEATURE_DS_SOCKETS
/*===========================================================================

FUNCTION DS_RLP_TX_NON_EMPTY_EVENT

DESCRIPTION
  This function performs the 'Non Empty' event processing for the PACKET
  DATA SERVICES shared queue.

  This function is invoked when the first item is added to a previously
  empty RLP Tx watermark.

DEPENDENCIES
  DS must be in Packet Service mode.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_rlp_tx_non_empty_event( void);

#endif /* FEATURE_DS_SOCKETS */

#ifdef FEATURE_DS_DEBUG_UI
/*===========================================================================

FUNCTION DS_GET_STATS

DESCRIPTION
  This function will copy DS stats into the given array.

DEPENDENCIES
  The given array must have at least DS_STATS_COUNT elements.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ds_get_stats
(
  word *stats_ptr      /* Pointer to array into which stats will be copied */
);
#endif /* FEATURE_DS_DEBUG_UI */

#ifdef FEATURE_DS_PZID_RECON
/*===========================================================================
FUNCTION DS_PZID_RECONNECT_CB

DESCRIPTION
  This function gets called when Packet Zone ID has changed, i.e, mobile has
  moved to a new PPP point of attachment.
  This queues a command to the DS task, which will originate a packet call.
  
  This function is called from the context of the MC task - from a mccsyobj
  callback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void ds_pzid_reconnect_cb
(
  void
);
#endif /* FEATURE_DS_PZID_RECON */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION       DS_SCRM_CALL_INIT

DESCRIPTION    Called at the beginning of a data or S-TDSO call, to
               initialize SCRM variables.

DEPENDENCIES   NONE

RETURN VALUE   None

SIDE EFFECTS   None
===========================================================================*/
void ds_scrm_call_init(void);
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_IS2000_R_SCH
/*===========================================================================
FUNCTION     DS_SCRM_MAX_ALLOWABLE_SCH_RATE

DESCRIPTION  Look at the table scrm_sch_rate_matrix[] for the max allowable
             rate the MSM can do in the Rev. SCH. 

DEPENDENCIES None.

RETURN VALUE Max number of MuxPDUs.

SIDE EFFECTS None.
===========================================================================*/
byte ds_scrm_max_allowable_sch_rate( void );
#endif /* FEATURE_IS2000_R_SCH */

#ifdef FEATURE_DS_IS2000
/*===========================================================================

FUNCTION      DSM_REGISTER_DNE_CALLBACK

DESCRIPTION
  Called by a task or service (i.e., RLP) to register a callback that is
  called when the number of free (large or small) items hits 
  DO_NOT_EXCEED.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None.  Will ERR_FATAL if could not register the callback

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_register_dne_callback
(
  void      (*dne_cback)(uint32 param), /* callback function               */
  uint32    param1                 /* parameter to pass to the callback    */
);
#endif /* FEATURE_DS_IS2000 */

#ifdef FEATURE_DS_IS2000
/*===========================================================================

FUNCTION      DSM_REGISTER_HI_MARK_CALLBACK

DESCRIPTION
  Called by a task or service (i.e., RLP) to register a callback that is
  called when the number of free (large or small) items hits 
  HI_WATER_MARK.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None.  Will ERR_FATAL if could not register the callback

SIDE EFFECTS
  None
===========================================================================*/

extern void dsm_register_hi_mark_callback
(
  void      (*hi_mark_cback)(uint32 param), /* callback function           */
  uint32    param1                 /* parameter to pass to the callback    */
);
#endif /* FEATURE_DS_IS2000 */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */


#endif /* DS_H */

