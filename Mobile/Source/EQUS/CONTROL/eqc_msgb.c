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
** 00-07-11   girin      Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include "eqs.h"

#define NEW_MBOX

#define MSG_BOX_WIDTH   108//103
#define MSG_BOX_HEIGHT  87//70

#define MSG_BUTTON_BMP      g_BmpButtonYes // the size of buttons are all same
#define MSG_BUTTON_WIDTH    MSG_BUTTON_BMP.Header.xWidth //35
#define MSG_BUTTON_HEIGHT   MSG_BUTTON_BMP.Header.Size.yHeight //15

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
/// MesgBox Property
#define	MESGBOX_SPACING	    	2
#define MIN_BYTE_PER_LINE       10
#define MAX_TITLE_LEN           16
#define SOFTKEY_FONT            FT_MED 
#define DEFAULT_TIME            3   // secs

enum tag_EQC_MESGBOX_TYPE_E {
    EMT_NORMAL,
    EMT_SYSTEM,
    EMT_FIXEDSIZE,
    EMT_BEEP,
    EMT_BEEPSYSTEM,
    EMT_NOBEEP
};
typedef enum tag_EQC_MESGBOX_TYPE_E EQC_MESGBOX_TYPE_E;

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagMESSAGE_BOX_T     MESSAGE_BOX_T;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagMESSAGE_BOX_T
{
    CONTROL_HEADER_T    Header;
	BYTE                szMessage[MESSAGE_BOX_MAX_MESG_LEN+1];
	WORD               nSec;
    MESSAGE_BOX_TYPE_E  Type;
    WINDOW              *pWindow;
    BYTE                *szTitle;
    UINT8               nSelected;
    BUTTON              *pButton1;
    BUTTON              *pButton2;
    UINT8               nChar;
    UINT8               nLine;

    BOOL                fDarken;
};

/*
**---------------------------------------------------------------------------
**  Global variables
**---------------------------------------------------------------------------
*/
LOCAL HTIMER msgTimer = INVALID_TIMER_HANDLE;
/*
**---------------------------------------------------------------------------
**  Internal variables
**---------------------------------------------------------------------------
*/
//LOCAL l_fDarken = FALSE;
CONST static SIZE * g_pSize;
LOCAL BOOL  fBeep = FALSE;

const EQS_TOKEN MsgTitle[] = {
    TKN_MESSAGE_BOX_SUCCESS,
    TKN_MESSAGE_BOX_FAIL,
    TKN_MESSAGE_BOX_HELP,
    TKN_MESSAGE_BOX_CONFIRM,
    TKN_MESSAGE_BOX_SYSTEM
};

LOCAL COLOR_SCHEME_E l_Theme;

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/*  Message Box Alloc/Feee/Handle Function */
LOCAL void* EQS_API sAllocInstance(BYTE *pszMessage, WORD nSec, 
                                   MESSAGE_BOX_TYPE_E Type, 
                                   MESSAGE_BOX_TITLE_E TitleType);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent);

LOCAL void sEndMessageBox(HCONTROL hControl);
LOCAL RECT sDrawWindow(HCONTROL hControl, COORD xWidth, COORD yHeight, 
                       CONST SIZE *pSize);
LOCAL void  sDrawMessageBox(HCONTROL hControl);
LOCAL UINT8 sSpliteMessage(BYTE *pszSrcMsg, 
                           BYTE *pszDstBuf, 
                           UINT8 nLen, 
                           UINT8 nOffset, 
                           BOOL fAuto,
                           UINT8 *nStrLen);

LOCAL HCONTROL sCreateMessageBox(EQC_MESGBOX_TYPE_E mtype, 
                             BYTE* pszMessage, WORD nSec, 
                             MESSAGE_BOX_TYPE_E type, 
                             MESSAGE_BOX_TITLE_E TitleType,
                             UINT8 nChar, UINT8 nLine);            
