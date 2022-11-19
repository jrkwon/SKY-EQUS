#include "eqs.h"
#include "eqs_feat.h"
#include "eqs_grph.h"
#include "EqsSystem.h"
#include "msm4win32.h"

#ifdef __cplusplus
extern "C" {
#endif


BYTE* GetLCDBuffer(VOID)
{
    SCREEN_INDEX_E Screen;
    BYTE *pBuf;

    Screen = SelectScreen(SI_MAIN_SCREEN);
    pBuf = GetScreenBuffer();
    SelectScreen(Screen);

    return pBuf;
}

VOID ClearNUIScreen(VOID)
{
    SCREEN_INDEX_E Screen;
    BYTE *pBuf;

    Screen = SelectScreen(SI_MAIN_SCREEN);
    ClearScreen();
    SelectScreen(Screen);
}

BYTE* GetSubLCDBuffer(VOID)
{
    SCREEN_INDEX_E Screen;
    BYTE *pBuf;

    Screen = SelectScreen(SI_SUB_SCREEN);
    pBuf = GetScreenBuffer();
    SelectScreen(Screen);
    
    return pBuf;
}

VOID ClearSubNUIScreen(VOID)
{
    SCREEN_INDEX_E Screen;

    Screen = SelectScreen(SI_SUB_SCREEN);
    ClearScreen();
    SelectScreen(Screen);
}


#ifdef __cplusplus
}
#endif

