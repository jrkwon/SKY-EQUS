#ifndef UISCRN_H
#define UISCRN_H
/*===========================================================================

        U S E R   I N T E R F A C E    S C R E E N   F U N C T I O N S

DESCRIPTION
  The externalized functions for UISCRN.C

    Copyright (c) 1991,1992 by QUALCOMM INCORPORATED All Rights Reserved.
    Copyright (c) 1993,1994,1995,1996,1997 by QUALCOMM, Incorporated.
    Copyright (c) 1998 by QUALCOMM, Incorporated.
    All Rights Reserved.
===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   L:/src/asw/MSM5000/VCS/uixscrn.h_v   1.1   29 Mar 2000 18:11:10   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/03/99   VT      Merged TriMode Code.
10/26/99   CS      Removed Kpdbacklight control function.
08/27/99   CS      Added Lcdbacklight and Kpdbacklight menu options to control
                   LCD and Keypad backlight levels for PM1000.
07/13/98   AYK     Removed the definition of uiscrn_animate_dot_type because
                   it is not needed anymore since the ellipsis animation is now
                   using a bit field to represent active animations.
06/22/98   JGR     Change ]cursor logic to support blinking with any character
                   not always just a space.
05/29/98   AYK     Added the enum type uiscrn_animate_dot_type to indicate 
                   which field the animated ellipsis should go in.  Also added 
                   prototypes for the start and stop functions under
                   FEATURE_UI_ANIMATE_CALL and FEATURE_UI_ANIMATE_SEARCH.
04/02/98   KAR     Fixed Copyright notice.
03/04/98   KAR     Moved "Ringer Off" display function from uinsnd
06/17/97   SIU     Removed pragma statements with target name.
03/12/97   jjn     Added (TG==T_MD) in conditional compile statements
                   wherever appropriate for the Module
05/09/96   JGR     Screen Handler Phase II without Ram Allocation. No direct access 
                   to ui_field_array[...].txt_ptr.
04/18/96   BK      New UI branching created from uiscrn.h ver 1.17
03/05/96   BK      Replace T_PRMCO1900 with T_GP.
02/29/96   JGR     New screen handling technique
02/13/96   BK      T_PRMCO1900 introduction
10/11/95   DAK     Added #include "target.h" and conditionals to include
                   uitxtw.h when compiling ISS2
05/01/95   CAH     Initial Gemini mods.
04/15/95   RAS     Initial ISS2 target support.
04/13/95   CAH     Preliminary CDMA RSSI support for Beta2 and Portable
08/24/94   cah     Support for CDMA RSSI during no service in ISS1, prototype
                    fix.
10/13/92   rrd     Comment and clean
02/02/92   rrd     Created

===========================================================================*/

#include "target.h"
#include "comdef.h"               /* basic definitions */

#include "uiutxt.h"                /* ui screen stuff */


/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#define NO_CDMA_SIGNAL 0                /* No CDMA signal index value */
#define BATT_OUTLINE 1                  /* Battery annunciator outline only */
#define NO_BATT 0                       /* No battery icon */
#define FULL_BATT 5                     /* Full Battery annunciator */


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* <EJECT> */
/*===========================================================================

FUNCTION UISCRN_REFRESH

DESCRIPTION
  This actually rebuilds the screen if necessary.  If ui_list_chg is set,
  the pointers to all the screen positions are built from the linked list.
  If ui_update_disp is set, all the pointers are dereferenced to create
  the actual screen.

  The final result is sent to the handset driver.
  
DEPENDENCIES
  Everything depends on the list, list_ptr, being correct

RETURN VALUE
  True if actual update took place
       
SIDE EFFECTS
  Plays with almost all of the variables in this module.

===========================================================================*/
extern boolean uiscrn_refresh
(
  void
);


/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_LIST_UPDATE

DESCRIPTION
  Just sets a flag letting the screen handler know that a field somewhere
  has been fundamentally changed - position,  blinking,  etc..
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void uiscrn_list_update( void );


/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_UPDATE

DESCRIPTION
  Just sets a flag letting the screen handler know that a field somewhere
  has been updated.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void uiscrn_update( void );



