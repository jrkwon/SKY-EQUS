#include "target.h"

#if defined(MMI_TREX)
#include <uintxt.h>
#elif (MMI_TYPE == MMI_A10)
#include <uiutxt.h>
#else

#ifndef UITXT_H
#define UITXT_H
/*===========================================================================

           U S E R   I N T E R F A C E   M O B I L E   T E X T

DESCRIPTION
  This gives the format of mobile screen text fields, menus, and list items,
  as well as a list of all defined fields, menus, and lists.

    Copyright (c) 1991 through 2000 by QUALCOMM INCORPORATED All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uiutxt.h_v   1.4   21 Jun 2000 17:37:04   hqu  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/21/00   HQ      Under FEATURE_ODIE_FONT for FFA builds, define different
                   values for ELLIPSE and TRIANGLE characters.
05/10/00   CS      Changed FEATURE_USB to FEATURE_USB_TESTING.
05/10/00   HQ      Changed EMERGENCY_NUMBERS from 3 to 10 (from 100 to 109).
04/18/00   CS      Added support for USB device selection.
04/11/00   CS      Added support for FEATURE_TTY.
03/16/00   CS      Removed Broadcast SMS report API support.
02/23/00   CS      Added support for FEATURE_BROADCAST_SMS.
10/27/99   HD      Added support for FEATURE_ANALOG_HFK.
10/26/99   VT      Defined ui_voice_so_l for Voice SO selection.
10/08/99   CS      Added FEATURE_AUDIO_AGC support.
08/27/99   CS      Added Lcdbacklight and Kpdbacklight menu options to control
                   LCD and Keypad backlight levels for PM1000.
07/07/99   VT      Replaced FEATURE_JSTD008 with FEATURE_CDMA_1900.
07/21/98   KAR     Added support for roam ringer menu.
07/17/98   JGR     Added support for Net Mode exit all changes under
                   FEATURE_DS_UP
07/01/98   YM      Added support for FEATURE_DS_VOICE_AS_DATA
06/08/98   YM      Changed TRIANGLE definition for FEATURE_ODIE_FONT
05/29/98   ILO     Changed soft key from ok zero to OK RESET per spec 10.2.3.2
05/21/98   AYK     Added keyguard support for 5GP under FEATURE_UI_KEYGUARD.
03/26/98   YM      Added strings for scratch pad entry in call history-CR5050
03/04/98   KAR     Added support for Seven Level Earpiece vol.
02/05/98   KAR     Added headset support.
01/27/98   YM      Added support for date and duration for call history
01/13/98   YM      Add support for ESN display in version menu - CR4522
12/23/97   ces     Add new FEATURE_UI_SEVEN_LEVEL_SND.
12/22/97   skk     Added SMS Volume menu.
12/16/97   skk     Added SMS option 6 capability.
11/03/97   ldg     Added QNC support (under FEATURE_DS_QNC).
07/31/97   ldg     Added data services debug UI under FEATURE_DS_DEBUG_UI.
07/09/97   fkm     FEATURE_xxx Updates (via Automatic Scripts)
06/17/97   SIU     Removed pragma statements with target name. Added some
                   character and string definitions.
05/30/97   BK      Moved AKEY entry to Service Programming
05/21/97   BK      Changed pause characters back to their original defines.
05/15/97   SIU     Added force call and force mode support for 800MHz targets.
04/25/97   BK      Changed definition for UI_PSE_CHR and UI_TPSE_CHR
03/25/97   BK      Renamed to FEATURE_UI_CALL_ANSWER_MODE
03/25/97   BK      Implemented FEATURE_CALL_ANSWER_MODE
03/14/97   RC      Implemented multiple ringer types. Compiled in when
                   FEATURE_MULTIPLE_RINGER_TYPES is defined.
03/12/97   jjn     Added (TG==T_MD) in conditional compile statements
                   wherever appropriate for the Module
01/29/97   BK      Added FEATURE_VIBRATOR initial version.
01/27/97   RC      Added service option texts for data calls.
01/09/97   BK      New UI to SOUND interface modifications.
12/23/96   JGR     Add support for Unwired Planet with ifdef FEATURE_DS_UP
10/16/96   RC      Added text string UI_PLEASE_WAIT_TXT
10/14/96   BK      FEATURE_UI_OTASP_MENU added.
09/13/96   BK      Added FEATURE_OTASP.
09/05/96   RC      Added text string UI_EMPTY_TXT
08/13/96   CAH     Better Caller ID support with the Analog Flash w/Info.
08/09/96   bk      Added prompt for exit at the end of OTKSL use
08/08/96   CAH     One NAM support.
08/02/96   BK      Implemented table driven menus.
07/22/96   BK      Added ifdefs for FEATURE_UI_ENTER_NAME_PROMPT
07/06/96   BK      "Enter Name' prompt added for alpha edit screens.
                   Volumes text changed to 'Off' from 'Silent'
06/24/96   BK      New system selection for PCS
06/11/96   JGR     Added field to support screen handler II.
05/20/96   BK      Remove two unused menus.
04/18/96   BK      New UI branching created from uitxt.h ver 1.93
04/02/96   BK      Added FEATURE_UI_CHECK_BILLING and PCS menu ordering.
03/25/96   BK      ifdef fix for Portable and Mobile targets.
03/14/96   BK      Added ifdefs for customer specific builds.
03/05/96   BK      Replace T_PRMCO1900 with T_GP.
02/29/96   JGR     New screen handling technique
02/22/96   CAH     Alpha softkey switch from X's to A's for 1900 only.
02/20/96   CAH     CNI now supported for all cases w/ or w/o Alerting, and
                     all PI settings.
02/20/96   BK      ui_force_call_l decleration is removed form QCP1900 only.
02/19/96   CAH     Gemini case softkey now uses A's instead of X's.
02/13/96   BK      T_PRMCO1900 : Q and Z are moved from 1 to 7 and 9 in the key pad.
01/17/96   BK      -Ringer Off- text implementation.
12/27/95   BK      New Service Programming is implemented.
12/21/95   CAH     New 8k/13k service negotiation support.
12/06/95   BK      Lint cleanup.
11/29/95   CAH     Support STANDARD preference setting and delete SID ACQ list
                     and NV_SYS_SID_ONLY preference setting.
10/12/95   CAH     Add UI_SPACE define which broke non Gemini builds.
10/12/95   CAH     Switch memcpy's to memset's where possible.
10/05/95   BK      A check mark for the current selection of every list.
09/24/95   CAH     Timestamp now displayed in calls list and code optimizing.
09/23/95   CAH     Alpha search does punctuation match and code optimizing.
09/23/95   CAH     Display NAM info while in a call and INFO is pressed.
09/21/95   BK      Bug number 594 is fixed.
09/14/95   CAH     New NAM info display.
09/11/95   CAH     'PREPENDING' title.
09/08/95   CAH     Rid unused Gemini displays.
08/31/95   CAH     New version display.
08/30/95   BK      Bug 458 : Banner selection spec compliant
08/22/95   DAY     Added new softkey option for SMS UI - [ERASE][QUIT]
08/20/95   BK      New Locked mode implementation.
08/15/95   CAH     Final system menus support.
08/12/95   BK      Missed calls display, and operation.
08/14/95   DAY     Added support for SMS UI - new softkey messages and
                   new field types for SMS status/warning, notification
                   and message display.
08/12/95   BK      Displays time and day when idle if set and avail.
08/11/95   CAH     Lock/Unlock/MaintReq and deep sleep display support.
08/10/95   CAH     Gemini new debug menus and voice privacy support.
07/10/95   BK      Locked-state emergency-number-calling implemented.
07/07/95   CAH     Gemini authentication support.
07/06/95   CAH     13K support.
07/04/95   BK      Code for emergency numbers
06/27/95   CAH     Gemini low battery support.
06/20/95   CAH     Gemini originated loopback support.
06/20/95   BK      Changes needed for new GEMINI service programming.
06/19/95   CAH     Gemini default banner is now blank.
06/19/95   CAH     Gemini Send PIN code prompt support.
06/14/95   BK      Various minor additions and modifications to GEMINI UI
06/12/95   BK      GEMINI new way of entering "new code"
06/06/95   CAH     Gemini new messages
06/05/95   BK      Fixed the problem with non GEMINI builds.
06/04/95   BK      Main Menu changes for GEMINI introduced.
05/29/95   CAH     Gemini Invalid Speed Dial number support.
05/23/95   CAH     Gemini TONE support.
05/16/95   CAH     Gemini SAVE, VIEW support.  Cosmetic fixes.
05/16/95   CAH     Fix the Beta2 DS compile error.
05/01/95   CAH     Initial Gemini mods.
04/15/95   RAS     Initial ISS2 target support.
04/10/95   BK      Markov menu item is shown but results in timed-out error-
                   screen, if pressed during non-valid times.
01/06/95   CAH     Markov menu item no longer shows up in Analog
11/18/94   CAH     Data call status display
08/17/94   CAH     Display both CDMA and FM numbers at init on portable (Log 179)
07/22/94   CAH     Serial Port Lump Rate mods
07/22/94   CAH     ISS1 modifications
05/18/94   CAH     Ron's ISS0 updates as per release 56
10/13/92   rrd     comment and clean
02/05/92   rrd     created

===========================================================================*/

