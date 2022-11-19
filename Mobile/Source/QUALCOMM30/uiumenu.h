#include "target.h"

#if defined(MMI_TREX)
#include <uinmenu.h>
#elif (MMI_TYPE == MMI_A10)
#include <uiumenu.h>
#else

#ifndef UINMENU_H
#define UINMENU_H
/*===========================================================================

         U S E R   I N T E R F A C E   M E N U   D E F I N I T I O N S

DESCRIPTION
  This header contains all menu related information

    Copyright (c) 1995-2000 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uiumenu.h_v   1.5   18 May 2000 10:28:28   gshi  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/10/00   CS      Changed FEATURE_USB to FEATURE_USB_TESTING.
05/09/00   HD      Added new SIO configuration menus.
04/18/00   CS      Added support for USB device selection.
04/11/00   CS      Added support for FEATURE_TTY.
03/16/00   CS      Removed Broadcast SMS report API support.
02/24/00   CS      Added support for FEATURE_BROADCAST_SMS.
12/03/99   VT      Merged TriMode Code.
10/27/99   HD      Added FEATURE_ANALOG_HFK support.
10/08/99   CS      Added FEATURE_AUDIO_AGC support.
08/27/99   CS      Added Lcdbacklight and Kpdbacklight menu options to control
                   LCD and Keypad backlight levels for PM1000.
07/07/99   VT      Replaced FEATURE_JSTD008 with FEATURE_CDMA_1900.
07/21/98   KAR     Added support for roam ringer menu item
05/21/98   AYK     Added keyguard support for 5GP under FEATURE_UI_KEYGUARD.
04/16/98   YM      Added support for FEATURE_DS_VOICE_AS_DATA
12/22/97   skk     Added SMS Volume menu.
11/03/97   ldg     Added prototype for function to toggle QNC (under
                   FEATURE_DS_QNC).
07/31/97   ldg     Added data services debug UI under FEATURE_DS_DEBUG_UI.
07/08/97   BK      Modified inuse_restricted array to be more modular.
06/17/97   SIU     Removed pragma statements with target name.
05/15/97   SIU     Added force call and force mode support for 800MHz targets.
03/25/97   BK      Renamed to FEATURE_UI_CALL_ANSWER_MODE
03/25/97   BK      Implemented FEATURE_CALL_ANSWER_MODE
03/14/97   RC      Implemented multiple ringer types. Compiled in when
                   FEATURE_MULTIPLE_RINGER_TYPES is defined.
03/12/97   jjn     Added (TG==T_MD) in conditional compile statements
                   wherever appropriate for the Module
01/29/97   BK      Added FEATURE_VIBRATOR initial version.
10/14/96   BK      FEATURE_UI_OTASP_MENU added.
08/02/96   BK      Created to implement table driven menus.

===========================================================================*/

#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "queue.h"
#include "hs.h"
#include "uiutxt.h"
#include "uiutxti.h"
#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

/* rosa 00-05-17 --> @why, what */
#ifdef FEATURE_NUI_D1 
/* use nua_menu.h */
#else

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* This is where the menu order and contents are determined. The       */
/* determination depends on which combination of UI features are       */
/* defined in the customer header file. In the future for a new menu   */
/* there may be some features turned on or off which would effect      */
/* the definitions of older menus. So when a new menu type is defined  */
/* we must pay close attention to make sure the old conditions are     */
/* still valid. If not we must modify the old conditions accordingly.  */
/* If none of the belov is defined, then QCP-800 menu is the default.  */
#if defined ( FEATURE_UI_CHECK_BILLING )
#error code not present
#elif defined ( FEATURE_UI_ENHANCED_PRIVACY_SET )
#error code not present
#elif ( ! defined ( FEATURE_UI_FORCE_CALL ) )
  #define UI_FEATURE_MENU_002  /* Qualcomm generic PCS menu */
#endif
/* If none of the above is defined, then by default QCP-800 menu is used */


/* If this is the PCS build with menu type 2 and there's only one NAM,
   then there's no system menus. */
#if (defined (FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
  #undef UI_FEATURE_NO_SYSTEM_MENU
#else
  #ifdef FEATURE_CDMA_1900
    #if ( NV_MAX_NAMS == 1 )
      #define UI_FEATURE_NO_SYSTEM_MENU
    #endif
  #endif
#endif /* (defined (FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) ) */

