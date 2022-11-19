/==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY
//
//	FILE NAME
//		MIL.cpp
//
//	DESCRIPTION
//		Mobile Interface Library Graphic Part
//
//	REFERENCES
//		SINJISOFT SWAP Porting Guide
//
//	Copyright (C) 2000, 2001 by SINJISOFT. All Rights Reserved.
//
//	HISTORY
//		2000. 5.20  Alpha Release
//		2000. 6.10  Beta Release
//		2000. 7. 3  Beta2 Release
//		2000. 7.24  Beta3 Release
//		2000. 8. 7  Version 1.0 Release
//		2000. 8.28  Version 1.01 Release
//		2000. 9.26  Version 1.02 Release
//		2000.10.27  Version 1.10 Release
//		2000.12.19	Version 1.5 Beta Start
//		2001. 1. 1	Version 1.5 Beta1 Release
//		2001.10.12 	Version 1.55.01 Beta Release (True Color)
//		2001.10.17	Version 1.55.01 Release
//
//==============================================================================

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "GLib.h"

#include "MilFont.def" 

#define sW4PB08i(Jv2UAbfL, WDqujLu9, len) memcpy((void*)(Jv2UAbfL), (void*)(WDqujLu9), (int)(len))

#define CKOk9Z4Q(Jv2UAbfL, c, len) memset((void*)(Jv2UAbfL), (int)(c), (int)(len))

