//==============================================================================
//
//	SINJISOFT GAME LIBRARY
//
//	FILE NAME
//		GLibMain.cpp
//
//	DESCRIPTION
//		Gmae library main interface routines
//
//	REFERENCES
//		SINJISOFT Game Porting Guide
//
//	Copyright (C) 1999 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		1999.11. 0	Release Version 0.9
//		1999.12.21	Release Version 1.0
//		2000. 1. 7  Release Version 1.1
//		2000. 2.22  Release Version 1.2
//		2000.10. 8  Release Version 2.0
//
//==============================================================================

#include <stdio.h>

#include <string.h>

#include "GLib.h"

#ifdef _WIN32

#include "..\GEmul.h"

#include "..\GEmulDlg.h"

#endif
int s_Frame; 
int s_Frame2; 
int s_Frame3; 
int s_Frame6; 
/*
#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
WORD *swLcdBuffer; WORD *swBackLcdBuffer;

#else
BYTE *swLcdBuffer; BYTE *swBackLcdBuffer;

#endif
*/
int s_GameID = 0; static int e7mBDeVX; static int Y8rugKNJ = -1;
static int BT9PYMlQ; 
#if defined (GIGATEL1) || defined (GIGATEL2) || defined (GIGATEL1_SINJILOGO) || defined (GIGATEL2_SINJILOGO) || defined (SMARTMOBILE) || defined (SAMSUNG1) || defined (SAMSUNG2) || defined (NOKIA2_CELLULAR) || defined (NOKIA2_PCS)
static int bCO0bM8w; 
#endif
static int lhoxsk7M[G_MAX_GAME_SIZE][3]; static int GdG8mwdy[G_MAX_GAME_SIZE][3];

#ifdef LGIC11
static int nSub0nar; 
#endif
static void CQHSxkW0()
{ lhoxsk7M[s_GameID][0] = 0; GdG8mwdy[s_GameID][0] = 0;
gext_StopTimer(0); lhoxsk7M[s_GameID][1] = 0;
GdG8mwdy[s_GameID][1] = 0; gext_StopTimer(1);
lhoxsk7M[s_GameID][2] = 0; GdG8mwdy[s_GameID][2] = 0;
gext_StopTimer(2); }
static void jzpVRmy0()
{ gext_StopTimer(0); gext_StopTimer(1); gext_StopTimer(2);
}
static void mSAJ4wdn()
{ if (lhoxsk7M[s_GameID][0] > 0) gext_StartTimer(0, lhoxsk7M[s_GameID][0], GdG8mwdy[s_GameID][0]);
if (lhoxsk7M[s_GameID][1] > 0) gext_StartTimer(1, lhoxsk7M[s_GameID][1], GdG8mwdy[s_GameID][1]);
if (lhoxsk7M[s_GameID][2] > 0) gext_StartTimer(2, lhoxsk7M[s_GameID][2], GdG8mwdy[s_GameID][2]);
} void g_StartTimer(const int nTid, const int nMsec, const int nRepeat)
{ lhoxsk7M[s_GameID][nTid] = nMsec; GdG8mwdy[s_GameID][nTid] = nRepeat;
gext_StartTimer(nTid, nMsec, nRepeat); } void g_StopTimer(const int nTid)
{ lhoxsk7M[s_GameID][nTid] = 0; GdG8mwdy[s_GameID][nTid] = 0;
gext_StopTimer(nTid); } void g_SetColor(const int LG0dStpd)
{
mil_SetColor(LG0dStpd);
}
void glib_Init_SINJISOFT_Game()
{
gext_InitMemory();
s_Frame = 0;
s_Frame2 = 0; s_Frame3 = 0; s_Frame6 = 0;
mil_InitLcd(SWAP_LCD_WIDTH, SWAP_LCD_HEIGHT, (BYTE*)swLcdBuffer, (BYTE*)swBackLcdBuffer);
return; }

#if defined (NOKIA2_CELLULAR) || defined (NOKIA2_PCS)

#include "menu_nokia2.cpp"

#elif defined (SAMSUNG2) 

#include "menu_color.cpp"

#elif defined (LGIC13) || defined (LGIC14)

#include "menu_LGE13.cpp"

