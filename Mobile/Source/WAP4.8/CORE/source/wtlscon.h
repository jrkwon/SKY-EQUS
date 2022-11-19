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
 * wtlscon.h
 *
 * Created by Anders Edenbrandt, Thu Oct 12 15:39:24 2000.
 *
 * Revision history:
 *
 */
#ifndef _wtlscon_h
#define _wtlscon_h

#include "confvars.h"
#include "errcodes.h"

/************************************************************
 * CONFIGURABLE PARAMETERS.
 * Should be moved to confvars.h?
 ************************************************************/

/*
 * How often should the encryption keys be refreshed, i.e.,
 * calculated anew from the master secret. A value of n means
 * that it is refreshed every 2^n messages.
 */
#define WTLS_KEY_REFRESH_RATE_UDP    8
#define WTLS_KEY_REFRESH_RATE_USSD   7
#define WTLS_KEY_REFRESH_RATE_SMS    6

/*
 * Max number of retransmissions during handshake.
 * The default is to use the same value as WTP.
 */
#define WTLS_MAX_RETRANSMISSIONS_UDP  no_of_retransmissions_UDP_WTP
#define WTLS_MAX_RETRANSMISSIONS_USSD no_of_retransmissions_USSD_WTP
#define WTLS_MAX_RETRANSMISSIONS_SMS  no_of_retransmissions_SMS_WTP

/*
 * The time between to retransmissions.
 * The value should be given in 1/10ths of a second.
 * The default is to use the same value as WTP.
 */
#define WTLS_RETRANSMISSION_INTERVAL_UDP  retransmission_interval_UDP_WTP
#define WTLS_RETRANSMISSION_INTERVAL_USSD retransmission_interval_USSD_WTP
#define WTLS_RETRANSMISSION_INTERVAL_SMS  retransmission_interval_SMS_WTP

/*
 * The maximum idle time for a Push connection.
 * If this time interval passes without having received any
 * data on a secure connectionless Push connection,
 * the WTLS connection will be terminated.
 * The value should be given in 1/10ths of a second.
 */
#define WTLS_PUSH_MAX_IDLE_TIME            600


/*
 * Which MAC lengths should be used, given in order of
 * preference, with the highest preference first.
 */
#define WTLS_MAC_SHA_UDP_1   MAC_SHA
#define WTLS_MAC_SHA_UDP_2   MAC_SHA_80
#define WTLS_MAC_SHA_UDP_3   MAC_SHA_40

#define WTLS_MAC_SHA_USSD_1  MAC_SHA_80
#define WTLS_MAC_SHA_USSD_2  MAC_SHA
#define WTLS_MAC_SHA_USSD_3  MAC_SHA_40

#define WTLS_MAC_SHA_SMS_1   MAC_SHA_40
#define WTLS_MAC_SHA_SMS_2   MAC_SHA_80
#define WTLS_MAC_SHA_SMS_3   MAC_SHA

#define WTLS_MAC_MD5_UDP_1   MAC_MD5
#define WTLS_MAC_MD5_UDP_2   MAC_MD5_80
#define WTLS_MAC_MD5_UDP_3   MAC_MD5_40

#define WTLS_MAC_MD5_USSD_1  MAC_MD5_80
#define WTLS_MAC_MD5_USSD_2  MAC_MD5
#define WTLS_MAC_MD5_USSD_3  MAC_MD5_40

#define WTLS_MAC_MD5_SMS_1   MAC_MD5_40
#define WTLS_MAC_MD5_SMS_2   MAC_MD5_80
#define WTLS_MAC_MD5_SMS_3   MAC_MD5

/*
 * How many Alerts with alert level "warning" do we accept,
 * before terminating the connection.
 */
#define WTLS_MAX_NUM_WARNINGS   5

/************************************************************
 * END OF CONFIGURABLE PARAMETERS.
 ************************************************************/


/************************************************************
 * ERROR CODES
 ************************************************************/

/*
 * Type of error
 */
#define ERR_GENERAL                     ERR_WTLS_GENERAL_START
#define ERR_HANDSHAKE_FAILURE           ERR_WTLS_HANDSHAKE_FAILURE_START
#define ERR_CRYPTLIB                    ERR_WTLS_CRYPTLIB_START
#define ERR_RECEIVED_ALERT              ERR_WTLS_RECEIVED_ALERT_START
#define ERR_INTERNAL                    ERR_WTLS_INTERNAL_START

/*
 * Error code, i.e., description of error
 */
/* GENERAL */
#define ERR_INCORRECT_ALERT             ERR_GENERAL + 1
#define ERR_DECODING                    ERR_GENERAL + 2
#define ERR_BAD_PADDING                 ERR_GENERAL + 3
#define ERR_BAD_MAC                     ERR_GENERAL + 4
#define ERR_TERMINATE_REQUEST           ERR_GENERAL + 5
#define ERR_CONNECTION_TIMED_OUT        ERR_GENERAL + 6

