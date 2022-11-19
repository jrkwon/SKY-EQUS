//==============================================================================
//
//	SINJISOFT GAME
//
//	FILE NAME
//		Apache5.cpp
//
//	DESCRIPTION
//		아파치5특공대 게임 
//
//	REFERENCES
//		SINJISOFT Game Porting Guide
//
//	Copyright (C) 1999 by SINJISOFT. All Rights Reserved.
//
//==============================================================================

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "..\GLib\GLib.h"

#ifdef SKTT5
extern void gm_DrawPauseMark(); 
extern void gm_PlayStageChange(); 
extern void DrawReadyMark(int stageNum, int nX);

#define h3Xp5zzX 1000 

#define aoZjmFp7 -1 

#define ak6MkVXT 0 

#define mRHSnFp1 1

#define JWyRjw0d 2

#define DMqJ0k0h 3

#define s3wzcxdl 4

#define GR8qJMC3 5

#define ftfjUT53 16 

#define JwIur34E 24

#define whFcpWZ2 1 

#define QsIeLzu2 6 

#define xgVzvYXt 4 

#define RGI6Le21 7 

#define gFu09gz6 0

#define bDA1TYkC 5 

#define RF4rjaDK 5

#define pHk3bPDl 8 

#define TRUeJF5p -3 

#define fJSt9vf5 10 

#define FU7aKqij 3 

#define TgVg3TkO -4 

#define iFW76UTu 20 

#define FVfKdJDv 15 

#define zz4tjOti 8 

#define pvs6R3Sj -4 

#define vm2jSjAz -4

#define rFnXVlz6 -8 

#define Z0UtcsUA -2 

#define Jikqh1MW 3

#define DWHZiOAD 4 

#include "ma2Apache5.cpp"