/****************************************************************************
  Type that contains menu table summary info. That is, the menu header line
  text display string, the number of entries for the menu tier, and a
  pointer to the menu entries array.

  Each menu tier will contain 1 of these types (and an array of menu entries)
****************************************************************************/

typedef struct
{
  byte menu_hdr[UI_WIDE + 1] ; /* header text string (top line)                 */
  void *menu_data_ptr;     /* pointer to the menu entries array, this is a  */
                           /* void but should be a 'uimenu_entry_type'      */
  byte menu_entry_siz;     /* the number of menu selections for the tier    */

} uimenu_tier_type ;


/****************************************************************************
  Type to contain the info needed for each menu entry of a menu tier. Each
  menu tier will have an array of these types.
****************************************************************************/

typedef struct
{

  byte *entries;  /* entry text string for display */
  boolean menu_flag; /* indicates whether the union contains the next */
                     /* menu or an action to invoke. TRUE = next_menu */
  void (*action_func_ptr)(void) ;    /* function pointer to invoke if */
                                       /* at end of menu tree           */
  uimenu_tier_type *next_menu_ptr ;  /* points to next menu down      */
} uimenu_entry_type ;

/* Defined Menus */
extern uimenu_entry_type   const volume_menu[];           /* Volume sub menu     */
extern uimenu_entry_type   const display_menu[];          /* Display sub menu    */
extern uimenu_entry_type   const alerts_menu[];           /* Alerts sub menu     */
extern uimenu_entry_type   const system_menu[];           /* System sub menu     */
#ifdef FEATURE_UI_FORCE_CALL
extern uimenu_entry_type   const system_mode_sel_menu[];   /* System sub menu with force mode support */
#endif /* FEATURE_UI_FORCE_CALL */
extern uimenu_entry_type   const security_menu[];         /* Security sub menu   */
extern uimenu_entry_type   const features_menu[];         /* Features sub menu   */
extern uimenu_entry_type   const debug_menu[];            /* Debug sub menu      */
extern uimenu_entry_type   const main_menu[] ;            /* Main Menu           */

#if (defined(FEATURE_DS_SOCKETS_TEST_TCP) || defined(FEATURE_DS_SOCKETS_TEST_UDP))
#error code not present
#endif /* FEATURE_DS_SOCKETS_TEST_TCP || FEATURE_DS_SOCKETS_TEST_UDP */

/* Defined Tiers */
extern uimenu_tier_type
   volume_menu_tier ,           /* Volume sub menu tier     */
   display_menu_tier ,          /* Display sub menu tier    */
   alerts_menu_tier ,           /* Alerts sub menu tier     */
#ifndef UI_FEATURE_NO_SYSTEM_MENU
   system_menu_tier ,           /* System sub menu tier     */
#endif
#ifdef FEATURE_UI_FORCE_CALL  
   system_mode_sel_menu_tier ,   /* System sub menu with force mode support */
#endif /* FEATURE_UI_FORCE_CALL */
   security_menu_tier ,         /* Security sub menu tier   */
   features_menu_tier ,         /* Features sub menu tier   */
   debug_menu_tier ,            /* Debug sub menu tier      */
#if defined( FEATURE_SDEVMAP_UI_MENU ) || defined(FEATURE_DS_UI_BAUD) || defined (FEATURE_DIAG_UI_BAUD)
   sio_config_menu_tier ,       /* SIO config setting sub menu tier */
#endif
   main_menu_tier ;             /* Main Menu tier           */

#if (defined(FEATURE_DS_SOCKETS_TEST_TCP) || defined(FEATURE_DS_SOCKETS_TEST_UDP))
#error code not present
#endif /* FEATURE_DS_SOCKETS_TEST_TCP || FEATURE_DS_SOCKETS_TEST_UDP */

/* the keys from main menu that are to be restricted in call */
extern const boolean inuse_restricted[] ;


/**************************************************************************
******        Action Functions tied to the menu definitions        *******
**************************************************************************/

#ifdef FEATURE_UI_CHECK_BILLING
#error code not present
#endif

#ifdef NEW_SOUND_DEBUG
/* <EJECT> */
/*===========================================================================
FUNCTION NEW_SOUND_TEST

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates new sound testing functionality.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void new_sound_test ( void ) ;
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION RINGER_VOLUME_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  ringer volume options is activated.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ringer_volume_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION KEYBEEP_VOLUME_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  keybeep volume options is activated.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void keybeep_volume_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION EARPIECE_VOLUME_SETTING

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  ringer volume options is activated.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void earpiece_volume_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION MESSAGE_VOLUME_SETTING

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  message volume options is activated.
  SMS Volume
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void message_volume_settings ( void ) ;

#ifdef FEATURE_PM1000
#error code not present
#endif /* FEATURE_PM1000 */

