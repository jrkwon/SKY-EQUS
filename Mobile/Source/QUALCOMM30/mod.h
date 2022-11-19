#ifndef MOD_H
#define MOD_H
/*===========================================================================

                    M O D U L E  S P E C I F I C

                       H E A D E R    F I L E

DESCRIPTION
  This file contains types and declarations associated with the DMSS Module

Copyright (c) 1997,1998 by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header:   L:/src/asw/MSM5000/VCS/mod.h_v   1.3   04 Aug 2000 10:44:22   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/13/99   st      Adapted for the new Sound API
08/18/98   tkuo    ADD FEATURE_MOD_GENERIC_PKT for extended state packet.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
12/08/97   ces     add defines to support clear and setting of status bits.
04/30/97   jjn     Added copies of type declarations from other locations.
                   The trouble is that practically no one packs their data
                   structures.  So, the data structures are of varying size
                   depending on the whim of the compiler.  This is bad
                   when DMSS and DM/SystemUnit have to communicate using the 
                   same data structures.  So, I made duplicates of the data
                   structures and packed them.
02/28/97   jjn     Support remote ringer and new DM packets
02/18/97   jjn     Created From Scratch
===========================================================================*/

#include "rex.h"        /* REX multitasking package             */
#include "target.h"     /* Target specific definitions          */

#if (TG!=T_DM)
#include "customer.h"   /* Customer specific definitions        */
#endif

#include "snd.h"
#include "nv.h"

#if (TG!=T_DM)
#include "clk.h"
#endif

#ifdef FEATURE_DS
#include "dsm.h"        /* Data service ring buffer definition  */
#endif


/*===========================================================================
                     CONSTANT DEFINITIONS
===========================================================================*/

/* Amount of RSSI change that we consider as "significant"*/
/* enough to report to System Unit that RSSI changed      */

#define MOD_SIG_RSSI_DELTA           3          /* 3 db   */


/* Maximum Number of tones that can be reported to the    */
/* System Unit in a single "Module Status Response" packet*/

#define MOD_MAX_NUM_TONES         NV_MAX_DIAL_DIGITS       

/* These are the bits that may be set for the "flag" field*/
/* of the diag_get_sms_message_rsp_type packet            */

#define MOD_SMS_MESSAGES_EXIST    0x1
#define MOD_SMS_NEW_MESSAGE       0x2
#define MOD_SMS_TXT_MESSAGE       0x4
#define MOD_SMS_MORE_MESSAGES     0x8
#define MOD_SMS_READ_FAIL         0x10
#define MOD_SMS_DELETE_FAIL       0x20

/* This is used for events that do not generate any sound */
/* For most events, sound is generated.  So, it was more  */
/* efficient to exchange sound and status information in  */
/* one shot, using "MODULE STATUS" packet.  mod_silence is*/
/* declared in mod.c                                      */

#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
extern  snd_type                  mod_silence;
#endif /* FEATURE_NEW_SOUND_API */

/* Amount of time for which SYS_WAKE should be pulsed */
#define MOD_SYS_WAKE_PULSE        100 // (ms)
#define MOD_POWER_DOWN            250 // (ms)
#define MOD_RING                  400 // (ms)

/*===========================================================================
                     TYPE DEFINITIONS
===========================================================================*/

/*---------------------------------------------------------------------------
 Type definition for all events that could cause the SYS_WAKE pin to be 
 asserted.  Items cannot be added to this enum without taking others out,
 and MOD_NONE should always be -1 (or some other value less than 0).
---------------------------------------------------------------------------*/

#define MOD_BOUNDARY 16