/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Message Box Alloc/Feee/Handle Function
**---------------------------------------------------------------------------
*/
LOCAL void* EQS_API sAllocInstance(BYTE *pszMessage, WORD nSec, 
                                   MESSAGE_BOX_TYPE_E Type, 
                                   MESSAGE_BOX_TITLE_E TitleType)
{
    MESSAGE_BOX_T *pMessageBox;

	pMessageBox = (MESSAGE_BOX_T *)EQS_Malloc(sizeof(MESSAGE_BOX_T));
	if (pMessageBox != 0)
	{
        if(STRLEN(pszMessage) > MESSAGE_BOX_MAX_MESG_LEN)
        {
    		STRNCPY(pMessageBox->szMessage, pszMessage, MESSAGE_BOX_MAX_MESG_LEN);
            pMessageBox->szMessage[MESSAGE_BOX_MAX_MESG_LEN] = '\0';
        }
        else
        {
            STRCPY(pMessageBox->szMessage, pszMessage);
        }

        if(TitleType != MBT_NOTITLE)
            pMessageBox->szTitle = (BYTE *)EQS_GET_TOKEN(MsgTitle[TitleType]);
        else
            pMessageBox->szTitle = NULL;

        pMessageBox->nSec = nSec;
        pMessageBox->Type = Type;

        pMessageBox->pWindow = (WINDOW *)EQS_Malloc(sizeof(WINDOW));

        if(pMessageBox->Type == MBT_NOYES)
            pMessageBox->nSelected = 1;
        else
            pMessageBox->nSelected = 0;

        pMessageBox->pButton1 = NULL;
        pMessageBox->pButton2 = NULL;

        if(pMessageBox->Type == MBT_OK)
        {
            pMessageBox->pButton1 = (BUTTON *)EQS_Malloc(sizeof(BUTTON));
            pMessageBox->pButton1->pszText = 
                    (BYTE *)EQS_GET_TOKEN(TKN_MESSAGE_BOX_OK);
            pMessageBox->pButton1->fActive = FALSE;
        }

        if(pMessageBox->Type == MBT_CANCEL)
        {
            pMessageBox->pButton1 = (BUTTON *)EQS_Malloc(sizeof(BUTTON));
            pMessageBox->pButton1->pszText = 
                    (BYTE *)EQS_GET_TOKEN(TKN_POPUP_MENU_CANCEL);
            pMessageBox->pButton1->fActive = FALSE;
        }

        if(pMessageBox->Type == MBT_YESNO || pMessageBox->Type == MBT_NOYES)
        {
            // Yes
            pMessageBox->pButton1 = (BUTTON *)EQS_Malloc(sizeof(BUTTON));
            pMessageBox->pButton1->pszText = 
                    (BYTE *)EQS_GET_TOKEN(TKN_MESSAGE_BOX_YES);
            pMessageBox->pButton1->fActive = FALSE;
            
            // No
            pMessageBox->pButton2 = (BUTTON *)EQS_Malloc(sizeof(BUTTON));
            pMessageBox->pButton2->pszText = 
                    (BYTE *)EQS_GET_TOKEN(TKN_MESSAGE_BOX_NO);
            pMessageBox->pButton2->fActive = FALSE;
        }
	}

	return (void *)pMessageBox;
}

LOCAL void  EQS_API sFreeInstance(HCONTROL hControl)
{
	void* pVoid;
	pVoid = EQC_GET_INSTANCE(hControl);

    if(msgTimer != INVALID_TIMER_HANDLE)
        EQS_StopControlTimer(hControl, msgTimer);
 
    EQS_CloseWindow(((MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl))->pWindow);
 
    EQS_Free((void**)&((MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl))->pWindow);
    EQS_Free((void**)&((MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl))->pButton1);
    EQS_Free((void**)&((MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl))->pButton2);
	EQS_Free(&pVoid);
	EQC_SET_TYPE(hControl, CT_NONE);

    //l_fDarken = FALSE;

    if(EQS_GetModal() == hControl)
        EQS_EndModalIO();   // will cause EQE_APP_DRAW 
}

LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pEvent)
{
	MESSAGE_BOX_T *pMessageBox;

    ASSERT(EQC_GET_TYPE(hControl) == CT_MESSAGE_BOX);

	pMessageBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);

    if (pEvent->EventID == EQE_APP_IO_KILL)
    {
        sEndMessageBox(hControl);
        return;
    }
	else if (pEvent->EventID == EQE_APP_DRAW)
	{
		sDrawMessageBox(hControl);
        if(pMessageBox->Type == MBT_NONE){
            if(pMessageBox->nSec == 0)
            {
                //sEndMessageBox(hControl);
                EQS_SendEvent(EQE_APP_IO_KILL, hControl, EQS_NA);
                return;
            }
        }

        if((EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_BEEP) && !fBeep)
        {
            fBeep = TRUE;
            EQS_SendEvent(EQE_IO_BEEP_SOUND, hControl, EQC_GET_USERDATA(hControl));
        }
	}
	else
	{
        if ((EQS_GetCategory(pEvent->EventID) == EQC_KEY)
            && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_IGNORE_KEYS)
            && (pMessageBox->Type == MBT_NONE)) 
            return;

        
        if (pEvent->EventID == EQE_KEY_END)
        {
            EQS_SendEvent(EQE_IO_TO_IDLE, EQS_NA, EQC_GET_USERDATA(hControl));
            sEndMessageBox(hControl);
            return;
        }
        else if (( pEvent->EventID == EQE_KEY_CLEAR) || ( pEvent->EventID == EQE_IO_DELETE))
        {
			if(pEvent->EventID == EQE_KEY_CLEAR)
				EQS_SendEvent(EQE_IO_MESGBOX_CLEAR, (WORD)hControl, EQC_GET_USERDATA(hControl));
            sEndMessageBox(hControl);
            return;
        }
        else
        {
            switch(pMessageBox->Type)
            {
            case MBT_OK:
                if(pEvent->EventID == EQE_KEY_CENTER || pEvent->EventID == EQE_KEY_CLEAR
                    || pEvent->EventID == EQE_KEY_END)
                {
					if(pEvent->EventID == EQE_KEY_CLEAR)
						EQS_SendEvent(EQE_IO_MESGBOX_CLEAR, (WORD)hControl, EQC_GET_USERDATA(hControl));

                    EQS_SendEvent(EQE_IO_MESGBOX_OK, (WORD)hControl, EQC_GET_USERDATA(hControl));
                    sEndMessageBox(hControl);
                    return;
                }
                break;

            case MBT_CANCEL:
                if(pEvent->EventID == EQE_KEY_CENTER)
                {
                    EQS_SendEvent(EQE_IO_MESGBOX_CANCEL, (WORD)hControl, EQC_GET_USERDATA(hControl));
                    sEndMessageBox(hControl);
                    return;
                }
                break;

            case MBT_YESNO:
            case MBT_NOYES:
                switch(pEvent->EventID){
                case EQE_KEY_RIGHT:
                case EQE_KEY_LEFT:
                    pMessageBox->nSelected = (pMessageBox->nSelected)?0:1;
                    EQS_SendEvent(EQE_APP_DRAW, EQS_NA, EQS_NA);
                    break;

                case EQE_KEY_CENTER:
                    if(pMessageBox->nSelected == 0)
                        EQS_SendEvent(EQE_IO_MESGBOX_YES, (WORD)hControl, EQC_GET_USERDATA(hControl));
                    else
                        EQS_SendEvent(EQE_IO_MESGBOX_NO, (WORD)hControl, EQC_GET_USERDATA(hControl));

                    sEndMessageBox(hControl);
                    break;

                case EQE_KEY_CLEAR:
					EQS_SendEvent(EQE_IO_MESGBOX_CLEAR, (WORD)hControl, EQC_GET_USERDATA(hControl));
                    sEndMessageBox(hControl);
                    break;

                case EQE_KEY_END:
                    EQS_SendEvent(EQE_IO_MESGBOX_NO, (WORD)hControl, EQC_GET_USERDATA(hControl));
                    sEndMessageBox(hControl);
                    break;

                default:
                    break;
                }
                break;

            // XXX: Not Implemented Yet!
            case MBT_EVENT:
                break;

            case MBT_NONE:
                // go through...
            default:
                if ((EQS_GetCategory(pEvent->EventID) == EQC_KEY)
                    || (pEvent->EventID == EQE_TM_TIMER1))
                    sEndMessageBox(hControl);
            } /* switch */
        } /* else */
    } /* else */
}    
	 
LOCAL void sEndMessageBox(HCONTROL hControl)
{
    MESSAGE_BOX_T *pMessageBox;

    if (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SYSTEM)
    {
        EQS_SendEvent(EQE_IO_EXMSG_END, (WORD)hControl, EQC_GET_USERDATA(hControl));
    }
    else
    {
        pMessageBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);
        if(pMessageBox->Type == MBT_NONE)
	        EQS_SendEvent(EQE_IO_MESGBOX_END, (WORD)hControl, EQC_GET_USERDATA(hControl));
    }

    sFreeInstance(hControl);
}

LOCAL UINT8 sSpliteMessage(BYTE *pszSrcMsg, 
                           BYTE *pszDstBuf, 
                           UINT8 nLimit, 
                           UINT8 nOffset, 
                           BOOL fAuto,
                           UINT8 *nSizeLen)
{
    BYTE   *pszTmpMsg, *pDest;
    UINT8   i, j, nSPoint;
    UINT8   nMinByte;
	INT8    nNum;

    pszTmpMsg = (BYTE *)EQS_Malloc((SIZE_T)(nLimit+1));
    memset(pszTmpMsg, ' ', (SIZE_T)(nLimit+1));
    memcpy(pszTmpMsg, pszSrcMsg+nOffset, nLimit);
    pszTmpMsg[nLimit] = '\0';
    nSPoint = 0;

    if((pDest = STRCHR(pszTmpMsg, '\n')) != NULL)
    {      // If there is New-Line Char.
        nNum = pDest - pszTmpMsg;
        if (nNum == 0)
        {
            pszTmpMsg[0] = ' ';
            if ((pDest = STRCHR(pszTmpMsg, '\n')) != NULL)
                nNum = pDest - pszTmpMsg;
        }

        pszTmpMsg[nNum] = ' ';
        if (nNum > 1)
            nSPoint = nNum;

    }
    else if(fAuto)
    { 
        if(STRLEN((BYTE*)&pszSrcMsg[nOffset]) -  nLimit < MIN_BYTE_PER_LINE)
        {
            //nMinByte = MIN_BYTE_PER_LINE /2 ;
            nMinByte = 6;
        }
        else
            nMinByte = MIN_BYTE_PER_LINE;

        for(i = nMinByte; i < nLimit; i++)
        {
            if(pszTmpMsg[i] == ' ' || pszTmpMsg[i] == '\0')
            {
                nSPoint = i;
                break;
            }
        }
    }

    if(nSPoint > 0)
    {
        i = j = 0;
        while(pszTmpMsg[i] == ' ' && pszTmpMsg[i] != NULL)
            i++;

        if(nSPoint > i)
            memcpy(pszDstBuf, (BYTE*)(pszTmpMsg+i), (SIZE_T)(nSPoint+1-i));
        else
            pszDstBuf[0] = NULL;

        EQS_Free((void**)&pszTmpMsg);

        while(pszDstBuf[nSPoint-i-j] == ' ')
            j ++;

        if(nSPoint > (i+j))
            *nSizeLen = nSPoint+1-i-j;
        else 
            *nSizeLen = 0;

        return (nSPoint+1);
    }
    else
    {
        i = j = 0;
        while(pszTmpMsg[i] == ' ')
            i++;

        if(IsMiddleOfMultiByte(pszTmpMsg, nLimit))
        {
            memcpy(pszDstBuf, (BYTE*)(pszTmpMsg+i), (SIZE_T)(nLimit-1-i));
            memset(pszDstBuf+(nLimit-i), '\0', 1);
            EQS_Free((void**)&pszTmpMsg);

            while(pszDstBuf[nLimit-i-1-j] == ' ')
                j ++;

            if(nLimit > (i+j))
                *nSizeLen = nLimit-1-i-j;
            else
                *nSizeLen = 0;
            return (nLimit-1);
        }
        else
        {
            memcpy(pszDstBuf, (BYTE*)(pszTmpMsg+i), nLimit-i);
            memset(pszDstBuf+(nLimit+1-i), '\0', 1);
            EQS_Free((void**)&pszTmpMsg);

            while(pszDstBuf[nLimit-i-j] == ' ')
                j ++;

            if(nLimit > (i+j))
                *nSizeLen = nLimit-i-j;
            else
                *nSizeLen = 0;
            return (nLimit);
        }
    }

    EQS_Free((void**)&pszTmpMsg);
    return 0;       // error!
}