#include "target.h"
#include "customer.h"
#include "comdef.h"
#include "queue.h"
#include "hs.h"
#include "uiutxti.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* This is the screen width */
#define UI_WIDE HS_SCRN_WIDE

/* menu items per screen */
#define MENU_HIGH HS_SCRN_HIGH

/* pause character representation : NV uses the same */
#define UI_PSE_CHR 'P'
/* timed pause character representation : NV uses the same */
#define UI_TPSE_CHR 'T'
/* hyphen character INTERNAL representation */
#define UI_HYP_CHR '-'



#define EMERGENCY_NUMBERS 10     /* number of emergency numbers */
#define UI_NO_MARK -1           /* check mark for ui_list_type */

/* blank - for making any field blank */
#define UI_SPACE ' '
#define UI_SPACES "                                                "

/* underscore character - for making lock code entry */
#define UI_UNDERSCORE '_'

/* Arrow characters */
#define UI_UP_ARROW        0xC0 /* '¿' */
#define UI_DOWN_ARROW      0xC2 /* '¬' */
#define UI_DOUBLE_ARROW    0xC1 /* '¡' */

/* Pause Display characters */
#define HARD_PAUSE_CHAR 0xDE    /* Inverse H */
#define TIMED_PAUSE_CHAR 0xDF   /* Inverse T */


