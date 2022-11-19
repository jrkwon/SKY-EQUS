/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_frmt.c
**
** Description: 
**     EQUS line formatter
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
** 
** History: 
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-10-04 jrkwon     Created.
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
//#define GET_CONTROL_HEADER(hControl) (&g_ControlCommonHeader[hControl])
    
#define INIT_MARGIN_XLEFT           4//2
#define INIT_MARGIN_XRIGHT          INIT_MARGIN_XLEFT
#define INIT_MARGIN_YTOP            2
#define INIT_MARGIN_YBOTTOM         0
#define INIT_X_SPACE                4//2
#define INIT_Y_SPACE                3//4

#define SET_X_SPACE(x)              (g_pStyle->xSpace = (x))
#define SET_Y_SPACE(y)              (g_pStyle->ySpace = (y))
#define SET_MARGIN_XLEFT(x)         (g_pStyle->Margin.xLeft = (x))
#define SET_MARGIN_XRIGHT(x)        (g_pStyle->Margin.xRight = (x))
#define SET_MARGIN_YTOP(y)          (g_pStyle->Margin.yTop = (y))
#define SET_MARGIN_YBOTTOM(y)       (g_pStyle->Margin.yBottom = (y))
#define SET_MARGIN(m)               (g_pStyle->Margin = (m))

#define GET_X_SPACE()               (g_pStyle->xSpace)
#define GET_Y_SPACE()               (g_pStyle->ySpace)
#define GET_MARGIN_XLEFT()          (g_pStyle->Margin.xLeft)
#define GET_MARGIN_XRIGHT()         (g_pStyle->Margin.xRight)
#define GET_MARGIN_YTOP()           (g_pStyle->Margin.yTop)
#define GET_MARGIN_YBOTTOM()        (g_pStyle->Margin.yBottom)
#define GET_MARGIN()                (g_pStyle->Margin)
#define GET_STYLE()                 (*g_pStyle)

#define GET_TOTAL_WIDTH()           (g_nTotalWidth)
#define SET_TOTAL_WIDTH(nWidth)     (g_nTotalWidth = (nWidth))

//#define IS_FIXED_SIZE_CONTROL(Type) (Type == IOC_TYPE_ACTIVE_ICON || Type == IOC_TYPE_RADIO_BUTTON)
#define IS_FIXED_SIZE_CONTROL(Type) (Type == CT_ACTIVE_ICON)

#define GET_X_POS()                 (g_xPos)
#define GET_Y_POS()                 (g_yPos)
#define SET_X_POS(x)                (g_xPos = (x))
#define SET_Y_POS(y)                (g_yPos = (y))
#define GET_MAX_HEIGHT()            (g_yMaxHeight)
#define SET_MAX_HEIGHT(y)           (g_yMaxHeight = (y))

#define GET_COUNT_NOT_FIXED_ELE()      (g_nEleCount)
#define SET_COUNT_NOT_FIXED_ELE(n)     (g_nEleCount = (n))

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagELE_T             ELE_T;
typedef struct tagSTYLE_T           STYLE_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagELE_T {
    HCONTROL            hControl;
    ELE_SIZE_E          Size;
    UINT8               nSize;
    ELE_HORIZ_ALIGN_E   HorizAlign;
    ELE_VERT_ALIGN_E    VertAlign;
    BOOL                fFixedPos;
};

