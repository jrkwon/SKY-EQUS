//==============================================================================
//
//	Gray LCD library
//
//==============================================================================

#ifdef SWAP_LCD_GRAY
static const BYTE OjejimrN[] = {
S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY0
}; static const BYTE xWYbAHVO[] = { S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY1
}; static const BYTE Ge9ypBEE[] = { S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY2
}; static const BYTE GsgfrHnu[] = { S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY2, S_GRAY3, S_GRAY0, S_GRAY1, S_GRAY3 
}; static const BYTE ZYj9qvyb[] = { S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY2
}; static const BYTE B0oOslTf[] = { S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY3, S_TRANSPARENT,
S_GRAY1, S_GRAY2, S_GRAY3, S_GRAY0, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY1, S_GRAY2, S_GRAY1 
};
static const BYTE* SPvM1Gb8[6] = { OjejimrN, xWYbAHVO, Ge9ypBEE, GsgfrHnu, ZYj9qvyb, B0oOslTf };
static BYTE* apHGYOU7; 
static BYTE jYPKuxnw; 
static BYTE kkNN5Ev6; 

#if (SWAP_LCD_TYPE != SWAP_LCD_GRAY_MP_HORIZ_MF && SWAP_LCD_TYPE != SWAP_LCD_GRAY_MP_VERT_MF && SWAP_LCD_TYPE != SWAP_LCD_GRAY_MP_VERT_LF)
static const BYTE RwVPjzxH[] = { 0x3F, 0xCF, 0xF3, 0xFC };
static const BYTE wXY67Rf3[] = { 0xC0, 0x30, 0x0C, 0x03 };
static const BYTE s7C8SZZs[] = { 0x80, 0x20, 0x08, 0x02 };
static const BYTE k0MQayom[] = { 0x40, 0x10, 0x04, 0x01 };
static const BYTE HRCKFYTf[] = { 0x00, 0x00, 0x00, 0x00 };
static const BYTE* JZLJSz2M[] = { wXY67Rf3, wXY67Rf3, wXY67Rf3, wXY67Rf3 };
static const BYTE* EOI4B9jZ[] = { s7C8SZZs, wXY67Rf3, wXY67Rf3, wXY67Rf3 };
static const BYTE* oEnAig1J[] = { k0MQayom, s7C8SZZs, wXY67Rf3, wXY67Rf3 };
static const BYTE* TmtaAsPv[] = { HRCKFYTf, k0MQayom, s7C8SZZs, wXY67Rf3 };
static const BYTE* Cw9bZk4k[] = { HRCKFYTf, HRCKFYTf, k0MQayom, s7C8SZZs };
static const BYTE* FJweE8VK[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, k0MQayom };
static const BYTE* GBecIdEn[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, HRCKFYTf };
static const BYTE** dFi5bO99[] = { GBecIdEn, FJweE8VK, Cw9bZk4k, TmtaAsPv, oEnAig1J, EOI4B9jZ, JZLJSz2M };
static const BYTE* boNcuwqS[4];

