/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**    eqc_scrl.c 
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
** 00-07-07   girin      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "stdio.h"
#include "eqs.h"

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4761 )
#endif

#ifdef CONFIG_EQS_COLOR_64K
#define EQS_COMBO_BACK_COLOR    EQS_RGB(128,128,128)
#define EQS_COMBO_FORE_COLOR    EQS_RGB(101,101,101)

#define EQS_COMBO_ACTIVE_BACK_COLOR   EQS_RGB( 0,192,192)//EQS_RGB(248,147,176)
#endif

#define TAB_ACTIVE_DRAW             FALSE
#define NORMAL_REVERSE              FALSE
#define USE_SKY_LOGO_SYSTEM_BOX     FALSE

//Head ICON
//extern BITMAP g_BmpS16x15x256C;

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define MAX_TITLE_SIZE        23
#define MAX_STATUS_TEXT_SIZE  23
#define MAX_ACCEL_KEY         12
#define MAX_TAB_SIZE          27
/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagSCROLL_LIST_T       SCROLL_LIST_T;
typedef struct tagTAB_LIST_T          TAB_LIST_T;


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

enum  tagSCROLL_TYPE_E
{
	ST_NONE=0,
	ST_DRAW,
	ST_NORMAL,
	ST_TOP_DOWN,
	ST_SCROLL,
	ST_FLOW,
	ST_REDRAW
};
typedef enum tagSCROLL_TYPE_E  SCROLL_TYPE_E;


struct tagTAB_LIST_T 
{
	UINT8   nTabSize;
	UINT8   nViewTab;
	UINT8   nTabCursor;
	UINT8   nCurrentTab;
	BOOL    fEqual;
	WORD    wTabDataSize[MAX_TAB_SIZE];
	BYTE*   pTabText[MAX_TAB_SIZE];
};

struct tagSCROLL_LIST_T
{
    CONTROL_HEADER_T Header;
    // 4 bytes
	UINT8 nIndex;
	UINT8 nCursor;
	UINT8 nViewChar;
	UINT8 nViewLine;

    UINT8 nAllocLine;
    UINT8 nPhysicalLine;
    WORD  nLogicalLine;
    WORD  nLogicalIndex;
 
    UINT8 nCombo;
    BYTE  szTitle[MAX_TITLE_SIZE];
	BYTE  szStatusText[MAX_STATUS_TEXT_SIZE];
    BOOL  fShowTitle;
    BOOL  fLogicalList;
	BOOL  fActive;
	BOOL  fAccelKey;
	
	BOOL  fShowScrollBar;
    SCROLL_BAR       *pScrollBar;
    BOOL* pafCheckBox;
	TAB_LIST_T* pTab;

   
    // 8 bytes
    BYTE **ppNode;  
    BOOL   fFreeMemory;
    BYTE*  pMemory;
	UINT8* pAccelKey;    
    UINT8  ListHandle;
	UINT8  TitleStyle;

    UINT8 nMaxStrLen;

	HTIMER hTimer;
	UINT8  nPos;
	UINT8  FlowMode;
	WORD   nPreviousIndex;
	SCROLL_TYPE_E ScrollType;
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
LOCAL UINT8 l_nMaxStrLen = 0;
LOCAL BOOL  l_fDarken = FALSE;
LOCAL WORD  l_nPreviousLogicalIndex=0;
LOCAL UINT8 l_nPreviousIndex = 0;
LOCAL UINT8 l_nPreviousCursor = 0;
LOCAL COORD l_dwIncreasedCanvasSize=0;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(BYTE **ppszInit, 
                                   UINT8 nInitLine, WORD nLogicalLine);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawInstance(HCONTROL hControl);


LOCAL void sDrawScrollTab(HCONTROL hControl);

LOCAL void sDrawScrollText(HCONTROL hControl, UINT8 nOffset, UINT8 nPos, UINT8 nDataIndex, UINT16 nNumber, BOOL fReverse );

           
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(BYTE **ppszInit, UINT8 nInitLine, 
                                   WORD  nLogicalLine)
{
	UINT8 i;
    SCROLL_LIST_T *pScrollList;
    /*
    if (nInitLine > nLogicalLine)
        return 0;
    */

	pScrollList = (SCROLL_LIST_T *)EQS_Malloc(sizeof(SCROLL_LIST_T));
    pScrollList->nAllocLine = nInitLine+10;
	pScrollList->ppNode = (BYTE **)EQS_Malloc((SIZE_T) \
                                   ((pScrollList->nAllocLine) * sizeof(BYTE*)));

    for (i = 0; i < nInitLine; i++)
    {
        UINT8 nStrLen;

        (pScrollList->ppNode)[i] = ppszInit[i];
        nStrLen = (UINT8)STRLEN(ppszInit[i]);
        l_nMaxStrLen = (nStrLen > l_nMaxStrLen) ? nStrLen : l_nMaxStrLen;
    }

    pScrollList->nPhysicalLine    = nInitLine;
    pScrollList->nLogicalLine     = nLogicalLine;
    pScrollList->nMaxStrLen       = l_nMaxStrLen;
    pScrollList->szTitle[0]       = 0;
	pScrollList->szStatusText[0]  = 0;
    pScrollList->fShowTitle       = FALSE;          // does not display title on default
    pScrollList->fLogicalList     = FALSE;
	pScrollList->fShowScrollBar   = TRUE;
	pScrollList->fAccelKey        = TRUE;
    pScrollList->pafCheckBox = NULL;

#if 0
    pScrollList->pafCheckBox = \
                              (BOOL*) EQS_Malloc(sizeof(BOOL)*pScrollList->nLogicalLine);
    memset((void*)pScrollList->pafCheckBox, 0, sizeof(BOOL)*pScrollList->nLogicalLine );
#endif
	pScrollList->pTab        = NULL;

	
	pScrollList->pAccelKey   = NULL;
    pScrollList->fFreeMemory = FALSE;
    //pScrollList->pMemory     = (BYTE*) ppszInit;
    pScrollList->pMemory     = ppszInit[0];
    pScrollList->ListHandle  = 0;

    pScrollList->pScrollBar = (SCROLL_BAR *)EQS_Malloc(sizeof(SCROLL_BAR));
	pScrollList->TitleStyle = CS_SCROLL_LIST_NO_ARROW_TITLE;

	pScrollList->fActive    = FALSE;

	pScrollList->hTimer   = INVALID_TIMER_HANDLE;
	pScrollList->nPos     = 0;
	pScrollList->FlowMode = 0;
	pScrollList->nPreviousIndex = 0;
	pScrollList->ScrollType = ST_DRAW;

	return (void *)pScrollList;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	SCROLL_LIST_T   *pScrollList;
    BYTE *pChar;
    BOOL  fNeedData = FALSE;
    UINT8 Number, nViewLine;
	COLOR OldTextColor;
	COORD StartX;
   

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE( hControl );

    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine -1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;


	switch (pEvent->EventID)
	{
    case EQE_APP_IO_MODE: 
		pScrollList->ScrollType = ST_DRAW;

		l_nPreviousCursor = pScrollList->nCursor;
		l_nPreviousLogicalIndex = pScrollList->nLogicalIndex;
        l_nPreviousIndex  = pScrollList->nIndex;

		pScrollList->fActive = TRUE;

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO  ||\
			EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )  // "combo box" attribute
		{
			RECT   WindowRect;
			COORD  StartY, DY, WindowSize;


			EQS_GetSWindowViewAreaRect(&WindowRect);
			WindowSize = WindowRect.yBottom - WindowRect.yTop + 1;

			if( pScrollList->fLogicalList == FALSE )
			{   // view line 의 크기를 늘려준다.

				pScrollList->nViewLine = WindowSize/ EQS_CharHeight((BYTE)' ') - 2;
				if( pScrollList->nViewLine > pScrollList->nPhysicalLine )
				{
					if( pScrollList->fShowTitle )
					{
						pScrollList->nViewLine = pScrollList->nPhysicalLine+1;
					}
					else
					{
						pScrollList->nViewLine = pScrollList->nPhysicalLine;
					}
				}
			}
		
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB ||\
				pScrollList->fLogicalList )
			{
				EQC_SetScrollLogicalIndex(hControl, pScrollList->nLogicalIndex);
			}
			else
			{
				EQC_SetScrollPhysicalIndex(hControl, pScrollList->nIndex);
			}
#ifdef CONFIG_EQS_COLOR_64K
			if( pScrollList->fShowTitle )
			{
				DY = EQS_CharHeight((BYTE)' ')*(pScrollList->nViewLine-1) + EQS_SWINDOW_HEADER_HEIGHT;
			}
			else
			{
				DY =  EQS_CharHeight((BYTE)' ')*pScrollList->nViewLine;
			}
#elif
			DY =  EQS_CharHeight((BYTE)' ')*pScrollList->nViewLine;
#endif
			EQC_SET_DY(hControl, DY);	
			StartY = EQC_GET_Y(hControl);
		    
			l_dwIncreasedCanvasSize = EQS_GetCanVasHeight();
			if( l_dwIncreasedCanvasSize < (StartY + DY + 10 ) )
			{				
				EQS_SetCanvasSize(EQS_NA, StartY + DY+10);
			}
			EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ') *pScrollList->nViewChar);


			EQS_SetSWindowPosition(StartY - (WindowSize-DY)+2);
		}
        break;
	case EQE_APP_IO_END:
		break;

    case EQE_TM_TIMER1:
		if( STRLEN(pScrollList->ppNode[pScrollList->nIndex]) >= (pScrollList->nViewChar-2) )
		{
			pScrollList->ScrollType = ST_FLOW;
			EQS_SendEvent(EQE_APP_DRAW, hControl, ST_FLOW);
		}
		break;

	case EQE_APP_DRAW:
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP && l_fDarken == FALSE)
		{				
			l_fDarken = TRUE ;
			//MakeScreenDarker(BG_DARK_COUNT);
		}
#if 0
		if( pScrollList->fActive == TRUE && EQS_IsActive(hControl) == FALSE )
		{
			pScrollList->fActive = FALSE;
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO || \
				EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST	)
			{				
				EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);	
				l_dwIncreasedCanvasSize = 0;
				EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
				EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);
			}
		}
#endif
        		
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
			EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST	)
		{	
			if( EQS_GetModal() == INVALID_CONTROL_HANDLE &&\
				EQS_IsActive(hControl) == FALSE && EQC_GET_DY(hControl) > EQS_CharHeight((BYTE)' ') )
			{
				pScrollList->fActive = FALSE;
				EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);	
				l_dwIncreasedCanvasSize = 0;				
				EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
				EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);
			}
		}

		if( pScrollList->ScrollType == ST_FLOW )
		{
			if( pEvent->wParam != hControl || pEvent->dwParam != ST_FLOW )
			{
				pScrollList->ScrollType = ST_DRAW;
			}
		}
		if( pEvent->wParam ==  EQS_NA )
		{
			pScrollList->ScrollType = ST_DRAW;
		}
		sDrawInstance(hControl);
	
        return ;
		break;
	case EQE_KEY_LEFT:
		if( pScrollList->nLogicalLine == 0 )
			break;
		if( EQS_IsActive(hControl) == FALSE )
		{			
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO )
            {
				pScrollList->nPos = 0;
				EQC_SET_DIRTY(hControl, TRUE);
				if( pScrollList->nLogicalIndex == 0 )
				{
					pScrollList->nLogicalIndex =  pScrollList->nLogicalLine - 1;
				}
				else
				{
					pScrollList->nLogicalIndex--;
				}

				if( pScrollList->fLogicalList == FALSE )
				{
					pScrollList->nIndex = pScrollList->nLogicalIndex;
					if( pScrollList->fShowTitle )
					{
						STRCPY( pScrollList->szTitle, pScrollList->ppNode[pScrollList->nIndex]);
					}
					EQS_SendEvent( EQE_IO_SCROLL_LIST_CHG, hControl, pScrollList->nLogicalIndex );				
				}
				else
				{
					pScrollList->nIndex = 0;
					EQS_SendEvent( EQE_IO_SCROLL_LIST_UP, hControl, pScrollList->nLogicalIndex );
				}
			}
			break;
		}
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			INT16  nTabDataIndex, nIndex;
			
            pScrollList->ScrollType = ST_REDRAW;

			nTabDataIndex = EQC_GetScrollTabIndex( hControl, pScrollList->pTab->nCurrentTab );

			pScrollList->nPos = 0;
			if( nTabDataIndex == 0 )
			{
				nTabDataIndex = EQC_GetScrollTabIndex(hControl, pScrollList->pTab->nTabSize -1);
				if( pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab] != pScrollList->nLogicalLine )
				{
					if( nTabDataIndex == pScrollList->nLogicalLine )
					{
						UINT8 nTab;
						nTab = EQC_GetScrollTabPos(hControl, nTabDataIndex - 1);
						nTabDataIndex -= pScrollList->pTab->wTabDataSize[nTab];
						EQC_SetScrollLogicalIndex(hControl, nTabDataIndex );
						//EQC_SetScrollLogicalIndex(hControl, nTabDataIndex -1 );
					}
					else
					{											
						EQC_SetScrollLogicalIndex(hControl, nTabDataIndex );
					}
				}
				else 
				{
					break;
				}
			}
			else
			{
				for(nIndex = pScrollList->pTab->nCurrentTab -1; nIndex >= 0; nIndex--)
				{
					if( pScrollList->pTab->wTabDataSize[nIndex] )
						break;
				}
				if( nIndex >=0 )
				{
					nTabDataIndex -= pScrollList->pTab->wTabDataSize[nIndex];
				}
				else
				{
					nTabDataIndex = EQC_GetScrollTabIndex(hControl, pScrollList->pTab->nTabSize -1);
				}
				EQC_SetScrollLogicalIndex(hControl, nTabDataIndex );
			}
		
#if 0
			if( pScrollList->pTab->nCurrentTab ==  0 )
			{
				pScrollList->pTab->nCurrentTab = pScrollList->pTab->nTabSize -1;
				pScrollList->pTab->nTabCursor  = pScrollList->pTab->nViewTab-1;
			}
			else
			{
				pScrollList->pTab->nCurrentTab--;
				if( pScrollList->pTab->nTabCursor > 0 )
				{
					pScrollList->pTab->nTabCursor--;
				}
			}
			pScrollList->nCursor = 0;
			pScrollList->nLogicalIndex = EQC_GetScrollTabIndex(hControl, pScrollList->pTab->nCurrentTab);
			if( pScrollList->fLogicalList )
			{
				pScrollList->nIndex = 0;
			}
			else
			{
				pScrollList->nIndex        = pScrollList->nLogicalIndex;
			}
			EQS_SendEvent( EQE_IO_SCROLL_TAB_CHG, hControl, pScrollList->pTab->nCurrentTab);
			EQS_SendEvent( EQE_APP_DRAW, EQS_NA, EQS_NA );
#endif
		}
		else
		{
			EQS_SendEvent( EQE_IO_SCROLL_LIST_LEFT, hControl, EQS_NA);
		}
		break;

	case EQE_KEY_RIGHT:
		if( pScrollList->nLogicalLine == 0 )
			break;
		if( EQS_IsActive(hControl) == FALSE )
		{
			
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO )
            {
				pScrollList->nPos = 0;
				EQC_SET_DIRTY(hControl, TRUE);
				pScrollList->nLogicalIndex = ( pScrollList->nLogicalIndex +1 ) % pScrollList->nLogicalLine ;
				if( pScrollList->fLogicalList == FALSE )
				{
					pScrollList->nIndex = pScrollList->nLogicalIndex;
					if( pScrollList->fShowTitle )
					{
						STRCPY( pScrollList->szTitle, pScrollList->ppNode[pScrollList->nIndex]);
					}
					EQS_SendEvent( EQE_IO_SCROLL_LIST_CHG, hControl, pScrollList->nLogicalIndex );
				}
				else
				{
					pScrollList->nIndex  = 0;
					EQS_SendEvent( EQE_IO_SCROLL_LIST_DOWN, hControl, pScrollList->nLogicalIndex );
				}
			}
			break;
		}

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			UINT8 nCurrentTab;
			WORD  wNextIndex;

			pScrollList->ScrollType = ST_REDRAW;
			pScrollList->nPos = 0;
			nCurrentTab = pScrollList->pTab->nCurrentTab;
			wNextIndex  = EQC_GetScrollTabIndex(hControl, nCurrentTab) +\
				          pScrollList->pTab->wTabDataSize[nCurrentTab];

			if( wNextIndex < pScrollList->nLogicalLine )
			{
				EQC_SetScrollLogicalIndex(hControl, wNextIndex);
			}
			else
			{
				EQC_SetScrollLogicalIndex(hControl, 0 );
			}
#if 0
			pScrollList->pTab->nCurrentTab++;
			if( pScrollList->pTab->nCurrentTab == pScrollList->pTab->nTabSize )
			{
				pScrollList->pTab->nCurrentTab = 0;
				pScrollList->pTab->nTabCursor  = 0;
			}
			else
			{
				if( pScrollList->pTab->nTabCursor < (pScrollList->pTab->nViewTab-1) )
				{
					pScrollList->pTab->nTabCursor++;
				}
			}
			pScrollList->nCursor = 0;
			pScrollList->nLogicalIndex = EQC_GetScrollTabIndex(hControl, pScrollList->pTab->nCurrentTab);
			if( pScrollList->fLogicalList )
			{
				pScrollList->nIndex = 0;
			}
			else
			{
				pScrollList->nIndex = pScrollList->nLogicalIndex;
			}
			EQS_SendEvent( EQE_IO_SCROLL_TAB_CHG, hControl, pScrollList->pTab->nCurrentTab);
			EQS_SendEvent( EQE_APP_DRAW, EQS_NA, EQS_NA );
