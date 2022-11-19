#include <customer.h>
#ifdef MMI_TREX
	#include <uinstate.h>
#else //#ifdef MMI_TREX
#ifndef UISTATE_H
#define UISTATE_H
/*===========================================================================

          U S E R   I N T E R F A C E   S T A T E   I N T E R N A L


DESCRIPTION
  The state machine shared variables.  Passing them all over the place would
  slow down things too much

      Copyright (c) 1991 through 2000 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

$Header:   L:/src/asw/MSM5000/VCS/uistate.h_v   1.3   18 May 2000 10:25:04   gshi  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/11/00   CS      Changed FEATURE_USB to FEATURE_USB_TESTING.
04/18/00   CS      Added support for USB device selection.
04/11/00   CS      Added support for FEATURE_TTY.
04/06/00   CS      Added a global data curr_menu_sel to save the current menu selection.
02/24/00   CS      Added support for FEATURE_BROADCAST_SMS.
10/28/99   HD      Added support for charging.
10/27/99   HD      Added analog_hfk to the ui state variable.
10/26/99   VT      Added force_so to the ui state variable table.
10/26/99   CS      Added FEATURE_UI_8K_SUPPORT for test calls.
10/08/99   CS      Added FEATURE_AUDIO_AGC support.
07/22/98   KAR     Added EVRC support
07/22/98   KCK     Added support for SMS UTC time stamp.
07/21/98   KAR     Added support for roam ringer menu.
                   Changes for new HS-UI interface
07/01/98   YM      Added support for FEATURE_DS_VOICE_AS_DATA
05/22/98   YM      Added support for Low Power Mode
05/21/98   AYK     Added keyguard support for 5GP under FEATURE_UI_KEYGUARD.
04/16/98   SIU     Added ui.cli_timeout to solve CLI not timing out problem.
03/18/98   YM      Added prototype for ui_get_so_txt_ptr for data call ID
03/04/98   KAR     Added more ui variables to eliminate certain global var.
02/14/98   AYK     Added DTACO support under FEATURE_UI_DTACO.
02/05/98   KAR     Added headset/handyphone support.
01/13/98   SIU     Removed incorrect pragma statement for Caller ID enh.
12/23/97   SIU     Caller ID/Call history enhancement.
12/22/97   skk     Added SMS Volume menu.
12/16/97   skk     Added SMS option 6 functionality.
08/19/97   dna     Renamed IMSI_MNC_0 to IMSI_11_12_0
08/18/97   dna     Cleaned up IMSI defaults and allowed for Q800
08/13/97   dna     Merged Bilhan's NV Init changes
07/31/97   ldg     Added data services debug UI under FEATURE_DS_DEBUG_UI.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/17/97   SIU     Removed pragma statement with target name.
06/06/97   SIU     Added mapping from UI ringer type to SND enums
05/21/97   BK      added functions cni_format, format_display, and modified
                   num_format to call format_display
05/12/97   RC      Added "allow_mode_sel" to ui_state_var_type -- Support
                   for 800 MHz targets.
05/07/97   BK      Fixed double beeps, no beeps and inconsistencies during
                   volume adjustment.
04/22/97   BK      Added Restricted and Unavailable numbers to call stack.
04/19/97   RC      Added "pwrkey_pressed_long" to ui_state_var_type --
                   TRUE when power key is pressed long enough at powerup.
04/10/97   RC      Added "boolean ok_to_sleep" to ui_state_var_type --
                   TRUE when UI OKed sleeping.
04/10/97   RC      Added "boolean backlight_on" to ui_state_var_type --
                   TRUE when backlight is on.
03/25/97   BK      Renamed to FEATURE_UI_CALL_ANSWER_MODE
03/25/97   BK      Implemented FEATURE_CALL_ANSWER_MODE
03/14/97   RC      Implemented multiple ringer types. Compiled in when
                   FEATURE_MULTIPLE_RINGER_TYPES is defined.
03/12/97   jjn     Added (TG==T_MD) in conditional compile statements
                   wherever appropriate for the Module
02/18/97   RC      Implemented escalating ringing -- added escalating_ring
                   and escalating_ring_spkr to ui_state_var_type.
02/11/97   BK      Corrected uistate_list return type.
                   Temp mod to clear system time display after deep sleep.
01/29/97   BK      Added FEATURE_VIBRATOR initial version.
01/27/97   RC      Removed data_alert from ui_state_var_type.
01/20/97   BK      Double beep bug in volume settings is eliminated.
01/09/97   BK      New UI to SOUND interface modifications.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
12/11/96   RC      Added "ui_edit_type pse" to ui_state_var_type.
11/11/96   RC      Added "boolean conv" to ui_state_var_type. This variable
                   is TRUE only when we are in a phone conversation.
09/26/96   BK      Add definitions for IMSI components as per IS-683 sect 3.1.
09/22/96   BK      For JSTD008 MCC default is 000 and not 310.
09/13/96   BK      Added FEATURE_OTASP.
08/14/96   JGR     Added menu_curkey and menu_prevkey for menu simplification.
08/07/96   cah     Renamed FEATURE_PRIVACY_ALERT to FEATURE_UI_PRIVACY_ALERT.
08/05/96   CAH     Handle alerting at voice privacy state change.
08/02/96   BK      Implemented table driven menus.
07/22/96   BK      Added ifdefs for FEATURE_UI_SAVE_REDIAL_BUFFER
07/05/96   BK      Re-dial maintained after power-cycle.
05/22/96   CAH     Don't store CNI numbers with invalid characters.
04/18/96   BK      New UI branching created from uistate.h ver 1.75
03/14/96   BK      Added ifdefs for customer specific builds.
03/05/96   BK      Replace T_PRMCO1900 with T_GP.
02/29/96   JGR     New screen handling technique
02/20/96   CAH     CNI now supported for all cases w/ or w/o Alerting, and
                     all PI settings.
02/13/96   BK      T_PRMCO1900 introduction
01/17/96   BK      -Ringer Off- text implementation.
12/27/95   BK      New Service Programming is implemented.
12/21/95   BNS     Removed ui.confirm_tone
12/05/95   BK      Lint modifications.
11/30/95   BNS     Added support for antenna aiming mode in ISS2
11/27/95   CAH     Rid unused name sorting code and data.
11/20/95   CAH     Fixed bug in which redial wouldn't work if the last
                     outgoing number is bumped off the calls list by keeping
                     redial separate.
11/09/95   DAK     Added redial buffer to global ui in ISS2/CSS
11/07/95   DAK     Fixed #ifdefs so that code compiles for ISS2 with
                   T_INDIA_WLL flag off.
11/03/95   DAK     Added offhook_offline boolean to ui structure.
10/30/95   RAS     Updates to have css build from \ref
10/17/95   DAK     #ifdef'd out unused code for ISS2.
10/16/95   CAH     Fixed time display to use local time and daylt savings.
10/11/95   DAK     Added #include "target.h" and conditional flags to use
                   uitxtw.h for ISS2
10/11/95   DAK     #ifdef'd a field in ui so it doesn't compile for ISS2
10/06/95   CAH     Fixed Missed calls display to support UI_TOIDLE_F and
                     displaying when there's only one ring and you're not
                     idling.
09/24/95   CAH     Timestamp now displayed in calls list and code optimizing.
09/23/95   CAH     Display NAM info while in a call and INFO is pressed.
09/22/95   BNS     Added flags for confirmation tone and iss2 rssi.
09/15/95   BK      Restricted warning display fixed .
09/14/95   BK      Call timers and a global restricted warning function added.
09/14/95   DAK     Added hotline,alarms_init_done and dialing_plan fields to
                   uistate_var_type.
09/11/95   BNS     Added flags for ISS2 ringback and alarm support.
09/04/95   CAH     Force call now times out.
08/31/95   CAH     New debug menus Field Service code entry support.
08/23/95   CAH     Add default MCC and MNC values.
08/17/95   CAH     New incoming call handling including support for new
                     Markovs and SO9.
08/12/95   BK      Displays time and day when idle if set and avail.
08/10/95   CAH     New Markov and Loopback flag.
08/01/95   CAH     Rid ORIG_MIN
07/10/95   BK      Locked-state emergency-number-calling implemented.
07/04/95   BK      Code for emergency numbers, and prep for counters
06/27/95   CAH     Gemini low battery support.
06/20/95   CAH     Gemini preliminary 13K Markov support.
06/19/95   CAH     Gemini Send PIN prompt support.
06/06/95   CAH     Gemini preliminary restrict support, NV updates and dials.
05/16/95   CAH     Gemini Calls list support.
05/01/95   CAH     Initial Gemini mods.
04/15/95   RAS     Initial ISS2 target support.
12/22/94   CAH     Data calls now connect
11/21/94   CAH     Data calls and Loopback calls now separate
11/18/94   CAH     Data call status display
11/10/94   CAH     ISS1 send timeout via nvram item support
08/09/94   CAH     Lint modifications
07/22/94   CAH     ISS1 modifications
05/01/92   RRD     Created

===========================================================================*/


