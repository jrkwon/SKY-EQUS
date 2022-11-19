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
#include "capiwta.h"
#include "aapiwta.h"
#include "aapiclnt.h"
#include "msgqueue.h"
#include "cmmnrsrc.h"
#ifdef CONFIG_WTA
#include "wta_ua.h"
#endif


#ifdef CONFIG_WTA


EXPORT_C VOID    WTAIc_outgoingCallIndication (INT8 callId, const CHAR *callerId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_callerId = callerId;
        if (!_callerId)
        {
            _callerId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_outgoingCallIndication\nINT8 \t callId \t %i\n, const CHAR * \t callerId \t %s", (int)callId, _callerId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventOutgoingCallIndication(callId, callerId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/oc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_connectingCallIndication (INT8 callId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_connectingCallIndication\nINT8 \t callId \t %i", (int)callId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventConnectiongCallIndication(callId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_incomingCall (INT8 callId, const CHAR *callerId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_callerId = callerId;
        if (!_callerId)
        {
            _callerId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_incomingCall\nINT8 \t callId \t %i\n, const CHAR * \t callerId \t %s\n", (int)callId, _callerId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventIncomingCall(callId, callerId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/ic");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callConnected (INT8 callId, const CHAR *callerId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_callerId = callerId;
        if (!_callerId)
        {
            _callerId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_callConnected\nINT8 \t callId \t %i\n, const CHAR * \t callerId \t %s\n", (int)callId, _callerId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventCallConnected(callId, callerId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callCleared (INT8 callId, INT8 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callCleared\nINT8 \t callId \t %i\n, INT8 \t result \t %i\n", (int)callId, (int)result);
    }
#endif

    pMsg->voidstar = wtaiEventCallCleared(callId, result);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cl");

    pMsg->Id = eWTAIc_any;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_DTMFsent (const WCHAR *result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_result = result;
        if (!_result)
        {
            _result = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_DTMFsent\nconst WCHAR * \t result \t %S\n", _result);
    }
#endif

    pMsg->voidstar = wtaiEventDTMFSent(result);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/dtmf");

    pMsg->Id = eWTAIc_any;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_networkStatusIndication (BOOL camping, const WCHAR *networkName, INT8 notCampingCause)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_networkName = networkName;
        if (!_networkName)
        {
            _networkName = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_networkStatusIndication\nBOOL \t camping \t %u\n, const WCHAR * \t networkName \t %S\n, INT8 \t notCampingCause \t %i\n", (int)camping, _networkName, (int)notCampingCause);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventNetworkStatusIndication(camping, networkName, notCampingCause);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-ms/ns");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_incomingText (INT16 textId, const WCHAR *sender)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_sender = sender;
        if (!_sender)
        {
            _sender = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_incomingText\nINT16 \t textId \t %i\n, const WCHAR * \t sender \t %S\n", (int)textId, _sender);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventIncomingText(textId, sender);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-nt/it");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_networkTextSent (INT16 textId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_networkTextSent\nINT16 \t textId \t %i\n", (int)textId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventNetworkTextSent(textId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-nt/st");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_GSMcallActiveIndication (INT8 callId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    CLNTa_log(0, 0, "WTAIc_GSMcallActiveIndication\nINT8 \t callId \t %i\n", (int)callId);
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventGSMCallActiveIndication(callId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-gsm/ca");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_GSMcallHeldIndication (INT8 callId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_GSMcallHeldIndication\nINT8 \t callId \t %i\n", (int)callId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventGSMCallHeldIndication(callId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-gsm/ch");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_receivedUSSD (const WCHAR *text, const WCHAR *dataCodingScheme, INT8 type, const WCHAR *transactionId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        char    szFormat[256];
        size_t  strlength, end = 0;
        const char  *csz;
        const WCHAR *_text = text;
        const WCHAR *_dataCodingScheme = dataCodingScheme;
        const WCHAR *_transactionId = transactionId;

        if (!_text)
        {
            _text = cwzNULL;
        }
        if (!_dataCodingScheme)
        {
            _dataCodingScheme = cwzNULL;
        }
        strlength = strlen(csz = "WTAIc_receivedUSSD\nconst WCHAR * \t text \t %S\n, const WCHAR * \t dataCodingScheme \t %S\n, INT8 \t type \t %i   (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (type)
        {
            case WTA_USSD_PRC_REQ:  /*  0 */
                csz = "WTA_USSD_PRC_REQ";
                break;
            case WTA_USSD_REQ:      /*  1 */
                csz = "WTA_USSD_REQ";
                break;
            case WTA_USSD_NOT:      /*  2 */
                csz = "WTA_USSD_NOT";
                break;
            case WTA_USSD_PRC_ERR:  /*  3 */
                csz = "WTA_USSD_PRC_ERR";
                pText = pDCS = cwzNULL;
                break;
            default:
                csz = "Currently not a supported USSD operation.";
                break;
        }
        strlength = strlen(csz);
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        strlength = strlen(csz = ")\n, const WCHAR * \t transactionId \t %S\n");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        if (!_transactionId)
        {
            _transactionId = cwzNULL;
        }

        szFormat[end] = '\0';
        CLNTa_log(0, 0, szFormat, _text, _dataCodingScheme, (int)type, _transactionId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventUSSD_Received(text, dataCodingScheme, type, transactionId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-gsm/ru");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_terminateService (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_terminateService\nVOID");
    }
#endif

    pMsg->Id = eWTAIc_terminateService;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_confirmInstallation (INT8 installId, BOOL install)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_confirmInstallation\nINT8 \t installId \t %i\n, BOOL \t install \t %u\n", (int)installId, (int)install);
    }
#endif

    pMsg->Id = eWTAc_confirmInstallation;

    pMsg->vINT8 = installId;
    pMsg->vBOOL[0] = install;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_retryGetInstallationResult (INT8 installId, BOOL retry)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_retryGetInstallationResult\nINT8 \t installId \t %i\n, BOOL \t retry \t %u\n", (int)installId, (int)retry);
    }
#endif

    pMsg->Id = eWTAc_retryGetInstallationResult;

    pMsg->vINT8 = installId;
    pMsg->vBOOL[0] = retry;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_showInstallationResult (INT8 installId, BOOL show)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_showInstallationResult\nINT8 \t installId \t %i\n, BOOL \t show \t %u\n", (int)installId, (int)show);
    }
#endif

    pMsg->Id = eWTAc_showInstallationResult;

    pMsg->vINT8 = installId;
    pMsg->vBOOL[0] = show;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_abortInstallation (INT8 installId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_abortInstallation\nINT8 \t installId \t %i\n", (int)installId);
    }
#endif

    pMsg->Id = eWTAc_abortInstallation;

    pMsg->vINT8 = installId;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_getServices (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_getServices\nVOID");
    }
#endif

    pMsg->Id = eWTAc_getServices;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_deleteService (const CHAR *eventId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *sz;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_eventId = eventId;
        if (!_eventId)
        {
            _eventId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAc_deleteService\nconst CHAR * \t eventId \t %s\n", _eventId);
    }
#endif

    pMsg->Id = eWTAc_deleteService;

    sz = NULL;
    if (eventId)
    {
        size_t  len;
        len = strlen(eventId);
        /*  Add one character for the terminating zero. */
        sz = (CHAR *)OSConnectorAlloc(++len);
        strncpy(sz, eventId, len);
    }
    pMsg->sz[0] = sz;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_executeService (const CHAR *eventId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    CHAR    *sz;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_eventId = eventId;
        if (!_eventId)
        {
            _eventId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAc_executeService\nconst CHAR * \t eventId \t %s\n", _eventId);
    }
#endif

    pMsg->Id = eWTAc_executeService;

    sz = NULL;
    if (eventId)
    {
        size_t  len;
        len = strlen(eventId);
        /*  Add one character for the terminating zero. */
        sz = (CHAR *)OSConnectorAlloc(++len);
        strncpy(sz, eventId, len);
    }
    pMsg->sz[0] = sz;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_clearServices (VOID)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_clearServices\nVOID");
    }
#endif

    pMsg->Id = eWTAc_clearServices;

    bSendMsg = SendMsg(pMsg);
}


#endif
