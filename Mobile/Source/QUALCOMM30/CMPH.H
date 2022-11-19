#ifndef CMPH_H
#define CMPH_H
/*===========================================================================

         C A L L   M A N A G E R   P H   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMPH.C


Copyright (c) 1991 - 1999 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/CMPH.H_v   1.3   16 Feb 2001 15:35:20   shuang  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/16/01   SH      Added is_end_ack_pending flag
02/06/01   RC      Added support for System Determination 2.0.
01/23/01   ych     Merged T53 and JCDMA features.
12/03/99   VT      Merged TriMode Code.
05/31/99   SH      Added CM 2.0 features
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
12/08/98   RC      Changes per second week of I&T.
11/24/98   RC      Initial release.

===========================================================================*/


#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmcall.h"    /* Interface to CM call object */
#include "nv.h"        /* Interface to NV services */
#include "snd.h"       /* Interface to sound services */
#include "voc.h"       /* Interface to vocoder services */




/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#define CMPH_RSSI_NO_SIGNAL     125  /* -125dBm for no signal */
#define CMPH_DEFAULT_RSSI_DELTA 5    /* default RSSI delta change threshold for
                                      RSSI reporting */


/* Phone type - holds all the information that is associated
** with a phone.
*/
typedef struct cmph_s  {

    cm_init_mask               init_mask;
        /* Indicate whether this object was properly initialized */
    
    boolean                    is_in_use;
         /* Indicate whether phone is currently in use */

    cm_oprt_mode_e_type        oprt_mode;
        /* Indicate current operating mode - online, offline, LPM, etc. */
    
    cm_srv_state_e_type        srv_state;
        /* Indicate current service state - no-service, analog, digital */

    #ifdef FEATURE_SD20
#error code not present
    #endif    
    
    cm_cdma_lock_mode_e_type   cdma_lock_mode;
        /* Indicate whether phone is CMDA locked until power cycle */
    
    cm_answer_voice_e_type     answer_voice;
        /* Indicate whether phone should answer incoming calls as
        ** voice, modem, or fax */

    dword                      answer_uptime;
        /* Indicate uptime for temporary answer-voice settings - i.e. the
        ** uptime when the a temporary answer-voice setting is expired and
        ** phone should restore the normal CM_ANSWER_VOICE_AS_VOICE
        ** setting */
    
    cm_mode_pref_e_type        prst_mode_pref[NV_MAX_NAMS];
        /* Indicate the persistent mode preference setting */
    
    cm_mode_pref_e_type        mode_pref;
        /* Indicate the current mode preference -
        ** analog-only digital-only, etc. */
        
    cm_mode_term_e_type        mode_term;
        /* Indicate the current mode preference term -
        ** one-call, power-cycle, etc.*/
            
    dword                      mode_uptime;
        /* Indicate uptime for temporary mode terms - i.e. the uptime
        ** when the temporary mode is expired and phone should restore
        ** the persistent mode preference */

    cm_sys_pref_e_type         prst_sys_pref[NV_MAX_NAMS];
        /* Indicate the persistent system preference setting */
        
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
    #ifdef FEATURE_SD20
#error code not present
    #else 
    boolean                    is_end_ack_pending;
       /* If call end is requested via MC_END_F then an ack for MC_END_F is expected. 
          The next call orig request can only be sent to MC if 
          the previous call end has been acked by MC or we know the previous
          call has been ended           
       */
    #endif /* FEATURE_SD20 */
   
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
    cm_sys_pref_e_type         sys_pref;
        /* Indicate the current system preference */
       
    cm_roam_status_e_type      roam_status;
        /* Indicate current roaming status */
    
    cm_nam_e_type              nam_sel;
        /* Indicate the NAM selection */
        
    cm_nam_e_type              curr_nam;
        /* Indicate the current NAM -
        ** needed when NAM selection is set to AUTO-NAM */
        
    cmcall_s_type              *call_ptr;
        /* Pointer to a call object containing all the information
        ** that is associated with a call and its states */

    cm_reply_s_type            reply;
        /* CM reply object to handles the details that are associated
        ** with waiting for a reply from MC */
              
    boolean                    is_sr_with_amps;
       /* Indicate whether the phone should try forcing to AMPS
       ** during a silent redial origination */

    uint16                     rssi;
       /* RSSI in positive dBm i.e. 75 means -75dBm */

    uint8                      rssi_delta;
       /* RSSI change exceeds the delta threshold will be reported, default is 5dBm */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

 
} cmph_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the persistent mode preference
** of the current NAM.
**
** NOTE! Phone object must have already been initialized with
** cmph_init().
*/
#define CMPH_PRST_MODE_PREF( ph_ptr ) \
                     ( ph_ptr->prst_mode_pref[ ph_ptr->curr_nam ] )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the persistent system preference
