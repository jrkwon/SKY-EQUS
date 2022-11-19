/*
 * Copyright (C) 1999, AU-System Radio AB. All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System Radio AB, and may be used and copied
 * only in accordance with the terms of this agreement.
 *
 * AU-System Radio AB assumes no responsibility
 * or liability for any errors or inaccuracies in this software,
 * or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 *
 * wip_mem.c
 *
 * Created by Anders Edenbrandt, Tue Jul 13 15:58:45 1999.
 *
 *
 * Memory allocation routines, tailored for WIP on platforms
 * with scarce RAM resources.
 *
 * Revision history:
 *
 *   990713: Created, AED
 *   990721: Removed include of standard ANSI C files
 *   990817, AED: Added a diagnostic routine wip_printalloc, and a function
 *                to return the size of a memory area, wip_memsize.
 *
 */
//#define TRGT_REX
//#include "au_cmmn.h" // <-- tapicmmn.h : girin. 2000.12.20
#include "au_mem.h" // <-- wip_mem.h : girin. 2000.12.20


#ifdef DEBUG_WIP_MALLOC
extern void CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);
#endif//#ifdef DEBUG_WIP_MALLOC

//#ifdef USE_WIP_MALLOC

#define SIZE_T                 UINT16
#define SIZE_T_SZ              2
#define CHUNKHEADERSIZE        4
#define MINCHUNKSIZE           8
#define OFFSET_T               UINT16
#define MALLOC_ALIGNMENT       4
#define MALLOC_ALIGN_MASK      3
#define MINBLOCKSIZE           24

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
static chunkptr freelist = 0;
static chunkptr lastchunk = 0;

 
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

#define set_hd1(p, v)   ((p)->prev_size = (SIZE_T)v)
#define set_hd2(p, v)   ((p)->size = (SIZE_T)v)

/* The next and previous links */
#define prevchunk(p)    ((chunkptr)(((BYTE *)(p)) - prevsize (p)))
#define nextchunk(p)    ((chunkptr)(((BYTE *)(p)) + chunksize (p)))

/* The IN USE bit */
#define chunk_isfree(p) ((((chunkptr)p)->size & 0x01) == 0)
#define chunk_inuse(p) ((((chunkptr)p)->size & 0x01) == 1)
#define set_inuse(p)    (((chunkptr)p)->size |= 0x01)
#define clear_inuse(p)  (((chunkptr)p)->size &= ~0x01)

/* The GC bit */
#define chunk_has_gcbit(p) ((((chunkptr)p)->prev_size & 0x01) == 1)
#define set_gcbit(p)       (((chunkptr)p)->prev_size |= 0x01)
#define clear_gcbit(p)     (((chunkptr)p)->prev_size &= ~0x01)

/* The forward and back links.
 * Instead of real pointers we store offsets, and since we are
 * always aligned on 4-byte boundaries, we do not store the last
 * two bits. In this way, we can address 256k of memory located
 * anywhere in the application's memory space. */
#define offset2ptr(o)   ((chunkptr)(baseptr + (o << 2)))
#define ptr2offset(p)   ((OFFSET_T)((((BYTE *)p) - baseptr) >> 2))

#define forward(p)      (offset2ptr (((chunkptr)p)->fwd))
#define back(p)         (offset2ptr (((chunkptr)p)->bck))

#define set_fwd(p, q)   (((chunkptr)p)->fwd = ptr2offset (q))
#define set_bck(p, q)   (((chunkptr)p)->bck = ptr2offset (q))

/*
 * List operations.
 */

#define remove_from_list(p)       { \
  chunkptr fwd = forward (p); \
  chunkptr bck = back (p); \
  fwd->bck = p->bck; \
  bck->fwd = p->fwd; \
}

#define replace_in_list(p, q) { \
  set_fwd (back (p), q); \
  set_bck (forward (p), q); \
  q->fwd = p->fwd; \
  q->bck = p->bck; \
}

#define add_to_list(l, p) { \
  p->fwd = l->fwd; \
  set_bck (p, l); \
  set_bck (forward (l), p); \
  set_fwd (l, p); \
}

#define append_to_list(l, p) { \
  set_fwd (p, l); \
  set_bck (p, back (l)); \
  set_fwd (back (l), p); \
  set_bck (l, p); \
}

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
static UINT32 listcount = 0;
static UINT32 chunkcount = 0;
#endif



/*
 * Initialize the WIP memory allocation library.
 * The first parameter is a pointer to a block of memory,
 * and the second is the size of the memory area.
 * Returns 0 on success, and -1 otherwise.
 */ 
  
