#ifndef MCCTCSUP_H
#define MCCTCSUP_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C D M A   T R A F F I C   C H A N N E L   S T A T E
         S U P P O R T   R O U T I N E S   H E A D E R   F I L E

GENERAL DESCRIPTION
  This module externalizes the support routines for the CDMA Sub-system,
  Mobile Station Control on the Traffic Channel state.

Copyright (c) 1990,1991,1992 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1993,1994,1995 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1996 by QUALCOMM, Incorporated.  All Rights Reserved.
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header:   O:/src/asw/COMMON/vcs/mcctcsup.h_v   1.9   26 Feb 2001 16:54:14   marci  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/26/01   mai     Changed FEATURE_GPSONE to FEATURE_GPS_MODE to move code needed
                   only for GPS part of position location to a new #define.
01/24/01   ych     Merged T-53/JCDMA features for SK.
12/15/00   lh      Add support for MS originated Long Code Transistion Request
                   Order
11/30/00   yll     Removed FEATURE_GPSONE_TAWD_TEST_ONLY.
11/28/00   ck      Changed the return type and added another parameter for
                   tc_auth_rpt().
11/04/00   ks      Added prototypes for send_srv_ctl_tdso_ctl_dir() and
                   send_srv_ctl_tdso_stat().
10/27/00   jq      Merged T_AUTH to FEATURE_AUTH.
09/11/00   yll     Added support for Position Determination DBM.
08/18/00           Added prototype for process_retry_order().
08/02/00   yll     Added GPSONE TAWD (tuning away w/o drop) testing support.
06/19/00   yll     Added extern definition for tc_block_srch_gps_visit(),
                   tc_gps_in_progress, tc_gps_init().                 
06/01/00   fc      Added prototype for process_outer_loop_report_order().
04/13/00   lcc     Added prototype for tc_trans_done_rpt.
03/13/00   ry      Added IS2000 featurization
11/15/99   va      IS2000 Changes:
                   tc_get_plc added for getting the value of PLC
                   tc_populate_scr_info added to populate the fields of SCR info
                   in the interfaces to lower layers from pending service config.
                   process_pwr_ctrl_msg added to proces the enhanced PCM.
08/30/99   ry      Added extern definition for send_plc_req_ord()
08/13/99   jq      Added tc_send_flash_info_msg().
07/28/99   lh      CM and two-way SMS support added.
07/08/99   ych     Added support for Periodic Pilot Strength Measurment
                   Message
05/26/99   fc      Added support for closed loop power control step size.
05/19/99   doj     Moved extern declaration of SEND_HANDOFF_COMPLETE_MSG,
                   PROCESS_FM_HO_MSG and HANDOFF_TO_CURRENT_ASET to
                   mcctcho.h.  Removed extern declaration of
                   INIT_FRAME_OFFSET_HANDOFF, PROCESS_HO_DIR_MSG,
                   PROCESS_EXT_HO_DIR_MSG, GHDM_VALID_AND_HANDOFF_OK and
                   PROCESS_GEN_HO_DIR_MSG and consolidated them in the
                   extern declaration for MCCTCHO_PROCESS_GENERIC_HANDOFF_MSG
                   in mcctcho.h.
02/13/99   doj     Added support for General Handoff Direction Message (GHDM)
                   and FEATURE_SPECIAL_MDR.
08/25/98   ck      Externalized the functions tc_dh_rpt.
09/21/96   rdh     ROM savings by consolidating header fill-ins of RXTX mssgs.
08/08/96   dna     process_tc_data_burst() now can change next_state if too
                   many OTASP subsidy unlock attempts fail.
06/25/96   dna     Externalized SEND_TC_DATA_BURST for FEATURE_OTASP.
05/30/96   dna     Created header.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "caii.h"
#include "comdef.h"
#include "mc.h"
#include "mccrx.h"
#include "mccrxtx.h"
#include "mccsrch.h"
#include "qw.h"
#include "rxtx.h"
#ifdef FEATURE_AUTH
#include "auth.h"
#endif /* FEATURE_AUTH */

#ifdef FEATURE_IS2000
#include "rxc.h"
#include "txc.h"
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_IS2000_TDSO
#include "tdso.h"
#endif /* FEATURE_IS2000_TDSO */

