/*
** ============================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqsrutil.h
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

#ifndef _EQSRUTIL_H_
#define _EQSRUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/
#include "msg.h"        /* MSG_XXXX */
#include "assert.h"     /* ASSERT   */
#include "err.h"        /* ERR_XXXX */

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/* Assert Support */
/* use ASSERT macro defined in assert.h */
#ifdef FEATURE_EQS_ASSERT
#define EQS_ASSERT(Exp)                     ASSERT(Exp)

#define EQS_ASSERT_EX(Exp, ReturnValue)     \
{ \
    if(!(Exp)) \
    { \
        if(g_fSKTT_Release) \
            return ReturnValue; \
        else \
            ASSERT(Exp); \
    } \
}

#define EQS_ASSERT_VOID(Exp)     \
{ \
    if(!(Exp)) \
    { \
        if(g_fSKTT_Release) \
            return; \
        else \
            ASSERT(Exp); \
    } \
}

#else
#define EQS_ASSERT(Exp)                     ((void)0)
#define EQS_ASSERT_EX(Exp, ReturnValue)     ((void)0)
#define EQS_ASSERT_VOID(Exp)                ((void)0)
#endif /* FEATURE_EQS_ASSERT */

/* Trace Support */
#ifdef FEATURE_EQS_TRACE
#define EQS_TRACE(Level, pszMsg)                                        \
{                                                                       \
    if(Level >= MSG_LEVEL)                                              \
        MSG_BASE(MSG_LEVEL, pszMsg, 0, 0, 0);                           \
}
#define EQS_TRACE1(Level, pszMsg, nArg1)                                \
{                                                                       \
    if(Level >= MSG_LEVEL)                                              \
        MSG_BASE(MSG_LEVEL, pszMsg, nArg1, 0, 0);                       \
}
#define EQS_TRACE2(Level, pszMsg, nArg1, nArg2)                         \
{                                                                       \
    if(Level >= MSG_LEVEL)                                              \
        MSG_BASE(MSG_LEVEL, pszMsg, nArg1, nArg2, 0);                   \
}
#define EQS_ERROR(pszMsg)                                               \
{                                                                       \
    ERR(pszMsg, 0, 0, 0);                                               \
}
#define EQS_ERROR1(pszMsg, nArg1)                                       \
{                                                                       \
    ERR(pszMsg, nArg1, 0, 0);                                           \
}
#define EQS_ERROR2(pszMsg, nArg1, nArg2)                                \
{                                                                       \
    ERR(pszMsg, nArg1, nArg2, 0);                                       \
}
#else
#define EQS_TRACE(Level, pszMsg)                    ((void)0)
#define EQS_TRACE1(Level, pszMsg, nArg1)            ((void)0)
#define EQS_TRACE2(Level, pszMsg, nArg1, nArg2)     ((void)0)
#define EQS_ERROR(Level, pszMsg)                    ((void)0)
#define EQS_ERROR1(Level, pszMsg, nArg1)            ((void)0)
#define EQS_ERROR2(Level, pszMsg, nArg1, nArg2)     ((void)0)
#endif

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
enum tagTRACE_LEVEL_E
{
    TL_LOW      = MSG_LVL_LOW,
    TL_MED      = MSG_LVL_MED,
    TL_HIGH     = MSG_LVL_HIGH,
    TL_ERROR    = MSG_LVL_ERROR,
    TL_FATAL    = MSG_LVL_FATAL
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
//extern TRACE_LEVEL_E EQS_API EQS_REX_GetTraceLevel(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQSQUTIL_H_ */