#endif
		}
		else
		{
			EQS_SendEvent( EQE_IO_SCROLL_LIST_RIGHT, hControl, EQS_NA);
		}

		break;	
	case EQE_KEY_UP:		
		if( pScrollList->nLogicalLine == 0 )
			break;

		pScrollList->nPreviousIndex = pScrollList->nLogicalIndex;
		pScrollList->nPos = 0;

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			if( pScrollList->nCursor == 0 )
			{
				INT16 nCurrentTab,nTabIndex;

				pScrollList->ScrollType = ST_REDRAW;
#if 0 //전체 연속적인 것 
				//EQS_SendEvent(EQE_KEY_LEFT,hControl,EQS_NA);
				if( pScrollList->nLogicalIndex == 0 )
				{
					EQC_SetScrollLogicalIndex(hControl, pScrollList->nLogicalLine-1);
				}
				else
				{
                    if( pScrollList->nLogicalIndex == EQC_GetScrollTabIndex(hControl, pScrollList->pTab->nCurrentTab)  )
					{
						EQC_SetScrollLogicalIndex(hControl, pScrollList->nLogicalIndex-1);
					}
					else
					{
						pScrollList->nLogicalIndex--;
						if( pScrollList->fLogicalList == FALSE )
						{
							pScrollList->nIndex = pScrollList->nLogicalIndex;
							EQS_SendEvent( EQE_IO_SCROLL_LIST_CHG, hControl, pScrollList->nLogicalIndex);		
						}
						else
						{
							pScrollList->nIndex = 0;
							EQS_SendEvent( EQE_IO_SCROLL_LIST_TOP, hControl, pScrollList->nLogicalIndex);
						}
						EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);
					}										
				}
				break;
#endif

				nCurrentTab = pScrollList->pTab->nCurrentTab;
				nTabIndex = EQC_GetScrollTabIndex(hControl, nCurrentTab );
                if( pScrollList->nLogicalIndex == nTabIndex )
				{
					EQC_SetScrollLogicalIndex(hControl, nTabIndex+ pScrollList->pTab->wTabDataSize[nCurrentTab]-1 );
					if( EQC_GetScrollCurrentTabDataSize(hControl)  <= nViewLine )
					{
						pScrollList->ScrollType = ST_TOP_DOWN;
					}
				}
				else
				{
					pScrollList->nLogicalIndex--;
					if( pScrollList->fLogicalList == FALSE )
					{
						pScrollList->nIndex = pScrollList->nLogicalIndex;
						EQS_SendEvent( EQE_IO_SCROLL_LIST_CHG, hControl, pScrollList->nLogicalIndex);		
					}
					else
					{
						pScrollList->nIndex = 0;
						EQS_SendEvent( EQE_IO_SCROLL_LIST_TOP, hControl, pScrollList->nLogicalIndex);
					}
					EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);
				}
				break;
			}
		}

        if( pScrollList->nLogicalIndex == 0 )
        {	
			pScrollList->ScrollType = ST_REDRAW;

            if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
            {
                break;
            }
            else if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_FIXED )
            {
                EQS_SendEvent(EQE_IO_SCROLL_LIST_TOP, (WORD)hControl, 0);
                break;
            }
            else if( pScrollList->fLogicalList && \
                     !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
            {
				if( pScrollList->nLogicalLine >= nViewLine )
				{
					pScrollList->nCursor = nViewLine -1;
				}
				else
				{
					pScrollList->nCursor = pScrollList->nLogicalLine -1;
				}      
				pScrollList->nLogicalIndex  = pScrollList->nLogicalLine -1;
                pScrollList->nIndex  = pScrollList->nCursor;

                EQS_SendEvent( EQE_IO_SCROLL_LIST_BOTTOM, (WORD) hControl,\
                               (DWORD) (pScrollList->nLogicalLine - pScrollList->nCursor-1) );
				EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0);

				if( pScrollList->nLogicalLine <= nViewLine )
				{
					pScrollList->ScrollType = ST_TOP_DOWN;
				}
				break;
            }
            else if( pScrollList->nLogicalLine < pScrollList->nPhysicalLine )
            {
				pScrollList->ScrollType = ST_TOP_DOWN;
                if( pScrollList->nIndex == 0 )
                {
                    pScrollList->nLogicalIndex = pScrollList->nIndex = pScrollList->nLogicalLine-1;
                    if( nViewLine > pScrollList->nLogicalLine )
                        pScrollList->nCursor = pScrollList->nLogicalLine -1;
                    else
                        pScrollList->nCursor = nViewLine-1;
                    EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
		            EQS_SendEvent(EQE_IO_SCROLL_LIST_CHG, (WORD)hControl, (DWORD)(pScrollList->nLogicalIndex));
                }
                else//make EQE_IO_SCROLL_LIST_BOTTOM  (fLogicalList == TRUE 인 경우만 가능 )
                {
                    pScrollList->nCursor = nViewLine -1;
                    pScrollList->nIndex  = pScrollList->nCursor;
                    pScrollList->nLogicalIndex = pScrollList->nLogicalLine -1;
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_BOTTOM, (WORD) hControl,\
                                   (DWORD) (pScrollList->nLogicalLine - pScrollList->nCursor-1) );
                    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0);
                }
                break;
            }
            else ;
        }

		if (pScrollList->nCursor > 0)
		{
			pScrollList->nCursor--;
			pScrollList->ScrollType = ST_NORMAL;
		}
        else
        {
            if( pScrollList->fLogicalList == TRUE )
                fNeedData = TRUE;

			if( pScrollList->nLogicalLine > nViewLine )
			{
				pScrollList->ScrollType = ST_SCROLL;
			}
			else
			{
				pScrollList->ScrollType = ST_TOP_DOWN;
			}
        }

		if (pScrollList->nIndex == 0)
        {
			pScrollList->nIndex = pScrollList->nPhysicalLine - 1; // wrap around  
			pScrollList->ScrollType = ST_REDRAW;

            if ( EQC_GET_STYLE( hControl ) & CS_SCROLL_LIST_SWAP)
            {
                // swap data.
                pChar = (pScrollList->ppNode)[pScrollList->nIndex];
                (pScrollList->ppNode)[pScrollList->nIndex] = (pScrollList->ppNode)[0];
                (pScrollList->ppNode)[0] = pChar;
            }
            if( pScrollList->fLogicalList == FALSE )
            {
                if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
                {
                    if( nViewLine <= pScrollList->nPhysicalLine )
                        pScrollList->nCursor = nViewLine - 1;
                    else
                        pScrollList->nCursor = pScrollList->nPhysicalLine -1;
                }
            }
            
        }
		else
        {
			pScrollList->nIndex--;

            if ( EQC_GET_STYLE( hControl ) & CS_SCROLL_LIST_SWAP)
            {
                // swap data.
                pChar = (pScrollList->ppNode)[pScrollList->nIndex];
                (pScrollList->ppNode)[pScrollList->nIndex] = (pScrollList->ppNode)[pScrollList->nIndex + 1];
                (pScrollList->ppNode)[pScrollList->nIndex + 1] = pChar;
            }
        }

        if( pScrollList->nLogicalIndex == 0 )
        {
            pScrollList->nLogicalIndex  = pScrollList->nLogicalLine -1;
        }
        else
            pScrollList->nLogicalIndex --;

        if( fNeedData )
        {
            if( pScrollList->nLogicalIndex == pScrollList->nLogicalLine - 1  && \
                !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
            {
                pScrollList->nCursor = nViewLine -1;
                pScrollList->nIndex  = pScrollList->nCursor;

                EQS_SendEvent( EQE_IO_SCROLL_LIST_BOTTOM, (WORD) hControl,\
                               (DWORD) (pScrollList->nLogicalLine - pScrollList->nCursor-1) );
            }
            else
            {
                if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
                {
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_UP, (WORD) hControl, (DWORD) pScrollList->nLogicalIndex+1 );
                }
                else
                {
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_UP, (WORD) hControl, (DWORD) pScrollList->nLogicalIndex );
                }
            }
            EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0);
        }
        else
        {   
            EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);		    		        
            EQS_SendEvent(EQE_IO_SCROLL_LIST_CHG, (WORD)hControl, (DWORD)(pScrollList->nLogicalIndex));
            
        }
		break;

	case EQE_KEY_DOWN:
		if( pScrollList->nLogicalLine == 0 )
			break;

		pScrollList->nPreviousIndex = pScrollList->nLogicalIndex;
		pScrollList->nPos = 0;
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
#if 0 //전체 연속적인 것
			if( pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab] == 0 ||
			    (pScrollList->nLogicalIndex +1 ) == EQC_GetScrollTabIndex(hControl,pScrollList->pTab->nCurrentTab+1) )
			{
				//EQS_SendEvent(EQE_KEY_RIGHT,hControl,EQS_NA);
				EQC_SetScrollLogicalIndex(hControl, (pScrollList->nLogicalIndex+1)%pScrollList->nLogicalLine);
				break;
			}
#endif
			if( (pScrollList->nLogicalIndex +1 ) == EQC_GetScrollTabIndex(hControl,pScrollList->pTab->nCurrentTab+1) )
			{	
				//pScrollList->ScrollType = ST_REDRAW;
				EQC_SetScrollLogicalIndex(hControl, EQC_GetScrollTabIndex(hControl,pScrollList->pTab->nCurrentTab));
				if( EQC_GetScrollCurrentTabDataSize(hControl)  <= nViewLine )
				{
					pScrollList->ScrollType = ST_TOP_DOWN;
				}
				break;
			}
		}

        if( pScrollList->nLogicalIndex == (pScrollList->nLogicalLine-1) )
        {
			pScrollList->ScrollType = ST_REDRAW;
            if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
                break;
            else if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_FIXED )
            {
                EQS_SendEvent(EQE_IO_SCROLL_LIST_BOTTOM, (WORD)hControl, (DWORD)pScrollList->nLogicalLine - 1);
				EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
                break;
            }
            else if( pScrollList->fLogicalList && \
                     !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
            {
                pScrollList->nCursor = 0;
                pScrollList->nIndex  = 0;
				pScrollList->nLogicalIndex = 0;
                EQS_SendEvent( EQE_IO_SCROLL_LIST_TOP, (WORD) hControl, 0 );
				EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
				if( pScrollList->nLogicalLine <= nViewLine )
				{
					pScrollList->ScrollType = ST_TOP_DOWN;
				}
				break;
            }
            else if( pScrollList->nLogicalLine < pScrollList->nPhysicalLine )
            {
				pScrollList->ScrollType = ST_TOP_DOWN;

                if( pScrollList->nIndex == pScrollList->nLogicalIndex )
                {
                    pScrollList->nLogicalIndex = pScrollList->nIndex = pScrollList->nCursor = 0;
                    EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
		            EQS_SendEvent(EQE_IO_SCROLL_LIST_CHG, (WORD)hControl, (DWORD)(pScrollList->nLogicalIndex));
                }
                else // send EQE_IO_SCROLL_LIST_TOP Event  (fLogicalList == TRUE 인 경우만 가능 )
                {
                    pScrollList->nCursor = 0;
                    pScrollList->nIndex  = 0;
                    pScrollList->nLogicalIndex = 0;
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_TOP, (WORD) hControl, 0 );
                    EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
                }
                break;
            }                
            else ;
        }

		if (pScrollList->nCursor < nViewLine - 1)
		{
			pScrollList->ScrollType = ST_NORMAL;
			pScrollList->nCursor++;
		}
        else
        {
			if( pScrollList->nLogicalLine > nViewLine )
			{
				pScrollList->ScrollType = ST_SCROLL;
			}
			else
			{
				pScrollList->ScrollType = ST_TOP_DOWN;
			}

            if( pScrollList->fLogicalList == TRUE )
                fNeedData = TRUE;
        }

		if (pScrollList->nCursor > pScrollList->nPhysicalLine - 1)
			pScrollList->nCursor = pScrollList->nPhysicalLine - 1;

		if (pScrollList->nIndex >= pScrollList->nPhysicalLine - 1)
        {
			pScrollList->nIndex = 0; // wrap around
			pScrollList->ScrollType = ST_REDRAW;
           

            if ( EQC_GET_STYLE( hControl ) & CS_SCROLL_LIST_SWAP)
            {
                // swap data.
                pChar = (pScrollList->ppNode)[pScrollList->nIndex];
                (pScrollList->ppNode)[pScrollList->nIndex] = (pScrollList->ppNode)[pScrollList->nPhysicalLine - 1];
                (pScrollList->ppNode)[pScrollList->nPhysicalLine - 1] = pChar;
            }
            if( pScrollList->fLogicalList == FALSE )
            {                                
                if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
                {
                    pScrollList->nCursor = 0;
                }
            }
            
        }
		else
        {
			pScrollList->nIndex++;

            if ( EQC_GET_STYLE( hControl ) & CS_SCROLL_LIST_SWAP)
            {
                // swap data.
                pChar = (pScrollList->ppNode)[pScrollList->nIndex];
                (pScrollList->ppNode)[pScrollList->nIndex] = (pScrollList->ppNode)[pScrollList->nIndex - 1];
                (pScrollList->ppNode)[pScrollList->nIndex - 1] = pChar;
            }
        }

        if( pScrollList->nLogicalIndex >= pScrollList->nLogicalLine -1 )
        {
            pScrollList->nLogicalIndex  = 0;
        }
        else
            pScrollList->nLogicalIndex++;

        if( fNeedData )
        {
            if( pScrollList->nLogicalIndex == 0 && \
                !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_WRAP) )
            {
                pScrollList->nCursor = 0;
                pScrollList->nIndex  = 0;
                EQS_SendEvent( EQE_IO_SCROLL_LIST_TOP, (WORD) hControl, 0 );
            }
            else
            {
                if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SWAP )
                {
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_DOWN, (WORD) hControl, (DWORD) pScrollList->nLogicalIndex );
                }
                else
                {
                    EQS_SendEvent( EQE_IO_SCROLL_LIST_DOWN, (WORD) hControl, (DWORD) pScrollList->nLogicalIndex );
                }
            }
            EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
        }
        else
        {
            EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);		    		        
            EQS_SendEvent(EQE_IO_SCROLL_LIST_CHG, (WORD)hControl, (DWORD)(pScrollList->nLogicalIndex));            
        }
		break;

    case EQE_KEY_CENTER:
		if( pScrollList->nLogicalLine == 0 )
			break;

		pScrollList->nPos = 0;

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			if( pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab] == 0 )
				break;
		}
		EQC_SET_DIRTY(hControl, TRUE);

        if( pScrollList->Header.Style & CS_SCROLL_LIST_CHECK_BOX )
        {
            if( pScrollList->pafCheckBox == NULL )
            {            
                pScrollList->pafCheckBox = \
                              (BOOL*) EQS_Malloc(sizeof(BOOL)*pScrollList->nLogicalLine);
                memset((void*)pScrollList->pafCheckBox, 0, \
                           sizeof(BOOL)*pScrollList->nLogicalLine );
            }  
            pScrollList->pafCheckBox[ pScrollList->nLogicalIndex ] = !pScrollList->pafCheckBox[ pScrollList->nLogicalIndex ];
            EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
        }
        else            
        {           
            EQS_SendEvent(EQE_IO_SCROLL_LIST_SELECT, (WORD)hControl, (DWORD)(pScrollList->nLogicalIndex));             
            if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
				EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
            {		

				EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);					
				l_dwIncreasedCanvasSize = 0;
				EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
				EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);
            }
			pScrollList->fActive = FALSE;
            EQS_EndIO();            
        } 
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
		{           
			l_fDarken = FALSE;
		}
        return;
        break;
          
    case EQE_KEY_CLEAR:
		pScrollList->nPos = 0;

        EQS_SendEvent(EQE_IO_SCROLL_LIST_CANCEL, (WORD)hControl, 0);
        if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO || \
			EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST	)
        {				
			EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);	
			l_dwIncreasedCanvasSize = 0;
			EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
			EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);
        }

        pScrollList->nCursor       = l_nPreviousCursor;
        pScrollList->nLogicalIndex = l_nPreviousLogicalIndex;
		if( pScrollList->fLogicalList == TRUE )
		{
			EQC_SetScrollLogicalIndex(hControl, l_nPreviousLogicalIndex);
		}
		else
		{
			pScrollList->nIndex        = l_nPreviousIndex;
		}
		pScrollList->fActive = FALSE;
        EQS_EndIO();    
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
		{		
			l_fDarken = FALSE;
		}
        return;
        break;

    case EQE_IO_DELETE:
        // Send ending event you want..
		if( pEvent->wParam == hControl )
		{
			EQS_SendEvent(EQE_IO_SCROLL_LIST_CANCEL, (WORD)hControl, (DWORD)EQC_DELETED);
		}
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
		{
			l_fDarken = FALSE;
		}
        // Free instance
		if( pScrollList->fFreeMemory == TRUE )
		{
			EQS_SendEvent(EQE_IO_FREE_MEMORY, (WORD)hControl, (DWORD) pScrollList->pMemory);
		}
		if( pScrollList->ListHandle != 0 )
		{
			EQS_SendEvent(EQE_IO_SCROLL_LIST_DELETE_LIST, (WORD)hControl, (DWORD) pScrollList->ListHandle);
        }
        sFreeInstance(hControl);
        return;
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
    case EQE_KEY_STAR:
    case EQE_KEY_POUND:
/*
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			INT  nTab, nTabIndex;

			nTab = pEvent->EventID - EQE_KEY_0;
			if( nTab > 0 && nTab <= pScrollList->pTab->nTabSize )
			{
				EQC_SetScrollLogicalIndex(hControl, EQC_GetScrollTabIndex(hControl, nTab-1) );
			}
		}
*/
		pScrollList->nPos = 0;
        if( pScrollList->fAccelKey && EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED)
        {				
			for( Number = 0; Number <MAX_ACCEL_KEY; Number++ )
			{
				if( pScrollList->pAccelKey[Number] == (ACCEL_KEY_0 + pEvent->EventID - EQE_KEY_0) )
				{
					if( pScrollList->nLogicalLine >= (Number+1) )
					{
						if( pScrollList->fLogicalList )
						{
							EQC_SetScrollLogicalIndex(hControl, Number);
						}
						else
						{
							EQC_SetScrollPhysicalIndex(hControl, Number);
						}
/*
						pScrollList->nIndex         =  Number;
						pScrollList->nLogicalIndex  =  Number;
						if( pScrollList->nLogicalLine <= nViewLine )
						{
							pScrollList->nCursor = pScrollList->nIndex;
						}
						else
						{
							if( pScrollList->nIndex >= nViewLine )
							{
								pScrollList->nCursor = nViewLine-1;
							}
							else
							{
								pScrollList->nCursor = pScrollList->nIndex;
							}
						}
*/
						EQS_SendEvent(EQE_IO_SCROLL_LIST_SELECT, (WORD)hControl, (DWORD)Number);
						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
							l_fDarken = FALSE;
                        pScrollList->fActive = FALSE;

						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
							EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST)
						{				
							EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
							EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);	
							l_dwIncreasedCanvasSize = 0;
						}
						EQS_EndIO();
						break;
					}
				}
            }
        }
        else
        {
            EQS_SendEvent((EQS_EVENT_ID)(EQE_IO_KEY_0 + pEvent->EventID - EQE_KEY_0),\
                           (WORD)hControl,\
                           pScrollList->nLogicalIndex);			
        }		
        break;                   
    default:
        break;
	}    
}

LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	SCROLL_LIST_T   *pScrollList;
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE( hControl );

	if( pScrollList->ppNode != NULL )
		EQS_Free((void**)&(pScrollList->ppNode));

    if( pScrollList->pafCheckBox != NULL )
        EQS_Free((void**)&(pScrollList->pafCheckBox) );

	if( pScrollList->pAccelKey != NULL )
		EQS_Free((void**)&(pScrollList->pAccelKey) );
                                  
    if( pScrollList->pScrollBar != NULL )
        EQS_Free((void**)&(pScrollList->pScrollBar) );
	
	if( pScrollList->pTab != NULL )
	{
		EQS_Free((void**)&(pScrollList->pTab));
	}
    if( pScrollList->hTimer != INVALID_TIMER_HANDLE )
        EQS_StopControlTimer(hControl, pScrollList->hTimer);

    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

#define sTITLE_POS_OFFSET       0

LOCAL void EQS_API sDrawInstance(HCONTROL hControl)
{
	SCROLL_LIST_T *pScrollList;
    SCROLL_BAR_INFO SBarInfo;
    COLOR  TextColor, TextBackColor, SaveTextColor;
    COORD  StartX, StartY;
    DRAW_MODE DrawMode;    
	UINT8 uDrawIndx, nHeight, nWidth, nTitleHeight, nLength, nSize,i, nIndex, nViewLine;
    INT16 nTitlePosX, nTitlePosY ;
    COLOR OldTextBackColor, OldTextColor;
	BYTE*  pszBuf;

    EQS_GetDC(TRUE);

#if 0
	if( EQC_GET_FOCUS(hControl) == FALSE )
	{
		EQS_SetTextColor(CTRL_DEACTIVE_COLOR);
	}
#endif


	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE(hControl);
  
    nHeight = EQS_CharHeight((BYTE)' ');
    nWidth  = EQS_CharWidth((BYTE)' ');

	StartY = EQC_GET_Y(hControl);
	StartY = EQC_GET_DY(hControl);



    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO || EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )  // "combo box" attribute
    {            
        if( EQS_IsActive(hControl) == FALSE )
        {
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO )
			{	
				BYTE* pszLeftTriangle, *pszRightTriangle;
/*					
#ifdef CONFIG_EQS_COLOR_64K
				if( EQS_HaveFocus(hControl) == FALSE )
				{
					OldTextBackColor = EQS_GetTextBackColor();
					OldTextColor = EQS_GetTextBackColor();					
					EQS_SetTextBackColor(EQS_COMBO_BACK_COLOR);
					EQS_SetTextColor(EQS_COMBO_FORE_COLOR);
				}
#endif
*/
				if( EQS_GetFocusDraw()  )
				{
					pszLeftTriangle = (BYTE*) SF_LEFT_TRIANGLE;
					pszRightTriangle = (BYTE*) SF_RIGHT_TRIANGLE;
				}

				else
				{
					pszLeftTriangle = (BYTE*) SF_DARK_LEFT_TRIANGLE;
					pszRightTriangle = (BYTE*) SF_DARK_RIGHT_TRIANGLE;
				}

				if( EQC_GET_ENABLE(hControl) )
				{
					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl),
										  pszLeftTriangle,
										  2,
										  TDM_NORMAL);  

					EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth*(pScrollList->nCombo-2),
										  EQC_GET_Y(hControl),
										  pszRightTriangle,
										  2,
										  TDM_NORMAL);
				}
				else
				{
					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl),
										  (BYTE*)"  ",
										  2,
										  TDM_NORMAL);  

					EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth*(pScrollList->nCombo-2),
										  EQC_GET_Y(hControl),
										  (BYTE*)"  ",
										  2,
										  TDM_NORMAL);
				}


				if( pScrollList->fShowTitle == TRUE && !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB) )
				{
					nLength = STRLEN(pScrollList->szTitle);				
					if( nLength >= pScrollList->nCombo - 2 )
					{
						EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth,
											  EQC_GET_Y(hControl),
											  pScrollList->szTitle,
											  pScrollList->nCombo-2,//nLength,
											  TDM_NORMAL);
					}
					else
					{
						if( EQC_GET_ENABLE(hControl) )
						{
							StartX = EQC_GET_X(hControl)+nWidth + (nWidth*(pScrollList->nCombo-2-nLength))/2;
							nLength = pScrollList->nCombo - 2;
						}
						else
						{
							StartX   = EQC_GET_X(hControl);
							nLength  = pScrollList->nCombo-1;
						}

						EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth,
											  EQC_GET_Y(hControl),
											  EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK),
											  nLength,//pScrollList->nCombo-2,
											  TDM_NORMAL);


						EQS_TextOutStrTDM( StartX, EQC_GET_Y(hControl), pScrollList->szTitle,TDM_NORMAL);
					}
				}
				else
				{           

					pszBuf = (BYTE*)((pScrollList->ppNode)[pScrollList->nIndex]);

					nLength = STRLEN(pszBuf);
					if( nLength >= pScrollList->nCombo - 2 )
					{
						EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth,
											  EQC_GET_Y(hControl),
											  pszBuf,
											  pScrollList->nCombo-2,//nLength,
											  TDM_NORMAL);
					}
					else
					{
						if( EQC_GET_ENABLE(hControl) )
						{
							StartX  = EQC_GET_X(hControl)+nWidth + (nWidth*(pScrollList->nCombo-2-nLength))/2;
							nLength = pScrollList->nCombo - 2;

						}
						else
						{
							StartX   = EQC_GET_X(hControl);
							nLength  = pScrollList->nCombo-1;
						}
						EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth,
											  EQC_GET_Y(hControl),
											  EQS_GET_TOKEN(TKN_TIME_CONTROL_BLANK),
											  nLength,//pScrollList->nCombo-2
											  TDM_NORMAL);							
						EQS_TextOutStrTDM( StartX, EQC_GET_Y(hControl), pszBuf, TDM_NORMAL);
						
					}
				}
/*
#ifdef CONFIG_EQS_COLOR_64K
				if( EQS_HaveFocus(hControl) == FALSE )
				{
					OldTextBackColor = EQS_GetTextBackColor();
					OldTextColor = EQS_GetTextBackColor();
					EQS_SetTextBackColor( OldTextBackColor );
					EQS_SetTextColor( OldTextColor );
				}
#endif
*/
			}
			else
			{		
				
				if( pScrollList->fShowTitle == TRUE && !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB) )
				{

					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl),
										  pScrollList->szTitle,
										  pScrollList->nCombo-2,//nLength,
										  TDM_NORMAL);

				}
				else
				{           

					pszBuf = (BYTE*)((pScrollList->ppNode)[pScrollList->nIndex]);

					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl),
										  pszBuf,
										  pScrollList->nCombo-2,//nLength,
										  TDM_NORMAL);
				}
				EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth*(pScrollList->nCombo-2),
					                  EQC_GET_Y(hControl),
						              (BYTE*)SF_COMBO_BOX,//"\205\355",
							          2,
								      TDM_NORMAL);            
			}

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_BORDER )
			{
				COLOR ForeColor;
				ForeColor = EQS_GetForeColor();
	            EQS_SetForeColor(182);
/*
				EQS_Rectangle( EQC_GET_X(hControl)-1 , EQC_GET_Y(hControl)-1,
					          (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)), 
						      (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)));
*/
				EQC_DrawBorder(hControl, EQC_GET_X(hControl) , EQC_GET_Y(hControl),
					           (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
						       (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl))-1);
				EQS_SetForeColor(ForeColor);

			}

			if( EQS_HaveFocus(hControl) && EQC_GET_ENABLE(hControl) == TRUE )
			{
				if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_NO_CML) )
				{
					EQS_EVENT  CurrentEvent;

					EQS_DrawIOCCML(IOC_CML_SELECT_CENTER);

					CurrentEvent = EQS_GetEvent();
					if( CurrentEvent.wParam == hControl )
					{
						EQS_UpdateIOCCML();
					}
				}
			}

            EQS_UpdateControlDisplay();
            EQS_ReleaseDC();
            return;
        }        
    }

    //EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ') *pScrollList->nViewChar);

#if 0
    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO  && !pScrollList->fYPositionChanged)   // Combox box is actived
    {
		RECT WindowRect;

        EQS_GetIOCWindowPosY(hControl, &StartY);
        EQS_GetSWindowViewAreaRect(&WindowRect);
        if( (StartY + EQC_GET_DY(hControl)-1) > (WindowRect.yBottom-WindowRect.yTop) ) // +1 01.03.25
        {           
            pScrollList->fYPositionChanged = TRUE;
            EQC_SET_Y( hControl, (EQC_GET_Y(hControl)+EQS_CharHeight((BYTE)' ')-EQC_GET_DY(hControl)));
        }
    }
#endif

    if( pScrollList->Header.Style & CS_SCROLL_LIST_CHECK_BOX )
    {
        EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')  * (pScrollList->nViewChar+2));

        EQS_FillRectangle( (COORD)(EQC_GET_X(hControl)),
                           (COORD)(EQC_GET_Y(hControl)), 
                           (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)), 
                           (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)),
                           EQS_GetTextBackColor());//EQS_GetBackColor());
    }
#if 0
    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP ) 
	{//redstar 2001.03.27  for pop up scroll

        EQS_Draw3DBox( EQC_GET_X(hControl), EQC_GET_Y(hControl),  
                       (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
                       (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1) );

	}
#endif

    if( pScrollList->fShowTitle == TRUE )
    {   				
		nSize = pScrollList->nViewLine -1;   //2001.01.15 타이틀을 포함하여 VIEW LINE 만큼 출력    
		
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{

			COLOR OldColor;
			nSize = pScrollList->nViewLine -1;   //2001.01.15 타이틀을 포함하여 VIEW LINE 만큼 출력   
#ifdef CONFIG_EQS_COLOR_64K
			nTitleHeight = EQS_SWINDOW_HEADER_HEIGHT;
#else
			nTitleHeight = EQC_GET_DY(hControl) - nSize*EQS_CharHeight((BYTE)' ');

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_BORDER )
			{
				EQS_Line((COORD)EQC_GET_X(hControl), 
						 (COORD)EQC_GET_Y(hControl) + nTitleHeight,
						 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1),
						 (COORD)EQC_GET_Y(hControl) + nTitleHeight);
			}
#endif
			if( pScrollList->ScrollType == ST_DRAW || pScrollList->ScrollType == ST_REDRAW)
			{
				if(  pScrollList->pTab->fEqual == TRUE )
				{
					sDrawScrollTab(hControl);
				}
				else
				{

					BYTE szBuf[40];


					OldColor = EQS_GetForeColor();
					TextColor = EQS_GetTextColor();
					if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
					{
						EQS_FillRectangle( EQC_GET_X(hControl),
										   EQC_GET_Y(hControl), 
										   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
										   EQC_GET_Y(hControl) + nTitleHeight-1,
										   CAPTION_BACK_COLOR);
						EQS_SetForeColor(R_WHITE);
						EQS_Line((COORD)EQC_GET_X(hControl)-1, 
								 (COORD)EQC_GET_Y(hControl),
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl));
						EQS_Line((COORD)EQC_GET_X(hControl)-1, 
								 (COORD)EQC_GET_Y(hControl),
								 (COORD)EQC_GET_X(hControl)-1,
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight -1);
        
						EQS_SetForeColor(R_GRAY);
						EQS_Line((COORD)EQC_GET_X(hControl), 
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
						EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
								 (COORD)EQC_GET_Y(hControl)+1,
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
						EQS_SetTextColor(CAPTION_FORE_COLOR);
					}
					else
					{
						EQS_FillRectangle( EQC_GET_X(hControl),
										   EQC_GET_Y(hControl), 
										   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
										   EQC_GET_Y(hControl) + nTitleHeight-1,
										   LIST_CAPTION_TEXT_BACK_COLOR);
						EQS_SetForeColor(LIST_CAPTION_R_WHITE);
						EQS_Line((COORD)EQC_GET_X(hControl)-1, 
								 (COORD)EQC_GET_Y(hControl),
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl));

						EQS_Line((COORD)EQC_GET_X(hControl), 
								 (COORD)EQC_GET_Y(hControl),
								 (COORD)EQC_GET_X(hControl),
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight -1);

        
						EQS_SetForeColor(LIST_CAPTION_R_GRAY);
						EQS_Line((COORD)EQC_GET_X(hControl), 
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
						EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
								 (COORD)EQC_GET_Y(hControl)+1,
								 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
								 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);

						EQS_SetTextColor(LIST_CAPTION_TEXT_FORE_COLOR);
					}
					

					EQS_SetForeColor(OldColor);															
					DrawMode  = EQS_GetDrawMode();
					EQS_SetDrawMode(DM_OR);

					nTitlePosY = (nTitleHeight - EQS_CharHeight((BYTE)' ')) /2 + sTITLE_POS_OFFSET;


					if( pScrollList->pTab->nCurrentTab > 0 )
					{
						EQS_TextOutStrLen( EQC_GET_X(hControl), EQC_GET_Y(hControl)+nTitlePosY,\
										   (BYTE*)SF_LEFT_TRIANGLE,2);
					}
						
					nIndex = pScrollList->pTab->nCurrentTab;
#if 0
					if( (nIndex+1) < MAX_LIMIT_DIGIT )
						sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex);	
					else
						sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT_NEXT_BASE,\
													SPECIAL_DIGIT_96+nIndex+1-MAX_LIMIT_DIGIT);	
					STRCAT(szBuf, pScrollList->pTab->pTabText[nIndex]);
