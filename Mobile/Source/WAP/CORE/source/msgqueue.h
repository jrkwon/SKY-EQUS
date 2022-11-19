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
#ifndef MSGQUEUE_H
#define MSGQUEUE_H


#include "tapicmmn.h"
#include "capiclnt.h"
#include "confvars.h"


typedef enum    eMsgId
{
    eUndefined = 0,
    eDiscardAllContext, /* internal signal initiated from cmmnrsrc.c
                           when memory has reached a too high level */
    eCLNTc_start,
    eCLNTc_terminate,
    eCLNTc_timerExpired,
    eCLNTc_file,
    eCLNTc_functionResult,
    eCLNTc_getContent,
    eCLNTc_postContent,
	eCLNTc_postMoreContent,
	eCLNTc_acknowledgeContent,
	eCLNTc_cancelContent,
	eCLNTc_requestConnectionDone,
    eCLNTc_setupConnectionDone,
	eCLNTc_closeConnection,
	eCLNTc_WMLSLibFuncResponse,
    eCRYPTc_getMethodsResponse,
    eCRYPTc_PRFResponse,
    eCRYPTc_verifyCertificateChainResponse,
    eCRYPTc_keyExchangeResponse,
    eCRYPTc_getClientCertificateResponse,
    eCRYPTc_computeSignatureResponse,
    eCRYPTc_peerLookupResponse,
    eCRYPTc_sessionFetchResponse,
    eMEMc_initCache,
    eMEMc_prepareCache,
    eMMIc_startUserAgent,
    eMMIc_terminateUserAgent,
    eMMIc_loadURL,
    eMMIc_reload,
    eMMIc_stop,
    eMMIc_goBack,
    eMMIc_passwordDialogResponse,
    eMMIc_promptDialogResponse,
    eMMIc_confirmDialogResponse,
    eMMIc_alertDialogResponse,
    eMMIc_keySelected,
    eMMIc_textSelected,
    eMMIc_imageSelected,
    eMMIc_optionSelected,
    eMMIc_inputString,
    eMMIc_linkInfo,
    eMMIc_textSigned,
    ePUSHc_loadSI,
    ePUSHc_deleteSI,
    ePUSHc_getSIinfo,
    ePUSHc_loadSL,
    ePUSHc_deleteSL,
		ePUSHc_requestConnectionDone,
    ePUSHc_changeStatus,
	ePUSHc_getSLinfo,
    eSMSc_sentRequest,
    eSMSc_receivedRequest,
    eSMSc_receivedError,
    eUDPc_receivedRequest,
    eUDPc_errorRequest,
    eUSSDc_receivedResultProcessRequest,
    eUSSDc_receivedError,
    eUSSDc_receivedInvokeRequest,
    eUSSDc_receivedRelease,
	eWTAIc_response,
    eWTAc_confirmInstallation,
    eWTAc_retryGetInstallationResult,
    eWTAc_showInstallationResult,
    eWTAc_abortInstallation,
    eWTAc_getServices,
    eWTAc_deleteService,
    eWTAc_executeService,
    eWTAc_clearServices,
    eWTAIc_any, /*  Almost all WTAI functions are mapped to the same signal. */
    eWTAIc_terminateService,
    eEnd
} MsgId;


/*
Any of the members may be used or not.
The order of the parameters in the function determines the order in the struct.
*/
typedef struct  stMsg
{
    BOOL            vBOOL[3];
    INT8            vINT8;
    INT16           vINT16;
    UINT8           vUINT8[5];
    UINT16          vUINT16[4];
    UINT32          vUINT32[2];
    VOID            *voidstar;
    CHAR            *sz[4];
    CHAR            *pCHAR[3];
    WCHAR           *wz;
    WCHAR           *pWCHAR;
    variableType    **ppVariableType;
	#ifdef USE_PROPRIETARY_WMLS_LIBS
    WMLSvar         *pWMLSvar;
    #endif

    MsgId           Id;
    struct stMsg    *pNext;
} Msg;


int     InitQueue(void);

void    DeleteQueue(void);

int     SendMsg(Msg *);

Msg *   GetMsg(void);

Msg *   FirstMsg(void);


#endif