** of the current NAM.
**
** NOTE! Phone object must have already been initialized with
** cmph_init().
*/
#define CMPH_PRST_SYS_PREF( ph_ptr ) \
                     ( ph_ptr->prst_sys_pref[ ph_ptr->curr_nam ] )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to check whether phone is in E911 callback mode.
**
** NOTE! Phone object must have already been initialized with
** cmph_init().
*/
#define CMPH_IS_E911_CALLBACK_MODE( ph_ptr ) \
                     ( (ph_ptr->mode_pref == CM_MODE_PREF_E911) && \
                       (ph_ptr->mode_term == CM_MODE_TERM_PWR_CYCLE) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* CM phone object.
** The phone object is responsible for:
** 1. Processing clients' phone commands.
** 2. Processing MC replies for clients' phone commands.   
** 3. Processing MC phone related notifications.
** 3. Notifying the client list of phone events.
*/



/* <EJECT> */
/*===========================================================================
                          FUNCTION PROTOTYPES
===========================================================================*/


/* <EJECT> */
/*===========================================================================

FUNCTION cmph_ptr

DESCRIPTION
  Return a pointer to the one and only phone object.

  The phone object is responsible for:
  1. Processing clients' phone commands.
  2. Processing MC replies for clients' phone commands.   
  3. Processing MC phone related notifications.
  4. Notifying the client list of phone events.
  
DEPENDENCIES
  none
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmph_s_type  *cmph_ptr( void );       

     

/* <EJECT> */
/*===========================================================================

FUNCTION cmph_init

DESCRIPTION
  Initializing the phone object.

  This function must be called before the phone object 
  is being used, in any way, place, or form.

DEPENDENCIES
  none
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_init( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_is_valid_mode_pref

DESCRIPTION
  Check whether a given mode preference is valid.

DEPENDENCIES
  none
  
RETURN VALUE
  TRUE if mode preference is valid, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_valid_mode_pref(

    cm_mode_pref_e_type    mode_pref
        /* mode preference to check */
);



#ifdef FEATURE_SD20
#error code not present
#else /* FEATURE_SD20 */


/* <EJECT> */
/*===========================================================================

FUNCTION cmph_force_mode_on_the_fly

DESCRIPTION
  Force the mode preference on the fly

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  A ballpark estimation for the number of seconds it takes the phone to
  do a rescan and reacquiring service. If mode is not forced return 0.

SIDE EFFECTS
  none

===========================================================================*/
extern dword cmph_force_mode_on_the_fly(

    cmph_s_type           *ph_ptr,
        /* Pointer to a phone object */
        
    cm_mode_pref_e_type    mode_pref,
        /* Force phone to this mode preference */
        
    cm_mode_term_e_type    mode_term,
        /* Force mode for this term - one-call, power-cycle, etc. */
         
    dword                  mode_duration,
        /* Indicate seconds duration for mode term 
        CM_MODE_TERM_1_CALL_OR_TIME */
    
    cm_sys_pref_e_type     sys_pref
        /* Force phone to this system preference */
);

#endif /* FEATURE_SD20 */



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_unforce_mode_on_the_fly

DESCRIPTION
  Unforce the mode preference on the fly

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  A ballpark estimation for the number of seconds it takes the phone to
  reacquire service. If mode is not forced return 0.

SIDE EFFECTS
  none

===========================================================================*/
extern dword cmph_unforce_mode_on_the_fly(

    cmph_s_type    *ph_ptr
        /* Pointer to a phone object */
);



#if (defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900))
/* <EJECT> */
/*===========================================================================

FUNCTION cmph_otasp_update_mode

DESCRIPTION
  Check the mode in NV and update the mode

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  Phone mode may be changed to reflect the NV item

===========================================================================*/
void cmph_otasp_update_mode( void );
#endif


/* <EJECT> */
/*===========================================================================

FUNCTION cmph_call_start

DESCRIPTION
  Does necessary phone's call start activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_start(

    cmph_s_type    *ph_ptr
        /* Pointer to a phone object */
);



