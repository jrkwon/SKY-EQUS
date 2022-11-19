/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


           U S E R   I N T E R F A C E   M O D U L E

GENERAL DESCRIPTION
  The interface between UI and Call Manager, note that call back functions
  are called from call manager context. UI functions/variables in call back 
  functions have to be multi-thread safe. 

  Copyright (c) 1998 by QUALCOMM INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uixcm.c_v   1.6   04 Aug 2000 10:44:06   
hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/00   CS      Removed 999 as an emergency number.
05/10/00   HQ      Check ui.dial.status for emergency numbers.
04/11/00   CS      Added support for FEATURE_TTY.
03/29/00   VT      Changed *228 from STD to NON_STD in OTASP Table.
01/11/00   VT      Removed dependency on cmph.h
12/18/99   CS      Used ui_get_nv_for_cm instead of ui_get_nv function.
12/03/99   VT      Merged TriMode Code.
10/27/99   VT      Now passing SO argument to in cm_call_cmd_orig().
10/07/99   CS      Added display for new SS info fields.
                   Changed CM_SMS_EVENT_N_CLI to CM_SMS_EVENT_N_VOICE.
09/01/99   CS      Added support for undefine FEATURE_OTASP and FEATURE_UASMS.
                   Added CM SMS event handling when FEATURE_UASMS is undefined.
07/29/99   CS      Changed ui_set_ph_oprt_mode from void to boolean.
07/14/99   VT      Renamed uincm.c to uixcm.c.
11/23/98   SIU     Added Service Changed support.

===========================================================================*/


#include "target.h"
#include "customer.h"
#include "comdef.h"
#include <string.h>
#include "assert.h"
#include "queue.h"
#include "err.h"
#include "rex.h"
#include "db.h"
#include "cm.h"

/* SKY user interface */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) || (MODEL_ID == MODEL_B2)

#include "sky.h"
#include "uixcm.h"

#else

#include "ui.h"
#include "uistate.h"
#include "uixcm.h"

#endif

#ifdef FEATURE_UASMS
#include "uixuasms.h"
#include "uasms.h"
#endif /* FEATURE_UASMS */


/* <EJECT> */
/*===========================================================================
                         DEFINITIONS
===========================================================================*/

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

/* SKY user interface */
#ifdef SKY_UI
#define GET_PHONE_INFO_NAM_SELECT()       SKY_GET_PHONE_INFO_NAM_SELECT()
#define GET_PHONE_INFO_CURRENT_NAM()      SKY_GET_PHONE_INFO_CURRENT_NAM()
#define GET_FORCE_SO()                    SKY_GET_FORCE_SERVICE_OPTION()
#define SET_FORCE_SO(so)                  SKY_SET_FORCE_SERVICE_OPTION(so)

static void ui_get_keys(boolean onoff)
{
    SKY_AcceptKey(onoff);
}

static void ui_stop_fwd_burst_dtmf()
{
    //SKY_StopForwardBurstDTMF();
}

/* original interface */
#else
#define GET_PHONE_INFO_NAM_SELECT       ui.ph_info.nam_sel
#define GET_PHONE_INFO_CURRENT_NAM      ui.ph_info.curr_nam
#define GET_FORCE_SO()                  ui.force_so
#define SET_FORCE_SO(so)                ui.force_so = so
#endif

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */

/* <EJECT> */
/*===========================================================================
                      STATIC DATA
===========================================================================*/
static cm_client_id_type client_ui;
static boolean cm_ph_info_avail;

#ifdef FEATURE_OTASP
#define STAR228 "*228"
#define STANDARD_OTASP_LENGTH 6
  /* Table specifying default hard-coded special numbers and their
  ** corresponding call type.
  **
  ** Note that for OTASP numbers the corresponding activation code
  ** is also specified.
  */
  static const struct {
      byte                     *num_str;      /* number string */
      cm_call_type_e_type      call_type;     /* associated call type */
      cm_activate_code_type    activate_code; /* OTASP activation code */
  } otasp_num_tbl[] = {

      /* number */   /* call type */               /* OTASP activate code */
#ifdef FEATURE_UI_STAR2_CALL
#error code not present
#endif
#ifdef FEATURE_CDMA_800
      { (byte *) "*22800",    CM_CALL_TYPE_STD_OTASP,       ABAND     },
      { (byte *) "*22801",    CM_CALL_TYPE_STD_OTASP,       BBAND     },
#endif
#ifdef FEATURE_CDMA_1900
      { (byte *) "*22802",    CM_CALL_TYPE_STD_OTASP,       ABLOCK    },
      { (byte *) "*22803",    CM_CALL_TYPE_STD_OTASP,       BBLOCK    },
      { (byte *) "*22804",    CM_CALL_TYPE_STD_OTASP,       CBLOCK    },
      { (byte *) "*22805",    CM_CALL_TYPE_STD_OTASP,       DBLOCK    },
      { (byte *) "*22806",    CM_CALL_TYPE_STD_OTASP,       EBLOCK    },
      { (byte *) "*22807",    CM_CALL_TYPE_STD_OTASP,       FBLOCK    },
#endif
#ifdef FEATURE_UI_OTASP_STAR2281X
#error code not present
#endif
      { (byte *) "*228",      CM_CALL_TYPE_NON_STD_OTASP,   PREFERRED }
  };

#endif /* FEATURE_OTASP */


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_info_avail

DESCRIPTION
   set phone info is available or not

DEPENDENCIES
   None

RETURNS
   None

SIDE EFFECTS
   None
