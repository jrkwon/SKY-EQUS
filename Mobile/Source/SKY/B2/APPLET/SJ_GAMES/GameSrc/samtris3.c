//==============================================================================
//
//	SINJISOFT GAME
//
//	FILE NAME
//		Samtis.cpp
//
//	DESCRIPTION
//		삼트리스 게임
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
extern void gm_PlayStageChange(); 
extern void DrawReadyMark(int stageNum, int nX);
extern void DrawOverMark(void); 
#define ak6MkVXT 0 

#define mRHSnFp1 1

#define JWyRjw0d 2

#define FJpqwQUt 3

#define axN8G8Je 4

#define cXA0tYDE 5 

#define etGPD48C 6

#define GR8qJMC3 7

#define IJ6fvIPs 0

#define jfHXujyk 1

#define ElxXNkY5 2

#define OLapCl7K 3

#define I3FF1zrK 4

#define GXjTjVCm 5

#define sCs89RUJ 6

#define Z7rvBEA2 7

#define dlySRkUp 8

#define IC3PpXdW 9

#define JcJgonyG 10

#define XwB4p1g8 11

#define kn7Xrhqz 11

#define pEmCjWTy 20

#define HhZEMHol(a7P6Xwbo) ((a7P6Xwbo) == jfHXujyk || (a7P6Xwbo) == ElxXNkY5 || \
(a7P6Xwbo) == OLapCl7K || (a7P6Xwbo) == I3FF1zrK || \
(a7P6Xwbo) == sCs89RUJ || (a7P6Xwbo) == dlySRkUp || (a7P6Xwbo) == JcJgonyG)

#define Z57ZEglC 600

#define bddWlOUt 190

#include "ma2Samtris3.cpp"

