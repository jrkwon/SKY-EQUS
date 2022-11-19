#ifndef FS_H
#define FS_H
/*==========================================================================

                   E M B E D D E D   F I L E   S Y S T E M

DESCRIPTION

  This header file contains definitions necessary for tasks to 
  interface with the Embedded File System.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   O:/src/asw/COMMON/vcs/fs.h_v   1.2   29 Jan 2001 12:46:48   davidb  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11-09-00   pg      Initial version for common archive.
11-06-00   dlb     Added FS_NO_MEDIA_S, and FS_MEDIA_CHANGED_S.
05-12-00   dlb     Added FEATURE_EFS_ACCESS_METHODS and code.
05-12-00   dlb     Added fs_enum_finish().
04-25-00   pg      Fixed the problem that fs_test() could not properly
                   distinguish a file from a directory:
                   Replaced function fs_test() with fs_nametest(), and
                   defined a MACRO fs_test to keep the old interface.
04-11-00   pg      Added status for exceeding maximum remote links allowed.
03-24-00   pg      Added fs_enum_prev() function and support for it.
03-16-00   pg      Added fs_seek_read(), fs_seek_write() functions and 
                   supports for them.
02-23-00   rp      Add response message for ANY operation.
02-14-00   pg      Changed struct fs_enum_iterator_type -
                   added word remote_file_processed_index -
                   to support fs_enum_next() on remote links.
02-02-00   rp      Moved new status values associted with fs_enum functions
                   to end of fs_status_type so all existing status values
                   remain in sync with diag.
01-31-00   pg      Added fs_enum_init(), fs_enum_next() functions and 
                   supports for them.
01-21-00   pg      Added fs_rename function and support for it.
12-16-99   rp      Added fs_remove_remote_links function. 
08-13-99   rp      Picked up Lee's latest fixes.  
06-22-99   ls      Added status for full Open Files Table.
05-24-99   ls      Added SYS_PERMANENT attribute.
05-24-99   ls      Added system configuration parameters for client use.
05-20-99   ls      Added fmemset support.
04-25-99   ls      Added space_erased function.
04-22-99   ls      Removed enclosing ifdef.
04-21-99   ls      Version 2.2 release.
04-03-99   ls      Changed interface per Version 2.1 specification.
03-04-99   ls      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "fs_parms.h"



/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed to interface with the embedded file system.

===========================================================================*/


/* Operation status values */
/* Do not move these values around as they must be in sysn with diag */
typedef enum {
  FS_OKAY_S,                /* Good status                                 */
  FS_FAIL_S,                /* Low-level operation failed                  */
  FS_BUSY_S,                /* Operation is queued or in progress          */
  FS_FILE_NOT_OPEN_S,       /* File needs to be opened for this operation  */
  FS_FILE_OPEN_S,           /* File needs to be closed for this operation  */
  FS_FILE_ALREADY_EXISTS_S, /* File already exists                         */
  FS_NONEXISTENT_FILE_S,    /* File doesn't exist                          */
  FS_DIR_ALREADY_EXISTS_S,  /* User directory already exists               */
  FS_NONEXISTENT_DIR_S,     /* User directory doesn't exist                */
  FS_DIR_NOT_EMPTY_S,       /* User directory not empty for remove         */
  FS_UNKNOWN_OPERATION_S,   /* Client specified an unknown operation       */
  FS_ILLEGAL_OPERATION_S,   /* Client specified an invalid operation       */
  FS_PARAMETER_ERROR_S,     /* Client supplied a bad parameter value       */
  FS_BAD_FILE_NAME_S,       /* Client specified invalid file/directory name*/
  FS_BAD_FILE_HANDLE_S,     /* Client specified invalid file handle        */
  FS_BAD_SEEK_POS_S,        /* Client specified an invalid SEEK position   */
  FS_BAD_TRUNCATE_POS_S,    /* Client supplied an invalid truncate position*/
  FS_FILE_IS_REMOTE_S,      /* Operation invalid for remote files          */
  FS_INVALID_CONF_CODE_S,   /* Invalid confirmation code specified         */
  FS_EOF_S,                 /* End of file reached                         */
  FS_MORE_S,                /* More records exist to be processed          */
  FS_GC_IN_PROGRESS_S,      /* Garbage Collection in progress              */
  FS_SPACE_EXHAUSTED_S,     /* Out of file system space                    */
  FS_BLK_FULL_S,            /* File block is full                          */
  FS_DIR_SPACE_EXHAUSTED_S, /* Out of Master Directory space               */
  FS_FBL_POOL_EXHAUSTED_S,  /* Out of File Block List free pool space      */
  FS_OPEN_TABLE_FULL_S,     /* Out of open-file table slots                */
  FS_INVALID_ITERATOR_S,    /* Invalid enumeration iterator control struct */
  FS_ENUM_DONE_S,           /* Enumeration operation completed             */
  FS_RMLINK_EXHAUSTED_S,    /* Maximum number of remote links reached      */
  FS_NO_MEDIA_S,            /* Removable media not inserted                */
  FS_MEDIA_CHANGED_S        /* Media has been changed since opened         */
} fs_status_type;