#ifdef FEATURE_ODIE_FONT /* for FFA builds */
#error code not present
#else /* FEATURE_ODIE_FONT */
  #define TRIANGLE 0x9E
  #define ELLIPSE  0xDB
#endif /* FEATURE_ODIE_FONT */

/* Force Mode Temp Analog option */
#define UI_LIST_TEMP_ANALOG ((int)NV_MODE_ANALOG_ONLY + 0x10)



/* ROMmed pointers for constant strings */
extern const byte ui_sms6_notif_txt[]; /* SMS6 */
extern const byte ui_secret_txt_rstr[];
extern const byte ui_emergency_txt_rstr[];
extern const byte ui_newcode_txt_rstr[];
extern const byte ui_newcode_again_txt_rstr[];
extern const byte ui_nmsg_txt_rstr[];
extern const byte ui_mwn_txt_rstr[];
extern const byte ui_nvoice1_txt_rstr[];
extern const byte ui_nvoice2_txt_rstr[];
extern const byte ui_ver_txt_rstr[];
extern const byte ui_service3_txt_rstr[];
extern const byte ui_not_common_txt_rstr[];
extern const byte ui_re_enter_new_txt_rstr[];
extern const byte ui_ringer_off_txt_rstr[];
extern const byte ui_ncli_txt_rstr[];
extern const byte incoming_analog_str[];
extern const byte incoming_analog_roam_str[];
extern const byte incoming_digital_str[];
extern const byte incoming_digital_roam_str[];

#ifdef FEATURE_UI_AUTO_ANS_INDICATE
extern const byte ui_autoanswer_on_txt_rstr[];
#endif

extern const byte ui_data_only_txt_rstr[];
extern const byte ui_fax_only_txt_rstr[];

#ifdef FEATURE_UI_CHECK_BILLING
#error code not present
#endif
#ifdef FEATURE_UI_ENTER_NAME_PROMPT
#error code not present
#endif
extern const byte ui_empty_txt[] ;
extern const byte ui_please_wait_txt[] ;

#ifdef FEATURE_OTASP
extern const byte ui_otasp_activation_txt[] ;
extern const byte ui_otasp_in_progress_txt[] ;
extern const byte ui_otasp_completed_txt[] ;
extern const byte ui_otasp_failed_txt[] ;
extern const byte ui_otasp_invalid_txt[] ;
#endif // FEATURE_OTASP

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)

#else  /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
extern const byte ui_emergency_nametag_txt[];
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */



