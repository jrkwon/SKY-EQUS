/*
** ===========================================================================
**
** Project: 
**     Athens
**
** File:
**     eqc_dedt.c
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
** 00-07-29   rosa       Created.
** 00-11-22   woodstok   Big Change!
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
#define CLEAR_CANCEL

#define NE_BLINK_CURSOR_TIME       250
#define NEED_HYPEN_POSITION        4
#define MAX_DIAL_NUMBER_LEN        8

#define sMAX_MATCH_NUMBER          3

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

typedef struct tagNE_CURSOR_T          NE_CURSOR_T;
typedef struct tagNUMBER_EDIT_T        NUMBER_EDIT_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagNE_CURSOR_T {
    COORD   x;
    COORD   y;
    BYTE    bChar;
    BOOL    fCursorReverse;
};

struct tagNUMBER_EDIT_T
{
    CONTROL_HEADER_T    Header;
    BYTE                *szNumber; 
	UINT8               nChar;
	UINT8               nLine;
    UINT8               nMaxLen;
    UINT8               nPos;
    UINT8               nCodePos;
    UINT8               nHypenPos;
    BYTE                *paszMatchCond[sMAX_MATCH_NUMBER];
    UINT8               nNotifyLen;
    BOOL                fMatchFlag;
    HTIMER              hCursorTimer;
    NE_CURSOR_T         Cursor;
    BOOL                fBigFont;
    BOOL                fActive;
    BYTE                *szLocalCode;   // 자동지역번호
    COLOR               BackColor;
	COLOR               ForeColor;
    UINT8               nPrefix;
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

LOCAL NUMBER_EDIT_T *pNumberEdit;
LOCAL BYTE  *szSaveNumber = NULL;

LOCAL UINT8   nSaveLen;
LOCAL BOOL    fDirty;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(void);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);

LOCAL UINT8 sAddDigit(HCONTROL hControl, BYTE bDigit);
LOCAL UINT8 sDeleteDigit(HCONTROL hControl);
LOCAL void sMoveLeft(HCONTROL hControl);
LOCAL void sMoveRight(HCONTROL hControl);

/* Display Associated Function */
LOCAL void sDrawNumberEdit(HCONTROL hControl);
LOCAL void sDrawNumber(HCONTROL hControl);
LOCAL void sDrawCursor(HCONTROL hControl);
LOCAL void sTextOutNumber(HCONTROL hControl, BYTE *szBuffer, UINT8 nPos, UINT8 nLen);
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

LOCAL void* sAllocInstance(void)
{
	NUMBER_EDIT_T *pNumberEdit;

	pNumberEdit = (NUMBER_EDIT_T *)EQS_Malloc(sizeof(NUMBER_EDIT_T));

	return (void *)pNumberEdit;
}

LOCAL void  EQS_API sFreeInstance(HCONTROL hControl)
{
    UINT8 i;
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    EQS_Free((void**)&(pNumberEdit->szNumber));

    for(i = 0; i < sMAX_MATCH_NUMBER; i++)
    {
        if(pNumberEdit->paszMatchCond[i] != NULL)
            EQS_Free((void**)&(pNumberEdit->paszMatchCond[i]));
    }

    if(pNumberEdit->szLocalCode != NULL)
        EQS_Free((void**)&(pNumberEdit->szLocalCode));

    if(szSaveNumber != NULL)
    {
        EQS_Free((void**)&szSaveNumber);
        szSaveNumber = NULL;
    }

    // Stop Timer
    if(pNumberEdit->hCursorTimer != NULL)
    {
        EQS_StopControlTimer(hControl,pNumberEdit->hCursorTimer);
        pNumberEdit->hCursorTimer = NULL;
    }

	EQS_Free((void**)&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL void  EQS_API sClearNumberEdit(NUMBER_EDIT_T * pNumberEdit)
{
    if(pNumberEdit->nPrefix)
    {
        pNumberEdit->szNumber[pNumberEdit->nPrefix] = '\0';
        pNumberEdit->nPos = pNumberEdit->nPrefix;
    }
    else
    {
        pNumberEdit->szNumber[0] = '\0';
        pNumberEdit->nPos = 0;
        pNumberEdit->nCodePos = 0;
        pNumberEdit->nHypenPos = 0;
    }
}

LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent)
{
    RECT    Rect;
    STATIC  UINT8 nLastEvent = NULL;

    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);

    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

	switch (pEvent->EventID)
	{
    case EQE_KEY_UP:
        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_RETURN_UPDOWN_KEY))
        {
            EQS_SendEvent(EQE_IO_NEDIT_PRESS_UPKEY, EQS_NA, EQS_NA);
            break;
        }

        if(!EQS_IsActive(hControl) || (EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_OKKEY)
            || (EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ALONE))
            break;
