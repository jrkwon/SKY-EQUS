#ifndef CMI_H
#define CMI_H
/*===========================================================================

              C A L L   M A N A G E R   H E A D E R   F I L E

DESCRIPTION
  This header file contains definitions used internally by the Call Manager.
  It provides internal interface to the CM.C file. 
  It should NOT be included by any source file outside the Call Manager

  
Copyright (c) 1991 - 2001 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmi.h_v   1.10   28 Feb 2001 18:00:08   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/01   HQ      Fixed compiler warnings.
02/23/01   HQ      Added CM_RUIM_STATUS_SIG.
02/07/01   RC      Added support for System Determination 2.0.
01/29/01   HQ      Added GPSONE_DBM support when NEWCM is defined.
01/23/01   ych     Merged JCDMA features.
12/15/00   HQ      Added privacy_pref field.
10/06/00   HD      Added support to pass the DRS bit setting to CP.
07/27/00   PJ      Added support for PD commands. (FEATURE_GPSONE_DBM).
08/23/00   vt      Added new CM interface changes.
07/18/00   JQ      Added support for end_reason
03/20/00   RY      Featurized all the cm_commit_reply_* functions with
                   FEATURE_OTASP
03/16/00   VT      Added BROADCAST SMS support
03/13/00   RY      Added OTAPA support
12/18/99   CS      Added cm_kick_dog function.
08/31/99   SH      Modified definitions to featurize UASMS 
08/09/99   SH      Added SMS object
07/07/99   SH      Added CNAP feature
06/30/99   SH      Changed type of otasp_act_code to cm_activate_code_type
06/21/99   SH      added the cmd_err field for all cmd_info structures
05/31/99   SH      added CM 2.0 features
02/04/99   RC      Changes the definition of CM_OTASP_ACT_CODE_NONE and
                   CM_SRV_OPT_NONE to avoid ARM compiler confusion.
01/07/99   RC      Changes per ARM compiler.
12/30/98   RC      Changes per 4th week of I&T.
11/24/98   RC      Initial release.

===========================================================================*/


#include "comdef.h"    /* Definition for basic types and macros */
#include "target.h"    /* Target configuration file */
#include "customer.h"  /* Customer configuration file */
#include "cm.h"        /* External interface to cm.c */
#include "cmmc.h"        /* Interface to UI task */
#include "db.h"        /* Interface to Database services */
#include "snd.h"       /* Interface to sound services */
#include "mc.h"        /* Interface to MC Task */



/* <EJECT> */
/*===========================================================================
=============================================================================
============================= MISCELLANEOUS BLOCK ===========================
=============================================================================
===========================================================================*/

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */


typedef word    cm_timer_event_type;
    /* CM timer event type */

/*---------------------------------------------------------------------------
                        BASIC TYPES, MACROS & FUNCTIONS
---------------------------------------------------------------------------*/

/* Init mask to identify whether a specific object was ever initialized.
** During object initialization the object's init_mask parameter is set
** to a unique mask of alternating 1's and 0's (i.e. 1010...).
** Later the init_mask parameter can be checked against this value to
** confirm that the object was properly initialized.
*/
    
/* Init mask value.
*/ 
#define CM_INIT_MASK    0x66


/* Macro to mark an object as properly initialized.
*/
#define CM_INIT_MARK( mask )   ( (mask) = CM_INIT_MASK )
    

/* Macro to check whether an object was properly initialized.
*/
#define CM_INIT_CHECK( mask )  ( (mask) == CM_INIT_MASK )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                            
/* Enumeration of CM feature states.
*/
typedef enum cm_feature_e {

    CM_FEATURE_NONE=-1,      /* Don't care if feature is turned ON/OFF */
    
    CM_FEATURE_OFF = FALSE,  /* Feature is turned ON */
    CM_FEATURE_ON  = TRUE,   /* Feature us turned OFF */
    
    CM_FEATURE_MAX

} cm_feature_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to check whether val is between min and max
** (not include min and max).
*/
#define BETWEEN( val, min, max )  ( (val) > (min) && (val) < (max) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to check whether val is in the range of min to max
** (including min and max).
*/
#define INRANGE( val, min, max )  ( (val) >= (min) && (val) <= (max) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/* Macro to check whether a character is an ASCII alpha.
*/ 
#define IS_ALPHA( c ) ( ((c) >= 'a' && (c) <= 'z') || \
                        ((c) >= 'A' && (c) <= 'Z') )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to safely decrement to 0. 
