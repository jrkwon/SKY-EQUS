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
#ifndef ERRCODES_H
#define ERRCODES_H


/*  the different types of errors in CLNTa_error() */
enum errorTypes
{
    /*  to eliminate the message in CLNTa_error() */
    ERRTYPE_UNDEFINED = 0,

    ERRTYPE_INFORMATION,
    ERRTYPE_CRITICAL,
    ERRTYPE_FATAL,
    ERRTYPE_BEARER,
    ERRTYPE_INTERNAL,
	ERRTYPE_SKTT,

    ERRTYPE_END /*  at least one compiler doesn't accept a comma after the last enum */
};


enum errorCodes
{
    /*  WTP Abort reason. See further WTP Spec */
    ERR_WTP_ABORT_START,
        ERR_WTP_UNKNOWN = 0,
        ERR_WTP_PROTOERR = 1,
        ERR_WTP_INVALIDTID = 2,
        ERR_WTP_NOTIMPLEMENTEDCL2 = 3,
        ERR_WTP_NOTIMPLEMENTEDSAR = 4,
        ERR_WTP_NOTIMPLEMENTEDUACK = 5,
        ERR_WTP_WTPVERSIONONE = 6,
        ERR_WTP_CAPTEMPEXCEEDED = 7,
        ERR_WTP_NORESPONSE = 8,
        ERR_WTP_MESSAGETOOLARGE = 9,
    ERR_WTP_ABORT_END,

    /*  WSP Status codes */
    /*  These are statuscodes from WSP requests, mainly server status messagages. See further RFC 2068 and WSP/B Appendix A */
    ERR_HTTP_START,
        HTTPBadRequest = 0x40,
        HTTPUnauthorized,
        HTTPPaymentRequired,
        HTTPForbidden,
        HTTPFileNotFound,
        HTTPMethodNotAllowed,
        HTTPNotAcceptable,
        HTTPProxyAuthenticationRequired,
        HTTPRequestTimeout,
        HTTPConflict,
        HTTPGone,
        HTTPLengthRequired,
        HTTPPreconditionFailed,
        HTTPRequestedEntityTooLarge,
        HTTPRequestURITooLarge,
        HTTPUnsupportedMediaType,

        HTTPInternalServerError = 0x60,
        HTTPNotImplemented,
        HTTPBadGateway,
        HTTPServiceUnavailable,
        HTTPGatewayTimeout,
        HTTPVerNotSupported,
    ERR_HTTP_END,

    /*  WSP Abort reason. See further WSP Spec, Appendix A */
    ERR_WSP_ABORT_START,
        ERR_WSP_PROTOERR = 0xE0,
        ERR_WSP_DISCONNECT,
        ERR_WSP_SUSPEND,
        ERR_WSP_RESUME,
        ERR_WSP_CONGESTION,
        ERR_WSP_CONNECTERR,
        ERR_WSP_MRUEXCEEDED,
        ERR_WSP_MOREXCEEDED,
        ERR_WSP_PEERREQ,
        ERR_WSP_NETERR,
        ERR_WSP_USERREQ,
    ERR_WSP_ABORT_END,