INT16
au_initmalloc (void *mem, UINT32 memsize)
{
  chunkptr body, p;

#ifdef debug
  CLNTa_log(1,0,"\nau_initmalloc() is called!!");
#endif//#ifdef debug

  /* Truncate to nearest multiple of the alignment. */
  memsize &= ~MALLOC_ALIGN_MASK;
  if ((mem == NULL) || (memsize < MINBLOCKSIZE) || (memsize > (1L << 17))) {
    return -1;
  }
  baseptr = (BYTE*)mem;

  /* The first chunk will serve as header for the doubly linked list
   * of free chunks. We mark it as "in use" to avoid having it merged
   * with an adjacent chunk on freeing. */
  p = (chunkptr)baseptr;
  set_hd1 (p, 0);
  set_hd2 (p, (MINCHUNKSIZE >> 1) | 0x01);
  p->fwd = ptr2offset (p);
  p->bck = ptr2offset (p);
  freelist = p;

  /* This chunk starts out on the free list and contains almost all
   * of the available memory. */
  body = (chunkptr)(((BYTE *)baseptr) + MINCHUNKSIZE);
  set_hd1 (body, MINCHUNKSIZE >> 1);
  set_hd2 (body, (memsize - (2 * MINCHUNKSIZE)) >> 1);
  add_to_list (((chunkptr)baseptr), body);

  /* The last chunk is never placed on any list; it is marked "in use",
   * and simply acts as a sentry element, when scanning all the chunks. */
  lastchunk = (chunkptr)(((BYTE *)baseptr) + memsize - MINCHUNKSIZE);
  set_hd1 (lastchunk, (memsize - 2 * MINCHUNKSIZE) >> 1);
  set_hd2 (lastchunk, (MINCHUNKSIZE >> 1) | 0x01);

  return 0;
}

/*
 * wip_malloc and wip_gcmalloc are almost identical, so it would make
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


void *
au_malloc (UINT32 size)
{
  chunkptr p, q;
  UINT32   nb;

  /* We add space for our overhead (4 bytes) plus round to nearest
   * larger multiple of 4, plus never allocate a chunk less than 8 bytes. */
  nb = request2size (size);

#ifdef DEBUG_WIP_MALLOC
  traverse_count  = 0;
#endif
  /* Use first fit strategy */
  for (p = forward (freelist); p != freelist; p = forward (p)) {
    UINT32 sz = chunksize (p);

#ifdef DEBUG_WIP_MALLOC
    traverse_count++;
#endif
    if (sz >= nb) {
      UINT32 remsize = sz - nb;

      /* We have found a large enough chunk. */

      if (remsize >= MINCHUNKSIZE) {
        /* The remainder is large enough to become a separate chunk */
        chunkptr next;

        q = (chunkptr)((BYTE *)p + nb);
        set_hd2 (q, remsize >> 1);
        set_hd1 (q, nb >> 1);
        next = nextchunk (q);
        set_prevsize (next, remsize);

        replace_in_list (p, q);
        set_hd2 (p, nb >> 1 | 0x01);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = nb;
#endif
      }
      else {
        /* Use the chunk as is, without splitting */
        remove_from_list (p);
        set_inuse (p);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = sz;
#endif
      }
      clear_gcbit (p);

#ifdef DEBUG_WIP_MALLOC
      bytes_allocated += last_alloc;
      last_req = size;
      bytes_requested += last_req;
      currently_allocated += last_alloc;
      if (currently_allocated > max_allocated)
        max_allocated = currently_allocated;
      num_alloc++;
#endif
      return chunk2mem (p);
    }
  }

  //wip_printbriefstats();
  /* The freelist contained no chunk that was large enough. */
  return NULL;
}

/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Memory blocks allocated with this routine can be freed in one
 * batch operation by calling wip_gc.
 * Returns 0 if allocation fails.
 */




#ifdef MEMORY_ENHANCED


