/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_ckbx.c
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
** 2000-10-20  redstar   
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

#define NORMAL_CHECK_BOX_WIDTH  10
#define NORMAL_CHECK_BOX_HEIGHT 10

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagCHECK_BOX_T  CHECK_BOX_T;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
enum  tagCHECKBOX_DRAW_TYPE_E
{
	CHK_DRAW,
	CHK_NORMAL,
	CHK_ONE,
	CHK_SCROLL,
	CHK_REDRAW
};
typedef enum tagCHECKBOX_DRAW_TYPE_E  CHECKBOX_DRAW_TYPE_E;

struct tagCHECK_BOX_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    UINT8            nSize;
    UINT8            nIndex;
	UINT8            nLen;
    BYTE             **ppszString;
    BOOL             *pafCheck;
	UINT8            nPreviousIndex;
	COLOR            BackColor;
	COLOR            ForeColor;

	CHECKBOX_DRAW_TYPE_E DrawType;    
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
LOCAL void EQS_API sDrawCheckBox(HCONTROL hControl,UINT8 nPos, BOOL fReverse);
LOCAL void* EQS_API sAllocInstance(BYTE **paszStr, UINT8 nSizeA);
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

LOCAL void* EQS_API sAllocInstance(BYTE **paszStr, UINT8 nSizeA)
{
    CHECK_BOX_T *pCheckBox;
    UINT8  i;

    /* allocate memory for the instance itself */
    pCheckBox = (CHECK_BOX_T*) EQS_Malloc(sizeof(CHECK_BOX_T));

    pCheckBox->nSize  = nSizeA;
    pCheckBox->nIndex = 0; 

    /* --> Add here <-- allocate memory for this instance information */
    pCheckBox->pafCheck   = (BOOL*)  EQS_Malloc((SIZE_T)(sizeof(BOOL)*nSizeA));
    pCheckBox->ppszString = (BYTE**) EQS_Malloc((SIZE_T)(sizeof(BYTE*)*nSizeA));
	pCheckBox->nPreviousIndex = 0;
	pCheckBox->DrawType   = CHK_DRAW;
	pCheckBox->BackColor  = MAX_COLOR;
	pCheckBox->ForeColor  = MAX_COLOR;
    for( i=0; i< nSizeA; i++)
    {
        pCheckBox->pafCheck[i]   = FALSE;
        pCheckBox->ppszString[i] = paszStr[i];
    }

    return (void *)pCheckBox;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */ 
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);
    
	if(pEvent->EventID ==  EQE_APP_IO_MODE )
	{		
		pCheckBox->DrawType = CHK_DRAW;
	}
    else if(pEvent->EventID == EQE_APP_DRAW)
    {
		if( pEvent->wParam == EQS_NA )
		{
			pCheckBox->DrawType = CHK_DRAW;
		}
        sDrawInstance(hControl);
    }
    else if( pEvent->EventID == EQE_KEY_UP )
    {
        if( pCheckBox->nIndex > 0 )
		{
			pCheckBox->DrawType = CHK_NORMAL;
			pCheckBox->nPreviousIndex = pCheckBox->nIndex;
            pCheckBox->nIndex--;
		}
        else
        {
			pCheckBox->DrawType = CHK_REDRAW;
            EQS_SendEvent(EQE_IO_CHECK_BOX_UP, (WORD)hControl, EQS_NA);
            EQS_EndIO();
        }
		if( pCheckBox->pafCheck[pCheckBox->nIndex] )
		{
			EQS_DrawIOCCML(IOC_CML_UNSETUP_CENTER);
		}
		else
		{
			EQS_DrawIOCCML(IOC_CML_SETUP_CENTER);
		}			
		EQS_UpdateIOCCML();
        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA);
    }
    else if( pEvent->EventID == EQE_KEY_DOWN )
    {
        if( pCheckBox->nIndex < (pCheckBox->nSize-1) )
		{
			pCheckBox->DrawType = CHK_NORMAL;
			pCheckBox->nPreviousIndex = pCheckBox->nIndex;
            pCheckBox->nIndex++;
		}
        else
        {
			pCheckBox->DrawType = CHK_REDRAW;
            EQS_SendEvent( EQE_IO_CHECK_BOX_DOWN , (WORD)hControl, 0 );
            EQS_EndIO();
        }

		if( pCheckBox->pafCheck[pCheckBox->nIndex] )
		{
			EQS_DrawIOCCML(IOC_CML_UNSETUP_CENTER);
		}
		else
		{
			EQS_DrawIOCCML(IOC_CML_SETUP_CENTER);
		}			
		EQS_UpdateIOCCML();		

        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA);
    }
    else if( pEvent->EventID == EQE_KEY_CENTER )
    {

		EQC_SET_DIRTY(hControl, TRUE);
        pCheckBox->pafCheck[ pCheckBox->nIndex ] = !pCheckBox->pafCheck[ pCheckBox->nIndex ];
        if( pCheckBox->pafCheck[ pCheckBox->nIndex ] == TRUE )
        {
            EQS_SendEvent( EQE_IO_CHECK_BOX_CHECK , (WORD)hControl, (DWORD)pCheckBox->nIndex );
        }
        else
        {
            EQS_SendEvent( EQE_IO_CHECK_BOX_UNCHECK , (WORD)hControl, (DWORD)pCheckBox->nIndex );
        }
        
		if( pCheckBox->nSize == 1 )
		{
			pCheckBox->DrawType = CHK_REDRAW;
			EQS_EndIO();
		}
		else
		{
			pCheckBox->DrawType = CHK_ONE;
			EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);
		}
    }
    else if( pEvent->EventID == EQE_KEY_CLEAR)
    {
		pCheckBox->DrawType = CHK_REDRAW;
        EQS_SendEvent( EQE_IO_CHECK_BOX_CANCEL , (WORD)hControl, 0 );
        EQS_EndIO();
    }
    else if (pEvent->EventID == EQE_IO_DELETE)
    {
        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent( EQE_IO_CHECK_BOX_CANCEL , (WORD)hControl, EQC_DELETED );
        // Free instance
        sFreeInstance(hControl);
    }
    else 
        ;
}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
    CHECK_BOX_T *pCheckBox;
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);
    /* free the memory that was allocated */
    /* <-- Add here --> */
    EQS_Free((void**)&(pCheckBox->pafCheck));
    EQS_Free((void**)&(pCheckBox->ppszString));

    /* free the instance itself */
    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL  void EQS_API sDrawCheckBox(HCONTROL hControl,UINT8 nPos, BOOL fReverse)
{
    CHECK_BOX_T *pCheckBox;
    UINT8  nHeight, nWidth;
	BYTE   szBuf[22];
    COLOR OldTextBackColor, OldTextColor;
    
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    nHeight = EQS_CharHeight((BYTE)' ');
    nWidth  = EQS_CharHeight((BYTE)' ');


	OldTextBackColor = EQS_GetTextBackColor();
	OldTextColor = EQS_GetTextColor();

	if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_LIST )
	{
		if( fReverse && EQS_IsActive(hControl) )
		{							
			EQS_SetTextBackColor(SELECT_BACK_COLOR );
			EQS_SetTextColor(SELECT_FORE_COLOR);		
		}
		else
		{
			EQS_SetTextBackColor(TEXT_BACK_COLOR );
			EQS_SetTextColor(TEXT_FORE_COLOR);	
		}
	}
	else
	{
		if( fReverse &&  EQS_IsActive(hControl) )
		{							
			EQS_SetTextBackColor(EQS_CURSOR_BACK_COLOR);
			EQS_SetTextColor(EQS_CURSOR_FORE_COLOR);	
		}
	}

    if( pCheckBox->pafCheck[nPos] == TRUE )
    {			
		STRCPY(szBuf, (BYTE*) SF_CHECK_BOX);
    } 
    else
    {			
		STRCPY(szBuf, (BYTE*) SF_BOX); 
    }

	STRCAT(szBuf, (BYTE*)((pCheckBox->ppszString)[nPos]));
    EQS_TextOutStrLen(EQC_GET_X(hControl),
		              EQC_GET_Y(hControl) + nPos * nHeight,
                      szBuf, pCheckBox->nLen);		        
		
		
	EQS_SetTextBackColor(OldTextBackColor);					
	EQS_SetTextColor(OldTextColor);		
	
}