#ifdef FEATURE_DS_UP
#error code not present
#endif
#ifdef FEATURE_DS
/* service option texts for data calls */
extern const byte data_call_txt[];
extern const byte modem_data_txt[];
extern const byte ppp_data_txt[];
extern const byte fax_call_txt[];
#endif /* ifdef FEATURE_DS */

/* Some text values to be used, must have UI_WIDE characters */

#define UI_SECRET_TXT        ui_secret_txt_rstr
#define UI_EMERGENCY_TXT     ui_emergency_txt_rstr
#define UI_NEWCODE_TXT       ui_newcode_txt_rstr
#define UI_NEWCODE_AGAIN_TXT ui_newcode_again_txt_rstr
#define UI_NMSG_TXT          ui_nmsg_txt_rstr
#define UI_MWN_TXT           ui_mwn_txt_rstr
#define UI_NVOICE1_TXT       ui_nvoice1_txt_rstr
#define UI_NVOICE2_TXT       ui_nvoice2_txt_rstr
#define UI_VER_TXT           ui_ver_txt_rstr
#define UI_SERVICE3_TXT      ui_service3_txt_rstr
#define UI_NOT_COMMON_TXT    ui_not_common_txt_rstr
#define UI_RE_ENTER_NEW_TXT  ui_re_enter_new_txt_rstr
#define UI_RINGER_OFF_TXT    ui_ringer_off_txt_rstr
#define UI_NCLI_TXT          ui_ncli_txt_rstr
#ifdef FEATURE_UI_AUTO_ANS_INDICATE
#define UI_AUTO_ANSWER_ON_TXT ui_autoanswer_on_txt_rstr
#endif

#define UI_DATA_ONLY_TXT   ui_data_only_txt_rstr
#define UI_FAX_ONLY_TXT    ui_fax_only_txt_rstr

#ifdef FEATURE_UI_CHECK_BILLING
#error code not present
#endif
#ifdef FEATURE_UI_ENTER_NAME_PROMPT
#error code not present
#endif
#define UI_EMPTY_TXT ui_empty_txt
#define UI_PLEASE_WAIT_TXT ui_please_wait_txt

#ifdef FEATURE_OTASP
#define UI_OTASP_ACTIVATION_TXT ui_otasp_activation_txt
#define UI_OTASP_IN_PROGRESS_TXT ui_otasp_in_progress_txt
#define UI_OTASP_COMPLETED_TXT ui_otasp_completed_txt
#define UI_OTASP_FAILED_TXT ui_otasp_failed_txt
#define UI_OTASP_INVALID_TXT ui_otasp_invalid_txt
#endif


#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)

#else  /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
#define UI_EMERGENCY_NAMETAG_TXT ui_emergency_nametag_txt
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */



/* Characters for bar graph, like volume */
// these defines are only used in other text, ie. already in ROM

/* 95 = _, 138 = ä, 139 = ã, 140 = å, 141 = ç, 142 = é, 143 = è */
#ifdef FEATURE_UI_SILENT_TO_OFF
#error code not present
#else
#define UI_VOL_0 "VOLUME            silent"  /* silent */
#endif

#define UI_VOL_1 "VOLUME          __      "  /* low    */
#define UI_VOL_2 "VOLUME          __äã    "  /* M-     */
#define UI_VOL_3 "VOLUME          __äãåç  "  /* M+     */
#define UI_VOL_4 "VOLUME          __äãåçéè"  /* High   */

#ifdef FEATURE_UI_SEVEN_LEVEL_SND
#error code not present
#endif /* FEATURE_SEVEN_LEVEL_SND */

#define UI_CHECK_MARK '?' //'ù''

#define SOFTKEY_SIZE UI_WIDE/2         /* Size of one softkey label */
#define SOFTKEY2 UI_WIDE/2             /* Position of second softkey */
#define ALPHA_CURSOR 0x9F              /* Alpha entry cursor ( Checkerboard ) */

/* Soft Key Labels */
#ifdef FEATURE_UI_KEYGUARD
#error code not present
#endif



extern const byte ui_sk_lock_f_rstr[];
extern const byte ui_sk_unlock_f_rstr[];    
extern const byte ui_sk_menu_book_f_rstr[]; 
extern const byte ui_sk_save_book_f_rstr[]; 

#ifdef FEATURE_DS_UP
#error code not present
#endif

extern const byte ui_sk_menu_next_f_rstr[];