typedef enum
{

  MOD_FIRST            = -1, /* Not a valid value.  Used for bounds checking*/
  MOD_EAR              =  0, /* All signals that should have gone to the    */
                             /* ear-piece but was directed to the ringer    */
                             /* because the phone is in a call or because of*/
                             /* other reasons                               */

  MOD_STOP_SOUND       =  1, /* Tells remote ringer to stop generating a    */
                             /* sound.  For instance, for an incoming call, */
                             /* the remote ringer is told to generate a     */
                             /* repeating sound.  If the call drops before  */
                             /* the user answers the call, have to tell     */
                             /* remote ringer to stop ringing.              */
  MOD_SMS              =  2,
  MOD_CALL_DROP        =  3,
  MOD_STOP_TONE        =  4, /* Same as MOD_STOP_SOUND, except meant for    */
                             /* tones                                       */

  MOD_IS54B_ALERT      =  5, /* Basically an alert from the base station    */
                             /* Indicating that there's and incoming call   */

  MOD_ISDN_ALERT       =  6, /* This counts as an incoming call as well     */
  MOD_RELEASE          =  7, /* Used For both RELEASE order and Stop Alert  */
                             /* Order                                       */
  MOD_REORDER          =  8,
  MOD_INTERCEPT        =  9,
  MOD_PWR_DOWN         = 10,
  MOD_KEY_BEEP         = 11,
  MOD_PWR_UP           = 12,
  MOD_SVC_CHANGE       = 13,
  MOD_CALLER_ID        = 14, /* Reserved Value                              */
  MOD_MISC_ALERT       = 15, /* Miscellaneous Alerts That Do Not Fit Into   */
                             /* Other Categories                            */

  MOD_CALL_ESTABLISHED = 16, /* Indicates that an outgoing call has         */
                             /* been established                            */

  MOD_RSSI_CHANGED     = 17, /* RSSI has changed by a significant           */
                             /* amount                                      */
  
  MOD_LOW_POWER        = 18, /* Low power condition detected                */
  MOD_ROAM_IND         = 19, /* Module is roaming                           */
  MOD_NO_ROAM          = 20,
  MOD_SERV_IND         = 21, /* Module has acquired service                 */ 
  MOD_NO_SERV_IND      = 22, /* Module cannot acquire service               */
  MOD_AUTO_ANSWERED    = 23, /* Incoming call was auto-answered             */
  MOD_MINUTE_ALERT     = 24, /* Beep every minute on 50s mark               */
  MOD_MARKOV           = 25, /* Markov Call Made                            */
  MOD_INT_POWER        = 26, /* Change To Internal Power                    */
  MOD_EXT_POWER        = 27, /* Change To External Power                    */
  MOD_FADE             = 28, /* Fade (going out of service) during          */
                             /* origination or while in IDLE state          */
  MOD_DIGITAL          = 29,
  MOD_ANALOG           = 30,
  MOD_SOUND            = 31, /* Just a place holder event to indicate that  */
                             /* a sound or tone needs to be made            */
#ifdef FEATURE_MOD_GENERIC_PKT

  MOD_NAM_CHANGE       = 32, /* NAM change alert, AutoNAM has to be set on  */
  MOD_VOICE_PRIVACY    = 33, /* Voice privacy is gained or lost alert       */
  MOD_UNLOCK           = 34, /* Receive unlock order message                */

  MOD_LOCK_UNTIL_PWR_CYCLE = 35, /* Receive lock_until_pwr_cycle message    */
  
  MOD_MAINTREQ         = 36, /* Receive Maintanence request message         */
  MOD_BLINK_ROAM_IND   = 37, /* Blink Roaming indication                    */
  MOD_CALLING          = 38, /* After dialing, wait for connection          */
  MOD_END_KEY_PRESS    = 39, /* User ends up a call                         */
  MOD_LAST             = 40  /* Not a valid value. Used for bounds checking */

#else /* FEATURE_MOD_GENERIC_PKT */
  MOD_LAST             = 32  /* Not a valid value. Used for bounds checking */
  
#endif /* FEATURE_MOD_GENERIC_PKT */

} mod_sys_wake_event_type;

/*  Define some handy BIT Mask for setting and clearing bits in Status Mask */
/*  used to notify the system unit of status changes.                       */

