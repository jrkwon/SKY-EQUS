#include <math.h>
#include "eqs.h"
#include "testgrph.h"

#define SKY_COLOR_SPACE          CS_256COLOR //CS_4GRAY
#define SKY_SCREEN_WIDTH         120                  /* screen width */
#define SKY_SCREEN_HEIGHT        127                  /* screen height */
#define SKY_SCREEN_BUFFER_WIDTH  (SKY_SCREEN_WIDTH+8) /* screen buffer width */
#define SKY_SCREEN_BUFFER_HEIGHT (SKY_SCREEN_HEIGHT) /* screen buffer height */
#define SKY_SOUND_INTERFACE      SKY_3100_SOUND_INTERFACE   

#define SKY_SUB_COLOR_SPACE          CS_BW
#define SKY_SUB_SCREEN_WIDTH         90                     /* screen width */
#define SKY_SUB_SCREEN_HEIGHT        16                     /* screen height */
#define SKY_SUB_SCREEN_BUFFER_WIDTH  (SKY_SUB_SCREEN_WIDTH+6) /* screen buffer width */
#define SKY_SUB_SCREEN_BUFFER_HEIGHT (SKY_SCREEN_HEIGHT)    /* screen buffer height */


SCREEN_INFO ScreenInfo;

BYTE abMainScreenBuffer[(SKY_SCREEN_BUFFER_WIDTH/8)*SKY_SCREEN_BUFFER_HEIGHT*SKY_COLOR_SPACE];
BYTE abSubScreenBuffer[(SKY_SUB_SCREEN_BUFFER_WIDTH/8)*SKY_SUB_SCREEN_BUFFER_HEIGHT*SKY_SUB_COLOR_SPACE];

VOID OpenNUIGraphics(int nColor)
{
    SCREEN_INFO MainScreenInfo = {
        SKY_COLOR_SPACE,
        { SKY_SCREEN_WIDTH, SKY_SCREEN_HEIGHT }
    };
    SCREEN MainScreen;
//    SCREEN_INFO SubScreenInfo = {
//        SKY_SUB_COLOR_SPACE,
//        { SKY_SUB_SCREEN_WIDTH, SKY_SUB_SCREEN_HEIGHT }
//    };
    SCREEN SubScreen;
    int i; 

    /* ---------------------
    ** LCD 화면 정보 설정 
    */
    MainScreen.Info     = MainScreenInfo;
//    SubScreen.Info      = SubScreenInfo;
    MainScreen.pbBuffer = abMainScreenBuffer;
//    SubScreen.pbBuffer  = abSubScreenBuffer;

    /* main screen */
    OpenGraph(&MainScreen, SI_MAIN_SCREEN);
    SetFontType(INIT_FONT_TYPE);    /* system font */
    EQS_SetTheme(INIT_COLOR_SCHEME, INIT_FONT_TYPE, INIT_SCROLLBAR_SIZE);
    EQS_InitDC();

    /* sub screen */
//    OpenGraph(&SubScreen, SI_SUB_SCREEN);
//    SetFontType(INIT_FONT_TYPE);    /* system font */
//    SelectScreen(SI_SUB_SCREEN);

//    ScreenInfo.ColorSpace = (nColor == 0) ? CS_4GRAY: CS_256COLOR;
/*    ScreenInfo.ColorSpace = CS_BW;
    ScreenInfo.Size.xWidth = SCREEN_WIDTH;
    ScreenInfo.Size.yHeight = SCREEN_HEIGHT;
    if(!IS_RESULT_OK(OpenGraph(&ScreenInfo)))
        return;*/
}

VOID CloseNUIGraphics(VOID)
{
    CloseGraph(SI_MAIN_SCREEN);
}

BYTE* GetLCDBuffer(VOID)
{
//    SCREEN_INDEX_E Screen;
    BYTE* pBuf;

//    Screen = SelectScreen(SI_SUB_SCREEN);
    /* get the screen buffer */
    pBuf = GetScreenBuffer();
    
//    SelectScreen(Screen);

    return pBuf;
}

VOID ClearNUIScreen(VOID)
{
    ClearScreen();
}

/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
/* Draw bitmap imgae test                                                   */
/* ************************************************************************ */
/* ************************************************************************ */
/* ************************************************************************ */
#define TKN_IMG_NAME_CALCULATOR     "WP_CAL.EBI"
#define TKN_IMG_NAME_FACES          "FACES.EBI"
#define TKN_IMG_NAME_ROSES          "ROSE.EBI"

