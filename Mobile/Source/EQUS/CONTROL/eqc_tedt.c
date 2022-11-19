/*
** ===========================================================================
**
** Project: 
**     EQUS
**
** File:
**     eqc_tedt.c
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
** 00-09-05   woodstok      Created.
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
#include "eqs_kor.tbl"      // 한글 코드 테이블
#include "eqs_eng.tbl"      // 영문 코드 테이블
#include "eqs_sym.tbl"      // Symbol 코드 테이블

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
//#define FEATURE_CURSOR_AUTOMOVE
#define FEATURE_COLOR_CURSOR

#define TE_MAX_LINE_WIDTH          (18) /* MAX_WIDTH/CHAR_WIDTH : 128/7 = 18. */

#define TE_AUTO_CURSOR_TIME        1500  
#define TE_BLINK_CURSOR_TIME       250

#define TE_SOFTKEY_Y               DEF_CML_HEIGHT
#define TE_SOFTKEY_FONT            FT_MED

#define GET_CURRENT_MODE(p)         p->EditMode.ModeSequence[p->EditMode.CurrentMode]

/* text box coordinates in symbol table */
#define sTEXT_BOX_IN_SYMBOL_LEFT        5
#define sTEXT_BOX_IN_SYMBOL_TOP         102
#define sTEXT_BOX_IN_SYMBOL_RIGHT       (128-5)
#define sTEXT_BOX_IN_SYMBOL_BOTTOM      (102+14)

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/
typedef struct tagCURSOR_T          CURSOR_T;
typedef struct tagTEXT_EDIT_T    TEXT_EDIT_T;

struct tagCURSOR_T 
{
    COORD   x;
    COORD   y;
    BYTE    pszStr[3];
    UINT8   nLen;
    BOOL    fReverse;
};

enum tag_TEXT_FORMAT_TYPE_E {
    TFT_A,  // UPPER
    TFT_a,  // LOWER
    TFT_N,  // NUMBER
    TFT_X,  // KOREAN->UPPER->SYMBOL
    TFT_x,  // KOREAN->LOWER->SYMBOL
    TFT_M,  // KOREAN->UPPER->LOWER->SYMBOL->NUMBER
    TFT_m,  // KOREAN->LOWER->UPPER->SYMBOL->NUMBER
    TFT_Error
};

typedef enum tag_TEXT_FORMAT_TYPE_E TEXT_FORMAT_TYPE_E;

enum tag_OLD_KOREAN_MODE_E {
    JAUM1,
    JAUM2,
    MOUM1,
    MOUM2
};

typedef enum tag_OLD_KOREAN_MODE_E OLD_KOREAN_MODE_E;

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/
struct tagTEXT_EDIT_T 
{
    CONTROL_HEADER_T Header;
	BYTE             *pszBuffer;        // String
    UINT16           nLen;              // String Length
    UINT16           nPos;              // Current Position(Bytes)
    UINT16           nMaxLen;           // Maximum length of String
    UINT8            nSymbolPage;       // Current Symbol Page
    BYTE             *pszFormat;        // WML Format String (eg."NN\\-MM")
    UINT8            nFormatLen;
    TEXT_FORMAT_TYPE_E  FormatType;     // Current WML Format Type(depend on WML Format String)
    EDIT_MODE_SELECT_T  EditMode;       // Current Edit Mode & available mode list
    BOOL             fPassword;         // TRUE, if PASSWORD property is set
    UINT8            nChar;
    UINT8            nLine;

    UINT8            nKoreanState;      // Current Korean Edit State(Jaum, Jaum+Moum...)
    HTIMER           hCursorTimer;      // Cursor Blink Timer handle
    HTIMER           hAutoCursorTimer;  // Auto Cursor move Timer 
    CURSOR_T         *pCursor;          // Cursor 
    SCROLL_BAR       *pScrollBar;       // Scroll Bar(in Active mode, default)
    SCROLL_BAR       *pBrowseScrollBar; // Scroll Bar(in Browse mode, VSCROLL property)
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
LOCAL BOOL  l_fSkyHangul;     

//LOCAL BOOL l_fDarken = FALSE;

LOCAL TEXT_EDIT_T   *pTextEdit;

LOCAL SIZE          g_FontSize;
LOCAL UINT8         g_nMaxCharPerLine;

LOCAL EQS_EVENT_ID  g_LastEvent, g_CurrentEvent;
LOCAL UINT8         g_nInputCount;
LOCAL BOOL          g_fEditChar;
LOCAL BOOL          g_fAdd;

LOCAL BYTE          *pszSaveBuffer = NULL;
LOCAL UINT16        nSavePos;
LOCAL UINT16        nSaveLen;

TEXT_FORMAT_TYPE_E  FormatType;

LOCAL OLD_KOREAN_MODE_E  l_KoreanMode;

LOCAL	BOOL	fDirty;
LOCAL   BYTE    l_LastSave[2];
LOCAL   UINT16  l_LastLen;
LOCAL   UINT16  l_LastPos;
LOCAL   UINT16  l_setPos = 0;

// for Hangul Input
static UINT16	uTmpCode, uKorCode, uWansung, uJohab, uBokMoum, uRecoverCons;
static UINT16  uSaveCode, uSaveCons, uSaveWansung, uSaveJohabCho, uSaveJohabJung, uSaveJohabJong;
static UINT16  uJohabCho, uJohabJung, uJohabJong;

// Normal Edit Mode
// LOCALE에 따라서 다르게 설정해야함...Not yet!
ROM EDIT_MODE_SELECT_T  NormalMode = {
    TEM_MAX,     0,
    {TEM_KOREAN, TEM_ENGLISH_UPPER, TEM_ENGLISH_LOWER, TEM_SYMBOL, TEM_NUMBER},
	TRUE
};

ROM EDIT_MODE_SELECT_T  PasswordMode = {
    3,     0,
    {TEM_ENGLISH_UPPER, TEM_ENGLISH_LOWER, TEM_NUMBER},
	TRUE
};

// WML Formatting Mode
ROM EDIT_MODE_SELECT_T  TFT_AMode = {
    1,     0,
    {TEM_ENGLISH_UPPER},
	FALSE
};

ROM EDIT_MODE_SELECT_T  TFT_aMode = {
    1,     0,
    {TEM_ENGLISH_LOWER},
	FALSE
};

ROM EDIT_MODE_SELECT_T  TFT_NMode = {
    1,     0,
    {TEM_NUMBER},
	FALSE
};

ROM EDIT_MODE_SELECT_T  TFT_XMode = {
    3,     0,
    {TEM_KOREAN, TEM_ENGLISH_UPPER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  TFT_xMode = {
    3,     0,
    {TEM_KOREAN, TEM_ENGLISH_LOWER, TEM_SYMBOL},
	FALSE
};

ROM EDIT_MODE_SELECT_T  TFT_MMode = {
    TEM_MAX,     0,
    {TEM_KOREAN, TEM_ENGLISH_UPPER, TEM_ENGLISH_LOWER, TEM_SYMBOL, TEM_NUMBER},
	TRUE
};

ROM EDIT_MODE_SELECT_T  TFT_mMode = {
    TEM_MAX,     0,
    {TEM_KOREAN, TEM_ENGLISH_LOWER, TEM_ENGLISH_UPPER, TEM_SYMBOL, TEM_NUMBER},
	TRUE
};

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Declarations
**---------------------------------------------------------------------------
*/

/*  Text Edit Alloc/Feee/Handle Function */
LOCAL void* EQS_API sAllocInstance(void);
LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT *pEvent);
LOCAL void  EQS_API sFreeInstance(HCONTROL hControl);

/*   위치 관련 함수들 */
LOCAL UINT16 sGetCurY(HCONTROL hControl);
LOCAL UINT16 sGetY(HCONTROL hControl);
LOCAL UINT16 sGetCurX(void);
LOCAL void sSetCursorPos(HCONTROL hControl);

/*  Text Edit Draw Functions */
LOCAL void sDrawCursor(HCONTROL hControl);
LOCAL void sDrawSoftKey(HCONTROL hControl);
LOCAL void sDrawScrollBar(HCONTROL hControl);
LOCAL void sDrawBrowseScrollBar(HCONTROL hControl);
LOCAL void sDrawTextBuf(HCONTROL hControl);
LOCAL void sDrawTextWindow(HCONTROL hControl);
LOCAL void sDrawDisplayTextEdit(HCONTROL hControl);
LOCAL void sDrawTextEdit(HCONTROL hControl);
LOCAL void sDrawStatusLine(TEXT_EDIT_T * pTextEdit);

/*  Text Edit Editing Functions */
LOCAL void  EQS_API sInitMode(TEXT_EDIT_T * pTextEdit);
LOCAL UINT16 sAddStr(HCONTROL hControl, BYTE * szStr, UINT16 nLen, BOOL fWord, BOOL fMovPos);
LOCAL BOOL sReplaceStr(HCONTROL hControl, BYTE * szStr, UINT16 nLen, BOOL fWord, BOOL fNextAdd);
LOCAL void sDeleteStr(HCONTROL hControl);
LOCAL void sProcMoveEvent(HCONTROL hControl, EQS_EVENT_ID EventID);

/*  한글 입력 관련 함수 */
LOCAL UINT16 sWansungToJohab(UINT16 wcode);
LOCAL UINT16 sJohabToWansung(UINT16 jcode);
LOCAL UINT16 sWansungToJohabJong(UINT16 code);
LOCAL UINT16 sGetBokMoum(UINT16 code1, UINT16 code2);
LOCAL UINT16 sGetBokJaum(UINT16 code1, UINT16 code2);

/*  언어별 Automata (Korean, English, Symbol, Number..) */
LOCAL void sKorean(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL void sOldKorean(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL void sEnglish(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL void sSymbol(HCONTROL hControl, EQS_EVENT_ID EventID);
LOCAL void sNumber(HCONTROL hControl, EQS_EVENT_ID EventID);
            
// WML Formatting 관련 함수
LOCAL UINT16 sGetStringLength(UINT16 nLen);
LOCAL void sSetDefaultFormatMode(TEXT_FORMAT_TYPE_E Type);
LOCAL TEXT_FORMAT_TYPE_E sGetCurrentFormat(UINT16 nLen);
LOCAL UINT16 sGetFormatLength(BYTE *pszFormat, UINT16 nLen, UINT8 nFormatLen);
LOCAL void sAddFormatChar(void);
LOCAL void sDeleteFormatChar(HCONTROL hControl);

/*
**---------------------------------------------------------------------------
**  Function(internal use only) Definitions
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Text Edit Alloc/Feee/Handle Function
**---------------------------------------------------------------------------
*/

LOCAL void* EQS_API sAllocInstance(void)
{

	void *pMem;

	pMem = EQS_Malloc(sizeof(TEXT_EDIT_T));
	if (pMem != 0){}

    //if(!l_fSkyHangul)
    //{
    //}
    
    return (void *)pMem;
}

LOCAL void  EQS_API sFreeInstance(HCONTROL hControl)
{
//	void* pVoid;
//	pVoid = EQC_GET_INSTANCE(hControl);

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    EQS_Free((void**)&(pTextEdit->pszBuffer));

    // Stop Timer
    if(pTextEdit->hCursorTimer != NULL)
    {
        EQS_StopControlTimer(hControl,pTextEdit->hCursorTimer);
        pTextEdit->hCursorTimer = NULL;
    }

    if(pTextEdit->hAutoCursorTimer != NULL)
    {
        EQS_StopControlTimer(hControl,pTextEdit->hAutoCursorTimer);
        pTextEdit->hAutoCursorTimer = NULL;
    }

    // Free Memory
    if(pTextEdit->pScrollBar != NULL)
        EQS_Free((void**)&(pTextEdit->pScrollBar));

    if(pTextEdit->pBrowseScrollBar != NULL)
        EQS_Free((void**)&(pTextEdit->pBrowseScrollBar));

    if(pTextEdit->pCursor != NULL)
        EQS_Free((void**)&(pTextEdit->pCursor));

    if(pszSaveBuffer != NULL)
    {
        EQS_Free((void**)&pszSaveBuffer);
        pszSaveBuffer = NULL;
    }

    if(pTextEdit->pszFormat != NULL)
        EQS_Free((void**)&(pTextEdit->pszFormat));

	EQS_Free((void**)&pTextEdit);
    EQC_SET_TYPE(hControl, CT_NONE);

    //l_fDarken = FALSE;

	return;
}

LOCAL void  EQS_API sInitMode(TEXT_EDIT_T * pTextEdit)
{
    g_LastEvent = EQE_NONE;
	g_nInputCount = 0;
    pTextEdit->nKoreanState = 0;
    g_fEditChar = FALSE;

    if(!l_fSkyHangul)
        l_KoreanMode = JAUM1;
}

LOCAL void EQS_API sEndEditChar(void)
{
    g_fEditChar = FALSE;

    if(pTextEdit->pszFormat != NULL)
    {
        FormatType = sGetCurrentFormat((UINT8)(pTextEdit->nPos+1));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);
    }
}

LOCAL BOOL EQS_API sIsFunctionKey(EQS_EVENT_ID EventID)
{
    if(EventID == EQE_KEY_F1 || 
        EventID == EQE_KEY_F2 ||
        EventID == EQE_KEY_LEFT ||
        EventID == EQE_KEY_RIGHT ||
        EventID == EQE_KEY_CENTER ||
        EventID == EQE_KEY_UP ||
        EventID == EQE_KEY_DOWN || 
        EventID == EQE_KEY_CLEAR)

        return TRUE;
    else
        return FALSE;
}

LOCAL void  EQS_API sHandleEvent(HCONTROL hControl, EQS_EVENT* pAnEvent)
{
    BOOL fOldSWindowState;
    VIEW_PORT OldViewPort, ViewPort;
    RECT    Rect;
    UINT8   i;
    TEXT_EDIT_MODE_E Mode;

	ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if((EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_PASSWORD) && !pTextEdit->fPassword)
    {
        if(pTextEdit->pszFormat == NULL)
        {
            Mode = GET_CURRENT_MODE(pTextEdit);
            pTextEdit->EditMode = PasswordMode;

            for(i = 0; i < (pTextEdit->EditMode.nModeNum) ; i++)
            {
                if(pTextEdit->EditMode.ModeSequence[i] == Mode)
                {
                    pTextEdit->EditMode.CurrentMode = i;
                    break;
                }
            }
            if(i == pTextEdit->EditMode.nModeNum)
                pTextEdit->EditMode.CurrentMode = 0;    
        }

        pTextEdit->fPassword = TRUE;
    }

	switch (pAnEvent->EventID)
	{
    case EQE_APP_IO_MODE:
        // Save State (For Cancel Key Event)
        if(pszSaveBuffer != NULL)
            EQS_Free((void**)&pszSaveBuffer);

        pszSaveBuffer = (BYTE*)EQS_Malloc((SIZE_T)(STRLEN(pTextEdit->pszBuffer)+1));
        STRCPY(pszSaveBuffer, pTextEdit->pszBuffer);
        nSavePos = pTextEdit->nPos;
        nSaveLen = pTextEdit->nLen;

        if(!l_fSkyHangul)
        {
            l_KoreanMode = JAUM1;
        }
        if(l_setPos)
            l_setPos = 0;
        else
            pTextEdit->nPos = pTextEdit->nLen;

        if(pTextEdit->pszFormat != NULL)
        {
            FormatType = sGetCurrentFormat((UINT16)(pTextEdit->nPos));
            if(FormatType != TFT_Error)
                sSetDefaultFormatMode(FormatType);
        }

		fDirty = EQC_GET_DIRTY(hControl);
        //EQC_SET_DIRTY(hControl, FALSE);
        memset(l_LastSave, NULL, 2);
        l_LastLen = l_LastPos = 0;
        sInitMode(pTextEdit);
        break;

	case EQE_APP_DRAW:
		sDrawTextEdit(hControl);
        EQS_UpdateControlDisplay();
        break;

    case EQE_KEY_CLEAR:
        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY)
        {
            pTextEdit->nPos = 0;
            EQS_EndIO();
            break;
        }

        if(pTextEdit->nLen == 0)
        {
            sInitMode(pTextEdit);
            // Restore State 
            STRCPY(pTextEdit->pszBuffer, pszSaveBuffer);
            if(pszSaveBuffer != NULL)
            {
                EQS_Free((void**)&pszSaveBuffer);
                pszSaveBuffer = NULL;
            }
            pTextEdit->nPos = nSavePos;
            pTextEdit->nLen = nSaveLen;     
            
            if(!fDirty)
            {
                EQC_SET_DIRTY(hControl, FALSE);
            }
            pTextEdit->nPos = 0;
            EQS_SendEvent(EQE_IO_TEDIT_CANCEL, (WORD)hControl, (DWORD)NULL);  
            EQS_EndIO();
            break;
        }

        // Go through..
    case EQE_KEY_UP:
    case EQE_KEY_DOWN:
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
    case EQE_KEY_RIGHT:
    case EQE_KEY_LEFT:
    case EQE_KEY_F2:
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
    // Handle Long Key Event (digit long key -> insert digit)
    case EQE_KEY_0_LONG:
    case EQE_KEY_1_LONG:
    case EQE_KEY_2_LONG:
    case EQE_KEY_3_LONG:
    case EQE_KEY_4_LONG:
    case EQE_KEY_5_LONG:
    case EQE_KEY_6_LONG:
    case EQE_KEY_7_LONG:
    case EQE_KEY_8_LONG:
    case EQE_KEY_9_LONG:
        if(EQS_IsActive(hControl) && !(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_ALONE)
            && GET_CURRENT_MODE(pTextEdit) != TEM_SYMBOL && l_fSkyHangul)
        {
            if(pAnEvent->EventID == EQE_KEY_UP && pTextEdit->nPos == 0)
            {
                if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
                {
   		            g_LastEvent = EQE_NONE;
		            g_nInputCount = 0;
                    sEndEditChar();
/*
                    if(STRCMP(pszSaveBuffer, pTextEdit->pszBuffer))
                        EQC_SET_DIRTY(hControl, TRUE);
*/
                    if(pszSaveBuffer != NULL)
                    {
                        EQS_Free((void**)&pszSaveBuffer);
                        pszSaveBuffer = NULL;
                    }
                }

                pTextEdit->nPos = 0;

                EQS_SendEvent(EQE_IO_TEDIT_DONE, (WORD)hControl, (DWORD)pTextEdit->nLen);     
                EQS_PassIO(-1);

                break;
            }
            else if(pAnEvent->EventID == EQE_KEY_DOWN && pTextEdit->nPos == pTextEdit->nLen)
            {
                if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
                {
   		            g_LastEvent = EQE_NONE;
		            g_nInputCount = 0;
                    sEndEditChar();
/*
                    if(STRCMP(pszSaveBuffer, pTextEdit->pszBuffer))
                        EQC_SET_DIRTY(hControl, TRUE);
*/
                    if(pszSaveBuffer != NULL)
                    {
                        EQS_Free((void**)&pszSaveBuffer);
                        pszSaveBuffer = NULL;
                    }
                }

                pTextEdit->nPos = 0;

                EQS_SendEvent(EQE_IO_TEDIT_DONE, (WORD)hControl, (DWORD)pTextEdit->nLen);     
                EQS_EndIO();

                break;
            }
        }

        g_CurrentEvent = pAnEvent->EventID;

        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY)
        {
            if(pAnEvent->EventID != EQE_KEY_UP &&
               pAnEvent->EventID != EQE_KEY_DOWN )
               break;
        }
        
        aLocaleTable[EQS_GetCountry()].pfnIME(hControl, pAnEvent);
        if(!sIsFunctionKey(pAnEvent->EventID))
            g_LastEvent = pAnEvent->EventID;
        // Start Timer 
        pTextEdit->hAutoCursorTimer = EQS_StartControlTimer(hControl, EQS_TIMER_2, TE_AUTO_CURSOR_TIME, FALSE);


        if(pAnEvent->EventID == EQE_KEY_F2)
        {
            EQS_SendEvent(EQE_APP_DRAW, 0, (DWORD)NULL);     
            //l_fDarken = FALSE;
        }
        else
            EQS_SendEvent(EQE_APP_DRAW, hControl, (DWORD)NULL);   
        
        break;

    case EQE_KEY_POUND_LONG:
        if(l_fSkyHangul)
            break;
        // Go through.. (Old Sky Hangul)
    case EQE_KEY_RIGHT_LONG:
        if(pTextEdit->pszFormat != NULL)
            break;

        pTextEdit->nPos = pTextEdit->nLen;

        sInitMode(pTextEdit);
        EQS_SendEvent(EQE_APP_DRAW, hControl, (DWORD)NULL);     
        break;

    case EQE_KEY_STAR_LONG:
        if(pTextEdit->pszFormat != NULL)
            break;

        if(l_fSkyHangul)
            break;
        // Go through.. (Old Sky Hangul)
    case EQE_KEY_LEFT_LONG:
        if(pTextEdit->pszFormat != NULL)
            break;

        pTextEdit->nPos = 0;

        sInitMode(pTextEdit);
        EQS_SendEvent(EQE_APP_DRAW, hControl, (DWORD)NULL);     
        break;

    case EQE_KEY_CLEAR_LONG:
        pTextEdit->nLen = 0;
        pTextEdit->nPos = 0;

        sInitMode(pTextEdit);

        if(pTextEdit->pszFormat != NULL)
        {
            FormatType = sGetCurrentFormat((UINT16)(pTextEdit->nPos));
            if(FormatType != TFT_Error)
                sSetDefaultFormatMode(FormatType);
        }

        EQS_SendEvent(EQE_APP_DRAW, hControl, (DWORD)NULL);     
        break;

    case EQE_TM_TIMER2:     // Editing Timer
        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY)
            break;

   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;
        if(GET_CURRENT_MODE(pTextEdit) != TEM_KOREAN)
        {
            sEndEditChar();
        }
#ifdef FEATURE_CURSOR_AUTOMOVE
        else if(l_fSkyHangul)
        {
            // 한글 모드일때는 현재 글자가 종성까지 입력되고, 종성이 복자음이거나
            // 복자음 조합이 안될때는 자동으로 다음글자 입력으로 넘어가도록 한다.
            if(pTextEdit->nKoreanState == 4)
            {
                sEndEditChar();
                pTextEdit->nKoreanState = 0;
            }

            if(pTextEdit->nKoreanState == 3)
            {
                UINT8 i;
	            for (i = 0; i < 11; i++)
                {
		            if (BokJaumTable[i][0] == uKorCode)
			            break;
                }

                if(i == 11)
                {
                    sEndEditChar();
                    pTextEdit->nKoreanState = 0;
                }
            }
        }
#endif
            //g_fEditChar = FALSE;
        pTextEdit->hAutoCursorTimer = NULL;

        EQS_SendEvent(EQE_APP_DRAW, hControl, (DWORD)NULL);     
        break;

    case EQE_TM_TIMER1:     // Cursor Blink Timer
        if(EQS_IsActive(hControl) && 
            !(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
        {
            // If Modal I/O Control exixts, do nothing
            if(EQS_GetModal() != INVALID_CONTROL_HANDLE)
                break;

/*
            if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL && l_fSkyHangul)
                break;
*/            
            EQS_GetDC(FALSE);

            if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) != TEM_SYMBOL)
            {
                sDrawCursor(hControl);
            }
            else
            {
                GetViewPort(&OldViewPort);

                EQS_GetSWindowViewAreaRect(&Rect);
                ViewPort.Rect = Rect;
                ViewPort.Rect.yBottom = EQS_GetMaxY();
                ViewPort.CoordSystem = CS_SCREEN;
                ViewPort.Origin.x = 0;
                ViewPort.Origin.y = 0;
                SetViewPort(&ViewPort);

                fOldSWindowState = GetSlidingWindowState();
                SetSlidingWindowState(FALSE);
                // Draw Cursor Blink
                SetRect(&Rect, 1, (COORD)(TE_SYMBOL_DY), (COORD)(GetMaxX()-1), GetMaxY());   
                EQS_SetViewPortRect(&Rect);

                sDrawCursor(hControl);

                SetSlidingWindowState(fOldSWindowState);
                SetViewPort(&OldViewPort);
            }
            EQS_ReleaseDC();
            EQS_UpdateControlDisplay();
        }
        break;

    case EQE_KEY_CENTER:
        if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
        {
   		    g_LastEvent = EQE_NONE;
		    g_nInputCount = 0;
            sEndEditChar();
            //g_fEditChar = FALSE;
/*
            if(STRCMP(pszSaveBuffer, pTextEdit->pszBuffer))
                EQC_SET_DIRTY(hControl, TRUE);
*/
            if(pszSaveBuffer != NULL)
            {
                EQS_Free((void**)&pszSaveBuffer);
                pszSaveBuffer = NULL;
            }
        }

        pTextEdit->nPos = 0;

        EQS_SendEvent(EQE_IO_TEDIT_DONE, (WORD)hControl, (DWORD)pTextEdit->nLen);     
        EQS_EndIO();
        break;

    case EQE_KEY_F1:
        if (EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_NO_LCML)
        {
   		    g_LastEvent = EQE_NONE;
		    g_nInputCount = 0;
            sEndEditChar();
            //g_fEditChar = FALSE;

            if(pszSaveBuffer != NULL)
            {
                EQS_Free((void**)&pszSaveBuffer);
                pszSaveBuffer = NULL;
            }

            EQS_SendEvent(EQE_IO_TEDIT_KEY_F1, (WORD)hControl, (DWORD)pTextEdit->nPos);  
            EQS_EndIO();
            break;
        }

        if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
        {
            sInitMode(pTextEdit);
            // Restore State 
            STRCPY(pTextEdit->pszBuffer, pszSaveBuffer);
            if(pszSaveBuffer != NULL)
            {
                EQS_Free((void**)&pszSaveBuffer);
                pszSaveBuffer = NULL;
            }
            pTextEdit->nPos = nSavePos;
            pTextEdit->nLen = nSaveLen;     

            if(!fDirty)
            {
                EQC_SET_DIRTY(hControl, FALSE);
            }
        }

        pTextEdit->nPos = 0;
        EQS_SendEvent(EQE_IO_TEDIT_CANCEL, (WORD)hControl, (DWORD)NULL);  
        
        EQS_EndIO();
        break;

    case EQE_IO_DELETE:
        // Send ending event you want..
		if( pAnEvent->wParam == hControl )
			EQS_SendEvent(EQE_IO_TEDIT_CANCEL, (WORD)hControl, (DWORD)EQC_DELETED);

        // Free instance
        sFreeInstance(hControl);

        break;

    default:
        break;
	}
}

/*
**---------------------------------------------------------------------------
**  WML Formatting 관련 함수
**---------------------------------------------------------------------------
*/
// 한글과 영문 모두 한글자씩으로 게산
LOCAL UINT16 sGetStringLength(UINT16 nLen)
{
    UINT16   nStringLen, i;

    nStringLen = 0;

    for(i=0; i < nLen; i++)
    {
        if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(i+1)))
            i++;
        nStringLen ++;
    }

    return nStringLen;
}

