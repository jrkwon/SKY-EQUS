#ifndef UIINT_H
#define UIINT_H
/*===========================================================================

              U S E R   I N T E R F A C E   I N T E R N A L


DESCRIPTION
  Everything internal to the user interface that has to be shared among
  the modules, but we don't really want everyone else to know.

  A significant part of the user interface is run as a state machine.
  Its states are described here.

  Copyright (c) 1991 through 1998 by QUALCOMM INCORPORATED  All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uiuint.h_v   1.4   18 May 2000 10:26:24   gshi  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/00   HQ      Changed SECRET_SD_NUM to NV_SECRET_SD_NUM;
                   added more status definitions for NV_DIAL_I;
                   use EMERGENCY_NUMBERS instead of 3 for UI_NV_DIALS.
02/17/00   HQ      Added macro HFK_ON for Analog HFK VR support.
01/28/00   HQ      Added HFK support - wake-up training, voice activation,
                   incoming call voice answer, and expert window.
12/18/99    CS     Added ui_get_nv_for_cm function.
08/31/99    HD     Took out SILENT_REDIAL switch and code in it.
07/07/99    VT     Merged TRUE IMSI changes.
4/08/99     CS     Added function ui_initialize_imsi_t_components for IS95B
                   TRUE IMSI.
11/23/98   SIU     Added service changed support.
07/17/98   JGR     Added event for Net Mode under FEATURE_DS_UP
06/05/98   YM      Added support for one touch mute/unmute
05/29/98   AYK     Added the timer for the animated ellipsis called
                   UI_ANIMATE_DOT_TIMER.
05/22/98   YM      Added support for Low Power Mode
04/24/98   EJL     Changed some plain "enums" to "typedef enum".
02/14/98   AYK     Added DTACO support under FEATURE_UI_DTACO.
01/07/98   KAR     Removed associated code with FEATURE_CAR_KIT_DTMF
12/23/97   SIU     Caller ID/Call history enhancement.
12/11/97   ldg     Moved data services debug screen prototypes to uindata.
11/24/97   ldg     Added extern declaration for data text pointers.
11/13/97   jgr     Added more support and update support for Unwired Planet.
                   Changes are ifdef FEATURE_DS_UP
09/09/97   ldg     Moved data debug UI display indices to uin.c.
08/18/97   dna     Removed ifdefs on ui_initialize_imsi_components for Q800
07/31/97   ldg     Added data services debug UI under FEATURE_DS_DEBUG_UI.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/17/97   SIU     Removed pragma statement with target name.
06/04/97   SIU     Added missing edit history of change 06/02/97.
06/02/97   SIU     Added function ui_is_silent_redial() and
                   ui_redial_time_expired().
04/22/97   BK      Added Restricted and Unavailable numbers to call stack.
04/22/97   BK      Rid unworking sni timer stuff.
04/19/97   RC      Implemented FEATURE_HOLD_PWRKEY_TO_PWRUP in which the power
                   key needs to be pressed for 3 sec. to power on the MS.
03/12/97   jjn     Added (TG==T_MD) in conditional compile statements
                   wherever appropriate for the Module
02/11/97   BK      Temp mod to clear system time display after deep sleep.
02/04/97   RC      Added function ui_disp_datacall_alert().
02/04/97   JGR     Added "#" and "*" press and helds for network mode
01/27/97   RC      Added function ui_send_mc_cmd().
01/09/97   BK      New UI to SOUND interface modifications.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
12/16/96   RC      function ui_initialize_imsi_components() is now defined when
                   FEATURE_JSTD008 is defined -- previously it was defined when
                   FEATURE_OTASP was defined.
11/07/96   RC      Added FUNCTION ui_add_event.
10/23/96   RC      Added FUNCTION ui_add_event_at_tail.
09/25/96   BK      Add FUNCTION ui_initialize_imsi_components.
09/13/96   BK      Removed UI_BUSYSIG_F.
                   Added UI_DO_END_ACTIVATION_F and UI_DO_OTASP_STATUS_F for
                   FEATURE_OTASP.
08/27/96   RC      Eliminated UI_OTHER_S from ui_maj_type.
08/02/96   BK      Implemented table driven menus.
07/22/96   BK      Added FEATURE_UI_MORE_911_NUMBERS
07/17/96   BK      Added FEATURE_UI_QUICK_VM_DIAL.
05/21/96   BK      Tied MENU_002 to FEATURE_UI_ENHANCED_PRIVACY_SET.
05/21/96   BK      UI_FEATURE_MENU_002 ( yet another menu ) is included.
04/18/96   BK      New UI branching created from uiint.h ver 1.63
04/02/96   BK      Added FEATURE_UI_CHECK_BILLING and PCS menu ordering.
03/14/96   BK      Added ifdefs for customer specific builds.
03/05/96   BK      Replace T_PRMCO1900 with T_GP. New power down features
                   for QCP-1900
02/13/96   BK      T_PRMCO1900 introduction
12/21/95   BNS     Lint cleanup
12/20/95   BNS     Changed constants for Antenna Aiming mode to work with new
                   cdma rssi values returned from search
12/01/95   BNS     Changed define for UI_RSSI_TIMER to remove conflict with
                   UI_ISS0 and changed comment for UI_ISS0
                   Put #ifdef T_HAS_LCD around UI_MSG_TIMER define
11/30/95   BNS     Added support for antenna aiming mode in ISS2
11/30/95   CAH     Rid magic number for one year.
11/03/95   DAK     Added prototype and constant for offhook timer.
10/30/95   RAS     Updates to have css build from \ref
10/25/95   DAK     Added prototype for ui_3wc_state_timer_set()
10/23/95   DAK     Made some constants long to avoid lint errors.
10/11/95   DAK     Added conditionals to include uitxtw.h in ISS2
10/10/95   DAK     Added conditional compilation flags for screen code.
09/11/95   BNS     Added UI_ALARM_SHUTOFF_TIMER definition, added prototypes
                   for alarm timer setting functions.
08/31/95   DAK     Added definitions for FLASH, ALARM and SNOOZE timer
                   expirations. Added new states for FEATURE, FEATURE_DONE
                   and BAD_FEATURE. Added new FLASH, ALARM and SNOOZE events.
                   All changes are ISS2 specific.
08/17/95   CAH     New incoming call handling including support for new
                     Markovs and SO9.
08/16/95   DAY     Add #ifdef around ui_free_nv and SMS state for Gemini
                   target.
08/14/95   DAY     Added new SMS events and major UI state - UI_SMS_S.
08/10/95   CAH     Rid unused Markov defines.
08/03/95   CAH     Gemini garbage display fix.  Fixes SCTS 284 and the FE
                    bug in which INFO displays garbage.
07/07/95   CAH     Fixed non Gemini compile error for UI_8K_MARKOV
06/27/95   CAH     Gemini backlight when earpiece up support.
06/20/95   CAH     Rid unnecessary <memory.h> include and preliminary
                    Gemini 13K Markov support.
06/13/95   CAH     Gemini Blink SNI LED during alert.
05/29/95   CAH     Gemini preliminary Emergency number support.
05/16/95   CAH     Gemini SECRET and Calls List support.
05/01/95   CAH     Initial Gemini mods.  New Pause handling for Beta2 and
                    Portables.
04/15/95   RAS     Initial ISS2 target support.
11/18/94   CAH     Data call status display
11/14/94   CAH     ISS1 sleep during a call bug fix
11/09/94   CAH     ISS1 mobile to mobile busy signal support fix
11/09/94   CAH     Mobile to mobile busy signal support fix
08/09/94   CAH     Lint modifications
07/22/94   CAH     ISS1 modifications
05/18/94   CAH     Ron's ISS0 updates as per release 56
10/13/92   RRD     Cleanup and commenting
02/08/92   RRD     Created

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#include "comdef.h"
#include "hs.h"
#include "target.h"
#include "customer.h"
#include "clk.h"
#include "dog.h"
#include "mc.h"
#include "uiutxt.h"
#include "ui.h"
#include "nv.h"


#define DISP_TIME 250L        /* check RSSI every 250 ms */
#define PAUSE_TIME 2000L      /* Gemini Timed Pause is 2 seconds */
#define ONE_YEAR 31536000L    /* one year in seconds */

