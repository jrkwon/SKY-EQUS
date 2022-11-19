/*
** ===========================================================================
**
** Project: 
**     Bunny
**
** File:
**     ska_sbel.c
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
** 01-01-02 rosa     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "ska.h" 

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

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
//static  VOID SKY_API sSKA_SetAlertBell( SKY_EVENT_T *pEvent );

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
VOID SKY_API SKA_MainAlertBell(SKY_EVENT_T *pEvent)
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQS_START_CHILD( SKA_MainSchedulerBellSetting );
        break;

    case SE_APP_END:
        EQS_ReleaseDC();
        break;

    case SE_APP_RESUME:
        EQS_END();
        break;

    default:
        EQS_DEF_EVENT_PROC( pEvent );
        break;
    }    
}

/*
static  VOID SKY_API sSKA_SetAlertBell( SKY_EVENT_T *pEvent )
{
    switch( pEvent->EventID )
    {
    case SE_APP_START:
        EQS_GetDC(TRUE);
        EQC_MessageBox( (BYTE *)"알림벨이 설정됐습니다", 3, MBT_NONE, (BYTE *)"Error" );
        break;
    case SE_APP_END:
        EQS_ReleaseDC();
        break;
    case EQE_IO_MESGBOX_OK:
        EQS_END();
        break;
    default:
        EQS_DEF_EVENT_PROC( pEvent );
    }
}
*/