/* System configuration parameters */

/* Maximum length of a file name */
#define  FS_FILENAME_MAX_LENGTH  FS_FILENAME_MAX_LENGTH_P

/* Directory separator character */
#define  FS_DIRECTORY_CHAR       FS_DIRECTORY_CHAR_P

/* Maximum number of files in the system */
#define  FS_MAX_NUM_FILES        FS_MAX_NUM_FILES_P

/* Maximum number of simultaneously "open" files in the system */
#define  FS_MAX_OPEN_FILES       FS_MAX_OPEN_FILES_P

/* Maximum number of outstanding file space reservations */
#define  FS_MAX_RESERVATIONS     FS_MAX_RESERVATIONS_P

/* Maximum number of command packets to the file system task */
#define  FS_MAX_CMD_PACKETS      FS_MAX_CMD_PACKETS_P


/* File handle type definition */
typedef  word   fs_handle_type;

/* A Null file handle */
#define  FS_NULL_HANDLE  0


/* File SEEK operation origin */
typedef enum {
  FS_SEEK_SET,            /* Start of file position                        */
  FS_SEEK_CURRENT,        /* Current file position                         */
  FS_SEEK_EOF             /* End of file position                          */
} fs_seek_origin_type;


/* File OPEN operation access options */
typedef enum {
  FS_OA_CREATE,           /* Create new file                               */
  FS_OA_APPEND,           /* Open a file for append access                 */
  FS_OA_READONLY,         /* Open a file for read-only access              */
  FS_OA_READWRITE,        /* Open a file for read-write access             */
  FS_OA_TRUNCATE,         /* Truncates file contents                       */
  FS_OA_TEST              /* Reserved for file system testing              */
} fs_access_option_type;


/* File OPEN operation clean-up options */
typedef enum {
  FS_OC_CLOSE     = 0x00, /* Close file as-is                              */
  FS_OC_DISCARD   = 0x01, /* Delete file and remove it from directory      */
  FS_OC_TRUNCATE  = 0x02, /* Truncate file from specified position         */
  FS_OC_REVERT    = 0x03  /* Revert to last checkpointed version           */
} fs_cleanup_option_type;


/* File OPEN operation buffering options */
typedef enum {
  FS_OB_PROHIBIT,         /* Prohibit file buffering                       */
  FS_OB_ALLOW             /* Allow file buffering                          */
} fs_buffering_option_type;


/* File SET ATTRIBUTE MASK operation file attribute mask values */
typedef enum {
  FS_FA_UNRESTRICTED =0x00FF, /* No file access restrictions               */
  FS_FA_READONLY     =0x00BF, /* File cannot be opened for update access   */
  FS_FA_PERMANENT    =0x007F, /* File cannot be removed nor truncated      */
  FS_FA_SYS_PERMANENT=0x0037, /* File persists across system reformats     */
  FS_FA_REMOTE       =0x001F  /* Remote file (resides outside the space    */
} fs_attribute_mask_type;     /*   managed by the file system)             */


/* File Creation Date */
typedef  dword  fs_file_date_type;


/* File position for TRUNCATE, SEEK, and TELL */
typedef  dword  fs_file_position_type;


/* Extended file Open parameters */

/* OPEN-Create parameters */
typedef struct {
  fs_cleanup_option_type      cleanup_option;   /* bad powerdown recovery */
  fs_buffering_option_type    buffering_option; /* buffering allowed? */
  fs_attribute_mask_type      attribute_mask;   /* attribute mask */
  dword                       base_address;     /* For remote files only */
} fs_open_create_type;

/* OPEN-Truncate parameters */
typedef struct {
  fs_file_position_type       position;         /* position to truncate at */
} fs_open_truncate_type;

/* OPEN extended parameters for "create" and "truncate" */
typedef union {
  fs_open_create_type         create;            
  fs_open_truncate_type       truncate;
} fs_open_xparms_type;


/* Type for requesting reservation of file system space for normal files */
typedef struct {
  dword                       amount;
} fs_rsvr_normal_type;

/* Type for reserving file system space for revertable files */
typedef struct {
  dword                       amount;
  dword                       fsize;
} fs_rsvr_revert_type;

typedef union {
  fs_rsvr_normal_type         normal_file_rsvr;
  fs_rsvr_revert_type         revertable_file_rsvr;
} fs_rsvr_request_type;

/* Type for reservation confirmation codes */
typedef struct {
  dword                       code;
  dword                       count;
} fs_conf_code_type;

