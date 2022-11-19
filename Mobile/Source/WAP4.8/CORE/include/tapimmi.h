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
#ifndef TAPIMMI_H
#define TAPIMMI_H


#include "wiptrgt.h"
#include "wipprdct.h"
#include "tapicmmn.h"


/* These UA-defines must be less than 100. */
#define WML_USER_AGENT     1
#define WTA_USER_AGENT     2


typedef INT8    TextAlignmentType;

#define ALIGN_LEFT      (TextAlignmentType)0
#define ALIGN_CENTER    (TextAlignmentType)1
#define ALIGN_RIGHT     (TextAlignmentType)2


typedef INT8    ImageAlignmentType;

#define ALIGN_BOTTOM    (ImageAlignmentType)0
#define ALIGN_MIDDLE    (ImageAlignmentType)1
#define ALIGN_TOP       (ImageAlignmentType)2


typedef INT8    PercentType;

#define NONE_IS_PERCENT     (PercentType)0
#define WIDTH_IS_PERCENT    (PercentType)1
#define HEIGHT_IS_PERCENT   (PercentType)2
#define VSPACE_IS_PERCENT   (PercentType)4
#define HSPACE_IS_PERCENT   (PercentType)8


typedef INT8    FormatType;

#define TXT_NORMAL      (FormatType) 0
#define TXT_SMALL       (FormatType) 1
#define TXT_BIG         (FormatType) 2
#define TXT_BOLD        (FormatType) 4
#define TXT_ITALIC      (FormatType) 8
#define TXT_UNDERLINE   (FormatType)16
#define TXT_EMPHASIS    (FormatType)32
#define TXT_STRONG      (FormatType)64


typedef UINT8   StatusType;

/*  status          Description */
/*  WAE Status  */
#define ContentIsOpened                 (StatusType)  1 /*  Used when a link or another navigation task is executed and content is downloaded. Content can be a WML deck, WMLS byte package or any unknown content. */
#define ContentIsDone                   (StatusType)  2 /*  Used when the content finally is downloaded. */
#define ImageIsOpened                   (StatusType)  3 /*  Used when an image in a WML deck is downloaded. */
#define ImageIsDone                     (StatusType)  4 /*  Used when the image finally is downloaded. */
#define ScriptIsRunning                 (StatusType)  5 /*  Used when a WML script has started its execution. */
#define ScriptIsDone                    (StatusType)  6 /*  Used when the WML script is done with the execution. */

#define Redirect                        (StatusType)  7 /*  Used when a Redirect statuscode from WSP is detected. */
#define ReadFromCache                   (StatusType)  8 /*  Used when WSPIF get the content from the cache. */
#define ReadFromNetwork                 (StatusType)  9 /*  Used when WSPIF get the content from the Server. */
#define CheckForNewerContent            (StatusType) 10 /*  Used when WSPIF check for newer content at the Server then exists in the cache. */
#define ReadFromNetworkDone             (StatusType) 11 /*  Used when WSPIF received SUnitMethodResultInd. */


#define WSPSessionIsSetup               (StatusType) 12 /*  ..........  */
#define WSPSessionIsDone                (StatusType) 13 /*  ..........  */
#define LoadingData                     (StatusType) 14 /*  ..........  */
#define LoadingDataDone                 (StatusType) 15 /* ...........  */


#ifdef CONFIG_WTA
#define WTAServiceStarted               (StatusType)100 /*  ONLY used in WTA mode */
#define WTAServiceTerminated            (StatusType)101 /*  ONLY used in WTA mode */
#define WTAInstallingServiceInitiated   (StatusType)102 /*  ONLY used in WTA mode */
#define WTAInstallingServiceComplete    (StatusType)103 /*  ONLY used in WTA mode */
#define WTAServiceUnloadingInitiated    (StatusType)104 /*  ONLY used in WTA mode */
#define WTAUnloadServiceComplete	    (StatusType)105 /*  ONLY used in WTA mode */
#endif


#define SKTEncryptionIsRunning          (StatusType)110 /*  ONLY used in SKT_Security mode */
#define SKTEncryptionIsRunningDone      (StatusType)111 /*  ONLY used in SKT_Security mode */


#ifdef CONFIG_PUSH
#define PushStarted                     (StatusType)200 /*  ONLY used in Push mode */
#endif

#ifdef CONFIG_WTLS
#define WTLSConnectionEstablished       (StatusType)230
#define WTLSConnectionTerminated        (StatusType)231
#endif


typedef INT8    LinkType;

#define WIDGET_IMAGELINK  (LinkType)1
#define WIDGET_TEXTLINK   (LinkType)2
#define WIDGET_DOLINK     (LinkType)3
#define WIDGET_OPTIONLINK (LinkType)4


/*  predefined view ids (when configuring values for a specific static UA 
e.g. the repository use these predefined viewids */
#define ALL_USER_AGENT      0
#define CONTENT_USER_AGENT    128
#define REPOSITORY_USER_AGENT 129
#define PUSH_USER_AGENT       130


#ifdef CAN_SIGN_TEXT
/* Values for keyIdType parameter in MMIa_signText: */
#define SIGN_NO_KEY         0
#define SIGN_SHA_KEY        1
#define SIGN_SHA_CA_KEY     2

/* Values for options parameter in MMIa_signText: */
#define SIGN_RETURN_HASHED_KEY      0x2
#define SIGN_RETURN_CERTIFICATE     0x4

/* Values for algorithm parameter in MMIc_textSigned: */
#define SIGN_ALG_RSA        1
#define SIGN_ALG_ECDSA      2

/* Values for certificateType parameter in MMIc_textSigned: */
#define SIGN_WTLS_CERTIFICATE   2
#define SIGN_X509_CERTIFICATE   3
#define SIGN_X968_CERTIFICATE   4
#define SIGN_URL_CERTIFICATE    5

/* Values for err parameter in MMIc_textSigned: */
#define SIGN_NO_ERROR               0
#define SIGN_MISSING_CERTIFICATE    1
#define SIGN_USER_CANCELED          2
#define SIGN_OTHER_ERROR            3
#endif


#endif