LOCAL ROM BYTE abCalculator[] = {
#include TKN_IMG_NAME_CALCULATOR
};

LOCAL ROM BYTE abFaces[] = {
#include TKN_IMG_NAME_FACES
};

LOCAL ROM BYTE abRoses[] = {
#include TKN_IMG_NAME_ROSES
};

ROM BITMAP FacesImage = {
    { BT_IMAGE, { 120, 78 }, 119, 8 }, abFaces
};

ROM BITMAP CalculatorImage = {
    { BT_IMAGE, { 120, 80 }, 119, 2 }, abCalculator
}; 

ROM BITMAP RosesImage = {
    { BT_IMAGE, { 120, 120 }, 118, 8 }, abRoses
}; 

VOID NUIGraphicsDemo256Image()
{
/*    BYTE *pszMsg[] = {
        "나의 모든 것의 반을",
        "당신의 반을",
        "주십시오",
        "당신의 생각의 반",
        "당신의 시간의 반",
        "당신의 생활의 반",
        "당신의",
        "모든 것의 반을",
        "",
        "당신이 주신 반을",
        "당신을",
        "사랑하는 것처럼",
        "당신같이",
        "사랑하겠습니다",
        "",
        "그것이",
        "아름다운", 
        "추억이 아닌",
        "되돌리기",
        "싫은 아픔의", 
        "시간들이었다고", 
        "할지라도",
        "당신의 모든 것은",
        "당신만큼이나",
        "사랑스럽습니다",
        "",
        "반쪼가리 난...",
        "당신이 필요합니다",
        "당신의 아픔까지도",
        "당신의 슬픔까지도",
        "내게는 소중합니다",
        "",
        "당신의 반을",
        "내게 주십시오",
        NULL
    };
    INT8 i;
    static COORD yOffset = 0;

    SetSlidingWindowState(FALSE);
    PutBitmap(0, 0, &RosesImage);
    
    SetSlidingWindowState(TRUE);
    SetSlidingWindowOffsetY(yOffset);*/

    PutBitmap(0, 0, &FacesImage);
/*    SetDrawMode(DM_OR);
    SetTextColor(YELLOW);
    SetTextAlignHoriz(TAH_CENTER);
    for(i = 0; pszMsg[i]; i++)
        TextOutStr(GetMaxWidth()/2, i*13, pszMsg[i]);

    if(++yOffset > i*13)
        yOffset = 0;

    SetSlidingWindowState(FALSE);*/
}

VOID NUIGraphicsDemo4GImage()
{
    SetDrawMode(DM_OR);
    TextOutStr(0, 40,  "권재락 만세다");
    PutBitmap(0, 0, &CalculatorImage);
    SetDrawMode(DM_COPY);
    SetTextAlignHoriz(TAH_CENTER);
    SetTextAlignVert(TAV_CENTER);
    TextOutStr(60, 48,  "하하하 호호호");
    SetDrawMode(DM_XOR);
    TextOutStr(60, 60,  "하하하 호호호");
    ClearTextAlign();
    //TextOutStr(60, 16,  "호호호");
}
//VOID EQS_API EQA_Calculator(EQS_EVENT *pInEvent);
//EQS_EVENT Dummy;


BYTE* pbGetPutBuffer;


VOID NUIGraphicsDemoPrimitive()
{
    POINT Origin = { 0, -48 };
    double pi = 3.1415926535;
    double x, y;
    COORD prex, prey;
    //POINT Origin = { 0, 48 };
  //  EQA_Calculator(&Dummy);
    if(pbGetPutBuffer)
    {
        EQS_PutRegionImage(0,0,pbGetPutBuffer);
        EQS_Free(pbGetPutBuffer);
    }
    Line(0,48,120,48);
    Line(0,0,0,96);
    SetCoordinates(CS_MATH);
    SetOrigin(&Origin);

    Circle(40, -10, 15);
    Circle(40, 10, 15);
    
    MoveTo(0,0);
    prex = prey = 0;
    for(x = 0.0; x < 2*pi; x+= 0.05)
    {
        y = sin(x);
        //Line(prex, prey, (COORD)(x*17), (COORD)(y*48));
        prex = (COORD)(x*17), prey = (COORD)(y*48);
        //LineTo((COORD)(x*17), (COORD)(y*48));
        SetPixel((COORD)(x*17), (COORD)(y*48), WHITE);
        //MoveTo((COORD)(x*17), (COORD)(y*48));
    }
    SetCoordinates(CS_SCREEN);

    Line(8, 23, 70, 10);
    Line(0, 48, 60, 70);
    //Rectangle(5, 72, 115, 91);
    Circle(40, -10, 15);
    Circle(40, 10, 15);
    //Ellipse(40, 30, 20, 15);
    //FillRectangle(70, 20, 100, 50, LIGHTGRAY);
    //FillRectangle(70, 50, 110, 70, DARKGRAY);
}

