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
 * Storage.c
 *
 * Module for storing and retrieving data records on persistent memory.
 *
 * Created by Anders Edenbrandt.
 *
 * Revision history: 
 *   990926, AED: Redesigned
 *   991019, AED: Fixed missing updates of currNode
 *   991110, AED: Fixed bug in ScanStorage
 *   991111, AED: Removed the use of currNode
 *   001213, AED: Conditional inclusion depending on whether
 *                any other module needs these routines.
 *
 */
#include "confvars.h"
#include "storage.h"
#include "cmmnrsrc.h"

#if USE_STORAGE

/*
 * The storage area will be divided into blocks of varying sizes.
 * Each block will have a two-word header, of 8 bytes. The first word
 * will hold the unique block ID, and the second word will hold the
 * block size. A block ID of 0 indicates that the block is currently
 * not allocated. All blocks have a size that is a multiple of 8 bytes.
 * 
 * At startup, the storage is scanned and all the block headers are read.
 * Using the information thus collected, a list is created in
 * main memory: this is called the block list, and it has one node
 * for each block and is sorted in order of positions in the storage area.
 *
 */

#define MINBLOCKSIZE   16      /* Minimum allowed size of a block */
#define START_ID       0x400   /* Block IDs 1-1024 are reserved, i.e.,
                                * this system will never allocate them. */


/*
 * The nodes in the block list:
 */
typedef struct ListNode_st {
  struct ListNode_st *next;
  UINT32             pos;
  UINT32             size;
  UINT32             id;
} ListNode;

/*
 * Each block in the storage has a header:
*/
typedef struct {
  UINT32   size;
  UINT32   id;
} BlockHeader;

/*
 * Macro to copy the contents of a block list node to the store.
 */
#define PUTHEADER(_stobj, _p) do { \
  BlockHeader hd; \
  hd.size = (_p)->size; \
  hd.id = (_p)->id; \
  (_stobj)->writeStorage ((_p)->pos, sizeof (BlockHeader), &hd); \
} while (0)


/*
 * Find the node in the block list with given ID.
 */
static ListNode *
IdToBlock (StorageObject *stobj, UINT32 id)
{
  ListNode *p;

  for (p = stobj->blocklist; p; p = p->next) {
    if (p->id == id)
      break;
  }

  return p;
}

/*
 * Return a new, unique ID, larger than any of IDs currently in use.
 */
static UINT32
AllocNewId (StorageObject *stobj)
{
  stobj->lastId += 1;

  return stobj->lastId;
}

/*
 * Scan the storage area, reading all block headers,
 * and create the block list.
 */
static BOOL
ScanStorage (StorageObject *stobj, UINT32 size)
{
  ListNode    *p, *np;
  BlockHeader hd;
  UINT32      pos;
  UINT32      oldsize, remsize;
  UINT32      largestId = START_ID;

  stobj->readStorage (0, 4, &oldsize);
  if (oldsize == 0) {
    /* The storage area has not been used before: */
    p = NEWARRAY (ListNode, 1);
    if (p == NULL)
      return 0;
    p->next = NULL;
    p->pos = 4;
    p->size = size - 4;
    p->id = 0;
    PUTHEADER (stobj, p);
    stobj->writeStorage (0, 4, &size);
    stobj->lastId = largestId;
    stobj->blocklist = p;

    return TRUE;
  }

  p = NULL;
  pos = 4;
  remsize = size - 4;
  while ((remsize >= MINBLOCKSIZE) && (pos + sizeof (BlockHeader) < oldsize)) {
    stobj->readStorage (pos, sizeof (BlockHeader), &hd);
    largestId = MAX (largestId, hd.id);

    if (hd.size > remsize) {
      /* We cannot fit all of this block: turn it into a free area */
      if ((p == NULL) || (p->id != 0)) {
        np = NEWARRAY (ListNode, 1);
        if (np == NULL)
          return FALSE;
        np->next = NULL;
        np->pos = pos;
        np->size = remsize;
        np->id = 0;
        PUTHEADER (stobj, np);
        if (p != NULL)
          p->next = np;
        else
          stobj->blocklist = np;
      }
      else {
        /* Join with previous free block */
        p->size += remsize;
        PUTHEADER (stobj, p);
      }
      remsize = 0;
      break;
    }

    /* The normal case: */
    np = NEWARRAY (ListNode, 1);
    if (np == NULL)
      return FALSE;
    np->next = NULL;
    np->pos = pos;
    np->size = hd.size;
    np->id = hd.id;
    if (p != NULL)
      p->next = np;
    else
      stobj->blocklist = np;
    if (np->id != 0)
      stobj->bytesUsed += np->size;

    pos += np->size;
    remsize -= np->size;
    p = np;
  } /* End of while loop */

  if (remsize > 0) {
    if ((p != NULL) && (p->id == 0)) {
      /* Join with previous free block */
      p->size += remsize;
      PUTHEADER (stobj, p);
    }
    else if (remsize >= MINBLOCKSIZE) {
      np = NEWARRAY (ListNode, 1);
      if (np == NULL)
        return FALSE;
      np->next = NULL;
      np->pos = pos;
      np->size = remsize;
      np->id = 0;
      PUTHEADER (stobj, np);
      if (p != NULL)
        p->next = np;
      else
        stobj->blocklist = np;
    }
  }

  stobj->lastId = largestId;
  stobj->writeStorage (0, 4, &size);
  
  return TRUE;
}

