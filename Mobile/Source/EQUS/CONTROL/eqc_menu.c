/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqc_menu.c
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
** 2000-11-07 girin     Created.
**
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

#define NORMAL_REVERSE           FALSE
#define USE_SKY_LOGO_SYSTEM_BOX  FALSE


#define ANIMATION_DRAW   0xffff0000

#define X_MARGIN   4
#define Y_MARGIN   2
#define XSPACE     2
#define YSPACE     2

#define MAX_MENU_DEPTH      10 // enough ?
#define BUF_SIZE            40 // enough ?

#define ICON_WIDTH          36//32
#define ICON_HEIGHT         28//32

#define X_NUM                3
#define Y_NUM                3

#define MAX_TOP_ITEM_NUM     20

const UINT8  l_MenuPos[9][2] = 
{
	{0, 0}, {40,0}, {83,0},
	{0,39}, {40,39},{83,39},
	{0,77}, {40,77},{83,77}
};

#define RESTORE_IMAGE_WIDTH  47
#define RESTORE_IMAGE_HEIGHT 47
const UINT8  l_RestorePos[9][2] =
{
	{0, 0}, {40,0}, {81,0},
	{0,39}, {40,39},{81,39},
	{0,76}, {40,76},{81,76}
};


//#define DEFAULT_ICON        ((BITMAP*)(&BmpIconBellVib)) // type casting to override 'CONST' qualifier.

#define GET_MENU_MAX_LINE(pMnode) ((pMnode)->nMnodeLine)
#define GET_MENU_HIDDEN_LINE(pMode)((pMode)->nHiddenLine)
#define GET_MENU_TITLE(pMnode) (EQS_GET_TOKEN_EX((pMnode)->TitleTokenID))
#define GET_MENU_STR(pMnode, nIndex) (EQS_GET_TOKEN_EX(((*((pMnode)->pMbodyArray))[nIndex]).TokenID))

#define GET_MENU_ICON_ANI(pMnode, nIndex) ((ANIMATION_INFO_T  *)(((*((pMnode)->pMbodyArray))[nIndex]).pAnimatedIcon)) // type casting to override 'CONST' qualifier.
#define GET_ANI_FRAME(pAni)               (pAni->AnimationHeader.nFrame)
#define GET_ANI_TIME(pAni, nIndex)        (pAni->AnimationHeader.awDelayTime[nIndex])
#define GET_MENU_ICON_FRAME(pMnode, nIndex) ((*((pMnode)->pMbodyArray))[nIndex]).pAnimatedIcon->AnimationHeader.nFrame)
#define GET_MENU_ICON_TIME(pMode, nIndex, nIter) ((*((pMnode)->pMbodyArray))[nIndex]).pAnimatedIcon->AnimationHeader.awDelayTime[nIter])

#define IS_MENU_LEAF_APPLET(pMnode, nIndex) (((*((pMnode)->pMbodyArray))[nIndex]).fIsLeaf)
#define GET_MENU_PARENT(pMnode) ((pMnode)->pParentMnode)
#define GET_MENU_LEAF(pMnode, nIndex) (ROM MNODE_T*) (((*((pMnode)->pMbodyArray))[nIndex]).pNext)
#define GET_MENU_ACCEL_KEY(pMnode, nIndex) (((*((pMnode)->pMbodyArray))[nIndex]).AccelKey)
#define GET_MENU_ITEM_TEXT(pMnode, nIndex) (((*((pMnode)->pMbodyArray))[nIndex]).pszTitle)

#define GET_BITMAP_DATA_POS(pAniInfo, nCurrentFrame) \
                            pAniInfo->pbData \
                            + EQC_GET_FRAME_SIZE(\
                                    pAniInfo->BitmapHeader)\
                              * nCurrentFrame


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/



enum  tagMENU_SCROLL_TYPE_E
{
	MST_DRAW,
	MST_NORMAL,
	MST_TOP_DOWN,
	MST_SCROLL,
	MST_REDRAW
};
typedef enum tagMENU_SCROLL_TYPE_E  MENU_SCROLL_TYPE_E;

typedef struct tagLATTICE_T       LATTICE_T;
struct tagLATTICE_T
{
    UINT8               nViewNum;
    UINT8               nViewNumX;
    UINT8               nViewNumY;
    COORD               xSpace;
    COORD               ySpace;
};

typedef struct tagMENU_LIST_T       MENU_LIST_T;
struct tagMENU_LIST_T
{
    CONTROL_HEADER_T    Header;
	MNODE_T*            pMNodeOrigin;
	BYTE*               pszItemText[MAX_TOP_ITEM_NUM];
    ROM MNODE_T*        pMnode;
	BITMAP              *pBackImage;
    UINT8               nIndexStack[MAX_MENU_DEPTH];
    UINT8               nCursorStack[MAX_MENU_DEPTH];
    UINT8               nCurrentPos; // current position on Traverse Stack.
    UINT8               nCurrentFrame;
    SCROLL_BAR          ScrollBar;
    UINT8               nChar;
    UINT8               nLine;
    LATTICE_T           Lattice;
    BOOL                fIcon;	
	COLOR               BackColor;
	INT8                nPreviousIndex;
	MENU_SCROLL_TYPE_E  ScrollType;
};


/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/


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
LOCAL void* EQS_API sAllocInstance(ROM MNODE_T* pMnode, UINT8 nStartIndx, 
                                   UINT8 nChar, UINT8 nLine, BOOL fIcon);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sTextMenuHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sIconMenuHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawTextMenuInstance(HCONTROL hControl);
LOCAL void  EQS_API sDrawIconMenuInstance(HCONTROL hControl, BOOL fDrawAni);
LOCAL ACCEL_KEY_T   sKey2AccelKey(EQS_EVENT_ID EventID);
LOCAL LATTICE_T     sGetLattice(HCONTROL hMenuControl);
LOCAL RECT          sGetIconViewRect(HCONTROL hMenuControl);

LOCAL void sDrawMenuText(HCONTROL hControl, UINT8 nOffset, INT8 nPos, INT8 nIndex, BOOL fReverse );

void sDrawBorder(COORD LeftX, COORD TopY, COORD RightX, COORD BottomY);



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
LOCAL ACCEL_KEY_T sKey2AccelKey(EQS_EVENT_ID EventID)
{
    switch (EventID)
    {
    case EQE_KEY_0:
        return ACCEL_KEY_0;
    case EQE_KEY_1:
        return ACCEL_KEY_1;
    case EQE_KEY_2:
        return ACCEL_KEY_2;
    case EQE_KEY_3:
        return ACCEL_KEY_3;
    case EQE_KEY_4:
        return ACCEL_KEY_4;
    case EQE_KEY_5:
        return ACCEL_KEY_5;
    case EQE_KEY_6:
        return ACCEL_KEY_6;
    case EQE_KEY_7:
        return ACCEL_KEY_7;
    case EQE_KEY_8:
        return ACCEL_KEY_8;
    case EQE_KEY_9:
        return ACCEL_KEY_9;
    case EQE_KEY_STAR:
        return ACCEL_KEY_STAR;
    case EQE_KEY_POUND:
        return ACCEL_KEY_POUND;

    default:
        return ACCEL_KEY_0;
    }

}

LOCAL LATTICE_T     sGetLattice(HCONTROL hControl)
{
    RECT IconViewRect;
    INT   nWidth, nHeight;
    MENU_LIST_T *pMenuList;

    pMenuList = (MENU_LIST_T *)EQC_GET_INSTANCE( hControl );

    IconViewRect = sGetIconViewRect(hControl);
    //nWidth  = (IconViewRect.xRight - IconViewRect.xLeft + 1)-X_MARGIN*2 - R_SCROLLBAR_SIZE;
    //nHeight = (IconViewRect.yBottom - IconViewRect.yTop + 1)-Y_MARGIN*2;

    nWidth  = (IconViewRect.xRight - IconViewRect.xLeft + 1);
    nHeight = (IconViewRect.yBottom - IconViewRect.yTop + 1);

    pMenuList->Lattice.nViewNumX =  3; //nWidth  / ICON_WIDTH; 
    pMenuList->Lattice.nViewNumY =  3;//nHeight / ICON_HEIGHT;
    

    pMenuList->Lattice.nViewNum = 9;//pMenuList->Lattice.nViewNumX * pMenuList->Lattice.nViewNumY;

    if (pMenuList->Lattice.nViewNum > 0)
    {
/*
		if( pMenuList->Lattice.nViewNumX > 1 )
			pMenuList->Lattice.xSpace = (nWidth  - ICON_WIDTH*pMenuList->Lattice.nViewNumX)  / (pMenuList->Lattice.nViewNumX - 1);
		else
			pMenuList->Lattice.xSpace = 0;

		if( pMenuList->Lattice.nViewNumY > 1 )
			pMenuList->Lattice.ySpace = (nHeight - ICON_HEIGHT*pMenuList->Lattice.nViewNumY) / (pMenuList->Lattice.nViewNumY - 1);
		else
			pMenuList->Lattice.ySpace = 0;
*/
		pMenuList->Lattice.xSpace = 2;
		pMenuList->Lattice.ySpace = 2;

    }
    else
    {
        pMenuList->Lattice.xSpace = pMenuList->Lattice.ySpace = 0;
    }

    return pMenuList->Lattice;
}