/* <EJECT> */
#if (MMI_TYPE == SKY_UI)
/*===========================================================================

FUNCTION cmph_call_start_with_no_event

DESCRIPTION
  전화가 왔을 때 cmph_call_start에서는 CM_PH_EVENT_IN_USE_STATE 이벤트를 
  UI 쪽으로 날려준다. 그 결과 전화를 받지도 않았는 데, Annunciator의 
  전화기가 통화 중으로 나타난다.
  이를 방지하기 위해 만들어진 함수이다.             2001.05.06   hjcho

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_start_with_no_event(

    cmph_s_type    *ph_ptr
        /* Pointer to a phone object */
);
#endif /* #if (MMI_TYPE == SKY_UI) */


/*===========================================================================

FUNCTION cmph_call_end

DESCRIPTION
  Does necessary phone's call end activities.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_call_end(

    cmph_s_type    *ph_ptr
        /* Pointer to a phone object */
);



#ifdef FEATURE_SD20
#error code not present
#endif /* FEATURE_SD20 */



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_is_waiting_for_reply

DESCRIPTION
  Check whether the phone object is currently waiting for a reply.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  TRUE if phone object is currently waiting for a rely, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_is_waiting_for_reply( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_info_get

DESCRIPTION
  Copy the current phone state information into a specified buffer.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  TRUE if phone information is valid, FALSE otherwise.
  
  Note that phone information only becomes valid after cmph_init()
  returns.
  
SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmph_info_get(

    cm_ph_info_s_type    *ph_info_ptr
        /* Copy phone state info into this buffer */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_client_cmd_proc

DESCRIPTION
  Process clients' phone commands.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_client_cmd_proc( 

    cm_cmd_s_type  *cmd_ptr
        /* pointer to a CM command */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_mc_rpt_proc

DESCRIPTION
  Process MC reports (i.e. notifications of phone activity or
  replies to clients phone commands).


DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_mc_rpt_proc(

    const cm_cmd_s_type    *cmd_ptr
        /* Pointer to a CM command */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_timer_proc(

    cm_timer_event_type    timer_event
        /* Indicate specific timer event */
);

/* <EJECT> */
/*===========================================================================
FUNCTION CMPH_RSSI_TIMER_HANDLE

DESCRIPTION
  This function check the rssi. If RSSI change exceeds the delta threshold,
  CM reports rssi change event to registered clients.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  none
SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_rssi_timer_handle (void);



/* <EJECT> */
/*===========================================================================

FUNCTION cmph_get_curr_nam

DESCRIPTION
  Get the current NAM.

DEPENDENCIES
  Phone object must be initialized.

RETURNS
  The current NAM. If phone was not yet initialized, return CM_NAM_NONE.

SIDE EFFECTS
  None

===========================================================================*/
extern cm_nam_e_type cmph_get_curr_nam( void );


/* <EJECT> */
/*===========================================================================

FUNCTION cmph_autonam_is_enabled

DESCRIPTION
  Check whether auto-NAM is currently enabled.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  TRUE if auto NAM is enabled. FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
extern boolean cmph_autonam_is_enabled( void );

/* <EJECT> */
/*===========================================================================
FUNCTION CMPH_UPDATE_RSSI
DESCRIPTION
  This function update RSSI in the phone object

DEPENDENCIES
  Phone object must be initialized

RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_rssi (void);

/*===========================================================================

FUNCTION cmph_update_srv_state

DESCRIPTION
  update current serving state information in the phone object

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  serving state

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_update_srv_state(void);

/* <EJECT> */
/*===========================================================================
FUNCTION CMPH_OFFLINE

DESCRIPTION
  This function performs offline processing for the phone object
   
DEPENDENCIES
  Phone object must be initialized

RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_offline (void);


/* <EJECT> */
/*===========================================================================

FUNCTION cmph_send_nam_change_to_mc

DESCRIPTION
  Forward a MC_NAM_CHANGED_F command to MC.

DEPENDENCIES
  Phone object must have already been initialized with
  cmph_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
extern void cmph_send_nam_change_to_mc(

    cmph_s_type    *ph_ptr,
        /* Pointer to a phone object */

    cm_nam_e_type  nam
        /* The current NAM */
);

#ifndef FEATURE_SD20
/* <EJECT> */
/*===========================================================================

FUNCTION cmph_set_end_ack_pending

DESCRIPTION

  This function set the end ack pending field in the ph object

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmph_set_end_ack_pending( boolean ack_pending );

/* <EJECT> */
/*===========================================================================

FUNCTION cmph_is_end_ack_pending

DESCRIPTION

  This function return the status of the end ack pending field in the ph object

DEPENDENCIES
  none

RETURN VALUE
  TRUE - phone is waiting for the end ack

SIDE EFFECTS
  none

===========================================================================*/
boolean cmph_is_end_ack_pending( void );

#endif /* !FEATURE_SD20 */

#endif /* CMPH_H */


