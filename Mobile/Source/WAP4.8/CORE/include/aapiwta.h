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


#include "wipprdct.h"
#include "tapicmmn.h"


VOID    WTAIa_publicMakeCall (UINT8 objectId, const CHAR *number);

VOID    WTAIa_publicSendDTMF (UINT8 objectId, const CHAR *number);

VOID    WTAIa_publicAddPBEntry (UINT8 objectId, const CHAR *number, const WCHAR *name);



#if defined CONFIG_WTA





/* Script function - voiceCallCallStatus - 513.4 */
typedef INT8    CallStatusType;

#define WTA_VC_CS_NUMBER				(CallStatusType)0
#define WTA_VC_CS_STATUS				(CallStatusType)1
#define WTA_VC_CS_MODE					(CallStatusType)2
#define WTA_VC_CS_NAME					(CallStatusType)3
#define WTA_VC_CS_DURATION				(CallStatusType)4


/* Script function - netTextList - 514.1 */
typedef INT8	NetTextListType;

#define WTA_NT_LIST_ALL					(NetTextListType)0
#define WTA_NT_LIST_ONLY_UNREAD			(NetTextListType)1
#define WTA_NT_LIST_ONLY_READ			(NetTextListType)2
#define WTA_NT_LIST_ONLY_UNSENT			(NetTextListType)3


/* Script function - netTextGetFieldValue - 514.3 */
typedef INT8	NetTextGetType;

#define WTA_NT_GET_TEXT					(NetTextGetType)0
#define WTA_NT_GET_TSTAMP				(NetTextGetType)1
#define WTA_NT_GET_ADDRESS				(NetTextGetType)2
#define WTA_NT_GET_STATUS				(NetTextGetType)3
#define WTA_NT_GET_READ					(NetTextGetType)4
#define WTA_NT_GET_TSTAMP_OFF			(NetTextGetType)5
#define WTA_NT_GET_TSTAMP_DEVICE		(NetTextGetType)6


/* Script function - phoneBookSearch - 515.1 */
typedef INT8    PhoneBookSearchType;

#define WTA_PB_SEARCH_CONTINUE					(PhoneBookSearchType)0
#define WTA_PB_SEARCH_NAME						(PhoneBookSearchType)1
#define WTA_PB_SEARCH_NUMBER					(PhoneBookSearchType)2


/* Script function - phoneBookGetFieldValue - 515.3 */
typedef INT8    PhoneBookGetType;

#define WTA_PB_GET_NAME							(PhoneBookGetType)0
#define WTA_PB_GET_NUMBER						(PhoneBookGetType)1


/* Script function - phoneBookChange - 515.4 */
typedef INT8    PhoneBookChangeType;

#define WTA_PB_CHANGE_NAME						(PhoneBookChangeType)0
#define WTA_PB_CHANGE_NUMBER					(PhoneBookChangeType)1


/* Script function - callLogGetFieldValue - 519.3 */
typedef INT8    CallLogGetType;

#define WTA_CL_GET_NUMBER					(CallLogGetType)0
#define WTA_CL_GET_TSTAMP 					(CallLogGetType)1
#define WTA_CL_GET_EXPLANATION 				(CallLogGetType)2


/* Script function - miscSetIndicator - 516.0 */
typedef INT8    MiscSetIndicatorType;

#define WTA_MISC_INCOMING_SPEECH		(MiscSetIndicatorType)0
#define WTA_MISC_INCOMING_DATA			(MiscSetIndicatorType)1
#define WTA_MISC_INCOMING_FAX			(MiscSetIndicatorType)2
#define WTA_MISC_CALL_WAITING			(MiscSetIndicatorType)3
#define WTA_MISC_TEXT_MESSAGE			(MiscSetIndicatorType)4
#define WTA_MISC_VOICE_MAIL				(MiscSetIndicatorType)5
#define WTA_MISC_FAX_MESSAGE			(MiscSetIndicatorType)6
#define WTA_MISC_EMAIL_MESSAGE			(MiscSetIndicatorType)7


/* Script function - GSMSendUSSD - 518.6 */
typedef INT8    GSMSendUSSDType;