LOCAL RECT      sGetIconViewRect(HCONTROL hMenuControl)
{
    RECT Rect;
    UINT8 nTitleHeight;

    COORD Left, Top, Right, Bottom;


    if (EQC_GET_STYLE(hMenuControl) & CS_MENU_LIST_SHOW_TITLE)
    {
        nTitleHeight = EQS_CharHeight((BYTE)' ')+1;
    }
    else
    {
        nTitleHeight = 0;
    }

    Left = 0;
    Top  = 0 + nTitleHeight;

    Right = 0 + DevGetMaxX();

    if (EQC_GET_STYLE(hMenuControl) & CS_MENU_LIST_CAPTION)
        Bottom = 0 + DevGetMaxY() - EQS_CharHeight((BYTE)' ');
    else
        Bottom = 0 + DevGetMaxY() ;

#if 0
    Left = EQC_GET_X(hMenuControl);
    Top = EQC_GET_Y(hMenuControl) + nTitleHeight;

    Right = EQC_GET_X(hMenuControl) + EQC_GET_DX(hMenuControl) - 1;

    if (EQC_GET_STYLE(hMenuControl) & CS_MENU_LIST_CAPTION)
        Bottom = EQC_GET_Y(hMenuControl) + EQC_GET_DY(hMenuControl) - 1 - EQS_CharHeight((BYTE)' ');
    else
        Bottom = EQC_GET_Y(hMenuControl) + EQC_GET_DY(hMenuControl) - 1;

#endif 


    SetRect(&Rect, Left, Top, Right, Bottom);

    return Rect;
}


LOCAL void* EQS_API sAllocInstance(ROM MNODE_T* pMnode, UINT8 nStartIndx, UINT8 nChar, UINT8 nLine, BOOL fIcon)
{
    MENU_LIST_T *pMenuList;
	int i;

	pMenuList = (MENU_LIST_T *)EQS_Malloc(sizeof(MENU_LIST_T));
	pMenuList->pMNodeOrigin = (MNODE_T*)pMnode;
	pMenuList->pMnode = pMnode;
    pMenuList->nCurrentPos = 0;
    pMenuList->nIndexStack[0] = nStartIndx;
    pMenuList->nCursorStack[0] = 0;
    pMenuList->nChar = nChar;
    pMenuList->nLine = nLine;
    pMenuList->nCurrentFrame = 0;
    pMenuList->fIcon = fIcon;	
	pMenuList->pBackImage = NULL;
	pMenuList->BackColor  = EQS_GetBackColor();
	pMenuList->nPreviousIndex = -1; // INIT VALUE - draw all 
	pMenuList->ScrollType  = MST_DRAW;

	for(i=0; i<MAX_TOP_ITEM_NUM; i++)
	{
		pMenuList->pszItemText[i] = NULL;
	}

	return (void *)pMenuList;
}

LOCAL void EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_ICON)
        sIconMenuHandleEvent(hControl, pEvent);
    else
        sTextMenuHandleEvent(hControl, pEvent);
}


LOCAL void EQS_API sTextMenuHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	MENU_LIST_T *pMenuList;
    UINT8 nCurrentPos, nViewLine;
    ACCEL_KEY_T AccelKey;
    UINT8 i, nSize, nHiddenLine;
    ANIMATION_INFO_T  *pAni;

	ASSERT( EQC_GET_TYPE( hControl ) == CT_MENU_LIST);

	pMenuList = (MENU_LIST_T *)EQC_GET_INSTANCE( hControl );
    nCurrentPos = pMenuList->nCurrentPos;

    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_SHOW_TITLE)            
        nViewLine = pMenuList->nLine-1;
    else
        nViewLine = pMenuList->nLine;

    nSize = GET_MENU_MAX_LINE(pMenuList->pMnode);
	nHiddenLine = GET_MENU_HIDDEN_LINE(pMenuList->pMnode);

	switch (pEvent->EventID)
	{

    case EQE_APP_IO_MODE:		
		pMenuList->nPreviousIndex = -1;
		pMenuList->ScrollType = MST_DRAW;

		pMenuList->nCurrentFrame = 0;
        break;
	case EQE_APP_DRAW:
		if( pEvent->wParam == EQS_NA )
		{
			pMenuList->ScrollType = MST_DRAW;
		}
		sDrawInstance(hControl);
		break;

	case EQE_KEY_UP:

		pMenuList->nPreviousIndex = pMenuList->nIndexStack[nCurrentPos];
		if (pMenuList->nIndexStack[nCurrentPos] == 0)
        {
			pMenuList->nIndexStack[nCurrentPos] =  nSize - 1; // wrap around
            pMenuList->nCursorStack[nCurrentPos] = nViewLine - 1;
            if( nSize > nViewLine )
			{
				pMenuList->ScrollType = MST_REDRAW;
			}
			else
			{
				pMenuList->nCursorStack[nCurrentPos] = nSize - 1;
				pMenuList->ScrollType = MST_TOP_DOWN;
			}
        }
		else
        {
			(pMenuList->nIndexStack[nCurrentPos])--;
			if (pMenuList->nCursorStack[nCurrentPos] > 0)
			{
				(pMenuList->nCursorStack[nCurrentPos])--;	
				pMenuList->ScrollType = MST_NORMAL;
			}
			else
			{
				pMenuList->ScrollType = MST_SCROLL;
			}
			
        }

        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA);
        
		break;

	case EQE_KEY_DOWN:

		pMenuList->nPreviousIndex = pMenuList->nIndexStack[nCurrentPos];


		if (pMenuList->nIndexStack[nCurrentPos] >= nSize- 1)
        {
			pMenuList->nIndexStack[nCurrentPos] = 0; // wrap around
            pMenuList->nCursorStack[nCurrentPos] = 0;

            if( nSize > nViewLine )
			{
				pMenuList->ScrollType = MST_REDRAW;
			}
			else
			{
				pMenuList->ScrollType = MST_TOP_DOWN;
			}
			
        }
		else
        {
			if (pMenuList->nCursorStack[nCurrentPos] < nViewLine - 1 )
			{
				(pMenuList->nCursorStack[nCurrentPos])++;
				pMenuList->ScrollType = MST_NORMAL;
			}
			else
			{
				pMenuList->ScrollType = MST_SCROLL;
			}

			if (pMenuList->nCursorStack[nCurrentPos] > nSize)
			{
				pMenuList->nCursorStack[nCurrentPos] = nSize - 1;
			}
			(pMenuList->nIndexStack[nCurrentPos])++;			
        }

        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA);

		break;
	case EQE_KEY_LEFT:
		if( pMenuList->nCurrentPos == 1 )
		{

			pMenuList->ScrollType = MST_DRAW;

			nSize = GET_MENU_MAX_LINE(GET_MENU_PARENT(pMenuList->pMnode));
		    
			if( pMenuList->nIndexStack[pMenuList->nCurrentPos-1] > 0 )
			{
				pMenuList->nIndexStack[pMenuList->nCurrentPos-1] = pMenuList->nIndexStack[pMenuList->nCurrentPos-1] -1;
         			
			}
			else
			{
				pMenuList->nIndexStack[pMenuList->nCurrentPos-1] = nSize -1;
			}

			if( pMenuList->nIndexStack[pMenuList->nCurrentPos-1]  < nViewLine )
			{
				pMenuList->nCursorStack[pMenuList->nCurrentPos-1] = pMenuList->nIndexStack[pMenuList->nCurrentPos-1];
			}
			else
			{
				pMenuList->nCursorStack[pMenuList->nCurrentPos-1] = nViewLine - 1;
			}
			pMenuList->nCurrentPos--;
			pMenuList->pMnode = GET_MENU_PARENT(pMenuList->pMnode);
			nCurrentPos = pMenuList->nCurrentPos;

			if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]))
			{
				EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
					(WORD)hControl, 
					(DWORD) GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]));  
				EQS_EndIO();
			}
			else            
			{
				if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
				{ // go to child menu list
					(pMenuList->nCurrentPos)++;
					pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]);
					pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
					pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;
					if (pMenuList->fIcon)
					{
						pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] ); 
						if( pAni != NULL )
						{
							EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT );               
							EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
							//EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
							EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
						}                
						else            
							EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
					}
					else
						EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
				}
			}
		}
		break;
	case EQE_KEY_RIGHT:
		if( pMenuList->nCurrentPos == 1 )
		{

			pMenuList->ScrollType = MST_DRAW;

			nSize = GET_MENU_MAX_LINE(GET_MENU_PARENT(pMenuList->pMnode));
		
			if( pMenuList->nIndexStack[pMenuList->nCurrentPos-1] < (nSize-1) )
			{
				pMenuList->nIndexStack[pMenuList->nCurrentPos-1] = pMenuList->nIndexStack[pMenuList->nCurrentPos-1] +1;         			
			}
			else
			{
				pMenuList->nIndexStack[pMenuList->nCurrentPos-1] = 0;
			}

			if( pMenuList->nIndexStack[pMenuList->nCurrentPos-1]  < nViewLine )
			{
				pMenuList->nCursorStack[pMenuList->nCurrentPos-1] = pMenuList->nIndexStack[pMenuList->nCurrentPos-1];
			}
			else
			{
				pMenuList->nCursorStack[pMenuList->nCurrentPos-1] = nViewLine - 1;
			}
			pMenuList->nCurrentPos--;
			pMenuList->pMnode = GET_MENU_PARENT(pMenuList->pMnode);
			nCurrentPos = pMenuList->nCurrentPos;

			if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]))
			{
				EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
					(WORD)hControl, 
					(DWORD) GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]));  
				EQS_EndIO();
			}
			else            
			{
				if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
				{ // go to child menu list
					(pMenuList->nCurrentPos)++;
					pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]);
					pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
					pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;
					if (pMenuList->fIcon)
					{
						pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] ); 
						if( pAni != NULL )
						{
							EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT );               
							EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
							//EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
							EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
						}                
						else            
							EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
					}
					else
						EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
				}
			}
		}
		break;

    case EQE_KEY_CENTER:
		pMenuList->nPreviousIndex = -1;

        if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]))
        {            
            EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
                (WORD)hControl, 
                (DWORD) GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]));  
            EQS_EndIO();
        }
        else            
        {
            if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
            { // go to child menu list
				pMenuList->ScrollType = MST_DRAW;

                (pMenuList->nCurrentPos)++;
                pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]);
                pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
                pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;
                if (pMenuList->fIcon)
                {
                    pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] ); 
                    if( pAni != NULL )
                    {
                        EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT );               
                        EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
                        //EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
						EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
                    }                
                    else            
                        EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
                }
                else
                    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);

            }
        }

        break;

    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
    case EQE_KEY_0:
    case EQE_KEY_STAR:
    case EQE_KEY_POUND:

        if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_ENUMERATED)
        {
            AccelKey = sKey2AccelKey(pEvent->EventID);

            for (i = 0; i < (nSize+nHiddenLine); i++)
            {
                if (AccelKey == GET_MENU_ACCEL_KEY(pMenuList->pMnode, i))
                {
					pMenuList->ScrollType = MST_DRAW;
					pMenuList->nPreviousIndex = -1;
					if( i < nSize )
					{
						pMenuList->nIndexStack[nCurrentPos] = i;
						if( i < nViewLine )
							pMenuList->nCursorStack[nCurrentPos] = i;
						else
							pMenuList->nCursorStack[nCurrentPos] = nViewLine -1;
					}
                    ///
                    if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, i ))//pMenuList->nIndexStack[nCurrentPos]))
                    {
                        EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
                            (WORD)hControl, 
                            (DWORD)(GET_MENU_LEAF(pMenuList->pMnode, i)));  
                        EQS_EndIO();
                    }
                    else            
                    {
                        if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
                        { // go to child menu list
                            (pMenuList->nCurrentPos)++;
                            pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, i);
                            pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
                            pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;

                            pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] ); 
                            if (pMenuList->fIcon)
                            {
                                if( pAni != NULL )
                                {
                                    EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT );               
                                    EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
                                    //EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
									EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
                                }                               
                                else            
                                    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA); 
                            }
                            else
                                EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA); 

                        }
                    }
                    break;
                    ///
                }
            }
        }
        break;
          
    case EQE_KEY_CLEAR:
		pMenuList->ScrollType = MST_DRAW;
		pMenuList->nPreviousIndex = -1;

		pMenuList->nCurrentFrame = 0;
        if (pMenuList->nCurrentPos > 0)
        {
            pMenuList->pMnode = GET_MENU_PARENT(pMenuList->pMnode);
            (pMenuList->nCurrentPos)--;
			
            if (pMenuList->fIcon)
            {
                pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] ); 
                if( pAni != NULL )
                {
					pMenuList->nCursorStack[pMenuList->nCurrentPos] =  pMenuList->nIndexStack[pMenuList->nCurrentPos] % pMenuList->Lattice.nViewNum;
					if( pMenuList->nCursorStack[pMenuList->nCurrentPos]  < 3  &&  pMenuList->nIndexStack[pMenuList->nCurrentPos] > 2)
					{
						 pMenuList->nCursorStack[pMenuList->nCurrentPos] += 3;
					}
                    EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT );               
                    EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
                    //EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
					EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
                } 
                else        
				{
                    EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
				}
            }
            else
			{
                EQS_SendEvent(EQE_APP_DRAW, (WORD)hControl, EQS_NA);
			}

        }
        else
        {
            EQS_SendEvent(EQE_IO_MENU_LIST_CANCEL, 
                (WORD)hControl, 
                EQS_NA); 
            EQS_EndIO();        
        }

        break;

    case EQE_KEY_END:

        EQS_EndIO();
        EQS_SendEvent(pEvent->EventID, pEvent->wParam, pEvent->dwParam);
        break;

    case EQE_IO_DELETE:
        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_MENU_LIST_CANCEL, (WORD)hControl, (DWORD)NULL);
        // Free instance
        sFreeInstance(hControl);
        break;
	}

}