#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */

#if ( NV_MAX_NAMS != 1 )
/* <EJECT> */
/*===========================================================================
FUNCTION AUTONAM_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  autonam is activated.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/

void autonam_list_settings ( void ) ;
#endif

#ifdef FEATURE_UI_OTASP_MENU
/* <EJECT> */
/*===========================================================================
FUNCTION ACTIVATION_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It enables the user to select from a list to start OTASP
  activation on a specific block.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/

void activation_list_settings ( void ) ;
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION LOCKMODE_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  lock mode options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void lockmode_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION RESTRICT_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  restrict options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void restrict_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION CLEARCALLS_LIST_SETINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  clear calls options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void clearcalls_list_settings ( void ) ;

#ifdef FEATURE_UI_SEND_PIN

/* <EJECT> */
/*===========================================================================
FUNCTION SENDPIN_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  send-pin options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void sendPIN_list_settings ( void ) ;
#endif


/* <EJECT> */
/*===========================================================================
FUNCTION AUTOANSWER_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  auto-answer options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void autoanswer_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION AUTORETRY_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  ringer volume options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void autoretry_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION KEYBEEP_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  keybeep options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void keybeep_list_settings ( void ) ;

#ifdef FEATURE_UI_KEYGUARD
#error code not present
#endif



/* <EJECT> */
/*===========================================================================
FUNCTION ONETOUCH_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  onetouch options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void onetouch_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION PRIVACY_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  voice privacy options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void privacy_list_settings ( void ) ;

#ifdef FEATURE_MULTIPLE_RINGER_TYPES
#error code not present
#endif /* FEATURE_MULTIPLE_RINGER_TYPES */

/* <EJECT> */
/*===========================================================================
FUNCTION SHOWTIME_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables to activate time display
  list setting

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void showtime_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION AUTOHYPHEN_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables to activate list selection
  for autohyphen options.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void autohyphen_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION FADE_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It sets some global variables to activate list selection
  for fade alert options.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/

void fade_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION MINUTE_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It sets some global variables to activate list selection
  for minute alert options.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void minute_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION SERVICE_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It sets some global variables to activate list selection
  for service alert options.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void service_list_settings ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ROAM_RINGER_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It sets some global variables to activate list selection
  for roam ringer options.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void roam_ringer_list_settings ( void ) ;

#ifdef FEATURE_UI_FORCE_CALL
/* <EJECT> */
/*===========================================================================
FUNCTION FORCECALL_LIST_SEETINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  forcecall options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void forcecall_list_settings ( void ) ;
#endif

#ifdef FEATURE_SDEVMAP_UI_MENU
/* <EJECT> */
/*===========================================================================
FUNCTION PORTMAP_SEETINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  forcecall options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void portmap_settings ( void ) ;
#endif

#ifdef FEATURE_DS_UI_BAUD
/* <EJECT> */
/*===========================================================================
FUNCTION DS_BAUD_SEETINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  forcecall options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ds_baud_settings ( void ) ;
#endif

#ifdef FEATURE_DIAG_UI_BAUD
/* <EJECT> */
/*===========================================================================
FUNCTION DIAG_BAUD_SEETINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Sets some global variables so that list selection for
  forcecall options is activated.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void diag_baud_settings ( void ) ;
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION LAST_CALL_INFO

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It displays last call info.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void last_call_info ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION HOME_CALLS_INFO

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It displays number of calls made and the total time of
  those calls since the last reset of these values. These calls are only
  of those made in the home system.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/

void home_calls_info ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ROAM_CALLS_INFO

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It displays number of calls made and the total time of
  those calls since the last reset of these values. These calls are of
  those made in outside the home system (i.e roaming calls).

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void roam_calls_info ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ALL_CALLS_INFO

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It displays number of calls made and the total time of
  those calls since the manufacturing. These are home calls and roaming
  calls, and are not reset.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void all_calls_info ( void ) ;

/* <EJECT> */
#ifdef FEATURE_DS_DEBUG_UI
/*===========================================================================
FUNCTION TOGGLE_DATA_SCREEN

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It toggles the data debug screen display on and off.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void toggle_data_screen ( void ) ;
#endif /* FEATURE_DS_DEBUG_UI */

