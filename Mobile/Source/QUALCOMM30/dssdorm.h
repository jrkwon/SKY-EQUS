#ifndef DS_DORM_H
#define DS_DORM_H

/*===========================================================================

  D A T A  S E R V I C E S  S O C K E T  D O R M A N T  H E A D E R  F I L E
                   
DESCRIPTION

 The Data Services Sockets dormancy header file.  This contains declarations
 of functions used in implementing dormancy for sockets.
 
 This file is included by data services code and sockets code.  The 
 sockets application should NOT include this file or use the functions
 shown here.

EXTERNALIZED FUNCTIONS

  dssdorm_set_flow_ctrl()
  Sets the flow control variable.  TRUE= application is prevented from 
  doing write()'s and connect()'s.  FALSE = application can go ahead.
  
  dssdorm_get_flow_ctrl()
    Returns the current flow control setting.
    
  dssdorm_get_dormant_mode()
    Returns the dormant state.
    
  dssdorm_set_dormant_mode()
    Sets the dormant mode variable.  
    DSSDORM_MODE_REORIG = We are dormant, and should re-originate on write()'s
    and connect()'s.  
    DSSDORM_MODE_TRANSIENT = Traffic Channel is in the process of coming up.
    DSSDORM_MODE_NOT_DORM = Not Dormant.
    
  dssdorm_reorigination()
    Does two things.  If flow control is asserted, then returns an error to
    the caller, with EWOULDBLOCK.  If the mode is dormant, then causes a
    re-origination.
    
  dssdorm_active()
   Returns TRUE if either in dormant mode(DSSDORM_MODE_REORIG or 
   DSSDORM_MODE_TRANSIENT) or flow controlled, otherwise  
   returns FALSE.
 


Copyright (c) 1998, 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/dssdorm.h_v   1.0   23 Oct 2000 17:32:16   jeffd  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/22/99   rc      Incorporated code review changes and comments.
7/6/99     rc/ak   Initial version 
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_DS_SOCKETS
#include "comdef.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
Enum to check different states of dormancy.
---------------------------------------------------------------------------*/

typedef enum {
      DSSDORM_MODE_REORIG = 0,  /* Should Re-originate on next write or    */
                                /* connect                                 */
      DSSDORM_MODE_TRANSIENT,   /* Trying to bring traffic channel up      */
      DSSDORM_MODE_NOT_DORM     /* Re-origination has either failed or     */
                                /* succeeded but no longer dormant         */
}dssdorm_mode_enum_type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION DSSDORM_SET_FLOW_CTRL

DESCRIPTION
  Sets the flow control variable.  TRUE= application is prevented from 
  doing write()'s and connect()'s.  FALSE = application can go ahead.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssdorm_set_flow_ctrl
(
  boolean flow_ctrl                                /* sockets flow control */
);



/*===========================================================================

FUNCTION DSSDORM_GET_FLOW_CTRL

DESCRIPTION
  Returns the current flow control setting

DEPENDENCIES
  None.

RETURN VALUE
  TRUE = application is flow controlled. 
  FALSE = applications is not flow controlled.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean dssdorm_get_flow_ctrl(void);



/*===========================================================================

FUNCTION DSSDORM_GET_DORMANT_MODE

DESCRIPTION

Returns the dormant state.

DEPENDENCIES
  None.

RETURN VALUE
  Dormant state.

SIDE EFFECTS
  None.

===========================================================================*/
extern dssdorm_mode_enum_type dssdorm_get_dormant_mode(void);


/*===========================================================================

FUNCTION DSSDORM_SET_DORMANT_MODE

DESCRIPTION
  Sets the dormant mode variable.  
  DSSDORM_MODE_REORIG = We are dormant, and should re-originate on write()'s
  and connect()'s.  
  DSSDORM_MODE_TRANSIENT = Traffic Channel is in the process of coming up.
  DSSDORM_MODE_NOT_DORM = Not Dormant.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void dssdorm_set_dormant_mode
(
  dssdorm_mode_enum_type dormant_mode           /* dormant state           */
);


/*===========================================================================

FUNCTION DSSDORM_REORIGINATION

DESCRIPTION
  Does two things.  If flow control is asserted, then returns an error to
  the caller, with EWOULDBLOCK.  If the mode is dormant, then causes a
  re-origination.

DEPENDENCIES
  errno value should be non-null.

RETURN VALUE
  If application is flow controlled or re-originating or in the
  tranisent state return DSS_ERROR else return DSS_SUCCESS.

SIDE EFFECTS
  None.

===========================================================================*/
sint15 dssdorm_reorigination
(
  sint15 *errno                                   /* error condition value */
);



/*===========================================================================

FUNCTION DSSDORM_ACTIVE

DESCRIPTION
   Returns TRUE if either in dormant mode(DSSDORM_MODE_REORIG or 
   DSSDORM_MODE_TRANSIENT) or flow controlled, otherwise  
   returns FALSE.
   
DEPENDENCIES
  None.

RETURN VALUE
  TRUE -  If flow controlled or in dormant mode.
  FALSE - Otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean dssdorm_active(void);


#endif /* FEATURE_DS_SOCKETS */

#endif /* DS_DORM_H */

