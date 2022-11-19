//==============================================================================
//
//	SINJISOFT SWAP MODULE LIBRARY
//
//	FILE NAME
//		MILNet.cpp
//
//	DESCRIPTION
//		Mobile Interface Library Network Part
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

#include <string.h>

#include "SVM.h"

#ifdef _WIN32

#include "..\WGEmul.h"

#include "..\WGEmulDlg.h"

#include "..\MMI\MMI.h"

#endif 

#ifdef SWAP_FEATURE_NETWORK

#define joIosVQ1 'S'

#define QsZBfaXf 'G'

#define xPfn7LCK 'F'

#define ulIgzpum 'C' 

#define ukJoStXU 'c' 

#define VGMutz5I 'M' 

#define fNuXUKcl 'A' 

#define mrguskuo 'a' 

#define kpRdC5dM 'Q' 

#define bkaqULUC 'D' 

#define vXrprofN 'R' 

#ifdef SWAP_DOWNLOAD_SNDERR

#define MRaPnQQi 'N' 

#endif

#define t4qzaYHw 'G'

#define mmqiZxi7 'D' 

#define jPVrKFZz 'C' 

#define uwVq9OeB 'M' 

#define t7fE8lgK 'c' 

#define i2NrRJiB 'm' 

#define RoLcx5Rp 'A' 

#define CCyNC48Y 'a' 

#define oCa3i5tg 'Q' 

#define r23FchhA 'M' 

#define HTiKEkBK 'R' 

#define zH7wUCjQ 'X' 

#define S0wnz7JD(b, H0Ng1GoT, n) { b[H0Ng1GoT] = (n) >> 24; b[(H0Ng1GoT) + 1] = ((n) >> 16) & 0xFF; b[(H0Ng1GoT) + 2] = ((n) >> 8) & 0xFF; b[(H0Ng1GoT) + 3] = (n) & 0xFF; }

#define JK0JqFmQ(b, H0Ng1GoT, n) { b[H0Ng1GoT] = (n) >> 8; b[(H0Ng1GoT) + 1] = (n) & 0xFF; }

#define HWpGyAeb(b, H0Ng1GoT) ((b[H0Ng1GoT] << 24) | (b[(H0Ng1GoT) + 1] << 16) | (b[(H0Ng1GoT) + 2] << 8) | b[(H0Ng1GoT) + 3])

#define A4DyGYLk(b, H0Ng1GoT) ((b[H0Ng1GoT] << 8) | b[(H0Ng1GoT) + 1])

#define H1G8XoEy(b, H0Ng1GoT) ((b[(H0Ng1GoT) + 1] << 8) | b[H0Ng1GoT])
enum _RcvState { SWAP_NET_HUNT, SWAP_NET_MAGIC,
SWAP_NET_SESSION, SWAP_NET_TYPE, SWAP_NET_LENGTH_H,
SWAP_NET_LENGTH_L, SWAP_NET_DATA, SWAP_NET_SUM
};
static BYTE cXsO2ZWP[SWAP_MAX_RCVBUF];
static WORD fVWtP902; static BYTE nYHkffJe = SWAP_NET_HUNT;
static INT2 UL1j91DY = -1; static WORD ZKd4bSEe;
static WORD KBiBLRsS; static WORD zTQdDEMy;
static WORD n8sPwEQZ; static INT2 yB88FzrU; 
ReqInfo svReqInfo; 