/* TEMPORARY change, until deep sleep timing bug fixed. */
#define INVALID_SYS_TIME (31536000L - 1L)  /* less than one year in seconds */

/* Phone restriction bits */
#define NV_RSTRCT_ANY  0x0001     /* any restrictions? */
#define NV_RSTRCT_ROAM 0x0002     /* allow roaming? */
#define NV_RSTRCT_IN   0x0004     /* any incoming calls? */
#define NV_RSTRCT_OUT  0x0008     /* any outgoing calls? */
#define NV_RSTRCT_USER 0x0010     /* dial user entered numbers? */
#define NV_RSTRCT_DMEM 0x0020     /* dial numbers from memory? */


/* defines for the callback timer expirations */
#define UI_KEY_TIMER      0x01    /* key down timer */
#define UI_GEN_TIMER      0x02    /* general countdown timer */
#define UI_DISP_TIMER     0x04    /* one second display timer */
#define UI_MENU_TIMER     0x08    /* menu/list timer */
#define UI_MSG_TIMER      0x10    /* message timer for uistate_msg only */
#define UI_DISPMSG_TIMER  0x20    /* Temporary message display timer */
#define UI_SMSMSG_TIMER   0x40    /* SMS status message display timer */
#define UI_ALPHA_TIMER    0x800   /* Gemini Alpha entry interdigit timer */