#else 
static const BYTE RwVPjzxH[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
static const BYTE k0MQayom[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static const BYTE HRCKFYTf[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const BYTE* O15IeEKn[] = { k0MQayom, k0MQayom, k0MQayom, k0MQayom };
static const BYTE* fthEy4WO[] = { k0MQayom, k0MQayom, k0MQayom, k0MQayom };
static const BYTE* XPxhpuKc[] = { HRCKFYTf, k0MQayom, k0MQayom, k0MQayom };
static const BYTE* eTx6s9W0[] = { HRCKFYTf, HRCKFYTf, k0MQayom, k0MQayom };
static const BYTE* CpPc5HRj[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, k0MQayom };
static const BYTE* EccKMVci[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, HRCKFYTf };
static const BYTE* W1lcDjm6[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, HRCKFYTf };
static const BYTE** FJweE8VK[] = { W1lcDjm6, EccKMVci, CpPc5HRj, eTx6s9W0, XPxhpuKc, fthEy4WO, O15IeEKn };
static const BYTE* mhLcYuKT[] = { k0MQayom, k0MQayom, k0MQayom, k0MQayom };
static const BYTE* Fb7QjkfV[] = { HRCKFYTf, k0MQayom, k0MQayom, k0MQayom };
static const BYTE* dSet7rec[] = { k0MQayom, HRCKFYTf, k0MQayom, k0MQayom };
static const BYTE* YN5px9wH[] = { HRCKFYTf, k0MQayom, HRCKFYTf, k0MQayom };
static const BYTE* HzrKAV9w[] = { HRCKFYTf, HRCKFYTf, k0MQayom, HRCKFYTf };
static const BYTE* weUuaSVW[] = { HRCKFYTf, HRCKFYTf, k0MQayom, k0MQayom };
static const BYTE* UVOMl6I0[] = { HRCKFYTf, HRCKFYTf, HRCKFYTf, HRCKFYTf };
static const BYTE** Cw9bZk4k[] = { UVOMl6I0, weUuaSVW, HzrKAV9w, YN5px9wH, dSet7rec, Fb7QjkfV, mhLcYuKT };
static const BYTE* JhgEoEOk[4]; static const BYTE* JxdvQVIg[4];
static BYTE *iiaR1Qyh; static BYTE *H5h5hXcT;
static int lhNFI5Ex; 
#endif 
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void (*SZysP4qw[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&iW0pdjQP, &pohX7Er3, &jGtO1FB9 }; static void (*inTEQRbH[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&IOd5DYzC, &VJ9k9RQN, &M1P1NHy9 };
void mil_ClearWhite()
{ CKOk9Z4Q(JZagjmLi, 0x00, AvwqZ1W4); } void mil_ClearBlack()
{ CKOk9Z4Q(JZagjmLi, 0xFF, AvwqZ1W4); }

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VERT_MF)
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = lcdWidth;
AvwqZ1W4 = lcdWidth*(lcdHeight+7)/8*2;
apHGYOU7 = (BYTE*)SPvM1Gb8[0]; mil_SetGamma(3);
mil_SetColor(S_BLACK); mil_ResetClip(); }
void mil_Clear(int D397hZog)
{ BYTE LG0dStpd; BYTE m3ESBpLZ; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; LG0dStpd = boNcuwqS[apHGYOU7[D397hZog]][3];
m3ESBpLZ = (LG0dStpd << 6) | (LG0dStpd << 4) | (LG0dStpd << 2) | (LG0dStpd);
CKOk9Z4Q(JZagjmLi, m3ESBpLZ, AvwqZ1W4); }
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE m3ESBpLZ; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog == S_TRANSPARENT) return;
m3ESBpLZ = boNcuwqS[apHGYOU7[D397hZog]][3];
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu*2 + (LbPt5vw8 << 1) + ((y >> 2) & 0x01)];
gIVFkk7i = 6 - ((y & 0x03) << 1); G3Ob0FpR = 0x03 << gIVFkk7i;
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (m3ESBpLZ << gIVFkk7i);
}
static void kSeyq5l8(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (jYPKuxnw != S_TRANSPARENT) { kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu*2 + (LbPt5vw8 << 1) + ((y >> 2) & 0x01)];
gIVFkk7i = 6 - ((y & 0x03) << 1); G3Ob0FpR = 0x03 << gIVFkk7i;
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (jYPKuxnw << gIVFkk7i);
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
cAQOtAsc = kSeyq5l8; } }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; if (kkNN5Ev6 == S_TRANSPARENT)
jYPKuxnw = S_TRANSPARENT; else jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(boNcuwqS, dFi5bO99[Gamma], sizeof(boNcuwqS));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK) {
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu*2 + (LbPt5vw8 << 1) + ((y >> 2) & 0x01)];
gIVFkk7i = 6 - ((y & 0x03) << 1); G3Ob0FpR = 0x03 << gIVFkk7i;
return (*kor8Q3C3 & G3Ob0FpR) >> gIVFkk7i;
} else { return -1; } }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int mSdyqwa7; BYTE G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
G3Ob0FpR = 0xFF >> ((qb0L1TPI & 0x03) << 1);
for (N3jRR8ZR = qb0L1TPI - (qb0L1TPI % 4); (N3jRR8ZR + 3) <= TYFI6PC8; N3jRR8ZR += 4) {
kor8Q3C3 = &JZagjmLi[((N3jRR8ZR >> 3)*NP3y8tCu*2) + (CYVLXJkW*2)
+ ((N3jRR8ZR >> 2) & 0x01)]; for (OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += 2;
} G3Ob0FpR = 0xFF; } G3Ob0FpR = 0xFF << (6 - ((TYFI6PC8 & 3) << 1));
kor8Q3C3 = &JZagjmLi[((N3jRR8ZR >> 3)*NP3y8tCu*2) + (CYVLXJkW*2)
+ ((TYFI6PC8 >> 2) & 0x01)]; for (OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += 2;
} }
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdy; BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE*2];
BYTE gIVFkk7i; if((wrap & (~0x01)) || (lcd & (~0x01)))
return; if (lcd == 0) { tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if (dx > h8iL9Zij || dx < -h8iL9Zij) {
if (wrap == 1) { dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < ((UwJNuSkK + 7)/8); N3jRR8ZR++) {
if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[((N3jRR8ZR + 1)*NP3y8tCu - dx)*2];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx*2);
} kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu*2];
GICEruWy(kor8Q3C3 + (2*dx), kor8Q3C3, (h8iL9Zij - dx)*2);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx*2);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx*2);
} } else { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu*2];
if (wrap == 1) { 
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -(dx*2));
} GICEruWy(kor8Q3C3, kor8Q3C3 - (2*dx), (h8iL9Zij + dx)*2);
kor8Q3C3 = &tempLcd[((N3jRR8ZR + 1)*h8iL9Zij + dx)*2];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -(dx*2));
} else { CKOk9Z4Q(kor8Q3C3, 0, -(dx*2));
} } } } if (dy > UwJNuSkK || dy < -UwJNuSkK) {
if (wrap == 1) { dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } } tdy = dy; if (dy != 0) {
for (OOzmyCcI = 0; OOzmyCcI < h8iL9Zij; OOzmyCcI++) {
dy = tdy; if (dy % 8) { 
kor8Q3C3 = &tempLcd[OOzmyCcI*2]; s_ScrollBuffer[0] = kor8Q3C3[((UwJNuSkK - 1)/8)*NP3y8tCu*2];
s_ScrollBuffer[1] = kor8Q3C3[((UwJNuSkK - 1)/8)*NP3y8tCu*2 + 1];
gIVFkk7i = (((dy % 8) + 8) % 8)*2; 
int mSdyqwa7, temp2; for (N3jRR8ZR = ((UwJNuSkK - 1)/8); N3jRR8ZR > 0; N3jRR8ZR --) {
mSdyqwa7 = kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] << 8 
| kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1]; temp2 = kor8Q3C3[(N3jRR8ZR - 1)*NP3y8tCu*2] << 8 
| kor8Q3C3[(N3jRR8ZR - 1)*NP3y8tCu*2 + 1];
mSdyqwa7 = mSdyqwa7 >> gIVFkk7i | temp2 << (16 - gIVFkk7i);
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = mSdyqwa7 >> 8;
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = mSdyqwa7 & 0xFF;
} temp2 = s_ScrollBuffer[0] << 8 | s_ScrollBuffer[1]; 
mSdyqwa7 = kor8Q3C3[0] << 8 | kor8Q3C3[1];
if (wrap == 1) { mSdyqwa7 = mSdyqwa7 >> gIVFkk7i | temp2 << (16 - gIVFkk7i);
} else { if (dy > 0) { 
mSdyqwa7 = mSdyqwa7 >> gIVFkk7i; } else { 
mSdyqwa7 = temp2 << (16 - gIVFkk7i); } } kor8Q3C3[0] = mSdyqwa7 >> 8;
kor8Q3C3[1] = mSdyqwa7 & 0xFF; dy -= gIVFkk7i/2;
} dy /= 8; if (dy > 0) { 
kor8Q3C3 = &tempLcd[((((UwJNuSkK + 7)/8) - dy)*NP3y8tCu*2) + (OOzmyCcI*2)];
if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < dy*2; N3jRR8ZR += 2, kor8Q3C3 += NP3y8tCu*2) {
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; s_ScrollBuffer[N3jRR8ZR + 1] = *(kor8Q3C3 + 1);
} } kor8Q3C3 = &tempLcd[OOzmyCcI*2]; for (N3jRR8ZR = (((UwJNuSkK + 7)/8) - dy - 1); N3jRR8ZR >= 0; N3jRR8ZR--) {
kor8Q3C3[(N3jRR8ZR + dy)*NP3y8tCu*2] = kor8Q3C3[N3jRR8ZR*NP3y8tCu*2];
kor8Q3C3[(N3jRR8ZR + dy)*NP3y8tCu*2 + 1] = kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1];
} if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = s_ScrollBuffer[N3jRR8ZR*2];
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = s_ScrollBuffer[N3jRR8ZR*2 + 1];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = 0;
} } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI*2]; if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < (-dy*2); N3jRR8ZR += 2) {
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[N3jRR8ZR + 1] = kor8Q3C3[N3jRR8ZR*NP3y8tCu + 1];
} } for (N3jRR8ZR = 0; N3jRR8ZR < (((UwJNuSkK + 7)/8) + dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu*2];
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu*2 + 1];
} kor8Q3C3 = &tempLcd[(((((UwJNuSkK + 7)/8) + dy)*NP3y8tCu) + OOzmyCcI)*2];
if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < -dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = s_ScrollBuffer[N3jRR8ZR*2];
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = s_ScrollBuffer[N3jRR8ZR*2 + 1];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy*2); N3jRR8ZR += 2) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu*2] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu*2 + 1] = 0;
} } } } } }
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + (cx << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];
} FcyUMPlI += 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + (cx << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} FcyUMPlI += 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + (cx << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 2) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[apHGYOU7[KrspwWdU]][v32pnQd7];
} FcyUMPlI += 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + ((cx + qm7J64q3 - 1) << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} FcyUMPlI -= 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + ((cx + qm7J64q3 - 1) << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} FcyUMPlI -= 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu*2 + ((cx + qm7J64q3 - 1) << 1) + ((cy >> 2) & 0x01);
kT6WFmL3 = cy & 0x03;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[apHGYOU7[KrspwWdU]][v32pnQd7];
} FcyUMPlI -= 2;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } if (kT6WFmL3 != 3)
{ kT6WFmL3++; } else { if (((cy + y) & 0x04))
{ XBx7hZIP += NP3y8tCu*2 - 1; } else { XBx7hZIP++;
} kT6WFmL3 = 0; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } } 
#endif 

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
NP3y8tCu = (lcdWidth + 3)/4; AvwqZ1W4 = (lcdWidth+3)/4*lcdHeight;

#else
NP3y8tCu = (lcdWidth + 7)/8*2; AvwqZ1W4 = (lcdWidth+7)/8*2*lcdHeight;

#endif
apHGYOU7 = (BYTE*)SPvM1Gb8[0];
mil_SetGamma(3); mil_SetColor(S_BLACK); mil_ResetClip();
}
void mil_Clear(int D397hZog)
{ BYTE LG0dStpd; BYTE m3ESBpLZ; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; LG0dStpd = boNcuwqS[apHGYOU7[D397hZog]][3];
m3ESBpLZ = (LG0dStpd << 6) | (LG0dStpd << 4) | (LG0dStpd << 2) | (LG0dStpd);
CKOk9Z4Q(JZagjmLi, m3ESBpLZ, AvwqZ1W4); }
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE m3ESBpLZ; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog == S_TRANSPARENT) return;
m3ESBpLZ = boNcuwqS[apHGYOU7[D397hZog]][3];

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 2)];