LOCAL void sSetDefaultFormatMode(TEXT_FORMAT_TYPE_E Type)
{
    if(Type == pTextEdit->FormatType)
        return;

    switch(Type){
    case TFT_A:
        pTextEdit->EditMode = TFT_AMode;
        break;
    case TFT_a:
        pTextEdit->EditMode = TFT_aMode;
        break;
    case TFT_N:
        pTextEdit->EditMode = TFT_NMode;
        break;
    case TFT_X:
        pTextEdit->EditMode = TFT_XMode;
        break;
    case TFT_x:
        pTextEdit->EditMode = TFT_xMode;
        break;
    case TFT_M:
        pTextEdit->EditMode = TFT_MMode;
        break;
    case TFT_m:
        pTextEdit->EditMode = TFT_mMode;
        break;
    }

    pTextEdit->FormatType = Type;
}


LOCAL TEXT_FORMAT_TYPE_E sGetCurrentFormat(UINT16 nLen)
{
    UINT16  nStringLen, i, nFormatPos, j;
    BYTE    bCurrent;
    UINT16  nFormatLen;

    if(pTextEdit->pszFormat == NULL)
        return TFT_Error;

    nStringLen = sGetStringLength(nLen);

//    if(g_fEditChar)
//        nStringLen --;

    bCurrent = 'X';
    nFormatPos = 0;
    for(i = 0; i < STRLEN(pTextEdit->pszFormat); i++)
    {
        if(pTextEdit->pszFormat[i] > '0' && pTextEdit->pszFormat[i] <= '9')
        {
            nFormatLen = 0;

            for(j=0; j < 5; j++)
            {
                if(j == 0)
                    nFormatLen += pTextEdit->pszFormat[i] - '0';
                else
                {
                    nFormatLen = nFormatLen * 10;
                    nFormatLen += pTextEdit->pszFormat[i] - '0';
                }

                i++;
                if(pTextEdit->pszFormat[i] < '0' || pTextEdit->pszFormat[i] > '9')
                    break;
            }

            nFormatPos += nFormatLen;
            bCurrent = pTextEdit->pszFormat[i];
        }
        else if(pTextEdit->pszFormat[i] == '\\')
        {
            i+= 2;
        }
        else if(pTextEdit->pszFormat[i] == '*')
        {
            bCurrent = pTextEdit->pszFormat[i+1];
            nFormatPos = nStringLen;
        }
        else
        {
            bCurrent = pTextEdit->pszFormat[i];
            nFormatPos ++;
        }

        if(nStringLen <= nFormatPos)
        {
            if(bCurrent == 'A')                return TFT_A;
            else if(bCurrent == 'a')           return TFT_a;
            else if(bCurrent == 'N')           return TFT_N;
            else if(bCurrent == 'X')           return TFT_X;
            else if(bCurrent == 'x')           return TFT_x;
            else if(bCurrent == 'M')           return TFT_M;
            else if(bCurrent == 'm')           return TFT_m;
        }
    }

    return TFT_Error;
}

LOCAL UINT16 sGetFormatLength(BYTE *pszFormat, UINT16 nLength, UINT8 nFormatLen)
{
    UINT16   nLen, i, j;

    if(nFormatLen > 0)
        return nFormatLen;

    nLen = 0;
    for(i = 0; i < nLength; i++)
    {
        if(pszFormat[i] > '0' && pszFormat[i] <= '9')
        {
            nFormatLen = 0;

            for(j=0; j < 5; j++)
            {
                if(j == 0)
                    nFormatLen += pszFormat[i] - '0';
                else
                {
                    nFormatLen = nFormatLen * 10;
                    nFormatLen += pszFormat[i] - '0';
                }
                i++;
                if(pszFormat[i] < '0' || pszFormat[i] > '9')
                    break;
            }

            nLen += nFormatLen;
        }
        else if(pszFormat[i] == '*')
        {
            return 0xFFFF;
        }
        else if(pszFormat[i] != '\\')
        {
            nLen++;
        }
    }

    return nLen;
}

LOCAL void sAddFormatChar(void)
{
    BYTE *szTmp;
    UINT16  nTmp, nLen, nOffset;

    nOffset = 0;
    while(TRUE)
    {
        szTmp = STRCHR((BYTE*)(pTextEdit->pszFormat + nOffset), '\\');
        if(szTmp == NULL)
            break;

        nTmp = szTmp - pTextEdit->pszFormat;
        nLen = sGetFormatLength(pTextEdit->pszFormat, nTmp, pTextEdit->nFormatLen);
        nOffset = nTmp+1;

        if(sGetStringLength(pTextEdit->nLen) == nLen)
        {
            memcpy(&pTextEdit->pszBuffer[pTextEdit->nPos], (BYTE*)(szTmp+1), 1);

            pTextEdit->nLen ++;
            pTextEdit->nPos ++;
            pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';

            if(pTextEdit->pszFormat != NULL)
            {
                FormatType = sGetCurrentFormat((UINT8)(pTextEdit->nPos));
                if(FormatType != TFT_Error)
                    sSetDefaultFormatMode(FormatType);
            }
            //break;
        }
    }
}


LOCAL void sDeleteFormatChar(HCONTROL hControl)
{
    BYTE *szTmp;
    UINT16  nTmp, nLen, nOffset;

    nOffset = 0;
    while(TRUE)
    {
        szTmp = STRCHR((BYTE*)(pTextEdit->pszFormat + nOffset), '\\');
        if(szTmp == NULL)
            break;

        nTmp = szTmp - pTextEdit->pszFormat;
        nLen = sGetFormatLength(pTextEdit->pszFormat, nTmp, pTextEdit->nFormatLen);
        nOffset = nTmp+1;

        if(sGetStringLength(pTextEdit->nLen) == nLen+1)
        {
            sDeleteStr(hControl);
            break;
        }
    }
}

/*
**---------------------------------------------------------------------------
**  위치 관련 함수들
**---------------------------------------------------------------------------
*/

LOCAL UINT16 sGetCurY(HCONTROL hControl)
{
    UINT16  nCurPos, nNumChars, nCurLine, nEndPos;
    BYTE *pTmp;

    nCurPos = 0;
    nCurLine = 0;

    nEndPos = pTextEdit->nPos;

    while(nCurPos < nEndPos)
    {
        nNumChars = g_nMaxCharPerLine;

        if(nCurPos+nNumChars >= nEndPos)
        {
            nNumChars = nEndPos - nCurPos;
        }
        else
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(nCurPos+nNumChars))) // two-byte code
                nNumChars --;
        }

        if(pTmp = STRCHR(&pTextEdit->pszBuffer[nCurPos], '\n'))
        {
            if(pTmp < &pTextEdit->pszBuffer[nCurPos+nNumChars])
                nNumChars = pTmp - (pTextEdit->pszBuffer + nCurPos) +1;    // include '\n'
        }

        nCurPos+= nNumChars;
        nCurLine++;
    }

    if(nCurLine == 0)   
        return 0;
    else     
        return (nCurLine - 1);
}