#define MOD_EAR_BIT              (0x01 << (word)MOD_EAR)
#define MOD_STOP_SOUND_BIT       (0x01 << (word)MOD_STOP_SOUND)
#define MOD_SMS_BIT              (0x01 << (word)MOD_SMS)
#define MOD_CALL_DROP_BIT        (0x01 << (word)MOD_CALL_DROP)
#define MOD_STOP_TONE_BIT        (0x01 << (word)MOD_STOP_TONE)
#define MOD_IS54B_ALERT_BIT      (0x01 << (word)MOD_IS54B_ALERT)
#define MOD_ISDN_ALERT_BIT       (0x01 << (word)MOD_ISDN_ALERT)
#define MOD_RELEASE_BIT          (0x01 << (word)MOD_RELEASE)
#define MOD_REORDER_BIT          (0x01 << (word)MOD_REORDER)
#define MOD_INTERCEPT_BIT        (0x01 << (word)MOD_INTERCEPT)
#define MOD_PWR_DOWN_BIT         (0x01 << (word)MOD_PWR_DOWN)
#define MOD_KEY_BEEP_BIT         (0x01 << (word)MOD_KEY_BEEP)
#define MOD_PWR_UP_BIT           (0x01 << (word)MOD_PWR_UP)
#define MOD_SVC_CHANGE_BIT       (0x01 << (word)MOD_SVC_CHANGE)
#define MOD_CALLER_ID_BIT        (0x01 << (word)MOD_CALLER_ID)
#define MOD_MISC_ALERT_BIT       (0x01 << (word)MOD_MISC_ALERT)
#define MOD_CALL_ESTABLISHED_BIT (0x01 << (word)(MOD_CALL_ESTABLISHED - MOD_BOUNDARY))
#define MOD_RSSI_CHANGED_BIT     (0x01 << (word)(MOD_RSSI_CHANGED - MOD_BOUNDARY) 
#define MOD_LOW_POWER_BIT        (0x01 << (word)(MOD_LOW_POWER - MOD_BOUNDARY))
#define MOD_ROAM_IND_BIT         (0x01 << (word)(MOD_ROAM_IND - MOD_BOUNDARY))
#define MOD_NO_ROAM_BIT          (0x01 << (word)(MOD_NO_ROAM - MOD_BOUNDARY))
#define MOD_SERV_IND_BIT         (0x01 << (word)(MOD_SERV_IND - MOD_BOUNDARY))
#define MOD_NO_SERV_IND_BIT      (0x01 << (word)(MOD_NO_SERV_IND - MOD_BOUNDARY))
#define MOD_AUTO_ANSWERED_BIT    (0x01 << (word)(MOD_AUTO_ANSWERED - MOD_BOUNDARY))
#define MOD_MINUTE_ALERT_BIT     (0x01 << (word)(MOD_MINUTE_ALERT - MOD_BOUNDARY))
#define MOD_MARKOV_BIT           (0x01 << (word)(MOD_MARKOV - MOD_BOUNDARY))
#define MOD_INT_POWER_BIT        (0x01 << (word)(MOD_INT_POWER - MOD_BOUNDARY))
#define MOD_EXT_POWER_BIT        (0x01 << (word)(MOD_EXT_POWER - MOD_BOUNDARY))
#define MOD_FADE_BIT             (0x01 << (word)(MOD_FADE - MOD_BOUNDARY))
#define MOD_DIGITAL_BIT          (0x01 << (word)(MOD_DIGITAL - MOD_BOUNDARY))
#define MOD_ANALOG_BIT           (0x01 << (word)(MOD_ANALOG - MOD_BOUNDARY)) 
#define MOD_SOUND_BIT            (0x01 << (word)(MOD_SOUND - MOD_BOUNDARY))

/*---------------------------------------------------------------------------
  All different erase codes for the diag_get_sms_message_req_type packet
---------------------------------------------------------------------------*/

typedef enum {
  MOD_SMS_ERASE_MIN = -1,          /* Meant to be less than the    */
                                   /* smallest defined value       */

  MOD_SMS_DONT_ERASE = 0,          /* Don't Erase                  */

  MOD_SMS_ERASE_ALL_TEXT    = 1,   /* Erase All Text SMS messages, */
                                   /* which have already been read */

  MOD_SMS_ERASE_NEWEST_TEXT = 2,   /* Erase the newest (last read) */
                                   /* text SMS message             */
                           
  MOD_SMS_ERASE_OLDEST_TEXT = 3,   /* Erase the oldest text SMS    */
                                   /* message                      */

  MOD_SMS_ERASE_ALL_VOICE    = 4,  /* Erase all voice SMS messages */
                                   /* Most likely there will be    */
                                   /* just one to erase because    */
                                   /* voice SMS messages are meant */
                                   /* to simply indicate that      */
                                   /* the subscriber has voice     */
                                   /* mail.  The voice mail is not */
                                   /* stored in the phone          */
                                   
  MOD_SMS_ERASE_NEWEST_VOICE = 5,  /* Erase the newest (last read) */
                                   /* voice SMS message            */
                                 
  MOD_SMS_ERASE_OLDEST_VOICE = 6,  /* Erase the oldest voice SMS   */
                                   /* message                      */

  MOD_SMS_ERASE_ALL = 7,           /* Erase all SMS messages (both */
                                   /* voice and text), except for  */
                                   /* text message that have not   */
                                   /* yet been read                */

  MOD_SMS_ERASE_MAX = 8            /* Meant to be larger than the  */
                                   /* largest defined value        */

} mod_sms_erase_code_type;

