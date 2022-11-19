/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_pbar.c
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
** 2000-00-22 <redstar>       Created.
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
#define DEFAULT_PROGRESS_BAR_STEP     1
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagPROGRESS_BAR_T  PROGRESS_BAR_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagPROGRESS_BAR_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    WORD nMin;
    WORD nMax;
    UINT8 nStep;
    WORD nValue; 
    BOOL  fContinue;
    BOOL  fActive;
	WORD  wMinLimit;
	WORD  wMaxLimit;
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
#define  DRAW_LEVEL_COLOR_NUMBER  5

#ifdef CONFIG_EQS_COLOR_256
COLOR LevelColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB( 0,192,192),
	EQS_RGB(32,160,192),
	EQS_RGB( 0,160,192),
	EQS_RGB(32,128,192),
	EQS_RGB( 0,128,192),
	EQS_RGB(32, 96,192),
	EQS_RGB( 0, 96,192),
	EQS_RGB(32, 64,192),
	EQS_RGB( 0,  0,192),
	EQS_RGB(32,  0,128)

};
#elif CONFIG_EQS_COLOR_64K
/*
COLOR LevelColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(116, 67, 84),
	EQS_RGB(156, 85,111),
	EQS_RGB(188,141,155),
	EQS_RGB(212,182,191),
	EQS_RGB(233,223,215)

};
*/
const COLOR LevelColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(121, 154, 255),
	EQS_RGB( 88, 107, 255),
	EQS_RGB(115,  50, 255),
	EQS_RGB(121,   0, 187),
	EQS_RGB( 82,   0, 116)

};


const COLOR LevelColor2[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(255, 218,  72),
	EQS_RGB(255, 168,  29),
	EQS_RGB(255,  93,  29),
	EQS_RGB(210,  57,   0),
	EQS_RGB(185,   0,   5)

};

#else

#endif

#ifdef CONFIG_EQS_COLOR_256
COLOR LevelFocusColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(224,192,128),
	EQS_RGB(224,160,0),
	EQS_RGB(224,128,64),
	EQS_RGB(224,96,64),
	EQS_RGB(192,64,64),
	EQS_RGB(224,0,64),
	EQS_RGB(192,0,64),
	EQS_RGB(160, 0,64),
	EQS_RGB(128, 0,0),
	EQS_RGB(64, 0, 0)

};
#elif CONFIG_EQS_COLOR_64K
/*
COLOR LevelFocusColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(175, 9, 6),
	EQS_RGB(242, 0, 89),
	EQS_RGB(245, 84,131),
	EQS_RGB(248,147,176),
	EQS_RGB(255,220,193)

};
*/
const COLOR LevelFocusColor[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(121, 154, 255),
	EQS_RGB( 88, 107, 255),
	EQS_RGB(115,  50, 255),
	EQS_RGB(121,   0, 187),
	EQS_RGB( 82,   0, 116)
};


const COLOR LevelFocusColor2[DRAW_LEVEL_COLOR_NUMBER] =
{
	EQS_RGB(255, 218,  72),
	EQS_RGB(255, 168,  29),
	EQS_RGB(255,  93,  29),
	EQS_RGB(210,  57,   0),
	EQS_RGB(185,   0,   5)
};

#else

#endif


LOCAL BOOL             l_fDirty;


/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(WORD nMinA, WORD nMaxA, WORD nValueA);
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

