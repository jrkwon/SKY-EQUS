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
 * WSPproCM.h
 *
 * Procedures for WSP Connection oriented.
 *
 * Created by Anders Edenbrandt, May 1999.
 *
 * Revision history:
 *   990915, AED: Changed from using MBM buffers to PDU buffers.
 *
 */
#ifndef _WSPproCM_h
#define _WSPproCM_h

#include "wiptrgt.h"

#if defined XSCT_CMICRO
    #include "ml_typ.h"
#else
    #include "scttypes.h"
#endif

#include "CmmnRsrc.h"
#include "ctypes.h"
#include "pdubuf.h"
#include "GlblDecl.h"
#include "WAE_WSP.h"


#define MRU      MaxPDUsize
#define SMRU     MaxPDUsize
#define MOM         3
#define MOP         3
#define PROTOOPT  224

#define WSP_PROTOCOL_VERSION     0x10

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


#define CAP_CLIENT_SDU_SIZE      0x00
#define CAP_SERVER_SDU_SIZE      0x01
#define CAP_PROTOCOL_OPTIONS     0x02
#define CAP_METHOD_MOR           0x03
#define CAP_PUSH_MOR             0x04
#define CAP_EXTENDED_METHODS     0x05
#define CAP_HEADER_CODE_PAGES    0x06
#define CAP_ALIASES              0x07

#define MDF_METHOD_MOR           0x01
#define MDF_PUSH_MOR             0x02
#define MDF_CLIENT_SDU           0x04
#define MDF_SERVER_SDU           0x08
#define MDF_PROTOCOL_OPTIONS     0x10

#define OPT_CONFIRMED_PUSH       0x80
#define OPT_PUSH                 0x40
#define OPT_SESSION_RESUME       0x20
#define OPT_ACK_HEADERS          0x10

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


/*
 * Examine a list of capabilities and possibly modify some of them.
 * The (possibly modified) values found are stored in the out-parameters
 * CurrentMOM, CurrentMOP, etc. In the parameter MdfBy a bit is set
 * to show which capabilities were found. If a capability of the type
 * Protocol Options is present, the out-parameters ConfirmedPushEnabled,
 * PushEnabled, and SessionResumeEnabled are set to show which of these
 * options are enabled.
 * The parameter ReqCapLen holds the length of the buffer on input,
 * and on output holds the length of the modified buffer.
 * Returns FALSE on error, TRUE otherwise.
 */
extern SDL_Boolean
GetAndMdfReqCap (void *RequestedCapabilities,
                 SDL_Natural *ReqCapLen,
                 SDL_Natural *CurrentMOM, SDL_Natural *CurrentMOP,
                 SDL_Natural *CurrentMRU, SDL_Natural *CurrentSMRU,
                 SDL_Octet *CurrentProtoOpt, SDL_Boolean *ConfirmedPushEnabled,
                 SDL_Boolean *PushEnabled, SDL_Boolean *SessionResumeEnabled,
                 SDL_Octet *MdfBy);


/*
 * Builds and returns a buffer of capabilities.
 * The out-parameter RequstedCapabilities is set to point to
 * the newly allocated buffer, and ReqCapLen is set to the length
 * of the list of capabilities.
 * Which capabilities are included is governed by the bit maps
 * MdfByClient and MdfByServer, such that a capability is included
 * if and only if the corresponding bit is set in MdfByClient and
 * is not set in MdfByServer.
 * Returns FALSE on error, and TRUE otherwise.
 * NOTE: it is the responsibility of the caller to deallocate
 * the returned buffer, unless this routine returns FALSE.
 */
extern SDL_Boolean
ModifiedCap (SDL_Octet MdfByClient, SDL_Octet MdfByServer,
             SDL_Natural CurrentMOM, SDL_Natural CurrentMOP,
             SDL_Natural CurrentMRU, SDL_Natural CurrentSMRU,
             SDL_Octet CurrentProtoOpt,
             void **RequestedCapabilities, SDL_Natural *ReqCapLen);


#endif
