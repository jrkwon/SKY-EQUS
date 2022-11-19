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

#include "msgqueue.h"
#include "cmmnrsrc.h"
#ifdef LOG_EXTERNAL
#include "aapiwta.h"
#include "aapiclnt.h"
#endif

#ifdef CONFIG_WTA
#include "wta_ua.h"
#endif

/* Responses */

EXPORT_C VOID    WTAIc_publicMakeCallResponse (UINT8 objectID, INT8 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_publicMakeCallResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
    }
#endif

    pMsg->Id = eWTAIc_response;

    pMsg->vUINT8[0] = objectID;
	pMsg->vINT16 = result;
    pMsg->voidstar = NULL;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_publicSendDTMFResponse (UINT8 objectID, INT8 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_publicSendDTMFResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
    }
#endif

    pMsg->Id = eWTAIc_response;

    pMsg->vUINT8[0] = objectID;
	pMsg->vINT16 = result;
    pMsg->voidstar = NULL;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_publicAddPBEntryResponse (UINT8 objectID, INT8 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_publicAddPBEntryResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
    }
#endif

    pMsg->Id = eWTAIc_response;

    pMsg->vUINT8[0] = objectID;
	pMsg->vINT16 = result;
    pMsg->voidstar = NULL;

    bSendMsg = SendMsg(pMsg);
}




#ifdef CONFIG_WTA


BOOL help_func_int(UINT8 objectID, INT16 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return FALSE;

    pMsg->Id = eWTAIc_response;

    pMsg->vUINT8[0] = objectID;
	pMsg->vINT16 = result;
    pMsg->voidstar = NULL;

    bSendMsg = SendMsg(pMsg);

#ifdef LOG_EXTERNAL
	return TRUE;
#else
	return FALSE;
#endif
}


BOOL help_func_int_wchar(UINT8 objectID, INT16 result, WCHAR *fieldValue)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return FALSE;

    pMsg->Id = eWTAIc_response;

    pMsg->vUINT8[0] = objectID;
	pMsg->vINT16 = result;
    pMsg->voidstar = fieldValue;

    bSendMsg = SendMsg(pMsg);

#ifdef LOG_EXTERNAL
	return TRUE;
#else
	return FALSE;
#endif
}