    /* general error codes */
    ERR_WAE_START = 1000,
        ERR_WAE_OUT_OF_MEMORY = 1001,
        ERR_WAE_UA_START,
            ERR_WAE_UA_VIEWID_INVALID = 1003,              /* invalid view id was used */
            ERR_WAE_UA_MAX_EXCEEDED = 1004,                /* too many active user agents */
            ERR_WAE_UA_PARSE = 1005,                       /* error in wml code */
            ERR_WAE_UA_DISPLAY_ERROR = 1006,               /* mmi engine is not able to process content */
            ERR_WAE_UA_RESPONSE_BODY_INVALID = 1007,       /* no proper WML repsonse body, possibly empty */
            ERR_WAE_UA_URL_INVALID = 1008,                 /* malformed url was encountered */
            ERR_WAE_UA_URL_TIMEOUT = 1009,                 /* urlrequest timed out */
            ERR_WAE_UA_WSP_RESPONSE_INVALID = 1010,        /* error in wsp header e.g.no valid contenttype, non recognizable HTTP-WSP/B response was encountered */
            ERR_WAE_UA_WMLDECK_ACCESS_DENIED = 1011,       /* wmldeck contained accessrestrictions not fullfilled by ua */
            ERR_WAE_UA_URL_NONSUPPORTED_SCHEME = 1012,     /* a non supported scheme was used in the wrong context*/
            ERR_WAE_UA_REDIRECT_ERROR = 1013,              /* a request has redirected more than 5 times */
            ERR_WAE_UA_SESSION_NOT_CONNECTED = 1014,
            ERR_WAE_UA_MAX_NR_OF_SESSIONS_REACHED = 1015,
        ERR_WAE_UA_END,
        ERR_WAE_WML_START = 1100,
            ERR_WAE_WML_INSTREAM_FAULT = 1101,
            ERR_WAE_WML_CONTENT_CHARSET_ERROR = 1102,          /* error in the charset coding */
            ERR_WAE_WML_CONTENT_CHARSET_NOT_SUPPORTED = 1103,  /* charset coding not supported */
            ERR_WAE_WML_UNKNOWN_TOKEN = 1104,
            ERR_WAE_WML_WML_ERROR = 1105,
            ERR_WAE_WML_CONTENT_VERSION_WARNING = 1106,        /* version nbr warning - parsing is NOT cancelled */
            ERR_WAE_WML_CONTENT_PUBLIC_ID_WARNING = 1107,      /* public id warning - parsing is NOT cancelled */
            ERR_WAE_WML_CONTENT_PUBLIC_ID_ERROR = 1108,        /* public id NOT wml- parsing is cancelled */
        ERR_WAE_WML_END,
        ERR_WAE_WMLS_START = 1200,
            ERR_WAE_WMLS_NONE = 1201,                      /* NO ERROR */
            ERR_WAE_WMLS_VERIFICATION = 1202,              /* Verification failed */
            ERR_WAE_WMLS_LIB = 1203,                       /* Fatal library function error */
            ERR_WAE_WMLS_FUNC_ARGS = 1204,                 /* Invalid function arguments */
            ERR_WAE_WMLS_EXTERNAL = 1205,                  /* External function not found */
            ERR_WAE_WMLS_LOAD = 1206,                      /* Unable to load compilation unit */
            ERR_WAE_WMLS_ACCESS = 1207,                    /* Access violation */
            ERR_WAE_WMLS_STACK_UNDERFLOW = 1208,           /* Stack underflow */
            ERR_WAE_WMLS_ABORT = 1209,                     /* Programmed abort */
            ERR_WAE_WMLS_STACK_OVRFLW = 1210,              /* Stack overflow */
            ERR_WAE_WMLS_USER_ABORT = 1211,                /* User initiated */
            ERR_WAE_WMLS_SYSTEM_ABORT = 1212,              /* System initiated */
            ERR_WAE_WMLS_NULL = 1213,                      /* some component was inaccessible */
        ERR_WAE_WMLS_END,
        ERR_WAE_REP_START = 1300,
            ERR_WAE_REP_SERVICE_INSTALL_FAILED = 1301,  /* Service installation failed */
            ERR_WAE_REP_MEM_ACCESS_FAILED = 1302,       /* Memory access failed when global binding exists */
        ERR_WAE_REP_END,
        ERR_WAE_PUSH_START = 1400,
            ERR_WAE_PUSH_ACTIVATE_FAILED = 1401,    /* Activation of postponed push failed */
            ERR_WAE_PUSH_DELETE_FAILED = 1402,      /* Removal of postponed push failed */
            ERR_WAE_PUSH_EXPIRED = 1403,            /* Push has expired */
            ERR_WAE_PUSH_STORE_FAULT = 1404,        /* Push storing failure */
        ERR_WAE_PUSH_END,
		ERR_WAE_SKT_SECURITY_START = 1500,
			ERR_WAE_SKT_SECURITY_CLIENTHELLO = 1501,    /* ClentHello message couldn't be created */
			ERR_WAE_SKT_SECURITY_SERVERHELLO = 1502,    /* An error is detected in the ServerHello message */
			ERR_WAE_SKT_SECURITY_KEYEXCHANGE = 1503,    /* KeyExchange message couldn't be created */
			ERR_WAE_SKT_SECURITY_FINISHED = 1504,       /* An error is detected in the Finished message */
			ERR_WAE_SKT_SECURITY_ENCRYPT = 1505,        /* The encryption failured */
			ERR_WAE_SKT_SECURITY_DECRYPT = 1506,		/* The decryption failured */
			ERR_WAE_SKT_SECURITY_TIMEOUT = 1507,		/* The timeout expired during the handshaking procedure */
		ERR_WAE_SKT_SECURITY_END,
    ERR_WAE_END,
    ERR_WSPCL_START = 2000,
        ERR_WSPCL_ErrorInAddressFromWAE = 2001,             /* Address received from WAE incorrect */
        ERR_WSPCL_ErrorExtractReplyPDUFailed = 2002,        /* Received ReplyPDU incorrect or no memory available for extraction */
        ERR_WSPCL_ErrorNoBuffersAvailable = 2004,           /* No free memory available */
        ERR_WSPCL_ErrorMethodNotSupported = 2005,           /* Requested method not supported */
        ERR_WSPCL_ErrorExtractPushPDUFailed = 2006,         /* Received PushPDU incorrect or no memory available for extraction */
        ERR_WSPCL_ErrorErroneousPDUTypeReceived = 2007,     /* Recived type of PDU not applicable */
        ERR_WSPCL_ErrorErroneousStackConfigReceived = 2008, /* Received stackconfig from WAE erroneous  */
    ERR_WSPCL_END,
    ERR_WSPCM_START = 3000,
        ERR_WSPCM_ErrorNoMemoryAvailableForPDUPacking = 3001,       /* No memory available for PDU construction */
        ERR_WSPCM_ErrorNoMemoryAvailableForPDUUnPacking = 3002,     /* No memory available for PDU extraction  */
        ERR_WSPCM_ErrorInDataFromWAE = 3003,                        /* Data from WAE erroneous */
        ERR_WSPCM_ErrorInReplyFromServer = 3004,                    /* Data from server erroneous */
        ERR_WSPCM_ErrorMaxSessionsAlreadyReached = 3005,            /* Number of active sessions already equal to MXSESSIONS */
        ERR_WSPCM_ErrorMOMAlreadyReached = 3006,                    /* Number of outstanding methods already equal to MOM  */
        ERR_WSPCM_WAEErrorNoPIdFoundMatchingSession = 3007,         /* The dispatcher finds no PId corresponding to the wanted session from WAE */
        ERR_WSPCM_ErrorNoPIdFoundMatchingMethod = 3008,             /* No ClientSessionProcess found matching TID from gateway */
        ERR_WSPCM_ErrorNoPIdFoundMatchingPush = 3009,               /* No ClientPushProcess found matching PushID from gateway*/
        ERR_WSPCM_ErrorStoreOMInfoFailed = 3010,                    /* Session data could not be stored */
        ERR_WSPCM_ErrorStoreHandleFailed = 3011,                    /* Handle could not be stored */
        ERR_WSPCM_ErrorMethodNotSupported = 3012,                   /* Requested method not supported */
        ERR_WSPCM_ErrorSameAQUsed = 3013,                           /* Session disconnected because a new one is started using the same adress quadruple */
        ERR_WSPCM_WAEErrorNoPIdFoundMatchingMethod = 3014,          /* No ClientSessionProcess found matching TID from WAE */
    ERR_WSPCM_END,
    ERR_WTP_START = 4000,
        ERR_WTP_ErrorNORESPONSE = 4001,
        ERR_WTP_ErrorNOFREEBUFF = 4002,
        ERR_WTP_ErrorINVALID_BEARER = 4003,
        ERR_WTP_ErrorINVALID_CLASS = 4004,
        ERR_WTP_ErrorINVALID_ACKTYPE = 4005,
    ERR_WTP_END,
    ERR_WTLS_START = 5000,
        ERR_WTLS_GENERAL_START = 5001,
        ERR_WTLS_GENERAL_END = 5099,
        ERR_WTLS_HANDSHAKE_FAILURE_START = 5100,
        ERR_WTLS_HANDSHAKE_FAILURE_END = 5199,
        ERR_WTLS_CRYPTLIB_START = 5200,
        ERR_WTLS_CRYPTLIB_END = 5299,
        ERR_WTLS_RECEIVED_ALERT_START = 5300,
        ERR_WTLS_RECEIVED_ALERT_END = 5399,
        ERR_WTLS_INTERNAL_START = 5400,
        ERR_WTLS_INTERNAL_END = 5499,
    ERR_WTLS_END,
    ERR_WDP_START = 6000,
        ERR_WDP_ErrorInDatafromWSP = 6001,      /* The data reveived from WSP was faulty */
        ERR_WDP_ErrorInDatafromWTP = 6002,      /* The data reveived from WTP was faulty */
        ERR_WDP_ErrorBearerNotSupported = 6003, /* The bearer is not supported */
        ERR_WDP_WDErrorInd = 6004,              /* Bearer WD error */
        ERR_WDP_UDPErrorInd = 6005,             /* Bearer UDP error  */
        ERR_WDP_UDPBigBuffer = 6006,            /* Bearer UDP received DG too big  */
        ERR_WDP_SMSErrorInd = 6007,             /* Bearer SMS error  */
        ERR_WDP_SMSBigBuffer = 6008,            /* Bearer SMS received DG too big  */
        ERR_WCMP_PortUnreachable = 6009,        /* WCMP port unreachable received  */
        ERR_WCMP_MessageTooBig = 6010,          /* WCMP message too big received  */
    ERR_WDP_END,
    ERR_UDCP_START = 7000,
    ERR_UDCP_END,
    ERR_MEM_START = 8000,
        ERR_MEMORY_WARNING = 8001, /* The memory has reached the level specified in the configuration variable MEMORY_WARNING in the file confvars.h */
        ERR_OUT_OF_MEMORY = 8002,  /* No more memory! */
    ERR_MEM_END,

/*------------------------------- these errors are only used internally -----------------------------*/
    ERR_INTERNAL_START = 10000,
        ERR_WAE_UA_OTA_NOT_ALLOWED,    /* ONLY USED INTERNALLY! : a non allowed OTA-request was attempted */
        ERR_WAE_UA_REQUEST_ABORTED,    /* ONLY USED INTERNALLY! : the urlrequest was aborted by the UA */
        ERR_WAE_UA_INVALID_SESSION_ID, /* ONLY USED INTERNALLY! : WSPIF has detected an invalid SessionID in a request */
        ERR_WAE_WMLS_EXIT,             /* ONLY USED INTERNALLY! : the WMLS interpreter must deallocate the entire call chain of scripts and return control to the UA */
    ERR_INTERNAL_END,

	ERR_SKTT_START = 10100,
		ERR_MMI_UNKNOWN_CONTENT,
	ERR_SKTT_END
};


#ifndef __H2SDL__

typedef enum errorTypes enumErrorType;
typedef enum errorCodes enumErrorCode;

#endif


#endif