/* <EJECT> */
#ifdef FEATURE_DS_QNC
/*===========================================================================
FUNCTION TOGGLE_QNC

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It toggles Quick Net Connect services on or off.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void toggle_qnc ( void ) ;
#endif /* FEATURE_DS_QNC */

#ifdef FEATURE_USB_TESTING
#error code not present
#endif /* FEATURE_USB_TESTING */

#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif /* FEATURE_ANALOG_HFK */

/* <EJECT> */

#ifdef FEATURE_DS_SOCKETS_TEST_TCP
/*===========================================================================
FUNCTION    ui_tcp_test_start

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  start running the series of tests.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_start(void);

/*===========================================================================
FUNCTION    ui_tcp_test_manual

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  start running the manual test setup.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_manual(void);

/*===========================================================================
FUNCTION    ui_tcp_test_pppclose

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  close ppp.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_pppclose(void);

/*===========================================================================
FUNCTION    ui_tcp_test_pppopen

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  open ppp.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_pppopen(void);

/*===========================================================================
FUNCTION    ui_tcp_test_tcpclose

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  close the tcp connection

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_tcpclose(void);

/*===========================================================================
FUNCTION    ui_tcp_test_connect

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  connect the tcp socket.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_connect(void);

/*===========================================================================
FUNCTION    ui_tcp_test_write

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  do a write

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_write(void);

/*===========================================================================
FUNCTION    ui_tcp_test_close_netlib

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  close the netlib.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_tcp_test_close_netlib(void);

#endif

#ifdef FEATURE_DS_SOCKETS_TEST_UDP

/*===========================================================================
FUNCTION  ui_udp_test_start

DESCRIPTION
  This function sends a message to the udp test task, telling it to 
  start running the series of tests.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_test_start(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_test_start

DESCRIPTION 
  This function sends a message to the udp test task, telling it to
  start running the series of tests.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_test_start(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_ppp_open

DESCRIPTION 
  This function sends a message to the tcp test task, telling it to
  start running the series of tests.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_ppp_open(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_write

DESCRIPTION 
  This function sends a message to the udp test task, telling it to
  write something server.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_write(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_open

DESCRIPTION 
  This function sends a message to the udp test task, telling it to
  open a socket.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_open(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_close

DESCRIPTION 
  This function sends a message to the udp test task, telling it to
  close a socket.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_close(void);

/*===========================================================================
FUNCTION    ui_udp_dormant_ppp_close

DESCRIPTION 
  This function sends a message to the udp test task, telling it to
  close ppp.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_udp_dormant_ppp_close(void);
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION GET_NEW_CODE

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates lock code modification mechanism. It is also
  called from the NEWLOCK_S state, for the re-entry of the lock code.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/

void get_new_code ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION EMERGENCY_NUMBERS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates emergency number edit/display functionality.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void emergency_numbers ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION activate_scratchpad

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates scratchpad functionality.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void activate_scratchpad ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION MUTE_MODE_TOGGLE

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It mutes the phone.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void mute_mode_toggle ( void ) ;

#if defined(FEATURE_CDMA_800)
/* <EJECT> */
/*===========================================================================
FUNCTION SELECT_PREFERRED_SETTING

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates list selection for system settings.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void select_preferred_setting ( void ) ;
#endif /* if (defined (FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) ) */


#if ( NV_MAX_NAMS != 1 )
/* <EJECT> */
/*===========================================================================
FUNCTION SET_NAM

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. Activates list selection to choose a NAM.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void set_nam ( void ) ;
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION ACTIVATE_SERVICE_PROGRAMMING

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void activate_service_programming ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION BACKLIGHT_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. This function sets some global variables so that list
  selection for backlight settings will take place.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void backlight_list_settings ( void ) ;

/* <EJECT> */
#ifdef FEATURE_DS_VOICE_AS_DATA
/*===========================================================================
FUNCTION DATACALL_LIST_SETTINGS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It will ask user to select a choice to determine
  answer_voice_as_data attributed.

DEPENDENCIES
  State machine

SIDE EFFECTS
  State machine is changed 
===========================================================================*/
void datacall_list_settings ( void );