/*
 * Initialize the storage object to be of the indicated size.
 * "rfp" and "wfp" are pointers to the functions that will be used
 * to read and write from the store.
 * Returns FALSE in case of error, TRUE otherwise.
 */
BOOL
Storage_Init (StorageObject *stobj, UINT32 size,
              ReadFunction *rfp, WriteFunction *wfp)
{
  if (stobj == NULL) {
    return FALSE;
  }

  if (size < MINBLOCKSIZE + 4)
    return FALSE;

  /* Truncate to nearest multiple of 8 */
  size = ((size + 4) & ~7) - 4;

  stobj->size = size;
  stobj->bytesUsed = 0;
  stobj->blocklist = NULL;
  stobj->lastId = START_ID;

  stobj->readStorage = rfp;
  stobj->writeStorage = wfp;

  if (!ScanStorage (stobj, size)) {
    return FALSE;
  }

  return TRUE;
}

/*
 * Prepare a storage object for close down.
 * After a call to this routine, the storage object can not be used
 * further. It will require a new call to Storage_Init first.
 */
void
Storage_Finalize (StorageObject *stobj)
{
  ListNode *p, *tmp;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return;
  }

  /* Free all nodes in the block list. */
  p = stobj->blocklist;
  while (p) {
    tmp = p;
    p = p->next;
    DEALLOC (&tmp);
  }
  stobj->blocklist = NULL;
  stobj->size = 0;
  stobj->bytesUsed = 0;
}

/*
 * Change the storage area to have the new indicated size.
 * This might require that some data is discarded, in which
 * case arbitrary data blocks might be deleted.
 * Returns FALSE in case of error.
 */
BOOL
Storage_ChangeSize (StorageObject *stobj, UINT32 newSize)
{
  ListNode *p, *tmp;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return FALSE;
  }

  if (newSize < MINBLOCKSIZE + 4)
    return FALSE;

  /* Truncate to nearest multiple of 8 */
  newSize = ((newSize + 4) & ~7) - 4;

  /* Free all nodes in the block list. */
  p = stobj->blocklist;
  while (p) {
    tmp = p;
    p = p->next;
    DEALLOC (&tmp);
  }
  stobj->blocklist = NULL;
  stobj->size = newSize;
  stobj->bytesUsed = 0;

  if (!ScanStorage (stobj, newSize)) {
    return FALSE;
  }

  return TRUE;
}

#define BUFSIZE 64

/*
 * Copy data from one location in the store to another.
 * This routine does not handle overlapping data areas!
 */
static VOID
CopyData (StorageObject *stobj, UINT32 dstpos, UINT32 srcpos, UINT32 len)
{
  BYTE   buf[BUFSIZE];
  UINT32 n;

  while (len > 0) {
    n = MIN (BUFSIZE, len);
    n = stobj->readStorage (srcpos, n, buf);
    stobj->writeStorage (dstpos, n, buf);
    srcpos += n;
    dstpos += n;
    len -= n;
  }
}

/*
 * Compact the storage area, i.e., try to form larger free blocks.
 * This routine does not necessarily complete the compaction process,
 * rather it tries to make progress towards a complete compaction.
 * Returns TRUE if progress was made, FALSE otherwise.
 * As long as the function returns TRUE, it may be beneficial to
 * call the function again.
 */