LOCAL COORD sButtonWidth(HCONTROL hControl)
{
    MESSAGE_BOX_T *pMsgBox;
    COORD          xWidth = 0;

    pMsgBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);

#ifdef NEW_MBOX
    if(pMsgBox->pButton1 != NULL)
        xWidth += MSG_BUTTON_WIDTH;

    if(pMsgBox->pButton2 != NULL)
        xWidth += (COORD)(MSG_BUTTON_WIDTH + MESGBOX_SPACING);
#else
    if(pMsgBox->pButton1 != NULL)
        xWidth += (COORD)(BUTTON_WIDTH(pMsgBox->pButton1));

    if(pMsgBox->pButton2 != NULL)
        xWidth += (COORD)(BUTTON_WIDTH(pMsgBox->pButton2) + MESGBOX_SPACING);
#endif
    return xWidth;
}

LOCAL COORD sButtonHeight(HCONTROL hControl)
{
#ifdef NEW_MBOX
    return MSG_BUTTON_HEIGHT;
#else
    MESSAGE_BOX_T *pMsgBox;

    pMsgBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);

    if(pMsgBox->pButton1 != NULL)
        return (COORD)(BUTTON_HEIGHT(pMsgBox->pButton1));

    if(pMsgBox->pButton2 != NULL)
        return (COORD)(BUTTON_HEIGHT(pMsgBox->pButton2));

    return 0;
#endif
}


// 버튼이 테두리에 너무 붙어 있는 거 같아서 간격을 좀 줘봤어요...
// 선미씨... 나중에 예쁘게 고쳐주세요.. 
#define MARGIN_WIDTH    2//4
#define MARGIN_HEIGHT   4

LOCAL RECT sDrawWindow(HCONTROL hControl, COORD xWidth, COORD yHeight, 
                       CONST SIZE *pSize)
{
    MESSAGE_BOX_T *pMsgBox;
    COORD   xPos, yPos, xTitleWidth;
	UINT8   nHeight; 
    RECT    Rect;

    pMsgBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);
	nHeight = EQS_CharHeight((BYTE)' ');

    // Draw Window
    pMsgBox->pWindow->Style = WS_BORDER;

//#ifndef NEW_MBOX
    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_TITLE)
    {
        pMsgBox->pWindow->pszCaption = pMsgBox->szTitle;
        pMsgBox->pWindow->Style |= WS_CAPTION;

        if(pMsgBox->Type == MBT_NONE)
            EQS_SetCaptionHeight(pMsgBox->pWindow, 19);
        else
            EQS_SetCaptionHeight(pMsgBox->pWindow, 15);
            
        xTitleWidth = (STRLEN(pMsgBox->szTitle) * pSize->xWidth) + (MESGBOX_SPACING * 2);

        if(xTitleWidth > xWidth)
            xWidth = xTitleWidth;
    }
//#endif

    if( (pMsgBox->Type == MBT_OK 
        || pMsgBox->Type == MBT_CANCEL
        || pMsgBox->Type == MBT_YESNO || pMsgBox->Type == MBT_NOYES)
        && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SOFTKEY)){
        if(xWidth < sButtonWidth(hControl))
            xWidth = sButtonWidth(hControl)+4;
    }

    xPos = (DevGetMaxX() - xWidth)/2+1;