/*
        if(STRCMP(szSaveNumber, pNumberEdit->szNumber))
            EQC_SET_DIRTY(hControl, TRUE);
*/
        //EQS_SendEvent(EQE_IO_NEDIT_DONE, (WORD)hControl, STRLEN(pNumberEdit->szNumber));
        EQS_SendEvent(EQE_IO_NEDIT_DONE, (WORD)hControl, NER_UP_KEY);
        if(szSaveNumber != NULL)
        {
            EQS_Free((void**)&szSaveNumber);
            szSaveNumber = NULL;
        }

        pNumberEdit->nPos = 0;
        pNumberEdit->fActive = FALSE;

        if(!(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_AUTOFOCUSING))
            EQS_PassIO(-1);
        else
            EQS_EndIO();
        break;

    case EQE_KEY_DOWN:
        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_RETURN_UPDOWN_KEY))
        {
            EQS_SendEvent(EQE_IO_NEDIT_PRESS_DOWNKEY, EQS_NA, EQS_NA);
            break;
        }

        if(!EQS_IsActive(hControl) || (EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_OKKEY)
            || (EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ALONE))
            break;
/*
        if(STRCMP(szSaveNumber, pNumberEdit->szNumber))
            EQC_SET_DIRTY(hControl, TRUE);
*/
        //EQS_SendEvent(EQE_IO_NEDIT_DONE, (WORD)hControl, STRLEN(pNumberEdit->szNumber));
        EQS_SendEvent(EQE_IO_NEDIT_DONE, (WORD)hControl, NER_DOWN_KEY);
        if(szSaveNumber != NULL)
        {
            EQS_Free((void**)&szSaveNumber);
            szSaveNumber = NULL;
        }

        pNumberEdit->nPos = 0;
        pNumberEdit->fActive = FALSE;
        EQS_EndIO();
        break;

    case EQE_APP_IO_MODE:
        // Set Initial Cursor position in Active state
        pNumberEdit->fActive = TRUE;
        pNumberEdit->nPos = (UINT8)STRLEN(pNumberEdit->szNumber);

        szSaveNumber = (BYTE*)EQS_Malloc((SIZE_T)(pNumberEdit->nMaxLen+1));

        STRCPY(szSaveNumber, pNumberEdit->szNumber);

		fDirty = EQC_GET_DIRTY(hControl);
        break;

	case EQE_APP_DRAW:

        if(pEvent->wParam == hControl 
            && pEvent->dwParam != NULL
            && pEvent->dwParam == pNumberEdit->hCursorTimer)    // Cursor Blink
        {
            EQS_GetDC(TRUE);
            SetRect(&Rect, EQC_GET_X(hControl), EQC_GET_Y(hControl), 
                    (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl) - 1),
                    (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl) - 1));

            EQS_SetViewPortRect(&Rect);

            sDrawCursor(hControl);
            EQS_ReleaseDC();
        }
        else
    	    sDrawNumberEdit(hControl);
        EQS_UpdateControlDisplay();
		break;

    case EQE_KEY_0:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
        if(pNumberEdit->szLocalCode != NULL)
        {
            if(!STRCMP(pNumberEdit->szLocalCode, pNumberEdit->szNumber))
            {
                sClearNumberEdit(pNumberEdit);
                EQS_Free((void**)&(pNumberEdit->szLocalCode));
                pNumberEdit->szLocalCode = NULL;
            }
        }
	case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
        nLastEvent = (pEvent->EventID)-EQE_KEY_0;

        if(pEvent->EventID == EQE_KEY_POUND)
        {
            if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DIGIT_ONLY)
                break;

            sAddDigit(hControl, (BYTE)'#');
        }
        else if(pEvent->EventID == EQE_KEY_STAR)
        {
            if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DIGIT_ONLY)
                break;

            sAddDigit(hControl, (BYTE)'*');
        }
        else
        {
            sAddDigit(hControl, (BYTE)((pEvent->EventID)-EQE_KEY_0 + '0'));
        }

		if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
		{
			EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
		}
		else
		{
			EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
		}

        if(pNumberEdit->fMatchFlag == TRUE)
        {
            UINT8 i;

            for(i = 0; i < sMAX_MATCH_NUMBER; i++)
            {
                if(STRCMP(pNumberEdit->szNumber, pNumberEdit->paszMatchCond[i]) == 0)
                    EQS_SendEvent(EQE_IO_NEDIT_MATCH, hControl, i);
            }
        }
        break;

    case EQE_KEY_DIGIT_LONG:
        {
            UINT8   nLocalCodeLen;

            if(pNumberEdit->szLocalCode != NULL)
                nLocalCodeLen = STRLEN(pNumberEdit->szLocalCode);
            else
                nLocalCodeLen = 0;

            EQS_SendEvent(EQE_IO_NEDIT_PRESS_LONGKEY, hControl, (DWORD)nLocalCodeLen);

            if (!(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_TP_PAUSE))
                break;

            if(nLastEvent == 7 && pNumberEdit->nPos > (3 + nLocalCodeLen) ) // P-Pause
            {
                sDeleteDigit(hControl);
                sAddDigit(hControl, (BYTE)'P');
            }
            else if(nLastEvent == 8 && pNumberEdit->nPos > (3 + nLocalCodeLen))    // T_Pause
            {
                sDeleteDigit(hControl);
                sAddDigit(hControl, (BYTE)'T');
            }

		    if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
		    {
			    EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
		    }
		    else
		    {
			    EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
		    }
        }
        break;

    case EQE_KEY_CLEAR:

        nLastEvent = NULL;

        if(!STRLEN(pNumberEdit->szNumber))
        {
            EQS_SendEvent(EQE_IO_NEDIT_NODIGIT, hControl, 0);

#ifdef CLEAR_CANCEL
            if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_CANCEL))
            {
                EQS_SendEvent(EQE_IO_NEDIT_CANCEL, (WORD)hControl, 0);
                break;
            }

            // Restore Saved Number
            if(szSaveNumber != NULL)
            {
                EQC_SetNumberEditValue(hControl, szSaveNumber);
                EQS_Free((void**)&szSaveNumber);
                szSaveNumber = NULL;
            }

            if(!fDirty)
    	        EQC_SET_DIRTY(hControl, FALSE);

            EQS_SendEvent(EQE_IO_NEDIT_CANCEL, (WORD)hControl, 0);
            pNumberEdit->nPos = 0;
            pNumberEdit->fActive = FALSE;
            EQS_EndIO();