EXPORT_C VOID	WTAIc_voiceCallSetupResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_voiceCallSetupResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_voiceCallAcceptResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_voiceCallAcceptResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_voiceCallReleaseResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_voiceCallReleaseResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID   WTAIc_voiceCallSendDTMFResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_voiceCallSendDTMFResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID   WTAIc_voiceCallCallStatusResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue)
{
	if ( help_func_int_wchar(objectID, result, fieldValue) )
        CLNTa_log(0, 0, "WTAIc_voiceCallCallStatusResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nWCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
}

EXPORT_C VOID	WTAIc_voiceCallListResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_voiceCallListResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_netTextSendResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_netTextSendResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_netTextListResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_netTextListResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_netTextRemoveResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_netTextRemoveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_netTextGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue)
{
	if ( help_func_int_wchar(objectID, result, fieldValue) )
        CLNTa_log(0, 0, "WTAIc_netTextGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nWCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
}


EXPORT_C VOID	WTAIc_netTextMarkAsReadResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_netTextMarkAsReadResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_phoneBookWriteResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_phoneBookWriteResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_phoneBookSearchResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_phoneBookSearchResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_phoneBookRemoveResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_phoneBookRemoveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_phoneBookGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue)
{
	if ( help_func_int_wchar(objectID, result, fieldValue) )
        CLNTa_log(0, 0, "WTAIc_phoneBookGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nWCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
}


EXPORT_C VOID	WTAIc_phoneBookChangeResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_phoneBookChangeResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_miscSetIndicatorResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_miscSetIndicatorResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_callLogDialledResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_callLogDialledResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_callLogMissedResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_callLogMissedResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_callLogReceivedResponse  (UINT8 objectID, INT16 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_callLogReceivedResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_callLogGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue)
{
	if ( help_func_int_wchar(objectID, result, fieldValue) )
        CLNTa_log(0, 0, "WTAIc_callLogGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nWCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
}




EXPORT_C VOID	WTAIc_GSMHoldResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMHoldResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMRetrieveResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMRetrieveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMTransferResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMTransferResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMDeflectResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMDeflectResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMMultipartyResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMMultipartyResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMSeparateResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMSeparateResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMSendUSSDResponse  (UINT8 objectID, INT8 result)
{
	if ( help_func_int(objectID, result) )
        CLNTa_log(0, 0, "WTAIc_GSMSendUSSDResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
}

EXPORT_C VOID	WTAIc_GSMNetinfoResponse (UINT8 objectID, WCHAR *result)
{
	if ( help_func_int_wchar(objectID, 0, result) )
        CLNTa_log(0, 0, "WTAIc_GSMNetinfoResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %s\n", (int)objectID, result);
}





/* Events */


EXPORT_C VOID    WTAIc_incomingCall (INT8 callHandle, const CHAR *callerId)
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
        CLNTa_log(0, 0, "WTAIc_incomingCall\nINT8 \t callHandle \t %i\n, const CHAR * \t callerId \t %s\n", (int)callHandle, _callerId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventIncomingCall(callHandle, (const unsigned char *)callerId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/ic");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callCleared (INT8 callHandle, INT8 result)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callCleared\nINT8 \t callHandle \t %i\n, INT8 \t result \t %i\n", (int)callHandle, (int)result);
    }
#endif

    pMsg->voidstar = wtaiEventCallCleared(callHandle, result);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cl");

    pMsg->Id = eWTAIc_any;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callConnected (INT8 callHandle, const CHAR *callerId)
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
        CLNTa_log(0, 0, "WTAIc_callConnected\nINT8 \t callHandle \t %i\n, const CHAR * \t callerId \t %s\n", (int)callHandle, _callerId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventCallConnected(callHandle, (const unsigned char *)callerId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_outgoingCall (INT8 callHandle, const CHAR *number)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const char  *_number = number;
        if (!_number)
        {
            _number = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_outgoingCall\nINT8 \t callHandle \t %i\n, const CHAR * \t number \t %s", (int)callHandle, _number);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventOutgoingCall (callHandle, (const unsigned char *)number);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/oc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callAlerting (INT8 callHandle)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callAlerting\nINT8 \t callHandle \t %i", (int)callHandle);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventCallAlerting(callHandle);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/cc");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_DTMFSent (INT8 callHandle, const WCHAR *dtmf)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_dtmf = dtmf;
        if (!_dtmf)
        {
            _dtmf = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_DTMFSent\nINT8 \t callHandle \t %i\nconst WCHAR * \t dtmf \t %S\n", (int)callHandle, _dtmf);
    }
#endif

    pMsg->voidstar = wtaiEventDTMFSent(callHandle, dtmf);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-cc/dtmf");

    pMsg->Id = eWTAIc_any;

    bSendMsg = SendMsg(pMsg);
}












EXPORT_C VOID    WTAIc_networkStatus (INT8 inService, const WCHAR *networkName, INT8 explanation)
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
        CLNTa_log(0, 0, "WTAIc_networkStatus\nINT8 \t inService \t %i\n, const WCHAR * \t networkName \t %S\n, INT8 \t explanation \t %i\n", (int)inService, _networkName, (int)explanation);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventNetworkStatus(inService, networkName, explanation);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-ms/ns");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_incomingMessage (INT16 textId, const WCHAR *sender)
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
        CLNTa_log(0, 0, "WTAIc_incomingMessage\nINT16 \t textId \t %i\n, const WCHAR * \t sender \t %S\n", (int)textId, _sender);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventIncomingMessage(textId, sender);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-nt/it");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_messageSendStatus (INT16 msgHandle, INT8 textId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_messageSendStatus\nINT16 \t msgHandle \t %i\nINT8 \t textId \t %i\n", (int)msgHandle, (int)textId);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventMessageSendStatus(msgHandle, textId);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-nt/st");

    bSendMsg = SendMsg(pMsg);
}





EXPORT_C VOID    WTAIc_callHeld (INT8 callHandle)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callHeld\nINT8 \t callHandle \t %i\n", (int)callHandle);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventCallHeld(callHandle);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-gsm/ch");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_callActive (INT8 callHandle)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callActive\nINT8 \t callHandle \t %i\n", (int)callHandle);
    }
