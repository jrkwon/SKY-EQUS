#ifndef UIDATA_H
#define UIDATA_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


          U S E R   I N T E R F A C E   D A T A

GENERAL DESCRIPTION
  Data services debug screen and call status display functions.

  Copyright (c) 1997,2000 by QUALCOMM, Inc.
  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/MSM5000/VCS/uiudata.h_v   1.2   29 Mar 2000 18:11:00   hdowlat  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/00   cs      Added FEATURE_DS_IS2000 support.
12/11/97   ldg     First revision.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "target.h"
#include "customer.h"
#ifdef FEATURE_DS
#include "comdef.h"

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_DS_IS2000
#define MAX_NO_SCH_RATES    5
#define DS_DATA_RATE_9_6    96     /* 9.6 */
#define DS_DATA_RATE_19_2   192    /* 19.2 */
#define DS_DATA_RATE_28_8   288    /* 28.8 */
#define DS_DATA_RATE_48     480    /* 48.0 */
#define DS_DATA_RATE_86_4   864    /* 86.4 */
#define DS_DATA_RATE_163_2  1632   /* 163.2 */

extern const byte dec_sch_rate_table[MAX_NO_SCH_RATES][2];
extern const byte enc_sch_rate_table[MAX_NO_SCH_RATES][2];
#endif /* FEATURE_DS_IS2000 */

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION UIDATA_CALL_STATUS

DESCRIPTION:
  Update the CDMA data call status display ("Status: T R ")
  with the current T/R rates.

DEPENDENCIES:
  1) For this function to actually update the status display ui_data2_txt_ptr
     must NOT be NULL. 
  2) The RX and TX bar graph level calculations round up.

RETURNS:
  Nothing
===========================================================================*/
extern void uidata_call_status( void );

#ifdef FEATURE_DS_DEBUG_UI

/* <EJECT> */
/*===========================================================================
FUNCTION UIDATA_DEBUG_SCREEN

DESCRIPTION
  Update the DATA DEBUG screen for the mobile, which when turned on
  shows data services call and protocol state information.

DEPENDENCIES
  ui.isdatadebug tells us whether the data debug screen should show.

RETURN VALUE
  None.

SIDE EFFECTS
  May obscure other things on the screen.

===========================================================================*/
extern void uidata_debug_screen(
  void
);

/* <EJECT> */
/*===========================================================================
FUNCTION UIDATA_DEBUG_SCREEN_NEXT

DESCRIPTION
  Scroll to the next part of the DATA DEBUG screen.

DEPENDENCIES

RETURN VALUE
  Nothing

SIDE EFFECTS
  May obscure other things on the screen

===========================================================================*/
extern void uidata_debug_screen_next(
  void
);

#endif /* FEATURE_DS_DEBUG_UI */

/* <EJECT> */
#ifdef FEATURE_DS_QNC
/*===========================================================================
FUNCTION UIDATA_TOGGLE_QNC

DESCRIPTION
  This function toggles Quick Net Connect services on or off by reading
  a setting from NV, toggling it, writing it back to NV, and signalling
  the DS task.

DEPENDENCIES
  none

RETURN VALUE
  Boolean QNC state: TRUE if enabled, FALSE if disabled.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean uidata_toggle_qnc(
  void
);
#endif /* FEATURE_DS_QNC */

#endif /* FEATURE_DS */
#endif /* ifndef UIDATA_H */