extern const byte ui_sk_save_find_f_rstr[];
extern const byte ui_sk_quit_akey_f_rstr[]; 
extern const byte ui_sk_ok_akey_f_rstr[];
extern const byte ui_sk_ok_more_f_rstr[];
extern const byte ui_sk_save_time_f_rstr[];
extern const byte ui_sk_save_f_rstr[];
extern const byte ui_sk_ok_next_f_rstr[];   
extern const byte ui_sk_ok_quit_f_rstr[];   
extern const byte ui_sk_quit_f_rstr[];      
extern const byte ui_sk_ok_zero_f_rstr[]; 
extern const byte ui_sk_ok_reset_f_rstr[];
extern const byte ui_sk_ok_f_rstr[];        
extern const byte ui_sk_save_lower_f_rstr[];
extern const byte ui_sk_save_upper_f_rstr[];
extern const byte ui_sk_tone_erase_f_rstr[];
extern const byte ui_sk_edit_erase_f_rstr[];
extern const byte ui_sk_erase_f_rstr[];     
extern const byte ui_sk_view_f_rstr[];      
extern const byte ui_sk_name_calls_f_rstr[];
extern const byte ui_sk_find_f_rstr[];       
extern const byte ui_sk_edit_next_f_rstr[]; 
extern const byte ui_sk_ok_edit_f_rstr[];   
extern const byte ui_sk_ok_old_f_rstr[];    
extern const byte ui_sk_voice_text_f_rstr[];
extern const byte ui_sk_erase_next_f_rstr[];
extern const byte ui_sk_erase_quit_f_rstr[];
extern const byte ui_sk_erase_view_f_rstr[];
extern const byte ui_sk_yes_no_f_rstr[];    
extern const byte ui_sk_add_quit_f_rstr[];  
extern const byte ui_sk_resume_f_rstr[];    
extern const byte ui_sk_lower_f_rstr[];     
extern const byte ui_sk_upper_f_rstr[];     
extern const byte ui_sk_exit_more_f_rstr[];
extern const byte ui_sk_view_erase_f_rstr[];
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */
#ifdef FEATURE_UI_OTKSL
#error code not present
#endif
#define UI_SK_LOCK_F         ui_sk_lock_f_rstr
#define UI_SK_UNLOCK_F       ui_sk_unlock_f_rstr
#define UI_SK_MENU_BOOK_F    ui_sk_menu_book_f_rstr
#define UI_SK_SAVE_BOOK_F    ui_sk_save_book_f_rstr
#ifdef FEATURE_DS_UP
#error code not present
#endif

#define UI_SK_MENU_NEXT_F     ui_sk_menu_next_f_rstr

#define UI_SK_SAVE_FIND_F    ui_sk_save_find_f_rstr
#define UI_SK_QUIT_AKEY_F    ui_sk_quit_akey_f_rstr
#define UI_SK_OK_AKEY_F      ui_sk_ok_akey_f_rstr
#define UI_SK_OK_MORE_F      ui_sk_ok_more_f_rstr
#define UI_SK_SAVE_TIME_F    ui_sk_save_time_f_rstr
#define UI_SK_SAVE_F         ui_sk_save_f_rstr
#define UI_SK_OK_NEXT_F      ui_sk_ok_next_f_rstr
#define UI_SK_OK_QUIT_F      ui_sk_ok_quit_f_rstr
#define UI_SK_OK_ZERO_F      ui_sk_ok_zero_f_rstr
#define UI_SK_OK_RESET_F     ui_sk_ok_reset_f_rstr
#define UI_SK_OK_F           ui_sk_ok_f_rstr
#define UI_SK_SAVE_LOWER_F   ui_sk_save_lower_f_rstr
#define UI_SK_SAVE_UPPER_F   ui_sk_save_upper_f_rstr
#define UI_SK_TONE_ERASE_F   ui_sk_tone_erase_f_rstr
#define UI_SK_EDIT_ERASE_F   ui_sk_edit_erase_f_rstr
#define UI_SK_ERASE_F        ui_sk_erase_f_rstr
#define UI_SK_VIEW_F         ui_sk_view_f_rstr
#define UI_SK_NAME_CALLS_F   ui_sk_name_calls_f_rstr
#define UI_SK_FIND_F         ui_sk_find_f_rstr
#define UI_SK_EDIT_NEXT_F    ui_sk_edit_next_f_rstr
#define UI_SK_OK_EDIT_F      ui_sk_ok_edit_f_rstr
#define UI_SK_OK_OLD_F       ui_sk_ok_old_f_rstr
#define UI_SK_VOICE_TEXT_F   ui_sk_voice_text_f_rstr
#define UI_SK_ERASE_NEXT_F   ui_sk_erase_next_f_rstr
#define UI_SK_ERASE_QUIT_F   ui_sk_erase_quit_f_rstr
#define UI_SK_ERASE_VIEW_F   ui_sk_erase_view_f_rstr
#define UI_SK_YES_NO_F       ui_sk_yes_no_f_rstr
#define UI_SK_ADD_QUIT_F     ui_sk_add_quit_f_rstr
#define UI_SK_QUIT_F         ui_sk_quit_f_rstr
#define UI_SK_RESUME_F       ui_sk_resume_f_rstr
#define UI_SK_LOWER_F        ui_sk_lower_f_rstr
#define UI_SK_UPPER_F        ui_sk_upper_f_rstr
#define UI_SK_EXIT_MORE_F    ui_sk_exit_more_f_rstr
#define UI_SK_VIEW_ERASE_F   ui_sk_view_erase_f_rstr
#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */
#ifdef FEATURE_UI_OTKSL
#error code not present
#endif
extern const byte ui_akey_ok_f_rstr[];
extern const byte ui_akey_entry_f_rstr[];
extern const byte ui_akey_bad_f_rstr[];
extern const byte ui_akey_good_f_rstr[];

