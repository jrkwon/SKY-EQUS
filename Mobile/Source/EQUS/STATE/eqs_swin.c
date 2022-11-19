/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     eqs_swin.c
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
** 2000-10-04 girin     Created.
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
#ifdef CONFIG_EQS_COLOR_64K
#define SWINDOW_HEADER_STR_POS_Y 2
#else
#define SWINDOW_HEADER_STR_POS_Y 1
#endif

#define SWINDOW_HEADER_TOP_X     0
#define SWINDOW_HEADER_TOP_Y     0
#define SWINDOW_HEADER_BOTTOM_X  EQS_GetMaxX()
#define SWINDOW_HEADER_BOTTOM_Y  EQS_SWINDOW_HEADER_HEIGHT



/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

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
LOCAL ROM CML_INFO_T* l_pCommandLine;
LOCAL void (*l_pfnDefDrawSWindowHeader)(void) = NULL;

#define GET_CML_CONTEXT_NUM()           ((l_pCommandLine)->nNumCML)
#define GET_CML_F1_BITMAP()             ((l_pCommandLine)->pLeftBitMap)
#define GET_CML_CENTER_BITMAP()         ((l_pCommandLine)->pCenterBitMap)
#define GET_CML_F2_BITMAP()             ((l_pCommandLine)->pRightBitMap)
#define GET_CML_F1_EVENT(nCML)          ((EQS_EVENT_ID)(((*((l_pCommandLine)->pCommandLine))[nCML]).LeftCML.nSoftKeyEvent))
#define GET_CML_CENTER_EVENT(nCML)      ((EQS_EVENT_ID)(((*((l_pCommandLine)->pCommandLine))[nCML]).CenterCML.nSoftKeyEvent))
#define GET_CML_F2_EVENT(nCML)          ((EQS_EVENT_ID)(((*((l_pCommandLine)->pCommandLine))[nCML]).RightCML.nSoftKeyEvent))

#define GET_CML_F1_INDEX(nCML)          (((*((l_pCommandLine)->pCommandLine))[nCML]).LeftCML.nBitmapIndex)
#define GET_CML_CENTER_INDEX(nCML)      (((*((l_pCommandLine)->pCommandLine))[nCML]).CenterCML.nBitmapIndex)
#define GET_CML_F2_INDEX(nCML)          (((*((l_pCommandLine)->pCommandLine))[nCML]).RightCML.nBitmapIndex)

#define CML_SIZE_X                      DEF_CML_WIDTH
#define CML_SIZE_Y                      DEF_CML_HEIGHT
#define CML_POS_X                       0
#define CML_POS_Y                       (DevGetMaxHeight() - CML_SIZE_Y)


// Macros used by I/O Control Command Line
#define IOC_CML_LEFT_POS_X             0
#define IOC_CML_LEFT_POS_Y             CML_POS_Y

#define IOC_CML_CENTER_POS_X           (GET_CML_F1_BITMAP()->Header.xWidth)
#define IOC_CML_CENTER_POS_Y           CML_POS_Y

#define IOC_CML_RIGHT_POS_X            (IOC_CML_CENTER_POS_X+GET_CML_CENTER_BITMAP()->Header.xWidth)
#define IOC_CML_RIGHT_POS_Y            CML_POS_Y

#define GET_IOC_CML_BITMAP_LEFT()       (&g_BmpControlCommandLineLeft)
#define GET_IOC_CML_BITMAP_CENTER()     (&g_BmpControlCommandLineCenter)
#define GET_IOC_CML_BITMAP_RIGHT()      (&g_BmpControlCommandLineRight)

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

LOCAL BOOL          sDeserveEventByCML(EQS_EVENT_ID EventID);

LOCAL BOOL sDeserveEventByCML(EQS_EVENT_ID EventID)
{
    UINT8 nCML;
    
    if ((nCML = EQS_GetWindowCML()) >= GET_CML_CONTEXT_NUM())
        return FALSE;

    switch (EventID)
    {
    case EQE_KEY_F1:
        if (GET_CML_F1_EVENT(nCML) == EQE_KEY_NONE)
            return FALSE;
        else
            return TRUE;

    case EQE_KEY_CENTER:
        if (GET_CML_CENTER_EVENT(nCML) == EQE_KEY_NONE)
            return FALSE;
        else
            return TRUE;

    case EQE_KEY_F2:
	case EQE_KEY_F2_LONG:
        if (GET_CML_F2_EVENT(nCML) == EQE_KEY_NONE)
            return FALSE;
        else
            return TRUE;

    default:
        return FALSE;
    }
}