LOCAL void EQS_API sIconMenuHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	MENU_LIST_T *pMenuList;
    UINT8 nCurrentPos, nSize, nHiddenLine;
    ACCEL_KEY_T AccelKey;
    UINT8 i;
    ANIMATION_INFO_T  *pAni;


	ASSERT( EQC_GET_TYPE( hControl ) == CT_MENU_LIST);

	pMenuList = (MENU_LIST_T *)EQC_GET_INSTANCE( hControl );
    nCurrentPos = pMenuList->nCurrentPos;
    nSize = GET_MENU_MAX_LINE(pMenuList->pMnode);
	nHiddenLine = GET_MENU_HIDDEN_LINE(pMenuList->pMnode);

	switch (pEvent->EventID)
	{
    case EQE_APP_IO_MODE:

        pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] );  
		pMenuList->nPreviousIndex = -1; // draw all 
        if( pAni == NULL )
        {                                                
            EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT );
        }
        else           
		{
			pMenuList->nCurrentFrame = 0;
            EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, 0), FALSE );
		}
        break;
	case EQE_APP_DRAW:	
		if( pEvent->wParam == EQS_NA )
		{
			pMenuList->ScrollType = MST_DRAW;
		}

		if( GET_MENU_ICON_ANI(pMenuList->pMnode, 0) == NULL )
		{
			sDrawTextMenuInstance(hControl);
		}
		else
		{
			if( pEvent->dwParam == ANIMATION_DRAW )
			{
				sDrawIconMenuInstance(hControl, TRUE);
			}
			else
			{
				sDrawIconMenuInstance(hControl, FALSE);
			}
		}
		break;

    case  EQE_TM_TIMER1:
        
        pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, pMenuList->nIndexStack[pMenuList->nCurrentPos] );

        if( pMenuList->nCurrentFrame < GET_ANI_FRAME(pAni) - 1 )
        {
            pMenuList->nCurrentFrame++;
        }
        else
            pMenuList->nCurrentFrame = 0;
        EQS_StartControlTimer(hControl, EQS_TIMER_1, GET_ANI_TIME(pAni, pMenuList->nCurrentFrame), FALSE );

        EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, ANIMATION_DRAW);

        break;
	case EQE_KEY_UP:
		if (pMenuList->nIndexStack[nCurrentPos] > 0)
        {      
			pMenuList->nCurrentFrame = 0;
            if (pMenuList->nCursorStack[nCurrentPos] < pMenuList->Lattice.nViewNumX )
            {
                if (pMenuList->nIndexStack[nCurrentPos] >= pMenuList->Lattice.nViewNumX )
                {
			        pMenuList->nIndexStack[nCurrentPos] -= pMenuList->Lattice.nViewNumX;
                }
                else
                {
                    pMenuList->nIndexStack[nCurrentPos]  = 0;
                    pMenuList->nCursorStack[nCurrentPos] = 0;
                }                   
            }
            else
            {
				
                pMenuList->nIndexStack[nCurrentPos] -= pMenuList->Lattice.nViewNumX;
                pMenuList->nCursorStack[nCurrentPos] -= pMenuList->Lattice.nViewNumX;
            }            
        }
		else //if( pMenuList->nIndexStack[nCurrentPos] == 0)
		{
			pMenuList->nCurrentFrame = 0;
            pMenuList->nIndexStack[nCurrentPos]  = nSize -1;
            pMenuList->nCursorStack[nCurrentPos] = nSize -1;
		}
		//EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
		EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
		        
		break;

	case EQE_KEY_DOWN:

		if( pMenuList->nIndexStack[nCurrentPos]  == nSize - 1 )
		{
			pMenuList->nCurrentFrame = 0;
			pMenuList->nIndexStack[nCurrentPos]  = pMenuList->nCursorStack[nCurrentPos]  = 0;
		}       
        else if( pMenuList->nCursorStack[nCurrentPos] + pMenuList->Lattice.nViewNumX > pMenuList->Lattice.nViewNum -1 )
        {
			pMenuList->nCurrentFrame = 0;
		    if (pMenuList->nIndexStack[nCurrentPos] + pMenuList->Lattice.nViewNumX < nSize-1)
            {
			    pMenuList->nIndexStack[nCurrentPos] += pMenuList->Lattice.nViewNumX;
            }
            else
            {
				if( pMenuList->nIndexStack[nCurrentPos] == nSize -1 )
				{
					return ;
				}
                 pMenuList->nIndexStack[nCurrentPos] = nSize -1;
                 pMenuList->nCursorStack[nCurrentPos] = pMenuList->Lattice.nViewNum -pMenuList->Lattice.nViewNumX +\
                                                        pMenuList->nIndexStack[nCurrentPos] % pMenuList->Lattice.nViewNumX;
            }            
        }
        else
        {
			pMenuList->nCurrentFrame = 0;
		    if (pMenuList->nIndexStack[nCurrentPos] + pMenuList->Lattice.nViewNumX < nSize-1)
            {
                pMenuList->nIndexStack[nCurrentPos]  += pMenuList->Lattice.nViewNumX;
                pMenuList->nCursorStack[nCurrentPos] += pMenuList->Lattice.nViewNumX;

            }
            else
            {
                 pMenuList->nIndexStack[nCurrentPos] = nSize -1;
                 pMenuList->nCursorStack[nCurrentPos] = pMenuList->Lattice.nViewNum -pMenuList->Lattice.nViewNumX +\
                                                        pMenuList->nIndexStack[nCurrentPos] % pMenuList->Lattice.nViewNumX;
            }   
        }
      
        //EQS_SendEvent( EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
		EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);

		break;

	case EQE_KEY_LEFT:
		if (pMenuList->nIndexStack[nCurrentPos] > 0)
        {
			pMenuList->nCurrentFrame = 0;
			pMenuList->nIndexStack[nCurrentPos]--;            
           
		    if (pMenuList->nCursorStack[nCurrentPos] > 0)
            {
			    (pMenuList->nCursorStack[nCurrentPos])--;
            }
            else
            {
			    pMenuList->nCursorStack[nCurrentPos] = (pMenuList->Lattice.nViewNumX-1);
            }			
        } 
		else 
		{
			pMenuList->nCurrentFrame = 0;
            pMenuList->nIndexStack[nCurrentPos]  = nSize -1;
            pMenuList->nCursorStack[nCurrentPos] = nSize -1;
		}
		//EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
		EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
		break;

	case EQE_KEY_RIGHT:   
		if (pMenuList->nIndexStack[nCurrentPos] < nSize -1)
        {
			pMenuList->nCurrentFrame = 0;
			pMenuList->nIndexStack[nCurrentPos]++;
		    if (pMenuList->nCursorStack[nCurrentPos] < (pMenuList->Lattice.nViewNum -1))
            {
			    (pMenuList->nCursorStack[nCurrentPos])++;
            }
            else
            {
                (pMenuList->nCursorStack[nCurrentPos])-=(pMenuList->Lattice.nViewNumX-1);
            }
        }
		else 
		{
			pMenuList->nCurrentFrame = 0;
            pMenuList->nIndexStack[nCurrentPos]  = 0;
            pMenuList->nCursorStack[nCurrentPos] = 0;
		}
		//EQS_SendEvent( EQE_APP_DRAW,(WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
		EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
              
		break;
    case EQE_KEY_CENTER:
		pMenuList->nPreviousIndex = -1;
        if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]))
        {
            
            EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
                (WORD)hControl, 
                (DWORD) GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]));  
            EQS_EndIO();
        }
        else            
        {
            if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
            { // go to child menu list
				pMenuList->ScrollType = MST_DRAW;
                (pMenuList->nCurrentPos)++;
                pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, pMenuList->nIndexStack[nCurrentPos]);
                pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
                pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;

                if( GET_MENU_ICON_ANI(pMenuList->pMnode, 0) == NULL )
                {
                    EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT );
                }            
                //EQS_SendEvent( EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
				EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
            }
        }

        break;

    case EQE_KEY_1:
    case EQE_KEY_2:
    case EQE_KEY_3:
    case EQE_KEY_4:
    case EQE_KEY_5:
    case EQE_KEY_6:
    case EQE_KEY_7:
    case EQE_KEY_8:
    case EQE_KEY_9:
    case EQE_KEY_0:
    case EQE_KEY_STAR:
    case EQE_KEY_POUND:

        if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_ENUMERATED)
        {

            AccelKey = sKey2AccelKey(pEvent->EventID);

            for (i = 0; i < (nSize+nHiddenLine); i++)
            {
                if (AccelKey == GET_MENU_ACCEL_KEY(pMenuList->pMnode, i))
                {
					pMenuList->ScrollType = MST_DRAW;
					pMenuList->nPreviousIndex = -1;
					if( i < nSize )
					{
						pMenuList->nIndexStack[nCurrentPos] = i;
						pMenuList->nCursorStack[nCurrentPos] =  i % pMenuList->Lattice.nViewNumX;					
						if( pMenuList->nIndexStack[nCurrentPos] + pMenuList->Lattice.nViewNumX > nSize -1 )
						{
							pMenuList->nCursorStack[nCurrentPos] += pMenuList->Lattice.nViewNumX;
						}
					}

                    ///
                    if(IS_MENU_LEAF_APPLET(pMenuList->pMnode, i))
                    {
                        EQS_SendEvent(EQE_IO_MENU_LIST_SELECT, 
                            (WORD)hControl, 
                            (DWORD)(GET_MENU_LEAF(pMenuList->pMnode, i)));  
                        EQS_EndIO();
                    }
                    else            
                    {
                        if (pMenuList->nCurrentPos < MAX_MENU_DEPTH - 1)
                        { // go to child menu list
                            (pMenuList->nCurrentPos)++;
                            pMenuList->pMnode = GET_MENU_LEAF(pMenuList->pMnode, i);
                            pMenuList->nIndexStack[pMenuList->nCurrentPos] = 0;
                            pMenuList->nCursorStack[pMenuList->nCurrentPos] = 0;

                            if( GET_MENU_ICON_ANI(pMenuList->pMnode, 0) == NULL )
                            {
                                EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT );
                            }                
                            //EQS_SendEvent( EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
							EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
                        }
                    }
                    break;
                }
            }
        }
        break;
          
    case EQE_KEY_CLEAR:
		pMenuList->ScrollType = MST_DRAW;
		pMenuList->nPreviousIndex = -1;

		pMenuList->nCurrentFrame = 0;
        if (pMenuList->nCurrentPos > 0)
        {
			UINT8 nViewLine;

            pMenuList->pMnode = GET_MENU_PARENT(pMenuList->pMnode);
            (pMenuList->nCurrentPos)--;
			pMenuList->nCurrentFrame = 0;
            if( GET_MENU_ICON_ANI(pMenuList->pMnode, 0) == NULL )
            {
                EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT );
            }  

			if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_SHOW_TITLE)
			{
				nViewLine = pMenuList->nLine -1;  
			}
			else
			{
				nViewLine = pMenuList->nLine;
			}
			nSize =  GET_MENU_MAX_LINE(pMenuList->pMnode);  
			if (nViewLine >= nSize)
			{
				pMenuList->nCursorStack[nCurrentPos] = pMenuList->nIndexStack[nCurrentPos];
			}
			else
			{
				if( nViewLine > pMenuList->nIndexStack[nCurrentPos] )
				{
					pMenuList->nCursorStack[nCurrentPos] =  0;
				}
				else
				{
					pMenuList->nCursorStack[nCurrentPos] =  nViewLine-1;
				}
			}
            //EQS_SendEvent(EQE_APP_DRAW, (WORD)EQS_DRAW_LEVEL_ALL, EQS_NA);
			EQS_SendEvent( EQE_APP_DRAW,(WORD)hControl, EQS_NA);
        }
        else
        {
            EQS_SendEvent(EQE_IO_MENU_LIST_CANCEL, 
                (WORD)hControl, 
                EQS_NA); 
            EQS_EndIO();        
        }

        break;

    case EQE_KEY_END:

        EQS_EndIO();
        EQS_SendEvent(pEvent->EventID, pEvent->wParam, pEvent->dwParam);
        break;

    case EQE_IO_DELETE:
        // Send ending event you want..
		if( pEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_MENU_LIST_CANCEL, (WORD)EQS_DRAW_LEVEL_ALL, (DWORD)NULL);

        // Free instance
        sFreeInstance(hControl);
        break;

	}

}