#define UI_AKEY_OK_F    ui_akey_ok_f_rstr
#define UI_AKEY_ENTRY_F ui_akey_entry_f_rstr
#define UI_AKEY_BAD_F   ui_akey_bad_f_rstr
#define UI_AKEY_GOOD_F  ui_akey_good_f_rstr

/* TEMP */


extern const byte ui_13k_enable_rstr[];
extern const byte ui_13k_disable_rstr[];
extern const byte ui_13k_notavailable_rstr[];
extern const byte ui_cdmaonly_enable_rstr[];

#ifdef FEATURE_DS_QNC
extern const byte ui_qnc_enable_rstr[];
extern const byte ui_qnc_disable_rstr[];
#endif
#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif

extern const byte ui_earup_deepsleep_f_rstr[];
extern const byte ui_eardn_deepsleep_f_rstr[];
extern const byte ui_num_restricted_rstr[];
extern const byte ui_num_unavailable_rstr[];
extern const byte ui_call_made_f_rstr[];
extern const byte ui_num_added_f_rstr[];
extern const byte ui_no_duration_f_rstr[];
extern const byte ui_no_call_date_f_rstr[];

/* callinfo_str will be copied from below strings */
extern const byte lastcall_str[] ;
extern const byte homecall_str[] ;
extern const byte roamcall_str[] ;
extern const byte allcall_str[] ;

#define UI_13K_ENABLE        ui_13k_enable_rstr
#define UI_13K_DISABLE       ui_13k_disable_rstr
#define UI_13K_NOTAVAILABLE  ui_13k_notavailable_rstr
#define UI_CDMAONLY_ENABLE   ui_cdmaonly_enable_rstr

#ifdef FEATURE_DS_QNC
#define UI_QNC_ENABLE        ui_qnc_enable_rstr
#define UI_QNC_DISABLE       ui_qnc_disable_rstr
#endif

#ifdef FEATURE_ANALOG_HFK
#error code not present
#endif
#define UI_EARUP_DEEPSLEEP_F ui_earup_deepsleep_f_rstr
#define UI_EARDN_DEEPSLEEP_F ui_eardn_deepsleep_f_rstr
#define UI_NUM_RESTRICTED  ui_num_restricted_rstr
#define UI_NUM_UNAVAILABLE ui_num_unavailable_rstr

#define UI_CALL_MADE_F       ui_call_made_f_rstr
#define UI_NUM_ADDED_F       ui_num_added_f_rstr
#define UI_NO_DURATION_F     ui_no_duration_f_rstr
#define UI_NO_CALL_DATE_F    ui_no_call_date_f_rstr


/* <EJECT> */
typedef byte ui_font_type[HS_GLYPH_HEIGHT];

#define UI_FONT_CNT 4
extern const ui_font_type ui_font[ UI_FONT_CNT ];

#define UI_CHAR_00 0x00
#define UI_CHAR_01 0x01
#define UI_CHAR_02 0x02
#define UI_CHAR_03 0x03