#endif
        }
        else if(pNumberEdit->szLocalCode != NULL)
        {
            if(!STRCMP(pNumberEdit->szLocalCode, pNumberEdit->szNumber))
            {
                sClearNumberEdit(pNumberEdit);
                EQS_Free((void**)&(pNumberEdit->szLocalCode));
                pNumberEdit->szLocalCode = NULL;
            }
            else if(STRLEN(pNumberEdit->szNumber) == 0)
                EQS_SendEvent(EQE_IO_NEDIT_NODIGIT, hControl, 0);
            else
                sDeleteDigit(hControl);
        }
        else
        {
            sDeleteDigit(hControl);
        }
		if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
		{
			EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
		}
		else
		{
			EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
		}

		break;
        
    case EQE_KEY_CLEAR_LONG:    // Clear All
        nLastEvent = NULL;
        
        sClearNumberEdit(pNumberEdit);
        EQS_SendEvent(EQE_IO_NEDIT_NODIGIT, hControl, 0);

		if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
		{
			EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
		}
		else
		{
			EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
		}
        break;

    case EQE_KEY_LEFT:
        nLastEvent = NULL;

        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_CURSOR))
        {
            sMoveLeft(hControl);
			if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
			{
				EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
			}
			else
			{
				EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
			}

        }
        break;

    case EQE_KEY_RIGHT:
        nLastEvent = NULL;

        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_CURSOR))
        {
            sMoveRight(hControl);
			if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
			{
				EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
			}
			else
			{
				EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
			}
        }
        break;

    case EQE_KEY_LEFT_LONG:
        nLastEvent = NULL;

        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_CURSOR))
        {
            pNumberEdit->nPos = 0;
			if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
			{
				EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
			}
			else
			{
				EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
			}
        }
        break;

    case EQE_KEY_RIGHT_LONG:
        nLastEvent = NULL;

        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_CURSOR))
        {
            pNumberEdit->nPos = STRLEN(pNumberEdit->szNumber) ;
			if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
			{
				EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
			}
			else
			{
				EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
			}		    
        }
        break;

    case EQE_KEY_CENTER:
        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_OKKEY))
            break;
/*
        if(STRCMP(szSaveNumber, pNumberEdit->szNumber))
            EQC_SET_DIRTY(hControl, TRUE);
*/
        EQS_SendEvent(EQE_IO_NEDIT_DONE, (WORD)hControl, NER_OK_KEY);
        if(szSaveNumber != NULL)
        {
            EQS_Free((void**)&szSaveNumber);
            szSaveNumber = NULL;
        }

        pNumberEdit->nPos = 0;
        pNumberEdit->fActive = FALSE;
        EQS_EndIO();
        break;

    case EQE_TM_TIMER1:     // Cursor Blink Timer
        if(EQS_IsActive(hControl))
        {
            // If Modal I/O Control exixts, do nothing
            if(EQS_GetModal() != INVALID_CONTROL_HANDLE)
                break;

			if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
			{
				EQS_SendEvent(EQE_APP_DRAW, EQS_NA, pNumberEdit->hCursorTimer);
			}
			else
			{
				EQS_SendEvent(EQE_APP_DRAW, hControl, pNumberEdit->hCursorTimer);
			}

        }
        break;

    case EQE_KEY_F1:
        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_IGNORE_CANCEL))
        {
            EQS_SendEvent(EQE_IO_NEDIT_CANCEL, (WORD)hControl, 0);
            break;
        }

        // Restore Saved Number
        if(szSaveNumber != NULL)
        {
            EQC_SetNumberEditValue(hControl, szSaveNumber);
            EQS_Free((void**)&szSaveNumber);
            szSaveNumber = NULL;
        }

        if(!fDirty)
    	    EQC_SET_DIRTY(hControl, FALSE);

        EQS_SendEvent(EQE_IO_NEDIT_CANCEL, (WORD)hControl, 0);
        pNumberEdit->nPos = 0;
        pNumberEdit->fActive = FALSE;
        EQS_EndIO();
        break;

    case EQE_IO_DELETE:
        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_NEDIT_CANCEL, (WORD)hControl, EQC_DELETED);

        // Free instance
        sFreeInstance(hControl);
        break;
	}
}