#include "mapSamtris3.cpp"
static const BYTE *const JgF4Gutj[] = {
ERpHYnbb, azKceFq5, aqq8cP6P, j2iu1WKa, 
OqMZLlPn, 
Bj6rsXgV, 
oXyy9SsS, 
srjRkrV5, 
socA4FPW, 
XKtQ2CWQ 
}; static WORD iazoOS2M; 
static int J6vnDQgO; 
static int E1nAopVG; 
static int qNLHJYTJ; static int const Oknweiik[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
static int const VEVx5eMH[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
static int sMffNVB8; 
static int YypumJ13; static int ydv7K9nw[3]; 
static int lrsszqOQ[3]; static int Fk6Q9DYG[3]; 
static int Gmz3MYsl[3]; 
static int D22w2Rso; 
static int A2dWaSpb; static int MUHAV5iy[3]; 
static int nV8ty1oU[3]; static int dkETUzdI[3]; 
static int tNbaxIQz[3]; 
static int HrtNJt2h[kn7Xrhqz][pEmCjWTy]; 
static int PN5mpiGe[pEmCjWTy]; static int gFukqubo;
static int Ym2fl3ZV; static void OWRIs8Dy()
{ mil_CopyImage((s_ImgBlock*)rYrAq3Ls, 0, 0);
mil_SaveLCD(); }
static void M8ngRBvH()
{ BYTE LbPt5vw8, y; int year, month, day, hour, IExrkaR4, sec;
gext_GetDate(&year, &month, &day, &hour, &IExrkaR4, &sec);
gext_RandomSeed(sec);
J6vnDQgO = ak6MkVXT; 
E1nAopVG = 0; 
iazoOS2M = 0; 
gFukqubo = Z57ZEglC; for (LbPt5vw8 = 1; LbPt5vw8 < kn7Xrhqz - 1; LbPt5vw8++) {
HrtNJt2h[LbPt5vw8][0] = XwB4p1g8; HrtNJt2h[LbPt5vw8][pEmCjWTy - 1] = XwB4p1g8;
for (y = 1; y < pEmCjWTy - 1; y++) HrtNJt2h[LbPt5vw8][y] = IJ6fvIPs;
} for (y = 1; y < pEmCjWTy - 1; y++) { HrtNJt2h[0][y] = XwB4p1g8;
HrtNJt2h[kn7Xrhqz - 1][y] = XwB4p1g8; PN5mpiGe[y] = 0;
} HrtNJt2h[0][0] = XwB4p1g8; HrtNJt2h[0][pEmCjWTy - 1] = XwB4p1g8;
HrtNJt2h[kn7Xrhqz - 1][0] = XwB4p1g8; HrtNJt2h[kn7Xrhqz - 1][pEmCjWTy - 1] = XwB4p1g8;
} static void bNYNJhvq() {
if (Gmz3MYsl[0] == jfHXujyk) mil_CopyImage((s_ImgBlock*)ERpHYnbb, 100, 37);
else if (Gmz3MYsl[0] == ElxXNkY5) mil_CopyImage((s_ImgBlock*)azKceFq5, 100, 37);
else mil_CopyImage((s_ImgBlock*)aqq8cP6P, 100, 37);
if (Gmz3MYsl[1] == jfHXujyk) mil_CopyImage((s_ImgBlock*)ERpHYnbb, 100 + ydv7K9nw[1] * 9, 37 - lrsszqOQ[1] * 9);
else if (Gmz3MYsl[1] == ElxXNkY5) mil_CopyImage((s_ImgBlock*)azKceFq5, 100 + ydv7K9nw[1] * 9, 37 - lrsszqOQ[1] * 9);
else if (Gmz3MYsl[1] == OLapCl7K) mil_CopyImage((s_ImgBlock*)aqq8cP6P, 100 + ydv7K9nw[1] * 9, 37 - lrsszqOQ[1] * 9);
else mil_CopyImage((s_ImgBlock*)JgF4Gutj[Gmz3MYsl[1] - 1], 100 + ydv7K9nw[1] * 9, 37 - lrsszqOQ[1] * 9);
if (Gmz3MYsl[2] == jfHXujyk) mil_CopyImage((s_ImgBlock*)ERpHYnbb, 100 + ydv7K9nw[2] * 9, 37 - lrsszqOQ[2] * 9);
else if (Gmz3MYsl[2] == ElxXNkY5) mil_CopyImage((s_ImgBlock*)azKceFq5, 100 + ydv7K9nw[2] * 9, 37 - lrsszqOQ[2] * 9);
else if (Gmz3MYsl[2] == OLapCl7K) mil_CopyImage((s_ImgBlock*)aqq8cP6P, 100 + ydv7K9nw[2] * 9, 37 - lrsszqOQ[2] * 9);
else mil_CopyImage((s_ImgBlock*)JgF4Gutj[Gmz3MYsl[2] - 1], 100 + ydv7K9nw[2] * 9, 37 - lrsszqOQ[2] * 9);
} static void NOq6T7uE() { char A7FOZK6a[10];
if(Ym2fl3ZV) OWRIs8Dy();
mil_RestoreLCD();
sprintf(A7FOZK6a, "%02d", E1nAopVG + 1);
mil_DrawString(108, 94, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
sprintf(A7FOZK6a, "%06d", iazoOS2M); mil_DrawString(89, 134, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
bNYNJhvq(); }
static void WZMEppzp()
{ char A7FOZK6a[10];
mil_CopyImage((s_ImgBlock*)dPs7MDO4, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
sprintf(A7FOZK6a, "STAGE%2d", E1nAopVG + 1);
mil_DrawString(44, 3, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
}
static void ZzZgAgiF(int ShowMode)
{ mil_Clear(S_WHITE); if (ShowMode != 0) 
{ mil_CopyImage((s_ImgBlock*)XnKWyIqj, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
mil_CopyImage((s_ImgBlock*)cpS72k5g, 9+4+15, 11+15);
}
else 
{
mil_CopyImage((s_ImgBlock*)cpS72k5g, 9+4+15, 11+15);
}
} void uigs_MakeNewDownBlock()
{ sMffNVB8 = 5; YypumJ13 = 18; ydv7K9nw[0] = 0;
lrsszqOQ[0] = 0; Gmz3MYsl[0] = gext_Random(jfHXujyk, OLapCl7K+1);
Fk6Q9DYG[1] = (RANDOM(80)) ? 0 : 1; ydv7K9nw[1] = Oknweiik[Fk6Q9DYG[1]];
lrsszqOQ[1] = VEVx5eMH[Fk6Q9DYG[1]]; Gmz3MYsl[1] = (RANDOM(70)) ? gext_Random(jfHXujyk, OLapCl7K+1) : 4 + (rand()%5 );
Fk6Q9DYG[2] = 2 + (rand()%3); ydv7K9nw[2] = Oknweiik[Fk6Q9DYG[2]];
lrsszqOQ[2] = VEVx5eMH[Fk6Q9DYG[2]]; Gmz3MYsl[2] = (RANDOM(95)) ? gext_Random(jfHXujyk, OLapCl7K+1) : 4 + (rand()%5 );
} static void u3mefNIN() { uigs_MakeNewDownBlock();
gFukqubo = Z57ZEglC - E1nAopVG * 45;
} static void xHMPxxim() { D22w2Rso = sMffNVB8;
A2dWaSpb = YypumJ13; MUHAV5iy[0] = ydv7K9nw[0];
nV8ty1oU[0] = lrsszqOQ[0]; tNbaxIQz[0] = Gmz3MYsl[0];
dkETUzdI[1] = Fk6Q9DYG[1]; MUHAV5iy[1] = ydv7K9nw[1];
nV8ty1oU[1] = lrsszqOQ[1]; tNbaxIQz[1] = Gmz3MYsl[1];
dkETUzdI[2] = Fk6Q9DYG[2]; MUHAV5iy[2] = ydv7K9nw[2];
nV8ty1oU[2] = lrsszqOQ[2]; tNbaxIQz[2] = Gmz3MYsl[2];
} static void KCZrlECf(int ix, int iy, int type)
{ int dx = (ix - 1)*9 + 1; int dy = ((18 - iy) - 1)*9 + 7;
mil_CopyImage((s_ImgBlock*)JgF4Gutj[type - 1], dx, dy-1);
} static void djt0YVI3() { int LbPt5vw8, y, H0Ng1GoT;
NOq6T7uE(); for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++)
{ for (y = 1; y <= 17; y++) { if (HrtNJt2h[LbPt5vw8][y] > IJ6fvIPs)
{ KCZrlECf(LbPt5vw8, y, HrtNJt2h[LbPt5vw8][y]);
} } } for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++)
{ KCZrlECf(D22w2Rso + MUHAV5iy[H0Ng1GoT], A2dWaSpb + nV8ty1oU[H0Ng1GoT], tNbaxIQz[H0Ng1GoT]);
} } static int ZzWSs0wf(int dx, int dy) {
int H0Ng1GoT; int LbPt5vw8, y; for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++) {
LbPt5vw8 = D22w2Rso + MUHAV5iy[H0Ng1GoT] + dx;
y = A2dWaSpb + nV8ty1oU[H0Ng1GoT] + dy; if (LbPt5vw8 < 0 || y < 0 || HrtNJt2h[LbPt5vw8][y] != IJ6fvIPs)
return 1; } return 0; } static void ytAXwdeD()
{ dkETUzdI[1] = (dkETUzdI[1] + 2)%8; MUHAV5iy[1] = Oknweiik[dkETUzdI[1]];
nV8ty1oU[1] = VEVx5eMH[dkETUzdI[1]]; dkETUzdI[2] = (dkETUzdI[2] + 2)%8;
MUHAV5iy[2] = Oknweiik[dkETUzdI[2]]; nV8ty1oU[2] = VEVx5eMH[dkETUzdI[2]];
if (ZzWSs0wf(0, 0) == 1) { if (ZzWSs0wf(+1, 0) == 0)
D22w2Rso++; else if (ZzWSs0wf(-1, 0) == 0)
D22w2Rso--;
else {
dkETUzdI[1] = (dkETUzdI[1] + 6)%8; MUHAV5iy[1] = Oknweiik[dkETUzdI[1]];
nV8ty1oU[1] = VEVx5eMH[dkETUzdI[1]]; dkETUzdI[2] = (dkETUzdI[2] + 6)%8;
MUHAV5iy[2] = Oknweiik[dkETUzdI[2]]; nV8ty1oU[2] = VEVx5eMH[dkETUzdI[2]];
} } } static void EC0D4E4K(int LbPt5vw8, int y)
{ short CYVLXJkW, qb0L1TPI; if (LbPt5vw8 < 1 || LbPt5vw8 > 9 || y < 1 || y > 16)
return; switch (HrtNJt2h[LbPt5vw8][y]) { case jfHXujyk:
case ElxXNkY5: case OLapCl7K: case I3FF1zrK:
case sCs89RUJ: case dlySRkUp: case JcJgonyG: 
HrtNJt2h[LbPt5vw8][y] = IC3PpXdW; break; case Z7rvBEA2:
HrtNJt2h[LbPt5vw8][y] = JcJgonyG; break; case GXjTjVCm:
HrtNJt2h[LbPt5vw8][y] = IC3PpXdW; for (CYVLXJkW = LbPt5vw8 - 1; CYVLXJkW <= LbPt5vw8 + 1; CYVLXJkW++)
for (qb0L1TPI = y - 1; qb0L1TPI <= y + 1; qb0L1TPI++) {
if (CYVLXJkW == LbPt5vw8 && qb0L1TPI == y)
continue; EC0D4E4K(CYVLXJkW, qb0L1TPI); }
break; } } static void L5ZdR4Tc() { int LbPt5vw8, y;
int H0Ng1GoT, hRldyQfl; int redraw = 0; int audio = 0; 
for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++) {
LbPt5vw8 = D22w2Rso + MUHAV5iy[H0Ng1GoT];
y = A2dWaSpb + nV8ty1oU[H0Ng1GoT]; PN5mpiGe[y]++;
if (HhZEMHol(tNbaxIQz[H0Ng1GoT])) { if ((tNbaxIQz[H0Ng1GoT]) == jfHXujyk ) HrtNJt2h[LbPt5vw8][y] = jfHXujyk;
else if ((tNbaxIQz[H0Ng1GoT]) == ElxXNkY5 ) HrtNJt2h[LbPt5vw8][y] = ElxXNkY5;
else if ((tNbaxIQz[H0Ng1GoT]) == OLapCl7K ) HrtNJt2h[LbPt5vw8][y] = OLapCl7K;
else HrtNJt2h[LbPt5vw8][y] = jfHXujyk; } else
HrtNJt2h[LbPt5vw8][y] = tNbaxIQz[H0Ng1GoT];
} for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++) 
{ if (tNbaxIQz[H0Ng1GoT] != dlySRkUp) continue;
audio = 1; hRldyQfl = 2; for (y = 1; hRldyQfl > 0 && y <= 17; y++)
{ for (LbPt5vw8 = 1; hRldyQfl > 0 && LbPt5vw8 <= 9; LbPt5vw8++)
{ if (HrtNJt2h[LbPt5vw8][y] == IJ6fvIPs && 
HrtNJt2h[LbPt5vw8][y - 1] != IJ6fvIPs && 
RANDOM(50))  { PN5mpiGe[y]++; HrtNJt2h[LbPt5vw8][y] = JcJgonyG;
redraw = 1; hRldyQfl--; } } } } for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++) {
if (tNbaxIQz[H0Ng1GoT] != sCs89RUJ) continue;
audio = 1; y = A2dWaSpb + nV8ty1oU[H0Ng1GoT];
LbPt5vw8 = D22w2Rso + MUHAV5iy[H0Ng1GoT] + 1;
while (HrtNJt2h[LbPt5vw8][y] == IJ6fvIPs) {
PN5mpiGe[y]++; HrtNJt2h[LbPt5vw8][y] = JcJgonyG;
redraw = 1; LbPt5vw8++; } LbPt5vw8 = D22w2Rso + MUHAV5iy[H0Ng1GoT] - 1;
while (HrtNJt2h[LbPt5vw8][y] == IJ6fvIPs) {
PN5mpiGe[y]++; HrtNJt2h[LbPt5vw8][y] = JcJgonyG;
redraw = 1; LbPt5vw8--; } } for (H0Ng1GoT = 0; H0Ng1GoT < 3; H0Ng1GoT++) {
if (tNbaxIQz[H0Ng1GoT] != I3FF1zrK) continue;
audio = 1; LbPt5vw8 = D22w2Rso + MUHAV5iy[H0Ng1GoT];
y = A2dWaSpb + nV8ty1oU[H0Ng1GoT]; for (hRldyQfl = 0; hRldyQfl < 2 && 
(HhZEMHol(HrtNJt2h[LbPt5vw8][y]) || HrtNJt2h[LbPt5vw8][y] == GXjTjVCm); hRldyQfl++) {
EC0D4E4K(LbPt5vw8, y); redraw = 1; y--; }
if (HrtNJt2h[LbPt5vw8][y] == Z7rvBEA2) HrtNJt2h[LbPt5vw8][y] = JcJgonyG;
} if (redraw == 1) { audio = 1; djt0YVI3();
redraw = 0; } for (y = 1; y <= 17; y++) {
if (PN5mpiGe[y] == 9)  { audio = 2; for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++)
EC0D4E4K(LbPt5vw8, y); } } switch(audio) {
case 0: gext_PlaySound((BYTE *)Mky7qcBz, sizeof(Mky7qcBz));
break; case 1: gext_PlaySound((BYTE *)W5tgSgYZ, sizeof(W5tgSgYZ));
break; case 2: gext_PlaySound((BYTE *)lBJqIvPC, sizeof(lBJqIvPC));
break; } }
static void BnykaMZu()
{ int LbPt5vw8, y, qb0L1TPI; int f; int line = 10;
int score = 0; for (y = 1; y <= 17; y++) {
if (PN5mpiGe[y] != 9) continue; f = 0; for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++) {
if (HrtNJt2h[LbPt5vw8][y] != IC3PpXdW) { f = 1;
break; } } if (f == 0) { for (qb0L1TPI = y + 1; qb0L1TPI <= 17; qb0L1TPI++) {
PN5mpiGe[qb0L1TPI - 1] = PN5mpiGe[qb0L1TPI];
for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++)
HrtNJt2h[LbPt5vw8][qb0L1TPI - 1] = HrtNJt2h[LbPt5vw8][qb0L1TPI];
} PN5mpiGe[17] = 0; for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++)
HrtNJt2h[LbPt5vw8][17] = IJ6fvIPs; y--; score += line;
line += 4; } } for (y = 17; y >= 1; y--) {
if (PN5mpiGe[y] == 0) continue; for (LbPt5vw8 = 1; LbPt5vw8 <= 9; LbPt5vw8++)
if (HrtNJt2h[LbPt5vw8][y] == IC3PpXdW)  {
HrtNJt2h[LbPt5vw8][y] = IJ6fvIPs; PN5mpiGe[y]--;
score++; } } if (iazoOS2M < 99999) { iazoOS2M += score;
} }
int Samtris3Main(int nReason, int nData)
{ switch (nReason)  { case GLIB_MENU: 
ZzZgAgiF(0);
break; case GLIB_START:
gext_PlaySound((BYTE *)rz60pzJp, sizeof(rz60pzJp));
M8ngRBvH(); g_StartTimer(0, gFukqubo, 1);
break;  case GLIB_TIMEOUT: if(nData==3) Ym2fl3ZV =1;
else Ym2fl3ZV =0; switch (J6vnDQgO)  { case ak6MkVXT:
WZMEppzp(); break; case JWyRjw0d: if (qNLHJYTJ <= 0)
{ J6vnDQgO = FJpqwQUt; xHMPxxim(); uigs_MakeNewDownBlock();
} NOq6T7uE(); DrawReadyMark(E1nAopVG + 1, SWAP_LCD_WIDTH/2-20);
qNLHJYTJ--; break; case FJpqwQUt: J6vnDQgO = axN8G8Je;
djt0YVI3(); if (gFukqubo > bddWlOUt) { gFukqubo -= 2;
g_StartTimer(0, gFukqubo, 1);
E1nAopVG = (int)((Z57ZEglC - gFukqubo) / 45);
} if (ZzWSs0wf(0, 0)) { 
qNLHJYTJ = 6; gext_PlaySound((BYTE *)Ts8OVQs5, sizeof(Ts8OVQs5));
J6vnDQgO = GR8qJMC3; } break; case axN8G8Je:
if (ZzWSs0wf(0, -1) == 0) { 
A2dWaSpb--; djt0YVI3(); } else { 
J6vnDQgO = cXA0tYDE; } break; case cXA0tYDE:
L5ZdR4Tc(); xHMPxxim(); uigs_MakeNewDownBlock();
djt0YVI3(); J6vnDQgO = etGPD48C; break; case etGPD48C:
BnykaMZu(); djt0YVI3(); J6vnDQgO = FJpqwQUt;
break; case GR8qJMC3: if (qNLHJYTJ < 0) {
ZzZgAgiF(1); 
return g_CheckScore(iazoOS2M); } else { djt0YVI3();
DrawOverMark();
qNLHJYTJ--; } break; } gext_DrawLcd((char*)swLcdBuffer);
break; case GLIB_KEYPRESS: switch (J6vnDQgO) {
case ak6MkVXT: switch (nData) { case GLIB_KEY_DOWN: 
E1nAopVG = (E1nAopVG + 9)%10; gm_PlayStageChange();
break; case GLIB_KEY_UP: 
E1nAopVG = (E1nAopVG + 1)%10; gm_PlayStageChange();
break; case GLIB_KEY_1: E1nAopVG = 0; gm_PlayStageChange(); break;
case GLIB_KEY_2: E1nAopVG = 1; gm_PlayStageChange(); break;
case GLIB_KEY_3: E1nAopVG = 2; gm_PlayStageChange(); break;
case GLIB_KEY_4: E1nAopVG = 3; gm_PlayStageChange(); break;
case GLIB_KEY_5: E1nAopVG = 4; gm_PlayStageChange(); break;
case GLIB_KEY_6: E1nAopVG = 5; gm_PlayStageChange(); break;
case GLIB_KEY_7: E1nAopVG = 6; gm_PlayStageChange(); break;
case GLIB_KEY_8: E1nAopVG = 7; gm_PlayStageChange(); break;
case GLIB_KEY_9: E1nAopVG = 8; gm_PlayStageChange(); break;
case GLIB_KEY_0: E1nAopVG = 9; gm_PlayStageChange(); break;
case GLIB_KEY_OK:  mil_Clear(S_WHITE); J6vnDQgO = JWyRjw0d;
OWRIs8Dy(); u3mefNIN(); qNLHJYTJ = 9; gext_PlaySound((BYTE *)lEtTo6yp, sizeof(lEtTo6yp));
break; } break; case JWyRjw0d: J6vnDQgO = FJpqwQUt;
xHMPxxim(); uigs_MakeNewDownBlock(); qNLHJYTJ=0;
break; case FJpqwQUt: case axN8G8Je:
case etGPD48C:
switch(nData) { case GLIB_KEY_LEFT: case GLIB_KEY_4:
if (ZzWSs0wf(-1, 0) == 0)  { D22w2Rso--; djt0YVI3();
} break; case GLIB_KEY_RIGHT: case GLIB_KEY_6:
if (ZzWSs0wf(+1, 0) == 0)  { D22w2Rso++; djt0YVI3();
} break; case GLIB_KEY_DOWN: case GLIB_KEY_8:
if (ZzWSs0wf(0, -1) == 0)  { A2dWaSpb--; djt0YVI3();
} break; case GLIB_KEY_OK: case GLIB_KEY_5:
ytAXwdeD(); djt0YVI3(); break; } gext_DrawLcd((char*)swLcdBuffer);
break; } break; case GLIB_PAUSE:
gm_DrawPauseMark();
gext_DrawLcd((char*)swLcdBuffer); return GRET_PAUSE;
case GLIB_CONTINUE: OWRIs8Dy(); if(J6vnDQgO==ak6MkVXT) gext_PlaySound((BYTE *)rz60pzJp, sizeof(rz60pzJp));
g_StartTimer(0, gFukqubo, 1); break; case GLIB_END:
mil_SaveLCD(); return GRET_PAUSE; case GLIB_MENU_SCORE:
case GLIB_SCORE:
ZzZgAgiF(1); 
break; case GLIB_HELP: case GLIB_HELP_TIMEOUT:
mil_CopyImage((s_ImgBlock*)tDwPyRCh, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
break; } return GRET_NORMAL; } 
#endif 
