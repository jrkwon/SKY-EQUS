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


#if defined CONFIG_WTA


#define WTA_RES_ID_NOT_FOUND             -1
#define WTA_RES_ILLEGAL_PARAMETERS       -2
#define WTA_RES_SERVICE_NOT_AVAILABLE    -3
#define WTA_RES_SERVICE_TEMP_UNAVAILABLE -4
#define WTA_RES_BUSY                     -5
#define WTA_RES_NETWORK_BUSY             -6
#define WTA_RES_NO_ANSWER                -7
#define WTA_RES_UNKNOWN                  -8
#define WTA_RES_OUT_OF_MEMORY            -9
#define WTA_RES_USSD_PROGRESS           -64
#define WTA_RES_ILLEGAL_CHARACTERS      -65


IMPORT_C VOID    WTAIc_outgoingCallIndication (INT8 callId, const CHAR *callerId);

IMPORT_C VOID    WTAIc_connectingCallIndication (INT8 callId);

IMPORT_C VOID    WTAIc_incomingCall (INT8 callId, const CHAR *callerId);

IMPORT_C VOID    WTAIc_callConnected (INT8 callId, const CHAR *callerId);

IMPORT_C VOID    WTAIc_callCleared (INT8 callId, INT8 result);

IMPORT_C VOID    WTAIc_DTMFsent (const WCHAR *result);

IMPORT_C VOID    WTAIc_networkStatusIndication (BOOL camping, const WCHAR *networkName, INT8 notCampingCause);

IMPORT_C VOID    WTAIc_incomingText (INT16 textId, const WCHAR *sender);

IMPORT_C VOID    WTAIc_networkTextSent (INT16 textId);

IMPORT_C VOID    WTAIc_GSMcallActiveIndication (INT8 callId);

IMPORT_C VOID    WTAIc_GSMcallHeldIndication (INT8 callId);

IMPORT_C VOID    WTAIc_receivedUSSD (const WCHAR *text, const WCHAR *dataCodingScheme, INT8 type, const WCHAR *transactionId);

IMPORT_C VOID    WTAIc_terminateService (VOID);

IMPORT_C VOID    WTAc_confirmInstallation (INT8 installId, BOOL install);

IMPORT_C VOID    WTAc_retryGetInstallationResult (INT8 installId, BOOL retry);

IMPORT_C VOID    WTAc_showInstallationResult (INT8 installId, BOOL show);

IMPORT_C VOID    WTAc_abortInstallation (INT8 installId);

IMPORT_C VOID    WTAc_getServices (VOID);

IMPORT_C VOID    WTAc_deleteService (const CHAR *eventId);

IMPORT_C VOID    WTAc_executeService (const CHAR *eventId);

IMPORT_C VOID    WTAc_clearServices (VOID);


#endif

#endif
