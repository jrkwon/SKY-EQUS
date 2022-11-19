/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_rbtn.c
**
** Description: 
**     Radio Button Control
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-10-03   redstar           Created.
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
typedef struct tagRADIO_BUTTON_T  RADIO_BUTTON_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagRADIO_BUTTON_T
{
    CONTROL_HEADER_T Header;
    UINT8            nButton;
    UINT8            nCheckButton;
	UINT8            nLen;
    BYTE             **ppTextInfo;
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
LOCAL UINT8            l_nCheckIndex;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, \
                                            EQS_EVENT* pEvent);
LOCAL void* EQS_API sAllocInstance(BYTE** ppszInit, UINT8 nButton);
LOCAL void  EQS_API sDrawInstance(HCONTROL hControl);

            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(BYTE** ppszInit, UINT8 nButton)
{
    UINT8 i;
	RADIO_BUTTON_T *pRadioButton;

	pRadioButton = (RADIO_BUTTON_T *)EQS_Malloc(sizeof(RADIO_BUTTON_T));

    pRadioButton->nButton     = nButton;
    pRadioButton->ppTextInfo  = (BYTE**) EQS_Malloc((SIZE_T)(sizeof(BYTE*)*nButton));

    for(i=0; i< nButton; i++)
    {
        pRadioButton->ppTextInfo[i] = (BYTE *)ppszInit[i];
    }

	return (void *)pRadioButton;
}


LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	RADIO_BUTTON_T *pRadioButton;
    static   BOOL  fCheck = FALSE;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_RADIO_BUTTON);

	pRadioButton = (RADIO_BUTTON_T *) EQC_GET_INSTANCE( hControl );

	switch (pEvent->EventID)
	{
	case EQE_APP_IO_MODE:
		l_fDirty = EQC_GET_DIRTY(hControl);
		l_nCheckIndex  = pRadioButton->nCheckButton;
		break;
	case EQE_APP_DRAW:
        if( EQS_HaveFocus(hControl) == TRUE )
            fCheck = TRUE;
        if( EQS_IsActive(hControl) == TRUE )
            fCheck = FALSE;
		sDrawInstance(hControl);
		break;

	case EQE_KEY_UP:
		if (pRadioButton->nCheckButton > 0)
        {
			pRadioButton->nCheckButton --;            
        }
        else
        {
            pRadioButton->nCheckButton = pRadioButton->nButton - 1;

        }       
        EQS_SendEvent( EQE_APP_DRAW, hControl, 0);
		break;

	case EQE_KEY_DOWN:
		if (pRadioButton->nCheckButton < (pRadioButton->nButton-1) )
        {
			pRadioButton->nCheckButton++;            
        }
        else
        {         
            pRadioButton->nCheckButton = 0;

        }  
        EQS_SendEvent( EQE_APP_DRAW, hControl, 0);
		break;

    case EQE_KEY_CENTER:
        //pRadioButton->nCheckButton = pRadioButton->nIndex;
		EQC_SET_DIRTY(hControl, TRUE);
        EQS_SendEvent(EQE_IO_RADIO_BUTTON_SELECT, (WORD)hControl, (DWORD)(pRadioButton->nCheckButton));     

        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, 0);
        EQS_EndIO();
        break;
    case EQE_KEY_CLEAR:
		EQC_SET_DIRTY(hControl, l_fDirty);
		pRadioButton->nCheckButton = l_nCheckIndex;
        EQS_SendEvent(EQE_IO_RADIO_BUTTON_CANCEL, (WORD)hControl, (DWORD)0);
        EQS_EndIO();
        break;

    case EQE_IO_DELETE:

        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_RADIO_BUTTON_CANCEL, (WORD)hControl, EQC_DELETED);  

        // Free instance
        sFreeInstance(hControl);

        break;
	}
}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{       
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

    EQS_Free((void**)&((RADIO_BUTTON_T *)EQC_GET_INSTANCE(hControl))->ppTextInfo );    
	EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL void EQS_API sDrawRadioButton(HCONTROL hControl, UINT8 nPos, BOOL fReverse)
{//
	UINT8 i;
	UINT8 nHeight;
	BYTE  szBuf[22];
	RADIO_BUTTON_T *pRadioButton;
    COLOR OldTextBackColor, OldTextColor;  

	
	pRadioButton = (RADIO_BUTTON_T *)  EQC_GET_INSTANCE( hControl );
    nHeight = EQS_CharHeight((BYTE)' ');

	OldTextBackColor = EQS_GetTextBackColor();
	OldTextColor = EQS_GetTextColor();

	if( fReverse )
	{
        if( EQS_IsActive(hControl))
		{
			EQS_SetTextBackColor(EQS_CURSOR_BACK_COLOR);
			EQS_SetTextColor(EQS_CURSOR_FORE_COLOR);
		}
		STRCPY(szBuf, (BYTE*) SF_RADIO_BUTTON_ON);
	}
    else
    {
		STRCPY(szBuf, (BYTE*) SF_RADIO_BUTTON_OFF); 
    }

	STRCAT(szBuf, (BYTE*)(pRadioButton->ppTextInfo[nPos]));

    EQS_TextOutStrLen(EQC_GET_X(hControl),
		              EQC_GET_Y(hControl) + nPos * nHeight,
                      szBuf, pRadioButton->nLen);
	if( fReverse )
	{
		EQS_SetTextBackColor(OldTextBackColor);
		EQS_SetTextColor(OldTextColor);				
	}
}

LOCAL void EQS_API sDrawInstance(HCONTROL hControl)
{
	UINT8 i;
	UINT8 nHeight;
	BYTE  szBuf[22];
	RADIO_BUTTON_T *pRadioButton;
    COLOR OldTextBackColor, OldTextColor;  
    TEXT_DRAW_MODE TextDrawMode;
	

    EQS_GetDC(TRUE);
#if 0
	if( EQC_GET_FOCUS(hControl) == FALSE )
	{
		EQS_SetTextColor(CTRL_DEACTIVE_COLOR);
	}
#endif

	pRadioButton = (RADIO_BUTTON_T *)  EQC_GET_INSTANCE( hControl );
    nHeight = EQS_CharHeight((BYTE)' ');

	for (i = 0; i < pRadioButton->nButton; i++)
	{
        if( i == pRadioButton->nCheckButton && pRadioButton->nLen != 1 )
        {   
			sDrawRadioButton(hControl, i, TRUE);
        }
        else
        {
			sDrawRadioButton(hControl, i, FALSE);
        }
	}
	
    if( EQC_GET_STYLE(hControl) & CS_RADIO_BUTTON_BORDER  )
	{
        EQC_DrawBorder(hControl, EQC_GET_X(hControl) , EQC_GET_Y(hControl),
                       (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
                       (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1));
	}

    EQS_ReleaseDC();
	EQS_UpdateControlDisplay();
   
}
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL EQS_API EQC_RadioButton(COORD x, COORD y, UINT8 nChar, UINT8 nLine,
                                 BYTE** ppszInit,  UINT8 nIndex )
{
	void *pInstance;
	HCONTROL hControl;
    COORD  Width, Height;
    UINT8  nLen, i;

	pInstance = sAllocInstance(ppszInit, nLine);

    Width = nChar;
    for(i=0; i<nLine; i++)
    {
        nLen = STRLEN(ppszInit[i]);
        if( nLen > Width )
            Width = nLen;
    }
        
        
    Width   = EQS_CharWidth((BYTE)' ') * (Width +2) ;
    Height = EQS_CharHeight((BYTE)' ') * nLine;

/* Original - woodstok 01.1.8    
	hControl = EQC_AllocControl(CT_RADIO_BUTTON,  pInstance, x, y, 
                                nWidth, nHeight,
                                sHandleEvent, sFreeInstance);
*/
	hControl = EQC_AllocControl(CT_RADIO_BUTTON,  pInstance, x, y, 
                                Width, Height,
                                sHandleEvent);

	ASSERT(hControl != INVALID_CONTROL_HANDLE);

	((RADIO_BUTTON_T *)pInstance)->nCheckButton  = nIndex;
	((RADIO_BUTTON_T *)pInstance)->nLen   = Width / EQS_CharWidth((BYTE)' ');

    EQC_SET_STYLE(hControl, CS_RADIO_BUTTON_DEFAULT);	

    EQS_SendEvent( EQE_APP_DRAW, 0, 0 );

	return hControl;
}

void        EQS_API EQC_SetRadioButton(HCONTROL hControl,   UINT8 nPos)
{
    RADIO_BUTTON_T *pRadioButton;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_RADIO_BUTTON);

    /* get the pointer of teh control instance */
    pRadioButton = (RADIO_BUTTON_T *)EQC_GET_INSTANCE(hControl);

    pRadioButton->nCheckButton = nPos;
    EQS_SendEvent( EQE_APP_DRAW, 0, 0);

}


UINT8        EQS_API EQC_GetRadioButton(HCONTROL hControl)
{
    RADIO_BUTTON_T *pRadioButton;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_RADIO_BUTTON);

    /* get the pointer of teh control instance */
    pRadioButton = (RADIO_BUTTON_T *)EQC_GET_INSTANCE(hControl);

    return pRadioButton->nCheckButton;
}