#endif
					STRCPY(szBuf, pScrollList->pTab->pTabText[nIndex]);
					nTitlePosX = (EQC_GET_DX(hControl))/2 - STRLEN(szBuf)*nWidth/2;
					
					/*EQS_TextOutStrLenTDM(  EQC_GET_X(hControl)+nTitlePosX,
										   EQC_GET_Y(hControl),
										   szBuf,
										   STRLEN(szBuf), TDM_BOLD
										);*/
					EQS_TextOutStrLen(  EQC_GET_X(hControl)+nTitlePosX,
										EQC_GET_Y(hControl)+nTitlePosY,
										szBuf,
										STRLEN(szBuf)
										);
					if( pScrollList->pTab->nCurrentTab < (pScrollList->pTab->nTabSize-1) )
					{
						EQS_TextOutStrLen( EQC_GET_X(hControl)+EQC_GET_DX(hControl)-2*nWidth-1,\
										   EQC_GET_Y(hControl)+nTitlePosY,(BYTE*)SF_RIGHT_TRIANGLE,2);
					}
					EQS_SetDrawMode(DrawMode);  
					EQS_SetTextColor(TextColor); 
				}
			} //if( pScrollList->ScrollType == ST_DRAW )
		}
		else
		{   // tab 컨트롤이 아닌 경우 
			COLOR OldColor;

#ifdef CONFIG_EQS_COLOR_64K
			if( (EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST) ||\
				(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO))
			{
				//nTitleHeight = EQC_GET_DY(hControl) - nSize*EQS_CharHeight((BYTE)' ');
				nTitleHeight = EQS_SWINDOW_HEADER_HEIGHT;
			}
			else
			{						
				nTitleHeight = EQS_SWINDOW_HEADER_HEIGHT;
			}

#else
            nTitleHeight = EQC_GET_DY(hControl) - nSize*EQS_CharHeight((BYTE)' ');	


			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_HMAXIMIZE )
			{
				nTitleHeight +=1;
			}

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_BORDER )
			{
				EQS_Line((COORD)EQC_GET_X(hControl), 
						 (COORD)EQC_GET_Y(hControl) + nTitleHeight,
						 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1),
						 (COORD)EQC_GET_Y(hControl) + nTitleHeight);
			}
#endif

			if( pScrollList->ScrollType == ST_DRAW || pScrollList->ScrollType == ST_REDRAW)
			{


/*
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
                    EQS_DrawTitlePannel();
                }
				else
				{
					EQS_FillRectangle( EQC_GET_X(hControl),
									   EQC_GET_Y(hControl), 
									   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
									   EQC_GET_Y(hControl) + nTitleHeight-1,
									   LIST_CAPTION_TEXT_BACK_COLOR);
				}
*/
				OldColor = EQS_GetForeColor();
				TextColor = EQS_GetTextColor();
				TextBackColor = EQS_GetTextBackColor();
				DrawMode  = EQS_GetDrawMode();

				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
					EQS_FillRectangle( EQC_GET_X(hControl),
									   EQC_GET_Y(hControl), 
									   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
									   EQC_GET_Y(hControl) + nTitleHeight-1,
									   CAPTION_BACK_COLOR);    
					
					EQS_SetForeColor(R_WHITE);
					EQS_Line((COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl),
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl));
					EQS_Line((COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl),
							 (COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
    
					EQS_SetForeColor(R_GRAY);
					EQS_Line((COORD)EQC_GET_X(hControl)+1, 
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
					EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
							 (COORD)EQC_GET_Y(hControl)+1,
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
					EQS_SetTextColor(CAPTION_FORE_COLOR);
					EQS_SetTextBackColor(CAPTION_BACK_COLOR);
				}
				else
				{
					EQS_FillRectangle( EQC_GET_X(hControl),
									   EQC_GET_Y(hControl), 
									   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
									   EQC_GET_Y(hControl) + nTitleHeight-1,
									   LIST_CAPTION_TEXT_BACK_COLOR);    
					
					EQS_SetForeColor(LIST_CAPTION_R_WHITE);
					EQS_Line((COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl),
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl));
					EQS_Line((COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl),
							 (COORD)EQC_GET_X(hControl)-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
    
					EQS_SetForeColor(LIST_CAPTION_R_GRAY);
					EQS_Line((COORD)EQC_GET_X(hControl)+1, 
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
					EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
							 (COORD)EQC_GET_Y(hControl)+1,
							 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
							 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
					EQS_SetTextColor(LIST_CAPTION_TEXT_FORE_COLOR);
					EQS_SetTextBackColor(LIST_CAPTION_TEXT_BACK_COLOR);

				}

				EQS_SetForeColor(OldColor);
				EQS_SetDrawMode(DM_OR);

				nTitlePosX = (EQC_GET_DX(hControl))/2 - STRLEN(pScrollList->szTitle)*nWidth/2;
				nTitlePosX = (nTitlePosX <= 1 ) ? 2 : nTitlePosX;
				nTitlePosY = (nTitleHeight - EQS_CharHeight((BYTE)' ')) /2 + sTITLE_POS_OFFSET;

				if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO) &&\
					!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST) )
				{			
					WINDOW_TSYMBOL_MODE  WindowSymbolMode;

					WindowSymbolMode = EQS_GetWindowSymbolMode();

					nTitlePosX = (nTitlePosX > 17 ) ? nTitlePosX : 16; 
					switch( WindowSymbolMode )
					{
					case WTSM_NONE_MODE:
						break;
					case WTSM_NORMAL_MODE:
//#if USE_SKY_LOGO_SYSTEM_BOX
//					EQS_PutBitmap(EQC_GET_X(hControl), EQC_GET_Y(hControl) + nTitlePosY,\
//								  &g_BmpS16x15x256C); //draw logo
//#else
						nTitlePosX = (EQC_GET_DX(hControl))/2 - STRLEN(pScrollList->szTitle)*nWidth/2;
//#endif
						break;
					case WTSM_CALL_MODE:
						EQS_TextOutStr(EQC_GET_X(hControl),EQC_GET_Y(hControl) + nTitlePosY,\
									   (BYTE*)SF_CALL_CALLING);
						break;
					case WTSM_DORMONT_MODE:
						EQS_TextOutStr(EQC_GET_X(hControl),EQC_GET_Y(hControl) + nTitlePosY,\
									   (BYTE*)SF_CALL_STND);
						break;
					case WTSM_NATIVE_MODE:
						break;
					default:
						break;
					}
				}			
				nLength =  pScrollList->nViewChar -1; //실제 가능한 길이 보다 한 글자 줄여서 여백을 좀 만든다.

				if( nTitlePosX < 0 )
					nTitlePosX = 3;                   //make space 

				if( STRLEN(pScrollList->szTitle) > nLength )
				{
					BYTE pszBuffer[22];
					memset(pszBuffer, 0, 22);

					if( IsMiddleOfMultiByte(pScrollList->szTitle, nLength-2) )
					{
						STRNCPY(pszBuffer, pScrollList->szTitle, nLength -3);
						STRNCPY(pszBuffer+nLength-3, (BYTE*)SF_ETC_SYMBOL,2);
					}
					else
					{
						STRNCPY(pszBuffer, pScrollList->szTitle, nLength-2);
						STRNCPY(pszBuffer+ nLength-2, (BYTE*)SF_ETC_SYMBOL,2);
					}
					EQS_TextOutStrLen( (COORD)(EQC_GET_X(hControl)+nTitlePosX),
									   (COORD)(EQC_GET_Y(hControl)+nTitlePosY),
										pszBuffer,
										nLength
									 );
				}    
				else
				{
					EQS_TextOutStrLen( (COORD)(EQC_GET_X(hControl)+nTitlePosX),
									   (COORD)(EQC_GET_Y(hControl)+nTitlePosY),
										pScrollList->szTitle,
										nLength
									 );
				}
				switch( pScrollList->TitleStyle )
				{
				case CS_SCROLL_LIST_NO_ARROW_TITLE:
					break;
				case CS_SCROLL_LIST_LEFT_ARROW_TITLE:
					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl)+nTitlePosY,
										  (BYTE*) SF_DARK_LEFT_TRIANGLE,
										  2,
										  TDM_NORMAL); 
					break;
				case CS_SCROLL_LIST_RIGHT_ARROW_TITLE:
					EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth*(pScrollList->nCombo-2),
										  EQC_GET_Y(hControl)+nTitlePosY,
										  (BYTE*) SF_DARK_RIGHT_TRIANGLE,
										  2,
										  TDM_NORMAL);
					break;
				case CS_SCROLL_LIST_BOTH_ARROW_TITLE:
					EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
										  EQC_GET_Y(hControl)+nTitlePosY,
										  (BYTE*) SF_LEFT_TRIANGLE,
										  2,
										  TDM_NORMAL);  

					EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+nWidth*(pScrollList->nCombo-2),
										  EQC_GET_Y(hControl)+nTitlePosY,
										  (BYTE*) SF_RIGHT_TRIANGLE,
										  2,
										  TDM_NORMAL);
					break;
				default:
					break;
				}
				EQS_SetTextColor(TextColor);   
				EQS_SetTextBackColor(TextBackColor);
				EQS_SetDrawMode(DrawMode);  
			} //if( pScrollList->ScrollType == ST_DRAW )
			
        }

#ifdef CONFIG_EQS_COLOR_64K		
		if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST) &&\
			!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO)
		   )
		{
			nTitleHeight += EQS_SWINDOW_TOP_MARGIN ;
		}