LOCAL ROM BYTE abDayBar[] = {
#include "Day10215-1.ebi"
};

ROM BITMAP DayBarImage = {
    { BT_IMAGE, { 104, 15 }, EQS_NA, 2 }, abDayBar
};


VOID NUIGraphicsDemoTextOut(VOID)
{
    //SetBackColor(RED);
    ClearScreen();
    //Rectangle(0, 119, 0, 4);
    //RoundRectangle(0, 0, 119, 95, 20); 
    //RoundRectangle(5, 5, 119-5, 95-5, 20);

    /*Rectangle(13, 0, 50, 20);
    TextOutStr(1, 0,     "가각간갇갈갉갊"); 
    TextOutStr(1, 16,    "괌괍괏광괘괜괠");
    TextOutStr(1, 16*2,  "뇟뇨녹굔뇰뇹뇻");
    TextOutStr(1, 16*3,  "벙벚베벡벤벧벨");
    TextOutStr(1, 16*4,  "샥샨샬샴샵샷샹");
    TextOutStr(1, 16*5,  "혤혭호혹혼홀홅");*/


//    SetTextDrawMode(TDM_REVERSE);
//    TextOutStr(0, 0,  "반전");
    //ClearTextDrawMode(TDM_ALL);
    //SetTextDrawMode(TDM_BOLD);
    //SetTextDrawMode(TDM_GRAY);
    //TextOutStr(0, 0, "ABCDEFG");
    //SetDrawMode(DM_XOR);
    //FillRectangle(0, 0, 119, 20, BLACK);

    //SetForeColor(LIGHTGRAY);
//    Line(2, 1, 118, 1);
    /*Line(2, 1, 2, 13);
    Line(5, 1, 5, 13);
    Line(31, 1, 31, 13);
    Line(50, 1, 50, 13);*/
    //Rectangle(2, 1, 118, 13);

    SetTextDrawMode(TDM_REVERSE);
    TextOutStr(0, 0, "바");//가나다라마바사");
    TextOutStr(1, 13, "abcd가나다라마바사");
    TextOutStr(2, 26, "가나다라마바사");
    TextOutStr(3, 39, "가나다라마바사");
    TextOutStr(4, 52, "가나다라마바사");
    TextOutStr(5, 65, "가나다라마바사");
/*    PutBitmap(10, 15, &DayBarImage);
    //TextOutStr(0,15, "일 월 화 수 목 금 토 일");
    SetFontType(FT_SMALL);
    TextOutStr(10, 33, " 1  2  3  4  5  6  7");
    TextOutStr(10, 43+2, " 8  9 10 11 12 13 14");
    TextOutStr(10, 53+4, "15 16 17 18 19 20 21");
    TextOutStr(10, 63+6, "22 23 24 25 26 27 28");
    TextOutStr(10, 73+8, "29 30               ");
    TextOutStr(10, 83+10, "12345678901234567890");
    SetFontType(FT_MED);*/
//    Rectangle(6, 18, 110, 110);
//    Line(6, 32, 110, 32);
//    TextOutStr(2, 26, "가");
//    TextOutStr(3, 39, "가");

    //s4G_HorizLine(0, 118, 19);
    //s4G_VertLine(2, 1, 13);
//    TextOutStr(3, 39, "ABCDEFGhijklmn");
//    TextOutStr(4, 52, "ABCDEFGhijklmn");
/*    SetTextColor(GRAY);
    TextOutStr(0, 0,  "흐리게");
    SetTextColor(LIGHTGRAY);
    TextOutStr(0, 16,  "흐리게");
    SetTextColor(WHITE);*/
    //TextOutStrTDM(0, 16,  "흐리게", TDM_GRAY);
    //ClearTextDrawMode(TDM_BOLD);
    //ClearTextDrawMode(TDM_ALL);
    //SetTextDrawMode(TDM_SHADOW);
#if 0
    TextOutStrTDM(0, 32, "그림자", TDM_SHADOW);
    //TextOutStr(42, 32, "0123456");
    //ClearTextDrawMode(TDM_SHADOW);
    //SetTextDrawMode(TDM_OUTLINE);
    TextOutStrTDM(0, 48,  "외곽선", TDM_OUTLINE);
    //ClearTextDrawMode(TDM_OUTLINE);
    //SetTextDrawMode(TDM_UNDERLINE);
    TextOutStrTDM(42, 48,  "밑줄쫙", TDM_UNDERLINE);
    //ClearTextDrawMode(TDM_UNDERLINE);
    //SetTextDrawMode(TDM_BOLD);
    TextOutStrTDM(84, 48,  "굵게", TDM_BOLD);
    //SetTextDrawMode(TDM_ALL);
    //TextOutStr(0, 80,  "권재락만세바보다");
    //ClearTextDrawMode(TDM_ALL);
    //SetTextDrawMode(TDM_NORMAL); /* clear */
#endif
/*    TextOutStrTDMF(0, 80,  TDM_SHADOW | TDM_UNDERLINE, "%02d:%s", 5, "권재락만세바보다");
    TextOutStr(42, 80, "\203\340\203\341");
    TextOutStr(42, 96, "\204\240\204\241");
    TextOutStr(0, 64, "♡◈♧♣㉠㉥あい");*/
    //TextOutStr(0, 80, "♬♪♩◎△◇◆▽");
}

