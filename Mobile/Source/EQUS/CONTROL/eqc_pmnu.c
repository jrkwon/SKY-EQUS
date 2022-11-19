/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_pmnu.c
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
** 00-10-27   woodstok      Created.
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <stdio.h>
#include "eqs.h"

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

#define	POPUP_MENU_SPACING	    	2
#define VIEW_HEIGHT 7
#define VIEW_WIDTH 17 // 16 ascii char. width
#define MAX_TITLE_STRING    VIEW_WIDTH
#define MAX_STATUS_STRING    VIEW_WIDTH

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

enum  tagPOPUP_MENU_DRAW_TYPE_E
{
	PMN_DRAW,
	PMN_NORMAL,	
	PMN_TOP_DOWN,
	PMN_SCROLL,
	PMN_REDRAW
};
typedef enum tagPOPUP_MENU_DRAW_TYPE_E  POPUP_MENU_DRAW_TYPE_E;


typedef struct tagPOPUP_MENU_T     POPUP_MENU_T;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

struct tagPOPUP_MENU_T
{
    CONTROL_HEADER_T    Header;

	UINT8 nIndex;
	UINT8 nCursor;
    UINT8 nLine;

	BYTE** ppList;
    BOOL*  pExceptList;
    BYTE *pszTitle;
    BYTE szStringTitle[MAX_TITLE_STRING+1];
    BYTE szStatus[MAX_TITLE_STRING+1];

    EQS_EVENT_ID *pHotKey;
    EQS_TOKEN_EX *pToken;

    /* jrkwon 2001-02-17 */
    WINDOW              *pWindow;
    BOOL                fDarken;

	/* redstar 2001-08-28 */
	POPUP_MENU_DRAW_TYPE_E DrawType;
	UINT8                  nPreviousIndex;

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
//LOCAL l_fDarken = FALSE;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* sAllocPopUpMenuInstance(EQS_TOKEN_EX* paItem, UINT8 nLine);
LOCAL void sDrawPopUpMenu(HCONTROL hControl);
LOCAL void sMakePopupItem(HCONTROL hControl, BYTE *pszStr, 
                          POPUP_MENU_T *pPopUpHeader, UINT8 nIndex);
LOCAL BOOL sIsAllDisable(POPUP_MENU_T *pPopUpHeader);            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
	
LOCAL void* sAllocPopUpMenuInstance(EQS_TOKEN_EX* paItem, UINT8 nLine)
{
	UINT8 i;
	void *pMem;
    POPUP_MENU_T *pPopUpHeader;

	pMem = EQS_Malloc(sizeof(POPUP_MENU_T));
    ASSERT(pMem != 0);

    pPopUpHeader = (POPUP_MENU_T *)pMem;

	pPopUpHeader->ppList = 
        (BYTE **)EQS_Malloc(nLine * sizeof(BYTE*));
	pPopUpHeader->pHotKey = 
        (EQS_EVENT_ID *)EQS_Malloc(nLine * sizeof(EQS_EVENT_ID));
    pPopUpHeader->pToken =
        (EQS_TOKEN_EX *)EQS_Malloc(nLine * sizeof(EQS_TOKEN_EX));

    pPopUpHeader->pWindow = (WINDOW *)EQS_Malloc(sizeof(WINDOW));

    for (i = 0; i < nLine; i++)
    {
        (pPopUpHeader->ppList)[i] = (BYTE*)EQS_GET_TOKEN_EX(paItem[i]);
        (pPopUpHeader->pToken)[i] = paItem[i];

        if(i < 9)
            (pPopUpHeader->pHotKey)[i] = (EQS_EVENT_ID)(i + EQE_KEY_1);
        else
            (pPopUpHeader->pHotKey)[i] = EQE_KEY_NONE;
    }

    pPopUpHeader->nLine = nLine;
	pPopUpHeader->nPreviousIndex = 0;
	pPopUpHeader->DrawType = PMN_DRAW;

	return (void *)pMem;
}

LOCAL void  EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

	if ( ((POPUP_MENU_T *)EQC_GET_INSTANCE(hControl))->ppList != 0 )
		EQS_Free((void**)&((POPUP_MENU_T *)EQC_GET_INSTANCE(hControl))->ppList);

	if ( ((POPUP_MENU_T *)EQC_GET_INSTANCE(hControl))->pHotKey != 0 )
		EQS_Free((void**)&((POPUP_MENU_T *)EQC_GET_INSTANCE(hControl))->pHotKey);

    EQS_Free((void**)&((POPUP_MENU_T *)EQC_GET_INSTANCE(hControl))->pWindow);
    EQS_Free((void**)&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);

    if(EQS_GetModal() == hControl)
        EQS_EndModalIO();   // will cause EQE_APP_DRAW 

//    l_fDarken = FALSE;
}

LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	POPUP_MENU_T *pPopUpHeader;
    UINT8       i, nViewHeight;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    if(pPopUpHeader->nLine > VIEW_HEIGHT)
        nViewHeight = VIEW_HEIGHT;
    else
        nViewHeight = pPopUpHeader->nLine;

	switch (pEvent->EventID)
	{
	case EQE_APP_DRAW:
        if(pPopUpHeader->fDarken == FALSE)
        {
            /* jrkwon 2001-02-17 make background screen darker */
            MakeScreenDarker(BG_DARK_COUNT);			
        }		
				
		sDrawPopUpMenu(hControl);
		
		break;

	case EQE_KEY_UP:
        if(sIsAllDisable(pPopUpHeader))
        {
            EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl, 0);
            sFreeInstance(hControl);
            break;
        }
		pPopUpHeader->nPreviousIndex = pPopUpHeader->nIndex;
        do
        {
		    if (pPopUpHeader->nCursor > 0)			
			    pPopUpHeader->nCursor--;

		    if (pPopUpHeader->nIndex == 0)
            {
                pPopUpHeader->nCursor = nViewHeight-1;//(pPopUpHeader->nLine - (VIEW_HEIGHT-1))%VIEW_HEIGHT;
			    pPopUpHeader->nIndex = pPopUpHeader->nLine - 1; 
            }
		    else
            {
			    pPopUpHeader->nIndex--;
            }
        }
        while(pPopUpHeader->pExceptList[pPopUpHeader->nIndex]);
		
		if( pPopUpHeader->nPreviousIndex != pPopUpHeader->nIndex )
		{
            if( (pPopUpHeader->nPreviousIndex - pPopUpHeader->nIndex) == 1)
			{
				pPopUpHeader->DrawType = PMN_NORMAL;
			}
			else
			{
				pPopUpHeader->DrawType = PMN_TOP_DOWN;
			}
			EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
			EQS_SendEvent(EQE_IO_POPUP_CHG, hControl, (DWORD)(pPopUpHeader->nIndex));			
		}
		break;

	case EQE_KEY_DOWN:
        if(sIsAllDisable(pPopUpHeader))
        {
            EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl, 0);
            sFreeInstance(hControl);
            break;
        }
		pPopUpHeader->nPreviousIndex = pPopUpHeader->nIndex;
        do
        {
		    if (pPopUpHeader->nCursor < nViewHeight - 1)
			    pPopUpHeader->nCursor++;

		    if (pPopUpHeader->nCursor > pPopUpHeader->nLine - 1)
			    pPopUpHeader->nCursor = pPopUpHeader->nLine - 1;

		    if (pPopUpHeader->nIndex >= pPopUpHeader->nLine - 1)
            {
			    pPopUpHeader->nIndex = 0; // wrap around
                pPopUpHeader->nCursor = 0;
            }
		    else
            {
			    pPopUpHeader->nIndex++;
            }
        }
        while(pPopUpHeader->pExceptList[pPopUpHeader->nIndex]);

		if( pPopUpHeader->nPreviousIndex != pPopUpHeader->nIndex )
		{
            if( (pPopUpHeader->nIndex - pPopUpHeader->nPreviousIndex) == 1 )
			{
				pPopUpHeader->DrawType = PMN_NORMAL;
			}
			else
			{
				pPopUpHeader->DrawType = PMN_TOP_DOWN;
			}
			EQS_SendEvent(EQE_APP_DRAW, EQS_NA, 0);
			EQS_SendEvent(EQE_IO_POPUP_CHG, hControl, (DWORD)(pPopUpHeader->nIndex));
		}		
		break;

    case EQE_KEY_0:
    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
        if(sIsAllDisable(pPopUpHeader))
        {
            EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl, 0);
            sFreeInstance(hControl);
            break;
        }

        for( i = 0 ; i < pPopUpHeader->nLine; i++)
        {
            if(pPopUpHeader->pHotKey[i] == pEvent->EventID && !pPopUpHeader->pExceptList[i])
            {
                if(EQC_GET_STYLE(hControl) & CS_POPUP_RETURN_TOKEN)
                    EQS_SendEvent(EQE_IO_POPUP_SELECT, hControl, (DWORD)(pPopUpHeader->pToken[i]));
                else
                    EQS_SendEvent(EQE_IO_POPUP_SELECT, hControl, (DWORD)(i));
                sFreeInstance(hControl);
                break;
            }
        }
        break;

    case EQE_KEY_CENTER:
        if(sIsAllDisable(pPopUpHeader))
        {
            EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl, 0);
            sFreeInstance(hControl);
            break;
        }

        if(EQC_GET_STYLE(hControl) & CS_POPUP_RETURN_TOKEN)
            EQS_SendEvent(EQE_IO_POPUP_SELECT, hControl, (DWORD)(pPopUpHeader->pToken[pPopUpHeader->nIndex]));
        else
            EQS_SendEvent(EQE_IO_POPUP_SELECT, hControl, (DWORD)(pPopUpHeader->nIndex));		
        sFreeInstance(hControl);
        break;

    case EQE_KEY_F1:
//        if (EQC_GET_STYLE(hControl) & CS_POPUP_NO_CML)
//            break;
    case EQE_KEY_CLEAR:
//    case EQE_KEY_END:
        // Go Through..
    case EQE_IO_DELETE:

		if( pEvent->wParam == hControl && pEvent->EventID == EQE_IO_DELETE)
             EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl,EQC_DELETED);
		else
             EQS_SendEvent(EQE_IO_POPUP_CANCEL, (WORD)hControl, 0);
				
        sFreeInstance(hControl);
        break;

	}
}

LOCAL BOOL sIsAllDisable(POPUP_MENU_T *pPopUpHeader)
{
    UINT8 i;

    for(i = 0; i < pPopUpHeader->nLine; i++)
    {
        if(pPopUpHeader->pExceptList[i] == FALSE)
            return FALSE;
    }

    return TRUE;
}


LOCAL void sMakePopupItem(HCONTROL hControl, BYTE *pszStr, 
                          POPUP_MENU_T *pPopUpHeader, UINT8 nIndex)
{

	sprintf((char*)pszStr,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex);

    if(IsMiddleOfMultiByte((pPopUpHeader->ppList)[nIndex], VIEW_WIDTH-2))
        memcpy(&pszStr[2], (BYTE*)((pPopUpHeader->ppList)[nIndex]), VIEW_WIDTH-3);
    else
        memcpy(&pszStr[2], (BYTE*)((pPopUpHeader->ppList)[nIndex]), VIEW_WIDTH-2);

    pszStr[VIEW_WIDTH] = '\0';
}

LOCAL RECT sDrawWindow(HCONTROL hControl, COORD xWidth, COORD yHeight)                     
{
    POPUP_MENU_T *pPopupMenu;
    COORD    xPos, yPos, xTitleWidth;
    RECT    Rect;
    BYTE    pszTmp[25];
	UINT8   nWidth, nHeight;

    pPopupMenu = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

	nWidth  = EQS_CharWidth((BYTE)' ');
	nHeight = EQS_CharHeight((BYTE)' ');

    // Draw Window
    pPopupMenu->pWindow->Style = WS_BORDER;

    if(EQC_GET_STYLE(hControl) & CS_POPUP_TITLE)
    {
        if(pPopupMenu->nLine > VIEW_HEIGHT  && !STRLEN(pPopupMenu->szStringTitle))
        {
            INT8    nFrom, nTo;
            
            nFrom = pPopupMenu->nIndex - pPopupMenu->nCursor;
            nTo = pPopupMenu->nIndex + (VIEW_HEIGHT - pPopupMenu->nCursor);

            if(nFrom > 0 && nTo < pPopupMenu->nLine)
                sprintf((char*)pszTmp, SF_BRIGHT_SPIN"    %s      ", (char*)pPopupMenu->pszTitle); 
            else if(nFrom > 0)
                sprintf((char*)pszTmp, SF_BRIGHT_UPPER_TRIANGLE"    %s      ", (char*)pPopupMenu->pszTitle); 
            else if(nTo < pPopupMenu->nLine)
                sprintf((char*)pszTmp, SF_BRIGHT_LOWER_TRIANGLE"    %s      ", (char*)pPopupMenu->pszTitle); 
        }
        else if(STRLEN(pPopupMenu->szStringTitle))
        {
            STRCPY(pszTmp, pPopupMenu->szStringTitle);
        }
        else
        {
            STRCPY(pszTmp, pPopupMenu->pszTitle);
        }

        pPopupMenu->pWindow->pszCaption = pszTmp;
        pPopupMenu->pWindow->Style |= WS_CAPTION;

        xTitleWidth = (STRLEN(pPopupMenu->pszTitle) * nWidth) + (POPUP_MENU_SPACING * 2);

        if(xTitleWidth > xWidth)
            xWidth = xTitleWidth;
    }

    xPos = (DevGetMaxX() - xWidth)/2;
    if(EQC_GET_STYLE(hControl) & CS_POPUP_TITLE)
        yPos = (DevGetMaxY() - yHeight + (nHeight + (POPUP_MENU_SPACING * 2)) )/2;
    else
        yPos = (DevGetMaxY() - yHeight)/2;

    SetRect(&Rect, xPos, yPos, (COORD)(xPos+xWidth), (COORD)(yPos+yHeight)); 

	EQC_SET_X(hControl, xPos);
	EQC_SET_DX(hControl, xWidth);
	if(EQC_GET_STYLE(hControl) & CS_POPUP_TITLE)
	{
		EQC_SET_Y(hControl, yPos-nHeight-1);
		EQC_SET_DY(hControl, yHeight+nHeight+1);
	}
	else
	{
		EQC_SET_Y(hControl, yPos);
		EQC_SET_DY(hControl, yHeight);
	}
	
    EQS_OpenWindow(pPopupMenu->pWindow, &Rect, FALSE);

    return Rect;
}