/* <EJECT> */
/*===========================================================================

FUNCTION UISCRN_DEACT

DESCRIPTION
  Remove an item from the display, i.e. from the linked list.
  
DEPENDENCIES
  list_ptr and the list should be uncorrupted.

RETURN VALUE
  TRUE if the field was active, FALSE if not
       
SIDE EFFECTS
  None

===========================================================================*/
extern boolean uiscrn_deact
(
  ui_field_item_enum_type field_item            /* field item, array index */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_DEACT_ALL

DESCRIPTION
  Remove all items from the linked list.  For powerdown.
  
DEPENDENCIES
  list_ptr and the list should be uncorrupted.

RETURN VALUE
  UI_DONE_S
       
SIDE EFFECTS
  None

===========================================================================*/
extern void uiscrn_deact_all( void );

/* <EJECT> */
/*===========================================================================

FUNCTION UISCRN_ACT_CURSOR

DESCRIPTION
  This function either adds the passed cursor to current screen list or changes
  the position/char of the currently active cursor. 
  
DEPENDENCIES
  None

RETURN VALUE
  None
       
SIDE EFFECTS
  None

===========================================================================*/

extern void uiscrn_act_cursor
(
  byte  pri,                            /* screen display priority         */
  byte  y_pos,                          /* row position to place cursor    */
  byte  x_pos,                          /* column position to place cursor */
  byte  cursor,                         /* actual cursor character         */
  byte  blink_cursor                    /* the cursor character to blink with */
);


/*===========================================================================
FUNCTION UISCRN_ACT

DESCRIPTION
  Activate a field,  i.e. place it on the screen,  by putting it into the
  priority sorted list
  
DEPENDENCIES
  list_ptr and the list should be uncorrupted.

RETURN VALUE
  DONE_S or ERR_S
       
SIDE EFFECTS
  None
===========================================================================*/
extern int uiscrn_act
(
  ui_field_item_enum_type field_item            /* field item, array index */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_SCRN

DESCRIPTION
  Turn the screen on or off, also set the blink rate.


DEPENDENCIES
  We can get a free HS buffer
  

RETURN VALUE
  None

SIDE EFFECTS
  Resets all the internal annunciators.

===========================================================================*/
extern void uiscrn_scrn
(
  boolean onoff, 
  word  rate                      /* blink rate */
);


/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_BACKLIGHT

DESCRIPTION
  Turn backlighting on or off

DEPENDENCIES
  Screen is on

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
extern void uiscrn_backlight
(
  boolean onoff
);



#ifdef FEATURE_PM1000
#error code not present
#endif /* FEATURE_PM1000 */

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_ANN

DESCRIPTION
  Set handset annunciators on or off

DEPENDENCIES
  Screen is on

RETURN VALUE
  None

SIDE EFFECTS
  None
  
  
===========================================================================*/
extern void uiscrn_ann
(
  word  mask,                     /* mask of annunciators */
  word  value                     /* annunciator values to mask in */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_ANNBLINK

DESCRIPTION
  Set annunciator(s) blinking.  Very similar to UISCRN_ANN except blinking
  can only be activated.  To turn it off, call UISCRN_ANN for that
  annunciator.

DEPENDENCIES
  Screen is on

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
extern void uiscrn_annblink
(
  word  mask                      /* which annunciators */
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_RSSI

DESCRIPTION
  Set RSSI annunciator bar
  
DEPENDENCIES
  Screen is on

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
extern void uiscrn_rssi
#if ( defined(FEATURE_CDMA_800) && defined(FEATURE_CDMA_1900) )
(
  byte val,
  boolean blink,
  boolean cdma_cellular
);
#else
(
  byte val,
  boolean blink
);
#endif
/*===========================================================================

FUNCTION UISCRN_SET_BLINK_STATE

DESCRIPTION
  Set the attribute of the passed screen item to the passed state.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE if passed item in list, else FALSE.
       
SIDE EFFECTS
  None

===========================================================================*/
extern boolean uiscrn_set_blink_state
(
  ui_field_item_enum_type field_item,                    /* item to change */
  boolean                 blink_state               /* blink state         */
);

/* <EJECT> */
/*===========================================================================

FUNCTION UISCRN_INIT

DESCRIPTION
  Sets up the screen handlers' internal variables, blanks the screen
  
DEPENDENCIES

RETURN VALUE
  None
       
SIDE EFFECTS
  Only as above

===========================================================================*/
extern void uiscrn_init( void );

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_BATT

DESCRIPTION
  Set Battery annunciator bar
  
DEPENDENCIES
  Screen is on

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
extern void uiscrn_batt
(
  byte val,
  boolean blink
);

/*===========================================================================
FUNCTION UISCRN_BIND_ACT

DESCRIPTION
  This function calls uiscrn_bind_field to get the text pointer for the
  passed field value which is returned. It then calls uiscrn_act.

DEPENDENCIES
  None.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_act
(
  ui_field_item_enum_type field_value,            /* field item, array index */
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_ALLOC_FIELD

DESCRIPTION
  This function finds the text pointer for the field value passed. If one does
  not exist it gets one. 

DEPENDENCIES
  None.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_alloc_field
(
  ui_field_item_enum_type field_value            /* field item, array index */
);

/*===========================================================================
FUNCTION UISCRN_BIND_FIELD

DESCRIPTION
  This function gets a pointer to the text string. Then copies the src_txt_ptr
  to the text string from uiscrn_alloc_field. 

DEPENDENCIES
  It assume the length of the string to copy is UI_WIDE.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_field
(
  ui_field_item_enum_type field_value,            /* field item, array index */
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_BIND_FIELD2

DESCRIPTION
  This function gets a pointer to the text string. Then copies the src_txt_ptr
  to the text string from uiscrn_alloc_field. 

DEPENDENCIES
  It assume the length of the string to copy is 2*UI_WIDE.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_field2
(
  ui_field_item_enum_type field_value,            /* field item, array index */
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_BIND_FIELD3

DESCRIPTION
  This function gets a pointer to the text string. Then copies the src_txt_ptr
  to the text string from uiscrn_alloc_field. 

DEPENDENCIES
  It assume the length of the string to copy is 3*UI_WIDE.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_field3
(
  ui_field_item_enum_type field_value,            /* field item, array index */
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_BIND_FIELD4

DESCRIPTION
  This function gets a pointer to the text string. Then copies the src_txt_ptr
  to the text string from uiscrn_alloc_field. 

DEPENDENCIES
  It assume the length of the string to copy is 4*UI_WIDE.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_field4

(
  ui_field_item_enum_type field_value,            /* field item, array index */
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_BIND_FIELD_OFFSET

DESCRIPTION
  This function gets a pointer to the text string and adds the offset. 
  Then copies the src_txt_ptr to the text string from uiscrn_alloc_field. 

DEPENDENCIES
  It assume the length of the string to copy is UI_WIDE.

RETURN VALUE
  pointer to character for the text string of the field value passed in.
       
SIDE EFFECTS
  None
===========================================================================*/
extern char *uiscrn_bind_field_offset

(
  ui_field_item_enum_type field_value,            /* field item, array index */
  int offset,
  char *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_BIND_SOFTKEY

DESCRIPTION
  This function copies a giving string to the SOFTKEY text string.

DEPENDENCIES
  None.

RETURN VALUE
  None
       
SIDE EFFECTS
  None
===========================================================================*/
extern void uiscrn_bind_softkey
(
  byte *src_txt_ptr
);
/*===========================================================================
FUNCTION UISCRN_GET_SOFTKEY

DESCRIPTION
  This function copies a the SOFTKEY text string to the passed string.

DEPENDENCIES
  None.

RETURN VALUE
  None
       
SIDE EFFECTS
  None
===========================================================================*/
extern void uiscrn_get_softkey
(
  byte *dest_txt_ptr
);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_RINGER_ON_OFF_TXT

DESCRIPTION
  Displays or removes "-Ringer Off-" text.

DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_ringer_on_off_txt( void ) ;

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_ANIMATE_ELLIPSIS

DESCRIPTION
  Used to animate an ellipsis.  This function is called when the animate dot 
  timer expires.  Calls uiscrn_ellipsis with the correct field and parameters
  depending on what uiscrn_which_dot_animation is set to.

DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_animate_ellipsis( void );

#ifdef FEATURE_UI_ANIMATE_SEARCH
#error code not present
#endif

#ifdef FEATURE_UI_ANIMATE_CALL
#error code not present
#endif

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_START_ANIMATE_E911_SEARCH1

DESCRIPTION
  Starts the Looking for Emerbency Service ... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_start_animate_e911_srch1(void);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_STOP_ANIMATE_E911_SEARCH1

DESCRIPTION
  Stops the Looking for Emergency Service ... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_stop_animate_e911_srch1(void);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_START_ANIMATE_E911_SEARCH1

DESCRIPTION
  Starts the Continuing Emergency Search ... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_start_animate_e911_srch2(void);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_STOP_ANIMATE_E911_SEARCH1

DESCRIPTION
  Stops the Continuing Emergency Search ... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_stop_animate_e911_srch2(void);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_START_ANIMATE_E911_SEARCH1

DESCRIPTION
  Starts the Emergency service lost Looking... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_start_animate_e911_lost(void);

/* <EJECT> */
/*===========================================================================
FUNCTION UISCRN_STOP_ANIMATE_E911_SEARCH1

DESCRIPTION
  Stops the Emergency service lost Looking... animation.
  
DEPENDENCIES
  None
===========================================================================*/
extern void uiscrn_stop_animate_e911_lost(void);


#endif /* #ifndef UISCRN_H */