===========================================================================*/
void ui_set_ph_info_avail(boolean is_avail)
{
  cm_ph_info_avail = is_avail;
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_ph_info_is_avail

DESCRIPTION
   phone info is available or not

DEPENDENCIES
   None

RETURNS
   phone info is available ?

SIDE EFFECTS
   None
===========================================================================*/
boolean ui_ph_info_is_avail()
{
  return cm_ph_info_avail;
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_call_is_from_ui

DESCRIPTION
  see if the client for a call event is UI

DEPENDENCIES
  None

RETURNS
  Is the call from ui?

SIDE EFFECTS
  None

===========================================================================*/
boolean ui_call_is_from_ui(const cm_call_info_s_type* call_info_ptr)
{
  if  (call_info_ptr->call_client == client_ui) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/* <EJECT> */
/*===========================================================================
FUNCTION   ui_alloc_cmdbuf_die_if_fail

DESCRIPTION
  allocates a command buffer from UI free command queue. die if it fails.

DEPENDENCIES
  command queue is intialized

RETURNS
  pointer to the buffer allocated

SIDE EFFECTS
  None

===========================================================================*/
static ui_cmd_type* ui_alloc_cmdbuf_die_if_fail()
{
  ui_cmd_type *cmd_ptr;

  cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q);
  if (cmd_ptr == NULL) {
    ERR_FATAL("No Buffers", 0, 0, 0);
  }

  return (cmd_ptr);
}

#if 0
/* <EJECT> */
/*===========================================================================
FUNCTION   ui_alloc_cmdbuf

DESCRIPTION
  allocates a command buffer from UI free command queue. return NULL if it 
fails

DEPENDENCIES
  command queue is intialized

RETURNS
  pointer to the buffer allocated, NULL if it fails

SIDE EFFECTS
  None
===========================================================================*/
static ui_cmd_type* ui_alloc_cmdbuf()
{
  ui_cmd_type *cmd_ptr;

  cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_free_q);
  if (cmd_ptr == NULL) {
    ERR("No Buffers", 0, 0, 0);
  }

  return (cmd_ptr);
}
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_call_event_cb

DESCRIPTION
  Call event callback function, called when a call event is happening

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Put command in UI command buffer
===========================================================================*/
static void ui_cm_call_event_cb (
  cm_call_event_e_type call_event,
  const cm_call_info_s_type *call_info_ptr
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT (call_info_ptr != NULL);

  if (call_event == CM_CALL_EVENT_INFO)
  {
     MSG_MED("CM_CALL_EVENT_INFO",0,0,0);
     tst_call_info_print(call_info_ptr);
     return;
  }

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();

  switch (call_event) {
    case CM_CALL_EVENT_INCOM:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_INCOMING_F;
      break;

    case CM_CALL_EVENT_ORIG:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ORIGINATED_F;
      break;

    case CM_CALL_EVENT_ANSWER:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ANSWERED_F;
      break;

    case CM_CALL_EVENT_CONNECT:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_CONNECT_F;
      break;

    case CM_CALL_EVENT_END:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ENDED_F;
      break;

    case CM_CALL_EVENT_SRV_OPT:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_SO_CHANGED_F;
      break;

    case CM_CALL_EVENT_PRIVACY:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_PRIVACY_CHANGED_F;
      break;

    case CM_CALL_EVENT_CALLER_ID:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_CALLER_ID_F;
      break;

    case CM_CALL_EVENT_FLASH:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_FLASHED_F;
      break;

    // Support registration to SIGNAL event (ringing)
    case CM_CALL_EVENT_SIGNAL:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_SIGNAL_F;
      break;

    // Additions for CM2.0  
    case CM_CALL_EVENT_ABRV_ALERT:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ABRV_ALERT_F;
      break;

    case CM_CALL_EVENT_ABRV_REORDER:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ABRV_REORDER_F;
      break;

    case CM_CALL_EVENT_ABRV_INTERCEPT:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_ABRV_INTERCEPT_F;
      break;

    case CM_CALL_EVENT_DISPLAY:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_DISPLAY_F;
      break;

    case CM_CALL_EVENT_CALLED_PARTY:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_CALLED_PARTY_F;
      break;

    case CM_CALL_EVENT_CONNECTED_NUM:
      cmd_ptr->call_event.hdr.cmd = UI_CALL_CONNECTED_NUM_F;
      break;

#ifdef FEATURE_IS95B_EXT_DISP
      case CM_CALL_EVENT_EXT_DISP:
        cmd_ptr->call_event.hdr.cmd = UI_CALL_EXT_DISPLAY_F;
        break;
#endif

#ifdef FEATURE_IS95B_NDSS
      case CM_CALL_EVENT_NDSS_START:
        cmd_ptr->call_event.hdr.cmd = UI_CALL_NDSS_START_F;
        break;

      case CM_CALL_EVENT_NDSS_CONNECT:
        cmd_ptr->call_event.hdr.cmd = UI_CALL_NDSS_CONNECT_F;
        break;
#endif
    default:
      ASSERT(FALSE);
      break;

  }

  cmd_ptr->call_event.call_info = *call_info_ptr;
  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}

/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_ph_event_cb

DESCRIPTION
  Phone event callback function, called when a phone event is happening

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_ph_event_cb (
  cm_ph_event_e_type ph_event,
  const cm_ph_info_s_type *ph_info_ptr
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT (ph_info_ptr != NULL);

  if (ph_event == CM_PH_EVENT_INFO)
  {
     MSG_MED("CM_PH_EVENT_INFO",0,0,0);
     tst_ph_info_print(ph_info_ptr);
     return;
  }

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();

  switch (ph_event) {
    case CM_PH_EVENT_IN_USE_STATE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_INUSE_STATE_F;
      break;

    case CM_PH_EVENT_SRV_STATE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_SRV_STATE_F;
      break;

    case CM_PH_EVENT_OPRT_MODE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_OPRT_MODE_F;
      break;

    case CM_PH_EVENT_CDMA_LOCK_MODE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_CDMA_LOCK_MODE_F;
      break;

    case CM_PH_EVENT_MODE_PREF:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_MODE_PREF_F;
      break;

    case CM_PH_EVENT_SYS_PREF:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_SYS_PREF_F;
      break;

    case CM_PH_EVENT_ANSWER_VOICE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_ANSWER_VOICE_F;
      break;

    case CM_PH_EVENT_CURR_NAM:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_CURR_NAM_F;
      break;

    case CM_PH_EVENT_NAM_SEL:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_NAM_SEL_F;
      break;

    case CM_PH_EVENT_ROAM_STATUS:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_ROAM_STATUS_F;
      break;

    case CM_PH_EVENT_INFO_AVAIL:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_INFO_AVAIL_F;
      break;

      // Additions for CM2.0
    case CM_PH_EVENT_MAINTREQ:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_MAINTREQ_F;
      break;

    case CM_PH_EVENT_STANDBY_SLEEP:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_STANDBY_SLEEP_F;
      break;

    case CM_PH_EVENT_STANDBY_WAKE:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_STANDBY_WAKE_F;
      break;

    case CM_PH_EVENT_RSSI:
      cmd_ptr->ph_event.hdr.cmd = UI_PH_RSSI_F;
      break;
      
    default:
      ASSERT(FALSE);
      break;
  }

  cmd_ptr->ph_event.ph_info = *ph_info_ptr;
  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}

/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_ss_event_cb

DESCRIPTION
  Serving system event callback function, called when a serving system
  event is happening

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_ss_event_cb (
  cm_ss_event_e_type ss_event,
  const cm_ss_info_s_type *ss_info_ptr
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT (ss_info_ptr != NULL);

  if (ss_event == CM_SS_EVENT_INFO)
  {
     MSG_MED("CM_SS_EVENT_INFO",0,0,0);
     tst_ss_info_print(ss_info_ptr);
     return;
  }

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();

  switch (ss_event) {
    case CM_SS_EVENT_SRV_CHANGED:
      cmd_ptr->ss_event.hdr.cmd = UI_SS_SRV_CHANGED_F;
      MSG_MED("CM_SS_SRV_CHANGED",0,0,0);
      tst_ss_info_print(ss_info_ptr);
      break;

    default:
      ASSERT(FALSE);
      break;
  }

  cmd_ptr->ss_event.ss_info = *ss_info_ptr;
  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}


/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_inband_event_cb

DESCRIPTION
  Serving inband event callback function, called when an inband
  event is happening

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_inband_event_cb (
  cm_inband_event_e_type inband_event,
  const cm_inband_info_s_type *inband_info_ptr
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT (inband_info_ptr != NULL);

  /* Do not process the commands if they were originated from UI */
  if ((inband_event == CM_INBAND_EVENT_REV_BURST_DTMF ||
       inband_event == CM_INBAND_EVENT_REV_START_CONT_DTMF ||
       inband_event == CM_INBAND_EVENT_REV_STOP_CONT_DTMF ) &&
      ( inband_info_ptr->client == client_ui))
        return;


  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();

  switch (inband_event) {
    case CM_INBAND_EVENT_REV_BURST_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_REV_BURST_DTMF_F;
      break;

    case CM_INBAND_EVENT_REV_START_CONT_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_REV_START_CONT_DTMF_F;
      break;

    case CM_INBAND_EVENT_REV_STOP_CONT_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_REV_STOP_CONT_DTMF_F;
      break;

    case CM_INBAND_EVENT_FWD_BURST_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_FWD_BURST_DTMF_F;
      break;

    case CM_INBAND_EVENT_FWD_START_CONT_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_FWD_START_CONT_DTMF_F;
      break;

    case CM_INBAND_EVENT_FWD_STOP_CONT_DTMF:
      cmd_ptr->inband_event.hdr.cmd = UI_INBAND_FWD_STOP_CONT_DTMF_F;
      break;

    default:
      ASSERT(FALSE);
      break;
  }

  cmd_ptr->inband_event.inband_info = *inband_info_ptr;
  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}



#ifndef FEATURE_UASMS

/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_sms_event_cb

DESCRIPTION
  SMS event callback function, called when a SMS
  event is happening

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_sms_event_cb (
  cm_sms_event_e_type sms_event,
  const cm_sms_info_s_type *sms_info_ptr
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT (sms_info_ptr != NULL);

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();

  switch (sms_event) {
    case CM_SMS_EVENT_MSG_WAITING:
      cmd_ptr->sms_event.hdr.cmd = UI_SMS_MSG_WAITING_F;
      break;

#ifdef FEATURE_ACP
    case CM_SMS_EVENT_N_VOICE:
      cmd_ptr->sms_event.hdr.cmd = UI_SMS_N_VOICE_F;
      break;
#endif /* FEATURE_ACP */
    default:
      ASSERT(FALSE);
      break;
  }

  cmd_ptr->sms_event.sms_info = *sms_info_ptr;
  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}
#endif /* FEATURE_UASMS */

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_cm_call_cmd_err_cb

DESCRIPTION
  Called when a call command error (from other tasks) happens

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_call_cmd_err_cb (
  cm_client_id_type client,
  cm_call_cmd_e_type cmd,
  cm_call_cmd_err_e_type cmd_err
)
{
  ui_cmd_type *cmd_ptr;
  extern void  CLNTa_log(uint8 viewId, int16 logNo, const char *format, ...);

  if (cmd_err == CM_CALL_CMD_ERR_NOERR) {
     return;
  }
  CLNTa_log(1,0,"in ui_cm_call_cmd_err_cb");

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();
  cmd_ptr->call_cmd_err.hdr.cmd = UI_CALL_CMD_ERR_F;
  cmd_ptr->call_cmd_err.client  = client;
  cmd_ptr->call_cmd_err.cmd     = cmd;
  cmd_ptr->call_cmd_err.cmd_err = cmd_err;

  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_cm_call_cmd_cb

DESCRIPTION
  Called by Call manager to indicate status of call command

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_call_cmd_cb (
  void* data_block_ptr,
  cm_call_cmd_e_type cmd,
  cm_call_cmd_err_e_type cmd_err
)
{
  ASSERT(data_block_ptr == NULL);  // we do not use data_block_ptr for now 

  // treait it the same as call command error 
  ui_cm_call_cmd_err_cb(client_ui, cmd, cmd_err);

}


/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_ph_cmd_cb

DESCRIPTION
  Called by Call Manager to indicate phone command status

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_ph_cmd_cb (
  void*  data_block_ptr,
  cm_ph_cmd_e_type cmd,
  cm_ph_cmd_err_e_type cmd_err
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT(data_block_ptr == NULL);  // we do not use data_block_ptr for now 

  if (cmd_err == CM_PH_CMD_ERR_NOERR) {
     return;
  }

  if (cmd == CM_PH_CMD_OPRT_MODE) {
    // not recoverable if it fails to go offline, powerdown etc
    ERR_FATAL("fail to change to operation mode", 0, 0, 0);
  }

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();
  cmd_ptr->ph_cmd_err.hdr.cmd = UI_PH_CMD_ERR_F;
  cmd_ptr->call_cmd_err.client= client_ui;
  cmd_ptr->ph_cmd_err.cmd     = cmd;
  cmd_ptr->ph_cmd_err.cmd_err = cmd_err;

  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);
  // enable keypad input that may be disabled when UI sends commands,
  ui_get_keys(TRUE);

}



/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_inband_cmd_cb

DESCRIPTION
  Called by Call Manager to indicate inband command status

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_inband_cmd_cb (
  void*  data_block_ptr,
  cm_inband_cmd_e_type cmd,
  cm_inband_cmd_err_e_type cmd_err
)
{
  ui_cmd_type *cmd_ptr;

  ASSERT(data_block_ptr == NULL);  // we do not use data_block_ptr for now 
  if (cmd_err == CM_INBAND_CMD_ERR_NOERR) {
     return;
  }

  cmd_ptr = ui_alloc_cmdbuf_die_if_fail();
  cmd_ptr->inband_cmd_err.hdr.cmd = UI_INBAND_CMD_ERR_F;
  cmd_ptr->inband_cmd_err.client= client_ui;
  cmd_ptr->inband_cmd_err.cmd     = cmd;
  cmd_ptr->inband_cmd_err.cmd_err = cmd_err;

  cmd_ptr->hdr.task_ptr = NULL;
  cmd_ptr->hdr.done_q_ptr = &ui_cmd_free_q;
  ui_cmd(cmd_ptr);

}

#ifndef SKY_UI /* original version */
/* <EJECT> */
/*===========================================================================
FUNCTION   number_is_matched

DESCRIPTION
  Determine if two number are matched

DEPENDENCIES
  None

RETURNS
  is it an match ?

SIDE EFFECTS
  None
===========================================================================*/
static boolean number_is_matched(
  const byte* num1_buf,    // buffer to store first number
  byte        num1_len,    // length of the first number
  const byte* num2_buf,    // buffer to store 2nd numbers
  byte        num2_len     // length of the 2nd number
)
{
  if (num1_len != num2_len) {
    return FALSE;
  } else {
    int i;
    for (i=0; i<num1_len; i++) {
      if (num1_buf[i] != num2_buf[i]) {
        return FALSE;
      }
    }
    return TRUE;
  }
}

/* <EJECT> */
/*===========================================================================
FUNCTION   is_emergency_call

DESCRIPTION
  Determine if a call is an emergency call

DEPENDENCIES
  None

RETURNS
  is it an emergency call?

SIDE EFFECTS
  None
===========================================================================*/
static boolean is_emergency_call(
  const byte* num_buf,    // buffer to store numbers
  byte        num_len     // length of the number
)
{

  static const struct {
      byte                     *num_str;
  } emergency_num_tbl[] = {
      { (byte *) "911" },
      { (byte *) "*911" },
      { (byte *) "#911" }
  };

  int i;

  if (num_len == 0) {
    return FALSE;
  }

  // is it a hardcode emergency # ?
  for (i=0; i<(int)ARR_SIZE(emergency_num_tbl); i++) {
    const byte* num2_buf = emergency_num_tbl[i].num_str;
    byte  num2_len = strlen((char *) num2_buf);
    if (number_is_matched( num_buf, num_len, num2_buf, num2_len)) {
      return TRUE;
    }
  }

  // not hard coded emergency number, check phone book
  for ( i=0; i<EMERGENCY_NUMBERS; i++ ) {
    const byte* num2_buf;
    byte  num2_len;
    ui.dial.address = (byte) ( i + UI_NV_DIALS + 1 ) ;
    (void) ui_get_nv_for_cm( NV_DIAL_I, ( nv_item_type * ) &ui.dial );
    num2_buf = (byte *) ui.dial.digits;
    num2_len = ui.dial.num_digits;
    if( ui.dial.status == NV_EMERGENCY_SD_NUM &&
        number_is_matched(num_buf, num_len, num2_buf, num2_len) )
    {
      return TRUE;
    }
  }

  return FALSE ;

}
#endif /* ifndef SKY_UI */

/* <EJECT> */
/*===========================================================================
FUNCTION   get_call_type

DESCRIPTION
  Determine call type for a origination #

DEPENDENCIES
  None

RETURNS
  None, result will be put into call_type_ptr and otasp_code_ptr

SIDE EFFECTS
  None
===========================================================================*/
static void get_call_type(
  const cm_num_s_type *orig_num_ptr,    // pointer to origination #
  cm_call_type_e_type *call_type_ptr,   // associated call type, save result
  cm_activate_code_type *otasp_code_ptr // OTASP activation code(OTASP call)
)
{


#ifdef FEATURE_OTASP
  int   i;
#endif /* FEATURE_OTASP */

  /* If this is NOT a voice call, return now  */
  if( *call_type_ptr != CM_CALL_TYPE_VOICE ) {
    return;
  }
#ifndef SKY_UI
  if (is_emergency_call(orig_num_ptr->buf, orig_num_ptr->len)) {
    *call_type_ptr = CM_CALL_TYPE_E911;
  }
#endif

#ifdef FEATURE_OTASP
  /* Consult the default hard-coded otasp numbers table to
  ** determine the originating number call type.
  */
  for( i=0; i< (int)ARR_SIZE(otasp_num_tbl); i++ ) {
    /* Check whether one of the otasp numbers match the origination # */
    if(memcmp(orig_num_ptr->buf, otasp_num_tbl[i].num_str, 
STANDARD_OTASP_LENGTH) == 0) {
      /* If we found a match, change the call type to the corresponding,
      ** type in the table and indicate the OTASP activation code.
      */
      *call_type_ptr  = otasp_num_tbl[i].call_type;
      *otasp_code_ptr = otasp_num_tbl[i].activate_code;
      break;
    }
  }

#endif /* FEATURE_OTASP */

}

/* <EJECT> */
/*===========================================================================
FUNCTION   call_is_allowed

DESCRIPTION
  Determine if a call is allowed from phone book

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean call_is_allowed(
  const cm_num_s_type *orig_num_ptr  // pointer to origination #
)
{
  byte i;

#ifdef FEATURE_OTASP
  /* Consult the default hard-coded otasp numbers table to
  ** determine the originating number call type.
  */
  for( i=0; i< (int)ARR_SIZE(otasp_num_tbl); i++ ) {
    /* Check whether one of the otasp numbers match the origination # */
    if(memcmp(orig_num_ptr->buf, otasp_num_tbl[i].num_str, 
STANDARD_OTASP_LENGTH) == 0) {
      break;
    }
  }
  /* Wrong activation code in PCS/Celluar is not allowed */
  if( i >= (int)ARR_SIZE(otasp_num_tbl) && 
      (memcmp(orig_num_ptr->buf, STAR228, strlen(STAR228)) == 0) ) {
    return FALSE;               
  }
#endif /* FEATURE_OTASP */

#ifdef SKY_UI
  // no restrictions
  return TRUE;
#else
  if (ui.lock) {
    // only emergency # is allowed
    if (is_emergency_call(orig_num_ptr->buf, orig_num_ptr->len)) {
       return TRUE;
    } else {
       return FALSE;
    }
  } else if (ui.restrict) {
    // allow emergency # and # in phone book
    if (is_emergency_call(orig_num_ptr->buf, orig_num_ptr->len)) {
       return TRUE;
    }
    for (i=1; i<100; i++) {
      const byte* num1_buf = orig_num_ptr->buf;
      byte  num1_len = orig_num_ptr->len;
      const byte* num2_buf;
      byte  num2_len;
      ui.dial.address = i;
      (void)ui_get_nv_for_cm(NV_DIAL_I, (nv_item_type*)&ui.dial);
      num2_buf = (byte *) ui.dial.digits;
      num2_len = ui.dial.num_digits;
      if (number_is_matched(num1_buf, num1_len, num2_buf, num2_len)) {
        return TRUE;
      }
    }
  } else {
    // no restrictions
    return TRUE;
  }
  
  return FALSE;
#endif
}


