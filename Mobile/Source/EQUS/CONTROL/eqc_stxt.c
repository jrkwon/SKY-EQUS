/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     
**
** Description: 
**     <@Desciption about this file here>
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 00-07-05   girin      Created.
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
//#define STEXT_DEFAULTS      0
//#define DUMMY 0

#define TEXT_AREA_CLEAR    1
#define DEACTIVE_COLOR  RGB332(192,160, 128)
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
#define MAX_STATIC_TEXT_LEN      21
typedef struct tagSTATIC_TEXT_T  STATIC_TEXT_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagSTATIC_TEXT_T
{
    /* control header */
    CONTROL_HEADER_T Header;   
    /* <-- Add here --> the information about this control */
    BYTE		szStr[MAX_STATIC_TEXT_LEN];
	DRAW_MODE   DrawMode;
    BOOL		fShow;
    HTIMER		hTimer;
    INT16		nPos;
    UINT16		nLen, nPreviousLen, nAllocLen;
    BOOL        fActive;        //white 2001.06.08
	COLOR       TextColor;
	BOOL        fUserTextColor;
	STATIC_TEXT_FLOW_TYPE_E FlowMode;
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
LOCAL void* EQS_API sAllocInstance(BYTE *pszStr);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void EQS_API sDrawInstance(HCONTROL hControl);
            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(BYTE *pszStr)
{
    STATIC_TEXT_T *pStaticText;

    /* allocate memory for the instance itself */
	pStaticText = (STATIC_TEXT_T *)EQS_Malloc(sizeof(STATIC_TEXT_T));
    
    /* --> Add here <-- allocate memory for this instance information */
	pStaticText->szStr[0] = 0;

    
    STRNCPY(pStaticText->szStr, pszStr, MAX_STATIC_TEXT_LEN-1);
	pStaticText->szStr[MAX_STATIC_TEXT_LEN-1] = 0;
    pStaticText->nLen   = STRLEN(pStaticText->szStr);
	pStaticText->nAllocLen = pStaticText->nLen;
    pStaticText->nPreviousLen = pStaticText->nLen;  // woodstok 01.6.14
    pStaticText->fShow  = TRUE;
    pStaticText->hTimer = INVALID_TIMER_HANDLE;
    pStaticText->nPos   = 1;
	pStaticText->DrawMode = TDM_NORMAL;
    pStaticText->fActive = TRUE;                 // white 2001.06.08 
	pStaticText->TextColor = EQS_GetTextColor();
	pStaticText->fUserTextColor = FALSE;

	pStaticText->FlowMode = STF_NONE;

	return (void *)pStaticText;
}


LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	STATIC_TEXT_T *pStaticText;
    COORD   Dx;

	ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

    switch( pEvent->EventID )
    {
    case EQE_APP_DRAW:
        if( pEvent->dwParam == TEXT_AREA_CLEAR )
        {      
            Dx = pStaticText->nPreviousLen * EQS_CharWidth((BYTE)' ');
            
            if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_BORDER )
            {
                // use background color instead of text background color 
                if(!(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING))
                    EQS_FillRectangle(EQC_GET_X(hControl)-1,\
                                      EQC_GET_Y(hControl)-1,\
                                      EQC_GET_X(hControl)+Dx,\
                                      EQC_GET_Y(hControl)+EQC_GET_DY(hControl),\
                                      EQS_GetBackColor());  
            }
            else
            {
                if(!(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING))
                    EQS_FillRectangle(EQC_GET_X(hControl),\
                                      EQC_GET_Y(hControl),\
                                      EQC_GET_X(hControl)+Dx-1,\
                                      EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1,\
                                      EQS_GetBackColor());
            }
      
        }
        sDrawInstance( hControl );
        break;
	
    case EQE_TM_TIMER1:
	case EQE_TM_TIMER2:
        if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_FLOW )
        {
            pStaticText->nPos++;
        }
        EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
        
        break;
    case EQE_IO_DELETE:
    
        // Send ending event you want..

        // Free instance
        sFreeInstance(hControl);    
        break;
    default:
        break;
    }
}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	STATIC_TEXT_T *pStaticText;
	void * pVoid;

	ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);
    /* free the memory that was allocated */
    if( pStaticText->hTimer != 0 )
        EQS_StopControlTimer(hControl, pStaticText->hTimer);

    /* free the instance itself */
	pVoid = EQC_GET_INSTANCE(hControl);
    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL void EQS_API sDrawInstance(HCONTROL hControl)
{
	STATIC_TEXT_T *pStaticText;
    COORD          StartX;
    COLOR          BackColor;
    INT16          nChar, nLength, nWidth;
	BYTE           szBuf[80];
    DRAW_MODE	   OldDrawMode;
    COLOR		   OldColor, OldTextColor = WHITE;


	ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);


	if( EQC_GET_X(hControl) + EQC_GET_DX(hControl) > EQS_GetMaxX() )
	{
		nWidth = EQS_GetMaxX() - EQC_GET_X(hControl);
	}
	else
	{
		nWidth = EQC_GET_DX(hControl);
	}

    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_BLINK )
    {
        pStaticText->fShow = !pStaticText->fShow;
        if( !pStaticText->fShow)
        {
            if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_BORDER )
            {
                // use background color instead of text background color 
                if(!(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING))
                    EQS_FillRectangle(EQC_GET_X(hControl)-1,\
                                      EQC_GET_Y(hControl)-1,\
                                      EQC_GET_X(hControl)+nWidth,\
                                      EQC_GET_Y(hControl)+EQC_GET_DY(hControl),\
                                      EQS_GetBackColor());
            }
            else
            {
                if(!(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING))
                    EQS_FillRectangle(EQC_GET_X(hControl),\
                                      EQC_GET_Y(hControl),\
                                      EQC_GET_X(hControl)+nWidth-1,\
                                      EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1,\
                                      EQS_GetBackColor());
            }    
            EQS_UpdateControlDisplay();
        }

    }    
    if( pStaticText->fShow == FALSE )
        return ;


    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_LEFT )
    {
        StartX = EQC_GET_X(hControl);
    }
    else if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_RIGHT )
    {
        StartX = EQC_GET_X(hControl) + nWidth -1   \
                 - EQS_CharWidth((BYTE)' ')*STRLEN(pStaticText->szStr);
    }
    else if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_CENTER )
    {
        StartX = EQC_GET_X(hControl) + nWidth/2 -1\
                 - (EQS_CharWidth((BYTE)' ')*STRLEN(pStaticText->szStr))/2;
    }
    else
    {
        StartX = EQC_GET_X(hControl);
    }

    EQS_GetDC(TRUE);


	if( pStaticText->fUserTextColor )
	{
		EQS_SetTextColor( pStaticText->TextColor );
	}
	else
	{
		EQS_SetTextColor(TEXT_FORE_COLOR);
		//EQS_SetTextColor(  EQS_GetTextColor() );
	}


    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_CAPTION )
    {
        EQS_SetDrawMode( DM_OR );
        EQS_SetTextColor( CAPTION_FORE_COLOR );
    }
    
    
    if( EQC_GET_STYLE( hControl ) & CS_STATIC_TEXT_BORDER )
    {
        EQC_DrawBorder(hControl,\
			           EQC_GET_X(hControl)-1,                      \
                       EQC_GET_Y(hControl)-1,                      \
                       EQC_GET_X(hControl)+nWidth,   \
                       EQC_GET_Y(hControl)+EQC_GET_DY(hControl)    \
                      );        
    }     
    if( !(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_NO_CHANGE ) && (!(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING)))
    {
        if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_CAPTION )        
            BackColor = CAPTION_BACK_COLOR;
        else
            BackColor = EQS_GetBackColor();

        EQS_FillRectangle(EQC_GET_X(hControl),  
                          EQC_GET_Y(hControl), \
                          EQC_GET_X(hControl)+nWidth-1, \
                          EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-1, \
                          BackColor);
    }

    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_FLOW )
    {

		nLength = nWidth / EQS_CharWidth((BYTE)' ');
        if( nLength >= pStaticText->nPos )
		{			
			if( IsMiddleOfMultiByte(pStaticText->szStr,pStaticText->nPos) )
				pStaticText->nPos++;
			nChar = pStaticText->nPos;
			
			STRNCPY(szBuf, pStaticText->szStr, nChar);
			szBuf[nChar] = 0;
			
			StartX = EQC_GET_X(hControl) + nWidth -1 -EQS_TextWidth(szBuf) -1;
		}
		else
		{

			nChar = pStaticText->nPos - nLength-1;
			if( IsMiddleOfMultiByte(pStaticText->szStr, nChar) )
				nChar++;
			if( nChar >= STRLEN( pStaticText->szStr ) )//pStaticText->nLen )
			{
				nLength = nWidth / EQS_CharWidth((BYTE)' ');
/*
				switch( pStaticText->FlowMode )
				{
				case STF_NONE:
					pStaticText->nPos = 1;
					break;
				case STF_ALL_SHOW:
					pStaticText->nPos = nLength;
					break;
				default:
					pStaticText->nPos = 1;
					break;
				}
*/
				pStaticText->nPos = 1;
				
				EQS_StartControlTimer(hControl, EQS_TIMER_2, 300, FALSE);
				return ;
			}
			STRCPY(szBuf, pStaticText->szStr + nChar);
		}	

	}