/*---------------------------------------------------------------------------
  Different types of serial links for Module (no longer used)
---------------------------------------------------------------------------*/

typedef enum
{
  MOD_SERIAL_STATE_FIRST,          /* This value has to remain the        */
                                   /* very first serial connection        */
                                   /* It is here for bounds checking      */

  MOD_SERIAL_STATE_SYSTEM_UNIT,    /* Talking to the System Unit          */

  MOD_SERIAL_STATE_EXTERNAL,       /* Talking to the External Connector   */
  
  MOD_SERIAL_STATE_LAST            /* This value has to remain the        */
                                   /* very last serial connection.        */
                                   /* It is here for bounds checking      */
} mod_serial_state_type;

/*---------------------------------------------------------------------------
  Different return parameters for procedure calls, queue commands, etc
---------------------------------------------------------------------------*/

typedef enum
{
  MOD_STATUS_MIN       = -1,       /* First entry for testing purposes    */

  MOD_STATUS_BAD_PARAM =  0,       /* Bad Parameters caused improper      */
                                   /* operation of command                */

  MOD_STATUS_GOOD      =  1,       /* Command accepted and processed      */
                                   /* correctly                           */

  MOD_STATUS_ERROR     =  2,       /* Command and associated parameters   */
                                   /* were (as far as could be            */
                                   /* determined when the error occurred) */
                                   /* o.k.  The problem occurred in the   */
                                   /* execution of the command.  One      */
                                   /* reason could be that a command queue*/
                                   /* was empty.                          */

  MOD_STATUS_MAX                   /* Last Entry for testing purposes     */

} mod_status_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                  Audio Control Request                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* The following structures are modified versions of structures found in
 * snd.h.  These modified structures eliminate fields that are not 
 * necessary for the system unit to send.
 */

/* Module Sound Commands
*/
typedef enum {          /* (parameter) Brief description                */

  MOD_SND_SET_PATH = 0,     /* (path) Set the audio path, and muting        */
  MOD_SND_VOLUME,           /* (volume) Set the voice volume for all paths  */
  MOD_SND_RING_VOLUME,      /* (volume) Set the ringer volume               */
  MOD_SND_DTMF_START,       /* Play a DTMF tone for awhile                  */
  MOD_SND_DTMF_STOP,        /* Stop playing the DTMF tone                   */
  MOD_SND_TONE_START,       /* (tone) Play a single tone for awhile         */
  MOD_SND_TONE_STOP,        /* (no parameter) Stop playing the current tone */
  MOD_SND_SOUND_START,      /* (sound) Play a predefined sound              */
  MOD_SND_SOUND_STOP,       /* (no parameter) Stop the current sound        */
  MOD_SND_SET_KEY_SCLASS,   /* dynamically set key sound class for module       */

  MOD_SND_INVALID           /* (Leave Last) Invalid Command - Causes Error  */

} mod_snd_op_type;

/* Audio path setting packet
*/
typedef struct {
#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
  snd_apath_type         apath;         /* Audio path                  */
#endif /* FEATURE_NEW_SOUND_API */
  snd_mute_control_type  out_mute_s;    /* Is output path muted?       */
  snd_mute_control_type  in_mute_s;     /* Is input path muted?        */
} mod_snd_path_packet_type;


/* Volume level control packet
*/
typedef struct {
  word phone;                           /* Volume level for handset    */
  word ext;                             /* Volume for speaker          */
  word priv;                            /* Reserved for Qualcomm use   */
} mod_snd_vol_type;

#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
/* Single tone packet
*/
typedef struct {
  snd_type                tone;         /* Single tone                 */
  snd_apath_type          apath;        /* Audio path                  */
  snd_class_type          sclass;       /* Which generator to use      */
  mod_snd_vol_type        vol;          /* Tone Volume for all paths   */
  snd_dtmf_apath_type     dtmf_path;    /* DTMF tx/local/mute pathing  */
} mod_snd_tone_packet_type;

/* Pre-defined sound packet
*/
typedef struct {
  snd_predef_type         sound;        /* Predefined sound            */
  snd_apath_type          apath;        /* Audio path                  */
  snd_class_type          sclass;       /* Which generator to use      */
  mod_snd_vol_type        vol;          /* Tone Volume for all paths   */
  snd_dtmf_apath_type     dtmf_path;    /* DTMF tx/local/mute pathing  */
} mod_snd_predef_packet_type;
#endif /* FEATURE_NEW_SOUND_API */