#else
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (((LbPt5vw8 >> 3) << 1) | (((LbPt5vw8+4) >> 2) & 1))];

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#else 
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (m3ESBpLZ << gIVFkk7i);
}
static void kSeyq5l8(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (jYPKuxnw != S_TRANSPARENT) { 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 2)];

#else
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (((LbPt5vw8 >> 3) << 1) | (((LbPt5vw8+4) >> 2) & 1))];

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#else 
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (jYPKuxnw << gIVFkk7i);
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
cAQOtAsc = kSeyq5l8; } }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; if (kkNN5Ev6 == S_TRANSPARENT)
jYPKuxnw = S_TRANSPARENT; else jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(boNcuwqS, dFi5bO99[Gamma], sizeof(boNcuwqS));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK) {

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 2)];

#else
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (((LbPt5vw8 >> 3) << 1) | (((LbPt5vw8+4) >> 2) & 1))];

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#else 
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; return (*kor8Q3C3 & G3Ob0FpR) >> gIVFkk7i;
} else { return -1; } }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
WORD *kor8Q3C3; int mSdyqwa7; WORD G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
G3Ob0FpR = 0xFFFF << ((CYVLXJkW % 8)*2); G3Ob0FpR = ((G3Ob0FpR & 0xFF00) >> 8) | ((G3Ob0FpR & 0x00FF) << 8);
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
kor8Q3C3 = (WORD *)&JZagjmLi[N3jRR8ZR*NP3y8tCu + ((CYVLXJkW >> 3) << 1)];
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3++;
for (OOzmyCcI = CYVLXJkW - (CYVLXJkW % 8) + 8; (OOzmyCcI + 7) <= LINj8eGO; OOzmyCcI += 8) {
*kor8Q3C3 = ~(*kor8Q3C3); kor8Q3C3++; } }
G3Ob0FpR = 0xFFFF >> ((7 - (LINj8eGO % 8))*2);
G3Ob0FpR = ((G3Ob0FpR & 0xFF00) >> 8) | ((G3Ob0FpR & 0x00FF) << 8);
kor8Q3C3 = (WORD *)&JZagjmLi[qb0L1TPI*NP3y8tCu + ((OOzmyCcI >> 3) << 1)];
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += NP3y8tCu >> 1;
} 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
BYTE *kor8Q3C3; int mSdyqwa7; BYTE G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
G3Ob0FpR = 0xFF; for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
G3Ob0FpR = G3Ob0FpR >> ((CYVLXJkW % 4)*2);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
G3Ob0FpR = G3Ob0FpR << ((CYVLXJkW % 4)*2);

#endif
kor8Q3C3 = &JZagjmLi[N3jRR8ZR*NP3y8tCu + (CYVLXJkW >> 2)];
for (OOzmyCcI = CYVLXJkW - (CYVLXJkW % 4); (OOzmyCcI + 3) <= LINj8eGO; OOzmyCcI += 4) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3++;
G3Ob0FpR = 0xFF; }
}

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
G3Ob0FpR = G3Ob0FpR << ((LINj8eGO % 4)*2);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
G3Ob0FpR = G3Ob0FpR >> ((LINj8eGO % 4)*2);