/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#ifdef FEATURE_IS95B_PPSMM
/* MC uses this flag to determine wheter the PPSMM is enabled or not. If
 * mcctcsup_ppsmm_enabled is set to TRUE, it means that PPSMM Processing
 * is enabled, else disabled.  This flag is used to determine whether
 * to really send a SRCH_STOP_PPSM_F to SRCH in case of GHDM, EHDM
 * processing and one time PPSMM. MC will only send SRCH_STOP_PPSM_F if
 * PPSMM is enable. This way MC will not unneccsarily send CMDS to SRCH
 */
extern boolean mcctc_ppsm_enabled;
#endif /* FEATURE_IS95B_PPSMM */

#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTC_FILL_RXTX_HDR_ACK_Q

DESCRIPTION
  This function fills in the header of the RXTX_TC_MSG command for
  messages which need to be returned to the ack queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcctc_fill_rxtx_hdr_ack_q
(
  rxtx_cmd_type *cmd_ptr
   /* Pointer to command to send to layer 2 task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCCTC_FILL_RXTX_HDR_FREE_Q

DESCRIPTION
  This function fills in the header of the RXTX_TC_MSG command for
  messages which do NOT need to be returned to the ack queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcctc_fill_rxtx_hdr_free_q
(
  rxtx_cmd_type *cmd_ptr
   /* Pointer to command to send to layer 2 task */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_REJ_ORDER

DESCRIPTION
  This function builds and sends a Mobile Station Reject Order to the
  layer 2 task to be sent to the Base Station.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_rej_order
(
  byte rej_msg_type,
    /* Message type of rejected message */
  byte rej_code,
    /* Indicator of reason why message was rejected */
  byte rej_order,
    /* Order type of rejected message - only filled in if
       rej_msg_type == Order */
  byte rej_ordq,
     /* Order qualification code of rejected message - only filled in if
        rej_msg_type == Order */
  word rej_param_id,
    /* Parameter if of rejected message - only filled in if rej_msg_type ==
       Set Parameters or Retrieve Parameters */
  byte rej_record
    /* Record type of rejected information record only filled in if
       rej_msg_type == Flash with Information or Alert with information */
);

#ifdef FEATURE_AUTH
/* <EJECT> */
/*===========================================================================

FUNCTION START_SSD_UPDATE

DESCRIPTION
  This function sends a command to the Authentication Task to begin the
  SSD Update process.  It then sends a BS Challenge Order, which forces
  the base station to authenticate itself before we agree to update our SSD.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the SSD Update can not be started, the SSD Update Message is rejected.

===========================================================================*/
extern void start_ssd_update
(
  qword randssd
    /* Random number given by base station for use in SSD generation */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_AUTH_RESP_MSG

DESCRIPTION
  This function sends an Authentication Response Message with the
  auth signature value passed in.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void send_auth_resp_msg
(
  dword authu
    /* Authentication Signature to be included with message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_SSD_UPDATE_ORD

DESCRIPTION
  This function queries the authentication task to find out if the BS
  Challenge was successful, and sends an SSD Update Accepted or SSD
  Update Rejected order as appropriate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_ssd_update_ord
(
  dword authbs
    /* Value of AUTHBS calculated by Base Station */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_PARAM_UPDATE_ORD

DESCRIPTION
  This function sends a Parameter Update Confirmation order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_param_update_ord
(
  byte ordq
    /* Value of ordq field of parameter update order */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_PLC_REQ_ORD

DESCRIPTION
  This function sends a Long Code Transition Request Order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_plc_req_ord
(
  byte req_ordq
    /* Value of ordq field of Long Code Transition Request Order */
);


/* <EJECT> */
/*===========================================================================

FUNCTION CHANGE_PLC

DESCRIPTION
  This function changes the private long code, if possible and if necessary.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void change_plc
(
  byte req_ordq
    /* Value of ordq field of Long Code Transition Request Order */
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_PRIVACY_PREF

DESCRIPTION
  This function processes a privacy preferenc request from CM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void process_privacy_pref
(
  mc_msg_type *msg_ptr
    /* Pointer to message received from the handset*/
);


#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION TC_GET_PLC

DESCRIPTION
  This function returns plc value sent as reference, It determines if we have
  to switch our PLC and computes the right one if we need to change

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if plc has changed, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tc_get_plc
(
  byte req_ordq,
  qword plcm
);
#endif /* FEATURE_IS2000 */


/* <EJECT> */
/*===========================================================================

FUNCTION START_AUTH_SIGNATURE

DESCRIPTION
  This function sends a command to AUTH to perform the auth_signature
  for an auth challenge message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If we can not perform the auth signature we reject the auth challenge
  message.

===========================================================================*/

extern void start_auth_signature ( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TC_AUTH_RPT

DESCRIPTION
  This function processes a report from the Authentication task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern word tc_auth_rpt
(
  auth_rpt_type *auth_rpt_ptr,
    /* Pointer to authentication report */
  word next_state
    /* Next state */
);
#endif /* FEATURE_AUTH */

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION GET_ABS_ACTION_TIME

DESCRIPTION
  This function converts an action time in units of mod 64 super frame
  into an absolute time in frames for lower layers

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The input argument abs_action_time is passed by address and contains the return value

===========================================================================*/

extern void get_abs_action_time
(
  qword abs_action_time,
  byte action_time
);

/*===========================================================================

FUNCTION tc_send_rel_to_rxc_txc

DESCRIPTION
This function sends release command to RXC/TXC making them release their
Service Option (SO)

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/
extern void tc_send_rel_to_rxc_txc( void );
#endif /* FEATURE_IS2000 */


/* <EJECT> */
/*===========================================================================

FUNCTION SEND_ORIG_C_MSG

DESCRIPTION
  This function sends an Origination Continuation Message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_orig_c_msg( void );

/* <EJECT> */
/*===========================================================================

FUNCTION START_CONT_DTMF

DESCRIPTION
  This function processes a request from ui to send a continous dtmf order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void start_cont_dtmf
(
  mc_msg_type *msg_ptr
    /* Pointer to MC_START_CONT_DTMF_F command from UI */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_STOP_DTMF

DESCRIPTION
  This function sends a stop continous dtmf order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void send_stop_dtmf( void );

/* <EJECT> */
/*===========================================================================

FUNCTION STOP_CONT_DTMF

DESCRIPTION
  This function processes requests to Stop continous dtmfs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void stop_cont_dtmf
(
  mc_msg_type *msg_ptr
    /* Pointer to MC_STOP_CONT_DTMF_F command from UI */
);

/* <EJECT> */
/*===========================================================================

FUNCTION BURST_DTMF

DESCRIPTION
  This function processes MC_BURST_DTMF_F commands.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void burst_dtmf
(
  mc_burst_dtmf_type *cmd_ptr
    /* Pointer to MC_BURST_DTMF_F command from UI */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_ACTION_CHK

DESCRIPTION
  This function checks the action time in a message.  If the action time
  is valid, it sets the action_timer to wake up on the action time and
  copies the received message to the tc_action_msg buffer.  If the action
  time is invalid, it sends a Mobile Station Reject Order to the base station
  indicating that the message was invalid.  If the function finds that the
  action timer is already set, this function will send a Mobile Station
  Reject Order since IS-95 only allows for one message with an outstanding
  action time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  See function description.

===========================================================================*/

extern void tc_action_chk
(
  caii_rx_msg_type *msg_ptr
    /* Pointer to message with action time field */
);

/* <EJECT> */
/*===========================================================================

FUNCTION STOP_TX

DESCRIPTION
  This function sends a message to the Transmit task to stop
  transmitting and sends a message to the layer 2 task to indicate that
  the transmitter has been turned off.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void stop_tx( void );

/* <EJECT> */
/*===========================================================================

FUNCTION START_TX

DESCRIPTION
  This function sends a message to the Transmit task to start
  transmitting and subsequently sends a message to the layer 2 task to
  indicate that the transmitter has be started.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void start_tx( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TC_INCOMING_CALL

DESCRIPTION
  This function sends an Incoming Call notification to the User Interface
  and Data Services tasks.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void tc_incoming_call( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TC_STOP_ALERT

DESCRIPTION
  This function builds a Stop Alert message to be sent to the User Interface
  to stop ringing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void tc_stop_alert( void );

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SO_CTL

DESCRIPTION
  This function processes a received Service Option Control Order.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void tc_so_ctl( caii_rx_msg_type *msg_ptr );


#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION TC_POPULATE_SCR_INFO

DESCRIPTION
  This function gets a pointer to pending configuration and populates scr info
  to be sent to rxc and txc

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The contents of the pointers passed in are changed.

===========================================================================*/
extern void tc_populate_scr_info
(
  rxc_scr_info_type * rxc_scr_info,
  txc_scr_info_type * txc_scr_info
);

/*===========================================================================

FUNCTION TC_CAN_SO_BE_CONNECTED

DESCRIPTION
  This function determines if SO can be connected with the lower layer at this time.
  This decision is based on the SO to be connected and our current sub state in TC state.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if SO can be connected, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tc_can_so_be_connected
(
  word new_so
);

/*===========================================================================

FUNCTION PROCESS_OUTER_LOOP_REPORT_ORDER

DESCRIPTION
  This function processes the input outer loop report request order message. 
  The message will be rejected if P_REV_IN_USE is less than 6. Otherwise,
  a command is sent to RXC to request the outer loop report. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void process_outer_loop_report_order
(
  caii_rx_msg_type * msg_ptr
    /* Pointer to the received outer loop report request order message */
); /* process_outer_loop_report_order */

/*===========================================================================

FUNCTION PROCESS_RETRY_ORDER

DESCRIPTION
  This function processes the input retry order message. The message will be 
  rejected if P_REV_IN_USE is less than 6. Otherwise, if the retry type is 
  valid, it will inform DS the changes in retry delay if DS wants.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void process_retry_order
(
  caii_rx_msg_type * msg_ptr
); /* process_retry_order */

#endif /* FEATURE_IS2000 */


/* <EJECT> */
/*===========================================================================

FUNCTION TC_SRV_CTL_MSG

DESCRIPTION
  This function processes a received Service Option Control Message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void tc_srv_ctl_msg( caii_rx_msg_type *msg_ptr );

/* <EJECT> */
/*===========================================================================

FUNCTION REQ_PIL_STR

DESCRIPTION
  This function asks SRCH to send a Pilot Strength Measurement message,
  in response to a Pilot strength measurement request order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void req_pil_str( void );

#ifdef FEATURE_IS95B_PPSMM
/* <EJECT> */
/*===========================================================================

FUNCTION REQ_PIL_STR

DESCRIPTION
  This function asks SRCH to send a Pilot Strength Measurement message,
  in response to a Pilot strength measurement request order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void req_ppm_rpt( mccrxtx_msg_type *msg_ptr );
#endif /* FEATURE_IS95B_PPSMM */


/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_PWR_MSG

DESCRIPTION
  This function processes an input Power Control Parameters Message.
  In response to this message a Power Measurement Report message is
  formatted.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_pwr_msg
(
  caii_rx_msg_type *msg_ptr
   /* Pointer to received Power Control Parameters Message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_PROC_ACK

DESCRIPTION
  This function processes messages for which RXTX is required to signal
  MC when the acknowldegement is received for the message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Command buffer will be placed back on the rxtx_md_free_q.

===========================================================================*/

extern void tc_proc_ack
(
  rxtx_cmd_type *cmd_ptr
    /* Pointer to rxtx command taken from mc_rxtx_ack_q */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_LOCK_ORD

DESCRIPTION
  This function processes a Lock Until Power-Cycled order, a Maintenance
  Required Order, and an Unlock Order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern word tc_lock_ord
(
  caii_ftc_ord_type *msg_ptr,
    /* Pointer to received Forward Traffic Channel Order Message */
  word curr_state
    /* Traffic Channel substate currently being processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_MOB_REG

DESCRIPTION
  This function processes the Mobile Station Registered Message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The roam status and the registration variables will be updated.

===========================================================================*/
extern void tc_mob_reg
(
  caii_registered_type *msg_ptr
    /* pointer to received mobile station registered message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_FTC_TMSI_ASSIGN

DESCRIPTION
  This function processes a TMSI Assignment Message received on the
  Forward Traffic Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void process_ftc_tmsi_assign
(
  caii_rx_msg_type *msg_ptr
    /* pointer to received Data Burst message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_TC_DATA_BURST

DESCRIPTION
  This function processes a Data Burst Message received on the
  Foward Traffic Channel.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
extern word process_tc_data_burst
(
  caii_rx_msg_type *msg_ptr,
    /* pointer to received Data Burst message */
  word next_state
    /* Next state to be processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_NLU_MSG

DESCRIPTION
  This function processes an input Neighbor List Update message. The
  Neighbor set is update accordingly and a command is sent to the
  Searcher task to use the new neighbor set.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_nlu_msg
(
     caii_rx_msg_type *msg_ptr
       /* pointer to received Neighbor List Update message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION PROCESS_SP_MSG

DESCRIPTION
  This function processes an input In-Traffic System Parameters message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void process_sp_msg
  (
     caii_rx_msg_type *msg_ptr
      /* pointer to received In-Traffic System Parameters message */
  );

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_PARM_RESP

DESCRIPTION
  This function sends a Parameter Response message in response to a Retreive
  Parameters message.  If this procedure determines that it cannot supply a
  value for a parameter it will instead send a Mobile Station Reject Order.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void send_parm_resp
(
  caii_rx_msg_type *msg_ptr
    /* Pointer to Retreive Parameters message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SET_PARM

DESCRIPTION
  This function responds to a Set Parameters message.  If the Set Parameters
  message contains a parameter which cannot be set NONE of the parameters
  will be set.

DEPENDENCIES
  None.

RETURN VALUE
  MC_DONE_S if parameters were set.
  MC_BADPARM_S if a parameter was invalid.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tc_set_parm
(
   caii_rx_msg_type *msg_ptr
     /* Pointer to Set Parameters Message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SO_REQ

DESCRIPTION
  This function processes a Service Option Request Order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void tc_so_req
(
  caii_rx_msg_type *msg_ptr
    /* Pointer to received SOR Request message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SO_RESP

DESCRIPTION
  This function processes a Service Option Response Order.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.  If it is not necessary to change the
  current state as a result of the received Service Option Response
  Order, the return state will be equal to the current state.

SIDE EFFECTS
  None

===========================================================================*/

extern word tc_so_resp
(
  caii_rx_msg_type *msg_ptr,
    /* Pointer to received SOR Response message */
  word curr_state
    /* Traffic Channel substate currently being processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_STATUS_MSG

DESCRIPTION
  This function sends a Status Message in response to a received
  Status Request Order.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void send_status_msg
(
  caii_rx_msg_type *msg_ptr
    /* Pointer to Status Request Order */
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_STATUS_RSP_MSG

DESCRIPTION
  This function sends a Status Response Message in response to a received
  Status Request Message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void send_status_rsp_msg
(
  caii_rx_msg_type *msg_ptr
    /* Pointer to Status Request Message */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_RX_RPT

DESCRIPTION
  This function processes reports from the Receive task during the Traffic
  Channel substates.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word tc_rx_rpt
(
  mccrx_rpt_type *rpt_ptr,
    /* pointer to report received from RX */
  word next_state
    /* next state to be processed */
);

/* <EJECT> */
/*===========================================================================

FUNCTION TC_SRCH_RPT

DESCRIPTION
  This function processes the reports from the Searcher task during the
  Conversation substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void tc_srch_rpt
(
  mccsrch_rpt_type *rpt_ptr
    /* report received from Searcher */
);

/*===========================================================================

FUNCTION TC_TRANS_DONE_RPT

DESCRIPTION
  This function processes the transaction done reports from transaction servers
  (can be RXC, TXC, or SRCH).  Note that no report type is included in this
  report because it is not needed.  This report is currently used only to indicate
  the completion of a particular transaction.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tc_trans_done_rpt( void );

/* <EJECT> */
/*===========================================================================

FUNCTION SO_CONV

DESCRIPTION
  This function processes the service option actions needed on the change to
  Conversation substate.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void so_conv
(
void
);

/* <EJECT> */
/*===========================================================================

FUNCTION MCC_AUTO_ANSWER

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word mcc_auto_answer
(
word next_state
);

/* <EJECT> */
/*===========================================================================

FUNCTION SO_WFA

DESCRIPTION
  This function processes service option actions needed for the Mobile Station
  Control on the Traffic Channel Waiting For Answer substate.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

extern word so_wfa
(
  word next_state
);

/* <EJECT> */
/*===========================================================================

FUNCTION SEND_TC_DATA_BURST
  This function builds an Data Burst message for Reverse Traffic Channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void send_tc_data_burst
(
  byte burst_type,
    /* Type of Data Burst */
  byte chari_len,
    /* Byte length of chari data */
  byte *chari_data_ptr,
    /* Pointer to chari data */
  boolean ack_req
    /* Layer 2 ack required */
);

#if (defined (FEATURE_UASMS) || defined (FEATURE_GPSONE_DBM))
/* <EJECT> */
/*===========================================================================

FUNCTION SEND_TC_MO_DBM
  This function sends out a mobile originated data burst message (Position
  Determination or SMS) by calling send_tc_data_burst function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void send_tc_mo_dbm
(
  mc_mo_dbm_type mo_dbm
    /* mobile originated data burst message type to be sent over the air.*/
);
#endif /* FEATURE_UASMS || FEATURE_GPSONE_DBM */

#if (defined (FEATURE_DH) && defined(FEATURE_DH_EXP))
/* <EJECT> */
/*===========================================================================

FUNCTION TC_DH_RPT

DESCRIPTION
  This function processes a report from the Authentication task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tc_dh_rpt
(
  dh_rpt_type *dh_rpt_ptr
    /* Pointer to dh report */
);

#endif /*FEATURE_DH */

#ifdef FEATURE_IS95B_NGHBR_SRCH_ENH
/*===========================================================================

FUNCTION PROCESS_ENLU_MSG

DESCRIPTION
  This function processes an input Extended Neighbor List Update message.
  The Neighbor set is update accordingly and a command is sent to the
  Searcher task to use the new neighbor set.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void process_enlu_msg
(
     caii_rx_msg_type *msg_ptr
       /* pointer to received Extended Neighbor List Update message */
);
#endif /* FEATURE_IS95B_NGHBR_SRCH_ENH */

#ifdef FEATURE_IS95B
/*===========================================================================

FUNCTION PROCESS_PWR_CNTL_STEP

DESCRIPTION
  This function processes the power control step within the Power Control
  Message or General Handoff Direction Message received from the Forward
  Traffic Channel. If MS supports the received power control step Size,
  then MS stores the received power control step and informs the MSM to
  use it as the Closed Loop Power Control Step Change. Otherwise, MS just
  ignores it.
  However, the processing mentioned above will only be implemented after
  Reverse Link MDR has been implemented because of benefits tradeoff.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void process_pwr_cntl_step
(
  byte pwr_cntl_step /* Power Control Step */
);
#endif /* FEATURE_IS95B */

#ifdef FEATURE_IS2000
/*===========================================================================

FUNCTION PROCESS_PWR_CTRL_MSG

DESCRIPTION
  This function processes the forward and reverse power control parameters
  received in the power control message. The forward power control parameters are sent
  to the RXC and the reverse power control parameters are sent to the TXC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void process_pwr_ctrl_msg
(
  caii_rx_msg_type * msg_ptr
);
#endif


/*===========================================================================

FUNCTION TC_SEND_FLASH_INFO_MSG

DESCRIPTION
  This function sends the flash with information message during traffic
  channel processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void tc_send_flash_info_msg
(
  mc_msg_type *msg_ptr
  /* pointer to message received from other other tasks */
);

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_GPS_MODE
#error code not present
#endif /* FEATURE_GPS_MODE */

#ifdef FEATURE_IS2000_TDSO
/*===========================================================================

FUNCTION SEND_SRV_CTL_TDSO_STAT

DESCRIPTION
  This function sends a Service Option Control message in response to a
  Retrieve TDSO Counters directive.  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void send_srv_ctl_tdso_stat
( 
  uint8 ctl_rec_type,      
  uint8 vect_counter_id, 
  uint8 con_ref         
);

/*===========================================================================

FUNCTION SEND_SRV_CTL_TDSO_CTL_DIR

DESCRIPTION
  This function sends a Service Option Control message that proposes TDSO
  test parameters.  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void send_srv_ctl_tdso_ctl_dir
(
  tdso_tst_par_type fch_tst_par,
  tdso_tst_par_type sch0_tst_par
);
#endif /* FEATURE_IS2000_TDSO */

#endif /* MCCTCSUP_H */

