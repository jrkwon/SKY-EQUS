#ifndef UINCALL_H
#define UINCALL_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


          U S E R   I N T E R F A C E   C A L L   S T A T E  

GENERAL DESCRIPTION
  The in call state for UI state machine

  Copyright (c) 1998-2000 by QUALCOMM INCORPORATED.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uiscall.h_v   1.2   29 Mar 2000 18:10:40   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/00   HD      Added support for OTAPA.
===========================================================================*/


#include "cm.h"





/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_cm_call_info

DESCRIPTION
  update part (which is from call manager) of current call information

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_cm_call_info(const cm_call_info_s_type* cm_call_info_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_cm_inband_info

DESCRIPTION
  update part (which is from call manager) of current inband information

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_cm_inband_info(const cm_inband_info_s_type* cm_inband_info_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_start_fwd_burst_dtmf

DESCRIPTION
  Start playing the foward burst DTMF tones

DEPENDENCIES
  None

RETURNS
  void

SIDE EFFECT
  None
===========================================================================*/
void ui_start_fwd_burst_dtmf(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_stop_fwd_burst_dtmf

DESCRIPTION
  Stop playing the foward burst DTMF tones

DEPENDENCIES
  None

RETURNS
  void

SIDE EFFECT
  None
===========================================================================*/
void ui_stop_fwd_burst_dtmf(void);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_setup_voice_privacy_disp

DESCRIPTION
  set up voice privacy display

DEPENDENCIES
  English, size of UI_TIME_F >=12, will be changed later

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_setup_voice_privacy_disp (void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_clear_voice_privacy_disp

DESCRIPTION
  clear voice privacy display

DEPENDENCIES
  English, size of UI_TIME_F >=12, will be changed later

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_clear_voice_privacy_disp (void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_autoanswer_is_ready

DESCRIPTION
  should we autoanswer the call?

DEPENDENCIES
  global variable ui.isanswer, ui.earpiceup, ui.answer, ui.rings

RETURNS
  Ture if autoanswer conditions are met, False otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_autoanswer_is_ready (void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_autoanswer_call

DESCRIPTION
  auto answer the call, record module event if necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_autoanswer_call (void);

#ifdef FEATURE_OTASP_OTAPA 
/*===========================================================================
FUNCTION ui_is_in_otapa_call

DESCRIPTION
  Is the phone in an OTAPA call

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in an OTAPA call?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_otapa_call(void);

#endif /* FEATURE_OTASP_OTAPA  */

/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_call

DESCRIPTION
  Is the phone in a call

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in a call?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_call(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_conversation

DESCRIPTION
  Is the phone in a conversation

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in a conversation?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_conversation(void);



/* <EJECT> */
/*===========================================================================
FUNCTION ui_is_in_alert

DESCRIPTION
  Is the phone getting an incoming call

DEPENDENCIES
  None

RETURN VALUE
  Is the phone in alert ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_is_in_alert(void);


/* <EJECT> */
/*===========================================================================
FUNCTION ui_curr_call_allow_sound

DESCRIPTION
  Is the service option enable playing singal (sound)?

DEPENDENCIES
  None

RETURN VALUE
  Is the service option enable playing singal (sound)?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_allow_sound(void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_mutable

DESCRIPTION
  Is current call mutable ?

DEPENDENCIES
  None

RETURNS
  Is current call mutable ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_mutable(void);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_so

DESCRIPTION
  Get service option of current call

DEPENDENCIES
  None

RETURNS
  service option of current call

SIDE EFFECTS
  None
===========================================================================*/
word ui_curr_call_so(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_is_data

DESCRIPTION
  Is current call a data call ?

DEPENDENCIES
  None

RETURNS
  Is current call a data call ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_is_data(void);


/*===========================================================================
FUNCTION  ui_curr_call_is_sms

DESCRIPTION
  Is current call a sms call ?

DEPENDENCIES
  None

RETURNS
  Is current call a sms call ?

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_curr_call_is_sms(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_data_alpha_tag

DESCRIPTION
  return alpha tag for current data call

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
char* ui_curr_data_alpha_tag(void);



/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_e911_orig_if_needed

DESCRIPTION
  update E911 origination display if it is necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_e911_orig_if_needed(
  cm_srv_state_e_type old_srv_state, // old service state
  cm_srv_state_e_type new_srv_state  // new service state
);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_e911_cb_if_needed

DESCRIPTION
  Update E911 call back display when the service is changed  if it is 
  necessary

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_e911_cb_if_needed(
  cm_srv_state_e_type old_srv_state, // old service state
  cm_srv_state_e_type new_srv_state  // new service state
);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_update_data_status_if_needed

DESCRIPTION
  Update data call status if necessary, will be called every 250 ms

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_update_data_status_if_needed(void);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_curr_call_get_so

DESCRIPTION
  Get the service option of the current call.

DEPENDENCIES
  None

RETURNS
  Service option

SIDE EFFECTS
  None
===========================================================================*/
word ui_curr_call_get_so
(
  void
);


#endif
