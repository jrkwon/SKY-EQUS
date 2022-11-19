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

#include "cmmnrsrc.h"
#include "ml_typw.h" 
#include "wap.ifc"

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
	XMK_SEND_TMP_VARS
    yPDef_WTAResponse var;

#ifdef LOG_EXTERNAL
    CLNTa_log(0, 0, "WTAIc_publicMakeCallResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
#endif

    var.Param1 = objectID;
	var.Param2 = result;            
    var.Param3 = NULL;

    XMK_SEND_ENV( ENV,
                  WTAResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAResponse ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}

EXPORT_C VOID    WTAIc_publicSendDTMFResponse (UINT8 objectID, INT8 result)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAResponse var;

#ifdef LOG_EXTERNAL
    CLNTa_log(0, 0, "WTAIc_publicSendDTMFResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
#endif

    var.Param1 = objectID;
	var.Param2 = result;             
    var.Param3 = NULL;

    XMK_SEND_ENV( ENV,
                  WTAResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAResponse ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}

EXPORT_C VOID    WTAIc_publicAddPBEntryResponse (UINT8 objectID, INT8 result)
{
	XMK_SEND_TMP_VARS
	yPDef_WTAResponse var;

#ifdef LOG_EXTERNAL
    CLNTa_log(0, 0, "WTAIc_publicAddPBEntryResponse\nUINT8 \t objectID \t %i\n, INT8 \t result \t %i\n", (int)objectID, (int)result);
#endif

    var.Param1 = objectID;
	var.Param2 = result;           
    var.Param3 = NULL;

    XMK_SEND_ENV( ENV,
                  WTAResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAResponse ),
                  &var,
                  GLOBALPID(XPTID_UA_ME,0));
}



#ifdef CONFIG_WTA


VOID help_func_int  (UINT8 objectID, INT16 result)
{
	XMK_SEND_TMP_VARS
	yPDef_WTAResponse var;

    var.Param1 = objectID;
	var.Param2 = result;              
    var.Param3 = NULL;

    XMK_SEND_ENV( ENV,
                  WTAResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAResponse ),
                  &var,
			 	  GLOBALPID(XPTID_UA_ME,0));
}


VOID help_func_int_wchar  (UINT8 objectID, INT16 result, const WCHAR *fieldValue)
{
	XMK_SEND_TMP_VARS
	yPDef_WTAResponse var;

    var.Param1 = objectID;
	var.Param2 = result;              
    var.Param3 = (WCHAR*)fieldValue;

    XMK_SEND_ENV( ENV,
                  WTAResponse,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAResponse ),
                  &var,
			 	  GLOBALPID(XPTID_UA_ME,0));
}


EXPORT_C VOID   WTAIc_voiceCallSetupResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallSetupResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_voiceCallAcceptResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallAcceptResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_voiceCallReleaseResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallReleaseResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID   WTAIc_voiceCallSendDTMFResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallSendDTMFResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID   WTAIc_voiceCallCallStatusResponse  (UINT8 objectID, INT8 result, const WCHAR *fieldValue)
{
  help_func_int_wchar(objectID, result, fieldValue);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallCallStatusResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nconst WCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
#endif
}

EXPORT_C VOID	WTAIc_voiceCallListResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_voiceCallListResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}



EXPORT_C VOID	WTAIc_netTextSendResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_netTextSendResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_netTextListResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_netTextListResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_netTextRemoveResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_netTextRemoveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_netTextGetFieldValueResponse  (UINT8 objectID, INT8 result, const WCHAR *fieldValue)
{
  help_func_int_wchar(objectID, result, fieldValue);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_netTextGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nconst WCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
#endif
}

EXPORT_C VOID	WTAIc_netTextMarkAsReadResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_netTextMarkAsReadResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}



EXPORT_C VOID	WTAIc_phoneBookWriteResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_phoneBookWriteResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_phoneBookSearchResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_phoneBookSearchResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_phoneBookRemoveResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_phoneBookRemoveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_phoneBookGetFieldValueResponse  (UINT8 objectID, INT8 result, const WCHAR *fieldValue)
{
  help_func_int_wchar(objectID, result, fieldValue);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_phoneBookGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nconst WCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
#endif
}

