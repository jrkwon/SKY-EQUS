/*
** ===========================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_mem.c
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
** 2000-06-27 jrkwon     Created.
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
#include "eqs.h"

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
#ifdef FEATURE_EQS_DEBUG_MEM
LOCAL INT16     nAllocCount = 0;
LOCAL void*     l_pBaseMemory;
LOCAL INT       l_nMemorySize;
#endif /* FEATURE_EQS_DEBUG_MEM */

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

// Target dependent ...
extern signed short au_initmalloc(void *mem, unsigned long memsize);
extern void *au_malloc(unsigned long size);
extern void au_free(void *mem);



WORD EQS_API EQS_InitMemory(VOID *pMemBlock, UINT32 nMemSize)
{

#ifdef FEATURE_EQS_DEBUG_MEM
    l_pBaseMemory = pMemBlock;
    l_nMemorySize = nMemSize;
#endif /* FEATURE_EQS_DEBUG_MEM */
	return au_initmalloc(pMemBlock, nMemSize);
}


VOID* EQS_API EQS_Malloc(SIZE_T stSize)
{
    VOID* pMemBlock;

	pMemBlock = au_malloc(stSize);

    
    EQS_ASSERT(pMemBlock != NULL);

#ifdef FEATURE_EQS_DEBUG_MEM
    nAllocCount++;
#endif /* FEATURE_EQS_DEBUG_MEM */

    return pMemBlock;
}

VOID* EQS_API EQS_Calloc(SIZE_T stElement, SIZE_T stSize)
{
    VOID* pMemBlock;

	pMemBlock = EQS_Malloc(stSize*stElement);
	memset(pMemBlock, 0, stElement*stSize);
    
    EQS_ASSERT(pMemBlock != NULL);

#ifdef FEATURE_EQS_DEBUG_MEM
    nAllocCount++;
#endif /* FEATURE_EQS_DEBUG_MEM */

    return pMemBlock;
}

/* 
VOID* EQS_API EQS_Memset(VOID* pDest, BYTE bFillChar, SIZE_T stCount)
{
    return memset(pDest, bFillChar, stCount);
}

VOID* EQS_API EQS_Memcpy(VOID* pDest, CONST VOID* pSrc, SIZE_T stCount)
{
    return memcpy(pDest, pSrc, stCount);
}
*/

VOID EQS_API EQS_Free(VOID** ppMemBlock)
{
    /*
    ** trying to free null pointer should've generated assertion error.
    ** but, ....
    */
    //EQS_ASSERT(pMemBlock != NULL);
    if(*ppMemBlock == NULL)
        return;

#ifdef FEATURE_EQS_DEBUG_MEM
    EQS_ASSERT(  nAllocCount > 0 );	
	EQS_ASSERT( (BYTE*)*ppMemBlock >= (BYTE*)l_pBaseMemory && (BYTE*)*ppMemBlock < (BYTE*)l_pBaseMemory + l_nMemorySize );
#endif /* FEATURE_EQS_DEBUG_MEM */

	au_free(*ppMemBlock);
	*ppMemBlock = NULL;

#ifdef FEATURE_EQS_DEBUG_MEM
    nAllocCount--;
#endif /* FEATURE_EQS_DEBUG_MEM */
}

#ifdef FEATURE_EQS_DEBUG_MEM
INT16 EQS_API EQS_GetMemAllocCount(VOID)
{
    return nAllocCount;
}
#endif