LOCAL void sDrawNumberEdit(HCONTROL hControl)
{
    RECT Rect;

    EQS_GetDC(TRUE);

    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    // Set Display Color
    SetRect(&Rect, EQC_GET_X(hControl), EQC_GET_Y(hControl), 
            (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl) - 1),
            (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl) - 1));

    EQS_SetViewPortRect(&Rect);


    if(pNumberEdit->fBigFont)
    {
		if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING) == NULL)
        {
            if(pNumberEdit->BackColor == MAX_COLOR)
			    EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), EQS_GetTextBackColor());
            else
                EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), pNumberEdit->BackColor);
        }
    }
    else
    {
		if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING) == NULL)
        {
            if(pNumberEdit->BackColor == MAX_COLOR)
	            EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), EQS_GetTextBackColor());
            else
                EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), pNumberEdit->BackColor);
        }
    }

    if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_BORDER)
    {
        EQC_DrawBorder(hControl, 0, 0, EQS_GetMaxX(), EQS_GetMaxY()); 
    }

    if(EQS_IsActive(hControl))
    { 
        // Start Cursor Timer && Set Cursor blink state
        if((EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_CURSOR))
        {
            if(pNumberEdit->hCursorTimer == NULL)
            {
                // Start Blink cursor timer
                pNumberEdit->hCursorTimer 
                    = EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                                           NE_BLINK_CURSOR_TIME, TRUE);
            }
        }
        if( pNumberEdit->nNotifyLen )
        {
            if( STRLEN(pNumberEdit->szNumber) == pNumberEdit->nNotifyLen
                && nSaveLen != pNumberEdit->nNotifyLen)
                EQS_SendEvent(EQE_IO_NEDIT_MATCH_LENGTH, hControl, 0);

            nSaveLen = STRLEN(pNumberEdit->szNumber);
        }
    }   
    else    // not Active
    {
        if(pNumberEdit->hCursorTimer != NULL)
        {
            EQS_StopControlTimer(hControl,pNumberEdit->hCursorTimer);
            pNumberEdit->hCursorTimer = NULL;
        }

        // Set Initial Cursor position in Not Active state
        if(pNumberEdit->fActive == TRUE)
        {
            pNumberEdit->fActive = FALSE;
            pNumberEdit->nPos = 0;
        }

    }

/*==============*/

    // Draw Number
    sDrawNumber(hControl);

    EQS_ReleaseDC();

    if (EQS_IsActive(hControl) && !(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_NO_CML))
    {
		EQS_EVENT CurrentEvent;

        EQS_DrawIOCCML(IOC_CML_CANCEL_LEFT);
        EQS_DrawIOCCML(IOC_CML_DONE_CENTER);

		CurrentEvent = EQS_GetEvent();
		if( CurrentEvent.wParam == hControl )
		{
			EQS_UpdateIOCCML();
		}
		
    }
}


LOCAL void sDrawCursor(HCONTROL hControl)
{
    COLOR  saveColor, OldTextBackColor, OldTextColor;
    CONST SIZE* pFontSize;
    FONT_TYPE   saveFont;

	pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(pNumberEdit->Cursor.bChar == '\0')
        return;

    saveFont = EQS_GetFontType();

    if(pNumberEdit->fBigFont)
    {
        pFontSize = EQS_GetFontSize(FT_BIGBIG, FK_ASCII);
        EQS_SetFontType(FT_BIGBIG);
    }
    else
    {
        pFontSize = EQS_GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);
    }

    saveColor = EQS_GetForeColor();

    if(pNumberEdit->Cursor.fCursorReverse)
        EQS_SetForeColor(EQS_GetTextBackColor());

    if(pNumberEdit->Cursor.fCursorReverse)
    {
        OldTextBackColor = EQS_GetTextBackColor();
        OldTextColor = EQS_GetTextColor();
        EQS_SetTextBackColor(BLUE);
        EQS_SetTextColor(WHITE);
        EQS_TextOutStrLenTDM(pNumberEdit->Cursor.x, pNumberEdit->Cursor.y,
                (BYTE*)&pNumberEdit->Cursor.bChar, 
                (UINT8)1,
                TDM_NORMAL);
        EQS_SetTextBackColor(OldTextBackColor);
        EQS_SetTextColor(OldTextColor);
    }
    else
    {
        EQS_TextOutStrLenTDM(pNumberEdit->Cursor.x, pNumberEdit->Cursor.y,
                (BYTE*)&pNumberEdit->Cursor.bChar, 
                (UINT8)1,
                TDM_NORMAL);
    }

/*
    EQS_Line(pNumberEdit->Cursor.x, 
             pNumberEdit->Cursor.y, 
             pNumberEdit->Cursor.x,
             (COORD)(pNumberEdit->Cursor.y + pFontSize->yHeight));
*/
    pNumberEdit->Cursor.fCursorReverse = !pNumberEdit->Cursor.fCursorReverse;

    EQS_SetFontType(saveFont);
    EQS_SetForeColor(saveColor);
}