//    EQS_SetDrawMode(DM_OR);    
    EQS_SetTextBackColor(EQS_GetBackColor());

    OldColor = EQS_GetTextBackColor();
    OldDrawMode = EQS_GetDrawMode();
    if(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_ORING)
    {
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextBackColor(BLACK);
    }
	if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_FLOW )
	{	
		nLength = EQC_GET_DX(hControl) / EQS_CharWidth((BYTE)' ');
		EQS_TextOutStrLenTDM(StartX, EQC_GET_Y(hControl), szBuf, nLength, pStaticText->DrawMode);	
		EQS_StartControlTimer(hControl, EQS_TIMER_2, 300, FALSE);
	}
	else
	{
        //white 2001.06.08 
        if(!pStaticText->fActive)
        {
            EQS_SetTextBackColor(EQS_GetBackColor());
            OldTextColor = EQS_GetTextColor();
            EQS_SetTextColor(DEACTIVE_COLOR);
        }
        //white 
        nLength = STRLEN( pStaticText->szStr );
		if( nLength > pStaticText->nLen )
			nLength = pStaticText->nLen;
		EQS_TextOutStrLenTDM(StartX, EQC_GET_Y(hControl),\
						     pStaticText->szStr, nLength,\
							 pStaticText->DrawMode);
        
        //white 2001.06.08 
        if(!pStaticText->fActive)
            EQS_SetTextColor(OldTextColor);
        //white 

	}
    EQS_SetDrawMode(OldDrawMode);
    EQS_SetTextBackColor(OldColor);