//#ifndef NEW_MBOX
    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_TITLE)
        yPos = (DevGetMaxY() - yHeight + (pSize->yHeight + (MESGBOX_SPACING * 2)) )/2;
    else
//#endif
        yPos = (DevGetMaxY() - yHeight)/2;

    //xPos = (xPos < MARGIN_WIDTH) ? 0 : xPos - MARGIN_WIDTH;
    //yPos = (yPos < MARGIN_HEIGHT) ? 0 : yPos - MARGIN_HEIGHT;
    SetRect(&Rect, xPos, yPos, (COORD)(xPos+xWidth+/*MARGIN_WIDTH*/-1), (COORD)(yPos+yHeight+MARGIN_HEIGHT-1)); 
    //SetRect(&Rect, xPos, yPos, (COORD)(xPos+xWidth), (COORD)(yPos+yHeight)); 
    EQS_OpenWindow(pMsgBox->pWindow, &Rect, FALSE);

	EQC_SET_X(hControl, Rect.xLeft);
	EQC_SET_DX(hControl, Rect.xRight - Rect.xLeft+1);
	if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_TITLE)
	{
		EQC_SET_Y(hControl, Rect.yTop-nHeight-1);
		EQC_SET_DY(hControl, Rect.yBottom-Rect.yTop+1+nHeight+1);
	}
	else
	{
		EQC_SET_Y(hControl, yPos);
		EQC_SET_DY(hControl, Rect.yBottom-Rect.yTop+1);
	}

    return Rect;
}

LOCAL UINT8 sNumMultiChar(BYTE *pszStr, UINT8 nLen)
{
    UINT8   i, nMulti;

    for(i = 0, nMulti = 0; i < nLen; i++)
    {
        if(*(pszStr+i) & 0x80)
        {
            nMulti ++;
            i++;
        }
    }

    return nMulti;
}

LOCAL void sDrawMessageBox(HCONTROL hControl)
{
    MESSAGE_BOX_T *pMsgBox;
    BYTE  szTmpMsg[MESSAGE_BOX_MAX_MESG_LEN+1];
    UINT8 nMaxChar, nMaxLine, nTextMaxX, nTextMaxY;
    INT8 nDrawOffset;
    COORD xBoxWidth, yBoxHeight;
    UINT8 nOffset, nLines, nLen, nMaxLen, i;
    BYTE  *pBuffer;
    RECT  Rect, RectButton;
    UINT8 *pnChar;
#ifdef NEW_MBOX
    COORD   nYOffset;
#endif
    UINT8 nStrLen;

    pMsgBox = (MESSAGE_BOX_T *)EQC_GET_INSTANCE(hControl);

    /* jrkwon 2001-02-17 make background screen darker */
    if(pMsgBox->fDarken == FALSE)
    {
        MakeScreenDarker(BG_DARK_COUNT);        
    }

    EQS_GetDC(TRUE);

    // Get Max Chars 
    EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));
//    EQS_SetTextColor(EQC_GET_TEXT_COLOR(hControl));

    g_pSize = GetFontSize(EQC_GET_FONT_TYPE(hControl), FK_ASCII);

#ifdef NEW_MBOX
    nTextMaxX = MSG_BOX_WIDTH -3;
    nTextMaxY = MSG_BOX_HEIGHT;
#else
    nTextMaxX = DevGetMaxX() - (MESGBOX_SPACING * 2) - (WINDOW_BORDER_SIZE *2);
    nTextMaxY = DevGetMaxY() - (MESGBOX_SPACING * 2) - (WINDOW_BORDER_SIZE *2);
#endif

    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_FIXED_SIZE)
        nMaxChar = pMsgBox->nChar;
    else
        nMaxChar = nTextMaxX / g_pSize->xWidth;

#ifndef NEW_MBOX
    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_TITLE)
    {
        if(STRLEN(pMsgBox->szTitle) > nMaxChar)
            MakeShortString(pMsgBox->szTitle, nMaxChar);

        nTextMaxY -= g_pSize->yHeight + (MESGBOX_SPACING * 2); 
    }
