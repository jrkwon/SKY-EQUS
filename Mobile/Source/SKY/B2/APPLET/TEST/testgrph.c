/*
** ===========================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     testgrph.c
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
** 2000-09-29 jrkwon     Created.
**
** ===========================================================================
*/

/* 
**----------------------------------------------------------------------------
**  Includes
**---------------------------------------------------------------------------- 
*/
#include <math.h>
#include "eqs.h"
#include "eqa.h"

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

LOCAL void EQS_API sDrawSinCosGraph(void)
{
    POINT Origin;
    double pi = 3.1415926535;
    double x, y;
    COORD prex, prey;
    COORD yHalf;

    EQS_GetDC(TRUE);
    {
        yHalf = (EQS_GetMaxY()/2);
        Origin.x = 0;
        Origin.y = -yHalf;

        EQS_Line(0,yHalf, EQS_GetMaxX(), yHalf);
        EQS_Line(0, 0, 0, EQS_GetMaxY());
        EQS_SetCoordinates(CS_MATH);
        EQS_SetOrigin(&Origin);

        /* Draw sine graph */
        MoveTo(0,0);
        prex = prey = 0;
        for(x = 0.0; x < 2*pi; x+= 0.05)
        {
            y = sin(x);
            prex = (COORD)(x*17), prey = (COORD)(y*yHalf);
            EQS_SetPixel((COORD)(x*17), (COORD)(y*yHalf), YELLOW);
        }

        /* Draw cosine graph */
        MoveTo(0,0);
        prex = prey = 0;
        for(x = 0.0; x < 2*pi; x+= 0.05)
        {
            y = cos(x);
            prex = (COORD)(x*17), prey = (COORD)(y*yHalf);
            EQS_SetPixel((COORD)(x*17), (COORD)(y*yHalf), WHITE);
        }

        EQS_SetCoordinates(CS_SCREEN);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sDrawPrimitives(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_SetForeColor(YELLOW);
        EQS_Circle(40, -10, 15);
        EQS_SetForeColor(RED);
        EQS_Circle(40, 10, 15);
    
        EQS_SetForeColor(BLUE);
        EQS_Line(8, 23, 70, 10);
        EQS_SetForeColor(WHITE);
        EQS_Line(0, 48, 60, 70);
        
        EQS_Rectangle(5, 72, 115, 91);
        
        EQS_SetForeColor(BLUE);
        EQS_Ellipse(40, 30, 20, 15);

        EQS_FillRectangle(70, 20, 100, 50, CYAN);
        EQS_FillRectangle(70, 50, 110, 70, LIGHTBLUE);
    }
    EQS_ReleaseDC();
}

/*LOCAL void EQS_API sEQA_Primitive(EQS_EVENT *pInEvent)
{
    void EQS_API (*pfnDraw[2])(void) = {
        sDrawSinCosGraph,
        sDrawPrimitives
    };
    STATIC INT8 i = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_WAP:
        if(i > 0)
            i--;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_MSG:
        if(i < 1)
            i++;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}
*/

enum tagTEST_CONTROL {
    TC_STT_NAME,
    TC_TED_NAME,
    TC_BTN_OK,
    TC_BTN_CANCEL
};
typedef enum tagTEST_CONTROL    TEST_CONTROL;

LOCAL void EQS_API sEQA_Primitive(EQS_EVENT *pInEvent)
{
    HCONTROL hStaticName, hEdit;
    POINT Point;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
        EQS_SetTextDrawMode(TDM_REVERSE);
        EQS_SetTextColor(YELLOW);
        
        /* ---> */
        //EQS_StartFormatter();

        hStaticName = EQC_StaticText(0, 0, (BYTE *)"¿Ã∏ß");
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 30, EHA_NO, EVA_NO, FALSE);
        //EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_NO, EVA_NO, FALSE);

        //hEdit = EQC_TextEdit((BYTE*)"±‚∫ª", 0, EQS_KOREAN, 5, TE_DEFAULTS, FT_BIG
        //              , Point, 5, 1, FT_BIG);
        hStaticName = EQC_StaticText(0, 0, (BYTE *)"πŸ∫∏");
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 50, EHA_NO, EVA_NO, FALSE);
        //EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_NO, EVA_NO, FALSE);

        //--hStaticName = EQC_StaticText("∂À∞≥", 0, 0, FT_BIG, 0);
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 50, EHA_NO, EVA_NO, FALSE);
        //--EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_NO, EVA_NO, FALSE);
        //EQS_EndLine();

        hStaticName = EQC_StaticText(0, 0, (BYTE *)"¿Ã∏ß");
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 30, EHA_NO, EVA_NO, FALSE);
        //EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_LEFT, EVA_NO, TRUE);

        hStaticName = EQC_StaticText(0, 0, (BYTE *)"πŸ∫∏");
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 50, EHA_NO, EVA_NO, FALSE);
        //EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_NO, EVA_NO, TRUE);

        hStaticName = EQC_StaticText(0, 0, (BYTE *)"∂À∞≥");
        //EQS_SetControlPosition(hStaticName,  ES_PERCENT, 50, EHA_NO, EVA_NO, FALSE);
        //EQS_SetControlPosition(hStaticName,  ES_AUTO, EQS_NA, EHA_NO, EVA_NO, FALSE);
        //EQS_EndLine();

        //EQS_EndFormatter();
        /* <--- */
        
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_WAP:
        break;
	case EQE_KEY_MSG:
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}


