//==============================================================================
//
//	SINJISOFT GAME
//
//	FILE NAME
//		Mars Scouter.cpp
//
//	DESCRIPTION
//		마스 스카우터 게임
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
extern void DrawClearMark(void); extern int s_Frame6;

#define bljeLxCR 99

#define ak6MkVXT 0 

#define JWyRjw0d 2

#define DMqJ0k0h 3

#define hcocB2XK 4

#define FoKeC2BO 5

#define JdwXljeo 6

#define axN8G8Je 94

#define cXA0tYDE 95

#define etGPD48C 96

#define GR8qJMC3 97

#define DNMVYa70 3

#define vctgWaNN 3

#define ust9nAqu 2

#define wejYFiXW 2

#define b960XAqm 0

#define cwmeT2iK 1

#define Xfpg6Hn4 48 

#define TJqtqEnm 12

#define jzPg7JQT 108

#define DxYvDeVv 2 

#define XLKqwhTr 0

#define zVi0iEzB 1

#define VCkMUNre SWAP_LCD_WIDTH

#define fAoBQfIH 8

#define qMlDGg6i 12

#define g6gRMlSa 6

#define zdsxsArb 0

#define oTGWmLqD 1

#define NQBohLTA 2

#define qmBDxVZb 3

#define M5Dv1WTN 4

#define hqTpGU7O -15

#define OoR8DqUq 135

#define e6GtTAi2 48

#define kke5vOAN 64

#define syaQov4s 6

#define maCfct8E 12

#define u7nUfFvO 0

#define LQbEUd6B 1

#define wW4vgom1 2

#define ace4BKJi 2

#define DuvYHyxN 4

#define pRq0WLKR 0

#define FAKGFvPB 1

#define spFS3AE7 2

#define mBvEXIEp -15 

#define CqoWUC6W 135

#define vkl2bPx2 4

#define y1oVYYEy 0

#define SIeOndNS 1

#define yqA9fxzR 2

#define CjwK0tTm -15

#define e3TWnYdJ 135

#define qp7pt9J5 6

#define hzuP5mfT 14

#define i1tmoCvP (SWAP_LCD_HEIGHT - hzuP5mfT)

#define arDxWcAw 9
static int EyBcRfs6; 
static int bV3A9vxX; 
static BYTE oMlvQYgP; 
static int RPDlrQhu; static int boDvlNKK;
static int smPxjsAd; static BYTE aIn8WRPy;
static const BYTE XMUpmb7R[arDxWcAw] = { hzuP5mfT + 8,
hzuP5mfT + 11, hzuP5mfT + 17, hzuP5mfT + 20,
hzuP5mfT + 22, hzuP5mfT + 20, hzuP5mfT + 17,
hzuP5mfT + 11, hzuP5mfT + 8 };

#include "ma2MarsScouter3.cpp" 