VOID NUIGraphicsDemoDC()
{
    EQS_GetDC(TRUE);
    {
        EQS_SetForeColor(LIGHTGRAY);

        EQS_RoundRectangle(0, 0, 119, 95, 20);
        EQS_RoundRectangle(5, 5, 119-5, 95-5, 20);

        EQS_Line(10, 15, 50, 62);
        EQS_Rectangle(5, 72, 100, 80);
        EQS_Circle(40, 60, 15);
        EQS_Ellipse(40, 30, 20, 15);

        EQS_SetTextDrawMode(TDM_REVERSE);
        
        EQS_GetDC(FALSE);
        {
            EQS_SetFontType(FT_BIGBIG);
            EQS_TextOutStr(0, 0,  "0123456789");
        }
        EQS_ReleaseDC();

        EQS_TextOutStr(0, 0,  "반전");
    
        EQS_GetDC(TRUE);
        {
            EQS_SetForeColor(BLACK);
            EQS_Line(10+5, 15+5, 50+5, 62+5);
            EQS_Rectangle(5+5, 72+5, 100+5, 80+5);
            EQS_Circle(40+5, 60+5, 15);
            EQS_Ellipse(40+5, 30+5, 20, 15);

            EQS_SetFontType(FT_BIG);
            EQS_TextOutStr(0, 16,  "큰 글자 보통");
            EQS_TextOutStr(0, 64,  "\203\340\203\341");
            EQS_TextOutStr(0, 80, "\204\240\204\241");
        }
        EQS_ReleaseDC();

        EQS_Line(10-10, 15-10, 50-10, 62-10);
        EQS_Rectangle(0, 72-10, 100+30, 80+20);
        EQS_Circle(40-10, 60-10, 15);
        EQS_Ellipse(40-10, 30-10, 20, 15);

        EQS_TextOutStr(0, 32,  "조그만 글자 반전");
    }

    EQS_ReleaseDC();
}

VOID NUIGraphicsDemoGetPutImage()
{

    EQS_GetDC(TRUE);
    {
        NUIGraphicsDemoDC();
        EQS_GetRegionImageBuffer(0,0,80,50, &pbGetPutBuffer);
        EQS_GetRegionImage(0,0,80,50,pbGetPutBuffer);
        EQS_FillRectangle(0,0,80,50,LIGHTGRAY);
    }
    EQS_ReleaseDC();
}

WINDOW gWin1, gWin2;
SCROLL_BAR gScrollBar1, gScrollBar2;
BUTTON gButton1, gButton2, gButton3;

