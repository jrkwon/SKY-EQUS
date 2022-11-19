#ifndef FS_ALLOC_H
#define FS_ALLOC_H
/*==========================================================================

                   E M B E D D E D   F I L E   S Y S T E M

DESCRIPTION

  Header for File System Allocation/Deallocation/Garbage-Collection 
  routines.  Also localizes all knowledge of file block internal structure.

Copyright (c) 1999 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header:   L:/src/asw/COMMON/vcs/FS_ALLOC.H_v   1.0   09 Nov 2000 08:38:20   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------

11-09-00   pg      Initial version for common archive.
01-31-00   pg      Added fs_alloc_read_fsequence_num() to support
                   fs_enum_next().
10-06-99   rp      Added "erased block" as a return from data_integrity_ok.
08-13-99   rp      Added support for block size larger than 256 bytes.
07-22-99   ls      Added fs_alloc_get_gc_state and get_gc_sector.
07-02-99   ls      Changed blk_volatility functions.
06-26-99   ls      Added blk_volatility functions.
04-25-99   ls      Added get_cleanblk_count function.
04-22-99   ls      Removed enclosing ifdef.
04-21-99   ls      Version 2.2 release.
04-03-99   ls      Changed interface per Version 2.1 specification.
03-04-99   ls      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "fsi.h"




/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_INIT_GC                                                       *
 *                                                                         *
 *  Sets GC state to allow garbage collection to run.                      *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_init_gc
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_GC_STATE                                                  *
 *                                                                         *
 *  Return current state of garbage collection.                            *
 *                                                                         *
 ***************************************************************************/
fsi_gc_state_type  fs_alloc_get_gc_state
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_GC_SECTOR                                                 *
 *                                                                         *
 *  Return number of the sector currently undergoing erasure.  If no       *
 *  sectors are currently undergoing erasure, return -1.                   *
 *                                                                         *
 ***************************************************************************/
int  fs_alloc_get_gc_sector
(
  void
);

 
/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FREEBLK_COUNT                                             *
 *                                                                         *
 *  Return the current count of free file system blocks.                   *
 *                                                                         *
 ***************************************************************************/
word  fs_alloc_get_freeblk_count
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FREEBLK_COUNT                                             *
 *                                                                         *
 *  Set the current count of free file system blocks.                      *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_freeblk_count
