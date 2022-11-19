#ifndef FS_PARMS
#define FS_PARMS

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   E M B E D D E D   F I L E   S Y S T E M


GENERAL DESCRIPTION
  Embedded File System parameters file.  This module contains client 
  specified global parameters.

Copyright (c) 1999,2000 by QUALCOMM Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/FS_PARMS.H_v   1.0   09 Nov 2000 08:38:42   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11-09-00   pg      Initial version for common archive.
                   Add support for AMD on SURF.
09-27-00   pg      Added definitions for CMX build with VR.
08-22-00   dlb     Fixed some support for unit testing.
07-25-00   pg      Took out some command for testing purpose.
07-24-00   pg      Added support for directly accessing the file system 
                   with FS_DIR_INIT_DIRECT_ACCESS turned on.
07-20-00   dlb     Support Intel and Mitsubishi dynamically.
06-29-00   dlb     Add support for Mitsubishi on SURF.
05-23-00   pg      Added definitions for CMX build.
02-23-00   rp      Modified FS_PHYSICAL_BLKSIZE_P from 1024 to 256 bytes
                   on the FFA for NV Item Manager and VR support; also
                   adjusted FS_MAX_FBL_NODES_P, etc. for this block size.
01-20-00   rp      Adjusted FS_MAX_NUM_FILES_P values for an additonal 10
                   files (voice memos) under FEATURE_PHONE_VR.
12-08-99   rp      Changed use of T_SURF to FEATURE_SURF_FLASH so FFA
                   can use the appropriate Flash devices. 
11-08-99   rp      Added section for the use of the NV Item Manager in 
                   combination with VR.
                   Adjusted FS_MAX_NUM_FILES_P values to simply add 10
                   files to the required user file count to account for six
                   system files and some padding.
09-28-99   rp      Removed undef of FEATURE_FS_RAM_DRIVER.
                   Updated FS_MAX_NUM_FILES_P to 100 for NV Item Manager
                   due to an unusual high number of transaction log files.  
09-27-99   rw      Added FEATURE_FS_RAM_DRIVER and associated parameters when
                   using the new driver for RAM-based devices.
08-13-99   rp      Featurized for NV Item Manager and VR support.
                   using the new driver for RAM-based devices.
08-13-99   rp      Featurized for NV Item Manager and VR support.
07-20-99   rp      Increased FS_MAX_NUM_FILES_P from 75 to 85 for VR.
05/27/99   sa      Changed FS_FILE_SPACE_LIMIT_P and FS_MAX_FBL_NODES_P
05/27/99   sa      Disabled FEATURE_FS_BACKGROUND_ERASE 
05/27/99   sa      Changed number of simultaneously open files from 4 to 6.
05-24-99   sa      Changed parameters to reduce RAM usage.
05-21-99   ls      Configured for SURF.
05-21-99   ls      Configured for Digby.
05-20-99   ls      Added space limit parameter.
04-30-99   ls      Re-enable background erase.
04-29-99   ls      Temporarily turn off background erase.
04-27-99   ls      Added space available offset parameter.
04-25-99   ls      Tuned garbage collection parameters for maximum activity.
04-23-99   ls      Added second garbage collection threshold parameter.
04-22-99   ls      Removed enclosing ifdef.
04-21-99   ls      Version 2.2 release.
04-03-99   ls      Updated parameter values.
04-01-99   dlb     Changed flash type to be automatic.
03-04-99   ls      Initial version.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include	"customer.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This file contains user-configurable system parameters.  Great caution
is advised before changing any parameters.

===========================================================================*/


/* Selection of flash components must be based on addressing issues.
   At this time, we support two platforms:
   SURF, a separate intel flash at 0x200000.
   5GP+ (Dibgy), RWW either AMD or MITS at 0x000000 and up.

   We also support SURF using Mitsutishi parts.  This selection is made
   with the FEATURE_EFS_MITSUBISHI feature.
   
   We also support SURF using AMD parts.  This selection is made with 
   the FEATURE_EFS_AMD feature.

   The selection of devices must be carefully chosen because the
   probes of some devices will render the code space unreadable. */
#ifdef MODEL_SKTT
	#define FS_DEV_ENABLE_INTEL
  	#define FS_DIR_INIT_DIRECT_ACCESS	
