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
/*
 * wspprocm.h
 *
 * Procedures for WSP Connection oriented.
 *
 * Created by Anders Edenbrandt, May 1999.
 *
 * Revision history:
 *   990915, AED: Changed from using MBM buffers to PDU buffers.
 *   001027, HEAD: Added two functions for SAR.
 *   001211, AED: Rewritten to use the new wapcvt functions.
 *                New way of handling capabilities.
 *
 */
#ifndef _wspprocm_h
#define _wspprocm_h

#include "wiptrgt.h"

#if defined XSCT_CMICRO
#include "ml_typw.h"
#else
#include "scttypes.h"
#endif

#include "tapicmmn.h"
#include "ctypes.h"
#include "pdubuf.h"
#include "glbldecl.h"
#include "wae_wsp.h"


#define WSP_PROTOCOL_VERSION     0x10

/* WSP PDU types */
#define CONNECT_PDU_TYPE         0x01
#define CONNECTREPLY_PDU_TYPE    0x02
#define REDIRECT_PDU_TYPE        0x03
#define REPLY_PDU_TYPE           0x04
#define DISCONNECT_PDU_TYPE      0x05
#define PUSH_PDU_TYPE            0x06
#define CONFIRMED_PUSH_PDU_TYPE  0x07
#define SUSPEND_PDU_TYPE         0x08
#define RESUME_PDU_TYPE          0x09
#define GET_PDU_TYPE             0x40
#define POST_PDU_TYPE            0x60
#define PUT_PDU_TYPE             0x61

/* Capability identifiers */
#define CAP_CLIENT_SDU_SIZE      0x80
#define CAP_SERVER_SDU_SIZE      0x81
#define CAP_PROTOCOL_OPTIONS     0x82
#define CAP_METHOD_MOR           0x83
#define CAP_PUSH_MOR             0x84
#define CAP_EXTENDED_METHODS     0x85
#define CAP_HEADER_CODE_PAGES    0x86
#define CAP_ALIASES              0x87

/* Protocol option flags */
#define OPT_CONFIRMED_PUSH       0x80
#define OPT_PUSH                 0x40
#define OPT_SESSION_RESUME       0x20
#define OPT_ACK_HEADERS          0x10

/* Redirection flags */
#define REDIRECT_FLAGS_PERMANENT 0x80
#define REDIRECT_FLAGS_REUSE_SEC 0x40


typedef void *VOIDSTAR;

/*
 * Extract the fields from a reply PDU.
 * Sets a status value, two VOIDSTAR out-parameters and two integer
 * out-parameters (containing headers and data from the PDU as well as
 * their lengths), and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractReplyPDU (pdubuf *PDU,
                 SDL_Octet *Status, VOIDSTAR *Headers, SDL_Natural *HeadLen,
                 VOIDSTAR *Data, SDL_Natural *DataLen);

/*
 * Extract the fields from a Data PDU.
 * Sets a VOIDSTAR out-parameter and one integer out-parameter
 * (containing data from the PDU as well as the length) and then
 * releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffer
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractDataPDU (pdubuf *PDU, VOIDSTAR *Data, SDL_Natural *DataLen);

/*
 * Extract the fields from a Push PDU.
 * Sets two VOIDSTAR out-parameters and two integer
 * out-parameters (containing headers and data from the PDU as well as
 * their lengths), and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractPushPDU (pdubuf *PDU,
                VOIDSTAR *Headers, SDL_Natural *HeadLen,
                VOIDSTAR *Data, SDL_Natural *DataLen);

/*
 * Create a PDU for a Connect request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateConnectPDU (SConnectReqType *SConnectReqData);

/*
 * Extract the fields from a Connect Reply PDU.
 * Sets a session ID value and fields in the SConnectCnfType structure
 * representing capabilities and headers as well as their lengths,
 * and then releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffers
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractConnectReplyPDU (pdubuf *PDU,
                        SDL_Natural *ServerSessionId,
                        SConnectCnfType *SConnectCnfData);

/*
 * Create a Disconnect PDU.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateDisconnectPDU (SDL_Integer SessionID);

/*
 * Extract the fields from a Disconnect PDU.
 * Sets the session ID value, and then releases the PDU buffer.
 * In case of error, the PDU buffer is NOT released.
 * Returns FALSE on error, TRUE otherwise.
 */
extern SDL_Boolean
ExtractDisconnectPDU (pdubuf *PDU,
                      SDL_Natural *ServerSessionId);

/*
 * Create a PDU for a Get or similar request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateMethodPDU (SMethodInvokeReqType *SMethodInvokeReqData);

/*
 * Create a PDU for a Post request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreatePostPDU (SMethodInvokeReqType *SMethodInvokeReqData);

/*
 * Create a PDU for a Suspend request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed.
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateSuspendPDU (SDL_Natural sessionId);

/*
 * Create a PDU for a Resume request.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateResumePDU (SDL_Natural sessionId, SResumeReqType *SResumeReqData);

/*
 * Create a PDU for more Data.
 * Returns a pointer to a PDU buffer where the relevant data has
 * been placed. Also deallocates the VOIDSTAR buffers that are
 * given as in-parameters (unless the routine returns NULL).
 * Returns NULL in case of error.
 * NOTE: it is the caller's responsibility to deallocate the returned buffer.
 */
extern pdubuf *
CreateMoreDataPDU (SMethodInvokeReqType *SMethodInvokeReqData);

/*
 * Extract the fields from a redirect PDU.
 * Sets a reason code, a redirect security indication,
 * and places the redirect addresses in a newly allocated buffer,
 * and finally releases the PDU buffer.
 * Returns FALSE in case of error, in which case the PDU buffer
 * is NOT released.
 * NOTE: it is the caller's responsibility to deallocate the VOIDSTAR buffer
 * (unless the routine returns FALSE).
 */
extern SDL_Boolean
ExtractRedirectPDU (pdubuf *PDU, SDL_Octet *ReasonCode,
                    SDL_Boolean *RedirectSecurity,
                    VOIDSTAR *RedirectedAddresses,
                    SDL_Natural *AddressLen);

#endif
