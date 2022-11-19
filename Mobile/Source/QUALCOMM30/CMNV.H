#ifndef CMNV_H
#define CMNV_H
/*===========================================================================

          C A L L   M A N A G E R   N V   H E A D E R   F I L E

DESCRIPTION
  This header file contains all the definitions necessary to interface
  with CMNV.C


Copyright (c) 1991 - 1999 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:   O:/src/asw/COMMON/vcs/cmnv.h_v   1.1   08 Feb 2001 08:58:04   rotemc  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/07/01   RC      Added support for System Determination 2.0.
01/07/99   RC      Changes per ARM compiler.
11/24/98   RC      Initial release.

===========================================================================*/


#include "nv.h"        /* Interface to NV services */
#include "comdef.h"    /* Definition for basic types and macros */



/* <EJECT> */
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/


/* Type for CM NV data buffer. The only purpose of this type is to create
** a big enough buffer to hold data that is associated with CM NV items.
**
** Note that we could simply use nv_item_type, but this would be a waste
** of ~300 bytes of RAM.
**
** Note that Lint info 754 (local structure member not referenced) is 
** is expected here.
*/
typedef struct {  /*lint -e754 */

    union {
    
        nv_pref_mode_type   pref_mode;
        nv_pref_serv_type   pref_serv;
        nv_sys_pref_type    sys_pref;
        nv_sec_code_type    sec_code;
    
    } buffer;

    byte    padding[8];

} cmnv_data_s_type; 



/* <EJECT> */
/*===========================================================================

FUNCTION cmnv_init

DESCRIPTION
  Initialize the NV interface block.
  
  This function must be called before the NV interface block is used in
  any way, place or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/                                  
extern void cmnv_init( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmnv_write

DESCRIPTION
  Write an item to NV memory.
  
  Note that NV writes are done in asynchronous fashion; that is, upon 
  return of this function the write request is only being queued in the
  CM NV queue. The CM NV queue then services the request in the background.

DEPENDENCIES
  CM NV queue must have already been initialized with 
  cmnv_q_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern void cmnv_write(

    nv_items_enum_type    nv_item,      
        /* NV item to write */
        
    const nv_item_type    *data_ptr
        /* pointer to data associated with the NV item */
);
                                    


/* <EJECT> */
/*===========================================================================

FUNCTION cmnv_write_wait

DESCRIPTION
  Write an item to the NV memory in a synchronous fashion; that is, this
  function only returns  after the NV write is done being serviced by the
  NV task.
  
  This function is only expected to be used during power-up initialization.
  During operation cmnv_write() must be used to write items to NV.

DEPENDENCIES
  mc_task_start() must have already been called.

  This function is only expected to be used during power-up initialization.
  During operation cmnv_write() must be used to items to NV.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmnv_write_wait(

    nv_items_enum_type    nv_item,      
        /* NV item to write */
        
    nv_item_type          *data_ptr
        /* pointer to data associated with the NV item */
);



/* <EJECT> */
/*===========================================================================

FUNCTION cmnv_q_write_done_cb

DESCRIPTION
  Callback function for completion of NV write operations

DEPENDENCIES
  CM NV queue must have already been initialized with 
  cmnv_q_init().

RETURN VALUE
  none

SIDE EFFECTS
  Dequeue item from the CM NV queue.

===========================================================================*/
extern void cmnv_q_write_done_cb( void );



/* <EJECT> */
/*===========================================================================

FUNCTION cmnv_read_wait

DESCRIPTION
  Read an item to NV memory.
  
  Note that NV reads are done in synchronous fashion; that is, this function
  returns only after the NV read is done being serviced by the NV task.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmnv_read_wait(

    nv_items_enum_type  nv_item,      
        /* NV item to read */
        
    nv_item_type        *data_ptr
        /* pointer to buffer to place data associated with the NV item */
);


#endif /* CMNV_H */