#else
	#ifdef FEATURE_EFS_FLASH2
	# define FS_DEV_ENABLE_INTEL
	# define FS_DEV_ENABLE_AMD
  	#define FS_DIR_INIT_DIRECT_ACCESS	
	#else /* not FEATURE_EFS_FLASH2 */
	#error code not present
	#endif
#endif
/* Allow commands which reset the flash device (i.e., erases everything) */
#define FEATURE_FS_ALLOW_MASTER_RESET

/* Device erase suspend/resume setting */ 
#define FEATURE_FS_BACKGROUND_ERASE

/* Directory separator character */
#define  FS_DIRECTORY_CHAR_P        ('/')

/* Maximum file name length (cannot exceed block size) */
#define  FS_FILENAME_MAX_LENGTH_P   40

#ifdef FEATURE_NV_ITEM_MGR

  #ifdef FEATURE_PHONE_VR
#error code not present
  #elif defined(FEATURE_AUDIO_FORMAT)
     /* Threshold at which garbage collection nominally begins on a sector */
     #define  FS_GC_INIT_THRESHOLD_P     50

     /* Threshold at which garbage collection runs aggressively (this parameter */
     /* specifies the desired minimum number of clean blocks to be maintained   */
     /* at all times, if possible)                                              */
     #define  FS_GC_CLEAN_RESERVE_P      128

     /* The number of blocks to subtract from the "space_available" function so */
     /* the reported ceiling is a little less than the actual ceiling           */
     #define  FS_SPACE_REPORT_OFFSET_P   32

     /* Maximum number of files in the system */
     #define  FS_MAX_NUM_FILES_P         145

     /* Maximum number of simultaneously "open" files in the system */
     #define  FS_MAX_OPEN_FILES_P        50

     /* Size of a physical block in bytes */
     #define  FS_PHYSICAL_BLKSIZE_P  256

     /* Maximum total number of blocks in any sector */
     #define  FS_MAX_BLKS_ALL_SECTORS_P   256

     /* Maximum total number of sectors in the device */
     #define  FS_NUM_SECTORS_P  40

     /* Maximum amount of file space to be managed by the file system */
     #define  FS_FILE_SPACE_LIMIT_P      0

     /* Maximum number of file block list nodes in the system */
     #define  FS_MAX_FBL_NODES_P         8192

     /* Number of entries per file block list node */
     #define  FS_FBL_ENTRIES_PER_NODE_P  1
  #else

     /* Threshold at which garbage collection nominally begins on a sector */
//     #define  FS_GC_INIT_THRESHOLD_P     50
     #define  FS_GC_INIT_THRESHOLD_P     30		// by jnlee for B1 01.08.08

     /* Threshold at which garbage collection runs aggressively (this parameter */
     /* specifies the desired minimum number of clean blocks to be maintained   */
     /* at all times, if possible)                                              */
     #define  FS_GC_CLEAN_RESERVE_P      128

     /* The number of blocks to subtract from the "space_available" function so */
     /* the reported ceiling is a little less than the actual ceiling           */
     #define  FS_SPACE_REPORT_OFFSET_P   32

     /* Maximum number of files in the system */
//   #define  FS_MAX_NUM_FILES_P         46 
//   #define  FS_MAX_NUM_FILES_P         100			//by jnlee 00.2.10
     #define  FS_MAX_NUM_FILES_P         300			//by woodstok 01.05.14

     /* Maximum number of simultaneously "open" files in the system */
     #define  FS_MAX_OPEN_FILES_P        50 

     /* Size of a physical block in bytes */
     #define  FS_PHYSICAL_BLKSIZE_P  256

     /* Maximum total number of blocks in any sector */
     #define  FS_MAX_BLKS_ALL_SECTORS_P   256

     /* Maximum total number of sectors in the device */
#if MODEL_ID== MODEL_B2
     #define  FS_NUM_SECTORS_P  48
#else
     #define  FS_NUM_SECTORS_P  40
#endif

     /* Maximum amount of file space to be managed by the file system (this  */
     /* parameter can be used to limit the amount of RAM required when the   */
     /* file system is used with devices that provide more capacity than     */
     /* needed).  A setting of 0 indicates "no limitation": the full         */
     /* capacity of the device will be available.  Otherwise, the maximum    */
     /* amount of file system space will be either the full capacity of the  */
     /* device or the values of this parameter, whichever is smaller.  The   */
     /* units are bytes (NOTE: the value will be rounded down to the nearest */
     /* complete sector).                                                    */