#endif
kor8Q3C3 = &JZagjmLi[qb0L1TPI*NP3y8tCu + (OOzmyCcI/4)];
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += NP3y8tCu;
} 
#endif 
}
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdx; BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE];
BYTE gIVFkk7i; if((wrap & (~0x01)) || (lcd & (~0x01)))
return; if (lcd == 0) { tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if (dx > h8iL9Zij || dx < -h8iL9Zij) {
if (wrap == 1) { dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
tdx = dx;
if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < UwJNuSkK; N3jRR8ZR++) {
dx = tdx; if (dx % 8) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; s_ScrollBuffer[0] = kor8Q3C3[NP3y8tCu-2];
s_ScrollBuffer[1] = kor8Q3C3[NP3y8tCu-1];
gIVFkk7i = (((dx % 8) + 8) % 8)*2; if (gIVFkk7i >= 8) {
gIVFkk7i -= 8; for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 2; OOzmyCcI -= 2) {
kor8Q3C3[OOzmyCcI - 1] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) 
| (kor8Q3C3[OOzmyCcI-3] >> (8 - gIVFkk7i));
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI-3] << gIVFkk7i) | (kor8Q3C3[OOzmyCcI-2] >> (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[1] << gIVFkk7i)
| (s_ScrollBuffer[0] >> (8 - gIVFkk7i)); kor8Q3C3[1] = (s_ScrollBuffer[0] << gIVFkk7i) 
| (s_ScrollBuffer[1] >> (8 - gIVFkk7i)); } else {
if (dx > 0) { kor8Q3C3[0] = kor8Q3C3[1] << gIVFkk7i;
kor8Q3C3[1] = 0; } else { 
kor8Q3C3[0] = (s_ScrollBuffer[0] >> (8 - gIVFkk7i));
kor8Q3C3[1] = (s_ScrollBuffer[0] << gIVFkk7i) 
| (s_ScrollBuffer[1] >> (8 - gIVFkk7i)); }
} gIVFkk7i += 8; } else { for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 2; OOzmyCcI -= 2) {
kor8Q3C3[OOzmyCcI - 1] = (kor8Q3C3[OOzmyCcI-1] << gIVFkk7i) | (kor8Q3C3[OOzmyCcI] >> (8 - gIVFkk7i));
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) | (kor8Q3C3[OOzmyCcI-3] >> (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i) | (kor8Q3C3[1] >> (8 - gIVFkk7i));
kor8Q3C3[1] = (kor8Q3C3[1] << gIVFkk7i)  | (s_ScrollBuffer[0] >> (8 - gIVFkk7i));
} else { if (dx > 0) { kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i) 
| (kor8Q3C3[1] >> (8 - gIVFkk7i)); kor8Q3C3[1] = (kor8Q3C3[1] << gIVFkk7i);
} else { 
kor8Q3C3[0] = kor8Q3C3[1] >> (8 - gIVFkk7i);
kor8Q3C3[1] = (kor8Q3C3[1] << gIVFkk7i)  | (s_ScrollBuffer[0] >> (8 - gIVFkk7i));
} } } dx -= gIVFkk7i/2; } dx /= 4; if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[(N3jRR8ZR+1)*NP3y8tCu - dx];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx); }
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; GICEruWy(kor8Q3C3 + dx, kor8Q3C3, NP3y8tCu - dx);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx); } } else if (dx < 0) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; if (wrap == 1) {
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dx); }
GICEruWy(kor8Q3C3, kor8Q3C3 - dx, NP3y8tCu + dx);
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu + dx];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dx); } } }
} 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
tdx = dx;
if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < UwJNuSkK; N3jRR8ZR++) {
dx = tdx; if (dx % 4) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; s_ScrollBuffer[0] = kor8Q3C3[NP3y8tCu-1];
gIVFkk7i = (((dx % 4) + 4) % 4)*2; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 0; OOzmyCcI--) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI-1] << (8 - gIVFkk7i)) | (kor8Q3C3[OOzmyCcI] >> gIVFkk7i);
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] >> gIVFkk7i) 
| (s_ScrollBuffer[0] << (8 - gIVFkk7i)); } else {
if (dx > 0) { kor8Q3C3[0] = kor8Q3C3[0] >> gIVFkk7i;
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] << (8 - gIVFkk7i);
} } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 0; OOzmyCcI--) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI-1] >> (8 - gIVFkk7i)) | (kor8Q3C3[OOzmyCcI] << gIVFkk7i);
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i) 
| (s_ScrollBuffer[0] >> (8 - gIVFkk7i)); } else {
if (dx > 0) { kor8Q3C3[0] = kor8Q3C3[0] << gIVFkk7i;
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] >> (8 - gIVFkk7i);
} } 
#endif
dx -= gIVFkk7i/2; } dx /= 4; if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[(N3jRR8ZR+1)*NP3y8tCu - dx];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx); }
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; GICEruWy(kor8Q3C3 + dx, kor8Q3C3, NP3y8tCu - dx);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx); } } else if (dx < 0) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; if (wrap == 1) {
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dx); }
GICEruWy(kor8Q3C3, kor8Q3C3 - dx, NP3y8tCu + dx);
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu + dx];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dx); } } }
} 
#endif 
if (dy > UwJNuSkK || dy < -UwJNuSkK) { if (wrap == 1) {
dy %= UwJNuSkK; } else { dy = UwJNuSkK; }
}
if (dy != 0) {
for (OOzmyCcI = 0; OOzmyCcI < NP3y8tCu; OOzmyCcI++) {
if (dy > 0) { 
kor8Q3C3 = &tempLcd[((UwJNuSkK - dy)*NP3y8tCu) + OOzmyCcI];
if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu) {
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; } }
kor8Q3C3 = &tempLcd[OOzmyCcI]; for (N3jRR8ZR = (UwJNuSkK - dy - 1); N3jRR8ZR >= 0; N3jRR8ZR--) {
kor8Q3C3[(N3jRR8ZR + dy)*NP3y8tCu] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
} if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; } } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if (wrap == 1) {
for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
} } for (N3jRR8ZR = 0; N3jRR8ZR < (UwJNuSkK + dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu];
} kor8Q3C3 = &tempLcd[(UwJNuSkK + dy)*NP3y8tCu + OOzmyCcI];
if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; } } } } }
}
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx >> 3) << 1);
ByteShift = ((cx+4) >> 2) & 1; StartByteShift = ByteShift;

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#else 
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI++;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI++;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { if (ByteShift == 0) {
FcyUMPlI += 2; ByteShift = 1; } else ByteShift = 0;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx >> 3) << 1);
ByteShift = ((cx+4) >> 2) & 1; StartByteShift = ByteShift;

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#else 
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI++;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI++;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { if (ByteShift == 0) {
FcyUMPlI += 2; ByteShift = 1; } else ByteShift = 0;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx >> 3) << 1);
ByteShift = ((cx+4) >> 2) & 1; StartByteShift = ByteShift;

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#else 
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 2) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[apHGYOU7[KrspwWdU]])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[apHGYOU7[KrspwWdU]])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI++;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI++;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { if (ByteShift == 0) {
FcyUMPlI += 2; ByteShift = 1; } else ByteShift = 0;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (((cx + qm7J64q3 - 1) >> 3) << 1);
ByteShift = (((cx+qm7J64q3-1)+4) >> 2) & 1;
StartByteShift = ByteShift; 
#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#else 
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI--;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI--;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { if (ByteShift == 1) {
FcyUMPlI -= 2; ByteShift = 0; } else ByteShift = 1;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (((cx + qm7J64q3 - 1) >> 3) << 1);
ByteShift = (((cx+qm7J64q3-1)+4) >> 2) & 1;
StartByteShift = ByteShift; 
#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#else 
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI--;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI--;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { if (ByteShift == 1) {
FcyUMPlI -= 2; ByteShift = 0; } else ByteShift = 1;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
int ByteShift, StartByteShift; 
#endif
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;

#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 2);

#else
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (((cx + qm7J64q3 - 1) >> 3) << 1);
ByteShift = (((cx+qm7J64q3-1)+4) >> 2) & 1;
StartByteShift = ByteShift; 
#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#else 
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
*FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[apHGYOU7[KrspwWdU]])[v32pnQd7];

#else
*(FcyUMPlI+ByteShift) = (*(FcyUMPlI+ByteShift) & RwVPjzxH[v32pnQd7]) | (boNcuwqS[apHGYOU7[KrspwWdU]])[v32pnQd7];

#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI--;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI--;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { if (ByteShift == 1) {
FcyUMPlI -= 2; ByteShift = 0; } else ByteShift = 1;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if !(SWAP_LCD_TYPE == SWAP_LCD_GRAY_HW_HORIZ_LF)
XBx7hZIP += NP3y8tCu; 
#else
XBx7hZIP += NP3y8tCu; ByteShift = StartByteShift;

#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } } 
#endif 

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_HORIZ_MF)
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = (lcdWidth + 7)/8;
AvwqZ1W4 = (lcdWidth+7)/8*2*lcdHeight;
lhNFI5Ex = NP3y8tCu*UwJNuSkK; iiaR1Qyh = JZagjmLi;
H5h5hXcT = JZagjmLi + lhNFI5Ex; apHGYOU7 = (BYTE*)SPvM1Gb8[0];
mil_SetGamma(3); mil_SetColor(S_BLACK); mil_ResetClip();
}
void mil_Clear(int D397hZog)
{ BYTE color1, color2; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; color1 = JhgEoEOk[apHGYOU7[D397hZog]][0];
color2 = JxdvQVIg[apHGYOU7[D397hZog]][0];
if (color1) CKOk9Z4Q(iiaR1Qyh, 0xFF, lhNFI5Ex);
else CKOk9Z4Q(iiaR1Qyh, 0x00, lhNFI5Ex); if (color2)
CKOk9Z4Q(H5h5hXcT, 0xFF, lhNFI5Ex); else CKOk9Z4Q(H5h5hXcT, 0x00, lhNFI5Ex);
}
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE bDNXfqbG, qMrXEwsi; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog != S_TRANSPARENT) { bDNXfqbG = JhgEoEOk[apHGYOU7[D397hZog]][0];
qMrXEwsi = JxdvQVIg[apHGYOU7[D397hZog]][0];
gIVFkk7i = LbPt5vw8 & 0x07; G3Ob0FpR = 0x80 >> gIVFkk7i;
kor8Q3C3 = &iiaR1Qyh[y*NP3y8tCu + (LbPt5vw8 >> 3)];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG >> gIVFkk7i);
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi >> gIVFkk7i);
} }
static void kSeyq5l8(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE bDNXfqbG, qMrXEwsi; if (jYPKuxnw != S_TRANSPARENT) {
bDNXfqbG = (jYPKuxnw & 0x02) >> 1; qMrXEwsi = (jYPKuxnw & 0x01);
gIVFkk7i = LbPt5vw8 & 0x07; G3Ob0FpR = 0x80 >> gIVFkk7i;
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 3)];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG << (7 - gIVFkk7i));
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi << (7 - gIVFkk7i));
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = (JhgEoEOk[apHGYOU7[kkNN5Ev6]][7] << 1) |
JxdvQVIg[apHGYOU7[kkNN5Ev6]][7]; cAQOtAsc = kSeyq5l8;
} }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; if (kkNN5Ev6 == S_TRANSPARENT)
jYPKuxnw = S_TRANSPARENT; else jYPKuxnw = (JhgEoEOk[apHGYOU7[kkNN5Ev6]][7] << 1) | JxdvQVIg[apHGYOU7[kkNN5Ev6]][7];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(JhgEoEOk, FJweE8VK[Gamma], sizeof(JhgEoEOk));
sW4PB08i(JxdvQVIg, Cw9bZk4k[Gamma], sizeof(JxdvQVIg));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
int m3ESBpLZ; if (LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK) {
gIVFkk7i = LbPt5vw8 & 0x07; G3Ob0FpR = 0x80 >> gIVFkk7i;
kor8Q3C3 = &JZagjmLi[y*NP3y8tCu + (LbPt5vw8 >> 3)];
m3ESBpLZ = (*kor8Q3C3 & G3Ob0FpR) ? 0x02 : 0x00;
kor8Q3C3 += lhNFI5Ex; m3ESBpLZ += (*kor8Q3C3 & G3Ob0FpR) ? 0x01 : 0x00;
return m3ESBpLZ; } else { return -1; } }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int mSdyqwa7; BYTE G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
G3Ob0FpR = 0xFF >> (CYVLXJkW % 8); kor8Q3C3 = &JZagjmLi[N3jRR8ZR*NP3y8tCu + (CYVLXJkW/8)];
for (OOzmyCcI = CYVLXJkW - (CYVLXJkW % 8); (OOzmyCcI + 7) <= LINj8eGO; OOzmyCcI += 8) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; *(kor8Q3C3 + lhNFI5Ex) = (*(kor8Q3C3 + lhNFI5Ex)) ^ G3Ob0FpR;
kor8Q3C3++; G3Ob0FpR = 0xFF; } } G3Ob0FpR = G3Ob0FpR << (7 - (LINj8eGO % 8));
kor8Q3C3 = &JZagjmLi[qb0L1TPI*NP3y8tCu + (OOzmyCcI/8)];
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; *(kor8Q3C3 + lhNFI5Ex) = (*(kor8Q3C3 + lhNFI5Ex)) ^ G3Ob0FpR;
kor8Q3C3 += NP3y8tCu; } }
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdx;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE*2];
BYTE gIVFkk7i; if((wrap & (~0x01)) || (lcd & (~0x01)))
return; if (lcd == 0) { tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if (dx > h8iL9Zij || dx < -h8iL9Zij) {
if (wrap == 1) { dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } tdx = dx;
if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < UwJNuSkK; N3jRR8ZR++) {
dx = tdx; if (dx % 8) { 
if (dx > 0) { kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[0] = kor8Q3C3[NP3y8tCu-1];
s_ScrollBuffer[1] = kor8Q3C3[NP3y8tCu-1 + lhNFI5Ex];
gIVFkk7i = dx % 8; for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 0; OOzmyCcI--) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i) | (kor8Q3C3[OOzmyCcI-1] << (8 - gIVFkk7i));
kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] >> gIVFkk7i) 
| (kor8Q3C3[OOzmyCcI-1 + lhNFI5Ex] << (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i) 
| s_ScrollBuffer[0] << (8 - gIVFkk7i); kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] >> gIVFkk7i) 
| s_ScrollBuffer[1] << (8 - gIVFkk7i); } else {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i);
kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] >> gIVFkk7i);
} dx -= gIVFkk7i; } else { kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[0] = kor8Q3C3[0]; s_ScrollBuffer[1] = kor8Q3C3[0 + lhNFI5Ex];
gIVFkk7i = -(dx % 8); for (OOzmyCcI = 0; OOzmyCcI < NP3y8tCu - 1; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) | (kor8Q3C3[OOzmyCcI+1] >> (8 - gIVFkk7i));
kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] << gIVFkk7i) 
| (kor8Q3C3[OOzmyCcI+1 + lhNFI5Ex] >> (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) 
| s_ScrollBuffer[0] >> (8 - gIVFkk7i); kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] << gIVFkk7i) 
| s_ScrollBuffer[1] >> (8 - gIVFkk7i); } else {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i); 
kor8Q3C3[OOzmyCcI + lhNFI5Ex] = (kor8Q3C3[OOzmyCcI + lhNFI5Ex] << gIVFkk7i);
} dx += gIVFkk7i; } } dx /= 8; if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu - dx];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx); sW4PB08i(s_ScrollBuffer + dx, kor8Q3C3 + lhNFI5Ex, dx);
} kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu];
GICEruWy(kor8Q3C3 + dx, kor8Q3C3, NP3y8tCu - dx);
GICEruWy(kor8Q3C3 + dx + lhNFI5Ex, kor8Q3C3 + lhNFI5Ex, NP3y8tCu - dx);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx);
sW4PB08i(kor8Q3C3 + lhNFI5Ex, s_ScrollBuffer + dx, dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx);
CKOk9Z4Q(kor8Q3C3 + lhNFI5Ex, 0, dx); } } else if (dx < 0) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; if (wrap == 1) { 
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dx); sW4PB08i(s_ScrollBuffer - dx, kor8Q3C3 + lhNFI5Ex, -dx);
} GICEruWy(kor8Q3C3, kor8Q3C3 - dx, NP3y8tCu + dx);
GICEruWy(kor8Q3C3 + lhNFI5Ex, kor8Q3C3 - dx + lhNFI5Ex, NP3y8tCu + dx);
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu + dx];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dx);
sW4PB08i(kor8Q3C3 + lhNFI5Ex, s_ScrollBuffer - dx, -dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dx);
CKOk9Z4Q(kor8Q3C3 + lhNFI5Ex, 0, -dx); } }
} } if (dy > UwJNuSkK || dy < -UwJNuSkK) {
if (wrap == 1) { dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } } if (dy != 0) {
for (OOzmyCcI = 0; OOzmyCcI < ((h8iL9Zij + 7)/8); OOzmyCcI++) {
if (dy > 0) { 
kor8Q3C3 = &tempLcd[((UwJNuSkK - dy)*NP3y8tCu) + OOzmyCcI];
if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu) {
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; s_ScrollBuffer[N3jRR8ZR+dy] = *(kor8Q3C3 + lhNFI5Ex);
} } kor8Q3C3 = &tempLcd[OOzmyCcI]; for (N3jRR8ZR = UwJNuSkK - 1; N3jRR8ZR >= dy; N3jRR8ZR--) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex]  = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu + lhNFI5Ex];
} if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = s_ScrollBuffer[N3jRR8ZR + dy];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = 0;
} } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[N3jRR8ZR - dy] = kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex];
} } for (N3jRR8ZR = 0; N3jRR8ZR < (UwJNuSkK + dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex]  = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu + lhNFI5Ex];
} kor8Q3C3 = &tempLcd[(UwJNuSkK + dy)*NP3y8tCu + OOzmyCcI];
if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = s_ScrollBuffer[N3jRR8ZR - dy];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = 0;
} } } } } }
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; VUbfJ7Rp++;
v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; VUbfJ7Rp++;
v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp;
BYTE *CJ6JYaHh; int kT6WFmL3, v32pnQd7; int uidVeSnj;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 2) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ jzr2C3mn = apHGYOU7[KrspwWdU]; *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; VUbfJ7Rp++;
v32pnQd7 = 0; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; VUbfJ7Rp--;
v32pnQd7 = 7; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; VUbfJ7Rp--;
v32pnQd7 = 7; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp;
BYTE *CJ6JYaHh; int kT6WFmL3, v32pnQd7; int uidVeSnj;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy*NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; v32pnQd7 = kT6WFmL3;
for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ jzr2C3mn = apHGYOU7[KrspwWdU]; *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (JhgEoEOk[jzr2C3mn])[v32pnQd7];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[v32pnQd7]) | (JxdvQVIg[jzr2C3mn])[v32pnQd7];
} if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; VUbfJ7Rp--;
v32pnQd7 = 7; } if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } } 
#endif 

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = lcdWidth;
AvwqZ1W4 = lcdWidth*(lcdHeight+7)/8*2;
lhNFI5Ex = NP3y8tCu*((UwJNuSkK + 7)/8); iiaR1Qyh = JZagjmLi;
H5h5hXcT = JZagjmLi + lhNFI5Ex; apHGYOU7 = (BYTE*)SPvM1Gb8[0];
mil_SetGamma(3); mil_SetColor(S_BLACK); mil_ResetClip();
}
void mil_Clear(int D397hZog)
{ BYTE color1, color2; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; color1 = JhgEoEOk[apHGYOU7[D397hZog]][0];
color2 = JxdvQVIg[apHGYOU7[D397hZog]][0];
if (color1) CKOk9Z4Q(iiaR1Qyh, 0xFF, lhNFI5Ex);
else CKOk9Z4Q(iiaR1Qyh, 0x00, lhNFI5Ex); if (color2)
CKOk9Z4Q(H5h5hXcT, 0xFF, lhNFI5Ex); else CKOk9Z4Q(H5h5hXcT, 0x00, lhNFI5Ex);
}
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE bDNXfqbG, qMrXEwsi; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog != S_TRANSPARENT) { bDNXfqbG = JhgEoEOk[apHGYOU7[D397hZog]][0];
qMrXEwsi = JxdvQVIg[apHGYOU7[D397hZog]][0];

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
gIVFkk7i = y & 0x07; G3Ob0FpR = 0x80 >> gIVFkk7i;
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG >> gIVFkk7i);
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi >> gIVFkk7i);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
gIVFkk7i = 7 - (y & 0x07); G3Ob0FpR = 0x80 >> gIVFkk7i;
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG >> gIVFkk7i);
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi >> gIVFkk7i);

