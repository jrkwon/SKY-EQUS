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
 * Logging.c
 *
 * Created by Anders Edenbrandt, Thu Oct 21 08:55:16 1999.
 *
 *
 * Revision history:
 *   981021, AED:  Created
 *   000119, AED:  New function, LogClose; new argument to LogInit.
 *   010125, AED:  Do not try to interpret encrypted messages.
 *
 */
#ifdef LOG_WTLS

#include "logging.h"
#ifdef LOG_WTLS_CERT
#include "cert.h"
#endif

#include <stdio.h>
#include <stdarg.h>

static FILE *logfile = NULL;

void
wtls_log_msg (int lvl, char *fmt, ...)
{
  va_list args;
  int i;
  
  for (i = 0; i < lvl; i++)
    fprintf (logfile, "    ");

  va_start (args, fmt);
  vfprintf (logfile, fmt, args);
  fflush (logfile);
  va_end (args);
}

void
wtls_log_init (char *filename)
{
  if (filename == NULL) {
    filename = "WTLSlog.txt";
  }
  logfile = fopen (filename, "w");
}

void
wtls_log_close (void)
{
  fclose (logfile);
}

static char *
wtls_log_key_exchange_to_string (BYTE key_exchange_suite)
{
  switch (key_exchange_suite) {
  case KEY_EXCH_NULL: return "NULL";
  case KEY_EXCH_SHARED_SECRET: return "SHARED_SECRET";
  case KEY_EXCH_DH_ANON: return "DH_ANON";
  case KEY_EXCH_DH_ANON_512: return "DH_ANON_512";
  case KEY_EXCH_DH_ANON_768: return "DH_ANON_768";
  case KEY_EXCH_RSA_ANON: return "RSA_ANON";
  case KEY_EXCH_RSA_ANON_512: return "RSA_ANON_512";
  case KEY_EXCH_RSA_ANON_768: return "RSA_ANON_768";
  case KEY_EXCH_RSA: return "RSA";
  case KEY_EXCH_RSA_512: return "RSA_512";
  case KEY_EXCH_RSA_768: return "RSA_768";
  case KEY_EXCH_ECDH_ANON: return "ECDH_ANON";
  case KEY_EXCH_ECDH_ANON_113: return "ECDH_ANON_113";
  case KEY_EXCH_ECDH_ANON_131: return "ECDH_ANON_131";
  case KEY_EXCH_ECDH_ECDSA: return "ECDH_ECDSA";
  default : return "Error";
  }
}

static char *
wtls_log_bulk_cipher_algorithm_to_string (BulkCipherAlgorithm alg)
{
  switch (alg) {
  case CIPHER_NULL: return "NULL";
  case CIPHER_RC5_CBC_40: return "RC5_CBC_40";
  case CIPHER_RC5_CBC_56: return "RC5_CBC_56";
  case CIPHER_RC5_CBC: return "RC5_CBC";
  case CIPHER_DES_CBC_40: return "DES_CBC_40";
  case CIPHER_DES_CBC: return "DES_CBC";
  case CIPHER_3DES_CBC_EDE: return "3DES_CBC_EDE";
  case CIPHER_IDEA_CBC_40: return "IDEA_CBC_40";
  case CIPHER_IDEA_CBC_56: return "IDEA_CBC_56";
  case CIPHER_IDEA_CBC: return "IDEA_CBC";
  default : return "Error";
  }
}

static char *
wtls_log_MAC_algorithm_to_string (wtls_MAC_algorithm alg)
{
  switch (alg) {
  case MAC_SHA_0: return "SHA_0";
  case MAC_SHA_40: return "SHA_40";
  case MAC_SHA_80: return "SHA_80";
  case MAC_SHA: return "SHA";
  case MAC_SHA_XOR_40: return "SHA_XOR_40";
  case MAC_MD5_40: return "MD5_40";
  case MAC_MD5_80: return "MD5_80";
  case MAC_MD5: return "MD5";
  default : return "Error";
  }
}

