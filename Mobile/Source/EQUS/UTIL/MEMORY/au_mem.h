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
 *   2000-12-20, girin : Not to overlap with WAP build, all related 
 *                       header file names and function names are 
 *                       changed using 'au_' prefix.
 *                       tapicmmn.h --> au_cmmn.h
 *                       confvars.h --> au_cfvr.h
 *                       wapmemory.h --> au_mmry.h
 *                       waptarget.h --> au_trgt.h
 *                       wip_mem.h --> au_mem.h
 *                       commrsrc.h --> au_rsrc.h
 * 
 *                       EQUS always need these routines 
 *                       regardless of WAP porting.
 */
#ifndef _au_mem_h
#define _au_mem_h

//#define USE_WIP_MALLOC

#include "au_cfvr.h"

//#ifdef USE_WIP_MALLOC

//#define DEBUG_WIP_MALLOC
#define MEMORY_ENHANCED

#if defined LOG_INTERNAL && !defined DEBUG_WIP_MALLOC
#define DEBUG_WIP_MALLOC
#endif

#include "au_rsrc.h" // <-- "CmmnRsrc.h" : girin. 2000.12.20

/*
 * Initialize the WIP memory allocation library.
 * The first parameter is a pointer to a block of memory,
 * and the second is the size of that memory area.
 * Returns 0 on success, and -1 otherwise.
 */
INT16 au_initmalloc (void *mem, UINT32 memsize);


/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Returns 0 if allocation fails.
 */
void *au_malloc (UINT32 size);

/*
 * Free a memory area previously allocated with wip_malloc or wip_gcmalloc.
 * Calling this routine with 'mem' equal to 0 has no effect.
 */
void au_free (void* mem);


/*
 * Allocate and return a pointer to a memory area of at least
 * the indicated size. The allocated block of memory will be aligned
 * on a 4-byte boundary.
 * Memory blocks allocated with this routine can be freed in one
 * batch operation by calling wip_gc.
 * Returns 0 if allocation fails.
 */
void *au_gcmalloc (UINT32 size);


/*
 * Free all memory areas allocated with wip_gcmalloc.
 * Memory blocks allocated with wip_malloc are left undisturbed.
 */
void au_gc (void);


/*
 * Return the size of an allocated memory area.
 * The reported size includes the overhead used by the memory allocation
 * system.
 */
UINT32
au_memsize (void *mem);


#ifdef DEBUG_WIP_MALLOC

/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */
void au_printbriefstats (void);

/* Prints accumulated statistics. */
void au_printstats (void);

#endif //#ifdef DEBUG_WIP_MALLOC


#ifdef BLOCK
/* Print all currently allocated chunks. */
void au_printalloc (FILE *logfile);

/* Prints a list of all the current chunks. */
void au_printchunks (FILE *logfile);

/* Prints a sequence of numbers representing the sizes of
 * the chunks on the free list. */
void au_printfreelist (FILE *logfile);

/*
 * Print brief statistics:
 *   the number of bytes requested in the last allocation/free made
 *   the number of bytes allocated in the last allocation/free made
 *   the total number of bytes currently allocated
 *   the number of chunks checked in the last allocation
 *   the current length of the free list
 *   the current number of chunks that the memory is divided into
 */
void au_printbriefstats (FILE *logfile);

/* Prints accumulated statistics. */
void au_printstats (FILE *logfile);
#endif //#ifdef BLOCK

#endif

//#endif
