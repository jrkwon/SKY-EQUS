#ifndef __FS_AM_H__
#define __FS_AM_H__
/*===========================================================================

                  E M B E D D E D   F I L E   S Y S T E M

DESCRIPTION

  Definitions necessary for the Embedded Filesystem Access Method Manager.

Copyright (c) 2000 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/fs_am.h_v   1.1   29 Jan 2001 12:47:50   davidb  $

when      who     what, where, why
--------  ---     ----------------------------------------------------------
01-29-01  dlb     Support card generation tracking.
11-09-00  pg      Initial version for common archive.
08-22-00  dlb     Add fs_am_cleanup_after_command.
05-12-00  dlb     Initial version.

===========================================================================*/

#include "comdef.h"
#include "fs_ops.h"
#include "fs.h"
#include "fsi.h"

/*===========================================================================
FUNCTION FS_ADD_ACCESS_DIR

DESCRIPTION
  "Mount" the given mount point using the specified access method.  The
  mountpoint should specify the directory that the mount should appear at.
  It should terminate with a slash.  Since normal EFS filenames do not have
  leading slashes, it also shouldn't have a trailing slash.

  access_method is the name of the access method used.  It is a string.

  prefix is a (possibly empty, but not NULL) string that will be prefixed
  onto the used filename when calling the access method.  For example, if
  you fs_add_access_dir ("subdir1/haha/", "am1", "a:/data/", TRUE), then
  calling fs_open with the filename of "subdir1/haha/mine/file.dat" will
  invoke the "am1" access method with a filename of
  "a:/data/mine/file.dat".

  Persistent indicates whether this access mount will survive a reboot.

DEPENDENCIES
  The FS task must be started.

RETURN VALUE
  Returns FS_OKAY_S if the mountpoint could be added, otherwise returns an
  error status indicator.

SIDE EFFECTS
  Blocks interrupts while adding the access method.

===========================================================================*/
fs_status_type fs_add_access_dir
(
  const char *mountpoint,               /* Mountpoint to add. */
  const char *access_method,            /* Name of access method to use. */
  const char *prefix,                   /* Prefix given to access method. */
  boolean     persistent                /* Does this mountpoint survive
                                           reboots? */
);


/*===========================================================================
FUNCTION FS_REMOVE_ACCESS_DIR

DESCRIPTION
  Remove a previously added access dir (added with fs_add_access_dir).  If
  the mount was persistent, it will be persistently removed.

DEPENDENCIES
  The mount must have been previously added.
  The FS task must be started.

RETURN VALUE
  Returns FS_OKAY_S if the mountpoint was removed, otherwise returns an
  error status indicator.

SIDE_EFFECTS
  Blocks interrupts while adding the access method.

===========================================================================*/
fs_status_type fs_remove_access_dir
(
  const char *mountpoint                /* Mountpoint to remove. */
);

 
/*===========================================================================
FUNCTION FS_ADD_ACCESS_FILE

DESCRIPTION
  "Mounts" a given file using a given access method.  Only matches the
  given file from the user.

  access_method is the name of the access method used.  It is a string.

  access_path is the file name given to the access method.

  Persistent indicates whether this access mount will survive a reboot.

DEPENDENCIES
  The FS task must be started.

RETURN VALUE
  Returns FS_OKAY_S if the mountpoint could be added, otherwise returns an
  error status indicator.

SIDE EFFECTS
  Blocks interrupts while adding the access method.
  
  NOT IMPLEMENTED YET

===========================================================================*/
fs_status_type fs_add_access_file
(
  const char *mountfile,                /* Name mounted file appears
                                           visible at. */
  const char *access_method,            /* Name of access method to use. */
  const char *access_path,              /* File name given to access
                                           method. */
  boolean     persistent                /* Does this mountpoint survive
                                           reboots? */
);


/*===========================================================================
FUNCTION FS_REMOVE_ACCESS_FILE

DESCRIPTION
  Remove a previously added access file (added with fs_add_access_dir).  If
  the mount was persistent, it will be persistently removed.

DEPENDENCIES
  The mount must have been previously added.
  The FS task must be started.

RETURN VALUE
  Returns FS_OKAY_S if the mountpoint was removed, otherwise returns an
  error status indicator.

SIDE_EFFECTS
  Blocks interrupts while adding the access method.

===========================================================================*/
fs_status_type fs_remove_access_file
(
  const char *mountfile                 /* Name of mounted file. */
);


/**********************************************************************
 * Lower level interface.

   All of the routines and defintions below this point are intended
   for use by access method implementations.  Users of the filesystem
   should never call these directly.
**********************************************************************/
#define FS_INVALID_HANDLE 0xFFFF

/* Returns True if the operation worked, False if it could not be done (not
 * enough handles or string space. */
boolean fs_am_allocate_handle
  (const char *filename, fs_handle_type *handle, const char **new_filename);

void fs_am_register_handle (fs_handle_type am_handle,
                            fs_handle_type down_handle);

void fs_am_free_handle (fs_handle_type);

fs_handle_type fs_am_lookup_handle (fs_handle_type);

typedef void (*fs_am_ops_cmd) (fs_cmd_type *cmd_ptr);

fs_am_ops_cmd fs_am_get_ops_cmd (fs_handle_type);

void fs_am_init (void);

void fs_am_fixup_list (char *buffer, int length, dword *used,
    fs_handle_type handle);

void fs_am_fixup_enum (fs_enum_data_type *data,
                       fs_enum_iterator_type *iterator);

void fs_am_cleanup_after_command (fs_cmd_type *op);

/* Generation counting. */
void fs_am_set_generation (fs_handle_type, dword);
dword fs_am_get_generation (fs_handle_type);

#endif /* __FS_AM_H__ */