static char *
wtls_log_content_type_to_string (BYTE rec_type)
{
  switch (rec_type & RECTYPE_CONTENT_TYPE) {
  case CONTENT_TYPE_EMPTY: return "";
  case CONTENT_TYPE_CCS: return "Change Cipher Spec";
  case CONTENT_TYPE_ALERT: return "Alert";
  case CONTENT_TYPE_HANDSHAKE: return "Handshake";
  case CONTENT_TYPE_DATA: return "Data";
  default: return "Error";
  }
}

static char *
wtls_log_alert_level_to_string (BYTE alert_level)
{
  switch (alert_level) {
  case ALERT_LEVEL_WARNING: return "Warning";
  case ALERT_LEVEL_CRITICAL: return "Critical";
  case ALERT_LEVEL_FATAL: return "Fatal";
  default: return "Unknown alert level";
  }
}

static char *
wtls_log_alert_desc_to_string (BYTE alert_desc)
{
  switch (alert_desc) {
  case ALERT_DESC_CONNECTION_CLOSE_NOTIFY: return "Connection Close Notify";
  case ALERT_DESC_SESSION_CLOSE_NOTIFY: return "Session Close Notify";
  case ALERT_DESC_NO_CONNECTION: return "No Connection";
  case ALERT_DESC_UNEXPECTED_MESSAGE: return "Unexpected Message";
  case ALERT_DESC_TIME_REQUIRED: return "Time Required";
  case ALERT_DESC_BAD_RECORD_MAC: return "Bad Record Mac";
  case ALERT_DESC_DECRYPTION_FAILED: return "Decryption Failed";
  case ALERT_DESC_RECORD_OVERFLOW: return "Record Overflow";
  case ALERT_DESC_DECOMPRESSION_FAILURE: return "Decompression Failure";
  case ALERT_DESC_HANDSHAKE_FAILURE: return "Handshake Failure";
  case ALERT_DESC_BAD_CERTIFICATE: return "Bad Certificate";
  case ALERT_DESC_UNSUPPORTED_CERTIFICATE: return "Unsupported Certificate";
  case ALERT_DESC_CERTIFICATE_REVOKED: return "Certificate Revoked";
  case ALERT_DESC_CERTIFICATE_EXPIRED: return "Certificate Expired";
  case ALERT_DESC_CERTIFICATE_UNKNOWN: return "Certificate Unknown";
  case ALERT_DESC_ILLEGAL_PARAMETER: return "Illegal Parameter";
  case ALERT_DESC_UNKNOWN_CA: return "Unknown CA";
  case ALERT_DESC_ACCESS_DENIED: return "Access Denied";
  case ALERT_DESC_DECODE_ERROR: return "Decode Error";
  case ALERT_DESC_DECRYPT_ERROR: return "Decrypt Error";
  case ALERT_DESC_UNKNOWN_KEY_ID: return "Unknown Key ID";
  case ALERT_DESC_DISABLED_KEY_ID: return "Disabled Key ID";
  case ALERT_DESC_KEY_EXCHANGE_DISABLED: return "Key Exchange Disabled";
  case ALERT_DESC_SESSION_NOT_READY: return "Session Not Ready";
  case ALERT_DESC_UNKNOWN_PARAMETER_INDEX: return "Unknown Parameter Index";
  case ALERT_DESC_DUPLICATE_FINISHED_RECEIVED:
    return "Duplicate Finished Received";
  case ALERT_DESC_EXPORT_RESTRICTION: return "Export Restriction";
  case ALERT_DESC_PROTOCOL_VERSION: return "Protocol Version";
  case ALERT_DESC_INSUFFICIENT_SECURITY: return "Insufficient Security";
  case ALERT_DESC_INTERNAL_ERROR: return "Internal Error";
  case ALERT_DESC_USER_CANCELED: return "User Canceled";
  case ALERT_DESC_NO_RENEGOTIATION: return "No Renegotiation";
  default: return "Unknown alert description";
  }
}

