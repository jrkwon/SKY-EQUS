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
#ifndef AAPIWTA_H
#define AAPIWTA_H


#include "tapicmmn.h"


VOID    WTAIa_publicMakeCall (const CHAR *number);

VOID    WTAIa_publicSendDTMF (const CHAR *number);

INT8    WTAIa_publicPBwrite (const CHAR *number, const WCHAR *name);


#if defined CONFIG_WTA


typedef INT8    CallStatusType;

#define WTA_CS_NUMBER       (CallStatusType)1
#define WTA_CS_NAME         (CallStatusType)2
#define WTA_CS_DURATION     (CallStatusType)3
#define WTA_CS_DURATION_HMS (CallStatusType)4
#define WTA_CS_STATE        (CallStatusType)5


typedef INT8    MiscType;

#define WTA_MI_INC_CALL (MiscType)0
#define WTA_MI_INC_DAT  (MiscType)1
#define WTA_MI_INC_FAX  (MiscType)2
#define WTA_MI_CLL_WAI  (MiscType)3
#define WTA_MI_REC_TXT  (MiscType)4
#define WTA_MI_VOI_MAI  (MiscType)5
#define WTA_MI_FAX_NOT  (MiscType)6
#define WTA_MI_MAI_NOT  (MiscType)7


typedef INT8    MiscOpType;

#define WTA_MI_ACTIVATE (MiscOpType)1
#define WTA_MI_RESET    (MiscOpType)2


typedef INT8    TextMessageType;

#define WTA_TXT_TXT (TextMessageType)0
#define WTA_TXT_TST (TextMessageType)1
#define WTA_TXT_TSO (TextMessageType)2
#define WTA_TXT_TSR (TextMessageType)3
#define WTA_TXT_ADR (TextMessageType)4
#define WTA_TXT_STT (TextMessageType)5


typedef INT8    PhoneBookType;

#define WTA_PB_ID       (PhoneBookType)0
#define WTA_PB_NAME     (PhoneBookType)1
#define WTA_PB_NUMBER   (PhoneBookType)2
#define WTA_PB_RESET    (PhoneBookType)3


typedef INT8    USSDopType;

#define WTA_USSD_PRC_REQ    (USSDopType)0
#define WTA_USSD_REQ        (USSDopType)1
#define WTA_USSD_NOT        (USSDopType)2
#define WTA_USSD_PRC_ERR    (USSDopType)3


typedef struct  ServiceType
{
    WCHAR   *title;
    WCHAR   *abstract;
    WCHAR   *eventId;
} ServiceType;


INT8    WTAIa_setupVoiceCall (const CHAR *number);

INT8    WTAIa_acceptCall (INT8 callId);

INT8    WTAIa_releaseCall (INT8 callId);

INT8    WTAIa_listCall (INT8 callIndex);

INT8    WTAIa_sendDTMFstring (const CHAR *dtmf);

WCHAR * WTAIa_callStatus (INT8 callId, INT8 field, INT8 *result);

VOID    WTAIa_miscIndication (INT8 type, INT8 operation, INT8 count);

INT16   WTAIa_lastDialledNumber (INT16 callIndex);

INT16   WTAIa_missedCalls (INT16 callIndex);

INT16   WTAIa_receivedCalls (INT16 callIndex);

WCHAR * WTAIa_callLogGetFieldValue (INT16 handle, INT8 field, INT8 *result);

INT8    WTAIa_sendText (const CHAR *number, const WCHAR *text);

INT16   WTAIa_readText (INT16 textIndex);

INT8    WTAIa_removeText (INT16 handle);

WCHAR * WTAIa_textGetFieldValue (UINT16 handle, INT8 field, INT8 *result);

INT16   WTAIa_phonebookWrite (INT16 index, const CHAR *number, const WCHAR *name);

INT16   WTAIa_phonebookRead (INT8 field, const WCHAR *value);

INT8    WTAIa_phonebookRemove (INT16 index);

WCHAR * WTAIa_phonebookGetFieldValue (INT16 index, INT8 field, INT8 *result);

INT8    WTAIa_changePhonebookEntry (INT16 index, INT8 field, const WCHAR *value);

INT8    WTAIa_GSMcallReject (INT8 callId);

INT8    WTAIa_GSMexplicitCallTransfer (INT8 callIdB, INT8 callIdC);

INT8    WTAIa_GSMcallDeflection (INT8 callId, const CHAR *destination);

INT8    WTAIa_GSMcallHold (INT8 callId);

INT8    WTAIa_GSMjoinMultiparty (VOID);

INT8    WTAIa_GSMretrieveFromMultiparty (INT8 callId);

UINT8 * WTAIa_GSMprovideLocationInformation (INT8 *result);

INT8    WTAIa_sendUSSD (const WCHAR *text, const WCHAR *dataCodingScheme, INT8 type, const CHAR *transactionId);

VOID    WTAa_confirmInstallation (INT8 installId, const WCHAR *title, const WCHAR *abstract);

VOID    WTAa_retryGetInstallationResult (INT8 installId);

VOID    WTAa_showInstallationResult (INT8 installId, const CHAR *url);

VOID    WTAa_services (const ServiceType * const *services);

VOID    WTAa_processedByAService (BOOL processed);

#endif


#endif
