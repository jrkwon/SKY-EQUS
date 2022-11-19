/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqswutil.h
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
** 2000-10-26 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQSWUTIL_H_
#define _EQSWUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include <crtdbg.h>     /* ASSERT */
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* EQS_ASSERT Support */
#ifdef FEATURE_EQS_ASSERT

#define EQS_ASSERT(Exp)                          _ASSERT(Exp)
#define EQS_ASSERT_EX(Exp, ReturnValue)     \
{ \
    if(!(Exp)) \
    { \
        if(g_fSKTT_Release) \
            return ReturnValue; \
        else \
            _ASSERT(Exp); \
    } \
}

#define EQS_ASSERT_VOID(Exp)     \
{ \
    if(!(Exp)) \
    { \
        if(g_fSKTT_Release) \
            return; \
        else \
            _ASSERT(Exp); \
    } \
}


#ifndef ASSERT
#define ASSERT      EQS_ASSERT   // temp for compilation 
#endif

#else

#define EQS_ASSERT(Exp)                     ((void)0)
#define EQS_ASSERT_EX(Exp, ReturnValue)     ((void)0)
#define EQS_ASSERT_VOID(Exp)                ((void)0)

#endif /* FEATURE_EQS_EQS_EQS_ASSERT */

/* Trace Support */
#ifdef FEATURE_EQS_TRACE
#define EQS_TRACE(Level, pszMsg)                                        \
{                                                                       \
    if(Level >= EQS_GetTraceLevel())                                    \
       EQS_WIN32_Trace("%s\t(%d)\t:%s\n", __FILE__, __LINE__, pszMsg);  \
}
#define EQS_TRACE1(Level, pszMsg, nArg)                                 \
{                                                                       \
    if(Level >= EQS_GetTraceLevel())                                    \
       EQS_WIN32_Trace1("%s\t(%d)\t:%s(%d)\n", __FILE__, __LINE__,      \
                                             pszMsg, nArg);             \
}
#define EQS_TRACE2(Level, pszMsg, nArg1, nArg2)                         \
{                                                                       \
    if(Level >= EQS_GetTraceLevel())                                    \
       EQS_WIN32_Trace2("%s\t(%d)\t:%s(%d, %d)\n", __FILE__, __LINE__,  \
                                                 pszMsg, nArg1, nArg2); \
}
/* 
** EQS_ERROR saves the error message on the nv in REX version.
** but, we don't in WIN32 version
*/
#define EQS_ERROR(pszMsg)                  EQS_TRACE(TL_ERROR, pszMsg)
#define EQS_ERROR1(pszMsg, nArg)           EQS_TRACE1(TL_ERROR, pszMsg, nArg)
#define EQS_ERROR2(pszMsg, nArg1, nArg2)   EQS_TRACE2(TL_ERROR, pszMsg, nArg1, nArg2)
#else
#define EQS_TRACE(Level, pszMsg)                    ((void)0)
#define EQS_TRACE1(Level, pszMsg, nArg)             ((void)0)
#define EQS_TRACE2(Level, pszMsg, nArg1, nArg2)     ((void)0)
#define EQS_ERROR(Level, pszMsg)                    ((void)0)
#define EQS_ERROR1(Level, pszMsg, nArg)             ((void)0)
#define EQS_ERROR2(Level, pszMsg, nArg1, nArg2)     ((void)0)
#endif

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagTRACE_LEVEL_E
{
    TL_LOW,
    TL_MED,
    TL_HIGH,
    TL_ERROR,
    TL_FATAL
};
typedef enum tagTRACE_LEVEL_E   TRACE_LEVEL_E;

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
/* do not use this directly, but use EQS_TRACE macro */
extern TRACE_LEVEL_E EQS_API EQS_GetTraceLevel(void);

extern void EQS_API EQS_SetTraceLevel(TRACE_LEVEL_E Level);
extern void EQS_API EQS_WIN32_Trace(CONST BYTE *pFmt, BYTE *pszFileName, 
                                    INT nLine, BYTE *pszMsg);
extern void EQS_API EQS_WIN32_Trace1(CONST BYTE *pFmt, BYTE *pszFileName, 
                                    INT nLine, BYTE *pszMsg, INT nArg1);
extern void EQS_API EQS_WIN32_Trace2(CONST BYTE *pFmt, BYTE *pszFileName, 
                                    INT nLine, BYTE *pszMsg, 
                                    INT nArg1, INT nArg2);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQSWUTIL_H_ */