#define GICEruWy(Jv2UAbfL, WDqujLu9, len) memmove((void*)(Jv2UAbfL), (void*)(WDqujLu9), (int)(len))
static BYTE *JZagjmLi; 
static BYTE *nmRH4auG; 
static WORD *uW9Kdys6; 
static WORD *Tsbk5B54; 
static int h8iL9Zij; static int UwJNuSkK;
static BYTE b5Lr0cwi; static int f3CK6csT, s_ClipY1;
static int Kh97ARZU, s_ClipY2; static int U6liRwBT;
static int mkfkxUSx; static int NP3y8tCu;
static int AvwqZ1W4; static void (*cAQOtAsc)(int LbPt5vw8, int y);
int mil_GetLcdBufferSize()
{ return AvwqZ1W4; }
const int fontWidth[] = { 4, 6, 6, 6 }; 
const int fontHeight[] = { 6, 8, 12, 12 }; 
const int isFontKorean[] = { 0, 0, 1, 1 };
const int isFontJapanese[] = { 0, 0, 1, 1 };
const int isFontHebrew[] = { 0, 0, 1, 1 };
static BYTE *lJdEwKs7(int font, char *str, int *skip)
{ BYTE *sCpdgnu5 = NULL; WORD code = (BYTE)*str++;
WORD index; 
#if (SWAP_LANG == SWAP_KSC5601)
if ((code & 0x80) && isFontKorean[font])
{ code = (code << 8) | (BYTE)*str; if (code >= 0xB0A1 && code <= 0xC9FE && (code & 0x8080) == 0x8080)
{ 
code -= 0xB0A1; index = (code >> 8)*0x5e + (code & 0xFF); 
sCpdgnu5 = (BYTE*)&nFN2dtzj[index][0]; } else if (code >= 0xA1A1 && code <= 0xACFE && (code & 0x8080) == 0x8080)
{ 
code -= 0xA1A1; index = (code >> 8)*0x5e + (code & 0xFF); 
sCpdgnu5 = (BYTE*)&A7uvuQlI[index][0]; } else 
sCpdgnu5 = (BYTE*)&nFN2dtzj[0][0]; *skip = 2;
} else 
#endif 

#if (SWAP_LANG == SWAP_SHIFTJIS)
if ((code & 0x80) && isFontJapanese[font])
{ code = (code << 8) | (BYTE)*str; if (0x8240 <= code && code <= 0x82FF) 
index = code - 0x8240 - 0x00C0; else if (0x8340 <= code && code <= 0x83DF) 
index = code - 0x8340 - 0x0180; else if (0x8140 <= code && code <= 0x81FF) 
index = code - 0x8140; else if (0x8440 <= code && code <= 0x84BF) 
index = code - 0x8440 - 0x0220; else if (0x8740 <= code && code <= 0x879F) 
index = code - 0x8740 - 0x02A0; else 
index = 0; sCpdgnu5 = (BYTE*)&LmmADihB[index][0];
*skip = 2; } else  
#endif 

#if (SWAP_LANG == SWAP_HEBREW)
if ((code & 0x80) && isFontHebrew[font])
{ if (160 <= code && code <= 190) index = code - 161;
else if (223 <= code && code <= 250) index = code - 223 + 30;
else index = 0; sCpdgnu5 = (BYTE*)&s_FontLargeHebrew[index][0];
*skip = 1; } else  
#endif 
{
if (code & 0x80) code = 0; switch (font) {
case S_FONT_LARGE: case S_FONT_DOUBLE: sCpdgnu5 = (BYTE*)&dRZl7pCZ[code][0];
break; case S_FONT_MEDIUM: sCpdgnu5 = (BYTE*)&OOEgS7ee[code][0];
break; case S_FONT_SMALL: sCpdgnu5 = (BYTE*)&QhkGDqQp[code][0];
break; default: 
sCpdgnu5 = (BYTE*)&dRZl7pCZ[0][0]; break;
} *skip = 1; } return sCpdgnu5; }
static void XW9UZcmx(int LbPt5vw8, int y, int qm7J64q3, int h, BYTE *sCpdgnu5, int fcr)
{ BYTE bcT4nNv6; int RbDjjTxl, HO29zLHG; int QedZRwdl, M2sxkLzx;
bcT4nNv6 = 0x80; RbDjjTxl = LbPt5vw8 + qm7J64q3 - 1;
HO29zLHG = y + h - 1; if (LbPt5vw8 < f3CK6csT || RbDjjTxl > Kh97ARZU || y < s_ClipY1 || HO29zLHG > s_ClipY2) 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx++)
{ for (QedZRwdl = LbPt5vw8; QedZRwdl < RbDjjTxl; QedZRwdl++)
{ if (*sCpdgnu5 & bcT4nNv6) { if (!(QedZRwdl < f3CK6csT || QedZRwdl > Kh97ARZU || M2sxkLzx < s_ClipY1 || M2sxkLzx > s_ClipY2))
cAQOtAsc(QedZRwdl, M2sxkLzx); } if (bcT4nNv6 == 0x01)
{ bcT4nNv6 = 0x80; sCpdgnu5++; } else bcT4nNv6 >>= 1;
} } } else 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx++)
{ for (QedZRwdl = LbPt5vw8; QedZRwdl < RbDjjTxl; QedZRwdl++)
{ if (*sCpdgnu5 & bcT4nNv6) cAQOtAsc(QedZRwdl, M2sxkLzx);
if (bcT4nNv6 == 0x01) { bcT4nNv6 = 0x80; sCpdgnu5++;
} else bcT4nNv6 >>= 1; } } } }
static void ufOcQ3Xs(int LbPt5vw8, int y, int qm7J64q3, int h, BYTE *sCpdgnu5, int fcr)
{ BYTE bcT4nNv6; int RbDjjTxl, HO29zLHG; int QedZRwdl, M2sxkLzx;
int xi1, yi1; bcT4nNv6 = 0x80;
RbDjjTxl = LbPt5vw8 + (qm7J64q3 << 1) - 3; 
HO29zLHG = y + (h << 1) - 3; 
if (LbPt5vw8 < f3CK6csT || RbDjjTxl + 1 > Kh97ARZU || y < s_ClipY1 || HO29zLHG + 1 > s_ClipY2) 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx += 2)
{ yi1 = M2sxkLzx + 1; for (QedZRwdl = LbPt5vw8; QedZRwdl < RbDjjTxl; QedZRwdl += 2)
{ xi1 = QedZRwdl + 1; if (*sCpdgnu5 & bcT4nNv6)
{ if (!(QedZRwdl < f3CK6csT || QedZRwdl > Kh97ARZU))
{ if (!(M2sxkLzx < s_ClipY1 || M2sxkLzx > s_ClipY2))
cAQOtAsc(QedZRwdl, M2sxkLzx); if (!(yi1 < s_ClipY1 || yi1 > s_ClipY2))
cAQOtAsc(QedZRwdl, yi1); } if (!(xi1 < f3CK6csT || xi1 > Kh97ARZU))
{ if (!(M2sxkLzx < s_ClipY1 || M2sxkLzx > s_ClipY2))
cAQOtAsc(xi1, M2sxkLzx); if (!(yi1 < s_ClipY1 || yi1 > s_ClipY2))
cAQOtAsc(xi1, yi1); } } if (bcT4nNv6 == 0x01)
{ bcT4nNv6 = 0x80; sCpdgnu5++; } else bcT4nNv6 >>= 1;
} } } else 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx += 2)
{ yi1 = M2sxkLzx + 1; for (QedZRwdl = LbPt5vw8; QedZRwdl < RbDjjTxl; QedZRwdl += 2)
{ xi1 = QedZRwdl + 1; if (*sCpdgnu5 & bcT4nNv6)
{ cAQOtAsc(QedZRwdl, M2sxkLzx); cAQOtAsc(xi1, M2sxkLzx);
cAQOtAsc(QedZRwdl, yi1); cAQOtAsc(xi1, yi1);
} if (bcT4nNv6 == 0x01) { bcT4nNv6 = 0x80;
sCpdgnu5++; } else bcT4nNv6 >>= 1; } } } }
void mil_DrawStr(int LbPt5vw8, int y, char* str, int font, int fcr, int bcr, int kJXs8wEH, int solid)
{ int saveColor = b5Lr0cwi; BYTE *sCpdgnu5;
int len, H0Ng1GoT; int skip; int qm7J64q3, h;
len = strlen(str); switch (kJXs8wEH) { case S_LEFT:
break; case S_CENTER: if (font < S_FONT_DOUBLE)
LbPt5vw8 -= (len*fontWidth[font]) >> 1; 
else LbPt5vw8 -= len*fontWidth[font]; break;
case S_RIGHT: if (font < S_FONT_DOUBLE) LbPt5vw8 -= len*fontWidth[font];
else LbPt5vw8 -= (len*fontWidth[font]) << 1; 
break; } switch (font) { case S_FONT_LARGE: 
case S_FONT_MEDIUM: 
case S_FONT_SMALL: 
if (solid) { mil_SetColor(bcr); mil_FillRect(LbPt5vw8 - 1, y - 1, LbPt5vw8 + fontWidth[font]*len - 1, y + fontHeight[font] - 1);
} mil_SetColor(fcr); h = fontHeight[font];
H0Ng1GoT = 0; while (H0Ng1GoT < len) { sCpdgnu5 = lJdEwKs7(font, &str[H0Ng1GoT], &skip);
qm7J64q3 = fontWidth[font]*skip; XW9UZcmx(LbPt5vw8, y, qm7J64q3, h, sCpdgnu5, fcr);
LbPt5vw8 += qm7J64q3; H0Ng1GoT += skip; }
break; case S_FONT_DOUBLE: 
if (solid) { mil_SetColor(bcr); mil_FillRect(LbPt5vw8 - 1, y - 1, LbPt5vw8 + ((fontWidth[font]*len) << 1), y + (fontHeight[font] << 1));
} mil_SetColor(fcr); LbPt5vw8 += 1; y += 1;
h = fontHeight[font]; H0Ng1GoT = 0; while (H0Ng1GoT < len)
{ sCpdgnu5 = lJdEwKs7(font, &str[H0Ng1GoT], &skip);
qm7J64q3 = fontWidth[font]*skip; ufOcQ3Xs(LbPt5vw8, y, qm7J64q3, h, sCpdgnu5, fcr);
LbPt5vw8 += qm7J64q3 << 1; 
H0Ng1GoT += skip; } break; } mil_SetColor(saveColor); 
}
void mil_DrawStr2(int LbPt5vw8, int y, char* str, int font, int fcr, int bcr, int kJXs8wEH, int mode, int solid)
{ int saveColor = b5Lr0cwi; BYTE *sCpdgnu5;
BYTE bcT4nNv6; int len, H0Ng1GoT; int skip;
int qm7J64q3; int QedZRwdl, M2sxkLzx; int RbDjjTxl, HO29zLHG;
int OOzmyCcI, dx; len = strlen(str); switch (kJXs8wEH)
{ case S_LEFT: break; case S_CENTER: if (font < S_FONT_DOUBLE)
LbPt5vw8 -= (len*fontWidth[font]) >> 1; 
else LbPt5vw8 -= len*fontWidth[font]; break;
case S_RIGHT: if (font < S_FONT_DOUBLE) LbPt5vw8 -= len*fontWidth[font];
else LbPt5vw8 -= (len*fontWidth[font]) << 1; 
break; } OOzmyCcI = (mode & S_DRAWSTR2_BOLD) ? 1 : 0;
switch (font) { case S_FONT_LARGE: 
case S_FONT_MEDIUM: 
case S_FONT_SMALL: 
if (solid) { mil_SetColor(bcr); if (mode & S_DRAWSTR2_ITALIC)
OOzmyCcI += (fontHeight[font] >> 2) - 1; 
mil_FillRect(LbPt5vw8 - 1, y - 1, LbPt5vw8 + fontWidth[font]*len + OOzmyCcI - 1, y + fontHeight[font] - 1);
} mil_SetColor(fcr); HO29zLHG = y + fontHeight[font] - 1;
if (mode & S_DRAWSTR2_UNDERLINE) mil_DrawLineH(LbPt5vw8 - 1, LbPt5vw8 + fontWidth[font]*len + OOzmyCcI - 1, HO29zLHG + 1);
H0Ng1GoT = 0; while (H0Ng1GoT < len) { sCpdgnu5 = lJdEwKs7(font, &str[H0Ng1GoT], &skip);
qm7J64q3 = fontWidth[font]*skip - 1; bcT4nNv6 = 0x80;
if (mode & S_DRAWSTR2_ITALIC) OOzmyCcI = (fontHeight[font] >> 2) - 1; 
else OOzmyCcI = 0; if (mode & S_DRAWSTR2_BOLD) 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx++)
{ RbDjjTxl = LbPt5vw8 + qm7J64q3 + OOzmyCcI;
for (QedZRwdl = LbPt5vw8 + OOzmyCcI; QedZRwdl < RbDjjTxl; QedZRwdl++)
{ if (*sCpdgnu5 & bcT4nNv6) { if (!(M2sxkLzx < s_ClipY1 || M2sxkLzx > s_ClipY2))
{ if (!(QedZRwdl < f3CK6csT || QedZRwdl > Kh97ARZU))
cAQOtAsc(QedZRwdl, M2sxkLzx); if (!(QedZRwdl + 1 < f3CK6csT || QedZRwdl + 1 > Kh97ARZU))
cAQOtAsc(QedZRwdl + 1, M2sxkLzx); } } if (bcT4nNv6 == 0x01)
{ bcT4nNv6 = 0x80; sCpdgnu5++; } else bcT4nNv6 >>= 1;
} if (OOzmyCcI && (((y - M2sxkLzx + 2) % 4) == 0))
OOzmyCcI--; } } else 
{ for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx++)
{ RbDjjTxl = LbPt5vw8 + qm7J64q3 + OOzmyCcI;
for (QedZRwdl = LbPt5vw8 + OOzmyCcI; QedZRwdl < RbDjjTxl; QedZRwdl++)
{ if (*sCpdgnu5 & bcT4nNv6) { if (!(QedZRwdl < f3CK6csT || QedZRwdl > Kh97ARZU || M2sxkLzx < s_ClipY1 || M2sxkLzx > s_ClipY2))
cAQOtAsc(QedZRwdl, M2sxkLzx); } if (bcT4nNv6 == 0x01)
{ bcT4nNv6 = 0x80; sCpdgnu5++; } else bcT4nNv6 >>= 1;
} if (OOzmyCcI && (((y - M2sxkLzx + 2) % 4) == 0))
OOzmyCcI--; } } LbPt5vw8 += qm7J64q3 + 1;
H0Ng1GoT += skip; } break; case S_FONT_DOUBLE: 
if (solid) { mil_SetColor(bcr); if (mode & S_DRAWSTR2_ITALIC)
OOzmyCcI += (fontHeight[font] - 8); mil_FillRect(LbPt5vw8 - 1, y - 1, LbPt5vw8 + ((fontWidth[font]*len) << 1) + OOzmyCcI, y + (fontHeight[font] << 1));
} mil_SetColor(fcr); LbPt5vw8 += 1; y += 1;
HO29zLHG = y + ((fontHeight[font] - 1) << 1) - 1; 
if (mode & S_DRAWSTR2_UNDERLINE) mil_DrawLineH(LbPt5vw8 - 2, LbPt5vw8 + ((fontWidth[font]*len) << 1) + OOzmyCcI - 1, HO29zLHG + 2);
H0Ng1GoT = 0; while (H0Ng1GoT < len) { sCpdgnu5 = lJdEwKs7(font, &str[H0Ng1GoT], &skip);
qm7J64q3 = fontWidth[font]*skip - 1; bcT4nNv6 = 0x80;
RbDjjTxl = LbPt5vw8 + (qm7J64q3 << 1) - 1; 
if (mode & S_DRAWSTR2_ITALIC) OOzmyCcI = (fontHeight[font] >> 1) - 2; 
else OOzmyCcI = 0; dx = (mode & S_DRAWSTR2_BOLD) ? 3 : 1 ;
for (M2sxkLzx = y; M2sxkLzx < HO29zLHG; M2sxkLzx += 2)
{ for (QedZRwdl = LbPt5vw8 + OOzmyCcI; QedZRwdl < RbDjjTxl + OOzmyCcI; QedZRwdl += 2)
{ if (*sCpdgnu5 & bcT4nNv6) mil_FillRect(QedZRwdl, M2sxkLzx, QedZRwdl + dx, M2sxkLzx + 1);
if (bcT4nNv6 == 0x01) { bcT4nNv6 = 0x80; sCpdgnu5++;
} else bcT4nNv6 >>= 1; } if (OOzmyCcI && ((y - M2sxkLzx) % 4))
OOzmyCcI -= 1; } LbPt5vw8 += (qm7J64q3 << 1) + 2; 
H0Ng1GoT += skip; } break; } mil_SetColor(saveColor); 
}

