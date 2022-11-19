#ifndef ACPRXMSG_H
#define ACPRXMSG_H

/*===========================================================================

                          A C P R X M S G

DESCRIPTION
  This header file contains the definitions necessary to interface with the 
  ACPRXMSG unit of the ACPRX sub-task.


       Copyright (c) 1992 by QUALCOMM, Incorporated  All Rights Reserved.
Copyright (c) 1999 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/common/shared/cp/vcs/acprxmsg.h_v   1.0   Aug 11 2000 11:40:56   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/28/92   jjw     creation

===========================================================================*/


#include "comdef.h"
#include "acprx.h"



/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/*===========================================================================

                   PUBLIC FUNCTION DELCARATIONS

===========================================================================*/






/*==========================================================================
  
   FUNCTION ACPRXMSG_SETMODE
  
   DESCRIPTION 
     This function informs the ACPRX message processing unit that the channel
     mode has been set to the passed mode. This function will reset the 
     message building process.
       
   DEPENDENCIES 
     None

   RETURN VALUE 
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxmsg_setmode
(
  acp553_mode_type   mode                       /* new channel mode value  */
);

/*==========================================================================
  
   FUNCTION ACPRXMSG_START_BUILDING
  
   DESCRIPTION 
     This function initializes the message building process for the ACPRX 
     sub-task. The passed commmand pointer provides the specified msg_type 
     and other possible parameters (min1/min2) for message building. These
     values are saved for use during the message building process.
       
   DEPENDENCIES 
     None

   RETURN VALUE 
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxmsg_start_building
(
  acprx_cmd_type *cmd_ptr  /* pointer to command that contains START_RX_MSG
                              command parameters                           */
);

/*==========================================================================
  
   FUNCTION ACPRXMSG_STOP_BUILDING
  
   DESCRIPTION 
     This function resets and disables the message building process for the 
     ACPRX sub-task. 
       
   DEPENDENCIES 
     None

   RETURN VALUE 
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxmsg_stop_building( void);

/*==========================================================================

   FUNCTION ACPRXMSG_BUILD

   DESCRIPTION
     This is the top level function responsible for building forward channel
     messages for the analog call processing sub-system ACPRX. This includes
     getting words from the ACPFM interface, determining the msg type,building
     the message from the received words and reporting the message to the 
     requesting task via the stored command report function. The message 
     building process is reset when an error condition occurs or the MIN1/MIN2
     fields do not match during mobile station control message processing.

   DEPENDENCIES
     If the message words received are to be reported to a requesting task
     'acprxmsg_start_building' must be invoked with the proper command 
     parameters to prepare for the building of messages.

   RETURN VALUE 
     None

   SIDE EFFECTS
     None

===========================================================================*/

extern void acprxmsg_build
(
  void
);


#endif