LOCAL void sDrawPopUpText(HCONTROL hControl, UINT8 XPos, UINT8 YPos, UINT8 nPos, UINT8 nIndex, BOOL fReverse)
{
	POPUP_MENU_T *pPopUpHeader;
    BYTE    pszTmpStr[VIEW_WIDTH];    
    COLOR OldTextBackColor, OldTextColor;
	UINT8   nHeight;

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);
    //pFontSize = EQS_GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);
	nHeight   = EQS_CharHeight((BYTE)' ');

	sMakePopupItem(hControl, pszTmpStr, pPopUpHeader, nIndex);

	OldTextBackColor = EQS_GetTextBackColor();
	OldTextColor = EQS_GetTextColor();
	if ( fReverse )
	{
		// REVER

		EQS_SetTextBackColor(POPUP_SELECT_BACK_COLOR);//BACK_COLOR);
		EQS_SetTextColor(POPUP_SELECT_FORE_COLOR);//FORE_COLOR);
		//EQS_SetTextBackColor((COLOR)1);//BLUE);//WHITE);
		//EQS_SetTextColor(WHITE);//BLACK);

		/*EQS_TextOutStrLenTDM(xPos, yPos + i * pFontSize->yHeight,
			 pszTmpStr,
			 VIEW_WIDTH, TDM_BOLD);*/
		EQS_TextOutStrLen(XPos, YPos + nPos * nHeight,
			 pszTmpStr,
			 VIEW_WIDTH);
		EQS_SetTextBackColor(OldTextBackColor);
		EQS_SetTextColor(OldTextColor);
		//EQS_Line(xPos, yPos + i * pFontSize->yHeight+CharHeight(' ')-1, xPos + VIEW_WIDTH*CharWidth(' ')-1, yPos + i * pFontSize->yHeight+CharHeight(' ')-1);
		/*EQS_TextOutStrLenTDM(xPos, yPos + i * pFontSize->yHeight,
			 pszTmpStr,
			 VIEW_WIDTH,
			 TDM_REVERSE);*/
	}
	else
	{
		if(pPopUpHeader->pExceptList[nIndex])    // Disable
		{		
#if 0
			//EQS_SetTextColor(CTRL_DEACTIVE_COLOR);			
#endif
			EQS_SetTextColor(CD_TEXT_FORE_COLOR);
			EQS_SetTextBackColor(POPUP_NORMAL_BACK_COLOR);//CD_TEXT_BACK_COLOR);

    		EQS_TextOutStrLen(XPos, YPos + nPos * nHeight,
				pszTmpStr,
				VIEW_WIDTH);
		}
		else
		{
			EQS_SetTextBackColor(POPUP_NORMAL_BACK_COLOR);
			EQS_SetTextColor(POPUP_NORMAL_FORE_COLOR);
    		EQS_TextOutStrLen(XPos, YPos + nPos * nHeight,
				pszTmpStr,
				VIEW_WIDTH);
		}
	
	}
	EQS_SetTextBackColor(OldTextBackColor);
	EQS_SetTextColor(OldTextColor);

}