void *
au_gcmalloc (UINT32 size)
{
  chunkptr p, q;
  UINT32   nb;
//  int traverse_count ;
  UINT32   right_size = 50000;
  UINT32   tmp_size;
  UINT32   sz;
  /* We add space for our overhead (4 bytes) plus round to nearest
   * larger multiple of 4, plus never allocate a chunk less than 8 bytes. */
  nb = request2size (size);

#ifdef DEBUG_WIP_MALLOC
  traverse_count  = 0;
#endif
  /* Use first fit strategy */

  for (p = forward (freelist); p != freelist; p = forward (p)) {
    sz = chunksize (p);
	if (sz >= nb) {
		tmp_size = sz;
		if( right_size > tmp_size) {
			right_size = tmp_size;
		}
	}
  }


  for (p = forward (freelist); p != freelist; p = forward (p)) {
    sz = chunksize (p);

#ifdef DEBUG_WIP_MALLOC
    traverse_count++;
#endif
	if( right_size != 50000 && sz == right_size) {
    if (sz >= nb) {
      UINT32 remsize = sz - nb;

      /* We have found a large enough chunk. */

      if (remsize >= MINCHUNKSIZE) {
        /* The remainder is large enough to become a separate chunk */
        chunkptr next;

        q = (chunkptr)((BYTE *)p + nb);
        set_hd2 (q, remsize >> 1);
        set_hd1 (q, nb >> 1);
        next = nextchunk (q);
        set_prevsize (next, remsize);

        replace_in_list (p, q);
        set_hd2 (p, nb >> 1 | 0x01);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = nb;
#endif
      }
      else {
        /* Use the chunk as is, without splitting */
        remove_from_list (p);
        set_inuse (p);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = sz;
#endif
      }
      set_gcbit (p);

#ifdef DEBUG_WIP_MALLOC
      bytes_allocated += last_alloc;
      last_req = size;
      bytes_requested += last_req;
      currently_allocated += last_alloc;
      if (currently_allocated > max_allocated)
        max_allocated = currently_allocated;
      num_alloc++;
#endif
      return chunk2mem (p);
    }
	}
  }
#ifdef DEBUG_WIP_MALLOC
  CLNTa_log(1,0, "gc alloc fail : %ld requested %d count %ld currently allocated %d sz", size, traverse_count, currently_allocated, sz);
#endif //#ifdef DEBUG_WIP_MALLOC
  /* The freelist contained no chunk that was large enough. */
  return NULL;
}

#else //#ifdef MEMORY_ENHANCED

void *
au_gcmalloc (UINT32 size)
{
  chunkptr p, q;
  UINT32   nb;
  

  /* We add space for our overhead (4 bytes) plus round to nearest
   * larger multiple of 4, plus never allocate a chunk less than 8 bytes. */
  nb = request2size (size);

#ifdef DEBUG_WIP_MALLOC
  traverse_count  = 0;
#endif
  /* Use first fit strategy */
  for (p = forward (freelist); p != freelist; p = forward (p)) {
    UINT32 sz = chunksize (p);

#ifdef DEBUG_WIP_MALLOC
    traverse_count++;
#endif
    if (sz >= nb) {
      UINT32 remsize = sz - nb;

      /* We have found a large enough chunk. */

      if (remsize >= MINCHUNKSIZE) {
        /* The remainder is large enough to become a separate chunk */
        chunkptr next;

        q = (chunkptr)((BYTE *)p + nb);
        set_hd2 (q, remsize >> 1);
        set_hd1 (q, nb >> 1);
        next = nextchunk (q);
        set_prevsize (next, remsize);

        replace_in_list (p, q);
        set_hd2 (p, nb >> 1 | 0x01);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = nb;
#endif
      }
      else {
        /* Use the chunk as is, without splitting */
        remove_from_list (p);
        set_inuse (p);
#ifdef DEBUG_WIP_MALLOC
        last_alloc = sz;
#endif
      }
      set_gcbit (p);

#ifdef DEBUG_WIP_MALLOC
      bytes_allocated += last_alloc;
      last_req = size;
      bytes_requested += last_req;
      currently_allocated += last_alloc;
      if (currently_allocated > max_allocated)
        max_allocated = currently_allocated;
      num_alloc++;
#endif
      return chunk2mem (p);
    }
  }
#ifdef DEBUG_WIP_MALLOC
  //wip_printbriefstats();
  //CLNTa_log(1,0, "gc alloc fail : %ld requested %d count %ld currently allocated %d chunkcount", size, traverse_count, currently_allocated, chunkcount);
#endif //#ifdef DEBUG_WIP_MALLOC
  /* The freelist contained no chunk that was large enough. */
  return NULL;
  
}

#endif //#ifdef MEMORY_ENHANCED

/*
 * Free a memory area previously allocated with wip_malloc or wip_gcmalloc.
 * Calling this routine with 'mem' equal to 0 has no effect.
 */
