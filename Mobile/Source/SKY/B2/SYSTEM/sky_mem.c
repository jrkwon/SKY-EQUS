/*
** ===========================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_mem.c
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2001-04-01 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdlib.h> /* malloc and free */
#include <string.h> /* malloc and free */
#include "sky.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

#if defined LOG_INTERNAL && !defined DEBUG_WIP_MALLOC
#define DEBUG_WIP_MALLOC
#endif


#ifdef DEBUG_WIP_MALLOC
/* Print all currently allocated chunks. */
void sWip_printalloc (FILE *logfile);

/* Prints a list of all the current chunks. */
void sWip_printchunks (FILE *logfile);

/* Prints a sequence of numbers representing the sizes of
 * the chunks on the free list. */
void sWip_printfreelist (FILE *logfile);

/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */
void sWip_printbriefstats (FILE *logfile);

/* Prints accumulated statistics. */
void sWip_printstats (FILE *logfile);

#endif



#define SIZE_T                 UINT16
#define SIZE_T_SZ              2
#define CHUNKHEADERSIZE        4
#define MINCHUNKSIZE           8
#define OFFSET_T               UINT16
#define MALLOC_ALIGNMENT       4
#define MALLOC_ALIGN_MASK      3
#define MINBLOCKSIZE           24

LOCAL void sWip_debug(void);

typedef struct {
  SIZE_T   prev_size; /* Size of previous chunk.            */
  SIZE_T   size;      /* Size in bytes, including overhead. */
  OFFSET_T fwd;       /* Double links -- used only if free. */
  OFFSET_T bck;
} chunk;
typedef chunk* chunkptr;


/*
 * Global variables.
 */
static BYTE     *baseptr = 0;
static chunkptr firstchunk = 0;
static chunkptr lastchunk = 0;

/*
 * We use segregated free lists, with separate lists for
 * chunks of different sizes. Currently, we use sizes that are
 * powers of two.
 * In list number n is kept all the free chunks whose size is
 * strictly less than maxsizes[n].
 */
#define NUM_FREE_LISTS 10
static chunkptr freelists[NUM_FREE_LISTS];
static UINT32   maxsizes[NUM_FREE_LISTS] = {
  16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 0x10000
};

/*
 * Return the number of the list that a chunk of size "n"
 * belongs to.
 */
static INT16
sList_idx (UINT32 n)
{
  INT16 l;

  for (l = 0; maxsizes[l] <= n; l++);

  return l;
}

/*
 * Macros, rather than function calls, are used for maximum performance:
 */

/* Conversion from chunk headers to user pointers, and back */
#define chunk2mem(p)    ((void*)((BYTE *)(p) + CHUNKHEADERSIZE))
#define mem2chunk(mem)  ((chunkptr)((BYTE *)(mem) - CHUNKHEADERSIZE))

/* Reading and writing of the size fields.
 * Since all chunks have sizes that are a multiple of 4 bytes we
 * do not need to store the last two bits. However, the rightmost
 * bit is used for marking whether the chunk is free or not.
 * Hence, the maximum chunk size is 128k. */
#define chunksize(p)        (((p)->size & ~0x01) << 1)
#define prevsize(p)         (((p)->prev_size & ~0x01) << 1)
#define set_size(p, sz)     \
  (((chunkptr)(p))->size = (SIZE_T)((((sz) >> 1) & ~0x01) | \
                                     (((chunkptr)(p))->size & 0x01)))
#define set_prevsize(p, sz) \
  (((chunkptr)(p))->prev_size = (SIZE_T)((((sz) >> 1) & ~0x01) |\
                     (((chunkptr)(p))->prev_size & 0x01)))

#define set_hd1(p, v)   ((p)->prev_size = (SIZE_T)(v))
#define set_hd2(p, v)   ((p)->size = (SIZE_T)(v))

/* The next and previous links */
#define prevchunk(p)    ((chunkptr)(((BYTE *)(p)) - prevsize (p)))
#define nextchunk(p)    ((chunkptr)(((BYTE *)(p)) + chunksize (p)))

/* The IN USE bit */
#define chunk_isfree(p) ((((chunkptr)(p))->size & 0x01) == 0)
#define chunk_inuse(p) ((((chunkptr)(p))->size & 0x01) == 1)
#define set_inuse(p)    (((chunkptr)(p))->size |= 0x01)
#define clear_inuse(p)  (((chunkptr)(p))->size &= ~0x01)