LOCAL UINT16 sGetY(HCONTROL hControl)
{
    UINT16  nCurPos, nNumChars, nCurLine;
    BYTE * pTmp;

    nCurPos = 0;
    nCurLine = 0;

    while(nCurPos < pTextEdit->nLen)
    {
        nNumChars = g_nMaxCharPerLine;
        if(nCurPos+nNumChars >= pTextEdit->nLen)
        {
            nNumChars = pTextEdit->nLen - nCurPos;
        }
        else
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(nCurPos+nNumChars))) // two-byte code
                nNumChars --;
        }

        if(pTmp = STRCHR(&pTextEdit->pszBuffer[nCurPos], '\n'))
        {
            if(pTmp < &pTextEdit->pszBuffer[nCurPos+nNumChars])
                nNumChars = pTmp - (pTextEdit->pszBuffer + nCurPos) +1;    // include '\n'
        }
        nCurPos+= nNumChars;
        nCurLine++;
    }

    if(nCurLine == 0)   
        return 0;
    else     
        return (nCurLine - 1);
}

LOCAL UINT16 sGetCurX(void)
{
    UINT16  nCurPos, nNumChars, nCurLine;
    BYTE * pTmp;

    nCurPos = 0;
    nCurLine = 0;
    nNumChars = 0;


    if(pTextEdit->nPos == 0)
        return 0;

    while(nCurPos < pTextEdit->nPos)
    {
        nNumChars = g_nMaxCharPerLine;
        if(nCurPos+nNumChars >= pTextEdit->nPos)
        {
            nNumChars = pTextEdit->nPos - nCurPos;
        }
        else
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(nCurPos+nNumChars))) // two-byte code
                nNumChars --;
        }

        if(pTmp = STRCHR(&pTextEdit->pszBuffer[nCurPos], '\n'))
        {
            if(pTmp < &pTextEdit->pszBuffer[nCurPos+nNumChars])
                nNumChars = pTmp - (pTextEdit->pszBuffer + nCurPos) +1;    // include '\n'
        }
        nCurPos+= nNumChars;
        nCurLine++;
    }

    return nNumChars;
}

LOCAL void sSetCursorPos(HCONTROL hControl)
{
    UINT16  nDispLines, nCurLine;
    INT16   nSLine;

    if(l_fSkyHangul)
        nDispLines = pTextEdit->nLine;
    else
        nDispLines = (EQS_GetMaxHeight() - TE_SOFTKEY_Y) / g_FontSize.yHeight;

    nCurLine = sGetCurY(hControl);

    //if(!g_fEditChar && pTextEdit->nLen < pTextEdit->nMaxLen)
    if(!g_fEditChar)
    {
        if(pTextEdit->nPos == pTextEdit->nLen && sGetCurX() == g_nMaxCharPerLine 
            && pTextEdit->nLen < pTextEdit->nMaxLen)
            nCurLine ++;
        else if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos+1)) &&
            sGetCurX() >= g_nMaxCharPerLine-1 && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine++;
        else if(sGetCurX() == g_nMaxCharPerLine && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine ++;
        else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
            nCurLine ++;
    }

    nSLine = nCurLine - nDispLines + 1;
    if(nSLine < 0)  nSLine = 0;

    if(g_fEditChar)
    {
        switch(GET_CURRENT_MODE(pTextEdit))
        {
        case TEM_ENGLISH_UPPER :
        case TEM_ENGLISH_LOWER :
            pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()-1));
            pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));

            pTextEdit->pCursor->nLen = 1;
            if(pTextEdit->fPassword && !g_fEditChar)
                pTextEdit->pCursor->pszStr[0] = '*';
            else
                pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos-1];

            pTextEdit->pCursor->pszStr[1] = '\0';
            pTextEdit->pCursor->pszStr[2] = ' ';
            break;

        case TEM_KOREAN :
            pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()-2));
            pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));

            pTextEdit->pCursor->nLen = 2;
            pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos-2];
            pTextEdit->pCursor->pszStr[1] = pTextEdit->pszBuffer[pTextEdit->nPos-1];
            pTextEdit->pCursor->pszStr[2] = '\0';
            break;

        case TEM_SYMBOL :
        case TEM_NUMBER :
        default:
            if(sGetCurX() == g_nMaxCharPerLine)
            {
                pTextEdit->pCursor->x = 0;
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
            }
            else
            {
                pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * sGetCurX());
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
            }

            pTextEdit->pCursor->nLen = 1;
            pTextEdit->pCursor->pszStr[0] = ' ';
            pTextEdit->pCursor->pszStr[1] = '\0';
            pTextEdit->pCursor->pszStr[2] = ' ';

            break;
        }
    }
    else
    {  // !g_fEditChar

        if(pTextEdit->nPos == pTextEdit->nLen && pTextEdit->nLen < pTextEdit->nMaxLen)
        {
            pTextEdit->pCursor->nLen = 1;
            pTextEdit->pCursor->pszStr[0] = ' ';
            pTextEdit->pCursor->pszStr[1] = '\0';
            pTextEdit->pCursor->pszStr[2] = ' ';

            pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()));
            pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));

            if(sGetCurX() == g_nMaxCharPerLine)
            {
                pTextEdit->pCursor->x = 0;
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
            }

            if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
            {
                pTextEdit->pCursor->x = 0;
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
            }
        }
        else if(pTextEdit->nPos == pTextEdit->nLen && pTextEdit->nLen >= pTextEdit->nMaxLen)
        {
/*
            pTextEdit->pCursor->nLen = 0;
            pTextEdit->pCursor->pszStr[0] = ' ';
            pTextEdit->pCursor->pszStr[1] = '\0';
*/
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos-1)))
            {
                pTextEdit->pCursor->nLen = 2;
                pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos-2];
                pTextEdit->pCursor->pszStr[1] = pTextEdit->pszBuffer[pTextEdit->nPos-1];
                pTextEdit->pCursor->pszStr[2] = '\0';

                pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()-2));
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
            }
            else
            {
                pTextEdit->pCursor->nLen = 1;
                if(pTextEdit->fPassword)
                    pTextEdit->pCursor->pszStr[0] = '*';
                else
                    pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos-1];
                pTextEdit->pCursor->pszStr[1] = '\0';
                pTextEdit->pCursor->pszStr[2] = ' ';

                pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()-1));
                pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
            }

        }
        else //if(pTextEdit->nPos > 0)
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos+1)))
            {
                pTextEdit->pCursor->nLen = 2;
                pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos];
                pTextEdit->pCursor->pszStr[1] = pTextEdit->pszBuffer[pTextEdit->nPos+1];
                pTextEdit->pCursor->pszStr[2] = '\0';

                if(sGetCurX() >= g_nMaxCharPerLine-1)
                {
                    pTextEdit->pCursor->x = 0;
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
                }
                else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
                {
                    pTextEdit->pCursor->x = 0;
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
                }
                else
                {
                    pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()));
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
                }
            }
            else
            {
                pTextEdit->pCursor->nLen = 1;
                if(pTextEdit->fPassword)
                    pTextEdit->pCursor->pszStr[0] = '*';
                else
                    pTextEdit->pCursor->pszStr[0] = pTextEdit->pszBuffer[pTextEdit->nPos];
                pTextEdit->pCursor->pszStr[1] = '\0';
                pTextEdit->pCursor->pszStr[2] = ' ';

                if(sGetCurX() == g_nMaxCharPerLine)
                {
                    pTextEdit->pCursor->x = 0;
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
                }
                else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
                {
                    pTextEdit->pCursor->x = 0;
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (nCurLine-nSLine));
                }
                else
                {
                    pTextEdit->pCursor->x = (COORD)(g_FontSize.xWidth * (sGetCurX()));
                    pTextEdit->pCursor->y = (COORD)(g_FontSize.yHeight * (sGetCurY(hControl)-nSLine));
                }

//                if(sGetCurX() >= pTextEdit->nChar)
//                    pTextEdit->pCursor->nLen = 0;
            }
        }
    }

    if(pTextEdit->pCursor->pszStr[0] == '\r' || pTextEdit->pCursor->pszStr[0] == '\n')
        pTextEdit->pCursor->pszStr[0] = ' ';

}

/*
**---------------------------------------------------------------------------
**  Text Edit Draw Functions
**---------------------------------------------------------------------------
*/

LOCAL void sDrawCursor(HCONTROL hControl)
{
    RECT Rect;
    BOOL fOldSWindowState = FALSE;
    VIEW_PORT OldViewPort, ViewPort;
    COLOR OldTextBackColor, OldTextColor;

    if(l_fSkyHangul)
    {
        fOldSWindowState = GetSlidingWindowState();
        GetViewPort(&OldViewPort);

        if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL && EQS_IsActive(hControl))
        {
            SetRect(&Rect, sTEXT_BOX_IN_SYMBOL_LEFT, sTEXT_BOX_IN_SYMBOL_TOP, 
                           sTEXT_BOX_IN_SYMBOL_RIGHT, sTEXT_BOX_IN_SYMBOL_BOTTOM);
        }
        else
        {
            SetSlidingWindowState(TRUE);

            EQS_GetSWindowViewAreaRect(&Rect);
            ViewPort.Rect = Rect;
            ViewPort.CoordSystem = CS_SCREEN;
            ViewPort.Origin.x = 0;
            ViewPort.Origin.y = 0;
            SetViewPort(&ViewPort);

            SetRect(&Rect, EQC_GET_X(hControl), EQC_GET_Y(hControl), 
                    (COORD)((EQC_GET_X(hControl) + EQC_GET_DX(hControl)) - 1),
                    (COORD)((EQC_GET_Y(hControl) + EQC_GET_DY(hControl)) - 1));
        }

        EQS_SetViewPortRect(&Rect);
    }

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));

    if(pTextEdit->pCursor->fReverse)
    {
        OldTextBackColor = EQS_GetTextBackColor();
        OldTextColor = EQS_GetTextColor();
#ifdef FEATURE_COLOR_CURSOR
        if(g_LastEvent == EQE_NONE)
            EQS_SetTextBackColor(BLUE);
        else
        {
            if(GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_UPPER || 
                GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_LOWER ||
                (GET_CURRENT_MODE(pTextEdit) == TEM_KOREAN && l_fSkyHangul))
                EQS_SetTextBackColor(196);
            else
                EQS_SetTextBackColor(BLUE);
        }
#else
        EQS_SetTextBackColor(BLUE);
#endif
        EQS_SetTextColor(WHITE);
        EQS_TextOutStrLenTDM(pTextEdit->pCursor->x, pTextEdit->pCursor->y,
                (BYTE*)pTextEdit->pCursor->pszStr, 
                (UINT16)pTextEdit->pCursor->nLen,
                TDM_NORMAL);
        EQS_SetTextBackColor(OldTextBackColor);
        EQS_SetTextColor(OldTextColor);
    }
    else
    {
        EQS_TextOutStrLenTDM(pTextEdit->pCursor->x, pTextEdit->pCursor->y,
                (BYTE*)pTextEdit->pCursor->pszStr, 
                (UINT16)pTextEdit->pCursor->nLen,
                TDM_NORMAL);
    }

    pTextEdit->pCursor->fReverse = pTextEdit->pCursor->fReverse?0:1;

    if(l_fSkyHangul)
    {
        SetSlidingWindowState(fOldSWindowState);
        SetViewPort(&OldViewPort);
    }
}

LOCAL void sDrawSoftKey(HCONTROL hControl)
{
    if (EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_NO_CML)
        return;

    if (!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_NO_LCML))
        EQS_DrawIOCCML(IOC_CML_CANCEL_LEFT);

    if (!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_NO_CCML))
        EQS_DrawIOCCML(IOC_CML_DONE_CENTER);

    switch(GET_CURRENT_MODE(pTextEdit))
    {
        case TEM_ENGLISH_UPPER:
            EQS_DrawIOCCML(IOC_CML_CAPITAL_ENGLISH_RIGHT);
            break;

        case TEM_ENGLISH_LOWER:
            EQS_DrawIOCCML(IOC_CML_ENGLISH_RIGHT);
            break;

        case TEM_KOREAN:
            EQS_DrawIOCCML(IOC_CML_KOREAN_RIGHT);
            break;

        case TEM_SYMBOL:
            EQS_DrawIOCCML(IOC_CML_SYMBOL_RIGHT);
            break;

        case TEM_NUMBER:
            EQS_DrawIOCCML(IOC_CML_NUMBER_RIGHT);
            break;

        default:
            EQS_DrawIOCCML(IOC_CML_BLANK_RIGHT);
            break;
    }
}



LOCAL void sDrawScrollBar(HCONTROL hControl)
{
	INT8  nDispLines, nMaxLines;

    if(l_fSkyHangul)
        nDispLines = pTextEdit->nLine;
    else
        nDispLines = (EQS_GetMaxHeight() - TE_SOFTKEY_Y) / g_FontSize.yHeight;

	if( pTextEdit->nMaxLen % g_nMaxCharPerLine )
	{
		nMaxLines = pTextEdit->nMaxLen / g_nMaxCharPerLine + 1;
	}
	else
	{
		nMaxLines = pTextEdit->nMaxLen / g_nMaxCharPerLine;
	}

	if( nMaxLines < nDispLines  )
	{
		nMaxLines = nDispLines;
	}

    if(pTextEdit->pScrollBar == NULL)
    {
        pTextEdit->pScrollBar = (SCROLL_BAR*)EQS_Malloc(sizeof(SCROLL_BAR));

        // Add Scroll Bar
#if 0
        EQS_AddScrollBar(NULL, pTextEdit->pScrollBar, SBT_VERT, 
                        (COORD)(EQS_GetMaxWidth()-6), 0, 
                        (INT8)(EQS_GetMaxHeight() - TE_SOFTKEY_Y), 0, 
                        (INT16)((sGetY(hControl)+1) * g_FontSize.yHeight-g_FontSize.yHeight-1), 
                        (INT16)g_FontSize.yHeight, 
                        (INT16)(sGetCurY(hControl) * g_FontSize.yHeight));
#else

        EQS_AddScrollBar(NULL, pTextEdit->pScrollBar, SBT_VERT, 
                        (COORD)(EQS_GetMaxWidth()-6), 0, 
                        (INT8)(EQS_GetMaxHeight() - TE_SOFTKEY_Y), 0, 
                        nMaxLines-nDispLines,
                        nDispLines,
						0
                        );

#endif

        EQS_DrawScrollBar(pTextEdit->pScrollBar);

    }
    else
    {
        SCROLL_BAR_INFO ScrlBarInfo;

        ScrlBarInfo.nMax = nMaxLines - nDispLines;
        ScrlBarInfo.nMin = 0;
        ScrlBarInfo.nPageSize = nDispLines;
		if( sGetCurY(hControl) >= nDispLines )
		{		
			ScrlBarInfo.nPos = sGetCurY(hControl) - nDispLines + 1;
		}
		else
		{
			ScrlBarInfo.nPos = 0;
		}
        EQS_SetScrollBarInfo(pTextEdit->pScrollBar, &ScrlBarInfo, TRUE);
    }
}

LOCAL void sDrawBrowseScrollBar(HCONTROL hControl)
{
    UINT16 nMaxCharPerLine;
	INT8  nDispLines, nMaxLines;

    nMaxCharPerLine = g_nMaxCharPerLine;

    if(l_fSkyHangul)
        g_nMaxCharPerLine = pTextEdit->nChar;
    else
        g_nMaxCharPerLine = EQC_GET_DX(hControl) / g_FontSize.xWidth;


    if(l_fSkyHangul)
        nDispLines = pTextEdit->nLine;
    else
        nDispLines = (EQS_GetMaxHeight() - TE_SOFTKEY_Y) / g_FontSize.yHeight;


	if( pTextEdit->nMaxLen % nMaxCharPerLine )
	{
		nMaxLines = pTextEdit->nMaxLen / nMaxCharPerLine + 1;
	}
	else
	{
		nMaxLines = pTextEdit->nMaxLen / nMaxCharPerLine;
	}

	if( nMaxLines < nDispLines  )
	{
		nMaxLines = nDispLines;
	}


    if(pTextEdit->pBrowseScrollBar == NULL)
    {
        pTextEdit->pBrowseScrollBar = (SCROLL_BAR*)EQS_Malloc(sizeof(SCROLL_BAR));

        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_BORDER)
        {
            // Add Scroll Bar
            EQS_AddScrollBar(NULL, pTextEdit->pBrowseScrollBar, SBT_VERT, 
                            EQC_GET_DX(hControl), 0,
                            (INT8)(EQC_GET_DY(hControl)+3), 0, 
                            nMaxLines-nDispLines, 
                            nDispLines, 
                            0);
        }
        else
        {
            // Add Scroll Bar
            EQS_AddScrollBar(NULL, pTextEdit->pBrowseScrollBar, SBT_VERT, 
                            EQC_GET_DX(hControl), 0,
                            (INT8)EQC_GET_DY(hControl), 0, 
                            nMaxLines-nDispLines, 
                            nDispLines, 
                            0);

        }
        
        EQS_DrawScrollBar(pTextEdit->pBrowseScrollBar);

    }
    else
    {
        SCROLL_BAR_INFO ScrlBarInfo;

        ScrlBarInfo.nMax = nMaxLines-nDispLines;
        ScrlBarInfo.nMin = 0;
        ScrlBarInfo.nPageSize = nDispLines;
		if( sGetCurY(hControl) >= nDispLines )
		{		
			ScrlBarInfo.nPos = sGetCurY(hControl) - nDispLines+1;
		}
		else
		{
			ScrlBarInfo.nPos = 0;
		}        

        EQS_SetScrollBarInfo(pTextEdit->pBrowseScrollBar, &ScrlBarInfo, TRUE);
    }
    
    if(l_fSkyHangul)
        g_nMaxCharPerLine = pTextEdit->nChar;
    else
        g_nMaxCharPerLine = nMaxCharPerLine;
}