/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#include "target.h"
#include "customer.h"
#include "comdef.h"

#include "uiutxt.h"

#include "uiuint.h"
#include "ui.h"
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
//#ifdef FEATURE_EQUS_A10B1 // girin. 2000.7.26
#include "uixsnd.h"
#else
#include "uinsnd.h"

#endif
#include "nv.h"
#include "db.h"
#include "cm.h"
#include "uiscall.h"

#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

#ifdef FEATURE_USB_TESTING
#error code not present
#endif /* FEATURE_USB_TESTING */

/* hjcho 2000-09-13 --> @why, what */
#if  (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) 
//#include "eqa.h"
#endif /*(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)*/
/* <-- */


#define DIRTY_BIT_MASK 0x80   /* Mask to isolate byte written flag */

/* Default IMSI values */
#define IMSI_MCC_0 999       /* 000 */
#define IMSI_S1_0  16378855   /* 0000000 */
#define IMSI_S2_0  999       /* 000 */
#define IMSI_CLASS0_ADDR_NUM 0xFF
#define IMSI_11_12_0 99

#ifdef FEATURE_CDMA_800
// modified by chos 00.03.03	default channelÀ» 400À¸·Î
// remodifed by tasik 00.04.24  default channel to 779 for A and to 738 for B

#ifdef	MODEL_SKTT

/* Primary Paging Channel Defaults */
#define PCH_A_DEF 779           /* Primary Channel A Default */
#define PCH_B_DEF 779           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEF 738           /* Secondary Channel A Default */
#define SCH_B_DEF 738           /* Secondary Channel B Default */

#else	// #ifdef	MODEL_SKTT

/* Primary Paging Channel Defaults */
#define PCH_A_DEF 283           /* Primary Channel A Default */
#define PCH_B_DEF 384           /* Primary Channel B Default */

/* Secondary Paging Channel Defaults */
#define SCH_A_DEF 691           /* Secondary Channel A Default */
#define SCH_B_DEF 777           /* Secondary Channel B Default */

#endif	// #ifdef	MODEL_SKTT