static void
wtls_log_key_exchange_id (int lvl, wtls_key_exchange_id *kex)
{
  wtls_log_msg (lvl, "Key exchange method: %s\n",
                wtls_log_key_exchange_to_string (kex->key_exchange_suite));

  wtls_log_msg (lvl, "Parameter specifier: %d\n",
                kex->parameter_specifier.parameterIndex);

  wtls_log_msg (lvl, "Identifier:\n");
#ifdef LOG_WTLS_CERT
  wtls_cert_log_identifier (lvl, &(kex->identifier));
#else
  wtls_log_msg (lvl + 1, "<NOT PARSED>\n");
#endif
}

void
wtls_log_client_hello (wtls_client_hello *ch)
{
  INT16                i;
  wap_cvt_t            cvt_obj, cvt_obj2;
  wtls_key_exchange_id kex;

  wtls_log_msg (1, "CLIENT HELLO[\n");
  wtls_log_msg (2, "Protocol version: %d\n", ch->client_version);
  wtls_log_msg (2, "Random:");
  for (i = 0; i < 16; i++)
    fprintf (logfile, " %02x", ch->random[i]);
  wtls_log_msg (2, "\n");
  wtls_log_msg (2, "Session Id:");
  for (i = 0; i < ch->session_id.length; i++)
    fprintf (logfile, " %02x", ch->session_id.id[i]);
  wtls_log_msg (2, "\n");

  wtls_log_msg (2, "Client Key Ids:\n");
  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE, ch->client_key_ids,
                ch->client_key_id_len);
  while (cvt_obj.pos < cvt_obj.length) {
    if (!wtls_cvt_key_exchange_id (&cvt_obj, &kex))
      break;
    wtls_log_msg (2, "[\n");
    wtls_log_key_exchange_id (3, &kex);
    wtls_log_msg (2, "]\n");
    wap_cvt_init (&cvt_obj2, WAP_CVT_FREE, NULL, 0);
    wtls_cvt_key_exchange_id (&cvt_obj2, &kex);
  }
  wtls_log_msg (2, "\n");

  wtls_log_msg (2, "Trusted Key Ids:\n");
  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE, ch->trusted_key_ids,
                ch->trusted_key_id_len);
  while (cvt_obj.pos < cvt_obj.length) {
    if (!wtls_cvt_key_exchange_id (&cvt_obj, &kex))
      break;
    wtls_log_msg (2, "[\n");
    wtls_log_key_exchange_id (3, &kex);
    wtls_log_msg (2, "]\n");
    wap_cvt_init (&cvt_obj2, WAP_CVT_FREE, NULL, 0);
    wtls_cvt_key_exchange_id (&cvt_obj2, &kex);
  }
  wtls_log_msg (2, "\n");

  wtls_log_msg (2, "Cipher Suites:\n");
  for (i = 0; i < ch->cipher_suites_len; i += 2)
    wtls_log_msg (3, " <%s, %s>\n",
                  wtls_log_bulk_cipher_algorithm_to_string (ch->cipher_suites[i]),
                  wtls_log_MAC_algorithm_to_string (ch->cipher_suites[i + 1]));
  wtls_log_msg (2, "\n");

  wtls_log_msg (2, "Compression Methods:");
  for (i = 0; i < ch->num_compression_methods; i++)
    wtls_log_msg (0, " %02x", ch->compression_methods[i]);
  wtls_log_msg (2, "\n");
  wtls_log_msg (2, "Sequence Number Mode: %d\n", ch->seqnum_mode);
  wtls_log_msg (2, "Key Refresh Rate: %d\n", ch->key_refresh_rate);
  wtls_log_msg (1, "]\n");
}

