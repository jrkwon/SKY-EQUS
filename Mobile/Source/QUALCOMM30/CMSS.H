#ifndef CMSS_H
#define CMSS_H
/*===========================================================================

C A L L   M A N A G E R   S E R V I N G   S Y S T E M   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMSS.C

Copyright (c) 1991 - 1999 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   L:/src/asw/COMMON/vcs/CMSS.H_v   1.0   Aug 18 2000 11:42:04   lpetrucc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/09/99   SH      Added p_rev and is_registered to support data application
05/31/99   SH      Added CM 2.0 fetures
01/07/99   RC      Changes per ARM compiler.
11/24/98   RC      Initial release.

===========================================================================*/

#include "comdef.h"    /* Definition for basic types and macros */
#include "cm.h"        /* External interface to cm.c */
#include "cmi.h"       /* Internal interface to cm.c */

/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

                                                
                                                
 /* Enumeration of serving system info update action*/
typedef enum cmss_update_act_e {

    CMSS_INFO_UPDATE_ACT_NONE=-1,        /* FOR INTERNAL USE OF CM ONLY! */

    CMSS_INFO_UPDATE_N_TO_D,
    CMSS_INFO_UPDATE_A_TO_D,
    CMSS_INFO_UPDATE_N_TO_A,
    CMSS_INFO_UPDATE_D_TO_A,
    
    CMSS_INFO_UPDATE_A_TO_A,
    CMSS_INFO_UPDATE_D_TO_D,

    CMSS_INFO_UPDATE_A_TO_N,
    CMSS_INFO_UPDATE_D_TO_N,



       
    CMSS_INFO_UPDATE_ACT_MAX             /* FOR INTERNAL USE OF CM ONLY! */

} cmss_update_act_e_type;
                                               
                                                                                             
/* Serving System type - holds all the information that is associated
** with a Serving System.
*/
typedef struct cmss_s  {

  cm_init_mask              init_mask;          /* Indicate whether this object was properly initialized */
  cm_srv_state_e_type       srv_state;          /* analog or digital */
  boolean                   is_registered;      /* is mobile registered on the current system */
  cm_band_class_e_type      band_class;         /* cellular or PCS */
  cm_block_or_system_e_type block_or_system;    /* PCS frequency block: A to F Cellular System A or B*/   
  word                      changed_fields;     /* bit mask indicating serving system fields changed on last update*/
  word                      sid;                /* 15 bit SID field */
  word                      nid;                /* 16 bit NID field */
  word                      reg_zone;           /* 12 bit REG_ZONE */
  byte                      packet_zone;        /* 8 bit IS-95B PACKET_ZONE */
  byte                      bs_p_rev;           /* 8 bit base station p_rev */
  byte                      p_rev_in_use;       /* 8  bit p_rev in use the lessor of  
                                                   BS p_rev and mobile p_rev */
  
} cmss_s_type;



/* <EJECT> */
/*===========================================================================
                          FUNCTION PROTOTYPES
===========================================================================*/


/*===========================================================================

FUNCTION cmss_ptr

DESCRIPTION
  Return a pointer to the one and only phone object.

  The serving system object is responsible for:
  1. Processing MC serving system related notifications.
  2. Notifying the client list of serving system events.
  
DEPENDENCIES
  none
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cmss_s_type  *cmss_ptr( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmss_init

DESCRIPTION
  Initializes the serving system object.

  This function must be called before the serving system object 
  is being used, in any way, place, or form.

DEPENDENCIES
  none
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_init( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmss_info_get

DESCRIPTION
  Copy the current serving system state information into a specified buffer.

DEPENDENCIES
  Serving System object must have already been initialized with
  cmss_init().

RETURN VALUE
  none
  
SIDE EFFECTS
  none

===========================================================================*/
extern boolean cmss_info_get(

    cm_ss_info_s_type    *ss_info_ptr
        /* Copy serving system state info into this buffer */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmss_mc_rpt_proc

DESCRIPTION
  Process MC reports
   

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmss_mc_rpt_proc(

    const cm_cmd_s_type    *cmd_ptr
        /* Pointer to a CM command */
);

/*===========================================================================

FUNCTION cmss_client_cmd_proc

DESCRIPTION
  Process clients' ss commands

DEPENDENCIES
  SS object must have already been initialized with
  cmss_init().
  
RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_client_cmd_proc(

    cm_cmd_s_type  *cmd_ptr
        /* pointer to a CM command */
);

/* <EJECT> */
/*===========================================================================

FUNCTION cmss_timer_proc

DESCRIPTION
  Process timer events.

DEPENDENCIES
  Serving system object must have already been initialized with
  cmss_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmss_timer_proc(

    cm_timer_event_type    timer_event
        /* Indicate specific timer event.
        **
        ** Note that currently timer event is not being used */
        /*lint -esym(715,timer_event) */
);


#endif /* CMSS_H */


