#ifndef FS_DIR_H
#define FS_DIR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   E M B E D D E D   F I L E   S Y S T E M


GENERAL DESCRIPTION
  Header file for File System Master File Directory and Sector Status Table 
  management routines.

Copyright (c) 1999 by QUALCOMM Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/FS_DIR.H_v   1.0   09 Nov 2000 08:38:34   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------

11-09-00   pg      Initial version for common archive.
08-22-00   dlb     Remove ^Z from end of file.
07-25-00   pg      Added support to reserve some blocks for NV-writes.
07-24-00   pg      Renamed fsi_dir_kill_blocks to fs_dir_kill_blocks and
                   maked it a global function.
03-24-00   pg      Added fs_dir_enum_prev() function.
02-14-00   pg      Changed fs_dir_enum_next() interface to support the
                   operation on remote links.
01-31-00   pg      Added fs_dir_enum_next() to support fs_enum_next().
01-21-00   pg      Added fs_dir_rename_file function 
                   and fs_dir_copy_file function.
12-16-99   rp      Added fs_dir_remove_remote_files function.
11-01-99   ls      Added support for move of tlog_index to master directory.
10-28-99   ls      Backward-merged Version 3.0 fixes.
06-27-99   ls      Added blk_volatility functions.
06-22-99   ls      Changed transaction logging to support user revertable
                   files across multiple garbage collections.
05-21-99   ls      Added fmemset function.
04-29-99   ls      Added suspended_writes_ok function.
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
 *  FS_DIR_GET_NUM_SECTORS                                                 *
 *                                                                         *
 *  Return the number of sectors in the file system device.                *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_num_sectors
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_NUM_BLKS                                                    *
 *                                                                         *
 *  Return the number of blocks in the specified sector.                   *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_num_blks