#endif
/* <EJECT> */
/*===========================================================================
FUNCTION BANNER_DISPLAY_EDIT

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates banner display/edit functionality.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void banner_display_edit  ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION DISPLAY_PHONE_VERSION

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It displays the version of the phone

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void display_phone_version ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION PASSWORD_ENTRY_FUNCTION

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates lock code entry mechanism, that is required
  before entering the SECURITY sub menu.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void password_entry_function ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION FIELD_SERVICE_ENTRY

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates field service entry mechanism, that is required
  before entering the DEBUG sub menu.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void field_service_entry ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION

DESCRIPTION TOGGLE_DEBUG_SCREEN
  This function is tied to a menu item and executed upon the selection
  of that item. It toggles the debug screen display on and off.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void toggle_debug_screen ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ACTIVATE_TEST_CALL

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates the screen to choose a test call in.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void activate_test_call ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ENABLE_CDMA_ONLY

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It puts the phone in CDMA only mode.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void enable_CDMA_only ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION SHOW_ERRORS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It toggles the error screen on and off.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void show_errors ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION CLEAR_ERRORS

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item. It activates the list selection to for clearing errors.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void clear_errors ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION TOGGLE_13K

DESCRIPTION
  This function is tied to a menu item and executed upon the selection
  of that item.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void toggle_13K ( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION ACTIVATE_MAIN_MENU

DESCRIPTION
  Activates the main menu by setting the pointers. This function is called
  from idle state or call state when the menu smart key is pressed.

DEPENDENCIES
  none

SIDE EFFECTS
  None
===========================================================================*/
void ui_activate_main_menu (void) ;


/* <EJECT> */
/*===========================================================================
FUNCTION uinsmenu_display_msg

DESCRIPTION
  Requests the UI to display a text message on the screen for a specified time.

DEPENDENCIES
  None

SIDE EFFECTS
  Change the message id and UI's states.
===========================================================================*/
void uinsmenu_display_msg
(
  ui_field_item_enum_type  msg_id,
  byte                     seconds
);

#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

#ifdef FEATURE_PHONE_VR
#error code not present
#endif /* FEATURE_PHONE_VR */

#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS

/*===========================================================================

FUNCTION    bcsms_user_pref
DESCRIPTION
  Allow the user to view and set Broadcast SMS service user preference.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_user_pref (void);


/*===========================================================================

FUNCTION    bcsms_get_all_service_ids

DESCRIPTION
  Get all the BC SMS service IDs.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_get_all_service_ids(void);

/*===========================================================================

FUNCTION    bcsms_get_service_info

DESCRIPTION
  Get a service info.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_get_service_info(void);

/*===========================================================================

FUNCTION    bcsms_srvc_select
DESCRIPTION
  Allow the user to select a service options: activate all services,
  activate services in the table, and deactive.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_srvc_select(void);

/*===========================================================================

FUNCTION    bcsms_srvc_priority
DESCRIPTION
  Set the service priority.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_srvc_priority(void);

/*===========================================================================

FUNCTION    bcsms_srvc_delete
DESCRIPTION
  Delete a service.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_srvc_delete(void);

/*===========================================================================

FUNCTION    bcsms_srvc_change_label

DESCRIPTION
  change a service label.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_srvc_change_label(void);

/*===========================================================================

FUNCTION    bcsms_srvc_add
DESCRIPTION
  Add a service.
    
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void bcsms_srvc_add(void);

#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */
/* <EJECT> */

/* <EJECT> */
#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */



#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===============================================================================

                                   EQUS UI Variables
                                        ( Menu )
=================================================================================*/
/*
**--------------------------------------------------------------------------------
**  Type Definitions
**--------------------------------------------------------------------------------
*/
#if 0
typedef struct tagMENU    eqsMenu;


/*
**--------------------------------------------------------------------------------
**  General Definitions
**--------------------------------------------------------------------------------
*/
struct tagMENU{
	byte      CurrentKey;     //ui.menu_curkey
	byte      PreviousKey;    //ui.menu_prevkey
};
#endif
/*
**--------------------------------------------------------------------------------
**  Variable Declarations
**--------------------------------------------------------------------------------
*/
//extern eqsMenu      gMenu;        //defined in uismenu.c
#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)


#endif /* FEATURE_NUI_D1 */
/* <-- */

#endif /* UINMENU_H */

#endif // #if defined (MMI_TREX)