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
 * WTLSerr.c
 *
 * Created by Anders Edenbrandt, Wed Jun 23 08:27:29 1999.
 *
 *
 * Revision history:
 *
 */
#include "wtlsdef.h"
#include "aapiclnt.h"

static UINT16 globalErrType = 0;
static UINT16 globalErrCode = 0;
static UINT16 globalSendAlert = 0;
static UINT16 globalAlertLevel = 0;
static UINT16 globalAlertDesc = 0;

#ifdef LOG_WTLS
static char *
Errtype2String (INT16 errtype)
{
  switch (errtype) {
  case ERR_GENERAL: return "General";
  case ERR_HANDSHAKE_FAILURE: return "Handshake Failure";
  case ERR_CRYPTLIB: return "Cryptlib";
  case ERR_RECEIVED_ALERT: return "Received Alert";
  case ERR_INTERNAL: return "Internal";
  default: return "Unknown";
  }
}

static char *
Errcode2String (INT16 errcode)
{
  switch (errcode) {
  case ERR_INCORRECT_ALERT: return "Incorrect Alert";
  case ERR_DECODING: return "Decoding";
  case ERR_BAD_PADDING: return "Bad Padding";
  case ERR_BAD_MAC: return "Bad Mac";
  case ERR_TERMINATE_REQUEST: return "Terminate Request";
  case ERR_CONNECTION_TIMED_OUT: return "Connection Timed Out";
  case ERR_UNEXPECTED_MESSAGE: return "Unexpected Message";
  case ERR_MESSAGE_PARAMETERS: return "Message Parameters";
  case ERR_CIPHER_SUITE: return "Cipher Suite";
  case ERR_COMPRESSION_ALG: return "Compression Alg";
  case ERR_SEQNUM_MODE: return "Seqnum Mode";
  case ERR_KEY_REFRESH: return "Key Refresh";
  case ERR_SERVER_FINISHED: return "Server Finished";
  case ERR_KEY_EXCHANGE_IDX: return "Key Exchange Idx";
  case ERR_KEY_EXCHANGE_DECODE: return "Key Exchange Decode";
  case ERR_UNEXPECTED_KEY_EXCHANGE: return "Unexpected Key Exchange";
  case ERR_MISSING_KEY_EXCHANGE: return "Missing Key Exchange";
  case ERR_MISSING_SERVER_CERTIFICATE: return "Missing Server Certificate";
  case ERR_BAD_CCS: return "Bad Ccs";
  case ERR_MAX_RETRANSMISSIONS: return "Max Retransmissions";
  case ERR_UNSUPPORTED_CERTIFICATE: return "Unsupported Certificate";
  case ERR_UNKNOWN_CA: return "Unknown CA";
  case ERR_BAD_CERTIFICATE: return "Bad Certificate";
  case ERR_CERTIFICATE_EXPIRED: return "Certificate Expired";
  case ERR_MISSING_MESSAGE: return "Missing Message";
  case ERR_ENCRYPT: return "Encrypt";
  case ERR_ENCRYPT_INIT: return "Encrypt Init";
  case ERR_ENCRYPT_UPDATE: return "Encrypt Update";
  case ERR_ENCRYPT_FINAL: return "Encrypt Final";
  case ERR_DECRYPT: return "Decrypt";
  case ERR_HASH: return "Hash";
  case ERR_HASH_INIT: return "Hash Init";
  case ERR_HASH_UPDATE: return "Hash Update";
  case ERR_HASH_FINAL: return "Hash Final";
  case ERR_KEY_EXCHANGE: return "Key Exchange";
    /*
  case ERR_NULL_KEY_AGREEMENT: return "Null Key Agreement";
  case ERR_SECRET_KEY_AGREEMENT: return "Secret Key Agreement";
  case ERR_KEY_AGREEMENT_DH: return "Key AGreement DH";
  case ERR_KEY_TRANSPORT_RSA: return "Key Transport RSA";
  case ERR_KEY_AGREEMENT_ECDH: return "Key Agreement ECDH";
  case ERR_DERIVE_MASTER_SECRET: return "Derive Master Secret";
    */
  case ERR_PRF: return "PRF";
  case ERR_HASH_KEY_TOO_LONG: return "Hash Key Too Long";
  case ERR_GENERATE_RANDOM: return "Generate Random";
  case ERR_GET_CIPHER_METHODS: return "Get Cipher Methods";
  case ERR_CRYPTLIB_INIT: return "Cryptlib Init";
  case ERR_CRYPTLIB_TERMINATE: return "Cryptlib Terminate";
  case ERR_GET_METHODS: return "Get Methods";
  case ERR_GET_CLIENT_CERTIFICATE: return "Get Client Certificate";
  case ERR_COMPUTE_SIGNATURE: return "Compute Signature";

  case ERR_ENCODING: return "Encoding";
  case ERR_INSUFFICIENT_MEMORY: return "Insufficient Memory";
  case ERR_MISSING_CONNECTION_NODE: return "Missing Connection Node";
  case ERR_MISSING_RECORD_BUFFER: return "Missing Record Buffer";
  default: return "";
  }
}
#endif

void
wtls_err_set (SDL_Integer errtype, SDL_Integer errcode,
              SDL_Integer sendAlert,
              SDL_Integer alertLevel, SDL_Integer alertDesc)
{
  globalErrType = (UINT16)errtype;
  globalErrCode = (UINT16)errcode;
  globalSendAlert = (UINT16)sendAlert;
  globalAlertLevel = (UINT16)alertLevel;
  globalAlertDesc = (UINT16)alertDesc;
#ifdef LOG_WTLS
  wtls_log_msg (0, "ERROR: error type '%s', error code '%s'\n",
                Errtype2String ((INT16)errtype),
                Errcode2String ((INT16)errcode));
#endif
  CLNTa_error (0, (INT16)errcode,
               (UINT8)((errcode == ERR_INSUFFICIENT_MEMORY) ?
                       ERRTYPE_FATAL : ERRTYPE_INFORMATION));
}


void
wtls_err_get (SDL_Integer *errtype, SDL_Integer *errcode,
              SDL_Integer *sendAlert,
              SDL_Integer *alertLevel, SDL_Integer *alertDesc)
{
  *errtype = globalErrType;
  *errcode = globalErrCode;
  *sendAlert = globalSendAlert;
  *alertLevel = globalAlertLevel;
  *alertDesc = globalAlertDesc;
}

void
wtls_err_check (SDL_Integer *errtype, SDL_Integer *errcode,
                SDL_Integer *sendAlert,
                SDL_Integer *alertLevel, SDL_Integer *alertDesc)
{
  *errtype = globalErrType;
  *errcode = globalErrCode;
  *sendAlert = globalSendAlert;
  *alertLevel = globalAlertLevel;
  *alertDesc = globalAlertDesc;

  if (globalErrCode == ERR_RECEIVED_ALERT) {
    if (globalAlertDesc == ALERT_DESC_CONNECTION_CLOSE_NOTIFY) {
      *sendAlert = 1;
    }
    else if (globalAlertDesc == ALERT_DESC_SESSION_CLOSE_NOTIFY) {
      *sendAlert = 1;
      *alertLevel = ALERT_LEVEL_FATAL;
    }
  }
}


void
wtls_err_clear (void)
{
  globalErrType = 0;
  globalErrCode = 0;
  globalSendAlert = 0;
  globalAlertLevel = 0;
  globalAlertDesc = 0;
}