#endif // FEATURE_CDMA_800

#define NID_DEFAULTS 65535

typedef enum {
  UI_NO_MARKOV,                     /* Not performing Markov */
#ifdef FEATURE_UI_8K_SUPPORT
  UI_OLD_8K_MARKOV,                 /* Old version of 8K Markov */
  UI_MARKOV_8K,                     /* New version of 8K Markov */
#endif /* FEATURE_UI_8K_SUPPORT */
  UI_MARKOV_13K                    /* New version of 13K Markov */
} ui_markov_call_type;

typedef enum {
  UI_NO_LOOPBACK,                   /* Not performing Loopback */
#ifdef FEATURE_UI_8K_SUPPORT
  UI_LOOPBK_8K=10,                  /* 8K Loopback !!force it to be > Markovs */
  UI_LOOPBK_13K                     /* 13K Loopback */
#else
  UI_LOOPBK_13K=10                  /* 13K Loopback !!force it to be > Markovs */
#endif /* FEATURE_UI_8K_SUPPORT */
} ui_loopback_type;

typedef enum {
  UI_LOCK_CODE,                     /* 4 digit lock code */
  UI_MENU_LOCK_CODE,                /* 4 digit Security menu lock code */
  UI_SECURITY_CODE,                 /* 6 digit Security code */
  UI_FIELD_SVC_CODE                 /* 6 digit Field Service code */
} ui_lock_code_type;


typedef struct {                  /* Calling Number ID type */
  ui_edit_type n;                 /* Number storage */
  byte    name[NV_MAX_LTRS];      /* Name associated with number */
  byte    name_length;            /* length of the name */
  byte    pi;                     /* Presentation indication */
} ui_cni_type;


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
typedef struct {
  boolean                         active;
  uasms_bc_service_info_type      info;
} ui_uasms_bc_service_info_type;

typedef struct {
  uint16                          size;
  ui_uasms_bc_service_info_type   entries[UASMS_BC_TABLE_MAX];
} ui_uasms_bc_table_type;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */


#else

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
typedef struct {
  boolean                         active;
  uasms_bc_service_info_type      info;
} ui_uasms_bc_service_info_type;

typedef struct {
  uint16                          size;
  ui_uasms_bc_service_info_type   entries[UASMS_BC_TABLE_MAX];
} ui_uasms_bc_table_type;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */
#endif