LOCAL void EQS_API sDrawColoredText(void)
{
    EQS_GetDC(TRUE);
    {
        COLOR aColor[] = {
            RED, LIGHTRED, YELLOW, GREEN, LIGHTBLUE, BLUE, RED+BLUE
        };
        FONT_TYPE aFontType[] = {
            FT_MED, FT_BIG, FT_BIG, FT_MED, FT_MED, FT_MED, FT_MED
        };
        CONST SIZE *pSize;
        INT8 i;

        for(i = 0; i < 7; i++)
        {
            EQS_SetFontType(aFontType[i]);
            EQS_SetTextColor(aColor[i]);
            pSize = EQS_GetFontSize(aFontType[i], FK_ASCII);
            EQS_TextOut(0, i*pSize->yHeight, TKN_TEST_TEXT_STRING);
        }
        EQS_SetFontType(FT_BIGBIG);
        EQS_TextOutStr(0, 0, (BYTE*)"012345678");
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sDrawTextAttr(void)
{
    struct tagTOKEN_TDM {
        EQS_TOKEN Token;
        TEXT_DRAW_MODE TDM;
    };
    struct tagTOKEN_TDM TokenTDM[] = {
        { TKN_TEST_TEXT_DIM, TDM_GRAY },
        { TKN_TEST_TEXT_BOLD, TDM_BOLD },
        { TKN_TEST_TEXT_REV, TDM_REVERSE },
        { TKN_TEST_TEXT_UNDER, TDM_UNDERLINE },
        { TKN_TEST_TEXT_SHADOW, TDM_SHADOW },
        { TKN_TEST_TEXT_OUTLINE, TDM_OUTLINE }
    };
    INT8 i;
    CONST SIZE *pSize;

    EQS_GetDC(TRUE);
    {
        for(i = 0; i < 6; i++)
        {
            //EQS_SetFontType(aFontType[i]);
            pSize = EQS_GetFontSize(FT_MED, FK_ASCII);
            EQS_TextOutTDM(0, i*pSize->yHeight, TokenTDM[i].Token, TokenTDM[i].TDM);
        }
    }
    EQS_ReleaseDC();
}


LOCAL void EQS_API sEQA_Text(EQS_EVENT *pInEvent)
{
    void EQS_API (*pfnDraw[2])(void) = {
        sDrawColoredText,
        sDrawTextAttr
    };
    STATIC INT8 i = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_WAP:
        if(i > 0)
            i--;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_MSG:
        if(i < 1)
            i++;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

#define TKN_IMG_NAME_CALCULATOR     "WP_CAL.EBI"
#define TKN_IMG_NAME_ROCK           "ROCK.EBI"
#define TKN_IMG_NAME_ROSES          "ROSE.EBI"

LOCAL ROM BYTE abCalculator[] = {
#include TKN_IMG_NAME_CALCULATOR
};

LOCAL ROM BYTE abRock[] = {
#include TKN_IMG_NAME_ROCK
};

LOCAL ROM BYTE abRoses[] = {
#include TKN_IMG_NAME_ROSES
};

ROM BITMAP CalculatorImage = {
    { BT_IMAGE, { 120, 80 }, EQS_NA, 2 }, abCalculator
}; 

ROM BITMAP RosesImage = {
    { BT_IMAGE, { 120, 120 }, EQS_NA, 8 }, abRoses
}; 

ROM BITMAP RockImage = {
    { BT_IMAGE, { 120, 120 }, EQS_NA, 8 }, abRock
}; 

LOCAL void EQS_API sDraw4GImage(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &CalculatorImage);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sDraw256ImageRoses(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &RosesImage);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sDraw256ImageRock(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &RockImage);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sEQA_Image(EQS_EVENT *pInEvent)
{
    void EQS_API (*pfnDraw[3])(void) = {
        sDraw4GImage,
        sDraw256ImageRoses,
        sDraw256ImageRock
    };
    STATIC INT8 i = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_WAP:
        if(i > 0)
            i--;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_MSG:
        if(i < 2)
            i++;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

LOCAL void EQS_API sDrawDrawModeOR(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &RosesImage);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextAlignHoriz(TAH_CENTER);
        EQS_TextOut(0, 0, TKN_TEST_TEXT_STRING);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sDrawDrawModeCOPY(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &RosesImage);
        EQS_SetDrawMode(DM_COPY);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextAlignHoriz(TAH_CENTER);
        EQS_TextOut(0, 0, TKN_TEST_TEXT_STRING);
    }
    EQS_ReleaseDC();
}

LOCAL void EQS_API sEQA_DrawMode(EQS_EVENT *pInEvent)
{
    void EQS_API (*pfnDraw[2])(void) = {
        sDrawDrawModeOR,
        sDrawDrawModeCOPY
    };
    STATIC INT8 i = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_WAP:
        if(i > 0)
            i--;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	case EQE_KEY_MSG:
        if(i < 1)
            i++;
        EQS_ClearScreen();
        pfnDraw[i]();
        EQS_UpdateDisplay();
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

LOCAL void EQS_API sEQA_Multilang(EQS_EVENT *pInEvent)
{
    STATIC HCONTROL hStatic;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_SetCountry(EQS_GetCountry() == COUNTRY_USA ? COUNTRY_KOR : COUNTRY_USA);
        EQS_End();
		break;

	case EQE_APP_END:
        break;
	default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}

WINDOW gWin1;
SCROLL_BAR gScrollBar1, gScrollBar2;

LOCAL void EQS_API sEQA_GUI(EQS_EVENT *pInEvent)
{
    RECT Rect1 = { 10, 35, 110, 80 };
//    WINDOW_CAPTION Caption = {
//       (BYTE*)"∏∂¬°∞° Z", YELLOW, BLUE
//    };

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

        EQS_ClearScreen();
        EQS_PutBitmap(0, 0, &RockImage);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextWrap(TRUE);
        EQS_TextOut(0,0, TKN_TEST_TEXT_MSG);

        EQS_PreOpenWindow(&gWin1, WS_POPUP, (BYTE *)"∏∂¬°∞°Z"); //WS_FULLSCREEN||WS_BORDER|
        EQS_OpenWindow(&gWin1, &Rect1, TRUE);
        EQS_AddScrollBar(&gWin1, &gScrollBar1, SBT_VERT, Rect1.xLeft, Rect1.yTop, Rect1.yBottom-Rect1.yTop+1,
                        0, 99, 10, 20);
        EQS_AddScrollBar(&gWin1, &gScrollBar2, SBT_HORIZ, Rect1.xLeft, Rect1.yBottom-5, Rect1.xRight-Rect1.xLeft+1,
                         0, 99, 90, 10);
        EQS_DrawScrollBar(&gScrollBar1);
        EQS_DrawScrollBar(&gScrollBar2);
        EQS_UpdateDisplay();

        EQS_StartTimer(EQS_TIMER_1, 3000, FALSE);
		break;

	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

    case EQE_TM_TIMER1:
        EQS_CloseWindow(&gWin1);
        EQS_UpdateDisplay();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
    }
}

LOCAL void EQS_API sEQA_SlidingWindow(EQS_EVENT *pInEvent)
{
    STATIC HTIMER hTimer;
    STATIC INT8 nCnt = 0;
    static COORD yOffset = 0;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);

        EQS_ClearScreen();
        EQS_PutBitmap(0, 0, &RockImage);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextWrap(TRUE);
        EQS_TextOut(0,0, TKN_TEST_TEXT_MSG);
        EQS_UpdateDisplay();

        hTimer = EQS_StartTimer(EQS_TIMER_1, 500, TRUE);
		break;

	case EQE_APP_END:
        EQS_SetDrawMode(DM_COPY);
        EQS_StopTimer(hTimer);
        EQS_ReleaseDC();
        break;
    case EQE_KEY_CENTER:
        EQS_End();
        break;

    case EQE_TM_TIMER1:
        nCnt++;
        yOffset++;

        SetSlidingWindowState(FALSE);
        EQS_PutBitmap(0, 0, &RockImage);

        SetSlidingWindowState(TRUE);
        SetSlidingWindowOffsetY(yOffset);

        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextWrap(TRUE);
        EQS_TextOut(0,0, TKN_TEST_TEXT_MSG);

        SetSlidingWindowState(FALSE);
        EQS_UpdateDisplay();

        if(nCnt > 20)
            EQS_End();
        break;

	default:
		EQS_DefEventProc(pInEvent);
		break;
    }
}