/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_init

DESCRIPTION
  initialize UI-CM interface

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_cm_init()
{
cm_client_status_e_type return_status;
  

  ui_set_ph_info_avail(FALSE);  // phone information is not available yet
  
  return_status = cm_client_init(&client_ui);
  if ( return_status != CM_CLIENT_OK ) 
    ERR_FATAL("cm_client_init() returned bad status %d", return_status, 0, 0);

  

  // register for call events the UI is interested in
  return_status = cm_client_call_reg( 
                      client_ui,               // pointer to the client struct 
                      ui_cm_call_event_cb,      // pointer to a cb function for call events
                      CM_CLIENT_EVENT_REG,      // event registration
                      CM_CALL_EVENT_ORIG,       // from this call event
                      CM_CALL_EVENT_INFO,       // to this call event
                      ui_cm_call_cmd_err_cb );  // pointer to a cb function for errors
  
  if (return_status != CM_CLIENT_OK ) 
    ERR_FATAL("cm_client_call_reg() returned bad status %d", return_status, 0, 0);

#ifdef FEATURE_IS95B_EXT_DISP
  return_status = cm_client_call_reg( 
                      client_ui,               // pointer to the client struct 
                      ui_cm_call_event_cb,      // pointer to a cb function for call events
                      CM_CLIENT_EVENT_REG,      // event registration
                      CM_CALL_EVENT_EXT_DISP,       // from this call event
                      CM_CALL_EVENT_EXT_DISP,       // to this call event
                      ui_cm_call_cmd_err_cb );  // pointer to a cb function for errors

  if (return_status != CM_CLIENT_OK ) 
    ERR_FATAL("cm_client_call_reg() returned bad status %d", return_status, 0, 0);

#endif




  
  // register for phone events the UI is interested in
  return_status = cm_client_ph_reg(  
                      client_ui,                 // pointer to the client struct 
                      ui_cm_ph_event_cb,          // pointer to a cb function for ph events
                      CM_CLIENT_EVENT_REG,        // event registration
                      CM_PH_EVENT_OPRT_MODE,      // from this call event
                      CM_PH_EVENT_INFO_AVAIL,           // to this call event
                      NULL );                     // pointer to a cb function for errors
  
  if (return_status != CM_CLIENT_OK )                  
    ERR_FATAL("cm_client_ph_reg() returned bad status %d", return_status, 0, 0);



  // register for serving system events 
  return_status = cm_client_ss_reg(  
                      client_ui,                 // pointer to the client struct 
                      ui_cm_ss_event_cb,          // pointer to a cb function for ss events
                      CM_CLIENT_EVENT_REG,        // event registration
                      CM_SS_EVENT_SRV_CHANGED,    // from this call event
                      CM_SS_EVENT_INFO,           // to this call event
                      NULL );                     // pointer to a cb function for errors

  if (return_status != CM_CLIENT_OK )
    ERR_FATAL("cm_client_ss_reg() returned bad status %d", return_status, 0, 0);


  // register for inband related events and callback functions
  return_status = cm_client_inband_reg(
                   client_ui,                          // pointer to the client struct 
                   ui_cm_inband_event_cb,               // pointer to a cb function for inband events
                   CM_CLIENT_EVENT_REG,                 // event registration
                   CM_INBAND_EVENT_REV_BURST_DTMF,      // from this call event
                   CM_INBAND_EVENT_FWD_STOP_CONT_DTMF,  // to this call event
                   NULL );                              // pointer to a cb function for errors

  if (return_status != CM_CLIENT_OK )
    ERR_FATAL("cm_client_inband_reg() returned bad status %d", return_status, 0, 0); 

#ifndef FEATURE_UASMS
  // register for SMS related events 
  return_status = cm_client_sms_reg(
                  client_ui,                    /* pointer to Requesting client structure */
                  ui_cm_sms_event_cb,           /* pointer to a cb function for SMS event */
                  CM_CLIENT_EVENT_REG,          /* event registration */
                  CM_SMS_EVENT_MSG_WAITING,     /* register from this event (including) */
                  CM_SMS_EVENT_N_VOICE,           /* to this event (including) */
                  NULL );                       /* pointer to a callback function to notify the client of sms
                                                ** command errors of OTHER clients */

#endif /* FEATURE_UASMS */

  // register phone book - number classification functions
  cm_nc_reg(get_call_type, call_is_allowed);


  cm_client_act(client_ui);
  
  
  // girin. 2001.1.4
  // Decrease RSSI Delta reporting threshold to 2dBm to fit in QC spec.

  /*
  // This will set the RSSI Delta reporting threshold. Use 5 dBm 
  cm_ph_cmd_rssi_delta_threshold(
                    NULL,
                    NULL,
                    client_ui,
                    5 );
    */
    cm_ph_cmd_rssi_delta_threshold(NULL, NULL, client_ui, 2 );
    // girin. end.

  


}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_orig_voice_call