BOOL
Storage_Compact (StorageObject *stobj)
{
  ListNode *p, *pn, *pnn;
  UINT32   psize, pnsize, pnnsize;
  UINT32   pnid;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return FALSE;
  }

  /* Scan block list until we find an empty block */
  p = IdToBlock (stobj, 0);
  if ((p == NULL) || (p->next == NULL))
    return FALSE;  /* Nothing further to be done */

  pn = p->next;
  if (pn->id == 0) {
    /* We have two adjacent blocks that are free, they must be joined. */
    /* This should never happen, but we take care of it just in case. */
    p->size += pn->size;
    PUTHEADER (stobj, p);
    p->next = pn->next;
    DEALLOC (&pn);
    return TRUE;
  }

  psize = p->size;
  pnsize = pn->size;
  pnid = pn->id;

  /* If the block following p->next is free, it has to be joined: */
  pnn = pn->next;
  pnnsize = 0;
  if ((pnn != NULL) && (pnn->id == 0))
    pnnsize = pnn->size;

  p->size += pnsize + pnnsize;
  /* For now, we mark the area of the two (or three) blocks as one big
   * empty block. The write operations to the store are carried out
   * in an order that will guarantee that the contents are always
   * consistent. */
  PUTHEADER(stobj, p);

  /* Move data part of allocated block */
  CopyData (stobj, p->pos + sizeof (BlockHeader),
            pn->pos + sizeof (BlockHeader),
            pnsize - sizeof (BlockHeader));

  /* Write header of the new empty block */
  pn->size = psize + pnnsize;
  pn->pos = p->pos + pnsize;
  pn->id = 0;
  PUTHEADER (stobj, pn);

  /* Write header of the new allocated block */
  p->size = pnsize;
  p->id = pnid;
  PUTHEADER (stobj, p);

  /* Remove the list node, in case we joined two free blocks */
  if (pnnsize > 0) {
    pn->next = pnn->next;
    DEALLOC (&pnn);
  }

  return TRUE;
}

/*
 * Allocate a new block of a size that will hold "len" bytes of data.
 * "id" must be different from NULL. If *id != 0, then that number
 * is used as the Id for the new block, unless it is alreaday in use,
 * in which case FALSE is returned. If *id == 0, then a new unique
 * id is allocated, larger than any other currently in use.
 * Returns FALSE in case of error, e.g., if there is not enough
 * memory left to add a block of the indicated size.
 */
BOOL
Storage_AllocateBlock (StorageObject *stobj, UINT32 len, UINT32 *id)
{
  ListNode *p, *tmp, *pnew;
  UINT32   newid;
  UINT32   sz;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return FALSE;
  }

  /* Compute the size we will search for: add space for block header
   * and then round upward to nearest multiple of 8. */
  sz = len + sizeof (BlockHeader);
  sz = (sz + 7) & ~7;

  /* Search the block list for a block of size at least sz.
   * The search begins at the beginning of the block list. */
  for (p = stobj->blocklist; p; p = p->next) {
    if ((p->id == 0) && (p->size >= sz)) {
      break;
    }
  }
  /* If there was no block of sufficient size, return FALSE. */
  if (p == NULL) {
    return FALSE;
  }

  /* Get new ID */
  if (id == NULL)
    return FALSE;
  if (*id == 0) {
    /* Create a new unique ID */
    newid = AllocNewId (stobj);
    *id = newid;
  }
  else {
    /* Use the ID suggested by the caller, unless it is already in use. */
    newid = *id;
    for (tmp = stobj->blocklist; tmp; tmp = tmp->next)
      if (tmp->id == newid)
        return FALSE;
  }

  if (p->size >= sz + MINBLOCKSIZE) {
    /* If block is large enough, it can be split:
     *   write second header (size, id = 0)
     *   write first header (size, id = ID) */
    pnew = NEWARRAY (ListNode, 1);
    if (pnew == NULL)
      return FALSE;
    pnew->pos = p->pos + sz;
    pnew->size = p->size - sz;
    pnew->id = 0;
    PUTHEADER (stobj, pnew);

    p->size = sz;
    p->id = newid;
    PUTHEADER (stobj, p);

    pnew->next = p->next;
    p->next = pnew;
  }
  else /* No split, use the entire block. */ {
    /*   write header (size, id = ID) */
    p->id = newid;
    PUTHEADER (stobj, p);
  }
  stobj->bytesUsed += p->size;

  return TRUE;
}

/*
 * Fetch the contents of a block, or part of a block.
 * The retrieved data is copied to the location indicated by "recData".
 * Returns FALSE in case of error, e.g., if the specified block
 * does not exist.
 */