/* The GC bit */
#define chunk_has_gcbit(p) ((((chunkptr)(p))->prev_size & 0x01) == 1)
#define set_gcbit(p)       (((chunkptr)(p))->prev_size |= 0x01)
#define clear_gcbit(p)     (((chunkptr)(p))->prev_size &= ~0x01)

/* The forward and back links.
 * Instead of real pointers we store offsets, and since we are
 * always aligned on 4-byte boundaries, we do not store the last
 * two bits. In this way, we can address 256k of memory located
 * anywhere in the application's memory space. */
#define offset2ptr(o)   ((chunkptr)(baseptr + ((o) << 2)))
#define ptr2offset(p)   ((OFFSET_T)((((BYTE *)(p)) - baseptr) >> 2))

#define forward(p)      (offset2ptr (((chunkptr)(p))->fwd))
#define back(p)         (offset2ptr (((chunkptr)(p))->bck))

#define set_fwd(p, q)   (((chunkptr)(p))->fwd = ptr2offset (q))
#define set_bck(p, q)   (((chunkptr)(p))->bck = ptr2offset (q))

/*
 * List operations.
 */

#define remove_from_list(p)  do { \
  chunkptr fwd = forward (p); \
  chunkptr bck = back (p); \
  fwd->bck = (p)->bck; \
  bck->fwd = (p)->fwd; \
} while (0)

#define add_to_list(l, p) do { \
  (p)->fwd = (l)->fwd; \
  set_bck (p, l); \
  set_bck (forward (l), p); \
  set_fwd (l, p); \
} while (0)

#define append_to_list(l, p) do { \
  set_fwd (p, l); \
  set_bck (p, back (l)); \
  set_fwd (back (l), p); \
  set_bck (l, p); \
} while (0)

/*
 * Compute the chunk size we will need for an allocation request:
 */
#define request2size(req) \
 (((UINT32)((req) + CHUNKHEADERSIZE + MALLOC_ALIGN_MASK) < \
  (UINT32)(MINCHUNKSIZE + MALLOC_ALIGN_MASK)) ? MINCHUNKSIZE : \
  (UINT32)(((req) + CHUNKHEADERSIZE + MALLOC_ALIGN_MASK) & ~(MALLOC_ALIGN_MASK)))



#ifdef DEBUG_WIP_MALLOC
static UINT32 bytes_allocated = 0;
static UINT32 bytes_requested = 0;
static UINT32 currently_allocated = 0;
static UINT32 max_allocated = 0;
static INT32  last_alloc = 0;
static INT32  last_req = 0;
static UINT32 num_alloc = 0;
static UINT32 num_free = 0;
static UINT32 traverse_count = 0;
static UINT32 maxfreelistlength = 0;
#endif




/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/

LOCAL INT           l_nAllocCount = 0;
LOCAL void*         l_pBaseMemory;
LOCAL INT           l_nMemorySize;


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/*
 * Initialize the WIP memory allocation library.
 * The first parameter is a pointer to a block of memory,
 * and the second is the size of the memory area.
 * Returns 0 on success, and -1 otherwise.
 */
LOCAL INT16 sWip_initmalloc (void *mem, UINT32 memsize)
{
  chunkptr p;
  UINT32   size;
  INT16    i;

  /* Truncate to nearest multiple of the alignment. */
  memsize &= ~MALLOC_ALIGN_MASK;
  if ((mem == NULL) || (memsize < MINBLOCKSIZE) || (memsize > (1 << 17))) {
    return -1;
  }
  baseptr = (BYTE *)mem;

  /* For each of the lists of free chunks, we need one chunk to
   * serve as list header. We mark each one as "in use" to avoid
   * having it merged with an adjacent chunk on freeing. */
  for (i = 0; i < NUM_FREE_LISTS; i++) {
    p = (chunkptr)(baseptr + i * MINCHUNKSIZE);
    set_hd1 (p, 0);
    set_hd2 (p, (MINCHUNKSIZE >> 1) | 0x01);
    p->fwd = ptr2offset (p);
    p->bck = ptr2offset (p);
    freelists[i] = p;
  }

  /* This chunk starts out on the appropriate free list and
   * contains all of the available memory. */
  firstchunk = (chunkptr)(baseptr + NUM_FREE_LISTS * MINCHUNKSIZE);
  set_hd1 (firstchunk, MINCHUNKSIZE >> 1);
  size = memsize - ((NUM_FREE_LISTS + 1) * MINCHUNKSIZE);
  set_hd2 (firstchunk, size >> 1);
  add_to_list (freelists[sList_idx (size)], firstchunk);

  /* The last chunk is never placed on any list; it is marked "in use",
   * and simply acts as a sentry element, when scanning all the chunks. */
  lastchunk = (chunkptr)(((BYTE *)baseptr) + memsize - MINCHUNKSIZE);
  set_hd1 (lastchunk, size >> 1);
  set_hd2 (lastchunk, (MINCHUNKSIZE >> 1) | 0x01);

  return 0;
}