void
wtls_log_server_hello (wtls_server_hello *sh)
{
  INT16 i;

  wtls_log_msg (1, "SERVER HELLO[\n");
  wtls_log_msg (2, "Server Version: %d\n", sh->server_version);
  wtls_log_msg (2, "Random:");
  for (i = 0; i < 16; i++)
    wtls_log_msg (0, " %02x", sh->random[i]);
  wtls_log_msg (0, "\n");
  wtls_log_msg (2, "Session Id:");
  for (i = 0; i < sh->session_id.length; i++)
    wtls_log_msg (0, " %02x", sh->session_id.id[i]);
  wtls_log_msg (0, "\n");
  wtls_log_msg (2, "Client Key Id: %d\n", sh->client_key_id);
  wtls_log_msg (2, "Cipher Suite: <%s, %s>\n",
          wtls_log_bulk_cipher_algorithm_to_string (sh->cipher_suite.bulk_cipher_alg),
          wtls_log_MAC_algorithm_to_string (sh->cipher_suite.mac_alg));
  wtls_log_msg (2, "Compression Method: %d\n", sh->compression_method);
  wtls_log_msg (2, "Sequence number mode: %d\n", sh->seqnum_mode);
  wtls_log_msg (2, "Key Refresh Rate: %d\n", sh->key_refresh_rate);
  wtls_log_msg (1, "]\n");
}

void
wtls_log_server_key_exchange (wtls_server_key_exchange *ske)
{
  wtls_log_msg (1, "SERVER KEY EXCHANGE[\n");
  wtls_log_msg (2, "Parameter index: %d\n",
                ske->parameter_specifier.parameterIndex);
  if (ske->parameter_specifier.parameterIndex == 255) {
    wtls_log_msg (3, "");
    wtls_log_bytevector (ske->parameter_specifier.params,
                         ske->parameter_specifier.paramLen);
  }
  wtls_log_msg (2, "Public key:\n");
#if 0
  wtls_log_bytevector (ske->public_key._u.dh_key.y,
                       ske->public_key._u.dh_key.len);
#endif
  wtls_log_msg (1, "]\n");
}


#ifdef LOG_WTLS_CERT
void
wtls_log_certificate (wtls_certificate_t *cert)
{
  wtls_cert_log_certificate (cert);
}
#endif

void
wtls_log_handshake_msg (wtls_record_t *rec)
{
  wap_cvt_t cvt_obj;

  wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, rec->fragment, rec->length);

  switch (rec->fragment[0]) {
  case HANDSHK_HELLO_REQUEST:
    wtls_log_msg (1, "HELLO REQUEST []\n");
    break;

  case HANDSHK_CLIENT_HELLO:
    {
      wtls_client_hello ch;

      wtls_cvt_client_hello (&cvt_obj, &ch);
      wtls_log_client_hello (&ch);
    }
    break;

  case HANDSHK_SERVER_HELLO:
    {
      wtls_server_hello sh;

      wtls_cvt_server_hello (&cvt_obj, &sh);
      wtls_log_server_hello (&sh);
    }
    break;

  case HANDSHK_CERTIFICATE:
#ifdef LOG_WTLS_CERT
    {
      wtls_certificate_t      cert;
      wtls_client_certificate ccert;

      wtls_log_msg (1, "CERTIFICATE [\n");
      wtls_cvt_client_certificate (&cvt_obj, &ccert);
      wap_cvt_init (&cvt_obj, WAP_CVT_DECODE_STATIC, ccert.buf, ccert.buflen);
      if (ccert.buflen > 0) {
        wtls_cvt_certificate (&cvt_obj, &cert);
        wtls_log_certificate (&cert);
      }
      wtls_log_msg (1, "]\n");
    }
#else
    wtls_log_msg (1, "CERTIFICATE [...]\n");
#endif
    break;

  case HANDSHK_SERVER_KEY_EXCHANGE:
    {
      wtls_server_key_exchange ske;

      /* TODO: where do we get the key exchange algorithm from? */
      wtls_cvt_server_key_exchange (&cvt_obj, 0, &ske);
      wtls_log_server_key_exchange (&ske);
    }
    break;

  case HANDSHK_CERTIFICATE_REQUEST:
    wtls_log_msg (1, "CERTIFICATE REQUEST []\n");
    break;

  case HANDSHK_SERVER_HELLO_DONE:
    wtls_log_msg (1, "SERVER HELLO DONE []\n");
    break;

  case HANDSHK_CERTIFICATE_VERIFY:
    wtls_log_msg (1, "CERTIFICATE VERIFY []\n");
    break;

  case HANDSHK_CLIENT_KEY_EXCHANGE:
    wtls_log_msg (1, "CLIENT KEY EXCHANGE []\n");
    break;

  case HANDSHK_FINISHED:
    {
      wtls_finished f;
      int           i;

      wtls_cvt_finished (&cvt_obj, &f);
      wtls_log_msg (1, "FINISHED[\n");
      wtls_log_msg (2, "<");
      for (i = 0; i < 12; i++) {
        wtls_log_msg (0, "%02x ", f.verify_data[i]);
      }
      wtls_log_msg (0, ">\n");
      wtls_log_msg (1, "]\n");
    }
    break;

  default:
    wtls_log_msg (1, "Handshake message type: %d\n", rec->fragment[0]);
    break;
  }
}

