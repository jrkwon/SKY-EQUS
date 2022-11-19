/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqswutil.c
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
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <string.h>     /* strxxxx */
#include <stdlib.h>     /* _MAX_FNAME, _MAX_EXT */

#include "eqswutil.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define INIT_TRACE_LEVEL    TL_LOW

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
LOCAL TRACE_LEVEL_E l_TraceLevel = INIT_TRACE_LEVEL;


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/* 
** ---------------------------------------------------------------------------
**
** Function:    
**     @functionName()
**
** Description: 
**     @DesciptionAboutThisFileHere
**
** Input:
**     @ArgumentsList
**
** Output:
**     @TheResultOfThisFunction
**
** Return value:
**     @ReturnValue
**
** Side effects:
**     @SideEffects
**
** ---------------------------------------------------------------------------
*/
/* trace ------------------------------------------------------------------- */
void EQS_API EQS_SetTraceLevel(TRACE_LEVEL_E Level)
{
    l_TraceLevel = Level;
}

TRACE_LEVEL_E EQS_API EQS_GetTraceLevel(void)
{
    return l_TraceLevel;
}

void EQS_API EQS_WIN32_Trace(CONST BYTE *pFmt, BYTE *pszFileName, 
                             INT nLine, BYTE *pszMsg)
{
    /* extract the file name */
    BYTE szFileName[_MAX_FNAME+_MAX_EXT];
    BYTE *pbPos;

    pbPos = (BYTE*)strrchr((const char*)pszFileName, '\\');
    if(pbPos != NULL)
        strcpy((char*)szFileName, (char*)pbPos+1);
//    TRACE((char*)pFmt, szFileName, nLine, pszMsg);
}

void EQS_API EQS_WIN32_Trace1(CONST BYTE *pFmt, BYTE *pszFileName, 
                             INT nLine, BYTE *pszMsg, INT nArg1)
{
    /* extract the file name */
    BYTE szFileName[_MAX_FNAME+_MAX_EXT];
    BYTE *pbPos;

    pbPos = (BYTE*)strrchr((const char*)pszFileName, '\\');
    if(pbPos != NULL)
        strcpy((char*)szFileName, (char*)pbPos+1);
//    TRACE((char*)pFmt, szFileName, nLine, pszMsg, nArg1);
}

void EQS_API EQS_WIN32_Trace2(CONST BYTE *pFmt, BYTE *pszFileName, 
                             INT nLine, BYTE *pszMsg, INT nArg1, INT nArg2)
{
    /* extract the file name */
    BYTE szFileName[_MAX_FNAME+_MAX_EXT];
    BYTE *pbPos;

    pbPos = (BYTE*)strrchr((const char*)pszFileName, '\\');
    if(pbPos != NULL)
        strcpy((char*)szFileName, (char*)pbPos+1);
//    TRACE((char*)pFmt, szFileName, nLine, pszMsg, nArg1, nArg2);
}


#ifdef __cplusplus
}
#endif