#endif

    if( (pMsgBox->Type == MBT_OK 
        || pMsgBox->Type == MBT_CANCEL
        || pMsgBox->Type == MBT_YESNO || pMsgBox->Type == MBT_NOYES)
        && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SOFTKEY))
        nTextMaxY -= sButtonHeight(hControl);

    nMaxLine = nTextMaxY / g_pSize->yHeight;

    // Draw Message Box
    pBuffer = (BYTE *)EQS_Malloc((SIZE_T)(nMaxChar*(nMaxLine+1)));
    pnChar = (BYTE *)EQS_Malloc((SIZE_T)(nMaxLine+1));
    memset(pBuffer, '\0', (SIZE_T)(nMaxChar*(nMaxLine+1)));

    nOffset = nLines = nMaxLen = 0;

    STRCPY(szTmpMsg, pMsgBox->szMessage);

    while(nOffset < STRLEN(szTmpMsg))
    {
        UINT8 nLimit;

        if((nOffset + nMaxChar) < STRLEN(szTmpMsg))
            nLimit = nMaxChar;
        else
            nLimit = STRLEN(szTmpMsg) - nOffset;

        if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_FIXED_SIZE)
            nLen = sSpliteMessage(szTmpMsg, (pBuffer+(nLines*nMaxChar)), nLimit, nOffset, FALSE, &nStrLen);
        else
            nLen = sSpliteMessage(szTmpMsg, (pBuffer+(nLines*nMaxChar)), nLimit, nOffset, TRUE, &nStrLen);

        *(pBuffer+(nLines*nMaxChar) + nLen+1) = '\0';

        if(nStrLen > nMaxLen)
        {
            nMaxLen = nStrLen;
        }
        pnChar[nLines] = nStrLen;
        nOffset += nLen;
        nLines++;

        if(nLines >= nMaxLine || nOffset >= STRLEN(szTmpMsg))    break;
    }

    if(nOffset < STRLEN(pMsgBox->szMessage))
    {
        STRCPY(szTmpMsg, pMsgBox->szMessage);

        memset(pBuffer, ' ', (SIZE_T)(nMaxChar*nMaxLine));
        nOffset = nLines = 0;

        while(nOffset < STRLEN(szTmpMsg)){
            nLen = sSpliteMessage(szTmpMsg, (pBuffer+(nLines*nMaxChar)), nMaxChar, nOffset, FALSE, &nStrLen);

            *(pBuffer+(nLines*nMaxChar) + nLen+1) = '\0';

            if(nStrLen > nMaxLen)
            {
                nMaxLen = nStrLen;
            }
            pnChar[nLines] = nStrLen;
            nOffset += nLen;
            nLines++;

            if(nLines >= nMaxLine || nOffset >= STRLEN(szTmpMsg))    break;
        }
    }

    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_FIXED_SIZE)
    {
        xBoxWidth = (pMsgBox->nChar * g_pSize->xWidth) + (MESGBOX_SPACING * 2);
        yBoxHeight = (pMsgBox->nLine * g_pSize->yHeight) + (MESGBOX_SPACING * 2);
        nMaxLen = pMsgBox->nChar;
    }
    else
    {
        xBoxWidth = (nMaxLen * g_pSize->xWidth) + (MESGBOX_SPACING * 2);
        yBoxHeight = (g_pSize->yHeight * nLines) + (MESGBOX_SPACING * 2);
    }

    if( (pMsgBox->Type == MBT_OK 
        || pMsgBox->Type == MBT_CANCEL
        || pMsgBox->Type == MBT_YESNO || pMsgBox->Type == MBT_NOYES)
        && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SOFTKEY))
        yBoxHeight += sButtonHeight(hControl);

#ifdef NEW_MBOX
    nYOffset = (MSG_BOX_HEIGHT - yBoxHeight)/2;

    xBoxWidth = MSG_BOX_WIDTH;
    yBoxHeight = MSG_BOX_HEIGHT;

    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_TITLE)
    {
        if(pMsgBox->Type == MBT_NONE)
        {
            yBoxHeight -= 19;
            nYOffset -= 9;
        }
        else
        {
            yBoxHeight -= 15;
            nYOffset -= 7;
        }
        if(nLines >= 3)
        {
            yBoxHeight += 13;
            nYOffset += 7;
        }
    }
#else
    nYOffset = 0;