BOOL
Storage_Get (StorageObject *stobj, UINT32 id,
             UINT32 start, UINT32 len, VOID *recData)
{
  ListNode *p;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return FALSE;
  }

  p = IdToBlock (stobj, id);
  if (p == NULL)
    return FALSE;

  if (start + len + sizeof (BlockHeader) > p->size)
    return FALSE;

  stobj->readStorage (p->pos + sizeof (BlockHeader) + start,
                      len, recData);

  return TRUE;
}

/*
 * Write the contents of an existing block, or part of a block.
 * The data written to the block is read from the location
 * indicated by "recData".
 * Returns FALSE in case of error, e.g., if the specified block
 * does not exist, or if the indicated start position and data length
 * adds up to a position beyond the current end of the block.
 */
BOOL
Storage_Put (StorageObject *stobj, UINT32 id,
             UINT32 start, UINT32 len, VOID *recData)
{
  ListNode *p;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return FALSE;
  }

  p = IdToBlock (stobj, id);
  if (p == NULL)
    return FALSE;

  if (start + len + sizeof (BlockHeader) > p->size)
    return FALSE;

  stobj->writeStorage (p->pos + sizeof (BlockHeader) + start,
                       len, recData);

  return TRUE;
}

/*
 * Delete the block with given block ID.
 * Returns FALSE in case of error, e.g., if the indicated block
 * does not exist.
 */
BOOL
Storage_DeleteBlock (StorageObject *stobj, UINT32 id)
{
  ListNode *p, *pred, *succ;

  if ((stobj == NULL) || (stobj->blocklist == NULL) || (id == 0)) {
    return FALSE;
  }

  for (pred = NULL, p = stobj->blocklist; p; pred = p, p = p->next) {
    if (p->id == id)
      break;
  }

  if (p == NULL)
    return FALSE;
  succ = p->next;
  stobj->bytesUsed -= p->size;

  /* Set to 'free' */
  p->id = 0;
  PUTHEADER (stobj, p);

  /* Merge with previous record? */
  if (pred && (pred->id == 0)) {
    pred->size += p->size;
    PUTHEADER (stobj, pred);
    pred->next = p->next;
    DEALLOC (&p);
    p = pred;
  }

  /* Merge with next record? */
  if (succ && (succ->id == 0)) {
    p->size += succ->size;
    PUTHEADER (stobj, p);
    p->next = succ->next;
    DEALLOC (&succ);
  }

  return TRUE;
}


/*
 * Return the size of the indicated block.
 * Returns 0 in case of error.
 */
UINT32
Storage_GetBlockSize (StorageObject *stobj, UINT32 id)
{
  ListNode *p;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return 0;
  }

  p = IdToBlock (stobj, id);
  if (p == NULL)
    return 0;

  return p->size - sizeof (BlockHeader);
}


/*
 * Get the IDs of all blocks in the storage area.
 * Sets "*idarr" to point to an array of UINT32 values,
 * and "*len" to the number of elements in the array.
 * Returns FALSE in case of error.
 * NOTE: it is the caller's responsibility to deallocate the array.
 */
BOOL
Storage_GetAllBlockIds (StorageObject *stobj,
                        UINT32 **idarr, UINT16 *len)
{
  UINT16   i = 0;
  ListNode *p;

  if ((stobj == NULL) || (stobj->blocklist == NULL) ||
      (idarr == NULL) || (len == NULL))
    return FALSE;

  *idarr = NULL;
  *len = 0;

  /* Count the number of allocated blocks */
  for (p = stobj->blocklist; p; p = p->next) {
    if (p->id != 0)
      i++;
  }
  if (i == 0) {
    return TRUE;
  }

  *idarr = NEWARRAY (UINT32, i);
  if (*idarr == NULL)
    return FALSE;
  *len = i;

  i = 0;
  for (p = stobj->blocklist; p; p = p->next) {
    if (p->id != 0)
      (*idarr)[i++] = p->id;
  }

  return TRUE;
}

/*
 * Return the size of the largest block that could be allocated
 * if the storage area were completely compacted.
 * Returns 0 in case of error.
 */
UINT32
Storage_NumBytesFree (StorageObject *stobj)
{
  INT32 nf;

  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return 0;
  }
  nf = stobj->size - stobj->bytesUsed - 4 - sizeof (BlockHeader);
  if (nf < 0)
    return 0;

  return (UINT32)(nf & ~0x7);
}


/*
 * Return the number of bytes currently used in the allocated blocks.
 */
UINT32
Storage_BytesUsed (StorageObject *stobj)
{
  if ((stobj == NULL) || (stobj->blocklist == NULL)) {
    return 0;
  }
  return stobj->bytesUsed + 4;
}
#endif