#endif

    }
    else
    {
        nTitleHeight = 0;
        nSize = pScrollList->nViewLine;
    }

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
	{
		nSize--;
		nViewLine = nSize;
	}
        

	if( pScrollList->fShowTitle == TRUE )     //2001.01.15 타이틀을 포함하여 VIEW LINE 만큼 출력
		nViewLine = pScrollList->nViewLine -1;
	else
		nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;



	if( pScrollList->ScrollType == ST_NORMAL )
	{
		RECT Rect;
		INT  nScrollBarSize;

        if( pScrollList->nPreviousIndex < pScrollList->nLogicalIndex )
		{
			sDrawScrollText(hControl, nTitleHeight, pScrollList->nCursor -1 ,\
				            pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
							pScrollList->nLogicalIndex-1,FALSE);
			Rect.yTop = EQC_GET_Y(hControl) + (pScrollList->nCursor -1) * EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		else
		{
			sDrawScrollText(hControl, nTitleHeight, pScrollList->nCursor +1 ,\
				            pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
							pScrollList->nLogicalIndex+1,FALSE);
			Rect.yTop = EQC_GET_Y(hControl) + (pScrollList->nCursor) * EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		Rect.yBottom = Rect.yTop + 2 * EQS_CharHeight((BYTE)' ');
		Rect.xLeft = EQC_GET_X(hControl);
		Rect.xRight = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + 1 ;
		sDrawScrollText(hControl, nTitleHeight, pScrollList->nCursor , pScrollList->nIndex,\
			            pScrollList->nLogicalIndex,TRUE);

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
		{
			nScrollBarSize = R_SCROLLBAR_SIZE+4;
		}
		else
		{
			nScrollBarSize = R_SCROLLBAR_SIZE+3;
		}
		
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR )
		{
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
			{
				UINT8 nCurrentTab;
				WORD  wCurrentTabData, wTabIndex;

				nCurrentTab = pScrollList->pTab->nCurrentTab;
				wCurrentTabData = pScrollList->pTab->wTabDataSize[nCurrentTab];
				if( wCurrentTabData > nSize )
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
			else
			{
				if( pScrollList->nLogicalLine > nViewLine)			
				{				
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
		}
		else
		{
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
		}
	}
	else if( pScrollList->ScrollType == ST_FLOW )
	{
		if( pScrollList->FlowMode )
		{
			RECT Rect;
			INT  nScrollBarSize;

			Rect.yTop = EQC_GET_Y(hControl) + (pScrollList->nCursor) * EQS_CharHeight((BYTE)' ')+nTitleHeight ;
			Rect.yBottom = Rect.yTop + EQS_CharHeight((BYTE)' ');
			Rect.xLeft = EQC_GET_X(hControl);
			Rect.xRight = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + 1 ;

			sDrawScrollText(hControl, nTitleHeight, pScrollList->nCursor ,\
				            pScrollList->nIndex, pScrollList->nLogicalIndex, TRUE);

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
			{
				nScrollBarSize = R_SCROLLBAR_SIZE+4;
			}
			else
			{
				nScrollBarSize = R_SCROLLBAR_SIZE+3;
			}
			
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR )
			{
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					UINT8 nCurrentTab;
					WORD  wCurrentTabData, wTabIndex;

					nCurrentTab = pScrollList->pTab->nCurrentTab;
					wCurrentTabData = pScrollList->pTab->wTabDataSize[nCurrentTab];
					if( wCurrentTabData > nSize )
					{
						EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
					}
					else
					{
						EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
					}
				}
				else
				{
					if( pScrollList->nLogicalLine > nViewLine)			
					{				
						EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
					}
					else
					{
						EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
					}
				}
			}
			else
			{
				EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
			}
		}
	}
	else if( pScrollList->ScrollType == ST_TOP_DOWN )
	{
		RECT  Rect;
		UINT8 nCursor, nScrollBarSize;
					
		Rect.xLeft = EQC_GET_X(hControl);
		Rect.xRight = EQC_GET_X(hControl) + EQC_GET_DX(hControl) + 1;

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
		{
			nScrollBarSize = R_SCROLLBAR_SIZE+4;
		}
		else
		{
			nScrollBarSize = R_SCROLLBAR_SIZE+3;
		}

		nCursor = nViewLine;
		if( nCursor > pScrollList->nLogicalLine )
		{
			nCursor = pScrollList->nLogicalLine;
		}

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{
			if( nCursor > EQC_GetScrollCurrentTabDataSize(hControl) )
			{
				nCursor =  EQC_GetScrollCurrentTabDataSize(hControl);
			}
		}

		if( pScrollList->nCursor == 0 )
		{
			sDrawScrollText(hControl, nTitleHeight, nCursor-1 ,\
			                pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
			                nCursor-1, FALSE);
			Rect.yTop = EQC_GET_Y(hControl)+ (nCursor-1)*EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		else
		{
			sDrawScrollText(hControl, nTitleHeight, 0,\
			                pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
			                0,FALSE);
			Rect.yTop = EQC_GET_Y(hControl)+nTitleHeight ;	
		}
		Rect.yBottom = Rect.yTop + EQS_CharHeight((BYTE)' ');

		sDrawScrollText(hControl, nTitleHeight, pScrollList->nCursor,\
			            pScrollList->nIndex, pScrollList->nLogicalIndex, TRUE);

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR )
		{
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
			{
				UINT8 nCurrentTab;
				WORD  wCurrentTabData, wTabIndex;

				nCurrentTab = pScrollList->pTab->nCurrentTab;
				wCurrentTabData = pScrollList->pTab->wTabDataSize[nCurrentTab];
				if( wCurrentTabData > nSize )
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
			else
			{
				if( pScrollList->nLogicalLine > nViewLine)			
				{				
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
		}
		else
		{
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
		}

		Rect.yTop = EQC_GET_Y(hControl)+ pScrollList->nCursor*EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		Rect.yBottom = Rect.yTop + EQS_CharHeight((BYTE)' ');
			
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR )
		{
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
			{
				UINT8 nCurrentTab;
				WORD  wCurrentTabData, wTabIndex;

				nCurrentTab = pScrollList->pTab->nCurrentTab;
				wCurrentTabData = pScrollList->pTab->wTabDataSize[nCurrentTab];
				if( wCurrentTabData > nSize )
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
			else
			{
				if( pScrollList->nLogicalLine > nViewLine)			
				{				
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-nScrollBarSize, Rect.yBottom);
				}
				else
				{
					EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
				}
			}
		}
		else
		{
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
		}

	}
	else if( pScrollList->ScrollType == ST_SCROLL )
	{
		INT nScrollWidth;

		StartY = (COORD)(EQC_GET_Y(hControl)+ EQS_CharHeight((BYTE)' ')+nTitleHeight);

		nScrollWidth = EQC_GET_DX(hControl);
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR )
		{
			nScrollWidth -= R_SCROLLBAR_SIZE;
		}

        if( pScrollList->nPreviousIndex < pScrollList->nLogicalIndex )
		{   // scroll up			
			sDrawScrollText(hControl, nTitleHeight, nViewLine-1,\
			                pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
			                pScrollList->nLogicalIndex-1,FALSE);
			ScrollScreen(EQC_GET_X(hControl), EQC_GET_Y(hControl)+nTitleHeight,\
				         EQC_GET_X(hControl), (UINT8)StartY,\
						 nScrollWidth,\
						 (nViewLine-1)*EQS_TextHeight((BYTE*)" "));
			sDrawScrollText(hControl, nTitleHeight, nViewLine-1, pScrollList->nIndex,
				            pScrollList->nLogicalIndex,TRUE);

		}
		else
		{
			sDrawScrollText(hControl, nTitleHeight, 0,\
			                pScrollList->nPreviousIndex-pScrollList->nLogicalIndex+pScrollList->nIndex,\
			                pScrollList->nLogicalIndex+1,FALSE);
			ScrollScreen(EQC_GET_X(hControl), StartY,\
				         EQC_GET_X(hControl), EQC_GET_Y(hControl)+nTitleHeight,\
						 nScrollWidth,\
						 (nViewLine-1)*EQS_TextHeight((BYTE*)" "));
			sDrawScrollText(hControl, nTitleHeight, 0, pScrollList->nIndex,\
				            pScrollList->nLogicalIndex,TRUE);
		}	
		//EQS_RefreshDisplayRect(EQC_GET_X(hControl), EQC_GET_Y(hControl)+nTitleHeight,\
			                   EQC_GET_X(hControl)+EQC_GET_DX(hControl)-R_SCROLLBAR_SIZE,\
							   EQC_GET_Y(hControl)+ nViewLine*EQS_CharHeight((BYTE)' ')+nTitleHeight );
	}
	else //ST_DRAW  or ST_REDRAW 
	{
		if (nSize >= pScrollList->nPhysicalLine)
		{
			pScrollList->nCursor = pScrollList->nIndex;

			for (i = 0; i < nSize; i++)
			{
				if (pScrollList->nCursor == i)
				{   // REVERSE                        
					sDrawScrollText(hControl, nTitleHeight,i,i,\
						            pScrollList->nLogicalIndex+i-pScrollList->nCursor,TRUE);
				}
				else
				{   // NORMAL
					sDrawScrollText(hControl, nTitleHeight,i,i,\
						            pScrollList->nLogicalIndex+i-pScrollList->nCursor,FALSE);
				}
			}
		}
		else
		{
			uDrawIndx = (pScrollList->nIndex - pScrollList->nCursor + pScrollList->nPhysicalLine) % pScrollList->nPhysicalLine;
/*        
			if( pScrollList->fShowTitle == TRUE )     //2001.01.15 타이틀을 포함하여 VIEW LINE 만큼 출력
				nViewLine = pScrollList->nViewLine -1;
			else
				nViewLine = pScrollList->nViewLine;

			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
				nViewLine--;
*/
			if( pScrollList->nLogicalLine < nViewLine )
			{
				nSize = pScrollList->nLogicalLine;   

			}
			else
				nSize = nViewLine;
                
			for (i = 0; i < nViewLine; i++, uDrawIndx = MOD_INCREMENT(uDrawIndx, pScrollList->nPhysicalLine))
			{
				if (pScrollList->nCursor == i)
				{   // REVERSE                        
					sDrawScrollText(hControl, nTitleHeight,i,uDrawIndx,\
						            pScrollList->nLogicalIndex+i-pScrollList->nCursor,TRUE);
				}
				else
				{   // NORMAL
					sDrawScrollText(hControl, nTitleHeight,i,uDrawIndx,\
						            pScrollList->nLogicalIndex+i-pScrollList->nCursor,FALSE);               
				}
			}
		}
	}

	if( pScrollList->ScrollType == ST_DRAW || pScrollList->ScrollType == ST_REDRAW || pScrollList->ScrollType == ST_SCROLL)
	{

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR  &&  pScrollList->fShowScrollBar )
		{
			UINT8  nStatusBarHeight = 0;

			if( pScrollList->nLogicalLine > nSize )
			{

#ifdef CONFIG_EQS_COLOR_64K

				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
					nWidth = R_SCROLLBAR_SIZE + 1;
				}
				else
				{
					nWidth = R_SCROLLBAR_SIZE;
				}
#elif
				nWidth = R_SCROLLBAR_SIZE;
#endif
	/*
				if( pScrollList->fShowTitle == TRUE )
					nHeight = EQS_CharHeight((BYTE)' ')+1;
				else
					nHeight = 0;
	*/
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
				{
					nStatusBarHeight = EQS_CharHeight((BYTE)' ') ;
				}


				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
				{
					UINT8 nCurrentTab;
					WORD  wCurrentTabData, wTabIndex;

					nCurrentTab = pScrollList->pTab->nCurrentTab;
					wCurrentTabData = pScrollList->pTab->wTabDataSize[nCurrentTab];
					if( wCurrentTabData > nSize )
					{
#ifdef CONFIG_EQS_COLOR_64K
						EQS_AddScrollBar(NULL, pScrollList->pScrollBar,SBT_VERT,
										 EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth-1,\
										 EQC_GET_Y(hControl)+nTitleHeight,\
										 nSize*EQS_CharHeight((BYTE)' '),\
										 0,100,nSize,wCurrentTabData);

#elif
						EQS_AddScrollBar(NULL, pScrollList->pScrollBar,SBT_VERT,
										 EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth-1,\
										 EQC_GET_Y(hControl)+nTitleHeight,\
										 EQC_GET_DY(hControl)-nTitleHeight-nStatusBarHeight,\
										 0,100,nSize,wCurrentTabData);
#endif

						SBarInfo.nMax = wCurrentTabData - nSize;
						SBarInfo.nMin = 0;                            
						SBarInfo.nPageSize = nSize;
						SBarInfo.nPos =  pScrollList->nLogicalIndex - \
										 EQC_GetScrollTabIndex(hControl, nCurrentTab) - \
										 pScrollList->nCursor ;
						EQS_SetScrollBarInfo(pScrollList->pScrollBar, &SBarInfo, TRUE);
					}
					else
					{
#ifdef CONFIG_EQS_COLOR_64K

						if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
						{
							EQS_FillRectangle(EQC_GET_X(hControl)+EQC_GET_DX(hControl)-3,\
											  EQC_GET_Y(hControl)+nTitleHeight,\
											  EQC_GET_X(hControl)+EQC_GET_DX(hControl)+1,\
											  EQC_GET_Y(hControl)+nTitleHeight+nSize*EQS_CharHeight((BYTE)' ')-1,\
											  EQS_GetBackColor());								 

						}
#endif
					}
				}
				else
				{   // normal case 
#ifdef CONFIG_EQS_COLOR_64K
					EQS_AddScrollBar(NULL, pScrollList->pScrollBar,SBT_VERT,
									 EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth-1,\
									 EQC_GET_Y(hControl)+nTitleHeight,\
									 nSize*EQS_CharHeight((BYTE)' '),\
									 0,100,nSize,pScrollList->nLogicalLine);

#elif
					EQS_AddScrollBar(NULL, pScrollList->pScrollBar,SBT_VERT,
									 EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth-1,\
									 EQC_GET_Y(hControl)+nTitleHeight,\
									 EQC_GET_DY(hControl)-nTitleHeight-nStatusBarHeight,\
									 0,100,nSize,pScrollList->nLogicalLine);
#endif

					SBarInfo.nMax = pScrollList->nLogicalLine - nSize;
					SBarInfo.nMin = 0;                            
					SBarInfo.nPageSize = nSize;
					SBarInfo.nPos = pScrollList->nLogicalIndex - pScrollList->nCursor;
					EQS_SetScrollBarInfo(pScrollList->pScrollBar, &SBarInfo, TRUE);
				}            
			}
/*
			else
			{
#ifdef CONFIG_EQS_COLOR_64K			
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
				{
					EQS_FillRectangle(EQC_GET_X(hControl)+EQC_GET_DX(hControl)-3,\
									  EQC_GET_Y(hControl)+nTitleHeight,\
									  EQC_GET_X(hControl)+EQC_GET_DX(hControl)+1,\
									  EQC_GET_Y(hControl)+nTitleHeight+nSize*EQS_CharHeight((BYTE)' ')-1,\
									  EQS_GetBackColor());								 

				}
#endif
			}
*/
		} //if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR  &&  pScrollList->fShowScrollBar )

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		{	
			COLOR OldTextBackColor, OldTextColor, OldForeColor;
			BYTE  szBuf[10];
			UINT8 nLen, nWidth;

			OldTextBackColor = EQS_GetTextBackColor();
			OldTextColor     = EQS_GetTextColor();
			OldForeColor     = EQS_GetForeColor();
			EQS_SetTextBackColor(LIGHTGRAY);
			EQS_SetTextColor(BLUE);
 

			StartY = EQC_GET_Y(hControl) + EQC_GET_DY(hControl) - EQS_CharHeight((BYTE)' ');
			EQS_FillRectangle(EQC_GET_X(hControl),StartY,EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
							 StartY+EQS_CharHeight((BYTE)' ')-1, LIGHTGRAY);//CAPTION_BACK_COLOR);
#if 0
			sprintf((char*)szBuf,"%3d/%d", pScrollList->nLogicalIndex+1, pScrollList->nLogicalLine);

			nLen   = STRLEN(szBuf);
			nWidth = EQS_CharWidth((BYTE)' ')*(nLen+1);		
			EQS_TextOutStrLenTDM(EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth-1,\
								 StartY, szBuf, nLen, TDM_NORMAL);
#endif
						
			nWidth = 0;
			if( STRLEN(pScrollList->szStatusText) > 0 )
			{	
#if 0
				EQS_SetTextColor(BLACK);
				nLen = STRLEN(pScrollList->szStatusText);
				StartX = EQC_GET_X(hControl)+(EQC_GET_DX(hControl) - nWidth - EQS_CharWidth((BYTE)' ') * nLen)/2;
				EQS_TextOutStrLenTDM(StartX, StartY, pScrollList->szStatusText, nLen, TDM_NORMAL);
#endif
				UINT8 nLength ;
				BYTE  szText[30];
				memset(szText, 0, 30);

				nLength = pScrollList->nViewChar;
				if( STRLEN(pScrollList->szStatusText) > nLength )
				{
					if( IsMiddleOfMultiByte(pScrollList->szStatusText, nLength-2) )
					{
						STRNCPY(szText, pScrollList->szStatusText, nLength -3);
						STRNCPY(szText+nLength-3, (BYTE*)SF_ETC_SYMBOL,2);
					}
					else
					{
						STRNCPY(szText, pScrollList->szStatusText, nLength-2);
						STRNCPY(szText+ nLength-2, (BYTE*)SF_ETC_SYMBOL,2);
					}
				}
				else
				{
					STRCPY(szText, pScrollList->szStatusText);
				}
				EQS_SetTextColor(BLACK);
				nLength = STRLEN(szText);
				StartX = EQC_GET_X(hControl)+(EQC_GET_DX(hControl) - nWidth - EQS_CharWidth((BYTE)' ') * nLength)/2;
				if( StartX < EQC_GET_X(hControl) )
				{
					StartX = EQC_GET_X(hControl) + 3; // make space
				}
				EQS_TextOutStrLenTDM(StartX, StartY, szText, nLength, TDM_NORMAL);
			}
					
			//EQS_SetForeColor(WHITE);
			//EQS_Line(EQC_GET_X(hControl), StartY+1, EQC_GET_X(hControl)+EQC_GET_DX(hControl), StartY+1);

			EQS_SetTextBackColor(OldTextBackColor);
			EQS_SetTextColor(OldTextColor);
			EQS_SetForeColor(OldForeColor);
			
			
	/*
			//EQS_SetForeColor(R_WHITE); 
			//EQS_Line(EQC_GET_X(hControl), StartY+1, EQC_GET_X(hControl)+EQC_GET_DX(hControl), StartY+1);
			StartY = EQC_GET_Y(hControl) + EQC_GET_DY(hControl) - EQS_CharHeight((BYTE)' ')+1;
			if( EQS_IsActive(hControl)  )
			{
				EQS_Draw3DBorder(EQC_GET_X(hControl),StartY,EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
										StartY+EQS_CharHeight((BYTE)' ')-1);
			}
			else
			{
				EQS_Draw3DReverseBorder(EQC_GET_X(hControl),StartY,EQC_GET_X(hControl)+EQC_GET_DX(hControl)-1,\
										StartY+EQS_CharHeight((BYTE)' ')-1);
			}
	*/	
		}
		
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_BORDER  &&\
			!( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE)) //!( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP ) &&
		{
       			
			EQC_DrawBorder(hControl,\
						   EQC_GET_X(hControl), EQC_GET_Y(hControl),  //redstar 2001.01.15
						   (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
						   (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1) );
		}


#if 0
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP ) 
		{	
			EQS_Draw3DBox( EQC_GET_X(hControl), EQC_GET_Y(hControl),  
						   (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1), 
						   (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1) );
		}
#endif

		if( EQS_HaveFocus(hControl) && EQC_GET_ENABLE(hControl) == TRUE)
		{
			if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_NO_CML) )
			{
				EQS_EVENT  CurrentEvent;
				EQS_DrawIOCCML(IOC_CML_SELECT_CENTER);

				CurrentEvent = EQS_GetEvent();
				if( CurrentEvent.wParam == hControl )
				{
					EQS_UpdateIOCCML();
				}
			}
		}

	}

	if( pScrollList->ScrollType == ST_DRAW || pScrollList->ScrollType == ST_REDRAW )
	{
		EQS_UpdateControlDisplay();
	}
	else if( pScrollList->ScrollType == ST_SCROLL )
	{

		EQS_RefreshDisplayRect(hControl, EQC_GET_X(hControl), EQC_GET_Y(hControl)+nTitleHeight,\
			                   EQC_GET_X(hControl)+EQC_GET_DX(hControl),\
							   EQC_GET_Y(hControl)+ nViewLine*EQS_CharHeight((BYTE)' ')+nTitleHeight );

		//EQS_UpdateControlDisplay();
	}
	else ;

	pScrollList->ScrollType = ST_DRAW;

    EQS_ReleaseDC();

}

HCONTROL EQS_API EQC_ScrollCombo(COORD x, COORD y, UINT8 nChar, UINT8 nViewLine, 
                                 BYTE **ppszInit, UINT8 nPhysicalLine, 
                                 WORD nMaxLine, UINT8 nIndex)
{
   
    SCROLL_LIST_T   *pScrollList;
    
    HCONTROL hControl = EQC_ScrollList(x, y, nChar, nViewLine, \
                                       ppszInit, nPhysicalLine, nMaxLine, nIndex);

    pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );
   
    EQC_SET_STYLE(hControl, CS_STATIC_TEXT_BORDER |\
                            CS_SCROLL_LIST_COMBO  |\
                            CS_SCROLL_LIST_SCROLL_BAR);

    EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
    EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);

    return hControl;    
}


/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_ScrollList(COORD x, COORD y, UINT8 nChar, UINT8 nViewLine, 
                                BYTE **ppszInit, UINT8 nPhysicalLine, 
                                WORD nMaxLine, UINT8 nIndex)
{
    SCROLL_LIST_T   *pScrollList;
	void *pInstance;
	HCONTROL hControl;
    UINT8 nWidth, nHeight;
	
	pInstance = sAllocInstance(ppszInit, nPhysicalLine, nMaxLine);

    nWidth  = EQS_CharWidth((BYTE)' ')  * nChar ;
    nHeight = EQS_CharHeight((BYTE)' ') * nViewLine ;
/* Original - woodstok 01.1.8
	hControl= EQC_AllocControl(CT_SCROLL_LIST, pInstance, x, y, nWidth , nHeight, 
                               sHandleEvent, sFreeInstance);
*/
	hControl= EQC_AllocControl(CT_SCROLL_LIST, pInstance, x, y, nWidth , nHeight, 
                               sHandleEvent);

    ASSERT (hControl != INVALID_CONTROL_HANDLE);

    pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

	pScrollList->nViewChar     = nChar;
	pScrollList->nViewLine     = nViewLine;
    pScrollList->nCombo        = nChar; 

    if( nIndex >= nPhysicalLine )
		nIndex = nPhysicalLine - 1;
    pScrollList->nIndex         =  nIndex;
    pScrollList->nLogicalIndex  =  nIndex;
    if( pScrollList->nLogicalLine <= nViewLine )
    {
        pScrollList->nCursor = pScrollList->nIndex;
    }
    else
    {
        if( pScrollList->nIndex >= nViewLine && nViewLine > 0 )
        {
            pScrollList->nCursor = nViewLine-1;
        }
        else
        {
            pScrollList->nCursor = pScrollList->nIndex;
        }
    }

    EQC_SET_STYLE(hControl, CS_SCROLL_LIST_DEFAULT);	
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );

    return hControl;
}
 
UINT8 EQS_API EQC_GetScrollPhysicalIndex(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    return pScrollList->nIndex;
}

WORD EQS_API EQC_GetScrollLogicalIndex(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    return pScrollList->nLogicalIndex;
}

UINT8 EQS_API EQC_GetScrollCursor(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    return pScrollList->nCursor;
}

BOOL EQS_API  EQC_SetScrollCursor(HCONTROL hControl, UINT8 nCursor)
{
    SCROLL_LIST_T *pScrollList;
	INT nViewLine;

	ASSERT( EQC_GET_TYPE(hControl) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

	nViewLine = pScrollList->nViewLine;
	if( pScrollList->fShowTitle )
		nViewLine--;
	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;
	if( nCursor < nViewLine )
	{
		pScrollList->nCursor = nCursor;
		return TRUE;
	}
	return FALSE;
}

BYTE* EQS_API EQC_GetScrollData(HCONTROL hControl, UINT8 nPos)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl ) ;
    
    if (nPos < pScrollList->nPhysicalLine)
    {
        return (BYTE*) pScrollList->ppNode[nPos];

    }
    else
        return NULL;
}


