/*
** ============================================================================
**
** Project: 
**     Graphics
**
** File:
**     eqs_mem.h
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
** ============================================================================
*/

#ifndef _EQS_MEM_H_
#define _EQS_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

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
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/
EQS_DCL WORD  EQS_API EQS_InitMemory(VOID *pMemBlock, UINT32 nMemSize);

EQS_DCL VOID* EQS_API EQS_Malloc(SIZE_T stSize);
EQS_DCL VOID* EQS_API EQS_Calloc(SIZE_T stElement, SIZE_T stSize);
EQS_DCL VOID* EQS_API EQS_Memset(VOID* pDest, BYTE bFillChar, SIZE_T stCount);
EQS_DCL VOID* EQS_API EQS_Memcpy(VOID* pDest, CONST VOID* pSrc, SIZE_T stCount);

EQS_DCL VOID EQS_API EQS_Free(VOID** ppMem);

#ifdef FEATURE_EQS_DEBUG_MEM
EQS_DCL INT16 EQS_API EQS_GetMemAllocCount(VOID);
#endif


#ifdef __cplusplus
}
#endif

#endif  /* _EQS_MEM_H_ */