#endif

    pMsg->Id = eWTAIc_any;

    pMsg->voidstar = wtaiEventCallActive(callHandle);
    pMsg->pWCHAR = wip_byte2wchar("wtaev-gsm/ca");

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAIc_USSDReceived (const WCHAR *text, const WCHAR *dataCodingScheme, INT8 type, const WCHAR *transactionId)
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
        strlength = strlen(csz = "WTAIc_USSDReceived\nconst WCHAR * \t text \t %S\n, const WCHAR * \t dataCodingScheme \t %S\n, INT8 \t type \t %i   (");
        strncpy(&szFormat[end], csz, strlength);
        end += strlength;
        switch (type)
        {
            case WTA_USSD_RECEIVED_RESULT:  /*  0 */
                csz = "WTA_USSD_RECEIVED_RESULT";
                break;
            case WTA_USSD_RECEIVED_REQUEST:      /*  1 */
                csz = "WTA_USSD_RECEIVED_REQUEST";
                break;
            case WTA_USSD_RECEIVED_NOTIFY:      /*  2 */
                csz = "WTA_USSD_RECEIVED_NOTIFY";
                break;
            case WTA_USSD_RECEIVED_ERROR:  /*  3 */
                csz = "WTA_USSD_RECEIVED_ERROR";
/*              what is this? ASPN
                pText = pDCS = cwzNULL;
*/
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




/* Services */

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
        CLNTa_log(0, 0, "WTAc_getServices\nVOID\n");
    }
#endif

    pMsg->Id = eWTAc_getServices;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_deleteService (const WCHAR *eventId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WCHAR   *wz;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_eventId = eventId;
        if (!_eventId)
        {
            _eventId = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAc_deleteService\nconst WCHAR * \t eventId \t %S\n", _eventId);
    }
#endif

    pMsg->Id = eWTAc_deleteService;

    wz = NULL;
    if (eventId)
    {
        size_t  len;
        /*  Add one character for the terminating zero. */
        len = (w_strlen(eventId) + 1) * sizeof(WCHAR);
        wz = (WCHAR *)OSConnectorAlloc(len);
        if (!wz)
            return;
        memcpy(wz, eventId, len);
    }
    pMsg->wz = wz;

    bSendMsg = SendMsg(pMsg);
}

EXPORT_C VOID    WTAc_executeService (const WCHAR *eventId)
{
    Msg *pMsg = (Msg*)OSConnectorAlloc(sizeof(Msg));
    int bSendMsg;
    WCHAR   *wz;

    if (!pMsg)
        return;

#if     defined LOG_EXTERNAL
    {
        const WCHAR *_eventId = eventId;
        if (!_eventId)
        {
            _eventId = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAc_executeService\nconst WCHAR * \t eventId \t %S\n", _eventId);
    }
#endif

    pMsg->Id = eWTAc_executeService;

    wz = NULL;
    if (eventId)
    {
        size_t  len;
        /*  Add one character for the terminating zero. */
        len = (w_strlen(eventId) + 1) * sizeof(WCHAR);
        wz = (WCHAR *)OSConnectorAlloc(len);
        if (!wz)
            return;
        memcpy(wz, eventId, len);
    }
    pMsg->wz = wz;

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
        CLNTa_log(0, 0, "WTAc_clearServices\nVOID\n");
    }
#endif

    pMsg->Id = eWTAc_clearServices;

    bSendMsg = SendMsg(pMsg);
}


#endif