LOCAL void sTextOutNumber(HCONTROL hControl, BYTE *szBuffer, UINT8 nPos, UINT8 nLen)
{
    UINT8   nViewLen, nDispPos, nStart, nCursorX, nCursorY;
    CONST SIZE* pFontSize;
    UINT8 nTmpLen;
    INT8    i;
    BYTE    szTmpBuffer[NE_MAX_SIZE];
    COLOR OldTextBackColor, OldTextColor;
    DRAW_MODE OldDrawMode;

	OldTextColor = EQS_GetTextColor();
	OldTextBackColor = EQS_GetTextBackColor();
	if( pNumberEdit->ForeColor != MAX_COLOR )
	{
		EQS_SetTextColor(pNumberEdit->ForeColor);
	}
	if( pNumberEdit->BackColor != MAX_COLOR )
	{
		EQS_SetTextBackColor(pNumberEdit->BackColor);
	}
    nViewLen = pNumberEdit->nChar * pNumberEdit->nLine;

    if(pNumberEdit->fBigFont)
    {
        pFontSize = EQS_GetFontSize(FT_BIGBIG, FK_ASCII);
        EQS_SetFontType(FT_BIGBIG);
    }
    else
    {
        pFontSize = EQS_GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);
        EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));
    }

    if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_PASSWD )
    {
        memset(szTmpBuffer, '*', (UINT8)STRLEN(pNumberEdit->szNumber));
    }

    if(nLen >= nViewLen) 
    {
        if((nPos - nViewLen) < 0)   
            nStart = 0;
        else
        {
            if(!(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_RIGHT2LEFT) &&
                !pNumberEdit->fBigFont  &&
                nPos < pNumberEdit->nMaxLen)
                nStart = nPos - nViewLen +1;
            else
                nStart = nPos - nViewLen;
        }
    }
    else
    {
        nStart = 0;
    }

    if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_RIGHT2LEFT )
    {
        nDispPos = nStart + nViewLen;
        if(nDispPos > nLen)
            nDispPos = nLen;

        // Initial Cursor Postion
        nCursorX = pNumberEdit->nChar;
        nCursorY = pNumberEdit->nLine-1;

        for(i = pNumberEdit->nLine-1; i >= 0; i--)
        {
            if(nDispPos == 0)
                break;

            if(nDispPos - pNumberEdit->nChar < 0)
                nTmpLen = nDispPos;
            else
                nTmpLen = pNumberEdit->nChar;

            nDispPos -= nTmpLen;
#if 0
            if(pNumberEdit->fBigFont)
            {
                OldTextBackColor = EQS_GetTextBackColor();
                if(pNumberEdit->BackColor == MAX_COLOR)
                    EQS_SetTextBackColor(EQS_GetTextBackColor());
                else
                    EQS_SetTextBackColor(pNumberEdit->BackColor);
            }
#endif

            if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_PASSWD )
            {
                OldDrawMode = EQS_GetDrawMode();
                if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING)
                    EQS_SetDrawMode(DM_OR);
                EQS_TextOutStrLen((COORD)((pNumberEdit->nChar - nTmpLen) 
                                           * pFontSize->xWidth), 
                                  (COORD)(i * (pFontSize->yHeight)),
                                  &(szTmpBuffer[nDispPos]),
                                  nTmpLen);
                EQS_SetDrawMode(OldDrawMode);
            }
            else
            {
                OldDrawMode = EQS_GetDrawMode();
                if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING)
                    EQS_SetDrawMode(DM_OR);
                EQS_TextOutStrLen((COORD)((pNumberEdit->nChar - nTmpLen) 
                                           * pFontSize->xWidth), 
                                  (COORD)(i * (pFontSize->yHeight)),
                                  &(szBuffer[nDispPos]),
                                  nTmpLen);
                EQS_SetDrawMode(OldDrawMode);
            }
#if 0
            if(pNumberEdit->fBigFont)
                EQS_SetTextBackColor(OldTextBackColor);
#endif

            if(nDispPos <= nPos && nPos <= nDispPos+nTmpLen)
            {
                nCursorX = (pNumberEdit->nChar - nTmpLen) + (nPos - nDispPos) ;
                nCursorY = i;
            }
        }
    }
    else
    {
        nDispPos = nStart;

        // Initial Cursor Postion
        nCursorX = 0;
        nCursorY = 0;

        for(i = 0; i < pNumberEdit->nLine; i++)
        {
            if(nDispPos >= nLen)  break;

            if(nDispPos + pNumberEdit->nChar > nLen)
                nTmpLen = nLen - nDispPos;
            else
                nTmpLen = pNumberEdit->nChar;
#if 0
            if(pNumberEdit->fBigFont)
            {
                OldTextBackColor = EQS_GetTextBackColor();
                if(pNumberEdit->BackColor == MAX_COLOR)
                    EQS_SetTextBackColor(EQS_GetTextBackColor());
                else
                    EQS_SetTextBackColor(pNumberEdit->BackColor);
            }
#endif
            if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_PASSWD )
            {
                OldDrawMode = EQS_GetDrawMode();
                if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING)
                    EQS_SetDrawMode(DM_OR);
                EQS_TextOutStrLen(0, (COORD)(i * (pFontSize->yHeight)),
                                  &(szTmpBuffer[nDispPos]),
                                  nTmpLen);
                EQS_SetDrawMode(OldDrawMode);
            }
            else
            {
                OldDrawMode = EQS_GetDrawMode();
                if(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_ORING)
                    EQS_SetDrawMode(DM_OR);
                EQS_TextOutStrLen(0, (COORD)(i * (pFontSize->yHeight)),
                                  &(szBuffer[nDispPos]),
                                  nTmpLen);
                EQS_SetDrawMode(OldDrawMode);
            }

#if 0
            if(pNumberEdit->fBigFont)
                EQS_SetTextBackColor(OldTextBackColor);