/*=======================================================================*/

#ifdef FEATURE_MOD_GENERIC_PKT
#error code not present
#endif /*FEATURE_MOD_GENERIC_PKT*/

/*===========================================================================
                       E X P O R T E D   V A R I A B L E S
===========================================================================*/
      
#if (TG!=T_DM)

extern clk_cb_type           mod_power_down_cb_item;
extern clk_cb_type           mod_ring_cb_item;
extern boolean               mod_in_sys_wake_isr;
#endif



/*===========================================================================
                  F U N C T I O N   A N D   P R O C E D U R E

                             P R O T O T Y P E S
===========================================================================*/

/*===========================================================================

FUNCTION MOD_POWER_DOWN_ISR

DESCRIPTION
  This function will create a pulse by setting GPIO25 low

DEPENDENCIES
  Will only create a pulse if the signal was low to begin with and set high.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void mod_power_down_isr
(
  int4 dummy   /* dummy argument */
);

/*===========================================================================

FUNCTION MOD_CLEAR_STATUS

DESCRIPTION
  Utility that allows other processes to clear the status information
     
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Will annul record of any event that has happened since last call to
  this procedure

===========================================================================*/


extern void mod_clear_status( void );

/*===========================================================================

FUNCTION MOD_RECORD_EVENT

DESCRIPTION
  Tells the system unit that an event has occurred by asserting the SYS_WAKE
  pin.  Also, records information so that the subsequent packet exchange will
  convey the necessary information to the system unit
     
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern void mod_record_event( 
#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
                     snd_class_type          sound_class,
                     mod_sys_wake_event_type event, 
                     boolean                 tone, 
                     snd_predef_type         sound,
                     word                    num_tones,
                     snd_type                *snds
#endif /* FEATURE_NEW_SOUND_API */
                     );


/*===========================================================================

FUNCTION MOD_RETURN_STATUS

DESCRIPTION
  Returns the module status to the calling task.  Most likely the calling
  task is DIAG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern word mod_return_status( void );

/*===========================================================================

FUNCTION MOD_RETURN_STATUS2

DESCRIPTION
  Returns the module status to the calling task.  Most likely the calling
  task is DIAG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern word mod_return_status2( void );


/*===========================================================================

FUNCTION MOD_RETURN_SND_CLASS

DESCRIPTION
  Returns the mod_snd_class to the calling task.  Most likely the 
  calling task is DIAG

DEPENDENCIES
  None

RETURN VALUE
  Indicates the sound class.  Used to support System Units that have
  multiple ways to make a sound

SIDE EFFECTS
  None

===========================================================================*/

#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
snd_class_type mod_return_snd_class( void );
#endif /* FEATURE_NEW_SOUND_API */

/*===========================================================================

FUNCTION MOD_RETURN_SOUND

DESCRIPTION
  Returns the module sound to the calling task.  Most likely the calling
  task is DIAG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern word mod_return_sound( void );

/*===========================================================================

FUNCTION MOD_RETURN_TONE

DESCRIPTION
  Returns the boolean mod_tone to the calling task.  Most likely the calling
  task is DIAG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

extern boolean mod_return_tone( void );

/*===========================================================================

FUNCTION MOD_FILL_IN_TONES

DESCRIPTION
  Records the number of tones that needs to be played and fill in the array
  of tones that should be played at the remote ringer.  Will actually fill
  in the array.  Otherwise, there are issues of reading the array while it
  is being written to, etc.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef FEATURE_NEW_SOUND_API
#error code not present
#else
extern void mod_fill_in_tones( word *num_tones, snd_type *snds );
#endif /* FEATURE_NEW_SOUND_API */