#elif defined (SWAP_LCD_BW)

#include "menu_bw.cpp"

#elif defined (SWAP_LCD_GRAY) 

#include "menu_gray.cpp"

#else 

#include "menu_color.cpp"

#endif
static void HAvJVeND(int pause) {
s_Frame = (s_Frame + 1)%5040;
s_Frame2 = s_Frame%2; s_Frame3 = s_Frame%3;
s_Frame6 = s_Frame%6; mil_SetBlinkColor(s_Frame6);

#if defined (NOKIA2_CELLULAR) || defined (NOKIA2_PCS) || defined (SAMSUNG2)
s_GameDef[s_GameID].GameMain(GLIB_MENU, bCO0bM8w);

#else
s_GameDef[s_GameID].GameMain(GLIB_MENU, 0);

#endif
mil_SetGamma(3); 
#if defined (NOKIA2_CELLULAR) || defined (NOKIA2_PCS)
if(bCO0bM8w == 0){ 
mil_CopyImage((s_ImgBlock*)Eng_simg_1_newgame, 0, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)Eng_simg_2_highscore, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)Eng_simg_3_help, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 1); 
mil_CopyImage((s_ImgBlock*)hA1H4K56, 4, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)na0XNBCg, SWAP_LCD_WIDTH -50, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)CiOLuoM0, SWAP_LCD_WIDTH -20, SWAP_LCD_HEIGHT - 45); 
if (pause == 0) mil_SetGamma(1); mil_CopyImage((s_ImgBlock*)Eng_simg_4_continue, 0, SWAP_LCD_HEIGHT - 1); 
} else { 
mil_CopyImage((s_ImgBlock*)Kor_simg_1_newgame, 0, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)Kor_simg_2_highscore, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)Kor_simg_3_help, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 1); 
mil_CopyImage((s_ImgBlock*)hA1H4K56, 4, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)na0XNBCg, SWAP_LCD_WIDTH -50, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)CiOLuoM0, SWAP_LCD_WIDTH -20, SWAP_LCD_HEIGHT - 45); 
if (pause == 0) mil_SetGamma(1); mil_CopyImage((s_ImgBlock*)Kor_simg_4_continue, 0, SWAP_LCD_HEIGHT - 1); 
} 
#elif defined (LGIC13) 
mil_CopyImage((s_ImgBlock*)SVdgQk7R, 2, 42); 
mil_CopyImage((s_ImgBlock*)LukcFCrd, 57 , 42); 
mil_CopyImage((s_ImgBlock*)rinMobWi, 2 , 94 ); 
mil_CopyImage((s_ImgBlock*)hA1H4K56, 3, 55); 
mil_CopyImage((s_ImgBlock*)na0XNBCg, 81, 56); 
mil_CopyImage((s_ImgBlock*)CiOLuoM0, 22, 107); 
if (pause == 0) mil_SetGamma(1); mil_CopyImage((s_ImgBlock*)R823MiOq, 57, 94 ); 
mil_CopyImage((s_ImgBlock*)simg_continue, 77, 107); 

#elif defined (LGIC14) 
mil_CopyImage((s_ImgBlock*)SVdgQk7R, 2, 42); 
mil_CopyImage((s_ImgBlock*)LukcFCrd, 57 , 42); 
mil_CopyImage((s_ImgBlock*)rinMobWi, 2 , 94 ); 
mil_CopyImage((s_ImgBlock*)hA1H4K56, 3, 55); 
mil_CopyImage((s_ImgBlock*)na0XNBCg, 81, 56); 
mil_CopyImage((s_ImgBlock*)CiOLuoM0, 22, 107); 
if (pause == 0) mil_SetGamma(1); mil_CopyImage((s_ImgBlock*)R823MiOq, 57, 94 ); 
mil_CopyImage((s_ImgBlock*)simg_continue, 77, 107); 