LOCAL void sDrawTextBuf(HCONTROL hControl)
{
    UINT16       nCurLine, nCurPos, nNumChars;
    INT16        nSLine, nDispLines;
    BYTE        szTmpBuf[TE_MAX_LINE_WIDTH];
    BYTE * pTmp;

	
    EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY()-TE_SOFTKEY_Y, EQS_GetTextBackColor());

    nDispLines = (EQS_GetMaxHeight() - TE_SOFTKEY_Y) / g_FontSize.yHeight;

    nCurLine = sGetCurY(hControl);

//    if(!g_fEditChar && sGetCurX() == g_nMaxCharPerLine)
//        nCurLine ++;
    //if(!g_fEditChar && pTextEdit->nLen < pTextEdit->nMaxLen)
    if(!g_fEditChar)
    {
        if(pTextEdit->nPos == pTextEdit->nLen && sGetCurX() == g_nMaxCharPerLine
            && pTextEdit->nLen < pTextEdit->nMaxLen)
            nCurLine ++;
        else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
            nCurLine ++;
        else if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos+1)) &&
            sGetCurX() >= g_nMaxCharPerLine-1
            && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine++;
        else if(sGetCurX() == g_nMaxCharPerLine && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine ++;
    }

    nSLine = nCurLine - nDispLines + 1;
    if(nSLine < 0)  nSLine = 0;

    nCurLine = 0;
    nCurPos = 0;
    nNumChars = 0;

    while(nCurPos < pTextEdit->nLen)
    {
        BOOL    fNewLine;

        if(nDispLines <= 0)   break;

        nNumChars = g_nMaxCharPerLine;
        if(nCurPos+nNumChars >= pTextEdit->nLen)
        {
            nNumChars = pTextEdit->nLen - nCurPos;
        }
        else
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(nCurPos+nNumChars))) // two-byte code
                nNumChars --;
        }
        fNewLine = FALSE;

        if(pTmp = STRCHR(&pTextEdit->pszBuffer[nCurPos], '\n'))
        {
            if(pTmp < &pTextEdit->pszBuffer[nCurPos+nNumChars])
            {
                nNumChars = pTmp - (pTextEdit->pszBuffer + nCurPos);    // include '\n'
                fNewLine = TRUE;
            }

            if(nNumChars == 0)
            {
                nCurPos += 1;
                nCurLine++;
                if(nCurLine >= nSLine)
                    nDispLines --;
                continue;
            }
        }

        if(nCurLine >= nSLine)
        {
            memset(szTmpBuf, ' ', TE_MAX_LINE_WIDTH);
            if(pTextEdit->fPassword)
                memset(szTmpBuf, '*', nNumChars);
            else
                memcpy(szTmpBuf, &pTextEdit->pszBuffer[nCurPos], nNumChars);

            if(nNumChars < g_nMaxCharPerLine)
                memset(&szTmpBuf[nNumChars], ' ', (UINT16)(g_nMaxCharPerLine - nNumChars));


            EQS_TextOutStrLen(	0, (COORD)(g_FontSize.yHeight * (nCurLine-nSLine)),
                (BYTE*)szTmpBuf, 
                g_nMaxCharPerLine);

            nDispLines --;
        }

        nCurPos+= nNumChars;
        if(fNewLine)
            nCurPos += 1;
        nCurLine++;
    }
}

LOCAL void sDrawTextWindow(HCONTROL hControl)
{
    UINT16       nCurLine, nCurPos, nNumChars;
    INT16        nSLine, nDispLines;
    BYTE         szTmpBuf[TE_MAX_LINE_WIDTH+1];
    RECT Rect;
    COLOR       FontColor;
    BYTE *pTmp;

    FontColor = EQS_GetTextColor();
    
    //EQS_SetTextColor(EQC_GET_TEXT_COLOR(hControl));
    EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));

    if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL && EQS_IsActive(hControl))
    {
        SetRect(&Rect, sTEXT_BOX_IN_SYMBOL_LEFT, sTEXT_BOX_IN_SYMBOL_TOP, 
                       sTEXT_BOX_IN_SYMBOL_RIGHT, sTEXT_BOX_IN_SYMBOL_BOTTOM);
        EQS_SetViewPortRect(&Rect);
    }
    else
    {
        EQS_Rectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY());

        SetRect(&Rect, EQC_GET_X(hControl), EQC_GET_Y(hControl), 
                (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1),
                (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1));

        EQS_SetViewPortRect(&Rect);
   
        if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_ORING))
            EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), EQS_GetTextBackColor());
    }

    if(l_fSkyHangul)
    {
        nDispLines = pTextEdit->nLine;
        g_nMaxCharPerLine = pTextEdit->nChar;
    }
    else
    {
        nDispLines = (EQS_GetMaxHeight()+1) / g_FontSize.yHeight;
        g_nMaxCharPerLine = (EQS_GetMaxWidth()) / g_FontSize.xWidth;
    }

    nCurLine = sGetCurY(hControl);

//    if(!g_fEditChar && sGetCurX() == g_nMaxCharPerLine)
//        nCurLine ++;
    //if(!g_fEditChar && pTextEdit->nLen < pTextEdit->nMaxLen)
    if(!g_fEditChar)
    {
        if(pTextEdit->nPos == pTextEdit->nLen && sGetCurX() == g_nMaxCharPerLine
            && pTextEdit->nLen < pTextEdit->nMaxLen)
            nCurLine ++;
        else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
            nCurLine ++;
        else if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos+1)) &&
            sGetCurX() >= g_nMaxCharPerLine-1
            && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine++;
        else if(sGetCurX() == g_nMaxCharPerLine && pTextEdit->nPos < pTextEdit->nMaxLen)
            nCurLine ++;
    }

    nSLine = nCurLine - nDispLines + 1;
    if(nSLine < 0)  nSLine = 0;

    nCurLine = 0;
    nCurPos = 0;
    nNumChars = 0;

    while(nCurPos < pTextEdit->nLen)
    {
        BOOL    fNewLine;

        if(nDispLines <= 0)   break;

        nNumChars = g_nMaxCharPerLine;
        if(nCurPos+nNumChars >= pTextEdit->nLen)
        {
            nNumChars = pTextEdit->nLen - nCurPos;
        }
        else
        {
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(nCurPos+nNumChars))) // two-byte code
                nNumChars --;
        }
        fNewLine = FALSE;

        if(pTmp = STRCHR(&pTextEdit->pszBuffer[nCurPos], '\n'))
        {
            if(pTmp < &pTextEdit->pszBuffer[nCurPos+nNumChars])
            {
                nNumChars = pTmp - (pTextEdit->pszBuffer + nCurPos);    // include '\n'
                fNewLine = TRUE;

                if(nNumChars == 0)
                {
                    nCurPos += 1;
                    nCurLine++;

                    if(nCurLine >= nSLine)
                        nDispLines --;
                    continue;
                }
            }
        }

        if(nCurLine >= nSLine)
        {
            DRAW_MODE OldDrawMode;

            memset(szTmpBuf, ' ', TE_MAX_LINE_WIDTH);
            if(pTextEdit->fPassword)
                memset(szTmpBuf, '*', nNumChars);
            else
                memcpy(szTmpBuf, &pTextEdit->pszBuffer[nCurPos], nNumChars);
            if(nNumChars < g_nMaxCharPerLine)
                memset(&szTmpBuf[nNumChars], ' ', (UINT16)(g_nMaxCharPerLine - nNumChars));

            OldDrawMode = EQS_GetDrawMode();

            if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_ORING)
                EQS_SetDrawMode(DM_OR);

            EQS_TextOutStrLen(	0, (COORD)(g_FontSize.yHeight * (nCurLine-nSLine)),
                (BYTE*)szTmpBuf, 
                g_nMaxCharPerLine);
            EQS_SetDrawMode(OldDrawMode);

            nDispLines --;
        }        

        nCurPos+= nNumChars;

        if(fNewLine)
            nCurPos += 1;

        nCurLine++;
    }

    EQS_SetTextColor(FontColor);
}

LOCAL void sDrawDisplayTextEdit(HCONTROL hControl)
{
    RECT Rect;
    BOOL fOldSWindowState;
    VIEW_PORT OldViewPort, ViewPort;

    fOldSWindowState = GetSlidingWindowState();

    GetViewPort(&OldViewPort);

    // Display Mode
    EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));

    if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL && EQS_IsActive(hControl))
    {
        //EQS_Draw3DBox(sTEXT_BOX_IN_SYMBOL_LEFT-1, sTEXT_BOX_IN_SYMBOL_TOP-1, 
                       //sTEXT_BOX_IN_SYMBOL_RIGHT+1, sTEXT_BOX_IN_SYMBOL_BOTTOM+1);

        SetRect(&Rect, sTEXT_BOX_IN_SYMBOL_LEFT, sTEXT_BOX_IN_SYMBOL_TOP, 
                       sTEXT_BOX_IN_SYMBOL_RIGHT, sTEXT_BOX_IN_SYMBOL_BOTTOM);


        EQS_SetViewPortRect(&Rect);
		EQS_FillRectangle(0, 0, EQS_GetMaxX(), EQS_GetMaxY(), EQS_GetTextBackColor());
        sDrawTextWindow(hControl);
    }
    else
    {
        SetSlidingWindowState(TRUE);

        EQS_GetSWindowViewAreaRect(&Rect);
        ViewPort.Rect = Rect;
        ViewPort.CoordSystem = CS_SCREEN;
        ViewPort.Origin.x = 0;
        ViewPort.Origin.y = 0;
        SetViewPort(&ViewPort);

        SetRect(&Rect, EQC_GET_X(hControl), EQC_GET_Y(hControl), 
                (COORD)((EQC_GET_X(hControl) + EQC_GET_DX(hControl)) - 1),
                (COORD)((EQC_GET_Y(hControl) + EQC_GET_DY(hControl)) - 1));

        EQS_SetViewPortRect(&Rect);

        sDrawTextWindow(hControl);

        // Draw Border
        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_BORDER)
        {
            EQC_DrawBorder(hControl, 0, 0, EQS_GetMaxX(), EQS_GetMaxY());

            SetRect(&Rect, EQC_GET_X(hControl), (COORD)(EQC_GET_Y(hControl)), 
                    (COORD)(EQC_GET_X(hControl) + EQC_GET_DX(hControl)-1),
                    (COORD)(EQC_GET_Y(hControl) + EQC_GET_DY(hControl)-1));

            EQS_SetViewPortRect(&Rect);
        }

        // Draw Scroll Bar
        if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_VSCROLL)
            sDrawBrowseScrollBar(hControl);
    }
    
    SetSlidingWindowState(fOldSWindowState);
    SetViewPort(&OldViewPort);

    if(l_fSkyHangul && EQS_IsActive(hControl) && 
        !(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
    {
        sDrawStatusLine(pTextEdit);
    }
}

LOCAL void sDrawStatusLine(TEXT_EDIT_T * pTextEdit)
{
    COLOR FontColor;
    BYTE  szBuffer[7];
    DRAW_MODE saveDrawMode;

/*
    sprintf((char*)szBuffer, "%3d/%3d", pTextEdit->nLen, pTextEdit->nMaxLen);

    FontColor = GetTextColor();
    saveDrawMode = GetDrawMode();

    SetTextColor(RED);
    SetDrawMode(DM_OR);

    PutBitmap(36, (COORD)(GetMaxHeight() - TE_SOFTKEY_Y), &g_BmpCntframe48x13x256c);
    TextOutStrLen(	37, (COORD)(GetMaxHeight() - TE_SOFTKEY_Y +1),
        szBuffer, 7);

    SetDrawMode(saveDrawMode);
    SetTextColor(FontColor);
*/
}

LOCAL void sDrawTextEdit(HCONTROL hControl)
{
    COORD x;
    BITMAP Bitmap;
    RECT Rect;


    EQS_GetDC(TRUE);

  	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    EQS_SetFontType(EQC_GET_FONT_TYPE(hControl));
    //EQS_SetTextColor(EQC_GET_TEXT_COLOR(hControl));

    // Get Edit Font Size
    g_FontSize.xWidth = EQS_CharWidth('A');
    g_FontSize.yHeight = EQS_CharHeight('A');

    // Edit Mode
    if(EQS_IsActive(hControl) && 
        !(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
    {  
/*
        if(!l_fSkyHangul && l_fDarken == FALSE)
        {
            MakeScreenDarker(1);
            l_fDarken = TRUE;
        }
*/
        if(pTextEdit->hCursorTimer == NULL)
        {
            // Start Blink cursor timer
            pTextEdit->hCursorTimer = EQS_StartControlTimer(hControl, 
                            EQS_TIMER_1, TE_BLINK_CURSOR_TIME, TRUE);
        }

		if( !l_fSkyHangul )
		{
			EQS_ResetViewPort();
			EQS_FillRectangle(0,0,10,TE_SYMBOL_DY-1, EQS_GetBackColor());
			EQS_FillRectangle(GetMaxX()-10,0,GetMaxX(),TE_SYMBOL_DY-1, EQS_GetBackColor());
		}			
	
        if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_KOREAN)  // 한글입력창
        {
            EQS_ResetViewPort();

            x = (GetMaxX() - TE_SYMBOL_DX)/2;
            if(x < 0)   x = 0;

            Bitmap = g_BmpSkyHangulTable;
            Bitmap.pbData = g_BmpSkyHangulTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * l_KoreanMode;
            EQS_PutBitmap(x, 0, &Bitmap);		
			
        }
        else if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_UPPER)  // 영문대문자
        {
            EQS_ResetViewPort();

            x = (GetMaxX() - TE_SYMBOL_DX)/2;
            if(x < 0)   x = 0;

            Bitmap = g_BmpSkyEnglishNumberTable;
            Bitmap.pbData = g_BmpSkyEnglishNumberTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 0;
            EQS_PutBitmap(x, 0, &Bitmap);
			
        }
        else if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_LOWER)  // 영문소문자
        {
            EQS_ResetViewPort();

            x = (GetMaxX() - TE_SYMBOL_DX)/2;
            if(x < 0)   x = 0;

            Bitmap = g_BmpSkyEnglishNumberTable;
            Bitmap.pbData = g_BmpSkyEnglishNumberTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 1;
            EQS_PutBitmap(x, 0, &Bitmap);
        }
        else if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_NUMBER)  // 숫자
        {
            EQS_ResetViewPort();

            x = (GetMaxX() - TE_SYMBOL_DX)/2;
            if(x < 0)   x = 0;

            Bitmap = g_BmpSkyEnglishNumberTable;
            Bitmap.pbData = g_BmpSkyEnglishNumberTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header) * 2;
            EQS_PutBitmap(x, 0, &Bitmap);
        }
        else if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            BYTE szTmp[18];

            // 기호 입력 창
            if(l_fSkyHangul)
            {
                COORD y;
                DRAW_MODE saveDrawMode;
                COLOR OldTextColor, OldTextBackColor;
                BITMAP Bitmap;

/*
                if(l_fDarken == FALSE)
                {
                    MakeScreenDarker(1);
                    l_fDarken = TRUE;
                }
*/
                y = (GetMaxY() - TE_SYMBOL_DY)/2;

			
				EQS_FillRectangle(0, 0, EQS_GetMaxX(),\
					              EQS_GetMaxY()-EQS_SWINDOW_HEADER_HEIGHT-DEF_CML_HEIGHT,\
								  EQS_GetBackColor());
			

                Bitmap = g_BmpSymbolTable;
                Bitmap.pbData = g_BmpSymbolTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)*(pTextEdit->nSymbolPage);
                EQS_PutBitmap(4, 23, &Bitmap);

                saveDrawMode = EQS_GetDrawMode();
                OldTextColor = EQS_GetTextColor();
				OldTextBackColor = EQS_GetTextBackColor();

                EQS_SetTextColor(EQS_GetForeColor());
				EQS_SetTextBackColor(EQS_GetBackColor());
                EQS_SetDrawMode(DM_OR);

                if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_USE_SKFONT)
                    sprintf((char*)szTmp, (char*)"기호입력 (%d/%d)", pTextEdit->nSymbolPage +1, SK_SYMBOL_PAGE_NUMBER);
                else 
                    sprintf((char*)szTmp, (char *)"기호입력 (%d/%d)", pTextEdit->nSymbolPage +1, SYMBOL_PAGE_NUMBER);

                EQS_TextOutStr(	3, 4, szTmp); 
                EQS_SetTextColor(OldTextColor);
				EQS_SetTextBackColor(OldTextBackColor);
                EQS_SetDrawMode(saveDrawMode);

            }
            else
            {
                COORD x;
                BITMAP Bitmap;

                EQS_ResetViewPort();

                x = (GetMaxX() - TE_SYMBOL_DX)/2;
                if(x < 0)   x = 0;

                Bitmap = g_BmpSymbolTable;
                Bitmap.pbData = g_BmpSymbolTable.pbData + EQC_GET_FRAME_SIZE(Bitmap.Header)* (pTextEdit->nSymbolPage);
                EQS_PutBitmap(x, 0, &Bitmap);
            }
        }

        if(l_fSkyHangul)
        {
            if(GET_CURRENT_MODE(pTextEdit) != TEM_SYMBOL)
            {
                g_nMaxCharPerLine = pTextEdit->nChar;
                sDrawDisplayTextEdit(hControl);
                sDrawSoftKey(hControl);
                sDrawStatusLine(pTextEdit);
                sSetCursorPos(hControl);
                sDrawCursor(hControl);
            }
            else
            {
                UINT8 nSaveChar, nSaveLine;

                nSaveChar = pTextEdit->nChar;
                nSaveLine = pTextEdit->nLine;

                pTextEdit->nChar = TE_MAX_LINE_WIDTH-1;
                pTextEdit->nLine = 1;

                g_nMaxCharPerLine = pTextEdit->nChar;
                sDrawDisplayTextEdit(hControl);
                sDrawSoftKey(hControl);
                sDrawStatusLine(pTextEdit);
                sSetCursorPos(hControl);
                sDrawCursor(hControl);

                pTextEdit->nChar = nSaveChar;
                pTextEdit->nLine = nSaveLine;
            }
        }
        else 
        {

            EQS_ResetViewPort();
            SetRect(&Rect, 1, (COORD)(TE_SYMBOL_DY), 
                            (COORD)(GetMaxX()-1), (COORD)(GetMaxY()));   
			
            EQS_SetViewPortRect(&Rect);
			

            g_nMaxCharPerLine = (EQS_GetMaxWidth()-6) / g_FontSize.xWidth;

            sDrawTextBuf(hControl);
            sDrawSoftKey(hControl);
            sDrawScrollBar(hControl);

            sSetCursorPos(hControl);
            sDrawStatusLine(pTextEdit);
            sDrawCursor(hControl);


        }
 
    }
    else if(EQS_IsActive(hControl) && 
            (EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_READONLY))
    {
        sDrawDisplayTextEdit(hControl);
        //l_fDarken = FALSE;
    }
    else
    {
        if(pTextEdit->hCursorTimer != NULL)
        {
            EQS_StopControlTimer(hControl,pTextEdit->hCursorTimer);
            pTextEdit->hCursorTimer = NULL;
        }

        if(pTextEdit->hAutoCursorTimer != NULL)
        {
            EQS_StopControlTimer(hControl,pTextEdit->hAutoCursorTimer);
            pTextEdit->hAutoCursorTimer = NULL;
        }

        // Display Mode
        if(!(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_HIDDEN))
        {
            sDrawDisplayTextEdit(hControl);
        }
        //l_fDarken = FALSE;
    }

    EQS_ReleaseDC();
}