struct tagSTYLE_T {
    MARGIN_T    Margin; 
    COORD       xSpace; /* the space between two elements in the same line */
    COORD       ySpace; /* the line space between two lines */
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
LOCAL STYLE_T*  g_pStyle;
LOCAL LIST_T*   g_pLine;            /* the linked list for elements */
LOCAL UINT8     g_nTotalWidth;      /* the total width of the elements */
                                    /* except fixed positioned elements */

LOCAL COORD g_xPos;
LOCAL COORD g_yPos;
LOCAL COORD g_yMaxHeight;           /* in a line */
LOCAL UINT8 g_nEleCount;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/
LOCAL void EQS_API sDestroyElement(void *pData);
LOCAL void EQS_API sLeftAlign(void);
LOCAL void EQS_API sCenterAlign(void);
LOCAL void EQS_API sRightAlign(void);
LOCAL void EQS_API sReformatLine(ELE_T* pEle);
LOCAL void EQS_API sDestroyElement(void *pData);

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
void EQS_API EQS_StartFormatter(void)
{
    ASSERT(!g_pStyle);
    ASSERT(!g_pLine);

    g_pStyle = (STYLE_T*)EQS_Malloc(sizeof(STYLE_T));
    ASSERT(g_pStyle);
    g_pLine = (LIST_T*)EQS_Malloc(sizeof(LIST_T));
    ASSERT(g_pLine);

    InitList(g_pLine, sDestroyElement);

    SET_TOTAL_WIDTH(0);
    SET_MAX_HEIGHT(0);

    SET_COUNT_NOT_FIXED_ELE(0);

    /* init style */
    SET_MARGIN_XLEFT(INIT_MARGIN_XLEFT);
    SET_MARGIN_XRIGHT(INIT_MARGIN_XRIGHT);
    SET_MARGIN_YTOP(INIT_MARGIN_YTOP);
    SET_MARGIN_YBOTTOM(INIT_MARGIN_YBOTTOM);
    SET_X_SPACE(INIT_X_SPACE);
    SET_Y_SPACE(INIT_Y_SPACE);

    SET_X_POS(GET_MARGIN_XLEFT());
    SET_Y_POS(GET_MARGIN_YTOP());
}

void EQS_API EQS_EndFormatter(void)
{
    /* clear the current line -- double check !! */
    DestroyList(g_pLine);

    /* free */
    EQS_Free((void**)&g_pStyle);
    EQS_Free((void**)&g_pLine);
    g_pStyle = NULL;
    g_pLine = NULL;
}

void EQS_API EQS_AutoFormatting(UINT8 nControl,
                                ELE_HORIZ_ALIGN_E HorizAlign,
                                ELE_VERT_ALIGN_E VertAlign)
{
    UINT8 index;

    EQS_StartFormatter();

    for(index = 0; index < nControl; index++)
    {
        EQS_SetControlPosition(EQS_GetNthIOC(index), 
            ES_AUTO, 0, HorizAlign, VertAlign, FALSE);

        EQS_NewLine();
    }

    EQS_EndFormatter();
    EQS_FitCanvasSize();
}

void EQS_API EQS_SetMargin(COORD xLeft, COORD yTop, 
                           COORD xRight, COORD yBottom)
{
    g_pStyle->Margin.xLeft = xLeft;
    g_pStyle->Margin.yTop = yTop;
    g_pStyle->Margin.xRight = xRight;
    g_pStyle->Margin.yBottom = yBottom;
}

LOCAL void EQS_API sLeftAlign(void)
{
    //CommonControlHeader_T *pControlHeader;
    ELEMENT_T *pEleInList;
    ELE_T *pEle;
    COORD xMove;

    /* get the x position of the first element */
    pEleInList = GET_LIST_HEAD(g_pLine);
    pEle = (ELE_T *)pEleInList->pData;
    ASSERT(pEle);
    //pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
    //xMove = pControlHeader->x - GET_MARGIN_XLEFT();
    xMove = EQC_GET_X(pEle->hControl) - GET_MARGIN_XLEFT();
    ASSERT(xMove >= 0);

    for(pEleInList = GET_LIST_HEAD(g_pLine);
        pEleInList; pEleInList = GET_LIST_NEXT(pEleInList))
    {
        pEle = (ELE_T *)pEleInList->pData;
        ASSERT(pEle);

        if(pEle->fFixedPos == FALSE)
        {
            /* get the common control header */
            //pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
            //pControlHeader->x -= xMove;
            EQC_GET_X(pEle->hControl) -= xMove;
        }
    }
    g_xPos -= xMove;
}

LOCAL void EQS_API sCenterAlign(void)
{
    //CommonControlHeader_T *pControlHeader;
    ELEMENT_T *pEleInList;
    ELE_T *pEle;
    COORD xMove;
    UINT8 nWidth;

    /* get the total width of the elements */
    if(GET_COUNT_NOT_FIXED_ELE() > 0)
        nWidth = GET_TOTAL_WIDTH() 
                 + (GET_COUNT_NOT_FIXED_ELE() - 1) * GET_X_SPACE();
    else
        nWidth = GET_TOTAL_WIDTH();

    /* get the x position of the first element */
    pEleInList = GET_LIST_HEAD(g_pLine);
    pEle = (ELE_T *)pEleInList->pData;
    ASSERT(pEle);
    //pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
    //xMove = pControlHeader->x 
    //        - (EQS_GetMaxWidth()/2 - nWidth/2 - GET_MARGIN_XLEFT());
    
//    if(EQC_GET_TYPE(pEle->hControl) == CT_TEXT_EDIT 
//        || EQC_GET_TYPE(pEle->hControl) == CT_NUMBER_EDIT)
//        xMove = //EQC_GET_X(pEle->hControl) 
//                - (EQS_GetMaxWidth()/2 - nWidth/2);// - GET_MARGIN_XLEFT());
//    else
        xMove = EQC_GET_X(pEle->hControl) 
                - (EQS_GetMaxWidth()/2 - nWidth/2);// - GET_MARGIN_XLEFT());
    
    //ASSERT(xMove >= 0);

    for(pEleInList = GET_LIST_HEAD(g_pLine);
        pEleInList; pEleInList = GET_LIST_NEXT(pEleInList))
    {
        pEle = (ELE_T *)pEleInList->pData;
        ASSERT(pEle);

        if(pEle->fFixedPos == FALSE)
        {
            /* get the common control header */
            //pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
            //pControlHeader->x -= xMove;
            EQC_GET_X(pEle->hControl) -= xMove;
        }
    }
    /* get the last element's start postition + its width */
    //g_xPos = pControlHeader->x + pControlHeader->dx;
    g_xPos = EQC_GET_X(pEle->hControl) + EQC_GET_DX(pEle->hControl);
}

LOCAL void EQS_API sRightAlign(void)
{
    //CommonControlHeader_T *pControlHeader;
    ELEMENT_T *pEleInList;
    ELE_T *pEle;
    COORD xMove;

    /* get the first element */

    xMove = EQS_GetMaxX() - GET_MARGIN_XRIGHT() - GET_X_POS();
    ASSERT(xMove >= 0);

    for(pEleInList = GET_LIST_HEAD(g_pLine);
        pEleInList; pEleInList = GET_LIST_NEXT(pEleInList))
    {
        pEle = (ELE_T *)pEleInList->pData;
        ASSERT(pEle);

        if(pEle->fFixedPos == FALSE)
        {
            /* get the common control header */
            //pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
            //pControlHeader->x += xMove;
            EQC_GET_X(pEle->hControl) += xMove;
        }
    }
    g_xPos += xMove;
}


LOCAL void EQS_API sReformatLine(ELE_T* pEle)
{
//    CommonControlHeader_T *pControlHeader;

    /* get the common control header */
    //  pControlHeader = GET_CONTROL_HEADER(pEle->hControl);
    /* control start position */
    //pControlHeader->x = g_xPos;
    //pControlHeader->y = g_yPos;
    EQC_SET_X(pEle->hControl, g_xPos);
    EQC_SET_Y(pEle->hControl, g_yPos);

    /* -----------------------
    ** positioning the element 
    */
    
    /* 
    ** adjust x according to the width of the element 
    */
    
    /* adjust the size for the only not-fixed size item */
    //if(IS_FIXED_SIZE_CONTROL(pControlHeader->type) == FALSE)
    if(IS_FIXED_SIZE_CONTROL(EQC_GET_TYPE(pEle->hControl)) == FALSE)
    {
        /* get the height -- it's auto or fixed */
        //pControlHeader->dy = 

        /* calculate the size of the element */
        switch(pEle->Size)
        {
        case ES_AUTO :
            /* use the the size value given */
            break;
        case ES_PERCENT :
            //pControlHeader->dx = (EQS_GetMaxWidth() - GET_X_POS()
            //                     - GET_MARGIN_XRIGHT()) * (pEle->nSize/100.0);
//#pragma warning( push )
//#pragma warning( disable : 4244 )
            EQC_SET_DX(pEle->hControl, (EQS_GetMaxWidth() - GET_X_POS()
                                  - GET_MARGIN_XRIGHT()) * (pEle->nSize/100.0));
//#pragma warning( pop ) 
            break;
        case ES_PIXEL :
            //pControlHeader->dx = pEle->nSize;
            EQC_SET_DX(pEle->hControl, pEle->nSize);
            break;
        case ES_CHAR :
            //ontrolHeader->dx = EQS_CharWidth(' ') * pEle->nSize;
            EQC_SET_DX(pEle->hControl, EQS_CharWidth(' ') * pEle->nSize);
            break;
        }
        //g_nTotalWidth += pControlHeader->dx;
        g_nTotalWidth += EQC_GET_DX(pEle->hControl);
        g_nEleCount++;
    }
    
    if(pEle->fFixedPos == FALSE)
    {
        //g_xPos += pControlHeader->dx;
        //g_yMaxHeight = (g_yMaxHeight > pControlHeader->dy) 
        //                ? g_yMaxHeight : pControlHeader->dy;
        g_xPos += EQC_GET_DX(pEle->hControl);
        g_yMaxHeight = (g_yMaxHeight > EQC_GET_DY(pEle->hControl)) 
                        ? g_yMaxHeight : EQC_GET_DY(pEle->hControl);
    }

    /* ---------
    ** alignment 
    */
    if(pEle->HorizAlign != EHA_NO)
    {
        /* adjust positions for all of the elements in the line */
        switch(pEle->HorizAlign)
        {
        case EHA_LEFT :
            sLeftAlign();
            break;
        case EHA_CENTER :
            sCenterAlign();
            break;
        case EHA_RIGHT :
            sRightAlign();
            break;
        }
    }
    /* not yet implemented ---
    if(pEle->VertAlign != EVA_NO)
    {
        switch(pEle->VertAlign)
        {
        case EVA_TOP :
            break;
        case EVA_CENTER :
            break;
        case EVA_BOTTOM :
            break;
        }
    }
    --- */
}

LOCAL void EQS_API sDestroyElement(void *pData)
{
    EQS_Free(&pData);
}

void EQS_API EQS_NewLine(void)
{
    SET_Y_POS(GET_Y_POS() + GET_MAX_HEIGHT() + GET_Y_SPACE()); 

    SET_TOTAL_WIDTH(0);
    SET_MAX_HEIGHT(0);
    SET_COUNT_NOT_FIXED_ELE(0);

    /* clear the current line */
    DestroyList(g_pLine);
}

/* 
** CAUTION : you must add END_OF_CONTROL at the end of adding controls 
*/
void EQS_API EQS_SetControlPosition(HCONTROL hControl, 
                                    ELE_SIZE_E Size, UINT8 nSize,
                                    ELE_HORIZ_ALIGN_E HorizAlign,
                                    ELE_VERT_ALIGN_E VertAlign,
                                    BOOL fFixedPos)
{
    ELE_T* pEle;

    /* no element in the line */
    if(GET_LIST_SIZE(g_pLine) == 0)
    {
        /* init element list */
        InitList(g_pLine, sDestroyElement);
        SET_X_POS(GET_MARGIN_XLEFT());
    }
    else
    {
        g_xPos += GET_X_SPACE();
    }

    /* allocate the space for an element */
    pEle = (ELE_T*)EQS_Malloc(sizeof(ELE_T));
    ASSERT(pEle);

    /* set the value into the element */
    pEle->hControl = hControl;
    pEle->Size = Size;
    pEle->nSize = nSize;
    pEle->HorizAlign = HorizAlign;
    pEle->VertAlign = VertAlign;
    pEle->fFixedPos = fFixedPos;

    /* add the element at the head position */
    //EQS_InsertElement(g_pLine, NULL, pEle);
    InsertListElement(g_pLine, GET_LIST_TAIL(g_pLine), pEle);
    
    /* formatting the element in the line */
    sReformatLine(pEle);
}

void EQS_API EQS_GetControlMargin(MARGIN_T* pMargin)
{
    *pMargin = GET_MARGIN();
}

void EQS_API EQS_GetControlSpace(COORD* pxSpace, COORD* pySpace)
{
    *pxSpace = GET_X_SPACE();
    *pySpace = GET_Y_SPACE();
}

void EQS_API EQS_SetControlMargin(MARGIN_T* pMargin)
{
    SET_MARGIN(*pMargin);    
}

void EQS_API EQS_SetControlSpace(COORD xSpace, COORD ySpace)
{
    SET_X_SPACE(xSpace);
    SET_Y_SPACE(ySpace);
}