#ifdef SWAP_FEATURE_DOWNLOAD
static WORD YyRteHno; 
static WORD obd3rXH3; 
static WORD xRSAt1IX; 
static WORD Ae3zrUtq; 
static BYTE NGBqi8R2; 
#ifdef SWAP_DOWNLOAD_PKTRETRY
static BYTE s_DlRetryReason;
#define SWAP_RETRYREASON_TIMEOUT		0
#define SWAP_RETRYREASON_RRQ_SNDFAIL	1
#define SWAP_RETRYREASON_PKTSKIP		2
#endif
static const BYTE Swn9p038[256] = {
0x8C, 0x05, 0x3E, 0x0F, 0x30, 0xAD, 0x72, 0xAB, 0xA4, 0x15, 0x86, 0x97, 0x18, 0xF9, 0x66, 0x6B, 
0x14, 0x9D, 0xAE, 0x2F, 0x20, 0x41, 0x82, 0x23, 0x34, 0xA5, 0x82, 0x47, 0x80, 0xF9, 0xAA, 0x5B, 
0x6C, 0x69, 0x8A, 0xCF, 0x88, 0xB1, 0x32, 0x63, 0x04, 0xE5, 0x36, 0x57, 0xB4, 0xA5, 0xBA, 0x6F, 
0x58, 0x39, 0x5A, 0xCB, 0x4C, 0x4D, 0xD2, 0x3B, 0x6C, 0x7D, 0x5E, 0xDF, 0xA0, 0xF1, 0xC2, 0x5F, 
0x1C, 0x05, 0x0E, 0x4F, 0x80, 0x21, 0xD2, 0x03, 0x94, 0xE5, 0xD6, 0xB3, 0x74, 0xB9, 0x32, 0x1B, 
0xD8, 0x7D, 0xBE, 0xAF, 0x60, 0x81, 0xE2, 0xCF, 0xF4, 0xA5, 0x76, 0xE7, 0x78, 0x29, 0x7A, 0x3B, 
0x68, 0x29, 0x2E, 0xC7, 0xB8, 0xF9, 0x66, 0x43, 0xF4, 0x85, 0x72, 0xC7, 0xF8, 0x89, 0x0A, 0x7B, 
0x9C, 0x4D, 0x3E, 0xFF, 0x20, 0xDD, 0xC2, 0xC3, 0xC4, 0x81, 0xA6, 0xC7, 0x48, 0x89, 0xDA, 0xCB, 
0xAC, 0x0D, 0xEA, 0x4F, 0x50, 0xCD, 0xA2, 0xAB, 0x24, 0x95, 0x96, 0x77, 0xF8, 0x39, 0x36, 0xFB, 
0x74, 0x79, 0x7C, 0xC1, 0x7A, 0x63, 0x4C, 0x9D, 0xA8, 0xC3, 0xB4, 0xC9, 0xB6, 0x4B, 0xBC, 0x75, 
0x82, 0x2D, 0x94, 0x71, 0x66, 0xAF, 0x32, 0x1F, 0xC2, 0x97, 0x10, 0x75, 0x36, 0xCB, 0x1E, 0x79, 
0x8A, 0xDF, 0xCC, 0x61, 0xD2, 0x8B, 0x98, 0x43, 0xAA, 0x83, 0x84, 0xD5, 0x74, 0xD5, 0x18, 0x6D, 
0x66, 0x8B, 0x5C, 0x01, 0xF4, 0x0F, 0xA0, 0xF5, 0xE2, 0x77, 0xE8, 0xA1, 0xAE, 0x59, 0xC0, 0x99, 
0x9A, 0xEF, 0x56, 0xF5, 0x5E, 0x63, 0x28, 0x7F, 0x30, 0xB3, 0x84, 0x85, 0xB6, 0x77, 0x1C, 0xFF, 
0xEA, 0x3B, 0x90, 0xFD, 0x9E, 0xFF, 0x74, 0xF9, 0xF4, 0x43, 0x58, 0xD1, 0xAE, 0x7B, 0x60, 0xB9, 
0xCA, 0x19, 0xCC, 0x9D, 0x62, 0x47, 0x34, 0x29, 0x14, 0x69, 0x06, 0x5F, 0xF0, 0xD1, 0x7E, 0xD7
};
static WORD H1eVDWXU(BYTE nVer, WORD nCpid)
{ WORD authKey0[3], authKey1[3], authKey;
authKey0[0] = nVer & 0xFF; authKey0[1] = (nCpid >> 8) & 0xFF;
authKey0[2] = nCpid & 0xFF; authKey1[0] = Swn9p038[(0x100 + authKey0[1] - authKey0[2]) & 0xFF];
authKey1[1] = Swn9p038[(authKey0[2] + authKey0[0]) & 0xFF];
authKey1[2] = Swn9p038[(authKey0[0] * authKey0[1]) & 0xFF];
authKey0[0] = authKey1[2] * Swn9p038[authKey1[1]];
authKey0[1] = authKey1[0] * ~Swn9p038[authKey1[2]];
authKey0[2] = authKey1[1] * Swn9p038[authKey1[0]];
authKey = ((authKey0[0] << 12) | (authKey0[1] << 4) | (authKey0[2] >> 4)) & 0xFFFF;
return authKey; } static WORD EeRR3hHy(WORD nGid, WORD nKey)
{ WORD authValue0[4], authValue1[4], authValue;
authValue0[0] = (nGid >> 8) & 0xFF; authValue0[1] = nGid & 0xFF;
authValue0[2] = (nKey >> 8) & 0xFF; authValue0[3] = nKey & 0xFF;
authValue1[0] = Swn9p038[(0x100 + authValue0[1] - authValue0[2]) & 0xFF];
authValue1[1] = Swn9p038[(authValue0[2] + authValue0[0]) & 0xFF];
authValue1[2] = Swn9p038[(authValue0[3] * authValue0[1]) & 0xFF];
authValue1[3] = Swn9p038[(nKey * authValue0[0]) & 0xFF];
authValue0[0] = (authValue1[3] * Swn9p038[authValue1[2]]) & 0xFF;
authValue0[1] = (authValue1[0] * ~Swn9p038[authValue1[3]]) & 0xFF;
authValue0[2] = (authValue1[1] * Swn9p038[authValue1[0]]) & 0xFF;
authValue0[3] = (authValue1[2] * ~Swn9p038[authValue1[1]]) & 0xFF;
authValue = ((authValue0[0]^authValue0[2]) << 8) | (authValue0[1]^authValue0[3]) &0xFFFF;
return authValue; }
static int tHTO6ObW(WORD TB4GUKNg)
{ BYTE sndBuf[9]; sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = xPfn7LCK; sndBuf[3] = kpRdC5dM;
JK0JqFmQ(sndBuf, 4, 2); 
JK0JqFmQ(sndBuf, 6, TB4GUKNg); 
sndBuf[8] = sndBuf[6] + sndBuf[7]; 
return midd_SendData(sndBuf, 9); }
static void FUvKrLFp()
{ BYTE sndBuf[7]; sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = xPfn7LCK; sndBuf[3] = vXrprofN;
JK0JqFmQ(sndBuf, 4, 0); 
sndBuf[6] = 0; 
midd_SendData(sndBuf, 7); }
static int P0sDgg0A(BYTE *data, int len)
{ DWORD size, lack; WORD checksum; BYTE buf[SWAP_USERNV_SIZE];

#ifndef SWAP_DOWNLOAD_SPEC2 
size = A4DyGYLk(data, 0); checksum = A4DyGYLk(data, 2);

#else 
size = HWpGyAeb(data, 0); checksum = A4DyGYLk(data, 4);

#endif
if (size > SWAP_MAX_SCRIPT_SIZE || size == 0)
return SWAP_RCV_DLSIZEERR; lack = midd_CheckFreeSSB(size, &svSSBIndex);
if (svSSBIndex == -1) { 
svResult.param.ssb.size = size; 
svResult.param.ssb.lack = lack; 
svResult.param.ssb.index = -1; return SWAP_RCV_DLSSBERR;
} if (lack > 0) { svResult.param.ssb.size = size; 
svResult.param.ssb.lack = lack; 
svResult.param.ssb.index = svSSBIndex; return SWAP_RCV_DLMEMERR;
} if (svSSBInfo.ScriptCPID != svReqInfo.ScriptCPID ||
svSSBInfo.ScriptID != svReqInfo.ScriptID ||
svSSBInfo.ScriptSize != size || YyRteHno != checksum)
{ strcpy(svSSBInfo.UserID, svReqInfo.UserID);
svSSBInfo.ScriptCPID = svReqInfo.ScriptCPID;
svSSBInfo.ScriptID = svReqInfo.ScriptID; strcpy(svSSBInfo.SvrIP, svReqInfo.SvrIP);
svSSBInfo.SvrPort = svReqInfo.SvrPort; svSSBInfo.ScriptSize = size;
strcpy(svSSBInfo.ScriptName, SWAP_TEXT_DL_NONAME);
memset (buf, 0, 64);
midd_PutUserNV(svSSBIndex, buf, 64); YyRteHno = checksum;
obd3rXH3 = 0; xRSAt1IX = 0; } mil_DrawProg(xRSAt1IX*SWAP_BLOCK_SIZE, size, 0);
#ifdef SWAP_DOWNLOAD_PKTRETRY
if (!tHTO6ObW(xRSAt1IX)) return SWAP_RCV_PROCFAIL;
#else
tHTO6ObW(xRSAt1IX);
#endif
return SWAP_RCV_NONE;
}
static int Y1UuJ8mM(int year, int month)
{ int mday[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
if (month == 2 && year%4 == 0 && year%100 != 0)
return 29; return mday[month - 1]; } static void QaKStTtt(int days, BYTE term[3])
{ INT2 year, month, day, wday; int mday; midd_GetDate(&year, &month, &day, &wday);
mday = Y1UuJ8mM(year, month) - day + 1; 
while (days > 0) { if (days < mday) { day += days;
days = 0; } else { days -= mday; month++;
if (month == 13) { year++; month = 1; } day = 1;
mday = Y1UuJ8mM(year, month); 
} } term[0] = (BYTE)(year - 2000); term[1] = (BYTE)month;
term[2] = (BYTE)day; }
static int V8c05IaJ(BYTE *data, int len)
{ S_HeaderType *header; WORD cpid; WORD gid;
WORD TB4GUKNg; int H0Ng1GoT; TB4GUKNg = A4DyGYLk(data, 0);
mil_DrawProg(TB4GUKNg*SWAP_BLOCK_SIZE, svSSBInfo.ScriptSize, 0);
#ifdef SWAP_DOWNLOAD_PKTRETRY
s_DlRetryReason = SWAP_RETRYREASON_TIMEOUT;
#endif
if (TB4GUKNg == 0) { 
header = (S_HeaderType*)&data[2]; svSSBInfo.ScriptVer = header->ScriptVer;
svSSBInfo.ImageType = header->ImageType; 
#if (SWAP_LCD_CLASS == SWAP_LCD_CLASS_0)
if (!(header->LcdClass & SWAP_LCD_CLASS_0))
return SWAP_RCV_DLSCRERR; 
#elif (SWAP_LCD_CLASS == SWAP_LCD_CLASS_1)
if (!(header->LcdClass & SWAP_LCD_CLASS_0) &&
!(header->LcdClass & SWAP_LCD_CLASS_1))  return SWAP_RCV_DLSCRERR;

#elif (SWAP_LCD_CLASS == SWAP_LCD_CLASS_2)
if (!(header->LcdClass & SWAP_LCD_CLASS_0) &&
!(header->LcdClass & SWAP_LCD_CLASS_1) &&
!(header->LcdClass & SWAP_LCD_CLASS_2))  return SWAP_RCV_DLSCRERR;

#endif 

#if defined(SWAP_LCD_BW) || defined(SWAP_LCD_GRAY)
if (!(header->ImageType & 0x02))
return SWAP_RCV_DLSCRERR; 
#endif 
if (!(header->AudioType & SWAP_AUDIO_TYPE))
return SWAP_RCV_DLSCRERR; svSSBInfo.ScriptType = header->ScriptType; 

#ifndef SWAP_FEATURE_PTP
switch (svSSBInfo.ScriptType) { case SWAP_STYPE_PTP:
return SWAP_RCV_DLSCRERR; case SWAP_STYPE_ALONE_PTP:
svSSBInfo.ScriptType = SWAP_STYPE_ALONE; break;
case SWAP_STYPE_PTP_SERVER: svSSBInfo.ScriptType = SWAP_STYPE_SERVER;
break; case SWAP_STYPE_ALL: svSSBInfo.ScriptType = SWAP_STYPE_ALONE_SERVER;
break; } 
#endif
cpid = header->ScriptCPID; 
gid = header->ScriptID; 
if (cpid != svReqInfo.ScriptCPID || gid != svReqInfo.ScriptID) 
return SWAP_RCV_DLSCRERR; 
#ifdef SWAP_FEATURE_AUTH
if (EeRR3hHy(header->ScriptID, H1eVDWXU(header->ScriptVer, header->ScriptCPID)) != header->Auth)
return SWAP_RCV_DLAUTHERR; 
#endif 
memset(svSSBInfo.ScriptName, 0, sizeof(svSSBInfo.ScriptName));
strncpy((char*)svSSBInfo.ScriptName, header->ScriptName, SWAP_LEN_GNAME);
if (svSSBInfo.ScriptVer == SWAP_VERSION_10) {
memset(svSSBInfo.ScriptDir, 0, sizeof(svSSBInfo.ScriptDir));
strcpy((char*)svSSBInfo.ScriptDir, SWAP_TEXT_SWAP10_DIR);
svSSBInfo.ValidCount = 0xFF; 
svSSBInfo.ValidTerm[2] = 0xFF; 
} else {
memset(svSSBInfo.ScriptDir, 0, sizeof(svSSBInfo.ScriptDir));
strncpy((char*)svSSBInfo.ScriptDir, header->DirName, SWAP_LEN_GNAME);
svSSBInfo.ValidCount = header->ValidCount;
if (header->ValidTerm == 0xFF) memset(svSSBInfo.ValidTerm, 0xFF, 3); 
else QaKStTtt(header->ValidTerm, svSSBInfo.ValidTerm);
} }
if ((DWORD)(TB4GUKNg + 1)*SWAP_BLOCK_SIZE < svSSBInfo.ScriptSize) {

#ifdef SWAP_DOWNLOAD_SPEC2 
if (xRSAt1IX != TB4GUKNg) {

#ifdef SWAP_DOWNLOAD_PKTRETRY
if (xRSAt1IX + SWAP_MAGIC_UNIT >= TB4GUKNg) {
if (NGBqi8R2 == 0) { NGBqi8R2++; midd_StartTimer(0, 1000, 0);
s_DlRetryReason = SWAP_RETRYREASON_PKTSKIP;
} return SWAP_RCV_DLPKTSKIP; } else 
#endif
return SWAP_RCV_DLPKTERR; } else 
#endif
{ NGBqi8R2 = 0; memcpy(swScriptBuffer + TB4GUKNg*SWAP_BLOCK_SIZE, &data[2], len - 2);
for (H0Ng1GoT = 2; H0Ng1GoT < len; H0Ng1GoT++)
obd3rXH3 += data[H0Ng1GoT]; xRSAt1IX = TB4GUKNg + 1;

#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC) 
if ((DWORD)(xRSAt1IX + SWAP_MAGIC_UNIT - 1)*SWAP_BLOCK_SIZE > svSSBInfo.ScriptSize)  // added by undersea 01/11/07
return SWAP_RCV_NONE;
#ifdef SWAP_DOWNLOAD_PKTRETRY
if (!tHTO6ObW(xRSAt1IX)) { s_DlRetryReason = 
SWAP_RETRYREASON_RRQ_SNDFAIL; return SWAP_RCV_PROCFAIL; }
#else
tHTO6ObW(xRSAt1IX); 
#endif
#endif
} } else { 
for (H0Ng1GoT = 2; H0Ng1GoT < len; H0Ng1GoT++)
obd3rXH3 += data[H0Ng1GoT]; if (obd3rXH3 != YyRteHno) {
svSSBInfo.ScriptCPID = 0; 
return SWAP_RCV_DLSUMERR; 
} FUvKrLFp(); mil_DrawProg(svSSBInfo.ScriptSize, svSSBInfo.ScriptSize, 0);
memcpy(swScriptBuffer + xRSAt1IX*SWAP_BLOCK_SIZE, &data[2], len - 2);
memset(&svReqInfo, 0, sizeof(svReqInfo));
return SWAP_RCV_DLCOMP; 
} return SWAP_RCV_NONE; }
static int s_SendCFR()
{ 
#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
BYTE sndBuf[27]; 
#else
BYTE sndBuf[28]; 
#endif
BYTE sum = 0; int ret; int H0Ng1GoT; sndBuf[0] = joIosVQ1;
sndBuf[1] = QsZBfaXf; sndBuf[2] = xPfn7LCK;

#ifndef SWAP_DOWNLOAD_SPEC2 
sndBuf[3] = ulIgzpum; JK0JqFmQ(sndBuf, 4, 20); 

#else 

#if (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
sndBuf[3] = ukJoStXU; JK0JqFmQ(sndBuf, 4, 20); 

#elif (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC)
sndBuf[3] = VGMutz5I; JK0JqFmQ(sndBuf, 4, 21); 

#endif

#endif
sndBuf[6] = SWAP_VERSION; 
S0wnz7JD(sndBuf, 7, SWAP_HW_CONFIG); JK0JqFmQ(sndBuf, 11, svReqInfo.ScriptCPID);
JK0JqFmQ(sndBuf, 13, svReqInfo.ScriptID);
strcpy((char*)&sndBuf[15], svMIN); 
#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
for (H0Ng1GoT = 6; H0Ng1GoT < 26; H0Ng1GoT++)
sum += sndBuf[H0Ng1GoT]; sndBuf[26] = sum; 
SWAP_TRACE("SWAP Send Download CFR2(cpid=%ld,id=%ld)", svReqInfo.ScriptCPID, svReqInfo.ScriptID, 0);
ret = midd_SendData(sndBuf, 27); 
#else
sndBuf[26] = SWAP_MAGIC_UNIT; for (H0Ng1GoT = 6; H0Ng1GoT < 27; H0Ng1GoT++)
sum += sndBuf[H0Ng1GoT]; sndBuf[27] = sum; 
SWAP_TRACE("SWAP Send Download CFR3(cpid=%ld,id=%ld)", svReqInfo.ScriptCPID, svReqInfo.ScriptID, 0);
ret = midd_SendData(sndBuf, 28); 
#endif
Ae3zrUtq = 1; mil_InitNetwork(); return ret;
}
int mil_StartDownload()
{ int ret; s_SendCFR(); NGBqi8R2 = 0; return ret; } 
#ifdef SWAP_DOWNLOAD_SNDERR
int mil_SendNak(WORD code, WORD TB4GUKNg)
{ BYTE sndBuf[11]; sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = xPfn7LCK; sndBuf[3] = MRaPnQQi;
JK0JqFmQ(sndBuf, 4, 4); 
JK0JqFmQ(sndBuf, 6, code); 
JK0JqFmQ(sndBuf, 8, TB4GUKNg); 
sndBuf[10] = sndBuf[6] + sndBuf[7] + sndBuf[8] + sndBuf[9]; 
return midd_SendData(sndBuf, 11); } 
#endif
int mil_RetryDownload()
{ 
#ifdef SWAP_DOWNLOAD_CFRRETRY
if (Ae3zrUtq == 1 && NGBqi8R2 < SWAP_DOWNLOAD_RETRY) {
s_SendCFR(); NGBqi8R2++; return 0; } 
#endif

#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC)
if (Ae3zrUtq == 0 && NGBqi8R2 < SWAP_DOWNLOAD_RETRY) {

#if !defined (SWAP_DOWNLOAD_SPEC2)
tHTO6ObW(xRSAt1IX); 
#elif  defined (SWAP_DOWNLOAD_PKTRETRY)
mil_SendNak(s_DlRetryReason << 8 | SWAP_NAK_DLRETRY, xRSAt1IX); 
#endif
NGBqi8R2++; return 0; 
} 
#endif
return 1; 
} 
#endif 
void mil_InitNetwork()
{ nYHkffJe = SWAP_NET_HUNT; yB88FzrU = 1; 
}
int mil_SendData(const BYTE *IntData, WORD nLength)
{ BYTE sndBuf[SWAP_MAX_SNDBUF]; BYTE *Data = (BYTE*)IntData;
BYTE sum = 0; int ret, H0Ng1GoT; if (nLength > SWAP_MAX_SNDBUF - 7)
nLength = SWAP_MAX_SNDBUF - 7; sndBuf[0] = joIosVQ1;
sndBuf[1] = QsZBfaXf; sndBuf[2] = t4qzaYHw;
sndBuf[3] = mmqiZxi7; JK0JqFmQ(sndBuf, 4, nLength); 
memcpy(&sndBuf[6], Data, nLength); 
for (H0Ng1GoT = 0; H0Ng1GoT < nLength; H0Ng1GoT++)
sum += Data[H0Ng1GoT]; sndBuf[6 + nLength] = sum; 

#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_PTP) ret = midd_PtpSendData(sndBuf, 7 + nLength);
else 
#endif 
ret = midd_SendData(sndBuf, 7 + nLength);
return ret; }
static int v2vsJu6r(char *chat)
{ BYTE sndBuf[SWAP_MAX_SNDBUF]; BYTE sum = 0;
int size = strlen(chat) + 1; int ret, H0Ng1GoT;
sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = t4qzaYHw; sndBuf[3] = jPVrKFZz;
JK0JqFmQ(sndBuf, 4, size); 
memcpy(&sndBuf[6], chat, size); 
for (H0Ng1GoT = 0; H0Ng1GoT < size; H0Ng1GoT++)
sum += chat[H0Ng1GoT]; sndBuf[6 + size] = sum; 

#ifdef SWAP_FEATURE_PTP
if (svRunScriptType == SWAP_STYPE_PTP) ret = midd_PtpSendData(sndBuf, 7 + size);
else 
#endif 
ret = midd_SendData(sndBuf, 7 + size); return ret;
}
void mil_SendChat(char *str)
{ char pNQDUYjw[SWAP_LEN_TEXT + SWAP_LEN_USERID + 1];
SWAP_TRACE("SWAP Send CHAT", 0, 0, 0); strcpy(pNQDUYjw, svSSBInfo.UserID);
strcat(pNQDUYjw, ":"); strcat(pNQDUYjw, str);
if (svRunScriptType == SWAP_STYPE_PTP) { svm_ReceiveChat(pNQDUYjw); 
v2vsJu6r(pNQDUYjw); 
} else if (svRunScriptType == SWAP_STYPE_SERVER)
v2vsJu6r(pNQDUYjw); 
}
static void ylaTlwTN(int TB4GUKNg)
{ BYTE sndBuf[9]; sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = t4qzaYHw; sndBuf[3] = oCa3i5tg;
JK0JqFmQ(sndBuf, 4, 2); 
JK0JqFmQ(sndBuf, 6, TB4GUKNg); 
sndBuf[8] = sndBuf[6] + sndBuf[7]; 
midd_SendData(sndBuf, 9); }
static void KuARn8AE()
{ BYTE sndBuf[7]; sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = t4qzaYHw; sndBuf[3] = HTiKEkBK;
JK0JqFmQ(sndBuf, 4, 0); 
sndBuf[6] = 0; 
midd_SendData(sndBuf, 7); }
static void OT9BvqUD(int type, BYTE *data, int len)
{ int TB4GUKNg; int rsize; BYTE *gh3BT4ts;
int H0Ng1GoT; switch (type) { case 0: 
ZKd4bSEe = A4DyGYLk(data, 0); KBiBLRsS = ZKd4bSEe;
zTQdDEMy = A4DyGYLk(data, 2); n8sPwEQZ = 0;
if (svm_SetMediaSize(UL1j91DY, ZKd4bSEe) == 0) {
yB88FzrU = 3; 
svm_Result(SWAP_RST_MDOWN_FAIL); return; }
ylaTlwTN(0); 
break; case 1: 
TB4GUKNg = A4DyGYLk(data, 0); rsize = (KBiBLRsS < SWAP_BLOCK_SIZE) ? KBiBLRsS : SWAP_BLOCK_SIZE;
memcpy(swRamMediaTable[UL1j91DY].addr + TB4GUKNg*SWAP_BLOCK_SIZE, &data[2], rsize);
KBiBLRsS -= rsize; if (KBiBLRsS > 0) 
#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC) 
ylaTlwTN(TB4GUKNg + 1); 

#else
break; 
#endif
else { gh3BT4ts = swRamMediaTable[UL1j91DY].addr;
for (H0Ng1GoT = 0; H0Ng1GoT < ZKd4bSEe; H0Ng1GoT++)
n8sPwEQZ += *gh3BT4ts++; if (n8sPwEQZ == zTQdDEMy) {
yB88FzrU = 1; 
KuARn8AE(); svm_Result(SWAP_RST_MDOWN_OK);
} else { yB88FzrU = 2; 
svm_Result(SWAP_RST_MDOWN_FAIL); } } break;
} }
int mil_LoadMedia(const INT2 mindex, const INT2 key)
{ 
#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
BYTE sndBuf[18]; 
#else
BYTE sndBuf[19]; 
#endif
BYTE sum = 0; int H0Ng1GoT; if (yB88FzrU == 0)
return 0; 
mil_InitNetwork(); 
UL1j91DY = mindex; yB88FzrU = 0; 
sndBuf[0] = joIosVQ1; sndBuf[1] = QsZBfaXf;
sndBuf[2] = t4qzaYHw; 
#ifndef SWAP_DOWNLOAD_SPEC2 
sndBuf[3] = uwVq9OeB; JK0JqFmQ(sndBuf, 4, 11); 

#else

#if (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
sndBuf[3] = t7fE8lgK; JK0JqFmQ(sndBuf, 4, 11); 

#elif (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_MAGIC)
sndBuf[3] = i2NrRJiB; JK0JqFmQ(sndBuf, 4, 12); 

#endif

#endif
sndBuf[6] = SWAP_VERSION; 
S0wnz7JD(sndBuf, 7, SWAP_HW_CONFIG); 
JK0JqFmQ(sndBuf, 11, svSSBInfo.ScriptCPID);
JK0JqFmQ(sndBuf, 13, svSSBInfo.ScriptID);
JK0JqFmQ(sndBuf, 15, key); 
#if !defined (SWAP_DOWNLOAD_SPEC2) || (SWAP_DOWNLOAD_TYPE == SWAP_DOWNLOAD_SLIDING)
for (H0Ng1GoT = 6; H0Ng1GoT < 17; H0Ng1GoT++)
sum += sndBuf[H0Ng1GoT]; sndBuf[17] = sum; 
mil_InitNetwork(); 
return midd_SendData(sndBuf, 18); 
#else
sndBuf[17] = SWAP_MAGIC_UNIT; for (H0Ng1GoT = 6; H0Ng1GoT < 18; H0Ng1GoT++)
sum += sndBuf[H0Ng1GoT]; sndBuf[18] = sum; 
mil_InitNetwork(); 
return midd_SendData(sndBuf, 19); 
#endif
}
int mil_LoadMResult(const INT2 index)
{ if (index == UL1j91DY) return yB88FzrU;
else return 4; 
}
int mil_ReceiveData(BYTE *Data, WORD nLength)
{ static BYTE VFlB3af7; 
static BYTE JgsVT4zY; 
static WORD HRpVpREK; 
static BYTE U9283A0P; 
BYTE *endData = Data + nLength; int ret = SWAP_RCV_NONE;
while (Data < endData) { switch (nYHkffJe) {
case SWAP_NET_HUNT: if (*Data == joIosVQ1)
nYHkffJe = SWAP_NET_MAGIC; Data++; break;
case SWAP_NET_MAGIC: if (*Data == QsZBfaXf) {
nYHkffJe = SWAP_NET_SESSION; Data++; } else
nYHkffJe = SWAP_NET_HUNT; break; case SWAP_NET_SESSION:
VFlB3af7 = *Data; if (VFlB3af7 == xPfn7LCK ||
VFlB3af7 == t4qzaYHw) { nYHkffJe = SWAP_NET_TYPE;
Data++; } else nYHkffJe = SWAP_NET_HUNT; break;
case SWAP_NET_TYPE: JgsVT4zY = *Data++; nYHkffJe = SWAP_NET_LENGTH_H;
break; case SWAP_NET_LENGTH_H: HRpVpREK = (*Data++) << 8;
nYHkffJe = SWAP_NET_LENGTH_L; case SWAP_NET_LENGTH_L:
HRpVpREK |= *Data++; if (HRpVpREK == 0) nYHkffJe = SWAP_NET_SUM;
else nYHkffJe = SWAP_NET_DATA; U9283A0P = 0;
fVWtP902 = 0; break; case SWAP_NET_DATA: U9283A0P += *Data;
cXsO2ZWP[fVWtP902++] = *Data++; if (fVWtP902 == HRpVpREK)
nYHkffJe = SWAP_NET_SUM; break; case SWAP_NET_SUM:
nYHkffJe = SWAP_NET_HUNT; if (*Data++ == U9283A0P) {
switch (VFlB3af7) { 
#ifdef SWAP_FEATURE_DOWNLOAD
case xPfn7LCK: switch (JgsVT4zY) { 
#ifndef SWAP_DOWNLOAD_SPEC2 
case fNuXUKcl: Ae3zrUtq = 0; SWAP_TRACE("SWAP Receive Download CFA", 0, 0, 0);
ret = P0sDgg0A(cXsO2ZWP, fVWtP902); break;

#else 
case mrguskuo: SWAP_TRACE("SWAP Receive Download CFA2", 0, 0, 0);

#ifdef SWAP_DOWNLOAD_CFRRETRY
if (Ae3zrUtq == 0) break;

#endif
Ae3zrUtq = 0; ret = P0sDgg0A(cXsO2ZWP, fVWtP902);
break; 
#endif

#ifdef SWAP_DOWNLOAD_RCVERR
case MRaPnQQi: SWAP_TRACE("SWAP Receive Download NAK", 0, 0, 0);
memset (svResultString, 0, sizeof(svResultString));
memcpy((BYTE*)svResultString, cXsO2ZWP, HRpVpREK);
ret = SWAP_RCV_DLNAKERR; break; 
#endif
case bkaqULUC: SWAP_TRACE("SWAP Receive Download DATA", 0, 0, 0);
ret = V8c05IaJ(cXsO2ZWP, fVWtP902); break;
} break; 
#endif 
case t4qzaYHw: switch (JgsVT4zY) { case mmqiZxi7:
SWAP_TRACE("SWAP Receive DATA", 0, 0, 0);
memcpy((BYTE*)swRcvBuf, cXsO2ZWP, fVWtP902);
svm_ReceiveData(fVWtP902); 
break; case jPVrKFZz: SWAP_TRACE("SWAP Receive CHAT", 0, 0, 0);
svm_ReceiveChat((char*)cXsO2ZWP); 
svm_Result(SWAP_RST_RCV_CHAT); break; 
#ifndef SWAP_DOWNLOAD_SPEC2 
case RoLcx5Rp: SWAP_TRACE("SWAP Receive CFA", 0, 0, 0);
OT9BvqUD(0, cXsO2ZWP, fVWtP902); break; 
#else
case CCyNC48Y: SWAP_TRACE("SWAP Receive CFA", 0, 0, 0);
OT9BvqUD(0, cXsO2ZWP, fVWtP902); break; 
#endif
case r23FchhA: SWAP_TRACE("SWAP Receive MEDIA", 0, 0, 0);
OT9BvqUD(1, cXsO2ZWP, fVWtP902); break; }
break; } 
} 
break; } 
} 
return ret; } 
#endif 