/*
**---------------------------------------------------------------------------
**  Text Edit Editing Functions
**---------------------------------------------------------------------------
*/

LOCAL void sRecoverLastStr()
{
    if(l_LastSave[1] != NULL)
    {
        pTextEdit->pszBuffer[pTextEdit->nPos-2] = l_LastSave[0];
        pTextEdit->pszBuffer[pTextEdit->nPos-1] = l_LastSave[1];
    }
    else if(l_LastSave[0] != NULL)
    {
         pTextEdit->pszBuffer[pTextEdit->nPos-1] = l_LastSave[0];
    }

    pTextEdit->nLen = l_LastLen;
    pTextEdit->nPos = l_LastPos;
}

LOCAL UINT16 sAddStr(HCONTROL hControl, BYTE * szStr, UINT16  nLen, BOOL fWord, BOOL fMovPos)
{
    INT16   nMovChar, nAddChar;
    TEXT_EDIT_MODE_E oldMode;
    BOOL fWord1;

    g_fAdd = FALSE;
    l_LastLen = pTextEdit->nLen;
    l_LastPos = pTextEdit->nPos;

    fWord1 = fWord;

    // Shift right old string
    nAddChar = nLen;

    if(pTextEdit->pszFormat != NULL)
    {
        BYTE   inputKey;

        oldMode = GET_CURRENT_MODE(pTextEdit);
        sAddFormatChar();

        if(sGetStringLength(pTextEdit->nLen) >= 
            sGetFormatLength(pTextEdit->pszFormat, STRLEN(pTextEdit->pszFormat), pTextEdit->nFormatLen))
        {
            g_LastEvent = EQE_NONE;
            sEndEditChar();
            //g_fEditChar = FALSE;
            return 0;
        }

        FormatType = sGetCurrentFormat((UINT8)(pTextEdit->nPos+1));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);

        if(GET_CURRENT_MODE(pTextEdit) != oldMode)  // Change szStr
        {
            switch(GET_CURRENT_MODE(pTextEdit)){
            case TEM_ENGLISH_UPPER:
            case TEM_ENGLISH_LOWER:
		        inputKey = g_CurrentEvent - EQE_KEY_0;

		        if(g_CurrentEvent == EQE_KEY_STAR)	        inputKey = 10;
		        else if(g_CurrentEvent == EQE_KEY_POUND)	inputKey = 11;

		        if(GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_UPPER)
                {
			        if(englishTableU[g_nInputCount][inputKey] == ' ')	g_nInputCount = 0;
                    memcpy(szStr, &englishTableU[g_nInputCount][inputKey], 1);
		        }
		        else
                {
			        if(englishTableL[g_nInputCount][inputKey] == ' ')	g_nInputCount = 0;
                    memcpy(szStr, &englishTableL[g_nInputCount][inputKey], 1);
                }

                nAddChar = 1;
                fWord1 = FALSE;
                break;

            case TEM_NUMBER:
                inputKey = g_CurrentEvent - EQE_KEY_0 + '0';

		        if(g_CurrentEvent == EQE_KEY_STAR)	        inputKey = '*';
		        else if(g_CurrentEvent == EQE_KEY_POUND)	inputKey = '#';

                memcpy(szStr, &inputKey, 1);

                nAddChar = 1;
                fWord1 = FALSE;
                break;

            case TEM_KOREAN:
                if(!l_fSkyHangul)
                {
                    sOldKorean(hControl, g_CurrentEvent);  // Old
                    return 0;
                }
                else
                {
		            sKorean(hControl, g_CurrentEvent);     // New(SKY Hangul)
                    return 0;
                }
                break;
            }
        }
    }


    if(pTextEdit->nLen + nLen > pTextEdit->nMaxLen)
    {
        nAddChar = pTextEdit->nMaxLen - pTextEdit->nLen;

        if(nAddChar > 1)
        {
            if(IsMiddleOfMultiByte((BYTE *)szStr, (UINT16)(nAddChar-1)))
                nAddChar--;
        }
    }

    if((fWord1 && nAddChar < 2) || (!fWord1 && nAddChar <1))   
    {
        g_LastEvent = EQE_NONE;
        sEndEditChar();
        //g_fEditChar = FALSE;
        return 0;
    }

    nMovChar = pTextEdit->nLen - pTextEdit->nPos;

    if(nMovChar > 0)
    {
        memmove(&pTextEdit->pszBuffer[pTextEdit->nPos+nLen], 
        &pTextEdit->pszBuffer[pTextEdit->nPos],
        nMovChar);

        pTextEdit->nLen+= nMovChar - (pTextEdit->nLen - pTextEdit->nPos);
    }

    if(fWord1 && nAddChar == 2)
    {
        pTextEdit->pszBuffer[pTextEdit->nPos] = szStr[1];
        pTextEdit->pszBuffer[pTextEdit->nPos+1] = szStr[0];
    }
    else if(!fWord1)
    {
        memcpy(&pTextEdit->pszBuffer[pTextEdit->nPos], szStr, nAddChar);
    }

    pTextEdit->nLen += nAddChar;
    if(fMovPos)
        pTextEdit->nPos += nAddChar;

    pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';

    EQC_SET_DIRTY(hControl, TRUE);

    g_fAdd = TRUE;
    g_fEditChar = TRUE;

    if(pTextEdit->pszFormat != NULL && !g_fEditChar)
    {
        FormatType = sGetCurrentFormat((UINT8)(pTextEdit->nPos+1));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);
    }

    memset(l_LastSave, NULL, 2);

    return nAddChar;
}

LOCAL BOOL sCanAdd(UINT16 nLen)
{
    UINT16 nAddChar;

    nAddChar = nLen;

    if(pTextEdit->pszFormat != NULL)
    {
        if(sGetStringLength(pTextEdit->nLen) >= 
            sGetFormatLength(pTextEdit->pszFormat, STRLEN(pTextEdit->pszFormat), pTextEdit->nFormatLen))
        {
            g_fAdd = FALSE;
            return FALSE;
        }
    }

    if(pTextEdit->nLen + nLen > pTextEdit->nMaxLen)
    {
        nAddChar = pTextEdit->nMaxLen - pTextEdit->nLen;
    }

    if(nAddChar < nLen)   
    {
        g_fAdd = FALSE;
        return FALSE;
    }

    return TRUE;
}

LOCAL BOOL sReplaceStr(HCONTROL hControl, BYTE * szStr, UINT16  nLen, BOOL fWord, BOOL fNextAdd)
{
    l_LastLen = pTextEdit->nLen;
    l_LastPos = pTextEdit->nPos;

    if(fNextAdd)    // Check String Length
    {
        if(!sCanAdd(2))
            return FALSE;
    }

    if(nLen > pTextEdit->nPos)
        nLen = pTextEdit->nPos;

    pTextEdit->nPos -= nLen;    

    if(fWord)
    {
        l_LastSave[0] = pTextEdit->pszBuffer[pTextEdit->nPos];
        l_LastSave[1] = pTextEdit->pszBuffer[pTextEdit->nPos+1];

        pTextEdit->pszBuffer[pTextEdit->nPos] = szStr[1];
        pTextEdit->pszBuffer[pTextEdit->nPos+1] = szStr[0];
    }
    else
    {
        l_LastSave[0] = pTextEdit->pszBuffer[pTextEdit->nPos];
        memcpy(&pTextEdit->pszBuffer[pTextEdit->nPos], szStr, nLen);
    }

    pTextEdit->nPos += nLen;

    pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';

    EQC_SET_DIRTY(hControl, TRUE);

    return TRUE;
}

LOCAL void sDeleteStr(HCONTROL hControl)
{
    UINT16 nLen;
    INT16 uMovLen;


    if(pTextEdit->nLen < 1)
        return;

    if(pTextEdit->nPos == pTextEdit->nLen)
    {
        if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos-1)))
            nLen = 2;
        //else if(pTextEdit->pszBuffer[pTextEdit->nPos-1] == '\n')
//            nLen = 2;
        else
            nLen = 1;
    }
    else
    {
        if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos+1)))
            nLen = 2;
//        else if(pTextEdit->pszBuffer[pTextEdit->nPos+1] == '\n')
//            nLen = 2;
        else
            nLen = 1;

        // Shift left old string
        uMovLen = pTextEdit->nLen - pTextEdit->nPos - nLen;
        if(uMovLen > 0)
        {
            memmove(&pTextEdit->pszBuffer[pTextEdit->nPos],
                    &pTextEdit->pszBuffer[pTextEdit->nPos+nLen], 
                    uMovLen);
        }
    }

    pTextEdit->nLen -= nLen;    
    if(pTextEdit->nPos > pTextEdit->nLen) 
        pTextEdit->nPos = pTextEdit->nLen;

    memset(&pTextEdit->pszBuffer[pTextEdit->nLen], ' ', 
        (SIZE_T)(pTextEdit->nMaxLen - pTextEdit->nLen));
    pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';

    if(pTextEdit->pszFormat != NULL)
    {
        sDeleteFormatChar(hControl);

        FormatType = sGetCurrentFormat((UINT16)(pTextEdit->nPos));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);
    }

    EQC_SET_DIRTY(hControl, TRUE);
}

LOCAL void sProcMoveEvent(HCONTROL hControl, EQS_EVENT_ID EventID)
{
    switch(EventID)
    {
    case EQE_KEY_POUND:
        if(l_fSkyHangul || GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
            break;
        // Go through ... Old SKY Hangul
    case EQE_KEY_RIGHT:
        if(pTextEdit->pszFormat != NULL)        return;

        if(!g_fEditChar)
        {
            if(pTextEdit->nPos == pTextEdit->nLen)
            {
                if(pTextEdit->nPos+1 <= pTextEdit->nMaxLen)
                    sAddStr(hControl, (BYTE *)" ", 1, FALSE, TRUE);
            }
            else
            {
                pTextEdit->nPos++;
                if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos))){
                    pTextEdit->nPos++;
                }
//                if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
//                    pTextEdit->nPos ++ ;
            }
        }
        break;

    case EQE_KEY_STAR:
        if(l_fSkyHangul)
            break;
        // Go through ... Old SKY Hangul
    case EQE_KEY_LEFT:
        if(pTextEdit->pszFormat != NULL)        return;

        if(pTextEdit->nPos > 0)
        {
            pTextEdit->nPos--;
            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos)) 
                && pTextEdit->nPos > 0){
                pTextEdit->nPos--;
            }
            //if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
//                pTextEdit->nPos--;
        }
        break;

    case EQE_KEY_DOWN:
        if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            pTextEdit->nSymbolPage++;

            if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_USE_SKFONT)
            {
                if(pTextEdit->nSymbolPage >= SK_SYMBOL_PAGE_NUMBER)
                    pTextEdit->nSymbolPage = 0;
            }
            else
            {
                if(pTextEdit->nSymbolPage >= SYMBOL_PAGE_NUMBER)
                    pTextEdit->nSymbolPage = 0;
            }
        }
        else
        {
            if(pTextEdit->pszFormat != NULL)        return;

            if(sGetY(hControl) == sGetCurY(hControl))
            {
                pTextEdit->nPos = pTextEdit->nLen;
                break;
            }

            pTextEdit->nPos += g_nMaxCharPerLine;
            if(pTextEdit->nPos > pTextEdit->nLen)
                pTextEdit->nPos = pTextEdit->nLen;

            if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos)))
                pTextEdit->nPos--;

            if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
                pTextEdit->nPos--;
/*
            else if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
                pTextEdit->nPos -= 2;
*/
        }

        break;

    case EQE_KEY_UP:
        if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            if(pTextEdit->nSymbolPage == 0)
            {
                if(EQC_GET_STYLE(hControl) & CS_TEXT_EDIT_USE_SKFONT)
                    pTextEdit->nSymbolPage = SK_SYMBOL_PAGE_NUMBER-1;
                else
                    pTextEdit->nSymbolPage = SYMBOL_PAGE_NUMBER-1;
            }
            else
                pTextEdit->nSymbolPage--;
        }
        else
        {
            if(pTextEdit->pszFormat != NULL)        return;

//            if(sGetCurY(hControl) == 0)
//                break;

            if((pTextEdit->nPos - g_nMaxCharPerLine) < 0)
            {
                pTextEdit->nPos = 0;
                break;
            }
            else
            {
                pTextEdit->nPos -= g_nMaxCharPerLine;

                if(IsMiddleOfMultiByte(pTextEdit->pszBuffer, (UINT16)(pTextEdit->nPos)))
                    pTextEdit->nPos++;

                if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
                    pTextEdit->nPos += 1;
/*
                else if(pTextEdit->pszBuffer[pTextEdit->nPos] == '\n')
                    pTextEdit->nPos ++;
*/
            }
        }
        break;

    default:
        break;
    }

    if(pTextEdit->pszFormat != NULL)
    {
        FormatType = sGetCurrentFormat((UINT16)(pTextEdit->nPos+1));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);
    }
}


/*
**---------------------------------------------------------------------------
**              한글 입력 관련 함수
**
**      sWansungToJohab
**      sJohabToWansung
**      sWansungToJohabJong
**      sGetBokMoum
**      sGetBokJaum
**      sKorean
**
**---------------------------------------------------------------------------
*/

LOCAL UINT16 sWansungToJohab(UINT16 wcode)
{
	UINT16	index;

	// 홑음
	if (0xA4A1 <= wcode && wcode <= 0xA4D3) 
    {
		index = wcode - 0xA4A1;
		return KSTableSingle[index];
	}

	// 완음
	if (0xB0A1 <= wcode && wcode <= 0xC8FE) 
    {
		wcode -= 0xB0A1;
		index = (wcode & 0xFF)*0x5E + (wcode >> 8);
		return KSTable[index];
	}

	return 0;
}

