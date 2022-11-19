#ifndef FS_DEVI_H
#define FS_DEVI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   E M B E D D E D   F I L E   S Y S T E M


GENERAL DESCRIPTION
  This file contains internal definitions needed by the low level device 
  layer for the embedded filesystem.

Copyright (c) 1999 by QUALCOMM Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/FS_DEVI.H_v   1.0   09 Nov 2000 08:38:28   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11-09-00   pg      Initial version for common archive.
                   Put drivers in different files.
04-22-99   ls      Removed enclosing ifdef.
03-04-99   ls      Initial version.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


/* Internals for flash driver. */

/* Base address of the flash component. */
#define FLASH_BASE 0

/* Word offset of the NV portion of the various flash components. */
#define FS_AMD_NV_BASE  0x70000

/* Device information. */
typedef struct {
  const char      *name;        /* Textual name of device. */
  fsi_dev_info     info;        /* User returned info. */
  dword            nv_offset;   /* Word offset to start of NV area in part. */
  dword            nv_size;     /* Size (in words) of the NV part. */

  fs_status_type (*read) (byte *buffer, dword offset, dword count);
                                /* Device read operation. */
  fs_status_type (*write) (byte *buffer, dword offset, dword count);
                                /* Device write operation. */
  fs_status_type (*erase) (dword offset);
                                /* Device erase operation. */
  fs_status_type (*erase_status) ();
                                /* Progress of erase operation. */

  fs_status_type (*suspend_erase) ();
                                /* Suspend an erase operation. */
  fs_status_type (*resume_erase) ();
                                /* Resume a suspended erase. */
} fsi_device;

/* Low level status bits for AMD. */

#define FS_AMD_DQ7      0x80
#define FS_AMD_DQ6      0x40
#define FS_AMD_DQ5      0x20
#define FS_AMD_DQ2      0x04


/* For unit testing, the following declarations allow flash operations to
 * be made externally, rather than directly to a device. */

#ifdef FS_DEV_UNIT_TEST
#undef FEATURE_SURF_FLASH
#undef FS_DEV_ENABLE_MITS
#define FS_DEV_ENABLE_MITS
#undef FS_DEV_ENABLE_INTEL
#undef FS_DEV_ENABLE_AMD

/* Unit test device operations go to these simulation functions. */
extern void dev_simulate_write (dword, word);
extern word dev_simulate_read (dword);
extern void dev_simulate_memmove (void *, void *, dword);
#define FOUTW(addr,val)  (dev_simulate_write ((dword) (addr), (word) (val)))
#define FINW(addr)       (dev_simulate_read  ((dword) (addr)))
#define memmove          dev_simulate_memmove

/* Local memory is still local memory, but we want to check for alignment. */
#define GET_BYTE(addr)   (*((byte *) addr))
#define GET_WORD(addr) \
        checked_read ((dword) (addr));

static word
checked_read (dword offset)
{
  if ((offset & 1) != 0) {
    ERR_FATAL ("Misaligned access: 0x%x", offset, 0, 0);
  }

  return *((word *) offset);
}
#else /* not FS_DEV_UNIT_TEST */
/* If we're not unit testing, then these operations should go directly to
 * the device. */
#define FOUTW(addr,val)  (*((volatile word *) addr) = (val))
#define FINW(addr)       (*((volatile word *) addr))

#define GET_BYTE(addr)   (*((byte *) addr))
#define GET_WORD(addr)   (*((word *) addr))
#endif









extern volatile word *fs_dev_base;
extern fsi_device *fsi_target;
extern fsi_erase_state_type fsi_erase_state;
extern dword fsi_erase_location;
extern fs_status_type fsi_worded_read (byte *buffer,
                                       dword offset,
                                       dword count);

/* Functions for different drivers */

#ifdef FS_DEV_ENABLE_AMD
fsi_device *amd_probe (void);
#endif /* FS_DEV_ENABLE_AMD */

#ifdef FS_DEV_ENABLE_INTEL
fsi_device *intel_probe (void);
#endif /* FS_DEV_ENABLE_INTEL */

#ifdef FS_DEV_ENABLE_MITS
fsi_device *mitsubishi_probe (void);
#endif /* FS_DEV_ENABLE_MITS */

#endif /* not FS_DEVI_H */