#else 
mil_CopyImage((s_ImgBlock*)SVdgQk7R, 0, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)LukcFCrd, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 79); 
mil_CopyImage((s_ImgBlock*)rinMobWi, SWAP_LCD_WIDTH - 1, SWAP_LCD_HEIGHT - 1); 
mil_CopyImage((s_ImgBlock*)hA1H4K56, 4, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)na0XNBCg, SWAP_LCD_WIDTH -50, SWAP_LCD_HEIGHT - 63); 
mil_CopyImage((s_ImgBlock*)CiOLuoM0, SWAP_LCD_WIDTH -20, SWAP_LCD_HEIGHT - 45); 
if (pause == 0) mil_SetGamma(1); mil_CopyImage((s_ImgBlock*)R823MiOq, 0, SWAP_LCD_HEIGHT - 1); 

#endif
mil_SetGamma(3);

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
gext_DrawLcd((WORD*)swLcdBuffer); 
#else
gext_DrawLcd((char*)swLcdBuffer); 
#endif
}
static void FZRf2WUX()
{
gext_StopSound();
gext_StopVib();

#ifndef LGIC11 
CQHSxkW0(); 
#endif
s_GameDef[s_GameID].GameMain(GLIB_START, 0);
}
static int M7oSEwdG(int mode)
{ int nRet; 
#if defined (SKTT4) || defined (SKTT5) || defined (SKTT7) || defined (LGIC13) || defined (LGIC14)

#if defined (LGIC13) || defined (LGIC14)
s_GameDef[s_GameID].GameMain(GLIB_CONTINUE, 0);
s_GameDef[s_GameID].GameMain(GLIB_TIMEOUT, 0);

#else 
s_GameDef[s_GameID].GameMain(GLIB_TIMEOUT, 3);

#endif
gext_StopTimer(0); gext_StopTimer(1); gext_StopTimer(2);

#endif
nRet = s_GameDef[s_GameID].GameMain(GLIB_PAUSE, mode);
if (nRet == GRET_PAUSE) {
gext_StopSound(); gext_StopVib(); if (mode!=2) jzpVRmy0();
}
return nRet; }
static void bmE4Gwyt()
{
gext_StopSound();
gext_StopVib(); s_GameDef[s_GameID].GameMain(GLIB_CONTINUE, 0);
mSAJ4wdn();
}
static int mRc49kNz(int id)
{
if (id == 0) {
s_Frame = (s_Frame + 1)%5040; s_Frame2 = s_Frame%2;
s_Frame3 = s_Frame%3; s_Frame6 = s_Frame%6;
mil_SetBlinkColor(s_Frame6); } return s_GameDef[s_GameID].GameMain(GLIB_TIMEOUT, id);
}
static int JG14eUey(int Key)
{
return s_GameDef[s_GameID].GameMain(GLIB_KEYPRESS, Key);
}
static int f0SXh3qy()
{ int nRet; nRet = s_GameDef[s_GameID].GameMain(GLIB_END, 0);
gext_StopSound();
gext_StopVib(); jzpVRmy0(); return nRet; }
static void tfNSCqil(int edit)
{
gext_StopVib();
if(edit!=3){
s_Frame = (s_Frame + 1)%5040; s_Frame2 = s_Frame%2;
s_Frame3 = s_Frame%3; s_Frame6 = s_Frame%6;
mil_SetBlinkColor(s_Frame6); } 
if(edit==2) s_GameDef[s_GameID].GameMain(GLIB_MENU_SCORE, s_GameID);
else s_GameDef[s_GameID].GameMain(GLIB_SCORE, s_GameID);
if ((edit == 0)||(edit == 2)) {
g_DrawScoreBoard(0);
} }
static void QU4IcLFB()
{
gext_StopSound();
gext_StopVib();
s_GameDef[s_GameID].GameMain(GLIB_HELP, s_GameID);

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
gext_DrawLcd((WORD*)swLcdBuffer); 
#else
gext_DrawLcd((char*)swLcdBuffer); 
#endif
} static void WrSRS9NB(int id) {
if (id == 0) {
s_Frame = (s_Frame + 1)%5040; s_Frame2 = s_Frame%2;
s_Frame3 = s_Frame%3; s_Frame6 = s_Frame%6;
mil_SetBlinkColor(s_Frame6); } s_GameDef[s_GameID].GameMain(GLIB_HELP_TIMEOUT, id);

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
gext_DrawLcd((WORD*)swLcdBuffer); 
#else
gext_DrawLcd((char*)swLcdBuffer); 
#endif
}
static int u4T8Qmc6(int mode)
{ int nRet;
nRet = s_GameDef[s_GameID].GameMain(GLIB_CHECK_STATE, mode);
return nRet; } 
#ifdef G_NET_GAME
int glib_ReceiveData(BYTE nLength)
{ return s_GameDef[s_GameID].GameMain(GLIB_NET_RECEIVE, nLength);
} 
#endif 