/*===========================================================================

FUNCTION MOD_ENABLE_MOD_WAKE_EVENT

DESCRIPTION
  This procedure will allow Module software to interrupt on the rising
  edge of the MOD_WAKE pin

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void mod_enable_mod_wake_event( void );


/*===========================================================================

FUNCTION MOD_SYSTEM_UNIT

DESCRIPTION
  This system simply allows the state of the Module's serial connection
  to be determined.  Right now, the connection can be either with the
  System Unit or with an external connector.  So, the return value is 
  boolean.  If this ever changes, we may need to return an ENUM or we
  may need other functions

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mod_system_unit( void );


/*===========================================================================

FUNCTION MOD_INIT

DESCRIPTION
  This routine does Module specific initialization
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mod_init( void );


/*===========================================================================

FUNCTION MOD_POWERDOWN

DESCRIPTION
  This routine does Module specific power down processing
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mod_powerdown( void );


/*===========================================================================

FUNCTION MOD_RECORD_MASKS

DESCRIPTION
  Records the masks of events that the System Unit desires to be reported to
  it.
     
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean mod_record_masks( 
                        word mask1,
                        word mask2,
                        word mask3
                        );


/*===========================================================================

FUNCTION MOD_SET_PULSE_RI

DESCRIPTION
  This procedures determines whether or not RI/ is pulsed every time SYS_WAKE
  is pulsed.  This feature comes in handy because DM (which we used to test
  the Module) can interrupt on RI/ whereas interrupting on SYS_WAKE would take
  much more work.  Also, this feature cannot be permanent because RI/ is a
  signal that actually means something in RS232.  
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mod_set_pulse_ri( 
                     byte pulse_ri
                     );



/*===========================================================================

FUNCTION MOD_RING_ISR

DESCRIPTION
  This ISR will set UI_RING_SIG so that UI knows that there was a ring

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void mod_ring_isr(
                 int4 dummy   /* dummy argument */
                 );

#ifdef FEATURE_MOD_GENERIC_PKT
#error code not present
#endif /*FEATURE_MOD_GENERIC_PKT*/

/*===========================================================================

FUNCTION MOD_EXT_RSSI_SEN 

DESCRIPTION
  Sensitivity check of RSSI for CDMA or Analog.

DEPENDENCIES
  None

RETURN VALUE
  For CDMA, we return current RSSI or last RSSI in DBM. But, for Analog, we 
  return current RSSI or last RSSI in register value of RSSI.  
  The reason we don't return current RSSI all the times is becuase RSSI 
  changes slowly sometimes.
  
SIDE EFFECTS
  None

===========================================================================*/
word mod_ext_RSSI_sen(word currentrssi, word lastrssi, 
                      mod_sys_wake_event_type item
                      );


/*===========================================================================

FUNCTION MOD_ANALOG_RSSI_TO_DBM

DESCRIPTION
   Register's RSSI is different from real RSSI in DBM, we use this formula to
   convert it to real RSSI in DBM.

DEPENDENCIES
  None

RETURN VALUE
  Absolute of RSSI in DBM

SIDE EFFECTS
  None

===========================================================================*/
word mod_analog_rssi_to_dbm(word currentrssi);



/*===========================================================================
  Here start the duplicates of data types declared in other modules.  Names
  were changed to protect the innocent and to prevent compile time problems.
===========================================================================*/

/* 1) The following are the declarations for SMS messages                  */

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

//#pragma pack(1) /* Byte align structures */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*     SMS INTERNAL PARAMETER MASK DEFINITIONS                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*                INTERNAL SMS PARAMETER FLAGS                       */
/* In the internal SMS message format, a 3 byte mask is provided to  */
/* represent which parameters are specified in the SMS message.      */
#define MOD_NO_PARAMS       0x000000 /* Indicates no parameters specified */
                                      /* in SMS message                    */

#define MOD_BEARER_REPLY    0x000001 /* Indicates internal SMS message    */
                                      /* contains a bearer reply parameter */

#define MOD_VALID_ABS       0x000002 /* Indicates internal SMS message    */
                                      /* contains a valid period -         */
                                      /* absolute parameter                */

#define MOD_VALID_REL       0x000004 /* Indicates internal SMS message    */
                                      /* contains a valid period -         */
                                      /* relative parameter                */

#define MOD_DEFER_ABS       0x000008 /* Indicates internal SMS message    */
                                      /* contains a deferred delivery time */
                                      /* - absolute parameter              */

#define MOD_DEFER_REL       0x000010 /* Indicates internal SMS message    */
                                      /* contains a deferred delivery time */
                                      /* - relative parameter              */

#define MOD_USER_RESP       0x000020 /* Indicates internal SMS message    */
                                      /* contains a user response code     */
                                      /* parameter                         */

#define MOD_CAUSE_CODES     0x000040 /* Indicates internal SMS message    */
                                      /* contains a cause codes parameter  */

#define MOD_ORIG_ADDR       0x000080 /* Indicates internal SMS message    */
                                      /* contains originating address      */
                                      /* parameter                         */

#define MOD_ORIG_SUBADDR    0x000100 /* Indicates internal SMS message    */
                                      /* contains originating subaddress   */
                                      /* parameter                         */