#include "mapApache5.cpp"
static const BYTE * const OLs9dzti[] = {
oraWC1hf, 
HKZxWVxZ, 
K3Bu0Zgz 
}; static const BYTE * const qrCMdwBc[] = {
sC3z7XAy, 
UtXSFjMR 
}; static const BYTE * const mnutAzPu[] = {
fzs6gcVR, 
UFp6IScG, 
nJZLqpvm, 
as6KHubY, 
XGsB5xvE, 
KPPUdUrc 
}; static const BYTE * const KMrcImxP[] = {
WflzIdQF, 
qrHE0K2P, 
ICwlUmIc, 
hP6yA9qA 
}; static const BYTE * const WTFlcfxL[] = {
VBhjh08N, 
iUaWXG7O, 
Kdd5m1X7 
}; static const BYTE * const mSfYMjTo[] = {
nKbQTG5U, 
WD0g9dsU, y5zPYZW7, PeHtO9i2 };
static g_Object sgWZa35q =
{ 0, 0, 0, aoZjmFp7, 0, 0, 0 }; static g_Object VCNYGzTw = 
{ 2, 10, 16, 0, 0, 10, 4 }; static g_Object EZgYCKCk[xgVzvYXt] = {
{ 0, 0, 0, 0, 0, 2, 4 }, { 0, 0, 0, 0, 0, 2, 4 },
{ 0, 0, 0, 0, 0, 2, 4 }, { 0, 0, 0, 0, 0, 2, 4 }
}; static g_Object gcOyvaiF[pHk3bPDl] = {
{ 0, 0, 0, TRUeJF5p, 0, 8, 10 }, { 0, 0, 0, TRUeJF5p, 0, 8, 10 },
{ 0, 0, 0, TRUeJF5p, 0, 8, 10 }, { 0, 0, 0, TRUeJF5p, 0, 8, 10 },
{ 0, 0, 0, TRUeJF5p, 0, 8, 10 }, { 0, 0, 0, TRUeJF5p, 0, 8, 10 },
{ 0, 0, 0, TRUeJF5p, 0, 8, 10 }, { 0, 0, 0, TRUeJF5p, 0, 8, 10 }
}; static g_Object LlBG6qjZ[FU7aKqij] = {
{ 0, 0, 0, TgVg3TkO, 0, 10, 3 }, { 0, 0, 0, TgVg3TkO, 0, 10, 3 },
{ 0, 0, 0, TgVg3TkO, 0, 10, 3 } }; static g_Object KWjjk3Ne[zz4tjOti] = {
{ 0, 0, 0, 0, 0, 3, 3 }, { 0, 0, 0, 0, 0, 3, 3 },
{ 0, 0, 0, 0, 0, 3, 3 }, { 0, 0, 0, 0, 0, 3, 3 },
{ 0, 0, 0, 0, 0, 3, 3 }, { 0, 0, 0, 0, 0, 3, 3 },
{ 0, 0, 0, 0, 0, 3, 3 }, { 0, 0, 0, 0, 0, 3, 3 }
}; static g_Object intYQAZ4[Jikqh1MW] = {
{ 0, 0, 0, 0, 0, 8, 7 }, { 0, 0, 0, 0, 0, 8, 7 },
{ 0, 0, 0, 0, 0, 8, 7 } }; static g_Object K4TZC498 = 
{ 0, 0, 0, Z0UtcsUA, 0, 12, 8 }; static int const TSvFBcBx[] = { 250, 240, 230, 230, 220, 220, 210, 210, 200, 200 };
static int const y7yEY3Hf[] = { 4, 5, 6, 7, 8, 8, 8, 8, 8, 8 };
static int const F94HVh2c[] = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 100 };
static int const zyxcZsxb[] = { 2, 2, 3, 3, 3, 3, 3, 3, 3, 3 };
static int const YS93vXOc[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
static int const ml5o3Rmz[] = { 4, 4, 5, 5, 6, 6, 7, 7, 8, 8 };
static int const aMgYZu1G[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
static int const jYk28tjm[] = { -5, 5, 3, 4, 4, 4, 5 };
static WORD iazoOS2M; 
static int J6vnDQgO; 
static int E1nAopVG; 
static int fvnlciAY; 
static int FvOb6IEh; 
static int mJVqXuCS; 
static int SwIfwP05; 
static int t9iTtCgu; 
static int lnpRbNNp; 
static int XWZgUjtk; 
static int uH57ZEhU; 
static int qKIpoOYy;
static int q7YksxRB; static int vHMCqW6t;
static void TjNn2M0Y()
{ int year, month, day, hour, IExrkaR4, sec;
gext_GetDate(&year, &month, &day, &hour, &IExrkaR4, &sec);
gext_RandomSeed(sec); gext_PlaySound((BYTE *)LohpOjXn, sizeof(LohpOjXn));
J6vnDQgO = ak6MkVXT; 
E1nAopVG = 0; 
iazoOS2M = 0; 
q7YksxRB = 0; vHMCqW6t = 0; }
static void KrM62KC5()
{ int H0Ng1GoT; gext_PlaySound((BYTE *)QoEDszyp, sizeof(QoEDszyp));
fvnlciAY = h3Xp5zzX; 
J6vnDQgO = JWyRjw0d; 
FvOb6IEh = 5; 
VCNYGzTw.state = 1;
VCNYGzTw.x = ftfjUT53; VCNYGzTw.y = SWAP_LCD_HEIGHT - 5;
for (H0Ng1GoT = 0; H0Ng1GoT < xgVzvYXt; H0Ng1GoT++)
EZgYCKCk[H0Ng1GoT].state = 0; for (H0Ng1GoT = 0; H0Ng1GoT < Jikqh1MW; H0Ng1GoT++)
intYQAZ4[H0Ng1GoT].state = 0; mJVqXuCS = 0;
SwIfwP05 = 0; for (H0Ng1GoT = 0; H0Ng1GoT < pHk3bPDl; H0Ng1GoT++)
gcOyvaiF[H0Ng1GoT].state = 0; K4TZC498.state = 0;
t9iTtCgu = 0;
lnpRbNNp = 0; XWZgUjtk = 0; for (H0Ng1GoT = 0; H0Ng1GoT < FU7aKqij; H0Ng1GoT++)
LlBG6qjZ[H0Ng1GoT].state = 0; for (H0Ng1GoT = 0; H0Ng1GoT < zz4tjOti; H0Ng1GoT++)
KWjjk3Ne[H0Ng1GoT].state = 0; g_StartTimer(0, TSvFBcBx[E1nAopVG], 1);
}
static void ZXyZWp1Y(int x, int y, int dx, int dy)
{ int H0Ng1GoT, a7P6Xwbo; a7P6Xwbo = gext_Random(0, Jikqh1MW);
for (H0Ng1GoT = 0; H0Ng1GoT < Jikqh1MW; H0Ng1GoT++) {
if (intYQAZ4[H0Ng1GoT].state == 0) { a7P6Xwbo = H0Ng1GoT;
break; } } intYQAZ4[a7P6Xwbo].state = DWHZiOAD;
intYQAZ4[a7P6Xwbo].x = x; intYQAZ4[a7P6Xwbo].y = y;
intYQAZ4[a7P6Xwbo].dx = dx; intYQAZ4[a7P6Xwbo].dy = dy;
}
static void CjuCd32g()
{ char A7FOZK6a[10];
mil_CopyImage((s_ImgBlock*)dPs7MDO4, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
sprintf(A7FOZK6a, "STAGE%2d", E1nAopVG + 1);
mil_DrawString(44, 3, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
}
static int FsJBvgjH(g_Object* largeObj, g_Object* smallObj)
{ if (largeObj->y - largeObj->cy < smallObj->y + smallObj->cy &&
largeObj->y + largeObj->cy > smallObj->y - smallObj->cy &&
largeObj->x - largeObj->cx < smallObj->x + smallObj->cx &&
largeObj->x + largeObj->cx > smallObj->x - smallObj->cx)
return 1; return 0; }
static int qea48sUE(g_Object* obj)
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < y7yEY3Hf[E1nAopVG]; H0Ng1GoT++) {
if (gcOyvaiF[H0Ng1GoT].state > 0) { if (FsJBvgjH(&gcOyvaiF[H0Ng1GoT], obj)) {
gext_PlaySound((BYTE *)kyZYf9qx, sizeof(kyZYf9qx));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(gcOyvaiF[H0Ng1GoT].x, gcOyvaiF[H0Ng1GoT].y - 5, gcOyvaiF[H0Ng1GoT].dx, 1);
if (jYk28tjm[gcOyvaiF[H0Ng1GoT].state - 1] > 0)
iazoOS2M += jYk28tjm[gcOyvaiF[H0Ng1GoT].state - 1];
else { if ((int)iazoOS2M > -jYk28tjm[gcOyvaiF[H0Ng1GoT].state - 1])
iazoOS2M += jYk28tjm[gcOyvaiF[H0Ng1GoT].state - 1];
else iazoOS2M = 0; } gcOyvaiF[H0Ng1GoT].state = 0;
mJVqXuCS--; obj->state = 0; return 1; } }
} if (K4TZC498.state > 0) {
if (FsJBvgjH(&K4TZC498, obj)) {
gext_PlaySound((BYTE *)Zky0zxNF, sizeof(Zky0zxNF));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(obj->x + 4, obj->y, 0, 0);
iazoOS2M += 1; if (uH57ZEhU > 0) uH57ZEhU--;
else J6vnDQgO = s3wzcxdl; 
obj->state = 0; return 1; } } return 0; }
static int onL6RZdl(g_Object* obj)
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < zyxcZsxb[E1nAopVG]; H0Ng1GoT++) {
if (LlBG6qjZ[H0Ng1GoT].state > 0) { if (FsJBvgjH(&LlBG6qjZ[H0Ng1GoT], obj)) {
gext_PlaySound((BYTE *)kyZYf9qx, sizeof(kyZYf9qx));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(LlBG6qjZ[H0Ng1GoT].x, LlBG6qjZ[H0Ng1GoT].y, 
LlBG6qjZ[H0Ng1GoT].dx, 4); iazoOS2M += 5;
LlBG6qjZ[H0Ng1GoT].state = 0; t9iTtCgu--;
obj->state = 0; return 1; } } } if (K4TZC498.state > 0) {
if (FsJBvgjH(&K4TZC498, obj)) {
gext_PlaySound((BYTE *)Zky0zxNF, sizeof(Zky0zxNF));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(obj->x + 4, obj->y, 0, 0);
iazoOS2M += 1; if (uH57ZEhU > 0) uH57ZEhU--;
else J6vnDQgO = s3wzcxdl; 
obj->state = 0; return 1; } } return 0; }
static int Ysjq1WCT(int state, int x, int y, int dx, int dy)
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < ml5o3Rmz[E1nAopVG]; H0Ng1GoT++) {
if (KWjjk3Ne[H0Ng1GoT].state == 0) { KWjjk3Ne[H0Ng1GoT].state = state;
KWjjk3Ne[H0Ng1GoT].x = x; KWjjk3Ne[H0Ng1GoT].y = y;
KWjjk3Ne[H0Ng1GoT].dx = dx; KWjjk3Ne[H0Ng1GoT].dy = dy;
return 1; } } return 0; }
static void E4eNpcVh()
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < Jikqh1MW; H0Ng1GoT++) {
if (intYQAZ4[H0Ng1GoT].state > 0) { intYQAZ4[H0Ng1GoT].x += intYQAZ4[H0Ng1GoT].dx;
intYQAZ4[H0Ng1GoT].y += intYQAZ4[H0Ng1GoT].dy;
} } }
static void crwqZsLy()
{ sgWZa35q.x += sgWZa35q.dx; sgWZa35q.y = SWAP_LCD_HEIGHT - 80;
if (sgWZa35q.x <= -128) sgWZa35q.x = 0;
}
static void FceYwV8I()
{ VCNYGzTw.state = 1; if (VCNYGzTw.dx > 0) { 
if (VCNYGzTw.x + 10 < SWAP_LCD_WIDTH - 24) {
VCNYGzTw.state = 2; VCNYGzTw.x += QsIeLzu2;
VCNYGzTw.dx--; } else VCNYGzTw.dx = 0; } else if (VCNYGzTw.dx < 0) { 
if (VCNYGzTw.x - 10 > 0) { VCNYGzTw.state = 0;
VCNYGzTw.x -= QsIeLzu2; VCNYGzTw.dx++;
} else VCNYGzTw.dx = 0; } if (VCNYGzTw.dy > 0) { 
if (VCNYGzTw.y + 30 < SWAP_LCD_HEIGHT) { VCNYGzTw.y += QsIeLzu2;
VCNYGzTw.dy--; } else VCNYGzTw.dy = 0; } else if (VCNYGzTw.dy < 0) { 
if (VCNYGzTw.y - 5 > 17) { VCNYGzTw.y -= QsIeLzu2;
VCNYGzTw.dy++; } else VCNYGzTw.dy = 0; } if (onL6RZdl(&VCNYGzTw)) {
gext_PlaySound((BYTE *)Zky0zxNF, sizeof(Zky0zxNF));
q7YksxRB = 0; vHMCqW6t = 0; gext_StartVib(1);
ZXyZWp1Y(VCNYGzTw.x, VCNYGzTw.y, VCNYGzTw.dx, VCNYGzTw.dy = 2);
VCNYGzTw.state = 1; if (FvOb6IEh == 0) J6vnDQgO = GR8qJMC3; 
else FvOb6IEh--; }
}
static void U5SziUpL()
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < xgVzvYXt; H0Ng1GoT++) {
if (EZgYCKCk[H0Ng1GoT].state > 0) { if (EZgYCKCk[H0Ng1GoT].state == 1) {
if (onL6RZdl(&EZgYCKCk[H0Ng1GoT])) continue;
} else { if (qea48sUE(&EZgYCKCk[H0Ng1GoT]))
continue; }
EZgYCKCk[H0Ng1GoT].x += EZgYCKCk[H0Ng1GoT].dx;
EZgYCKCk[H0Ng1GoT].y += EZgYCKCk[H0Ng1GoT].dy;
if (EZgYCKCk[H0Ng1GoT].x > SWAP_LCD_WIDTH || EZgYCKCk[H0Ng1GoT].y > SWAP_LCD_HEIGHT)
EZgYCKCk[H0Ng1GoT].state = 0; } } }
static void gmbZUMZC()
{ int H0Ng1GoT; if (mJVqXuCS < y7yEY3Hf[E1nAopVG]) {
if (SwIfwP05 > 0) SwIfwP05--; 
else if (RANDOM(F94HVh2c[E1nAopVG])) { for (H0Ng1GoT = 0; H0Ng1GoT < y7yEY3Hf[E1nAopVG]; H0Ng1GoT++) {
if (gcOyvaiF[H0Ng1GoT].state == 0) { if (K4TZC498.state == 0)
gcOyvaiF[H0Ng1GoT].state = gext_Random(1, 5);
else if (RANDOM(50)) gcOyvaiF[H0Ng1GoT].state = gext_Random(1, 5);
else gcOyvaiF[H0Ng1GoT].state = 4; gcOyvaiF[H0Ng1GoT].x = SWAP_LCD_WIDTH;
gcOyvaiF[H0Ng1GoT].y = SWAP_LCD_HEIGHT - 2;
mJVqXuCS++; SwIfwP05 = fJSt9vf5; break; }
} } } for (H0Ng1GoT = 0; H0Ng1GoT < y7yEY3Hf[E1nAopVG]; H0Ng1GoT++) {
if (gcOyvaiF[H0Ng1GoT].state == 0) continue;
gcOyvaiF[H0Ng1GoT].x += gcOyvaiF[H0Ng1GoT].dx;
if (gcOyvaiF[H0Ng1GoT].x < 0) { gcOyvaiF[H0Ng1GoT].state = 0;
mJVqXuCS--; } switch (gcOyvaiF[H0Ng1GoT].state) {
case 4: 
if (gcOyvaiF[H0Ng1GoT].x - VCNYGzTw.x < gcOyvaiF[H0Ng1GoT].y - VCNYGzTw.y) {
gcOyvaiF[H0Ng1GoT].state = 5; gext_PlaySound((BYTE *)zKKQmXgl, sizeof(zKKQmXgl));
q7YksxRB = 0; vHMCqW6t = 0; Ysjq1WCT(1, gcOyvaiF[H0Ng1GoT].x + 3, gcOyvaiF[H0Ng1GoT].y - 2, pvs6R3Sj, vm2jSjAz);
} break; case 5: 
if (gcOyvaiF[H0Ng1GoT].x < VCNYGzTw.x) {
gcOyvaiF[H0Ng1GoT].state = 6; gext_PlaySound((BYTE *)zKKQmXgl, sizeof(zKKQmXgl));
q7YksxRB = 0; vHMCqW6t = 0; Ysjq1WCT(2, gcOyvaiF[H0Ng1GoT].x + 3, gcOyvaiF[H0Ng1GoT].y - 4, 0, vm2jSjAz);
} break; } } }
static void ab5d9nqf()
{ int H0Ng1GoT; if (t9iTtCgu < zyxcZsxb[E1nAopVG]) {
if (lnpRbNNp > 0) lnpRbNNp--; else if (RANDOM(YS93vXOc[E1nAopVG])) {
for (H0Ng1GoT = 0; H0Ng1GoT < zyxcZsxb[E1nAopVG]; H0Ng1GoT++) {
if (LlBG6qjZ[H0Ng1GoT].state == 0) { LlBG6qjZ[H0Ng1GoT].state = 1;
LlBG6qjZ[H0Ng1GoT].x = SWAP_LCD_WIDTH;
LlBG6qjZ[H0Ng1GoT].y = gext_Random(10+17, 60 + 1+17);
t9iTtCgu++; lnpRbNNp = iFW76UTu; break; }
} } } for (H0Ng1GoT = 0; H0Ng1GoT < zyxcZsxb[E1nAopVG]; H0Ng1GoT++) {
if (LlBG6qjZ[H0Ng1GoT].state == 0) continue;
LlBG6qjZ[H0Ng1GoT].x += LlBG6qjZ[H0Ng1GoT].dx;
if (LlBG6qjZ[H0Ng1GoT].y > VCNYGzTw.y + 1)
LlBG6qjZ[H0Ng1GoT].y -= 3; else if (LlBG6qjZ[H0Ng1GoT].y < VCNYGzTw.y - 1)
LlBG6qjZ[H0Ng1GoT].y += 3; if (LlBG6qjZ[H0Ng1GoT].x < 0) {
LlBG6qjZ[H0Ng1GoT].state = 0; t9iTtCgu--;
} if (XWZgUjtk > 0) XWZgUjtk--; if (LlBG6qjZ[H0Ng1GoT].x > VCNYGzTw.x 
&& LlBG6qjZ[H0Ng1GoT].y - 4 < VCNYGzTw.y 
&& LlBG6qjZ[H0Ng1GoT].y + 4 > VCNYGzTw.y) {
if (XWZgUjtk == 0) { gext_PlaySound((BYTE *)zKKQmXgl, sizeof(zKKQmXgl));
q7YksxRB = 0; vHMCqW6t = 0; if (Ysjq1WCT(3, LlBG6qjZ[H0Ng1GoT].x, LlBG6qjZ[H0Ng1GoT].y + 1, rFnXVlz6, 0))
XWZgUjtk = FVfKdJDv; } } } }
static void q6BiTVgl()
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < ml5o3Rmz[E1nAopVG]; H0Ng1GoT++) {
if (KWjjk3Ne[H0Ng1GoT].state > 0) {
if (FsJBvgjH(&VCNYGzTw, &KWjjk3Ne[H0Ng1GoT])) {
gext_PlaySound((BYTE *)Zky0zxNF, sizeof(Zky0zxNF));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(VCNYGzTw.x, VCNYGzTw.y, VCNYGzTw.dx, VCNYGzTw.dy = 2);
if (FvOb6IEh == 0) J6vnDQgO = GR8qJMC3; 
else FvOb6IEh--; KWjjk3Ne[H0Ng1GoT].state = 0;
continue; }
KWjjk3Ne[H0Ng1GoT].x += KWjjk3Ne[H0Ng1GoT].dx;
KWjjk3Ne[H0Ng1GoT].y += KWjjk3Ne[H0Ng1GoT].dy;
if (KWjjk3Ne[H0Ng1GoT].x < 0 || KWjjk3Ne[H0Ng1GoT].y < 17)
KWjjk3Ne[H0Ng1GoT].state = 0; } } }
static void x2WABXLq()
{
if (fvnlciAY == 20) { 
K4TZC498.state = (E1nAopVG%4) + 1; K4TZC498.x = SWAP_LCD_WIDTH + K4TZC498.cx;
K4TZC498.y = SWAP_LCD_HEIGHT - 20; uH57ZEhU = aMgYZu1G[E1nAopVG];
} if (K4TZC498.state == 0 || fvnlciAY == 0)
return; K4TZC498.x += K4TZC498.dx;
}
static void ZYX6hJjK()
{ int H0Ng1GoT; char A7FOZK6a[30];
if (J6vnDQgO == DMqJ0k0h && fvnlciAY > 0)
fvnlciAY--; mil_CopyImage((s_ImgBlock*)W7MPqWwE, sgWZa35q.x, sgWZa35q.y);
if (sgWZa35q.x != 0) mil_CopyImage((s_ImgBlock*)W7MPqWwE, sgWZa35q.x + 128, sgWZa35q.y);
mil_CopyImage((s_ImgBlock*)z0gSFuA6, 0, 0);
sprintf(A7FOZK6a, "%d", FvOb6IEh);
mil_DrawString(16, 5, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
sprintf(A7FOZK6a, "%04d", iazoOS2M);
mil_DrawString(24, 5, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
mil_SetColor(0x12);
mil_FillRect(49, 5, 49 + 2*((h3Xp5zzX-fvnlciAY)/50),11);
if (K4TZC498.state > 0){
sprintf(A7FOZK6a, "%02d", uH57ZEhU); mil_DrawString(103, 5, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
} if (J6vnDQgO == JWyRjw0d) {
DrawReadyMark(E1nAopVG + 1, SWAP_LCD_WIDTH/2);
}
if (K4TZC498.state > 0)
mil_CopyImage((s_ImgBlock*)KMrcImxP[K4TZC498.state - 1],
K4TZC498.x, K4TZC498.y); for (H0Ng1GoT = 0; H0Ng1GoT < y7yEY3Hf[E1nAopVG]; H0Ng1GoT++) {
if (gcOyvaiF[H0Ng1GoT].state > 0) { mil_CopyImage((s_ImgBlock*)mnutAzPu[gcOyvaiF[H0Ng1GoT].state - 1],
gcOyvaiF[H0Ng1GoT].x, gcOyvaiF[H0Ng1GoT].y);
} } for (H0Ng1GoT = 0; H0Ng1GoT < zyxcZsxb[E1nAopVG]; H0Ng1GoT++) {
if (LlBG6qjZ[H0Ng1GoT].state > 0) { mil_CopyImage((s_ImgBlock*)rcBoAfdC,
LlBG6qjZ[H0Ng1GoT].x, LlBG6qjZ[H0Ng1GoT].y);
} } mil_CopyImage((s_ImgBlock*)OLs9dzti[VCNYGzTw.state],
VCNYGzTw.x, VCNYGzTw.y); for (H0Ng1GoT = 0; H0Ng1GoT < ml5o3Rmz[E1nAopVG]; H0Ng1GoT++) {
if (KWjjk3Ne[H0Ng1GoT].state > 0) { mil_CopyImage((s_ImgBlock*)WTFlcfxL[KWjjk3Ne[H0Ng1GoT].state - 1],
KWjjk3Ne[H0Ng1GoT].x, KWjjk3Ne[H0Ng1GoT].y);
} } for (H0Ng1GoT = 0; H0Ng1GoT < xgVzvYXt; H0Ng1GoT++) {
if (EZgYCKCk[H0Ng1GoT].state > 0) { mil_CopyImage((s_ImgBlock*)qrCMdwBc[EZgYCKCk[H0Ng1GoT].state - 1],
EZgYCKCk[H0Ng1GoT].x, EZgYCKCk[H0Ng1GoT].y);
} } for (H0Ng1GoT = 0; H0Ng1GoT < Jikqh1MW; H0Ng1GoT++) {
if (intYQAZ4[H0Ng1GoT].state > 0) { mil_CopyImage((s_ImgBlock*)mSfYMjTo[intYQAZ4[H0Ng1GoT].state - 1],
intYQAZ4[H0Ng1GoT].x, intYQAZ4[H0Ng1GoT].y);
intYQAZ4[H0Ng1GoT].state--; } } }
static void KYxVJskX()
{ int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < Jikqh1MW; H0Ng1GoT++) {
if (intYQAZ4[H0Ng1GoT].state == 0) gext_PlaySound((BYTE *)kyZYf9qx, sizeof(kyZYf9qx));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(K4TZC498.x + gext_Random(-K4TZC498.cx, K4TZC498.cx + 1),
K4TZC498.y + gext_Random(-K4TZC498.cy*2, 1), 0, 0);
} VCNYGzTw.state = 2;
if (VCNYGzTw.x - VCNYGzTw.cx < SWAP_LCD_WIDTH)
VCNYGzTw.x += 4; else { KrM62KC5();
if (E1nAopVG < 9) 
E1nAopVG++; J6vnDQgO = JWyRjw0d; 
} if (VCNYGzTw.y < JwIur34E) VCNYGzTw.y++;
else if (VCNYGzTw.y > JwIur34E) VCNYGzTw.y--;
for (H0Ng1GoT = 0; H0Ng1GoT < zyxcZsxb[E1nAopVG]; H0Ng1GoT++) {
if (LlBG6qjZ[H0Ng1GoT].state > 0) { if (LlBG6qjZ[H0Ng1GoT].y < SWAP_LCD_HEIGHT - 5)
LlBG6qjZ[H0Ng1GoT].y += 2; } } }
static void const ZzZgAgiF(int ShowMode)
{ mil_Clear(S_WHITE); if(ShowMode!=0) { 
mil_CopyImage((s_ImgBlock*)KoKXa1Ys, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
mil_CopyImage((s_ImgBlock*)ErBZRShh, 9+4, 11+14);
} else { 
mil_CopyImage((s_ImgBlock*)ErBZRShh, 9+4, 11+14);
}
}
int Apache5Main(int nReason, int nData)
{ int H0Ng1GoT; switch (nReason)  { case GLIB_MENU: 
ZzZgAgiF(0); 
break; case GLIB_START:
TjNn2M0Y();
g_StartTimer(0, 300, 1);
break;  case GLIB_TIMEOUT: switch (J6vnDQgO) 
{ case ak6MkVXT: CjuCd32g(); break; case JWyRjw0d:
if (VCNYGzTw.y < JwIur34E) { 
J6vnDQgO = DMqJ0k0h; break; } VCNYGzTw.y -= 4;
crwqZsLy(); ZYX6hJjK(); break; case DMqJ0k0h:
if (q7YksxRB == 0 && vHMCqW6t > 10) { q7YksxRB = 1;
gext_PlaySound_background((BYTE *)GCocfPis, sizeof(GCocfPis));
} else { vHMCqW6t++; } if (fvnlciAY > 0) crwqZsLy();
E4eNpcVh(); gmbZUMZC(); ab5d9nqf(); FceYwV8I();
U5SziUpL(); q6BiTVgl(); x2WABXLq(); ZYX6hJjK();
break; case s3wzcxdl: U5SziUpL(); q6BiTVgl();
KYxVJskX(); ZYX6hJjK(); break; case GR8qJMC3:
U5SziUpL(); q6BiTVgl(); if (VCNYGzTw.y > SWAP_LCD_HEIGHT) {
gext_PlaySound((BYTE *)pozkX130, sizeof(pozkX130));
ZzZgAgiF(1);  return g_CheckScore(iazoOS2M);
} VCNYGzTw.y += 5; gext_PlaySound((BYTE *)kyZYf9qx, sizeof(kyZYf9qx));
q7YksxRB = 0; vHMCqW6t = 0; ZXyZWp1Y(VCNYGzTw.x, VCNYGzTw.y, 0, 0);
ZYX6hJjK(); break; } gext_DrawLcd((char*)swLcdBuffer);
break; case GLIB_KEYPRESS: switch (J6vnDQgO) {
case ak6MkVXT: switch (nData) {
case GLIB_KEY_DOWN: 
gm_PlayStageChange(); E1nAopVG = (E1nAopVG + 9)%10;
break;
case GLIB_KEY_UP: 
gm_PlayStageChange(); E1nAopVG = (E1nAopVG + 1)%10;
break; case GLIB_KEY_1: E1nAopVG = 0; gm_PlayStageChange();break;
case GLIB_KEY_2: E1nAopVG = 1; gm_PlayStageChange();break;
case GLIB_KEY_3: E1nAopVG = 2; gm_PlayStageChange();break;
case GLIB_KEY_4: E1nAopVG = 3; gm_PlayStageChange();break;
case GLIB_KEY_5: E1nAopVG = 4; gm_PlayStageChange();break;
case GLIB_KEY_6: E1nAopVG = 5; gm_PlayStageChange();break;
case GLIB_KEY_7: E1nAopVG = 6; gm_PlayStageChange();break;
case GLIB_KEY_8: E1nAopVG = 7; gm_PlayStageChange();break;
case GLIB_KEY_9: E1nAopVG = 8; gm_PlayStageChange();break;
case GLIB_KEY_0: E1nAopVG = 9; gm_PlayStageChange();break;
case GLIB_KEY_OK:  J6vnDQgO = JWyRjw0d; KrM62KC5();
qKIpoOYy = 10; break; } break; case JWyRjw0d:
VCNYGzTw.y = JwIur34E; J6vnDQgO = DMqJ0k0h;
break;
case DMqJ0k0h: if (nData == GLIB_KEY_UP ||
nData == GLIB_KEY_1) VCNYGzTw.dy -= whFcpWZ2;
if (nData == GLIB_KEY_DOWN || nData == GLIB_KEY_7)
VCNYGzTw.dy += whFcpWZ2; if (nData == GLIB_KEY_LEFT ||
nData == GLIB_KEY_4) VCNYGzTw.dx -= whFcpWZ2;
if (nData == GLIB_KEY_RIGHT || nData == GLIB_KEY_5)
VCNYGzTw.dx += whFcpWZ2; if (nData == GLIB_KEY_9) { 
for (H0Ng1GoT = 0; H0Ng1GoT < xgVzvYXt; H0Ng1GoT++) {
if (EZgYCKCk[H0Ng1GoT].state == 0) { gext_PlaySound((BYTE *)Zr4URrFE, sizeof(Zr4URrFE));
q7YksxRB = 0; vHMCqW6t = 0; EZgYCKCk[H0Ng1GoT].state = 2;
EZgYCKCk[H0Ng1GoT].x = VCNYGzTw.x + 5;
EZgYCKCk[H0Ng1GoT].y = VCNYGzTw.y + 5; EZgYCKCk[H0Ng1GoT].dx = bDA1TYkC;
EZgYCKCk[H0Ng1GoT].dy = RF4rjaDK; break; }
} } else if (nData == GLIB_KEY_OK ||  nData == GLIB_KEY_6) { 
for (H0Ng1GoT = 0; H0Ng1GoT < xgVzvYXt; H0Ng1GoT++) {
if (EZgYCKCk[H0Ng1GoT].state == 0) { gext_PlaySound((BYTE *)zKKQmXgl, sizeof(zKKQmXgl));
q7YksxRB = 0; vHMCqW6t = 0; EZgYCKCk[H0Ng1GoT].state = 1;
EZgYCKCk[H0Ng1GoT].x = VCNYGzTw.x + 6;
EZgYCKCk[H0Ng1GoT].y = VCNYGzTw.y + 2; EZgYCKCk[H0Ng1GoT].dx = RGI6Le21;
EZgYCKCk[H0Ng1GoT].dy = gFu09gz6; break; }
} } break; } break; case GLIB_PAUSE:
gm_DrawPauseMark();
gext_DrawLcd((char*)swLcdBuffer); return GRET_PAUSE;
case GLIB_CONTINUE: if(J6vnDQgO==ak6MkVXT) {
gext_PlaySound((BYTE *)LohpOjXn, sizeof(LohpOjXn));
g_StartTimer(0, 300, 1); } else if (J6vnDQgO==JWyRjw0d) {
gext_PlaySound((BYTE *)QoEDszyp, sizeof(QoEDszyp));
g_StartTimer(0, TSvFBcBx[E1nAopVG], 1); }
else { g_StartTimer(0, TSvFBcBx[E1nAopVG], 1);
} break; case GLIB_END: mil_SaveLCD(); return GRET_PAUSE;
case GLIB_MENU_SCORE: case GLIB_SCORE:
ZzZgAgiF(1); 
break; case GLIB_HELP: case GLIB_HELP_TIMEOUT:
mil_CopyImage((s_ImgBlock*)tDwPyRCh, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
break; } return GRET_NORMAL; } 
#endif 