#endif
} }
static void kSeyq5l8(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE bDNXfqbG, qMrXEwsi; if (jYPKuxnw != S_TRANSPARENT) {
bDNXfqbG = (jYPKuxnw & 0x02) >> 1; qMrXEwsi = (jYPKuxnw & 0x01);

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
gIVFkk7i = 7 - (y & 0x07); G3Ob0FpR = 0x01 << gIVFkk7i;
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG << gIVFkk7i);
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi << gIVFkk7i);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
gIVFkk7i = y & 0x07; G3Ob0FpR = 0x01 << gIVFkk7i;
kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8];
*kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (bDNXfqbG << gIVFkk7i);
kor8Q3C3 += lhNFI5Ex; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (qMrXEwsi << gIVFkk7i);

#endif
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = (JhgEoEOk[apHGYOU7[kkNN5Ev6]][7] << 1) |
JxdvQVIg[apHGYOU7[kkNN5Ev6]][7]; cAQOtAsc = kSeyq5l8;
} }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; if (kkNN5Ev6 == S_TRANSPARENT)
jYPKuxnw = S_TRANSPARENT; else jYPKuxnw = (JhgEoEOk[apHGYOU7[kkNN5Ev6]] [7] << 1) | JxdvQVIg[apHGYOU7[kkNN5Ev6]] [7];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(JhgEoEOk, FJweE8VK[Gamma], sizeof(JhgEoEOk));
sW4PB08i(JxdvQVIg, Cw9bZk4k[Gamma], sizeof(JxdvQVIg));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
int m3ESBpLZ; if (LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK) {

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
gIVFkk7i = y & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
gIVFkk7i = 7 - (y & 0x07); 
#endif
G3Ob0FpR = 0x80 >> gIVFkk7i; kor8Q3C3 = &JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8];
m3ESBpLZ = (*kor8Q3C3 & G3Ob0FpR) ? 0x02 : 0x00;
kor8Q3C3 += lhNFI5Ex; m3ESBpLZ += (*kor8Q3C3 & G3Ob0FpR) ? 0x01 : 0x00;
return m3ESBpLZ; } else { return -1; } }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int OOzmyCcI, N3jRR8ZR;
BYTE G3Ob0FpR; int mSdyqwa7; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
G3Ob0FpR = 0xFF;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
G3Ob0FpR = G3Ob0FpR >> (qb0L1TPI % 8); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
G3Ob0FpR = G3Ob0FpR << (qb0L1TPI % 8); 
#endif
for (N3jRR8ZR = qb0L1TPI - (qb0L1TPI % 8); (N3jRR8ZR + 7) <= TYFI6PC8; N3jRR8ZR += 8) {
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR >> 3)*NP3y8tCu + CYVLXJkW];
for (OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; *(kor8Q3C3 + lhNFI5Ex) = (*(kor8Q3C3 + lhNFI5Ex)) ^ G3Ob0FpR;
kor8Q3C3++; } G3Ob0FpR = 0xFF; } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
G3Ob0FpR = G3Ob0FpR << (7 - (TYFI6PC8 % 8));

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
G3Ob0FpR = G3Ob0FpR >> (7 - (TYFI6PC8 % 8));