/* Structure which holds all information that needs to be passed between */
/* state machine pieces                                                  */
typedef struct {
  int     event;                  /* the current action/event */

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
  
#else  /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
  ui_cmd_type evt_param;          /* in the future, parametered event will 
                                  ** used, this is a transition toward that,
                                  ** only valid for Call Manager related
                                  ** event. 
                                  */

  cm_ph_info_s_type ph_info;      /* phone information */

  boolean getkeys;                /* whether state machine is accepting keys */

  boolean offline;                /* phone is offline? */
  boolean alert;                  /* we have an incoming alert? */
  boolean callroam;               /* roaming? - Only valid in call */
  boolean originate;              /* did we originate this? */

  boolean upcall;                 /* immediate data call? */
  boolean digital;                /* digital call? */
  boolean pwr;                    /* power on or off? */
  boolean ign;                    /* ignition on or off? */
  boolean onhook;                 /* phone on hook? */
  boolean handyphone;             /* In handyphone ? */
  boolean headset;                /* Headset attached ? */
  boolean extdev;                 /* External device ? */
  boolean mute;                   /* phone muted? */
  boolean spkr;                   /* hands free speaker? */
  boolean pad;                    /* scratch pad? */
  boolean txdtmf;                 /* transmit dtmfs? */
  boolean idling;                 /* are we idling? */


  boolean isdebug;                /* debug screen on? */
  boolean roam_ringer;            /* Roam ringer enabled ? */
#ifdef FEATURE_DS_DEBUG_UI
  boolean isdatadebug;            /* data debug screen on? */
#endif /* FEATURE_DS_DEBUG_UI */

  boolean editrcl;                /* from numedit to rcl? */
#ifdef FEATURE_HOLD_PWRKEY_TO_PWRUP
#error code not present
#endif /* FEATURE_HOLD_PWRKEY_TO_PWRUP */
boolean powerup;                /* powered up */
  boolean extpwr;                 /* external power */
  boolean batwarn;                /* warned about battery */
  byte    restrict;               /* phone restrictions */
  boolean one_min_bat;            /* One minute call time */

  word    force_so;               /* Service Option when call is forced */
  int     msg_waiting;            /* number of messages waiting at base */

  byte    nam;                    /* current nam */
  boolean hasfm[NV_MAX_NAMS];     /* whether there is analog in NAM */
  boolean hascdma[NV_MAX_NAMS];   /* whether there is CDMA in NAM */
  boolean hasacdma[NV_MAX_NAMS];  /* has channel A CDMA */
  boolean hasbcdma[NV_MAX_NAMS];  /* has channel B CDMA */
  dword   min1[NV_MAX_NAMS][NV_MAX_MINS]; /* hold min1s */
  word    min2[NV_MAX_NAMS][NV_MAX_MINS]; /* hold min2s */


  ui_edit_type n;                 /* numerical edit buffer */
  ui_edit_type a;                 /* alphanumeric buffer */


  dword needstop;                 /* need stop continuous DTMF message */
  int   blinktime;                /* time to stop blinking call timer */

  /* parameters for call stack */
  nv_stack_idx_type sidx;
  nv_stdial_type st[ NV_MAX_STACK_DIALS ]; /* call stack */
  nv_dial_type redial_buf;               /* redial storage */
#ifndef FEATURE_UI_SAVE_REDIAL_BUFFER
  boolean      redial_secret;            /* redial secret status */
#endif
  boolean schg;

  /* sorted names for alpha recall */
  boolean isdial[ NV_MAX_SPEED_DIALS+1 ]; /* whether number is here or not */

  /* both curkey and prevkey are stored as ascii symbol value */
  byte          menu_curkey;       /* key just pressed for menu entry */
  byte          menu_prevkey;      /* previous key pressed for menu entry */

  /* parameter for UI_MESSAGE_S */
  ui_field_item_enum_type msg;    /* which message to display */
  word          msglen;           /* time in seconds, if 0 use default */

  /* parameter for UI_LIST_S */
  ui_list_type  *list;            /* which list to use */
  word          lval;             /* list choice */
  byte          lnam;             /* extra parameter - NAM */
  byte          lmin;             /* extra parameter - MIN */
  boolean       edvol;            /* editing volume - not from menus */
  boolean       menuvol;       /* editing menu volume             */

  /* parameter for UI_ALPHAEDIT_S */
  ui_edit_type ed;                /* edit buffer */
  
  /* queues for UI_INFO_S */
  q_type       info_q;            /* informational messages */
  q_type       info_free_q;       /* informational free queue */

  /* --------------------- User Preferences ------------------- */
  boolean lock;                   /* phone locked? */
  char    pass[ NV_LOCK_CODE_SIZE ];
  dword   igntime;                /* ignition off time */

  boolean burst;                  /* manual burst DTMF */
  boolean sburst;                 /* string burst dtmf */


  byte backval;                   /* backlight value */
  byte backlight1;                /* backlight for normal use */
  byte backlight2;                /* car kit backlight */
  boolean hyphen;                 /* auto-hyphenation */
  boolean time_show;              /* show time if available or never */
  dword air_cnt;                  /* number of calls since reset */
  dword roam_cnt;                 /* number of roam calls since reset */
  dword life_cnt;                 /* total number of calls */

  boolean sms_as_is_time;         /* SMS UTC time */

  boolean fadebeep;               /* call fade beep */
  boolean minbeep;                /* one minute beep */
  boolean areabeep;               /* service area change beep */
  byte  beep_lvl;                 /* key beep level            */
  byte  beep_spkr_lvl;            /* key beep level in car kit */
  byte  beep_headset_lvl;         /* key beep level in headset */
  byte  ear_lvl;                  /* voice level               */
  byte  ear_spkr_lvl;             /* voice level in car kit    */
  byte  ear_headset_lvl;          /* voice level in headset    */
  byte alerts_lvl;                /* SMS alerts volume        SMS Volume */
  byte alerts_lvl_shadow;         /* SMS alerts volume in HFK  SMS Volume */

  byte          rings;            /* number of rings incoming */

  boolean       isretry;          /* auto retry? */
  byte          retry;            /* retry time */
  boolean       isanswer;         /* auto answer? */
  byte          answer;           /* answer rings */
  dword         aircount[ NV_MAX_NAMS ];  /* number of home calls */
  dword         roamcount[ NV_MAX_NAMS ]; /* number of roam calls */
  dword         calltime_g ;              /* GEMINI call timer, all nams */
  dword         lifetime_g ;              /* GEMINI life timer, all nams */
  byte ringer_level;              /* indicates what the ringer level is set to */
  byte ringer_spkr_lvl;           /* ringer level when in car kit */
#ifdef FEATURE_MULTIPLE_RINGER_TYPES
#error code not present
#endif /* FEATURE_MULTIPLE_RINGER_TYPES */
#ifdef FEATURE_UI_ESCALATING_RING
#error code not present
#endif /* ifdef FEATURE_UI_ESCALATING_RING */
  dword         airtime[ NV_MAX_NAMS ]; /* air timer */
  dword         roamtime[ NV_MAX_NAMS ]; /* roam timer */
  boolean       newtime[ NV_MAX_NAMS ]; /* call was made - update timers */
  dword         runtime;          /* run timer */
  boolean       voice13k;         /* TEMP 13K voice selection */
  boolean       earpieceup;       /* Gemini earpiece up */
  boolean       onetouch;         /* One Touch dial enabled */
  boolean       canlookup;        /* Valid memory location */
  boolean       wasonetouch;      /* One touch dialed */
  boolean       disploc;          /* BOOK with a memory loc specified */
  boolean       listloc;          /* List display from index */
  byte          rcl_index;        /* Start index for recall list */
  ui_infotxt_type  *info_display; /* INFO key help menu displays */
  boolean       op_help_only;     /* Operational help displays only */
  boolean       alpha_entry;      /* In alpha entry mode */
  boolean       postscript;       /* Re-edit name and number mode */
  boolean       emergency_edit;   /* Emergency numbers edit mode */
  boolean       prepend;          /* Prepend digits mode */
  boolean       prepend_ok;       /* Prepend available state */
  nv_dial_type  dial;             /* current working dial number */
  ui_edit_type  prepend_n;        /* prepend numberical edit buffer */
  boolean       digit_srch;       /* Search for digits in numbers */
  boolean       send_pin;         /* Send PIN code feature */
  ui_cni_type   cni;              /* Calling Number ID */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#ifdef FEATURE_OTASP
  boolean       otasp_failed;     /* Ending status of an OTASP call */
#endif

/* TEMPORARY change, until deep sleep timing bug fixed. */
  boolean       systime_valid;    /* Valid system time */

  boolean ok_to_sleep;            /* indicates whether UI OKed sleeping */


  boolean       allow_mode_sel;   /* Allow mode selection from the menus */
#ifdef FEATURE_UI_PRIVACY_ALERT
#error code not present
#endif
  boolean       is_idling ;

  boolean clrallbeep;          /* need clrall beep? */
#ifdef FEATURE_UI_QUICK_VM_DIAL
#error code not present
#endif
  snd_tone_type   key_down_tone; /* which tone is playing for key */
  snd_tone_type   key_down_dtmf; /* which dtmf is playing for key */

#ifdef FEATURE_UI_KEYGUARD
#error code not present
#endif

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */

#ifdef FEATURE_PHONE_VR
#error code not present
#endif
#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif /* FEATURE_ANALOG_HFK */
  boolean       charging;

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
  uasms_bc_config_e_type        bcsms_config;
  uasms_bc_pref_e_type          bcsms_pref;
  ui_uasms_bc_table_type        bcsms_srvc_table;
  ui_uasms_bc_service_info_type bcsms_srvc_info;
  byte                          bcsms_menu_index;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */

  byte          curr_menu_sel;  /* key numbers 1-9 and 0 for menu entry selection */

#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */

#ifdef FEATURE_USB_TESTING
#error code not present
#endif /* FEATURE_USB_TESTING */

byte              music_lvl;        /* by hjcho for adjusting volume level */
ui_edit_type      last_dial;        /* by hjcho for redial                 */

#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
} ui_state_var_type;