/* what characters on what keys */
extern const byte ui_keys[20][5 + 1];
extern const byte punc_list[]; 
extern const char months_array[13][3 + 1];
extern const char day_of_week[7][3 + 1];

/* Used to display hex numbers; defined in uintxt.c */
extern const byte hex_cnt[16 + 1];

/* <EJECT> */
/* --------------- Define Screen Display Field -------------------- */

/* A link type to use for the linked list placeholder when defining */
#define LINK NULL


typedef struct {
  byte    pri;                    /* priority of field */
  byte    y;                      /* which line on screen, 0 is top */
  byte    x;                      /* which column on screen,  0 is left */
  boolean blnk;                   /* true if blinking */
  byte    length;                 /* length of item to copy to RAM */
  byte    *txt_ptr;               /* the actual text */
} ui_field_type;


/* Maximum possible priority of 255 is reserved for use by ui_dummy_f */
#define UI_MAX_FIELD_PRI 255

/* Time priorities, so it doesn't obscure the debug screen */
#define UI_TIME_NORM_PRI 24       /* normal time priority */
#define UI_TIME_DBG_PRI   6       /* debug time priority */

#define UI_DBG_NORM_PRI  8        /* not in call */
#define UI_DBG_CALL_PRI  12       /* in call */
                             /* List all the fields defined - these are all viable fields that can */
/* end up on the screen, although just defining them doesn't put them */
/* on the screen.  These are listed in priority order for convenience */


/*---------------------------------------------------------------------------
  Actual array of field items (except cursor items).
---------------------------------------------------------------------------*/
extern const ui_field_type ui_field_array[ ((int)UI_DUMMY_F+1)];


/* <EJECT> */
/* -------------- Help Screen Defines -------------------------- */

#define HELPSCRN_SIZE UI_WIDE * 4

/* Help Screen type */
typedef byte helpscreen[HELPSCRN_SIZE+1];   /* Help screen type */

/* INFO key help screens */
typedef struct {
  byte   fn_help_scrns;         /* Number of Functional help screens */
  byte   op_help_scrns;         /* Number of Operational help screens */
  helpscreen *fn_help_scrn_txt; /* Functional help screens text */
  helpscreen *op_help_scrn_txt; /* Operational help screens text */
} ui_infotxt_type;

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)

#else  /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */
/* Defined Menus */
extern ui_infotxt_type const pause_hyphen_info; /* Pause & Hyphen insertion Help menus */
extern ui_infotxt_type const punctuation_info;  /* Punctuation insertion Help menus */
extern ui_infotxt_type const recall_info;       /* Recall Help menus */
extern ui_infotxt_type const store_info;        /* Store Help menus */
extern ui_infotxt_type const storeover_info;    /* Store Overwrite Help menus */
extern ui_infotxt_type const retry_info;        /* Auto Retry Help menus */
#endif /* (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0) */



/* <EJECT> */
/* -------------- Define Selection Lists ----------------------- */

/* list items - leave room for null terminated */
typedef byte listtxt_type[UI_WIDE*2+1];

#define ISNV TRUE
#define NOTNV FALSE
#define WRAP TRUE
#define NOWRAP FALSE
#define LINE1 1
#define LINE2 2

/* list structure */
typedef struct {
  listtxt_type  txt;              /* list title */
  boolean       isnv;             /* TRUE if "item" is valid nv item code */
  word          item;             /* nv item code */
  boolean       wrap;             /* if list wraps around */
  byte          high;             /* 1 or 2 lines per item */
  byte          nitems;           /* number of items */
  struct {                        /* array of list text items */
    listtxt_type  txt;            /* text of choice - last is empty */
    int          check;          /* index to where the default check mark goes */
    word          val;            /* value of choice */
/*lint -e43  Lint doesn't like this items[] */
  } items[10];
/*lint +e43 */
} ui_list_type;

/* defined lists */
extern ui_list_type const ui_makesecret_l;            /* make secret? */

extern ui_list_type const ui_clrerrs_l;               /* clr err log? */

extern ui_list_type const ui_retry_l;                 /* Retry options */


extern ui_list_type const ui_answer_l;                /* autoanswer options */

#if defined( FEATURE_CDMA_800 )
extern ui_list_type const ui_cdma_pref_l;             /* cdma prefered system */
#endif /* defined( FEATURE_CDMA_800 ) */

