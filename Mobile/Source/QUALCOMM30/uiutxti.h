#include "target.h"

#if defined(MMI_TREX)
#include <uintxti.h>
#elif (MMI_TYPE == MMI_A10)
#include <uiutxti.h>
#else

#ifndef UITXTGI_H
#define UITXTGI_H
/*===========================================================================

           U S E R   I N T E R F A C E   F I E L D  I T E M S
                        A R R A Y  I N D E X E S
                        G E M I N I  T A R G E T 

DESCRIPTION
  This file defines the enumerated ui_field_type array indexes for the Gemini
  target. These enumerated items are used by UI application code to identify
  specific ui_field_type items.

  Copyright (c) 1991-1998 by QUALCOMM INCORPORATED.
  All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uiutxti.h_v   1.4   18 May 2000 10:34:14   gshi  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/09/00   HD      Added strings for new SIO configuration menus.
01/28/00   HQ      Added HFK support - wake-up training, voice activation,
                   incoming call voice answer, and expert window.
10/27/99   HD      Added support for Analog HFK.
07/17/98   JGR     Remove a old screen from Net Mode all changes are under
                   FEATURE_DS_UP
07/01/98   YM      Added support for FEATURE_VOICE_AS_DATA, silence ringer
                   and phone lock message
06/05/98   YM      Added support for one touch mute/unmute
05/21/98   AYK     Added keyguard support for 5GP under FEATURE_UI_KEYGUARD.
02/14/98   AYK     Added DTACO support under FEATURE_UI_DTACO.
01/28/98   YM      Enhanced prompt while recalling phonebooks - CR4331
01/26/98   YM      Added support for extended version display
01/14/98   YM      Enchance "Feature Unavailable" display - CR4523, 4645
12/23/97   SIU     Caller ID/Call history enhancement.
12/16/97   skk     Added SMS option 6 capability.
07/31/97   ldg     Added data services debug UI under FEATURE_DS_DEBUG_UI.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
05/15/97   SIU     Added force call and force mode support for 800MHz targets.
04/22/97   BK      Don't blink the caller id in the incoming call display.
02/27/97   RC      Implemented "Searching..." indication when there is no svc.
02/07/97   RC      Change to Copyright (c) 1991,1992,1993,1994,1995,1996,1997.
01/27/97   RC      Added UI_CANNOT_ANSWER_DC_F, "Can not answer data call
                   "from keypad" screen for data calls.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
10/16/96   BK      Removed UI_PAUSED1_F, UI_PAUSED2_F screens.
09/13/96   BK      Added FEATURE_OTASP.
08/08/96   CAH     One NAM support.
08/02/96   BK      Implemented table driven menus.
04/18/96   BK      New UI branching created from uitxtgi.h ver 1.5
03/31/96   CAH     Rid unused UI_ALPHA_F.
03/18/96   BK      FEATURE_UI_SHOW_DROP_CALL is added.
03/14/96   BK      Added ifdefs for customer specific builds.
03/11/96   JGR     Removed UI_ROAMTIME_F not used anywhere.
03/11/96   BK      Added UI_PWR_KEY_INFO_F for QCP-1900
09/24/95   jjw     created

===========================================================================*/
#include "target.h"
#include "customer.h"

#include "nv.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
/*--------------------------------------------------------------------------- 
  Indexes into Screen Array items. NOTE: must be ordered identically to 
  screen array items in UIUTXT.C
---------------------------------------------------------------------------*/