LOCAL void sDrawPopUpMenu(HCONTROL hControl)
{
	UINT8 i;
	UINT8 uDrawIndx, nCursor, xPos, yPos, nViewHeight, nHeight, nWidth;
	POPUP_MENU_T *pPopUpHeader;
    BYTE    pszTmpStr[VIEW_WIDTH];
    CONST SIZE* pFontSize;
    COLOR OldTextBackColor, OldTextColor;

    EQS_GetDC(TRUE);
	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);
    //FontSize = EQS_GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);
	nWidth  = EQS_CharWidth((BYTE)' ');
	nHeight = EQS_CharHeight((BYTE)' ');

	
    if(pPopUpHeader->nLine > VIEW_HEIGHT)
        nViewHeight = VIEW_HEIGHT;
    else
        nViewHeight = pPopUpHeader->nLine;

	if( pPopUpHeader->DrawType == PMN_DRAW )
	{
		if (EQC_GET_STYLE(hControl) & CS_POPUP_STATUS)
			sDrawWindow(hControl, (VIEW_WIDTH * nWidth),
						((nViewHeight+1) * nHeight));
		else
			sDrawWindow(hControl, (VIEW_WIDTH * nWidth),
						(nViewHeight * nHeight));
	}

    //EQS_SetTextColor(TEXT_FORE_COLOR); // XXX: Temp
    //EQS_SetDrawMode(DM_OR);
    xPos = (DevGetMaxX() - (VIEW_WIDTH * nWidth))/2;// + 1;
    if (EQC_GET_STYLE(hControl) & CS_POPUP_STATUS)
    {
        if(EQC_GET_STYLE(hControl) & CS_POPUP_TITLE)
            yPos = (DevGetMaxY() - ((nViewHeight+1) * nHeight) 
                    + (nHeight + (POPUP_MENU_SPACING * 2)) )/2;
        else
            yPos = (DevGetMaxY() - ((nViewHeight+1) * nHeight))/2;
    }
    else
    {
        if(EQC_GET_STYLE(hControl) & CS_POPUP_TITLE)
            yPos = (DevGetMaxY() - (nViewHeight * nHeight) 
                    + (nHeight + (POPUP_MENU_SPACING * 2)) )/2;
        else
            yPos = (DevGetMaxY() - (nViewHeight * nHeight))/2;
    }


	if( pPopUpHeader->DrawType == PMN_TOP_DOWN )
	{			
		nCursor = pPopUpHeader->nCursor + pPopUpHeader->nPreviousIndex - pPopUpHeader->nIndex;
						
		sDrawPopUpText(hControl, xPos, yPos, nCursor, pPopUpHeader->nPreviousIndex ,FALSE);
		EQS_RefreshDisplayRect(hControl, xPos, yPos+nCursor*nHeight, 
				               xPos+EQC_GET_DX(hControl), yPos+(nCursor+1)*nHeight);			

		sDrawPopUpText(hControl, xPos, yPos, pPopUpHeader->nCursor, pPopUpHeader->nIndex , TRUE);
		EQS_RefreshDisplayRect(hControl, xPos, yPos+(pPopUpHeader->nCursor)*nHeight, 
				               xPos+EQC_GET_DX(hControl), yPos+(pPopUpHeader->nCursor+1)*nHeight);
		
	}
	else if( pPopUpHeader->DrawType == PMN_NORMAL )
	{

		if( pPopUpHeader->nPreviousIndex < pPopUpHeader->nIndex )
		{
			sDrawPopUpText(hControl, xPos, yPos, pPopUpHeader->nCursor-1, pPopUpHeader->nPreviousIndex ,FALSE);	
			sDrawPopUpText(hControl, xPos, yPos, pPopUpHeader->nCursor, pPopUpHeader->nIndex , TRUE);
			EQS_RefreshDisplayRect(hControl, xPos, yPos+(pPopUpHeader->nCursor-1)*nHeight, 
				                   xPos+EQC_GET_DX(hControl), yPos+(pPopUpHeader->nCursor+1)*nHeight);
		}
		else
		{
			sDrawPopUpText(hControl, xPos, yPos, pPopUpHeader->nCursor+1, pPopUpHeader->nPreviousIndex ,FALSE);	
			sDrawPopUpText(hControl, xPos, yPos, pPopUpHeader->nCursor, pPopUpHeader->nIndex , TRUE);

			EQS_RefreshDisplayRect(hControl, xPos, yPos+(pPopUpHeader->nCursor)*nHeight, 
				                   xPos+EQC_GET_DX(hControl), yPos+(pPopUpHeader->nCursor+2)*nHeight);
		}
		
		//EQS_UpdateControlDisplay();

	}
	else if( pPopUpHeader->DrawType ==  PMN_DRAW )
	{
		if (nViewHeight >= pPopUpHeader->nLine)
		{
			pPopUpHeader->nCursor = pPopUpHeader->nIndex;

			for (i = 0; i < pPopUpHeader->nLine; i++)
			{
				if (pPopUpHeader->nCursor == i && !sIsAllDisable(pPopUpHeader))
				{
					// REVER
					sDrawPopUpText(hControl, xPos, yPos, i,i,TRUE);				
				}
				else
				{
					sDrawPopUpText(hControl, xPos, yPos, i,i,FALSE);
				}
			}
		}
		else
		{
			uDrawIndx = (pPopUpHeader->nIndex - pPopUpHeader->nCursor + pPopUpHeader->nLine)
						% pPopUpHeader->nLine;
			for (i = 0; i < nViewHeight; i++, uDrawIndx = MOD_INCREMENT(uDrawIndx, pPopUpHeader->nLine))
			{

				if (pPopUpHeader->nCursor == i && !sIsAllDisable(pPopUpHeader))
				{
					// REVER
					sDrawPopUpText(hControl, xPos, yPos, i,uDrawIndx,TRUE);	
				}
				else
				{			
					sDrawPopUpText(hControl, xPos, yPos, i,uDrawIndx,FALSE);	
				}
			}
		}
		if (EQC_GET_STYLE(hControl) & CS_POPUP_STATUS)
		{
			COORD xCenter;
			OldTextBackColor = EQS_GetTextBackColor();
			OldTextColor     = EQS_GetTextColor();
			EQS_SetTextBackColor(LIGHTGRAY);
			EQS_SetTextColor(BLUE);

			EQS_FillRectangle(xPos, yPos +  nViewHeight * nHeight,
							  xPos + VIEW_WIDTH * nWidth, 
							  yPos +  (nViewHeight+1) * nHeight, LIGHTGRAY);

			xCenter = (VIEW_WIDTH - STRLEN(pPopUpHeader->szStatus))/2 * nWidth;

    		EQS_TextOutStrLenTDM(xPos+xCenter, yPos +  nViewHeight * nHeight,
				pPopUpHeader->szStatus,
				STRLEN(pPopUpHeader->szStatus),
				TDM_NORMAL);

			EQS_SetTextBackColor(OldTextBackColor);
			EQS_SetTextColor(OldTextColor);
		}
		if( pPopUpHeader->fDarken == FALSE )
		{
	         pPopUpHeader->fDarken = TRUE;
			 EQS_RefreshDisplayRect(hControl, 0, 0, DevGetMaxX(), DevGetMaxY());
		}
		else
		{
			EQS_UpdateControlDisplay();
		}
	}
	else ;
    //EQS_SetDrawMode(DM_COPY);

	EQS_ReleaseDC();

	pPopUpHeader->DrawType = PMN_DRAW;
       