void
au_free (void* mem)
{
  chunkptr p;         /* chunk corresponding to mem */
  UINT32   sz;        /* its size */
  chunkptr next;      /* next adjacent chunk */
  chunkptr prev;      /* previous adjacent chunk */

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
  }
  
  if (chunk_isfree (next)) {            /* Join with next chunk */
    sz += chunksize (next);
    remove_from_list (next);
  }

  set_hd2 (p, sz >> 1);
  clear_gcbit (p);
  next = nextchunk (p);
  set_prevsize (next, sz);

  if (p != prev) {
    add_to_list (freelist, p);
  }
}


/*
 * Free all memory areas allocated with wip_gcmalloc.
 * Memory blocks allocated with wip_malloc are left undisturbed.
 */
void
au_gc (void)
{
  chunkptr p;

  for (p = nextchunk (((chunkptr)baseptr)); p != lastchunk;
       p = nextchunk (p)) {
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

      add_to_list (freelist, p);
    }
  }
}


/*
 * Return the size of an allocated memory area.
 * The reported size includes the overhead used by the memory allocation
 * system.
 */
UINT32
au_memsize (void *mem)
{
  chunkptr p;         /* chunk corresponding to mem */

  p = mem2chunk (mem);
  return chunksize (p);
}



#ifdef DEBUG_WIP_MALLOC

/*
 * Routines for debugging and statistics.
 */
/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */

void
au_printbriefstats(void)
{
  chunkptr p;
  UINT32 sz =0;
 
  for (p = forward (freelist); p != freelist; p = forward (p)) {
	sz += chunksize (p);
    listcount++;
  }
  for (p = nextchunk ((chunkptr)baseptr); p != lastchunk; p = nextchunk (p)) {
    chunkcount++;
  }
  if (last_req < 0) {
    last_req = 0;
  }
#ifdef debug
  CLNTa_log(1,0, "%6d%6d%6d%3d%3d%4d%6d%6d", last_req, last_alloc,
           currently_allocated, traverse_count, listcount, chunkcount, sz, sz+currently_allocated);
#endif//#ifdef debug
}

/*
 * Prints accumulated statistics.
 */
void
au_printstats (void)
{
#ifdef debug
  CLNTa_log(1,0, "Bytes requested: %d\n", bytes_requested);
  CLNTa_log(1,0, "Bytes allocated: %d\n", bytes_allocated);
  CLNTa_log(1,0, "Bytes currently allocated: %d\n", currently_allocated);
  CLNTa_log(1,0, "Max bytes allocated: %d\n", max_allocated);
  CLNTa_log(1,0, "Number of allocations: %d\n", num_alloc);
  CLNTa_log(1,0, "Number of free: %d\n", num_free);
#endif //#ifdef debug
}

#endif //#ifdef DEBUG_WIP_MALLOC


#ifdef BLOCK
/*
 * Print all currently allocated chunks.
 */
void
au_printalloc (FILE *logfile)
{
  chunkptr p = (chunkptr)baseptr;

  p = nextchunk (p);
  while (p != lastchunk) {
    if (!chunk_isfree (p)) {
      fprintf (logfile, "id: %d\tsize: %d\n", 0, chunksize (p));
    }
    p = nextchunk (p);
  }
}


/*
 * Prints a list of all the current chunks.
 */
void
au_printchunks (FILE *logfile)
{
  chunkptr p = (chunkptr)baseptr;

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
void
au_printfreelist (FILE *logfile)
{
  chunkptr p;
  UINT32 sz;

  fprintf (logfile, "\t");
  for (p = forward (freelist); p != freelist; p = forward (p)) {
    sz = chunksize (p);
    fprintf (logfile, " %d", sz);
  }
}


void
au_printbriefstats (FILE *logfile)
{
  chunkptr p;

  UINT32 listcount = 0;
  UINT32 chunkcount = 0;

  for (p = forward (freelist); p != freelist; p = forward (p)) {
    listcount++;
  }
  for (p = nextchunk ((chunkptr)baseptr); p != lastchunk; p = nextchunk (p)) {
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
void
au_printstats (FILE *logfile)
{
  fprintf (logfile, "Bytes requested: %d\n", bytes_requested);
  fprintf (logfile, "Bytes allocated: %d\n", bytes_allocated);
  fprintf (logfile, "Bytes currently allocated: %d\n", currently_allocated);
  fprintf (logfile, "Max bytes allocated: %d\n", max_allocated);
  fprintf (logfile, "Number of allocations: %d\n", num_alloc);
  fprintf (logfile, "Number of free: %d\n", num_free);
}
#endif //#ifdef BLOCK

//#endif //#ifdef USE_WIP_MALLOC