/* HANDSHAKE_FAILURE */
#define ERR_UNEXPECTED_MESSAGE          ERR_HANDSHAKE_FAILURE + 1
#define ERR_MESSAGE_PARAMETERS          ERR_HANDSHAKE_FAILURE + 2
#define ERR_CIPHER_SUITE                ERR_HANDSHAKE_FAILURE + 3
#define ERR_COMPRESSION_ALG             ERR_HANDSHAKE_FAILURE + 4
#define ERR_SEQNUM_MODE                 ERR_HANDSHAKE_FAILURE + 5
#define ERR_KEY_REFRESH                 ERR_HANDSHAKE_FAILURE + 6
#define ERR_SERVER_FINISHED             ERR_HANDSHAKE_FAILURE + 7
#define ERR_KEY_EXCHANGE_IDX            ERR_HANDSHAKE_FAILURE + 8
#define ERR_KEY_EXCHANGE_DECODE         ERR_HANDSHAKE_FAILURE + 9
#define ERR_UNEXPECTED_KEY_EXCHANGE     ERR_HANDSHAKE_FAILURE + 10
#define ERR_MISSING_KEY_EXCHANGE        ERR_HANDSHAKE_FAILURE + 11
#define ERR_MISSING_SERVER_CERTIFICATE  ERR_HANDSHAKE_FAILURE + 12
#define ERR_BAD_CCS                     ERR_HANDSHAKE_FAILURE + 13
#define ERR_MAX_RETRANSMISSIONS         ERR_HANDSHAKE_FAILURE + 14
#define ERR_UNSUPPORTED_CERTIFICATE     ERR_HANDSHAKE_FAILURE + 15
#define ERR_UNKNOWN_CA                  ERR_HANDSHAKE_FAILURE + 16
#define ERR_BAD_CERTIFICATE             ERR_HANDSHAKE_FAILURE + 17
#define ERR_CERTIFICATE_EXPIRED         ERR_HANDSHAKE_FAILURE + 18
#define ERR_MISSING_MESSAGE             ERR_HANDSHAKE_FAILURE + 19

/* CRYPTLIB */
#define ERR_ENCRYPT                     ERR_CRYPTLIB + 2
#define ERR_ENCRYPT_INIT                ERR_CRYPTLIB + 3
#define ERR_ENCRYPT_UPDATE              ERR_CRYPTLIB + 4
#define ERR_ENCRYPT_FINAL               ERR_CRYPTLIB + 5
#define ERR_DECRYPT                     ERR_CRYPTLIB + 6
#define ERR_HASH                        ERR_CRYPTLIB + 7
#define ERR_HASH_INIT                   ERR_CRYPTLIB + 8
#define ERR_HASH_UPDATE                 ERR_CRYPTLIB + 9
#define ERR_HASH_FINAL                  ERR_CRYPTLIB + 10
#define ERR_KEY_EXCHANGE                ERR_CRYPTLIB + 11
#define ERR_PRF                         ERR_CRYPTLIB + 17
#define ERR_HASH_KEY_TOO_LONG           ERR_CRYPTLIB + 18
#define ERR_GENERATE_RANDOM             ERR_CRYPTLIB + 19
#define ERR_GET_CIPHER_METHODS          ERR_CRYPTLIB + 20
#define ERR_GET_TRUSTED_IDS             ERR_CRYPTLIB + 21
#define ERR_GET_CLIENT_IDS              ERR_CRYPTLIB + 22
#define ERR_CRYPTLIB_INIT               ERR_CRYPTLIB + 23
#define ERR_CRYPTLIB_TERMINATE          ERR_CRYPTLIB + 24
#define ERR_GET_METHODS                 ERR_CRYPTLIB + 25
#define ERR_GET_CLIENT_CERTIFICATE      ERR_CRYPTLIB + 26
#define ERR_COMPUTE_SIGNATURE           ERR_CRYPTLIB + 27

/* RECEIVED_ALERT */
#define ERR_FATAL_ALERT                 ERR_RECEIVED_ALERT + 1
#define ERR_CRITICAL_ALERT              ERR_RECEIVED_ALERT + 2
#define ERR_TOO_MANY_WARNINGS           ERR_RECEIVED_ALERT + 3

/* INTERNAL */
#define ERR_ENCODING                    ERR_INTERNAL + 1
#define ERR_INSUFFICIENT_MEMORY         ERR_INTERNAL + 2
#define ERR_MISSING_CONNECTION_NODE     ERR_INTERNAL + 3
#define ERR_MISSING_RECORD_BUFFER       ERR_INTERNAL + 4
#define ERR_PROGRAMMING_ERROR           ERR_INTERNAL + 5

/************************************************************
 * END OF ERROR CODES
 ************************************************************/

/* The keyed MAC algorithms defined by WTLS. */
#define MAC_SHA_0               0
#define MAC_SHA_40              1
#define MAC_SHA_80              2
#define MAC_SHA                 3
#define MAC_SHA_XOR_40          4
#define MAC_MD5_40              5
#define MAC_MD5_80              6
#define MAC_MD5                 7

#endif
