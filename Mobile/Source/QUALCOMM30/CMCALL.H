#ifndef CMCALL_H
#define CMCALL_H
/*===========================================================================

         C A L L   M A N A G E R   C A L L   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMCALL.C


Copyright (c) 1991 - 2000 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmcall.h_v   1.11   08 Mar 2001 17:41:42   rotemc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/08/01   RC      Added CMCALL_ACT_REDIAL_SHORT to cmcall_act_e_type.
03/06/01   HQ      Added support for FEATURE_IS2000_REL_A.
02/07/01   RC      Added support for System Determination 2.0.
01/23/01   ych     Merged T53 and JCDMA features.
12/28/00   fc      Added support for retry order.
12/15/00   HQ      Added privacy_pref field.
10/18/00   HQ      Added ruim_last_poll_time in cmcall_s_type.
10/06/00   HD      Added support to pass the DRS bit setting to CP.
08/23/00   vt      Added new CM interface changes.
07/18/00   JQ      Added support for end_reason
03/20/00   RY      Removed cm_flush_hold_cmd_q()
03/13/00   RY      Added OTAPA support
12/13/99   IT      Added handling for next data call as data.
12/03/99   VT      Merged TriMode Code.
11/04/99   SH      CR11880 - Fixed a problem with NDSS. 
10/19/99   SH      Fixed CR11844 - to NDSS connect or not to UI 
09/01/99   SH      Featurized OTASP
08/31/99   SH      Added cmcall_end
08/25/99   SH      Added cmcall_offline
08/19/98   SH      Increased act_tbl size 
08/09/99   SH      Added alert answer bypass feature
07/07/99   SH      Added CNAP feature
06/30/99   SH      Changed type of otasp_act_code to cm_activate_code_type
05/31/99   SH      Added CM 2.0 features
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
11/24/98   RC      Initial release.

===========================================================================*/


#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmmc.h"      /* MC to CM interface*/


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* Enumeration of call origination action.
**
** When call origination fails (due to no-service, fade, reorder, intercept
** release, etc.) the call origination specifies which one of the following
** action should be invoked.
*/
typedef enum cmcall_act_e {

    CMCALL_ORIG_ACT_NONE,        /* No action should be taken */

    CMCALL_ACT_END_ORIG,         /* End the origination process */
    CMCALL_ACT_FORCE_AMPS,       /* Force AMPS */
    CMCALL_ACT_FORCE_AMPS_RSSI,  /* Force AMPS if RSSI is low */
    CMCALL_ACT_REDIAL,           /* Schedule next SR */
    CMCALL_ACT_FORCE_AUTO,       /* Tri-mode */
    CMCALL_ACT_FORCE_AUTO_RSSI,  /* Tri-mode */
    CMCALL_ACT_REDIAL_SHORT,     /* Schedule next SR, shorten if OK to orig*/ 
    CMCALL_ACT_WAIT_OK_TO_ORIG,  /* Wait for ok-to-orig indication (SD 2.0)*/
    
    CMCALL_ORIG_ACT_MAX

} cmcall_act_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for call origination plan.
** Holds all the information that is associated with a particular
** origination plan.
*/
typedef struct cmcall_orig_plan_s {

    cm_feature_e_type    sr_with_amps;
        /* Is this origination plan is for when SR with AMPS feature
        ** is turned ON, OFF, or don't care (NONE=don't care) */

    cm_call_type_e_type  call_type;
        /* What call type is this origination plan for (NONE=don't care) */

    byte                 *dial_str;
        /* What dial string is this origination plan for (NULL=don't care) */

    boolean              is_honor_mc_hold_orig;
        /* Indicate whether we should honor MC hold-orig during the
        ** origination process */

    dword                pwrup_hold_orig_time;
        /* Power-up hold orig uptime - i.e. at power-up how long we should
        ** hold the origination, giving service chance to appear */

    dword                sr_period_time;
        /* Silent redial off period - i.e. time from origination failure
        ** to next redial attempt */

    unsigned int         sr_max_dial;
        /* Max numbers of dial attempts */

    dword                sr_max_time;
        /* Max time for silent redial sequence */


    /* Action table indicating action to take when call origination
    ** fails due to no-service, fade, reorder, intercept, or release.
    */
    struct {
        cm_call_end_e_type end_status;
            /* Origination end status - no-service, fade, reorder, etc */

        cmcall_act_e_type  action;
            /* Action to take - redial, force-AMPS, etc */

    } act_tbl[13];

} cmcall_orig_plan_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Silent redial type - holds all the variables that are
** associated with the silent redial mechanism.
*/
typedef struct cm_sr_s {

    const cmcall_orig_plan_s_type  *orig_plan_ptr;
        /* Pointer to call origination plan */

    dword                          sr_start_uptime;
        /* Silent redial sequence start uptime */

    dword                          sr_max_uptime;
        /* Silent redial sequence max uptime */

    #ifdef FEATURE_SD20
#error code not present
    #endif /* FEATURE_SD20 */
  
    boolean                        is_orig_sent;
        /* Indicate whether origination command was sent to MC */
    
    dword                          hold_orig_uptime;
        /* Hold origination until this uptime */

    dword                          next_orig_uptime;
        /* Try the next origination at this uptime */

    dword                          sr_left;
        /* Indicate how many silent redial left */

    #if ( defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
    boolean                        sr_tried;
        /* Indicates whether silent redial was tried even once */
    #endif

} cm_sr_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Forward declarations to pacify the compiler.
*/
typedef struct cmph_s   *cmph_p_type;     /*lint -esym( 761, cmph_s_type ) */
typedef struct cmcall_s *cmcall_p_type; /*lint -esym( 761, cmcall_s_type ) */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* OTASP call types
*/
typedef enum cmcall_otasp_call_e
{
    CMCALL_OTASP_NONE          /* Regular call other than OTASP */
  , CMCALL_OTASP               /* User initiated service provisioning */
  , CMCALL_PC_OTAPA            /* Network initiated service provisioning 
                               ** by paging the mobile station with SO 18/19 
                               */
  , CMCALL_BUSY_OTAPA          /* Network initiated service provisioning
                               ** while the mobile station is already in 
                               ** CDMA conversation state 
                               */
} cmcall_otasp_call_type;


