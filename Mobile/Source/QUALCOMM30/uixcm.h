#include "target.h"

#if (MMI_TYPE == MMI_A10)
#include <uixcm.h>
#else

#ifndef UINCM_H
#define UINCM_H

/*===========================================================================

           U S E R   I N T E R F A C E   I N T E R N A L     

DESCRIPTION
  This is the interface related to call manager.

 Copyright (c) 1998 by QUALCOMM INCORPORATED  All Rights Reserved.

===========================================================================*/



/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uixcm.h_v   1.2   29 Mar 2000 18:11:08   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/03/99   VT      Merged TriMode Code.
10/15/99   HD      Changed ISVOICE macro to include OTASP, if defined.
10/07/99   CS      Added external test function declarations.
07/29/99   CS      Changed ui_set_ph_oprt_mode from void to boolean.
07/14/99   VT      Initial version. Renamed uincm.h to uixcm.h.

===========================================================================*/



#include "comdef.h"
#include "customer.h"
#include "cm.h"



/* <EJECT> */
/*===========================================================================

                      PUBLIC DECLARATIONS

===========================================================================*/

#ifdef FEATURE_OTASP
#define ISOTASP(call_type) \
        ( ( (call_type) == CM_CALL_TYPE_STD_OTASP ) || \
          ( (call_type) == CM_CALL_TYPE_NON_STD_OTASP ) )
#else
#define ISOTASP(call_type)   FALSE
#endif

#ifdef FEATURE_DS
#define ISDATA(call_type)    ( (call_type) == CM_CALL_TYPE_DATA ) 
#else
#define ISDATA(call_type)    FALSE
#endif

#ifdef FEATURE_UASMS
#define ISSMS(call_type)     ( (call_type) == CM_CALL_TYPE_SMS ) 
#else
#define ISSMS(call_type)     FALSE
#endif

#define ISE911(call_type)    ( (call_type) == CM_CALL_TYPE_E911 ) 

#ifdef FEATURE_OTASP
#define ISVOICE(call_type)   \
        ( ( (call_type) == CM_CALL_TYPE_VOICE )  || \
          ( (call_type) == CM_CALL_TYPE_E911 )   || \
          ( (call_type) == CM_CALL_TYPE_STD_OTASP ) || \
          ( (call_type) == CM_CALL_TYPE_NON_STD_OTASP ) )
#else
#define ISVOICE(call_type)   \
        ( ( (call_type) == CM_CALL_TYPE_VOICE )  || \
          ( (call_type) == CM_CALL_TYPE_E911 ) )

#endif

#define ISTEST(call_type)    ( (call_type) == CM_CALL_TYPE_TEST ) 




/* <EJECT> */
/*===========================================================================

                      PUBLIC FUNCTIONS

===========================================================================*/



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
void ui_set_ph_info_avail(boolean is_avail);


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
boolean ui_ph_info_is_avail(void);


/* <EJECT> */
/*===========================================================================
FUNCTION   ui_cm_init

DESCRIPTION
  initialize UI-CM interface

DEPENDENCIES

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_cm_init(void);




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
boolean ui_call_is_from_ui ( const cm_call_info_s_type* call_info_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION  ui_orig_voice_call

DESCRIPTION
  send voice call origination to Call Manager

DEPENDENCIES
  UI and CM interface initialized

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void ui_orig_voice_call(
  const byte* num_buf,       // # for origination, could contain pause/link
  byte        num_len,       // length of the #
  const byte* alpha_buf,     // alpha tag (name) of the origination
  byte        alpha_len      // length of the alpha tag
);


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
);


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
void ui_end_call(void);


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
void ui_answer_call(void);


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
void ui_answer_sms_call(void);



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
);



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
void ui_power_down(void);
void ui_power_reset(void);

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
);


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
);


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
);





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
);


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
);


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
void ui_set_ph_voice_as_data(
  cm_answer_voice_e_type answer_voice   // the setting to be set
);



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
);

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
);


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
);



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
);



/* <EJECT> */
/*===========================================================================

                Test Menu Functions

===========================================================================*/
extern void tst_cm_reg_ph_info(void );
extern void tst_cm_reg_call_info(void );
extern void tst_cm_reg_ss_info(void );
extern void tst_cm_dereg_ph_info(void );
extern void tst_cm_dereg_call_info(void );
extern void tst_cm_dereg_ss_info(void );
extern void tst_cm_act_ph_info(void );
extern void tst_cm_act_call_info(void );
extern void tst_cm_act_ss_info(void );
extern void tst_cm_get_ph_info(void);
extern void tst_cm_get_call_info(void);
extern void tst_cm_get_ss_info(void);
extern void tst_ph_info_print(const cm_ph_info_s_type *);
extern void tst_call_info_print(const cm_call_info_s_type *);
extern void tst_ss_info_print(const cm_ss_info_s_type *);

#endif

#endif // #if defined (MMI_TREX)