(
  word                        sector
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SUSPENDED_WRITES_OK                                             *
 *                                                                         *
 *  Return TRUE if the device can support writing while an erase operation *
 *  is suspended, FALSE otherwise.                                         *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_suspended_writes_ok
(
  void
);
 
 
/***************************************************************************
 *                                                                         *
 *  FS_DIR_MAP_ADRS                                                        *
 *                                                                         *
 *  Determine a physical device offset corresponding to the input block    *
 *  address.                                                               *
 *                                                                         *
 ***************************************************************************/
dword  fs_dir_map_adrs
(
  fsi_file_adrs_type          blk_adrs
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_MAP_OFFSET                                                      *
 *                                                                         *
 *  Determine the block address corresponding to the input physical device *
 *  address.                                                               *
 *                                                                         *
 ***************************************************************************/
fsi_file_adrs_type  fs_dir_map_offset
(
  dword                       offset
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_COPY_BLK                                                        *
 *                                                                         *
 *  Local version of device write-bytes routine, using block addresses     *
 *  instead of physical offsets.                                           *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_copy_blk
(
  fsi_file_adrs_type     dest_blk,
  fsi_file_adrs_type     source_blk
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_HI_FSEQUENCE_NUM                                            *
 *                                                                         *
 ***************************************************************************/
fsi_fsequence_num_type  fs_dir_get_hi_fsequence_num
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_SS_VOLATILITY_SUM                                           *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_get_ss_volatility_sum
(
  word                       sindex,
  dword                      *volatility_sum
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_SS_VOLATILITY_AVG                                           *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_get_ss_volatility_avg
(
  word                       sindex,
  dword                      *volatility_avg
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_UPDATE_SS_VOLATILITY                                            *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_update_ss_volatility
(
  word                       sindex,
  dword                      blk_volatility,
  boolean                    addition
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_SS_TOTAL_COUNT                                              *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_ss_total_count
(
  word                       sindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_SS_ALLOC_COUNT                                              *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_ss_alloc_count
(
  word                       sindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_SS_ALLOC_COUNT                                              *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_ss_alloc_count
(
  word                       sindex,
  word                       alloc_count
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_SS_GARBAGE_COUNT                                            *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_ss_garbage_count
(
  word                       sindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_LARGEST_SECTOR                                              *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_largest_sector
(
  word                       blk_count
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_LARGEST_SECTOR                                              *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_largest_sector
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_GC_RATING                                                   *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_gc_rating
(
  word                       insector,
  byte                       inrating
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_GC_RATING                                                   *
 *                                                                         *
 ***************************************************************************/
byte  fs_dir_get_gc_rating
(
  word                       insector
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_HI_SS_GC_RATING                                             *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_get_hi_ss_gc_rating
(
  word                       *outsector,
  byte                       *outrating
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_CLEAR_SS_ENTRY                                                  *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_clear_ss_entry
(
  word                       insector
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_BLK_STATUS                                                  *
 *                                                                         *
 ***************************************************************************/
byte  fs_dir_get_blk_status
(
  fsi_file_adrs_type         blk_adrs
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_BLK_OWNER_ID                                                *
 *                                                                         *
 ***************************************************************************/
fsi_owner_id_type  fs_dir_get_blk_owner_id
(
  fsi_file_adrs_type         blk_adrs
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FILENAME                                                    *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_get_filename
(
  fsi_access_filename_type   *inparm
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_REMOTE_FILENAME                                             *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_get_remote_filename
(
  fsi_access_filename_type   *inparm
);
 

/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FILENAME_LENGTH                                             *
 *                                                                         *
 ***************************************************************************/
byte  fs_dir_get_filename_length
(
  fsi_file_adrs_type         inparm
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_COMPARE_FILENAMES                                               *
 *                                                                         *
 *  Compares the two input file names and returns a byte representing      *
 *  "less than" (if the first file name is less than the second file       *
 *  name), "equal", or "greater than."                                     *
 *                                                                         *
 ***************************************************************************/
fsi_comparison_type  fs_dir_compare_filenames
(
  fsi_filename_type          *fname1,
  fsi_filename_type          *fname2
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_COMPARE_STORED_FILENAMES                                        *
 *                                                                         *
 *  Compares the file name stored in the first specified file block        *
 *  against the file name stored in the second specified file block.       *
 *  Returns a byte representing "less than" (if the first file name is     *
 *  less than the second file name), "equal", or "greater than."           *
 *                                                                         *
 ***************************************************************************/
fsi_comparison_type  fs_dir_compare_stored_filenames
(
  fsi_compare_filenames_type  *inparm
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_BLK_IS_GARBAGE                                                  *
 *                                                                         *
 *  Higher-level version of "fs_dir_get_garbage_list_bit" for use by       *
 *  clients without visibility to the sector status table.  Inputs to      *
 *  this routine are just a sector number and a bit index.                 *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_blk_is_garbage
(
  fsi_file_adrs_type      blk_adrs
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_BLK_IS_MARKED                                                   *
 *                                                                         *
 *  Returns TRUE if the input block status value represents an explicitly  *
 *  marked garbage block (i.e., if the garbage bit in the block status     *
 *  value is cleared).                                                     *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_blk_is_marked
(
  byte                    blk_status
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_MARK_BLK                                                        *
 *                                                                         *
 *  Clears the explicit garbage status bit in the input block status       *
 *  value, which marks the block as garbage.                               *
 *                                                                         *
 ***************************************************************************/
byte  fs_dir_mark_blk
(
  byte                    blk_status
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_BLK_IS_ERASED                                                   *
 *                                                                         *
 *  Returns TRUE if the specified block is set to all-erased bytes, and    *
 *  FALSE otherwise.                                                       *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_blk_is_erased
(
  fsi_generic_blk_type    *blk
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_INCREMENT_BLK_PTR                                               *
 *                                                                         *
 *  "Increments" the input file block address pointer to the next block    *
 *  in the file.                                                           *
 *                                                                         *
 ***************************************************************************/
fsi_fbl_element_index_type  fs_dir_increment_blk_ptr
(
  fsi_fbl_element_index_type  eindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_FIND_FBL_EINDEX                                                 *
 *                                                                         *
 *  Find the fbl entry index corresponding to the input node data in the   *
 *  specified file block list.                                             *
 *                                                                         *
 ***************************************************************************/
fsi_fbl_element_index_type  fs_dir_find_fbl_eindex
(
  word                        dindex,     /* Master Directory table index */
  fsi_fbl_list_id_type        fbl_list,   /* Which file block list */ 
  fsi_fbl_node_data_type      node_data   /* Node data to find */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_INFO_AT_BLK_PTR                                             *
 *                                                                         *
 *  Sets the file block address and block version number at the fbl        *
 *  element pointed to.                                                    *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_info_at_blk_ptr
(
  fsi_fbl_element_index_type  eindex,
  fsi_fbl_node_data_type      node_data
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_INFO_AT_BLK_PTR                                             *
 *                                                                         *
 *  Returns the file block address and block version number at the fbl     *
 *  element pointed to.                                                    *
 *                                                                         *
 ***************************************************************************/
fsi_fbl_node_data_type  fs_dir_get_info_at_blk_ptr
(
  fsi_fbl_element_index_type  eindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_INVALIDATE_SBLKPTR                                              *
 *                                                                         *
 *  Traverses the checkpointed blocks fbl list (sblkptr) for the specified *
 *  file and makes blocks garbage only if they do not match the current    *
 *  file's fbl list.  All nodes are removed and the list is NULLed.        *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_invalidate_sblkptr
(
  word                        dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_FIND_HIGHEST_VERSION                                            *
 *                                                                         *
 *  Search a specified file's garbage blocks list for all blocks with the  *
 *  specified block ordinal number; return the highest version number      *
 *  among these blocks.  If no blocks are found with the specified ordinal *
 *  number, return a version number of zero (which represents "empty").    *
 *                                                                         *
 ***************************************************************************/
fsi_blk_version_type  fs_dir_find_highest_version
(
  fsi_blk_number_type         inblknum,
  word                        dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_FIND_HI_BLK_OUTSIDE_SECT                                        *
 *                                                                         *
 *  Search a specified file's garbage blocks list for all blocks with the  *
 *  specified block ordinal number that are located outside the specified  *
 *  sector; return the fbl element index of the block with the highest     *
 *  version number among these.  If no blocks are found with the specified *
 *  ordinal number outside the specified sector, return a NULL fbl list    *
 *  index.                                                                 *
 *                                                                         *
 ***************************************************************************/
fsi_fbl_element_index_type  fs_dir_find_hi_blk_outside_sect
(
  byte                        insector,
  fsi_blk_number_type         inblknum,
  word                        dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FBL_LIST                                                    *
 *                                                                         *
 *  Copy the specified file's fbl list to the specified buffer.            *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_get_fbl_list
(
  word                        dindex,
  void                        *buffer,
  word                        *buf_size
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_SBLKPTR                                                     *
 *                                                                         *
 *  Copy the entire fbl list of the file specified by dindex to its        *
 *  sblkptr list.                                                          *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_sblkptr
(
  word                        dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_FILE_CHECKPOINTED                                               *
 *                                                                         *
 *  Return TRUE if the specified file currently has checkpointed blocks    *
 *  associated with it (i.e., if the file's sblkptr is non-NULL); return   *
 *  FALSE otherwise (i.e., if the file has no sblkptr or if its sblkptr    *
 *  is currently NULL).                                                    *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_file_checkpointed
(
  word                        dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_TLOG_INDEX                                                  *
 *                                                                         *
 *  Set the tlog_index field of the specified master file directory entry  *
 *  to the specified value.                                                *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_tlog_index
(
  word                        dindex,
  word                        tlog_index
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_TLOG_INDEX                                                  *
 *                                                                         *
 *  Get the tlog_index field of the specified master file directory entry. *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_tlog_index
(
  word                        dindex
);

 
/***************************************************************************
 *                                                                         *
 *  FS_DIR_FIND_FILENAME_INDEX                                             *
 *                                                                         *
 *  Do a binary search of the Master File Directory for the index of the   *
 *  specified file name.                                                   *
 *                                                                         *
 *  The return value will be negative if the file is not found.            *
 *                                                                         *
 ***************************************************************************/
int  fs_dir_find_filename_index
(
  fsi_filename_type        *infilename
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_REMOVE_REMOTE_FILES                                             *
 *                                                                         *
 *  Remove all remote file links.  All remote file entries are deleted     *
 *  from the Master File Directory and the system remote links file is     *
 *  deleted.                                                               *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_remove_remote_files
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_ADRS                                                        *
 *                                                                         *
 *  Return the block address of the file header block from the specified   *
 *  entry of the Master Directory table.                                   *
 *                                                                         *
 ***************************************************************************/
fsi_file_adrs_type  fs_dir_get_adrs
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_ADRS                                                        *
 *                                                                         *
 *  Sets the block address of the file header block in the specified       *
 *  entry of the Master Directory table.                                   *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_adrs
(
  word                     dindex,
  fsi_file_adrs_type       new_addr
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FSIZE                                                       *
 *                                                                         *
 *  Gets the current file size from the specified entry of the Master      *
 *  Directory table.                                                       *
 *                                                                         *
 ***************************************************************************/
dword  fs_dir_get_fsize
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_FSIZE                                                       *
 *                                                                         *
 *  Sets the current file size field in the specified entry of the         *
 *  Master Directory table.                                                *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_set_fsize
(
  word                     dindex,
  dword                    new_size
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_NUMBLKS                                                     *
 *                                                                         *
 *  Gets the current number of blocks used by the file from the specified  *
 *  entry of the Master Directory table.                                   *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_numblks
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_REMOTE_PTR                                                  *
 *                                                                         *
 *  Gets the remote pointer (index into the system "remote files" file)    *
 *  from the specified entry of the Master Directory table.                *
 *                                                                         *
 ***************************************************************************/
dword  fs_dir_get_remote_ptr
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_ATTRIBUTE_MASK                                              *
 *                                                                         *
 *  Return the attribute mask for the file referenced by the specified     *
 *  entry of the Master Directory table.                                   *
 *                                                                         *
 ***************************************************************************/
fs_attribute_mask_type  fs_dir_get_attribute_mask
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_CLEANUP_OPTION                                              *
 *                                                                         *
 *  Return the bad powerdown file clean-up option for the file referenced  *
 *  by the specified entry of the Master Directory table.                  *
 *                                                                         *
 ***************************************************************************/
fs_cleanup_option_type  fs_dir_get_cleanup_option
(
  word                     dindex
);
 

/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_OWNER_ID                                                    *
 *                                                                         *
 *  Gets the file owner identification from the specified entry of the     *
 *  Master Directory table.                                                *
 *                                                                         *
 ***************************************************************************/
fsi_owner_id_type  fs_dir_get_owner_id
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FBLKPTR                                                     *
 *                                                                         *
 *  Gets the file block list pointer from the specified entry of the       *
 *  Master Directory table.                                                *
 *                                                                         *
 ***************************************************************************/
fsi_list_index_type  fs_dir_get_fblkptr
(
  word                     dindex
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_DIR_INDEX                                                   *
 *                                                                         *
 *  Return the Master Directory Table index corresponding to the input     *
 *  owner ID value.                                                        *
 *                                                                         *
 *  The return value will be negative if the owner ID isn't found.         *
 *                                                                         *
 ***************************************************************************/
int  fs_dir_get_dir_index
(
  fsi_owner_id_type        owner_id
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_DEL_FBL_GARBAGE_ENTRY                                           *
 *                                                                         *
 *  Delete the specified entry of the specified file's garbage blocks fbl  *
 *  list and compress the fbl list; free the enclosing fbl node if it      *
 *  becomes empty.                                                         *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_del_fbl_garbage_entry
(                           
  word                         dindex,     /* Master Directory index */
  fsi_fbl_node_data_type       innode_data /* Data in fbl entry to remove */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_SET_FILE_ATTRIBS                                                *
 *                                                                         *
 *  Set the specified file's attributes to the specified values.           *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_set_file_attribs
(
  word                      dindex,         /* Master Directory index */
  fs_attribute_mask_type    attrib_mask,    /* Attribute mask for file */
  fs_file_date_type         creation_date,  /* File creation date */
  fs_buffering_option_type  buffer_option,  /* File buffering option */
  fs_cleanup_option_type    cleanup_option  /* File clean-up option */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_FILE_ATTRIBS                                                *
 *                                                                         *
 *  Get the specified file's attributes.                                   *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_get_file_attribs
(
  word                      dindex,         /* Master Directory index */
  fs_attribute_mask_type    *attrib_mask,   /* Attribute mask for file */
  fs_file_date_type         *creation_date, /* File creation date */
  fs_buffering_option_type  *buffer_option, /* File buffering option */
  fs_cleanup_option_type    *cleanup_option /* File clean-up option */
);
 
 
/***************************************************************************
 *                                                                         *
 *  FS_DIR_SEARCH_USER_DIRS                                                *
 *                                                                         *
 *  Search the system "user directories" file for a directory name that    *
 *  matches the specified directory name.                                  *
 *                                                                         *
 *  Return TRUE if a match is found, FALSE otherwise.                      *
 *                                                                         *
 ***************************************************************************/
boolean  fs_dir_search_user_dirs
(
  fsi_filename_type          *dirname
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_ADD_USER_DIRS                                                   *
 *                                                                         *
 *  Adds the specified directory name to the "user directories" system     *
 *  file.                                                                  *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_add_user_dirs
(
  fsi_filename_type          *dirname
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_CREATE                                                          *
 *                                                                         *
 *  Creates the (RAM-resident) Master Directory Table and Sector Status    *
 *  Table by reading all the file system blocks and extracting the         *
 *  relevant information.                                                  *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_create
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_WRITE_TO_FILE                                                   *
 *                                                                         *
 *  Writes data to a file starting at the current file pointer position.   *
 *  If the block being written-to is invalidated by the new data, a new    *
 *  block is allocated, the data is relocated, and the fbl list is         *
 *  updated.  If the block is a new block for the file, a new fbl entry    *
 *  is added to the file's fbl list.  If the block being updated is a file *
 *  header block, the Master Directory is updated to reflect the location  *
 *  of the new file header block.  Any blocks that become garbage are      *
 *  added to the Sector Status table's garbage blocks list.  The current   *
 *  file size, file position, and block count attributes in the Master     *
 *  Directory table are updated.                                           *
 *                                                                         *
 *  If the "erased_data" parameter is TRUE, only erased bytes are written  *
 *  and any new complete blocks are left in a virgin state so they can be  *
 *  overwritten in-place during subsequent write operations (equivalent to *
 *  pre-allocating erased file system blocks).  Otherwise, the block's     *
 *  data count, data checksum, and data integrity fields are set when the  *
 *  block becomes full or the end of the input data is reached.            *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_write_to_file
(
  word                       dindex,         /* Master Directory index */
  fs_file_position_type      write_position, /* Start write position */
  boolean                    erased_data,    /* Writing erased data? */
  byte                       *buf,           /* Input data buffer */
  dword                      *buf_length,     /* Length of data in buffer */
  fsi_op_req_task_type       req_task         /* Task requesting the operation */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_WRITE_BLANKS_TO_FILE                                            *
 *                                                                         *
 *  Writes erased bytes to a file starting at the current file pointer     *
 *  position.                                                              *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_write_blanks_to_file
(
  word                       dindex,         /* Master Directory index */
  fs_file_position_type      write_position, /* Start write position */
  dword                      *length         /* Number of bytes to write */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_FMEMSET                                                         *
 *                                                                         *
 *  Writes a specified character to the file starting at the current file  *
 *  pointer position.                                                      *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_fmemset
(
  word                       dindex,         /* Master Directory index */
  byte                       set_char,       /* Character to write */
  fs_file_position_type      write_position, /* Start write position */
  dword                      *length         /* Number of bytes to write */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_READ_FROM_FILE                                                  *
 *                                                                         *
 *  Reads bytes from a specified file starting at the specified read       *
 *  position.                                                              *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_read_from_file
(
  word                        dindex,        /* Master Directory index */
  fs_file_position_type       read_position, /* Start read position */
  byte                        *buf,          /* Buffer to receive data */
  dword                       *buf_length    /* Max number bytes to read */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_READ_FROM_REMOTE_FILE                                           *
 *                                                                         *
 *  Reads bytes from a specified remote file starting at the specified     *
 *  read position.                                                         *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_read_from_remote_file
(
  word                        dindex,        /* Master Directory index */
  fs_file_position_type       read_position, /* Start read position */
  byte                        *buf,          /* Buffer to receive data */
  dword                       *buf_length    /* Max number bytes to read */
);
 

/***************************************************************************
 *                                                                         *
 *  FS_DIR_TRUNCATE_FILE                                                   *
 *                                                                         *
 *  Truncates a file from the current file pointer position to the end     *
 *  of the file.  Invalidated blocks are marked as garbage and added to    *
 *  the Sector Status tables' garbage blocks list.  If the truncate        *
 *  position falls within an existing block, that block is relocated.      *
 *  If that block is a file header block, the Master Directory is updated  *
 *  to reflect the location of the new file header block.  The current     *
 *  file size, file position, and block count attributes in the Master     *
 *  Directory table are updated.                                           *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_truncate_file
(
  word                       dindex,           /* Master Directory index */
  fs_file_position_type      truncate_position /* Start truncate position */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_CREATE_FILE                                                     *
 *                                                                         *
 *  Builds a file header block and writes it to the file system, and adds  *
 *  the new file to the master file directory.  Remote files are handled   *
 *  differently: instead of writing an actual file system block, the       *
 *  remote file name is added to the system "remote files" file.           *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_create_file
(
  fsi_filename_type         *filename,        /* Name of file to create */
  fs_attribute_mask_type    attrib_mask,      /* Attribute mask for file */
  fs_file_date_type         creation_date,    /* File creation date */
  fs_cleanup_option_type    cleanup_option,   /* File clean-up option */
  fs_buffering_option_type  buffering_option, /* File buffering option */
  dword                     base_address,     /* Only use for remote files */
  dword                     remote_fsize,     /* Only use for remote files */
  fsi_file_type             file_type,        /* User or system file */
  word                      *dindex           /* Returned master dir index */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_REMOVE_FILE                                                     *
 *                                                                         *
 *  Remove a file from the file system, including all list and directory   *
 *  updates.                                                               *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_remove_file
(
  word                       dindex      /* Master File Directory index */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_RENAME_FILE                                                     *
 *                                                                         *
 *  Rename a file.  The old and new file names must be the same length     *
 *  (to rename using a name of a different length, file copy must be used  *
 *  instead).                                                              *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_rename_file
(
  fsi_filename_type          *old_filename,
  fsi_filename_type          *new_filename
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_COPY_FILE                                                       *
 *                                                                         *
 *  Copy the contents of file "source_file" starting at the specified      *
 *  "source_pos" to file "dest_file" starting at the specified "dest_pos". *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_copy_file
(
  fsi_filename_type          *source_file,
  fs_file_position_type      source_pos,
  fsi_filename_type          *dest_file,
  fs_file_position_type      dest_pos
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_LIST_FILES                                                      *
 *                                                                         *
 *  List all files in a specified user directory (not including other      *
 *  directory files).                                                      *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_list_files
(
  fsi_filename_type      *dirname,      /* List contents of this directory */
  byte                   *list_buf,     /* Client's buffer to receive data */
  dword                  *list_buf_size,/* Size of client's buffer         */
  boolean                expand         /* Expanded or partial file names  */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_ENUM_NEXT                                                       *
 *                                                                         *
 *  List the "next" file (according to creation order) following the       *
 *  specified file in the specified user directory (not including other    *
 *  directory files).                                                      *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_enum_next
(
  fsi_filename_type        *dirname,      /* Root directory name          */
  dword                    *sequence_num, /* Current file sequence number */
  word                     *remote_index, /* Number of remote links processed */
  fs_enum_data_type        *finfo         /* Returned file information    */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_ENUM_PREV                                                       *
 *                                                                         *
 *  List the "prev" file (according to creation order) following the       *
 *  specified file in the specified user directory (not including other    *
 *  directory files).                                                      *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_dir_enum_prev
(
  fsi_filename_type        *dirname,      /* Root directory name          */
  dword                    *sequence_num, /* Current file sequence number */
  word                     *remote_index, /* Current remote file index    */
  fs_enum_data_type        *finfo         /* Returned file information    */
);


/***************************************************************************
 *                                                                         *
 *  FS_DIR_GET_NFILES                                                      *
 *                                                                         *
 *  Return the number of files in the specified user directory (not        *
 *  including other directory files).                                      *
 *                                                                         *
 ***************************************************************************/
word  fs_dir_get_nfiles
(
  fsi_filename_type        *dirname     /* Count files in this directory */
);
 

/***************************************************************************
 *                                                                         *
 *  FS_DIR_KILL_BLKS                                                      *
 *                                                                         *
 *  Marks as corrupted every block in the specified sector.                *
 *                                                                         *
 ***************************************************************************/
void  fs_dir_kill_blks
(                           
  byte                       sector
);
#endif /* FS_DIR_H */

