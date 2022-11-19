/*
** ============================================================================
**
** Project: 
**     SkySimul
**
** File:
**     sky_util.h
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
** 2000-12-02 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _SKY_UTIL_H_
#define _SKY_UTIL_H_

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
#ifdef FEATURE_SKY_ASSERT
#define SKY_ASSERT          EQS_ASSERT
#define SKY_ASSERT_EX       EQS_ASSERT_EX
#define SKY_ASSERT_VOID     EQS_ASSERT_VOID
#else
#define SKY_ASSERT          ((void)0)
#define SKY_ASSERT_EX       ((void)0)
#define SKY_ASSERT_VOID     ((void)0)
#endif

#ifdef FEATURE_SKY_TRACE
#define SKY_TRACE           EQS_TRACE
#define SKY_TRACE1          EQS_TRACE1
#define SKY_TRACE2          EQS_TRACE2

#define SKY_ERROR           EQS_ERROR
#define SKY_ERROR1          EQS_ERROR1
#define SKY_ERROR2          EQS_ERROR2

#define SKY_ERROR_FATAL     EQS_ERROR
#define SKY_ERROR_FATAL1    EQS_ERROR1
#define SKY_ERROR_FATAL2    EQS_ERROR2
#else
#define SKY_TRACE           ((void)0)
#define SKY_TRACE1          ((void)0)
#define SKY_TRACE2          ((void)0)

#define SKY_ERROR           ((void)0)
#define SKY_ERROR1          ((void)0)
#define SKY_ERROR2          ((void)0)
#endif

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

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _SKY_UTIL_H_ */
