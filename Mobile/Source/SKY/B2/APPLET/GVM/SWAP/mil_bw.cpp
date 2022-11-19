//==============================================================================
//
//	Black/white & gray LCD library
//
//==============================================================================

#ifdef SWAP_LCD_BW

#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)

#define b8heDkBn(LbPt5vw8, y) JZagjmLi[(y)*NP3y8tCu + ((LbPt5vw8) >> 3)]

#define Fiyr9w3x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) & (0x80 >> ((LbPt5vw8) & 0x07)))

#define UsjwID4x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) |= (0x80 >> ((LbPt5vw8) & 0x07)))

#define EEF7iunr(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) &= ~(0x80 >> ((LbPt5vw8) & 0x07)))

#define Xgnuagnt(qm7J64q3) (((qm7J64q3) + 7)/8)

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)

#define b8heDkBn(LbPt5vw8, y) JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8]

#define Fiyr9w3x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) & (0x01 << ((y) & 0x07)))

#define UsjwID4x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) |= (0x01 << ((y) & 0x07)))

#define EEF7iunr(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) &= ~(0x01 << ((y) & 0x07)))

#define Xgnuagnt(qm7J64q3) (qm7J64q3)

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)

#define b8heDkBn(LbPt5vw8, y) JZagjmLi[(y >> 3)*NP3y8tCu + LbPt5vw8]

#define Fiyr9w3x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) & (0x80 >> ((y) & 0x07)))

#define UsjwID4x(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) |= (0x80 >> ((y) & 0x07)))

#define EEF7iunr(LbPt5vw8, y) (b8heDkBn(LbPt5vw8, y) &= ~(0x80 >> ((y) & 0x07)))

#define Xgnuagnt(qm7J64q3) (qm7J64q3)