/*
    if (!(EQC_GET_STYLE(hControl) & CS_POPUP_NO_CML))
    {
        EQS_DrawIOCCML(IOC_CML_POPUP_MENU_CANCEL_LEFT);
    }
*/
}


/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL EQC_PopUpMenuExtend(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex, DWORD dwStyle,
                             WORD nTitleToken, BYTE *pszTitle, BYTE *pszStatus)
{
	POPUP_MENU_T *pPopup;
	HCONTROL hControl;
    UINT8   i;

	if( nLine > VIEW_HEIGHT )
	{
		nLine = VIEW_HEIGHT;
	}
    // girin. 2000.12.19
    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
        EQS_DeleteControl(hControl);
    }
    //
	
	pPopup = (POPUP_MENU_T*)sAllocPopUpMenuInstance((EQS_TOKEN_EX*)paItem, nLine);

	hControl = EQC_AllocControl(CT_POPUP_MENU, (void*)pPopup, 0, 0, 0, 0, 
                                sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

	pPopup->nIndex = nIndex;
	pPopup->nCursor = 0;
    pPopup->pszTitle = 
                (BYTE *)EQS_GET_TOKEN(TKN_POPUP_MENU_TITLE);
    pPopup->fDarken = FALSE;
    pPopup->pExceptList = EQS_Malloc(sizeof(BOOL) * nLine);
    pPopup->szStringTitle[0] = '\0';

    for(i = 0; i < nLine; i++)
        pPopup->pExceptList[i] = FALSE;


    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);
    EQC_SET_STYLE(hControl, dwStyle);

    if(nTitleToken)
    {
        pPopup->pszTitle = EQS_GET_TOKEN_EX(nTitleToken);
    }

    if(pszTitle!= NULL && STRLEN(pszTitle))
    {
	    memset(pPopup->szStringTitle, 0, MAX_TITLE_STRING+1);

        if(STRLEN(pszTitle) > MAX_TITLE_STRING)
        {
	        if( IsMiddleOfMultiByte(pszTitle, MAX_TITLE_STRING-2) )
	        {
		        STRNCPY(pPopup->szStringTitle, pszTitle, MAX_TITLE_STRING -3);
		        STRNCPY(pPopup->szStringTitle+MAX_TITLE_STRING-3, (BYTE*)SF_ETC_SYMBOL,2);
	        }
	        else
	        {
		        STRNCPY(pPopup->szStringTitle, pszTitle, MAX_TITLE_STRING-2);
		        STRNCPY(pPopup->szStringTitle+ MAX_TITLE_STRING-2, (BYTE*)SF_ETC_SYMBOL,2);
	        }
        }
        else
        {
            STRCPY(pPopup->szStringTitle, pszTitle);
        }
    }

    if(pszStatus != NULL && STRLEN(pszStatus))
    {
	    memset(pPopup->szStatus, 0, MAX_TITLE_STRING+1);

        if(STRLEN(pszStatus) > MAX_STATUS_STRING)
        {
	        if( IsMiddleOfMultiByte(pszStatus, MAX_STATUS_STRING-2) )
	        {
		        STRNCPY(pPopup->szStatus, pszStatus, MAX_STATUS_STRING -3);
		        STRNCPY(pPopup->szStatus+MAX_STATUS_STRING-3, (BYTE*)SF_ETC_SYMBOL,2);
	        }
	        else
	        {
		        STRNCPY(pPopup->szStatus, pszStatus, MAX_STATUS_STRING-2);
		        STRNCPY(pPopup->szStatus+ MAX_STATUS_STRING-2, (BYTE*)SF_ETC_SYMBOL,2);
	        }
        }
        else
        {
            STRCPY(pPopup->szStatus, pszStatus);
        }
    }

    EQS_SendEvent(EQE_IO_POPUP_START, hControl, EQS_NA);
    if(EQC_GET_STYLE(hControl) & CS_POPUP_BEEP)
        EQS_SendEvent(EQE_IO_BEEP_SOUND, hControl, EQS_NA);

    EQS_SetIOCActive(hControl);
    EQS_StartModalIO();

    return hControl;
}

