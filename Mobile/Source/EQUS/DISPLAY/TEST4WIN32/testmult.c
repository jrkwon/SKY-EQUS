#include "eqs.h"
#include "testmult.h"

void TestMultiLang(void)
{
//    SIZE* pSize;
    VIEW_PORT ViewPort = { 
        { 10, 10, 100, 50 },
        CS_SCREEN,
        { 0, 0 }
    };

    EQS_GetDC(TRUE);
    {
        /*EQS_SetCountry(COUNTRY_KOR);
        EQS_TextOut(5, 0, TKN_ERR_OUT_OF_MEMORY);
        EQS_SetCountry(COUNTRY_USA);
        EQS_TextOut(5, 16, TKN_ERR_OUT_OF_MEMORY);
        EQS_SetCountry(COUNTRY_KOR);
        EQS_TextOut(5, 32, TKN_ERR_OUT_OF_MEMORY);
        EQS_SetCountry(COUNTRY_USA);
        EQS_TextOut(5, 48, TKN_ERR_OUT_OF_MEMORY);
        EQS_TextOutStrF(5, 70, EQS_GET_TOKEN(TKN_TEST_FMT_TEST1), 2, "kkk");

        pSize = EQS_GetFontSize(FT_BIGBIG, FK_ASCII);
        pSize = EQS_GetFontSize(FT_BIGBIG, FK_HANGUL);
        pSize = EQS_GetFontSize(FT_BIGBIG, FK_SKYSYMBOL);
        pSize = EQS_GetFontSize(FT_BIG, FK_ASCII);
        pSize = EQS_GetFontSize(FT_MED, FK_ASCII);
        pSize = EQS_GetFontSize(FT_MED, FK_HANGUL);
//        pSize = EQS_GetFontSize(SMALL_FONT, FK_ASCII);*/
        /* view port test */


        //EQS_SetViewPort(&ViewPort);
        //EQS_Rectangle(0,0,90,40);
        //EQS_Circle(10,10,15);
        //EQS_SetBackColor(LIGHTGRAY);
        //EQS_ClearViewPort();
        {
            //int i;
            //for(i = 0; i < 255; i++)
            {
                EQS_SetTextColor(YELLOW);
                //EQS_TextOutStr(0, 0, 
                //    "I'd like to live a peaceful country. ");
                EQS_SetTextWrap(TRUE);
                SetSlidingWindowState(TRUE);
                SetSlidingWindowOffsetY(60);
                EQS_TextOutStr(0, 0, 
                //"I'd love to meet clean air and clear sky. If I got a lover."
                    "조용한 산골에서 살면 좋겠어요. "\
                    "맑은 공기와 하늘 빛 아침을 산새들과 함께 맞이하면 정말 좋겠어요. "\
                    "사랑하는 사람이 생긴다면 말이죠"
                    );
                //SetSlidingWindowState(FALSE);
            }
        }
        //EQS_SetTextColor(RED);
        //EQS_TextOutStr(115, 5, "람");
        //EQS_SetTextColor(BLUE);
        //EQS_TextOutStr(5, 7+13, "나는야 구름 나는 바람");
        //EQS_SetTextColor(GREEN);
        //EQS_TextOutStr(5, 7+13*2, "나는야 구름 나는 바람");
        //EQS_SetTextColor(YELLOW);
        //EQS_TextOutStr(5, 7+13*3, "나는야 구름 나는 바람");
        //EQS_Line(0,12,120,12);
        //EQS_TextOutStr(-7, 0, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-6, 13, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-5, 13*2, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-4, 13*3, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-3, 13*4, "A가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-2, 13*5, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(-1, 13*6, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(0, 13*6, "하 둘세네다여일여아열열하나");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(1, 13*7, "가 뷰포트");// 아아 뷰포트 라디오");
        //EQS_TextOutStr(100, 90, "감놔라대추놔라");// 아아 뷰포트 라디오");
        /*SetViewPort(&ViewPort);
        Rectangle(0,0,90,40);
        TextOutStr(0, 0, "여긴 뷰포트");*/

    }
    EQS_ReleaseDC();
}

VOID EQC_UsaEditBox(VOID) {}
VOID EQC_KorEditBox(VOID) {}