DESCRIPTION
  send voice origination to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_orig_voice_call(
  const byte* num_buf,       // # for origination, could contain pause
  byte        num_len,       // length of the #
  const byte* alpha_buf,     // alpha tag (name) of the origination
  byte        alpha_len      // length of the alpha tag
)
{
  int  i;
  byte digit_count;
  byte work_buf[CM_MAX_NUMBER_CHARS]; //may be static to reduce stack usage

  // the num_buf could contain pause or hyphen that CM does not accept
  // preprosses it so CM take digits only
  digit_count = 0;
  for (i=0; i<num_len; i++) {
    if (ISPAUSE(num_buf[i])) {
      break;
    } else if (num_buf[i] != UI_HYP_CHR) {
      // programm error or buffer definition not reasonable
      ASSERT(digit_count <= sizeof(work_buf));
      work_buf[digit_count++] = num_buf[i];
    }
  }

  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 


  if (GET_FORCE_SO() == CAI_SO_VOICE_WILDCARD)
  {

#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */
    cm_call_cmd_orig(
                  ui_cm_call_cmd_cb,
                  NULL,
                  client_ui,
                  CM_CALL_TYPE_VOICE,
                  CM_SRV_TYPE_AUTOMATIC,
                  0,
                  work_buf,
                  digit_count,
                  CM_DIGIT_MODE_4BIT_DTMF,
                  alpha_buf,
                  alpha_len,
                  CM_OTASP_ACT_CODE_NONE
                  );
#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */
  }
  else
  {
    cm_call_cmd_orig(
                  ui_cm_call_cmd_cb,
                  NULL,
                  client_ui,
                  CM_CALL_TYPE_VOICE,
                  CM_SRV_TYPE_CDMA_SPECIFIC,
                  GET_FORCE_SO(),
                  work_buf,
                  digit_count,
                  CM_DIGIT_MODE_4BIT_DTMF,
                  alpha_buf,
                  alpha_len,
                  CM_OTASP_ACT_CODE_NONE
                  );
    SET_FORCE_SO(CAI_SO_VOICE_WILDCARD);
  }

}



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_orig_test_call