HCONTROL EQC_PopUpMenu(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex)
{
	POPUP_MENU_T *pPopup;
	HCONTROL hControl;
    UINT8   i;

	if( nLine > VIEW_HEIGHT )
	{
		nLine = VIEW_HEIGHT;
	}
    // girin. 2000.12.19
    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
        EQS_DeleteControl(hControl);
    }
    //
	
	pPopup = (POPUP_MENU_T*)sAllocPopUpMenuInstance((EQS_TOKEN_EX*)paItem, nLine);

	hControl = EQC_AllocControl(CT_POPUP_MENU, (void*)pPopup, 0, 0, 0, 0, 
                                sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

	pPopup->nIndex = nIndex;
	pPopup->nCursor = 0;
    pPopup->pszTitle = 
                (BYTE *)EQS_GET_TOKEN(TKN_POPUP_MENU_TITLE);
    pPopup->fDarken = FALSE;
    pPopup->pExceptList = EQS_Malloc(sizeof(BOOL) * nLine);
    pPopup->szStringTitle[0] = '\0';

    for(i = 0; i < nLine; i++)
        pPopup->pExceptList[i] = FALSE;

    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);
    EQC_SET_STYLE(hControl, CS_POPUP_DEFAULT);

    EQS_SendEvent(EQE_IO_POPUP_START, hControl, EQS_NA);
    if(EQC_GET_STYLE(hControl) & CS_POPUP_BEEP)
        EQS_SendEvent(EQE_IO_BEEP_SOUND, hControl, EQS_NA);

    EQS_SetIOCActive(hControl);
    EQS_StartModalIO();

    return hControl;
}

HCONTROL EQC_ExceptPopUpMenu(EQS_TOKEN_EX *paItem, UINT8 nLine, UINT8 nIndex,
                                   BOOL *paActive)
{
	POPUP_MENU_T *pPopup;
	HCONTROL hControl;
    UINT8   i, j, nViewHeight;

	if( nLine > VIEW_HEIGHT )
	{
		nLine = VIEW_HEIGHT;
	}
    // girin. 2000.12.19
    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
        EQS_DeleteControl(hControl);
    }
    //
	
	pPopup = (POPUP_MENU_T*)sAllocPopUpMenuInstance((EQS_TOKEN_EX*)paItem, nLine);

	hControl = EQC_AllocControl(CT_POPUP_MENU, (void*)pPopup, 0, 0, 0, 0, 
                                sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

	pPopup->nIndex = nIndex;
	pPopup->nCursor = 0;
    pPopup->pszTitle = 
                (BYTE *)EQS_GET_TOKEN(TKN_POPUP_MENU_TITLE);
    pPopup->fDarken = FALSE;
    pPopup->pExceptList = EQS_Malloc(sizeof(BOOL) * nLine);
    pPopup->szStringTitle[0] = '\0';

    for(i = 0; i < nLine; i++)
        pPopup->pExceptList[i] = !(paActive[i]);

/*
    for(i = 0; i < nExcept; i ++)
    {
        for(j = 0; j < nLine; j++)
        {
            if(paExcept[i] == paItem[j])
                pPopup->pExceptList[j] = TRUE;
        }
    }
*/

    if(pPopup->nLine > VIEW_HEIGHT)
        nViewHeight = VIEW_HEIGHT;
    else
        nViewHeight = pPopup->nLine;

    j = 0;

    if(!sIsAllDisable(pPopup))
    {
        while(pPopup->pExceptList[pPopup->nIndex])
        {
		    if (pPopup->nCursor < nViewHeight - 1)
			    pPopup->nCursor++;

		    if (pPopup->nCursor > pPopup->nLine - 1)
			    pPopup->nCursor = pPopup->nLine - 1;

		    if (pPopup->nIndex >= pPopup->nLine - 1)
            {
			    pPopup->nIndex = 0; // wrap around
                pPopup->nCursor = 0;
            }
		    else
            {
			    pPopup->nIndex++;
            }
        }
    }

    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);
    EQC_SET_STYLE(hControl, CS_POPUP_DEFAULT);

    EQS_SendEvent(EQE_IO_POPUP_START, hControl, EQS_NA);
    if(EQC_GET_STYLE(hControl) & CS_POPUP_BEEP)
        EQS_SendEvent(EQE_IO_BEEP_SOUND, hControl, EQS_NA);

    EQS_SetIOCActive(hControl);
    EQS_StartModalIO();

    return hControl;
}


void EQC_SetPopupMenuTitle(HCONTROL hControl, WORD TokenID)
{
	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);
    
    pPopUpHeader->pszTitle = EQS_GET_TOKEN_EX(TokenID);
}

void EQC_SetPopupMenuTitleString(HCONTROL hControl, BYTE *pszStr)
{
	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);
    
	memset(pPopUpHeader->szStringTitle, 0, MAX_TITLE_STRING+1);

    if(STRLEN(pszStr) > MAX_TITLE_STRING)
    {
	    if( IsMiddleOfMultiByte(pszStr, MAX_TITLE_STRING-2) )
	    {
		    STRNCPY(pPopUpHeader->szStringTitle, pszStr, MAX_TITLE_STRING -3);
		    STRNCPY(pPopUpHeader->szStringTitle+MAX_TITLE_STRING-3, (BYTE*)SF_ETC_SYMBOL,2);
	    }
	    else
	    {
		    STRNCPY(pPopUpHeader->szStringTitle, pszStr, MAX_TITLE_STRING-2);
		    STRNCPY(pPopUpHeader->szStringTitle+ MAX_TITLE_STRING-2, (BYTE*)SF_ETC_SYMBOL,2);
	    }
    }
    else
    {
        STRCPY(pPopUpHeader->szStringTitle, pszStr);
    }

    EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
}