/*
 * sWip_malloc and sWip_gcmalloc are almost identical, so it would make
 * sense to have one local function doing most of the job that
 * the other two functions called. However, for efficiency reasons
 * we maintain two copies...
 */

/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Returns 0 if allocation fails.
 */
LOCAL void * sWip_sky_malloc (UINT32 size)
{
  chunkptr p = 0, ptmp;
  UINT32   nb;
  UINT32   sz = 250000;
  UINT32   remsize;
  INT16    i;
#ifdef DEBUG_WIP_MALLOC
  UINT32   tmp_traverse_count  = 0;
#endif

  /* We add space for our overhead (4 bytes) plus round to nearest
   * larger multiple of 4, plus never allocate a chunk less than 8 bytes. */
  nb = request2size (size);

  /* Check all relevant free lists, until a non-empty one is found. */
  for (i = sList_idx (nb); i < NUM_FREE_LISTS; i++) {
    chunkptr freelist = freelists[i];

    /* Search the entire list, select chunk with closest fit */
    for (ptmp = forward (freelist); ptmp != freelist;
         ptmp = forward (ptmp)) {
      UINT32 tmpsz = chunksize (ptmp);

#ifdef DEBUG_WIP_MALLOC
      tmp_traverse_count++;
#endif
      if (tmpsz == nb) { /* Exact fit: no need to search any further. */
        p = ptmp;
        sz = tmpsz;
        goto found;
      }
      else if (tmpsz > nb) { /* Chunk is large enough */
        if (tmpsz < sz) {
          /* This is the best so far. */
          p = ptmp;
          sz = tmpsz;
        }
      }
    }
    if (p != 0) {
      goto found;
    }
  }
  /* Searched all lists, but found no large enough chunk */
  return 0;

 found:
  /* We have found a large enough chunk, namely "p" of size "sz". */
  remove_from_list (p);
  remsize = sz - nb;

  if (remsize >= MINCHUNKSIZE) {
    /* The remainder is large enough to become a separate chunk */
    chunkptr q, next;
    chunkptr l;

    sz = nb;
    /* "q" will be the new chunk */
    q = (chunkptr)((BYTE *)p + sz);
    set_hd2 (q, remsize >> 1);
    set_hd1 (q, nb >> 1);
    next = nextchunk (q);
    set_prevsize (next, remsize);

    l = freelists[sList_idx (remsize)];
    add_to_list (l, q);
  }
  set_hd2 (p, sz >> 1 | 0x01);
  clear_gcbit (p);

#ifdef DEBUG_WIP_MALLOC
  last_alloc = sz;
  bytes_allocated += last_alloc;
  last_req = size;
  bytes_requested += last_req;
  currently_allocated += last_alloc;
  if (currently_allocated > max_allocated)
    max_allocated = currently_allocated;
  num_alloc++;
  traverse_count += tmp_traverse_count;
  maxfreelistlength = MAX (maxfreelistlength, tmp_traverse_count);
#endif
  return chunk2mem (p);
}

/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Memory blocks allocated with this routine can be freed in one
 * batch operation by calling sWip_gc.
 * Returns 0 if allocation fails.
 */