(
  word                    freeblk_count
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_RSVDBLK_COUNT                                             *
 *                                                                         *
 *  Return the current count of reserved file system blocks.               *
 *                                                                         *
 ***************************************************************************/
word  fs_alloc_get_rsvdblk_count
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_RSVDBLK_COUNT                                             *
 *                                                                         *
 *  Set the current count of reserved file system blocks.                  *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_rsvdblk_count
(
  word                    rsvdblk_count
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_CLEANBLK_COUNT                                            *
 *                                                                         *
 *  Return the current count of erased file system blocks.                 *
 *                                                                         *
 ***************************************************************************/
word  fs_alloc_get_cleanblk_count
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_TOTALBLK_COUNT                                            *
 *                                                                         *
 *  Return the total number of file system blocks.  This quantity may be   *
 *  reduced over time if sectors fail or permanently bad blocks are        *
 *  detected.                                                              *
 *                                                                         *
 ***************************************************************************/
word  fs_alloc_get_totalblk_count
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_TOTALBLK_COUNT                                            *
 *                                                                         *
 *  Set the total count of file system blocks.                             *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_totalblk_count
(
  word                    totalblk_count
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_NEW                                                           *
 *                                                                         *
 *  Select a free block for allocation.  The caller identifies which       *
 *  block (if any) will become invalided when this block is written.       *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_alloc_new
(
  fsi_alloc_type          *alloc_parms
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_STATUS                                                *
 *                                                                         *
 *  Set the block status field of the indicated block to the specified     *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_status
(
  fsi_generic_blk_type    *blk,
  fsi_blk_status_type     blk_status
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_RESERVED1                                             *
 *                                                                         *
 *  Set the block reserved1 field of the indicated block to the specified  *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_reserved1
(
  fsi_generic_blk_type    *blk,
  byte                    reserved1
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_OWNER_ID                                              *
 *                                                                         *
 *  Set the block owner-id field of the indicated block to the specified   *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_owner_id
(
  fsi_generic_blk_type    *blk,
  fsi_owner_id_type       owner_id
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_NUMBER                                                *
 *                                                                         *
 *  Set the block number field of the indicated block to the specified     *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_number
(
  fsi_generic_blk_type    *blk,
  fsi_blk_number_type     blk_number
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_VERSION                                               *
 *                                                                         *
 *  Set the block version field of the indicated block to the specified    *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_version
(
  fsi_generic_blk_type    *blk,
  fsi_blk_version_type    blk_version
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_RELOC_COUNT                                           *
 *                                                                         *
 *  Set the block relocation count field of the indicated block to the     *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_reloc_count
(
  fsi_generic_blk_type    *blk,
  fsi_blk_reloc_type      blk_reloc_count
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_DATA_COUNT                                            *
 *                                                                         *
 *  Set the block data count field of the indicated block to the specified *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_data_count
(
  fsi_generic_blk_type    *blk,
  word                    data_count
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FILE_ATTRIB_MASK                                          *
 *                                                                         *
 *  Set the file attribute mask field of the indicated block to the        *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_file_attrib_mask
(
  fsi_generic_blk_type    *blk,
  fs_attribute_mask_type  attrib_mask
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FILE_ATTRIB_MASK                                          *
 *                                                                         *
 *  Get the file attribute mask field of the indicated block.              *
 *                                                                         *
 ***************************************************************************/
fs_attribute_mask_type  fs_alloc_get_file_attrib_mask
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FILE_CREATION_DATE                                        *
 *                                                                         *
 *  Set the file creation date field of the indicated block to the         *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_file_creation_date
(
  fsi_generic_blk_type    *blk,
  fs_file_date_type       creation_date
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FILE_CREATION_DATE                                        *
 *                                                                         *
 *  Get the file creation date field of the indicated block.               *
 *                                                                         *
 ***************************************************************************/
fs_file_date_type  fs_alloc_get_file_creation_date
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FILE_BUFFER_OPT                                           *
 *                                                                         *
 *  Set the file buffering option field of the indicated block to the      *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_file_buffer_opt
(
  fsi_generic_blk_type      *blk,
  fs_buffering_option_type  buffering_option
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FILE_BUFFER_OPT                                           *
 *                                                                         *
 *  Get the file buffering option field of the indicated block.            *
 *                                                                         *
 ***************************************************************************/
fs_buffering_option_type  fs_alloc_get_file_buffer_opt
(
  fsi_generic_blk_type      *blk
);
 

/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FILE_FSEQUENCE_NUM                                        *
 *                                                                         *
 *  Set the file sequence number field of the indicated block to the       *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_file_fsequence_num
(
  fsi_generic_blk_type    *blk,
  fsi_fsequence_num_type  fsequence_num
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FILE_FSEQUENCE_NUM                                        *
 *                                                                         *
 *  Get the file sequence number field of the indicated block.             *
 *                                                                         *
 ***************************************************************************/
fsi_fsequence_num_type  fs_alloc_get_file_fsequence_num
(
  fsi_generic_blk_type    *blk
);
 
 
/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_READ_FSEQUENCE_NUM                                            *
 *                                                                         *
 *  Read the file sequence number field of the file header block at the    *
 *  specified block address.                                               *
 *                                                                         *
 ***************************************************************************/
fsi_fsequence_num_type  fs_alloc_read_fsequence_num
(
  fsi_file_adrs_type      blk_adrs
);

 
/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_FILE_CLEANUP_OPT                                          *
 *                                                                         *
 *  Set the file clean-up option field of the indicated block to the       *
 *  specified value.                                                       *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_file_cleanup_opt
(
  fsi_generic_blk_type    *blk,
  fs_cleanup_option_type  cleanup_option
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_FILE_CLEANUP_OPT                                          *
 *                                                                         *
 *  Get the file clean-up option field of the indicated block.             *
 *                                                                         *
 ***************************************************************************/
fs_cleanup_option_type  fs_alloc_get_file_cleanup_opt
(
  fsi_generic_blk_type    *blk
);

 
/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_BLK_FILENAME                                              *
 *                                                                         *
 *  Set the block data count field of the indicated block to the specified *
 *  value.                                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_blk_filename
(
  fsi_generic_blk_type    *blk,
  fsi_filename_type       *filename
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_BLK_FILENAME                                              *
 *                                                                         *
 *  Get the block filename field of the indicated block.                   *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_alloc_get_blk_filename
(
  fsi_generic_blk_type    *blk,
  fsi_filename_type       *filename

);
 

/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_EOD_POS                                                   *
 *                                                                         *
 *  Determine and return the end-of-data position for the specified        *
 *  block.  The end-of-data position is the next byte position after the   *
 *  last valid byte in a block (i.e., an appended byte could be written    *
 *  at this position).                                                     *
 *                                                                         *
 ***************************************************************************/
word  fs_alloc_get_eod_pos
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SET_DATA_INTEGRITY                                            *
 *                                                                         *
 *  Compute and set the block crc of the specified block.                  *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_set_data_integrity
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_DATA_INTEGRITY_OK                                             *
 *                                                                         *
 *  Verify the block crc and version number of the specified block.        *
 *  Return constant FSI_INTEGRITY_OK (1) if the crc is correct, and        *
 *  FSI_INTEGRITY_ERASED (2) if the block is erased; return constant       *
 *  FSI_INTEGRITY_BAD (0) otherwise, or if the block version number is     *
 *  invalid.                                                               *
 *                                                                         *
 ***************************************************************************/
int  fs_alloc_data_integrity_ok
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_COMPUTE_BLK_VOLATILITY                                        *
 *                                                                         *
 *  Compute the volatility of the specified block.                         *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_compute_blk_volatility
(
  fsi_blk_header_type     *blk_hdr,
  dword                   *blk_volatility
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GET_BLK_VOLATILITY                                            *
 *                                                                         *
 *  Read and compute the volatility of the specified block.                *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_get_blk_volatility
(
  word                    sector,
  word                    blk,
  dword                   *blk_volatility
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_LOAD_BLK_DATA                                                 *
 *                                                                         *
 *  Load data into the specified block, starting at the specified          *
 *  position, from the specified data buffer.  If the "finalize_blk"       *
 *  parameter is TRUE, set the data count, data checksum, and data         *
 *  integrity fields in the block.  The input data buffer index is         *
 *  decremented for each byte loaded.                                      *
 *                                                                         *
 *  On return, the specified position variable is updated with the new     *
 *  block position.  In addition, a "block invalidated" flag is returned   *
 *  which indicates whether the block's data changed in a way that         *
 *  requires that the block be relocated prior to being rewritten.         *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_alloc_load_blk_data
(
  fsi_generic_blk_type    *blk,
  word                    *blk_position,
  boolean                 *blk_invalidated,
  boolean                 finalize_blk,
  byte                    *data_buf,
  dword                   *data_buf_index,
  dword                   data_buf_max
);


#ifdef FEATURE_FS_HANDOVER_SUPPORT
/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_SCRUB_SECTORS                                                 *
 *                                                                         *
 *  Erase every sector that is not already completely erased that does not *
 *  contain any allocated blocks.  This function is normally used only     *
 *  during handover block processing to delete old file system contents.   *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_scrub_sectors
(
  void
);
#endif













/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GC_END                                                        *
 *                                                                         *
 *  This function is called at completion of the sector erase operation    *
 *  to reset sector statistics and clear in-progress flags, and to remove  *
 *  the garbage fbl list entries that were cleaned.                        *
 *                                                                         *
 ***************************************************************************/
void  fs_alloc_gc_end
(
  word                        insector
);


/***************************************************************************
 *                                                                         *
 *  FS_ALLOC_GC_EVAL                                                       *
 *                                                                         *
 *  Evaluate dynamic conditions and start garbage collection if            *
 *  appropriate.  Return TRUE if garbage collection was initiated.         *
 *                                                                         *
 ***************************************************************************/
boolean  fs_alloc_gc_eval
(
  fsi_gc_directives_type      gc_directive
);

#endif /* FS_ALLOC_H */

