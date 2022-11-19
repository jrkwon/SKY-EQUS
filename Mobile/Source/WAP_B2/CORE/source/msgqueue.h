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
#include "tapimem.h"
#include "capiclnt.h"


typedef enum    eMsgId
{
    eUndefined = 0,
    eDiscardAllContext, /* internal signal initiated from cmmnrsrc.c
                           when memory has reached a too high level */
    eCLNTc_start,
    eCLNTc_terminate,
    eCLNTc_initialised,
    eCLNTc_timerExpired,
    eCLNTc_setIntConfig,
    eCLNTc_setStrConfig,
    eCLNTc_usePort,
    eCLNTc_closePort,
    eCLNTc_file,
    eCLNTc_functionResult,
    eCLNTc_getContent,
    eCLNTc_confirmDownload,
	eCLNTc_WMLSLibFuncResponse,
    eCRYPTc_certificateListUpdated,
    eCRYPTc_verifyCertificateChainResponse,
    eCRYPTc_keyExchangeResponse,
    eMEMc_initCache,
    eMEMc_prepareCache,
    eMMIc_openView,
    eMMIc_closeView,
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
    eSMSc_sentRequest,
    eSMSc_receivedRequest,
    eSMSc_receivedError,
    eUDPc_receivedRequest,
    eUDPc_errorRequest,
    eUSSDc_receivedResultProcessRequest,
    eUSSDc_receivedError,
    eUSSDc_receivedInvokeRequest,
    eUSSDc_receivedRelease,
    eWDc_receivedRequest,
    eWDc_errorRequest,
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
    BOOL            vBOOL;
    INT8            vINT8;
    INT16           vINT16;
    UINT8           vUINT8[3];
    UINT16          vUINT16[3];
    UINT32          vUINT32[2];
    memSizeType     memSize[2];
    BYTE            *pBYTE;
    CHAR            *sz;
    CHAR            *pCHAR[3];
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
