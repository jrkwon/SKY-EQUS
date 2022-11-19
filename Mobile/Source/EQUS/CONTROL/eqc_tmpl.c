/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_xxx.c
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
** 2000-MM-DD <id>       Created.
**
** ===========================================================================
*/

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
typedef struct tagXXX_YYY_T  XXX_YYY_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagXXX_YYY_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    
};

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
LOCAL void* EQS_API sAllocInstance(/* --> Add here <-- the information */);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(void *pInstance);

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

LOCAL void* EQS_API sAllocInstance(/* --> Add here <-- the information */)
{
    XXX_YYY_T *pXxxYyy;

    /* allocate memory for the instance itself */
    pXxxYyy = EQS_Malloc(sizeof(XXX_YYY_T));

    /* --> Add here <-- allocate memory for this instance information */

    return (void *)pXxxYyy;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    XXX_YYY_T *pXxxYyy;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_XXX_YYY);

    /* get the pointer of teh control instance */
    pXxxYyy = (XXX_YYY_Y *)EQC_GET_INSTANCE(hControl);
    
    if(pEvent->EventID == EQE_APP_DRAW)
    {
        /* <-- Add here --> drawing routine */

        EQS_UpdateDisplay();
    }
    else if (pEvent->EventID == EQE_IO_DELETE)
    {
        /* <-- Add here --> Send ending event you want */


        // Free instance
        sFreeInstance(hControl);
    }
}

LOCAL void EQS_API sFreeInstance(void *pInstance)
{
    /* free the memory that was allocated */
    /* <-- Add here --> */

    /* free the instance itself */
    EQS_Free(&pInstance);
}

HCONTROL EQS_API EQC_XxxYyy(COORD x, COORD y, BYTE *pszStr)
{
    void *pInstance;
    HCONTROL hControl;
    COORD dx, dy;
    /* <-- Add here --> local variables */

    /* allocation memory for this instance */
    pInstance = sAllocInstance(/* <-- Add here --> */);

    /* get dx, dy */
    dx = /* <-- Add here --> */
    dy = /* <-- Add here --> */

/* Original - woodstok 01.1.8
    hControl = EQC_AllocControl(CT_STATIC_TEXT,  pInstance, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
    hControl = EQC_AllocControl(CT_STATIC_TEXT,  pInstance, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    /* <-- Add here --> set the control init value */

    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);

    return hControl;
}

void EQS_API EQC_SetXxxYyy(HCONTROL hControl, /* <-- Add here --> */)
{
}

/* <-- Add here --> */ EQS_API EQC_GetXxxYyy(HCONTROL hControl)
{
    return /* <-- Add here --> */
}

