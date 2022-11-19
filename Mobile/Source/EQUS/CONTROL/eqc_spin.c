/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_vbar.c
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
** 2000-10-24 redstar       Created.
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
typedef struct tagSPIN_BAR_T  SPIN_BAR_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagSPIN_BAR_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
	INT16  wMin;
	INT16  wMax;
    INT16  wStep;
    INT16  wValue;   
    
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
LOCAL BOOL             l_fDirty;
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(void);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawInstance(HCONTROL hControl);


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

LOCAL void* EQS_API sAllocInstance(void)
{
    SPIN_BAR_T *pSpin;

    /* allocate memory for the instance itself */
    pSpin = (SPIN_BAR_T*)EQS_Malloc(sizeof(SPIN_BAR_T));

    /* --> Add here <-- allocate memory for this instance information */

    return (void *)pSpin;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    SPIN_BAR_T *pSpin;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_SPIN);

    /* get the pointer of teh control instance */
    pSpin = (SPIN_BAR_T *)EQC_GET_INSTANCE(hControl);


	if( pEvent->EventID == EQE_APP_IO_MODE )
	{
		l_fDirty = EQC_GET_DIRTY(hControl);
		EQC_SET_DIRTY(hControl, TRUE);
	}    
    else if(pEvent->EventID == EQE_APP_DRAW)
    {
        /* <-- Add here --> drawing routine */
        sDrawInstance(hControl);       
    }
    else if( pEvent->EventID == EQE_KEY_UP )
    {
        if(  (pSpin->wValue + pSpin->wStep) <= pSpin->wMax )
        {
             pSpin->wValue += pSpin->wStep ;  
             EQS_SendEvent(EQE_IO_SPIN_CHG,(WORD)hControl, pSpin->wValue);
             EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
        }
        
    }
    else if( pEvent->EventID == EQE_KEY_DOWN )
    {
        if(  (pSpin->wValue - pSpin->wStep ) >= pSpin->wMin )
        {			 
             pSpin->wValue -= pSpin->wStep; 
             EQS_SendEvent(EQE_IO_SPIN_CHG,(WORD)hControl, pSpin->wValue);
             EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
        }        
    }
    else if( pEvent->EventID == EQE_KEY_CENTER )
    {
        EQS_SendEvent(EQE_IO_SPIN_SELECT, (WORD)hControl, (DWORD) pSpin->wValue);
        EQS_EndIO();
    }
    else if( pEvent->EventID == EQE_KEY_CLEAR )
    {
		EQC_SET_DIRTY(hControl, l_fDirty);
        EQS_SendEvent(EQE_IO_SPIN_CANCEL, (WORD)hControl, 0);
        EQS_EndIO();
    }
    else if (pEvent->EventID == EQE_IO_DELETE)
    {
        // Send ending event you want..
		if( pEvent->wParam == hControl )					
			EQS_SendEvent(EQE_IO_SPIN_CANCEL, (WORD)hControl, EQC_DELETED);
        // Free instance
        sFreeInstance(hControl);
    }
    else ;
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

LOCAL  void EQS_API sDrawInstance(HCONTROL hControl)
{
    SPIN_BAR_T *pSpin;
    /* <-- Add here --> the local variable for handling the event */
    INT16   wDigit, wValue;
    INT16   wMaxDigit, wValueDigit;
    UINT8   nWidth, nLen, i,j;
    BYTE    szStr[13];
    BYTE    szValue[14];


    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_SPIN);

    /* get the pointer of teh control instance */
    pSpin = (SPIN_BAR_T *)EQC_GET_INSTANCE(hControl);

    EQS_GetDC(TRUE);

#if 0
	if( EQC_GET_FOCUS(hControl) == FALSE )
	{
		EQS_SetTextColor(CTRL_DEACTIVE_COLOR);
	}