/* redstar  2000.09.21    timer bit mask */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
#define UI_EQS_TIMER1      0x80
#define UI_EQS_TIMER2      0x100
#define UI_EQS_TIMER3      0x200
//#define UI_EQS_TIMER4      0x400  
#endif


#define UI_ANIMATE_DOT_TIMER 0x400   /* Timer for animated ellipsis. */
                                     /* Used by feature_ui_animate_search. */

#define UI_PAUSE_TIMER    0x1000  /* Timed Pause timer */
#define UI_BKLT_SLEEP     0x8000  /* backlight sleep? */

/* define this for the tx power test code */
/* #define TXTEST */

/* define this to debug return values from menus and lists */
/*#define UIDEBUGEVENT*/

/* seconds to wait on restriction error message - 2 seconds */
#define UI_RSTRCT_PAUSE 2000

/* Speed Dial numbers status values */
/* Bit 7 of Calls List status byte */
#define NV_NON_SECRET_SD_NUM 0    /* Normal (Non secret) speed dial number */
#define NV_SECRET_SD_NUM     1    /* Secret speed dial number */
#define NV_PRIORITY_SD_NUM   2    /* Priority speed dial number */
#define NV_EMERGENCY_SD_NUM  3    /* Emergency speed dial number */
#define NV_OTASP_SD_NUM      4    /* OTASP speed dial number */
#define NV_OTASP2_SD_NUM     6    /* OTASP-2 (customer-specific) speed dial number */


/* Call Stack entries status values */
#define CALL_TO 0              /* Outgoing call */
#define CALLED_BY 1            /* Incoming Call Answered */
#define MISSED_CALL 2          /* Incoming Call Missed */
#define ADDED_NUM 3            /* Number added from Scratchpad */
#define CALLED_BY_RES 4        /* Incoming Call Answered - Restricted */
#define CALLED_BY_UNA 5        /* Incoming Call Answered - Unavailable */
#define MISSED_CALL_RES 6      /* Incoming Call Missed - Restricted */
#define MISSED_CALL_UNA 7      /* Incoming Call Missed - Unavailable */
#define CALLWAITING 8
#define CONFERENCE  9

/* Call types text in Calls List display screen. */
/* This item is dependent on the above defines.  */
extern const byte call_stack_types[10][UI_WIDE + 1];

/* Call Stack Name Text for Restricted and Unavailable CLI calls */
#define RESTRICTED_TEXT  "Restricted  "
#define UNAVAILABLE_TEXT "Unavailable "

/* Call Stack view text for Restricted and Unavailable calls */
#define RES_UNA_TEXT "Number      "

/* FM Min is min #0, CDMA Min is min #1 */
#define FMMIN 0
#define CDMAMIN 1

