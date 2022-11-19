//==============================================================================
//
//	SINJISOFT GAME
//
//	FILE NAME
//		Puzzle3.cpp
//
//	DESCRIPTION
//		∆€¡Ò ∞‘¿”
//
//	REFERENCES
//		SINJISOFT Game Porting Guide
//
//	Copyright (C) 2001 by SINJISOFT. All Rights Reserved.
//
//==============================================================================

#include <stdio.h> 

#include <string.h> 

#include <stdlib.h>

#include "..\GLib\GLib.h"

#ifdef SKTT5
extern void gm_DrawPauseMark(); 
extern void DrawReadyMark(int stageNum, int nX);

#include "ma2Puzzle3.cpp"

#include "mapPuzzle3.cpp"
static const BYTE * const Jux6nHFq[] = {
TWR52BcQ, XDwrPIvY, xOlDoj6B, fKm7GhqC, hdWHRVeX,
gSFbQUtH, Wq8qhCyn, bgk2grTf, Qd9aieRM, fhmch3L1,
g2M9zq5b, ntib0a5E, IydWGHCJ, tCpf2Bke, UGiwmt8j,
mPzLoCzg, pGSqVBlv, v0BYbunm, Md9FrxUM, EzYr24ac,
HoTrELDV, XnLASMT9, iRS6JvpL, llRQIPZL, mN4h69mz,
Xtg8Z76X, g7xTgnQd, d11wlnpu, aSaJ8dBG, GTVctbtY,
oTDwK8dc, tSTxrEy5, HQwsf4hJ, jzafWOP0, dPgRwsjz,
PZimFLPu, Rmq8Dm2S, kdkhEO9x, mYZm4Zaj, EMFUUmmQ,
}; 
#define L6FduKmG 0

#define to2bFQq0 1

#define f0tj1kUM 2

#define vuNOOqry 3