*/
#define DEC( num )  ( (num) > 0 ? (--(num)) : (num) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to safely assign a value to a boolean variable. 
*/
#define BOOLEAN( val )  ( (val) ? TRUE : FALSE )


/* <EJECT> */
/*---------------------------------------------------------------------------
                          CIRCULAR QUEUE MACROS
---------------------------------------------------------------------------*/


/* Macros to operate on a circular queue that is implemented as a
** contiguous array of elements in memory.
*/ 


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to define a circular queue of a specified type, name and size.
*/
#define CIRC_Q_DEF( type, name, size ) struct { unsigned int  head;      \
                                                unsigned int  tail;      \
                                                unsigned int  cnt;       \
                                                type arr[size];          \
                                              } name


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to initialize a circular queue.
*/
#define CIRC_Q_INIT( q )  q.tail = q.head = q.cnt = 0;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return the size of a circular queue.
*/
#define CIRC_Q_SIZE( q ) ARR_SIZE( q.arr )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return the count (i.e. the number of elements) of a circular
** queue.
*/
#define CIRC_Q_CNT( q ) ( q.cnt )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return the next index of a circular queue relatively
** to a given index.
*/
#define CIRC_Q_NEXT( q_size, q_pos ) ((q_pos)+1 >= (q_size) ? 0: ((q_pos)+1))


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 
/* Macro to increment an index (i.e. head or tail) of a circular queue.
*/
#define CIRC_Q_INC( q_size, q_pos )  ( q_pos = CIRC_Q_NEXT( q_size, q_pos ) )



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to enqueue an item onto a circular queue.
** Return TRUE if item is successfully enqueued, else return FALSE.
*/
#define CIRC_Q_PUT( q, item )                                               \
 ( q.cnt >= CIRC_Q_SIZE(q) ? FALSE : ( q.arr[q.tail] = item,                \
                                       CIRC_Q_INC( CIRC_Q_SIZE(q), q.tail), \
                                       q.cnt++,                             \
                                       TRUE ) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                

/* Macro to dequeue an item from a circular queue.
** Return TRUE if item is successfully dequeued, else return FALSE.
*/
#define CIRC_Q_GET( q, item )                                               \
    ( q.cnt == 0 ? FALSE : ( item = q.arr[q.head],                          \
                             CIRC_Q_INC( CIRC_Q_SIZE(q), q.head ),          \
                             q.cnt--,                                       \
                             TRUE ) )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the item at the head of a circular
** queue. If queue is empty a NULL is returned.
*/
#define CIRC_Q_CHECK( q ) ( q.cnt == 0 ? NULL : &q.arr[q.head] )



/* <EJECT> */
/*---------------------------------------------------------------------------
                              NUMBER FUNCTIONS
---------------------------------------------------------------------------*/


/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_init

DESCRIPTION
  Initialize a CM number object.

  This function must be called on a CM number object before it being used,
  in any way, place, or form.
  
DEPENDENCIES
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_init
(cm_num_s_type *num_ptr);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_copy

DESCRIPTION
  Copy a byte buffer number into a specified CM number.

DEPENDENCIES
  Specified CM number must have already been initialized with
  cm_num_init()
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_copy(

    cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number */

    const byte*      num_buf,
        /* Pointer to a number to be copied into CM number */
    
    byte             num_len
        /* Length of number to be copied */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_ep_msg_copy

DESCRIPTION
  Copy an IS91 Extended Protocol Short Message into CM number.

DEPENDENCIES
  Specified CM number must have already been initialized with
  cm_num_init()
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_ep_msg_copy(

    cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number */
    
    byte*            num_buf,
        /* Pointer to a number to be copied into CM number */
    
    byte             num_len
        /* Length of number to be copied */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_ep_cli_copy

DESCRIPTION
  Copy IS91 Extended Protocol Caller Line ID into a CM number.

DEPENDENCIES
  Specified CM number must have already been initialized with
  cm_num_init()

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_ep_cli_copy(

    cm_num_s_type    *cm_num_ptr,
        /* Pointer to a CM number */
    
    byte*            num_buf,
        /* Pointer to a number to be copied into CM number */
    
    byte             num_len
        /* Length of number to be copied */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_attrib_copy

DESCRIPTION
  Set the attributes of a CM number to a specified set of attributes

DEPENDENCIES
  CM number must have already been initialized with
  cm_num_init()
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_attrib_copy(

    cm_num_s_type           *num_ptr,
        /* Pointer to a CM number */

    byte                    number_type,
        /* Number type - see J-STD-008 2.7.1.3.2.4-2
        ** NOTE! ONLY VALID FOR CDMA CALLS */
        
    byte                    number_plan,
        /* Numbering Plan - see J-STD-008 2.7.1.3.2.4-3
        ** NOTE! ONLY VALID FOR CDMA CALLS */
        
    byte                    pi,
        /* Presentation Indicator - see J-STD-008  2.7.4.4-1 */

    byte                    si,
        /* Screening Indicator - see J-STD-008 2.7.4.4-2 */

    cm_digit_mode_e_type    digit_mode
        /* Origination digit mode - see J-STD-008 2.7.1.3.2.4 */ 
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_num_assert

DESCRIPTION
  Check a series of assertions over a CM number to validate its state
  
DEPENDENCIES
  CM number must have already been initialized with
  cm_num_init()
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_num_assert(

    const cm_num_s_type    *num_ptr
        /* Pointer to a CM number to be verified */
);



/* <EJECT> */
/*---------------------------------------------------------------------------
                       PHONE-BOOK INTERFACE FUNCTIONS
---------------------------------------------------------------------------*/


/* <EJECT> */
/*===========================================================================

FUNCTION cm_nc_resolve_call_type

DESCRIPTION
  Resolve originating number type.
  
  This functions consults the number-classification to whether the originating number
  matches one of the special numbers that are stored in the phone-book
  (i.e. E911, OTASP, etc.). If a match is found, it modifies the originating
  call type accordingly.

DEPENDENCIES
  number-classification interface object must have already been initialized with
  cm_nc_init().

RETURN VALUE
  none
  
SIDE EFFECTS
  Modifies the originating call type (pointed by activate_code_ptr) and or
  the activation code (pointed by activate_code_ptr).

===========================================================================*/
extern void cm_nc_resolve_call_type(  

    cm_num_s_type            *orig_num_ptr,
        /* pointer to originating number */
    
    cm_call_type_e_type      *call_type_ptr,
        /* pointer to associated call type */
      
    cm_activate_code_type    *activate_code_ptr
        /* pointer to associated OTASP activation code
        ** (for OTASP call types only) */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_nc_is_call_allowed_func

DESCRIPTION
  Decide whether an originating number is allowed.
  
  This functions consults number-classification to whether the originating number
  is allowed by the origination restriction currently in effect.

DEPENDENCIES
  number-classification interface object must have already been initialized with
  cm_nc_init().
  
RETURN VALUE
  TRUE of originating number is allowed for origination, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_nc_is_call_allowed_func(

    cm_num_s_type            *orig_num_ptr
        /* pointer to originating number */
);



/* <EJECT> */
#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */



/* <EJECT> */
/*---------------------------------------------------------------------------
                       GENERAL CM SERVICES FUNCTIONS
---------------------------------------------------------------------------*/


/* <EJECT> */
/*===========================================================================

FUNCTION cm_wait

DESCRIPTION
  Suspend CM from executing until at least one of the specified signals
  in the input mask is set.
  
  Note that this function keep reporting to the watchdog while waiting
  for the signal(s) to be set.

DEPENDENCIES
  cm_rpt_timer timer must have been defined with rex_def_timer().

RETURN VALUE
  void
  
SIDE EFFECTS
  None

===========================================================================*/
extern void cm_wait(

    rex_sigs_type    mask
        /* Mask of signals to wait on */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_mc_rpt_get_buf_else_err_fatal

DESCRIPTION
  Allocate a CM report command buffer from the cm_mc_rpt_free_q. Error
  fatal if allocation is NOT successful.

DEPENDENCIES
  none

RETURN VALUE
  Pointer to allocated CM report command buffer.

SIDE EFFECTS
  none

===========================================================================*/
extern cm_mc_rpt_type *cm_mc_rpt_get_buf_else_err_fatal( void );



/* <EJECT> */
/*===========================================================================
=============================================================================
=========================== CLIENT INTERFACE BLOCK ==========================
=============================================================================
===========================================================================*/


/*---------------------------------------------------------------------------
                                  CM COMMAND
---------------------------------------------------------------------------*/


/* Enum of CM command types.
*/
typedef enum cm_cmd_type_e {

    CM_CMD_TYPE_NONE = -1,

    CM_CMD_TYPE_CALL,         /* Client call command */
    CM_CMD_TYPE_PH,           /* Client phone command */
    CM_CMD_TYPE_SS,           /* Client serving system command */
    CM_CMD_TYPE_INBAND,       /* Client inband command */
    CM_CMD_TYPE_SMS,          /* Client SMS command */
    CM_CMD_TYPE_MC_RPT,       /* MC report */

    #ifdef FEATURE_GPSONE_DBM
#error code not present
    #endif /* FEATURE_GPSONE_DBM */

    #ifdef FEATURE_JCDMA_MONITOR
#error code not present
    #endif

    CM_CMD_MAX

} cm_cmd_type_e_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Define values indicating empty (or inactive) command fields,
*/
#define CM_SRV_OPT_NONE             ( (word) (0xFFFF) )
#define CM_NUMBER_LEN_NONE          ( 0 )
#define CM_ALPHA_LEN_NONE           ( 0 )
#ifdef FEATURE_IS95B_EXT_DISP
/* display type for CDMA extended display info record */
#define CM_EXT_DISP_TYPE_NONE  ( (byte) 0xFF )
#endif /* FEATURE_IS95B_EXT_DISP */
#define CM_CLIENT_NONE           ( 0 )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold call commands information.
*/
typedef struct cm_call_cmd_info_s {

    cm_call_cmd_err_e_type     cmd_err;
        /* error in command request */

    cm_call_type_e_type        call_type;
        /* Call type */

    cm_srv_type_e_type         srv_type;
        /* Service type - AMPS, CDMA, etc. */
        
    cm_mode_pref_e_type        favored_mode_pref;
        /* Call mode preference */
    
    word                       srv_opt;
        /* service option */

    cm_num_s_type              num;
        /* Number buffer:
        ** Originated calls - contains dialed address.
        ** Flash with info  - contains dialed address. */

    cm_alpha_s_type            alpha;
        /* Client specified alpha tag to be displayed along
        ** with dialed address of an originated call.
        ** NOTE! THIS IS NOT A PHONE BOOK NAME MATCH */

    cm_call_end_e_type         end_status;
        /* Call end status (i.e. reason for ending a call in origination,
        ** incoming or conversation state) */

    cm_activate_code_type      otasp_act_code;
        /* OTASP call activation code */

#ifdef FEATURE_IS2000
    byte                       end_reason;
        /* reason for ending the call */
    boolean                    drs_bit;
        /* set the DRS bit or not */
#endif /* FEATURE_IS2000 */

    cm_privacy_pref_e_type     privacy_pref;

} cm_call_cmd_info_s_type;



/* Type for call commands.
*/
typedef struct cm_call_cmd_s {

    cm_call_cmd_cb_f_type      *cmd_cb_func;
        /* pointer to a client provided callback function */
          
    void                       *data_block_ptr;
        /* pointer to a client provided callback data block */

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

    cm_client_id_type          client;
        /* Client sending this command */

    cm_call_cmd_e_type         cmd;
        /* CM command */

    cm_call_cmd_info_s_type    info;
        /* CM command information */

} cm_call_cmd_s_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold phone command information.
*/
typedef struct cm_ph_cmd_info_s {
    
    cm_ph_cmd_err_e_type     cmd_err;
        /* error in command request */

    cm_oprt_mode_e_type         oprt_mode;
      /* operating mode */
    
    cm_answer_voice_e_type      answer_voice;
        /* indicate whether incoming calls answered as voice,
        ** modem, fax, etc */

    dword                       answer_duration;
        /* indicate seconds duration for temp. answer-voice settings,
        such as CM_ANSWER_VOICE_AS_FAX_ONCE or DB_VOICE_AS_MODEM_ONCE */
    
    cm_mode_pref_e_type         mode_pref;
        /* mode preference */
        
    cm_mode_term_e_type         mode_term;
        /* indicate the mode term */
            
    dword                       mode_duration;
        /* indicate seconds duration for temp. mode term */

    cm_sys_pref_e_type          sys_pref;
        /* system preference */

    #ifdef FEATURE_SD20
#error code not present
    #endif /* FEATURE_SD20 */
    
    cm_cdma_lock_mode_e_type    cdma_lock_mode;
        /* indicate whether phone is CDMA locked until power cycle */

    cm_nam_e_type               nam_sel;
        /* indicate current NAM selection */

    uint8                       rssi_delta;
        /* RSSI change delta threshold for RSSI reporting */

#ifdef FEATURE_JCDMA_DS
#error code not present
#endif /* FEATURE_JCDMA_DS */

#ifdef FEATURE_JCDMA
#error code not present
#endif /* FEATURE_JCDMA */

} cm_ph_cmd_info_s_type;



/* Type for phone commands.
*/
typedef struct cm_ph_cmd_s {

    cm_ph_cmd_cb_f_type         *cmd_cb_func;
        /* pointer to a client provided callback function */

    void                        *data_block_ptr;
        /* pointer to a client provided callback data block */

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

    cm_client_id_type           client;
        /* client sending this command */

    cm_ph_cmd_e_type            cmd;
        /* command */

    cm_ph_cmd_info_s_type       info;
        /* command information */

} cm_ph_cmd_s_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type for serving system commands.
*/
typedef struct cm_ss_cmd_s {

    cm_ss_cmd_cb_f_type         *cmd_cb_func;
        /* pointer to a client provided callback function */

    void                        *data_block_ptr;
        /* pointer to a client provided callback data block */

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

    cm_client_id_type           client;
        /* client sending this command */

    cm_ss_cmd_e_type            cmd;
        /* command */


} cm_ss_cmd_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold inbnad commands information.
*/
typedef struct cm_inband_cmd_info_s {

 cm_inband_cmd_err_e_type     cmd_err;
        /* error in command request */
 uint8             on_length;    
     /* DTMF pulse width */
 uint8             off_length;    
     /* DTMF inter-digit interval */
 uint8             cnt;          
     /* digit count */
 uint8             digits[CM_INBAND_MAX_REV_DIALED_DIGITS]; 
     /* DTMF digit buffer */
 uint16            duration;
    /* Continuous DTMF duration; in milliseconds */
} cm_inband_cmd_info_s_type;



/* Type for inbnad commands.
*/
typedef struct cm_inband_cmd_s {

    cm_inband_cmd_cb_f_type     *cmd_cb_func;
        /* pointer to a client provided callback function */
          
    void                        *data_block_ptr;
        /* pointer to a client provided callback data block */

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

    cm_client_id_type           client;
        /* Client sending this command */

    cm_inband_cmd_e_type        cmd;
        /* CM command */

    cm_inband_cmd_info_s_type   info;
        /* CM command information */

} cm_inband_cmd_s_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Type to hold SMS commands information.
*/
#ifdef FEATURE_UASMS
typedef struct cm_sms_cmd_info_s {

 cm_sms_cmd_err_e_type     cmd_err;
     /* error in command request */

 cm_sms_awi_ack_s_type     awi_ack;            
     /* field for awi ack */  
 
 uint8                   *data;               
     /* pointer to MO SMS buffer */

 boolean                 l2_ack_requested;   
    /* l2 ack required or not - used by MO SMS*/

#ifdef FEATURE_BROADCAST_SMS
 boolean                 bc_enable;
#endif /* FEATURE_BROADCAST_SMS */

} cm_sms_cmd_info_s_type;



/* Type for SMS commands.
*/
typedef struct cm_sms_cmd_s {

    cm_sms_cmd_cb_f_type    *cmd_cb_func;
        /* pointer to a client provided callback function */
          
    void                    *data_block_ptr;
        /* pointer to a client provided callback data block */

#ifdef FEATURE_NEWCM
#error code not present
#endif /* FEATURE_NEWCM */

    cm_client_id_type       client;
        /* Client sending this command */

    cm_sms_cmd_e_type       cmd;
        /* CM command */

    cm_sms_cmd_info_s_type  info;
        /* CM command information */

} cm_sms_cmd_s_type;

#endif /* FEATURE_UASMS */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type for MC reports */
typedef struct cm_mc_rpt_s {

    cm_mc_rpt_type          *cm_mc_rpt_ptr;
        /* pointer to MC report */

} cm_mc_rpt_s_type;



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Union of all CM command types
*/
typedef union cm_cmd_u {

    cm_call_cmd_s_type              call;
    cm_ph_cmd_s_type                ph;
    cm_ss_cmd_s_type                ss;
    cm_inband_cmd_s_type            inband;
#ifdef FEATURE_UASMS
    cm_sms_cmd_s_type               sms;
#endif /* FEATURE_UASMS */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif
    cm_mc_rpt_s_type                mc_rpt;

} cm_cmd_u_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Type to hold all CM commands.
*/
typedef struct cm_cmd_s {
        
    q_link_type           link;
        /* Q linking element
         
           This element has to be the first
           element in this data structure
           because of removal of self_ptr 
           in q_link_type 
         */


    cm_cmd_type_e_type    cmd_type;
        /* Type of CM command */

    cm_cmd_u_type         cmd;
        /* CM command information */

} cm_cmd_s_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return the type of a CM command.
*/
#define CMD_TYPE( cmd_ptr )             (cmd_ptr->cmd_type )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the call command part of a CM command.
*/
#define CALL_CMD_PTR( cmd_ptr )         ( &cmd_ptr->cmd.call )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the phone command part of a CM command.
*/
#define PH_CMD_PTR( cmd_ptr )           ( &cmd_ptr->cmd.ph )


/* Macro to return a pointer to the serving system command part of a CM command.
*/
#define SS_CMD_PTR( cmd_ptr )           ( &cmd_ptr->cmd.ss )


/* Macro to return a pointer to the inband command part of a CM command.
*/
#define INBAND_CMD_PTR( cmd_ptr )       ( &cmd_ptr->cmd.inband )


#ifdef FEATURE_UASMS
/* Macro to return a pointer to the SMS command part of a CM command.
*/
#define SMS_CMD_PTR( cmd_ptr )          ( &cmd_ptr->cmd.sms )
#endif /* FEATURE_UASMS */

#ifdef FEATURE_GPSONE_DBM
#error code not present
#endif /* FEATURE_GPSONE_DBM */

#ifdef FEATURE_JCDMA_MONITOR
#error code not present
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the MC report
*/
#define MC_RPT_PTR( cmd_ptr )  ( &cmd_ptr->cmd.mc_rpt )


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* Macro to return a pointer to the information part of a CM command.
*/
#define CMD_INFO_PTR( xx_cmd_ptr )    ( &xx_cmd_ptr->info )



/* <EJECT> */
/*===========================================================================

FUNCTION cm_cmd_dealloc

DESCRIPTION
  De-allocate a CM command buffer by returning it to the
  CM free command queue.

DEPENDENCIES
  CM free command queue must have already been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_cmd_dealloc( cm_cmd_s_type  *cmd_ptr );



/* <EJECT> */
/*===========================================================================

FUNCTION cm_cmd_client_act

DESCRIPTION
  Inform CM that a client was activated so that CM can inform the client
  when the phone information become available.
  
  This is mostly important during powerup, where a client needs to get
  the phone information in order to make powerup decisions.
  
DEPENDENCIES
  cm_init_before_task_start() must have already been called.
   
RETURN VALUE
  none

SIDE EFFECTS

===========================================================================*/
void cm_cmd_client_act(

        cm_ph_cmd_cb_f_type        cmd_cb_func,
            /* client callback function */
          
        void                       *data_block_ptr,
            /* pointer to client callback data block */
                        
#ifdef FEATURE_NEWCM
#error code not present
#else /* FEATURE_NEWCM */
        cm_client_s_type           *client_ptr
            /* Requesting client */
#endif /* FEATURE_NEWCM */

);



/*---------------------------------------------------------------------------
                                 REPLY OBJECT 
---------------------------------------------------------------------------*/


/* Type for CM reply.
** Handles the details that are associated with setting up a CM command
** to wait for a reply from MC.
*/  
typedef struct cm_reply_s {

    cm_cmd_s_type              *cmd_ptr;
        /* Pointer to a CM command waiting for a reply from MC */    
  
    cm_name_type               cm_mc_rpt;
        /* The mc reply  for which we are waiting */
    
    dword                      uptime;
        /* An uptime upper bound for how long we should wait
        ** for the reply to come */

} cm_reply_s_type;



/* <EJECT> */
/*===========================================================================

FUNCTION mc_reply_init

DESCRIPTION
  Initialize a CM reply object.

  This function must be called on a CM reply object before it being used,
  in any way, place, or form.

DEPENDENCIES
  none
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_reply_init( 

    cm_reply_s_type    *reply_ptr
        /* Pointer to a reply object to be initialized */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_reply_set

DESCRIPTION
  Setup necessary conditions for putting a CM command on a wait
  for reply from MC.

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_reply_set(

    cm_reply_s_type          *reply_ptr,
        /* Pointer to a reply object */

    cm_cmd_s_type      *cmd_ptr,
        /* Pointer to a CM command waiting for a reply from MC */    
   
    cm_name_type       mc_rpt_id,
        /* Indicate the MC report we are waiting for */

    dword                    reply_time
        /* Indicate an uptime upper bound for how long we should wait
        ** for the reply to come */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_reply_get

DESCRIPTION
  Get a pointer to the CM command waiting for a replay from MC.

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().
  
RETURN VALUE
  A pointer to the CM command waiting for a replay from MC.
  If no command is waiting for reply, return NULL.

SIDE EFFECTS
  none

===========================================================================*/
extern cm_cmd_s_type  *cm_reply_get(

    cm_reply_s_type    *reply_ptr
        /* Pointer to a reply object */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_reply_check

DESCRIPTION
  Check whether a specified mc report is the reply for which
  we are waiting.

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().

RETURN VALUE
  TRUE if the given mc report matches the replay for which
  we are waiting, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/

boolean cm_reply_check(

    cm_reply_s_type    *reply_ptr,
        /* Pointer to a reply object */

    cm_name_type       cm_mc_rpt
        /* Are we waiting for this mc report */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_reply_is_waiting

DESCRIPTION
  Check whether we are currently waiting for a reply.

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().
  
RETURN VALUE
  TRUE if we are currently waiting for a rely, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_reply_is_waiting(

    cm_reply_s_type    *reply_ptr
        /* Pointer to a reply object */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_reply_too_long

DESCRIPTION
  Check whether are waiting for too long on a reply

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().
  
RETURN VALUE
  TRUE if are waiting for too long on a reply, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_reply_too_long(

    cm_reply_s_type    *reply_ptr
        /* Pointer to a reply object */
);





/* <EJECT> */
/*===========================================================================
=============================================================================
=========================== SOUND INTERFACE BLOCK ===========================
=============================================================================
===========================================================================*/


/* <EJECT> */
/*===========================================================================

FUNCTION cm_snd_set_filters

DESCRIPTION
  Set sound filtering for VOICE or DATA.

DEPENDENCIES
  mc_task_start() must have already been called.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_snd_set_filters( 

    snd_filters_type  filters
        /* Indicate VOICE or DATA filters */
);



/* <EJECT> */
/*===========================================================================
=============================================================================
============================= MC INTERFACE BLOCK ============================
=============================================================================
===========================================================================*/


/* <EJECT> */
/*===========================================================================
 
FUNCTION cm_mc_get_buf_else_err_fatal

DESCRIPTION
  Allocate a MC Task command buffer. Error fatal if allocation is NOT
  successful. 
 
DEPENDENCIES
  none

RETURN VALUE
  Pointer to allocated MC command buffer.

SIDE EFFECTS
  none

===========================================================================*/
extern mc_msg_type *cm_mc_get_buf_else_err_fatal( void );



/* <EJECT> */
/*===========================================================================
 
FUNCTION cm_mc_send_cmd

DESCRIPTION
  Send a command to the MC task.
 
DEPENDENCIES
  none

RETURN VALUE
  none
  
SIDE EFFECTS
  none

===========================================================================*/
extern void cm_mc_send_cmd( 
  
  mc_msg_type  *mc_cmd_ptr
      /* Pointer to mc command */
);


#ifdef FEATURE_OTASP_HOLD_CMD_IN_COMMIT

/*===========================================================================

FUNCTION cm_hold_cmd_free_q_init

DESCRIPTION
  Initialize the hold command free queue.

  This function must be called before the hold command free queue is used.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_hold_cmd_free_q_init( void );


/*===========================================================================

FUNCTION cm_hold_cmd_q_init

DESCRIPTION
  Initialize the hold command queue.

  This function must be called before the hold command free queue is used.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_hold_cmd_q_init( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_q_get

DESCRIPTION
  Dequeue a command from the head of the CM command queue.

DEPENDENCIES
  cm_free_cmd_q_init() must have already been called.

RETURN VALUE
  A pointer to the dequeued command buffer.
  If the Q is empty A NULL is returned.

SIDE EFFECTS
  none

===========================================================================*/
extern cm_cmd_s_type *cm_hold_cmd_q_get( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_q_put

DESCRIPTION
  Enqueue a command buffer onto the CM hold command queue.

DEPENDENCIES
  cm_free_cmd_q_init() must have already been called.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_hold_cmd_q_put(

    cm_cmd_s_type *cmd_ptr
        /* pointer to a command buffer to be enqueued */
);




/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_q_check

DESCRIPTION
  Check if the passed in command matches with the items in hold command q.
  It only checks the cmd type and cmd. It does not check for the contents
  of command, for example, the dialed digits for origination cmd.

DEPENDENCIES
  cm_hold_cmd_q_init() must have already been called.

RETURN VALUE
  TRUE if cmd type and cmd are same.
  FALSE if not.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_hold_cmd_q_check(

    cm_cmd_s_type *cmd_ptr
        /* pointer to a command buffer to be enqueued */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_alloc

DESCRIPTION
  Allocate a CM command buffer from the CM free command queue.

DEPENDENCIES
  CM free command queue must have already been initialized.

RETURN VALUE
  A pointer to the allocated command buffer.
  If the Q is empty A NULL is returned.

SIDE EFFECTS
  none

===========================================================================*/
extern cm_cmd_s_type *cm_hold_cmd_alloc( void );




/* <EJECT> */
/*===========================================================================

FUNCTION cm_hold_cmd_dealloc

DESCRIPTION
  Deallocate a CM command buffer by returning it to the
  CM free command queue.

DEPENDENCIES
  CM free command queue must have already been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cm_hold_cmd_dealloc( cm_cmd_s_type  *cmd_ptr );



#endif /* FEATURE_OTASP_HOLD_CMD_IN_COMMIT */




#ifdef FEATURE_OTASP
/* <EJECT> */
/*===========================================================================

FUNCTION cm_commit_reply_set

DESCRIPTION
  Setup necessary conditions to wait for MC's OTASP commit status command.  

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

extern void cm_commit_reply_set(

    cm_reply_s_type    *reply_ptr,
        /* Pointer to a reply object */

    cm_cmd_s_type      *cmd_ptr,
        /* Pointer to a CM command waiting for a reply from MC */

    cm_name_type       mc_rpt_id,
        /* Indicate the MC report we are waiting for */

    dword              reply_time
        /* Indicate an uptime upper bound for how long we should wait
        ** for the reply to come */
);


/* <EJECT> */
/*===========================================================================

FUNCTION cm_commit_reply_check

DESCRIPTION
  Check whether a specified mc report is the reply for which
  we are waiting: OTASP commit status command.

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().

RETURN VALUE
  TRUE if the given mc report matches the replay for which
  we are waiting, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/

extern boolean cm_commit_reply_check(

    cm_reply_s_type    *reply_ptr,
        /* Pointer to a reply object */

    cm_name_type       cm_mc_rpt
        /* Are we waiting for this mc report */
);




/* <EJECT> */
/*===========================================================================

FUNCTION cm_commit_reply_too_long

DESCRIPTION
  Check whether are waiting for too long on a reply

DEPENDENCIES
  Reply object must have already been initialized with
  cm_reply_init().

RETURN VALUE
  TRUE if are waiting for too long on a reply, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
extern boolean cm_commit_reply_too_long(

    cm_reply_s_type    *reply_ptr
        /* Pointer to a reply object */
);
#endif /* FEATURE_OTASP */


/*===========================================================================
FUNCTION CM_KICK_DOG

DESCRIPTION
  Kick the watchdog and set the watchdog timer.

DEPENDENCIES
  None.

RETURN VALUE
  Never exits.

SIDE EFFECTS

===========================================================================*/
void cm_kick_dog(void);



#endif /* CMI_H */