void EQC_SetPopupMenuStatus(HCONTROL hControl, BYTE *pszStr)
{
	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);
    
	memset(pPopUpHeader->szStatus, 0, MAX_TITLE_STRING+1);

    if(STRLEN(pszStr) > MAX_STATUS_STRING)
    {
	    if( IsMiddleOfMultiByte(pszStr, MAX_STATUS_STRING-2) )
	    {
		    STRNCPY(pPopUpHeader->szStatus, pszStr, MAX_STATUS_STRING -3);
		    STRNCPY(pPopUpHeader->szStatus+MAX_STATUS_STRING-3, (BYTE*)SF_ETC_SYMBOL,2);
	    }
	    else
	    {
		    STRNCPY(pPopUpHeader->szStatus, pszStr, MAX_STATUS_STRING-2);
		    STRNCPY(pPopUpHeader->szStatus+ MAX_STATUS_STRING-2, (BYTE*)SF_ETC_SYMBOL,2);
	    }
    }
    else
    {
        STRCPY(pPopUpHeader->szStatus, pszStr);
    }

    EQS_SendEvent(EQE_APP_DRAW, hControl, 0);
}

void EQC_SetExceptPopupMenu(HCONTROL hControl, EQS_TOKEN_EX exceptToken)
{
    UINT8 i, nViewHeight;

	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    for(i = 0; i < pPopUpHeader->nLine; i++)
    {
        if(exceptToken == pPopUpHeader->pToken[i])
            pPopUpHeader->pExceptList[i] = TRUE;
    }

    if(pPopUpHeader->nLine > VIEW_HEIGHT)
        nViewHeight = VIEW_HEIGHT;
    else
        nViewHeight = pPopUpHeader->nLine;

    if(!sIsAllDisable(pPopUpHeader))
    {
        while(pPopUpHeader->pExceptList[pPopUpHeader->nIndex])
        {
		    if (pPopUpHeader->nCursor < nViewHeight - 1)
			    pPopUpHeader->nCursor++;

		    if (pPopUpHeader->nCursor > pPopUpHeader->nLine - 1)
			    pPopUpHeader->nCursor = pPopUpHeader->nLine - 1;

		    if (pPopUpHeader->nIndex >= pPopUpHeader->nLine - 1)
            {
			    pPopUpHeader->nIndex = 0; // wrap around
                pPopUpHeader->nCursor = 0;
            }
		    else
            {
			    pPopUpHeader->nIndex++;
            }
        }
    }
}

void EQC_ResetExceptPopupMenu(HCONTROL hControl)
{
    UINT8 i;

	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    for(i = 0; i < pPopUpHeader->nLine; i++)
        pPopUpHeader->pExceptList[i] = FALSE;
}

void EQC_DisablePopupMenu(HCONTROL hControl)
{
    UINT8 i;

	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    for(i = 0; i < pPopUpHeader->nLine; i++)
        pPopUpHeader->pExceptList[i] = TRUE;
}

void EQC_EnablePopupMenuItem(HCONTROL hControl, EQS_TOKEN_EX enableToken)
{
    UINT8 i, nViewHeight;

	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    for(i = 0; i < pPopUpHeader->nLine; i++)
    {
        if(enableToken == pPopUpHeader->pToken[i])
            pPopUpHeader->pExceptList[i] = FALSE;
    }

    if(pPopUpHeader->nLine > VIEW_HEIGHT)
        nViewHeight = VIEW_HEIGHT;
    else
        nViewHeight = pPopUpHeader->nLine;

    if(!sIsAllDisable(pPopUpHeader))
    {
        while(pPopUpHeader->pExceptList[pPopUpHeader->nIndex])
        {
		    if (pPopUpHeader->nCursor < nViewHeight - 1)
			    pPopUpHeader->nCursor++;

		    if (pPopUpHeader->nCursor > pPopUpHeader->nLine - 1)
			    pPopUpHeader->nCursor = pPopUpHeader->nLine - 1;

		    if (pPopUpHeader->nIndex >= pPopUpHeader->nLine - 1)
            {
			    pPopUpHeader->nIndex = 0; // wrap around
                pPopUpHeader->nCursor = 0;
            }
		    else
            {
			    pPopUpHeader->nIndex++;
            }
        }
    }
}


void EQC_SetPopupMenuStyle(HCONTROL hControl, WORD StyleA)
{

	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

	EQC_GET_INSTANCE(hControl)->Style = (StyleA);
}


void EQC_SetPopupStringItem(HCONTROL hControl, BYTE **paszString)
{
    UINT8   i;
	POPUP_MENU_T *pPopUpHeader;

    ASSERT(EQC_GET_TYPE(hControl) == CT_POPUP_MENU);

	pPopUpHeader = (POPUP_MENU_T *)EQC_GET_INSTANCE(hControl);

    for (i = 0; i < pPopUpHeader->nLine; i++)
    {
        (pPopUpHeader->ppList)[i] = paszString[i];
    }
}