LOCAL UINT16 sJohabToWansung(UINT16 jcode)
{
	UINT16	min, mid, max, oldmid;
	UINT16	row, col;
	UINT16	i;

    if(jcode == 0x0000)
        return 0;

	// 홑음
	for (i = 0; i < 51; i++)
    {
		if (KSTableSingle[i] == jcode)
			return 0xA4A1 + i;
    }

	// 완음
	min = 0;
	max = 2349;
	mid = (min + max)/2;
	oldmid = 0xFFFF;

	while (mid != oldmid) 
    {
	
        if(mid >= 2350) break;

		if (KSTable[mid] == jcode) 
        {
			row = mid/0x5E;
			col = mid%0x5E;
			return 0xB0A1 + (row << 8) + col;
		}

		if (KSTable[mid] < jcode)
			min = mid + 1;
		else
			max = mid - 1;

		oldmid = mid;
		mid = (min + max)/2;
	}

	// 대응되는 완성형 코드 없음
	return 0;
}
LOCAL UINT16 sWansungToJohabJong(UINT16 code)
{
	UINT16	index;

	// 바침
	if (0xA4A1 <= code && code <= 0xA4BE) 
    {
		index = code - 0xA4A1;
		return KSTableJong[index];
	}

	return 0;
}

LOCAL UINT16 sGetBokMoum(UINT16 code1, UINT16 code2)
{
	UINT16	i;

	for (i = 0; i < 11; i++)
    {
		if (BokMoumTable[i][0] == code1 && BokMoumTable[i][1] == code2)
			return BokMoumTable[i][2];
    }

	return 0;
}

LOCAL UINT16 sGetBokJaum(UINT16 code1, UINT16 code2)
{
	UINT16	i;

	for (i = 0; i < 11; i++)
    {
		if (BokJaumTable[i][0] == code1 && BokJaumTable[i][1] == code2)
			return BokJaumTable[i][2];
    }

	return 0;
}


/*   한글 입력 관련 함수 - End  */

/*
**--------------------------------------------------------------------------
**   언어별 Automata (Korean, English, Symbol, Number..)
**--------------------------------------------------------------------------
*/

LOCAL void sKorean(HCONTROL hControl, EQS_EVENT_ID EventID)
{
	BOOL	fDupKey;
    UINT8   inputKey;
	BYTE	inputChar;

	if(EventID == g_LastEvent && g_fAdd)
    //if(EventID == g_LastEvent)
    {
		g_nInputCount++;
		g_nInputCount = g_nInputCount%3;
		fDupKey = TRUE;
	}
	else
    {
		g_nInputCount = 0;
		fDupKey = FALSE;
	}

	switch(EventID)
    {
    case EQE_KEY_CLEAR:
        switch(pTextEdit->nKoreanState)
        {
        case 1: // 자음
            pTextEdit->nPos -= 2;
        case 0: // 초기
            sDeleteStr(hControl);
            pTextEdit->nKoreanState = 0;
            break;
        case 2: // 자음+모음 or 자음+복모음
 		    uJohab = JOHAB(uJohabCho, FILL_JUNG, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
    	    pTextEdit->nKoreanState = 1;
            g_fEditChar = TRUE;
            break;

        case 3: // 자음+모음+자음
 		    uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
    	    pTextEdit->nKoreanState = 2;
            g_fEditChar = TRUE;
            break;

        case 4: // 자음+모음+복자음           
  		    uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uSaveCons));
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
		    pTextEdit->nKoreanState = 3;
            g_fEditChar = TRUE;
            break;
        }
        break;

    case EQE_KEY_0_LONG:
    case EQE_KEY_1_LONG:
    case EQE_KEY_2_LONG:
    case EQE_KEY_3_LONG:
    case EQE_KEY_4_LONG:
    case EQE_KEY_5_LONG:
    case EQE_KEY_6_LONG:
    case EQE_KEY_7_LONG:
    case EQE_KEY_8_LONG:
    case EQE_KEY_9_LONG:
/*
        switch(pTextEdit->nKoreanState)
        {
        case 0: // 초기
        case 1: // 자음
            if(g_fAdd == FALSE)
                break;

			pTextEdit->nPos -= 2;
			sDeleteStr(hControl);
            break;

        case 2: // 자음+모음 or 자음+복모음
 		    uJohab = JOHAB(uJohabCho, FILL_JUNG, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;

        case 3: // 자음+모음+자음
 		    uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;

        case 4: // 자음+모음+복자음           
  		    uJohab = JOHAB(uJohabCho, uJohabJung, uSaveCons);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;
        }
*/
        sRecoverLastStr();

		inputChar = (EventID - EQE_KEY_0_LONG) + '0';
		sAddStr(hControl, &inputChar, 1, FALSE, TRUE);
        pTextEdit->nKoreanState = 0;
        sEndEditChar();
		//g_fEditChar = FALSE;
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
        g_fEditChar = TRUE;
		inputKey = EventID - EQE_KEY_0;

		if(EventID == EQE_KEY_STAR)			inputKey = 10;
		else if(EventID == EQE_KEY_POUND)	inputKey = 11;

		if(koreanTable[g_nInputCount][inputKey*2] == ' ')
			g_nInputCount = 0;

        uKorCode = ((WORD)koreanTable[g_nInputCount][inputKey*2] << 8) | 
			      (WORD)koreanTable[g_nInputCount][inputKey*2 + 1];

	
        switch(pTextEdit->nKoreanState)
        {
		case 0:	// 초기
			uJohabCho = FILL_CHO;
			uJohabJung = FILL_JUNG;
			uJohabJong = FILL_JONG;

			uSaveJohabCho = FILL_CHO;
			uSaveJohabJung = FILL_JUNG;
			uSaveJohabJong = FILL_JONG;

            uSaveWansung = NULL;

            uBokMoum = 0;

			if(fDupKey)
				sReplaceStr(hControl, (BYTE *)&uKorCode, 2, TRUE, FALSE);
			else
            {
				if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                    break;
            }

			if(ISJAUM(uKorCode))
            {
				uJohabCho = sWansungToJohab(uKorCode);
				pTextEdit->nKoreanState = 1;
			}
            else
            {
				pTextEdit->nKoreanState = 0;
			}
			
			break;

		case 1:	// 자음 
			if(ISJAUM(uKorCode))    // 자음
            {	
                if(fDupKey)
                {
                    uRecoverCons = uKorCode;
                    if(uJohabJong == FILL_JONG)
                    {
                        uJohabJong = sWansungToJohabJong(uKorCode);
                        uJohab = JOHAB(uSaveJohabCho, uSaveJohabJung, uJohabJong);
                        uWansung = sJohabToWansung(uJohab);
                    
                        if(uWansung)
                        {           // 자음/쌍자음 받침
                            pTextEdit->nPos -= 2;
                            sDeleteStr(hControl);
                            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
                            uJohabCho = uSaveJohabCho;
                            uJohabJung = uSaveJohabJung;
                            uSaveJohabCho = FILL_CHO;
                            uSaveJohabJung = FILL_JUNG;
                            uSaveCons = uKorCode;
                            pTextEdit->nKoreanState = 3;
                        }
                        else
                        {
        					sReplaceStr(hControl, (BYTE *)&uKorCode, 2, TRUE, FALSE);
                            uJohabCho = sWansungToJohab(uKorCode);
        					uJohabJung = FILL_JUNG;
		        			uJohabJong = FILL_JONG;
                        }

                    }
                    else
                    {                  // 복자음 받침
					    uTmpCode = sGetBokJaum(uSaveCons, uRecoverCons);
					    uJohab = JOHAB(uSaveJohabCho, uSaveJohabJung, sWansungToJohabJong(uTmpCode));
					    uWansung = sJohabToWansung(uJohab);

                        if(uWansung)
                        {
                            pTextEdit->nPos -= 2;
                            sDeleteStr(hControl);
                            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
                            uJohabCho = uSaveJohabCho;
                            uJohabJung = uSaveJohabJung;

                            pTextEdit->nKoreanState = 4;
                        }
                        else
                        {
        					sReplaceStr(hControl, (BYTE *)&uKorCode, 2, TRUE, FALSE);
                            uJohabCho = sWansungToJohab(uKorCode);
        					uJohabJung = FILL_JUNG;
                        }
                    }
                }
                else
                {
					if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;

                    uJohabCho = sWansungToJohab(uKorCode);

                    uJohabJung = FILL_JUNG;
        			uJohabJong = FILL_JONG;    

                    uSaveJohabCho = FILL_CHO;
                    uSaveJohabJung = FILL_JUNG;

                }
			}
            else
            {		// 모음
				uSaveCode = uKorCode;	

				uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 2;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
				}

       			uSaveJohabCho = FILL_CHO;
                uSaveJohabJung = FILL_JUNG;
                uSaveJohabJong = FILL_JONG;
			}

			break;

		case 2:	// 자음 + 모음 or 자음 + 복모음
			if(ISJAUM(uKorCode))
            {	// 자음
				uSaveCons = uKorCode;
                uSaveJohabCho = uJohabCho;
                uSaveJohabJung = uJohabJung;
                uSaveJohabJong = uJohabJong;

				uJohabJong = sWansungToJohabJong(uKorCode);
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 3;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					uJohabJung = FILL_JUNG;
					uJohabJong = FILL_JONG;
					uJohabCho = sWansungToJohab(uKorCode);

					pTextEdit->nKoreanState = 1;
				}
			}
            else
            {		// 모음
				if(fDupKey)
                {
					uSaveCode = uKorCode;	

					uJohabJung = sWansungToJohab(uKorCode);
					uJohabJong = FILL_JONG;
					uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
					uWansung = sJohabToWansung(uJohab);

					if(uWansung)
                    {
                        sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
						pTextEdit->nKoreanState = 2;
					}
                    else
                    {
                        if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                            break;
						pTextEdit->nKoreanState = 0;
					}
				}
                else
                {
					uBokMoum = sGetBokMoum(uSaveCode, uKorCode);
                    uSaveCode = uBokMoum;
					uJohabJung = sWansungToJohab(uBokMoum);
                    uJohabJong = FILL_JONG;
					uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
					uWansung = sJohabToWansung(uJohab);

					if(uBokMoum && uWansung)
                    {
                        sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
						pTextEdit->nKoreanState = 2;
					}
                    else
                    {
                        if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                            break;
						pTextEdit->nKoreanState = 0;
					}
				}
			}

			break;

		case 3:		// 자음 + 모음 + 자음
			if(ISJAUM(uKorCode))
            {
				if(fDupKey)
                {
					uSaveCons = uKorCode;
                    uSaveJohabCho = uJohabCho;
                    uSaveJohabJung = uJohabJung;
       			    uSaveWansung = uWansung;

					uJohabJong = sWansungToJohabJong(uKorCode);
                    uSaveJohabJong = uJohabJong;

					uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
					uWansung = sJohabToWansung(uJohab);

					if(uWansung)
                    {
                        sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
						pTextEdit->nKoreanState = 3;
					}
                    else
                    {
						uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
						uWansung = sJohabToWansung(uJohab);
                        if(!sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        {
                            pTextEdit->nKoreanState = 0;
                            break;
                        }
                        if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                            break;
						uJohabJung = FILL_JUNG;
						uJohabJong = FILL_JONG;
						uJohabCho = sWansungToJohab(uKorCode);

						pTextEdit->nKoreanState = 1;
					}
				}
                else
                {
					uRecoverCons = uKorCode;
                    uSaveJohabCho = uJohabCho;
                    uSaveJohabJung = uJohabJung;

					uSaveWansung = uWansung;
					uTmpCode = sGetBokJaum(uSaveCons, uKorCode);
					uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uTmpCode));
					uWansung = sJohabToWansung(uJohab);

					if(uTmpCode && uWansung)
                    {	// 복자음
                        sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
						pTextEdit->nKoreanState = 4;
					}
                    else
                    {
                        if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                            break;
						uJohabCho = sWansungToJohab(uKorCode);
						uJohabJung = FILL_JUNG;
						pTextEdit->nKoreanState = 1;
					}
				}
			}
            else
            {
				uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
				uSaveWansung = sJohabToWansung(uJohab);
				uSaveCode = uKorCode;
				uJohabCho = sWansungToJohab(uSaveCons);
				uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;

				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    if(!sReplaceStr(hControl, (BYTE *)&uSaveWansung, 2, TRUE, TRUE))
                    {
                        pTextEdit->nKoreanState = 0;
                        break;
                    }
                    if(!sAddStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 2;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
				}
			}

			break;

		case 4:	// 자 + 모 + 복자음
			if(ISJAUM(uKorCode))
            {
				if(fDupKey)
                {
					uRecoverCons = uKorCode;
					uTmpCode = sGetBokJaum(uSaveCons, uRecoverCons);
					uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uTmpCode));
					uWansung = sJohabToWansung(uJohab);

					if(uTmpCode && uWansung)
                    {	// 복자음
                        sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
						pTextEdit->nKoreanState = 4;
					}
                    else
                    {
                        uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uSaveCons));
                        uWansung = sJohabToWansung(uJohab);
                        if(!sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        {
                            pTextEdit->nKoreanState = 0;
                            break;
                        }
                        if(!sAddStr(hControl, (BYTE *)&uRecoverCons, 2, TRUE, TRUE))
                            break;
						uJohabCho = sWansungToJohab(uKorCode);
						uJohabJung = FILL_JUNG;
						pTextEdit->nKoreanState = 1;
					}
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					uJohabCho = sWansungToJohab(uKorCode);
					uJohabJung = FILL_JUNG;
                    uJohabJong = FILL_JONG;
                    uSaveJohabCho = FILL_CHO;
                    uSaveJohabJung = FILL_JUNG;

					pTextEdit->nKoreanState = 1;
				}
			}
            else
            {
				uJohabCho = sWansungToJohab(uRecoverCons);

                uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;
				uSaveCode = uKorCode;
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);
				if(uWansung)
                {
                    if(!sReplaceStr(hControl, (BYTE *)&uSaveWansung, 2, TRUE, TRUE))
                    {
                        pTextEdit->nKoreanState = 0;
                        break;
                    }
                    if(!sAddStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 2;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
				}
			}

			break;

		default:
			break;
		}
		break;

	default	:
		break;
	}
}