extern ui_state_var_type ui;


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===============================================================================

                                   EQUS UI Variables
	   
       (Event, Phone state, Message, Service Program(NAM), Editor, Password)

=================================================================================*/
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
//typedef struct tagEVENT                 eqsEvent;
//typedef struct tagSTATE                 eqsState;
//typedef struct tagMESSAGE               eqsMessage;
//typedef struct tagSERVICEPROG           eqsSrvProg;
//typedef struct tagEDITOR                eqsEditor;
//typedef struct tagCODE                  eqsCode;
//typedef struct tagPHONEBOOK             eqsPhoneBook;
//typedef struct tagSOUNDSTATE            eqsSoundState;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
#if 0
/*
**----------------------------------------------------------------------------
**	                    Event (event, event parameter...)
**----------------------------------------------------------------------------
*/
struct tagEVENT{
	int                Event;          //ui.event;
	ui_cmd_type        Param;       //ui.evt_param;
};
#endif

#if 0
/*
**----------------------------------------------------------------------------
**	             Phone State (Debug, Sleep, Mute, Lock ...)       
**----------------------------------------------------------------------------
*/
struct tagSTATE{
	boolean     InDebug;               //ui.isdebug        
	boolean     SleepOK;               //ui.ok_to_sleep
	boolean     PowerOn;               //ui.pwr
	boolean     OneTouchDialed;        //ui.wasonetouch
	boolean     Mute;                  //ui.mute
	boolean     InIdle;                //ui.is_idling
	boolean     OneMinBattery;         //ui.one_min_bat
	boolean     InAlphaEntry;          //ui.alpha_entry
	boolean     BatteryWarn;           //ui.batwarn
	boolean     FromEdit2RCL;          //ui.editrcl
	boolean     InIdling;              //ui.idling
#ifdef FEATURE_DS_DEBUG_UI	
	boolean     InDataDebug;           //ui.isdatadebug
#endif /* FEATURE_DS_DEBUG_UI */	
	boolean     InLock;                //ui.lock
	boolean     InOffline;             //ui.offline
	boolean     PowerUp;               //ui.powerup
	boolean     PhoneRestrict;         //ui.restrict
	boolean     DispAlloc;             //ui.disploc
	boolean     ListAlloc;             //ui.listloc
	boolean     SystemValid;           //ui.system_valid
	boolean     GetKeys;               //ui.getkeys
	boolean     OneTouchDial;          //ui.onetouch
	boolean     EarPieceUp;            //ui.earpieceup
	boolean     SilentPad;             //ui.pad
	dword       IgnitionTime;          //ui.igntime
    boolean     Ignition;              //ui.ign
    boolean     SysTimeValid;          //ui.systime_valid
};
#endif
#if 0
/*
**----------------------------------------------------------------------------
**	                               Message
**----------------------------------------------------------------------------
*/
struct tagMESSAGE{
	ui_field_item_enum_type     Message;    //ui.msg
	word                        MessageLen; //ui.msglen
};
#endif


/*
**----------------------------------------------------------------------------
**	                   Service Program (NAM, MIN, Channel)          
**----------------------------------------------------------------------------
*/

/*
struct tagSERVICEPROG{
	byte        CurrentNAM;                           //ui.nam
	boolean     AllowModeSel;                         //ui.allow_mode_sel
	word        ForceSo;                              //ui.force_so
	boolean     ChannelA_CDMA[NV_MAX_NAMS];           //ui.hasacdma
	boolean     ChannelB_CDMA[NV_MAX_NAMS];           //ui.hasbcdma
	boolean     CDMA[NV_MAX_NAMS];                    //ui.hascdma
	boolean     Analog[NV_MAX_NAMS];                  //ui.hasfm
	dword       MIN1[NV_MAX_NAMS][NV_MAX_MINS];       //ui.min1
	word        MIN2[NV_MAX_NAMS][NV_MAX_MINS];       //ui.min2
};
*/

