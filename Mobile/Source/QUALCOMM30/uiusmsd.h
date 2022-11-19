#ifdef FEATURE_UASMS
#ifndef UISMSD_H
#define UISMSD_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


    U S E R   I N T E R F A C E   S M S   D I S P L A Y   S E R V I C E

GENERAL DESCRIPTION
  Contains the types and items required for controlling and displaying 
  SMS menus and screens.

      Copyright (c) 1995 by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uiusmsd.h_v   1.1   29 Mar 2000 18:11:02   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/22/98   KCK     Added support for SMS UTC Time Stamp.
07/17/98   JGR     Added support for Net Alerts for UP all changes are under
                   FEATURE_DS_UP
06/26/98   skk     Fixed a bug in SMS Scroll Menu where memory was not allocated
                   for ui_sms_first_menu 
06/12/98   skk     Added SMS Scroll Menu implementation for Soft key targets.
                   Added data structures and functions to allow addition of 
                   UP menus.
11/14/97   skk     Added a "Please wait erasing" message when erasing old SMS
                   messages. CR4279
04/18/96   BK      New UI branching created from uismsd.h ver 1.5
09/19/95   day     Change and add voice and call back number displays.
                   Change memcpy to memset calls for blanking screens.
09/13/95   day     Added auto-format routine for character strings.
09/11/95   day     Added BCD to ASCII conversion for time stamp parameter.
09/06/95   day     Added no voice mail number display enumerated tag.
08/22/95   day     Added call back number save and prepend functions.
08/10/95   day     Created

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "comdef.h"
#include "uissms.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* UI SMS display types */
typedef enum {
  UI_SMS_DISP_NONE,                  /* no SMS displays                  */
  UI_SMS_DISP_MSG_NONE,              /* no SMS message display           */
  UI_SMS_DISP_NOTIF_NONE,            /* no SMS notification display      */
  UI_SMS_DISP_STS_NONE,              /* no SMS status display            */
  UI_SMS_DISP_NOTIF,                 /* SMS page/voice notification      */
  UI_SMS_DISP_REJ_STATUS,            /* SMS message reject status        */
  UI_SMS_DISP_MEM_STATUS,            /* SMS memory status (>75%)         */
  UI_SMS_DISP_NEW_MSGS,              /* SMS new msg/voice mail status    */
  UI_SMS_DISP_TEXT,                  /* SMS message text                 */
  UI_SMS_DISP_MSG_LIST,              /* SMS list of text messages        */
  UI_SMS_DISP_NO_TEXT,               /* SMS no text messages status      */
  UI_SMS_DISP_ERASE,                 /* SMS message erase option         */
  UI_SMS_DISP_MSG_ERASED,            /* SMS message erased               */
  UI_SMS_DISP_VOICE,                 /* SMS voice mail number            */
  UI_SMS_DISP_CALL_BACK,             /* SMS call back number             */
  UI_SMS_DISP_VC_CALL_BACK,          /* SMS no voice mail number         */
  UI_SMS_DISP_NO_VC_INFO,            /* SMS no voice mail information    */
  UI_SMS_DISP_MSG_WAIT_ERASE,        /* SMS message waiting to be erased CR4279*/
#ifdef FEATURE_DS_UP
#error code not present
#endif
  UI_SMS_SCROLL_MENU                 /* SMS Scroll Menu                         */
} ui_sms_display_type;

/* Maximum number of display lines required to */
/* hold the largest possible text message.     */
/* A display line = one line * UI_WIDE.        */
#define UI_SMS_DISP_LEN 25

// #pragma pack(1)

/* Structure to hold current display information */
typedef struct
{
  ui_sms_display_type display;   /* display type             */
  boolean             new_disp;  /* new message display or use current */
  boolean             soft_disp; /* Indicates that softkeys are displayed */
  byte                sms_list;  /* SMS list for current display */
  byte                sms_idx;   /* Index to SMS list item */
  byte                cur_line;  /* current top line being displayed */
  byte                scr_line;  /* current line where scroll marker is */
  byte                line_len;  /* number of lines required for message */
  byte                cb_len;    /* call back number length */
  byte                call_back[SMSI_CALL_BACK_MAX]; /* call back number */
  byte                text[UI_SMS_DISP_LEN][UI_WIDE]; /* message being display */
} ui_sms_disp_info_type;