#define MOD_DEST_ADDR       0x000200 /* Indicates internal SMS message    */
                                      /* contains destination address      */
                                      /* parameter                         */

#define MOD_DEST_SUBADDR    0x000400 /* Indicates internal SMS message    */
                                      /* contains destination subaddress   */
                                      /* parameter                         */

#define MOD_USER_DATA       0x000800 /* Indicates internal SMS message    */
                                      /* contains a user data parameter    */

#define MOD_PRIORITY        0x001000 /* Indicates internal SMS message    */
                                      /* contains a priority indicator     */
                                      /* parameter                         */

#define MOD_PRIVACY         0x002000 /* Indicates internal SMS message    */
                                      /* contains a privacy indicator      */
                                      /* parameter                         */

#define MOD_REPLY_OPTION    0x004000 /* Indicates internal SMS message    */
                                      /* contains a reply option           */
                                      /* parameter                         */

#define MOD_NUM_OF_MSGS     0x008000 /* Indicates internal SMS message    */
                                      /* contains a number of messages     */
                                      /* parameter                         */

#define MOD_ALERT           0x010000 /* Indicates alert on SMS message    */
                                      /* delivery (no parameter reqd)      */

#define MOD_LANGUAGE        0x020000 /* Indicates internal SMS message    */
                                      /* contains a language indicator     */
                                      /* parameter                         */

#define MOD_CALL_BACK       0x040000 /* Indicates internal SMS message    */
                                      /* contains a call back number       */
                                      /* parameter                         */

#define MOD_MC_TIME_STAMP   0x080000 /* Indicates internal SMS message    */
                                      /* contains a Message Center time    */
                                      /* stamp parameter                   */

#define MOD_BEARER_DATA     0x100000 /* Indicates internal SMS message    */
                                      /* contains a bearer data parameter  */


/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*     TYPES FOR SMS INTERNAL PARAMETER VALUES                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-637 Section 3.4.2 SMS Transport Layer Message Categories */
typedef enum
{
  MOD_SMS_PTP = 0,     /* IS-637 SMS Point-to-Point Message */
  MOD_SMS_BC,          /* IS-637 SMS Broadcast Message      */
  MOD_SMS_ACK          /* IS-637 SMS Acknowledge Message    */
} mod_msg_cat_type;

/* See IS-637 Section 4.4.1 SMS Teleservice types                    */
/* Used internally between MC and UI tasks for SMS message parsing.  */
typedef enum
{
  MOD_CPT_95,             /* IS-95A Cellular Paging                  */
  MOD_CMT_95,             /* IS-95A Cellular Messaging               */
  MOD_VMN_95,             /* IS-95A Voice Mail Notification          */
  MOD_CMT_91_CLI,         /* IS-91 Extended Protocol - CLI Order     */
  MOD_CMT_91_VOICE_MAIL,  /* IS-91 Extended Protocol - Voice Mail    */
  MOD_CMT_91_SHORT_MSG,   /* IS-91 Extended Protocol - Short Message */
  MOD_AWI_95              /* IS-95A Alert With Info                  */
} mod_teleservice_type;

/* <EJECT> */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*               INTERNAL TYPES USED FOR SMS MESSAGES                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Internal message formats are used to represented SMS message data on    */
/* byte-aligned boundaries.  Bit-packing and bit-unpacking routines        */
/* utilize these definitions to translate SMS message data to and from     */
/* bit-packed formats.                                                     */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* INTERNAL SMS PARAMETERS AND MESSAGES                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define MOD_MSG_MAX 255
#define MOD_USER_DATA_MAX 245
#define MOD_CALL_BACK_MAX 32
#define MOD_TL_ADDRESS_MAX 50

/* Alert With Info SMS info_data field */
typedef byte mod_awi_data_type[MOD_MSG_MAX];

/* Parameter Header */
typedef struct
{
  byte parm_type;
  byte parm_len;
} mod_param_hdr_type;

/* Message Identifier */
typedef struct
{
  byte message_type;
  word message_id;
} mod_msg_id_type;

/* User Data */
typedef struct
{
  byte msg_encoding;
  byte mst;
  byte num_fields;
  byte chari[MOD_USER_DATA_MAX];
} mod_user_data_type;

/* User Response Code */
typedef byte mod_user_resp_type;