#if 0
/*
**----------------------------------------------------------------------------
**                    Editor (Num Editor, Alpha Editor ...)
**----------------------------------------------------------------------------
*/
struct tagEDITOR{
	ui_edit_type Alpha;           //ui.a
	ui_edit_type Num;             //ui.n
	ui_edit_type Edit;            //ui.ed
	boolean      EmergencyEdit;         //ui.emergency_edit
	boolean      AutoHyphen;            //ui.hyphen
	boolean      PostScript;            //ui.postscript
	boolean      Prepend;               //ui.prepend
	ui_edit_type PrependNum;            //ui.prepend_n
	boolean      PrependOK;             //ui.prepend_ok
};
#endif



/*
**----------------------------------------------------------------------------
**                                    Password
**----------------------------------------------------------------------------
*/
#if 0
struct tagCODE{
	char    Pass[NV_LOCK_CODE_SIZE];
};
#endif

#if 0
/*
**----------------------------------------------------------------------------
**                                    Phone Book
**----------------------------------------------------------------------------
*/
struct tagPHONEBOOK{
	 nv_dial_type      CurrentDial;              //ui.dial
     boolean           CanLookUp;                //ui.canlookup
	 boolean           DigitSearch;              //ui.digit_srch
	 boolean           IsDial;                   //ui.isdial
};

#endif

#if 0
/**----------------------------------------------------------------------------
**	          Sound State (Service Area Change, One Minute Beep, ....) 
**---------------------------------------------------------------------------*/
struct tagSOUNDSTATE{
    boolean     SvcAreaAlert;           //ui.areabeep
	boolean     FadeAlert;              //ui.fadebeep
	boolean     OneMinBeep;             //ui.minbeep		
    boolean     BurstDTMF;              //ui.burst
	boolean     ClrAllBeep;             //ui.clrallbeep
	boolean     RoamRinger;             //ui.roam_ringer
	boolean     ShortBurstDTMF;         //ui.sburst
    boolean     EditVolume;             //ui.edvol
    boolean     MenuVolume;             //ui.menuvol
};
#endif



/*
**---------------------------------------------------------------------------
**  Variable Declarations  
**---------------------------------------------------------------------------
*/
//extern eqsEvent           gEvent;             //defined in uistate.c
//extern eqsState           gState;             //defined in uistate.c
//extern eqsMessage         gMessage;           //defined in uistate.c
//extern eqsSrvProg         gSrvProg;           //defined in uistate.c
//extern eqsEditor          gEditor;            //defined in uistate.c
//extern eqsCode            gCode;              //defined in uistate.c
//extern eqsPhoneBook       gPBook;             //defined in uistate.c
//extern eqsSoundState      gSndState;          //defined in uistate.c

#endif//(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)