LOCAL  void EQS_API sDrawInstance(HCONTROL hControl)
{

    CHECK_BOX_T *pCheckBox;
    UINT8  i, nHeight;
	RECT   Rect;

    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    EQS_GetDC(TRUE);

	Rect.xLeft   = EQC_GET_X(hControl);
	Rect.xRight  = EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1;
	Rect.yTop    = EQC_GET_Y(hControl);
	Rect.yBottom = EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1;
	nHeight = EQS_CharHeight((BYTE)' ');

#if 0
	if( EQC_GET_FOCUS(hControl) == FALSE )
	{
		EQS_SetTextColor(CTRL_DEACTIVE_COLOR);
	}
#endif

	if( pCheckBox->DrawType == CHK_NORMAL )
	{
		sDrawCheckBox(hControl, pCheckBox->nPreviousIndex, FALSE);
		sDrawCheckBox(hControl, pCheckBox->nIndex, TRUE);
		if( pCheckBox->nPreviousIndex < pCheckBox->nIndex )
		{
			Rect.yTop += pCheckBox->nPreviousIndex*nHeight;
		}
		else
		{
			Rect.yTop += pCheckBox->nIndex*nHeight;
		}
		Rect.yBottom = Rect.yTop + (nHeight<<1);
		EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
	}
	else if( pCheckBox->DrawType == CHK_ONE)
	{
		sDrawCheckBox(hControl, pCheckBox->nIndex, TRUE);
		EQS_RefreshDisplayRect(hControl,Rect.xLeft, Rect.yTop+pCheckBox->nIndex*nHeight,\
			                   Rect.xRight,Rect.yTop+(pCheckBox->nIndex+1)*nHeight);
	}
#if 0	
	else if( pCheckBox->DrawType == CHK_REDRAW )
	{
		sDrawCheckBox(hControl, pCheckBox->nIndex, TRUE);
		EQS_RefreshDisplayRect(hControl,Rect.xLeft, Rect.yTop+pCheckBox->nIndex*nHeight,\
			                   Rect.xRight,Rect.yTop+(pCheckBox->nIndex+1)*nHeight);
	}
#endif
	else if( pCheckBox->DrawType == CHK_REDRAW || pCheckBox->DrawType == CHK_DRAW )
	{
		for (i = 0; i < pCheckBox->nSize; i++)
		{   
			if( i == pCheckBox->nIndex && pCheckBox->nSize != 1 )
			{
				sDrawCheckBox(hControl, i, TRUE);
			}
			else
			{
				sDrawCheckBox(hControl, i, FALSE);
			}
		}
	}
	else ;


	if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_BORDER && pCheckBox->DrawType == CHK_DRAW )
	{
        EQC_DrawBorder(hControl, EQC_GET_X(hControl) , EQC_GET_Y(hControl),
                       (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
                       (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1));
	}

	if( EQS_HaveFocus(hControl) && (pCheckBox->DrawType == CHK_DRAW || pCheckBox->DrawType == CHK_ONE) )
	{
		EQS_EVENT CurrentEvent;
		if( pCheckBox->pafCheck[pCheckBox->nIndex] )
		{
			EQS_DrawIOCCML(IOC_CML_UNSETUP_CENTER);
		}
		else
		{
			EQS_DrawIOCCML(IOC_CML_SETUP_CENTER);
		}
		CurrentEvent = EQS_GetEvent();
		if( CurrentEvent.wParam == hControl )
		{
			EQS_UpdateIOCCML();
		}
	}
		
    EQS_ReleaseDC();

	if( pCheckBox->DrawType == CHK_DRAW || pCheckBox->DrawType == CHK_ONE)
	{
		EQS_UpdateControlDisplay();
	}

	pCheckBox->DrawType = CHK_DRAW;
}