CONST LOCAL BYTE *pabTestMenu[7];

EQS_Applet afnTestApplet[] = {	
    sEQA_Primitive,
    sEQA_Text,
    sEQA_Image,
    sEQA_DrawMode,
    sEQA_Multilang,
    sEQA_GUI,
    sEQA_SlidingWindow
};

LOCAL void EQS_API sInitMenuString(void)
{
    EQS_TOKEN TestToken[] = {
        TKN_TEST_PRIMITIVE, 
        TKN_TEST_TEXT,      
        TKN_TEST_IMAGE,     
        TKN_TEST_DRAWMODE,  
        TKN_TEST_MULTILANG, 
        TKN_TEST_GUI,       
        TKN_TEST_SLIDING
    };
    INT8 i;

    for(i = 0; i < sizeof(TestToken)/sizeof(TestToken[0]); i++)
        pabTestMenu[i] = EQS_GET_TOKEN(TestToken[i]);
}

void EQS_API EQA_MainTestGraph(EQS_EVENT *pInEvent)
{
	static HCONTROL hHnd;
	UINT8	iKey;

	switch (pInEvent->EventID)
	{
	case EQE_APP_START:
        EQS_GetDC(TRUE);
        //EQS_SetCountry(COUNTRY_KOR);
        sInitMenuString();
//		hHnd = EQC_ScrollList((CHAR**)pabTestMenu, 7, 7, 6, 6, 18, 7, 0, SCROLL_DEFAULTS);
		//EQC_CMDConsole(CONSOLE_TYPE_SEL_CANCEL, CMD_DEFAULTS);
        EQS_SetIOCActive(hHnd);

		break;

    case EQE_APP_RESUME:
        EQS_DeleteControl(hHnd);
        sInitMenuString();
//		hHnd = EQC_ScrollList((CHAR**)pabTestMenu, 7, 7, 6, 6, 18, 7, 0, SCROLL_DEFAULTS);
        EQS_SetIOCActive(hHnd);
        break;
	case EQE_APP_END:
        EQS_ReleaseDC();
        break;

	case EQE_KEY_1:
	case EQE_KEY_2:
	case EQE_KEY_3:
	case EQE_KEY_4:
	case EQE_KEY_5:
	case EQE_KEY_6:
	case EQE_KEY_7:
	case EQE_KEY_8:
	//case EQE_KEY_9:
		iKey = pInEvent->EventID - EQE_KEY_1;
		EQC_SetScrollPhysicalIndex(hHnd, iKey);

	//case EQE_SOFTKEY_SELECT:
    case EQE_IO_SCROLL_LIST_SELECT:
		//EQS_StartChild((EQS_Applet*)afnTestApplet[pInEvent->dwParam]);
        break;

    default:
		EQS_DefEventProc(pInEvent);
		break;
	}
}