#define UI_BKLT_OFF 0             /* backlight always off */
#define UI_BKLT_ON  100           /* backlight always on */
#define UI_BKLT_TURN_ON_TIME 5	  /* backlight turn-on time (sec) */ //tasik 00.05.19
#define UI_BKLT_WHEN_EARPC_UP 101 /* backlight on while earpiece up */

#define UI_NV_SECRET 90           /* Where the secret numbers begin */
#define UI_NV_DIALS (NV_MAX_SPEED_DIALS-EMERGENCY_NUMBERS) /* Total number of speed dials */
                       /* The remaining are emergency numbers */

/* The amount of time in a state machine tick - 200 ms */
#define UI_TICK_TIME  200

/* Translate a time in seconds into a number of UI_TICK_TIME ticks. */
#define SEC_TO_WAIT( sec ) ( ( sec ) * 1000L/UI_TICK_TIME )


#define ISPAUSE(chr) \
        ( ( (chr) == UI_TPSE_CHR ) || \
          ( (chr) == UI_PSE_CHR )   )


#define ISLOOP(so) \
        ( ( (so) == CAI_SO_LOOPBACK)    || \
          ( (so) == CAI_SO_LOOPBACK_13K)  )

#define ISMARKOV(so) \
        ( ( (so) == CAI_SO_RS1_MARKOV) || \
          ( (so) == CAI_SO_RS2_MARKOV) || \
          ( (so) == CAI_SO_MARKOV)     || \
          ( (so) == CAI_SO_MARKOV_13K)   )


#ifdef FEATURE_PHONE_VR_HFK
#error code not present
#endif /* FEATURE_PHONE_VR_HFK */


#ifdef FEATURE_HOLD_PWRKEY_TO_PWRUP
#error code not present
#endif /* FEATURE_HOLD_PWRKEY_TO_PWRUP */

/* Internal error codes */
typedef enum {
  DONE_S,                         /* done */
  ERR_S                           /* error */
} ui_errcode_type;


/*
** Informational messages from base station to be displayed on the
** handset.  This structure defines the buffers and how many of them
** to define.  Several CAI messages can cause these - due to formatting
** done by the UI, there is no fixed maximum size, but ten lines should
** be enough.
*/
#define UI_INFO_LINES 10            /* 10 lines each should be enough */
typedef struct {
  q_link_type link;
  int lines;                        /* number of lines in message */
  char txt[UI_INFO_LINES][UI_WIDE]; /* actual data */
} ui_info_type;

/*
** The type used for the editors - numerical and alphanumeric
*/
typedef struct {                  /* editor buffer type */
  boolean act;                    /* active or not*/
  boolean secret;                 /* secret number or not */
  byte    buf[UI_MAX_DIALED_DIGITS]; /* the data */
  byte    pos;                    /* length of the data */
} ui_edit_type;

/* When a test call made this structure will have the needed information */
/* Before the call origination command is issued, the buffers will be    */
/* updated using this structure. At this point Service Option is not     */
/* utilized. Goal for next step is to utilize so also                    */
typedef struct {
  byte         pos ;      /* number of valid digits */
  byte         buf[ UI_MAX_DIALED_DIGITS ] ;  /* number to dial */
  word         so ;   /* Service Option */
} ui_test_call_type ;


#ifdef FEATURE_OTASP
/* below flags are needed, for the case when MC decides the phone should */
/* power down, because of wrong lock code being sent over the air.       */
extern boolean ui_pwrdown_complete ;
extern boolean ui_mc_initiated_pwrdown ;
#endif