#define yoXCexSm 4
static int XT4IiYmo;
static INT1 qrAGzg3T; 
static int Gqw0BYPX; static int ShSmRQr0;
static char A7FOZK6a[10]; static BYTE OJvLPIdU[5][8];
static BYTE IExrkaR4, sec; static BYTE LbPt5vw8, y;
static void VctRDcDX() { int year, month, day, hour, IExrkaR4, sec;
gext_GetDate(&year, &month, &day, &hour, &IExrkaR4, &sec);
gext_RandomSeed(sec); XT4IiYmo = 0; Gqw0BYPX = 0;
sec=0; IExrkaR4=0; ShSmRQr0= 25; for(LbPt5vw8=0;LbPt5vw8<5;LbPt5vw8++)
for(y=0;y<8;y++) OJvLPIdU[LbPt5vw8][y] = LbPt5vw8+y*5;
}
static void i9cNSIBU()
{ BYTE dir, H0Ng1GoT, n; BYTE mSdyqwa7; 
for (n = 0; n < 8; n++) { dir = rand()%2;
for (H0Ng1GoT = 0; H0Ng1GoT < (rand()%3) + 1; H0Ng1GoT++) {
switch (dir) { case 0: 
if (y > 0) {
mSdyqwa7 = OJvLPIdU[LbPt5vw8][y];
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8][y - 1];
OJvLPIdU[LbPt5vw8][y - 1] = mSdyqwa7; y--;
} break; case 1: 
if (y < 7) {
mSdyqwa7 = OJvLPIdU[LbPt5vw8][y];
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8][y + 1];
OJvLPIdU[LbPt5vw8][y +1] = mSdyqwa7; y++;
} break; } } dir = rand()%2; for (H0Ng1GoT = 0; H0Ng1GoT < (rand()%3) + 1; H0Ng1GoT++) {
switch (dir) { case 0: 
if (LbPt5vw8 > 0)  {
mSdyqwa7 = OJvLPIdU[LbPt5vw8][y];
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8 - 1][y];
OJvLPIdU[LbPt5vw8-1][y] = mSdyqwa7; LbPt5vw8--;
} break; case 1: 
if (LbPt5vw8 < 4)  {
mSdyqwa7 = OJvLPIdU[LbPt5vw8][y];
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8 + 1][y];
OJvLPIdU[LbPt5vw8+1][y] = mSdyqwa7; LbPt5vw8++;
} break; } } }
for (H0Ng1GoT = 0; H0Ng1GoT < 39; H0Ng1GoT++){
if ( *(*(OJvLPIdU)+H0Ng1GoT) == 39 ){ break;
} } *(*(OJvLPIdU)+H0Ng1GoT) = OJvLPIdU[4][7];
OJvLPIdU[4][7] = 39; }
static int mOuEm71Q()
{ BYTE H0Ng1GoT, hRldyQfl; for (H0Ng1GoT = 0; H0Ng1GoT < 5; H0Ng1GoT++)
for (hRldyQfl = 0; hRldyQfl < 8; hRldyQfl++)
if (OJvLPIdU[H0Ng1GoT][hRldyQfl] != H0Ng1GoT+hRldyQfl*5)
return 0; return 1; } static void kpSqKL0Q()
{ BYTE H0Ng1GoT, hRldyQfl; for (H0Ng1GoT = 0; H0Ng1GoT < 5; H0Ng1GoT++){
for (hRldyQfl = 0; hRldyQfl < 8; hRldyQfl++){
if (OJvLPIdU[H0Ng1GoT][hRldyQfl] == 39){ LbPt5vw8=H0Ng1GoT;
y=hRldyQfl; } } } } static void d1AOMPNl()
{
mil_CopyImage((s_ImgBlock*)ZjB9Wymz, 0, 0);
gext_DrawLcd((char*)swLcdBuffer); } static void LL4MLfPv(int ShowMode)
{ if (ShowMode != 0){ 
mil_CopyImage((s_ImgBlock*)sEJBB9HB, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
mil_CopyImage((s_ImgBlock*)xjJSzPh0, 64, 11+25);
}
else{ 
mil_Clear(S_WHITE);
mil_CopyImage((s_ImgBlock*)xjJSzPh0, 64, 11+25);
}
} static void uTaigoyS() {
mil_CopyImage((s_ImgBlock*)TyInFEXI, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
if(s_Frame2==1) mil_CopyImage((s_ImgBlock*)q68HsJeg, 24, 17);
gext_DrawLcd((char*)swLcdBuffer); } static void zAgwb0Yu()
{ BYTE H0Ng1GoT,hRldyQfl; int init_x=1; int init_y=0;
int step_x=19; int step_y=20; mil_Clear(0x5D);
for(H0Ng1GoT=0;H0Ng1GoT<5;H0Ng1GoT++){
for(hRldyQfl=0;hRldyQfl<8;hRldyQfl++){ mil_CopyImage((s_ImgBlock*)Jux6nHFq[OJvLPIdU[H0Ng1GoT][hRldyQfl]], init_x+step_x*H0Ng1GoT, init_y+step_y*hRldyQfl);
} }
mil_CopyImage((s_ImgBlock*)AqQbtEKR, 96, 0);
mil_CopyImage((s_ImgBlock*)dEqkt2zG, 96, 118);
if(qrAGzg3T == f0tj1kUM){ H0Ng1GoT = gext_Random(0,10);
if(rand()%2==0){ if(ShSmRQr0 + H0Ng1GoT>50) ShSmRQr0 = 50+38;
else ShSmRQr0+=H0Ng1GoT; } else { if(ShSmRQr0 - H0Ng1GoT<0) ShSmRQr0 = 0;
else ShSmRQr0-=H0Ng1GoT; } switch(s_Frame2){
case 0: mil_CopyImage((s_ImgBlock*)dr06UF1s, 96, ShSmRQr0);break;
case 1: mil_CopyImage((s_ImgBlock*)KaikOJaA, 96, ShSmRQr0);break;
} }
sprintf(A7FOZK6a, "%02d", IExrkaR4);
mil_DrawString(97+6, 98+38, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
sprintf(A7FOZK6a, ":%02d ", sec); mil_DrawString(97+6, 108+38, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
if(qrAGzg3T==to2bFQq0 && s_Frame2==0) {
DrawReadyMark(0, SWAP_LCD_WIDTH/2);
} gext_DrawLcd((char*)swLcdBuffer); }
int Puzzle3Main(int nReason, int nData)
{
switch (nReason) 
{ case GLIB_MENU: 
LL4MLfPv(0); 
break; case GLIB_START:
VctRDcDX();
gext_PlaySound((BYTE *)P5DOZPIn, sizeof(P5DOZPIn));
qrAGzg3T = L6FduKmG; g_StartTimer(0, 250, 1);
break;  case GLIB_TIMEOUT: switch (qrAGzg3T)
{ case L6FduKmG: d1AOMPNl(); break; case to2bFQq0:
if(++Gqw0BYPX>=30) {  kpSqKL0Q(); qrAGzg3T = f0tj1kUM;
Gqw0BYPX=0;
g_StartTimer(0, 250, 1);
break; } i9cNSIBU(); zAgwb0Yu(); break; case f0tj1kUM:
if(mOuEm71Q())  { gext_PlaySound((BYTE *)VtT55F8s, sizeof(VtT55F8s));
if(IExrkaR4<20) XT4IiYmo = (19-IExrkaR4)*60 + (60-sec);
else XT4IiYmo = 0; qrAGzg3T = vuNOOqry; Gqw0BYPX = 0;
} if(++Gqw0BYPX%4==0)  { sec++; Gqw0BYPX = 0;
} if (sec == 60) { sec = 0; Gqw0BYPX=0; IExrkaR4++;
if (IExrkaR4 > 99) IExrkaR4 = 99; } zAgwb0Yu();
break; case vuNOOqry: if(++Gqw0BYPX>=10){
Gqw0BYPX = 0; LL4MLfPv(1);  return g_CheckScore(XT4IiYmo);
} uTaigoyS(); break; } break; case GLIB_KEYPRESS:
switch (qrAGzg3T) { case L6FduKmG: qrAGzg3T = to2bFQq0;
Gqw0BYPX=0; LbPt5vw8=4; y=5; gext_PlaySound((BYTE *)Fxby1XBz, sizeof(Fxby1XBz));
g_StartTimer(0, 250, 1); break; case f0tj1kUM:
switch (nData){ case GLIB_KEY_DOWN: 
case GLIB_KEY_8: 
if (y > 0) { gext_PlaySound((BYTE *)NYhJYgIG, sizeof(NYhJYgIG));
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8][y - 1];
y--; OJvLPIdU[LbPt5vw8][y] = 39; } break;
case GLIB_KEY_UP: 
case GLIB_KEY_2: 
if (y < 7) { gext_PlaySound((BYTE *)NYhJYgIG, sizeof(NYhJYgIG));
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8][y + 1];
y++; OJvLPIdU[LbPt5vw8][y] = 39; } break;
case GLIB_KEY_RIGHT: 
case GLIB_KEY_6: 
if (LbPt5vw8 > 0) { gext_PlaySound((BYTE *)NYhJYgIG, sizeof(NYhJYgIG));
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8 - 1][y];
LbPt5vw8--; OJvLPIdU[LbPt5vw8][y] = 39; }
break; case GLIB_KEY_LEFT: 
case GLIB_KEY_4: 
if (LbPt5vw8 < 4) { gext_PlaySound((BYTE *)NYhJYgIG, sizeof(NYhJYgIG));
OJvLPIdU[LbPt5vw8][y] = OJvLPIdU[LbPt5vw8 + 1][y];
LbPt5vw8++; OJvLPIdU[LbPt5vw8][y] = 39; }
break; } break; case vuNOOqry: if(Gqw0BYPX>3) {
Gqw0BYPX = 0;
LL4MLfPv(1); 
} break; } break; case GLIB_PAUSE:
gm_DrawPauseMark();
gext_DrawLcd((char*)swLcdBuffer); return GRET_PAUSE;
case GLIB_CONTINUE: if(qrAGzg3T==L6FduKmG) gext_PlaySound((BYTE *)P5DOZPIn, sizeof(P5DOZPIn));
g_StartTimer(0, 250, 1); break; case GLIB_END:
mil_SaveLCD(); return GRET_PAUSE; case GLIB_MENU_SCORE:
case GLIB_SCORE:
LL4MLfPv(1); 
break; case GLIB_HELP: case GLIB_HELP_TIMEOUT:
mil_CopyImage((s_ImgBlock*)oWsCnQyf, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
break; } return GRET_NORMAL; } 
#endif 