#endif 
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
static const BYTE JZLJSz2M[] = { 0x03, 0x03, 0x03, 0x03 };
static const BYTE EOI4B9jZ[] = { 0x02, 0x03, 0x03, 0x03 };
static const BYTE oEnAig1J[] = { 0x01, 0x02, 0x03, 0x03 };
static const BYTE TmtaAsPv[] = { 0x00, 0x01, 0x02, 0x03 };
static const BYTE Cw9bZk4k[] = { 0x00, 0x00, 0x01, 0x02 };
static const BYTE FJweE8VK[] = { 0x00, 0x00, 0x01, 0x01 };
static const BYTE GBecIdEn[] = { 0x00, 0x00, 0x00, 0x00 };
static const BYTE* dFi5bO99[] = { GBecIdEn, FJweE8VK, Cw9bZk4k, TmtaAsPv, oEnAig1J, EOI4B9jZ, JZLJSz2M };
static BYTE boNcuwqS[4];
static BYTE* apHGYOU7; 
static BYTE jYPKuxnw; 
static BYTE kkNN5Ev6; 
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp);
static void (*SZysP4qw[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&iW0pdjQP, &pohX7Er3, &jGtO1FB9, }; static void (*inTEQRbH[])(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp) = {
&IOd5DYzC, &VJ9k9RQN, &M1P1NHy9, };
void mil_InitLcd(int lcdWidth, int lcdHeight, BYTE* foreBuf, BYTE* backBuf)
{ JZagjmLi = foreBuf; nmRH4auG = backBuf;
h8iL9Zij = lcdWidth; UwJNuSkK = lcdHeight;
NP3y8tCu = Xgnuagnt(lcdWidth); 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
AvwqZ1W4 = (lcdWidth+7)/8*lcdHeight; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF) || (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
AvwqZ1W4 = lcdWidth*(lcdHeight+7)/8; 
#endif
apHGYOU7 = (BYTE*)SPvM1Gb8[0]; U6liRwBT = 3;
mil_SetGamma(3); mil_SetColor(S_BLACK); mil_ResetClip();
}
void mil_ClearWhite()
{ CKOk9Z4Q(JZagjmLi, 0x00, AvwqZ1W4); } void mil_ClearBlack()
{ CKOk9Z4Q(JZagjmLi, 0xFF, AvwqZ1W4); } void mil_Clear(int D397hZog)
{ 
#if(SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF )
BYTE *kor8Q3C3; int row; 
#endif
int LG0dStpd; int col; if (D397hZog == S_TRANSPARENT)
D397hZog = S_WHITE; LG0dStpd = boNcuwqS[apHGYOU7[D397hZog]];
switch (LG0dStpd) { case S_GRAY3: 
CKOk9Z4Q(JZagjmLi, 0xFF, AvwqZ1W4); break;

#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
case S_GRAY2: 
kor8Q3C3 = JZagjmLi; col = Xgnuagnt(h8iL9Zij);
for (row = 0; row < UwJNuSkK; row += 2) {
CKOk9Z4Q(kor8Q3C3, 0x55, col); kor8Q3C3 += col;
CKOk9Z4Q(kor8Q3C3, 0xAA, col); kor8Q3C3 += col;
} break; case S_GRAY1: 
kor8Q3C3 = JZagjmLi; col = Xgnuagnt(h8iL9Zij);
for (row = 0; row < UwJNuSkK; row += 2) {
CKOk9Z4Q(kor8Q3C3, 0x55, col); kor8Q3C3 += col;
CKOk9Z4Q(kor8Q3C3, 0x00, col); kor8Q3C3 += col;
} break; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY2: 
for (col = 0; col < AvwqZ1W4;) { JZagjmLi[col++] = 0x55;
JZagjmLi[col++] = 0xAA; } break; case S_GRAY1: 
for (col = 0; col < AvwqZ1W4;) { JZagjmLi[col++] = 0x55;
JZagjmLi[col++] = 0x00; } break; 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY2: 
for (col = 0; col < AvwqZ1W4;) { JZagjmLi[col++] = 0xAA;
JZagjmLi[col++] = 0x55; } break; case S_GRAY1: 
for (col = 0; col < AvwqZ1W4;) { JZagjmLi[col++] = 0x00;
JZagjmLi[col++] = 0x55; } break; 
#endif
case S_GRAY0: 
CKOk9Z4Q(JZagjmLi, 0x00, AvwqZ1W4); break;
} }
void mil_PutPixel(int LbPt5vw8, int y, int D397hZog)
{ int LG0dStpd; int LINj8eGO; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU || y < s_ClipY1 || y > s_ClipY2)
return; if (D397hZog == S_TRANSPARENT) return;
LG0dStpd = boNcuwqS[apHGYOU7[D397hZog]]; switch (LG0dStpd)
{ case S_GRAY3: UsjwID4x(LbPt5vw8, y); break;
case S_GRAY0: EEF7iunr(LbPt5vw8, y); break;
case S_GRAY2: 
if (LbPt5vw8%2 != y%2) UsjwID4x(LbPt5vw8, y);
else EEF7iunr(LbPt5vw8, y); break; case S_GRAY1: 
LINj8eGO = LbPt5vw8%2; if (LINj8eGO == 1 && LINj8eGO == y%2)
UsjwID4x(LbPt5vw8, y); else EEF7iunr(LbPt5vw8, y);
break; } }
static void PaCZSuFD(int LbPt5vw8, int y)
{ int LINj8eGO; switch (jYPKuxnw) { case S_GRAY3:
UsjwID4x(LbPt5vw8, y); break; case S_GRAY0:
EEF7iunr(LbPt5vw8, y); break; case S_GRAY2: 
if (LbPt5vw8%2 != y%2) UsjwID4x(LbPt5vw8, y);
else EEF7iunr(LbPt5vw8, y); break; case S_GRAY1: 
LINj8eGO = LbPt5vw8%2; if (LINj8eGO == 1 && LINj8eGO == y%2)
UsjwID4x(LbPt5vw8, y); else EEF7iunr(LbPt5vw8, y);
break; } }
void mil_SetColor(int D397hZog)
{ b5Lr0cwi = D397hZog; kkNN5Ev6 = D397hZog;
if (D397hZog == S_TRANSPARENT) jYPKuxnw = S_TRANSPARENT;
else { jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]];
cAQOtAsc = PaCZSuFD; } }
void mil_SetBlinkColor(int frame6)
{ apHGYOU7 = (BYTE*)SPvM1Gb8[frame6]; jYPKuxnw = boNcuwqS[apHGYOU7[kkNN5Ev6]];
}
void mil_SetGamma(int Gamma)
{ U6liRwBT = Gamma; sW4PB08i(boNcuwqS, dFi5bO99[Gamma], sizeof(boNcuwqS));
mil_SetColor(kkNN5Ev6); }
int mil_GetPixel(int LbPt5vw8, int y)
{ return Fiyr9w3x( LbPt5vw8, y );
}
void mil_InvertRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ BYTE *kor8Q3C3; int mSdyqwa7; BYTE G3Ob0FpR;
int OOzmyCcI, N3jRR8ZR; if (CYVLXJkW > LINj8eGO) {
mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8) {
mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; }
if( CYVLXJkW < f3CK6csT ) CYVLXJkW = f3CK6csT;
else if( CYVLXJkW > Kh97ARZU ) CYVLXJkW = Kh97ARZU;
if( LINj8eGO < f3CK6csT ) LINj8eGO = f3CK6csT;
else if( LINj8eGO > Kh97ARZU ) LINj8eGO = Kh97ARZU;
if( qb0L1TPI < s_ClipY1 ) qb0L1TPI = s_ClipY1;
else if( qb0L1TPI > s_ClipY2 ) qb0L1TPI = s_ClipY2;
if( TYFI6PC8 < s_ClipY1 ) TYFI6PC8 = s_ClipY1;
else if( TYFI6PC8 > s_ClipY2 ) TYFI6PC8 = s_ClipY2;