void EQS_API EQC_SetScrollPhysicalIndex(HCONTROL hControl, UINT8 iIndex)
{
    SCROLL_LIST_T *pScrollList;
    UINT8 nViewLine, index, i;
	INT16  nTabIndex = 0;


	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );
    pScrollList->nIndex        = iIndex;
    pScrollList->nLogicalIndex = iIndex;
    
    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine - 1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;

    if( iIndex < nViewLine )
        pScrollList->nCursor = iIndex;
    else
    {
        pScrollList->nCursor = nViewLine-1;
    }

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
	{
		for(i=0; i< pScrollList->pTab->nTabSize; i++)
		{
			nTabIndex += pScrollList->pTab->wTabDataSize[i];
		    if( pScrollList->nLogicalIndex < nTabIndex || (i == (pScrollList->pTab->nTabSize-1)) )
			{				
				index =  pScrollList->nLogicalIndex - (nTabIndex - pScrollList->pTab->wTabDataSize[i]);
				
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
				{
					nViewLine = pScrollList->nViewLine - 2;
				}
				else
				{
					nViewLine = pScrollList->nViewLine - 1;
				}

				if( index >=  nViewLine )
				{
					pScrollList->nCursor = nViewLine - 1;
				}
				else
				{
					pScrollList->nCursor = index ;
				}

				if( i > pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor +i - pScrollList->pTab->nCurrentTab ) > (pScrollList->pTab->nViewTab-1) )
					{
						pScrollList->pTab->nTabCursor = pScrollList->pTab->nViewTab - 1;
					}
					else
					{
						pScrollList->pTab->nTabCursor += (i-pScrollList->pTab->nCurrentTab);
					}
				}
				else if( i < pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor - (pScrollList->pTab->nCurrentTab - i) ) <= 0 )
					{
						pScrollList->pTab->nTabCursor = 0;		
					}
					else
					{   
						pScrollList->pTab->nTabCursor -= (pScrollList->pTab->nCurrentTab-i);
					}
				}
				else ;

				pScrollList->pTab->nCurrentTab = i;
				break;
			}			
		}
		EQS_SendEvent( EQE_IO_SCROLL_TAB_CHG, hControl, pScrollList->pTab->nCurrentTab);
	}   
    EQS_SendEvent( EQE_APP_DRAW, EQS_NA, EQS_NA); //tab일경우 깜박이는 문제때문에 Draw All 한다.
	pScrollList->ScrollType = ST_REDRAW;
}

void EQS_API EQC_SetScrollLogicalIndex(HCONTROL hControl, WORD iIndex)
{
    SCROLL_LIST_T *pScrollList;
    UINT8 nViewLine, index, i;
	INT16 nTabIndex = 0;


	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );    
    pScrollList->nLogicalIndex = iIndex;
    
    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine - 1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;


	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
	{
		for(i=0; i< pScrollList->pTab->nTabSize; i++)
		{
			nTabIndex += pScrollList->pTab->wTabDataSize[i];
		    if( pScrollList->nLogicalIndex < nTabIndex || (i == (pScrollList->pTab->nTabSize-1)) )
			{				
				index =  pScrollList->nLogicalIndex - (nTabIndex - pScrollList->pTab->wTabDataSize[i]);
				
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
				{
					nViewLine = pScrollList->nViewLine - 2;
				}
				else
				{
					nViewLine = pScrollList->nViewLine - 1;
				}


				if( index >=  nViewLine )
				{
					pScrollList->nCursor = nViewLine - 1;
				}
				else
				{
					pScrollList->nCursor = index ;
				}
				

				if( i > pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor +i - pScrollList->pTab->nCurrentTab ) > (pScrollList->pTab->nViewTab-1) )
					{
						pScrollList->pTab->nTabCursor = pScrollList->pTab->nViewTab - 1;
					}
					else
					{
						pScrollList->pTab->nTabCursor += (i-pScrollList->pTab->nCurrentTab);
/*
						pScrollList->pTab->nTabCursor =  i - pScrollList->pTab->nCurrentTab ;
						if( i + pScrollList->pTab->nViewTab - pScrollList->pTab->nTabCursor > pScrollList->pTab->nTabSize )
						{
							pScrollList->pTab->nTabCursor =  pScrollList->pTab->nViewTab - (pScrollList->pTab->nTabSize - i);
						}
*/
					}
				}
				else if( i < pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor - (pScrollList->pTab->nCurrentTab - i) ) <= 0 )
					{
						pScrollList->pTab->nTabCursor = 0;		
					}
					else
					{   
						pScrollList->pTab->nTabCursor -= (pScrollList->pTab->nCurrentTab-i);
/*
						if( pScrollList->pTab->nTabCursor >= (pScrollList->pTab->nCurrentTab - i ) )
						{
							pScrollList->pTab->nTabCursor -= (pScrollList->pTab->nCurrentTab - i);
						}
						else
						{
							pScrollList->pTab->nTabCursor = 0;
						}
*/
					}
				}
				else ;

				pScrollList->pTab->nCurrentTab = i;
				break;
			}			
		}

		EQS_SendEvent( EQE_IO_SCROLL_TAB_CHG, hControl, pScrollList->pTab->nCurrentTab);
	} 
	else
	{
		if( iIndex < nViewLine )
		{
			pScrollList->nCursor = iIndex;
		}
		else
		{
			pScrollList->nCursor = nViewLine-1;            
		}
		
	}

	if( pScrollList->fLogicalList )
	{
		pScrollList->nIndex = pScrollList->nCursor;
	}
	else
	{
		pScrollList->nIndex = pScrollList->nLogicalIndex;
	}

	EQS_SendEvent(EQE_IO_SCROLL_LIST_BOTTOM,(WORD)hControl,\
							 (DWORD)(iIndex-pScrollList->nCursor));
			            
    EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);
	pScrollList->ScrollType = ST_REDRAW;
}


void EQS_API EQC_SetScrollData(HCONTROL hControl, \
                                     UINT8 nIndex, BYTE* pszStr)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

    pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    STRCPY( (BYTE*)((pScrollList->ppNode)[nIndex]), pszStr );
        
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0);

}

//redstar 2000.11.17

INT16 EQS_API EQC_AddScrollData(HCONTROL hControl,UINT8 nIndex, \
                                            BYTE* pszStr, BOOL fIncreaseViewLine)
{
    SCROLL_LIST_T *pScrollList;
    INT16 i;
    BYTE** ppBuf;
    UINT8  nViewLine;
    
    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

    pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    if( nIndex > pScrollList->nLogicalLine )
        return -1;

    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine -1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;

    if( pScrollList->fLogicalList == FALSE )
    {
        if( pScrollList->nAllocLine > pScrollList->nPhysicalLine )
        {
            for( i=pScrollList->nPhysicalLine-1; i >= nIndex; i-- )
            {
                pScrollList->ppNode[i+1] = pScrollList->ppNode[i];
            }
            pScrollList->ppNode[nIndex] = pszStr;
        }
        else
        {
            pScrollList->nAllocLine += 10;
            ppBuf = (BYTE **)EQS_Malloc((SIZE_T) \
                                   ((pScrollList->nAllocLine) * sizeof(BYTE*)));
            for( i=0; i<nIndex; i++)
            {
                ppBuf[i] = pScrollList->ppNode[i];
            }
            ppBuf[i] = pszStr;
            for( i= nIndex; i< pScrollList->nPhysicalLine; i++)
            {
                ppBuf[i+1] = pScrollList->ppNode[i];
            }                        
            EQS_Free((void**)&(pScrollList->ppNode) );
            pScrollList->ppNode = ppBuf;
           
        }        

		if( nIndex < nViewLine -1 )
		{
			pScrollList->nCursor        = nIndex;
			pScrollList->nIndex         = nIndex;
			pScrollList->nLogicalIndex  = nIndex;
		}
		else
		{
			pScrollList->nCursor        = nViewLine -1;
			pScrollList->nIndex         = nIndex;
			pScrollList->nLogicalIndex  = nIndex;
		}

        pScrollList->nPhysicalLine++;
        pScrollList->nLogicalLine++;     
		
    }
    else  // if( pScrollList->fLogicalList == TRUE )
    {
#if 0
        if( fIncreaseViewLine == TRUE )
            pScrollList->nViewLine++;
        pScrollList->nLogicalLine++;       
        pScrollList->nLogicalIndex  = nIndex;
        if( nIndex == 0 )
        {
            pScrollList->nCursor = 0;
        }
        else if( nIndex == pScrollList->nLogicalLine-1 )
        {
            if( nViewLine < pScrollList->nLogicalLine )
                pScrollList->nCursor = nViewLine -1;
            else
                pScrollList->nCursor = pScrollList->nLogicalLine -1;
        }
        else ;

        pScrollList->nIndex      = pScrollList->nCursor;
#endif
        pScrollList->nLogicalLine++;       
        pScrollList->nLogicalIndex  = nIndex;
		//pScrollList->nIndex  =  0;
		if( nIndex >= nViewLine )
		{			
			pScrollList->nCursor =  nViewLine-1;
			
		}
		else
		{
			pScrollList->nCursor = nIndex;			
		}
		pScrollList->nIndex = pScrollList->nCursor;
    }  
	pScrollList->ScrollType = ST_REDRAW;
	return (pScrollList->nLogicalIndex - pScrollList->nCursor);  
}


INT16 EQS_API EQC_DeleteScrollData(HCONTROL hControl, UINT8 nLogicalIndex)
{
    SCROLL_LIST_T *pScrollList;
    UINT8 i, nViewLine;
    WORD DataIndex;
    

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);

    pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );
    
    if( nLogicalIndex >=  pScrollList->nLogicalLine )
        return -1;

    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine -1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;

    if( pScrollList->fLogicalList == FALSE )
    {
        // delete room for new list data        
        for (i = nLogicalIndex; i < pScrollList->nPhysicalLine - 1; i++)
        {
            (pScrollList->ppNode)[i] = (pScrollList->ppNode)[i+1];
        }
        if( pScrollList->nPhysicalLine == pScrollList->nLogicalLine )
            pScrollList->nPhysicalLine = pScrollList->nPhysicalLine - 1;


        pScrollList->nLogicalLine  = pScrollList->nLogicalLine  - 1;


        if( pScrollList->nLogicalIndex != 0 )
        {
            pScrollList->nIndex--;
            pScrollList->nLogicalIndex--;
        }


        if( pScrollList->nCursor >= nViewLine )
        {
            pScrollList->nCursor  = nViewLine - 1;            
        }
        else if( pScrollList->nCursor >= pScrollList->nPhysicalLine )
        {
            pScrollList->nCursor  = pScrollList->nPhysicalLine -1;
        }
        else ;

		if( pScrollList->nCursor > pScrollList->nIndex )
			pScrollList->nCursor = pScrollList->nIndex;

        EQS_SendEvent( EQE_APP_DRAW,(WORD) hControl, 0);
		pScrollList->ScrollType  = ST_REDRAW;

        return 1;
    }
    else  // if( pScrollList->fLogicalList == TRUE )
    {
        pScrollList->nLogicalLine--;
        if( nViewLine > pScrollList->nLogicalLine )
        {
            if( pScrollList->nLogicalIndex >= pScrollList->nLogicalLine )//redstar
            {
                pScrollList->nLogicalIndex--;
                pScrollList->nIndex = (UINT8)pScrollList->nLogicalIndex;
            }          
            if( pScrollList->nCursor >= pScrollList->nLogicalLine )
                pScrollList->nCursor--;
            else ;

            DataIndex = pScrollList->nLogicalIndex - pScrollList->nCursor;
        }
        else
        {
            DataIndex = pScrollList->nLogicalIndex - pScrollList->nCursor;
            if( DataIndex + nViewLine -1 >= pScrollList->nLogicalLine )
            {
                pScrollList->nLogicalIndex--;
                DataIndex--;
            }

            pScrollList->nIndex = pScrollList->nCursor;            
        }
        pScrollList->ScrollType = ST_REDRAW;
        return  DataIndex;            
    }
}

UINT8 EQS_API EQC_GetScrollViewLine(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;
	UINT8          nViewLine;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );
    if( pScrollList->fShowTitle == TRUE )
        nViewLine =  (pScrollList->nViewLine - 1);
    else
        nViewLine =  pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;

	return nViewLine;
}

void EQS_API EQC_SetScrollViewLine(HCONTROL hControl, UINT8 nLine)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    pScrollList->nViewLine = nLine;
}

UINT8 EQS_API EQC_GetScrollDataLine(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    return pScrollList->nPhysicalLine;
}

WORD EQS_API EQC_GetScrollMaxDataLine(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    return pScrollList->nLogicalLine;
}

void EQS_API EQC_SetScrollMaxDataLine(HCONTROL hControl, UINT8 nLogicalLine)
{
    SCROLL_LIST_T *pScrollList;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
	pScrollList = (SCROLL_LIST_T *) EQC_GET_INSTANCE( hControl );

    pScrollList->nLogicalLine = nLogicalLine;
}

UINT8 EQS_API EQC_ScrollMaxStrLen(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    return pScrollList->nMaxStrLen;
}

void EQS_API EQC_SetScrollComboSize(HCONTROL hControl, UINT8 nChar)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
	{
		pScrollList->nCombo = nChar;
		EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*nChar);
	}

}


UINT8 EQS_API EQC_GetScrollComboSize(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    return pScrollList->nCombo;
}

void EQS_API EQC_SetScrollList(HCONTROL hControl, UINT8 nPos)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    if( pScrollList->pafCheckBox == NULL )
    {            
        pScrollList->pafCheckBox = \
                      (BOOL*) EQS_Malloc(sizeof(BOOL)*pScrollList->nLogicalLine);
        memset((void*)pScrollList->pafCheckBox, 0,
                sizeof(BOOL)*pScrollList->nLogicalLine );
    }
   
    pScrollList->pafCheckBox[nPos] = TRUE;   
}


void EQS_API EQC_ResetScrollList(HCONTROL hControl, UINT8 nPos)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    if( pScrollList->pafCheckBox == NULL )
    {            
        pScrollList->pafCheckBox = \
                      (BOOL*) EQS_Malloc(sizeof(BOOL)*pScrollList->nLogicalLine);
        memset((void*)pScrollList->pafCheckBox, 0,
                sizeof(BOOL)*pScrollList->nLogicalLine );
    }   

    pScrollList->pafCheckBox[nPos] = FALSE;   
}

BOOL EQS_API EQC_GetScrollList(HCONTROL hControl, UINT8 nPos)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    if( pScrollList->pafCheckBox == NULL )
    {            
        pScrollList->pafCheckBox = \
                      (BOOL*) EQS_Malloc(sizeof(BOOL)*pScrollList->nLogicalLine);
        memset((void*)pScrollList->pafCheckBox, 0,
                sizeof(BOOL)*pScrollList->nLogicalLine );
    }   

    return pScrollList->pafCheckBox[nPos];
}

void EQS_API EQC_SetScrollTitle(HCONTROL hControl, BYTE* pszStr)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    STRNCPY( pScrollList->szTitle, pszStr,MAX_TITLE_SIZE-1);
	pScrollList->szTitle[MAX_TITLE_SIZE-1] = 0;
}

BYTE* EQS_API EQC_GetScrollTitle(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
    	
    return pScrollList->szTitle;
}

void EQS_API EQC_SetScrollListHandle(HCONTROL hControl, UINT8 hList)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    pScrollList->ListHandle = hList;
}

UINT8 EQS_API EQC_GetScrollListHandle(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    return pScrollList->ListHandle;
}


void EQS_API EQC_ShowScrollTitle(HCONTROL hControl, BOOL fShow)
{    
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    pScrollList->fShowTitle = fShow;
	if( fShow == TRUE )
	{
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		{
			if( pScrollList->nCursor == (pScrollList->nViewLine -2) )
				pScrollList->nCursor = pScrollList->nCursor - 1;
		}
		else
		{
			if( pScrollList->nCursor == (pScrollList->nViewLine -1) )
				pScrollList->nCursor = pScrollList->nCursor - 1;
		}
#ifdef CONFIG_EQS_COLOR_64K
		if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE) &&\
			!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO)    &&\
			!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST) )
		{
			EQC_SET_DY(hControl, EQC_GET_DY(hControl) + 7 ); // difference with title and normal line
		}
#endif
	}  
	else
	{
#ifdef CONFIG_EQS_COLOR_64K
		if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE) &&\
			!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO)    &&\
			!(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST) )
		{
			EQC_SET_DY(hControl, EQC_GET_DY(hControl) - 7 ); // difference with title and normal line
		}
#else
		;
#endif

	}
}
void EQS_API EQC_FreeScrollMemory(HCONTROL hControl, BOOL fFree)
{

    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    pScrollList->fFreeMemory = fFree;

}


void EQS_API EQC_ScrollAdjust(HCONTROL hControl)
{

    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
		EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
    {
        EQC_SET_DX( hControl, EQS_CharWidth((BYTE)' ')*(pScrollList->nCombo+2));
        EQC_SET_DY( hControl, EQS_CharHeight((BYTE)' '));
    }

}

void EQS_API EQC_SetLogicalList(HCONTROL hControl, BOOL fLogical)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    pScrollList->fLogicalList = fLogical;
}

void EQS_API EQC_RePositionScrollList(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

    pScrollList->nIndex        = 0;
    pScrollList->nLogicalIndex = 0;
    pScrollList->nCursor       = 0;
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );
}

UINT8 EQS_API EQC_GetScrollDisplayLine(void)
{
    UINT8  nLines;
    RECT   WindowRect;
    COORD  DY;    
    
    EQS_GetSWindowViewAreaRect(&WindowRect);

    DY = WindowRect.yBottom - WindowRect.yTop;
  
    nLines =  DY / EQS_CharHeight((BYTE)' ');

    return nLines;
}


BOOL  EQS_API  EQC_SetScrollAccelKey(HCONTROL hControl,UINT8 nIndex, ACCEL_KEY_T AKey)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED )
	{
		if( nIndex < pScrollList->nLogicalLine && nIndex < MAX_ACCEL_KEY )
		{
			pScrollList->pAccelKey[nIndex] = AKey;
			return TRUE;
		}
	}			
	return FALSE;
}