/* Type for OTASP/OTAPA Information.
*/
typedef struct cmcall_otasp_info_s {

    cmcall_otasp_call_type     otasp_call;
        /* What type of OTASP/OTAPA call */

    cm_activate_code_type      otasp_act_code;
        /* OTASP activation code (for OTASP calls) */

    boolean                    is_commit_successful;
        /* Indicate whether OTASP activation is successful */

    boolean                    is_wait_for_commit_reply;
        /* Indicate whether we need to wait for OTASP-STATUS from MC
        ** at end of call */

    cm_reply_s_type            commit_reply;
        /* CM reply object to handles the details that are associated
        ** with waiting for an otasp commit status reply from MC */        
    
} cmcall_otasp_info_type;


/* Enumeration for call substates in incoming state.
*/
typedef enum cm_call_incom_substate_e {
    
    CM_CALL_INCOM_SUBST_NONE = -1, /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_INCOM_SUBST_REGULAR,   /* All the other SO incoming calls */

    CM_CALL_INCOM_SUBST_OTAPA,     /* OTAPA SO 18/19 incoming call */

    CM_CALL_INCOM_SUBST_MAX        /* FOR INTERNAL USE OF CM ONLY! */

} cm_call_incom_subst_e_type;


/* Enumeration for call substates in conversation state.
*/
typedef enum cm_call_conv_substate_e {

    CM_CALL_CONV_SUBST_NONE = -1,  /* FOR INTERNAL USE OF CM ONLY! */

    CM_CALL_CONV_SUBST_REGULAR,    /* All the other calls */

    CM_CALL_CONV_SUBST_COMMIT,
                                   /* OTASP or busy OTAPA call. */
           /* Commit is in progress */

    CM_CALL_CONV_SUBST_PC_OTAPA_REGULAR,      
                                   /* SO 18/19 OTAPA call */
           /* Without commit (either commit */
           /* never happens or commit finishes */

    CM_CALL_CONV_SUBST_PC_OTAPA_COMMIT,   
                                   /* SO 18/19 OTAPA call */
           /* Commit is in progress. */

    CM_CALL_CONV_SUBSTATE_MAX      /* FOR INTERNAL USE OF CM ONLY! */

} cm_call_conv_subst_e_type;