#define WTA_GSM_SEND_REQUEST		(GSMSendUSSDType)0
#define WTA_GSM_SEND_REQUEST_RESULT	(GSMSendUSSDType)1
#define WTA_GSM_SEND_NOTIFY_RESULT	(GSMSendUSSDType)2


/* Script function - GSMNetinfo - 518.7 */
typedef INT8	GSMNetinfoType;

#define WTA_GSM_NETINFO_NO			(GSMNetinfoType)0
#define WTA_GSM_NETINFO_SIX_BEST	(GSMNetinfoType)1
#define WTA_GSM_NETINFO_ALL			(GSMNetinfoType)2



typedef struct  ServiceType
{
    WCHAR   *title;
    WCHAR   *abstract;
    WCHAR   *wtaChannelId;
	BOOL	useraccess;
    BOOL    bServiceAccessible;
} ServiceType, *pServiceType;



/* Functions */

VOID    WTAIa_voiceCallSetup (UINT8 objectId, const WCHAR *wtaChannelId, const CHAR *number);

VOID    WTAIa_voiceCallAccept (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle);

VOID    WTAIa_voiceCallRelease (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, BOOL enableResponse);

VOID    WTAIa_voiceCallSendDTMF (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, const CHAR *dtmf);

VOID    WTAIa_voiceCallCallStatus (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, INT8 field);

VOID    WTAIa_voiceCallList (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst);



VOID    WTAIa_netTextSend (UINT8 objectId, const WCHAR *wtaChannelId, const CHAR *number, const WCHAR *text);

VOID    WTAIa_netTextList (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst, INT8 messageType);

VOID    WTAIa_netTextRemove (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle);

VOID    WTAIa_netTextGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle, INT8 field);

VOID    WTAIa_netTextMarkAsRead (UINT8 objectId, const WCHAR *wtaChannelId, INT16 msgHandle);



VOID    WTAIa_phoneBookWrite (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, const CHAR *number, const WCHAR *name);

VOID    WTAIa_phoneBookSearch (UINT8 objectId, const WCHAR *wtaChannelId, INT8 field, const WCHAR *value);

VOID    WTAIa_phoneBookRemove (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index);

VOID    WTAIa_phoneBookGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, INT8 field);

VOID    WTAIa_phoneBookChange (UINT8 objectId, const WCHAR *wtaChannelId, INT16 index, INT8 field, const WCHAR *newValue);



VOID    WTAIa_miscSetIndicator (UINT8 objectId, const WCHAR *wtaChannelId, INT8 type, INT8 newState);



VOID    WTAIa_callLogDialled (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst);

VOID    WTAIa_callLogMissed (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst);

VOID    WTAIa_callLogReceived (UINT8 objectId, const WCHAR *wtaChannelId, BOOL returnFirst);

VOID    WTAIa_callLogGetFieldValue (UINT8 objectId, const WCHAR *wtaChannelId, INT16 logHandle, INT8 field);



VOID    WTAIa_GSMHold (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle);

VOID    WTAIa_GSMRetrieve (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle);

VOID    WTAIa_GSMTransfer (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandleB, INT16 callHandleC);

VOID    WTAIa_GSMDeflect (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle, const CHAR *number);

VOID    WTAIa_GSMMultiparty (UINT8 objectId, const WCHAR *wtaChannelId);

VOID    WTAIa_GSMSeparate (UINT8 objectId, const WCHAR *wtaChannelId, INT16 callHandle);

VOID    WTAIa_GSMSendUSSD (UINT8 objectId, const WCHAR *wtaChannelId, const WCHAR *message, const WCHAR *codingScheme, INT8 type, const CHAR *transactionId);

VOID    WTAIa_GSMNetinfo (UINT8 objectId, const WCHAR *wtaChannelId, INT8 type);



/* Services */

VOID    WTAa_confirmInstallation (INT8 installId, const WCHAR *wtaChannelId, const WCHAR *title, const WCHAR *abstract);

VOID    WTAa_retryGetInstallationResult (INT8 installId);

VOID    WTAa_showInstallationResult (INT8 installId, const CHAR *url);

VOID    WTAa_services (const ServiceType * const *services);

VOID	WTAa_serviceDeleted (const WCHAR *wtaChannelId);



/* Events */

VOID    WTAa_processedByAService (BOOL processed);


#endif


#endif