#endif

    Rect = sDrawWindow(hControl, xBoxWidth, yBoxHeight, g_pSize);

    //EQS_SetTextColor(TEXT_FORE_COLOR); // XXX: Temp
	EQS_SetTextColor(POPUP_NORMAL_FORE_COLOR);

    EQS_SetDrawMode(DM_OR);

    for(i = 0; i < nLines; i++)
    {
        BYTE* pTmpChar;

        if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_CENTER_ALIGN)
        {
            nDrawOffset = ((Rect.xRight - Rect.xLeft)
                            - ((pnChar[i] * g_pSize->xWidth) + sNumMultiChar((pBuffer + (i*nMaxChar)), nMaxLen))) / 2;

            if(nDrawOffset < 0) nDrawOffset = 0;
        }
        else
        {
            nDrawOffset = 0;
        }

        memcpy(szTmpMsg, (void*)(pBuffer + (i*nMaxChar)), nMaxLen);
        szTmpMsg[nMaxLen] = '\0';

        while((pTmpChar = STRCHR(szTmpMsg, '\n')) != NULL)
            *pTmpChar = ' ';

        EQS_TextOutStrLen(	(COORD)(Rect.xLeft + nDrawOffset + MESGBOX_SPACING), 
            (COORD)(Rect.yTop + MESGBOX_SPACING + nYOffset + (g_pSize->yHeight*i)),
            szTmpMsg, 
            nMaxLen);
    }
    EQS_SetDrawMode(DM_COPY);

    // Draw Soft Key 
    if( (pMsgBox->Type == MBT_OK 
        || pMsgBox->Type == MBT_CANCEL
        || pMsgBox->Type == MBT_YESNO || pMsgBox->Type == MBT_NOYES)
        && (EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_SOFTKEY))
    {
        RectButton.xLeft = Rect.xLeft + 
                           (((Rect.xRight-Rect.xLeft) - sButtonWidth(hControl) + MARGIN_WIDTH)/2);
//                           (((Rect.xRight-Rect.xLeft) - sButtonWidth(hControl))/2);
        RectButton.yTop = Rect.yBottom - MESGBOX_SPACING*2 - sButtonHeight(hControl);
//        RectButton.yTop = Rect.yTop + MESGBOX_SPACING*2 + (g_pSize->yHeight*nLines);

        switch(pMsgBox->Type)
        {
        case MBT_OK:
        case MBT_CANCEL:
#ifndef NEW_MBOX
            if(!pMsgBox->pButton1->fActive)
            {
                pMsgBox->pButton1->fActive = TRUE;
                EQS_AddButton(NULL, pMsgBox->pButton1, 
                    RectButton.xLeft, RectButton.yTop, BUTTON_WIDTH_AUTO);
            }
            EQS_DrawButton(pMsgBox->pButton1, TRUE);
#else
            EQS_PutBitmap(RectButton.xLeft, RectButton.yTop, &g_BmpButtonConfirm);
#endif
            break;

        case MBT_YESNO:
        case MBT_NOYES:
#ifndef NEW_MBOX
            if(!pMsgBox->pButton1->fActive)
            {
                pMsgBox->pButton1->fActive = TRUE;
                EQS_AddButton(NULL, pMsgBox->pButton1, 
                    RectButton.xLeft, RectButton.yTop, BUTTON_WIDTH_AUTO);

                pMsgBox->pButton2->fActive = TRUE;
                EQS_AddButton(NULL, pMsgBox->pButton2, 
                    (COORD)(pMsgBox->pButton1->Frame.xRight + MESGBOX_SPACING), 
                    RectButton.yTop, BUTTON_WIDTH_AUTO);
            }

            EQS_DrawButton(pMsgBox->pButton1, (BOOL)!pMsgBox->nSelected);
            EQS_DrawButton(pMsgBox->pButton2, (BOOL)pMsgBox->nSelected);
#else
            if(pMsgBox->nSelected == 0)
            {
                EQS_PutBitmap(RectButton.xLeft, RectButton.yTop, &g_BmpButtonYesFocused);

                EQS_PutBitmap(RectButton.xLeft+MSG_BUTTON_WIDTH+MESGBOX_SPACING, RectButton.yTop, &g_BmpButtonNo);
            }
            else
            {
                EQS_PutBitmap(RectButton.xLeft, RectButton.yTop, &g_BmpButtonYes);
                EQS_PutBitmap(RectButton.xLeft+MSG_BUTTON_WIDTH+MESGBOX_SPACING, RectButton.yTop, &g_BmpButtonNoFocused);
            }
#endif

            break;
        default:
            break;
        }
    }  

    EQS_Free((void**)&pBuffer);
    EQS_Free((void**)&pnChar);
	if( pMsgBox->fDarken == FALSE )  //redstar 01.08.29
	{
		pMsgBox->fDarken = TRUE;
		EQS_RefreshDisplayRect(hControl, 0, 0, DevGetMaxX(), DevGetMaxY());
	}
	else
	{
		EQS_UpdateControlDisplay();
	}
    EQS_ReleaseDC();
}