DESCRIPTION
  send test call origination to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_orig_test_call(
  word        cdma_so,       // service option for origination
  const byte* num_buf,       // # for origination, no pause
  byte        num_len,       // length of the #
  const byte* alpha_buf,     // alpha tag (name) of the origination
  byte        alpha_len      // length of the alpha tag
)
{
  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 
  cm_call_cmd_orig(
                  ui_cm_call_cmd_cb,
                  NULL,
                  client_ui,
                  CM_CALL_TYPE_TEST,
                  CM_SRV_TYPE_CDMA_SPECIFIC,
                  cdma_so,
                  num_buf,
                  num_len,
                  CM_DIGIT_MODE_4BIT_DTMF,
                  alpha_buf,
                  alpha_len,
                  CM_OTASP_ACT_CODE_NONE
                  );
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_answer_call

DESCRIPTION
  Send answer command to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_answer_call()
{
  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 


  cm_call_cmd_answer(
                    ui_cm_call_cmd_cb,
                    NULL,
                    client_ui,
                    CM_CALL_TYPE_VOICE
                    );


}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_answer_sms_call

DESCRIPTION
  Send answer SMS call command to Call Manager, will disappear when SMS2.0
  rolls in

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_answer_sms_call()
{
  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 
  cm_call_cmd_answer(
                    ui_cm_call_cmd_cb,
                    NULL,
                    client_ui,
                    CM_CALL_TYPE_SMS
                    );
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_end_call

DESCRIPTION
  Send  call end command to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_end_call()
{
  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 


  ui_stop_fwd_burst_dtmf();

#ifdef FEATURE_UASMS
  if( uiuasms_SMS_call_active == TRUE )
  {
    uasms_DTC_disconnect();
    uiuasms_SMS_call_active = FALSE;
  }
  else
#endif /* FEATURE_UASMS */
  {


  cm_call_cmd_end(
                 ui_cm_call_cmd_cb,
                 NULL,
                 client_ui
                 );


  } /* if - else */


}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_send_flash

DESCRIPTION
  Send  call flash command to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_send_flash(
  const byte* num_buf,     // # to be flashed, could include pause
  byte  num_len            // length of the #
)
{
  int  i;
  byte digit_count;
  byte work_buf[CM_MAX_NUMBER_CHARS]; //may be static to reduce stack usage

  // the num_buf could contain pause or hyphen that CM does not accept
  // preprosses it so CM take digits only
  digit_count = 0;
  for (i=0; i<num_len; i++) {
    if (ISPAUSE(num_buf[i])) {
      break;
    } else if (num_buf[i] != UI_HYP_CHR) {
      // programm error or buffer definition not reasonable
      ASSERT(digit_count <= sizeof(work_buf));
      work_buf[digit_count++] = num_buf[i];
    }
  }

  // disable key input, enabled again when UI receives call event/command error
  ui_get_keys(FALSE); 
  cm_call_cmd_flash(
                 ui_cm_call_cmd_cb,
                 NULL,
                 client_ui,
                 work_buf,
                 digit_count
                 );
}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_power_down

DESCRIPTION
  Send power down command to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_power_down()
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  cm_ph_cmd_oprt_mode(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      CM_OPRT_MODE_PWROFF
                     );
}

void ui_power_reset(void)
{
    ui_get_keys(FALSE); 
  cm_ph_cmd_oprt_mode(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      CM_OPRT_MODE_RESET
                     );
}
/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_oprt_mode

DESCRIPTION
  Send call manager commands to set phone operation mode
  (ONLINE, OFFLINE, RESET, POWEROFF, RESET)

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_set_ph_oprt_mode(
  cm_oprt_mode_e_type oprt_mode  // operation mode to be set
)
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  return cm_ph_cmd_oprt_mode(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      oprt_mode
                     );
}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_sys_pref