/* Type for call substate.
*/
typedef union cm_call_substate_u {
   
    cm_call_incom_subst_e_type incom;

    cm_call_conv_subst_e_type  conv;

} cm_call_substate_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Call Type - holds all the information that is associated with
** a specific call.
*/
typedef struct cmcall_s {

    cm_init_mask               init_mask;
        /* Indicate whether this object was properly initialized */

    cm_call_state_e_type       call_state;
        /* Call state - idle, orig, etc. */

    cm_call_substate_type      call_subst;
        /* Call substate. */

#ifdef FEATURE_NEWCM
#error code not present
#else
    cm_client_id_type          call_client;
#endif        
        /* Call client - i.e. originating/answering client */

    cm_call_type_e_type        call_type;
        /* Call type - VOICE, SMS, E911, etc */

    cm_srv_type_e_type         srv_type;
        /* service type - AMPS, CDMA, automatic, etc. */

    cm_mode_pref_e_type        favored_mode_pref;
        /* Favored mode preference to originate the call */

    word                       srv_opt;
        /* service option */

    cm_num_s_type              num;
        /* Number buffer:
        ** CM_CALL_EVENT_ORIG           - contains dialed digits string.
        ** CM_CALL_EVENT_FLASH          - contains dialed digits string.
        ** CM_CALL_EVENT_INCOM          - contains caller ID string.
        ** CM_CALL_EVENT_CALLER_ID      - contains caller ID string.
        ** CM_CALL_EVENT_CONNECTED_NUM  - contains connected number string.
        ** CM_CALL_EVENT_CALLED_PARTY   - contains called party string.
        **                                Note: presentation and screening indicator
        **                                      fields for called party info are not
        **                                      used
        */


    cm_alpha_s_type            alpha;
        /* Alpha tag to be displayed along with dialed address
        ** of an originated call.
        **
        ** NOTE! this is not a phone book name match, but rather
        ** an indication for the UI to display some text for an
        ** outgoing call
        **
        ** We probably need to change this one to a UI text component to
        ** support I18N
        Alpha buffer:
        CM_CALL_EVENT_ORIG
        CM_CALL_EVENT_DISPLAY
        CM_CALL_EVENT_EXT_DISP        */

    cm_signal_s_type           signal;
        /* Alert signal information to be played for an incoming call */

    cm_privacy_pref_e_type     privacy_pref;
        /* Indicate the user preference of privacy mode */

    boolean                    is_privacy;
        /* Indicate whether privacy mode is on */

    cm_call_end_e_type         end_status;
        /* Call end status (i.e. reason for ending a call in origination,
        ** incoming or conversation state) */
    
    cmcall_otasp_info_type     otasp_info;
          /* Contains the necessary information for OTASP/OTAPA calls */

    void (*orig_func)          ( cmcall_p_type call_ptr );
        /* Pointer to origination processing function. Different call types
        ** might have different origination processing functions associated
        ** with them */

    cm_sr_s_type               sr;
        /* Silent redial component. Holds all the information that is
        ** associated with the silent redial mechanism */

    cm_reply_s_type            reply;
        /* CM reply object to handles the details that are associated
        ** with waiting for a reply from MC */

    cmph_p_type                ph_ptr;
        /* Pointer to phone object. Holds all the information that is
        ** associated with the phone and its settings */

    boolean                    is_last_cdma_info_rec;
        /* this field indicates whether this is the last
           CDMA info record or not */


#ifdef FEATURE_IS95B_EXT_DISP
    byte                        ext_disp_type;
        /* display type for extended display info record */
#endif

#ifdef FEATURE_IS95B_ALERT_ANS_BYPASS
    boolean                    alert_ans_bypass;
        /* Indicate whether the incoming call need to be answered by the clients */
#endif /* FEATURE_IS95B_ALERT_ANS_BYPASS  */

#ifdef FEATURE_IS95B_NDSS
    boolean                     ndss_in_progress;
    /* MC is performing NDSS redirection */
    boolean                     ndss_connect;
    /* send NDSS connect instead of connect */
#endif

    boolean                     is_user_originated_call;
    /* is this a call that we are originating? */

#ifdef FEATURE_OTASP_OTAPA
    boolean                     defer_otapa_end_until_signal;
    /* wait until the incoming call signal comes, then end the call */
#endif /* FEATURE_OTASP_OTAPA */

#ifdef FEATURE_IS2000
    byte end_reason;  
      /* the reason for ending a call, it's different from end_status to keep
       * the old interface unchanged while adding more IS2000 specific reasons
       * */
    boolean                    drs_bit;
      /* to set the DRS bit or not */

    cm_retry_order_type         retry_order;
      /* retry order info */
#endif /* FEATURE_IS2000 */

#ifdef FEATURE_T53
#error code not present
#endif /* FEATURE_T53 */

#ifdef FEATURE_RUIM
    dword                       ruim_last_poll_time;
#endif /* FEATURE_RUIM */

} cmcall_s_type;



/* Macros to check the call state and substate.
*/
#define CMCALL_IS_INCOM_OTAPA(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_INCOM) && \
   ((call_ptr)->call_subst.incom == CM_CALL_INCOM_SUBST_OTAPA))

#define CMCALL_IS_INCOM_REGULAR(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_INCOM) && \
   ((call_ptr)->call_subst.incom == CM_CALL_INCOM_SUBST_REGULAR))