LOCAL HCONTROL sCreateMessageBox(EQC_MESGBOX_TYPE_E mtype, 
                             BYTE* pszMessage, WORD nSec, 
                             MESSAGE_BOX_TYPE_E type, 
                             MESSAGE_BOX_TITLE_E TitleType,
                             UINT8 nChar, UINT8 nLine)
{
	void *pInstance;
	HCONTROL hControl;
    MESSAGE_BOX_T *pMessageBox;
    UINT8 nMaxMsgLen;

    // girin. 2000.12.19
    if((hControl = EQS_GetModal()) != INVALID_CONTROL_HANDLE)
    {
        EQS_DeleteControl(hControl);
    }
    //

    if(mtype == EMT_FIXEDSIZE)
    {
        if((nChar * nLine) > MESSAGE_BOX_MAX_MESG_LEN)
            nChar = MESSAGE_BOX_MAX_MESG_LEN / nLine;

        nMaxMsgLen = nChar * nLine;
    }
    else
    {
        nMaxMsgLen = MESSAGE_BOX_MAX_MESG_LEN;
    }

    if(STRLEN(pszMessage) > nMaxMsgLen)
        MakeShortString(pszMessage, nMaxMsgLen);

	pInstance = sAllocInstance(pszMessage, nSec, type, TitleType);

	hControl = EQC_AllocControl(CT_MESSAGE_BOX, pInstance, 0, 0, 0, 0, 
                                sHandleEvent);

    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);

    pMessageBox = (MESSAGE_BOX_T*)pInstance;

    switch(mtype){
    case EMT_NORMAL:
    case EMT_BEEP:
    case EMT_NOBEEP:
        if(mtype == EMT_NOBEEP)
            EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_DEFAULT);
        else
            EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_DEFAULT|CS_MESSAGE_BOX_BEEP);

        EQS_SendEvent(EQE_IO_MESGBOX_START, EQS_NA, EQC_GET_USERDATA(hControl));
        break;

    case EMT_SYSTEM:
    case EMT_BEEPSYSTEM:
        EQC_SET_STYLE(hControl, CS_SYSMESSAGE_BOX_DEFAULT |CS_MESSAGE_BOX_BEEP);
        EQS_SendEvent(EQE_IO_EXMSG_START, EQS_NA, EQC_GET_USERDATA(hControl));
        break;

    case EMT_FIXEDSIZE:
        EQC_SET_STYLE(hControl, CS_MESSAGE_BOX_DEFAULT | 
                                CS_MESSAGE_BOX_FIXED_SIZE |
                                CS_MESSAGE_BOX_BEEP);

        EQS_SendEvent(EQE_IO_MESGBOX_START, EQS_NA, EQC_GET_USERDATA(hControl));

        pMessageBox->nChar = nChar;
        pMessageBox->nLine = nLine;

        break;
    }

    if(pMessageBox->Type == MBT_NONE 
        && pMessageBox->nSec != MESSAGE_BOX_TIME_UNLIMIT){
		msgTimer = EQS_StartControlTimer(hControl, EQS_TIMER_1, 
                              pMessageBox->nSec * 1000, FALSE);
    }

//#ifndef NEW_MBOX
    if(TitleType != MBT_NOTITLE)
        EQC_SET_STYLE(hControl, EQC_GET_STYLE(hControl) | CS_MESSAGE_BOX_TITLE);
//#endif

    fBeep = FALSE;
    EQS_SetIOCActive(hControl);
    pMessageBox->fDarken = FALSE;
    
    if(EQC_GET_STYLE(hControl) & CS_MESSAGE_BOX_BEEP)
    {
        switch(TitleType){
        case MBT_SUCCESS:
            EQS_SendEvent(EQE_IO_OK_MESGBOX_SOUND, hControl, EQS_NA);
            break;
        case MBT_FAIL:
            EQS_SendEvent(EQE_IO_FAIL_MESGBOX_SOUND, hControl, EQS_NA);
            break;
        case MBT_HELP:
            EQS_SendEvent(EQE_IO_HELP_MESGBOX_SOUND, hControl, EQS_NA);
            break;
        case MBT_CONFIRM:
            EQS_SendEvent(EQE_IO_USER_MESGBOX_SOUND, hControl, EQS_NA);
            break;
        case MBT_SYSTEM:
            EQS_SendEvent(EQE_IO_SYSTEM_MESGBOX_SOUND, hControl, EQS_NA);
            break;
        }
    }

	//EQS_SendEvent(EQE_APP_DRAW, hControl, EQS_NA);
	EQS_StartModalIO();

    return hControl;
}

/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL EQS_API EQC_MessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType)
{

    return sCreateMessageBox(EMT_NORMAL,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             EQS_NA, EQS_NA);
}

HCONTROL EQS_API EQC_BeepMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType)
{

    return sCreateMessageBox(EMT_BEEP,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             EQS_NA, EQS_NA);
}

HCONTROL EQS_API EQC_NoBeepMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType)
{

    return sCreateMessageBox(EMT_NOBEEP,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             EQS_NA, EQS_NA);
}

HCONTROL EQS_API EQC_SystemMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType)
{
    return sCreateMessageBox(EMT_SYSTEM,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             EQS_NA, EQS_NA);
}

HCONTROL EQS_API EQC_BeepSystemMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType)
{
    return sCreateMessageBox(EMT_BEEPSYSTEM,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             EQS_NA, EQS_NA);
}

HCONTROL EQS_API EQC_FixedMessageBox(BYTE* pszMessage, WORD nSec, 
                        MESSAGE_BOX_TYPE_E type, MESSAGE_BOX_TITLE_E TitleType, 
                        UINT8 nChar, UINT8 nLine)
{
    return sCreateMessageBox(EMT_FIXEDSIZE,
                             pszMessage,
                             nSec,
                             type,
                             TitleType,
                             nChar, nLine);
}

EQS_RESULT EQS_API EQC_AddMessageBoxEvent(HCONTROL hControl, 
                                          EQS_EVENT_ID Event)
{
    return RESULT_OK;
}


void EQC_SetMessageBoxStyle(HCONTROL hControl, WORD StyleA)
{
	EQC_GET_INSTANCE(hControl)->Style = (StyleA);
}


/*EQS_RESULT EQS_API EQC_SetCurrentMesgBoxTheme(COLOR_SCHEME_E Theme)
{
    l_Theme = Theme;

    return RESULT_OK;
}*/