/*
** User Interface Major States
*/
typedef enum {
  UI_NOSTATE_S,                   /* no state, used for state stack */
  UI_POP_S,                       /* pop the state stack */
  UI_STARTUP_S,                   /* Starting up */
  UI_LOCKED_S,                    /* Earpiece down locked state */
  UI_CODE_S,                      /* Enter lock code state */
  UI_IDLE_S,                      /* Idling, waiting for something to happen */
  UI_CALL_S,                      /* In call, call attempt, ringing, etc. */
  UI_CLI_S,
  UI_MESSAGE_S,                   /* Show a message */
  UI_INFO_S,                      /* Info displays for user */
  UI_RCL_S,                       /* Recalling */
  UI_STO_S,                       /* Storing */
  UI_MENU_S,                      /* Menus general handling */
  UI_SERVICE_S,                   /* System setup */
  UI_VIEW_S,                      /* Viewing number */
  UI_NUMEDIT_S,                   /* Editing a number */
  UI_ALPHAEDIT_S,                 /* Editing an alpha buffer */
  UI_LIST_S,                      /* Choose from list */
  UI_HELP_S,                      /* Displaying Help menus state */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#ifdef FEATURE_UASMS
  UI_SMS_S,                       /* SMS message menus */
#endif /* FEATURE_UASMS */
#ifdef FEATURE_LPM
#error code not present
#endif

#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

  UI_OFF_S                        /* Power off  */
} ui_maj_type;


/*
** User Interface State Machine Events.  0x00-0xFF are reserved for
** events from the handset.
*/
typedef enum {
  
  UI_NONE_F = (int)HS_RELEASE_K+1,/* no event */
  UI_CLRALL_F,                    /* CLR held down */
  UI_PSEK_F,                      /* pause "key" */
  UI_TPSEK_F,                     /* timed pause "key" */
  UI_HYPHEN_F,                    /* hyphen "key" */
  UI_RCLUP_F,                     /* recall key released */
  UI_RCLDN_F,                     /* recall key held down */
  UI_DIGDOWN_F,                   /* digit (1-9) held down */
  UI_SENDDTMF_F,                  /* send string dtmf "key" */
  UI_MULTIDONE_F,                 /* multi-tone done playing */
  UI_RING_F,                      /* end of a ring */
  UI_DOABRVALERT_F,               /* abbreviated alert */
  UI_OFFLINE_F,                   /* go offline "key" */
  UI_RELEASE_F,                   /* call release */
  UI_INFO_F,                      /* information came in from base station */
  
  UI_SMUTE_F,                     /* speaker mute */
  UI_NVOICE_F,                    /* NAMPS voice mail status */
  UI_NMSG_F,                      /* NAMPS short message */
  UI_UPDISP_F,                    /* update display */
  UI_TIMER_F,                     /* general purpose timer expired */
  UI_MENUTIMER_F,                 /* menu/list timer expired */
  UI_MSGTIMER_F,                  /* uistate_msg message timer expired */
  UI_DISPMSGTIMER_F,              /* temporary message display timer expired */

/* redstars 2000-09-21 --> @for EQS local timert */
#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
  UI_EQS_TIMER1_F,
  UI_EQS_TIMER2_F,
  UI_EQS_TIMER3_F,
#endif  // FEATURE B1 or B0  
  
  UI_BOOSTCALL_F,                 /* make a call for power boost */
  UI_TOIDLE_F,                    /* back out to idle state */

  UI_LOCKWARN_F,                  /* key pressed during earpiece down */
  UI_SAVE_F,                      /* SAVE soft key pressed */
  UI_PUNC_F,                      /* Punctuation character selected */
  UI_ALPHATIMER_F,                /* Alpha entry interdigit timeout */
  UI_PAUSETIMER_F,                /* Timed Pause timeout */
  UI_PWR_K_F,                     /* Power up even though earpiece down */
#ifdef FEATURE_DS_UP
#error code not present
#endif
#ifdef FEATURE_UASMS
  UI_SMSTIMER_F,                  /* SMS status/warning timeout */
  UI_SMS_MSG_F,                   /* SMS message received */
  UI_SMS_STS_MSG_F,               /* SMS status message received */
#endif /* FEATURE_UASMS */
#ifdef FEATURE_UI_PWR_KEY_ALT
  UI_PWR_KEY_PRESS_F,             /* for the display when pwr key is pressed */
#endif

#ifdef FEATURE_OTASP
  UI_DO_END_ACTIVATION_F,         /* End otasp activation call */
  UI_DO_OTASP_STATUS_F,           /* Sent otasp activation ending status */
#endif

#ifdef FEATURE_UI_QUICK_VM_DIAL
#error code not present
#endif
#if ( defined(FEATURE_DS_UP) || defined(FEATURE_UI_ONE_TOUCH_MUTE) )
  UI_0_HELD_F,                    /* Keys 0-9 held down */
  UI_1_HELD_F,
  UI_2_HELD_F,
  UI_3_HELD_F,
  UI_4_HELD_F,
  UI_5_HELD_F,
  UI_6_HELD_F,
  UI_7_HELD_F,
  UI_8_HELD_F,
  UI_9_HELD_F,
  UI_END_HELD_F,
  UI_PF1_HELD_F,                  /* softkeys held */
  UI_PF2_HELD_F,
#endif


  // definition for new call event etc.
  // make it end with _E to avoid confusion with
  // command

  UI_CALL_INCOMING_E,             /* incoming call */
  UI_CALL_ORIGINATED_E,           /* a call is originated */
  UI_CALL_CONNECT_E,              /* the call is connected */
  UI_CALL_ENDED_E,                /* the call is ended */
  UI_CALL_SO_CHANGED_E,           /* the service option is changed */
  UI_CALL_PRIVACY_CHANGED_E,      /* the voice privacy is changed */
  UI_CALL_CALLER_ID_E,            /* caller id, could be call waiting */
  UI_CALL_FLASHED_E,              /* a flash is sent */
  UI_CALL_CMD_ERR_E,              /* some error happend to call commands */
#ifdef FEATURE_IS95B_EXT_DISP
  UI_CALL_EXT_DISPLAY_E,          /* Display Ext disp information                 */
#endif

  UI_PH_INUSE_STATE_E,            /* inuse state is changed */
  UI_PH_SRV_STATE_E,              /* service state is changed */
  UI_PH_OPRT_MODE_E,              /* operating mode is changed */
  UI_PH_CDMA_LOCK_MODE_E,         /* CDMA lock mode is changed */
  UI_PH_MODE_PREF_E,              /* prefered mode is changed */
  UI_PH_SYS_PREF_E,               /* prefered system is changed */
  UI_PH_ANSWER_VOICE_E,           /* answer voice as data setting is changed*/
  UI_PH_RES_LEVEL_E,              /* restriction level is changed */
  UI_PH_CURR_NAM_E,               /* current NAM is changed */
  UI_PH_NAM_SEL_E,                /* NAM selection is changed */
  UI_PH_ROAM_STATUS_E,            /* roaming status is changed */
  UI_PH_INFO_AVAIL_E,             /* Phone information is now available */
  UI_PH_CMD_ERR_E,                /* some error happend to phone commands */

  UI_INBAND_FWD_BURST_DTMF_E,     /* forward burst DTMF */

  UI_SS_SRV_CHANGED_E,            /* Serving system is changed */

#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

  UI_LAST_CMD_F                   /* always last */
} ui_events_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_DS
/* <EJECT> */
/*===========================================================================
FUNCTION UI_DISP_DATACALL_ALERT

DESCRIPTION
  Prepare and display the data call alert screen.

  Based on the mode ( CDMA / AMPS ) and the service option, copies a text
  string, identifing the type of the data call, to the incoming call display. The
  The service option text is written into the second line, overwriting the
  "   Call  " text.

DEPENDENCIES
  ui.alert must be TRUE.
===========================================================================*/
void ui_disp_datacall_alert(void);
#endif /* ifdef FEATURE_DS */