UINT8 EQS_API EQC_GetRadioButtonDisplayLine(COORD StartPosY)
{
    UINT8  nLines;
    RECT   WindowRect;
    COORD  DY;    
    
    EQS_GetSWindowViewAreaRect(&WindowRect);

    DY = WindowRect.yBottom - WindowRect.yTop - StartPosY -2;
  
    nLines =  DY / EQS_CharHeight((BYTE)' ');

    return nLines;
}

void        EQS_API EQC_SetRadioButtonStyle(HCONTROL hControl, WORD StyleA)
{
    RECT   WindowRect;
    RADIO_BUTTON_T *pRadioButton;
	UINT8  nMargin;
    /* <-- Add here --> the local variable for handling the event */
    
    /* check the control type */
    ASSERT(EQC_GET_TYPE(hControl) == CT_RADIO_BUTTON);
	pRadioButton = (RADIO_BUTTON_T *)EQC_GET_INSTANCE(hControl);

	EQC_GET_INSTANCE(hControl)->Style = (StyleA);

    EQS_GetSWindowViewAreaRect(&WindowRect);

	if( EQC_GET_STYLE(hControl) & CS_RADIO_BUTTON_BORDER )
		nMargin = 2;
	else
		nMargin = 0;

	if( EQC_GET_STYLE(hControl) & CS_RADIO_BUTTON_MAXIMIZE )
	{
		EQC_SET_X(hControl, nMargin);
		EQC_SET_Y(hControl, nMargin);
		pRadioButton->nLen = (WindowRect.xRight-WindowRect.xLeft+1-2*nMargin) / EQS_CharWidth((BYTE)' ');
		EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-2*nMargin);
		EQC_SET_DY(hControl, WindowRect.yBottom - WindowRect.yTop + 1 -2*nMargin);
	}
	else if( EQC_GET_STYLE(hControl) & CS_CHECK_BOX_HMAXIMIZE )
	{
		EQC_SET_X(hControl, nMargin);
		pRadioButton->nLen = (WindowRect.xRight-WindowRect.xLeft+1-2*nMargin) / EQS_CharWidth((BYTE)' ');
		EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-2*nMargin);	
	}
	else ;

}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
