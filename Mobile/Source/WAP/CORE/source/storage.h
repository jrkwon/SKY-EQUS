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
 * Storage.h
 *
 * Module for storing and retrieving data records on persistent memory.
 *
 * Created by Anders Edenbrandt.
 *
 * Revision history: 
 *   990926, AED: Redesigned.
 *
 */

#ifndef _Storage_h
#define _Storage_h

#include "tapicmmn.h"


/*
 * A function of this type is used for reading from persistent storage.
 *
 * Here, "pos" is the where the read should start, "len" is the
 * number of bytes to be read, and "buf" is where the data is placed.
 * Returns the number of bytes that were read.
 */
typedef
UINT32 ReadFunction (UINT32 pos, UINT32 len, VOID *buf);

/*
 * A function of this type is used for writing to persistent storage.
 *
 * Here, "pos" is the where the write should start, "len" is the
 * number of bytes to be written, and "buf" is where the data is fetched.
 * Returns the number of bytes that were written.
 */
typedef
UINT32 WriteFunction (UINT32 pos, UINT32 len, const VOID *buf);

/*
 * Each instance of a persistent store is represented by an object
 * of the following type. The user should consider the contents
 * to be "private", and not access any of the fields directly.
 */
typedef struct {
  UINT32             size;
  UINT32             bytesUsed;
  struct ListNode_st *blocklist;
  struct ListNode_st *currNode;
  UINT32             lastId;
  ReadFunction       *readStorage;
  WriteFunction      *writeStorage;
} StorageObject;

/*
 * Initialize the storage object to be of the indicated size.
 * "rfp" and "wfp" are pointers to the functions that will be used
 * to read and write from the store.
 * Returns FALSE in case of error, TRUE otherwise.
 */
BOOL Storage_Init (StorageObject *stobj, UINT32 size,
                   ReadFunction *rfp, WriteFunction *wfp);

/*
 * Prepare a storage object for close down.
 * After a call to this routine, the storage object can not be used
 * further. It will require a new call to Storage_Init first.
 */
void Storage_Finalize (StorageObject *stobj);

/*
 * Change the storage area to have the new indicated size.
 * This might require that some data is discarded, in which
 * case arbitrary data blocks might be deleted.
 * Returns FALSE in case of error.
 */
BOOL Storage_ChangeSize (StorageObject *stobj, UINT32 newSize);

/*
 * Compact the storage area, i.e., try to form larger free blocks.
 * This routine does not necessarily complete the compaction process,
 * rather it tries to make progress towards a complete compaction.
 * Returns TRUE if progress was made, FALSE otherwise.
 * As long as the function returns TRUE, it may be beneficial to
 * call the function again.
 */
BOOL Storage_Compact (StorageObject *stobj);

/*
 * Allocate a new block of a size that will hold "len" bytes of data.
 * "id" must be different from NULL. If *id != 0, then that number
 * is used as the Id for the new block, unless it is alreaday in use,
 * in which case FALSE is returned. If *id == 0, then a new unique
 * id is allocated, larger than any other currently in use.
 * Returns FALSE in case of error, e.g., if there is not enough
 * memory left to add a block of the indicated size.
 */
BOOL Storage_AllocateBlock (StorageObject *stobj, UINT32 len, UINT32 *id);

/*
 * Fetch the contents of a block, or part of a block.
 * The retrieved data is copied to the location indicated by "recData".
 * Returns FALSE in case of error, e.g., if the specified block
 * does not exist.
 */
BOOL Storage_Get (StorageObject *stobj, UINT32 id,
                  UINT32 start, UINT32 len, VOID *recData);

/*
 * Write the contents of an existing block, or part of a block.
 * The data written to the block is read from the location
 * indicated by "recData".
 * Returns FALSE in case of error, e.g., if the specified block
 * does not exist, or if the indicated start position and data length
 * adds up to a position beyond the current end of the block.
 */
BOOL Storage_Put (StorageObject *stobj, UINT32 id,
                  UINT32 start, UINT32 len, VOID *recData);

/*
 * Delete the block with given block ID.
 * Returns FALSE in case of error, e.g., if the indicated block
 * does not exist.
 */
BOOL Storage_DeleteBlock (StorageObject *stobj, UINT32 id);

/*
 * Return the size of the indicated block.
 * Returns 0 in case of error.
 */
UINT32 Storage_GetBlockSize (StorageObject *stobj, UINT32 id);

/*
 * Get the IDs of all blocks in the storage area.
 * Sets "*idarr" to point to an array of UINT32 values,
 * and "*len" to the number of elements in the array.
 * Returns FALSE in case of error.
 * NOTE: it is the caller's responsibility to deallocate the array.
 */
BOOL Storage_GetAllBlockIds (StorageObject *stobj,
                             UINT32 **idarr, UINT16 *len);

/*
 * Return the size of the largest block that could be allocated
 * if the storage area were completely compacted.
 * Returns 0 in case of error.
 */
UINT32 Storage_NumBytesFree (StorageObject *stobj);


/*
 * Return the number of bytes currently used in the allocated blocks.
 */
UINT32 Storage_BytesUsed (StorageObject *stobj);

#endif