#endif
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR >> 3)*NP3y8tCu + CYVLXJkW];
for (OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; *(kor8Q3C3 + lhNFI5Ex) = (*(kor8Q3C3 + lhNFI5Ex)) ^ G3Ob0FpR;
kor8Q3C3++; } }
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdy;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE*2];
BYTE gIVFkk7i; if((wrap & (~0x01)) || (lcd & (~0x01)))
return; if (lcd == 0) { tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if (dx > h8iL9Zij || dx < -h8iL9Zij) {
if (wrap == 1) { dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < (UwJNuSkK/8); N3jRR8ZR++) {
if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[(N3jRR8ZR+1)*h8iL9Zij - dx];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx); sW4PB08i(s_ScrollBuffer + dx, kor8Q3C3 + lhNFI5Ex, dx);
} kor8Q3C3 = &tempLcd[N3jRR8ZR*h8iL9Zij];
GICEruWy(kor8Q3C3 + dx, kor8Q3C3, h8iL9Zij - dx);
GICEruWy(kor8Q3C3 + dx + lhNFI5Ex, kor8Q3C3 + lhNFI5Ex, h8iL9Zij - dx);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx);
sW4PB08i(kor8Q3C3 + lhNFI5Ex, s_ScrollBuffer + dx, dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx);
CKOk9Z4Q(kor8Q3C3 + lhNFI5Ex, 0, dx); } } else { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*h8iL9Zij]; if (wrap == 1) { 
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dx); sW4PB08i(s_ScrollBuffer - dx, kor8Q3C3 + lhNFI5Ex, -dx);
} GICEruWy(kor8Q3C3, kor8Q3C3 - dx, h8iL9Zij + dx);
GICEruWy(kor8Q3C3 + lhNFI5Ex, kor8Q3C3 - dx + lhNFI5Ex, h8iL9Zij + dx);
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*h8iL9Zij + dx];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dx);
sW4PB08i(kor8Q3C3 + lhNFI5Ex, s_ScrollBuffer - dx, -dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dx);
CKOk9Z4Q(kor8Q3C3 + lhNFI5Ex, 0, -dx); } }
} } if (dy > UwJNuSkK || dy < -UwJNuSkK) {
if (wrap == 1) { dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } }
if (dy != 0) {
tdy = dy; for (OOzmyCcI = 0; OOzmyCcI < h8iL9Zij; OOzmyCcI++) {
dy = tdy; if (dy % 8) { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; s_ScrollBuffer[0] = kor8Q3C3[((UwJNuSkK - 1)/8)*NP3y8tCu];
s_ScrollBuffer[1] = kor8Q3C3[(((UwJNuSkK - 1)/8)*NP3y8tCu) + lhNFI5Ex];
gIVFkk7i = (((dy % 8) + 8) % 8); 

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
for (N3jRR8ZR = (((UwJNuSkK - 1)/8)*NP3y8tCu); N3jRR8ZR > 0; N3jRR8ZR -= NP3y8tCu) {
kor8Q3C3[N3jRR8ZR] = (kor8Q3C3[N3jRR8ZR] >> gIVFkk7i) 
| (kor8Q3C3[N3jRR8ZR - NP3y8tCu] << (8 - gIVFkk7i));
kor8Q3C3[N3jRR8ZR + lhNFI5Ex] = (kor8Q3C3[N3jRR8ZR + lhNFI5Ex] >> gIVFkk7i) 
| (kor8Q3C3[N3jRR8ZR - NP3y8tCu + lhNFI5Ex] << (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] >> gIVFkk7i) | s_ScrollBuffer[0] << (8 - gIVFkk7i);
kor8Q3C3[0 + lhNFI5Ex] = (kor8Q3C3[0 + lhNFI5Ex] >> gIVFkk7i)
| (s_ScrollBuffer[1] << (8 - gIVFkk7i)); } else {
if (dy > 0) { 
kor8Q3C3[0] = (kor8Q3C3[0] >> gIVFkk7i); kor8Q3C3[lhNFI5Ex] = (kor8Q3C3[lhNFI5Ex] >> gIVFkk7i);
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] << (8 - gIVFkk7i);
kor8Q3C3[lhNFI5Ex] = (s_ScrollBuffer[1] << (8 - gIVFkk7i));
} } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
for (N3jRR8ZR = (((UwJNuSkK - 1)/8)*NP3y8tCu); N3jRR8ZR > 0; N3jRR8ZR -= NP3y8tCu) {
kor8Q3C3[N3jRR8ZR] = (kor8Q3C3[N3jRR8ZR] << gIVFkk7i) 
| (kor8Q3C3[N3jRR8ZR-NP3y8tCu] >> (8 - gIVFkk7i));
kor8Q3C3[N3jRR8ZR + lhNFI5Ex] = (kor8Q3C3[N3jRR8ZR + lhNFI5Ex] << gIVFkk7i) 
| (kor8Q3C3[N3jRR8ZR - NP3y8tCu + lhNFI5Ex] >> (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i) | s_ScrollBuffer[0] >> (8 - gIVFkk7i);
kor8Q3C3[lhNFI5Ex] = (kor8Q3C3[lhNFI5Ex] << gIVFkk7i)
| (s_ScrollBuffer[1] >> (8 - gIVFkk7i)); } else {
if (dy > 0) { 
kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i); kor8Q3C3[lhNFI5Ex] = (kor8Q3C3[lhNFI5Ex] << gIVFkk7i);
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] >> (8 - gIVFkk7i);
kor8Q3C3[lhNFI5Ex] = (s_ScrollBuffer[1] >> (8 - gIVFkk7i));
} } 
#endif
dy -= gIVFkk7i; } dy /= 8; if (dy > 0) { 
kor8Q3C3 = &tempLcd[((((UwJNuSkK + 7)/8) - dy)*NP3y8tCu) + OOzmyCcI];
if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu) {
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; s_ScrollBuffer[N3jRR8ZR+dy] = *(kor8Q3C3 + lhNFI5Ex);
} } kor8Q3C3 = &tempLcd[OOzmyCcI]; for (N3jRR8ZR = (((UwJNuSkK + 7)/8) - dy - 1); N3jRR8ZR >= 0; N3jRR8ZR--) {
kor8Q3C3[(N3jRR8ZR + dy)*NP3y8tCu] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
kor8Q3C3[(N3jRR8ZR + dy)*NP3y8tCu + lhNFI5Ex] 
= kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex];
} if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
kor8Q3C3[N3jRR8ZR*NP3y8tCu+ lhNFI5Ex]  = s_ScrollBuffer[N3jRR8ZR + dy];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = 0;
} } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[N3jRR8ZR-dy] = kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex];
} } for (N3jRR8ZR = 0; N3jRR8ZR < (((UwJNuSkK + 7)/8) + dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex]  = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu + lhNFI5Ex];
} kor8Q3C3 = &tempLcd[(((UwJNuSkK + 7)/8) + dy)*NP3y8tCu + OOzmyCcI];
if (wrap == 1) { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = s_ScrollBuffer[N3jRR8ZR-dy];
} } else { for (N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = 0; kor8Q3C3[N3jRR8ZR*NP3y8tCu + lhNFI5Ex] = 0;
} } } } } }
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + cx;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI++;
VUbfJ7Rp++; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + cx;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI++;
VUbfJ7Rp++; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp;
BYTE *CJ6JYaHh; int kT6WFmL3; int uidVeSnj;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + cx;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 2) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ jzr2C3mn = apHGYOU7[KrspwWdU]; *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI++;
VUbfJ7Rp++; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + (cx + qm7J64q3 - 1);

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI--;
VUbfJ7Rp--; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp; BYTE *CJ6JYaHh;
int kT6WFmL3; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + (cx + qm7J64q3 - 1);

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI--;
VUbfJ7Rp--; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI, *VUbfJ7Rp;
BYTE *CJ6JYaHh; int kT6WFmL3; int uidVeSnj;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cy >> 3)*NP3y8tCu + (cx + qm7J64q3 - 1);

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
kT6WFmL3 = cy & 0x07; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
kT6WFmL3 = 7 - (cy & 0x07); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
VUbfJ7Rp = XBx7hZIP + lhNFI5Ex; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ jzr2C3mn = apHGYOU7[KrspwWdU]; *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[kT6WFmL3]) | (JhgEoEOk[jzr2C3mn])[kT6WFmL3];
*VUbfJ7Rp = (*VUbfJ7Rp & RwVPjzxH[kT6WFmL3]) | (JxdvQVIg[jzr2C3mn])[kT6WFmL3];
} FcyUMPlI--;
VUbfJ7Rp--; if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_MF)
if (kT6WFmL3 != 7) { kT6WFmL3++; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_MP_VERT_LF)
if (kT6WFmL3 != 0) { kT6WFmL3--; } else {
XBx7hZIP += NP3y8tCu; kT6WFmL3 = 7; } 
#endif
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } } 
#endif 

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF || SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = (lcdWidth + 3)/4;
AvwqZ1W4 = (lcdWidth+3)/4*lcdHeight;
apHGYOU7 = (BYTE*)SPvM1Gb8[0]; mil_SetGamma(3);
mil_SetColor(S_BLACK); mil_ResetClip(); }
void mil_Clear(int D397hZog)
{ BYTE LG0dStpd; BYTE m3ESBpLZ; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; LG0dStpd = boNcuwqS[apHGYOU7[D397hZog]][3];
m3ESBpLZ = (LG0dStpd << 6) | (LG0dStpd << 4) | (LG0dStpd << 2) | (LG0dStpd);
CKOk9Z4Q(JZagjmLi, m3ESBpLZ, AvwqZ1W4); }
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
BYTE m3ESBpLZ; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog == S_TRANSPARENT) return;
m3ESBpLZ = boNcuwqS[apHGYOU7[D397hZog]][3];
kor8Q3C3 = &JZagjmLi[(LbPt5vw8 >> 2)*UwJNuSkK + y];

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (m3ESBpLZ << gIVFkk7i);
}
static void kSeyq5l8(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (jYPKuxnw != S_TRANSPARENT) { kor8Q3C3 = &JZagjmLi[(LbPt5vw8 >> 2)*UwJNuSkK + y];

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; *kor8Q3C3 = (*kor8Q3C3 & ~G3Ob0FpR) | (jYPKuxnw << gIVFkk7i);
} }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
cAQOtAsc = kSeyq5l8; } }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; if (kkNN5Ev6 == S_TRANSPARENT)
jYPKuxnw = S_TRANSPARENT; else jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]][3];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(boNcuwqS, dFi5bO99[Gamma], sizeof(boNcuwqS));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ BYTE *kor8Q3C3; BYTE G3Ob0FpR; BYTE gIVFkk7i;
if (LbPt5vw8 >= 0 && LbPt5vw8 < h8iL9Zij && y >= 0 && y < UwJNuSkK) {
kor8Q3C3 = &JZagjmLi[(LbPt5vw8 >> 2)*UwJNuSkK + y];

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
gIVFkk7i = 6 - ((LbPt5vw8 & 0x03) << 1); 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
gIVFkk7i = (LbPt5vw8 & 0x03) << 1; 
#endif
G3Ob0FpR = 0x03 << gIVFkk7i; return (*kor8Q3C3 & G3Ob0FpR) >> gIVFkk7i;
} else { return -1; } }
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int mSdyqwa7; BYTE G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
else if (CYVLXJkW > Kh97ARZU) CYVLXJkW = Kh97ARZU;
if (LINj8eGO < f3CK6csT) LINj8eGO = f3CK6csT;
else if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
if (qb0L1TPI < s_ClipY1) qb0L1TPI = s_ClipY1;
else if (qb0L1TPI > s_ClipY2) qb0L1TPI = s_ClipY2;
if (TYFI6PC8 < s_ClipY1) TYFI6PC8 = s_ClipY1;
else if (TYFI6PC8 > s_ClipY2) TYFI6PC8 = s_ClipY2;
G3Ob0FpR = 0xFF; for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
G3Ob0FpR = G3Ob0FpR >> ((CYVLXJkW & 0x03)*2);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
G3Ob0FpR = G3Ob0FpR << ((CYVLXJkW & 0x03)*2);