ACCEL_KEY_T     EQS_API  EQC_GetScrollAccelKey(HCONTROL hControl,UINT8 nIndex)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

		
	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED )
	{
		if( nIndex < pScrollList->nLogicalLine && nIndex < MAX_ACCEL_KEY )
		{
			return (ACCEL_KEY_T) pScrollList->pAccelKey[nIndex];
		}
	}			
	return ACCEL_KEY_1 ;
}
void  EQS_API  EQC_SetScrollStyle(HCONTROL hControl, WORD StyleA)
{
    RECT   WindowRect;
    SCROLL_LIST_T *pScrollList;
    UINT8  nViewLine, i, nSpace;
      

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	pScrollList->ScrollType = ST_REDRAW;

    EQC_GET_INSTANCE(hControl)->Style = (StyleA);
	

    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
		EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST)  // "combo box" attribute
    {
        EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
        EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);
    }

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED )
	{
		if( pScrollList->pAccelKey == NULL )
		{
			pScrollList->pAccelKey = (UINT8*) EQS_Malloc(sizeof(UINT8)* MAX_ACCEL_KEY);
			for( i=0; i< MAX_ACCEL_KEY-3; i++)
				pScrollList->pAccelKey[i] = ACCEL_KEY_1 + i;

			pScrollList->pAccelKey[i]   = ACCEL_KEY_NONE;
			pScrollList->pAccelKey[i+1] = ACCEL_KEY_NONE;
			pScrollList->pAccelKey[i+2] = ACCEL_KEY_NONE;

		}
	}

    EQS_GetSWindowViewAreaRect(&WindowRect);
	
    //동적으로 스크롤 리스트를 사용하는 경우 사용되는 버퍼가 view line 보다 하나 더 커야 하는데
    //maximize 속성에서 자동적으로 view line을 조절하는 위의 조건에 위배되게 된다.
    if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
    {		
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
		{	
			nSpace = 0;
#ifdef CONFIG_EQS_COLOR_64K
			EQC_SET_X(hControl, 1);//0//2*nSpace+1);
#else
			EQC_SET_X(hControl, 0);
#endif
			EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-4*nSpace);
		}
		else
		{			
			nSpace = 0;
#ifdef CONFIG_EQS_COLOR_64K
			EQC_SET_X(hControl, 1);
#else
			EQC_SET_X(hControl, nSpace);
#endif
			EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1);
		}
				
		pScrollList->nViewChar = (WindowRect.xRight-WindowRect.xLeft+1)/EQS_CharWidth((BYTE)' ');
		pScrollList->nCombo    = pScrollList->nViewChar;  
        

		EQC_SET_Y(hControl,nSpace);
        EQC_SET_DY(hControl, WindowRect.yBottom-WindowRect.yTop+1-2*nSpace);

        if( pScrollList->fLogicalList == FALSE )        
        {     
            nViewLine = EQC_GET_DY(hControl) / EQS_CharHeight((BYTE)' ');
            pScrollList->nViewLine = nViewLine;

			if( pScrollList->nIndex >= nViewLine && nViewLine > 0 )
			{
				pScrollList->nCursor = nViewLine-1;
			}
			else
			{
				pScrollList->nCursor = pScrollList->nIndex;
			}
        }
    }
	else if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_HMAXIMIZE )
	{
		EQC_SET_X(hControl,2);	
		pScrollList->nViewChar = (WindowRect.xRight-WindowRect.xLeft+1) / EQS_CharWidth((BYTE)' ');
		pScrollList->nCombo    = pScrollList->nViewChar;
		EQC_SET_DX(hControl, WindowRect.xRight-WindowRect.xLeft+1-4);
	}
	else if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
	{
		EQC_SET_DY(hControl, EQC_GET_DY(hControl) + 2 );
	}
	else
		;

    if( pScrollList->fShowTitle == TRUE )
        nViewLine = pScrollList->nViewLine -1;
    else
        nViewLine = pScrollList->nViewLine;

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
		nViewLine--;

	if( pScrollList->nCursor >= nViewLine )
	{
		pScrollList->nCursor = 0;
		pScrollList->nIndex  = 0;
		pScrollList->nLogicalIndex = 0;
	}     

   
	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_TAB )
	{
		if( pScrollList->pTab == NULL )
		{
			pScrollList->pTab = (TAB_LIST_T*) EQS_Malloc( sizeof(TAB_LIST_T) );
			pScrollList->pTab->nCurrentTab   = 0;
			pScrollList->pTab->nTabSize      = 6;
			pScrollList->pTab->nViewTab      = 4;
			pScrollList->pTab->nTabCursor    = 0;
			pScrollList->pTab->fEqual        = TRUE;
			pScrollList->fShowTitle          = TRUE;
		}
	}
}

BYTE* EQS_API  EQC_GetScrollMemory(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;    
      
    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	return NULL;
}

void  EQS_API  EQC_SetScrollStatusBarText(HCONTROL hControl, BYTE* szStr)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    STRNCPY( pScrollList->szStatusText, szStr,MAX_STATUS_TEXT_SIZE-1);
	pScrollList->szStatusText[MAX_STATUS_TEXT_SIZE-1] = 0;
	EQS_SendEvent( EQE_APP_DRAW, hControl, EQS_NA);

}

BYTE* EQS_API  EQC_GetScrollStatusBarText(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
   
    return pScrollList->szStatusText;

}

BOOL EQC_SetScrollTab(HCONTROL hControl, UINT8 nTabSize, UINT8 nViewTab, BOOL fEqualSize, UINT8 nIndex)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab != NULL )
	{
		if( nTabSize > MAX_TAB_SIZE )
			pScrollList->pTab->nTabSize = MAX_TAB_SIZE;
		else
			pScrollList->pTab->nTabSize = nTabSize;
		pScrollList->pTab->nViewTab = nViewTab;
		pScrollList->pTab->fEqual   = fEqualSize;
		pScrollList->pTab->nCurrentTab = nIndex;
		if( nIndex >= nViewTab )
		{
			pScrollList->pTab->nTabCursor = nViewTab -1;
		}
		else
		{
			pScrollList->pTab->nTabCursor = nIndex;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;

}
BOOL EQC_SetScrollTabData(HCONTROL hControl, WORD* pwDataSize, BYTE** ppText)
{
    SCROLL_LIST_T *pScrollList;	
	UINT8   i, index, nViewLine;
	INT16   nTabIndex = 0;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab != NULL )
	{
		for(i=0; i< pScrollList->pTab->nTabSize; i++)
		{
			pScrollList->pTab->wTabDataSize[i] = pwDataSize[i];
			pScrollList->pTab->pTabText[i]     = ppText[i];
		}
		for(i=0; i< pScrollList->pTab->nTabSize; i++)
		{
			nTabIndex += pScrollList->pTab->wTabDataSize[i];
		    if( pScrollList->nLogicalIndex < nTabIndex || (i == (pScrollList->pTab->nTabSize-1)) )
			{				
				index =  pScrollList->nLogicalIndex - (nTabIndex - pScrollList->pTab->wTabDataSize[i]);
				
				if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_STATUS_BAR )
				{
					nViewLine = pScrollList->nViewLine - 2;
				}
				else
				{
					nViewLine = pScrollList->nViewLine - 1;
				}

				if( index >=  nViewLine )
				{
					pScrollList->nCursor = nViewLine - 1;
				}
				else
				{
					pScrollList->nCursor = index ;
				}

				if( i > pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor +i - pScrollList->pTab->nCurrentTab ) > (pScrollList->pTab->nViewTab-1) )
					{
						pScrollList->pTab->nTabCursor = pScrollList->pTab->nViewTab - 1;
					}
					else
					{
						pScrollList->pTab->nTabCursor += (i-pScrollList->pTab->nCurrentTab);
					}
				}
				else if( i < pScrollList->pTab->nCurrentTab )
				{
					if( (pScrollList->pTab->nTabCursor - (pScrollList->pTab->nCurrentTab - i) ) <= 0 )
					{
						pScrollList->pTab->nTabCursor = 0;		
					}
					else
					{   
						pScrollList->pTab->nTabCursor -= (pScrollList->pTab->nCurrentTab-i);
					}
				}
				else ;

				pScrollList->pTab->nCurrentTab = i;
				break;
			}			
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

LOCAL void sDrawScrollTab(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;
	COLOR OldColor, OldTextBackColor, OldTextColor, TextColor, FillColor;
	UINT8 nFontHeight,nWidth, nActiveTabCell, nMargin, i, nStart, nEnd;
	COORD StartY, StartX, EndX;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab->fEqual )
	{
		nActiveTabCell = 0;
	}
	else
	{
		nActiveTabCell = 2;
	}

#ifdef CONFIG_EQS_COLOR_64K
	StartY = 3;

#else
    StartY = 0;
#endif

	nFontHeight = EQS_CharHeight((BYTE)' ');
				
	nWidth = EQC_GET_DX(hControl) / (pScrollList->pTab->nViewTab+nActiveTabCell);
    nMargin = EQC_GET_DX(hControl) % (pScrollList->pTab->nViewTab+nActiveTabCell);

	OldColor = EQS_GetForeColor();
	EQS_SetForeColor(BLACK);
	TextColor = EQS_GetTextColor();

	nStart = pScrollList->pTab->nCurrentTab - pScrollList->pTab->nTabCursor;
	nEnd   = nStart + pScrollList->pTab->nViewTab;
	OldTextBackColor = EQS_GetTextBackColor();
	for(i= nStart; i< nEnd; i++)
	{
		if( (i-nStart) != pScrollList->pTab->nTabCursor )
		{
			if( pScrollList->pTab->fEqual )
			{
				nActiveTabCell = 0;
				if( (i-nStart) < pScrollList->pTab->nTabCursor )
				{
					StartX = (i-nStart+nActiveTabCell)*nWidth;
					EndX   = (i-nStart+1+nActiveTabCell)*nWidth;
				}
				else
				{
					StartX = (i-nStart+nActiveTabCell)*nWidth + nMargin;
					EndX   = (i-nStart+1+nActiveTabCell)*nWidth + nMargin;
				}
			}
			else if( (i-nStart) < pScrollList->pTab->nTabCursor )
			{
				nActiveTabCell = 0;		
				StartX = (i-nStart+nActiveTabCell)*nWidth;
				EndX   = (i-nStart+1+nActiveTabCell)*nWidth;
			}
			else 
			{
				nActiveTabCell = 2;							
				StartX = (i-nStart+nActiveTabCell)*nWidth + nMargin;
				EndX   = (i-nStart+1+nActiveTabCell)*nWidth + nMargin;
			}

			EQS_Draw3DReverseBorder( EQC_GET_X(hControl)+ StartX+1,
							  EQC_GET_Y(hControl) + StartY, 
							  EQC_GET_X(hControl) + EndX-1, 
							  EQC_GET_Y(hControl) + StartY+ nFontHeight );


#if TAB_ACTIVE_DRAW
			if( pScrollList->pTab->wTabDataSize[i]  )
			{
				FillColor = R_LIGHTGRAY;//EQS_GetBackColor();				
			}
			else
			{
				FillColor = R_GRAY;								
			}
#else 
			FillColor = GRAY;//RGB332(144,144,160);//GRAY;
#endif
			

			EQS_FillRectangle( EQC_GET_X(hControl)+ StartX+2,
				   EQC_GET_Y(hControl) + StartY, 
				   EQC_GET_X(hControl) + EndX-1, 
				   EQC_GET_Y(hControl) + StartY + nFontHeight, FillColor);//EQS_GetBackColor());

			
			EQS_SetTextBackColor(FillColor);//EQS_GetBackColor());

			if( pScrollList->pTab->wTabDataSize[i] == 0)
			{
				EQS_SetTextColor(LIGHTGRAY);//WHITE);
			}
			else
			{
				EQS_SetTextColor(BLACK);
			}


			EQS_TextOutStrLen(EQC_GET_X(hControl)+ StartX+3,\
				              EQC_GET_Y(hControl)+ StartY,\
							  pScrollList->pTab->pTabText[i],2);
								
			EQS_SetTextColor(TextColor);			

			EQS_Line( EQC_GET_X(hControl) + StartX+2,\
				      EQC_GET_Y(hControl)+ StartY+ nFontHeight,\
					  EQC_GET_X(hControl) + EndX-1,\
					  EQC_GET_Y(hControl) + StartY + nFontHeight );
			EQS_SetTextBackColor(OldTextBackColor);
		}
		else
		{

			if( pScrollList->pTab->fEqual )
			{
				nActiveTabCell = 0;
			}
			else
			{
				nActiveTabCell = 2;
			}
			StartX = (i-nStart)*nWidth;
			EndX   = (i-nStart+1+nActiveTabCell)*nWidth + nMargin;

			EQS_Draw3DReverseBorder( EQC_GET_X(hControl)+ StartX+2,
							  EQC_GET_Y(hControl) + StartY, 
							  EQC_GET_X(hControl) + EndX-3, 
							  EQC_GET_Y(hControl) + StartY + nFontHeight );

#if NORMAL_REVERSE
			FillColor = EQS_GetTextBackColor();
#else
			FillColor = EQS_GetBackColor();
			OldTextBackColor = EQS_GetTextBackColor();
			OldTextColor = EQS_GetTextColor();
			EQS_SetTextBackColor(WHITE);//FillColor);
			EQS_SetTextColor(BLACK);
#endif


			EQS_FillRectangle( EQC_GET_X(hControl)+ StartX+3,
				   EQC_GET_Y(hControl) + StartY, 
				   EQC_GET_X(hControl) + EndX-2, 
				   EQC_GET_Y(hControl) + StartY + nFontHeight, WHITE);//EQS_GetTextBackColor());


			if( pScrollList->pTab->fEqual )
			{
				EQS_TextOutStrLen(EQC_GET_X(hControl)+ StartX+3, EQC_GET_Y(hControl)+ StartY,\
								  pScrollList->pTab->pTabText[i], STRLEN(pScrollList->pTab->pTabText[0])); 
			}
			else
			{
				EQS_TextOutStr(EQC_GET_X(hControl)+ StartX+3, EQC_GET_Y(hControl)+ StartY,\
								  pScrollList->pTab->pTabText[i]); 
			}
#if !NORMAL_REVERSE
			EQS_SetTextBackColor(OldTextBackColor);
			EQS_SetTextColor(OldTextColor);
#endif

		}		
	}

	EQS_SetForeColor(OldColor);
	

}

WORD EQC_GetScrollTabIndex(HCONTROL hControl, UINT8 nTab)
{
    SCROLL_LIST_T *pScrollList;	
	UINT8 i;
	WORD wTabIndex = 0;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab == NULL )
		return wTabIndex;

	for(i=0; i< nTab; i++)
	{
		wTabIndex += pScrollList->pTab->wTabDataSize[i];
	}
	return wTabIndex;
}

UINT8 EQC_GetScrollTabSize(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		return pScrollList->pTab->nTabSize;
	}
	else
	{
		return 0;
	}

}

UINT8 EQC_GetScrollTabViewLine(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		return pScrollList->pTab->nViewTab;
	}
	else
	{
		return 0;
	}
}

WORD  EQC_GetScrollTabDataSize(HCONTROL hControl, UINT8 nTab)
{

    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		if( nTab < pScrollList->pTab->nTabSize )
			return pScrollList->pTab->wTabDataSize[nTab];
	}	
		
	return 0;	
}

UINT8 EQC_GetScrollCurrentTab(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		return pScrollList->pTab->nCurrentTab;
	}
	else
	{
		return 0;
	}

}

WORD  EQC_GetScrollCurrentTabDataSize(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		return pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab];
	}
	else
	{
		return 0;
	}
}


BOOL  EQC_SetScrollTabSize(HCONTROL hControl, UINT8 nTabSize)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	if( pScrollList->pTab )
	{
		pScrollList->pTab->nTabSize = nTabSize;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL  EQC_SetScrollCurrentTab(HCONTROL hControl, UINT8 nCurrentTab)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	if( pScrollList->pTab )
	{

		pScrollList->pTab->nCurrentTab = nCurrentTab;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL  EQC_SetScrollTabViewLine(HCONTROL hControl, UINT8 nTabViewLine)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	if( pScrollList->pTab )
	{
		pScrollList->pTab->nViewTab = nTabViewLine;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL  EQC_SetScrollTabDataSize(HCONTROL hControl, UINT8 nTab, WORD nDataSize)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	if( pScrollList->pTab )
	{
		if( nTab < pScrollList->pTab->nTabSize )
		{
			pScrollList->pTab->wTabDataSize[nTab] = nDataSize;
			return TRUE;
		}
	}				
	return FALSE;	
}

BOOL  EQC_SetScrollCurrentTabDataSize(HCONTROL hControl, WORD nDataSize)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	if( pScrollList->pTab )
	{
		pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab] = nDataSize;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


UINT8 EQC_GetScrollTabPos(HCONTROL hControl, WORD nIndex)
{
    SCROLL_LIST_T *pScrollList;	
	WORD   nTabIndex;
	UINT8  nTab,i;

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( pScrollList->pTab )
	{
		nTabIndex = 0;
		for(nTab=0; nTab < pScrollList->pTab->nTabSize; nTab++)
		{
			nTabIndex += pScrollList->pTab->wTabDataSize[nTab];
			if( nIndex < nTabIndex )
			{
				return nTab;
			}
		}
		return  0; // error		
	}
	else
	{
		return  0;
	}

}

BOOL  EQS_API  EQC_IsShowScrollTitle(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	return pScrollList->fShowTitle;

}

void   EQC_SetScrollTitleStyle(HCONTROL hControl, UINT8 TStyle)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	pScrollList->TitleStyle = TStyle;
}
UINT8  EQC_GetScrollTitleStyle(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	return pScrollList->TitleStyle;
}

BOOL EQC_IsShowScrollListBar(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	return pScrollList->fShowScrollBar;

}

void EQC_SetShowScrollBar(HCONTROL hControl, BOOL fShow)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	pScrollList->fShowScrollBar = fShow;
}