LOCAL void * sWip_gcmalloc (UINT32 size)
{
  chunkptr p = 0, ptmp;
  UINT32   nb;
  UINT32   sz = 250000;
  UINT32   remsize;
  INT16    i;
#ifdef DEBUG_WIP_MALLOC
  UINT32   tmp_traverse_count  = 0;
#endif

  /* We add space for our overhead (4 bytes) plus round to nearest
   * larger multiple of 4, plus never allocate a chunk less than 8 bytes. */
  nb = request2size (size);

  /* Check all relevant free lists, until a non-empty one is found. */
  for (i = sList_idx (nb); i < NUM_FREE_LISTS; i++) {
    chunkptr freelist = freelists[i];

    /* Search the entire list, select chunk with closest fit */
    for (ptmp = forward (freelist); ptmp != freelist;
         ptmp = forward (ptmp)) {
      UINT32 tmpsz = chunksize (ptmp);

#ifdef DEBUG_WIP_MALLOC
      tmp_traverse_count++;
#endif
      if (tmpsz == nb) { /* Exact fit: no need to search any further. */
        p = ptmp;
        sz = tmpsz;
        goto found;
      }
      else if (tmpsz > nb) { /* Chunk is large enough */
        if (tmpsz < sz) {
          /* This is the best so far. */
          p = ptmp;
          sz = tmpsz;
        }
      }
    }
    if (p != 0) {
      goto found;
    }
  }
  /* Searched all lists, but found no large enough chunk */
  return 0;

 found:
  /* We have found a large enough chunk, namely "p" of size "sz". */
  remove_from_list (p);
  remsize = sz - nb;

  if (remsize >= MINCHUNKSIZE) {
    /* The remainder is large enough to become a separate chunk */
    chunkptr q, next;
    chunkptr l;

    sz = nb;
    /* "q" will be the new chunk */
    q = (chunkptr)((BYTE *)p + sz);
    set_hd2 (q, remsize >> 1);
    set_hd1 (q, nb >> 1);
    next = nextchunk (q);
    set_prevsize (next, remsize);

    l = freelists[sList_idx (remsize)];
    add_to_list (l, q);
  }
  set_hd2 (p, sz >> 1 | 0x01);
  set_gcbit (p);

#ifdef DEBUG_WIP_MALLOC
  last_alloc = sz;
  bytes_allocated += last_alloc;
  last_req = size;
  bytes_requested += last_req;
  currently_allocated += last_alloc;
  if (currently_allocated > max_allocated)
    max_allocated = currently_allocated;
  num_alloc++;
  traverse_count += tmp_traverse_count;
  maxfreelistlength = MAX (maxfreelistlength, tmp_traverse_count);
#endif
  return chunk2mem (p);
}

/*
 * Free a memory area previously allocated with sWip_malloc or sWip_gcmalloc.
 * Calling this routine with 'mem' equal to 0 has no effect.
 */
LOCAL void sWip_sky_free (void* mem)
{
  chunkptr p;         /* chunk corresponding to mem */
  UINT32   sz;        /* its size */
  chunkptr next;      /* next adjacent chunk */
  chunkptr prev;      /* previous adjacent chunk */
  chunkptr l;

  if (mem == 0) {
    return;
  }

  p = mem2chunk (mem);
  sz = chunksize (p);
  prev = prevchunk (p);
  next = nextchunk (p);
#ifdef DEBUG_WIP_MALLOC
  last_alloc = -(INT32)sz;
  last_req = last_alloc;
  currently_allocated -= sz;
  num_free++;
#endif
  
  if (chunk_isfree (prev)) {            /* Join with previous chunk */
    sz += chunksize (prev);
    p = prev;
    remove_from_list (prev);
  }
  if (chunk_isfree (next)) {            /* Join with next chunk */
    sz += chunksize (next);
    remove_from_list (next);
  }
  set_hd2 (p, sz >> 1);
  clear_gcbit (p);
  next = nextchunk (p);
  set_prevsize (next, sz);

  l = freelists [sList_idx (sz)];
  add_to_list (l, p);
}


/*
 * Free all memory areas allocated with sWip_gcmalloc.
 * Memory blocks allocated with sWip_malloc are left undisturbed.
 */
LOCAL void sWip_gc (void)
{
  chunkptr p;
  chunkptr l;

  for (p = firstchunk; p != lastchunk; p = nextchunk (p)) {
    if (chunk_inuse (p) && chunk_has_gcbit (p)) {
      UINT32    sz;        /* its size */
      chunkptr  next;      /* next contiguous chunk */
      chunkptr  prev;      /* previous contiguous chunk */

      sz = chunksize (p);
      prev = prevchunk (p);
      next = nextchunk (p);
#ifdef DEBUG_WIP_MALLOC
      last_alloc = -(INT32)sz;
      last_req = last_alloc;
      currently_allocated -= sz;
      num_free++;
#endif
  
      if (chunk_isfree (prev)) {            /* Join with previous chunk */
        sz += chunksize (prev);
        remove_from_list (prev);
        p = prev;
      }
      if (chunk_isfree (next)) {            /* Join with next chunk */
        sz += chunksize (next);
        remove_from_list (next);
      }
      set_hd2 (p, sz >> 1);
      clear_gcbit (p);
      next = nextchunk (p);
      set_prevsize (next, sz);

      l = freelists [sList_idx (sz)];
      add_to_list (l, p);
    }
  }
}


