#ifndef _au_mmry_h
#define _au_mmry_h

signed short au_initmalloc (void *mem, unsigned long memsize);

void *au_malloc (unsigned long size);

void au_free (void* mem);

void *au_gcmalloc (unsigned long size);

void au_gc (void);

unsigned long au_memsize (void *mem);

#endif //_wapmemory_h