void EQC_DeActiveScrollList(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	if( EQS_IsActive(hControl) == FALSE )
		return ;


    pScrollList->nCursor       = l_nPreviousCursor;
    pScrollList->nLogicalIndex = l_nPreviousLogicalIndex;
	if( pScrollList->fLogicalList == TRUE )
	{
		EQC_SetScrollLogicalIndex(hControl, l_nPreviousLogicalIndex);
	}
	else
	{
		pScrollList->nIndex        = l_nPreviousIndex;
	}
	pScrollList->fActive = FALSE;   
	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_POPUP )
	{		
		l_fDarken = FALSE;
	}

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
		EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST	)
	{	
		if(	EQC_GET_DY(hControl) > EQS_CharHeight((BYTE)' ') )
		{
			EQS_SetCanvasSize(EQS_NA, l_dwIncreasedCanvasSize);
			l_dwIncreasedCanvasSize = 0;
			EQC_SET_DY(hControl, EQS_CharHeight((BYTE)' '));
			EQC_SET_DX(hControl, EQS_CharWidth((BYTE)' ')*pScrollList->nCombo);			
		}
	}

	EQS_EndIO();

}

BOOL EQC_IsScrollListAccelKeySetting(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	return pScrollList->fAccelKey;
}
BOOL EQC_SetScrollListAccelKeySetting(HCONTROL hControl, BOOL fSetting)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	pScrollList->fAccelKey = fSetting;

	return pScrollList->fAccelKey;
}



void EQC_SetScrollFlowMode(HCONTROL hControl, BOOL fSetting)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);

	pScrollList->FlowMode = fSetting;

	if( fSetting )
	{
        if( pScrollList->hTimer != INVALID_TIMER_HANDLE )
		{
			EQS_StopControlTimer( hControl, pScrollList->hTimer );
		}
        pScrollList->hTimer = EQS_StartControlTimer(hControl, \
                                                        EQS_TIMER_1, 800,TRUE);
	}
	else
	{
        if( pScrollList->hTimer != INVALID_TIMER_HANDLE )
		{
			EQS_StopControlTimer( hControl, pScrollList->hTimer );
		}
	}
}
BOOL EQC_GetScrollFlowMode(HCONTROL hControl)
{
    SCROLL_LIST_T *pScrollList;	

    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);
	
	return pScrollList->FlowMode;
}


LOCAL void sDrawScrollText(HCONTROL hControl, UINT8 nOffset, UINT8 nPos, UINT8 nDataIndex, UINT16 nNumber, BOOL fReverse )
{      
    SCROLL_LIST_T *pScrollList;	
	UINT8  nHeight, nWidth, nIndex, nBufPos, nLength;
	COORD  StartX, StartY;
	COLOR  OldTextBackColor, OldTextColor;
	BYTE*  pszBuf;
	BYTE   szBuf[3];


    ASSERT( EQC_GET_TYPE( hControl ) == CT_SCROLL_LIST);
    pScrollList = (SCROLL_LIST_T *)EQC_GET_INSTANCE(hControl);	


	nHeight = EQS_CharHeight((BYTE)' ');
	nWidth  = EQS_CharWidth((BYTE)' ');


	//nBufPos = (nDataIndex-pScrollList->nLogicalIndex) + pScrollList->nCursor;
	if( pScrollList->nPhysicalLine == 0 || pScrollList->nLogicalLine == 0 )
		return ;

	if( nDataIndex >= pScrollList->nPhysicalLine )
	{
		nBufPos = nDataIndex % pScrollList->nPhysicalLine;
	}
	else if( nDataIndex < 0 )
	{
		nBufPos = (nDataIndex + pScrollList->nPhysicalLine ) % pScrollList->nPhysicalLine;
	}
	else
	{
		nBufPos = nDataIndex;
	}

                  
	if(  pScrollList->Header.Style & CS_SCROLL_LIST_CHECK_BOX )
	{
		if( pScrollList->pafCheckBox[nPos] == TRUE )
		{
			EQS_Line(  (COORD)(EQC_GET_X(hControl)+2),
					   (COORD)(EQC_GET_Y(hControl) + nHeight*nPos+2 +nOffset), 
					   (COORD)(EQC_GET_X(hControl) + nWidth-3), 
					   (COORD)(EQC_GET_Y(hControl) + nHeight*(nPos+1)-3)+nOffset);

			EQS_Line( (COORD)(EQC_GET_X(hControl)+2),
					  (COORD)(EQC_GET_Y(hControl) + nHeight*(nPos+1)-3+nOffset),
					  (COORD)(EQC_GET_X(hControl) + nWidth-3),
					  (COORD)(EQC_GET_Y(hControl) + nHeight*nPos+2+nOffset)
					 );
		}
		else
		{
			EQS_FillRectangle( (COORD)(EQC_GET_X(hControl)+2),
							   (COORD)(EQC_GET_Y(hControl) + nHeight*nPos+2+nOffset), 
							   (COORD)(EQC_GET_X(hControl) + nWidth-3), 
							   (COORD)(EQC_GET_Y(hControl) + nHeight*(nPos+1)-3+nOffset),
							   EQS_GetTextBackColor());//EQS_GetBackColor());
		}
		EQS_Rectangle( (COORD)(EQC_GET_X(hControl)+2),
					   (COORD)(EQC_GET_Y(hControl) + nHeight*nPos+2+nOffset), 
					   (COORD)(EQC_GET_X(hControl) + nWidth-3), 
					   (COORD)(EQC_GET_Y(hControl) + nHeight*(nPos+1)-3+nOffset));
	}

	if( pScrollList->Header.Style & CS_SCROLL_LIST_CHECK_BOX )            
	{                   
		StartX = (COORD)(EQC_GET_X(hControl)+nWidth);                    
	}
	else
	{
		StartX = EQC_GET_X(hControl);                                                                         
	}

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_CENTER_ALIGN )
	{
		StartX = StartX + (EQC_GET_DX(hControl) - \
				 EQS_CharWidth((BYTE)' ')*        \
				 STRLEN(pScrollList->ppNode[nBufPos]) )/2;
	}
	/*
	else if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_RIGHT_ALIGN )
	{
		StartX = EQC_GET_DX(hControl)-EQS_CharWidth((BYTE)' ')*STRLEN(pScrollList->ppNode[i]);
	}
	*/
	else ;

	StartY = (COORD)(EQC_GET_Y(hControl) + nPos* EQS_CharHeight((BYTE)' ')+ nOffset);

	if( nPos >= pScrollList->nPhysicalLine || nPos >= pScrollList->nLogicalLine )
	{
#if !NORMAL_REVERSE
		OldTextBackColor = EQS_GetTextBackColor();
#ifdef CONFIG_EQS_COLOR_64K
/*
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
			EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
		{
			EQS_SetTextBackColor(EQS_COMBO_ACTIVE_BACK_COLOR);
		}
		else
		{
			EQS_SetTextBackColor(EQS_GetBackColor());
		}
*/
		if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE) )
		{
			EQS_SetTextBackColor(LIST_NORMAL_BACK_COLOR);
			EQS_SetTextColor(LIST_NORMAL_FORE_COLOR);
		}
		else
		{
			EQS_SetTextBackColor(TEXT_BACK_COLOR);
			EQS_SetTextColor(TEXT_FORE_COLOR);
		}

#else
		EQS_SetTextBackColor(EQS_GetBackColor());
#endif

#endif
		EQS_TextOutStrLenTDM(StartX,StartY,(BYTE*)" ",\
							 pScrollList->nViewChar, TDM_NORMAL);
#if !NORMAL_REVERSE
		EQS_SetTextBackColor(OldTextBackColor);		
#endif		
		return ;
	}
	else if( EQC_GET_STYLE(hControl) &  CS_SCROLL_LIST_TAB )
	{
		if( nPos >= pScrollList->pTab->wTabDataSize[pScrollList->pTab->nCurrentTab] )
		{
#if !NORMAL_REVERSE
			OldTextBackColor = EQS_GetTextBackColor();
#ifdef CONFIG_EQS_COLOR_64K
/*
			if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
				EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
			{
				EQS_SetTextBackColor(EQS_COMBO_ACTIVE_BACK_COLOR);
			}
			else
			{
				EQS_SetTextBackColor(EQS_GetBackColor());
			}
*/
			if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE) )
			{
				EQS_SetTextBackColor(LIST_NORMAL_BACK_COLOR);
				EQS_SetTextColor(LIST_NORMAL_FORE_COLOR);
			}
			else
			{
				EQS_SetTextBackColor(TEXT_BACK_COLOR);
				EQS_SetTextColor(TEXT_FORE_COLOR);
			}
#else

			EQS_SetTextBackColor(EQS_GetBackColor());				
#endif
#endif
			EQS_TextOutStrLenTDM(StartX,StartY,(BYTE*)" ", pScrollList->nViewChar, TDM_NORMAL);
#if !NORMAL_REVERSE
			EQS_SetTextBackColor(OldTextBackColor);
#endif
			return ;
		}
	}
	else ;


	nLength = pScrollList->nViewChar;          

	if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED)
	{   
		            								
		//nIndex = nDataIndex-pScrollList->nIndex+pScrollList->nLogicalIndex;
		nIndex = nNumber;		
		//nIndex  = nPos - pScrollList->nCursor + pScrollList->nLogicalIndex;
		


		
		if( nIndex <= MAX_SPECIAL_DIGIT )
		{
			if( nIndex < MAX_ACCEL_KEY )
			{
				if( pScrollList->pAccelKey[nIndex] == ACCEL_KEY_NONE )						
					sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex);
				else if( pScrollList->pAccelKey[nIndex]  == ACCEL_KEY_STAR )						
					STRCPY(szBuf,(BYTE*)SF_BLACK_STAR);						
				else if( pScrollList->pAccelKey[nIndex]  == ACCEL_KEY_POUND )
					STRCPY(szBuf,(BYTE*)SF_BLACK_POUND);						
				else if( pScrollList->pAccelKey[nIndex]  == ACCEL_KEY_0 )
					STRCPY(szBuf,(BYTE*)SF_BLACK_NUM0);						
				else
					sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+pScrollList->pAccelKey[nIndex]-1);
			}
			else
			{
				if( (nIndex+1) < MAX_LIMIT_DIGIT )
					sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT,SPECIAL_DIGIT_1+nIndex);	
				else
					sprintf((char*)szBuf,"%c%c",SPECIAL_DIGIT_NEXT_BASE,\
												SPECIAL_DIGIT_96+nIndex+1-MAX_LIMIT_DIGIT);	

			}
			/*EQS_TextOutStrLenTDM(StartX,StartY,\
								 szBuf,\
								 2,TDM_BOLD);//,TDM_REVERSE);					*/
#if 0
			if( fReverse )
			{
				EQS_TextOutStrLenTDM(StartX, StartY, szBuf, 2,TDM_REVERSE);
			}
			else
			{
				EQS_TextOutStrLen(StartX, StartY, szBuf, 2);//,TDM_REVERSE);
			}
			//EQS_Line(StartX, StartY+CharHeight(' ')-1, StartX + 2*CharWidth(' ')-1, StartY+CharHeight(' ')-1);

			StartX += EQS_TextWidth(szBuf);
			nLength-=2;
#endif
		}
    
	}

	pszBuf = (BYTE*)((pScrollList->ppNode)[nBufPos]);


	//if (pScrollList->nCursor == nPos)
	if( fReverse )
	{   // REVERSE                        

#if NORMAL_REVERSE 
		OldTextBackColor = EQS_GetTextBackColor();
		EQS_SetTextBackColor(EQS_GetBackColor());

#else
		OldTextBackColor = EQS_GetTextBackColor();
		OldTextColor = EQS_GetTextColor();
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE )
		{
			EQS_SetTextBackColor(SELECT_BACK_COLOR);
			EQS_SetTextColor(SELECT_FORE_COLOR);
		}
		else
		{
			EQS_SetTextBackColor(LIST_CAPTION_TEXT_BACK_COLOR);//LIST_SELECT_BACK_COLOR);
			EQS_SetTextColor(LIST_SELECT_FORE_COLOR);
		}
#endif

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED)
		{
			EQS_TextOutStrLenTDM(StartX, StartY, szBuf, 2,TDM_REVERSE);
			StartX += EQS_TextWidth(szBuf);
			nLength-=2;
		}


		if( STRLEN(pszBuf) > nLength ||\
			(STRLEN(pszBuf) == nLength && (EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR)))
		{
			BYTE pszBuffer[20];
			memset(pszBuffer, 0, 20);

			if( pScrollList->FlowMode )//&& pScrollList->ScrollType == ST_FLOW )
			{				
				if( STRLEN(pszBuf+pScrollList->nPos) >= nLength )
				{
					STRCPY(pszBuffer, pszBuf+pScrollList->nPos);
					if( IsMiddleOfMultiByte(pszBuffer,nLength-2) )
					{
						pszBuffer[nLength-3] = 0;
						pScrollList->nPos += (nLength-3);
					}
					else
					{
						pszBuffer[nLength-2] = 0;
						pScrollList->nPos += (nLength-2);
					}
				}
				else
				{
					STRCPY(pszBuffer, pszBuf+pScrollList->nPos);
					pScrollList->nPos = 0;
				}
			}
			else
			{
				if( IsMiddleOfMultiByte(pszBuf,nLength-2) )
				{
					STRNCPY(pszBuffer, pszBuf, nLength -3);
					STRNCPY(pszBuffer+nLength-3, (BYTE*)SF_ETC_SYMBOL,2);
				}
				else
				{
					STRNCPY(pszBuffer, pszBuf, nLength-2);
					STRNCPY(pszBuffer+ nLength-2, (BYTE*)SF_ETC_SYMBOL,2);
				}

			}
			EQS_TextOutStrLen( StartX, StartY, pszBuffer, nLength);
		}
		else
		{
			EQS_TextOutStrLen(StartX,StartY, pszBuf, nLength);//, TDM_REVERSE);
		}
		//EQS_Line(StartX, StartY+CharHeight(' ')-1, StartX + nLength*CharWidth(' ')-1, StartY+CharHeight(' ')-1);
#if NORMAL_REVERSE 
		EQS_SetTextBackColor(OldTextBackColor);
#endif

#if !NORMAL_REVERSE 
		EQS_SetTextBackColor(OldTextBackColor);
		EQS_SetTextColor(OldTextColor);
#endif
	}
	else
	{   // NORMAL
    
#if !NORMAL_REVERSE 
		OldTextBackColor = EQS_GetTextBackColor();	
#ifdef CONFIG_EQS_COLOR_64K

		if( !(EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_MAXIMIZE) )
		{
			EQS_SetTextBackColor(LIST_NORMAL_BACK_COLOR);
			EQS_SetTextColor(LIST_NORMAL_FORE_COLOR);
		}
		else
		{
			EQS_SetTextBackColor(TEXT_BACK_COLOR);
			EQS_SetTextColor(TEXT_FORE_COLOR);
		}
		
/*
		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO ||\
			EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_COMBO_LIST )
		{
			EQS_SetTextBackColor(EQS_COMBO_ACTIVE_BACK_COLOR);
		}
		else
		{
			EQS_SetTextBackColor(EQS_GetBackColor());
		}
*/
#else				
		EQS_SetTextBackColor(EQS_GetBackColor());
#endif

#endif

		if( EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_ENUMBERATED)
		{
			EQS_TextOutStrLen(StartX, StartY, szBuf, 2);
			StartX += EQS_TextWidth(szBuf);
			nLength-=2;
		}


		if( STRLEN(pszBuf) > nLength ||\
		   (STRLEN(pszBuf) == nLength && EQC_GET_STYLE(hControl) & CS_SCROLL_LIST_SCROLL_BAR))
		{
			BYTE pszBuffer[20];
			memset(pszBuffer, 0, 20);

			if( IsMiddleOfMultiByte(pszBuf,nLength-2) )
			{
				STRNCPY(pszBuffer, pszBuf, nLength -3);
				STRNCPY(pszBuffer+nLength-3, (BYTE*)SF_ETC_SYMBOL,2);
			}
			else
			{
				STRNCPY(pszBuffer, pszBuf, nLength-2);
				STRNCPY(pszBuffer+ nLength-2, (BYTE*)SF_ETC_SYMBOL,2);
			}
			EQS_TextOutStrLenTDM( StartX, StartY, pszBuffer, nLength, TDM_NORMAL);
		}
		else
		{
			EQS_TextOutStrLenTDM(StartX,StartY, pszBuf,
								 nLength, TDM_NORMAL);
		}
#if !NORMAL_REVERSE 
		EQS_SetTextBackColor(OldTextBackColor);
#endif
	}
}