/* Message Center Time Stamp         */
/* Validity Period - Absolute        */
/* Deferred Delivery Time - Absolute */
typedef struct
{
  byte yr;
  byte mon;
  byte day;
  byte hrs;
  byte min;
  byte sec;
} mod_abs_time_type;

/* Validity Period - Relative        */
/* Deferred Delivery Time - Relative */
typedef byte mod_rel_time_type;

/* Priority Indicator */
/* Privacy Indicator  */
typedef byte mod_indicator_type;

/* Reply Option */
typedef struct
{
  byte user_ack_req;
  byte dak_req;
} mod_reply_opt_type;

#define NUM_OF_MSGS_LENGTH 2

/* Number Of Messages  */
typedef byte mod_num_of_msgs_type[NUM_OF_MSGS_LENGTH];

/* Language Indicator */
typedef byte mod_language_type;

/* Call Back Number */
typedef struct
{
  byte digit_mode;
  byte num_type;
  byte num_plan;
  byte num_fields;
  byte chari[MOD_CALL_BACK_MAX];
} mod_call_back_type;


/* Address Parameters */
typedef struct
{
  byte digit_mode;
  byte num_mode;
  byte num_type;
  byte num_plan;
  byte num_fields;
  byte chari[MOD_TL_ADDRESS_MAX];
} mod_address_type;

/* Subaddress */
typedef struct
{
  byte type;
  byte odd;
  byte num_fields;
  byte chari[MOD_TL_ADDRESS_MAX];
} mod_subaddress_type;

/* Bearer Reply Option */
typedef byte mod_bearer_reply_type;

/* Cause Codes */
typedef struct
{
  byte reply_seq;
  byte error_class;
  byte cause_code;
} mod_cause_codes_type;

/* Used to hold bit-packed SMS message for transmit */
typedef struct
{
  byte len;
  byte chari[MOD_USER_DATA_MAX];
} mod_ext_msg_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                  INTERNAL SMS MESSAGE FORMATS                           */

/* Internal SMS Message Data Format */
typedef struct
{
  mod_address_type      address;      /* Orig/Dest Address            */
  mod_subaddress_type   subaddress;   /* Orig/Dest Subaddress         */
  mod_user_data_type    user_data;    /* User Data                    */
  mod_indicator_type    priority;     /* Priority Indicator           */
  mod_indicator_type    privacy;      /* Privacy Indicator            */
  mod_reply_opt_type    reply_opt;    /* Reply Option                 */
  mod_num_of_msgs_type  num_of_msgs;  /* Number Of Messages           */
  mod_language_type     lang;         /* Language Indicator           */
  mod_call_back_type    call_back;    /* Call Back Number             */
  mod_abs_time_type     mc_time;      /* Message Center Time Stamp    */
} mod_msg_data_type;


/* Transport Layer SMS Message */
typedef struct
{
  mod_teleservice_type  tele_srv;       /* Internal Teleservice Type    */
  mod_msg_cat_type      sms_msg_cat;    /* SMS Message Category         */
  mod_msg_id_type       sms_msg_id;     /* SMS Message Identifier       */
  dword                 sms_param_m;    /* SMS Parameter Mask           */
  mod_bearer_reply_type bearer_reply;   /* Bearer Reply                 */
  mod_abs_time_type     valid_abs;      /* Validity Period - Absolute   */
  mod_rel_time_type     valid_rel;      /* Validity Period - Relative   */
  mod_abs_time_type     defer_abs;      /* Deferred Delivery Time - abs */
  mod_rel_time_type     defer_rel;      /* Deferred Delivery Time - rel */
  mod_user_resp_type    user_resp;      /* User Response Code           */
  mod_cause_codes_type  cause_codes;    /* Cause Codes                  */
  mod_msg_data_type     msg_data;       /* Message Data                 */
} mod_msg_type;

/* 1) The following are the declarations for Caller ID information      */

#define     MOD_MAX_DIALED_DIGITS  32        /* Maximum 32 digit number */

typedef struct {                  /* editor buffer type   */
  boolean act;                    /* active or not        */
  boolean secret;                 /* secret number or not */
  byte    buf[MOD_MAX_DIALED_DIGITS]; /* the data */
  byte    pos;                    /* length of the data */
} mod_edit_type;

typedef struct {                  /* Calling Number ID type */
  mod_edit_type n;                /* Number storage */
  byte    name[NV_MAX_LTRS];      /* Name associated with number */
  byte    name_length;            /* length of the name */
} mod_cni_type;


//#pragma pack()

#endif /* MOD_H */