LOCAL void* EQS_API sAllocInstance(WORD nMinA, WORD nMaxA, WORD nValueA)
{
    PROGRESS_BAR_T *pProgressBar;

    /* allocate memory for the instance itself */
    pProgressBar = (PROGRESS_BAR_T*) EQS_Malloc(sizeof(PROGRESS_BAR_T));

    if( nValueA < nMinA || nValueA > nMaxA )
        nValueA = nMinA;
    pProgressBar->nMin      = nMinA;
    pProgressBar->nMax      = nMaxA;
    pProgressBar->nValue    = nValueA;
    pProgressBar->nStep     = DEFAULT_PROGRESS_BAR_STEP;
    pProgressBar->fContinue = FALSE;
    pProgressBar->fActive   = FALSE;
	pProgressBar->wMinLimit = nMinA;
	pProgressBar->wMaxLimit = nMaxA;
    

    /* --> Add here <-- allocate memory for this instance information */
    return (void *)pProgressBar;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);
    

	if( pEvent->EventID == EQE_APP_IO_MODE )
	{
		l_fDirty = EQC_GET_DIRTY(hControl);
	}
    else if(pEvent->EventID == EQE_APP_DRAW)
    {
        /* <-- Add here --> drawing routine */
        if( pProgressBar->fActive == FALSE &&  EQS_IsActive(hControl) == TRUE )
        {
            pProgressBar->fActive = TRUE;
            EQS_SendEvent( EQE_IO_PROGRESS_BAR_ACTIVE, (WORD)hControl, pProgressBar->nValue);
        }

        sDrawInstance(hControl);      
    }
    else if( pEvent->EventID == EQE_KEY_LEFT  )

    {
        if( pProgressBar->nValue > pProgressBar->wMinLimit )
        {
            if( (pProgressBar->nValue - pProgressBar->nStep) < pProgressBar->wMinLimit )
                pProgressBar->nValue  = pProgressBar->wMinLimit;
            else
                pProgressBar->nValue -= pProgressBar->nStep;

			EQC_SET_DIRTY(hControl, TRUE);
            EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA); 
        }
        EQS_SendEvent(EQE_IO_PROGRESS_BAR_DOWN, (WORD)hControl, \
                                                (DWORD) pProgressBar->nValue);
    } 
    else if( pEvent->EventID == EQE_KEY_RIGHT )
    {
        if( pProgressBar->nValue < pProgressBar->wMaxLimit )
        {
            if( (pProgressBar->nValue+ pProgressBar->nStep) > pProgressBar->wMaxLimit )
                pProgressBar->nValue  = pProgressBar->wMaxLimit;
            else
                pProgressBar->nValue += pProgressBar->nStep;

			EQC_SET_DIRTY(hControl, TRUE);
            EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
        }
        EQS_SendEvent(EQE_IO_PROGRESS_BAR_UP, (WORD)hControl, \
                                              (DWORD) pProgressBar->nValue);
    }
    else if( pEvent->EventID == EQE_KEY_CENTER )
    {
        EQS_SendEvent(EQE_IO_PROGRESS_BAR_SELECT,(WORD)hControl, \
                                                 (DWORD) pProgressBar->nValue);
        pProgressBar->fActive = FALSE;
        EQS_EndIO();      
    }
    else if( pEvent->EventID == EQE_KEY_CLEAR)
    {
		//EQC_SET_DIRTY(hControl, l_fDirty);
        EQS_SendEvent(EQE_IO_PROGRESS_BAR_CANCEL,(WORD)hControl, 0);
        pProgressBar->fActive = FALSE;
        EQS_EndIO();      
    }
	else if( pEvent->EventID == EQE_KEY_UP )
	{
		if( EQC_GET_STYLE(hControl) & CS_PROGRESS_BAR_ALONE )
		{			
			;
		}
		else 
		{
			//EQS_SendEvent(EQE_IO_PROGRESS_BAR_CANCEL,(WORD)hControl, EQE_KEY_UP);
			pProgressBar->fActive = FALSE;
			EQS_PassIO(-1);
		}
	}
	else if( pEvent->EventID == EQE_KEY_DOWN )
	{
		if( EQC_GET_STYLE(hControl) & CS_PROGRESS_BAR_ALONE )
		{			
			;
		}
		else
		{
			//EQS_SendEvent(EQE_IO_PROGRESS_BAR_CANCEL,(WORD)hControl, EQE_KEY_DOWN);
			pProgressBar->fActive = FALSE;
			EQS_PassIO(1);
		}
	}
    else if ( pEvent->EventID == EQE_IO_DELETE )
    {
        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_PROGRESS_BAR_CANCEL,(WORD)hControl, EQC_DELETED);

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
    PROGRESS_BAR_T *pProgressBar;
    WORD   nIndex, nCell;
    COORD  StartX, StartY, DX;
    COLOR  ForeColor;
	UINT8  nLevel, nCellWidth, nBoxWidth, nDrawColor, nActiveHeight;


    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    EQS_GetDC(TRUE);

    StartX = EQC_GET_X(hControl);
    StartY = EQC_GET_Y(hControl);

  
	ForeColor = EQS_GetForeColor();

    if( pProgressBar->fContinue )
    { // 楷加利牢   

		EQS_FillRectangle( EQC_GET_X(hControl),
						   EQC_GET_Y(hControl), 
						   EQC_GET_X(hControl)+EQC_GET_DX(hControl),
						   EQC_GET_Y(hControl)+EQC_GET_DY(hControl),
						   EQS_GetBackColor());  
        DX = EQC_GET_DX(hControl) * (pProgressBar->nValue-pProgressBar->nMin)/\
                                    (pProgressBar->nMax-pProgressBar->nMin);
		EQS_SetForeColor(GRAY);
		if( DX > 0 )
		{
			EQS_FillRectangle( EQC_GET_X(hControl),
							   EQC_GET_Y(hControl), 
							   EQC_GET_X(hControl)+DX,
							   EQC_GET_Y(hControl)+EQC_GET_DY(hControl),
							   EQS_GetForeColor()); 
		}
		
	}
	else
	{ // 窜加利牢 

		nCell  = (pProgressBar->nMax - pProgressBar->nMin) / pProgressBar->nStep;
		if( nCell == 0 )
		{
			pProgressBar->nStep = 1;
			nCell = 1;
		}
		nCellWidth = (EQC_GET_DX(hControl)) / nCell;
		nBoxWidth  = nCellWidth;

														        
		if( nCell*nCellWidth !=  EQC_GET_DX(hControl) )
            EQC_SET_DX(hControl, nCell*nCellWidth);

		EQS_FillRectangle( EQC_GET_X(hControl),
						   EQC_GET_Y(hControl), 
						   EQC_GET_X(hControl)+EQC_GET_DX(hControl),
						   EQC_GET_Y(hControl)+EQC_GET_DY(hControl),
						   EQS_GetBackColor());  

		nLevel = (pProgressBar->nValue-pProgressBar->nMin) / pProgressBar->nStep ;        

		for( nIndex = 1; nIndex <= nLevel; nIndex++)
		{																			
			nDrawColor =  ((nIndex-1) * DRAW_LEVEL_COLOR_NUMBER ) / nCell;
			if( nDrawColor >= DRAW_LEVEL_COLOR_NUMBER )
				nDrawColor = DRAW_LEVEL_COLOR_NUMBER - 1;							
			if( EQS_HaveFocus(hControl) )
			{
				if( g_GUI_Theme.ColorScheme < 3 )
				{			
					EQS_SetForeColor(LevelFocusColor[nDrawColor]);
				}
				else
				{
					EQS_SetForeColor(LevelFocusColor2[nDrawColor]);
				}
			}
			else
			{
				if( g_GUI_Theme.ColorScheme < 3 )
				{
					EQS_SetForeColor(LevelColor[nDrawColor]);
				}
				else
				{
					EQS_SetForeColor(LevelColor2[nDrawColor]);
				}
			}

			EQS_FillRectangle( StartX +(nIndex-1) * nCellWidth,\
							   StartY,\
							   StartX +(nIndex-1) * nCellWidth + nBoxWidth -1,\
							   StartY + EQC_GET_DY(hControl)-1,\
							   EQS_GetForeColor());
/*
			EQS_SetForeColor(R_BLACK);
			
			EQS_Line(StartX +(nIndex-1) * nCellWidth + nBoxWidth -1, StartY,
				     StartX +(nIndex-1) * nCellWidth + nBoxWidth -1,
					 StartY + EQC_GET_DY(hControl)-1 );
*/

		}
		if( EQS_HaveFocus(hControl) )
		{
			EQC_DrawBorder(hControl,\
						   EQC_GET_X(hControl)+1 , EQC_GET_Y(hControl)+1, \
						   EQC_GET_X(hControl)+EQC_GET_DX(hControl)-2,    \
						   EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-2     \
						  );
		}
	}   

	EQC_DrawBorder(hControl,\
		           EQC_GET_X(hControl) , EQC_GET_Y(hControl), \
				   EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,    \
				   EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1     \
				  );

    EQS_SetForeColor(ForeColor);
   
    EQS_ReleaseDC();
    EQS_UpdateControlDisplay(); 
}