/*
 * Return the size of an allocated memory area.
 * The reported size includes the overhead used by the memory allocation
 * system.
 */
LOCAL UINT32 sWip_memsize (void *mem)
{
  chunkptr p;         /* chunk corresponding to mem */

  p = mem2chunk (mem);
  return chunksize (p);
}


/*
 * Routines for debugging and statistics.
 */
#ifdef DEBUG_WIP_MALLOC

/*
 * Print all currently allocated chunks.
 */
LOCAL void sWip_printalloc (FILE *logfile)
{
  chunkptr p;

  for (p = firstchunk; p != lastchunk; p = nextchunk (p)) {
    if (!chunk_isfree (p)) {
      fprintf (logfile, "id: %d\tsize: %d\n", 0, chunksize (p));
    }
  }
}

/*
 * Prints a list of all the current chunks.
 */
LOCAL void sWip_printchunks (FILE *logfile)
{
  chunkptr p = firstchunk;

  fprintf (logfile, "\n***************\n");
  for (;;) {
    fprintf (logfile, "-------------------\n");
    fprintf (logfile, "address: 0x%x\n", p);
    fprintf (logfile, "prev size: %d\n", prevsize (p));
    fprintf (logfile, "size: %d\n", chunksize (p));
    fprintf (logfile, "gc: %d, inuse: %d\n", chunk_has_gcbit (p),
             chunk_inuse (p));
    if (chunk_isfree (p) || (p == (chunkptr)baseptr)) {
      fprintf (logfile, "fwd: 0x%x\n", forward (p));
      fprintf (logfile, "bck: 0x%x\n", back (p));
    }
    if (p == lastchunk) {
      break;
    }
    p = nextchunk (p);
  }
  fprintf (logfile, "***************\n\n");
}

/*
 * Prints a sequence of numbers representing the sizes of
 * the chunks on the free list.
 */
LOCAL void sWip_printfreelist (FILE *logfile)
{
  chunkptr p;
  UINT32   sz;
  INT16    i;

  fprintf (logfile, "\t");
  for (i = 0; i < NUM_FREE_LISTS; i++) {
    for (p = forward (freelists[i]); p != freelists[i]; p = forward (p)) {
      sz = chunksize (p);
      fprintf (logfile, " %d", sz);
    }
  }
}

/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */
LOCAL void sWip_printbriefstats (FILE *logfile)
{
  INT16    i;
  chunkptr p;
  UINT32   listcount = 0;
  UINT32   chunkcount = 0;

  for (i = 0; i < NUM_FREE_LISTS; i++) {
    for (p = forward (freelists[i]); p != freelists[i]; p = forward (p)) {
      listcount++;
    }
  }
  for (p = firstchunk; p != lastchunk; p = nextchunk (p)) {
    chunkcount++;
  }
  if (last_req < 0) {
    last_req = 0;
  }

  fprintf (logfile, "%6d%6d%6d%6d%6d%6d", last_req, last_alloc,
           currently_allocated, traverse_count, listcount, chunkcount);
}

/*
 * Prints accumulated statistics.
 */
LOCAL void sWip_printstats (FILE *logfile)
{
  fprintf (logfile, "Bytes requested: %d\n", bytes_requested);
  fprintf (logfile, "Bytes allocated: %d\n", bytes_allocated);
  fprintf (logfile, "Bytes currently allocated: %d\n", currently_allocated);
  fprintf (logfile, "Max bytes allocated: %d\n", max_allocated);
  fprintf (logfile, "Number of allocations: %d\n", num_alloc);
  fprintf (logfile, "Number of free: %d\n", num_free);
  fprintf (logfile, "Checks in freelist: %d\n", traverse_count);
  fprintf (logfile, "Max length of freelist: %d\n", maxfreelistlength);
}


LOCAL void sWip_debug(void)
{
  extern void    CLNTa_log (unsigned char viewId, signed short logNo, const char *format, ...);

  CLNTa_log(1,0, "Bytes requested: %d\n", bytes_requested);
  CLNTa_log(1,0, "Bytes allocated: %d\n", bytes_allocated);
  CLNTa_log(1,0, "Bytes currently allocated: %d\n", currently_allocated);
  CLNTa_log(1,0, "Max bytes allocated: %d\n", max_allocated);
  CLNTa_log(1,0, "Number of allocations: %d\n", num_alloc);
  CLNTa_log(1,0, "Number of free: %d\n", num_free);
  CLNTa_log(1,0, "Checks in freelist: %d\n", traverse_count);
  CLNTa_log(1,0, "Max length of freelist: %d\n", maxfreelistlength);
}