#ifdef FEATURE_UI_FORCE_CALL
extern ui_list_type const ui_force_call_l;            /*  force call now? */
extern ui_list_type const ui_force_mode_l;            /*  force mode options */
#endif /* FEATURE_UI_FORCE_CALL */

#if ( NV_MAX_NAMS != 1 )
extern ui_list_type const ui_auto_nam_l;              /* Phone # menu */
#endif /* NV_MAX_NAMS != 1 */


extern ui_list_type     ui_back_light_l;            /* display setup menu */
extern ui_list_type const ui_auto_hyphen_l;           /* auto hyphen on off */
extern ui_list_type const ui_time_show_l;             /* show time if available */
#ifdef FEATURE_UI_SEND_PIN
extern ui_list_type const ui_send_pin_l;              /* send PIN : never,  prompt */
#endif
#ifndef FEATURE_UI_VOICE_PRIVACY
extern ui_list_type const ui_voice_privacy_l;         /* Voice Privacy selection */
#endif /* !FEATURE_UI_VOICE_PRIVACY */
extern ui_list_type const ui_testcalls_l;             /* Debug Test calls */
#ifdef FEATURE_UI_OTASP_MENU
extern ui_list_type const ui_activation_l;
#endif

#ifdef FEATURE_DS_VOICE_AS_DATA
extern ui_list_type const ui_voice_as_data_l;         /* answer voice as data */
#endif
#ifdef FEATURE_TTY
#error code not present
#endif /* FEATURE_TTY */

#ifdef FEATURE_USB_TESTING
#error code not present
#endif /* FEATURE_USB_TESTING */



extern ui_list_type const ui_ear_vol_l;               /* sound setup menu */
extern ui_list_type const ui_speaker_vol_l;
extern ui_list_type const ui_headset_vol_l;
extern ui_list_type const ui_ring_vol_l;
extern ui_list_type const ui_ring_spkr_vol_l;
extern ui_list_type const ui_beep_vol_l;
extern ui_list_type const ui_beep_spkr_vol_l;
extern ui_list_type const ui_beep_headset_vol_l;
extern ui_list_type const ui_alerts_vol_l;           /* SMS Volume */
extern ui_list_type const ui_alerts_vol_shadow_l;
extern ui_list_type const ui_key_dtmf_l;
extern ui_list_type const ui_one_touch_l;
extern ui_list_type const ui_voice_so_l;


#ifdef FEATURE_AUDIO_AGC
#error code not present
#endif /* FEATURE_AUDIO_AGC */
#ifdef FEATURE_MULTIPLE_RINGER_TYPES
#error code not present
#endif /* FEATURE_MULTIPLE_RINGER_TYPES */
#ifdef FEATURE_UASMS
#ifdef FEATURE_BROADCAST_SMS
extern ui_list_type const ui_bcsms_pref_l;
extern ui_list_type const ui_bcsms_srvc_enable_l;
extern ui_list_type const ui_bcsms_srvc_pri_l;
extern ui_list_type const ui_bcsms_srvc_delete_l;
#endif /* FEATURE_BROADCAST_SMS */
#endif /* FEATURE_UASMS */
#ifdef FEATURE_VS_TASK
#error code not present
#endif /* FEATURE_VS_TASK */

#ifdef FEATURE_VOICE_MEMO
#error code not present
#endif /* FEATURE_VOICE_MEMO */

extern ui_list_type const ui_auto_lock_l;             /* lock setup menus */
extern ui_list_type const ui_clr_stack_l;             /* clear call stack? */

extern ui_list_type const ui_restrict1_l;             /* call restrictions */

extern ui_list_type const ui_call_fade_l;             /* alerts setup menu */
extern ui_list_type const ui_call_beep_l;
extern ui_list_type const ui_svc_area_l;
extern ui_list_type const ui_roam_ringer_l;

#ifdef FEATURE_PM1000
#error code not present
#endif /* FEATURE_PM1000 */

#if (MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)
/*===============================================================================

                            EQUS UI Variables (LIST)
	          
=================================================================================*/
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagLIST                 eqsList;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagLIST{
	ui_list_type    List;                //ui.list
};


/*
**--------------------------------------------------------------------------------
**  Variable Declarations
**--------------------------------------------------------------------------------
*/
extern eqsList      g_List;

#endif //(MODEL_ID == MODEL_B1) || (MODEL_ID == MODEL_B0)

#endif

#endif // #if defined (MMI_TREX)