LOCAL void EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	int i, j;

	MENU_LIST_T *pMenuList;

	pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);

	for( i=0; i< GET_MENU_MAX_LINE(pMenuList->pMNodeOrigin); i++)
	{
		for(j= 0; j< MAX_TOP_ITEM_NUM; j++ )
		{
			if( pMenuList->pszItemText[j] )
			{
				EQS_Free( (VOID**)(&pMenuList->pszItemText[j]) );
			}
		}
	}

	pVoid = EQC_GET_INSTANCE(hControl);

    EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);
}

LOCAL void EQS_API sDrawInstance(HCONTROL hControl)
{
	MENU_LIST_T *pMenuList;

	pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
	
	if( GET_MENU_ICON_ANI(pMenuList->pMnode, 0) == NULL )
	{
		sDrawTextMenuInstance(hControl);	    
	}
	else
	{
		if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_ICON)
		{
			sDrawIconMenuInstance(hControl,FALSE);
		}
		else
		{
			sDrawTextMenuInstance(hControl);
		}
	}
}

#define sTITLE_POS_OFFSET       1

LOCAL void EQS_API sDrawTextMenuInstance(HCONTROL hControl)
{
    SCROLL_BAR_INFO SBarInfo;
	UINT8 i, nSize, nViewLine;
	UINT8 uDrawIndx, nHeight, nWidth, nTitleHeight;
    INT16 nTitlePosX, nTitlePosY;
	MENU_LIST_T *pMenuList;
    COLOR  TextColor;
    COORD  StartX, StartY;
    DRAW_MODE DrawMode;
    UINT8 nCurrentPos;
    BYTE StrBuf[BUF_SIZE];
    COLOR OldColor, OldTextColor;
	EQS_EVENT DrawEvent;


	DrawEvent.EventID = EQE_APP_DRAW;


/*
EQS_TextOutStrLenTDM을 사용하면 현재 text draw mode를 변경시키지 않으며 
지정된 length까지 공백으로 채워줍니다.

!! 현재 메뉴 개수가 5개, 다음 메뉴가 3개면 밑에 2개가 그대로 나타나는 문젠
EQS_TextOutStrLen(x, y, " ", pMenuList->nChar)로 지워주는게 가장 빠릅니다...   
*/
    EQS_GetDC(TRUE);

/*    EQS_FillRectangle( EQC_GET_X(hControl),
                   EQC_GET_Y(hControl), 
                   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
                   EQC_GET_Y(hControl) + EQC_GET_DX(hControl)-1,
                   R_LIGHTGRAY);*/

    pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
    nCurrentPos = pMenuList->nCurrentPos;
    
    nHeight = EQS_CharHeight((BYTE)' ');
    nWidth  = EQS_CharWidth((BYTE)' ');



    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_SHOW_TITLE)
    {
		WINDOW_TSYMBOL_MODE  WindowSymbolMode;


        nViewLine = pMenuList->nLine -1;  
        
#ifdef CONFIG_EQS_COLOR_64K

		nTitleHeight = EQS_SWINDOW_HEADER_HEIGHT;
#else
        nTitleHeight = EQC_GET_DY(hControl) - nViewLine*EQS_CharHeight((BYTE)' ');
#endif

		if( pMenuList->ScrollType == MST_DRAW )
		{
            //EQS_DrawTitlePannel();

			EQS_FillRectangle( EQC_GET_X(hControl),
							   EQC_GET_Y(hControl), 
							   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
							   EQC_GET_Y(hControl) + nTitleHeight-1,
							   CAPTION_BACK_COLOR);

			EQS_FillRectangle( 0,
							   EQC_GET_Y(hControl) + nTitleHeight-1, 
							   DevGetMaxX(), 
							   DevGetMaxY(),						   
							   BACK_COLOR);
		

			OldColor = EQS_GetForeColor();
			EQS_SetForeColor(R_WHITE);
			EQS_Line((COORD)EQC_GET_X(hControl), 
					 (COORD)EQC_GET_Y(hControl),
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl));

			EQS_Line((COORD)EQC_GET_X(hControl)-1, 
					 (COORD)EQC_GET_Y(hControl),
					 (COORD)EQC_GET_X(hControl)-1,
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
        
			EQS_SetForeColor(R_GRAY);
			EQS_Line((COORD)EQC_GET_X(hControl), 
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
			EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
					 (COORD)EQC_GET_Y(hControl)+1,
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
			EQS_SetForeColor(OldColor);


        

			TextColor = EQS_GetTextColor();
			//BackColor = EQS_GetBackColor();
			DrawMode  = EQS_GetDrawMode();

			EQS_SetTextColor(CAPTION_FORE_COLOR);
			//EQS_SetTextBackColor(CAPTION_BACK_COLOR);
			EQS_SetDrawMode(DM_OR);
			nTitlePosX = EQC_GET_DX(hControl)/2 - STRLEN(GET_MENU_TITLE(pMenuList->pMnode))*nWidth/2;
			nTitlePosX = (nTitlePosX > 0 ) ? nTitlePosX : 1;
			nTitlePosY = (nTitleHeight - EQS_CharHeight((BYTE)' ')) /2 + sTITLE_POS_OFFSET;

			
			if( pMenuList->nCurrentPos == 1 )
			{

				EQS_TextOutStrLenTDM( EQC_GET_X(hControl),
									  nTitlePosY,
									  (BYTE*)SF_LEFT_TRIANGLE,
									  2,
									  TDM_NORMAL);  

				EQS_TextOutStrLenTDM( EQC_GET_X(hControl)+EQC_GET_DX(hControl)-EQS_CharWidth((BYTE)' ')*2-1,
									  nTitlePosY,
									  (BYTE*)SF_RIGHT_TRIANGLE,
									  2,
									  TDM_NORMAL); 

			}

			WindowSymbolMode = EQS_GetWindowSymbolMode();
			switch( WindowSymbolMode )
			{
			case WTSM_NONE_MODE:
				break;
			case WTSM_NORMAL_MODE:
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

			if( pMenuList->nCurrentPos == 1 )
			{
#if 0
				sprintf((char*)StrBuf,"%c%c%s",SPECIAL_DIGIT,\
											   SPECIAL_DIGIT_1+pMenuList->nIndexStack[pMenuList->nCurrentPos-1],\
											   GET_MENU_TITLE(pMenuList->pMnode));
#endif
				sprintf((char*)StrBuf,"%d.%s",\
											 pMenuList->nIndexStack[pMenuList->nCurrentPos-1]+1,\
											 GET_MENU_TITLE(pMenuList->pMnode));
				nTitlePosX -= EQS_CharWidth((BYTE)' ');
				if( nTitlePosX < 0 )
				{
					nTitlePosX = 0;
				}
			}
			else
			{
				STRCPY(StrBuf, (BYTE*)GET_MENU_TITLE(pMenuList->pMnode) );
			}
			EQS_TextOutStr( (COORD)(EQC_GET_X(hControl)+nTitlePosX),
							(COORD)(EQC_GET_Y(hControl)+nTitlePosY),
							StrBuf);
			EQS_SetTextColor(TextColor);        
			//EQS_SetBackColor(BackColor);
			EQS_SetDrawMode(DrawMode);
		}
  
#ifdef CONFIG_EQS_COLOR_64K
		nTitleHeight += EQS_SWINDOW_TOP_MARGIN;
#endif

    }
    else
    {
        nTitleHeight = 0;
        nViewLine = pMenuList->nLine;
    }

    /* get color and draw mode */
    //TextColor = EQS_GetTextColor();
    //BackColor = EQS_GetBackColor();
    //DrawMode  = EQS_GetDrawMode();

    /* set color and draw mode */
    //EQS_SetDrawMode(DM_OR);
    //EQS_SetTextColor(TEXT_FORE_COLOR);
    //EQS_SetBackColor(TEXT_BACK_COLOR);



    nSize =  GET_MENU_MAX_LINE(pMenuList->pMnode);  

	if( pMenuList->ScrollType == MST_NORMAL )
	{
		RECT Rect;

        if( pMenuList->nPreviousIndex < pMenuList->nIndexStack[nCurrentPos] )
		{
			sDrawMenuText(hControl, nTitleHeight, pMenuList->nCursorStack[nCurrentPos] -1 , pMenuList->nPreviousIndex, FALSE);
			Rect.yTop = EQC_GET_Y(hControl) + (pMenuList->nCursorStack[nCurrentPos] -1) * EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		else
		{
			sDrawMenuText(hControl, nTitleHeight, pMenuList->nCursorStack[nCurrentPos] +1 , pMenuList->nPreviousIndex, FALSE);
			Rect.yTop = EQC_GET_Y(hControl) + (pMenuList->nCursorStack[nCurrentPos]) * EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		Rect.yBottom = Rect.yTop + 2 * EQS_CharHeight((BYTE)' ');
		Rect.xLeft = 0;
		Rect.xRight = DevGetMaxX();	
		sDrawMenuText(hControl, nTitleHeight, pMenuList->nCursorStack[nCurrentPos] , pMenuList->nIndexStack[nCurrentPos], TRUE);
		
		if( nSize > nViewLine )
		{				
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight-R_SCROLLBAR_SIZE-1, Rect.yBottom);			
		}
		else
		{
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);
		}
	}
	else if( pMenuList->ScrollType == MST_TOP_DOWN )
	{
		RECT Rect;
					
		Rect.xLeft = 0;
		Rect.xRight = DevGetMaxX();	
		if( pMenuList->nCursorStack[nCurrentPos] == 0 )
		{
			sDrawMenuText(hControl, nTitleHeight, nSize-1 , pMenuList->nPreviousIndex, FALSE);
			Rect.yTop = EQC_GET_Y(hControl)+ (nSize-1)*EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		}
		else
		{
			sDrawMenuText(hControl, nTitleHeight, 0, pMenuList->nPreviousIndex, FALSE);
			Rect.yTop = EQC_GET_Y(hControl)+nTitleHeight ;			
		}
		Rect.yBottom = Rect.yTop + EQS_CharHeight((BYTE)' ');

		sDrawMenuText(hControl, nTitleHeight, pMenuList->nCursorStack[nCurrentPos] , pMenuList->nIndexStack[nCurrentPos], TRUE);

		EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);

		Rect.yTop = EQC_GET_Y(hControl)+ pMenuList->nCursorStack[nCurrentPos]*EQS_CharHeight((BYTE)' ')+nTitleHeight ;
		Rect.yBottom = Rect.yTop + EQS_CharHeight((BYTE)' ');
		EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop, Rect.xRight, Rect.yBottom);		
	}
	else if( pMenuList->ScrollType == MST_SCROLL )
	{
		StartY = (COORD)(EQC_GET_Y(hControl)+ EQS_CharHeight((BYTE)' ')+nTitleHeight);
        if( pMenuList->nPreviousIndex < pMenuList->nIndexStack[nCurrentPos] )
		{   // scroll up			
			sDrawMenuText(hControl, nTitleHeight, nViewLine-1, pMenuList->nPreviousIndex, FALSE);
			ScrollScreen(0, nTitleHeight, 0, (UINT8)StartY, (UINT8)(DevGetMaxX()+1-R_SCROLLBAR_SIZE), (UINT8)((nViewLine-1)*EQS_TextHeight((BYTE*)" ")));
			sDrawMenuText(hControl, nTitleHeight, nViewLine-1, pMenuList->nIndexStack[nCurrentPos], TRUE);

		}
		else
		{
			sDrawMenuText(hControl, nTitleHeight, 0, pMenuList->nPreviousIndex, FALSE);
			ScrollScreen(0, StartY, 0, nTitleHeight, DevGetMaxX()+1-R_SCROLLBAR_SIZE, (UINT8)((nViewLine-1)*EQS_TextHeight((BYTE*)" ")));
			sDrawMenuText(hControl, nTitleHeight, 0, pMenuList->nIndexStack[nCurrentPos], TRUE);
		}	
		EQS_RefreshDisplayRect(hControl, 0, nTitleHeight, DevGetMaxX()-R_SCROLLBAR_SIZE, EQC_GET_Y(hControl)+ nViewLine*EQS_CharHeight((BYTE)' ')+nTitleHeight );
	}
	else // MST_DRAW, MST_REDRAW
	{
		if (nViewLine >= nSize)
		{
			pMenuList->nCursorStack[nCurrentPos] = pMenuList->nIndexStack[nCurrentPos];

			for (i = 0; i < nSize; i++)
			{

				if (pMenuList->nCursorStack[nCurrentPos] == i)  // reverse 
					sDrawMenuText(hControl, nTitleHeight, i, i, TRUE);
				else                                            // normal 
					sDrawMenuText(hControl, nTitleHeight, i, i, FALSE);
			}

#if !NORMAL_REVERSE
			OldColor = EQS_GetTextBackColor();
			EQS_SetTextBackColor(EQS_GetBackColor());
#endif 

			// Fill the rest lines with blank.
			StartX = EQC_GET_X(hControl);
			for ( /* Use 'i' as it was */  ; i < nViewLine; i++)
			{
				StartY = (COORD)(EQC_GET_Y(hControl)+ i * EQS_CharHeight((BYTE)' ')+nTitleHeight);

				EQS_TextOutStrLen(StartX, StartY, (BYTE *)" ", pMenuList->nChar);
			}
#if !NORMAL_REVERSE
			EQS_SetTextBackColor(OldColor);
#endif
		}
		else
		{
			uDrawIndx = (pMenuList->nIndexStack[nCurrentPos] - pMenuList->nCursorStack[nCurrentPos] 
				+ nSize) % nSize;

			for (i = 0; i < nViewLine; i++, uDrawIndx = MOD_INCREMENT(uDrawIndx, nSize))
			{
				if (pMenuList->nCursorStack[nCurrentPos] == i) // reverse 
					sDrawMenuText(hControl, nTitleHeight, i, uDrawIndx, TRUE);
				else                                           // normal 
					sDrawMenuText(hControl, nTitleHeight, i, uDrawIndx, FALSE);
			}
		}
	}

    /* restore color and draw mode */
    //EQS_SetTextColor(TextColor);        
    //EQS_SetBackColor(BackColor);
    //EQS_SetDrawMode(DrawMode);