DESCRIPTION
  Send call manager commands to set phone system preference
  (A only, B only, Home only, Standard)

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_set_ph_sys_pref(
  cm_sys_pref_e_type  sys_pref   // system preference to be set
)
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  cm_ph_cmd_sys_pref(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      sys_pref
                     );
}



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_mode_pref

DESCRIPTION
  Send call manager commands to set phone mode preference
  (Analog only/pref, Digital only/pref, automatic, E911)
  and termination condition (permanent, powercycle etc)

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_set_ph_mode_pref(
  cm_mode_pref_e_type  mode_pref,    // mode preference to be set
  cm_mode_term_e_type  mode_term,    // mode termination
  dword                mode_duration // time for temp mode preference change
)
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  cm_ph_cmd_mode_pref(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      mode_pref,
                      mode_term,
                      mode_duration
                     );
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_curr_nam

DESCRIPTION
  Send call manager commands to set current NAM for the phone

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_set_ph_curr_nam(
  byte curr_nam     // current NAM to be set
)
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 

  if (GET_PHONE_INFO_NAM_SELECT() == CM_NAM_AUTO) {
    cm_ph_cmd_nam_sel(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      (cm_nam_e_type)curr_nam   
                     );
    cm_ph_cmd_nam_sel(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      CM_NAM_AUTO
                     );
  } else {
    cm_ph_cmd_nam_sel(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      (cm_nam_e_type)curr_nam
                     );
  }

}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_auto_nam

