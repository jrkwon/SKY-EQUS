//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY
//
//	FILE NAME
//		SVM.cpp
//
//	DESCRIPTION
//		SWAP Virtual Machine Library
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
//		2001.10.17	Version 1.55.01 Release
//
//==============================================================================

#include <stdio.h>

#include <ctype.h>

#include <string.h>

#include "SVM.h"
char svMIN[SWAP_LEN_MIN + 1]; 
SSBInfo svSSBInfo; 
int svSSBIndex; 
BYTE svMyUserID; 
char svRsvdChat[9][SWAP_RSVD_CHAT_SIZE + 1]; 
BYTE svRunScriptType; 
DWORD svScriptInfo[SWAP_SSB_SIZE]; 
int swLcdWidth; 
int swLcdHeight; 
S_Result svResult; 
int svResultReason; 
char svResultString[SWAP_LEN_URL + 1]; 

#if (SWAP_LCD_TYPE > SWAP_LCD_WORDCOLOR_START)
WORD *swLcdBuffer; 
WORD *swBackLcdBuffer; 

#else
BYTE *swLcdBuffer; 
BYTE *swBackLcdBuffer; 

#endif
BYTE *swScriptBuffer; 
BYTE *swWorkSpace; 
INT2 *swRcvBuf; 
INT2 *swRsRcvBuf; 
static INT2 e7mBDeVX[SWAP_STACK_SIZE]; 
static int Y8rugKNJ; 
static BYTE *BT9PYMlQ; 
static BYTE *bCO0bM8w[SWAP_PC_STACK_SIZE]; 
static int lhoxsk7M; 
static int GdG8mwdy; 
static S_HeaderType *nSub0nar; 
static int CQHSxkW0; 
static int jzpVRmy0; 
static int mSAJ4wdn; 
static S_RamSymElem *HAvJVeND; 
static INT2 *FZRf2WUX; 
static INT2 *M7oSEwdG; 
static INT2 *bmE4Gwyt; 
static INT2 *mRc49kNz; 
static int JG14eUey; 
static int f0SXh3qy; 
static int tfNSCqil; 
S_RamMediaElem *swRamMediaTable; 
static BYTE *QU4IcLFB; 
static BYTE *WrSRS9NB; 
static BYTE *u4T8Qmc6; 
static BYTE *YTwPrBKb; 
static INT2 *eLbFc93O; 
static INT2 *p0bvn9mb; 
static INT2 *w1ZW3mTZ; 
static INT2 *zwjAPDYU; 
static INT2 *mMDSsdVr; 
static INT2 *I2bmJWPw; 
static INT2 *FmJKOy5y; 
static BYTE Y9HeE4kB; 
static BYTE y8sIUNuW; 
static BYTE ZtFhCFq7; 
static BYTE VrJqCx8U; 
static INT2 DFbjS7T4[3]; 
static INT2 EOeyaIK2[3]; 
static INT2 y5TvOyEz; 
static BYTE sW4PB08i; 
BYTE swDispMode; 
static char CKOk9Z4Q[SWAP_LEN_CHAT][SWAP_LEN_CHAT_TEXT + 1]; 
enum _S_Reserved { V_DATA, V_WIDTH, V_HEIGHT,
V_FRAME, V_FRAME2, V_FRAME3, V_FRAME6, V_RCVBUF,
V_RSRCVBUF, V_RESERVED09, V_RESERVED10, V_RESERVED11,
V_RESERVED12, V_RESERVED13, V_RESERVED14,
V_RESERVED15, V_SIZE };
const static BYTE GICEruWy = 0xFF;
void svm_ResetAllTimers() { DFbjS7T4[0] = 0;
midd_StopTimer(0); DFbjS7T4[1] = 0; midd_StopTimer(1);
DFbjS7T4[2] = 0; midd_StopTimer(2); } static void JZagjmLi()
{ midd_StopTimer(0); midd_StopTimer(1); midd_StopTimer(2);
} static void nmRH4auG() { if (DFbjS7T4[0] > 0)
midd_StartTimer(0, DFbjS7T4[0], EOeyaIK2[0]);
if (DFbjS7T4[1] > 0) midd_StartTimer(1, DFbjS7T4[1], EOeyaIK2[1]);
if (DFbjS7T4[2] > 0) midd_StartTimer(2, DFbjS7T4[2], EOeyaIK2[2]);
} void s_Exception(int code) { svm_ResetAllTimers();
midd_StopSound(); mil_ClearWhite(); svResult.code = SWAP_RC_EXCEPTION;
svResult.param.exception.ecode = code; if (BT9PYMlQ != NULL) {
svResult.param.exception.addr = BT9PYMlQ - swScriptBuffer - 1;
svResult.param.exception.pc = *(BT9PYMlQ - 1);
} else { svResult.param.exception.addr = 0;
svResult.param.exception.pc = 0; } BT9PYMlQ = (BYTE*)&GICEruWy; 
}
S_RamMediaElem *s_InvalidMedia()
{ s_Exception(SWAP_ECPT_INVALID_MDA_INDEX);
return &swRamMediaTable[0]; }

#define uW9Kdys6 0 

#define Tsbk5B54 1 

#define h8iL9Zij 0x4000 

#define UwJNuSkK 0xBFFF 

#define b5Lr0cwi sizeof(KPPUdUrc)/sizeof(KPPUdUrc[0])

#define f3CK6csT() e7mBDeVX[Y8rugKNJ]

#define Kh97ARZU() e7mBDeVX[Y8rugKNJ - 1]

#define U6liRwBT() e7mBDeVX[Y8rugKNJ - 2]

#define mkfkxUSx() e7mBDeVX[Y8rugKNJ - 3]

#define NP3y8tCu() e7mBDeVX[Y8rugKNJ - 4]

#define AvwqZ1W4() e7mBDeVX[Y8rugKNJ - 5]

#define cAQOtAsc() e7mBDeVX[Y8rugKNJ - 6]

#define lJdEwKs7() e7mBDeVX[Y8rugKNJ - 7]

#define XW9UZcmx(top) if ((top) >= SWAP_STACK_SIZE) { s_Exception(SWAP_ECPT_STACK_OVER); return; }

#define ufOcQ3Xs(A, c) if ((c) < 0 || (c) >= HAvJVeND[A].dim) { s_Exception(SWAP_ECPT_OUT_OF_DIM); return; }

#define CLVY98Pr(H0Ng1GoT) if ((H0Ng1GoT) < 0 || (H0Ng1GoT) >= CQHSxkW0) { s_Exception(SWAP_ECPT_INVALID_SYM_INDEX); return; }

#define d6gJLmZB(p) if (!(FZRf2WUX <= (p) && (p) < M7oSEwdG) && !(bmE4Gwyt <= (p) && (p) < mRc49kNz)) { s_Exception(SWAP_ECPT_INVALID_SYM_ADDR); return; }

#define EmBm3cNq(p, A, c) { CLVY98Pr(A); ufOcQ3Xs(A, c); (p) = HAvJVeND[A].addr + (c); d6gJLmZB(p); }

#define iKnsjF6q(p, A) { CLVY98Pr(A); (p) = HAvJVeND[A].addr; d6gJLmZB(p); }

#define jPiNmNu7(H0Ng1GoT) if ((H0Ng1GoT) < 0 || (H0Ng1GoT) >= JG14eUey) { s_Exception(SWAP_ECPT_INVALID_MDA_INDEX); return; }

#define r63fdurc(p) if (!(QU4IcLFB <= (p) && (p) < WrSRS9NB) && !(u4T8Qmc6 <= (p) && (p) < YTwPrBKb)) { s_Exception(SWAP_ECPT_INVALID_MDA_ADDR); return; }

#define USCUWVfg(p, A) { jPiNmNu7(A); (p) = swRamMediaTable[A].addr; r63fdurc(p); }
static INT2* gz7Da4KB(INT2 offset)
{ register INT2 offset2; register INT2 *Ydsi5ovo;
if (offset&h8iL9Zij) { offset2 = offset&UwJNuSkK;
if (offset2 < 0 || jzpVRmy0 <= offset2) {
s_Exception(SWAP_ECPT_OUT_OF_DIM); return NULL;
} Ydsi5ovo = &FZRf2WUX[offset2]; } else {
if (offset < 0 || mSAJ4wdn <= offset) { s_Exception(SWAP_ECPT_OUT_OF_DIM);
return NULL; } Ydsi5ovo = &bmE4Gwyt[offset];
} return Ydsi5ovo; } 
#define IcCNiiQp(pc) (INT1)(*(pc))

#define JdHT2n1Q(pc) (WORD)(((*(pc)) << 8) + *((pc) + 1))

#define v9V2Pnjk(pc) (WORD)(((*((pc) + 1)) << 8) + *((pc) + 2))

#define a9r8REYe(H0Ng1GoT) ((H0Ng1GoT < JG14eUey) ? &swRamMediaTable[H0Ng1GoT] : s_InvalidMedia())

#define AI9TGVGd(H0Ng1GoT) (a9r8REYe(H0Ng1GoT)->addr)