HCONTROL EQS_API EQC_ProgressBar(COORD x, COORD y, UINT8 nWidth, UINT8 nHeight,\
                                 WORD nMinA, WORD nMaxA,WORD nInitValue, \
                                 BOOL  fContinue)
{
    PROGRESS_BAR_T *pProgressBar;
    void           *pInstance;
    HCONTROL        hControl;
    COORD           dx, dy;

    if( nInitValue > nMaxA )
        nInitValue = nMaxA;
    if( nMinA == nMaxA )
    {
        nMaxA = nMinA +1;
    }
    pInstance = sAllocInstance(nMinA, nMaxA, nInitValue);

    dx   = nWidth;
    if( nHeight )
        dy   = nHeight;
    else
        dy   = EQS_CharHeight((BYTE)' ');


    hControl = EQC_AllocControl(CT_PROGRESS_BAR,  pInstance, x, y, dx, dy, \
                                sHandleEvent );

    ASSERT(hControl != INVALID_CONTROL_HANDLE);
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);
    pProgressBar->fContinue = fContinue;

    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);

    return hControl;
}

void EQS_API EQC_SetProgressBar(HCONTROL hControl, WORD nValueA)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    pProgressBar->nValue = nValueA;
	EQC_SET_DIRTY(hControl, TRUE);

    EQS_SendEvent(EQE_APP_DRAW,(WORD)hControl,0);

}

/* <-- Add here --> */
WORD EQS_API EQC_GetProgressBar(HCONTROL hControl)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    return pProgressBar->nValue;
}


void EQS_API EQC_SetProgressBarStep(HCONTROL hControl, UINT8 nStep)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    pProgressBar->nStep = nStep;
  
}


UINT8 EQS_API EQC_GetProgressBarStep(HCONTROL hControl)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    return pProgressBar->nStep;
  
}

void  EQS_API EQC_SetProgressBarLimit(HCONTROL hControl, WORD  wMin,  WORD  wMax)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    pProgressBar->wMinLimit = wMin;
	pProgressBar->wMaxLimit = wMax;

}
void  EQS_API EQC_GetProgressBarLimit(HCONTROL hControl, WORD* pwMin, WORD* pwMax)
{
    PROGRESS_BAR_T *pProgressBar;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_PROGRESS_BAR);

    /* get the pointer of teh control instance */
    pProgressBar = (PROGRESS_BAR_T *)EQC_GET_INSTANCE(hControl);

    *pwMin = pProgressBar->wMinLimit;
	*pwMax = pProgressBar->wMaxLimit;

}