//  #define  FS_FILE_SPACE_LIMIT_P		196608		//3block 사용시 
//	#define  FS_FILE_SPACE_LIMIT_P		524288		//8block 사용시 
//	#define  FS_FILE_SPACE_LIMIT_P		786432		//12block 사용시 
#if MODEL_ID == MODEL_B1
	#define  FS_FILE_SPACE_LIMIT_P		2031616		//31block 사용시 01.2.26
#elif MODEL_ID == MODEL_B2
//	#define  FS_FILE_SPACE_LIMIT_P		2031616		//31block 사용시 01.2.26
//	#define  FS_FILE_SPACE_LIMIT_P		2621440		//40block 사용시 01.08.08
	#define  FS_FILE_SPACE_LIMIT_P		2097152		//32block 사용시 01.08.08
#else
	#define  FS_FILE_SPACE_LIMIT_P		1048576		//16block 사용시 00.2.7
#endif

     /* Maximum number of file block list nodes in the system */
//  #define  FS_MAX_FBL_NODES_P         768			//3block 사용시 
//	#define  FS_MAX_FBL_NODES_P         2048		//8block 사용시 
//	#define  FS_MAX_FBL_NODES_P         3072		//12block 사용시 
#if MODEL_ID == MODEL_B1
	#define  FS_MAX_FBL_NODES_P         7936		//31block 사용시 01.2.26
#elif MODEL_ID == MODEL_B2
//	#define  FS_MAX_FBL_NODES_P         7936		//31block 사용시 01.2.26
//	#define  FS_MAX_FBL_NODES_P         10240		//40block 사용시 01.08.08
	#define  FS_MAX_FBL_NODES_P         8192		//32block 사용시 01.08.08
#else
	#define  FS_MAX_FBL_NODES_P         4096		//16block 사용시 00.2.7
#endif
     /* Number of entries per file block list node */
     #define  FS_FBL_ENTRIES_PER_NODE_P  1
   #endif
#else
  #ifdef FEATURE_PHONE_VR
#error code not present
  #else

    /* Threshold at which garbage collection nominally begins on a sector */
    #define  FS_GC_INIT_THRESHOLD_P     50

    /* Threshold at which garbage collection runs aggressively (this parameter */
    /* specifies the desired minimum number of clean blocks to be maintained   */
    /* at all times, if possible)                                              */
    #define  FS_GC_CLEAN_RESERVE_P      128

    /* The number of blocks to subtract from the "space_available" function so */
    /* the reported ceiling is a little less than the actual ceiling           */
    #define  FS_SPACE_REPORT_OFFSET_P   32

    /* Maximum number of files in the system */
    #define  FS_MAX_NUM_FILES_P         100

    /* Maximum number of simultaneously "open" files in the system */
    #define  FS_MAX_OPEN_FILES_P        50

    /* Size of a physical block in bytes */
    #define  FS_PHYSICAL_BLKSIZE_P  256

    /* Maximum total number of blocks in any sector */
    #define  FS_MAX_BLKS_ALL_SECTORS_P   256

    /* Maximum total number of sectors in the device */
    #define  FS_NUM_SECTORS_P  40
    
    /* Maximum amount of file space to be managed by the file system */
    #define  FS_FILE_SPACE_LIMIT_P      0

    /* Maximum number of file block list nodes in the system */
    #define  FS_MAX_FBL_NODES_P         3000

    /* Number of entries per file block list node */
    #define  FS_FBL_ENTRIES_PER_NODE_P  1
  
  #endif /* FEATURE_PHONE_VR */ 

#endif /* FEATURE_NV_ITEM_MGR */

/* Maximum number of remote files */
#define  FS_MAX_REMOTE_FILES_P      10

/* Maximum size in bytes of transaction log system files */
// jnlee. 2000.2.10.
//#define  FS_MAX_TLOG_SIZE_P         476
//#define  FS_MAX_TLOG_SIZE_P         1000
#define  FS_MAX_TLOG_SIZE_P         1500        // for D1 phonebook 00.08.24 by white

// jnlee. end.

/* Maximum number of outstanding file space reservations */
#define  FS_MAX_RESERVATIONS_P      10

/* Maximum number of command packets to the file system task */
#define  FS_MAX_CMD_PACKETS_P       25

/* Maximum number of commands on the internal sub-command queue */
#define  FS_MAX_SUBCMDS_P           30

#endif /* FS_PARMS */