LOCAL void sOldKorean(HCONTROL hControl, EQS_EVENT_ID EventID)
{
    UINT8   inputKey;
	BYTE inputChar;

//    static UINT16	uTmpCode, uKorCode, uWansung, uJohab, uBokMoum, uRecoverCons;
//    static UINT16  uSaveCode, uSaveCons, uSaveWansung, uSaveJohabCho, uSaveJohabJung, uSaveJohabJong;
//    static UINT16  uJohabCho, uJohabJung, uJohabJong;

	switch(EventID)
    {
    case EQE_KEY_CLEAR:
        switch(pTextEdit->nKoreanState)
        {
        case 1: // 자음
            pTextEdit->nPos -= 2;
        case 0: // 초기
            sDeleteStr(hControl);
            pTextEdit->nKoreanState = 0;
            l_KoreanMode = JAUM1;
            break;

        case 2: // 자음+모음 or 자음+복모음
 		    uJohab = JOHAB(uJohabCho, FILL_JUNG, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
    	    pTextEdit->nKoreanState = 1;

            l_KoreanMode = MOUM1;
            g_fEditChar = TRUE;
            break;

        case 3: // 자음+모음+자음
 		    uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
    	    pTextEdit->nKoreanState = 2;
            g_fEditChar = TRUE;
            break;

        case 4: // 자음+모음+복자음           
  		    uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uSaveCons));
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
		    pTextEdit->nKoreanState = 3;
            g_fEditChar = TRUE;
            break;
        }
        break;

    case EQE_KEY_0_LONG:
    case EQE_KEY_1_LONG:
    case EQE_KEY_2_LONG:
    case EQE_KEY_3_LONG:
    case EQE_KEY_4_LONG:
    case EQE_KEY_5_LONG:
    case EQE_KEY_6_LONG:
    case EQE_KEY_7_LONG:
    case EQE_KEY_8_LONG:
    case EQE_KEY_9_LONG:
/*
        switch(pTextEdit->nKoreanState)
        {
        case 0: // 초기
        case 1: // 자음
            if(g_fAdd == FALSE)
                break;

			pTextEdit->nPos -= 2;
			sDeleteStr(hControl);
            break;

        case 2: // 자음+모음 or 자음+복모음
 		    uJohab = JOHAB(uJohabCho, FILL_JUNG, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;

        case 3: // 자음+모음+자음
 		    uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;

        case 4: // 자음+모음+복자음           
  		    uJohab = JOHAB(uJohabCho, uJohabJung, uSaveCons);
		    uWansung = sJohabToWansung(uJohab);
            sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE);
            break;
        }
*/
        sRecoverLastStr();

		inputChar = (EventID - EQE_KEY_0_LONG) + '0';
		sAddStr(hControl, &inputChar, 1, FALSE, TRUE);
        pTextEdit->nKoreanState = 0;
		//g_fEditChar = FALSE;
        sEndEditChar();
		break;

    case EQE_KEY_STAR:
    case EQE_KEY_POUND:
        sAddStr(hControl, (BYTE *)" ", 1, FALSE, TRUE);
        pTextEdit->nKoreanState = 0;
        l_KoreanMode = JAUM1;
        //g_fEditChar = FALSE;
        sEndEditChar();
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
        g_fEditChar = TRUE;
		inputKey = EventID - EQE_KEY_0;

        uKorCode = ((WORD)old_koreanTable[l_KoreanMode][inputKey*2] << 8) | 
			      (WORD)old_koreanTable[l_KoreanMode][inputKey*2 + 1];

        if(old_koreanTable[l_KoreanMode][inputKey*2] == ' ')    // Space
        {
            sAddStr(hControl, (BYTE *)" ", 1, FALSE, TRUE);
            pTextEdit->nKoreanState = 0;
            l_KoreanMode = JAUM1;
            sEndEditChar();
            break;
        }

        switch(pTextEdit->nKoreanState)
        {
		case 0:	// 초기
			uJohabCho = FILL_CHO;
			uJohabJung = FILL_JUNG;
			uJohabJong = FILL_JONG;

			uSaveJohabCho = FILL_CHO;
			uSaveJohabJung = FILL_JUNG;
			uSaveJohabJong = FILL_JONG;

            uSaveWansung = NULL;

            uBokMoum = 0;

			if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                break;

			if(ISJAUM(uKorCode))
            {
				uJohabCho = sWansungToJohab(uKorCode);
				pTextEdit->nKoreanState = 1;

                l_KoreanMode = MOUM1;
			}
            else
            {
				pTextEdit->nKoreanState = 0;
			}
			break;

		case 1:	// 자음 
			if(ISJAUM(uKorCode))    // 자음
            {	
				if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                    break;

                uJohabCho = sWansungToJohab(uKorCode);

                uJohabJung = FILL_JUNG;
        		uJohabJong = FILL_JONG;    

                uSaveJohabCho = FILL_CHO;
                uSaveJohabJung = FILL_JUNG;
                l_KoreanMode = MOUM1;
			}
            else
            {		// 모음
				uSaveCode = uKorCode;	

				uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 2;
                    l_KoreanMode = JAUM1;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
                    l_KoreanMode = JAUM1;
				}

       			uSaveJohabCho = FILL_CHO;
                uSaveJohabJung = FILL_JUNG;
                uSaveJohabJong = FILL_JONG;
			}

			break;

		case 2:	// 자음 + 모음 or 자음 + 복모음
			if(ISJAUM(uKorCode))
            {	// 자음
				uSaveCons = uKorCode;
                uSaveJohabCho = uJohabCho;
                uSaveJohabJung = uJohabJung;
                uSaveJohabJong = uJohabJong;

				uJohabJong = sWansungToJohabJong(uKorCode);
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 3;
                    l_KoreanMode = JAUM1;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					uJohabJung = FILL_JUNG;
					uJohabJong = FILL_JONG;
					uJohabCho = sWansungToJohab(uKorCode);

					pTextEdit->nKoreanState = 1;
                    l_KoreanMode = MOUM1;
				}
			}
            else
            {		// 모음
				uBokMoum = sGetBokMoum(uSaveCode, uKorCode);
                uSaveCode = uBokMoum;
				uJohabJung = sWansungToJohab(uBokMoum);
                uJohabJong = FILL_JONG;
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uBokMoum && uWansung)
                {
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 2;
                    l_KoreanMode = JAUM1;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
                    l_KoreanMode = JAUM1;
				}
			}

			break;

		case 3:		// 자음 + 모음 + 자음
			if(ISJAUM(uKorCode))
            {
				uRecoverCons = uKorCode;
                uSaveJohabCho = uJohabCho;
                uSaveJohabJung = uJohabJung;

				uSaveWansung = uWansung;
				uTmpCode = sGetBokJaum(uSaveCons, uKorCode);
				uJohab = JOHAB(uJohabCho, uJohabJung, sWansungToJohabJong(uTmpCode));
				uWansung = sJohabToWansung(uJohab);

				if(uTmpCode && uWansung)
                {	// 복자음
                    sReplaceStr(hControl, (BYTE *)&uWansung, 2, TRUE, FALSE);
					pTextEdit->nKoreanState = 4;
                    l_KoreanMode = JAUM1;
			    }
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					uJohabCho = sWansungToJohab(uKorCode);
					uJohabJung = FILL_JUNG;
					pTextEdit->nKoreanState = 1;
                    l_KoreanMode = MOUM1;
				}
			}
            else
            {
				uJohab = JOHAB(uJohabCho, uJohabJung, FILL_JONG);
				uSaveWansung = sJohabToWansung(uJohab);
				uSaveCode = uKorCode;
				uJohabCho = sWansungToJohab(uSaveCons);
				uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;

				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);

				if(uWansung)
                {
                    if(!sReplaceStr(hControl, (BYTE *)&uSaveWansung, 2, TRUE, TRUE))
                    {
                        pTextEdit->nKoreanState = 0;
                        break;
                    }
                    if(!sAddStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 2;
                    l_KoreanMode = JAUM1;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
                    l_KoreanMode = JAUM1;
				}
			}

			break;

		case 4:	// 자 + 모 + 복자음
			if(ISJAUM(uKorCode))
            {
                if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                    break;
				uJohabCho = sWansungToJohab(uKorCode);
				uJohabJung = FILL_JUNG;
				pTextEdit->nKoreanState = 1;
                l_KoreanMode = MOUM1;
			}
            else
            {
				uJohabCho = sWansungToJohab(uRecoverCons);

                uJohabJung = sWansungToJohab(uKorCode);
				uJohabJong = FILL_JONG;
				uSaveCode = uKorCode;
				uJohab = JOHAB(uJohabCho, uJohabJung, uJohabJong);
				uWansung = sJohabToWansung(uJohab);
				if(uWansung)
                {
                    if(!sReplaceStr(hControl, (BYTE *)&uSaveWansung, 2, TRUE, TRUE))
                    {
                        pTextEdit->nKoreanState = 0;
                        break;
                    }
                    if(!sAddStr(hControl, (BYTE *)&uWansung, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 2;
                    l_KoreanMode = JAUM1;
				}
                else
                {
                    if(!sAddStr(hControl, (BYTE *)&uKorCode, 2, TRUE, TRUE))
                        break;
					pTextEdit->nKoreanState = 0;
                    l_KoreanMode = JAUM1;
				}
			}

			break;

		default:
			break;
		}
		break;

	default	:
		break;
	}
}

LOCAL void sEnglish(HCONTROL hControl, EQS_EVENT_ID EventID)
{
    BYTE   inputKey, inputChar;
    BOOL   fInsert;

	if(EventID == g_LastEvent)
    {
		g_nInputCount++;
		g_nInputCount = g_nInputCount % 4;
		fInsert = FALSE;
	}
    else
    {
		g_nInputCount = 0;
        fInsert = TRUE;
    }

	switch(EventID)
    {
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
		inputKey = EventID - EQE_KEY_0;

		if(EventID == EQE_KEY_STAR)	        inputKey = 10;
		else if(EventID == EQE_KEY_POUND)	inputKey = 11;

		if(GET_CURRENT_MODE(pTextEdit) == TEM_ENGLISH_UPPER)
        {
			if(englishTableU[g_nInputCount][inputKey] == ' ')	g_nInputCount = 0;
			inputChar = englishTableU[g_nInputCount][inputKey];
		}
		else
        {
			if(englishTableL[g_nInputCount][inputKey] == ' ')	g_nInputCount = 0;
			inputChar = englishTableL[g_nInputCount][inputKey];
        }

        if(EventID == EQE_KEY_STAR || EventID == EQE_KEY_POUND)	
            fInsert = TRUE;

        if(fInsert)
        {
            sAddStr(hControl, &inputChar, 1, FALSE, TRUE);
        }
        else if(g_fEditChar)
            sReplaceStr(hControl, &inputChar, 1, FALSE, FALSE);
		break;

    case EQE_KEY_CLEAR:
        sDeleteStr(hControl);
        break;

	default	:
		break;
	}
}

LOCAL void sEndSymbol(void)
{
   	g_LastEvent = EQE_NONE;
	g_nInputCount = 0;
    //g_fEditChar = FALSE;
    sEndEditChar();

    pTextEdit->EditMode.CurrentMode = 0;

    if(GET_CURRENT_MODE(pTextEdit) == TEM_KOREAN)
        pTextEdit->nKoreanState = 0;

    EQS_SendEvent(EQE_APP_DRAW, 0, 0);  // Draw All
}

LOCAL void sSymbol(HCONTROL hControl, EQS_EVENT_ID EventID)
{
    UINT8   nInput;
    BYTE    bChar;
    UINT16  wChar;

	g_nInputCount = 0;

	switch(EventID)
    {
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

        nInput = EventID - EQE_KEY_0;

		if(EventID == EQE_KEY_STAR)	        nInput = 10;
		else if(EventID == EQE_KEY_POUND)	nInput = 11;

        if(pTextEdit->nSymbolPage < 3)      // one-byte symbol
        {
            bChar = SymbolCode1[pTextEdit->nSymbolPage][nInput];
            sAddStr(hControl, &bChar, 1, FALSE, TRUE);
        }
        else if(pTextEdit->nSymbolPage < 8) // two-byte symbol
        {
            wChar = ((WORD)SymbolCode2[pTextEdit->nSymbolPage-3][nInput*2] << 8) | 
			          (WORD)SymbolCode2[pTextEdit->nSymbolPage-3][nInput*2 + 1];

            sAddStr(hControl, (BYTE *)&wChar, 2, TRUE, TRUE);
        }
        else    // Special font
        {
            wChar = ((WORD)SymbolCode3[pTextEdit->nSymbolPage-8][nInput*2] << 8) | 
			          (WORD)SymbolCode3[pTextEdit->nSymbolPage-8][nInput*2 + 1];

            sAddStr(hControl, (BYTE *)&wChar, 2, TRUE, TRUE);
        }

/*
        if(l_fSkyHangul)
        {
            sEndSymbol();
        }
*/
		break;

    case EQE_KEY_CLEAR:
        sDeleteStr(hControl);
        break;

	default	:
		break;
	}
}

LOCAL void sNumber(HCONTROL hControl, EQS_EVENT_ID EventID)
{
    BYTE   ucInput;

	g_nInputCount = 0;

	switch(EventID)
    {
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

        ucInput = EventID - EQE_KEY_0 + '0';

		if(EventID == EQE_KEY_STAR)	        ucInput = '*';
		else if(EventID == EQE_KEY_POUND)	ucInput = '#';

        sAddStr(hControl, &ucInput, 1, FALSE, TRUE);
		break;

    case EQE_KEY_CLEAR:
        sDeleteStr(hControl);
        break;

	default	:
		break;
	}
}


/*
**---------------------------------------------------------------------------------
**  지역별 AlpahEditor
**             
**   - eqs_i18n에 정의된 locale에 따른 함수 호출
**---------------------------------------------------------------------------------
*/

/// AlpaEditor for Korean
void EQC_KorEditBox(HCONTROL hControl, EQS_EVENT* pAnEvent)
{
	switch (pAnEvent->EventID)
	{
    case EQE_KEY_CLEAR:
/*
        if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            sEndSymbol();
            break;
        }
*/
   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;

        sEndEditChar();
        //g_fEditChar = FALSE;
        // go through.....
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:
        if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) != TEM_SYMBOL && pAnEvent->EventID != EQE_KEY_CLEAR)
        {
            sProcMoveEvent(hControl, pAnEvent->EventID);
            sInitMode(pTextEdit);    
            break;
        }
        // go through.....
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
        switch(GET_CURRENT_MODE(pTextEdit))
        {
        case TEM_KOREAN:
            if(pAnEvent->EventID != EQE_KEY_CLEAR && !g_fEditChar &&
                (pTextEdit->nMaxLen - pTextEdit->nLen) < 2 &&
                pTextEdit->pszFormat != NULL)
                break;

            if(!l_fSkyHangul)
                sOldKorean(hControl, pAnEvent->EventID);  // Old
            else
		        sKorean(hControl, pAnEvent->EventID);     // New(SKY Hangul)
		    break;
	
        case TEM_ENGLISH_UPPER:
        case TEM_ENGLISH_LOWER:
            if(pAnEvent->EventID != EQE_KEY_CLEAR && 
               !g_fEditChar && (pTextEdit->nMaxLen - pTextEdit->nLen) < 1 &&
               pTextEdit->pszFormat != NULL)
                break;

		    sEnglish(hControl, pAnEvent->EventID);
            break;

        case TEM_SYMBOL:
            if(pTextEdit->nSymbolPage < 3)
            {
                if(pAnEvent->EventID != EQE_KEY_CLEAR && 
                   (pTextEdit->nMaxLen - pTextEdit->nLen) < 1 && pTextEdit->pszFormat != NULL)
                    break;
            }
            else
            {
                if(pAnEvent->EventID != EQE_KEY_CLEAR && 
                   (pTextEdit->nMaxLen - pTextEdit->nLen) < 2 && pTextEdit->pszFormat != NULL)
                    break;
            }

            sSymbol(hControl, pAnEvent->EventID);
            sEndEditChar();
            break;

        case TEM_NUMBER:
            if(pAnEvent->EventID != EQE_KEY_CLEAR && 
               (pTextEdit->nMaxLen - pTextEdit->nLen) < 1 && pTextEdit->pszFormat != NULL)
                break;

            sNumber(hControl, pAnEvent->EventID);
            sEndEditChar();
            break;
        }
		break;

    // Handle Long Key Event (digit long key -> insert digit)
    case EQE_KEY_0_LONG:
    case EQE_KEY_1_LONG:
    case EQE_KEY_2_LONG:
    case EQE_KEY_3_LONG:
    case EQE_KEY_4_LONG:
    case EQE_KEY_5_LONG:
    case EQE_KEY_6_LONG:
    case EQE_KEY_7_LONG:
    case EQE_KEY_8_LONG:
    case EQE_KEY_9_LONG:
		if(!pTextEdit->EditMode.fEnableDigDown)
			break;

        if(!g_fEditChar && (pTextEdit->nMaxLen - pTextEdit->nLen) < 1 && pTextEdit->pszFormat != NULL)
            break;

/*
        if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            sEndSymbol();
            break;
        }
*/
		/////
		// Add number
		////
		if(GET_CURRENT_MODE(pTextEdit) != TEM_KOREAN)
		{
			BYTE inputChar;

			inputChar = (pAnEvent->EventID - EQE_KEY_0_LONG) + '0';
			
            if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL && pTextEdit->nSymbolPage >= 3)      // two-byte symbol
            {
                sDeleteStr(hControl);
                sAddStr(hControl, &inputChar, 1, FALSE, TRUE);
            }
            else
            {
			    sReplaceStr(hControl, &inputChar, 1, FALSE, FALSE);
            }

		}
		else
		{
            if(!l_fSkyHangul)
                sOldKorean(hControl, pAnEvent->EventID);  // Old
            else
		        sKorean(hControl, pAnEvent->EventID);     // New(SKY Hangul)
		    break;
		}
		break;

    case EQE_KEY_F2:
   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;
        sEndEditChar();
        //g_fEditChar = FALSE;

        pTextEdit->EditMode.CurrentMode++;

        if(pTextEdit->EditMode.CurrentMode == pTextEdit->EditMode.nModeNum)
            pTextEdit->EditMode.CurrentMode = 0;

        if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
            pTextEdit->nSymbolPage = 0;

        if(GET_CURRENT_MODE(pTextEdit) == TEM_KOREAN)
            pTextEdit->nKoreanState = 0;

        break;

    case EQE_KEY_RIGHT:
    case EQE_KEY_LEFT:
/*
        if(l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
        {
            sEndSymbol();
            break;
        }
*/
    case EQE_KEY_UP:
    case EQE_KEY_DOWN:

        if(!l_fSkyHangul && GET_CURRENT_MODE(pTextEdit) == TEM_KOREAN)
        {
            switch(pAnEvent->EventID){
            case EQE_KEY_UP:
                l_KoreanMode = (OLD_KOREAN_MODE_E)((l_KoreanMode == JAUM1)? JAUM2:JAUM1);
                break;
            case EQE_KEY_DOWN:
                l_KoreanMode = (OLD_KOREAN_MODE_E)((l_KoreanMode == MOUM1)? MOUM2:MOUM1);
                break;
            case EQE_KEY_LEFT:
            case EQE_KEY_RIGHT:
                sProcMoveEvent(hControl, pAnEvent->EventID);
                sInitMode(pTextEdit);
                break;
            }
        }
        else
        {
            sProcMoveEvent(hControl, pAnEvent->EventID);
            sInitMode(pTextEdit);
        }
        break;

    default:
        break;
	}	

}