/* A Null confirmation code */
#define  FS_NULL_CONF_CODE  ((dword)0)

/* Starting offset of user data area within system-permanent files       */
#define  FS_SYS_PERM_DATA_OFFSET       0x78

/* Starting offset of owner ID field within system-permanent files       */
#define  FS_SYS_PERM_OWNER_ID_OFFSET   FSI_BLK_OFFSET_OWNER_ID

/* Maximum size in bytes of user data area within system-permanent files */
#define  FS_SYS_PERM_DATA_SIZE         128

/* Specifies whether to enumerate files or directories */
typedef enum {
  FS_ENUM_FILES,                           /* Enumerate files only */
  FS_ENUM_DIRECTORIES                      /* Enumerate directories only */
} fs_enum_type;

/* Control block for file and directory enumeration services */
typedef struct {
  fs_enum_type                enum_kind;   /* File or subdir enumeration */
  dword                       fsequence;   /* Current file sequence number */
  word                        remote_file_processed_index;   
                                           /* Number of remote linked processed */ 
  byte                        length;      /* Rootname length */
#ifdef FEATURE_EFS_ACCESS_METHODS
#error code not present
#endif
  byte                        rootname[FS_FILENAME_MAX_LENGTH];
} fs_enum_iterator_type;

/* Returned data from ENUM_NEXT and ENUM_PREV operation */
typedef struct {
  byte                        fullname_length;
  byte                        dirname_length;
  char                        fullname[FS_FILENAME_MAX_LENGTH];
  fs_attribute_mask_type      attributes;
  fs_file_date_type           creation_date;
  fs_buffering_option_type    buffering_option;
  fs_cleanup_option_type      cleanup_option;
  dword                       logical_size;
  dword                       physical_size;
} fs_enum_data_type;

/* Specifies whether to nametest a file or a directory */
typedef enum {
  FS_TEST_FILE_OR_DIR,                     /* Nametest a file or a dir */
  FS_TEST_FILE,                            /* Nametest a file only */
  FS_TEST_DIR                              /* Nametest a directory only */
} fs_nametest_type;

/* Response message from OPEN operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  fs_handle_type              handle;      /* Returned file handle */
} fs_open_rsp_type;

/* Response message from CLOSE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_close_rsp_type;

/* Response message from READ operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* Number of bytes read */
} fs_read_rsp_type;

/* Response message from WRITE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* Number of bytes written */
} fs_write_rsp_type;

/* Response message from SET_ATTRIBS operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_set_attribs_rsp_type;

/* Response message from GET_ATTRIBS operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  fs_attribute_mask_type      attributes;       /* Attribute mask */
  fs_file_date_type           creation_date;    /* Date of file creation */
  fs_buffering_option_type    buffering_option; /* Whether bufferable */
  fs_cleanup_option_type      cleanup_option;   /* Bad powerdown cleanup */
} fs_get_attribs_rsp_type;

/* Response message from FILE_SIZE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       size;        /* Size of file in bytes */
} fs_file_size_rsp_type;

/* Response message from SEEK operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_seek_rsp_type;

/* Response message from SEEK_READ operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* Number of bytes read */
} fs_seek_read_rsp_type;

/* Response message from SEEK_WRITE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* Number of bytes written */
} fs_seek_write_rsp_type;

/* Response message from TELL operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  fs_file_position_type       position;    /* Current file pointer pos */
} fs_tell_rsp_type;

/* Response message from TRUNCATE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_truncate_rsp_type;

/* Response message from RMFILE operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_rmfile_rsp_type;

/* Response message from RENAME operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_rename_rsp_type;

/* Response message from MKDIR operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_mkdir_rsp_type;

/* Response message from RMDIR operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_rmdir_rsp_type;

/* Response message from FMEMSET operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_fmemset_rsp_type;

/* Response message from NAMETEST operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  boolean                     name_found;  /* Whether file name was found */
} fs_nametest_rsp_type;

/* Response message from NUM_FILES operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  word                        num_files;   /* Number of files in directory */
} fs_num_files_rsp_type;

/* Response message from NUM_DIRS operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  word                        num_dirs;    /* Number of subdirectories */
} fs_num_dirs_rsp_type;

/* Response message from LIST_FILES operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* # bytes written to buffer */
} fs_list_files_rsp_type;

/* Response message from LIST_DIRS operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
  dword                       count;       /* # bytes written to buffer */
} fs_list_dirs_rsp_type;

