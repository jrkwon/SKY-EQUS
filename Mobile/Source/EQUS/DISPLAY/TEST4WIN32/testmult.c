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
                    "������ ��񿡼� ��� ���ھ��. "\
                    "���� ����� �ϴ� �� ��ħ�� ������ �Բ� �����ϸ� ���� ���ھ��. "\
                    "����ϴ� ����� ����ٸ� ������"
                    );
                //SetSlidingWindowState(FALSE);
            }
        }
        //EQS_SetTextColor(RED);
        //EQS_TextOutStr(115, 5, "��");
        //EQS_SetTextColor(BLUE);
        //EQS_TextOutStr(5, 7+13, "���¾� ���� ���� �ٶ�");
        //EQS_SetTextColor(GREEN);
        //EQS_TextOutStr(5, 7+13*2, "���¾� ���� ���� �ٶ�");
        //EQS_SetTextColor(YELLOW);
        //EQS_TextOutStr(5, 7+13*3, "���¾� ���� ���� �ٶ�");
        //EQS_Line(0,12,120,12);
        //EQS_TextOutStr(-7, 0, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-6, 13, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-5, 13*2, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-4, 13*3, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-3, 13*4, "A�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-2, 13*5, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(-1, 13*6, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(0, 13*6, "�� �Ѽ��״ٿ��Ͽ��ƿ����ϳ�");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(1, 13*7, "�� ����Ʈ");// �ƾ� ����Ʈ ����");
        //EQS_TextOutStr(100, 90, "��������߳���");// �ƾ� ����Ʈ ����");
        /*SetViewPort(&ViewPort);
        Rectangle(0,0,90,40);
        TextOutStr(0, 0, "���� ����Ʈ");*/

    }
    EQS_ReleaseDC();
}

VOID EQC_UsaEditBox(VOID) {}
VOID EQC_KorEditBox(VOID) {}

