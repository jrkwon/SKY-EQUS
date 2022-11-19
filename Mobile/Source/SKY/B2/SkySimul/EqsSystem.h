/*
** ============================================================================
**
** Project: 
**     EQUSIMUL
**
** File:
**     EqsSystem.h
**
** Description:
**     @DesciptionAboutThisFileHere
**
** Copyright (c) 2000 SK Teletech, Co. All Rights Reserved.
**
** History:
**
** When       who        what, where, why
** ---------- --------   ------------------------------------------------------
** 2000-08-17 jrkwon     Created.
**
** ============================================================================
*/

#ifndef _EQSSYSTEM_H_
#define _EQSSYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "eqs_evnt.h"
#include "eqs_feat.h"
#include "sky_feat.h"
#include "sky_def.h"

/*#ifdef FEATURE_EQS_COLOR_4GRAY
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   128
#else
#define SCREEN_WIDTH    120
#define SCREEN_HEIGHT   120
#endif
*/
VOID OpenEQUS(int nColor);
VOID SendEventToEQS(EQS_EVENT_ID Event);
VOID FlushEvent(VOID);
VOID FlushLeftKeyEvent(VOID);
VOID CloseEQUS(VOID);
BYTE* GetLCDBuffer(VOID);
BYTE* GetSubLCDBuffer(VOID);
VOID ClearNUIScreen(VOID);
    
#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* _EQSSYSTEM_H_ */
