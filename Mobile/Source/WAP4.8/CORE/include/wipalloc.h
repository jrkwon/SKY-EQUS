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
#ifndef WIPALLOC_H
#define WIPALLOC_H

#include "tapicmmn.h"

#ifdef LOG_INTERNAL

	#define OSConnectorAlloc(size) WIPAlloc(size, __FILE__, __LINE__)
	#define OSConnectorFree(block) WIPFree(block, __FILE__, __LINE__)

	/* Allocates a struct */
	#define NEWSTRUCT(type) (p##type)WIPAlloc(sizeof(type), __FILE__, __LINE__)

	/* Allocates an array of elements */
	#define NEWARRAY(type,num) (type*)WIPAlloc((num)*sizeof(type), __FILE__, __LINE__)

	/* Deletes a block of memory. TAKES A POINTER TO THE POINTER! */
	#define DEALLOC(pp) if(*(pp)!=NULL){WIPFree((VOID*)*(pp), __FILE__, __LINE__);*(pp)=NULL;}

	VOID* WIPAlloc( UINT32 size, const char *file, int lineNo );
	VOID WIPFree( VOID* block, const char *file, int lineNo );

    /* TRUE means that the memory allocator never returns
       if out of memory occurs. FALSE means that the memory
       allocator returns NULL if out of memory occurs.
       It returns the old value. */
    BOOL SetMemoryGuard(BOOL);

#else

	#define OSConnectorAlloc(size) WIPAlloc(size)
	#define OSConnectorFree(block) WIPFree(block)

	/* Allocates a struct */
	#define NEWSTRUCT(type) (p##type)WIPAlloc(sizeof(type))

	/* Allocates an array of elements */
	#define NEWARRAY(type,num) (type*)WIPAlloc((num)*sizeof(type))

	/* Deletes a block of memory. TAKES A POINTER TO THE POINTER! */
	#define DEALLOC(pp) if(*(pp)!=NULL){WIPFree((VOID*)*(pp));*(pp)=NULL;}

	VOID* WIPAlloc( UINT32 size );
	VOID WIPFree( VOID* block );

    /* TRUE means that the memory allocator never returns
       if out of memory occurs. FALSE means that the memory
       allocator returns NULL if out of memory occurs.
       It returns the old value. */
	BOOL SetMemoryGuard(BOOL);

#endif


#endif