#endif

            if(nDispPos <= nPos && nPos <= nDispPos+nTmpLen)
            {
                nCursorX = nPos - nDispPos;
                nCursorY = i;
            }

            nDispPos += nTmpLen;
        }
    }

    // Draw Cursor
    if(pNumberEdit->hCursorTimer != NULL)
    {
        if(nCursorX == pNumberEdit->nChar)
        {
            if(STRLEN(pNumberEdit->szNumber) == 0)
            {
                nCursorX --;
                pNumberEdit->Cursor.bChar = ' ';
            }
            else if(!(EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_RIGHT2LEFT) &&
                      !pNumberEdit->fBigFont  &&
                        nPos == nLen && nPos < pNumberEdit->nMaxLen)
            {
                pNumberEdit->Cursor.bChar = ' ';

                if(pNumberEdit->nLine >1)
                {
                    nCursorX = 0;
                    nCursorY++;
                }
            }
            else
                pNumberEdit->Cursor.bChar = '\0';
        }
        else
        {
            pNumberEdit->Cursor.bChar = pNumberEdit->szNumber[pNumberEdit->nPos];
            if(STRLEN(pNumberEdit->szNumber) == pNumberEdit->nPos)
                pNumberEdit->Cursor.bChar = ' ';
        }

        pNumberEdit->Cursor.x = 
            (COORD)(nCursorX * (pFontSize->xWidth));
        pNumberEdit->Cursor.y = (COORD)((nCursorY) * pFontSize->yHeight);

        pNumberEdit->Cursor.fCursorReverse = TRUE;

        sDrawCursor(hControl); 
    }
	EQS_SetTextColor(OldTextColor);
	EQS_SetTextBackColor(OldTextBackColor);
}

// Draw Dial 
LOCAL void sDrawNumber(HCONTROL hControl)
{
    UINT8       nHypenedLen, nPos;
    BYTE        szTmpStr[NE_MAX_SIZE+2];

    if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_PLAIN_NUM )
    {
        sTextOutNumber(hControl, pNumberEdit->szNumber,
              pNumberEdit->nPos,
              (UINT8)STRLEN(pNumberEdit->szNumber));
    }
    else
    {
        nHypenedLen = MakeHypenedNumber(pNumberEdit->szNumber, szTmpStr,
                           (UINT8)STRLEN(pNumberEdit->szNumber), 
                           pNumberEdit->nCodePos, 
                           pNumberEdit->nHypenPos);

        nPos = pNumberEdit->nPos;

        if(pNumberEdit->nCodePos > 0
            && pNumberEdit->nPos >= pNumberEdit->nCodePos)    nPos++;

        if(pNumberEdit->nHypenPos > 0
            && pNumberEdit->nPos >= pNumberEdit->nHypenPos)    nPos++;

        sTextOutNumber(hControl, szTmpStr, nPos, nHypenedLen);
    }
}

LOCAL UINT8 sAddDigit(HCONTROL hControl, BYTE bDigit)
{
    UINT8   nSize;
    BYTE    szTmpBuffer[NE_MAX_SIZE];

    nSize = (UINT8)STRLEN(pNumberEdit->szNumber);

    if( STRLEN(pNumberEdit->szNumber) < pNumberEdit->nMaxLen )
	{
        if( pNumberEdit->nPos < nSize )     // Insert
        {
            STRNCPY(szTmpBuffer, 
                    pNumberEdit->szNumber, 
                    (UINT16)(pNumberEdit->nPos));
        
            szTmpBuffer[pNumberEdit->nPos] = bDigit;

            STRNCPY(&szTmpBuffer[pNumberEdit->nPos+1], 
                    &pNumberEdit->szNumber[pNumberEdit->nPos], 
                    (UINT16)(nSize - pNumberEdit->nPos));

            nSize++;
            szTmpBuffer[nSize] = NULL;
            STRCPY(pNumberEdit->szNumber, szTmpBuffer);

            pNumberEdit->nPos ++;
        }
        else
        {
	        pNumberEdit->szNumber[nSize] = bDigit;  
		    nSize++;
            pNumberEdit->szNumber[nSize] = NULL;

            pNumberEdit->nPos ++;
	    }
    }

    pNumberEdit->nCodePos = FindCodeSet(pNumberEdit->szNumber);
    pNumberEdit->nHypenPos = FindHypen(pNumberEdit->szNumber, 
                            pNumberEdit->nCodePos, MAX_DIAL_NUMBER_LEN);
    
    EQC_SET_DIRTY(hControl, TRUE);
    return nSize;
}