void    EQS_DrawCmdLine(UINT8 nCML)
{
    BITMAP Bitmap;

    if (nCML < GET_CML_CONTEXT_NUM())
    {
        // Left
        Bitmap = *(GET_CML_F1_BITMAP());
        Bitmap.Header = GET_CML_F1_BITMAP()->Header;
        Bitmap.pbData = GET_CML_F1_BITMAP()->pbData 
                        + EQC_GET_FRAME_SIZE(Bitmap.Header) * GET_CML_F1_INDEX(nCML);
        PutBitmap(IOC_CML_LEFT_POS_X, CML_POS_Y, &Bitmap);

        // Center
        Bitmap = *(GET_CML_CENTER_BITMAP());
        Bitmap.Header = GET_CML_CENTER_BITMAP()->Header;
        Bitmap.pbData = GET_CML_CENTER_BITMAP()->pbData 
                        + EQC_GET_FRAME_SIZE(Bitmap.Header) * GET_CML_CENTER_INDEX(nCML);
        PutBitmap(IOC_CML_CENTER_POS_X, CML_POS_Y, &Bitmap);

        // Right
        Bitmap = *(GET_CML_F2_BITMAP());
        Bitmap.Header = GET_CML_F2_BITMAP()->Header;
        Bitmap.pbData = GET_CML_F2_BITMAP()->pbData 
                        + EQC_GET_FRAME_SIZE(Bitmap.Header) * GET_CML_F2_INDEX(nCML);
        PutBitmap(IOC_CML_RIGHT_POS_X, CML_POS_Y, &Bitmap);
    }
}

#define USE_SKY_LOGO_SYSTEM_BOX     FALSE

#define S_LOGO_WIDTH    12//16
#define S_LOGO_HEIGHT   13//15