//    EQS_SetDrawMode(DM_COPY);

    EQS_ReleaseDC();
    EQS_UpdateControlDisplay();
}

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_StaticText(COORD x, COORD y, WORD TokenID)
{
	STATIC_TEXT_T *pStaticText;
	HCONTROL hControl;
    COORD dx, dy;
    BYTE* pszStr;

    pszStr = EQS_GET_TOKEN_EX( TokenID );

    /* allocation memory for this instance */
	pStaticText = (STATIC_TEXT_T *)sAllocInstance(pszStr);

    /* get dx, dy */
    dx = EQS_TextWidth(pszStr);
    dy = EQS_TextHeight(pszStr);
    if( dx > EQS_GetMaxWidth() )
        dx = EQS_GetMaxWidth();

/* Original - woodstok 01.1.8
	hControl = EQC_AllocControl(CT_STATIC_TEXT,  pStaticText, x, y, dx, dy, 
                                sHandleEvent, sFreeInstance );
*/
	hControl = EQC_AllocControl(CT_STATIC_TEXT,  pStaticText, x, y, dx, dy, 
                                sHandleEvent );

    ASSERT( hControl != INVALID_CONTROL_HANDLE);

    //pStaticText->Header.fFocus = FALSE;             

    EQC_SET_FOCUS(hControl, FALSE);
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_DEFAULT);
    /* send draw event to the state machine engine */
	EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, 0);

    return hControl;
}

//white 2001.06.08
void EQS_API EQC_SetStaticTextActive(HCONTROL hControl, BOOL fActive)
{
	STATIC_TEXT_T *pStaticText;

    /* jrkwon 2001.06.28 */
	EQS_ASSERT_VOID( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);
    if( !(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_NO_CHANGE ) )
    {
        pStaticText->fActive = fActive;
    }

    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_NO_CHANGE )
        return;
    else
    {
        EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, TEXT_AREA_CLEAR );
    }

}
//white 

void EQS_API EQC_SetStaticText(HCONTROL hControl, CONST BYTE* pszStr)
{
	STATIC_TEXT_T *pStaticText;
	UINT8 nLen;
    
	EQS_ASSERT_VOID(EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	nLen = STRLEN(pszStr);

    
    STRNCPY((BYTE*) pStaticText->szStr , (BYTE*)pszStr, MAX_STATIC_TEXT_LEN-1 );
	pStaticText->szStr[MAX_STATIC_TEXT_LEN-1] = 0;
  
    if( !(EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_NO_CHANGE ) )
    {
        pStaticText->nPreviousLen = pStaticText->nLen;
        pStaticText->nLen = nLen;
        EQC_SET_DX(hControl, EQS_TextWidth(pStaticText->szStr) );  //2001.01.18
        EQC_SET_DY(hControl, EQS_TextHeight(pStaticText->szStr) );      
    }

    if( EQC_GET_STYLE(hControl) & CS_STATIC_TEXT_NO_CHANGE )
        return;
    else
    {
        EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, TEXT_AREA_CLEAR );
    }
}

