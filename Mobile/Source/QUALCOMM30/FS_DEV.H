#ifndef FS_DEV_H
#define FS_DEV_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   E M B E D D E D   F I L E   S Y S T E M


GENERAL DESCRIPTION
  This is the header file for the low level device layer for the embedded
  filesystem.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  This task should be started up with rex_def_task.

Copyright (c) 1999 by QUALCOMM Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/FS_DEV.H_v   1.0   09 Nov 2000 08:38:28   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11-09-00   pg      Initial version for common archive.
08-03-00   pg      Added function fs_dir_base_addr() so that when direct
                   accessing the device during EFS initialization, the 
                   base addresses of different devices can adjust 
                   accordingly.
04-29-99   ls      Moved definition of device control option bits to fsi.h.
04-22-99   ls      Removed enclosing ifdef.
03-04-99   ls      Initial version.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* Simplified fs_dev.h */

#include "comdef.h"
#include "fsi.h"

#define FSD_MAX_SECTORS 48

/* Device info. */
typedef struct {
  word num_sectors;             /* Total number of sectors in the device. */
  dword num_bytes[FSD_MAX_SECTORS]; /* Number of bytes in each sector. */
  word codes[4];                /* Manufacture's codes. */
  word device_control_options;
} fsi_dev_info;

/* Prototypes. */
fs_status_type fs_dev_read_bytes (byte *buffer, dword offset, dword count);
void fs_dev_init (void);
fs_status_type fs_dev_write_bytes (byte *buffer, dword offset, dword count);

/* Return the device info.  This is a statically defined structure,
   and should not be changed. */
fsi_dev_info *fs_dev_get_info (void);

fs_status_type fs_dev_erase_start (word sector);

/* Determine erase status.  Must be called until it returns FS_OKAY_S. */
fs_status_type fs_dev_erase_status (void);

/* Erase suspending.  Two possibilities:
   - The erase was happening:
     The erase is suspended, and FS_GC_IN_PROGRESS_S is returned
     (the erase is suspended though)
   - The erase has finished:
     FS_OKAY_S is returned, and the erase is completed (no need to
     call fs_dev_erase_status. */
fs_status_type fs_dev_suspend_erase (void);

/* Resume a suspended erase.  Must then call fs_dev_erase_status, or
   fs_dev_suspend_erase before using the device.  Returns either
   FS_OKAY_S for success, or FS_FAIL_S for a failure. */
fs_status_type fs_dev_resume_erase (void);

/* Erase the whole device */
fs_status_type fs_dev_erase_all (void);

#ifdef MODEL_SKTT
/* moved from FS_DEV.C  by jnlee 99.11.5 */
/* Status of erasure operations. */
typedef enum {
  FSI_READ_MODE,
  FSI_ERASING,
  FSI_ERASE_SUSPENDED
} fsi_erase_state_type;
/*****************************************/

#define DOWNLOAD_SIZE 3000
typedef fs_status_type (*F_ERASERAM) (dword);
extern F_ERASERAM FlashEraseRAM;
typedef fs_status_type (*F_WRITERAM) (byte*,dword,dword);
extern F_WRITERAM FlashWriteRAM;
fs_status_type ROM_fsi_intel_write (byte *,dword ,dword );
fs_status_type ROM_fsi_intel_erase (dword);
void ROM_fsi_intel_write_end(void);
void ROM_fsi_intel_erase_end(void);

#endif		//ifdef MODEL_SKTT

 /* returns the base address of the EFS to start reading from or
   writing to . */
byte* fs_dev_base_addr (void); 

#endif /* not FS_DEV_H */