LOCAL void sDrawSWindowTitleStr(CONST BYTE *pText)
{
    COLOR  TextColor, BackColor, OldColor;
    DRAW_MODE DrawMode;
    TEXT_ALIGN OldAlign;
	WINDOW_TSYMBOL_MODE  WindowSymbolMode;
	UINT8   StartX;

#ifdef CONFIG_EQS_COLOR_64K
    //EQS_DrawTitlePannel();
    EQS_FillRectangle( EQS_SWINDOW_HEADER_X,
                       EQS_SWINDOW_HEADER_Y, 
                       GetMaxX(), 
                       EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT - 1,
                       CAPTION_BACK_COLOR);

    OldColor = EQS_GetForeColor();
    EQS_SetForeColor(R_WHITE);
    EQS_Line(EQS_SWINDOW_HEADER_X, 
             EQS_SWINDOW_HEADER_Y,
             GetMaxX(),
             EQS_SWINDOW_HEADER_Y);
    EQS_Line(EQS_SWINDOW_HEADER_X, 
             (COORD)EQS_SWINDOW_HEADER_Y,
             (COORD)EQS_SWINDOW_HEADER_X,
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    
    EQS_SetForeColor(R_GRAY);
    EQS_Line((COORD)(EQS_SWINDOW_HEADER_X+1), 
             (COORD)EQS_SWINDOW_HEADER_Y+ EQS_SWINDOW_HEADER_HEIGHT-1,
             GetMaxX(),
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    EQS_Line(GetMaxX(), 
             (COORD)EQS_SWINDOW_HEADER_Y+1,
             GetMaxX(),
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    EQS_SetForeColor(OldColor);
#else
    EQS_FillRectangle( EQS_SWINDOW_HEADER_X,
                       EQS_SWINDOW_HEADER_Y, 
                       GetMaxX(), 
                       EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT - 1,
                       CAPTION_BACK_COLOR);

    OldColor = EQS_GetForeColor();
    EQS_SetForeColor((COLOR)223);
    EQS_Line(EQS_SWINDOW_HEADER_X, 
             EQS_SWINDOW_HEADER_Y,
             GetMaxX(),
             EQS_SWINDOW_HEADER_Y);
    EQS_Line(EQS_SWINDOW_HEADER_X, 
             (COORD)EQS_SWINDOW_HEADER_Y,
             (COORD)EQS_SWINDOW_HEADER_X,
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    
    EQS_SetForeColor((COLOR)14);
    EQS_Line((COORD)(EQS_SWINDOW_HEADER_X+1), 
             (COORD)EQS_SWINDOW_HEADER_Y+ EQS_SWINDOW_HEADER_HEIGHT-1,
             GetMaxX(),
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    EQS_Line(GetMaxX(), 
             (COORD)EQS_SWINDOW_HEADER_Y+1,
             GetMaxX(),
             (COORD)EQS_SWINDOW_HEADER_Y + EQS_SWINDOW_HEADER_HEIGHT-1);
    EQS_SetForeColor(OldColor);
#endif

    TextColor = EQS_GetTextColor();
    BackColor = EQS_GetBackColor();
    DrawMode  = EQS_GetDrawMode();

    EQS_SetTextColor(CAPTION_FORE_COLOR);
    EQS_SetBackColor(CAPTION_BACK_COLOR);
    EQS_SetDrawMode(DM_OR);


#if USE_SKY_LOGO_SYSTEM_BOX
	StartX = GetMaxWidth()/2+S_LOGO_WIDTH/2;
#else
    StartX = GetMaxWidth()/2;
#endif


	WindowSymbolMode = EQS_GetWindowSymbolMode();
	switch( WindowSymbolMode )
	{
    case WTSM_NONE_MODE:
		break;
    case WTSM_NORMAL_MODE:
//#if USE_SKY_LOGO_SYSTEM_BOX
//		EQS_PutBitmap(EQS_SWINDOW_HEADER_X, 
//						EQS_SWINDOW_HEADER_Y, 
//						&g_BmpS16x15x256C);//&BitmapInfoHeadLogo);
//#endif
		break;
    case WTSM_CALL_MODE:
		StartX = GetMaxWidth()/2+S_LOGO_WIDTH/2;
		EQS_TextOutStr( 0, EQS_SWINDOW_HEADER_Y + 1, (BYTE*)SF_CALL_CALLING);
		break;

	case WTSM_DORMONT_MODE:
		StartX = GetMaxWidth()/2+S_LOGO_WIDTH/2;
		EQS_TextOutStr( 0, EQS_SWINDOW_HEADER_Y + 1, (BYTE*)SF_CALL_STND);
		break;

    case WTSM_NATIVE_MODE:
		break;
	default:
		break;
	}
	
    GetTextAlign(&OldAlign);
    EQS_SetTextAlignHoriz(TAH_CENTER);
    /*EQS_TextOutStrLen( (GetMaxWidth()-S_LOGO_WIDTH)/2,
	                EQS_SWINDOW_HEADER_Y + 1,
                    pText,
                    EQS_SWINDOW_HEADER_MAX_NCHAR);*/

    EQS_TextOutStr(StartX, EQS_SWINDOW_HEADER_Y + SWINDOW_HEADER_STR_POS_Y, pText);
    EQS_SetTextAlignHoriz(OldAlign.Horiz);

    EQS_SetTextColor(TextColor);
    EQS_SetBackColor(BackColor);
    EQS_SetDrawMode(DrawMode);
}

/*
**---------------------------------------------------------------------------
**  Function(external use) Declarations
**---------------------------------------------------------------------------
*/
void EQS_API EQS_InitSWindowHeader(void (*pfnDrawSWindowHeader)(void))
{
    l_pfnDefDrawSWindowHeader = pfnDrawSWindowHeader;
}


void EQS_API EQS_ProcessSWindowEvent(EQS_EVENT *pInEvent)
{
    WORD wAttr;
    CONST BYTE *pszHeaderStr;

    switch (pInEvent->EventID)
    {
    case EQE_APP_DRAW:

        wAttr = EQS_GetWindowAttr();
        if ((wAttr & WA_SHOW_HEADER) == WA_SHOW_HEADER)
        {
            pszHeaderStr = EQS_GetCurrentHeaderStr();
            if ( pszHeaderStr != NULL)
                sDrawSWindowTitleStr(pszHeaderStr);
            else // draw default SWIndow Header
            {
                if (l_pfnDefDrawSWindowHeader != NULL)
                {
                    (*l_pfnDefDrawSWindowHeader)();
                }
            }
        }

        break;
    }
}

void EQS_API EQS_InitCMLInfo(ROM CML_INFO_T* pCMLInfo)
{
    if (pCMLInfo != NULL)
        l_pCommandLine = pCMLInfo;
}

void EQS_API EQS_ProcessCMLEvent(EQS_EVENT *pInEvent)
{
    UINT8 nCML;
    WORD wAttr;

    nCML = EQS_GetWindowCML();

    switch (pInEvent->EventID)
    {
    case EQE_APP_DRAW:
        wAttr = EQS_GetWindowAttr();
        if ((wAttr & WA_SHOW_CML) == WA_SHOW_CML)
            EQS_DrawCmdLine(nCML);
        break;

    case EQE_KEY_F1:
        EQS_SendEvent(GET_CML_F1_EVENT(nCML), EQS_NA, EQS_NA);
        break;

    case EQE_KEY_CENTER:
        EQS_SendEvent(GET_CML_CENTER_EVENT(nCML), EQS_NA, EQS_NA);
        break;

    case EQE_KEY_F2:
	case EQE_KEY_F2_LONG:
        EQS_SendEvent(GET_CML_F2_EVENT(nCML), EQS_NA, EQS_NA);
        break;
    }
}

BOOL EQS_API EQS_DeserveEventByCML(EQS_EVENT_ID EventID)
{
    return sDeserveEventByCML(EventID);
}

UINT8 EQS_API EQS_GetSWindowHeaderHeight(void)
{
#ifdef CONFIG_EQS_COLOR_64K

	if( EQS_GetCurrentHeaderStr() == NULL )
	{
		return EQS_SWINDOW_ANNUNCIATOR_HEIGHT;
	}
	else
	{
		return EQS_SWINDOW_HEADER_HEIGHT;
	}
#else
    return EQS_SWINDOW_HEADER_HEIGHT;
#endif
}

UINT8 EQS_API EQS_GetCMLHeight(void)
{
    return CML_SIZE_Y;
}

/*
**---------------------------------------------------------------------------
**  I/O Control Command Line
**---------------------------------------------------------------------------
*/

void EQS_API EQS_DrawIOCCML(IOC_CML_E IOCCML)
{
    BITMAP Bitmap;

    VIEW_PORT OldViewPort;
    BOOL fOldSWindowState;

    if (IOCCML < IOC_CML_MAX)
    {
        fOldSWindowState = GetSlidingWindowState();
        GetViewPort(&OldViewPort);

        SetSlidingWindowState(FALSE);
        ResetViewPort();

        if (IOCCML < IOC_CML_BLANK_CENTER) // Left IOC CML
        {
            Bitmap = *GET_IOC_CML_BITMAP_LEFT();
            Bitmap.Header = GET_IOC_CML_BITMAP_LEFT()->Header;
            Bitmap.pbData = GET_IOC_CML_BITMAP_LEFT()->pbData + 
                EQC_GET_FRAME_SIZE(Bitmap.Header) * (IOCCML - IOC_CML_BLANK_LEFT);
            PutBitmap(IOC_CML_LEFT_POS_X, IOC_CML_LEFT_POS_Y, &Bitmap);
        }
        else if (IOCCML < IOC_CML_BLANK_RIGHT) // Center IOC CML
        {
            Bitmap = *GET_IOC_CML_BITMAP_LEFT();
            Bitmap.Header = GET_IOC_CML_BITMAP_CENTER()->Header;
            Bitmap.pbData = GET_IOC_CML_BITMAP_CENTER()->pbData + 
                EQC_GET_FRAME_SIZE(Bitmap.Header) * (IOCCML - IOC_CML_BLANK_CENTER);
            PutBitmap(IOC_CML_CENTER_POS_X, IOC_CML_CENTER_POS_Y, &Bitmap);
        }
        else // Right IOC CML
        {
            Bitmap = *GET_IOC_CML_BITMAP_LEFT();
            Bitmap.Header = GET_IOC_CML_BITMAP_RIGHT()->Header;
            Bitmap.pbData = GET_IOC_CML_BITMAP_RIGHT()->pbData + 
                EQC_GET_FRAME_SIZE(Bitmap.Header) * (IOCCML - IOC_CML_BLANK_RIGHT);
            PutBitmap(IOC_CML_RIGHT_POS_X, IOC_CML_RIGHT_POS_Y, &Bitmap);
        }

        SetViewPort(&OldViewPort);
        SetSlidingWindowState(fOldSWindowState);

    }
}

void     EQS_API EQS_UpdateIOCCML(void)
{
	EQS_RefreshDisplayRect(INVALID_CONTROL_HANDLE, CML_POS_X, CML_POS_Y,\
		                   CML_POS_X+CML_SIZE_X-1, CML_POS_Y+CML_SIZE_Y-1);
}