DESCRIPTION
  Send call manager commands to set auto nam on or off

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_set_ph_auto_nam(
  boolean auto_nam_on // set it on or off
)
{
  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  if (auto_nam_on) {
    cm_ph_cmd_nam_sel(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      CM_NAM_AUTO
                     );
  } else {
    cm_ph_cmd_nam_sel(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      SKY_GET_CURRENT_NAM()
                     );
  }

}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_set_ph_answer_voice_as_data

DESCRIPTION
  Send call manager commands to set answer voice as data setting

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
#define VOICE_AS_DATA_TIME   600         // 10 minutes time out

void ui_set_ph_voice_as_data(
  cm_answer_voice_e_type answer_voice   // the setting to be set
)
{
  dword answer_duration;   // duration, useful only for temp setting
  switch (answer_voice) {
    case CM_ANSWER_VOICE_AS_FAX_ONCE:
    case CM_ANSWER_VOICE_AS_MODEM_ONCE:
      answer_duration = VOICE_AS_DATA_TIME;  // timeout after 10 minutes
      break;

    default:
      answer_duration = 0;   // not used, put a dummy value
      break;
  }

  //disable key input, enabled again when UI receives phone event/command error
  ui_get_keys(FALSE); 
  cm_ph_cmd_answer_voice(
                      ui_cm_ph_cmd_cb,
                      NULL,
                      client_ui,
                      answer_voice,
                      answer_duration
                     );

}




/* <EJECT> */
/*===========================================================================
FUNCTION  ui_burst_dtmf

DESCRIPTION
  Send call manager command to send DTMF burst

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  boolean: TRUE if command sent to CM successfully, FALSE if not.

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_burst_dtmf(
  byte on_length,             // DTMF pulse width
  byte off_length,            // DTMF inter-digit interval
  byte cnt,                   // Digit count
  const byte *dtmf_digit_ptr  // Pointer to DTMF digit buffer
)
{
  if ( cm_inband_cmd_burst_dtmf(
                      ui_cm_inband_cmd_cb,
                      NULL,
                      client_ui,
                      on_length,
                      off_length,
                      cnt,
                      dtmf_digit_ptr
                      ) == TRUE)
   { 
     return 1;
   }
   else {
     ERR("DTMF command dropped at CM ...", 0, 0, 0);
     return 0;
   } 
}


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_start_cont_dtmf

DESCRIPTION
  Send call manager command to send continuous DTMF

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  boolean: TRUE if command sent to CM successfully, FALSE if not.

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_start_cont_dtmf(
  byte digit   // DTMF digit 
)
{
  if ( cm_inband_cmd_start_cont_dtmf(
                      ui_cm_inband_cmd_cb,
                      NULL,
                      client_ui,
                      digit
                     ) == TRUE)
  { 
    return 1;
  }
  else {
    ERR("DTMF command dropped at CM ...", 0, 0, 0);
    return 0;
  } 
}

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_stop_cont_dtmf

DESCRIPTION
  Send call manager command to stop continuous DTMF

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  boolean: TRUE if command sent to CM successfully, FALSE if not.

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_stop_cont_dtmf(
  word duration   // DTMF duration
)
{
  if ( cm_inband_cmd_stop_cont_dtmf(
                      ui_cm_inband_cmd_cb,
                      NULL,
                      client_ui,
                      duration
                     ) == TRUE)
  { 
    return 1;
  }
  else {
    ERR("DTMF command dropped at CM ...", 0, 0, 0);
    return 0;
  } 
}


/*===========================================================================
FUNCTION  ui_cm_ss_cmd_cb

DESCRIPTION
  Called by Call manager to indicate status of ss command

DEPENDENCIES
  UI command buffer is available

RETURNS
  None

SIDE EFFECTS
  Add command in UI command buffer
===========================================================================*/
static void ui_cm_ss_cmd_cb (
  void* data_block_ptr,
  cm_ss_cmd_e_type cmd,
  cm_ss_cmd_err_e_type cmd_err
)
{
  ASSERT(data_block_ptr == NULL);  // we do not use data_block_ptr for now 

  MSG_MED("ui_cm_ss_cmd_cb",0,0,0);
}

#if (defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900))
/* <EJECT> */
/*===========================================================================
FUNCTION  ui_ph_cmd_is_mode_valid

DESCRIPTION
  Ask call manager if the mode is valid

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  boolean: TRUE if mode is valid, FALSE if not.

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_ph_cmd_is_mode_valid(
  cm_mode_pref_e_type mode                                            // mode
)
{
  return cm_is_valid_mode_pref( mode ); 
}
#endif

/* <EJECT> */
/*===========================================================================

                Test Menu Functions

===========================================================================*/
void tst_cm_reg_ph_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_ph_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_ph_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_REG,        // event registration
                    CM_PH_EVENT_INFO,           // from this call event
                    CM_PH_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_reg_ph_info Could not Register!!!", 0, 0, 0);
    }
}