LOCAL UINT8 sDeleteDigit(HCONTROL hControl)
{
    UINT8   nSize;
    BYTE    szTmpBuffer[NE_MAX_SIZE];

    nSize = (UINT8)STRLEN(pNumberEdit->szNumber);

    if(pNumberEdit->nPrefix > 0)
    {
        if(pNumberEdit->nPos <= pNumberEdit->nPrefix && pNumberEdit->nPos >= nSize)
            return nSize;
        if(pNumberEdit->nPos < pNumberEdit->nPrefix && pNumberEdit->nPos < nSize)
            return nSize;
    }

    if(nSize > 0)
    {
        if( pNumberEdit->nPos < nSize )     
        {
            STRNCPY(szTmpBuffer, 
                    pNumberEdit->szNumber, 
                    (UINT16)(pNumberEdit->nPos));
        
            STRNCPY(&szTmpBuffer[pNumberEdit->nPos], 
                    &pNumberEdit->szNumber[pNumberEdit->nPos+1], 
                    (UINT16)(nSize - pNumberEdit->nPos -1));

            nSize--;
            szTmpBuffer[nSize] = NULL;
            STRCPY(pNumberEdit->szNumber, szTmpBuffer);
        }
        else
        {
            nSize--;
            pNumberEdit->szNumber[nSize] = NULL;
            pNumberEdit->nPos --;
        }
    }

    pNumberEdit->nCodePos = FindCodeSet(pNumberEdit->szNumber);
    pNumberEdit->nHypenPos = FindHypen(pNumberEdit->szNumber, 
                            pNumberEdit->nCodePos, MAX_DIAL_NUMBER_LEN);

    EQC_SET_DIRTY(hControl, TRUE);

    return nSize;
}

LOCAL void sMoveLeft(HCONTROL hControl)
{
    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(pNumberEdit->nPrefix > 0 && pNumberEdit->nPos <= pNumberEdit->nPrefix)
        return;

    if(pNumberEdit->nPos > 0)
        pNumberEdit->nPos --;
}

LOCAL void sMoveRight(HCONTROL hControl)
{
    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(pNumberEdit->nPos < STRLEN(pNumberEdit->szNumber))
        pNumberEdit->nPos ++;

}
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

/*
** ***************************************************************************
** Control Handling Part
** ***************************************************************************
*/
HCONTROL EQC_NumberEdit(COORD x, COORD y, UINT8 nChar, UINT8 nLine, UINT8 nMaxLen,
                         BYTE* pszInitNum )
{
	void  *pInstance;
	HCONTROL hControl;
    SIZE  FontSize;
    SIZE  dispSize;
    UINT8 i;
	
	pInstance = sAllocInstance();

    if(nMaxLen > NE_MAX_SIZE)
        nMaxLen = NE_MAX_SIZE;

    FontSize.xWidth = EQS_CharWidth('A');
    FontSize.yHeight = EQS_CharHeight('A');

    dispSize.xWidth = (nChar * FontSize.xWidth) ;
    dispSize.yHeight = (nLine * (FontSize.yHeight)); 

    if(dispSize.xWidth > EQS_GetMaxWidth())
    {
        dispSize.xWidth = EQS_GetMaxWidth();
        nChar = (dispSize.xWidth-1) / FontSize.xWidth;
    }
    if(dispSize.yHeight > EQS_GetMaxHeight())
    {
        dispSize.yHeight = EQS_GetMaxHeight();
        nLine = dispSize.yHeight / (FontSize.yHeight+1);
    }

	hControl = EQC_AllocControl(CT_NUMBER_EDIT, pInstance, x, y, 
                                dispSize.xWidth, dispSize.yHeight,
                                sHandleEvent);

	ASSERT(hControl != INVALID_CONTROL_HANDLE);

    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);
    EQC_SET_STYLE(hControl, CS_NUMBER_EDIT_DEFAULTS);
	EQC_SET_DIRTY(hControl, FALSE);

    // put attached I/O Control list of current applet.
    pNumberEdit = (NUMBER_EDIT_T *)pInstance;

    // Clear Number Edit value..
    pNumberEdit->nChar = nChar;
    pNumberEdit->nLine = nLine;
    pNumberEdit->nMaxLen = nMaxLen;
    pNumberEdit->fMatchFlag = FALSE;
    pNumberEdit->hCursorTimer = NULL;
    pNumberEdit->fActive = FALSE;
    pNumberEdit->nCodePos = 0;
    pNumberEdit->nHypenPos = 0;
    for(i = 0; i < sMAX_MATCH_NUMBER; i++)
        pNumberEdit->paszMatchCond[i] = NULL;
    pNumberEdit->szLocalCode = NULL;
    pNumberEdit->fBigFont = FALSE;
    pNumberEdit->szNumber = (BYTE*)EQS_Malloc(pNumberEdit->nMaxLen+1);
    pNumberEdit->BackColor = MAX_COLOR;
	pNumberEdit->ForeColor = MAX_COLOR;
    pNumberEdit->nPrefix = 0;
	pNumberEdit->nPos    = 0; //redstar 01.08.27
    // Init Cursor
    pNumberEdit->Cursor.fCursorReverse = TRUE;

	EQC_SetNumberEditValue(hControl, pszInitNum );

	if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
	{
		EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
	}
	else
	{
		EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
	}
	return hControl;

}

 

/*
** ***************************************************************************
** Control Property Handling Part
** ***************************************************************************
*/

UINT8 EQC_GetNumberEditValue( HCONTROL hControl, BYTE *pszBuf )
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

    STRCPY( pszBuf, pNumberEdit->szNumber );

	return (UINT8)STRLEN(pNumberEdit->szNumber);
}

