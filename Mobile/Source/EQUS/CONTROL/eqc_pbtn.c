/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_pbtn.c
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
** 2000-10-25 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
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
typedef struct tagPUSH_BUTTON_T  PUSH_BUTTON_T;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagPUSH_BUTTON_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    BYTE    szText[40];
	UINT8   nMaxLen;
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
LOCAL void* EQS_API sAllocInstance(BYTE* pszStr);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);

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

LOCAL void* EQS_API sAllocInstance(BYTE* szBuf)
{

	PUSH_BUTTON_T * pPushButton;
     
    /* allocate memory for the instance itself */
    pPushButton = (PUSH_BUTTON_T*) EQS_Malloc(sizeof(PUSH_BUTTON_T));
    /* --> Add here <-- allocate memory for this instance information */
	STRCPY(pPushButton->szText, szBuf);
	pPushButton->nMaxLen = STRLEN(szBuf);
    return (void *)pPushButton;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    PUSH_BUTTON_T *pPushButton;
	UINT8  StartX, StartY;
	UINT8  nLen;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PUSH_BUTTON);
    
    /* get the pointer of teh control instance */
    pPushButton = (PUSH_BUTTON_T *)EQC_GET_INSTANCE(hControl);
    
	if( pEvent->EventID == EQE_APP_IO_MODE )
	{
        EQS_SendEvent(EQE_IO_PUSH_BUTTON_SELECT,(WORD)hControl, EQS_NA);
        EQS_EndIO();
	}
    else if(pEvent->EventID == EQE_APP_DRAW)
    {
        /* <-- Add here --> drawing routine */
		EQS_GetDC(TRUE);		

		nLen = STRLEN( pPushButton->szText );
		if( EQC_GET_STYLE(hControl) & CS_PUSH_BUTTON_CENTER )
		{
			if( pPushButton->nMaxLen > nLen )
			{
				StartX = EQC_GET_X(hControl)+\
					     (EQC_GET_DX(hControl) - EQS_TextWidth(pPushButton->szText))/2;
			}
			else
			{
				StartX = EQC_GET_X(hControl);
				nLen = pPushButton->nMaxLen;
			}
		}
		else
		{
			StartX = EQC_GET_X(hControl);
		}
		StartY = EQC_GET_Y(hControl);

		if( IsMiddleOfMultiByte(pPushButton->szText,nLen) ) 
		{
			nLen--;
		}

		EQS_FillRectangle(EQC_GET_X(hControl),\
			      EQC_GET_Y(hControl),\
				  EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
				  EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1,\
				  EQS_GetTextBackColor());
		EQS_TextOutStrLenTDM(StartX, StartY, pPushButton->szText, nLen, TDM_NORMAL);
		StartX = EQC_GET_X(hControl);
		EQS_Draw3DBox( StartX-1, StartY-1,\
				       StartX + EQC_GET_DX(hControl),\
					   StartY+EQC_GET_DY(hControl));
		EQS_ReleaseDC();
		EQS_UpdateControlDisplay();
    }
    else if ( pEvent->EventID == EQE_IO_DELETE )
    {
        // Send ending event you want..
        // Free instance
        sFreeInstance(hControl);
    }

}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);
    /* free the memory that was allocated */
    /* <-- Add here --> */
    
    /* free the instance itself */
    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

HCONTROL EQS_API EQC_PushButton(COORD x, COORD y, WORD Caption)
{
    PUSH_BUTTON_T *pPushButton;
    HCONTROL hControl;
    COORD dx, dy;
	BYTE* pszStr;


	pszStr = EQS_GET_TOKEN_EX(Caption);
    /* <-- Add here --> local variables */
    pPushButton = (PUSH_BUTTON_T *)sAllocInstance(pszStr);

	dx = EQS_TextWidth(pszStr);
	dy = EQS_TextHeight(pszStr);
/* Original - woodstok 01.1.8    
    hControl = EQC_AllocControl(CT_PUSH_BUTTON,  pPushButton, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
    hControl = EQC_AllocControl(CT_PUSH_BUTTON,  pPushButton, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT(hControl != INVALID_CONTROL_HANDLE);
    
    /* <-- Add here --> set the control init value */
    EQC_SET_STYLE(hControl, CS_PUSH_BUTTON_DEFAULT);
    
    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
    
    return hControl;
}

void EQS_API EQC_SetPushButtonCaption(HCONTROL hControl, BYTE* szString, UINT8 nWidth)
{
	PUSH_BUTTON_T *pPushButton;

	ASSERT( EQC_GET_TYPE(hControl) == CT_PUSH_BUTTON);

    pPushButton = (PUSH_BUTTON_T *) EQC_GET_INSTANCE(hControl);
    
	STRCPY(pPushButton->szText, szString);

    /* get dx, dy */
	if( nWidth == 0 )
		EQC_SET_DX(hControl,EQS_TextWidth(szString));
	else
	{
		EQC_SET_DX(hControl,EQS_CharWidth((BYTE)' ')*nWidth);
		pPushButton->nMaxLen = nWidth;
	}
    EQC_SET_DY(hControl,EQS_TextHeight(szString));

    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);     
}

BYTE * EQS_API EQC_GetPushButtonCaption(HCONTROL hControl)
{
	PUSH_BUTTON_T *pPushButton;

	ASSERT( EQC_GET_TYPE(hControl) == CT_PUSH_BUTTON);

    pPushButton = (PUSH_BUTTON_T *) EQC_GET_INSTANCE(hControl);

    return pPushButton->szText;
}