typedef enum  
{
      UI_BANNER_F = 0,            /* own name display */
#ifdef FEATURE_UI_SHOW_SEARCHING
#error code not present
#endif /* FEATURE_UI_SHOW_SEARCHING */
      UI_NAM_F,                   /* current NAM name display */
      UI_NAM_INFO_F,              /* current NAM name display via Info */
      UI_NUMBER_F,                /* own (cdma if portable) number display */
      UI_ERASED_F,                /* erased speed dial number confirmation */
      UI_LOCKCODE_F,              /* Enter lock code display */
      UI_BADCODE_F,               /* Incorect lock code display */
      UI_LOCK_WARN_F,             /* Earpiece down keypad locked msg */
      UI_RESTRICTED_F,            /* Partial or full Call restriction enabled */
      UI_NO_ONETOUCH_F,           /* One Touch dialing disabled */
#ifdef FEATURE_UI_PWR_KEY_ALT
      UI_PWR_KEY_INFO_F,          /* pwr key press displays pwr down info */
#endif
      UI_MISSED_CALL_F,           /* Prompt user about the calls missed    */
      UI_HELP_F,                  /* Help displays */
      UI_SOFTKEYS_F,              /* Gemini Soft Keys */
      UI_SPEED_F,                 /* Speed dial number display */
      UI_MEM_LIST_F,              /* Speed dial recall list display */
      UI_DIAL_NUM_F,              /* dialing number */
      UI_DIAL_ALPHA_F,            /* dialing name */
#ifdef FEATURE_OTASP
      UI_OTASP_F,                 /* Various OTASP display messages */
#endif
#ifdef FEATURE_UI_SEND_PIN
      UI_SEND_PIN_F,              /* Send PIN code prompt */
#endif
      UI_AKEY_F,                  /* AKEY entry */
      UI_SRCH_AEDIT_F,            /* alpha search edit */
      UI_ASRCH_TITLE_F,           /* alpha search title */
      UI_SECRET_F,                /* secret number stored */
      UI_VERIFY_F,                /* verify memory storage */
      UI_STATUS_F,                /* give memory status */
      UI_DECADE_NONE_F,           /* display a empty decade message */
      UI_AMATCH_NONE_F,           /* no alpha memory matches */
      UI_SERVICE_F,               /* service needed */
      UI_MAINT_REQ_F,             /* Maintenance required displayed */
      UI_LOWPOWER_F,              /* Low Power deep sleep displayed */
      UI_BADNAM_F,                /* NAM with no analog, no digital */
      UI_CALL_F,                  /* in paged call indicator */
#ifdef FEATURE_OTASP
      UI_SEARCH_F,                /* in otasp call display */
#endif
      UI_MARKOV1_F,               /* in markov call indicator */
      UI_MARKOV2_F,
      UI_DEBUG1_F,                /* debug screen */
      UI_DEBUG2_F,                /* debug screen */
#ifdef FEATURE_DS_DEBUG_UI
      UI_DEBUG3_F,                 /* extra debug screen for data services */
#endif /* FEATURE_DS_DEBUG_UI */
      UI_NOSVC_F,                 /* no service */
      UI_DATA0_F,                 /* data call */
      UI_DATA1_F,                 /* data call status */
      UI_DATA2_F,                 /* data call status */
      UI_CANNOT_ANSWER_DC_F,      /* Can not answer data call from keypad */
#ifdef FEATURE_UI_KEYGUARD
#error code not present
#endif
#ifdef FEATURE_DS_UP
#error code not present
#endif
      UI_LOOP1_F,                 /* loopback call */
      UI_LOOP2_F,                 /* loop back call 2nd line */
      UI_E911_SEARCH1_F,          /* Looking for Emerbency Service ... */
      UI_E911_SEARCH2_F,          /* Continuing Emergency Search ... */
      UI_E911_LOST_F,             /* Emergency service lost Looking... */
      UI_E911_SEARCH_TIME_F,      /* Time: X:XX */
      UI_E911_ONLY_F,             /* Emergency calls only. END to exit. */
      UI_E911_RESTRICT_F,         /* Use is restricted. Emergency calls only. */
      UI_E911_TIMEDATE_F,         /* current time */
      UI_E911_END_TO_EXIT_F,      /* END to exit. */
      UI_NUM_F,                   /* edit number */
      UI_AEDIT_F,                 /* alpha edit */
      UI_TIME_F,                  /* call time */
#ifdef FEATURE_UI_SHOW_DROP_CALL
      UI_LOST_CALL_F,             /* lost call indicator */
#endif
      UI_MUTE_F,                  /* in mute */
      UI_S_MUTE_F,                /* speaker mute */

#ifdef FEATURE_UI_ONE_TOUCH_MUTE
#error code not present
#endif

      UI_PAD_F,                   /* in silent scratch pad */
      UI_NUMEDIT_F,               /* editing number */
      UI_CALLINFO_F,              /* display for call info items */
      UI_BANNERSHOW_F,            /* show banner */
      UI_SETLBL_F,                /* setup text */
      UI_SETVAL_F,                /* setup value */
      UI_VER_DISP_F,              /* version display */
      UI_SERV1_F,                 /* service programming */
      UI_SERV2_F,
      UI_SERV3_F, 
      UI_SERV4_F, 
      UI_LSTLBL_F,                /* list text */
      UI_LSTVAL_F,                /* list value */
      UI_LSTVAL2_F,
      UI_HOMESYS_F,               /* "Home is _ " */
      UI_ERRNAME_F,               /* show errors from NV */
      UI_ERRNUM_F, 
      UI_ERRFATAL_F, 
      UI_ERRLINE_F,
      UI_ERRCNT_F, 
      UI_CURSOR_F,                /* cursor */
      UI_PUNC_UP_F,               /* first display for punctuation chars */
      UI_PUNC_DOWN_F,             /* second display for punctuation chars */
      UI_PAUSE_DISP_F,            /* display for pause and dash characters */
      UI_RCL_F,                   /* recall number */
      UI_RCL_PROMPT_F,            /* recall prompt */
      UI_STO_F,                   /* store number */
      UI_ADDR_F,                  /* address for RCL/STO */
      UI_OVER_F,                  /* prompt to overwrite memory */
#if ( NV_MAX_NAMS != 1 )
      UI_MIN1_F,                  /* min/nam choice */
#endif /* NV_MAX_NAMS != 1 */
      UI_MIN2_F, 
      UI_BADNAM2_F, 
      UI_MSG_EMPTY_F,             /* display a "empty number" message */
      UI_MSG_INVALID_ADDR_F,      /* display a "Invalid Location" message */
      UI_MSG_NONE_F,              /* no saved memory locations */
      UI_MSG_NOST_F,              /* no numbers on the save stack */
      UI_MATCH_LOOK_F,            /* searching memory */
      UI_MATCH_NONE_F,            /* no memory matches */
      UI_MSG_NFREE_F,             /* no free numbers for storage */
      UI_MSG_ERRSVC_F,            /* error in service programming */
      UI_MSG_RSTRCT_F,            /* this phone is restricted */
#ifdef FEATURE_DS
      UI_MSG_LOCK_F,              /* the phone is locked */
#endif
      UI_MSG_SILENCE_RING_F,      /* ringer is silenced */
#ifdef FEATURE_IS95B_INC_CALL_FWD_ENH
      UI_MSG_CALL_FORWARD_F,      /* call is forwarded  */
#endif
      UI_CALLFAIL_F,              /* call failed */

      UI_CALL_TEMP_FAIL_F,        /* call failed temporarily */

      UI_RETRY_F,                 /* retrying call */
      UI_MENU_F,                  /* menu display */
      UI_MENUB_F,                 /* menu blink character */
      UI_FEAT_UNAVAIL_IN_CALL_F,    /* feature unavailable in call */
      UI_FEAT_UNAVAIL_IN_STDBY_F,   /* feature only available in call */

#ifdef FEATURE_UI_FORCE_CALL
       UI_INVALID_MODE_SEL_F,      /* invalid mode selection */
#endif /* FEATURE_UI_FORCE_CALL */

      UI_VOICE_AS_DATA_OFF_F,      /* voice as data off message */
      UI_VOICE_AS_DATA_ON_F,       /* voice as data on message */
      UI_VOICE_AS_DATA_BANNER_F,   /* voice as data banner     */
      UI_CODES_NOT_MATCH_F,       /* used when entering new security code*/
      UI_CODE_CHANGED_F,           /* Display new security code */
      UI_LOWBATT_F,               /* One minute warning */
      UI_PREPENDT_F,              /* Prepend title */
#ifdef FEATURE_UASMS
      UI_SMS_M_F,                   /* SMS message/menu display */
      UI_SMS_NOTIF_F,             /* SMS notification */
      UI_SMS_STS_F,               /* SMS status/warning - timed msg */
      UI_SMS_NOTIF_SMS6_F,        /* SMS Option 6 Receiving Message Please wait - SMS6 */
#endif /* FEATURE_UASMS */
      UI_13K_F,                   /* 13K selection confirmation */
      UI_INFO_MSG_F,              /* informational message display */
      UI_VIEW_F,                  /* view number */
      UI_SLEAVE_F,                /* leaving service area */
      UI_SENTER_F,                /* entering service area */
      UI_SCHANGE_F,               /* changing service area */
      UI_MSG_ALERT_F,             /* incoming call */
      UI_MSG_ALERT2_F,            /* call from */
      UI_CLI_ALERT_F,             /* incoming call caller id */
      UI_BADBATT_F,               /* bad battery */
      UI_MSG_SMS_SENT_F,          /* SMS msg sent */
      UI_MSG_SMS_STATUS_F,        /* SMS status */
      UI_MSG_SMS_CLEAR_F,         /* clear all SMS statuses */
      UI_MSG_SMS_AUTO_D_ON_F,     /* enable auto-disconnect */
      UI_MSG_SMS_AUTO_D_OFF_F,    /* disable auto-disconnect */
      UI_MSG_SMS_RESET_SEQ_NUM_F, /* reset sequence number */
      UI_MSG_TEST_DUMMY_F,        /* place holder for test menu */
      UI_CM_CMD_SENT_F,           /* CM test cmd sent */
#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

#ifdef FEATURE_DS
      UI_CALLFAIL_NO_DATA_F,      /* Data Service not available */
#endif      
#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif /* FEATURE_ANALOG_HFK */
#if defined ( FEATURE_DS_UI_BAUD ) || defined (FEATURE_DIAG_UI_BAUD ) || defined (FEATURE_SDEVMAP_UI_MENU)
      UI_MSG_NEED_RESET_F,        /* Need reset to take effect */
      UI_MSG_CONF_CHANGED_F,      /* Configuration took effect */
#endif      
      UI_DUMMY_F                  /* to end it all */
} ui_field_item_enum_type;


#endif

#endif // #if defined (MMI_TREX)