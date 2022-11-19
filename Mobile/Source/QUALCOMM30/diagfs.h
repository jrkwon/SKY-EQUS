/*==========================================================================

                  D I A G N O S T I C    T A S K   
    
               I N T E R N A L   H E A D E R    F I L E

DESCRIPTION
  Header file for DIAG Embedded Filesystem (EFS) support.
 
Copyright (c) 1999-2000 by QUALCOMM, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

 $Header:   L:/src/asw/MSM5000/VCS/diagfs.h_v   1.3   12 Apr 2000 02:41:56   ldefauw  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/11/00   lad     Added diag_fs_iter prototype.
02/28/00   lad     Changed functions to use diagbuf allocation services.
02/02/00   lad     Added prototypes for full DIAG EFS support.
07/23/99   lad     Added rev 2 of EFS support.
03/26/99   lad     Created file for EFS support.  Write and MKDIR only.

===========================================================================*/

/* This file only used if EFS is on. */
#ifdef FEATURE_EFS


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "comdef.h"
#include "customer.h"
#include "diagpkt.h"



/*===========================================================================

FUNCTION DIAG_FS_MKDIR

DESCRIPTION
  This function handles FS "Create Directory" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_mkdir (
  diag_fs_mkdir_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                     /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_DISP_DIRS

DESCRIPTION
  This function handles FS "Display Directory List" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_disp_dirs (
  diag_fs_disp_dirs_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                         /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_DISP_FILES

DESCRIPTION
  This function handles FS "Display File List" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_disp_files (
  diag_fs_disp_files_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                          /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_RMDIR

DESCRIPTION
  This function handles FS "Remove Directory" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_rmdir (
  diag_fs_rmdir_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                     /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_GET_ATTRIB

DESCRIPTION
  This function handles FS "Get File Attributes" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_get_attrib (
  diag_fs_get_attrib_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                          /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_SET_ATTRIB

DESCRIPTION
  This function handles FS "Set File Attributes" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_set_attrib (
  diag_fs_set_attrib_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                          /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_READ

DESCRIPTION
  This function handles FS read commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_read (
  diag_fs_read_req_type *req_ptr,    /* pointer to DIAG FS request packet */
  word pkt_len                       /* length of request packet          */
);


/*===========================================================================

FUNCTION DIAG_FS_WRITE

DESCRIPTION
  This function handles FS write commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_write (
  diag_fs_write_req_type *req_ptr,   /* pointer to DIAG FS request packet */
  word pkt_len                       /* length of request packet          */
);


/*===========================================================================

FUNCTION DIAG_FS_RMFILE

DESCRIPTION
  This function handles FS "Remove File" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_rmfile (
  diag_fs_rmfile_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                      /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_REMOTE_LINK

DESCRIPTION
  This function handles FS "Remote File Link" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_remote_link (
  diag_fs_remote_link_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len                           /* Length of this packet      */
);


/*===========================================================================

FUNCTION DIAG_FS_ITER

DESCRIPTION
  This function handles FS "Iterate listings" commands in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_iter (
  diag_fs_iter_dirs_req_type *req_ptr, /* pointer to request packet  */
  word pkt_len,                        /* Length of this packet      */
  diag_fs_op_enum_type op_type         /* listing to iterate         */
);


/*===========================================================================

FUNCTION DIAG_FS_SPACE_AVAIL

DESCRIPTION
  This function handles FS "Space Available" command in DIAG.
  
DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
extern diag_rsp_type * diag_fs_space_avail (void);

#endif /* FEATURE_EFS */

/* Restore error reporting for these symbols */
/*lint +esym(715,pkt_len,req_ptr) */