//redstar 2001.02.14
//Draw Scroll Bar  
    if( nSize > nViewLine && pMenuList->ScrollType != MST_NORMAL )
    {
        nWidth = R_SCROLLBAR_SIZE+1;

#ifdef CONFIG_EQS_COLOR_64K
        EQS_AddScrollBar(NULL, &pMenuList->ScrollBar,SBT_VERT,
                         EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth,\
                         EQC_GET_Y(hControl)+nTitleHeight,\
                         nViewLine*EQS_CharHeight((BYTE)' '),0,100,nViewLine,nSize);
#elif
        EQS_AddScrollBar(NULL, &pMenuList->ScrollBar,SBT_VERT,
                         EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth,\
                         EQC_GET_Y(hControl)+nTitleHeight,\
                         EQC_GET_DY(hControl)-nTitleHeight,0,100,nViewLine,nSize);
#endif
        SBarInfo.nMax = nSize-nViewLine;
        SBarInfo.nMin = 0;
                    
        SBarInfo.nPageSize = nViewLine;       
        SBarInfo.nPos = pMenuList->nIndexStack[nCurrentPos] - pMenuList->nCursorStack[nCurrentPos];
        EQS_SetScrollBarInfo(&pMenuList->ScrollBar, &SBarInfo, TRUE);
    }

	if( pMenuList->ScrollType == MST_DRAW || pMenuList->ScrollType == MST_REDRAW )
	{
		EQS_FillRectangle(0,DevGetMaxY() - EQS_GetCMLHeight(), DevGetMaxX(), DevGetMaxY() , EQS_GetBackColor() );
		EQS_ProcessCMLEvent(&DrawEvent);
		EQS_RefreshDisplayRect(hControl, 0,0, DevGetMaxX()+1,DevGetMaxY()+1);
	}
	else if( pMenuList->ScrollType == MST_SCROLL )
	{
#ifdef CONFIG_EQS_COLOR_64K
		EQS_RefreshDisplayRect( hControl, EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth,\
                                EQC_GET_Y(hControl)+nTitleHeight,\
								EQC_GET_X(hControl)+EQC_GET_DX(hControl),
                                EQC_GET_Y(hControl)+nTitleHeight+nViewLine*EQS_CharHeight((BYTE)' ')
							   );
#elif
		EQS_RefreshDisplayRect( hControl, EQC_GET_X(hControl)+EQC_GET_DX(hControl)-nWidth,\
                                EQC_GET_Y(hControl)+nTitleHeight,\
								EQC_GET_X(hControl)+EQC_GET_DX(hControl),
								EQC_GET_Y(hControl)+EQC_GET_DY(hControl)-nTitleHeight 
							  );
#endif
	}
	else ;
	//EQS_UpdateControlDisplay();
    EQS_ReleaseDC();

	pMenuList->ScrollType = MST_DRAW;
}