typedef struct
{
  boolean             show_count;       /* if Trure count will be shown in front of text */ 
  byte                count;            /* Holds a number to be inserted in front of text */
#ifdef FEATURE_DS_UP
#error code not present
#endif
  byte                text[UI_WIDE];    /* list text */
} ui_sms_scroll_item_type;              /* SMS Scroll Menu */

/* Structure to hold current display information */
typedef struct
{
  ui_sms_display_type     display;                   /* display type              */
  byte                    num_items;                 /* number of items in the list-Maximum of UI_SMS_DISP_LEN */
  byte                    selected_item;             /* index into selected item from scroll menu list */
  byte                    cur_line;                  /* current top line being displayed */
  byte                    scr_line;                  /* current line where scroll marker is */
  byte                    soft_disp[UI_WIDE];        /* Indicates which softkeys are displayed */
  byte                    title[UI_WIDE];
  ui_sms_scroll_item_type list_item[UI_SMS_DISP_LEN];/* holds the scroll menu list */
} ui_sms_scroll_menu_type;                           /* SMS Scroll Menu */

/* Resume default packing. */
// #pragma pack()

/* Use for num_format function call. */
extern ui_edit_type call_back_buf;

/* Holds the hyphenated call back number. */
extern byte number_buf[UI_WIDE*3];

/* Holds the initial SMS Scroll Menu */
extern ui_sms_scroll_menu_type ui_sms_first_menu;

#ifdef FEATURE_DS_UP
#error code not present
#endif
/* Define space character for memset function call. */
#define SPACE_CHAR  ' '

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_AUTO_FORMAT

DESCRIPTION
  Eliminate extra spaces between words and move word to new
  display line if it does not fit on current line.  Formatted
  text will be left-justified.
  Returns number of display lines for resulting formatted 
  character string.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern byte ui_sms_auto_format
(
  boolean word_wrap,          /* Indicate if wrapping words      */
  byte    *input_txt,         /* Pointer to text to be formatted */
  word    input_len,          /* Number of bytes to be formatted */
  byte    *format_txt,        /* Pointer to formatted text       */
  word    format_len          /* Size of formatted text          */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_BCD_ASCII_CVT

DESCRIPTION
  Convert 2 BCD numbers to 2 ASCII values.
  Returns pointer to character after last character written

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern char *ui_sms_bcd_ascii_cvt
(
  char bcd_num,                 /* 2 BCD numbers to convert */
  char *txt                     /* ascii conversion for up to 2 numbers */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_ASCII_CVT

DESCRIPTION
  Convert a number to ASCII
  Returns pointer to character after last character written

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
extern char *ui_sms_ascii_cvt
(
  word num,                     /* number to convert */
  char *txt                     /* ascii conversion for up to 2 numbers */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_DISPLAY_CTRL

DESCRIPTION
  Determine what type of SMS display needs to be formatted and
  displayed to the user.
  
DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void ui_sms_display_ctrl 
(
  ui_sms_display_type display,
    /* Indicates screen to be displayed */
  ui_sms_disp_info_type *disp_info
    /* Current display information */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_DISP_GENERIC_LIST

DESCRIPTION
  Format and display SMS list of old text messages in LIFO order.
  SMS Scroll Menu  
DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void ui_sms_disp_generic_list 
(
ui_sms_scroll_menu_type *ui_sms_generic_list
);

/* <EJECT> */
/*===========================================================================

FUNCTION UI_SMS_BCD_TO_JULIAN
SMS UTC time 

DESCRIPTION

DEPENDENCIES
  None

RETURNS

SIDE EFFECTS
  None

===========================================================================*/
extern void ui_sms_bcd_to_julian 
(
  smsi_abs_time_type mc_time, 
  clk_julian_type *julian
);

/* <EJECT> */
/*===========================================================================
FUNCTION UI_SMS_UTC_TO_LOCAL
SMS UTC time 

DESCRIPTION

DEPENDENCIES
  None

RETURNS

SIDE EFFECTS
  None
===========================================================================*/
extern void ui_sms_utc_to_local 
(
  dword utc_time, 
  clk_julian_type *julian
);

#endif
#endif /* FEATURE_UASMS */