UINT8 EQC_GetLocalCodeLength(HCONTROL hControl)
{
    NUMBER_EDIT_T *pNumberEdit;

    pNumberEdit = (NUMBER_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(pNumberEdit->szLocalCode != NULL)
        return STRLEN(pNumberEdit->szLocalCode);
    else
        return 0;
}

EQS_RESULT EQC_SetNumberEditValue( HCONTROL hControl, BYTE* pszNewDialNum )
{ 
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

	if(STRLEN(pszNewDialNum) > pNumberEdit->nMaxLen)
	{
		STRNCPY( pNumberEdit->szNumber, pszNewDialNum, pNumberEdit->nMaxLen);
		pNumberEdit->szNumber[pNumberEdit->nMaxLen] = '\0';
	}
	else
	{
		STRCPY( pNumberEdit->szNumber, pszNewDialNum);
	}

    pNumberEdit->nCodePos = FindCodeSet(pNumberEdit->szNumber);
    pNumberEdit->nHypenPos = FindHypen(pNumberEdit->szNumber, 
                        pNumberEdit->nCodePos, MAX_DIAL_NUMBER_LEN);

    if(pNumberEdit->fActive)
    {
        if(szSaveNumber)
            STRCPY( szSaveNumber, pszNewDialNum);

        pNumberEdit->nPos = (UINT8)STRLEN(pNumberEdit->szNumber);
    }

	if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
	{
		EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
	}
	else
	{
		EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
	}

	return RESULT_OK;
}

EQS_RESULT EQC_SetNumberEditPrefix( HCONTROL hControl, UINT16 nPrefix )
{ 
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

    pNumberEdit->nPrefix = nPrefix;

	return RESULT_OK;
}

void EQC_NotifyNumberEditMatch( HCONTROL hControl, BYTE* pszMatchNum, UINT8 nIndex )
{ 
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    ASSERT(nIndex < sMAX_MATCH_NUMBER);

    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

	if(STRLEN(pszMatchNum) > 0 )
	{
        pNumberEdit->paszMatchCond[nIndex] = (BYTE*)EQS_Malloc((SIZE_T)(STRLEN(pszMatchNum)+1));
        STRCPY( pNumberEdit->paszMatchCond[nIndex], pszMatchNum);
        pNumberEdit->fMatchFlag = TRUE;
	}
}

void EQC_NotifyNumberEditMatchLen( HCONTROL hControl, UINT8 nLength )
{ 
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

	if(nLength > 0 )
	{
        pNumberEdit->nNotifyLen = nLength;
	}
}

void EQC_SetLocalAreaCode( HCONTROL hControl, BYTE* pszLocalCode)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);

    if(STRLEN(pszLocalCode) > 0)
    {
        pNumberEdit->szLocalCode = (BYTE*)EQS_Malloc((SIZE_T)(STRLEN(pszLocalCode)+1));
        STRCPY( pNumberEdit->szLocalCode, pszLocalCode);

        STRCPY( pNumberEdit->szNumber, pszLocalCode);
        pNumberEdit->nCodePos = FindCodeSet(pNumberEdit->szNumber);
        pNumberEdit->nHypenPos = FindHypen(pNumberEdit->szNumber, 
                            pNumberEdit->nCodePos, MAX_DIAL_NUMBER_LEN);
        pNumberEdit->nPos = STRLEN(pszLocalCode);

		if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
		{
			EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
		}
		else
		{
			EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
		}

    }
}

void EQC_SetNumberEditBigFont( HCONTROL hControl, BOOL fSet )
{
    CONST SIZE  *pFontSize;
    SIZE  dispSize;

    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);
 
    pNumberEdit->fBigFont = fSet;

    if(fSet)
    {
        pFontSize = EQS_GetFontSize(FT_BIGBIG, FK_ASCII);
    }
    else
    {
        pFontSize = EQS_GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);
    }

    dispSize.xWidth = (pNumberEdit->nChar * pFontSize->xWidth);
    dispSize.yHeight = (pNumberEdit->nLine * pFontSize->yHeight);      // include cursor line

    if(dispSize.xWidth > EQS_GetMaxWidth())
    {
        dispSize.xWidth = EQS_GetMaxWidth();
        pNumberEdit->nChar = dispSize.xWidth / pFontSize->xWidth;
    }
    if(dispSize.yHeight > EQS_GetMaxHeight())
    {
        dispSize.yHeight = EQS_GetMaxHeight();
        pNumberEdit->nLine = dispSize.yHeight / pFontSize->yHeight;
    }

    EQC_SET_DX(hControl, dispSize.xWidth);
    EQC_SET_DY(hControl, dispSize.yHeight);

	if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
	{
		EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
	}
	else
	{
		EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
	}
}

void EQC_SetNumberEditBackColor(HCONTROL hControl, COLOR backColor)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);
 
    pNumberEdit->BackColor = backColor;
}

void EQC_SetNumberEditColor(HCONTROL hControl, COLOR ForeColor)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);
 
    pNumberEdit->ForeColor = ForeColor;
}

UINT16 EQC_GetNumberEditPos(HCONTROL hControl)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);
 
    return pNumberEdit->nPos;
}

void EQC_SetNumberEditPos(HCONTROL hControl, UINT16 nPos)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_NUMBER_EDIT);
    pNumberEdit = (NUMBER_EDIT_T *)EQC_GET_INSTANCE(hControl);
 
    pNumberEdit->nPos = nPos;

	if( EQC_GET_STYLE(hControl) & CS_NUMBER_EDIT_DRAW_ALL_CONTROL )
	{
		EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
	}
	else
	{
		EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
	}
}