LOCAL CONST BYTE* sGetBitmapData(ANIMATION_INFO_T* pAniInfo, UINT8 nCurrentFrame)
{
    WORD wFrameSize;
    UINT8 i;
    BYTE* pbData;


    wFrameSize = 0;
    pbData = (BYTE*)pAniInfo->pbData;

    if(IS_EQUS_X_IMAGE(&(pAniInfo->BitmapHeader)) 
       || IS_EQUS_X_ANIMATION(&(pAniInfo->BitmapHeader)))
    {
        for(i = 0; i < nCurrentFrame; i++)
        {
            wFrameSize = ((*(pbData)) << 8) + *(pbData+1);
            /* sizeof(WORD) is for frame size data */
            pbData += wFrameSize + sizeof(WORD);
        }
    }
	/* LZB 압축 - haengeun  2001-09-26 --> */
	else if(IS_EQUS_Z_IMAGE(&(pAniInfo->BitmapHeader))
           || IS_EQUS_Z_ANIMATION(&(pAniInfo->BitmapHeader)))
	{  
	  for(i = 0; i < nCurrentFrame; i++)
      {
            wFrameSize = ((*(pbData)) << 8) + *(pbData+1);
            /* sizeof(WORD) is for frame size data */
            pbData += wFrameSize + sizeof(WORD);
      }
    }	        
    /* <-- */
    else /* not packed data */
    {
        pbData = (BYTE*)GET_BITMAP_DATA_POS(pAniInfo, nCurrentFrame);
    }
    return pbData;
}