void TestOpenWindow(void)
{
    RECT Rect1 = { 10, 35, 100, 80 };
    RECT Rect2 = { 5, 55, 115, 90 };
//    WINDOW_CAPTION Caption = {
//       "마징가 Z", WHITE, 128
//    };
    BYTE* pszCaption = "마징가";

    EQS_GetDC(TRUE);
    {
        RECT ViewRect;

        //EQS_SetTheme(2, FT_MED, 7);

        EQS_PutBitmap(0, 0, &RosesImage);

        //EQS_SetFontType(FT_BIG);
        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextWrap(TRUE);
        EQS_TextOutStr(0,0,
            "나의 모든 것의 반을 "\
            "당신의 반을 "\
            "주십시오. "\
            "당신의 생각의 반 "\
            "당신의 시간의 반 "\
            "당신의 생활의 반 ");
        //NUGraphicsDemo4GImage();

        /*gWin1.Style = WS_CAPTION;
        //EQS_SetWindowStyle(&gWin1, WS_CAPTION);
        gWin1.Caption.pszTitle="로보캅";
        gWin1.Caption.BackColor = BLUE;
        gWin1.Caption.TextColor = YELLOW;
        //EQS_SetWindowCation(&gWin1, "제목인데", BLUE, YELLOW);*/

        //TextOutStr(0, 0, "메시지 박스");
        EQS_PreOpenWindow(&gWin1, WS_POPUP, pszCaption); //WS_FULLSCREEN||WS_BORDER|
        EQS_OpenWindow(&gWin1, &Rect1, TRUE);
        EQS_AddScrollBar(&gWin1, &gScrollBar1, SBT_VERT, Rect1.xLeft, Rect1.yTop, Rect1.yBottom-Rect1.yTop+1,
                        0, 99, 10, 20);
        EQS_DrawScrollBar(&gScrollBar1);
        EQS_SetDrawMode(DM_OR);
        //EQS_FillRectangle(0,0,120,50,DARKGRAY);

        /*{
            SCROLL_BAR_INFO Info;
            EQS_GetScrollBarInfo(&gScrollBar, &Info);
            Info.nPageSize = 50;
            Info.nPos = 50;
            EQS_SetScrollBarInfo(&gScrollBar, &Info, TRUE);
        }*/
        //EQS_SetTextColor(BLUE);
        //EQS_SetTextWrap(TRUE);
        //EQS_SetViewPortRect(&Rect1);
        EQS_AddScrollBar(NULL, &gScrollBar1, SBT_VERT, 120-6, 0, Rect1.yBottom-Rect1.yTop+1,
                         0, 99, 90, 50);
        EQS_DrawScrollBar(&gScrollBar1);
        //EQS_TextOutStr(0, 0, "메시지 박스 예제입니다. 하하 멋있지");
        //EQS_SetTheme(11, FT_BIG, 7);
//        EQS_SetTheme(15, FT_MED, 7);
        //EQS_PreOpenWindow(&gWin2, WS_POPUP, "비너스"); //WS_FULLSCREEN||WS_BORDER|
        //EQS_OpenWindow(&gWin2, &Rect2, TRUE);
        //EQS_AddScrollBar(NULL, &gScrollBar2, SBT_, Rect2.xRight, Rect2.yBottom-7, Rect2.xRight-(Rect2.xLeft)+1,
        //                 0, 99, 90, 10);
        //EQS_DrawScrollBar(&gScrollBar2);
/*        gButton1.pszText = "예";
        gButton1.fActive = TRUE;
        EQS_AddButton(&gWin2, &gButton1, Rect2.xLeft + 5, Rect2.yBottom - 30, -1);
        EQS_DrawButton(&gButton1, 1);

        gButton2.pszText = "아니오";
        gButton2.fActive = TRUE;
        EQS_AddButton(&gWin2, &gButton2, Rect2.xLeft + 5+20, Rect2.yBottom - 30, -1);
        EQS_DrawButton(&gButton2, 0);

        gButton3.pszText = "취소";
        gButton3.fActive = TRUE;
        EQS_AddButton(&gWin2, &gButton3, Rect2.xLeft + 5+80, Rect2.yBottom - 30, -1);
        EQS_DrawButton(&gButton3, 0);*/
    }
    EQS_ReleaseDC();
}

void TestCloseWindow(void)
{
    EQS_GetDC(TRUE);
    {
        EQS_PutBitmap(0, 0, &RosesImage);

        EQS_SetDrawMode(DM_OR);
        EQS_SetTextColor(YELLOW);
        EQS_SetTextWrap(TRUE);
        EQS_TextOutStr(0,0,
            "나의 모든 것의 반을 "\
            "당신의 반을 "\
            "주십시오. "\
            "당신의 생각의 반 "\
            "당신의 시간의 반 "\
            "당신의 생활의 반 ");
    }
    EQS_ReleaseDC();
    //NUIGraphicsDemo4GImage();

    //EQS_CloseWindow(&gWin2);
    EQS_CloseWindow(&gWin1);
}

void TestFormatter(void)
{
    EQS_GetDC(TRUE);
    {
/*        EQS_InitFormatter();
        EQS_AddElement(0, 1, -1);
        EQS_AddElement(0, 2, 10);
        EQS_AddElement(0, 3, 20);
        EQS_AddElement(1, 4, -1);
        EQS_ClearElement();*/
    }
    EQS_ReleaseDC();

    EQS_CloseWindow(&gWin1);
}