EXPORT_C VOID	WTAIc_phoneBookChangeResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_phoneBookChangeResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}



EXPORT_C VOID	WTAIc_miscSetIndicatorResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_miscSetIndicatorResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}



EXPORT_C VOID	WTAIc_callLogDialledResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_callLogDialledResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_callLogMissedResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_callLogMissedResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_callLogReceivedResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_callLogReceivedResponse\nUINT8 \t objectID \t %i\nINT16 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_callLogGetFieldValueResponse  (UINT8 objectID, INT8 result, const WCHAR *fieldValue)
{
  help_func_int_wchar(objectID, result, fieldValue);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_callLogGetFieldValueResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\nconst WCHAR * \t fieldvalue \t %s\n", (int)objectID, (int)result, fieldValue);
#endif
}



EXPORT_C VOID	WTAIc_GSMHoldResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMHoldResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMRetrieveResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMRetrieveResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMTransferResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMTransferResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMDeflectResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMDeflectResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMMultipartyResponse  (UINT8 objectID, INT16 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMMultipartyResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMSeparateResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMSeparateResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMSendUSSDResponse  (UINT8 objectID, INT8 result)
{
  help_func_int(objectID, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMSendUSSDResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %i\n", (int)objectID, (int)result);
#endif
}

EXPORT_C VOID	WTAIc_GSMNetinfoResponse (UINT8 objectID, const WCHAR *result)
{
  help_func_int_wchar(objectID, 0, result);
#ifdef LOG_EXTERNAL
  CLNTa_log(0, 0, "WTAIc_GSMNetinfoResponse\nUINT8 \t objectID \t %i\nINT8 \t result \t %s\n", (int)objectID, result);
#endif
}





/* Events */


EXPORT_C VOID    WTAIc_incomingCall (INT16 callHandle, const CHAR *callerId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const char  *_callerId = callerId;
        if (!_callerId)
        {
            _callerId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_incomingCall\nINT16 \t callHandle \t %i\n, const CHAR * \t callerId \t %s\n", (int)callHandle, _callerId);
    }
#endif

    var.Param2 = wtaiEventIncomingCall(callHandle, (const unsigned char *)callerId);
    var.Param1 = wip_byte2wchar("wtaev-cc/ic");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_callCleared (INT16 callHandle, INT8 result)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callCleared\nINT16 \t callHandle \t %i\n, INT8 \t result \t %i\n", (int)callHandle, (int)result);
    }
#endif

    var.Param2 = wtaiEventCallCleared(callHandle, result);
    var.Param1 = wip_byte2wchar("wtaev-cc/cl");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_callConnected (INT16 callHandle, const CHAR *callerId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const char  *_callerId = callerId;
        if (!_callerId)
        {
            _callerId = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_callConnected\nINT16 \t callHandle \t %i\n, const CHAR * \t callerId \t %s\n", (int)callHandle, _callerId);
    }
#endif

    var.Param2 = wtaiEventCallConnected(callHandle, (const unsigned char *)callerId);
    var.Param1 = wip_byte2wchar("wtaev-cc/co");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_outgoingCall (INT16 callHandle, const CHAR *number)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const char  *_number = number;
        if (!_number)
        {
            _number = cszNULL;
        }
        CLNTa_log(0, 0, "WTAIc_outgoingCall\nINT16 \t callHandle \t %i\n, const CHAR * \t number \t %s", (int)callHandle, _number);
    }
#endif

    var.Param2 = wtaiEventOutgoingCall (callHandle, (const unsigned char *)number);
    var.Param1 = wip_byte2wchar("wtaev-cc/oc");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_callAlerting (INT16 callHandle)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callAlerting\nINT16 \t callHandle \t %i", (int)callHandle);
    }
#endif

    var.Param2 = wtaiEventCallAlerting(callHandle);
    var.Param1 = wip_byte2wchar("wtaev-cc/cc");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_DTMFSent (INT16 callHandle, const WCHAR *dtmf)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_dtmf = dtmf;
        if (!_dtmf)
        {
            _dtmf = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_DTMFSent\nINT16 \t callHandle \t %i\nconst WCHAR * \t dtmf \t %S\n", (int)callHandle, _dtmf);
    }
#endif

    var.Param2 = wtaiEventDTMFSent(callHandle, dtmf);
    var.Param1 = wip_byte2wchar("wtaev-cc/dtmf");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_networkStatus (INT8 inService, const WCHAR *networkName, INT8 explanation)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_networkName = networkName;
        if (!_networkName)
        {
            _networkName = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_networkStatus\nINT8 \t inService \t %i\n, const WCHAR * \t networkName \t %S\n, INT8 \t explanation \t %i\n", (int)inService, _networkName, (int)explanation);
    }
#endif

    var.Param2 = wtaiEventNetworkStatus(inService, networkName, explanation);
    var.Param1 = wip_byte2wchar("wtaev-ms/ns");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_incomingMessage (INT16 textId, const WCHAR *sender)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_sender = sender;
        if (!_sender)
        {
            _sender = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAIc_incomingMessage\nINT16 \t textId \t %i\n, const WCHAR * \t sender \t %S\n", (int)textId, _sender);
    }
#endif

    var.Param2 = wtaiEventIncomingMessage(textId, sender);
    var.Param1 = wip_byte2wchar("wtaev-nt/it");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_messageSendStatus (INT16 msgHandle, INT8 textId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_messageSendStatus\nINT16 \t msgHandle \t %i\nINT8 \t textId \t %i\n", (int)msgHandle, (int)textId);
    }
#endif

    var.Param2 = wtaiEventMessageSendStatus(msgHandle, textId);
    var.Param1 = wip_byte2wchar("wtaev-nt/st");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}


