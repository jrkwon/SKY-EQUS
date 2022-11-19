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
 * wip_mem.h
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
 *   990721, ONJ: USE_WIP_MALLOC shall be taken from confvars.h
 *                Standard ANSI C files are taken from ansilibs.h,
 *                through cmmnrsrc.h.
 *   990817, AED: Added a diagnostic routine wip_printalloc, and a function
 *                to return the size of a memory area, wip_memsize.
 *
 */
#ifndef _wip_mem_h
#define _wip_mem_h

#include "confvars.h"

#ifdef USE_WIP_MALLOC

#if defined LOG_INTERNAL && !defined DEBUG_WIP_MALLOC
#define DEBUG_WIP_MALLOC
#endif

#include "cmmnrsrc.h"

/*
 * Initialize the WIP memory allocation library.
 * The first parameter is a pointer to a block of memory,
 * and the second is the size of that memory area.
 * Returns 0 on success, and -1 otherwise.
 */
INT16 wip_initmalloc (void *mem, UINT32 memsize);


/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Returns 0 if allocation fails.
 */
void *wip_malloc (UINT32 size);

/*
 * Free a memory area previously allocated with wip_malloc or wip_gcmalloc.
 * Calling this routine with 'mem' equal to 0 has no effect.
 */
void wip_free (void* mem);


/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Memory blocks allocated with this routine can be freed in one
 * batch operation by calling wip_gc.
 * Returns 0 if allocation fails.
 */
void *wip_gcmalloc (UINT32 size);


/*
 * Free all memory areas allocated with wip_gcmalloc.
 * Memory blocks allocated with wip_malloc are left undisturbed.
 */
void wip_gc (void);


/*
 * Return the size of an allocated memory area.
 * The reported size includes the overhead used by the memory allocation
 * system.
 */
UINT32
wip_memsize (void *mem);


#ifdef DEBUG_WIP_MALLOC
typedef struct wip_malloc_stats_st {
  int bytes_allocated;
  int bytes_requested;
  int currently_allocated;
  int max_allocated;
  int last_alloc;
  int last_req;
  int num_alloc;
  int num_free;
  int traverse_count;
  int maxfreelistlength;
} wip_malloc_stats;

/* Print all currently allocated chunks. */
void wip_printalloc (FILE *logfile);

/* Prints a list of all the current chunks. */
void wip_printchunks (FILE *logfile);

/* Prints a sequence of numbers representing the sizes of
 * the chunks on the free list. */
void wip_printfreelist (FILE *logfile);

/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */
void wip_printbriefstats (FILE *logfile);

/* Get accumulated statistics. */
wip_malloc_stats
wip_getstats (void);

/*
 * Measure "fragmentation". The function measures the ratio
 * of the largest free chunk and the total free area.
 * The returned figure represents the largest memory block
 * possible to allocate, as a percentage of the total free area.
 * Hence, 100% means that there is NO fragmentation, and a small
 * number means that there is extensive fragmentation. 
 */
INT16
wip_getfragmentation (void);

/*
 * Besides the fragmentation value, this function also prints
 * the values of the total free area and the largest free chunk.
 */
void
wip_fragmentationstats (FILE *logfile);

#endif
#endif
#endif