LOCAL void EQS_API sDrawIconMenuInstance(HCONTROL hControl, BOOL fDrawAni)
{
	UINT8 i,j;
	UINT8 uDrawIndx, nHeight, nWidth, nTitleHeight, nSize;
    UINT8 nStart, nViewNum;
    INT16 nTitlePos;
	MENU_LIST_T *pMenuList;
    COLOR  TextColor, TextBackColor;
    COLOR  OldColor = WHITE;
    COORD  StartX, StartY;
    DRAW_MODE DrawMode;
    UINT8 nCurrentPos;
    BYTE StrBuf[BUF_SIZE];
	BYTE* pszMenuItemText;



    BITMAP  BitMap;
    ANIMATION_INFO_T  *pAni;
   

    EQS_GetDC(TRUE);

	pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
    nCurrentPos = pMenuList->nCurrentPos;
    nSize = GET_MENU_MAX_LINE(pMenuList->pMnode);
    
    nHeight = EQS_CharHeight((BYTE)' ');
    nWidth  = EQS_CharWidth((BYTE)' ');


	
    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_SHOW_TITLE)
    {
#ifdef CONFIG_EQS_COLOR_64K				
		nTitleHeight = EQS_SWINDOW_HEADER_HEIGHT;
		StartY = 2;
#else
        nTitleHeight = EQS_CharHeight((BYTE)' ')+1;
		StartY = 1;
#endif
        /*EQS_Line((COORD)EQC_GET_X(hControl)-1, 
                 (COORD)EQC_GET_Y(hControl) + nTitleHeight,
                 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)+1)-1,
                 (COORD)EQC_GET_Y(hControl) + nTitleHeight);*/

		if( pMenuList->nPreviousIndex == -1   ||\
			pMenuList->ScrollType == MST_DRAW ||\
			pMenuList->ScrollType == MST_REDRAW )
		{
#ifdef CONFIG_EQS_COLOR_64K
            EQS_DrawTitlePannel();
#else        
			EQS_FillRectangle( EQC_GET_X(hControl),
							   EQC_GET_Y(hControl), 
							   EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1, 
							   EQC_GET_Y(hControl) + nTitleHeight-1,
							   CAPTION_BACK_COLOR);
        
			OldColor = EQS_GetForeColor();
			EQS_SetForeColor((COLOR)223);
			EQS_Line((COORD)EQC_GET_X(hControl), 
					 (COORD)EQC_GET_Y(hControl),
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl));
			EQS_Line((COORD)EQC_GET_X(hControl), 
					 (COORD)EQC_GET_Y(hControl),
					 (COORD)EQC_GET_X(hControl),
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight);
        
			EQS_SetForeColor((COLOR)14);
			EQS_Line((COORD)EQC_GET_X(hControl)+1, 
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1,
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
			EQS_Line((COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1, 
					 (COORD)EQC_GET_Y(hControl)+1,
					 (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl))-1,
					 (COORD)EQC_GET_Y(hControl) + nTitleHeight-1);
			EQS_SetForeColor(OldColor);
#endif        

			TextColor = EQS_GetTextColor();
			DrawMode  = EQS_GetDrawMode();

			EQS_SetTextColor(CAPTION_FORE_COLOR);
			EQS_SetDrawMode(DM_OR);
			nTitlePos = EQC_GET_DX(hControl)/2 - STRLEN(GET_MENU_TITLE(pMenuList->pMnode))*nWidth/2;
			nTitlePos = (nTitlePos > 0 ) ? nTitlePos : 0;

			EQS_TextOutStr( (COORD)(EQC_GET_X(hControl)+nTitlePos),
							(COORD)(EQC_GET_Y(hControl)+StartY),
							GET_MENU_TITLE(pMenuList->pMnode));
			EQS_SetTextColor(TextColor);        
			EQS_SetDrawMode(DrawMode);
		}

#ifdef CONFIG_EQS_COLOR_64K				
		//nTitleHeight += EQS_SWINDOW_TOP_MARGIN ;
#endif
            
    }
    else
	{
        nTitleHeight = 0;
	}

	DrawMode  = EQS_GetDrawMode();
    EQS_SetDrawMode(DM_OR);
	if( pMenuList->pBackImage != NULL )
	{
		if( pMenuList->nPreviousIndex == -1   ||\
			pMenuList->ScrollType == MST_DRAW ||\
			pMenuList->ScrollType == MST_REDRAW )
		{
#ifdef CONFIG_EQS_COLOR_64K	
			EQS_PutBitmap(EQS_SWINDOW_HEADER_X, EQS_SWINDOW_HEADER_HEIGHT, pMenuList->pBackImage);
#else
			EQS_PutBitmap(EQC_GET_X(hControl)+X_MARGIN, Y_MARGIN+EQC_GET_Y(hControl)+nTitleHeight, pMenuList->pBackImage);		
#endif
		}
		else if( pMenuList->nPreviousIndex != pMenuList->nIndexStack[nCurrentPos] )
		{
			RECT Rect;

			Rect.xLeft = l_RestorePos[pMenuList->nPreviousIndex][0];
			Rect.yTop  = l_RestorePos[pMenuList->nPreviousIndex][1];
			Rect.xRight = Rect.xLeft + RESTORE_IMAGE_WIDTH-1;
			Rect.yBottom = Rect.yTop + RESTORE_IMAGE_HEIGHT-1;

#ifdef CONFIG_EQS_COLOR_64K	
			EQS_PutBitmapRect(Rect.xLeft, Rect.yTop+nTitleHeight, pMenuList->pBackImage, Rect);
#else
			EQS_PutBitmapRect(Rect.xLeft, Rect.yTop+nTitleHeight, pMenuList->pBackImage, Rect);	
#endif
			EQS_RefreshDisplayRect(hControl, Rect.xLeft, Rect.yTop+nTitleHeight, Rect.xRight, Rect.yBottom+nTitleHeight);

		}
		else ;
	}

	EQS_SetDrawMode(DrawMode);
    // Adjust Cursor Position.
    //pMenuList->nCursorStack[nCurrentPos] = pMenuList->nIndexStack[nCurrentPos];

	for (i = 0; i < nSize; i++)
	{
/*
        StartX = X_MARGIN + EQC_GET_X(hControl) + (i%pMenuList->Lattice.nViewNumX)*pMenuList->Lattice.xSpace 
            + ((i % pMenuList->Lattice.nViewNumX) * ICON_WIDTH );
        StartY = Y_MARGIN + EQC_GET_Y(hControl) + nTitleHeight 
            + ((i / pMenuList->Lattice.nViewNumX) * (ICON_HEIGHT + pMenuList->Lattice.ySpace));
*/
		StartX = l_MenuPos[i][0];
		StartY = l_MenuPos[i][1] + nTitleHeight;
		if ( pMenuList->nIndexStack[nCurrentPos] == i  )
        {   

            pAni = GET_MENU_ICON_ANI(pMenuList->pMnode, i);
            BitMap.Header = pAni->BitmapHeader;
            BitMap.Header.fPalette = FALSE;
            BitMap.pbData = sGetBitmapData(pAni, pMenuList->nCurrentFrame);

            BitMap.pbPalette = NULL;
            OldColor = EQS_GetBackColor();
            EQS_SetBackColor((COLOR)RGB332(192, 255, 255));
            EQS_PutBitmap(StartX, StartY, &BitMap);
            EQS_SetBackColor(OldColor);
			if( pMenuList->nPreviousIndex != -1       &&\
				pMenuList->ScrollType     != MST_DRAW &&\
				pMenuList->ScrollType     != MST_REDRAW )
			{
				EQS_RefreshDisplayRect(hControl, StartX,StartY, StartX+BitMap.Header.Size.xWidth, StartY + BitMap.Header.Size.yHeight );
			}
        }       
	}
       
    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_CAPTION && fDrawAni == FALSE &&\
		 ( pMenuList->nPreviousIndex != pMenuList->nIndexStack[nCurrentPos] ||\
		   pMenuList->ScrollType == MST_DRAW ||\
		   pMenuList->ScrollType == MST_REDRAW )
	   )
    {
        // Draw Caption Line. start.
        
		i = pMenuList->nIndexStack[nCurrentPos];
        StartX = 0;                                                                         

		//if( GET_MENU_ITEM_TEXT(pMenuList->pMnode, i ) == NULL  || nCurrentPos > 0 )
		if( nCurrentPos > 0 )
		{
			pszMenuItemText = GET_MENU_STR(pMenuList->pMnode, i);
		}
		else
		{
			if( pMenuList->pszItemText[i] == NULL )
			{
				pszMenuItemText = GET_MENU_STR(pMenuList->pMnode, i);
			}
			else
			{
				pszMenuItemText = pMenuList->pszItemText[i];
			}
		}

        if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_CENTER_ALIGN )
        {
            StartX = StartX +  (DevGetMaxX() - StartX 
                    -  EQS_CharWidth((BYTE)' ') * STRLEN(pszMenuItemText) )/2;
        }
        else if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_RIGHT_ALIGN )
        {
            StartX = DevGetMaxX()-EQS_CharWidth((BYTE)' ')*STRLEN( pszMenuItemText );
        }
        else ;

        StartY = (COORD)(0 + DevGetMaxY() - nHeight);

        TextColor     = EQS_GetTextColor();
		TextBackColor = EQS_GetTextBackColor();
		EQS_SetTextColor(BLACK);
		EQS_SetTextBackColor(WHITE);
  
        EQS_FillRectangle(0,
		    	          StartY-2,
                          0+DevGetMaxX()+1,
                          StartY + EQS_CharHeight((BYTE)' '),
                          WHITE);//EQS_GetTextBackColor());

        

        if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_RIGHT_ALIGN )
        {				
			STRCPY(StrBuf, pszMenuItemText);			
        }
        else
        {

			if( GET_MENU_ACCEL_KEY(pMenuList->pMnode, i) == 10 )
				STRCPY( StrBuf, (BYTE*)SF_BLACK_STAR);						
			else if( GET_MENU_ACCEL_KEY(pMenuList->pMnode, i) == 11 )
				STRCPY( StrBuf, (BYTE*)SF_BLACK_POUND);
			else
				sprintf((char*)StrBuf,"%c%c",SPECIAL_DIGIT,\
				        SPECIAL_DIGIT_1+GET_MENU_ACCEL_KEY(pMenuList->pMnode, i)-1);

			STRCAT(StrBuf, pszMenuItemText);

        }

		StartX = StartX +  (DevGetMaxX() - StartX 
					-  EQS_CharWidth((BYTE)' ') * (STRLEN(pszMenuItemText)+2) )/2;

        //EQS_TextOutStrTDM(StartX, StartY, StrBuf, TDM_BOLD); 
        EQS_TextOutStr(StartX, StartY, StrBuf); 

        EQS_SetTextColor(TextColor);
        EQS_SetTextBackColor(TextBackColor);

        // Draw Caption Line. end.
    }
            
/*
    if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_BORDER)
    {
        EQS_Rectangle( EQC_GET_X(hControl)-1 , EQC_GET_Y(hControl)-1,
                    (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)+1), 
                    (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)+1) );
    }
*/
	if( pMenuList->nPreviousIndex == -1   ||\
		pMenuList->ScrollType == MST_DRAW ||\
		pMenuList->ScrollType == MST_REDRAW )
	{
		EQS_RefreshDisplayRect(hControl, 0,0, DevGetMaxX()+1,DevGetMaxY()+1);
	}
	else if(pMenuList->nPreviousIndex != pMenuList->nIndexStack[nCurrentPos] ) 
	{
		EQS_RefreshDisplayRect(hControl, 0,StartY,DevGetMaxX()+1,DevGetMaxY()+1);
	}
	else ;

	pMenuList->nPreviousIndex = pMenuList->nIndexStack[nCurrentPos];

	if( pMenuList->ScrollType == MST_DRAW || pMenuList->ScrollType == MST_REDRAW )
	{
		pMenuList->ScrollType = MST_NORMAL;
	}
	//EQS_UpdateControlDisplay();
    EQS_ReleaseDC();
 
}