/* Response message from ENUM_INIT operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_enum_init_rsp_type;

/* Response message from ENUM_NEXT operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_enum_next_rsp_type;

/* Response message from ENUM_PREV operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_enum_prev_rsp_type;

/* Response message from ENUM_FINISH operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_enum_finish_rsp_type;

/* Response message from REMOTE_LINK operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_remote_link_rsp_type;

/* Response message from REMOVE_REMOTE_LINKS operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_rmrlink_rsp_type;

/* Response message from RESERVED operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_reserved_rsp_type;

/* Response message for ANY operation */
typedef struct {
  fs_status_type              status;      /* Returned operation status */
} fs_op_rsp_type;

/* Generic response message passed back via callback function */
typedef union {
  fs_open_rsp_type            open;
  fs_close_rsp_type           close;
  fs_read_rsp_type            read;
  fs_write_rsp_type           write;
  fs_set_attribs_rsp_type     set_attribs;
  fs_get_attribs_rsp_type     get_attribs;
  fs_file_size_rsp_type       file_size;
  fs_seek_rsp_type            seek;
  fs_seek_read_rsp_type       seek_read;
  fs_seek_write_rsp_type      seek_write;
  fs_tell_rsp_type            tell;
  fs_truncate_rsp_type        truncate;
  fs_rmfile_rsp_type          rmfile;
  fs_rename_rsp_type          rename;
  fs_mkdir_rsp_type           mkdir;
  fs_rmdir_rsp_type           rmdir;
  fs_fmemset_rsp_type         fmemset;
  fs_nametest_rsp_type        nametest;
  fs_num_files_rsp_type       num_files;
  fs_num_dirs_rsp_type        num_dirs;
  fs_list_files_rsp_type      list_files;
  fs_list_dirs_rsp_type       list_dirs;
  fs_enum_init_rsp_type       enum_init;
  fs_enum_next_rsp_type       enum_next;
  fs_enum_prev_rsp_type       enum_prev;
  fs_enum_finish_rsp_type     enum_finish;
  fs_remote_link_rsp_type     remote_link;
  fs_rmrlink_rsp_type         rmrlink;
  fs_reserved_rsp_type        reserved;
  fs_op_rsp_type              any;
} fs_rsp_msg_type;

/* Function pointer type for operation callback functions */
typedef  void (*fs_callback_p_type)(fs_rsp_msg_type *rsp_msg);


/**************************************************************************/
/*             EMBEDDED FILE SYSTEM EXPORTED OPERATIONS                   */
/**************************************************************************/


/*===========================================================================

FUNCTION FS_DEV_GET_BASE_ADDR

DESCRIPTION
  This function returns the base address of the EFS as described in the
  fsi_device structure.  The part must be identified prior to using this
  function.

DEPENDENCIES
  fs_dev_init must have been called.

RETURN VALUE
  The base address of the NV in the part.

SIDE EFFECTS
  None

===========================================================================*/
dword fs_dev_get_base_addr(void);


/*===========================================================================

FUNCTION FS_OPEN

DESCRIPTION
  Open the file specified by "filename" for the type of access specified
  by "access_mode."  

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_open returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a file handle.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_open
(
  const char                 *filename,          /* Name of file to open */
  fs_access_option_type      access_mode,        /* Open mode */
  const fs_open_xparms_type  *open_options,      /* Optional parameters */
  fs_callback_p_type         callback_function,  /* Callback function ptr */
  fs_rsp_msg_type            *open_rsp_msg       /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_CLOSE

DESCRIPTION
  Close the file identified by "handle."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_close returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_close