#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
for( N3jRR8ZR = qb0L1TPI; N3jRR8ZR <= TYFI6PC8; N3jRR8ZR++ ){
G3Ob0FpR = 0xFF >> ( CYVLXJkW % 8 ); kor8Q3C3 = &JZagjmLi[N3jRR8ZR * NP3y8tCu + ( CYVLXJkW / 8 )];
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
G3Ob0FpR = 0xFF; for( OOzmyCcI = CYVLXJkW + 8 - ( CYVLXJkW % 8 ); ( OOzmyCcI / 8 ) < ( LINj8eGO / 8 ); OOzmyCcI += 8 ){
*kor8Q3C3 = ~( *kor8Q3C3 ); kor8Q3C3++; }
G3Ob0FpR = G3Ob0FpR << ( 7 - ( LINj8eGO % 8 ));
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
} 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
G3Ob0FpR = 0xFF >> ( qb0L1TPI % 8 ); for( N3jRR8ZR = qb0L1TPI - ( qb0L1TPI % 8 ); ( N3jRR8ZR + 7 ) <= TYFI6PC8; N3jRR8ZR += 8 ){
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR / 8)*NP3y8tCu + CYVLXJkW];
for( OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++ ){
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
} G3Ob0FpR = 0xFF; } G3Ob0FpR = G3Ob0FpR << ( 7 - ( TYFI6PC8 % 8 ));
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR / 8)*NP3y8tCu + CYVLXJkW];
for( OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++ ){
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
} 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
G3Ob0FpR = 0xFF << ( qb0L1TPI % 8 ); for( N3jRR8ZR = qb0L1TPI - ( qb0L1TPI % 8 ); ( N3jRR8ZR + 7 ) <= TYFI6PC8; N3jRR8ZR += 8 ){
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR / 8)*NP3y8tCu + CYVLXJkW];
for( OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++ ){
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
} G3Ob0FpR = 0xFF; } G3Ob0FpR = G3Ob0FpR >> ( 7 - ( TYFI6PC8 % 8 ));
kor8Q3C3 = &JZagjmLi[(N3jRR8ZR / 8)*NP3y8tCu + CYVLXJkW];
for( OOzmyCcI = CYVLXJkW; OOzmyCcI <= LINj8eGO; OOzmyCcI++ ){
*kor8Q3C3 = ( *kor8Q3C3 ) ^ G3Ob0FpR; kor8Q3C3++;
} 
#endif
}
void mil_ScrollLCD(int lcd, int dx, int dy, int wrap)
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
int tdx;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE];
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
gIVFkk7i = dx % 8; for (OOzmyCcI = (NP3y8tCu - 1); OOzmyCcI > 0; OOzmyCcI--) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i) | (kor8Q3C3[OOzmyCcI-1] << (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i) 
| s_ScrollBuffer[0] << (8 - gIVFkk7i); } else {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] >> gIVFkk7i);
} dx -= gIVFkk7i; } else { kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu];
s_ScrollBuffer[0] = kor8Q3C3[0]; gIVFkk7i = -(dx % 8);
for (OOzmyCcI = 0; OOzmyCcI < NP3y8tCu - 1; OOzmyCcI++) {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) | (kor8Q3C3[OOzmyCcI+1] >> (8 - gIVFkk7i));
} if (wrap == 1) { kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i) 
| s_ScrollBuffer[0] >> (8 - gIVFkk7i); } else {
kor8Q3C3[OOzmyCcI] = (kor8Q3C3[OOzmyCcI] << gIVFkk7i); 
} dx += gIVFkk7i; } } dx /= 8; if (dx > 0) { 
if (wrap == 1) { 
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu - dx];
sW4PB08i(s_ScrollBuffer, kor8Q3C3, dx); }
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu];
GICEruWy(kor8Q3C3 + dx, kor8Q3C3, NP3y8tCu - dx);
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, dx);
} } else if (dx < 0) { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*NP3y8tCu]; if (wrap == 1) { 
sW4PB08i(s_ScrollBuffer, kor8Q3C3, -dx); }
GICEruWy(kor8Q3C3, kor8Q3C3 - dx, NP3y8tCu + dx);
kor8Q3C3 = &tempLcd[(N3jRR8ZR + 1)*NP3y8tCu + dx];
if (wrap == 1) { sW4PB08i(kor8Q3C3, s_ScrollBuffer, -dx);
} else { CKOk9Z4Q(kor8Q3C3, 0, -dx);
} } } } if (dy > UwJNuSkK || dy < -UwJNuSkK) {
if (wrap == 1) { dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } } if (dy != 0) {
for (OOzmyCcI = 0; OOzmyCcI < ((h8iL9Zij + 7)/8); OOzmyCcI++) {
if (dy > 0) { 
kor8Q3C3 = &tempLcd[((UwJNuSkK - dy)*NP3y8tCu) + OOzmyCcI];
if (wrap == 1) { 
for (N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu) {
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; } }
kor8Q3C3 = &tempLcd[OOzmyCcI]; for (N3jRR8ZR = UwJNuSkK - 1; N3jRR8ZR >= dy; N3jRR8ZR--) {
kor8Q3C3[N3jRR8ZR*NP3y8tCu] = kor8Q3C3[(N3jRR8ZR - dy)*NP3y8tCu];
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

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
BYTE *kor8Q3C3; BYTE *tempLcd; int OOzmyCcI, N3jRR8ZR;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE]; BYTE gIVFkk7i;
int tdy; if(( wrap & ( ~0x01 )) || ( lcd & ( ~0x01 )))
return; if( lcd == 0 ){ tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if( dx > h8iL9Zij || dx < -h8iL9Zij ){
if( wrap == 1 ){ dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } }
if( dx != 0 ){
for( N3jRR8ZR = 0; N3jRR8ZR < ( UwJNuSkK / 4 ); N3jRR8ZR++ ){
if( dx > 0 ){ 
if( wrap == 1 ){ 
kor8Q3C3 = &tempLcd[( N3jRR8ZR + 1 ) * h8iL9Zij - dx];
sW4PB08i( s_ScrollBuffer, kor8Q3C3, dx );
} kor8Q3C3 = &tempLcd[N3jRR8ZR * h8iL9Zij];
GICEruWy( kor8Q3C3 + dx, kor8Q3C3, h8iL9Zij - dx );
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, dx );
} else { CKOk9Z4Q( kor8Q3C3, 0, dx );
} } else { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*h8iL9Zij]; if( wrap == 1 ){ 
sW4PB08i( s_ScrollBuffer, kor8Q3C3, -dx );
} GICEruWy( kor8Q3C3, kor8Q3C3 - dx, h8iL9Zij + dx );
kor8Q3C3 = &tempLcd[( N3jRR8ZR + 1 ) * h8iL9Zij + dx];
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, -dx );
} else { CKOk9Z4Q( kor8Q3C3, 0, -dx );
} } } } if( dy > UwJNuSkK || dy < -UwJNuSkK ){
if( wrap == 1 ){ dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } } tdy = dy; if( dy != 0 ){
for( OOzmyCcI = 0; OOzmyCcI < h8iL9Zij; OOzmyCcI++ ){
dy = tdy; if( dy % 8 ){ 
kor8Q3C3 = &tempLcd[OOzmyCcI + (((( UwJNuSkK + 7 ) / 8 ) - 1 ) * NP3y8tCu )];
s_ScrollBuffer[0] = *kor8Q3C3; gIVFkk7i = ((( dy % 8 ) + 8 ) % 8 ); 
for( N3jRR8ZR = ((( UwJNuSkK + 7 ) / 8 ) - 1 ); N3jRR8ZR > 0; N3jRR8ZR--, kor8Q3C3 -= NP3y8tCu ){
*kor8Q3C3 = ( *kor8Q3C3 >> gIVFkk7i ) | ( *( kor8Q3C3 - NP3y8tCu ) << ( 8 - gIVFkk7i));
} if( wrap == 1 ){ kor8Q3C3[0] = ( kor8Q3C3[0] >> gIVFkk7i) | s_ScrollBuffer[0] << ( 8 - gIVFkk7i );
} else { if( dy > 0 ){ 
kor8Q3C3[0] = ( kor8Q3C3[0] >> gIVFkk7i);
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] << ( 8 - gIVFkk7i);
} } dy -= gIVFkk7i; } dy /= 8; if( dy > 0 ){ 
kor8Q3C3 = &tempLcd[(((( UwJNuSkK + 7 ) / 8 ) - dy ) * NP3y8tCu ) + OOzmyCcI];
if( wrap == 1 ){ 
for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu ){
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; } }
kor8Q3C3 = &tempLcd[OOzmyCcI];
for( N3jRR8ZR = (( UwJNuSkK / 8 ) - dy ); N3jRR8ZR >= 0; N3jRR8ZR-- ){
kor8Q3C3[(N3jRR8ZR + dy) * NP3y8tCu] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = 0; } } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if( wrap == 1 ){ 
for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} } for( N3jRR8ZR = 0; N3jRR8ZR < ((( UwJNuSkK + 7 ) / 8 ) + dy ); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = kor8Q3C3[( N3jRR8ZR - dy ) * NP3y8tCu];
} kor8Q3C3 = &tempLcd[((( UwJNuSkK + 7 ) / 8 ) + dy ) * NP3y8tCu + OOzmyCcI];
if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = 0; } } } }
} 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
BYTE *kor8Q3C3, *tempLcd; int OOzmyCcI, N3jRR8ZR;
BYTE s_ScrollBuffer[S_SCROLL_BUFSIZE];
BYTE gIVFkk7i; int tdy; if(( wrap & ( ~0x01 )) || ( lcd & ( ~0x01 )))
return; if( lcd == 0 ){ tempLcd = JZagjmLi;
} else { tempLcd = nmRH4auG; } if( dx > h8iL9Zij || dx < -h8iL9Zij ){
if( wrap == 1 ){ dx %= h8iL9Zij; } else {
dx = h8iL9Zij; } }
if( dx != 0 ){
for( N3jRR8ZR = 0; N3jRR8ZR < ( UwJNuSkK / 4 ); N3jRR8ZR++ ){
if( dx > 0 ){ 
if( wrap == 1 ){ 
kor8Q3C3 = &tempLcd[( N3jRR8ZR + 1 ) * h8iL9Zij - dx];
sW4PB08i( s_ScrollBuffer, kor8Q3C3, dx );
} kor8Q3C3 = &tempLcd[N3jRR8ZR * h8iL9Zij];
GICEruWy( kor8Q3C3 + dx, kor8Q3C3, h8iL9Zij - dx );
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, dx );
} else { CKOk9Z4Q( kor8Q3C3, 0, dx );
} } else { 
kor8Q3C3 = &tempLcd[N3jRR8ZR*h8iL9Zij]; if( wrap == 1 ){ 
sW4PB08i( s_ScrollBuffer, kor8Q3C3, -dx );
} GICEruWy( kor8Q3C3, kor8Q3C3 - dx, h8iL9Zij + dx );
kor8Q3C3 = &tempLcd[( N3jRR8ZR + 1 ) * h8iL9Zij + dx];
if( wrap == 1 ){ sW4PB08i( kor8Q3C3, s_ScrollBuffer, -dx );
} else { CKOk9Z4Q( kor8Q3C3, 0, -dx );
} } } } if( dy > UwJNuSkK || dy < -UwJNuSkK ){
if( wrap == 1 ){ dy %= UwJNuSkK; } else {
dy = UwJNuSkK; } } tdy = dy;
if( dy != 0 ){
for( OOzmyCcI = 0; OOzmyCcI < h8iL9Zij; OOzmyCcI++ ){
dy = tdy; if( dy % 8 ){ 
kor8Q3C3 = &tempLcd[OOzmyCcI + (((( UwJNuSkK + 7 ) / 8 ) - 1 ) * NP3y8tCu )];
s_ScrollBuffer[0] = *kor8Q3C3; gIVFkk7i = ((( dy % 8 ) + 8 ) % 8 ); 
for( N3jRR8ZR = ((( UwJNuSkK + 7 ) / 8 ) - 1 ); N3jRR8ZR > 0; N3jRR8ZR--, kor8Q3C3 -= NP3y8tCu ){
*kor8Q3C3 = ( *kor8Q3C3 << gIVFkk7i ) | ( *( kor8Q3C3 - NP3y8tCu ) >> ( 8 - gIVFkk7i));
} if( wrap == 1 ){ kor8Q3C3[0] = ( kor8Q3C3[0] << gIVFkk7i) | s_ScrollBuffer[0] >> ( 8 - gIVFkk7i );
} else { if( dy > 0 ){ 
kor8Q3C3[0] = ( kor8Q3C3[0] << gIVFkk7i);
} else { 
kor8Q3C3[0] = s_ScrollBuffer[0] >> ( 8 - gIVFkk7i);
} } dy -= gIVFkk7i; } dy /= 8; if( dy > 0 ){ 
kor8Q3C3 = &tempLcd[(((( UwJNuSkK + 7 ) / 8 ) - dy ) * NP3y8tCu ) + OOzmyCcI];
if( wrap == 1 ){ 
for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++, kor8Q3C3 += NP3y8tCu ){
s_ScrollBuffer[N3jRR8ZR] = *kor8Q3C3; } }
kor8Q3C3 = &tempLcd[OOzmyCcI]; for( N3jRR8ZR = ((( UwJNuSkK + 7 ) / 8 ) - dy - 1 ); N3jRR8ZR >= 0; N3jRR8ZR-- ){
kor8Q3C3[(N3jRR8ZR + dy) * NP3y8tCu] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < dy; N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = 0; } } } else { 
kor8Q3C3 = &tempLcd[OOzmyCcI]; if( wrap == 1 ){ 
for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
s_ScrollBuffer[N3jRR8ZR] = kor8Q3C3[N3jRR8ZR * NP3y8tCu];
} } for( N3jRR8ZR = 0; N3jRR8ZR < ((( UwJNuSkK + 7 ) / 8 ) + dy ); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = kor8Q3C3[( N3jRR8ZR - dy ) * NP3y8tCu];
} kor8Q3C3 = &tempLcd[((( UwJNuSkK + 7 ) / 8 ) + dy ) * NP3y8tCu + OOzmyCcI];
if( wrap == 1 ){ for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = s_ScrollBuffer[N3jRR8ZR];
} } else { for( N3jRR8ZR = 0; N3jRR8ZR < (-dy); N3jRR8ZR++ ){
kor8Q3C3[N3jRR8ZR * NP3y8tCu] = 0; } } } }
} 
#endif
} 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) {
switch (boNcuwqS[jzr2C3mn]) { case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; v32pnQd7 = 0;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; v32pnQd7 = 0;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + (cx >> 3);
kT6WFmL3 = cx & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 2) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = apHGYOU7[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ case S_GRAY3: *(FcyUMPlI) |= 0x80 >> v32pnQd7;
break; case S_GRAY0: *(FcyUMPlI) &= ~(0x80 >> v32pnQd7);
break; case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 7)
{ v32pnQd7++; } else { FcyUMPlI++; v32pnQd7 = 0;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF) || (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
static void iW0pdjQP(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + cx;
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) {
switch (boNcuwqS[jzr2C3mn]) { 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI++;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void pohX7Er3(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + cx;
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI++;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void jGtO1FB9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!JdHT2n1Q(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + cx;
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 2) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = apHGYOU7[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI++;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } } 
#else

#error No Support LCD Type

#endif

#if (SWAP_LCD_TYPE == SWAP_LCD_BW_HORIZ_MF)
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) {
switch (boNcuwqS[jzr2C3mn]) { case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; v32pnQd7 = 7;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; v32pnQd7 = 7;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 1);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x00000001) << 2;
XBx7hZIP = JZagjmLi + cy * NP3y8tCu + ((cx + qm7J64q3 - 1) >> 3);
kT6WFmL3 = (cx + qm7J64q3 - 1) & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = apHGYOU7[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ case S_GRAY3: *(FcyUMPlI) |= 0x80 >> v32pnQd7;
break; case S_GRAY0: *(FcyUMPlI) &= ~(0x80 >> v32pnQd7);
break; case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
} } if (v32pnQd7 != 0)
{ v32pnQd7--; } else { FcyUMPlI--; v32pnQd7 = 7;
} if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} XBx7hZIP += NP3y8tCu;
CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } } 
#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF) || (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
static void IOd5DYzC(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[2]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY1(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 3);
Mux7oWk7 = (vzL7ZKBJ - qm7J64q3) & 0x07;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + (cx + qm7J64q3 - 1);
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 3);
uidVeSnj = (bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (7 - uidVeSnj++)) & 0x01;
jzr2C3mn = LG0dStpd[KrspwWdU]; if (jzr2C3mn != S_TRANSPARENT) {
switch (boNcuwqS[jzr2C3mn]) { 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI--;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void VJ9k9RQN(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE LG0dStpd[4]; BYTE KrspwWdU, jzr2C3mn;
int u4hoxB7X; int Mux7oWk7; int vzL7ZKBJ;
BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY2(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
LG0dStpd[0] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[1] = apHGYOU7[(*ObyzSETp++ & 0x0f) ];
LG0dStpd[2] = apHGYOU7[(*ObyzSETp & 0xf0) >> 4];
LG0dStpd[3] = apHGYOU7[(*ObyzSETp & 0x0f) ];
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + (cx + qm7J64q3 - 1);
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 2);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) << 1) & 0x07;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (6 - uidVeSnj)) & 0x03;
uidVeSnj += 2; jzr2C3mn = LG0dStpd[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI--;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } }
static void M1P1NHy9(BYTE *TB4GUKNg, int cx, int cy, int qm7J64q3, int h, BYTE *ObyzSETp)
{ BYTE *r2Z9u4o4; int LbPt5vw8, y; int IsDSC069, bDefoJ6l;
BYTE KrspwWdU, jzr2C3mn; int u4hoxB7X; int Mux7oWk7;
int vzL7ZKBJ; BYTE *XBx7hZIP, *FcyUMPlI; BYTE *CJ6JYaHh;
int kT6WFmL3, v32pnQd7; int uidVeSnj; BYTE dMask0, dMask1;
r2Z9u4o4 = VDI_BITMAP_GRAY4(TB4GUKNg); vzL7ZKBJ = qm7J64q3;
IsDSC069 = bDefoJ6l = 0; if (!v9V2Pnjk(&IsDSC069, &bDefoJ6l, &qm7J64q3, &h, &cx, &cy))
return; u4hoxB7X = ((vzL7ZKBJ - qm7J64q3) >> 2);
Mux7oWk7 = ((vzL7ZKBJ - qm7J64q3) & 0x03) << 1;
XBx7hZIP = JZagjmLi + (cy >> 3) * NP3y8tCu + (cx + qm7J64q3 - 1);
kT6WFmL3 = cy & 0x07;
CJ6JYaHh = r2Z9u4o4 + ((bDefoJ6l * vzL7ZKBJ + IsDSC069) >> 1);
uidVeSnj = ((bDefoJ6l * vzL7ZKBJ + IsDSC069) & 0x00000001) << 2;
dMask0 = 0;
for (y = 0; y < h; y++) { FcyUMPlI = XBx7hZIP;
v32pnQd7 = kT6WFmL3; dMask1 = dMask0; for (LbPt5vw8 = 0; LbPt5vw8 < qm7J64q3; LbPt5vw8++)
{ KrspwWdU = (*CJ6JYaHh >> (4 - uidVeSnj)) & 0x0F;
uidVeSnj += 4; jzr2C3mn = apHGYOU7[KrspwWdU];
if (jzr2C3mn != S_TRANSPARENT) { switch (boNcuwqS[jzr2C3mn])
{ 
#if (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_MF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x80 >> v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x80 >> v32pnQd7;
else *(FcyUMPlI) &= ~(0x80 >> v32pnQd7); break;

#elif (SWAP_LCD_TYPE == SWAP_LCD_BW_VERT_LF)
case S_GRAY3: 
*(FcyUMPlI) |= 0x01 << v32pnQd7; break; case S_GRAY0: 
*(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY2: 
if (dMask1) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;
case S_GRAY1: 
if (dMask1 && dMask0) *(FcyUMPlI) |= 0x01 << v32pnQd7;
else *(FcyUMPlI) &= ~(0x01 << v32pnQd7); break;

#endif
} } FcyUMPlI--;
if (uidVeSnj >= 8)
{ CJ6JYaHh++; uidVeSnj = 0; } dMask1 = 1 - dMask1;
} kT6WFmL3++;
if (kT6WFmL3 >= 8) { XBx7hZIP += NP3y8tCu;
kT6WFmL3 = kT6WFmL3 & 0x07; } CJ6JYaHh += u4hoxB7X;
uidVeSnj += Mux7oWk7; if (uidVeSnj >= 8) {
CJ6JYaHh += (uidVeSnj) >> 3; uidVeSnj = uidVeSnj & 0x07;
} dMask0 = 1 - dMask0; } } 
#else

#error No Support LCD Type

#endif
void mil_CopyImage(BYTE* TB4GUKNg, int cx, int cy)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} void mil_CopyImagePal(BYTE* TB4GUKNg, int cx, int cy, BYTE *OEjq6ZgK)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, (BYTE*)OEjq6ZgK);
}
void mil_CopyImageDir(BYTE* TB4GUKNg, int cx, int cy, int W11wt2C6)
{ int type; int qm7J64q3, h; type = VDI_TYPE(TB4GUKNg);
if (type != S_IMG_GRAY1 && type != S_IMG_GRAY2 && type != S_IMG_GRAY4)
return; qm7J64q3 = VDI_WIDTH(TB4GUKNg); h = VDI_HEIGHT(TB4GUKNg);
if (!W11wt2C6) { cx -= VDI_CX(TB4GUKNg); cy -= VDI_CY(TB4GUKNg);
SZysP4qw[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} else { cx -= VDI_WIDTH(TB4GUKNg) - VDI_CX(TB4GUKNg);
cy -= VDI_CY(TB4GUKNg); inTEQRbH[type - S_IMG_GRAY1](TB4GUKNg, cx, cy, qm7J64q3, h, VDI_PAL_PTR(TB4GUKNg));
} }
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