/* <EJECT> */
/*===========================================================================
FUNCTION UI_SEND_MC_CMD

DESCRIPTION
  Send MC_END_F or MC_POWERDOWN_F command to MC.

DEPENDENCIES

RETURNS
  boolean denoting successful send.
===========================================================================*/
boolean ui_send_mc_cmd ( mc_name_type cmd );


/* <EJECT> */
/*===========================================================================

FUNCTION UI_ADD_EVENT_AT_TAIL

DESCRIPTION
  Push an event onto the TAIL of the event queue, so it will
  be the next event to be processed.


===========================================================================*/
void ui_add_event_at_tail(
  word event                      /* event to push */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_ADD_EVENT

DESCRIPTION
  Push an event onto the event queue.


===========================================================================*/
void ui_add_event(
  word event                      /* event to push */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_ADD_PEVENT

DESCRIPTION
  Push an event onto the priority event queue.


===========================================================================*/
void ui_add_pevent(
  word event                      /* event to push */
);

#ifdef FEATURE_UI_MORE_911_NUMBERS
#error code not present
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION UI_TIMER_SET             ui.c

DESCRIPTION
  Set up for general purpose callback

DEPENDENCIES
  Called from state machine

RETURN VALUE
  Nothing

SIDE EFFECTS
  Generates UI_TIME_F when it expires
===========================================================================*/
void ui_timer_set (
  int4  interval,                 /* interval */
  boolean repeat                  /* true if to repeat */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_MENUTIMER_SET

DESCRIPTION
  Set up for menu/list timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_menutimer_set (
  int4  interval                  /* interval */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_MSGTIMER_SET

DESCRIPTION
  Set up for message timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_msgtimer_set (
  int4  interval                  /* interval */
);


#ifdef FEATURE_NUI_D1   

void ui_msgtimer_set (
  int4  interval                  /* interval */
);

#endif

#ifdef FEATURE_UASMS
/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMSTIMER_SET

DESCRIPTION
  Set up for SMS status/warning message timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_smstimer_set (
  int4  interval                  /* interval */
);
#endif /* FEATURE_UASMS */


/* used by FEATURE_UI_ANIMATE_SEARCH */
/* <EJECT> */
/*===========================================================================
FUNCTION UI_ANIMATE_DOT_TIMER_SET

DESCRIPTION
  Set the animate dot timeout

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_animate_dot_timer_set (
  int4 interval
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_DISPMSGTIMER_SET

DESCRIPTION
  Set up for temporary message display timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_dispmsgtimer_set (
  int4  interval                  /* interval */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_ALPHATIMER_SET

DESCRIPTION
  Set up for inter digit timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_alphatimer_set (
  int4  interval                  /* interval */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_PAUSETIMER_SET

DESCRIPTION
  Set up for Timed Pause timer callback

DEPENDENCIES
  Called from state machine

SIDE EFFECTS
  sets UI_TIMERS_SIG when it expires
===========================================================================*/
void ui_pausetimer_set (
  int4  interval                  /* interval */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_BACKLIGHT

DESCRIPTION
  Handle backlight off timing

DEPENDENCIES
  ui.backval should be set correctly

RETURN VALUE
  Nothing

SIDE EFFECTS
  Sets up a clock callback

===========================================================================*/
void ui_backlight (
  boolean onoff,                  /* turn on or off */
  int time                        /* how long on - UI_BKLT_ON = always */
                                  /* UI_BKLT_ON = never. */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_BACKLIGHT_WHICH

DESCRIPTION
  Choose either ui.backlight1 or ui.backlight2 to be our backlight value
  depending on whether we are in the portable car kit.

DEPENDENCIES
  ui.backlight1, ui.backlight2, ui.onhook

RETURN VALUE
  Nothing

SIDE EFFECTS
  Changes current backlighting

===========================================================================*/
void ui_backlight_which( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UI_NEW_INFO

DESCRIPTION
  A new informational message from the base station to be displayed.  Put
  it on the queue of informational messages

DEPENDENCIES
  Can only handle text as large as we've allocated for message fields
===========================================================================*/
void ui_new_info (
  ui_name_type  cmd,              /* what command got us here */
  int           len,              /* length of message */
  char          *txt              /* text to display */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_DEBUG_SCREEN

DESCRIPTION
  Update the DEBUG screen for the mobile, which shows some interesting
  information constantly if you turn it on.

DEPENDENCIES
  ui.isdebug tells us if the debug screen should show

RETURN VALUE
  Nothing

SIDE EFFECTS
  May obscure other things on the screen

===========================================================================*/
void ui_debug_screen(
  void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SORT_NAMES

DESCRIPTION
  Sort the NV name list that we keep for ourselves - use a shell sort, which
  actually outperforms the Quicksort when there are this few entries and the
  list is almost sorted (every time but the first ).

DEPENDENCIES
  ui.nidx and ui.names have been read.  ui.names[x][0] == 127 if there is
  nothing in the name.

RETURN VALUE
  Nothing

SIDE EFFECTS
  May take a while, so we kick the watchdog.

===========================================================================*/
void ui_sort_names( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UI_KEY_UP

DESCRIPTION
  Fake a key up in case we need to play a sound or multitone even while a
  key is pressed down.

DEPENDENCIES
  It won't cause any harm if there is no key down.

RETURN VALUE
  Nothing

SIDE EFFECTS
  Stops key tone, if any.
  Sends continuous DTMF stop command to MC if needed.

===========================================================================*/
extern void ui_key_up( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UI_GET_MC_BUF

DESCRIPTION
  Get an MC buffer for passing commands to MC

RETURN VALUE
  Pointer to the buffer - NULL if none available

DEPENDENCIES
  MC should be humming along and have the buffers ready

SIDE EFFECTS
  None
===========================================================================*/
extern mc_msg_type *ui_get_mc_buf( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UI_GET_NV

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  None
===========================================================================*/
extern nv_stat_enum_type ui_get_nv(            /* in UI.C */
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UI_GET_NV_FOR_CM

DESCRIPTION
  Get an item from the nonvolatile memory.  Handles nonactive items by
  providing a default value.

  This function is defined for the purpose of supporting two UI callback
  functions: get_call_type, call_is_allowed that are used by the CM to
  determine the call type and whether the call is allowed. This function
  must be completed in a specified time as defined by the CM API.
  
  This is an example for using the call back functions.
  
RETURN VALUE
  The NV return code, except for NV_NOTACTIVE_S, which is handled
  internally.

DEPENDENCIES
  None
===========================================================================*/
extern nv_stat_enum_type ui_get_nv_for_cm(            /* in UI.C */
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_PUT_NV

DESCRIPTION
  Write an item to NV memory.  Wait till write is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  NV_TASK should be running
===========================================================================*/
extern nv_stat_enum_type ui_put_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_FREE_NV

DESCRIPTION
  Free an item to NV memory.  Wait till free is completed.

RETURN VALUE
  The NV Return Code

DEPENDENCIES
  NV_TASK should be running
===========================================================================*/
extern nv_stat_enum_type ui_free_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_NUMBER_MATCH

DESCRIPTION
  Attempt to find a number match in the speed dial memory of the
  input number.
  -Secret numbers are never part of the match search.
  -Anything after and including a Pause is not matched.
  -All manual hyphens are ignored.
  -If an exact match of the remaining number is not found and the
   number is greater than 7 digits, attempt to find a match with
   only the last 7 digits.

DEPENDENCIES
  ui.isdial must have been initialized.

SIDE EFFECTS
  None
===========================================================================*/
boolean ui_number_match (
  ui_edit_type *number,
  char *name
);

/* <EJECT> */
/*===========================================================================

FUNCTION UI_INITIALIZE_IMSI_COMPONENTS

DESCRIPTION
  This function computes the default for all IMSI components, and then
  writes them to NV. ACCOLC is also drived from the last digit of the
  IMSI.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ui_initialize_imsi_components ( byte nam ) ;


#ifdef FEATURE_IS95B_TRUE_IMSI

/*===========================================================================
        
FUNCTION UI_INITIALIZE_IMSI_T_COMPONENTS

DESCRIPTION
  This function computes the default for all TRUE IMSI components, and then
  writes them to NV. 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ui_initialize_imsi_t_components ( byte nam );

#endif //FEATURE_IS95B_TRUE_IMSI


#ifdef FEATURE_IS95B_EXT_DISP
/* <EJECT> */
/*===========================================================================

FUNCTION UI_SETUP_CLI_CNAP

DESCRIPTION
  Setup CNAP in call info object, ui_cm_call_info.

  Only the first CM_MAX_ALPHA_TAG_CHARS characters of CNAP, before NULL
  character, are saved. Non-printable control characters are translated
  to space character.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Setup CNAP in call info object, uicmd_cm_call_info.

===========================================================================*/
extern void ui_setup_cli_cnap
(
 /* CLI string, may contain alpha or control character
 ** Not necessary NULL terminated */
 byte* chars,
 /* CLI string length, UI_MAX_DIALED_DIGITS if chars is NULL
 ** terminated */
 int2  num
 );
#endif //FEATURE_IS95B_EXT_DISP

#endif /* #ifndef UIINT_H */