(
  fs_handle_type        handle,            /* Handle of file to close */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *close_rsp_msg     /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_READ

DESCRIPTION
  Read "count" bytes from the file identified by "handle" into "buffer"
  starting at the current file pointer position associated with "handle."
  The number of bytes actually read is returned in "count" following
  operation completion.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_read returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a count of the bytes actually read.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_read
(
  fs_handle_type        handle,            /* Handle of file to read from */
  PACKED void           *buffer,           /* Buffer for read bytes */
  dword                 count,             /* Number of bytes to read */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *read_rsp_msg      /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_WRITE

DESCRIPTION
  Write "count" bytes from "buffer" into the file identified by "handle"
  starting at the current file pointer position associated with "handle."
  The number of bytes actually written is returned in "count" following
  operation completion.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_write returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a count of the bytes actually written.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_write
(
  fs_handle_type        handle,            /* Handle of file to write to */
  PACKED const void     *buffer,           /* Buffer with bytes to write */
  dword                 count,             /* Number of bytes to write */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *write_rsp_msg     /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_RESERVED_WRITE

DESCRIPTION
  Write "count" bytes from "buffer" into the file identified by "handle"
  starting at the current file pointer position associated with "handle."
  The number of bytes actually written is returned in "count" following
  operation completion.  A confirmation code from a previously-made
  space reservation is required; the write is guaranteed to work if
  the number of bytes to be written does not exceed the number of bytes
  reserved.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_write returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a count of the bytes actually written.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_reserved_write
(
  fs_handle_type        handle,            /* Handle of file to write to */
  PACKED const void     *buffer,           /* Buffer with bytes to write */
  dword                 count,             /* Number of bytes to write */
  fs_conf_code_type     conf_code,         /* Confirmation code */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *write_rsp_msg     /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_SET_FILE_ATTRIBUTES

DESCRIPTION
  Set the file attributes of the file specified by "filename" to the
  specified values.
  
  The attributes are:
    -- Access restrictions (None, Permanent, Readonly, Remote)
    -- Creation date
    -- File buffering option
    -- Bad powerdown recovery option  
  
  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_open returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_set_file_attributes
(
  const char                *filename,           /* File name */
  fs_attribute_mask_type    attributes,          /* New attributes */
  fs_file_date_type         creation_date,       /* File creation date */
  fs_buffering_option_type  buffering_option,    /* Whether bufferable */
  fs_cleanup_option_type    cleanup_option,      /* Bad powerdown clean-up */
  fs_callback_p_type        callback_function,   /* Callback function ptr */
  fs_rsp_msg_type           *set_attribs_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_GET_FILE_ATTRIBUTES

DESCRIPTION
  Return the file attributes of the file specified by "filename."
  
  The attributes are:
    -- Access restrictions (None, Permanent, Readonly, Remote)
    -- Creation date
    -- File buffering option
    -- Bad powerdown recovery option  
  
  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_open returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the specified file's attributes.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_get_file_attributes
(
  const char                *filename,           /* File name */
  fs_callback_p_type        callback_function,   /* Callback function ptr */
  fs_rsp_msg_type           *get_attribs_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_GET_OPEN_FILE_ATTRIBUTES

DESCRIPTION
  Return the file attributes of the currently open file specified by 
  "handle."
  
  The attributes are:
    -- Access restrictions (None, Permanent, Readonly, Remote)
    -- Creation date
    -- File buffering option
    -- Bad powerdown recovery option  
  
  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_open returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the specified file's attributes.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_get_open_file_attributes
(
  fs_handle_type            handle,              /* File handle */
  fs_callback_p_type        callback_function,   /* Callback function ptr */
  fs_rsp_msg_type           *get_attribs_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_FILE_SIZE

DESCRIPTION
  Return the current file size of the file identified by "filename."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_file_size returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the specified file's current size in bytes.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_file_size
(
  const char                *filename,           /* Name of file */
  fs_callback_p_type        callback_function,   /* Callback function ptr */
  fs_rsp_msg_type           *file_size_rsp_msg   /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_SEEK

DESCRIPTION
  Set the current file pointer position of the file associated with "handle"
  to the position specified by "position" and "origin."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_seek returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_seek
(
  fs_handle_type        handle,                /* File handle */
  fs_seek_origin_type   origin,                /* Seek origin */
  fs_file_position_type position,              /* Position to seek to */
  fs_callback_p_type    callback_function,     /* Callback function ptr */
  fs_rsp_msg_type       *seek_rsp_msg          /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_SEEK_READ

DESCRIPTION
  Set the current file pointer position of the file associated with "handle"
  to the position specified by "position" and "origin."  Then read "count" 
  bytes from the file into "buffer" starting at the seeked position.
  The number of bytes actually read is returned in "count" following
  operation completion.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_seek_read returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a count of the bytes actually read.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_seek_read
(
  fs_handle_type        handle,            /* Handle of file to read from */
  fs_seek_origin_type   origin,            /* Seek origin */
  fs_file_position_type position,          /* Position to seek to */
  PACKED void           *buffer,           /* Buffer for read bytes */
  dword                 count,             /* Number of bytes to read */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *seek_read_rsp_msg /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_SEEK_WRITE

DESCRIPTION
  Set the current file pointer position of the file associated with "handle"
  to the position specified by "position" and "origin."  Then write "count" 
  bytes from "buffer" into the file starting at the seeked position.
  The number of bytes actually written is returned in "count" following
  operation completion.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_seek_write returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a count of the bytes actually written.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_seek_write
(
  fs_handle_type        handle,            /* Handle of file to write to */
  fs_seek_origin_type   origin,            /* Seek origin */
  fs_file_position_type position,          /* Position to seek to */
  PACKED const void     *buffer,           /* Buffer with bytes to write */
  dword                 count,             /* Number of bytes to write */
  fs_callback_p_type    callback_function, /* Callback function pointer */
  fs_rsp_msg_type       *write_rsp_msg     /* Response message pointer */
);



/*===========================================================================

FUNCTION FS_TELL

DESCRIPTION
  Return the current file pointer position of the file associated with 
  "handle."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_tell returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the specified file's current file pointer position.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_tell
(
  fs_handle_type        handle,                /* File handle */
  fs_callback_p_type    callback_function,     /* Callback function ptr */
  fs_rsp_msg_type       *tell_rsp_msg          /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_TRUNCATE

DESCRIPTION
  Truncate the file identified by "handle" to the position specified by
  "truncate_pos."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_truncate returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_truncate
(
  fs_handle_type        handle,                /* File handle */
  fs_file_position_type truncate_pos,          /* File truncate position */
  fs_callback_p_type    callback_function,     /* Callback function ptr */
  fs_rsp_msg_type       *truncate_rsp_msg      /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_REMOVE

DESCRIPTION
  Remove the file identified by "filename" ("filename" must be fully
  qualified).

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_remove returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_remove
(
  const char              *filename,           /* Name of file to remove */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *rmfile_rsp_msg      /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_RENAME

DESCRIPTION
  Rename the file identified by "old_filename" to "new_filename".

  NOTE: File renames can generate a large amount of file system activity,
  unless the old and new file names are exactly the same length.  It is
  highly recommended that file renames be constrained to same-length
  names, unless the files are small (under 1K bytes or so).  Note that
  blanks are valid file name characters, so padding can easily be
  used to satisfy this constraint.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_remove returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/
void  fs_rename
(
  const char                  *old_filename,     /* Current name of file */
  const char                  *new_filename,     /* New name of file */
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *rename_rsp_msg    /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_FMEMSET

DESCRIPTION
  Set the file contents of the file associated with "handle" to the 
  specified character, starting at the current file pointer position,
  up to the specified file pointer position.  The ending position may
  be beyond the current end of file; this function may therefore be
  used to initialize a file to some definite byte value and size.
  
  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_seek returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_fmemset
(
  fs_handle_type        handle,                /* File handle */
  fs_file_position_type position,              /* Position to write up to */
  byte                  set_char,              /* Character to write */
  fs_callback_p_type    callback_function,     /* Callback function ptr */
  fs_rsp_msg_type       *fmemset_rsp_msg       /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_MKDIR

DESCRIPTION
  Creates a new user file directory named "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_mkdir returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_mkdir
(
  const char              *dirname,            /* Name of new directory */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *mkdir_rsp_msg       /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_RMDIR

DESCRIPTION
  Removes the user file directory specified by "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_rmdir returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_rmdir
(
  const char              *dirname,            /* Directory to remove */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *rmdir_rsp_msg       /* Response message ptr */
);


/*===========================================================================

MACRO FS_TEST

DESCRIPTION
  This is the old interface of fs_test() (now fs_nametest()) function.  It 
  tests for the existence of a file or a directory: first checks if there 
  is a file with the specified name existed. If not, it then checks if there 
  is a directory with the specified name existed.
  
  Eventually, we are hoping fs_nametest() will be the only one used.  For
  now, this macro is here for backward compatibility.
  
=============================================================================*/
  
#define fs_test(filename, callback_function, test_rsp_msg)   \
        fs_nametest(filename, FS_TEST_FILE_OR_DIR, callback_function, test_rsp_msg)


/*===========================================================================

FUNCTION FS_NAMETEST

DESCRIPTION
  Test for the existence of a file or a directory, specified by type,
  
  FS_TEST_FILE_OR_DIR - if there is no file with the specified 
      (fully qualified) name, check if there is a directory 
      with that name.  
      
  FS_TEST_FILE - check if there is a file with the specified (fully
      qualified) name.
      
  FS_TEST_DIR - check if there is a directory with the specified
      (fully qualified) name.
      
  Return TRUE (via the response message) if the file or the directory
  already exists and FALSE otherwise.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_open returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with a flag indicating whether or not the file exists.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_nametest
(
  const char              *filename,           /* File name to check for */
  fs_nametest_type        type,                /* Test a file or a dir */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *test_rsp_msg        /* Response message ptr */
);




/*===========================================================================

FUNCTION FS_NUM_FILES

DESCRIPTION
  Return the number of files in the user file directory specified by
  "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_num_files returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the number of files in the specified directory.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_num_files
(
  const char              *dirname,            /* Root directory name */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *num_files_rsp_msg   /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_NUM_DIRS

DESCRIPTION
  Return the number of subdirectiories in the user file directory specified 
  by "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_num_dirs returns to the caller immediately and the callback
  function is invoked upon operation completion.  If no callback function
  is specified, a REX_WAIT (on operation completion) is invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message
  along with the number of subdirectories in the specified directory.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_num_dirs
(
  const char              *dirname,            /* Root directory name */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *num_dirs_rsp_msg    /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_LIST_FILES

DESCRIPTION
  Return a list of the files (filenames) in the user file directory 
  specified by "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_list_files returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.
  The file list is copied to the buffer specified by "buf."

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_list_files
(
  const char              *dirname,            /* Root directory name */
  void                    *buf,                /* Buffer to receive list */
  dword                   buf_length,          /* Length of buffer */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *listf_rsp_msg       /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_LIST_DIRS

DESCRIPTION
  Return a list of the subdirectories in the user file directory specified 
  by "dirname."

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_list_dirs returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.
  The subdirectory list is copied to the buffer specified by "buf."

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_list_dirs
(
  const char              *dirname,            /* Root directory name */
  void                    *buf,                /* Buffer to receive list */
  dword                   buf_length,          /* Length of buffer */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *listd_rsp_msg       /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_ENUM_INIT

DESCRIPTION
  Initialize a file or directory enumeration control object (allocated
  by the client) for subsequent use with the "fs_enum_next" operation.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_enum_init returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_enum_init
(
  const char                  *dirname,          /* Root directory */
  fs_enum_type                enum_kind,         /* File or subdir list */
  fs_enum_iterator_type       *iterator_ptr,     /* Ptr to control struct */
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *enum_init_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_ENUM_NEXT

DESCRIPTION
  Using the file or directory enumeration control object referenced
  by "iterator_ptr", return file information for the next file or 
  directory within the specified root directory.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_enum_next returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

  The file or directory enumeration control object referenced by 
  "iterator_ptr" must have been previously initialized via a call
  to "fs_enum_init".

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_enum_next
(
  fs_enum_data_type           *data_ptr,         /* Returned file info */ 
  fs_enum_iterator_type       *iterator_ptr,     /* Ptr to control struct */
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *enum_next_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_ENUM_PREV

DESCRIPTION
  Using the file or directory enumeration control object referenced
  by "iterator_ptr", return file information for the prev (created) file 
  or directory within the specified root directory.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_enum_next returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

  The file or directory enumeration control object referenced by 
  "iterator_ptr" must have been previously initialized via a call
  to "fs_enum_init".

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_enum_prev
(
  fs_enum_data_type           *data_ptr,         /* Returned file info */ 
  fs_enum_iterator_type       *iterator_ptr,     /* Ptr to control struct */
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *enum_prev_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_ENUM_FINISH

DESCRIPTION
  Clean up the enumeration control object referenced by iterator_ptr.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_enum_next returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

  The file or directory enumeration control object referenced by 
  "iterator_ptr" must have been previously initialized via a call
  to "fs_enum_init".

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_enum_finish
(
  fs_enum_iterator_type       *iterator_ptr,     /* Ptr to control struct */
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *enum_next_rsp_msg /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_REMOTE_LINK

DESCRIPTION

  Create a link to a remote file (i.e., a file residing outside the
  file system address space).  

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_remote_link returns to the caller immediately and the 
  callback function is invoked upon operation completion.  If no callback 
  function is specified, a REX_WAIT (on operation completion) is invoked 
  instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_remote_link
(
  const char              *filename,           /* Name of remote file */
  dword                   base_address,        /* Address of remote file */
  dword                   length,              /* Size of remote file */
  fs_callback_p_type      callback_function,   /* Callback function ptr */
  fs_rsp_msg_type         *remote_rsp_msg      /* Response message ptr */
);



/*===========================================================================

FUNCTION FS_REMOVE_REMOTE_LINKS

DESCRIPTION

  Remove all remote file links.

  This function constructs an fs_cmd_type object and places it on the
  embedded file system task's command queue.  This function can be either
  blocking (synchronous) or non-blocking (asynchronous) depending on
  whether a callback function is specified.  If a callback function is 
  specified, fs_remove_remote_links returns to the caller immediately 
  and the callback function is invoked upon operation completion.  If no 
  callback function is specified, a REX_WAIT (on operation completion) is 
  invoked instead.
  
DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  An operation status indicator is returned in the response message.

SIDE EFFECTS
  If no callback function is specified, calling task REX_WAITs.
  
===========================================================================*/

void  fs_remove_remote_links
(
  fs_callback_p_type          callback_function, /* Callback function ptr */
  fs_rsp_msg_type             *rmrlink_rsp_msg   /* Response message ptr */
);
 


/*===========================================================================

FUNCTION FS_RESERVE_SPACE

DESCRIPTION

  Reserve the amount of file system space needed to guarantee that 
  the specified number of bytes can be written.  If the requested space 
  is unavailable, return an error indication; otherwise, return a
  "confirmation code" that can be used by the caller to insure 
  successful completion of the write.  Note that a confirmation code 
  can only be used once, even if the caller does not actually write
  the full number of bytes previously reserved.  A reservation is held 
  for the reserver until either: 1) it is used to perform a write; or 
  2) the reservation is cancelled via a call to fs_cancel_reservation; 
  or 3) a power-cycle occurs (all reservations are non-persistent and
  are lost across a power-cycle).  

  If the reservation is intended to be used for a "revertable" file
  (i.e., a file that has the cleanup-attribute of FS_OC_REVERT), this
  must be indicated so that additional space will be reserved for the
  file's checkpoint data.  For revertable files only, an upper limit 
  for the size of the revertable file prior to the expected write must 
  be supplied.

  A reservation for a revertable file may be used for a non-revertable 
  file, but a reservation for a non-revertable file is not guaranteed 
  to work with a revertable file.

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  If enough file system space exists to satisfy the reservation:
    a confirmation code that can be subsequently used to guarantee
    that a write of less than or equal to the number of reserved 
    bytes will be successful.
  If not enough file system space exists to satisfy the reservation:
    a Null confirmation code.

SIDE EFFECTS
  The amount of file system space available for allocation is immediately
  reduced by the amount needed to satisfy the reservation request.
  
===========================================================================*/

fs_conf_code_type  fs_reserve_space
(
  boolean               for_revertable_file,
  fs_rsvr_request_type  rsvr_request
);



/*===========================================================================

FUNCTION FS_CANCEL_RESERVATION

DESCRIPTION

  Cancel the specified reservation of file system space.

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  FS_OKAY_S               -- if reservation was cancelled successfully
  FS_INVALID_CONF_CODE_S  -- if no reservation was found matching the
                             specified confirmation code

SIDE EFFECTS
  The amount of file system space available for allocation is immediately
  increased by the amount released by the cancelled reservation.
  
===========================================================================*/

fs_status_type  fs_cancel_reservation
(
  fs_conf_code_type     conf_code
);



/*===========================================================================

FUNCTION FS_RESERVATION_IS_NULL

DESCRIPTION

  Returns TRUE if the specified confirmation code is Null, FALSE otherwise.

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  TRUE   -- if the specified confirmation code is Null
  FALSE  -- if the specified confirmation code is valid

SIDE EFFECTS
  None.
  
===========================================================================*/

boolean  fs_reservation_is_null
(
  fs_conf_code_type     conf_code
);



/*===========================================================================

FUNCTION FS_SPACE_AVAILABLE

DESCRIPTION

  Returns the amount of available file system space (units are bytes).

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  Number of bytes of file system space currently available.

SIDE EFFECTS
  None.
  
===========================================================================*/

dword  fs_space_available
(
  void
);



/*===========================================================================

FUNCTION FS_SPACE_ERASED

DESCRIPTION

  Returns the amount of file system space currently "erased," that is,
  able to be written without an intervening garbage collection operation
  (units are bytes).

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  Number of bytes of file system space currently erased.

SIDE EFFECTS
  None.
  
===========================================================================*/

dword  fs_space_erased
(
  void
);



/*===========================================================================

FUNCTION FS_SPACE_USED

DESCRIPTION

  Returns the amount of "in-use" file system space (units are bytes).

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  Number of bytes of file system space currently in-use.

SIDE EFFECTS
  None.
  
===========================================================================*/

dword  fs_space_used
(
  void
);



/*===========================================================================

FUNCTION FS_SPACE_RESERVED

DESCRIPTION

  Returns the amount of reserved file system space (units are bytes).

  This function call is synchronous and does not involve the fs task
  command queues.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  Number of bytes of file system space currently reserved.

SIDE EFFECTS
  None.
  
===========================================================================*/

dword  fs_space_reserved
(
  void
);



/*===========================================================================

FUNCTION FS_CMD_IS_WAITING

DESCRIPTION
  This function returns TRUE if there is currently a command on the
  EFS task's command queue (fs_cmd_q), FALSE otherwise.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.

===========================================================================*/

boolean  fs_cmd_is_waiting
( 
  void
);



/*===========================================================================

FUNCTION FS_TASK

DESCRIPTION
  This is the Embedded File System (FS) task, created via rex_def_task 
  from the Main Control task.  The FS task sets MC_ACK_SIG when it is done 
  with initialization and it then waits for FS_START_SIG before beginning 
  it's normal operations.  The FS task then enters an infiniten loop, 
  awaiting commands received on its command queue.  Each command received 
  is processed to completion before the next command is processed.  While 
  in the main loop the task kicks the watchdog periodically.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void  fs_task
( 
  dword              parm       /* from REX - ignored                     */
);


#ifdef FEATURE_FS_ALLOW_MASTER_RESET

/*===========================================================================

FUNCTION FS_MASTER_RESET

DESCRIPTION

  Erase all file system space.  Phone must be rebooted before file system
  can be used again.

  This function call is synchronous and does not involve the fs task
  command queues.  This function must not be enabled in production code.

DEPENDENCIES
  The FS task must have been started up already. 

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

void  fs_master_reset
(
  void
);
#endif /* FEATURE_FS_ALLOW_MASTER_RESET */

#endif /* FS_H */