void tst_cm_reg_call_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_call_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_call_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_REG,        // event registration
                    CM_CALL_EVENT_INFO,             // from this call event
                    CM_CALL_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_reg_call_info Could not Register!!!", 0, 0, 0);
    }
}

void tst_cm_reg_ss_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_ss_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_ss_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_REG,        // event registration
                    CM_SS_EVENT_INFO,           // from this call event
                    CM_SS_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_reg_ss_info Could not Register!!!", 0, 0, 0);
    }
}

void tst_cm_dereg_ph_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_ph_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_ph_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_DEREG,        // event registration
                    CM_PH_EVENT_INFO,           // from this call event
                    CM_PH_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_dereg_ph_info Could not deRegister!!!", 0, 0, 0);
    }
}

void tst_cm_dereg_call_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_call_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_call_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_DEREG,        // event registration
                    CM_CALL_EVENT_INFO,             // from this call event
                    CM_CALL_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_dereg_call_info Could not deRegister!!!", 0, 0, 0);
    }
}

void tst_cm_dereg_ss_info( )
{
    cm_client_status_e_type return_status;

    return_status = cm_client_ss_reg(  
                    client_ui,                 // pointer to the client struct 
                    ui_cm_ss_event_cb,          // pointer to a cb function for ph events
                    CM_CLIENT_EVENT_DEREG,        // event registration
                    CM_SS_EVENT_INFO,           // from this call event
                    CM_SS_EVENT_INFO,           // to this call event
                    NULL );                     // pointer to a cb function for errors

    if (return_status != CM_CLIENT_OK ) {
        ERR("tst_cm_dereg_ss_info Could not deRegister!!!", 0, 0, 0);
    }
}

void tst_cm_act_ph_info( )
{
    boolean return_status;
  
    return_status = cm_ph_cmd_get_ph_info(
                        ui_cm_ph_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_act_ph_info FAILED!!!", 0, 0, 0);
    }
}

void tst_cm_act_call_info( )
{
    boolean return_status;
  
    return_status = cm_call_cmd_get_call_info(
                        ui_cm_call_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_act_call_info FAILED!!!", 0, 0, 0);
    }
}

void tst_cm_act_ss_info( )
{
    boolean return_status;
  
    return_status = cm_ss_cmd_get_ss_info(
                        ui_cm_ss_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_act_ss_info FAILED!!!", 0, 0, 0);
    }
}


void tst_cm_get_ph_info( )
{
    boolean return_status;
  
    MSG_MED("CM_GET_PH_INFO",0,0,0);
    return_status = cm_ph_cmd_get_ph_info(
                        ui_cm_ph_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_get_ph_info FAILED!!!", 0, 0, 0);
    }
}

void tst_cm_get_call_info( )
{
    boolean return_status;
  
    MSG_MED("CM_GET_CALL_INFO",0,0,0);
    return_status = cm_call_cmd_get_call_info(
                        ui_cm_call_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_get_call_info FAILED!!!", 0, 0, 0);
    }
}

void tst_cm_get_ss_info( )
{
    boolean return_status;
  
    MSG_MED("CM_GET_SS_INFO",0,0,0);
    return_status = cm_ss_cmd_get_ss_info(
                        ui_cm_ss_cmd_cb,
                        NULL,
                        client_ui
                        );

    if (return_status != TRUE ) {
        ERR("tst_cm_get_ss_info FAILED!!!", 0, 0, 0);
    }
}


void tst_ph_info_print(const cm_ph_info_s_type *info_ptr)
{
    
    MSG_MED("is_in_use: %d",info_ptr->is_in_use,0,0);
    MSG_MED("oprt_mode: %d",info_ptr->oprt_mode,0,0);
    MSG_MED("srv_state: %d",info_ptr->srv_state,0,0);
    MSG_MED("cdma_lock_mode: %d",info_ptr->cdma_lock_mode,0,0);
    MSG_MED("answer_voice: %d",info_ptr->answer_voice,0,0);
    MSG_MED("mode_pref: %d",info_ptr->mode_pref,0,0);
    MSG_MED("mode_term: %d",info_ptr->mode_term,0,0);
    MSG_MED("sys_pref: %d",info_ptr->sys_pref,0,0);
    MSG_MED("roam_status: %d",info_ptr->roam_status,0,0);
    MSG_MED("nam_sel: %d",info_ptr->nam_sel,0,0);
    MSG_MED("curr_nam: %d",info_ptr->curr_nam,0,0);
    MSG_MED("rssi: %d",info_ptr->rssi,0,0);

}


void tst_call_info_print(const cm_call_info_s_type *info_ptr)
{

    MSG_MED("call_state: %d",info_ptr->call_state,0,0);
    MSG_MED("call_client: %d",info_ptr->call_client,0,0);
    MSG_MED("call_type: %d",info_ptr->call_type,0,0);
    MSG_MED("srv_opt: %d",info_ptr->srv_opt,0,0);
    MSG_MED("is_privacy: %d",info_ptr->is_privacy,0,0);
    MSG_MED("end_status: %d",info_ptr->end_status,0,0);
    MSG_MED("is_otasp_commit: %d",info_ptr->is_otasp_commit,0,0);

}

void tst_ss_info_print(const cm_ss_info_s_type *info_ptr)
{

    MSG_MED("srv_state: %d",info_ptr->srv_state,0,0);
    MSG_MED("is_registered: %d",info_ptr->is_registered,0,0);
    MSG_MED("band_class: %d",info_ptr->band_class,0,0);
    MSG_MED("block_or_system: %d",info_ptr->block_or_system,0,0);
    MSG_MED("changed_field: %d",info_ptr->changed_fields,0,0);
    MSG_MED("sid: %d",info_ptr->sid,0,0);
    MSG_MED("nid: %d",info_ptr->nid,0,0);
    MSG_MED("reg_zone: %d",info_ptr->reg_zone,0,0);
    MSG_MED("packet_zone: %d",info_ptr->packet_zone,0,0);
    MSG_MED("bs_p_rev: %d",info_ptr->bs_p_rev,0,0);
    MSG_MED("p_rev_in_use: %d",info_ptr->p_rev_in_use,0,0);
}