#define CMCALL_IS_CONV_REGULAR(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_CONV) && \
   ((call_ptr)->call_subst.conv == CM_CALL_CONV_SUBST_REGULAR))

#define CMCALL_IS_CONV_COMMIT(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_CONV) && \
   ((call_ptr)->call_subst.conv == CM_CALL_CONV_SUBST_COMMIT))

#define CMCALL_IS_CONV_PC_OTAPA_REGULAR(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_CONV) && \
   ((call_ptr)->call_subst.conv == CM_CALL_CONV_SUBST_PC_OTAPA_REGULAR))

#define CMCALL_IS_CONV_PC_OTAPA_COMMIT(call_ptr) \
  (((call_ptr)->call_state == CM_CALL_STATE_CONV) && \
   ((call_ptr)->call_subst.conv == CM_CALL_CONV_SUBST_PC_OTAPA_COMMIT))

#define CMCALL_IS_PC_OTAPA(call_ptr) \
  (CMCALL_IS_INCOM_OTAPA(call_ptr) || \
         CMCALL_IS_CONV_PC_OTAPA_REGULAR(call_ptr) || \
         CMCALL_IS_CONV_PC_OTAPA_COMMIT(call_ptr) )



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_ptr

DESCRIPTION
  Return a pointer to the one and only call object.

  CM call object.
  The call object is responsible for:
  1. Processing clients' call commands.
  2. Processing MC replies for clients' call commands.
  3. Processing MC call related notifications.
  4. Notifying the client list of call events.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmcall_s_type  *cmcall_ptr( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_init

DESCRIPTION
  Initializing the call object.

  This function must be called before the call object
  is being used, in any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmcall_init( void );

/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_is_valid_call_type

DESCRIPTION
  Check whether a call type is valid 
  
DEPENDENCIES
  none

RETURN VALUE
  TRUE if call type is valid, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
boolean cmcall_is_valid_call_type(

    cm_call_type_e_type    call_type
        /* call type */
);

/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_is_valid_srv_type

DESCRIPTION
  Check whether a service type is valid relative to the call type.

DEPENDENCIES
  none

RETURN VALUE
  TRUE if service type is valid relative to the call type,
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmcall_is_valid_srv_type(

    cm_call_type_e_type    call_type,
        /* call type */

    cm_srv_type_e_type     srv_type
        /* service type */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_is_valid_srv_opt

DESCRIPTION
  Check whether a service option is valid relative to the call type.

DEPENDENCIES
  none

RETURN VALUE
  TRUE if service option is valid relative to the call type,
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmcall_is_valid_srv_opt(

    cm_call_type_e_type    call_type,
        /* call type */

    word                   srv_opt
        /* service option */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_is_waiting_for_reply

DESCRIPTION
  Check whether the call object is currently waiting for a reply.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  TRUE if call object is currently waiting for a rely, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmcall_is_waiting_for_reply( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_info_get

DESCRIPTION
  Copy the current call state information into a specified buffer.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  TRUE if call information is valid, FALSE otherwise.

  Note that call information only becomes valid after cmcall_init()
  returns.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmcall_info_get(

    cm_call_info_s_type    *call_info_ptr
        /* Copy call state info into this buffer */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_client_cmd_proc

DESCRIPTION
  Process clients' call commands.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmcall_client_cmd_proc(

    cm_cmd_s_type  *cmd_ptr
        /* pointer to a CM command */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_mc_rpt_proc

DESCRIPTION
  Process MC reports


DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmcall_mc_rpt_proc(

    const cm_cmd_s_type    *cmd_ptr
        /* pointer to a CM command */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_timer_proc

DESCRIPTION
  Process call timer events

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmcall_timer_proc(

    cm_timer_event_type    timer_event
        /* Indicate specific timer event */
);

/* <EJECT> */
/*===========================================================================
FUNCTION CMCALL_OFFLINE

DESCRIPTION
  This function performs offline processing for the call object

DEPENDENCIES
  Call object must be initialized

RETURNS
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmcall_offline (void);


/* <EJECT> */
/*===========================================================================

FUNCTION cmcall_end

DESCRIPTION
  Does call end necessary activities.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmcall_end(

    cmcall_s_type  *call_ptr
        /* pointer to a call object */
);

#ifdef FEATURE_OTASP_HOLD_CMD_IN_COMMIT


/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_handle

DESCRIPTION
  Process clients' call commands

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_hold_cmd_handle(

    void        
);
#endif /* FEATURE_OTASP_HOLD_CMD_IN_COMMIT */


#endif /* CMCALL_H */