LOCAL INT16 sWip_getfragmentation ()
{
  chunkptr p;
  UINT32   largestsize = 0;
  UINT32   totalfreesize = 0;
  UINT32   sz;

  for (p = firstchunk; p != lastchunk; p = nextchunk (p)) {
    if (chunk_isfree (p)) {
      sz = chunksize (p);
      totalfreesize += sz;
      largestsize = MAX (largestsize, sz);
    }
  }

  if (totalfreesize == 0)
    return 100;

  return (INT16)((largestsize * 100) / totalfreesize);
}

LOCAL void sWip_fragmentationstats (FILE *logfile)
{
  chunkptr p;
  UINT32   largestsize = 0;
  UINT32   totalfreesize = 0;
  UINT32   sz;

  for (p = firstchunk; p != lastchunk; p = nextchunk (p)) {
    if (chunk_isfree (p)) {
      sz = chunksize (p);
      totalfreesize += sz;
      largestsize = MAX (largestsize, sz);
    }
  }

  fprintf (logfile, "Total free size: %d\nLargest chunk: %d (%d%%)\n",
           totalfreesize, largestsize, (largestsize * 100) / totalfreesize);
}

#endif

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/


WORD SKY_API SKY_InitMemory(void *pMemBlock, UINT32 nMemSize)
{		
#if defined (WAPPER) || defined (FEATURE_WAP)
	if( SKY_IS_IN_WAP() == FALSE )
	{
		l_pBaseMemory = pMemBlock;
		l_nMemorySize = nMemSize;
		l_nAllocCount = 0;
	}
	else
		return 0;
#else
	l_pBaseMemory = pMemBlock;
	l_nMemorySize = nMemSize;
	l_nAllocCount = 0;
#endif //#if defined (WAPPER) || defined (FEATURE_WAP)

	memset(pMemBlock,0,nMemSize);
	return sWip_initmalloc(pMemBlock, nMemSize);
}


void* SKY_API SKY_Malloc(SIZE_T stSize)
{

    void* pMemBlock;

	pMemBlock = sWip_sky_malloc(stSize);    

    SKY_ASSERT(pMemBlock != NULL);
#if defined (WAPPER) || defined (FEATURE_WAP)
	if( SKY_IS_IN_WAP() )
		return pMemBlock;
	else
#endif //#if defined (WAPPER) || defined (FEATURE_WAP)
	{
		if( (BYTE*)pMemBlock >= (BYTE*)l_pBaseMemory &&\
			(BYTE*)pMemBlock < (BYTE*)l_pBaseMemory + l_nMemorySize )
		{
			l_nAllocCount++;
		}
		else
		{
			//SKY_ASSERT( l_nAllocCount == 0 );
			sWip_initmalloc( l_pBaseMemory, l_nMemorySize);				
			pMemBlock = sWip_sky_malloc(stSize);
			SKY_ASSERT(pMemBlock != NULL);
			l_nAllocCount = 1;
		}
	}
    return pMemBlock;

}

void* SKY_API SKY_Calloc(SIZE_T stElement, SIZE_T stSize)
{
    void* pMemBlock;

	pMemBlock = SKY_Malloc(stSize*stElement);
	memset(pMemBlock, 0, stElement*stSize);
    
    SKY_ASSERT(pMemBlock != NULL);

    return pMemBlock;
}

void SKY_API SKY_Free(void* pMemBlock)
{
    /*
    ** trying to free null pointer should've generated assertion error.
    ** but, ....
    */    
    if(pMemBlock == NULL)
        return;
#if defined (WAPPER) || defined (FEATURE_WAP)
	if( SKY_IS_IN_WAP() == FALSE )
#endif //#if defined (WAPPER) || defined (FEATURE_WAP)
	{
		SKY_ASSERT( l_nAllocCount > 0 );
		SKY_ASSERT( (BYTE*)pMemBlock >= (BYTE*)l_pBaseMemory && (BYTE*)pMemBlock < (BYTE*)l_pBaseMemory + l_nMemorySize );
		l_nAllocCount--;
	}

	sWip_sky_free(pMemBlock);
}

#ifdef FEATURE_EQS_DEBUG_MEM
INT SKY_API SKY_GetMemAllocCount(void)
{
    return l_nAllocCount;
}
#endif