HCONTROL EQS_API EQC_CheckBox(COORD x, COORD y, BYTE **paszStr, UINT8 nSizeA)
{
	CHECK_BOX_T *pCheckBox;
    void *pInstance;
    HCONTROL hControl;
    COORD dx, dy;
    UINT8 i, nChar, nSize;
    /* <-- Add here --> local variables    */

    /* allocation memory for this instance */
    pInstance = sAllocInstance(paszStr, nSizeA);

    /* get dx, dy */
    nChar = 0;
    for(i=0; i<nSizeA; i++)
    {
        nSize = (UINT8)STRLEN( paszStr[i] );
        if( nSize > nChar )
            nChar = nSize;
    }

    dx = EQS_CharWidth((BYTE)' ') * (nChar+2);
    dy = EQS_CharHeight((BYTE)' ')* nSizeA;        

/* Original - woodstok 01.1.8
    hControl = EQC_AllocControl(CT_CHECK_BOX,  pInstance, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/                                
    hControl = EQC_AllocControl(CT_CHECK_BOX,  pInstance, x, y, dx, dy, 
                                sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

	pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);
	pCheckBox->nLen = nChar+2;
	EQC_SET_STYLE(hControl, CS_CHECK_BOX_DEFAULT);

    /* <-- Add here --> set the control init value */

    /* send draw event to the state machine engine */
    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);

    return hControl;
}