#define CLVY98Pr 20

#define d6gJLmZB 0

#define EmBm3cNq 1

#define iKnsjF6q 0 

#define jPiNmNu7 1 

#define r63fdurc 2 

#define USCUWVfg 3 
typedef struct _s_BlockList { BYTE *N4CplloT;
int cx, cy; int dir; } s_BlockList; static s_BlockList gz7Da4KB[CLVY98Pr];
static BYTE IcCNiiQp;
void mil_InitDQ()
{ IcCNiiQp = 0; }
int mil_AddDQ(BYTE *N4CplloT, int LbPt5vw8, int y, int dir)
{ if (IcCNiiQp == CLVY98Pr) return 0; 
gz7Da4KB[IcCNiiQp].N4CplloT = (BYTE*)N4CplloT;
gz7Da4KB[IcCNiiQp].cx = LbPt5vw8; gz7Da4KB[IcCNiiQp].cy = y;
gz7Da4KB[IcCNiiQp].dir = dir; IcCNiiQp++;
return 1; }
void mil_DrawDQ(int type)
{ s_BlockList *pList; BYTE drawOrder[CLVY98Pr];
int H0Ng1GoT, hRldyQfl, a7P6Xwbo; if (IcCNiiQp == 0)
return; for (H0Ng1GoT = 0; H0Ng1GoT < IcCNiiQp; H0Ng1GoT++)
drawOrder[H0Ng1GoT] = H0Ng1GoT; switch (type)
{ case iKnsjF6q: for (H0Ng1GoT = IcCNiiQp - 1; H0Ng1GoT > 0; H0Ng1GoT--)
{ for (hRldyQfl = 0; hRldyQfl < H0Ng1GoT; hRldyQfl++)
{ if (gz7Da4KB[drawOrder[hRldyQfl]].cx > gz7Da4KB[drawOrder[hRldyQfl + 1]].cx)
{ a7P6Xwbo = drawOrder[hRldyQfl]; drawOrder[hRldyQfl] = drawOrder[hRldyQfl + 1];
drawOrder[hRldyQfl + 1] = a7P6Xwbo; } } }
break; case jPiNmNu7: for (H0Ng1GoT = IcCNiiQp - 1; H0Ng1GoT > 0; H0Ng1GoT--)
{ for (hRldyQfl = 0; hRldyQfl < H0Ng1GoT; hRldyQfl++)
{ if (gz7Da4KB[drawOrder[hRldyQfl]].cx < gz7Da4KB[drawOrder[hRldyQfl + 1]].cx)
{ a7P6Xwbo = drawOrder[hRldyQfl]; drawOrder[hRldyQfl] = drawOrder[hRldyQfl + 1];
drawOrder[hRldyQfl + 1] = a7P6Xwbo; } } }
break; case r63fdurc: for (H0Ng1GoT = IcCNiiQp - 1; H0Ng1GoT > 0; H0Ng1GoT--)
{ for (hRldyQfl = 0; hRldyQfl < H0Ng1GoT; hRldyQfl++)
{ if (gz7Da4KB[drawOrder[hRldyQfl]].cy > gz7Da4KB[drawOrder[hRldyQfl + 1]].cy)
{ a7P6Xwbo = drawOrder[hRldyQfl]; drawOrder[hRldyQfl] = drawOrder[hRldyQfl + 1];
drawOrder[hRldyQfl + 1] = a7P6Xwbo; } } }
break; case USCUWVfg: for (H0Ng1GoT = IcCNiiQp - 1; H0Ng1GoT > 0; H0Ng1GoT--)
{ for (hRldyQfl = 0; hRldyQfl < H0Ng1GoT; hRldyQfl++)
{ if (gz7Da4KB[drawOrder[hRldyQfl]].cy < gz7Da4KB[drawOrder[hRldyQfl + 1]].cy)
{ a7P6Xwbo = drawOrder[hRldyQfl]; drawOrder[hRldyQfl] = drawOrder[hRldyQfl + 1];
drawOrder[hRldyQfl + 1] = a7P6Xwbo; } } }
break; } for (H0Ng1GoT = 0; H0Ng1GoT < IcCNiiQp; H0Ng1GoT++)
{ pList = &gz7Da4KB[drawOrder[H0Ng1GoT]];
mil_CopyImageDir(pList->N4CplloT, pList->cx, pList->cy, pList->dir);
} }
static int JdHT2n1Q(int* IsDSC069, int* bDefoJ6l, int* qm7J64q3, int* h, int* cx, int* cy)
{
if (*cx > Kh97ARZU)
return 0; if (*cx < f3CK6csT) { *qm7J64q3 -= f3CK6csT - *cx;
*IsDSC069 += f3CK6csT - *cx; *cx = f3CK6csT;
} if (*cy > s_ClipY2)
return 0; if (*cy < s_ClipY1) { *h -= s_ClipY1 - *cy;
*bDefoJ6l += s_ClipY1 - *cy; *cy = s_ClipY1;
} if (*qm7J64q3 <= 0)
return 0; if (*cx + *qm7J64q3 > Kh97ARZU + 1)
*qm7J64q3 = Kh97ARZU - *cx + 1; if (*h <= 0)
return 0; if (*cy + *h > s_ClipY2 + 1) *h = s_ClipY2 - *cy + 1;
return 1; }
static int v9V2Pnjk(int* IsDSC069, int* bDefoJ6l, int* qm7J64q3, int* h, int* cx, int* cy)
{
if (*cx > Kh97ARZU)
return 0; if (*cx < f3CK6csT) { *qm7J64q3 -= f3CK6csT - *cx;
*cx = f3CK6csT; } if (*cy > s_ClipY2)
return 0; if (*cy < s_ClipY1) { *h -= s_ClipY1 - *cy;
*bDefoJ6l += s_ClipY1 - *cy; *cy = s_ClipY1;
} if (*qm7J64q3 <= 0)
return 0; if (*cx + *qm7J64q3 > Kh97ARZU + 1)
{ *IsDSC069 += *cx + *qm7J64q3 - Kh97ARZU - 1;
*qm7J64q3 = Kh97ARZU + 1 - *cx; } if (*h <= 0)
return 0; if (*cy + *h > s_ClipY2 + 1) *h = s_ClipY2 - *cy + 1;
return 1; }
static void a9r8REYe(int LbPt5vw8, int qb0L1TPI, int TYFI6PC8)
{ int y; if (qb0L1TPI > TYFI6PC8)
{ y = TYFI6PC8; TYFI6PC8 = qb0L1TPI; qb0L1TPI = y;
} for (y = qb0L1TPI; y <= TYFI6PC8; y++)
cAQOtAsc(LbPt5vw8, y); }
void mil_DrawLineV(int LbPt5vw8, int qb0L1TPI, int TYFI6PC8)
{ int y; if (LbPt5vw8 < f3CK6csT || LbPt5vw8 > Kh97ARZU)
return; if (qb0L1TPI > TYFI6PC8)
{ y = TYFI6PC8; TYFI6PC8 = qb0L1TPI; qb0L1TPI = y;
} if (qb0L1TPI >= UwJNuSkK || TYFI6PC8 < 0)
return; if (qb0L1TPI < s_ClipY1)
qb0L1TPI = s_ClipY1; if (TYFI6PC8 > s_ClipY2)
TYFI6PC8 = s_ClipY2; for (y = qb0L1TPI; y <= TYFI6PC8; y++)
cAQOtAsc(LbPt5vw8, y); }
static void AI9TGVGd(int CYVLXJkW, int LINj8eGO, int y)
{ int LbPt5vw8; if (CYVLXJkW > LINj8eGO)
{ LbPt5vw8 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = LbPt5vw8; } for (LbPt5vw8 = CYVLXJkW; LbPt5vw8 <= LINj8eGO; LbPt5vw8++)
cAQOtAsc(LbPt5vw8, y); }
void mil_DrawLineH(int CYVLXJkW, int LINj8eGO, int y)
{ int LbPt5vw8; if (y < s_ClipY1 || y > s_ClipY2)
return; if (CYVLXJkW > LINj8eGO)
{ LbPt5vw8 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = LbPt5vw8; } if (CYVLXJkW >= h8iL9Zij || LINj8eGO < 0)
return; if (CYVLXJkW < f3CK6csT)
CYVLXJkW = f3CK6csT; if (LINj8eGO > Kh97ARZU)
LINj8eGO = Kh97ARZU; for (LbPt5vw8 = CYVLXJkW; LbPt5vw8 <= LINj8eGO; LbPt5vw8++)
cAQOtAsc(LbPt5vw8, y); }
static int ZgTiYWea(int* CYVLXJkW, int* qb0L1TPI, int* LINj8eGO, int* TYFI6PC8)
{ int OOzmyCcI, N3jRR8ZR; if (*CYVLXJkW > *LINj8eGO)
{ OOzmyCcI = *CYVLXJkW; N3jRR8ZR = *qb0L1TPI;
*CYVLXJkW = *LINj8eGO; *qb0L1TPI = *TYFI6PC8;
*LINj8eGO = OOzmyCcI; *TYFI6PC8 = N3jRR8ZR;
} if (*LINj8eGO < f3CK6csT || *CYVLXJkW > Kh97ARZU)
return 0; if (*CYVLXJkW < f3CK6csT)
{ *qb0L1TPI = ((*LINj8eGO - f3CK6csT)*(*TYFI6PC8 - *qb0L1TPI))/(*CYVLXJkW - *LINj8eGO) + *TYFI6PC8;
*CYVLXJkW = f3CK6csT; } if (*LINj8eGO > Kh97ARZU)
{ *TYFI6PC8 = ((*LINj8eGO - Kh97ARZU)*(*TYFI6PC8 - *qb0L1TPI))/(*CYVLXJkW - *LINj8eGO) + *TYFI6PC8;
*LINj8eGO = Kh97ARZU; } if (*qb0L1TPI > *TYFI6PC8)
{ OOzmyCcI = *CYVLXJkW; N3jRR8ZR = *qb0L1TPI;
*CYVLXJkW = *LINj8eGO; *qb0L1TPI = *TYFI6PC8;
*LINj8eGO = OOzmyCcI; *TYFI6PC8 = N3jRR8ZR;
} if (*TYFI6PC8 < s_ClipY1 || *qb0L1TPI > s_ClipY2)
return 0; if (*qb0L1TPI < s_ClipY1)
{ *CYVLXJkW = ((*TYFI6PC8 - s_ClipY1)*(*LINj8eGO - *CYVLXJkW))/(*qb0L1TPI - *TYFI6PC8) + *LINj8eGO;
*qb0L1TPI = s_ClipY1; } if (*TYFI6PC8 > s_ClipY2)
{ *LINj8eGO = ((*TYFI6PC8 - s_ClipY2)*(*LINj8eGO - *CYVLXJkW))/(*qb0L1TPI - *TYFI6PC8) + *LINj8eGO;
*TYFI6PC8 = s_ClipY2; } return 1; } 
#define aDdby7o9(n) (((n) >= 0) ? (n) : -(n))
void mil_DrawLine(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ int putx, puty; int endx, endy; int dx, dy;
int add; int p, step1, step2; if (!ZgTiYWea(&CYVLXJkW, &qb0L1TPI, &LINj8eGO, &TYFI6PC8))
return; if (CYVLXJkW == LINj8eGO) 
{ a9r8REYe(CYVLXJkW, qb0L1TPI, TYFI6PC8);
return; } if (qb0L1TPI == TYFI6PC8) 
{ AI9TGVGd(CYVLXJkW, LINj8eGO, qb0L1TPI);
return; } if (aDdby7o9(LINj8eGO - CYVLXJkW) > aDdby7o9(TYFI6PC8 - qb0L1TPI))
{  if (CYVLXJkW < LINj8eGO) {  putx = CYVLXJkW; puty = qb0L1TPI; 
endx = LINj8eGO; endy = TYFI6PC8;  } else
{ putx = LINj8eGO; puty = TYFI6PC8;  endx = CYVLXJkW; endy = qb0L1TPI; 
} dx = endx - putx; dy = endy - puty; add = (dy > 0) ? 1 : -1;
dy = aDdby7o9(dy); p = (dy << 1) - dx; 
step1 = dy << 1; 
step2 = (dy - dx) << 1; 
cAQOtAsc(putx, puty); while (putx < endx)
{ ++putx; if (p < 0) p += step1; else {  p += step2;
puty += add;  } cAQOtAsc(putx, puty); } }
else {  if (qb0L1TPI < TYFI6PC8) {  putx = CYVLXJkW; puty = qb0L1TPI; 
endx = LINj8eGO; endy = TYFI6PC8;  } else
{  putx = LINj8eGO; puty = TYFI6PC8;  endx = CYVLXJkW; endy = qb0L1TPI; 
} dx = endx - putx; dy = endy - puty; add = (dx > 0) ? 1 : -1;
dx = aDdby7o9(dx); p = (dx << 1) - dy; 
step1 = dx << 1; 
step2 = (dx - dy) << 1; 
cAQOtAsc(putx, puty); while (puty < endy)
{ ++puty; if (p < 0) p += step1; else { p += step2;
putx += add;  } cAQOtAsc(putx, puty); } }
}
static void NchisNIj(int cx, int cy, int uKVneQDx, int ZGAapTyX)
{ int LbPt5vw8, y; LbPt5vw8 = cx + uKVneQDx;
if (LbPt5vw8 >= f3CK6csT && LbPt5vw8 <= Kh97ARZU)
{ y = cy + ZGAapTyX; if (y >= s_ClipY1 && y <= s_ClipY2)
cAQOtAsc(LbPt5vw8, y); y = cy - ZGAapTyX;
if (y >= s_ClipY1 && y <= s_ClipY2) cAQOtAsc(LbPt5vw8, y);
} LbPt5vw8 = cx - uKVneQDx; if (LbPt5vw8 >= f3CK6csT && LbPt5vw8 <= Kh97ARZU)
{ y = cy + ZGAapTyX; if (y >= s_ClipY1 && y <= s_ClipY2)
cAQOtAsc(LbPt5vw8, y); y = cy - ZGAapTyX;
if (y >= s_ClipY1 && y <= s_ClipY2) cAQOtAsc(LbPt5vw8, y);
} }
void mil_DrawEllipse(int cx, int cy, int uKVneQDx, int ZGAapTyX)
{ int LbPt5vw8,y; int Ap0RoJwH,VbOQID1X; double obomtA20,I9bfzNUY;
if (uKVneQDx < 0) uKVneQDx = -uKVneQDx; if (ZGAapTyX < 0)
ZGAapTyX = -ZGAapTyX; LbPt5vw8 = 0; y = ZGAapTyX;
Ap0RoJwH = uKVneQDx*uKVneQDx; VbOQID1X = ZGAapTyX*ZGAapTyX;
obomtA20 = Ap0RoJwH - Ap0RoJwH*ZGAapTyX + (Ap0RoJwH >> 2); 
NchisNIj(cx, cy, LbPt5vw8, y); while (Ap0RoJwH*(y - 0.5) > VbOQID1X*(LbPt5vw8 + 1))
{ if (obomtA20 < 0) { obomtA20 += VbOQID1X*((LbPt5vw8 << 1) + 3); 
LbPt5vw8++; } else { obomtA20 += VbOQID1X*((LbPt5vw8 << 1) + 3) + Ap0RoJwH*(-2*y + 2); 
LbPt5vw8++; y--; } NchisNIj(cx, cy, LbPt5vw8, y);
} I9bfzNUY = VbOQID1X*(LbPt5vw8 + 0.5)*(LbPt5vw8 + 0.5) + Ap0RoJwH*(y - 1)*(y - 1) - Ap0RoJwH*VbOQID1X;
while (y > 0) { if (I9bfzNUY < 0) { I9bfzNUY += VbOQID1X*((LbPt5vw8 << 1) + 2) + Ap0RoJwH*(-2*y + 3); 
LbPt5vw8++; y--; } else { I9bfzNUY += Ap0RoJwH*(-2*y + 3);
y--; } NchisNIj(cx, cy, LbPt5vw8, y); } }
static void mYDvYy0a(int cx, int cy, int uKVneQDx, int ZGAapTyX)
{ int LbPt5vw8, CYVLXJkW, LINj8eGO, y; CYVLXJkW = cx - uKVneQDx;
LINj8eGO = cx + uKVneQDx; y = cy + ZGAapTyX;
if (y >= s_ClipY1 && y <= s_ClipY2)
{ if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
for (LbPt5vw8 = CYVLXJkW; LbPt5vw8 <= LINj8eGO; LbPt5vw8++)
cAQOtAsc(LbPt5vw8, y); } y = cy - ZGAapTyX;
if (y >= s_ClipY1 && y <= s_ClipY2)
{ if (CYVLXJkW < f3CK6csT) CYVLXJkW = f3CK6csT;
if (LINj8eGO > Kh97ARZU) LINj8eGO = Kh97ARZU;
for (LbPt5vw8 = CYVLXJkW; LbPt5vw8 <= LINj8eGO; LbPt5vw8++)
cAQOtAsc(LbPt5vw8, y); } }
void mil_FillEllipse(int cx, int cy, int uKVneQDx, int ZGAapTyX)
{ int LbPt5vw8,y; int Ap0RoJwH,VbOQID1X; double obomtA20,I9bfzNUY;
if (uKVneQDx < 0) uKVneQDx = -uKVneQDx; if (ZGAapTyX < 0)
ZGAapTyX = -ZGAapTyX; LbPt5vw8 = 0; y = ZGAapTyX;
Ap0RoJwH = uKVneQDx*uKVneQDx; VbOQID1X = ZGAapTyX*ZGAapTyX;
obomtA20 = Ap0RoJwH - Ap0RoJwH*ZGAapTyX + (Ap0RoJwH >> 2); 
mYDvYy0a(cx, cy, LbPt5vw8, y); while (Ap0RoJwH*(y - 0.5) > VbOQID1X*(LbPt5vw8 + 1))
{ if (obomtA20 < 0) { obomtA20 += VbOQID1X*((LbPt5vw8 << 1) + 3); 
LbPt5vw8++; } else { mYDvYy0a(cx, cy, LbPt5vw8, y);
obomtA20 += VbOQID1X*((LbPt5vw8 << 1) + 3) + Ap0RoJwH*(-2*y + 2); 
LbPt5vw8++; y--; } } mYDvYy0a(cx, cy, LbPt5vw8, y);
I9bfzNUY = VbOQID1X*(LbPt5vw8 + 0.5)*(LbPt5vw8 + 0.5) + Ap0RoJwH*(y - 1)*(y - 1) - Ap0RoJwH*VbOQID1X;
while (y > 0) { if (I9bfzNUY < 0) { I9bfzNUY += VbOQID1X*((LbPt5vw8 << 1) + 2) + Ap0RoJwH*(-2*y + 3); 
LbPt5vw8++; y--; } else { I9bfzNUY += Ap0RoJwH*(-2*y + 3);
y--; } mYDvYy0a(cx, cy, LbPt5vw8, y); } }
void mil_DrawRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ mil_DrawLineV(CYVLXJkW, qb0L1TPI, TYFI6PC8);
mil_DrawLineV(LINj8eGO, qb0L1TPI, TYFI6PC8);
mil_DrawLineH(CYVLXJkW, LINj8eGO, qb0L1TPI);
mil_DrawLineH(CYVLXJkW, LINj8eGO, TYFI6PC8);
}
void mil_FillRect(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ int lz9QnfXJ, QeUzFM7x, SOQlpwfb, udlZVnlD;
int LbPt5vw8, y; int mSdyqwa7; if (CYVLXJkW > LINj8eGO)
{ mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8)
{ mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } lz9QnfXJ = (CYVLXJkW >= f3CK6csT) ? CYVLXJkW : f3CK6csT;
SOQlpwfb = (LINj8eGO <= Kh97ARZU) ? LINj8eGO : Kh97ARZU;
QeUzFM7x = (qb0L1TPI >= s_ClipY1) ? qb0L1TPI : s_ClipY1;
udlZVnlD = (TYFI6PC8 <= s_ClipY2) ? TYFI6PC8 : s_ClipY2;
for (y = QeUzFM7x; y <= udlZVnlD; y++)
for (LbPt5vw8 = lz9QnfXJ; LbPt5vw8 <= SOQlpwfb; LbPt5vw8++)
cAQOtAsc(LbPt5vw8, y); }
void mil_SaveGamma()
{ mkfkxUSx = U6liRwBT; mil_SetGamma(3); }
void mil_RestoreGamma()
{ mil_SetGamma(mkfkxUSx); }
void mil_SaveLCD()
{ sW4PB08i(nmRH4auG, JZagjmLi, AvwqZ1W4);
} void mil_RestoreLCD() { sW4PB08i(JZagjmLi, nmRH4auG, AvwqZ1W4);
}
void mil_SetClip(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8)
{ int mSdyqwa7; if (CYVLXJkW > LINj8eGO)
{ mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8)
{ mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } f3CK6csT = (CYVLXJkW >= 0) ? CYVLXJkW : 0;
s_ClipY1 = (qb0L1TPI >= 0) ? qb0L1TPI : 0;
Kh97ARZU = (LINj8eGO < h8iL9Zij) ? LINj8eGO : h8iL9Zij - 1;
s_ClipY2 = (TYFI6PC8 < UwJNuSkK) ? TYFI6PC8 : UwJNuSkK - 1;
}
void mil_ResetClip()
{ f3CK6csT = 0; s_ClipY1 = 0; Kh97ARZU = h8iL9Zij - 1;
s_ClipY2 = UwJNuSkK - 1; }
static void mVOxnEmw(int vx, int vy)
{ if (vx >= f3CK6csT && vx <= Kh97ARZU && vy >= s_ClipY1 && vy <= s_ClipY2)
cAQOtAsc(vx, vy); }
void mil_DrawRectRound(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8, int r)
{ int dx; int dy; int s; int mSdyqwa7; if (CYVLXJkW > LINj8eGO)
{ mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8)
{ mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } if (r < 0) r = -r;
if ((LINj8eGO - CYVLXJkW) < (r << 1)) 
r = (LINj8eGO - CYVLXJkW) >> 1; 
if ((TYFI6PC8 - qb0L1TPI) < (r << 1)) 
r = (TYFI6PC8 - qb0L1TPI) >> 1; 
dx = r; s = r; dy = 0; mil_DrawLineV(CYVLXJkW, qb0L1TPI + r, TYFI6PC8 - r);
mil_DrawLineV(LINj8eGO, qb0L1TPI + r, TYFI6PC8 - r);
mil_DrawLineH(CYVLXJkW + r, LINj8eGO - r, qb0L1TPI);
mil_DrawLineH(CYVLXJkW + r, LINj8eGO - r, TYFI6PC8);
while(dx >= dy) { mVOxnEmw(LINj8eGO - r + dx, TYFI6PC8 - r + dy); 
mVOxnEmw(LINj8eGO - r + dy, TYFI6PC8 - r + dx); 
mVOxnEmw(LINj8eGO - r + dx, qb0L1TPI + r - dy); 
mVOxnEmw(LINj8eGO - r + dy, qb0L1TPI + r - dx); 
mVOxnEmw(CYVLXJkW + r - dx, TYFI6PC8 - r + dy); 
mVOxnEmw(CYVLXJkW + r - dy, TYFI6PC8 - r + dx); 
mVOxnEmw(CYVLXJkW + r - dx, qb0L1TPI + r - dy); 
mVOxnEmw(CYVLXJkW + r - dy, qb0L1TPI + r - dx); 
s -= (dy << 1) + 1; 
if (s < 0) { s += (dx - 1) << 1; 
dx--; } dy++; } }
void mil_FillRectRound(int CYVLXJkW, int qb0L1TPI, int LINj8eGO, int TYFI6PC8, int r)
{ int dx, dy, s; int mSdyqwa7; if (CYVLXJkW > LINj8eGO)
{ mSdyqwa7 = LINj8eGO; LINj8eGO = CYVLXJkW;
CYVLXJkW = mSdyqwa7; } if (qb0L1TPI > TYFI6PC8)
{ mSdyqwa7 = TYFI6PC8; TYFI6PC8 = qb0L1TPI;
qb0L1TPI = mSdyqwa7; } if (r < 0) r = -r;
if ((LINj8eGO - CYVLXJkW) < (r << 1)) 
r = (LINj8eGO - CYVLXJkW) >> 1; 
if ((TYFI6PC8 - qb0L1TPI) < (r << 1)) 
r = (TYFI6PC8 - qb0L1TPI) >> 1; 
dx = r; s = r; dy = 0; while(dx >= dy) { mil_DrawLineH(CYVLXJkW + r - dy, LINj8eGO - r + dy, qb0L1TPI + r - dx);
mil_DrawLineH(CYVLXJkW + r - dx, LINj8eGO - r + dx, qb0L1TPI + r - dy);
mil_DrawLineH(CYVLXJkW + r - dy, LINj8eGO - r + dy, TYFI6PC8 - r + dx);
mil_DrawLineH(CYVLXJkW + r - dx, LINj8eGO - r + dx, TYFI6PC8 - r + dy);
s -= (dy << 1) + 1; 
if (s < 0) { s += (dx - 1) << 1; 
dx--; } dy++; } mil_FillRect(CYVLXJkW, qb0L1TPI + r + 1, LINj8eGO, TYFI6PC8 - r - 1);
}

#include "mil_bw.cpp"

#include "mil_gray.cpp"

#include "mil_cr16.cpp"

#include "mil_cr256.cpp"

#include "mil_cr4096.cpp"

#include "mil_cr64k.cpp"