#define ZgTiYWea(H0Ng1GoT) (a9r8REYe(H0Ng1GoT)->size)
static void aDdby7o9(void) {} static void NchisNIj(void) {}
static void mYDvYy0a(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ;
XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, B, c); EmBm3cNq(lz9QnfXJ, A, *p);
e7mBDeVX[++Y8rugKNJ] = *lz9QnfXJ; } static void mVOxnEmw(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ; XW9UZcmx(Y8rugKNJ);
iKnsjF6q(p, B); EmBm3cNq(lz9QnfXJ, A, *p);
e7mBDeVX[++Y8rugKNJ] = *lz9QnfXJ; } static void b8heDkBn(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 *p; XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, A, c);
e7mBDeVX[++Y8rugKNJ] = *p; } static void Fiyr9w3x(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 *p;
XW9UZcmx(Y8rugKNJ); iKnsjF6q(p, A); e7mBDeVX[++Y8rugKNJ] = *p;
} static void UsjwID4x(void) 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = (INT1)*BT9PYMlQ++;
} static void EEF7iunr(void) 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = (INT2)((*BT9PYMlQ << 8) + *(BT9PYMlQ + 1));
BT9PYMlQ += 2; } static void Xgnuagnt(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ;
XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, B, c); EmBm3cNq(lz9QnfXJ, A, *p);
*lz9QnfXJ = e7mBDeVX[Y8rugKNJ--]; } static void OjejimrN(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ; XW9UZcmx(Y8rugKNJ);
iKnsjF6q(p, B); EmBm3cNq(lz9QnfXJ, A, *p);
*lz9QnfXJ = e7mBDeVX[Y8rugKNJ--]; } static void xWYbAHVO(void) 
{  register INT2 A = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 *p; XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, A, c);
*p = e7mBDeVX[Y8rugKNJ--]; } static void Ge9ypBEE(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 *p;
XW9UZcmx(Y8rugKNJ); iKnsjF6q(p, A); *p = e7mBDeVX[Y8rugKNJ--];
} static void GsgfrHnu(void) { GdG8mwdy = Y8rugKNJ; }
static void ZYj9qvyb(void) { Y8rugKNJ = GdG8mwdy; }
static void B0oOslTf(void) { f3CK6csT()++; }
static void SPvM1Gb8(void) { f3CK6csT()--; }
static void JZLJSz2M(void) { f3CK6csT() = -f3CK6csT(); }
static void EOI4B9jZ(void) { XW9UZcmx(Y8rugKNJ); Y8rugKNJ++; f3CK6csT() = Kh97ARZU(); }
static void oEnAig1J(void) { register INT2 mSdyqwa7 = f3CK6csT(); f3CK6csT() = Kh97ARZU(); Kh97ARZU() = mSdyqwa7; }
static void TmtaAsPv(void) { Kh97ARZU() += f3CK6csT(); Y8rugKNJ--; }
static void Cw9bZk4k(void) { Kh97ARZU() -= f3CK6csT(); Y8rugKNJ--; }
static void FJweE8VK(void) { Kh97ARZU() *= f3CK6csT(); Y8rugKNJ--; }
static void GBecIdEn(void)  { if (f3CK6csT() != 0)
Kh97ARZU() /= f3CK6csT(); else s_Exception(SWAP_ECPT_DIV_ZERO);
Y8rugKNJ--;  } static void dFi5bO99(void)
{ if (f3CK6csT() != 0) Kh97ARZU() %= f3CK6csT();
else s_Exception(SWAP_ECPT_MOD_ZERO); Y8rugKNJ--;
} static void boNcuwqS(void) { Kh97ARZU() &= f3CK6csT(); Y8rugKNJ--; }
static void apHGYOU7(void) { Kh97ARZU() |= f3CK6csT(); Y8rugKNJ--; }
static void jYPKuxnw(void) { f3CK6csT() = !f3CK6csT(); }
static void kkNN5Ev6(void) { Kh97ARZU() ^= f3CK6csT(); Y8rugKNJ--; }
static void iW0pdjQP(void) { Kh97ARZU() >>= f3CK6csT(); Y8rugKNJ--; }
static void pohX7Er3(void) { Kh97ARZU() <<= f3CK6csT(); Y8rugKNJ--; }
static void jGtO1FB9(void) { Kh97ARZU() = Kh97ARZU() > f3CK6csT(); Y8rugKNJ--; }
static void IOd5DYzC(void) { Kh97ARZU() = Kh97ARZU() < f3CK6csT(); Y8rugKNJ--; }
static void VJ9k9RQN(void) { Kh97ARZU() = Kh97ARZU() >= f3CK6csT(); Y8rugKNJ--; }
static void M1P1NHy9(void) { Kh97ARZU() = Kh97ARZU() <= f3CK6csT(); Y8rugKNJ--; }
static void SZysP4qw(void) { Kh97ARZU() = Kh97ARZU() == f3CK6csT(); Y8rugKNJ--; }
static void inTEQRbH(void) { Kh97ARZU() = Kh97ARZU() != f3CK6csT(); Y8rugKNJ--; }
static void PaCZSuFD(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c1 = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 D = *BT9PYMlQ++; register INT2 c2 = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; EmBm3cNq(p, B, c1);
EmBm3cNq(lz9QnfXJ, A, *p); EmBm3cNq(p, D, c2);
EmBm3cNq(SOQlpwfb, C, *p); *lz9QnfXJ = *SOQlpwfb;
} static void RwVPjzxH(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 D = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
EmBm3cNq(p, B, c); EmBm3cNq(lz9QnfXJ, A, *p);
iKnsjF6q(p, D); EmBm3cNq(SOQlpwfb, C, *p);
*lz9QnfXJ = *SOQlpwfb; } static void wXY67Rf3(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c1 = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 c2 = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
EmBm3cNq(p, B, c1); EmBm3cNq(lz9QnfXJ, A, *p);
EmBm3cNq(SOQlpwfb, C, c2); *lz9QnfXJ = *SOQlpwfb;
} static void s7C8SZZs(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; EmBm3cNq(p, B, c);
EmBm3cNq(lz9QnfXJ, A, *p); iKnsjF6q(SOQlpwfb, C);
*lz9QnfXJ = *SOQlpwfb; } static void k0MQayom(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c1 = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ;
XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, B, c1); EmBm3cNq(lz9QnfXJ, A, *p);
*lz9QnfXJ = (INT1)*BT9PYMlQ++; } static void HRCKFYTf(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 D = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
iKnsjF6q(p, B); EmBm3cNq(lz9QnfXJ, A, *p);
EmBm3cNq(p, D, c); EmBm3cNq(SOQlpwfb, C, *p);
*lz9QnfXJ = *SOQlpwfb; } static void O15IeEKn(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 D = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; iKnsjF6q(p, B);
EmBm3cNq(lz9QnfXJ, A, *p); iKnsjF6q(p, D);
EmBm3cNq(SOQlpwfb, C, *p); *lz9QnfXJ = *SOQlpwfb;
} static void fthEy4WO(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; iKnsjF6q(p, B);
EmBm3cNq(lz9QnfXJ, A, *p); EmBm3cNq(SOQlpwfb, C, c);
*lz9QnfXJ = *SOQlpwfb; } static void XPxhpuKc(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
iKnsjF6q(p, B); EmBm3cNq(lz9QnfXJ, A, *p);
iKnsjF6q(SOQlpwfb, C); *lz9QnfXJ = *SOQlpwfb;
} static void eTx6s9W0(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ; iKnsjF6q(p, B);
EmBm3cNq(lz9QnfXJ, A, *p); *lz9QnfXJ = (INT1)*BT9PYMlQ++;
} static void CpPc5HRj(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c1 = *BT9PYMlQ++;
register INT2 B = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 c2 = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
EmBm3cNq(lz9QnfXJ, A, c1); EmBm3cNq(p, C, c2);
EmBm3cNq(SOQlpwfb, B, *p); *lz9QnfXJ = *SOQlpwfb;
} static void EccKMVci(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 B = *BT9PYMlQ++; register INT2 C = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; EmBm3cNq(lz9QnfXJ, A, c);
iKnsjF6q(p, C); EmBm3cNq(SOQlpwfb, B, *p);
*lz9QnfXJ = *SOQlpwfb; } static void W1lcDjm6(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c1 = *BT9PYMlQ++;
register INT2 B = *BT9PYMlQ++; register INT2 c2 = *BT9PYMlQ++;
register INT2 *lz9QnfXJ, *SOQlpwfb; EmBm3cNq(lz9QnfXJ, A, c1);
EmBm3cNq(SOQlpwfb, B, c2); *lz9QnfXJ = *SOQlpwfb;
} static void mhLcYuKT(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 B = *BT9PYMlQ++; register INT2 *lz9QnfXJ, *SOQlpwfb;
EmBm3cNq(lz9QnfXJ, A, c); iKnsjF6q(SOQlpwfb, B);
*lz9QnfXJ = *SOQlpwfb; } static void Fb7QjkfV(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c1 = *BT9PYMlQ++;
register INT2 *lz9QnfXJ; EmBm3cNq(lz9QnfXJ, A, c1);
*lz9QnfXJ = (INT1)*BT9PYMlQ++; } static void dSet7rec(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ, *SOQlpwfb; iKnsjF6q(lz9QnfXJ, A);
EmBm3cNq(p, C, c); EmBm3cNq(SOQlpwfb, B, *p);
*lz9QnfXJ = *SOQlpwfb; } static void YN5px9wH(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 C = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ, *SOQlpwfb;
iKnsjF6q(lz9QnfXJ, A); iKnsjF6q(p, C); EmBm3cNq(SOQlpwfb, B, *p);
*lz9QnfXJ = *SOQlpwfb; } static void HzrKAV9w(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *lz9QnfXJ, *SOQlpwfb;
iKnsjF6q(lz9QnfXJ, A); EmBm3cNq(SOQlpwfb, B, c);
*lz9QnfXJ = *SOQlpwfb; } static void weUuaSVW(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *lz9QnfXJ, *SOQlpwfb; iKnsjF6q(lz9QnfXJ, A);
iKnsjF6q(SOQlpwfb, B); *lz9QnfXJ = *SOQlpwfb;
} static void UVOMl6I0(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 *lz9QnfXJ;
iKnsjF6q(lz9QnfXJ, A); *lz9QnfXJ = (INT1)*BT9PYMlQ++;
} static void JhgEoEOk(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c1 = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ;
EmBm3cNq(p, B, c1); EmBm3cNq(lz9QnfXJ, A, *p);
*lz9QnfXJ += (INT1)*BT9PYMlQ++; } static void JxdvQVIg(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ; iKnsjF6q(p, B);
EmBm3cNq(lz9QnfXJ, A, *p); *lz9QnfXJ += (INT1)*BT9PYMlQ++;
} static void iiaR1Qyh(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c1 = *BT9PYMlQ++;
register INT2 *lz9QnfXJ; EmBm3cNq(lz9QnfXJ, A, c1);
*lz9QnfXJ += (INT1)*BT9PYMlQ++; } static void H5h5hXcT(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 *lz9QnfXJ;
iKnsjF6q(lz9QnfXJ, A); *lz9QnfXJ += (INT1)*BT9PYMlQ++;
} static void lhNFI5Ex(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *p, *lz9QnfXJ;
EmBm3cNq(p, B, c); EmBm3cNq(lz9QnfXJ, A, *p);
*lz9QnfXJ = f3CK6csT(); } static void kSeyq5l8(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 B = *BT9PYMlQ++;
register INT2 *p, *lz9QnfXJ; iKnsjF6q(p, B);
EmBm3cNq(lz9QnfXJ, A, *p); *lz9QnfXJ = f3CK6csT();
} static void uoQaTMb8(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 c = *BT9PYMlQ++;
register INT2 *lz9QnfXJ; EmBm3cNq(lz9QnfXJ, A, c);
*lz9QnfXJ = f3CK6csT(); } static void EW6LVBlK(void) 
{ register INT2 A = *BT9PYMlQ++; register INT2 *lz9QnfXJ;
iKnsjF6q(lz9QnfXJ, A); *lz9QnfXJ = f3CK6csT();
} static void tyJDygB4(void) 
{  register S_RamSymElem *sym; register INT2 A = *BT9PYMlQ++;
register INT2 c = *BT9PYMlQ++; register INT2 *p;
XW9UZcmx(Y8rugKNJ); EmBm3cNq(p, A, c); sym = &HAvJVeND[A];
if (sym->var == uW9Kdys6) e7mBDeVX[++Y8rugKNJ] = (p - FZRf2WUX) | h8iL9Zij;
else e7mBDeVX[++Y8rugKNJ] = p - bmE4Gwyt;
} static void k2ax889Q(void) 
{  register S_RamSymElem *sym; register INT2 A = *BT9PYMlQ++;
register INT2 *p; XW9UZcmx(Y8rugKNJ); iKnsjF6q(p, A);
sym = &HAvJVeND[A]; if (sym->var == uW9Kdys6)
e7mBDeVX[++Y8rugKNJ] = (p - FZRf2WUX) | h8iL9Zij;
else e7mBDeVX[++Y8rugKNJ] = p - bmE4Gwyt;
} static void OWsyYiyk(void) 
{  register UINT2 offset = f3CK6csT(); register INT2 *addr = gz7Da4KB(offset);
if (addr == NULL) return; f3CK6csT() = *addr;
} static void qlGFWhKG(void) 
{  bmE4Gwyt[Kh97ARZU()] = f3CK6csT();  Y8rugKNJ -= 2; 
} static void b80baDCo(void) 
{  bmE4Gwyt[Kh97ARZU()] = f3CK6csT();  Kh97ARZU() = f3CK6csT(); 
Y8rugKNJ--;  } static void JRIvsd2n(void)
{ if (e7mBDeVX[Y8rugKNJ--] > IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void BCu736FU(void)
{ if (e7mBDeVX[Y8rugKNJ--] < IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void k3ux4fDW(void)
{ if (e7mBDeVX[Y8rugKNJ--] >= IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void YT8oq9M8(void)
{ if (e7mBDeVX[Y8rugKNJ--] <= IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void YX6WZJ9k(void)
{ if (e7mBDeVX[Y8rugKNJ--] == IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void uXtW6Xl8(void)
{ if (e7mBDeVX[Y8rugKNJ--] != IcCNiiQp(BT9PYMlQ))
BT9PYMlQ = swScriptBuffer + v9V2Pnjk(BT9PYMlQ);
else BT9PYMlQ += 3; } static void I8XDgmy2(void)
{ BT9PYMlQ = swScriptBuffer + JdHT2n1Q(BT9PYMlQ);
} static void M0dimU4G(void) { if (f3CK6csT())
BT9PYMlQ = swScriptBuffer + JdHT2n1Q(BT9PYMlQ);
else BT9PYMlQ += 2; Y8rugKNJ--; } static void HjvlX3r5(void)
{ if (f3CK6csT() == 0) BT9PYMlQ = swScriptBuffer + JdHT2n1Q(BT9PYMlQ);
else BT9PYMlQ += 2; Y8rugKNJ--; } static void PlsvbI8t(void)
{ if (lhoxsk7M == SWAP_PC_STACK_SIZE) { s_Exception(SWAP_ECPT_PCSTACK_OVER);
return; } bCO0bM8w[++lhoxsk7M] = BT9PYMlQ + 2; 
BT9PYMlQ = swScriptBuffer + JdHT2n1Q(BT9PYMlQ);
} static void tWA94Aka(void) { if (lhoxsk7M > -1)
BT9PYMlQ = bCO0bM8w[lhoxsk7M--]; else  BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void wwIRCwE5(void)  { s_Exception(SWAP_ECPT_STOP_OP);
}
static void reI5L8za() {
register INT2 *Ydsi5ovo = gz7Da4KB(f3CK6csT());
Y8rugKNJ--; *Ydsi5ovo = SWAP_LCD_CLASS; *(Ydsi5ovo + 1) = SWAP_COLOR_TYPE;
*(Ydsi5ovo + 2) = SWAP_AUDIO_TYPE; *(Ydsi5ovo + 3) = SWAP_CARRIER_TYPE;
} static void nsZP3ml5() 
{ register INT2 a7P6Xwbo = f3CK6csT(); if (svm_SetMediaSize(a7P6Xwbo, strlen(svMIN) + 1))
strcpy((char*)AI9TGVGd(a7P6Xwbo), svMIN);
Y8rugKNJ--; } static void bdnHdpxf() 
{ register INT2 a7P6Xwbo = f3CK6csT(); if (svm_SetMediaSize(a7P6Xwbo, strlen(svSSBInfo.UserID) + 1))
strcpy((char*)AI9TGVGd(a7P6Xwbo), svSSBInfo.UserID);
f3CK6csT() = svMyUserID; } static void KjJA0KYA() 
{ register INT2 *Ydsi5ovo = gz7Da4KB(f3CK6csT());
if (Ydsi5ovo == NULL) return; Y8rugKNJ--;
midd_GetLocInfo((WORD*)Ydsi5ovo); }
static void ZOq5qzwQ() 
{ mil_Clear(f3CK6csT()%S_MAX_COLOR); Y8rugKNJ--;
} static void yN7fykzl() 
{ mil_PutPixel(U6liRwBT(), Kh97ARZU(), f3CK6csT()%S_MAX_COLOR);
Y8rugKNJ -= 3; } static void ysPpvW0m() 
{ register INT2 g = f3CK6csT(); if (g < 0)
g = 0; else if (g > 6) g = 6; mil_SetGamma(g);
Y8rugKNJ--; }
static void wxQOsShb() 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = S_WHITE;
} static void H1sBAY04() 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = S_BLACK;
} static void oJWdHhFi() 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = S_TRANSPARENT;
} static void i3tjYRQj() 
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = 5;
}
static void xc9U5L3x() 
{ int LG0dStpd = (BYTE)e7mBDeVX[Y8rugKNJ--]%S_MAX_COLOR;
mil_SetColor(LG0dStpd); } static void v9AXzz0Z() 
{ mil_DrawLine(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void JqAuamKe() 
{ mil_DrawLineH(U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 3; } static void fcQTGqqD() 
{ mil_DrawLineV(U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 3; } static void caJazpv1() 
{ mil_DrawRect(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void YNzmk8Mt() 
{ mil_FillRect(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void aoCoeMc6() 
{ mil_DrawEllipse(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void dpv7Xqnw() 
{ mil_FillEllipse(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; }
static void n4emhOcH() 
{ Y9HeE4kB = (BYTE)mkfkxUSx()%S_MAX_FONT;
y8sIUNuW = (BYTE)U6liRwBT()%S_MAX_COLOR; ZtFhCFq7 = (BYTE)Kh97ARZU()%S_MAX_COLOR;
VrJqCx8U = (BYTE)f3CK6csT()%S_MAX_ALIGN; Y8rugKNJ -= 4;
} static void JTgvU2RB() 
{ Y9HeE4kB = (BYTE)e7mBDeVX[Y8rugKNJ--]%S_MAX_FONT;
} static void Btd8PqaC() 
{ y8sIUNuW = (BYTE)Kh97ARZU()%S_MAX_COLOR;
ZtFhCFq7 = (BYTE)f3CK6csT()%S_MAX_COLOR; Y8rugKNJ -= 2;
} static void Jy2VajCp() 
{ VrJqCx8U = (BYTE)e7mBDeVX[Y8rugKNJ--]%S_MAX_ALIGN;
} static void RVhGK7nm() 
{ register INT2 a7P6Xwbo = f3CK6csT(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_DrawString(U6liRwBT(), Kh97ARZU(), (char*)p, Y9HeE4kB, y8sIUNuW, VrJqCx8U);
Y8rugKNJ -= 3; } static void wdYZBfxC() 
{ register INT2 a7P6Xwbo = f3CK6csT(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_DrawStringSolid(U6liRwBT(), Kh97ARZU(), (char*)p, Y9HeE4kB, y8sIUNuW, ZtFhCFq7, VrJqCx8U);
Y8rugKNJ -= 3; } static void bjupxEy9() 
{ register INT2 a7P6Xwbo = f3CK6csT(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_DrawString(U6liRwBT(), Kh97ARZU(), (char*)p, S_FONT_LARGE, y8sIUNuW, VrJqCx8U);
Y8rugKNJ -= 3; } static void Zi0BnxOT() 
{ register INT2 a7P6Xwbo = f3CK6csT(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_DrawStringSolid(U6liRwBT(), Kh97ARZU(), (char*)p, S_FONT_LARGE, y8sIUNuW, ZtFhCFq7, VrJqCx8U);
Y8rugKNJ -= 3; }
static void CzyMA07Z() 
{ register BYTE *Jv2UAbfL = (BYTE*)gz7Da4KB(Kh97ARZU());
register INT2 *WDqujLu9 = (INT2*)gz7Da4KB(f3CK6csT());
register INT2 type; register int H0Ng1GoT;
if (Jv2UAbfL == NULL || WDqujLu9 == NULL)
return; type = *WDqujLu9++; switch (type)
{ case S_IMG_GRAY1: 
*Jv2UAbfL = (*WDqujLu9++ & 0x0F) << 4; *Jv2UAbfL |= *WDqujLu9++ & 0x0F;
break; case S_IMG_GRAY2: 
*Jv2UAbfL = (*WDqujLu9++ & 0x0F) << 4; *Jv2UAbfL |= *WDqujLu9++ & 0x0F;
Jv2UAbfL++; *Jv2UAbfL = (*WDqujLu9++ & 0x0F) << 4;
*Jv2UAbfL |= *WDqujLu9++ & 0x0F; break; case S_IMG_GRAY4: 
for (H0Ng1GoT = 0; H0Ng1GoT < 8; H0Ng1GoT++) {
*Jv2UAbfL = (*WDqujLu9++ & 0x0F) << 4; *Jv2UAbfL |= *WDqujLu9++ & 0x0F;
Jv2UAbfL++; } break; } 
#ifdef SWAP_LCD_COLOR
switch (type) { case S_IMG_COLOR1: 
*Jv2UAbfL++ = *WDqujLu9++ & 0xFF; *Jv2UAbfL++ = *WDqujLu9++ & 0xFF;
break; case S_IMG_COLOR2: 
for (H0Ng1GoT = 0; H0Ng1GoT < 4; H0Ng1GoT++)
*Jv2UAbfL++ = *WDqujLu9++ & 0xFF; break; case S_IMG_COLOR4: 
for (H0Ng1GoT = 0; H0Ng1GoT < 16; H0Ng1GoT++)
*Jv2UAbfL++ = *WDqujLu9++ & 0xFF; break; case S_IMG_COLOR8: 
break; } 
#endif 
Y8rugKNJ -= 2; } static void AQ4qIfP0() 
{ register INT2 a7P6Xwbo = f3CK6csT(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_CopyImage(p, U6liRwBT(), Kh97ARZU());
Y8rugKNJ -= 3; } static void hnClyY4g() 
{ register INT2 a7P6Xwbo = Kh97ARZU(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); mil_CopyImageDir(p, mkfkxUSx(), U6liRwBT(), f3CK6csT());
Y8rugKNJ -= 4; } static void O811fHpi() 
{ register INT2 a7P6Xwbo = Kh97ARZU(); register BYTE* p;
register BYTE* addr; USCUWVfg(p, a7P6Xwbo);
addr = (BYTE*)gz7Da4KB(f3CK6csT()); if (addr == NULL)
return; mil_CopyImagePal(p, mkfkxUSx(), U6liRwBT(), addr);
Y8rugKNJ -= 4; } static void Z8Gi7fdg() 
{ register INT2 a7P6Xwbo = U6liRwBT(); register BYTE* p;
register BYTE* addr; USCUWVfg(p, a7P6Xwbo);
addr = (BYTE*)gz7Da4KB(f3CK6csT()); if (addr == NULL)
return; mil_CopyImageDirPal(p, NP3y8tCu(), mkfkxUSx(), Kh97ARZU(), addr);
Y8rugKNJ -= 5; }
static void FbTnNFAq() 
{ register INT2 a7P6Xwbo = Kh97ARZU(); register BYTE* p;
USCUWVfg(p, a7P6Xwbo); NP3y8tCu() = mil_AddDQ(p, mkfkxUSx(), U6liRwBT(), f3CK6csT());
Y8rugKNJ -= 5; } static void V0N3MW7L() 
{ mil_DrawDQ(e7mBDeVX[Y8rugKNJ--]); }
static void FpIq8p4F()
{ if (swDispMode == S_CM_GAME) midd_DrawLcd(swLcdBuffer);
} static void E5oieFg5() { mil_SaveLCD();
} static void z2QhsGng() { mil_RestoreLCD();
}
int svm_SetMediaSize(int index, int size)
{ register S_RamMediaElem *zftHByT3; register BYTE *ShSmRQr0;
register int MS2OjvYL; register int SZL2Yb99;
register int H0Ng1GoT; if (index < 0 || index >= JG14eUey) {
s_Exception(SWAP_ECPT_INVALID_MDA_INDEX);
return 0; } zftHByT3 = &swRamMediaTable[index];
MS2OjvYL = size - zftHByT3->size; if (MS2OjvYL%2 == 1) 
MS2OjvYL++;  if (-SWAP_MEDIA_ACCEPT_DIFF <= MS2OjvYL && MS2OjvYL <= 0) { 
return 1; } if (YTwPrBKb + MS2OjvYL > swWorkSpace + SWAP_MAX_WORKSPACE) {
s_Exception(SWAP_ECPT_MDA_OVER); return 0; 
} ShSmRQr0 = zftHByT3->addr + zftHByT3->size; 
SZL2Yb99 = 0; 
for (H0Ng1GoT = index + 1; H0Ng1GoT < JG14eUey; H0Ng1GoT++) {
if (swRamMediaTable[H0Ng1GoT].var == Tsbk5B54) {
SZL2Yb99 += swRamMediaTable[H0Ng1GoT].size; 
swRamMediaTable[H0Ng1GoT].addr += MS2OjvYL; 
} } memmove(ShSmRQr0 + MS2OjvYL, ShSmRQr0, SZL2Yb99); 
zftHByT3->size += MS2OjvYL; YTwPrBKb += MS2OjvYL;
return 1; } static void FQF2EFY9() 
{ f3CK6csT() = ZgTiYWea(f3CK6csT()); } static void QNJ3ZSiJ() 
{ Kh97ARZU() = svm_SetMediaSize(Kh97ARZU(), f3CK6csT());
Y8rugKNJ--; } static void mgRKuK6x() 
{ register int index = Kh97ARZU(); register int size = f3CK6csT();
if (svm_SetMediaSize(index, size)) memset(AI9TGVGd(index), 0, size);
Y8rugKNJ -= 2; } static void GVi6vy3B() 
{ f3CK6csT() = strlen((char*)AI9TGVGd(f3CK6csT()));
} static void vRZkPo25() 
{ register int t70tQu1I = Kh97ARZU(); register int Dt5xkCCq = f3CK6csT();
if (svm_SetMediaSize(t70tQu1I, ZgTiYWea(Dt5xkCCq)))
strcpy((char*)AI9TGVGd(t70tQu1I), (const char*)AI9TGVGd(Dt5xkCCq));
Y8rugKNJ -= 2; } static void hMH3WsiT() 
{ register int s1 = mkfkxUSx(); register int s2 = U6liRwBT();
register int ShSmRQr0 = Kh97ARZU(); register int len = f3CK6csT();
if (svm_SetMediaSize(s1, len + 1)) { strncpy((char*)AI9TGVGd(s1), (const char*)(AI9TGVGd(s2) + ShSmRQr0), len);
*(AI9TGVGd(s1) + len) = '\0'; } Y8rugKNJ -= 4;
} static void QhkGDqQp() 
{ register int t70tQu1I = Kh97ARZU(); register int Dt5xkCCq = f3CK6csT();
register int len = strlen((char*)AI9TGVGd(t70tQu1I)) + strlen((char*)AI9TGVGd(Dt5xkCCq)) + 1;
if (svm_SetMediaSize(t70tQu1I, len)) strcat((char*)AI9TGVGd(t70tQu1I), (char*)AI9TGVGd(Dt5xkCCq));
Y8rugKNJ -= 2; } static void OOEgS7ee() 
{ Kh97ARZU() = strcmp((const char*)AI9TGVGd(Kh97ARZU()), (const char*)AI9TGVGd(f3CK6csT()));
Y8rugKNJ--; } static void dRZl7pCZ() 
{ Kh97ARZU() = ((short)*((const char*)AI9TGVGd(Kh97ARZU()) + f3CK6csT()));
Y8rugKNJ--; } static void nFN2dtzj() 
{ *((char*)(AI9TGVGd(U6liRwBT()) + Kh97ARZU())) = (char)(f3CK6csT());
Y8rugKNJ -= 3; } static void A7uvuQlI() 
{ f3CK6csT() = atoi((char*)(AI9TGVGd(f3CK6csT())));
} static void LmmADihB() 
{ char mSdyqwa7[64]; register int Jv2UAbfL = Kh97ARZU();
sprintf(mSdyqwa7, "%d", f3CK6csT()); if (svm_SetMediaSize(Jv2UAbfL, strlen(mSdyqwa7) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), mSdyqwa7);
Y8rugKNJ -= 2; } static void drSwT6Gs() 
{ register BYTE *data = (BYTE*)gz7Da4KB(U6liRwBT());
if (data == NULL) return; *(data + Kh97ARZU()) = (BYTE)f3CK6csT();
Y8rugKNJ -= 3; } static void x8GCfooj() 
{ register BYTE *data = (BYTE*)gz7Da4KB(Kh97ARZU());
if (data == NULL) return; Kh97ARZU() = (INT2)*(data + f3CK6csT());
Y8rugKNJ--; } static void z48637g9() 
{ register BYTE *Jv2UAbfL = (BYTE*)gz7Da4KB(mkfkxUSx());
register BYTE *WDqujLu9 = (BYTE*)AI9TGVGd(Kh97ARZU());
if (Jv2UAbfL == NULL) return; memcpy(Jv2UAbfL + U6liRwBT(), WDqujLu9, f3CK6csT());
Y8rugKNJ -= 4; } static void KGRTuzlW() 
{ register int Jv2UAbfL = Kh97ARZU(); register int size = f3CK6csT();
register BYTE *addr; if (svm_SetMediaSize(Jv2UAbfL, size)) {
addr = (BYTE*)gz7Da4KB(mkfkxUSx()); if (addr == NULL)
return; memcpy(AI9TGVGd(Kh97ARZU()), addr + U6liRwBT(), size);
} Y8rugKNJ -= 4; } 
#define F6VdJTwN 0x08 

#define ND5kuLMs 0x10 

#define Fd0iW106 0x20 

#define d9AshwlE 0x40 

#define A6pcPPFu 255

#ifndef Gte7bGwX

#define Gte7bGwX(_c) ( (unsigned)(_c) < 0x80 )

#endif
static void jynRrmeb() 
{ char mSdyqwa7[A6pcPPFu+1]; int index = 0;
int Jv2UAbfL = U6liRwBT(); char *fmt; 
char *WDqujLu9;
int n; 
char *a7P6Xwbo; 
int dprec; 
int fieldsz; 
int bfFlags; 
int fpprec; 
int prec; 
int realsz; 
int size; 
int nWidth; 
char sign; 
int neg_ldouble = 0; 
fmt = (char*)AI9TGVGd(Kh97ARZU()); WDqujLu9 = (char*)AI9TGVGd(f3CK6csT());
for (;; ++fmt){ while ((*fmt) && (*fmt) != '%'){
mSdyqwa7[index++] = (char)*fmt; if( index >= A6pcPPFu ){
mSdyqwa7[index++] = '\0'; goto IndexOverflow;
} fmt++; } if (!(*fmt)) goto endOfLoop; bfFlags = 0; dprec = 0; fpprec = 0; nWidth = 0;
prec = -1; sign = '\0';
rflag:
switch (*++fmt){
case '-': bfFlags |= ND5kuLMs; goto rflag;
case '.': n = 0; while (Gte7bGwX((unsigned char)*fmt) && isdigit((unsigned char)*fmt))
n = 10 * n + ((*fmt++) - '0' ); --fmt; prec = n < 0 ? -1 : n;
goto rflag; case '0': bfFlags |= Fd0iW106;
goto rflag; case '1': case '2': case '3': case '4':
case '5': case '6': case '7': case '8': case '9':
n = 0; do { n = 10 * n + ((*fmt) - '0'); } while (Gte7bGwX((unsigned char)*++fmt) && isdigit((unsigned char)*fmt));
nWidth = n; --fmt; goto rflag; case 's': a7P6Xwbo = WDqujLu9;
if (!a7P6Xwbo){ a7P6Xwbo = ""; } if (prec >= 0){
char *p ;
p = (char *)memchr(a7P6Xwbo, 0, prec); if (p){
size = p - a7P6Xwbo; if (size > prec) size = prec;
} else size = prec; } else size = strlen(a7P6Xwbo);
sign = '\0'; fieldsz = size + fpprec; realsz = dprec > fieldsz ? dprec : fieldsz;
if (sign){ realsz++; } if (bfFlags & d9AshwlE){
realsz += 2; } if ((bfFlags & (ND5kuLMs|Fd0iW106)) == 0 && nWidth){
for (n = realsz; n < nWidth; n++){ mSdyqwa7[index++] = ' ';
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } } } if (sign){
mSdyqwa7[index++] = sign; if( index >= A6pcPPFu ){
mSdyqwa7[index++] = '\0'; goto IndexOverflow;
} } if (bfFlags & d9AshwlE) { mSdyqwa7[index++] = '0';
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } mSdyqwa7[index++] = (char)*fmt;
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } } if ((bfFlags & (ND5kuLMs|Fd0iW106)) == Fd0iW106){
for (n = realsz; n < nWidth; n++){ mSdyqwa7[index++] = '0';
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } } } for (n = fieldsz; n < dprec; n++){
mSdyqwa7[index++] = '0'; if( index >= A6pcPPFu ){
mSdyqwa7[index++] = '\0'; goto IndexOverflow;
} } for (n = size; n > 0; n--){
mSdyqwa7[index++] = *a7P6Xwbo++; if( index >= A6pcPPFu ){
mSdyqwa7[index++] = '\0'; goto IndexOverflow;
} } while (--fpprec >= 0){
mSdyqwa7[index++] = '0'; if( index >= A6pcPPFu ){
mSdyqwa7[index++] = '\0'; goto IndexOverflow;
} } if (bfFlags & ND5kuLMs){
for (n = realsz; n < nWidth; n++){ mSdyqwa7[index++] = ' ';
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } } } break;
case '\0': 
goto endOfLoop; default: mSdyqwa7[index++] = *fmt;
if( index >= A6pcPPFu ){ mSdyqwa7[index++] = '\0';
goto IndexOverflow; } } }
endOfLoop:
mSdyqwa7[index++] = (char)*fmt;
IndexOverflow:
if (svm_SetMediaSize(Jv2UAbfL, strlen(mSdyqwa7) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), mSdyqwa7);
Y8rugKNJ -= 3; } static void lu0rwjYj( char *fstr, char *lpszFormat, int top[], int NumOfParam )
{ int index = 0; int var[5]; int variable;
int H0Ng1GoT = 0; char *fmt; int nWidth; int nPrecision;
int bfFlags; int dprec; int fpprec; char sign;
const char *digs; 
int n; char *a7P6Xwbo; char buf[6]; int realsz; 
int fieldsz; int size; int base; for( H0Ng1GoT = 0; H0Ng1GoT < NumOfParam; H0Ng1GoT++ ){
var[H0Ng1GoT] = top[H0Ng1GoT]; } H0Ng1GoT = 0;
digs = "0123456789abcdef"; for (fmt = lpszFormat;; fmt++)
{ while (((*fmt) && *fmt != '%' ) || ( H0Ng1GoT > NumOfParam )){
fstr[index++] = (char)*fmt; if( index >= A6pcPPFu ){
fstr[index++] = '\0'; goto IndexOverflow;
} fmt++; } if (!*fmt){ break; } bfFlags = 0; dprec = 0; fpprec = 0; nWidth = 0;
nPrecision = -1; sign = '\0';
rflag: 
switch (*++fmt)
{ case ' ': if (!sign)
sign = ' '; goto rflag; case '#': bfFlags |= F6VdJTwN;
goto rflag; case '-': bfFlags |= ND5kuLMs;
goto rflag; case '+': sign = '+'; goto rflag;
case '.': n = 0; while (Gte7bGwX((unsigned char)*fmt) && isdigit((unsigned char)*fmt))
n = 10 * n + ((*fmt++) - '0'); --fmt; nPrecision = n < 0 ? -1 : n;
goto rflag; case '0': bfFlags |= Fd0iW106;
goto rflag; case '1': case '2': case '3': case '4':
case '5': case '6': case '7': case '8': case '9':
n = 0; do { n = 10 * n + ( *fmt - '0' );
} while (Gte7bGwX((unsigned char)*++fmt) && isdigit((unsigned char)*fmt));
nWidth = n; --fmt; goto rflag; case 'c': *(a7P6Xwbo = buf) = ( char ) var[H0Ng1GoT++];
size = 1; sign = '\0'; goto pforw; case 'd':
variable = var[H0Ng1GoT++]; if (variable < 0){
variable = -variable; sign = '-'; } base = 10;
goto number; case 'X': digs = "0123456789ABCDEF";
case 'x':
variable = var[H0Ng1GoT++]; base = 16; if (bfFlags & F6VdJTwN && variable != 0)
bfFlags |= d9AshwlE; sign = '\0';
number:
if ((dprec = nPrecision) >= 0)
bfFlags &= ~Fd0iW106; a7P6Xwbo = buf + 6;
if (variable != 0 || nPrecision != 0){ 
#define uzBLxvrM(type) \
{ \
register type _n = (type)variable; \
do { \
*--a7P6Xwbo = digs[_n % base]; \
_n /= base; \
} while (_n); \
} 
uzBLxvrM(unsigned long)
#undef uzBLxvrM
if (bfFlags & F6VdJTwN && base == 8 && *a7P6Xwbo != '0')
*--a7P6Xwbo = '0'; 
} digs = "0123456789abcdef"; size = buf + 6 - a7P6Xwbo;
pforw:
fieldsz = size + fpprec;
realsz = dprec > fieldsz ? dprec : fieldsz;
if (sign) realsz++; if (bfFlags & d9AshwlE)
realsz += 2; if ((bfFlags & (ND5kuLMs|Fd0iW106)) == 0 && nWidth){
for (n = realsz; n < nWidth; n++){ fstr[index++] = ' ';
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } } } if (sign){
fstr[index++] = sign; if( index >= A6pcPPFu ){
fstr[index++] = '\0'; goto IndexOverflow;
} } if (bfFlags & d9AshwlE){ fstr[index++] = '0';
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } fstr[index++] = (char)*fmt;
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } } if ((bfFlags & (ND5kuLMs|Fd0iW106)) == Fd0iW106){
for (n = realsz; n < nWidth; n++){ fstr[index++] = '0';
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } } } for (n = fieldsz; n < dprec; n++){
fstr[index++] = '0'; if( index >= A6pcPPFu ){
fstr[index++] = '\0'; goto IndexOverflow;
} } for (n = size; n > 0; n--){
fstr[index++] = *a7P6Xwbo++; if( index >= A6pcPPFu ){
fstr[index++] = '\0'; goto IndexOverflow;
} } while (--fpprec >= 0){
fstr[index++] = '0'; if( index >= A6pcPPFu ){
fstr[index++] = '\0'; goto IndexOverflow;
} } if (bfFlags & ND5kuLMs){
for (n = realsz; n < nWidth; n++){ fstr[index++] = ' ';
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } } } break;
case '\0': 
goto endOfLoop; default: fstr[index++] = *fmt;
if( index >= A6pcPPFu ){ fstr[index++] = '\0';
goto IndexOverflow; } } }
endOfLoop:
fstr[index++] = (char)*fmt;
IndexOverflow:
return;
} static void Iiir6kHM() 
{ char fstr[A6pcPPFu+1]; int Jv2UAbfL = U6liRwBT();
int var[1]; var[0] = f3CK6csT(); lu0rwjYj( fstr, (char*)AI9TGVGd(Kh97ARZU()), var, 1 );
if (svm_SetMediaSize(Jv2UAbfL, strlen(fstr) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), fstr); Y8rugKNJ -= 3;
} static void zl43W2TJ() 
{ char fstr[A6pcPPFu+1]; int Jv2UAbfL = mkfkxUSx();
int var[2]; var[0] = Kh97ARZU(); var[1] = f3CK6csT();
lu0rwjYj( fstr, (char*)AI9TGVGd(U6liRwBT()), var, 2 );
if (svm_SetMediaSize(Jv2UAbfL, strlen(fstr) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), fstr); Y8rugKNJ -= 4;
} static void Tf8yQ77b() 
{ char fstr[A6pcPPFu+1]; int Jv2UAbfL = NP3y8tCu();
int var[3]; var[0] = U6liRwBT(); var[1] = Kh97ARZU();
var[2] = f3CK6csT(); lu0rwjYj( fstr, (char*)AI9TGVGd(mkfkxUSx()), var, 3 );
if (svm_SetMediaSize(Jv2UAbfL, strlen(fstr) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), fstr); Y8rugKNJ -= 5;
} static void vVP7jCTx() 
{ char fstr[A6pcPPFu+1]; int Jv2UAbfL = AvwqZ1W4();
int var[4]; var[0] = mkfkxUSx(); var[1] = U6liRwBT();
var[2] = Kh97ARZU(); var[3] = f3CK6csT();
lu0rwjYj( fstr, (char*)AI9TGVGd(NP3y8tCu()), var, 4 );
if (svm_SetMediaSize(Jv2UAbfL, strlen(fstr) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), fstr); Y8rugKNJ -= 6;
} static void Hx3dIuDA() 
{ char fstr[A6pcPPFu+1]; int Jv2UAbfL = cAQOtAsc();
int var[5]; var[0] = NP3y8tCu(); var[1] = mkfkxUSx();
var[2] = U6liRwBT(); var[3] = Kh97ARZU();
var[4] = f3CK6csT(); lu0rwjYj( fstr, (char*)AI9TGVGd(AvwqZ1W4()), var, 5 );
if (svm_SetMediaSize(Jv2UAbfL, strlen(fstr) + 1))
strcpy((char*)AI9TGVGd(Jv2UAbfL), fstr); Y8rugKNJ -= 7;
} static void abSzvvHf() 
{ JZagjmLi(); y5TvOyEz = (int)f3CK6csT();
svResult.code = SWAP_RC_TEXTEDIT; svResult.param.edit.title = (char*)AI9TGVGd(Kh97ARZU());
svResult.param.edit.input =(char*)AI9TGVGd(y5TvOyEz);
svResult.param.edit.maxlen = SWAP_LEN_TEXT;
svResult.param.edit.strtype = 0; Y8rugKNJ -= 2;
BT9PYMlQ = (BYTE*)&GICEruWy; 
}
static void ZiXpvN3k() 
{ midd_PlaySound((BYTE*)AI9TGVGd(f3CK6csT()), ZgTiYWea(f3CK6csT()));
Y8rugKNJ--; } static void zroUNpSW() 
{ midd_PlayVocoder((BYTE*)AI9TGVGd(f3CK6csT()), ZgTiYWea(f3CK6csT()));
Y8rugKNJ--; } static void zwMe7uTc() 
{ midd_StartVib(e7mBDeVX[Y8rugKNJ--]); } static void R0C2PjBV() 
{ midd_SetKeyTone(e7mBDeVX[Y8rugKNJ--]); }
static void Fju2VHHp() 
{ midd_SetBacklight(e7mBDeVX[Y8rugKNJ--]);
} static void ufCNKLtK() 
{ register BYTE *addr = (BYTE*)gz7Da4KB(Kh97ARZU());
if (addr == NULL) return; midd_GetUserNV(svSSBIndex, addr, f3CK6csT()*2);
Y8rugKNJ -= 2; } static void wPfkxgQc() 
{ register BYTE *addr = (BYTE*)gz7Da4KB(Kh97ARZU());
if (addr == NULL) return; midd_PutUserNV(svSSBIndex, addr, f3CK6csT()*2);
Y8rugKNJ -= 2; } static void LA9zIClI() 
{ midd_StartTimer(0, DFbjS7T4[0] = Kh97ARZU(), EOeyaIK2[0] = f3CK6csT());
Y8rugKNJ -= 2; } static void RmhvEPkn() 
{ midd_StartTimer(1, DFbjS7T4[1] = Kh97ARZU(), EOeyaIK2[1] = f3CK6csT());
Y8rugKNJ -= 2; } static void cdSPy35v() 
{ midd_StartTimer(2, DFbjS7T4[2] = Kh97ARZU(), EOeyaIK2[2] = f3CK6csT());
Y8rugKNJ -= 2; } static void SVdgQk7R() {
DFbjS7T4[0] = 0; midd_StopTimer(0); } static void LukcFCrd()
{ DFbjS7T4[1] = 0; midd_StopTimer(1); } static void rinMobWi()
{ DFbjS7T4[2] = 0; midd_StopTimer(2); }
static void R823MiOq() 
{ srand(e7mBDeVX[Y8rugKNJ--]); } static void hA1H4K56() 
{ register INT2 mSdyqwa7; register INT2 min = Kh97ARZU();
register INT2 max = f3CK6csT(); if (min != max) {
if (min > max) { mSdyqwa7 = min; min = max;
max = mSdyqwa7; } Kh97ARZU() = min + (rand()%(max - min));
} Y8rugKNJ--; } static void na0XNBCg() 
{ f3CK6csT() = ((rand() % 100 < f3CK6csT()) ? 1 : 0);
} static void CiOLuoM0() 
{ if (f3CK6csT() < 0) f3CK6csT() = -f3CK6csT();
} static void h3Xp5zzX() 
{ if (f3CK6csT() > 0) f3CK6csT() = 1; else if (f3CK6csT() == 0)
f3CK6csT() = 0; else f3CK6csT() = -1; } static const INT2 aoZjmFp7[91][3] = {
0, 100, 0,  2, 100, 2,  3, 100, 3,  5, 100, 5,  7, 100, 7,
9, 100, 9,  10, 99, 11,  12, 99, 12,  14, 99, 14,  16, 99, 16,
17, 98, 18,  19, 98, 19,  21, 98, 21,  22, 97, 23,  24, 97, 25,
26, 97, 27,  28, 96, 29,  29, 96, 31,  31, 95, 32,  33, 95, 34,
34, 94, 36,  36, 93, 38,  37, 93, 40,  39, 92, 42,  41, 91, 45,
42, 91, 47,  44, 90, 49,  45, 89, 51,  47, 88, 53,  48, 87, 55,
50, 87, 58,  52, 86, 60,  53, 85, 62,  54, 84, 65,  56, 83, 67,
57, 82, 70,  59, 81, 73,  60, 80, 75,  62, 79, 78,  63, 78, 81,
64, 77, 84,  66, 75, 87,  67, 74, 90,  68, 73, 93,  69, 72, 97,
71, 71, 100,  72, 69, 104,  73, 68, 107,  74, 67, 111,  75, 66, 115,
77, 64, 119,  78, 63, 123,  79, 62, 128,  80, 60, 133,  81, 59, 138,
82, 57, 143,  83, 56, 148,  84, 54, 154,  85, 53, 160,  86, 52, 166,
87, 50, 173,  87, 48, 180,  88, 47, 188,  89, 45, 196,  90, 44, 205,
91, 42, 214,  91, 41, 225,  92, 39, 236,  93, 37, 248,  93, 36, 261,
94, 34, 275,  95, 33, 290,  95, 31, 308,  96, 29, 327,  96, 28, 349,
97, 26, 373,  97, 24, 401,  97, 22, 433,  98, 21, 470,  98, 19, 514,
98, 17, 567,  99, 16, 631,  99, 14, 712,  99, 12, 814,  99, 10, 951,
100, 9, 1143,  100, 7, 1430,  100, 5, 1908,  100, 3, 2864,  100, 2, 5729,
100, 0, 0 
}; static const INT2 ak6MkVXT[101][3] = {
0, 90, 0,  1, 89, 1,  1, 89, 1,  2, 88, 2,  2, 88, 2,
3, 87, 3,  3, 87, 3,  4, 86, 4,  5, 85, 5,  5, 85, 5,
6, 84, 6,  6, 84, 6,  7, 83, 7,  7, 83, 7,  8, 82, 8,
9, 81, 9,  9, 81, 9,  10, 80, 10,  10, 80, 10,  11, 79, 11,
12, 78, 11,  12, 78, 12,  13, 77, 12,  13, 77, 13,  14, 76, 13,
14, 76, 14,  15, 75, 15,  16, 74, 15,  16, 74, 16,  17, 73, 16,
17, 73, 17,  18, 72, 17,  19, 71, 18,  19, 71, 18,  20, 70, 19,
20, 70, 19,  21, 69, 20,  22, 68, 20,  22, 68, 21,  23, 67, 21,
24, 66, 22,  24, 66, 22,  25, 65, 23,  25, 65, 23,  26, 64, 24,
27, 63, 24,  27, 63, 25,  28, 62, 25,  29, 61, 26,  29, 61, 26,
30, 60, 27,  31, 59, 27,  31, 59, 27,  32, 58, 28,  33, 57, 28,
33, 57, 29,  34, 56, 29,  35, 55, 30,  35, 55, 30,  36, 54, 31,
37, 53, 31,  38, 52, 31,  38, 52, 32,  39, 51, 32,  40, 50, 33,
41, 49, 33,  41, 49, 33,  42, 48, 34,  43, 47, 34,  44, 46, 35,
44, 46, 35,  45, 45, 35,  46, 44, 36,  47, 43, 36,  48, 42, 37,
49, 41, 37,  49, 41, 37,  50, 40, 38,  51, 39, 38,  52, 38, 38,
53, 37, 39,  54, 36, 39,  55, 35, 39,  56, 34, 40,  57, 33, 40,
58, 32, 40,  59, 31, 41,  60, 30, 41,  62, 28, 41,  63, 27, 42,
64, 26, 42,  66, 24, 42,  67, 23, 43,  68, 22, 43,  70, 20, 43,
72, 18, 44,  74, 16, 44,  76, 14, 44,  79, 11, 44,  82, 8, 45,
90, 0, 45
}; static void mRHSnFp1() 
{ register INT2 hTWbhhmL = f3CK6csT(); while (hTWbhhmL < 0)
hTWbhhmL += 360; hTWbhhmL %= 360; if (hTWbhhmL <= 90) 
f3CK6csT() = aoZjmFp7[hTWbhhmL][0]; else if (hTWbhhmL <= 180) 
f3CK6csT() = aoZjmFp7[180 - hTWbhhmL][0];
else if (hTWbhhmL <= 270) 
f3CK6csT() = -aoZjmFp7[hTWbhhmL - 180][0];
else 
f3CK6csT() = -aoZjmFp7[360 - hTWbhhmL][0];
} static void JWyRjw0d() 
{ register INT2 hTWbhhmL = f3CK6csT(); while (hTWbhhmL < 0)
hTWbhhmL += 360; hTWbhhmL %= 360; if (hTWbhhmL <= 90) 
f3CK6csT() = aoZjmFp7[hTWbhhmL][1]; else if (hTWbhhmL <= 180) 
f3CK6csT() = -aoZjmFp7[180 - hTWbhhmL][1];
else if (hTWbhhmL <= 270) 
f3CK6csT() = -aoZjmFp7[hTWbhhmL - 180][1];
else 
f3CK6csT() = aoZjmFp7[360 - hTWbhhmL][1];
} static void DMqJ0k0h() 
{ register INT2 hTWbhhmL = f3CK6csT(); while (hTWbhhmL < 0)
hTWbhhmL += 180; hTWbhhmL %= 180; if (hTWbhhmL <= 90) 
f3CK6csT() = aoZjmFp7[hTWbhhmL][2]; else 
f3CK6csT() = -aoZjmFp7[180 - hTWbhhmL][2];
} static void s3wzcxdl() 
{ register INT2 xZSi5aLV = f3CK6csT(); if (xZSi5aLV < -100 || xZSi5aLV > 100) {
f3CK6csT() = 32767; 
return; } if (xZSi5aLV >= 0) 
f3CK6csT() = ak6MkVXT[xZSi5aLV][0]; else 
f3CK6csT() = -ak6MkVXT[-xZSi5aLV][0]; } static void GR8qJMC3() 
{ register INT2 xZSi5aLV = f3CK6csT(); if (xZSi5aLV < -100 || xZSi5aLV > 100) {
f3CK6csT() = 32767; 
return; } if (xZSi5aLV >= 0) 
f3CK6csT() = ak6MkVXT[xZSi5aLV][1]; else 
f3CK6csT() = 180 - ak6MkVXT[-xZSi5aLV][1];
} static void ftfjUT53() 
{ register INT2 xZSi5aLV = f3CK6csT(); if (xZSi5aLV <= -100 || xZSi5aLV > 100) {
f3CK6csT() = 32767; 
return; } if (xZSi5aLV >= 0) 
f3CK6csT() = ak6MkVXT[xZSi5aLV][2]; else 
f3CK6csT() = -ak6MkVXT[-xZSi5aLV][2]; } static void JwIur34E() 
{ e7mBDeVX[--Y8rugKNJ] = (f3CK6csT() + Kh97ARZU())/2;
} static void whFcpWZ2() 
{ U6liRwBT() = (f3CK6csT() + Kh97ARZU() + U6liRwBT())/3;
Y8rugKNJ -= 2; } static void QsIeLzu2() 
{ register INT2 aRQXmi4s = f3CK6csT(); register INT2 DbluEGUN = Kh97ARZU();
e7mBDeVX[--Y8rugKNJ] = (aRQXmi4s >= DbluEGUN) ? aRQXmi4s : DbluEGUN;
} static void xgVzvYXt() 
{ register INT2 aRQXmi4s = f3CK6csT(); register INT2 DbluEGUN = Kh97ARZU();
register INT2 BoGg8j3G = U6liRwBT(); if (aRQXmi4s >= DbluEGUN)
U6liRwBT() = (aRQXmi4s >= BoGg8j3G) ? aRQXmi4s : BoGg8j3G;
else U6liRwBT() = (DbluEGUN >= BoGg8j3G) ? DbluEGUN : BoGg8j3G;
Y8rugKNJ -= 2; } static void RGI6Le21() 
{ register INT2 aRQXmi4s = f3CK6csT(); register INT2 DbluEGUN = Kh97ARZU();
e7mBDeVX[--Y8rugKNJ] = (aRQXmi4s <= DbluEGUN) ? aRQXmi4s : DbluEGUN;
} static void gFu09gz6() 
{ register INT2 aRQXmi4s = f3CK6csT(); register INT2 DbluEGUN = Kh97ARZU();
register INT2 BoGg8j3G = U6liRwBT(); if (aRQXmi4s <= DbluEGUN)
U6liRwBT() = (aRQXmi4s <= BoGg8j3G) ? aRQXmi4s : BoGg8j3G;
else U6liRwBT() = (DbluEGUN <= BoGg8j3G) ? DbluEGUN : BoGg8j3G;
Y8rugKNJ -= 2; } static void bDA1TYkC() { 
register INT2 offset = Kh97ARZU(); register INT2 *Ydsi5ovo = gz7Da4KB(offset);
register INT2 size = f3CK6csT(); register int GnvQpz11, max, H0Ng1GoT;
if (Ydsi5ovo == NULL) return; Y8rugKNJ--;
GnvQpz11 = 0; max = *Ydsi5ovo; Ydsi5ovo++;
for (H0Ng1GoT = 1; H0Ng1GoT < size; H0Ng1GoT++, Ydsi5ovo++) {
if (*Ydsi5ovo > max) { GnvQpz11 = H0Ng1GoT;
max = *Ydsi5ovo; } } f3CK6csT() = GnvQpz11;
} static void RF4rjaDK() { 
register INT2 offset = Kh97ARZU(); register INT2 *Ydsi5ovo = gz7Da4KB(offset);
register INT2 size = f3CK6csT(); register int GnvQpz11, min, H0Ng1GoT;
if (Ydsi5ovo == NULL) return; Y8rugKNJ--;
GnvQpz11 = 0; min = *Ydsi5ovo; Ydsi5ovo++;
for (H0Ng1GoT = 1; H0Ng1GoT < size; H0Ng1GoT++, Ydsi5ovo++) {
if (*Ydsi5ovo < min) { GnvQpz11 = H0Ng1GoT;
min = *Ydsi5ovo; } } f3CK6csT() = GnvQpz11;
} 
#define pHk3bPDl(n) (((n) >= 0) ? (n) : -(n))
static void TRUeJF5p() { 
register INT2 offset = U6liRwBT(); register INT2 *Ydsi5ovo = gz7Da4KB(offset);
register INT2 size = Kh97ARZU(); register INT2 key = f3CK6csT();
register int GnvQpz11, mdist, H0Ng1GoT, D1YwFQc1;
if (Ydsi5ovo == NULL) return; Y8rugKNJ -= 2;
GnvQpz11 = 0; mdist = pHk3bPDl(*Ydsi5ovo - key);
Ydsi5ovo++; for (H0Ng1GoT = 1; H0Ng1GoT < size; H0Ng1GoT++, Ydsi5ovo++) {
D1YwFQc1 = pHk3bPDl(*Ydsi5ovo - key); if (D1YwFQc1 < mdist) {
GnvQpz11 = H0Ng1GoT; mdist = D1YwFQc1; if (mdist == 0) 
break; } } f3CK6csT() = GnvQpz11; } static void fJSt9vf5(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = v; a++; }
} static void FU7aKqij(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a += v; a++; } 
} static void TgVg3TkO(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a -= v; a++; } 
} static void iFW76UTu(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a *= v; a++; } 
} static void FVfKdJDv(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; if (v == 0) { s_Exception(SWAP_ECPT_DIV_ZERO); return; } for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a /= v; a++; } 
} static void zz4tjOti(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; if (v == 0) { s_Exception(SWAP_ECPT_MOD_ZERO); return; } for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a %= v; a++; } 
} static void pvs6R3Sj(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a &= v; a++; } 
} static void vm2jSjAz(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a |= v; a++; } 
} static void rFnXVlz6(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = ~v; a++; } 
} static void Z0UtcsUA(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a ^= v; a++; } 
} static void Jikqh1MW(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a >>= v; a++; } 
} static void DWHZiOAD(INT2 *a, INT2 v, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a <<= v; a++; } 
} static void (*OLs9dzti[])(INT2 *a, INT2 v, INT2 size) = {
fJSt9vf5, FU7aKqij, TgVg3TkO, iFW76UTu, FVfKdJDv,
zz4tjOti, pvs6R3Sj, vm2jSjAz, rFnXVlz6, Z0UtcsUA,
Jikqh1MW, DWHZiOAD }; static void qrCMdwBc() 
{ register INT2 *addr = gz7Da4KB(mkfkxUSx());
if (addr == NULL) return; OLs9dzti[f3CK6csT()](addr, U6liRwBT(), Kh97ARZU());
Y8rugKNJ -= 4; } static void mnutAzPu(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b; a++; b++; } 
} static void KMrcImxP(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a += *b; a++; b++; } 
} static void WTFlcfxL(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a -= *b; a++; b++; } 
} static void mSfYMjTo(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a *= *b; a++; b++; } 
} static void sgWZa35q(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { if (*b == 0) { s_Exception(SWAP_ECPT_DIV_ZERO); return; } *a /= *b; a++; b++; } 
} static void VCNYGzTw(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { if (*b == 0) { s_Exception(SWAP_ECPT_MOD_ZERO); return; } *a %= *b; a++; b++; } 
} static void EZgYCKCk(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a &= *b; a++; b++; } 
} static void gcOyvaiF(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a |= *b; a++; b++; } 
} static void LlBG6qjZ(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = ~*b; a++; b++; } 
} static void KWjjk3Ne(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a ^= *b; a++; b++; } 
} static void intYQAZ4(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a >>= *b; a++; b++; } 
} static void K4TZC498(INT2 *a, INT2 *b, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a <<= *b; a++; b++; } 
} static void (*TSvFBcBx[])(INT2 *a, INT2 *b, INT2 size) = {
mnutAzPu, KMrcImxP, WTFlcfxL, mSfYMjTo, sgWZa35q,
VCNYGzTw, EZgYCKCk, gcOyvaiF, LlBG6qjZ, KWjjk3Ne,
intYQAZ4, K4TZC498 }; static void y7yEY3Hf() 
{ register INT2 *addr1 = gz7Da4KB(mkfkxUSx());
register INT2 *addr2 = gz7Da4KB(U6liRwBT());
if (addr1 == NULL || addr2 == NULL) return;
TSvFBcBx[f3CK6csT()](addr1, addr2, Kh97ARZU());
Y8rugKNJ -= 4; } static void F94HVh2c(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b; a++; b++; c++; } 
} static void zyxcZsxb(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b + *c; a++; b++; c++; } 
} static void YS93vXOc(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b - *c; a++; b++; c++; } 
} static void ml5o3Rmz(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b * *c; a++; b++; c++; } 
} static void aMgYZu1G(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { if (*c == 0) { s_Exception(SWAP_ECPT_DIV_ZERO); return; } *a = *b / *c; a++; b++; c++; } 
} static void jYk28tjm(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { if (*c == 0) { s_Exception(SWAP_ECPT_MOD_ZERO); return; } *a = *b % *c; a++; b++; c++; } 
} static void iazoOS2M(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b & *c; a++; b++; c++; } 
} static void J6vnDQgO(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b | *c; a++; b++; c++; } 
} static void E1nAopVG(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = ~*b; a++; b++; } 
} static void fvnlciAY(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b ^ *c; a++; b++; c++; } 
} static void FvOb6IEh(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b >> *c; a++; b++; c++; } 
} static void mJVqXuCS(INT2 *a, INT2 *b, INT2 *c, INT2 size)
{ register int H0Ng1GoT; for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++) { *a = *b << *c; a++; b++; c++; } 
} static void (*SwIfwP05[])(INT2 *a, INT2 *b, INT2 *c, INT2 size) = {
F94HVh2c, zyxcZsxb, YS93vXOc, ml5o3Rmz, aMgYZu1G,
jYk28tjm, iazoOS2M, J6vnDQgO, E1nAopVG, fvnlciAY,
FvOb6IEh, mJVqXuCS }; static void t9iTtCgu() 
{ register INT2 *addr1 = gz7Da4KB(NP3y8tCu());
register INT2 *addr2 = gz7Da4KB(mkfkxUSx());
register INT2 *addr3 = gz7Da4KB(U6liRwBT());
if (addr1 == NULL || addr2 == NULL || addr3 == NULL)
return; SwIfwP05[f3CK6csT()](addr1, addr2, addr3, Kh97ARZU());
Y8rugKNJ -= 5; }
static void lnpRbNNp() 
{ register INT2 aRQXmi4s = U6liRwBT(); register INT2 DbluEGUN = Kh97ARZU();
register INT2 d = f3CK6csT(); Y8rugKNJ -= 2;
f3CK6csT() = (aRQXmi4s - d < DbluEGUN && DbluEGUN < aRQXmi4s + d) ? 1 : 0;
} static void XWZgUjtk() 
{ register INT2 *data = gz7Da4KB(f3CK6csT());
if (data == NULL) return; midd_GetDate(data, data + 1, data + 2, data + 3);
Y8rugKNJ--; } static void uH57ZEhU() 
{ register INT2 *data = gz7Da4KB(f3CK6csT());
if (data == NULL) return; midd_GetTime(data, data + 1, data + 2, data + 3);
Y8rugKNJ--; } static void qKIpoOYy() { register int H0Ng1GoT, y;
if (swDispMode == S_CM_GAME) return; mil_SaveGamma();
mil_Clear(SWAP_CR_CHAT_BACK); mil_DrawStringSolid(0, swLcdHeight - 12, SWAP_TEXT_CHAT_COM1,
S_FONT_LARGE, SWAP_CR_CHAT_COM, SWAP_CR_CHAT_COM_BACK, S_LEFT);
mil_DrawStringSolid(swLcdWidth, swLcdHeight - 12, SWAP_TEXT_CHAT_COM2,
S_FONT_LARGE, SWAP_CR_CHAT_COM, SWAP_CR_CHAT_COM_BACK, S_RIGHT);
for (H0Ng1GoT = SWAP_LEN_CHAT - 1, y = swLcdHeight - 14; H0Ng1GoT >= 0 && y >= 0; H0Ng1GoT--, y -= 13)
mil_DrawString(0, y - 12, CKOk9Z4Q[H0Ng1GoT], S_FONT_LARGE, SWAP_CR_CHAT_TEXT, S_LEFT);
midd_DrawLcd(swLcdBuffer); mil_RestoreGamma();
} static void q7YksxRB() 
{ sW4PB08i = (BYTE)e7mBDeVX[Y8rugKNJ--]; }
static void vHMCqW6t() 
{ if (e7mBDeVX[Y8rugKNJ--] != swDispMode) {
if (sW4PB08i == 1) return; if (swDispMode == S_CM_GAME) {
swDispMode = S_CM_CHAT; qKIpoOYy(); } else
swDispMode = S_CM_GAME; } }
static void TjNn2M0Y() 
{ JZagjmLi(); if (svRunScriptType == SWAP_STYPE_ALONE) { 
svResultReason = SWAP_RR_CONNECT_FROM_ALONE;
svResult.code = SWAP_RC_CONNECT; svResult.param.svr.reasons = svResultReason;
svResult.param.svr.ip = (char*)AI9TGVGd(Kh97ARZU());
svResult.param.svr.port = (WORD)f3CK6csT();
Y8rugKNJ -= 2; } else if (svRunScriptType == SWAP_STYPE_SERVER) { 
svResultReason = SWAP_RR_CONNECT_FROM_SVR;
svResult.code = SWAP_RC_CONNECT; svResult.param.svr.reasons = svResultReason;
svResult.param.svr.ip = (char*)AI9TGVGd(Kh97ARZU());
svResult.param.svr.port = (WORD)f3CK6csT();
Y8rugKNJ -= 2; } else { svResult.code = SWAP_RC_CONNECT_FAIL;
Y8rugKNJ -= 2; } BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void KrM62KC5() 
{ register int offset = Kh97ARZU(); register BYTE *data = (BYTE*)gz7Da4KB(offset);
if (data == NULL) return; 
#ifdef SWAP_FEATURE_NETWORK
*eLbFc93O = mil_SendData(data, f3CK6csT());

#endif 
Y8rugKNJ -= 2; } static void ZXyZWp1Y() 
{ register int ret; 
#ifdef SWAP_FEATURE_NETWORK
if (svRunScriptType == SWAP_STYPE_SERVER)
ret = mil_LoadMedia(Kh97ARZU(), f3CK6csT());
else 
#endif 
ret = 0; 
Y8rugKNJ--; f3CK6csT() = ret; } static void CjuCd32g() 
{ 
#ifdef SWAP_FEATURE_NETWORK
f3CK6csT() = mil_LoadMResult(f3CK6csT());

#else
f3CK6csT() = 0; 
#endif 
}
static void FsJBvgjH() 
{ register INT2 *data = gz7Da4KB(Kh97ARZU());
if (data == NULL) return; 
#ifdef SWAP_FEATURE_SERIAL
midd_RsCom((BYTE)U6liRwBT(), f3CK6csT(), (BYTE*)data);

#endif 
Y8rugKNJ -= 3; } static void qea48sUE() 
{ register INT2 *data = gz7Da4KB(Kh97ARZU());
if (data == NULL) return; 
#ifdef SWAP_FEATURE_SERIAL
midd_RsDownload(f3CK6csT(), (BYTE*)data);

#endif 
Y8rugKNJ -= 2; }
static void onL6RZdl() 
{ register WORD ti = (WORD)U6liRwBT(); register char *t70tQu1I = (char*)AI9TGVGd(Kh97ARZU());
register char *Dt5xkCCq = (char*)AI9TGVGd(f3CK6csT());
Y8rugKNJ -= 2; f3CK6csT() = midd_SendSMS(ti, t70tQu1I, Dt5xkCCq);
} static void Ysjq1WCT() 
{ svm_ResetAllTimers(); 
#ifdef SWAP_FEATURE_DOWNLOAD
scl_StoreRunInfo(svSSBInfo.UserID, AvwqZ1W4(), NP3y8tCu(), 
(char*)AI9TGVGd(mkfkxUSx()), U6liRwBT(), 
(char*)AI9TGVGd(Kh97ARZU()), f3CK6csT(), "");
svResult.code = SWAP_RC_DOWNLOAD; 
#endif 
Y8rugKNJ -= 6; BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void E4eNpcVh() 
{ svResult.code = SWAP_RC_BROWSER;
strcpy(svResultString, (char*)AI9TGVGd(e7mBDeVX[Y8rugKNJ]));
Y8rugKNJ--; BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void crwqZsLy() { svResult.code = SWAP_RC_EXEC_EXIT;
BT9PYMlQ = (BYTE*)&GICEruWy; 
}
static void FceYwV8I() 
{ 
#if (SWAP_AUDIO_TYPE == SWAP_AUDIO_BUZZER)
if (*AI9TGVGd(f3CK6csT()) != 0x00)
return; 
#elif (SWAP_AUDIO_TYPE == SWAP_AUDIO_MA1)
if (*AI9TGVGd(f3CK6csT()) != 0x01)
return; 
#elif (SWAP_AUDIO_TYPE == SWAP_AUDIO_MA2)
if (*AI9TGVGd(f3CK6csT()) != 0x02)
return; 
#elif (SWAP_AUDIO_TYPE == SWAP_AUDIO_CMX)
if (!(*AI9TGVGd(f3CK6csT()) == 0x03 || *AI9TGVGd(f3CK6csT()) == 0x04))
return; 
#elif (SWAP_AUDIO_TYPE == SWAP_AUDIO_MIDI)
if (*AI9TGVGd(f3CK6csT()) != 0x04)
return; 
#endif
midd_RegMusicBell((char*)AI9TGVGd(Kh97ARZU()), (BYTE*)AI9TGVGd(f3CK6csT()) + 2, ZgTiYWea(f3CK6csT()) - 2);
Y8rugKNJ -= 2; } static void U5SziUpL() 
{ midd_RegScreen((char*)AI9TGVGd(f3CK6csT()), swLcdBuffer);
Y8rugKNJ--; }
static void gmbZUMZC() 
{ mil_SetClip(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void ab5d9nqf() {
mil_ResetClip(); } static void q6BiTVgl() 
{ Kh97ARZU() = mil_GetPixel(Kh97ARZU(), f3CK6csT());
Y8rugKNJ--; } static void x2WABXLq() 
{ mil_DrawRectRound(NP3y8tCu(), mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 5; } static void ZYX6hJjK() 
{ mil_FillRectRound(NP3y8tCu(), mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 5; } static void KYxVJskX() 
{ mil_InvertRect(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void ZzZgAgiF() 
{ mil_ScrollLCD(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), f3CK6csT());
Y8rugKNJ -= 4; } static void oraWC1hf() 
{ mil_DrawString2(mkfkxUSx(), U6liRwBT(), (char*)AI9TGVGd(Kh97ARZU()), Y9HeE4kB, y8sIUNuW, VrJqCx8U, f3CK6csT());
Y8rugKNJ -= 4; } static void HKZxWVxZ() 
{ mil_DrawStringSolid2(mkfkxUSx(), U6liRwBT(), (char*)AI9TGVGd(Kh97ARZU()), Y9HeE4kB, y8sIUNuW, ZtFhCFq7, VrJqCx8U, f3CK6csT());
Y8rugKNJ -= 4; } static void K3Bu0Zgz() 
{ JZagjmLi(); y5TvOyEz = (int)U6liRwBT();
svResult.code = SWAP_RC_TEXTEDIT; svResult.param.edit.title = (char*)AI9TGVGd(mkfkxUSx());
if ((BYTE)f3CK6csT() != 0) strcpy((char*)AI9TGVGd(y5TvOyEz), "");
svResult.param.edit.input = (char*)AI9TGVGd(y5TvOyEz);
svResult.param.edit.maxlen = (BYTE)Kh97ARZU();
svResult.param.edit.strtype = (BYTE)f3CK6csT();
Y8rugKNJ -= 4; BT9PYMlQ = (BYTE*)&GICEruWy; 
}
static void ErBZRShh()
{ XW9UZcmx(Y8rugKNJ); e7mBDeVX[++Y8rugKNJ] = svRunScriptType;
} static void WflzIdQF() { JZagjmLi(); if (svSSBInfo.ScriptType == SWAP_STYPE_FREE && svRunScriptType == SWAP_STYPE_SERVER) {
svResultReason = SWAP_RR_DISCONNECT; svResult.code = SWAP_RC_DISCONNECT;
} else svResult.code = SWAP_RC_DISCONNECT_FAIL;
BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void qrHE0K2P() { JZagjmLi(); if (svRunScriptType == SWAP_STYPE_ALONE) {
svResult.code = SWAP_RC_VOICECALL;
strcpy(svResultString, (char*)AI9TGVGd(Kh97ARZU()));
svResult.param.call.ret = (BYTE)f3CK6csT();
Y8rugKNJ -= 2; } else { svResult.code = SWAP_RC_VOICECALL_FAIL;
Y8rugKNJ -= 2; } BT9PYMlQ = (BYTE*)&GICEruWy; 
}
static void ICwlUmIc()
{ JZagjmLi(); 
#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_ALONE) {
svResultReason = SWAP_RR_PTPCALL_FROM_ALONE;
svResult.code = SWAP_RC_PTPCALL; svResult.param.call.dial = (char*)AI9TGVGd(f3CK6csT());
Y8rugKNJ--; } else  
#endif 
{ svResult.code = SWAP_RC_PTPCALL_FAIL; Y8rugKNJ--;
} BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void hP6yA9qA() { JZagjmLi(); 
#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_ALONE) {
svResultReason = SWAP_RR_PTPCALL_FROM_ALONE;
svResult.code = SWAP_RC_PTPCALLMMI; } else

#endif 
svResult.code = SWAP_RC_PTPCALL_FAIL; BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void nKbQTG5U() { JZagjmLi(); 
#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_ALONE) {
svResultReason = SWAP_RR_PTPWAIT_FROM_ALONE;
svResult.code = SWAP_RC_PTPWAIT; } else 
#endif 
svResult.code = SWAP_RC_PTPCALL_FAIL; BT9PYMlQ = (BYTE*)&GICEruWy; 
} static void WD0g9dsU() { JZagjmLi(); 
#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_PTP) { svResultReason = SWAP_RR_PTPDISCONNECT;
svResult.code = SWAP_RC_PTPRELEASE; } else

#endif 
svResult.code = SWAP_RC_PTPRELEASE_FAIL; BT9PYMlQ = (BYTE*)&GICEruWy; 
}

#ifdef SWAP_FEATURE_DOWNLOAD
static void y5zPYZW7() 
{ SSBInfo ssbInfo; WORD cpid, gid; int H0Ng1GoT;
cpid = (WORD)AvwqZ1W4(); gid = (WORD)NP3y8tCu();
for (H0Ng1GoT = 0; H0Ng1GoT < SWAP_SSB_SIZE; H0Ng1GoT++) {
if (scl_CheckValidate(H0Ng1GoT)) { midd_GetScriptInfo(H0Ng1GoT, &ssbInfo);
if (ssbInfo.ScriptCPID == cpid &&  ssbInfo.ScriptID == gid) {
midd_DeleteScript(H0Ng1GoT); svSSBInfo.ScriptSize = 0;
break; } } } svm_ResetAllTimers(); 
#ifdef SWAP_FEATURE_DOWNLOAD
scl_StoreRunInfo(svSSBInfo.UserID, cpid, gid,
(char*)AI9TGVGd(mkfkxUSx()), U6liRwBT(), 
(char*)AI9TGVGd(Kh97ARZU()), f3CK6csT(), "");
svResult.code = SWAP_RC_DOWNLOAD; 
#endif 
Y8rugKNJ -= 6; BT9PYMlQ = (BYTE*)&GICEruWy; 
} 
#endif 
static void PeHtO9i2()
{ 
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)

#if (SWAP_AAD_TYPE == SWAP_AAD_G_723_1)
if (*AI9TGVGd(f3CK6csT()) != 0x10) 
return; 
#elif (SWAP_AAD_TYPE == SWAP_AAD_AAC)
if (*AI9TGVGd(f3CK6csT()) != 0x20) 
return; 
#elif (SWAP_AAD_TYPE == SWAP_AAD_MP3)
if (*AI9TGVGd(f3CK6csT()) != 0x30) 
return; 
#endif
midd_RegMusicBell((char*)AI9TGVGd(Kh97ARZU()), (BYTE*)AI9TGVGd(f3CK6csT()) + 2, ZgTiYWea(f3CK6csT()) - 2);

#endif 
Y8rugKNJ -= 2; } static void fzs6gcVR() {

#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)

#if (SWAP_AAD_TYPE == SWAP_AAD_G_723_1)
if (*AI9TGVGd(f3CK6csT()) != 0x10) 
return; 
#elif (SWAP_AAD_TYPE == SWAP_AAD_AAC)
if (*AI9TGVGd(f3CK6csT()) != 0x20) 
return; 
#elif (SWAP_AAD_TYPE == SWAP_AAD_MP3)
if (*AI9TGVGd(f3CK6csT()) != 0x30) 
return; 
#endif
midd_PlayAdvAudio((BYTE*)AI9TGVGd(f3CK6csT()), ZgTiYWea(f3CK6csT()));

#endif 
Y8rugKNJ--; } static void UFp6IScG() { 
#if (SWAP_AAD_TYPE > SWAP_AAD_NONE)
switch (f3CK6csT()) { case 0: midd_PauseAdvAudio();
break; case 1: midd_RestartAdvAudio(); break;
case 2: midd_StopAdvAudio(); break; } 
#endif 
Y8rugKNJ--; }
static void nJZLqpvm()
{ register INT2 ret; ret = midd_ReadHandsetData(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), (BYTE*)AI9TGVGd(f3CK6csT()));
Y8rugKNJ -= 3; f3CK6csT() = ret; } static void as6KHubY()
{ register INT2 ret; ret = midd_WriteHandsetData(mkfkxUSx(), U6liRwBT(), Kh97ARZU(), (BYTE*)AI9TGVGd(f3CK6csT()));
Y8rugKNJ -= 3; f3CK6csT() = ret; }
static void XGsB5xvE() 
{ INT2 CarrierCode = AvwqZ1W4(); INT2 ApiCode = NP3y8tCu();
INT2 Param1 = mkfkxUSx(); INT2 Param2 = U6liRwBT();
INT2 Param3 = Kh97ARZU(); INT2 Param4 = f3CK6csT();
Y8rugKNJ -= 6; 
#if (SWAP_CARRIER == SWAP_CARRIER_SKT)
if (CarrierCode == SWAP_CARRIER_SKT) { switch (ApiCode) {
case 0: 
midd_SetSoMo((WORD)Param1, (WORD)Param2);
break; } } 
#elif (SWAP_CARRIER == SWAP_CARRIER_STI)
if (CarrierCode == SWAP_CARRIER_STI) { } 
#elif (SWAP_CARRIER == SWAP_CARRIER_TUKA)
if (CarrierCode == SWAP_CARRIER_TUKA) { }

#elif (SWAP_CARRIER == SWAP_CARRIER_DDIP)
if (CarrierCode == SWAP_CARRIER_DDIP) { }

#endif
}
static void (*KPPUdUrc[])(void) = {
&aDdby7o9, 
&mYDvYy0a, 
&mVOxnEmw, 
&b8heDkBn, 
&Fiyr9w3x, 
&UsjwID4x, 
&EEF7iunr, 
&Xgnuagnt, 
&OjejimrN, 
&xWYbAHVO, 
&Ge9ypBEE, 
&GsgfrHnu, 
&ZYj9qvyb, 
&B0oOslTf, 
&SPvM1Gb8, 
&EOI4B9jZ, 
&JZLJSz2M, 
&oEnAig1J, 
&TmtaAsPv, 
&Cw9bZk4k, 
&FJweE8VK, 
&GBecIdEn, 
&dFi5bO99, 
&boNcuwqS, 
&apHGYOU7, 
&jYPKuxnw, 
&kkNN5Ev6, 
&iW0pdjQP, 
&pohX7Er3, 
&jGtO1FB9, 
&IOd5DYzC, 
&VJ9k9RQN, 
&M1P1NHy9, 
&SZysP4qw, 
&inTEQRbH, 
&PaCZSuFD, 
&RwVPjzxH, 
&wXY67Rf3, 
&s7C8SZZs, 
&k0MQayom, 
&HRCKFYTf, 
&O15IeEKn, 
&fthEy4WO, 
&XPxhpuKc, 
&eTx6s9W0, 
&CpPc5HRj, 
&EccKMVci, 
&W1lcDjm6, 
&mhLcYuKT, 
&Fb7QjkfV, 
&dSet7rec, 
&YN5px9wH, 
&HzrKAV9w, 
&weUuaSVW, 
&UVOMl6I0, 
&JhgEoEOk, 
&JxdvQVIg, 
&iiaR1Qyh, 
&H5h5hXcT, 
&JRIvsd2n, 
&BCu736FU, 
&k3ux4fDW, 
&YT8oq9M8, 
&YX6WZJ9k, 
&uXtW6Xl8, 
&I8XDgmy2, 
&M0dimU4G, 
&HjvlX3r5, 
&PlsvbI8t, 
&tWA94Aka, 
&wwIRCwE5, 
&NchisNIj, 
&lhNFI5Ex, 
&kSeyq5l8, 
&uoQaTMb8, 
&EW6LVBlK, 
&tyJDygB4, 
&k2ax889Q, 
&OWsyYiyk, 
&qlGFWhKG, 
&b80baDCo, 
&reI5L8za,
&nsZP3ml5, &bdnHdpxf, &KjJA0KYA, &mil_ClearWhite, 
&mil_ClearBlack,  &ZOq5qzwQ,  &yN7fykzl, 
&ysPpvW0m,  &wxQOsShb, 
&H1sBAY04, &oJWdHhFi, &i3tjYRQj, &xc9U5L3x, 
&v9AXzz0Z,  &JqAuamKe,  &fcQTGqqD,  &caJazpv1, 
&YNzmk8Mt,  &aoCoeMc6, 
&dpv7Xqnw,  &n4emhOcH, &JTgvU2RB, &Btd8PqaC,
&Jy2VajCp, &RVhGK7nm,  &wdYZBfxC,  &bjupxEy9, 
&Zi0BnxOT,  &CzyMA07Z, 
&AQ4qIfP0,  &hnClyY4g,  &O811fHpi,  &Z8Gi7fdg,
&mil_InitDQ,  &FbTnNFAq,  &V0N3MW7L,  &E5oieFg5, 
&z2QhsGng,  &FpIq8p4F, 
&FQF2EFY9,  &QNJ3ZSiJ,  &mgRKuK6x,  &GVi6vy3B, 
&vRZkPo25,  &hMH3WsiT, &QhkGDqQp,  &OOEgS7ee, 
&dRZl7pCZ,  &nFN2dtzj, 
&A7uvuQlI, &LmmADihB, &x8GCfooj, &drSwT6Gs,
&KGRTuzlW, &z48637g9, &jynRrmeb, &Iiir6kHM, 
&zl43W2TJ,  &Tf8yQ77b, 
&vVP7jCTx,  &Hx3dIuDA,  &abSzvvHf,  &ZiXpvN3k, 
&midd_StopSound, &zroUNpSW,  &midd_StopVocoder,
&zwMe7uTc,  &midd_StopVib,  &R0C2PjBV, 
&Fju2VHHp, &ufCNKLtK, &wPfkxgQc, &LA9zIClI,
&RmhvEPkn, &cdSPy35v, &SVdgQk7R, &LukcFCrd,
&rinMobWi, &R823MiOq, 
&hA1H4K56,  &na0XNBCg,  &CiOLuoM0,  &h3Xp5zzX, 
&mRHSnFp1,  &JWyRjw0d,  &DMqJ0k0h,  &s3wzcxdl, 
&GR8qJMC3,  &ftfjUT53, 
&JwIur34E,  &whFcpWZ2,  &QsIeLzu2,  &xgVzvYXt, 
&RGI6Le21,  &gFu09gz6,  &bDA1TYkC,  &RF4rjaDK, 
&TRUeJF5p,  &qrCMdwBc, 
&y7yEY3Hf, &t9iTtCgu, &lnpRbNNp,  &XWZgUjtk, 
&uH57ZEhU,  &q7YksxRB,  &vHMCqW6t,  &TjNn2M0Y,
&KrM62KC5, &ZXyZWp1Y, 
&CjuCd32g, &FsJBvgjH,  &qea48sUE,  &onL6RZdl, 
&Ysjq1WCT,  &E4eNpcVh, &crwqZsLy,  &FceYwV8I,
&U5SziUpL, 
&gmbZUMZC, 
&ab5d9nqf, &q6BiTVgl, &x2WABXLq, &ZYX6hJjK,
&KYxVJskX, &ZzZgAgiF, &oraWC1hf, &HKZxWVxZ,
&K3Bu0Zgz, &ErBZRShh, 
&TjNn2M0Y, &WflzIdQF, &qrHE0K2P, &ICwlUmIc,
&hP6yA9qA, &nKbQTG5U, &WD0g9dsU, &y5zPYZW7,
&PeHtO9i2, &fzs6gcVR, 
&UFp6IScG, &nJZLqpvm, &as6KHubY, &XGsB5xvE, 
};
static int rcBoAfdC()
{ BYTE *MFItZr5T; 
S_SymElem *swRomSymTable; 
S_SymElem *YUzoAHpX; 
S_RamSymElem *b7vWuxr0; 
INT2 *XvZi1MCG; 
INT2 *iXRHZF49; 
S_MediaElem *swRomMediaTable; 
S_MediaElem *X6nFNa8X; 
S_RamMediaElem *MqdqdZw3; 
BYTE *fULSQauW; 
BYTE *uIHssIxh; 
int H0Ng1GoT; nSub0nar = (S_HeaderType*)swScriptBuffer;
swRomSymTable = (S_SymElem*)(swScriptBuffer + nSub0nar->SymTable);
FZRf2WUX = (INT2*)(swScriptBuffer + nSub0nar->SymPool);
CQHSxkW0 = ((BYTE*)FZRf2WUX - (BYTE*)swRomSymTable)/sizeof(S_SymElem);
swRomMediaTable = (S_MediaElem*)(swScriptBuffer + nSub0nar->MediaTable);
QU4IcLFB = (BYTE*)(swScriptBuffer + nSub0nar->MediaPool);
JG14eUey = ((BYTE*)QU4IcLFB - (BYTE*)swRomMediaTable)/sizeof(S_MediaElem);
MFItZr5T = swWorkSpace;
HAvJVeND = (S_RamSymElem*)MFItZr5T; 
MFItZr5T += CQHSxkW0*sizeof(S_RamSymElem); 
swRamMediaTable = (S_RamMediaElem*)MFItZr5T; 
MFItZr5T += JG14eUey*sizeof(S_RamMediaElem);
bmE4Gwyt = (INT2*)MFItZr5T; YUzoAHpX = swRomSymTable; 
XvZi1MCG = FZRf2WUX; 
b7vWuxr0 = HAvJVeND;
iXRHZF49 = bmE4Gwyt; for (H0Ng1GoT = 0; H0Ng1GoT < CQHSxkW0; H0Ng1GoT++) { 
b7vWuxr0->dim = YUzoAHpX->dim; 
if (YUzoAHpX->var == uW9Kdys6) { 
b7vWuxr0->var = uW9Kdys6; b7vWuxr0->addr = XvZi1MCG; 
XvZi1MCG += YUzoAHpX->dim; } else { 
if ((BYTE*)(iXRHZF49 + YUzoAHpX->dim) > swWorkSpace + SWAP_MAX_WORKSPACE)
return 0; 
b7vWuxr0->var = Tsbk5B54; b7vWuxr0->addr = iXRHZF49; 
if (YUzoAHpX->init) { memcpy(iXRHZF49, XvZi1MCG, 2*YUzoAHpX->dim);
XvZi1MCG += YUzoAHpX->dim; } else { memset(iXRHZF49, 0, 2*YUzoAHpX->dim);
} iXRHZF49 += YUzoAHpX->dim; } YUzoAHpX++;
b7vWuxr0++; } M7oSEwdG = XvZi1MCG; mRc49kNz = iXRHZF49;
jzpVRmy0 = (M7oSEwdG - FZRf2WUX); mSAJ4wdn = (mRc49kNz - bmE4Gwyt);

#ifdef _DEBUG
int stsize = CQHSxkW0*sizeof(S_RamSymElem);
TRACE2("RAM SYMBOL TABLE: %4d bytes (%d elements)\n", stsize, CQHSxkW0);
int spsize = (BYTE*)iXRHZF49 - (BYTE*)bmE4Gwyt;
TRACE1("RAM SYMBOL POOL : %4d bytes\n", spsize);

#endif 
MFItZr5T = (BYTE*)iXRHZF49;
u4T8Qmc6 = (BYTE*)MFItZr5T; 
X6nFNa8X = swRomMediaTable; 
fULSQauW = QU4IcLFB; 
MqdqdZw3 = swRamMediaTable; 
uIHssIxh = u4T8Qmc6;  for (H0Ng1GoT = 0; H0Ng1GoT < JG14eUey; H0Ng1GoT++) { 
MqdqdZw3->type = X6nFNa8X->type; 
MqdqdZw3->size = X6nFNa8X->size; 
if (X6nFNa8X->var == uW9Kdys6) { 
MqdqdZw3->var = uW9Kdys6; MqdqdZw3->addr = fULSQauW; 
fULSQauW += X6nFNa8X->size; } else { 
if (uIHssIxh + X6nFNa8X->size > swWorkSpace + SWAP_MAX_WORKSPACE)
return 0; 
MqdqdZw3->var = Tsbk5B54; MqdqdZw3->addr = uIHssIxh; 
memcpy(uIHssIxh, fULSQauW, X6nFNa8X->size);
fULSQauW += X6nFNa8X->size; uIHssIxh += X6nFNa8X->size;
} X6nFNa8X++; MqdqdZw3++; } WrSRS9NB = fULSQauW;
YTwPrBKb = uIHssIxh; 
#ifdef _DEBUG
int mtsize = JG14eUey*sizeof(S_RamMediaElem);
TRACE2("RAM MEDIA TABLE : %4d bytes (%d elements)\n", mtsize, JG14eUey);
int mpsize = (BYTE*)uIHssIxh - (BYTE*)u4T8Qmc6;
TRACE1("RAM MEDIA POOL  : %4d bytes\n", mpsize);
int tsize = stsize + spsize + mtsize + mpsize;
TRACE2("RAM SIZE TOTAL  : %4d bytes (%4.2f Kbytes)\n", tsize, (float)tsize/1024);

#endif 
eLbFc93O = HAvJVeND[V_DATA].addr;
*eLbFc93O = 0; p0bvn9mb = HAvJVeND[V_WIDTH].addr;
*p0bvn9mb = swLcdWidth; w1ZW3mTZ = HAvJVeND[V_HEIGHT].addr;
*w1ZW3mTZ = swLcdHeight; zwjAPDYU = HAvJVeND[V_FRAME].addr;
*zwjAPDYU = 0; mMDSsdVr = HAvJVeND[V_FRAME2].addr;
*mMDSsdVr = 0; I2bmJWPw = HAvJVeND[V_FRAME3].addr;
*I2bmJWPw = 0; FmJKOy5y = HAvJVeND[V_FRAME6].addr;
*FmJKOy5y = 0; swRcvBuf = HAvJVeND[V_RCVBUF].addr;
swRsRcvBuf = HAvJVeND[V_RSRCVBUF].addr; strcpy((char*)swRsRcvBuf, SWAP_HANDSET_NAME);
for (H0Ng1GoT = 0; H0Ng1GoT < SWAP_LEN_CHAT; H0Ng1GoT++)
CKOk9Z4Q[H0Ng1GoT][0] = '\0'; mil_SetColor(S_BLACK);
Y9HeE4kB = S_FONT_LARGE; y8sIUNuW = S_BLACK;
ZtFhCFq7 = S_WHITE; VrJqCx8U = S_LEFT; DFbjS7T4[0] = 0;
DFbjS7T4[1] = 0; DFbjS7T4[2] = 0; EOeyaIK2[0] = 0;
EOeyaIK2[1] = 0; EOeyaIK2[2] = 0; swDispMode = S_CM_GAME;
sW4PB08i = (svRunScriptType == SWAP_STYPE_ALONE) ? 1 : 0;
mil_SetColor(S_BLACK); return 1; } 
#ifdef _DEBUG

#endif
static void tDwPyRCh(WORD Offset)
{ BYTE dziitHPy; if (Offset == 0) return;
Y8rugKNJ = -1; lhoxsk7M = -1; BT9PYMlQ = swScriptBuffer + Offset;
while ((dziitHPy = *BT9PYMlQ++) < b5Lr0cwi) {
KPPUdUrc[dziitHPy](); } if (dziitHPy != 0xFF) 
s_Exception(SWAP_ECPT_INVALID_OP); }
void svm_StartGame()
{
if (rcBoAfdC() == 0)
s_Exception(SWAP_ECPT_SYM_OVER); else { 
*eLbFc93O = svRunScriptType;
tDwPyRCh(nSub0nar->StartEntry); } }
void svm_EndGame()
{
swDispMode = S_CM_GAME;
tDwPyRCh(nSub0nar->EndEntry);
svm_ResetAllTimers(); midd_StopSound(); midd_StopVocoder();
midd_StopVib(); }
void svm_TimeOut(int id)
{
if (EOeyaIK2[id] == 0)
DFbjS7T4[id] = 0; *eLbFc93O = id;
if (id == 0) {
*zwjAPDYU = (*zwjAPDYU + 1)%5040; *mMDSsdVr = *zwjAPDYU%2;
*I2bmJWPw = *zwjAPDYU%3; *FmJKOy5y = *zwjAPDYU%6;
} mil_SetBlinkColor(*FmJKOy5y); tDwPyRCh(nSub0nar->TimeoutEntry);
}
void svm_KeyPress(int Key)
{ 
#ifdef SWAP_FEATURE_NETWORK
if (Key == SWAP_KEY_SND && sW4PB08i == 0 && svRunScriptType != SWAP_STYPE_ALONE) 
{ if (swDispMode == S_CM_GAME) { swDispMode = S_CM_CHAT;
qKIpoOYy(); } else { swDispMode = S_CM_GAME;
svm_Result(SWAP_RST_INVALID_LCD); } }
else if (swDispMode == S_CM_GAME) {

#endif 
*eLbFc93O = Key; tDwPyRCh(nSub0nar->KeyEntry);

#ifdef SWAP_FEATURE_NETWORK
}
else {
if (SWAP_KEY_1 <= Key && Key <= SWAP_KEY_9) {
y5TvOyEz = -1; mil_SendChat((char*)svRsvdChat[Key - SWAP_KEY_1]);
} else if (Key == SWAP_KEY_OK || Key == SWAP_KEY_0) { 
JZagjmLi(); y5TvOyEz = -1; svResult.code = SWAP_RC_TEXTEDIT;
svResult.param.edit.title = SWAP_TEXT_CHAT_TITLE;
svResult.param.edit.input = ""; svResult.param.edit.maxlen = SWAP_LEN_TEXT;
svResult.param.edit.strtype = 0; } } 
#endif 
}
void svm_PauseGame()
{ if (svRunScriptType != SWAP_STYPE_ALONE)
return; JZagjmLi(); midd_StopSound(); midd_StopVocoder();
midd_StopVib(); }
void svm_ContinueGame()
{ if (svRunScriptType != SWAP_STYPE_ALONE)
return; midd_DrawLcd(swLcdBuffer); *eLbFc93O = SWAP_RST_CONTINUE;
tDwPyRCh(nSub0nar->ResultEntry); nmRH4auG();
}
void svm_ReceiveData(int Length)
{
*eLbFc93O = Length;
tDwPyRCh(nSub0nar->ReceiveEntry); }
void svm_Result(int B1Z4oC1s)
{
*eLbFc93O = B1Z4oC1s;
tDwPyRCh(nSub0nar->ResultEntry); if (B1Z4oC1s <= 2 || 9 <= B1Z4oC1s)
nmRH4auG(); }
void svm_SetText(char *str)
{ if (y5TvOyEz >= 0) { 
midd_DrawLcd(swLcdBuffer);
if (svm_SetMediaSize(y5TvOyEz, strlen(str) + 1))
strcpy((char*)AI9TGVGd(y5TvOyEz), str); svm_Result(SWAP_RST_TEDIT_DONE);
} 
#ifdef SWAP_FEATURE_NETWORK
else { 
if (str == NULL || str[0] == '\0') qKIpoOYy();
else mil_SendChat(str); nmRH4auG(); } 
#endif 
}
void svm_ReceiveChat(char *chat)
{ char *T96CI1uh = &CKOk9Z4Q[SWAP_LEN_CHAT - 1][0];
int AJqLAOin, gh3BT4ts = 0; int width = swLcdWidth/6;
while (gh3BT4ts < (int)strlen(chat)) { memmove(CKOk9Z4Q[0], CKOk9Z4Q[1], (SWAP_LEN_CHAT - 1)*(SWAP_LEN_CHAT_TEXT + 1));
if (gh3BT4ts == 0) AJqLAOin = 0; else { T96CI1uh[0] = ' ';
AJqLAOin = 1; } while (AJqLAOin < width && chat[gh3BT4ts] != '\0') {
if (chat[gh3BT4ts] & 0x80) { if (AJqLAOin < width - 1) {
T96CI1uh[AJqLAOin++] = chat[gh3BT4ts++]; T96CI1uh[AJqLAOin++] = chat[gh3BT4ts++];
} else break; } else { T96CI1uh[AJqLAOin++] = chat[gh3BT4ts++];
} } T96CI1uh[AJqLAOin] = '\0'; } qKIpoOYy();
}
void scl_RsEvent(int code, int size, BYTE *data)
{ memcpy(swRsRcvBuf, data, size); *eLbFc93O = code;
tDwPyRCh(nSub0nar->RSEventEntry); }
void scl_RsUpload(int size, BYTE *data)
{ memcpy(swRsRcvBuf, data, size); *eLbFc93O = size;
tDwPyRCh(nSub0nar->RSReceiveEntry); } 