void sDrawBorder(COORD LeftX, COORD TopY, COORD RightX, COORD BottomY)
{
    COLOR ForeColor;

    ForeColor = EQS_GetForeColor();
    EQS_SetForeColor(R_WHITE);
    EQS_Line(LeftX-1,TopY+1,RightX-1,TopY+1);
    EQS_Line(LeftX+1, TopY-1,LeftX+1, BottomY-1);

    EQS_SetForeColor(R_LIGHTGRAY);
    EQS_Line(LeftX-1,TopY,RightX-1,TopY);
    EQS_Line(LeftX-1,BottomY,RightX-1,BottomY);
    EQS_Line(LeftX, TopY-1,LeftX, BottomY-1);
    EQS_Line(RightX,TopY-1,RightX,BottomY-1);

    EQS_SetForeColor(R_GRAY);
    EQS_Line(LeftX-1,BottomY+1,RightX-1,BottomY+1);
    EQS_Line(RightX+1,TopY-1,RightX+1,BottomY-1);
        
    EQS_SetForeColor(ForeColor);

}
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/

HCONTROL EQS_API EQC_MenuList(COORD x, COORD y, UINT8 nChar, UINT8 nLine, 
                              ROM MNODE_T* pMnode, UINT8 nStartIndx, BOOL fIcon)
{
	void *pInstance;
	HCONTROL hControl;
    UINT8 nWidth, nHeight;
    RECT   WindowRect;
	
	       
    x = 1;
    y = 0;
    EQS_GetSWindowViewAreaRect(&WindowRect);
    nWidth  = WindowRect.xRight-WindowRect.xLeft;
    nHeight = WindowRect.yBottom-WindowRect.yTop+1;
    nLine   = nHeight / EQS_CharHeight((BYTE)' ') ;
    nChar   = nWidth / EQS_CharWidth((BYTE)' ');

    pInstance = sAllocInstance(pMnode, nStartIndx, nChar, nLine, fIcon);
   // contains CS_MENU_LIST_SHOW_TITLE

	hControl= EQC_AllocControl(CT_MENU_LIST, pInstance, x, y, nWidth , nHeight, 
                               sHandleEvent);


    ASSERT (hControl != INVALID_CONTROL_HANDLE);

    if( fIcon )
        EQC_SET_STYLE(hControl, CS_MENU_LIST_ICON_DEFAULT);
    else   
        EQC_SET_STYLE(hControl, CS_MENU_LIST_DEFAULT);	

    EQS_SendEvent( EQE_APP_DRAW, (WORD)hControl, EQS_NA );

    return hControl;
}

void     EQS_API EQC_SetMenuListStyle(HCONTROL hControl, WORD StyleA)
{
    EQC_GET_INSTANCE(hControl)->Style = (StyleA);
    sGetLattice(hControl);
}

void     EQS_API EQC_SetMenuIconMode(HCONTROL hControl, BOOL fIcon)
{
	MENU_LIST_T *pMenuList;

    pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);

    ASSERT( EQC_GET_TYPE( hControl ) == CT_MENU_LIST);

	pMenuList->fIcon = fIcon;
}
 
void     EQS_API EQC_SetMenuImage(HCONTROL hControl, CONST BITMAP* pImage)
{

	MENU_LIST_T *pMenuList;

    pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
	ASSERT( EQC_GET_TYPE( hControl ) == CT_MENU_LIST);

	pMenuList->pBackImage = (BITMAP*)pImage;
}

void  EQS_API EQC_SetMenuBackColor(HCONTROL hControl, COLOR BackColor)
{
	MENU_LIST_T *pMenuList;

    pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
	ASSERT( EQC_GET_TYPE( hControl ) == CT_MENU_LIST);
	pMenuList->BackColor = BackColor;
}


LOCAL void sDrawMenuText(HCONTROL hControl, UINT8 nOffset, INT8 nPos, INT8 nIndex, BOOL fReverse )
{
	MENU_LIST_T *pMenuList;
    COLOR  TextColor;
    COORD  StartX, StartY;
    DRAW_MODE DrawMode;
    UINT8 nCurrentPos;
    BYTE StrBuf[BUF_SIZE];
    COLOR OldColor, OldTextColor;
	BYTE*  pszMenuItemText;
	

    pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);
    nCurrentPos = pMenuList->nCurrentPos;

/*
	if( GET_MENU_ITEM_TEXT(pMenuList->pMnode, nIndex ) == NULL || nCurrentPos > 0)
	{
		pszMenuItemText = GET_MENU_STR(pMenuList->pMnode, nIndex);
	}
	else
*/
	{
		if( pMenuList->pszItemText[nIndex] == NULL )
		{
			pszMenuItemText = GET_MENU_STR(pMenuList->pMnode, nIndex);
		}
		else
		{
			pszMenuItemText = pMenuList->pszItemText[nIndex];

		}
	}


	StartX = EQC_GET_X(hControl);                                                                         

	if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_CENTER_ALIGN )
	{
		StartX = StartX +  (EQC_GET_DX(hControl) - StartX 
				-  EQS_CharWidth((BYTE)' ') * STRLEN(pszMenuItemText) )/2;
	}
	else if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_RIGHT_ALIGN )
	{
		StartX = EQC_GET_DX(hControl)-EQS_CharWidth((BYTE)' ')*STRLEN( pszMenuItemText );
	}
	else ;

	StartY = (COORD)(EQC_GET_Y(hControl) + nPos* EQS_CharHeight((BYTE)' ')+ nOffset);
	
	
	if (EQC_GET_STYLE(hControl) & CS_MENU_LIST_ENUMERATED)
	{
		if( EQC_GET_STYLE(hControl) & CS_MENU_LIST_RIGHT_ALIGN )
		{
			STRCPY(StrBuf, pszMenuItemText);
		}
		else
		{
			if( GET_MENU_ACCEL_KEY(pMenuList->pMnode, nIndex) == 10 )
				STRCPY( StrBuf, (BYTE*)SF_BLACK_STAR);						
			else if( GET_MENU_ACCEL_KEY(pMenuList->pMnode, nIndex) == 11 )
				STRCPY( StrBuf, (BYTE*)SF_BLACK_POUND);
			else
				sprintf((char*)StrBuf,"%c%c",SPECIAL_DIGIT,\
						SPECIAL_DIGIT_1+GET_MENU_ACCEL_KEY(pMenuList->pMnode, nIndex)-1);
			STRCAT(StrBuf, pszMenuItemText);
		}
	}
	else
	{
		STRCPY(StrBuf, pszMenuItemText);
	}	

	if ( fReverse )
	{   // REVERSE
#if NORMAL_REVERSE 
		OldColor = EQS_GetTextBackColor();
		OldTextColor = EQS_GetTextColor();
		EQS_SetTextBackColor(EQS_GetBackColor());				
#else
		OldColor = EQS_GetTextBackColor();
		OldTextColor = EQS_GetTextColor();
		EQS_SetTextBackColor(SELECT_BACK_COLOR);
		EQS_SetTextColor(SELECT_FORE_COLOR);
#endif
		//EQS_TextOutStrLenTDM(StartX, StartY, StrBuf, pMenuList->nChar, TDM_UNDERLINE);//, TDM_REVERSE);
		//EQS_TextOutStrLenTDM(StartX, StartY, StrBuf, pMenuList->nChar,TDM_BOLD);
		EQS_TextOutStrLen(StartX, StartY, StrBuf, pMenuList->nChar);

		EQS_SetTextBackColor(OldColor);
		EQS_SetTextColor(OldTextColor);

		//OldColor = GetForeColor();
		//EQS_SetForeColor(EQS_GetTextColor());
		//EQS_Line(StartX, StartY+CharHeight(' ')-1, StartX + pMenuList->nChar*CharWidth(' ')-1, StartY+CharHeight(' ')-1);
		//EQS_SetForeColor(OldColor);

	}
	else
	{   // NORMAL

#if !NORMAL_REVERSE
		OldColor = EQS_GetTextBackColor();
		EQS_SetTextBackColor(EQS_GetBackColor());
#endif

		EQS_TextOutStrLen(StartX, StartY, StrBuf, pMenuList->nChar);
#if !NORMAL_REVERSE
		EQS_SetTextBackColor(OldColor);
#endif
	}
}

void     EQS_API EQC_SetMenuItemText(HCONTROL hControl,  UINT8 nTopIndex, BYTE* pszStr)
{
	MENU_LIST_T *pMenuList;

	pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);

	if( pMenuList->pszItemText[nTopIndex] == NULL )
	{
		pMenuList->pszItemText[nTopIndex] = (BYTE*) EQS_Malloc(32*sizeof(BYTE));
	}

	STRCPY(pMenuList->pszItemText[nTopIndex], pszStr);
}

BYTE*    EQS_API EQC_GetMenuItemText(HCONTROL hControl, UINT8 nTopIndex)
{
	MENU_LIST_T *pMenuList;

	pMenuList = (MENU_LIST_T *) EQC_GET_INSTANCE(hControl);

	return pMenuList->pszItemText[nTopIndex];
}