/// AlphaEditor for USA
void EQC_UsaEditBox(HCONTROL hControl, EQS_EVENT* pAnEvent)
{
	BYTE inputChar;

	switch (pAnEvent->EventID)
	{
    case EQE_KEY_CLEAR:
   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;
        sEndEditChar();
        //g_fEditChar = FALSE;
        // Go through..
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
    case EQE_KEY_POUND:
    case EQE_KEY_STAR:

        if(pAnEvent->EventID != EQE_KEY_CLEAR)
            g_fEditChar = TRUE;

        switch(GET_CURRENT_MODE(pTextEdit)){
        case TEM_ENGLISH_UPPER:
        case TEM_ENGLISH_LOWER:
		    sEnglish(hControl, pAnEvent->EventID);
            break;

        case TEM_SYMBOL:
            sSymbol(hControl, pAnEvent->EventID);
            break;

        case TEM_NUMBER:
            sNumber(hControl, pAnEvent->EventID);
            break;
        }
		break;

    // Handle Long Key Event (digit long key -> insert digit)
    case EQE_KEY_0_LONG:
    case EQE_KEY_1_LONG:
    case EQE_KEY_2_LONG:
    case EQE_KEY_3_LONG:
    case EQE_KEY_4_LONG:
    case EQE_KEY_5_LONG:
    case EQE_KEY_6_LONG:
    case EQE_KEY_7_LONG:
    case EQE_KEY_8_LONG:
    case EQE_KEY_9_LONG:
		if(!pTextEdit->EditMode.fEnableDigDown)
			break;

		inputChar = (pAnEvent->EventID - EQE_KEY_0_LONG) + '0';
		sReplaceStr(hControl, &inputChar, 1, FALSE, FALSE);
		break;

    case EQE_KEY_F2:
   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;
        sEndEditChar();
        //g_fEditChar = FALSE;

        if(pTextEdit->EditMode.CurrentMode == pTextEdit->EditMode.nModeNum)
            pTextEdit->EditMode.CurrentMode = 0;

        if(GET_CURRENT_MODE(pTextEdit) == TEM_SYMBOL)
            pTextEdit->nSymbolPage = 0;
        break;

    case EQE_KEY_RIGHT:
    case EQE_KEY_LEFT:
    case EQE_KEY_UP:
    case EQE_KEY_DOWN:

        sProcMoveEvent(hControl, pAnEvent->EventID);

   		g_LastEvent = EQE_NONE;
		g_nInputCount = 0;
        pTextEdit->nKoreanState = 0;
        sEndEditChar();
        //g_fEditChar = FALSE;
        
        break;

    case EQE_KEY_CENTER:
    case EQE_KEY_POUND_LONG: 
    case EQE_KEY_STAR_LONG:
       
        break;

    default:
        break;
	}	
}
/*
**---------------------------------------------------------------------------
**  Function(external use only) Definitions
**---------------------------------------------------------------------------
*/
HCONTROL EQS_API EQC_TextEdit(COORD x, COORD y, UINT8 nChar, UINT8 nLine, 
                              TEXT_EDIT_MODE_E Mode, UINT16 nMaxLen, 
                              BYTE *pszStr, UINT16 nPos)
{
    HCONTROL hControl;
    SIZE dispSize;
    UINT8   i;
    UINT16 nStrLen;
    BYTE *pszTmp, *pszBuffer;

    // Current Font Size
    g_FontSize.xWidth = EQS_CharWidth('A');
    g_FontSize.yHeight = EQS_CharHeight('A');

    // Get Display Mode TextEdit Size
    dispSize.xWidth = (nChar * g_FontSize.xWidth);
    dispSize.yHeight = (nLine * g_FontSize.yHeight);
    if(dispSize.xWidth > GetMaxX()) dispSize.xWidth = GetMaxX();
    if(dispSize.yHeight > GetMaxY()) dispSize.yHeight = GetMaxY();

    // Init TEdit state variables
    g_LastEvent = EQE_NONE;
    g_nInputCount = 0;
    g_fEditChar = FALSE;

    pTextEdit = (TEXT_EDIT_T*)sAllocInstance();

	hControl = EQC_AllocControl(CT_TEXT_EDIT, (void *)pTextEdit, x, y, \
                                dispSize.xWidth, dispSize.yHeight, \
                                sHandleEvent);
    
    ASSERT(hControl != INVALID_CONTROL_HANDLE);

    if(nPos > nMaxLen) nPos = nMaxLen;
    if(Mode > TEM_MAX) Mode = TEM_ENGLISH_UPPER;      // English default;                

    // Set CommonHeader
    EQC_SET_FOCUS(hControl, TRUE);
    EQC_SET_ENABLE(hControl, TRUE);
    EQC_SET_STYLE(hControl, CS_TEXT_EDIT_DEFAULT);

    // Set TextEditorHeader
    pTextEdit->EditMode = NormalMode;

    for(i = 0; i < (pTextEdit->EditMode.nModeNum) ; i++)
    {
        if(pTextEdit->EditMode.ModeSequence[i] == Mode)
            pTextEdit->EditMode.CurrentMode = i;
    }

    pTextEdit->fPassword = FALSE;
    pTextEdit->pszBuffer = (BYTE*)EQS_Malloc((SIZE_T)(nMaxLen +1));
    pTextEdit->nLen = 0;
    pTextEdit->nPos = 0;
    pTextEdit->nMaxLen = nMaxLen;
    pTextEdit->pszBuffer[0] = '\0';
    pTextEdit->pszFormat = NULL;
    pTextEdit->nFormatLen = 0;
    pTextEdit->nChar = nChar;
    pTextEdit->nLine = nLine;

    // String copy routine
    if(pszStr)
        nStrLen = STRLEN(pszStr);
    else
        nStrLen = 0;

    if(nStrLen > pTextEdit->nMaxLen)
    {
        nStrLen = pTextEdit->nMaxLen;

        if(IsMiddleOfMultiByte(pszStr, (UINT16)nStrLen)) // two-byte code
            nStrLen --;
    }

    if(nStrLen)
        sAddStr(hControl, pszStr, nStrLen, FALSE, FALSE);

    pTextEdit->nPos = nPos;

    // Delete '\r' char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\r'))
    {
        //*pszTmp = ' ';
        memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1)));
        pTextEdit->nLen --;
        pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
        //pszBuffer = pszTmp+1;
    }

    // delete nChar(th) new line char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\n'))
    {
        if(((pszTmp - pszBuffer) % nChar) == 0)
        {
            memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1)));
            pTextEdit->nLen --;
            pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
        }
        else
            pszBuffer = pszTmp+1;
    }

    EQC_SET_DIRTY(hControl, FALSE);
    pTextEdit->pCursor = (CURSOR_T*)EQS_Malloc(sizeof(CURSOR_T));
    pTextEdit->hCursorTimer = NULL;
    pTextEdit->pScrollBar = NULL;
    pTextEdit->pBrowseScrollBar = NULL;
    pTextEdit->nKoreanState = 0;

    EQS_SendEvent(EQE_APP_DRAW, (BYTE)hControl, 0);

    return hControl;
}

 

/*
**---------------------------------------------------------------------------------
**  외부 호출용 함수
**---------------------------------------------------------------------------------
*/

UINT16 EQC_GetTextEditData(HCONTROL hControl, BYTE *pszBuf)
{

    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);
    pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
    STRCPY(pszBuf, pTextEdit->pszBuffer);
    
    return(pTextEdit->nLen);
}

UINT16 EQC_SetTextEditData(HCONTROL hControl, BYTE *pszBuf)
{
    BYTE *pszTmp, *pszBuffer;
    UINT16 nStrLen;

    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    if(!pszBuf)
        return 0;

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    pTextEdit->nLen = 0;
    pTextEdit->nPos = 0;

    // String copy routine
    nStrLen = STRLEN(pszBuf);
    if(nStrLen > pTextEdit->nMaxLen)
    {
        nStrLen = pTextEdit->nMaxLen;

        if(IsMiddleOfMultiByte(pszBuf, (UINT16)nStrLen)) // two-byte code
            nStrLen --;
    }

    sAddStr(hControl, pszBuf, nStrLen, FALSE, FALSE);
    pTextEdit->nPos = 0;

    // Delete '\r' char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\r'))
    {
        memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1))+1);
        pTextEdit->nLen --;
        pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
    }

    // delete nChar(th) new line char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\n'))
    {
        if(((pszTmp - pszBuffer) % pTextEdit->nChar) == 0)
        {
            memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1)));
            pTextEdit->nLen --;
            pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
        }
        else
            pszBuffer = pszTmp+1;
    }

    EQC_SET_DIRTY(hControl, FALSE);

    if(pTextEdit->pszFormat != NULL)
    {
        FormatType = sGetCurrentFormat((UINT16)(pTextEdit->nPos));
        if(FormatType != TFT_Error)
            sSetDefaultFormatMode(FormatType);
    }

    /* jrkwon 2000.01.28 */
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );

    return(pTextEdit->nLen);
}


UINT16 EQC_SetTextEditDataPos(HCONTROL hControl, BYTE *pszBuf, UINT16 nPos)
{
    BYTE *pszTmp, *pszBuffer;
    UINT16 nStrLen;

    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    if(!pszBuf)
        return 0;

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    // String copy routine
    nStrLen = STRLEN(pszBuf);
    if(nStrLen + pTextEdit->nLen > pTextEdit->nMaxLen)
    {
        return 0;
/* 중간에 삽입하는 루틴인데 일단 막음.
        nStrLen = pTextEdit->nMaxLen - pTextEdit->nLen;

        if(IsMiddleOfMultiByte(pszBuf, (UINT16)nStrLen)) // two-byte code
            nStrLen --;
*/
    }

    pTextEdit->nPos = nPos;
    sAddStr(hControl, pszBuf, nStrLen, FALSE, TRUE);

    // Delete '\r' char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\r'))
    {
        memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1))+1);
        pTextEdit->nLen --;
        pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
    }

    // delete nChar(th) new line char.
    pszBuffer = pTextEdit->pszBuffer;
    while(pszTmp = STRCHR(pszBuffer, '\n'))
    {
        if(((pszTmp - pszBuffer) % pTextEdit->nChar) == 0)
        {
            memmove(pszTmp, (void*)(pszTmp+1), STRLEN((BYTE*)(pszTmp+1)));
            pTextEdit->nLen --;
            pTextEdit->pszBuffer[pTextEdit->nLen] = '\0';
        }
        else
            pszBuffer = pszTmp+1;
    }

    EQC_SET_DIRTY(hControl, FALSE);

    /* jrkwon 2000.01.28 */
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );

    return(pTextEdit->nPos);
}

void EQC_SetTextEditPos(HCONTROL hControl, UINT16 nPos)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    pTextEdit->nPos = nPos;
    l_setPos = nPos;

    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );
}

UINT16 EQC_GetTextEditLen(HCONTROL hControl)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);
    
    return pTextEdit->nLen;
}

UINT16 EQC_GetTextEditPos(HCONTROL hControl)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);
    
    return pTextEdit->nPos;
}

LOCAL BOOL sCheckFormatString(BYTE *pszFormat)
{
    UINT16 nLen, i, nOffset;
    TEXT_FORMAT_TYPE_E Type;
    BYTE *szTmp, nTmp;

    nLen = sGetFormatLength(pszFormat, STRLEN(pszFormat), 0);
    if(sGetStringLength(pTextEdit->nLen > nLen) > nLen)
        return FALSE;

    for(i = 0; i < pTextEdit->nLen ;)
    {
        Type = sGetCurrentFormat(i);

        switch(Type){
        case TFT_A:
            if(pTextEdit->pszBuffer[i] < 'A' || pTextEdit->pszBuffer[i] > 'Z')  // Upper Only
                return FALSE;
            break;

        case TFT_a:
            if(pTextEdit->pszBuffer[i] < 'a' || pTextEdit->pszBuffer[i] > 'z')  // Lower Only
                return FALSE;
            break;

        case TFT_N:
            if(pTextEdit->pszBuffer[i] < '0' || pTextEdit->pszBuffer[i] > '9')  // Number Only
                return FALSE;
            break;

        case TFT_X:
            if(pTextEdit->pszBuffer[i] >= '0' && pTextEdit->pszBuffer[i] <= '9')    // Not Number
                return FALSE;

            if(pTextEdit->pszBuffer[i] >= 'a' && pTextEdit->pszBuffer[i] <= 'z')    // Not Lower
                return FALSE;
            break;

        case TFT_x:
            if(pTextEdit->pszBuffer[i] >= '0' && pTextEdit->pszBuffer[i] <= '9')    // Not Number
                return FALSE;

            if(pTextEdit->pszBuffer[i] >= 'A' && pTextEdit->pszBuffer[i] <= 'Z')    // Not Upper
                return FALSE;
            break;

        case TFT_M:
        case TFT_m:
            break;
        }

        if(pTextEdit->pszBuffer[i] > 0x80)
            i += 2;
        else
            i ++;

        nOffset = 0;
        while(TRUE)
        {
            szTmp = STRCHR((BYTE*)(pTextEdit->pszFormat + nOffset), '\\');
            if(szTmp == NULL)
                break;

            nTmp = szTmp - pTextEdit->pszFormat;
            nLen = sGetFormatLength(pTextEdit->pszFormat, nTmp, 0);
            nOffset = nTmp+1;

            if(i == nLen)
            {
                i++;
                break;
            }
        }
    }

    return TRUE;
}

void EQC_SetFormatString(HCONTROL hControl, BYTE *pszFormat)
{
    TEXT_FORMAT_TYPE_E Type;

    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(!STRLEN(pszFormat))
        return;

    //pTextEdit->nMaxLen = nLen;
    pTextEdit->pszFormat = (BYTE*)EQS_Malloc((SIZE_T)(STRLEN(pszFormat)+1));
    STRCPY(pTextEdit->pszFormat, pszFormat);

    if(sCheckFormatString(pszFormat) == FALSE)
    {
        // clear string buffer
        pTextEdit->pszBuffer[0] = '\0';
        pTextEdit->nPos = 0;
        pTextEdit->nLen = 0;
    }

    // Change Current Sate 
    Type = sGetCurrentFormat(pTextEdit->nPos);

    switch(Type){
    case TFT_A:
        pTextEdit->EditMode = TFT_AMode;
        break;
    case TFT_a:
        pTextEdit->EditMode = TFT_aMode;
        break;
    case TFT_N:
        pTextEdit->EditMode = TFT_NMode;
        break;
    case TFT_X:
        pTextEdit->EditMode = TFT_XMode;
        break;
    case TFT_x:
        pTextEdit->EditMode = TFT_xMode;
        break;
    case TFT_M:
        pTextEdit->EditMode = TFT_MMode;
        break;
    case TFT_m:
        pTextEdit->EditMode = TFT_mMode;
        break;
    }

    pTextEdit->FormatType = Type;
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );
}

void EQC_SetFormatLength(HCONTROL hControl, UINT8 nLen)
{
    TEXT_FORMAT_TYPE_E Type;
    UINT16 nFormatLen;

    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    if(pTextEdit->pszFormat == NULL || nLen == 0)
        return;

    nFormatLen = sGetFormatLength(pTextEdit->pszFormat, STRLEN(pTextEdit->pszFormat), 0);
    if(nLen < nFormatLen)
        pTextEdit->nFormatLen = nLen;
    else
        pTextEdit->nFormatLen = nFormatLen;
}

void EQC_SetTextEditModeSequence(HCONTROL hControl, EDIT_MODE_SELECT_T modeSeq)
{
    ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

	pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    pTextEdit->EditMode = modeSeq;
    EQS_SendEvent( EQE_APP_DRAW, (WORD) hControl, 0 );
}


void EQC_SetSKYHangulMode(BOOL fSkyHangul)
{
    l_fSkyHangul = fSkyHangul;
}

UINT16   EQS_API EQC_GetTextEditLine(HCONTROL hControl)
{
	ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);
    //g_nMaxCharPerLine = EQC_GET_DX(hControl) / EQS_CharWidth('A');
    g_nMaxCharPerLine = pTextEdit->nChar;

    return (sGetY(hControl) +1);
}

void   EQS_API EQC_FitTextEditLine(HCONTROL hControl)
{
	ASSERT(EQC_GET_TYPE(hControl) == CT_TEXT_EDIT);

    pTextEdit = (TEXT_EDIT_T *) EQC_GET_INSTANCE(hControl);

    g_nMaxCharPerLine = EQC_GET_DX(hControl) / EQS_CharWidth('A');

    pTextEdit->nChar = g_nMaxCharPerLine;
    pTextEdit->nLine = sGetY(hControl)+1;

    EQC_SET_DY(hControl, (UINT16)((sGetY(hControl)+1) * EQS_CharHeight('A')));
    EQS_SendEvent(EQE_APP_DRAW, NULL, (DWORD)NULL);     
}

/*
**---------------------------------------------------------------------------
**  END.
**---------------------------------------------------------------------------
*/