#include "mapMarsScouter3.cpp" 
static const BYTE * const aZa3lCGu[] = {
oP8MMZ97, MO2HgWaR, n7s0ugk2, cxZ785jD, i2IQyyp2,
mNgJIGFR }; static const BYTE * const lxlHBKk1[] = {
DAaJ3SLR, daFcbPl4 }; static const BYTE * const hMgJWd9P[] = {
CtRVT7oq, zekb6wVd }; static const BYTE * const Ea1fhBEB[] = {
IjdQamCc, fT7JQxwH }; static const BYTE * const fdH0qGwx[] = {
ErS0x3H0, vAJAREyO }; static const BYTE * const YvD3F4Vs[] = {
IsxUvW6g, xPHKzDq2, VaAZwN6e };
static int J6vnDQgO; 
static int E1nAopVG; 
static int qNLHJYTJ; static int gFukqubo;
static int ZiIxgvnh; static int zCgyj8Sh;
static int fn7UjdwX; static int viUz9Tj5;
static int QdcJfG4Q; static int g4PA0eRW;
static BYTE eYBbS5Lv; static BYTE poPO0nRf; 
static BYTE EUe8jnIv; 
static BYTE dRTvMgT8[DNMVYa70]; static BYTE ZovNcL71[DNMVYa70];
static BYTE GForC808[DNMVYa70]; static BYTE const OxNf6QGj[10] = { 50, 50, 60, 70, 80, 90, 90, 90, 90, 90 };
static BYTE const wPz6S9bI[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 90 };
static BYTE const CcCYf4km[10] = { 0, 0, 20, 30, 40, 50, 60, 70, 80, 90 };
static BYTE const K5HGhUAg[10] = { 10, 20, 20, 30, 40, 50, 60, 70, 80, 90 };
static int Q7uvooBr[DNMVYa70]; static int DDvEbYig[DNMVYa70];
static int wmKp6QpB[DNMVYa70]; static int Z0nhq0Kh[vctgWaNN];
static int zZmQ5tSj; static int VMOPFjty;
static int rCfdY3Fu[vctgWaNN]; static BYTE const oAzq8mq4[10] = { 1, 1, 2, 2, 3, 3, 3, 3, 3, 3 };
static int DTwBI3cA[ust9nAqu]; static int m68O0SN5;
static int e5qYfd72[ust9nAqu]; static BYTE const ZMXL8qNJ[10] = { 5, 5, 10, 10, 10, 10, 10, 10, 10, 10 };
static void xL4BtFZx()
{ int year, month, day, hour, IExrkaR4, sec;
gext_GetDate(&year, &month, &day, &hour, &IExrkaR4, &sec);
gext_RandomSeed(sec); J6vnDQgO = ak6MkVXT; 
gFukqubo = 200; E1nAopVG = 0; EyBcRfs6 = 0;
bV3A9vxX = 0; oMlvQYgP = 3; RPDlrQhu = 0;
boDvlNKK = 20; smPxjsAd = i1tmoCvP; aIn8WRPy = b960XAqm;
}
static void yJHeWr3w()
{ char A7FOZK6a[10];
mil_CopyImage((s_ImgBlock*)dPs7MDO4, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
sprintf(A7FOZK6a, "STAGE%2d", E1nAopVG + 1);
mil_DrawString(44, 3, A7FOZK6a, S_FONT_MEDIUM, S_BLACK, S_LEFT);
}
static void ZzZgAgiF(int ShowMode)
{ if (ShowMode != 0) { 
mil_CopyImage((s_ImgBlock*)XnKWyIqj, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2+14);
mil_CopyImage((s_ImgBlock*)dXCM2PAv, 6, 11+12);
}
else  { mil_Clear(S_WHITE);
mil_CopyImage((s_ImgBlock*)dXCM2PAv, 6, 11+12);
}
} static void jbceWr6S() { int H0Ng1GoT; boDvlNKK = 20;
smPxjsAd = i1tmoCvP; aIn8WRPy = b960XAqm;
ZiIxgvnh = XLKqwhTr; viUz9Tj5 = XLKqwhTr;
poPO0nRf = 0; EUe8jnIv = 0; eYBbS5Lv = 120;
for (H0Ng1GoT = 0; H0Ng1GoT < DNMVYa70; H0Ng1GoT++) {
dRTvMgT8[H0Ng1GoT] = zdsxsArb; Q7uvooBr[H0Ng1GoT] = u7nUfFvO;
} zZmQ5tSj = 0; VMOPFjty = 0; for (H0Ng1GoT = 0; H0Ng1GoT < vctgWaNN; H0Ng1GoT++)
Z0nhq0Kh[H0Ng1GoT] = pRq0WLKR; m68O0SN5 = 0;
for (H0Ng1GoT = 0; H0Ng1GoT < ust9nAqu; H0Ng1GoT++)
DTwBI3cA[H0Ng1GoT] = y1oVYYEy; }
void uigm_DrawGround()
{ BYTE H0Ng1GoT; char A7FOZK6a[5]; mil_Clear(0x28);
mil_CopyImage((s_ImgBlock*)Rma51LcP, RPDlrQhu, i1tmoCvP);
if (RPDlrQhu){ mil_CopyImage((s_ImgBlock*)Rma51LcP, RPDlrQhu + 128, i1tmoCvP);
}
for (H0Ng1GoT = 0; H0Ng1GoT < vctgWaNN; H0Ng1GoT++)
{ switch (Z0nhq0Kh[H0Ng1GoT])  { case FAKGFvPB:
mil_CopyImage((s_ImgBlock*)eGFYKa1T, rCfdY3Fu[H0Ng1GoT], i1tmoCvP+3);
break; case spFS3AE7: mil_CopyImage((s_ImgBlock*)QqVb5KzM, rCfdY3Fu[H0Ng1GoT], i1tmoCvP+3);
break; } } for (H0Ng1GoT = 0; H0Ng1GoT < ust9nAqu; H0Ng1GoT++)
{ switch (DTwBI3cA[H0Ng1GoT])  { case SIeOndNS:
mil_CopyImage((s_ImgBlock*)r28JSGVK, e5qYfd72[H0Ng1GoT], i1tmoCvP);
break; case yqA9fxzR: mil_CopyImage((s_ImgBlock*)BB0dh4Z5, e5qYfd72[H0Ng1GoT], i1tmoCvP);
break; } } mil_CopyImage((s_ImgBlock*)Jv1uBtJS, 0, 0);
for (H0Ng1GoT = 0; H0Ng1GoT < oMlvQYgP; H0Ng1GoT++)
{ mil_CopyImage((s_ImgBlock*)EbIHtrcv, 16 + H0Ng1GoT * 7, 4);
}
mil_CopyImage((s_ImgBlock*)uQR8wqqy, 41 + (EyBcRfs6 * 57 / bljeLxCR), 8);
sprintf(A7FOZK6a, "%04d", bV3A9vxX);  mil_DrawString(125, 3, A7FOZK6a, S_FONT_LARGE, S_BLACK, S_RIGHT);
} static void tVcmJMjw() { static BYTE bYB6cvqc = 0;
RPDlrQhu = RPDlrQhu - 2; if (RPDlrQhu <= -120) RPDlrQhu = 0;
if (J6vnDQgO == DMqJ0k0h) { bYB6cvqc++; if (bYB6cvqc >= 8)
{ bYB6cvqc = 0; EyBcRfs6++; if (EyBcRfs6 >= bljeLxCR) { gext_PlaySound((BYTE *)OE6kSgKR, sizeof(OE6kSgKR));
J6vnDQgO = hcocB2XK; 
qNLHJYTJ = 0; } if (EyBcRfs6 >= bljeLxCR)
EyBcRfs6 = bljeLxCR; } } } static void BE87rPkM()
{ switch (aIn8WRPy) { case cwmeT2iK: boDvlNKK += 2; 
smPxjsAd = SWAP_LCD_HEIGHT - XMUpmb7R[qNLHJYTJ++];
if (qNLHJYTJ > arDxWcAw) { aIn8WRPy = b960XAqm;
smPxjsAd = i1tmoCvP; } break; } } static void V9TuFoHp()
{ if (J6vnDQgO != JdwXljeo) { switch(aIn8WRPy)
{ case b960XAqm: mil_CopyImage((s_ImgBlock*)aZa3lCGu[s_Frame6], boDvlNKK, smPxjsAd);
break; case cwmeT2iK: if (qNLHJYTJ <= 1 || qNLHJYTJ >= 7)
mil_CopyImage((s_ImgBlock*)lxlHBKk1[0], boDvlNKK, smPxjsAd);
else mil_CopyImage((s_ImgBlock*)lxlHBKk1[1], boDvlNKK, smPxjsAd);
break; } } else  { mil_CopyImage((s_ImgBlock*)YvD3F4Vs[qNLHJYTJ % 3], boDvlNKK, smPxjsAd);
} } static void BFYhZ39Y() { BYTE check; BYTE H0Ng1GoT, hRldyQfl;
if (J6vnDQgO != JdwXljeo)  { if (zZmQ5tSj > 0)
zZmQ5tSj--; for (H0Ng1GoT = 0; H0Ng1GoT < vctgWaNN; H0Ng1GoT++) {
switch (Z0nhq0Kh[H0Ng1GoT]) { case pRq0WLKR:
continue; case FAKGFvPB: Z0nhq0Kh[H0Ng1GoT] = spFS3AE7;
break; } if (rCfdY3Fu[H0Ng1GoT] > mBvEXIEp)
rCfdY3Fu[H0Ng1GoT] -= wejYFiXW; else { Z0nhq0Kh[H0Ng1GoT] = pRq0WLKR;
VMOPFjty--; bV3A9vxX += 2; } if (aIn8WRPy == b960XAqm
&& HIT_CHECK(rCfdY3Fu[H0Ng1GoT], boDvlNKK, 10)) {
gext_PlaySound((BYTE *)n67EAbnG, sizeof(n67EAbnG));
J6vnDQgO = JdwXljeo; 
gext_StartVib(1); qNLHJYTJ = 0; } } if (m68O0SN5 > 0)
m68O0SN5--; for (H0Ng1GoT = 0; H0Ng1GoT < ust9nAqu; H0Ng1GoT++) {
switch (DTwBI3cA[H0Ng1GoT]) { case y1oVYYEy:
if (J6vnDQgO == DMqJ0k0h && EyBcRfs6 < bljeLxCR - 5
&& m68O0SN5 == 0 && RANDOM(ZMXL8qNJ[E1nAopVG])) {
DTwBI3cA[H0Ng1GoT] = SIeOndNS; e5qYfd72[H0Ng1GoT] = e3TWnYdJ;
m68O0SN5 = qp7pt9J5; } break; case SIeOndNS:
if (e5qYfd72[H0Ng1GoT] > CjwK0tTm)
e5qYfd72[H0Ng1GoT] -= wejYFiXW; else DTwBI3cA[H0Ng1GoT] = y1oVYYEy;
if (HIT_CHECK(e5qYfd72[H0Ng1GoT], boDvlNKK, 8)
&& i1tmoCvP - 20 < smPxjsAd) { gext_PlaySound((BYTE *)n67EAbnG, sizeof(n67EAbnG));
J6vnDQgO = JdwXljeo; 
gext_StartVib(1); qNLHJYTJ = 0; } break; case yqA9fxzR:
DTwBI3cA[H0Ng1GoT] = y1oVYYEy; break; } }
} for (H0Ng1GoT = 0; J6vnDQgO == DMqJ0k0h && H0Ng1GoT < DNMVYa70; H0Ng1GoT++) {
switch (dRTvMgT8[H0Ng1GoT]) { case zdsxsArb:
if (EyBcRfs6 < bljeLxCR - 5
&& ( (poPO0nRf == 0 && RANDOM(OxNf6QGj[E1nAopVG]))
|| (poPO0nRf == 1 && EyBcRfs6 > 30 && RANDOM(wPz6S9bI[E1nAopVG]))
|| (poPO0nRf == 2 && EyBcRfs6 > 60 && RANDOM(CcCYf4km[E1nAopVG])) ) ) {
poPO0nRf++; GForC808[H0Ng1GoT] = 26 + H0Ng1GoT*7;
if (RANDOM(50)) { dRTvMgT8[H0Ng1GoT] = qmBDxVZb;
ZovNcL71[H0Ng1GoT] = OoR8DqUq; } else { dRTvMgT8[H0Ng1GoT] = oTGWmLqD;
ZovNcL71[H0Ng1GoT] = hqTpGU7O; } } break;
case oTGWmLqD: if (EyBcRfs6 == bljeLxCR - 5) {
gext_PlaySound((BYTE *)QVlC2aGF, sizeof(QVlC2aGF));
dRTvMgT8[H0Ng1GoT] = M5Dv1WTN; break; } if (ZovNcL71[H0Ng1GoT] >= e6GtTAi2 && ZovNcL71[H0Ng1GoT] <= kke5vOAN
&& RANDOM(50) && EUe8jnIv == 0) { EUe8jnIv = maCfct8E;
dRTvMgT8[H0Ng1GoT] = NQBohLTA; } else if (ZovNcL71[H0Ng1GoT] < OoR8DqUq)
ZovNcL71[H0Ng1GoT] += syaQov4s; else dRTvMgT8[H0Ng1GoT] = qmBDxVZb;
break; case NQBohLTA: if (EyBcRfs6 == bljeLxCR - 5) {
gext_PlaySound((BYTE *)QVlC2aGF, sizeof(QVlC2aGF));
EUe8jnIv = 0; dRTvMgT8[H0Ng1GoT] = M5Dv1WTN;
break; } if (zZmQ5tSj == 0 && VMOPFjty < oAzq8mq4[E1nAopVG]
&& EyBcRfs6 < 90 && RANDOM(K5HGhUAg[E1nAopVG])) {
for (hRldyQfl = 0; hRldyQfl < DNMVYa70; hRldyQfl++) {
if (Q7uvooBr[hRldyQfl] != u7nUfFvO) continue;
gext_PlaySound((BYTE *)kYwwwuDd, sizeof(kYwwwuDd));
zZmQ5tSj = qp7pt9J5; if (RANDOM(70)) Q7uvooBr[hRldyQfl] = LQbEUd6B;
else Q7uvooBr[hRldyQfl] = wW4vgom1; DDvEbYig[hRldyQfl] = ZovNcL71[H0Ng1GoT];
wmKp6QpB[hRldyQfl] = GForC808[H0Ng1GoT]; break;
} } EUe8jnIv--;
if (EUe8jnIv == 0) { if (RANDOM(50)) { dRTvMgT8[H0Ng1GoT] = oTGWmLqD;
ZovNcL71[H0Ng1GoT] += syaQov4s; } else { dRTvMgT8[H0Ng1GoT] = qmBDxVZb;
ZovNcL71[H0Ng1GoT] -= syaQov4s; } } break;
case qmBDxVZb: if (EyBcRfs6 == bljeLxCR - 5) {
gext_PlaySound((BYTE *)QVlC2aGF, sizeof(QVlC2aGF));
dRTvMgT8[H0Ng1GoT] = M5Dv1WTN; break; } if (ZovNcL71[H0Ng1GoT] >= e6GtTAi2 && ZovNcL71[H0Ng1GoT] <= kke5vOAN
&& RANDOM(50) && EUe8jnIv == 0) { EUe8jnIv = maCfct8E;
dRTvMgT8[H0Ng1GoT] = NQBohLTA; } if (ZovNcL71[H0Ng1GoT] > hqTpGU7O)
ZovNcL71[H0Ng1GoT] -= syaQov4s; else { poPO0nRf--;
dRTvMgT8[H0Ng1GoT] = zdsxsArb; } break; case M5Dv1WTN:
poPO0nRf--; dRTvMgT8[H0Ng1GoT] = zdsxsArb;
break; } } if (ZiIxgvnh == zVi0iEzB) {
if (fn7UjdwX > qMlDGg6i)
fn7UjdwX -= g6gRMlSa; else ZiIxgvnh = XLKqwhTr;
for (H0Ng1GoT = 0; H0Ng1GoT < DNMVYa70; H0Ng1GoT++) {
hRldyQfl = DNMVYa70 - H0Ng1GoT - 1; if (dRTvMgT8[hRldyQfl] == zdsxsArb)
continue; if (HIT_CHECK(ZovNcL71[hRldyQfl], zCgyj8Sh, 9)
&& HIT_CHECK(GForC808[hRldyQfl], fn7UjdwX, 9)) {
gext_PlaySound((BYTE *)QVlC2aGF, sizeof(QVlC2aGF));
ZiIxgvnh = XLKqwhTr; if (dRTvMgT8[hRldyQfl] == NQBohLTA)
EUe8jnIv = 0; dRTvMgT8[hRldyQfl] = M5Dv1WTN;
bV3A9vxX += hRldyQfl + 3; break; } } } if (viUz9Tj5 == zVi0iEzB) {
if (QdcJfG4Q < VCkMUNre)
QdcJfG4Q += fAoBQfIH; else viUz9Tj5 = XLKqwhTr;
for (H0Ng1GoT = 0; H0Ng1GoT < ust9nAqu; H0Ng1GoT++)
if (DTwBI3cA[H0Ng1GoT] == SIeOndNS && HIT_CHECK(e5qYfd72[H0Ng1GoT], QdcJfG4Q, 8)
&& g4PA0eRW >= i1tmoCvP - 30) { gext_PlaySound((BYTE *)MbrDqDAt, sizeof(MbrDqDAt));
viUz9Tj5 = XLKqwhTr; DTwBI3cA[H0Ng1GoT] = yqA9fxzR;
bV3A9vxX += 1; break; } } for (H0Ng1GoT = 0; H0Ng1GoT < DNMVYa70; H0Ng1GoT++) {
check = 0; switch (Q7uvooBr[H0Ng1GoT]) { case u7nUfFvO:
continue; case LQbEUd6B: DDvEbYig[H0Ng1GoT] += ace4BKJi;
break; case wW4vgom1: DDvEbYig[H0Ng1GoT] -= ace4BKJi;
break; } wmKp6QpB[H0Ng1GoT] += DuvYHyxN; if (HIT_CHECK(DDvEbYig[H0Ng1GoT], boDvlNKK, 7)
&& HIT_CHECK(wmKp6QpB[H0Ng1GoT], smPxjsAd, 7)) {
gext_PlaySound((BYTE *)n67EAbnG, sizeof(n67EAbnG));
Q7uvooBr[H0Ng1GoT] = u7nUfFvO; J6vnDQgO = JdwXljeo; 
gext_StartVib(1); qNLHJYTJ = 0; continue;
} if (wmKp6QpB[H0Ng1GoT] < i1tmoCvP - 8) continue;
for (hRldyQfl = 0; hRldyQfl < ust9nAqu; hRldyQfl++) {
if (DTwBI3cA[hRldyQfl] == SIeOndNS && HIT_CHECK(e5qYfd72[hRldyQfl], DDvEbYig[H0Ng1GoT], 6)) {
gext_PlaySound((BYTE *)MbrDqDAt, sizeof(MbrDqDAt));
DTwBI3cA[hRldyQfl] = yqA9fxzR; Q7uvooBr[H0Ng1GoT] = u7nUfFvO;
zZmQ5tSj = 0; check = 1; break; } } if (check || wmKp6QpB[H0Ng1GoT] < i1tmoCvP)
continue; for (hRldyQfl = 0; hRldyQfl < vctgWaNN; hRldyQfl++) {
if (Z0nhq0Kh[hRldyQfl] != pRq0WLKR && HIT_CHECK(rCfdY3Fu[hRldyQfl], DDvEbYig[H0Ng1GoT], 8)) 
{ gext_PlaySound((BYTE *)MbrDqDAt, sizeof(MbrDqDAt));
Z0nhq0Kh[hRldyQfl] = FAKGFvPB; Q7uvooBr[H0Ng1GoT] = u7nUfFvO;
check = 1; break; } } for (hRldyQfl = 0; check == 0 && hRldyQfl < vctgWaNN; hRldyQfl++) {
if (Z0nhq0Kh[hRldyQfl] == pRq0WLKR) { gext_PlaySound((BYTE *)MbrDqDAt, sizeof(MbrDqDAt));
Z0nhq0Kh[hRldyQfl] = FAKGFvPB; rCfdY3Fu[hRldyQfl] = DDvEbYig[H0Ng1GoT];
Q7uvooBr[H0Ng1GoT] = u7nUfFvO; VMOPFjty++;
break; } } } }
static void baMkReZj()
{ BYTE H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < DNMVYa70; H0Ng1GoT++) { 
switch (Q7uvooBr[H0Ng1GoT])  { case LQbEUd6B:
mil_CopyImage((s_ImgBlock*)hMgJWd9P[s_Frame2], DDvEbYig[H0Ng1GoT], wmKp6QpB[H0Ng1GoT]);
break; case wW4vgom1: mil_CopyImage((s_ImgBlock*)Ea1fhBEB[s_Frame2], DDvEbYig[H0Ng1GoT], wmKp6QpB[H0Ng1GoT]);
break; } switch (dRTvMgT8[H0Ng1GoT])  { case oTGWmLqD:
case NQBohLTA: case qmBDxVZb: mil_CopyImage((s_ImgBlock*)nwlhDrUV, ZovNcL71[H0Ng1GoT], GForC808[H0Ng1GoT]);
break; case M5Dv1WTN: mil_CopyImage((s_ImgBlock*)fdH0qGwx[s_Frame2], ZovNcL71[H0Ng1GoT], GForC808[H0Ng1GoT]);
break; } } }
static void PbGwy0Yq()
{ if (ZiIxgvnh == zVi0iEzB) { mil_CopyImage((s_ImgBlock*)dPgiUtLW, zCgyj8Sh, fn7UjdwX);
} if (viUz9Tj5 == zVi0iEzB) { mil_CopyImage((s_ImgBlock*)vcCIx3Gs, QdcJfG4Q, g4PA0eRW);
} } static int MyVHeQhH() { if (boDvlNKK > 20)
{ boDvlNKK--; } else if (boDvlNKK < 20) {
boDvlNKK++; } if (boDvlNKK < 50 && boDvlNKK > 5)
{ if (120 - eYBbS5Lv > 82) { return 1; } eYBbS5Lv -= 2;
} return 0; } static void Xu2ZZsQe() { mil_CopyImage((s_ImgBlock*)KMrcImxP, eYBbS5Lv, i1tmoCvP);
}
int MarsScouter3Main(int nReason, int nData)
{ switch (nReason)  { case GLIB_MENU: 
ZzZgAgiF(0);
break; case GLIB_START:
xL4BtFZx();
gext_PlaySound((BYTE *)Y6fDr1h3, sizeof(Y6fDr1h3));
g_StartTimer(0, gFukqubo, 1);
break;  case GLIB_TIMEOUT: switch (J6vnDQgO) 
{ case ak6MkVXT: yJHeWr3w(); break; case JWyRjw0d:
if (qNLHJYTJ <= 0) { J6vnDQgO = DMqJ0k0h;
} qNLHJYTJ--; mil_Clear(S_WHITE); tVcmJMjw();
uigm_DrawGround(); V9TuFoHp(); DrawReadyMark(E1nAopVG + 1, SWAP_LCD_WIDTH/2);
break; case DMqJ0k0h: mil_Clear(S_WHITE);
BFYhZ39Y(); tVcmJMjw(); BE87rPkM(); uigm_DrawGround();
baMkReZj(); PbGwy0Yq(); V9TuFoHp(); break;
case JdwXljeo: if (qNLHJYTJ < 10) { mil_Clear(S_WHITE);
uigm_DrawGround(); baMkReZj(); PbGwy0Yq();
V9TuFoHp(); qNLHJYTJ++; } else { oMlvQYgP--;
if (oMlvQYgP > 0) { jbceWr6S(); gext_PlaySound((BYTE *)qOj1fhn5, sizeof(qOj1fhn5));
J6vnDQgO = JWyRjw0d; } else { mil_Clear(0x89);
ZzZgAgiF(1);  return g_CheckScore(bV3A9vxX);
} } break; case hcocB2XK: { mil_Clear(S_WHITE);
BFYhZ39Y(); tVcmJMjw(); BE87rPkM(); uigm_DrawGround();
Xu2ZZsQe(); V9TuFoHp(); if (MyVHeQhH()) {
mil_SaveLCD(); E1nAopVG++; if (E1nAopVG >= 10) E1nAopVG = 9;
EyBcRfs6 = 0; gext_PlaySound((BYTE *)OE6kSgKR, sizeof(OE6kSgKR));
J6vnDQgO = FoKeC2BO; qNLHJYTJ = 0; } } break;
case FoKeC2BO: if (qNLHJYTJ < 13) { mil_RestoreLCD();
DrawClearMark(); qNLHJYTJ++; } else { jbceWr6S();
gext_PlaySound((BYTE *)qOj1fhn5, sizeof(qOj1fhn5));
J6vnDQgO = JWyRjw0d; qNLHJYTJ = 13; } break;
} gext_DrawLcd((char*)swLcdBuffer); break;
case GLIB_KEYPRESS: switch (J6vnDQgO) {
case ak6MkVXT: switch (nData) { case GLIB_KEY_DOWN: 
E1nAopVG = (E1nAopVG + 9)%10; gm_PlayStageChange();
break; case GLIB_KEY_UP: 
E1nAopVG = (E1nAopVG + 1)%10; gm_PlayStageChange();
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
case GLIB_KEY_OK: 
mil_Clear(S_WHITE);
gext_PlaySound((BYTE *)qOj1fhn5, sizeof(qOj1fhn5));
J6vnDQgO = JWyRjw0d; jbceWr6S(); qNLHJYTJ = 12;
break; } break; case JWyRjw0d: J6vnDQgO = DMqJ0k0h;
break; case DMqJ0k0h: switch (nData) { case GLIB_KEY_LEFT: 
case GLIB_KEY_4: if (boDvlNKK > 5) boDvlNKK -= 3;
break; case GLIB_KEY_RIGHT: 
case GLIB_KEY_5: if (boDvlNKK < 80) boDvlNKK += 3;
break; case GLIB_KEY_UP: 
case GLIB_KEY_1: if (aIn8WRPy == b960XAqm)
{ gext_PlaySound((BYTE *)pxTpHZSv, sizeof(pxTpHZSv));
aIn8WRPy = cwmeT2iK; qNLHJYTJ = 0; } break;
case GLIB_KEY_OK: case GLIB_KEY_6: if (ZiIxgvnh == XLKqwhTr
|| viUz9Tj5 == XLKqwhTr) { gext_PlaySound((BYTE *)XLkoda1i, sizeof(XLkoda1i));
} if (ZiIxgvnh == XLKqwhTr) { ZiIxgvnh = zVi0iEzB;
zCgyj8Sh = boDvlNKK - 5; fn7UjdwX = smPxjsAd - 40;
} if (viUz9Tj5 == XLKqwhTr) { viUz9Tj5 = zVi0iEzB;
QdcJfG4Q = boDvlNKK + 5; g4PA0eRW = smPxjsAd - 26;
} break; }
case etGPD48C:
break; } break; case GLIB_PAUSE:
gm_DrawPauseMark();
gext_DrawLcd((char*)swLcdBuffer); return GRET_PAUSE;
case GLIB_CONTINUE: if(J6vnDQgO==ak6MkVXT) gext_PlaySound((BYTE *)Y6fDr1h3, sizeof(Y6fDr1h3));
g_StartTimer(0, gFukqubo, 1); break; case GLIB_END:
mil_SaveLCD(); return GRET_PAUSE; case GLIB_MENU_SCORE:
case GLIB_SCORE: mil_Clear(0x89); ZzZgAgiF(1); 
break; case GLIB_HELP: case GLIB_HELP_TIMEOUT:
mil_CopyImage((s_ImgBlock*)tDwPyRCh, SWAP_LCD_WIDTH/2, SWAP_LCD_HEIGHT/2);
break; } return GRET_NORMAL; } 
#endif 