#endif


    wMaxDigit = 1;
    if( pSpin->wMax < 0 )
        wDigit = -pSpin->wMax / 10;
    else
        wDigit = pSpin->wMax/10;

    if( wDigit > 0 )
        wMaxDigit++;
    while(wDigit > 9)
    {
        wMaxDigit++;
        wDigit = wDigit/10;
    }

    wValueDigit = 1;    
    if( pSpin->wValue < 0 )
        wDigit = -pSpin->wValue / 10;
    else
        wDigit = pSpin->wValue/10;

    if( wDigit > 0 )
        wValueDigit++;
    while( wDigit > 9 )
    {
        wValueDigit++;
        wDigit = wDigit/10;
    }
         
    for( i = 0; i < (wMaxDigit-wValueDigit); i++)
    {
        if( pSpin->Header.Style & CS_SPIN_LEADING_ZERO )
        {
            szStr[i] = (BYTE)'0';
        }
        else
        {
            szStr[i] =(BYTE)' ';
        }
    }

    if( pSpin->wValue < 0 )
        wDigit = -pSpin->wValue;
    else
        wDigit = pSpin->wValue;
    for( j = 0; j < wValueDigit; j++)
    {
        wValue = wDigit % 10; 
        wDigit = wDigit / 10;               
        szStr[i+wValueDigit-1-j] = (BYTE)('0'+wValue);
    }
    szStr[i+wValueDigit] = 0;

    nWidth  = EQS_CharWidth((BYTE)' ');
    /*
    EQS_TextOutStr( pSpin->Header.x,
			       pSpin->Header.y+1, 
                   (BYTE*)"\205\355");
    */
    STRCAT(szStr,(BYTE*)SF_COMBO_BOX);
    if( pSpin->wValue < 0 )
    {
        szValue[0] = '-';
        szValue[1] = 0;             
    }
    else if (pSpin->wValue == 0 )
    {
        szValue[0] = ' ';
        szValue[1] = 0;
    }
    else
    {
        szValue[0] = '+';
        szValue[1] = 0;        
    }
    STRCAT(szValue,szStr);
    nLen = STRLEN( szValue );


	EQS_TextOutStr((COORD)(pSpin->Header.x), (COORD)(pSpin->Header.y), szValue);
  
    if( EQC_GET_STYLE( hControl ) &  CS_SPIN_BORDER)
    {
        EQC_DrawBorder(hControl,\
			           EQC_GET_X(hControl)-1,                      \
                       EQC_GET_Y(hControl)-1,                      \
                       EQC_GET_X(hControl)+EQC_GET_DX(hControl),   \
                       EQC_GET_Y(hControl)+EQC_GET_DY(hControl)    \
                      );        
    } 
   
    EQS_ReleaseDC();
    EQS_UpdateControlDisplay();
   
}


HCONTROL EQS_API EQC_Spin(COORD x, COORD y, INT16 MinA, INT16 MaxA, INT16 StepA, INT16 InitValue)
{
    void *pInstance;
    HCONTROL hControl;
    SPIN_BAR_T* pSpin;
    COORD dx, dy;
    INT16  Digit , MaxDigit, MaxValue;
    /* <-- Add here --> local variables */

    /* allocation memory for this instance */
    pInstance = sAllocInstance();

    MaxDigit = 1;

    if(  MinA < 0 )
    {
        MaxDigit++;
        MaxValue = -MinA;
        if( MaxA > MaxValue )
            MaxValue = MaxA;
    }
    else
        MaxValue = MaxA;

    
    
    Digit = MaxValue/10;
    if( Digit > 0 )
        MaxDigit++;
    while( Digit > 9)
    {
        MaxDigit++;
        Digit = Digit/10;
    }
   
    /* get dx, dy */
    dx = EQS_CharWidth((BYTE)' ')*( MaxDigit + 3 );
    dy = EQS_CharHeight((BYTE)' ');

/* Original - woodstok 01.1.8    
    hControl = EQC_AllocControl(CT_SPIN,  pInstance, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
    hControl = EQC_AllocControl(CT_SPIN,  pInstance, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    /* <-- Add here --> set the control init value */
    pSpin = (SPIN_BAR_T*) pInstance;
    pSpin->wMin    = MinA;
    pSpin->wMax    = MaxA;
    pSpin->wStep   = StepA;
    pSpin->wValue  = InitValue;

    if( pSpin->wMin > pSpin->wMax )
    {
        pSpin->wMin = MaxA;
        pSpin->wMax = MinA;
    }
    if( StepA > ( pSpin->wMax - pSpin->wMin ) )
    {
        pSpin->wStep = 1;
    }
    if( InitValue > pSpin->wMax )
        pSpin->wValue = pSpin->wMin;

    EQC_SET_STYLE( hControl , CS_SPIN_DEFAULT );

    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);

    return hControl;
}

void EQS_API EQC_SetSpin(HCONTROL hControl, INT16 SetValue)
{
    SPIN_BAR_T *pSpin;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_SPIN);

    /* get the pointer of teh control instance */
    pSpin = (SPIN_BAR_T *)EQC_GET_INSTANCE(hControl);
    
    pSpin->wValue = SetValue;
    EQS_SendEvent(EQE_IO_SPIN_CHG,(WORD)hControl, pSpin->wValue);
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);
}


INT16 EQS_API EQC_GetSpin(HCONTROL hControl)
{
    SPIN_BAR_T *pSpin;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_SPIN);

    /* get the pointer of teh control instance */
    pSpin = (SPIN_BAR_T *)EQC_GET_INSTANCE(hControl);

    return pSpin->wValue;
}


