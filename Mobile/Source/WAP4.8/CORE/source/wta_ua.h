/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#ifndef WTA_UA_H
#define WTA_UA_H
#include "waedef.h"
#include "capiclnt.h"
#include "ansilibs.h"
#include "aapimmi.h"
#include "aapiwta.h"
#include "cmmnrsrc.h"
#include "wae_cmmn.h"
#include "url.h"
#include "wtai_if.h"

BOOL CallModeIsKeep(void** ppCallList, INT16 callHandle);

void* SDL_CreateWTAControl(int iViewID);

void SDL_DeleteWTAControl(void** ppCallCtrl);

void SDL_TerminateService(void* pWTAControl);

void AddCall(void** ppCallList, INT16 callId);

BOOL GetLibAndFunction(BYTE* pURL, BYTE** ppWTAILib, BYTE** ppWTAIFunction, BYTE** ppWTAIParameters);

BYTE* GetNextWTAIInParamFromAnURI(BYTE** ppbInStr);

BYTE* GetNextWTAIOutParamFromAnURI(BYTE** ppbInStr);

WCHAR* int2wchar (INT32 iValue);

/*------------------------------------------------------------------------------------------
wtai conversion calls 
--------------------------------------------------------------------------------------------*/

BOOL CreateVariableList(UINT8 iSize, variableType*** pppListHandle, BOOL fWithIndex);

variableType** AddVariable(variableType** ppListHandle, WCHAR* pName, WCHAR* pStrValue);


variableType** wtaiEventIncomingCall(INT16 iId, const BYTE* pbCallerID);

variableType** wtaiEventCallCleared(INT16 iId, INT8 iResult);

variableType** wtaiEventCallConnected(INT16 iId, const BYTE* pbCallerID);

variableType** wtaiEventOutgoingCall (INT16 iId, const BYTE* pbCallerID);

variableType** wtaiEventCallAlerting (INT16 iId);

variableType** wtaiEventDTMFSent(INT16 callHandle, const WCHAR* pwchResultString);



variableType** wtaiEventMessageSendStatus(INT16 msgHandle, INT16 iId);



variableType** wtaiEventCallHeld(INT16 iId);

variableType** wtaiEventCallActive(INT16 iId);

variableType** wtaiEventUSSD_Received(const WCHAR* pwchUSSDString, const WCHAR* pwchUSSDDataCodingScheme, INT8 iType, const WCHAR* pwchId);



variableType** wtaiEventIncomingMessage (INT16 iId, const WCHAR* pwchSender);

variableType** wtaiEventNetworkStatus (BOOL fCamping, const WCHAR* pwchNetworkName, INT8 iNotCampingCause);



#endif