#define YTwPrBKb 0xFFFF

#define eLbFc93O 0xFFFE

#if defined (SKTT3MA2) || defined (SKTT5)

#define p0bvn9mb GLIB_KEY_OK 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW GLIB_KEY_CLR 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_OK 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (SKTT7)

#define p0bvn9mb GLIB_KEY_LEFT 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW GLIB_KEY_CLR 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_LEFT 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (LGIC9)

#define p0bvn9mb YTwPrBKb 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_SND 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (LGIC10)

#define p0bvn9mb GLIB_KEY_PF1 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y GLIB_KEY_CLR 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U GLIB_KEY_PF2 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_PF1 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (LGIC11)

#define p0bvn9mb GLIB_KEY_OK 

#define w1ZW3mTZ YTwPrBKb 

#define zwjAPDYU GLIB_KEY_SOFT1 

#define mMDSsdVr GLIB_KEY_SOFT2 

#define I2bmJWPw GLIB_KEY_SOFT3 

#define FmJKOy5y GLIB_KEY_DONE 

#define Y9HeE4kB GLIB_KEY_DONE 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 GLIB_KEY_DONE 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_DONE 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (WORLDMTEL)

#define p0bvn9mb YTwPrBKb 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_SND 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (LGIC12)

#define p0bvn9mb GLIB_KEY_SOFT1 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y GLIB_KEY_SOFT1 

#define Y9HeE4kB GLIB_KEY_SOFT2 

#define y8sIUNuW GLIB_KEY_SOFT2 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_SOFT2 

#define EOeyaIK2 GLIB_KEY_SOFT1 

#define y5TvOyEz GLIB_KEY_END 

#elif defined (SAMSUNG1) || defined (SAMSUNG2)

#define p0bvn9mb YTwPrBKb 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB eLbFc93O 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_OK 

#define y5TvOyEz GLIB_KEY_END 

#else

#define p0bvn9mb YTwPrBKb 

#define w1ZW3mTZ GLIB_KEY_1 

#define zwjAPDYU GLIB_KEY_2 

#define mMDSsdVr GLIB_KEY_3 

#define I2bmJWPw GLIB_KEY_4 

#define FmJKOy5y YTwPrBKb 

#define Y9HeE4kB GLIB_KEY_CLR 

#define y8sIUNuW YTwPrBKb 

#define ZtFhCFq7 YTwPrBKb 

#define VrJqCx8U YTwPrBKb 

#define DFbjS7T4 GLIB_KEY_CLR 

#define EOeyaIK2 GLIB_KEY_OK 

#define y5TvOyEz GLIB_KEY_END 