void EQS_API EQC_ShowStaticText(HCONTROL hControl, BOOL fShow)
{
	STATIC_TEXT_T *pStaticText;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

    pStaticText->fShow  = fShow;

}

void EQS_API EQC_SetStaticTextStyle(HCONTROL hControl, WORD StyleA)
{

	STATIC_TEXT_T *pStaticText;
    

	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
    if(EQC_GET_TYPE(hControl) != CT_STATIC_TEXT)
        return;


    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

    EQC_GET_INSTANCE(hControl)->Style = (StyleA);

    if( StyleA &  CS_STATIC_TEXT_BLINK )
    {
        if( pStaticText->hTimer != INVALID_TIMER_HANDLE )
		{
			EQS_StopControlTimer( hControl, pStaticText->hTimer );
		}
        pStaticText->hTimer = EQS_StartControlTimer(hControl, \
                                                        EQS_TIMER_1, 500,TRUE);
    }
    else if( StyleA & CS_STATIC_TEXT_FLOW )
    {
		pStaticText->nPos = 1;
        EQS_StartControlTimer(hControl, EQS_TIMER_2, 300, FALSE);
    }
    else
    {
        if( pStaticText->hTimer != INVALID_TIMER_HANDLE )
		{
			EQS_StopControlTimer( hControl, pStaticText->hTimer );
			pStaticText->hTimer = INVALID_TIMER_HANDLE;
		}		
    }
}

void     EQS_API EQC_SetStaticTextBlink(HCONTROL hControl, WORD Time)
{

	STATIC_TEXT_T *pStaticText;
    
	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
    if(EQC_GET_TYPE(hControl) != CT_STATIC_TEXT)
        return;

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

    if( Time == 0 )
    {
        pStaticText->fShow = TRUE;
        if( pStaticText->hTimer )
            EQS_StopControlTimer(hControl, pStaticText->hTimer);
    }
    else
    {
        pStaticText->hTimer = EQS_StartControlTimer(hControl, \
                                                    EQS_TIMER_1, Time,TRUE);
    }

}


CONST BYTE* EQS_API EQC_GetStaticText(HCONTROL hControl)
{
	STATIC_TEXT_T *pStaticText;
    
	ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	return pStaticText->szStr;
  
}

void     EQS_API EQC_SetStaticTextAttribute(HCONTROL hControl, DRAW_MODE DrawMode)
{
	STATIC_TEXT_T *pStaticText;
    

	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
    if(EQC_GET_TYPE(hControl) != CT_STATIC_TEXT)
        return;

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	pStaticText->DrawMode = DrawMode;
}

void   EQS_API EQC_SetStaticTextColor(HCONTROL hControl, COLOR TextColor)
{
	STATIC_TEXT_T *pStaticText;
    
	ASSERT(EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
	    	
	pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	pStaticText->TextColor = TextColor;
	pStaticText->fUserTextColor = TRUE;

}
COLOR  EQS_API EQC_GetStaticTextColor(HCONTROL hControl)
{
	STATIC_TEXT_T *pStaticText;
    
	ASSERT(EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
	    	
	pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	return pStaticText->TextColor;
}


void     EQS_API EQC_SetStaticTextFlowMode(HCONTROL hControl, STATIC_TEXT_FLOW_TYPE_E FlowMode)
{
	STATIC_TEXT_T *pStaticText;
	INT nLength, nWidth;
    
	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
    if(EQC_GET_TYPE(hControl) != CT_STATIC_TEXT)
        return;

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);

	if( EQC_GET_X(hControl) + EQC_GET_DX(hControl) > EQS_GetMaxX() )
	{
		nWidth = EQS_GetMaxX() - EQC_GET_X(hControl);
	}
	else
	{
		nWidth = EQC_GET_DX(hControl);
	}
	nLength = nWidth / EQS_CharWidth((BYTE)' ');

	
	switch( FlowMode )
	{
    case STF_NONE:
		pStaticText->nPos = 1;
		break;
    case STF_ALL_SHOW:
		pStaticText->nPos = nLength+1;
		break;
	default:
		pStaticText->nPos = 1;
		break;
	}
	pStaticText->FlowMode = FlowMode;
}

UINT8 EQS_API EQC_GetStaticTextPreviousLength(HCONTROL hControl)
{
	STATIC_TEXT_T *pStaticText;
	UINT8 nLen;
    
	EQS_ASSERT_VOID(EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);
	//ASSERT( EQC_GET_TYPE(hControl) == CT_STATIC_TEXT);

    pStaticText = (STATIC_TEXT_T *) EQC_GET_INSTANCE(hControl);
    return (UINT8) (pStaticText->nPreviousLen * EQS_CharWidth((BYTE)' '));
}

