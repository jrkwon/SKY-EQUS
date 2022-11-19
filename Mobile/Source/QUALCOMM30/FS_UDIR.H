#ifndef FS_UDIR_H
#define FS_UDIR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   E M B E D D E D   F I L E   S Y S T E M


GENERAL DESCRIPTION
  Header file for File System User Directory management routines.

Copyright (c) 1999 by QUALCOMM Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header:   L:/src/asw/COMMON/vcs/FS_UDIR.H_v   1.0   09 Nov 2000 08:38:48   pingguan  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11-09-00   pg      Initial version for common archive.
03-24-00   pg      Added fs_udir_enum_prev() to support fs_enum_prev().
01-31-00   pg      Added fs_udir_enum_next() to support fs_enum_next().
10-28-99   ls      Backward-merged Version 3.0 fixes.
04-22-99   ls      Removed enclosing ifdef.
04-21-99   ls      Version 2.2 release.
04-03-99   ls      Changed interface per Version 2.1 specification.
03-04-99   ls      Initial version.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_EXISTS                                                         *
 *                                                                         *
 *  Return TRUE if the system user directory file exists, FALSE otherwise. *
 *                                                                         *
 ***************************************************************************/
boolean  fs_udir_exists
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_FIND_UDIR_DINDEX                                               *
 *                                                                         *
 *  Return the master file directory entry index of the user directory     *
 *  file.  File is assumed to have already been created.                   *
 *                                                                         *
 ***************************************************************************/
int  fs_udir_find_udir_dindex
(
  void
);
 

/***************************************************************************
 *                                                                         *
 *  FS_UDIR_SPLIT_FILENAME                                                 *
 *                                                                         *
 *  Splits the input fully-qualified file name into its directory path     *
 *  and local file name.                                                   *
 *                                                                         *
 ***************************************************************************/
void  fs_udir_split_filename
(
  fsi_filename_type          *in_fname,    /* Input filename to split */
  fsi_filename_type          *out_path,    /* Output path name */
  fsi_filename_type          *out_fname    /* Output file name */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_EXTRACT_PATHS                                                  *
 *                                                                         *
 *  Extract the specified directory path level.                            *
 *                                                                         *
 *  For example, if the input name was "A/B/C/D/", specifying level=1      *
 *  returns "A/"; specifying level=2 returns "A/B/", and so on.  If the    *
 *  specified level exceeds the number of directory levels in the input    *
 *  name, a NULL path is returned (using this example, level=5 would       *
 *  return a NULL result).                                                 *
 *                                                                         *
 ***************************************************************************/
void  fs_udir_extract_paths
(
  fsi_filename_type          *in_name,     /* Input name to extract from */
  word                       level,        /* Number of levels to extract */
  fsi_filename_type          *out_path     /* Output extracted path name */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_SEARCH_USER_DIRS                                               *
 *                                                                         *
 *  Search the system "user directories" file for a directory name that    *
 *  matches the specified directory name.                                  *
 *                                                                         *
 *  If a match is found, set the return variable "found" to TRUE and set   *
 *  "found_pos" to the file position within the user directories file      *
 *  where the match was found.                                             *
 *                                                                         *
 *  If no match is found, set the return variable "found" to FALSE.        *
 *                                                                         *
 ***************************************************************************/
void  fs_udir_search_user_dirs
(
  word                       dindex,
  fsi_filename_type          *dirname,   /* Input directory name to match */
  boolean                    *found,     /* Output "found" flag */
  fsi_current_pointer_type   *found_pos  /* Output position where found */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_ADD_USER_DIRS                                                  *
 *                                                                         *
 *  Adds the specified directory name to the "user directories" system     *
 *  file.                                                                  *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_udir_add_user_dirs
(
  word                       dindex,
  fsi_filename_type          *dirname
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_REMOVE_USER_DIRS                                               *
 *                                                                         *
 *  Adds the specified directory name to the "user directories" system     *
 *  file.                                                                  *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_udir_remove_user_dirs
(
  word                       dindex,
  fsi_filename_type          *dirname,
  fsi_current_pointer_type   dirname_pos
);
 

/***************************************************************************
 *                                                                         *
 *  FS_UDIR_CREATE_USER_DIRS                                               *
 *                                                                         *
 *  Creates an initial "user directories" system file.                     *
 *                                                                         *
 ***************************************************************************/
void  fs_udir_create_user_dirs
(
  void
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_LIST_USER_DIRS                                                 *
 *                                                                         *
 *  List all user file directories starting at the specified directory.    *
 *                                                                         *
 ***************************************************************************/
void  fs_udir_list_user_dirs
(
  word                     dindex,
  fsi_filename_type        *dirname,
  byte                     *list_buf,     /* Client's buf to receive data */
  dword                    *list_buf_size /* Size of client's buffer      */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_ENUM_NEXT                                                      *
 *                                                                         *
 *  List the "next" user directory (according to creation order) following *
 *  the specified user directory.                                          *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_udir_enum_next
(
  fsi_filename_type          *root_dir,     /* Root directory name       */
  fsi_filename_type          *last_dir,     /* Last dir name returned    */
  dword                      *sequence_num, /* Current enum sequence num */
  fs_enum_data_type          *finfo         /* Returned file information */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_ENUM_PREV                                                      *
 *                                                                         *
 *  List the "prev" user directory (according to creation order) following *
 *  the specified user directory.                                          *
 *                                                                         *
 ***************************************************************************/
fs_status_type  fs_udir_enum_prev
(
  fsi_filename_type          *root_dir,     /* Root directory name       */
  fsi_filename_type          *last_dir,     /* Last dir name returned    */
  dword                      *sequence_num, /* Current enum sequence num */
  fs_enum_data_type          *finfo         /* Returned file information */
);


/***************************************************************************
 *                                                                         *
 *  FS_UDIR_GET_NDIRS                                                      *
 *                                                                         *
 *  Return the number of subdirectories in the specified user directory    *
 *  (subdirectories of subdirectories are not included in the count).      *
 *                                                                         *
 ***************************************************************************/
word  fs_udir_get_ndirs
(
  fsi_filename_type        *dirname     /* Count subdirs in this directory */
);
#endif /* FS_UDIR_H */
