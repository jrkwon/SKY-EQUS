/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
/*
 * aapifile.h
 *
 * Created by Anders Edenbrandt, Tue Oct 03 12:47:02 2000.
 *
 * Revision  history:
 *   010129, AED:  Added new function FILEa_setSize.
 *
 */
#ifndef _aapifile_h
#define _aapifile_h

#include "tapicmmn.h"

/*
 * File API where each file is represented by a category
 * (a character) and an integer ID.
 * Within each category, the file ID 0 is special in that
 * it represents a file that should always exist; i.e., it
 * should be possible to use file 0 without having done
 * any previous call to FILEa_create. Also, FILEa_delete
 * on file 0 should not be allowed.
 *
 * Currently the following categories are defined:
 *   'C'      cache
 *   'P'      push repository
 *
 */

/*
 * Create a new file within the indicated category,
 * setting *file_id to a new unique integer identifying the file.
 * Returns -1 in case of error, e.g., if the maximum number
 * of files allowed within the indicated category has
 * been exceeded.
 * Otherwise, returns 0.
 */
INT16
FILEa_create (CHAR category, UINT32 *file_id);

/*
 * Delete a file.
 */
void
FILEa_delete (CHAR category, UINT32 file_id);

/*
 * Fetch "count" bytes from position "pos" in the indicated file,
 * and store in "buf".
 * In case of error, returns -1.
 * Otherwise, returns the number of bytes actually read;
 * this may be less than the requested amount in case the length
 * of the file is less than pos + count.
 */
INT32
FILEa_read (CHAR category, UINT32 file_id,
            void *buf, UINT32 pos, UINT32 count);

/*
 * Write "count" bytes at position "pos" in the indicated file,
 * fetching the data from "buf".
 * In case of error, returns -1.
 * Otherwise, returns the number of bytes actually written;
 * this may be less than the requested amount in case the file
 * system is full.
 */
INT32
FILEa_write (CHAR category, UINT32 file_id,
             void *buf, UINT32 pos, UINT32 count);

/*
 * Return the size in bytes of the indicated file.
 * Returns -1 in case of error, e.g., if the file does
 * not exist.
 */
INT32
FILEa_getSize (CHAR category, UINT32 file_id);

/*
 * Set the size of the file, either extending it or
 * truncating it to the new size. If the requested size
 * is larger than what the system can accomodate,
 * the new size may be less than what was requested.
 * Returns the new size of the file, or -1 in case of error.
 */
INT32
FILEa_setSize (CHAR category, UINT32 file_id, UINT32 size);

/*
 * Force the buffered contents to be written to disk.
 * An implementation that uses internal buffering should
 * write all modified content to disk for the indicated file.
 */
void
FILEa_flush (CHAR category, UINT32 file_id);

/*
 * Retrieve a list of all file ids currently in use
 * within the indicated category. If file_ids != NULL,
 * then it points to a buffer large enough to hold at
 * least maxcount file ids.
 * If file_ids == NULL, then this routine should just
 * return the number of file_ids.
 * Returns the number of file ids, or -1 on error.
 * Note that this number should always be > 0, since
 * the file with file_id 0 is assumed to always exist,
 * and should be included in this count.
 */
INT16
FILEa_getFileIds (CHAR category, UINT32 *file_ids, UINT16 maxcount);

#endif