/* <EJECT> */
/*===========================================================================
FUNCTION UI_NUM_DISP

DESCRIPTION
  Handle all the garbage associated with turning a beautifully pristine and
  compact number/name into something that can be displayed on the screen
  for that user person.  Messes with ui.n, ui.a, ui_num_f, ui_alpha_f.

===========================================================================*/
void ui_num_disp(
  nv_dial_type *dial              /* the dial information */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_CALL_TIMER

DESCRIPTION
  Update the call timer display with the current time in call

RETURNS
  Nothing
===========================================================================*/
void ui_call_timer( void );       /* in UISCALL.C */
/* <EJECT> */

/*===========================================================================
FUNCTION UI_DISPLAY_TIME_AND_DATE

DESCRIPTION
  Update the display fo time and date if available. This screen ui_banner_f
  is used for this purpose which is displayed when phone is idle.

RETURNS
  Nothing
===========================================================================*/
void ui_display_time_and_date( void ); /* in UISCALL.C */

/* <EJECT> */

#ifdef FEATURE_DS
/*===========================================================================
FUNCTION UI_DATACALL_STATUS

DESCRIPTION
  Update the data call status display with the current RLP status

RETURNS
  Nothing
===========================================================================*/
void ui_datacall_status( void );       /* in UISCALL.C */
#endif



/* <EJECT> */
/*===========================================================================
FUNCTION MIN2_ASCII

DESCRIPTION
  Convert a MIN2 type number to ASCII
  Returns pointer to character after last character written
===========================================================================*/
char *ui_min2_ascii(
  dword minc,                     /* min2 to convert from */
  char *txt                       /* room for 3 digits */
);

/* <EJECT> */
/*===========================================================================
FUNCTION MIN1_ASCII

DESCRIPTION
  Convert a MIN1 type number to ASCII
  Returns pointer to character after last character written
===========================================================================*/
char *ui_min1_ascii(
  dword minc,                     /* min1 to convert from */
  char *txt                       /* room for 8 digits */
);


/* <EJECT> */
/*===========================================================================

FUNCTION UI_SETUP_CLI

DESCRIPTION
  Setup CLI structure, ui.cni. Truncate CLI to maximum length of 
  UI_MAX_DIALED_DIGITS. NULL terminate CLI. Performs phone book lookup
  for a CLI string. If there is a match, copy the phone book data to
  the CLI structure.
                              
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Setup CLI structure, ui.cni.
  Notify module system unit, if module is defined.
  
===========================================================================*/
void ui_setup_cli
(
 /* CLI string, may contain alpha or control character
 ** Not necessary NULL terminated */
 char* chars, 
 /* CLI string length, UI_MAX_DIALED_DIGITS or any value bigger than or 
 ** equal to the actual length if chars is NULL terminated 
 */
 int2 num,
 /* CLI presentation indicator */ 
 byte pi
 ) ;

/* <EJECT> */
/*===========================================================================

FUNCTION UI_SETUP_CLI_DISPLAY

DESCRIPTION
  Setup incoming call display based on the CLI structure, ui.cni. 
  If there isn't any number stored, "Incoming Call" message is shown
  on screen. Otherwise, "Call From" message with name and auto-hyphenated 
  caller ID is shown.
  
DEPENDENCIES
  CLI structure, ui.cni, must be setup first by calling
  ui_setup_cli

RETURN VALUE
  None

SIDE EFFECTS
  Change display.
  Set display_mode = ACT_CLI, if there is number stored in ui.cni.
  Set display_mode = ACT_NO_CLI, otherwise.
  
===========================================================================*/
void ui_setup_cli_display (void);

/* <EJECT> */
/*===========================================================================

FUNCTION UI_UPDATE_CLI_DISPLAY

DESCRIPTION
  Update the CLI roam/analog indication text and the call type information.
  It checks the DB items DB_ROAM and DB_DIGITAL and puts text corresponding
  to the current mode to the incoming call display field. This function
  changes both "Incoming call" and "Call from" messages. Since only one
  of them is activated at one time, this function visually affects only one
  that is activated.
                              
DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  Change display.

  
===========================================================================*/
void ui_update_cli_display (void) ;


/* <EJECT> */
/*===========================================================================

FUNCTION UI_CLEAR_CLI_DISPLAY

DESCRIPTION
  Deactivate the CLI display, either "Incoming Call" or "Call from" message
                              
DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  Set display_mode = DEACT.
  Change display.

===========================================================================*/
void ui_clear_cli_display (void) ;

/* <EJECT> */
/*===========================================================================

FUNCTION UI_SAVE_CLI

DESCRIPTION
  Saves the current CLI information to the call history list

  This routine is called to store call information of all calls with valid
  CLI: including call to, call by, missed call, call-waiting call and 
  3-way calls.
  
  If FEATURE_UI_RESTRICTED_IN_LIST is defined, this routine also stores calls
  which have restricted or unavailable number in CLI.

DEPENDENCIES
  Caller ID information in ui.cni setup by ui_setup_cli before calling
  this routine.

RETURN VALUE
  None

SIDE EFFECTS
  Change content of variable ui.n, ui.a and call history.
  
===========================================================================*/
void ui_save_cli 
(
 byte status /* Type of CLI to save */
 );

/* <EJECT> */
/*===========================================================================
FUNCTION UI_NV_SAVESTUFF

DESCRIPTION
  Save stuff in the NV that needs to be saved for offline or powerdown

DEPENDENCIES
  Called from UI_DO_EVENT
===========================================================================*/
void ui_nv_savestuff( void );     /* save nv items */


/* <EJECT> */
/*===========================================================================
FUNCTION UI_GET_KEYS ( UISTATE.C )

DESCRIPTION
  Whenther state machine is accepting keys

DEPENDENCIES
  Called from UI_DO_EVENT
===========================================================================*/
void ui_get_keys(
  boolean onoff
);

/* <EJECT> */

/*===========================================================================
FUNCTION fast_update_rssi_icon

DESCRIPTION
  Update RSSI icon based only on the  new service state. 
===========================================================================*/
void fast_update_rssi_icon( const ui_ph_event_type *ph_event_ptr );

/* <EJECT> */
/*===========================================================================
FUNCTION NUM_FORMAT

DESCRIPTION
  Take a number and format it for display on a screen type display. In this
  screen pause characters are possible.

RETURN VALUE
  Number of screens

DEPENDENCIES
  Called from UI_EVENT
===========================================================================*/
int num_format(
  ui_edit_type *e_ptr,            /* edit pointer */
  char         *rtxt,             /* result text */
  int           scrn              /* which screen, 0 is bottom */
);

/* <EJECT> */
/*===========================================================================
FUNCTION CNI_FORMAT

DESCRIPTION
  Take format CNI information for display on a screen type display. In this
  screen pause characters are not possible.

RETURN VALUE
  Number of screens

DEPENDENCIES
  Called from UI_EVENT
===========================================================================*/
int cni_format(
  ui_edit_type *e_ptr,            /* edit pointer */
  char         *rtxt,             /* result text */
  int     scrn                    /* which screen, 0 is bottom */
);

/* <EJECT> */
/*===========================================================================
FUNCTION FORMAT_DISPLAY

DESCRIPTION
  Take a number and format it for display on a screen type display.

RETURN VALUE
  Number of screens

DEPENDENCIES
  None
===========================================================================*/
int format_display (
  ui_edit_type *e_ptr,            /* edit pointer */
  char         *rtxt,             /* result text */
  int           scrn,             /* which screen, 0 is bottom */
  boolean      pauses
);

/* <EJECT> */
/*===========================================================================
FUNCTION uicode_init ( UISCODE.C )

DESCRIPTION
  Initialize necessary parameters for code entry

RETURN VALUE
  None

DEPENDENCIES
===========================================================================*/
void uicode_init (
  ui_lock_code_type code            /* lock type */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_STARTUP ( UISSTART.C )

DESCRIPTION
  User interface state machine startup routine - initialize UI, etc.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_startup( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_IDLE ( UISIDLE.C )

DESCRIPTION
  User interface state machine idle state.  Doing nothing.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_idle( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_MENU ( UISMENU.C )

DESCRIPTION
  Show the menus for the state machine

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_menu( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_MAIN_MENU

DESCRIPTION
  User interface state machine handle menu functions

DEPENDENCIES
  Called from UI_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_main_menu( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_other ( UISOTHER.C)

DESCRIPTION
  User interface other menu state

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display and nv
===========================================================================*/
ui_maj_type uistate_other( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_SERVICE ( UISSERV.C )

DESCRIPTION
  User interface service programming

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display and nv
===========================================================================*/
ui_maj_type uistate_service( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_CALL ( UISCALL.C )

DESCRIPTION
  User interface state machine call state.  In call.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_call( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_CLI ( UISCLI.C )

DESCRIPTION
  User interface state machine call state.  In call.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_cli( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_MSG ( UISMSG.C )

DESCRIPTION
  Handle quick messages for the state handler

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_msg( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_INFO ( UISINFO.C )

DESCRIPTION
  Display messages from the base station

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_info( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_RCL ( UISRCL.C )

DESCRIPTION
  User interface state machine recall numbers state.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_rcl( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_STO ( UISSTO.C )

DESCRIPTION
  User interface state machine store numbers state.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_sto( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_VIEW ( UISVIEW.C )

DESCRIPTION
  Do the number viewing for editing, etc.

DEPENDENCIES
  Called from UI_DO_EVENT only

RETURN VALUE
  None

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_view( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_ALPHAEDIT ( UISALPHA.C )

DESCRIPTION
  Do the alpha editing for the user interface, for the mobile

DEPENDENCIES
  Called from UI_DO_EVENT only

RETURN VALUE
  None

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_alphaedit( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_NUMEDIT ( UINUM.C )

DESCRIPTION
  Do the numerical editing for the user interface, for the mobile

DEPENDENCIES
  Called from UI_DO_EVENT only

RETURN VALUE
  None

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_numedit( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_LIST  ( UISLIST.C )

DESCRIPTION
  Do list selection for the mobile

DEPENDENCIES
  Called from UI_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_list( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_OFF ( UISOFF.C )

DESCRIPTION
  The off state for the state machine

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_off( void );         /* in UISPWR.C */

/* <EJECT> */
#ifdef FEATURE_LPM
#error code not present
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_CMDS

DESCRIPTION
  Handle commands to the user interface task

DEPENDENCIES
  Called from DO_CMDS

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void uistate_cmds (
  ui_cmd_type *cmd_ptr
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_NEW_MISSED_CALL

DESCRIPTION
  update missed call count and set up display

DEPENDENCIES
  ui_missed_call
  ui_time_string

RETURNS
  None

SIDE EFFECT
  None
===========================================================================*/
void ui_new_missed_call(void);

/* <EJECT> */
/*===========================================================================
FUNCTION FORMAT_TIME

DESCRIPTION
  Formats time display line.

DEPENDENCIES
  Current time must been available when time was stored.

SIDE EFFECTS
===========================================================================*/
void ui_format_time (dword current_time, byte *disp_ptr);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_DO_EVENT

DESCRIPTION
  The main state machine called when an event happens.  Farm the stuff
  out to the appropriate procedure,

RETURNS
  Nothing

DEPENDENCIES
  Called from UI_MAIN
===========================================================================*/
void ui_do_event(
  word  in_event
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_LOCKED ( UISLOCK.C )

DESCRIPTION
  User interface state machine locked ( Earpiece down ) state.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
===========================================================================*/
ui_maj_type uistate_locked( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_HELP ( UISHELP.C )

DESCRIPTION
  User interface state machine help menus state.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
===========================================================================*/
ui_maj_type uistate_help( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISTATE_CODE ( UISCODE.C )

DESCRIPTION
  User interface state machine enter lock code state.

DEPENDENCIES
  Called from UI_DO_EVENT only

SIDE EFFECTS
  Plays with the display
===========================================================================*/
ui_maj_type uistate_code( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UI_NAM_INFO_DISPLAY (uisidleg.c)

DESCRIPTION
  Displays current NAM name and number for 5 seconds unless a key is pressed.

DEPENDENCIES
  UI must have been initialzed and idle.

SIDE EFFECTS
  modifies display
===========================================================================*/
void ui_nam_info_display( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UI_FORMAT_DATE_TIME  (uisrclg.c)

DESCRIPTION
  Formats date and time display lines.

DEPENDENCIES
  Current time must been available when time was stored.

SIDE EFFECTS
===========================================================================*/
void ui_format_date_time (dword current_time, byte *disp_ptr);

/* <EJECT> */
/*===========================================================================
FUNCTION  ui_calls_list_add

DESCRIPTION
  add an entry in call list. mostly copied from uinscall.c, 
  remove dependency on global variable ui.n, ui.pse

DEPENDENCIES
  global variable ui.sidx, ui.st
  clock task started

RETURNS
  None

SIDE EFFECTS
  global variable ui.schg set to TRUE
===========================================================================*/
void ui_calls_list_add (  
  byte           status,        // call to/from, 3-way etc
  const byte*    num_buf,       // buffer for the number
  byte           num_len,       // length of the number
  const byte*    alpha_buf,     // buffer for alpha tag
  byte           alpha_len,     // length for alpha tag
  boolean        secret         // number is secret?
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_GET_LOCAL_TIME

DESCRIPTION
  Calculate local time based on the system time, leap seconds, local time
    offset and daylight savings time.

    According to IS95A -
    7.7.1.3 Sync Channel Message: Local time = SYS_TIME - LP_SEC + LTM_OFF

    * Daylight savings time must also be added to this equation.
    Local time = ( SYS_TIME - LP_SEC + ( LTM_OFF + DAY_LT ) )

    If the time returned by clk_read_secs is invalid, the invalid time
    is returned without any local time offset.

DEPENDENCIES

RETURNS
  local time in seconds
===========================================================================*/
dword ui_get_local_time ( void ) ;

/* <EJECT> */

#ifdef FEATURE_CALL_INFO_PACKET
#error code not present
#endif /* FEATURE_CALL_INFO_PACKET */


#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

/* <EJECT> */
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */


#endif

#endif //#ifdef MMI_TREX
