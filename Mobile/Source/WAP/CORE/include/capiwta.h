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
#ifndef CAPIWTA_H
#define CAPIWTA_H


#include "tapicmmn.h"
#include "wipprdct.h"

/* Error-codes */
#define WTA_SUCCESS						   0
#define	WTA_UNSPECIFIED_ERROR			  -1
#define WTA_INVALID						-128

#define WTA_PUB_BUSY					-105
#define WTA_PUB_NO_NETWORK				-106
#define WTA_PUB_NO_ANSWER				-107
#define WTA_PUB_NO_CONNECTION			-108


#if defined CONFIG_WTA

#define WTA_PB_NAME_INVALID				-100
#define WTA_PB_NUMBER_INVALID			-101
#define WTA_PB_NUMBER_TOO_LONG			-102
#define WTA_PB_ENTRY_NOT_WRITTEN		-103
#define WTA_PB_FULL						-104
#define WTA_PB_ENTRY_NOT_REMOVABLE		-105
#define WTA_PB_FIELD_NOT_SUPPORTED		-106
#define WTA_PB_VALUE_INVALID			-107

#define WTA_NET_TEXT_TOO_LONG			-100
#define WTA_NET_NO_REMOVE				-101

#define WTA_GSM_USSD_IN_PROGRESS		-100
#define WTA_GSM_ILLEGAL_CHAR			-101
#define WTA_GSM_NO_NETWORK				-106


/* Event - callCleard - wtaev-cc/cl */
#define WTA_CC_CL_NORMAL			(CallClearedType)0
#define WTA_CC_CL_UNSPECIFIED		(CallClearedType)1
#define WTA_CC_CL_NETWORK_SPECIFIC	(CallClearedType)2
#define WTA_CC_CL_DROPPED			(CallClearedType)3
#define WTA_CC_CL_BUSY				(CallClearedType)4
#define WTA_CC_CL_NO_NETWORK		(CallClearedType)5
#define WTA_CC_CL_NO_ANSWER			(CallClearedType)6

#ifdef CONFIG_WTAI_GSM
#define WTA_CC_CL_MULTI_OK			(CallClearedType)100
#define WTA_CC_CL_MULTI_UNSPECIFIED	(CallClearedType)101
#endif

/* Event - messageSendStatus - wtaev-nt/st */
typedef INT8	MessageSendStatusType;

#define WTA_NT_ST_MESSAGE_SENT		(MessageSendStatusType)0
#define WTA_NT_ST_UNSPECIFIED		(MessageSendStatusType)1
#define WTA_NT_ST_NO_NETWORK		(MessageSendStatusType)2
#define WTA_NT_ST_NO_RESOURCE		(MessageSendStatusType)3


/* Event - networkStatus - wtaev-ms/ns */
typedef INT8	NetworkStatusType;

#define WTA_MS_NS_NOT_IN_SERVICE	(NetworkStatusType)0
#define WTA_MS_NS_IN_SERVICE		(NetworkStatusType)1
#define WTA_MS_NS_NO_EXPLANATION	(NetworkStatusType)0
#define WTA_MS_NS_NO_NETWORK		(NetworkStatusType)1
#define WTA_MS_NS_FORBIDDEN			(NetworkStatusType)2


/* Event - USSDReceived - wtaev-gsm/ru */
typedef INT8    USSDReceivedType;

#define WTA_USSD_RECEIVED_RESULT	(USSDReceivedType)0
#define WTA_USSD_RECEIVED_REQUEST	(USSDReceivedType)1
#define WTA_USSD_RECEIVED_NOTIFY	(USSDReceivedType)2
#define WTA_USSD_RECEIVED_ERROR		(USSDReceivedType)3

#endif


/* Responses */

IMPORT_C VOID	WTAIc_publicMakeCallResponse (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_publicSendDTMFResponse (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_publicAddPBEntryResponse (UINT8 objectID, INT8 result);


#if defined CONFIG_WTA

IMPORT_C VOID	WTAIc_voiceCallSetupResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_voiceCallAcceptResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_voiceCallReleaseResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID   WTAIc_voiceCallSendDTMFResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID   WTAIc_voiceCallCallStatusResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue);

IMPORT_C VOID	WTAIc_voiceCallListResponse  (UINT8 objectID, INT8 result);



IMPORT_C VOID	WTAIc_netTextSendResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_netTextListResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_netTextRemoveResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_netTextGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue);

IMPORT_C VOID	WTAIc_netTextMarkAsReadResponse  (UINT8 objectID, INT8 result);



IMPORT_C VOID	WTAIc_phoneBookWriteResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_phoneBookSearchResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_phoneBookRemoveResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_phoneBookGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue);

IMPORT_C VOID	WTAIc_phoneBookChangeResponse  (UINT8 objectID, INT8 result);



IMPORT_C VOID	WTAIc_miscSetIndicatorResponse  (UINT8 objectID, INT8 result);



IMPORT_C VOID	WTAIc_callLogDialledResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_callLogMissedResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_callLogReceivedResponse  (UINT8 objectID, INT16 result);

IMPORT_C VOID	WTAIc_callLogGetFieldValueResponse  (UINT8 objectID, INT8 result, WCHAR *fieldValue);



IMPORT_C VOID	WTAIc_GSMHoldResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMRetrieveResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMTransferResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMDeflectResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMMultipartyResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMSeparateResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMSendUSSDResponse  (UINT8 objectID, INT8 result);

IMPORT_C VOID	WTAIc_GSMNetinfoResponse (UINT8 objectID, WCHAR *result);



/* Events */

IMPORT_C VOID    WTAIc_incomingCall (INT8 callHandle, const CHAR *callerId);

IMPORT_C VOID    WTAIc_callCleared (INT8 callHandle, INT8 result);

IMPORT_C VOID    WTAIc_callConnected (INT8 callHandle, const CHAR *callerId);

IMPORT_C VOID    WTAIc_outgoingCall (INT8 callHandle, const CHAR *number);

IMPORT_C VOID    WTAIc_callAlerting (INT8 callHandle);

IMPORT_C VOID    WTAIc_DTMFSent (INT8 callHandle, const WCHAR *dtmf);



IMPORT_C VOID    WTAIc_networkStatus (INT8 inService, const WCHAR *networkName, INT8 explanation);

IMPORT_C VOID    WTAIc_incomingMessage (INT16 msgHandle, const WCHAR *sender);

IMPORT_C VOID    WTAIc_messageSendStatus (INT16 msgHandle, INT8 result);



IMPORT_C VOID    WTAIc_callHeld (INT8 callHandle);

IMPORT_C VOID    WTAIc_callActive (INT8 callHandle);

IMPORT_C VOID    WTAIc_USSDReceived (const WCHAR *message, const WCHAR *codingScheme, INT8 type, const WCHAR *transactionId);



/* Services */

IMPORT_C VOID    WTAIc_terminateService (VOID);

IMPORT_C VOID    WTAc_confirmInstallation (INT8 installId, BOOL install);

IMPORT_C VOID    WTAc_retryGetInstallationResult (INT8 installId, BOOL retry);

IMPORT_C VOID    WTAc_showInstallationResult (INT8 installId, BOOL show);

IMPORT_C VOID    WTAc_abortInstallation (INT8 installId);

IMPORT_C VOID    WTAc_getServices (VOID);

IMPORT_C VOID    WTAc_deleteService (const WCHAR *eventId);

IMPORT_C VOID    WTAc_executeService (const WCHAR *eventId);

IMPORT_C VOID    WTAc_clearServices (VOID);


#endif

#endif