EXPORT_C VOID    WTAIc_callHeld (INT16 callHandle)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callHeld\nINT16 \t callHandle \t %i\n", (int)callHandle);
    }
#endif

    var.Param2 = wtaiEventCallHeld(callHandle);
    var.Param1 = wip_byte2wchar("wtaev-gsm/ch");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_callActive (INT16 callHandle)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAIc_callActive\nINT16 \t callHandle \t %i\n", (int)callHandle);
    }
#endif

    var.Param2 = wtaiEventCallActive(callHandle);
    var.Param1 = wip_byte2wchar("wtaev-gsm/ca");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAIc_USSDReceived (const WCHAR *text, const WCHAR *dataCodingScheme, INT8 type, const WCHAR *transactionId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAEvent var;

#ifdef LOG_EXTERNAL
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

    var.Param2 = wtaiEventUSSD_Received(text, dataCodingScheme, type, transactionId);
    var.Param1 = wip_byte2wchar("wtaev-gsm/ru");

    XMK_SEND_ENV( ENV,
                  WTAEvent,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAEvent ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}




/* Services */

EXPORT_C VOID    WTAc_terminateService (VOID)
{
	XMK_SEND_TMP_VARS

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_terminateService\nVOID");
    }
#endif

    XMK_SEND_ENV( ENV,
                  TerminateWTAService,
                  xDefaultPrioSignal,
                  0,
                  NULL,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAc_confirmInstallation (INT8 installId, BOOL install)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcConfirmInstallation var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_confirmInstallation\nINT8 \t installId \t %i\n, BOOL \t install \t %u\n", (int)installId, (int)install);
    }
#endif

    var.Param1 = installId;
    var.Param2 = install;

    XMK_SEND_ENV( ENV,
                  WTAcConfirmInstallation,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcConfirmInstallation ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_retryGetInstallationResult (INT8 installId, BOOL retry)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcRetryGetInstallationResult var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_retryGetInstallationResult\nINT8 \t installId \t %i\n, BOOL \t retry \t %u\n", (int)installId, (int)retry);
    }
#endif

    var.Param1 = installId;
    var.Param2 = retry;

    XMK_SEND_ENV( ENV,
                  WTAcRetryGetInstallationResult,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcRetryGetInstallationResult ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_showInstallationResult (INT8 installId, BOOL show)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcShowInstallationResult var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_showInstallationResult\nINT8 \t installId \t %i\n, BOOL \t show \t %u\n", (int)installId, (int)show);
    }
#endif

    var.Param1 = installId;
    var.Param2 = show;

    XMK_SEND_ENV( ENV,
                  WTAcShowInstallationResult,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcShowInstallationResult ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_abortInstallation (INT8 installId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcAbortInstallation var;

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_abortInstallation\nINT8 \t installId \t %i\n", (int)installId);
    }
#endif

    var.Param1 = installId;

    XMK_SEND_ENV( ENV,
                  WTAcAbortInstallation,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcAbortInstallation ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_getServices (VOID)
{
	XMK_SEND_TMP_VARS

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_getServices\nVOID\n");
    }
#endif

    XMK_SEND_ENV( ENV,
                  WTAcGetServices,
                  xDefaultPrioSignal,
                  0,
                  NULL,
                  GLOBALPID(XPTID_RepH_ME,0));
}


EXPORT_C VOID    WTAc_serviceAccessible (const WCHAR *wtaChannelId, BOOL status)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcServiceAccessible var;
    WCHAR   *wz;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_wtaChannelId = wtaChannelId;
        if (!_wtaChannelId)
        {
            _wtaChannelId = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAc_serviceAccessible\nconst WCHAR * \t wtaChannelId \t %S\nBOOL \t status \t %u\n", _wtaChannelId, (int)status);
    }
#endif

    wz = NULL;
    if (wtaChannelId)
    {
        size_t  len;
        /*  Add one character for the terminating zero. */
        len = (w_strlen(wtaChannelId) + 1) * sizeof(WCHAR);
        wz = (WCHAR *)OSConnectorAlloc(len);
        if (!wz)
            return;
        memcpy(wz, wtaChannelId, len);
    }

    var.Param1 = wz;
    var.Param2 = status;

    XMK_SEND_ENV( ENV,
                  WTAcServiceAccessible,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcServiceAccessible ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_deleteService (const WCHAR *wtaChannelId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAcDeleteService var;
    WCHAR   *wz;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_wtaChannelId = wtaChannelId;
        if (!_wtaChannelId)
        {
            _wtaChannelId = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAc_deleteService\nconst WCHAR * \t wtaChannelId \t %S\n", _wtaChannelId);
    }
#endif

    wz = NULL;
    if (wtaChannelId)
    {
        size_t  len;
        /*  Add one character for the terminating zero. */
        len = (w_strlen(wtaChannelId) + 1) * sizeof(WCHAR);
        wz = (WCHAR *)OSConnectorAlloc(len);
        if (!wz)
            return;
        memcpy(wz, wtaChannelId, len);
    }
    var.Param1 = wz;

    XMK_SEND_ENV( ENV,
                  WTAcDeleteService,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAcDeleteService ),
                  &var,
                  GLOBALPID(XPTID_RepH_ME,0));
}

EXPORT_C VOID    WTAc_executeService (const WCHAR *wtaChannelId)
{
	XMK_SEND_TMP_VARS
    yPDef_WTAExecuteService var;
    WCHAR   *wz;

#ifdef LOG_EXTERNAL
    {
        const WCHAR *_wtaChannelId = wtaChannelId;
        if (!_wtaChannelId)
        {
            _wtaChannelId = cwzNULL;
        }
        CLNTa_log(0, 0, "WTAc_executeService\nconst WCHAR * \t wtaChannelId \t %S\n", _wtaChannelId);
    }
#endif

    wz = NULL;
    if (wtaChannelId)
    {
        size_t  len;
        /*  Add one character for the terminating zero. */
        len = (w_strlen(wtaChannelId) + 1) * sizeof(WCHAR);
        wz = (WCHAR *)OSConnectorAlloc(len);
        if (!wz)
            return;
        memcpy(wz, wtaChannelId, len);
    }
    var.Param1 = wz;

    XMK_SEND_ENV( ENV,
                  WTAExecuteService,
                  xDefaultPrioSignal,
                  sizeof( yPDef_WTAExecuteService ),
                  &var,
                  GLOBALPID(XPTID_WTA_IF,0));
}

EXPORT_C VOID    WTAc_clearServices (VOID)
{
	XMK_SEND_TMP_VARS

#ifdef LOG_EXTERNAL
    {
        CLNTa_log(0, 0, "WTAc_clearServices\nVOID\n");
    }
#endif

    XMK_SEND_ENV( ENV,
                  WTAcClearServices,
                  xDefaultPrioSignal,
                  0,
                  NULL,
                  GLOBALPID(XPTID_RepH_ME,0));
}


#endif