void        EQS_API EQC_SetCheckBox(HCONTROL hControl,   UINT8 nPos)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    pCheckBox->pafCheck[nPos] = TRUE;

    EQS_SendEvent( EQE_APP_DRAW, 0, 0);

}

void        EQS_API EQC_ResetCheckBox(HCONTROL hControl, UINT8 nPos)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    pCheckBox->pafCheck[nPos] = FALSE;

    EQS_SendEvent( EQE_APP_DRAW, 0, 0);
}

/* <-- Add here --> */ 
UINT8       EQS_API EQC_GetCheckBoxSize(HCONTROL hControl)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    return pCheckBox->nSize;

}

BOOL        EQS_API EQC_GetCheckBox(HCONTROL hControl, UINT8 nPos)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    return pCheckBox->pafCheck[nPos];
}



void        EQS_API EQC_SetCheckBoxIndex(HCONTROL hControl, UINT8 nPos)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    if( nPos < pCheckBox->nSize )
    {
        pCheckBox->nIndex = nPos;
        EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, EQS_NA );
    }

}

UINT8        EQS_API EQC_GetCheckBoxIndex(HCONTROL hControl)
{
    CHECK_BOX_T *pCheckBox;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);

    /* get the pointer of teh control instance */
    pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

    return pCheckBox->nIndex;
}


UINT8 EQS_API EQC_GetCheckBoxDisplayLine(COORD StartPosY)
{
    UINT8  nLines;
    RECT   WindowRect;
    COORD  DY;    
    
    EQS_GetSWindowViewAreaRect(&WindowRect);

    DY = WindowRect.yBottom - WindowRect.yTop - StartPosY -2;
  
    nLines =  DY / EQS_CharHeight((BYTE)' ');

    return nLines;
}

void  EQS_API EQC_SetCheckBoxStyle(HCONTROL hControl, WORD StyleA)
{
    RECT   WindowRect;
    CHECK_BOX_T *pCheckBox;
	UINT8  nMargin ;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_CHECK_BOX);
	pCheckBox = (CHECK_BOX_T *)EQC_GET_INSTANCE(hControl);

	EQC_GET_INSTANCE(hControl)->Style = (StyleA);

    EQS_GetSWindowViewAreaRect(&WindowRect);

	if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_BORDER )
		nMargin = 2;
	else
		nMargin = 0;
	if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_MAXIMIZE )
	{
		EQC_SET_X(hControl, nMargin);
		EQC_SET_Y(hControl, nMargin);
		pCheckBox->nLen = (WindowRect.xRight-WindowRect.xLeft+1-2*nMargin) / EQS_CharWidth((BYTE)' ');
		EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-2*nMargin);
		EQC_SET_DY(hControl, WindowRect.yBottom - WindowRect.yTop + 1 -2*nMargin);
	}
	else if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_HMAXIMIZE )
	{
		EQC_SET_X(hControl, nMargin);
		pCheckBox->nLen = (WindowRect.xRight-WindowRect.xLeft+1-2*nMargin) / EQS_CharWidth((BYTE)' ');
		EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-2*nMargin);	
	}
	else ;
}