void
wtls_log_data (wtls_record_t *rec)
{
  INT16 i;
  INT16 len = rec->length;

  wtls_log_msg (1, "\"");
  for (i = 0; i < len; i++)
    wtls_log_msg (0, "%c", rec->fragment[i]);
  wtls_log_msg (0, "\"\n");
}

void
wtls_log_alert (wtls_record_t *rec)
{
  wtls_log_msg (1, "%s, %s, (%02x%02x%02x%02x)\n",
                wtls_log_alert_level_to_string (rec->fragment[0]),
                wtls_log_alert_desc_to_string (rec->fragment[1]),
                rec->fragment[2], rec->fragment[3],
                rec->fragment[4], rec->fragment[5]);
}

void
wtls_log_record (wtls_record_t *rec, BYTE direction)
{
  wtls_log_msg (0, "RECORD[ %s\n", (direction == LOG_OUTBOUND ? "=>" : "<="));
  wtls_log_msg (1, "%s", wtls_log_content_type_to_string (rec->rec_type));
  if (rec->rec_type & RECTYPE_USE_CS)
    wtls_log_msg (0, " CS");
  if (rec->rec_type & RECTYPE_SEQNUM)
    wtls_log_msg (0, " S(%d)", rec->seqnum);
  if (rec->rec_type & RECTYPE_LENGTH_FIELD)
    wtls_log_msg (0, " L(%d)", rec->length);
  wtls_log_msg (0, "\n");

  switch (rec->rec_type & RECTYPE_CONTENT_TYPE) {
  case CONTENT_TYPE_DATA:
    if (rec->rec_type & RECTYPE_USE_CS)
      wtls_log_msg (1, "<ENCRYPTED DATA...>\n");
    else
      wtls_log_data (rec);
    break;

  case CONTENT_TYPE_ALERT:
    if (rec->rec_type & RECTYPE_USE_CS)
      wtls_log_msg (1, "<ENCRYPTED ALERT...>\n");
    else
      wtls_log_alert (rec);
    break;

  case CONTENT_TYPE_HANDSHAKE:
    if (rec->rec_type & RECTYPE_USE_CS)
      wtls_log_msg (1, "<ENCRYPTED HANDSHAKE MESSAGE...>\n");
    else
      wtls_log_handshake_msg (rec);
    break;
  }
  wtls_log_msg (0, "]\n");
}

void
wtls_log_record_brief (wtls_record_t *rec)
{
  wtls_log_msg (1, "RECORD[ ");
  wtls_log_msg (0, "%s", wtls_log_content_type_to_string (rec->rec_type));
  if (rec->rec_type & RECTYPE_USE_CS)
    wtls_log_msg (0, " CS");
  if (rec->rec_type & RECTYPE_SEQNUM)
    wtls_log_msg (0, " S(%d)", rec->seqnum);
  if (rec->rec_type & RECTYPE_LENGTH_FIELD)
    wtls_log_msg (0, " L(%d)", rec->length);
  wtls_log_msg (0, " ]\n");
}

void
wtls_log_bytevector (BYTE *buf, UINT16 buflen)
{
  INT16 i;

  wtls_log_msg (0, "<");
  for (i = 0; i < buflen; i++) {
    if ((i > 0) && ((i & 0xf) == 0))
      wtls_log_msg (0, "\n");
    if (i > 0)
      wtls_log_msg (0, " ");
    wtls_log_msg (0, "%02x", buf[i]);
  }
  wtls_log_msg (0, ">\n");
}

#endif