#endif
kor8Q3C3 = &JZagjmLi[(CYVLXJkW >> 2)*UwJNuSkK + N3jRR8ZR];
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += UwJNuSkK;
G3Ob0FpR = 0xFF; for (OOzmyCcI = (CYVLXJkW >> 2) + 1; OOzmyCcI < (LINj8eGO >> 2); OOzmyCcI++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3 += UwJNuSkK;
} } 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
G3Ob0FpR = G3Ob0FpR << ((LINj8eGO & 0x03)*2);

#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
G3Ob0FpR = G3Ob0FpR >> ((LINj8eGO & 0x03)*2);

#endif
kor8Q3C3 = &JZagjmLi[(LINj8eGO >> 2)*UwJNuSkK + qb0L1TPI];
for (N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++) {
*kor8Q3C3 = (*kor8Q3C3) ^ G3Ob0FpR; kor8Q3C3++;
} }

#define uoQaTMb8 NP3y8tCu
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdx; BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE];
BYTE gIVFkk7i; if((wrap & (~0x01)) || (lcd & (~0x01)))
return; if (lcd == 0) { tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if (dx > h8iL9Zij || dx < -h8iL9Zij) {
if (wrap == 1) { dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } } tdx = dx; if (dx != 0) {
for (N3jRR8ZR = 0; N3jRR8ZR < UwJNuSkK; N3jRR8ZR++) {
dx = tdx; if (dx % 4) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR]; s_ScrollBuffer[0] = kor8Q3C3[(NP3y8tCu - 1)*UwJNuSkK];
gIVFkk7i = (((dx % 4) + 4) % 4)*2; 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
for (OOzmyCcI = ((NP3y8tCu - 1)*UwJNuSkK); OOzmyCcI > 0; OOzmyCcI -= UwJNuSkK) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI - UwJNuSkK] << (8 - gIVFkk7i)) | (kor8Q3C3[OOzmyCcI] >> gIVFkk7i);
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] >> gIVFkk7i) 
| (s_ScrollBuffer[0] << (8 - gIVFkk7i)); } else {
if (dx > 0) { kor8Q3C3[0] = kor8Q3C3[0] >> gIVFkk7i;
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] << (8 - gIVFkk7i);
} } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
for (OOzmyCcI = ((NP3y8tCu - 1)*UwJNuSkK); OOzmyCcI > 0; OOzmyCcI -= UwJNuSkK) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI - UwJNuSkK] >> (8 - gIVFkk7i)) | (kor8Q3C3[OOzmyCcI] << gIVFkk7i);
} if (wrap == 1) { kor8Q3C3[0] = (kor8Q3C3[0] << gIVFkk7i) 
| (s_ScrollBuffer[0] >> (8 - gIVFkk7i)); } else {
if (dx > 0) { kor8Q3C3[0] = kor8Q3C3[0] << gIVFkk7i;
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] >> (8 - gIVFkk7i);
} } 
#endif
dx -= gIVFkk7i/2; } dx /= 4; if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[((NP3y8tCu - dx)*UwJNuSkK) + N3jRR8ZR];
for (OOzmyCcI = 0; OOzmyCcI < dx; OOzmyCcI++) {
s_ScrollBuffer[OOzmyCcI] = kor8Q3C3[OOzmyCcI*UwJNuSkK];
} } kor8Q3C3 = &tempLcd[((NP3y8tCu - 1)*UwJNuSkK) + N3jRR8ZR];
for (OOzmyCcI = 0; OOzmyCcI < (NP3y8tCu - dx); OOzmyCcI++, kor8Q3C3 -= UwJNuSkK) {
*kor8Q3C3 = *(kor8Q3C3 - (dx*UwJNuSkK)); }
kor8Q3C3 = &tempLcd[N3jRR8ZR]; if (wrap == 1) {
for (OOzmyCcI = 0; OOzmyCcI < dx; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI*UwJNuSkK] = s_ScrollBuffer[OOzmyCcI];
} } else { for (OOzmyCcI = 0; OOzmyCcI < dx; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI*UwJNuSkK] = 0; } } } else if (dx < 0) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR]; if (wrap == 1) { 
for (OOzmyCcI = 0; OOzmyCcI < -dx; OOzmyCcI++) {
s_ScrollBuffer[OOzmyCcI] = kor8Q3C3[OOzmyCcI*UwJNuSkK];
} } for (OOzmyCcI = 0; OOzmyCcI < (NP3y8tCu + dx); OOzmyCcI++, kor8Q3C3 += UwJNuSkK) {
*kor8Q3C3 = *(kor8Q3C3 - (dx*UwJNuSkK)); }
kor8Q3C3 = &tempLcd[((NP3y8tCu + dx)*UwJNuSkK) + N3jRR8ZR];
if (wrap == 1) { for (OOzmyCcI = 0; OOzmyCcI < -dx; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI*UwJNuSkK] = s_ScrollBuffer[OOzmyCcI];
} } else { for (OOzmyCcI = 0; OOzmyCcI < -dx; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI*UwJNuSkK] = 0; } } } } }
if (dy > UwJNuSkK || dy < -UwJNuSkK) { if (wrap == 1) {
dy %= UwJNuSkK; } else { dy = UwJNuSkK; }
} if (dy != 0) {
if (dy > 0) { 
for (OOzmyCcI = 0; OOzmyCcI < uoQaTMb8; OOzmyCcI++) {
kor8Q3C3 = &tempLcd[((OOzmyCcI + 1)*UwJNuSkK) - dy];
if (wrap == 1) { sW4PB08i(s_ScrollBuffer, kor8Q3C3, dy);
} kor8Q3C3 = &tempLcd[OOzmyCcI*UwJNuSkK];
GICEruWy(kor8Q3C3 + dy, kor8Q3C3, UwJNuSkK - dy);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dy);
} else { CKOk9Z4Q(kor8Q3C3, 0, dy); } } } else { 
for (OOzmyCcI = 0; OOzmyCcI < uoQaTMb8; OOzmyCcI++) {
kor8Q3C3 = &tempLcd[OOzmyCcI*UwJNuSkK]; if (wrap == 1) {
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dy); }
GICEruWy(kor8Q3C3, kor8Q3C3 - dy, UwJNuSkK + dy);
kor8Q3C3 = &tempLcd[(OOzmyCcI + 1)*UwJNuSkK + dy];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dy);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dy); } } }
} }
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cx >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | (boNcuwqS[jzr2C3mn])[v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cx >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cx >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = cx & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - (cx & 0x03); 
#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 2) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[apHGYOU7[KrspwWdU]][v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 0; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI += UwJNuSkK;
v32pnQd7 = 3; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + ((cx + qm7J64q3 - 1) >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + ((cx + qm7J64q3 - 1) >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) << 1) & 0x07;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[jzr2C3mn][v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg);
vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + ((cx + qm7J64q3 - 1) >> 2)*UwJNuSkK + cy;

#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x03; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
kT6WFmL3 = 3 - ((cx + qm7J64q3 - 1) & 0x03);

#endif
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l*vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l*vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; if (KrspwWdU != S_TRANSPARENT) 
{ *FcyUMPlI = (*FcyUMPlI & RwVPjzxH[v32pnQd7]) | boNcuwqS[apHGYOU7[KrspwWdU]][v32pnQd7];
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_MF)
if (v32pnQd7 != 0) v32pnQd7--; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 3; } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_GRAY_VM_HORIZ_LF)
if (v32pnQd7 != 3) v32pnQd7++; else { FcyUMPlI -= UwJNuSkK;
v32pnQd7 = 0; } 
#endif
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } } XBx7hZIP++;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} } } 
#endif 
void mil_CopyImage(BYTE* TB4GUKNg, int cx, int cy)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} void mil_CopyImageDir(BYTE* TB4GUKNg, int cx, int cy, int W11wt2C6)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
if (!W11wt2C6) { cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} else { cx -= VDI_WIDTH(TB4GUKNg) - VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); inTEQRbH[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} }
void mil_CopyImagePal(BYTE* TB4GUKNg, int cx, int cy, BYTE *OEjq6ZgK)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
}
void mil_CopyImageDirPal(BYTE* TB4GUKNg, int cx, int cy, int W11wt2C6, BYTE *OEjq6ZgK)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
if (!W11wt2C6) { cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
} else { cx -= VDI_WIDTH(TB4GUKNg) - VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); inTEQRbH[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
} } 
#endif 