#endif
int glib_GameEvent(int state, int data)
{ int nRet; if (state == GLIB_START_EVENT)
{ 
#if defined (SAMSUNG2) || defined (NOKIA2_CELLULAR) || defined (NOKIA2_PCS)
s_GameID = data%10; bCO0bM8w = (data/10)%10;

#else 
s_GameID = data; 
#endif
e7mBDeVX = GLIB_MODE_MENU; 
gext_OnKeyTone();
HAvJVeND(Y8rugKNJ == s_GameID); 
CQHSxkW0(); g_StartTimer(0, 500, 1); return 0;
}

#ifdef LGIC11
if (state == GLIB_SCREEN_EVENT) { if (e7mBDeVX == GLIB_MODE_MENU && Y8rugKNJ != s_GameID) return 1;
else if (e7mBDeVX == GLIB_MODE_MENU && Y8rugKNJ == s_GameID) return 4;
else if (e7mBDeVX == GLIB_MODE_GAME && Y8rugKNJ != s_GameID) return u4T8Qmc6(0);
else if (e7mBDeVX == GLIB_MODE_GAME && Y8rugKNJ == s_GameID) return 7;
else return 0; } 
#endif

#ifdef LGIC10
if (state == GLIB_SCREEN_EVENT) { if (e7mBDeVX == GLIB_MODE_MENU && Y8rugKNJ != s_GameID) return 1;
else if (e7mBDeVX == GLIB_MODE_MENU && Y8rugKNJ == s_GameID) return 2;
else if (e7mBDeVX == GLIB_MODE_GAME && Y8rugKNJ != s_GameID) 
{ if(u4T8Qmc6(0)) return 3; else return 4;
} else if (e7mBDeVX == GLIB_MODE_GAME && Y8rugKNJ == s_GameID) 
{ if(u4T8Qmc6(0)) return 5; else return 6;
} else return 0; } 
#endif
switch (e7mBDeVX) {
case GLIB_MODE_MENU: switch (state) { case GLIB_TIMEOUT_EVENT:
HAvJVeND(Y8rugKNJ == s_GameID); 
break; case GLIB_KEY_EVENT: switch (data) {
case p0bvn9mb:  case w1ZW3mTZ:  e7mBDeVX = GLIB_MODE_GAME; 
gext_OffKeyTone();
Y8rugKNJ = -1; FZRf2WUX(); 
break; case zwjAPDYU: e7mBDeVX = GLIB_MODE_SCORE; 
g_LoadScore();
tfNSCqil(2); 
break; case mMDSsdVr: e7mBDeVX = GLIB_MODE_HELP; 
QU4IcLFB(); 
break; case I2bmJWPw: if(Y8rugKNJ == s_GameID){
switch (BT9PYMlQ) { case GLIB_MODE_GAME: e7mBDeVX = GLIB_MODE_GAME; 
gext_OffKeyTone();
nRet = M7oSEwdG(1); 

#ifdef LGIC10
g_StartTimer(0, 500, 1); 

#endif
if (nRet != GRET_PAUSE) { Y8rugKNJ = -1; bmE4Gwyt(); 
} break; case GLIB_MODE_SCOREEDIT: Y8rugKNJ = -1;
e7mBDeVX = GLIB_MODE_SCOREEDIT; 
g_StartTimer(0, 1000, 1); 
break; } } break; case Y9HeE4kB: 
jzpVRmy0(); 
s_GameID = -1; return 1; break; } break; case GLIB_END_EVENT:
break;
} break; case GLIB_MODE_GAME: switch (state) {
case GLIB_TIMEOUT_EVENT: 
#ifdef LGIC10 
if (Y8rugKNJ == s_GameID){ M7oSEwdG(2); break;
} 
#else 
if (Y8rugKNJ == s_GameID) break; 
#endif 

#ifdef LGIC11
if(nSub0nar>=1){ if(--nSub0nar == 0 ) { nSub0nar = 0;
nRet = M7oSEwdG(0); 
if (nRet == GRET_PAUSE) { Y8rugKNJ = s_GameID;
BT9PYMlQ = GLIB_MODE_GAME; } break; } } 
#endif
nRet = mRc49kNz(data); 
switch (nRet) { case GRET_SCORE_VIEW:
e7mBDeVX = GLIB_MODE_SCORE; 
gext_OnKeyTone();
tfNSCqil(0);
break; case GRET_SCORE_EDIT: CQHSxkW0(); e7mBDeVX = GLIB_MODE_SCOREEDIT; 
gext_OnKeyTone();
g_EditScore(GLIB_START, 0); 
g_StartTimer(0, 1000, 1); break; } break;
case GLIB_KEY_EVENT: 
#ifdef LGIC11
if(nSub0nar) break; 
#endif
if ( (VrJqCx8U & YTwPrBKb) != YTwPrBKb){ 
if(data == VrJqCx8U && u4T8Qmc6(0)) { 
nRet = M7oSEwdG(0); 
g_StartTimer(0, 500, 1); if (nRet == GRET_PAUSE) {
Y8rugKNJ = s_GameID; BT9PYMlQ = GLIB_MODE_GAME;
} e7mBDeVX = GLIB_MODE_MENU; 
gext_OnKeyTone(); HAvJVeND(Y8rugKNJ == s_GameID);
break; } }
if ( (ZtFhCFq7 & YTwPrBKb) != YTwPrBKb){ 
if(data == ZtFhCFq7 && Y8rugKNJ != s_GameID){ 
nRet = M7oSEwdG(3); 
g_StartTimer(0, 500, 1); if (nRet == GRET_PAUSE) {
Y8rugKNJ = s_GameID; BT9PYMlQ = GLIB_MODE_GAME;
} e7mBDeVX = GLIB_MODE_MENU; 
gext_OnKeyTone(); HAvJVeND(Y8rugKNJ == s_GameID);
break; } } if (Y8rugKNJ == s_GameID) { 
if(data == y8sIUNuW){ 
gext_OnKeyTone();
return 1; 
break; 
}  else if ( (FmJKOy5y & YTwPrBKb) != YTwPrBKb){ 
if(data == FmJKOy5y){ Y8rugKNJ = -1; bmE4Gwyt(); 
break; } else break; } else { Y8rugKNJ = -1;
bmE4Gwyt(); 
break; } } if (data == DFbjS7T4) { 

#ifdef LGIC11
if( u4T8Qmc6(1) == 20){ nSub0nar = 2; return 0;
} 
#endif
nRet = M7oSEwdG(0); 

#ifdef LGIC10
g_StartTimer(0, 500, 1); 

#endif
if (nRet == GRET_PAUSE) { Y8rugKNJ = s_GameID;
BT9PYMlQ = GLIB_MODE_GAME; } break; } nRet = JG14eUey(data); 
switch (nRet) { case GRET_SCORE_VIEW:
e7mBDeVX = GLIB_MODE_SCORE; 
gext_OnKeyTone();
tfNSCqil(0);
break; case GRET_SCORE_EDIT: CQHSxkW0(); e7mBDeVX = GLIB_MODE_SCOREEDIT; 
gext_OnKeyTone();
g_EditScore(GLIB_START, 0); 
g_StartTimer(0, 1000, 1); break; } break;
case GLIB_END_EVENT: nRet = f0SXh3qy(); 
if (nRet == GRET_PAUSE) { 
Y8rugKNJ = s_GameID; BT9PYMlQ = GLIB_MODE_GAME;
} s_GameID = -1; break; } break; case GLIB_MODE_SCOREEDIT:
switch (state) { case GLIB_TIMEOUT_EVENT:
tfNSCqil(1); g_EditScore(GLIB_TIMEOUT, data); 
break; case GLIB_KEY_EVENT: if (data == EOeyaIK2) {
g_EditScore(GLIB_END, 0); 
g_SaveScore(); e7mBDeVX = GLIB_MODE_MENU; 
HAvJVeND(Y8rugKNJ == s_GameID); break; } tfNSCqil(3);
g_EditScore(GLIB_KEYPRESS, data); 
break; case GLIB_END_EVENT: g_SaveScore();
Y8rugKNJ = s_GameID; 
BT9PYMlQ = GLIB_MODE_SCOREEDIT; s_GameID = -1;
break; } break; case GLIB_MODE_SCORE: switch (state) {
case GLIB_TIMEOUT_EVENT: tfNSCqil(2); 
break; case GLIB_KEY_EVENT:
if ( (ZtFhCFq7 & YTwPrBKb) != YTwPrBKb){ 
if(data == ZtFhCFq7 ){ 
e7mBDeVX = GLIB_MODE_MENU; 
HAvJVeND(Y8rugKNJ == s_GameID); break; } else break; 
} e7mBDeVX = GLIB_MODE_MENU; 
if(data!=y5TvOyEz) HAvJVeND(Y8rugKNJ == s_GameID);
break; case GLIB_END_EVENT: break;
} break; case GLIB_MODE_HELP: switch (state) {
case GLIB_TIMEOUT_EVENT: WrSRS9NB(data); break;
case GLIB_KEY_EVENT:
if ( (ZtFhCFq7 & YTwPrBKb) != YTwPrBKb){ 
if(data == ZtFhCFq7 ){ 
e7mBDeVX = GLIB_MODE_MENU; 
HAvJVeND(Y8rugKNJ == s_GameID); break; } else break; 
} e7mBDeVX = GLIB_MODE_MENU; 
if(data!=y5TvOyEz) HAvJVeND(Y8rugKNJ == s_GameID);
break; case GLIB_END_EVENT: break;
} } return 0; } 